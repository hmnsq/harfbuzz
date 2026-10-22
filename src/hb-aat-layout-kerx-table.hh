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
	       अचिन्हित पूर्णाङ्क tuple_गन्ती,
	       अचर शून्य *base,
	       hb_aat_apply_context_t *c)
{
  यदि (likely (!tuple_गन्ती || !c)) फर्काऊ value;

  अचिन्हित पूर्णाङ्क offset = value;
  अचर FWORD *pv = &StructAtOffset<FWORD> (base, offset);
  यदि (unlikely (!c->sanitizer.check_array (pv, tuple_गन्ती))) फर्काऊ 0;
  hb_barrier ();
  फर्काऊ *pv;
}


संरचना hb_glyph_pair_t
{
  hb_codepoint_t left;
  hb_codepoint_t दायाँ;
};

संरचना KernPair
{
  पूर्णाङ्क get_kerning () अचर { फर्काऊ value; }

  पूर्णाङ्क cmp (अचर hb_glyph_pair_t &o) अचर
  {
    पूर्णाङ्क ret = left.cmp (o.left);
    यदि (ret) फर्काऊ ret;
    फर्काऊ दायाँ.cmp (o.दायाँ);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) अचर
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्));
  }

  सार्वजनिक:
  HBGlyphID16	left;
  HBGlyphID16	दायाँ;
  FWORD		value;
  सार्वजनिक:
  DEFINE_SIZE_STATIC (6);
};

ढाँचा <typename KernSubTableHeader>
संरचना KerxSubTableFormat0
{
  पूर्णाङ्क get_kerning (hb_codepoint_t left, hb_codepoint_t दायाँ,
		   hb_aat_apply_context_t *c = nullptr) अचर
  {
    hb_glyph_pair_t pair = {left, दायाँ};
    पूर्णाङ्क v = pairs.द्वा_खोज (pair).get_kerning ();
    फर्काऊ kerxTupleKern (v, header.tuple_गन्ती_2 (), एतत्, c);
  }

  तार्किक apply (hb_aat_apply_context_t *c) अचर
  {
    TRACE_APPLY (एतत्);

    यदि (!c->plan->requested_kerning)
      return_trace (असत्य);

    यदि (header.coverage & header.Backwards)
      return_trace (असत्य);

    accelerator_t accel (*एतत्, c);
    hb_kern_machine_t<accelerator_t> machine (accel, header.coverage & header.CrossStream);
    machine.kern (c->font, c->buffer, c->plan->kern_mask);

    return_trace (सत्य);
  }

  ढाँचा <typename set_t>
  शून्य collect_glyphs (set_t &first_set, set_t &second_set, अचिन्हित अङ्क_glyphs) अचर
  {
    लागि (अचर KernPair& pair : pairs)
    {
      first_set.add (pair.left);
      second_set.add (pair.दायाँ);
    }
  }

  संरचना accelerator_t
  {
    अचर KerxSubTableFormat0 &table;
    hb_aat_apply_context_t *c;

    accelerator_t (अचर KerxSubTableFormat0 &table_,
		   hb_aat_apply_context_t *c_) :
		     table (table_), c (c_) {}

    पूर्णाङ्क get_kerning (hb_codepoint_t left, hb_codepoint_t दायाँ) अचर
    {
      यदि (!(*c->first_set)[left] || !(*c->second_set)[दायाँ]) फर्काऊ 0;
      फर्काऊ table.get_kerning (left, दायाँ, c);
    }
  };


  तार्किक sanitize (hb_sanitize_context_t *c) अचर
  {
    TRACE_SANITIZE (एतत्);
    return_trace (likely (pairs.sanitize (c)));
  }

  रक्षित:
  KernSubTableHeader	header;
  Bin_खोज_Array_Of<KernPair, typename KernSubTableHeader::Types::HBUINT>
			pairs;	/* Sorted kern records. */
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (KernSubTableHeader::static_size + 16, pairs);
};


ढाँचा <तार्किक extended>
संरचना Format1Entry;

ढाँचा <>
संरचना Format1Entry<सत्य>
{
  गणना Flags
  {
    थप		= 0x8000,	/* If set, push this glyph on the kerning stack. */
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

  स्थिर तार्किक initiateAction (अचर Entry<EntryData> &entry)
  { फर्काऊ entry.flags & थप; }

  स्थिर तार्किक performAction (अचर Entry<EntryData> &entry)
  { फर्काऊ entry.data.kernActionIndex != 0xFFFF; }

  स्थिर अचिन्हित पूर्णाङ्क kernActionIndex (अचर Entry<EntryData> &entry)
  { फर्काऊ entry.data.kernActionIndex; }
};
ढाँचा <>
संरचना Format1Entry<असत्य>
{
  गणना Flags
  {
    थप		= 0x8000,	/* If set, push this glyph on the kerning stack. */
    DontAdvance		= 0x4000,	/* If set, don't advance to the next glyph
					 * before going to the new state. */
    Offset		= 0x3FFF,	/* Byte offset from beginning of subtable to the
					 * value table for the glyphs on the kerning stack. */

    Reset		= 0x0000,	/* Not supported? */
  };

  प्रकारनिर्देश शून्य EntryData;

  स्थिर तार्किक initiateAction (अचर Entry<EntryData> &entry)
  { फर्काऊ entry.flags & थप; }

  स्थिर तार्किक performAction (अचर Entry<EntryData> &entry)
  { फर्काऊ entry.flags & Offset; }

  स्थिर अचिन्हित पूर्णाङ्क kernActionIndex (अचर Entry<EntryData> &entry)
  { फर्काऊ entry.flags & Offset; }
};

ढाँचा <typename KernSubTableHeader>
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

  तार्किक is_action_initiable (अचर Entry<EntryData> &entry) अचर
  {
    फर्काऊ Format1EntryT::initiateAction (entry);
  }
  तार्किक is_actionable (अचर Entry<EntryData> &entry) अचर
  {
    फर्काऊ Format1EntryT::performAction (entry);
  }

  संरचना driver_context_t
  {
    स्थिर constexpr तार्किक in_place = सत्य;

    driver_context_t (अचर KerxSubTableFormat1 *table_,
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
		     अचर Entry<EntryData> &entry)
    {
      अचिन्हित पूर्णाङ्क flags = entry.flags;

      यदि (flags & Format1EntryT::Reset)
	depth = 0;

      यदि (flags & Format1EntryT::थप)
      {
	यदि (likely (depth < ARRAY_LENGTH (थाक)))
	  थाक[depth++] = buffer->idx;
	अन्यथा
	  depth = 0; /* Probably not what CoreText does, but better? */
      }

      यदि (Format1EntryT::performAction (entry) && depth)
      {
	अचिन्हित पूर्णाङ्क tuple_गन्ती_2 = hb_max (1u, table->header.tuple_गन्ती_2 ());

	अचिन्हित पूर्णाङ्क kern_idx = Format1EntryT::kernActionIndex (entry);
	kern_idx = Types::byteOffsetToIndex (kern_idx, &table->machine, kernAction.arrayZ);
	अचर FWORD *actions = &kernAction[kern_idx];
	यदि (!c->sanitizer.check_array (actions, depth, tuple_गन्ती_2))
	{
	  depth = 0;
	  फर्काऊ;
	}
	hb_barrier ();

	hb_mask_t kern_mask = c->plan->kern_mask;

	/* From Apple 'kern' spec:
	 * "Each pops one glyph from the kerning stack and applies the kerning value to it.
	 * The end of the list is marked by an odd value... */
	तार्किक last = असत्य;
	जबसम्म (!last && depth)
	{
	  अचिन्हित पूर्णाङ्क idx = थाक[--depth];
	  पूर्णाङ्क v = *actions;
	  actions += tuple_गन्ती_2;
	  यदि (idx >= buffer->len) जारी;

	  /* "The end of the list is marked by an odd value..." */
	  last = v & 1;
	  v &= ~1;

	  hb_glyph_position_t &o = buffer->pos[idx];

	  यदि (HB_DIRECTION_IS_HORIZONTAL (buffer->props.direction))
	  {
	    यदि (crossStream)
	    {
	      /* The following flag is undocumented in the spec, but described
	       * in the 'kern' table example. */
	      यदि (v == -0x8000)
	      {
		o.attach_type() = OT::Layout::GPOS_impl::ATTACH_TYPE_NONE;
		o.attach_chain() = 0;
		o.y_offset = 0;
	      }
	      अन्यथा यदि (o.attach_type())
	      {
		o.y_offset = hb_saturate_add (o.y_offset, c->font->em_scale_y (v));
		buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT;
	      }
	    }
	    अन्यथा यदि (buffer->info[idx].mask & kern_mask)
	    {
	      स्वतः scaled = c->font->em_scale_x (v);
	      o.x_advance = hb_saturate_add (o.x_advance, scaled);
	      o.x_offset = hb_saturate_add (o.x_offset, scaled);
	    }
	  }
	  अन्यथा
	  {
	    यदि (crossStream)
	    {
	      /* CoreText doesn't do crossStream kerning in vertical.  We do. */
	      यदि (v == -0x8000)
	      {
		o.attach_type() = OT::Layout::GPOS_impl::ATTACH_TYPE_NONE;
		o.attach_chain() = 0;
		o.x_offset = 0;
	      }
	      अन्यथा यदि (o.attach_type())
	      {
		o.x_offset = hb_saturate_add (o.x_offset, c->font->em_scale_x (v));
		buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT;
	      }
	    }
	    अन्यथा यदि (buffer->info[idx].mask & kern_mask)
	    {
	      स्वतः scaled = c->font->em_scale_y (v);
	      o.y_advance = hb_saturate_add (o.y_advance, scaled);
	      o.y_offset = hb_saturate_add (o.y_offset, scaled);
	    }
	  }
	}
      }
    }

    सार्वजनिक:
    hb_aat_apply_context_t *c;
    अचर KerxSubTableFormat1 *table;
    गुप्त:
    अचर UnsizedArrayOf<FWORD> &kernAction;
    अचिन्हित पूर्णाङ्क थाक[8];
    अचिन्हित पूर्णाङ्क depth;
    तार्किक crossStream;
  };

  तार्किक apply (hb_aat_apply_context_t *c) अचर
  {
    TRACE_APPLY (एतत्);

    यदि (!c->plan->requested_kerning &&
	!(header.coverage & header.CrossStream))
      फर्काऊ असत्य;

    driver_context_t dc (एतत्, c);

    StateTableDriver<Types, EntryData, Flags> driver (machine, c->font->face);

    driver.drive (&dc, c);

    return_trace (सत्य);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) अचर
  {
    TRACE_SANITIZE (एतत्);
    /* The rest of array sanitizations are done at run-time. */
    return_trace (likely (c->check_struct (एतत्) &&
			  machine.sanitize (c)));
  }

  ढाँचा <typename set_t>
  शून्य collect_glyphs (set_t &first_set, set_t &second_set, अचिन्हित अङ्क_glyphs) अचर
  {
    machine.collect_initial_glyphs (first_set, अङ्क_glyphs, *एतत्);
    //machine.collect_glyphs (second_set, num_glyphs); // second_set is unused for machine kerning
  }

  रक्षित:
  KernSubTableHeader				header;
  StateTable<Types, EntryData>			machine;
  NNOffsetTo<UnsizedArrayOf<FWORD>, HBUINT>	kernAction;
  सार्वजनिक:
  DEFINE_SIZE_STATIC (KernSubTableHeader::static_size + (StateTable<Types, EntryData>::static_size + HBUINT::static_size));
};

ढाँचा <typename KernSubTableHeader>
संरचना KerxSubTableFormat2
{
  प्रकारनिर्देश typename KernSubTableHeader::Types Types;
  प्रकारनिर्देश typename Types::HBUINT HBUINT;

  पूर्णाङ्क get_kerning (hb_codepoint_t left, hb_codepoint_t दायाँ,
		   hb_aat_apply_context_t *c) अचर
  {
    अचिन्हित पूर्णाङ्क अङ्क_glyphs = c->sanitizer.get_अङ्क_glyphs ();
    अचिन्हित पूर्णाङ्क l = (एतत्+बायाँ_Class_Table).get_class (left, अङ्क_glyphs, 0);
    अचिन्हित पूर्णाङ्क r = (एतत्+दायाँ_Class_Table).get_class (दायाँ, अङ्क_glyphs, 0);

    अचर UnsizedArrayOf<FWORD> &arrayZ = एतत्+array;
    अचिन्हित पूर्णाङ्क kern_idx = l + r;
    kern_idx = Types::offsetToIndex (kern_idx, एतत्, arrayZ.arrayZ);
    अचर FWORD *v = &arrayZ[kern_idx];
    यदि (unlikely (!v->sanitize (&c->sanitizer))) फर्काऊ 0;
    hb_barrier ();

    फर्काऊ kerxTupleKern (*v, header.tuple_गन्ती_2 (), एतत्, c);
  }

  तार्किक apply (hb_aat_apply_context_t *c) अचर
  {
    TRACE_APPLY (एतत्);

    यदि (!c->plan->requested_kerning)
      return_trace (असत्य);

    यदि (header.coverage & header.Backwards)
      return_trace (असत्य);

    accelerator_t accel (*एतत्, c);
    hb_kern_machine_t<accelerator_t> machine (accel, header.coverage & header.CrossStream);
    machine.kern (c->font, c->buffer, c->plan->kern_mask);

    return_trace (सत्य);
  }

  ढाँचा <typename set_t>
  शून्य collect_glyphs (set_t &first_set, set_t &second_set, अचिन्हित अङ्क_glyphs) अचर
  {
    (एतत्+बायाँ_Class_Table).collect_glyphs (first_set, अङ्क_glyphs);
    (एतत्+दायाँ_Class_Table).collect_glyphs (second_set, अङ्क_glyphs);
  }

  संरचना accelerator_t
  {
    अचर KerxSubTableFormat2 &table;
    hb_aat_apply_context_t *c;

    accelerator_t (अचर KerxSubTableFormat2 &table_,
		   hb_aat_apply_context_t *c_) :
		     table (table_), c (c_) {}

    पूर्णाङ्क get_kerning (hb_codepoint_t left, hb_codepoint_t दायाँ) अचर
    {
      यदि (!(*c->first_set)[left] || !(*c->second_set)[दायाँ]) फर्काऊ 0;
      फर्काऊ table.get_kerning (left, दायाँ, c);
    }
  };

  तार्किक sanitize (hb_sanitize_context_t *c) अचर
  {
    TRACE_SANITIZE (एतत्);
    return_trace (likely (c->check_struct (एतत्) &&
			  बायाँ_Class_Table.sanitize (c, एतत्) &&
			  दायाँ_Class_Table.sanitize (c, एतत्) &&
			  hb_barrier () &&
			  c->check_range (एतत्, array)));
  }

  रक्षित:
  KernSubTableHeader	header;
  HBUINT		rowWidth;	/* The width, in bytes, of a row in the table. */
  NNOffsetTo<typename Types::ClassTypeWide, HBUINT>
			बायाँ_Class_Table;	/* Offset from beginning of this subtable to
					 * left-hand class table. */
  NNOffsetTo<typename Types::ClassTypeWide, HBUINT>
			दायाँ_Class_Table;/* Offset from beginning of this subtable to
					 * right-hand class table. */
  NNOffsetTo<UnsizedArrayOf<FWORD>, HBUINT>
			 array;		/* Offset from beginning of this subtable to
					 * the start of the kerning array. */
  सार्वजनिक:
  DEFINE_SIZE_STATIC (KernSubTableHeader::static_size + 4 * परिमाण (HBUINT));
};

ढाँचा <typename KernSubTableHeader>
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

  तार्किक is_action_initiable (अचर Entry<EntryData> &entry) अचर
  {
    फर्काऊ (entry.flags & Mark);
  }
  तार्किक is_actionable (अचर Entry<EntryData> &entry) अचर
  {
    फर्काऊ entry.data.ankrActionIndex != 0xFFFF;
  }

  संरचना driver_context_t
  {
    स्थिर constexpr तार्किक in_place = सत्य;
    गणना SubTableFlags
    {
      ActionType	= 0xC0000000,	/* A two-bit field containing the action type. */
      Unused		= 0x3F000000,	/* Unused - must be zero. */
      Offset		= 0x00FFFFFF,	/* Masks the offset in bytes from the beginning
					 * of the subtable to the beginning of the control
					 * point table. */
    };

    driver_context_t (अचर KerxSubTableFormat4 *table_,
		      hb_aat_apply_context_t *c_) :
	c (c_),
	table (table_),
	action_type ((table->flags & ActionType) >> 30),
	ankrData ((HBUINT16 *) ((अचर वर्ण *) &table->machine + (table->flags & Offset))),
	mark_set (असत्य),
	mark (0) {}

    HB_AAT_TRANSITION_INLINE
    शून्य transition (hb_buffer_t *buffer,
		     StateTableDriver<Types, EntryData, Flags> *driver,
		     अचर Entry<EntryData> &entry)
    {
      यदि (mark_set && entry.data.ankrActionIndex != 0xFFFF && buffer->idx < buffer->len)
      {
	hb_glyph_position_t &o = buffer->cur_pos();
	विकल्प (action_type)
	{
	  पक्ष 0: /* Control Point Actions.*/
	  {
	    /* Indexed into glyph outline. */
	    /* Each action (record in ankrData) contains two 16-bit fields, so we must
	       double the ankrActionIndex to get the correct offset here. */
	    अचर HBUINT16 *data = &ankrData[entry.data.ankrActionIndex * 2];
	    यदि (!c->sanitizer.check_array (data, 2)) फर्काऊ;
	    hb_barrier ();
	    अचिन्हित पूर्णाङ्क markControlPoint = *data++;
	    अचिन्हित पूर्णाङ्क currControlPoint = *data++;
	    hb_position_t markX = 0;
	    hb_position_t markY = 0;
	    hb_position_t currX = 0;
	    hb_position_t currY = 0;
	    यदि (!c->font->get_glyph_contour_point_for_origin (c->buffer->info[mark].codepoint,
							      markControlPoint,
							      HB_DIRECTION_LTR /*XXX*/,
							      &markX, &markY) ||
		!c->font->get_glyph_contour_point_for_origin (c->buffer->cur ().codepoint,
							      currControlPoint,
							      HB_DIRECTION_LTR /*XXX*/,
							      &currX, &currY))
	      फर्काऊ;

	    o.x_offset = hb_saturate_sub (markX, currX);
	    o.y_offset = hb_saturate_sub (markY, currY);
	  }
	  रोक;

	  पक्ष 1: /* Anchor Point Actions. */
	  {
	    /* Indexed into 'ankr' table. */
	    /* Each action (record in ankrData) contains two 16-bit fields, so we must
	       double the ankrActionIndex to get the correct offset here. */
	    अचर HBUINT16 *data = &ankrData[entry.data.ankrActionIndex * 2];
	    यदि (!c->sanitizer.check_array (data, 2)) फर्काऊ;
	    hb_barrier ();
	    अचिन्हित पूर्णाङ्क markAnchorPoint = *data++;
	    अचिन्हित पूर्णाङ्क currAnchorPoint = *data++;
	    अचर Anchor &markAnchor = c->ankr_table->get_anchor (c->buffer->info[mark].codepoint,
								  markAnchorPoint,
								  c->sanitizer.get_अङ्क_glyphs ());
	    अचर Anchor &currAnchor = c->ankr_table->get_anchor (c->buffer->cur ().codepoint,
								  currAnchorPoint,
								  c->sanitizer.get_अङ्क_glyphs ());

	    o.x_offset = hb_saturate_sub (c->font->em_scale_x (markAnchor.xCoordinate),
					  c->font->em_scale_x (currAnchor.xCoordinate));
	    o.y_offset = hb_saturate_sub (c->font->em_scale_y (markAnchor.yCoordinate),
					  c->font->em_scale_y (currAnchor.yCoordinate));
	  }
	  रोक;

	  पक्ष 2: /* Control Point Coordinate Actions. */
	  {
	    /* Each action contains four 16-bit fields, so we multiply the ankrActionIndex
	       by 4 to get the correct offset for the given action. */
	    अचर FWORD *data = (अचर FWORD *) &ankrData[entry.data.ankrActionIndex * 4];
	    यदि (!c->sanitizer.check_array (data, 4)) फर्काऊ;
	    hb_barrier ();
	    पूर्णाङ्क markX = *data++;
	    पूर्णाङ्क markY = *data++;
	    पूर्णाङ्क currX = *data++;
	    पूर्णाङ्क currY = *data++;

	    o.x_offset = hb_saturate_sub (c->font->em_scale_x (markX), c->font->em_scale_x (currX));
	    o.y_offset = hb_saturate_sub (c->font->em_scale_y (markY), c->font->em_scale_y (currY));
	  }
	  रोक;
	}
	o.attach_type() = OT::Layout::GPOS_impl::ATTACH_TYPE_MARK;
	o.attach_chain() = (पूर्णाङ्क) mark - (पूर्णाङ्क) buffer->idx;
	यदि (c->buffer_is_reversed)
	  o.attach_chain() = -o.attach_chain();
	buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT;
      }

      यदि (entry.flags & Mark)
      {
	mark_set = सत्य;
	mark = buffer->idx;
      }
    }

    सार्वजनिक:
    hb_aat_apply_context_t *c;
    अचर KerxSubTableFormat4 *table;
    गुप्त:
    अचिन्हित पूर्णाङ्क action_type;
    अचर HBUINT16 *ankrData;
    तार्किक mark_set;
    अचिन्हित पूर्णाङ्क mark;
  };

  तार्किक apply (hb_aat_apply_context_t *c) अचर
  {
    TRACE_APPLY (एतत्);

    driver_context_t dc (एतत्, c);

    StateTableDriver<Types, EntryData, Flags> driver (machine, c->font->face);

    driver.drive (&dc, c);

    return_trace (सत्य);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) अचर
  {
    TRACE_SANITIZE (एतत्);
    /* The rest of array sanitizations are done at run-time. */
    return_trace (likely (c->check_struct (एतत्) &&
			  machine.sanitize (c)));
  }

  ढाँचा <typename set_t>
  शून्य collect_glyphs (set_t &first_set, set_t &second_set, अचिन्हित अङ्क_glyphs) अचर
  {
    machine.collect_initial_glyphs (first_set, अङ्क_glyphs, *एतत्);
    //machine.collect_glyphs (second_set, num_glyphs); // second_set is unused for machine kerning
  }

  रक्षित:
  KernSubTableHeader		header;
  StateTable<Types, EntryData>	machine;
  HBUINT32			flags;
  सार्वजनिक:
  DEFINE_SIZE_STATIC (KernSubTableHeader::static_size + (StateTable<Types, EntryData>::static_size + HBUINT32::static_size));
};

ढाँचा <typename KernSubTableHeader>
संरचना KerxSubTableFormat6
{
  गणना Flags
  {
    मानहरू_Are_Long	= 0x00000001,
  };

  तार्किक is_long () अचर { फर्काऊ flags & मानहरू_Are_Long; }

  पूर्णाङ्क get_kerning (hb_codepoint_t left, hb_codepoint_t दायाँ,
		   hb_aat_apply_context_t *c) अचर
  {
    अचिन्हित पूर्णाङ्क अङ्क_glyphs = c->sanitizer.get_अङ्क_glyphs ();
    यदि (is_long ())
    {
      अचर स्वतः &t = u.l;
      अचिन्हित पूर्णाङ्क l = (एतत्+t.rowIndexTable).get_मान_or_null (left, अङ्क_glyphs);
      अचिन्हित पूर्णाङ्क r = (एतत्+t.columnIndexTable).get_मान_or_null (दायाँ, अङ्क_glyphs);
      अचिन्हित पूर्णाङ्क offset = l + r;
      यदि (unlikely (offset < l)) फर्काऊ 0; /* Addition overflow. */
      यदि (unlikely (hb_unsigned_mul_overflows (offset, परिमाण (FWORD32)))) फर्काऊ 0;
      अचर FWORD32 *v = &StructAtOffset<FWORD32> (&(एतत्+t.array), offset * परिमाण (FWORD32));
      यदि (unlikely (!v->sanitize (&c->sanitizer))) फर्काऊ 0;
      hb_barrier ();
      फर्काऊ kerxTupleKern (*v, header.tuple_गन्ती_2 (), &(एतत्+vector), c);
    }
    अन्यथा
    {
      अचर स्वतः &t = u.s;
      अचिन्हित पूर्णाङ्क l = (एतत्+t.rowIndexTable).get_मान_or_null (left, अङ्क_glyphs);
      अचिन्हित पूर्णाङ्क r = (एतत्+t.columnIndexTable).get_मान_or_null (दायाँ, अङ्क_glyphs);
      अचिन्हित पूर्णाङ्क offset = l + r;
      अचर FWORD *v = &StructAtOffset<FWORD> (&(एतत्+t.array), offset * परिमाण (FWORD));
      यदि (unlikely (!v->sanitize (&c->sanitizer))) फर्काऊ 0;
      hb_barrier ();
      फर्काऊ kerxTupleKern (*v, header.tuple_गन्ती_2 (), &(एतत्+vector), c);
    }
  }

  तार्किक apply (hb_aat_apply_context_t *c) अचर
  {
    TRACE_APPLY (एतत्);

    यदि (!c->plan->requested_kerning)
      return_trace (असत्य);

    यदि (header.coverage & header.Backwards)
      return_trace (असत्य);

    accelerator_t accel (*एतत्, c);
    hb_kern_machine_t<accelerator_t> machine (accel, header.coverage & header.CrossStream);
    machine.kern (c->font, c->buffer, c->plan->kern_mask);

    return_trace (सत्य);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) अचर
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
			  (header.tuple_गन्ती_2 () == 0 ||
			   c->check_range (एतत्, vector))));
  }

  ढाँचा <typename set_t>
  शून्य collect_glyphs (set_t &first_set, set_t &second_set, अचिन्हित अङ्क_glyphs) अचर
  {
    यदि (is_long ())
    {
      अचर स्वतः &t = u.l;
      (एतत्+t.rowIndexTable).collect_glyphs (first_set, अङ्क_glyphs);
      (एतत्+t.columnIndexTable).collect_glyphs (second_set, अङ्क_glyphs);
    }
    अन्यथा
    {
      अचर स्वतः &t = u.s;
      (एतत्+t.rowIndexTable).collect_glyphs (first_set, अङ्क_glyphs);
      (एतत्+t.columnIndexTable).collect_glyphs (second_set, अङ्क_glyphs);
    }
  }

  संरचना accelerator_t
  {
    अचर KerxSubTableFormat6 &table;
    hb_aat_apply_context_t *c;

    accelerator_t (अचर KerxSubTableFormat6 &table_,
		   hb_aat_apply_context_t *c_) :
		     table (table_), c (c_) {}

    पूर्णाङ्क get_kerning (hb_codepoint_t left, hb_codepoint_t दायाँ) अचर
    {
      यदि (!(*c->first_set)[left] || !(*c->second_set)[दायाँ]) फर्काऊ 0;
      फर्काऊ table.get_kerning (left, दायाँ, c);
    }
  };

  रक्षित:
  KernSubTableHeader		header;
  HBUINT32			flags;
  HBUINT16			row_गन्ती;
  HBUINT16			column_गन्ती;
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

  अचिन्हित   tuple_गन्ती_2 () अचर { फर्काऊ tuple_गन्ती; }
  तार्किक     is_horizontal () अचर { फर्काऊ !(coverage & Vertical); }

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

  तार्किक sanitize (hb_sanitize_context_t *c) अचर
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्));
  }

  सार्वजनिक:
  HBUINT32	length;
  HBUINT32	coverage;
  HBUINT32	tuple_गन्ती;
  सार्वजनिक:
  DEFINE_SIZE_STATIC (12);
};

संरचना KerxSubTable
{
  मित्र संरचना kerx;

  माप_प्रकार get_size () अचर { फर्काऊ u.header.length; }
  अचिन्हित पूर्णाङ्क get_type () अचर { फर्काऊ u.header.coverage & u.header.SubtableType; }

  ढाँचा <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) अचर
  {
    अचिन्हित पूर्णाङ्क subtable_type = get_type ();
    TRACE_DISPATCH (एतत्, subtable_type);
    विकल्प (subtable_type) {
    पक्ष 0:	hb_barrier (); return_trace (c->dispatch (u.format0, std::forward<Ts> (ds)...));
    पक्ष 1:	hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    पक्ष 2:	hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    पक्ष 4:	hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
    पक्ष 6:	hb_barrier (); return_trace (c->dispatch (u.format6, std::forward<Ts> (ds)...));
    स्वभाव:	return_trace (c->default_return_मान ());
    }
  }

  ढाँचा <typename set_t>
  शून्य collect_glyphs (set_t &first_set, set_t &second_set, अचिन्हित अङ्क_glyphs) अचर
  {
    अचिन्हित पूर्णाङ्क subtable_type = get_type ();
    विकल्प (subtable_type) {
    पक्ष 0:	hb_barrier (); u.format0.collect_glyphs (first_set, second_set, अङ्क_glyphs); फर्काऊ;
    पक्ष 1:	hb_barrier (); u.format1.collect_glyphs (first_set, second_set, अङ्क_glyphs); फर्काऊ;
    पक्ष 2:	hb_barrier (); u.format2.collect_glyphs (first_set, second_set, अङ्क_glyphs); फर्काऊ;
    पक्ष 4:	hb_barrier (); u.format4.collect_glyphs (first_set, second_set, अङ्क_glyphs); फर्काऊ;
    पक्ष 6:	hb_barrier (); u.format6.collect_glyphs (first_set, second_set, अङ्क_glyphs); फर्काऊ;
    स्वभाव:	फर्काऊ;
    }
  }

  तार्किक sanitize (hb_sanitize_context_t *c) अचर
  {
    TRACE_SANITIZE (एतत्);
    यदि (!(u.header.sanitize (c) &&
	  hb_barrier () &&
	  u.header.length >= u.header.static_size &&
	  c->check_range (एतत्, u.header.length)))
      return_trace (असत्य);

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

ढाँचा <typename T>
संरचना KerxTable
{
  /* https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern */
  अचर T* thiz () अचर { फर्काऊ static_cast<अचर T *> (एतत्); }

  तार्किक has_state_machine () अचर
  {
    प्रकारनिर्देश typename T::SubTable SubTable;

    अचर SubTable *st = &thiz()->firstSubTable;
    अचिन्हित पूर्णाङ्क गन्ती = thiz()->table_गन्ती;
    लागि (अचिन्हित पूर्णाङ्क i = 0; i < गन्ती; i++)
    {
      यदि (st->get_type () == 1)
	फर्काऊ सत्य;

      // TODO: What about format 4? What's this API used for anyway?

      st = &StructAfter<SubTable> (*st);
    }
    फर्काऊ असत्य;
  }

  तार्किक has_cross_stream () अचर
  {
    प्रकारनिर्देश typename T::SubTable SubTable;

    अचर SubTable *st = &thiz()->firstSubTable;
    अचिन्हित पूर्णाङ्क गन्ती = thiz()->table_गन्ती;
    लागि (अचिन्हित पूर्णाङ्क i = 0; i < गन्ती; i++)
    {
      यदि (st->u.header.coverage & st->u.header.CrossStream)
	फर्काऊ सत्य;
      st = &StructAfter<SubTable> (*st);
    }
    फर्काऊ असत्य;
  }

  पूर्णाङ्क get_h_kerning (hb_codepoint_t left, hb_codepoint_t दायाँ) अचर
  {
    प्रकारनिर्देश typename T::SubTable SubTable;

    पूर्णाङ्क v = 0;
    अचर SubTable *st = &thiz()->firstSubTable;
    अचिन्हित पूर्णाङ्क गन्ती = thiz()->table_गन्ती;
    लागि (अचिन्हित पूर्णाङ्क i = 0; i < गन्ती; i++)
    {
      यदि ((st->u.header.coverage & (st->u.header.Variation | st->u.header.CrossStream)) ||
	  !st->u.header.is_horizontal ())
	जारी;
      v += st->get_kerning (left, दायाँ);
      st = &StructAfter<SubTable> (*st);
    }
    फर्काऊ v;
  }

  तार्किक apply (AAT::hb_aat_apply_context_t *c,
	      अचर kern_accelerator_data_t &accel_data) अचर
  {
    c->buffer->unsafe_to_concat ();

    c->setup_buffer_glyph_set ();

    प्रकारनिर्देश typename T::SubTable SubTable;

    तार्किक ret = असत्य;
    तार्किक seenCrossStream = असत्य;
    c->set_lookup_index (0);
    अचर SubTable *st = &thiz()->firstSubTable;
    अचिन्हित पूर्णाङ्क गन्ती = thiz()->table_गन्ती;
    लागि (अचिन्हित पूर्णाङ्क i = 0; i < गन्ती; i++)
    {
      तार्किक reverse;

      स्वतः &subtable_accel = accel_data.subtable_accels[i];

      यदि (!T::Types::extended && (st->u.header.coverage & st->u.header.Variation))
	गच्छ skip;

      यदि (HB_DIRECTION_IS_HORIZONTAL (c->buffer->props.direction) != st->u.header.is_horizontal ())
	गच्छ skip;

      c->first_set = &subtable_accel.first_set;
      c->second_set = &subtable_accel.second_set;
      c->machine_class_cache = &subtable_accel.class_cache;

      यदि (!c->buffer_intersects_machine ())
      {
	(शून्य) c->buffer->message (c->font, "skipped subtable %u because no glyph matches", c->lookup_index);
	गच्छ skip;
      }

      reverse = तार्किक (st->u.header.coverage & st->u.header.Backwards) !=
		HB_DIRECTION_IS_BACKWARD (c->buffer->props.direction);

      यदि (!c->buffer->message (c->font, "start subtable %u", c->lookup_index))
	गच्छ skip;

      यदि (!seenCrossStream &&
	  (st->u.header.coverage & st->u.header.CrossStream))
      {
	/* Attach all glyphs into a chain. */
	seenCrossStream = सत्य;
	hb_glyph_position_t *pos = c->buffer->pos;
	अचिन्हित पूर्णाङ्क गन्ती = c->buffer->len;
	लागि (अचिन्हित पूर्णाङ्क i = 0; i < गन्ती; i++)
	{
	  pos[i].attach_type() = OT::Layout::GPOS_impl::ATTACH_TYPE_CURSIVE;
	  pos[i].attach_chain() = HB_DIRECTION_IS_FORWARD (c->buffer->props.direction) ? -1 : +1;
	  /* We intentionally don't set HB_BUFFER_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT,
	   * since there needs to be a non-zero attachment for post-positioning to
	   * be needed. */
	}
      }

      यदि (reverse != c->buffer_is_reversed)
        c->reverse_buffer ();

      {
	/* See comment in sanitize() for conditional here. */
	hb_sanitize_with_object_t with (&c->sanitizer, i < गन्ती - 1 ? st : (अचर SubTable *) nullptr);
	ret |= st->dispatch (c);
      }

      (शून्य) c->buffer->message (c->font, "end subtable %u", c->lookup_index);

    skip:
      st = &StructAfter<SubTable> (*st);
      c->set_lookup_index (c->lookup_index + 1);
    }
    यदि (c->buffer_is_reversed)
      c->reverse_buffer ();

    फर्काऊ ret;
  }

  तार्किक sanitize (hb_sanitize_context_t *c) अचर
  {
    TRACE_SANITIZE (एतत्);
    यदि (unlikely (!(thiz()->version.sanitize (c) &&
		    hb_barrier () &&
		    (अचिन्हित) thiz()->version >= (अचिन्हित) T::minVersion &&
		    thiz()->table_गन्ती.sanitize (c))))
      return_trace (असत्य);

    प्रकारनिर्देश typename T::SubTable SubTable;

    अचर SubTable *st = &thiz()->firstSubTable;
    अचिन्हित पूर्णाङ्क गन्ती = thiz()->table_गन्ती;
    लागि (अचिन्हित पूर्णाङ्क i = 0; i < गन्ती; i++)
    {
      यदि (unlikely (!st->u.header.sanitize (c)))
	return_trace (असत्य);
      hb_barrier ();
      /* OpenType kern table has 2-byte subtable lengths.  That's limiting.
       * MS implementation also only supports one subtable, of format 0,
       * anyway.  Certain versions of some fonts, like Calibry, contain
       * kern subtable that exceeds 64kb.  Looks like, the subtable length
       * is simply ignored.  Which makes sense.  It's only needed if you
       * have multiple subtables.  To handle such fonts, we just ignore
       * the length for the last subtable. */
      hb_sanitize_with_object_t with (c, i < गन्ती - 1 ? st : (अचर SubTable *) nullptr);

      यदि (unlikely (!st->sanitize (c)))
	return_trace (असत्य);

      st = &StructAfter<SubTable> (*st);
    }

    अचिन्हित majorVersion = thiz()->version;
    यदि (परिमाण (thiz()->version) == 4)
      majorVersion = majorVersion >> 16;
    यदि (majorVersion >= 3)
    {
      अचर SubtableGlyphCoverage *coverage = (अचर SubtableGlyphCoverage *) st;
      यदि (!coverage->sanitize (c, गन्ती))
        return_trace (असत्य);
    }

    return_trace (सत्य);
  }

  kern_accelerator_data_t create_accelerator_data (अचिन्हित अङ्क_glyphs) अचर
  {
    kern_accelerator_data_t accel_data;

    प्रकारनिर्देश typename T::SubTable SubTable;

    अचर SubTable *st = &thiz()->firstSubTable;
    अचिन्हित पूर्णाङ्क गन्ती = thiz()->table_गन्ती;
    लागि (अचिन्हित पूर्णाङ्क i = 0; i < गन्ती; i++)
    {
      स्वतः &subtable_accel = *accel_data.subtable_accels.push ();
      यदि (unlikely (accel_data.subtable_accels.in_error ()))
	  फर्काऊ accel_data;

      st->collect_glyphs (subtable_accel.first_set, subtable_accel.second_set, अङ्क_glyphs);
      subtable_accel.class_cache.clear ();

      st = &StructAfter<SubTable> (*st);
    }

    फर्काऊ accel_data;
  }

  संरचना accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      hb_sanitize_context_t sc;
      एतत्->table = sc.reference_table<T> (face);
      एतत्->accel_data = एतत्->table->create_accelerator_data (face->get_अङ्क_glyphs ());
    }
    ~accelerator_t ()
    {
      एतत्->table.destroy ();
    }

    hb_blob_t *get_blob () अचर { फर्काऊ table.get_blob (); }

    तार्किक apply (AAT::hb_aat_apply_context_t *c) अचर
    {
      फर्काऊ table->apply (c, accel_data);
    }

    hb_blob_ptr_t<T> table;
    kern_accelerator_data_t accel_data;
    hb_aat_scratch_t scratch;
  };
};

संरचना kerx : KerxTable<kerx>
{
  मित्र संरचना KerxTable<kerx>;

  स्थिर constexpr hb_tag_t tableTag = HB_AAT_TAG_kerx;
  स्थिर constexpr अचिन्हित minVersion = 2u;

  प्रकारनिर्देश KerxSubTableHeader SubTableHeader;
  प्रकारनिर्देश SubTableHeader::Types Types;
  प्रकारनिर्देश KerxSubTable SubTable;

  तार्किक has_data () अचर { फर्काऊ version; }

  रक्षित:
  HBUINT16	version;	/* The version number of the extended kerning table
				 * (currently 2, 3, or 4). */
  HBUINT16	unused;		/* Set to 0. */
  HBUINT32	table_गन्ती;	/* The number of subtables included in the extended kerning
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
