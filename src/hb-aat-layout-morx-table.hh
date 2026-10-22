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

#ଅନ୍ତର୍ଭାଵ "hb-open-type.hh"
#ଅନ୍ତର୍ଭାଵ "hb-aat-layout-common.hh"
#ଅନ୍ତର୍ଭାଵ "hb-ot-layout.hh"
#ଅନ୍ତର୍ଭାଵ "hb-aat-map.hh"

/*
 * morx -- Extended Glyph Metamorphosis
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6morx.html
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6mort.html
 */
#define HB_AAT_TAG_morx HB_TAG('m','o','r','x')
#define HB_AAT_TAG_mort HB_TAG('m','o','r','t')


ନାମାକାଶ AAT {

using ନାମାକାଶ OT;

template <typename Types>
ସଂରଚନା RearrangementSubtable
{
  ପ୍ରକାରବ୍ୟାଖ୍ୟା typename Types::HBUINT HBUINT;

  ପ୍ରକାରବ୍ୟାଖ୍ୟା ଶୂନ୍ୟ EntryData;

  ଗଣନା Flags
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

  ତାର୍କିକ is_action_initiable (ଅଚଳ Entry<EntryData> &entry) ଅଚଳ
  {
    ଫେରନ୍ତୁ (entry.flags & MarkFirst);
  }
  ତାର୍କିକ is_actionable (ଅଚଳ Entry<EntryData> &entry) ଅଚଳ
  {
    ଫେରନ୍ତୁ (entry.flags & Verb);
  }

  ସଂରଚନା driver_context_t
  {
    ସ୍ଥିର constexpr ତାର୍କିକ in_place = ସତ୍ୟ;

    driver_context_t (ଅଚଳ RearrangementSubtable *table_) :
	ret (ମିଥ୍ୟା),
	table (table_),
	start (0), end (0) {}

    HB_AAT_TRANSITION_INLINE
    ଶୂନ୍ୟ transition (hb_buffer_t *buffer,
		     StateTableDriver<Types, EntryData, Flags> *driver,
		     ଅଚଳ Entry<EntryData> &entry)
    {
      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags = entry.flags;

      ଯଦି (flags & MarkFirst)
	start = buffer->idx;

      ଯଦି (flags & MarkLast)
	end = hb_min (buffer->idx + 1, buffer->len);

      ଯଦି ((flags & Verb) && start < end)
      {
	/* The following map has two nibbles, for start-side
	 * and end-side. Values of 0,1,2 mean move that many
	 * to the other side. Value of 3 means move 2 and
	 * flip them. */
	ଅଚଳ unsigned ଅକ୍ଷର map[16] =
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

	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା m = map[flags & Verb];
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା l = hb_min (2u, m >> 4);
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା r = hb_min (2u, m & 0x0F);
	ତାର୍କିକ reverse_l = 3 == (m >> 4);
	ତାର୍କିକ reverse_r = 3 == (m & 0x0F);

	ଯଦି (end - start >= l + r && end-start <= HB_MAX_CONTEXT_LENGTH)
	{
	  buffer->merge_clusters (start, hb_min (buffer->idx + 1, buffer->len));
	  buffer->merge_clusters (start, end);

	  hb_glyph_info_t *info = buffer->info;
	  hb_glyph_info_t buf[4];

	  hb_memcpy (buf, info + start, l * ଆକାର (buf[0]));
	  hb_memcpy (buf + 2, info + end - r, r * ଆକାର (buf[0]));

	  ଯଦି (l != r)
	    memmove (info + start + r, info + start + l, (end - start - l - r) * ଆକାର (buf[0]));

	  hb_memcpy (info + start, buf + 2, r * ଆକାର (buf[0]));
	  hb_memcpy (info + end - l, buf, l * ଆକାର (buf[0]));
	  ଯଦି (reverse_l)
	  {
	    buf[0] = info[end - 1];
	    info[end - 1] = info[end - 2];
	    info[end - 2] = buf[0];
	  }
	  ଯଦି (reverse_r)
	  {
	    buf[0] = info[start];
	    info[start] = info[start + 1];
	    info[start + 1] = buf[0];
	  }
	}
      }
    }

    ସର୍ବସାଧାରଣ:
    ତାର୍କିକ ret;
    ଅଚଳ RearrangementSubtable *table;
    ବ୍ୟକ୍ତିଗତ:
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା start;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା end;
  };

  ତାର୍କିକ apply (hb_aat_apply_context_t *c) ଅଚଳ
  {
    TRACE_APPLY (ଏହି);

    driver_context_t dc (ଏହି);

    StateTableDriver<Types, EntryData, Flags> driver (machine, c->face);

    driver.drive (&dc, c);

    return_trace (dc.ret);
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (machine.sanitize (c));
  }

  ସର୍ବସାଧାରଣ:
  StateTable<Types, EntryData>	machine;
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC ((StateTable<Types, EntryData>::static_size));
};

template <typename Types>
ସଂରଚନା ContextualSubtable
{
  ପ୍ରକାରବ୍ୟାଖ୍ୟା typename Types::HBUINT HBUINT;

  ସଂରଚନା EntryData
  {
    ପ୍ରକାରବ୍ୟାଖ୍ୟା typename std::conditional<Types::extended, HBUINT16, HBINT16>::type OffsetType;

    OffsetType	markIndex;	/* Index/offset of the substitution table for the
				 * marked glyph. */
    OffsetType	currentIndex;	/* Index/offset of the substitution table for the
				 * current glyph. */
    ସର୍ବସାଧାରଣ:
    DEFINE_SIZE_STATIC (4);
  };

  ଗଣନା Flags
  {
    SetMark		= 0x8000,	/* If set, make the current glyph the marked glyph. */
    DontAdvance		= 0x4000,	/* If set, don't advance to the next glyph before
					 * going to the new state. */
    Reserved		= 0x3FFF,	/* These bits are reserved and should be set to 0. */
  };

  ତାର୍କିକ is_action_initiable (ଅଚଳ Entry<EntryData> &entry) ଅଚଳ
  {
    ଫେରନ୍ତୁ (entry.flags & SetMark);
  }
  ତାର୍କିକ is_actionable (ଅଚଳ Entry<EntryData> &entry) ଅଚଳ
  {
    ଯଦି (Types::extended)
      ଫେରନ୍ତୁ entry.data.markIndex != 0xFFFF || entry.data.currentIndex != 0xFFFF;
    ଅନ୍ୟଥା
      ଫେରନ୍ତୁ entry.data.markIndex || entry.data.currentIndex;
  }

  ସଂରଚନା driver_context_t
  {
    ସ୍ଥିର constexpr ତାର୍କିକ in_place = ସତ୍ୟ;

    driver_context_t (ଅଚଳ ContextualSubtable *table_,
			     hb_aat_apply_context_t *c_) :
	ret (ମିଥ୍ୟା),
	c (c_),
	table (table_),
	mark_set (ମିଥ୍ୟା),
	mark (0),
	subs (table+table->substitutionTables) {}

    HB_AAT_TRANSITION_INLINE
    ଶୂନ୍ୟ transition (hb_buffer_t *buffer,
		     StateTableDriver<Types, EntryData, Flags> *driver,
		     ଅଚଳ Entry<EntryData> &entry)
    {
      /* Looks like CoreText applies neither mark nor current substitution for
       * end-of-text if mark was not explicitly set. */
      ଯଦି (buffer->idx == buffer->len && !mark_set)
	ଫେରନ୍ତୁ;

      ଅଚଳ HBGlyphID16 *replacement;

      replacement = nullptr;
      ଯଦି (Types::extended)
      {
	ଯଦି (entry.data.markIndex != 0xFFFF)
	{
	  ଅଚଳ Lookup<HBGlyphID16> &lookup = subs[entry.data.markIndex];
	  replacement = lookup.get_ମୂଲ୍ୟ (buffer->info[mark].codepoint, driver->ଅଙ୍କ_glyphs);
	}
      }
      ଅନ୍ୟଥା
      {
	ଯଦି (entry.data.markIndex)
	{
	  ପୂର୍ଣ୍ଣସଂଖ୍ୟା offset = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) entry.data.markIndex + buffer->info[mark].codepoint;
	  ଅଚଳ UnsizedArrayOf<HBGlyphID16> &subs_old = (ଅଚଳ UnsizedArrayOf<HBGlyphID16> &) subs;
	  replacement = &subs_old[Types::wordOffsetToIndex (offset, table, subs_old.arrayZ)];
	  ଯଦି (!(replacement->sanitize (&c->sanitizer) &&
		hb_barrier () &&
		*replacement))
	    replacement = nullptr;
	}
      }
      ଯଦି (replacement)
      {
	buffer->unsafe_to_break (mark, hb_min (buffer->idx + 1, buffer->len));
	c->replace_glyph_inplace (mark, *replacement);
	ret = ସତ୍ୟ;
      }

      replacement = nullptr;
      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା idx = hb_min (buffer->idx, buffer->len - 1);
      ଯଦି (Types::extended)
      {
	ଯଦି (entry.data.currentIndex != 0xFFFF)
	{
	  ଅଚଳ Lookup<HBGlyphID16> &lookup = subs[entry.data.currentIndex];
	  replacement = lookup.get_ମୂଲ୍ୟ (buffer->info[idx].codepoint, driver->ଅଙ୍କ_glyphs);
	}
      }
      ଅନ୍ୟଥା
      {
	ଯଦି (entry.data.currentIndex)
	{
	  ପୂର୍ଣ୍ଣସଂଖ୍ୟା offset = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) entry.data.currentIndex + buffer->info[idx].codepoint;
	  ଅଚଳ UnsizedArrayOf<HBGlyphID16> &subs_old = (ଅଚଳ UnsizedArrayOf<HBGlyphID16> &) subs;
	  replacement = &subs_old[Types::wordOffsetToIndex (offset, table, subs_old.arrayZ)];
	  ଯଦି (!(replacement->sanitize (&c->sanitizer) &&
		hb_barrier () &&
		*replacement))
	    replacement = nullptr;
	}
      }
      ଯଦି (replacement)
      {
	c->replace_glyph_inplace (idx, *replacement);
	ret = ସତ୍ୟ;
      }

      ଯଦି (entry.flags & SetMark)
      {
	mark_set = ସତ୍ୟ;
	mark = buffer->idx;
      }
    }

    ସର୍ବସାଧାରଣ:
    ତାର୍କିକ ret;
    hb_aat_apply_context_t *c;
    ଅଚଳ ContextualSubtable *table;
    ବ୍ୟକ୍ତିଗତ:
    ତାର୍କିକ mark_set;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା mark;
    ଅଚଳ UnsizedListOfOffset16To<Lookup<HBGlyphID16>, HBUINT, ଶୂନ୍ୟ, ମିଥ୍ୟା> &subs;
  };

  ତାର୍କିକ apply (hb_aat_apply_context_t *c) ଅଚଳ
  {
    TRACE_APPLY (ଏହି);

    driver_context_t dc (ଏହି, c);

    StateTableDriver<Types, EntryData, Flags> driver (machine, c->face);

    driver.drive (&dc, c);

    return_trace (dc.ret);
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);

    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_entries = 0;
    ଯଦି (unlikely (!machine.sanitize (c, &ଅଙ୍କ_entries))) return_trace (ମିଥ୍ୟା);
    hb_barrier ();

    ଯଦି (!Types::extended)
      return_trace (substitutionTables.sanitize (c, ଏହି, 0));

    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_lookups = 0;

    ଅଚଳ Entry<EntryData> *entries = machine.get_entries ();
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଅଙ୍କ_entries; i++)
    {
      ଅଚଳ EntryData &data = entries[i].data;

      ଯଦି (data.markIndex != 0xFFFF)
	ଅଙ୍କ_lookups = hb_max (ଅଙ୍କ_lookups, 1u + data.markIndex);
      ଯଦି (data.currentIndex != 0xFFFF)
	ଅଙ୍କ_lookups = hb_max (ଅଙ୍କ_lookups, 1u + data.currentIndex);
    }

    return_trace (substitutionTables.sanitize (c, ଏହି, ଅଙ୍କ_lookups));
  }

  ସର୍ବସାଧାରଣ:
  StateTable<Types, EntryData>
		machine;
  ସୁରକ୍ଷିତ:
  NNOffsetTo<UnsizedListOfOffset16To<Lookup<HBGlyphID16>, HBUINT, ଶୂନ୍ୟ, ମିଥ୍ୟା>, HBUINT>
		substitutionTables;
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC ((StateTable<Types, EntryData>::static_size + HBUINT::static_size));
};


template <ତାର୍କିକ extended>
ସଂରଚନା LigatureEntry;

template <>
ସଂରଚନା LigatureEntry<ସତ୍ୟ>
{

  ସଂରଚନା EntryData
  {
    HBUINT16	ligActionIndex;	/* Index to the first ligActionTable entry
				 * for processing this group, if indicated
				 * by the flags. */
    ସର୍ବସାଧାରଣ:
    DEFINE_SIZE_STATIC (2);
  };

  ଗଣନା Flags
  {
    SetComponent	= 0x8000,	/* Push this glyph onto the component stack for
					 * eventual processing. */
    DontAdvance		= 0x4000,	/* Leave the glyph pointer at this glyph for the
					   next iteration. */
    PerformAction	= 0x2000,	/* Use the ligActionIndex to process a ligature
					 * group. */
    Reserved		= 0x1FFF,	/* These bits are reserved and should be set to 0. */
  };

  ସ୍ଥିର ତାର୍କିକ initiateAction (ଅଚଳ Entry<EntryData> &entry)
  { ଫେରନ୍ତୁ entry.flags & SetComponent; }

  ସ୍ଥିର ତାର୍କିକ performAction (ଅଚଳ Entry<EntryData> &entry)
  { ଫେରନ୍ତୁ entry.flags & PerformAction; }

  ସ୍ଥିର unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ligActionIndex (ଅଚଳ Entry<EntryData> &entry)
  { ଫେରନ୍ତୁ entry.data.ligActionIndex; }
};
template <>
ସଂରଚନା LigatureEntry<ମିଥ୍ୟା>
{
  ପ୍ରକାରବ୍ୟାଖ୍ୟା ଶୂନ୍ୟ EntryData;

  ଗଣନା Flags
  {
    SetComponent	= 0x8000,	/* Push this glyph onto the component stack for
					 * eventual processing. */
    DontAdvance		= 0x4000,	/* Leave the glyph pointer at this glyph for the
					   next iteration. */
    Offset		= 0x3FFF,	/* Byte offset from beginning of subtable to the
					 * ligature action list. This value must be a
					 * multiple of 4. */
  };

  ସ୍ଥିର ତାର୍କିକ initiateAction (ଅଚଳ Entry<EntryData> &entry)
  { ଫେରନ୍ତୁ entry.flags & SetComponent; }

  ସ୍ଥିର ତାର୍କିକ performAction (ଅଚଳ Entry<EntryData> &entry)
  { ଫେରନ୍ତୁ entry.flags & Offset; }

  ସ୍ଥିର unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ligActionIndex (ଅଚଳ Entry<EntryData> &entry)
  { ଫେରନ୍ତୁ entry.flags & Offset; }
};


template <typename Types>
ସଂରଚନା LigatureSubtable
{
  ପ୍ରକାରବ୍ୟାଖ୍ୟା typename Types::HBUINT HBUINT;

  ପ୍ରକାରବ୍ୟାଖ୍ୟା LigatureEntry<Types::extended> LigatureEntryT;
  ପ୍ରକାରବ୍ୟାଖ୍ୟା typename LigatureEntryT::EntryData EntryData;

  ଗଣନା Flags
  {
    DontAdvance	= LigatureEntryT::DontAdvance,
  };

  ତାର୍କିକ is_action_initiable (ଅଚଳ Entry<EntryData> &entry) ଅଚଳ
  {
    ଫେରନ୍ତୁ LigatureEntryT::initiateAction (entry);
  }
  ତାର୍କିକ is_actionable (ଅଚଳ Entry<EntryData> &entry) ଅଚଳ
  {
    ଫେରନ୍ତୁ LigatureEntryT::performAction (entry);
  }

  ସଂରଚନା driver_context_t
  {
    ସ୍ଥିର constexpr ତାର୍କିକ in_place = ମିଥ୍ୟା;
    ଗଣନା LigActionFlags
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

    driver_context_t (ଅଚଳ LigatureSubtable *table_,
		      hb_aat_apply_context_t *c_) :
	ret (ମିଥ୍ୟା),
	c (c_),
	table (table_),
	ligAction (table+table->ligAction),
	component (table+table->component),
	ligature (table+table->ligature),
	match_length (0) {}

    HB_AAT_TRANSITION_INLINE
    ଶୂନ୍ୟ transition (hb_buffer_t *buffer,
		     StateTableDriver<Types, EntryData, Flags> *driver,
		     ଅଚଳ Entry<EntryData> &entry)
    {
      DEBUG_ସନ୍ଦେଶ (APPLY, nullptr, "Ligature transition at %u", buffer->idx);
      ଯଦି (entry.flags & LigatureEntryT::SetComponent)
      {
	/* Never mark same index twice, in case DontAdvance was used... */
	ଯଦି (unlikely (match_length && match_positions[(match_length - 1u) % ARRAY_LENGTH (match_positions)] == buffer->out_len))
	  match_length--;

	match_positions[match_length++ % ARRAY_LENGTH (match_positions)] = buffer->out_len;
	DEBUG_ସନ୍ଦେଶ (APPLY, nullptr, "Set component at %u", buffer->out_len);
      }

      ଯଦି (LigatureEntryT::performAction (entry))
      {
	DEBUG_ସନ୍ଦେଶ (APPLY, nullptr, "Perform action with %u", match_length);
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା end = buffer->out_len;

	ଯଦି (unlikely (!match_length))
	  ଫେରନ୍ତୁ;

	ଯଦି (buffer->idx >= buffer->len)
	  ଫେରନ୍ତୁ; /* TODO Work on previous instead? */

	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା cursor = match_length;

	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା action_idx = LigatureEntryT::ligActionIndex (entry);
	action_idx = Types::offsetToIndex (action_idx, table, ligAction.arrayZ);
	ଅଚଳ HBUINT32 *actionData = &ligAction[action_idx];

	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ligature_idx = 0;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା action;
	କର
	{
	  ଯଦି (unlikely (!cursor))
	  {
	    /* Stack underflow.  Clear the stack. */
	    DEBUG_ସନ୍ଦେଶ (APPLY, nullptr, "Stack underflow");
	    match_length = 0;
	    ଭାଙ୍ଗ;
	  }

	  DEBUG_ସନ୍ଦେଶ (APPLY, nullptr, "Moving to stack position %u", cursor - 1);
	  ଯଦି (unlikely (!buffer->move_to (match_positions[--cursor % ARRAY_LENGTH (match_positions)]))) ଫେରନ୍ତୁ;

	  ଯଦି (unlikely (!actionData->sanitize (&c->sanitizer))) ଭାଙ୍ଗ;
	  hb_barrier ();
	  action = *actionData;

	  uint32_t uoffset = action & LigActionOffset;
	  ଯଦି (uoffset & 0x20000000)
	    uoffset |= 0xC0000000; /* Sign-extend. */
	  int32_t offset = (int32_t) uoffset;
	  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା component_idx = buffer->cur().codepoint + offset;
	  component_idx = Types::wordOffsetToIndex (component_idx, table, component.arrayZ);
	  ଅଚଳ HBUINT16 &componentData = component[component_idx];
	  ଯଦି (unlikely (!componentData.sanitize (&c->sanitizer))) ଭାଙ୍ଗ;
	  hb_barrier ();
	  ligature_idx += componentData;

	  DEBUG_ସନ୍ଦେଶ (APPLY, nullptr, "Action store %d last %d",
		     ତାର୍କିକ (action & LigActionStore),
		     ତାର୍କିକ (action & LigActionLast));
	  ଯଦି (action & (LigActionStore | LigActionLast))
	  {
	    ligature_idx = Types::offsetToIndex (ligature_idx, table, ligature.arrayZ);
	    ଅଚଳ HBGlyphID16 &ligatureData = ligature[ligature_idx];
	    ଯଦି (unlikely (!ligatureData.sanitize (&c->sanitizer))) ଭାଙ୍ଗ;
	    hb_barrier ();
	    hb_codepoint_t lig = ligatureData;

	    DEBUG_ସନ୍ଦେଶ (APPLY, nullptr, "Produced ligature %u", lig);
	    ଯଦି (unlikely (!c->replace_glyph (lig))) ଫେରନ୍ତୁ;

	    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା lig_end = match_positions[(match_length - 1u) % ARRAY_LENGTH (match_positions)] + 1u;
	    /* Now go and delete all subsequent components. */
	    ଯେତେବେଳେ (match_length - 1u > cursor)
	    {
	      DEBUG_ସନ୍ଦେଶ (APPLY, nullptr, "Skipping ligature component");
	      ଯଦି (unlikely (!buffer->move_to (match_positions[--match_length % ARRAY_LENGTH (match_positions)]))) ଫେରନ୍ତୁ;
	      ଯଦି (!c->delete_glyph ()) ଫେରନ୍ତୁ;
	    }

	    ଯଦି (unlikely (!buffer->move_to (lig_end))) ଫେରନ୍ତୁ;
	    buffer->merge_out_clusters (match_positions[cursor % ARRAY_LENGTH (match_positions)], buffer->out_len);
	  }

	  actionData++;
	}
	ଯେତେବେଳେ (!(action & LigActionLast));
	ଯଦି (unlikely (!buffer->move_to (end))) ଫେରନ୍ତୁ;
      }
    }

    ସର୍ବସାଧାରଣ:
    ତାର୍କିକ ret;
    hb_aat_apply_context_t *c;
    ଅଚଳ LigatureSubtable *table;
    ବ୍ୟକ୍ତିଗତ:
    ଅଚଳ UnsizedArrayOf<HBUINT32> &ligAction;
    ଅଚଳ UnsizedArrayOf<HBUINT16> &component;
    ଅଚଳ UnsizedArrayOf<HBGlyphID16> &ligature;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା match_length;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା match_positions[HB_MAX_CONTEXT_LENGTH];
  };

  ତାର୍କିକ apply (hb_aat_apply_context_t *c) ଅଚଳ
  {
    TRACE_APPLY (ଏହି);

    driver_context_t dc (ଏହି, c);

    StateTableDriver<Types, EntryData, Flags> driver (machine, c->face);

    driver.drive (&dc, c);

    return_trace (dc.ret);
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    /* The rest of array sanitizations are done at run-time. */
    return_trace (c->check_struct (ଏହି) && machine.sanitize (c) &&
		  hb_barrier () &&
		  ligAction && component && ligature);
  }

  ସର୍ବସାଧାରଣ:
  StateTable<Types, EntryData>
		machine;
  ସୁରକ୍ଷିତ:
  NNOffsetTo<UnsizedArrayOf<HBUINT32>, HBUINT>
		ligAction;	/* Offset to the ligature action table. */
  NNOffsetTo<UnsizedArrayOf<HBUINT16>, HBUINT>
		component;	/* Offset to the component table. */
  NNOffsetTo<UnsizedArrayOf<HBGlyphID16>, HBUINT>
		ligature;	/* Offset to the actual ligature lists. */
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC ((StateTable<Types, EntryData>::static_size + 3 * HBUINT::static_size));
};

template <typename Types>
ସଂରଚନା NoncontextualSubtable
{
  ତାର୍କିକ apply (hb_aat_apply_context_t *c) ଅଚଳ
  {
    TRACE_APPLY (ଏହି);

    ତାର୍କିକ ret = ମିଥ୍ୟା;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_glyphs = c->face->get_ଅଙ୍କ_glyphs ();

    hb_glyph_info_t *info = c->buffer->info;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = c->buffer->len;
    // If there's only one range, we already checked the flag.
    auto *last_range = c->range_flags && (c->range_flags->length > 1) ? &(*c->range_flags)[0] : nullptr;
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଗଣନା_2; i++)
    {
      /* This block copied from StateTableDriver::drive. Keep in sync. */
      ଯଦି (unlikely (last_range))
      {
	auto *range = last_range;
	{
	  unsigned cluster = info[i].cluster;
	  ଯେତେବେଳେ (cluster < range->cluster_first)
	    range--;
	  ଯେତେବେଳେ (cluster > range->cluster_last)
	    range++;

	  last_range = range;
	}
	ଯଦି (!(range->flags & c->subtable_flags))
	  ଜାରି;
      }

      ଅଚଳ HBGlyphID16 *replacement = substitute.get_ମୂଲ୍ୟ (info[i].codepoint, ଅଙ୍କ_glyphs);
      ଯଦି (replacement)
      {
	c->replace_glyph_inplace (i, *replacement);
	ret = ସତ୍ୟ;
      }
    }

    return_trace (ret);
  }

  template <typename set_t>
  ଶୂନ୍ୟ collect_initial_glyphs (set_t &glyphs, unsigned ଅଙ୍କ_glyphs) ଅଚଳ
  {
    substitute.collect_glyphs (glyphs, ଅଙ୍କ_glyphs);
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (substitute.sanitize (c));
  }

  ସୁରକ୍ଷିତ:
  Lookup<HBGlyphID16>	substitute;
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_MIN (2);
};

template <typename Types>
ସଂରଚନା InsertionSubtable
{
  ପ୍ରକାରବ୍ୟାଖ୍ୟା typename Types::HBUINT HBUINT;

  ସଂରଚନା EntryData
  {
    HBUINT16	current_ଭର୍ତ୍ତି_Index;	/* Zero-based index into the insertion glyph table.
					 * The number of glyphs to be inserted is contained
					 * in the currentInsertCount field in the flags.
					 * A value of 0xFFFF indicates no insertion is to
					 * be done. */
    HBUINT16	marked_ଭର୍ତ୍ତି_Index;	/* Zero-based index into the insertion glyph table.
					 * The number of glyphs to be inserted is contained
					 * in the markedInsertCount field in the flags.
					 * A value of 0xFFFF indicates no insertion is to
					 * be done. */
    ସର୍ବସାଧାରଣ:
    DEFINE_SIZE_STATIC (4);
  };

  ଗଣନା Flags
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
    Current_ଭର୍ତ୍ତି_Before= 0x0800,      /* If set, specifies that insertions are to be made
				       * to the left of the current glyph. If clear,
				       * they're made to the right of the current glyph. */
    Marked_ଭର୍ତ୍ତି_Before= 0x0400,	      /* If set, specifies that insertions are to be
				       * made to the left of the marked glyph. If clear,
				       * they're made to the right of the marked glyph. */
    Current_ଭର୍ତ୍ତି_ଗଣନା= 0x3E0,	      /* This 5-bit field is treated as a count of the
				       * number of glyphs to insert at the current
				       * position. Since zero means no insertions, the
				       * largest number of insertions at any given
				       * current location is 31 glyphs. */
    Marked_ଭର୍ତ୍ତି_ଗଣନା= 0x001F,	      /* This 5-bit field is treated as a count of the
				       * number of glyphs to insert at the marked
				       * position. Since zero means no insertions, the
				       * largest number of insertions at any given
				       * marked location is 31 glyphs. */
  };

  ତାର୍କିକ is_action_initiable (ଅଚଳ Entry<EntryData> &entry) ଅଚଳ
  {
    ଫେରନ୍ତୁ (entry.flags & SetMark);
  }
  ତାର୍କିକ is_actionable (ଅଚଳ Entry<EntryData> &entry) ଅଚଳ
  {
    ଫେରନ୍ତୁ (entry.flags & (Current_ଭର୍ତ୍ତି_ଗଣନା | Marked_ଭର୍ତ୍ତି_ଗଣନା)) &&
	   (entry.data.current_ଭର୍ତ୍ତି_Index != 0xFFFF ||entry.data.marked_ଭର୍ତ୍ତି_Index != 0xFFFF);
  }

  ସଂରଚନା driver_context_t
  {
    ସ୍ଥିର constexpr ତାର୍କିକ in_place = ମିଥ୍ୟା;

    driver_context_t (ଅଚଳ InsertionSubtable *table_,
		      hb_aat_apply_context_t *c_) :
	ret (ମିଥ୍ୟା),
	c (c_),
	table (table_),
	mark (0),
	insertionAction (table+table->insertionAction) {}

    HB_AAT_TRANSITION_INLINE
    ଶୂନ୍ୟ transition (hb_buffer_t *buffer,
		     StateTableDriver<Types, EntryData, Flags> *driver,
		     ଅଚଳ Entry<EntryData> &entry)
    {
      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags = entry.flags;

      unsigned mark_loc = buffer->out_len;

      ଯଦି (entry.data.marked_ଭର୍ତ୍ତି_Index != 0xFFFF)
      {
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = (flags & Marked_ଭର୍ତ୍ତି_ଗଣନା);
	ଯଦି (unlikely ((buffer->max_ops -= ଗଣନା_2) <= 0)) ଫେରନ୍ତୁ;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା start = entry.data.marked_ଭର୍ତ୍ତି_Index;
	ଅଚଳ HBGlyphID16 *glyphs = &insertionAction[start];
	ଯଦି (unlikely (!c->sanitizer.check_array (glyphs, ଗଣନା_2))) ଗଣନା_2 = 0;
	hb_barrier ();

	ତାର୍କିକ before = flags & Marked_ଭର୍ତ୍ତି_Before;

	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା end = buffer->out_len;
	ଯଦି (unlikely (!buffer->move_to (mark))) ଫେରନ୍ତୁ;

	ଯଦି (buffer->idx < buffer->len && !before)
	  ଯଦି (unlikely (!buffer->copy_glyph ())) ଫେରନ୍ତୁ;
	/* TODO We ignore KashidaLike setting. */
	ଯଦି (unlikely (!c->output_glyphs (ଗଣନା_2, glyphs))) ଫେରନ୍ତୁ;
	ret = ସତ୍ୟ;
	ଯଦି (buffer->idx < buffer->len && !before)
	  buffer->skip_glyph ();

	ଯଦି (unlikely (!buffer->move_to (end + ଗଣନା_2))) ଫେରନ୍ତୁ;

	buffer->unsafe_to_break_from_outbuffer (mark, hb_min (buffer->idx + 1, buffer->len));
      }

      ଯଦି (flags & SetMark)
	mark = mark_loc;

      ଯଦି (entry.data.current_ଭର୍ତ୍ତି_Index != 0xFFFF)
      {
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = (flags & Current_ଭର୍ତ୍ତି_ଗଣନା) >> 5;
	ଯଦି (unlikely ((buffer->max_ops -= ଗଣନା_2) <= 0)) ଫେରନ୍ତୁ;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା start = entry.data.current_ଭର୍ତ୍ତି_Index;
	ଅଚଳ HBGlyphID16 *glyphs = &insertionAction[start];
	ଯଦି (unlikely (!c->sanitizer.check_array (glyphs, ଗଣନା_2))) ଗଣନା_2 = 0;
	hb_barrier ();

	ତାର୍କିକ before = flags & Current_ଭର୍ତ୍ତି_Before;

	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା end = buffer->out_len;

	ଯଦି (buffer->idx < buffer->len && !before)
	  ଯଦି (unlikely (!buffer->copy_glyph ())) ଫେରନ୍ତୁ;
	/* TODO We ignore KashidaLike setting. */
	ଯଦି (unlikely (!c->output_glyphs (ଗଣନା_2, glyphs))) ଫେରନ୍ତୁ;
	ret = ସତ୍ୟ;
	ଯଦି (buffer->idx < buffer->len && !before)
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
	ଯଦି (unlikely (!buffer->move_to ((flags & DontAdvance) ? end : end + ଗଣନା_2))) ଫେରନ୍ତୁ;
      }
    }

    ସର୍ବସାଧାରଣ:
    ତାର୍କିକ ret;
    hb_aat_apply_context_t *c;
    ଅଚଳ InsertionSubtable *table;
    ବ୍ୟକ୍ତିଗତ:
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା mark;
    ଅଚଳ UnsizedArrayOf<HBGlyphID16> &insertionAction;
  };

  ତାର୍କିକ apply (hb_aat_apply_context_t *c) ଅଚଳ
  {
    TRACE_APPLY (ଏହି);

    driver_context_t dc (ଏହି, c);

    StateTableDriver<Types, EntryData, Flags> driver (machine, c->face);

    driver.drive (&dc, c);

    return_trace (dc.ret);
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    /* The rest of array sanitizations are done at run-time. */
    return_trace (c->check_struct (ଏହି) && machine.sanitize (c) &&
		  hb_barrier () &&
		  insertionAction);
  }

  ସର୍ବସାଧାରଣ:
  StateTable<Types, EntryData>
		machine;
  ସୁରକ୍ଷିତ:
  NNOffsetTo<UnsizedArrayOf<HBGlyphID16>, HBUINT>
		insertionAction;	/* Byte offset from stateHeader to the start of
					 * the insertion glyph table. */
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC ((StateTable<Types, EntryData>::static_size + HBUINT::static_size));
};


ସଂରଚନା Feature
{
  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି));
  }

  ସର୍ବସାଧାରଣ:
  HBUINT16	featureType;	/* The type of feature. */
  HBUINT16	featureSetting;	/* The feature's setting (aka selector). */
  HBUINT32	enableFlags;	/* Flags for the settings that this feature
				 * and setting enables. */
  HBUINT32	disableFlags;	/* Complement of flags for the settings that this
				 * feature and setting disable. */

  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (12);
};


ସଂରଚନା hb_accelerate_subtables_context_t :
       hb_dispatch_context_t<hb_accelerate_subtables_context_t>
{
  ସଂରଚନା hb_applicable_t
  {
    friend ସଂରଚନା hb_accelerate_subtables_context_t;
    friend ସଂରଚନା hb_aat_layout_lookup_accelerator_t;

    ସର୍ବସାଧାରଣ:
    hb_bit_set_t glyph_set;
    mutable hb_aat_class_cache_t class_cache;

    template <typename T>
    auto init_ (ଅଚଳ T &obj_, unsigned ଅଙ୍କ_glyphs, hb_priority<1>) HB_AUTO_RETURN
    (
      obj_.machine.collect_initial_glyphs (glyph_set, ଅଙ୍କ_glyphs, obj_)
    )

    template <typename T>
    ଶୂନ୍ୟ init_ (ଅଚଳ T &obj_, unsigned ଅଙ୍କ_glyphs, hb_priority<0>)
    {
      obj_.collect_initial_glyphs (glyph_set, ଅଙ୍କ_glyphs);
    }

    template <typename T>
    ଶୂନ୍ୟ init (ଅଚଳ T &obj_, unsigned ଅଙ୍କ_glyphs)
    {
      glyph_set.init ();
      init_ (obj_, ଅଙ୍କ_glyphs, hb_prioritize);
      class_cache.clear ();
    }

    ଶୂନ୍ୟ
    fini ()
    {
      glyph_set.fini ();
    }
  };

  /* Dispatch interface. */
  template <typename T>
  return_t dispatch (ଅଚଳ T &obj)
  {
    hb_applicable_t *entry = &array[i++];

    entry->init (obj, ଅଙ୍କ_glyphs);

    ଫେରନ୍ତୁ hb_empty_t ();
  }
  ସ୍ଥିର return_t default_return_ମୂଲ୍ୟ () { ଫେରନ୍ତୁ hb_empty_t (); }

  ତାର୍କିକ stop_sublookup_iteration (return_t r) ଅଚଳ { ଫେରନ୍ତୁ ମିଥ୍ୟା; }

  hb_accelerate_subtables_context_t (hb_applicable_t *array_, unsigned ଅଙ୍କ_glyphs_2) :
				     hb_dispatch_context_t<hb_accelerate_subtables_context_t> (),
				     array (array_), ଅଙ୍କ_glyphs (ଅଙ୍କ_glyphs_2) {}

  hb_applicable_t *array;
  unsigned ଅଙ୍କ_glyphs;
  unsigned i = 0;
};

ସଂରଚନା hb_aat_layout_chain_accelerator_t
{
  template <typename TChain>
  ସ୍ଥିର hb_aat_layout_chain_accelerator_t *create (ଅଚଳ TChain &chain, unsigned ଅଙ୍କ_glyphs)
  {
    unsigned ଗଣନା_2 = chain.get_subtable_ଗଣନା ();

    unsigned product;
    ଯଦି (unlikely (hb_unsigned_mul_overflows (ଗଣନା_2,
        ଆକାର (hb_accelerate_subtables_context_t::hb_applicable_t), &product)))
      ଫେରନ୍ତୁ nullptr;

    unsigned size;
    ଯଦି (unlikely (hb_unsigned_add_overflows (ଆକାର (hb_aat_layout_chain_accelerator_t) -
        HB_VAR_ARRAY * ଆକାର (hb_accelerate_subtables_context_t::hb_applicable_t), product, &size))) {
      ଫେରନ୍ତୁ nullptr;
    }

    /* The following is a calloc because when we are collecting subtables,
     * some of them might be invalid and hence not collect; as a result,
     * we might not fill in all the count entries of the subtables array.
     * Zeroing it allows the set digest to gatekeep it without having to
     * initialize it further. */
    auto *thiz = (hb_aat_layout_chain_accelerator_t *) hb_calloc (1, size);
    ଯଦି (unlikely (!thiz))
      ଫେରନ୍ତୁ nullptr;

    thiz->ଗଣନା_2 = ଗଣନା_2;

    hb_accelerate_subtables_context_t c_accelerate_subtables (thiz->subtables, ଅଙ୍କ_glyphs);
    chain.dispatch (&c_accelerate_subtables);

    ଫେରନ୍ତୁ thiz;
  }

  ଶୂନ୍ୟ destroy ()
  {
    ପାଇଁ (unsigned i = 0; i < ଗଣନା_2; i++)
      subtables[i].fini ();
  }

  unsigned ଗଣନା_2;
  hb_accelerate_subtables_context_t::hb_applicable_t subtables[HB_VAR_ARRAY];
};

template <typename Types>
ସଂରଚନା ChainSubtable
{
  ପ୍ରକାରବ୍ୟାଖ୍ୟା typename Types::HBUINT HBUINT;

  template <typename T>
  friend ସଂରଚନା Chain;

  size_t get_size () ଅଚଳ     { ଫେରନ୍ତୁ length; }
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_type () ଅଚଳ     { ଫେରନ୍ତୁ coverage & 0xFF; }
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_coverage () ଅଚଳ { ଫେରନ୍ତୁ coverage >> (ଆକାର (HBUINT) * 8 - 8); }

  ଗଣନା Coverage
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
  ଗଣନା Type
  {
    Rearrangement	= 0,
    Contextual		= 1,
    Ligature		= 2,
    Noncontextual	= 4,
    Insertion		= 5
  };

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ଅଚଳ
  {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା subtable_type = get_type ();
    TRACE_DISPATCH (ଏହି, subtable_type);
    ବାଛ (subtable_type) {
    ସ୍ଥିତି Rearrangement:		hb_barrier (); return_trace (c->dispatch (u.rearrangement, std::forward<Ts> (ds)...));
    ସ୍ଥିତି Contextual:		hb_barrier (); return_trace (c->dispatch (u.contextual, std::forward<Ts> (ds)...));
    ସ୍ଥିତି Ligature:		hb_barrier (); return_trace (c->dispatch (u.ligature, std::forward<Ts> (ds)...));
    ସ୍ଥିତି Noncontextual:		hb_barrier (); return_trace (c->dispatch (u.noncontextual, std::forward<Ts> (ds)...));
    ସ୍ଥିତି Insertion:		hb_barrier (); return_trace (c->dispatch (u.insertion, std::forward<Ts> (ds)...));
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:			return_trace (c->default_return_ମୂଲ୍ୟ ());
    }
  }

  ତାର୍କିକ apply (hb_aat_apply_context_t *c) ଅଚଳ
  {
    TRACE_APPLY (ଏହି);
    // Disabled for https://github.com/harfbuzz/harfbuzz/issues/4873
    //hb_sanitize_with_object_t with (&c->sanitizer, this);
    return_trace (dispatch (c));
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    ଯଦି (!(length.sanitize (c) &&
	  hb_barrier () &&
	  length >= min_size &&
	  c->check_range (ଏହି, length)))
      return_trace (ମିଥ୍ୟା);

    // Disabled for https://github.com/harfbuzz/harfbuzz/issues/4873
    //hb_sanitize_with_object_t with (c, this);
    return_trace (dispatch (c));
  }

  ସୁରକ୍ଷିତ:
  HBUINT	length;		/* Total subtable length, including this header. */
  HBUINT	coverage;	/* Coverage flags and subtable type. */
  HBUINT32	subFeatureFlags;/* The 32-bit mask identifying which subtable this is. */
  ସଂଘ {
  RearrangementSubtable<Types>	rearrangement;
  ContextualSubtable<Types>	contextual;
  LigatureSubtable<Types>	ligature;
  NoncontextualSubtable<Types>	noncontextual;
  InsertionSubtable<Types>	insertion;
  } u;
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_MIN (2 * ଆକାର (HBUINT) + 4);
};

template <typename Types>
ସଂରଚନା Chain
{
  ପ୍ରକାରବ୍ୟାଖ୍ୟା typename Types::HBUINT HBUINT;

  unsigned get_subtable_ଗଣନା () ଅଚଳ { ଫେରନ୍ତୁ subtable_ଗଣନା_2; }

  hb_mask_t compile_flags (ଅଚଳ hb_aat_map_builder_t *map) ଅଚଳ
  {
    hb_mask_t flags = defaultFlags;
    {
      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = feature_ଗଣନା;
      ପାଇଁ (unsigned i = 0; i < ଗଣନା_2; i++)
      {
	ଅଚଳ Feature &feature = featureZ[i];
	hb_aat_layout_feature_type_t type = (hb_aat_layout_feature_type_t) (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା) feature.featureType;
	hb_aat_layout_feature_selector_t setting = (hb_aat_layout_feature_selector_t) (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା) feature.featureSetting;
      retry:
	// Check whether this type/setting pair was requested in the map, and if so, apply its flags.
	// (The search here only looks at the type and setting fields of feature_info_t.)
	hb_aat_map_builder_t::feature_info_t info = { type, setting, ମିଥ୍ୟା, 0 };
	ଯଦି (map->current_features.bsearch (info))
	{
	  flags &= feature.disableFlags;
	  flags |= feature.enableFlags;
	}
	ଅନ୍ୟଥା ଯଦି (type == HB_AAT_LAYOUT_FEATURE_TYPE_LETTER_CASE && setting == HB_AAT_LAYOUT_FEATURE_SELECTOR_SMALL_CAPS)
	{
	  /* Deprecated. https://github.com/harfbuzz/harfbuzz/issues/1342 */
	  type = HB_AAT_LAYOUT_FEATURE_TYPE_LOWER_CASE;
	  setting = HB_AAT_LAYOUT_FEATURE_SELECTOR_LOWER_CASE_SMALL_CAPS;
	  ଯାଅ retry;
	}
#ifndef HB_NO_AAT
	ଅନ୍ୟଥା ଯଦି (type == HB_AAT_LAYOUT_FEATURE_TYPE_LANGUAGE_TAG_TYPE && setting &&
		 /* TODO: Rudimentary language matching. */
		 hb_language_matches (map->face->table.ltag->get_language (setting - 1), map->props.language))
	{
	  flags &= feature.disableFlags;
	  flags |= feature.enableFlags;
	}
#endif
      }
    }
    ଫେରନ୍ତୁ flags;
  }

  ଶୂନ୍ୟ apply (hb_aat_apply_context_t *c,
	      ଅଚଳ hb_aat_layout_chain_accelerator_t *accel) ଅଚଳ
  {
    ଅଚଳ ChainSubtable<Types> *subtable = &StructAfter<ChainSubtable<Types>> (featureZ.as_array (feature_ଗଣନା));
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = subtable_ଗଣନା_2;
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଗଣନା_2; i++)
    {
      ତାର୍କିକ reverse;

      auto coverage = subtable->get_coverage ();

      hb_mask_t subtable_flags = subtable->subFeatureFlags;
      ଯଦି (hb_none (hb_iter (c->range_flags) |
		   hb_map ([subtable_flags] (ଅଚଳ hb_aat_map_t::range_flags_t _) -> ତାର୍କିକ { ଫେରନ୍ତୁ subtable_flags & (_.flags); })))
	ଯାଅ skip;

      ଯଦି (!(coverage & ChainSubtable<Types>::AllDirections) &&
	  HB_DIRECTION_IS_VERTICAL (c->buffer->props.direction) !=
	  ତାର୍କିକ (coverage & ChainSubtable<Types>::Vertical))
	ଯାଅ skip;

      c->subtable_flags = subtable_flags;
      c->first_set = accel ? &accel->subtables[i].glyph_set : &Null(hb_bit_set_t);
      c->machine_class_cache = accel ? &accel->subtables[i].class_cache : nullptr;

      ଯଦି (!c->buffer_intersects_machine ())
      {
	(ଶୂନ୍ୟ) c->buffer->message (c->font, "skipped chainsubtable %u because no glyph matches", c->lookup_index);
	ଯାଅ skip;
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
		ତାର୍କିକ (coverage & ChainSubtable<Types>::Backwards) :
		ତାର୍କିକ (coverage & ChainSubtable<Types>::Backwards) !=
		HB_DIRECTION_IS_BACKWARD (c->buffer->props.direction);

      ଯଦି (!c->buffer->message (c->font, "start chainsubtable %u", c->lookup_index))
	ଯାଅ skip;

      ଯଦି (reverse != c->buffer_is_reversed)
        c->reverse_buffer ();

      subtable->apply (c);

      (ଶୂନ୍ୟ) c->buffer->message (c->font, "end chainsubtable %u", c->lookup_index);

      ଯଦି (unlikely (!c->buffer->successful)) ଭାଙ୍ଗ;

    skip:
      subtable = &StructAfter<ChainSubtable<Types>> (*subtable);
      c->set_lookup_index (c->lookup_index + 1);
    }
    ଯଦି (c->buffer_is_reversed)
      c->reverse_buffer ();
  }

  size_t get_size () ଅଚଳ { ଫେରନ୍ତୁ length; }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ଅଚଳ
  {
    ଅଚଳ ChainSubtable<Types> *subtable = &StructAfter<ChainSubtable<Types>> (featureZ.as_array (feature_ଗଣନା));
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = subtable_ଗଣନା_2;
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଗଣନା_2; i++)
    {
      typename context_t::return_t ret = subtable->dispatch (c, std::forward<Ts> (ds)...);
      ଯଦି (c->stop_sublookup_iteration (ret))
	ଫେରନ୍ତୁ ret;
      subtable = &StructAfter<ChainSubtable<Types>> (*subtable);
    }
    ଫେରନ୍ତୁ c->default_return_ମୂଲ୍ୟ ();
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା version) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    ଯଦି (!(length.sanitize (c) &&
	  hb_barrier () &&
	  length >= min_size &&
	  c->check_range (ଏହି, length)))
      return_trace (ମିଥ୍ୟା);

    ଯଦି (!c->check_array (featureZ.arrayZ, feature_ଗଣନା))
      return_trace (ମିଥ୍ୟା);

    ଅଚଳ ChainSubtable<Types> *subtable = &StructAfter<ChainSubtable<Types>> (featureZ.as_array (feature_ଗଣନା));
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = subtable_ଗଣନା_2;
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଗଣନା_2; i++)
    {
      ଯଦି (!subtable->sanitize (c))
	return_trace (ମିଥ୍ୟା);
      hb_barrier ();
      subtable = &StructAfter<ChainSubtable<Types>> (*subtable);
    }

    ଯଦି (version >= 3)
    {
      ଅଚଳ SubtableGlyphCoverage *coverage = (ଅଚଳ SubtableGlyphCoverage *) subtable;
      ଯଦି (!coverage->sanitize (c, ଗଣନା_2))
        return_trace (ମିଥ୍ୟା);
    }

    return_trace (ସତ୍ୟ);
  }

  ସୁରକ୍ଷିତ:
  HBUINT32	defaultFlags;	/* The default specification for subtables. */
  HBUINT32	length;		/* Total byte count, including this header. */
  HBUINT	feature_ଗଣନା;	/* Number of feature subtable entries. */
  HBUINT	subtable_ଗଣନା_2;	/* The number of subtables in the chain. */

  UnsizedArrayOf<Feature>	featureZ;	/* Features. */
/*ChainSubtable	firstSubtable;*//* Subtables. */
/*SubtableGlyphCoverage coverages*//* Only if version >= 3. */

  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_MIN (8 + 2 * ଆକାର (HBUINT));
};


/*
 * The 'mort'/'morx' Table
 */

template <typename T, typename Types, hb_tag_t TAG>
ସଂରଚନା mortmorx
{
  ସ୍ଥିର constexpr hb_tag_t tableTag = TAG;

  ତାର୍କିକ has_data () ଅଚଳ { ଫେରନ୍ତୁ version != 0; }

  ସଂରଚନା accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      hb_sanitize_context_t sc;
      ଏହି->table = sc.reference_table<T> (face);

      ଯଦି (unlikely (ଏହି->table->is_blocklisted (ଏହି->table.get_blob (), face)))
      {
        hb_blob_destroy (ଏହି->table.get_blob ());
        ଏହି->table = hb_blob_get_empty ();
      }

      ଏହି->chain_ଗଣନା = table->get_chain_ଗଣନା ();

      ଏହି->accels = (hb_atomic_t<hb_aat_layout_chain_accelerator_t *> *) hb_calloc (ଏହି->chain_ଗଣନା, ଆକାର (*accels));
      ଯଦି (unlikely (!ଏହି->accels))
      {
	ଏହି->chain_ଗଣନା = 0;
	ଏହି->table.destroy ();
	ଏହି->table = hb_blob_get_empty ();
      }
    }
    ~accelerator_t ()
    {
      ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଏହି->chain_ଗଣନା; i++)
      {
	ଯଦି (ଏହି->accels[i])
	  ଏହି->accels[i]->destroy ();
	hb_free (ଏହି->accels[i]);
      }
      hb_free (ଏହି->accels);
      ଏହି->table.destroy ();
    }

    hb_blob_t *get_blob () ଅଚଳ { ଫେରନ୍ତୁ table.get_blob (); }

    template <typename Chain>
    hb_aat_layout_chain_accelerator_t *get_accel (unsigned chain_index, ଅଚଳ Chain &chain, unsigned ଅଙ୍କ_glyphs) ଅଚଳ
    {
      ଯଦି (unlikely (chain_index >= chain_ଗଣନା)) ଫେରନ୍ତୁ nullptr;

    retry:
      auto *accel = accels[chain_index].get_acquire ();
      ଯଦି (unlikely (!accel))
      {
	accel = hb_aat_layout_chain_accelerator_t::create (chain, ଅଙ୍କ_glyphs);
	ଯଦି (unlikely (!accel))
	  ଫେରନ୍ତୁ nullptr;

	ଯଦି (unlikely (!accels[chain_index].cmpexch (nullptr, accel)))
	{
	  hb_free (accel);
	  ଯାଅ retry;
	}
      }

      ଫେରନ୍ତୁ accel;
    }

    hb_blob_ptr_t<T> table;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା chain_ଗଣନା;
    hb_atomic_t<hb_aat_layout_chain_accelerator_t *> *accels;
    hb_aat_scratch_t scratch;
  };


  ଶୂନ୍ୟ compile_flags (ଅଚଳ hb_aat_map_builder_t *mapper,
		      hb_aat_map_t *map) ଅଚଳ
  {
    ଅଚଳ Chain<Types> *chain = &firstChain;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = chain_ଗଣନା_2;
    ଯଦି (unlikely (!map->chain_flags.resize (ଗଣନା_2)))
      ଫେରନ୍ତୁ;
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଗଣନା_2; i++)
    {
      map->chain_flags[i].push (hb_aat_map_t::range_flags_t {chain->compile_flags (mapper),
							     mapper->range_first,
							     mapper->range_last});
      chain = &StructAfter<Chain<Types>> (*chain);
    }
  }

  unsigned get_chain_ଗଣନା () ଅଚଳ
  {
    ଫେରନ୍ତୁ chain_ଗଣନା_2;
  }
  ଶୂନ୍ୟ apply (hb_aat_apply_context_t *c,
	      ଅଚଳ hb_aat_map_t &map,
	      ଅଚଳ accelerator_t &accel) ଅଚଳ
  {
    ଯଦି (unlikely (!c->buffer->successful)) ଫେରନ୍ତୁ;

    c->buffer->unsafe_to_concat ();

    c->setup_buffer_glyph_set ();

    c->set_lookup_index (0);
    ଅଚଳ Chain<Types> *chain = &firstChain;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = chain_ଗଣନା_2;
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଗଣନା_2; i++)
    {
      auto *chain_accel = accel.get_accel (i, *chain, c->face->get_ଅଙ୍କ_glyphs ());
      c->range_flags = &map.chain_flags[i];
      chain->apply (c, chain_accel);
      ଯଦି (unlikely (!c->buffer->successful)) ଫେରନ୍ତୁ;
      chain = &StructAfter<Chain<Types>> (*chain);
    }
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    ଯଦି (!(version.sanitize (c) &&
	  hb_barrier () &&
	  version &&
	  chain_ଗଣନା_2.sanitize (c)))
      return_trace (ମିଥ୍ୟା);

    ଅଚଳ Chain<Types> *chain = &firstChain;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = chain_ଗଣନା_2;
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଗଣନା_2; i++)
    {
      ଯଦି (!chain->sanitize (c, version))
	return_trace (ମିଥ୍ୟା);
      hb_barrier ();
      chain = &StructAfter<Chain<Types>> (*chain);
    }

    return_trace (ସତ୍ୟ);
  }

  ସୁରକ୍ଷିତ:
  HBUINT16	version;	/* Version number of the glyph metamorphosis table.
				 * 1, 2, or 3. */
  HBUINT16	unused;		/* Set to 0. */
  HBUINT32	chain_ଗଣନା_2;	/* Number of metamorphosis chains contained in this
				 * table. */
  Chain<Types>	firstChain;	/* Chains. */

  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_MIN (8);
};

ସଂରଚନା morx : mortmorx<morx, ExtendedTypes, HB_AAT_TAG_morx>
{
  HB_INTERNAL ତାର୍କିକ is_blocklisted (hb_blob_t *blob,
                                   hb_face_t *face) ଅଚଳ;
};

ସଂରଚନା mort : mortmorx<mort, ObsoleteTypes, HB_AAT_TAG_mort>
{
  HB_INTERNAL ତାର୍କିକ is_blocklisted (hb_blob_t *blob,
                                   hb_face_t *face) ଅଚଳ;
};

ସଂରଚନା morx_accelerator_t : morx::accelerator_t {
  morx_accelerator_t (hb_face_t *face) : morx::accelerator_t (face) {}
};
ସଂରଚନା mort_accelerator_t : mort::accelerator_t {
  mort_accelerator_t (hb_face_t *face) : mort::accelerator_t (face) {}
};


} /* namespace AAT */


#endif /* HB_AAT_LAYOUT_MORX_TABLE_HH */
