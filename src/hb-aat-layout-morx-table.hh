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

#شامل "hb-open-type.hh"
#شامل "hb-aat-layout-common.hh"
#شامل "hb-ot-layout.hh"
#شامل "hb-aat-map.hh"

/*
 * morx -- Extended Glyph Metamorphosis
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6morx.html
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6mort.html
 */
#define HB_AAT_TAG_morx HB_TAG('m','o','r','x')
#define HB_AAT_TAG_mort HB_TAG('m','o','r','t')


فضاینام AAT {

using فضاینام OT;

template <typename Types>
ساختار RearrangementSubtable
{
  تعریفنوع typename Types::HBUINT HBUINT;

  تعریفنوع تهی EntryData;

  شمارش Flags
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

  منطقی is_action_initiable (ثابت Entry<EntryData> &entry) ثابت
  {
    بازگشت (entry.flags & MarkFirst);
  }
  منطقی is_actionable (ثابت Entry<EntryData> &entry) ثابت
  {
    بازگشت (entry.flags & Verb);
  }

  ساختار driver_context_t
  {
    ایستا constexpr منطقی in_place = درست;

    driver_context_t (ثابت RearrangementSubtable *table_) :
	ret (نادرست),
	table (table_),
	start (0), end (0) {}

    HB_AAT_TRANSITION_INLINE
    تهی transition (hb_buffer_t *buffer,
		     StateTableDriver<Types, EntryData, Flags> *driver,
		     ثابت Entry<EntryData> &entry)
    {
      unsigned صحیح flags = entry.flags;

      اگر (flags & MarkFirst)
	start = buffer->idx;

      اگر (flags & MarkLast)
	end = hb_min (buffer->idx + 1, buffer->len);

      اگر ((flags & Verb) && start < end)
      {
	/* The following map has two nibbles, for start-side
	 * and end-side. Values of 0,1,2 mean move that many
	 * to the other side. Value of 3 means move 2 and
	 * flip them. */
	ثابت unsigned نویسه map[16] =
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

	unsigned صحیح m = map[flags & Verb];
	unsigned صحیح l = hb_min (2u, m >> 4);
	unsigned صحیح r = hb_min (2u, m & 0x0F);
	منطقی reverse_l = 3 == (m >> 4);
	منطقی reverse_r = 3 == (m & 0x0F);

	اگر (end - start >= l + r && end-start <= HB_MAX_CONTEXT_LENGTH)
	{
	  buffer->merge_clusters (start, hb_min (buffer->idx + 1, buffer->len));
	  buffer->merge_clusters (start, end);

	  hb_glyph_info_t *info = buffer->info;
	  hb_glyph_info_t buf[4];

	  hb_memcpy (buf, info + start, l * اندازه (buf[0]));
	  hb_memcpy (buf + 2, info + end - r, r * اندازه (buf[0]));

	  اگر (l != r)
	    memmove (info + start + r, info + start + l, (end - start - l - r) * اندازه (buf[0]));

	  hb_memcpy (info + start, buf + 2, r * اندازه (buf[0]));
	  hb_memcpy (info + end - l, buf, l * اندازه (buf[0]));
	  اگر (reverse_l)
	  {
	    buf[0] = info[end - 1];
	    info[end - 1] = info[end - 2];
	    info[end - 2] = buf[0];
	  }
	  اگر (reverse_r)
	  {
	    buf[0] = info[start];
	    info[start] = info[start + 1];
	    info[start + 1] = buf[0];
	  }
	}
      }
    }

    عمومی:
    منطقی ret;
    ثابت RearrangementSubtable *table;
    خصوصی:
    unsigned صحیح start;
    unsigned صحیح end;
  };

  منطقی apply (hb_aat_apply_context_t *c) ثابت
  {
    TRACE_APPLY (این);

    driver_context_t dc (این);

    StateTableDriver<Types, EntryData, Flags> driver (machine, c->face);

    driver.drive (&dc, c);

    return_trace (dc.ret);
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (machine.sanitize (c));
  }

  عمومی:
  StateTable<Types, EntryData>	machine;
  عمومی:
  DEFINE_SIZE_STATIC ((StateTable<Types, EntryData>::static_size));
};

template <typename Types>
ساختار ContextualSubtable
{
  تعریفنوع typename Types::HBUINT HBUINT;

  ساختار EntryData
  {
    تعریفنوع typename std::conditional<Types::extended, HBUINT16, HBINT16>::type OffsetType;

    OffsetType	markIndex;	/* Index/offset of the substitution table for the
				 * marked glyph. */
    OffsetType	currentIndex;	/* Index/offset of the substitution table for the
				 * current glyph. */
    عمومی:
    DEFINE_SIZE_STATIC (4);
  };

  شمارش Flags
  {
    SetMark		= 0x8000,	/* If set, make the current glyph the marked glyph. */
    DontAdvance		= 0x4000,	/* If set, don't advance to the next glyph before
					 * going to the new state. */
    Reserved		= 0x3FFF,	/* These bits are reserved and should be set to 0. */
  };

  منطقی is_action_initiable (ثابت Entry<EntryData> &entry) ثابت
  {
    بازگشت (entry.flags & SetMark);
  }
  منطقی is_actionable (ثابت Entry<EntryData> &entry) ثابت
  {
    اگر (Types::extended)
      بازگشت entry.data.markIndex != 0xFFFF || entry.data.currentIndex != 0xFFFF;
    وگرنه
      بازگشت entry.data.markIndex || entry.data.currentIndex;
  }

  ساختار driver_context_t
  {
    ایستا constexpr منطقی in_place = درست;

    driver_context_t (ثابت ContextualSubtable *table_,
			     hb_aat_apply_context_t *c_) :
	ret (نادرست),
	c (c_),
	table (table_),
	mark_set (نادرست),
	mark (0),
	subs (table+table->substitutionTables) {}

    HB_AAT_TRANSITION_INLINE
    تهی transition (hb_buffer_t *buffer,
		     StateTableDriver<Types, EntryData, Flags> *driver,
		     ثابت Entry<EntryData> &entry)
    {
      /* Looks like CoreText applies neither mark nor current substitution for
       * end-of-text if mark was not explicitly set. */
      اگر (buffer->idx == buffer->len && !mark_set)
	بازگشت;

      ثابت HBGlyphID16 *replacement;

      replacement = nullptr;
      اگر (Types::extended)
      {
	اگر (entry.data.markIndex != 0xFFFF)
	{
	  ثابت Lookup<HBGlyphID16> &lookup = subs[entry.data.markIndex];
	  replacement = lookup.get_مقدار (buffer->info[mark].codepoint, driver->رقم_glyphs);
	}
      }
      وگرنه
      {
	اگر (entry.data.markIndex)
	{
	  صحیح offset = (صحیح) entry.data.markIndex + buffer->info[mark].codepoint;
	  ثابت UnsizedArrayOf<HBGlyphID16> &subs_old = (ثابت UnsizedArrayOf<HBGlyphID16> &) subs;
	  replacement = &subs_old[Types::wordOffsetToIndex (offset, table, subs_old.arrayZ)];
	  اگر (!(replacement->sanitize (&c->sanitizer) &&
		hb_barrier () &&
		*replacement))
	    replacement = nullptr;
	}
      }
      اگر (replacement)
      {
	buffer->unsafe_to_break (mark, hb_min (buffer->idx + 1, buffer->len));
	c->replace_glyph_inplace (mark, *replacement);
	ret = درست;
      }

      replacement = nullptr;
      unsigned صحیح idx = hb_min (buffer->idx, buffer->len - 1);
      اگر (Types::extended)
      {
	اگر (entry.data.currentIndex != 0xFFFF)
	{
	  ثابت Lookup<HBGlyphID16> &lookup = subs[entry.data.currentIndex];
	  replacement = lookup.get_مقدار (buffer->info[idx].codepoint, driver->رقم_glyphs);
	}
      }
      وگرنه
      {
	اگر (entry.data.currentIndex)
	{
	  صحیح offset = (صحیح) entry.data.currentIndex + buffer->info[idx].codepoint;
	  ثابت UnsizedArrayOf<HBGlyphID16> &subs_old = (ثابت UnsizedArrayOf<HBGlyphID16> &) subs;
	  replacement = &subs_old[Types::wordOffsetToIndex (offset, table, subs_old.arrayZ)];
	  اگر (!(replacement->sanitize (&c->sanitizer) &&
		hb_barrier () &&
		*replacement))
	    replacement = nullptr;
	}
      }
      اگر (replacement)
      {
	c->replace_glyph_inplace (idx, *replacement);
	ret = درست;
      }

      اگر (entry.flags & SetMark)
      {
	mark_set = درست;
	mark = buffer->idx;
      }
    }

    عمومی:
    منطقی ret;
    hb_aat_apply_context_t *c;
    ثابت ContextualSubtable *table;
    خصوصی:
    منطقی mark_set;
    unsigned صحیح mark;
    ثابت UnsizedListOfOffset16To<Lookup<HBGlyphID16>, HBUINT, تهی, نادرست> &subs;
  };

  منطقی apply (hb_aat_apply_context_t *c) ثابت
  {
    TRACE_APPLY (این);

    driver_context_t dc (این, c);

    StateTableDriver<Types, EntryData, Flags> driver (machine, c->face);

    driver.drive (&dc, c);

    return_trace (dc.ret);
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);

    unsigned صحیح رقم_entries = 0;
    اگر (unlikely (!machine.sanitize (c, &رقم_entries))) return_trace (نادرست);
    hb_barrier ();

    اگر (!Types::extended)
      return_trace (substitutionTables.sanitize (c, این, 0));

    unsigned صحیح رقم_lookups = 0;

    ثابت Entry<EntryData> *entries = machine.get_entries ();
    برای (unsigned صحیح i = 0; i < رقم_entries; i++)
    {
      ثابت EntryData &data = entries[i].data;

      اگر (data.markIndex != 0xFFFF)
	رقم_lookups = hb_max (رقم_lookups, 1u + data.markIndex);
      اگر (data.currentIndex != 0xFFFF)
	رقم_lookups = hb_max (رقم_lookups, 1u + data.currentIndex);
    }

    return_trace (substitutionTables.sanitize (c, این, رقم_lookups));
  }

  عمومی:
  StateTable<Types, EntryData>
		machine;
  محافظتشده:
  NNOffsetTo<UnsizedListOfOffset16To<Lookup<HBGlyphID16>, HBUINT, تهی, نادرست>, HBUINT>
		substitutionTables;
  عمومی:
  DEFINE_SIZE_STATIC ((StateTable<Types, EntryData>::static_size + HBUINT::static_size));
};


template <منطقی extended>
ساختار LigatureEntry;

template <>
ساختار LigatureEntry<درست>
{

  ساختار EntryData
  {
    HBUINT16	ligActionIndex;	/* Index to the first ligActionTable entry
				 * for processing this group, if indicated
				 * by the flags. */
    عمومی:
    DEFINE_SIZE_STATIC (2);
  };

  شمارش Flags
  {
    SetComponent	= 0x8000,	/* Push this glyph onto the component stack for
					 * eventual processing. */
    DontAdvance		= 0x4000,	/* Leave the glyph pointer at this glyph for the
					   next iteration. */
    PerformAction	= 0x2000,	/* Use the ligActionIndex to process a ligature
					 * group. */
    Reserved		= 0x1FFF,	/* These bits are reserved and should be set to 0. */
  };

  ایستا منطقی initiateAction (ثابت Entry<EntryData> &entry)
  { بازگشت entry.flags & SetComponent; }

  ایستا منطقی performAction (ثابت Entry<EntryData> &entry)
  { بازگشت entry.flags & PerformAction; }

  ایستا unsigned صحیح ligActionIndex (ثابت Entry<EntryData> &entry)
  { بازگشت entry.data.ligActionIndex; }
};
template <>
ساختار LigatureEntry<نادرست>
{
  تعریفنوع تهی EntryData;

  شمارش Flags
  {
    SetComponent	= 0x8000,	/* Push this glyph onto the component stack for
					 * eventual processing. */
    DontAdvance		= 0x4000,	/* Leave the glyph pointer at this glyph for the
					   next iteration. */
    Offset		= 0x3FFF,	/* Byte offset from beginning of subtable to the
					 * ligature action list. This value must be a
					 * multiple of 4. */
  };

  ایستا منطقی initiateAction (ثابت Entry<EntryData> &entry)
  { بازگشت entry.flags & SetComponent; }

  ایستا منطقی performAction (ثابت Entry<EntryData> &entry)
  { بازگشت entry.flags & Offset; }

  ایستا unsigned صحیح ligActionIndex (ثابت Entry<EntryData> &entry)
  { بازگشت entry.flags & Offset; }
};


template <typename Types>
ساختار LigatureSubtable
{
  تعریفنوع typename Types::HBUINT HBUINT;

  تعریفنوع LigatureEntry<Types::extended> LigatureEntryT;
  تعریفنوع typename LigatureEntryT::EntryData EntryData;

  شمارش Flags
  {
    DontAdvance	= LigatureEntryT::DontAdvance,
  };

  منطقی is_action_initiable (ثابت Entry<EntryData> &entry) ثابت
  {
    بازگشت LigatureEntryT::initiateAction (entry);
  }
  منطقی is_actionable (ثابت Entry<EntryData> &entry) ثابت
  {
    بازگشت LigatureEntryT::performAction (entry);
  }

  ساختار driver_context_t
  {
    ایستا constexpr منطقی in_place = نادرست;
    شمارش LigActionFlags
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
	ret (نادرست),
	c (c_),
	table (table_),
	ligAction (table+table->ligAction),
	component (table+table->component),
	ligature (table+table->ligature),
	match_length (0) {}

    HB_AAT_TRANSITION_INLINE
    تهی transition (hb_buffer_t *buffer,
		     StateTableDriver<Types, EntryData, Flags> *driver,
		     ثابت Entry<EntryData> &entry)
    {
      DEBUG_پیام (APPLY, nullptr, "Ligature transition at %u", buffer->idx);
      اگر (entry.flags & LigatureEntryT::SetComponent)
      {
	/* Never mark same index twice, in case DontAdvance was used... */
	اگر (unlikely (match_length && match_positions[(match_length - 1u) % ARRAY_LENGTH (match_positions)] == buffer->out_len))
	  match_length--;

	match_positions[match_length++ % ARRAY_LENGTH (match_positions)] = buffer->out_len;
	DEBUG_پیام (APPLY, nullptr, "Set component at %u", buffer->out_len);
      }

      اگر (LigatureEntryT::performAction (entry))
      {
	DEBUG_پیام (APPLY, nullptr, "Perform action with %u", match_length);
	unsigned صحیح end = buffer->out_len;

	اگر (unlikely (!match_length))
	  بازگشت;

	اگر (buffer->idx >= buffer->len)
	  بازگشت; /* TODO Work on previous instead? */

	unsigned صحیح cursor = match_length;

	unsigned صحیح action_idx = LigatureEntryT::ligActionIndex (entry);
	action_idx = Types::offsetToIndex (action_idx, table, ligAction.arrayZ);
	ثابت HBUINT32 *actionData = &ligAction[action_idx];

	unsigned صحیح ligature_idx = 0;
	unsigned صحیح action;
	انجامبده
	{
	  اگر (unlikely (!cursor))
	  {
	    /* Stack underflow.  Clear the stack. */
	    DEBUG_پیام (APPLY, nullptr, "Stack underflow");
	    match_length = 0;
	    بشکن;
	  }

	  DEBUG_پیام (APPLY, nullptr, "Moving to stack position %u", cursor - 1);
	  اگر (unlikely (!buffer->move_to (match_positions[--cursor % ARRAY_LENGTH (match_positions)]))) بازگشت;

	  اگر (unlikely (!actionData->sanitize (&c->sanitizer))) بشکن;
	  hb_barrier ();
	  action = *actionData;

	  uint32_t uoffset = action & LigActionOffset;
	  اگر (uoffset & 0x20000000)
	    uoffset |= 0xC0000000; /* Sign-extend. */
	  int32_t offset = (int32_t) uoffset;
	  unsigned صحیح component_idx = buffer->cur().codepoint + offset;
	  component_idx = Types::wordOffsetToIndex (component_idx, table, component.arrayZ);
	  ثابت HBUINT16 &componentData = component[component_idx];
	  اگر (unlikely (!componentData.sanitize (&c->sanitizer))) بشکن;
	  hb_barrier ();
	  ligature_idx += componentData;

	  DEBUG_پیام (APPLY, nullptr, "Action store %d last %d",
		     منطقی (action & LigActionStore),
		     منطقی (action & LigActionLast));
	  اگر (action & (LigActionStore | LigActionLast))
	  {
	    ligature_idx = Types::offsetToIndex (ligature_idx, table, ligature.arrayZ);
	    ثابت HBGlyphID16 &ligatureData = ligature[ligature_idx];
	    اگر (unlikely (!ligatureData.sanitize (&c->sanitizer))) بشکن;
	    hb_barrier ();
	    hb_codepoint_t lig = ligatureData;

	    DEBUG_پیام (APPLY, nullptr, "Produced ligature %u", lig);
	    اگر (unlikely (!c->replace_glyph (lig))) بازگشت;

	    unsigned صحیح lig_end = match_positions[(match_length - 1u) % ARRAY_LENGTH (match_positions)] + 1u;
	    /* Now go and delete all subsequent components. */
	    وقتی (match_length - 1u > cursor)
	    {
	      DEBUG_پیام (APPLY, nullptr, "Skipping ligature component");
	      اگر (unlikely (!buffer->move_to (match_positions[--match_length % ARRAY_LENGTH (match_positions)]))) بازگشت;
	      اگر (!c->delete_glyph ()) بازگشت;
	    }

	    اگر (unlikely (!buffer->move_to (lig_end))) بازگشت;
	    buffer->merge_out_clusters (match_positions[cursor % ARRAY_LENGTH (match_positions)], buffer->out_len);
	  }

	  actionData++;
	}
	وقتی (!(action & LigActionLast));
	اگر (unlikely (!buffer->move_to (end))) بازگشت;
      }
    }

    عمومی:
    منطقی ret;
    hb_aat_apply_context_t *c;
    ثابت LigatureSubtable *table;
    خصوصی:
    ثابت UnsizedArrayOf<HBUINT32> &ligAction;
    ثابت UnsizedArrayOf<HBUINT16> &component;
    ثابت UnsizedArrayOf<HBGlyphID16> &ligature;
    unsigned صحیح match_length;
    unsigned صحیح match_positions[HB_MAX_CONTEXT_LENGTH];
  };

  منطقی apply (hb_aat_apply_context_t *c) ثابت
  {
    TRACE_APPLY (این);

    driver_context_t dc (این, c);

    StateTableDriver<Types, EntryData, Flags> driver (machine, c->face);

    driver.drive (&dc, c);

    return_trace (dc.ret);
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    /* The rest of array sanitizations are done at run-time. */
    return_trace (c->check_struct (این) && machine.sanitize (c) &&
		  hb_barrier () &&
		  ligAction && component && ligature);
  }

  عمومی:
  StateTable<Types, EntryData>
		machine;
  محافظتشده:
  NNOffsetTo<UnsizedArrayOf<HBUINT32>, HBUINT>
		ligAction;	/* Offset to the ligature action table. */
  NNOffsetTo<UnsizedArrayOf<HBUINT16>, HBUINT>
		component;	/* Offset to the component table. */
  NNOffsetTo<UnsizedArrayOf<HBGlyphID16>, HBUINT>
		ligature;	/* Offset to the actual ligature lists. */
  عمومی:
  DEFINE_SIZE_STATIC ((StateTable<Types, EntryData>::static_size + 3 * HBUINT::static_size));
};

template <typename Types>
ساختار NoncontextualSubtable
{
  منطقی apply (hb_aat_apply_context_t *c) ثابت
  {
    TRACE_APPLY (این);

    منطقی ret = نادرست;
    unsigned صحیح رقم_glyphs = c->face->get_رقم_glyphs ();

    hb_glyph_info_t *info = c->buffer->info;
    unsigned صحیح شمار = c->buffer->len;
    // If there's only one range, we already checked the flag.
    auto *last_range = c->range_flags && (c->range_flags->length > 1) ? &(*c->range_flags)[0] : nullptr;
    برای (unsigned صحیح i = 0; i < شمار; i++)
    {
      /* This block copied from StateTableDriver::drive. Keep in sync. */
      اگر (unlikely (last_range))
      {
	auto *range = last_range;
	{
	  unsigned cluster = info[i].cluster;
	  وقتی (cluster < range->cluster_first)
	    range--;
	  وقتی (cluster > range->cluster_last)
	    range++;

	  last_range = range;
	}
	اگر (!(range->flags & c->subtable_flags))
	  ادامه;
      }

      ثابت HBGlyphID16 *replacement = substitute.get_مقدار (info[i].codepoint, رقم_glyphs);
      اگر (replacement)
      {
	c->replace_glyph_inplace (i, *replacement);
	ret = درست;
      }
    }

    return_trace (ret);
  }

  template <typename set_t>
  تهی collect_initial_glyphs (set_t &glyphs, unsigned رقم_glyphs) ثابت
  {
    substitute.collect_glyphs (glyphs, رقم_glyphs);
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (substitute.sanitize (c));
  }

  محافظتشده:
  Lookup<HBGlyphID16>	substitute;
  عمومی:
  DEFINE_SIZE_MIN (2);
};

template <typename Types>
ساختار InsertionSubtable
{
  تعریفنوع typename Types::HBUINT HBUINT;

  ساختار EntryData
  {
    HBUINT16	current_درج_Index;	/* Zero-based index into the insertion glyph table.
					 * The number of glyphs to be inserted is contained
					 * in the currentInsertCount field in the flags.
					 * A value of 0xFFFF indicates no insertion is to
					 * be done. */
    HBUINT16	marked_درج_Index;	/* Zero-based index into the insertion glyph table.
					 * The number of glyphs to be inserted is contained
					 * in the markedInsertCount field in the flags.
					 * A value of 0xFFFF indicates no insertion is to
					 * be done. */
    عمومی:
    DEFINE_SIZE_STATIC (4);
  };

  شمارش Flags
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
    Current_درج_Before= 0x0800,      /* If set, specifies that insertions are to be made
				       * to the left of the current glyph. If clear,
				       * they're made to the right of the current glyph. */
    Marked_درج_Before= 0x0400,	      /* If set, specifies that insertions are to be
				       * made to the left of the marked glyph. If clear,
				       * they're made to the right of the marked glyph. */
    Current_درج_شمار= 0x3E0,	      /* This 5-bit field is treated as a count of the
				       * number of glyphs to insert at the current
				       * position. Since zero means no insertions, the
				       * largest number of insertions at any given
				       * current location is 31 glyphs. */
    Marked_درج_شمار= 0x001F,	      /* This 5-bit field is treated as a count of the
				       * number of glyphs to insert at the marked
				       * position. Since zero means no insertions, the
				       * largest number of insertions at any given
				       * marked location is 31 glyphs. */
  };

  منطقی is_action_initiable (ثابت Entry<EntryData> &entry) ثابت
  {
    بازگشت (entry.flags & SetMark);
  }
  منطقی is_actionable (ثابت Entry<EntryData> &entry) ثابت
  {
    بازگشت (entry.flags & (Current_درج_شمار | Marked_درج_شمار)) &&
	   (entry.data.current_درج_Index != 0xFFFF ||entry.data.marked_درج_Index != 0xFFFF);
  }

  ساختار driver_context_t
  {
    ایستا constexpr منطقی in_place = نادرست;

    driver_context_t (ثابت InsertionSubtable *table_,
		      hb_aat_apply_context_t *c_) :
	ret (نادرست),
	c (c_),
	table (table_),
	mark (0),
	insertionAction (table+table->insertionAction) {}

    HB_AAT_TRANSITION_INLINE
    تهی transition (hb_buffer_t *buffer,
		     StateTableDriver<Types, EntryData, Flags> *driver,
		     ثابت Entry<EntryData> &entry)
    {
      unsigned صحیح flags = entry.flags;

      unsigned mark_loc = buffer->out_len;

      اگر (entry.data.marked_درج_Index != 0xFFFF)
      {
	unsigned صحیح شمار = (flags & Marked_درج_شمار);
	اگر (unlikely ((buffer->max_ops -= شمار) <= 0)) بازگشت;
	unsigned صحیح start = entry.data.marked_درج_Index;
	ثابت HBGlyphID16 *glyphs = &insertionAction[start];
	اگر (unlikely (!c->sanitizer.check_array (glyphs, شمار))) شمار = 0;
	hb_barrier ();

	منطقی before = flags & Marked_درج_Before;

	unsigned صحیح end = buffer->out_len;
	اگر (unlikely (!buffer->move_to (mark))) بازگشت;

	اگر (buffer->idx < buffer->len && !before)
	  اگر (unlikely (!buffer->copy_glyph ())) بازگشت;
	/* TODO We ignore KashidaLike setting. */
	اگر (unlikely (!c->output_glyphs (شمار, glyphs))) بازگشت;
	ret = درست;
	اگر (buffer->idx < buffer->len && !before)
	  buffer->skip_glyph ();

	اگر (unlikely (!buffer->move_to (end + شمار))) بازگشت;

	buffer->unsafe_to_break_from_outbuffer (mark, hb_min (buffer->idx + 1, buffer->len));
      }

      اگر (flags & SetMark)
	mark = mark_loc;

      اگر (entry.data.current_درج_Index != 0xFFFF)
      {
	unsigned صحیح شمار = (flags & Current_درج_شمار) >> 5;
	اگر (unlikely ((buffer->max_ops -= شمار) <= 0)) بازگشت;
	unsigned صحیح start = entry.data.current_درج_Index;
	ثابت HBGlyphID16 *glyphs = &insertionAction[start];
	اگر (unlikely (!c->sanitizer.check_array (glyphs, شمار))) شمار = 0;
	hb_barrier ();

	منطقی before = flags & Current_درج_Before;

	unsigned صحیح end = buffer->out_len;

	اگر (buffer->idx < buffer->len && !before)
	  اگر (unlikely (!buffer->copy_glyph ())) بازگشت;
	/* TODO We ignore KashidaLike setting. */
	اگر (unlikely (!c->output_glyphs (شمار, glyphs))) بازگشت;
	ret = درست;
	اگر (buffer->idx < buffer->len && !before)
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
	اگر (unlikely (!buffer->move_to ((flags & DontAdvance) ? end : end + شمار))) بازگشت;
      }
    }

    عمومی:
    منطقی ret;
    hb_aat_apply_context_t *c;
    ثابت InsertionSubtable *table;
    خصوصی:
    unsigned صحیح mark;
    ثابت UnsizedArrayOf<HBGlyphID16> &insertionAction;
  };

  منطقی apply (hb_aat_apply_context_t *c) ثابت
  {
    TRACE_APPLY (این);

    driver_context_t dc (این, c);

    StateTableDriver<Types, EntryData, Flags> driver (machine, c->face);

    driver.drive (&dc, c);

    return_trace (dc.ret);
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    /* The rest of array sanitizations are done at run-time. */
    return_trace (c->check_struct (این) && machine.sanitize (c) &&
		  hb_barrier () &&
		  insertionAction);
  }

  عمومی:
  StateTable<Types, EntryData>
		machine;
  محافظتشده:
  NNOffsetTo<UnsizedArrayOf<HBGlyphID16>, HBUINT>
		insertionAction;	/* Byte offset from stateHeader to the start of
					 * the insertion glyph table. */
  عمومی:
  DEFINE_SIZE_STATIC ((StateTable<Types, EntryData>::static_size + HBUINT::static_size));
};


ساختار Feature
{
  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این));
  }

  عمومی:
  HBUINT16	featureType;	/* The type of feature. */
  HBUINT16	featureSetting;	/* The feature's setting (aka selector). */
  HBUINT32	enableFlags;	/* Flags for the settings that this feature
				 * and setting enables. */
  HBUINT32	disableFlags;	/* Complement of flags for the settings that this
				 * feature and setting disable. */

  عمومی:
  DEFINE_SIZE_STATIC (12);
};


ساختار hb_accelerate_subtables_context_t :
       hb_dispatch_context_t<hb_accelerate_subtables_context_t>
{
  ساختار hb_applicable_t
  {
    friend ساختار hb_accelerate_subtables_context_t;
    friend ساختار hb_aat_layout_lookup_accelerator_t;

    عمومی:
    hb_bit_set_t glyph_set;
    mutable hb_aat_class_cache_t class_cache;

    template <typename T>
    auto init_ (ثابت T &obj_, unsigned رقم_glyphs, hb_priority<1>) HB_AUTO_RETURN
    (
      obj_.machine.collect_initial_glyphs (glyph_set, رقم_glyphs, obj_)
    )

    template <typename T>
    تهی init_ (ثابت T &obj_, unsigned رقم_glyphs, hb_priority<0>)
    {
      obj_.collect_initial_glyphs (glyph_set, رقم_glyphs);
    }

    template <typename T>
    تهی init (ثابت T &obj_, unsigned رقم_glyphs)
    {
      glyph_set.init ();
      init_ (obj_, رقم_glyphs, hb_prioritize);
      class_cache.clear ();
    }

    تهی
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

    entry->init (obj, رقم_glyphs);

    بازگشت hb_empty_t ();
  }
  ایستا return_t default_return_مقدار () { بازگشت hb_empty_t (); }

  منطقی stop_sublookup_iteration (return_t r) ثابت { بازگشت نادرست; }

  hb_accelerate_subtables_context_t (hb_applicable_t *array_, unsigned رقم_glyphs_2) :
				     hb_dispatch_context_t<hb_accelerate_subtables_context_t> (),
				     array (array_), رقم_glyphs (رقم_glyphs_2) {}

  hb_applicable_t *array;
  unsigned رقم_glyphs;
  unsigned i = 0;
};

ساختار hb_aat_layout_chain_accelerator_t
{
  template <typename TChain>
  ایستا hb_aat_layout_chain_accelerator_t *create (ثابت TChain &chain, unsigned رقم_glyphs)
  {
    unsigned شمار = chain.get_subtable_شمار ();

    unsigned product;
    اگر (unlikely (hb_unsigned_mul_overflows (شمار,
        اندازه (hb_accelerate_subtables_context_t::hb_applicable_t), &product)))
      بازگشت nullptr;

    unsigned size;
    اگر (unlikely (hb_unsigned_add_overflows (اندازه (hb_aat_layout_chain_accelerator_t) -
        HB_VAR_ARRAY * اندازه (hb_accelerate_subtables_context_t::hb_applicable_t), product, &size))) {
      بازگشت nullptr;
    }

    /* The following is a calloc because when we are collecting subtables,
     * some of them might be invalid and hence not collect; as a result,
     * we might not fill in all the count entries of the subtables array.
     * Zeroing it allows the set digest to gatekeep it without having to
     * initialize it further. */
    auto *thiz = (hb_aat_layout_chain_accelerator_t *) hb_calloc (1, size);
    اگر (unlikely (!thiz))
      بازگشت nullptr;

    thiz->شمار = شمار;

    hb_accelerate_subtables_context_t c_accelerate_subtables (thiz->subtables, رقم_glyphs);
    chain.dispatch (&c_accelerate_subtables);

    بازگشت thiz;
  }

  تهی destroy ()
  {
    برای (unsigned i = 0; i < شمار; i++)
      subtables[i].fini ();
  }

  unsigned شمار;
  hb_accelerate_subtables_context_t::hb_applicable_t subtables[HB_VAR_ARRAY];
};

template <typename Types>
ساختار ChainSubtable
{
  تعریفنوع typename Types::HBUINT HBUINT;

  template <typename T>
  friend ساختار Chain;

  size_t get_size () ثابت     { بازگشت length; }
  unsigned صحیح get_type () ثابت     { بازگشت coverage & 0xFF; }
  unsigned صحیح get_coverage () ثابت { بازگشت coverage >> (اندازه (HBUINT) * 8 - 8); }

  شمارش Coverage
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
  شمارش Type
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
    unsigned صحیح subtable_type = get_type ();
    TRACE_DISPATCH (این, subtable_type);
    انتخاب (subtable_type) {
    حالت Rearrangement:		hb_barrier (); return_trace (c->dispatch (u.rearrangement, std::forward<Ts> (ds)...));
    حالت Contextual:		hb_barrier (); return_trace (c->dispatch (u.contextual, std::forward<Ts> (ds)...));
    حالت Ligature:		hb_barrier (); return_trace (c->dispatch (u.ligature, std::forward<Ts> (ds)...));
    حالت Noncontextual:		hb_barrier (); return_trace (c->dispatch (u.noncontextual, std::forward<Ts> (ds)...));
    حالت Insertion:		hb_barrier (); return_trace (c->dispatch (u.insertion, std::forward<Ts> (ds)...));
    پیشفرض:			return_trace (c->default_return_مقدار ());
    }
  }

  منطقی apply (hb_aat_apply_context_t *c) ثابت
  {
    TRACE_APPLY (این);
    // Disabled for https://github.com/harfbuzz/harfbuzz/issues/4873
    //hb_sanitize_with_object_t with (&c->sanitizer, this);
    return_trace (dispatch (c));
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    اگر (!(length.sanitize (c) &&
	  hb_barrier () &&
	  length >= min_size &&
	  c->check_range (این, length)))
      return_trace (نادرست);

    // Disabled for https://github.com/harfbuzz/harfbuzz/issues/4873
    //hb_sanitize_with_object_t with (c, this);
    return_trace (dispatch (c));
  }

  محافظتشده:
  HBUINT	length;		/* Total subtable length, including this header. */
  HBUINT	coverage;	/* Coverage flags and subtable type. */
  HBUINT32	subFeatureFlags;/* The 32-bit mask identifying which subtable this is. */
  اجتماع {
  RearrangementSubtable<Types>	rearrangement;
  ContextualSubtable<Types>	contextual;
  LigatureSubtable<Types>	ligature;
  NoncontextualSubtable<Types>	noncontextual;
  InsertionSubtable<Types>	insertion;
  } u;
  عمومی:
  DEFINE_SIZE_MIN (2 * اندازه (HBUINT) + 4);
};

template <typename Types>
ساختار Chain
{
  تعریفنوع typename Types::HBUINT HBUINT;

  unsigned get_subtable_شمار () ثابت { بازگشت subtable_شمار_2; }

  hb_mask_t compile_flags (ثابت hb_aat_map_builder_t *map) ثابت
  {
    hb_mask_t flags = defaultFlags;
    {
      unsigned صحیح شمار = feature_شمار;
      برای (unsigned i = 0; i < شمار; i++)
      {
	ثابت Feature &feature = featureZ[i];
	hb_aat_layout_feature_type_t type = (hb_aat_layout_feature_type_t) (unsigned صحیح) feature.featureType;
	hb_aat_layout_feature_selector_t setting = (hb_aat_layout_feature_selector_t) (unsigned صحیح) feature.featureSetting;
      retry:
	// Check whether this type/setting pair was requested in the map, and if so, apply its flags.
	// (The search here only looks at the type and setting fields of feature_info_t.)
	hb_aat_map_builder_t::feature_info_t info = { type, setting, نادرست, 0 };
	اگر (map->current_features.bsearch (info))
	{
	  flags &= feature.disableFlags;
	  flags |= feature.enableFlags;
	}
	وگرنه اگر (type == HB_AAT_LAYOUT_FEATURE_TYPE_LETTER_CASE && setting == HB_AAT_LAYOUT_FEATURE_SELECTOR_SMALL_CAPS)
	{
	  /* Deprecated. https://github.com/harfbuzz/harfbuzz/issues/1342 */
	  type = HB_AAT_LAYOUT_FEATURE_TYPE_LOWER_CASE;
	  setting = HB_AAT_LAYOUT_FEATURE_SELECTOR_LOWER_CASE_SMALL_CAPS;
	  برو retry;
	}
#ifndef HB_NO_AAT
	وگرنه اگر (type == HB_AAT_LAYOUT_FEATURE_TYPE_LANGUAGE_TAG_TYPE && setting &&
		 /* TODO: Rudimentary language matching. */
		 hb_language_matches (map->face->table.ltag->get_language (setting - 1), map->props.language))
	{
	  flags &= feature.disableFlags;
	  flags |= feature.enableFlags;
	}
#endif
      }
    }
    بازگشت flags;
  }

  تهی apply (hb_aat_apply_context_t *c,
	      ثابت hb_aat_layout_chain_accelerator_t *accel) ثابت
  {
    ثابت ChainSubtable<Types> *subtable = &StructAfter<ChainSubtable<Types>> (featureZ.as_array (feature_شمار));
    unsigned صحیح شمار = subtable_شمار_2;
    برای (unsigned صحیح i = 0; i < شمار; i++)
    {
      منطقی reverse;

      auto coverage = subtable->get_coverage ();

      hb_mask_t subtable_flags = subtable->subFeatureFlags;
      اگر (hb_none (hb_iter (c->range_flags) |
		   hb_map ([subtable_flags] (ثابت hb_aat_map_t::range_flags_t _) -> منطقی { بازگشت subtable_flags & (_.flags); })))
	برو skip;

      اگر (!(coverage & ChainSubtable<Types>::AllDirections) &&
	  HB_DIRECTION_IS_VERTICAL (c->buffer->props.direction) !=
	  منطقی (coverage & ChainSubtable<Types>::Vertical))
	برو skip;

      c->subtable_flags = subtable_flags;
      c->first_set = accel ? &accel->subtables[i].glyph_set : &Null(hb_bit_set_t);
      c->machine_class_cache = accel ? &accel->subtables[i].class_cache : nullptr;

      اگر (!c->buffer_intersects_machine ())
      {
	(تهی) c->buffer->message (c->font, "skipped chainsubtable %u because no glyph matches", c->lookup_index);
	برو skip;
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
		منطقی (coverage & ChainSubtable<Types>::Backwards) :
		منطقی (coverage & ChainSubtable<Types>::Backwards) !=
		HB_DIRECTION_IS_BACKWARD (c->buffer->props.direction);

      اگر (!c->buffer->message (c->font, "start chainsubtable %u", c->lookup_index))
	برو skip;

      اگر (reverse != c->buffer_is_reversed)
        c->reverse_buffer ();

      subtable->apply (c);

      (تهی) c->buffer->message (c->font, "end chainsubtable %u", c->lookup_index);

      اگر (unlikely (!c->buffer->successful)) بشکن;

    skip:
      subtable = &StructAfter<ChainSubtable<Types>> (*subtable);
      c->set_lookup_index (c->lookup_index + 1);
    }
    اگر (c->buffer_is_reversed)
      c->reverse_buffer ();
  }

  size_t get_size () ثابت { بازگشت length; }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    ثابت ChainSubtable<Types> *subtable = &StructAfter<ChainSubtable<Types>> (featureZ.as_array (feature_شمار));
    unsigned صحیح شمار = subtable_شمار_2;
    برای (unsigned صحیح i = 0; i < شمار; i++)
    {
      typename context_t::return_t ret = subtable->dispatch (c, std::forward<Ts> (ds)...);
      اگر (c->stop_sublookup_iteration (ret))
	بازگشت ret;
      subtable = &StructAfter<ChainSubtable<Types>> (*subtable);
    }
    بازگشت c->default_return_مقدار ();
  }

  منطقی sanitize (hb_sanitize_context_t *c, unsigned صحیح version) ثابت
  {
    TRACE_SANITIZE (این);
    اگر (!(length.sanitize (c) &&
	  hb_barrier () &&
	  length >= min_size &&
	  c->check_range (این, length)))
      return_trace (نادرست);

    اگر (!c->check_array (featureZ.arrayZ, feature_شمار))
      return_trace (نادرست);

    ثابت ChainSubtable<Types> *subtable = &StructAfter<ChainSubtable<Types>> (featureZ.as_array (feature_شمار));
    unsigned صحیح شمار = subtable_شمار_2;
    برای (unsigned صحیح i = 0; i < شمار; i++)
    {
      اگر (!subtable->sanitize (c))
	return_trace (نادرست);
      hb_barrier ();
      subtable = &StructAfter<ChainSubtable<Types>> (*subtable);
    }

    اگر (version >= 3)
    {
      ثابت SubtableGlyphCoverage *coverage = (ثابت SubtableGlyphCoverage *) subtable;
      اگر (!coverage->sanitize (c, شمار))
        return_trace (نادرست);
    }

    return_trace (درست);
  }

  محافظتشده:
  HBUINT32	defaultFlags;	/* The default specification for subtables. */
  HBUINT32	length;		/* Total byte count, including this header. */
  HBUINT	feature_شمار;	/* Number of feature subtable entries. */
  HBUINT	subtable_شمار_2;	/* The number of subtables in the chain. */

  UnsizedArrayOf<Feature>	featureZ;	/* Features. */
/*ChainSubtable	firstSubtable;*//* Subtables. */
/*SubtableGlyphCoverage coverages*//* Only if version >= 3. */

  عمومی:
  DEFINE_SIZE_MIN (8 + 2 * اندازه (HBUINT));
};


/*
 * The 'mort'/'morx' Table
 */

template <typename T, typename Types, hb_tag_t TAG>
ساختار mortmorx
{
  ایستا constexpr hb_tag_t tableTag = TAG;

  منطقی has_data () ثابت { بازگشت version != 0; }

  ساختار accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      hb_sanitize_context_t sc;
      این->table = sc.reference_table<T> (face);

      اگر (unlikely (این->table->is_blocklisted (این->table.get_blob (), face)))
      {
        hb_blob_destroy (این->table.get_blob ());
        این->table = hb_blob_get_empty ();
      }

      این->chain_شمار = table->get_chain_شمار ();

      این->accels = (hb_atomic_t<hb_aat_layout_chain_accelerator_t *> *) hb_calloc (این->chain_شمار, اندازه (*accels));
      اگر (unlikely (!این->accels))
      {
	این->chain_شمار = 0;
	این->table.destroy ();
	این->table = hb_blob_get_empty ();
      }
    }
    ~accelerator_t ()
    {
      برای (unsigned صحیح i = 0; i < این->chain_شمار; i++)
      {
	اگر (این->accels[i])
	  این->accels[i]->destroy ();
	hb_free (این->accels[i]);
      }
      hb_free (این->accels);
      این->table.destroy ();
    }

    hb_blob_t *get_blob () ثابت { بازگشت table.get_blob (); }

    template <typename Chain>
    hb_aat_layout_chain_accelerator_t *get_accel (unsigned chain_index, ثابت Chain &chain, unsigned رقم_glyphs) ثابت
    {
      اگر (unlikely (chain_index >= chain_شمار)) بازگشت nullptr;

    retry:
      auto *accel = accels[chain_index].get_acquire ();
      اگر (unlikely (!accel))
      {
	accel = hb_aat_layout_chain_accelerator_t::create (chain, رقم_glyphs);
	اگر (unlikely (!accel))
	  بازگشت nullptr;

	اگر (unlikely (!accels[chain_index].cmpexch (nullptr, accel)))
	{
	  hb_free (accel);
	  برو retry;
	}
      }

      بازگشت accel;
    }

    hb_blob_ptr_t<T> table;
    unsigned صحیح chain_شمار;
    hb_atomic_t<hb_aat_layout_chain_accelerator_t *> *accels;
    hb_aat_scratch_t scratch;
  };


  تهی compile_flags (ثابت hb_aat_map_builder_t *mapper,
		      hb_aat_map_t *map) ثابت
  {
    ثابت Chain<Types> *chain = &firstChain;
    unsigned صحیح شمار = chain_شمار_2;
    اگر (unlikely (!map->chain_flags.resize (شمار)))
      بازگشت;
    برای (unsigned صحیح i = 0; i < شمار; i++)
    {
      map->chain_flags[i].push (hb_aat_map_t::range_flags_t {chain->compile_flags (mapper),
							     mapper->range_first,
							     mapper->range_last});
      chain = &StructAfter<Chain<Types>> (*chain);
    }
  }

  unsigned get_chain_شمار () ثابت
  {
    بازگشت chain_شمار_2;
  }
  تهی apply (hb_aat_apply_context_t *c,
	      ثابت hb_aat_map_t &map,
	      ثابت accelerator_t &accel) ثابت
  {
    اگر (unlikely (!c->buffer->successful)) بازگشت;

    c->buffer->unsafe_to_concat ();

    c->setup_buffer_glyph_set ();

    c->set_lookup_index (0);
    ثابت Chain<Types> *chain = &firstChain;
    unsigned صحیح شمار = chain_شمار_2;
    برای (unsigned صحیح i = 0; i < شمار; i++)
    {
      auto *chain_accel = accel.get_accel (i, *chain, c->face->get_رقم_glyphs ());
      c->range_flags = &map.chain_flags[i];
      chain->apply (c, chain_accel);
      اگر (unlikely (!c->buffer->successful)) بازگشت;
      chain = &StructAfter<Chain<Types>> (*chain);
    }
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    اگر (!(version.sanitize (c) &&
	  hb_barrier () &&
	  version &&
	  chain_شمار_2.sanitize (c)))
      return_trace (نادرست);

    ثابت Chain<Types> *chain = &firstChain;
    unsigned صحیح شمار = chain_شمار_2;
    برای (unsigned صحیح i = 0; i < شمار; i++)
    {
      اگر (!chain->sanitize (c, version))
	return_trace (نادرست);
      hb_barrier ();
      chain = &StructAfter<Chain<Types>> (*chain);
    }

    return_trace (درست);
  }

  محافظتشده:
  HBUINT16	version;	/* Version number of the glyph metamorphosis table.
				 * 1, 2, or 3. */
  HBUINT16	unused;		/* Set to 0. */
  HBUINT32	chain_شمار_2;	/* Number of metamorphosis chains contained in this
				 * table. */
  Chain<Types>	firstChain;	/* Chains. */

  عمومی:
  DEFINE_SIZE_MIN (8);
};

ساختار morx : mortmorx<morx, ExtendedTypes, HB_AAT_TAG_morx>
{
  HB_INTERNAL منطقی is_blocklisted (hb_blob_t *blob,
                                   hb_face_t *face) ثابت;
};

ساختار mort : mortmorx<mort, ObsoleteTypes, HB_AAT_TAG_mort>
{
  HB_INTERNAL منطقی is_blocklisted (hb_blob_t *blob,
                                   hb_face_t *face) ثابت;
};

ساختار morx_accelerator_t : morx::accelerator_t {
  morx_accelerator_t (hb_face_t *face) : morx::accelerator_t (face) {}
};
ساختار mort_accelerator_t : mort::accelerator_t {
  mort_accelerator_t (hb_face_t *face) : mort::accelerator_t (face) {}
};


} /* namespace AAT */


#endif /* HB_AAT_LAYOUT_MORX_TABLE_HH */
