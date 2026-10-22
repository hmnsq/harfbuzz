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


static anta_hstha ຈຳນວນເຕັມ
kerxTupleKern (ຈຳນວນເຕັມ value,
	       unsigned ຈຳນວນເຕັມ tupleCount,
	       const void *base,
	       hb_aat_apply_context_t *c)
{
  ຖ້າ (likely (!tupleCount || !c)) ສົ່ງຄືນ value;

  unsigned ຈຳນວນເຕັມ offset = value;
  const FWORD *pv = &StructAtOffset<FWORD> (base, offset);
  ຖ້າ (unlikely (!c->sanitizer.check_array (pv, tupleCount))) ສົ່ງຄືນ 0;
  hb_barrier ();
  ສົ່ງຄືນ *pv;
}


sa_nracanaa hb_glyph_pair_t
{
  hb_codepoint_t left;
  hb_codepoint_t right;
};

sa_nracanaa KernPair
{
  ຈຳນວນເຕັມ get_kerning () const { ສົ່ງຄືນ value; }

  ຈຳນວນເຕັມ cmp (const hb_glyph_pair_t &o) const
  {
    ຈຳນວນເຕັມ ret = left.cmp (o.left);
    ຖ້າ (ret) ສົ່ງຄືນ ret;
    ສົ່ງຄືນ right.cmp (o.right);
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
sa_nracanaa KerxSubTableFormat0
{
  ຈຳນວນເຕັມ get_kerning (hb_codepoint_t left, hb_codepoint_t right,
		   hb_aat_apply_context_t *c = nullptr) const
  {
    hb_glyph_pair_t pair = {left, right};
    ຈຳນວນເຕັມ v = pairs.bsearch (pair).get_kerning ();
    ສົ່ງຄືນ kerxTupleKern (v, header.tuple_count (), this, c);
  }

  bool apply (hb_aat_apply_context_t *c) const
  {
    TRACE_APPLY (this);

    ຖ້າ (!c->plan->requested_kerning)
      return_trace (false);

    ຖ້າ (header.coverage & header.Backwards)
      return_trace (false);

    accelerator_t accel (*this, c);
    hb_kern_machine_t<accelerator_t> machine (accel, header.coverage & header.CrossStream);
    machine.kern (c->font, c->buffer, c->plan->kern_mask);

    return_trace (true);
  }

  template <typename set_t>
  void collect_glyphs (set_t &first_set, set_t &second_set, unsigned num_glyphs) const
  {
    ສຳລັບ (const KernPair& pair : pairs)
    {
      first_set.add (pair.left);
      second_set.add (pair.right);
    }
  }

  sa_nracanaa accelerator_t
  {
    const KerxSubTableFormat0 &table;
    hb_aat_apply_context_t *c;

    accelerator_t (const KerxSubTableFormat0 &table_,
		   hb_aat_apply_context_t *c_) :
		     table (table_), c (c_) {}

    ຈຳນວນເຕັມ get_kerning (hb_codepoint_t left, hb_codepoint_t right) const
    {
      ຖ້າ (!(*c->first_set)[left] || !(*c->second_set)[right]) ສົ່ງຄືນ 0;
      ສົ່ງຄືນ table.get_kerning (left, right, c);
    }
  };


  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (likely (pairs.sanitize (c)));
  }

  rak_sita:
  KernSubTableHeader	header;
  BinSearchArrayOf<KernPair, typename KernSubTableHeader::Types::HBUINT>
			pairs;	/* Sorted kern records. */
  public:
  DEFINE_SIZE_ARRAY (KernSubTableHeader::static_size + 16, pairs);
};


template <bool extended>
sa_nracanaa Format1Entry;

template <>
sa_nracanaa Format1Entry<true>
{
  ga_nanaa Flags
  {
    Push		= 0x8000,	/* If set, push this glyph on the kerning stack. */
    DontAdvance		= 0x4000,	/* If set, don't advance to the next glyph
					 * before going to the new state. */
    Reset		= 0x2000,	/* If set, reset the kerning data (clear the stack) */
    Reserved		= 0x1FFF,	/* Not used; set to 0. */
  };

  sa_nracanaa EntryData
  {
    HBUINT16	kernActionIndex;/* Index into the kerning value array. If
				 * this index is 0xFFFF, then no kerning
				 * is to be performed. */
    public:
    DEFINE_SIZE_STATIC (2);
  };

  static bool initiateAction (const Entry<EntryData> &entry)
  { ສົ່ງຄືນ entry.flags & Push; }

  static bool performAction (const Entry<EntryData> &entry)
  { ສົ່ງຄືນ entry.data.kernActionIndex != 0xFFFF; }

  static unsigned ຈຳນວນເຕັມ kernActionIndex (const Entry<EntryData> &entry)
  { ສົ່ງຄືນ entry.data.kernActionIndex; }
};
template <>
sa_nracanaa Format1Entry<false>
{
  ga_nanaa Flags
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
  { ສົ່ງຄືນ entry.flags & Push; }

  static bool performAction (const Entry<EntryData> &entry)
  { ສົ່ງຄືນ entry.flags & Offset; }

  static unsigned ຈຳນວນເຕັມ kernActionIndex (const Entry<EntryData> &entry)
  { ສົ່ງຄືນ entry.flags & Offset; }
};

template <typename KernSubTableHeader>
sa_nracanaa KerxSubTableFormat1
{
  typedef typename KernSubTableHeader::Types Types;
  typedef typename Types::HBUINT HBUINT;

  typedef Format1Entry<Types::extended> Format1EntryT;
  typedef typename Format1EntryT::EntryData EntryData;

  ga_nanaa Flags
  {
    DontAdvance	= Format1EntryT::DontAdvance,
  };

  bool is_action_initiable (const Entry<EntryData> &entry) const
  {
    ສົ່ງຄືນ Format1EntryT::initiateAction (entry);
  }
  bool is_actionable (const Entry<EntryData> &entry) const
  {
    ສົ່ງຄືນ Format1EntryT::performAction (entry);
  }

  sa_nracanaa driver_context_t
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
      unsigned ຈຳນວນເຕັມ flags = entry.flags;

      ຖ້າ (flags & Format1EntryT::Reset)
	depth = 0;

      ຖ້າ (flags & Format1EntryT::Push)
      {
	ຖ້າ (likely (depth < ARRAY_LENGTH (stack)))
	  stack[depth++] = buffer->idx;
	ບໍ່ດັ່ງນັ້ນ
	  depth = 0; /* Probably not what CoreText does, but better? */
      }

      ຖ້າ (Format1EntryT::performAction (entry) && depth)
      {
	unsigned ຈຳນວນເຕັມ tuple_count = hb_max (1u, table->header.tuple_count ());

	unsigned ຈຳນວນເຕັມ kern_idx = Format1EntryT::kernActionIndex (entry);
	kern_idx = Types::byteOffsetToIndex (kern_idx, &table->machine, kernAction.arrayZ);
	const FWORD *actions = &kernAction[kern_idx];
	ຖ້າ (!c->sanitizer.check_array (actions, depth, tuple_count))
	{
	  depth = 0;
	  ສົ່ງຄືນ;
	}
	hb_barrier ();

	hb_mask_t kern_mask = c->plan->kern_mask;

	/* From Apple 'kern' spec:
	 * "Each pops one glyph from the kerning stack and applies the kerning value to it.
	 * The end of the list is marked by an odd value... */
	bool last = false;
	ໃນຂະນະທີ່ (!last && depth)
	{
	  unsigned ຈຳນວນເຕັມ idx = stack[--depth];
	  ຈຳນວນເຕັມ v = *actions;
	  actions += tuple_count;
	  ຖ້າ (idx >= buffer->len) continue;

	  /* "The end of the list is marked by an odd value..." */
	  last = v & 1;
	  v &= ~1;

	  hb_glyph_position_t &o = buffer->pos[idx];

	  ຖ້າ (HB_DIRECTION_IS_HORIZONTAL (buffer->props.direction))
	  {
	    ຖ້າ (crossStream)
	    {
	      /* The following flag is undocumented in the spec, but described
	       * in the 'kern' table example. */
	      ຖ້າ (v == -0x8000)
	      {
		o.attach_type() = OT::Layout::GPOS_impl::ATTACH_TYPE_NONE;
		o.attach_chain() = 0;
		o.y_offset = 0;
	      }
	      ບໍ່ດັ່ງນັ້ນ ຖ້າ (o.attach_type())
	      {
		o.y_offset = hb_saturate_add (o.y_offset, c->font->em_scale_y (v));
		buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT;
	      }
	    }
	    ບໍ່ດັ່ງນັ້ນ ຖ້າ (buffer->info[idx].mask & kern_mask)
	    {
	      auto scaled = c->font->em_scale_x (v);
	      o.x_advance = hb_saturate_add (o.x_advance, scaled);
	      o.x_offset = hb_saturate_add (o.x_offset, scaled);
	    }
	  }
	  ບໍ່ດັ່ງນັ້ນ
	  {
	    ຖ້າ (crossStream)
	    {
	      /* CoreText doesn't do crossStream kerning in vertical.  We do. */
	      ຖ້າ (v == -0x8000)
	      {
		o.attach_type() = OT::Layout::GPOS_impl::ATTACH_TYPE_NONE;
		o.attach_chain() = 0;
		o.x_offset = 0;
	      }
	      ບໍ່ດັ່ງນັ້ນ ຖ້າ (o.attach_type())
	      {
		o.x_offset = hb_saturate_add (o.x_offset, c->font->em_scale_x (v));
		buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT;
	      }
	    }
	    ບໍ່ດັ່ງນັ້ນ ຖ້າ (buffer->info[idx].mask & kern_mask)
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
    unsigned ຈຳນວນເຕັມ stack[8];
    unsigned ຈຳນວນເຕັມ depth;
    bool crossStream;
  };

  bool apply (hb_aat_apply_context_t *c) const
  {
    TRACE_APPLY (this);

    ຖ້າ (!c->plan->requested_kerning &&
	!(header.coverage & header.CrossStream))
      ສົ່ງຄືນ false;

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

  rak_sita:
  KernSubTableHeader				header;
  StateTable<Types, EntryData>			machine;
  NNOffsetTo<UnsizedArrayOf<FWORD>, HBUINT>	kernAction;
  public:
  DEFINE_SIZE_STATIC (KernSubTableHeader::static_size + (StateTable<Types, EntryData>::static_size + HBUINT::static_size));
};

template <typename KernSubTableHeader>
sa_nracanaa KerxSubTableFormat2
{
  typedef typename KernSubTableHeader::Types Types;
  typedef typename Types::HBUINT HBUINT;

  ຈຳນວນເຕັມ get_kerning (hb_codepoint_t left, hb_codepoint_t right,
		   hb_aat_apply_context_t *c) const
  {
    unsigned ຈຳນວນເຕັມ num_glyphs = c->sanitizer.get_num_glyphs ();
    unsigned ຈຳນວນເຕັມ l = (this+leftClassTable).get_class (left, num_glyphs, 0);
    unsigned ຈຳນວນເຕັມ r = (this+rightClassTable).get_class (right, num_glyphs, 0);

    const UnsizedArrayOf<FWORD> &arrayZ = this+array;
    unsigned ຈຳນວນເຕັມ kern_idx = l + r;
    kern_idx = Types::offsetToIndex (kern_idx, this, arrayZ.arrayZ);
    const FWORD *v = &arrayZ[kern_idx];
    ຖ້າ (unlikely (!v->sanitize (&c->sanitizer))) ສົ່ງຄືນ 0;
    hb_barrier ();

    ສົ່ງຄືນ kerxTupleKern (*v, header.tuple_count (), this, c);
  }

  bool apply (hb_aat_apply_context_t *c) const
  {
    TRACE_APPLY (this);

    ຖ້າ (!c->plan->requested_kerning)
      return_trace (false);

    ຖ້າ (header.coverage & header.Backwards)
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

  sa_nracanaa accelerator_t
  {
    const KerxSubTableFormat2 &table;
    hb_aat_apply_context_t *c;

    accelerator_t (const KerxSubTableFormat2 &table_,
		   hb_aat_apply_context_t *c_) :
		     table (table_), c (c_) {}

    ຈຳນວນເຕັມ get_kerning (hb_codepoint_t left, hb_codepoint_t right) const
    {
      ຖ້າ (!(*c->first_set)[left] || !(*c->second_set)[right]) ສົ່ງຄືນ 0;
      ສົ່ງຄືນ table.get_kerning (left, right, c);
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

  rak_sita:
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
  DEFINE_SIZE_STATIC (KernSubTableHeader::static_size + 4 * parimaa_na (HBUINT));
};

template <typename KernSubTableHeader>
sa_nracanaa KerxSubTableFormat4
{
  typedef ExtendedTypes Types;

  sa_nracanaa EntryData
  {
    HBUINT16	ankrActionIndex;/* Either 0xFFFF (for no action) or the index of
				 * the action to perform. */
    public:
    DEFINE_SIZE_STATIC (2);
  };

  ga_nanaa Flags
  {
    Mark		= 0x8000,	/* If set, remember this glyph as the marked glyph. */
    DontAdvance		= 0x4000,	/* If set, don't advance to the next glyph before
					 * going to the new state. */
    Reserved		= 0x3FFF,	/* Not used; set to 0. */
  };

  bool is_action_initiable (const Entry<EntryData> &entry) const
  {
    ສົ່ງຄືນ (entry.flags & Mark);
  }
  bool is_actionable (const Entry<EntryData> &entry) const
  {
    ສົ່ງຄືນ entry.data.ankrActionIndex != 0xFFFF;
  }

  sa_nracanaa driver_context_t
  {
    static constexpr bool in_place = true;
    ga_nanaa SubTableFlags
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
	ankrData ((HBUINT16 *) ((const var_na *) &table->machine + (table->flags & Offset))),
	mark_set (false),
	mark (0) {}

    HB_AAT_TRANSITION_INLINE
    void transition (hb_buffer_t *buffer,
		     StateTableDriver<Types, EntryData, Flags> *driver,
		     const Entry<EntryData> &entry)
    {
      ຖ້າ (mark_set && entry.data.ankrActionIndex != 0xFFFF && buffer->idx < buffer->len)
      {
	hb_glyph_position_t &o = buffer->cur_pos();
	switch (action_type)
	{
	  pak_sa 0: /* Control Point Actions.*/
	  {
	    /* Indexed into glyph outline. */
	    /* Each action (record in ankrData) contains two 16-bit fields, so we must
	       double the ankrActionIndex to get the correct offset here. */
	    const HBUINT16 *data = &ankrData[entry.data.ankrActionIndex * 2];
	    ຖ້າ (!c->sanitizer.check_array (data, 2)) ສົ່ງຄືນ;
	    hb_barrier ();
	    unsigned ຈຳນວນເຕັມ markControlPoint = *data++;
	    unsigned ຈຳນວນເຕັມ currControlPoint = *data++;
	    hb_position_t markX = 0;
	    hb_position_t markY = 0;
	    hb_position_t currX = 0;
	    hb_position_t currY = 0;
	    ຖ້າ (!c->font->get_glyph_contour_point_for_origin (c->buffer->info[mark].codepoint,
							      markControlPoint,
							      HB_DIRECTION_LTR /*XXX*/,
							      &markX, &markY) ||
		!c->font->get_glyph_contour_point_for_origin (c->buffer->cur ().codepoint,
							      currControlPoint,
							      HB_DIRECTION_LTR /*XXX*/,
							      &currX, &currY))
	      ສົ່ງຄືນ;

	    o.x_offset = hb_saturate_sub (markX, currX);
	    o.y_offset = hb_saturate_sub (markY, currY);
	  }
	  bha_nga;

	  pak_sa 1: /* Anchor Point Actions. */
	  {
	    /* Indexed into 'ankr' table. */
	    /* Each action (record in ankrData) contains two 16-bit fields, so we must
	       double the ankrActionIndex to get the correct offset here. */
	    const HBUINT16 *data = &ankrData[entry.data.ankrActionIndex * 2];
	    ຖ້າ (!c->sanitizer.check_array (data, 2)) ສົ່ງຄືນ;
	    hb_barrier ();
	    unsigned ຈຳນວນເຕັມ markAnchorPoint = *data++;
	    unsigned ຈຳນວນເຕັມ currAnchorPoint = *data++;
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
	  bha_nga;

	  pak_sa 2: /* Control Point Coordinate Actions. */
	  {
	    /* Each action contains four 16-bit fields, so we multiply the ankrActionIndex
	       by 4 to get the correct offset for the given action. */
	    const FWORD *data = (const FWORD *) &ankrData[entry.data.ankrActionIndex * 4];
	    ຖ້າ (!c->sanitizer.check_array (data, 4)) ສົ່ງຄືນ;
	    hb_barrier ();
	    ຈຳນວນເຕັມ markX = *data++;
	    ຈຳນວນເຕັມ markY = *data++;
	    ຈຳນວນເຕັມ currX = *data++;
	    ຈຳນວນເຕັມ currY = *data++;

	    o.x_offset = hb_saturate_sub (c->font->em_scale_x (markX), c->font->em_scale_x (currX));
	    o.y_offset = hb_saturate_sub (c->font->em_scale_y (markY), c->font->em_scale_y (currY));
	  }
	  bha_nga;
	}
	o.attach_type() = OT::Layout::GPOS_impl::ATTACH_TYPE_MARK;
	o.attach_chain() = (ຈຳນວນເຕັມ) mark - (ຈຳນວນເຕັມ) buffer->idx;
	ຖ້າ (c->buffer_is_reversed)
	  o.attach_chain() = -o.attach_chain();
	buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT;
      }

      ຖ້າ (entry.flags & Mark)
      {
	mark_set = true;
	mark = buffer->idx;
      }
    }

    public:
    hb_aat_apply_context_t *c;
    const KerxSubTableFormat4 *table;
    private:
    unsigned ຈຳນວນເຕັມ action_type;
    const HBUINT16 *ankrData;
    bool mark_set;
    unsigned ຈຳນວນເຕັມ mark;
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

  rak_sita:
  KernSubTableHeader		header;
  StateTable<Types, EntryData>	machine;
  HBUINT32			flags;
  public:
  DEFINE_SIZE_STATIC (KernSubTableHeader::static_size + (StateTable<Types, EntryData>::static_size + HBUINT32::static_size));
};

template <typename KernSubTableHeader>
sa_nracanaa KerxSubTableFormat6
{
  ga_nanaa Flags
  {
    ValuesAreLong	= 0x00000001,
  };

  bool is_long () const { ສົ່ງຄືນ flags & ValuesAreLong; }

  ຈຳນວນເຕັມ get_kerning (hb_codepoint_t left, hb_codepoint_t right,
		   hb_aat_apply_context_t *c) const
  {
    unsigned ຈຳນວນເຕັມ num_glyphs = c->sanitizer.get_num_glyphs ();
    ຖ້າ (is_long ())
    {
      const auto &t = u.l;
      unsigned ຈຳນວນເຕັມ l = (this+t.rowIndexTable).get_value_or_null (left, num_glyphs);
      unsigned ຈຳນວນເຕັມ r = (this+t.columnIndexTable).get_value_or_null (right, num_glyphs);
      unsigned ຈຳນວນເຕັມ offset = l + r;
      ຖ້າ (unlikely (offset < l)) ສົ່ງຄືນ 0; /* Addition overflow. */
      ຖ້າ (unlikely (hb_unsigned_mul_overflows (offset, parimaa_na (FWORD32)))) ສົ່ງຄືນ 0;
      const FWORD32 *v = &StructAtOffset<FWORD32> (&(this+t.array), offset * parimaa_na (FWORD32));
      ຖ້າ (unlikely (!v->sanitize (&c->sanitizer))) ສົ່ງຄືນ 0;
      hb_barrier ();
      ສົ່ງຄືນ kerxTupleKern (*v, header.tuple_count (), &(this+vector), c);
    }
    ບໍ່ດັ່ງນັ້ນ
    {
      const auto &t = u.s;
      unsigned ຈຳນວນເຕັມ l = (this+t.rowIndexTable).get_value_or_null (left, num_glyphs);
      unsigned ຈຳນວນເຕັມ r = (this+t.columnIndexTable).get_value_or_null (right, num_glyphs);
      unsigned ຈຳນວນເຕັມ offset = l + r;
      const FWORD *v = &StructAtOffset<FWORD> (&(this+t.array), offset * parimaa_na (FWORD));
      ຖ້າ (unlikely (!v->sanitize (&c->sanitizer))) ສົ່ງຄືນ 0;
      hb_barrier ();
      ສົ່ງຄືນ kerxTupleKern (*v, header.tuple_count (), &(this+vector), c);
    }
  }

  bool apply (hb_aat_apply_context_t *c) const
  {
    TRACE_APPLY (this);

    ຖ້າ (!c->plan->requested_kerning)
      return_trace (false);

    ຖ້າ (header.coverage & header.Backwards)
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
    ຖ້າ (is_long ())
    {
      const auto &t = u.l;
      (this+t.rowIndexTable).collect_glyphs (first_set, num_glyphs);
      (this+t.columnIndexTable).collect_glyphs (second_set, num_glyphs);
    }
    ບໍ່ດັ່ງນັ້ນ
    {
      const auto &t = u.s;
      (this+t.rowIndexTable).collect_glyphs (first_set, num_glyphs);
      (this+t.columnIndexTable).collect_glyphs (second_set, num_glyphs);
    }
  }

  sa_nracanaa accelerator_t
  {
    const KerxSubTableFormat6 &table;
    hb_aat_apply_context_t *c;

    accelerator_t (const KerxSubTableFormat6 &table_,
		   hb_aat_apply_context_t *c_) :
		     table (table_), c (c_) {}

    ຈຳນວນເຕັມ get_kerning (hb_codepoint_t left, hb_codepoint_t right) const
    {
      ຖ້າ (!(*c->first_set)[left] || !(*c->second_set)[right]) ສົ່ງຄືນ 0;
      ສົ່ງຄືນ table.get_kerning (left, right, c);
    }
  };

  rak_sita:
  KernSubTableHeader		header;
  HBUINT32			flags;
  HBUINT16			rowCount;
  HBUINT16			columnCount;
  sa_ngha U
  {
    sa_nracanaa Long
    {
      NNOffset32To<Lookup<HBUINT32>>		rowIndexTable;
      NNOffset32To<Lookup<HBUINT32>>		columnIndexTable;
      NNOffset32To<UnsizedArrayOf<FWORD32>>	array;
    } l;
    sa_nracanaa Short
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


sa_nracanaa KerxSubTableHeader
{
  typedef ExtendedTypes Types;

  unsigned   tuple_count () const { ສົ່ງຄືນ tupleCount; }
  bool     is_horizontal () const { ສົ່ງຄືນ !(coverage & Vertical); }

  ga_nanaa Coverage
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

sa_nracanaa KerxSubTable
{
  friend sa_nracanaa kerx;

  size_t get_size () const { ສົ່ງຄືນ u.header.length; }
  unsigned ຈຳນວນເຕັມ get_type () const { ສົ່ງຄືນ u.header.coverage & u.header.SubtableType; }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) const
  {
    unsigned ຈຳນວນເຕັມ subtable_type = get_type ();
    TRACE_DISPATCH (this, subtable_type);
    switch (subtable_type) {
    pak_sa 0:	hb_barrier (); return_trace (c->dispatch (u.format0, std::forward<Ts> (ds)...));
    pak_sa 1:	hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    pak_sa 2:	hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    pak_sa 4:	hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
    pak_sa 6:	hb_barrier (); return_trace (c->dispatch (u.format6, std::forward<Ts> (ds)...));
    default:	return_trace (c->default_return_value ());
    }
  }

  template <typename set_t>
  void collect_glyphs (set_t &first_set, set_t &second_set, unsigned num_glyphs) const
  {
    unsigned ຈຳນວນເຕັມ subtable_type = get_type ();
    switch (subtable_type) {
    pak_sa 0:	hb_barrier (); u.format0.collect_glyphs (first_set, second_set, num_glyphs); ສົ່ງຄືນ;
    pak_sa 1:	hb_barrier (); u.format1.collect_glyphs (first_set, second_set, num_glyphs); ສົ່ງຄືນ;
    pak_sa 2:	hb_barrier (); u.format2.collect_glyphs (first_set, second_set, num_glyphs); ສົ່ງຄືນ;
    pak_sa 4:	hb_barrier (); u.format4.collect_glyphs (first_set, second_set, num_glyphs); ສົ່ງຄືນ;
    pak_sa 6:	hb_barrier (); u.format6.collect_glyphs (first_set, second_set, num_glyphs); ສົ່ງຄືນ;
    default:	ສົ່ງຄືນ;
    }
  }

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    ຖ້າ (!(u.header.sanitize (c) &&
	  hb_barrier () &&
	  u.header.length >= u.header.static_size &&
	  c->check_range (this, u.header.length)))
      return_trace (false);

    return_trace (dispatch (c));
  }

  public:
  sa_ngha {
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

sa_nracanaa kern_subtable_accelerator_data_t
{
  hb_bit_set_t first_set;
  hb_bit_set_t second_set;
  mutable hb_aat_class_cache_t class_cache;
};

sa_nracanaa kern_accelerator_data_t
{
  hb_vector_t<kern_subtable_accelerator_data_t> subtable_accels;
  hb_aat_scratch_t scratch;
};

template <typename T>
sa_nracanaa KerxTable
{
  /* https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern */
  const T* thiz () const { ສົ່ງຄືນ static_cast<const T *> (this); }

  bool has_state_machine () const
  {
    typedef typename T::SubTable SubTable;

    const SubTable *st = &thiz()->firstSubTable;
    unsigned ຈຳນວນເຕັມ count = thiz()->tableCount;
    ສຳລັບ (unsigned ຈຳນວນເຕັມ i = 0; i < count; i++)
    {
      ຖ້າ (st->get_type () == 1)
	ສົ່ງຄືນ true;

      // TODO: What about format 4? What's this API used for anyway?

      st = &StructAfter<SubTable> (*st);
    }
    ສົ່ງຄືນ false;
  }

  bool has_cross_stream () const
  {
    typedef typename T::SubTable SubTable;

    const SubTable *st = &thiz()->firstSubTable;
    unsigned ຈຳນວນເຕັມ count = thiz()->tableCount;
    ສຳລັບ (unsigned ຈຳນວນເຕັມ i = 0; i < count; i++)
    {
      ຖ້າ (st->u.header.coverage & st->u.header.CrossStream)
	ສົ່ງຄືນ true;
      st = &StructAfter<SubTable> (*st);
    }
    ສົ່ງຄືນ false;
  }

  ຈຳນວນເຕັມ get_h_kerning (hb_codepoint_t left, hb_codepoint_t right) const
  {
    typedef typename T::SubTable SubTable;

    ຈຳນວນເຕັມ v = 0;
    const SubTable *st = &thiz()->firstSubTable;
    unsigned ຈຳນວນເຕັມ count = thiz()->tableCount;
    ສຳລັບ (unsigned ຈຳນວນເຕັມ i = 0; i < count; i++)
    {
      ຖ້າ ((st->u.header.coverage & (st->u.header.Variation | st->u.header.CrossStream)) ||
	  !st->u.header.is_horizontal ())
	continue;
      v += st->get_kerning (left, right);
      st = &StructAfter<SubTable> (*st);
    }
    ສົ່ງຄືນ v;
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
    unsigned ຈຳນວນເຕັມ count = thiz()->tableCount;
    ສຳລັບ (unsigned ຈຳນວນເຕັມ i = 0; i < count; i++)
    {
      bool reverse;

      auto &subtable_accel = accel_data.subtable_accels[i];

      ຖ້າ (!T::Types::extended && (st->u.header.coverage & st->u.header.Variation))
	goto skip;

      ຖ້າ (HB_DIRECTION_IS_HORIZONTAL (c->buffer->props.direction) != st->u.header.is_horizontal ())
	goto skip;

      c->first_set = &subtable_accel.first_set;
      c->second_set = &subtable_accel.second_set;
      c->machine_class_cache = &subtable_accel.class_cache;

      ຖ້າ (!c->buffer_intersects_machine ())
      {
	(void) c->buffer->message (c->font, "skipped subtable %u because no glyph matches", c->lookup_index);
	goto skip;
      }

      reverse = bool (st->u.header.coverage & st->u.header.Backwards) !=
		HB_DIRECTION_IS_BACKWARD (c->buffer->props.direction);

      ຖ້າ (!c->buffer->message (c->font, "start subtable %u", c->lookup_index))
	goto skip;

      ຖ້າ (!seenCrossStream &&
	  (st->u.header.coverage & st->u.header.CrossStream))
      {
	/* Attach all glyphs into a chain. */
	seenCrossStream = true;
	hb_glyph_position_t *pos = c->buffer->pos;
	unsigned ຈຳນວນເຕັມ count = c->buffer->len;
	ສຳລັບ (unsigned ຈຳນວນເຕັມ i = 0; i < count; i++)
	{
	  pos[i].attach_type() = OT::Layout::GPOS_impl::ATTACH_TYPE_CURSIVE;
	  pos[i].attach_chain() = HB_DIRECTION_IS_FORWARD (c->buffer->props.direction) ? -1 : +1;
	  /* We intentionally don't set HB_BUFFER_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT,
	   * since there needs to be a non-zero attachment for post-positioning to
	   * be needed. */
	}
      }

      ຖ້າ (reverse != c->buffer_is_reversed)
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
    ຖ້າ (c->buffer_is_reversed)
      c->reverse_buffer ();

    ສົ່ງຄືນ ret;
  }

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    ຖ້າ (unlikely (!(thiz()->version.sanitize (c) &&
		    hb_barrier () &&
		    (unsigned) thiz()->version >= (unsigned) T::minVersion &&
		    thiz()->tableCount.sanitize (c))))
      return_trace (false);

    typedef typename T::SubTable SubTable;

    const SubTable *st = &thiz()->firstSubTable;
    unsigned ຈຳນວນເຕັມ count = thiz()->tableCount;
    ສຳລັບ (unsigned ຈຳນວນເຕັມ i = 0; i < count; i++)
    {
      ຖ້າ (unlikely (!st->u.header.sanitize (c)))
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

      ຖ້າ (unlikely (!st->sanitize (c)))
	return_trace (false);

      st = &StructAfter<SubTable> (*st);
    }

    unsigned majorVersion = thiz()->version;
    ຖ້າ (parimaa_na (thiz()->version) == 4)
      majorVersion = majorVersion >> 16;
    ຖ້າ (majorVersion >= 3)
    {
      const SubtableGlyphCoverage *coverage = (const SubtableGlyphCoverage *) st;
      ຖ້າ (!coverage->sanitize (c, count))
        return_trace (false);
    }

    return_trace (true);
  }

  kern_accelerator_data_t create_accelerator_data (unsigned num_glyphs) const
  {
    kern_accelerator_data_t accel_data;

    typedef typename T::SubTable SubTable;

    const SubTable *st = &thiz()->firstSubTable;
    unsigned ຈຳນວນເຕັມ count = thiz()->tableCount;
    ສຳລັບ (unsigned ຈຳນວນເຕັມ i = 0; i < count; i++)
    {
      auto &subtable_accel = *accel_data.subtable_accels.push ();
      ຖ້າ (unlikely (accel_data.subtable_accels.in_error ()))
	  ສົ່ງຄືນ accel_data;

      st->collect_glyphs (subtable_accel.first_set, subtable_accel.second_set, num_glyphs);
      subtable_accel.class_cache.clear ();

      st = &StructAfter<SubTable> (*st);
    }

    ສົ່ງຄືນ accel_data;
  }

  sa_nracanaa accelerator_t
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

    hb_blob_t *get_blob () const { ສົ່ງຄືນ table.get_blob (); }

    bool apply (AAT::hb_aat_apply_context_t *c) const
    {
      ສົ່ງຄືນ table->apply (c, accel_data);
    }

    hb_blob_ptr_t<T> table;
    kern_accelerator_data_t accel_data;
    hb_aat_scratch_t scratch;
  };
};

sa_nracanaa kerx : KerxTable<kerx>
{
  friend sa_nracanaa KerxTable<kerx>;

  static constexpr hb_tag_t tableTag = HB_AAT_TAG_kerx;
  static constexpr unsigned minVersion = 2u;

  typedef KerxSubTableHeader SubTableHeader;
  typedef SubTableHeader::Types Types;
  typedef KerxSubTable SubTable;

  bool has_data () const { ສົ່ງຄືນ version; }

  rak_sita:
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

sa_nracanaa kerx_accelerator_t : kerx::accelerator_t {
  kerx_accelerator_t (hb_face_t *face) : kerx::accelerator_t (face) {}
};

} /* namespace AAT */

#endif /* HB_AAT_LAYOUT_KERX_TABLE_HH */
