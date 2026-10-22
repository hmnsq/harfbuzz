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

#अन्तर्भाव "hb-kern.hh"
#अन्तर्भाव "hb-aat-layout-ankr-table.hh"
#अन्तर्भाव "hb-set-digest.hh"

/*
 * kerx -- Extended Kerning
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6kerx.html
 */
#define HB_AAT_TAG_kerx HB_TAG('k','e','r','x')


नामाकाश AAT {

using नामाकाश OT;


स्थिर अन्तःस्थ पूर्णाङ्क
kerxTupleKern (पूर्णाङ्क value,
	       unsigned पूर्णाङ्क tuple_गणनम्,
	       ध्रुव शून्य *base,
	       hb_aat_apply_context_t *c)
{
  जइ (likely (!tuple_गणनम् || !c)) प्रत्यागम value;

  unsigned पूर्णाङ्क offset = value;
  ध्रुव FWORD *pv = &StructAtOffset<FWORD> (base, offset);
  जइ (unlikely (!c->sanitizer.check_array (pv, tuple_गणनम्))) प्रत्यागम 0;
  hb_barrier ();
  प्रत्यागम *pv;
}


संरचना hb_glyph_pair_t
{
  hb_codepoint_t left;
  hb_codepoint_t दक्षिणः;
};

संरचना KernPair
{
  पूर्णाङ्क get_kerning () ध्रुव { प्रत्यागम value; }

  पूर्णाङ्क cmp (ध्रुव hb_glyph_pair_t &o) ध्रुव
  {
    पूर्णाङ्क ret = left.cmp (o.left);
    जइ (ret) प्रत्यागम ret;
    प्रत्यागम दक्षिणः.cmp (o.दक्षिणः);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्));
  }

  सार्वजनिक:
  HBGlyphID16	left;
  HBGlyphID16	दक्षिणः;
  FWORD		value;
  सार्वजनिक:
  DEFINE_SIZE_STATIC (6);
};

template <typename KernSubTableHeader>
संरचना KerxSubTableFormat0
{
  पूर्णाङ्क get_kerning (hb_codepoint_t left, hb_codepoint_t दक्षिणः,
		   hb_aat_apply_context_t *c = nullptr) ध्रुव
  {
    hb_glyph_pair_t pair = {left, दक्षिणः};
    पूर्णाङ्क v = pairs.bsearch (pair).get_kerning ();
    प्रत्यागम kerxTupleKern (v, header.tuple_गणनम्_2 (), एतत्, c);
  }

  तार्किक apply (hb_aat_apply_context_t *c) ध्रुव
  {
    TRACE_APPLY (एतत्);

    जइ (!c->plan->requested_kerning)
      return_trace (मिच्छ);

    जइ (header.coverage & header.Backwards)
      return_trace (मिच्छ);

    accelerator_t accel (*एतत्, c);
    hb_kern_machine_t<accelerator_t> machine (accel, header.coverage & header.CrossStream);
    machine.kern (c->font, c->buffer, c->plan->kern_mask);

    return_trace (सच्च);
  }

  template <typename set_t>
  शून्य collect_glyphs (set_t &first_set, set_t &second_set, unsigned अङ्कः_glyphs) ध्रुव
  {
    प्रति (ध्रुव KernPair& pair : pairs)
    {
      first_set.add (pair.left);
      second_set.add (pair.दक्षिणः);
    }
  }

  संरचना accelerator_t
  {
    ध्रुव KerxSubTableFormat0 &table;
    hb_aat_apply_context_t *c;

    accelerator_t (ध्रुव KerxSubTableFormat0 &table_,
		   hb_aat_apply_context_t *c_) :
		     table (table_), c (c_) {}

    पूर्णाङ्क get_kerning (hb_codepoint_t left, hb_codepoint_t दक्षिणः) ध्रुव
    {
      जइ (!(*c->first_set)[left] || !(*c->second_set)[दक्षिणः]) प्रत्यागम 0;
      प्रत्यागम table.get_kerning (left, दक्षिणः, c);
    }
  };


  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (likely (pairs.sanitize (c)));
  }

  रक्षित:
  KernSubTableHeader	header;
  Bin_अन्वेषणम्_Array_Of<KernPair, typename KernSubTableHeader::Types::HBUINT>
			pairs;	/* Sorted kern records. */
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (KernSubTableHeader::static_size + 16, pairs);
};


template <तार्किक extended>
संरचना Format1Entry;

template <>
संरचना Format1Entry<सच्च>
{
  गणना Flags
  {
    प्रक्षिप		= 0x8000,	/* If set, push this glyph on the kerning stack. */
    DontAdvance		= 0x4000,	/* If set, don't advance to the next glyph
					 * before going to the new state. */
    Reset		= 0x2000,	/* If set, reset the kerning data (clear the stack) */
    Reserved		= 0x1FFF,	/* Not used; set to 0. */
  };

  संरचना EntryData
  {
    HBUINT16	kernActionIndex;/* Index into the kerning value array. If
				 * this index is 0xFFFF, then no kerning
				 * is to be performed. */
    सार्वजनिक:
    DEFINE_SIZE_STATIC (2);
  };

  स्थिर तार्किक initiateAction (ध्रुव Entry<EntryData> &entry)
  { प्रत्यागम entry.flags & प्रक्षिप; }

  स्थिर तार्किक performAction (ध्रुव Entry<EntryData> &entry)
  { प्रत्यागम entry.data.kernActionIndex != 0xFFFF; }

  स्थिर unsigned पूर्णाङ्क kernActionIndex (ध्रुव Entry<EntryData> &entry)
  { प्रत्यागम entry.data.kernActionIndex; }
};
template <>
संरचना Format1Entry<मिच्छ>
{
  गणना Flags
  {
    प्रक्षिप		= 0x8000,	/* If set, push this glyph on the kerning stack. */
    DontAdvance		= 0x4000,	/* If set, don't advance to the next glyph
					 * before going to the new state. */
    Offset		= 0x3FFF,	/* Byte offset from beginning of subtable to the
					 * value table for the glyphs on the kerning stack. */

    Reset		= 0x0000,	/* Not supported? */
  };

  प्रकारनिर्देश शून्य EntryData;

  स्थिर तार्किक initiateAction (ध्रुव Entry<EntryData> &entry)
  { प्रत्यागम entry.flags & प्रक्षिप; }

  स्थिर तार्किक performAction (ध्रुव Entry<EntryData> &entry)
  { प्रत्यागम entry.flags & Offset; }

  स्थिर unsigned पूर्णाङ्क kernActionIndex (ध्रुव Entry<EntryData> &entry)
  { प्रत्यागम entry.flags & Offset; }
};

template <typename KernSubTableHeader>
संरचना KerxSubTableFormat1
{
  प्रकारनिर्देश typename KernSubTableHeader::Types Types;
  प्रकारनिर्देश typename Types::HBUINT HBUINT;

  प्रकारनिर्देश Format1Entry<Types::extended> Format1EntryT;
  प्रकारनिर्देश typename Format1EntryT::EntryData EntryData;

  गणना Flags
  {
    DontAdvance	= Format1EntryT::DontAdvance,
  };

  तार्किक is_action_initiable (ध्रुव Entry<EntryData> &entry) ध्रुव
  {
    प्रत्यागम Format1EntryT::initiateAction (entry);
  }
  तार्किक is_actionable (ध्रुव Entry<EntryData> &entry) ध्रुव
  {
    प्रत्यागम Format1EntryT::performAction (entry);
  }

  संरचना driver_context_t
  {
    स्थिर constexpr तार्किक in_place = सच्च;

    driver_context_t (ध्रुव KerxSubTableFormat1 *table_,
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
    शून्य transition (hb_buffer_t *buffer,
		     StateTableDriver<Types, EntryData, Flags> *driver,
		     ध्रुव Entry<EntryData> &entry)
    {
      unsigned पूर्णाङ्क flags = entry.flags;

      जइ (flags & Format1EntryT::Reset)
	depth = 0;

      जइ (flags & Format1EntryT::प्रक्षिप)
      {
	जइ (likely (depth < ARRAY_LENGTH (राशिः)))
	  राशिः[depth++] = buffer->idx;
	अण्णहा
	  depth = 0; /* Probably not what CoreText does, but better? */
      }

      जइ (Format1EntryT::performAction (entry) && depth)
      {
	unsigned पूर्णाङ्क tuple_गणनम्_2 = hb_max (1u, table->header.tuple_गणनम्_2 ());

	unsigned पूर्णाङ्क kern_idx = Format1EntryT::kernActionIndex (entry);
	kern_idx = Types::byteOffsetToIndex (kern_idx, &table->machine, kernAction.arrayZ);
	ध्रुव FWORD *actions = &kernAction[kern_idx];
	जइ (!c->sanitizer.check_array (actions, depth, tuple_गणनम्_2))
	{
	  depth = 0;
	  प्रत्यागम;
	}
	hb_barrier ();

	hb_mask_t kern_mask = c->plan->kern_mask;

	/* From Apple 'kern' spec:
	 * "Each pops one glyph from the kerning stack and applies the kerning value to it.
	 * The end of the list is marked by an odd value... */
	तार्किक last = मिच्छ;
	जाव (!last && depth)
	{
	  unsigned पूर्णाङ्क idx = राशिः[--depth];
	  पूर्णाङ्क v = *actions;
	  actions += tuple_गणनम्_2;
	  जइ (idx >= buffer->len) अनुवर्त;

	  /* "The end of the list is marked by an odd value..." */
	  last = v & 1;
	  v &= ~1;

	  hb_glyph_position_t &o = buffer->pos[idx];

	  जइ (HB_DIRECTION_IS_HORIZONTAL (buffer->props.direction))
	  {
	    जइ (crossStream)
	    {
	      /* The following flag is undocumented in the spec, but described
	       * in the 'kern' table example. */
	      जइ (v == -0x8000)
	      {
		o.attach_type() = OT::Layout::GPOS_impl::ATTACH_TYPE_NONE;
		o.attach_chain() = 0;
		o.y_offset = 0;
	      }
	      अण्णहा जइ (o.attach_type())
	      {
		o.y_offset = hb_saturate_add (o.y_offset, c->font->em_scale_y (v));
		buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT;
	      }
	    }
	    अण्णहा जइ (buffer->info[idx].mask & kern_mask)
	    {
	      auto scaled = c->font->em_scale_x (v);
	      o.x_advance = hb_saturate_add (o.x_advance, scaled);
	      o.x_offset = hb_saturate_add (o.x_offset, scaled);
	    }
	  }
	  अण्णहा
	  {
	    जइ (crossStream)
	    {
	      /* CoreText doesn't do crossStream kerning in vertical.  We do. */
	      जइ (v == -0x8000)
	      {
		o.attach_type() = OT::Layout::GPOS_impl::ATTACH_TYPE_NONE;
		o.attach_chain() = 0;
		o.x_offset = 0;
	      }
	      अण्णहा जइ (o.attach_type())
	      {
		o.x_offset = hb_saturate_add (o.x_offset, c->font->em_scale_x (v));
		buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT;
	      }
	    }
	    अण्णहा जइ (buffer->info[idx].mask & kern_mask)
	    {
	      auto scaled = c->font->em_scale_y (v);
	      o.y_advance = hb_saturate_add (o.y_advance, scaled);
	      o.y_offset = hb_saturate_add (o.y_offset, scaled);
	    }
	  }
	}
      }
    }

    सार्वजनिक:
    hb_aat_apply_context_t *c;
    ध्रुव KerxSubTableFormat1 *table;
    गुप्त:
    ध्रुव UnsizedArrayOf<FWORD> &kernAction;
    unsigned पूर्णाङ्क राशिः[8];
    unsigned पूर्णाङ्क depth;
    तार्किक crossStream;
  };

  तार्किक apply (hb_aat_apply_context_t *c) ध्रुव
  {
    TRACE_APPLY (एतत्);

    जइ (!c->plan->requested_kerning &&
	!(header.coverage & header.CrossStream))
      प्रत्यागम मिच्छ;

    driver_context_t dc (एतत्, c);

    StateTableDriver<Types, EntryData, Flags> driver (machine, c->font->face);

    driver.drive (&dc, c);

    return_trace (सच्च);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    /* The rest of array sanitizations are done at run-time. */
    return_trace (likely (c->check_struct (एतत्) &&
			  machine.sanitize (c)));
  }

  template <typename set_t>
  शून्य collect_glyphs (set_t &first_set, set_t &second_set, unsigned अङ्कः_glyphs) ध्रुव
  {
    machine.collect_initial_glyphs (first_set, अङ्कः_glyphs, *एतत्);
    //machine.collect_glyphs (second_set, num_glyphs); // second_set is unused for machine kerning
  }

  रक्षित:
  KernSubTableHeader				header;
  StateTable<Types, EntryData>			machine;
  NNOffsetTo<UnsizedArrayOf<FWORD>, HBUINT>	kernAction;
  सार्वजनिक:
  DEFINE_SIZE_STATIC (KernSubTableHeader::static_size + (StateTable<Types, EntryData>::static_size + HBUINT::static_size));
};

template <typename KernSubTableHeader>
संरचना KerxSubTableFormat2
{
  प्रकारनिर्देश typename KernSubTableHeader::Types Types;
  प्रकारनिर्देश typename Types::HBUINT HBUINT;

  पूर्णाङ्क get_kerning (hb_codepoint_t left, hb_codepoint_t दक्षिणः,
		   hb_aat_apply_context_t *c) ध्रुव
  {
    unsigned पूर्णाङ्क अङ्कः_glyphs = c->sanitizer.get_अङ्कः_glyphs ();
    unsigned पूर्णाङ्क l = (एतत्+वामः_Class_Table).get_class (left, अङ्कः_glyphs, 0);
    unsigned पूर्णाङ्क r = (एतत्+दक्षिणः_Class_Table).get_class (दक्षिणः, अङ्कः_glyphs, 0);

    ध्रुव UnsizedArrayOf<FWORD> &arrayZ = एतत्+array;
    unsigned पूर्णाङ्क kern_idx = l + r;
    kern_idx = Types::offsetToIndex (kern_idx, एतत्, arrayZ.arrayZ);
    ध्रुव FWORD *v = &arrayZ[kern_idx];
    जइ (unlikely (!v->sanitize (&c->sanitizer))) प्रत्यागम 0;
    hb_barrier ();

    प्रत्यागम kerxTupleKern (*v, header.tuple_गणनम्_2 (), एतत्, c);
  }

  तार्किक apply (hb_aat_apply_context_t *c) ध्रुव
  {
    TRACE_APPLY (एतत्);

    जइ (!c->plan->requested_kerning)
      return_trace (मिच्छ);

    जइ (header.coverage & header.Backwards)
      return_trace (मिच्छ);

    accelerator_t accel (*एतत्, c);
    hb_kern_machine_t<accelerator_t> machine (accel, header.coverage & header.CrossStream);
    machine.kern (c->font, c->buffer, c->plan->kern_mask);

    return_trace (सच्च);
  }

  template <typename set_t>
  शून्य collect_glyphs (set_t &first_set, set_t &second_set, unsigned अङ्कः_glyphs) ध्रुव
  {
    (एतत्+वामः_Class_Table).collect_glyphs (first_set, अङ्कः_glyphs);
    (एतत्+दक्षिणः_Class_Table).collect_glyphs (second_set, अङ्कः_glyphs);
  }

  संरचना accelerator_t
  {
    ध्रुव KerxSubTableFormat2 &table;
    hb_aat_apply_context_t *c;

    accelerator_t (ध्रुव KerxSubTableFormat2 &table_,
		   hb_aat_apply_context_t *c_) :
		     table (table_), c (c_) {}

    पूर्णाङ्क get_kerning (hb_codepoint_t left, hb_codepoint_t दक्षिणः) ध्रुव
    {
      जइ (!(*c->first_set)[left] || !(*c->second_set)[दक्षिणः]) प्रत्यागम 0;
      प्रत्यागम table.get_kerning (left, दक्षिणः, c);
    }
  };

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (likely (c->check_struct (एतत्) &&
			  वामः_Class_Table.sanitize (c, एतत्) &&
			  दक्षिणः_Class_Table.sanitize (c, एतत्) &&
			  hb_barrier () &&
			  c->check_range (एतत्, array)));
  }

  रक्षित:
  KernSubTableHeader	header;
  HBUINT		rowWidth;	/* The width, in bytes, of a row in the table. */
  NNOffsetTo<typename Types::ClassTypeWide, HBUINT>
			वामः_Class_Table;	/* Offset from beginning of this subtable to
					 * left-hand class table. */
  NNOffsetTo<typename Types::ClassTypeWide, HBUINT>
			दक्षिणः_Class_Table;/* Offset from beginning of this subtable to
					 * right-hand class table. */
  NNOffsetTo<UnsizedArrayOf<FWORD>, HBUINT>
			 array;		/* Offset from beginning of this subtable to
					 * the start of the kerning array. */
  सार्वजनिक:
  DEFINE_SIZE_STATIC (KernSubTableHeader::static_size + 4 * परिमाण (HBUINT));
};

template <typename KernSubTableHeader>
संरचना KerxSubTableFormat4
{
  प्रकारनिर्देश ExtendedTypes Types;

  संरचना EntryData
  {
    HBUINT16	ankrActionIndex;/* Either 0xFFFF (for no action) or the index of
				 * the action to perform. */
    सार्वजनिक:
    DEFINE_SIZE_STATIC (2);
  };

  गणना Flags
  {
    Mark		= 0x8000,	/* If set, remember this glyph as the marked glyph. */
    DontAdvance		= 0x4000,	/* If set, don't advance to the next glyph before
					 * going to the new state. */
    Reserved		= 0x3FFF,	/* Not used; set to 0. */
  };

  तार्किक is_action_initiable (ध्रुव Entry<EntryData> &entry) ध्रुव
  {
    प्रत्यागम (entry.flags & Mark);
  }
  तार्किक is_actionable (ध्रुव Entry<EntryData> &entry) ध्रुव
  {
    प्रत्यागम entry.data.ankrActionIndex != 0xFFFF;
  }

  संरचना driver_context_t
  {
    स्थिर constexpr तार्किक in_place = सच्च;
    गणना SubTableFlags
    {
      ActionType	= 0xC0000000,	/* A two-bit field containing the action type. */
      Unused		= 0x3F000000,	/* Unused - must be zero. */
      Offset		= 0x00FFFFFF,	/* Masks the offset in bytes from the beginning
					 * of the subtable to the beginning of the control
					 * point table. */
    };

    driver_context_t (ध्रुव KerxSubTableFormat4 *table_,
		      hb_aat_apply_context_t *c_) :
	c (c_),
	table (table_),
	action_type ((table->flags & ActionType) >> 30),
	ankrData ((HBUINT16 *) ((ध्रुव वर्ण *) &table->machine + (table->flags & Offset))),
	mark_set (मिच्छ),
	mark (0) {}

    HB_AAT_TRANSITION_INLINE
    शून्य transition (hb_buffer_t *buffer,
		     StateTableDriver<Types, EntryData, Flags> *driver,
		     ध्रुव Entry<EntryData> &entry)
    {
      जइ (mark_set && entry.data.ankrActionIndex != 0xFFFF && buffer->idx < buffer->len)
      {
	hb_glyph_position_t &o = buffer->cur_pos();
	विकल्प (action_type)
	{
	  पक्ष 0: /* Control Point Actions.*/
	  {
	    /* Indexed into glyph outline. */
	    /* Each action (record in ankrData) contains two 16-bit fields, so we must
	       double the ankrActionIndex to get the correct offset here. */
	    ध्रुव HBUINT16 *data = &ankrData[entry.data.ankrActionIndex * 2];
	    जइ (!c->sanitizer.check_array (data, 2)) प्रत्यागम;
	    hb_barrier ();
	    unsigned पूर्णाङ्क markControlPoint = *data++;
	    unsigned पूर्णाङ्क currControlPoint = *data++;
	    hb_position_t markX = 0;
	    hb_position_t markY = 0;
	    hb_position_t currX = 0;
	    hb_position_t currY = 0;
	    जइ (!c->font->get_glyph_contour_point_for_origin (c->buffer->info[mark].codepoint,
							      markControlPoint,
							      HB_DIRECTION_LTR /*XXX*/,
							      &markX, &markY) ||
		!c->font->get_glyph_contour_point_for_origin (c->buffer->cur ().codepoint,
							      currControlPoint,
							      HB_DIRECTION_LTR /*XXX*/,
							      &currX, &currY))
	      प्रत्यागम;

	    o.x_offset = hb_saturate_sub (markX, currX);
	    o.y_offset = hb_saturate_sub (markY, currY);
	  }
	  भङ्ग;

	  पक्ष 1: /* Anchor Point Actions. */
	  {
	    /* Indexed into 'ankr' table. */
	    /* Each action (record in ankrData) contains two 16-bit fields, so we must
	       double the ankrActionIndex to get the correct offset here. */
	    ध्रुव HBUINT16 *data = &ankrData[entry.data.ankrActionIndex * 2];
	    जइ (!c->sanitizer.check_array (data, 2)) प्रत्यागम;
	    hb_barrier ();
	    unsigned पूर्णाङ्क markAnchorPoint = *data++;
	    unsigned पूर्णाङ्क currAnchorPoint = *data++;
	    ध्रुव Anchor &markAnchor = c->ankr_table->get_anchor (c->buffer->info[mark].codepoint,
								  markAnchorPoint,
								  c->sanitizer.get_अङ्कः_glyphs ());
	    ध्रुव Anchor &currAnchor = c->ankr_table->get_anchor (c->buffer->cur ().codepoint,
								  currAnchorPoint,
								  c->sanitizer.get_अङ्कः_glyphs ());

	    o.x_offset = hb_saturate_sub (c->font->em_scale_x (markAnchor.xCoordinate),
					  c->font->em_scale_x (currAnchor.xCoordinate));
	    o.y_offset = hb_saturate_sub (c->font->em_scale_y (markAnchor.yCoordinate),
					  c->font->em_scale_y (currAnchor.yCoordinate));
	  }
	  भङ्ग;

	  पक्ष 2: /* Control Point Coordinate Actions. */
	  {
	    /* Each action contains four 16-bit fields, so we multiply the ankrActionIndex
	       by 4 to get the correct offset for the given action. */
	    ध्रुव FWORD *data = (ध्रुव FWORD *) &ankrData[entry.data.ankrActionIndex * 4];
	    जइ (!c->sanitizer.check_array (data, 4)) प्रत्यागम;
	    hb_barrier ();
	    पूर्णाङ्क markX = *data++;
	    पूर्णाङ्क markY = *data++;
	    पूर्णाङ्क currX = *data++;
	    पूर्णाङ्क currY = *data++;

	    o.x_offset = hb_saturate_sub (c->font->em_scale_x (markX), c->font->em_scale_x (currX));
	    o.y_offset = hb_saturate_sub (c->font->em_scale_y (markY), c->font->em_scale_y (currY));
	  }
	  भङ्ग;
	}
	o.attach_type() = OT::Layout::GPOS_impl::ATTACH_TYPE_MARK;
	o.attach_chain() = (पूर्णाङ्क) mark - (पूर्णाङ्क) buffer->idx;
	जइ (c->buffer_is_reversed)
	  o.attach_chain() = -o.attach_chain();
	buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT;
      }

      जइ (entry.flags & Mark)
      {
	mark_set = सच्च;
	mark = buffer->idx;
      }
    }

    सार्वजनिक:
    hb_aat_apply_context_t *c;
    ध्रुव KerxSubTableFormat4 *table;
    गुप्त:
    unsigned पूर्णाङ्क action_type;
    ध्रुव HBUINT16 *ankrData;
    तार्किक mark_set;
    unsigned पूर्णाङ्क mark;
  };

  तार्किक apply (hb_aat_apply_context_t *c) ध्रुव
  {
    TRACE_APPLY (एतत्);

    driver_context_t dc (एतत्, c);

    StateTableDriver<Types, EntryData, Flags> driver (machine, c->font->face);

    driver.drive (&dc, c);

    return_trace (सच्च);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    /* The rest of array sanitizations are done at run-time. */
    return_trace (likely (c->check_struct (एतत्) &&
			  machine.sanitize (c)));
  }

  template <typename set_t>
  शून्य collect_glyphs (set_t &first_set, set_t &second_set, unsigned अङ्कः_glyphs) ध्रुव
  {
    machine.collect_initial_glyphs (first_set, अङ्कः_glyphs, *एतत्);
    //machine.collect_glyphs (second_set, num_glyphs); // second_set is unused for machine kerning
  }

  रक्षित:
  KernSubTableHeader		header;
  StateTable<Types, EntryData>	machine;
  HBUINT32			flags;
  सार्वजनिक:
  DEFINE_SIZE_STATIC (KernSubTableHeader::static_size + (StateTable<Types, EntryData>::static_size + HBUINT32::static_size));
};

template <typename KernSubTableHeader>
संरचना KerxSubTableFormat6
{
  गणना Flags
  {
    मानानि_Are_Long	= 0x00000001,
  };

  तार्किक is_long () ध्रुव { प्रत्यागम flags & मानानि_Are_Long; }

  पूर्णाङ्क get_kerning (hb_codepoint_t left, hb_codepoint_t दक्षिणः,
		   hb_aat_apply_context_t *c) ध्रुव
  {
    unsigned पूर्णाङ्क अङ्कः_glyphs = c->sanitizer.get_अङ्कः_glyphs ();
    जइ (is_long ())
    {
      ध्रुव auto &t = u.l;
      unsigned पूर्णाङ्क l = (एतत्+t.rowIndexTable).get_मानम्_or_null (left, अङ्कः_glyphs);
      unsigned पूर्णाङ्क r = (एतत्+t.columnIndexTable).get_मानम्_or_null (दक्षिणः, अङ्कः_glyphs);
      unsigned पूर्णाङ्क offset = l + r;
      जइ (unlikely (offset < l)) प्रत्यागम 0; /* Addition overflow. */
      जइ (unlikely (hb_unsigned_mul_overflows (offset, परिमाण (FWORD32)))) प्रत्यागम 0;
      ध्रुव FWORD32 *v = &StructAtOffset<FWORD32> (&(एतत्+t.array), offset * परिमाण (FWORD32));
      जइ (unlikely (!v->sanitize (&c->sanitizer))) प्रत्यागम 0;
      hb_barrier ();
      प्रत्यागम kerxTupleKern (*v, header.tuple_गणनम्_2 (), &(एतत्+vector), c);
    }
    अण्णहा
    {
      ध्रुव auto &t = u.s;
      unsigned पूर्णाङ्क l = (एतत्+t.rowIndexTable).get_मानम्_or_null (left, अङ्कः_glyphs);
      unsigned पूर्णाङ्क r = (एतत्+t.columnIndexTable).get_मानम्_or_null (दक्षिणः, अङ्कः_glyphs);
      unsigned पूर्णाङ्क offset = l + r;
      ध्रुव FWORD *v = &StructAtOffset<FWORD> (&(एतत्+t.array), offset * परिमाण (FWORD));
      जइ (unlikely (!v->sanitize (&c->sanitizer))) प्रत्यागम 0;
      hb_barrier ();
      प्रत्यागम kerxTupleKern (*v, header.tuple_गणनम्_2 (), &(एतत्+vector), c);
    }
  }

  तार्किक apply (hb_aat_apply_context_t *c) ध्रुव
  {
    TRACE_APPLY (एतत्);

    जइ (!c->plan->requested_kerning)
      return_trace (मिच्छ);

    जइ (header.coverage & header.Backwards)
      return_trace (मिच्छ);

    accelerator_t accel (*एतत्, c);
    hb_kern_machine_t<accelerator_t> machine (accel, header.coverage & header.CrossStream);
    machine.kern (c->font, c->buffer, c->plan->kern_mask);

    return_trace (सच्च);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (likely (c->check_struct (एतत्) &&
			  hb_barrier () &&
			  (is_long () ?
			   (
			     u.l.rowIndexTable.sanitize (c, एतत्) &&
			     u.l.columnIndexTable.sanitize (c, एतत्) &&
			     c->check_range (एतत्, u.l.array)
			   ) : (
			     u.s.rowIndexTable.sanitize (c, एतत्) &&
			     u.s.columnIndexTable.sanitize (c, एतत्) &&
			     c->check_range (एतत्, u.s.array)
			   )) &&
			  (header.tuple_गणनम्_2 () == 0 ||
			   c->check_range (एतत्, vector))));
  }

  template <typename set_t>
  शून्य collect_glyphs (set_t &first_set, set_t &second_set, unsigned अङ्कः_glyphs) ध्रुव
  {
    जइ (is_long ())
    {
      ध्रुव auto &t = u.l;
      (एतत्+t.rowIndexTable).collect_glyphs (first_set, अङ्कः_glyphs);
      (एतत्+t.columnIndexTable).collect_glyphs (second_set, अङ्कः_glyphs);
    }
    अण्णहा
    {
      ध्रुव auto &t = u.s;
      (एतत्+t.rowIndexTable).collect_glyphs (first_set, अङ्कः_glyphs);
      (एतत्+t.columnIndexTable).collect_glyphs (second_set, अङ्कः_glyphs);
    }
  }

  संरचना accelerator_t
  {
    ध्रुव KerxSubTableFormat6 &table;
    hb_aat_apply_context_t *c;

    accelerator_t (ध्रुव KerxSubTableFormat6 &table_,
		   hb_aat_apply_context_t *c_) :
		     table (table_), c (c_) {}

    पूर्णाङ्क get_kerning (hb_codepoint_t left, hb_codepoint_t दक्षिणः) ध्रुव
    {
      जइ (!(*c->first_set)[left] || !(*c->second_set)[दक्षिणः]) प्रत्यागम 0;
      प्रत्यागम table.get_kerning (left, दक्षिणः, c);
    }
  };

  रक्षित:
  KernSubTableHeader		header;
  HBUINT32			flags;
  HBUINT16			row_गणनम्;
  HBUINT16			column_गणनम्;
  सङ्घ U
  {
    संरचना Long
    {
      NNOffset32To<Lookup<HBUINT32>>		rowIndexTable;
      NNOffset32To<Lookup<HBUINT32>>		columnIndexTable;
      NNOffset32To<UnsizedArrayOf<FWORD32>>	array;
    } l;
    संरचना Short
    {
      NNOffset32To<Lookup<HBUINT16>>		rowIndexTable;
      NNOffset32To<Lookup<HBUINT16>>		columnIndexTable;
      NNOffset32To<UnsizedArrayOf<FWORD>>	array;
    } s;
  } u;
  NNOffset32To<UnsizedArrayOf<FWORD>>	vector;
  सार्वजनिक:
  DEFINE_SIZE_STATIC (KernSubTableHeader::static_size + 24);
};


संरचना KerxSubTableHeader
{
  प्रकारनिर्देश ExtendedTypes Types;

  unsigned   tuple_गणनम्_2 () ध्रुव { प्रत्यागम tuple_गणनम्; }
  तार्किक     is_horizontal () ध्रुव { प्रत्यागम !(coverage & Vertical); }

  गणना Coverage
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

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्));
  }

  सार्वजनिक:
  HBUINT32	length;
  HBUINT32	coverage;
  HBUINT32	tuple_गणनम्;
  सार्वजनिक:
  DEFINE_SIZE_STATIC (12);
};

संरचना KerxSubTable
{
  friend संरचना kerx;

  size_t get_size () ध्रुव { प्रत्यागम u.header.length; }
  unsigned पूर्णाङ्क get_type () ध्रुव { प्रत्यागम u.header.coverage & u.header.SubtableType; }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ध्रुव
  {
    unsigned पूर्णाङ्क subtable_type = get_type ();
    TRACE_DISPATCH (एतत्, subtable_type);
    विकल्प (subtable_type) {
    पक्ष 0:	hb_barrier (); return_trace (c->dispatch (u.format0, std::forward<Ts> (ds)...));
    पक्ष 1:	hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    पक्ष 2:	hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    पक्ष 4:	hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
    पक्ष 6:	hb_barrier (); return_trace (c->dispatch (u.format6, std::forward<Ts> (ds)...));
    स्वभाव:	return_trace (c->default_return_मानम् ());
    }
  }

  template <typename set_t>
  शून्य collect_glyphs (set_t &first_set, set_t &second_set, unsigned अङ्कः_glyphs) ध्रुव
  {
    unsigned पूर्णाङ्क subtable_type = get_type ();
    विकल्प (subtable_type) {
    पक्ष 0:	hb_barrier (); u.format0.collect_glyphs (first_set, second_set, अङ्कः_glyphs); प्रत्यागम;
    पक्ष 1:	hb_barrier (); u.format1.collect_glyphs (first_set, second_set, अङ्कः_glyphs); प्रत्यागम;
    पक्ष 2:	hb_barrier (); u.format2.collect_glyphs (first_set, second_set, अङ्कः_glyphs); प्रत्यागम;
    पक्ष 4:	hb_barrier (); u.format4.collect_glyphs (first_set, second_set, अङ्कः_glyphs); प्रत्यागम;
    पक्ष 6:	hb_barrier (); u.format6.collect_glyphs (first_set, second_set, अङ्कः_glyphs); प्रत्यागम;
    स्वभाव:	प्रत्यागम;
    }
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    जइ (!(u.header.sanitize (c) &&
	  hb_barrier () &&
	  u.header.length >= u.header.static_size &&
	  c->check_range (एतत्, u.header.length)))
      return_trace (मिच्छ);

    return_trace (dispatch (c));
  }

  सार्वजनिक:
  सङ्घ {
  KerxSubTableHeader				header;
  KerxSubTableFormat0<KerxSubTableHeader>	format0;
  KerxSubTableFormat1<KerxSubTableHeader>	format1;
  KerxSubTableFormat2<KerxSubTableHeader>	format2;
  KerxSubTableFormat4<KerxSubTableHeader>	format4;
  KerxSubTableFormat6<KerxSubTableHeader>	format6;
  } u;
  सार्वजनिक:
  DEFINE_SIZE_MIN (12);
};


/*
 * The 'kerx' Table
 */

संरचना kern_subtable_accelerator_data_t
{
  hb_bit_set_t first_set;
  hb_bit_set_t second_set;
  mutable hb_aat_class_cache_t class_cache;
};

संरचना kern_accelerator_data_t
{
  hb_vector_t<kern_subtable_accelerator_data_t> subtable_accels;
  hb_aat_scratch_t scratch;
};

template <typename T>
संरचना KerxTable
{
  /* https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern */
  ध्रुव T* thiz () ध्रुव { प्रत्यागम static_cast<ध्रुव T *> (एतत्); }

  तार्किक has_state_machine () ध्रुव
  {
    प्रकारनिर्देश typename T::SubTable SubTable;

    ध्रुव SubTable *st = &thiz()->firstSubTable;
    unsigned पूर्णाङ्क गणनम् = thiz()->table_गणनम्;
    प्रति (unsigned पूर्णाङ्क i = 0; i < गणनम्; i++)
    {
      जइ (st->get_type () == 1)
	प्रत्यागम सच्च;

      // TODO: What about format 4? What's this API used for anyway?

      st = &StructAfter<SubTable> (*st);
    }
    प्रत्यागम मिच्छ;
  }

  तार्किक has_cross_stream () ध्रुव
  {
    प्रकारनिर्देश typename T::SubTable SubTable;

    ध्रुव SubTable *st = &thiz()->firstSubTable;
    unsigned पूर्णाङ्क गणनम् = thiz()->table_गणनम्;
    प्रति (unsigned पूर्णाङ्क i = 0; i < गणनम्; i++)
    {
      जइ (st->u.header.coverage & st->u.header.CrossStream)
	प्रत्यागम सच्च;
      st = &StructAfter<SubTable> (*st);
    }
    प्रत्यागम मिच्छ;
  }

  पूर्णाङ्क get_h_kerning (hb_codepoint_t left, hb_codepoint_t दक्षिणः) ध्रुव
  {
    प्रकारनिर्देश typename T::SubTable SubTable;

    पूर्णाङ्क v = 0;
    ध्रुव SubTable *st = &thiz()->firstSubTable;
    unsigned पूर्णाङ्क गणनम् = thiz()->table_गणनम्;
    प्रति (unsigned पूर्णाङ्क i = 0; i < गणनम्; i++)
    {
      जइ ((st->u.header.coverage & (st->u.header.Variation | st->u.header.CrossStream)) ||
	  !st->u.header.is_horizontal ())
	अनुवर्त;
      v += st->get_kerning (left, दक्षिणः);
      st = &StructAfter<SubTable> (*st);
    }
    प्रत्यागम v;
  }

  तार्किक apply (AAT::hb_aat_apply_context_t *c,
	      ध्रुव kern_accelerator_data_t &accel_data) ध्रुव
  {
    c->buffer->unsafe_to_concat ();

    c->setup_buffer_glyph_set ();

    प्रकारनिर्देश typename T::SubTable SubTable;

    तार्किक ret = मिच्छ;
    तार्किक seenCrossStream = मिच्छ;
    c->set_lookup_index (0);
    ध्रुव SubTable *st = &thiz()->firstSubTable;
    unsigned पूर्णाङ्क गणनम् = thiz()->table_गणनम्;
    प्रति (unsigned पूर्णाङ्क i = 0; i < गणनम्; i++)
    {
      तार्किक reverse;

      auto &subtable_accel = accel_data.subtable_accels[i];

      जइ (!T::Types::extended && (st->u.header.coverage & st->u.header.Variation))
	गच्छ skip;

      जइ (HB_DIRECTION_IS_HORIZONTAL (c->buffer->props.direction) != st->u.header.is_horizontal ())
	गच्छ skip;

      c->first_set = &subtable_accel.first_set;
      c->second_set = &subtable_accel.second_set;
      c->machine_class_cache = &subtable_accel.class_cache;

      जइ (!c->buffer_intersects_machine ())
      {
	(शून्य) c->buffer->message (c->font, "skipped subtable %u because no glyph matches", c->lookup_index);
	गच्छ skip;
      }

      reverse = तार्किक (st->u.header.coverage & st->u.header.Backwards) !=
		HB_DIRECTION_IS_BACKWARD (c->buffer->props.direction);

      जइ (!c->buffer->message (c->font, "start subtable %u", c->lookup_index))
	गच्छ skip;

      जइ (!seenCrossStream &&
	  (st->u.header.coverage & st->u.header.CrossStream))
      {
	/* Attach all glyphs into a chain. */
	seenCrossStream = सच्च;
	hb_glyph_position_t *pos = c->buffer->pos;
	unsigned पूर्णाङ्क गणनम् = c->buffer->len;
	प्रति (unsigned पूर्णाङ्क i = 0; i < गणनम्; i++)
	{
	  pos[i].attach_type() = OT::Layout::GPOS_impl::ATTACH_TYPE_CURSIVE;
	  pos[i].attach_chain() = HB_DIRECTION_IS_FORWARD (c->buffer->props.direction) ? -1 : +1;
	  /* We intentionally don't set HB_BUFFER_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT,
	   * since there needs to be a non-zero attachment for post-positioning to
	   * be needed. */
	}
      }

      जइ (reverse != c->buffer_is_reversed)
        c->reverse_buffer ();

      {
	/* See comment in sanitize() for conditional here. */
	hb_sanitize_with_object_t with (&c->sanitizer, i < गणनम् - 1 ? st : (ध्रुव SubTable *) nullptr);
	ret |= st->dispatch (c);
      }

      (शून्य) c->buffer->message (c->font, "end subtable %u", c->lookup_index);

    skip:
      st = &StructAfter<SubTable> (*st);
      c->set_lookup_index (c->lookup_index + 1);
    }
    जइ (c->buffer_is_reversed)
      c->reverse_buffer ();

    प्रत्यागम ret;
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    जइ (unlikely (!(thiz()->version.sanitize (c) &&
		    hb_barrier () &&
		    (unsigned) thiz()->version >= (unsigned) T::minVersion &&
		    thiz()->table_गणनम्.sanitize (c))))
      return_trace (मिच्छ);

    प्रकारनिर्देश typename T::SubTable SubTable;

    ध्रुव SubTable *st = &thiz()->firstSubTable;
    unsigned पूर्णाङ्क गणनम् = thiz()->table_गणनम्;
    प्रति (unsigned पूर्णाङ्क i = 0; i < गणनम्; i++)
    {
      जइ (unlikely (!st->u.header.sanitize (c)))
	return_trace (मिच्छ);
      hb_barrier ();
      /* OpenType kern table has 2-byte subtable lengths.  That's limiting.
       * MS implementation also only supports one subtable, of format 0,
       * anyway.  Certain versions of some fonts, like Calibry, contain
       * kern subtable that exceeds 64kb.  Looks like, the subtable length
       * is simply ignored.  Which makes sense.  It's only needed if you
       * have multiple subtables.  To handle such fonts, we just ignore
       * the length for the last subtable. */
      hb_sanitize_with_object_t with (c, i < गणनम् - 1 ? st : (ध्रुव SubTable *) nullptr);

      जइ (unlikely (!st->sanitize (c)))
	return_trace (मिच्छ);

      st = &StructAfter<SubTable> (*st);
    }

    unsigned majorVersion = thiz()->version;
    जइ (परिमाण (thiz()->version) == 4)
      majorVersion = majorVersion >> 16;
    जइ (majorVersion >= 3)
    {
      ध्रुव SubtableGlyphCoverage *coverage = (ध्रुव SubtableGlyphCoverage *) st;
      जइ (!coverage->sanitize (c, गणनम्))
        return_trace (मिच्छ);
    }

    return_trace (सच्च);
  }

  kern_accelerator_data_t create_accelerator_data (unsigned अङ्कः_glyphs) ध्रुव
  {
    kern_accelerator_data_t accel_data;

    प्रकारनिर्देश typename T::SubTable SubTable;

    ध्रुव SubTable *st = &thiz()->firstSubTable;
    unsigned पूर्णाङ्क गणनम् = thiz()->table_गणनम्;
    प्रति (unsigned पूर्णाङ्क i = 0; i < गणनम्; i++)
    {
      auto &subtable_accel = *accel_data.subtable_accels.push ();
      जइ (unlikely (accel_data.subtable_accels.in_error ()))
	  प्रत्यागम accel_data;

      st->collect_glyphs (subtable_accel.first_set, subtable_accel.second_set, अङ्कः_glyphs);
      subtable_accel.class_cache.clear ();

      st = &StructAfter<SubTable> (*st);
    }

    प्रत्यागम accel_data;
  }

  संरचना accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      hb_sanitize_context_t sc;
      एतत्->table = sc.reference_table<T> (face);
      एतत्->accel_data = एतत्->table->create_accelerator_data (face->get_अङ्कः_glyphs ());
    }
    ~accelerator_t ()
    {
      एतत्->table.destroy ();
    }

    hb_blob_t *get_blob () ध्रुव { प्रत्यागम table.get_blob (); }

    तार्किक apply (AAT::hb_aat_apply_context_t *c) ध्रुव
    {
      प्रत्यागम table->apply (c, accel_data);
    }

    hb_blob_ptr_t<T> table;
    kern_accelerator_data_t accel_data;
    hb_aat_scratch_t scratch;
  };
};

संरचना kerx : KerxTable<kerx>
{
  friend संरचना KerxTable<kerx>;

  स्थिर constexpr hb_tag_t tableTag = HB_AAT_TAG_kerx;
  स्थिर constexpr unsigned minVersion = 2u;

  प्रकारनिर्देश KerxSubTableHeader SubTableHeader;
  प्रकारनिर्देश SubTableHeader::Types Types;
  प्रकारनिर्देश KerxSubTable SubTable;

  तार्किक has_data () ध्रुव { प्रत्यागम version; }

  रक्षित:
  HBUINT16	version;	/* The version number of the extended kerning table
				 * (currently 2, 3, or 4). */
  HBUINT16	unused;		/* Set to 0. */
  HBUINT32	table_गणनम्;	/* The number of subtables included in the extended kerning
				 * table. */
  SubTable	firstSubTable;	/* Subtables. */
/*subtableGlyphCoverageArray*/	/* Only if version >= 3. We don't use. */

  सार्वजनिक:
  DEFINE_SIZE_MIN (8);
};

संरचना kerx_accelerator_t : kerx::accelerator_t {
  kerx_accelerator_t (hb_face_t *face) : kerx::accelerator_t (face) {}
};

} /* namespace AAT */

#endif /* HB_AAT_LAYOUT_KERX_TABLE_HH */
