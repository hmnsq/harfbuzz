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

#অন্তর্ভাব "hb-kern.hh"
#অন্তর্ভাব "hb-aat-layout-ankr-table.hh"
#অন্তর্ভাব "hb-set-digest.hh"

/*
 * kerx -- Extended Kerning
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6kerx.html
 */
#define HB_AAT_TAG_kerx HB_TAG('k','e','r','x')


নামাকাশ AAT {

using নামাকাশ OT;


স্থির অন্তঃস্থ পূর্ণাঙ্ক
kerxTupleKern (পূর্ণাঙ্ক value,
	       unsigned পূর্ণাঙ্ক tuple_গুনতি,
	       ধ্রুব শূন্য *base,
	       hb_aat_apply_context_t *c)
{
  যদি (likely (!tuple_গুনতি || !c)) প্রত্যাগম value;

  unsigned পূর্ণাঙ্ক offset = value;
  ধ্রুব FWORD *pv = &StructAtOffset<FWORD> (base, offset);
  যদি (unlikely (!c->sanitizer.check_array (pv, tuple_গুনতি))) প্রত্যাগম 0;
  hb_barrier ();
  প্রত্যাগম *pv;
}


সংরচনা hb_glyph_pair_t
{
  hb_codepoint_t left;
  hb_codepoint_t ডান;
};

সংরচনা KernPair
{
  পূর্ণাঙ্ক get_kerning () ধ্রুব { প্রত্যাগম value; }

  পূর্ণাঙ্ক cmp (ধ্রুব hb_glyph_pair_t &o) ধ্রুব
  {
    পূর্ণাঙ্ক ret = left.cmp (o.left);
    যদি (ret) প্রত্যাগম ret;
    প্রত্যাগম ডান.cmp (o.ডান);
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্));
  }

  সার্বজনিক:
  HBGlyphID16	left;
  HBGlyphID16	ডান;
  FWORD		value;
  সার্বজনিক:
  DEFINE_SIZE_STATIC (6);
};

template <typename KernSubTableHeader>
সংরচনা KerxSubTableFormat0
{
  পূর্ণাঙ্ক get_kerning (hb_codepoint_t left, hb_codepoint_t ডান,
		   hb_aat_apply_context_t *c = nullptr) ধ্রুব
  {
    hb_glyph_pair_t pair = {left, ডান};
    পূর্ণাঙ্ক v = pairs.bsearch (pair).get_kerning ();
    প্রত্যাগম kerxTupleKern (v, header.tuple_গুনতি_2 (), এতত্, c);
  }

  তার্কিক apply (hb_aat_apply_context_t *c) ধ্রুব
  {
    TRACE_APPLY (এতত্);

    যদি (!c->plan->requested_kerning)
      return_trace (অসত্য);

    যদি (header.coverage & header.Backwards)
      return_trace (অসত্য);

    accelerator_t accel (*এতত্, c);
    hb_kern_machine_t<accelerator_t> machine (accel, header.coverage & header.CrossStream);
    machine.kern (c->font, c->buffer, c->plan->kern_mask);

    return_trace (সত্য);
  }

  template <typename set_t>
  শূন্য collect_glyphs (set_t &first_set, set_t &second_set, unsigned অঙ্ক_glyphs) ধ্রুব
  {
    প্রতি (ধ্রুব KernPair& pair : pairs)
    {
      first_set.add (pair.left);
      second_set.add (pair.ডান);
    }
  }

  সংরচনা accelerator_t
  {
    ধ্রুব KerxSubTableFormat0 &table;
    hb_aat_apply_context_t *c;

    accelerator_t (ধ্রুব KerxSubTableFormat0 &table_,
		   hb_aat_apply_context_t *c_) :
		     table (table_), c (c_) {}

    পূর্ণাঙ্ক get_kerning (hb_codepoint_t left, hb_codepoint_t ডান) ধ্রুব
    {
      যদি (!(*c->first_set)[left] || !(*c->second_set)[ডান]) প্রত্যাগম 0;
      প্রত্যাগম table.get_kerning (left, ডান, c);
    }
  };


  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    return_trace (likely (pairs.sanitize (c)));
  }

  রক্ষিত:
  KernSubTableHeader	header;
  Bin_অনুসন্ধান_Array_Of<KernPair, typename KernSubTableHeader::Types::HBUINT>
			pairs;	/* Sorted kern records. */
  সার্বজনিক:
  DEFINE_SIZE_ARRAY (KernSubTableHeader::static_size + 16, pairs);
};


template <তার্কিক extended>
সংরচনা Format1Entry;

template <>
সংরচনা Format1Entry<সত্য>
{
  গণনা Flags
  {
    ঠেলো		= 0x8000,	/* If set, push this glyph on the kerning stack. */
    DontAdvance		= 0x4000,	/* If set, don't advance to the next glyph
					 * before going to the new state. */
    Reset		= 0x2000,	/* If set, reset the kerning data (clear the stack) */
    Reserved		= 0x1FFF,	/* Not used; set to 0. */
  };

  সংরচনা EntryData
  {
    HBUINT16	kernActionIndex;/* Index into the kerning value array. If
				 * this index is 0xFFFF, then no kerning
				 * is to be performed. */
    সার্বজনিক:
    DEFINE_SIZE_STATIC (2);
  };

  স্থির তার্কিক initiateAction (ধ্রুব Entry<EntryData> &entry)
  { প্রত্যাগম entry.flags & ঠেলো; }

  স্থির তার্কিক performAction (ধ্রুব Entry<EntryData> &entry)
  { প্রত্যাগম entry.data.kernActionIndex != 0xFFFF; }

  স্থির unsigned পূর্ণাঙ্ক kernActionIndex (ধ্রুব Entry<EntryData> &entry)
  { প্রত্যাগম entry.data.kernActionIndex; }
};
template <>
সংরচনা Format1Entry<অসত্য>
{
  গণনা Flags
  {
    ঠেলো		= 0x8000,	/* If set, push this glyph on the kerning stack. */
    DontAdvance		= 0x4000,	/* If set, don't advance to the next glyph
					 * before going to the new state. */
    Offset		= 0x3FFF,	/* Byte offset from beginning of subtable to the
					 * value table for the glyphs on the kerning stack. */

    Reset		= 0x0000,	/* Not supported? */
  };

  প্রকারনির্দেশ শূন্য EntryData;

  স্থির তার্কিক initiateAction (ধ্রুব Entry<EntryData> &entry)
  { প্রত্যাগম entry.flags & ঠেলো; }

  স্থির তার্কিক performAction (ধ্রুব Entry<EntryData> &entry)
  { প্রত্যাগম entry.flags & Offset; }

  স্থির unsigned পূর্ণাঙ্ক kernActionIndex (ধ্রুব Entry<EntryData> &entry)
  { প্রত্যাগম entry.flags & Offset; }
};

template <typename KernSubTableHeader>
সংরচনা KerxSubTableFormat1
{
  প্রকারনির্দেশ typename KernSubTableHeader::Types Types;
  প্রকারনির্দেশ typename Types::HBUINT HBUINT;

  প্রকারনির্দেশ Format1Entry<Types::extended> Format1EntryT;
  প্রকারনির্দেশ typename Format1EntryT::EntryData EntryData;

  গণনা Flags
  {
    DontAdvance	= Format1EntryT::DontAdvance,
  };

  তার্কিক is_action_initiable (ধ্রুব Entry<EntryData> &entry) ধ্রুব
  {
    প্রত্যাগম Format1EntryT::initiateAction (entry);
  }
  তার্কিক is_actionable (ধ্রুব Entry<EntryData> &entry) ধ্রুব
  {
    প্রত্যাগম Format1EntryT::performAction (entry);
  }

  সংরচনা driver_context_t
  {
    স্থির constexpr তার্কিক in_place = সত্য;

    driver_context_t (ধ্রুব KerxSubTableFormat1 *table_,
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
    শূন্য transition (hb_buffer_t *buffer,
		     StateTableDriver<Types, EntryData, Flags> *driver,
		     ধ্রুব Entry<EntryData> &entry)
    {
      unsigned পূর্ণাঙ্ক flags = entry.flags;

      যদি (flags & Format1EntryT::Reset)
	depth = 0;

      যদি (flags & Format1EntryT::ঠেলো)
      {
	যদি (likely (depth < ARRAY_LENGTH (স্তূপ)))
	  স্তূপ[depth++] = buffer->idx;
	অন্যথা
	  depth = 0; /* Probably not what CoreText does, but better? */
      }

      যদি (Format1EntryT::performAction (entry) && depth)
      {
	unsigned পূর্ণাঙ্ক tuple_গুনতি_2 = hb_max (1u, table->header.tuple_গুনতি_2 ());

	unsigned পূর্ণাঙ্ক kern_idx = Format1EntryT::kernActionIndex (entry);
	kern_idx = Types::byteOffsetToIndex (kern_idx, &table->machine, kernAction.arrayZ);
	ধ্রুব FWORD *actions = &kernAction[kern_idx];
	যদি (!c->sanitizer.check_array (actions, depth, tuple_গুনতি_2))
	{
	  depth = 0;
	  প্রত্যাগম;
	}
	hb_barrier ();

	hb_mask_t kern_mask = c->plan->kern_mask;

	/* From Apple 'kern' spec:
	 * "Each pops one glyph from the kerning stack and applies the kerning value to it.
	 * The end of the list is marked by an odd value... */
	তার্কিক last = অসত্য;
	যাবত্ (!last && depth)
	{
	  unsigned পূর্ণাঙ্ক idx = স্তূপ[--depth];
	  পূর্ণাঙ্ক v = *actions;
	  actions += tuple_গুনতি_2;
	  যদি (idx >= buffer->len) অনুবর্ত;

	  /* "The end of the list is marked by an odd value..." */
	  last = v & 1;
	  v &= ~1;

	  hb_glyph_position_t &o = buffer->pos[idx];

	  যদি (HB_DIRECTION_IS_HORIZONTAL (buffer->props.direction))
	  {
	    যদি (crossStream)
	    {
	      /* The following flag is undocumented in the spec, but described
	       * in the 'kern' table example. */
	      যদি (v == -0x8000)
	      {
		o.attach_type() = OT::Layout::GPOS_impl::ATTACH_TYPE_NONE;
		o.attach_chain() = 0;
		o.y_offset = 0;
	      }
	      অন্যথা যদি (o.attach_type())
	      {
		o.y_offset = hb_saturate_add (o.y_offset, c->font->em_scale_y (v));
		buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT;
	      }
	    }
	    অন্যথা যদি (buffer->info[idx].mask & kern_mask)
	    {
	      auto scaled = c->font->em_scale_x (v);
	      o.x_advance = hb_saturate_add (o.x_advance, scaled);
	      o.x_offset = hb_saturate_add (o.x_offset, scaled);
	    }
	  }
	  অন্যথা
	  {
	    যদি (crossStream)
	    {
	      /* CoreText doesn't do crossStream kerning in vertical.  We do. */
	      যদি (v == -0x8000)
	      {
		o.attach_type() = OT::Layout::GPOS_impl::ATTACH_TYPE_NONE;
		o.attach_chain() = 0;
		o.x_offset = 0;
	      }
	      অন্যথা যদি (o.attach_type())
	      {
		o.x_offset = hb_saturate_add (o.x_offset, c->font->em_scale_x (v));
		buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT;
	      }
	    }
	    অন্যথা যদি (buffer->info[idx].mask & kern_mask)
	    {
	      auto scaled = c->font->em_scale_y (v);
	      o.y_advance = hb_saturate_add (o.y_advance, scaled);
	      o.y_offset = hb_saturate_add (o.y_offset, scaled);
	    }
	  }
	}
      }
    }

    সার্বজনিক:
    hb_aat_apply_context_t *c;
    ধ্রুব KerxSubTableFormat1 *table;
    গুপ্ত:
    ধ্রুব UnsizedArrayOf<FWORD> &kernAction;
    unsigned পূর্ণাঙ্ক স্তূপ[8];
    unsigned পূর্ণাঙ্ক depth;
    তার্কিক crossStream;
  };

  তার্কিক apply (hb_aat_apply_context_t *c) ধ্রুব
  {
    TRACE_APPLY (এতত্);

    যদি (!c->plan->requested_kerning &&
	!(header.coverage & header.CrossStream))
      প্রত্যাগম অসত্য;

    driver_context_t dc (এতত্, c);

    StateTableDriver<Types, EntryData, Flags> driver (machine, c->font->face);

    driver.drive (&dc, c);

    return_trace (সত্য);
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    /* The rest of array sanitizations are done at run-time. */
    return_trace (likely (c->check_struct (এতত্) &&
			  machine.sanitize (c)));
  }

  template <typename set_t>
  শূন্য collect_glyphs (set_t &first_set, set_t &second_set, unsigned অঙ্ক_glyphs) ধ্রুব
  {
    machine.collect_initial_glyphs (first_set, অঙ্ক_glyphs, *এতত্);
    //machine.collect_glyphs (second_set, num_glyphs); // second_set is unused for machine kerning
  }

  রক্ষিত:
  KernSubTableHeader				header;
  StateTable<Types, EntryData>			machine;
  NNOffsetTo<UnsizedArrayOf<FWORD>, HBUINT>	kernAction;
  সার্বজনিক:
  DEFINE_SIZE_STATIC (KernSubTableHeader::static_size + (StateTable<Types, EntryData>::static_size + HBUINT::static_size));
};

template <typename KernSubTableHeader>
সংরচনা KerxSubTableFormat2
{
  প্রকারনির্দেশ typename KernSubTableHeader::Types Types;
  প্রকারনির্দেশ typename Types::HBUINT HBUINT;

  পূর্ণাঙ্ক get_kerning (hb_codepoint_t left, hb_codepoint_t ডান,
		   hb_aat_apply_context_t *c) ধ্রুব
  {
    unsigned পূর্ণাঙ্ক অঙ্ক_glyphs = c->sanitizer.get_অঙ্ক_glyphs ();
    unsigned পূর্ণাঙ্ক l = (এতত্+বাম_Class_Table).get_class (left, অঙ্ক_glyphs, 0);
    unsigned পূর্ণাঙ্ক r = (এতত্+ডান_Class_Table).get_class (ডান, অঙ্ক_glyphs, 0);

    ধ্রুব UnsizedArrayOf<FWORD> &arrayZ = এতত্+array;
    unsigned পূর্ণাঙ্ক kern_idx = l + r;
    kern_idx = Types::offsetToIndex (kern_idx, এতত্, arrayZ.arrayZ);
    ধ্রুব FWORD *v = &arrayZ[kern_idx];
    যদি (unlikely (!v->sanitize (&c->sanitizer))) প্রত্যাগম 0;
    hb_barrier ();

    প্রত্যাগম kerxTupleKern (*v, header.tuple_গুনতি_2 (), এতত্, c);
  }

  তার্কিক apply (hb_aat_apply_context_t *c) ধ্রুব
  {
    TRACE_APPLY (এতত্);

    যদি (!c->plan->requested_kerning)
      return_trace (অসত্য);

    যদি (header.coverage & header.Backwards)
      return_trace (অসত্য);

    accelerator_t accel (*এতত্, c);
    hb_kern_machine_t<accelerator_t> machine (accel, header.coverage & header.CrossStream);
    machine.kern (c->font, c->buffer, c->plan->kern_mask);

    return_trace (সত্য);
  }

  template <typename set_t>
  শূন্য collect_glyphs (set_t &first_set, set_t &second_set, unsigned অঙ্ক_glyphs) ধ্রুব
  {
    (এতত্+বাম_Class_Table).collect_glyphs (first_set, অঙ্ক_glyphs);
    (এতত্+ডান_Class_Table).collect_glyphs (second_set, অঙ্ক_glyphs);
  }

  সংরচনা accelerator_t
  {
    ধ্রুব KerxSubTableFormat2 &table;
    hb_aat_apply_context_t *c;

    accelerator_t (ধ্রুব KerxSubTableFormat2 &table_,
		   hb_aat_apply_context_t *c_) :
		     table (table_), c (c_) {}

    পূর্ণাঙ্ক get_kerning (hb_codepoint_t left, hb_codepoint_t ডান) ধ্রুব
    {
      যদি (!(*c->first_set)[left] || !(*c->second_set)[ডান]) প্রত্যাগম 0;
      প্রত্যাগম table.get_kerning (left, ডান, c);
    }
  };

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    return_trace (likely (c->check_struct (এতত্) &&
			  বাম_Class_Table.sanitize (c, এতত্) &&
			  ডান_Class_Table.sanitize (c, এতত্) &&
			  hb_barrier () &&
			  c->check_range (এতত্, array)));
  }

  রক্ষিত:
  KernSubTableHeader	header;
  HBUINT		rowWidth;	/* The width, in bytes, of a row in the table. */
  NNOffsetTo<typename Types::ClassTypeWide, HBUINT>
			বাম_Class_Table;	/* Offset from beginning of this subtable to
					 * left-hand class table. */
  NNOffsetTo<typename Types::ClassTypeWide, HBUINT>
			ডান_Class_Table;/* Offset from beginning of this subtable to
					 * right-hand class table. */
  NNOffsetTo<UnsizedArrayOf<FWORD>, HBUINT>
			 array;		/* Offset from beginning of this subtable to
					 * the start of the kerning array. */
  সার্বজনিক:
  DEFINE_SIZE_STATIC (KernSubTableHeader::static_size + 4 * পরিমাণ (HBUINT));
};

template <typename KernSubTableHeader>
সংরচনা KerxSubTableFormat4
{
  প্রকারনির্দেশ ExtendedTypes Types;

  সংরচনা EntryData
  {
    HBUINT16	ankrActionIndex;/* Either 0xFFFF (for no action) or the index of
				 * the action to perform. */
    সার্বজনিক:
    DEFINE_SIZE_STATIC (2);
  };

  গণনা Flags
  {
    Mark		= 0x8000,	/* If set, remember this glyph as the marked glyph. */
    DontAdvance		= 0x4000,	/* If set, don't advance to the next glyph before
					 * going to the new state. */
    Reserved		= 0x3FFF,	/* Not used; set to 0. */
  };

  তার্কিক is_action_initiable (ধ্রুব Entry<EntryData> &entry) ধ্রুব
  {
    প্রত্যাগম (entry.flags & Mark);
  }
  তার্কিক is_actionable (ধ্রুব Entry<EntryData> &entry) ধ্রুব
  {
    প্রত্যাগম entry.data.ankrActionIndex != 0xFFFF;
  }

  সংরচনা driver_context_t
  {
    স্থির constexpr তার্কিক in_place = সত্য;
    গণনা SubTableFlags
    {
      ActionType	= 0xC0000000,	/* A two-bit field containing the action type. */
      Unused		= 0x3F000000,	/* Unused - must be zero. */
      Offset		= 0x00FFFFFF,	/* Masks the offset in bytes from the beginning
					 * of the subtable to the beginning of the control
					 * point table. */
    };

    driver_context_t (ধ্রুব KerxSubTableFormat4 *table_,
		      hb_aat_apply_context_t *c_) :
	c (c_),
	table (table_),
	action_type ((table->flags & ActionType) >> 30),
	ankrData ((HBUINT16 *) ((ধ্রুব বর্ণ *) &table->machine + (table->flags & Offset))),
	mark_set (অসত্য),
	mark (0) {}

    HB_AAT_TRANSITION_INLINE
    শূন্য transition (hb_buffer_t *buffer,
		     StateTableDriver<Types, EntryData, Flags> *driver,
		     ধ্রুব Entry<EntryData> &entry)
    {
      যদি (mark_set && entry.data.ankrActionIndex != 0xFFFF && buffer->idx < buffer->len)
      {
	hb_glyph_position_t &o = buffer->cur_pos();
	বিকল্প (action_type)
	{
	  পক্ষ 0: /* Control Point Actions.*/
	  {
	    /* Indexed into glyph outline. */
	    /* Each action (record in ankrData) contains two 16-bit fields, so we must
	       double the ankrActionIndex to get the correct offset here. */
	    ধ্রুব HBUINT16 *data = &ankrData[entry.data.ankrActionIndex * 2];
	    যদি (!c->sanitizer.check_array (data, 2)) প্রত্যাগম;
	    hb_barrier ();
	    unsigned পূর্ণাঙ্ক markControlPoint = *data++;
	    unsigned পূর্ণাঙ্ক currControlPoint = *data++;
	    hb_position_t markX = 0;
	    hb_position_t markY = 0;
	    hb_position_t currX = 0;
	    hb_position_t currY = 0;
	    যদি (!c->font->get_glyph_contour_point_for_origin (c->buffer->info[mark].codepoint,
							      markControlPoint,
							      HB_DIRECTION_LTR /*XXX*/,
							      &markX, &markY) ||
		!c->font->get_glyph_contour_point_for_origin (c->buffer->cur ().codepoint,
							      currControlPoint,
							      HB_DIRECTION_LTR /*XXX*/,
							      &currX, &currY))
	      প্রত্যাগম;

	    o.x_offset = hb_saturate_sub (markX, currX);
	    o.y_offset = hb_saturate_sub (markY, currY);
	  }
	  ভঙ্গ;

	  পক্ষ 1: /* Anchor Point Actions. */
	  {
	    /* Indexed into 'ankr' table. */
	    /* Each action (record in ankrData) contains two 16-bit fields, so we must
	       double the ankrActionIndex to get the correct offset here. */
	    ধ্রুব HBUINT16 *data = &ankrData[entry.data.ankrActionIndex * 2];
	    যদি (!c->sanitizer.check_array (data, 2)) প্রত্যাগম;
	    hb_barrier ();
	    unsigned পূর্ণাঙ্ক markAnchorPoint = *data++;
	    unsigned পূর্ণাঙ্ক currAnchorPoint = *data++;
	    ধ্রুব Anchor &markAnchor = c->ankr_table->get_anchor (c->buffer->info[mark].codepoint,
								  markAnchorPoint,
								  c->sanitizer.get_অঙ্ক_glyphs ());
	    ধ্রুব Anchor &currAnchor = c->ankr_table->get_anchor (c->buffer->cur ().codepoint,
								  currAnchorPoint,
								  c->sanitizer.get_অঙ্ক_glyphs ());

	    o.x_offset = hb_saturate_sub (c->font->em_scale_x (markAnchor.xCoordinate),
					  c->font->em_scale_x (currAnchor.xCoordinate));
	    o.y_offset = hb_saturate_sub (c->font->em_scale_y (markAnchor.yCoordinate),
					  c->font->em_scale_y (currAnchor.yCoordinate));
	  }
	  ভঙ্গ;

	  পক্ষ 2: /* Control Point Coordinate Actions. */
	  {
	    /* Each action contains four 16-bit fields, so we multiply the ankrActionIndex
	       by 4 to get the correct offset for the given action. */
	    ধ্রুব FWORD *data = (ধ্রুব FWORD *) &ankrData[entry.data.ankrActionIndex * 4];
	    যদি (!c->sanitizer.check_array (data, 4)) প্রত্যাগম;
	    hb_barrier ();
	    পূর্ণাঙ্ক markX = *data++;
	    পূর্ণাঙ্ক markY = *data++;
	    পূর্ণাঙ্ক currX = *data++;
	    পূর্ণাঙ্ক currY = *data++;

	    o.x_offset = hb_saturate_sub (c->font->em_scale_x (markX), c->font->em_scale_x (currX));
	    o.y_offset = hb_saturate_sub (c->font->em_scale_y (markY), c->font->em_scale_y (currY));
	  }
	  ভঙ্গ;
	}
	o.attach_type() = OT::Layout::GPOS_impl::ATTACH_TYPE_MARK;
	o.attach_chain() = (পূর্ণাঙ্ক) mark - (পূর্ণাঙ্ক) buffer->idx;
	যদি (c->buffer_is_reversed)
	  o.attach_chain() = -o.attach_chain();
	buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT;
      }

      যদি (entry.flags & Mark)
      {
	mark_set = সত্য;
	mark = buffer->idx;
      }
    }

    সার্বজনিক:
    hb_aat_apply_context_t *c;
    ধ্রুব KerxSubTableFormat4 *table;
    গুপ্ত:
    unsigned পূর্ণাঙ্ক action_type;
    ধ্রুব HBUINT16 *ankrData;
    তার্কিক mark_set;
    unsigned পূর্ণাঙ্ক mark;
  };

  তার্কিক apply (hb_aat_apply_context_t *c) ধ্রুব
  {
    TRACE_APPLY (এতত্);

    driver_context_t dc (এতত্, c);

    StateTableDriver<Types, EntryData, Flags> driver (machine, c->font->face);

    driver.drive (&dc, c);

    return_trace (সত্য);
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    /* The rest of array sanitizations are done at run-time. */
    return_trace (likely (c->check_struct (এতত্) &&
			  machine.sanitize (c)));
  }

  template <typename set_t>
  শূন্য collect_glyphs (set_t &first_set, set_t &second_set, unsigned অঙ্ক_glyphs) ধ্রুব
  {
    machine.collect_initial_glyphs (first_set, অঙ্ক_glyphs, *এতত্);
    //machine.collect_glyphs (second_set, num_glyphs); // second_set is unused for machine kerning
  }

  রক্ষিত:
  KernSubTableHeader		header;
  StateTable<Types, EntryData>	machine;
  HBUINT32			flags;
  সার্বজনিক:
  DEFINE_SIZE_STATIC (KernSubTableHeader::static_size + (StateTable<Types, EntryData>::static_size + HBUINT32::static_size));
};

template <typename KernSubTableHeader>
সংরচনা KerxSubTableFormat6
{
  গণনা Flags
  {
    মানসমূহ_Are_Long	= 0x00000001,
  };

  তার্কিক is_long () ধ্রুব { প্রত্যাগম flags & মানসমূহ_Are_Long; }

  পূর্ণাঙ্ক get_kerning (hb_codepoint_t left, hb_codepoint_t ডান,
		   hb_aat_apply_context_t *c) ধ্রুব
  {
    unsigned পূর্ণাঙ্ক অঙ্ক_glyphs = c->sanitizer.get_অঙ্ক_glyphs ();
    যদি (is_long ())
    {
      ধ্রুব auto &t = u.l;
      unsigned পূর্ণাঙ্ক l = (এতত্+t.rowIndexTable).get_মান_or_null (left, অঙ্ক_glyphs);
      unsigned পূর্ণাঙ্ক r = (এতত্+t.columnIndexTable).get_মান_or_null (ডান, অঙ্ক_glyphs);
      unsigned পূর্ণাঙ্ক offset = l + r;
      যদি (unlikely (offset < l)) প্রত্যাগম 0; /* Addition overflow. */
      যদি (unlikely (hb_unsigned_mul_overflows (offset, পরিমাণ (FWORD32)))) প্রত্যাগম 0;
      ধ্রুব FWORD32 *v = &StructAtOffset<FWORD32> (&(এতত্+t.array), offset * পরিমাণ (FWORD32));
      যদি (unlikely (!v->sanitize (&c->sanitizer))) প্রত্যাগম 0;
      hb_barrier ();
      প্রত্যাগম kerxTupleKern (*v, header.tuple_গুনতি_2 (), &(এতত্+vector), c);
    }
    অন্যথা
    {
      ধ্রুব auto &t = u.s;
      unsigned পূর্ণাঙ্ক l = (এতত্+t.rowIndexTable).get_মান_or_null (left, অঙ্ক_glyphs);
      unsigned পূর্ণাঙ্ক r = (এতত্+t.columnIndexTable).get_মান_or_null (ডান, অঙ্ক_glyphs);
      unsigned পূর্ণাঙ্ক offset = l + r;
      ধ্রুব FWORD *v = &StructAtOffset<FWORD> (&(এতত্+t.array), offset * পরিমাণ (FWORD));
      যদি (unlikely (!v->sanitize (&c->sanitizer))) প্রত্যাগম 0;
      hb_barrier ();
      প্রত্যাগম kerxTupleKern (*v, header.tuple_গুনতি_2 (), &(এতত্+vector), c);
    }
  }

  তার্কিক apply (hb_aat_apply_context_t *c) ধ্রুব
  {
    TRACE_APPLY (এতত্);

    যদি (!c->plan->requested_kerning)
      return_trace (অসত্য);

    যদি (header.coverage & header.Backwards)
      return_trace (অসত্য);

    accelerator_t accel (*এতত্, c);
    hb_kern_machine_t<accelerator_t> machine (accel, header.coverage & header.CrossStream);
    machine.kern (c->font, c->buffer, c->plan->kern_mask);

    return_trace (সত্য);
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    return_trace (likely (c->check_struct (এতত্) &&
			  hb_barrier () &&
			  (is_long () ?
			   (
			     u.l.rowIndexTable.sanitize (c, এতত্) &&
			     u.l.columnIndexTable.sanitize (c, এতত্) &&
			     c->check_range (এতত্, u.l.array)
			   ) : (
			     u.s.rowIndexTable.sanitize (c, এতত্) &&
			     u.s.columnIndexTable.sanitize (c, এতত্) &&
			     c->check_range (এতত্, u.s.array)
			   )) &&
			  (header.tuple_গুনতি_2 () == 0 ||
			   c->check_range (এতত্, vector))));
  }

  template <typename set_t>
  শূন্য collect_glyphs (set_t &first_set, set_t &second_set, unsigned অঙ্ক_glyphs) ধ্রুব
  {
    যদি (is_long ())
    {
      ধ্রুব auto &t = u.l;
      (এতত্+t.rowIndexTable).collect_glyphs (first_set, অঙ্ক_glyphs);
      (এতত্+t.columnIndexTable).collect_glyphs (second_set, অঙ্ক_glyphs);
    }
    অন্যথা
    {
      ধ্রুব auto &t = u.s;
      (এতত্+t.rowIndexTable).collect_glyphs (first_set, অঙ্ক_glyphs);
      (এতত্+t.columnIndexTable).collect_glyphs (second_set, অঙ্ক_glyphs);
    }
  }

  সংরচনা accelerator_t
  {
    ধ্রুব KerxSubTableFormat6 &table;
    hb_aat_apply_context_t *c;

    accelerator_t (ধ্রুব KerxSubTableFormat6 &table_,
		   hb_aat_apply_context_t *c_) :
		     table (table_), c (c_) {}

    পূর্ণাঙ্ক get_kerning (hb_codepoint_t left, hb_codepoint_t ডান) ধ্রুব
    {
      যদি (!(*c->first_set)[left] || !(*c->second_set)[ডান]) প্রত্যাগম 0;
      প্রত্যাগম table.get_kerning (left, ডান, c);
    }
  };

  রক্ষিত:
  KernSubTableHeader		header;
  HBUINT32			flags;
  HBUINT16			row_গুনতি;
  HBUINT16			column_গুনতি;
  সঙ্ঘ U
  {
    সংরচনা Long
    {
      NNOffset32To<Lookup<HBUINT32>>		rowIndexTable;
      NNOffset32To<Lookup<HBUINT32>>		columnIndexTable;
      NNOffset32To<UnsizedArrayOf<FWORD32>>	array;
    } l;
    সংরচনা Short
    {
      NNOffset32To<Lookup<HBUINT16>>		rowIndexTable;
      NNOffset32To<Lookup<HBUINT16>>		columnIndexTable;
      NNOffset32To<UnsizedArrayOf<FWORD>>	array;
    } s;
  } u;
  NNOffset32To<UnsizedArrayOf<FWORD>>	vector;
  সার্বজনিক:
  DEFINE_SIZE_STATIC (KernSubTableHeader::static_size + 24);
};


সংরচনা KerxSubTableHeader
{
  প্রকারনির্দেশ ExtendedTypes Types;

  unsigned   tuple_গুনতি_2 () ধ্রুব { প্রত্যাগম tuple_গুনতি; }
  তার্কিক     is_horizontal () ধ্রুব { প্রত্যাগম !(coverage & Vertical); }

  গণনা Coverage
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

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্));
  }

  সার্বজনিক:
  HBUINT32	length;
  HBUINT32	coverage;
  HBUINT32	tuple_গুনতি;
  সার্বজনিক:
  DEFINE_SIZE_STATIC (12);
};

সংরচনা KerxSubTable
{
  friend সংরচনা kerx;

  size_t get_size () ধ্রুব { প্রত্যাগম u.header.length; }
  unsigned পূর্ণাঙ্ক get_type () ধ্রুব { প্রত্যাগম u.header.coverage & u.header.SubtableType; }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ধ্রুব
  {
    unsigned পূর্ণাঙ্ক subtable_type = get_type ();
    TRACE_DISPATCH (এতত্, subtable_type);
    বিকল্প (subtable_type) {
    পক্ষ 0:	hb_barrier (); return_trace (c->dispatch (u.format0, std::forward<Ts> (ds)...));
    পক্ষ 1:	hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    পক্ষ 2:	hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    পক্ষ 4:	hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
    পক্ষ 6:	hb_barrier (); return_trace (c->dispatch (u.format6, std::forward<Ts> (ds)...));
    স্বভাব:	return_trace (c->default_return_মান ());
    }
  }

  template <typename set_t>
  শূন্য collect_glyphs (set_t &first_set, set_t &second_set, unsigned অঙ্ক_glyphs) ধ্রুব
  {
    unsigned পূর্ণাঙ্ক subtable_type = get_type ();
    বিকল্প (subtable_type) {
    পক্ষ 0:	hb_barrier (); u.format0.collect_glyphs (first_set, second_set, অঙ্ক_glyphs); প্রত্যাগম;
    পক্ষ 1:	hb_barrier (); u.format1.collect_glyphs (first_set, second_set, অঙ্ক_glyphs); প্রত্যাগম;
    পক্ষ 2:	hb_barrier (); u.format2.collect_glyphs (first_set, second_set, অঙ্ক_glyphs); প্রত্যাগম;
    পক্ষ 4:	hb_barrier (); u.format4.collect_glyphs (first_set, second_set, অঙ্ক_glyphs); প্রত্যাগম;
    পক্ষ 6:	hb_barrier (); u.format6.collect_glyphs (first_set, second_set, অঙ্ক_glyphs); প্রত্যাগম;
    স্বভাব:	প্রত্যাগম;
    }
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    যদি (!(u.header.sanitize (c) &&
	  hb_barrier () &&
	  u.header.length >= u.header.static_size &&
	  c->check_range (এতত্, u.header.length)))
      return_trace (অসত্য);

    return_trace (dispatch (c));
  }

  সার্বজনিক:
  সঙ্ঘ {
  KerxSubTableHeader				header;
  KerxSubTableFormat0<KerxSubTableHeader>	format0;
  KerxSubTableFormat1<KerxSubTableHeader>	format1;
  KerxSubTableFormat2<KerxSubTableHeader>	format2;
  KerxSubTableFormat4<KerxSubTableHeader>	format4;
  KerxSubTableFormat6<KerxSubTableHeader>	format6;
  } u;
  সার্বজনিক:
  DEFINE_SIZE_MIN (12);
};


/*
 * The 'kerx' Table
 */

সংরচনা kern_subtable_accelerator_data_t
{
  hb_bit_set_t first_set;
  hb_bit_set_t second_set;
  mutable hb_aat_class_cache_t class_cache;
};

সংরচনা kern_accelerator_data_t
{
  hb_vector_t<kern_subtable_accelerator_data_t> subtable_accels;
  hb_aat_scratch_t scratch;
};

template <typename T>
সংরচনা KerxTable
{
  /* https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern */
  ধ্রুব T* thiz () ধ্রুব { প্রত্যাগম static_cast<ধ্রুব T *> (এতত্); }

  তার্কিক has_state_machine () ধ্রুব
  {
    প্রকারনির্দেশ typename T::SubTable SubTable;

    ধ্রুব SubTable *st = &thiz()->firstSubTable;
    unsigned পূর্ণাঙ্ক গুনতি = thiz()->table_গুনতি;
    প্রতি (unsigned পূর্ণাঙ্ক i = 0; i < গুনতি; i++)
    {
      যদি (st->get_type () == 1)
	প্রত্যাগম সত্য;

      // TODO: What about format 4? What's this API used for anyway?

      st = &StructAfter<SubTable> (*st);
    }
    প্রত্যাগম অসত্য;
  }

  তার্কিক has_cross_stream () ধ্রুব
  {
    প্রকারনির্দেশ typename T::SubTable SubTable;

    ধ্রুব SubTable *st = &thiz()->firstSubTable;
    unsigned পূর্ণাঙ্ক গুনতি = thiz()->table_গুনতি;
    প্রতি (unsigned পূর্ণাঙ্ক i = 0; i < গুনতি; i++)
    {
      যদি (st->u.header.coverage & st->u.header.CrossStream)
	প্রত্যাগম সত্য;
      st = &StructAfter<SubTable> (*st);
    }
    প্রত্যাগম অসত্য;
  }

  পূর্ণাঙ্ক get_h_kerning (hb_codepoint_t left, hb_codepoint_t ডান) ধ্রুব
  {
    প্রকারনির্দেশ typename T::SubTable SubTable;

    পূর্ণাঙ্ক v = 0;
    ধ্রুব SubTable *st = &thiz()->firstSubTable;
    unsigned পূর্ণাঙ্ক গুনতি = thiz()->table_গুনতি;
    প্রতি (unsigned পূর্ণাঙ্ক i = 0; i < গুনতি; i++)
    {
      যদি ((st->u.header.coverage & (st->u.header.Variation | st->u.header.CrossStream)) ||
	  !st->u.header.is_horizontal ())
	অনুবর্ত;
      v += st->get_kerning (left, ডান);
      st = &StructAfter<SubTable> (*st);
    }
    প্রত্যাগম v;
  }

  তার্কিক apply (AAT::hb_aat_apply_context_t *c,
	      ধ্রুব kern_accelerator_data_t &accel_data) ধ্রুব
  {
    c->buffer->unsafe_to_concat ();

    c->setup_buffer_glyph_set ();

    প্রকারনির্দেশ typename T::SubTable SubTable;

    তার্কিক ret = অসত্য;
    তার্কিক seenCrossStream = অসত্য;
    c->set_lookup_index (0);
    ধ্রুব SubTable *st = &thiz()->firstSubTable;
    unsigned পূর্ণাঙ্ক গুনতি = thiz()->table_গুনতি;
    প্রতি (unsigned পূর্ণাঙ্ক i = 0; i < গুনতি; i++)
    {
      তার্কিক reverse;

      auto &subtable_accel = accel_data.subtable_accels[i];

      যদি (!T::Types::extended && (st->u.header.coverage & st->u.header.Variation))
	গচ্ছ skip;

      যদি (HB_DIRECTION_IS_HORIZONTAL (c->buffer->props.direction) != st->u.header.is_horizontal ())
	গচ্ছ skip;

      c->first_set = &subtable_accel.first_set;
      c->second_set = &subtable_accel.second_set;
      c->machine_class_cache = &subtable_accel.class_cache;

      যদি (!c->buffer_intersects_machine ())
      {
	(শূন্য) c->buffer->message (c->font, "skipped subtable %u because no glyph matches", c->lookup_index);
	গচ্ছ skip;
      }

      reverse = তার্কিক (st->u.header.coverage & st->u.header.Backwards) !=
		HB_DIRECTION_IS_BACKWARD (c->buffer->props.direction);

      যদি (!c->buffer->message (c->font, "start subtable %u", c->lookup_index))
	গচ্ছ skip;

      যদি (!seenCrossStream &&
	  (st->u.header.coverage & st->u.header.CrossStream))
      {
	/* Attach all glyphs into a chain. */
	seenCrossStream = সত্য;
	hb_glyph_position_t *pos = c->buffer->pos;
	unsigned পূর্ণাঙ্ক গুনতি = c->buffer->len;
	প্রতি (unsigned পূর্ণাঙ্ক i = 0; i < গুনতি; i++)
	{
	  pos[i].attach_type() = OT::Layout::GPOS_impl::ATTACH_TYPE_CURSIVE;
	  pos[i].attach_chain() = HB_DIRECTION_IS_FORWARD (c->buffer->props.direction) ? -1 : +1;
	  /* We intentionally don't set HB_BUFFER_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT,
	   * since there needs to be a non-zero attachment for post-positioning to
	   * be needed. */
	}
      }

      যদি (reverse != c->buffer_is_reversed)
        c->reverse_buffer ();

      {
	/* See comment in sanitize() for conditional here. */
	hb_sanitize_with_object_t with (&c->sanitizer, i < গুনতি - 1 ? st : (ধ্রুব SubTable *) nullptr);
	ret |= st->dispatch (c);
      }

      (শূন্য) c->buffer->message (c->font, "end subtable %u", c->lookup_index);

    skip:
      st = &StructAfter<SubTable> (*st);
      c->set_lookup_index (c->lookup_index + 1);
    }
    যদি (c->buffer_is_reversed)
      c->reverse_buffer ();

    প্রত্যাগম ret;
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    যদি (unlikely (!(thiz()->version.sanitize (c) &&
		    hb_barrier () &&
		    (unsigned) thiz()->version >= (unsigned) T::minVersion &&
		    thiz()->table_গুনতি.sanitize (c))))
      return_trace (অসত্য);

    প্রকারনির্দেশ typename T::SubTable SubTable;

    ধ্রুব SubTable *st = &thiz()->firstSubTable;
    unsigned পূর্ণাঙ্ক গুনতি = thiz()->table_গুনতি;
    প্রতি (unsigned পূর্ণাঙ্ক i = 0; i < গুনতি; i++)
    {
      যদি (unlikely (!st->u.header.sanitize (c)))
	return_trace (অসত্য);
      hb_barrier ();
      /* OpenType kern table has 2-byte subtable lengths.  That's limiting.
       * MS implementation also only supports one subtable, of format 0,
       * anyway.  Certain versions of some fonts, like Calibry, contain
       * kern subtable that exceeds 64kb.  Looks like, the subtable length
       * is simply ignored.  Which makes sense.  It's only needed if you
       * have multiple subtables.  To handle such fonts, we just ignore
       * the length for the last subtable. */
      hb_sanitize_with_object_t with (c, i < গুনতি - 1 ? st : (ধ্রুব SubTable *) nullptr);

      যদি (unlikely (!st->sanitize (c)))
	return_trace (অসত্য);

      st = &StructAfter<SubTable> (*st);
    }

    unsigned majorVersion = thiz()->version;
    যদি (পরিমাণ (thiz()->version) == 4)
      majorVersion = majorVersion >> 16;
    যদি (majorVersion >= 3)
    {
      ধ্রুব SubtableGlyphCoverage *coverage = (ধ্রুব SubtableGlyphCoverage *) st;
      যদি (!coverage->sanitize (c, গুনতি))
        return_trace (অসত্য);
    }

    return_trace (সত্য);
  }

  kern_accelerator_data_t create_accelerator_data (unsigned অঙ্ক_glyphs) ধ্রুব
  {
    kern_accelerator_data_t accel_data;

    প্রকারনির্দেশ typename T::SubTable SubTable;

    ধ্রুব SubTable *st = &thiz()->firstSubTable;
    unsigned পূর্ণাঙ্ক গুনতি = thiz()->table_গুনতি;
    প্রতি (unsigned পূর্ণাঙ্ক i = 0; i < গুনতি; i++)
    {
      auto &subtable_accel = *accel_data.subtable_accels.push ();
      যদি (unlikely (accel_data.subtable_accels.in_error ()))
	  প্রত্যাগম accel_data;

      st->collect_glyphs (subtable_accel.first_set, subtable_accel.second_set, অঙ্ক_glyphs);
      subtable_accel.class_cache.clear ();

      st = &StructAfter<SubTable> (*st);
    }

    প্রত্যাগম accel_data;
  }

  সংরচনা accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      hb_sanitize_context_t sc;
      এতত্->table = sc.reference_table<T> (face);
      এতত্->accel_data = এতত্->table->create_accelerator_data (face->get_অঙ্ক_glyphs ());
    }
    ~accelerator_t ()
    {
      এতত্->table.destroy ();
    }

    hb_blob_t *get_blob () ধ্রুব { প্রত্যাগম table.get_blob (); }

    তার্কিক apply (AAT::hb_aat_apply_context_t *c) ধ্রুব
    {
      প্রত্যাগম table->apply (c, accel_data);
    }

    hb_blob_ptr_t<T> table;
    kern_accelerator_data_t accel_data;
    hb_aat_scratch_t scratch;
  };
};

সংরচনা kerx : KerxTable<kerx>
{
  friend সংরচনা KerxTable<kerx>;

  স্থির constexpr hb_tag_t tableTag = HB_AAT_TAG_kerx;
  স্থির constexpr unsigned minVersion = 2u;

  প্রকারনির্দেশ KerxSubTableHeader SubTableHeader;
  প্রকারনির্দেশ SubTableHeader::Types Types;
  প্রকারনির্দেশ KerxSubTable SubTable;

  তার্কিক has_data () ধ্রুব { প্রত্যাগম version; }

  রক্ষিত:
  HBUINT16	version;	/* The version number of the extended kerning table
				 * (currently 2, 3, or 4). */
  HBUINT16	unused;		/* Set to 0. */
  HBUINT32	table_গুনতি;	/* The number of subtables included in the extended kerning
				 * table. */
  SubTable	firstSubTable;	/* Subtables. */
/*subtableGlyphCoverageArray*/	/* Only if version >= 3. We don't use. */

  সার্বজনিক:
  DEFINE_SIZE_MIN (8);
};

সংরচনা kerx_accelerator_t : kerx::accelerator_t {
  kerx_accelerator_t (hb_face_t *face) : kerx::accelerator_t (face) {}
};

} /* namespace AAT */

#endif /* HB_AAT_LAYOUT_KERX_TABLE_HH */
