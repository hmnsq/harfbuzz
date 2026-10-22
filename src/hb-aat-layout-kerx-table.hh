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

#ಅನ್ತರ್ಭಾವ "hb-kern.hh"
#ಅನ್ತರ್ಭಾವ "hb-aat-layout-ankr-table.hh"
#ಅನ್ತರ್ಭಾವ "hb-set-digest.hh"

/*
 * kerx -- Extended Kerning
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6kerx.html
 */
#define HB_AAT_TAG_kerx HB_TAG('k','e','r','x')


ನಾಮಾಕಾಶ AAT {

using ನಾಮಾಕಾಶ OT;


ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ
kerxTupleKern (ಪೂರ್ಣಾಙ್ಕ value,
	       unsigned ಪೂರ್ಣಾಙ್ಕ tuple_ಎಣಿಕೆ,
	       ಧ್ರುವ ಶೂನ್ಯ *base,
	       hb_aat_apply_context_t *c)
{
  ಯದಿ (likely (!tuple_ಎಣಿಕೆ || !c)) ಪ್ರತ್ಯಾಗಮ value;

  unsigned ಪೂರ್ಣಾಙ್ಕ offset = value;
  ಧ್ರುವ FWORD *pv = &StructAtOffset<FWORD> (base, offset);
  ಯದಿ (unlikely (!c->sanitizer.check_array (pv, tuple_ಎಣಿಕೆ))) ಪ್ರತ್ಯಾಗಮ 0;
  hb_barrier ();
  ಪ್ರತ್ಯಾಗಮ *pv;
}


ಸಂರಚನಾ hb_glyph_pair_t
{
  hb_codepoint_t left;
  hb_codepoint_t ಬಲ;
};

ಸಂರಚನಾ KernPair
{
  ಪೂರ್ಣಾಙ್ಕ get_kerning () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ value; }

  ಪೂರ್ಣಾಙ್ಕ cmp (ಧ್ರುವ hb_glyph_pair_t &o) ಧ್ರುವ
  {
    ಪೂರ್ಣಾಙ್ಕ ret = left.cmp (o.left);
    ಯದಿ (ret) ಪ್ರತ್ಯಾಗಮ ret;
    ಪ್ರತ್ಯಾಗಮ ಬಲ.cmp (o.ಬಲ);
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್));
  }

  ಸಾರ್ವಜನಿಕ:
  HBGlyphID16	left;
  HBGlyphID16	ಬಲ;
  FWORD		value;
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (6);
};

template <typename KernSubTableHeader>
ಸಂರಚನಾ KerxSubTableFormat0
{
  ಪೂರ್ಣಾಙ್ಕ get_kerning (hb_codepoint_t left, hb_codepoint_t ಬಲ,
		   hb_aat_apply_context_t *c = nullptr) ಧ್ರುವ
  {
    hb_glyph_pair_t pair = {left, ಬಲ};
    ಪೂರ್ಣಾಙ್ಕ v = pairs.bsearch (pair).get_kerning ();
    ಪ್ರತ್ಯಾಗಮ kerxTupleKern (v, header.tuple_ಎಣಿಕೆ_2 (), ಏತತ್, c);
  }

  ತಾರ್ಕಿಕ apply (hb_aat_apply_context_t *c) ಧ್ರುವ
  {
    TRACE_APPLY (ಏತತ್);

    ಯದಿ (!c->plan->requested_kerning)
      return_trace (ಅಸತ್ಯ);

    ಯದಿ (header.coverage & header.Backwards)
      return_trace (ಅಸತ್ಯ);

    accelerator_t accel (*ಏತತ್, c);
    hb_kern_machine_t<accelerator_t> machine (accel, header.coverage & header.CrossStream);
    machine.kern (c->font, c->buffer, c->plan->kern_mask);

    return_trace (ಸತ್ಯ);
  }

  template <typename set_t>
  ಶೂನ್ಯ collect_glyphs (set_t &first_set, set_t &second_set, unsigned ಅಂಕೆ_glyphs) ಧ್ರುವ
  {
    ಪ್ರತಿ (ಧ್ರುವ KernPair& pair : pairs)
    {
      first_set.add (pair.left);
      second_set.add (pair.ಬಲ);
    }
  }

  ಸಂರಚನಾ accelerator_t
  {
    ಧ್ರುವ KerxSubTableFormat0 &table;
    hb_aat_apply_context_t *c;

    accelerator_t (ಧ್ರುವ KerxSubTableFormat0 &table_,
		   hb_aat_apply_context_t *c_) :
		     table (table_), c (c_) {}

    ಪೂರ್ಣಾಙ್ಕ get_kerning (hb_codepoint_t left, hb_codepoint_t ಬಲ) ಧ್ರುವ
    {
      ಯದಿ (!(*c->first_set)[left] || !(*c->second_set)[ಬಲ]) ಪ್ರತ್ಯಾಗಮ 0;
      ಪ್ರತ್ಯಾಗಮ table.get_kerning (left, ಬಲ, c);
    }
  };


  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (likely (pairs.sanitize (c)));
  }

  ರಕ್ಷಿತ:
  KernSubTableHeader	header;
  Bin_ಹುಡುಕು_Array_Of<KernPair, typename KernSubTableHeader::Types::HBUINT>
			pairs;	/* Sorted kern records. */
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_ARRAY (KernSubTableHeader::static_size + 16, pairs);
};


template <ತಾರ್ಕಿಕ extended>
ಸಂರಚನಾ Format1Entry;

template <>
ಸಂರಚನಾ Format1Entry<ಸತ್ಯ>
{
  ಗಣನಾ Flags
  {
    ತಳ್ಳು		= 0x8000,	/* If set, push this glyph on the kerning stack. */
    DontAdvance		= 0x4000,	/* If set, don't advance to the next glyph
					 * before going to the new state. */
    Reset		= 0x2000,	/* If set, reset the kerning data (clear the stack) */
    Reserved		= 0x1FFF,	/* Not used; set to 0. */
  };

  ಸಂರಚನಾ EntryData
  {
    HBUINT16	kernActionIndex;/* Index into the kerning value array. If
				 * this index is 0xFFFF, then no kerning
				 * is to be performed. */
    ಸಾರ್ವಜನಿಕ:
    DEFINE_SIZE_STATIC (2);
  };

  ಸ್ಥಿರ ತಾರ್ಕಿಕ initiateAction (ಧ್ರುವ Entry<EntryData> &entry)
  { ಪ್ರತ್ಯಾಗಮ entry.flags & ತಳ್ಳು; }

  ಸ್ಥಿರ ತಾರ್ಕಿಕ performAction (ಧ್ರುವ Entry<EntryData> &entry)
  { ಪ್ರತ್ಯಾಗಮ entry.data.kernActionIndex != 0xFFFF; }

  ಸ್ಥಿರ unsigned ಪೂರ್ಣಾಙ್ಕ kernActionIndex (ಧ್ರುವ Entry<EntryData> &entry)
  { ಪ್ರತ್ಯಾಗಮ entry.data.kernActionIndex; }
};
template <>
ಸಂರಚನಾ Format1Entry<ಅಸತ್ಯ>
{
  ಗಣನಾ Flags
  {
    ತಳ್ಳು		= 0x8000,	/* If set, push this glyph on the kerning stack. */
    DontAdvance		= 0x4000,	/* If set, don't advance to the next glyph
					 * before going to the new state. */
    Offset		= 0x3FFF,	/* Byte offset from beginning of subtable to the
					 * value table for the glyphs on the kerning stack. */

    Reset		= 0x0000,	/* Not supported? */
  };

  ಪ್ರಕಾರನಿರ್ದೇಶ ಶೂನ್ಯ EntryData;

  ಸ್ಥಿರ ತಾರ್ಕಿಕ initiateAction (ಧ್ರುವ Entry<EntryData> &entry)
  { ಪ್ರತ್ಯಾಗಮ entry.flags & ತಳ್ಳು; }

  ಸ್ಥಿರ ತಾರ್ಕಿಕ performAction (ಧ್ರುವ Entry<EntryData> &entry)
  { ಪ್ರತ್ಯಾಗಮ entry.flags & Offset; }

  ಸ್ಥಿರ unsigned ಪೂರ್ಣಾಙ್ಕ kernActionIndex (ಧ್ರುವ Entry<EntryData> &entry)
  { ಪ್ರತ್ಯಾಗಮ entry.flags & Offset; }
};

template <typename KernSubTableHeader>
ಸಂರಚನಾ KerxSubTableFormat1
{
  ಪ್ರಕಾರನಿರ್ದೇಶ typename KernSubTableHeader::Types Types;
  ಪ್ರಕಾರನಿರ್ದೇಶ typename Types::HBUINT HBUINT;

  ಪ್ರಕಾರನಿರ್ದೇಶ Format1Entry<Types::extended> Format1EntryT;
  ಪ್ರಕಾರನಿರ್ದೇಶ typename Format1EntryT::EntryData EntryData;

  ಗಣನಾ Flags
  {
    DontAdvance	= Format1EntryT::DontAdvance,
  };

  ತಾರ್ಕಿಕ is_action_initiable (ಧ್ರುವ Entry<EntryData> &entry) ಧ್ರುವ
  {
    ಪ್ರತ್ಯಾಗಮ Format1EntryT::initiateAction (entry);
  }
  ತಾರ್ಕಿಕ is_actionable (ಧ್ರುವ Entry<EntryData> &entry) ಧ್ರುವ
  {
    ಪ್ರತ್ಯಾಗಮ Format1EntryT::performAction (entry);
  }

  ಸಂರಚನಾ driver_context_t
  {
    ಸ್ಥಿರ constexpr ತಾರ್ಕಿಕ in_place = ಸತ್ಯ;

    driver_context_t (ಧ್ರುವ KerxSubTableFormat1 *table_,
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
    ಶೂನ್ಯ transition (hb_buffer_t *buffer,
		     StateTableDriver<Types, EntryData, Flags> *driver,
		     ಧ್ರುವ Entry<EntryData> &entry)
    {
      unsigned ಪೂರ್ಣಾಙ್ಕ flags = entry.flags;

      ಯದಿ (flags & Format1EntryT::Reset)
	depth = 0;

      ಯದಿ (flags & Format1EntryT::ತಳ್ಳು)
      {
	ಯದಿ (likely (depth < ARRAY_LENGTH (ರಾಶಿ)))
	  ರಾಶಿ[depth++] = buffer->idx;
	ಅನ್ಯಥಾ
	  depth = 0; /* Probably not what CoreText does, but better? */
      }

      ಯದಿ (Format1EntryT::performAction (entry) && depth)
      {
	unsigned ಪೂರ್ಣಾಙ್ಕ tuple_ಎಣಿಕೆ_2 = hb_max (1u, table->header.tuple_ಎಣಿಕೆ_2 ());

	unsigned ಪೂರ್ಣಾಙ್ಕ kern_idx = Format1EntryT::kernActionIndex (entry);
	kern_idx = Types::byteOffsetToIndex (kern_idx, &table->machine, kernAction.arrayZ);
	ಧ್ರುವ FWORD *actions = &kernAction[kern_idx];
	ಯದಿ (!c->sanitizer.check_array (actions, depth, tuple_ಎಣಿಕೆ_2))
	{
	  depth = 0;
	  ಪ್ರತ್ಯಾಗಮ;
	}
	hb_barrier ();

	hb_mask_t kern_mask = c->plan->kern_mask;

	/* From Apple 'kern' spec:
	 * "Each pops one glyph from the kerning stack and applies the kerning value to it.
	 * The end of the list is marked by an odd value... */
	ತಾರ್ಕಿಕ last = ಅಸತ್ಯ;
	ಯಾವತ್ (!last && depth)
	{
	  unsigned ಪೂರ್ಣಾಙ್ಕ idx = ರಾಶಿ[--depth];
	  ಪೂರ್ಣಾಙ್ಕ v = *actions;
	  actions += tuple_ಎಣಿಕೆ_2;
	  ಯದಿ (idx >= buffer->len) ಅನುವರ್ತ;

	  /* "The end of the list is marked by an odd value..." */
	  last = v & 1;
	  v &= ~1;

	  hb_glyph_position_t &o = buffer->pos[idx];

	  ಯದಿ (HB_DIRECTION_IS_HORIZONTAL (buffer->props.direction))
	  {
	    ಯದಿ (crossStream)
	    {
	      /* The following flag is undocumented in the spec, but described
	       * in the 'kern' table example. */
	      ಯದಿ (v == -0x8000)
	      {
		o.attach_type() = OT::Layout::GPOS_impl::ATTACH_TYPE_NONE;
		o.attach_chain() = 0;
		o.y_offset = 0;
	      }
	      ಅನ್ಯಥಾ ಯದಿ (o.attach_type())
	      {
		o.y_offset = hb_saturate_add (o.y_offset, c->font->em_scale_y (v));
		buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT;
	      }
	    }
	    ಅನ್ಯಥಾ ಯದಿ (buffer->info[idx].mask & kern_mask)
	    {
	      auto scaled = c->font->em_scale_x (v);
	      o.x_advance = hb_saturate_add (o.x_advance, scaled);
	      o.x_offset = hb_saturate_add (o.x_offset, scaled);
	    }
	  }
	  ಅನ್ಯಥಾ
	  {
	    ಯದಿ (crossStream)
	    {
	      /* CoreText doesn't do crossStream kerning in vertical.  We do. */
	      ಯದಿ (v == -0x8000)
	      {
		o.attach_type() = OT::Layout::GPOS_impl::ATTACH_TYPE_NONE;
		o.attach_chain() = 0;
		o.x_offset = 0;
	      }
	      ಅನ್ಯಥಾ ಯದಿ (o.attach_type())
	      {
		o.x_offset = hb_saturate_add (o.x_offset, c->font->em_scale_x (v));
		buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT;
	      }
	    }
	    ಅನ್ಯಥಾ ಯದಿ (buffer->info[idx].mask & kern_mask)
	    {
	      auto scaled = c->font->em_scale_y (v);
	      o.y_advance = hb_saturate_add (o.y_advance, scaled);
	      o.y_offset = hb_saturate_add (o.y_offset, scaled);
	    }
	  }
	}
      }
    }

    ಸಾರ್ವಜನಿಕ:
    hb_aat_apply_context_t *c;
    ಧ್ರುವ KerxSubTableFormat1 *table;
    ಗುಪ್ತ:
    ಧ್ರುವ UnsizedArrayOf<FWORD> &kernAction;
    unsigned ಪೂರ್ಣಾಙ್ಕ ರಾಶಿ[8];
    unsigned ಪೂರ್ಣಾಙ್ಕ depth;
    ತಾರ್ಕಿಕ crossStream;
  };

  ತಾರ್ಕಿಕ apply (hb_aat_apply_context_t *c) ಧ್ರುವ
  {
    TRACE_APPLY (ಏತತ್);

    ಯದಿ (!c->plan->requested_kerning &&
	!(header.coverage & header.CrossStream))
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    driver_context_t dc (ಏತತ್, c);

    StateTableDriver<Types, EntryData, Flags> driver (machine, c->font->face);

    driver.drive (&dc, c);

    return_trace (ಸತ್ಯ);
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    /* The rest of array sanitizations are done at run-time. */
    return_trace (likely (c->check_struct (ಏತತ್) &&
			  machine.sanitize (c)));
  }

  template <typename set_t>
  ಶೂನ್ಯ collect_glyphs (set_t &first_set, set_t &second_set, unsigned ಅಂಕೆ_glyphs) ಧ್ರುವ
  {
    machine.collect_initial_glyphs (first_set, ಅಂಕೆ_glyphs, *ಏತತ್);
    //machine.collect_glyphs (second_set, num_glyphs); // second_set is unused for machine kerning
  }

  ರಕ್ಷಿತ:
  KernSubTableHeader				header;
  StateTable<Types, EntryData>			machine;
  NNOffsetTo<UnsizedArrayOf<FWORD>, HBUINT>	kernAction;
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (KernSubTableHeader::static_size + (StateTable<Types, EntryData>::static_size + HBUINT::static_size));
};

template <typename KernSubTableHeader>
ಸಂರಚನಾ KerxSubTableFormat2
{
  ಪ್ರಕಾರನಿರ್ದೇಶ typename KernSubTableHeader::Types Types;
  ಪ್ರಕಾರನಿರ್ದೇಶ typename Types::HBUINT HBUINT;

  ಪೂರ್ಣಾಙ್ಕ get_kerning (hb_codepoint_t left, hb_codepoint_t ಬಲ,
		   hb_aat_apply_context_t *c) ಧ್ರುವ
  {
    unsigned ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_glyphs = c->sanitizer.get_ಅಂಕೆ_glyphs ();
    unsigned ಪೂರ್ಣಾಙ್ಕ l = (ಏತತ್+ಎಡ_Class_Table).get_class (left, ಅಂಕೆ_glyphs, 0);
    unsigned ಪೂರ್ಣಾಙ್ಕ r = (ಏತತ್+ಬಲ_Class_Table).get_class (ಬಲ, ಅಂಕೆ_glyphs, 0);

    ಧ್ರುವ UnsizedArrayOf<FWORD> &arrayZ = ಏತತ್+array;
    unsigned ಪೂರ್ಣಾಙ್ಕ kern_idx = l + r;
    kern_idx = Types::offsetToIndex (kern_idx, ಏತತ್, arrayZ.arrayZ);
    ಧ್ರುವ FWORD *v = &arrayZ[kern_idx];
    ಯದಿ (unlikely (!v->sanitize (&c->sanitizer))) ಪ್ರತ್ಯಾಗಮ 0;
    hb_barrier ();

    ಪ್ರತ್ಯಾಗಮ kerxTupleKern (*v, header.tuple_ಎಣಿಕೆ_2 (), ಏತತ್, c);
  }

  ತಾರ್ಕಿಕ apply (hb_aat_apply_context_t *c) ಧ್ರುವ
  {
    TRACE_APPLY (ಏತತ್);

    ಯದಿ (!c->plan->requested_kerning)
      return_trace (ಅಸತ್ಯ);

    ಯದಿ (header.coverage & header.Backwards)
      return_trace (ಅಸತ್ಯ);

    accelerator_t accel (*ಏತತ್, c);
    hb_kern_machine_t<accelerator_t> machine (accel, header.coverage & header.CrossStream);
    machine.kern (c->font, c->buffer, c->plan->kern_mask);

    return_trace (ಸತ್ಯ);
  }

  template <typename set_t>
  ಶೂನ್ಯ collect_glyphs (set_t &first_set, set_t &second_set, unsigned ಅಂಕೆ_glyphs) ಧ್ರುವ
  {
    (ಏತತ್+ಎಡ_Class_Table).collect_glyphs (first_set, ಅಂಕೆ_glyphs);
    (ಏತತ್+ಬಲ_Class_Table).collect_glyphs (second_set, ಅಂಕೆ_glyphs);
  }

  ಸಂರಚನಾ accelerator_t
  {
    ಧ್ರುವ KerxSubTableFormat2 &table;
    hb_aat_apply_context_t *c;

    accelerator_t (ಧ್ರುವ KerxSubTableFormat2 &table_,
		   hb_aat_apply_context_t *c_) :
		     table (table_), c (c_) {}

    ಪೂರ್ಣಾಙ್ಕ get_kerning (hb_codepoint_t left, hb_codepoint_t ಬಲ) ಧ್ರುವ
    {
      ಯದಿ (!(*c->first_set)[left] || !(*c->second_set)[ಬಲ]) ಪ್ರತ್ಯಾಗಮ 0;
      ಪ್ರತ್ಯಾಗಮ table.get_kerning (left, ಬಲ, c);
    }
  };

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (likely (c->check_struct (ಏತತ್) &&
			  ಎಡ_Class_Table.sanitize (c, ಏತತ್) &&
			  ಬಲ_Class_Table.sanitize (c, ಏತತ್) &&
			  hb_barrier () &&
			  c->check_range (ಏತತ್, array)));
  }

  ರಕ್ಷಿತ:
  KernSubTableHeader	header;
  HBUINT		rowWidth;	/* The width, in bytes, of a row in the table. */
  NNOffsetTo<typename Types::ClassTypeWide, HBUINT>
			ಎಡ_Class_Table;	/* Offset from beginning of this subtable to
					 * left-hand class table. */
  NNOffsetTo<typename Types::ClassTypeWide, HBUINT>
			ಬಲ_Class_Table;/* Offset from beginning of this subtable to
					 * right-hand class table. */
  NNOffsetTo<UnsizedArrayOf<FWORD>, HBUINT>
			 array;		/* Offset from beginning of this subtable to
					 * the start of the kerning array. */
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (KernSubTableHeader::static_size + 4 * ಪರಿಮಾಣ (HBUINT));
};

template <typename KernSubTableHeader>
ಸಂರಚನಾ KerxSubTableFormat4
{
  ಪ್ರಕಾರನಿರ್ದೇಶ ExtendedTypes Types;

  ಸಂರಚನಾ EntryData
  {
    HBUINT16	ankrActionIndex;/* Either 0xFFFF (for no action) or the index of
				 * the action to perform. */
    ಸಾರ್ವಜನಿಕ:
    DEFINE_SIZE_STATIC (2);
  };

  ಗಣನಾ Flags
  {
    Mark		= 0x8000,	/* If set, remember this glyph as the marked glyph. */
    DontAdvance		= 0x4000,	/* If set, don't advance to the next glyph before
					 * going to the new state. */
    Reserved		= 0x3FFF,	/* Not used; set to 0. */
  };

  ತಾರ್ಕಿಕ is_action_initiable (ಧ್ರುವ Entry<EntryData> &entry) ಧ್ರುವ
  {
    ಪ್ರತ್ಯಾಗಮ (entry.flags & Mark);
  }
  ತಾರ್ಕಿಕ is_actionable (ಧ್ರುವ Entry<EntryData> &entry) ಧ್ರುವ
  {
    ಪ್ರತ್ಯಾಗಮ entry.data.ankrActionIndex != 0xFFFF;
  }

  ಸಂರಚನಾ driver_context_t
  {
    ಸ್ಥಿರ constexpr ತಾರ್ಕಿಕ in_place = ಸತ್ಯ;
    ಗಣನಾ SubTableFlags
    {
      ActionType	= 0xC0000000,	/* A two-bit field containing the action type. */
      Unused		= 0x3F000000,	/* Unused - must be zero. */
      Offset		= 0x00FFFFFF,	/* Masks the offset in bytes from the beginning
					 * of the subtable to the beginning of the control
					 * point table. */
    };

    driver_context_t (ಧ್ರುವ KerxSubTableFormat4 *table_,
		      hb_aat_apply_context_t *c_) :
	c (c_),
	table (table_),
	action_type ((table->flags & ActionType) >> 30),
	ankrData ((HBUINT16 *) ((ಧ್ರುವ ವರ್ಣ *) &table->machine + (table->flags & Offset))),
	mark_set (ಅಸತ್ಯ),
	mark (0) {}

    HB_AAT_TRANSITION_INLINE
    ಶೂನ್ಯ transition (hb_buffer_t *buffer,
		     StateTableDriver<Types, EntryData, Flags> *driver,
		     ಧ್ರುವ Entry<EntryData> &entry)
    {
      ಯದಿ (mark_set && entry.data.ankrActionIndex != 0xFFFF && buffer->idx < buffer->len)
      {
	hb_glyph_position_t &o = buffer->cur_pos();
	ವಿಕಲ್ಪ (action_type)
	{
	  ಪಕ್ಷ 0: /* Control Point Actions.*/
	  {
	    /* Indexed into glyph outline. */
	    /* Each action (record in ankrData) contains two 16-bit fields, so we must
	       double the ankrActionIndex to get the correct offset here. */
	    ಧ್ರುವ HBUINT16 *data = &ankrData[entry.data.ankrActionIndex * 2];
	    ಯದಿ (!c->sanitizer.check_array (data, 2)) ಪ್ರತ್ಯಾಗಮ;
	    hb_barrier ();
	    unsigned ಪೂರ್ಣಾಙ್ಕ markControlPoint = *data++;
	    unsigned ಪೂರ್ಣಾಙ್ಕ currControlPoint = *data++;
	    hb_position_t markX = 0;
	    hb_position_t markY = 0;
	    hb_position_t currX = 0;
	    hb_position_t currY = 0;
	    ಯದಿ (!c->font->get_glyph_contour_point_for_origin (c->buffer->info[mark].codepoint,
							      markControlPoint,
							      HB_DIRECTION_LTR /*XXX*/,
							      &markX, &markY) ||
		!c->font->get_glyph_contour_point_for_origin (c->buffer->cur ().codepoint,
							      currControlPoint,
							      HB_DIRECTION_LTR /*XXX*/,
							      &currX, &currY))
	      ಪ್ರತ್ಯಾಗಮ;

	    o.x_offset = hb_saturate_sub (markX, currX);
	    o.y_offset = hb_saturate_sub (markY, currY);
	  }
	  ಭಙ್ಗ;

	  ಪಕ್ಷ 1: /* Anchor Point Actions. */
	  {
	    /* Indexed into 'ankr' table. */
	    /* Each action (record in ankrData) contains two 16-bit fields, so we must
	       double the ankrActionIndex to get the correct offset here. */
	    ಧ್ರುವ HBUINT16 *data = &ankrData[entry.data.ankrActionIndex * 2];
	    ಯದಿ (!c->sanitizer.check_array (data, 2)) ಪ್ರತ್ಯಾಗಮ;
	    hb_barrier ();
	    unsigned ಪೂರ್ಣಾಙ್ಕ markAnchorPoint = *data++;
	    unsigned ಪೂರ್ಣಾಙ್ಕ currAnchorPoint = *data++;
	    ಧ್ರುವ Anchor &markAnchor = c->ankr_table->get_anchor (c->buffer->info[mark].codepoint,
								  markAnchorPoint,
								  c->sanitizer.get_ಅಂಕೆ_glyphs ());
	    ಧ್ರುವ Anchor &currAnchor = c->ankr_table->get_anchor (c->buffer->cur ().codepoint,
								  currAnchorPoint,
								  c->sanitizer.get_ಅಂಕೆ_glyphs ());

	    o.x_offset = hb_saturate_sub (c->font->em_scale_x (markAnchor.xCoordinate),
					  c->font->em_scale_x (currAnchor.xCoordinate));
	    o.y_offset = hb_saturate_sub (c->font->em_scale_y (markAnchor.yCoordinate),
					  c->font->em_scale_y (currAnchor.yCoordinate));
	  }
	  ಭಙ್ಗ;

	  ಪಕ್ಷ 2: /* Control Point Coordinate Actions. */
	  {
	    /* Each action contains four 16-bit fields, so we multiply the ankrActionIndex
	       by 4 to get the correct offset for the given action. */
	    ಧ್ರುವ FWORD *data = (ಧ್ರುವ FWORD *) &ankrData[entry.data.ankrActionIndex * 4];
	    ಯದಿ (!c->sanitizer.check_array (data, 4)) ಪ್ರತ್ಯಾಗಮ;
	    hb_barrier ();
	    ಪೂರ್ಣಾಙ್ಕ markX = *data++;
	    ಪೂರ್ಣಾಙ್ಕ markY = *data++;
	    ಪೂರ್ಣಾಙ್ಕ currX = *data++;
	    ಪೂರ್ಣಾಙ್ಕ currY = *data++;

	    o.x_offset = hb_saturate_sub (c->font->em_scale_x (markX), c->font->em_scale_x (currX));
	    o.y_offset = hb_saturate_sub (c->font->em_scale_y (markY), c->font->em_scale_y (currY));
	  }
	  ಭಙ್ಗ;
	}
	o.attach_type() = OT::Layout::GPOS_impl::ATTACH_TYPE_MARK;
	o.attach_chain() = (ಪೂರ್ಣಾಙ್ಕ) mark - (ಪೂರ್ಣಾಙ್ಕ) buffer->idx;
	ಯದಿ (c->buffer_is_reversed)
	  o.attach_chain() = -o.attach_chain();
	buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT;
      }

      ಯದಿ (entry.flags & Mark)
      {
	mark_set = ಸತ್ಯ;
	mark = buffer->idx;
      }
    }

    ಸಾರ್ವಜನಿಕ:
    hb_aat_apply_context_t *c;
    ಧ್ರುವ KerxSubTableFormat4 *table;
    ಗುಪ್ತ:
    unsigned ಪೂರ್ಣಾಙ್ಕ action_type;
    ಧ್ರುವ HBUINT16 *ankrData;
    ತಾರ್ಕಿಕ mark_set;
    unsigned ಪೂರ್ಣಾಙ್ಕ mark;
  };

  ತಾರ್ಕಿಕ apply (hb_aat_apply_context_t *c) ಧ್ರುವ
  {
    TRACE_APPLY (ಏತತ್);

    driver_context_t dc (ಏತತ್, c);

    StateTableDriver<Types, EntryData, Flags> driver (machine, c->font->face);

    driver.drive (&dc, c);

    return_trace (ಸತ್ಯ);
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    /* The rest of array sanitizations are done at run-time. */
    return_trace (likely (c->check_struct (ಏತತ್) &&
			  machine.sanitize (c)));
  }

  template <typename set_t>
  ಶೂನ್ಯ collect_glyphs (set_t &first_set, set_t &second_set, unsigned ಅಂಕೆ_glyphs) ಧ್ರುವ
  {
    machine.collect_initial_glyphs (first_set, ಅಂಕೆ_glyphs, *ಏತತ್);
    //machine.collect_glyphs (second_set, num_glyphs); // second_set is unused for machine kerning
  }

  ರಕ್ಷಿತ:
  KernSubTableHeader		header;
  StateTable<Types, EntryData>	machine;
  HBUINT32			flags;
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (KernSubTableHeader::static_size + (StateTable<Types, EntryData>::static_size + HBUINT32::static_size));
};

template <typename KernSubTableHeader>
ಸಂರಚನಾ KerxSubTableFormat6
{
  ಗಣನಾ Flags
  {
    ಮೌಲ್ಯಗಳು_Are_Long	= 0x00000001,
  };

  ತಾರ್ಕಿಕ is_long () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ flags & ಮೌಲ್ಯಗಳು_Are_Long; }

  ಪೂರ್ಣಾಙ್ಕ get_kerning (hb_codepoint_t left, hb_codepoint_t ಬಲ,
		   hb_aat_apply_context_t *c) ಧ್ರುವ
  {
    unsigned ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_glyphs = c->sanitizer.get_ಅಂಕೆ_glyphs ();
    ಯದಿ (is_long ())
    {
      ಧ್ರುವ auto &t = u.l;
      unsigned ಪೂರ್ಣಾಙ್ಕ l = (ಏತತ್+t.rowIndexTable).get_ಮೌಲ್ಯ_or_null (left, ಅಂಕೆ_glyphs);
      unsigned ಪೂರ್ಣಾಙ್ಕ r = (ಏತತ್+t.columnIndexTable).get_ಮೌಲ್ಯ_or_null (ಬಲ, ಅಂಕೆ_glyphs);
      unsigned ಪೂರ್ಣಾಙ್ಕ offset = l + r;
      ಯದಿ (unlikely (offset < l)) ಪ್ರತ್ಯಾಗಮ 0; /* Addition overflow. */
      ಯದಿ (unlikely (hb_unsigned_mul_overflows (offset, ಪರಿಮಾಣ (FWORD32)))) ಪ್ರತ್ಯಾಗಮ 0;
      ಧ್ರುವ FWORD32 *v = &StructAtOffset<FWORD32> (&(ಏತತ್+t.array), offset * ಪರಿಮಾಣ (FWORD32));
      ಯದಿ (unlikely (!v->sanitize (&c->sanitizer))) ಪ್ರತ್ಯಾಗಮ 0;
      hb_barrier ();
      ಪ್ರತ್ಯಾಗಮ kerxTupleKern (*v, header.tuple_ಎಣಿಕೆ_2 (), &(ಏತತ್+vector), c);
    }
    ಅನ್ಯಥಾ
    {
      ಧ್ರುವ auto &t = u.s;
      unsigned ಪೂರ್ಣಾಙ್ಕ l = (ಏತತ್+t.rowIndexTable).get_ಮೌಲ್ಯ_or_null (left, ಅಂಕೆ_glyphs);
      unsigned ಪೂರ್ಣಾಙ್ಕ r = (ಏತತ್+t.columnIndexTable).get_ಮೌಲ್ಯ_or_null (ಬಲ, ಅಂಕೆ_glyphs);
      unsigned ಪೂರ್ಣಾಙ್ಕ offset = l + r;
      ಧ್ರುವ FWORD *v = &StructAtOffset<FWORD> (&(ಏತತ್+t.array), offset * ಪರಿಮಾಣ (FWORD));
      ಯದಿ (unlikely (!v->sanitize (&c->sanitizer))) ಪ್ರತ್ಯಾಗಮ 0;
      hb_barrier ();
      ಪ್ರತ್ಯಾಗಮ kerxTupleKern (*v, header.tuple_ಎಣಿಕೆ_2 (), &(ಏತತ್+vector), c);
    }
  }

  ತಾರ್ಕಿಕ apply (hb_aat_apply_context_t *c) ಧ್ರುವ
  {
    TRACE_APPLY (ಏತತ್);

    ಯದಿ (!c->plan->requested_kerning)
      return_trace (ಅಸತ್ಯ);

    ಯದಿ (header.coverage & header.Backwards)
      return_trace (ಅಸತ್ಯ);

    accelerator_t accel (*ಏತತ್, c);
    hb_kern_machine_t<accelerator_t> machine (accel, header.coverage & header.CrossStream);
    machine.kern (c->font, c->buffer, c->plan->kern_mask);

    return_trace (ಸತ್ಯ);
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (likely (c->check_struct (ಏತತ್) &&
			  hb_barrier () &&
			  (is_long () ?
			   (
			     u.l.rowIndexTable.sanitize (c, ಏತತ್) &&
			     u.l.columnIndexTable.sanitize (c, ಏತತ್) &&
			     c->check_range (ಏತತ್, u.l.array)
			   ) : (
			     u.s.rowIndexTable.sanitize (c, ಏತತ್) &&
			     u.s.columnIndexTable.sanitize (c, ಏತತ್) &&
			     c->check_range (ಏತತ್, u.s.array)
			   )) &&
			  (header.tuple_ಎಣಿಕೆ_2 () == 0 ||
			   c->check_range (ಏತತ್, vector))));
  }

  template <typename set_t>
  ಶೂನ್ಯ collect_glyphs (set_t &first_set, set_t &second_set, unsigned ಅಂಕೆ_glyphs) ಧ್ರುವ
  {
    ಯದಿ (is_long ())
    {
      ಧ್ರುವ auto &t = u.l;
      (ಏತತ್+t.rowIndexTable).collect_glyphs (first_set, ಅಂಕೆ_glyphs);
      (ಏತತ್+t.columnIndexTable).collect_glyphs (second_set, ಅಂಕೆ_glyphs);
    }
    ಅನ್ಯಥಾ
    {
      ಧ್ರುವ auto &t = u.s;
      (ಏತತ್+t.rowIndexTable).collect_glyphs (first_set, ಅಂಕೆ_glyphs);
      (ಏತತ್+t.columnIndexTable).collect_glyphs (second_set, ಅಂಕೆ_glyphs);
    }
  }

  ಸಂರಚನಾ accelerator_t
  {
    ಧ್ರುವ KerxSubTableFormat6 &table;
    hb_aat_apply_context_t *c;

    accelerator_t (ಧ್ರುವ KerxSubTableFormat6 &table_,
		   hb_aat_apply_context_t *c_) :
		     table (table_), c (c_) {}

    ಪೂರ್ಣಾಙ್ಕ get_kerning (hb_codepoint_t left, hb_codepoint_t ಬಲ) ಧ್ರುವ
    {
      ಯದಿ (!(*c->first_set)[left] || !(*c->second_set)[ಬಲ]) ಪ್ರತ್ಯಾಗಮ 0;
      ಪ್ರತ್ಯಾಗಮ table.get_kerning (left, ಬಲ, c);
    }
  };

  ರಕ್ಷಿತ:
  KernSubTableHeader		header;
  HBUINT32			flags;
  HBUINT16			row_ಎಣಿಕೆ;
  HBUINT16			column_ಎಣಿಕೆ;
  ಸಙ್ಘ U
  {
    ಸಂರಚನಾ Long
    {
      NNOffset32To<Lookup<HBUINT32>>		rowIndexTable;
      NNOffset32To<Lookup<HBUINT32>>		columnIndexTable;
      NNOffset32To<UnsizedArrayOf<FWORD32>>	array;
    } l;
    ಸಂರಚನಾ Short
    {
      NNOffset32To<Lookup<HBUINT16>>		rowIndexTable;
      NNOffset32To<Lookup<HBUINT16>>		columnIndexTable;
      NNOffset32To<UnsizedArrayOf<FWORD>>	array;
    } s;
  } u;
  NNOffset32To<UnsizedArrayOf<FWORD>>	vector;
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (KernSubTableHeader::static_size + 24);
};


ಸಂರಚನಾ KerxSubTableHeader
{
  ಪ್ರಕಾರನಿರ್ದೇಶ ExtendedTypes Types;

  unsigned   tuple_ಎಣಿಕೆ_2 () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ tuple_ಎಣಿಕೆ; }
  ತಾರ್ಕಿಕ     is_horizontal () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ !(coverage & Vertical); }

  ಗಣನಾ Coverage
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

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್));
  }

  ಸಾರ್ವಜನಿಕ:
  HBUINT32	length;
  HBUINT32	coverage;
  HBUINT32	tuple_ಎಣಿಕೆ;
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (12);
};

ಸಂರಚನಾ KerxSubTable
{
  friend ಸಂರಚನಾ kerx;

  size_t get_size () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ u.header.length; }
  unsigned ಪೂರ್ಣಾಙ್ಕ get_type () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ u.header.coverage & u.header.SubtableType; }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ಧ್ರುವ
  {
    unsigned ಪೂರ್ಣಾಙ್ಕ subtable_type = get_type ();
    TRACE_DISPATCH (ಏತತ್, subtable_type);
    ವಿಕಲ್ಪ (subtable_type) {
    ಪಕ್ಷ 0:	hb_barrier (); return_trace (c->dispatch (u.format0, std::forward<Ts> (ds)...));
    ಪಕ್ಷ 1:	hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    ಪಕ್ಷ 2:	hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    ಪಕ್ಷ 4:	hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
    ಪಕ್ಷ 6:	hb_barrier (); return_trace (c->dispatch (u.format6, std::forward<Ts> (ds)...));
    ಸ್ವಭಾವ:	return_trace (c->default_return_ಮೌಲ್ಯ ());
    }
  }

  template <typename set_t>
  ಶೂನ್ಯ collect_glyphs (set_t &first_set, set_t &second_set, unsigned ಅಂಕೆ_glyphs) ಧ್ರುವ
  {
    unsigned ಪೂರ್ಣಾಙ್ಕ subtable_type = get_type ();
    ವಿಕಲ್ಪ (subtable_type) {
    ಪಕ್ಷ 0:	hb_barrier (); u.format0.collect_glyphs (first_set, second_set, ಅಂಕೆ_glyphs); ಪ್ರತ್ಯಾಗಮ;
    ಪಕ್ಷ 1:	hb_barrier (); u.format1.collect_glyphs (first_set, second_set, ಅಂಕೆ_glyphs); ಪ್ರತ್ಯಾಗಮ;
    ಪಕ್ಷ 2:	hb_barrier (); u.format2.collect_glyphs (first_set, second_set, ಅಂಕೆ_glyphs); ಪ್ರತ್ಯಾಗಮ;
    ಪಕ್ಷ 4:	hb_barrier (); u.format4.collect_glyphs (first_set, second_set, ಅಂಕೆ_glyphs); ಪ್ರತ್ಯಾಗಮ;
    ಪಕ್ಷ 6:	hb_barrier (); u.format6.collect_glyphs (first_set, second_set, ಅಂಕೆ_glyphs); ಪ್ರತ್ಯಾಗಮ;
    ಸ್ವಭಾವ:	ಪ್ರತ್ಯಾಗಮ;
    }
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    ಯದಿ (!(u.header.sanitize (c) &&
	  hb_barrier () &&
	  u.header.length >= u.header.static_size &&
	  c->check_range (ಏತತ್, u.header.length)))
      return_trace (ಅಸತ್ಯ);

    return_trace (dispatch (c));
  }

  ಸಾರ್ವಜನಿಕ:
  ಸಙ್ಘ {
  KerxSubTableHeader				header;
  KerxSubTableFormat0<KerxSubTableHeader>	format0;
  KerxSubTableFormat1<KerxSubTableHeader>	format1;
  KerxSubTableFormat2<KerxSubTableHeader>	format2;
  KerxSubTableFormat4<KerxSubTableHeader>	format4;
  KerxSubTableFormat6<KerxSubTableHeader>	format6;
  } u;
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_MIN (12);
};


/*
 * The 'kerx' Table
 */

ಸಂರಚನಾ kern_subtable_accelerator_data_t
{
  hb_bit_set_t first_set;
  hb_bit_set_t second_set;
  mutable hb_aat_class_cache_t class_cache;
};

ಸಂರಚನಾ kern_accelerator_data_t
{
  hb_vector_t<kern_subtable_accelerator_data_t> subtable_accels;
  hb_aat_scratch_t scratch;
};

template <typename T>
ಸಂರಚನಾ KerxTable
{
  /* https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern */
  ಧ್ರುವ T* thiz () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ static_cast<ಧ್ರುವ T *> (ಏತತ್); }

  ತಾರ್ಕಿಕ has_state_machine () ಧ್ರುವ
  {
    ಪ್ರಕಾರನಿರ್ದೇಶ typename T::SubTable SubTable;

    ಧ್ರುವ SubTable *st = &thiz()->firstSubTable;
    unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = thiz()->table_ಎಣಿಕೆ;
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಎಣಿಕೆ; i++)
    {
      ಯದಿ (st->get_type () == 1)
	ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;

      // TODO: What about format 4? What's this API used for anyway?

      st = &StructAfter<SubTable> (*st);
    }
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
  }

  ತಾರ್ಕಿಕ has_cross_stream () ಧ್ರುವ
  {
    ಪ್ರಕಾರನಿರ್ದೇಶ typename T::SubTable SubTable;

    ಧ್ರುವ SubTable *st = &thiz()->firstSubTable;
    unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = thiz()->table_ಎಣಿಕೆ;
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಎಣಿಕೆ; i++)
    {
      ಯದಿ (st->u.header.coverage & st->u.header.CrossStream)
	ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
      st = &StructAfter<SubTable> (*st);
    }
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
  }

  ಪೂರ್ಣಾಙ್ಕ get_h_kerning (hb_codepoint_t left, hb_codepoint_t ಬಲ) ಧ್ರುವ
  {
    ಪ್ರಕಾರನಿರ್ದೇಶ typename T::SubTable SubTable;

    ಪೂರ್ಣಾಙ್ಕ v = 0;
    ಧ್ರುವ SubTable *st = &thiz()->firstSubTable;
    unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = thiz()->table_ಎಣಿಕೆ;
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಎಣಿಕೆ; i++)
    {
      ಯದಿ ((st->u.header.coverage & (st->u.header.Variation | st->u.header.CrossStream)) ||
	  !st->u.header.is_horizontal ())
	ಅನುವರ್ತ;
      v += st->get_kerning (left, ಬಲ);
      st = &StructAfter<SubTable> (*st);
    }
    ಪ್ರತ್ಯಾಗಮ v;
  }

  ತಾರ್ಕಿಕ apply (AAT::hb_aat_apply_context_t *c,
	      ಧ್ರುವ kern_accelerator_data_t &accel_data) ಧ್ರುವ
  {
    c->buffer->unsafe_to_concat ();

    c->setup_buffer_glyph_set ();

    ಪ್ರಕಾರನಿರ್ದೇಶ typename T::SubTable SubTable;

    ತಾರ್ಕಿಕ ret = ಅಸತ್ಯ;
    ತಾರ್ಕಿಕ seenCrossStream = ಅಸತ್ಯ;
    c->set_lookup_index (0);
    ಧ್ರುವ SubTable *st = &thiz()->firstSubTable;
    unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = thiz()->table_ಎಣಿಕೆ;
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಎಣಿಕೆ; i++)
    {
      ತಾರ್ಕಿಕ reverse;

      auto &subtable_accel = accel_data.subtable_accels[i];

      ಯದಿ (!T::Types::extended && (st->u.header.coverage & st->u.header.Variation))
	ಗಚ್ಛ skip;

      ಯದಿ (HB_DIRECTION_IS_HORIZONTAL (c->buffer->props.direction) != st->u.header.is_horizontal ())
	ಗಚ್ಛ skip;

      c->first_set = &subtable_accel.first_set;
      c->second_set = &subtable_accel.second_set;
      c->machine_class_cache = &subtable_accel.class_cache;

      ಯದಿ (!c->buffer_intersects_machine ())
      {
	(ಶೂನ್ಯ) c->buffer->message (c->font, "skipped subtable %u because no glyph matches", c->lookup_index);
	ಗಚ್ಛ skip;
      }

      reverse = ತಾರ್ಕಿಕ (st->u.header.coverage & st->u.header.Backwards) !=
		HB_DIRECTION_IS_BACKWARD (c->buffer->props.direction);

      ಯದಿ (!c->buffer->message (c->font, "start subtable %u", c->lookup_index))
	ಗಚ್ಛ skip;

      ಯದಿ (!seenCrossStream &&
	  (st->u.header.coverage & st->u.header.CrossStream))
      {
	/* Attach all glyphs into a chain. */
	seenCrossStream = ಸತ್ಯ;
	hb_glyph_position_t *pos = c->buffer->pos;
	unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = c->buffer->len;
	ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಎಣಿಕೆ; i++)
	{
	  pos[i].attach_type() = OT::Layout::GPOS_impl::ATTACH_TYPE_CURSIVE;
	  pos[i].attach_chain() = HB_DIRECTION_IS_FORWARD (c->buffer->props.direction) ? -1 : +1;
	  /* We intentionally don't set HB_BUFFER_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT,
	   * since there needs to be a non-zero attachment for post-positioning to
	   * be needed. */
	}
      }

      ಯದಿ (reverse != c->buffer_is_reversed)
        c->reverse_buffer ();

      {
	/* See comment in sanitize() for conditional here. */
	hb_sanitize_with_object_t with (&c->sanitizer, i < ಎಣಿಕೆ - 1 ? st : (ಧ್ರುವ SubTable *) nullptr);
	ret |= st->dispatch (c);
      }

      (ಶೂನ್ಯ) c->buffer->message (c->font, "end subtable %u", c->lookup_index);

    skip:
      st = &StructAfter<SubTable> (*st);
      c->set_lookup_index (c->lookup_index + 1);
    }
    ಯದಿ (c->buffer_is_reversed)
      c->reverse_buffer ();

    ಪ್ರತ್ಯಾಗಮ ret;
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    ಯದಿ (unlikely (!(thiz()->version.sanitize (c) &&
		    hb_barrier () &&
		    (unsigned) thiz()->version >= (unsigned) T::minVersion &&
		    thiz()->table_ಎಣಿಕೆ.sanitize (c))))
      return_trace (ಅಸತ್ಯ);

    ಪ್ರಕಾರನಿರ್ದೇಶ typename T::SubTable SubTable;

    ಧ್ರುವ SubTable *st = &thiz()->firstSubTable;
    unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = thiz()->table_ಎಣಿಕೆ;
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಎಣಿಕೆ; i++)
    {
      ಯದಿ (unlikely (!st->u.header.sanitize (c)))
	return_trace (ಅಸತ್ಯ);
      hb_barrier ();
      /* OpenType kern table has 2-byte subtable lengths.  That's limiting.
       * MS implementation also only supports one subtable, of format 0,
       * anyway.  Certain versions of some fonts, like Calibry, contain
       * kern subtable that exceeds 64kb.  Looks like, the subtable length
       * is simply ignored.  Which makes sense.  It's only needed if you
       * have multiple subtables.  To handle such fonts, we just ignore
       * the length for the last subtable. */
      hb_sanitize_with_object_t with (c, i < ಎಣಿಕೆ - 1 ? st : (ಧ್ರುವ SubTable *) nullptr);

      ಯದಿ (unlikely (!st->sanitize (c)))
	return_trace (ಅಸತ್ಯ);

      st = &StructAfter<SubTable> (*st);
    }

    unsigned majorVersion = thiz()->version;
    ಯದಿ (ಪರಿಮಾಣ (thiz()->version) == 4)
      majorVersion = majorVersion >> 16;
    ಯದಿ (majorVersion >= 3)
    {
      ಧ್ರುವ SubtableGlyphCoverage *coverage = (ಧ್ರುವ SubtableGlyphCoverage *) st;
      ಯದಿ (!coverage->sanitize (c, ಎಣಿಕೆ))
        return_trace (ಅಸತ್ಯ);
    }

    return_trace (ಸತ್ಯ);
  }

  kern_accelerator_data_t create_accelerator_data (unsigned ಅಂಕೆ_glyphs) ಧ್ರುವ
  {
    kern_accelerator_data_t accel_data;

    ಪ್ರಕಾರನಿರ್ದೇಶ typename T::SubTable SubTable;

    ಧ್ರುವ SubTable *st = &thiz()->firstSubTable;
    unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = thiz()->table_ಎಣಿಕೆ;
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಎಣಿಕೆ; i++)
    {
      auto &subtable_accel = *accel_data.subtable_accels.push ();
      ಯದಿ (unlikely (accel_data.subtable_accels.in_error ()))
	  ಪ್ರತ್ಯಾಗಮ accel_data;

      st->collect_glyphs (subtable_accel.first_set, subtable_accel.second_set, ಅಂಕೆ_glyphs);
      subtable_accel.class_cache.clear ();

      st = &StructAfter<SubTable> (*st);
    }

    ಪ್ರತ್ಯಾಗಮ accel_data;
  }

  ಸಂರಚನಾ accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      hb_sanitize_context_t sc;
      ಏತತ್->table = sc.reference_table<T> (face);
      ಏತತ್->accel_data = ಏತತ್->table->create_accelerator_data (face->get_ಅಂಕೆ_glyphs ());
    }
    ~accelerator_t ()
    {
      ಏತತ್->table.destroy ();
    }

    hb_blob_t *get_blob () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ table.get_blob (); }

    ತಾರ್ಕಿಕ apply (AAT::hb_aat_apply_context_t *c) ಧ್ರುವ
    {
      ಪ್ರತ್ಯಾಗಮ table->apply (c, accel_data);
    }

    hb_blob_ptr_t<T> table;
    kern_accelerator_data_t accel_data;
    hb_aat_scratch_t scratch;
  };
};

ಸಂರಚನಾ kerx : KerxTable<kerx>
{
  friend ಸಂರಚನಾ KerxTable<kerx>;

  ಸ್ಥಿರ constexpr hb_tag_t tableTag = HB_AAT_TAG_kerx;
  ಸ್ಥಿರ constexpr unsigned minVersion = 2u;

  ಪ್ರಕಾರನಿರ್ದೇಶ KerxSubTableHeader SubTableHeader;
  ಪ್ರಕಾರನಿರ್ದೇಶ SubTableHeader::Types Types;
  ಪ್ರಕಾರನಿರ್ದೇಶ KerxSubTable SubTable;

  ತಾರ್ಕಿಕ has_data () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ version; }

  ರಕ್ಷಿತ:
  HBUINT16	version;	/* The version number of the extended kerning table
				 * (currently 2, 3, or 4). */
  HBUINT16	unused;		/* Set to 0. */
  HBUINT32	table_ಎಣಿಕೆ;	/* The number of subtables included in the extended kerning
				 * table. */
  SubTable	firstSubTable;	/* Subtables. */
/*subtableGlyphCoverageArray*/	/* Only if version >= 3. We don't use. */

  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_MIN (8);
};

ಸಂರಚನಾ kerx_accelerator_t : kerx::accelerator_t {
  kerx_accelerator_t (hb_face_t *face) : kerx::accelerator_t (face) {}
};

} /* namespace AAT */

#endif /* HB_AAT_LAYOUT_KERX_TABLE_HH */
