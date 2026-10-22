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

#ಅನ್ತರ್ಭಾವ "hb-open-type.hh"
#ಅನ್ತರ್ಭಾವ "hb-aat-layout-common.hh"
#ಅನ್ತರ್ಭಾವ "hb-ot-layout.hh"
#ಅನ್ತರ್ಭಾವ "hb-aat-map.hh"

/*
 * morx -- Extended Glyph Metamorphosis
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6morx.html
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6mort.html
 */
#define HB_AAT_TAG_morx HB_TAG('m','o','r','x')
#define HB_AAT_TAG_mort HB_TAG('m','o','r','t')


ನಾಮಾಕಾಶ AAT {

using ನಾಮಾಕಾಶ OT;

template <typename Types>
ಸಂರಚನಾ RearrangementSubtable
{
  ಪ್ರಕಾರನಿರ್ದೇಶ typename Types::HBUINT HBUINT;

  ಪ್ರಕಾರನಿರ್ದೇಶ ಶೂನ್ಯ EntryData;

  ಗಣನಾ Flags
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

  ತಾರ್ಕಿಕ is_action_initiable (ಧ್ರುವ Entry<EntryData> &entry) ಧ್ರುವ
  {
    ಪ್ರತ್ಯಾಗಮ (entry.flags & MarkFirst);
  }
  ತಾರ್ಕಿಕ is_actionable (ಧ್ರುವ Entry<EntryData> &entry) ಧ್ರುವ
  {
    ಪ್ರತ್ಯಾಗಮ (entry.flags & Verb);
  }

  ಸಂರಚನಾ driver_context_t
  {
    ಸ್ಥಿರ constexpr ತಾರ್ಕಿಕ in_place = ಸತ್ಯ;

    driver_context_t (ಧ್ರುವ RearrangementSubtable *table_) :
	ret (ಅಸತ್ಯ),
	table (table_),
	start (0), end (0) {}

    HB_AAT_TRANSITION_INLINE
    ಶೂನ್ಯ transition (hb_buffer_t *buffer,
		     StateTableDriver<Types, EntryData, Flags> *driver,
		     ಧ್ರುವ Entry<EntryData> &entry)
    {
      unsigned ಪೂರ್ಣಾಙ್ಕ flags = entry.flags;

      ಯದಿ (flags & MarkFirst)
	start = buffer->idx;

      ಯದಿ (flags & MarkLast)
	end = hb_min (buffer->idx + 1, buffer->len);

      ಯದಿ ((flags & Verb) && start < end)
      {
	/* The following map has two nibbles, for start-side
	 * and end-side. Values of 0,1,2 mean move that many
	 * to the other side. Value of 3 means move 2 and
	 * flip them. */
	ಧ್ರುವ unsigned ವರ್ಣ map[16] =
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

	unsigned ಪೂರ್ಣಾಙ್ಕ m = map[flags & Verb];
	unsigned ಪೂರ್ಣಾಙ್ಕ l = hb_min (2u, m >> 4);
	unsigned ಪೂರ್ಣಾಙ್ಕ r = hb_min (2u, m & 0x0F);
	ತಾರ್ಕಿಕ reverse_l = 3 == (m >> 4);
	ತಾರ್ಕಿಕ reverse_r = 3 == (m & 0x0F);

	ಯದಿ (end - start >= l + r && end-start <= HB_MAX_CONTEXT_LENGTH)
	{
	  buffer->merge_clusters (start, hb_min (buffer->idx + 1, buffer->len));
	  buffer->merge_clusters (start, end);

	  hb_glyph_info_t *info = buffer->info;
	  hb_glyph_info_t buf[4];

	  hb_memcpy (buf, info + start, l * ಪರಿಮಾಣ (buf[0]));
	  hb_memcpy (buf + 2, info + end - r, r * ಪರಿಮಾಣ (buf[0]));

	  ಯದಿ (l != r)
	    memmove (info + start + r, info + start + l, (end - start - l - r) * ಪರಿಮಾಣ (buf[0]));

	  hb_memcpy (info + start, buf + 2, r * ಪರಿಮಾಣ (buf[0]));
	  hb_memcpy (info + end - l, buf, l * ಪರಿಮಾಣ (buf[0]));
	  ಯದಿ (reverse_l)
	  {
	    buf[0] = info[end - 1];
	    info[end - 1] = info[end - 2];
	    info[end - 2] = buf[0];
	  }
	  ಯದಿ (reverse_r)
	  {
	    buf[0] = info[start];
	    info[start] = info[start + 1];
	    info[start + 1] = buf[0];
	  }
	}
      }
    }

    ಸಾರ್ವಜನಿಕ:
    ತಾರ್ಕಿಕ ret;
    ಧ್ರುವ RearrangementSubtable *table;
    ಗುಪ್ತ:
    unsigned ಪೂರ್ಣಾಙ್ಕ start;
    unsigned ಪೂರ್ಣಾಙ್ಕ end;
  };

  ತಾರ್ಕಿಕ apply (hb_aat_apply_context_t *c) ಧ್ರುವ
  {
    TRACE_APPLY (ಏತತ್);

    driver_context_t dc (ಏತತ್);

    StateTableDriver<Types, EntryData, Flags> driver (machine, c->face);

    driver.drive (&dc, c);

    return_trace (dc.ret);
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (machine.sanitize (c));
  }

  ಸಾರ್ವಜನಿಕ:
  StateTable<Types, EntryData>	machine;
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC ((StateTable<Types, EntryData>::static_size));
};

template <typename Types>
ಸಂರಚನಾ ContextualSubtable
{
  ಪ್ರಕಾರನಿರ್ದೇಶ typename Types::HBUINT HBUINT;

  ಸಂರಚನಾ EntryData
  {
    ಪ್ರಕಾರನಿರ್ದೇಶ typename std::conditional<Types::extended, HBUINT16, HBINT16>::type OffsetType;

    OffsetType	markIndex;	/* Index/offset of the substitution table for the
				 * marked glyph. */
    OffsetType	currentIndex;	/* Index/offset of the substitution table for the
				 * current glyph. */
    ಸಾರ್ವಜನಿಕ:
    DEFINE_SIZE_STATIC (4);
  };

  ಗಣನಾ Flags
  {
    SetMark		= 0x8000,	/* If set, make the current glyph the marked glyph. */
    DontAdvance		= 0x4000,	/* If set, don't advance to the next glyph before
					 * going to the new state. */
    Reserved		= 0x3FFF,	/* These bits are reserved and should be set to 0. */
  };

  ತಾರ್ಕಿಕ is_action_initiable (ಧ್ರುವ Entry<EntryData> &entry) ಧ್ರುವ
  {
    ಪ್ರತ್ಯಾಗಮ (entry.flags & SetMark);
  }
  ತಾರ್ಕಿಕ is_actionable (ಧ್ರುವ Entry<EntryData> &entry) ಧ್ರುವ
  {
    ಯದಿ (Types::extended)
      ಪ್ರತ್ಯಾಗಮ entry.data.markIndex != 0xFFFF || entry.data.currentIndex != 0xFFFF;
    ಅನ್ಯಥಾ
      ಪ್ರತ್ಯಾಗಮ entry.data.markIndex || entry.data.currentIndex;
  }

  ಸಂರಚನಾ driver_context_t
  {
    ಸ್ಥಿರ constexpr ತಾರ್ಕಿಕ in_place = ಸತ್ಯ;

    driver_context_t (ಧ್ರುವ ContextualSubtable *table_,
			     hb_aat_apply_context_t *c_) :
	ret (ಅಸತ್ಯ),
	c (c_),
	table (table_),
	mark_set (ಅಸತ್ಯ),
	mark (0),
	subs (table+table->substitutionTables) {}

    HB_AAT_TRANSITION_INLINE
    ಶೂನ್ಯ transition (hb_buffer_t *buffer,
		     StateTableDriver<Types, EntryData, Flags> *driver,
		     ಧ್ರುವ Entry<EntryData> &entry)
    {
      /* Looks like CoreText applies neither mark nor current substitution for
       * end-of-text if mark was not explicitly set. */
      ಯದಿ (buffer->idx == buffer->len && !mark_set)
	ಪ್ರತ್ಯಾಗಮ;

      ಧ್ರುವ HBGlyphID16 *replacement;

      replacement = nullptr;
      ಯದಿ (Types::extended)
      {
	ಯದಿ (entry.data.markIndex != 0xFFFF)
	{
	  ಧ್ರುವ Lookup<HBGlyphID16> &lookup = subs[entry.data.markIndex];
	  replacement = lookup.get_ಮೌಲ್ಯ (buffer->info[mark].codepoint, driver->ಅಂಕೆ_glyphs);
	}
      }
      ಅನ್ಯಥಾ
      {
	ಯದಿ (entry.data.markIndex)
	{
	  ಪೂರ್ಣಾಙ್ಕ offset = (ಪೂರ್ಣಾಙ್ಕ) entry.data.markIndex + buffer->info[mark].codepoint;
	  ಧ್ರುವ UnsizedArrayOf<HBGlyphID16> &subs_old = (ಧ್ರುವ UnsizedArrayOf<HBGlyphID16> &) subs;
	  replacement = &subs_old[Types::wordOffsetToIndex (offset, table, subs_old.arrayZ)];
	  ಯದಿ (!(replacement->sanitize (&c->sanitizer) &&
		hb_barrier () &&
		*replacement))
	    replacement = nullptr;
	}
      }
      ಯದಿ (replacement)
      {
	buffer->unsafe_to_break (mark, hb_min (buffer->idx + 1, buffer->len));
	c->replace_glyph_inplace (mark, *replacement);
	ret = ಸತ್ಯ;
      }

      replacement = nullptr;
      unsigned ಪೂರ್ಣಾಙ್ಕ idx = hb_min (buffer->idx, buffer->len - 1);
      ಯದಿ (Types::extended)
      {
	ಯದಿ (entry.data.currentIndex != 0xFFFF)
	{
	  ಧ್ರುವ Lookup<HBGlyphID16> &lookup = subs[entry.data.currentIndex];
	  replacement = lookup.get_ಮೌಲ್ಯ (buffer->info[idx].codepoint, driver->ಅಂಕೆ_glyphs);
	}
      }
      ಅನ್ಯಥಾ
      {
	ಯದಿ (entry.data.currentIndex)
	{
	  ಪೂರ್ಣಾಙ್ಕ offset = (ಪೂರ್ಣಾಙ್ಕ) entry.data.currentIndex + buffer->info[idx].codepoint;
	  ಧ್ರುವ UnsizedArrayOf<HBGlyphID16> &subs_old = (ಧ್ರುವ UnsizedArrayOf<HBGlyphID16> &) subs;
	  replacement = &subs_old[Types::wordOffsetToIndex (offset, table, subs_old.arrayZ)];
	  ಯದಿ (!(replacement->sanitize (&c->sanitizer) &&
		hb_barrier () &&
		*replacement))
	    replacement = nullptr;
	}
      }
      ಯದಿ (replacement)
      {
	c->replace_glyph_inplace (idx, *replacement);
	ret = ಸತ್ಯ;
      }

      ಯದಿ (entry.flags & SetMark)
      {
	mark_set = ಸತ್ಯ;
	mark = buffer->idx;
      }
    }

    ಸಾರ್ವಜನಿಕ:
    ತಾರ್ಕಿಕ ret;
    hb_aat_apply_context_t *c;
    ಧ್ರುವ ContextualSubtable *table;
    ಗುಪ್ತ:
    ತಾರ್ಕಿಕ mark_set;
    unsigned ಪೂರ್ಣಾಙ್ಕ mark;
    ಧ್ರುವ UnsizedListOfOffset16To<Lookup<HBGlyphID16>, HBUINT, ಶೂನ್ಯ, ಅಸತ್ಯ> &subs;
  };

  ತಾರ್ಕಿಕ apply (hb_aat_apply_context_t *c) ಧ್ರುವ
  {
    TRACE_APPLY (ಏತತ್);

    driver_context_t dc (ಏತತ್, c);

    StateTableDriver<Types, EntryData, Flags> driver (machine, c->face);

    driver.drive (&dc, c);

    return_trace (dc.ret);
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);

    unsigned ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_entries = 0;
    ಯದಿ (unlikely (!machine.sanitize (c, &ಅಂಕೆ_entries))) return_trace (ಅಸತ್ಯ);
    hb_barrier ();

    ಯದಿ (!Types::extended)
      return_trace (substitutionTables.sanitize (c, ಏತತ್, 0));

    unsigned ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_lookups = 0;

    ಧ್ರುವ Entry<EntryData> *entries = machine.get_entries ();
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಅಂಕೆ_entries; i++)
    {
      ಧ್ರುವ EntryData &data = entries[i].data;

      ಯದಿ (data.markIndex != 0xFFFF)
	ಅಂಕೆ_lookups = hb_max (ಅಂಕೆ_lookups, 1u + data.markIndex);
      ಯದಿ (data.currentIndex != 0xFFFF)
	ಅಂಕೆ_lookups = hb_max (ಅಂಕೆ_lookups, 1u + data.currentIndex);
    }

    return_trace (substitutionTables.sanitize (c, ಏತತ್, ಅಂಕೆ_lookups));
  }

  ಸಾರ್ವಜನಿಕ:
  StateTable<Types, EntryData>
		machine;
  ರಕ್ಷಿತ:
  NNOffsetTo<UnsizedListOfOffset16To<Lookup<HBGlyphID16>, HBUINT, ಶೂನ್ಯ, ಅಸತ್ಯ>, HBUINT>
		substitutionTables;
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC ((StateTable<Types, EntryData>::static_size + HBUINT::static_size));
};


template <ತಾರ್ಕಿಕ extended>
ಸಂರಚನಾ LigatureEntry;

template <>
ಸಂರಚನಾ LigatureEntry<ಸತ್ಯ>
{

  ಸಂರಚನಾ EntryData
  {
    HBUINT16	ligActionIndex;	/* Index to the first ligActionTable entry
				 * for processing this group, if indicated
				 * by the flags. */
    ಸಾರ್ವಜನಿಕ:
    DEFINE_SIZE_STATIC (2);
  };

  ಗಣನಾ Flags
  {
    SetComponent	= 0x8000,	/* Push this glyph onto the component stack for
					 * eventual processing. */
    DontAdvance		= 0x4000,	/* Leave the glyph pointer at this glyph for the
					   next iteration. */
    PerformAction	= 0x2000,	/* Use the ligActionIndex to process a ligature
					 * group. */
    Reserved		= 0x1FFF,	/* These bits are reserved and should be set to 0. */
  };

  ಸ್ಥಿರ ತಾರ್ಕಿಕ initiateAction (ಧ್ರುವ Entry<EntryData> &entry)
  { ಪ್ರತ್ಯಾಗಮ entry.flags & SetComponent; }

  ಸ್ಥಿರ ತಾರ್ಕಿಕ performAction (ಧ್ರುವ Entry<EntryData> &entry)
  { ಪ್ರತ್ಯಾಗಮ entry.flags & PerformAction; }

  ಸ್ಥಿರ unsigned ಪೂರ್ಣಾಙ್ಕ ligActionIndex (ಧ್ರುವ Entry<EntryData> &entry)
  { ಪ್ರತ್ಯಾಗಮ entry.data.ligActionIndex; }
};
template <>
ಸಂರಚನಾ LigatureEntry<ಅಸತ್ಯ>
{
  ಪ್ರಕಾರನಿರ್ದೇಶ ಶೂನ್ಯ EntryData;

  ಗಣನಾ Flags
  {
    SetComponent	= 0x8000,	/* Push this glyph onto the component stack for
					 * eventual processing. */
    DontAdvance		= 0x4000,	/* Leave the glyph pointer at this glyph for the
					   next iteration. */
    Offset		= 0x3FFF,	/* Byte offset from beginning of subtable to the
					 * ligature action list. This value must be a
					 * multiple of 4. */
  };

  ಸ್ಥಿರ ತಾರ್ಕಿಕ initiateAction (ಧ್ರುವ Entry<EntryData> &entry)
  { ಪ್ರತ್ಯಾಗಮ entry.flags & SetComponent; }

  ಸ್ಥಿರ ತಾರ್ಕಿಕ performAction (ಧ್ರುವ Entry<EntryData> &entry)
  { ಪ್ರತ್ಯಾಗಮ entry.flags & Offset; }

  ಸ್ಥಿರ unsigned ಪೂರ್ಣಾಙ್ಕ ligActionIndex (ಧ್ರುವ Entry<EntryData> &entry)
  { ಪ್ರತ್ಯಾಗಮ entry.flags & Offset; }
};


template <typename Types>
ಸಂರಚನಾ LigatureSubtable
{
  ಪ್ರಕಾರನಿರ್ದೇಶ typename Types::HBUINT HBUINT;

  ಪ್ರಕಾರನಿರ್ದೇಶ LigatureEntry<Types::extended> LigatureEntryT;
  ಪ್ರಕಾರನಿರ್ದೇಶ typename LigatureEntryT::EntryData EntryData;

  ಗಣನಾ Flags
  {
    DontAdvance	= LigatureEntryT::DontAdvance,
  };

  ತಾರ್ಕಿಕ is_action_initiable (ಧ್ರುವ Entry<EntryData> &entry) ಧ್ರುವ
  {
    ಪ್ರತ್ಯಾಗಮ LigatureEntryT::initiateAction (entry);
  }
  ತಾರ್ಕಿಕ is_actionable (ಧ್ರುವ Entry<EntryData> &entry) ಧ್ರುವ
  {
    ಪ್ರತ್ಯಾಗಮ LigatureEntryT::performAction (entry);
  }

  ಸಂರಚನಾ driver_context_t
  {
    ಸ್ಥಿರ constexpr ತಾರ್ಕಿಕ in_place = ಅಸತ್ಯ;
    ಗಣನಾ LigActionFlags
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

    driver_context_t (ಧ್ರುವ LigatureSubtable *table_,
		      hb_aat_apply_context_t *c_) :
	ret (ಅಸತ್ಯ),
	c (c_),
	table (table_),
	ligAction (table+table->ligAction),
	component (table+table->component),
	ligature (table+table->ligature),
	match_length (0) {}

    HB_AAT_TRANSITION_INLINE
    ಶೂನ್ಯ transition (hb_buffer_t *buffer,
		     StateTableDriver<Types, EntryData, Flags> *driver,
		     ಧ್ರುವ Entry<EntryData> &entry)
    {
      DEBUG_ಸಂದೇಶ (APPLY, nullptr, "Ligature transition at %u", buffer->idx);
      ಯದಿ (entry.flags & LigatureEntryT::SetComponent)
      {
	/* Never mark same index twice, in case DontAdvance was used... */
	ಯದಿ (unlikely (match_length && match_positions[(match_length - 1u) % ARRAY_LENGTH (match_positions)] == buffer->out_len))
	  match_length--;

	match_positions[match_length++ % ARRAY_LENGTH (match_positions)] = buffer->out_len;
	DEBUG_ಸಂದೇಶ (APPLY, nullptr, "Set component at %u", buffer->out_len);
      }

      ಯದಿ (LigatureEntryT::performAction (entry))
      {
	DEBUG_ಸಂದೇಶ (APPLY, nullptr, "Perform action with %u", match_length);
	unsigned ಪೂರ್ಣಾಙ್ಕ end = buffer->out_len;

	ಯದಿ (unlikely (!match_length))
	  ಪ್ರತ್ಯಾಗಮ;

	ಯದಿ (buffer->idx >= buffer->len)
	  ಪ್ರತ್ಯಾಗಮ; /* TODO Work on previous instead? */

	unsigned ಪೂರ್ಣಾಙ್ಕ cursor = match_length;

	unsigned ಪೂರ್ಣಾಙ್ಕ action_idx = LigatureEntryT::ligActionIndex (entry);
	action_idx = Types::offsetToIndex (action_idx, table, ligAction.arrayZ);
	ಧ್ರುವ HBUINT32 *actionData = &ligAction[action_idx];

	unsigned ಪೂರ್ಣಾಙ್ಕ ligature_idx = 0;
	unsigned ಪೂರ್ಣಾಙ್ಕ action;
	ಕುರು
	{
	  ಯದಿ (unlikely (!cursor))
	  {
	    /* Stack underflow.  Clear the stack. */
	    DEBUG_ಸಂದೇಶ (APPLY, nullptr, "Stack underflow");
	    match_length = 0;
	    ಭಙ್ಗ;
	  }

	  DEBUG_ಸಂದೇಶ (APPLY, nullptr, "Moving to stack position %u", cursor - 1);
	  ಯದಿ (unlikely (!buffer->move_to (match_positions[--cursor % ARRAY_LENGTH (match_positions)]))) ಪ್ರತ್ಯಾಗಮ;

	  ಯದಿ (unlikely (!actionData->sanitize (&c->sanitizer))) ಭಙ್ಗ;
	  hb_barrier ();
	  action = *actionData;

	  uint32_t uoffset = action & LigActionOffset;
	  ಯದಿ (uoffset & 0x20000000)
	    uoffset |= 0xC0000000; /* Sign-extend. */
	  int32_t offset = (int32_t) uoffset;
	  unsigned ಪೂರ್ಣಾಙ್ಕ component_idx = buffer->cur().codepoint + offset;
	  component_idx = Types::wordOffsetToIndex (component_idx, table, component.arrayZ);
	  ಧ್ರುವ HBUINT16 &componentData = component[component_idx];
	  ಯದಿ (unlikely (!componentData.sanitize (&c->sanitizer))) ಭಙ್ಗ;
	  hb_barrier ();
	  ligature_idx += componentData;

	  DEBUG_ಸಂದೇಶ (APPLY, nullptr, "Action store %d last %d",
		     ತಾರ್ಕಿಕ (action & LigActionStore),
		     ತಾರ್ಕಿಕ (action & LigActionLast));
	  ಯದಿ (action & (LigActionStore | LigActionLast))
	  {
	    ligature_idx = Types::offsetToIndex (ligature_idx, table, ligature.arrayZ);
	    ಧ್ರುವ HBGlyphID16 &ligatureData = ligature[ligature_idx];
	    ಯದಿ (unlikely (!ligatureData.sanitize (&c->sanitizer))) ಭಙ್ಗ;
	    hb_barrier ();
	    hb_codepoint_t lig = ligatureData;

	    DEBUG_ಸಂದೇಶ (APPLY, nullptr, "Produced ligature %u", lig);
	    ಯದಿ (unlikely (!c->replace_glyph (lig))) ಪ್ರತ್ಯಾಗಮ;

	    unsigned ಪೂರ್ಣಾಙ್ಕ lig_end = match_positions[(match_length - 1u) % ARRAY_LENGTH (match_positions)] + 1u;
	    /* Now go and delete all subsequent components. */
	    ಯಾವತ್ (match_length - 1u > cursor)
	    {
	      DEBUG_ಸಂದೇಶ (APPLY, nullptr, "Skipping ligature component");
	      ಯದಿ (unlikely (!buffer->move_to (match_positions[--match_length % ARRAY_LENGTH (match_positions)]))) ಪ್ರತ್ಯಾಗಮ;
	      ಯದಿ (!c->delete_glyph ()) ಪ್ರತ್ಯಾಗಮ;
	    }

	    ಯದಿ (unlikely (!buffer->move_to (lig_end))) ಪ್ರತ್ಯಾಗಮ;
	    buffer->merge_out_clusters (match_positions[cursor % ARRAY_LENGTH (match_positions)], buffer->out_len);
	  }

	  actionData++;
	}
	ಯಾವತ್ (!(action & LigActionLast));
	ಯದಿ (unlikely (!buffer->move_to (end))) ಪ್ರತ್ಯಾಗಮ;
      }
    }

    ಸಾರ್ವಜನಿಕ:
    ತಾರ್ಕಿಕ ret;
    hb_aat_apply_context_t *c;
    ಧ್ರುವ LigatureSubtable *table;
    ಗುಪ್ತ:
    ಧ್ರುವ UnsizedArrayOf<HBUINT32> &ligAction;
    ಧ್ರುವ UnsizedArrayOf<HBUINT16> &component;
    ಧ್ರುವ UnsizedArrayOf<HBGlyphID16> &ligature;
    unsigned ಪೂರ್ಣಾಙ್ಕ match_length;
    unsigned ಪೂರ್ಣಾಙ್ಕ match_positions[HB_MAX_CONTEXT_LENGTH];
  };

  ತಾರ್ಕಿಕ apply (hb_aat_apply_context_t *c) ಧ್ರುವ
  {
    TRACE_APPLY (ಏತತ್);

    driver_context_t dc (ಏತತ್, c);

    StateTableDriver<Types, EntryData, Flags> driver (machine, c->face);

    driver.drive (&dc, c);

    return_trace (dc.ret);
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    /* The rest of array sanitizations are done at run-time. */
    return_trace (c->check_struct (ಏತತ್) && machine.sanitize (c) &&
		  hb_barrier () &&
		  ligAction && component && ligature);
  }

  ಸಾರ್ವಜನಿಕ:
  StateTable<Types, EntryData>
		machine;
  ರಕ್ಷಿತ:
  NNOffsetTo<UnsizedArrayOf<HBUINT32>, HBUINT>
		ligAction;	/* Offset to the ligature action table. */
  NNOffsetTo<UnsizedArrayOf<HBUINT16>, HBUINT>
		component;	/* Offset to the component table. */
  NNOffsetTo<UnsizedArrayOf<HBGlyphID16>, HBUINT>
		ligature;	/* Offset to the actual ligature lists. */
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC ((StateTable<Types, EntryData>::static_size + 3 * HBUINT::static_size));
};

template <typename Types>
ಸಂರಚನಾ NoncontextualSubtable
{
  ತಾರ್ಕಿಕ apply (hb_aat_apply_context_t *c) ಧ್ರುವ
  {
    TRACE_APPLY (ಏತತ್);

    ತಾರ್ಕಿಕ ret = ಅಸತ್ಯ;
    unsigned ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_glyphs = c->face->get_ಅಂಕೆ_glyphs ();

    hb_glyph_info_t *info = c->buffer->info;
    unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = c->buffer->len;
    // If there's only one range, we already checked the flag.
    auto *last_range = c->range_flags && (c->range_flags->length > 1) ? &(*c->range_flags)[0] : nullptr;
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಎಣಿಕೆ; i++)
    {
      /* This block copied from StateTableDriver::drive. Keep in sync. */
      ಯದಿ (unlikely (last_range))
      {
	auto *range = last_range;
	{
	  unsigned cluster = info[i].cluster;
	  ಯಾವತ್ (cluster < range->cluster_first)
	    range--;
	  ಯಾವತ್ (cluster > range->cluster_last)
	    range++;

	  last_range = range;
	}
	ಯದಿ (!(range->flags & c->subtable_flags))
	  ಅನುವರ್ತ;
      }

      ಧ್ರುವ HBGlyphID16 *replacement = substitute.get_ಮೌಲ್ಯ (info[i].codepoint, ಅಂಕೆ_glyphs);
      ಯದಿ (replacement)
      {
	c->replace_glyph_inplace (i, *replacement);
	ret = ಸತ್ಯ;
      }
    }

    return_trace (ret);
  }

  template <typename set_t>
  ಶೂನ್ಯ collect_initial_glyphs (set_t &glyphs, unsigned ಅಂಕೆ_glyphs) ಧ್ರುವ
  {
    substitute.collect_glyphs (glyphs, ಅಂಕೆ_glyphs);
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (substitute.sanitize (c));
  }

  ರಕ್ಷಿತ:
  Lookup<HBGlyphID16>	substitute;
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_MIN (2);
};

template <typename Types>
ಸಂರಚನಾ InsertionSubtable
{
  ಪ್ರಕಾರನಿರ್ದೇಶ typename Types::HBUINT HBUINT;

  ಸಂರಚನಾ EntryData
  {
    HBUINT16	current_ಸೇರಿಸು_Index;	/* Zero-based index into the insertion glyph table.
					 * The number of glyphs to be inserted is contained
					 * in the currentInsertCount field in the flags.
					 * A value of 0xFFFF indicates no insertion is to
					 * be done. */
    HBUINT16	marked_ಸೇರಿಸು_Index;	/* Zero-based index into the insertion glyph table.
					 * The number of glyphs to be inserted is contained
					 * in the markedInsertCount field in the flags.
					 * A value of 0xFFFF indicates no insertion is to
					 * be done. */
    ಸಾರ್ವಜನಿಕ:
    DEFINE_SIZE_STATIC (4);
  };

  ಗಣನಾ Flags
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
    Current_ಸೇರಿಸು_Before= 0x0800,      /* If set, specifies that insertions are to be made
				       * to the left of the current glyph. If clear,
				       * they're made to the right of the current glyph. */
    Marked_ಸೇರಿಸು_Before= 0x0400,	      /* If set, specifies that insertions are to be
				       * made to the left of the marked glyph. If clear,
				       * they're made to the right of the marked glyph. */
    Current_ಸೇರಿಸು_ಎಣಿಕೆ= 0x3E0,	      /* This 5-bit field is treated as a count of the
				       * number of glyphs to insert at the current
				       * position. Since zero means no insertions, the
				       * largest number of insertions at any given
				       * current location is 31 glyphs. */
    Marked_ಸೇರಿಸು_ಎಣಿಕೆ= 0x001F,	      /* This 5-bit field is treated as a count of the
				       * number of glyphs to insert at the marked
				       * position. Since zero means no insertions, the
				       * largest number of insertions at any given
				       * marked location is 31 glyphs. */
  };

  ತಾರ್ಕಿಕ is_action_initiable (ಧ್ರುವ Entry<EntryData> &entry) ಧ್ರುವ
  {
    ಪ್ರತ್ಯಾಗಮ (entry.flags & SetMark);
  }
  ತಾರ್ಕಿಕ is_actionable (ಧ್ರುವ Entry<EntryData> &entry) ಧ್ರುವ
  {
    ಪ್ರತ್ಯಾಗಮ (entry.flags & (Current_ಸೇರಿಸು_ಎಣಿಕೆ | Marked_ಸೇರಿಸು_ಎಣಿಕೆ)) &&
	   (entry.data.current_ಸೇರಿಸು_Index != 0xFFFF ||entry.data.marked_ಸೇರಿಸು_Index != 0xFFFF);
  }

  ಸಂರಚನಾ driver_context_t
  {
    ಸ್ಥಿರ constexpr ತಾರ್ಕಿಕ in_place = ಅಸತ್ಯ;

    driver_context_t (ಧ್ರುವ InsertionSubtable *table_,
		      hb_aat_apply_context_t *c_) :
	ret (ಅಸತ್ಯ),
	c (c_),
	table (table_),
	mark (0),
	insertionAction (table+table->insertionAction) {}

    HB_AAT_TRANSITION_INLINE
    ಶೂನ್ಯ transition (hb_buffer_t *buffer,
		     StateTableDriver<Types, EntryData, Flags> *driver,
		     ಧ್ರುವ Entry<EntryData> &entry)
    {
      unsigned ಪೂರ್ಣಾಙ್ಕ flags = entry.flags;

      unsigned mark_loc = buffer->out_len;

      ಯದಿ (entry.data.marked_ಸೇರಿಸು_Index != 0xFFFF)
      {
	unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = (flags & Marked_ಸೇರಿಸು_ಎಣಿಕೆ);
	ಯದಿ (unlikely ((buffer->max_ops -= ಎಣಿಕೆ) <= 0)) ಪ್ರತ್ಯಾಗಮ;
	unsigned ಪೂರ್ಣಾಙ್ಕ start = entry.data.marked_ಸೇರಿಸು_Index;
	ಧ್ರುವ HBGlyphID16 *glyphs = &insertionAction[start];
	ಯದಿ (unlikely (!c->sanitizer.check_array (glyphs, ಎಣಿಕೆ))) ಎಣಿಕೆ = 0;
	hb_barrier ();

	ತಾರ್ಕಿಕ before = flags & Marked_ಸೇರಿಸು_Before;

	unsigned ಪೂರ್ಣಾಙ್ಕ end = buffer->out_len;
	ಯದಿ (unlikely (!buffer->move_to (mark))) ಪ್ರತ್ಯಾಗಮ;

	ಯದಿ (buffer->idx < buffer->len && !before)
	  ಯದಿ (unlikely (!buffer->copy_glyph ())) ಪ್ರತ್ಯಾಗಮ;
	/* TODO We ignore KashidaLike setting. */
	ಯದಿ (unlikely (!c->output_glyphs (ಎಣಿಕೆ, glyphs))) ಪ್ರತ್ಯಾಗಮ;
	ret = ಸತ್ಯ;
	ಯದಿ (buffer->idx < buffer->len && !before)
	  buffer->skip_glyph ();

	ಯದಿ (unlikely (!buffer->move_to (end + ಎಣಿಕೆ))) ಪ್ರತ್ಯಾಗಮ;

	buffer->unsafe_to_break_from_outbuffer (mark, hb_min (buffer->idx + 1, buffer->len));
      }

      ಯದಿ (flags & SetMark)
	mark = mark_loc;

      ಯದಿ (entry.data.current_ಸೇರಿಸು_Index != 0xFFFF)
      {
	unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = (flags & Current_ಸೇರಿಸು_ಎಣಿಕೆ) >> 5;
	ಯದಿ (unlikely ((buffer->max_ops -= ಎಣಿಕೆ) <= 0)) ಪ್ರತ್ಯಾಗಮ;
	unsigned ಪೂರ್ಣಾಙ್ಕ start = entry.data.current_ಸೇರಿಸು_Index;
	ಧ್ರುವ HBGlyphID16 *glyphs = &insertionAction[start];
	ಯದಿ (unlikely (!c->sanitizer.check_array (glyphs, ಎಣಿಕೆ))) ಎಣಿಕೆ = 0;
	hb_barrier ();

	ತಾರ್ಕಿಕ before = flags & Current_ಸೇರಿಸು_Before;

	unsigned ಪೂರ್ಣಾಙ್ಕ end = buffer->out_len;

	ಯದಿ (buffer->idx < buffer->len && !before)
	  ಯದಿ (unlikely (!buffer->copy_glyph ())) ಪ್ರತ್ಯಾಗಮ;
	/* TODO We ignore KashidaLike setting. */
	ಯದಿ (unlikely (!c->output_glyphs (ಎಣಿಕೆ, glyphs))) ಪ್ರತ್ಯಾಗಮ;
	ret = ಸತ್ಯ;
	ಯದಿ (buffer->idx < buffer->len && !before)
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
	ಯದಿ (unlikely (!buffer->move_to ((flags & DontAdvance) ? end : end + ಎಣಿಕೆ))) ಪ್ರತ್ಯಾಗಮ;
      }
    }

    ಸಾರ್ವಜನಿಕ:
    ತಾರ್ಕಿಕ ret;
    hb_aat_apply_context_t *c;
    ಧ್ರುವ InsertionSubtable *table;
    ಗುಪ್ತ:
    unsigned ಪೂರ್ಣಾಙ್ಕ mark;
    ಧ್ರುವ UnsizedArrayOf<HBGlyphID16> &insertionAction;
  };

  ತಾರ್ಕಿಕ apply (hb_aat_apply_context_t *c) ಧ್ರುವ
  {
    TRACE_APPLY (ಏತತ್);

    driver_context_t dc (ಏತತ್, c);

    StateTableDriver<Types, EntryData, Flags> driver (machine, c->face);

    driver.drive (&dc, c);

    return_trace (dc.ret);
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    /* The rest of array sanitizations are done at run-time. */
    return_trace (c->check_struct (ಏತತ್) && machine.sanitize (c) &&
		  hb_barrier () &&
		  insertionAction);
  }

  ಸಾರ್ವಜನಿಕ:
  StateTable<Types, EntryData>
		machine;
  ರಕ್ಷಿತ:
  NNOffsetTo<UnsizedArrayOf<HBGlyphID16>, HBUINT>
		insertionAction;	/* Byte offset from stateHeader to the start of
					 * the insertion glyph table. */
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC ((StateTable<Types, EntryData>::static_size + HBUINT::static_size));
};


ಸಂರಚನಾ Feature
{
  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್));
  }

  ಸಾರ್ವಜನಿಕ:
  HBUINT16	featureType;	/* The type of feature. */
  HBUINT16	featureSetting;	/* The feature's setting (aka selector). */
  HBUINT32	enableFlags;	/* Flags for the settings that this feature
				 * and setting enables. */
  HBUINT32	disableFlags;	/* Complement of flags for the settings that this
				 * feature and setting disable. */

  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (12);
};


ಸಂರಚನಾ hb_accelerate_subtables_context_t :
       hb_dispatch_context_t<hb_accelerate_subtables_context_t>
{
  ಸಂರಚನಾ hb_applicable_t
  {
    friend ಸಂರಚನಾ hb_accelerate_subtables_context_t;
    friend ಸಂರಚನಾ hb_aat_layout_lookup_accelerator_t;

    ಸಾರ್ವಜನಿಕ:
    hb_bit_set_t glyph_set;
    mutable hb_aat_class_cache_t class_cache;

    template <typename T>
    auto init_ (ಧ್ರುವ T &obj_, unsigned ಅಂಕೆ_glyphs, hb_priority<1>) HB_AUTO_RETURN
    (
      obj_.machine.collect_initial_glyphs (glyph_set, ಅಂಕೆ_glyphs, obj_)
    )

    template <typename T>
    ಶೂನ್ಯ init_ (ಧ್ರುವ T &obj_, unsigned ಅಂಕೆ_glyphs, hb_priority<0>)
    {
      obj_.collect_initial_glyphs (glyph_set, ಅಂಕೆ_glyphs);
    }

    template <typename T>
    ಶೂನ್ಯ init (ಧ್ರುವ T &obj_, unsigned ಅಂಕೆ_glyphs)
    {
      glyph_set.init ();
      init_ (obj_, ಅಂಕೆ_glyphs, hb_prioritize);
      class_cache.clear ();
    }

    ಶೂನ್ಯ
    fini ()
    {
      glyph_set.fini ();
    }
  };

  /* Dispatch interface. */
  template <typename T>
  return_t dispatch (ಧ್ರುವ T &obj)
  {
    hb_applicable_t *entry = &array[i++];

    entry->init (obj, ಅಂಕೆ_glyphs);

    ಪ್ರತ್ಯಾಗಮ hb_empty_t ();
  }
  ಸ್ಥಿರ return_t default_return_ಮೌಲ್ಯ () { ಪ್ರತ್ಯಾಗಮ hb_empty_t (); }

  ತಾರ್ಕಿಕ stop_sublookup_iteration (return_t r) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ; }

  hb_accelerate_subtables_context_t (hb_applicable_t *array_, unsigned ಅಂಕೆ_glyphs_2) :
				     hb_dispatch_context_t<hb_accelerate_subtables_context_t> (),
				     array (array_), ಅಂಕೆ_glyphs (ಅಂಕೆ_glyphs_2) {}

  hb_applicable_t *array;
  unsigned ಅಂಕೆ_glyphs;
  unsigned i = 0;
};

ಸಂರಚನಾ hb_aat_layout_chain_accelerator_t
{
  template <typename TChain>
  ಸ್ಥಿರ hb_aat_layout_chain_accelerator_t *create (ಧ್ರುವ TChain &chain, unsigned ಅಂಕೆ_glyphs)
  {
    unsigned ಎಣಿಕೆ = chain.get_subtable_ಎಣಿಕೆ ();

    unsigned product;
    ಯದಿ (unlikely (hb_unsigned_mul_overflows (ಎಣಿಕೆ,
        ಪರಿಮಾಣ (hb_accelerate_subtables_context_t::hb_applicable_t), &product)))
      ಪ್ರತ್ಯಾಗಮ nullptr;

    unsigned size;
    ಯದಿ (unlikely (hb_unsigned_add_overflows (ಪರಿಮಾಣ (hb_aat_layout_chain_accelerator_t) -
        HB_VAR_ARRAY * ಪರಿಮಾಣ (hb_accelerate_subtables_context_t::hb_applicable_t), product, &size))) {
      ಪ್ರತ್ಯಾಗಮ nullptr;
    }

    /* The following is a calloc because when we are collecting subtables,
     * some of them might be invalid and hence not collect; as a result,
     * we might not fill in all the count entries of the subtables array.
     * Zeroing it allows the set digest to gatekeep it without having to
     * initialize it further. */
    auto *thiz = (hb_aat_layout_chain_accelerator_t *) hb_calloc (1, size);
    ಯದಿ (unlikely (!thiz))
      ಪ್ರತ್ಯಾಗಮ nullptr;

    thiz->ಎಣಿಕೆ = ಎಣಿಕೆ;

    hb_accelerate_subtables_context_t c_accelerate_subtables (thiz->subtables, ಅಂಕೆ_glyphs);
    chain.dispatch (&c_accelerate_subtables);

    ಪ್ರತ್ಯಾಗಮ thiz;
  }

  ಶೂನ್ಯ destroy ()
  {
    ಪ್ರತಿ (unsigned i = 0; i < ಎಣಿಕೆ; i++)
      subtables[i].fini ();
  }

  unsigned ಎಣಿಕೆ;
  hb_accelerate_subtables_context_t::hb_applicable_t subtables[HB_VAR_ARRAY];
};

template <typename Types>
ಸಂರಚನಾ ChainSubtable
{
  ಪ್ರಕಾರನಿರ್ದೇಶ typename Types::HBUINT HBUINT;

  template <typename T>
  friend ಸಂರಚನಾ Chain;

  size_t get_size () ಧ್ರುವ     { ಪ್ರತ್ಯಾಗಮ length; }
  unsigned ಪೂರ್ಣಾಙ್ಕ get_type () ಧ್ರುವ     { ಪ್ರತ್ಯಾಗಮ coverage & 0xFF; }
  unsigned ಪೂರ್ಣಾಙ್ಕ get_coverage () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ coverage >> (ಪರಿಮಾಣ (HBUINT) * 8 - 8); }

  ಗಣನಾ Coverage
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
  ಗಣನಾ Type
  {
    Rearrangement	= 0,
    Contextual		= 1,
    Ligature		= 2,
    Noncontextual	= 4,
    Insertion		= 5
  };

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ಧ್ರುವ
  {
    unsigned ಪೂರ್ಣಾಙ್ಕ subtable_type = get_type ();
    TRACE_DISPATCH (ಏತತ್, subtable_type);
    ವಿಕಲ್ಪ (subtable_type) {
    ಪಕ್ಷ Rearrangement:		hb_barrier (); return_trace (c->dispatch (u.rearrangement, std::forward<Ts> (ds)...));
    ಪಕ್ಷ Contextual:		hb_barrier (); return_trace (c->dispatch (u.contextual, std::forward<Ts> (ds)...));
    ಪಕ್ಷ Ligature:		hb_barrier (); return_trace (c->dispatch (u.ligature, std::forward<Ts> (ds)...));
    ಪಕ್ಷ Noncontextual:		hb_barrier (); return_trace (c->dispatch (u.noncontextual, std::forward<Ts> (ds)...));
    ಪಕ್ಷ Insertion:		hb_barrier (); return_trace (c->dispatch (u.insertion, std::forward<Ts> (ds)...));
    ಸ್ವಭಾವ:			return_trace (c->default_return_ಮೌಲ್ಯ ());
    }
  }

  ತಾರ್ಕಿಕ apply (hb_aat_apply_context_t *c) ಧ್ರುವ
  {
    TRACE_APPLY (ಏತತ್);
    // Disabled for https://github.com/harfbuzz/harfbuzz/issues/4873
    //hb_sanitize_with_object_t with (&c->sanitizer, this);
    return_trace (dispatch (c));
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    ಯದಿ (!(length.sanitize (c) &&
	  hb_barrier () &&
	  length >= min_size &&
	  c->check_range (ಏತತ್, length)))
      return_trace (ಅಸತ್ಯ);

    // Disabled for https://github.com/harfbuzz/harfbuzz/issues/4873
    //hb_sanitize_with_object_t with (c, this);
    return_trace (dispatch (c));
  }

  ರಕ್ಷಿತ:
  HBUINT	length;		/* Total subtable length, including this header. */
  HBUINT	coverage;	/* Coverage flags and subtable type. */
  HBUINT32	subFeatureFlags;/* The 32-bit mask identifying which subtable this is. */
  ಸಙ್ಘ {
  RearrangementSubtable<Types>	rearrangement;
  ContextualSubtable<Types>	contextual;
  LigatureSubtable<Types>	ligature;
  NoncontextualSubtable<Types>	noncontextual;
  InsertionSubtable<Types>	insertion;
  } u;
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_MIN (2 * ಪರಿಮಾಣ (HBUINT) + 4);
};

template <typename Types>
ಸಂರಚನಾ Chain
{
  ಪ್ರಕಾರನಿರ್ದೇಶ typename Types::HBUINT HBUINT;

  unsigned get_subtable_ಎಣಿಕೆ () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ subtable_ಎಣಿಕೆ_2; }

  hb_mask_t compile_flags (ಧ್ರುವ hb_aat_map_builder_t *map) ಧ್ರುವ
  {
    hb_mask_t flags = defaultFlags;
    {
      unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = feature_ಎಣಿಕೆ;
      ಪ್ರತಿ (unsigned i = 0; i < ಎಣಿಕೆ; i++)
      {
	ಧ್ರುವ Feature &feature = featureZ[i];
	hb_aat_layout_feature_type_t type = (hb_aat_layout_feature_type_t) (unsigned ಪೂರ್ಣಾಙ್ಕ) feature.featureType;
	hb_aat_layout_feature_selector_t setting = (hb_aat_layout_feature_selector_t) (unsigned ಪೂರ್ಣಾಙ್ಕ) feature.featureSetting;
      retry:
	// Check whether this type/setting pair was requested in the map, and if so, apply its flags.
	// (The search here only looks at the type and setting fields of feature_info_t.)
	hb_aat_map_builder_t::feature_info_t info = { type, setting, ಅಸತ್ಯ, 0 };
	ಯದಿ (map->current_features.bsearch (info))
	{
	  flags &= feature.disableFlags;
	  flags |= feature.enableFlags;
	}
	ಅನ್ಯಥಾ ಯದಿ (type == HB_AAT_LAYOUT_FEATURE_TYPE_LETTER_CASE && setting == HB_AAT_LAYOUT_FEATURE_SELECTOR_SMALL_CAPS)
	{
	  /* Deprecated. https://github.com/harfbuzz/harfbuzz/issues/1342 */
	  type = HB_AAT_LAYOUT_FEATURE_TYPE_LOWER_CASE;
	  setting = HB_AAT_LAYOUT_FEATURE_SELECTOR_LOWER_CASE_SMALL_CAPS;
	  ಗಚ್ಛ retry;
	}
#ifndef HB_NO_AAT
	ಅನ್ಯಥಾ ಯದಿ (type == HB_AAT_LAYOUT_FEATURE_TYPE_LANGUAGE_TAG_TYPE && setting &&
		 /* TODO: Rudimentary language matching. */
		 hb_language_matches (map->face->table.ltag->get_language (setting - 1), map->props.language))
	{
	  flags &= feature.disableFlags;
	  flags |= feature.enableFlags;
	}
#endif
      }
    }
    ಪ್ರತ್ಯಾಗಮ flags;
  }

  ಶೂನ್ಯ apply (hb_aat_apply_context_t *c,
	      ಧ್ರುವ hb_aat_layout_chain_accelerator_t *accel) ಧ್ರುವ
  {
    ಧ್ರುವ ChainSubtable<Types> *subtable = &StructAfter<ChainSubtable<Types>> (featureZ.as_array (feature_ಎಣಿಕೆ));
    unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = subtable_ಎಣಿಕೆ_2;
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಎಣಿಕೆ; i++)
    {
      ತಾರ್ಕಿಕ reverse;

      auto coverage = subtable->get_coverage ();

      hb_mask_t subtable_flags = subtable->subFeatureFlags;
      ಯದಿ (hb_none (hb_iter (c->range_flags) |
		   hb_map ([subtable_flags] (ಧ್ರುವ hb_aat_map_t::range_flags_t _) -> ತಾರ್ಕಿಕ { ಪ್ರತ್ಯಾಗಮ subtable_flags & (_.flags); })))
	ಗಚ್ಛ skip;

      ಯದಿ (!(coverage & ChainSubtable<Types>::AllDirections) &&
	  HB_DIRECTION_IS_VERTICAL (c->buffer->props.direction) !=
	  ತಾರ್ಕಿಕ (coverage & ChainSubtable<Types>::Vertical))
	ಗಚ್ಛ skip;

      c->subtable_flags = subtable_flags;
      c->first_set = accel ? &accel->subtables[i].glyph_set : &Null(hb_bit_set_t);
      c->machine_class_cache = accel ? &accel->subtables[i].class_cache : nullptr;

      ಯದಿ (!c->buffer_intersects_machine ())
      {
	(ಶೂನ್ಯ) c->buffer->message (c->font, "skipped chainsubtable %u because no glyph matches", c->lookup_index);
	ಗಚ್ಛ skip;
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
		ತಾರ್ಕಿಕ (coverage & ChainSubtable<Types>::Backwards) :
		ತಾರ್ಕಿಕ (coverage & ChainSubtable<Types>::Backwards) !=
		HB_DIRECTION_IS_BACKWARD (c->buffer->props.direction);

      ಯದಿ (!c->buffer->message (c->font, "start chainsubtable %u", c->lookup_index))
	ಗಚ್ಛ skip;

      ಯದಿ (reverse != c->buffer_is_reversed)
        c->reverse_buffer ();

      subtable->apply (c);

      (ಶೂನ್ಯ) c->buffer->message (c->font, "end chainsubtable %u", c->lookup_index);

      ಯದಿ (unlikely (!c->buffer->successful)) ಭಙ್ಗ;

    skip:
      subtable = &StructAfter<ChainSubtable<Types>> (*subtable);
      c->set_lookup_index (c->lookup_index + 1);
    }
    ಯದಿ (c->buffer_is_reversed)
      c->reverse_buffer ();
  }

  size_t get_size () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ length; }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ಧ್ರುವ
  {
    ಧ್ರುವ ChainSubtable<Types> *subtable = &StructAfter<ChainSubtable<Types>> (featureZ.as_array (feature_ಎಣಿಕೆ));
    unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = subtable_ಎಣಿಕೆ_2;
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಎಣಿಕೆ; i++)
    {
      typename context_t::return_t ret = subtable->dispatch (c, std::forward<Ts> (ds)...);
      ಯದಿ (c->stop_sublookup_iteration (ret))
	ಪ್ರತ್ಯಾಗಮ ret;
      subtable = &StructAfter<ChainSubtable<Types>> (*subtable);
    }
    ಪ್ರತ್ಯಾಗಮ c->default_return_ಮೌಲ್ಯ ();
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c, unsigned ಪೂರ್ಣಾಙ್ಕ version) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    ಯದಿ (!(length.sanitize (c) &&
	  hb_barrier () &&
	  length >= min_size &&
	  c->check_range (ಏತತ್, length)))
      return_trace (ಅಸತ್ಯ);

    ಯದಿ (!c->check_array (featureZ.arrayZ, feature_ಎಣಿಕೆ))
      return_trace (ಅಸತ್ಯ);

    ಧ್ರುವ ChainSubtable<Types> *subtable = &StructAfter<ChainSubtable<Types>> (featureZ.as_array (feature_ಎಣಿಕೆ));
    unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = subtable_ಎಣಿಕೆ_2;
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಎಣಿಕೆ; i++)
    {
      ಯದಿ (!subtable->sanitize (c))
	return_trace (ಅಸತ್ಯ);
      hb_barrier ();
      subtable = &StructAfter<ChainSubtable<Types>> (*subtable);
    }

    ಯದಿ (version >= 3)
    {
      ಧ್ರುವ SubtableGlyphCoverage *coverage = (ಧ್ರುವ SubtableGlyphCoverage *) subtable;
      ಯದಿ (!coverage->sanitize (c, ಎಣಿಕೆ))
        return_trace (ಅಸತ್ಯ);
    }

    return_trace (ಸತ್ಯ);
  }

  ರಕ್ಷಿತ:
  HBUINT32	defaultFlags;	/* The default specification for subtables. */
  HBUINT32	length;		/* Total byte count, including this header. */
  HBUINT	feature_ಎಣಿಕೆ;	/* Number of feature subtable entries. */
  HBUINT	subtable_ಎಣಿಕೆ_2;	/* The number of subtables in the chain. */

  UnsizedArrayOf<Feature>	featureZ;	/* Features. */
/*ChainSubtable	firstSubtable;*//* Subtables. */
/*SubtableGlyphCoverage coverages*//* Only if version >= 3. */

  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_MIN (8 + 2 * ಪರಿಮಾಣ (HBUINT));
};


/*
 * The 'mort'/'morx' Table
 */

template <typename T, typename Types, hb_tag_t TAG>
ಸಂರಚನಾ mortmorx
{
  ಸ್ಥಿರ constexpr hb_tag_t tableTag = TAG;

  ತಾರ್ಕಿಕ has_data () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ version != 0; }

  ಸಂರಚನಾ accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      hb_sanitize_context_t sc;
      ಏತತ್->table = sc.reference_table<T> (face);

      ಯದಿ (unlikely (ಏತತ್->table->is_blocklisted (ಏತತ್->table.get_blob (), face)))
      {
        hb_blob_destroy (ಏತತ್->table.get_blob ());
        ಏತತ್->table = hb_blob_get_empty ();
      }

      ಏತತ್->chain_ಎಣಿಕೆ = table->get_chain_ಎಣಿಕೆ ();

      ಏತತ್->accels = (hb_atomic_t<hb_aat_layout_chain_accelerator_t *> *) hb_calloc (ಏತತ್->chain_ಎಣಿಕೆ, ಪರಿಮಾಣ (*accels));
      ಯದಿ (unlikely (!ಏತತ್->accels))
      {
	ಏತತ್->chain_ಎಣಿಕೆ = 0;
	ಏತತ್->table.destroy ();
	ಏತತ್->table = hb_blob_get_empty ();
      }
    }
    ~accelerator_t ()
    {
      ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಏತತ್->chain_ಎಣಿಕೆ; i++)
      {
	ಯದಿ (ಏತತ್->accels[i])
	  ಏತತ್->accels[i]->destroy ();
	hb_free (ಏತತ್->accels[i]);
      }
      hb_free (ಏತತ್->accels);
      ಏತತ್->table.destroy ();
    }

    hb_blob_t *get_blob () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ table.get_blob (); }

    template <typename Chain>
    hb_aat_layout_chain_accelerator_t *get_accel (unsigned chain_index, ಧ್ರುವ Chain &chain, unsigned ಅಂಕೆ_glyphs) ಧ್ರುವ
    {
      ಯದಿ (unlikely (chain_index >= chain_ಎಣಿಕೆ)) ಪ್ರತ್ಯಾಗಮ nullptr;

    retry:
      auto *accel = accels[chain_index].get_acquire ();
      ಯದಿ (unlikely (!accel))
      {
	accel = hb_aat_layout_chain_accelerator_t::create (chain, ಅಂಕೆ_glyphs);
	ಯದಿ (unlikely (!accel))
	  ಪ್ರತ್ಯಾಗಮ nullptr;

	ಯದಿ (unlikely (!accels[chain_index].cmpexch (nullptr, accel)))
	{
	  hb_free (accel);
	  ಗಚ್ಛ retry;
	}
      }

      ಪ್ರತ್ಯಾಗಮ accel;
    }

    hb_blob_ptr_t<T> table;
    unsigned ಪೂರ್ಣಾಙ್ಕ chain_ಎಣಿಕೆ;
    hb_atomic_t<hb_aat_layout_chain_accelerator_t *> *accels;
    hb_aat_scratch_t scratch;
  };


  ಶೂನ್ಯ compile_flags (ಧ್ರುವ hb_aat_map_builder_t *mapper,
		      hb_aat_map_t *map) ಧ್ರುವ
  {
    ಧ್ರುವ Chain<Types> *chain = &firstChain;
    unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = chain_ಎಣಿಕೆ_2;
    ಯದಿ (unlikely (!map->chain_flags.resize (ಎಣಿಕೆ)))
      ಪ್ರತ್ಯಾಗಮ;
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಎಣಿಕೆ; i++)
    {
      map->chain_flags[i].push (hb_aat_map_t::range_flags_t {chain->compile_flags (mapper),
							     mapper->range_first,
							     mapper->range_last});
      chain = &StructAfter<Chain<Types>> (*chain);
    }
  }

  unsigned get_chain_ಎಣಿಕೆ () ಧ್ರುವ
  {
    ಪ್ರತ್ಯಾಗಮ chain_ಎಣಿಕೆ_2;
  }
  ಶೂನ್ಯ apply (hb_aat_apply_context_t *c,
	      ಧ್ರುವ hb_aat_map_t &map,
	      ಧ್ರುವ accelerator_t &accel) ಧ್ರುವ
  {
    ಯದಿ (unlikely (!c->buffer->successful)) ಪ್ರತ್ಯಾಗಮ;

    c->buffer->unsafe_to_concat ();

    c->setup_buffer_glyph_set ();

    c->set_lookup_index (0);
    ಧ್ರುವ Chain<Types> *chain = &firstChain;
    unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = chain_ಎಣಿಕೆ_2;
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಎಣಿಕೆ; i++)
    {
      auto *chain_accel = accel.get_accel (i, *chain, c->face->get_ಅಂಕೆ_glyphs ());
      c->range_flags = &map.chain_flags[i];
      chain->apply (c, chain_accel);
      ಯದಿ (unlikely (!c->buffer->successful)) ಪ್ರತ್ಯಾಗಮ;
      chain = &StructAfter<Chain<Types>> (*chain);
    }
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    ಯದಿ (!(version.sanitize (c) &&
	  hb_barrier () &&
	  version &&
	  chain_ಎಣಿಕೆ_2.sanitize (c)))
      return_trace (ಅಸತ್ಯ);

    ಧ್ರುವ Chain<Types> *chain = &firstChain;
    unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = chain_ಎಣಿಕೆ_2;
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಎಣಿಕೆ; i++)
    {
      ಯದಿ (!chain->sanitize (c, version))
	return_trace (ಅಸತ್ಯ);
      hb_barrier ();
      chain = &StructAfter<Chain<Types>> (*chain);
    }

    return_trace (ಸತ್ಯ);
  }

  ರಕ್ಷಿತ:
  HBUINT16	version;	/* Version number of the glyph metamorphosis table.
				 * 1, 2, or 3. */
  HBUINT16	unused;		/* Set to 0. */
  HBUINT32	chain_ಎಣಿಕೆ_2;	/* Number of metamorphosis chains contained in this
				 * table. */
  Chain<Types>	firstChain;	/* Chains. */

  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_MIN (8);
};

ಸಂರಚನಾ morx : mortmorx<morx, ExtendedTypes, HB_AAT_TAG_morx>
{
  HB_INTERNAL ತಾರ್ಕಿಕ is_blocklisted (hb_blob_t *blob,
                                   hb_face_t *face) ಧ್ರುವ;
};

ಸಂರಚನಾ mort : mortmorx<mort, ObsoleteTypes, HB_AAT_TAG_mort>
{
  HB_INTERNAL ತಾರ್ಕಿಕ is_blocklisted (hb_blob_t *blob,
                                   hb_face_t *face) ಧ್ರುವ;
};

ಸಂರಚನಾ morx_accelerator_t : morx::accelerator_t {
  morx_accelerator_t (hb_face_t *face) : morx::accelerator_t (face) {}
};
ಸಂರಚನಾ mort_accelerator_t : mort::accelerator_t {
  mort_accelerator_t (hb_face_t *face) : mort::accelerator_t (face) {}
};


} /* namespace AAT */


#endif /* HB_AAT_LAYOUT_MORX_TABLE_HH */
