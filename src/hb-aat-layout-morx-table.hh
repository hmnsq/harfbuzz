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

#अन्तर्भाव "hb-open-type.hh"
#अन्तर्भाव "hb-aat-layout-common.hh"
#अन्तर्भाव "hb-ot-layout.hh"
#अन्तर्भाव "hb-aat-map.hh"

/*
 * morx -- Extended Glyph Metamorphosis
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6morx.html
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6mort.html
 */
#define HB_AAT_TAG_morx HB_TAG('m','o','r','x')
#define HB_AAT_TAG_mort HB_TAG('m','o','r','t')


नामाकाश AAT {

using नामाकाश OT;

template <typename Types>
संरचना RearrangementSubtable
{
  प्रकारनिर्देश typename Types::HBUINT HBUINT;

  प्रकारनिर्देश शून्य EntryData;

  गणना Flags
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

  तार्किक is_action_initiable (ध्रुव Entry<EntryData> &entry) ध्रुव
  {
    प्रत्यागम (entry.flags & MarkFirst);
  }
  तार्किक is_actionable (ध्रुव Entry<EntryData> &entry) ध्रुव
  {
    प्रत्यागम (entry.flags & Verb);
  }

  संरचना driver_context_t
  {
    स्थिर constexpr तार्किक in_place = सच्च;

    driver_context_t (ध्रुव RearrangementSubtable *table_) :
	ret (मिच्छ),
	table (table_),
	start (0), end (0) {}

    HB_AAT_TRANSITION_INLINE
    शून्य transition (hb_buffer_t *buffer,
		     StateTableDriver<Types, EntryData, Flags> *driver,
		     ध्रुव Entry<EntryData> &entry)
    {
      unsigned पूर्णाङ्क flags = entry.flags;

      जइ (flags & MarkFirst)
	start = buffer->idx;

      जइ (flags & MarkLast)
	end = hb_min (buffer->idx + 1, buffer->len);

      जइ ((flags & Verb) && start < end)
      {
	/* The following map has two nibbles, for start-side
	 * and end-side. Values of 0,1,2 mean move that many
	 * to the other side. Value of 3 means move 2 and
	 * flip them. */
	ध्रुव unsigned वर्ण map[16] =
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

	unsigned पूर्णाङ्क m = map[flags & Verb];
	unsigned पूर्णाङ्क l = hb_min (2u, m >> 4);
	unsigned पूर्णाङ्क r = hb_min (2u, m & 0x0F);
	तार्किक reverse_l = 3 == (m >> 4);
	तार्किक reverse_r = 3 == (m & 0x0F);

	जइ (end - start >= l + r && end-start <= HB_MAX_CONTEXT_LENGTH)
	{
	  buffer->merge_clusters (start, hb_min (buffer->idx + 1, buffer->len));
	  buffer->merge_clusters (start, end);

	  hb_glyph_info_t *info = buffer->info;
	  hb_glyph_info_t buf[4];

	  hb_memcpy (buf, info + start, l * परिमाण (buf[0]));
	  hb_memcpy (buf + 2, info + end - r, r * परिमाण (buf[0]));

	  जइ (l != r)
	    memmove (info + start + r, info + start + l, (end - start - l - r) * परिमाण (buf[0]));

	  hb_memcpy (info + start, buf + 2, r * परिमाण (buf[0]));
	  hb_memcpy (info + end - l, buf, l * परिमाण (buf[0]));
	  जइ (reverse_l)
	  {
	    buf[0] = info[end - 1];
	    info[end - 1] = info[end - 2];
	    info[end - 2] = buf[0];
	  }
	  जइ (reverse_r)
	  {
	    buf[0] = info[start];
	    info[start] = info[start + 1];
	    info[start + 1] = buf[0];
	  }
	}
      }
    }

    सार्वजनिक:
    तार्किक ret;
    ध्रुव RearrangementSubtable *table;
    गुप्त:
    unsigned पूर्णाङ्क start;
    unsigned पूर्णाङ्क end;
  };

  तार्किक apply (hb_aat_apply_context_t *c) ध्रुव
  {
    TRACE_APPLY (एतत्);

    driver_context_t dc (एतत्);

    StateTableDriver<Types, EntryData, Flags> driver (machine, c->face);

    driver.drive (&dc, c);

    return_trace (dc.ret);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (machine.sanitize (c));
  }

  सार्वजनिक:
  StateTable<Types, EntryData>	machine;
  सार्वजनिक:
  DEFINE_SIZE_STATIC ((StateTable<Types, EntryData>::static_size));
};

template <typename Types>
संरचना ContextualSubtable
{
  प्रकारनिर्देश typename Types::HBUINT HBUINT;

  संरचना EntryData
  {
    प्रकारनिर्देश typename std::conditional<Types::extended, HBUINT16, HBINT16>::type OffsetType;

    OffsetType	markIndex;	/* Index/offset of the substitution table for the
				 * marked glyph. */
    OffsetType	currentIndex;	/* Index/offset of the substitution table for the
				 * current glyph. */
    सार्वजनिक:
    DEFINE_SIZE_STATIC (4);
  };

  गणना Flags
  {
    SetMark		= 0x8000,	/* If set, make the current glyph the marked glyph. */
    DontAdvance		= 0x4000,	/* If set, don't advance to the next glyph before
					 * going to the new state. */
    Reserved		= 0x3FFF,	/* These bits are reserved and should be set to 0. */
  };

  तार्किक is_action_initiable (ध्रुव Entry<EntryData> &entry) ध्रुव
  {
    प्रत्यागम (entry.flags & SetMark);
  }
  तार्किक is_actionable (ध्रुव Entry<EntryData> &entry) ध्रुव
  {
    जइ (Types::extended)
      प्रत्यागम entry.data.markIndex != 0xFFFF || entry.data.currentIndex != 0xFFFF;
    अण्णहा
      प्रत्यागम entry.data.markIndex || entry.data.currentIndex;
  }

  संरचना driver_context_t
  {
    स्थिर constexpr तार्किक in_place = सच्च;

    driver_context_t (ध्रुव ContextualSubtable *table_,
			     hb_aat_apply_context_t *c_) :
	ret (मिच्छ),
	c (c_),
	table (table_),
	mark_set (मिच्छ),
	mark (0),
	subs (table+table->substitutionTables) {}

    HB_AAT_TRANSITION_INLINE
    शून्य transition (hb_buffer_t *buffer,
		     StateTableDriver<Types, EntryData, Flags> *driver,
		     ध्रुव Entry<EntryData> &entry)
    {
      /* Looks like CoreText applies neither mark nor current substitution for
       * end-of-text if mark was not explicitly set. */
      जइ (buffer->idx == buffer->len && !mark_set)
	प्रत्यागम;

      ध्रुव HBGlyphID16 *replacement;

      replacement = nullptr;
      जइ (Types::extended)
      {
	जइ (entry.data.markIndex != 0xFFFF)
	{
	  ध्रुव Lookup<HBGlyphID16> &lookup = subs[entry.data.markIndex];
	  replacement = lookup.get_मानम् (buffer->info[mark].codepoint, driver->अङ्कः_glyphs);
	}
      }
      अण्णहा
      {
	जइ (entry.data.markIndex)
	{
	  पूर्णाङ्क offset = (पूर्णाङ्क) entry.data.markIndex + buffer->info[mark].codepoint;
	  ध्रुव UnsizedArrayOf<HBGlyphID16> &subs_old = (ध्रुव UnsizedArrayOf<HBGlyphID16> &) subs;
	  replacement = &subs_old[Types::wordOffsetToIndex (offset, table, subs_old.arrayZ)];
	  जइ (!(replacement->sanitize (&c->sanitizer) &&
		hb_barrier () &&
		*replacement))
	    replacement = nullptr;
	}
      }
      जइ (replacement)
      {
	buffer->unsafe_to_break (mark, hb_min (buffer->idx + 1, buffer->len));
	c->replace_glyph_inplace (mark, *replacement);
	ret = सच्च;
      }

      replacement = nullptr;
      unsigned पूर्णाङ्क idx = hb_min (buffer->idx, buffer->len - 1);
      जइ (Types::extended)
      {
	जइ (entry.data.currentIndex != 0xFFFF)
	{
	  ध्रुव Lookup<HBGlyphID16> &lookup = subs[entry.data.currentIndex];
	  replacement = lookup.get_मानम् (buffer->info[idx].codepoint, driver->अङ्कः_glyphs);
	}
      }
      अण्णहा
      {
	जइ (entry.data.currentIndex)
	{
	  पूर्णाङ्क offset = (पूर्णाङ्क) entry.data.currentIndex + buffer->info[idx].codepoint;
	  ध्रुव UnsizedArrayOf<HBGlyphID16> &subs_old = (ध्रुव UnsizedArrayOf<HBGlyphID16> &) subs;
	  replacement = &subs_old[Types::wordOffsetToIndex (offset, table, subs_old.arrayZ)];
	  जइ (!(replacement->sanitize (&c->sanitizer) &&
		hb_barrier () &&
		*replacement))
	    replacement = nullptr;
	}
      }
      जइ (replacement)
      {
	c->replace_glyph_inplace (idx, *replacement);
	ret = सच्च;
      }

      जइ (entry.flags & SetMark)
      {
	mark_set = सच्च;
	mark = buffer->idx;
      }
    }

    सार्वजनिक:
    तार्किक ret;
    hb_aat_apply_context_t *c;
    ध्रुव ContextualSubtable *table;
    गुप्त:
    तार्किक mark_set;
    unsigned पूर्णाङ्क mark;
    ध्रुव UnsizedListOfOffset16To<Lookup<HBGlyphID16>, HBUINT, शून्य, मिच्छ> &subs;
  };

  तार्किक apply (hb_aat_apply_context_t *c) ध्रुव
  {
    TRACE_APPLY (एतत्);

    driver_context_t dc (एतत्, c);

    StateTableDriver<Types, EntryData, Flags> driver (machine, c->face);

    driver.drive (&dc, c);

    return_trace (dc.ret);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);

    unsigned पूर्णाङ्क अङ्कः_entries = 0;
    जइ (unlikely (!machine.sanitize (c, &अङ्कः_entries))) return_trace (मिच्छ);
    hb_barrier ();

    जइ (!Types::extended)
      return_trace (substitutionTables.sanitize (c, एतत्, 0));

    unsigned पूर्णाङ्क अङ्कः_lookups = 0;

    ध्रुव Entry<EntryData> *entries = machine.get_entries ();
    प्रति (unsigned पूर्णाङ्क i = 0; i < अङ्कः_entries; i++)
    {
      ध्रुव EntryData &data = entries[i].data;

      जइ (data.markIndex != 0xFFFF)
	अङ्कः_lookups = hb_max (अङ्कः_lookups, 1u + data.markIndex);
      जइ (data.currentIndex != 0xFFFF)
	अङ्कः_lookups = hb_max (अङ्कः_lookups, 1u + data.currentIndex);
    }

    return_trace (substitutionTables.sanitize (c, एतत्, अङ्कः_lookups));
  }

  सार्वजनिक:
  StateTable<Types, EntryData>
		machine;
  रक्षित:
  NNOffsetTo<UnsizedListOfOffset16To<Lookup<HBGlyphID16>, HBUINT, शून्य, मिच्छ>, HBUINT>
		substitutionTables;
  सार्वजनिक:
  DEFINE_SIZE_STATIC ((StateTable<Types, EntryData>::static_size + HBUINT::static_size));
};


template <तार्किक extended>
संरचना LigatureEntry;

template <>
संरचना LigatureEntry<सच्च>
{

  संरचना EntryData
  {
    HBUINT16	ligActionIndex;	/* Index to the first ligActionTable entry
				 * for processing this group, if indicated
				 * by the flags. */
    सार्वजनिक:
    DEFINE_SIZE_STATIC (2);
  };

  गणना Flags
  {
    SetComponent	= 0x8000,	/* Push this glyph onto the component stack for
					 * eventual processing. */
    DontAdvance		= 0x4000,	/* Leave the glyph pointer at this glyph for the
					   next iteration. */
    PerformAction	= 0x2000,	/* Use the ligActionIndex to process a ligature
					 * group. */
    Reserved		= 0x1FFF,	/* These bits are reserved and should be set to 0. */
  };

  स्थिर तार्किक initiateAction (ध्रुव Entry<EntryData> &entry)
  { प्रत्यागम entry.flags & SetComponent; }

  स्थिर तार्किक performAction (ध्रुव Entry<EntryData> &entry)
  { प्रत्यागम entry.flags & PerformAction; }

  स्थिर unsigned पूर्णाङ्क ligActionIndex (ध्रुव Entry<EntryData> &entry)
  { प्रत्यागम entry.data.ligActionIndex; }
};
template <>
संरचना LigatureEntry<मिच्छ>
{
  प्रकारनिर्देश शून्य EntryData;

  गणना Flags
  {
    SetComponent	= 0x8000,	/* Push this glyph onto the component stack for
					 * eventual processing. */
    DontAdvance		= 0x4000,	/* Leave the glyph pointer at this glyph for the
					   next iteration. */
    Offset		= 0x3FFF,	/* Byte offset from beginning of subtable to the
					 * ligature action list. This value must be a
					 * multiple of 4. */
  };

  स्थिर तार्किक initiateAction (ध्रुव Entry<EntryData> &entry)
  { प्रत्यागम entry.flags & SetComponent; }

  स्थिर तार्किक performAction (ध्रुव Entry<EntryData> &entry)
  { प्रत्यागम entry.flags & Offset; }

  स्थिर unsigned पूर्णाङ्क ligActionIndex (ध्रुव Entry<EntryData> &entry)
  { प्रत्यागम entry.flags & Offset; }
};


template <typename Types>
संरचना LigatureSubtable
{
  प्रकारनिर्देश typename Types::HBUINT HBUINT;

  प्रकारनिर्देश LigatureEntry<Types::extended> LigatureEntryT;
  प्रकारनिर्देश typename LigatureEntryT::EntryData EntryData;

  गणना Flags
  {
    DontAdvance	= LigatureEntryT::DontAdvance,
  };

  तार्किक is_action_initiable (ध्रुव Entry<EntryData> &entry) ध्रुव
  {
    प्रत्यागम LigatureEntryT::initiateAction (entry);
  }
  तार्किक is_actionable (ध्रुव Entry<EntryData> &entry) ध्रुव
  {
    प्रत्यागम LigatureEntryT::performAction (entry);
  }

  संरचना driver_context_t
  {
    स्थिर constexpr तार्किक in_place = मिच्छ;
    गणना LigActionFlags
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

    driver_context_t (ध्रुव LigatureSubtable *table_,
		      hb_aat_apply_context_t *c_) :
	ret (मिच्छ),
	c (c_),
	table (table_),
	ligAction (table+table->ligAction),
	component (table+table->component),
	ligature (table+table->ligature),
	match_length (0) {}

    HB_AAT_TRANSITION_INLINE
    शून्य transition (hb_buffer_t *buffer,
		     StateTableDriver<Types, EntryData, Flags> *driver,
		     ध्रुव Entry<EntryData> &entry)
    {
      DEBUG_सन्देशः (APPLY, nullptr, "Ligature transition at %u", buffer->idx);
      जइ (entry.flags & LigatureEntryT::SetComponent)
      {
	/* Never mark same index twice, in case DontAdvance was used... */
	जइ (unlikely (match_length && match_positions[(match_length - 1u) % ARRAY_LENGTH (match_positions)] == buffer->out_len))
	  match_length--;

	match_positions[match_length++ % ARRAY_LENGTH (match_positions)] = buffer->out_len;
	DEBUG_सन्देशः (APPLY, nullptr, "Set component at %u", buffer->out_len);
      }

      जइ (LigatureEntryT::performAction (entry))
      {
	DEBUG_सन्देशः (APPLY, nullptr, "Perform action with %u", match_length);
	unsigned पूर्णाङ्क end = buffer->out_len;

	जइ (unlikely (!match_length))
	  प्रत्यागम;

	जइ (buffer->idx >= buffer->len)
	  प्रत्यागम; /* TODO Work on previous instead? */

	unsigned पूर्णाङ्क cursor = match_length;

	unsigned पूर्णाङ्क action_idx = LigatureEntryT::ligActionIndex (entry);
	action_idx = Types::offsetToIndex (action_idx, table, ligAction.arrayZ);
	ध्रुव HBUINT32 *actionData = &ligAction[action_idx];

	unsigned पूर्णाङ्क ligature_idx = 0;
	unsigned पूर्णाङ्क action;
	करेहि
	{
	  जइ (unlikely (!cursor))
	  {
	    /* Stack underflow.  Clear the stack. */
	    DEBUG_सन्देशः (APPLY, nullptr, "Stack underflow");
	    match_length = 0;
	    भङ्ग;
	  }

	  DEBUG_सन्देशः (APPLY, nullptr, "Moving to stack position %u", cursor - 1);
	  जइ (unlikely (!buffer->move_to (match_positions[--cursor % ARRAY_LENGTH (match_positions)]))) प्रत्यागम;

	  जइ (unlikely (!actionData->sanitize (&c->sanitizer))) भङ्ग;
	  hb_barrier ();
	  action = *actionData;

	  uint32_t uoffset = action & LigActionOffset;
	  जइ (uoffset & 0x20000000)
	    uoffset |= 0xC0000000; /* Sign-extend. */
	  int32_t offset = (int32_t) uoffset;
	  unsigned पूर्णाङ्क component_idx = buffer->cur().codepoint + offset;
	  component_idx = Types::wordOffsetToIndex (component_idx, table, component.arrayZ);
	  ध्रुव HBUINT16 &componentData = component[component_idx];
	  जइ (unlikely (!componentData.sanitize (&c->sanitizer))) भङ्ग;
	  hb_barrier ();
	  ligature_idx += componentData;

	  DEBUG_सन्देशः (APPLY, nullptr, "Action store %d last %d",
		     तार्किक (action & LigActionStore),
		     तार्किक (action & LigActionLast));
	  जइ (action & (LigActionStore | LigActionLast))
	  {
	    ligature_idx = Types::offsetToIndex (ligature_idx, table, ligature.arrayZ);
	    ध्रुव HBGlyphID16 &ligatureData = ligature[ligature_idx];
	    जइ (unlikely (!ligatureData.sanitize (&c->sanitizer))) भङ्ग;
	    hb_barrier ();
	    hb_codepoint_t lig = ligatureData;

	    DEBUG_सन्देशः (APPLY, nullptr, "Produced ligature %u", lig);
	    जइ (unlikely (!c->replace_glyph (lig))) प्रत्यागम;

	    unsigned पूर्णाङ्क lig_end = match_positions[(match_length - 1u) % ARRAY_LENGTH (match_positions)] + 1u;
	    /* Now go and delete all subsequent components. */
	    जाव (match_length - 1u > cursor)
	    {
	      DEBUG_सन्देशः (APPLY, nullptr, "Skipping ligature component");
	      जइ (unlikely (!buffer->move_to (match_positions[--match_length % ARRAY_LENGTH (match_positions)]))) प्रत्यागम;
	      जइ (!c->delete_glyph ()) प्रत्यागम;
	    }

	    जइ (unlikely (!buffer->move_to (lig_end))) प्रत्यागम;
	    buffer->merge_out_clusters (match_positions[cursor % ARRAY_LENGTH (match_positions)], buffer->out_len);
	  }

	  actionData++;
	}
	जाव (!(action & LigActionLast));
	जइ (unlikely (!buffer->move_to (end))) प्रत्यागम;
      }
    }

    सार्वजनिक:
    तार्किक ret;
    hb_aat_apply_context_t *c;
    ध्रुव LigatureSubtable *table;
    गुप्त:
    ध्रुव UnsizedArrayOf<HBUINT32> &ligAction;
    ध्रुव UnsizedArrayOf<HBUINT16> &component;
    ध्रुव UnsizedArrayOf<HBGlyphID16> &ligature;
    unsigned पूर्णाङ्क match_length;
    unsigned पूर्णाङ्क match_positions[HB_MAX_CONTEXT_LENGTH];
  };

  तार्किक apply (hb_aat_apply_context_t *c) ध्रुव
  {
    TRACE_APPLY (एतत्);

    driver_context_t dc (एतत्, c);

    StateTableDriver<Types, EntryData, Flags> driver (machine, c->face);

    driver.drive (&dc, c);

    return_trace (dc.ret);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    /* The rest of array sanitizations are done at run-time. */
    return_trace (c->check_struct (एतत्) && machine.sanitize (c) &&
		  hb_barrier () &&
		  ligAction && component && ligature);
  }

  सार्वजनिक:
  StateTable<Types, EntryData>
		machine;
  रक्षित:
  NNOffsetTo<UnsizedArrayOf<HBUINT32>, HBUINT>
		ligAction;	/* Offset to the ligature action table. */
  NNOffsetTo<UnsizedArrayOf<HBUINT16>, HBUINT>
		component;	/* Offset to the component table. */
  NNOffsetTo<UnsizedArrayOf<HBGlyphID16>, HBUINT>
		ligature;	/* Offset to the actual ligature lists. */
  सार्वजनिक:
  DEFINE_SIZE_STATIC ((StateTable<Types, EntryData>::static_size + 3 * HBUINT::static_size));
};

template <typename Types>
संरचना NoncontextualSubtable
{
  तार्किक apply (hb_aat_apply_context_t *c) ध्रुव
  {
    TRACE_APPLY (एतत्);

    तार्किक ret = मिच्छ;
    unsigned पूर्णाङ्क अङ्कः_glyphs = c->face->get_अङ्कः_glyphs ();

    hb_glyph_info_t *info = c->buffer->info;
    unsigned पूर्णाङ्क गणनम् = c->buffer->len;
    // If there's only one range, we already checked the flag.
    auto *last_range = c->range_flags && (c->range_flags->length > 1) ? &(*c->range_flags)[0] : nullptr;
    प्रति (unsigned पूर्णाङ्क i = 0; i < गणनम्; i++)
    {
      /* This block copied from StateTableDriver::drive. Keep in sync. */
      जइ (unlikely (last_range))
      {
	auto *range = last_range;
	{
	  unsigned cluster = info[i].cluster;
	  जाव (cluster < range->cluster_first)
	    range--;
	  जाव (cluster > range->cluster_last)
	    range++;

	  last_range = range;
	}
	जइ (!(range->flags & c->subtable_flags))
	  अनुवर्त;
      }

      ध्रुव HBGlyphID16 *replacement = substitute.get_मानम् (info[i].codepoint, अङ्कः_glyphs);
      जइ (replacement)
      {
	c->replace_glyph_inplace (i, *replacement);
	ret = सच्च;
      }
    }

    return_trace (ret);
  }

  template <typename set_t>
  शून्य collect_initial_glyphs (set_t &glyphs, unsigned अङ्कः_glyphs) ध्रुव
  {
    substitute.collect_glyphs (glyphs, अङ्कः_glyphs);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (substitute.sanitize (c));
  }

  रक्षित:
  Lookup<HBGlyphID16>	substitute;
  सार्वजनिक:
  DEFINE_SIZE_MIN (2);
};

template <typename Types>
संरचना InsertionSubtable
{
  प्रकारनिर्देश typename Types::HBUINT HBUINT;

  संरचना EntryData
  {
    HBUINT16	current_निवेशय_Index;	/* Zero-based index into the insertion glyph table.
					 * The number of glyphs to be inserted is contained
					 * in the currentInsertCount field in the flags.
					 * A value of 0xFFFF indicates no insertion is to
					 * be done. */
    HBUINT16	marked_निवेशय_Index;	/* Zero-based index into the insertion glyph table.
					 * The number of glyphs to be inserted is contained
					 * in the markedInsertCount field in the flags.
					 * A value of 0xFFFF indicates no insertion is to
					 * be done. */
    सार्वजनिक:
    DEFINE_SIZE_STATIC (4);
  };

  गणना Flags
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
    Current_निवेशय_Before= 0x0800,      /* If set, specifies that insertions are to be made
				       * to the left of the current glyph. If clear,
				       * they're made to the right of the current glyph. */
    Marked_निवेशय_Before= 0x0400,	      /* If set, specifies that insertions are to be
				       * made to the left of the marked glyph. If clear,
				       * they're made to the right of the marked glyph. */
    Current_निवेशय_गणनम्= 0x3E0,	      /* This 5-bit field is treated as a count of the
				       * number of glyphs to insert at the current
				       * position. Since zero means no insertions, the
				       * largest number of insertions at any given
				       * current location is 31 glyphs. */
    Marked_निवेशय_गणनम्= 0x001F,	      /* This 5-bit field is treated as a count of the
				       * number of glyphs to insert at the marked
				       * position. Since zero means no insertions, the
				       * largest number of insertions at any given
				       * marked location is 31 glyphs. */
  };

  तार्किक is_action_initiable (ध्रुव Entry<EntryData> &entry) ध्रुव
  {
    प्रत्यागम (entry.flags & SetMark);
  }
  तार्किक is_actionable (ध्रुव Entry<EntryData> &entry) ध्रुव
  {
    प्रत्यागम (entry.flags & (Current_निवेशय_गणनम् | Marked_निवेशय_गणनम्)) &&
	   (entry.data.current_निवेशय_Index != 0xFFFF ||entry.data.marked_निवेशय_Index != 0xFFFF);
  }

  संरचना driver_context_t
  {
    स्थिर constexpr तार्किक in_place = मिच्छ;

    driver_context_t (ध्रुव InsertionSubtable *table_,
		      hb_aat_apply_context_t *c_) :
	ret (मिच्छ),
	c (c_),
	table (table_),
	mark (0),
	insertionAction (table+table->insertionAction) {}

    HB_AAT_TRANSITION_INLINE
    शून्य transition (hb_buffer_t *buffer,
		     StateTableDriver<Types, EntryData, Flags> *driver,
		     ध्रुव Entry<EntryData> &entry)
    {
      unsigned पूर्णाङ्क flags = entry.flags;

      unsigned mark_loc = buffer->out_len;

      जइ (entry.data.marked_निवेशय_Index != 0xFFFF)
      {
	unsigned पूर्णाङ्क गणनम् = (flags & Marked_निवेशय_गणनम्);
	जइ (unlikely ((buffer->max_ops -= गणनम्) <= 0)) प्रत्यागम;
	unsigned पूर्णाङ्क start = entry.data.marked_निवेशय_Index;
	ध्रुव HBGlyphID16 *glyphs = &insertionAction[start];
	जइ (unlikely (!c->sanitizer.check_array (glyphs, गणनम्))) गणनम् = 0;
	hb_barrier ();

	तार्किक before = flags & Marked_निवेशय_Before;

	unsigned पूर्णाङ्क end = buffer->out_len;
	जइ (unlikely (!buffer->move_to (mark))) प्रत्यागम;

	जइ (buffer->idx < buffer->len && !before)
	  जइ (unlikely (!buffer->copy_glyph ())) प्रत्यागम;
	/* TODO We ignore KashidaLike setting. */
	जइ (unlikely (!c->output_glyphs (गणनम्, glyphs))) प्रत्यागम;
	ret = सच्च;
	जइ (buffer->idx < buffer->len && !before)
	  buffer->skip_glyph ();

	जइ (unlikely (!buffer->move_to (end + गणनम्))) प्रत्यागम;

	buffer->unsafe_to_break_from_outbuffer (mark, hb_min (buffer->idx + 1, buffer->len));
      }

      जइ (flags & SetMark)
	mark = mark_loc;

      जइ (entry.data.current_निवेशय_Index != 0xFFFF)
      {
	unsigned पूर्णाङ्क गणनम् = (flags & Current_निवेशय_गणनम्) >> 5;
	जइ (unlikely ((buffer->max_ops -= गणनम्) <= 0)) प्रत्यागम;
	unsigned पूर्णाङ्क start = entry.data.current_निवेशय_Index;
	ध्रुव HBGlyphID16 *glyphs = &insertionAction[start];
	जइ (unlikely (!c->sanitizer.check_array (glyphs, गणनम्))) गणनम् = 0;
	hb_barrier ();

	तार्किक before = flags & Current_निवेशय_Before;

	unsigned पूर्णाङ्क end = buffer->out_len;

	जइ (buffer->idx < buffer->len && !before)
	  जइ (unlikely (!buffer->copy_glyph ())) प्रत्यागम;
	/* TODO We ignore KashidaLike setting. */
	जइ (unlikely (!c->output_glyphs (गणनम्, glyphs))) प्रत्यागम;
	ret = सच्च;
	जइ (buffer->idx < buffer->len && !before)
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
	जइ (unlikely (!buffer->move_to ((flags & DontAdvance) ? end : end + गणनम्))) प्रत्यागम;
      }
    }

    सार्वजनिक:
    तार्किक ret;
    hb_aat_apply_context_t *c;
    ध्रुव InsertionSubtable *table;
    गुप्त:
    unsigned पूर्णाङ्क mark;
    ध्रुव UnsizedArrayOf<HBGlyphID16> &insertionAction;
  };

  तार्किक apply (hb_aat_apply_context_t *c) ध्रुव
  {
    TRACE_APPLY (एतत्);

    driver_context_t dc (एतत्, c);

    StateTableDriver<Types, EntryData, Flags> driver (machine, c->face);

    driver.drive (&dc, c);

    return_trace (dc.ret);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    /* The rest of array sanitizations are done at run-time. */
    return_trace (c->check_struct (एतत्) && machine.sanitize (c) &&
		  hb_barrier () &&
		  insertionAction);
  }

  सार्वजनिक:
  StateTable<Types, EntryData>
		machine;
  रक्षित:
  NNOffsetTo<UnsizedArrayOf<HBGlyphID16>, HBUINT>
		insertionAction;	/* Byte offset from stateHeader to the start of
					 * the insertion glyph table. */
  सार्वजनिक:
  DEFINE_SIZE_STATIC ((StateTable<Types, EntryData>::static_size + HBUINT::static_size));
};


संरचना Feature
{
  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्));
  }

  सार्वजनिक:
  HBUINT16	featureType;	/* The type of feature. */
  HBUINT16	featureSetting;	/* The feature's setting (aka selector). */
  HBUINT32	enableFlags;	/* Flags for the settings that this feature
				 * and setting enables. */
  HBUINT32	disableFlags;	/* Complement of flags for the settings that this
				 * feature and setting disable. */

  सार्वजनिक:
  DEFINE_SIZE_STATIC (12);
};


संरचना hb_accelerate_subtables_context_t :
       hb_dispatch_context_t<hb_accelerate_subtables_context_t>
{
  संरचना hb_applicable_t
  {
    friend संरचना hb_accelerate_subtables_context_t;
    friend संरचना hb_aat_layout_lookup_accelerator_t;

    सार्वजनिक:
    hb_bit_set_t glyph_set;
    mutable hb_aat_class_cache_t class_cache;

    template <typename T>
    auto init_ (ध्रुव T &obj_, unsigned अङ्कः_glyphs, hb_priority<1>) HB_AUTO_RETURN
    (
      obj_.machine.collect_initial_glyphs (glyph_set, अङ्कः_glyphs, obj_)
    )

    template <typename T>
    शून्य init_ (ध्रुव T &obj_, unsigned अङ्कः_glyphs, hb_priority<0>)
    {
      obj_.collect_initial_glyphs (glyph_set, अङ्कः_glyphs);
    }

    template <typename T>
    शून्य init (ध्रुव T &obj_, unsigned अङ्कः_glyphs)
    {
      glyph_set.init ();
      init_ (obj_, अङ्कः_glyphs, hb_prioritize);
      class_cache.clear ();
    }

    शून्य
    fini ()
    {
      glyph_set.fini ();
    }
  };

  /* Dispatch interface. */
  template <typename T>
  return_t dispatch (ध्रुव T &obj)
  {
    hb_applicable_t *entry = &array[i++];

    entry->init (obj, अङ्कः_glyphs);

    प्रत्यागम hb_empty_t ();
  }
  स्थिर return_t default_return_मानम् () { प्रत्यागम hb_empty_t (); }

  तार्किक stop_sublookup_iteration (return_t r) ध्रुव { प्रत्यागम मिच्छ; }

  hb_accelerate_subtables_context_t (hb_applicable_t *array_, unsigned अङ्कः_glyphs_2) :
				     hb_dispatch_context_t<hb_accelerate_subtables_context_t> (),
				     array (array_), अङ्कः_glyphs (अङ्कः_glyphs_2) {}

  hb_applicable_t *array;
  unsigned अङ्कः_glyphs;
  unsigned i = 0;
};

संरचना hb_aat_layout_chain_accelerator_t
{
  template <typename TChain>
  स्थिर hb_aat_layout_chain_accelerator_t *create (ध्रुव TChain &chain, unsigned अङ्कः_glyphs)
  {
    unsigned गणनम् = chain.get_subtable_गणनम् ();

    unsigned product;
    जइ (unlikely (hb_unsigned_mul_overflows (गणनम्,
        परिमाण (hb_accelerate_subtables_context_t::hb_applicable_t), &product)))
      प्रत्यागम nullptr;

    unsigned size;
    जइ (unlikely (hb_unsigned_add_overflows (परिमाण (hb_aat_layout_chain_accelerator_t) -
        HB_VAR_ARRAY * परिमाण (hb_accelerate_subtables_context_t::hb_applicable_t), product, &size))) {
      प्रत्यागम nullptr;
    }

    /* The following is a calloc because when we are collecting subtables,
     * some of them might be invalid and hence not collect; as a result,
     * we might not fill in all the count entries of the subtables array.
     * Zeroing it allows the set digest to gatekeep it without having to
     * initialize it further. */
    auto *thiz = (hb_aat_layout_chain_accelerator_t *) hb_calloc (1, size);
    जइ (unlikely (!thiz))
      प्रत्यागम nullptr;

    thiz->गणनम् = गणनम्;

    hb_accelerate_subtables_context_t c_accelerate_subtables (thiz->subtables, अङ्कः_glyphs);
    chain.dispatch (&c_accelerate_subtables);

    प्रत्यागम thiz;
  }

  शून्य destroy ()
  {
    प्रति (unsigned i = 0; i < गणनम्; i++)
      subtables[i].fini ();
  }

  unsigned गणनम्;
  hb_accelerate_subtables_context_t::hb_applicable_t subtables[HB_VAR_ARRAY];
};

template <typename Types>
संरचना ChainSubtable
{
  प्रकारनिर्देश typename Types::HBUINT HBUINT;

  template <typename T>
  friend संरचना Chain;

  size_t get_size () ध्रुव     { प्रत्यागम length; }
  unsigned पूर्णाङ्क get_type () ध्रुव     { प्रत्यागम coverage & 0xFF; }
  unsigned पूर्णाङ्क get_coverage () ध्रुव { प्रत्यागम coverage >> (परिमाण (HBUINT) * 8 - 8); }

  गणना Coverage
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
  गणना Type
  {
    Rearrangement	= 0,
    Contextual		= 1,
    Ligature		= 2,
    Noncontextual	= 4,
    Insertion		= 5
  };

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ध्रुव
  {
    unsigned पूर्णाङ्क subtable_type = get_type ();
    TRACE_DISPATCH (एतत्, subtable_type);
    विकल्प (subtable_type) {
    पक्ष Rearrangement:		hb_barrier (); return_trace (c->dispatch (u.rearrangement, std::forward<Ts> (ds)...));
    पक्ष Contextual:		hb_barrier (); return_trace (c->dispatch (u.contextual, std::forward<Ts> (ds)...));
    पक्ष Ligature:		hb_barrier (); return_trace (c->dispatch (u.ligature, std::forward<Ts> (ds)...));
    पक्ष Noncontextual:		hb_barrier (); return_trace (c->dispatch (u.noncontextual, std::forward<Ts> (ds)...));
    पक्ष Insertion:		hb_barrier (); return_trace (c->dispatch (u.insertion, std::forward<Ts> (ds)...));
    स्वभाव:			return_trace (c->default_return_मानम् ());
    }
  }

  तार्किक apply (hb_aat_apply_context_t *c) ध्रुव
  {
    TRACE_APPLY (एतत्);
    // Disabled for https://github.com/harfbuzz/harfbuzz/issues/4873
    //hb_sanitize_with_object_t with (&c->sanitizer, this);
    return_trace (dispatch (c));
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    जइ (!(length.sanitize (c) &&
	  hb_barrier () &&
	  length >= min_size &&
	  c->check_range (एतत्, length)))
      return_trace (मिच्छ);

    // Disabled for https://github.com/harfbuzz/harfbuzz/issues/4873
    //hb_sanitize_with_object_t with (c, this);
    return_trace (dispatch (c));
  }

  रक्षित:
  HBUINT	length;		/* Total subtable length, including this header. */
  HBUINT	coverage;	/* Coverage flags and subtable type. */
  HBUINT32	subFeatureFlags;/* The 32-bit mask identifying which subtable this is. */
  सङ्घ {
  RearrangementSubtable<Types>	rearrangement;
  ContextualSubtable<Types>	contextual;
  LigatureSubtable<Types>	ligature;
  NoncontextualSubtable<Types>	noncontextual;
  InsertionSubtable<Types>	insertion;
  } u;
  सार्वजनिक:
  DEFINE_SIZE_MIN (2 * परिमाण (HBUINT) + 4);
};

template <typename Types>
संरचना Chain
{
  प्रकारनिर्देश typename Types::HBUINT HBUINT;

  unsigned get_subtable_गणनम् () ध्रुव { प्रत्यागम subtable_गणनम्_2; }

  hb_mask_t compile_flags (ध्रुव hb_aat_map_builder_t *map) ध्रुव
  {
    hb_mask_t flags = defaultFlags;
    {
      unsigned पूर्णाङ्क गणनम् = feature_गणनम्;
      प्रति (unsigned i = 0; i < गणनम्; i++)
      {
	ध्रुव Feature &feature = featureZ[i];
	hb_aat_layout_feature_type_t type = (hb_aat_layout_feature_type_t) (unsigned पूर्णाङ्क) feature.featureType;
	hb_aat_layout_feature_selector_t setting = (hb_aat_layout_feature_selector_t) (unsigned पूर्णाङ्क) feature.featureSetting;
      retry:
	// Check whether this type/setting pair was requested in the map, and if so, apply its flags.
	// (The search here only looks at the type and setting fields of feature_info_t.)
	hb_aat_map_builder_t::feature_info_t info = { type, setting, मिच्छ, 0 };
	जइ (map->current_features.bsearch (info))
	{
	  flags &= feature.disableFlags;
	  flags |= feature.enableFlags;
	}
	अण्णहा जइ (type == HB_AAT_LAYOUT_FEATURE_TYPE_LETTER_CASE && setting == HB_AAT_LAYOUT_FEATURE_SELECTOR_SMALL_CAPS)
	{
	  /* Deprecated. https://github.com/harfbuzz/harfbuzz/issues/1342 */
	  type = HB_AAT_LAYOUT_FEATURE_TYPE_LOWER_CASE;
	  setting = HB_AAT_LAYOUT_FEATURE_SELECTOR_LOWER_CASE_SMALL_CAPS;
	  गच्छ retry;
	}
#ifndef HB_NO_AAT
	अण्णहा जइ (type == HB_AAT_LAYOUT_FEATURE_TYPE_LANGUAGE_TAG_TYPE && setting &&
		 /* TODO: Rudimentary language matching. */
		 hb_language_matches (map->face->table.ltag->get_language (setting - 1), map->props.language))
	{
	  flags &= feature.disableFlags;
	  flags |= feature.enableFlags;
	}
#endif
      }
    }
    प्रत्यागम flags;
  }

  शून्य apply (hb_aat_apply_context_t *c,
	      ध्रुव hb_aat_layout_chain_accelerator_t *accel) ध्रुव
  {
    ध्रुव ChainSubtable<Types> *subtable = &StructAfter<ChainSubtable<Types>> (featureZ.as_array (feature_गणनम्));
    unsigned पूर्णाङ्क गणनम् = subtable_गणनम्_2;
    प्रति (unsigned पूर्णाङ्क i = 0; i < गणनम्; i++)
    {
      तार्किक reverse;

      auto coverage = subtable->get_coverage ();

      hb_mask_t subtable_flags = subtable->subFeatureFlags;
      जइ (hb_none (hb_iter (c->range_flags) |
		   hb_map ([subtable_flags] (ध्रुव hb_aat_map_t::range_flags_t _) -> तार्किक { प्रत्यागम subtable_flags & (_.flags); })))
	गच्छ skip;

      जइ (!(coverage & ChainSubtable<Types>::AllDirections) &&
	  HB_DIRECTION_IS_VERTICAL (c->buffer->props.direction) !=
	  तार्किक (coverage & ChainSubtable<Types>::Vertical))
	गच्छ skip;

      c->subtable_flags = subtable_flags;
      c->first_set = accel ? &accel->subtables[i].glyph_set : &Null(hb_bit_set_t);
      c->machine_class_cache = accel ? &accel->subtables[i].class_cache : nullptr;

      जइ (!c->buffer_intersects_machine ())
      {
	(शून्य) c->buffer->message (c->font, "skipped chainsubtable %u because no glyph matches", c->lookup_index);
	गच्छ skip;
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
		तार्किक (coverage & ChainSubtable<Types>::Backwards) :
		तार्किक (coverage & ChainSubtable<Types>::Backwards) !=
		HB_DIRECTION_IS_BACKWARD (c->buffer->props.direction);

      जइ (!c->buffer->message (c->font, "start chainsubtable %u", c->lookup_index))
	गच्छ skip;

      जइ (reverse != c->buffer_is_reversed)
        c->reverse_buffer ();

      subtable->apply (c);

      (शून्य) c->buffer->message (c->font, "end chainsubtable %u", c->lookup_index);

      जइ (unlikely (!c->buffer->successful)) भङ्ग;

    skip:
      subtable = &StructAfter<ChainSubtable<Types>> (*subtable);
      c->set_lookup_index (c->lookup_index + 1);
    }
    जइ (c->buffer_is_reversed)
      c->reverse_buffer ();
  }

  size_t get_size () ध्रुव { प्रत्यागम length; }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ध्रुव
  {
    ध्रुव ChainSubtable<Types> *subtable = &StructAfter<ChainSubtable<Types>> (featureZ.as_array (feature_गणनम्));
    unsigned पूर्णाङ्क गणनम् = subtable_गणनम्_2;
    प्रति (unsigned पूर्णाङ्क i = 0; i < गणनम्; i++)
    {
      typename context_t::return_t ret = subtable->dispatch (c, std::forward<Ts> (ds)...);
      जइ (c->stop_sublookup_iteration (ret))
	प्रत्यागम ret;
      subtable = &StructAfter<ChainSubtable<Types>> (*subtable);
    }
    प्रत्यागम c->default_return_मानम् ();
  }

  तार्किक sanitize (hb_sanitize_context_t *c, unsigned पूर्णाङ्क version) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    जइ (!(length.sanitize (c) &&
	  hb_barrier () &&
	  length >= min_size &&
	  c->check_range (एतत्, length)))
      return_trace (मिच्छ);

    जइ (!c->check_array (featureZ.arrayZ, feature_गणनम्))
      return_trace (मिच्छ);

    ध्रुव ChainSubtable<Types> *subtable = &StructAfter<ChainSubtable<Types>> (featureZ.as_array (feature_गणनम्));
    unsigned पूर्णाङ्क गणनम् = subtable_गणनम्_2;
    प्रति (unsigned पूर्णाङ्क i = 0; i < गणनम्; i++)
    {
      जइ (!subtable->sanitize (c))
	return_trace (मिच्छ);
      hb_barrier ();
      subtable = &StructAfter<ChainSubtable<Types>> (*subtable);
    }

    जइ (version >= 3)
    {
      ध्रुव SubtableGlyphCoverage *coverage = (ध्रुव SubtableGlyphCoverage *) subtable;
      जइ (!coverage->sanitize (c, गणनम्))
        return_trace (मिच्छ);
    }

    return_trace (सच्च);
  }

  रक्षित:
  HBUINT32	defaultFlags;	/* The default specification for subtables. */
  HBUINT32	length;		/* Total byte count, including this header. */
  HBUINT	feature_गणनम्;	/* Number of feature subtable entries. */
  HBUINT	subtable_गणनम्_2;	/* The number of subtables in the chain. */

  UnsizedArrayOf<Feature>	featureZ;	/* Features. */
/*ChainSubtable	firstSubtable;*//* Subtables. */
/*SubtableGlyphCoverage coverages*//* Only if version >= 3. */

  सार्वजनिक:
  DEFINE_SIZE_MIN (8 + 2 * परिमाण (HBUINT));
};


/*
 * The 'mort'/'morx' Table
 */

template <typename T, typename Types, hb_tag_t TAG>
संरचना mortmorx
{
  स्थिर constexpr hb_tag_t tableTag = TAG;

  तार्किक has_data () ध्रुव { प्रत्यागम version != 0; }

  संरचना accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      hb_sanitize_context_t sc;
      एतत्->table = sc.reference_table<T> (face);

      जइ (unlikely (एतत्->table->is_blocklisted (एतत्->table.get_blob (), face)))
      {
        hb_blob_destroy (एतत्->table.get_blob ());
        एतत्->table = hb_blob_get_empty ();
      }

      एतत्->chain_गणनम् = table->get_chain_गणनम् ();

      एतत्->accels = (hb_atomic_t<hb_aat_layout_chain_accelerator_t *> *) hb_calloc (एतत्->chain_गणनम्, परिमाण (*accels));
      जइ (unlikely (!एतत्->accels))
      {
	एतत्->chain_गणनम् = 0;
	एतत्->table.destroy ();
	एतत्->table = hb_blob_get_empty ();
      }
    }
    ~accelerator_t ()
    {
      प्रति (unsigned पूर्णाङ्क i = 0; i < एतत्->chain_गणनम्; i++)
      {
	जइ (एतत्->accels[i])
	  एतत्->accels[i]->destroy ();
	hb_free (एतत्->accels[i]);
      }
      hb_free (एतत्->accels);
      एतत्->table.destroy ();
    }

    hb_blob_t *get_blob () ध्रुव { प्रत्यागम table.get_blob (); }

    template <typename Chain>
    hb_aat_layout_chain_accelerator_t *get_accel (unsigned chain_index, ध्रुव Chain &chain, unsigned अङ्कः_glyphs) ध्रुव
    {
      जइ (unlikely (chain_index >= chain_गणनम्)) प्रत्यागम nullptr;

    retry:
      auto *accel = accels[chain_index].get_acquire ();
      जइ (unlikely (!accel))
      {
	accel = hb_aat_layout_chain_accelerator_t::create (chain, अङ्कः_glyphs);
	जइ (unlikely (!accel))
	  प्रत्यागम nullptr;

	जइ (unlikely (!accels[chain_index].cmpexch (nullptr, accel)))
	{
	  hb_free (accel);
	  गच्छ retry;
	}
      }

      प्रत्यागम accel;
    }

    hb_blob_ptr_t<T> table;
    unsigned पूर्णाङ्क chain_गणनम्;
    hb_atomic_t<hb_aat_layout_chain_accelerator_t *> *accels;
    hb_aat_scratch_t scratch;
  };


  शून्य compile_flags (ध्रुव hb_aat_map_builder_t *mapper,
		      hb_aat_map_t *map) ध्रुव
  {
    ध्रुव Chain<Types> *chain = &firstChain;
    unsigned पूर्णाङ्क गणनम् = chain_गणनम्_2;
    जइ (unlikely (!map->chain_flags.resize (गणनम्)))
      प्रत्यागम;
    प्रति (unsigned पूर्णाङ्क i = 0; i < गणनम्; i++)
    {
      map->chain_flags[i].push (hb_aat_map_t::range_flags_t {chain->compile_flags (mapper),
							     mapper->range_first,
							     mapper->range_last});
      chain = &StructAfter<Chain<Types>> (*chain);
    }
  }

  unsigned get_chain_गणनम् () ध्रुव
  {
    प्रत्यागम chain_गणनम्_2;
  }
  शून्य apply (hb_aat_apply_context_t *c,
	      ध्रुव hb_aat_map_t &map,
	      ध्रुव accelerator_t &accel) ध्रुव
  {
    जइ (unlikely (!c->buffer->successful)) प्रत्यागम;

    c->buffer->unsafe_to_concat ();

    c->setup_buffer_glyph_set ();

    c->set_lookup_index (0);
    ध्रुव Chain<Types> *chain = &firstChain;
    unsigned पूर्णाङ्क गणनम् = chain_गणनम्_2;
    प्रति (unsigned पूर्णाङ्क i = 0; i < गणनम्; i++)
    {
      auto *chain_accel = accel.get_accel (i, *chain, c->face->get_अङ्कः_glyphs ());
      c->range_flags = &map.chain_flags[i];
      chain->apply (c, chain_accel);
      जइ (unlikely (!c->buffer->successful)) प्रत्यागम;
      chain = &StructAfter<Chain<Types>> (*chain);
    }
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    जइ (!(version.sanitize (c) &&
	  hb_barrier () &&
	  version &&
	  chain_गणनम्_2.sanitize (c)))
      return_trace (मिच्छ);

    ध्रुव Chain<Types> *chain = &firstChain;
    unsigned पूर्णाङ्क गणनम् = chain_गणनम्_2;
    प्रति (unsigned पूर्णाङ्क i = 0; i < गणनम्; i++)
    {
      जइ (!chain->sanitize (c, version))
	return_trace (मिच्छ);
      hb_barrier ();
      chain = &StructAfter<Chain<Types>> (*chain);
    }

    return_trace (सच्च);
  }

  रक्षित:
  HBUINT16	version;	/* Version number of the glyph metamorphosis table.
				 * 1, 2, or 3. */
  HBUINT16	unused;		/* Set to 0. */
  HBUINT32	chain_गणनम्_2;	/* Number of metamorphosis chains contained in this
				 * table. */
  Chain<Types>	firstChain;	/* Chains. */

  सार्वजनिक:
  DEFINE_SIZE_MIN (8);
};

संरचना morx : mortmorx<morx, ExtendedTypes, HB_AAT_TAG_morx>
{
  HB_INTERNAL तार्किक is_blocklisted (hb_blob_t *blob,
                                   hb_face_t *face) ध्रुव;
};

संरचना mort : mortmorx<mort, ObsoleteTypes, HB_AAT_TAG_mort>
{
  HB_INTERNAL तार्किक is_blocklisted (hb_blob_t *blob,
                                   hb_face_t *face) ध्रुव;
};

संरचना morx_accelerator_t : morx::accelerator_t {
  morx_accelerator_t (hb_face_t *face) : morx::accelerator_t (face) {}
};
संरचना mort_accelerator_t : mort::accelerator_t {
  mort_accelerator_t (hb_face_t *face) : mort::accelerator_t (face) {}
};


} /* namespace AAT */


#endif /* HB_AAT_LAYOUT_MORX_TABLE_HH */
