/*
 * Copyright © 2018  Ebrahim Byagowi
 * Copyright © 2018  Google, Inc.
 *
 *  This is part of HarfBuzz, a text shaping library.
 *
 * Permission is hereby granted, without written agreement and without
 * license or royalty fees, to use, copy, modify, and distribute this
 * software and its documentation for any purpose, provided that the
 * above copyright notice and the following two paragraphs appear in
 * all copies of this software.
 *
 * IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE TO ANY PARTY FOR
 * DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES
 * ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN
 * IF THE COPYRIGHT HOLDER HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 *
 * THE COPYRIGHT HOLDER SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING,
 * BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS
 * ON AN "AS IS" BASIS, AND THE COPYRIGHT HOLDER HAS NO OBLIGATION TO
 * PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
 *
 * Google Author(s): Behdad Esfahbod
 */

#ifndef HB_AAT_LAYOUT_KERX_TABLE_HH
#define HB_AAT_LAYOUT_KERX_TABLE_HH

#include "hb-kern.hh"
#include "hb-aat-layout-ankr-table.hh"
#include "hb-set-digest.hh"

/*
 * kerx -- Extended Kerning
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6kerx.html
 */
#define HB_AAT_TAG_kerx HB_TAG('k','e','r','x')


namespace AAT {

using namespace OT;


static inline 整数
kerxTupleKern (整数 value,
	       unsigned 整数 tupleCount,
	       const void *base,
	       hb_aat_apply_context_t *c)
{
  如果 (likely (!tupleCount || !c)) 返回 value;

  unsigned 整数 offset = value;
  const FWORD *pv = &StructAtOffset<FWORD> (base, offset);
  如果 (unlikely (!c->sanitizer.check_array (pv, tupleCount))) 返回 0;
  hb_barrier ();
  返回 *pv;
}


struct hb_glyph_pair_t
{
  hb_codepoint_t left;
  hb_codepoint_t right;
};

struct KernPair
{
  整数 get_kerning () const { 返回 value; }

  整数 cmp (const hb_glyph_pair_t &o) const
  {
    整数 ret = left.cmp (o.left);
    如果 (ret) 返回 ret;
    返回 right.cmp (o.right);
  }

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (c->check_struct (this));
  }

  public:
  HBGlyphID16	left;
  HBGlyphID16	right;
  FWORD		value;
  public:
  DEFINE_SIZE_STATIC (6);
};

template <typename KernSubTableHeader>
struct KerxSubTableFormat0
{
  整数 get_kerning (hb_codepoint_t left, hb_codepoint_t right,
		   hb_aat_apply_context_t *c = nullptr) const
  {
    hb_glyph_pair_t pair = {left, right};
    整数 v = pairs.bsearch (pair).get_kerning ();
    返回 kerxTupleKern (v, header.tuple_count (), this, c);
  }

  bool apply (hb_aat_apply_context_t *c) const
  {
    TRACE_APPLY (this);

    如果 (!c->plan->requested_kerning)
      return_trace (false);

    如果 (header.coverage & header.Backwards)
      return_trace (false);

    accelerator_t accel (*this, c);
    hb_kern_machine_t<accelerator_t> machine (accel, header.coverage & header.CrossStream);
    machine.kern (c->font, c->buffer, c->plan->kern_mask);

    return_trace (true);
  }

  template <typename set_t>
  void collect_glyphs (set_t &first_set, set_t &second_set, unsigned num_glyphs) const
  {
    为 (const KernPair& pair : pairs)
    {
      first_set.add (pair.left);
      second_set.add (pair.right);
    }
  }

  struct accelerator_t
  {
    const KerxSubTableFormat0 &table;
    hb_aat_apply_context_t *c;

    accelerator_t (const KerxSubTableFormat0 &table_,
		   hb_aat_apply_context_t *c_) :
		     table (table_), c (c_) {}

    整数 get_kerning (hb_codepoint_t left, hb_codepoint_t right) const
    {
      如果 (!(*c->first_set)[left] || !(*c->second_set)[right]) 返回 0;
      返回 table.get_kerning (left, right, c);
    }
  };


  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (likely (pairs.sanitize (c)));
  }

  protected:
  KernSubTableHeader	header;
  BinSearchArrayOf<KernPair, typename KernSubTableHeader::Types::HBUINT>
			pairs;	/* Sorted kern records. */
  public:
  DEFINE_SIZE_ARRAY (KernSubTableHeader::static_size + 16, pairs);
};


template <bool extended>
struct Format1Entry;

template <>
struct Format1Entry<true>
{
  enum Flags
  {
    Push		= 0x8000,	/* If set, push this glyph on the kerning stack. */
    DontAdvance		= 0x4000,	/* If set, don't advance to the next glyph
					 * before going to the new state. */
    Reset		= 0x2000,	/* If set, reset the kerning data (clear the stack) */
    Reserved		= 0x1FFF,	/* Not used; set to 0. */
  };

  struct EntryData
  {
    HBUINT16	kernActionIndex;/* Index into the kerning value array. If
				 * this index is 0xFFFF, then no kerning
				 * is to be performed. */
    public:
    DEFINE_SIZE_STATIC (2);
  };

  static bool initiateAction (const Entry<EntryData> &entry)
  { 返回 entry.flags & Push; }

  static bool performAction (const Entry<EntryData> &entry)
  { 返回 entry.data.kernActionIndex != 0xFFFF; }

  static unsigned 整数 kernActionIndex (const Entry<EntryData> &entry)
  { 返回 entry.data.kernActionIndex; }
};
template <>
struct Format1Entry<false>
{
  enum Flags
  {
    Push		= 0x8000,	/* If set, push this glyph on the kerning stack. */
    DontAdvance		= 0x4000,	/* If set, don't advance to the next glyph
					 * before going to the new state. */
    Offset		= 0x3FFF,	/* Byte offset from beginning of subtable to the
					 * value table for the glyphs on the kerning stack. */

    Reset		= 0x0000,	/* Not supported? */
  };

  typedef void EntryData;

  static bool initiateAction (const Entry<EntryData> &entry)
  { 返回 entry.flags & Push; }

  static bool performAction (const Entry<EntryData> &entry)
  { 返回 entry.flags & Offset; }

  static unsigned 整数 kernActionIndex (const Entry<EntryData> &entry)
  { 返回 entry.flags & Offset; }
};

template <typename KernSubTableHeader>
struct KerxSubTableFormat1
{
  typedef typename KernSubTableHeader::Types Types;
  typedef typename Types::HBUINT HBUINT;

  typedef Format1Entry<Types::extended> Format1EntryT;
  typedef typename Format1EntryT::EntryData EntryData;

  enum Flags
  {
    DontAdvance	= Format1EntryT::DontAdvance,
  };

  bool is_action_initiable (const Entry<EntryData> &entry) const
  {
    返回 Format1EntryT::initiateAction (entry);
  }
  bool is_actionable (const Entry<EntryData> &entry) const
  {
    返回 Format1EntryT::performAction (entry);
  }

  struct driver_context_t
  {
    static constexpr bool in_place = true;

    driver_context_t (const KerxSubTableFormat1 *table_,
		      hb_aat_apply_context_t *c_) :
	c (c_),
	table (table_),
	/* Apparently the offset kernAction is from the beginning of the state-machine,
	 * similar to offsets in morx table, NOT from beginning of this table, like
	 * other subtables in kerx.  Discovered via testing. */
	kernAction (&table->machine + table->kernAction),
	depth (0),
	crossStream (table->header.coverage & table->header.CrossStream) {}

    HB_AAT_TRANSITION_INLINE
    void transition (hb_buffer_t *buffer,
		     StateTableDriver<Types, EntryData, Flags> *driver,
		     const Entry<EntryData> &entry)
    {
      unsigned 整数 flags = entry.flags;

      如果 (flags & Format1EntryT::Reset)
	depth = 0;

      如果 (flags & Format1EntryT::Push)
      {
	如果 (likely (depth < ARRAY_LENGTH (stack)))
	  stack[depth++] = buffer->idx;
	否则
	  depth = 0; /* Probably not what CoreText does, but better? */
      }

      如果 (Format1EntryT::performAction (entry) && depth)
      {
	unsigned 整数 tuple_count = hb_max (1u, table->header.tuple_count ());

	unsigned 整数 kern_idx = Format1EntryT::kernActionIndex (entry);
	kern_idx = Types::byteOffsetToIndex (kern_idx, &table->machine, kernAction.arrayZ);
	const FWORD *actions = &kernAction[kern_idx];
	如果 (!c->sanitizer.check_array (actions, depth, tuple_count))
	{
	  depth = 0;
	  返回;
	}
	hb_barrier ();

	hb_mask_t kern_mask = c->plan->kern_mask;

	/* From Apple 'kern' spec:
	 * "Each pops one glyph from the kerning stack and applies the kerning value to it.
	 * The end of the list is marked by an odd value... */
	bool last = false;
	当 (!last && depth)
	{
	  unsigned 整数 idx = stack[--depth];
	  整数 v = *actions;
	  actions += tuple_count;
	  如果 (idx >= buffer->len) continue;

	  /* "The end of the list is marked by an odd value..." */
	  last = v & 1;
	  v &= ~1;

	  hb_glyph_position_t &o = buffer->pos[idx];

	  如果 (HB_DIRECTION_IS_HORIZONTAL (buffer->props.direction))
	  {
	    如果 (crossStream)
	    {
	      /* The following flag is undocumented in the spec, but described
	       * in the 'kern' table example. */
	      如果 (v == -0x8000)
	      {
		o.attach_type() = OT::Layout::GPOS_impl::ATTACH_TYPE_NONE;
		o.attach_chain() = 0;
		o.y_offset = 0;
	      }
	      否则 如果 (o.attach_type())
	      {
		o.y_offset = hb_saturate_add (o.y_offset, c->font->em_scale_y (v));
		buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT;
	      }
	    }
	    否则 如果 (buffer->info[idx].mask & kern_mask)
	    {
	      auto scaled = c->font->em_scale_x (v);
	      o.x_advance = hb_saturate_add (o.x_advance, scaled);
	      o.x_offset = hb_saturate_add (o.x_offset, scaled);
	    }
	  }
	  否则
	  {
	    如果 (crossStream)
	    {
	      /* CoreText doesn't do crossStream kerning in vertical.  We do. */
	      如果 (v == -0x8000)
	      {
		o.attach_type() = OT::Layout::GPOS_impl::ATTACH_TYPE_NONE;
		o.attach_chain() = 0;
		o.x_offset = 0;
	      }
	      否则 如果 (o.attach_type())
	      {
		o.x_offset = hb_saturate_add (o.x_offset, c->font->em_scale_x (v));
		buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT;
	      }
	    }
	    否则 如果 (buffer->info[idx].mask & kern_mask)
	    {
	      auto scaled = c->font->em_scale_y (v);
	      o.y_advance = hb_saturate_add (o.y_advance, scaled);
	      o.y_offset = hb_saturate_add (o.y_offset, scaled);
	    }
	  }
	}
      }
    }

    public:
    hb_aat_apply_context_t *c;
    const KerxSubTableFormat1 *table;
    private:
    const UnsizedArrayOf<FWORD> &kernAction;
    unsigned 整数 stack[8];
    unsigned 整数 depth;
    bool crossStream;
  };

  bool apply (hb_aat_apply_context_t *c) const
  {
    TRACE_APPLY (this);

    如果 (!c->plan->requested_kerning &&
	!(header.coverage & header.CrossStream))
      返回 false;

    driver_context_t dc (this, c);

    StateTableDriver<Types, EntryData, Flags> driver (machine, c->font->face);

    driver.drive (&dc, c);

    return_trace (true);
  }

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    /* The rest of array sanitizations are done at run-time. */
    return_trace (likely (c->check_struct (this) &&
			  machine.sanitize (c)));
  }

  template <typename set_t>
  void collect_glyphs (set_t &first_set, set_t &second_set, unsigned num_glyphs) const
  {
    machine.collect_initial_glyphs (first_set, num_glyphs, *this);
    //machine.collect_glyphs (second_set, num_glyphs); // second_set is unused for machine kerning
  }

  protected:
  KernSubTableHeader				header;
  StateTable<Types, EntryData>			machine;
  NNOffsetTo<UnsizedArrayOf<FWORD>, HBUINT>	kernAction;
  public:
  DEFINE_SIZE_STATIC (KernSubTableHeader::static_size + (StateTable<Types, EntryData>::static_size + HBUINT::static_size));
};

template <typename KernSubTableHeader>
struct KerxSubTableFormat2
{
  typedef typename KernSubTableHeader::Types Types;
  typedef typename Types::HBUINT HBUINT;

  整数 get_kerning (hb_codepoint_t left, hb_codepoint_t right,
		   hb_aat_apply_context_t *c) const
  {
    unsigned 整数 num_glyphs = c->sanitizer.get_num_glyphs ();
    unsigned 整数 l = (this+leftClassTable).get_class (left, num_glyphs, 0);
    unsigned 整数 r = (this+rightClassTable).get_class (right, num_glyphs, 0);

    const UnsizedArrayOf<FWORD> &arrayZ = this+array;
    unsigned 整数 kern_idx = l + r;
    kern_idx = Types::offsetToIndex (kern_idx, this, arrayZ.arrayZ);
    const FWORD *v = &arrayZ[kern_idx];
    如果 (unlikely (!v->sanitize (&c->sanitizer))) 返回 0;
    hb_barrier ();

    返回 kerxTupleKern (*v, header.tuple_count (), this, c);
  }

  bool apply (hb_aat_apply_context_t *c) const
  {
    TRACE_APPLY (this);

    如果 (!c->plan->requested_kerning)
      return_trace (false);

    如果 (header.coverage & header.Backwards)
      return_trace (false);

    accelerator_t accel (*this, c);
    hb_kern_machine_t<accelerator_t> machine (accel, header.coverage & header.CrossStream);
    machine.kern (c->font, c->buffer, c->plan->kern_mask);

    return_trace (true);
  }

  template <typename set_t>
  void collect_glyphs (set_t &first_set, set_t &second_set, unsigned num_glyphs) const
  {
    (this+leftClassTable).collect_glyphs (first_set, num_glyphs);
    (this+rightClassTable).collect_glyphs (second_set, num_glyphs);
  }

  struct accelerator_t
  {
    const KerxSubTableFormat2 &table;
    hb_aat_apply_context_t *c;

    accelerator_t (const KerxSubTableFormat2 &table_,
		   hb_aat_apply_context_t *c_) :
		     table (table_), c (c_) {}

    整数 get_kerning (hb_codepoint_t left, hb_codepoint_t right) const
    {
      如果 (!(*c->first_set)[left] || !(*c->second_set)[right]) 返回 0;
      返回 table.get_kerning (left, right, c);
    }
  };

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (likely (c->check_struct (this) &&
			  leftClassTable.sanitize (c, this) &&
			  rightClassTable.sanitize (c, this) &&
			  hb_barrier () &&
			  c->check_range (this, array)));
  }

  protected:
  KernSubTableHeader	header;
  HBUINT		rowWidth;	/* The width, in bytes, of a row in the table. */
  NNOffsetTo<typename Types::ClassTypeWide, HBUINT>
			leftClassTable;	/* Offset from beginning of this subtable to
					 * left-hand class table. */
  NNOffsetTo<typename Types::ClassTypeWide, HBUINT>
			rightClassTable;/* Offset from beginning of this subtable to
					 * right-hand class table. */
  NNOffsetTo<UnsizedArrayOf<FWORD>, HBUINT>
			 array;		/* Offset from beginning of this subtable to
					 * the start of the kerning array. */
  public:
  DEFINE_SIZE_STATIC (KernSubTableHeader::static_size + 4 * sizeof (HBUINT));
};

template <typename KernSubTableHeader>
struct KerxSubTableFormat4
{
  typedef ExtendedTypes Types;

  struct EntryData
  {
    HBUINT16	ankrActionIndex;/* Either 0xFFFF (for no action) or the index of
				 * the action to perform. */
    public:
    DEFINE_SIZE_STATIC (2);
  };

  enum Flags
  {
    Mark		= 0x8000,	/* If set, remember this glyph as the marked glyph. */
    DontAdvance		= 0x4000,	/* If set, don't advance to the next glyph before
					 * going to the new state. */
    Reserved		= 0x3FFF,	/* Not used; set to 0. */
  };

  bool is_action_initiable (const Entry<EntryData> &entry) const
  {
    返回 (entry.flags & Mark);
  }
  bool is_actionable (const Entry<EntryData> &entry) const
  {
    返回 entry.data.ankrActionIndex != 0xFFFF;
  }

  struct driver_context_t
  {
    static constexpr bool in_place = true;
    enum SubTableFlags
    {
      ActionType	= 0xC0000000,	/* A two-bit field containing the action type. */
      Unused		= 0x3F000000,	/* Unused - must be zero. */
      Offset		= 0x00FFFFFF,	/* Masks the offset in bytes from the beginning
					 * of the subtable to the beginning of the control
					 * point table. */
    };

    driver_context_t (const KerxSubTableFormat4 *table_,
		      hb_aat_apply_context_t *c_) :
	c (c_),
	table (table_),
	action_type ((table->flags & ActionType) >> 30),
	ankrData ((HBUINT16 *) ((const char *) &table->machine + (table->flags & Offset))),
	mark_set (false),
	mark (0) {}

    HB_AAT_TRANSITION_INLINE
    void transition (hb_buffer_t *buffer,
		     StateTableDriver<Types, EntryData, Flags> *driver,
		     const Entry<EntryData> &entry)
    {
      如果 (mark_set && entry.data.ankrActionIndex != 0xFFFF && buffer->idx < buffer->len)
      {
	hb_glyph_position_t &o = buffer->cur_pos();
	switch (action_type)
	{
	  case 0: /* Control Point Actions.*/
	  {
	    /* Indexed into glyph outline. */
	    /* Each action (record in ankrData) contains two 16-bit fields, so we must
	       double the ankrActionIndex to get the correct offset here. */
	    const HBUINT16 *data = &ankrData[entry.data.ankrActionIndex * 2];
	    如果 (!c->sanitizer.check_array (data, 2)) 返回;
	    hb_barrier ();
	    unsigned 整数 markControlPoint = *data++;
	    unsigned 整数 currControlPoint = *data++;
	    hb_position_t markX = 0;
	    hb_position_t markY = 0;
	    hb_position_t currX = 0;
	    hb_position_t currY = 0;
	    如果 (!c->font->get_glyph_contour_point_for_origin (c->buffer->info[mark].codepoint,
							      markControlPoint,
							      HB_DIRECTION_LTR /*XXX*/,
							      &markX, &markY) ||
		!c->font->get_glyph_contour_point_for_origin (c->buffer->cur ().codepoint,
							      currControlPoint,
							      HB_DIRECTION_LTR /*XXX*/,
							      &currX, &currY))
	      返回;

	    o.x_offset = hb_saturate_sub (markX, currX);
	    o.y_offset = hb_saturate_sub (markY, currY);
	  }
	  break;

	  case 1: /* Anchor Point Actions. */
	  {
	    /* Indexed into 'ankr' table. */
	    /* Each action (record in ankrData) contains two 16-bit fields, so we must
	       double the ankrActionIndex to get the correct offset here. */
	    const HBUINT16 *data = &ankrData[entry.data.ankrActionIndex * 2];
	    如果 (!c->sanitizer.check_array (data, 2)) 返回;
	    hb_barrier ();
	    unsigned 整数 markAnchorPoint = *data++;
	    unsigned 整数 currAnchorPoint = *data++;
	    const Anchor &markAnchor = c->ankr_table->get_anchor (c->buffer->info[mark].codepoint,
								  markAnchorPoint,
								  c->sanitizer.get_num_glyphs ());
	    const Anchor &currAnchor = c->ankr_table->get_anchor (c->buffer->cur ().codepoint,
								  currAnchorPoint,
								  c->sanitizer.get_num_glyphs ());

	    o.x_offset = hb_saturate_sub (c->font->em_scale_x (markAnchor.xCoordinate),
					  c->font->em_scale_x (currAnchor.xCoordinate));
	    o.y_offset = hb_saturate_sub (c->font->em_scale_y (markAnchor.yCoordinate),
					  c->font->em_scale_y (currAnchor.yCoordinate));
	  }
	  break;

	  case 2: /* Control Point Coordinate Actions. */
	  {
	    /* Each action contains four 16-bit fields, so we multiply the ankrActionIndex
	       by 4 to get the correct offset for the given action. */
	    const FWORD *data = (const FWORD *) &ankrData[entry.data.ankrActionIndex * 4];
	    如果 (!c->sanitizer.check_array (data, 4)) 返回;
	    hb_barrier ();
	    整数 markX = *data++;
	    整数 markY = *data++;
	    整数 currX = *data++;
	    整数 currY = *data++;

	    o.x_offset = hb_saturate_sub (c->font->em_scale_x (markX), c->font->em_scale_x (currX));
	    o.y_offset = hb_saturate_sub (c->font->em_scale_y (markY), c->font->em_scale_y (currY));
	  }
	  break;
	}
	o.attach_type() = OT::Layout::GPOS_impl::ATTACH_TYPE_MARK;
	o.attach_chain() = (整数) mark - (整数) buffer->idx;
	如果 (c->buffer_is_reversed)
	  o.attach_chain() = -o.attach_chain();
	buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT;
      }

      如果 (entry.flags & Mark)
      {
	mark_set = true;
	mark = buffer->idx;
      }
    }

    public:
    hb_aat_apply_context_t *c;
    const KerxSubTableFormat4 *table;
    private:
    unsigned 整数 action_type;
    const HBUINT16 *ankrData;
    bool mark_set;
    unsigned 整数 mark;
  };

  bool apply (hb_aat_apply_context_t *c) const
  {
    TRACE_APPLY (this);

    driver_context_t dc (this, c);

    StateTableDriver<Types, EntryData, Flags> driver (machine, c->font->face);

    driver.drive (&dc, c);

    return_trace (true);
  }

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    /* The rest of array sanitizations are done at run-time. */
    return_trace (likely (c->check_struct (this) &&
			  machine.sanitize (c)));
  }

  template <typename set_t>
  void collect_glyphs (set_t &first_set, set_t &second_set, unsigned num_glyphs) const
  {
    machine.collect_initial_glyphs (first_set, num_glyphs, *this);
    //machine.collect_glyphs (second_set, num_glyphs); // second_set is unused for machine kerning
  }

  protected:
  KernSubTableHeader		header;
  StateTable<Types, EntryData>	machine;
  HBUINT32			flags;
  public:
  DEFINE_SIZE_STATIC (KernSubTableHeader::static_size + (StateTable<Types, EntryData>::static_size + HBUINT32::static_size));
};

template <typename KernSubTableHeader>
struct KerxSubTableFormat6
{
  enum Flags
  {
    ValuesAreLong	= 0x00000001,
  };

  bool is_long () const { 返回 flags & ValuesAreLong; }

  整数 get_kerning (hb_codepoint_t left, hb_codepoint_t right,
		   hb_aat_apply_context_t *c) const
  {
    unsigned 整数 num_glyphs = c->sanitizer.get_num_glyphs ();
    如果 (is_long ())
    {
      const auto &t = u.l;
      unsigned 整数 l = (this+t.rowIndexTable).get_value_or_null (left, num_glyphs);
      unsigned 整数 r = (this+t.columnIndexTable).get_value_or_null (right, num_glyphs);
      unsigned 整数 offset = l + r;
      如果 (unlikely (offset < l)) 返回 0; /* Addition overflow. */
      如果 (unlikely (hb_unsigned_mul_overflows (offset, sizeof (FWORD32)))) 返回 0;
      const FWORD32 *v = &StructAtOffset<FWORD32> (&(this+t.array), offset * sizeof (FWORD32));
      如果 (unlikely (!v->sanitize (&c->sanitizer))) 返回 0;
      hb_barrier ();
      返回 kerxTupleKern (*v, header.tuple_count (), &(this+vector), c);
    }
    否则
    {
      const auto &t = u.s;
      unsigned 整数 l = (this+t.rowIndexTable).get_value_or_null (left, num_glyphs);
      unsigned 整数 r = (this+t.columnIndexTable).get_value_or_null (right, num_glyphs);
      unsigned 整数 offset = l + r;
      const FWORD *v = &StructAtOffset<FWORD> (&(this+t.array), offset * sizeof (FWORD));
      如果 (unlikely (!v->sanitize (&c->sanitizer))) 返回 0;
      hb_barrier ();
      返回 kerxTupleKern (*v, header.tuple_count (), &(this+vector), c);
    }
  }

  bool apply (hb_aat_apply_context_t *c) const
  {
    TRACE_APPLY (this);

    如果 (!c->plan->requested_kerning)
      return_trace (false);

    如果 (header.coverage & header.Backwards)
      return_trace (false);

    accelerator_t accel (*this, c);
    hb_kern_machine_t<accelerator_t> machine (accel, header.coverage & header.CrossStream);
    machine.kern (c->font, c->buffer, c->plan->kern_mask);

    return_trace (true);
  }

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (likely (c->check_struct (this) &&
			  hb_barrier () &&
			  (is_long () ?
			   (
			     u.l.rowIndexTable.sanitize (c, this) &&
			     u.l.columnIndexTable.sanitize (c, this) &&
			     c->check_range (this, u.l.array)
			   ) : (
			     u.s.rowIndexTable.sanitize (c, this) &&
			     u.s.columnIndexTable.sanitize (c, this) &&
			     c->check_range (this, u.s.array)
			   )) &&
			  (header.tuple_count () == 0 ||
			   c->check_range (this, vector))));
  }

  template <typename set_t>
  void collect_glyphs (set_t &first_set, set_t &second_set, unsigned num_glyphs) const
  {
    如果 (is_long ())
    {
      const auto &t = u.l;
      (this+t.rowIndexTable).collect_glyphs (first_set, num_glyphs);
      (this+t.columnIndexTable).collect_glyphs (second_set, num_glyphs);
    }
    否则
    {
      const auto &t = u.s;
      (this+t.rowIndexTable).collect_glyphs (first_set, num_glyphs);
      (this+t.columnIndexTable).collect_glyphs (second_set, num_glyphs);
    }
  }

  struct accelerator_t
  {
    const KerxSubTableFormat6 &table;
    hb_aat_apply_context_t *c;

    accelerator_t (const KerxSubTableFormat6 &table_,
		   hb_aat_apply_context_t *c_) :
		     table (table_), c (c_) {}

    整数 get_kerning (hb_codepoint_t left, hb_codepoint_t right) const
    {
      如果 (!(*c->first_set)[left] || !(*c->second_set)[right]) 返回 0;
      返回 table.get_kerning (left, right, c);
    }
  };

  protected:
  KernSubTableHeader		header;
  HBUINT32			flags;
  HBUINT16			rowCount;
  HBUINT16			columnCount;
  union U
  {
    struct Long
    {
      NNOffset32To<Lookup<HBUINT32>>		rowIndexTable;
      NNOffset32To<Lookup<HBUINT32>>		columnIndexTable;
      NNOffset32To<UnsizedArrayOf<FWORD32>>	array;
    } l;
    struct Short
    {
      NNOffset32To<Lookup<HBUINT16>>		rowIndexTable;
      NNOffset32To<Lookup<HBUINT16>>		columnIndexTable;
      NNOffset32To<UnsizedArrayOf<FWORD>>	array;
    } s;
  } u;
  NNOffset32To<UnsizedArrayOf<FWORD>>	vector;
  public:
  DEFINE_SIZE_STATIC (KernSubTableHeader::static_size + 24);
};


struct KerxSubTableHeader
{
  typedef ExtendedTypes Types;

  unsigned   tuple_count () const { 返回 tupleCount; }
  bool     is_horizontal () const { 返回 !(coverage & Vertical); }

  enum Coverage
  {
    Vertical	= 0x80000000u,	/* Set if table has vertical kerning values. */
    CrossStream	= 0x40000000u,	/* Set if table has cross-stream kerning values. */
    Variation	= 0x20000000u,	/* Set if table has variation kerning values. */
    Backwards	= 0x10000000u,	/* If clear, process the glyphs forwards, that
				 * is, from first to last in the glyph stream.
				 * If we, process them from last to first.
				 * This flag only applies to state-table based
				 * 'kerx' subtables (types 1 and 4). */
    Reserved	= 0x0FFFFF00u,	/* Reserved, set to zero. */
    SubtableType= 0x000000FFu,	/* Subtable type. */
  };

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (c->check_struct (this));
  }

  public:
  HBUINT32	length;
  HBUINT32	coverage;
  HBUINT32	tupleCount;
  public:
  DEFINE_SIZE_STATIC (12);
};

struct KerxSubTable
{
  friend struct kerx;

  size_t get_size () const { 返回 u.header.length; }
  unsigned 整数 get_type () const { 返回 u.header.coverage & u.header.SubtableType; }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) const
  {
    unsigned 整数 subtable_type = get_type ();
    TRACE_DISPATCH (this, subtable_type);
    switch (subtable_type) {
    case 0:	hb_barrier (); return_trace (c->dispatch (u.format0, std::forward<Ts> (ds)...));
    case 1:	hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    case 2:	hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    case 4:	hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
    case 6:	hb_barrier (); return_trace (c->dispatch (u.format6, std::forward<Ts> (ds)...));
    default:	return_trace (c->default_return_value ());
    }
  }

  template <typename set_t>
  void collect_glyphs (set_t &first_set, set_t &second_set, unsigned num_glyphs) const
  {
    unsigned 整数 subtable_type = get_type ();
    switch (subtable_type) {
    case 0:	hb_barrier (); u.format0.collect_glyphs (first_set, second_set, num_glyphs); 返回;
    case 1:	hb_barrier (); u.format1.collect_glyphs (first_set, second_set, num_glyphs); 返回;
    case 2:	hb_barrier (); u.format2.collect_glyphs (first_set, second_set, num_glyphs); 返回;
    case 4:	hb_barrier (); u.format4.collect_glyphs (first_set, second_set, num_glyphs); 返回;
    case 6:	hb_barrier (); u.format6.collect_glyphs (first_set, second_set, num_glyphs); 返回;
    default:	返回;
    }
  }

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    如果 (!(u.header.sanitize (c) &&
	  hb_barrier () &&
	  u.header.length >= u.header.static_size &&
	  c->check_range (this, u.header.length)))
      return_trace (false);

    return_trace (dispatch (c));
  }

  public:
  union {
  KerxSubTableHeader				header;
  KerxSubTableFormat0<KerxSubTableHeader>	format0;
  KerxSubTableFormat1<KerxSubTableHeader>	format1;
  KerxSubTableFormat2<KerxSubTableHeader>	format2;
  KerxSubTableFormat4<KerxSubTableHeader>	format4;
  KerxSubTableFormat6<KerxSubTableHeader>	format6;
  } u;
  public:
  DEFINE_SIZE_MIN (12);
};


/*
 * The 'kerx' Table
 */

struct kern_subtable_accelerator_data_t
{
  hb_bit_set_t first_set;
  hb_bit_set_t second_set;
  mutable hb_aat_class_cache_t class_cache;
};

struct kern_accelerator_data_t
{
  hb_vector_t<kern_subtable_accelerator_data_t> subtable_accels;
  hb_aat_scratch_t scratch;
};

template <typename T>
struct KerxTable
{
  /* https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern */
  const T* thiz () const { 返回 static_cast<const T *> (this); }

  bool has_state_machine () const
  {
    typedef typename T::SubTable SubTable;

    const SubTable *st = &thiz()->firstSubTable;
    unsigned 整数 count = thiz()->tableCount;
    为 (unsigned 整数 i = 0; i < count; i++)
    {
      如果 (st->get_type () == 1)
	返回 true;

      // TODO: What about format 4? What's this API used for anyway?

      st = &StructAfter<SubTable> (*st);
    }
    返回 false;
  }

  bool has_cross_stream () const
  {
    typedef typename T::SubTable SubTable;

    const SubTable *st = &thiz()->firstSubTable;
    unsigned 整数 count = thiz()->tableCount;
    为 (unsigned 整数 i = 0; i < count; i++)
    {
      如果 (st->u.header.coverage & st->u.header.CrossStream)
	返回 true;
      st = &StructAfter<SubTable> (*st);
    }
    返回 false;
  }

  整数 get_h_kerning (hb_codepoint_t left, hb_codepoint_t right) const
  {
    typedef typename T::SubTable SubTable;

    整数 v = 0;
    const SubTable *st = &thiz()->firstSubTable;
    unsigned 整数 count = thiz()->tableCount;
    为 (unsigned 整数 i = 0; i < count; i++)
    {
      如果 ((st->u.header.coverage & (st->u.header.Variation | st->u.header.CrossStream)) ||
	  !st->u.header.is_horizontal ())
	continue;
      v += st->get_kerning (left, right);
      st = &StructAfter<SubTable> (*st);
    }
    返回 v;
  }

  bool apply (AAT::hb_aat_apply_context_t *c,
	      const kern_accelerator_data_t &accel_data) const
  {
    c->buffer->unsafe_to_concat ();

    c->setup_buffer_glyph_set ();

    typedef typename T::SubTable SubTable;

    bool ret = false;
    bool seenCrossStream = false;
    c->set_lookup_index (0);
    const SubTable *st = &thiz()->firstSubTable;
    unsigned 整数 count = thiz()->tableCount;
    为 (unsigned 整数 i = 0; i < count; i++)
    {
      bool reverse;

      auto &subtable_accel = accel_data.subtable_accels[i];

      如果 (!T::Types::extended && (st->u.header.coverage & st->u.header.Variation))
	goto skip;

      如果 (HB_DIRECTION_IS_HORIZONTAL (c->buffer->props.direction) != st->u.header.is_horizontal ())
	goto skip;

      c->first_set = &subtable_accel.first_set;
      c->second_set = &subtable_accel.second_set;
      c->machine_class_cache = &subtable_accel.class_cache;

      如果 (!c->buffer_intersects_machine ())
      {
	(void) c->buffer->message (c->font, "skipped subtable %u because no glyph matches", c->lookup_index);
	goto skip;
      }

      reverse = bool (st->u.header.coverage & st->u.header.Backwards) !=
		HB_DIRECTION_IS_BACKWARD (c->buffer->props.direction);

      如果 (!c->buffer->message (c->font, "start subtable %u", c->lookup_index))
	goto skip;

      如果 (!seenCrossStream &&
	  (st->u.header.coverage & st->u.header.CrossStream))
      {
	/* Attach all glyphs into a chain. */
	seenCrossStream = true;
	hb_glyph_position_t *pos = c->buffer->pos;
	unsigned 整数 count = c->buffer->len;
	为 (unsigned 整数 i = 0; i < count; i++)
	{
	  pos[i].attach_type() = OT::Layout::GPOS_impl::ATTACH_TYPE_CURSIVE;
	  pos[i].attach_chain() = HB_DIRECTION_IS_FORWARD (c->buffer->props.direction) ? -1 : +1;
	  /* We intentionally don't set HB_BUFFER_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT,
	   * since there needs to be a non-zero attachment for post-positioning to
	   * be needed. */
	}
      }

      如果 (reverse != c->buffer_is_reversed)
        c->reverse_buffer ();

      {
	/* See comment in sanitize() for conditional here. */
	hb_sanitize_with_object_t with (&c->sanitizer, i < count - 1 ? st : (const SubTable *) nullptr);
	ret |= st->dispatch (c);
      }

      (void) c->buffer->message (c->font, "end subtable %u", c->lookup_index);

    skip:
      st = &StructAfter<SubTable> (*st);
      c->set_lookup_index (c->lookup_index + 1);
    }
    如果 (c->buffer_is_reversed)
      c->reverse_buffer ();

    返回 ret;
  }

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    如果 (unlikely (!(thiz()->version.sanitize (c) &&
		    hb_barrier () &&
		    (unsigned) thiz()->version >= (unsigned) T::minVersion &&
		    thiz()->tableCount.sanitize (c))))
      return_trace (false);

    typedef typename T::SubTable SubTable;

    const SubTable *st = &thiz()->firstSubTable;
    unsigned 整数 count = thiz()->tableCount;
    为 (unsigned 整数 i = 0; i < count; i++)
    {
      如果 (unlikely (!st->u.header.sanitize (c)))
	return_trace (false);
      hb_barrier ();
      /* OpenType kern table has 2-byte subtable lengths.  That's limiting.
       * MS implementation also only supports one subtable, of format 0,
       * anyway.  Certain versions of some fonts, like Calibry, contain
       * kern subtable that exceeds 64kb.  Looks like, the subtable length
       * is simply ignored.  Which makes sense.  It's only needed if you
       * have multiple subtables.  To handle such fonts, we just ignore
       * the length for the last subtable. */
      hb_sanitize_with_object_t with (c, i < count - 1 ? st : (const SubTable *) nullptr);

      如果 (unlikely (!st->sanitize (c)))
	return_trace (false);

      st = &StructAfter<SubTable> (*st);
    }

    unsigned majorVersion = thiz()->version;
    如果 (sizeof (thiz()->version) == 4)
      majorVersion = majorVersion >> 16;
    如果 (majorVersion >= 3)
    {
      const SubtableGlyphCoverage *coverage = (const SubtableGlyphCoverage *) st;
      如果 (!coverage->sanitize (c, count))
        return_trace (false);
    }

    return_trace (true);
  }

  kern_accelerator_data_t create_accelerator_data (unsigned num_glyphs) const
  {
    kern_accelerator_data_t accel_data;

    typedef typename T::SubTable SubTable;

    const SubTable *st = &thiz()->firstSubTable;
    unsigned 整数 count = thiz()->tableCount;
    为 (unsigned 整数 i = 0; i < count; i++)
    {
      auto &subtable_accel = *accel_data.subtable_accels.push ();
      如果 (unlikely (accel_data.subtable_accels.in_error ()))
	  返回 accel_data;

      st->collect_glyphs (subtable_accel.first_set, subtable_accel.second_set, num_glyphs);
      subtable_accel.class_cache.clear ();

      st = &StructAfter<SubTable> (*st);
    }

    返回 accel_data;
  }

  struct accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      hb_sanitize_context_t sc;
      this->table = sc.reference_table<T> (face);
      this->accel_data = this->table->create_accelerator_data (face->get_num_glyphs ());
    }
    ~accelerator_t ()
    {
      this->table.destroy ();
    }

    hb_blob_t *get_blob () const { 返回 table.get_blob (); }

    bool apply (AAT::hb_aat_apply_context_t *c) const
    {
      返回 table->apply (c, accel_data);
    }

    hb_blob_ptr_t<T> table;
    kern_accelerator_data_t accel_data;
    hb_aat_scratch_t scratch;
  };
};

struct kerx : KerxTable<kerx>
{
  friend struct KerxTable<kerx>;

  static constexpr hb_tag_t tableTag = HB_AAT_TAG_kerx;
  static constexpr unsigned minVersion = 2u;

  typedef KerxSubTableHeader SubTableHeader;
  typedef SubTableHeader::Types Types;
  typedef KerxSubTable SubTable;

  bool has_data () const { 返回 version; }

  protected:
  HBUINT16	version;	/* The version number of the extended kerning table
				 * (currently 2, 3, or 4). */
  HBUINT16	unused;		/* Set to 0. */
  HBUINT32	tableCount;	/* The number of subtables included in the extended kerning
				 * table. */
  SubTable	firstSubTable;	/* Subtables. */
/*subtableGlyphCoverageArray*/	/* Only if version >= 3. We don't use. */

  public:
  DEFINE_SIZE_MIN (8);
};

struct kerx_accelerator_t : kerx::accelerator_t {
  kerx_accelerator_t (hb_face_t *face) : kerx::accelerator_t (face) {}
};

} /* namespace AAT */

#endif /* HB_AAT_LAYOUT_KERX_TABLE_HH */
