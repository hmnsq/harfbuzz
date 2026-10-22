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

#அந்தர்பாவ "hb-open-type.hh"
#அந்தர்பாவ "hb-aat-layout-common.hh"
#அந்தர்பாவ "hb-ot-layout.hh"
#அந்தர்பாவ "hb-aat-map.hh"

/*
 * morx -- Extended Glyph Metamorphosis
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6morx.html
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6mort.html
 */
#define HB_AAT_TAG_morx HB_TAG('m','o','r','x')
#define HB_AAT_TAG_mort HB_TAG('m','o','r','t')


நாமாகாஶ AAT {

using நாமாகாஶ OT;

template <typename Types>
கட்டமைப்பு RearrangementSubtable
{
  வகைவரை typename Types::HBUINT HBUINT;

  வகைவரை வெறுமை EntryData;

  எண்ணிக்கை Flags
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

  தருக்க is_action_initiable (மாறிலி Entry<EntryData> &entry) மாறிலி
  {
    திரும்பு (entry.flags & MarkFirst);
  }
  தருக்க is_actionable (மாறிலி Entry<EntryData> &entry) மாறிலி
  {
    திரும்பு (entry.flags & Verb);
  }

  கட்டமைப்பு driver_context_t
  {
    நிலையான constexpr தருக்க in_place = உண்மை;

    driver_context_t (மாறிலி RearrangementSubtable *table_) :
	ret (பொய்),
	table (table_),
	start (0), end (0) {}

    HB_AAT_TRANSITION_INLINE
    வெறுமை transition (hb_buffer_t *buffer,
		     StateTableDriver<Types, EntryData, Flags> *driver,
		     மாறிலி Entry<EntryData> &entry)
    {
      unsigned முழு flags = entry.flags;

      எனில் (flags & MarkFirst)
	start = buffer->idx;

      எனில் (flags & MarkLast)
	end = hb_min (buffer->idx + 1, buffer->len);

      எனில் ((flags & Verb) && start < end)
      {
	/* The following map has two nibbles, for start-side
	 * and end-side. Values of 0,1,2 mean move that many
	 * to the other side. Value of 3 means move 2 and
	 * flip them. */
	மாறிலி unsigned எழுத்து map[16] =
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

	unsigned முழு m = map[flags & Verb];
	unsigned முழு l = hb_min (2u, m >> 4);
	unsigned முழு r = hb_min (2u, m & 0x0F);
	தருக்க reverse_l = 3 == (m >> 4);
	தருக்க reverse_r = 3 == (m & 0x0F);

	எனில் (end - start >= l + r && end-start <= HB_MAX_CONTEXT_LENGTH)
	{
	  buffer->merge_clusters (start, hb_min (buffer->idx + 1, buffer->len));
	  buffer->merge_clusters (start, end);

	  hb_glyph_info_t *info = buffer->info;
	  hb_glyph_info_t buf[4];

	  hb_memcpy (buf, info + start, l * அளவு (buf[0]));
	  hb_memcpy (buf + 2, info + end - r, r * அளவு (buf[0]));

	  எனில் (l != r)
	    memmove (info + start + r, info + start + l, (end - start - l - r) * அளவு (buf[0]));

	  hb_memcpy (info + start, buf + 2, r * அளவு (buf[0]));
	  hb_memcpy (info + end - l, buf, l * அளவு (buf[0]));
	  எனில் (reverse_l)
	  {
	    buf[0] = info[end - 1];
	    info[end - 1] = info[end - 2];
	    info[end - 2] = buf[0];
	  }
	  எனில் (reverse_r)
	  {
	    buf[0] = info[start];
	    info[start] = info[start + 1];
	    info[start + 1] = buf[0];
	  }
	}
      }
    }

    பொது:
    தருக்க ret;
    மாறிலி RearrangementSubtable *table;
    தனிப்பட்ட:
    unsigned முழு start;
    unsigned முழு end;
  };

  தருக்க apply (hb_aat_apply_context_t *c) மாறிலி
  {
    TRACE_APPLY (இது);

    driver_context_t dc (இது);

    StateTableDriver<Types, EntryData, Flags> driver (machine, c->face);

    driver.drive (&dc, c);

    return_trace (dc.ret);
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (machine.sanitize (c));
  }

  பொது:
  StateTable<Types, EntryData>	machine;
  பொது:
  DEFINE_SIZE_STATIC ((StateTable<Types, EntryData>::static_size));
};

template <typename Types>
கட்டமைப்பு ContextualSubtable
{
  வகைவரை typename Types::HBUINT HBUINT;

  கட்டமைப்பு EntryData
  {
    வகைவரை typename std::conditional<Types::extended, HBUINT16, HBINT16>::type OffsetType;

    OffsetType	markIndex;	/* Index/offset of the substitution table for the
				 * marked glyph. */
    OffsetType	currentIndex;	/* Index/offset of the substitution table for the
				 * current glyph. */
    பொது:
    DEFINE_SIZE_STATIC (4);
  };

  எண்ணிக்கை Flags
  {
    SetMark		= 0x8000,	/* If set, make the current glyph the marked glyph. */
    DontAdvance		= 0x4000,	/* If set, don't advance to the next glyph before
					 * going to the new state. */
    Reserved		= 0x3FFF,	/* These bits are reserved and should be set to 0. */
  };

  தருக்க is_action_initiable (மாறிலி Entry<EntryData> &entry) மாறிலி
  {
    திரும்பு (entry.flags & SetMark);
  }
  தருக்க is_actionable (மாறிலி Entry<EntryData> &entry) மாறிலி
  {
    எனில் (Types::extended)
      திரும்பு entry.data.markIndex != 0xFFFF || entry.data.currentIndex != 0xFFFF;
    இல்லையெனில்
      திரும்பு entry.data.markIndex || entry.data.currentIndex;
  }

  கட்டமைப்பு driver_context_t
  {
    நிலையான constexpr தருக்க in_place = உண்மை;

    driver_context_t (மாறிலி ContextualSubtable *table_,
			     hb_aat_apply_context_t *c_) :
	ret (பொய்),
	c (c_),
	table (table_),
	mark_set (பொய்),
	mark (0),
	subs (table+table->substitutionTables) {}

    HB_AAT_TRANSITION_INLINE
    வெறுமை transition (hb_buffer_t *buffer,
		     StateTableDriver<Types, EntryData, Flags> *driver,
		     மாறிலி Entry<EntryData> &entry)
    {
      /* Looks like CoreText applies neither mark nor current substitution for
       * end-of-text if mark was not explicitly set. */
      எனில் (buffer->idx == buffer->len && !mark_set)
	திரும்பு;

      மாறிலி HBGlyphID16 *replacement;

      replacement = nullptr;
      எனில் (Types::extended)
      {
	எனில் (entry.data.markIndex != 0xFFFF)
	{
	  மாறிலி Lookup<HBGlyphID16> &lookup = subs[entry.data.markIndex];
	  replacement = lookup.get_மதிப்பு (buffer->info[mark].codepoint, driver->இலக்கம்_glyphs);
	}
      }
      இல்லையெனில்
      {
	எனில் (entry.data.markIndex)
	{
	  முழு offset = (முழு) entry.data.markIndex + buffer->info[mark].codepoint;
	  மாறிலி UnsizedArrayOf<HBGlyphID16> &subs_old = (மாறிலி UnsizedArrayOf<HBGlyphID16> &) subs;
	  replacement = &subs_old[Types::wordOffsetToIndex (offset, table, subs_old.arrayZ)];
	  எனில் (!(replacement->sanitize (&c->sanitizer) &&
		hb_barrier () &&
		*replacement))
	    replacement = nullptr;
	}
      }
      எனில் (replacement)
      {
	buffer->unsafe_to_break (mark, hb_min (buffer->idx + 1, buffer->len));
	c->replace_glyph_inplace (mark, *replacement);
	ret = உண்மை;
      }

      replacement = nullptr;
      unsigned முழு idx = hb_min (buffer->idx, buffer->len - 1);
      எனில் (Types::extended)
      {
	எனில் (entry.data.currentIndex != 0xFFFF)
	{
	  மாறிலி Lookup<HBGlyphID16> &lookup = subs[entry.data.currentIndex];
	  replacement = lookup.get_மதிப்பு (buffer->info[idx].codepoint, driver->இலக்கம்_glyphs);
	}
      }
      இல்லையெனில்
      {
	எனில் (entry.data.currentIndex)
	{
	  முழு offset = (முழு) entry.data.currentIndex + buffer->info[idx].codepoint;
	  மாறிலி UnsizedArrayOf<HBGlyphID16> &subs_old = (மாறிலி UnsizedArrayOf<HBGlyphID16> &) subs;
	  replacement = &subs_old[Types::wordOffsetToIndex (offset, table, subs_old.arrayZ)];
	  எனில் (!(replacement->sanitize (&c->sanitizer) &&
		hb_barrier () &&
		*replacement))
	    replacement = nullptr;
	}
      }
      எனில் (replacement)
      {
	c->replace_glyph_inplace (idx, *replacement);
	ret = உண்மை;
      }

      எனில் (entry.flags & SetMark)
      {
	mark_set = உண்மை;
	mark = buffer->idx;
      }
    }

    பொது:
    தருக்க ret;
    hb_aat_apply_context_t *c;
    மாறிலி ContextualSubtable *table;
    தனிப்பட்ட:
    தருக்க mark_set;
    unsigned முழு mark;
    மாறிலி UnsizedListOfOffset16To<Lookup<HBGlyphID16>, HBUINT, வெறுமை, பொய்> &subs;
  };

  தருக்க apply (hb_aat_apply_context_t *c) மாறிலி
  {
    TRACE_APPLY (இது);

    driver_context_t dc (இது, c);

    StateTableDriver<Types, EntryData, Flags> driver (machine, c->face);

    driver.drive (&dc, c);

    return_trace (dc.ret);
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);

    unsigned முழு இலக்கம்_entries = 0;
    எனில் (unlikely (!machine.sanitize (c, &இலக்கம்_entries))) return_trace (பொய்);
    hb_barrier ();

    எனில் (!Types::extended)
      return_trace (substitutionTables.sanitize (c, இது, 0));

    unsigned முழு இலக்கம்_lookups = 0;

    மாறிலி Entry<EntryData> *entries = machine.get_entries ();
    க்கு (unsigned முழு i = 0; i < இலக்கம்_entries; i++)
    {
      மாறிலி EntryData &data = entries[i].data;

      எனில் (data.markIndex != 0xFFFF)
	இலக்கம்_lookups = hb_max (இலக்கம்_lookups, 1u + data.markIndex);
      எனில் (data.currentIndex != 0xFFFF)
	இலக்கம்_lookups = hb_max (இலக்கம்_lookups, 1u + data.currentIndex);
    }

    return_trace (substitutionTables.sanitize (c, இது, இலக்கம்_lookups));
  }

  பொது:
  StateTable<Types, EntryData>
		machine;
  பாதுகாக்கப்பட்ட:
  NNOffsetTo<UnsizedListOfOffset16To<Lookup<HBGlyphID16>, HBUINT, வெறுமை, பொய்>, HBUINT>
		substitutionTables;
  பொது:
  DEFINE_SIZE_STATIC ((StateTable<Types, EntryData>::static_size + HBUINT::static_size));
};


template <தருக்க extended>
கட்டமைப்பு LigatureEntry;

template <>
கட்டமைப்பு LigatureEntry<உண்மை>
{

  கட்டமைப்பு EntryData
  {
    HBUINT16	ligActionIndex;	/* Index to the first ligActionTable entry
				 * for processing this group, if indicated
				 * by the flags. */
    பொது:
    DEFINE_SIZE_STATIC (2);
  };

  எண்ணிக்கை Flags
  {
    SetComponent	= 0x8000,	/* Push this glyph onto the component stack for
					 * eventual processing. */
    DontAdvance		= 0x4000,	/* Leave the glyph pointer at this glyph for the
					   next iteration. */
    PerformAction	= 0x2000,	/* Use the ligActionIndex to process a ligature
					 * group. */
    Reserved		= 0x1FFF,	/* These bits are reserved and should be set to 0. */
  };

  நிலையான தருக்க initiateAction (மாறிலி Entry<EntryData> &entry)
  { திரும்பு entry.flags & SetComponent; }

  நிலையான தருக்க performAction (மாறிலி Entry<EntryData> &entry)
  { திரும்பு entry.flags & PerformAction; }

  நிலையான unsigned முழு ligActionIndex (மாறிலி Entry<EntryData> &entry)
  { திரும்பு entry.data.ligActionIndex; }
};
template <>
கட்டமைப்பு LigatureEntry<பொய்>
{
  வகைவரை வெறுமை EntryData;

  எண்ணிக்கை Flags
  {
    SetComponent	= 0x8000,	/* Push this glyph onto the component stack for
					 * eventual processing. */
    DontAdvance		= 0x4000,	/* Leave the glyph pointer at this glyph for the
					   next iteration. */
    Offset		= 0x3FFF,	/* Byte offset from beginning of subtable to the
					 * ligature action list. This value must be a
					 * multiple of 4. */
  };

  நிலையான தருக்க initiateAction (மாறிலி Entry<EntryData> &entry)
  { திரும்பு entry.flags & SetComponent; }

  நிலையான தருக்க performAction (மாறிலி Entry<EntryData> &entry)
  { திரும்பு entry.flags & Offset; }

  நிலையான unsigned முழு ligActionIndex (மாறிலி Entry<EntryData> &entry)
  { திரும்பு entry.flags & Offset; }
};


template <typename Types>
கட்டமைப்பு LigatureSubtable
{
  வகைவரை typename Types::HBUINT HBUINT;

  வகைவரை LigatureEntry<Types::extended> LigatureEntryT;
  வகைவரை typename LigatureEntryT::EntryData EntryData;

  எண்ணிக்கை Flags
  {
    DontAdvance	= LigatureEntryT::DontAdvance,
  };

  தருக்க is_action_initiable (மாறிலி Entry<EntryData> &entry) மாறிலி
  {
    திரும்பு LigatureEntryT::initiateAction (entry);
  }
  தருக்க is_actionable (மாறிலி Entry<EntryData> &entry) மாறிலி
  {
    திரும்பு LigatureEntryT::performAction (entry);
  }

  கட்டமைப்பு driver_context_t
  {
    நிலையான constexpr தருக்க in_place = பொய்;
    எண்ணிக்கை LigActionFlags
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

    driver_context_t (மாறிலி LigatureSubtable *table_,
		      hb_aat_apply_context_t *c_) :
	ret (பொய்),
	c (c_),
	table (table_),
	ligAction (table+table->ligAction),
	component (table+table->component),
	ligature (table+table->ligature),
	match_length (0) {}

    HB_AAT_TRANSITION_INLINE
    வெறுமை transition (hb_buffer_t *buffer,
		     StateTableDriver<Types, EntryData, Flags> *driver,
		     மாறிலி Entry<EntryData> &entry)
    {
      DEBUG_செய்தி (APPLY, nullptr, "Ligature transition at %u", buffer->idx);
      எனில் (entry.flags & LigatureEntryT::SetComponent)
      {
	/* Never mark same index twice, in case DontAdvance was used... */
	எனில் (unlikely (match_length && match_positions[(match_length - 1u) % ARRAY_LENGTH (match_positions)] == buffer->out_len))
	  match_length--;

	match_positions[match_length++ % ARRAY_LENGTH (match_positions)] = buffer->out_len;
	DEBUG_செய்தி (APPLY, nullptr, "Set component at %u", buffer->out_len);
      }

      எனில் (LigatureEntryT::performAction (entry))
      {
	DEBUG_செய்தி (APPLY, nullptr, "Perform action with %u", match_length);
	unsigned முழு end = buffer->out_len;

	எனில் (unlikely (!match_length))
	  திரும்பு;

	எனில் (buffer->idx >= buffer->len)
	  திரும்பு; /* TODO Work on previous instead? */

	unsigned முழு cursor = match_length;

	unsigned முழு action_idx = LigatureEntryT::ligActionIndex (entry);
	action_idx = Types::offsetToIndex (action_idx, table, ligAction.arrayZ);
	மாறிலி HBUINT32 *actionData = &ligAction[action_idx];

	unsigned முழு ligature_idx = 0;
	unsigned முழு action;
	செய்
	{
	  எனில் (unlikely (!cursor))
	  {
	    /* Stack underflow.  Clear the stack. */
	    DEBUG_செய்தி (APPLY, nullptr, "Stack underflow");
	    match_length = 0;
	    நிறுத்து;
	  }

	  DEBUG_செய்தி (APPLY, nullptr, "Moving to stack position %u", cursor - 1);
	  எனில் (unlikely (!buffer->move_to (match_positions[--cursor % ARRAY_LENGTH (match_positions)]))) திரும்பு;

	  எனில் (unlikely (!actionData->sanitize (&c->sanitizer))) நிறுத்து;
	  hb_barrier ();
	  action = *actionData;

	  uint32_t uoffset = action & LigActionOffset;
	  எனில் (uoffset & 0x20000000)
	    uoffset |= 0xC0000000; /* Sign-extend. */
	  int32_t offset = (int32_t) uoffset;
	  unsigned முழு component_idx = buffer->cur().codepoint + offset;
	  component_idx = Types::wordOffsetToIndex (component_idx, table, component.arrayZ);
	  மாறிலி HBUINT16 &componentData = component[component_idx];
	  எனில் (unlikely (!componentData.sanitize (&c->sanitizer))) நிறுத்து;
	  hb_barrier ();
	  ligature_idx += componentData;

	  DEBUG_செய்தி (APPLY, nullptr, "Action store %d last %d",
		     தருக்க (action & LigActionStore),
		     தருக்க (action & LigActionLast));
	  எனில் (action & (LigActionStore | LigActionLast))
	  {
	    ligature_idx = Types::offsetToIndex (ligature_idx, table, ligature.arrayZ);
	    மாறிலி HBGlyphID16 &ligatureData = ligature[ligature_idx];
	    எனில் (unlikely (!ligatureData.sanitize (&c->sanitizer))) நிறுத்து;
	    hb_barrier ();
	    hb_codepoint_t lig = ligatureData;

	    DEBUG_செய்தி (APPLY, nullptr, "Produced ligature %u", lig);
	    எனில் (unlikely (!c->replace_glyph (lig))) திரும்பு;

	    unsigned முழு lig_end = match_positions[(match_length - 1u) % ARRAY_LENGTH (match_positions)] + 1u;
	    /* Now go and delete all subsequent components. */
	    போது (match_length - 1u > cursor)
	    {
	      DEBUG_செய்தி (APPLY, nullptr, "Skipping ligature component");
	      எனில் (unlikely (!buffer->move_to (match_positions[--match_length % ARRAY_LENGTH (match_positions)]))) திரும்பு;
	      எனில் (!c->delete_glyph ()) திரும்பு;
	    }

	    எனில் (unlikely (!buffer->move_to (lig_end))) திரும்பு;
	    buffer->merge_out_clusters (match_positions[cursor % ARRAY_LENGTH (match_positions)], buffer->out_len);
	  }

	  actionData++;
	}
	போது (!(action & LigActionLast));
	எனில் (unlikely (!buffer->move_to (end))) திரும்பு;
      }
    }

    பொது:
    தருக்க ret;
    hb_aat_apply_context_t *c;
    மாறிலி LigatureSubtable *table;
    தனிப்பட்ட:
    மாறிலி UnsizedArrayOf<HBUINT32> &ligAction;
    மாறிலி UnsizedArrayOf<HBUINT16> &component;
    மாறிலி UnsizedArrayOf<HBGlyphID16> &ligature;
    unsigned முழு match_length;
    unsigned முழு match_positions[HB_MAX_CONTEXT_LENGTH];
  };

  தருக்க apply (hb_aat_apply_context_t *c) மாறிலி
  {
    TRACE_APPLY (இது);

    driver_context_t dc (இது, c);

    StateTableDriver<Types, EntryData, Flags> driver (machine, c->face);

    driver.drive (&dc, c);

    return_trace (dc.ret);
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    /* The rest of array sanitizations are done at run-time. */
    return_trace (c->check_struct (இது) && machine.sanitize (c) &&
		  hb_barrier () &&
		  ligAction && component && ligature);
  }

  பொது:
  StateTable<Types, EntryData>
		machine;
  பாதுகாக்கப்பட்ட:
  NNOffsetTo<UnsizedArrayOf<HBUINT32>, HBUINT>
		ligAction;	/* Offset to the ligature action table. */
  NNOffsetTo<UnsizedArrayOf<HBUINT16>, HBUINT>
		component;	/* Offset to the component table. */
  NNOffsetTo<UnsizedArrayOf<HBGlyphID16>, HBUINT>
		ligature;	/* Offset to the actual ligature lists. */
  பொது:
  DEFINE_SIZE_STATIC ((StateTable<Types, EntryData>::static_size + 3 * HBUINT::static_size));
};

template <typename Types>
கட்டமைப்பு NoncontextualSubtable
{
  தருக்க apply (hb_aat_apply_context_t *c) மாறிலி
  {
    TRACE_APPLY (இது);

    தருக்க ret = பொய்;
    unsigned முழு இலக்கம்_glyphs = c->face->get_இலக்கம்_glyphs ();

    hb_glyph_info_t *info = c->buffer->info;
    unsigned முழு மொத்தம் = c->buffer->len;
    // If there's only one range, we already checked the flag.
    auto *last_range = c->range_flags && (c->range_flags->length > 1) ? &(*c->range_flags)[0] : nullptr;
    க்கு (unsigned முழு i = 0; i < மொத்தம்; i++)
    {
      /* This block copied from StateTableDriver::drive. Keep in sync. */
      எனில் (unlikely (last_range))
      {
	auto *range = last_range;
	{
	  unsigned cluster = info[i].cluster;
	  போது (cluster < range->cluster_first)
	    range--;
	  போது (cluster > range->cluster_last)
	    range++;

	  last_range = range;
	}
	எனில் (!(range->flags & c->subtable_flags))
	  தொடர்;
      }

      மாறிலி HBGlyphID16 *replacement = substitute.get_மதிப்பு (info[i].codepoint, இலக்கம்_glyphs);
      எனில் (replacement)
      {
	c->replace_glyph_inplace (i, *replacement);
	ret = உண்மை;
      }
    }

    return_trace (ret);
  }

  template <typename set_t>
  வெறுமை collect_initial_glyphs (set_t &glyphs, unsigned இலக்கம்_glyphs) மாறிலி
  {
    substitute.collect_glyphs (glyphs, இலக்கம்_glyphs);
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (substitute.sanitize (c));
  }

  பாதுகாக்கப்பட்ட:
  Lookup<HBGlyphID16>	substitute;
  பொது:
  DEFINE_SIZE_MIN (2);
};

template <typename Types>
கட்டமைப்பு InsertionSubtable
{
  வகைவரை typename Types::HBUINT HBUINT;

  கட்டமைப்பு EntryData
  {
    HBUINT16	current_செருகு_Index;	/* Zero-based index into the insertion glyph table.
					 * The number of glyphs to be inserted is contained
					 * in the currentInsertCount field in the flags.
					 * A value of 0xFFFF indicates no insertion is to
					 * be done. */
    HBUINT16	marked_செருகு_Index;	/* Zero-based index into the insertion glyph table.
					 * The number of glyphs to be inserted is contained
					 * in the markedInsertCount field in the flags.
					 * A value of 0xFFFF indicates no insertion is to
					 * be done. */
    பொது:
    DEFINE_SIZE_STATIC (4);
  };

  எண்ணிக்கை Flags
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
    Current_செருகு_Before= 0x0800,      /* If set, specifies that insertions are to be made
				       * to the left of the current glyph. If clear,
				       * they're made to the right of the current glyph. */
    Marked_செருகு_Before= 0x0400,	      /* If set, specifies that insertions are to be
				       * made to the left of the marked glyph. If clear,
				       * they're made to the right of the marked glyph. */
    Current_செருகு_மொத்தம்= 0x3E0,	      /* This 5-bit field is treated as a count of the
				       * number of glyphs to insert at the current
				       * position. Since zero means no insertions, the
				       * largest number of insertions at any given
				       * current location is 31 glyphs. */
    Marked_செருகு_மொத்தம்= 0x001F,	      /* This 5-bit field is treated as a count of the
				       * number of glyphs to insert at the marked
				       * position. Since zero means no insertions, the
				       * largest number of insertions at any given
				       * marked location is 31 glyphs. */
  };

  தருக்க is_action_initiable (மாறிலி Entry<EntryData> &entry) மாறிலி
  {
    திரும்பு (entry.flags & SetMark);
  }
  தருக்க is_actionable (மாறிலி Entry<EntryData> &entry) மாறிலி
  {
    திரும்பு (entry.flags & (Current_செருகு_மொத்தம் | Marked_செருகு_மொத்தம்)) &&
	   (entry.data.current_செருகு_Index != 0xFFFF ||entry.data.marked_செருகு_Index != 0xFFFF);
  }

  கட்டமைப்பு driver_context_t
  {
    நிலையான constexpr தருக்க in_place = பொய்;

    driver_context_t (மாறிலி InsertionSubtable *table_,
		      hb_aat_apply_context_t *c_) :
	ret (பொய்),
	c (c_),
	table (table_),
	mark (0),
	insertionAction (table+table->insertionAction) {}

    HB_AAT_TRANSITION_INLINE
    வெறுமை transition (hb_buffer_t *buffer,
		     StateTableDriver<Types, EntryData, Flags> *driver,
		     மாறிலி Entry<EntryData> &entry)
    {
      unsigned முழு flags = entry.flags;

      unsigned mark_loc = buffer->out_len;

      எனில் (entry.data.marked_செருகு_Index != 0xFFFF)
      {
	unsigned முழு மொத்தம் = (flags & Marked_செருகு_மொத்தம்);
	எனில் (unlikely ((buffer->max_ops -= மொத்தம்) <= 0)) திரும்பு;
	unsigned முழு start = entry.data.marked_செருகு_Index;
	மாறிலி HBGlyphID16 *glyphs = &insertionAction[start];
	எனில் (unlikely (!c->sanitizer.check_array (glyphs, மொத்தம்))) மொத்தம் = 0;
	hb_barrier ();

	தருக்க before = flags & Marked_செருகு_Before;

	unsigned முழு end = buffer->out_len;
	எனில் (unlikely (!buffer->move_to (mark))) திரும்பு;

	எனில் (buffer->idx < buffer->len && !before)
	  எனில் (unlikely (!buffer->copy_glyph ())) திரும்பு;
	/* TODO We ignore KashidaLike setting. */
	எனில் (unlikely (!c->output_glyphs (மொத்தம், glyphs))) திரும்பு;
	ret = உண்மை;
	எனில் (buffer->idx < buffer->len && !before)
	  buffer->skip_glyph ();

	எனில் (unlikely (!buffer->move_to (end + மொத்தம்))) திரும்பு;

	buffer->unsafe_to_break_from_outbuffer (mark, hb_min (buffer->idx + 1, buffer->len));
      }

      எனில் (flags & SetMark)
	mark = mark_loc;

      எனில் (entry.data.current_செருகு_Index != 0xFFFF)
      {
	unsigned முழு மொத்தம் = (flags & Current_செருகு_மொத்தம்) >> 5;
	எனில் (unlikely ((buffer->max_ops -= மொத்தம்) <= 0)) திரும்பு;
	unsigned முழு start = entry.data.current_செருகு_Index;
	மாறிலி HBGlyphID16 *glyphs = &insertionAction[start];
	எனில் (unlikely (!c->sanitizer.check_array (glyphs, மொத்தம்))) மொத்தம் = 0;
	hb_barrier ();

	தருக்க before = flags & Current_செருகு_Before;

	unsigned முழு end = buffer->out_len;

	எனில் (buffer->idx < buffer->len && !before)
	  எனில் (unlikely (!buffer->copy_glyph ())) திரும்பு;
	/* TODO We ignore KashidaLike setting. */
	எனில் (unlikely (!c->output_glyphs (மொத்தம், glyphs))) திரும்பு;
	ret = உண்மை;
	எனில் (buffer->idx < buffer->len && !before)
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
	எனில் (unlikely (!buffer->move_to ((flags & DontAdvance) ? end : end + மொத்தம்))) திரும்பு;
      }
    }

    பொது:
    தருக்க ret;
    hb_aat_apply_context_t *c;
    மாறிலி InsertionSubtable *table;
    தனிப்பட்ட:
    unsigned முழு mark;
    மாறிலி UnsizedArrayOf<HBGlyphID16> &insertionAction;
  };

  தருக்க apply (hb_aat_apply_context_t *c) மாறிலி
  {
    TRACE_APPLY (இது);

    driver_context_t dc (இது, c);

    StateTableDriver<Types, EntryData, Flags> driver (machine, c->face);

    driver.drive (&dc, c);

    return_trace (dc.ret);
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    /* The rest of array sanitizations are done at run-time. */
    return_trace (c->check_struct (இது) && machine.sanitize (c) &&
		  hb_barrier () &&
		  insertionAction);
  }

  பொது:
  StateTable<Types, EntryData>
		machine;
  பாதுகாக்கப்பட்ட:
  NNOffsetTo<UnsizedArrayOf<HBGlyphID16>, HBUINT>
		insertionAction;	/* Byte offset from stateHeader to the start of
					 * the insertion glyph table. */
  பொது:
  DEFINE_SIZE_STATIC ((StateTable<Types, EntryData>::static_size + HBUINT::static_size));
};


கட்டமைப்பு Feature
{
  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது));
  }

  பொது:
  HBUINT16	featureType;	/* The type of feature. */
  HBUINT16	featureSetting;	/* The feature's setting (aka selector). */
  HBUINT32	enableFlags;	/* Flags for the settings that this feature
				 * and setting enables. */
  HBUINT32	disableFlags;	/* Complement of flags for the settings that this
				 * feature and setting disable. */

  பொது:
  DEFINE_SIZE_STATIC (12);
};


கட்டமைப்பு hb_accelerate_subtables_context_t :
       hb_dispatch_context_t<hb_accelerate_subtables_context_t>
{
  கட்டமைப்பு hb_applicable_t
  {
    friend கட்டமைப்பு hb_accelerate_subtables_context_t;
    friend கட்டமைப்பு hb_aat_layout_lookup_accelerator_t;

    பொது:
    hb_bit_set_t glyph_set;
    mutable hb_aat_class_cache_t class_cache;

    template <typename T>
    auto init_ (மாறிலி T &obj_, unsigned இலக்கம்_glyphs, hb_priority<1>) HB_AUTO_RETURN
    (
      obj_.machine.collect_initial_glyphs (glyph_set, இலக்கம்_glyphs, obj_)
    )

    template <typename T>
    வெறுமை init_ (மாறிலி T &obj_, unsigned இலக்கம்_glyphs, hb_priority<0>)
    {
      obj_.collect_initial_glyphs (glyph_set, இலக்கம்_glyphs);
    }

    template <typename T>
    வெறுமை init (மாறிலி T &obj_, unsigned இலக்கம்_glyphs)
    {
      glyph_set.init ();
      init_ (obj_, இலக்கம்_glyphs, hb_prioritize);
      class_cache.clear ();
    }

    வெறுமை
    fini ()
    {
      glyph_set.fini ();
    }
  };

  /* Dispatch interface. */
  template <typename T>
  return_t dispatch (மாறிலி T &obj)
  {
    hb_applicable_t *entry = &array[i++];

    entry->init (obj, இலக்கம்_glyphs);

    திரும்பு hb_empty_t ();
  }
  நிலையான return_t default_return_மதிப்பு () { திரும்பு hb_empty_t (); }

  தருக்க stop_sublookup_iteration (return_t r) மாறிலி { திரும்பு பொய்; }

  hb_accelerate_subtables_context_t (hb_applicable_t *array_, unsigned இலக்கம்_glyphs_2) :
				     hb_dispatch_context_t<hb_accelerate_subtables_context_t> (),
				     array (array_), இலக்கம்_glyphs (இலக்கம்_glyphs_2) {}

  hb_applicable_t *array;
  unsigned இலக்கம்_glyphs;
  unsigned i = 0;
};

கட்டமைப்பு hb_aat_layout_chain_accelerator_t
{
  template <typename TChain>
  நிலையான hb_aat_layout_chain_accelerator_t *create (மாறிலி TChain &chain, unsigned இலக்கம்_glyphs)
  {
    unsigned மொத்தம் = chain.get_subtable_மொத்தம் ();

    unsigned product;
    எனில் (unlikely (hb_unsigned_mul_overflows (மொத்தம்,
        அளவு (hb_accelerate_subtables_context_t::hb_applicable_t), &product)))
      திரும்பு nullptr;

    unsigned size;
    எனில் (unlikely (hb_unsigned_add_overflows (அளவு (hb_aat_layout_chain_accelerator_t) -
        HB_VAR_ARRAY * அளவு (hb_accelerate_subtables_context_t::hb_applicable_t), product, &size))) {
      திரும்பு nullptr;
    }

    /* The following is a calloc because when we are collecting subtables,
     * some of them might be invalid and hence not collect; as a result,
     * we might not fill in all the count entries of the subtables array.
     * Zeroing it allows the set digest to gatekeep it without having to
     * initialize it further. */
    auto *thiz = (hb_aat_layout_chain_accelerator_t *) hb_calloc (1, size);
    எனில் (unlikely (!thiz))
      திரும்பு nullptr;

    thiz->மொத்தம் = மொத்தம்;

    hb_accelerate_subtables_context_t c_accelerate_subtables (thiz->subtables, இலக்கம்_glyphs);
    chain.dispatch (&c_accelerate_subtables);

    திரும்பு thiz;
  }

  வெறுமை destroy ()
  {
    க்கு (unsigned i = 0; i < மொத்தம்; i++)
      subtables[i].fini ();
  }

  unsigned மொத்தம்;
  hb_accelerate_subtables_context_t::hb_applicable_t subtables[HB_VAR_ARRAY];
};

template <typename Types>
கட்டமைப்பு ChainSubtable
{
  வகைவரை typename Types::HBUINT HBUINT;

  template <typename T>
  friend கட்டமைப்பு Chain;

  size_t get_size () மாறிலி     { திரும்பு length; }
  unsigned முழு get_type () மாறிலி     { திரும்பு coverage & 0xFF; }
  unsigned முழு get_coverage () மாறிலி { திரும்பு coverage >> (அளவு (HBUINT) * 8 - 8); }

  எண்ணிக்கை Coverage
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
  எண்ணிக்கை Type
  {
    Rearrangement	= 0,
    Contextual		= 1,
    Ligature		= 2,
    Noncontextual	= 4,
    Insertion		= 5
  };

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) மாறிலி
  {
    unsigned முழு subtable_type = get_type ();
    TRACE_DISPATCH (இது, subtable_type);
    தேர்வு (subtable_type) {
    நிலை Rearrangement:		hb_barrier (); return_trace (c->dispatch (u.rearrangement, std::forward<Ts> (ds)...));
    நிலை Contextual:		hb_barrier (); return_trace (c->dispatch (u.contextual, std::forward<Ts> (ds)...));
    நிலை Ligature:		hb_barrier (); return_trace (c->dispatch (u.ligature, std::forward<Ts> (ds)...));
    நிலை Noncontextual:		hb_barrier (); return_trace (c->dispatch (u.noncontextual, std::forward<Ts> (ds)...));
    நிலை Insertion:		hb_barrier (); return_trace (c->dispatch (u.insertion, std::forward<Ts> (ds)...));
    இயல்பு:			return_trace (c->default_return_மதிப்பு ());
    }
  }

  தருக்க apply (hb_aat_apply_context_t *c) மாறிலி
  {
    TRACE_APPLY (இது);
    // Disabled for https://github.com/harfbuzz/harfbuzz/issues/4873
    //hb_sanitize_with_object_t with (&c->sanitizer, this);
    return_trace (dispatch (c));
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    எனில் (!(length.sanitize (c) &&
	  hb_barrier () &&
	  length >= min_size &&
	  c->check_range (இது, length)))
      return_trace (பொய்);

    // Disabled for https://github.com/harfbuzz/harfbuzz/issues/4873
    //hb_sanitize_with_object_t with (c, this);
    return_trace (dispatch (c));
  }

  பாதுகாக்கப்பட்ட:
  HBUINT	length;		/* Total subtable length, including this header. */
  HBUINT	coverage;	/* Coverage flags and subtable type. */
  HBUINT32	subFeatureFlags;/* The 32-bit mask identifying which subtable this is. */
  ஒன்றிப்பு {
  RearrangementSubtable<Types>	rearrangement;
  ContextualSubtable<Types>	contextual;
  LigatureSubtable<Types>	ligature;
  NoncontextualSubtable<Types>	noncontextual;
  InsertionSubtable<Types>	insertion;
  } u;
  பொது:
  DEFINE_SIZE_MIN (2 * அளவு (HBUINT) + 4);
};

template <typename Types>
கட்டமைப்பு Chain
{
  வகைவரை typename Types::HBUINT HBUINT;

  unsigned get_subtable_மொத்தம் () மாறிலி { திரும்பு subtable_மொத்தம்_2; }

  hb_mask_t compile_flags (மாறிலி hb_aat_map_builder_t *map) மாறிலி
  {
    hb_mask_t flags = defaultFlags;
    {
      unsigned முழு மொத்தம் = feature_மொத்தம்;
      க்கு (unsigned i = 0; i < மொத்தம்; i++)
      {
	மாறிலி Feature &feature = featureZ[i];
	hb_aat_layout_feature_type_t type = (hb_aat_layout_feature_type_t) (unsigned முழு) feature.featureType;
	hb_aat_layout_feature_selector_t setting = (hb_aat_layout_feature_selector_t) (unsigned முழு) feature.featureSetting;
      retry:
	// Check whether this type/setting pair was requested in the map, and if so, apply its flags.
	// (The search here only looks at the type and setting fields of feature_info_t.)
	hb_aat_map_builder_t::feature_info_t info = { type, setting, பொய், 0 };
	எனில் (map->current_features.bsearch (info))
	{
	  flags &= feature.disableFlags;
	  flags |= feature.enableFlags;
	}
	இல்லையெனில் எனில் (type == HB_AAT_LAYOUT_FEATURE_TYPE_LETTER_CASE && setting == HB_AAT_LAYOUT_FEATURE_SELECTOR_SMALL_CAPS)
	{
	  /* Deprecated. https://github.com/harfbuzz/harfbuzz/issues/1342 */
	  type = HB_AAT_LAYOUT_FEATURE_TYPE_LOWER_CASE;
	  setting = HB_AAT_LAYOUT_FEATURE_SELECTOR_LOWER_CASE_SMALL_CAPS;
	  செல் retry;
	}
#ifndef HB_NO_AAT
	இல்லையெனில் எனில் (type == HB_AAT_LAYOUT_FEATURE_TYPE_LANGUAGE_TAG_TYPE && setting &&
		 /* TODO: Rudimentary language matching. */
		 hb_language_matches (map->face->table.ltag->get_language (setting - 1), map->props.language))
	{
	  flags &= feature.disableFlags;
	  flags |= feature.enableFlags;
	}
#endif
      }
    }
    திரும்பு flags;
  }

  வெறுமை apply (hb_aat_apply_context_t *c,
	      மாறிலி hb_aat_layout_chain_accelerator_t *accel) மாறிலி
  {
    மாறிலி ChainSubtable<Types> *subtable = &StructAfter<ChainSubtable<Types>> (featureZ.as_array (feature_மொத்தம்));
    unsigned முழு மொத்தம் = subtable_மொத்தம்_2;
    க்கு (unsigned முழு i = 0; i < மொத்தம்; i++)
    {
      தருக்க reverse;

      auto coverage = subtable->get_coverage ();

      hb_mask_t subtable_flags = subtable->subFeatureFlags;
      எனில் (hb_none (hb_iter (c->range_flags) |
		   hb_map ([subtable_flags] (மாறிலி hb_aat_map_t::range_flags_t _) -> தருக்க { திரும்பு subtable_flags & (_.flags); })))
	செல் skip;

      எனில் (!(coverage & ChainSubtable<Types>::AllDirections) &&
	  HB_DIRECTION_IS_VERTICAL (c->buffer->props.direction) !=
	  தருக்க (coverage & ChainSubtable<Types>::Vertical))
	செல் skip;

      c->subtable_flags = subtable_flags;
      c->first_set = accel ? &accel->subtables[i].glyph_set : &Null(hb_bit_set_t);
      c->machine_class_cache = accel ? &accel->subtables[i].class_cache : nullptr;

      எனில் (!c->buffer_intersects_machine ())
      {
	(வெறுமை) c->buffer->message (c->font, "skipped chainsubtable %u because no glyph matches", c->lookup_index);
	செல் skip;
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
		தருக்க (coverage & ChainSubtable<Types>::Backwards) :
		தருக்க (coverage & ChainSubtable<Types>::Backwards) !=
		HB_DIRECTION_IS_BACKWARD (c->buffer->props.direction);

      எனில் (!c->buffer->message (c->font, "start chainsubtable %u", c->lookup_index))
	செல் skip;

      எனில் (reverse != c->buffer_is_reversed)
        c->reverse_buffer ();

      subtable->apply (c);

      (வெறுமை) c->buffer->message (c->font, "end chainsubtable %u", c->lookup_index);

      எனில் (unlikely (!c->buffer->successful)) நிறுத்து;

    skip:
      subtable = &StructAfter<ChainSubtable<Types>> (*subtable);
      c->set_lookup_index (c->lookup_index + 1);
    }
    எனில் (c->buffer_is_reversed)
      c->reverse_buffer ();
  }

  size_t get_size () மாறிலி { திரும்பு length; }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) மாறிலி
  {
    மாறிலி ChainSubtable<Types> *subtable = &StructAfter<ChainSubtable<Types>> (featureZ.as_array (feature_மொத்தம்));
    unsigned முழு மொத்தம் = subtable_மொத்தம்_2;
    க்கு (unsigned முழு i = 0; i < மொத்தம்; i++)
    {
      typename context_t::return_t ret = subtable->dispatch (c, std::forward<Ts> (ds)...);
      எனில் (c->stop_sublookup_iteration (ret))
	திரும்பு ret;
      subtable = &StructAfter<ChainSubtable<Types>> (*subtable);
    }
    திரும்பு c->default_return_மதிப்பு ();
  }

  தருக்க sanitize (hb_sanitize_context_t *c, unsigned முழு version) மாறிலி
  {
    TRACE_SANITIZE (இது);
    எனில் (!(length.sanitize (c) &&
	  hb_barrier () &&
	  length >= min_size &&
	  c->check_range (இது, length)))
      return_trace (பொய்);

    எனில் (!c->check_array (featureZ.arrayZ, feature_மொத்தம்))
      return_trace (பொய்);

    மாறிலி ChainSubtable<Types> *subtable = &StructAfter<ChainSubtable<Types>> (featureZ.as_array (feature_மொத்தம்));
    unsigned முழு மொத்தம் = subtable_மொத்தம்_2;
    க்கு (unsigned முழு i = 0; i < மொத்தம்; i++)
    {
      எனில் (!subtable->sanitize (c))
	return_trace (பொய்);
      hb_barrier ();
      subtable = &StructAfter<ChainSubtable<Types>> (*subtable);
    }

    எனில் (version >= 3)
    {
      மாறிலி SubtableGlyphCoverage *coverage = (மாறிலி SubtableGlyphCoverage *) subtable;
      எனில் (!coverage->sanitize (c, மொத்தம்))
        return_trace (பொய்);
    }

    return_trace (உண்மை);
  }

  பாதுகாக்கப்பட்ட:
  HBUINT32	defaultFlags;	/* The default specification for subtables. */
  HBUINT32	length;		/* Total byte count, including this header. */
  HBUINT	feature_மொத்தம்;	/* Number of feature subtable entries. */
  HBUINT	subtable_மொத்தம்_2;	/* The number of subtables in the chain. */

  UnsizedArrayOf<Feature>	featureZ;	/* Features. */
/*ChainSubtable	firstSubtable;*//* Subtables. */
/*SubtableGlyphCoverage coverages*//* Only if version >= 3. */

  பொது:
  DEFINE_SIZE_MIN (8 + 2 * அளவு (HBUINT));
};


/*
 * The 'mort'/'morx' Table
 */

template <typename T, typename Types, hb_tag_t TAG>
கட்டமைப்பு mortmorx
{
  நிலையான constexpr hb_tag_t tableTag = TAG;

  தருக்க has_data () மாறிலி { திரும்பு version != 0; }

  கட்டமைப்பு accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      hb_sanitize_context_t sc;
      இது->table = sc.reference_table<T> (face);

      எனில் (unlikely (இது->table->is_blocklisted (இது->table.get_blob (), face)))
      {
        hb_blob_destroy (இது->table.get_blob ());
        இது->table = hb_blob_get_empty ();
      }

      இது->chain_மொத்தம் = table->get_chain_மொத்தம் ();

      இது->accels = (hb_atomic_t<hb_aat_layout_chain_accelerator_t *> *) hb_calloc (இது->chain_மொத்தம், அளவு (*accels));
      எனில் (unlikely (!இது->accels))
      {
	இது->chain_மொத்தம் = 0;
	இது->table.destroy ();
	இது->table = hb_blob_get_empty ();
      }
    }
    ~accelerator_t ()
    {
      க்கு (unsigned முழு i = 0; i < இது->chain_மொத்தம்; i++)
      {
	எனில் (இது->accels[i])
	  இது->accels[i]->destroy ();
	hb_free (இது->accels[i]);
      }
      hb_free (இது->accels);
      இது->table.destroy ();
    }

    hb_blob_t *get_blob () மாறிலி { திரும்பு table.get_blob (); }

    template <typename Chain>
    hb_aat_layout_chain_accelerator_t *get_accel (unsigned chain_index, மாறிலி Chain &chain, unsigned இலக்கம்_glyphs) மாறிலி
    {
      எனில் (unlikely (chain_index >= chain_மொத்தம்)) திரும்பு nullptr;

    retry:
      auto *accel = accels[chain_index].get_acquire ();
      எனில் (unlikely (!accel))
      {
	accel = hb_aat_layout_chain_accelerator_t::create (chain, இலக்கம்_glyphs);
	எனில் (unlikely (!accel))
	  திரும்பு nullptr;

	எனில் (unlikely (!accels[chain_index].cmpexch (nullptr, accel)))
	{
	  hb_free (accel);
	  செல் retry;
	}
      }

      திரும்பு accel;
    }

    hb_blob_ptr_t<T> table;
    unsigned முழு chain_மொத்தம்;
    hb_atomic_t<hb_aat_layout_chain_accelerator_t *> *accels;
    hb_aat_scratch_t scratch;
  };


  வெறுமை compile_flags (மாறிலி hb_aat_map_builder_t *mapper,
		      hb_aat_map_t *map) மாறிலி
  {
    மாறிலி Chain<Types> *chain = &firstChain;
    unsigned முழு மொத்தம் = chain_மொத்தம்_2;
    எனில் (unlikely (!map->chain_flags.resize (மொத்தம்)))
      திரும்பு;
    க்கு (unsigned முழு i = 0; i < மொத்தம்; i++)
    {
      map->chain_flags[i].push (hb_aat_map_t::range_flags_t {chain->compile_flags (mapper),
							     mapper->range_first,
							     mapper->range_last});
      chain = &StructAfter<Chain<Types>> (*chain);
    }
  }

  unsigned get_chain_மொத்தம் () மாறிலி
  {
    திரும்பு chain_மொத்தம்_2;
  }
  வெறுமை apply (hb_aat_apply_context_t *c,
	      மாறிலி hb_aat_map_t &map,
	      மாறிலி accelerator_t &accel) மாறிலி
  {
    எனில் (unlikely (!c->buffer->successful)) திரும்பு;

    c->buffer->unsafe_to_concat ();

    c->setup_buffer_glyph_set ();

    c->set_lookup_index (0);
    மாறிலி Chain<Types> *chain = &firstChain;
    unsigned முழு மொத்தம் = chain_மொத்தம்_2;
    க்கு (unsigned முழு i = 0; i < மொத்தம்; i++)
    {
      auto *chain_accel = accel.get_accel (i, *chain, c->face->get_இலக்கம்_glyphs ());
      c->range_flags = &map.chain_flags[i];
      chain->apply (c, chain_accel);
      எனில் (unlikely (!c->buffer->successful)) திரும்பு;
      chain = &StructAfter<Chain<Types>> (*chain);
    }
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    எனில் (!(version.sanitize (c) &&
	  hb_barrier () &&
	  version &&
	  chain_மொத்தம்_2.sanitize (c)))
      return_trace (பொய்);

    மாறிலி Chain<Types> *chain = &firstChain;
    unsigned முழு மொத்தம் = chain_மொத்தம்_2;
    க்கு (unsigned முழு i = 0; i < மொத்தம்; i++)
    {
      எனில் (!chain->sanitize (c, version))
	return_trace (பொய்);
      hb_barrier ();
      chain = &StructAfter<Chain<Types>> (*chain);
    }

    return_trace (உண்மை);
  }

  பாதுகாக்கப்பட்ட:
  HBUINT16	version;	/* Version number of the glyph metamorphosis table.
				 * 1, 2, or 3. */
  HBUINT16	unused;		/* Set to 0. */
  HBUINT32	chain_மொத்தம்_2;	/* Number of metamorphosis chains contained in this
				 * table. */
  Chain<Types>	firstChain;	/* Chains. */

  பொது:
  DEFINE_SIZE_MIN (8);
};

கட்டமைப்பு morx : mortmorx<morx, ExtendedTypes, HB_AAT_TAG_morx>
{
  HB_INTERNAL தருக்க is_blocklisted (hb_blob_t *blob,
                                   hb_face_t *face) மாறிலி;
};

கட்டமைப்பு mort : mortmorx<mort, ObsoleteTypes, HB_AAT_TAG_mort>
{
  HB_INTERNAL தருக்க is_blocklisted (hb_blob_t *blob,
                                   hb_face_t *face) மாறிலி;
};

கட்டமைப்பு morx_accelerator_t : morx::accelerator_t {
  morx_accelerator_t (hb_face_t *face) : morx::accelerator_t (face) {}
};
கட்டமைப்பு mort_accelerator_t : mort::accelerator_t {
  mort_accelerator_t (hb_face_t *face) : mort::accelerator_t (face) {}
};


} /* namespace AAT */


#endif /* HB_AAT_LAYOUT_MORX_TABLE_HH */
