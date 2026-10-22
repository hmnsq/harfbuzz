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

#അന്തര്ഭാവ "hb-open-type.hh"
#അന്തര്ഭാവ "hb-aat-layout-common.hh"
#അന്തര്ഭാവ "hb-ot-layout.hh"
#അന്തര്ഭാവ "hb-aat-map.hh"

/*
 * morx -- Extended Glyph Metamorphosis
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6morx.html
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6mort.html
 */
#define HB_AAT_TAG_morx HB_TAG('m','o','r','x')
#define HB_AAT_TAG_mort HB_TAG('m','o','r','t')


നാമാകാശ AAT {

using നാമാകാശ OT;

template <typename Types>
ഘടന RearrangementSubtable
{
  തരനിർവചനം typename Types::HBUINT HBUINT;

  തരനിർവചനം ശൂന്യം EntryData;

  എണ്ണൽ Flags
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

  തർക്കം is_action_initiable (സ്ഥിരം Entry<EntryData> &entry) സ്ഥിരം
  {
    തിരികെ (entry.flags & MarkFirst);
  }
  തർക്കം is_actionable (സ്ഥിരം Entry<EntryData> &entry) സ്ഥിരം
  {
    തിരികെ (entry.flags & Verb);
  }

  ഘടന driver_context_t
  {
    സ്ഥിതം constexpr തർക്കം in_place = സത്യം;

    driver_context_t (സ്ഥിരം RearrangementSubtable *table_) :
	ret (അസത്യം),
	table (table_),
	start (0), end (0) {}

    HB_AAT_TRANSITION_INLINE
    ശൂന്യം transition (hb_buffer_t *buffer,
		     StateTableDriver<Types, EntryData, Flags> *driver,
		     സ്ഥിരം Entry<EntryData> &entry)
    {
      unsigned പൂർണ്ണസംഖ്യ flags = entry.flags;

      എങ്കിൽ (flags & MarkFirst)
	start = buffer->idx;

      എങ്കിൽ (flags & MarkLast)
	end = hb_min (buffer->idx + 1, buffer->len);

      എങ്കിൽ ((flags & Verb) && start < end)
      {
	/* The following map has two nibbles, for start-side
	 * and end-side. Values of 0,1,2 mean move that many
	 * to the other side. Value of 3 means move 2 and
	 * flip them. */
	സ്ഥിരം unsigned അക്ഷരം map[16] =
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

	unsigned പൂർണ്ണസംഖ്യ m = map[flags & Verb];
	unsigned പൂർണ്ണസംഖ്യ l = hb_min (2u, m >> 4);
	unsigned പൂർണ്ണസംഖ്യ r = hb_min (2u, m & 0x0F);
	തർക്കം reverse_l = 3 == (m >> 4);
	തർക്കം reverse_r = 3 == (m & 0x0F);

	എങ്കിൽ (end - start >= l + r && end-start <= HB_MAX_CONTEXT_LENGTH)
	{
	  buffer->merge_clusters (start, hb_min (buffer->idx + 1, buffer->len));
	  buffer->merge_clusters (start, end);

	  hb_glyph_info_t *info = buffer->info;
	  hb_glyph_info_t buf[4];

	  hb_memcpy (buf, info + start, l * വലുപ്പം (buf[0]));
	  hb_memcpy (buf + 2, info + end - r, r * വലുപ്പം (buf[0]));

	  എങ്കിൽ (l != r)
	    memmove (info + start + r, info + start + l, (end - start - l - r) * വലുപ്പം (buf[0]));

	  hb_memcpy (info + start, buf + 2, r * വലുപ്പം (buf[0]));
	  hb_memcpy (info + end - l, buf, l * വലുപ്പം (buf[0]));
	  എങ്കിൽ (reverse_l)
	  {
	    buf[0] = info[end - 1];
	    info[end - 1] = info[end - 2];
	    info[end - 2] = buf[0];
	  }
	  എങ്കിൽ (reverse_r)
	  {
	    buf[0] = info[start];
	    info[start] = info[start + 1];
	    info[start + 1] = buf[0];
	  }
	}
      }
    }

    പൊതു:
    തർക്കം ret;
    സ്ഥിരം RearrangementSubtable *table;
    സ്വകാര്യ:
    unsigned പൂർണ്ണസംഖ്യ start;
    unsigned പൂർണ്ണസംഖ്യ end;
  };

  തർക്കം apply (hb_aat_apply_context_t *c) സ്ഥിരം
  {
    TRACE_APPLY (ഇത്);

    driver_context_t dc (ഇത്);

    StateTableDriver<Types, EntryData, Flags> driver (machine, c->face);

    driver.drive (&dc, c);

    return_trace (dc.ret);
  }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (machine.sanitize (c));
  }

  പൊതു:
  StateTable<Types, EntryData>	machine;
  പൊതു:
  DEFINE_SIZE_STATIC ((StateTable<Types, EntryData>::static_size));
};

template <typename Types>
ഘടന ContextualSubtable
{
  തരനിർവചനം typename Types::HBUINT HBUINT;

  ഘടന EntryData
  {
    തരനിർവചനം typename std::conditional<Types::extended, HBUINT16, HBINT16>::type OffsetType;

    OffsetType	markIndex;	/* Index/offset of the substitution table for the
				 * marked glyph. */
    OffsetType	currentIndex;	/* Index/offset of the substitution table for the
				 * current glyph. */
    പൊതു:
    DEFINE_SIZE_STATIC (4);
  };

  എണ്ണൽ Flags
  {
    SetMark		= 0x8000,	/* If set, make the current glyph the marked glyph. */
    DontAdvance		= 0x4000,	/* If set, don't advance to the next glyph before
					 * going to the new state. */
    Reserved		= 0x3FFF,	/* These bits are reserved and should be set to 0. */
  };

  തർക്കം is_action_initiable (സ്ഥിരം Entry<EntryData> &entry) സ്ഥിരം
  {
    തിരികെ (entry.flags & SetMark);
  }
  തർക്കം is_actionable (സ്ഥിരം Entry<EntryData> &entry) സ്ഥിരം
  {
    എങ്കിൽ (Types::extended)
      തിരികെ entry.data.markIndex != 0xFFFF || entry.data.currentIndex != 0xFFFF;
    അല്ലെങ്കിൽ
      തിരികെ entry.data.markIndex || entry.data.currentIndex;
  }

  ഘടന driver_context_t
  {
    സ്ഥിതം constexpr തർക്കം in_place = സത്യം;

    driver_context_t (സ്ഥിരം ContextualSubtable *table_,
			     hb_aat_apply_context_t *c_) :
	ret (അസത്യം),
	c (c_),
	table (table_),
	mark_set (അസത്യം),
	mark (0),
	subs (table+table->substitutionTables) {}

    HB_AAT_TRANSITION_INLINE
    ശൂന്യം transition (hb_buffer_t *buffer,
		     StateTableDriver<Types, EntryData, Flags> *driver,
		     സ്ഥിരം Entry<EntryData> &entry)
    {
      /* Looks like CoreText applies neither mark nor current substitution for
       * end-of-text if mark was not explicitly set. */
      എങ്കിൽ (buffer->idx == buffer->len && !mark_set)
	തിരികെ;

      സ്ഥിരം HBGlyphID16 *replacement;

      replacement = nullptr;
      എങ്കിൽ (Types::extended)
      {
	എങ്കിൽ (entry.data.markIndex != 0xFFFF)
	{
	  സ്ഥിരം Lookup<HBGlyphID16> &lookup = subs[entry.data.markIndex];
	  replacement = lookup.get_മൂല്യം (buffer->info[mark].codepoint, driver->അക്കം_glyphs);
	}
      }
      അല്ലെങ്കിൽ
      {
	എങ്കിൽ (entry.data.markIndex)
	{
	  പൂർണ്ണസംഖ്യ offset = (പൂർണ്ണസംഖ്യ) entry.data.markIndex + buffer->info[mark].codepoint;
	  സ്ഥിരം UnsizedArrayOf<HBGlyphID16> &subs_old = (സ്ഥിരം UnsizedArrayOf<HBGlyphID16> &) subs;
	  replacement = &subs_old[Types::wordOffsetToIndex (offset, table, subs_old.arrayZ)];
	  എങ്കിൽ (!(replacement->sanitize (&c->sanitizer) &&
		hb_barrier () &&
		*replacement))
	    replacement = nullptr;
	}
      }
      എങ്കിൽ (replacement)
      {
	buffer->unsafe_to_break (mark, hb_min (buffer->idx + 1, buffer->len));
	c->replace_glyph_inplace (mark, *replacement);
	ret = സത്യം;
      }

      replacement = nullptr;
      unsigned പൂർണ്ണസംഖ്യ idx = hb_min (buffer->idx, buffer->len - 1);
      എങ്കിൽ (Types::extended)
      {
	എങ്കിൽ (entry.data.currentIndex != 0xFFFF)
	{
	  സ്ഥിരം Lookup<HBGlyphID16> &lookup = subs[entry.data.currentIndex];
	  replacement = lookup.get_മൂല്യം (buffer->info[idx].codepoint, driver->അക്കം_glyphs);
	}
      }
      അല്ലെങ്കിൽ
      {
	എങ്കിൽ (entry.data.currentIndex)
	{
	  പൂർണ്ണസംഖ്യ offset = (പൂർണ്ണസംഖ്യ) entry.data.currentIndex + buffer->info[idx].codepoint;
	  സ്ഥിരം UnsizedArrayOf<HBGlyphID16> &subs_old = (സ്ഥിരം UnsizedArrayOf<HBGlyphID16> &) subs;
	  replacement = &subs_old[Types::wordOffsetToIndex (offset, table, subs_old.arrayZ)];
	  എങ്കിൽ (!(replacement->sanitize (&c->sanitizer) &&
		hb_barrier () &&
		*replacement))
	    replacement = nullptr;
	}
      }
      എങ്കിൽ (replacement)
      {
	c->replace_glyph_inplace (idx, *replacement);
	ret = സത്യം;
      }

      എങ്കിൽ (entry.flags & SetMark)
      {
	mark_set = സത്യം;
	mark = buffer->idx;
      }
    }

    പൊതു:
    തർക്കം ret;
    hb_aat_apply_context_t *c;
    സ്ഥിരം ContextualSubtable *table;
    സ്വകാര്യ:
    തർക്കം mark_set;
    unsigned പൂർണ്ണസംഖ്യ mark;
    സ്ഥിരം UnsizedListOfOffset16To<Lookup<HBGlyphID16>, HBUINT, ശൂന്യം, അസത്യം> &subs;
  };

  തർക്കം apply (hb_aat_apply_context_t *c) സ്ഥിരം
  {
    TRACE_APPLY (ഇത്);

    driver_context_t dc (ഇത്, c);

    StateTableDriver<Types, EntryData, Flags> driver (machine, c->face);

    driver.drive (&dc, c);

    return_trace (dc.ret);
  }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);

    unsigned പൂർണ്ണസംഖ്യ അക്കം_entries = 0;
    എങ്കിൽ (unlikely (!machine.sanitize (c, &അക്കം_entries))) return_trace (അസത്യം);
    hb_barrier ();

    എങ്കിൽ (!Types::extended)
      return_trace (substitutionTables.sanitize (c, ഇത്, 0));

    unsigned പൂർണ്ണസംഖ്യ അക്കം_lookups = 0;

    സ്ഥിരം Entry<EntryData> *entries = machine.get_entries ();
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < അക്കം_entries; i++)
    {
      സ്ഥിരം EntryData &data = entries[i].data;

      എങ്കിൽ (data.markIndex != 0xFFFF)
	അക്കം_lookups = hb_max (അക്കം_lookups, 1u + data.markIndex);
      എങ്കിൽ (data.currentIndex != 0xFFFF)
	അക്കം_lookups = hb_max (അക്കം_lookups, 1u + data.currentIndex);
    }

    return_trace (substitutionTables.sanitize (c, ഇത്, അക്കം_lookups));
  }

  പൊതു:
  StateTable<Types, EntryData>
		machine;
  സംരക്ഷിത:
  NNOffsetTo<UnsizedListOfOffset16To<Lookup<HBGlyphID16>, HBUINT, ശൂന്യം, അസത്യം>, HBUINT>
		substitutionTables;
  പൊതു:
  DEFINE_SIZE_STATIC ((StateTable<Types, EntryData>::static_size + HBUINT::static_size));
};


template <തർക്കം extended>
ഘടന LigatureEntry;

template <>
ഘടന LigatureEntry<സത്യം>
{

  ഘടന EntryData
  {
    HBUINT16	ligActionIndex;	/* Index to the first ligActionTable entry
				 * for processing this group, if indicated
				 * by the flags. */
    പൊതു:
    DEFINE_SIZE_STATIC (2);
  };

  എണ്ണൽ Flags
  {
    SetComponent	= 0x8000,	/* Push this glyph onto the component stack for
					 * eventual processing. */
    DontAdvance		= 0x4000,	/* Leave the glyph pointer at this glyph for the
					   next iteration. */
    PerformAction	= 0x2000,	/* Use the ligActionIndex to process a ligature
					 * group. */
    Reserved		= 0x1FFF,	/* These bits are reserved and should be set to 0. */
  };

  സ്ഥിതം തർക്കം initiateAction (സ്ഥിരം Entry<EntryData> &entry)
  { തിരികെ entry.flags & SetComponent; }

  സ്ഥിതം തർക്കം performAction (സ്ഥിരം Entry<EntryData> &entry)
  { തിരികെ entry.flags & PerformAction; }

  സ്ഥിതം unsigned പൂർണ്ണസംഖ്യ ligActionIndex (സ്ഥിരം Entry<EntryData> &entry)
  { തിരികെ entry.data.ligActionIndex; }
};
template <>
ഘടന LigatureEntry<അസത്യം>
{
  തരനിർവചനം ശൂന്യം EntryData;

  എണ്ണൽ Flags
  {
    SetComponent	= 0x8000,	/* Push this glyph onto the component stack for
					 * eventual processing. */
    DontAdvance		= 0x4000,	/* Leave the glyph pointer at this glyph for the
					   next iteration. */
    Offset		= 0x3FFF,	/* Byte offset from beginning of subtable to the
					 * ligature action list. This value must be a
					 * multiple of 4. */
  };

  സ്ഥിതം തർക്കം initiateAction (സ്ഥിരം Entry<EntryData> &entry)
  { തിരികെ entry.flags & SetComponent; }

  സ്ഥിതം തർക്കം performAction (സ്ഥിരം Entry<EntryData> &entry)
  { തിരികെ entry.flags & Offset; }

  സ്ഥിതം unsigned പൂർണ്ണസംഖ്യ ligActionIndex (സ്ഥിരം Entry<EntryData> &entry)
  { തിരികെ entry.flags & Offset; }
};


template <typename Types>
ഘടന LigatureSubtable
{
  തരനിർവചനം typename Types::HBUINT HBUINT;

  തരനിർവചനം LigatureEntry<Types::extended> LigatureEntryT;
  തരനിർവചനം typename LigatureEntryT::EntryData EntryData;

  എണ്ണൽ Flags
  {
    DontAdvance	= LigatureEntryT::DontAdvance,
  };

  തർക്കം is_action_initiable (സ്ഥിരം Entry<EntryData> &entry) സ്ഥിരം
  {
    തിരികെ LigatureEntryT::initiateAction (entry);
  }
  തർക്കം is_actionable (സ്ഥിരം Entry<EntryData> &entry) സ്ഥിരം
  {
    തിരികെ LigatureEntryT::performAction (entry);
  }

  ഘടന driver_context_t
  {
    സ്ഥിതം constexpr തർക്കം in_place = അസത്യം;
    എണ്ണൽ LigActionFlags
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

    driver_context_t (സ്ഥിരം LigatureSubtable *table_,
		      hb_aat_apply_context_t *c_) :
	ret (അസത്യം),
	c (c_),
	table (table_),
	ligAction (table+table->ligAction),
	component (table+table->component),
	ligature (table+table->ligature),
	match_length (0) {}

    HB_AAT_TRANSITION_INLINE
    ശൂന്യം transition (hb_buffer_t *buffer,
		     StateTableDriver<Types, EntryData, Flags> *driver,
		     സ്ഥിരം Entry<EntryData> &entry)
    {
      DEBUG_സന്ദേശം (APPLY, nullptr, "Ligature transition at %u", buffer->idx);
      എങ്കിൽ (entry.flags & LigatureEntryT::SetComponent)
      {
	/* Never mark same index twice, in case DontAdvance was used... */
	എങ്കിൽ (unlikely (match_length && match_positions[(match_length - 1u) % ARRAY_LENGTH (match_positions)] == buffer->out_len))
	  match_length--;

	match_positions[match_length++ % ARRAY_LENGTH (match_positions)] = buffer->out_len;
	DEBUG_സന്ദേശം (APPLY, nullptr, "Set component at %u", buffer->out_len);
      }

      എങ്കിൽ (LigatureEntryT::performAction (entry))
      {
	DEBUG_സന്ദേശം (APPLY, nullptr, "Perform action with %u", match_length);
	unsigned പൂർണ്ണസംഖ്യ end = buffer->out_len;

	എങ്കിൽ (unlikely (!match_length))
	  തിരികെ;

	എങ്കിൽ (buffer->idx >= buffer->len)
	  തിരികെ; /* TODO Work on previous instead? */

	unsigned പൂർണ്ണസംഖ്യ cursor = match_length;

	unsigned പൂർണ്ണസംഖ്യ action_idx = LigatureEntryT::ligActionIndex (entry);
	action_idx = Types::offsetToIndex (action_idx, table, ligAction.arrayZ);
	സ്ഥിരം HBUINT32 *actionData = &ligAction[action_idx];

	unsigned പൂർണ്ണസംഖ്യ ligature_idx = 0;
	unsigned പൂർണ്ണസംഖ്യ action;
	ചെയ്യുക
	{
	  എങ്കിൽ (unlikely (!cursor))
	  {
	    /* Stack underflow.  Clear the stack. */
	    DEBUG_സന്ദേശം (APPLY, nullptr, "Stack underflow");
	    match_length = 0;
	    നിർത്തുക;
	  }

	  DEBUG_സന്ദേശം (APPLY, nullptr, "Moving to stack position %u", cursor - 1);
	  എങ്കിൽ (unlikely (!buffer->move_to (match_positions[--cursor % ARRAY_LENGTH (match_positions)]))) തിരികെ;

	  എങ്കിൽ (unlikely (!actionData->sanitize (&c->sanitizer))) നിർത്തുക;
	  hb_barrier ();
	  action = *actionData;

	  uint32_t uoffset = action & LigActionOffset;
	  എങ്കിൽ (uoffset & 0x20000000)
	    uoffset |= 0xC0000000; /* Sign-extend. */
	  int32_t offset = (int32_t) uoffset;
	  unsigned പൂർണ്ണസംഖ്യ component_idx = buffer->cur().codepoint + offset;
	  component_idx = Types::wordOffsetToIndex (component_idx, table, component.arrayZ);
	  സ്ഥിരം HBUINT16 &componentData = component[component_idx];
	  എങ്കിൽ (unlikely (!componentData.sanitize (&c->sanitizer))) നിർത്തുക;
	  hb_barrier ();
	  ligature_idx += componentData;

	  DEBUG_സന്ദേശം (APPLY, nullptr, "Action store %d last %d",
		     തർക്കം (action & LigActionStore),
		     തർക്കം (action & LigActionLast));
	  എങ്കിൽ (action & (LigActionStore | LigActionLast))
	  {
	    ligature_idx = Types::offsetToIndex (ligature_idx, table, ligature.arrayZ);
	    സ്ഥിരം HBGlyphID16 &ligatureData = ligature[ligature_idx];
	    എങ്കിൽ (unlikely (!ligatureData.sanitize (&c->sanitizer))) നിർത്തുക;
	    hb_barrier ();
	    hb_codepoint_t lig = ligatureData;

	    DEBUG_സന്ദേശം (APPLY, nullptr, "Produced ligature %u", lig);
	    എങ്കിൽ (unlikely (!c->replace_glyph (lig))) തിരികെ;

	    unsigned പൂർണ്ണസംഖ്യ lig_end = match_positions[(match_length - 1u) % ARRAY_LENGTH (match_positions)] + 1u;
	    /* Now go and delete all subsequent components. */
	    ആകുമ്പോൾ (match_length - 1u > cursor)
	    {
	      DEBUG_സന്ദേശം (APPLY, nullptr, "Skipping ligature component");
	      എങ്കിൽ (unlikely (!buffer->move_to (match_positions[--match_length % ARRAY_LENGTH (match_positions)]))) തിരികെ;
	      എങ്കിൽ (!c->delete_glyph ()) തിരികെ;
	    }

	    എങ്കിൽ (unlikely (!buffer->move_to (lig_end))) തിരികെ;
	    buffer->merge_out_clusters (match_positions[cursor % ARRAY_LENGTH (match_positions)], buffer->out_len);
	  }

	  actionData++;
	}
	ആകുമ്പോൾ (!(action & LigActionLast));
	എങ്കിൽ (unlikely (!buffer->move_to (end))) തിരികെ;
      }
    }

    പൊതു:
    തർക്കം ret;
    hb_aat_apply_context_t *c;
    സ്ഥിരം LigatureSubtable *table;
    സ്വകാര്യ:
    സ്ഥിരം UnsizedArrayOf<HBUINT32> &ligAction;
    സ്ഥിരം UnsizedArrayOf<HBUINT16> &component;
    സ്ഥിരം UnsizedArrayOf<HBGlyphID16> &ligature;
    unsigned പൂർണ്ണസംഖ്യ match_length;
    unsigned പൂർണ്ണസംഖ്യ match_positions[HB_MAX_CONTEXT_LENGTH];
  };

  തർക്കം apply (hb_aat_apply_context_t *c) സ്ഥിരം
  {
    TRACE_APPLY (ഇത്);

    driver_context_t dc (ഇത്, c);

    StateTableDriver<Types, EntryData, Flags> driver (machine, c->face);

    driver.drive (&dc, c);

    return_trace (dc.ret);
  }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    /* The rest of array sanitizations are done at run-time. */
    return_trace (c->check_struct (ഇത്) && machine.sanitize (c) &&
		  hb_barrier () &&
		  ligAction && component && ligature);
  }

  പൊതു:
  StateTable<Types, EntryData>
		machine;
  സംരക്ഷിത:
  NNOffsetTo<UnsizedArrayOf<HBUINT32>, HBUINT>
		ligAction;	/* Offset to the ligature action table. */
  NNOffsetTo<UnsizedArrayOf<HBUINT16>, HBUINT>
		component;	/* Offset to the component table. */
  NNOffsetTo<UnsizedArrayOf<HBGlyphID16>, HBUINT>
		ligature;	/* Offset to the actual ligature lists. */
  പൊതു:
  DEFINE_SIZE_STATIC ((StateTable<Types, EntryData>::static_size + 3 * HBUINT::static_size));
};

template <typename Types>
ഘടന NoncontextualSubtable
{
  തർക്കം apply (hb_aat_apply_context_t *c) സ്ഥിരം
  {
    TRACE_APPLY (ഇത്);

    തർക്കം ret = അസത്യം;
    unsigned പൂർണ്ണസംഖ്യ അക്കം_glyphs = c->face->get_അക്കം_glyphs ();

    hb_glyph_info_t *info = c->buffer->info;
    unsigned പൂർണ്ണസംഖ്യ എണ്ണം = c->buffer->len;
    // If there's only one range, we already checked the flag.
    auto *last_range = c->range_flags && (c->range_flags->length > 1) ? &(*c->range_flags)[0] : nullptr;
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < എണ്ണം; i++)
    {
      /* This block copied from StateTableDriver::drive. Keep in sync. */
      എങ്കിൽ (unlikely (last_range))
      {
	auto *range = last_range;
	{
	  unsigned cluster = info[i].cluster;
	  ആകുമ്പോൾ (cluster < range->cluster_first)
	    range--;
	  ആകുമ്പോൾ (cluster > range->cluster_last)
	    range++;

	  last_range = range;
	}
	എങ്കിൽ (!(range->flags & c->subtable_flags))
	  തുടരുക;
      }

      സ്ഥിരം HBGlyphID16 *replacement = substitute.get_മൂല്യം (info[i].codepoint, അക്കം_glyphs);
      എങ്കിൽ (replacement)
      {
	c->replace_glyph_inplace (i, *replacement);
	ret = സത്യം;
      }
    }

    return_trace (ret);
  }

  template <typename set_t>
  ശൂന്യം collect_initial_glyphs (set_t &glyphs, unsigned അക്കം_glyphs) സ്ഥിരം
  {
    substitute.collect_glyphs (glyphs, അക്കം_glyphs);
  }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (substitute.sanitize (c));
  }

  സംരക്ഷിത:
  Lookup<HBGlyphID16>	substitute;
  പൊതു:
  DEFINE_SIZE_MIN (2);
};

template <typename Types>
ഘടന InsertionSubtable
{
  തരനിർവചനം typename Types::HBUINT HBUINT;

  ഘടന EntryData
  {
    HBUINT16	current_തിരുകുക_Index;	/* Zero-based index into the insertion glyph table.
					 * The number of glyphs to be inserted is contained
					 * in the currentInsertCount field in the flags.
					 * A value of 0xFFFF indicates no insertion is to
					 * be done. */
    HBUINT16	marked_തിരുകുക_Index;	/* Zero-based index into the insertion glyph table.
					 * The number of glyphs to be inserted is contained
					 * in the markedInsertCount field in the flags.
					 * A value of 0xFFFF indicates no insertion is to
					 * be done. */
    പൊതു:
    DEFINE_SIZE_STATIC (4);
  };

  എണ്ണൽ Flags
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
    Current_തിരുകുക_Before= 0x0800,      /* If set, specifies that insertions are to be made
				       * to the left of the current glyph. If clear,
				       * they're made to the right of the current glyph. */
    Marked_തിരുകുക_Before= 0x0400,	      /* If set, specifies that insertions are to be
				       * made to the left of the marked glyph. If clear,
				       * they're made to the right of the marked glyph. */
    Current_തിരുകുക_എണ്ണം= 0x3E0,	      /* This 5-bit field is treated as a count of the
				       * number of glyphs to insert at the current
				       * position. Since zero means no insertions, the
				       * largest number of insertions at any given
				       * current location is 31 glyphs. */
    Marked_തിരുകുക_എണ്ണം= 0x001F,	      /* This 5-bit field is treated as a count of the
				       * number of glyphs to insert at the marked
				       * position. Since zero means no insertions, the
				       * largest number of insertions at any given
				       * marked location is 31 glyphs. */
  };

  തർക്കം is_action_initiable (സ്ഥിരം Entry<EntryData> &entry) സ്ഥിരം
  {
    തിരികെ (entry.flags & SetMark);
  }
  തർക്കം is_actionable (സ്ഥിരം Entry<EntryData> &entry) സ്ഥിരം
  {
    തിരികെ (entry.flags & (Current_തിരുകുക_എണ്ണം | Marked_തിരുകുക_എണ്ണം)) &&
	   (entry.data.current_തിരുകുക_Index != 0xFFFF ||entry.data.marked_തിരുകുക_Index != 0xFFFF);
  }

  ഘടന driver_context_t
  {
    സ്ഥിതം constexpr തർക്കം in_place = അസത്യം;

    driver_context_t (സ്ഥിരം InsertionSubtable *table_,
		      hb_aat_apply_context_t *c_) :
	ret (അസത്യം),
	c (c_),
	table (table_),
	mark (0),
	insertionAction (table+table->insertionAction) {}

    HB_AAT_TRANSITION_INLINE
    ശൂന്യം transition (hb_buffer_t *buffer,
		     StateTableDriver<Types, EntryData, Flags> *driver,
		     സ്ഥിരം Entry<EntryData> &entry)
    {
      unsigned പൂർണ്ണസംഖ്യ flags = entry.flags;

      unsigned mark_loc = buffer->out_len;

      എങ്കിൽ (entry.data.marked_തിരുകുക_Index != 0xFFFF)
      {
	unsigned പൂർണ്ണസംഖ്യ എണ്ണം = (flags & Marked_തിരുകുക_എണ്ണം);
	എങ്കിൽ (unlikely ((buffer->max_ops -= എണ്ണം) <= 0)) തിരികെ;
	unsigned പൂർണ്ണസംഖ്യ start = entry.data.marked_തിരുകുക_Index;
	സ്ഥിരം HBGlyphID16 *glyphs = &insertionAction[start];
	എങ്കിൽ (unlikely (!c->sanitizer.check_array (glyphs, എണ്ണം))) എണ്ണം = 0;
	hb_barrier ();

	തർക്കം before = flags & Marked_തിരുകുക_Before;

	unsigned പൂർണ്ണസംഖ്യ end = buffer->out_len;
	എങ്കിൽ (unlikely (!buffer->move_to (mark))) തിരികെ;

	എങ്കിൽ (buffer->idx < buffer->len && !before)
	  എങ്കിൽ (unlikely (!buffer->copy_glyph ())) തിരികെ;
	/* TODO We ignore KashidaLike setting. */
	എങ്കിൽ (unlikely (!c->output_glyphs (എണ്ണം, glyphs))) തിരികെ;
	ret = സത്യം;
	എങ്കിൽ (buffer->idx < buffer->len && !before)
	  buffer->skip_glyph ();

	എങ്കിൽ (unlikely (!buffer->move_to (end + എണ്ണം))) തിരികെ;

	buffer->unsafe_to_break_from_outbuffer (mark, hb_min (buffer->idx + 1, buffer->len));
      }

      എങ്കിൽ (flags & SetMark)
	mark = mark_loc;

      എങ്കിൽ (entry.data.current_തിരുകുക_Index != 0xFFFF)
      {
	unsigned പൂർണ്ണസംഖ്യ എണ്ണം = (flags & Current_തിരുകുക_എണ്ണം) >> 5;
	എങ്കിൽ (unlikely ((buffer->max_ops -= എണ്ണം) <= 0)) തിരികെ;
	unsigned പൂർണ്ണസംഖ്യ start = entry.data.current_തിരുകുക_Index;
	സ്ഥിരം HBGlyphID16 *glyphs = &insertionAction[start];
	എങ്കിൽ (unlikely (!c->sanitizer.check_array (glyphs, എണ്ണം))) എണ്ണം = 0;
	hb_barrier ();

	തർക്കം before = flags & Current_തിരുകുക_Before;

	unsigned പൂർണ്ണസംഖ്യ end = buffer->out_len;

	എങ്കിൽ (buffer->idx < buffer->len && !before)
	  എങ്കിൽ (unlikely (!buffer->copy_glyph ())) തിരികെ;
	/* TODO We ignore KashidaLike setting. */
	എങ്കിൽ (unlikely (!c->output_glyphs (എണ്ണം, glyphs))) തിരികെ;
	ret = സത്യം;
	എങ്കിൽ (buffer->idx < buffer->len && !before)
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
	എങ്കിൽ (unlikely (!buffer->move_to ((flags & DontAdvance) ? end : end + എണ്ണം))) തിരികെ;
      }
    }

    പൊതു:
    തർക്കം ret;
    hb_aat_apply_context_t *c;
    സ്ഥിരം InsertionSubtable *table;
    സ്വകാര്യ:
    unsigned പൂർണ്ണസംഖ്യ mark;
    സ്ഥിരം UnsizedArrayOf<HBGlyphID16> &insertionAction;
  };

  തർക്കം apply (hb_aat_apply_context_t *c) സ്ഥിരം
  {
    TRACE_APPLY (ഇത്);

    driver_context_t dc (ഇത്, c);

    StateTableDriver<Types, EntryData, Flags> driver (machine, c->face);

    driver.drive (&dc, c);

    return_trace (dc.ret);
  }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    /* The rest of array sanitizations are done at run-time. */
    return_trace (c->check_struct (ഇത്) && machine.sanitize (c) &&
		  hb_barrier () &&
		  insertionAction);
  }

  പൊതു:
  StateTable<Types, EntryData>
		machine;
  സംരക്ഷിത:
  NNOffsetTo<UnsizedArrayOf<HBGlyphID16>, HBUINT>
		insertionAction;	/* Byte offset from stateHeader to the start of
					 * the insertion glyph table. */
  പൊതു:
  DEFINE_SIZE_STATIC ((StateTable<Types, EntryData>::static_size + HBUINT::static_size));
};


ഘടന Feature
{
  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_struct (ഇത്));
  }

  പൊതു:
  HBUINT16	featureType;	/* The type of feature. */
  HBUINT16	featureSetting;	/* The feature's setting (aka selector). */
  HBUINT32	enableFlags;	/* Flags for the settings that this feature
				 * and setting enables. */
  HBUINT32	disableFlags;	/* Complement of flags for the settings that this
				 * feature and setting disable. */

  പൊതു:
  DEFINE_SIZE_STATIC (12);
};


ഘടന hb_accelerate_subtables_context_t :
       hb_dispatch_context_t<hb_accelerate_subtables_context_t>
{
  ഘടന hb_applicable_t
  {
    friend ഘടന hb_accelerate_subtables_context_t;
    friend ഘടന hb_aat_layout_lookup_accelerator_t;

    പൊതു:
    hb_bit_set_t glyph_set;
    mutable hb_aat_class_cache_t class_cache;

    template <typename T>
    auto init_ (സ്ഥിരം T &obj_, unsigned അക്കം_glyphs, hb_priority<1>) HB_AUTO_RETURN
    (
      obj_.machine.collect_initial_glyphs (glyph_set, അക്കം_glyphs, obj_)
    )

    template <typename T>
    ശൂന്യം init_ (സ്ഥിരം T &obj_, unsigned അക്കം_glyphs, hb_priority<0>)
    {
      obj_.collect_initial_glyphs (glyph_set, അക്കം_glyphs);
    }

    template <typename T>
    ശൂന്യം init (സ്ഥിരം T &obj_, unsigned അക്കം_glyphs)
    {
      glyph_set.init ();
      init_ (obj_, അക്കം_glyphs, hb_prioritize);
      class_cache.clear ();
    }

    ശൂന്യം
    fini ()
    {
      glyph_set.fini ();
    }
  };

  /* Dispatch interface. */
  template <typename T>
  return_t dispatch (സ്ഥിരം T &obj)
  {
    hb_applicable_t *entry = &array[i++];

    entry->init (obj, അക്കം_glyphs);

    തിരികെ hb_empty_t ();
  }
  സ്ഥിതം return_t default_return_മൂല്യം () { തിരികെ hb_empty_t (); }

  തർക്കം stop_sublookup_iteration (return_t r) സ്ഥിരം { തിരികെ അസത്യം; }

  hb_accelerate_subtables_context_t (hb_applicable_t *array_, unsigned അക്കം_glyphs_2) :
				     hb_dispatch_context_t<hb_accelerate_subtables_context_t> (),
				     array (array_), അക്കം_glyphs (അക്കം_glyphs_2) {}

  hb_applicable_t *array;
  unsigned അക്കം_glyphs;
  unsigned i = 0;
};

ഘടന hb_aat_layout_chain_accelerator_t
{
  template <typename TChain>
  സ്ഥിതം hb_aat_layout_chain_accelerator_t *create (സ്ഥിരം TChain &chain, unsigned അക്കം_glyphs)
  {
    unsigned എണ്ണം = chain.get_subtable_എണ്ണം ();

    unsigned product;
    എങ്കിൽ (unlikely (hb_unsigned_mul_overflows (എണ്ണം,
        വലുപ്പം (hb_accelerate_subtables_context_t::hb_applicable_t), &product)))
      തിരികെ nullptr;

    unsigned size;
    എങ്കിൽ (unlikely (hb_unsigned_add_overflows (വലുപ്പം (hb_aat_layout_chain_accelerator_t) -
        HB_VAR_ARRAY * വലുപ്പം (hb_accelerate_subtables_context_t::hb_applicable_t), product, &size))) {
      തിരികെ nullptr;
    }

    /* The following is a calloc because when we are collecting subtables,
     * some of them might be invalid and hence not collect; as a result,
     * we might not fill in all the count entries of the subtables array.
     * Zeroing it allows the set digest to gatekeep it without having to
     * initialize it further. */
    auto *thiz = (hb_aat_layout_chain_accelerator_t *) hb_calloc (1, size);
    എങ്കിൽ (unlikely (!thiz))
      തിരികെ nullptr;

    thiz->എണ്ണം = എണ്ണം;

    hb_accelerate_subtables_context_t c_accelerate_subtables (thiz->subtables, അക്കം_glyphs);
    chain.dispatch (&c_accelerate_subtables);

    തിരികെ thiz;
  }

  ശൂന്യം destroy ()
  {
    വേണ്ടി (unsigned i = 0; i < എണ്ണം; i++)
      subtables[i].fini ();
  }

  unsigned എണ്ണം;
  hb_accelerate_subtables_context_t::hb_applicable_t subtables[HB_VAR_ARRAY];
};

template <typename Types>
ഘടന ChainSubtable
{
  തരനിർവചനം typename Types::HBUINT HBUINT;

  template <typename T>
  friend ഘടന Chain;

  size_t get_size () സ്ഥിരം     { തിരികെ length; }
  unsigned പൂർണ്ണസംഖ്യ get_type () സ്ഥിരം     { തിരികെ coverage & 0xFF; }
  unsigned പൂർണ്ണസംഖ്യ get_coverage () സ്ഥിരം { തിരികെ coverage >> (വലുപ്പം (HBUINT) * 8 - 8); }

  എണ്ണൽ Coverage
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
  എണ്ണൽ Type
  {
    Rearrangement	= 0,
    Contextual		= 1,
    Ligature		= 2,
    Noncontextual	= 4,
    Insertion		= 5
  };

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) സ്ഥിരം
  {
    unsigned പൂർണ്ണസംഖ്യ subtable_type = get_type ();
    TRACE_DISPATCH (ഇത്, subtable_type);
    തിരഞ്ഞെടുപ്പ് (subtable_type) {
    അവസ്ഥ Rearrangement:		hb_barrier (); return_trace (c->dispatch (u.rearrangement, std::forward<Ts> (ds)...));
    അവസ്ഥ Contextual:		hb_barrier (); return_trace (c->dispatch (u.contextual, std::forward<Ts> (ds)...));
    അവസ്ഥ Ligature:		hb_barrier (); return_trace (c->dispatch (u.ligature, std::forward<Ts> (ds)...));
    അവസ്ഥ Noncontextual:		hb_barrier (); return_trace (c->dispatch (u.noncontextual, std::forward<Ts> (ds)...));
    അവസ്ഥ Insertion:		hb_barrier (); return_trace (c->dispatch (u.insertion, std::forward<Ts> (ds)...));
    സ്ഥിരസ്ഥിതി:			return_trace (c->default_return_മൂല്യം ());
    }
  }

  തർക്കം apply (hb_aat_apply_context_t *c) സ്ഥിരം
  {
    TRACE_APPLY (ഇത്);
    // Disabled for https://github.com/harfbuzz/harfbuzz/issues/4873
    //hb_sanitize_with_object_t with (&c->sanitizer, this);
    return_trace (dispatch (c));
  }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    എങ്കിൽ (!(length.sanitize (c) &&
	  hb_barrier () &&
	  length >= min_size &&
	  c->check_range (ഇത്, length)))
      return_trace (അസത്യം);

    // Disabled for https://github.com/harfbuzz/harfbuzz/issues/4873
    //hb_sanitize_with_object_t with (c, this);
    return_trace (dispatch (c));
  }

  സംരക്ഷിത:
  HBUINT	length;		/* Total subtable length, including this header. */
  HBUINT	coverage;	/* Coverage flags and subtable type. */
  HBUINT32	subFeatureFlags;/* The 32-bit mask identifying which subtable this is. */
  സംഘം {
  RearrangementSubtable<Types>	rearrangement;
  ContextualSubtable<Types>	contextual;
  LigatureSubtable<Types>	ligature;
  NoncontextualSubtable<Types>	noncontextual;
  InsertionSubtable<Types>	insertion;
  } u;
  പൊതു:
  DEFINE_SIZE_MIN (2 * വലുപ്പം (HBUINT) + 4);
};

template <typename Types>
ഘടന Chain
{
  തരനിർവചനം typename Types::HBUINT HBUINT;

  unsigned get_subtable_എണ്ണം () സ്ഥിരം { തിരികെ subtable_എണ്ണം_2; }

  hb_mask_t compile_flags (സ്ഥിരം hb_aat_map_builder_t *map) സ്ഥിരം
  {
    hb_mask_t flags = defaultFlags;
    {
      unsigned പൂർണ്ണസംഖ്യ എണ്ണം = feature_എണ്ണം;
      വേണ്ടി (unsigned i = 0; i < എണ്ണം; i++)
      {
	സ്ഥിരം Feature &feature = featureZ[i];
	hb_aat_layout_feature_type_t type = (hb_aat_layout_feature_type_t) (unsigned പൂർണ്ണസംഖ്യ) feature.featureType;
	hb_aat_layout_feature_selector_t setting = (hb_aat_layout_feature_selector_t) (unsigned പൂർണ്ണസംഖ്യ) feature.featureSetting;
      retry:
	// Check whether this type/setting pair was requested in the map, and if so, apply its flags.
	// (The search here only looks at the type and setting fields of feature_info_t.)
	hb_aat_map_builder_t::feature_info_t info = { type, setting, അസത്യം, 0 };
	എങ്കിൽ (map->current_features.bsearch (info))
	{
	  flags &= feature.disableFlags;
	  flags |= feature.enableFlags;
	}
	അല്ലെങ്കിൽ എങ്കിൽ (type == HB_AAT_LAYOUT_FEATURE_TYPE_LETTER_CASE && setting == HB_AAT_LAYOUT_FEATURE_SELECTOR_SMALL_CAPS)
	{
	  /* Deprecated. https://github.com/harfbuzz/harfbuzz/issues/1342 */
	  type = HB_AAT_LAYOUT_FEATURE_TYPE_LOWER_CASE;
	  setting = HB_AAT_LAYOUT_FEATURE_SELECTOR_LOWER_CASE_SMALL_CAPS;
	  പോകുക retry;
	}
#ifndef HB_NO_AAT
	അല്ലെങ്കിൽ എങ്കിൽ (type == HB_AAT_LAYOUT_FEATURE_TYPE_LANGUAGE_TAG_TYPE && setting &&
		 /* TODO: Rudimentary language matching. */
		 hb_language_matches (map->face->table.ltag->get_language (setting - 1), map->props.language))
	{
	  flags &= feature.disableFlags;
	  flags |= feature.enableFlags;
	}
#endif
      }
    }
    തിരികെ flags;
  }

  ശൂന്യം apply (hb_aat_apply_context_t *c,
	      സ്ഥിരം hb_aat_layout_chain_accelerator_t *accel) സ്ഥിരം
  {
    സ്ഥിരം ChainSubtable<Types> *subtable = &StructAfter<ChainSubtable<Types>> (featureZ.as_array (feature_എണ്ണം));
    unsigned പൂർണ്ണസംഖ്യ എണ്ണം = subtable_എണ്ണം_2;
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < എണ്ണം; i++)
    {
      തർക്കം reverse;

      auto coverage = subtable->get_coverage ();

      hb_mask_t subtable_flags = subtable->subFeatureFlags;
      എങ്കിൽ (hb_none (hb_iter (c->range_flags) |
		   hb_map ([subtable_flags] (സ്ഥിരം hb_aat_map_t::range_flags_t _) -> തർക്കം { തിരികെ subtable_flags & (_.flags); })))
	പോകുക skip;

      എങ്കിൽ (!(coverage & ChainSubtable<Types>::AllDirections) &&
	  HB_DIRECTION_IS_VERTICAL (c->buffer->props.direction) !=
	  തർക്കം (coverage & ChainSubtable<Types>::Vertical))
	പോകുക skip;

      c->subtable_flags = subtable_flags;
      c->first_set = accel ? &accel->subtables[i].glyph_set : &Null(hb_bit_set_t);
      c->machine_class_cache = accel ? &accel->subtables[i].class_cache : nullptr;

      എങ്കിൽ (!c->buffer_intersects_machine ())
      {
	(ശൂന്യം) c->buffer->message (c->font, "skipped chainsubtable %u because no glyph matches", c->lookup_index);
	പോകുക skip;
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
		തർക്കം (coverage & ChainSubtable<Types>::Backwards) :
		തർക്കം (coverage & ChainSubtable<Types>::Backwards) !=
		HB_DIRECTION_IS_BACKWARD (c->buffer->props.direction);

      എങ്കിൽ (!c->buffer->message (c->font, "start chainsubtable %u", c->lookup_index))
	പോകുക skip;

      എങ്കിൽ (reverse != c->buffer_is_reversed)
        c->reverse_buffer ();

      subtable->apply (c);

      (ശൂന്യം) c->buffer->message (c->font, "end chainsubtable %u", c->lookup_index);

      എങ്കിൽ (unlikely (!c->buffer->successful)) നിർത്തുക;

    skip:
      subtable = &StructAfter<ChainSubtable<Types>> (*subtable);
      c->set_lookup_index (c->lookup_index + 1);
    }
    എങ്കിൽ (c->buffer_is_reversed)
      c->reverse_buffer ();
  }

  size_t get_size () സ്ഥിരം { തിരികെ length; }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) സ്ഥിരം
  {
    സ്ഥിരം ChainSubtable<Types> *subtable = &StructAfter<ChainSubtable<Types>> (featureZ.as_array (feature_എണ്ണം));
    unsigned പൂർണ്ണസംഖ്യ എണ്ണം = subtable_എണ്ണം_2;
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < എണ്ണം; i++)
    {
      typename context_t::return_t ret = subtable->dispatch (c, std::forward<Ts> (ds)...);
      എങ്കിൽ (c->stop_sublookup_iteration (ret))
	തിരികെ ret;
      subtable = &StructAfter<ChainSubtable<Types>> (*subtable);
    }
    തിരികെ c->default_return_മൂല്യം ();
  }

  തർക്കം sanitize (hb_sanitize_context_t *c, unsigned പൂർണ്ണസംഖ്യ version) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    എങ്കിൽ (!(length.sanitize (c) &&
	  hb_barrier () &&
	  length >= min_size &&
	  c->check_range (ഇത്, length)))
      return_trace (അസത്യം);

    എങ്കിൽ (!c->check_array (featureZ.arrayZ, feature_എണ്ണം))
      return_trace (അസത്യം);

    സ്ഥിരം ChainSubtable<Types> *subtable = &StructAfter<ChainSubtable<Types>> (featureZ.as_array (feature_എണ്ണം));
    unsigned പൂർണ്ണസംഖ്യ എണ്ണം = subtable_എണ്ണം_2;
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < എണ്ണം; i++)
    {
      എങ്കിൽ (!subtable->sanitize (c))
	return_trace (അസത്യം);
      hb_barrier ();
      subtable = &StructAfter<ChainSubtable<Types>> (*subtable);
    }

    എങ്കിൽ (version >= 3)
    {
      സ്ഥിരം SubtableGlyphCoverage *coverage = (സ്ഥിരം SubtableGlyphCoverage *) subtable;
      എങ്കിൽ (!coverage->sanitize (c, എണ്ണം))
        return_trace (അസത്യം);
    }

    return_trace (സത്യം);
  }

  സംരക്ഷിത:
  HBUINT32	defaultFlags;	/* The default specification for subtables. */
  HBUINT32	length;		/* Total byte count, including this header. */
  HBUINT	feature_എണ്ണം;	/* Number of feature subtable entries. */
  HBUINT	subtable_എണ്ണം_2;	/* The number of subtables in the chain. */

  UnsizedArrayOf<Feature>	featureZ;	/* Features. */
/*ChainSubtable	firstSubtable;*//* Subtables. */
/*SubtableGlyphCoverage coverages*//* Only if version >= 3. */

  പൊതു:
  DEFINE_SIZE_MIN (8 + 2 * വലുപ്പം (HBUINT));
};


/*
 * The 'mort'/'morx' Table
 */

template <typename T, typename Types, hb_tag_t TAG>
ഘടന mortmorx
{
  സ്ഥിതം constexpr hb_tag_t tableTag = TAG;

  തർക്കം has_data () സ്ഥിരം { തിരികെ version != 0; }

  ഘടന accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      hb_sanitize_context_t sc;
      ഇത്->table = sc.reference_table<T> (face);

      എങ്കിൽ (unlikely (ഇത്->table->is_blocklisted (ഇത്->table.get_blob (), face)))
      {
        hb_blob_destroy (ഇത്->table.get_blob ());
        ഇത്->table = hb_blob_get_empty ();
      }

      ഇത്->chain_എണ്ണം = table->get_chain_എണ്ണം ();

      ഇത്->accels = (hb_atomic_t<hb_aat_layout_chain_accelerator_t *> *) hb_calloc (ഇത്->chain_എണ്ണം, വലുപ്പം (*accels));
      എങ്കിൽ (unlikely (!ഇത്->accels))
      {
	ഇത്->chain_എണ്ണം = 0;
	ഇത്->table.destroy ();
	ഇത്->table = hb_blob_get_empty ();
      }
    }
    ~accelerator_t ()
    {
      വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < ഇത്->chain_എണ്ണം; i++)
      {
	എങ്കിൽ (ഇത്->accels[i])
	  ഇത്->accels[i]->destroy ();
	hb_free (ഇത്->accels[i]);
      }
      hb_free (ഇത്->accels);
      ഇത്->table.destroy ();
    }

    hb_blob_t *get_blob () സ്ഥിരം { തിരികെ table.get_blob (); }

    template <typename Chain>
    hb_aat_layout_chain_accelerator_t *get_accel (unsigned chain_index, സ്ഥിരം Chain &chain, unsigned അക്കം_glyphs) സ്ഥിരം
    {
      എങ്കിൽ (unlikely (chain_index >= chain_എണ്ണം)) തിരികെ nullptr;

    retry:
      auto *accel = accels[chain_index].get_acquire ();
      എങ്കിൽ (unlikely (!accel))
      {
	accel = hb_aat_layout_chain_accelerator_t::create (chain, അക്കം_glyphs);
	എങ്കിൽ (unlikely (!accel))
	  തിരികെ nullptr;

	എങ്കിൽ (unlikely (!accels[chain_index].cmpexch (nullptr, accel)))
	{
	  hb_free (accel);
	  പോകുക retry;
	}
      }

      തിരികെ accel;
    }

    hb_blob_ptr_t<T> table;
    unsigned പൂർണ്ണസംഖ്യ chain_എണ്ണം;
    hb_atomic_t<hb_aat_layout_chain_accelerator_t *> *accels;
    hb_aat_scratch_t scratch;
  };


  ശൂന്യം compile_flags (സ്ഥിരം hb_aat_map_builder_t *mapper,
		      hb_aat_map_t *map) സ്ഥിരം
  {
    സ്ഥിരം Chain<Types> *chain = &firstChain;
    unsigned പൂർണ്ണസംഖ്യ എണ്ണം = chain_എണ്ണം_2;
    എങ്കിൽ (unlikely (!map->chain_flags.resize (എണ്ണം)))
      തിരികെ;
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < എണ്ണം; i++)
    {
      map->chain_flags[i].push (hb_aat_map_t::range_flags_t {chain->compile_flags (mapper),
							     mapper->range_first,
							     mapper->range_last});
      chain = &StructAfter<Chain<Types>> (*chain);
    }
  }

  unsigned get_chain_എണ്ണം () സ്ഥിരം
  {
    തിരികെ chain_എണ്ണം_2;
  }
  ശൂന്യം apply (hb_aat_apply_context_t *c,
	      സ്ഥിരം hb_aat_map_t &map,
	      സ്ഥിരം accelerator_t &accel) സ്ഥിരം
  {
    എങ്കിൽ (unlikely (!c->buffer->successful)) തിരികെ;

    c->buffer->unsafe_to_concat ();

    c->setup_buffer_glyph_set ();

    c->set_lookup_index (0);
    സ്ഥിരം Chain<Types> *chain = &firstChain;
    unsigned പൂർണ്ണസംഖ്യ എണ്ണം = chain_എണ്ണം_2;
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < എണ്ണം; i++)
    {
      auto *chain_accel = accel.get_accel (i, *chain, c->face->get_അക്കം_glyphs ());
      c->range_flags = &map.chain_flags[i];
      chain->apply (c, chain_accel);
      എങ്കിൽ (unlikely (!c->buffer->successful)) തിരികെ;
      chain = &StructAfter<Chain<Types>> (*chain);
    }
  }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    എങ്കിൽ (!(version.sanitize (c) &&
	  hb_barrier () &&
	  version &&
	  chain_എണ്ണം_2.sanitize (c)))
      return_trace (അസത്യം);

    സ്ഥിരം Chain<Types> *chain = &firstChain;
    unsigned പൂർണ്ണസംഖ്യ എണ്ണം = chain_എണ്ണം_2;
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < എണ്ണം; i++)
    {
      എങ്കിൽ (!chain->sanitize (c, version))
	return_trace (അസത്യം);
      hb_barrier ();
      chain = &StructAfter<Chain<Types>> (*chain);
    }

    return_trace (സത്യം);
  }

  സംരക്ഷിത:
  HBUINT16	version;	/* Version number of the glyph metamorphosis table.
				 * 1, 2, or 3. */
  HBUINT16	unused;		/* Set to 0. */
  HBUINT32	chain_എണ്ണം_2;	/* Number of metamorphosis chains contained in this
				 * table. */
  Chain<Types>	firstChain;	/* Chains. */

  പൊതു:
  DEFINE_SIZE_MIN (8);
};

ഘടന morx : mortmorx<morx, ExtendedTypes, HB_AAT_TAG_morx>
{
  HB_INTERNAL തർക്കം is_blocklisted (hb_blob_t *blob,
                                   hb_face_t *face) സ്ഥിരം;
};

ഘടന mort : mortmorx<mort, ObsoleteTypes, HB_AAT_TAG_mort>
{
  HB_INTERNAL തർക്കം is_blocklisted (hb_blob_t *blob,
                                   hb_face_t *face) സ്ഥിരം;
};

ഘടന morx_accelerator_t : morx::accelerator_t {
  morx_accelerator_t (hb_face_t *face) : morx::accelerator_t (face) {}
};
ഘടന mort_accelerator_t : mort::accelerator_t {
  mort_accelerator_t (hb_face_t *face) : mort::accelerator_t (face) {}
};


} /* namespace AAT */


#endif /* HB_AAT_LAYOUT_MORX_TABLE_HH */
