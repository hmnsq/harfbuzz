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

#ضمن "hb-kern.hh"
#ضمن "hb-aat-layout-ankr-table.hh"
#ضمن "hb-set-digest.hh"

/*
 * kerx -- Extended Kerning
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6kerx.html
 */
#define HB_AAT_TAG_kerx HB_TAG('k','e','r','x')


نطاق AAT {

using نطاق OT;


ساكن مضمن صحيح
kerxTupleKern (صحيح value,
	       unsigned صحيح tuple_گنتی,
	       ثابت فراغ *base,
	       hb_aat_apply_context_t *c)
{
  إذا (likely (!tuple_گنتی || !c)) أرجع value;

  unsigned صحيح offset = value;
  ثابت FWORD *pv = &StructAtOffset<FWORD> (base, offset);
  إذا (unlikely (!c->sanitizer.check_array (pv, tuple_گنتی))) أرجع 0;
  hb_barrier ();
  أرجع *pv;
}


بنية hb_glyph_pair_t
{
  hb_codepoint_t left;
  hb_codepoint_t دایاں;
};

بنية KernPair
{
  صحيح get_kerning () ثابت { أرجع value; }

  صحيح cmp (ثابت hb_glyph_pair_t &o) ثابت
  {
    صحيح ret = left.cmp (o.left);
    إذا (ret) أرجع ret;
    أرجع دایاں.cmp (o.دایاں);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  عام:
  HBGlyphID16	left;
  HBGlyphID16	دایاں;
  FWORD		value;
  عام:
  DEFINE_SIZE_STATIC (6);
};

template <typename KernSubTableHeader>
بنية KerxSubTableFormat0
{
  صحيح get_kerning (hb_codepoint_t left, hb_codepoint_t دایاں,
		   hb_aat_apply_context_t *c = nullptr) ثابت
  {
    hb_glyph_pair_t pair = {left, دایاں};
    صحيح v = pairs.bsearch (pair).get_kerning ();
    أرجع kerxTupleKern (v, header.tuple_گنتی_2 (), هذا, c);
  }

  منطقي apply (hb_aat_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);

    إذا (!c->plan->requested_kerning)
      return_trace (خطأ);

    إذا (header.coverage & header.Backwards)
      return_trace (خطأ);

    accelerator_t accel (*هذا, c);
    hb_kern_machine_t<accelerator_t> machine (accel, header.coverage & header.CrossStream);
    machine.kern (c->font, c->buffer, c->plan->kern_mask);

    return_trace (true);
  }

  template <typename set_t>
  فراغ collect_glyphs (set_t &first_set, set_t &second_set, unsigned عدد_glyphs) ثابت
  {
    لكل (ثابت KernPair& pair : pairs)
    {
      first_set.add (pair.left);
      second_set.add (pair.دایاں);
    }
  }

  بنية accelerator_t
  {
    ثابت KerxSubTableFormat0 &table;
    hb_aat_apply_context_t *c;

    accelerator_t (ثابت KerxSubTableFormat0 &table_,
		   hb_aat_apply_context_t *c_) :
		     table (table_), c (c_) {}

    صحيح get_kerning (hb_codepoint_t left, hb_codepoint_t دایاں) ثابت
    {
      إذا (!(*c->first_set)[left] || !(*c->second_set)[دایاں]) أرجع 0;
      أرجع table.get_kerning (left, دایاں, c);
    }
  };


  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (pairs.sanitize (c)));
  }

  محمي:
  KernSubTableHeader	header;
  Bin_تلاش_Array_Of<KernPair, typename KernSubTableHeader::Types::HBUINT>
			pairs;	/* Sorted kern records. */
  عام:
  DEFINE_SIZE_ARRAY (KernSubTableHeader::static_size + 16, pairs);
};


template <منطقي extended>
بنية Format1Entry;

template <>
بنية Format1Entry<true>
{
  تعداد Flags
  {
    ڈالو		= 0x8000,	/* If set, push this glyph on the kerning stack. */
    DontAdvance		= 0x4000,	/* If set, don't advance to the next glyph
					 * before going to the new state. */
    Reset		= 0x2000,	/* If set, reset the kerning data (clear the stack) */
    Reserved		= 0x1FFF,	/* Not used; set to 0. */
  };

  بنية EntryData
  {
    HBUINT16	kernActionIndex;/* Index into the kerning value array. If
				 * this index is 0xFFFF, then no kerning
				 * is to be performed. */
    عام:
    DEFINE_SIZE_STATIC (2);
  };

  ساكن منطقي initiateAction (ثابت Entry<EntryData> &entry)
  { أرجع entry.flags & ڈالو; }

  ساكن منطقي performAction (ثابت Entry<EntryData> &entry)
  { أرجع entry.data.kernActionIndex != 0xFFFF; }

  ساكن unsigned صحيح kernActionIndex (ثابت Entry<EntryData> &entry)
  { أرجع entry.data.kernActionIndex; }
};
template <>
بنية Format1Entry<خطأ>
{
  تعداد Flags
  {
    ڈالو		= 0x8000,	/* If set, push this glyph on the kerning stack. */
    DontAdvance		= 0x4000,	/* If set, don't advance to the next glyph
					 * before going to the new state. */
    Offset		= 0x3FFF,	/* Byte offset from beginning of subtable to the
					 * value table for the glyphs on the kerning stack. */

    Reset		= 0x0000,	/* Not supported? */
  };

  تعريفنوع فراغ EntryData;

  ساكن منطقي initiateAction (ثابت Entry<EntryData> &entry)
  { أرجع entry.flags & ڈالو; }

  ساكن منطقي performAction (ثابت Entry<EntryData> &entry)
  { أرجع entry.flags & Offset; }

  ساكن unsigned صحيح kernActionIndex (ثابت Entry<EntryData> &entry)
  { أرجع entry.flags & Offset; }
};

template <typename KernSubTableHeader>
بنية KerxSubTableFormat1
{
  تعريفنوع typename KernSubTableHeader::Types Types;
  تعريفنوع typename Types::HBUINT HBUINT;

  تعريفنوع Format1Entry<Types::extended> Format1EntryT;
  تعريفنوع typename Format1EntryT::EntryData EntryData;

  تعداد Flags
  {
    DontAdvance	= Format1EntryT::DontAdvance,
  };

  منطقي is_action_initiable (ثابت Entry<EntryData> &entry) ثابت
  {
    أرجع Format1EntryT::initiateAction (entry);
  }
  منطقي is_actionable (ثابت Entry<EntryData> &entry) ثابت
  {
    أرجع Format1EntryT::performAction (entry);
  }

  بنية driver_context_t
  {
    ساكن constexpr منطقي in_place = true;

    driver_context_t (ثابت KerxSubTableFormat1 *table_,
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
    فراغ transition (hb_buffer_t *buffer,
		     StateTableDriver<Types, EntryData, Flags> *driver,
		     ثابت Entry<EntryData> &entry)
    {
      unsigned صحيح flags = entry.flags;

      إذا (flags & Format1EntryT::Reset)
	depth = 0;

      إذا (flags & Format1EntryT::ڈالو)
      {
	إذا (likely (depth < ARRAY_LENGTH (ڈھیر)))
	  ڈھیر[depth++] = buffer->idx;
	وإلا
	  depth = 0; /* Probably not what CoreText does, but better? */
      }

      إذا (Format1EntryT::performAction (entry) && depth)
      {
	unsigned صحيح tuple_گنتی_2 = hb_max (1u, table->header.tuple_گنتی_2 ());

	unsigned صحيح kern_idx = Format1EntryT::kernActionIndex (entry);
	kern_idx = Types::byteOffsetToIndex (kern_idx, &table->machine, kernAction.arrayZ);
	ثابت FWORD *actions = &kernAction[kern_idx];
	إذا (!c->sanitizer.check_array (actions, depth, tuple_گنتی_2))
	{
	  depth = 0;
	  أرجع;
	}
	hb_barrier ();

	hb_mask_t kern_mask = c->plan->kern_mask;

	/* From Apple 'kern' spec:
	 * "Each pops one glyph from the kerning stack and applies the kerning value to it.
	 * The end of the list is marked by an odd value... */
	منطقي last = خطأ;
	طالما (!last && depth)
	{
	  unsigned صحيح idx = ڈھیر[--depth];
	  صحيح v = *actions;
	  actions += tuple_گنتی_2;
	  إذا (idx >= buffer->len) استمر;

	  /* "The end of the list is marked by an odd value..." */
	  last = v & 1;
	  v &= ~1;

	  hb_glyph_position_t &o = buffer->pos[idx];

	  إذا (HB_DIRECTION_IS_HORIZONTAL (buffer->props.direction))
	  {
	    إذا (crossStream)
	    {
	      /* The following flag is undocumented in the spec, but described
	       * in the 'kern' table example. */
	      إذا (v == -0x8000)
	      {
		o.attach_type() = OT::Layout::GPOS_impl::ATTACH_TYPE_NONE;
		o.attach_chain() = 0;
		o.y_offset = 0;
	      }
	      وإلا إذا (o.attach_type())
	      {
		o.y_offset = hb_saturate_add (o.y_offset, c->font->em_scale_y (v));
		buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT;
	      }
	    }
	    وإلا إذا (buffer->info[idx].mask & kern_mask)
	    {
	      auto scaled = c->font->em_scale_x (v);
	      o.x_advance = hb_saturate_add (o.x_advance, scaled);
	      o.x_offset = hb_saturate_add (o.x_offset, scaled);
	    }
	  }
	  وإلا
	  {
	    إذا (crossStream)
	    {
	      /* CoreText doesn't do crossStream kerning in vertical.  We do. */
	      إذا (v == -0x8000)
	      {
		o.attach_type() = OT::Layout::GPOS_impl::ATTACH_TYPE_NONE;
		o.attach_chain() = 0;
		o.x_offset = 0;
	      }
	      وإلا إذا (o.attach_type())
	      {
		o.x_offset = hb_saturate_add (o.x_offset, c->font->em_scale_x (v));
		buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT;
	      }
	    }
	    وإلا إذا (buffer->info[idx].mask & kern_mask)
	    {
	      auto scaled = c->font->em_scale_y (v);
	      o.y_advance = hb_saturate_add (o.y_advance, scaled);
	      o.y_offset = hb_saturate_add (o.y_offset, scaled);
	    }
	  }
	}
      }
    }

    عام:
    hb_aat_apply_context_t *c;
    ثابت KerxSubTableFormat1 *table;
    خاص:
    ثابت UnsizedArrayOf<FWORD> &kernAction;
    unsigned صحيح ڈھیر[8];
    unsigned صحيح depth;
    منطقي crossStream;
  };

  منطقي apply (hb_aat_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);

    إذا (!c->plan->requested_kerning &&
	!(header.coverage & header.CrossStream))
      أرجع خطأ;

    driver_context_t dc (هذا, c);

    StateTableDriver<Types, EntryData, Flags> driver (machine, c->font->face);

    driver.drive (&dc, c);

    return_trace (true);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    /* The rest of array sanitizations are done at run-time. */
    return_trace (likely (c->check_struct (هذا) &&
			  machine.sanitize (c)));
  }

  template <typename set_t>
  فراغ collect_glyphs (set_t &first_set, set_t &second_set, unsigned عدد_glyphs) ثابت
  {
    machine.collect_initial_glyphs (first_set, عدد_glyphs, *هذا);
    //machine.collect_glyphs (second_set, num_glyphs); // second_set is unused for machine kerning
  }

  محمي:
  KernSubTableHeader				header;
  StateTable<Types, EntryData>			machine;
  NNOffsetTo<UnsizedArrayOf<FWORD>, HBUINT>	kernAction;
  عام:
  DEFINE_SIZE_STATIC (KernSubTableHeader::static_size + (StateTable<Types, EntryData>::static_size + HBUINT::static_size));
};

template <typename KernSubTableHeader>
بنية KerxSubTableFormat2
{
  تعريفنوع typename KernSubTableHeader::Types Types;
  تعريفنوع typename Types::HBUINT HBUINT;

  صحيح get_kerning (hb_codepoint_t left, hb_codepoint_t دایاں,
		   hb_aat_apply_context_t *c) ثابت
  {
    unsigned صحيح عدد_glyphs = c->sanitizer.get_عدد_glyphs ();
    unsigned صحيح l = (هذا+بایاں_Class_Table).get_class (left, عدد_glyphs, 0);
    unsigned صحيح r = (هذا+دایاں_Class_Table).get_class (دایاں, عدد_glyphs, 0);

    ثابت UnsizedArrayOf<FWORD> &arrayZ = هذا+array;
    unsigned صحيح kern_idx = l + r;
    kern_idx = Types::offsetToIndex (kern_idx, هذا, arrayZ.arrayZ);
    ثابت FWORD *v = &arrayZ[kern_idx];
    إذا (unlikely (!v->sanitize (&c->sanitizer))) أرجع 0;
    hb_barrier ();

    أرجع kerxTupleKern (*v, header.tuple_گنتی_2 (), هذا, c);
  }

  منطقي apply (hb_aat_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);

    إذا (!c->plan->requested_kerning)
      return_trace (خطأ);

    إذا (header.coverage & header.Backwards)
      return_trace (خطأ);

    accelerator_t accel (*هذا, c);
    hb_kern_machine_t<accelerator_t> machine (accel, header.coverage & header.CrossStream);
    machine.kern (c->font, c->buffer, c->plan->kern_mask);

    return_trace (true);
  }

  template <typename set_t>
  فراغ collect_glyphs (set_t &first_set, set_t &second_set, unsigned عدد_glyphs) ثابت
  {
    (هذا+بایاں_Class_Table).collect_glyphs (first_set, عدد_glyphs);
    (هذا+دایاں_Class_Table).collect_glyphs (second_set, عدد_glyphs);
  }

  بنية accelerator_t
  {
    ثابت KerxSubTableFormat2 &table;
    hb_aat_apply_context_t *c;

    accelerator_t (ثابت KerxSubTableFormat2 &table_,
		   hb_aat_apply_context_t *c_) :
		     table (table_), c (c_) {}

    صحيح get_kerning (hb_codepoint_t left, hb_codepoint_t دایاں) ثابت
    {
      إذا (!(*c->first_set)[left] || !(*c->second_set)[دایاں]) أرجع 0;
      أرجع table.get_kerning (left, دایاں, c);
    }
  };

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (c->check_struct (هذا) &&
			  بایاں_Class_Table.sanitize (c, هذا) &&
			  دایاں_Class_Table.sanitize (c, هذا) &&
			  hb_barrier () &&
			  c->check_range (هذا, array)));
  }

  محمي:
  KernSubTableHeader	header;
  HBUINT		rowWidth;	/* The width, in bytes, of a row in the table. */
  NNOffsetTo<typename Types::ClassTypeWide, HBUINT>
			بایاں_Class_Table;	/* Offset from beginning of this subtable to
					 * left-hand class table. */
  NNOffsetTo<typename Types::ClassTypeWide, HBUINT>
			دایاں_Class_Table;/* Offset from beginning of this subtable to
					 * right-hand class table. */
  NNOffsetTo<UnsizedArrayOf<FWORD>, HBUINT>
			 array;		/* Offset from beginning of this subtable to
					 * the start of the kerning array. */
  عام:
  DEFINE_SIZE_STATIC (KernSubTableHeader::static_size + 4 * حجم (HBUINT));
};

template <typename KernSubTableHeader>
بنية KerxSubTableFormat4
{
  تعريفنوع ExtendedTypes Types;

  بنية EntryData
  {
    HBUINT16	ankrActionIndex;/* Either 0xFFFF (for no action) or the index of
				 * the action to perform. */
    عام:
    DEFINE_SIZE_STATIC (2);
  };

  تعداد Flags
  {
    Mark		= 0x8000,	/* If set, remember this glyph as the marked glyph. */
    DontAdvance		= 0x4000,	/* If set, don't advance to the next glyph before
					 * going to the new state. */
    Reserved		= 0x3FFF,	/* Not used; set to 0. */
  };

  منطقي is_action_initiable (ثابت Entry<EntryData> &entry) ثابت
  {
    أرجع (entry.flags & Mark);
  }
  منطقي is_actionable (ثابت Entry<EntryData> &entry) ثابت
  {
    أرجع entry.data.ankrActionIndex != 0xFFFF;
  }

  بنية driver_context_t
  {
    ساكن constexpr منطقي in_place = true;
    تعداد SubTableFlags
    {
      ActionType	= 0xC0000000,	/* A two-bit field containing the action type. */
      Unused		= 0x3F000000,	/* Unused - must be zero. */
      Offset		= 0x00FFFFFF,	/* Masks the offset in bytes from the beginning
					 * of the subtable to the beginning of the control
					 * point table. */
    };

    driver_context_t (ثابت KerxSubTableFormat4 *table_,
		      hb_aat_apply_context_t *c_) :
	c (c_),
	table (table_),
	action_type ((table->flags & ActionType) >> 30),
	ankrData ((HBUINT16 *) ((ثابت حرف *) &table->machine + (table->flags & Offset))),
	mark_set (خطأ),
	mark (0) {}

    HB_AAT_TRANSITION_INLINE
    فراغ transition (hb_buffer_t *buffer,
		     StateTableDriver<Types, EntryData, Flags> *driver,
		     ثابت Entry<EntryData> &entry)
    {
      إذا (mark_set && entry.data.ankrActionIndex != 0xFFFF && buffer->idx < buffer->len)
      {
	hb_glyph_position_t &o = buffer->cur_pos();
	بدل (action_type)
	{
	  حالة 0: /* Control Point Actions.*/
	  {
	    /* Indexed into glyph outline. */
	    /* Each action (record in ankrData) contains two 16-bit fields, so we must
	       double the ankrActionIndex to get the correct offset here. */
	    ثابت HBUINT16 *data = &ankrData[entry.data.ankrActionIndex * 2];
	    إذا (!c->sanitizer.check_array (data, 2)) أرجع;
	    hb_barrier ();
	    unsigned صحيح markControlPoint = *data++;
	    unsigned صحيح currControlPoint = *data++;
	    hb_position_t markX = 0;
	    hb_position_t markY = 0;
	    hb_position_t currX = 0;
	    hb_position_t currY = 0;
	    إذا (!c->font->get_glyph_contour_point_for_origin (c->buffer->info[mark].codepoint,
							      markControlPoint,
							      HB_DIRECTION_LTR /*XXX*/,
							      &markX, &markY) ||
		!c->font->get_glyph_contour_point_for_origin (c->buffer->cur ().codepoint,
							      currControlPoint,
							      HB_DIRECTION_LTR /*XXX*/,
							      &currX, &currY))
	      أرجع;

	    o.x_offset = hb_saturate_sub (markX, currX);
	    o.y_offset = hb_saturate_sub (markY, currY);
	  }
	  اكسر;

	  حالة 1: /* Anchor Point Actions. */
	  {
	    /* Indexed into 'ankr' table. */
	    /* Each action (record in ankrData) contains two 16-bit fields, so we must
	       double the ankrActionIndex to get the correct offset here. */
	    ثابت HBUINT16 *data = &ankrData[entry.data.ankrActionIndex * 2];
	    إذا (!c->sanitizer.check_array (data, 2)) أرجع;
	    hb_barrier ();
	    unsigned صحيح markAnchorPoint = *data++;
	    unsigned صحيح currAnchorPoint = *data++;
	    ثابت Anchor &markAnchor = c->ankr_table->get_anchor (c->buffer->info[mark].codepoint,
								  markAnchorPoint,
								  c->sanitizer.get_عدد_glyphs ());
	    ثابت Anchor &currAnchor = c->ankr_table->get_anchor (c->buffer->cur ().codepoint,
								  currAnchorPoint,
								  c->sanitizer.get_عدد_glyphs ());

	    o.x_offset = hb_saturate_sub (c->font->em_scale_x (markAnchor.xCoordinate),
					  c->font->em_scale_x (currAnchor.xCoordinate));
	    o.y_offset = hb_saturate_sub (c->font->em_scale_y (markAnchor.yCoordinate),
					  c->font->em_scale_y (currAnchor.yCoordinate));
	  }
	  اكسر;

	  حالة 2: /* Control Point Coordinate Actions. */
	  {
	    /* Each action contains four 16-bit fields, so we multiply the ankrActionIndex
	       by 4 to get the correct offset for the given action. */
	    ثابت FWORD *data = (ثابت FWORD *) &ankrData[entry.data.ankrActionIndex * 4];
	    إذا (!c->sanitizer.check_array (data, 4)) أرجع;
	    hb_barrier ();
	    صحيح markX = *data++;
	    صحيح markY = *data++;
	    صحيح currX = *data++;
	    صحيح currY = *data++;

	    o.x_offset = hb_saturate_sub (c->font->em_scale_x (markX), c->font->em_scale_x (currX));
	    o.y_offset = hb_saturate_sub (c->font->em_scale_y (markY), c->font->em_scale_y (currY));
	  }
	  اكسر;
	}
	o.attach_type() = OT::Layout::GPOS_impl::ATTACH_TYPE_MARK;
	o.attach_chain() = (صحيح) mark - (صحيح) buffer->idx;
	إذا (c->buffer_is_reversed)
	  o.attach_chain() = -o.attach_chain();
	buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT;
      }

      إذا (entry.flags & Mark)
      {
	mark_set = true;
	mark = buffer->idx;
      }
    }

    عام:
    hb_aat_apply_context_t *c;
    ثابت KerxSubTableFormat4 *table;
    خاص:
    unsigned صحيح action_type;
    ثابت HBUINT16 *ankrData;
    منطقي mark_set;
    unsigned صحيح mark;
  };

  منطقي apply (hb_aat_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);

    driver_context_t dc (هذا, c);

    StateTableDriver<Types, EntryData, Flags> driver (machine, c->font->face);

    driver.drive (&dc, c);

    return_trace (true);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    /* The rest of array sanitizations are done at run-time. */
    return_trace (likely (c->check_struct (هذا) &&
			  machine.sanitize (c)));
  }

  template <typename set_t>
  فراغ collect_glyphs (set_t &first_set, set_t &second_set, unsigned عدد_glyphs) ثابت
  {
    machine.collect_initial_glyphs (first_set, عدد_glyphs, *هذا);
    //machine.collect_glyphs (second_set, num_glyphs); // second_set is unused for machine kerning
  }

  محمي:
  KernSubTableHeader		header;
  StateTable<Types, EntryData>	machine;
  HBUINT32			flags;
  عام:
  DEFINE_SIZE_STATIC (KernSubTableHeader::static_size + (StateTable<Types, EntryData>::static_size + HBUINT32::static_size));
};

template <typename KernSubTableHeader>
بنية KerxSubTableFormat6
{
  تعداد Flags
  {
    اقدار_Are_Long	= 0x00000001,
  };

  منطقي is_long () ثابت { أرجع flags & اقدار_Are_Long; }

  صحيح get_kerning (hb_codepoint_t left, hb_codepoint_t دایاں,
		   hb_aat_apply_context_t *c) ثابت
  {
    unsigned صحيح عدد_glyphs = c->sanitizer.get_عدد_glyphs ();
    إذا (is_long ())
    {
      ثابت auto &t = u.l;
      unsigned صحيح l = (هذا+t.rowIndexTable).get_قدر_or_null (left, عدد_glyphs);
      unsigned صحيح r = (هذا+t.columnIndexTable).get_قدر_or_null (دایاں, عدد_glyphs);
      unsigned صحيح offset = l + r;
      إذا (unlikely (offset < l)) أرجع 0; /* Addition overflow. */
      إذا (unlikely (hb_unsigned_mul_overflows (offset, حجم (FWORD32)))) أرجع 0;
      ثابت FWORD32 *v = &StructAtOffset<FWORD32> (&(هذا+t.array), offset * حجم (FWORD32));
      إذا (unlikely (!v->sanitize (&c->sanitizer))) أرجع 0;
      hb_barrier ();
      أرجع kerxTupleKern (*v, header.tuple_گنتی_2 (), &(هذا+vector), c);
    }
    وإلا
    {
      ثابت auto &t = u.s;
      unsigned صحيح l = (هذا+t.rowIndexTable).get_قدر_or_null (left, عدد_glyphs);
      unsigned صحيح r = (هذا+t.columnIndexTable).get_قدر_or_null (دایاں, عدد_glyphs);
      unsigned صحيح offset = l + r;
      ثابت FWORD *v = &StructAtOffset<FWORD> (&(هذا+t.array), offset * حجم (FWORD));
      إذا (unlikely (!v->sanitize (&c->sanitizer))) أرجع 0;
      hb_barrier ();
      أرجع kerxTupleKern (*v, header.tuple_گنتی_2 (), &(هذا+vector), c);
    }
  }

  منطقي apply (hb_aat_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);

    إذا (!c->plan->requested_kerning)
      return_trace (خطأ);

    إذا (header.coverage & header.Backwards)
      return_trace (خطأ);

    accelerator_t accel (*هذا, c);
    hb_kern_machine_t<accelerator_t> machine (accel, header.coverage & header.CrossStream);
    machine.kern (c->font, c->buffer, c->plan->kern_mask);

    return_trace (true);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (c->check_struct (هذا) &&
			  hb_barrier () &&
			  (is_long () ?
			   (
			     u.l.rowIndexTable.sanitize (c, هذا) &&
			     u.l.columnIndexTable.sanitize (c, هذا) &&
			     c->check_range (هذا, u.l.array)
			   ) : (
			     u.s.rowIndexTable.sanitize (c, هذا) &&
			     u.s.columnIndexTable.sanitize (c, هذا) &&
			     c->check_range (هذا, u.s.array)
			   )) &&
			  (header.tuple_گنتی_2 () == 0 ||
			   c->check_range (هذا, vector))));
  }

  template <typename set_t>
  فراغ collect_glyphs (set_t &first_set, set_t &second_set, unsigned عدد_glyphs) ثابت
  {
    إذا (is_long ())
    {
      ثابت auto &t = u.l;
      (هذا+t.rowIndexTable).collect_glyphs (first_set, عدد_glyphs);
      (هذا+t.columnIndexTable).collect_glyphs (second_set, عدد_glyphs);
    }
    وإلا
    {
      ثابت auto &t = u.s;
      (هذا+t.rowIndexTable).collect_glyphs (first_set, عدد_glyphs);
      (هذا+t.columnIndexTable).collect_glyphs (second_set, عدد_glyphs);
    }
  }

  بنية accelerator_t
  {
    ثابت KerxSubTableFormat6 &table;
    hb_aat_apply_context_t *c;

    accelerator_t (ثابت KerxSubTableFormat6 &table_,
		   hb_aat_apply_context_t *c_) :
		     table (table_), c (c_) {}

    صحيح get_kerning (hb_codepoint_t left, hb_codepoint_t دایاں) ثابت
    {
      إذا (!(*c->first_set)[left] || !(*c->second_set)[دایاں]) أرجع 0;
      أرجع table.get_kerning (left, دایاں, c);
    }
  };

  محمي:
  KernSubTableHeader		header;
  HBUINT32			flags;
  HBUINT16			row_گنتی;
  HBUINT16			column_گنتی;
  اتحاد U
  {
    بنية Long
    {
      NNOffset32To<Lookup<HBUINT32>>		rowIndexTable;
      NNOffset32To<Lookup<HBUINT32>>		columnIndexTable;
      NNOffset32To<UnsizedArrayOf<FWORD32>>	array;
    } l;
    بنية Short
    {
      NNOffset32To<Lookup<HBUINT16>>		rowIndexTable;
      NNOffset32To<Lookup<HBUINT16>>		columnIndexTable;
      NNOffset32To<UnsizedArrayOf<FWORD>>	array;
    } s;
  } u;
  NNOffset32To<UnsizedArrayOf<FWORD>>	vector;
  عام:
  DEFINE_SIZE_STATIC (KernSubTableHeader::static_size + 24);
};


بنية KerxSubTableHeader
{
  تعريفنوع ExtendedTypes Types;

  unsigned   tuple_گنتی_2 () ثابت { أرجع tuple_گنتی; }
  منطقي     is_horizontal () ثابت { أرجع !(coverage & Vertical); }

  تعداد Coverage
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

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  عام:
  HBUINT32	length;
  HBUINT32	coverage;
  HBUINT32	tuple_گنتی;
  عام:
  DEFINE_SIZE_STATIC (12);
};

بنية KerxSubTable
{
  friend بنية kerx;

  size_t get_size () ثابت { أرجع u.header.length; }
  unsigned صحيح get_type () ثابت { أرجع u.header.coverage & u.header.SubtableType; }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    unsigned صحيح subtable_type = get_type ();
    TRACE_DISPATCH (هذا, subtable_type);
    بدل (subtable_type) {
    حالة 0:	hb_barrier (); return_trace (c->dispatch (u.format0, std::forward<Ts> (ds)...));
    حالة 1:	hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    حالة 2:	hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    حالة 4:	hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
    حالة 6:	hb_barrier (); return_trace (c->dispatch (u.format6, std::forward<Ts> (ds)...));
    افتراضي:	return_trace (c->default_return_قدر ());
    }
  }

  template <typename set_t>
  فراغ collect_glyphs (set_t &first_set, set_t &second_set, unsigned عدد_glyphs) ثابت
  {
    unsigned صحيح subtable_type = get_type ();
    بدل (subtable_type) {
    حالة 0:	hb_barrier (); u.format0.collect_glyphs (first_set, second_set, عدد_glyphs); أرجع;
    حالة 1:	hb_barrier (); u.format1.collect_glyphs (first_set, second_set, عدد_glyphs); أرجع;
    حالة 2:	hb_barrier (); u.format2.collect_glyphs (first_set, second_set, عدد_glyphs); أرجع;
    حالة 4:	hb_barrier (); u.format4.collect_glyphs (first_set, second_set, عدد_glyphs); أرجع;
    حالة 6:	hb_barrier (); u.format6.collect_glyphs (first_set, second_set, عدد_glyphs); أرجع;
    افتراضي:	أرجع;
    }
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (!(u.header.sanitize (c) &&
	  hb_barrier () &&
	  u.header.length >= u.header.static_size &&
	  c->check_range (هذا, u.header.length)))
      return_trace (خطأ);

    return_trace (dispatch (c));
  }

  عام:
  اتحاد {
  KerxSubTableHeader				header;
  KerxSubTableFormat0<KerxSubTableHeader>	format0;
  KerxSubTableFormat1<KerxSubTableHeader>	format1;
  KerxSubTableFormat2<KerxSubTableHeader>	format2;
  KerxSubTableFormat4<KerxSubTableHeader>	format4;
  KerxSubTableFormat6<KerxSubTableHeader>	format6;
  } u;
  عام:
  DEFINE_SIZE_MIN (12);
};


/*
 * The 'kerx' Table
 */

بنية kern_subtable_accelerator_data_t
{
  hb_bit_set_t first_set;
  hb_bit_set_t second_set;
  mutable hb_aat_class_cache_t class_cache;
};

بنية kern_accelerator_data_t
{
  hb_vector_t<kern_subtable_accelerator_data_t> subtable_accels;
  hb_aat_scratch_t scratch;
};

template <typename T>
بنية KerxTable
{
  /* https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern */
  ثابت T* thiz () ثابت { أرجع static_cast<ثابت T *> (هذا); }

  منطقي has_state_machine () ثابت
  {
    تعريفنوع typename T::SubTable SubTable;

    ثابت SubTable *st = &thiz()->firstSubTable;
    unsigned صحيح گنتی = thiz()->table_گنتی;
    لكل (unsigned صحيح i = 0; i < گنتی; i++)
    {
      إذا (st->get_type () == 1)
	أرجع true;

      // TODO: What about format 4? What's this API used for anyway?

      st = &StructAfter<SubTable> (*st);
    }
    أرجع خطأ;
  }

  منطقي has_cross_stream () ثابت
  {
    تعريفنوع typename T::SubTable SubTable;

    ثابت SubTable *st = &thiz()->firstSubTable;
    unsigned صحيح گنتی = thiz()->table_گنتی;
    لكل (unsigned صحيح i = 0; i < گنتی; i++)
    {
      إذا (st->u.header.coverage & st->u.header.CrossStream)
	أرجع true;
      st = &StructAfter<SubTable> (*st);
    }
    أرجع خطأ;
  }

  صحيح get_h_kerning (hb_codepoint_t left, hb_codepoint_t دایاں) ثابت
  {
    تعريفنوع typename T::SubTable SubTable;

    صحيح v = 0;
    ثابت SubTable *st = &thiz()->firstSubTable;
    unsigned صحيح گنتی = thiz()->table_گنتی;
    لكل (unsigned صحيح i = 0; i < گنتی; i++)
    {
      إذا ((st->u.header.coverage & (st->u.header.Variation | st->u.header.CrossStream)) ||
	  !st->u.header.is_horizontal ())
	استمر;
      v += st->get_kerning (left, دایاں);
      st = &StructAfter<SubTable> (*st);
    }
    أرجع v;
  }

  منطقي apply (AAT::hb_aat_apply_context_t *c,
	      ثابت kern_accelerator_data_t &accel_data) ثابت
  {
    c->buffer->unsafe_to_concat ();

    c->setup_buffer_glyph_set ();

    تعريفنوع typename T::SubTable SubTable;

    منطقي ret = خطأ;
    منطقي seenCrossStream = خطأ;
    c->set_lookup_index (0);
    ثابت SubTable *st = &thiz()->firstSubTable;
    unsigned صحيح گنتی = thiz()->table_گنتی;
    لكل (unsigned صحيح i = 0; i < گنتی; i++)
    {
      منطقي reverse;

      auto &subtable_accel = accel_data.subtable_accels[i];

      إذا (!T::Types::extended && (st->u.header.coverage & st->u.header.Variation))
	اذهب skip;

      إذا (HB_DIRECTION_IS_HORIZONTAL (c->buffer->props.direction) != st->u.header.is_horizontal ())
	اذهب skip;

      c->first_set = &subtable_accel.first_set;
      c->second_set = &subtable_accel.second_set;
      c->machine_class_cache = &subtable_accel.class_cache;

      إذا (!c->buffer_intersects_machine ())
      {
	(فراغ) c->buffer->message (c->font, "skipped subtable %u because no glyph matches", c->lookup_index);
	اذهب skip;
      }

      reverse = منطقي (st->u.header.coverage & st->u.header.Backwards) !=
		HB_DIRECTION_IS_BACKWARD (c->buffer->props.direction);

      إذا (!c->buffer->message (c->font, "start subtable %u", c->lookup_index))
	اذهب skip;

      إذا (!seenCrossStream &&
	  (st->u.header.coverage & st->u.header.CrossStream))
      {
	/* Attach all glyphs into a chain. */
	seenCrossStream = true;
	hb_glyph_position_t *pos = c->buffer->pos;
	unsigned صحيح گنتی = c->buffer->len;
	لكل (unsigned صحيح i = 0; i < گنتی; i++)
	{
	  pos[i].attach_type() = OT::Layout::GPOS_impl::ATTACH_TYPE_CURSIVE;
	  pos[i].attach_chain() = HB_DIRECTION_IS_FORWARD (c->buffer->props.direction) ? -1 : +1;
	  /* We intentionally don't set HB_BUFFER_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT,
	   * since there needs to be a non-zero attachment for post-positioning to
	   * be needed. */
	}
      }

      إذا (reverse != c->buffer_is_reversed)
        c->reverse_buffer ();

      {
	/* See comment in sanitize() for conditional here. */
	hb_sanitize_with_object_t with (&c->sanitizer, i < گنتی - 1 ? st : (ثابت SubTable *) nullptr);
	ret |= st->dispatch (c);
      }

      (فراغ) c->buffer->message (c->font, "end subtable %u", c->lookup_index);

    skip:
      st = &StructAfter<SubTable> (*st);
      c->set_lookup_index (c->lookup_index + 1);
    }
    إذا (c->buffer_is_reversed)
      c->reverse_buffer ();

    أرجع ret;
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (unlikely (!(thiz()->version.sanitize (c) &&
		    hb_barrier () &&
		    (unsigned) thiz()->version >= (unsigned) T::minVersion &&
		    thiz()->table_گنتی.sanitize (c))))
      return_trace (خطأ);

    تعريفنوع typename T::SubTable SubTable;

    ثابت SubTable *st = &thiz()->firstSubTable;
    unsigned صحيح گنتی = thiz()->table_گنتی;
    لكل (unsigned صحيح i = 0; i < گنتی; i++)
    {
      إذا (unlikely (!st->u.header.sanitize (c)))
	return_trace (خطأ);
      hb_barrier ();
      /* OpenType kern table has 2-byte subtable lengths.  That's limiting.
       * MS implementation also only supports one subtable, of format 0,
       * anyway.  Certain versions of some fonts, like Calibry, contain
       * kern subtable that exceeds 64kb.  Looks like, the subtable length
       * is simply ignored.  Which makes sense.  It's only needed if you
       * have multiple subtables.  To handle such fonts, we just ignore
       * the length for the last subtable. */
      hb_sanitize_with_object_t with (c, i < گنتی - 1 ? st : (ثابت SubTable *) nullptr);

      إذا (unlikely (!st->sanitize (c)))
	return_trace (خطأ);

      st = &StructAfter<SubTable> (*st);
    }

    unsigned majorVersion = thiz()->version;
    إذا (حجم (thiz()->version) == 4)
      majorVersion = majorVersion >> 16;
    إذا (majorVersion >= 3)
    {
      ثابت SubtableGlyphCoverage *coverage = (ثابت SubtableGlyphCoverage *) st;
      إذا (!coverage->sanitize (c, گنتی))
        return_trace (خطأ);
    }

    return_trace (true);
  }

  kern_accelerator_data_t create_accelerator_data (unsigned عدد_glyphs) ثابت
  {
    kern_accelerator_data_t accel_data;

    تعريفنوع typename T::SubTable SubTable;

    ثابت SubTable *st = &thiz()->firstSubTable;
    unsigned صحيح گنتی = thiz()->table_گنتی;
    لكل (unsigned صحيح i = 0; i < گنتی; i++)
    {
      auto &subtable_accel = *accel_data.subtable_accels.push ();
      إذا (unlikely (accel_data.subtable_accels.in_error ()))
	  أرجع accel_data;

      st->collect_glyphs (subtable_accel.first_set, subtable_accel.second_set, عدد_glyphs);
      subtable_accel.class_cache.clear ();

      st = &StructAfter<SubTable> (*st);
    }

    أرجع accel_data;
  }

  بنية accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      hb_sanitize_context_t sc;
      هذا->table = sc.reference_table<T> (face);
      هذا->accel_data = هذا->table->create_accelerator_data (face->get_عدد_glyphs ());
    }
    ~accelerator_t ()
    {
      هذا->table.destroy ();
    }

    hb_blob_t *get_blob () ثابت { أرجع table.get_blob (); }

    منطقي apply (AAT::hb_aat_apply_context_t *c) ثابت
    {
      أرجع table->apply (c, accel_data);
    }

    hb_blob_ptr_t<T> table;
    kern_accelerator_data_t accel_data;
    hb_aat_scratch_t scratch;
  };
};

بنية kerx : KerxTable<kerx>
{
  friend بنية KerxTable<kerx>;

  ساكن constexpr hb_tag_t tableTag = HB_AAT_TAG_kerx;
  ساكن constexpr unsigned minVersion = 2u;

  تعريفنوع KerxSubTableHeader SubTableHeader;
  تعريفنوع SubTableHeader::Types Types;
  تعريفنوع KerxSubTable SubTable;

  منطقي has_data () ثابت { أرجع version; }

  محمي:
  HBUINT16	version;	/* The version number of the extended kerning table
				 * (currently 2, 3, or 4). */
  HBUINT16	unused;		/* Set to 0. */
  HBUINT32	table_گنتی;	/* The number of subtables included in the extended kerning
				 * table. */
  SubTable	firstSubTable;	/* Subtables. */
/*subtableGlyphCoverageArray*/	/* Only if version >= 3. We don't use. */

  عام:
  DEFINE_SIZE_MIN (8);
};

بنية kerx_accelerator_t : kerx::accelerator_t {
  kerx_accelerator_t (hb_face_t *face) : kerx::accelerator_t (face) {}
};

} /* namespace AAT */

#endif /* HB_AAT_LAYOUT_KERX_TABLE_HH */
