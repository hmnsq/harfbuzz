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

#অন্তর্ভাব "hb-open-type.hh"
#অন্তর্ভাব "hb-aat-layout-common.hh"
#অন্তর্ভাব "hb-ot-layout.hh"
#অন্তর্ভাব "hb-aat-map.hh"

/*
 * morx -- Extended Glyph Metamorphosis
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6morx.html
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6mort.html
 */
#define HB_AAT_TAG_morx HB_TAG('m','o','r','x')
#define HB_AAT_TAG_mort HB_TAG('m','o','r','t')


নামাকাশ AAT {

using নামাকাশ OT;

template <typename Types>
সংরচনা RearrangementSubtable
{
  প্রকারনির্দেশ typename Types::HBUINT HBUINT;

  প্রকারনির্দেশ শূন্য EntryData;

  গণনা Flags
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

  তার্কিক is_action_initiable (ধ্রুব Entry<EntryData> &entry) ধ্রুব
  {
    প্রত্যাগম (entry.flags & MarkFirst);
  }
  তার্কিক is_actionable (ধ্রুব Entry<EntryData> &entry) ধ্রুব
  {
    প্রত্যাগম (entry.flags & Verb);
  }

  সংরচনা driver_context_t
  {
    স্থির constexpr তার্কিক in_place = সত্য;

    driver_context_t (ধ্রুব RearrangementSubtable *table_) :
	ret (অসত্য),
	table (table_),
	start (0), end (0) {}

    HB_AAT_TRANSITION_INLINE
    শূন্য transition (hb_buffer_t *buffer,
		     StateTableDriver<Types, EntryData, Flags> *driver,
		     ধ্রুব Entry<EntryData> &entry)
    {
      unsigned পূর্ণাঙ্ক flags = entry.flags;

      যদি (flags & MarkFirst)
	start = buffer->idx;

      যদি (flags & MarkLast)
	end = hb_min (buffer->idx + 1, buffer->len);

      যদি ((flags & Verb) && start < end)
      {
	/* The following map has two nibbles, for start-side
	 * and end-side. Values of 0,1,2 mean move that many
	 * to the other side. Value of 3 means move 2 and
	 * flip them. */
	ধ্রুব unsigned বর্ণ map[16] =
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

	unsigned পূর্ণাঙ্ক m = map[flags & Verb];
	unsigned পূর্ণাঙ্ক l = hb_min (2u, m >> 4);
	unsigned পূর্ণাঙ্ক r = hb_min (2u, m & 0x0F);
	তার্কিক reverse_l = 3 == (m >> 4);
	তার্কিক reverse_r = 3 == (m & 0x0F);

	যদি (end - start >= l + r && end-start <= HB_MAX_CONTEXT_LENGTH)
	{
	  buffer->merge_clusters (start, hb_min (buffer->idx + 1, buffer->len));
	  buffer->merge_clusters (start, end);

	  hb_glyph_info_t *info = buffer->info;
	  hb_glyph_info_t buf[4];

	  hb_memcpy (buf, info + start, l * পরিমাণ (buf[0]));
	  hb_memcpy (buf + 2, info + end - r, r * পরিমাণ (buf[0]));

	  যদি (l != r)
	    memmove (info + start + r, info + start + l, (end - start - l - r) * পরিমাণ (buf[0]));

	  hb_memcpy (info + start, buf + 2, r * পরিমাণ (buf[0]));
	  hb_memcpy (info + end - l, buf, l * পরিমাণ (buf[0]));
	  যদি (reverse_l)
	  {
	    buf[0] = info[end - 1];
	    info[end - 1] = info[end - 2];
	    info[end - 2] = buf[0];
	  }
	  যদি (reverse_r)
	  {
	    buf[0] = info[start];
	    info[start] = info[start + 1];
	    info[start + 1] = buf[0];
	  }
	}
      }
    }

    সার্বজনিক:
    তার্কিক ret;
    ধ্রুব RearrangementSubtable *table;
    গুপ্ত:
    unsigned পূর্ণাঙ্ক start;
    unsigned পূর্ণাঙ্ক end;
  };

  তার্কিক apply (hb_aat_apply_context_t *c) ধ্রুব
  {
    TRACE_APPLY (এতত্);

    driver_context_t dc (এতত্);

    StateTableDriver<Types, EntryData, Flags> driver (machine, c->face);

    driver.drive (&dc, c);

    return_trace (dc.ret);
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    return_trace (machine.sanitize (c));
  }

  সার্বজনিক:
  StateTable<Types, EntryData>	machine;
  সার্বজনিক:
  DEFINE_SIZE_STATIC ((StateTable<Types, EntryData>::static_size));
};

template <typename Types>
সংরচনা ContextualSubtable
{
  প্রকারনির্দেশ typename Types::HBUINT HBUINT;

  সংরচনা EntryData
  {
    প্রকারনির্দেশ typename std::conditional<Types::extended, HBUINT16, HBINT16>::type OffsetType;

    OffsetType	markIndex;	/* Index/offset of the substitution table for the
				 * marked glyph. */
    OffsetType	currentIndex;	/* Index/offset of the substitution table for the
				 * current glyph. */
    সার্বজনিক:
    DEFINE_SIZE_STATIC (4);
  };

  গণনা Flags
  {
    SetMark		= 0x8000,	/* If set, make the current glyph the marked glyph. */
    DontAdvance		= 0x4000,	/* If set, don't advance to the next glyph before
					 * going to the new state. */
    Reserved		= 0x3FFF,	/* These bits are reserved and should be set to 0. */
  };

  তার্কিক is_action_initiable (ধ্রুব Entry<EntryData> &entry) ধ্রুব
  {
    প্রত্যাগম (entry.flags & SetMark);
  }
  তার্কিক is_actionable (ধ্রুব Entry<EntryData> &entry) ধ্রুব
  {
    যদি (Types::extended)
      প্রত্যাগম entry.data.markIndex != 0xFFFF || entry.data.currentIndex != 0xFFFF;
    অন্যথা
      প্রত্যাগম entry.data.markIndex || entry.data.currentIndex;
  }

  সংরচনা driver_context_t
  {
    স্থির constexpr তার্কিক in_place = সত্য;

    driver_context_t (ধ্রুব ContextualSubtable *table_,
			     hb_aat_apply_context_t *c_) :
	ret (অসত্য),
	c (c_),
	table (table_),
	mark_set (অসত্য),
	mark (0),
	subs (table+table->substitutionTables) {}

    HB_AAT_TRANSITION_INLINE
    শূন্য transition (hb_buffer_t *buffer,
		     StateTableDriver<Types, EntryData, Flags> *driver,
		     ধ্রুব Entry<EntryData> &entry)
    {
      /* Looks like CoreText applies neither mark nor current substitution for
       * end-of-text if mark was not explicitly set. */
      যদি (buffer->idx == buffer->len && !mark_set)
	প্রত্যাগম;

      ধ্রুব HBGlyphID16 *replacement;

      replacement = nullptr;
      যদি (Types::extended)
      {
	যদি (entry.data.markIndex != 0xFFFF)
	{
	  ধ্রুব Lookup<HBGlyphID16> &lookup = subs[entry.data.markIndex];
	  replacement = lookup.get_মান (buffer->info[mark].codepoint, driver->অঙ্ক_glyphs);
	}
      }
      অন্যথা
      {
	যদি (entry.data.markIndex)
	{
	  পূর্ণাঙ্ক offset = (পূর্ণাঙ্ক) entry.data.markIndex + buffer->info[mark].codepoint;
	  ধ্রুব UnsizedArrayOf<HBGlyphID16> &subs_old = (ধ্রুব UnsizedArrayOf<HBGlyphID16> &) subs;
	  replacement = &subs_old[Types::wordOffsetToIndex (offset, table, subs_old.arrayZ)];
	  যদি (!(replacement->sanitize (&c->sanitizer) &&
		hb_barrier () &&
		*replacement))
	    replacement = nullptr;
	}
      }
      যদি (replacement)
      {
	buffer->unsafe_to_break (mark, hb_min (buffer->idx + 1, buffer->len));
	c->replace_glyph_inplace (mark, *replacement);
	ret = সত্য;
      }

      replacement = nullptr;
      unsigned পূর্ণাঙ্ক idx = hb_min (buffer->idx, buffer->len - 1);
      যদি (Types::extended)
      {
	যদি (entry.data.currentIndex != 0xFFFF)
	{
	  ধ্রুব Lookup<HBGlyphID16> &lookup = subs[entry.data.currentIndex];
	  replacement = lookup.get_মান (buffer->info[idx].codepoint, driver->অঙ্ক_glyphs);
	}
      }
      অন্যথা
      {
	যদি (entry.data.currentIndex)
	{
	  পূর্ণাঙ্ক offset = (পূর্ণাঙ্ক) entry.data.currentIndex + buffer->info[idx].codepoint;
	  ধ্রুব UnsizedArrayOf<HBGlyphID16> &subs_old = (ধ্রুব UnsizedArrayOf<HBGlyphID16> &) subs;
	  replacement = &subs_old[Types::wordOffsetToIndex (offset, table, subs_old.arrayZ)];
	  যদি (!(replacement->sanitize (&c->sanitizer) &&
		hb_barrier () &&
		*replacement))
	    replacement = nullptr;
	}
      }
      যদি (replacement)
      {
	c->replace_glyph_inplace (idx, *replacement);
	ret = সত্য;
      }

      যদি (entry.flags & SetMark)
      {
	mark_set = সত্য;
	mark = buffer->idx;
      }
    }

    সার্বজনিক:
    তার্কিক ret;
    hb_aat_apply_context_t *c;
    ধ্রুব ContextualSubtable *table;
    গুপ্ত:
    তার্কিক mark_set;
    unsigned পূর্ণাঙ্ক mark;
    ধ্রুব UnsizedListOfOffset16To<Lookup<HBGlyphID16>, HBUINT, শূন্য, অসত্য> &subs;
  };

  তার্কিক apply (hb_aat_apply_context_t *c) ধ্রুব
  {
    TRACE_APPLY (এতত্);

    driver_context_t dc (এতত্, c);

    StateTableDriver<Types, EntryData, Flags> driver (machine, c->face);

    driver.drive (&dc, c);

    return_trace (dc.ret);
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);

    unsigned পূর্ণাঙ্ক অঙ্ক_entries = 0;
    যদি (unlikely (!machine.sanitize (c, &অঙ্ক_entries))) return_trace (অসত্য);
    hb_barrier ();

    যদি (!Types::extended)
      return_trace (substitutionTables.sanitize (c, এতত্, 0));

    unsigned পূর্ণাঙ্ক অঙ্ক_lookups = 0;

    ধ্রুব Entry<EntryData> *entries = machine.get_entries ();
    প্রতি (unsigned পূর্ণাঙ্ক i = 0; i < অঙ্ক_entries; i++)
    {
      ধ্রুব EntryData &data = entries[i].data;

      যদি (data.markIndex != 0xFFFF)
	অঙ্ক_lookups = hb_max (অঙ্ক_lookups, 1u + data.markIndex);
      যদি (data.currentIndex != 0xFFFF)
	অঙ্ক_lookups = hb_max (অঙ্ক_lookups, 1u + data.currentIndex);
    }

    return_trace (substitutionTables.sanitize (c, এতত্, অঙ্ক_lookups));
  }

  সার্বজনিক:
  StateTable<Types, EntryData>
		machine;
  রক্ষিত:
  NNOffsetTo<UnsizedListOfOffset16To<Lookup<HBGlyphID16>, HBUINT, শূন্য, অসত্য>, HBUINT>
		substitutionTables;
  সার্বজনিক:
  DEFINE_SIZE_STATIC ((StateTable<Types, EntryData>::static_size + HBUINT::static_size));
};


template <তার্কিক extended>
সংরচনা LigatureEntry;

template <>
সংরচনা LigatureEntry<সত্য>
{

  সংরচনা EntryData
  {
    HBUINT16	ligActionIndex;	/* Index to the first ligActionTable entry
				 * for processing this group, if indicated
				 * by the flags. */
    সার্বজনিক:
    DEFINE_SIZE_STATIC (2);
  };

  গণনা Flags
  {
    SetComponent	= 0x8000,	/* Push this glyph onto the component stack for
					 * eventual processing. */
    DontAdvance		= 0x4000,	/* Leave the glyph pointer at this glyph for the
					   next iteration. */
    PerformAction	= 0x2000,	/* Use the ligActionIndex to process a ligature
					 * group. */
    Reserved		= 0x1FFF,	/* These bits are reserved and should be set to 0. */
  };

  স্থির তার্কিক initiateAction (ধ্রুব Entry<EntryData> &entry)
  { প্রত্যাগম entry.flags & SetComponent; }

  স্থির তার্কিক performAction (ধ্রুব Entry<EntryData> &entry)
  { প্রত্যাগম entry.flags & PerformAction; }

  স্থির unsigned পূর্ণাঙ্ক ligActionIndex (ধ্রুব Entry<EntryData> &entry)
  { প্রত্যাগম entry.data.ligActionIndex; }
};
template <>
সংরচনা LigatureEntry<অসত্য>
{
  প্রকারনির্দেশ শূন্য EntryData;

  গণনা Flags
  {
    SetComponent	= 0x8000,	/* Push this glyph onto the component stack for
					 * eventual processing. */
    DontAdvance		= 0x4000,	/* Leave the glyph pointer at this glyph for the
					   next iteration. */
    Offset		= 0x3FFF,	/* Byte offset from beginning of subtable to the
					 * ligature action list. This value must be a
					 * multiple of 4. */
  };

  স্থির তার্কিক initiateAction (ধ্রুব Entry<EntryData> &entry)
  { প্রত্যাগম entry.flags & SetComponent; }

  স্থির তার্কিক performAction (ধ্রুব Entry<EntryData> &entry)
  { প্রত্যাগম entry.flags & Offset; }

  স্থির unsigned পূর্ণাঙ্ক ligActionIndex (ধ্রুব Entry<EntryData> &entry)
  { প্রত্যাগম entry.flags & Offset; }
};


template <typename Types>
সংরচনা LigatureSubtable
{
  প্রকারনির্দেশ typename Types::HBUINT HBUINT;

  প্রকারনির্দেশ LigatureEntry<Types::extended> LigatureEntryT;
  প্রকারনির্দেশ typename LigatureEntryT::EntryData EntryData;

  গণনা Flags
  {
    DontAdvance	= LigatureEntryT::DontAdvance,
  };

  তার্কিক is_action_initiable (ধ্রুব Entry<EntryData> &entry) ধ্রুব
  {
    প্রত্যাগম LigatureEntryT::initiateAction (entry);
  }
  তার্কিক is_actionable (ধ্রুব Entry<EntryData> &entry) ধ্রুব
  {
    প্রত্যাগম LigatureEntryT::performAction (entry);
  }

  সংরচনা driver_context_t
  {
    স্থির constexpr তার্কিক in_place = অসত্য;
    গণনা LigActionFlags
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

    driver_context_t (ধ্রুব LigatureSubtable *table_,
		      hb_aat_apply_context_t *c_) :
	ret (অসত্য),
	c (c_),
	table (table_),
	ligAction (table+table->ligAction),
	component (table+table->component),
	ligature (table+table->ligature),
	match_length (0) {}

    HB_AAT_TRANSITION_INLINE
    শূন্য transition (hb_buffer_t *buffer,
		     StateTableDriver<Types, EntryData, Flags> *driver,
		     ধ্রুব Entry<EntryData> &entry)
    {
      DEBUG_বার্তা (APPLY, nullptr, "Ligature transition at %u", buffer->idx);
      যদি (entry.flags & LigatureEntryT::SetComponent)
      {
	/* Never mark same index twice, in case DontAdvance was used... */
	যদি (unlikely (match_length && match_positions[(match_length - 1u) % ARRAY_LENGTH (match_positions)] == buffer->out_len))
	  match_length--;

	match_positions[match_length++ % ARRAY_LENGTH (match_positions)] = buffer->out_len;
	DEBUG_বার্তা (APPLY, nullptr, "Set component at %u", buffer->out_len);
      }

      যদি (LigatureEntryT::performAction (entry))
      {
	DEBUG_বার্তা (APPLY, nullptr, "Perform action with %u", match_length);
	unsigned পূর্ণাঙ্ক end = buffer->out_len;

	যদি (unlikely (!match_length))
	  প্রত্যাগম;

	যদি (buffer->idx >= buffer->len)
	  প্রত্যাগম; /* TODO Work on previous instead? */

	unsigned পূর্ণাঙ্ক cursor = match_length;

	unsigned পূর্ণাঙ্ক action_idx = LigatureEntryT::ligActionIndex (entry);
	action_idx = Types::offsetToIndex (action_idx, table, ligAction.arrayZ);
	ধ্রুব HBUINT32 *actionData = &ligAction[action_idx];

	unsigned পূর্ণাঙ্ক ligature_idx = 0;
	unsigned পূর্ণাঙ্ক action;
	কুরু
	{
	  যদি (unlikely (!cursor))
	  {
	    /* Stack underflow.  Clear the stack. */
	    DEBUG_বার্তা (APPLY, nullptr, "Stack underflow");
	    match_length = 0;
	    ভঙ্গ;
	  }

	  DEBUG_বার্তা (APPLY, nullptr, "Moving to stack position %u", cursor - 1);
	  যদি (unlikely (!buffer->move_to (match_positions[--cursor % ARRAY_LENGTH (match_positions)]))) প্রত্যাগম;

	  যদি (unlikely (!actionData->sanitize (&c->sanitizer))) ভঙ্গ;
	  hb_barrier ();
	  action = *actionData;

	  uint32_t uoffset = action & LigActionOffset;
	  যদি (uoffset & 0x20000000)
	    uoffset |= 0xC0000000; /* Sign-extend. */
	  int32_t offset = (int32_t) uoffset;
	  unsigned পূর্ণাঙ্ক component_idx = buffer->cur().codepoint + offset;
	  component_idx = Types::wordOffsetToIndex (component_idx, table, component.arrayZ);
	  ধ্রুব HBUINT16 &componentData = component[component_idx];
	  যদি (unlikely (!componentData.sanitize (&c->sanitizer))) ভঙ্গ;
	  hb_barrier ();
	  ligature_idx += componentData;

	  DEBUG_বার্তা (APPLY, nullptr, "Action store %d last %d",
		     তার্কিক (action & LigActionStore),
		     তার্কিক (action & LigActionLast));
	  যদি (action & (LigActionStore | LigActionLast))
	  {
	    ligature_idx = Types::offsetToIndex (ligature_idx, table, ligature.arrayZ);
	    ধ্রুব HBGlyphID16 &ligatureData = ligature[ligature_idx];
	    যদি (unlikely (!ligatureData.sanitize (&c->sanitizer))) ভঙ্গ;
	    hb_barrier ();
	    hb_codepoint_t lig = ligatureData;

	    DEBUG_বার্তা (APPLY, nullptr, "Produced ligature %u", lig);
	    যদি (unlikely (!c->replace_glyph (lig))) প্রত্যাগম;

	    unsigned পূর্ণাঙ্ক lig_end = match_positions[(match_length - 1u) % ARRAY_LENGTH (match_positions)] + 1u;
	    /* Now go and delete all subsequent components. */
	    যাবত্ (match_length - 1u > cursor)
	    {
	      DEBUG_বার্তা (APPLY, nullptr, "Skipping ligature component");
	      যদি (unlikely (!buffer->move_to (match_positions[--match_length % ARRAY_LENGTH (match_positions)]))) প্রত্যাগম;
	      যদি (!c->delete_glyph ()) প্রত্যাগম;
	    }

	    যদি (unlikely (!buffer->move_to (lig_end))) প্রত্যাগম;
	    buffer->merge_out_clusters (match_positions[cursor % ARRAY_LENGTH (match_positions)], buffer->out_len);
	  }

	  actionData++;
	}
	যাবত্ (!(action & LigActionLast));
	যদি (unlikely (!buffer->move_to (end))) প্রত্যাগম;
      }
    }

    সার্বজনিক:
    তার্কিক ret;
    hb_aat_apply_context_t *c;
    ধ্রুব LigatureSubtable *table;
    গুপ্ত:
    ধ্রুব UnsizedArrayOf<HBUINT32> &ligAction;
    ধ্রুব UnsizedArrayOf<HBUINT16> &component;
    ধ্রুব UnsizedArrayOf<HBGlyphID16> &ligature;
    unsigned পূর্ণাঙ্ক match_length;
    unsigned পূর্ণাঙ্ক match_positions[HB_MAX_CONTEXT_LENGTH];
  };

  তার্কিক apply (hb_aat_apply_context_t *c) ধ্রুব
  {
    TRACE_APPLY (এতত্);

    driver_context_t dc (এতত্, c);

    StateTableDriver<Types, EntryData, Flags> driver (machine, c->face);

    driver.drive (&dc, c);

    return_trace (dc.ret);
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    /* The rest of array sanitizations are done at run-time. */
    return_trace (c->check_struct (এতত্) && machine.sanitize (c) &&
		  hb_barrier () &&
		  ligAction && component && ligature);
  }

  সার্বজনিক:
  StateTable<Types, EntryData>
		machine;
  রক্ষিত:
  NNOffsetTo<UnsizedArrayOf<HBUINT32>, HBUINT>
		ligAction;	/* Offset to the ligature action table. */
  NNOffsetTo<UnsizedArrayOf<HBUINT16>, HBUINT>
		component;	/* Offset to the component table. */
  NNOffsetTo<UnsizedArrayOf<HBGlyphID16>, HBUINT>
		ligature;	/* Offset to the actual ligature lists. */
  সার্বজনিক:
  DEFINE_SIZE_STATIC ((StateTable<Types, EntryData>::static_size + 3 * HBUINT::static_size));
};

template <typename Types>
সংরচনা NoncontextualSubtable
{
  তার্কিক apply (hb_aat_apply_context_t *c) ধ্রুব
  {
    TRACE_APPLY (এতত্);

    তার্কিক ret = অসত্য;
    unsigned পূর্ণাঙ্ক অঙ্ক_glyphs = c->face->get_অঙ্ক_glyphs ();

    hb_glyph_info_t *info = c->buffer->info;
    unsigned পূর্ণাঙ্ক গুনতি = c->buffer->len;
    // If there's only one range, we already checked the flag.
    auto *last_range = c->range_flags && (c->range_flags->length > 1) ? &(*c->range_flags)[0] : nullptr;
    প্রতি (unsigned পূর্ণাঙ্ক i = 0; i < গুনতি; i++)
    {
      /* This block copied from StateTableDriver::drive. Keep in sync. */
      যদি (unlikely (last_range))
      {
	auto *range = last_range;
	{
	  unsigned cluster = info[i].cluster;
	  যাবত্ (cluster < range->cluster_first)
	    range--;
	  যাবত্ (cluster > range->cluster_last)
	    range++;

	  last_range = range;
	}
	যদি (!(range->flags & c->subtable_flags))
	  অনুবর্ত;
      }

      ধ্রুব HBGlyphID16 *replacement = substitute.get_মান (info[i].codepoint, অঙ্ক_glyphs);
      যদি (replacement)
      {
	c->replace_glyph_inplace (i, *replacement);
	ret = সত্য;
      }
    }

    return_trace (ret);
  }

  template <typename set_t>
  শূন্য collect_initial_glyphs (set_t &glyphs, unsigned অঙ্ক_glyphs) ধ্রুব
  {
    substitute.collect_glyphs (glyphs, অঙ্ক_glyphs);
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    return_trace (substitute.sanitize (c));
  }

  রক্ষিত:
  Lookup<HBGlyphID16>	substitute;
  সার্বজনিক:
  DEFINE_SIZE_MIN (2);
};

template <typename Types>
সংরচনা InsertionSubtable
{
  প্রকারনির্দেশ typename Types::HBUINT HBUINT;

  সংরচনা EntryData
  {
    HBUINT16	current_ঢোকাও_Index;	/* Zero-based index into the insertion glyph table.
					 * The number of glyphs to be inserted is contained
					 * in the currentInsertCount field in the flags.
					 * A value of 0xFFFF indicates no insertion is to
					 * be done. */
    HBUINT16	marked_ঢোকাও_Index;	/* Zero-based index into the insertion glyph table.
					 * The number of glyphs to be inserted is contained
					 * in the markedInsertCount field in the flags.
					 * A value of 0xFFFF indicates no insertion is to
					 * be done. */
    সার্বজনিক:
    DEFINE_SIZE_STATIC (4);
  };

  গণনা Flags
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
    Current_ঢোকাও_Before= 0x0800,      /* If set, specifies that insertions are to be made
				       * to the left of the current glyph. If clear,
				       * they're made to the right of the current glyph. */
    Marked_ঢোকাও_Before= 0x0400,	      /* If set, specifies that insertions are to be
				       * made to the left of the marked glyph. If clear,
				       * they're made to the right of the marked glyph. */
    Current_ঢোকাও_গুনতি= 0x3E0,	      /* This 5-bit field is treated as a count of the
				       * number of glyphs to insert at the current
				       * position. Since zero means no insertions, the
				       * largest number of insertions at any given
				       * current location is 31 glyphs. */
    Marked_ঢোকাও_গুনতি= 0x001F,	      /* This 5-bit field is treated as a count of the
				       * number of glyphs to insert at the marked
				       * position. Since zero means no insertions, the
				       * largest number of insertions at any given
				       * marked location is 31 glyphs. */
  };

  তার্কিক is_action_initiable (ধ্রুব Entry<EntryData> &entry) ধ্রুব
  {
    প্রত্যাগম (entry.flags & SetMark);
  }
  তার্কিক is_actionable (ধ্রুব Entry<EntryData> &entry) ধ্রুব
  {
    প্রত্যাগম (entry.flags & (Current_ঢোকাও_গুনতি | Marked_ঢোকাও_গুনতি)) &&
	   (entry.data.current_ঢোকাও_Index != 0xFFFF ||entry.data.marked_ঢোকাও_Index != 0xFFFF);
  }

  সংরচনা driver_context_t
  {
    স্থির constexpr তার্কিক in_place = অসত্য;

    driver_context_t (ধ্রুব InsertionSubtable *table_,
		      hb_aat_apply_context_t *c_) :
	ret (অসত্য),
	c (c_),
	table (table_),
	mark (0),
	insertionAction (table+table->insertionAction) {}

    HB_AAT_TRANSITION_INLINE
    শূন্য transition (hb_buffer_t *buffer,
		     StateTableDriver<Types, EntryData, Flags> *driver,
		     ধ্রুব Entry<EntryData> &entry)
    {
      unsigned পূর্ণাঙ্ক flags = entry.flags;

      unsigned mark_loc = buffer->out_len;

      যদি (entry.data.marked_ঢোকাও_Index != 0xFFFF)
      {
	unsigned পূর্ণাঙ্ক গুনতি = (flags & Marked_ঢোকাও_গুনতি);
	যদি (unlikely ((buffer->max_ops -= গুনতি) <= 0)) প্রত্যাগম;
	unsigned পূর্ণাঙ্ক start = entry.data.marked_ঢোকাও_Index;
	ধ্রুব HBGlyphID16 *glyphs = &insertionAction[start];
	যদি (unlikely (!c->sanitizer.check_array (glyphs, গুনতি))) গুনতি = 0;
	hb_barrier ();

	তার্কিক before = flags & Marked_ঢোকাও_Before;

	unsigned পূর্ণাঙ্ক end = buffer->out_len;
	যদি (unlikely (!buffer->move_to (mark))) প্রত্যাগম;

	যদি (buffer->idx < buffer->len && !before)
	  যদি (unlikely (!buffer->copy_glyph ())) প্রত্যাগম;
	/* TODO We ignore KashidaLike setting. */
	যদি (unlikely (!c->output_glyphs (গুনতি, glyphs))) প্রত্যাগম;
	ret = সত্য;
	যদি (buffer->idx < buffer->len && !before)
	  buffer->skip_glyph ();

	যদি (unlikely (!buffer->move_to (end + গুনতি))) প্রত্যাগম;

	buffer->unsafe_to_break_from_outbuffer (mark, hb_min (buffer->idx + 1, buffer->len));
      }

      যদি (flags & SetMark)
	mark = mark_loc;

      যদি (entry.data.current_ঢোকাও_Index != 0xFFFF)
      {
	unsigned পূর্ণাঙ্ক গুনতি = (flags & Current_ঢোকাও_গুনতি) >> 5;
	যদি (unlikely ((buffer->max_ops -= গুনতি) <= 0)) প্রত্যাগম;
	unsigned পূর্ণাঙ্ক start = entry.data.current_ঢোকাও_Index;
	ধ্রুব HBGlyphID16 *glyphs = &insertionAction[start];
	যদি (unlikely (!c->sanitizer.check_array (glyphs, গুনতি))) গুনতি = 0;
	hb_barrier ();

	তার্কিক before = flags & Current_ঢোকাও_Before;

	unsigned পূর্ণাঙ্ক end = buffer->out_len;

	যদি (buffer->idx < buffer->len && !before)
	  যদি (unlikely (!buffer->copy_glyph ())) প্রত্যাগম;
	/* TODO We ignore KashidaLike setting. */
	যদি (unlikely (!c->output_glyphs (গুনতি, glyphs))) প্রত্যাগম;
	ret = সত্য;
	যদি (buffer->idx < buffer->len && !before)
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
	যদি (unlikely (!buffer->move_to ((flags & DontAdvance) ? end : end + গুনতি))) প্রত্যাগম;
      }
    }

    সার্বজনিক:
    তার্কিক ret;
    hb_aat_apply_context_t *c;
    ধ্রুব InsertionSubtable *table;
    গুপ্ত:
    unsigned পূর্ণাঙ্ক mark;
    ধ্রুব UnsizedArrayOf<HBGlyphID16> &insertionAction;
  };

  তার্কিক apply (hb_aat_apply_context_t *c) ধ্রুব
  {
    TRACE_APPLY (এতত্);

    driver_context_t dc (এতত্, c);

    StateTableDriver<Types, EntryData, Flags> driver (machine, c->face);

    driver.drive (&dc, c);

    return_trace (dc.ret);
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    /* The rest of array sanitizations are done at run-time. */
    return_trace (c->check_struct (এতত্) && machine.sanitize (c) &&
		  hb_barrier () &&
		  insertionAction);
  }

  সার্বজনিক:
  StateTable<Types, EntryData>
		machine;
  রক্ষিত:
  NNOffsetTo<UnsizedArrayOf<HBGlyphID16>, HBUINT>
		insertionAction;	/* Byte offset from stateHeader to the start of
					 * the insertion glyph table. */
  সার্বজনিক:
  DEFINE_SIZE_STATIC ((StateTable<Types, EntryData>::static_size + HBUINT::static_size));
};


সংরচনা Feature
{
  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্));
  }

  সার্বজনিক:
  HBUINT16	featureType;	/* The type of feature. */
  HBUINT16	featureSetting;	/* The feature's setting (aka selector). */
  HBUINT32	enableFlags;	/* Flags for the settings that this feature
				 * and setting enables. */
  HBUINT32	disableFlags;	/* Complement of flags for the settings that this
				 * feature and setting disable. */

  সার্বজনিক:
  DEFINE_SIZE_STATIC (12);
};


সংরচনা hb_accelerate_subtables_context_t :
       hb_dispatch_context_t<hb_accelerate_subtables_context_t>
{
  সংরচনা hb_applicable_t
  {
    friend সংরচনা hb_accelerate_subtables_context_t;
    friend সংরচনা hb_aat_layout_lookup_accelerator_t;

    সার্বজনিক:
    hb_bit_set_t glyph_set;
    mutable hb_aat_class_cache_t class_cache;

    template <typename T>
    auto init_ (ধ্রুব T &obj_, unsigned অঙ্ক_glyphs, hb_priority<1>) HB_AUTO_RETURN
    (
      obj_.machine.collect_initial_glyphs (glyph_set, অঙ্ক_glyphs, obj_)
    )

    template <typename T>
    শূন্য init_ (ধ্রুব T &obj_, unsigned অঙ্ক_glyphs, hb_priority<0>)
    {
      obj_.collect_initial_glyphs (glyph_set, অঙ্ক_glyphs);
    }

    template <typename T>
    শূন্য init (ধ্রুব T &obj_, unsigned অঙ্ক_glyphs)
    {
      glyph_set.init ();
      init_ (obj_, অঙ্ক_glyphs, hb_prioritize);
      class_cache.clear ();
    }

    শূন্য
    fini ()
    {
      glyph_set.fini ();
    }
  };

  /* Dispatch interface. */
  template <typename T>
  return_t dispatch (ধ্রুব T &obj)
  {
    hb_applicable_t *entry = &array[i++];

    entry->init (obj, অঙ্ক_glyphs);

    প্রত্যাগম hb_empty_t ();
  }
  স্থির return_t default_return_মান () { প্রত্যাগম hb_empty_t (); }

  তার্কিক stop_sublookup_iteration (return_t r) ধ্রুব { প্রত্যাগম অসত্য; }

  hb_accelerate_subtables_context_t (hb_applicable_t *array_, unsigned অঙ্ক_glyphs_2) :
				     hb_dispatch_context_t<hb_accelerate_subtables_context_t> (),
				     array (array_), অঙ্ক_glyphs (অঙ্ক_glyphs_2) {}

  hb_applicable_t *array;
  unsigned অঙ্ক_glyphs;
  unsigned i = 0;
};

সংরচনা hb_aat_layout_chain_accelerator_t
{
  template <typename TChain>
  স্থির hb_aat_layout_chain_accelerator_t *create (ধ্রুব TChain &chain, unsigned অঙ্ক_glyphs)
  {
    unsigned গুনতি = chain.get_subtable_গুনতি ();

    unsigned product;
    যদি (unlikely (hb_unsigned_mul_overflows (গুনতি,
        পরিমাণ (hb_accelerate_subtables_context_t::hb_applicable_t), &product)))
      প্রত্যাগম nullptr;

    unsigned size;
    যদি (unlikely (hb_unsigned_add_overflows (পরিমাণ (hb_aat_layout_chain_accelerator_t) -
        HB_VAR_ARRAY * পরিমাণ (hb_accelerate_subtables_context_t::hb_applicable_t), product, &size))) {
      প্রত্যাগম nullptr;
    }

    /* The following is a calloc because when we are collecting subtables,
     * some of them might be invalid and hence not collect; as a result,
     * we might not fill in all the count entries of the subtables array.
     * Zeroing it allows the set digest to gatekeep it without having to
     * initialize it further. */
    auto *thiz = (hb_aat_layout_chain_accelerator_t *) hb_calloc (1, size);
    যদি (unlikely (!thiz))
      প্রত্যাগম nullptr;

    thiz->গুনতি = গুনতি;

    hb_accelerate_subtables_context_t c_accelerate_subtables (thiz->subtables, অঙ্ক_glyphs);
    chain.dispatch (&c_accelerate_subtables);

    প্রত্যাগম thiz;
  }

  শূন্য destroy ()
  {
    প্রতি (unsigned i = 0; i < গুনতি; i++)
      subtables[i].fini ();
  }

  unsigned গুনতি;
  hb_accelerate_subtables_context_t::hb_applicable_t subtables[HB_VAR_ARRAY];
};

template <typename Types>
সংরচনা ChainSubtable
{
  প্রকারনির্দেশ typename Types::HBUINT HBUINT;

  template <typename T>
  friend সংরচনা Chain;

  size_t get_size () ধ্রুব     { প্রত্যাগম length; }
  unsigned পূর্ণাঙ্ক get_type () ধ্রুব     { প্রত্যাগম coverage & 0xFF; }
  unsigned পূর্ণাঙ্ক get_coverage () ধ্রুব { প্রত্যাগম coverage >> (পরিমাণ (HBUINT) * 8 - 8); }

  গণনা Coverage
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
  গণনা Type
  {
    Rearrangement	= 0,
    Contextual		= 1,
    Ligature		= 2,
    Noncontextual	= 4,
    Insertion		= 5
  };

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ধ্রুব
  {
    unsigned পূর্ণাঙ্ক subtable_type = get_type ();
    TRACE_DISPATCH (এতত্, subtable_type);
    বিকল্প (subtable_type) {
    পক্ষ Rearrangement:		hb_barrier (); return_trace (c->dispatch (u.rearrangement, std::forward<Ts> (ds)...));
    পক্ষ Contextual:		hb_barrier (); return_trace (c->dispatch (u.contextual, std::forward<Ts> (ds)...));
    পক্ষ Ligature:		hb_barrier (); return_trace (c->dispatch (u.ligature, std::forward<Ts> (ds)...));
    পক্ষ Noncontextual:		hb_barrier (); return_trace (c->dispatch (u.noncontextual, std::forward<Ts> (ds)...));
    পক্ষ Insertion:		hb_barrier (); return_trace (c->dispatch (u.insertion, std::forward<Ts> (ds)...));
    স্বভাব:			return_trace (c->default_return_মান ());
    }
  }

  তার্কিক apply (hb_aat_apply_context_t *c) ধ্রুব
  {
    TRACE_APPLY (এতত্);
    // Disabled for https://github.com/harfbuzz/harfbuzz/issues/4873
    //hb_sanitize_with_object_t with (&c->sanitizer, this);
    return_trace (dispatch (c));
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    যদি (!(length.sanitize (c) &&
	  hb_barrier () &&
	  length >= min_size &&
	  c->check_range (এতত্, length)))
      return_trace (অসত্য);

    // Disabled for https://github.com/harfbuzz/harfbuzz/issues/4873
    //hb_sanitize_with_object_t with (c, this);
    return_trace (dispatch (c));
  }

  রক্ষিত:
  HBUINT	length;		/* Total subtable length, including this header. */
  HBUINT	coverage;	/* Coverage flags and subtable type. */
  HBUINT32	subFeatureFlags;/* The 32-bit mask identifying which subtable this is. */
  সঙ্ঘ {
  RearrangementSubtable<Types>	rearrangement;
  ContextualSubtable<Types>	contextual;
  LigatureSubtable<Types>	ligature;
  NoncontextualSubtable<Types>	noncontextual;
  InsertionSubtable<Types>	insertion;
  } u;
  সার্বজনিক:
  DEFINE_SIZE_MIN (2 * পরিমাণ (HBUINT) + 4);
};

template <typename Types>
সংরচনা Chain
{
  প্রকারনির্দেশ typename Types::HBUINT HBUINT;

  unsigned get_subtable_গুনতি () ধ্রুব { প্রত্যাগম subtable_গুনতি_2; }

  hb_mask_t compile_flags (ধ্রুব hb_aat_map_builder_t *map) ধ্রুব
  {
    hb_mask_t flags = defaultFlags;
    {
      unsigned পূর্ণাঙ্ক গুনতি = feature_গুনতি;
      প্রতি (unsigned i = 0; i < গুনতি; i++)
      {
	ধ্রুব Feature &feature = featureZ[i];
	hb_aat_layout_feature_type_t type = (hb_aat_layout_feature_type_t) (unsigned পূর্ণাঙ্ক) feature.featureType;
	hb_aat_layout_feature_selector_t setting = (hb_aat_layout_feature_selector_t) (unsigned পূর্ণাঙ্ক) feature.featureSetting;
      retry:
	// Check whether this type/setting pair was requested in the map, and if so, apply its flags.
	// (The search here only looks at the type and setting fields of feature_info_t.)
	hb_aat_map_builder_t::feature_info_t info = { type, setting, অসত্য, 0 };
	যদি (map->current_features.bsearch (info))
	{
	  flags &= feature.disableFlags;
	  flags |= feature.enableFlags;
	}
	অন্যথা যদি (type == HB_AAT_LAYOUT_FEATURE_TYPE_LETTER_CASE && setting == HB_AAT_LAYOUT_FEATURE_SELECTOR_SMALL_CAPS)
	{
	  /* Deprecated. https://github.com/harfbuzz/harfbuzz/issues/1342 */
	  type = HB_AAT_LAYOUT_FEATURE_TYPE_LOWER_CASE;
	  setting = HB_AAT_LAYOUT_FEATURE_SELECTOR_LOWER_CASE_SMALL_CAPS;
	  গচ্ছ retry;
	}
#ifndef HB_NO_AAT
	অন্যথা যদি (type == HB_AAT_LAYOUT_FEATURE_TYPE_LANGUAGE_TAG_TYPE && setting &&
		 /* TODO: Rudimentary language matching. */
		 hb_language_matches (map->face->table.ltag->get_language (setting - 1), map->props.language))
	{
	  flags &= feature.disableFlags;
	  flags |= feature.enableFlags;
	}
#endif
      }
    }
    প্রত্যাগম flags;
  }

  শূন্য apply (hb_aat_apply_context_t *c,
	      ধ্রুব hb_aat_layout_chain_accelerator_t *accel) ধ্রুব
  {
    ধ্রুব ChainSubtable<Types> *subtable = &StructAfter<ChainSubtable<Types>> (featureZ.as_array (feature_গুনতি));
    unsigned পূর্ণাঙ্ক গুনতি = subtable_গুনতি_2;
    প্রতি (unsigned পূর্ণাঙ্ক i = 0; i < গুনতি; i++)
    {
      তার্কিক reverse;

      auto coverage = subtable->get_coverage ();

      hb_mask_t subtable_flags = subtable->subFeatureFlags;
      যদি (hb_none (hb_iter (c->range_flags) |
		   hb_map ([subtable_flags] (ধ্রুব hb_aat_map_t::range_flags_t _) -> তার্কিক { প্রত্যাগম subtable_flags & (_.flags); })))
	গচ্ছ skip;

      যদি (!(coverage & ChainSubtable<Types>::AllDirections) &&
	  HB_DIRECTION_IS_VERTICAL (c->buffer->props.direction) !=
	  তার্কিক (coverage & ChainSubtable<Types>::Vertical))
	গচ্ছ skip;

      c->subtable_flags = subtable_flags;
      c->first_set = accel ? &accel->subtables[i].glyph_set : &Null(hb_bit_set_t);
      c->machine_class_cache = accel ? &accel->subtables[i].class_cache : nullptr;

      যদি (!c->buffer_intersects_machine ())
      {
	(শূন্য) c->buffer->message (c->font, "skipped chainsubtable %u because no glyph matches", c->lookup_index);
	গচ্ছ skip;
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
		তার্কিক (coverage & ChainSubtable<Types>::Backwards) :
		তার্কিক (coverage & ChainSubtable<Types>::Backwards) !=
		HB_DIRECTION_IS_BACKWARD (c->buffer->props.direction);

      যদি (!c->buffer->message (c->font, "start chainsubtable %u", c->lookup_index))
	গচ্ছ skip;

      যদি (reverse != c->buffer_is_reversed)
        c->reverse_buffer ();

      subtable->apply (c);

      (শূন্য) c->buffer->message (c->font, "end chainsubtable %u", c->lookup_index);

      যদি (unlikely (!c->buffer->successful)) ভঙ্গ;

    skip:
      subtable = &StructAfter<ChainSubtable<Types>> (*subtable);
      c->set_lookup_index (c->lookup_index + 1);
    }
    যদি (c->buffer_is_reversed)
      c->reverse_buffer ();
  }

  size_t get_size () ধ্রুব { প্রত্যাগম length; }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ধ্রুব
  {
    ধ্রুব ChainSubtable<Types> *subtable = &StructAfter<ChainSubtable<Types>> (featureZ.as_array (feature_গুনতি));
    unsigned পূর্ণাঙ্ক গুনতি = subtable_গুনতি_2;
    প্রতি (unsigned পূর্ণাঙ্ক i = 0; i < গুনতি; i++)
    {
      typename context_t::return_t ret = subtable->dispatch (c, std::forward<Ts> (ds)...);
      যদি (c->stop_sublookup_iteration (ret))
	প্রত্যাগম ret;
      subtable = &StructAfter<ChainSubtable<Types>> (*subtable);
    }
    প্রত্যাগম c->default_return_মান ();
  }

  তার্কিক sanitize (hb_sanitize_context_t *c, unsigned পূর্ণাঙ্ক version) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    যদি (!(length.sanitize (c) &&
	  hb_barrier () &&
	  length >= min_size &&
	  c->check_range (এতত্, length)))
      return_trace (অসত্য);

    যদি (!c->check_array (featureZ.arrayZ, feature_গুনতি))
      return_trace (অসত্য);

    ধ্রুব ChainSubtable<Types> *subtable = &StructAfter<ChainSubtable<Types>> (featureZ.as_array (feature_গুনতি));
    unsigned পূর্ণাঙ্ক গুনতি = subtable_গুনতি_2;
    প্রতি (unsigned পূর্ণাঙ্ক i = 0; i < গুনতি; i++)
    {
      যদি (!subtable->sanitize (c))
	return_trace (অসত্য);
      hb_barrier ();
      subtable = &StructAfter<ChainSubtable<Types>> (*subtable);
    }

    যদি (version >= 3)
    {
      ধ্রুব SubtableGlyphCoverage *coverage = (ধ্রুব SubtableGlyphCoverage *) subtable;
      যদি (!coverage->sanitize (c, গুনতি))
        return_trace (অসত্য);
    }

    return_trace (সত্য);
  }

  রক্ষিত:
  HBUINT32	defaultFlags;	/* The default specification for subtables. */
  HBUINT32	length;		/* Total byte count, including this header. */
  HBUINT	feature_গুনতি;	/* Number of feature subtable entries. */
  HBUINT	subtable_গুনতি_2;	/* The number of subtables in the chain. */

  UnsizedArrayOf<Feature>	featureZ;	/* Features. */
/*ChainSubtable	firstSubtable;*//* Subtables. */
/*SubtableGlyphCoverage coverages*//* Only if version >= 3. */

  সার্বজনিক:
  DEFINE_SIZE_MIN (8 + 2 * পরিমাণ (HBUINT));
};


/*
 * The 'mort'/'morx' Table
 */

template <typename T, typename Types, hb_tag_t TAG>
সংরচনা mortmorx
{
  স্থির constexpr hb_tag_t tableTag = TAG;

  তার্কিক has_data () ধ্রুব { প্রত্যাগম version != 0; }

  সংরচনা accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      hb_sanitize_context_t sc;
      এতত্->table = sc.reference_table<T> (face);

      যদি (unlikely (এতত্->table->is_blocklisted (এতত্->table.get_blob (), face)))
      {
        hb_blob_destroy (এতত্->table.get_blob ());
        এতত্->table = hb_blob_get_empty ();
      }

      এতত্->chain_গুনতি = table->get_chain_গুনতি ();

      এতত্->accels = (hb_atomic_t<hb_aat_layout_chain_accelerator_t *> *) hb_calloc (এতত্->chain_গুনতি, পরিমাণ (*accels));
      যদি (unlikely (!এতত্->accels))
      {
	এতত্->chain_গুনতি = 0;
	এতত্->table.destroy ();
	এতত্->table = hb_blob_get_empty ();
      }
    }
    ~accelerator_t ()
    {
      প্রতি (unsigned পূর্ণাঙ্ক i = 0; i < এতত্->chain_গুনতি; i++)
      {
	যদি (এতত্->accels[i])
	  এতত্->accels[i]->destroy ();
	hb_free (এতত্->accels[i]);
      }
      hb_free (এতত্->accels);
      এতত্->table.destroy ();
    }

    hb_blob_t *get_blob () ধ্রুব { প্রত্যাগম table.get_blob (); }

    template <typename Chain>
    hb_aat_layout_chain_accelerator_t *get_accel (unsigned chain_index, ধ্রুব Chain &chain, unsigned অঙ্ক_glyphs) ধ্রুব
    {
      যদি (unlikely (chain_index >= chain_গুনতি)) প্রত্যাগম nullptr;

    retry:
      auto *accel = accels[chain_index].get_acquire ();
      যদি (unlikely (!accel))
      {
	accel = hb_aat_layout_chain_accelerator_t::create (chain, অঙ্ক_glyphs);
	যদি (unlikely (!accel))
	  প্রত্যাগম nullptr;

	যদি (unlikely (!accels[chain_index].cmpexch (nullptr, accel)))
	{
	  hb_free (accel);
	  গচ্ছ retry;
	}
      }

      প্রত্যাগম accel;
    }

    hb_blob_ptr_t<T> table;
    unsigned পূর্ণাঙ্ক chain_গুনতি;
    hb_atomic_t<hb_aat_layout_chain_accelerator_t *> *accels;
    hb_aat_scratch_t scratch;
  };


  শূন্য compile_flags (ধ্রুব hb_aat_map_builder_t *mapper,
		      hb_aat_map_t *map) ধ্রুব
  {
    ধ্রুব Chain<Types> *chain = &firstChain;
    unsigned পূর্ণাঙ্ক গুনতি = chain_গুনতি_2;
    যদি (unlikely (!map->chain_flags.resize (গুনতি)))
      প্রত্যাগম;
    প্রতি (unsigned পূর্ণাঙ্ক i = 0; i < গুনতি; i++)
    {
      map->chain_flags[i].push (hb_aat_map_t::range_flags_t {chain->compile_flags (mapper),
							     mapper->range_first,
							     mapper->range_last});
      chain = &StructAfter<Chain<Types>> (*chain);
    }
  }

  unsigned get_chain_গুনতি () ধ্রুব
  {
    প্রত্যাগম chain_গুনতি_2;
  }
  শূন্য apply (hb_aat_apply_context_t *c,
	      ধ্রুব hb_aat_map_t &map,
	      ধ্রুব accelerator_t &accel) ধ্রুব
  {
    যদি (unlikely (!c->buffer->successful)) প্রত্যাগম;

    c->buffer->unsafe_to_concat ();

    c->setup_buffer_glyph_set ();

    c->set_lookup_index (0);
    ধ্রুব Chain<Types> *chain = &firstChain;
    unsigned পূর্ণাঙ্ক গুনতি = chain_গুনতি_2;
    প্রতি (unsigned পূর্ণাঙ্ক i = 0; i < গুনতি; i++)
    {
      auto *chain_accel = accel.get_accel (i, *chain, c->face->get_অঙ্ক_glyphs ());
      c->range_flags = &map.chain_flags[i];
      chain->apply (c, chain_accel);
      যদি (unlikely (!c->buffer->successful)) প্রত্যাগম;
      chain = &StructAfter<Chain<Types>> (*chain);
    }
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    যদি (!(version.sanitize (c) &&
	  hb_barrier () &&
	  version &&
	  chain_গুনতি_2.sanitize (c)))
      return_trace (অসত্য);

    ধ্রুব Chain<Types> *chain = &firstChain;
    unsigned পূর্ণাঙ্ক গুনতি = chain_গুনতি_2;
    প্রতি (unsigned পূর্ণাঙ্ক i = 0; i < গুনতি; i++)
    {
      যদি (!chain->sanitize (c, version))
	return_trace (অসত্য);
      hb_barrier ();
      chain = &StructAfter<Chain<Types>> (*chain);
    }

    return_trace (সত্য);
  }

  রক্ষিত:
  HBUINT16	version;	/* Version number of the glyph metamorphosis table.
				 * 1, 2, or 3. */
  HBUINT16	unused;		/* Set to 0. */
  HBUINT32	chain_গুনতি_2;	/* Number of metamorphosis chains contained in this
				 * table. */
  Chain<Types>	firstChain;	/* Chains. */

  সার্বজনিক:
  DEFINE_SIZE_MIN (8);
};

সংরচনা morx : mortmorx<morx, ExtendedTypes, HB_AAT_TAG_morx>
{
  HB_INTERNAL তার্কিক is_blocklisted (hb_blob_t *blob,
                                   hb_face_t *face) ধ্রুব;
};

সংরচনা mort : mortmorx<mort, ObsoleteTypes, HB_AAT_TAG_mort>
{
  HB_INTERNAL তার্কিক is_blocklisted (hb_blob_t *blob,
                                   hb_face_t *face) ধ্রুব;
};

সংরচনা morx_accelerator_t : morx::accelerator_t {
  morx_accelerator_t (hb_face_t *face) : morx::accelerator_t (face) {}
};
সংরচনা mort_accelerator_t : mort::accelerator_t {
  mort_accelerator_t (hb_face_t *face) : mort::accelerator_t (face) {}
};


} /* namespace AAT */


#endif /* HB_AAT_LAYOUT_MORX_TABLE_HH */
