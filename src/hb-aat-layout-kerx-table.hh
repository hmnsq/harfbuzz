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

#شامل "hb-kern.hh"
#شامل "hb-aat-layout-ankr-table.hh"
#شامل "hb-set-digest.hh"

/*
 * kerx -- Extended Kerning
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6kerx.html
 */
#define HB_AAT_TAG_kerx HB_TAG('k','e','r','x')


فضاینام AAT {

using فضاینام OT;


ایستا درونخطی صحیح
kerxTupleKern (صحیح value,
	       unsigned صحیح tuple_شمار,
	       ثابت تهی *base,
	       hb_aat_apply_context_t *c)
{
  اگر (likely (!tuple_شمار || !c)) بازگشت value;

  unsigned صحیح offset = value;
  ثابت FWORD *pv = &StructAtOffset<FWORD> (base, offset);
  اگر (unlikely (!c->sanitizer.check_array (pv, tuple_شمار))) بازگشت 0;
  hb_barrier ();
  بازگشت *pv;
}


ساختار hb_glyph_pair_t
{
  hb_codepoint_t left;
  hb_codepoint_t راست;
};

ساختار KernPair
{
  صحیح get_kerning () ثابت { بازگشت value; }

  صحیح cmp (ثابت hb_glyph_pair_t &o) ثابت
  {
    صحیح ret = left.cmp (o.left);
    اگر (ret) بازگشت ret;
    بازگشت راست.cmp (o.راست);
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این));
  }

  عمومی:
  HBGlyphID16	left;
  HBGlyphID16	راست;
  FWORD		value;
  عمومی:
  DEFINE_SIZE_STATIC (6);
};

template <typename KernSubTableHeader>
ساختار KerxSubTableFormat0
{
  صحیح get_kerning (hb_codepoint_t left, hb_codepoint_t راست,
		   hb_aat_apply_context_t *c = nullptr) ثابت
  {
    hb_glyph_pair_t pair = {left, راست};
    صحیح v = pairs.bsearch (pair).get_kerning ();
    بازگشت kerxTupleKern (v, header.tuple_شمار_2 (), این, c);
  }

  منطقی apply (hb_aat_apply_context_t *c) ثابت
  {
    TRACE_APPLY (این);

    اگر (!c->plan->requested_kerning)
      return_trace (نادرست);

    اگر (header.coverage & header.Backwards)
      return_trace (نادرست);

    accelerator_t accel (*این, c);
    hb_kern_machine_t<accelerator_t> machine (accel, header.coverage & header.CrossStream);
    machine.kern (c->font, c->buffer, c->plan->kern_mask);

    return_trace (درست);
  }

  template <typename set_t>
  تهی collect_glyphs (set_t &first_set, set_t &second_set, unsigned رقم_glyphs) ثابت
  {
    برای (ثابت KernPair& pair : pairs)
    {
      first_set.add (pair.left);
      second_set.add (pair.راست);
    }
  }

  ساختار accelerator_t
  {
    ثابت KerxSubTableFormat0 &table;
    hb_aat_apply_context_t *c;

    accelerator_t (ثابت KerxSubTableFormat0 &table_,
		   hb_aat_apply_context_t *c_) :
		     table (table_), c (c_) {}

    صحیح get_kerning (hb_codepoint_t left, hb_codepoint_t راست) ثابت
    {
      اگر (!(*c->first_set)[left] || !(*c->second_set)[راست]) بازگشت 0;
      بازگشت table.get_kerning (left, راست, c);
    }
  };


  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (likely (pairs.sanitize (c)));
  }

  محافظتشده:
  KernSubTableHeader	header;
  Bin_جستجو_Array_Of<KernPair, typename KernSubTableHeader::Types::HBUINT>
			pairs;	/* Sorted kern records. */
  عمومی:
  DEFINE_SIZE_ARRAY (KernSubTableHeader::static_size + 16, pairs);
};


template <منطقی extended>
ساختار Format1Entry;

template <>
ساختار Format1Entry<درست>
{
  شمارش Flags
  {
    افزودن		= 0x8000,	/* If set, push this glyph on the kerning stack. */
    DontAdvance		= 0x4000,	/* If set, don't advance to the next glyph
					 * before going to the new state. */
    Reset		= 0x2000,	/* If set, reset the kerning data (clear the stack) */
    Reserved		= 0x1FFF,	/* Not used; set to 0. */
  };

  ساختار EntryData
  {
    HBUINT16	kernActionIndex;/* Index into the kerning value array. If
				 * this index is 0xFFFF, then no kerning
				 * is to be performed. */
    عمومی:
    DEFINE_SIZE_STATIC (2);
  };

  ایستا منطقی initiateAction (ثابت Entry<EntryData> &entry)
  { بازگشت entry.flags & افزودن; }

  ایستا منطقی performAction (ثابت Entry<EntryData> &entry)
  { بازگشت entry.data.kernActionIndex != 0xFFFF; }

  ایستا unsigned صحیح kernActionIndex (ثابت Entry<EntryData> &entry)
  { بازگشت entry.data.kernActionIndex; }
};
template <>
ساختار Format1Entry<نادرست>
{
  شمارش Flags
  {
    افزودن		= 0x8000,	/* If set, push this glyph on the kerning stack. */
    DontAdvance		= 0x4000,	/* If set, don't advance to the next glyph
					 * before going to the new state. */
    Offset		= 0x3FFF,	/* Byte offset from beginning of subtable to the
					 * value table for the glyphs on the kerning stack. */

    Reset		= 0x0000,	/* Not supported? */
  };

  تعریفنوع تهی EntryData;

  ایستا منطقی initiateAction (ثابت Entry<EntryData> &entry)
  { بازگشت entry.flags & افزودن; }

  ایستا منطقی performAction (ثابت Entry<EntryData> &entry)
  { بازگشت entry.flags & Offset; }

  ایستا unsigned صحیح kernActionIndex (ثابت Entry<EntryData> &entry)
  { بازگشت entry.flags & Offset; }
};

template <typename KernSubTableHeader>
ساختار KerxSubTableFormat1
{
  تعریفنوع typename KernSubTableHeader::Types Types;
  تعریفنوع typename Types::HBUINT HBUINT;

  تعریفنوع Format1Entry<Types::extended> Format1EntryT;
  تعریفنوع typename Format1EntryT::EntryData EntryData;

  شمارش Flags
  {
    DontAdvance	= Format1EntryT::DontAdvance,
  };

  منطقی is_action_initiable (ثابت Entry<EntryData> &entry) ثابت
  {
    بازگشت Format1EntryT::initiateAction (entry);
  }
  منطقی is_actionable (ثابت Entry<EntryData> &entry) ثابت
  {
    بازگشت Format1EntryT::performAction (entry);
  }

  ساختار driver_context_t
  {
    ایستا constexpr منطقی in_place = درست;

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
    تهی transition (hb_buffer_t *buffer,
		     StateTableDriver<Types, EntryData, Flags> *driver,
		     ثابت Entry<EntryData> &entry)
    {
      unsigned صحیح flags = entry.flags;

      اگر (flags & Format1EntryT::Reset)
	depth = 0;

      اگر (flags & Format1EntryT::افزودن)
      {
	اگر (likely (depth < ARRAY_LENGTH (پشته)))
	  پشته[depth++] = buffer->idx;
	وگرنه
	  depth = 0; /* Probably not what CoreText does, but better? */
      }

      اگر (Format1EntryT::performAction (entry) && depth)
      {
	unsigned صحیح tuple_شمار_2 = hb_max (1u, table->header.tuple_شمار_2 ());

	unsigned صحیح kern_idx = Format1EntryT::kernActionIndex (entry);
	kern_idx = Types::byteOffsetToIndex (kern_idx, &table->machine, kernAction.arrayZ);
	ثابت FWORD *actions = &kernAction[kern_idx];
	اگر (!c->sanitizer.check_array (actions, depth, tuple_شمار_2))
	{
	  depth = 0;
	  بازگشت;
	}
	hb_barrier ();

	hb_mask_t kern_mask = c->plan->kern_mask;

	/* From Apple 'kern' spec:
	 * "Each pops one glyph from the kerning stack and applies the kerning value to it.
	 * The end of the list is marked by an odd value... */
	منطقی last = نادرست;
	وقتی (!last && depth)
	{
	  unsigned صحیح idx = پشته[--depth];
	  صحیح v = *actions;
	  actions += tuple_شمار_2;
	  اگر (idx >= buffer->len) ادامه;

	  /* "The end of the list is marked by an odd value..." */
	  last = v & 1;
	  v &= ~1;

	  hb_glyph_position_t &o = buffer->pos[idx];

	  اگر (HB_DIRECTION_IS_HORIZONTAL (buffer->props.direction))
	  {
	    اگر (crossStream)
	    {
	      /* The following flag is undocumented in the spec, but described
	       * in the 'kern' table example. */
	      اگر (v == -0x8000)
	      {
		o.attach_type() = OT::Layout::GPOS_impl::ATTACH_TYPE_NONE;
		o.attach_chain() = 0;
		o.y_offset = 0;
	      }
	      وگرنه اگر (o.attach_type())
	      {
		o.y_offset = hb_saturate_add (o.y_offset, c->font->em_scale_y (v));
		buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT;
	      }
	    }
	    وگرنه اگر (buffer->info[idx].mask & kern_mask)
	    {
	      auto scaled = c->font->em_scale_x (v);
	      o.x_advance = hb_saturate_add (o.x_advance, scaled);
	      o.x_offset = hb_saturate_add (o.x_offset, scaled);
	    }
	  }
	  وگرنه
	  {
	    اگر (crossStream)
	    {
	      /* CoreText doesn't do crossStream kerning in vertical.  We do. */
	      اگر (v == -0x8000)
	      {
		o.attach_type() = OT::Layout::GPOS_impl::ATTACH_TYPE_NONE;
		o.attach_chain() = 0;
		o.x_offset = 0;
	      }
	      وگرنه اگر (o.attach_type())
	      {
		o.x_offset = hb_saturate_add (o.x_offset, c->font->em_scale_x (v));
		buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT;
	      }
	    }
	    وگرنه اگر (buffer->info[idx].mask & kern_mask)
	    {
	      auto scaled = c->font->em_scale_y (v);
	      o.y_advance = hb_saturate_add (o.y_advance, scaled);
	      o.y_offset = hb_saturate_add (o.y_offset, scaled);
	    }
	  }
	}
      }
    }

    عمومی:
    hb_aat_apply_context_t *c;
    ثابت KerxSubTableFormat1 *table;
    خصوصی:
    ثابت UnsizedArrayOf<FWORD> &kernAction;
    unsigned صحیح پشته[8];
    unsigned صحیح depth;
    منطقی crossStream;
  };

  منطقی apply (hb_aat_apply_context_t *c) ثابت
  {
    TRACE_APPLY (این);

    اگر (!c->plan->requested_kerning &&
	!(header.coverage & header.CrossStream))
      بازگشت نادرست;

    driver_context_t dc (این, c);

    StateTableDriver<Types, EntryData, Flags> driver (machine, c->font->face);

    driver.drive (&dc, c);

    return_trace (درست);
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    /* The rest of array sanitizations are done at run-time. */
    return_trace (likely (c->check_struct (این) &&
			  machine.sanitize (c)));
  }

  template <typename set_t>
  تهی collect_glyphs (set_t &first_set, set_t &second_set, unsigned رقم_glyphs) ثابت
  {
    machine.collect_initial_glyphs (first_set, رقم_glyphs, *این);
    //machine.collect_glyphs (second_set, num_glyphs); // second_set is unused for machine kerning
  }

  محافظتشده:
  KernSubTableHeader				header;
  StateTable<Types, EntryData>			machine;
  NNOffsetTo<UnsizedArrayOf<FWORD>, HBUINT>	kernAction;
  عمومی:
  DEFINE_SIZE_STATIC (KernSubTableHeader::static_size + (StateTable<Types, EntryData>::static_size + HBUINT::static_size));
};

template <typename KernSubTableHeader>
ساختار KerxSubTableFormat2
{
  تعریفنوع typename KernSubTableHeader::Types Types;
  تعریفنوع typename Types::HBUINT HBUINT;

  صحیح get_kerning (hb_codepoint_t left, hb_codepoint_t راست,
		   hb_aat_apply_context_t *c) ثابت
  {
    unsigned صحیح رقم_glyphs = c->sanitizer.get_رقم_glyphs ();
    unsigned صحیح l = (این+چپ_Class_Table).get_class (left, رقم_glyphs, 0);
    unsigned صحیح r = (این+راست_Class_Table).get_class (راست, رقم_glyphs, 0);

    ثابت UnsizedArrayOf<FWORD> &arrayZ = این+array;
    unsigned صحیح kern_idx = l + r;
    kern_idx = Types::offsetToIndex (kern_idx, این, arrayZ.arrayZ);
    ثابت FWORD *v = &arrayZ[kern_idx];
    اگر (unlikely (!v->sanitize (&c->sanitizer))) بازگشت 0;
    hb_barrier ();

    بازگشت kerxTupleKern (*v, header.tuple_شمار_2 (), این, c);
  }

  منطقی apply (hb_aat_apply_context_t *c) ثابت
  {
    TRACE_APPLY (این);

    اگر (!c->plan->requested_kerning)
      return_trace (نادرست);

    اگر (header.coverage & header.Backwards)
      return_trace (نادرست);

    accelerator_t accel (*این, c);
    hb_kern_machine_t<accelerator_t> machine (accel, header.coverage & header.CrossStream);
    machine.kern (c->font, c->buffer, c->plan->kern_mask);

    return_trace (درست);
  }

  template <typename set_t>
  تهی collect_glyphs (set_t &first_set, set_t &second_set, unsigned رقم_glyphs) ثابت
  {
    (این+چپ_Class_Table).collect_glyphs (first_set, رقم_glyphs);
    (این+راست_Class_Table).collect_glyphs (second_set, رقم_glyphs);
  }

  ساختار accelerator_t
  {
    ثابت KerxSubTableFormat2 &table;
    hb_aat_apply_context_t *c;

    accelerator_t (ثابت KerxSubTableFormat2 &table_,
		   hb_aat_apply_context_t *c_) :
		     table (table_), c (c_) {}

    صحیح get_kerning (hb_codepoint_t left, hb_codepoint_t راست) ثابت
    {
      اگر (!(*c->first_set)[left] || !(*c->second_set)[راست]) بازگشت 0;
      بازگشت table.get_kerning (left, راست, c);
    }
  };

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (likely (c->check_struct (این) &&
			  چپ_Class_Table.sanitize (c, این) &&
			  راست_Class_Table.sanitize (c, این) &&
			  hb_barrier () &&
			  c->check_range (این, array)));
  }

  محافظتشده:
  KernSubTableHeader	header;
  HBUINT		rowWidth;	/* The width, in bytes, of a row in the table. */
  NNOffsetTo<typename Types::ClassTypeWide, HBUINT>
			چپ_Class_Table;	/* Offset from beginning of this subtable to
					 * left-hand class table. */
  NNOffsetTo<typename Types::ClassTypeWide, HBUINT>
			راست_Class_Table;/* Offset from beginning of this subtable to
					 * right-hand class table. */
  NNOffsetTo<UnsizedArrayOf<FWORD>, HBUINT>
			 array;		/* Offset from beginning of this subtable to
					 * the start of the kerning array. */
  عمومی:
  DEFINE_SIZE_STATIC (KernSubTableHeader::static_size + 4 * اندازه (HBUINT));
};

template <typename KernSubTableHeader>
ساختار KerxSubTableFormat4
{
  تعریفنوع ExtendedTypes Types;

  ساختار EntryData
  {
    HBUINT16	ankrActionIndex;/* Either 0xFFFF (for no action) or the index of
				 * the action to perform. */
    عمومی:
    DEFINE_SIZE_STATIC (2);
  };

  شمارش Flags
  {
    Mark		= 0x8000,	/* If set, remember this glyph as the marked glyph. */
    DontAdvance		= 0x4000,	/* If set, don't advance to the next glyph before
					 * going to the new state. */
    Reserved		= 0x3FFF,	/* Not used; set to 0. */
  };

  منطقی is_action_initiable (ثابت Entry<EntryData> &entry) ثابت
  {
    بازگشت (entry.flags & Mark);
  }
  منطقی is_actionable (ثابت Entry<EntryData> &entry) ثابت
  {
    بازگشت entry.data.ankrActionIndex != 0xFFFF;
  }

  ساختار driver_context_t
  {
    ایستا constexpr منطقی in_place = درست;
    شمارش SubTableFlags
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
	ankrData ((HBUINT16 *) ((ثابت نویسه *) &table->machine + (table->flags & Offset))),
	mark_set (نادرست),
	mark (0) {}

    HB_AAT_TRANSITION_INLINE
    تهی transition (hb_buffer_t *buffer,
		     StateTableDriver<Types, EntryData, Flags> *driver,
		     ثابت Entry<EntryData> &entry)
    {
      اگر (mark_set && entry.data.ankrActionIndex != 0xFFFF && buffer->idx < buffer->len)
      {
	hb_glyph_position_t &o = buffer->cur_pos();
	انتخاب (action_type)
	{
	  حالت 0: /* Control Point Actions.*/
	  {
	    /* Indexed into glyph outline. */
	    /* Each action (record in ankrData) contains two 16-bit fields, so we must
	       double the ankrActionIndex to get the correct offset here. */
	    ثابت HBUINT16 *data = &ankrData[entry.data.ankrActionIndex * 2];
	    اگر (!c->sanitizer.check_array (data, 2)) بازگشت;
	    hb_barrier ();
	    unsigned صحیح markControlPoint = *data++;
	    unsigned صحیح currControlPoint = *data++;
	    hb_position_t markX = 0;
	    hb_position_t markY = 0;
	    hb_position_t currX = 0;
	    hb_position_t currY = 0;
	    اگر (!c->font->get_glyph_contour_point_for_origin (c->buffer->info[mark].codepoint,
							      markControlPoint,
							      HB_DIRECTION_LTR /*XXX*/,
							      &markX, &markY) ||
		!c->font->get_glyph_contour_point_for_origin (c->buffer->cur ().codepoint,
							      currControlPoint,
							      HB_DIRECTION_LTR /*XXX*/,
							      &currX, &currY))
	      بازگشت;

	    o.x_offset = hb_saturate_sub (markX, currX);
	    o.y_offset = hb_saturate_sub (markY, currY);
	  }
	  بشکن;

	  حالت 1: /* Anchor Point Actions. */
	  {
	    /* Indexed into 'ankr' table. */
	    /* Each action (record in ankrData) contains two 16-bit fields, so we must
	       double the ankrActionIndex to get the correct offset here. */
	    ثابت HBUINT16 *data = &ankrData[entry.data.ankrActionIndex * 2];
	    اگر (!c->sanitizer.check_array (data, 2)) بازگشت;
	    hb_barrier ();
	    unsigned صحیح markAnchorPoint = *data++;
	    unsigned صحیح currAnchorPoint = *data++;
	    ثابت Anchor &markAnchor = c->ankr_table->get_anchor (c->buffer->info[mark].codepoint,
								  markAnchorPoint,
								  c->sanitizer.get_رقم_glyphs ());
	    ثابت Anchor &currAnchor = c->ankr_table->get_anchor (c->buffer->cur ().codepoint,
								  currAnchorPoint,
								  c->sanitizer.get_رقم_glyphs ());

	    o.x_offset = hb_saturate_sub (c->font->em_scale_x (markAnchor.xCoordinate),
					  c->font->em_scale_x (currAnchor.xCoordinate));
	    o.y_offset = hb_saturate_sub (c->font->em_scale_y (markAnchor.yCoordinate),
					  c->font->em_scale_y (currAnchor.yCoordinate));
	  }
	  بشکن;

	  حالت 2: /* Control Point Coordinate Actions. */
	  {
	    /* Each action contains four 16-bit fields, so we multiply the ankrActionIndex
	       by 4 to get the correct offset for the given action. */
	    ثابت FWORD *data = (ثابت FWORD *) &ankrData[entry.data.ankrActionIndex * 4];
	    اگر (!c->sanitizer.check_array (data, 4)) بازگشت;
	    hb_barrier ();
	    صحیح markX = *data++;
	    صحیح markY = *data++;
	    صحیح currX = *data++;
	    صحیح currY = *data++;

	    o.x_offset = hb_saturate_sub (c->font->em_scale_x (markX), c->font->em_scale_x (currX));
	    o.y_offset = hb_saturate_sub (c->font->em_scale_y (markY), c->font->em_scale_y (currY));
	  }
	  بشکن;
	}
	o.attach_type() = OT::Layout::GPOS_impl::ATTACH_TYPE_MARK;
	o.attach_chain() = (صحیح) mark - (صحیح) buffer->idx;
	اگر (c->buffer_is_reversed)
	  o.attach_chain() = -o.attach_chain();
	buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT;
      }

      اگر (entry.flags & Mark)
      {
	mark_set = درست;
	mark = buffer->idx;
      }
    }

    عمومی:
    hb_aat_apply_context_t *c;
    ثابت KerxSubTableFormat4 *table;
    خصوصی:
    unsigned صحیح action_type;
    ثابت HBUINT16 *ankrData;
    منطقی mark_set;
    unsigned صحیح mark;
  };

  منطقی apply (hb_aat_apply_context_t *c) ثابت
  {
    TRACE_APPLY (این);

    driver_context_t dc (این, c);

    StateTableDriver<Types, EntryData, Flags> driver (machine, c->font->face);

    driver.drive (&dc, c);

    return_trace (درست);
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    /* The rest of array sanitizations are done at run-time. */
    return_trace (likely (c->check_struct (این) &&
			  machine.sanitize (c)));
  }

  template <typename set_t>
  تهی collect_glyphs (set_t &first_set, set_t &second_set, unsigned رقم_glyphs) ثابت
  {
    machine.collect_initial_glyphs (first_set, رقم_glyphs, *این);
    //machine.collect_glyphs (second_set, num_glyphs); // second_set is unused for machine kerning
  }

  محافظتشده:
  KernSubTableHeader		header;
  StateTable<Types, EntryData>	machine;
  HBUINT32			flags;
  عمومی:
  DEFINE_SIZE_STATIC (KernSubTableHeader::static_size + (StateTable<Types, EntryData>::static_size + HBUINT32::static_size));
};

template <typename KernSubTableHeader>
ساختار KerxSubTableFormat6
{
  شمارش Flags
  {
    مقادیر_Are_Long	= 0x00000001,
  };

  منطقی is_long () ثابت { بازگشت flags & مقادیر_Are_Long; }

  صحیح get_kerning (hb_codepoint_t left, hb_codepoint_t راست,
		   hb_aat_apply_context_t *c) ثابت
  {
    unsigned صحیح رقم_glyphs = c->sanitizer.get_رقم_glyphs ();
    اگر (is_long ())
    {
      ثابت auto &t = u.l;
      unsigned صحیح l = (این+t.rowIndexTable).get_مقدار_or_null (left, رقم_glyphs);
      unsigned صحیح r = (این+t.columnIndexTable).get_مقدار_or_null (راست, رقم_glyphs);
      unsigned صحیح offset = l + r;
      اگر (unlikely (offset < l)) بازگشت 0; /* Addition overflow. */
      اگر (unlikely (hb_unsigned_mul_overflows (offset, اندازه (FWORD32)))) بازگشت 0;
      ثابت FWORD32 *v = &StructAtOffset<FWORD32> (&(این+t.array), offset * اندازه (FWORD32));
      اگر (unlikely (!v->sanitize (&c->sanitizer))) بازگشت 0;
      hb_barrier ();
      بازگشت kerxTupleKern (*v, header.tuple_شمار_2 (), &(این+vector), c);
    }
    وگرنه
    {
      ثابت auto &t = u.s;
      unsigned صحیح l = (این+t.rowIndexTable).get_مقدار_or_null (left, رقم_glyphs);
      unsigned صحیح r = (این+t.columnIndexTable).get_مقدار_or_null (راست, رقم_glyphs);
      unsigned صحیح offset = l + r;
      ثابت FWORD *v = &StructAtOffset<FWORD> (&(این+t.array), offset * اندازه (FWORD));
      اگر (unlikely (!v->sanitize (&c->sanitizer))) بازگشت 0;
      hb_barrier ();
      بازگشت kerxTupleKern (*v, header.tuple_شمار_2 (), &(این+vector), c);
    }
  }

  منطقی apply (hb_aat_apply_context_t *c) ثابت
  {
    TRACE_APPLY (این);

    اگر (!c->plan->requested_kerning)
      return_trace (نادرست);

    اگر (header.coverage & header.Backwards)
      return_trace (نادرست);

    accelerator_t accel (*این, c);
    hb_kern_machine_t<accelerator_t> machine (accel, header.coverage & header.CrossStream);
    machine.kern (c->font, c->buffer, c->plan->kern_mask);

    return_trace (درست);
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (likely (c->check_struct (این) &&
			  hb_barrier () &&
			  (is_long () ?
			   (
			     u.l.rowIndexTable.sanitize (c, این) &&
			     u.l.columnIndexTable.sanitize (c, این) &&
			     c->check_range (این, u.l.array)
			   ) : (
			     u.s.rowIndexTable.sanitize (c, این) &&
			     u.s.columnIndexTable.sanitize (c, این) &&
			     c->check_range (این, u.s.array)
			   )) &&
			  (header.tuple_شمار_2 () == 0 ||
			   c->check_range (این, vector))));
  }

  template <typename set_t>
  تهی collect_glyphs (set_t &first_set, set_t &second_set, unsigned رقم_glyphs) ثابت
  {
    اگر (is_long ())
    {
      ثابت auto &t = u.l;
      (این+t.rowIndexTable).collect_glyphs (first_set, رقم_glyphs);
      (این+t.columnIndexTable).collect_glyphs (second_set, رقم_glyphs);
    }
    وگرنه
    {
      ثابت auto &t = u.s;
      (این+t.rowIndexTable).collect_glyphs (first_set, رقم_glyphs);
      (این+t.columnIndexTable).collect_glyphs (second_set, رقم_glyphs);
    }
  }

  ساختار accelerator_t
  {
    ثابت KerxSubTableFormat6 &table;
    hb_aat_apply_context_t *c;

    accelerator_t (ثابت KerxSubTableFormat6 &table_,
		   hb_aat_apply_context_t *c_) :
		     table (table_), c (c_) {}

    صحیح get_kerning (hb_codepoint_t left, hb_codepoint_t راست) ثابت
    {
      اگر (!(*c->first_set)[left] || !(*c->second_set)[راست]) بازگشت 0;
      بازگشت table.get_kerning (left, راست, c);
    }
  };

  محافظتشده:
  KernSubTableHeader		header;
  HBUINT32			flags;
  HBUINT16			row_شمار;
  HBUINT16			column_شمار;
  اجتماع U
  {
    ساختار Long
    {
      NNOffset32To<Lookup<HBUINT32>>		rowIndexTable;
      NNOffset32To<Lookup<HBUINT32>>		columnIndexTable;
      NNOffset32To<UnsizedArrayOf<FWORD32>>	array;
    } l;
    ساختار Short
    {
      NNOffset32To<Lookup<HBUINT16>>		rowIndexTable;
      NNOffset32To<Lookup<HBUINT16>>		columnIndexTable;
      NNOffset32To<UnsizedArrayOf<FWORD>>	array;
    } s;
  } u;
  NNOffset32To<UnsizedArrayOf<FWORD>>	vector;
  عمومی:
  DEFINE_SIZE_STATIC (KernSubTableHeader::static_size + 24);
};


ساختار KerxSubTableHeader
{
  تعریفنوع ExtendedTypes Types;

  unsigned   tuple_شمار_2 () ثابت { بازگشت tuple_شمار; }
  منطقی     is_horizontal () ثابت { بازگشت !(coverage & Vertical); }

  شمارش Coverage
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

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این));
  }

  عمومی:
  HBUINT32	length;
  HBUINT32	coverage;
  HBUINT32	tuple_شمار;
  عمومی:
  DEFINE_SIZE_STATIC (12);
};

ساختار KerxSubTable
{
  friend ساختار kerx;

  size_t get_size () ثابت { بازگشت u.header.length; }
  unsigned صحیح get_type () ثابت { بازگشت u.header.coverage & u.header.SubtableType; }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    unsigned صحیح subtable_type = get_type ();
    TRACE_DISPATCH (این, subtable_type);
    انتخاب (subtable_type) {
    حالت 0:	hb_barrier (); return_trace (c->dispatch (u.format0, std::forward<Ts> (ds)...));
    حالت 1:	hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    حالت 2:	hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    حالت 4:	hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
    حالت 6:	hb_barrier (); return_trace (c->dispatch (u.format6, std::forward<Ts> (ds)...));
    پیشفرض:	return_trace (c->default_return_مقدار ());
    }
  }

  template <typename set_t>
  تهی collect_glyphs (set_t &first_set, set_t &second_set, unsigned رقم_glyphs) ثابت
  {
    unsigned صحیح subtable_type = get_type ();
    انتخاب (subtable_type) {
    حالت 0:	hb_barrier (); u.format0.collect_glyphs (first_set, second_set, رقم_glyphs); بازگشت;
    حالت 1:	hb_barrier (); u.format1.collect_glyphs (first_set, second_set, رقم_glyphs); بازگشت;
    حالت 2:	hb_barrier (); u.format2.collect_glyphs (first_set, second_set, رقم_glyphs); بازگشت;
    حالت 4:	hb_barrier (); u.format4.collect_glyphs (first_set, second_set, رقم_glyphs); بازگشت;
    حالت 6:	hb_barrier (); u.format6.collect_glyphs (first_set, second_set, رقم_glyphs); بازگشت;
    پیشفرض:	بازگشت;
    }
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    اگر (!(u.header.sanitize (c) &&
	  hb_barrier () &&
	  u.header.length >= u.header.static_size &&
	  c->check_range (این, u.header.length)))
      return_trace (نادرست);

    return_trace (dispatch (c));
  }

  عمومی:
  اجتماع {
  KerxSubTableHeader				header;
  KerxSubTableFormat0<KerxSubTableHeader>	format0;
  KerxSubTableFormat1<KerxSubTableHeader>	format1;
  KerxSubTableFormat2<KerxSubTableHeader>	format2;
  KerxSubTableFormat4<KerxSubTableHeader>	format4;
  KerxSubTableFormat6<KerxSubTableHeader>	format6;
  } u;
  عمومی:
  DEFINE_SIZE_MIN (12);
};


/*
 * The 'kerx' Table
 */

ساختار kern_subtable_accelerator_data_t
{
  hb_bit_set_t first_set;
  hb_bit_set_t second_set;
  mutable hb_aat_class_cache_t class_cache;
};

ساختار kern_accelerator_data_t
{
  hb_vector_t<kern_subtable_accelerator_data_t> subtable_accels;
  hb_aat_scratch_t scratch;
};

template <typename T>
ساختار KerxTable
{
  /* https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern */
  ثابت T* thiz () ثابت { بازگشت static_cast<ثابت T *> (این); }

  منطقی has_state_machine () ثابت
  {
    تعریفنوع typename T::SubTable SubTable;

    ثابت SubTable *st = &thiz()->firstSubTable;
    unsigned صحیح شمار = thiz()->table_شمار;
    برای (unsigned صحیح i = 0; i < شمار; i++)
    {
      اگر (st->get_type () == 1)
	بازگشت درست;

      // TODO: What about format 4? What's this API used for anyway?

      st = &StructAfter<SubTable> (*st);
    }
    بازگشت نادرست;
  }

  منطقی has_cross_stream () ثابت
  {
    تعریفنوع typename T::SubTable SubTable;

    ثابت SubTable *st = &thiz()->firstSubTable;
    unsigned صحیح شمار = thiz()->table_شمار;
    برای (unsigned صحیح i = 0; i < شمار; i++)
    {
      اگر (st->u.header.coverage & st->u.header.CrossStream)
	بازگشت درست;
      st = &StructAfter<SubTable> (*st);
    }
    بازگشت نادرست;
  }

  صحیح get_h_kerning (hb_codepoint_t left, hb_codepoint_t راست) ثابت
  {
    تعریفنوع typename T::SubTable SubTable;

    صحیح v = 0;
    ثابت SubTable *st = &thiz()->firstSubTable;
    unsigned صحیح شمار = thiz()->table_شمار;
    برای (unsigned صحیح i = 0; i < شمار; i++)
    {
      اگر ((st->u.header.coverage & (st->u.header.Variation | st->u.header.CrossStream)) ||
	  !st->u.header.is_horizontal ())
	ادامه;
      v += st->get_kerning (left, راست);
      st = &StructAfter<SubTable> (*st);
    }
    بازگشت v;
  }

  منطقی apply (AAT::hb_aat_apply_context_t *c,
	      ثابت kern_accelerator_data_t &accel_data) ثابت
  {
    c->buffer->unsafe_to_concat ();

    c->setup_buffer_glyph_set ();

    تعریفنوع typename T::SubTable SubTable;

    منطقی ret = نادرست;
    منطقی seenCrossStream = نادرست;
    c->set_lookup_index (0);
    ثابت SubTable *st = &thiz()->firstSubTable;
    unsigned صحیح شمار = thiz()->table_شمار;
    برای (unsigned صحیح i = 0; i < شمار; i++)
    {
      منطقی reverse;

      auto &subtable_accel = accel_data.subtable_accels[i];

      اگر (!T::Types::extended && (st->u.header.coverage & st->u.header.Variation))
	برو skip;

      اگر (HB_DIRECTION_IS_HORIZONTAL (c->buffer->props.direction) != st->u.header.is_horizontal ())
	برو skip;

      c->first_set = &subtable_accel.first_set;
      c->second_set = &subtable_accel.second_set;
      c->machine_class_cache = &subtable_accel.class_cache;

      اگر (!c->buffer_intersects_machine ())
      {
	(تهی) c->buffer->message (c->font, "skipped subtable %u because no glyph matches", c->lookup_index);
	برو skip;
      }

      reverse = منطقی (st->u.header.coverage & st->u.header.Backwards) !=
		HB_DIRECTION_IS_BACKWARD (c->buffer->props.direction);

      اگر (!c->buffer->message (c->font, "start subtable %u", c->lookup_index))
	برو skip;

      اگر (!seenCrossStream &&
	  (st->u.header.coverage & st->u.header.CrossStream))
      {
	/* Attach all glyphs into a chain. */
	seenCrossStream = درست;
	hb_glyph_position_t *pos = c->buffer->pos;
	unsigned صحیح شمار = c->buffer->len;
	برای (unsigned صحیح i = 0; i < شمار; i++)
	{
	  pos[i].attach_type() = OT::Layout::GPOS_impl::ATTACH_TYPE_CURSIVE;
	  pos[i].attach_chain() = HB_DIRECTION_IS_FORWARD (c->buffer->props.direction) ? -1 : +1;
	  /* We intentionally don't set HB_BUFFER_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT,
	   * since there needs to be a non-zero attachment for post-positioning to
	   * be needed. */
	}
      }

      اگر (reverse != c->buffer_is_reversed)
        c->reverse_buffer ();

      {
	/* See comment in sanitize() for conditional here. */
	hb_sanitize_with_object_t with (&c->sanitizer, i < شمار - 1 ? st : (ثابت SubTable *) nullptr);
	ret |= st->dispatch (c);
      }

      (تهی) c->buffer->message (c->font, "end subtable %u", c->lookup_index);

    skip:
      st = &StructAfter<SubTable> (*st);
      c->set_lookup_index (c->lookup_index + 1);
    }
    اگر (c->buffer_is_reversed)
      c->reverse_buffer ();

    بازگشت ret;
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    اگر (unlikely (!(thiz()->version.sanitize (c) &&
		    hb_barrier () &&
		    (unsigned) thiz()->version >= (unsigned) T::minVersion &&
		    thiz()->table_شمار.sanitize (c))))
      return_trace (نادرست);

    تعریفنوع typename T::SubTable SubTable;

    ثابت SubTable *st = &thiz()->firstSubTable;
    unsigned صحیح شمار = thiz()->table_شمار;
    برای (unsigned صحیح i = 0; i < شمار; i++)
    {
      اگر (unlikely (!st->u.header.sanitize (c)))
	return_trace (نادرست);
      hb_barrier ();
      /* OpenType kern table has 2-byte subtable lengths.  That's limiting.
       * MS implementation also only supports one subtable, of format 0,
       * anyway.  Certain versions of some fonts, like Calibry, contain
       * kern subtable that exceeds 64kb.  Looks like, the subtable length
       * is simply ignored.  Which makes sense.  It's only needed if you
       * have multiple subtables.  To handle such fonts, we just ignore
       * the length for the last subtable. */
      hb_sanitize_with_object_t with (c, i < شمار - 1 ? st : (ثابت SubTable *) nullptr);

      اگر (unlikely (!st->sanitize (c)))
	return_trace (نادرست);

      st = &StructAfter<SubTable> (*st);
    }

    unsigned majorVersion = thiz()->version;
    اگر (اندازه (thiz()->version) == 4)
      majorVersion = majorVersion >> 16;
    اگر (majorVersion >= 3)
    {
      ثابت SubtableGlyphCoverage *coverage = (ثابت SubtableGlyphCoverage *) st;
      اگر (!coverage->sanitize (c, شمار))
        return_trace (نادرست);
    }

    return_trace (درست);
  }

  kern_accelerator_data_t create_accelerator_data (unsigned رقم_glyphs) ثابت
  {
    kern_accelerator_data_t accel_data;

    تعریفنوع typename T::SubTable SubTable;

    ثابت SubTable *st = &thiz()->firstSubTable;
    unsigned صحیح شمار = thiz()->table_شمار;
    برای (unsigned صحیح i = 0; i < شمار; i++)
    {
      auto &subtable_accel = *accel_data.subtable_accels.push ();
      اگر (unlikely (accel_data.subtable_accels.in_error ()))
	  بازگشت accel_data;

      st->collect_glyphs (subtable_accel.first_set, subtable_accel.second_set, رقم_glyphs);
      subtable_accel.class_cache.clear ();

      st = &StructAfter<SubTable> (*st);
    }

    بازگشت accel_data;
  }

  ساختار accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      hb_sanitize_context_t sc;
      این->table = sc.reference_table<T> (face);
      این->accel_data = این->table->create_accelerator_data (face->get_رقم_glyphs ());
    }
    ~accelerator_t ()
    {
      این->table.destroy ();
    }

    hb_blob_t *get_blob () ثابت { بازگشت table.get_blob (); }

    منطقی apply (AAT::hb_aat_apply_context_t *c) ثابت
    {
      بازگشت table->apply (c, accel_data);
    }

    hb_blob_ptr_t<T> table;
    kern_accelerator_data_t accel_data;
    hb_aat_scratch_t scratch;
  };
};

ساختار kerx : KerxTable<kerx>
{
  friend ساختار KerxTable<kerx>;

  ایستا constexpr hb_tag_t tableTag = HB_AAT_TAG_kerx;
  ایستا constexpr unsigned minVersion = 2u;

  تعریفنوع KerxSubTableHeader SubTableHeader;
  تعریفنوع SubTableHeader::Types Types;
  تعریفنوع KerxSubTable SubTable;

  منطقی has_data () ثابت { بازگشت version; }

  محافظتشده:
  HBUINT16	version;	/* The version number of the extended kerning table
				 * (currently 2, 3, or 4). */
  HBUINT16	unused;		/* Set to 0. */
  HBUINT32	table_شمار;	/* The number of subtables included in the extended kerning
				 * table. */
  SubTable	firstSubTable;	/* Subtables. */
/*subtableGlyphCoverageArray*/	/* Only if version >= 3. We don't use. */

  عمومی:
  DEFINE_SIZE_MIN (8);
};

ساختار kerx_accelerator_t : kerx::accelerator_t {
  kerx_accelerator_t (hb_face_t *face) : kerx::accelerator_t (face) {}
};

} /* namespace AAT */

#endif /* HB_AAT_LAYOUT_KERX_TABLE_HH */
