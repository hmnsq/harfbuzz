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

#ଅନ୍ତର୍ଭାଵ "hb-kern.hh"
#ଅନ୍ତର୍ଭାଵ "hb-aat-layout-ankr-table.hh"
#ଅନ୍ତର୍ଭାଵ "hb-set-digest.hh"

/*
 * kerx -- Extended Kerning
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6kerx.html
 */
#define HB_AAT_TAG_kerx HB_TAG('k','e','r','x')


ନାମାକାଶ AAT {

using ନାମାକାଶ OT;


ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା
kerxTupleKern (ପୂର୍ଣ୍ଣସଂଖ୍ୟା value,
	       unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା tuple_ଗଣନା,
	       ଅଚଳ ଶୂନ୍ୟ *base,
	       hb_aat_apply_context_t *c)
{
  ଯଦି (likely (!tuple_ଗଣନା || !c)) ଫେରନ୍ତୁ value;

  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା offset = value;
  ଅଚଳ FWORD *pv = &StructAtOffset<FWORD> (base, offset);
  ଯଦି (unlikely (!c->sanitizer.check_array (pv, tuple_ଗଣନା))) ଫେରନ୍ତୁ 0;
  hb_barrier ();
  ଫେରନ୍ତୁ *pv;
}


ସଂରଚନା hb_glyph_pair_t
{
  hb_codepoint_t left;
  hb_codepoint_t ଡାହାଣ;
};

ସଂରଚନା KernPair
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_kerning () ଅଚଳ { ଫେରନ୍ତୁ value; }

  ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmp (ଅଚଳ hb_glyph_pair_t &o) ଅଚଳ
  {
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = left.cmp (o.left);
    ଯଦି (ret) ଫେରନ୍ତୁ ret;
    ଫେରନ୍ତୁ ଡାହାଣ.cmp (o.ଡାହାଣ);
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି));
  }

  ସର୍ବସାଧାରଣ:
  HBGlyphID16	left;
  HBGlyphID16	ଡାହାଣ;
  FWORD		value;
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (6);
};

template <typename KernSubTableHeader>
ସଂରଚନା KerxSubTableFormat0
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_kerning (hb_codepoint_t left, hb_codepoint_t ଡାହାଣ,
		   hb_aat_apply_context_t *c = nullptr) ଅଚଳ
  {
    hb_glyph_pair_t pair = {left, ଡାହାଣ};
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା v = pairs.bsearch (pair).get_kerning ();
    ଫେରନ୍ତୁ kerxTupleKern (v, header.tuple_ଗଣନା_2 (), ଏହି, c);
  }

  ତାର୍କିକ apply (hb_aat_apply_context_t *c) ଅଚଳ
  {
    TRACE_APPLY (ଏହି);

    ଯଦି (!c->plan->requested_kerning)
      return_trace (ମିଥ୍ୟା);

    ଯଦି (header.coverage & header.Backwards)
      return_trace (ମିଥ୍ୟା);

    accelerator_t accel (*ଏହି, c);
    hb_kern_machine_t<accelerator_t> machine (accel, header.coverage & header.CrossStream);
    machine.kern (c->font, c->buffer, c->plan->kern_mask);

    return_trace (ସତ୍ୟ);
  }

  template <typename set_t>
  ଶୂନ୍ୟ collect_glyphs (set_t &first_set, set_t &second_set, unsigned ଅଙ୍କ_glyphs) ଅଚଳ
  {
    ପାଇଁ (ଅଚଳ KernPair& pair : pairs)
    {
      first_set.add (pair.left);
      second_set.add (pair.ଡାହାଣ);
    }
  }

  ସଂରଚନା accelerator_t
  {
    ଅଚଳ KerxSubTableFormat0 &table;
    hb_aat_apply_context_t *c;

    accelerator_t (ଅଚଳ KerxSubTableFormat0 &table_,
		   hb_aat_apply_context_t *c_) :
		     table (table_), c (c_) {}

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_kerning (hb_codepoint_t left, hb_codepoint_t ଡାହାଣ) ଅଚଳ
    {
      ଯଦି (!(*c->first_set)[left] || !(*c->second_set)[ଡାହାଣ]) ଫେରନ୍ତୁ 0;
      ଫେରନ୍ତୁ table.get_kerning (left, ଡାହାଣ, c);
    }
  };


  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (likely (pairs.sanitize (c)));
  }

  ସୁରକ୍ଷିତ:
  KernSubTableHeader	header;
  Bin_ସନ୍ଧାନ_Array_Of<KernPair, typename KernSubTableHeader::Types::HBUINT>
			pairs;	/* Sorted kern records. */
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_ARRAY (KernSubTableHeader::static_size + 16, pairs);
};


template <ତାର୍କିକ extended>
ସଂରଚନା Format1Entry;

template <>
ସଂରଚନା Format1Entry<ସତ୍ୟ>
{
  ଗଣନା Flags
  {
    ଠେଲ		= 0x8000,	/* If set, push this glyph on the kerning stack. */
    DontAdvance		= 0x4000,	/* If set, don't advance to the next glyph
					 * before going to the new state. */
    Reset		= 0x2000,	/* If set, reset the kerning data (clear the stack) */
    Reserved		= 0x1FFF,	/* Not used; set to 0. */
  };

  ସଂରଚନା EntryData
  {
    HBUINT16	kernActionIndex;/* Index into the kerning value array. If
				 * this index is 0xFFFF, then no kerning
				 * is to be performed. */
    ସର୍ବସାଧାରଣ:
    DEFINE_SIZE_STATIC (2);
  };

  ସ୍ଥିର ତାର୍କିକ initiateAction (ଅଚଳ Entry<EntryData> &entry)
  { ଫେରନ୍ତୁ entry.flags & ଠେଲ; }

  ସ୍ଥିର ତାର୍କିକ performAction (ଅଚଳ Entry<EntryData> &entry)
  { ଫେରନ୍ତୁ entry.data.kernActionIndex != 0xFFFF; }

  ସ୍ଥିର unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା kernActionIndex (ଅଚଳ Entry<EntryData> &entry)
  { ଫେରନ୍ତୁ entry.data.kernActionIndex; }
};
template <>
ସଂରଚନା Format1Entry<ମିଥ୍ୟା>
{
  ଗଣନା Flags
  {
    ଠେଲ		= 0x8000,	/* If set, push this glyph on the kerning stack. */
    DontAdvance		= 0x4000,	/* If set, don't advance to the next glyph
					 * before going to the new state. */
    Offset		= 0x3FFF,	/* Byte offset from beginning of subtable to the
					 * value table for the glyphs on the kerning stack. */

    Reset		= 0x0000,	/* Not supported? */
  };

  ପ୍ରକାରବ୍ୟାଖ୍ୟା ଶୂନ୍ୟ EntryData;

  ସ୍ଥିର ତାର୍କିକ initiateAction (ଅଚଳ Entry<EntryData> &entry)
  { ଫେରନ୍ତୁ entry.flags & ଠେଲ; }

  ସ୍ଥିର ତାର୍କିକ performAction (ଅଚଳ Entry<EntryData> &entry)
  { ଫେରନ୍ତୁ entry.flags & Offset; }

  ସ୍ଥିର unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା kernActionIndex (ଅଚଳ Entry<EntryData> &entry)
  { ଫେରନ୍ତୁ entry.flags & Offset; }
};

template <typename KernSubTableHeader>
ସଂରଚନା KerxSubTableFormat1
{
  ପ୍ରକାରବ୍ୟାଖ୍ୟା typename KernSubTableHeader::Types Types;
  ପ୍ରକାରବ୍ୟାଖ୍ୟା typename Types::HBUINT HBUINT;

  ପ୍ରକାରବ୍ୟାଖ୍ୟା Format1Entry<Types::extended> Format1EntryT;
  ପ୍ରକାରବ୍ୟାଖ୍ୟା typename Format1EntryT::EntryData EntryData;

  ଗଣନା Flags
  {
    DontAdvance	= Format1EntryT::DontAdvance,
  };

  ତାର୍କିକ is_action_initiable (ଅଚଳ Entry<EntryData> &entry) ଅଚଳ
  {
    ଫେରନ୍ତୁ Format1EntryT::initiateAction (entry);
  }
  ତାର୍କିକ is_actionable (ଅଚଳ Entry<EntryData> &entry) ଅଚଳ
  {
    ଫେରନ୍ତୁ Format1EntryT::performAction (entry);
  }

  ସଂରଚନା driver_context_t
  {
    ସ୍ଥିର constexpr ତାର୍କିକ in_place = ସତ୍ୟ;

    driver_context_t (ଅଚଳ KerxSubTableFormat1 *table_,
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
    ଶୂନ୍ୟ transition (hb_buffer_t *buffer,
		     StateTableDriver<Types, EntryData, Flags> *driver,
		     ଅଚଳ Entry<EntryData> &entry)
    {
      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags = entry.flags;

      ଯଦି (flags & Format1EntryT::Reset)
	depth = 0;

      ଯଦି (flags & Format1EntryT::ଠେଲ)
      {
	ଯଦି (likely (depth < ARRAY_LENGTH (ଗଦା)))
	  ଗଦା[depth++] = buffer->idx;
	ଅନ୍ୟଥା
	  depth = 0; /* Probably not what CoreText does, but better? */
      }

      ଯଦି (Format1EntryT::performAction (entry) && depth)
      {
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା tuple_ଗଣନା_2 = hb_max (1u, table->header.tuple_ଗଣନା_2 ());

	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା kern_idx = Format1EntryT::kernActionIndex (entry);
	kern_idx = Types::byteOffsetToIndex (kern_idx, &table->machine, kernAction.arrayZ);
	ଅଚଳ FWORD *actions = &kernAction[kern_idx];
	ଯଦି (!c->sanitizer.check_array (actions, depth, tuple_ଗଣନା_2))
	{
	  depth = 0;
	  ଫେରନ୍ତୁ;
	}
	hb_barrier ();

	hb_mask_t kern_mask = c->plan->kern_mask;

	/* From Apple 'kern' spec:
	 * "Each pops one glyph from the kerning stack and applies the kerning value to it.
	 * The end of the list is marked by an odd value... */
	ତାର୍କିକ last = ମିଥ୍ୟା;
	ଯେତେବେଳେ (!last && depth)
	{
	  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା idx = ଗଦା[--depth];
	  ପୂର୍ଣ୍ଣସଂଖ୍ୟା v = *actions;
	  actions += tuple_ଗଣନା_2;
	  ଯଦି (idx >= buffer->len) ଜାରି;

	  /* "The end of the list is marked by an odd value..." */
	  last = v & 1;
	  v &= ~1;

	  hb_glyph_position_t &o = buffer->pos[idx];

	  ଯଦି (HB_DIRECTION_IS_HORIZONTAL (buffer->props.direction))
	  {
	    ଯଦି (crossStream)
	    {
	      /* The following flag is undocumented in the spec, but described
	       * in the 'kern' table example. */
	      ଯଦି (v == -0x8000)
	      {
		o.attach_type() = OT::Layout::GPOS_impl::ATTACH_TYPE_NONE;
		o.attach_chain() = 0;
		o.y_offset = 0;
	      }
	      ଅନ୍ୟଥା ଯଦି (o.attach_type())
	      {
		o.y_offset = hb_saturate_add (o.y_offset, c->font->em_scale_y (v));
		buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT;
	      }
	    }
	    ଅନ୍ୟଥା ଯଦି (buffer->info[idx].mask & kern_mask)
	    {
	      auto scaled = c->font->em_scale_x (v);
	      o.x_advance = hb_saturate_add (o.x_advance, scaled);
	      o.x_offset = hb_saturate_add (o.x_offset, scaled);
	    }
	  }
	  ଅନ୍ୟଥା
	  {
	    ଯଦି (crossStream)
	    {
	      /* CoreText doesn't do crossStream kerning in vertical.  We do. */
	      ଯଦି (v == -0x8000)
	      {
		o.attach_type() = OT::Layout::GPOS_impl::ATTACH_TYPE_NONE;
		o.attach_chain() = 0;
		o.x_offset = 0;
	      }
	      ଅନ୍ୟଥା ଯଦି (o.attach_type())
	      {
		o.x_offset = hb_saturate_add (o.x_offset, c->font->em_scale_x (v));
		buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT;
	      }
	    }
	    ଅନ୍ୟଥା ଯଦି (buffer->info[idx].mask & kern_mask)
	    {
	      auto scaled = c->font->em_scale_y (v);
	      o.y_advance = hb_saturate_add (o.y_advance, scaled);
	      o.y_offset = hb_saturate_add (o.y_offset, scaled);
	    }
	  }
	}
      }
    }

    ସର୍ବସାଧାରଣ:
    hb_aat_apply_context_t *c;
    ଅଚଳ KerxSubTableFormat1 *table;
    ବ୍ୟକ୍ତିଗତ:
    ଅଚଳ UnsizedArrayOf<FWORD> &kernAction;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଦା[8];
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth;
    ତାର୍କିକ crossStream;
  };

  ତାର୍କିକ apply (hb_aat_apply_context_t *c) ଅଚଳ
  {
    TRACE_APPLY (ଏହି);

    ଯଦି (!c->plan->requested_kerning &&
	!(header.coverage & header.CrossStream))
      ଫେରନ୍ତୁ ମିଥ୍ୟା;

    driver_context_t dc (ଏହି, c);

    StateTableDriver<Types, EntryData, Flags> driver (machine, c->font->face);

    driver.drive (&dc, c);

    return_trace (ସତ୍ୟ);
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    /* The rest of array sanitizations are done at run-time. */
    return_trace (likely (c->check_struct (ଏହି) &&
			  machine.sanitize (c)));
  }

  template <typename set_t>
  ଶୂନ୍ୟ collect_glyphs (set_t &first_set, set_t &second_set, unsigned ଅଙ୍କ_glyphs) ଅଚଳ
  {
    machine.collect_initial_glyphs (first_set, ଅଙ୍କ_glyphs, *ଏହି);
    //machine.collect_glyphs (second_set, num_glyphs); // second_set is unused for machine kerning
  }

  ସୁରକ୍ଷିତ:
  KernSubTableHeader				header;
  StateTable<Types, EntryData>			machine;
  NNOffsetTo<UnsizedArrayOf<FWORD>, HBUINT>	kernAction;
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (KernSubTableHeader::static_size + (StateTable<Types, EntryData>::static_size + HBUINT::static_size));
};

template <typename KernSubTableHeader>
ସଂରଚନା KerxSubTableFormat2
{
  ପ୍ରକାରବ୍ୟାଖ୍ୟା typename KernSubTableHeader::Types Types;
  ପ୍ରକାରବ୍ୟାଖ୍ୟା typename Types::HBUINT HBUINT;

  ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_kerning (hb_codepoint_t left, hb_codepoint_t ଡାହାଣ,
		   hb_aat_apply_context_t *c) ଅଚଳ
  {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_glyphs = c->sanitizer.get_ଅଙ୍କ_glyphs ();
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା l = (ଏହି+ବାମ_Class_Table).get_class (left, ଅଙ୍କ_glyphs, 0);
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା r = (ଏହି+ଡାହାଣ_Class_Table).get_class (ଡାହାଣ, ଅଙ୍କ_glyphs, 0);

    ଅଚଳ UnsizedArrayOf<FWORD> &arrayZ = ଏହି+array;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା kern_idx = l + r;
    kern_idx = Types::offsetToIndex (kern_idx, ଏହି, arrayZ.arrayZ);
    ଅଚଳ FWORD *v = &arrayZ[kern_idx];
    ଯଦି (unlikely (!v->sanitize (&c->sanitizer))) ଫେରନ୍ତୁ 0;
    hb_barrier ();

    ଫେରନ୍ତୁ kerxTupleKern (*v, header.tuple_ଗଣନା_2 (), ଏହି, c);
  }

  ତାର୍କିକ apply (hb_aat_apply_context_t *c) ଅଚଳ
  {
    TRACE_APPLY (ଏହି);

    ଯଦି (!c->plan->requested_kerning)
      return_trace (ମିଥ୍ୟା);

    ଯଦି (header.coverage & header.Backwards)
      return_trace (ମିଥ୍ୟା);

    accelerator_t accel (*ଏହି, c);
    hb_kern_machine_t<accelerator_t> machine (accel, header.coverage & header.CrossStream);
    machine.kern (c->font, c->buffer, c->plan->kern_mask);

    return_trace (ସତ୍ୟ);
  }

  template <typename set_t>
  ଶୂନ୍ୟ collect_glyphs (set_t &first_set, set_t &second_set, unsigned ଅଙ୍କ_glyphs) ଅଚଳ
  {
    (ଏହି+ବାମ_Class_Table).collect_glyphs (first_set, ଅଙ୍କ_glyphs);
    (ଏହି+ଡାହାଣ_Class_Table).collect_glyphs (second_set, ଅଙ୍କ_glyphs);
  }

  ସଂରଚନା accelerator_t
  {
    ଅଚଳ KerxSubTableFormat2 &table;
    hb_aat_apply_context_t *c;

    accelerator_t (ଅଚଳ KerxSubTableFormat2 &table_,
		   hb_aat_apply_context_t *c_) :
		     table (table_), c (c_) {}

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_kerning (hb_codepoint_t left, hb_codepoint_t ଡାହାଣ) ଅଚଳ
    {
      ଯଦି (!(*c->first_set)[left] || !(*c->second_set)[ଡାହାଣ]) ଫେରନ୍ତୁ 0;
      ଫେରନ୍ତୁ table.get_kerning (left, ଡାହାଣ, c);
    }
  };

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (likely (c->check_struct (ଏହି) &&
			  ବାମ_Class_Table.sanitize (c, ଏହି) &&
			  ଡାହାଣ_Class_Table.sanitize (c, ଏହି) &&
			  hb_barrier () &&
			  c->check_range (ଏହି, array)));
  }

  ସୁରକ୍ଷିତ:
  KernSubTableHeader	header;
  HBUINT		rowWidth;	/* The width, in bytes, of a row in the table. */
  NNOffsetTo<typename Types::ClassTypeWide, HBUINT>
			ବାମ_Class_Table;	/* Offset from beginning of this subtable to
					 * left-hand class table. */
  NNOffsetTo<typename Types::ClassTypeWide, HBUINT>
			ଡାହାଣ_Class_Table;/* Offset from beginning of this subtable to
					 * right-hand class table. */
  NNOffsetTo<UnsizedArrayOf<FWORD>, HBUINT>
			 array;		/* Offset from beginning of this subtable to
					 * the start of the kerning array. */
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (KernSubTableHeader::static_size + 4 * ଆକାର (HBUINT));
};

template <typename KernSubTableHeader>
ସଂରଚନା KerxSubTableFormat4
{
  ପ୍ରକାରବ୍ୟାଖ୍ୟା ExtendedTypes Types;

  ସଂରଚନା EntryData
  {
    HBUINT16	ankrActionIndex;/* Either 0xFFFF (for no action) or the index of
				 * the action to perform. */
    ସର୍ବସାଧାରଣ:
    DEFINE_SIZE_STATIC (2);
  };

  ଗଣନା Flags
  {
    Mark		= 0x8000,	/* If set, remember this glyph as the marked glyph. */
    DontAdvance		= 0x4000,	/* If set, don't advance to the next glyph before
					 * going to the new state. */
    Reserved		= 0x3FFF,	/* Not used; set to 0. */
  };

  ତାର୍କିକ is_action_initiable (ଅଚଳ Entry<EntryData> &entry) ଅଚଳ
  {
    ଫେରନ୍ତୁ (entry.flags & Mark);
  }
  ତାର୍କିକ is_actionable (ଅଚଳ Entry<EntryData> &entry) ଅଚଳ
  {
    ଫେରନ୍ତୁ entry.data.ankrActionIndex != 0xFFFF;
  }

  ସଂରଚନା driver_context_t
  {
    ସ୍ଥିର constexpr ତାର୍କିକ in_place = ସତ୍ୟ;
    ଗଣନା SubTableFlags
    {
      ActionType	= 0xC0000000,	/* A two-bit field containing the action type. */
      Unused		= 0x3F000000,	/* Unused - must be zero. */
      Offset		= 0x00FFFFFF,	/* Masks the offset in bytes from the beginning
					 * of the subtable to the beginning of the control
					 * point table. */
    };

    driver_context_t (ଅଚଳ KerxSubTableFormat4 *table_,
		      hb_aat_apply_context_t *c_) :
	c (c_),
	table (table_),
	action_type ((table->flags & ActionType) >> 30),
	ankrData ((HBUINT16 *) ((ଅଚଳ ଅକ୍ଷର *) &table->machine + (table->flags & Offset))),
	mark_set (ମିଥ୍ୟା),
	mark (0) {}

    HB_AAT_TRANSITION_INLINE
    ଶୂନ୍ୟ transition (hb_buffer_t *buffer,
		     StateTableDriver<Types, EntryData, Flags> *driver,
		     ଅଚଳ Entry<EntryData> &entry)
    {
      ଯଦି (mark_set && entry.data.ankrActionIndex != 0xFFFF && buffer->idx < buffer->len)
      {
	hb_glyph_position_t &o = buffer->cur_pos();
	ବାଛ (action_type)
	{
	  ସ୍ଥିତି 0: /* Control Point Actions.*/
	  {
	    /* Indexed into glyph outline. */
	    /* Each action (record in ankrData) contains two 16-bit fields, so we must
	       double the ankrActionIndex to get the correct offset here. */
	    ଅଚଳ HBUINT16 *data = &ankrData[entry.data.ankrActionIndex * 2];
	    ଯଦି (!c->sanitizer.check_array (data, 2)) ଫେରନ୍ତୁ;
	    hb_barrier ();
	    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା markControlPoint = *data++;
	    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା currControlPoint = *data++;
	    hb_position_t markX = 0;
	    hb_position_t markY = 0;
	    hb_position_t currX = 0;
	    hb_position_t currY = 0;
	    ଯଦି (!c->font->get_glyph_contour_point_for_origin (c->buffer->info[mark].codepoint,
							      markControlPoint,
							      HB_DIRECTION_LTR /*XXX*/,
							      &markX, &markY) ||
		!c->font->get_glyph_contour_point_for_origin (c->buffer->cur ().codepoint,
							      currControlPoint,
							      HB_DIRECTION_LTR /*XXX*/,
							      &currX, &currY))
	      ଫେରନ୍ତୁ;

	    o.x_offset = hb_saturate_sub (markX, currX);
	    o.y_offset = hb_saturate_sub (markY, currY);
	  }
	  ଭାଙ୍ଗ;

	  ସ୍ଥିତି 1: /* Anchor Point Actions. */
	  {
	    /* Indexed into 'ankr' table. */
	    /* Each action (record in ankrData) contains two 16-bit fields, so we must
	       double the ankrActionIndex to get the correct offset here. */
	    ଅଚଳ HBUINT16 *data = &ankrData[entry.data.ankrActionIndex * 2];
	    ଯଦି (!c->sanitizer.check_array (data, 2)) ଫେରନ୍ତୁ;
	    hb_barrier ();
	    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା markAnchorPoint = *data++;
	    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା currAnchorPoint = *data++;
	    ଅଚଳ Anchor &markAnchor = c->ankr_table->get_anchor (c->buffer->info[mark].codepoint,
								  markAnchorPoint,
								  c->sanitizer.get_ଅଙ୍କ_glyphs ());
	    ଅଚଳ Anchor &currAnchor = c->ankr_table->get_anchor (c->buffer->cur ().codepoint,
								  currAnchorPoint,
								  c->sanitizer.get_ଅଙ୍କ_glyphs ());

	    o.x_offset = hb_saturate_sub (c->font->em_scale_x (markAnchor.xCoordinate),
					  c->font->em_scale_x (currAnchor.xCoordinate));
	    o.y_offset = hb_saturate_sub (c->font->em_scale_y (markAnchor.yCoordinate),
					  c->font->em_scale_y (currAnchor.yCoordinate));
	  }
	  ଭାଙ୍ଗ;

	  ସ୍ଥିତି 2: /* Control Point Coordinate Actions. */
	  {
	    /* Each action contains four 16-bit fields, so we multiply the ankrActionIndex
	       by 4 to get the correct offset for the given action. */
	    ଅଚଳ FWORD *data = (ଅଚଳ FWORD *) &ankrData[entry.data.ankrActionIndex * 4];
	    ଯଦି (!c->sanitizer.check_array (data, 4)) ଫେରନ୍ତୁ;
	    hb_barrier ();
	    ପୂର୍ଣ୍ଣସଂଖ୍ୟା markX = *data++;
	    ପୂର୍ଣ୍ଣସଂଖ୍ୟା markY = *data++;
	    ପୂର୍ଣ୍ଣସଂଖ୍ୟା currX = *data++;
	    ପୂର୍ଣ୍ଣସଂଖ୍ୟା currY = *data++;

	    o.x_offset = hb_saturate_sub (c->font->em_scale_x (markX), c->font->em_scale_x (currX));
	    o.y_offset = hb_saturate_sub (c->font->em_scale_y (markY), c->font->em_scale_y (currY));
	  }
	  ଭାଙ୍ଗ;
	}
	o.attach_type() = OT::Layout::GPOS_impl::ATTACH_TYPE_MARK;
	o.attach_chain() = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) mark - (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) buffer->idx;
	ଯଦି (c->buffer_is_reversed)
	  o.attach_chain() = -o.attach_chain();
	buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT;
      }

      ଯଦି (entry.flags & Mark)
      {
	mark_set = ସତ୍ୟ;
	mark = buffer->idx;
      }
    }

    ସର୍ବସାଧାରଣ:
    hb_aat_apply_context_t *c;
    ଅଚଳ KerxSubTableFormat4 *table;
    ବ୍ୟକ୍ତିଗତ:
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା action_type;
    ଅଚଳ HBUINT16 *ankrData;
    ତାର୍କିକ mark_set;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା mark;
  };

  ତାର୍କିକ apply (hb_aat_apply_context_t *c) ଅଚଳ
  {
    TRACE_APPLY (ଏହି);

    driver_context_t dc (ଏହି, c);

    StateTableDriver<Types, EntryData, Flags> driver (machine, c->font->face);

    driver.drive (&dc, c);

    return_trace (ସତ୍ୟ);
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    /* The rest of array sanitizations are done at run-time. */
    return_trace (likely (c->check_struct (ଏହି) &&
			  machine.sanitize (c)));
  }

  template <typename set_t>
  ଶୂନ୍ୟ collect_glyphs (set_t &first_set, set_t &second_set, unsigned ଅଙ୍କ_glyphs) ଅଚଳ
  {
    machine.collect_initial_glyphs (first_set, ଅଙ୍କ_glyphs, *ଏହି);
    //machine.collect_glyphs (second_set, num_glyphs); // second_set is unused for machine kerning
  }

  ସୁରକ୍ଷିତ:
  KernSubTableHeader		header;
  StateTable<Types, EntryData>	machine;
  HBUINT32			flags;
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (KernSubTableHeader::static_size + (StateTable<Types, EntryData>::static_size + HBUINT32::static_size));
};

template <typename KernSubTableHeader>
ସଂରଚନା KerxSubTableFormat6
{
  ଗଣନା Flags
  {
    ମୂଲ୍ୟଗୁଡ଼ିକ_Are_Long	= 0x00000001,
  };

  ତାର୍କିକ is_long () ଅଚଳ { ଫେରନ୍ତୁ flags & ମୂଲ୍ୟଗୁଡ଼ିକ_Are_Long; }

  ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_kerning (hb_codepoint_t left, hb_codepoint_t ଡାହାଣ,
		   hb_aat_apply_context_t *c) ଅଚଳ
  {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_glyphs = c->sanitizer.get_ଅଙ୍କ_glyphs ();
    ଯଦି (is_long ())
    {
      ଅଚଳ auto &t = u.l;
      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା l = (ଏହି+t.rowIndexTable).get_ମୂଲ୍ୟ_or_null (left, ଅଙ୍କ_glyphs);
      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା r = (ଏହି+t.columnIndexTable).get_ମୂଲ୍ୟ_or_null (ଡାହାଣ, ଅଙ୍କ_glyphs);
      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା offset = l + r;
      ଯଦି (unlikely (offset < l)) ଫେରନ୍ତୁ 0; /* Addition overflow. */
      ଯଦି (unlikely (hb_unsigned_mul_overflows (offset, ଆକାର (FWORD32)))) ଫେରନ୍ତୁ 0;
      ଅଚଳ FWORD32 *v = &StructAtOffset<FWORD32> (&(ଏହି+t.array), offset * ଆକାର (FWORD32));
      ଯଦି (unlikely (!v->sanitize (&c->sanitizer))) ଫେରନ୍ତୁ 0;
      hb_barrier ();
      ଫେରନ୍ତୁ kerxTupleKern (*v, header.tuple_ଗଣନା_2 (), &(ଏହି+vector), c);
    }
    ଅନ୍ୟଥା
    {
      ଅଚଳ auto &t = u.s;
      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା l = (ଏହି+t.rowIndexTable).get_ମୂଲ୍ୟ_or_null (left, ଅଙ୍କ_glyphs);
      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା r = (ଏହି+t.columnIndexTable).get_ମୂଲ୍ୟ_or_null (ଡାହାଣ, ଅଙ୍କ_glyphs);
      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା offset = l + r;
      ଅଚଳ FWORD *v = &StructAtOffset<FWORD> (&(ଏହି+t.array), offset * ଆକାର (FWORD));
      ଯଦି (unlikely (!v->sanitize (&c->sanitizer))) ଫେରନ୍ତୁ 0;
      hb_barrier ();
      ଫେରନ୍ତୁ kerxTupleKern (*v, header.tuple_ଗଣନା_2 (), &(ଏହି+vector), c);
    }
  }

  ତାର୍କିକ apply (hb_aat_apply_context_t *c) ଅଚଳ
  {
    TRACE_APPLY (ଏହି);

    ଯଦି (!c->plan->requested_kerning)
      return_trace (ମିଥ୍ୟା);

    ଯଦି (header.coverage & header.Backwards)
      return_trace (ମିଥ୍ୟା);

    accelerator_t accel (*ଏହି, c);
    hb_kern_machine_t<accelerator_t> machine (accel, header.coverage & header.CrossStream);
    machine.kern (c->font, c->buffer, c->plan->kern_mask);

    return_trace (ସତ୍ୟ);
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (likely (c->check_struct (ଏହି) &&
			  hb_barrier () &&
			  (is_long () ?
			   (
			     u.l.rowIndexTable.sanitize (c, ଏହି) &&
			     u.l.columnIndexTable.sanitize (c, ଏହି) &&
			     c->check_range (ଏହି, u.l.array)
			   ) : (
			     u.s.rowIndexTable.sanitize (c, ଏହି) &&
			     u.s.columnIndexTable.sanitize (c, ଏହି) &&
			     c->check_range (ଏହି, u.s.array)
			   )) &&
			  (header.tuple_ଗଣନା_2 () == 0 ||
			   c->check_range (ଏହି, vector))));
  }

  template <typename set_t>
  ଶୂନ୍ୟ collect_glyphs (set_t &first_set, set_t &second_set, unsigned ଅଙ୍କ_glyphs) ଅଚଳ
  {
    ଯଦି (is_long ())
    {
      ଅଚଳ auto &t = u.l;
      (ଏହି+t.rowIndexTable).collect_glyphs (first_set, ଅଙ୍କ_glyphs);
      (ଏହି+t.columnIndexTable).collect_glyphs (second_set, ଅଙ୍କ_glyphs);
    }
    ଅନ୍ୟଥା
    {
      ଅଚଳ auto &t = u.s;
      (ଏହି+t.rowIndexTable).collect_glyphs (first_set, ଅଙ୍କ_glyphs);
      (ଏହି+t.columnIndexTable).collect_glyphs (second_set, ଅଙ୍କ_glyphs);
    }
  }

  ସଂରଚନା accelerator_t
  {
    ଅଚଳ KerxSubTableFormat6 &table;
    hb_aat_apply_context_t *c;

    accelerator_t (ଅଚଳ KerxSubTableFormat6 &table_,
		   hb_aat_apply_context_t *c_) :
		     table (table_), c (c_) {}

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_kerning (hb_codepoint_t left, hb_codepoint_t ଡାହାଣ) ଅଚଳ
    {
      ଯଦି (!(*c->first_set)[left] || !(*c->second_set)[ଡାହାଣ]) ଫେରନ୍ତୁ 0;
      ଫେରନ୍ତୁ table.get_kerning (left, ଡାହାଣ, c);
    }
  };

  ସୁରକ୍ଷିତ:
  KernSubTableHeader		header;
  HBUINT32			flags;
  HBUINT16			row_ଗଣନା;
  HBUINT16			column_ଗଣନା;
  ସଂଘ U
  {
    ସଂରଚନା Long
    {
      NNOffset32To<Lookup<HBUINT32>>		rowIndexTable;
      NNOffset32To<Lookup<HBUINT32>>		columnIndexTable;
      NNOffset32To<UnsizedArrayOf<FWORD32>>	array;
    } l;
    ସଂରଚନା Short
    {
      NNOffset32To<Lookup<HBUINT16>>		rowIndexTable;
      NNOffset32To<Lookup<HBUINT16>>		columnIndexTable;
      NNOffset32To<UnsizedArrayOf<FWORD>>	array;
    } s;
  } u;
  NNOffset32To<UnsizedArrayOf<FWORD>>	vector;
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (KernSubTableHeader::static_size + 24);
};


ସଂରଚନା KerxSubTableHeader
{
  ପ୍ରକାରବ୍ୟାଖ୍ୟା ExtendedTypes Types;

  unsigned   tuple_ଗଣନା_2 () ଅଚଳ { ଫେରନ୍ତୁ tuple_ଗଣନା; }
  ତାର୍କିକ     is_horizontal () ଅଚଳ { ଫେରନ୍ତୁ !(coverage & Vertical); }

  ଗଣନା Coverage
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

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି));
  }

  ସର୍ବସାଧାରଣ:
  HBUINT32	length;
  HBUINT32	coverage;
  HBUINT32	tuple_ଗଣନା;
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (12);
};

ସଂରଚନା KerxSubTable
{
  friend ସଂରଚନା kerx;

  size_t get_size () ଅଚଳ { ଫେରନ୍ତୁ u.header.length; }
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_type () ଅଚଳ { ଫେରନ୍ତୁ u.header.coverage & u.header.SubtableType; }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ଅଚଳ
  {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା subtable_type = get_type ();
    TRACE_DISPATCH (ଏହି, subtable_type);
    ବାଛ (subtable_type) {
    ସ୍ଥିତି 0:	hb_barrier (); return_trace (c->dispatch (u.format0, std::forward<Ts> (ds)...));
    ସ୍ଥିତି 1:	hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    ସ୍ଥିତି 2:	hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    ସ୍ଥିତି 4:	hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
    ସ୍ଥିତି 6:	hb_barrier (); return_trace (c->dispatch (u.format6, std::forward<Ts> (ds)...));
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:	return_trace (c->default_return_ମୂଲ୍ୟ ());
    }
  }

  template <typename set_t>
  ଶୂନ୍ୟ collect_glyphs (set_t &first_set, set_t &second_set, unsigned ଅଙ୍କ_glyphs) ଅଚଳ
  {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା subtable_type = get_type ();
    ବାଛ (subtable_type) {
    ସ୍ଥିତି 0:	hb_barrier (); u.format0.collect_glyphs (first_set, second_set, ଅଙ୍କ_glyphs); ଫେରନ୍ତୁ;
    ସ୍ଥିତି 1:	hb_barrier (); u.format1.collect_glyphs (first_set, second_set, ଅଙ୍କ_glyphs); ଫେରନ୍ତୁ;
    ସ୍ଥିତି 2:	hb_barrier (); u.format2.collect_glyphs (first_set, second_set, ଅଙ୍କ_glyphs); ଫେରନ୍ତୁ;
    ସ୍ଥିତି 4:	hb_barrier (); u.format4.collect_glyphs (first_set, second_set, ଅଙ୍କ_glyphs); ଫେରନ୍ତୁ;
    ସ୍ଥିତି 6:	hb_barrier (); u.format6.collect_glyphs (first_set, second_set, ଅଙ୍କ_glyphs); ଫେରନ୍ତୁ;
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:	ଫେରନ୍ତୁ;
    }
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    ଯଦି (!(u.header.sanitize (c) &&
	  hb_barrier () &&
	  u.header.length >= u.header.static_size &&
	  c->check_range (ଏହି, u.header.length)))
      return_trace (ମିଥ୍ୟା);

    return_trace (dispatch (c));
  }

  ସର୍ବସାଧାରଣ:
  ସଂଘ {
  KerxSubTableHeader				header;
  KerxSubTableFormat0<KerxSubTableHeader>	format0;
  KerxSubTableFormat1<KerxSubTableHeader>	format1;
  KerxSubTableFormat2<KerxSubTableHeader>	format2;
  KerxSubTableFormat4<KerxSubTableHeader>	format4;
  KerxSubTableFormat6<KerxSubTableHeader>	format6;
  } u;
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_MIN (12);
};


/*
 * The 'kerx' Table
 */

ସଂରଚନା kern_subtable_accelerator_data_t
{
  hb_bit_set_t first_set;
  hb_bit_set_t second_set;
  mutable hb_aat_class_cache_t class_cache;
};

ସଂରଚନା kern_accelerator_data_t
{
  hb_vector_t<kern_subtable_accelerator_data_t> subtable_accels;
  hb_aat_scratch_t scratch;
};

template <typename T>
ସଂରଚନା KerxTable
{
  /* https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern */
  ଅଚଳ T* thiz () ଅଚଳ { ଫେରନ୍ତୁ static_cast<ଅଚଳ T *> (ଏହି); }

  ତାର୍କିକ has_state_machine () ଅଚଳ
  {
    ପ୍ରକାରବ୍ୟାଖ୍ୟା typename T::SubTable SubTable;

    ଅଚଳ SubTable *st = &thiz()->firstSubTable;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = thiz()->table_ଗଣନା;
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଗଣନା_2; i++)
    {
      ଯଦି (st->get_type () == 1)
	ଫେରନ୍ତୁ ସତ୍ୟ;

      // TODO: What about format 4? What's this API used for anyway?

      st = &StructAfter<SubTable> (*st);
    }
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
  }

  ତାର୍କିକ has_cross_stream () ଅଚଳ
  {
    ପ୍ରକାରବ୍ୟାଖ୍ୟା typename T::SubTable SubTable;

    ଅଚଳ SubTable *st = &thiz()->firstSubTable;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = thiz()->table_ଗଣନା;
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଗଣନା_2; i++)
    {
      ଯଦି (st->u.header.coverage & st->u.header.CrossStream)
	ଫେରନ୍ତୁ ସତ୍ୟ;
      st = &StructAfter<SubTable> (*st);
    }
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
  }

  ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_h_kerning (hb_codepoint_t left, hb_codepoint_t ଡାହାଣ) ଅଚଳ
  {
    ପ୍ରକାରବ୍ୟାଖ୍ୟା typename T::SubTable SubTable;

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା v = 0;
    ଅଚଳ SubTable *st = &thiz()->firstSubTable;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = thiz()->table_ଗଣନା;
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଗଣନା_2; i++)
    {
      ଯଦି ((st->u.header.coverage & (st->u.header.Variation | st->u.header.CrossStream)) ||
	  !st->u.header.is_horizontal ())
	ଜାରି;
      v += st->get_kerning (left, ଡାହାଣ);
      st = &StructAfter<SubTable> (*st);
    }
    ଫେରନ୍ତୁ v;
  }

  ତାର୍କିକ apply (AAT::hb_aat_apply_context_t *c,
	      ଅଚଳ kern_accelerator_data_t &accel_data) ଅଚଳ
  {
    c->buffer->unsafe_to_concat ();

    c->setup_buffer_glyph_set ();

    ପ୍ରକାରବ୍ୟାଖ୍ୟା typename T::SubTable SubTable;

    ତାର୍କିକ ret = ମିଥ୍ୟା;
    ତାର୍କିକ seenCrossStream = ମିଥ୍ୟା;
    c->set_lookup_index (0);
    ଅଚଳ SubTable *st = &thiz()->firstSubTable;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = thiz()->table_ଗଣନା;
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଗଣନା_2; i++)
    {
      ତାର୍କିକ reverse;

      auto &subtable_accel = accel_data.subtable_accels[i];

      ଯଦି (!T::Types::extended && (st->u.header.coverage & st->u.header.Variation))
	ଯାଅ skip;

      ଯଦି (HB_DIRECTION_IS_HORIZONTAL (c->buffer->props.direction) != st->u.header.is_horizontal ())
	ଯାଅ skip;

      c->first_set = &subtable_accel.first_set;
      c->second_set = &subtable_accel.second_set;
      c->machine_class_cache = &subtable_accel.class_cache;

      ଯଦି (!c->buffer_intersects_machine ())
      {
	(ଶୂନ୍ୟ) c->buffer->message (c->font, "skipped subtable %u because no glyph matches", c->lookup_index);
	ଯାଅ skip;
      }

      reverse = ତାର୍କିକ (st->u.header.coverage & st->u.header.Backwards) !=
		HB_DIRECTION_IS_BACKWARD (c->buffer->props.direction);

      ଯଦି (!c->buffer->message (c->font, "start subtable %u", c->lookup_index))
	ଯାଅ skip;

      ଯଦି (!seenCrossStream &&
	  (st->u.header.coverage & st->u.header.CrossStream))
      {
	/* Attach all glyphs into a chain. */
	seenCrossStream = ସତ୍ୟ;
	hb_glyph_position_t *pos = c->buffer->pos;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = c->buffer->len;
	ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଗଣନା_2; i++)
	{
	  pos[i].attach_type() = OT::Layout::GPOS_impl::ATTACH_TYPE_CURSIVE;
	  pos[i].attach_chain() = HB_DIRECTION_IS_FORWARD (c->buffer->props.direction) ? -1 : +1;
	  /* We intentionally don't set HB_BUFFER_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT,
	   * since there needs to be a non-zero attachment for post-positioning to
	   * be needed. */
	}
      }

      ଯଦି (reverse != c->buffer_is_reversed)
        c->reverse_buffer ();

      {
	/* See comment in sanitize() for conditional here. */
	hb_sanitize_with_object_t with (&c->sanitizer, i < ଗଣନା_2 - 1 ? st : (ଅଚଳ SubTable *) nullptr);
	ret |= st->dispatch (c);
      }

      (ଶୂନ୍ୟ) c->buffer->message (c->font, "end subtable %u", c->lookup_index);

    skip:
      st = &StructAfter<SubTable> (*st);
      c->set_lookup_index (c->lookup_index + 1);
    }
    ଯଦି (c->buffer_is_reversed)
      c->reverse_buffer ();

    ଫେରନ୍ତୁ ret;
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    ଯଦି (unlikely (!(thiz()->version.sanitize (c) &&
		    hb_barrier () &&
		    (unsigned) thiz()->version >= (unsigned) T::minVersion &&
		    thiz()->table_ଗଣନା.sanitize (c))))
      return_trace (ମିଥ୍ୟା);

    ପ୍ରକାରବ୍ୟାଖ୍ୟା typename T::SubTable SubTable;

    ଅଚଳ SubTable *st = &thiz()->firstSubTable;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = thiz()->table_ଗଣନା;
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଗଣନା_2; i++)
    {
      ଯଦି (unlikely (!st->u.header.sanitize (c)))
	return_trace (ମିଥ୍ୟା);
      hb_barrier ();
      /* OpenType kern table has 2-byte subtable lengths.  That's limiting.
       * MS implementation also only supports one subtable, of format 0,
       * anyway.  Certain versions of some fonts, like Calibry, contain
       * kern subtable that exceeds 64kb.  Looks like, the subtable length
       * is simply ignored.  Which makes sense.  It's only needed if you
       * have multiple subtables.  To handle such fonts, we just ignore
       * the length for the last subtable. */
      hb_sanitize_with_object_t with (c, i < ଗଣନା_2 - 1 ? st : (ଅଚଳ SubTable *) nullptr);

      ଯଦି (unlikely (!st->sanitize (c)))
	return_trace (ମିଥ୍ୟା);

      st = &StructAfter<SubTable> (*st);
    }

    unsigned majorVersion = thiz()->version;
    ଯଦି (ଆକାର (thiz()->version) == 4)
      majorVersion = majorVersion >> 16;
    ଯଦି (majorVersion >= 3)
    {
      ଅଚଳ SubtableGlyphCoverage *coverage = (ଅଚଳ SubtableGlyphCoverage *) st;
      ଯଦି (!coverage->sanitize (c, ଗଣନା_2))
        return_trace (ମିଥ୍ୟା);
    }

    return_trace (ସତ୍ୟ);
  }

  kern_accelerator_data_t create_accelerator_data (unsigned ଅଙ୍କ_glyphs) ଅଚଳ
  {
    kern_accelerator_data_t accel_data;

    ପ୍ରକାରବ୍ୟାଖ୍ୟା typename T::SubTable SubTable;

    ଅଚଳ SubTable *st = &thiz()->firstSubTable;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = thiz()->table_ଗଣନା;
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଗଣନା_2; i++)
    {
      auto &subtable_accel = *accel_data.subtable_accels.push ();
      ଯଦି (unlikely (accel_data.subtable_accels.in_error ()))
	  ଫେରନ୍ତୁ accel_data;

      st->collect_glyphs (subtable_accel.first_set, subtable_accel.second_set, ଅଙ୍କ_glyphs);
      subtable_accel.class_cache.clear ();

      st = &StructAfter<SubTable> (*st);
    }

    ଫେରନ୍ତୁ accel_data;
  }

  ସଂରଚନା accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      hb_sanitize_context_t sc;
      ଏହି->table = sc.reference_table<T> (face);
      ଏହି->accel_data = ଏହି->table->create_accelerator_data (face->get_ଅଙ୍କ_glyphs ());
    }
    ~accelerator_t ()
    {
      ଏହି->table.destroy ();
    }

    hb_blob_t *get_blob () ଅଚଳ { ଫେରନ୍ତୁ table.get_blob (); }

    ତାର୍କିକ apply (AAT::hb_aat_apply_context_t *c) ଅଚଳ
    {
      ଫେରନ୍ତୁ table->apply (c, accel_data);
    }

    hb_blob_ptr_t<T> table;
    kern_accelerator_data_t accel_data;
    hb_aat_scratch_t scratch;
  };
};

ସଂରଚନା kerx : KerxTable<kerx>
{
  friend ସଂରଚନା KerxTable<kerx>;

  ସ୍ଥିର constexpr hb_tag_t tableTag = HB_AAT_TAG_kerx;
  ସ୍ଥିର constexpr unsigned minVersion = 2u;

  ପ୍ରକାରବ୍ୟାଖ୍ୟା KerxSubTableHeader SubTableHeader;
  ପ୍ରକାରବ୍ୟାଖ୍ୟା SubTableHeader::Types Types;
  ପ୍ରକାରବ୍ୟାଖ୍ୟା KerxSubTable SubTable;

  ତାର୍କିକ has_data () ଅଚଳ { ଫେରନ୍ତୁ version; }

  ସୁରକ୍ଷିତ:
  HBUINT16	version;	/* The version number of the extended kerning table
				 * (currently 2, 3, or 4). */
  HBUINT16	unused;		/* Set to 0. */
  HBUINT32	table_ଗଣନା;	/* The number of subtables included in the extended kerning
				 * table. */
  SubTable	firstSubTable;	/* Subtables. */
/*subtableGlyphCoverageArray*/	/* Only if version >= 3. We don't use. */

  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_MIN (8);
};

ସଂରଚନା kerx_accelerator_t : kerx::accelerator_t {
  kerx_accelerator_t (hb_face_t *face) : kerx::accelerator_t (face) {}
};

} /* namespace AAT */

#endif /* HB_AAT_LAYOUT_KERX_TABLE_HH */
