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

ढाँचा <typename Types>
संरचना RearrangementSubtable
{
  प्रकारव्याख्या typename Types::HBUINT HBUINT;

  प्रकारव्याख्या शून्य EntryData;

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

  तार्किक is_action_initiable (स्थिरांक Entry<EntryData> &entry) स्थिरांक
  {
    परत (entry.flags & MarkFirst);
  }
  तार्किक is_actionable (स्थिरांक Entry<EntryData> &entry) स्थिरांक
  {
    परत (entry.flags & Verb);
  }

  संरचना driver_context_t
  {
    स्थिर constexpr तार्किक in_place = सत्य;

    driver_context_t (स्थिरांक RearrangementSubtable *table_) :
	ret (असत्य),
	table (table_),
	start (0), end (0) {}

    HB_AAT_TRANSITION_INLINE
    शून्य transition (hb_buffer_t *buffer,
		     StateTableDriver<Types, EntryData, Flags> *driver,
		     स्थिरांक Entry<EntryData> &entry)
    {
      अचिन्हित पूर्णांक flags = entry.flags;

      जर (flags & MarkFirst)
	start = buffer->idx;

      जर (flags & MarkLast)
	end = hb_min (buffer->idx + 1, buffer->len);

      जर ((flags & Verb) && start < end)
      {
	/* The following map has two nibbles, for start-side
	 * and end-side. Values of 0,1,2 mean move that many
	 * to the other side. Value of 3 means move 2 and
	 * flip them. */
	स्थिरांक अचिन्हित अक्षर map[16] =
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

	अचिन्हित पूर्णांक m = map[flags & Verb];
	अचिन्हित पूर्णांक l = hb_min (2u, m >> 4);
	अचिन्हित पूर्णांक r = hb_min (2u, m & 0x0F);
	तार्किक reverse_l = 3 == (m >> 4);
	तार्किक reverse_r = 3 == (m & 0x0F);

	जर (end - start >= l + r && end-start <= HB_MAX_CONTEXT_LENGTH)
	{
	  buffer->merge_clusters (start, hb_min (buffer->idx + 1, buffer->len));
	  buffer->merge_clusters (start, end);

	  hb_glyph_info_t *info = buffer->info;
	  hb_glyph_info_t buf[4];

	  hb_memcpy (buf, info + start, l * आकार (buf[0]));
	  hb_memcpy (buf + 2, info + end - r, r * आकार (buf[0]));

	  जर (l != r)
	    स_हटाओ (info + start + r, info + start + l, (end - start - l - r) * आकार (buf[0]));

	  hb_memcpy (info + start, buf + 2, r * आकार (buf[0]));
	  hb_memcpy (info + end - l, buf, l * आकार (buf[0]));
	  जर (reverse_l)
	  {
	    buf[0] = info[end - 1];
	    info[end - 1] = info[end - 2];
	    info[end - 2] = buf[0];
	  }
	  जर (reverse_r)
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
    स्थिरांक RearrangementSubtable *table;
    खाजगी:
    अचिन्हित पूर्णांक start;
    अचिन्हित पूर्णांक end;
  };

  तार्किक apply (hb_aat_apply_context_t *c) स्थिरांक
  {
    TRACE_APPLY (हे);

    driver_context_t dc (हे);

    StateTableDriver<Types, EntryData, Flags> driver (machine, c->face);

    driver.drive (&dc, c);

    return_trace (dc.ret);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (machine.sanitize (c));
  }

  सार्वजनिक:
  StateTable<Types, EntryData>	machine;
  सार्वजनिक:
  DEFINE_SIZE_STATIC ((StateTable<Types, EntryData>::static_size));
};

ढाँचा <typename Types>
संरचना ContextualSubtable
{
  प्रकारव्याख्या typename Types::HBUINT HBUINT;

  संरचना EntryData
  {
    प्रकारव्याख्या typename std::conditional<Types::extended, HBUINT16, HBINT16>::type OffsetType;

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

  तार्किक is_action_initiable (स्थिरांक Entry<EntryData> &entry) स्थिरांक
  {
    परत (entry.flags & SetMark);
  }
  तार्किक is_actionable (स्थिरांक Entry<EntryData> &entry) स्थिरांक
  {
    जर (Types::extended)
      परत entry.data.markIndex != 0xFFFF || entry.data.currentIndex != 0xFFFF;
    अन्यथा
      परत entry.data.markIndex || entry.data.currentIndex;
  }

  संरचना driver_context_t
  {
    स्थिर constexpr तार्किक in_place = सत्य;

    driver_context_t (स्थिरांक ContextualSubtable *table_,
			     hb_aat_apply_context_t *c_) :
	ret (असत्य),
	c (c_),
	table (table_),
	mark_set (असत्य),
	mark (0),
	subs (table+table->substitutionTables) {}

    HB_AAT_TRANSITION_INLINE
    शून्य transition (hb_buffer_t *buffer,
		     StateTableDriver<Types, EntryData, Flags> *driver,
		     स्थिरांक Entry<EntryData> &entry)
    {
      /* Looks like CoreText applies neither mark nor current substitution for
       * end-of-text if mark was not explicitly set. */
      जर (buffer->idx == buffer->len && !mark_set)
	परत;

      स्थिरांक HBGlyphID16 *replacement;

      replacement = nullptr;
      जर (Types::extended)
      {
	जर (entry.data.markIndex != 0xFFFF)
	{
	  स्थिरांक Lookup<HBGlyphID16> &lookup = subs[entry.data.markIndex];
	  replacement = lookup.get_मूल्य (buffer->info[mark].codepoint, driver->अंक_glyphs);
	}
      }
      अन्यथा
      {
	जर (entry.data.markIndex)
	{
	  पूर्णांक offset = (पूर्णांक) entry.data.markIndex + buffer->info[mark].codepoint;
	  स्थिरांक UnsizedArrayOf<HBGlyphID16> &subs_old = (स्थिरांक UnsizedArrayOf<HBGlyphID16> &) subs;
	  replacement = &subs_old[Types::wordOffsetToIndex (offset, table, subs_old.arrayZ)];
	  जर (!(replacement->sanitize (&c->sanitizer) &&
		hb_barrier () &&
		*replacement))
	    replacement = nullptr;
	}
      }
      जर (replacement)
      {
	buffer->unsafe_to_break (mark, hb_min (buffer->idx + 1, buffer->len));
	c->replace_glyph_inplace (mark, *replacement);
	ret = सत्य;
      }

      replacement = nullptr;
      अचिन्हित पूर्णांक idx = hb_min (buffer->idx, buffer->len - 1);
      जर (Types::extended)
      {
	जर (entry.data.currentIndex != 0xFFFF)
	{
	  स्थिरांक Lookup<HBGlyphID16> &lookup = subs[entry.data.currentIndex];
	  replacement = lookup.get_मूल्य (buffer->info[idx].codepoint, driver->अंक_glyphs);
	}
      }
      अन्यथा
      {
	जर (entry.data.currentIndex)
	{
	  पूर्णांक offset = (पूर्णांक) entry.data.currentIndex + buffer->info[idx].codepoint;
	  स्थिरांक UnsizedArrayOf<HBGlyphID16> &subs_old = (स्थिरांक UnsizedArrayOf<HBGlyphID16> &) subs;
	  replacement = &subs_old[Types::wordOffsetToIndex (offset, table, subs_old.arrayZ)];
	  जर (!(replacement->sanitize (&c->sanitizer) &&
		hb_barrier () &&
		*replacement))
	    replacement = nullptr;
	}
      }
      जर (replacement)
      {
	c->replace_glyph_inplace (idx, *replacement);
	ret = सत्य;
      }

      जर (entry.flags & SetMark)
      {
	mark_set = सत्य;
	mark = buffer->idx;
      }
    }

    सार्वजनिक:
    तार्किक ret;
    hb_aat_apply_context_t *c;
    स्थिरांक ContextualSubtable *table;
    खाजगी:
    तार्किक mark_set;
    अचिन्हित पूर्णांक mark;
    स्थिरांक UnsizedListOfOffset16To<Lookup<HBGlyphID16>, HBUINT, शून्य, असत्य> &subs;
  };

  तार्किक apply (hb_aat_apply_context_t *c) स्थिरांक
  {
    TRACE_APPLY (हे);

    driver_context_t dc (हे, c);

    StateTableDriver<Types, EntryData, Flags> driver (machine, c->face);

    driver.drive (&dc, c);

    return_trace (dc.ret);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);

    अचिन्हित पूर्णांक अंक_entries = 0;
    जर (unlikely (!machine.sanitize (c, &अंक_entries))) return_trace (असत्य);
    hb_barrier ();

    जर (!Types::extended)
      return_trace (substitutionTables.sanitize (c, हे, 0));

    अचिन्हित पूर्णांक अंक_lookups = 0;

    स्थिरांक Entry<EntryData> *entries = machine.get_entries ();
    साठी (अचिन्हित पूर्णांक i = 0; i < अंक_entries; i++)
    {
      स्थिरांक EntryData &data = entries[i].data;

      जर (data.markIndex != 0xFFFF)
	अंक_lookups = hb_max (अंक_lookups, 1u + data.markIndex);
      जर (data.currentIndex != 0xFFFF)
	अंक_lookups = hb_max (अंक_lookups, 1u + data.currentIndex);
    }

    return_trace (substitutionTables.sanitize (c, हे, अंक_lookups));
  }

  सार्वजनिक:
  StateTable<Types, EntryData>
		machine;
  संरक्षित:
  NNOffsetTo<UnsizedListOfOffset16To<Lookup<HBGlyphID16>, HBUINT, शून्य, असत्य>, HBUINT>
		substitutionTables;
  सार्वजनिक:
  DEFINE_SIZE_STATIC ((StateTable<Types, EntryData>::static_size + HBUINT::static_size));
};


ढाँचा <तार्किक extended>
संरचना LigatureEntry;

ढाँचा <>
संरचना LigatureEntry<सत्य>
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

  स्थिर तार्किक initiateAction (स्थिरांक Entry<EntryData> &entry)
  { परत entry.flags & SetComponent; }

  स्थिर तार्किक performAction (स्थिरांक Entry<EntryData> &entry)
  { परत entry.flags & PerformAction; }

  स्थिर अचिन्हित पूर्णांक ligActionIndex (स्थिरांक Entry<EntryData> &entry)
  { परत entry.data.ligActionIndex; }
};
ढाँचा <>
संरचना LigatureEntry<असत्य>
{
  प्रकारव्याख्या शून्य EntryData;

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

  स्थिर तार्किक initiateAction (स्थिरांक Entry<EntryData> &entry)
  { परत entry.flags & SetComponent; }

  स्थिर तार्किक performAction (स्थिरांक Entry<EntryData> &entry)
  { परत entry.flags & Offset; }

  स्थिर अचिन्हित पूर्णांक ligActionIndex (स्थिरांक Entry<EntryData> &entry)
  { परत entry.flags & Offset; }
};


ढाँचा <typename Types>
संरचना LigatureSubtable
{
  प्रकारव्याख्या typename Types::HBUINT HBUINT;

  प्रकारव्याख्या LigatureEntry<Types::extended> LigatureEntryT;
  प्रकारव्याख्या typename LigatureEntryT::EntryData EntryData;

  गणना Flags
  {
    DontAdvance	= LigatureEntryT::DontAdvance,
  };

  तार्किक is_action_initiable (स्थिरांक Entry<EntryData> &entry) स्थिरांक
  {
    परत LigatureEntryT::initiateAction (entry);
  }
  तार्किक is_actionable (स्थिरांक Entry<EntryData> &entry) स्थिरांक
  {
    परत LigatureEntryT::performAction (entry);
  }

  संरचना driver_context_t
  {
    स्थिर constexpr तार्किक in_place = असत्य;
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

    driver_context_t (स्थिरांक LigatureSubtable *table_,
		      hb_aat_apply_context_t *c_) :
	ret (असत्य),
	c (c_),
	table (table_),
	ligAction (table+table->ligAction),
	component (table+table->component),
	ligature (table+table->ligature),
	match_length (0) {}

    HB_AAT_TRANSITION_INLINE
    शून्य transition (hb_buffer_t *buffer,
		     StateTableDriver<Types, EntryData, Flags> *driver,
		     स्थिरांक Entry<EntryData> &entry)
    {
      DEBUG_संदेश (APPLY, nullptr, "Ligature transition at %u", buffer->idx);
      जर (entry.flags & LigatureEntryT::SetComponent)
      {
	/* Never mark same index twice, in case DontAdvance was used... */
	जर (unlikely (match_length && match_positions[(match_length - 1u) % ARRAY_LENGTH (match_positions)] == buffer->out_len))
	  match_length--;

	match_positions[match_length++ % ARRAY_LENGTH (match_positions)] = buffer->out_len;
	DEBUG_संदेश (APPLY, nullptr, "Set component at %u", buffer->out_len);
      }

      जर (LigatureEntryT::performAction (entry))
      {
	DEBUG_संदेश (APPLY, nullptr, "Perform action with %u", match_length);
	अचिन्हित पूर्णांक end = buffer->out_len;

	जर (unlikely (!match_length))
	  परत;

	जर (buffer->idx >= buffer->len)
	  परत; /* TODO Work on previous instead? */

	अचिन्हित पूर्णांक cursor = match_length;

	अचिन्हित पूर्णांक action_idx = LigatureEntryT::ligActionIndex (entry);
	action_idx = Types::offsetToIndex (action_idx, table, ligAction.arrayZ);
	स्थिरांक HBUINT32 *actionData = &ligAction[action_idx];

	अचिन्हित पूर्णांक ligature_idx = 0;
	अचिन्हित पूर्णांक action;
	करा
	{
	  जर (unlikely (!cursor))
	  {
	    /* Stack underflow.  Clear the stack. */
	    DEBUG_संदेश (APPLY, nullptr, "Stack underflow");
	    match_length = 0;
	    थांबा;
	  }

	  DEBUG_संदेश (APPLY, nullptr, "Moving to stack position %u", cursor - 1);
	  जर (unlikely (!buffer->move_to (match_positions[--cursor % ARRAY_LENGTH (match_positions)]))) परत;

	  जर (unlikely (!actionData->sanitize (&c->sanitizer))) थांबा;
	  hb_barrier ();
	  action = *actionData;

	  uint32_t uoffset = action & LigActionOffset;
	  जर (uoffset & 0x20000000)
	    uoffset |= 0xC0000000; /* Sign-extend. */
	  int32_t offset = (int32_t) uoffset;
	  अचिन्हित पूर्णांक component_idx = buffer->cur().codepoint + offset;
	  component_idx = Types::wordOffsetToIndex (component_idx, table, component.arrayZ);
	  स्थिरांक HBUINT16 &componentData = component[component_idx];
	  जर (unlikely (!componentData.sanitize (&c->sanitizer))) थांबा;
	  hb_barrier ();
	  ligature_idx += componentData;

	  DEBUG_संदेश (APPLY, nullptr, "Action store %d last %d",
		     तार्किक (action & LigActionStore),
		     तार्किक (action & LigActionLast));
	  जर (action & (LigActionStore | LigActionLast))
	  {
	    ligature_idx = Types::offsetToIndex (ligature_idx, table, ligature.arrayZ);
	    स्थिरांक HBGlyphID16 &ligatureData = ligature[ligature_idx];
	    जर (unlikely (!ligatureData.sanitize (&c->sanitizer))) थांबा;
	    hb_barrier ();
	    hb_codepoint_t lig = ligatureData;

	    DEBUG_संदेश (APPLY, nullptr, "Produced ligature %u", lig);
	    जर (unlikely (!c->replace_glyph (lig))) परत;

	    अचिन्हित पूर्णांक lig_end = match_positions[(match_length - 1u) % ARRAY_LENGTH (match_positions)] + 1u;
	    /* Now go and delete all subsequent components. */
	    पर्यंत (match_length - 1u > cursor)
	    {
	      DEBUG_संदेश (APPLY, nullptr, "Skipping ligature component");
	      जर (unlikely (!buffer->move_to (match_positions[--match_length % ARRAY_LENGTH (match_positions)]))) परत;
	      जर (!c->delete_glyph ()) परत;
	    }

	    जर (unlikely (!buffer->move_to (lig_end))) परत;
	    buffer->merge_out_clusters (match_positions[cursor % ARRAY_LENGTH (match_positions)], buffer->out_len);
	  }

	  actionData++;
	}
	पर्यंत (!(action & LigActionLast));
	जर (unlikely (!buffer->move_to (end))) परत;
      }
    }

    सार्वजनिक:
    तार्किक ret;
    hb_aat_apply_context_t *c;
    स्थिरांक LigatureSubtable *table;
    खाजगी:
    स्थिरांक UnsizedArrayOf<HBUINT32> &ligAction;
    स्थिरांक UnsizedArrayOf<HBUINT16> &component;
    स्थिरांक UnsizedArrayOf<HBGlyphID16> &ligature;
    अचिन्हित पूर्णांक match_length;
    अचिन्हित पूर्णांक match_positions[HB_MAX_CONTEXT_LENGTH];
  };

  तार्किक apply (hb_aat_apply_context_t *c) स्थिरांक
  {
    TRACE_APPLY (हे);

    driver_context_t dc (हे, c);

    StateTableDriver<Types, EntryData, Flags> driver (machine, c->face);

    driver.drive (&dc, c);

    return_trace (dc.ret);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    /* The rest of array sanitizations are done at run-time. */
    return_trace (c->check_struct (हे) && machine.sanitize (c) &&
		  hb_barrier () &&
		  ligAction && component && ligature);
  }

  सार्वजनिक:
  StateTable<Types, EntryData>
		machine;
  संरक्षित:
  NNOffsetTo<UnsizedArrayOf<HBUINT32>, HBUINT>
		ligAction;	/* Offset to the ligature action table. */
  NNOffsetTo<UnsizedArrayOf<HBUINT16>, HBUINT>
		component;	/* Offset to the component table. */
  NNOffsetTo<UnsizedArrayOf<HBGlyphID16>, HBUINT>
		ligature;	/* Offset to the actual ligature lists. */
  सार्वजनिक:
  DEFINE_SIZE_STATIC ((StateTable<Types, EntryData>::static_size + 3 * HBUINT::static_size));
};

ढाँचा <typename Types>
संरचना NoncontextualSubtable
{
  तार्किक apply (hb_aat_apply_context_t *c) स्थिरांक
  {
    TRACE_APPLY (हे);

    तार्किक ret = असत्य;
    अचिन्हित पूर्णांक अंक_glyphs = c->face->get_अंक_glyphs ();

    hb_glyph_info_t *info = c->buffer->info;
    अचिन्हित पूर्णांक मोजणी = c->buffer->len;
    // If there's only one range, we already checked the flag.
    स्वतः *last_range = c->range_flags && (c->range_flags->length > 1) ? &(*c->range_flags)[0] : nullptr;
    साठी (अचिन्हित पूर्णांक i = 0; i < मोजणी; i++)
    {
      /* This block copied from StateTableDriver::drive. Keep in sync. */
      जर (unlikely (last_range))
      {
	स्वतः *range = last_range;
	{
	  अचिन्हित cluster = info[i].cluster;
	  पर्यंत (cluster < range->cluster_first)
	    range--;
	  पर्यंत (cluster > range->cluster_last)
	    range++;

	  last_range = range;
	}
	जर (!(range->flags & c->subtable_flags))
	  सुरूठेवा;
      }

      स्थिरांक HBGlyphID16 *replacement = substitute.get_मूल्य (info[i].codepoint, अंक_glyphs);
      जर (replacement)
      {
	c->replace_glyph_inplace (i, *replacement);
	ret = सत्य;
      }
    }

    return_trace (ret);
  }

  ढाँचा <typename set_t>
  शून्य collect_initial_glyphs (set_t &glyphs, अचिन्हित अंक_glyphs) स्थिरांक
  {
    substitute.collect_glyphs (glyphs, अंक_glyphs);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (substitute.sanitize (c));
  }

  संरक्षित:
  Lookup<HBGlyphID16>	substitute;
  सार्वजनिक:
  DEFINE_SIZE_MIN (2);
};

ढाँचा <typename Types>
संरचना InsertionSubtable
{
  प्रकारव्याख्या typename Types::HBUINT HBUINT;

  संरचना EntryData
  {
    HBUINT16	current_घाला_Index;	/* Zero-based index into the insertion glyph table.
					 * The number of glyphs to be inserted is contained
					 * in the currentInsertCount field in the flags.
					 * A value of 0xFFFF indicates no insertion is to
					 * be done. */
    HBUINT16	marked_घाला_Index;	/* Zero-based index into the insertion glyph table.
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
    Current_घाला_Before= 0x0800,      /* If set, specifies that insertions are to be made
				       * to the left of the current glyph. If clear,
				       * they're made to the right of the current glyph. */
    Marked_घाला_Before= 0x0400,	      /* If set, specifies that insertions are to be
				       * made to the left of the marked glyph. If clear,
				       * they're made to the right of the marked glyph. */
    Current_घाला_मोजणी= 0x3E0,	      /* This 5-bit field is treated as a count of the
				       * number of glyphs to insert at the current
				       * position. Since zero means no insertions, the
				       * largest number of insertions at any given
				       * current location is 31 glyphs. */
    Marked_घाला_मोजणी= 0x001F,	      /* This 5-bit field is treated as a count of the
				       * number of glyphs to insert at the marked
				       * position. Since zero means no insertions, the
				       * largest number of insertions at any given
				       * marked location is 31 glyphs. */
  };

  तार्किक is_action_initiable (स्थिरांक Entry<EntryData> &entry) स्थिरांक
  {
    परत (entry.flags & SetMark);
  }
  तार्किक is_actionable (स्थिरांक Entry<EntryData> &entry) स्थिरांक
  {
    परत (entry.flags & (Current_घाला_मोजणी | Marked_घाला_मोजणी)) &&
	   (entry.data.current_घाला_Index != 0xFFFF ||entry.data.marked_घाला_Index != 0xFFFF);
  }

  संरचना driver_context_t
  {
    स्थिर constexpr तार्किक in_place = असत्य;

    driver_context_t (स्थिरांक InsertionSubtable *table_,
		      hb_aat_apply_context_t *c_) :
	ret (असत्य),
	c (c_),
	table (table_),
	mark (0),
	insertionAction (table+table->insertionAction) {}

    HB_AAT_TRANSITION_INLINE
    शून्य transition (hb_buffer_t *buffer,
		     StateTableDriver<Types, EntryData, Flags> *driver,
		     स्थिरांक Entry<EntryData> &entry)
    {
      अचिन्हित पूर्णांक flags = entry.flags;

      अचिन्हित mark_loc = buffer->out_len;

      जर (entry.data.marked_घाला_Index != 0xFFFF)
      {
	अचिन्हित पूर्णांक मोजणी = (flags & Marked_घाला_मोजणी);
	जर (unlikely ((buffer->max_ops -= मोजणी) <= 0)) परत;
	अचिन्हित पूर्णांक start = entry.data.marked_घाला_Index;
	स्थिरांक HBGlyphID16 *glyphs = &insertionAction[start];
	जर (unlikely (!c->sanitizer.check_array (glyphs, मोजणी))) मोजणी = 0;
	hb_barrier ();

	तार्किक before = flags & Marked_घाला_Before;

	अचिन्हित पूर्णांक end = buffer->out_len;
	जर (unlikely (!buffer->move_to (mark))) परत;

	जर (buffer->idx < buffer->len && !before)
	  जर (unlikely (!buffer->copy_glyph ())) परत;
	/* TODO We ignore KashidaLike setting. */
	जर (unlikely (!c->output_glyphs (मोजणी, glyphs))) परत;
	ret = सत्य;
	जर (buffer->idx < buffer->len && !before)
	  buffer->skip_glyph ();

	जर (unlikely (!buffer->move_to (end + मोजणी))) परत;

	buffer->unsafe_to_break_from_outbuffer (mark, hb_min (buffer->idx + 1, buffer->len));
      }

      जर (flags & SetMark)
	mark = mark_loc;

      जर (entry.data.current_घाला_Index != 0xFFFF)
      {
	अचिन्हित पूर्णांक मोजणी = (flags & Current_घाला_मोजणी) >> 5;
	जर (unlikely ((buffer->max_ops -= मोजणी) <= 0)) परत;
	अचिन्हित पूर्णांक start = entry.data.current_घाला_Index;
	स्थिरांक HBGlyphID16 *glyphs = &insertionAction[start];
	जर (unlikely (!c->sanitizer.check_array (glyphs, मोजणी))) मोजणी = 0;
	hb_barrier ();

	तार्किक before = flags & Current_घाला_Before;

	अचिन्हित पूर्णांक end = buffer->out_len;

	जर (buffer->idx < buffer->len && !before)
	  जर (unlikely (!buffer->copy_glyph ())) परत;
	/* TODO We ignore KashidaLike setting. */
	जर (unlikely (!c->output_glyphs (मोजणी, glyphs))) परत;
	ret = सत्य;
	जर (buffer->idx < buffer->len && !before)
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
	जर (unlikely (!buffer->move_to ((flags & DontAdvance) ? end : end + मोजणी))) परत;
      }
    }

    सार्वजनिक:
    तार्किक ret;
    hb_aat_apply_context_t *c;
    स्थिरांक InsertionSubtable *table;
    खाजगी:
    अचिन्हित पूर्णांक mark;
    स्थिरांक UnsizedArrayOf<HBGlyphID16> &insertionAction;
  };

  तार्किक apply (hb_aat_apply_context_t *c) स्थिरांक
  {
    TRACE_APPLY (हे);

    driver_context_t dc (हे, c);

    StateTableDriver<Types, EntryData, Flags> driver (machine, c->face);

    driver.drive (&dc, c);

    return_trace (dc.ret);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    /* The rest of array sanitizations are done at run-time. */
    return_trace (c->check_struct (हे) && machine.sanitize (c) &&
		  hb_barrier () &&
		  insertionAction);
  }

  सार्वजनिक:
  StateTable<Types, EntryData>
		machine;
  संरक्षित:
  NNOffsetTo<UnsizedArrayOf<HBGlyphID16>, HBUINT>
		insertionAction;	/* Byte offset from stateHeader to the start of
					 * the insertion glyph table. */
  सार्वजनिक:
  DEFINE_SIZE_STATIC ((StateTable<Types, EntryData>::static_size + HBUINT::static_size));
};


संरचना Feature
{
  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे));
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
    मित्र संरचना hb_accelerate_subtables_context_t;
    मित्र संरचना hb_aat_layout_lookup_accelerator_t;

    सार्वजनिक:
    hb_bit_set_t glyph_set;
    mutable hb_aat_class_cache_t class_cache;

    ढाँचा <typename T>
    स्वतः init_ (स्थिरांक T &obj_, अचिन्हित अंक_glyphs, hb_priority<1>) HB_AUTO_RETURN
    (
      obj_.machine.collect_initial_glyphs (glyph_set, अंक_glyphs, obj_)
    )

    ढाँचा <typename T>
    शून्य init_ (स्थिरांक T &obj_, अचिन्हित अंक_glyphs, hb_priority<0>)
    {
      obj_.collect_initial_glyphs (glyph_set, अंक_glyphs);
    }

    ढाँचा <typename T>
    शून्य init (स्थिरांक T &obj_, अचिन्हित अंक_glyphs)
    {
      glyph_set.init ();
      init_ (obj_, अंक_glyphs, hb_prioritize);
      class_cache.clear ();
    }

    शून्य
    fini ()
    {
      glyph_set.fini ();
    }
  };

  /* Dispatch interface. */
  ढाँचा <typename T>
  return_t dispatch (स्थिरांक T &obj)
  {
    hb_applicable_t *entry = &array[i++];

    entry->init (obj, अंक_glyphs);

    परत hb_empty_t ();
  }
  स्थिर return_t default_return_मूल्य () { परत hb_empty_t (); }

  तार्किक stop_sublookup_iteration (return_t r) स्थिरांक { परत असत्य; }

  hb_accelerate_subtables_context_t (hb_applicable_t *array_, अचिन्हित अंक_glyphs_2) :
				     hb_dispatch_context_t<hb_accelerate_subtables_context_t> (),
				     array (array_), अंक_glyphs (अंक_glyphs_2) {}

  hb_applicable_t *array;
  अचिन्हित अंक_glyphs;
  अचिन्हित i = 0;
};

संरचना hb_aat_layout_chain_accelerator_t
{
  ढाँचा <typename TChain>
  स्थिर hb_aat_layout_chain_accelerator_t *create (स्थिरांक TChain &chain, अचिन्हित अंक_glyphs)
  {
    अचिन्हित मोजणी = chain.get_subtable_मोजणी ();

    अचिन्हित product;
    जर (unlikely (hb_unsigned_mul_overflows (मोजणी,
        आकार (hb_accelerate_subtables_context_t::hb_applicable_t), &product)))
      परत nullptr;

    अचिन्हित size;
    जर (unlikely (hb_unsigned_add_overflows (आकार (hb_aat_layout_chain_accelerator_t) -
        HB_VAR_ARRAY * आकार (hb_accelerate_subtables_context_t::hb_applicable_t), product, &size))) {
      परत nullptr;
    }

    /* The following is a calloc because when we are collecting subtables,
     * some of them might be invalid and hence not collect; as a result,
     * we might not fill in all the count entries of the subtables array.
     * Zeroing it allows the set digest to gatekeep it without having to
     * initialize it further. */
    स्वतः *thiz = (hb_aat_layout_chain_accelerator_t *) hb_calloc (1, size);
    जर (unlikely (!thiz))
      परत nullptr;

    thiz->मोजणी = मोजणी;

    hb_accelerate_subtables_context_t c_accelerate_subtables (thiz->subtables, अंक_glyphs);
    chain.dispatch (&c_accelerate_subtables);

    परत thiz;
  }

  शून्य destroy ()
  {
    साठी (अचिन्हित i = 0; i < मोजणी; i++)
      subtables[i].fini ();
  }

  अचिन्हित मोजणी;
  hb_accelerate_subtables_context_t::hb_applicable_t subtables[HB_VAR_ARRAY];
};

ढाँचा <typename Types>
संरचना ChainSubtable
{
  प्रकारव्याख्या typename Types::HBUINT HBUINT;

  ढाँचा <typename T>
  मित्र संरचना Chain;

  माप_प्रकार get_size () स्थिरांक     { परत length; }
  अचिन्हित पूर्णांक get_type () स्थिरांक     { परत coverage & 0xFF; }
  अचिन्हित पूर्णांक get_coverage () स्थिरांक { परत coverage >> (आकार (HBUINT) * 8 - 8); }

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

  ढाँचा <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) स्थिरांक
  {
    अचिन्हित पूर्णांक subtable_type = get_type ();
    TRACE_DISPATCH (हे, subtable_type);
    निवड (subtable_type) {
    स्थिती Rearrangement:		hb_barrier (); return_trace (c->dispatch (u.rearrangement, std::forward<Ts> (ds)...));
    स्थिती Contextual:		hb_barrier (); return_trace (c->dispatch (u.contextual, std::forward<Ts> (ds)...));
    स्थिती Ligature:		hb_barrier (); return_trace (c->dispatch (u.ligature, std::forward<Ts> (ds)...));
    स्थिती Noncontextual:		hb_barrier (); return_trace (c->dispatch (u.noncontextual, std::forward<Ts> (ds)...));
    स्थिती Insertion:		hb_barrier (); return_trace (c->dispatch (u.insertion, std::forward<Ts> (ds)...));
    पूर्वनिर्धारित:			return_trace (c->default_return_मूल्य ());
    }
  }

  तार्किक apply (hb_aat_apply_context_t *c) स्थिरांक
  {
    TRACE_APPLY (हे);
    // Disabled for https://github.com/harfbuzz/harfbuzz/issues/4873
    //hb_sanitize_with_object_t with (&c->sanitizer, this);
    return_trace (dispatch (c));
  }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    जर (!(length.sanitize (c) &&
	  hb_barrier () &&
	  length >= min_size &&
	  c->check_range (हे, length)))
      return_trace (असत्य);

    // Disabled for https://github.com/harfbuzz/harfbuzz/issues/4873
    //hb_sanitize_with_object_t with (c, this);
    return_trace (dispatch (c));
  }

  संरक्षित:
  HBUINT	length;		/* Total subtable length, including this header. */
  HBUINT	coverage;	/* Coverage flags and subtable type. */
  HBUINT32	subFeatureFlags;/* The 32-bit mask identifying which subtable this is. */
  संघ {
  RearrangementSubtable<Types>	rearrangement;
  ContextualSubtable<Types>	contextual;
  LigatureSubtable<Types>	ligature;
  NoncontextualSubtable<Types>	noncontextual;
  InsertionSubtable<Types>	insertion;
  } u;
  सार्वजनिक:
  DEFINE_SIZE_MIN (2 * आकार (HBUINT) + 4);
};

ढाँचा <typename Types>
संरचना Chain
{
  प्रकारव्याख्या typename Types::HBUINT HBUINT;

  अचिन्हित get_subtable_मोजणी () स्थिरांक { परत subtable_मोजणी_2; }

  hb_mask_t compile_flags (स्थिरांक hb_aat_map_builder_t *map) स्थिरांक
  {
    hb_mask_t flags = defaultFlags;
    {
      अचिन्हित पूर्णांक मोजणी = feature_मोजणी;
      साठी (अचिन्हित i = 0; i < मोजणी; i++)
      {
	स्थिरांक Feature &feature = featureZ[i];
	hb_aat_layout_feature_type_t type = (hb_aat_layout_feature_type_t) (अचिन्हित पूर्णांक) feature.featureType;
	hb_aat_layout_feature_selector_t setting = (hb_aat_layout_feature_selector_t) (अचिन्हित पूर्णांक) feature.featureSetting;
      retry:
	// Check whether this type/setting pair was requested in the map, and if so, apply its flags.
	// (The search here only looks at the type and setting fields of feature_info_t.)
	hb_aat_map_builder_t::feature_info_t info = { type, setting, असत्य, 0 };
	जर (map->current_features.द्वा_खोज (info))
	{
	  flags &= feature.disableFlags;
	  flags |= feature.enableFlags;
	}
	अन्यथा जर (type == HB_AAT_LAYOUT_FEATURE_TYPE_LETTER_CASE && setting == HB_AAT_LAYOUT_FEATURE_SELECTOR_SMALL_CAPS)
	{
	  /* Deprecated. https://github.com/harfbuzz/harfbuzz/issues/1342 */
	  type = HB_AAT_LAYOUT_FEATURE_TYPE_LOWER_CASE;
	  setting = HB_AAT_LAYOUT_FEATURE_SELECTOR_LOWER_CASE_SMALL_CAPS;
	  जा retry;
	}
#ifndef HB_NO_AAT
	अन्यथा जर (type == HB_AAT_LAYOUT_FEATURE_TYPE_LANGUAGE_TAG_TYPE && setting &&
		 /* TODO: Rudimentary language matching. */
		 hb_language_matches (map->face->table.ltag->get_language (setting - 1), map->props.language))
	{
	  flags &= feature.disableFlags;
	  flags |= feature.enableFlags;
	}
#endif
      }
    }
    परत flags;
  }

  शून्य apply (hb_aat_apply_context_t *c,
	      स्थिरांक hb_aat_layout_chain_accelerator_t *accel) स्थिरांक
  {
    स्थिरांक ChainSubtable<Types> *subtable = &StructAfter<ChainSubtable<Types>> (featureZ.as_array (feature_मोजणी));
    अचिन्हित पूर्णांक मोजणी = subtable_मोजणी_2;
    साठी (अचिन्हित पूर्णांक i = 0; i < मोजणी; i++)
    {
      तार्किक reverse;

      स्वतः coverage = subtable->get_coverage ();

      hb_mask_t subtable_flags = subtable->subFeatureFlags;
      जर (hb_none (hb_iter (c->range_flags) |
		   hb_map ([subtable_flags] (स्थिरांक hb_aat_map_t::range_flags_t _) -> तार्किक { परत subtable_flags & (_.flags); })))
	जा skip;

      जर (!(coverage & ChainSubtable<Types>::AllDirections) &&
	  HB_DIRECTION_IS_VERTICAL (c->buffer->props.direction) !=
	  तार्किक (coverage & ChainSubtable<Types>::Vertical))
	जा skip;

      c->subtable_flags = subtable_flags;
      c->first_set = accel ? &accel->subtables[i].glyph_set : &Null(hb_bit_set_t);
      c->machine_class_cache = accel ? &accel->subtables[i].class_cache : nullptr;

      जर (!c->buffer_intersects_machine ())
      {
	(शून्य) c->buffer->message (c->font, "skipped chainsubtable %u because no glyph matches", c->lookup_index);
	जा skip;
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

      जर (!c->buffer->message (c->font, "start chainsubtable %u", c->lookup_index))
	जा skip;

      जर (reverse != c->buffer_is_reversed)
        c->reverse_buffer ();

      subtable->apply (c);

      (शून्य) c->buffer->message (c->font, "end chainsubtable %u", c->lookup_index);

      जर (unlikely (!c->buffer->successful)) थांबा;

    skip:
      subtable = &StructAfter<ChainSubtable<Types>> (*subtable);
      c->set_lookup_index (c->lookup_index + 1);
    }
    जर (c->buffer_is_reversed)
      c->reverse_buffer ();
  }

  माप_प्रकार get_size () स्थिरांक { परत length; }

  ढाँचा <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) स्थिरांक
  {
    स्थिरांक ChainSubtable<Types> *subtable = &StructAfter<ChainSubtable<Types>> (featureZ.as_array (feature_मोजणी));
    अचिन्हित पूर्णांक मोजणी = subtable_मोजणी_2;
    साठी (अचिन्हित पूर्णांक i = 0; i < मोजणी; i++)
    {
      typename context_t::return_t ret = subtable->dispatch (c, std::forward<Ts> (ds)...);
      जर (c->stop_sublookup_iteration (ret))
	परत ret;
      subtable = &StructAfter<ChainSubtable<Types>> (*subtable);
    }
    परत c->default_return_मूल्य ();
  }

  तार्किक sanitize (hb_sanitize_context_t *c, अचिन्हित पूर्णांक version) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    जर (!(length.sanitize (c) &&
	  hb_barrier () &&
	  length >= min_size &&
	  c->check_range (हे, length)))
      return_trace (असत्य);

    जर (!c->check_array (featureZ.arrayZ, feature_मोजणी))
      return_trace (असत्य);

    स्थिरांक ChainSubtable<Types> *subtable = &StructAfter<ChainSubtable<Types>> (featureZ.as_array (feature_मोजणी));
    अचिन्हित पूर्णांक मोजणी = subtable_मोजणी_2;
    साठी (अचिन्हित पूर्णांक i = 0; i < मोजणी; i++)
    {
      जर (!subtable->sanitize (c))
	return_trace (असत्य);
      hb_barrier ();
      subtable = &StructAfter<ChainSubtable<Types>> (*subtable);
    }

    जर (version >= 3)
    {
      स्थिरांक SubtableGlyphCoverage *coverage = (स्थिरांक SubtableGlyphCoverage *) subtable;
      जर (!coverage->sanitize (c, मोजणी))
        return_trace (असत्य);
    }

    return_trace (सत्य);
  }

  संरक्षित:
  HBUINT32	defaultFlags;	/* The default specification for subtables. */
  HBUINT32	length;		/* Total byte count, including this header. */
  HBUINT	feature_मोजणी;	/* Number of feature subtable entries. */
  HBUINT	subtable_मोजणी_2;	/* The number of subtables in the chain. */

  UnsizedArrayOf<Feature>	featureZ;	/* Features. */
/*ChainSubtable	firstSubtable;*//* Subtables. */
/*SubtableGlyphCoverage coverages*//* Only if version >= 3. */

  सार्वजनिक:
  DEFINE_SIZE_MIN (8 + 2 * आकार (HBUINT));
};


/*
 * The 'mort'/'morx' Table
 */

ढाँचा <typename T, typename Types, hb_tag_t TAG>
संरचना mortmorx
{
  स्थिर constexpr hb_tag_t tableTag = TAG;

  तार्किक has_data () स्थिरांक { परत version != 0; }

  संरचना accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      hb_sanitize_context_t sc;
      हे->table = sc.reference_table<T> (face);

      जर (unlikely (हे->table->is_blocklisted (हे->table.get_blob (), face)))
      {
        hb_blob_destroy (हे->table.get_blob ());
        हे->table = hb_blob_get_empty ();
      }

      हे->chain_मोजणी = table->get_chain_मोजणी ();

      हे->accels = (hb_atomic_t<hb_aat_layout_chain_accelerator_t *> *) hb_calloc (हे->chain_मोजणी, आकार (*accels));
      जर (unlikely (!हे->accels))
      {
	हे->chain_मोजणी = 0;
	हे->table.destroy ();
	हे->table = hb_blob_get_empty ();
      }
    }
    ~accelerator_t ()
    {
      साठी (अचिन्हित पूर्णांक i = 0; i < हे->chain_मोजणी; i++)
      {
	जर (हे->accels[i])
	  हे->accels[i]->destroy ();
	hb_free (हे->accels[i]);
      }
      hb_free (हे->accels);
      हे->table.destroy ();
    }

    hb_blob_t *get_blob () स्थिरांक { परत table.get_blob (); }

    ढाँचा <typename Chain>
    hb_aat_layout_chain_accelerator_t *get_accel (अचिन्हित chain_index, स्थिरांक Chain &chain, अचिन्हित अंक_glyphs) स्थिरांक
    {
      जर (unlikely (chain_index >= chain_मोजणी)) परत nullptr;

    retry:
      स्वतः *accel = accels[chain_index].get_acquire ();
      जर (unlikely (!accel))
      {
	accel = hb_aat_layout_chain_accelerator_t::create (chain, अंक_glyphs);
	जर (unlikely (!accel))
	  परत nullptr;

	जर (unlikely (!accels[chain_index].cmpexch (nullptr, accel)))
	{
	  hb_free (accel);
	  जा retry;
	}
      }

      परत accel;
    }

    hb_blob_ptr_t<T> table;
    अचिन्हित पूर्णांक chain_मोजणी;
    hb_atomic_t<hb_aat_layout_chain_accelerator_t *> *accels;
    hb_aat_scratch_t scratch;
  };


  शून्य compile_flags (स्थिरांक hb_aat_map_builder_t *mapper,
		      hb_aat_map_t *map) स्थिरांक
  {
    स्थिरांक Chain<Types> *chain = &firstChain;
    अचिन्हित पूर्णांक मोजणी = chain_मोजणी_2;
    जर (unlikely (!map->chain_flags.resize (मोजणी)))
      परत;
    साठी (अचिन्हित पूर्णांक i = 0; i < मोजणी; i++)
    {
      map->chain_flags[i].push (hb_aat_map_t::range_flags_t {chain->compile_flags (mapper),
							     mapper->range_first,
							     mapper->range_last});
      chain = &StructAfter<Chain<Types>> (*chain);
    }
  }

  अचिन्हित get_chain_मोजणी () स्थिरांक
  {
    परत chain_मोजणी_2;
  }
  शून्य apply (hb_aat_apply_context_t *c,
	      स्थिरांक hb_aat_map_t &map,
	      स्थिरांक accelerator_t &accel) स्थिरांक
  {
    जर (unlikely (!c->buffer->successful)) परत;

    c->buffer->unsafe_to_concat ();

    c->setup_buffer_glyph_set ();

    c->set_lookup_index (0);
    स्थिरांक Chain<Types> *chain = &firstChain;
    अचिन्हित पूर्णांक मोजणी = chain_मोजणी_2;
    साठी (अचिन्हित पूर्णांक i = 0; i < मोजणी; i++)
    {
      स्वतः *chain_accel = accel.get_accel (i, *chain, c->face->get_अंक_glyphs ());
      c->range_flags = &map.chain_flags[i];
      chain->apply (c, chain_accel);
      जर (unlikely (!c->buffer->successful)) परत;
      chain = &StructAfter<Chain<Types>> (*chain);
    }
  }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    जर (!(version.sanitize (c) &&
	  hb_barrier () &&
	  version &&
	  chain_मोजणी_2.sanitize (c)))
      return_trace (असत्य);

    स्थिरांक Chain<Types> *chain = &firstChain;
    अचिन्हित पूर्णांक मोजणी = chain_मोजणी_2;
    साठी (अचिन्हित पूर्णांक i = 0; i < मोजणी; i++)
    {
      जर (!chain->sanitize (c, version))
	return_trace (असत्य);
      hb_barrier ();
      chain = &StructAfter<Chain<Types>> (*chain);
    }

    return_trace (सत्य);
  }

  संरक्षित:
  HBUINT16	version;	/* Version number of the glyph metamorphosis table.
				 * 1, 2, or 3. */
  HBUINT16	unused;		/* Set to 0. */
  HBUINT32	chain_मोजणी_2;	/* Number of metamorphosis chains contained in this
				 * table. */
  Chain<Types>	firstChain;	/* Chains. */

  सार्वजनिक:
  DEFINE_SIZE_MIN (8);
};

संरचना morx : mortmorx<morx, ExtendedTypes, HB_AAT_TAG_morx>
{
  HB_INTERNAL तार्किक is_blocklisted (hb_blob_t *blob,
                                   hb_face_t *face) स्थिरांक;
};

संरचना mort : mortmorx<mort, ObsoleteTypes, HB_AAT_TAG_mort>
{
  HB_INTERNAL तार्किक is_blocklisted (hb_blob_t *blob,
                                   hb_face_t *face) स्थिरांक;
};

संरचना morx_accelerator_t : morx::accelerator_t {
  morx_accelerator_t (hb_face_t *face) : morx::accelerator_t (face) {}
};
संरचना mort_accelerator_t : mort::accelerator_t {
  mort_accelerator_t (hb_face_t *face) : mort::accelerator_t (face) {}
};


} /* namespace AAT */


#endif /* HB_AAT_LAYOUT_MORX_TABLE_HH */
