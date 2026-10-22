/*
 * Copyright © 2017  Google, Inc.
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

#ifndef HB_AAT_LAYOUT_MORX_TABLE_HH
#define HB_AAT_LAYOUT_MORX_TABLE_HH

#અન્તર્ભાવ "hb-open-type.hh"
#અન્તર્ભાવ "hb-aat-layout-common.hh"
#અન્તર્ભાવ "hb-ot-layout.hh"
#અન્તર્ભાવ "hb-aat-map.hh"

/*
 * morx -- Extended Glyph Metamorphosis
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6morx.html
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6mort.html
 */
#define HB_AAT_TAG_morx HB_TAG('m','o','r','x')
#define HB_AAT_TAG_mort HB_TAG('m','o','r','t')


નામાકાશ AAT {

using નામાકાશ OT;

template <typename Types>
સંરચના RearrangementSubtable
{
  પ્રકારવ્યાખ્યા typename Types::HBUINT HBUINT;

  પ્રકારવ્યાખ્યા શૂન્ય EntryData;

  ગણના Flags
  {
    MarkFirst		= 0x8000,	/* If set, make the current glyph the first
					 * glyph to be rearranged. */
    DontAdvance		= 0x4000,	/* If set, don't advance to the next glyph
					 * before going to the new state. This means
					 * that the glyph index doesn't change, even
					 * if the glyph at that index has changed. */
    MarkLast		= 0x2000,	/* If set, make the current glyph the last
					 * glyph to be rearranged. */
    Reserved		= 0x1FF0,	/* These bits are reserved and should be set to 0. */
    Verb		= 0x000F,	/* The type of rearrangement specified. */
  };

  તાર્કિક is_action_initiable (અચલ Entry<EntryData> &entry) અચલ
  {
    પાછા (entry.flags & MarkFirst);
  }
  તાર્કિક is_actionable (અચલ Entry<EntryData> &entry) અચલ
  {
    પાછા (entry.flags & Verb);
  }

  સંરચના driver_context_t
  {
    સ્થિર constexpr તાર્કિક in_place = સાચું;

    driver_context_t (અચલ RearrangementSubtable *table_) :
	ret (ખોટું),
	table (table_),
	start (0), end (0) {}

    HB_AAT_TRANSITION_INLINE
    શૂન્ય transition (hb_buffer_t *buffer,
		     StateTableDriver<Types, EntryData, Flags> *driver,
		     અચલ Entry<EntryData> &entry)
    {
      unsigned પૂર્ણાંક flags = entry.flags;

      જો (flags & MarkFirst)
	start = buffer->idx;

      જો (flags & MarkLast)
	end = hb_min (buffer->idx + 1, buffer->len);

      જો ((flags & Verb) && start < end)
      {
	/* The following map has two nibbles, for start-side
	 * and end-side. Values of 0,1,2 mean move that many
	 * to the other side. Value of 3 means move 2 and
	 * flip them. */
	અચલ unsigned અક્ષર map[16] =
	{
	  0x00,	/* 0	no change */
	  0x10,	/* 1	Ax => xA */
	  0x01,	/* 2	xD => Dx */
	  0x11,	/* 3	AxD => DxA */
	  0x20,	/* 4	ABx => xAB */
	  0x30,	/* 5	ABx => xBA */
	  0x02,	/* 6	xCD => CDx */
	  0x03,	/* 7	xCD => DCx */
	  0x12,	/* 8	AxCD => CDxA */
	  0x13,	/* 9	AxCD => DCxA */
	  0x21,	/* 10	ABxD => DxAB */
	  0x31,	/* 11	ABxD => DxBA */
	  0x22,	/* 12	ABxCD => CDxAB */
	  0x32,	/* 13	ABxCD => CDxBA */
	  0x23,	/* 14	ABxCD => DCxAB */
	  0x33,	/* 15	ABxCD => DCxBA */
	};

	unsigned પૂર્ણાંક m = map[flags & Verb];
	unsigned પૂર્ણાંક l = hb_min (2u, m >> 4);
	unsigned પૂર્ણાંક r = hb_min (2u, m & 0x0F);
	તાર્કિક reverse_l = 3 == (m >> 4);
	તાર્કિક reverse_r = 3 == (m & 0x0F);

	જો (end - start >= l + r && end-start <= HB_MAX_CONTEXT_LENGTH)
	{
	  buffer->merge_clusters (start, hb_min (buffer->idx + 1, buffer->len));
	  buffer->merge_clusters (start, end);

	  hb_glyph_info_t *info = buffer->info;
	  hb_glyph_info_t buf[4];

	  hb_memcpy (buf, info + start, l * કદ (buf[0]));
	  hb_memcpy (buf + 2, info + end - r, r * કદ (buf[0]));

	  જો (l != r)
	    memmove (info + start + r, info + start + l, (end - start - l - r) * કદ (buf[0]));

	  hb_memcpy (info + start, buf + 2, r * કદ (buf[0]));
	  hb_memcpy (info + end - l, buf, l * કદ (buf[0]));
	  જો (reverse_l)
	  {
	    buf[0] = info[end - 1];
	    info[end - 1] = info[end - 2];
	    info[end - 2] = buf[0];
	  }
	  જો (reverse_r)
	  {
	    buf[0] = info[start];
	    info[start] = info[start + 1];
	    info[start + 1] = buf[0];
	  }
	}
      }
    }

    જાહેર:
    તાર્કિક ret;
    અચલ RearrangementSubtable *table;
    ખાનગી:
    unsigned પૂર્ણાંક start;
    unsigned પૂર્ણાંક end;
  };

  તાર્કિક apply (hb_aat_apply_context_t *c) અચલ
  {
    TRACE_APPLY (આ);

    driver_context_t dc (આ);

    StateTableDriver<Types, EntryData, Flags> driver (machine, c->face);

    driver.drive (&dc, c);

    return_trace (dc.ret);
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (machine.sanitize (c));
  }

  જાહેર:
  StateTable<Types, EntryData>	machine;
  જાહેર:
  DEFINE_SIZE_STATIC ((StateTable<Types, EntryData>::static_size));
};

template <typename Types>
સંરચના ContextualSubtable
{
  પ્રકારવ્યાખ્યા typename Types::HBUINT HBUINT;

  સંરચના EntryData
  {
    પ્રકારવ્યાખ્યા typename std::conditional<Types::extended, HBUINT16, HBINT16>::type OffsetType;

    OffsetType	markIndex;	/* Index/offset of the substitution table for the
				 * marked glyph. */
    OffsetType	currentIndex;	/* Index/offset of the substitution table for the
				 * current glyph. */
    જાહેર:
    DEFINE_SIZE_STATIC (4);
  };

  ગણના Flags
  {
    SetMark		= 0x8000,	/* If set, make the current glyph the marked glyph. */
    DontAdvance		= 0x4000,	/* If set, don't advance to the next glyph before
					 * going to the new state. */
    Reserved		= 0x3FFF,	/* These bits are reserved and should be set to 0. */
  };

  તાર્કિક is_action_initiable (અચલ Entry<EntryData> &entry) અચલ
  {
    પાછા (entry.flags & SetMark);
  }
  તાર્કિક is_actionable (અચલ Entry<EntryData> &entry) અચલ
  {
    જો (Types::extended)
      પાછા entry.data.markIndex != 0xFFFF || entry.data.currentIndex != 0xFFFF;
    નહીંતર
      પાછા entry.data.markIndex || entry.data.currentIndex;
  }

  સંરચના driver_context_t
  {
    સ્થિર constexpr તાર્કિક in_place = સાચું;

    driver_context_t (અચલ ContextualSubtable *table_,
			     hb_aat_apply_context_t *c_) :
	ret (ખોટું),
	c (c_),
	table (table_),
	mark_set (ખોટું),
	mark (0),
	subs (table+table->substitutionTables) {}

    HB_AAT_TRANSITION_INLINE
    શૂન્ય transition (hb_buffer_t *buffer,
		     StateTableDriver<Types, EntryData, Flags> *driver,
		     અચલ Entry<EntryData> &entry)
    {
      /* Looks like CoreText applies neither mark nor current substitution for
       * end-of-text if mark was not explicitly set. */
      જો (buffer->idx == buffer->len && !mark_set)
	પાછા;

      અચલ HBGlyphID16 *replacement;

      replacement = nullptr;
      જો (Types::extended)
      {
	જો (entry.data.markIndex != 0xFFFF)
	{
	  અચલ Lookup<HBGlyphID16> &lookup = subs[entry.data.markIndex];
	  replacement = lookup.get_કિંમત (buffer->info[mark].codepoint, driver->અંક_glyphs);
	}
      }
      નહીંતર
      {
	જો (entry.data.markIndex)
	{
	  પૂર્ણાંક offset = (પૂર્ણાંક) entry.data.markIndex + buffer->info[mark].codepoint;
	  અચલ UnsizedArrayOf<HBGlyphID16> &subs_old = (અચલ UnsizedArrayOf<HBGlyphID16> &) subs;
	  replacement = &subs_old[Types::wordOffsetToIndex (offset, table, subs_old.arrayZ)];
	  જો (!(replacement->sanitize (&c->sanitizer) &&
		hb_barrier () &&
		*replacement))
	    replacement = nullptr;
	}
      }
      જો (replacement)
      {
	buffer->unsafe_to_break (mark, hb_min (buffer->idx + 1, buffer->len));
	c->replace_glyph_inplace (mark, *replacement);
	ret = સાચું;
      }

      replacement = nullptr;
      unsigned પૂર્ણાંક idx = hb_min (buffer->idx, buffer->len - 1);
      જો (Types::extended)
      {
	જો (entry.data.currentIndex != 0xFFFF)
	{
	  અચલ Lookup<HBGlyphID16> &lookup = subs[entry.data.currentIndex];
	  replacement = lookup.get_કિંમત (buffer->info[idx].codepoint, driver->અંક_glyphs);
	}
      }
      નહીંતર
      {
	જો (entry.data.currentIndex)
	{
	  પૂર્ણાંક offset = (પૂર્ણાંક) entry.data.currentIndex + buffer->info[idx].codepoint;
	  અચલ UnsizedArrayOf<HBGlyphID16> &subs_old = (અચલ UnsizedArrayOf<HBGlyphID16> &) subs;
	  replacement = &subs_old[Types::wordOffsetToIndex (offset, table, subs_old.arrayZ)];
	  જો (!(replacement->sanitize (&c->sanitizer) &&
		hb_barrier () &&
		*replacement))
	    replacement = nullptr;
	}
      }
      જો (replacement)
      {
	c->replace_glyph_inplace (idx, *replacement);
	ret = સાચું;
      }

      જો (entry.flags & SetMark)
      {
	mark_set = સાચું;
	mark = buffer->idx;
      }
    }

    જાહેર:
    તાર્કિક ret;
    hb_aat_apply_context_t *c;
    અચલ ContextualSubtable *table;
    ખાનગી:
    તાર્કિક mark_set;
    unsigned પૂર્ણાંક mark;
    અચલ UnsizedListOfOffset16To<Lookup<HBGlyphID16>, HBUINT, શૂન્ય, ખોટું> &subs;
  };

  તાર્કિક apply (hb_aat_apply_context_t *c) અચલ
  {
    TRACE_APPLY (આ);

    driver_context_t dc (આ, c);

    StateTableDriver<Types, EntryData, Flags> driver (machine, c->face);

    driver.drive (&dc, c);

    return_trace (dc.ret);
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);

    unsigned પૂર્ણાંક અંક_entries = 0;
    જો (unlikely (!machine.sanitize (c, &અંક_entries))) return_trace (ખોટું);
    hb_barrier ();

    જો (!Types::extended)
      return_trace (substitutionTables.sanitize (c, આ, 0));

    unsigned પૂર્ણાંક અંક_lookups = 0;

    અચલ Entry<EntryData> *entries = machine.get_entries ();
    માટે (unsigned પૂર્ણાંક i = 0; i < અંક_entries; i++)
    {
      અચલ EntryData &data = entries[i].data;

      જો (data.markIndex != 0xFFFF)
	અંક_lookups = hb_max (અંક_lookups, 1u + data.markIndex);
      જો (data.currentIndex != 0xFFFF)
	અંક_lookups = hb_max (અંક_lookups, 1u + data.currentIndex);
    }

    return_trace (substitutionTables.sanitize (c, આ, અંક_lookups));
  }

  જાહેર:
  StateTable<Types, EntryData>
		machine;
  સુરક્ષિત:
  NNOffsetTo<UnsizedListOfOffset16To<Lookup<HBGlyphID16>, HBUINT, શૂન્ય, ખોટું>, HBUINT>
		substitutionTables;
  જાહેર:
  DEFINE_SIZE_STATIC ((StateTable<Types, EntryData>::static_size + HBUINT::static_size));
};


template <તાર્કિક extended>
સંરચના LigatureEntry;

template <>
સંરચના LigatureEntry<સાચું>
{

  સંરચના EntryData
  {
    HBUINT16	ligActionIndex;	/* Index to the first ligActionTable entry
				 * for processing this group, if indicated
				 * by the flags. */
    જાહેર:
    DEFINE_SIZE_STATIC (2);
  };

  ગણના Flags
  {
    SetComponent	= 0x8000,	/* Push this glyph onto the component stack for
					 * eventual processing. */
    DontAdvance		= 0x4000,	/* Leave the glyph pointer at this glyph for the
					   next iteration. */
    PerformAction	= 0x2000,	/* Use the ligActionIndex to process a ligature
					 * group. */
    Reserved		= 0x1FFF,	/* These bits are reserved and should be set to 0. */
  };

  સ્થિર તાર્કિક initiateAction (અચલ Entry<EntryData> &entry)
  { પાછા entry.flags & SetComponent; }

  સ્થિર તાર્કિક performAction (અચલ Entry<EntryData> &entry)
  { પાછા entry.flags & PerformAction; }

  સ્થિર unsigned પૂર્ણાંક ligActionIndex (અચલ Entry<EntryData> &entry)
  { પાછા entry.data.ligActionIndex; }
};
template <>
સંરચના LigatureEntry<ખોટું>
{
  પ્રકારવ્યાખ્યા શૂન્ય EntryData;

  ગણના Flags
  {
    SetComponent	= 0x8000,	/* Push this glyph onto the component stack for
					 * eventual processing. */
    DontAdvance		= 0x4000,	/* Leave the glyph pointer at this glyph for the
					   next iteration. */
    Offset		= 0x3FFF,	/* Byte offset from beginning of subtable to the
					 * ligature action list. This value must be a
					 * multiple of 4. */
  };

  સ્થિર તાર્કિક initiateAction (અચલ Entry<EntryData> &entry)
  { પાછા entry.flags & SetComponent; }

  સ્થિર તાર્કિક performAction (અચલ Entry<EntryData> &entry)
  { પાછા entry.flags & Offset; }

  સ્થિર unsigned પૂર્ણાંક ligActionIndex (અચલ Entry<EntryData> &entry)
  { પાછા entry.flags & Offset; }
};


template <typename Types>
સંરચના LigatureSubtable
{
  પ્રકારવ્યાખ્યા typename Types::HBUINT HBUINT;

  પ્રકારવ્યાખ્યા LigatureEntry<Types::extended> LigatureEntryT;
  પ્રકારવ્યાખ્યા typename LigatureEntryT::EntryData EntryData;

  ગણના Flags
  {
    DontAdvance	= LigatureEntryT::DontAdvance,
  };

  તાર્કિક is_action_initiable (અચલ Entry<EntryData> &entry) અચલ
  {
    પાછા LigatureEntryT::initiateAction (entry);
  }
  તાર્કિક is_actionable (અચલ Entry<EntryData> &entry) અચલ
  {
    પાછા LigatureEntryT::performAction (entry);
  }

  સંરચના driver_context_t
  {
    સ્થિર constexpr તાર્કિક in_place = ખોટું;
    ગણના LigActionFlags
    {
      LigActionLast	= 0x80000000,	/* This is the last action in the list. This also
					 * implies storage. */
      LigActionStore	= 0x40000000,	/* Store the ligature at the current cumulated index
					 * in the ligature table in place of the marked
					 * (i.e. currently-popped) glyph. */
      LigActionOffset	= 0x3FFFFFFF,	/* A 30-bit value which is sign-extended to 32-bits
					 * and added to the glyph ID, resulting in an index
					 * into the component table. */
    };

    driver_context_t (અચલ LigatureSubtable *table_,
		      hb_aat_apply_context_t *c_) :
	ret (ખોટું),
	c (c_),
	table (table_),
	ligAction (table+table->ligAction),
	component (table+table->component),
	ligature (table+table->ligature),
	match_length (0) {}

    HB_AAT_TRANSITION_INLINE
    શૂન્ય transition (hb_buffer_t *buffer,
		     StateTableDriver<Types, EntryData, Flags> *driver,
		     અચલ Entry<EntryData> &entry)
    {
      DEBUG_સંદેશ (APPLY, nullptr, "Ligature transition at %u", buffer->idx);
      જો (entry.flags & LigatureEntryT::SetComponent)
      {
	/* Never mark same index twice, in case DontAdvance was used... */
	જો (unlikely (match_length && match_positions[(match_length - 1u) % ARRAY_LENGTH (match_positions)] == buffer->out_len))
	  match_length--;

	match_positions[match_length++ % ARRAY_LENGTH (match_positions)] = buffer->out_len;
	DEBUG_સંદેશ (APPLY, nullptr, "Set component at %u", buffer->out_len);
      }

      જો (LigatureEntryT::performAction (entry))
      {
	DEBUG_સંદેશ (APPLY, nullptr, "Perform action with %u", match_length);
	unsigned પૂર્ણાંક end = buffer->out_len;

	જો (unlikely (!match_length))
	  પાછા;

	જો (buffer->idx >= buffer->len)
	  પાછા; /* TODO Work on previous instead? */

	unsigned પૂર્ણાંક cursor = match_length;

	unsigned પૂર્ણાંક action_idx = LigatureEntryT::ligActionIndex (entry);
	action_idx = Types::offsetToIndex (action_idx, table, ligAction.arrayZ);
	અચલ HBUINT32 *actionData = &ligAction[action_idx];

	unsigned પૂર્ણાંક ligature_idx = 0;
	unsigned પૂર્ણાંક action;
	કરો
	{
	  જો (unlikely (!cursor))
	  {
	    /* Stack underflow.  Clear the stack. */
	    DEBUG_સંદેશ (APPLY, nullptr, "Stack underflow");
	    match_length = 0;
	    તોડો;
	  }

	  DEBUG_સંદેશ (APPLY, nullptr, "Moving to stack position %u", cursor - 1);
	  જો (unlikely (!buffer->move_to (match_positions[--cursor % ARRAY_LENGTH (match_positions)]))) પાછા;

	  જો (unlikely (!actionData->sanitize (&c->sanitizer))) તોડો;
	  hb_barrier ();
	  action = *actionData;

	  uint32_t uoffset = action & LigActionOffset;
	  જો (uoffset & 0x20000000)
	    uoffset |= 0xC0000000; /* Sign-extend. */
	  int32_t offset = (int32_t) uoffset;
	  unsigned પૂર્ણાંક component_idx = buffer->cur().codepoint + offset;
	  component_idx = Types::wordOffsetToIndex (component_idx, table, component.arrayZ);
	  અચલ HBUINT16 &componentData = component[component_idx];
	  જો (unlikely (!componentData.sanitize (&c->sanitizer))) તોડો;
	  hb_barrier ();
	  ligature_idx += componentData;

	  DEBUG_સંદેશ (APPLY, nullptr, "Action store %d last %d",
		     તાર્કિક (action & LigActionStore),
		     તાર્કિક (action & LigActionLast));
	  જો (action & (LigActionStore | LigActionLast))
	  {
	    ligature_idx = Types::offsetToIndex (ligature_idx, table, ligature.arrayZ);
	    અચલ HBGlyphID16 &ligatureData = ligature[ligature_idx];
	    જો (unlikely (!ligatureData.sanitize (&c->sanitizer))) તોડો;
	    hb_barrier ();
	    hb_codepoint_t lig = ligatureData;

	    DEBUG_સંદેશ (APPLY, nullptr, "Produced ligature %u", lig);
	    જો (unlikely (!c->replace_glyph (lig))) પાછા;

	    unsigned પૂર્ણાંક lig_end = match_positions[(match_length - 1u) % ARRAY_LENGTH (match_positions)] + 1u;
	    /* Now go and delete all subsequent components. */
	    જ્યારે (match_length - 1u > cursor)
	    {
	      DEBUG_સંદેશ (APPLY, nullptr, "Skipping ligature component");
	      જો (unlikely (!buffer->move_to (match_positions[--match_length % ARRAY_LENGTH (match_positions)]))) પાછા;
	      જો (!c->delete_glyph ()) પાછા;
	    }

	    જો (unlikely (!buffer->move_to (lig_end))) પાછા;
	    buffer->merge_out_clusters (match_positions[cursor % ARRAY_LENGTH (match_positions)], buffer->out_len);
	  }

	  actionData++;
	}
	જ્યારે (!(action & LigActionLast));
	જો (unlikely (!buffer->move_to (end))) પાછા;
      }
    }

    જાહેર:
    તાર્કિક ret;
    hb_aat_apply_context_t *c;
    અચલ LigatureSubtable *table;
    ખાનગી:
    અચલ UnsizedArrayOf<HBUINT32> &ligAction;
    અચલ UnsizedArrayOf<HBUINT16> &component;
    અચલ UnsizedArrayOf<HBGlyphID16> &ligature;
    unsigned પૂર્ણાંક match_length;
    unsigned પૂર્ણાંક match_positions[HB_MAX_CONTEXT_LENGTH];
  };

  તાર્કિક apply (hb_aat_apply_context_t *c) અચલ
  {
    TRACE_APPLY (આ);

    driver_context_t dc (આ, c);

    StateTableDriver<Types, EntryData, Flags> driver (machine, c->face);

    driver.drive (&dc, c);

    return_trace (dc.ret);
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    /* The rest of array sanitizations are done at run-time. */
    return_trace (c->check_struct (આ) && machine.sanitize (c) &&
		  hb_barrier () &&
		  ligAction && component && ligature);
  }

  જાહેર:
  StateTable<Types, EntryData>
		machine;
  સુરક્ષિત:
  NNOffsetTo<UnsizedArrayOf<HBUINT32>, HBUINT>
		ligAction;	/* Offset to the ligature action table. */
  NNOffsetTo<UnsizedArrayOf<HBUINT16>, HBUINT>
		component;	/* Offset to the component table. */
  NNOffsetTo<UnsizedArrayOf<HBGlyphID16>, HBUINT>
		ligature;	/* Offset to the actual ligature lists. */
  જાહેર:
  DEFINE_SIZE_STATIC ((StateTable<Types, EntryData>::static_size + 3 * HBUINT::static_size));
};

template <typename Types>
સંરચના NoncontextualSubtable
{
  તાર્કિક apply (hb_aat_apply_context_t *c) અચલ
  {
    TRACE_APPLY (આ);

    તાર્કિક ret = ખોટું;
    unsigned પૂર્ણાંક અંક_glyphs = c->face->get_અંક_glyphs ();

    hb_glyph_info_t *info = c->buffer->info;
    unsigned પૂર્ણાંક ગણતરી = c->buffer->len;
    // If there's only one range, we already checked the flag.
    auto *last_range = c->range_flags && (c->range_flags->length > 1) ? &(*c->range_flags)[0] : nullptr;
    માટે (unsigned પૂર્ણાંક i = 0; i < ગણતરી; i++)
    {
      /* This block copied from StateTableDriver::drive. Keep in sync. */
      જો (unlikely (last_range))
      {
	auto *range = last_range;
	{
	  unsigned cluster = info[i].cluster;
	  જ્યારે (cluster < range->cluster_first)
	    range--;
	  જ્યારે (cluster > range->cluster_last)
	    range++;

	  last_range = range;
	}
	જો (!(range->flags & c->subtable_flags))
	  ચાલુ;
      }

      અચલ HBGlyphID16 *replacement = substitute.get_કિંમત (info[i].codepoint, અંક_glyphs);
      જો (replacement)
      {
	c->replace_glyph_inplace (i, *replacement);
	ret = સાચું;
      }
    }

    return_trace (ret);
  }

  template <typename set_t>
  શૂન્ય collect_initial_glyphs (set_t &glyphs, unsigned અંક_glyphs) અચલ
  {
    substitute.collect_glyphs (glyphs, અંક_glyphs);
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (substitute.sanitize (c));
  }

  સુરક્ષિત:
  Lookup<HBGlyphID16>	substitute;
  જાહેર:
  DEFINE_SIZE_MIN (2);
};

template <typename Types>
સંરચના InsertionSubtable
{
  પ્રકારવ્યાખ્યા typename Types::HBUINT HBUINT;

  સંરચના EntryData
  {
    HBUINT16	current_ઉમેરો_Index;	/* Zero-based index into the insertion glyph table.
					 * The number of glyphs to be inserted is contained
					 * in the currentInsertCount field in the flags.
					 * A value of 0xFFFF indicates no insertion is to
					 * be done. */
    HBUINT16	marked_ઉમેરો_Index;	/* Zero-based index into the insertion glyph table.
					 * The number of glyphs to be inserted is contained
					 * in the markedInsertCount field in the flags.
					 * A value of 0xFFFF indicates no insertion is to
					 * be done. */
    જાહેર:
    DEFINE_SIZE_STATIC (4);
  };

  ગણના Flags
  {
    SetMark		= 0x8000,     /* If set, mark the current glyph. */
    DontAdvance		= 0x4000,     /* If set, don't advance to the next glyph before
				       * going to the new state.  This does not mean
				       * that the glyph pointed to is the same one as
				       * before. If you've made insertions immediately
				       * downstream of the current glyph, the next glyph
				       * processed would in fact be the first one
				       * inserted. */
    CurrentIsKashidaLike= 0x2000,     /* If set, and the currentInsertList is nonzero,
				       * then the specified glyph list will be inserted
				       * as a kashida-like insertion, either before or
				       * after the current glyph (depending on the state
				       * of the currentInsertBefore flag). If clear, and
				       * the currentInsertList is nonzero, then the
				       * specified glyph list will be inserted as a
				       * split-vowel-like insertion, either before or
				       * after the current glyph (depending on the state
				       * of the currentInsertBefore flag). */
    MarkedIsKashidaLike= 0x1000,      /* If set, and the markedInsertList is nonzero,
				       * then the specified glyph list will be inserted
				       * as a kashida-like insertion, either before or
				       * after the marked glyph (depending on the state
				       * of the markedInsertBefore flag). If clear, and
				       * the markedInsertList is nonzero, then the
				       * specified glyph list will be inserted as a
				       * split-vowel-like insertion, either before or
				       * after the marked glyph (depending on the state
				       * of the markedInsertBefore flag). */
    Current_ઉમેરો_Before= 0x0800,      /* If set, specifies that insertions are to be made
				       * to the left of the current glyph. If clear,
				       * they're made to the right of the current glyph. */
    Marked_ઉમેરો_Before= 0x0400,	      /* If set, specifies that insertions are to be
				       * made to the left of the marked glyph. If clear,
				       * they're made to the right of the marked glyph. */
    Current_ઉમેરો_ગણતરી= 0x3E0,	      /* This 5-bit field is treated as a count of the
				       * number of glyphs to insert at the current
				       * position. Since zero means no insertions, the
				       * largest number of insertions at any given
				       * current location is 31 glyphs. */
    Marked_ઉમેરો_ગણતરી= 0x001F,	      /* This 5-bit field is treated as a count of the
				       * number of glyphs to insert at the marked
				       * position. Since zero means no insertions, the
				       * largest number of insertions at any given
				       * marked location is 31 glyphs. */
  };

  તાર્કિક is_action_initiable (અચલ Entry<EntryData> &entry) અચલ
  {
    પાછા (entry.flags & SetMark);
  }
  તાર્કિક is_actionable (અચલ Entry<EntryData> &entry) અચલ
  {
    પાછા (entry.flags & (Current_ઉમેરો_ગણતરી | Marked_ઉમેરો_ગણતરી)) &&
	   (entry.data.current_ઉમેરો_Index != 0xFFFF ||entry.data.marked_ઉમેરો_Index != 0xFFFF);
  }

  સંરચના driver_context_t
  {
    સ્થિર constexpr તાર્કિક in_place = ખોટું;

    driver_context_t (અચલ InsertionSubtable *table_,
		      hb_aat_apply_context_t *c_) :
	ret (ખોટું),
	c (c_),
	table (table_),
	mark (0),
	insertionAction (table+table->insertionAction) {}

    HB_AAT_TRANSITION_INLINE
    શૂન્ય transition (hb_buffer_t *buffer,
		     StateTableDriver<Types, EntryData, Flags> *driver,
		     અચલ Entry<EntryData> &entry)
    {
      unsigned પૂર્ણાંક flags = entry.flags;

      unsigned mark_loc = buffer->out_len;

      જો (entry.data.marked_ઉમેરો_Index != 0xFFFF)
      {
	unsigned પૂર્ણાંક ગણતરી = (flags & Marked_ઉમેરો_ગણતરી);
	જો (unlikely ((buffer->max_ops -= ગણતરી) <= 0)) પાછા;
	unsigned પૂર્ણાંક start = entry.data.marked_ઉમેરો_Index;
	અચલ HBGlyphID16 *glyphs = &insertionAction[start];
	જો (unlikely (!c->sanitizer.check_array (glyphs, ગણતરી))) ગણતરી = 0;
	hb_barrier ();

	તાર્કિક before = flags & Marked_ઉમેરો_Before;

	unsigned પૂર્ણાંક end = buffer->out_len;
	જો (unlikely (!buffer->move_to (mark))) પાછા;

	જો (buffer->idx < buffer->len && !before)
	  જો (unlikely (!buffer->copy_glyph ())) પાછા;
	/* TODO We ignore KashidaLike setting. */
	જો (unlikely (!c->output_glyphs (ગણતરી, glyphs))) પાછા;
	ret = સાચું;
	જો (buffer->idx < buffer->len && !before)
	  buffer->skip_glyph ();

	જો (unlikely (!buffer->move_to (end + ગણતરી))) પાછા;

	buffer->unsafe_to_break_from_outbuffer (mark, hb_min (buffer->idx + 1, buffer->len));
      }

      જો (flags & SetMark)
	mark = mark_loc;

      જો (entry.data.current_ઉમેરો_Index != 0xFFFF)
      {
	unsigned પૂર્ણાંક ગણતરી = (flags & Current_ઉમેરો_ગણતરી) >> 5;
	જો (unlikely ((buffer->max_ops -= ગણતરી) <= 0)) પાછા;
	unsigned પૂર્ણાંક start = entry.data.current_ઉમેરો_Index;
	અચલ HBGlyphID16 *glyphs = &insertionAction[start];
	જો (unlikely (!c->sanitizer.check_array (glyphs, ગણતરી))) ગણતરી = 0;
	hb_barrier ();

	તાર્કિક before = flags & Current_ઉમેરો_Before;

	unsigned પૂર્ણાંક end = buffer->out_len;

	જો (buffer->idx < buffer->len && !before)
	  જો (unlikely (!buffer->copy_glyph ())) પાછા;
	/* TODO We ignore KashidaLike setting. */
	જો (unlikely (!c->output_glyphs (ગણતરી, glyphs))) પાછા;
	ret = સાચું;
	જો (buffer->idx < buffer->len && !before)
	  buffer->skip_glyph ();

	/* Humm. Not sure where to move to.  There's this wording under
	 * DontAdvance flag:
	 *
	 * "If set, don't update the glyph index before going to the new state.
	 * This does not mean that the glyph pointed to is the same one as
	 * before. If you've made insertions immediately downstream of the
	 * current glyph, the next glyph processed would in fact be the first
	 * one inserted."
	 *
	 * This suggests that if DontAdvance is NOT set, we should move to
	 * end+count.  If it *was*, then move to end, such that newly inserted
	 * glyphs are now visible.
	 *
	 * https://github.com/harfbuzz/harfbuzz/issues/1224#issuecomment-427691417
	 */
	જો (unlikely (!buffer->move_to ((flags & DontAdvance) ? end : end + ગણતરી))) પાછા;
      }
    }

    જાહેર:
    તાર્કિક ret;
    hb_aat_apply_context_t *c;
    અચલ InsertionSubtable *table;
    ખાનગી:
    unsigned પૂર્ણાંક mark;
    અચલ UnsizedArrayOf<HBGlyphID16> &insertionAction;
  };

  તાર્કિક apply (hb_aat_apply_context_t *c) અચલ
  {
    TRACE_APPLY (આ);

    driver_context_t dc (આ, c);

    StateTableDriver<Types, EntryData, Flags> driver (machine, c->face);

    driver.drive (&dc, c);

    return_trace (dc.ret);
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    /* The rest of array sanitizations are done at run-time. */
    return_trace (c->check_struct (આ) && machine.sanitize (c) &&
		  hb_barrier () &&
		  insertionAction);
  }

  જાહેર:
  StateTable<Types, EntryData>
		machine;
  સુરક્ષિત:
  NNOffsetTo<UnsizedArrayOf<HBGlyphID16>, HBUINT>
		insertionAction;	/* Byte offset from stateHeader to the start of
					 * the insertion glyph table. */
  જાહેર:
  DEFINE_SIZE_STATIC ((StateTable<Types, EntryData>::static_size + HBUINT::static_size));
};


સંરચના Feature
{
  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ));
  }

  જાહેર:
  HBUINT16	featureType;	/* The type of feature. */
  HBUINT16	featureSetting;	/* The feature's setting (aka selector). */
  HBUINT32	enableFlags;	/* Flags for the settings that this feature
				 * and setting enables. */
  HBUINT32	disableFlags;	/* Complement of flags for the settings that this
				 * feature and setting disable. */

  જાહેર:
  DEFINE_SIZE_STATIC (12);
};


સંરચના hb_accelerate_subtables_context_t :
       hb_dispatch_context_t<hb_accelerate_subtables_context_t>
{
  સંરચના hb_applicable_t
  {
    friend સંરચના hb_accelerate_subtables_context_t;
    friend સંરચના hb_aat_layout_lookup_accelerator_t;

    જાહેર:
    hb_bit_set_t glyph_set;
    mutable hb_aat_class_cache_t class_cache;

    template <typename T>
    auto init_ (અચલ T &obj_, unsigned અંક_glyphs, hb_priority<1>) HB_AUTO_RETURN
    (
      obj_.machine.collect_initial_glyphs (glyph_set, અંક_glyphs, obj_)
    )

    template <typename T>
    શૂન્ય init_ (અચલ T &obj_, unsigned અંક_glyphs, hb_priority<0>)
    {
      obj_.collect_initial_glyphs (glyph_set, અંક_glyphs);
    }

    template <typename T>
    શૂન્ય init (અચલ T &obj_, unsigned અંક_glyphs)
    {
      glyph_set.init ();
      init_ (obj_, અંક_glyphs, hb_prioritize);
      class_cache.clear ();
    }

    શૂન્ય
    fini ()
    {
      glyph_set.fini ();
    }
  };

  /* Dispatch interface. */
  template <typename T>
  return_t dispatch (અચલ T &obj)
  {
    hb_applicable_t *entry = &array[i++];

    entry->init (obj, અંક_glyphs);

    પાછા hb_empty_t ();
  }
  સ્થિર return_t default_return_કિંમત () { પાછા hb_empty_t (); }

  તાર્કિક stop_sublookup_iteration (return_t r) અચલ { પાછા ખોટું; }

  hb_accelerate_subtables_context_t (hb_applicable_t *array_, unsigned અંક_glyphs_2) :
				     hb_dispatch_context_t<hb_accelerate_subtables_context_t> (),
				     array (array_), અંક_glyphs (અંક_glyphs_2) {}

  hb_applicable_t *array;
  unsigned અંક_glyphs;
  unsigned i = 0;
};

સંરચના hb_aat_layout_chain_accelerator_t
{
  template <typename TChain>
  સ્થિર hb_aat_layout_chain_accelerator_t *create (અચલ TChain &chain, unsigned અંક_glyphs)
  {
    unsigned ગણતરી = chain.get_subtable_ગણતરી ();

    unsigned product;
    જો (unlikely (hb_unsigned_mul_overflows (ગણતરી,
        કદ (hb_accelerate_subtables_context_t::hb_applicable_t), &product)))
      પાછા nullptr;

    unsigned size;
    જો (unlikely (hb_unsigned_add_overflows (કદ (hb_aat_layout_chain_accelerator_t) -
        HB_VAR_ARRAY * કદ (hb_accelerate_subtables_context_t::hb_applicable_t), product, &size))) {
      પાછા nullptr;
    }

    /* The following is a calloc because when we are collecting subtables,
     * some of them might be invalid and hence not collect; as a result,
     * we might not fill in all the count entries of the subtables array.
     * Zeroing it allows the set digest to gatekeep it without having to
     * initialize it further. */
    auto *thiz = (hb_aat_layout_chain_accelerator_t *) hb_calloc (1, size);
    જો (unlikely (!thiz))
      પાછા nullptr;

    thiz->ગણતરી = ગણતરી;

    hb_accelerate_subtables_context_t c_accelerate_subtables (thiz->subtables, અંક_glyphs);
    chain.dispatch (&c_accelerate_subtables);

    પાછા thiz;
  }

  શૂન્ય destroy ()
  {
    માટે (unsigned i = 0; i < ગણતરી; i++)
      subtables[i].fini ();
  }

  unsigned ગણતરી;
  hb_accelerate_subtables_context_t::hb_applicable_t subtables[HB_VAR_ARRAY];
};

template <typename Types>
સંરચના ChainSubtable
{
  પ્રકારવ્યાખ્યા typename Types::HBUINT HBUINT;

  template <typename T>
  friend સંરચના Chain;

  size_t get_size () અચલ     { પાછા length; }
  unsigned પૂર્ણાંક get_type () અચલ     { પાછા coverage & 0xFF; }
  unsigned પૂર્ણાંક get_coverage () અચલ { પાછા coverage >> (કદ (HBUINT) * 8 - 8); }

  ગણના Coverage
  {
    Vertical		= 0x80,	/* If set, this subtable will only be applied
				 * to vertical text. If clear, this subtable
				 * will only be applied to horizontal text. */
    Backwards		= 0x40,	/* If set, this subtable will process glyphs
				 * in descending order. If clear, it will
				 * process the glyphs in ascending order. */
    AllDirections	= 0x20,	/* If set, this subtable will be applied to
				 * both horizontal and vertical text (i.e.
				 * the state of bit 0x80000000 is ignored). */
    Logical		= 0x10,	/* If set, this subtable will process glyphs
				 * in logical order (or reverse logical order,
				 * depending on the value of bit 0x80000000). */
  };
  ગણના Type
  {
    Rearrangement	= 0,
    Contextual		= 1,
    Ligature		= 2,
    Noncontextual	= 4,
    Insertion		= 5
  };

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) અચલ
  {
    unsigned પૂર્ણાંક subtable_type = get_type ();
    TRACE_DISPATCH (આ, subtable_type);
    પસંદગી (subtable_type) {
    સ્થિતિ Rearrangement:		hb_barrier (); return_trace (c->dispatch (u.rearrangement, std::forward<Ts> (ds)...));
    સ્થિતિ Contextual:		hb_barrier (); return_trace (c->dispatch (u.contextual, std::forward<Ts> (ds)...));
    સ્થિતિ Ligature:		hb_barrier (); return_trace (c->dispatch (u.ligature, std::forward<Ts> (ds)...));
    સ્થિતિ Noncontextual:		hb_barrier (); return_trace (c->dispatch (u.noncontextual, std::forward<Ts> (ds)...));
    સ્થિતિ Insertion:		hb_barrier (); return_trace (c->dispatch (u.insertion, std::forward<Ts> (ds)...));
    મૂળભૂત:			return_trace (c->default_return_કિંમત ());
    }
  }

  તાર્કિક apply (hb_aat_apply_context_t *c) અચલ
  {
    TRACE_APPLY (આ);
    // Disabled for https://github.com/harfbuzz/harfbuzz/issues/4873
    //hb_sanitize_with_object_t with (&c->sanitizer, this);
    return_trace (dispatch (c));
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    જો (!(length.sanitize (c) &&
	  hb_barrier () &&
	  length >= min_size &&
	  c->check_range (આ, length)))
      return_trace (ખોટું);

    // Disabled for https://github.com/harfbuzz/harfbuzz/issues/4873
    //hb_sanitize_with_object_t with (c, this);
    return_trace (dispatch (c));
  }

  સુરક્ષિત:
  HBUINT	length;		/* Total subtable length, including this header. */
  HBUINT	coverage;	/* Coverage flags and subtable type. */
  HBUINT32	subFeatureFlags;/* The 32-bit mask identifying which subtable this is. */
  સંઘ {
  RearrangementSubtable<Types>	rearrangement;
  ContextualSubtable<Types>	contextual;
  LigatureSubtable<Types>	ligature;
  NoncontextualSubtable<Types>	noncontextual;
  InsertionSubtable<Types>	insertion;
  } u;
  જાહેર:
  DEFINE_SIZE_MIN (2 * કદ (HBUINT) + 4);
};

template <typename Types>
સંરચના Chain
{
  પ્રકારવ્યાખ્યા typename Types::HBUINT HBUINT;

  unsigned get_subtable_ગણતરી () અચલ { પાછા subtable_ગણતરી_2; }

  hb_mask_t compile_flags (અચલ hb_aat_map_builder_t *map) અચલ
  {
    hb_mask_t flags = defaultFlags;
    {
      unsigned પૂર્ણાંક ગણતરી = feature_ગણતરી;
      માટે (unsigned i = 0; i < ગણતરી; i++)
      {
	અચલ Feature &feature = featureZ[i];
	hb_aat_layout_feature_type_t type = (hb_aat_layout_feature_type_t) (unsigned પૂર્ણાંક) feature.featureType;
	hb_aat_layout_feature_selector_t setting = (hb_aat_layout_feature_selector_t) (unsigned પૂર્ણાંક) feature.featureSetting;
      retry:
	// Check whether this type/setting pair was requested in the map, and if so, apply its flags.
	// (The search here only looks at the type and setting fields of feature_info_t.)
	hb_aat_map_builder_t::feature_info_t info = { type, setting, ખોટું, 0 };
	જો (map->current_features.bsearch (info))
	{
	  flags &= feature.disableFlags;
	  flags |= feature.enableFlags;
	}
	નહીંતર જો (type == HB_AAT_LAYOUT_FEATURE_TYPE_LETTER_CASE && setting == HB_AAT_LAYOUT_FEATURE_SELECTOR_SMALL_CAPS)
	{
	  /* Deprecated. https://github.com/harfbuzz/harfbuzz/issues/1342 */
	  type = HB_AAT_LAYOUT_FEATURE_TYPE_LOWER_CASE;
	  setting = HB_AAT_LAYOUT_FEATURE_SELECTOR_LOWER_CASE_SMALL_CAPS;
	  જાઓ retry;
	}
#ifndef HB_NO_AAT
	નહીંતર જો (type == HB_AAT_LAYOUT_FEATURE_TYPE_LANGUAGE_TAG_TYPE && setting &&
		 /* TODO: Rudimentary language matching. */
		 hb_language_matches (map->face->table.ltag->get_language (setting - 1), map->props.language))
	{
	  flags &= feature.disableFlags;
	  flags |= feature.enableFlags;
	}
#endif
      }
    }
    પાછા flags;
  }

  શૂન્ય apply (hb_aat_apply_context_t *c,
	      અચલ hb_aat_layout_chain_accelerator_t *accel) અચલ
  {
    અચલ ChainSubtable<Types> *subtable = &StructAfter<ChainSubtable<Types>> (featureZ.as_array (feature_ગણતરી));
    unsigned પૂર્ણાંક ગણતરી = subtable_ગણતરી_2;
    માટે (unsigned પૂર્ણાંક i = 0; i < ગણતરી; i++)
    {
      તાર્કિક reverse;

      auto coverage = subtable->get_coverage ();

      hb_mask_t subtable_flags = subtable->subFeatureFlags;
      જો (hb_none (hb_iter (c->range_flags) |
		   hb_map ([subtable_flags] (અચલ hb_aat_map_t::range_flags_t _) -> તાર્કિક { પાછા subtable_flags & (_.flags); })))
	જાઓ skip;

      જો (!(coverage & ChainSubtable<Types>::AllDirections) &&
	  HB_DIRECTION_IS_VERTICAL (c->buffer->props.direction) !=
	  તાર્કિક (coverage & ChainSubtable<Types>::Vertical))
	જાઓ skip;

      c->subtable_flags = subtable_flags;
      c->first_set = accel ? &accel->subtables[i].glyph_set : &Null(hb_bit_set_t);
      c->machine_class_cache = accel ? &accel->subtables[i].class_cache : nullptr;

      જો (!c->buffer_intersects_machine ())
      {
	(શૂન્ય) c->buffer->message (c->font, "skipped chainsubtable %u because no glyph matches", c->lookup_index);
	જાઓ skip;
      }

      /* Buffer contents is always in logical direction.  Determine if
       * we need to reverse before applying this subtable.  We reverse
       * back after if we did reverse indeed.
       *
       * Quoting the spac:
       * """
       * Bits 28 and 30 of the coverage field control the order in which
       * glyphs are processed when the subtable is run by the layout engine.
       * Bit 28 is used to indicate if the glyph processing direction is
       * the same as logical order or layout order. Bit 30 is used to
       * indicate whether glyphs are processed forwards or backwards within
       * that order.

		Bit 30	Bit 28	Interpretation for Horizontal Text
		0	0	The subtable is processed in layout order
				(the same order as the glyphs, which is
				always left-to-right).
		1	0	The subtable is processed in reverse layout order
				(the order opposite that of the glyphs, which is
				always right-to-left).
		0	1	The subtable is processed in logical order
				(the same order as the characters, which may be
				left-to-right or right-to-left).
		1	1	The subtable is processed in reverse logical order
				(the order opposite that of the characters, which
				may be right-to-left or left-to-right).
       */
      reverse = coverage & ChainSubtable<Types>::Logical ?
		તાર્કિક (coverage & ChainSubtable<Types>::Backwards) :
		તાર્કિક (coverage & ChainSubtable<Types>::Backwards) !=
		HB_DIRECTION_IS_BACKWARD (c->buffer->props.direction);

      જો (!c->buffer->message (c->font, "start chainsubtable %u", c->lookup_index))
	જાઓ skip;

      જો (reverse != c->buffer_is_reversed)
        c->reverse_buffer ();

      subtable->apply (c);

      (શૂન્ય) c->buffer->message (c->font, "end chainsubtable %u", c->lookup_index);

      જો (unlikely (!c->buffer->successful)) તોડો;

    skip:
      subtable = &StructAfter<ChainSubtable<Types>> (*subtable);
      c->set_lookup_index (c->lookup_index + 1);
    }
    જો (c->buffer_is_reversed)
      c->reverse_buffer ();
  }

  size_t get_size () અચલ { પાછા length; }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) અચલ
  {
    અચલ ChainSubtable<Types> *subtable = &StructAfter<ChainSubtable<Types>> (featureZ.as_array (feature_ગણતરી));
    unsigned પૂર્ણાંક ગણતરી = subtable_ગણતરી_2;
    માટે (unsigned પૂર્ણાંક i = 0; i < ગણતરી; i++)
    {
      typename context_t::return_t ret = subtable->dispatch (c, std::forward<Ts> (ds)...);
      જો (c->stop_sublookup_iteration (ret))
	પાછા ret;
      subtable = &StructAfter<ChainSubtable<Types>> (*subtable);
    }
    પાછા c->default_return_કિંમત ();
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c, unsigned પૂર્ણાંક version) અચલ
  {
    TRACE_SANITIZE (આ);
    જો (!(length.sanitize (c) &&
	  hb_barrier () &&
	  length >= min_size &&
	  c->check_range (આ, length)))
      return_trace (ખોટું);

    જો (!c->check_array (featureZ.arrayZ, feature_ગણતરી))
      return_trace (ખોટું);

    અચલ ChainSubtable<Types> *subtable = &StructAfter<ChainSubtable<Types>> (featureZ.as_array (feature_ગણતરી));
    unsigned પૂર્ણાંક ગણતરી = subtable_ગણતરી_2;
    માટે (unsigned પૂર્ણાંક i = 0; i < ગણતરી; i++)
    {
      જો (!subtable->sanitize (c))
	return_trace (ખોટું);
      hb_barrier ();
      subtable = &StructAfter<ChainSubtable<Types>> (*subtable);
    }

    જો (version >= 3)
    {
      અચલ SubtableGlyphCoverage *coverage = (અચલ SubtableGlyphCoverage *) subtable;
      જો (!coverage->sanitize (c, ગણતરી))
        return_trace (ખોટું);
    }

    return_trace (સાચું);
  }

  સુરક્ષિત:
  HBUINT32	defaultFlags;	/* The default specification for subtables. */
  HBUINT32	length;		/* Total byte count, including this header. */
  HBUINT	feature_ગણતરી;	/* Number of feature subtable entries. */
  HBUINT	subtable_ગણતરી_2;	/* The number of subtables in the chain. */

  UnsizedArrayOf<Feature>	featureZ;	/* Features. */
/*ChainSubtable	firstSubtable;*//* Subtables. */
/*SubtableGlyphCoverage coverages*//* Only if version >= 3. */

  જાહેર:
  DEFINE_SIZE_MIN (8 + 2 * કદ (HBUINT));
};


/*
 * The 'mort'/'morx' Table
 */

template <typename T, typename Types, hb_tag_t TAG>
સંરચના mortmorx
{
  સ્થિર constexpr hb_tag_t tableTag = TAG;

  તાર્કિક has_data () અચલ { પાછા version != 0; }

  સંરચના accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      hb_sanitize_context_t sc;
      આ->table = sc.reference_table<T> (face);

      જો (unlikely (આ->table->is_blocklisted (આ->table.get_blob (), face)))
      {
        hb_blob_destroy (આ->table.get_blob ());
        આ->table = hb_blob_get_empty ();
      }

      આ->chain_ગણતરી = table->get_chain_ગણતરી ();

      આ->accels = (hb_atomic_t<hb_aat_layout_chain_accelerator_t *> *) hb_calloc (આ->chain_ગણતરી, કદ (*accels));
      જો (unlikely (!આ->accels))
      {
	આ->chain_ગણતરી = 0;
	આ->table.destroy ();
	આ->table = hb_blob_get_empty ();
      }
    }
    ~accelerator_t ()
    {
      માટે (unsigned પૂર્ણાંક i = 0; i < આ->chain_ગણતરી; i++)
      {
	જો (આ->accels[i])
	  આ->accels[i]->destroy ();
	hb_free (આ->accels[i]);
      }
      hb_free (આ->accels);
      આ->table.destroy ();
    }

    hb_blob_t *get_blob () અચલ { પાછા table.get_blob (); }

    template <typename Chain>
    hb_aat_layout_chain_accelerator_t *get_accel (unsigned chain_index, અચલ Chain &chain, unsigned અંક_glyphs) અચલ
    {
      જો (unlikely (chain_index >= chain_ગણતરી)) પાછા nullptr;

    retry:
      auto *accel = accels[chain_index].get_acquire ();
      જો (unlikely (!accel))
      {
	accel = hb_aat_layout_chain_accelerator_t::create (chain, અંક_glyphs);
	જો (unlikely (!accel))
	  પાછા nullptr;

	જો (unlikely (!accels[chain_index].cmpexch (nullptr, accel)))
	{
	  hb_free (accel);
	  જાઓ retry;
	}
      }

      પાછા accel;
    }

    hb_blob_ptr_t<T> table;
    unsigned પૂર્ણાંક chain_ગણતરી;
    hb_atomic_t<hb_aat_layout_chain_accelerator_t *> *accels;
    hb_aat_scratch_t scratch;
  };


  શૂન્ય compile_flags (અચલ hb_aat_map_builder_t *mapper,
		      hb_aat_map_t *map) અચલ
  {
    અચલ Chain<Types> *chain = &firstChain;
    unsigned પૂર્ણાંક ગણતરી = chain_ગણતરી_2;
    જો (unlikely (!map->chain_flags.resize (ગણતરી)))
      પાછા;
    માટે (unsigned પૂર્ણાંક i = 0; i < ગણતરી; i++)
    {
      map->chain_flags[i].push (hb_aat_map_t::range_flags_t {chain->compile_flags (mapper),
							     mapper->range_first,
							     mapper->range_last});
      chain = &StructAfter<Chain<Types>> (*chain);
    }
  }

  unsigned get_chain_ગણતરી () અચલ
  {
    પાછા chain_ગણતરી_2;
  }
  શૂન્ય apply (hb_aat_apply_context_t *c,
	      અચલ hb_aat_map_t &map,
	      અચલ accelerator_t &accel) અચલ
  {
    જો (unlikely (!c->buffer->successful)) પાછા;

    c->buffer->unsafe_to_concat ();

    c->setup_buffer_glyph_set ();

    c->set_lookup_index (0);
    અચલ Chain<Types> *chain = &firstChain;
    unsigned પૂર્ણાંક ગણતરી = chain_ગણતરી_2;
    માટે (unsigned પૂર્ણાંક i = 0; i < ગણતરી; i++)
    {
      auto *chain_accel = accel.get_accel (i, *chain, c->face->get_અંક_glyphs ());
      c->range_flags = &map.chain_flags[i];
      chain->apply (c, chain_accel);
      જો (unlikely (!c->buffer->successful)) પાછા;
      chain = &StructAfter<Chain<Types>> (*chain);
    }
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    જો (!(version.sanitize (c) &&
	  hb_barrier () &&
	  version &&
	  chain_ગણતરી_2.sanitize (c)))
      return_trace (ખોટું);

    અચલ Chain<Types> *chain = &firstChain;
    unsigned પૂર્ણાંક ગણતરી = chain_ગણતરી_2;
    માટે (unsigned પૂર્ણાંક i = 0; i < ગણતરી; i++)
    {
      જો (!chain->sanitize (c, version))
	return_trace (ખોટું);
      hb_barrier ();
      chain = &StructAfter<Chain<Types>> (*chain);
    }

    return_trace (સાચું);
  }

  સુરક્ષિત:
  HBUINT16	version;	/* Version number of the glyph metamorphosis table.
				 * 1, 2, or 3. */
  HBUINT16	unused;		/* Set to 0. */
  HBUINT32	chain_ગણતરી_2;	/* Number of metamorphosis chains contained in this
				 * table. */
  Chain<Types>	firstChain;	/* Chains. */

  જાહેર:
  DEFINE_SIZE_MIN (8);
};

સંરચના morx : mortmorx<morx, ExtendedTypes, HB_AAT_TAG_morx>
{
  HB_INTERNAL તાર્કિક is_blocklisted (hb_blob_t *blob,
                                   hb_face_t *face) અચલ;
};

સંરચના mort : mortmorx<mort, ObsoleteTypes, HB_AAT_TAG_mort>
{
  HB_INTERNAL તાર્કિક is_blocklisted (hb_blob_t *blob,
                                   hb_face_t *face) અચલ;
};

સંરચના morx_accelerator_t : morx::accelerator_t {
  morx_accelerator_t (hb_face_t *face) : morx::accelerator_t (face) {}
};
સંરચના mort_accelerator_t : mort::accelerator_t {
  mort_accelerator_t (hb_face_t *face) : mort::accelerator_t (face) {}
};


} /* namespace AAT */


#endif /* HB_AAT_LAYOUT_MORX_TABLE_HH */
