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

#ضمن "hb-open-type.hh"
#ضمن "hb-aat-layout-common.hh"
#ضمن "hb-ot-layout.hh"
#ضمن "hb-aat-map.hh"

/*
 * morx -- Extended Glyph Metamorphosis
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6morx.html
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6mort.html
 */
#define HB_AAT_TAG_morx HB_TAG('m','o','r','x')
#define HB_AAT_TAG_mort HB_TAG('m','o','r','t')


نطاق AAT {

using نطاق OT;

template <typename Types>
بنية RearrangementSubtable
{
  تعريفنوع typename Types::HBUINT HBUINT;

  تعريفنوع فراغ EntryData;

  تعداد Flags
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

  منطقي is_action_initiable (ثابت Entry<EntryData> &entry) ثابت
  {
    أرجع (entry.flags & MarkFirst);
  }
  منطقي is_actionable (ثابت Entry<EntryData> &entry) ثابت
  {
    أرجع (entry.flags & Verb);
  }

  بنية driver_context_t
  {
    ساكن constexpr منطقي in_place = true;

    driver_context_t (ثابت RearrangementSubtable *table_) :
	ret (خطأ),
	table (table_),
	start (0), end (0) {}

    HB_AAT_TRANSITION_INLINE
    فراغ transition (hb_buffer_t *buffer,
		     StateTableDriver<Types, EntryData, Flags> *driver,
		     ثابت Entry<EntryData> &entry)
    {
      unsigned صحيح flags = entry.flags;

      إذا (flags & MarkFirst)
	start = buffer->idx;

      إذا (flags & MarkLast)
	end = hb_min (buffer->idx + 1, buffer->len);

      إذا ((flags & Verb) && start < end)
      {
	/* The following map has two nibbles, for start-side
	 * and end-side. Values of 0,1,2 mean move that many
	 * to the other side. Value of 3 means move 2 and
	 * flip them. */
	ثابت unsigned حرف map[16] =
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

	unsigned صحيح m = map[flags & Verb];
	unsigned صحيح l = hb_min (2u, m >> 4);
	unsigned صحيح r = hb_min (2u, m & 0x0F);
	منطقي reverse_l = 3 == (m >> 4);
	منطقي reverse_r = 3 == (m & 0x0F);

	إذا (end - start >= l + r && end-start <= HB_MAX_CONTEXT_LENGTH)
	{
	  buffer->merge_clusters (start, hb_min (buffer->idx + 1, buffer->len));
	  buffer->merge_clusters (start, end);

	  hb_glyph_info_t *info = buffer->info;
	  hb_glyph_info_t buf[4];

	  hb_memcpy (buf, info + start, l * حجم (buf[0]));
	  hb_memcpy (buf + 2, info + end - r, r * حجم (buf[0]));

	  إذا (l != r)
	    memmove (info + start + r, info + start + l, (end - start - l - r) * حجم (buf[0]));

	  hb_memcpy (info + start, buf + 2, r * حجم (buf[0]));
	  hb_memcpy (info + end - l, buf, l * حجم (buf[0]));
	  إذا (reverse_l)
	  {
	    buf[0] = info[end - 1];
	    info[end - 1] = info[end - 2];
	    info[end - 2] = buf[0];
	  }
	  إذا (reverse_r)
	  {
	    buf[0] = info[start];
	    info[start] = info[start + 1];
	    info[start + 1] = buf[0];
	  }
	}
      }
    }

    عام:
    منطقي ret;
    ثابت RearrangementSubtable *table;
    خاص:
    unsigned صحيح start;
    unsigned صحيح end;
  };

  منطقي apply (hb_aat_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);

    driver_context_t dc (هذا);

    StateTableDriver<Types, EntryData, Flags> driver (machine, c->face);

    driver.drive (&dc, c);

    return_trace (dc.ret);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (machine.sanitize (c));
  }

  عام:
  StateTable<Types, EntryData>	machine;
  عام:
  DEFINE_SIZE_STATIC ((StateTable<Types, EntryData>::static_size));
};

template <typename Types>
بنية ContextualSubtable
{
  تعريفنوع typename Types::HBUINT HBUINT;

  بنية EntryData
  {
    تعريفنوع typename std::conditional<Types::extended, HBUINT16, HBINT16>::type OffsetType;

    OffsetType	markIndex;	/* Index/offset of the substitution table for the
				 * marked glyph. */
    OffsetType	currentIndex;	/* Index/offset of the substitution table for the
				 * current glyph. */
    عام:
    DEFINE_SIZE_STATIC (4);
  };

  تعداد Flags
  {
    SetMark		= 0x8000,	/* If set, make the current glyph the marked glyph. */
    DontAdvance		= 0x4000,	/* If set, don't advance to the next glyph before
					 * going to the new state. */
    Reserved		= 0x3FFF,	/* These bits are reserved and should be set to 0. */
  };

  منطقي is_action_initiable (ثابت Entry<EntryData> &entry) ثابت
  {
    أرجع (entry.flags & SetMark);
  }
  منطقي is_actionable (ثابت Entry<EntryData> &entry) ثابت
  {
    إذا (Types::extended)
      أرجع entry.data.markIndex != 0xFFFF || entry.data.currentIndex != 0xFFFF;
    وإلا
      أرجع entry.data.markIndex || entry.data.currentIndex;
  }

  بنية driver_context_t
  {
    ساكن constexpr منطقي in_place = true;

    driver_context_t (ثابت ContextualSubtable *table_,
			     hb_aat_apply_context_t *c_) :
	ret (خطأ),
	c (c_),
	table (table_),
	mark_set (خطأ),
	mark (0),
	subs (table+table->substitutionTables) {}

    HB_AAT_TRANSITION_INLINE
    فراغ transition (hb_buffer_t *buffer,
		     StateTableDriver<Types, EntryData, Flags> *driver,
		     ثابت Entry<EntryData> &entry)
    {
      /* Looks like CoreText applies neither mark nor current substitution for
       * end-of-text if mark was not explicitly set. */
      إذا (buffer->idx == buffer->len && !mark_set)
	أرجع;

      ثابت HBGlyphID16 *replacement;

      replacement = nullptr;
      إذا (Types::extended)
      {
	إذا (entry.data.markIndex != 0xFFFF)
	{
	  ثابت Lookup<HBGlyphID16> &lookup = subs[entry.data.markIndex];
	  replacement = lookup.get_بەها (buffer->info[mark].codepoint, driver->ژمارە_glyphs);
	}
      }
      وإلا
      {
	إذا (entry.data.markIndex)
	{
	  صحيح offset = (صحيح) entry.data.markIndex + buffer->info[mark].codepoint;
	  ثابت UnsizedArrayOf<HBGlyphID16> &subs_old = (ثابت UnsizedArrayOf<HBGlyphID16> &) subs;
	  replacement = &subs_old[Types::wordOffsetToIndex (offset, table, subs_old.arrayZ)];
	  إذا (!(replacement->sanitize (&c->sanitizer) &&
		hb_barrier () &&
		*replacement))
	    replacement = nullptr;
	}
      }
      إذا (replacement)
      {
	buffer->unsafe_to_break (mark, hb_min (buffer->idx + 1, buffer->len));
	c->replace_glyph_inplace (mark, *replacement);
	ret = true;
      }

      replacement = nullptr;
      unsigned صحيح idx = hb_min (buffer->idx, buffer->len - 1);
      إذا (Types::extended)
      {
	إذا (entry.data.currentIndex != 0xFFFF)
	{
	  ثابت Lookup<HBGlyphID16> &lookup = subs[entry.data.currentIndex];
	  replacement = lookup.get_بەها (buffer->info[idx].codepoint, driver->ژمارە_glyphs);
	}
      }
      وإلا
      {
	إذا (entry.data.currentIndex)
	{
	  صحيح offset = (صحيح) entry.data.currentIndex + buffer->info[idx].codepoint;
	  ثابت UnsizedArrayOf<HBGlyphID16> &subs_old = (ثابت UnsizedArrayOf<HBGlyphID16> &) subs;
	  replacement = &subs_old[Types::wordOffsetToIndex (offset, table, subs_old.arrayZ)];
	  إذا (!(replacement->sanitize (&c->sanitizer) &&
		hb_barrier () &&
		*replacement))
	    replacement = nullptr;
	}
      }
      إذا (replacement)
      {
	c->replace_glyph_inplace (idx, *replacement);
	ret = true;
      }

      إذا (entry.flags & SetMark)
      {
	mark_set = true;
	mark = buffer->idx;
      }
    }

    عام:
    منطقي ret;
    hb_aat_apply_context_t *c;
    ثابت ContextualSubtable *table;
    خاص:
    منطقي mark_set;
    unsigned صحيح mark;
    ثابت UnsizedListOfOffset16To<Lookup<HBGlyphID16>, HBUINT, فراغ, خطأ> &subs;
  };

  منطقي apply (hb_aat_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);

    driver_context_t dc (هذا, c);

    StateTableDriver<Types, EntryData, Flags> driver (machine, c->face);

    driver.drive (&dc, c);

    return_trace (dc.ret);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);

    unsigned صحيح ژمارە_entries = 0;
    إذا (unlikely (!machine.sanitize (c, &ژمارە_entries))) return_trace (خطأ);
    hb_barrier ();

    إذا (!Types::extended)
      return_trace (substitutionTables.sanitize (c, هذا, 0));

    unsigned صحيح ژمارە_lookups = 0;

    ثابت Entry<EntryData> *entries = machine.get_entries ();
    لكل (unsigned صحيح i = 0; i < ژمارە_entries; i++)
    {
      ثابت EntryData &data = entries[i].data;

      إذا (data.markIndex != 0xFFFF)
	ژمارە_lookups = hb_max (ژمارە_lookups, 1u + data.markIndex);
      إذا (data.currentIndex != 0xFFFF)
	ژمارە_lookups = hb_max (ژمارە_lookups, 1u + data.currentIndex);
    }

    return_trace (substitutionTables.sanitize (c, هذا, ژمارە_lookups));
  }

  عام:
  StateTable<Types, EntryData>
		machine;
  محمي:
  NNOffsetTo<UnsizedListOfOffset16To<Lookup<HBGlyphID16>, HBUINT, فراغ, خطأ>, HBUINT>
		substitutionTables;
  عام:
  DEFINE_SIZE_STATIC ((StateTable<Types, EntryData>::static_size + HBUINT::static_size));
};


template <منطقي extended>
بنية LigatureEntry;

template <>
بنية LigatureEntry<true>
{

  بنية EntryData
  {
    HBUINT16	ligActionIndex;	/* Index to the first ligActionTable entry
				 * for processing this group, if indicated
				 * by the flags. */
    عام:
    DEFINE_SIZE_STATIC (2);
  };

  تعداد Flags
  {
    SetComponent	= 0x8000,	/* Push this glyph onto the component stack for
					 * eventual processing. */
    DontAdvance		= 0x4000,	/* Leave the glyph pointer at this glyph for the
					   next iteration. */
    PerformAction	= 0x2000,	/* Use the ligActionIndex to process a ligature
					 * group. */
    Reserved		= 0x1FFF,	/* These bits are reserved and should be set to 0. */
  };

  ساكن منطقي initiateAction (ثابت Entry<EntryData> &entry)
  { أرجع entry.flags & SetComponent; }

  ساكن منطقي performAction (ثابت Entry<EntryData> &entry)
  { أرجع entry.flags & PerformAction; }

  ساكن unsigned صحيح ligActionIndex (ثابت Entry<EntryData> &entry)
  { أرجع entry.data.ligActionIndex; }
};
template <>
بنية LigatureEntry<خطأ>
{
  تعريفنوع فراغ EntryData;

  تعداد Flags
  {
    SetComponent	= 0x8000,	/* Push this glyph onto the component stack for
					 * eventual processing. */
    DontAdvance		= 0x4000,	/* Leave the glyph pointer at this glyph for the
					   next iteration. */
    Offset		= 0x3FFF,	/* Byte offset from beginning of subtable to the
					 * ligature action list. This value must be a
					 * multiple of 4. */
  };

  ساكن منطقي initiateAction (ثابت Entry<EntryData> &entry)
  { أرجع entry.flags & SetComponent; }

  ساكن منطقي performAction (ثابت Entry<EntryData> &entry)
  { أرجع entry.flags & Offset; }

  ساكن unsigned صحيح ligActionIndex (ثابت Entry<EntryData> &entry)
  { أرجع entry.flags & Offset; }
};


template <typename Types>
بنية LigatureSubtable
{
  تعريفنوع typename Types::HBUINT HBUINT;

  تعريفنوع LigatureEntry<Types::extended> LigatureEntryT;
  تعريفنوع typename LigatureEntryT::EntryData EntryData;

  تعداد Flags
  {
    DontAdvance	= LigatureEntryT::DontAdvance,
  };

  منطقي is_action_initiable (ثابت Entry<EntryData> &entry) ثابت
  {
    أرجع LigatureEntryT::initiateAction (entry);
  }
  منطقي is_actionable (ثابت Entry<EntryData> &entry) ثابت
  {
    أرجع LigatureEntryT::performAction (entry);
  }

  بنية driver_context_t
  {
    ساكن constexpr منطقي in_place = خطأ;
    تعداد LigActionFlags
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

    driver_context_t (ثابت LigatureSubtable *table_,
		      hb_aat_apply_context_t *c_) :
	ret (خطأ),
	c (c_),
	table (table_),
	ligAction (table+table->ligAction),
	component (table+table->component),
	ligature (table+table->ligature),
	match_length (0) {}

    HB_AAT_TRANSITION_INLINE
    فراغ transition (hb_buffer_t *buffer,
		     StateTableDriver<Types, EntryData, Flags> *driver,
		     ثابت Entry<EntryData> &entry)
    {
      DEBUG_پەیام (APPLY, nullptr, "Ligature transition at %u", buffer->idx);
      إذا (entry.flags & LigatureEntryT::SetComponent)
      {
	/* Never mark same index twice, in case DontAdvance was used... */
	إذا (unlikely (match_length && match_positions[(match_length - 1u) % ARRAY_LENGTH (match_positions)] == buffer->out_len))
	  match_length--;

	match_positions[match_length++ % ARRAY_LENGTH (match_positions)] = buffer->out_len;
	DEBUG_پەیام (APPLY, nullptr, "Set component at %u", buffer->out_len);
      }

      إذا (LigatureEntryT::performAction (entry))
      {
	DEBUG_پەیام (APPLY, nullptr, "Perform action with %u", match_length);
	unsigned صحيح end = buffer->out_len;

	إذا (unlikely (!match_length))
	  أرجع;

	إذا (buffer->idx >= buffer->len)
	  أرجع; /* TODO Work on previous instead? */

	unsigned صحيح cursor = match_length;

	unsigned صحيح action_idx = LigatureEntryT::ligActionIndex (entry);
	action_idx = Types::offsetToIndex (action_idx, table, ligAction.arrayZ);
	ثابت HBUINT32 *actionData = &ligAction[action_idx];

	unsigned صحيح ligature_idx = 0;
	unsigned صحيح action;
	افعل
	{
	  إذا (unlikely (!cursor))
	  {
	    /* Stack underflow.  Clear the stack. */
	    DEBUG_پەیام (APPLY, nullptr, "Stack underflow");
	    match_length = 0;
	    اكسر;
	  }

	  DEBUG_پەیام (APPLY, nullptr, "Moving to stack position %u", cursor - 1);
	  إذا (unlikely (!buffer->move_to (match_positions[--cursor % ARRAY_LENGTH (match_positions)]))) أرجع;

	  إذا (unlikely (!actionData->sanitize (&c->sanitizer))) اكسر;
	  hb_barrier ();
	  action = *actionData;

	  uint32_t uoffset = action & LigActionOffset;
	  إذا (uoffset & 0x20000000)
	    uoffset |= 0xC0000000; /* Sign-extend. */
	  int32_t offset = (int32_t) uoffset;
	  unsigned صحيح component_idx = buffer->cur().codepoint + offset;
	  component_idx = Types::wordOffsetToIndex (component_idx, table, component.arrayZ);
	  ثابت HBUINT16 &componentData = component[component_idx];
	  إذا (unlikely (!componentData.sanitize (&c->sanitizer))) اكسر;
	  hb_barrier ();
	  ligature_idx += componentData;

	  DEBUG_پەیام (APPLY, nullptr, "Action store %d last %d",
		     منطقي (action & LigActionStore),
		     منطقي (action & LigActionLast));
	  إذا (action & (LigActionStore | LigActionLast))
	  {
	    ligature_idx = Types::offsetToIndex (ligature_idx, table, ligature.arrayZ);
	    ثابت HBGlyphID16 &ligatureData = ligature[ligature_idx];
	    إذا (unlikely (!ligatureData.sanitize (&c->sanitizer))) اكسر;
	    hb_barrier ();
	    hb_codepoint_t lig = ligatureData;

	    DEBUG_پەیام (APPLY, nullptr, "Produced ligature %u", lig);
	    إذا (unlikely (!c->replace_glyph (lig))) أرجع;

	    unsigned صحيح lig_end = match_positions[(match_length - 1u) % ARRAY_LENGTH (match_positions)] + 1u;
	    /* Now go and delete all subsequent components. */
	    طالما (match_length - 1u > cursor)
	    {
	      DEBUG_پەیام (APPLY, nullptr, "Skipping ligature component");
	      إذا (unlikely (!buffer->move_to (match_positions[--match_length % ARRAY_LENGTH (match_positions)]))) أرجع;
	      إذا (!c->delete_glyph ()) أرجع;
	    }

	    إذا (unlikely (!buffer->move_to (lig_end))) أرجع;
	    buffer->merge_out_clusters (match_positions[cursor % ARRAY_LENGTH (match_positions)], buffer->out_len);
	  }

	  actionData++;
	}
	طالما (!(action & LigActionLast));
	إذا (unlikely (!buffer->move_to (end))) أرجع;
      }
    }

    عام:
    منطقي ret;
    hb_aat_apply_context_t *c;
    ثابت LigatureSubtable *table;
    خاص:
    ثابت UnsizedArrayOf<HBUINT32> &ligAction;
    ثابت UnsizedArrayOf<HBUINT16> &component;
    ثابت UnsizedArrayOf<HBGlyphID16> &ligature;
    unsigned صحيح match_length;
    unsigned صحيح match_positions[HB_MAX_CONTEXT_LENGTH];
  };

  منطقي apply (hb_aat_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);

    driver_context_t dc (هذا, c);

    StateTableDriver<Types, EntryData, Flags> driver (machine, c->face);

    driver.drive (&dc, c);

    return_trace (dc.ret);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    /* The rest of array sanitizations are done at run-time. */
    return_trace (c->check_struct (هذا) && machine.sanitize (c) &&
		  hb_barrier () &&
		  ligAction && component && ligature);
  }

  عام:
  StateTable<Types, EntryData>
		machine;
  محمي:
  NNOffsetTo<UnsizedArrayOf<HBUINT32>, HBUINT>
		ligAction;	/* Offset to the ligature action table. */
  NNOffsetTo<UnsizedArrayOf<HBUINT16>, HBUINT>
		component;	/* Offset to the component table. */
  NNOffsetTo<UnsizedArrayOf<HBGlyphID16>, HBUINT>
		ligature;	/* Offset to the actual ligature lists. */
  عام:
  DEFINE_SIZE_STATIC ((StateTable<Types, EntryData>::static_size + 3 * HBUINT::static_size));
};

template <typename Types>
بنية NoncontextualSubtable
{
  منطقي apply (hb_aat_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);

    منطقي ret = خطأ;
    unsigned صحيح ژمارە_glyphs = c->face->get_ژمارە_glyphs ();

    hb_glyph_info_t *info = c->buffer->info;
    unsigned صحيح ژماردن = c->buffer->len;
    // If there's only one range, we already checked the flag.
    auto *last_range = c->range_flags && (c->range_flags->length > 1) ? &(*c->range_flags)[0] : nullptr;
    لكل (unsigned صحيح i = 0; i < ژماردن; i++)
    {
      /* This block copied from StateTableDriver::drive. Keep in sync. */
      إذا (unlikely (last_range))
      {
	auto *range = last_range;
	{
	  unsigned cluster = info[i].cluster;
	  طالما (cluster < range->cluster_first)
	    range--;
	  طالما (cluster > range->cluster_last)
	    range++;

	  last_range = range;
	}
	إذا (!(range->flags & c->subtable_flags))
	  استمر;
      }

      ثابت HBGlyphID16 *replacement = substitute.get_بەها (info[i].codepoint, ژمارە_glyphs);
      إذا (replacement)
      {
	c->replace_glyph_inplace (i, *replacement);
	ret = true;
      }
    }

    return_trace (ret);
  }

  template <typename set_t>
  فراغ collect_initial_glyphs (set_t &glyphs, unsigned ژمارە_glyphs) ثابت
  {
    substitute.collect_glyphs (glyphs, ژمارە_glyphs);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (substitute.sanitize (c));
  }

  محمي:
  Lookup<HBGlyphID16>	substitute;
  عام:
  DEFINE_SIZE_MIN (2);
};

template <typename Types>
بنية InsertionSubtable
{
  تعريفنوع typename Types::HBUINT HBUINT;

  بنية EntryData
  {
    HBUINT16	current_زیادکردن_Index;	/* Zero-based index into the insertion glyph table.
					 * The number of glyphs to be inserted is contained
					 * in the currentInsertCount field in the flags.
					 * A value of 0xFFFF indicates no insertion is to
					 * be done. */
    HBUINT16	marked_زیادکردن_Index;	/* Zero-based index into the insertion glyph table.
					 * The number of glyphs to be inserted is contained
					 * in the markedInsertCount field in the flags.
					 * A value of 0xFFFF indicates no insertion is to
					 * be done. */
    عام:
    DEFINE_SIZE_STATIC (4);
  };

  تعداد Flags
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
    Current_زیادکردن_Before= 0x0800,      /* If set, specifies that insertions are to be made
				       * to the left of the current glyph. If clear,
				       * they're made to the right of the current glyph. */
    Marked_زیادکردن_Before= 0x0400,	      /* If set, specifies that insertions are to be
				       * made to the left of the marked glyph. If clear,
				       * they're made to the right of the marked glyph. */
    Current_زیادکردن_ژماردن= 0x3E0,	      /* This 5-bit field is treated as a count of the
				       * number of glyphs to insert at the current
				       * position. Since zero means no insertions, the
				       * largest number of insertions at any given
				       * current location is 31 glyphs. */
    Marked_زیادکردن_ژماردن= 0x001F,	      /* This 5-bit field is treated as a count of the
				       * number of glyphs to insert at the marked
				       * position. Since zero means no insertions, the
				       * largest number of insertions at any given
				       * marked location is 31 glyphs. */
  };

  منطقي is_action_initiable (ثابت Entry<EntryData> &entry) ثابت
  {
    أرجع (entry.flags & SetMark);
  }
  منطقي is_actionable (ثابت Entry<EntryData> &entry) ثابت
  {
    أرجع (entry.flags & (Current_زیادکردن_ژماردن | Marked_زیادکردن_ژماردن)) &&
	   (entry.data.current_زیادکردن_Index != 0xFFFF ||entry.data.marked_زیادکردن_Index != 0xFFFF);
  }

  بنية driver_context_t
  {
    ساكن constexpr منطقي in_place = خطأ;

    driver_context_t (ثابت InsertionSubtable *table_,
		      hb_aat_apply_context_t *c_) :
	ret (خطأ),
	c (c_),
	table (table_),
	mark (0),
	insertionAction (table+table->insertionAction) {}

    HB_AAT_TRANSITION_INLINE
    فراغ transition (hb_buffer_t *buffer,
		     StateTableDriver<Types, EntryData, Flags> *driver,
		     ثابت Entry<EntryData> &entry)
    {
      unsigned صحيح flags = entry.flags;

      unsigned mark_loc = buffer->out_len;

      إذا (entry.data.marked_زیادکردن_Index != 0xFFFF)
      {
	unsigned صحيح ژماردن = (flags & Marked_زیادکردن_ژماردن);
	إذا (unlikely ((buffer->max_ops -= ژماردن) <= 0)) أرجع;
	unsigned صحيح start = entry.data.marked_زیادکردن_Index;
	ثابت HBGlyphID16 *glyphs = &insertionAction[start];
	إذا (unlikely (!c->sanitizer.check_array (glyphs, ژماردن))) ژماردن = 0;
	hb_barrier ();

	منطقي before = flags & Marked_زیادکردن_Before;

	unsigned صحيح end = buffer->out_len;
	إذا (unlikely (!buffer->move_to (mark))) أرجع;

	إذا (buffer->idx < buffer->len && !before)
	  إذا (unlikely (!buffer->copy_glyph ())) أرجع;
	/* TODO We ignore KashidaLike setting. */
	إذا (unlikely (!c->output_glyphs (ژماردن, glyphs))) أرجع;
	ret = true;
	إذا (buffer->idx < buffer->len && !before)
	  buffer->skip_glyph ();

	إذا (unlikely (!buffer->move_to (end + ژماردن))) أرجع;

	buffer->unsafe_to_break_from_outbuffer (mark, hb_min (buffer->idx + 1, buffer->len));
      }

      إذا (flags & SetMark)
	mark = mark_loc;

      إذا (entry.data.current_زیادکردن_Index != 0xFFFF)
      {
	unsigned صحيح ژماردن = (flags & Current_زیادکردن_ژماردن) >> 5;
	إذا (unlikely ((buffer->max_ops -= ژماردن) <= 0)) أرجع;
	unsigned صحيح start = entry.data.current_زیادکردن_Index;
	ثابت HBGlyphID16 *glyphs = &insertionAction[start];
	إذا (unlikely (!c->sanitizer.check_array (glyphs, ژماردن))) ژماردن = 0;
	hb_barrier ();

	منطقي before = flags & Current_زیادکردن_Before;

	unsigned صحيح end = buffer->out_len;

	إذا (buffer->idx < buffer->len && !before)
	  إذا (unlikely (!buffer->copy_glyph ())) أرجع;
	/* TODO We ignore KashidaLike setting. */
	إذا (unlikely (!c->output_glyphs (ژماردن, glyphs))) أرجع;
	ret = true;
	إذا (buffer->idx < buffer->len && !before)
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
	إذا (unlikely (!buffer->move_to ((flags & DontAdvance) ? end : end + ژماردن))) أرجع;
      }
    }

    عام:
    منطقي ret;
    hb_aat_apply_context_t *c;
    ثابت InsertionSubtable *table;
    خاص:
    unsigned صحيح mark;
    ثابت UnsizedArrayOf<HBGlyphID16> &insertionAction;
  };

  منطقي apply (hb_aat_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);

    driver_context_t dc (هذا, c);

    StateTableDriver<Types, EntryData, Flags> driver (machine, c->face);

    driver.drive (&dc, c);

    return_trace (dc.ret);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    /* The rest of array sanitizations are done at run-time. */
    return_trace (c->check_struct (هذا) && machine.sanitize (c) &&
		  hb_barrier () &&
		  insertionAction);
  }

  عام:
  StateTable<Types, EntryData>
		machine;
  محمي:
  NNOffsetTo<UnsizedArrayOf<HBGlyphID16>, HBUINT>
		insertionAction;	/* Byte offset from stateHeader to the start of
					 * the insertion glyph table. */
  عام:
  DEFINE_SIZE_STATIC ((StateTable<Types, EntryData>::static_size + HBUINT::static_size));
};


بنية Feature
{
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  عام:
  HBUINT16	featureType;	/* The type of feature. */
  HBUINT16	featureSetting;	/* The feature's setting (aka selector). */
  HBUINT32	enableFlags;	/* Flags for the settings that this feature
				 * and setting enables. */
  HBUINT32	disableFlags;	/* Complement of flags for the settings that this
				 * feature and setting disable. */

  عام:
  DEFINE_SIZE_STATIC (12);
};


بنية hb_accelerate_subtables_context_t :
       hb_dispatch_context_t<hb_accelerate_subtables_context_t>
{
  بنية hb_applicable_t
  {
    friend بنية hb_accelerate_subtables_context_t;
    friend بنية hb_aat_layout_lookup_accelerator_t;

    عام:
    hb_bit_set_t glyph_set;
    mutable hb_aat_class_cache_t class_cache;

    template <typename T>
    auto init_ (ثابت T &obj_, unsigned ژمارە_glyphs, hb_priority<1>) HB_AUTO_RETURN
    (
      obj_.machine.collect_initial_glyphs (glyph_set, ژمارە_glyphs, obj_)
    )

    template <typename T>
    فراغ init_ (ثابت T &obj_, unsigned ژمارە_glyphs, hb_priority<0>)
    {
      obj_.collect_initial_glyphs (glyph_set, ژمارە_glyphs);
    }

    template <typename T>
    فراغ init (ثابت T &obj_, unsigned ژمارە_glyphs)
    {
      glyph_set.init ();
      init_ (obj_, ژمارە_glyphs, hb_prioritize);
      class_cache.clear ();
    }

    فراغ
    fini ()
    {
      glyph_set.fini ();
    }
  };

  /* Dispatch interface. */
  template <typename T>
  return_t dispatch (ثابت T &obj)
  {
    hb_applicable_t *entry = &array[i++];

    entry->init (obj, ژمارە_glyphs);

    أرجع hb_empty_t ();
  }
  ساكن return_t default_return_بەها () { أرجع hb_empty_t (); }

  منطقي stop_sublookup_iteration (return_t r) ثابت { أرجع خطأ; }

  hb_accelerate_subtables_context_t (hb_applicable_t *array_, unsigned ژمارە_glyphs_2) :
				     hb_dispatch_context_t<hb_accelerate_subtables_context_t> (),
				     array (array_), ژمارە_glyphs (ژمارە_glyphs_2) {}

  hb_applicable_t *array;
  unsigned ژمارە_glyphs;
  unsigned i = 0;
};

بنية hb_aat_layout_chain_accelerator_t
{
  template <typename TChain>
  ساكن hb_aat_layout_chain_accelerator_t *create (ثابت TChain &chain, unsigned ژمارە_glyphs)
  {
    unsigned ژماردن = chain.get_subtable_ژماردن ();

    unsigned product;
    إذا (unlikely (hb_unsigned_mul_overflows (ژماردن,
        حجم (hb_accelerate_subtables_context_t::hb_applicable_t), &product)))
      أرجع nullptr;

    unsigned size;
    إذا (unlikely (hb_unsigned_add_overflows (حجم (hb_aat_layout_chain_accelerator_t) -
        HB_VAR_ARRAY * حجم (hb_accelerate_subtables_context_t::hb_applicable_t), product, &size))) {
      أرجع nullptr;
    }

    /* The following is a calloc because when we are collecting subtables,
     * some of them might be invalid and hence not collect; as a result,
     * we might not fill in all the count entries of the subtables array.
     * Zeroing it allows the set digest to gatekeep it without having to
     * initialize it further. */
    auto *thiz = (hb_aat_layout_chain_accelerator_t *) hb_calloc (1, size);
    إذا (unlikely (!thiz))
      أرجع nullptr;

    thiz->ژماردن = ژماردن;

    hb_accelerate_subtables_context_t c_accelerate_subtables (thiz->subtables, ژمارە_glyphs);
    chain.dispatch (&c_accelerate_subtables);

    أرجع thiz;
  }

  فراغ destroy ()
  {
    لكل (unsigned i = 0; i < ژماردن; i++)
      subtables[i].fini ();
  }

  unsigned ژماردن;
  hb_accelerate_subtables_context_t::hb_applicable_t subtables[HB_VAR_ARRAY];
};

template <typename Types>
بنية ChainSubtable
{
  تعريفنوع typename Types::HBUINT HBUINT;

  template <typename T>
  friend بنية Chain;

  size_t get_size () ثابت     { أرجع length; }
  unsigned صحيح get_type () ثابت     { أرجع coverage & 0xFF; }
  unsigned صحيح get_coverage () ثابت { أرجع coverage >> (حجم (HBUINT) * 8 - 8); }

  تعداد Coverage
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
  تعداد Type
  {
    Rearrangement	= 0,
    Contextual		= 1,
    Ligature		= 2,
    Noncontextual	= 4,
    Insertion		= 5
  };

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    unsigned صحيح subtable_type = get_type ();
    TRACE_DISPATCH (هذا, subtable_type);
    بدل (subtable_type) {
    حالة Rearrangement:		hb_barrier (); return_trace (c->dispatch (u.rearrangement, std::forward<Ts> (ds)...));
    حالة Contextual:		hb_barrier (); return_trace (c->dispatch (u.contextual, std::forward<Ts> (ds)...));
    حالة Ligature:		hb_barrier (); return_trace (c->dispatch (u.ligature, std::forward<Ts> (ds)...));
    حالة Noncontextual:		hb_barrier (); return_trace (c->dispatch (u.noncontextual, std::forward<Ts> (ds)...));
    حالة Insertion:		hb_barrier (); return_trace (c->dispatch (u.insertion, std::forward<Ts> (ds)...));
    افتراضي:			return_trace (c->default_return_بەها ());
    }
  }

  منطقي apply (hb_aat_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);
    // Disabled for https://github.com/harfbuzz/harfbuzz/issues/4873
    //hb_sanitize_with_object_t with (&c->sanitizer, this);
    return_trace (dispatch (c));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (!(length.sanitize (c) &&
	  hb_barrier () &&
	  length >= min_size &&
	  c->check_range (هذا, length)))
      return_trace (خطأ);

    // Disabled for https://github.com/harfbuzz/harfbuzz/issues/4873
    //hb_sanitize_with_object_t with (c, this);
    return_trace (dispatch (c));
  }

  محمي:
  HBUINT	length;		/* Total subtable length, including this header. */
  HBUINT	coverage;	/* Coverage flags and subtable type. */
  HBUINT32	subFeatureFlags;/* The 32-bit mask identifying which subtable this is. */
  اتحاد {
  RearrangementSubtable<Types>	rearrangement;
  ContextualSubtable<Types>	contextual;
  LigatureSubtable<Types>	ligature;
  NoncontextualSubtable<Types>	noncontextual;
  InsertionSubtable<Types>	insertion;
  } u;
  عام:
  DEFINE_SIZE_MIN (2 * حجم (HBUINT) + 4);
};

template <typename Types>
بنية Chain
{
  تعريفنوع typename Types::HBUINT HBUINT;

  unsigned get_subtable_ژماردن () ثابت { أرجع subtable_ژماردن_2; }

  hb_mask_t compile_flags (ثابت hb_aat_map_builder_t *map) ثابت
  {
    hb_mask_t flags = defaultFlags;
    {
      unsigned صحيح ژماردن = feature_ژماردن;
      لكل (unsigned i = 0; i < ژماردن; i++)
      {
	ثابت Feature &feature = featureZ[i];
	hb_aat_layout_feature_type_t type = (hb_aat_layout_feature_type_t) (unsigned صحيح) feature.featureType;
	hb_aat_layout_feature_selector_t setting = (hb_aat_layout_feature_selector_t) (unsigned صحيح) feature.featureSetting;
      retry:
	// Check whether this type/setting pair was requested in the map, and if so, apply its flags.
	// (The search here only looks at the type and setting fields of feature_info_t.)
	hb_aat_map_builder_t::feature_info_t info = { type, setting, خطأ, 0 };
	إذا (map->current_features.bsearch (info))
	{
	  flags &= feature.disableFlags;
	  flags |= feature.enableFlags;
	}
	وإلا إذا (type == HB_AAT_LAYOUT_FEATURE_TYPE_LETTER_CASE && setting == HB_AAT_LAYOUT_FEATURE_SELECTOR_SMALL_CAPS)
	{
	  /* Deprecated. https://github.com/harfbuzz/harfbuzz/issues/1342 */
	  type = HB_AAT_LAYOUT_FEATURE_TYPE_LOWER_CASE;
	  setting = HB_AAT_LAYOUT_FEATURE_SELECTOR_LOWER_CASE_SMALL_CAPS;
	  اذهب retry;
	}
#ifndef HB_NO_AAT
	وإلا إذا (type == HB_AAT_LAYOUT_FEATURE_TYPE_LANGUAGE_TAG_TYPE && setting &&
		 /* TODO: Rudimentary language matching. */
		 hb_language_matches (map->face->table.ltag->get_language (setting - 1), map->props.language))
	{
	  flags &= feature.disableFlags;
	  flags |= feature.enableFlags;
	}
#endif
      }
    }
    أرجع flags;
  }

  فراغ apply (hb_aat_apply_context_t *c,
	      ثابت hb_aat_layout_chain_accelerator_t *accel) ثابت
  {
    ثابت ChainSubtable<Types> *subtable = &StructAfter<ChainSubtable<Types>> (featureZ.as_array (feature_ژماردن));
    unsigned صحيح ژماردن = subtable_ژماردن_2;
    لكل (unsigned صحيح i = 0; i < ژماردن; i++)
    {
      منطقي reverse;

      auto coverage = subtable->get_coverage ();

      hb_mask_t subtable_flags = subtable->subFeatureFlags;
      إذا (hb_none (hb_iter (c->range_flags) |
		   hb_map ([subtable_flags] (ثابت hb_aat_map_t::range_flags_t _) -> منطقي { أرجع subtable_flags & (_.flags); })))
	اذهب skip;

      إذا (!(coverage & ChainSubtable<Types>::AllDirections) &&
	  HB_DIRECTION_IS_VERTICAL (c->buffer->props.direction) !=
	  منطقي (coverage & ChainSubtable<Types>::Vertical))
	اذهب skip;

      c->subtable_flags = subtable_flags;
      c->first_set = accel ? &accel->subtables[i].glyph_set : &Null(hb_bit_set_t);
      c->machine_class_cache = accel ? &accel->subtables[i].class_cache : nullptr;

      إذا (!c->buffer_intersects_machine ())
      {
	(فراغ) c->buffer->message (c->font, "skipped chainsubtable %u because no glyph matches", c->lookup_index);
	اذهب skip;
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
		منطقي (coverage & ChainSubtable<Types>::Backwards) :
		منطقي (coverage & ChainSubtable<Types>::Backwards) !=
		HB_DIRECTION_IS_BACKWARD (c->buffer->props.direction);

      إذا (!c->buffer->message (c->font, "start chainsubtable %u", c->lookup_index))
	اذهب skip;

      إذا (reverse != c->buffer_is_reversed)
        c->reverse_buffer ();

      subtable->apply (c);

      (فراغ) c->buffer->message (c->font, "end chainsubtable %u", c->lookup_index);

      إذا (unlikely (!c->buffer->successful)) اكسر;

    skip:
      subtable = &StructAfter<ChainSubtable<Types>> (*subtable);
      c->set_lookup_index (c->lookup_index + 1);
    }
    إذا (c->buffer_is_reversed)
      c->reverse_buffer ();
  }

  size_t get_size () ثابت { أرجع length; }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    ثابت ChainSubtable<Types> *subtable = &StructAfter<ChainSubtable<Types>> (featureZ.as_array (feature_ژماردن));
    unsigned صحيح ژماردن = subtable_ژماردن_2;
    لكل (unsigned صحيح i = 0; i < ژماردن; i++)
    {
      typename context_t::return_t ret = subtable->dispatch (c, std::forward<Ts> (ds)...);
      إذا (c->stop_sublookup_iteration (ret))
	أرجع ret;
      subtable = &StructAfter<ChainSubtable<Types>> (*subtable);
    }
    أرجع c->default_return_بەها ();
  }

  منطقي sanitize (hb_sanitize_context_t *c, unsigned صحيح version) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (!(length.sanitize (c) &&
	  hb_barrier () &&
	  length >= min_size &&
	  c->check_range (هذا, length)))
      return_trace (خطأ);

    إذا (!c->check_array (featureZ.arrayZ, feature_ژماردن))
      return_trace (خطأ);

    ثابت ChainSubtable<Types> *subtable = &StructAfter<ChainSubtable<Types>> (featureZ.as_array (feature_ژماردن));
    unsigned صحيح ژماردن = subtable_ژماردن_2;
    لكل (unsigned صحيح i = 0; i < ژماردن; i++)
    {
      إذا (!subtable->sanitize (c))
	return_trace (خطأ);
      hb_barrier ();
      subtable = &StructAfter<ChainSubtable<Types>> (*subtable);
    }

    إذا (version >= 3)
    {
      ثابت SubtableGlyphCoverage *coverage = (ثابت SubtableGlyphCoverage *) subtable;
      إذا (!coverage->sanitize (c, ژماردن))
        return_trace (خطأ);
    }

    return_trace (true);
  }

  محمي:
  HBUINT32	defaultFlags;	/* The default specification for subtables. */
  HBUINT32	length;		/* Total byte count, including this header. */
  HBUINT	feature_ژماردن;	/* Number of feature subtable entries. */
  HBUINT	subtable_ژماردن_2;	/* The number of subtables in the chain. */

  UnsizedArrayOf<Feature>	featureZ;	/* Features. */
/*ChainSubtable	firstSubtable;*//* Subtables. */
/*SubtableGlyphCoverage coverages*//* Only if version >= 3. */

  عام:
  DEFINE_SIZE_MIN (8 + 2 * حجم (HBUINT));
};


/*
 * The 'mort'/'morx' Table
 */

template <typename T, typename Types, hb_tag_t TAG>
بنية mortmorx
{
  ساكن constexpr hb_tag_t tableTag = TAG;

  منطقي has_data () ثابت { أرجع version != 0; }

  بنية accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      hb_sanitize_context_t sc;
      هذا->table = sc.reference_table<T> (face);

      إذا (unlikely (هذا->table->is_blocklisted (هذا->table.get_blob (), face)))
      {
        hb_blob_destroy (هذا->table.get_blob ());
        هذا->table = hb_blob_get_empty ();
      }

      هذا->chain_ژماردن = table->get_chain_ژماردن ();

      هذا->accels = (hb_atomic_t<hb_aat_layout_chain_accelerator_t *> *) hb_calloc (هذا->chain_ژماردن, حجم (*accels));
      إذا (unlikely (!هذا->accels))
      {
	هذا->chain_ژماردن = 0;
	هذا->table.destroy ();
	هذا->table = hb_blob_get_empty ();
      }
    }
    ~accelerator_t ()
    {
      لكل (unsigned صحيح i = 0; i < هذا->chain_ژماردن; i++)
      {
	إذا (هذا->accels[i])
	  هذا->accels[i]->destroy ();
	hb_free (هذا->accels[i]);
      }
      hb_free (هذا->accels);
      هذا->table.destroy ();
    }

    hb_blob_t *get_blob () ثابت { أرجع table.get_blob (); }

    template <typename Chain>
    hb_aat_layout_chain_accelerator_t *get_accel (unsigned chain_index, ثابت Chain &chain, unsigned ژمارە_glyphs) ثابت
    {
      إذا (unlikely (chain_index >= chain_ژماردن)) أرجع nullptr;

    retry:
      auto *accel = accels[chain_index].get_acquire ();
      إذا (unlikely (!accel))
      {
	accel = hb_aat_layout_chain_accelerator_t::create (chain, ژمارە_glyphs);
	إذا (unlikely (!accel))
	  أرجع nullptr;

	إذا (unlikely (!accels[chain_index].cmpexch (nullptr, accel)))
	{
	  hb_free (accel);
	  اذهب retry;
	}
      }

      أرجع accel;
    }

    hb_blob_ptr_t<T> table;
    unsigned صحيح chain_ژماردن;
    hb_atomic_t<hb_aat_layout_chain_accelerator_t *> *accels;
    hb_aat_scratch_t scratch;
  };


  فراغ compile_flags (ثابت hb_aat_map_builder_t *mapper,
		      hb_aat_map_t *map) ثابت
  {
    ثابت Chain<Types> *chain = &firstChain;
    unsigned صحيح ژماردن = chain_ژماردن_2;
    إذا (unlikely (!map->chain_flags.resize (ژماردن)))
      أرجع;
    لكل (unsigned صحيح i = 0; i < ژماردن; i++)
    {
      map->chain_flags[i].push (hb_aat_map_t::range_flags_t {chain->compile_flags (mapper),
							     mapper->range_first,
							     mapper->range_last});
      chain = &StructAfter<Chain<Types>> (*chain);
    }
  }

  unsigned get_chain_ژماردن () ثابت
  {
    أرجع chain_ژماردن_2;
  }
  فراغ apply (hb_aat_apply_context_t *c,
	      ثابت hb_aat_map_t &map,
	      ثابت accelerator_t &accel) ثابت
  {
    إذا (unlikely (!c->buffer->successful)) أرجع;

    c->buffer->unsafe_to_concat ();

    c->setup_buffer_glyph_set ();

    c->set_lookup_index (0);
    ثابت Chain<Types> *chain = &firstChain;
    unsigned صحيح ژماردن = chain_ژماردن_2;
    لكل (unsigned صحيح i = 0; i < ژماردن; i++)
    {
      auto *chain_accel = accel.get_accel (i, *chain, c->face->get_ژمارە_glyphs ());
      c->range_flags = &map.chain_flags[i];
      chain->apply (c, chain_accel);
      إذا (unlikely (!c->buffer->successful)) أرجع;
      chain = &StructAfter<Chain<Types>> (*chain);
    }
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (!(version.sanitize (c) &&
	  hb_barrier () &&
	  version &&
	  chain_ژماردن_2.sanitize (c)))
      return_trace (خطأ);

    ثابت Chain<Types> *chain = &firstChain;
    unsigned صحيح ژماردن = chain_ژماردن_2;
    لكل (unsigned صحيح i = 0; i < ژماردن; i++)
    {
      إذا (!chain->sanitize (c, version))
	return_trace (خطأ);
      hb_barrier ();
      chain = &StructAfter<Chain<Types>> (*chain);
    }

    return_trace (true);
  }

  محمي:
  HBUINT16	version;	/* Version number of the glyph metamorphosis table.
				 * 1, 2, or 3. */
  HBUINT16	unused;		/* Set to 0. */
  HBUINT32	chain_ژماردن_2;	/* Number of metamorphosis chains contained in this
				 * table. */
  Chain<Types>	firstChain;	/* Chains. */

  عام:
  DEFINE_SIZE_MIN (8);
};

بنية morx : mortmorx<morx, ExtendedTypes, HB_AAT_TAG_morx>
{
  HB_INTERNAL منطقي is_blocklisted (hb_blob_t *blob,
                                   hb_face_t *face) ثابت;
};

بنية mort : mortmorx<mort, ObsoleteTypes, HB_AAT_TAG_mort>
{
  HB_INTERNAL منطقي is_blocklisted (hb_blob_t *blob,
                                   hb_face_t *face) ثابت;
};

بنية morx_accelerator_t : morx::accelerator_t {
  morx_accelerator_t (hb_face_t *face) : morx::accelerator_t (face) {}
};
بنية mort_accelerator_t : mort::accelerator_t {
  mort_accelerator_t (hb_face_t *face) : mort::accelerator_t (face) {}
};


} /* namespace AAT */


#endif /* HB_AAT_LAYOUT_MORX_TABLE_HH */
