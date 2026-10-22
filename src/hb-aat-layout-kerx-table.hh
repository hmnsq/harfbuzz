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


স্থির অন্তঃরেখ পূর্ণসংখ্যা
kerxTupleKern (পূর্ণসংখ্যা value,
	       unsigned পূর্ণসংখ্যা tuple_গুনতি,
	       ধ্রুবক শূন্য *base,
	       hb_aat_apply_context_t *c)
{
  যদি (likely (!tuple_গুনতি || !c)) ফেরত value;

  unsigned পূর্ণসংখ্যা offset = value;
  ধ্রুবক FWORD *pv = &StructAtOffset<FWORD> (base, offset);
  যদি (unlikely (!c->sanitizer.check_array (pv, tuple_গুনতি))) ফেরত 0;
  hb_barrier ();
  ফেরত *pv;
}


কাঠামো hb_glyph_pair_t
{
  hb_codepoint_t left;
  hb_codepoint_t ডান;
};

কাঠামো KernPair
{
  পূর্ণসংখ্যা get_kerning () ধ্রুবক { ফেরত value; }

  পূর্ণসংখ্যা cmp (ধ্রুবক hb_glyph_pair_t &o) ধ্রুবক
  {
    পূর্ণসংখ্যা ret = left.cmp (o.left);
    যদি (ret) ফেরত ret;
    ফেরত ডান.cmp (o.ডান);
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই));
  }

  সর্বজনীন:
  HBGlyphID16	left;
  HBGlyphID16	ডান;
  FWORD		value;
  সর্বজনীন:
  DEFINE_আকার_STATIC (6);
};

template <typename KernSubTableHeader>
কাঠামো KerxSubTableFormat0
{
  পূর্ণসংখ্যা get_kerning (hb_codepoint_t left, hb_codepoint_t ডান,
		   hb_aat_apply_context_t *c = nullptr) ধ্রুবক
  {
    hb_glyph_pair_t pair = {left, ডান};
    পূর্ণসংখ্যা v = pairs.bsearch (pair).get_kerning ();
    ফেরত kerxTupleKern (v, header.tuple_গুনতি_2 (), এই, c);
  }

  যৌক্তিক apply (hb_aat_apply_context_t *c) ধ্রুবক
  {
    TRACE_APPLY (এই);

    যদি (!c->plan->requested_kerning)
      return_trace (মিথ্যা);

    যদি (header.coverage & header.Backwards)
      return_trace (মিথ্যা);

    accelerator_t accel (*এই, c);
    hb_kern_machine_t<accelerator_t> machine (accel, header.coverage & header.CrossStream);
    machine.kern (c->font, c->বাফার, c->plan->kern_mask);

    return_trace (সত্য);
  }

  template <typename set_t>
  শূন্য collect_glyphs (set_t &first_set, set_t &second_set, unsigned অঙ্ক_glyphs) ধ্রুবক
  {
    জন্য (ধ্রুবক KernPair& pair : pairs)
    {
      first_set.add (pair.left);
      second_set.add (pair.ডান);
    }
  }

  কাঠামো accelerator_t
  {
    ধ্রুবক KerxSubTableFormat0 &table;
    hb_aat_apply_context_t *c;

    accelerator_t (ধ্রুবক KerxSubTableFormat0 &table_,
		   hb_aat_apply_context_t *c_) :
		     table (table_), c (c_) {}

    পূর্ণসংখ্যা get_kerning (hb_codepoint_t left, hb_codepoint_t ডান) ধ্রুবক
    {
      যদি (!(*c->first_set)[left] || !(*c->second_set)[ডান]) ফেরত 0;
      ফেরত table.get_kerning (left, ডান, c);
    }
  };


  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (likely (pairs.sanitize (c)));
  }

  সুরক্ষিত:
  KernSubTableHeader	header;
  Bin_অনুসন্ধান_Array_Of<KernPair, typename KernSubTableHeader::Types::HBUINT>
			pairs;	/* Sorted kern records. */
  সর্বজনীন:
  DEFINE_আকার_ARRAY (KernSubTableHeader::static_আকার + 16, pairs);
};


template <যৌক্তিক extended>
কাঠামো Format1Entry;

template <>
কাঠামো Format1Entry<সত্য>
{
  গণনা Flags
  {
    ঠেলো		= 0x8000,	/* If set, push this glyph on the kerning stack. */
    DontAdvance		= 0x4000,	/* If set, don't advance to the next glyph
					 * before going to the new state. */
    Reset		= 0x2000,	/* If set, reset the kerning data (clear the stack) */
    Reserved		= 0x1FFF,	/* Not used; set to 0. */
  };

  কাঠামো Entry_তথ্য
  {
    HBUINT16	kern_Action_সূচক;/* Index into the kerning value array. If
				 * this index is 0xFFFF, then no kerning
				 * is to be performed. */
    সর্বজনীন:
    DEFINE_আকার_STATIC (2);
  };

  স্থির যৌক্তিক initiateAction (ধ্রুবক Entry<Entry_তথ্য> &entry)
  { ফেরত entry.flags & ঠেলো; }

  স্থির যৌক্তিক performAction (ধ্রুবক Entry<Entry_তথ্য> &entry)
  { ফেরত entry.তথ্য.kern_Action_সূচক != 0xFFFF; }

  স্থির unsigned পূর্ণসংখ্যা kern_Action_সূচক (ধ্রুবক Entry<Entry_তথ্য> &entry)
  { ফেরত entry.তথ্য.kern_Action_সূচক; }
};
template <>
কাঠামো Format1Entry<মিথ্যা>
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

  প্রকারলেখ শূন্য Entry_তথ্য;

  স্থির যৌক্তিক initiateAction (ধ্রুবক Entry<Entry_তথ্য> &entry)
  { ফেরত entry.flags & ঠেলো; }

  স্থির যৌক্তিক performAction (ধ্রুবক Entry<Entry_তথ্য> &entry)
  { ফেরত entry.flags & Offset; }

  স্থির unsigned পূর্ণসংখ্যা kern_Action_সূচক (ধ্রুবক Entry<Entry_তথ্য> &entry)
  { ফেরত entry.flags & Offset; }
};

template <typename KernSubTableHeader>
কাঠামো KerxSubTableFormat1
{
  প্রকারলেখ typename KernSubTableHeader::Types Types;
  প্রকারলেখ typename Types::HBUINT HBUINT;

  প্রকারলেখ Format1Entry<Types::extended> Format1EntryT;
  প্রকারলেখ typename Format1EntryT::Entry_তথ্য Entry_তথ্য;

  গণনা Flags
  {
    DontAdvance	= Format1EntryT::DontAdvance,
  };

  যৌক্তিক is_action_initiable (ধ্রুবক Entry<Entry_তথ্য> &entry) ধ্রুবক
  {
    ফেরত Format1EntryT::initiateAction (entry);
  }
  যৌক্তিক is_actionable (ধ্রুবক Entry<Entry_তথ্য> &entry) ধ্রুবক
  {
    ফেরত Format1EntryT::performAction (entry);
  }

  কাঠামো driver_context_t
  {
    স্থির constexpr যৌক্তিক in_place = সত্য;

    driver_context_t (ধ্রুবক KerxSubTableFormat1 *table_,
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
    শূন্য transition (hb_বাফার_t *বাফার,
		     StateTableDriver<Types, Entry_তথ্য, Flags> *driver,
		     ধ্রুবক Entry<Entry_তথ্য> &entry)
    {
      unsigned পূর্ণসংখ্যা flags = entry.flags;

      যদি (flags & Format1EntryT::Reset)
	depth = 0;

      যদি (flags & Format1EntryT::ঠেলো)
      {
	যদি (likely (depth < ARRAY_দৈর্ঘ্য (স্তূপ)))
	  স্তূপ[depth++] = বাফার->idx;
	অন্যথা
	  depth = 0; /* Probably not what CoreText does, but better? */
      }

      যদি (Format1EntryT::performAction (entry) && depth)
      {
	unsigned পূর্ণসংখ্যা tuple_গুনতি_2 = hb_max (1u, table->header.tuple_গুনতি_2 ());

	unsigned পূর্ণসংখ্যা kern_idx = Format1EntryT::kern_Action_সূচক (entry);
	kern_idx = Types::byte_Offset_To_সূচক (kern_idx, &table->machine, kernAction.arrayZ);
	ধ্রুবক FWORD *actions = &kernAction[kern_idx];
	যদি (!c->sanitizer.check_array (actions, depth, tuple_গুনতি_2))
	{
	  depth = 0;
	  ফেরত;
	}
	hb_barrier ();

	hb_mask_t kern_mask = c->plan->kern_mask;

	/* From Apple 'kern' spec:
	 * "Each pops one glyph from the kerning stack and applies the kerning value to it.
	 * The end of the list is marked by an odd value... */
	যৌক্তিক last = মিথ্যা;
	যখন (!last && depth)
	{
	  unsigned পূর্ণসংখ্যা idx = স্তূপ[--depth];
	  পূর্ণসংখ্যা v = *actions;
	  actions += tuple_গুনতি_2;
	  যদি (idx >= বাফার->len) চালু;

	  /* "The end of the list is marked by an odd value..." */
	  last = v & 1;
	  v &= ~1;

	  hb_glyph_position_t &o = বাফার->pos[idx];

	  যদি (HB_DIRECTION_IS_HORIZONTAL (বাফার->props.direction))
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
		বাফার->scratch_flags |= HB_বাফার_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT;
	      }
	    }
	    অন্যথা যদি (বাফার->info[idx].mask & kern_mask)
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
		বাফার->scratch_flags |= HB_বাফার_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT;
	      }
	    }
	    অন্যথা যদি (বাফার->info[idx].mask & kern_mask)
	    {
	      auto scaled = c->font->em_scale_y (v);
	      o.y_advance = hb_saturate_add (o.y_advance, scaled);
	      o.y_offset = hb_saturate_add (o.y_offset, scaled);
	    }
	  }
	}
      }
    }

    সর্বজনীন:
    hb_aat_apply_context_t *c;
    ধ্রুবক KerxSubTableFormat1 *table;
    ব্যক্তিগত:
    ধ্রুবক UnsizedArrayOf<FWORD> &kernAction;
    unsigned পূর্ণসংখ্যা স্তূপ[8];
    unsigned পূর্ণসংখ্যা depth;
    যৌক্তিক crossStream;
  };

  যৌক্তিক apply (hb_aat_apply_context_t *c) ধ্রুবক
  {
    TRACE_APPLY (এই);

    যদি (!c->plan->requested_kerning &&
	!(header.coverage & header.CrossStream))
      ফেরত মিথ্যা;

    driver_context_t dc (এই, c);

    StateTableDriver<Types, Entry_তথ্য, Flags> driver (machine, c->font->face);

    driver.drive (&dc, c);

    return_trace (সত্য);
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    /* The rest of array sanitizations are done at run-time. */
    return_trace (likely (c->check_struct (এই) &&
			  machine.sanitize (c)));
  }

  template <typename set_t>
  শূন্য collect_glyphs (set_t &first_set, set_t &second_set, unsigned অঙ্ক_glyphs) ধ্রুবক
  {
    machine.collect_initial_glyphs (first_set, অঙ্ক_glyphs, *এই);
    //machine.collect_glyphs (second_set, num_glyphs); // second_set is unused for machine kerning
  }

  সুরক্ষিত:
  KernSubTableHeader				header;
  StateTable<Types, Entry_তথ্য>			machine;
  NNOffsetTo<UnsizedArrayOf<FWORD>, HBUINT>	kernAction;
  সর্বজনীন:
  DEFINE_আকার_STATIC (KernSubTableHeader::static_আকার + (StateTable<Types, Entry_তথ্য>::static_আকার + HBUINT::static_আকার));
};

template <typename KernSubTableHeader>
কাঠামো KerxSubTableFormat2
{
  প্রকারলেখ typename KernSubTableHeader::Types Types;
  প্রকারলেখ typename Types::HBUINT HBUINT;

  পূর্ণসংখ্যা get_kerning (hb_codepoint_t left, hb_codepoint_t ডান,
		   hb_aat_apply_context_t *c) ধ্রুবক
  {
    unsigned পূর্ণসংখ্যা অঙ্ক_glyphs = c->sanitizer.get_অঙ্ক_glyphs ();
    unsigned পূর্ণসংখ্যা l = (এই+বাম_Class_Table).get_class (left, অঙ্ক_glyphs, 0);
    unsigned পূর্ণসংখ্যা r = (এই+ডান_Class_Table).get_class (ডান, অঙ্ক_glyphs, 0);

    ধ্রুবক UnsizedArrayOf<FWORD> &arrayZ = এই+array;
    unsigned পূর্ণসংখ্যা kern_idx = l + r;
    kern_idx = Types::offset_To_সূচক (kern_idx, এই, arrayZ.arrayZ);
    ধ্রুবক FWORD *v = &arrayZ[kern_idx];
    যদি (unlikely (!v->sanitize (&c->sanitizer))) ফেরত 0;
    hb_barrier ();

    ফেরত kerxTupleKern (*v, header.tuple_গুনতি_2 (), এই, c);
  }

  যৌক্তিক apply (hb_aat_apply_context_t *c) ধ্রুবক
  {
    TRACE_APPLY (এই);

    যদি (!c->plan->requested_kerning)
      return_trace (মিথ্যা);

    যদি (header.coverage & header.Backwards)
      return_trace (মিথ্যা);

    accelerator_t accel (*এই, c);
    hb_kern_machine_t<accelerator_t> machine (accel, header.coverage & header.CrossStream);
    machine.kern (c->font, c->বাফার, c->plan->kern_mask);

    return_trace (সত্য);
  }

  template <typename set_t>
  শূন্য collect_glyphs (set_t &first_set, set_t &second_set, unsigned অঙ্ক_glyphs) ধ্রুবক
  {
    (এই+বাম_Class_Table).collect_glyphs (first_set, অঙ্ক_glyphs);
    (এই+ডান_Class_Table).collect_glyphs (second_set, অঙ্ক_glyphs);
  }

  কাঠামো accelerator_t
  {
    ধ্রুবক KerxSubTableFormat2 &table;
    hb_aat_apply_context_t *c;

    accelerator_t (ধ্রুবক KerxSubTableFormat2 &table_,
		   hb_aat_apply_context_t *c_) :
		     table (table_), c (c_) {}

    পূর্ণসংখ্যা get_kerning (hb_codepoint_t left, hb_codepoint_t ডান) ধ্রুবক
    {
      যদি (!(*c->first_set)[left] || !(*c->second_set)[ডান]) ফেরত 0;
      ফেরত table.get_kerning (left, ডান, c);
    }
  };

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (likely (c->check_struct (এই) &&
			  বাম_Class_Table.sanitize (c, এই) &&
			  ডান_Class_Table.sanitize (c, এই) &&
			  hb_barrier () &&
			  c->check_range (এই, array)));
  }

  সুরক্ষিত:
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
  সর্বজনীন:
  DEFINE_আকার_STATIC (KernSubTableHeader::static_আকার + 4 * আকার (HBUINT));
};

template <typename KernSubTableHeader>
কাঠামো KerxSubTableFormat4
{
  প্রকারলেখ ExtendedTypes Types;

  কাঠামো Entry_তথ্য
  {
    HBUINT16	ankr_Action_সূচক;/* Either 0xFFFF (for no action) or the index of
				 * the action to perform. */
    সর্বজনীন:
    DEFINE_আকার_STATIC (2);
  };

  গণনা Flags
  {
    Mark		= 0x8000,	/* If set, remember this glyph as the marked glyph. */
    DontAdvance		= 0x4000,	/* If set, don't advance to the next glyph before
					 * going to the new state. */
    Reserved		= 0x3FFF,	/* Not used; set to 0. */
  };

  যৌক্তিক is_action_initiable (ধ্রুবক Entry<Entry_তথ্য> &entry) ধ্রুবক
  {
    ফেরত (entry.flags & Mark);
  }
  যৌক্তিক is_actionable (ধ্রুবক Entry<Entry_তথ্য> &entry) ধ্রুবক
  {
    ফেরত entry.তথ্য.ankr_Action_সূচক != 0xFFFF;
  }

  কাঠামো driver_context_t
  {
    স্থির constexpr যৌক্তিক in_place = সত্য;
    গণনা SubTableFlags
    {
      ActionType	= 0xC0000000,	/* A two-bit field containing the action type. */
      Unused		= 0x3F000000,	/* Unused - must be zero. */
      Offset		= 0x00FFFFFF,	/* Masks the offset in bytes from the beginning
					 * of the subtable to the beginning of the control
					 * point table. */
    };

    driver_context_t (ধ্রুবক KerxSubTableFormat4 *table_,
		      hb_aat_apply_context_t *c_) :
	c (c_),
	table (table_),
	action_type ((table->flags & ActionType) >> 30),
	ankr_তথ্য ((HBUINT16 *) ((ধ্রুবক অক্ষর *) &table->machine + (table->flags & Offset))),
	mark_set (মিথ্যা),
	mark (0) {}

    HB_AAT_TRANSITION_INLINE
    শূন্য transition (hb_বাফার_t *বাফার,
		     StateTableDriver<Types, Entry_তথ্য, Flags> *driver,
		     ধ্রুবক Entry<Entry_তথ্য> &entry)
    {
      যদি (mark_set && entry.তথ্য.ankr_Action_সূচক != 0xFFFF && বাফার->idx < বাফার->len)
      {
	hb_glyph_position_t &o = বাফার->cur_pos();
	নির্বাচন (action_type)
	{
	  ক্ষেত্র 0: /* Control Point Actions.*/
	  {
	    /* Indexed into glyph outline. */
	    /* Each action (record in ankrData) contains two 16-bit fields, so we must
	       double the ankrActionIndex to get the correct offset here. */
	    ধ্রুবক HBUINT16 *তথ্য = &ankr_তথ্য[entry.তথ্য.ankr_Action_সূচক * 2];
	    যদি (!c->sanitizer.check_array (তথ্য, 2)) ফেরত;
	    hb_barrier ();
	    unsigned পূর্ণসংখ্যা markControlPoint = *তথ্য++;
	    unsigned পূর্ণসংখ্যা currControlPoint = *তথ্য++;
	    hb_position_t markX = 0;
	    hb_position_t markY = 0;
	    hb_position_t currX = 0;
	    hb_position_t currY = 0;
	    যদি (!c->font->get_glyph_contour_point_for_origin (c->বাফার->info[mark].codepoint,
							      markControlPoint,
							      HB_DIRECTION_LTR /*XXX*/,
							      &markX, &markY) ||
		!c->font->get_glyph_contour_point_for_origin (c->বাফার->cur ().codepoint,
							      currControlPoint,
							      HB_DIRECTION_LTR /*XXX*/,
							      &currX, &currY))
	      ফেরত;

	    o.x_offset = hb_saturate_sub (markX, currX);
	    o.y_offset = hb_saturate_sub (markY, currY);
	  }
	  বিরতি;

	  ক্ষেত্র 1: /* Anchor Point Actions. */
	  {
	    /* Indexed into 'ankr' table. */
	    /* Each action (record in ankrData) contains two 16-bit fields, so we must
	       double the ankrActionIndex to get the correct offset here. */
	    ধ্রুবক HBUINT16 *তথ্য = &ankr_তথ্য[entry.তথ্য.ankr_Action_সূচক * 2];
	    যদি (!c->sanitizer.check_array (তথ্য, 2)) ফেরত;
	    hb_barrier ();
	    unsigned পূর্ণসংখ্যা markAnchorPoint = *তথ্য++;
	    unsigned পূর্ণসংখ্যা currAnchorPoint = *তথ্য++;
	    ধ্রুবক Anchor &markAnchor = c->ankr_table->get_anchor (c->বাফার->info[mark].codepoint,
								  markAnchorPoint,
								  c->sanitizer.get_অঙ্ক_glyphs ());
	    ধ্রুবক Anchor &currAnchor = c->ankr_table->get_anchor (c->বাফার->cur ().codepoint,
								  currAnchorPoint,
								  c->sanitizer.get_অঙ্ক_glyphs ());

	    o.x_offset = hb_saturate_sub (c->font->em_scale_x (markAnchor.xCoordinate),
					  c->font->em_scale_x (currAnchor.xCoordinate));
	    o.y_offset = hb_saturate_sub (c->font->em_scale_y (markAnchor.yCoordinate),
					  c->font->em_scale_y (currAnchor.yCoordinate));
	  }
	  বিরতি;

	  ক্ষেত্র 2: /* Control Point Coordinate Actions. */
	  {
	    /* Each action contains four 16-bit fields, so we multiply the ankrActionIndex
	       by 4 to get the correct offset for the given action. */
	    ধ্রুবক FWORD *তথ্য = (ধ্রুবক FWORD *) &ankr_তথ্য[entry.তথ্য.ankr_Action_সূচক * 4];
	    যদি (!c->sanitizer.check_array (তথ্য, 4)) ফেরত;
	    hb_barrier ();
	    পূর্ণসংখ্যা markX = *তথ্য++;
	    পূর্ণসংখ্যা markY = *তথ্য++;
	    পূর্ণসংখ্যা currX = *তথ্য++;
	    পূর্ণসংখ্যা currY = *তথ্য++;

	    o.x_offset = hb_saturate_sub (c->font->em_scale_x (markX), c->font->em_scale_x (currX));
	    o.y_offset = hb_saturate_sub (c->font->em_scale_y (markY), c->font->em_scale_y (currY));
	  }
	  বিরতি;
	}
	o.attach_type() = OT::Layout::GPOS_impl::ATTACH_TYPE_MARK;
	o.attach_chain() = (পূর্ণসংখ্যা) mark - (পূর্ণসংখ্যা) বাফার->idx;
	যদি (c->বাফার_is_reversed)
	  o.attach_chain() = -o.attach_chain();
	বাফার->scratch_flags |= HB_বাফার_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT;
      }

      যদি (entry.flags & Mark)
      {
	mark_set = সত্য;
	mark = বাফার->idx;
      }
    }

    সর্বজনীন:
    hb_aat_apply_context_t *c;
    ধ্রুবক KerxSubTableFormat4 *table;
    ব্যক্তিগত:
    unsigned পূর্ণসংখ্যা action_type;
    ধ্রুবক HBUINT16 *ankr_তথ্য;
    যৌক্তিক mark_set;
    unsigned পূর্ণসংখ্যা mark;
  };

  যৌক্তিক apply (hb_aat_apply_context_t *c) ধ্রুবক
  {
    TRACE_APPLY (এই);

    driver_context_t dc (এই, c);

    StateTableDriver<Types, Entry_তথ্য, Flags> driver (machine, c->font->face);

    driver.drive (&dc, c);

    return_trace (সত্য);
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    /* The rest of array sanitizations are done at run-time. */
    return_trace (likely (c->check_struct (এই) &&
			  machine.sanitize (c)));
  }

  template <typename set_t>
  শূন্য collect_glyphs (set_t &first_set, set_t &second_set, unsigned অঙ্ক_glyphs) ধ্রুবক
  {
    machine.collect_initial_glyphs (first_set, অঙ্ক_glyphs, *এই);
    //machine.collect_glyphs (second_set, num_glyphs); // second_set is unused for machine kerning
  }

  সুরক্ষিত:
  KernSubTableHeader		header;
  StateTable<Types, Entry_তথ্য>	machine;
  HBUINT32			flags;
  সর্বজনীন:
  DEFINE_আকার_STATIC (KernSubTableHeader::static_আকার + (StateTable<Types, Entry_তথ্য>::static_আকার + HBUINT32::static_আকার));
};

template <typename KernSubTableHeader>
কাঠামো KerxSubTableFormat6
{
  গণনা Flags
  {
    মানসমূহ_Are_Long	= 0x00000001,
  };

  যৌক্তিক is_long () ধ্রুবক { ফেরত flags & মানসমূহ_Are_Long; }

  পূর্ণসংখ্যা get_kerning (hb_codepoint_t left, hb_codepoint_t ডান,
		   hb_aat_apply_context_t *c) ধ্রুবক
  {
    unsigned পূর্ণসংখ্যা অঙ্ক_glyphs = c->sanitizer.get_অঙ্ক_glyphs ();
    যদি (is_long ())
    {
      ধ্রুবক auto &t = u.l;
      unsigned পূর্ণসংখ্যা l = (এই+t.row_সূচক_Table).get_মান_or_null (left, অঙ্ক_glyphs);
      unsigned পূর্ণসংখ্যা r = (এই+t.column_সূচক_Table).get_মান_or_null (ডান, অঙ্ক_glyphs);
      unsigned পূর্ণসংখ্যা offset = l + r;
      যদি (unlikely (offset < l)) ফেরত 0; /* Addition overflow. */
      যদি (unlikely (hb_unsigned_mul_overflows (offset, আকার (FWORD32)))) ফেরত 0;
      ধ্রুবক FWORD32 *v = &StructAtOffset<FWORD32> (&(এই+t.array), offset * আকার (FWORD32));
      যদি (unlikely (!v->sanitize (&c->sanitizer))) ফেরত 0;
      hb_barrier ();
      ফেরত kerxTupleKern (*v, header.tuple_গুনতি_2 (), &(এই+vector), c);
    }
    অন্যথা
    {
      ধ্রুবক auto &t = u.s;
      unsigned পূর্ণসংখ্যা l = (এই+t.row_সূচক_Table).get_মান_or_null (left, অঙ্ক_glyphs);
      unsigned পূর্ণসংখ্যা r = (এই+t.column_সূচক_Table).get_মান_or_null (ডান, অঙ্ক_glyphs);
      unsigned পূর্ণসংখ্যা offset = l + r;
      ধ্রুবক FWORD *v = &StructAtOffset<FWORD> (&(এই+t.array), offset * আকার (FWORD));
      যদি (unlikely (!v->sanitize (&c->sanitizer))) ফেরত 0;
      hb_barrier ();
      ফেরত kerxTupleKern (*v, header.tuple_গুনতি_2 (), &(এই+vector), c);
    }
  }

  যৌক্তিক apply (hb_aat_apply_context_t *c) ধ্রুবক
  {
    TRACE_APPLY (এই);

    যদি (!c->plan->requested_kerning)
      return_trace (মিথ্যা);

    যদি (header.coverage & header.Backwards)
      return_trace (মিথ্যা);

    accelerator_t accel (*এই, c);
    hb_kern_machine_t<accelerator_t> machine (accel, header.coverage & header.CrossStream);
    machine.kern (c->font, c->বাফার, c->plan->kern_mask);

    return_trace (সত্য);
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (likely (c->check_struct (এই) &&
			  hb_barrier () &&
			  (is_long () ?
			   (
			     u.l.row_সূচক_Table.sanitize (c, এই) &&
			     u.l.column_সূচক_Table.sanitize (c, এই) &&
			     c->check_range (এই, u.l.array)
			   ) : (
			     u.s.row_সূচক_Table.sanitize (c, এই) &&
			     u.s.column_সূচক_Table.sanitize (c, এই) &&
			     c->check_range (এই, u.s.array)
			   )) &&
			  (header.tuple_গুনতি_2 () == 0 ||
			   c->check_range (এই, vector))));
  }

  template <typename set_t>
  শূন্য collect_glyphs (set_t &first_set, set_t &second_set, unsigned অঙ্ক_glyphs) ধ্রুবক
  {
    যদি (is_long ())
    {
      ধ্রুবক auto &t = u.l;
      (এই+t.row_সূচক_Table).collect_glyphs (first_set, অঙ্ক_glyphs);
      (এই+t.column_সূচক_Table).collect_glyphs (second_set, অঙ্ক_glyphs);
    }
    অন্যথা
    {
      ধ্রুবক auto &t = u.s;
      (এই+t.row_সূচক_Table).collect_glyphs (first_set, অঙ্ক_glyphs);
      (এই+t.column_সূচক_Table).collect_glyphs (second_set, অঙ্ক_glyphs);
    }
  }

  কাঠামো accelerator_t
  {
    ধ্রুবক KerxSubTableFormat6 &table;
    hb_aat_apply_context_t *c;

    accelerator_t (ধ্রুবক KerxSubTableFormat6 &table_,
		   hb_aat_apply_context_t *c_) :
		     table (table_), c (c_) {}

    পূর্ণসংখ্যা get_kerning (hb_codepoint_t left, hb_codepoint_t ডান) ধ্রুবক
    {
      যদি (!(*c->first_set)[left] || !(*c->second_set)[ডান]) ফেরত 0;
      ফেরত table.get_kerning (left, ডান, c);
    }
  };

  সুরক্ষিত:
  KernSubTableHeader		header;
  HBUINT32			flags;
  HBUINT16			row_গুনতি;
  HBUINT16			column_গুনতি;
  সংঘ U
  {
    কাঠামো Long
    {
      NNOffset32To<Lookup<HBUINT32>>		row_সূচক_Table;
      NNOffset32To<Lookup<HBUINT32>>		column_সূচক_Table;
      NNOffset32To<UnsizedArrayOf<FWORD32>>	array;
    } l;
    কাঠামো Short
    {
      NNOffset32To<Lookup<HBUINT16>>		row_সূচক_Table;
      NNOffset32To<Lookup<HBUINT16>>		column_সূচক_Table;
      NNOffset32To<UnsizedArrayOf<FWORD>>	array;
    } s;
  } u;
  NNOffset32To<UnsizedArrayOf<FWORD>>	vector;
  সর্বজনীন:
  DEFINE_আকার_STATIC (KernSubTableHeader::static_আকার + 24);
};


কাঠামো KerxSubTableHeader
{
  প্রকারলেখ ExtendedTypes Types;

  unsigned   tuple_গুনতি_2 () ধ্রুবক { ফেরত tuple_গুনতি; }
  যৌক্তিক     is_horizontal () ধ্রুবক { ফেরত !(coverage & Vertical); }

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

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই));
  }

  সর্বজনীন:
  HBUINT32	দৈর্ঘ্য;
  HBUINT32	coverage;
  HBUINT32	tuple_গুনতি;
  সর্বজনীন:
  DEFINE_আকার_STATIC (12);
};

কাঠামো KerxSubTable
{
  friend কাঠামো kerx;

  size_t get_আকার () ধ্রুবক { ফেরত u.header.দৈর্ঘ্য; }
  unsigned পূর্ণসংখ্যা get_type () ধ্রুবক { ফেরত u.header.coverage & u.header.SubtableType; }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ধ্রুবক
  {
    unsigned পূর্ণসংখ্যা subtable_type = get_type ();
    TRACE_DISPATCH (এই, subtable_type);
    নির্বাচন (subtable_type) {
    ক্ষেত্র 0:	hb_barrier (); return_trace (c->dispatch (u.format0, std::forward<Ts> (ds)...));
    ক্ষেত্র 1:	hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    ক্ষেত্র 2:	hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    ক্ষেত্র 4:	hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
    ক্ষেত্র 6:	hb_barrier (); return_trace (c->dispatch (u.format6, std::forward<Ts> (ds)...));
    পূর্বনির্ধারিত:	return_trace (c->default_return_মান ());
    }
  }

  template <typename set_t>
  শূন্য collect_glyphs (set_t &first_set, set_t &second_set, unsigned অঙ্ক_glyphs) ধ্রুবক
  {
    unsigned পূর্ণসংখ্যা subtable_type = get_type ();
    নির্বাচন (subtable_type) {
    ক্ষেত্র 0:	hb_barrier (); u.format0.collect_glyphs (first_set, second_set, অঙ্ক_glyphs); ফেরত;
    ক্ষেত্র 1:	hb_barrier (); u.format1.collect_glyphs (first_set, second_set, অঙ্ক_glyphs); ফেরত;
    ক্ষেত্র 2:	hb_barrier (); u.format2.collect_glyphs (first_set, second_set, অঙ্ক_glyphs); ফেরত;
    ক্ষেত্র 4:	hb_barrier (); u.format4.collect_glyphs (first_set, second_set, অঙ্ক_glyphs); ফেরত;
    ক্ষেত্র 6:	hb_barrier (); u.format6.collect_glyphs (first_set, second_set, অঙ্ক_glyphs); ফেরত;
    পূর্বনির্ধারিত:	ফেরত;
    }
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    যদি (!(u.header.sanitize (c) &&
	  hb_barrier () &&
	  u.header.দৈর্ঘ্য >= u.header.static_আকার &&
	  c->check_range (এই, u.header.দৈর্ঘ্য)))
      return_trace (মিথ্যা);

    return_trace (dispatch (c));
  }

  সর্বজনীন:
  সংঘ {
  KerxSubTableHeader				header;
  KerxSubTableFormat0<KerxSubTableHeader>	format0;
  KerxSubTableFormat1<KerxSubTableHeader>	format1;
  KerxSubTableFormat2<KerxSubTableHeader>	format2;
  KerxSubTableFormat4<KerxSubTableHeader>	format4;
  KerxSubTableFormat6<KerxSubTableHeader>	format6;
  } u;
  সর্বজনীন:
  DEFINE_আকার_MIN (12);
};


/*
 * The 'kerx' Table
 */

কাঠামো kern_subtable_accelerator_তথ্য_t
{
  hb_bit_set_t first_set;
  hb_bit_set_t second_set;
  mutable hb_aat_class_cache_t class_cache;
};

কাঠামো kern_accelerator_তথ্য_t
{
  hb_vector_t<kern_subtable_accelerator_তথ্য_t> subtable_accels;
  hb_aat_scratch_t scratch;
};

template <typename T>
কাঠামো KerxTable
{
  /* https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern */
  ধ্রুবক T* thiz () ধ্রুবক { ফেরত static_cast<ধ্রুবক T *> (এই); }

  যৌক্তিক has_state_machine () ধ্রুবক
  {
    প্রকারলেখ typename T::SubTable SubTable;

    ধ্রুবক SubTable *st = &thiz()->firstSubTable;
    unsigned পূর্ণসংখ্যা গুনতি = thiz()->table_গুনতি;
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < গুনতি; i++)
    {
      যদি (st->get_type () == 1)
	ফেরত সত্য;

      // TODO: What about format 4? What's this API used for anyway?

      st = &StructAfter<SubTable> (*st);
    }
    ফেরত মিথ্যা;
  }

  যৌক্তিক has_cross_stream () ধ্রুবক
  {
    প্রকারলেখ typename T::SubTable SubTable;

    ধ্রুবক SubTable *st = &thiz()->firstSubTable;
    unsigned পূর্ণসংখ্যা গুনতি = thiz()->table_গুনতি;
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < গুনতি; i++)
    {
      যদি (st->u.header.coverage & st->u.header.CrossStream)
	ফেরত সত্য;
      st = &StructAfter<SubTable> (*st);
    }
    ফেরত মিথ্যা;
  }

  পূর্ণসংখ্যা get_h_kerning (hb_codepoint_t left, hb_codepoint_t ডান) ধ্রুবক
  {
    প্রকারলেখ typename T::SubTable SubTable;

    পূর্ণসংখ্যা v = 0;
    ধ্রুবক SubTable *st = &thiz()->firstSubTable;
    unsigned পূর্ণসংখ্যা গুনতি = thiz()->table_গুনতি;
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < গুনতি; i++)
    {
      যদি ((st->u.header.coverage & (st->u.header.Variation | st->u.header.CrossStream)) ||
	  !st->u.header.is_horizontal ())
	চালু;
      v += st->get_kerning (left, ডান);
      st = &StructAfter<SubTable> (*st);
    }
    ফেরত v;
  }

  যৌক্তিক apply (AAT::hb_aat_apply_context_t *c,
	      ধ্রুবক kern_accelerator_তথ্য_t &accel_তথ্য) ধ্রুবক
  {
    c->বাফার->unsafe_to_concat ();

    c->setup_বাফার_glyph_set ();

    প্রকারলেখ typename T::SubTable SubTable;

    যৌক্তিক ret = মিথ্যা;
    যৌক্তিক seenCrossStream = মিথ্যা;
    c->set_lookup_সূচক (0);
    ধ্রুবক SubTable *st = &thiz()->firstSubTable;
    unsigned পূর্ণসংখ্যা গুনতি = thiz()->table_গুনতি;
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < গুনতি; i++)
    {
      যৌক্তিক reverse;

      auto &subtable_accel = accel_তথ্য.subtable_accels[i];

      যদি (!T::Types::extended && (st->u.header.coverage & st->u.header.Variation))
	যাও skip;

      যদি (HB_DIRECTION_IS_HORIZONTAL (c->বাফার->props.direction) != st->u.header.is_horizontal ())
	যাও skip;

      c->first_set = &subtable_accel.first_set;
      c->second_set = &subtable_accel.second_set;
      c->machine_class_cache = &subtable_accel.class_cache;

      যদি (!c->বাফার_intersects_machine ())
      {
	(শূন্য) c->বাফার->message (c->font, "skipped subtable %u because no glyph matches", c->lookup_সূচক);
	যাও skip;
      }

      reverse = যৌক্তিক (st->u.header.coverage & st->u.header.Backwards) !=
		HB_DIRECTION_IS_BACKWARD (c->বাফার->props.direction);

      যদি (!c->বাফার->message (c->font, "start subtable %u", c->lookup_সূচক))
	যাও skip;

      যদি (!seenCrossStream &&
	  (st->u.header.coverage & st->u.header.CrossStream))
      {
	/* Attach all glyphs into a chain. */
	seenCrossStream = সত্য;
	hb_glyph_position_t *pos = c->বাফার->pos;
	unsigned পূর্ণসংখ্যা গুনতি = c->বাফার->len;
	জন্য (unsigned পূর্ণসংখ্যা i = 0; i < গুনতি; i++)
	{
	  pos[i].attach_type() = OT::Layout::GPOS_impl::ATTACH_TYPE_CURSIVE;
	  pos[i].attach_chain() = HB_DIRECTION_IS_FORWARD (c->বাফার->props.direction) ? -1 : +1;
	  /* We intentionally don't set HB_BUFFER_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT,
	   * since there needs to be a non-zero attachment for post-positioning to
	   * be needed. */
	}
      }

      যদি (reverse != c->বাফার_is_reversed)
        c->reverse_বাফার ();

      {
	/* See comment in sanitize() for conditional here. */
	hb_sanitize_with_object_t with (&c->sanitizer, i < গুনতি - 1 ? st : (ধ্রুবক SubTable *) nullptr);
	ret |= st->dispatch (c);
      }

      (শূন্য) c->বাফার->message (c->font, "end subtable %u", c->lookup_সূচক);

    skip:
      st = &StructAfter<SubTable> (*st);
      c->set_lookup_সূচক (c->lookup_সূচক + 1);
    }
    যদি (c->বাফার_is_reversed)
      c->reverse_বাফার ();

    ফেরত ret;
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    যদি (unlikely (!(thiz()->version.sanitize (c) &&
		    hb_barrier () &&
		    (unsigned) thiz()->version >= (unsigned) T::minVersion &&
		    thiz()->table_গুনতি.sanitize (c))))
      return_trace (মিথ্যা);

    প্রকারলেখ typename T::SubTable SubTable;

    ধ্রুবক SubTable *st = &thiz()->firstSubTable;
    unsigned পূর্ণসংখ্যা গুনতি = thiz()->table_গুনতি;
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < গুনতি; i++)
    {
      যদি (unlikely (!st->u.header.sanitize (c)))
	return_trace (মিথ্যা);
      hb_barrier ();
      /* OpenType kern table has 2-byte subtable lengths.  That's limiting.
       * MS implementation also only supports one subtable, of format 0,
       * anyway.  Certain versions of some fonts, like Calibry, contain
       * kern subtable that exceeds 64kb.  Looks like, the subtable length
       * is simply ignored.  Which makes sense.  It's only needed if you
       * have multiple subtables.  To handle such fonts, we just ignore
       * the length for the last subtable. */
      hb_sanitize_with_object_t with (c, i < গুনতি - 1 ? st : (ধ্রুবক SubTable *) nullptr);

      যদি (unlikely (!st->sanitize (c)))
	return_trace (মিথ্যা);

      st = &StructAfter<SubTable> (*st);
    }

    unsigned majorVersion = thiz()->version;
    যদি (আকার (thiz()->version) == 4)
      majorVersion = majorVersion >> 16;
    যদি (majorVersion >= 3)
    {
      ধ্রুবক SubtableGlyphCoverage *coverage = (ধ্রুবক SubtableGlyphCoverage *) st;
      যদি (!coverage->sanitize (c, গুনতি))
        return_trace (মিথ্যা);
    }

    return_trace (সত্য);
  }

  kern_accelerator_তথ্য_t create_accelerator_তথ্য (unsigned অঙ্ক_glyphs) ধ্রুবক
  {
    kern_accelerator_তথ্য_t accel_তথ্য;

    প্রকারলেখ typename T::SubTable SubTable;

    ধ্রুবক SubTable *st = &thiz()->firstSubTable;
    unsigned পূর্ণসংখ্যা গুনতি = thiz()->table_গুনতি;
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < গুনতি; i++)
    {
      auto &subtable_accel = *accel_তথ্য.subtable_accels.push ();
      যদি (unlikely (accel_তথ্য.subtable_accels.in_error ()))
	  ফেরত accel_তথ্য;

      st->collect_glyphs (subtable_accel.first_set, subtable_accel.second_set, অঙ্ক_glyphs);
      subtable_accel.class_cache.clear ();

      st = &StructAfter<SubTable> (*st);
    }

    ফেরত accel_তথ্য;
  }

  কাঠামো accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      hb_sanitize_context_t sc;
      এই->table = sc.reference_table<T> (face);
      এই->accel_তথ্য = এই->table->create_accelerator_তথ্য (face->get_অঙ্ক_glyphs ());
    }
    ~accelerator_t ()
    {
      এই->table.destroy ();
    }

    hb_blob_t *get_blob () ধ্রুবক { ফেরত table.get_blob (); }

    যৌক্তিক apply (AAT::hb_aat_apply_context_t *c) ধ্রুবক
    {
      ফেরত table->apply (c, accel_তথ্য);
    }

    hb_blob_ptr_t<T> table;
    kern_accelerator_তথ্য_t accel_তথ্য;
    hb_aat_scratch_t scratch;
  };
};

কাঠামো kerx : KerxTable<kerx>
{
  friend কাঠামো KerxTable<kerx>;

  স্থির constexpr hb_tag_t tableTag = HB_AAT_TAG_kerx;
  স্থির constexpr unsigned minVersion = 2u;

  প্রকারলেখ KerxSubTableHeader SubTableHeader;
  প্রকারলেখ SubTableHeader::Types Types;
  প্রকারলেখ KerxSubTable SubTable;

  যৌক্তিক has_তথ্য () ধ্রুবক { ফেরত version; }

  সুরক্ষিত:
  HBUINT16	version;	/* The version number of the extended kerning table
				 * (currently 2, 3, or 4). */
  HBUINT16	unused;		/* Set to 0. */
  HBUINT32	table_গুনতি;	/* The number of subtables included in the extended kerning
				 * table. */
  SubTable	firstSubTable;	/* Subtables. */
/*subtableGlyphCoverageArray*/	/* Only if version >= 3. We don't use. */

  সর্বজনীন:
  DEFINE_আকার_MIN (8);
};

কাঠামো kerx_accelerator_t : kerx::accelerator_t {
  kerx_accelerator_t (hb_face_t *face) : kerx::accelerator_t (face) {}
};

} /* namespace AAT */

#endif /* HB_AAT_LAYOUT_KERX_TABLE_HH */
