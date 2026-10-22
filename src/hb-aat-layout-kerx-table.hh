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


स्थिर ओळीत पूर्णांक
kerxTupleKern (पूर्णांक value,
	       अचिन्हित पूर्णांक tuple_मोजणी,
	       स्थिरांक शून्य *base,
	       hb_aat_apply_context_t *c)
{
  जर (likely (!tuple_मोजणी || !c)) परत value;

  अचिन्हित पूर्णांक offset = value;
  स्थिरांक FWORD *pv = &StructAtOffset<FWORD> (base, offset);
  जर (unlikely (!c->sanitizer.check_array (pv, tuple_मोजणी))) परत 0;
  hb_barrier ();
  परत *pv;
}


संरचना hb_glyph_pair_t
{
  hb_codepoint_t left;
  hb_codepoint_t उजवा;
};

संरचना KernPair
{
  पूर्णांक get_kerning () स्थिरांक { परत value; }

  पूर्णांक cmp (स्थिरांक hb_glyph_pair_t &o) स्थिरांक
  {
    पूर्णांक ret = left.cmp (o.left);
    जर (ret) परत ret;
    परत उजवा.cmp (o.उजवा);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे));
  }

  सार्वजनिक:
  HBGlyphID16	left;
  HBGlyphID16	उजवा;
  FWORD		value;
  सार्वजनिक:
  DEFINE_SIZE_STATIC (6);
};

ढाँचा <typename KernSubTableHeader>
संरचना KerxSubTableFormat0
{
  पूर्णांक get_kerning (hb_codepoint_t left, hb_codepoint_t उजवा,
		   hb_aat_apply_context_t *c = nullptr) स्थिरांक
  {
    hb_glyph_pair_t pair = {left, उजवा};
    पूर्णांक v = pairs.द्वा_खोज (pair).get_kerning ();
    परत kerxTupleKern (v, header.tuple_मोजणी_2 (), हे, c);
  }

  तार्किक apply (hb_aat_apply_context_t *c) स्थिरांक
  {
    TRACE_APPLY (हे);

    जर (!c->plan->requested_kerning)
      return_trace (असत्य);

    जर (header.coverage & header.Backwards)
      return_trace (असत्य);

    accelerator_t accel (*हे, c);
    hb_kern_machine_t<accelerator_t> machine (accel, header.coverage & header.CrossStream);
    machine.kern (c->font, c->buffer, c->plan->kern_mask);

    return_trace (सत्य);
  }

  ढाँचा <typename set_t>
  शून्य collect_glyphs (set_t &first_set, set_t &second_set, अचिन्हित अंक_glyphs) स्थिरांक
  {
    साठी (स्थिरांक KernPair& pair : pairs)
    {
      first_set.add (pair.left);
      second_set.add (pair.उजवा);
    }
  }

  संरचना accelerator_t
  {
    स्थिरांक KerxSubTableFormat0 &table;
    hb_aat_apply_context_t *c;

    accelerator_t (स्थिरांक KerxSubTableFormat0 &table_,
		   hb_aat_apply_context_t *c_) :
		     table (table_), c (c_) {}

    पूर्णांक get_kerning (hb_codepoint_t left, hb_codepoint_t उजवा) स्थिरांक
    {
      जर (!(*c->first_set)[left] || !(*c->second_set)[उजवा]) परत 0;
      परत table.get_kerning (left, उजवा, c);
    }
  };


  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (likely (pairs.sanitize (c)));
  }

  संरक्षित:
  KernSubTableHeader	header;
  Bin_शोध_Array_Of<KernPair, typename KernSubTableHeader::Types::HBUINT>
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
    ढकला		= 0x8000,	/* If set, push this glyph on the kerning stack. */
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

  स्थिर तार्किक initiateAction (स्थिरांक Entry<EntryData> &entry)
  { परत entry.flags & ढकला; }

  स्थिर तार्किक performAction (स्थिरांक Entry<EntryData> &entry)
  { परत entry.data.kernActionIndex != 0xFFFF; }

  स्थिर अचिन्हित पूर्णांक kernActionIndex (स्थिरांक Entry<EntryData> &entry)
  { परत entry.data.kernActionIndex; }
};
ढाँचा <>
संरचना Format1Entry<असत्य>
{
  गणना Flags
  {
    ढकला		= 0x8000,	/* If set, push this glyph on the kerning stack. */
    DontAdvance		= 0x4000,	/* If set, don't advance to the next glyph
					 * before going to the new state. */
    Offset		= 0x3FFF,	/* Byte offset from beginning of subtable to the
					 * value table for the glyphs on the kerning stack. */

    Reset		= 0x0000,	/* Not supported? */
  };

  प्रकारव्याख्या शून्य EntryData;

  स्थिर तार्किक initiateAction (स्थिरांक Entry<EntryData> &entry)
  { परत entry.flags & ढकला; }

  स्थिर तार्किक performAction (स्थिरांक Entry<EntryData> &entry)
  { परत entry.flags & Offset; }

  स्थिर अचिन्हित पूर्णांक kernActionIndex (स्थिरांक Entry<EntryData> &entry)
  { परत entry.flags & Offset; }
};

ढाँचा <typename KernSubTableHeader>
संरचना KerxSubTableFormat1
{
  प्रकारव्याख्या typename KernSubTableHeader::Types Types;
  प्रकारव्याख्या typename Types::HBUINT HBUINT;

  प्रकारव्याख्या Format1Entry<Types::extended> Format1EntryT;
  प्रकारव्याख्या typename Format1EntryT::EntryData EntryData;

  गणना Flags
  {
    DontAdvance	= Format1EntryT::DontAdvance,
  };

  तार्किक is_action_initiable (स्थिरांक Entry<EntryData> &entry) स्थिरांक
  {
    परत Format1EntryT::initiateAction (entry);
  }
  तार्किक is_actionable (स्थिरांक Entry<EntryData> &entry) स्थिरांक
  {
    परत Format1EntryT::performAction (entry);
  }

  संरचना driver_context_t
  {
    स्थिर constexpr तार्किक in_place = सत्य;

    driver_context_t (स्थिरांक KerxSubTableFormat1 *table_,
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
		     स्थिरांक Entry<EntryData> &entry)
    {
      अचिन्हित पूर्णांक flags = entry.flags;

      जर (flags & Format1EntryT::Reset)
	depth = 0;

      जर (flags & Format1EntryT::ढकला)
      {
	जर (likely (depth < ARRAY_LENGTH (चळत)))
	  चळत[depth++] = buffer->idx;
	अन्यथा
	  depth = 0; /* Probably not what CoreText does, but better? */
      }

      जर (Format1EntryT::performAction (entry) && depth)
      {
	अचिन्हित पूर्णांक tuple_मोजणी_2 = hb_max (1u, table->header.tuple_मोजणी_2 ());

	अचिन्हित पूर्णांक kern_idx = Format1EntryT::kernActionIndex (entry);
	kern_idx = Types::byteOffsetToIndex (kern_idx, &table->machine, kernAction.arrayZ);
	स्थिरांक FWORD *actions = &kernAction[kern_idx];
	जर (!c->sanitizer.check_array (actions, depth, tuple_मोजणी_2))
	{
	  depth = 0;
	  परत;
	}
	hb_barrier ();

	hb_mask_t kern_mask = c->plan->kern_mask;

	/* From Apple 'kern' spec:
	 * "Each pops one glyph from the kerning stack and applies the kerning value to it.
	 * The end of the list is marked by an odd value... */
	तार्किक last = असत्य;
	पर्यंत (!last && depth)
	{
	  अचिन्हित पूर्णांक idx = चळत[--depth];
	  पूर्णांक v = *actions;
	  actions += tuple_मोजणी_2;
	  जर (idx >= buffer->len) सुरूठेवा;

	  /* "The end of the list is marked by an odd value..." */
	  last = v & 1;
	  v &= ~1;

	  hb_glyph_position_t &o = buffer->pos[idx];

	  जर (HB_DIRECTION_IS_HORIZONTAL (buffer->props.direction))
	  {
	    जर (crossStream)
	    {
	      /* The following flag is undocumented in the spec, but described
	       * in the 'kern' table example. */
	      जर (v == -0x8000)
	      {
		o.attach_type() = OT::Layout::GPOS_impl::ATTACH_TYPE_NONE;
		o.attach_chain() = 0;
		o.y_offset = 0;
	      }
	      अन्यथा जर (o.attach_type())
	      {
		o.y_offset = hb_saturate_add (o.y_offset, c->font->em_scale_y (v));
		buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT;
	      }
	    }
	    अन्यथा जर (buffer->info[idx].mask & kern_mask)
	    {
	      स्वतः scaled = c->font->em_scale_x (v);
	      o.x_advance = hb_saturate_add (o.x_advance, scaled);
	      o.x_offset = hb_saturate_add (o.x_offset, scaled);
	    }
	  }
	  अन्यथा
	  {
	    जर (crossStream)
	    {
	      /* CoreText doesn't do crossStream kerning in vertical.  We do. */
	      जर (v == -0x8000)
	      {
		o.attach_type() = OT::Layout::GPOS_impl::ATTACH_TYPE_NONE;
		o.attach_chain() = 0;
		o.x_offset = 0;
	      }
	      अन्यथा जर (o.attach_type())
	      {
		o.x_offset = hb_saturate_add (o.x_offset, c->font->em_scale_x (v));
		buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT;
	      }
	    }
	    अन्यथा जर (buffer->info[idx].mask & kern_mask)
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
    स्थिरांक KerxSubTableFormat1 *table;
    खाजगी:
    स्थिरांक UnsizedArrayOf<FWORD> &kernAction;
    अचिन्हित पूर्णांक चळत[8];
    अचिन्हित पूर्णांक depth;
    तार्किक crossStream;
  };

  तार्किक apply (hb_aat_apply_context_t *c) स्थिरांक
  {
    TRACE_APPLY (हे);

    जर (!c->plan->requested_kerning &&
	!(header.coverage & header.CrossStream))
      परत असत्य;

    driver_context_t dc (हे, c);

    StateTableDriver<Types, EntryData, Flags> driver (machine, c->font->face);

    driver.drive (&dc, c);

    return_trace (सत्य);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    /* The rest of array sanitizations are done at run-time. */
    return_trace (likely (c->check_struct (हे) &&
			  machine.sanitize (c)));
  }

  ढाँचा <typename set_t>
  शून्य collect_glyphs (set_t &first_set, set_t &second_set, अचिन्हित अंक_glyphs) स्थिरांक
  {
    machine.collect_initial_glyphs (first_set, अंक_glyphs, *हे);
    //machine.collect_glyphs (second_set, num_glyphs); // second_set is unused for machine kerning
  }

  संरक्षित:
  KernSubTableHeader				header;
  StateTable<Types, EntryData>			machine;
  NNOffsetTo<UnsizedArrayOf<FWORD>, HBUINT>	kernAction;
  सार्वजनिक:
  DEFINE_SIZE_STATIC (KernSubTableHeader::static_size + (StateTable<Types, EntryData>::static_size + HBUINT::static_size));
};

ढाँचा <typename KernSubTableHeader>
संरचना KerxSubTableFormat2
{
  प्रकारव्याख्या typename KernSubTableHeader::Types Types;
  प्रकारव्याख्या typename Types::HBUINT HBUINT;

  पूर्णांक get_kerning (hb_codepoint_t left, hb_codepoint_t उजवा,
		   hb_aat_apply_context_t *c) स्थिरांक
  {
    अचिन्हित पूर्णांक अंक_glyphs = c->sanitizer.get_अंक_glyphs ();
    अचिन्हित पूर्णांक l = (हे+डावा_Class_Table).get_class (left, अंक_glyphs, 0);
    अचिन्हित पूर्णांक r = (हे+उजवा_Class_Table).get_class (उजवा, अंक_glyphs, 0);

    स्थिरांक UnsizedArrayOf<FWORD> &arrayZ = हे+array;
    अचिन्हित पूर्णांक kern_idx = l + r;
    kern_idx = Types::offsetToIndex (kern_idx, हे, arrayZ.arrayZ);
    स्थिरांक FWORD *v = &arrayZ[kern_idx];
    जर (unlikely (!v->sanitize (&c->sanitizer))) परत 0;
    hb_barrier ();

    परत kerxTupleKern (*v, header.tuple_मोजणी_2 (), हे, c);
  }

  तार्किक apply (hb_aat_apply_context_t *c) स्थिरांक
  {
    TRACE_APPLY (हे);

    जर (!c->plan->requested_kerning)
      return_trace (असत्य);

    जर (header.coverage & header.Backwards)
      return_trace (असत्य);

    accelerator_t accel (*हे, c);
    hb_kern_machine_t<accelerator_t> machine (accel, header.coverage & header.CrossStream);
    machine.kern (c->font, c->buffer, c->plan->kern_mask);

    return_trace (सत्य);
  }

  ढाँचा <typename set_t>
  शून्य collect_glyphs (set_t &first_set, set_t &second_set, अचिन्हित अंक_glyphs) स्थिरांक
  {
    (हे+डावा_Class_Table).collect_glyphs (first_set, अंक_glyphs);
    (हे+उजवा_Class_Table).collect_glyphs (second_set, अंक_glyphs);
  }

  संरचना accelerator_t
  {
    स्थिरांक KerxSubTableFormat2 &table;
    hb_aat_apply_context_t *c;

    accelerator_t (स्थिरांक KerxSubTableFormat2 &table_,
		   hb_aat_apply_context_t *c_) :
		     table (table_), c (c_) {}

    पूर्णांक get_kerning (hb_codepoint_t left, hb_codepoint_t उजवा) स्थिरांक
    {
      जर (!(*c->first_set)[left] || !(*c->second_set)[उजवा]) परत 0;
      परत table.get_kerning (left, उजवा, c);
    }
  };

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (likely (c->check_struct (हे) &&
			  डावा_Class_Table.sanitize (c, हे) &&
			  उजवा_Class_Table.sanitize (c, हे) &&
			  hb_barrier () &&
			  c->check_range (हे, array)));
  }

  संरक्षित:
  KernSubTableHeader	header;
  HBUINT		rowWidth;	/* The width, in bytes, of a row in the table. */
  NNOffsetTo<typename Types::ClassTypeWide, HBUINT>
			डावा_Class_Table;	/* Offset from beginning of this subtable to
					 * left-hand class table. */
  NNOffsetTo<typename Types::ClassTypeWide, HBUINT>
			उजवा_Class_Table;/* Offset from beginning of this subtable to
					 * right-hand class table. */
  NNOffsetTo<UnsizedArrayOf<FWORD>, HBUINT>
			 array;		/* Offset from beginning of this subtable to
					 * the start of the kerning array. */
  सार्वजनिक:
  DEFINE_SIZE_STATIC (KernSubTableHeader::static_size + 4 * आकार (HBUINT));
};

ढाँचा <typename KernSubTableHeader>
संरचना KerxSubTableFormat4
{
  प्रकारव्याख्या ExtendedTypes Types;

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

  तार्किक is_action_initiable (स्थिरांक Entry<EntryData> &entry) स्थिरांक
  {
    परत (entry.flags & Mark);
  }
  तार्किक is_actionable (स्थिरांक Entry<EntryData> &entry) स्थिरांक
  {
    परत entry.data.ankrActionIndex != 0xFFFF;
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

    driver_context_t (स्थिरांक KerxSubTableFormat4 *table_,
		      hb_aat_apply_context_t *c_) :
	c (c_),
	table (table_),
	action_type ((table->flags & ActionType) >> 30),
	ankrData ((HBUINT16 *) ((स्थिरांक अक्षर *) &table->machine + (table->flags & Offset))),
	mark_set (असत्य),
	mark (0) {}

    HB_AAT_TRANSITION_INLINE
    शून्य transition (hb_buffer_t *buffer,
		     StateTableDriver<Types, EntryData, Flags> *driver,
		     स्थिरांक Entry<EntryData> &entry)
    {
      जर (mark_set && entry.data.ankrActionIndex != 0xFFFF && buffer->idx < buffer->len)
      {
	hb_glyph_position_t &o = buffer->cur_pos();
	निवड (action_type)
	{
	  स्थिती 0: /* Control Point Actions.*/
	  {
	    /* Indexed into glyph outline. */
	    /* Each action (record in ankrData) contains two 16-bit fields, so we must
	       double the ankrActionIndex to get the correct offset here. */
	    स्थिरांक HBUINT16 *data = &ankrData[entry.data.ankrActionIndex * 2];
	    जर (!c->sanitizer.check_array (data, 2)) परत;
	    hb_barrier ();
	    अचिन्हित पूर्णांक markControlPoint = *data++;
	    अचिन्हित पूर्णांक currControlPoint = *data++;
	    hb_position_t markX = 0;
	    hb_position_t markY = 0;
	    hb_position_t currX = 0;
	    hb_position_t currY = 0;
	    जर (!c->font->get_glyph_contour_point_for_origin (c->buffer->info[mark].codepoint,
							      markControlPoint,
							      HB_DIRECTION_LTR /*XXX*/,
							      &markX, &markY) ||
		!c->font->get_glyph_contour_point_for_origin (c->buffer->cur ().codepoint,
							      currControlPoint,
							      HB_DIRECTION_LTR /*XXX*/,
							      &currX, &currY))
	      परत;

	    o.x_offset = hb_saturate_sub (markX, currX);
	    o.y_offset = hb_saturate_sub (markY, currY);
	  }
	  थांबा;

	  स्थिती 1: /* Anchor Point Actions. */
	  {
	    /* Indexed into 'ankr' table. */
	    /* Each action (record in ankrData) contains two 16-bit fields, so we must
	       double the ankrActionIndex to get the correct offset here. */
	    स्थिरांक HBUINT16 *data = &ankrData[entry.data.ankrActionIndex * 2];
	    जर (!c->sanitizer.check_array (data, 2)) परत;
	    hb_barrier ();
	    अचिन्हित पूर्णांक markAnchorPoint = *data++;
	    अचिन्हित पूर्णांक currAnchorPoint = *data++;
	    स्थिरांक Anchor &markAnchor = c->ankr_table->get_anchor (c->buffer->info[mark].codepoint,
								  markAnchorPoint,
								  c->sanitizer.get_अंक_glyphs ());
	    स्थिरांक Anchor &currAnchor = c->ankr_table->get_anchor (c->buffer->cur ().codepoint,
								  currAnchorPoint,
								  c->sanitizer.get_अंक_glyphs ());

	    o.x_offset = hb_saturate_sub (c->font->em_scale_x (markAnchor.xCoordinate),
					  c->font->em_scale_x (currAnchor.xCoordinate));
	    o.y_offset = hb_saturate_sub (c->font->em_scale_y (markAnchor.yCoordinate),
					  c->font->em_scale_y (currAnchor.yCoordinate));
	  }
	  थांबा;

	  स्थिती 2: /* Control Point Coordinate Actions. */
	  {
	    /* Each action contains four 16-bit fields, so we multiply the ankrActionIndex
	       by 4 to get the correct offset for the given action. */
	    स्थिरांक FWORD *data = (स्थिरांक FWORD *) &ankrData[entry.data.ankrActionIndex * 4];
	    जर (!c->sanitizer.check_array (data, 4)) परत;
	    hb_barrier ();
	    पूर्णांक markX = *data++;
	    पूर्णांक markY = *data++;
	    पूर्णांक currX = *data++;
	    पूर्णांक currY = *data++;

	    o.x_offset = hb_saturate_sub (c->font->em_scale_x (markX), c->font->em_scale_x (currX));
	    o.y_offset = hb_saturate_sub (c->font->em_scale_y (markY), c->font->em_scale_y (currY));
	  }
	  थांबा;
	}
	o.attach_type() = OT::Layout::GPOS_impl::ATTACH_TYPE_MARK;
	o.attach_chain() = (पूर्णांक) mark - (पूर्णांक) buffer->idx;
	जर (c->buffer_is_reversed)
	  o.attach_chain() = -o.attach_chain();
	buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT;
      }

      जर (entry.flags & Mark)
      {
	mark_set = सत्य;
	mark = buffer->idx;
      }
    }

    सार्वजनिक:
    hb_aat_apply_context_t *c;
    स्थिरांक KerxSubTableFormat4 *table;
    खाजगी:
    अचिन्हित पूर्णांक action_type;
    स्थिरांक HBUINT16 *ankrData;
    तार्किक mark_set;
    अचिन्हित पूर्णांक mark;
  };

  तार्किक apply (hb_aat_apply_context_t *c) स्थिरांक
  {
    TRACE_APPLY (हे);

    driver_context_t dc (हे, c);

    StateTableDriver<Types, EntryData, Flags> driver (machine, c->font->face);

    driver.drive (&dc, c);

    return_trace (सत्य);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    /* The rest of array sanitizations are done at run-time. */
    return_trace (likely (c->check_struct (हे) &&
			  machine.sanitize (c)));
  }

  ढाँचा <typename set_t>
  शून्य collect_glyphs (set_t &first_set, set_t &second_set, अचिन्हित अंक_glyphs) स्थिरांक
  {
    machine.collect_initial_glyphs (first_set, अंक_glyphs, *हे);
    //machine.collect_glyphs (second_set, num_glyphs); // second_set is unused for machine kerning
  }

  संरक्षित:
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
    मूल्ये_Are_Long	= 0x00000001,
  };

  तार्किक is_long () स्थिरांक { परत flags & मूल्ये_Are_Long; }

  पूर्णांक get_kerning (hb_codepoint_t left, hb_codepoint_t उजवा,
		   hb_aat_apply_context_t *c) स्थिरांक
  {
    अचिन्हित पूर्णांक अंक_glyphs = c->sanitizer.get_अंक_glyphs ();
    जर (is_long ())
    {
      स्थिरांक स्वतः &t = u.l;
      अचिन्हित पूर्णांक l = (हे+t.rowIndexTable).get_मूल्य_or_null (left, अंक_glyphs);
      अचिन्हित पूर्णांक r = (हे+t.columnIndexTable).get_मूल्य_or_null (उजवा, अंक_glyphs);
      अचिन्हित पूर्णांक offset = l + r;
      जर (unlikely (offset < l)) परत 0; /* Addition overflow. */
      जर (unlikely (hb_unsigned_mul_overflows (offset, आकार (FWORD32)))) परत 0;
      स्थिरांक FWORD32 *v = &StructAtOffset<FWORD32> (&(हे+t.array), offset * आकार (FWORD32));
      जर (unlikely (!v->sanitize (&c->sanitizer))) परत 0;
      hb_barrier ();
      परत kerxTupleKern (*v, header.tuple_मोजणी_2 (), &(हे+vector), c);
    }
    अन्यथा
    {
      स्थिरांक स्वतः &t = u.s;
      अचिन्हित पूर्णांक l = (हे+t.rowIndexTable).get_मूल्य_or_null (left, अंक_glyphs);
      अचिन्हित पूर्णांक r = (हे+t.columnIndexTable).get_मूल्य_or_null (उजवा, अंक_glyphs);
      अचिन्हित पूर्णांक offset = l + r;
      स्थिरांक FWORD *v = &StructAtOffset<FWORD> (&(हे+t.array), offset * आकार (FWORD));
      जर (unlikely (!v->sanitize (&c->sanitizer))) परत 0;
      hb_barrier ();
      परत kerxTupleKern (*v, header.tuple_मोजणी_2 (), &(हे+vector), c);
    }
  }

  तार्किक apply (hb_aat_apply_context_t *c) स्थिरांक
  {
    TRACE_APPLY (हे);

    जर (!c->plan->requested_kerning)
      return_trace (असत्य);

    जर (header.coverage & header.Backwards)
      return_trace (असत्य);

    accelerator_t accel (*हे, c);
    hb_kern_machine_t<accelerator_t> machine (accel, header.coverage & header.CrossStream);
    machine.kern (c->font, c->buffer, c->plan->kern_mask);

    return_trace (सत्य);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (likely (c->check_struct (हे) &&
			  hb_barrier () &&
			  (is_long () ?
			   (
			     u.l.rowIndexTable.sanitize (c, हे) &&
			     u.l.columnIndexTable.sanitize (c, हे) &&
			     c->check_range (हे, u.l.array)
			   ) : (
			     u.s.rowIndexTable.sanitize (c, हे) &&
			     u.s.columnIndexTable.sanitize (c, हे) &&
			     c->check_range (हे, u.s.array)
			   )) &&
			  (header.tuple_मोजणी_2 () == 0 ||
			   c->check_range (हे, vector))));
  }

  ढाँचा <typename set_t>
  शून्य collect_glyphs (set_t &first_set, set_t &second_set, अचिन्हित अंक_glyphs) स्थिरांक
  {
    जर (is_long ())
    {
      स्थिरांक स्वतः &t = u.l;
      (हे+t.rowIndexTable).collect_glyphs (first_set, अंक_glyphs);
      (हे+t.columnIndexTable).collect_glyphs (second_set, अंक_glyphs);
    }
    अन्यथा
    {
      स्थिरांक स्वतः &t = u.s;
      (हे+t.rowIndexTable).collect_glyphs (first_set, अंक_glyphs);
      (हे+t.columnIndexTable).collect_glyphs (second_set, अंक_glyphs);
    }
  }

  संरचना accelerator_t
  {
    स्थिरांक KerxSubTableFormat6 &table;
    hb_aat_apply_context_t *c;

    accelerator_t (स्थिरांक KerxSubTableFormat6 &table_,
		   hb_aat_apply_context_t *c_) :
		     table (table_), c (c_) {}

    पूर्णांक get_kerning (hb_codepoint_t left, hb_codepoint_t उजवा) स्थिरांक
    {
      जर (!(*c->first_set)[left] || !(*c->second_set)[उजवा]) परत 0;
      परत table.get_kerning (left, उजवा, c);
    }
  };

  संरक्षित:
  KernSubTableHeader		header;
  HBUINT32			flags;
  HBUINT16			row_मोजणी;
  HBUINT16			column_मोजणी;
  संघ U
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
  प्रकारव्याख्या ExtendedTypes Types;

  अचिन्हित   tuple_मोजणी_2 () स्थिरांक { परत tuple_मोजणी; }
  तार्किक     is_horizontal () स्थिरांक { परत !(coverage & Vertical); }

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

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे));
  }

  सार्वजनिक:
  HBUINT32	length;
  HBUINT32	coverage;
  HBUINT32	tuple_मोजणी;
  सार्वजनिक:
  DEFINE_SIZE_STATIC (12);
};

संरचना KerxSubTable
{
  मित्र संरचना kerx;

  माप_प्रकार get_size () स्थिरांक { परत u.header.length; }
  अचिन्हित पूर्णांक get_type () स्थिरांक { परत u.header.coverage & u.header.SubtableType; }

  ढाँचा <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) स्थिरांक
  {
    अचिन्हित पूर्णांक subtable_type = get_type ();
    TRACE_DISPATCH (हे, subtable_type);
    निवड (subtable_type) {
    स्थिती 0:	hb_barrier (); return_trace (c->dispatch (u.format0, std::forward<Ts> (ds)...));
    स्थिती 1:	hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    स्थिती 2:	hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    स्थिती 4:	hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
    स्थिती 6:	hb_barrier (); return_trace (c->dispatch (u.format6, std::forward<Ts> (ds)...));
    पूर्वनिर्धारित:	return_trace (c->default_return_मूल्य ());
    }
  }

  ढाँचा <typename set_t>
  शून्य collect_glyphs (set_t &first_set, set_t &second_set, अचिन्हित अंक_glyphs) स्थिरांक
  {
    अचिन्हित पूर्णांक subtable_type = get_type ();
    निवड (subtable_type) {
    स्थिती 0:	hb_barrier (); u.format0.collect_glyphs (first_set, second_set, अंक_glyphs); परत;
    स्थिती 1:	hb_barrier (); u.format1.collect_glyphs (first_set, second_set, अंक_glyphs); परत;
    स्थिती 2:	hb_barrier (); u.format2.collect_glyphs (first_set, second_set, अंक_glyphs); परत;
    स्थिती 4:	hb_barrier (); u.format4.collect_glyphs (first_set, second_set, अंक_glyphs); परत;
    स्थिती 6:	hb_barrier (); u.format6.collect_glyphs (first_set, second_set, अंक_glyphs); परत;
    पूर्वनिर्धारित:	परत;
    }
  }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    जर (!(u.header.sanitize (c) &&
	  hb_barrier () &&
	  u.header.length >= u.header.static_size &&
	  c->check_range (हे, u.header.length)))
      return_trace (असत्य);

    return_trace (dispatch (c));
  }

  सार्वजनिक:
  संघ {
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
  स्थिरांक T* thiz () स्थिरांक { परत static_cast<स्थिरांक T *> (हे); }

  तार्किक has_state_machine () स्थिरांक
  {
    प्रकारव्याख्या typename T::SubTable SubTable;

    स्थिरांक SubTable *st = &thiz()->firstSubTable;
    अचिन्हित पूर्णांक मोजणी = thiz()->table_मोजणी;
    साठी (अचिन्हित पूर्णांक i = 0; i < मोजणी; i++)
    {
      जर (st->get_type () == 1)
	परत सत्य;

      // TODO: What about format 4? What's this API used for anyway?

      st = &StructAfter<SubTable> (*st);
    }
    परत असत्य;
  }

  तार्किक has_cross_stream () स्थिरांक
  {
    प्रकारव्याख्या typename T::SubTable SubTable;

    स्थिरांक SubTable *st = &thiz()->firstSubTable;
    अचिन्हित पूर्णांक मोजणी = thiz()->table_मोजणी;
    साठी (अचिन्हित पूर्णांक i = 0; i < मोजणी; i++)
    {
      जर (st->u.header.coverage & st->u.header.CrossStream)
	परत सत्य;
      st = &StructAfter<SubTable> (*st);
    }
    परत असत्य;
  }

  पूर्णांक get_h_kerning (hb_codepoint_t left, hb_codepoint_t उजवा) स्थिरांक
  {
    प्रकारव्याख्या typename T::SubTable SubTable;

    पूर्णांक v = 0;
    स्थिरांक SubTable *st = &thiz()->firstSubTable;
    अचिन्हित पूर्णांक मोजणी = thiz()->table_मोजणी;
    साठी (अचिन्हित पूर्णांक i = 0; i < मोजणी; i++)
    {
      जर ((st->u.header.coverage & (st->u.header.Variation | st->u.header.CrossStream)) ||
	  !st->u.header.is_horizontal ())
	सुरूठेवा;
      v += st->get_kerning (left, उजवा);
      st = &StructAfter<SubTable> (*st);
    }
    परत v;
  }

  तार्किक apply (AAT::hb_aat_apply_context_t *c,
	      स्थिरांक kern_accelerator_data_t &accel_data) स्थिरांक
  {
    c->buffer->unsafe_to_concat ();

    c->setup_buffer_glyph_set ();

    प्रकारव्याख्या typename T::SubTable SubTable;

    तार्किक ret = असत्य;
    तार्किक seenCrossStream = असत्य;
    c->set_lookup_index (0);
    स्थिरांक SubTable *st = &thiz()->firstSubTable;
    अचिन्हित पूर्णांक मोजणी = thiz()->table_मोजणी;
    साठी (अचिन्हित पूर्णांक i = 0; i < मोजणी; i++)
    {
      तार्किक reverse;

      स्वतः &subtable_accel = accel_data.subtable_accels[i];

      जर (!T::Types::extended && (st->u.header.coverage & st->u.header.Variation))
	जा skip;

      जर (HB_DIRECTION_IS_HORIZONTAL (c->buffer->props.direction) != st->u.header.is_horizontal ())
	जा skip;

      c->first_set = &subtable_accel.first_set;
      c->second_set = &subtable_accel.second_set;
      c->machine_class_cache = &subtable_accel.class_cache;

      जर (!c->buffer_intersects_machine ())
      {
	(शून्य) c->buffer->message (c->font, "skipped subtable %u because no glyph matches", c->lookup_index);
	जा skip;
      }

      reverse = तार्किक (st->u.header.coverage & st->u.header.Backwards) !=
		HB_DIRECTION_IS_BACKWARD (c->buffer->props.direction);

      जर (!c->buffer->message (c->font, "start subtable %u", c->lookup_index))
	जा skip;

      जर (!seenCrossStream &&
	  (st->u.header.coverage & st->u.header.CrossStream))
      {
	/* Attach all glyphs into a chain. */
	seenCrossStream = सत्य;
	hb_glyph_position_t *pos = c->buffer->pos;
	अचिन्हित पूर्णांक मोजणी = c->buffer->len;
	साठी (अचिन्हित पूर्णांक i = 0; i < मोजणी; i++)
	{
	  pos[i].attach_type() = OT::Layout::GPOS_impl::ATTACH_TYPE_CURSIVE;
	  pos[i].attach_chain() = HB_DIRECTION_IS_FORWARD (c->buffer->props.direction) ? -1 : +1;
	  /* We intentionally don't set HB_BUFFER_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT,
	   * since there needs to be a non-zero attachment for post-positioning to
	   * be needed. */
	}
      }

      जर (reverse != c->buffer_is_reversed)
        c->reverse_buffer ();

      {
	/* See comment in sanitize() for conditional here. */
	hb_sanitize_with_object_t with (&c->sanitizer, i < मोजणी - 1 ? st : (स्थिरांक SubTable *) nullptr);
	ret |= st->dispatch (c);
      }

      (शून्य) c->buffer->message (c->font, "end subtable %u", c->lookup_index);

    skip:
      st = &StructAfter<SubTable> (*st);
      c->set_lookup_index (c->lookup_index + 1);
    }
    जर (c->buffer_is_reversed)
      c->reverse_buffer ();

    परत ret;
  }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    जर (unlikely (!(thiz()->version.sanitize (c) &&
		    hb_barrier () &&
		    (अचिन्हित) thiz()->version >= (अचिन्हित) T::minVersion &&
		    thiz()->table_मोजणी.sanitize (c))))
      return_trace (असत्य);

    प्रकारव्याख्या typename T::SubTable SubTable;

    स्थिरांक SubTable *st = &thiz()->firstSubTable;
    अचिन्हित पूर्णांक मोजणी = thiz()->table_मोजणी;
    साठी (अचिन्हित पूर्णांक i = 0; i < मोजणी; i++)
    {
      जर (unlikely (!st->u.header.sanitize (c)))
	return_trace (असत्य);
      hb_barrier ();
      /* OpenType kern table has 2-byte subtable lengths.  That's limiting.
       * MS implementation also only supports one subtable, of format 0,
       * anyway.  Certain versions of some fonts, like Calibry, contain
       * kern subtable that exceeds 64kb.  Looks like, the subtable length
       * is simply ignored.  Which makes sense.  It's only needed if you
       * have multiple subtables.  To handle such fonts, we just ignore
       * the length for the last subtable. */
      hb_sanitize_with_object_t with (c, i < मोजणी - 1 ? st : (स्थिरांक SubTable *) nullptr);

      जर (unlikely (!st->sanitize (c)))
	return_trace (असत्य);

      st = &StructAfter<SubTable> (*st);
    }

    अचिन्हित majorVersion = thiz()->version;
    जर (आकार (thiz()->version) == 4)
      majorVersion = majorVersion >> 16;
    जर (majorVersion >= 3)
    {
      स्थिरांक SubtableGlyphCoverage *coverage = (स्थिरांक SubtableGlyphCoverage *) st;
      जर (!coverage->sanitize (c, मोजणी))
        return_trace (असत्य);
    }

    return_trace (सत्य);
  }

  kern_accelerator_data_t create_accelerator_data (अचिन्हित अंक_glyphs) स्थिरांक
  {
    kern_accelerator_data_t accel_data;

    प्रकारव्याख्या typename T::SubTable SubTable;

    स्थिरांक SubTable *st = &thiz()->firstSubTable;
    अचिन्हित पूर्णांक मोजणी = thiz()->table_मोजणी;
    साठी (अचिन्हित पूर्णांक i = 0; i < मोजणी; i++)
    {
      स्वतः &subtable_accel = *accel_data.subtable_accels.push ();
      जर (unlikely (accel_data.subtable_accels.in_error ()))
	  परत accel_data;

      st->collect_glyphs (subtable_accel.first_set, subtable_accel.second_set, अंक_glyphs);
      subtable_accel.class_cache.clear ();

      st = &StructAfter<SubTable> (*st);
    }

    परत accel_data;
  }

  संरचना accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      hb_sanitize_context_t sc;
      हे->table = sc.reference_table<T> (face);
      हे->accel_data = हे->table->create_accelerator_data (face->get_अंक_glyphs ());
    }
    ~accelerator_t ()
    {
      हे->table.destroy ();
    }

    hb_blob_t *get_blob () स्थिरांक { परत table.get_blob (); }

    तार्किक apply (AAT::hb_aat_apply_context_t *c) स्थिरांक
    {
      परत table->apply (c, accel_data);
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

  प्रकारव्याख्या KerxSubTableHeader SubTableHeader;
  प्रकारव्याख्या SubTableHeader::Types Types;
  प्रकारव्याख्या KerxSubTable SubTable;

  तार्किक has_data () स्थिरांक { परत version; }

  संरक्षित:
  HBUINT16	version;	/* The version number of the extended kerning table
				 * (currently 2, 3, or 4). */
  HBUINT16	unused;		/* Set to 0. */
  HBUINT32	table_मोजणी;	/* The number of subtables included in the extended kerning
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
