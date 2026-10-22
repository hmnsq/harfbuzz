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
কাঠামো RearrangementSubtable
{
  প্রকারলেখ typename Types::HBUINT HBUINT;

  প্রকারলেখ শূন্য Entry_তথ্য;

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

  যৌক্তিক is_action_initiable (ধ্রুবক Entry<Entry_তথ্য> &entry) ধ্রুবক
  {
    ফেরত (entry.flags & MarkFirst);
  }
  যৌক্তিক is_actionable (ধ্রুবক Entry<Entry_তথ্য> &entry) ধ্রুবক
  {
    ফেরত (entry.flags & Verb);
  }

  কাঠামো driver_context_t
  {
    স্থির constexpr যৌক্তিক in_place = সত্য;

    driver_context_t (ধ্রুবক RearrangementSubtable *table_) :
	ret (মিথ্যা),
	table (table_),
	start (0), end (0) {}

    HB_AAT_TRANSITION_INLINE
    শূন্য transition (hb_বাফার_t *বাফার,
		     StateTableDriver<Types, Entry_তথ্য, Flags> *driver,
		     ধ্রুবক Entry<Entry_তথ্য> &entry)
    {
      unsigned পূর্ণসংখ্যা flags = entry.flags;

      যদি (flags & MarkFirst)
	start = বাফার->idx;

      যদি (flags & MarkLast)
	end = hb_min (বাফার->idx + 1, বাফার->len);

      যদি ((flags & Verb) && start < end)
      {
	/* The following map has two nibbles, for start-side
	 * and end-side. Values of 0,1,2 mean move that many
	 * to the other side. Value of 3 means move 2 and
	 * flip them. */
	ধ্রুবক unsigned অক্ষর map[16] =
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

	unsigned পূর্ণসংখ্যা m = map[flags & Verb];
	unsigned পূর্ণসংখ্যা l = hb_min (2u, m >> 4);
	unsigned পূর্ণসংখ্যা r = hb_min (2u, m & 0x0F);
	যৌক্তিক reverse_l = 3 == (m >> 4);
	যৌক্তিক reverse_r = 3 == (m & 0x0F);

	যদি (end - start >= l + r && end-start <= HB_MAX_CONTEXT_দৈর্ঘ্য)
	{
	  বাফার->merge_clusters (start, hb_min (বাফার->idx + 1, বাফার->len));
	  বাফার->merge_clusters (start, end);

	  hb_glyph_info_t *info = বাফার->info;
	  hb_glyph_info_t buf[4];

	  hb_memcpy (buf, info + start, l * আকার (buf[0]));
	  hb_memcpy (buf + 2, info + end - r, r * আকার (buf[0]));

	  যদি (l != r)
	    memmove (info + start + r, info + start + l, (end - start - l - r) * আকার (buf[0]));

	  hb_memcpy (info + start, buf + 2, r * আকার (buf[0]));
	  hb_memcpy (info + end - l, buf, l * আকার (buf[0]));
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

    সর্বজনীন:
    যৌক্তিক ret;
    ধ্রুবক RearrangementSubtable *table;
    ব্যক্তিগত:
    unsigned পূর্ণসংখ্যা start;
    unsigned পূর্ণসংখ্যা end;
  };

  যৌক্তিক apply (hb_aat_apply_context_t *c) ধ্রুবক
  {
    TRACE_APPLY (এই);

    driver_context_t dc (এই);

    StateTableDriver<Types, Entry_তথ্য, Flags> driver (machine, c->face);

    driver.drive (&dc, c);

    return_trace (dc.ret);
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (machine.sanitize (c));
  }

  সর্বজনীন:
  StateTable<Types, Entry_তথ্য>	machine;
  সর্বজনীন:
  DEFINE_আকার_STATIC ((StateTable<Types, Entry_তথ্য>::static_আকার));
};

template <typename Types>
কাঠামো ContextualSubtable
{
  প্রকারলেখ typename Types::HBUINT HBUINT;

  কাঠামো Entry_তথ্য
  {
    প্রকারলেখ typename std::conditional<Types::extended, HBUINT16, HBINT16>::type OffsetType;

    OffsetType	mark_সূচক_2;	/* Index/offset of the substitution table for the
				 * marked glyph. */
    OffsetType	current_সূচক;	/* Index/offset of the substitution table for the
				 * current glyph. */
    সর্বজনীন:
    DEFINE_আকার_STATIC (4);
  };

  গণনা Flags
  {
    SetMark		= 0x8000,	/* If set, make the current glyph the marked glyph. */
    DontAdvance		= 0x4000,	/* If set, don't advance to the next glyph before
					 * going to the new state. */
    Reserved		= 0x3FFF,	/* These bits are reserved and should be set to 0. */
  };

  যৌক্তিক is_action_initiable (ধ্রুবক Entry<Entry_তথ্য> &entry) ধ্রুবক
  {
    ফেরত (entry.flags & SetMark);
  }
  যৌক্তিক is_actionable (ধ্রুবক Entry<Entry_তথ্য> &entry) ধ্রুবক
  {
    যদি (Types::extended)
      ফেরত entry.তথ্য.mark_সূচক_2 != 0xFFFF || entry.তথ্য.current_সূচক != 0xFFFF;
    অন্যথা
      ফেরত entry.তথ্য.mark_সূচক_2 || entry.তথ্য.current_সূচক;
  }

  কাঠামো driver_context_t
  {
    স্থির constexpr যৌক্তিক in_place = সত্য;

    driver_context_t (ধ্রুবক ContextualSubtable *table_,
			     hb_aat_apply_context_t *c_) :
	ret (মিথ্যা),
	c (c_),
	table (table_),
	mark_set (মিথ্যা),
	mark (0),
	subs (table+table->substitutionTables) {}

    HB_AAT_TRANSITION_INLINE
    শূন্য transition (hb_বাফার_t *বাফার,
		     StateTableDriver<Types, Entry_তথ্য, Flags> *driver,
		     ধ্রুবক Entry<Entry_তথ্য> &entry)
    {
      /* Looks like CoreText applies neither mark nor current substitution for
       * end-of-text if mark was not explicitly set. */
      যদি (বাফার->idx == বাফার->len && !mark_set)
	ফেরত;

      ধ্রুবক HBGlyphID16 *replacement;

      replacement = nullptr;
      যদি (Types::extended)
      {
	যদি (entry.তথ্য.mark_সূচক_2 != 0xFFFF)
	{
	  ধ্রুবক Lookup<HBGlyphID16> &lookup = subs[entry.তথ্য.mark_সূচক_2];
	  replacement = lookup.get_মান (বাফার->info[mark].codepoint, driver->অঙ্ক_glyphs);
	}
      }
      অন্যথা
      {
	যদি (entry.তথ্য.mark_সূচক_2)
	{
	  পূর্ণসংখ্যা offset = (পূর্ণসংখ্যা) entry.তথ্য.mark_সূচক_2 + বাফার->info[mark].codepoint;
	  ধ্রুবক UnsizedArrayOf<HBGlyphID16> &subs_old = (ধ্রুবক UnsizedArrayOf<HBGlyphID16> &) subs;
	  replacement = &subs_old[Types::word_Offset_To_সূচক (offset, table, subs_old.arrayZ)];
	  যদি (!(replacement->sanitize (&c->sanitizer) &&
		hb_barrier () &&
		*replacement))
	    replacement = nullptr;
	}
      }
      যদি (replacement)
      {
	বাফার->unsafe_to_break (mark, hb_min (বাফার->idx + 1, বাফার->len));
	c->replace_glyph_inplace (mark, *replacement);
	ret = সত্য;
      }

      replacement = nullptr;
      unsigned পূর্ণসংখ্যা idx = hb_min (বাফার->idx, বাফার->len - 1);
      যদি (Types::extended)
      {
	যদি (entry.তথ্য.current_সূচক != 0xFFFF)
	{
	  ধ্রুবক Lookup<HBGlyphID16> &lookup = subs[entry.তথ্য.current_সূচক];
	  replacement = lookup.get_মান (বাফার->info[idx].codepoint, driver->অঙ্ক_glyphs);
	}
      }
      অন্যথা
      {
	যদি (entry.তথ্য.current_সূচক)
	{
	  পূর্ণসংখ্যা offset = (পূর্ণসংখ্যা) entry.তথ্য.current_সূচক + বাফার->info[idx].codepoint;
	  ধ্রুবক UnsizedArrayOf<HBGlyphID16> &subs_old = (ধ্রুবক UnsizedArrayOf<HBGlyphID16> &) subs;
	  replacement = &subs_old[Types::word_Offset_To_সূচক (offset, table, subs_old.arrayZ)];
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
	mark = বাফার->idx;
      }
    }

    সর্বজনীন:
    যৌক্তিক ret;
    hb_aat_apply_context_t *c;
    ধ্রুবক ContextualSubtable *table;
    ব্যক্তিগত:
    যৌক্তিক mark_set;
    unsigned পূর্ণসংখ্যা mark;
    ধ্রুবক UnsizedListOfOffset16To<Lookup<HBGlyphID16>, HBUINT, শূন্য, মিথ্যা> &subs;
  };

  যৌক্তিক apply (hb_aat_apply_context_t *c) ধ্রুবক
  {
    TRACE_APPLY (এই);

    driver_context_t dc (এই, c);

    StateTableDriver<Types, Entry_তথ্য, Flags> driver (machine, c->face);

    driver.drive (&dc, c);

    return_trace (dc.ret);
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);

    unsigned পূর্ণসংখ্যা অঙ্ক_entries = 0;
    যদি (unlikely (!machine.sanitize (c, &অঙ্ক_entries))) return_trace (মিথ্যা);
    hb_barrier ();

    যদি (!Types::extended)
      return_trace (substitutionTables.sanitize (c, এই, 0));

    unsigned পূর্ণসংখ্যা অঙ্ক_lookups = 0;

    ধ্রুবক Entry<Entry_তথ্য> *entries = machine.get_entries ();
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < অঙ্ক_entries; i++)
    {
      ধ্রুবক Entry_তথ্য &তথ্য = entries[i].তথ্য;

      যদি (তথ্য.mark_সূচক_2 != 0xFFFF)
	অঙ্ক_lookups = hb_max (অঙ্ক_lookups, 1u + তথ্য.mark_সূচক_2);
      যদি (তথ্য.current_সূচক != 0xFFFF)
	অঙ্ক_lookups = hb_max (অঙ্ক_lookups, 1u + তথ্য.current_সূচক);
    }

    return_trace (substitutionTables.sanitize (c, এই, অঙ্ক_lookups));
  }

  সর্বজনীন:
  StateTable<Types, Entry_তথ্য>
		machine;
  সুরক্ষিত:
  NNOffsetTo<UnsizedListOfOffset16To<Lookup<HBGlyphID16>, HBUINT, শূন্য, মিথ্যা>, HBUINT>
		substitutionTables;
  সর্বজনীন:
  DEFINE_আকার_STATIC ((StateTable<Types, Entry_তথ্য>::static_আকার + HBUINT::static_আকার));
};


template <যৌক্তিক extended>
কাঠামো LigatureEntry;

template <>
কাঠামো LigatureEntry<সত্য>
{

  কাঠামো Entry_তথ্য
  {
    HBUINT16	lig_Action_সূচক;	/* Index to the first ligActionTable entry
				 * for processing this group, if indicated
				 * by the flags. */
    সর্বজনীন:
    DEFINE_আকার_STATIC (2);
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

  স্থির যৌক্তিক initiateAction (ধ্রুবক Entry<Entry_তথ্য> &entry)
  { ফেরত entry.flags & SetComponent; }

  স্থির যৌক্তিক performAction (ধ্রুবক Entry<Entry_তথ্য> &entry)
  { ফেরত entry.flags & PerformAction; }

  স্থির unsigned পূর্ণসংখ্যা lig_Action_সূচক (ধ্রুবক Entry<Entry_তথ্য> &entry)
  { ফেরত entry.তথ্য.lig_Action_সূচক; }
};
template <>
কাঠামো LigatureEntry<মিথ্যা>
{
  প্রকারলেখ শূন্য Entry_তথ্য;

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

  স্থির যৌক্তিক initiateAction (ধ্রুবক Entry<Entry_তথ্য> &entry)
  { ফেরত entry.flags & SetComponent; }

  স্থির যৌক্তিক performAction (ধ্রুবক Entry<Entry_তথ্য> &entry)
  { ফেরত entry.flags & Offset; }

  স্থির unsigned পূর্ণসংখ্যা lig_Action_সূচক (ধ্রুবক Entry<Entry_তথ্য> &entry)
  { ফেরত entry.flags & Offset; }
};


template <typename Types>
কাঠামো LigatureSubtable
{
  প্রকারলেখ typename Types::HBUINT HBUINT;

  প্রকারলেখ LigatureEntry<Types::extended> LigatureEntryT;
  প্রকারলেখ typename LigatureEntryT::Entry_তথ্য Entry_তথ্য;

  গণনা Flags
  {
    DontAdvance	= LigatureEntryT::DontAdvance,
  };

  যৌক্তিক is_action_initiable (ধ্রুবক Entry<Entry_তথ্য> &entry) ধ্রুবক
  {
    ফেরত LigatureEntryT::initiateAction (entry);
  }
  যৌক্তিক is_actionable (ধ্রুবক Entry<Entry_তথ্য> &entry) ধ্রুবক
  {
    ফেরত LigatureEntryT::performAction (entry);
  }

  কাঠামো driver_context_t
  {
    স্থির constexpr যৌক্তিক in_place = মিথ্যা;
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

    driver_context_t (ধ্রুবক LigatureSubtable *table_,
		      hb_aat_apply_context_t *c_) :
	ret (মিথ্যা),
	c (c_),
	table (table_),
	ligAction (table+table->ligAction),
	component (table+table->component),
	ligature (table+table->ligature),
	match_দৈর্ঘ্য (0) {}

    HB_AAT_TRANSITION_INLINE
    শূন্য transition (hb_বাফার_t *বাফার,
		     StateTableDriver<Types, Entry_তথ্য, Flags> *driver,
		     ধ্রুবক Entry<Entry_তথ্য> &entry)
    {
      DEBUG_বার্তা (APPLY, nullptr, "Ligature transition at %u", বাফার->idx);
      যদি (entry.flags & LigatureEntryT::SetComponent)
      {
	/* Never mark same index twice, in case DontAdvance was used... */
	যদি (unlikely (match_দৈর্ঘ্য && match_positions[(match_দৈর্ঘ্য - 1u) % ARRAY_দৈর্ঘ্য (match_positions)] == বাফার->out_len))
	  match_দৈর্ঘ্য--;

	match_positions[match_দৈর্ঘ্য++ % ARRAY_দৈর্ঘ্য (match_positions)] = বাফার->out_len;
	DEBUG_বার্তা (APPLY, nullptr, "Set component at %u", বাফার->out_len);
      }

      যদি (LigatureEntryT::performAction (entry))
      {
	DEBUG_বার্তা (APPLY, nullptr, "Perform action with %u", match_দৈর্ঘ্য);
	unsigned পূর্ণসংখ্যা end = বাফার->out_len;

	যদি (unlikely (!match_দৈর্ঘ্য))
	  ফেরত;

	যদি (বাফার->idx >= বাফার->len)
	  ফেরত; /* TODO Work on previous instead? */

	unsigned পূর্ণসংখ্যা cursor = match_দৈর্ঘ্য;

	unsigned পূর্ণসংখ্যা action_idx = LigatureEntryT::lig_Action_সূচক (entry);
	action_idx = Types::offset_To_সূচক (action_idx, table, ligAction.arrayZ);
	ধ্রুবক HBUINT32 *action_তথ্য = &ligAction[action_idx];

	unsigned পূর্ণসংখ্যা ligature_idx = 0;
	unsigned পূর্ণসংখ্যা action;
	করো
	{
	  যদি (unlikely (!cursor))
	  {
	    /* Stack underflow.  Clear the stack. */
	    DEBUG_বার্তা (APPLY, nullptr, "Stack underflow");
	    match_দৈর্ঘ্য = 0;
	    বিরতি;
	  }

	  DEBUG_বার্তা (APPLY, nullptr, "Moving to stack position %u", cursor - 1);
	  যদি (unlikely (!বাফার->move_to (match_positions[--cursor % ARRAY_দৈর্ঘ্য (match_positions)]))) ফেরত;

	  যদি (unlikely (!action_তথ্য->sanitize (&c->sanitizer))) বিরতি;
	  hb_barrier ();
	  action = *action_তথ্য;

	  uint32_t uoffset = action & LigActionOffset;
	  যদি (uoffset & 0x20000000)
	    uoffset |= 0xC0000000; /* Sign-extend. */
	  int32_t offset = (int32_t) uoffset;
	  unsigned পূর্ণসংখ্যা component_idx = বাফার->cur().codepoint + offset;
	  component_idx = Types::word_Offset_To_সূচক (component_idx, table, component.arrayZ);
	  ধ্রুবক HBUINT16 &component_তথ্য = component[component_idx];
	  যদি (unlikely (!component_তথ্য.sanitize (&c->sanitizer))) বিরতি;
	  hb_barrier ();
	  ligature_idx += component_তথ্য;

	  DEBUG_বার্তা (APPLY, nullptr, "Action store %d last %d",
		     যৌক্তিক (action & LigActionStore),
		     যৌক্তিক (action & LigActionLast));
	  যদি (action & (LigActionStore | LigActionLast))
	  {
	    ligature_idx = Types::offset_To_সূচক (ligature_idx, table, ligature.arrayZ);
	    ধ্রুবক HBGlyphID16 &ligature_তথ্য = ligature[ligature_idx];
	    যদি (unlikely (!ligature_তথ্য.sanitize (&c->sanitizer))) বিরতি;
	    hb_barrier ();
	    hb_codepoint_t lig = ligature_তথ্য;

	    DEBUG_বার্তা (APPLY, nullptr, "Produced ligature %u", lig);
	    যদি (unlikely (!c->replace_glyph (lig))) ফেরত;

	    unsigned পূর্ণসংখ্যা lig_end = match_positions[(match_দৈর্ঘ্য - 1u) % ARRAY_দৈর্ঘ্য (match_positions)] + 1u;
	    /* Now go and delete all subsequent components. */
	    যখন (match_দৈর্ঘ্য - 1u > cursor)
	    {
	      DEBUG_বার্তা (APPLY, nullptr, "Skipping ligature component");
	      যদি (unlikely (!বাফার->move_to (match_positions[--match_দৈর্ঘ্য % ARRAY_দৈর্ঘ্য (match_positions)]))) ফেরত;
	      যদি (!c->delete_glyph ()) ফেরত;
	    }

	    যদি (unlikely (!বাফার->move_to (lig_end))) ফেরত;
	    বাফার->merge_out_clusters (match_positions[cursor % ARRAY_দৈর্ঘ্য (match_positions)], বাফার->out_len);
	  }

	  action_তথ্য++;
	}
	যখন (!(action & LigActionLast));
	যদি (unlikely (!বাফার->move_to (end))) ফেরত;
      }
    }

    সর্বজনীন:
    যৌক্তিক ret;
    hb_aat_apply_context_t *c;
    ধ্রুবক LigatureSubtable *table;
    ব্যক্তিগত:
    ধ্রুবক UnsizedArrayOf<HBUINT32> &ligAction;
    ধ্রুবক UnsizedArrayOf<HBUINT16> &component;
    ধ্রুবক UnsizedArrayOf<HBGlyphID16> &ligature;
    unsigned পূর্ণসংখ্যা match_দৈর্ঘ্য;
    unsigned পূর্ণসংখ্যা match_positions[HB_MAX_CONTEXT_দৈর্ঘ্য];
  };

  যৌক্তিক apply (hb_aat_apply_context_t *c) ধ্রুবক
  {
    TRACE_APPLY (এই);

    driver_context_t dc (এই, c);

    StateTableDriver<Types, Entry_তথ্য, Flags> driver (machine, c->face);

    driver.drive (&dc, c);

    return_trace (dc.ret);
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    /* The rest of array sanitizations are done at run-time. */
    return_trace (c->check_struct (এই) && machine.sanitize (c) &&
		  hb_barrier () &&
		  ligAction && component && ligature);
  }

  সর্বজনীন:
  StateTable<Types, Entry_তথ্য>
		machine;
  সুরক্ষিত:
  NNOffsetTo<UnsizedArrayOf<HBUINT32>, HBUINT>
		ligAction;	/* Offset to the ligature action table. */
  NNOffsetTo<UnsizedArrayOf<HBUINT16>, HBUINT>
		component;	/* Offset to the component table. */
  NNOffsetTo<UnsizedArrayOf<HBGlyphID16>, HBUINT>
		ligature;	/* Offset to the actual ligature lists. */
  সর্বজনীন:
  DEFINE_আকার_STATIC ((StateTable<Types, Entry_তথ্য>::static_আকার + 3 * HBUINT::static_আকার));
};

template <typename Types>
কাঠামো NoncontextualSubtable
{
  যৌক্তিক apply (hb_aat_apply_context_t *c) ধ্রুবক
  {
    TRACE_APPLY (এই);

    যৌক্তিক ret = মিথ্যা;
    unsigned পূর্ণসংখ্যা অঙ্ক_glyphs = c->face->get_অঙ্ক_glyphs ();

    hb_glyph_info_t *info = c->বাফার->info;
    unsigned পূর্ণসংখ্যা গুনতি = c->বাফার->len;
    // If there's only one range, we already checked the flag.
    auto *last_range = c->range_flags && (c->range_flags->দৈর্ঘ্য > 1) ? &(*c->range_flags)[0] : nullptr;
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < গুনতি; i++)
    {
      /* This block copied from StateTableDriver::drive. Keep in sync. */
      যদি (unlikely (last_range))
      {
	auto *range = last_range;
	{
	  unsigned cluster = info[i].cluster;
	  যখন (cluster < range->cluster_first)
	    range--;
	  যখন (cluster > range->cluster_last)
	    range++;

	  last_range = range;
	}
	যদি (!(range->flags & c->subtable_flags))
	  চালু;
      }

      ধ্রুবক HBGlyphID16 *replacement = substitute.get_মান (info[i].codepoint, অঙ্ক_glyphs);
      যদি (replacement)
      {
	c->replace_glyph_inplace (i, *replacement);
	ret = সত্য;
      }
    }

    return_trace (ret);
  }

  template <typename set_t>
  শূন্য collect_initial_glyphs (set_t &glyphs, unsigned অঙ্ক_glyphs) ধ্রুবক
  {
    substitute.collect_glyphs (glyphs, অঙ্ক_glyphs);
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (substitute.sanitize (c));
  }

  সুরক্ষিত:
  Lookup<HBGlyphID16>	substitute;
  সর্বজনীন:
  DEFINE_আকার_MIN (2);
};

template <typename Types>
কাঠামো InsertionSubtable
{
  প্রকারলেখ typename Types::HBUINT HBUINT;

  কাঠামো Entry_তথ্য
  {
    HBUINT16	current_ঢোকাও_সূচক;	/* Zero-based index into the insertion glyph table.
					 * The number of glyphs to be inserted is contained
					 * in the currentInsertCount field in the flags.
					 * A value of 0xFFFF indicates no insertion is to
					 * be done. */
    HBUINT16	marked_ঢোকাও_সূচক;	/* Zero-based index into the insertion glyph table.
					 * The number of glyphs to be inserted is contained
					 * in the markedInsertCount field in the flags.
					 * A value of 0xFFFF indicates no insertion is to
					 * be done. */
    সর্বজনীন:
    DEFINE_আকার_STATIC (4);
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

  যৌক্তিক is_action_initiable (ধ্রুবক Entry<Entry_তথ্য> &entry) ধ্রুবক
  {
    ফেরত (entry.flags & SetMark);
  }
  যৌক্তিক is_actionable (ধ্রুবক Entry<Entry_তথ্য> &entry) ধ্রুবক
  {
    ফেরত (entry.flags & (Current_ঢোকাও_গুনতি | Marked_ঢোকাও_গুনতি)) &&
	   (entry.তথ্য.current_ঢোকাও_সূচক != 0xFFFF ||entry.তথ্য.marked_ঢোকাও_সূচক != 0xFFFF);
  }

  কাঠামো driver_context_t
  {
    স্থির constexpr যৌক্তিক in_place = মিথ্যা;

    driver_context_t (ধ্রুবক InsertionSubtable *table_,
		      hb_aat_apply_context_t *c_) :
	ret (মিথ্যা),
	c (c_),
	table (table_),
	mark (0),
	insertionAction (table+table->insertionAction) {}

    HB_AAT_TRANSITION_INLINE
    শূন্য transition (hb_বাফার_t *বাফার,
		     StateTableDriver<Types, Entry_তথ্য, Flags> *driver,
		     ধ্রুবক Entry<Entry_তথ্য> &entry)
    {
      unsigned পূর্ণসংখ্যা flags = entry.flags;

      unsigned mark_loc = বাফার->out_len;

      যদি (entry.তথ্য.marked_ঢোকাও_সূচক != 0xFFFF)
      {
	unsigned পূর্ণসংখ্যা গুনতি = (flags & Marked_ঢোকাও_গুনতি);
	যদি (unlikely ((বাফার->max_ops -= গুনতি) <= 0)) ফেরত;
	unsigned পূর্ণসংখ্যা start = entry.তথ্য.marked_ঢোকাও_সূচক;
	ধ্রুবক HBGlyphID16 *glyphs = &insertionAction[start];
	যদি (unlikely (!c->sanitizer.check_array (glyphs, গুনতি))) গুনতি = 0;
	hb_barrier ();

	যৌক্তিক before = flags & Marked_ঢোকাও_Before;

	unsigned পূর্ণসংখ্যা end = বাফার->out_len;
	যদি (unlikely (!বাফার->move_to (mark))) ফেরত;

	যদি (বাফার->idx < বাফার->len && !before)
	  যদি (unlikely (!বাফার->copy_glyph ())) ফেরত;
	/* TODO We ignore KashidaLike setting. */
	যদি (unlikely (!c->output_glyphs (গুনতি, glyphs))) ফেরত;
	ret = সত্য;
	যদি (বাফার->idx < বাফার->len && !before)
	  বাফার->skip_glyph ();

	যদি (unlikely (!বাফার->move_to (end + গুনতি))) ফেরত;

	বাফার->unsafe_to_break_from_outbuffer (mark, hb_min (বাফার->idx + 1, বাফার->len));
      }

      যদি (flags & SetMark)
	mark = mark_loc;

      যদি (entry.তথ্য.current_ঢোকাও_সূচক != 0xFFFF)
      {
	unsigned পূর্ণসংখ্যা গুনতি = (flags & Current_ঢোকাও_গুনতি) >> 5;
	যদি (unlikely ((বাফার->max_ops -= গুনতি) <= 0)) ফেরত;
	unsigned পূর্ণসংখ্যা start = entry.তথ্য.current_ঢোকাও_সূচক;
	ধ্রুবক HBGlyphID16 *glyphs = &insertionAction[start];
	যদি (unlikely (!c->sanitizer.check_array (glyphs, গুনতি))) গুনতি = 0;
	hb_barrier ();

	যৌক্তিক before = flags & Current_ঢোকাও_Before;

	unsigned পূর্ণসংখ্যা end = বাফার->out_len;

	যদি (বাফার->idx < বাফার->len && !before)
	  যদি (unlikely (!বাফার->copy_glyph ())) ফেরত;
	/* TODO We ignore KashidaLike setting. */
	যদি (unlikely (!c->output_glyphs (গুনতি, glyphs))) ফেরত;
	ret = সত্য;
	যদি (বাফার->idx < বাফার->len && !before)
	  বাফার->skip_glyph ();

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
	যদি (unlikely (!বাফার->move_to ((flags & DontAdvance) ? end : end + গুনতি))) ফেরত;
      }
    }

    সর্বজনীন:
    যৌক্তিক ret;
    hb_aat_apply_context_t *c;
    ধ্রুবক InsertionSubtable *table;
    ব্যক্তিগত:
    unsigned পূর্ণসংখ্যা mark;
    ধ্রুবক UnsizedArrayOf<HBGlyphID16> &insertionAction;
  };

  যৌক্তিক apply (hb_aat_apply_context_t *c) ধ্রুবক
  {
    TRACE_APPLY (এই);

    driver_context_t dc (এই, c);

    StateTableDriver<Types, Entry_তথ্য, Flags> driver (machine, c->face);

    driver.drive (&dc, c);

    return_trace (dc.ret);
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    /* The rest of array sanitizations are done at run-time. */
    return_trace (c->check_struct (এই) && machine.sanitize (c) &&
		  hb_barrier () &&
		  insertionAction);
  }

  সর্বজনীন:
  StateTable<Types, Entry_তথ্য>
		machine;
  সুরক্ষিত:
  NNOffsetTo<UnsizedArrayOf<HBGlyphID16>, HBUINT>
		insertionAction;	/* Byte offset from stateHeader to the start of
					 * the insertion glyph table. */
  সর্বজনীন:
  DEFINE_আকার_STATIC ((StateTable<Types, Entry_তথ্য>::static_আকার + HBUINT::static_আকার));
};


কাঠামো Feature
{
  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই));
  }

  সর্বজনীন:
  HBUINT16	featureType;	/* The type of feature. */
  HBUINT16	featureSetting;	/* The feature's setting (aka selector). */
  HBUINT32	enableFlags;	/* Flags for the settings that this feature
				 * and setting enables. */
  HBUINT32	disableFlags;	/* Complement of flags for the settings that this
				 * feature and setting disable. */

  সর্বজনীন:
  DEFINE_আকার_STATIC (12);
};


কাঠামো hb_accelerate_subtables_context_t :
       hb_dispatch_context_t<hb_accelerate_subtables_context_t>
{
  কাঠামো hb_applicable_t
  {
    friend কাঠামো hb_accelerate_subtables_context_t;
    friend কাঠামো hb_aat_layout_lookup_accelerator_t;

    সর্বজনীন:
    hb_bit_set_t glyph_set;
    mutable hb_aat_class_cache_t class_cache;

    template <typename T>
    auto init_ (ধ্রুবক T &obj_, unsigned অঙ্ক_glyphs, hb_priority<1>) HB_AUTO_RETURN
    (
      obj_.machine.collect_initial_glyphs (glyph_set, অঙ্ক_glyphs, obj_)
    )

    template <typename T>
    শূন্য init_ (ধ্রুবক T &obj_, unsigned অঙ্ক_glyphs, hb_priority<0>)
    {
      obj_.collect_initial_glyphs (glyph_set, অঙ্ক_glyphs);
    }

    template <typename T>
    শূন্য init (ধ্রুবক T &obj_, unsigned অঙ্ক_glyphs)
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
  return_t dispatch (ধ্রুবক T &obj)
  {
    hb_applicable_t *entry = &array[i++];

    entry->init (obj, অঙ্ক_glyphs);

    ফেরত hb_empty_t ();
  }
  স্থির return_t default_return_মান () { ফেরত hb_empty_t (); }

  যৌক্তিক stop_sublookup_iteration (return_t r) ধ্রুবক { ফেরত মিথ্যা; }

  hb_accelerate_subtables_context_t (hb_applicable_t *array_, unsigned অঙ্ক_glyphs_2) :
				     hb_dispatch_context_t<hb_accelerate_subtables_context_t> (),
				     array (array_), অঙ্ক_glyphs (অঙ্ক_glyphs_2) {}

  hb_applicable_t *array;
  unsigned অঙ্ক_glyphs;
  unsigned i = 0;
};

কাঠামো hb_aat_layout_chain_accelerator_t
{
  template <typename TChain>
  স্থির hb_aat_layout_chain_accelerator_t *create (ধ্রুবক TChain &chain, unsigned অঙ্ক_glyphs)
  {
    unsigned গুনতি = chain.get_subtable_গুনতি ();

    unsigned product;
    যদি (unlikely (hb_unsigned_mul_overflows (গুনতি,
        আকার (hb_accelerate_subtables_context_t::hb_applicable_t), &product)))
      ফেরত nullptr;

    unsigned আকার_2;
    যদি (unlikely (hb_unsigned_add_overflows (আকার (hb_aat_layout_chain_accelerator_t) -
        HB_VAR_ARRAY * আকার (hb_accelerate_subtables_context_t::hb_applicable_t), product, &আকার_2))) {
      ফেরত nullptr;
    }

    /* The following is a calloc because when we are collecting subtables,
     * some of them might be invalid and hence not collect; as a result,
     * we might not fill in all the count entries of the subtables array.
     * Zeroing it allows the set digest to gatekeep it without having to
     * initialize it further. */
    auto *thiz = (hb_aat_layout_chain_accelerator_t *) hb_calloc (1, আকার_2);
    যদি (unlikely (!thiz))
      ফেরত nullptr;

    thiz->গুনতি = গুনতি;

    hb_accelerate_subtables_context_t c_accelerate_subtables (thiz->subtables, অঙ্ক_glyphs);
    chain.dispatch (&c_accelerate_subtables);

    ফেরত thiz;
  }

  শূন্য destroy ()
  {
    জন্য (unsigned i = 0; i < গুনতি; i++)
      subtables[i].fini ();
  }

  unsigned গুনতি;
  hb_accelerate_subtables_context_t::hb_applicable_t subtables[HB_VAR_ARRAY];
};

template <typename Types>
কাঠামো ChainSubtable
{
  প্রকারলেখ typename Types::HBUINT HBUINT;

  template <typename T>
  friend কাঠামো Chain;

  size_t get_আকার () ধ্রুবক     { ফেরত দৈর্ঘ্য; }
  unsigned পূর্ণসংখ্যা get_type () ধ্রুবক     { ফেরত coverage & 0xFF; }
  unsigned পূর্ণসংখ্যা get_coverage () ধ্রুবক { ফেরত coverage >> (আকার (HBUINT) * 8 - 8); }

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
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ধ্রুবক
  {
    unsigned পূর্ণসংখ্যা subtable_type = get_type ();
    TRACE_DISPATCH (এই, subtable_type);
    নির্বাচন (subtable_type) {
    ক্ষেত্র Rearrangement:		hb_barrier (); return_trace (c->dispatch (u.rearrangement, std::forward<Ts> (ds)...));
    ক্ষেত্র Contextual:		hb_barrier (); return_trace (c->dispatch (u.contextual, std::forward<Ts> (ds)...));
    ক্ষেত্র Ligature:		hb_barrier (); return_trace (c->dispatch (u.ligature, std::forward<Ts> (ds)...));
    ক্ষেত্র Noncontextual:		hb_barrier (); return_trace (c->dispatch (u.noncontextual, std::forward<Ts> (ds)...));
    ক্ষেত্র Insertion:		hb_barrier (); return_trace (c->dispatch (u.insertion, std::forward<Ts> (ds)...));
    পূর্বনির্ধারিত:			return_trace (c->default_return_মান ());
    }
  }

  যৌক্তিক apply (hb_aat_apply_context_t *c) ধ্রুবক
  {
    TRACE_APPLY (এই);
    // Disabled for https://github.com/harfbuzz/harfbuzz/issues/4873
    //hb_sanitize_with_object_t with (&c->sanitizer, this);
    return_trace (dispatch (c));
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    যদি (!(দৈর্ঘ্য.sanitize (c) &&
	  hb_barrier () &&
	  দৈর্ঘ্য >= min_আকার &&
	  c->check_range (এই, দৈর্ঘ্য)))
      return_trace (মিথ্যা);

    // Disabled for https://github.com/harfbuzz/harfbuzz/issues/4873
    //hb_sanitize_with_object_t with (c, this);
    return_trace (dispatch (c));
  }

  সুরক্ষিত:
  HBUINT	দৈর্ঘ্য;		/* Total subtable length, including this header. */
  HBUINT	coverage;	/* Coverage flags and subtable type. */
  HBUINT32	subFeatureFlags;/* The 32-bit mask identifying which subtable this is. */
  সংঘ {
  RearrangementSubtable<Types>	rearrangement;
  ContextualSubtable<Types>	contextual;
  LigatureSubtable<Types>	ligature;
  NoncontextualSubtable<Types>	noncontextual;
  InsertionSubtable<Types>	insertion;
  } u;
  সর্বজনীন:
  DEFINE_আকার_MIN (2 * আকার (HBUINT) + 4);
};

template <typename Types>
কাঠামো Chain
{
  প্রকারলেখ typename Types::HBUINT HBUINT;

  unsigned get_subtable_গুনতি () ধ্রুবক { ফেরত subtable_গুনতি_2; }

  hb_mask_t compile_flags (ধ্রুবক hb_aat_map_builder_t *map) ধ্রুবক
  {
    hb_mask_t flags = defaultFlags;
    {
      unsigned পূর্ণসংখ্যা গুনতি = feature_গুনতি;
      জন্য (unsigned i = 0; i < গুনতি; i++)
      {
	ধ্রুবক Feature &feature = featureZ[i];
	hb_aat_layout_feature_type_t type = (hb_aat_layout_feature_type_t) (unsigned পূর্ণসংখ্যা) feature.featureType;
	hb_aat_layout_feature_selector_t setting = (hb_aat_layout_feature_selector_t) (unsigned পূর্ণসংখ্যা) feature.featureSetting;
      retry:
	// Check whether this type/setting pair was requested in the map, and if so, apply its flags.
	// (The search here only looks at the type and setting fields of feature_info_t.)
	hb_aat_map_builder_t::feature_info_t info = { type, setting, মিথ্যা, 0 };
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
	  যাও retry;
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
    ফেরত flags;
  }

  শূন্য apply (hb_aat_apply_context_t *c,
	      ধ্রুবক hb_aat_layout_chain_accelerator_t *accel) ধ্রুবক
  {
    ধ্রুবক ChainSubtable<Types> *subtable = &StructAfter<ChainSubtable<Types>> (featureZ.as_array (feature_গুনতি));
    unsigned পূর্ণসংখ্যা গুনতি = subtable_গুনতি_2;
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < গুনতি; i++)
    {
      যৌক্তিক reverse;

      auto coverage = subtable->get_coverage ();

      hb_mask_t subtable_flags = subtable->subFeatureFlags;
      যদি (hb_none (hb_iter (c->range_flags) |
		   hb_map ([subtable_flags] (ধ্রুবক hb_aat_map_t::range_flags_t _) -> যৌক্তিক { ফেরত subtable_flags & (_.flags); })))
	যাও skip;

      যদি (!(coverage & ChainSubtable<Types>::AllDirections) &&
	  HB_DIRECTION_IS_VERTICAL (c->বাফার->props.direction) !=
	  যৌক্তিক (coverage & ChainSubtable<Types>::Vertical))
	যাও skip;

      c->subtable_flags = subtable_flags;
      c->first_set = accel ? &accel->subtables[i].glyph_set : &Null(hb_bit_set_t);
      c->machine_class_cache = accel ? &accel->subtables[i].class_cache : nullptr;

      যদি (!c->বাফার_intersects_machine ())
      {
	(শূন্য) c->বাফার->message (c->font, "skipped chainsubtable %u because no glyph matches", c->lookup_সূচক);
	যাও skip;
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
		যৌক্তিক (coverage & ChainSubtable<Types>::Backwards) :
		যৌক্তিক (coverage & ChainSubtable<Types>::Backwards) !=
		HB_DIRECTION_IS_BACKWARD (c->বাফার->props.direction);

      যদি (!c->বাফার->message (c->font, "start chainsubtable %u", c->lookup_সূচক))
	যাও skip;

      যদি (reverse != c->বাফার_is_reversed)
        c->reverse_বাফার ();

      subtable->apply (c);

      (শূন্য) c->বাফার->message (c->font, "end chainsubtable %u", c->lookup_সূচক);

      যদি (unlikely (!c->বাফার->successful)) বিরতি;

    skip:
      subtable = &StructAfter<ChainSubtable<Types>> (*subtable);
      c->set_lookup_সূচক (c->lookup_সূচক + 1);
    }
    যদি (c->বাফার_is_reversed)
      c->reverse_বাফার ();
  }

  size_t get_আকার () ধ্রুবক { ফেরত দৈর্ঘ্য; }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ধ্রুবক
  {
    ধ্রুবক ChainSubtable<Types> *subtable = &StructAfter<ChainSubtable<Types>> (featureZ.as_array (feature_গুনতি));
    unsigned পূর্ণসংখ্যা গুনতি = subtable_গুনতি_2;
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < গুনতি; i++)
    {
      typename context_t::return_t ret = subtable->dispatch (c, std::forward<Ts> (ds)...);
      যদি (c->stop_sublookup_iteration (ret))
	ফেরত ret;
      subtable = &StructAfter<ChainSubtable<Types>> (*subtable);
    }
    ফেরত c->default_return_মান ();
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c, unsigned পূর্ণসংখ্যা version) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    যদি (!(দৈর্ঘ্য.sanitize (c) &&
	  hb_barrier () &&
	  দৈর্ঘ্য >= min_আকার &&
	  c->check_range (এই, দৈর্ঘ্য)))
      return_trace (মিথ্যা);

    যদি (!c->check_array (featureZ.arrayZ, feature_গুনতি))
      return_trace (মিথ্যা);

    ধ্রুবক ChainSubtable<Types> *subtable = &StructAfter<ChainSubtable<Types>> (featureZ.as_array (feature_গুনতি));
    unsigned পূর্ণসংখ্যা গুনতি = subtable_গুনতি_2;
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < গুনতি; i++)
    {
      যদি (!subtable->sanitize (c))
	return_trace (মিথ্যা);
      hb_barrier ();
      subtable = &StructAfter<ChainSubtable<Types>> (*subtable);
    }

    যদি (version >= 3)
    {
      ধ্রুবক SubtableGlyphCoverage *coverage = (ধ্রুবক SubtableGlyphCoverage *) subtable;
      যদি (!coverage->sanitize (c, গুনতি))
        return_trace (মিথ্যা);
    }

    return_trace (সত্য);
  }

  সুরক্ষিত:
  HBUINT32	defaultFlags;	/* The default specification for subtables. */
  HBUINT32	দৈর্ঘ্য;		/* Total byte count, including this header. */
  HBUINT	feature_গুনতি;	/* Number of feature subtable entries. */
  HBUINT	subtable_গুনতি_2;	/* The number of subtables in the chain. */

  UnsizedArrayOf<Feature>	featureZ;	/* Features. */
/*ChainSubtable	firstSubtable;*//* Subtables. */
/*SubtableGlyphCoverage coverages*//* Only if version >= 3. */

  সর্বজনীন:
  DEFINE_আকার_MIN (8 + 2 * আকার (HBUINT));
};


/*
 * The 'mort'/'morx' Table
 */

template <typename T, typename Types, hb_tag_t TAG>
কাঠামো mortmorx
{
  স্থির constexpr hb_tag_t tableTag = TAG;

  যৌক্তিক has_তথ্য () ধ্রুবক { ফেরত version != 0; }

  কাঠামো accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      hb_sanitize_context_t sc;
      এই->table = sc.reference_table<T> (face);

      যদি (unlikely (এই->table->is_blocklisted (এই->table.get_blob (), face)))
      {
        hb_blob_destroy (এই->table.get_blob ());
        এই->table = hb_blob_get_empty ();
      }

      এই->chain_গুনতি = table->get_chain_গুনতি ();

      এই->accels = (hb_atomic_t<hb_aat_layout_chain_accelerator_t *> *) hb_calloc (এই->chain_গুনতি, আকার (*accels));
      যদি (unlikely (!এই->accels))
      {
	এই->chain_গুনতি = 0;
	এই->table.destroy ();
	এই->table = hb_blob_get_empty ();
      }
    }
    ~accelerator_t ()
    {
      জন্য (unsigned পূর্ণসংখ্যা i = 0; i < এই->chain_গুনতি; i++)
      {
	যদি (এই->accels[i])
	  এই->accels[i]->destroy ();
	hb_free (এই->accels[i]);
      }
      hb_free (এই->accels);
      এই->table.destroy ();
    }

    hb_blob_t *get_blob () ধ্রুবক { ফেরত table.get_blob (); }

    template <typename Chain>
    hb_aat_layout_chain_accelerator_t *get_accel (unsigned chain_সূচক, ধ্রুবক Chain &chain, unsigned অঙ্ক_glyphs) ধ্রুবক
    {
      যদি (unlikely (chain_সূচক >= chain_গুনতি)) ফেরত nullptr;

    retry:
      auto *accel = accels[chain_সূচক].get_acquire ();
      যদি (unlikely (!accel))
      {
	accel = hb_aat_layout_chain_accelerator_t::create (chain, অঙ্ক_glyphs);
	যদি (unlikely (!accel))
	  ফেরত nullptr;

	যদি (unlikely (!accels[chain_সূচক].cmpexch (nullptr, accel)))
	{
	  hb_free (accel);
	  যাও retry;
	}
      }

      ফেরত accel;
    }

    hb_blob_ptr_t<T> table;
    unsigned পূর্ণসংখ্যা chain_গুনতি;
    hb_atomic_t<hb_aat_layout_chain_accelerator_t *> *accels;
    hb_aat_scratch_t scratch;
  };


  শূন্য compile_flags (ধ্রুবক hb_aat_map_builder_t *mapper,
		      hb_aat_map_t *map) ধ্রুবক
  {
    ধ্রুবক Chain<Types> *chain = &firstChain;
    unsigned পূর্ণসংখ্যা গুনতি = chain_গুনতি_2;
    যদি (unlikely (!map->chain_flags.resize (গুনতি)))
      ফেরত;
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < গুনতি; i++)
    {
      map->chain_flags[i].push (hb_aat_map_t::range_flags_t {chain->compile_flags (mapper),
							     mapper->range_first,
							     mapper->range_last});
      chain = &StructAfter<Chain<Types>> (*chain);
    }
  }

  unsigned get_chain_গুনতি () ধ্রুবক
  {
    ফেরত chain_গুনতি_2;
  }
  শূন্য apply (hb_aat_apply_context_t *c,
	      ধ্রুবক hb_aat_map_t &map,
	      ধ্রুবক accelerator_t &accel) ধ্রুবক
  {
    যদি (unlikely (!c->বাফার->successful)) ফেরত;

    c->বাফার->unsafe_to_concat ();

    c->setup_বাফার_glyph_set ();

    c->set_lookup_সূচক (0);
    ধ্রুবক Chain<Types> *chain = &firstChain;
    unsigned পূর্ণসংখ্যা গুনতি = chain_গুনতি_2;
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < গুনতি; i++)
    {
      auto *chain_accel = accel.get_accel (i, *chain, c->face->get_অঙ্ক_glyphs ());
      c->range_flags = &map.chain_flags[i];
      chain->apply (c, chain_accel);
      যদি (unlikely (!c->বাফার->successful)) ফেরত;
      chain = &StructAfter<Chain<Types>> (*chain);
    }
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    যদি (!(version.sanitize (c) &&
	  hb_barrier () &&
	  version &&
	  chain_গুনতি_2.sanitize (c)))
      return_trace (মিথ্যা);

    ধ্রুবক Chain<Types> *chain = &firstChain;
    unsigned পূর্ণসংখ্যা গুনতি = chain_গুনতি_2;
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < গুনতি; i++)
    {
      যদি (!chain->sanitize (c, version))
	return_trace (মিথ্যা);
      hb_barrier ();
      chain = &StructAfter<Chain<Types>> (*chain);
    }

    return_trace (সত্য);
  }

  সুরক্ষিত:
  HBUINT16	version;	/* Version number of the glyph metamorphosis table.
				 * 1, 2, or 3. */
  HBUINT16	unused;		/* Set to 0. */
  HBUINT32	chain_গুনতি_2;	/* Number of metamorphosis chains contained in this
				 * table. */
  Chain<Types>	firstChain;	/* Chains. */

  সর্বজনীন:
  DEFINE_আকার_MIN (8);
};

কাঠামো morx : mortmorx<morx, ExtendedTypes, HB_AAT_TAG_morx>
{
  HB_INTERNAL যৌক্তিক is_blocklisted (hb_blob_t *blob,
                                   hb_face_t *face) ধ্রুবক;
};

কাঠামো mort : mortmorx<mort, ObsoleteTypes, HB_AAT_TAG_mort>
{
  HB_INTERNAL যৌক্তিক is_blocklisted (hb_blob_t *blob,
                                   hb_face_t *face) ধ্রুবক;
};

কাঠামো morx_accelerator_t : morx::accelerator_t {
  morx_accelerator_t (hb_face_t *face) : morx::accelerator_t (face) {}
};
কাঠামো mort_accelerator_t : mort::accelerator_t {
  mort_accelerator_t (hb_face_t *face) : mort::accelerator_t (face) {}
};


} /* namespace AAT */


#endif /* HB_AAT_LAYOUT_MORX_TABLE_HH */
