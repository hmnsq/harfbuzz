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

#അന്തര്ഭാവ "hb-kern.hh"
#അന്തര്ഭാവ "hb-aat-layout-ankr-table.hh"
#അന്തര്ഭാവ "hb-set-digest.hh"

/*
 * kerx -- Extended Kerning
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6kerx.html
 */
#define HB_AAT_TAG_kerx HB_TAG('k','e','r','x')


നാമാകാശ AAT {

using നാമാകാശ OT;


സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ
kerxTupleKern (പൂർണ്ണസംഖ്യ value,
	       unsigned പൂർണ്ണസംഖ്യ tuple_എണ്ണം,
	       സ്ഥിരം ശൂന്യം *base,
	       hb_aat_apply_context_t *c)
{
  എങ്കിൽ (likely (!tuple_എണ്ണം || !c)) തിരികെ value;

  unsigned പൂർണ്ണസംഖ്യ offset = value;
  സ്ഥിരം FWORD *pv = &StructAtOffset<FWORD> (base, offset);
  എങ്കിൽ (unlikely (!c->sanitizer.check_array (pv, tuple_എണ്ണം))) തിരികെ 0;
  hb_barrier ();
  തിരികെ *pv;
}


ഘടന hb_glyph_pair_t
{
  hb_codepoint_t left;
  hb_codepoint_t വലത്;
};

ഘടന KernPair
{
  പൂർണ്ണസംഖ്യ get_kerning () സ്ഥിരം { തിരികെ value; }

  പൂർണ്ണസംഖ്യ cmp (സ്ഥിരം hb_glyph_pair_t &o) സ്ഥിരം
  {
    പൂർണ്ണസംഖ്യ ret = left.cmp (o.left);
    എങ്കിൽ (ret) തിരികെ ret;
    തിരികെ വലത്.cmp (o.വലത്);
  }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_struct (ഇത്));
  }

  പൊതു:
  HBGlyphID16	left;
  HBGlyphID16	വലത്;
  FWORD		value;
  പൊതു:
  DEFINE_SIZE_STATIC (6);
};

template <typename KernSubTableHeader>
ഘടന KerxSubTableFormat0
{
  പൂർണ്ണസംഖ്യ get_kerning (hb_codepoint_t left, hb_codepoint_t വലത്,
		   hb_aat_apply_context_t *c = nullptr) സ്ഥിരം
  {
    hb_glyph_pair_t pair = {left, വലത്};
    പൂർണ്ണസംഖ്യ v = pairs.bsearch (pair).get_kerning ();
    തിരികെ kerxTupleKern (v, header.tuple_എണ്ണം_2 (), ഇത്, c);
  }

  തർക്കം apply (hb_aat_apply_context_t *c) സ്ഥിരം
  {
    TRACE_APPLY (ഇത്);

    എങ്കിൽ (!c->plan->requested_kerning)
      return_trace (അസത്യം);

    എങ്കിൽ (header.coverage & header.Backwards)
      return_trace (അസത്യം);

    accelerator_t accel (*ഇത്, c);
    hb_kern_machine_t<accelerator_t> machine (accel, header.coverage & header.CrossStream);
    machine.kern (c->font, c->buffer, c->plan->kern_mask);

    return_trace (സത്യം);
  }

  template <typename set_t>
  ശൂന്യം collect_glyphs (set_t &first_set, set_t &second_set, unsigned അക്കം_glyphs) സ്ഥിരം
  {
    വേണ്ടി (സ്ഥിരം KernPair& pair : pairs)
    {
      first_set.add (pair.left);
      second_set.add (pair.വലത്);
    }
  }

  ഘടന accelerator_t
  {
    സ്ഥിരം KerxSubTableFormat0 &table;
    hb_aat_apply_context_t *c;

    accelerator_t (സ്ഥിരം KerxSubTableFormat0 &table_,
		   hb_aat_apply_context_t *c_) :
		     table (table_), c (c_) {}

    പൂർണ്ണസംഖ്യ get_kerning (hb_codepoint_t left, hb_codepoint_t വലത്) സ്ഥിരം
    {
      എങ്കിൽ (!(*c->first_set)[left] || !(*c->second_set)[വലത്]) തിരികെ 0;
      തിരികെ table.get_kerning (left, വലത്, c);
    }
  };


  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (likely (pairs.sanitize (c)));
  }

  സംരക്ഷിത:
  KernSubTableHeader	header;
  Bin_തിരയൽ_Array_Of<KernPair, typename KernSubTableHeader::Types::HBUINT>
			pairs;	/* Sorted kern records. */
  പൊതു:
  DEFINE_SIZE_ARRAY (KernSubTableHeader::static_size + 16, pairs);
};


template <തർക്കം extended>
ഘടന Format1Entry;

template <>
ഘടന Format1Entry<സത്യം>
{
  എണ്ണൽ Flags
  {
    തള്ളുക		= 0x8000,	/* If set, push this glyph on the kerning stack. */
    DontAdvance		= 0x4000,	/* If set, don't advance to the next glyph
					 * before going to the new state. */
    Reset		= 0x2000,	/* If set, reset the kerning data (clear the stack) */
    Reserved		= 0x1FFF,	/* Not used; set to 0. */
  };

  ഘടന EntryData
  {
    HBUINT16	kernActionIndex;/* Index into the kerning value array. If
				 * this index is 0xFFFF, then no kerning
				 * is to be performed. */
    പൊതു:
    DEFINE_SIZE_STATIC (2);
  };

  സ്ഥിതം തർക്കം initiateAction (സ്ഥിരം Entry<EntryData> &entry)
  { തിരികെ entry.flags & തള്ളുക; }

  സ്ഥിതം തർക്കം performAction (സ്ഥിരം Entry<EntryData> &entry)
  { തിരികെ entry.data.kernActionIndex != 0xFFFF; }

  സ്ഥിതം unsigned പൂർണ്ണസംഖ്യ kernActionIndex (സ്ഥിരം Entry<EntryData> &entry)
  { തിരികെ entry.data.kernActionIndex; }
};
template <>
ഘടന Format1Entry<അസത്യം>
{
  എണ്ണൽ Flags
  {
    തള്ളുക		= 0x8000,	/* If set, push this glyph on the kerning stack. */
    DontAdvance		= 0x4000,	/* If set, don't advance to the next glyph
					 * before going to the new state. */
    Offset		= 0x3FFF,	/* Byte offset from beginning of subtable to the
					 * value table for the glyphs on the kerning stack. */

    Reset		= 0x0000,	/* Not supported? */
  };

  തരനിർവചനം ശൂന്യം EntryData;

  സ്ഥിതം തർക്കം initiateAction (സ്ഥിരം Entry<EntryData> &entry)
  { തിരികെ entry.flags & തള്ളുക; }

  സ്ഥിതം തർക്കം performAction (സ്ഥിരം Entry<EntryData> &entry)
  { തിരികെ entry.flags & Offset; }

  സ്ഥിതം unsigned പൂർണ്ണസംഖ്യ kernActionIndex (സ്ഥിരം Entry<EntryData> &entry)
  { തിരികെ entry.flags & Offset; }
};

template <typename KernSubTableHeader>
ഘടന KerxSubTableFormat1
{
  തരനിർവചനം typename KernSubTableHeader::Types Types;
  തരനിർവചനം typename Types::HBUINT HBUINT;

  തരനിർവചനം Format1Entry<Types::extended> Format1EntryT;
  തരനിർവചനം typename Format1EntryT::EntryData EntryData;

  എണ്ണൽ Flags
  {
    DontAdvance	= Format1EntryT::DontAdvance,
  };

  തർക്കം is_action_initiable (സ്ഥിരം Entry<EntryData> &entry) സ്ഥിരം
  {
    തിരികെ Format1EntryT::initiateAction (entry);
  }
  തർക്കം is_actionable (സ്ഥിരം Entry<EntryData> &entry) സ്ഥിരം
  {
    തിരികെ Format1EntryT::performAction (entry);
  }

  ഘടന driver_context_t
  {
    സ്ഥിതം constexpr തർക്കം in_place = സത്യം;

    driver_context_t (സ്ഥിരം KerxSubTableFormat1 *table_,
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
    ശൂന്യം transition (hb_buffer_t *buffer,
		     StateTableDriver<Types, EntryData, Flags> *driver,
		     സ്ഥിരം Entry<EntryData> &entry)
    {
      unsigned പൂർണ്ണസംഖ്യ flags = entry.flags;

      എങ്കിൽ (flags & Format1EntryT::Reset)
	depth = 0;

      എങ്കിൽ (flags & Format1EntryT::തള്ളുക)
      {
	എങ്കിൽ (likely (depth < ARRAY_LENGTH (അട്ടി)))
	  അട്ടി[depth++] = buffer->idx;
	അല്ലെങ്കിൽ
	  depth = 0; /* Probably not what CoreText does, but better? */
      }

      എങ്കിൽ (Format1EntryT::performAction (entry) && depth)
      {
	unsigned പൂർണ്ണസംഖ്യ tuple_എണ്ണം_2 = hb_max (1u, table->header.tuple_എണ്ണം_2 ());

	unsigned പൂർണ്ണസംഖ്യ kern_idx = Format1EntryT::kernActionIndex (entry);
	kern_idx = Types::byteOffsetToIndex (kern_idx, &table->machine, kernAction.arrayZ);
	സ്ഥിരം FWORD *actions = &kernAction[kern_idx];
	എങ്കിൽ (!c->sanitizer.check_array (actions, depth, tuple_എണ്ണം_2))
	{
	  depth = 0;
	  തിരികെ;
	}
	hb_barrier ();

	hb_mask_t kern_mask = c->plan->kern_mask;

	/* From Apple 'kern' spec:
	 * "Each pops one glyph from the kerning stack and applies the kerning value to it.
	 * The end of the list is marked by an odd value... */
	തർക്കം last = അസത്യം;
	ആകുമ്പോൾ (!last && depth)
	{
	  unsigned പൂർണ്ണസംഖ്യ idx = അട്ടി[--depth];
	  പൂർണ്ണസംഖ്യ v = *actions;
	  actions += tuple_എണ്ണം_2;
	  എങ്കിൽ (idx >= buffer->len) തുടരുക;

	  /* "The end of the list is marked by an odd value..." */
	  last = v & 1;
	  v &= ~1;

	  hb_glyph_position_t &o = buffer->pos[idx];

	  എങ്കിൽ (HB_DIRECTION_IS_HORIZONTAL (buffer->props.direction))
	  {
	    എങ്കിൽ (crossStream)
	    {
	      /* The following flag is undocumented in the spec, but described
	       * in the 'kern' table example. */
	      എങ്കിൽ (v == -0x8000)
	      {
		o.attach_type() = OT::Layout::GPOS_impl::ATTACH_TYPE_NONE;
		o.attach_chain() = 0;
		o.y_offset = 0;
	      }
	      അല്ലെങ്കിൽ എങ്കിൽ (o.attach_type())
	      {
		o.y_offset = hb_saturate_add (o.y_offset, c->font->em_scale_y (v));
		buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT;
	      }
	    }
	    അല്ലെങ്കിൽ എങ്കിൽ (buffer->info[idx].mask & kern_mask)
	    {
	      auto scaled = c->font->em_scale_x (v);
	      o.x_advance = hb_saturate_add (o.x_advance, scaled);
	      o.x_offset = hb_saturate_add (o.x_offset, scaled);
	    }
	  }
	  അല്ലെങ്കിൽ
	  {
	    എങ്കിൽ (crossStream)
	    {
	      /* CoreText doesn't do crossStream kerning in vertical.  We do. */
	      എങ്കിൽ (v == -0x8000)
	      {
		o.attach_type() = OT::Layout::GPOS_impl::ATTACH_TYPE_NONE;
		o.attach_chain() = 0;
		o.x_offset = 0;
	      }
	      അല്ലെങ്കിൽ എങ്കിൽ (o.attach_type())
	      {
		o.x_offset = hb_saturate_add (o.x_offset, c->font->em_scale_x (v));
		buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT;
	      }
	    }
	    അല്ലെങ്കിൽ എങ്കിൽ (buffer->info[idx].mask & kern_mask)
	    {
	      auto scaled = c->font->em_scale_y (v);
	      o.y_advance = hb_saturate_add (o.y_advance, scaled);
	      o.y_offset = hb_saturate_add (o.y_offset, scaled);
	    }
	  }
	}
      }
    }

    പൊതു:
    hb_aat_apply_context_t *c;
    സ്ഥിരം KerxSubTableFormat1 *table;
    സ്വകാര്യ:
    സ്ഥിരം UnsizedArrayOf<FWORD> &kernAction;
    unsigned പൂർണ്ണസംഖ്യ അട്ടി[8];
    unsigned പൂർണ്ണസംഖ്യ depth;
    തർക്കം crossStream;
  };

  തർക്കം apply (hb_aat_apply_context_t *c) സ്ഥിരം
  {
    TRACE_APPLY (ഇത്);

    എങ്കിൽ (!c->plan->requested_kerning &&
	!(header.coverage & header.CrossStream))
      തിരികെ അസത്യം;

    driver_context_t dc (ഇത്, c);

    StateTableDriver<Types, EntryData, Flags> driver (machine, c->font->face);

    driver.drive (&dc, c);

    return_trace (സത്യം);
  }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    /* The rest of array sanitizations are done at run-time. */
    return_trace (likely (c->check_struct (ഇത്) &&
			  machine.sanitize (c)));
  }

  template <typename set_t>
  ശൂന്യം collect_glyphs (set_t &first_set, set_t &second_set, unsigned അക്കം_glyphs) സ്ഥിരം
  {
    machine.collect_initial_glyphs (first_set, അക്കം_glyphs, *ഇത്);
    //machine.collect_glyphs (second_set, num_glyphs); // second_set is unused for machine kerning
  }

  സംരക്ഷിത:
  KernSubTableHeader				header;
  StateTable<Types, EntryData>			machine;
  NNOffsetTo<UnsizedArrayOf<FWORD>, HBUINT>	kernAction;
  പൊതു:
  DEFINE_SIZE_STATIC (KernSubTableHeader::static_size + (StateTable<Types, EntryData>::static_size + HBUINT::static_size));
};

template <typename KernSubTableHeader>
ഘടന KerxSubTableFormat2
{
  തരനിർവചനം typename KernSubTableHeader::Types Types;
  തരനിർവചനം typename Types::HBUINT HBUINT;

  പൂർണ്ണസംഖ്യ get_kerning (hb_codepoint_t left, hb_codepoint_t വലത്,
		   hb_aat_apply_context_t *c) സ്ഥിരം
  {
    unsigned പൂർണ്ണസംഖ്യ അക്കം_glyphs = c->sanitizer.get_അക്കം_glyphs ();
    unsigned പൂർണ്ണസംഖ്യ l = (ഇത്+ഇടത്_Class_Table).get_class (left, അക്കം_glyphs, 0);
    unsigned പൂർണ്ണസംഖ്യ r = (ഇത്+വലത്_Class_Table).get_class (വലത്, അക്കം_glyphs, 0);

    സ്ഥിരം UnsizedArrayOf<FWORD> &arrayZ = ഇത്+array;
    unsigned പൂർണ്ണസംഖ്യ kern_idx = l + r;
    kern_idx = Types::offsetToIndex (kern_idx, ഇത്, arrayZ.arrayZ);
    സ്ഥിരം FWORD *v = &arrayZ[kern_idx];
    എങ്കിൽ (unlikely (!v->sanitize (&c->sanitizer))) തിരികെ 0;
    hb_barrier ();

    തിരികെ kerxTupleKern (*v, header.tuple_എണ്ണം_2 (), ഇത്, c);
  }

  തർക്കം apply (hb_aat_apply_context_t *c) സ്ഥിരം
  {
    TRACE_APPLY (ഇത്);

    എങ്കിൽ (!c->plan->requested_kerning)
      return_trace (അസത്യം);

    എങ്കിൽ (header.coverage & header.Backwards)
      return_trace (അസത്യം);

    accelerator_t accel (*ഇത്, c);
    hb_kern_machine_t<accelerator_t> machine (accel, header.coverage & header.CrossStream);
    machine.kern (c->font, c->buffer, c->plan->kern_mask);

    return_trace (സത്യം);
  }

  template <typename set_t>
  ശൂന്യം collect_glyphs (set_t &first_set, set_t &second_set, unsigned അക്കം_glyphs) സ്ഥിരം
  {
    (ഇത്+ഇടത്_Class_Table).collect_glyphs (first_set, അക്കം_glyphs);
    (ഇത്+വലത്_Class_Table).collect_glyphs (second_set, അക്കം_glyphs);
  }

  ഘടന accelerator_t
  {
    സ്ഥിരം KerxSubTableFormat2 &table;
    hb_aat_apply_context_t *c;

    accelerator_t (സ്ഥിരം KerxSubTableFormat2 &table_,
		   hb_aat_apply_context_t *c_) :
		     table (table_), c (c_) {}

    പൂർണ്ണസംഖ്യ get_kerning (hb_codepoint_t left, hb_codepoint_t വലത്) സ്ഥിരം
    {
      എങ്കിൽ (!(*c->first_set)[left] || !(*c->second_set)[വലത്]) തിരികെ 0;
      തിരികെ table.get_kerning (left, വലത്, c);
    }
  };

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (likely (c->check_struct (ഇത്) &&
			  ഇടത്_Class_Table.sanitize (c, ഇത്) &&
			  വലത്_Class_Table.sanitize (c, ഇത്) &&
			  hb_barrier () &&
			  c->check_range (ഇത്, array)));
  }

  സംരക്ഷിത:
  KernSubTableHeader	header;
  HBUINT		rowWidth;	/* The width, in bytes, of a row in the table. */
  NNOffsetTo<typename Types::ClassTypeWide, HBUINT>
			ഇടത്_Class_Table;	/* Offset from beginning of this subtable to
					 * left-hand class table. */
  NNOffsetTo<typename Types::ClassTypeWide, HBUINT>
			വലത്_Class_Table;/* Offset from beginning of this subtable to
					 * right-hand class table. */
  NNOffsetTo<UnsizedArrayOf<FWORD>, HBUINT>
			 array;		/* Offset from beginning of this subtable to
					 * the start of the kerning array. */
  പൊതു:
  DEFINE_SIZE_STATIC (KernSubTableHeader::static_size + 4 * വലുപ്പം (HBUINT));
};

template <typename KernSubTableHeader>
ഘടന KerxSubTableFormat4
{
  തരനിർവചനം ExtendedTypes Types;

  ഘടന EntryData
  {
    HBUINT16	ankrActionIndex;/* Either 0xFFFF (for no action) or the index of
				 * the action to perform. */
    പൊതു:
    DEFINE_SIZE_STATIC (2);
  };

  എണ്ണൽ Flags
  {
    Mark		= 0x8000,	/* If set, remember this glyph as the marked glyph. */
    DontAdvance		= 0x4000,	/* If set, don't advance to the next glyph before
					 * going to the new state. */
    Reserved		= 0x3FFF,	/* Not used; set to 0. */
  };

  തർക്കം is_action_initiable (സ്ഥിരം Entry<EntryData> &entry) സ്ഥിരം
  {
    തിരികെ (entry.flags & Mark);
  }
  തർക്കം is_actionable (സ്ഥിരം Entry<EntryData> &entry) സ്ഥിരം
  {
    തിരികെ entry.data.ankrActionIndex != 0xFFFF;
  }

  ഘടന driver_context_t
  {
    സ്ഥിതം constexpr തർക്കം in_place = സത്യം;
    എണ്ണൽ SubTableFlags
    {
      ActionType	= 0xC0000000,	/* A two-bit field containing the action type. */
      Unused		= 0x3F000000,	/* Unused - must be zero. */
      Offset		= 0x00FFFFFF,	/* Masks the offset in bytes from the beginning
					 * of the subtable to the beginning of the control
					 * point table. */
    };

    driver_context_t (സ്ഥിരം KerxSubTableFormat4 *table_,
		      hb_aat_apply_context_t *c_) :
	c (c_),
	table (table_),
	action_type ((table->flags & ActionType) >> 30),
	ankrData ((HBUINT16 *) ((സ്ഥിരം അക്ഷരം *) &table->machine + (table->flags & Offset))),
	mark_set (അസത്യം),
	mark (0) {}

    HB_AAT_TRANSITION_INLINE
    ശൂന്യം transition (hb_buffer_t *buffer,
		     StateTableDriver<Types, EntryData, Flags> *driver,
		     സ്ഥിരം Entry<EntryData> &entry)
    {
      എങ്കിൽ (mark_set && entry.data.ankrActionIndex != 0xFFFF && buffer->idx < buffer->len)
      {
	hb_glyph_position_t &o = buffer->cur_pos();
	തിരഞ്ഞെടുപ്പ് (action_type)
	{
	  അവസ്ഥ 0: /* Control Point Actions.*/
	  {
	    /* Indexed into glyph outline. */
	    /* Each action (record in ankrData) contains two 16-bit fields, so we must
	       double the ankrActionIndex to get the correct offset here. */
	    സ്ഥിരം HBUINT16 *data = &ankrData[entry.data.ankrActionIndex * 2];
	    എങ്കിൽ (!c->sanitizer.check_array (data, 2)) തിരികെ;
	    hb_barrier ();
	    unsigned പൂർണ്ണസംഖ്യ markControlPoint = *data++;
	    unsigned പൂർണ്ണസംഖ്യ currControlPoint = *data++;
	    hb_position_t markX = 0;
	    hb_position_t markY = 0;
	    hb_position_t currX = 0;
	    hb_position_t currY = 0;
	    എങ്കിൽ (!c->font->get_glyph_contour_point_for_origin (c->buffer->info[mark].codepoint,
							      markControlPoint,
							      HB_DIRECTION_LTR /*XXX*/,
							      &markX, &markY) ||
		!c->font->get_glyph_contour_point_for_origin (c->buffer->cur ().codepoint,
							      currControlPoint,
							      HB_DIRECTION_LTR /*XXX*/,
							      &currX, &currY))
	      തിരികെ;

	    o.x_offset = hb_saturate_sub (markX, currX);
	    o.y_offset = hb_saturate_sub (markY, currY);
	  }
	  നിർത്തുക;

	  അവസ്ഥ 1: /* Anchor Point Actions. */
	  {
	    /* Indexed into 'ankr' table. */
	    /* Each action (record in ankrData) contains two 16-bit fields, so we must
	       double the ankrActionIndex to get the correct offset here. */
	    സ്ഥിരം HBUINT16 *data = &ankrData[entry.data.ankrActionIndex * 2];
	    എങ്കിൽ (!c->sanitizer.check_array (data, 2)) തിരികെ;
	    hb_barrier ();
	    unsigned പൂർണ്ണസംഖ്യ markAnchorPoint = *data++;
	    unsigned പൂർണ്ണസംഖ്യ currAnchorPoint = *data++;
	    സ്ഥിരം Anchor &markAnchor = c->ankr_table->get_anchor (c->buffer->info[mark].codepoint,
								  markAnchorPoint,
								  c->sanitizer.get_അക്കം_glyphs ());
	    സ്ഥിരം Anchor &currAnchor = c->ankr_table->get_anchor (c->buffer->cur ().codepoint,
								  currAnchorPoint,
								  c->sanitizer.get_അക്കം_glyphs ());

	    o.x_offset = hb_saturate_sub (c->font->em_scale_x (markAnchor.xCoordinate),
					  c->font->em_scale_x (currAnchor.xCoordinate));
	    o.y_offset = hb_saturate_sub (c->font->em_scale_y (markAnchor.yCoordinate),
					  c->font->em_scale_y (currAnchor.yCoordinate));
	  }
	  നിർത്തുക;

	  അവസ്ഥ 2: /* Control Point Coordinate Actions. */
	  {
	    /* Each action contains four 16-bit fields, so we multiply the ankrActionIndex
	       by 4 to get the correct offset for the given action. */
	    സ്ഥിരം FWORD *data = (സ്ഥിരം FWORD *) &ankrData[entry.data.ankrActionIndex * 4];
	    എങ്കിൽ (!c->sanitizer.check_array (data, 4)) തിരികെ;
	    hb_barrier ();
	    പൂർണ്ണസംഖ്യ markX = *data++;
	    പൂർണ്ണസംഖ്യ markY = *data++;
	    പൂർണ്ണസംഖ്യ currX = *data++;
	    പൂർണ്ണസംഖ്യ currY = *data++;

	    o.x_offset = hb_saturate_sub (c->font->em_scale_x (markX), c->font->em_scale_x (currX));
	    o.y_offset = hb_saturate_sub (c->font->em_scale_y (markY), c->font->em_scale_y (currY));
	  }
	  നിർത്തുക;
	}
	o.attach_type() = OT::Layout::GPOS_impl::ATTACH_TYPE_MARK;
	o.attach_chain() = (പൂർണ്ണസംഖ്യ) mark - (പൂർണ്ണസംഖ്യ) buffer->idx;
	എങ്കിൽ (c->buffer_is_reversed)
	  o.attach_chain() = -o.attach_chain();
	buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT;
      }

      എങ്കിൽ (entry.flags & Mark)
      {
	mark_set = സത്യം;
	mark = buffer->idx;
      }
    }

    പൊതു:
    hb_aat_apply_context_t *c;
    സ്ഥിരം KerxSubTableFormat4 *table;
    സ്വകാര്യ:
    unsigned പൂർണ്ണസംഖ്യ action_type;
    സ്ഥിരം HBUINT16 *ankrData;
    തർക്കം mark_set;
    unsigned പൂർണ്ണസംഖ്യ mark;
  };

  തർക്കം apply (hb_aat_apply_context_t *c) സ്ഥിരം
  {
    TRACE_APPLY (ഇത്);

    driver_context_t dc (ഇത്, c);

    StateTableDriver<Types, EntryData, Flags> driver (machine, c->font->face);

    driver.drive (&dc, c);

    return_trace (സത്യം);
  }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    /* The rest of array sanitizations are done at run-time. */
    return_trace (likely (c->check_struct (ഇത്) &&
			  machine.sanitize (c)));
  }

  template <typename set_t>
  ശൂന്യം collect_glyphs (set_t &first_set, set_t &second_set, unsigned അക്കം_glyphs) സ്ഥിരം
  {
    machine.collect_initial_glyphs (first_set, അക്കം_glyphs, *ഇത്);
    //machine.collect_glyphs (second_set, num_glyphs); // second_set is unused for machine kerning
  }

  സംരക്ഷിത:
  KernSubTableHeader		header;
  StateTable<Types, EntryData>	machine;
  HBUINT32			flags;
  പൊതു:
  DEFINE_SIZE_STATIC (KernSubTableHeader::static_size + (StateTable<Types, EntryData>::static_size + HBUINT32::static_size));
};

template <typename KernSubTableHeader>
ഘടന KerxSubTableFormat6
{
  എണ്ണൽ Flags
  {
    മൂല്യങ്ങൾ_Are_Long	= 0x00000001,
  };

  തർക്കം is_long () സ്ഥിരം { തിരികെ flags & മൂല്യങ്ങൾ_Are_Long; }

  പൂർണ്ണസംഖ്യ get_kerning (hb_codepoint_t left, hb_codepoint_t വലത്,
		   hb_aat_apply_context_t *c) സ്ഥിരം
  {
    unsigned പൂർണ്ണസംഖ്യ അക്കം_glyphs = c->sanitizer.get_അക്കം_glyphs ();
    എങ്കിൽ (is_long ())
    {
      സ്ഥിരം auto &t = u.l;
      unsigned പൂർണ്ണസംഖ്യ l = (ഇത്+t.rowIndexTable).get_മൂല്യം_or_null (left, അക്കം_glyphs);
      unsigned പൂർണ്ണസംഖ്യ r = (ഇത്+t.columnIndexTable).get_മൂല്യം_or_null (വലത്, അക്കം_glyphs);
      unsigned പൂർണ്ണസംഖ്യ offset = l + r;
      എങ്കിൽ (unlikely (offset < l)) തിരികെ 0; /* Addition overflow. */
      എങ്കിൽ (unlikely (hb_unsigned_mul_overflows (offset, വലുപ്പം (FWORD32)))) തിരികെ 0;
      സ്ഥിരം FWORD32 *v = &StructAtOffset<FWORD32> (&(ഇത്+t.array), offset * വലുപ്പം (FWORD32));
      എങ്കിൽ (unlikely (!v->sanitize (&c->sanitizer))) തിരികെ 0;
      hb_barrier ();
      തിരികെ kerxTupleKern (*v, header.tuple_എണ്ണം_2 (), &(ഇത്+vector), c);
    }
    അല്ലെങ്കിൽ
    {
      സ്ഥിരം auto &t = u.s;
      unsigned പൂർണ്ണസംഖ്യ l = (ഇത്+t.rowIndexTable).get_മൂല്യം_or_null (left, അക്കം_glyphs);
      unsigned പൂർണ്ണസംഖ്യ r = (ഇത്+t.columnIndexTable).get_മൂല്യം_or_null (വലത്, അക്കം_glyphs);
      unsigned പൂർണ്ണസംഖ്യ offset = l + r;
      സ്ഥിരം FWORD *v = &StructAtOffset<FWORD> (&(ഇത്+t.array), offset * വലുപ്പം (FWORD));
      എങ്കിൽ (unlikely (!v->sanitize (&c->sanitizer))) തിരികെ 0;
      hb_barrier ();
      തിരികെ kerxTupleKern (*v, header.tuple_എണ്ണം_2 (), &(ഇത്+vector), c);
    }
  }

  തർക്കം apply (hb_aat_apply_context_t *c) സ്ഥിരം
  {
    TRACE_APPLY (ഇത്);

    എങ്കിൽ (!c->plan->requested_kerning)
      return_trace (അസത്യം);

    എങ്കിൽ (header.coverage & header.Backwards)
      return_trace (അസത്യം);

    accelerator_t accel (*ഇത്, c);
    hb_kern_machine_t<accelerator_t> machine (accel, header.coverage & header.CrossStream);
    machine.kern (c->font, c->buffer, c->plan->kern_mask);

    return_trace (സത്യം);
  }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (likely (c->check_struct (ഇത്) &&
			  hb_barrier () &&
			  (is_long () ?
			   (
			     u.l.rowIndexTable.sanitize (c, ഇത്) &&
			     u.l.columnIndexTable.sanitize (c, ഇത്) &&
			     c->check_range (ഇത്, u.l.array)
			   ) : (
			     u.s.rowIndexTable.sanitize (c, ഇത്) &&
			     u.s.columnIndexTable.sanitize (c, ഇത്) &&
			     c->check_range (ഇത്, u.s.array)
			   )) &&
			  (header.tuple_എണ്ണം_2 () == 0 ||
			   c->check_range (ഇത്, vector))));
  }

  template <typename set_t>
  ശൂന്യം collect_glyphs (set_t &first_set, set_t &second_set, unsigned അക്കം_glyphs) സ്ഥിരം
  {
    എങ്കിൽ (is_long ())
    {
      സ്ഥിരം auto &t = u.l;
      (ഇത്+t.rowIndexTable).collect_glyphs (first_set, അക്കം_glyphs);
      (ഇത്+t.columnIndexTable).collect_glyphs (second_set, അക്കം_glyphs);
    }
    അല്ലെങ്കിൽ
    {
      സ്ഥിരം auto &t = u.s;
      (ഇത്+t.rowIndexTable).collect_glyphs (first_set, അക്കം_glyphs);
      (ഇത്+t.columnIndexTable).collect_glyphs (second_set, അക്കം_glyphs);
    }
  }

  ഘടന accelerator_t
  {
    സ്ഥിരം KerxSubTableFormat6 &table;
    hb_aat_apply_context_t *c;

    accelerator_t (സ്ഥിരം KerxSubTableFormat6 &table_,
		   hb_aat_apply_context_t *c_) :
		     table (table_), c (c_) {}

    പൂർണ്ണസംഖ്യ get_kerning (hb_codepoint_t left, hb_codepoint_t വലത്) സ്ഥിരം
    {
      എങ്കിൽ (!(*c->first_set)[left] || !(*c->second_set)[വലത്]) തിരികെ 0;
      തിരികെ table.get_kerning (left, വലത്, c);
    }
  };

  സംരക്ഷിത:
  KernSubTableHeader		header;
  HBUINT32			flags;
  HBUINT16			row_എണ്ണം;
  HBUINT16			column_എണ്ണം;
  സംഘം U
  {
    ഘടന Long
    {
      NNOffset32To<Lookup<HBUINT32>>		rowIndexTable;
      NNOffset32To<Lookup<HBUINT32>>		columnIndexTable;
      NNOffset32To<UnsizedArrayOf<FWORD32>>	array;
    } l;
    ഘടന Short
    {
      NNOffset32To<Lookup<HBUINT16>>		rowIndexTable;
      NNOffset32To<Lookup<HBUINT16>>		columnIndexTable;
      NNOffset32To<UnsizedArrayOf<FWORD>>	array;
    } s;
  } u;
  NNOffset32To<UnsizedArrayOf<FWORD>>	vector;
  പൊതു:
  DEFINE_SIZE_STATIC (KernSubTableHeader::static_size + 24);
};


ഘടന KerxSubTableHeader
{
  തരനിർവചനം ExtendedTypes Types;

  unsigned   tuple_എണ്ണം_2 () സ്ഥിരം { തിരികെ tuple_എണ്ണം; }
  തർക്കം     is_horizontal () സ്ഥിരം { തിരികെ !(coverage & Vertical); }

  എണ്ണൽ Coverage
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

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_struct (ഇത്));
  }

  പൊതു:
  HBUINT32	length;
  HBUINT32	coverage;
  HBUINT32	tuple_എണ്ണം;
  പൊതു:
  DEFINE_SIZE_STATIC (12);
};

ഘടന KerxSubTable
{
  friend ഘടന kerx;

  size_t get_size () സ്ഥിരം { തിരികെ u.header.length; }
  unsigned പൂർണ്ണസംഖ്യ get_type () സ്ഥിരം { തിരികെ u.header.coverage & u.header.SubtableType; }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) സ്ഥിരം
  {
    unsigned പൂർണ്ണസംഖ്യ subtable_type = get_type ();
    TRACE_DISPATCH (ഇത്, subtable_type);
    തിരഞ്ഞെടുപ്പ് (subtable_type) {
    അവസ്ഥ 0:	hb_barrier (); return_trace (c->dispatch (u.format0, std::forward<Ts> (ds)...));
    അവസ്ഥ 1:	hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    അവസ്ഥ 2:	hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    അവസ്ഥ 4:	hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
    അവസ്ഥ 6:	hb_barrier (); return_trace (c->dispatch (u.format6, std::forward<Ts> (ds)...));
    സ്ഥിരസ്ഥിതി:	return_trace (c->default_return_മൂല്യം ());
    }
  }

  template <typename set_t>
  ശൂന്യം collect_glyphs (set_t &first_set, set_t &second_set, unsigned അക്കം_glyphs) സ്ഥിരം
  {
    unsigned പൂർണ്ണസംഖ്യ subtable_type = get_type ();
    തിരഞ്ഞെടുപ്പ് (subtable_type) {
    അവസ്ഥ 0:	hb_barrier (); u.format0.collect_glyphs (first_set, second_set, അക്കം_glyphs); തിരികെ;
    അവസ്ഥ 1:	hb_barrier (); u.format1.collect_glyphs (first_set, second_set, അക്കം_glyphs); തിരികെ;
    അവസ്ഥ 2:	hb_barrier (); u.format2.collect_glyphs (first_set, second_set, അക്കം_glyphs); തിരികെ;
    അവസ്ഥ 4:	hb_barrier (); u.format4.collect_glyphs (first_set, second_set, അക്കം_glyphs); തിരികെ;
    അവസ്ഥ 6:	hb_barrier (); u.format6.collect_glyphs (first_set, second_set, അക്കം_glyphs); തിരികെ;
    സ്ഥിരസ്ഥിതി:	തിരികെ;
    }
  }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    എങ്കിൽ (!(u.header.sanitize (c) &&
	  hb_barrier () &&
	  u.header.length >= u.header.static_size &&
	  c->check_range (ഇത്, u.header.length)))
      return_trace (അസത്യം);

    return_trace (dispatch (c));
  }

  പൊതു:
  സംഘം {
  KerxSubTableHeader				header;
  KerxSubTableFormat0<KerxSubTableHeader>	format0;
  KerxSubTableFormat1<KerxSubTableHeader>	format1;
  KerxSubTableFormat2<KerxSubTableHeader>	format2;
  KerxSubTableFormat4<KerxSubTableHeader>	format4;
  KerxSubTableFormat6<KerxSubTableHeader>	format6;
  } u;
  പൊതു:
  DEFINE_SIZE_MIN (12);
};


/*
 * The 'kerx' Table
 */

ഘടന kern_subtable_accelerator_data_t
{
  hb_bit_set_t first_set;
  hb_bit_set_t second_set;
  mutable hb_aat_class_cache_t class_cache;
};

ഘടന kern_accelerator_data_t
{
  hb_vector_t<kern_subtable_accelerator_data_t> subtable_accels;
  hb_aat_scratch_t scratch;
};

template <typename T>
ഘടന KerxTable
{
  /* https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern */
  സ്ഥിരം T* thiz () സ്ഥിരം { തിരികെ static_cast<സ്ഥിരം T *> (ഇത്); }

  തർക്കം has_state_machine () സ്ഥിരം
  {
    തരനിർവചനം typename T::SubTable SubTable;

    സ്ഥിരം SubTable *st = &thiz()->firstSubTable;
    unsigned പൂർണ്ണസംഖ്യ എണ്ണം = thiz()->table_എണ്ണം;
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < എണ്ണം; i++)
    {
      എങ്കിൽ (st->get_type () == 1)
	തിരികെ സത്യം;

      // TODO: What about format 4? What's this API used for anyway?

      st = &StructAfter<SubTable> (*st);
    }
    തിരികെ അസത്യം;
  }

  തർക്കം has_cross_stream () സ്ഥിരം
  {
    തരനിർവചനം typename T::SubTable SubTable;

    സ്ഥിരം SubTable *st = &thiz()->firstSubTable;
    unsigned പൂർണ്ണസംഖ്യ എണ്ണം = thiz()->table_എണ്ണം;
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < എണ്ണം; i++)
    {
      എങ്കിൽ (st->u.header.coverage & st->u.header.CrossStream)
	തിരികെ സത്യം;
      st = &StructAfter<SubTable> (*st);
    }
    തിരികെ അസത്യം;
  }

  പൂർണ്ണസംഖ്യ get_h_kerning (hb_codepoint_t left, hb_codepoint_t വലത്) സ്ഥിരം
  {
    തരനിർവചനം typename T::SubTable SubTable;

    പൂർണ്ണസംഖ്യ v = 0;
    സ്ഥിരം SubTable *st = &thiz()->firstSubTable;
    unsigned പൂർണ്ണസംഖ്യ എണ്ണം = thiz()->table_എണ്ണം;
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < എണ്ണം; i++)
    {
      എങ്കിൽ ((st->u.header.coverage & (st->u.header.Variation | st->u.header.CrossStream)) ||
	  !st->u.header.is_horizontal ())
	തുടരുക;
      v += st->get_kerning (left, വലത്);
      st = &StructAfter<SubTable> (*st);
    }
    തിരികെ v;
  }

  തർക്കം apply (AAT::hb_aat_apply_context_t *c,
	      സ്ഥിരം kern_accelerator_data_t &accel_data) സ്ഥിരം
  {
    c->buffer->unsafe_to_concat ();

    c->setup_buffer_glyph_set ();

    തരനിർവചനം typename T::SubTable SubTable;

    തർക്കം ret = അസത്യം;
    തർക്കം seenCrossStream = അസത്യം;
    c->set_lookup_index (0);
    സ്ഥിരം SubTable *st = &thiz()->firstSubTable;
    unsigned പൂർണ്ണസംഖ്യ എണ്ണം = thiz()->table_എണ്ണം;
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < എണ്ണം; i++)
    {
      തർക്കം reverse;

      auto &subtable_accel = accel_data.subtable_accels[i];

      എങ്കിൽ (!T::Types::extended && (st->u.header.coverage & st->u.header.Variation))
	പോകുക skip;

      എങ്കിൽ (HB_DIRECTION_IS_HORIZONTAL (c->buffer->props.direction) != st->u.header.is_horizontal ())
	പോകുക skip;

      c->first_set = &subtable_accel.first_set;
      c->second_set = &subtable_accel.second_set;
      c->machine_class_cache = &subtable_accel.class_cache;

      എങ്കിൽ (!c->buffer_intersects_machine ())
      {
	(ശൂന്യം) c->buffer->message (c->font, "skipped subtable %u because no glyph matches", c->lookup_index);
	പോകുക skip;
      }

      reverse = തർക്കം (st->u.header.coverage & st->u.header.Backwards) !=
		HB_DIRECTION_IS_BACKWARD (c->buffer->props.direction);

      എങ്കിൽ (!c->buffer->message (c->font, "start subtable %u", c->lookup_index))
	പോകുക skip;

      എങ്കിൽ (!seenCrossStream &&
	  (st->u.header.coverage & st->u.header.CrossStream))
      {
	/* Attach all glyphs into a chain. */
	seenCrossStream = സത്യം;
	hb_glyph_position_t *pos = c->buffer->pos;
	unsigned പൂർണ്ണസംഖ്യ എണ്ണം = c->buffer->len;
	വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < എണ്ണം; i++)
	{
	  pos[i].attach_type() = OT::Layout::GPOS_impl::ATTACH_TYPE_CURSIVE;
	  pos[i].attach_chain() = HB_DIRECTION_IS_FORWARD (c->buffer->props.direction) ? -1 : +1;
	  /* We intentionally don't set HB_BUFFER_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT,
	   * since there needs to be a non-zero attachment for post-positioning to
	   * be needed. */
	}
      }

      എങ്കിൽ (reverse != c->buffer_is_reversed)
        c->reverse_buffer ();

      {
	/* See comment in sanitize() for conditional here. */
	hb_sanitize_with_object_t with (&c->sanitizer, i < എണ്ണം - 1 ? st : (സ്ഥിരം SubTable *) nullptr);
	ret |= st->dispatch (c);
      }

      (ശൂന്യം) c->buffer->message (c->font, "end subtable %u", c->lookup_index);

    skip:
      st = &StructAfter<SubTable> (*st);
      c->set_lookup_index (c->lookup_index + 1);
    }
    എങ്കിൽ (c->buffer_is_reversed)
      c->reverse_buffer ();

    തിരികെ ret;
  }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    എങ്കിൽ (unlikely (!(thiz()->version.sanitize (c) &&
		    hb_barrier () &&
		    (unsigned) thiz()->version >= (unsigned) T::minVersion &&
		    thiz()->table_എണ്ണം.sanitize (c))))
      return_trace (അസത്യം);

    തരനിർവചനം typename T::SubTable SubTable;

    സ്ഥിരം SubTable *st = &thiz()->firstSubTable;
    unsigned പൂർണ്ണസംഖ്യ എണ്ണം = thiz()->table_എണ്ണം;
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < എണ്ണം; i++)
    {
      എങ്കിൽ (unlikely (!st->u.header.sanitize (c)))
	return_trace (അസത്യം);
      hb_barrier ();
      /* OpenType kern table has 2-byte subtable lengths.  That's limiting.
       * MS implementation also only supports one subtable, of format 0,
       * anyway.  Certain versions of some fonts, like Calibry, contain
       * kern subtable that exceeds 64kb.  Looks like, the subtable length
       * is simply ignored.  Which makes sense.  It's only needed if you
       * have multiple subtables.  To handle such fonts, we just ignore
       * the length for the last subtable. */
      hb_sanitize_with_object_t with (c, i < എണ്ണം - 1 ? st : (സ്ഥിരം SubTable *) nullptr);

      എങ്കിൽ (unlikely (!st->sanitize (c)))
	return_trace (അസത്യം);

      st = &StructAfter<SubTable> (*st);
    }

    unsigned majorVersion = thiz()->version;
    എങ്കിൽ (വലുപ്പം (thiz()->version) == 4)
      majorVersion = majorVersion >> 16;
    എങ്കിൽ (majorVersion >= 3)
    {
      സ്ഥിരം SubtableGlyphCoverage *coverage = (സ്ഥിരം SubtableGlyphCoverage *) st;
      എങ്കിൽ (!coverage->sanitize (c, എണ്ണം))
        return_trace (അസത്യം);
    }

    return_trace (സത്യം);
  }

  kern_accelerator_data_t create_accelerator_data (unsigned അക്കം_glyphs) സ്ഥിരം
  {
    kern_accelerator_data_t accel_data;

    തരനിർവചനം typename T::SubTable SubTable;

    സ്ഥിരം SubTable *st = &thiz()->firstSubTable;
    unsigned പൂർണ്ണസംഖ്യ എണ്ണം = thiz()->table_എണ്ണം;
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < എണ്ണം; i++)
    {
      auto &subtable_accel = *accel_data.subtable_accels.push ();
      എങ്കിൽ (unlikely (accel_data.subtable_accels.in_error ()))
	  തിരികെ accel_data;

      st->collect_glyphs (subtable_accel.first_set, subtable_accel.second_set, അക്കം_glyphs);
      subtable_accel.class_cache.clear ();

      st = &StructAfter<SubTable> (*st);
    }

    തിരികെ accel_data;
  }

  ഘടന accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      hb_sanitize_context_t sc;
      ഇത്->table = sc.reference_table<T> (face);
      ഇത്->accel_data = ഇത്->table->create_accelerator_data (face->get_അക്കം_glyphs ());
    }
    ~accelerator_t ()
    {
      ഇത്->table.destroy ();
    }

    hb_blob_t *get_blob () സ്ഥിരം { തിരികെ table.get_blob (); }

    തർക്കം apply (AAT::hb_aat_apply_context_t *c) സ്ഥിരം
    {
      തിരികെ table->apply (c, accel_data);
    }

    hb_blob_ptr_t<T> table;
    kern_accelerator_data_t accel_data;
    hb_aat_scratch_t scratch;
  };
};

ഘടന kerx : KerxTable<kerx>
{
  friend ഘടന KerxTable<kerx>;

  സ്ഥിതം constexpr hb_tag_t tableTag = HB_AAT_TAG_kerx;
  സ്ഥിതം constexpr unsigned minVersion = 2u;

  തരനിർവചനം KerxSubTableHeader SubTableHeader;
  തരനിർവചനം SubTableHeader::Types Types;
  തരനിർവചനം KerxSubTable SubTable;

  തർക്കം has_data () സ്ഥിരം { തിരികെ version; }

  സംരക്ഷിത:
  HBUINT16	version;	/* The version number of the extended kerning table
				 * (currently 2, 3, or 4). */
  HBUINT16	unused;		/* Set to 0. */
  HBUINT32	table_എണ്ണം;	/* The number of subtables included in the extended kerning
				 * table. */
  SubTable	firstSubTable;	/* Subtables. */
/*subtableGlyphCoverageArray*/	/* Only if version >= 3. We don't use. */

  പൊതു:
  DEFINE_SIZE_MIN (8);
};

ഘടന kerx_accelerator_t : kerx::accelerator_t {
  kerx_accelerator_t (hb_face_t *face) : kerx::accelerator_t (face) {}
};

} /* namespace AAT */

#endif /* HB_AAT_LAYOUT_KERX_TABLE_HH */
