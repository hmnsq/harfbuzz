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

#अगर_अघोषित HB_AAT_LAYOUT_KERX_TABLE_HH
#घोषणा HB_AAT_LAYOUT_KERX_TABLE_HH

#समावेश "hb-kern.hh"
#समावेश "hb-aat-layout-ankr-table.hh"
#समावेश "hb-set-digest.hh"

/*
 * kerx -- Extended Kerning
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6kerx.html
 */
#घोषणा HB_AAT_TAG_kerx HB_TAG('k','e','r','x')


namespace AAT {

using namespace OT;


अटल अंतरभूत पूर्णांक
kerxTupleKern (पूर्णांक value,
	       अचिन्हित पूर्णांक tuple_गिनती,
	       स्थिर व्योम *base,
	       hb_aat_apply_context_t *c)
{
  अगर (likely (!tuple_गिनती || !c)) वापस value;

  अचिन्हित पूर्णांक offset = value;
  स्थिर FWORD *pv = &StructAtOffset<FWORD> (base, offset);
  अगर (unlikely (!c->sanitizer.check_array (pv, tuple_गिनती))) वापस 0;
  hb_barrier ();
  वापस *pv;
}


काष्ठा hb_glyph_pair_t
{
  hb_codepoint_t बाँए;
  hb_codepoint_t दाँए;
};

काष्ठा KernPair
{
  पूर्णांक get_kerning () स्थिर { वापस value; }

  पूर्णांक cmp (स्थिर hb_glyph_pair_t &o) स्थिर
  {
    पूर्णांक ret = बाँए.cmp (o.बाँए);
    अगर (ret) वापस ret;
    वापस दाँए.cmp (o.दाँए);
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह));
  }

  खुला:
  HBGlyphID16	बाँए;
  HBGlyphID16	दाँए;
  FWORD		value;
  खुला:
  DEFINE_आकार_STATIC (6);
};

ढाँचा <typename KernSubTableHeader>
काष्ठा KerxSubTableFormat0
{
  पूर्णांक get_kerning (hb_codepoint_t बाँए, hb_codepoint_t दाँए,
		   hb_aat_apply_context_t *c = nullptr) स्थिर
  {
    hb_glyph_pair_t pair = {बाँए, दाँए};
    पूर्णांक v = pairs.द्वा_खोज (pair).get_kerning ();
    वापस kerxTupleKern (v, header.tuple_गिनती_2 (), यह, c);
  }

  bool apply (hb_aat_apply_context_t *c) स्थिर
  {
    TRACE_APPLY (यह);

    अगर (!c->plan->requested_kerning)
      return_trace (false);

    अगर (header.coverage & header.Backwards)
      return_trace (false);

    accelerator_t accel (*यह, c);
    hb_kern_machine_t<accelerator_t> machine (accel, header.coverage & header.CrossStream);
    machine.kern (c->font, c->बफर, c->plan->kern_mask);

    return_trace (true);
  }

  ढाँचा <typename set_t>
  व्योम collect_glyphs (set_t &first_set, set_t &second_set, अचिन्हित अंक_glyphs) स्थिर
  {
    क्रम (स्थिर KernPair& pair : pairs)
    {
      first_set.add (pair.बाँए);
      second_set.add (pair.दाँए);
    }
  }

  काष्ठा accelerator_t
  {
    स्थिर KerxSubTableFormat0 &table;
    hb_aat_apply_context_t *c;

    accelerator_t (स्थिर KerxSubTableFormat0 &table_,
		   hb_aat_apply_context_t *c_) :
		     table (table_), c (c_) {}

    पूर्णांक get_kerning (hb_codepoint_t बाँए, hb_codepoint_t दाँए) स्थिर
    {
      अगर (!(*c->first_set)[बाँए] || !(*c->second_set)[दाँए]) वापस 0;
      वापस table.get_kerning (बाँए, दाँए, c);
    }
  };


  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (likely (pairs.sanitize (c)));
  }

  रक्षित:
  KernSubTableHeader	header;
  Bin_खोज_Array_Of<KernPair, typename KernSubTableHeader::Types::HBUINT>
			pairs;	/* Sorted kern records. */
  खुला:
  DEFINE_आकार_ARRAY (KernSubTableHeader::static_आकार + 16, pairs);
};


ढाँचा <bool extended>
काष्ठा Format1Entry;

ढाँचा <>
काष्ठा Format1Entry<true>
{
  क्रमागत Flags
  {
    धकेलो		= 0x8000,	/* If set, push this glyph on the kerning stack. */
    DontAdvance		= 0x4000,	/* If set, don't advance to the next glyph
					 * before going to the new state. */
    Reset		= 0x2000,	/* If set, reset the kerning data (clear the stack) */
    Reserved		= 0x1FFF,	/* Not used; set to 0. */
  };

  काष्ठा Entry_तथ्य
  {
    HBUINT16	kern_Action_सूचकांक;/* Index into the kerning value array. If
				 * this index is 0xFFFF, then no kerning
				 * is to be performed. */
    खुला:
    DEFINE_आकार_STATIC (2);
  };

  अटल bool initiateAction (स्थिर Entry<Entry_तथ्य> &entry)
  { वापस entry.flags & धकेलो; }

  अटल bool performAction (स्थिर Entry<Entry_तथ्य> &entry)
  { वापस entry.तथ्य.kern_Action_सूचकांक != 0xFFFF; }

  अटल अचिन्हित पूर्णांक kern_Action_सूचकांक (स्थिर Entry<Entry_तथ्य> &entry)
  { वापस entry.तथ्य.kern_Action_सूचकांक; }
};
ढाँचा <>
काष्ठा Format1Entry<false>
{
  क्रमागत Flags
  {
    धकेलो		= 0x8000,	/* If set, push this glyph on the kerning stack. */
    DontAdvance		= 0x4000,	/* If set, don't advance to the next glyph
					 * before going to the new state. */
    Offset		= 0x3FFF,	/* Byte offset from beginning of subtable to the
					 * value table for the glyphs on the kerning stack. */

    Reset		= 0x0000,	/* Not supported? */
  };

  प्रकार व्योम Entry_तथ्य;

  अटल bool initiateAction (स्थिर Entry<Entry_तथ्य> &entry)
  { वापस entry.flags & धकेलो; }

  अटल bool performAction (स्थिर Entry<Entry_तथ्य> &entry)
  { वापस entry.flags & Offset; }

  अटल अचिन्हित पूर्णांक kern_Action_सूचकांक (स्थिर Entry<Entry_तथ्य> &entry)
  { वापस entry.flags & Offset; }
};

ढाँचा <typename KernSubTableHeader>
काष्ठा KerxSubTableFormat1
{
  प्रकार typename KernSubTableHeader::Types Types;
  प्रकार typename Types::HBUINT HBUINT;

  प्रकार Format1Entry<Types::extended> Format1EntryT;
  प्रकार typename Format1EntryT::Entry_तथ्य Entry_तथ्य;

  क्रमागत Flags
  {
    DontAdvance	= Format1EntryT::DontAdvance,
  };

  bool is_action_initiable (स्थिर Entry<Entry_तथ्य> &entry) स्थिर
  {
    वापस Format1EntryT::initiateAction (entry);
  }
  bool is_actionable (स्थिर Entry<Entry_तथ्य> &entry) स्थिर
  {
    वापस Format1EntryT::performAction (entry);
  }

  काष्ठा driver_context_t
  {
    अटल constexpr bool in_place = true;

    driver_context_t (स्थिर KerxSubTableFormat1 *table_,
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
    व्योम transition (hb_बफर_t *बफर,
		     StateTableDriver<Types, Entry_तथ्य, Flags> *driver,
		     स्थिर Entry<Entry_तथ्य> &entry)
    {
      अचिन्हित पूर्णांक flags = entry.flags;

      अगर (flags & Format1EntryT::Reset)
	depth = 0;

      अगर (flags & Format1EntryT::धकेलो)
      {
	अगर (likely (depth < ARRAY_लंबाई (ढेर)))
	  ढेर[depth++] = बफर->idx;
	अन्यथा
	  depth = 0; /* Probably not what CoreText does, but better? */
      }

      अगर (Format1EntryT::performAction (entry) && depth)
      {
	अचिन्हित पूर्णांक tuple_गिनती_2 = hb_max (1u, table->header.tuple_गिनती_2 ());

	अचिन्हित पूर्णांक kern_idx = Format1EntryT::kern_Action_सूचकांक (entry);
	kern_idx = Types::byte_Offset_To_सूचकांक (kern_idx, &table->machine, kernAction.arrayZ);
	स्थिर FWORD *actions = &kernAction[kern_idx];
	अगर (!c->sanitizer.check_array (actions, depth, tuple_गिनती_2))
	{
	  depth = 0;
	  वापस;
	}
	hb_barrier ();

	hb_mask_t kern_mask = c->plan->kern_mask;

	/* From Apple 'kern' spec:
	 * "Each pops one glyph from the kerning stack and applies the kerning value to it.
	 * The end of the list is marked by an odd value... */
	bool last = false;
	जबतक (!last && depth)
	{
	  अचिन्हित पूर्णांक idx = ढेर[--depth];
	  पूर्णांक v = *actions;
	  actions += tuple_गिनती_2;
	  अगर (idx >= बफर->len) जारी;

	  /* "The end of the list is marked by an odd value..." */
	  last = v & 1;
	  v &= ~1;

	  hb_glyph_position_t &o = बफर->pos[idx];

	  अगर (HB_DIRECTION_IS_HORIZONTAL (बफर->props.direction))
	  {
	    अगर (crossStream)
	    {
	      /* The following flag is undocumented in the spec, but described
	       * in the 'kern' table example. */
	      अगर (v == -0x8000)
	      {
		o.attach_type() = OT::Layout::GPOS_impl::ATTACH_TYPE_NONE;
		o.attach_chain() = 0;
		o.y_offset = 0;
	      }
	      अन्यथा अगर (o.attach_type())
	      {
		o.y_offset = hb_saturate_add (o.y_offset, c->font->em_scale_y (v));
		बफर->scratch_flags |= HB_बफर_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT;
	      }
	    }
	    अन्यथा अगर (बफर->info[idx].mask & kern_mask)
	    {
	      स्वतः scaled = c->font->em_scale_x (v);
	      o.x_advance = hb_saturate_add (o.x_advance, scaled);
	      o.x_offset = hb_saturate_add (o.x_offset, scaled);
	    }
	  }
	  अन्यथा
	  {
	    अगर (crossStream)
	    {
	      /* CoreText doesn't do crossStream kerning in vertical.  We do. */
	      अगर (v == -0x8000)
	      {
		o.attach_type() = OT::Layout::GPOS_impl::ATTACH_TYPE_NONE;
		o.attach_chain() = 0;
		o.x_offset = 0;
	      }
	      अन्यथा अगर (o.attach_type())
	      {
		o.x_offset = hb_saturate_add (o.x_offset, c->font->em_scale_x (v));
		बफर->scratch_flags |= HB_बफर_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT;
	      }
	    }
	    अन्यथा अगर (बफर->info[idx].mask & kern_mask)
	    {
	      स्वतः scaled = c->font->em_scale_y (v);
	      o.y_advance = hb_saturate_add (o.y_advance, scaled);
	      o.y_offset = hb_saturate_add (o.y_offset, scaled);
	    }
	  }
	}
      }
    }

    खुला:
    hb_aat_apply_context_t *c;
    स्थिर KerxSubTableFormat1 *table;
    निजी:
    स्थिर UnsizedArrayOf<FWORD> &kernAction;
    अचिन्हित पूर्णांक ढेर[8];
    अचिन्हित पूर्णांक depth;
    bool crossStream;
  };

  bool apply (hb_aat_apply_context_t *c) स्थिर
  {
    TRACE_APPLY (यह);

    अगर (!c->plan->requested_kerning &&
	!(header.coverage & header.CrossStream))
      वापस false;

    driver_context_t dc (यह, c);

    StateTableDriver<Types, Entry_तथ्य, Flags> driver (machine, c->font->face);

    driver.drive (&dc, c);

    return_trace (true);
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    /* The rest of array sanitizations are done at run-time. */
    return_trace (likely (c->check_struct (यह) &&
			  machine.sanitize (c)));
  }

  ढाँचा <typename set_t>
  व्योम collect_glyphs (set_t &first_set, set_t &second_set, अचिन्हित अंक_glyphs) स्थिर
  {
    machine.collect_initial_glyphs (first_set, अंक_glyphs, *यह);
    //machine.collect_glyphs (second_set, num_glyphs); // second_set is unused for machine kerning
  }

  रक्षित:
  KernSubTableHeader				header;
  StateTable<Types, Entry_तथ्य>			machine;
  NNOffsetTo<UnsizedArrayOf<FWORD>, HBUINT>	kernAction;
  खुला:
  DEFINE_आकार_STATIC (KernSubTableHeader::static_आकार + (StateTable<Types, Entry_तथ्य>::static_आकार + HBUINT::static_आकार));
};

ढाँचा <typename KernSubTableHeader>
काष्ठा KerxSubTableFormat2
{
  प्रकार typename KernSubTableHeader::Types Types;
  प्रकार typename Types::HBUINT HBUINT;

  पूर्णांक get_kerning (hb_codepoint_t बाँए, hb_codepoint_t दाँए,
		   hb_aat_apply_context_t *c) स्थिर
  {
    अचिन्हित पूर्णांक अंक_glyphs = c->sanitizer.get_अंक_glyphs ();
    अचिन्हित पूर्णांक l = (यह+बायाँ_Class_Table).get_class (बाँए, अंक_glyphs, 0);
    अचिन्हित पूर्णांक r = (यह+दायाँ_Class_Table).get_class (दाँए, अंक_glyphs, 0);

    स्थिर UnsizedArrayOf<FWORD> &arrayZ = यह+array;
    अचिन्हित पूर्णांक kern_idx = l + r;
    kern_idx = Types::offset_To_सूचकांक (kern_idx, यह, arrayZ.arrayZ);
    स्थिर FWORD *v = &arrayZ[kern_idx];
    अगर (unlikely (!v->sanitize (&c->sanitizer))) वापस 0;
    hb_barrier ();

    वापस kerxTupleKern (*v, header.tuple_गिनती_2 (), यह, c);
  }

  bool apply (hb_aat_apply_context_t *c) स्थिर
  {
    TRACE_APPLY (यह);

    अगर (!c->plan->requested_kerning)
      return_trace (false);

    अगर (header.coverage & header.Backwards)
      return_trace (false);

    accelerator_t accel (*यह, c);
    hb_kern_machine_t<accelerator_t> machine (accel, header.coverage & header.CrossStream);
    machine.kern (c->font, c->बफर, c->plan->kern_mask);

    return_trace (true);
  }

  ढाँचा <typename set_t>
  व्योम collect_glyphs (set_t &first_set, set_t &second_set, अचिन्हित अंक_glyphs) स्थिर
  {
    (यह+बायाँ_Class_Table).collect_glyphs (first_set, अंक_glyphs);
    (यह+दायाँ_Class_Table).collect_glyphs (second_set, अंक_glyphs);
  }

  काष्ठा accelerator_t
  {
    स्थिर KerxSubTableFormat2 &table;
    hb_aat_apply_context_t *c;

    accelerator_t (स्थिर KerxSubTableFormat2 &table_,
		   hb_aat_apply_context_t *c_) :
		     table (table_), c (c_) {}

    पूर्णांक get_kerning (hb_codepoint_t बाँए, hb_codepoint_t दाँए) स्थिर
    {
      अगर (!(*c->first_set)[बाँए] || !(*c->second_set)[दाँए]) वापस 0;
      वापस table.get_kerning (बाँए, दाँए, c);
    }
  };

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (likely (c->check_struct (यह) &&
			  बायाँ_Class_Table.sanitize (c, यह) &&
			  दायाँ_Class_Table.sanitize (c, यह) &&
			  hb_barrier () &&
			  c->check_range (यह, array)));
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
  खुला:
  DEFINE_आकार_STATIC (KernSubTableHeader::static_आकार + 4 * माप (HBUINT));
};

ढाँचा <typename KernSubTableHeader>
काष्ठा KerxSubTableFormat4
{
  प्रकार ExtendedTypes Types;

  काष्ठा Entry_तथ्य
  {
    HBUINT16	ankr_Action_सूचकांक;/* Either 0xFFFF (for no action) or the index of
				 * the action to perform. */
    खुला:
    DEFINE_आकार_STATIC (2);
  };

  क्रमागत Flags
  {
    Mark		= 0x8000,	/* If set, remember this glyph as the marked glyph. */
    DontAdvance		= 0x4000,	/* If set, don't advance to the next glyph before
					 * going to the new state. */
    Reserved		= 0x3FFF,	/* Not used; set to 0. */
  };

  bool is_action_initiable (स्थिर Entry<Entry_तथ्य> &entry) स्थिर
  {
    वापस (entry.flags & Mark);
  }
  bool is_actionable (स्थिर Entry<Entry_तथ्य> &entry) स्थिर
  {
    वापस entry.तथ्य.ankr_Action_सूचकांक != 0xFFFF;
  }

  काष्ठा driver_context_t
  {
    अटल constexpr bool in_place = true;
    क्रमागत SubTableFlags
    {
      ActionType	= 0xC0000000,	/* A two-bit field containing the action type. */
      Unused		= 0x3F000000,	/* Unused - must be zero. */
      Offset		= 0x00FFFFFF,	/* Masks the offset in bytes from the beginning
					 * of the subtable to the beginning of the control
					 * point table. */
    };

    driver_context_t (स्थिर KerxSubTableFormat4 *table_,
		      hb_aat_apply_context_t *c_) :
	c (c_),
	table (table_),
	action_type ((table->flags & ActionType) >> 30),
	ankr_तथ्य ((HBUINT16 *) ((स्थिर अक्षर *) &table->machine + (table->flags & Offset))),
	mark_set (false),
	mark (0) {}

    HB_AAT_TRANSITION_INLINE
    व्योम transition (hb_बफर_t *बफर,
		     StateTableDriver<Types, Entry_तथ्य, Flags> *driver,
		     स्थिर Entry<Entry_तथ्य> &entry)
    {
      अगर (mark_set && entry.तथ्य.ankr_Action_सूचकांक != 0xFFFF && बफर->idx < बफर->len)
      {
	hb_glyph_position_t &o = बफर->cur_pos();
	चयन (action_type)
	{
	  हाल 0: /* Control Point Actions.*/
	  {
	    /* Indexed into glyph outline. */
	    /* Each action (record in ankrData) contains two 16-bit fields, so we must
	       double the ankrActionIndex to get the correct offset here. */
	    स्थिर HBUINT16 *तथ्य = &ankr_तथ्य[entry.तथ्य.ankr_Action_सूचकांक * 2];
	    अगर (!c->sanitizer.check_array (तथ्य, 2)) वापस;
	    hb_barrier ();
	    अचिन्हित पूर्णांक markControlPoint = *तथ्य++;
	    अचिन्हित पूर्णांक currControlPoint = *तथ्य++;
	    hb_position_t markX = 0;
	    hb_position_t markY = 0;
	    hb_position_t currX = 0;
	    hb_position_t currY = 0;
	    अगर (!c->font->get_glyph_contour_point_for_origin (c->बफर->info[mark].codepoint,
							      markControlPoint,
							      HB_DIRECTION_LTR /*XXX*/,
							      &markX, &markY) ||
		!c->font->get_glyph_contour_point_for_origin (c->बफर->cur ().codepoint,
							      currControlPoint,
							      HB_DIRECTION_LTR /*XXX*/,
							      &currX, &currY))
	      वापस;

	    o.x_offset = hb_saturate_sub (markX, currX);
	    o.y_offset = hb_saturate_sub (markY, currY);
	  }
	  अवरोध;

	  हाल 1: /* Anchor Point Actions. */
	  {
	    /* Indexed into 'ankr' table. */
	    /* Each action (record in ankrData) contains two 16-bit fields, so we must
	       double the ankrActionIndex to get the correct offset here. */
	    स्थिर HBUINT16 *तथ्य = &ankr_तथ्य[entry.तथ्य.ankr_Action_सूचकांक * 2];
	    अगर (!c->sanitizer.check_array (तथ्य, 2)) वापस;
	    hb_barrier ();
	    अचिन्हित पूर्णांक markAnchorPoint = *तथ्य++;
	    अचिन्हित पूर्णांक currAnchorPoint = *तथ्य++;
	    स्थिर Anchor &markAnchor = c->ankr_table->get_anchor (c->बफर->info[mark].codepoint,
								  markAnchorPoint,
								  c->sanitizer.get_अंक_glyphs ());
	    स्थिर Anchor &currAnchor = c->ankr_table->get_anchor (c->बफर->cur ().codepoint,
								  currAnchorPoint,
								  c->sanitizer.get_अंक_glyphs ());

	    o.x_offset = hb_saturate_sub (c->font->em_scale_x (markAnchor.xCoordinate),
					  c->font->em_scale_x (currAnchor.xCoordinate));
	    o.y_offset = hb_saturate_sub (c->font->em_scale_y (markAnchor.yCoordinate),
					  c->font->em_scale_y (currAnchor.yCoordinate));
	  }
	  अवरोध;

	  हाल 2: /* Control Point Coordinate Actions. */
	  {
	    /* Each action contains four 16-bit fields, so we multiply the ankrActionIndex
	       by 4 to get the correct offset for the given action. */
	    स्थिर FWORD *तथ्य = (स्थिर FWORD *) &ankr_तथ्य[entry.तथ्य.ankr_Action_सूचकांक * 4];
	    अगर (!c->sanitizer.check_array (तथ्य, 4)) वापस;
	    hb_barrier ();
	    पूर्णांक markX = *तथ्य++;
	    पूर्णांक markY = *तथ्य++;
	    पूर्णांक currX = *तथ्य++;
	    पूर्णांक currY = *तथ्य++;

	    o.x_offset = hb_saturate_sub (c->font->em_scale_x (markX), c->font->em_scale_x (currX));
	    o.y_offset = hb_saturate_sub (c->font->em_scale_y (markY), c->font->em_scale_y (currY));
	  }
	  अवरोध;
	}
	o.attach_type() = OT::Layout::GPOS_impl::ATTACH_TYPE_MARK;
	o.attach_chain() = (पूर्णांक) mark - (पूर्णांक) बफर->idx;
	अगर (c->बफर_is_reversed)
	  o.attach_chain() = -o.attach_chain();
	बफर->scratch_flags |= HB_बफर_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT;
      }

      अगर (entry.flags & Mark)
      {
	mark_set = true;
	mark = बफर->idx;
      }
    }

    खुला:
    hb_aat_apply_context_t *c;
    स्थिर KerxSubTableFormat4 *table;
    निजी:
    अचिन्हित पूर्णांक action_type;
    स्थिर HBUINT16 *ankr_तथ्य;
    bool mark_set;
    अचिन्हित पूर्णांक mark;
  };

  bool apply (hb_aat_apply_context_t *c) स्थिर
  {
    TRACE_APPLY (यह);

    driver_context_t dc (यह, c);

    StateTableDriver<Types, Entry_तथ्य, Flags> driver (machine, c->font->face);

    driver.drive (&dc, c);

    return_trace (true);
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    /* The rest of array sanitizations are done at run-time. */
    return_trace (likely (c->check_struct (यह) &&
			  machine.sanitize (c)));
  }

  ढाँचा <typename set_t>
  व्योम collect_glyphs (set_t &first_set, set_t &second_set, अचिन्हित अंक_glyphs) स्थिर
  {
    machine.collect_initial_glyphs (first_set, अंक_glyphs, *यह);
    //machine.collect_glyphs (second_set, num_glyphs); // second_set is unused for machine kerning
  }

  रक्षित:
  KernSubTableHeader		header;
  StateTable<Types, Entry_तथ्य>	machine;
  HBUINT32			flags;
  खुला:
  DEFINE_आकार_STATIC (KernSubTableHeader::static_आकार + (StateTable<Types, Entry_तथ्य>::static_आकार + HBUINT32::static_आकार));
};

ढाँचा <typename KernSubTableHeader>
काष्ठा KerxSubTableFormat6
{
  क्रमागत Flags
  {
    मूल्यावली_Are_Long	= 0x00000001,
  };

  bool is_long () स्थिर { वापस flags & मूल्यावली_Are_Long; }

  पूर्णांक get_kerning (hb_codepoint_t बाँए, hb_codepoint_t दाँए,
		   hb_aat_apply_context_t *c) स्थिर
  {
    अचिन्हित पूर्णांक अंक_glyphs = c->sanitizer.get_अंक_glyphs ();
    अगर (is_long ())
    {
      स्थिर स्वतः &t = u.l;
      अचिन्हित पूर्णांक l = (यह+t.row_सूचकांक_Table).get_मूल्य_or_null (बाँए, अंक_glyphs);
      अचिन्हित पूर्णांक r = (यह+t.column_सूचकांक_Table).get_मूल्य_or_null (दाँए, अंक_glyphs);
      अचिन्हित पूर्णांक offset = l + r;
      अगर (unlikely (offset < l)) वापस 0; /* Addition overflow. */
      अगर (unlikely (hb_unsigned_mul_overflows (offset, माप (FWORD32)))) वापस 0;
      स्थिर FWORD32 *v = &StructAtOffset<FWORD32> (&(यह+t.array), offset * माप (FWORD32));
      अगर (unlikely (!v->sanitize (&c->sanitizer))) वापस 0;
      hb_barrier ();
      वापस kerxTupleKern (*v, header.tuple_गिनती_2 (), &(यह+vector), c);
    }
    अन्यथा
    {
      स्थिर स्वतः &t = u.s;
      अचिन्हित पूर्णांक l = (यह+t.row_सूचकांक_Table).get_मूल्य_or_null (बाँए, अंक_glyphs);
      अचिन्हित पूर्णांक r = (यह+t.column_सूचकांक_Table).get_मूल्य_or_null (दाँए, अंक_glyphs);
      अचिन्हित पूर्णांक offset = l + r;
      स्थिर FWORD *v = &StructAtOffset<FWORD> (&(यह+t.array), offset * माप (FWORD));
      अगर (unlikely (!v->sanitize (&c->sanitizer))) वापस 0;
      hb_barrier ();
      वापस kerxTupleKern (*v, header.tuple_गिनती_2 (), &(यह+vector), c);
    }
  }

  bool apply (hb_aat_apply_context_t *c) स्थिर
  {
    TRACE_APPLY (यह);

    अगर (!c->plan->requested_kerning)
      return_trace (false);

    अगर (header.coverage & header.Backwards)
      return_trace (false);

    accelerator_t accel (*यह, c);
    hb_kern_machine_t<accelerator_t> machine (accel, header.coverage & header.CrossStream);
    machine.kern (c->font, c->बफर, c->plan->kern_mask);

    return_trace (true);
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (likely (c->check_struct (यह) &&
			  hb_barrier () &&
			  (is_long () ?
			   (
			     u.l.row_सूचकांक_Table.sanitize (c, यह) &&
			     u.l.column_सूचकांक_Table.sanitize (c, यह) &&
			     c->check_range (यह, u.l.array)
			   ) : (
			     u.s.row_सूचकांक_Table.sanitize (c, यह) &&
			     u.s.column_सूचकांक_Table.sanitize (c, यह) &&
			     c->check_range (यह, u.s.array)
			   )) &&
			  (header.tuple_गिनती_2 () == 0 ||
			   c->check_range (यह, vector))));
  }

  ढाँचा <typename set_t>
  व्योम collect_glyphs (set_t &first_set, set_t &second_set, अचिन्हित अंक_glyphs) स्थिर
  {
    अगर (is_long ())
    {
      स्थिर स्वतः &t = u.l;
      (यह+t.row_सूचकांक_Table).collect_glyphs (first_set, अंक_glyphs);
      (यह+t.column_सूचकांक_Table).collect_glyphs (second_set, अंक_glyphs);
    }
    अन्यथा
    {
      स्थिर स्वतः &t = u.s;
      (यह+t.row_सूचकांक_Table).collect_glyphs (first_set, अंक_glyphs);
      (यह+t.column_सूचकांक_Table).collect_glyphs (second_set, अंक_glyphs);
    }
  }

  काष्ठा accelerator_t
  {
    स्थिर KerxSubTableFormat6 &table;
    hb_aat_apply_context_t *c;

    accelerator_t (स्थिर KerxSubTableFormat6 &table_,
		   hb_aat_apply_context_t *c_) :
		     table (table_), c (c_) {}

    पूर्णांक get_kerning (hb_codepoint_t बाँए, hb_codepoint_t दाँए) स्थिर
    {
      अगर (!(*c->first_set)[बाँए] || !(*c->second_set)[दाँए]) वापस 0;
      वापस table.get_kerning (बाँए, दाँए, c);
    }
  };

  रक्षित:
  KernSubTableHeader		header;
  HBUINT32			flags;
  HBUINT16			row_गिनती;
  HBUINT16			column_गिनती;
  जोड़ U
  {
    काष्ठा Long
    {
      NNOffset32To<Lookup<HBUINT32>>		row_सूचकांक_Table;
      NNOffset32To<Lookup<HBUINT32>>		column_सूचकांक_Table;
      NNOffset32To<UnsizedArrayOf<FWORD32>>	array;
    } l;
    काष्ठा Short
    {
      NNOffset32To<Lookup<HBUINT16>>		row_सूचकांक_Table;
      NNOffset32To<Lookup<HBUINT16>>		column_सूचकांक_Table;
      NNOffset32To<UnsizedArrayOf<FWORD>>	array;
    } s;
  } u;
  NNOffset32To<UnsizedArrayOf<FWORD>>	vector;
  खुला:
  DEFINE_आकार_STATIC (KernSubTableHeader::static_आकार + 24);
};


काष्ठा KerxSubTableHeader
{
  प्रकार ExtendedTypes Types;

  अचिन्हित   tuple_गिनती_2 () स्थिर { वापस tuple_गिनती; }
  bool     is_horizontal () स्थिर { वापस !(coverage & Vertical); }

  क्रमागत Coverage
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

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह));
  }

  खुला:
  HBUINT32	लंबाई;
  HBUINT32	coverage;
  HBUINT32	tuple_गिनती;
  खुला:
  DEFINE_आकार_STATIC (12);
};

काष्ठा KerxSubTable
{
  मित्र काष्ठा kerx;

  माप_प्रकार get_आकार () स्थिर { वापस u.header.लंबाई; }
  अचिन्हित पूर्णांक get_type () स्थिर { वापस u.header.coverage & u.header.SubtableType; }

  ढाँचा <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) स्थिर
  {
    अचिन्हित पूर्णांक subtable_type = get_type ();
    TRACE_DISPATCH (यह, subtable_type);
    चयन (subtable_type) {
    हाल 0:	hb_barrier (); return_trace (c->dispatch (u.format0, std::forward<Ts> (ds)...));
    हाल 1:	hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    हाल 2:	hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    हाल 4:	hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
    हाल 6:	hb_barrier (); return_trace (c->dispatch (u.format6, std::forward<Ts> (ds)...));
    शेष:	return_trace (c->default_return_मूल्य ());
    }
  }

  ढाँचा <typename set_t>
  व्योम collect_glyphs (set_t &first_set, set_t &second_set, अचिन्हित अंक_glyphs) स्थिर
  {
    अचिन्हित पूर्णांक subtable_type = get_type ();
    चयन (subtable_type) {
    हाल 0:	hb_barrier (); u.format0.collect_glyphs (first_set, second_set, अंक_glyphs); वापस;
    हाल 1:	hb_barrier (); u.format1.collect_glyphs (first_set, second_set, अंक_glyphs); वापस;
    हाल 2:	hb_barrier (); u.format2.collect_glyphs (first_set, second_set, अंक_glyphs); वापस;
    हाल 4:	hb_barrier (); u.format4.collect_glyphs (first_set, second_set, अंक_glyphs); वापस;
    हाल 6:	hb_barrier (); u.format6.collect_glyphs (first_set, second_set, अंक_glyphs); वापस;
    शेष:	वापस;
    }
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    अगर (!(u.header.sanitize (c) &&
	  hb_barrier () &&
	  u.header.लंबाई >= u.header.static_आकार &&
	  c->check_range (यह, u.header.लंबाई)))
      return_trace (false);

    return_trace (dispatch (c));
  }

  खुला:
  जोड़ {
  KerxSubTableHeader				header;
  KerxSubTableFormat0<KerxSubTableHeader>	format0;
  KerxSubTableFormat1<KerxSubTableHeader>	format1;
  KerxSubTableFormat2<KerxSubTableHeader>	format2;
  KerxSubTableFormat4<KerxSubTableHeader>	format4;
  KerxSubTableFormat6<KerxSubTableHeader>	format6;
  } u;
  खुला:
  DEFINE_आकार_MIN (12);
};


/*
 * The 'kerx' Table
 */

काष्ठा kern_subtable_accelerator_तथ्य_t
{
  hb_bit_set_t first_set;
  hb_bit_set_t second_set;
  mutable hb_aat_class_cache_t class_cache;
};

काष्ठा kern_accelerator_तथ्य_t
{
  hb_vector_t<kern_subtable_accelerator_तथ्य_t> subtable_accels;
  hb_aat_scratch_t scratch;
};

ढाँचा <typename T>
काष्ठा KerxTable
{
  /* https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern */
  स्थिर T* thiz () स्थिर { वापस static_cast<स्थिर T *> (यह); }

  bool has_state_machine () स्थिर
  {
    प्रकार typename T::SubTable SubTable;

    स्थिर SubTable *st = &thiz()->firstSubTable;
    अचिन्हित पूर्णांक गिनती = thiz()->table_गिनती;
    क्रम (अचिन्हित पूर्णांक i = 0; i < गिनती; i++)
    {
      अगर (st->get_type () == 1)
	वापस true;

      // TODO: What about format 4? What's this API used for anyway?

      st = &StructAfter<SubTable> (*st);
    }
    वापस false;
  }

  bool has_cross_stream () स्थिर
  {
    प्रकार typename T::SubTable SubTable;

    स्थिर SubTable *st = &thiz()->firstSubTable;
    अचिन्हित पूर्णांक गिनती = thiz()->table_गिनती;
    क्रम (अचिन्हित पूर्णांक i = 0; i < गिनती; i++)
    {
      अगर (st->u.header.coverage & st->u.header.CrossStream)
	वापस true;
      st = &StructAfter<SubTable> (*st);
    }
    वापस false;
  }

  पूर्णांक get_h_kerning (hb_codepoint_t बाँए, hb_codepoint_t दाँए) स्थिर
  {
    प्रकार typename T::SubTable SubTable;

    पूर्णांक v = 0;
    स्थिर SubTable *st = &thiz()->firstSubTable;
    अचिन्हित पूर्णांक गिनती = thiz()->table_गिनती;
    क्रम (अचिन्हित पूर्णांक i = 0; i < गिनती; i++)
    {
      अगर ((st->u.header.coverage & (st->u.header.Variation | st->u.header.CrossStream)) ||
	  !st->u.header.is_horizontal ())
	जारी;
      v += st->get_kerning (बाँए, दाँए);
      st = &StructAfter<SubTable> (*st);
    }
    वापस v;
  }

  bool apply (AAT::hb_aat_apply_context_t *c,
	      स्थिर kern_accelerator_तथ्य_t &accel_तथ्य) स्थिर
  {
    c->बफर->unsafe_to_concat ();

    c->setup_बफर_glyph_set ();

    प्रकार typename T::SubTable SubTable;

    bool ret = false;
    bool seenCrossStream = false;
    c->set_lookup_सूचकांक (0);
    स्थिर SubTable *st = &thiz()->firstSubTable;
    अचिन्हित पूर्णांक गिनती = thiz()->table_गिनती;
    क्रम (अचिन्हित पूर्णांक i = 0; i < गिनती; i++)
    {
      bool reverse;

      स्वतः &subtable_accel = accel_तथ्य.subtable_accels[i];

      अगर (!T::Types::extended && (st->u.header.coverage & st->u.header.Variation))
	जाओ skip;

      अगर (HB_DIRECTION_IS_HORIZONTAL (c->बफर->props.direction) != st->u.header.is_horizontal ())
	जाओ skip;

      c->first_set = &subtable_accel.first_set;
      c->second_set = &subtable_accel.second_set;
      c->machine_class_cache = &subtable_accel.class_cache;

      अगर (!c->बफर_intersects_machine ())
      {
	(व्योम) c->बफर->message (c->font, "skipped subtable %u because no glyph matches", c->lookup_सूचकांक);
	जाओ skip;
      }

      reverse = bool (st->u.header.coverage & st->u.header.Backwards) !=
		HB_DIRECTION_IS_BACKWARD (c->बफर->props.direction);

      अगर (!c->बफर->message (c->font, "start subtable %u", c->lookup_सूचकांक))
	जाओ skip;

      अगर (!seenCrossStream &&
	  (st->u.header.coverage & st->u.header.CrossStream))
      {
	/* Attach all glyphs into a chain. */
	seenCrossStream = true;
	hb_glyph_position_t *pos = c->बफर->pos;
	अचिन्हित पूर्णांक गिनती = c->बफर->len;
	क्रम (अचिन्हित पूर्णांक i = 0; i < गिनती; i++)
	{
	  pos[i].attach_type() = OT::Layout::GPOS_impl::ATTACH_TYPE_CURSIVE;
	  pos[i].attach_chain() = HB_DIRECTION_IS_FORWARD (c->बफर->props.direction) ? -1 : +1;
	  /* We intentionally don't set HB_BUFFER_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT,
	   * since there needs to be a non-zero attachment for post-positioning to
	   * be needed. */
	}
      }

      अगर (reverse != c->बफर_is_reversed)
        c->reverse_बफर ();

      {
	/* See comment in sanitize() for conditional here. */
	hb_sanitize_with_object_t with (&c->sanitizer, i < गिनती - 1 ? st : (स्थिर SubTable *) nullptr);
	ret |= st->dispatch (c);
      }

      (व्योम) c->बफर->message (c->font, "end subtable %u", c->lookup_सूचकांक);

    skip:
      st = &StructAfter<SubTable> (*st);
      c->set_lookup_सूचकांक (c->lookup_सूचकांक + 1);
    }
    अगर (c->बफर_is_reversed)
      c->reverse_बफर ();

    वापस ret;
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    अगर (unlikely (!(thiz()->version.sanitize (c) &&
		    hb_barrier () &&
		    (अचिन्हित) thiz()->version >= (अचिन्हित) T::minVersion &&
		    thiz()->table_गिनती.sanitize (c))))
      return_trace (false);

    प्रकार typename T::SubTable SubTable;

    स्थिर SubTable *st = &thiz()->firstSubTable;
    अचिन्हित पूर्णांक गिनती = thiz()->table_गिनती;
    क्रम (अचिन्हित पूर्णांक i = 0; i < गिनती; i++)
    {
      अगर (unlikely (!st->u.header.sanitize (c)))
	return_trace (false);
      hb_barrier ();
      /* OpenType kern table has 2-byte subtable lengths.  That's limiting.
       * MS implementation also only supports one subtable, of format 0,
       * anyway.  Certain versions of some fonts, like Calibry, contain
       * kern subtable that exceeds 64kb.  Looks like, the subtable length
       * is simply ignored.  Which makes sense.  It's only needed if you
       * have multiple subtables.  To handle such fonts, we just ignore
       * the length for the last subtable. */
      hb_sanitize_with_object_t with (c, i < गिनती - 1 ? st : (स्थिर SubTable *) nullptr);

      अगर (unlikely (!st->sanitize (c)))
	return_trace (false);

      st = &StructAfter<SubTable> (*st);
    }

    अचिन्हित majorVersion = thiz()->version;
    अगर (माप (thiz()->version) == 4)
      majorVersion = majorVersion >> 16;
    अगर (majorVersion >= 3)
    {
      स्थिर SubtableGlyphCoverage *coverage = (स्थिर SubtableGlyphCoverage *) st;
      अगर (!coverage->sanitize (c, गिनती))
        return_trace (false);
    }

    return_trace (true);
  }

  kern_accelerator_तथ्य_t create_accelerator_तथ्य (अचिन्हित अंक_glyphs) स्थिर
  {
    kern_accelerator_तथ्य_t accel_तथ्य;

    प्रकार typename T::SubTable SubTable;

    स्थिर SubTable *st = &thiz()->firstSubTable;
    अचिन्हित पूर्णांक गिनती = thiz()->table_गिनती;
    क्रम (अचिन्हित पूर्णांक i = 0; i < गिनती; i++)
    {
      स्वतः &subtable_accel = *accel_तथ्य.subtable_accels.push ();
      अगर (unlikely (accel_तथ्य.subtable_accels.in_error ()))
	  वापस accel_तथ्य;

      st->collect_glyphs (subtable_accel.first_set, subtable_accel.second_set, अंक_glyphs);
      subtable_accel.class_cache.सफा ();

      st = &StructAfter<SubTable> (*st);
    }

    वापस accel_तथ्य;
  }

  काष्ठा accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      hb_sanitize_context_t sc;
      यह->table = sc.reference_table<T> (face);
      यह->accel_तथ्य = यह->table->create_accelerator_तथ्य (face->get_अंक_glyphs ());
    }
    ~accelerator_t ()
    {
      यह->table.destroy ();
    }

    hb_blob_t *get_blob () स्थिर { वापस table.get_blob (); }

    bool apply (AAT::hb_aat_apply_context_t *c) स्थिर
    {
      वापस table->apply (c, accel_तथ्य);
    }

    hb_blob_ptr_t<T> table;
    kern_accelerator_तथ्य_t accel_तथ्य;
    hb_aat_scratch_t scratch;
  };
};

काष्ठा kerx : KerxTable<kerx>
{
  मित्र काष्ठा KerxTable<kerx>;

  अटल constexpr hb_tag_t tableTag = HB_AAT_TAG_kerx;
  अटल constexpr अचिन्हित minVersion = 2u;

  प्रकार KerxSubTableHeader SubTableHeader;
  प्रकार SubTableHeader::Types Types;
  प्रकार KerxSubTable SubTable;

  bool has_तथ्य () स्थिर { वापस version; }

  रक्षित:
  HBUINT16	version;	/* The version number of the extended kerning table
				 * (currently 2, 3, or 4). */
  HBUINT16	unused;		/* Set to 0. */
  HBUINT32	table_गिनती;	/* The number of subtables included in the extended kerning
				 * table. */
  SubTable	firstSubTable;	/* Subtables. */
/*subtableGlyphCoverageArray*/	/* Only if version >= 3. We don't use. */

  खुला:
  DEFINE_आकार_MIN (8);
};

काष्ठा kerx_accelerator_t : kerx::accelerator_t {
  kerx_accelerator_t (hb_face_t *face) : kerx::accelerator_t (face) {}
};

} /* namespace AAT */

#पूर्ण_अगर /* HB_AAT_LAYOUT_KERX_TABLE_HH */
