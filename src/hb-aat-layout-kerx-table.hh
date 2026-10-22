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

#அந்தர்பாவ "hb-kern.hh"
#அந்தர்பாவ "hb-aat-layout-ankr-table.hh"
#அந்தர்பாவ "hb-set-digest.hh"

/*
 * kerx -- Extended Kerning
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6kerx.html
 */
#define HB_AAT_TAG_kerx HB_TAG('k','e','r','x')


நாமாகாஶ AAT {

using நாமாகாஶ OT;


நிலையான உள்வரி முழு
kerxTupleKern (முழு value,
	       unsigned முழு tuple_மொத்தம்,
	       மாறிலி வெறுமை *base,
	       hb_aat_apply_context_t *c)
{
  எனில் (likely (!tuple_மொத்தம் || !c)) திரும்பு value;

  unsigned முழு offset = value;
  மாறிலி FWORD *pv = &StructAtOffset<FWORD> (base, offset);
  எனில் (unlikely (!c->sanitizer.check_array (pv, tuple_மொத்தம்))) திரும்பு 0;
  hb_barrier ();
  திரும்பு *pv;
}


கட்டமைப்பு hb_glyph_pair_t
{
  hb_codepoint_t left;
  hb_codepoint_t வலது;
};

கட்டமைப்பு KernPair
{
  முழு get_kerning () மாறிலி { திரும்பு value; }

  முழு cmp (மாறிலி hb_glyph_pair_t &o) மாறிலி
  {
    முழு ret = left.cmp (o.left);
    எனில் (ret) திரும்பு ret;
    திரும்பு வலது.cmp (o.வலது);
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது));
  }

  பொது:
  HBGlyphID16	left;
  HBGlyphID16	வலது;
  FWORD		value;
  பொது:
  DEFINE_SIZE_STATIC (6);
};

template <typename KernSubTableHeader>
கட்டமைப்பு KerxSubTableFormat0
{
  முழு get_kerning (hb_codepoint_t left, hb_codepoint_t வலது,
		   hb_aat_apply_context_t *c = nullptr) மாறிலி
  {
    hb_glyph_pair_t pair = {left, வலது};
    முழு v = pairs.bsearch (pair).get_kerning ();
    திரும்பு kerxTupleKern (v, header.tuple_மொத்தம்_2 (), இது, c);
  }

  தருக்க apply (hb_aat_apply_context_t *c) மாறிலி
  {
    TRACE_APPLY (இது);

    எனில் (!c->plan->requested_kerning)
      return_trace (பொய்);

    எனில் (header.coverage & header.Backwards)
      return_trace (பொய்);

    accelerator_t accel (*இது, c);
    hb_kern_machine_t<accelerator_t> machine (accel, header.coverage & header.CrossStream);
    machine.kern (c->font, c->buffer, c->plan->kern_mask);

    return_trace (உண்மை);
  }

  template <typename set_t>
  வெறுமை collect_glyphs (set_t &first_set, set_t &second_set, unsigned இலக்கம்_glyphs) மாறிலி
  {
    க்கு (மாறிலி KernPair& pair : pairs)
    {
      first_set.add (pair.left);
      second_set.add (pair.வலது);
    }
  }

  கட்டமைப்பு accelerator_t
  {
    மாறிலி KerxSubTableFormat0 &table;
    hb_aat_apply_context_t *c;

    accelerator_t (மாறிலி KerxSubTableFormat0 &table_,
		   hb_aat_apply_context_t *c_) :
		     table (table_), c (c_) {}

    முழு get_kerning (hb_codepoint_t left, hb_codepoint_t வலது) மாறிலி
    {
      எனில் (!(*c->first_set)[left] || !(*c->second_set)[வலது]) திரும்பு 0;
      திரும்பு table.get_kerning (left, வலது, c);
    }
  };


  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (likely (pairs.sanitize (c)));
  }

  பாதுகாக்கப்பட்ட:
  KernSubTableHeader	header;
  Bin_தேடல்_Array_Of<KernPair, typename KernSubTableHeader::Types::HBUINT>
			pairs;	/* Sorted kern records. */
  பொது:
  DEFINE_SIZE_ARRAY (KernSubTableHeader::static_size + 16, pairs);
};


template <தருக்க extended>
கட்டமைப்பு Format1Entry;

template <>
கட்டமைப்பு Format1Entry<உண்மை>
{
  எண்ணிக்கை Flags
  {
    தள்ளு		= 0x8000,	/* If set, push this glyph on the kerning stack. */
    DontAdvance		= 0x4000,	/* If set, don't advance to the next glyph
					 * before going to the new state. */
    Reset		= 0x2000,	/* If set, reset the kerning data (clear the stack) */
    Reserved		= 0x1FFF,	/* Not used; set to 0. */
  };

  கட்டமைப்பு EntryData
  {
    HBUINT16	kernActionIndex;/* Index into the kerning value array. If
				 * this index is 0xFFFF, then no kerning
				 * is to be performed. */
    பொது:
    DEFINE_SIZE_STATIC (2);
  };

  நிலையான தருக்க initiateAction (மாறிலி Entry<EntryData> &entry)
  { திரும்பு entry.flags & தள்ளு; }

  நிலையான தருக்க performAction (மாறிலி Entry<EntryData> &entry)
  { திரும்பு entry.data.kernActionIndex != 0xFFFF; }

  நிலையான unsigned முழு kernActionIndex (மாறிலி Entry<EntryData> &entry)
  { திரும்பு entry.data.kernActionIndex; }
};
template <>
கட்டமைப்பு Format1Entry<பொய்>
{
  எண்ணிக்கை Flags
  {
    தள்ளு		= 0x8000,	/* If set, push this glyph on the kerning stack. */
    DontAdvance		= 0x4000,	/* If set, don't advance to the next glyph
					 * before going to the new state. */
    Offset		= 0x3FFF,	/* Byte offset from beginning of subtable to the
					 * value table for the glyphs on the kerning stack. */

    Reset		= 0x0000,	/* Not supported? */
  };

  வகைவரை வெறுமை EntryData;

  நிலையான தருக்க initiateAction (மாறிலி Entry<EntryData> &entry)
  { திரும்பு entry.flags & தள்ளு; }

  நிலையான தருக்க performAction (மாறிலி Entry<EntryData> &entry)
  { திரும்பு entry.flags & Offset; }

  நிலையான unsigned முழு kernActionIndex (மாறிலி Entry<EntryData> &entry)
  { திரும்பு entry.flags & Offset; }
};

template <typename KernSubTableHeader>
கட்டமைப்பு KerxSubTableFormat1
{
  வகைவரை typename KernSubTableHeader::Types Types;
  வகைவரை typename Types::HBUINT HBUINT;

  வகைவரை Format1Entry<Types::extended> Format1EntryT;
  வகைவரை typename Format1EntryT::EntryData EntryData;

  எண்ணிக்கை Flags
  {
    DontAdvance	= Format1EntryT::DontAdvance,
  };

  தருக்க is_action_initiable (மாறிலி Entry<EntryData> &entry) மாறிலி
  {
    திரும்பு Format1EntryT::initiateAction (entry);
  }
  தருக்க is_actionable (மாறிலி Entry<EntryData> &entry) மாறிலி
  {
    திரும்பு Format1EntryT::performAction (entry);
  }

  கட்டமைப்பு driver_context_t
  {
    நிலையான constexpr தருக்க in_place = உண்மை;

    driver_context_t (மாறிலி KerxSubTableFormat1 *table_,
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
    வெறுமை transition (hb_buffer_t *buffer,
		     StateTableDriver<Types, EntryData, Flags> *driver,
		     மாறிலி Entry<EntryData> &entry)
    {
      unsigned முழு flags = entry.flags;

      எனில் (flags & Format1EntryT::Reset)
	depth = 0;

      எனில் (flags & Format1EntryT::தள்ளு)
      {
	எனில் (likely (depth < ARRAY_LENGTH (அடுக்கு)))
	  அடுக்கு[depth++] = buffer->idx;
	இல்லையெனில்
	  depth = 0; /* Probably not what CoreText does, but better? */
      }

      எனில் (Format1EntryT::performAction (entry) && depth)
      {
	unsigned முழு tuple_மொத்தம்_2 = hb_max (1u, table->header.tuple_மொத்தம்_2 ());

	unsigned முழு kern_idx = Format1EntryT::kernActionIndex (entry);
	kern_idx = Types::byteOffsetToIndex (kern_idx, &table->machine, kernAction.arrayZ);
	மாறிலி FWORD *actions = &kernAction[kern_idx];
	எனில் (!c->sanitizer.check_array (actions, depth, tuple_மொத்தம்_2))
	{
	  depth = 0;
	  திரும்பு;
	}
	hb_barrier ();

	hb_mask_t kern_mask = c->plan->kern_mask;

	/* From Apple 'kern' spec:
	 * "Each pops one glyph from the kerning stack and applies the kerning value to it.
	 * The end of the list is marked by an odd value... */
	தருக்க last = பொய்;
	போது (!last && depth)
	{
	  unsigned முழு idx = அடுக்கு[--depth];
	  முழு v = *actions;
	  actions += tuple_மொத்தம்_2;
	  எனில் (idx >= buffer->len) தொடர்;

	  /* "The end of the list is marked by an odd value..." */
	  last = v & 1;
	  v &= ~1;

	  hb_glyph_position_t &o = buffer->pos[idx];

	  எனில் (HB_DIRECTION_IS_HORIZONTAL (buffer->props.direction))
	  {
	    எனில் (crossStream)
	    {
	      /* The following flag is undocumented in the spec, but described
	       * in the 'kern' table example. */
	      எனில் (v == -0x8000)
	      {
		o.attach_type() = OT::Layout::GPOS_impl::ATTACH_TYPE_NONE;
		o.attach_chain() = 0;
		o.y_offset = 0;
	      }
	      இல்லையெனில் எனில் (o.attach_type())
	      {
		o.y_offset = hb_saturate_add (o.y_offset, c->font->em_scale_y (v));
		buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT;
	      }
	    }
	    இல்லையெனில் எனில் (buffer->info[idx].mask & kern_mask)
	    {
	      auto scaled = c->font->em_scale_x (v);
	      o.x_advance = hb_saturate_add (o.x_advance, scaled);
	      o.x_offset = hb_saturate_add (o.x_offset, scaled);
	    }
	  }
	  இல்லையெனில்
	  {
	    எனில் (crossStream)
	    {
	      /* CoreText doesn't do crossStream kerning in vertical.  We do. */
	      எனில் (v == -0x8000)
	      {
		o.attach_type() = OT::Layout::GPOS_impl::ATTACH_TYPE_NONE;
		o.attach_chain() = 0;
		o.x_offset = 0;
	      }
	      இல்லையெனில் எனில் (o.attach_type())
	      {
		o.x_offset = hb_saturate_add (o.x_offset, c->font->em_scale_x (v));
		buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT;
	      }
	    }
	    இல்லையெனில் எனில் (buffer->info[idx].mask & kern_mask)
	    {
	      auto scaled = c->font->em_scale_y (v);
	      o.y_advance = hb_saturate_add (o.y_advance, scaled);
	      o.y_offset = hb_saturate_add (o.y_offset, scaled);
	    }
	  }
	}
      }
    }

    பொது:
    hb_aat_apply_context_t *c;
    மாறிலி KerxSubTableFormat1 *table;
    தனிப்பட்ட:
    மாறிலி UnsizedArrayOf<FWORD> &kernAction;
    unsigned முழு அடுக்கு[8];
    unsigned முழு depth;
    தருக்க crossStream;
  };

  தருக்க apply (hb_aat_apply_context_t *c) மாறிலி
  {
    TRACE_APPLY (இது);

    எனில் (!c->plan->requested_kerning &&
	!(header.coverage & header.CrossStream))
      திரும்பு பொய்;

    driver_context_t dc (இது, c);

    StateTableDriver<Types, EntryData, Flags> driver (machine, c->font->face);

    driver.drive (&dc, c);

    return_trace (உண்மை);
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    /* The rest of array sanitizations are done at run-time. */
    return_trace (likely (c->check_struct (இது) &&
			  machine.sanitize (c)));
  }

  template <typename set_t>
  வெறுமை collect_glyphs (set_t &first_set, set_t &second_set, unsigned இலக்கம்_glyphs) மாறிலி
  {
    machine.collect_initial_glyphs (first_set, இலக்கம்_glyphs, *இது);
    //machine.collect_glyphs (second_set, num_glyphs); // second_set is unused for machine kerning
  }

  பாதுகாக்கப்பட்ட:
  KernSubTableHeader				header;
  StateTable<Types, EntryData>			machine;
  NNOffsetTo<UnsizedArrayOf<FWORD>, HBUINT>	kernAction;
  பொது:
  DEFINE_SIZE_STATIC (KernSubTableHeader::static_size + (StateTable<Types, EntryData>::static_size + HBUINT::static_size));
};

template <typename KernSubTableHeader>
கட்டமைப்பு KerxSubTableFormat2
{
  வகைவரை typename KernSubTableHeader::Types Types;
  வகைவரை typename Types::HBUINT HBUINT;

  முழு get_kerning (hb_codepoint_t left, hb_codepoint_t வலது,
		   hb_aat_apply_context_t *c) மாறிலி
  {
    unsigned முழு இலக்கம்_glyphs = c->sanitizer.get_இலக்கம்_glyphs ();
    unsigned முழு l = (இது+இடது_Class_Table).get_class (left, இலக்கம்_glyphs, 0);
    unsigned முழு r = (இது+வலது_Class_Table).get_class (வலது, இலக்கம்_glyphs, 0);

    மாறிலி UnsizedArrayOf<FWORD> &arrayZ = இது+array;
    unsigned முழு kern_idx = l + r;
    kern_idx = Types::offsetToIndex (kern_idx, இது, arrayZ.arrayZ);
    மாறிலி FWORD *v = &arrayZ[kern_idx];
    எனில் (unlikely (!v->sanitize (&c->sanitizer))) திரும்பு 0;
    hb_barrier ();

    திரும்பு kerxTupleKern (*v, header.tuple_மொத்தம்_2 (), இது, c);
  }

  தருக்க apply (hb_aat_apply_context_t *c) மாறிலி
  {
    TRACE_APPLY (இது);

    எனில் (!c->plan->requested_kerning)
      return_trace (பொய்);

    எனில் (header.coverage & header.Backwards)
      return_trace (பொய்);

    accelerator_t accel (*இது, c);
    hb_kern_machine_t<accelerator_t> machine (accel, header.coverage & header.CrossStream);
    machine.kern (c->font, c->buffer, c->plan->kern_mask);

    return_trace (உண்மை);
  }

  template <typename set_t>
  வெறுமை collect_glyphs (set_t &first_set, set_t &second_set, unsigned இலக்கம்_glyphs) மாறிலி
  {
    (இது+இடது_Class_Table).collect_glyphs (first_set, இலக்கம்_glyphs);
    (இது+வலது_Class_Table).collect_glyphs (second_set, இலக்கம்_glyphs);
  }

  கட்டமைப்பு accelerator_t
  {
    மாறிலி KerxSubTableFormat2 &table;
    hb_aat_apply_context_t *c;

    accelerator_t (மாறிலி KerxSubTableFormat2 &table_,
		   hb_aat_apply_context_t *c_) :
		     table (table_), c (c_) {}

    முழு get_kerning (hb_codepoint_t left, hb_codepoint_t வலது) மாறிலி
    {
      எனில் (!(*c->first_set)[left] || !(*c->second_set)[வலது]) திரும்பு 0;
      திரும்பு table.get_kerning (left, வலது, c);
    }
  };

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (likely (c->check_struct (இது) &&
			  இடது_Class_Table.sanitize (c, இது) &&
			  வலது_Class_Table.sanitize (c, இது) &&
			  hb_barrier () &&
			  c->check_range (இது, array)));
  }

  பாதுகாக்கப்பட்ட:
  KernSubTableHeader	header;
  HBUINT		rowWidth;	/* The width, in bytes, of a row in the table. */
  NNOffsetTo<typename Types::ClassTypeWide, HBUINT>
			இடது_Class_Table;	/* Offset from beginning of this subtable to
					 * left-hand class table. */
  NNOffsetTo<typename Types::ClassTypeWide, HBUINT>
			வலது_Class_Table;/* Offset from beginning of this subtable to
					 * right-hand class table. */
  NNOffsetTo<UnsizedArrayOf<FWORD>, HBUINT>
			 array;		/* Offset from beginning of this subtable to
					 * the start of the kerning array. */
  பொது:
  DEFINE_SIZE_STATIC (KernSubTableHeader::static_size + 4 * அளவு (HBUINT));
};

template <typename KernSubTableHeader>
கட்டமைப்பு KerxSubTableFormat4
{
  வகைவரை ExtendedTypes Types;

  கட்டமைப்பு EntryData
  {
    HBUINT16	ankrActionIndex;/* Either 0xFFFF (for no action) or the index of
				 * the action to perform. */
    பொது:
    DEFINE_SIZE_STATIC (2);
  };

  எண்ணிக்கை Flags
  {
    Mark		= 0x8000,	/* If set, remember this glyph as the marked glyph. */
    DontAdvance		= 0x4000,	/* If set, don't advance to the next glyph before
					 * going to the new state. */
    Reserved		= 0x3FFF,	/* Not used; set to 0. */
  };

  தருக்க is_action_initiable (மாறிலி Entry<EntryData> &entry) மாறிலி
  {
    திரும்பு (entry.flags & Mark);
  }
  தருக்க is_actionable (மாறிலி Entry<EntryData> &entry) மாறிலி
  {
    திரும்பு entry.data.ankrActionIndex != 0xFFFF;
  }

  கட்டமைப்பு driver_context_t
  {
    நிலையான constexpr தருக்க in_place = உண்மை;
    எண்ணிக்கை SubTableFlags
    {
      ActionType	= 0xC0000000,	/* A two-bit field containing the action type. */
      Unused		= 0x3F000000,	/* Unused - must be zero. */
      Offset		= 0x00FFFFFF,	/* Masks the offset in bytes from the beginning
					 * of the subtable to the beginning of the control
					 * point table. */
    };

    driver_context_t (மாறிலி KerxSubTableFormat4 *table_,
		      hb_aat_apply_context_t *c_) :
	c (c_),
	table (table_),
	action_type ((table->flags & ActionType) >> 30),
	ankrData ((HBUINT16 *) ((மாறிலி எழுத்து *) &table->machine + (table->flags & Offset))),
	mark_set (பொய்),
	mark (0) {}

    HB_AAT_TRANSITION_INLINE
    வெறுமை transition (hb_buffer_t *buffer,
		     StateTableDriver<Types, EntryData, Flags> *driver,
		     மாறிலி Entry<EntryData> &entry)
    {
      எனில் (mark_set && entry.data.ankrActionIndex != 0xFFFF && buffer->idx < buffer->len)
      {
	hb_glyph_position_t &o = buffer->cur_pos();
	தேர்வு (action_type)
	{
	  நிலை 0: /* Control Point Actions.*/
	  {
	    /* Indexed into glyph outline. */
	    /* Each action (record in ankrData) contains two 16-bit fields, so we must
	       double the ankrActionIndex to get the correct offset here. */
	    மாறிலி HBUINT16 *data = &ankrData[entry.data.ankrActionIndex * 2];
	    எனில் (!c->sanitizer.check_array (data, 2)) திரும்பு;
	    hb_barrier ();
	    unsigned முழு markControlPoint = *data++;
	    unsigned முழு currControlPoint = *data++;
	    hb_position_t markX = 0;
	    hb_position_t markY = 0;
	    hb_position_t currX = 0;
	    hb_position_t currY = 0;
	    எனில் (!c->font->get_glyph_contour_point_for_origin (c->buffer->info[mark].codepoint,
							      markControlPoint,
							      HB_DIRECTION_LTR /*XXX*/,
							      &markX, &markY) ||
		!c->font->get_glyph_contour_point_for_origin (c->buffer->cur ().codepoint,
							      currControlPoint,
							      HB_DIRECTION_LTR /*XXX*/,
							      &currX, &currY))
	      திரும்பு;

	    o.x_offset = hb_saturate_sub (markX, currX);
	    o.y_offset = hb_saturate_sub (markY, currY);
	  }
	  நிறுத்து;

	  நிலை 1: /* Anchor Point Actions. */
	  {
	    /* Indexed into 'ankr' table. */
	    /* Each action (record in ankrData) contains two 16-bit fields, so we must
	       double the ankrActionIndex to get the correct offset here. */
	    மாறிலி HBUINT16 *data = &ankrData[entry.data.ankrActionIndex * 2];
	    எனில் (!c->sanitizer.check_array (data, 2)) திரும்பு;
	    hb_barrier ();
	    unsigned முழு markAnchorPoint = *data++;
	    unsigned முழு currAnchorPoint = *data++;
	    மாறிலி Anchor &markAnchor = c->ankr_table->get_anchor (c->buffer->info[mark].codepoint,
								  markAnchorPoint,
								  c->sanitizer.get_இலக்கம்_glyphs ());
	    மாறிலி Anchor &currAnchor = c->ankr_table->get_anchor (c->buffer->cur ().codepoint,
								  currAnchorPoint,
								  c->sanitizer.get_இலக்கம்_glyphs ());

	    o.x_offset = hb_saturate_sub (c->font->em_scale_x (markAnchor.xCoordinate),
					  c->font->em_scale_x (currAnchor.xCoordinate));
	    o.y_offset = hb_saturate_sub (c->font->em_scale_y (markAnchor.yCoordinate),
					  c->font->em_scale_y (currAnchor.yCoordinate));
	  }
	  நிறுத்து;

	  நிலை 2: /* Control Point Coordinate Actions. */
	  {
	    /* Each action contains four 16-bit fields, so we multiply the ankrActionIndex
	       by 4 to get the correct offset for the given action. */
	    மாறிலி FWORD *data = (மாறிலி FWORD *) &ankrData[entry.data.ankrActionIndex * 4];
	    எனில் (!c->sanitizer.check_array (data, 4)) திரும்பு;
	    hb_barrier ();
	    முழு markX = *data++;
	    முழு markY = *data++;
	    முழு currX = *data++;
	    முழு currY = *data++;

	    o.x_offset = hb_saturate_sub (c->font->em_scale_x (markX), c->font->em_scale_x (currX));
	    o.y_offset = hb_saturate_sub (c->font->em_scale_y (markY), c->font->em_scale_y (currY));
	  }
	  நிறுத்து;
	}
	o.attach_type() = OT::Layout::GPOS_impl::ATTACH_TYPE_MARK;
	o.attach_chain() = (முழு) mark - (முழு) buffer->idx;
	எனில் (c->buffer_is_reversed)
	  o.attach_chain() = -o.attach_chain();
	buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT;
      }

      எனில் (entry.flags & Mark)
      {
	mark_set = உண்மை;
	mark = buffer->idx;
      }
    }

    பொது:
    hb_aat_apply_context_t *c;
    மாறிலி KerxSubTableFormat4 *table;
    தனிப்பட்ட:
    unsigned முழு action_type;
    மாறிலி HBUINT16 *ankrData;
    தருக்க mark_set;
    unsigned முழு mark;
  };

  தருக்க apply (hb_aat_apply_context_t *c) மாறிலி
  {
    TRACE_APPLY (இது);

    driver_context_t dc (இது, c);

    StateTableDriver<Types, EntryData, Flags> driver (machine, c->font->face);

    driver.drive (&dc, c);

    return_trace (உண்மை);
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    /* The rest of array sanitizations are done at run-time. */
    return_trace (likely (c->check_struct (இது) &&
			  machine.sanitize (c)));
  }

  template <typename set_t>
  வெறுமை collect_glyphs (set_t &first_set, set_t &second_set, unsigned இலக்கம்_glyphs) மாறிலி
  {
    machine.collect_initial_glyphs (first_set, இலக்கம்_glyphs, *இது);
    //machine.collect_glyphs (second_set, num_glyphs); // second_set is unused for machine kerning
  }

  பாதுகாக்கப்பட்ட:
  KernSubTableHeader		header;
  StateTable<Types, EntryData>	machine;
  HBUINT32			flags;
  பொது:
  DEFINE_SIZE_STATIC (KernSubTableHeader::static_size + (StateTable<Types, EntryData>::static_size + HBUINT32::static_size));
};

template <typename KernSubTableHeader>
கட்டமைப்பு KerxSubTableFormat6
{
  எண்ணிக்கை Flags
  {
    மதிப்புகள்_Are_Long	= 0x00000001,
  };

  தருக்க is_long () மாறிலி { திரும்பு flags & மதிப்புகள்_Are_Long; }

  முழு get_kerning (hb_codepoint_t left, hb_codepoint_t வலது,
		   hb_aat_apply_context_t *c) மாறிலி
  {
    unsigned முழு இலக்கம்_glyphs = c->sanitizer.get_இலக்கம்_glyphs ();
    எனில் (is_long ())
    {
      மாறிலி auto &t = u.l;
      unsigned முழு l = (இது+t.rowIndexTable).get_மதிப்பு_or_null (left, இலக்கம்_glyphs);
      unsigned முழு r = (இது+t.columnIndexTable).get_மதிப்பு_or_null (வலது, இலக்கம்_glyphs);
      unsigned முழு offset = l + r;
      எனில் (unlikely (offset < l)) திரும்பு 0; /* Addition overflow. */
      எனில் (unlikely (hb_unsigned_mul_overflows (offset, அளவு (FWORD32)))) திரும்பு 0;
      மாறிலி FWORD32 *v = &StructAtOffset<FWORD32> (&(இது+t.array), offset * அளவு (FWORD32));
      எனில் (unlikely (!v->sanitize (&c->sanitizer))) திரும்பு 0;
      hb_barrier ();
      திரும்பு kerxTupleKern (*v, header.tuple_மொத்தம்_2 (), &(இது+vector), c);
    }
    இல்லையெனில்
    {
      மாறிலி auto &t = u.s;
      unsigned முழு l = (இது+t.rowIndexTable).get_மதிப்பு_or_null (left, இலக்கம்_glyphs);
      unsigned முழு r = (இது+t.columnIndexTable).get_மதிப்பு_or_null (வலது, இலக்கம்_glyphs);
      unsigned முழு offset = l + r;
      மாறிலி FWORD *v = &StructAtOffset<FWORD> (&(இது+t.array), offset * அளவு (FWORD));
      எனில் (unlikely (!v->sanitize (&c->sanitizer))) திரும்பு 0;
      hb_barrier ();
      திரும்பு kerxTupleKern (*v, header.tuple_மொத்தம்_2 (), &(இது+vector), c);
    }
  }

  தருக்க apply (hb_aat_apply_context_t *c) மாறிலி
  {
    TRACE_APPLY (இது);

    எனில் (!c->plan->requested_kerning)
      return_trace (பொய்);

    எனில் (header.coverage & header.Backwards)
      return_trace (பொய்);

    accelerator_t accel (*இது, c);
    hb_kern_machine_t<accelerator_t> machine (accel, header.coverage & header.CrossStream);
    machine.kern (c->font, c->buffer, c->plan->kern_mask);

    return_trace (உண்மை);
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (likely (c->check_struct (இது) &&
			  hb_barrier () &&
			  (is_long () ?
			   (
			     u.l.rowIndexTable.sanitize (c, இது) &&
			     u.l.columnIndexTable.sanitize (c, இது) &&
			     c->check_range (இது, u.l.array)
			   ) : (
			     u.s.rowIndexTable.sanitize (c, இது) &&
			     u.s.columnIndexTable.sanitize (c, இது) &&
			     c->check_range (இது, u.s.array)
			   )) &&
			  (header.tuple_மொத்தம்_2 () == 0 ||
			   c->check_range (இது, vector))));
  }

  template <typename set_t>
  வெறுமை collect_glyphs (set_t &first_set, set_t &second_set, unsigned இலக்கம்_glyphs) மாறிலி
  {
    எனில் (is_long ())
    {
      மாறிலி auto &t = u.l;
      (இது+t.rowIndexTable).collect_glyphs (first_set, இலக்கம்_glyphs);
      (இது+t.columnIndexTable).collect_glyphs (second_set, இலக்கம்_glyphs);
    }
    இல்லையெனில்
    {
      மாறிலி auto &t = u.s;
      (இது+t.rowIndexTable).collect_glyphs (first_set, இலக்கம்_glyphs);
      (இது+t.columnIndexTable).collect_glyphs (second_set, இலக்கம்_glyphs);
    }
  }

  கட்டமைப்பு accelerator_t
  {
    மாறிலி KerxSubTableFormat6 &table;
    hb_aat_apply_context_t *c;

    accelerator_t (மாறிலி KerxSubTableFormat6 &table_,
		   hb_aat_apply_context_t *c_) :
		     table (table_), c (c_) {}

    முழு get_kerning (hb_codepoint_t left, hb_codepoint_t வலது) மாறிலி
    {
      எனில் (!(*c->first_set)[left] || !(*c->second_set)[வலது]) திரும்பு 0;
      திரும்பு table.get_kerning (left, வலது, c);
    }
  };

  பாதுகாக்கப்பட்ட:
  KernSubTableHeader		header;
  HBUINT32			flags;
  HBUINT16			row_மொத்தம்;
  HBUINT16			column_மொத்தம்;
  ஒன்றிப்பு U
  {
    கட்டமைப்பு Long
    {
      NNOffset32To<Lookup<HBUINT32>>		rowIndexTable;
      NNOffset32To<Lookup<HBUINT32>>		columnIndexTable;
      NNOffset32To<UnsizedArrayOf<FWORD32>>	array;
    } l;
    கட்டமைப்பு Short
    {
      NNOffset32To<Lookup<HBUINT16>>		rowIndexTable;
      NNOffset32To<Lookup<HBUINT16>>		columnIndexTable;
      NNOffset32To<UnsizedArrayOf<FWORD>>	array;
    } s;
  } u;
  NNOffset32To<UnsizedArrayOf<FWORD>>	vector;
  பொது:
  DEFINE_SIZE_STATIC (KernSubTableHeader::static_size + 24);
};


கட்டமைப்பு KerxSubTableHeader
{
  வகைவரை ExtendedTypes Types;

  unsigned   tuple_மொத்தம்_2 () மாறிலி { திரும்பு tuple_மொத்தம்; }
  தருக்க     is_horizontal () மாறிலி { திரும்பு !(coverage & Vertical); }

  எண்ணிக்கை Coverage
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

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது));
  }

  பொது:
  HBUINT32	length;
  HBUINT32	coverage;
  HBUINT32	tuple_மொத்தம்;
  பொது:
  DEFINE_SIZE_STATIC (12);
};

கட்டமைப்பு KerxSubTable
{
  friend கட்டமைப்பு kerx;

  size_t get_size () மாறிலி { திரும்பு u.header.length; }
  unsigned முழு get_type () மாறிலி { திரும்பு u.header.coverage & u.header.SubtableType; }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) மாறிலி
  {
    unsigned முழு subtable_type = get_type ();
    TRACE_DISPATCH (இது, subtable_type);
    தேர்வு (subtable_type) {
    நிலை 0:	hb_barrier (); return_trace (c->dispatch (u.format0, std::forward<Ts> (ds)...));
    நிலை 1:	hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    நிலை 2:	hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    நிலை 4:	hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
    நிலை 6:	hb_barrier (); return_trace (c->dispatch (u.format6, std::forward<Ts> (ds)...));
    இயல்பு:	return_trace (c->default_return_மதிப்பு ());
    }
  }

  template <typename set_t>
  வெறுமை collect_glyphs (set_t &first_set, set_t &second_set, unsigned இலக்கம்_glyphs) மாறிலி
  {
    unsigned முழு subtable_type = get_type ();
    தேர்வு (subtable_type) {
    நிலை 0:	hb_barrier (); u.format0.collect_glyphs (first_set, second_set, இலக்கம்_glyphs); திரும்பு;
    நிலை 1:	hb_barrier (); u.format1.collect_glyphs (first_set, second_set, இலக்கம்_glyphs); திரும்பு;
    நிலை 2:	hb_barrier (); u.format2.collect_glyphs (first_set, second_set, இலக்கம்_glyphs); திரும்பு;
    நிலை 4:	hb_barrier (); u.format4.collect_glyphs (first_set, second_set, இலக்கம்_glyphs); திரும்பு;
    நிலை 6:	hb_barrier (); u.format6.collect_glyphs (first_set, second_set, இலக்கம்_glyphs); திரும்பு;
    இயல்பு:	திரும்பு;
    }
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    எனில் (!(u.header.sanitize (c) &&
	  hb_barrier () &&
	  u.header.length >= u.header.static_size &&
	  c->check_range (இது, u.header.length)))
      return_trace (பொய்);

    return_trace (dispatch (c));
  }

  பொது:
  ஒன்றிப்பு {
  KerxSubTableHeader				header;
  KerxSubTableFormat0<KerxSubTableHeader>	format0;
  KerxSubTableFormat1<KerxSubTableHeader>	format1;
  KerxSubTableFormat2<KerxSubTableHeader>	format2;
  KerxSubTableFormat4<KerxSubTableHeader>	format4;
  KerxSubTableFormat6<KerxSubTableHeader>	format6;
  } u;
  பொது:
  DEFINE_SIZE_MIN (12);
};


/*
 * The 'kerx' Table
 */

கட்டமைப்பு kern_subtable_accelerator_data_t
{
  hb_bit_set_t first_set;
  hb_bit_set_t second_set;
  mutable hb_aat_class_cache_t class_cache;
};

கட்டமைப்பு kern_accelerator_data_t
{
  hb_vector_t<kern_subtable_accelerator_data_t> subtable_accels;
  hb_aat_scratch_t scratch;
};

template <typename T>
கட்டமைப்பு KerxTable
{
  /* https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern */
  மாறிலி T* thiz () மாறிலி { திரும்பு static_cast<மாறிலி T *> (இது); }

  தருக்க has_state_machine () மாறிலி
  {
    வகைவரை typename T::SubTable SubTable;

    மாறிலி SubTable *st = &thiz()->firstSubTable;
    unsigned முழு மொத்தம் = thiz()->table_மொத்தம்;
    க்கு (unsigned முழு i = 0; i < மொத்தம்; i++)
    {
      எனில் (st->get_type () == 1)
	திரும்பு உண்மை;

      // TODO: What about format 4? What's this API used for anyway?

      st = &StructAfter<SubTable> (*st);
    }
    திரும்பு பொய்;
  }

  தருக்க has_cross_stream () மாறிலி
  {
    வகைவரை typename T::SubTable SubTable;

    மாறிலி SubTable *st = &thiz()->firstSubTable;
    unsigned முழு மொத்தம் = thiz()->table_மொத்தம்;
    க்கு (unsigned முழு i = 0; i < மொத்தம்; i++)
    {
      எனில் (st->u.header.coverage & st->u.header.CrossStream)
	திரும்பு உண்மை;
      st = &StructAfter<SubTable> (*st);
    }
    திரும்பு பொய்;
  }

  முழு get_h_kerning (hb_codepoint_t left, hb_codepoint_t வலது) மாறிலி
  {
    வகைவரை typename T::SubTable SubTable;

    முழு v = 0;
    மாறிலி SubTable *st = &thiz()->firstSubTable;
    unsigned முழு மொத்தம் = thiz()->table_மொத்தம்;
    க்கு (unsigned முழு i = 0; i < மொத்தம்; i++)
    {
      எனில் ((st->u.header.coverage & (st->u.header.Variation | st->u.header.CrossStream)) ||
	  !st->u.header.is_horizontal ())
	தொடர்;
      v += st->get_kerning (left, வலது);
      st = &StructAfter<SubTable> (*st);
    }
    திரும்பு v;
  }

  தருக்க apply (AAT::hb_aat_apply_context_t *c,
	      மாறிலி kern_accelerator_data_t &accel_data) மாறிலி
  {
    c->buffer->unsafe_to_concat ();

    c->setup_buffer_glyph_set ();

    வகைவரை typename T::SubTable SubTable;

    தருக்க ret = பொய்;
    தருக்க seenCrossStream = பொய்;
    c->set_lookup_index (0);
    மாறிலி SubTable *st = &thiz()->firstSubTable;
    unsigned முழு மொத்தம் = thiz()->table_மொத்தம்;
    க்கு (unsigned முழு i = 0; i < மொத்தம்; i++)
    {
      தருக்க reverse;

      auto &subtable_accel = accel_data.subtable_accels[i];

      எனில் (!T::Types::extended && (st->u.header.coverage & st->u.header.Variation))
	செல் skip;

      எனில் (HB_DIRECTION_IS_HORIZONTAL (c->buffer->props.direction) != st->u.header.is_horizontal ())
	செல் skip;

      c->first_set = &subtable_accel.first_set;
      c->second_set = &subtable_accel.second_set;
      c->machine_class_cache = &subtable_accel.class_cache;

      எனில் (!c->buffer_intersects_machine ())
      {
	(வெறுமை) c->buffer->message (c->font, "skipped subtable %u because no glyph matches", c->lookup_index);
	செல் skip;
      }

      reverse = தருக்க (st->u.header.coverage & st->u.header.Backwards) !=
		HB_DIRECTION_IS_BACKWARD (c->buffer->props.direction);

      எனில் (!c->buffer->message (c->font, "start subtable %u", c->lookup_index))
	செல் skip;

      எனில் (!seenCrossStream &&
	  (st->u.header.coverage & st->u.header.CrossStream))
      {
	/* Attach all glyphs into a chain. */
	seenCrossStream = உண்மை;
	hb_glyph_position_t *pos = c->buffer->pos;
	unsigned முழு மொத்தம் = c->buffer->len;
	க்கு (unsigned முழு i = 0; i < மொத்தம்; i++)
	{
	  pos[i].attach_type() = OT::Layout::GPOS_impl::ATTACH_TYPE_CURSIVE;
	  pos[i].attach_chain() = HB_DIRECTION_IS_FORWARD (c->buffer->props.direction) ? -1 : +1;
	  /* We intentionally don't set HB_BUFFER_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT,
	   * since there needs to be a non-zero attachment for post-positioning to
	   * be needed. */
	}
      }

      எனில் (reverse != c->buffer_is_reversed)
        c->reverse_buffer ();

      {
	/* See comment in sanitize() for conditional here. */
	hb_sanitize_with_object_t with (&c->sanitizer, i < மொத்தம் - 1 ? st : (மாறிலி SubTable *) nullptr);
	ret |= st->dispatch (c);
      }

      (வெறுமை) c->buffer->message (c->font, "end subtable %u", c->lookup_index);

    skip:
      st = &StructAfter<SubTable> (*st);
      c->set_lookup_index (c->lookup_index + 1);
    }
    எனில் (c->buffer_is_reversed)
      c->reverse_buffer ();

    திரும்பு ret;
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    எனில் (unlikely (!(thiz()->version.sanitize (c) &&
		    hb_barrier () &&
		    (unsigned) thiz()->version >= (unsigned) T::minVersion &&
		    thiz()->table_மொத்தம்.sanitize (c))))
      return_trace (பொய்);

    வகைவரை typename T::SubTable SubTable;

    மாறிலி SubTable *st = &thiz()->firstSubTable;
    unsigned முழு மொத்தம் = thiz()->table_மொத்தம்;
    க்கு (unsigned முழு i = 0; i < மொத்தம்; i++)
    {
      எனில் (unlikely (!st->u.header.sanitize (c)))
	return_trace (பொய்);
      hb_barrier ();
      /* OpenType kern table has 2-byte subtable lengths.  That's limiting.
       * MS implementation also only supports one subtable, of format 0,
       * anyway.  Certain versions of some fonts, like Calibry, contain
       * kern subtable that exceeds 64kb.  Looks like, the subtable length
       * is simply ignored.  Which makes sense.  It's only needed if you
       * have multiple subtables.  To handle such fonts, we just ignore
       * the length for the last subtable. */
      hb_sanitize_with_object_t with (c, i < மொத்தம் - 1 ? st : (மாறிலி SubTable *) nullptr);

      எனில் (unlikely (!st->sanitize (c)))
	return_trace (பொய்);

      st = &StructAfter<SubTable> (*st);
    }

    unsigned majorVersion = thiz()->version;
    எனில் (அளவு (thiz()->version) == 4)
      majorVersion = majorVersion >> 16;
    எனில் (majorVersion >= 3)
    {
      மாறிலி SubtableGlyphCoverage *coverage = (மாறிலி SubtableGlyphCoverage *) st;
      எனில் (!coverage->sanitize (c, மொத்தம்))
        return_trace (பொய்);
    }

    return_trace (உண்மை);
  }

  kern_accelerator_data_t create_accelerator_data (unsigned இலக்கம்_glyphs) மாறிலி
  {
    kern_accelerator_data_t accel_data;

    வகைவரை typename T::SubTable SubTable;

    மாறிலி SubTable *st = &thiz()->firstSubTable;
    unsigned முழு மொத்தம் = thiz()->table_மொத்தம்;
    க்கு (unsigned முழு i = 0; i < மொத்தம்; i++)
    {
      auto &subtable_accel = *accel_data.subtable_accels.push ();
      எனில் (unlikely (accel_data.subtable_accels.in_error ()))
	  திரும்பு accel_data;

      st->collect_glyphs (subtable_accel.first_set, subtable_accel.second_set, இலக்கம்_glyphs);
      subtable_accel.class_cache.clear ();

      st = &StructAfter<SubTable> (*st);
    }

    திரும்பு accel_data;
  }

  கட்டமைப்பு accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      hb_sanitize_context_t sc;
      இது->table = sc.reference_table<T> (face);
      இது->accel_data = இது->table->create_accelerator_data (face->get_இலக்கம்_glyphs ());
    }
    ~accelerator_t ()
    {
      இது->table.destroy ();
    }

    hb_blob_t *get_blob () மாறிலி { திரும்பு table.get_blob (); }

    தருக்க apply (AAT::hb_aat_apply_context_t *c) மாறிலி
    {
      திரும்பு table->apply (c, accel_data);
    }

    hb_blob_ptr_t<T> table;
    kern_accelerator_data_t accel_data;
    hb_aat_scratch_t scratch;
  };
};

கட்டமைப்பு kerx : KerxTable<kerx>
{
  friend கட்டமைப்பு KerxTable<kerx>;

  நிலையான constexpr hb_tag_t tableTag = HB_AAT_TAG_kerx;
  நிலையான constexpr unsigned minVersion = 2u;

  வகைவரை KerxSubTableHeader SubTableHeader;
  வகைவரை SubTableHeader::Types Types;
  வகைவரை KerxSubTable SubTable;

  தருக்க has_data () மாறிலி { திரும்பு version; }

  பாதுகாக்கப்பட்ட:
  HBUINT16	version;	/* The version number of the extended kerning table
				 * (currently 2, 3, or 4). */
  HBUINT16	unused;		/* Set to 0. */
  HBUINT32	table_மொத்தம்;	/* The number of subtables included in the extended kerning
				 * table. */
  SubTable	firstSubTable;	/* Subtables. */
/*subtableGlyphCoverageArray*/	/* Only if version >= 3. We don't use. */

  பொது:
  DEFINE_SIZE_MIN (8);
};

கட்டமைப்பு kerx_accelerator_t : kerx::accelerator_t {
  kerx_accelerator_t (hb_face_t *face) : kerx::accelerator_t (face) {}
};

} /* namespace AAT */

#endif /* HB_AAT_LAYOUT_KERX_TABLE_HH */
