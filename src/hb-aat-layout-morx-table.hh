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

#अगर_अघोषित HB_AAT_LAYOUT_MORX_TABLE_HH
#घोषणा HB_AAT_LAYOUT_MORX_TABLE_HH

#समावेश "hb-open-type.hh"
#समावेश "hb-aat-layout-common.hh"
#समावेश "hb-ot-layout.hh"
#समावेश "hb-aat-map.hh"

/*
 * morx -- Extended Glyph Metamorphosis
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6morx.html
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6mort.html
 */
#घोषणा HB_AAT_TAG_morx HB_TAG('m','o','r','x')
#घोषणा HB_AAT_TAG_mort HB_TAG('m','o','r','t')


namespace AAT {

using namespace OT;

ढाँचा <typename Types>
काष्ठा RearrangementSubtable
{
  प्रकार typename Types::HBUINT HBUINT;

  प्रकार व्योम Entry_तथ्य;

  क्रमागत Flags
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

  bool is_action_initiable (स्थिर Entry<Entry_तथ्य> &entry) स्थिर
  {
    वापस (entry.flags & MarkFirst);
  }
  bool is_actionable (स्थिर Entry<Entry_तथ्य> &entry) स्थिर
  {
    वापस (entry.flags & Verb);
  }

  काष्ठा driver_context_t
  {
    अटल constexpr bool in_place = true;

    driver_context_t (स्थिर RearrangementSubtable *table_) :
	ret (false),
	table (table_),
	start (0), end (0) {}

    HB_AAT_TRANSITION_INLINE
    व्योम transition (hb_बफर_t *बफर,
		     StateTableDriver<Types, Entry_तथ्य, Flags> *driver,
		     स्थिर Entry<Entry_तथ्य> &entry)
    {
      अचिन्हित पूर्णांक flags = entry.flags;

      अगर (flags & MarkFirst)
	start = बफर->idx;

      अगर (flags & MarkLast)
	end = hb_min (बफर->idx + 1, बफर->len);

      अगर ((flags & Verb) && start < end)
      {
	/* The following map has two nibbles, for start-side
	 * and end-side. Values of 0,1,2 mean move that many
	 * to the other side. Value of 3 means move 2 and
	 * flip them. */
	स्थिर अचिन्हित अक्षर map[16] =
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
	bool reverse_l = 3 == (m >> 4);
	bool reverse_r = 3 == (m & 0x0F);

	अगर (end - start >= l + r && end-start <= HB_MAX_CONTEXT_लंबाई)
	{
	  बफर->merge_clusters (start, hb_min (बफर->idx + 1, बफर->len));
	  बफर->merge_clusters (start, end);

	  hb_glyph_info_t *info = बफर->info;
	  hb_glyph_info_t buf[4];

	  hb_memcpy (buf, info + start, l * माप (buf[0]));
	  hb_memcpy (buf + 2, info + end - r, r * माप (buf[0]));

	  अगर (l != r)
	    स_हटाओ (info + start + r, info + start + l, (end - start - l - r) * माप (buf[0]));

	  hb_memcpy (info + start, buf + 2, r * माप (buf[0]));
	  hb_memcpy (info + end - l, buf, l * माप (buf[0]));
	  अगर (reverse_l)
	  {
	    buf[0] = info[end - 1];
	    info[end - 1] = info[end - 2];
	    info[end - 2] = buf[0];
	  }
	  अगर (reverse_r)
	  {
	    buf[0] = info[start];
	    info[start] = info[start + 1];
	    info[start + 1] = buf[0];
	  }
	}
      }
    }

    खुला:
    bool ret;
    स्थिर RearrangementSubtable *table;
    निजी:
    अचिन्हित पूर्णांक start;
    अचिन्हित पूर्णांक end;
  };

  bool apply (hb_aat_apply_context_t *c) स्थिर
  {
    TRACE_APPLY (यह);

    driver_context_t dc (यह);

    StateTableDriver<Types, Entry_तथ्य, Flags> driver (machine, c->face);

    driver.drive (&dc, c);

    return_trace (dc.ret);
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (machine.sanitize (c));
  }

  खुला:
  StateTable<Types, Entry_तथ्य>	machine;
  खुला:
  DEFINE_आकार_STATIC ((StateTable<Types, Entry_तथ्य>::static_आकार));
};

ढाँचा <typename Types>
काष्ठा ContextualSubtable
{
  प्रकार typename Types::HBUINT HBUINT;

  काष्ठा Entry_तथ्य
  {
    प्रकार typename std::conditional<Types::extended, HBUINT16, HBINT16>::type OffsetType;

    OffsetType	mark_सूचकांक_2;	/* Index/offset of the substitution table for the
				 * marked glyph. */
    OffsetType	current_सूचकांक;	/* Index/offset of the substitution table for the
				 * current glyph. */
    खुला:
    DEFINE_आकार_STATIC (4);
  };

  क्रमागत Flags
  {
    SetMark		= 0x8000,	/* If set, make the current glyph the marked glyph. */
    DontAdvance		= 0x4000,	/* If set, don't advance to the next glyph before
					 * going to the new state. */
    Reserved		= 0x3FFF,	/* These bits are reserved and should be set to 0. */
  };

  bool is_action_initiable (स्थिर Entry<Entry_तथ्य> &entry) स्थिर
  {
    वापस (entry.flags & SetMark);
  }
  bool is_actionable (स्थिर Entry<Entry_तथ्य> &entry) स्थिर
  {
    अगर (Types::extended)
      वापस entry.तथ्य.mark_सूचकांक_2 != 0xFFFF || entry.तथ्य.current_सूचकांक != 0xFFFF;
    अन्यथा
      वापस entry.तथ्य.mark_सूचकांक_2 || entry.तथ्य.current_सूचकांक;
  }

  काष्ठा driver_context_t
  {
    अटल constexpr bool in_place = true;

    driver_context_t (स्थिर ContextualSubtable *table_,
			     hb_aat_apply_context_t *c_) :
	ret (false),
	c (c_),
	table (table_),
	mark_set (false),
	mark (0),
	subs (table+table->substitutionTables) {}

    HB_AAT_TRANSITION_INLINE
    व्योम transition (hb_बफर_t *बफर,
		     StateTableDriver<Types, Entry_तथ्य, Flags> *driver,
		     स्थिर Entry<Entry_तथ्य> &entry)
    {
      /* Looks like CoreText applies neither mark nor current substitution for
       * end-of-text if mark was not explicitly set. */
      अगर (बफर->idx == बफर->len && !mark_set)
	वापस;

      स्थिर HBGlyphID16 *replacement;

      replacement = nullptr;
      अगर (Types::extended)
      {
	अगर (entry.तथ्य.mark_सूचकांक_2 != 0xFFFF)
	{
	  स्थिर Lookup<HBGlyphID16> &lookup = subs[entry.तथ्य.mark_सूचकांक_2];
	  replacement = lookup.get_मूल्य (बफर->info[mark].codepoint, driver->अंक_glyphs);
	}
      }
      अन्यथा
      {
	अगर (entry.तथ्य.mark_सूचकांक_2)
	{
	  पूर्णांक offset = (पूर्णांक) entry.तथ्य.mark_सूचकांक_2 + बफर->info[mark].codepoint;
	  स्थिर UnsizedArrayOf<HBGlyphID16> &subs_old = (स्थिर UnsizedArrayOf<HBGlyphID16> &) subs;
	  replacement = &subs_old[Types::word_Offset_To_सूचकांक (offset, table, subs_old.arrayZ)];
	  अगर (!(replacement->sanitize (&c->sanitizer) &&
		hb_barrier () &&
		*replacement))
	    replacement = nullptr;
	}
      }
      अगर (replacement)
      {
	बफर->unsafe_to_break (mark, hb_min (बफर->idx + 1, बफर->len));
	c->replace_glyph_inplace (mark, *replacement);
	ret = true;
      }

      replacement = nullptr;
      अचिन्हित पूर्णांक idx = hb_min (बफर->idx, बफर->len - 1);
      अगर (Types::extended)
      {
	अगर (entry.तथ्य.current_सूचकांक != 0xFFFF)
	{
	  स्थिर Lookup<HBGlyphID16> &lookup = subs[entry.तथ्य.current_सूचकांक];
	  replacement = lookup.get_मूल्य (बफर->info[idx].codepoint, driver->अंक_glyphs);
	}
      }
      अन्यथा
      {
	अगर (entry.तथ्य.current_सूचकांक)
	{
	  पूर्णांक offset = (पूर्णांक) entry.तथ्य.current_सूचकांक + बफर->info[idx].codepoint;
	  स्थिर UnsizedArrayOf<HBGlyphID16> &subs_old = (स्थिर UnsizedArrayOf<HBGlyphID16> &) subs;
	  replacement = &subs_old[Types::word_Offset_To_सूचकांक (offset, table, subs_old.arrayZ)];
	  अगर (!(replacement->sanitize (&c->sanitizer) &&
		hb_barrier () &&
		*replacement))
	    replacement = nullptr;
	}
      }
      अगर (replacement)
      {
	c->replace_glyph_inplace (idx, *replacement);
	ret = true;
      }

      अगर (entry.flags & SetMark)
      {
	mark_set = true;
	mark = बफर->idx;
      }
    }

    खुला:
    bool ret;
    hb_aat_apply_context_t *c;
    स्थिर ContextualSubtable *table;
    निजी:
    bool mark_set;
    अचिन्हित पूर्णांक mark;
    स्थिर UnsizedListOfOffset16To<Lookup<HBGlyphID16>, HBUINT, व्योम, false> &subs;
  };

  bool apply (hb_aat_apply_context_t *c) स्थिर
  {
    TRACE_APPLY (यह);

    driver_context_t dc (यह, c);

    StateTableDriver<Types, Entry_तथ्य, Flags> driver (machine, c->face);

    driver.drive (&dc, c);

    return_trace (dc.ret);
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);

    अचिन्हित पूर्णांक अंक_entries = 0;
    अगर (unlikely (!machine.sanitize (c, &अंक_entries))) return_trace (false);
    hb_barrier ();

    अगर (!Types::extended)
      return_trace (substitutionTables.sanitize (c, यह, 0));

    अचिन्हित पूर्णांक अंक_lookups = 0;

    स्थिर Entry<Entry_तथ्य> *entries = machine.get_entries ();
    क्रम (अचिन्हित पूर्णांक i = 0; i < अंक_entries; i++)
    {
      स्थिर Entry_तथ्य &तथ्य = entries[i].तथ्य;

      अगर (तथ्य.mark_सूचकांक_2 != 0xFFFF)
	अंक_lookups = hb_max (अंक_lookups, 1u + तथ्य.mark_सूचकांक_2);
      अगर (तथ्य.current_सूचकांक != 0xFFFF)
	अंक_lookups = hb_max (अंक_lookups, 1u + तथ्य.current_सूचकांक);
    }

    return_trace (substitutionTables.sanitize (c, यह, अंक_lookups));
  }

  खुला:
  StateTable<Types, Entry_तथ्य>
		machine;
  रक्षित:
  NNOffsetTo<UnsizedListOfOffset16To<Lookup<HBGlyphID16>, HBUINT, व्योम, false>, HBUINT>
		substitutionTables;
  खुला:
  DEFINE_आकार_STATIC ((StateTable<Types, Entry_तथ्य>::static_आकार + HBUINT::static_आकार));
};


ढाँचा <bool extended>
काष्ठा LigatureEntry;

ढाँचा <>
काष्ठा LigatureEntry<true>
{

  काष्ठा Entry_तथ्य
  {
    HBUINT16	lig_Action_सूचकांक;	/* Index to the first ligActionTable entry
				 * for processing this group, if indicated
				 * by the flags. */
    खुला:
    DEFINE_आकार_STATIC (2);
  };

  क्रमागत Flags
  {
    SetComponent	= 0x8000,	/* Push this glyph onto the component stack for
					 * eventual processing. */
    DontAdvance		= 0x4000,	/* Leave the glyph pointer at this glyph for the
					   next iteration. */
    PerformAction	= 0x2000,	/* Use the ligActionIndex to process a ligature
					 * group. */
    Reserved		= 0x1FFF,	/* These bits are reserved and should be set to 0. */
  };

  अटल bool initiateAction (स्थिर Entry<Entry_तथ्य> &entry)
  { वापस entry.flags & SetComponent; }

  अटल bool performAction (स्थिर Entry<Entry_तथ्य> &entry)
  { वापस entry.flags & PerformAction; }

  अटल अचिन्हित पूर्णांक lig_Action_सूचकांक (स्थिर Entry<Entry_तथ्य> &entry)
  { वापस entry.तथ्य.lig_Action_सूचकांक; }
};
ढाँचा <>
काष्ठा LigatureEntry<false>
{
  प्रकार व्योम Entry_तथ्य;

  क्रमागत Flags
  {
    SetComponent	= 0x8000,	/* Push this glyph onto the component stack for
					 * eventual processing. */
    DontAdvance		= 0x4000,	/* Leave the glyph pointer at this glyph for the
					   next iteration. */
    Offset		= 0x3FFF,	/* Byte offset from beginning of subtable to the
					 * ligature action list. This value must be a
					 * multiple of 4. */
  };

  अटल bool initiateAction (स्थिर Entry<Entry_तथ्य> &entry)
  { वापस entry.flags & SetComponent; }

  अटल bool performAction (स्थिर Entry<Entry_तथ्य> &entry)
  { वापस entry.flags & Offset; }

  अटल अचिन्हित पूर्णांक lig_Action_सूचकांक (स्थिर Entry<Entry_तथ्य> &entry)
  { वापस entry.flags & Offset; }
};


ढाँचा <typename Types>
काष्ठा LigatureSubtable
{
  प्रकार typename Types::HBUINT HBUINT;

  प्रकार LigatureEntry<Types::extended> LigatureEntryT;
  प्रकार typename LigatureEntryT::Entry_तथ्य Entry_तथ्य;

  क्रमागत Flags
  {
    DontAdvance	= LigatureEntryT::DontAdvance,
  };

  bool is_action_initiable (स्थिर Entry<Entry_तथ्य> &entry) स्थिर
  {
    वापस LigatureEntryT::initiateAction (entry);
  }
  bool is_actionable (स्थिर Entry<Entry_तथ्य> &entry) स्थिर
  {
    वापस LigatureEntryT::performAction (entry);
  }

  काष्ठा driver_context_t
  {
    अटल constexpr bool in_place = false;
    क्रमागत LigActionFlags
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

    driver_context_t (स्थिर LigatureSubtable *table_,
		      hb_aat_apply_context_t *c_) :
	ret (false),
	c (c_),
	table (table_),
	ligAction (table+table->ligAction),
	component (table+table->component),
	ligature (table+table->ligature),
	match_लंबाई (0) {}

    HB_AAT_TRANSITION_INLINE
    व्योम transition (hb_बफर_t *बफर,
		     StateTableDriver<Types, Entry_तथ्य, Flags> *driver,
		     स्थिर Entry<Entry_तथ्य> &entry)
    {
      DEBUG_संदेश (APPLY, nullptr, "Ligature transition at %u", बफर->idx);
      अगर (entry.flags & LigatureEntryT::SetComponent)
      {
	/* Never mark same index twice, in case DontAdvance was used... */
	अगर (unlikely (match_लंबाई && match_positions[(match_लंबाई - 1u) % ARRAY_लंबाई (match_positions)] == बफर->out_len))
	  match_लंबाई--;

	match_positions[match_लंबाई++ % ARRAY_लंबाई (match_positions)] = बफर->out_len;
	DEBUG_संदेश (APPLY, nullptr, "Set component at %u", बफर->out_len);
      }

      अगर (LigatureEntryT::performAction (entry))
      {
	DEBUG_संदेश (APPLY, nullptr, "Perform action with %u", match_लंबाई);
	अचिन्हित पूर्णांक end = बफर->out_len;

	अगर (unlikely (!match_लंबाई))
	  वापस;

	अगर (बफर->idx >= बफर->len)
	  वापस; /* TODO Work on previous instead? */

	अचिन्हित पूर्णांक cursor = match_लंबाई;

	अचिन्हित पूर्णांक action_idx = LigatureEntryT::lig_Action_सूचकांक (entry);
	action_idx = Types::offset_To_सूचकांक (action_idx, table, ligAction.arrayZ);
	स्थिर HBUINT32 *action_तथ्य = &ligAction[action_idx];

	अचिन्हित पूर्णांक ligature_idx = 0;
	अचिन्हित पूर्णांक action;
	करो
	{
	  अगर (unlikely (!cursor))
	  {
	    /* Stack underflow.  Clear the stack. */
	    DEBUG_संदेश (APPLY, nullptr, "Stack underflow");
	    match_लंबाई = 0;
	    अवरोध;
	  }

	  DEBUG_संदेश (APPLY, nullptr, "Moving to stack position %u", cursor - 1);
	  अगर (unlikely (!बफर->move_to (match_positions[--cursor % ARRAY_लंबाई (match_positions)]))) वापस;

	  अगर (unlikely (!action_तथ्य->sanitize (&c->sanitizer))) अवरोध;
	  hb_barrier ();
	  action = *action_तथ्य;

	  uint32_t uoffset = action & LigActionOffset;
	  अगर (uoffset & 0x20000000)
	    uoffset |= 0xC0000000; /* Sign-extend. */
	  int32_t offset = (int32_t) uoffset;
	  अचिन्हित पूर्णांक component_idx = बफर->cur().codepoint + offset;
	  component_idx = Types::word_Offset_To_सूचकांक (component_idx, table, component.arrayZ);
	  स्थिर HBUINT16 &component_तथ्य = component[component_idx];
	  अगर (unlikely (!component_तथ्य.sanitize (&c->sanitizer))) अवरोध;
	  hb_barrier ();
	  ligature_idx += component_तथ्य;

	  DEBUG_संदेश (APPLY, nullptr, "Action store %d last %d",
		     bool (action & LigActionStore),
		     bool (action & LigActionLast));
	  अगर (action & (LigActionStore | LigActionLast))
	  {
	    ligature_idx = Types::offset_To_सूचकांक (ligature_idx, table, ligature.arrayZ);
	    स्थिर HBGlyphID16 &ligature_तथ्य = ligature[ligature_idx];
	    अगर (unlikely (!ligature_तथ्य.sanitize (&c->sanitizer))) अवरोध;
	    hb_barrier ();
	    hb_codepoint_t lig = ligature_तथ्य;

	    DEBUG_संदेश (APPLY, nullptr, "Produced ligature %u", lig);
	    अगर (unlikely (!c->replace_glyph (lig))) वापस;

	    अचिन्हित पूर्णांक lig_end = match_positions[(match_लंबाई - 1u) % ARRAY_लंबाई (match_positions)] + 1u;
	    /* Now go and delete all subsequent components. */
	    जबतक (match_लंबाई - 1u > cursor)
	    {
	      DEBUG_संदेश (APPLY, nullptr, "Skipping ligature component");
	      अगर (unlikely (!बफर->move_to (match_positions[--match_लंबाई % ARRAY_लंबाई (match_positions)]))) वापस;
	      अगर (!c->delete_glyph ()) वापस;
	    }

	    अगर (unlikely (!बफर->move_to (lig_end))) वापस;
	    बफर->merge_out_clusters (match_positions[cursor % ARRAY_लंबाई (match_positions)], बफर->out_len);
	  }

	  action_तथ्य++;
	}
	जबतक (!(action & LigActionLast));
	अगर (unlikely (!बफर->move_to (end))) वापस;
      }
    }

    खुला:
    bool ret;
    hb_aat_apply_context_t *c;
    स्थिर LigatureSubtable *table;
    निजी:
    स्थिर UnsizedArrayOf<HBUINT32> &ligAction;
    स्थिर UnsizedArrayOf<HBUINT16> &component;
    स्थिर UnsizedArrayOf<HBGlyphID16> &ligature;
    अचिन्हित पूर्णांक match_लंबाई;
    अचिन्हित पूर्णांक match_positions[HB_MAX_CONTEXT_लंबाई];
  };

  bool apply (hb_aat_apply_context_t *c) स्थिर
  {
    TRACE_APPLY (यह);

    driver_context_t dc (यह, c);

    StateTableDriver<Types, Entry_तथ्य, Flags> driver (machine, c->face);

    driver.drive (&dc, c);

    return_trace (dc.ret);
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    /* The rest of array sanitizations are done at run-time. */
    return_trace (c->check_struct (यह) && machine.sanitize (c) &&
		  hb_barrier () &&
		  ligAction && component && ligature);
  }

  खुला:
  StateTable<Types, Entry_तथ्य>
		machine;
  रक्षित:
  NNOffsetTo<UnsizedArrayOf<HBUINT32>, HBUINT>
		ligAction;	/* Offset to the ligature action table. */
  NNOffsetTo<UnsizedArrayOf<HBUINT16>, HBUINT>
		component;	/* Offset to the component table. */
  NNOffsetTo<UnsizedArrayOf<HBGlyphID16>, HBUINT>
		ligature;	/* Offset to the actual ligature lists. */
  खुला:
  DEFINE_आकार_STATIC ((StateTable<Types, Entry_तथ्य>::static_आकार + 3 * HBUINT::static_आकार));
};

ढाँचा <typename Types>
काष्ठा NoncontextualSubtable
{
  bool apply (hb_aat_apply_context_t *c) स्थिर
  {
    TRACE_APPLY (यह);

    bool ret = false;
    अचिन्हित पूर्णांक अंक_glyphs = c->face->get_अंक_glyphs ();

    hb_glyph_info_t *info = c->बफर->info;
    अचिन्हित पूर्णांक गिनती = c->बफर->len;
    // If there's only one range, we already checked the flag.
    स्वतः *last_range = c->range_flags && (c->range_flags->लंबाई > 1) ? &(*c->range_flags)[0] : nullptr;
    क्रम (अचिन्हित पूर्णांक i = 0; i < गिनती; i++)
    {
      /* This block copied from StateTableDriver::drive. Keep in sync. */
      अगर (unlikely (last_range))
      {
	स्वतः *range = last_range;
	{
	  अचिन्हित cluster = info[i].cluster;
	  जबतक (cluster < range->cluster_first)
	    range--;
	  जबतक (cluster > range->cluster_last)
	    range++;

	  last_range = range;
	}
	अगर (!(range->flags & c->subtable_flags))
	  जारी;
      }

      स्थिर HBGlyphID16 *replacement = substitute.get_मूल्य (info[i].codepoint, अंक_glyphs);
      अगर (replacement)
      {
	c->replace_glyph_inplace (i, *replacement);
	ret = true;
      }
    }

    return_trace (ret);
  }

  ढाँचा <typename set_t>
  व्योम collect_initial_glyphs (set_t &glyphs, अचिन्हित अंक_glyphs) स्थिर
  {
    substitute.collect_glyphs (glyphs, अंक_glyphs);
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (substitute.sanitize (c));
  }

  रक्षित:
  Lookup<HBGlyphID16>	substitute;
  खुला:
  DEFINE_आकार_MIN (2);
};

ढाँचा <typename Types>
काष्ठा InsertionSubtable
{
  प्रकार typename Types::HBUINT HBUINT;

  काष्ठा Entry_तथ्य
  {
    HBUINT16	current_जोड़ो_सूचकांक;	/* Zero-based index into the insertion glyph table.
					 * The number of glyphs to be inserted is contained
					 * in the currentInsertCount field in the flags.
					 * A value of 0xFFFF indicates no insertion is to
					 * be done. */
    HBUINT16	marked_जोड़ो_सूचकांक;	/* Zero-based index into the insertion glyph table.
					 * The number of glyphs to be inserted is contained
					 * in the markedInsertCount field in the flags.
					 * A value of 0xFFFF indicates no insertion is to
					 * be done. */
    खुला:
    DEFINE_आकार_STATIC (4);
  };

  क्रमागत Flags
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
    Current_जोड़ो_Before= 0x0800,      /* If set, specifies that insertions are to be made
				       * to the left of the current glyph. If clear,
				       * they're made to the right of the current glyph. */
    Marked_जोड़ो_Before= 0x0400,	      /* If set, specifies that insertions are to be
				       * made to the left of the marked glyph. If clear,
				       * they're made to the right of the marked glyph. */
    Current_जोड़ो_गिनती= 0x3E0,	      /* This 5-bit field is treated as a count of the
				       * number of glyphs to insert at the current
				       * position. Since zero means no insertions, the
				       * largest number of insertions at any given
				       * current location is 31 glyphs. */
    Marked_जोड़ो_गिनती= 0x001F,	      /* This 5-bit field is treated as a count of the
				       * number of glyphs to insert at the marked
				       * position. Since zero means no insertions, the
				       * largest number of insertions at any given
				       * marked location is 31 glyphs. */
  };

  bool is_action_initiable (स्थिर Entry<Entry_तथ्य> &entry) स्थिर
  {
    वापस (entry.flags & SetMark);
  }
  bool is_actionable (स्थिर Entry<Entry_तथ्य> &entry) स्थिर
  {
    वापस (entry.flags & (Current_जोड़ो_गिनती | Marked_जोड़ो_गिनती)) &&
	   (entry.तथ्य.current_जोड़ो_सूचकांक != 0xFFFF ||entry.तथ्य.marked_जोड़ो_सूचकांक != 0xFFFF);
  }

  काष्ठा driver_context_t
  {
    अटल constexpr bool in_place = false;

    driver_context_t (स्थिर InsertionSubtable *table_,
		      hb_aat_apply_context_t *c_) :
	ret (false),
	c (c_),
	table (table_),
	mark (0),
	insertionAction (table+table->insertionAction) {}

    HB_AAT_TRANSITION_INLINE
    व्योम transition (hb_बफर_t *बफर,
		     StateTableDriver<Types, Entry_तथ्य, Flags> *driver,
		     स्थिर Entry<Entry_तथ्य> &entry)
    {
      अचिन्हित पूर्णांक flags = entry.flags;

      अचिन्हित mark_loc = बफर->out_len;

      अगर (entry.तथ्य.marked_जोड़ो_सूचकांक != 0xFFFF)
      {
	अचिन्हित पूर्णांक गिनती = (flags & Marked_जोड़ो_गिनती);
	अगर (unlikely ((बफर->max_ops -= गिनती) <= 0)) वापस;
	अचिन्हित पूर्णांक start = entry.तथ्य.marked_जोड़ो_सूचकांक;
	स्थिर HBGlyphID16 *glyphs = &insertionAction[start];
	अगर (unlikely (!c->sanitizer.check_array (glyphs, गिनती))) गिनती = 0;
	hb_barrier ();

	bool before = flags & Marked_जोड़ो_Before;

	अचिन्हित पूर्णांक end = बफर->out_len;
	अगर (unlikely (!बफर->move_to (mark))) वापस;

	अगर (बफर->idx < बफर->len && !before)
	  अगर (unlikely (!बफर->copy_glyph ())) वापस;
	/* TODO We ignore KashidaLike setting. */
	अगर (unlikely (!c->output_glyphs (गिनती, glyphs))) वापस;
	ret = true;
	अगर (बफर->idx < बफर->len && !before)
	  बफर->skip_glyph ();

	अगर (unlikely (!बफर->move_to (end + गिनती))) वापस;

	बफर->unsafe_to_break_from_outbuffer (mark, hb_min (बफर->idx + 1, बफर->len));
      }

      अगर (flags & SetMark)
	mark = mark_loc;

      अगर (entry.तथ्य.current_जोड़ो_सूचकांक != 0xFFFF)
      {
	अचिन्हित पूर्णांक गिनती = (flags & Current_जोड़ो_गिनती) >> 5;
	अगर (unlikely ((बफर->max_ops -= गिनती) <= 0)) वापस;
	अचिन्हित पूर्णांक start = entry.तथ्य.current_जोड़ो_सूचकांक;
	स्थिर HBGlyphID16 *glyphs = &insertionAction[start];
	अगर (unlikely (!c->sanitizer.check_array (glyphs, गिनती))) गिनती = 0;
	hb_barrier ();

	bool before = flags & Current_जोड़ो_Before;

	अचिन्हित पूर्णांक end = बफर->out_len;

	अगर (बफर->idx < बफर->len && !before)
	  अगर (unlikely (!बफर->copy_glyph ())) वापस;
	/* TODO We ignore KashidaLike setting. */
	अगर (unlikely (!c->output_glyphs (गिनती, glyphs))) वापस;
	ret = true;
	अगर (बफर->idx < बफर->len && !before)
	  बफर->skip_glyph ();

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
	अगर (unlikely (!बफर->move_to ((flags & DontAdvance) ? end : end + गिनती))) वापस;
      }
    }

    खुला:
    bool ret;
    hb_aat_apply_context_t *c;
    स्थिर InsertionSubtable *table;
    निजी:
    अचिन्हित पूर्णांक mark;
    स्थिर UnsizedArrayOf<HBGlyphID16> &insertionAction;
  };

  bool apply (hb_aat_apply_context_t *c) स्थिर
  {
    TRACE_APPLY (यह);

    driver_context_t dc (यह, c);

    StateTableDriver<Types, Entry_तथ्य, Flags> driver (machine, c->face);

    driver.drive (&dc, c);

    return_trace (dc.ret);
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    /* The rest of array sanitizations are done at run-time. */
    return_trace (c->check_struct (यह) && machine.sanitize (c) &&
		  hb_barrier () &&
		  insertionAction);
  }

  खुला:
  StateTable<Types, Entry_तथ्य>
		machine;
  रक्षित:
  NNOffsetTo<UnsizedArrayOf<HBGlyphID16>, HBUINT>
		insertionAction;	/* Byte offset from stateHeader to the start of
					 * the insertion glyph table. */
  खुला:
  DEFINE_आकार_STATIC ((StateTable<Types, Entry_तथ्य>::static_आकार + HBUINT::static_आकार));
};


काष्ठा Feature
{
  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह));
  }

  खुला:
  HBUINT16	featureType;	/* The type of feature. */
  HBUINT16	featureSetting;	/* The feature's setting (aka selector). */
  HBUINT32	enableFlags;	/* Flags for the settings that this feature
				 * and setting enables. */
  HBUINT32	disableFlags;	/* Complement of flags for the settings that this
				 * feature and setting disable. */

  खुला:
  DEFINE_आकार_STATIC (12);
};


काष्ठा hb_accelerate_subtables_context_t :
       hb_dispatch_context_t<hb_accelerate_subtables_context_t>
{
  काष्ठा hb_applicable_t
  {
    मित्र काष्ठा hb_accelerate_subtables_context_t;
    मित्र काष्ठा hb_aat_layout_lookup_accelerator_t;

    खुला:
    hb_bit_set_t glyph_set;
    mutable hb_aat_class_cache_t class_cache;

    ढाँचा <typename T>
    स्वतः init_ (स्थिर T &obj_, अचिन्हित अंक_glyphs, hb_priority<1>) HB_AUTO_RETURN
    (
      obj_.machine.collect_initial_glyphs (glyph_set, अंक_glyphs, obj_)
    )

    ढाँचा <typename T>
    व्योम init_ (स्थिर T &obj_, अचिन्हित अंक_glyphs, hb_priority<0>)
    {
      obj_.collect_initial_glyphs (glyph_set, अंक_glyphs);
    }

    ढाँचा <typename T>
    व्योम init (स्थिर T &obj_, अचिन्हित अंक_glyphs)
    {
      glyph_set.init ();
      init_ (obj_, अंक_glyphs, hb_prioritize);
      class_cache.सफा ();
    }

    व्योम
    fini ()
    {
      glyph_set.fini ();
    }
  };

  /* Dispatch interface. */
  ढाँचा <typename T>
  return_t dispatch (स्थिर T &obj)
  {
    hb_applicable_t *entry = &array[i++];

    entry->init (obj, अंक_glyphs);

    वापस hb_empty_t ();
  }
  अटल return_t default_return_मूल्य () { वापस hb_empty_t (); }

  bool stop_sublookup_iteration (return_t r) स्थिर { वापस false; }

  hb_accelerate_subtables_context_t (hb_applicable_t *array_, अचिन्हित अंक_glyphs_2) :
				     hb_dispatch_context_t<hb_accelerate_subtables_context_t> (),
				     array (array_), अंक_glyphs (अंक_glyphs_2) {}

  hb_applicable_t *array;
  अचिन्हित अंक_glyphs;
  अचिन्हित i = 0;
};

काष्ठा hb_aat_layout_chain_accelerator_t
{
  ढाँचा <typename TChain>
  अटल hb_aat_layout_chain_accelerator_t *create (स्थिर TChain &chain, अचिन्हित अंक_glyphs)
  {
    अचिन्हित गिनती = chain.get_subtable_गिनती ();

    अचिन्हित product;
    अगर (unlikely (hb_unsigned_mul_overflows (गिनती,
        माप (hb_accelerate_subtables_context_t::hb_applicable_t), &product)))
      वापस nullptr;

    अचिन्हित आकार;
    अगर (unlikely (hb_unsigned_add_overflows (माप (hb_aat_layout_chain_accelerator_t) -
        HB_VAR_ARRAY * माप (hb_accelerate_subtables_context_t::hb_applicable_t), product, &आकार))) {
      वापस nullptr;
    }

    /* The following is a calloc because when we are collecting subtables,
     * some of them might be invalid and hence not collect; as a result,
     * we might not fill in all the count entries of the subtables array.
     * Zeroing it allows the set digest to gatekeep it without having to
     * initialize it further. */
    स्वतः *thiz = (hb_aat_layout_chain_accelerator_t *) hb_calloc (1, आकार);
    अगर (unlikely (!thiz))
      वापस nullptr;

    thiz->गिनती = गिनती;

    hb_accelerate_subtables_context_t c_accelerate_subtables (thiz->subtables, अंक_glyphs);
    chain.dispatch (&c_accelerate_subtables);

    वापस thiz;
  }

  व्योम destroy ()
  {
    क्रम (अचिन्हित i = 0; i < गिनती; i++)
      subtables[i].fini ();
  }

  अचिन्हित गिनती;
  hb_accelerate_subtables_context_t::hb_applicable_t subtables[HB_VAR_ARRAY];
};

ढाँचा <typename Types>
काष्ठा ChainSubtable
{
  प्रकार typename Types::HBUINT HBUINT;

  ढाँचा <typename T>
  मित्र काष्ठा Chain;

  माप_प्रकार get_आकार () स्थिर     { वापस लंबाई; }
  अचिन्हित पूर्णांक get_type () स्थिर     { वापस coverage & 0xFF; }
  अचिन्हित पूर्णांक get_coverage () स्थिर { वापस coverage >> (माप (HBUINT) * 8 - 8); }

  क्रमागत Coverage
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
  क्रमागत Type
  {
    Rearrangement	= 0,
    Contextual		= 1,
    Ligature		= 2,
    Noncontextual	= 4,
    Insertion		= 5
  };

  ढाँचा <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) स्थिर
  {
    अचिन्हित पूर्णांक subtable_type = get_type ();
    TRACE_DISPATCH (यह, subtable_type);
    चयन (subtable_type) {
    हाल Rearrangement:		hb_barrier (); return_trace (c->dispatch (u.rearrangement, std::forward<Ts> (ds)...));
    हाल Contextual:		hb_barrier (); return_trace (c->dispatch (u.contextual, std::forward<Ts> (ds)...));
    हाल Ligature:		hb_barrier (); return_trace (c->dispatch (u.ligature, std::forward<Ts> (ds)...));
    हाल Noncontextual:		hb_barrier (); return_trace (c->dispatch (u.noncontextual, std::forward<Ts> (ds)...));
    हाल Insertion:		hb_barrier (); return_trace (c->dispatch (u.insertion, std::forward<Ts> (ds)...));
    शेष:			return_trace (c->default_return_मूल्य ());
    }
  }

  bool apply (hb_aat_apply_context_t *c) स्थिर
  {
    TRACE_APPLY (यह);
    // Disabled for https://github.com/harfbuzz/harfbuzz/issues/4873
    //hb_sanitize_with_object_t with (&c->sanitizer, this);
    return_trace (dispatch (c));
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    अगर (!(लंबाई.sanitize (c) &&
	  hb_barrier () &&
	  लंबाई >= min_आकार &&
	  c->check_range (यह, लंबाई)))
      return_trace (false);

    // Disabled for https://github.com/harfbuzz/harfbuzz/issues/4873
    //hb_sanitize_with_object_t with (c, this);
    return_trace (dispatch (c));
  }

  रक्षित:
  HBUINT	लंबाई;		/* Total subtable length, including this header. */
  HBUINT	coverage;	/* Coverage flags and subtable type. */
  HBUINT32	subFeatureFlags;/* The 32-bit mask identifying which subtable this is. */
  जोड़ {
  RearrangementSubtable<Types>	rearrangement;
  ContextualSubtable<Types>	contextual;
  LigatureSubtable<Types>	ligature;
  NoncontextualSubtable<Types>	noncontextual;
  InsertionSubtable<Types>	insertion;
  } u;
  खुला:
  DEFINE_आकार_MIN (2 * माप (HBUINT) + 4);
};

ढाँचा <typename Types>
काष्ठा Chain
{
  प्रकार typename Types::HBUINT HBUINT;

  अचिन्हित get_subtable_गिनती () स्थिर { वापस subtable_गिनती_2; }

  hb_mask_t compile_flags (स्थिर hb_aat_map_builder_t *map) स्थिर
  {
    hb_mask_t flags = defaultFlags;
    {
      अचिन्हित पूर्णांक गिनती = feature_गिनती;
      क्रम (अचिन्हित i = 0; i < गिनती; i++)
      {
	स्थिर Feature &feature = featureZ[i];
	hb_aat_layout_feature_type_t type = (hb_aat_layout_feature_type_t) (अचिन्हित पूर्णांक) feature.featureType;
	hb_aat_layout_feature_selector_t setting = (hb_aat_layout_feature_selector_t) (अचिन्हित पूर्णांक) feature.featureSetting;
      retry:
	// Check whether this type/setting pair was requested in the map, and if so, apply its flags.
	// (The search here only looks at the type and setting fields of feature_info_t.)
	hb_aat_map_builder_t::feature_info_t info = { type, setting, false, 0 };
	अगर (map->current_features.द्वा_खोज (info))
	{
	  flags &= feature.disableFlags;
	  flags |= feature.enableFlags;
	}
	अन्यथा अगर (type == HB_AAT_LAYOUT_FEATURE_TYPE_LETTER_CASE && setting == HB_AAT_LAYOUT_FEATURE_SELECTOR_SMALL_CAPS)
	{
	  /* Deprecated. https://github.com/harfbuzz/harfbuzz/issues/1342 */
	  type = HB_AAT_LAYOUT_FEATURE_TYPE_LOWER_CASE;
	  setting = HB_AAT_LAYOUT_FEATURE_SELECTOR_LOWER_CASE_SMALL_CAPS;
	  जाओ retry;
	}
#अगर_अघोषित HB_NO_AAT
	अन्यथा अगर (type == HB_AAT_LAYOUT_FEATURE_TYPE_LANGUAGE_TAG_TYPE && setting &&
		 /* TODO: Rudimentary language matching. */
		 hb_language_matches (map->face->table.ltag->get_language (setting - 1), map->props.language))
	{
	  flags &= feature.disableFlags;
	  flags |= feature.enableFlags;
	}
#पूर्ण_अगर
      }
    }
    वापस flags;
  }

  व्योम apply (hb_aat_apply_context_t *c,
	      स्थिर hb_aat_layout_chain_accelerator_t *accel) स्थिर
  {
    स्थिर ChainSubtable<Types> *subtable = &StructAfter<ChainSubtable<Types>> (featureZ.as_array (feature_गिनती));
    अचिन्हित पूर्णांक गिनती = subtable_गिनती_2;
    क्रम (अचिन्हित पूर्णांक i = 0; i < गिनती; i++)
    {
      bool reverse;

      स्वतः coverage = subtable->get_coverage ();

      hb_mask_t subtable_flags = subtable->subFeatureFlags;
      अगर (hb_none (hb_iter (c->range_flags) |
		   hb_map ([subtable_flags] (स्थिर hb_aat_map_t::range_flags_t _) -> bool { वापस subtable_flags & (_.flags); })))
	जाओ skip;

      अगर (!(coverage & ChainSubtable<Types>::AllDirections) &&
	  HB_DIRECTION_IS_VERTICAL (c->बफर->props.direction) !=
	  bool (coverage & ChainSubtable<Types>::Vertical))
	जाओ skip;

      c->subtable_flags = subtable_flags;
      c->first_set = accel ? &accel->subtables[i].glyph_set : &Null(hb_bit_set_t);
      c->machine_class_cache = accel ? &accel->subtables[i].class_cache : nullptr;

      अगर (!c->बफर_intersects_machine ())
      {
	(व्योम) c->बफर->message (c->font, "skipped chainsubtable %u because no glyph matches", c->lookup_सूचकांक);
	जाओ skip;
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
		bool (coverage & ChainSubtable<Types>::Backwards) :
		bool (coverage & ChainSubtable<Types>::Backwards) !=
		HB_DIRECTION_IS_BACKWARD (c->बफर->props.direction);

      अगर (!c->बफर->message (c->font, "start chainsubtable %u", c->lookup_सूचकांक))
	जाओ skip;

      अगर (reverse != c->बफर_is_reversed)
        c->reverse_बफर ();

      subtable->apply (c);

      (व्योम) c->बफर->message (c->font, "end chainsubtable %u", c->lookup_सूचकांक);

      अगर (unlikely (!c->बफर->successful)) अवरोध;

    skip:
      subtable = &StructAfter<ChainSubtable<Types>> (*subtable);
      c->set_lookup_सूचकांक (c->lookup_सूचकांक + 1);
    }
    अगर (c->बफर_is_reversed)
      c->reverse_बफर ();
  }

  माप_प्रकार get_आकार () स्थिर { वापस लंबाई; }

  ढाँचा <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) स्थिर
  {
    स्थिर ChainSubtable<Types> *subtable = &StructAfter<ChainSubtable<Types>> (featureZ.as_array (feature_गिनती));
    अचिन्हित पूर्णांक गिनती = subtable_गिनती_2;
    क्रम (अचिन्हित पूर्णांक i = 0; i < गिनती; i++)
    {
      typename context_t::return_t ret = subtable->dispatch (c, std::forward<Ts> (ds)...);
      अगर (c->stop_sublookup_iteration (ret))
	वापस ret;
      subtable = &StructAfter<ChainSubtable<Types>> (*subtable);
    }
    वापस c->default_return_मूल्य ();
  }

  bool sanitize (hb_sanitize_context_t *c, अचिन्हित पूर्णांक version) स्थिर
  {
    TRACE_SANITIZE (यह);
    अगर (!(लंबाई.sanitize (c) &&
	  hb_barrier () &&
	  लंबाई >= min_आकार &&
	  c->check_range (यह, लंबाई)))
      return_trace (false);

    अगर (!c->check_array (featureZ.arrayZ, feature_गिनती))
      return_trace (false);

    स्थिर ChainSubtable<Types> *subtable = &StructAfter<ChainSubtable<Types>> (featureZ.as_array (feature_गिनती));
    अचिन्हित पूर्णांक गिनती = subtable_गिनती_2;
    क्रम (अचिन्हित पूर्णांक i = 0; i < गिनती; i++)
    {
      अगर (!subtable->sanitize (c))
	return_trace (false);
      hb_barrier ();
      subtable = &StructAfter<ChainSubtable<Types>> (*subtable);
    }

    अगर (version >= 3)
    {
      स्थिर SubtableGlyphCoverage *coverage = (स्थिर SubtableGlyphCoverage *) subtable;
      अगर (!coverage->sanitize (c, गिनती))
        return_trace (false);
    }

    return_trace (true);
  }

  रक्षित:
  HBUINT32	defaultFlags;	/* The default specification for subtables. */
  HBUINT32	लंबाई;		/* Total byte count, including this header. */
  HBUINT	feature_गिनती;	/* Number of feature subtable entries. */
  HBUINT	subtable_गिनती_2;	/* The number of subtables in the chain. */

  UnsizedArrayOf<Feature>	featureZ;	/* Features. */
/*ChainSubtable	firstSubtable;*//* Subtables. */
/*SubtableGlyphCoverage coverages*//* Only if version >= 3. */

  खुला:
  DEFINE_आकार_MIN (8 + 2 * माप (HBUINT));
};


/*
 * The 'mort'/'morx' Table
 */

ढाँचा <typename T, typename Types, hb_tag_t TAG>
काष्ठा mortmorx
{
  अटल constexpr hb_tag_t tableTag = TAG;

  bool has_तथ्य () स्थिर { वापस version != 0; }

  काष्ठा accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      hb_sanitize_context_t sc;
      यह->table = sc.reference_table<T> (face);

      अगर (unlikely (यह->table->is_blocklisted (यह->table.get_blob (), face)))
      {
        hb_blob_destroy (यह->table.get_blob ());
        यह->table = hb_blob_get_empty ();
      }

      यह->chain_गिनती = table->get_chain_गिनती ();

      यह->accels = (hb_atomic_t<hb_aat_layout_chain_accelerator_t *> *) hb_calloc (यह->chain_गिनती, माप (*accels));
      अगर (unlikely (!यह->accels))
      {
	यह->chain_गिनती = 0;
	यह->table.destroy ();
	यह->table = hb_blob_get_empty ();
      }
    }
    ~accelerator_t ()
    {
      क्रम (अचिन्हित पूर्णांक i = 0; i < यह->chain_गिनती; i++)
      {
	अगर (यह->accels[i])
	  यह->accels[i]->destroy ();
	hb_free (यह->accels[i]);
      }
      hb_free (यह->accels);
      यह->table.destroy ();
    }

    hb_blob_t *get_blob () स्थिर { वापस table.get_blob (); }

    ढाँचा <typename Chain>
    hb_aat_layout_chain_accelerator_t *get_accel (अचिन्हित chain_सूचकांक, स्थिर Chain &chain, अचिन्हित अंक_glyphs) स्थिर
    {
      अगर (unlikely (chain_सूचकांक >= chain_गिनती)) वापस nullptr;

    retry:
      स्वतः *accel = accels[chain_सूचकांक].get_acquire ();
      अगर (unlikely (!accel))
      {
	accel = hb_aat_layout_chain_accelerator_t::create (chain, अंक_glyphs);
	अगर (unlikely (!accel))
	  वापस nullptr;

	अगर (unlikely (!accels[chain_सूचकांक].cmpexch (nullptr, accel)))
	{
	  hb_free (accel);
	  जाओ retry;
	}
      }

      वापस accel;
    }

    hb_blob_ptr_t<T> table;
    अचिन्हित पूर्णांक chain_गिनती;
    hb_atomic_t<hb_aat_layout_chain_accelerator_t *> *accels;
    hb_aat_scratch_t scratch;
  };


  व्योम compile_flags (स्थिर hb_aat_map_builder_t *mapper,
		      hb_aat_map_t *map) स्थिर
  {
    स्थिर Chain<Types> *chain = &firstChain;
    अचिन्हित पूर्णांक गिनती = chain_गिनती_2;
    अगर (unlikely (!map->chain_flags.resize (गिनती)))
      वापस;
    क्रम (अचिन्हित पूर्णांक i = 0; i < गिनती; i++)
    {
      map->chain_flags[i].push (hb_aat_map_t::range_flags_t {chain->compile_flags (mapper),
							     mapper->range_first,
							     mapper->range_last});
      chain = &StructAfter<Chain<Types>> (*chain);
    }
  }

  अचिन्हित get_chain_गिनती () स्थिर
  {
    वापस chain_गिनती_2;
  }
  व्योम apply (hb_aat_apply_context_t *c,
	      स्थिर hb_aat_map_t &map,
	      स्थिर accelerator_t &accel) स्थिर
  {
    अगर (unlikely (!c->बफर->successful)) वापस;

    c->बफर->unsafe_to_concat ();

    c->setup_बफर_glyph_set ();

    c->set_lookup_सूचकांक (0);
    स्थिर Chain<Types> *chain = &firstChain;
    अचिन्हित पूर्णांक गिनती = chain_गिनती_2;
    क्रम (अचिन्हित पूर्णांक i = 0; i < गिनती; i++)
    {
      स्वतः *chain_accel = accel.get_accel (i, *chain, c->face->get_अंक_glyphs ());
      c->range_flags = &map.chain_flags[i];
      chain->apply (c, chain_accel);
      अगर (unlikely (!c->बफर->successful)) वापस;
      chain = &StructAfter<Chain<Types>> (*chain);
    }
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    अगर (!(version.sanitize (c) &&
	  hb_barrier () &&
	  version &&
	  chain_गिनती_2.sanitize (c)))
      return_trace (false);

    स्थिर Chain<Types> *chain = &firstChain;
    अचिन्हित पूर्णांक गिनती = chain_गिनती_2;
    क्रम (अचिन्हित पूर्णांक i = 0; i < गिनती; i++)
    {
      अगर (!chain->sanitize (c, version))
	return_trace (false);
      hb_barrier ();
      chain = &StructAfter<Chain<Types>> (*chain);
    }

    return_trace (true);
  }

  रक्षित:
  HBUINT16	version;	/* Version number of the glyph metamorphosis table.
				 * 1, 2, or 3. */
  HBUINT16	unused;		/* Set to 0. */
  HBUINT32	chain_गिनती_2;	/* Number of metamorphosis chains contained in this
				 * table. */
  Chain<Types>	firstChain;	/* Chains. */

  खुला:
  DEFINE_आकार_MIN (8);
};

काष्ठा morx : mortmorx<morx, ExtendedTypes, HB_AAT_TAG_morx>
{
  HB_INTERNAL bool is_blocklisted (hb_blob_t *blob,
                                   hb_face_t *face) स्थिर;
};

काष्ठा mort : mortmorx<mort, ObsoleteTypes, HB_AAT_TAG_mort>
{
  HB_INTERNAL bool is_blocklisted (hb_blob_t *blob,
                                   hb_face_t *face) स्थिर;
};

काष्ठा morx_accelerator_t : morx::accelerator_t {
  morx_accelerator_t (hb_face_t *face) : morx::accelerator_t (face) {}
};
काष्ठा mort_accelerator_t : mort::accelerator_t {
  mort_accelerator_t (hb_face_t *face) : mort::accelerator_t (face) {}
};


} /* namespace AAT */


#पूर्ण_अगर /* HB_AAT_LAYOUT_MORX_TABLE_HH */
