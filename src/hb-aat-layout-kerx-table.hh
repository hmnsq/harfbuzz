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

#અન્તર્ભાવ "hb-kern.hh"
#અન્તર્ભાવ "hb-aat-layout-ankr-table.hh"
#અન્તર્ભાવ "hb-set-digest.hh"

/*
 * kerx -- Extended Kerning
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6kerx.html
 */
#define HB_AAT_TAG_kerx HB_TAG('k','e','r','x')


નામાકાશ AAT {

using નામાકાશ OT;


સ્થિર અંતઃરેખ પૂર્ણાંક
kerxTupleKern (પૂર્ણાંક value,
	       unsigned પૂર્ણાંક tuple_ગણતરી,
	       અચલ શૂન્ય *base,
	       hb_aat_apply_context_t *c)
{
  જો (likely (!tuple_ગણતરી || !c)) પાછા value;

  unsigned પૂર્ણાંક offset = value;
  અચલ FWORD *pv = &StructAtOffset<FWORD> (base, offset);
  જો (unlikely (!c->sanitizer.check_array (pv, tuple_ગણતરી))) પાછા 0;
  hb_barrier ();
  પાછા *pv;
}


સંરચના hb_glyph_pair_t
{
  hb_codepoint_t left;
  hb_codepoint_t જમણું;
};

સંરચના KernPair
{
  પૂર્ણાંક get_kerning () અચલ { પાછા value; }

  પૂર્ણાંક cmp (અચલ hb_glyph_pair_t &o) અચલ
  {
    પૂર્ણાંક ret = left.cmp (o.left);
    જો (ret) પાછા ret;
    પાછા જમણું.cmp (o.જમણું);
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ));
  }

  જાહેર:
  HBGlyphID16	left;
  HBGlyphID16	જમણું;
  FWORD		value;
  જાહેર:
  DEFINE_SIZE_STATIC (6);
};

template <typename KernSubTableHeader>
સંરચના KerxSubTableFormat0
{
  પૂર્ણાંક get_kerning (hb_codepoint_t left, hb_codepoint_t જમણું,
		   hb_aat_apply_context_t *c = nullptr) અચલ
  {
    hb_glyph_pair_t pair = {left, જમણું};
    પૂર્ણાંક v = pairs.bsearch (pair).get_kerning ();
    પાછા kerxTupleKern (v, header.tuple_ગણતરી_2 (), આ, c);
  }

  તાર્કિક apply (hb_aat_apply_context_t *c) અચલ
  {
    TRACE_APPLY (આ);

    જો (!c->plan->requested_kerning)
      return_trace (ખોટું);

    જો (header.coverage & header.Backwards)
      return_trace (ખોટું);

    accelerator_t accel (*આ, c);
    hb_kern_machine_t<accelerator_t> machine (accel, header.coverage & header.CrossStream);
    machine.kern (c->font, c->buffer, c->plan->kern_mask);

    return_trace (સાચું);
  }

  template <typename set_t>
  શૂન્ય collect_glyphs (set_t &first_set, set_t &second_set, unsigned અંક_glyphs) અચલ
  {
    માટે (અચલ KernPair& pair : pairs)
    {
      first_set.add (pair.left);
      second_set.add (pair.જમણું);
    }
  }

  સંરચના accelerator_t
  {
    અચલ KerxSubTableFormat0 &table;
    hb_aat_apply_context_t *c;

    accelerator_t (અચલ KerxSubTableFormat0 &table_,
		   hb_aat_apply_context_t *c_) :
		     table (table_), c (c_) {}

    પૂર્ણાંક get_kerning (hb_codepoint_t left, hb_codepoint_t જમણું) અચલ
    {
      જો (!(*c->first_set)[left] || !(*c->second_set)[જમણું]) પાછા 0;
      પાછા table.get_kerning (left, જમણું, c);
    }
  };


  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (likely (pairs.sanitize (c)));
  }

  સુરક્ષિત:
  KernSubTableHeader	header;
  Bin_શોધ_Array_Of<KernPair, typename KernSubTableHeader::Types::HBUINT>
			pairs;	/* Sorted kern records. */
  જાહેર:
  DEFINE_SIZE_ARRAY (KernSubTableHeader::static_size + 16, pairs);
};


template <તાર્કિક extended>
સંરચના Format1Entry;

template <>
સંરચના Format1Entry<સાચું>
{
  ગણના Flags
  {
    ધકેલો		= 0x8000,	/* If set, push this glyph on the kerning stack. */
    DontAdvance		= 0x4000,	/* If set, don't advance to the next glyph
					 * before going to the new state. */
    Reset		= 0x2000,	/* If set, reset the kerning data (clear the stack) */
    Reserved		= 0x1FFF,	/* Not used; set to 0. */
  };

  સંરચના EntryData
  {
    HBUINT16	kernActionIndex;/* Index into the kerning value array. If
				 * this index is 0xFFFF, then no kerning
				 * is to be performed. */
    જાહેર:
    DEFINE_SIZE_STATIC (2);
  };

  સ્થિર તાર્કિક initiateAction (અચલ Entry<EntryData> &entry)
  { પાછા entry.flags & ધકેલો; }

  સ્થિર તાર્કિક performAction (અચલ Entry<EntryData> &entry)
  { પાછા entry.data.kernActionIndex != 0xFFFF; }

  સ્થિર unsigned પૂર્ણાંક kernActionIndex (અચલ Entry<EntryData> &entry)
  { પાછા entry.data.kernActionIndex; }
};
template <>
સંરચના Format1Entry<ખોટું>
{
  ગણના Flags
  {
    ધકેલો		= 0x8000,	/* If set, push this glyph on the kerning stack. */
    DontAdvance		= 0x4000,	/* If set, don't advance to the next glyph
					 * before going to the new state. */
    Offset		= 0x3FFF,	/* Byte offset from beginning of subtable to the
					 * value table for the glyphs on the kerning stack. */

    Reset		= 0x0000,	/* Not supported? */
  };

  પ્રકારવ્યાખ્યા શૂન્ય EntryData;

  સ્થિર તાર્કિક initiateAction (અચલ Entry<EntryData> &entry)
  { પાછા entry.flags & ધકેલો; }

  સ્થિર તાર્કિક performAction (અચલ Entry<EntryData> &entry)
  { પાછા entry.flags & Offset; }

  સ્થિર unsigned પૂર્ણાંક kernActionIndex (અચલ Entry<EntryData> &entry)
  { પાછા entry.flags & Offset; }
};

template <typename KernSubTableHeader>
સંરચના KerxSubTableFormat1
{
  પ્રકારવ્યાખ્યા typename KernSubTableHeader::Types Types;
  પ્રકારવ્યાખ્યા typename Types::HBUINT HBUINT;

  પ્રકારવ્યાખ્યા Format1Entry<Types::extended> Format1EntryT;
  પ્રકારવ્યાખ્યા typename Format1EntryT::EntryData EntryData;

  ગણના Flags
  {
    DontAdvance	= Format1EntryT::DontAdvance,
  };

  તાર્કિક is_action_initiable (અચલ Entry<EntryData> &entry) અચલ
  {
    પાછા Format1EntryT::initiateAction (entry);
  }
  તાર્કિક is_actionable (અચલ Entry<EntryData> &entry) અચલ
  {
    પાછા Format1EntryT::performAction (entry);
  }

  સંરચના driver_context_t
  {
    સ્થિર constexpr તાર્કિક in_place = સાચું;

    driver_context_t (અચલ KerxSubTableFormat1 *table_,
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
    શૂન્ય transition (hb_buffer_t *buffer,
		     StateTableDriver<Types, EntryData, Flags> *driver,
		     અચલ Entry<EntryData> &entry)
    {
      unsigned પૂર્ણાંક flags = entry.flags;

      જો (flags & Format1EntryT::Reset)
	depth = 0;

      જો (flags & Format1EntryT::ધકેલો)
      {
	જો (likely (depth < ARRAY_LENGTH (ઢગલો)))
	  ઢગલો[depth++] = buffer->idx;
	નહીંતર
	  depth = 0; /* Probably not what CoreText does, but better? */
      }

      જો (Format1EntryT::performAction (entry) && depth)
      {
	unsigned પૂર્ણાંક tuple_ગણતરી_2 = hb_max (1u, table->header.tuple_ગણતરી_2 ());

	unsigned પૂર્ણાંક kern_idx = Format1EntryT::kernActionIndex (entry);
	kern_idx = Types::byteOffsetToIndex (kern_idx, &table->machine, kernAction.arrayZ);
	અચલ FWORD *actions = &kernAction[kern_idx];
	જો (!c->sanitizer.check_array (actions, depth, tuple_ગણતરી_2))
	{
	  depth = 0;
	  પાછા;
	}
	hb_barrier ();

	hb_mask_t kern_mask = c->plan->kern_mask;

	/* From Apple 'kern' spec:
	 * "Each pops one glyph from the kerning stack and applies the kerning value to it.
	 * The end of the list is marked by an odd value... */
	તાર્કિક last = ખોટું;
	જ્યારે (!last && depth)
	{
	  unsigned પૂર્ણાંક idx = ઢગલો[--depth];
	  પૂર્ણાંક v = *actions;
	  actions += tuple_ગણતરી_2;
	  જો (idx >= buffer->len) ચાલુ;

	  /* "The end of the list is marked by an odd value..." */
	  last = v & 1;
	  v &= ~1;

	  hb_glyph_position_t &o = buffer->pos[idx];

	  જો (HB_DIRECTION_IS_HORIZONTAL (buffer->props.direction))
	  {
	    જો (crossStream)
	    {
	      /* The following flag is undocumented in the spec, but described
	       * in the 'kern' table example. */
	      જો (v == -0x8000)
	      {
		o.attach_type() = OT::Layout::GPOS_impl::ATTACH_TYPE_NONE;
		o.attach_chain() = 0;
		o.y_offset = 0;
	      }
	      નહીંતર જો (o.attach_type())
	      {
		o.y_offset = hb_saturate_add (o.y_offset, c->font->em_scale_y (v));
		buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT;
	      }
	    }
	    નહીંતર જો (buffer->info[idx].mask & kern_mask)
	    {
	      auto scaled = c->font->em_scale_x (v);
	      o.x_advance = hb_saturate_add (o.x_advance, scaled);
	      o.x_offset = hb_saturate_add (o.x_offset, scaled);
	    }
	  }
	  નહીંતર
	  {
	    જો (crossStream)
	    {
	      /* CoreText doesn't do crossStream kerning in vertical.  We do. */
	      જો (v == -0x8000)
	      {
		o.attach_type() = OT::Layout::GPOS_impl::ATTACH_TYPE_NONE;
		o.attach_chain() = 0;
		o.x_offset = 0;
	      }
	      નહીંતર જો (o.attach_type())
	      {
		o.x_offset = hb_saturate_add (o.x_offset, c->font->em_scale_x (v));
		buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT;
	      }
	    }
	    નહીંતર જો (buffer->info[idx].mask & kern_mask)
	    {
	      auto scaled = c->font->em_scale_y (v);
	      o.y_advance = hb_saturate_add (o.y_advance, scaled);
	      o.y_offset = hb_saturate_add (o.y_offset, scaled);
	    }
	  }
	}
      }
    }

    જાહેર:
    hb_aat_apply_context_t *c;
    અચલ KerxSubTableFormat1 *table;
    ખાનગી:
    અચલ UnsizedArrayOf<FWORD> &kernAction;
    unsigned પૂર્ણાંક ઢગલો[8];
    unsigned પૂર્ણાંક depth;
    તાર્કિક crossStream;
  };

  તાર્કિક apply (hb_aat_apply_context_t *c) અચલ
  {
    TRACE_APPLY (આ);

    જો (!c->plan->requested_kerning &&
	!(header.coverage & header.CrossStream))
      પાછા ખોટું;

    driver_context_t dc (આ, c);

    StateTableDriver<Types, EntryData, Flags> driver (machine, c->font->face);

    driver.drive (&dc, c);

    return_trace (સાચું);
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    /* The rest of array sanitizations are done at run-time. */
    return_trace (likely (c->check_struct (આ) &&
			  machine.sanitize (c)));
  }

  template <typename set_t>
  શૂન્ય collect_glyphs (set_t &first_set, set_t &second_set, unsigned અંક_glyphs) અચલ
  {
    machine.collect_initial_glyphs (first_set, અંક_glyphs, *આ);
    //machine.collect_glyphs (second_set, num_glyphs); // second_set is unused for machine kerning
  }

  સુરક્ષિત:
  KernSubTableHeader				header;
  StateTable<Types, EntryData>			machine;
  NNOffsetTo<UnsizedArrayOf<FWORD>, HBUINT>	kernAction;
  જાહેર:
  DEFINE_SIZE_STATIC (KernSubTableHeader::static_size + (StateTable<Types, EntryData>::static_size + HBUINT::static_size));
};

template <typename KernSubTableHeader>
સંરચના KerxSubTableFormat2
{
  પ્રકારવ્યાખ્યા typename KernSubTableHeader::Types Types;
  પ્રકારવ્યાખ્યા typename Types::HBUINT HBUINT;

  પૂર્ણાંક get_kerning (hb_codepoint_t left, hb_codepoint_t જમણું,
		   hb_aat_apply_context_t *c) અચલ
  {
    unsigned પૂર્ણાંક અંક_glyphs = c->sanitizer.get_અંક_glyphs ();
    unsigned પૂર્ણાંક l = (આ+ડાબું_Class_Table).get_class (left, અંક_glyphs, 0);
    unsigned પૂર્ણાંક r = (આ+જમણું_Class_Table).get_class (જમણું, અંક_glyphs, 0);

    અચલ UnsizedArrayOf<FWORD> &arrayZ = આ+array;
    unsigned પૂર્ણાંક kern_idx = l + r;
    kern_idx = Types::offsetToIndex (kern_idx, આ, arrayZ.arrayZ);
    અચલ FWORD *v = &arrayZ[kern_idx];
    જો (unlikely (!v->sanitize (&c->sanitizer))) પાછા 0;
    hb_barrier ();

    પાછા kerxTupleKern (*v, header.tuple_ગણતરી_2 (), આ, c);
  }

  તાર્કિક apply (hb_aat_apply_context_t *c) અચલ
  {
    TRACE_APPLY (આ);

    જો (!c->plan->requested_kerning)
      return_trace (ખોટું);

    જો (header.coverage & header.Backwards)
      return_trace (ખોટું);

    accelerator_t accel (*આ, c);
    hb_kern_machine_t<accelerator_t> machine (accel, header.coverage & header.CrossStream);
    machine.kern (c->font, c->buffer, c->plan->kern_mask);

    return_trace (સાચું);
  }

  template <typename set_t>
  શૂન્ય collect_glyphs (set_t &first_set, set_t &second_set, unsigned અંક_glyphs) અચલ
  {
    (આ+ડાબું_Class_Table).collect_glyphs (first_set, અંક_glyphs);
    (આ+જમણું_Class_Table).collect_glyphs (second_set, અંક_glyphs);
  }

  સંરચના accelerator_t
  {
    અચલ KerxSubTableFormat2 &table;
    hb_aat_apply_context_t *c;

    accelerator_t (અચલ KerxSubTableFormat2 &table_,
		   hb_aat_apply_context_t *c_) :
		     table (table_), c (c_) {}

    પૂર્ણાંક get_kerning (hb_codepoint_t left, hb_codepoint_t જમણું) અચલ
    {
      જો (!(*c->first_set)[left] || !(*c->second_set)[જમણું]) પાછા 0;
      પાછા table.get_kerning (left, જમણું, c);
    }
  };

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (likely (c->check_struct (આ) &&
			  ડાબું_Class_Table.sanitize (c, આ) &&
			  જમણું_Class_Table.sanitize (c, આ) &&
			  hb_barrier () &&
			  c->check_range (આ, array)));
  }

  સુરક્ષિત:
  KernSubTableHeader	header;
  HBUINT		rowWidth;	/* The width, in bytes, of a row in the table. */
  NNOffsetTo<typename Types::ClassTypeWide, HBUINT>
			ડાબું_Class_Table;	/* Offset from beginning of this subtable to
					 * left-hand class table. */
  NNOffsetTo<typename Types::ClassTypeWide, HBUINT>
			જમણું_Class_Table;/* Offset from beginning of this subtable to
					 * right-hand class table. */
  NNOffsetTo<UnsizedArrayOf<FWORD>, HBUINT>
			 array;		/* Offset from beginning of this subtable to
					 * the start of the kerning array. */
  જાહેર:
  DEFINE_SIZE_STATIC (KernSubTableHeader::static_size + 4 * કદ (HBUINT));
};

template <typename KernSubTableHeader>
સંરચના KerxSubTableFormat4
{
  પ્રકારવ્યાખ્યા ExtendedTypes Types;

  સંરચના EntryData
  {
    HBUINT16	ankrActionIndex;/* Either 0xFFFF (for no action) or the index of
				 * the action to perform. */
    જાહેર:
    DEFINE_SIZE_STATIC (2);
  };

  ગણના Flags
  {
    Mark		= 0x8000,	/* If set, remember this glyph as the marked glyph. */
    DontAdvance		= 0x4000,	/* If set, don't advance to the next glyph before
					 * going to the new state. */
    Reserved		= 0x3FFF,	/* Not used; set to 0. */
  };

  તાર્કિક is_action_initiable (અચલ Entry<EntryData> &entry) અચલ
  {
    પાછા (entry.flags & Mark);
  }
  તાર્કિક is_actionable (અચલ Entry<EntryData> &entry) અચલ
  {
    પાછા entry.data.ankrActionIndex != 0xFFFF;
  }

  સંરચના driver_context_t
  {
    સ્થિર constexpr તાર્કિક in_place = સાચું;
    ગણના SubTableFlags
    {
      ActionType	= 0xC0000000,	/* A two-bit field containing the action type. */
      Unused		= 0x3F000000,	/* Unused - must be zero. */
      Offset		= 0x00FFFFFF,	/* Masks the offset in bytes from the beginning
					 * of the subtable to the beginning of the control
					 * point table. */
    };

    driver_context_t (અચલ KerxSubTableFormat4 *table_,
		      hb_aat_apply_context_t *c_) :
	c (c_),
	table (table_),
	action_type ((table->flags & ActionType) >> 30),
	ankrData ((HBUINT16 *) ((અચલ અક્ષર *) &table->machine + (table->flags & Offset))),
	mark_set (ખોટું),
	mark (0) {}

    HB_AAT_TRANSITION_INLINE
    શૂન્ય transition (hb_buffer_t *buffer,
		     StateTableDriver<Types, EntryData, Flags> *driver,
		     અચલ Entry<EntryData> &entry)
    {
      જો (mark_set && entry.data.ankrActionIndex != 0xFFFF && buffer->idx < buffer->len)
      {
	hb_glyph_position_t &o = buffer->cur_pos();
	પસંદગી (action_type)
	{
	  સ્થિતિ 0: /* Control Point Actions.*/
	  {
	    /* Indexed into glyph outline. */
	    /* Each action (record in ankrData) contains two 16-bit fields, so we must
	       double the ankrActionIndex to get the correct offset here. */
	    અચલ HBUINT16 *data = &ankrData[entry.data.ankrActionIndex * 2];
	    જો (!c->sanitizer.check_array (data, 2)) પાછા;
	    hb_barrier ();
	    unsigned પૂર્ણાંક markControlPoint = *data++;
	    unsigned પૂર્ણાંક currControlPoint = *data++;
	    hb_position_t markX = 0;
	    hb_position_t markY = 0;
	    hb_position_t currX = 0;
	    hb_position_t currY = 0;
	    જો (!c->font->get_glyph_contour_point_for_origin (c->buffer->info[mark].codepoint,
							      markControlPoint,
							      HB_DIRECTION_LTR /*XXX*/,
							      &markX, &markY) ||
		!c->font->get_glyph_contour_point_for_origin (c->buffer->cur ().codepoint,
							      currControlPoint,
							      HB_DIRECTION_LTR /*XXX*/,
							      &currX, &currY))
	      પાછા;

	    o.x_offset = hb_saturate_sub (markX, currX);
	    o.y_offset = hb_saturate_sub (markY, currY);
	  }
	  તોડો;

	  સ્થિતિ 1: /* Anchor Point Actions. */
	  {
	    /* Indexed into 'ankr' table. */
	    /* Each action (record in ankrData) contains two 16-bit fields, so we must
	       double the ankrActionIndex to get the correct offset here. */
	    અચલ HBUINT16 *data = &ankrData[entry.data.ankrActionIndex * 2];
	    જો (!c->sanitizer.check_array (data, 2)) પાછા;
	    hb_barrier ();
	    unsigned પૂર્ણાંક markAnchorPoint = *data++;
	    unsigned પૂર્ણાંક currAnchorPoint = *data++;
	    અચલ Anchor &markAnchor = c->ankr_table->get_anchor (c->buffer->info[mark].codepoint,
								  markAnchorPoint,
								  c->sanitizer.get_અંક_glyphs ());
	    અચલ Anchor &currAnchor = c->ankr_table->get_anchor (c->buffer->cur ().codepoint,
								  currAnchorPoint,
								  c->sanitizer.get_અંક_glyphs ());

	    o.x_offset = hb_saturate_sub (c->font->em_scale_x (markAnchor.xCoordinate),
					  c->font->em_scale_x (currAnchor.xCoordinate));
	    o.y_offset = hb_saturate_sub (c->font->em_scale_y (markAnchor.yCoordinate),
					  c->font->em_scale_y (currAnchor.yCoordinate));
	  }
	  તોડો;

	  સ્થિતિ 2: /* Control Point Coordinate Actions. */
	  {
	    /* Each action contains four 16-bit fields, so we multiply the ankrActionIndex
	       by 4 to get the correct offset for the given action. */
	    અચલ FWORD *data = (અચલ FWORD *) &ankrData[entry.data.ankrActionIndex * 4];
	    જો (!c->sanitizer.check_array (data, 4)) પાછા;
	    hb_barrier ();
	    પૂર્ણાંક markX = *data++;
	    પૂર્ણાંક markY = *data++;
	    પૂર્ણાંક currX = *data++;
	    પૂર્ણાંક currY = *data++;

	    o.x_offset = hb_saturate_sub (c->font->em_scale_x (markX), c->font->em_scale_x (currX));
	    o.y_offset = hb_saturate_sub (c->font->em_scale_y (markY), c->font->em_scale_y (currY));
	  }
	  તોડો;
	}
	o.attach_type() = OT::Layout::GPOS_impl::ATTACH_TYPE_MARK;
	o.attach_chain() = (પૂર્ણાંક) mark - (પૂર્ણાંક) buffer->idx;
	જો (c->buffer_is_reversed)
	  o.attach_chain() = -o.attach_chain();
	buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT;
      }

      જો (entry.flags & Mark)
      {
	mark_set = સાચું;
	mark = buffer->idx;
      }
    }

    જાહેર:
    hb_aat_apply_context_t *c;
    અચલ KerxSubTableFormat4 *table;
    ખાનગી:
    unsigned પૂર્ણાંક action_type;
    અચલ HBUINT16 *ankrData;
    તાર્કિક mark_set;
    unsigned પૂર્ણાંક mark;
  };

  તાર્કિક apply (hb_aat_apply_context_t *c) અચલ
  {
    TRACE_APPLY (આ);

    driver_context_t dc (આ, c);

    StateTableDriver<Types, EntryData, Flags> driver (machine, c->font->face);

    driver.drive (&dc, c);

    return_trace (સાચું);
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    /* The rest of array sanitizations are done at run-time. */
    return_trace (likely (c->check_struct (આ) &&
			  machine.sanitize (c)));
  }

  template <typename set_t>
  શૂન્ય collect_glyphs (set_t &first_set, set_t &second_set, unsigned અંક_glyphs) અચલ
  {
    machine.collect_initial_glyphs (first_set, અંક_glyphs, *આ);
    //machine.collect_glyphs (second_set, num_glyphs); // second_set is unused for machine kerning
  }

  સુરક્ષિત:
  KernSubTableHeader		header;
  StateTable<Types, EntryData>	machine;
  HBUINT32			flags;
  જાહેર:
  DEFINE_SIZE_STATIC (KernSubTableHeader::static_size + (StateTable<Types, EntryData>::static_size + HBUINT32::static_size));
};

template <typename KernSubTableHeader>
સંરચના KerxSubTableFormat6
{
  ગણના Flags
  {
    કિંમતો_Are_Long	= 0x00000001,
  };

  તાર્કિક is_long () અચલ { પાછા flags & કિંમતો_Are_Long; }

  પૂર્ણાંક get_kerning (hb_codepoint_t left, hb_codepoint_t જમણું,
		   hb_aat_apply_context_t *c) અચલ
  {
    unsigned પૂર્ણાંક અંક_glyphs = c->sanitizer.get_અંક_glyphs ();
    જો (is_long ())
    {
      અચલ auto &t = u.l;
      unsigned પૂર્ણાંક l = (આ+t.rowIndexTable).get_કિંમત_or_null (left, અંક_glyphs);
      unsigned પૂર્ણાંક r = (આ+t.columnIndexTable).get_કિંમત_or_null (જમણું, અંક_glyphs);
      unsigned પૂર્ણાંક offset = l + r;
      જો (unlikely (offset < l)) પાછા 0; /* Addition overflow. */
      જો (unlikely (hb_unsigned_mul_overflows (offset, કદ (FWORD32)))) પાછા 0;
      અચલ FWORD32 *v = &StructAtOffset<FWORD32> (&(આ+t.array), offset * કદ (FWORD32));
      જો (unlikely (!v->sanitize (&c->sanitizer))) પાછા 0;
      hb_barrier ();
      પાછા kerxTupleKern (*v, header.tuple_ગણતરી_2 (), &(આ+vector), c);
    }
    નહીંતર
    {
      અચલ auto &t = u.s;
      unsigned પૂર્ણાંક l = (આ+t.rowIndexTable).get_કિંમત_or_null (left, અંક_glyphs);
      unsigned પૂર્ણાંક r = (આ+t.columnIndexTable).get_કિંમત_or_null (જમણું, અંક_glyphs);
      unsigned પૂર્ણાંક offset = l + r;
      અચલ FWORD *v = &StructAtOffset<FWORD> (&(આ+t.array), offset * કદ (FWORD));
      જો (unlikely (!v->sanitize (&c->sanitizer))) પાછા 0;
      hb_barrier ();
      પાછા kerxTupleKern (*v, header.tuple_ગણતરી_2 (), &(આ+vector), c);
    }
  }

  તાર્કિક apply (hb_aat_apply_context_t *c) અચલ
  {
    TRACE_APPLY (આ);

    જો (!c->plan->requested_kerning)
      return_trace (ખોટું);

    જો (header.coverage & header.Backwards)
      return_trace (ખોટું);

    accelerator_t accel (*આ, c);
    hb_kern_machine_t<accelerator_t> machine (accel, header.coverage & header.CrossStream);
    machine.kern (c->font, c->buffer, c->plan->kern_mask);

    return_trace (સાચું);
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (likely (c->check_struct (આ) &&
			  hb_barrier () &&
			  (is_long () ?
			   (
			     u.l.rowIndexTable.sanitize (c, આ) &&
			     u.l.columnIndexTable.sanitize (c, આ) &&
			     c->check_range (આ, u.l.array)
			   ) : (
			     u.s.rowIndexTable.sanitize (c, આ) &&
			     u.s.columnIndexTable.sanitize (c, આ) &&
			     c->check_range (આ, u.s.array)
			   )) &&
			  (header.tuple_ગણતરી_2 () == 0 ||
			   c->check_range (આ, vector))));
  }

  template <typename set_t>
  શૂન્ય collect_glyphs (set_t &first_set, set_t &second_set, unsigned અંક_glyphs) અચલ
  {
    જો (is_long ())
    {
      અચલ auto &t = u.l;
      (આ+t.rowIndexTable).collect_glyphs (first_set, અંક_glyphs);
      (આ+t.columnIndexTable).collect_glyphs (second_set, અંક_glyphs);
    }
    નહીંતર
    {
      અચલ auto &t = u.s;
      (આ+t.rowIndexTable).collect_glyphs (first_set, અંક_glyphs);
      (આ+t.columnIndexTable).collect_glyphs (second_set, અંક_glyphs);
    }
  }

  સંરચના accelerator_t
  {
    અચલ KerxSubTableFormat6 &table;
    hb_aat_apply_context_t *c;

    accelerator_t (અચલ KerxSubTableFormat6 &table_,
		   hb_aat_apply_context_t *c_) :
		     table (table_), c (c_) {}

    પૂર્ણાંક get_kerning (hb_codepoint_t left, hb_codepoint_t જમણું) અચલ
    {
      જો (!(*c->first_set)[left] || !(*c->second_set)[જમણું]) પાછા 0;
      પાછા table.get_kerning (left, જમણું, c);
    }
  };

  સુરક્ષિત:
  KernSubTableHeader		header;
  HBUINT32			flags;
  HBUINT16			row_ગણતરી;
  HBUINT16			column_ગણતરી;
  સંઘ U
  {
    સંરચના Long
    {
      NNOffset32To<Lookup<HBUINT32>>		rowIndexTable;
      NNOffset32To<Lookup<HBUINT32>>		columnIndexTable;
      NNOffset32To<UnsizedArrayOf<FWORD32>>	array;
    } l;
    સંરચના Short
    {
      NNOffset32To<Lookup<HBUINT16>>		rowIndexTable;
      NNOffset32To<Lookup<HBUINT16>>		columnIndexTable;
      NNOffset32To<UnsizedArrayOf<FWORD>>	array;
    } s;
  } u;
  NNOffset32To<UnsizedArrayOf<FWORD>>	vector;
  જાહેર:
  DEFINE_SIZE_STATIC (KernSubTableHeader::static_size + 24);
};


સંરચના KerxSubTableHeader
{
  પ્રકારવ્યાખ્યા ExtendedTypes Types;

  unsigned   tuple_ગણતરી_2 () અચલ { પાછા tuple_ગણતરી; }
  તાર્કિક     is_horizontal () અચલ { પાછા !(coverage & Vertical); }

  ગણના Coverage
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

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ));
  }

  જાહેર:
  HBUINT32	length;
  HBUINT32	coverage;
  HBUINT32	tuple_ગણતરી;
  જાહેર:
  DEFINE_SIZE_STATIC (12);
};

સંરચના KerxSubTable
{
  friend સંરચના kerx;

  size_t get_size () અચલ { પાછા u.header.length; }
  unsigned પૂર્ણાંક get_type () અચલ { પાછા u.header.coverage & u.header.SubtableType; }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) અચલ
  {
    unsigned પૂર્ણાંક subtable_type = get_type ();
    TRACE_DISPATCH (આ, subtable_type);
    પસંદગી (subtable_type) {
    સ્થિતિ 0:	hb_barrier (); return_trace (c->dispatch (u.format0, std::forward<Ts> (ds)...));
    સ્થિતિ 1:	hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    સ્થિતિ 2:	hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    સ્થિતિ 4:	hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
    સ્થિતિ 6:	hb_barrier (); return_trace (c->dispatch (u.format6, std::forward<Ts> (ds)...));
    મૂળભૂત:	return_trace (c->default_return_કિંમત ());
    }
  }

  template <typename set_t>
  શૂન્ય collect_glyphs (set_t &first_set, set_t &second_set, unsigned અંક_glyphs) અચલ
  {
    unsigned પૂર્ણાંક subtable_type = get_type ();
    પસંદગી (subtable_type) {
    સ્થિતિ 0:	hb_barrier (); u.format0.collect_glyphs (first_set, second_set, અંક_glyphs); પાછા;
    સ્થિતિ 1:	hb_barrier (); u.format1.collect_glyphs (first_set, second_set, અંક_glyphs); પાછા;
    સ્થિતિ 2:	hb_barrier (); u.format2.collect_glyphs (first_set, second_set, અંક_glyphs); પાછા;
    સ્થિતિ 4:	hb_barrier (); u.format4.collect_glyphs (first_set, second_set, અંક_glyphs); પાછા;
    સ્થિતિ 6:	hb_barrier (); u.format6.collect_glyphs (first_set, second_set, અંક_glyphs); પાછા;
    મૂળભૂત:	પાછા;
    }
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    જો (!(u.header.sanitize (c) &&
	  hb_barrier () &&
	  u.header.length >= u.header.static_size &&
	  c->check_range (આ, u.header.length)))
      return_trace (ખોટું);

    return_trace (dispatch (c));
  }

  જાહેર:
  સંઘ {
  KerxSubTableHeader				header;
  KerxSubTableFormat0<KerxSubTableHeader>	format0;
  KerxSubTableFormat1<KerxSubTableHeader>	format1;
  KerxSubTableFormat2<KerxSubTableHeader>	format2;
  KerxSubTableFormat4<KerxSubTableHeader>	format4;
  KerxSubTableFormat6<KerxSubTableHeader>	format6;
  } u;
  જાહેર:
  DEFINE_SIZE_MIN (12);
};


/*
 * The 'kerx' Table
 */

સંરચના kern_subtable_accelerator_data_t
{
  hb_bit_set_t first_set;
  hb_bit_set_t second_set;
  mutable hb_aat_class_cache_t class_cache;
};

સંરચના kern_accelerator_data_t
{
  hb_vector_t<kern_subtable_accelerator_data_t> subtable_accels;
  hb_aat_scratch_t scratch;
};

template <typename T>
સંરચના KerxTable
{
  /* https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern */
  અચલ T* thiz () અચલ { પાછા static_cast<અચલ T *> (આ); }

  તાર્કિક has_state_machine () અચલ
  {
    પ્રકારવ્યાખ્યા typename T::SubTable SubTable;

    અચલ SubTable *st = &thiz()->firstSubTable;
    unsigned પૂર્ણાંક ગણતરી = thiz()->table_ગણતરી;
    માટે (unsigned પૂર્ણાંક i = 0; i < ગણતરી; i++)
    {
      જો (st->get_type () == 1)
	પાછા સાચું;

      // TODO: What about format 4? What's this API used for anyway?

      st = &StructAfter<SubTable> (*st);
    }
    પાછા ખોટું;
  }

  તાર્કિક has_cross_stream () અચલ
  {
    પ્રકારવ્યાખ્યા typename T::SubTable SubTable;

    અચલ SubTable *st = &thiz()->firstSubTable;
    unsigned પૂર્ણાંક ગણતરી = thiz()->table_ગણતરી;
    માટે (unsigned પૂર્ણાંક i = 0; i < ગણતરી; i++)
    {
      જો (st->u.header.coverage & st->u.header.CrossStream)
	પાછા સાચું;
      st = &StructAfter<SubTable> (*st);
    }
    પાછા ખોટું;
  }

  પૂર્ણાંક get_h_kerning (hb_codepoint_t left, hb_codepoint_t જમણું) અચલ
  {
    પ્રકારવ્યાખ્યા typename T::SubTable SubTable;

    પૂર્ણાંક v = 0;
    અચલ SubTable *st = &thiz()->firstSubTable;
    unsigned પૂર્ણાંક ગણતરી = thiz()->table_ગણતરી;
    માટે (unsigned પૂર્ણાંક i = 0; i < ગણતરી; i++)
    {
      જો ((st->u.header.coverage & (st->u.header.Variation | st->u.header.CrossStream)) ||
	  !st->u.header.is_horizontal ())
	ચાલુ;
      v += st->get_kerning (left, જમણું);
      st = &StructAfter<SubTable> (*st);
    }
    પાછા v;
  }

  તાર્કિક apply (AAT::hb_aat_apply_context_t *c,
	      અચલ kern_accelerator_data_t &accel_data) અચલ
  {
    c->buffer->unsafe_to_concat ();

    c->setup_buffer_glyph_set ();

    પ્રકારવ્યાખ્યા typename T::SubTable SubTable;

    તાર્કિક ret = ખોટું;
    તાર્કિક seenCrossStream = ખોટું;
    c->set_lookup_index (0);
    અચલ SubTable *st = &thiz()->firstSubTable;
    unsigned પૂર્ણાંક ગણતરી = thiz()->table_ગણતરી;
    માટે (unsigned પૂર્ણાંક i = 0; i < ગણતરી; i++)
    {
      તાર્કિક reverse;

      auto &subtable_accel = accel_data.subtable_accels[i];

      જો (!T::Types::extended && (st->u.header.coverage & st->u.header.Variation))
	જાઓ skip;

      જો (HB_DIRECTION_IS_HORIZONTAL (c->buffer->props.direction) != st->u.header.is_horizontal ())
	જાઓ skip;

      c->first_set = &subtable_accel.first_set;
      c->second_set = &subtable_accel.second_set;
      c->machine_class_cache = &subtable_accel.class_cache;

      જો (!c->buffer_intersects_machine ())
      {
	(શૂન્ય) c->buffer->message (c->font, "skipped subtable %u because no glyph matches", c->lookup_index);
	જાઓ skip;
      }

      reverse = તાર્કિક (st->u.header.coverage & st->u.header.Backwards) !=
		HB_DIRECTION_IS_BACKWARD (c->buffer->props.direction);

      જો (!c->buffer->message (c->font, "start subtable %u", c->lookup_index))
	જાઓ skip;

      જો (!seenCrossStream &&
	  (st->u.header.coverage & st->u.header.CrossStream))
      {
	/* Attach all glyphs into a chain. */
	seenCrossStream = સાચું;
	hb_glyph_position_t *pos = c->buffer->pos;
	unsigned પૂર્ણાંક ગણતરી = c->buffer->len;
	માટે (unsigned પૂર્ણાંક i = 0; i < ગણતરી; i++)
	{
	  pos[i].attach_type() = OT::Layout::GPOS_impl::ATTACH_TYPE_CURSIVE;
	  pos[i].attach_chain() = HB_DIRECTION_IS_FORWARD (c->buffer->props.direction) ? -1 : +1;
	  /* We intentionally don't set HB_BUFFER_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT,
	   * since there needs to be a non-zero attachment for post-positioning to
	   * be needed. */
	}
      }

      જો (reverse != c->buffer_is_reversed)
        c->reverse_buffer ();

      {
	/* See comment in sanitize() for conditional here. */
	hb_sanitize_with_object_t with (&c->sanitizer, i < ગણતરી - 1 ? st : (અચલ SubTable *) nullptr);
	ret |= st->dispatch (c);
      }

      (શૂન્ય) c->buffer->message (c->font, "end subtable %u", c->lookup_index);

    skip:
      st = &StructAfter<SubTable> (*st);
      c->set_lookup_index (c->lookup_index + 1);
    }
    જો (c->buffer_is_reversed)
      c->reverse_buffer ();

    પાછા ret;
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    જો (unlikely (!(thiz()->version.sanitize (c) &&
		    hb_barrier () &&
		    (unsigned) thiz()->version >= (unsigned) T::minVersion &&
		    thiz()->table_ગણતરી.sanitize (c))))
      return_trace (ખોટું);

    પ્રકારવ્યાખ્યા typename T::SubTable SubTable;

    અચલ SubTable *st = &thiz()->firstSubTable;
    unsigned પૂર્ણાંક ગણતરી = thiz()->table_ગણતરી;
    માટે (unsigned પૂર્ણાંક i = 0; i < ગણતરી; i++)
    {
      જો (unlikely (!st->u.header.sanitize (c)))
	return_trace (ખોટું);
      hb_barrier ();
      /* OpenType kern table has 2-byte subtable lengths.  That's limiting.
       * MS implementation also only supports one subtable, of format 0,
       * anyway.  Certain versions of some fonts, like Calibry, contain
       * kern subtable that exceeds 64kb.  Looks like, the subtable length
       * is simply ignored.  Which makes sense.  It's only needed if you
       * have multiple subtables.  To handle such fonts, we just ignore
       * the length for the last subtable. */
      hb_sanitize_with_object_t with (c, i < ગણતરી - 1 ? st : (અચલ SubTable *) nullptr);

      જો (unlikely (!st->sanitize (c)))
	return_trace (ખોટું);

      st = &StructAfter<SubTable> (*st);
    }

    unsigned majorVersion = thiz()->version;
    જો (કદ (thiz()->version) == 4)
      majorVersion = majorVersion >> 16;
    જો (majorVersion >= 3)
    {
      અચલ SubtableGlyphCoverage *coverage = (અચલ SubtableGlyphCoverage *) st;
      જો (!coverage->sanitize (c, ગણતરી))
        return_trace (ખોટું);
    }

    return_trace (સાચું);
  }

  kern_accelerator_data_t create_accelerator_data (unsigned અંક_glyphs) અચલ
  {
    kern_accelerator_data_t accel_data;

    પ્રકારવ્યાખ્યા typename T::SubTable SubTable;

    અચલ SubTable *st = &thiz()->firstSubTable;
    unsigned પૂર્ણાંક ગણતરી = thiz()->table_ગણતરી;
    માટે (unsigned પૂર્ણાંક i = 0; i < ગણતરી; i++)
    {
      auto &subtable_accel = *accel_data.subtable_accels.push ();
      જો (unlikely (accel_data.subtable_accels.in_error ()))
	  પાછા accel_data;

      st->collect_glyphs (subtable_accel.first_set, subtable_accel.second_set, અંક_glyphs);
      subtable_accel.class_cache.clear ();

      st = &StructAfter<SubTable> (*st);
    }

    પાછા accel_data;
  }

  સંરચના accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      hb_sanitize_context_t sc;
      આ->table = sc.reference_table<T> (face);
      આ->accel_data = આ->table->create_accelerator_data (face->get_અંક_glyphs ());
    }
    ~accelerator_t ()
    {
      આ->table.destroy ();
    }

    hb_blob_t *get_blob () અચલ { પાછા table.get_blob (); }

    તાર્કિક apply (AAT::hb_aat_apply_context_t *c) અચલ
    {
      પાછા table->apply (c, accel_data);
    }

    hb_blob_ptr_t<T> table;
    kern_accelerator_data_t accel_data;
    hb_aat_scratch_t scratch;
  };
};

સંરચના kerx : KerxTable<kerx>
{
  friend સંરચના KerxTable<kerx>;

  સ્થિર constexpr hb_tag_t tableTag = HB_AAT_TAG_kerx;
  સ્થિર constexpr unsigned minVersion = 2u;

  પ્રકારવ્યાખ્યા KerxSubTableHeader SubTableHeader;
  પ્રકારવ્યાખ્યા SubTableHeader::Types Types;
  પ્રકારવ્યાખ્યા KerxSubTable SubTable;

  તાર્કિક has_data () અચલ { પાછા version; }

  સુરક્ષિત:
  HBUINT16	version;	/* The version number of the extended kerning table
				 * (currently 2, 3, or 4). */
  HBUINT16	unused;		/* Set to 0. */
  HBUINT32	table_ગણતરી;	/* The number of subtables included in the extended kerning
				 * table. */
  SubTable	firstSubTable;	/* Subtables. */
/*subtableGlyphCoverageArray*/	/* Only if version >= 3. We don't use. */

  જાહેર:
  DEFINE_SIZE_MIN (8);
};

સંરચના kerx_accelerator_t : kerx::accelerator_t {
  kerx_accelerator_t (hb_face_t *face) : kerx::accelerator_t (face) {}
};

} /* namespace AAT */

#endif /* HB_AAT_LAYOUT_KERX_TABLE_HH */
