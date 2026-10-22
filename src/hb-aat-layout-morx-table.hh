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

#include "hb-open-type.hh"
#include "hb-aat-layout-common.hh"
#include "hb-ot-layout.hh"
#include "hb-aat-map.hh"

/*
 * morx -- Extended Glyph Metamorphosis
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6morx.html
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6mort.html
 */
#define HB_AAT_TAG_morx HB_TAG('m','o','r','x')
#define HB_AAT_TAG_mort HB_TAG('m','o','r','t')


namespace AAT {

using namespace OT;

template <typename Types>
struct RearrangementSubtable
{
  typedef typename Types::HBUINT HBUINT;

  typedef void EntryData;

  enum Flags
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

  bool is_action_initiable (const Entry<EntryData> &entry) const
  {
    반환 (entry.flags & MarkFirst);
  }
  bool is_actionable (const Entry<EntryData> &entry) const
  {
    반환 (entry.flags & Verb);
  }

  struct driver_context_t
  {
    static constexpr bool in_place = true;

    driver_context_t (const RearrangementSubtable *table_) :
	ret (false),
	table (table_),
	start (0), end (0) {}

    HB_AAT_TRANSITION_INLINE
    void transition (hb_buffer_t *buffer,
		     StateTableDriver<Types, EntryData, Flags> *driver,
		     const Entry<EntryData> &entry)
    {
      unsigned 정수 flags = entry.flags;

      만약 (flags & MarkFirst)
	start = buffer->idx;

      만약 (flags & MarkLast)
	end = hb_min (buffer->idx + 1, buffer->len);

      만약 ((flags & Verb) && start < end)
      {
	/* The following map has two nibbles, for start-side
	 * and end-side. Values of 0,1,2 mean move that many
	 * to the other side. Value of 3 means move 2 and
	 * flip them. */
	const unsigned char map[16] =
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

	unsigned 정수 m = map[flags & Verb];
	unsigned 정수 l = hb_min (2u, m >> 4);
	unsigned 정수 r = hb_min (2u, m & 0x0F);
	bool reverse_l = 3 == (m >> 4);
	bool reverse_r = 3 == (m & 0x0F);

	만약 (end - start >= l + r && end-start <= HB_MAX_CONTEXT_LENGTH)
	{
	  buffer->merge_clusters (start, hb_min (buffer->idx + 1, buffer->len));
	  buffer->merge_clusters (start, end);

	  hb_glyph_info_t *info = buffer->info;
	  hb_glyph_info_t buf[4];

	  hb_memcpy (buf, info + start, l * sizeof (buf[0]));
	  hb_memcpy (buf + 2, info + end - r, r * sizeof (buf[0]));

	  만약 (l != r)
	    memmove (info + start + r, info + start + l, (end - start - l - r) * sizeof (buf[0]));

	  hb_memcpy (info + start, buf + 2, r * sizeof (buf[0]));
	  hb_memcpy (info + end - l, buf, l * sizeof (buf[0]));
	  만약 (reverse_l)
	  {
	    buf[0] = info[end - 1];
	    info[end - 1] = info[end - 2];
	    info[end - 2] = buf[0];
	  }
	  만약 (reverse_r)
	  {
	    buf[0] = info[start];
	    info[start] = info[start + 1];
	    info[start + 1] = buf[0];
	  }
	}
      }
    }

    public:
    bool ret;
    const RearrangementSubtable *table;
    private:
    unsigned 정수 start;
    unsigned 정수 end;
  };

  bool apply (hb_aat_apply_context_t *c) const
  {
    TRACE_APPLY (this);

    driver_context_t dc (this);

    StateTableDriver<Types, EntryData, Flags> driver (machine, c->face);

    driver.drive (&dc, c);

    return_trace (dc.ret);
  }

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (machine.sanitize (c));
  }

  public:
  StateTable<Types, EntryData>	machine;
  public:
  DEFINE_SIZE_STATIC ((StateTable<Types, EntryData>::static_size));
};

template <typename Types>
struct ContextualSubtable
{
  typedef typename Types::HBUINT HBUINT;

  struct EntryData
  {
    typedef typename std::conditional<Types::extended, HBUINT16, HBINT16>::type OffsetType;

    OffsetType	markIndex;	/* Index/offset of the substitution table for the
				 * marked glyph. */
    OffsetType	currentIndex;	/* Index/offset of the substitution table for the
				 * current glyph. */
    public:
    DEFINE_SIZE_STATIC (4);
  };

  enum Flags
  {
    SetMark		= 0x8000,	/* If set, make the current glyph the marked glyph. */
    DontAdvance		= 0x4000,	/* If set, don't advance to the next glyph before
					 * going to the new state. */
    Reserved		= 0x3FFF,	/* These bits are reserved and should be set to 0. */
  };

  bool is_action_initiable (const Entry<EntryData> &entry) const
  {
    반환 (entry.flags & SetMark);
  }
  bool is_actionable (const Entry<EntryData> &entry) const
  {
    만약 (Types::extended)
      반환 entry.data.markIndex != 0xFFFF || entry.data.currentIndex != 0xFFFF;
    아니면
      반환 entry.data.markIndex || entry.data.currentIndex;
  }

  struct driver_context_t
  {
    static constexpr bool in_place = true;

    driver_context_t (const ContextualSubtable *table_,
			     hb_aat_apply_context_t *c_) :
	ret (false),
	c (c_),
	table (table_),
	mark_set (false),
	mark (0),
	subs (table+table->substitutionTables) {}

    HB_AAT_TRANSITION_INLINE
    void transition (hb_buffer_t *buffer,
		     StateTableDriver<Types, EntryData, Flags> *driver,
		     const Entry<EntryData> &entry)
    {
      /* Looks like CoreText applies neither mark nor current substitution for
       * end-of-text if mark was not explicitly set. */
      만약 (buffer->idx == buffer->len && !mark_set)
	반환;

      const HBGlyphID16 *replacement;

      replacement = nullptr;
      만약 (Types::extended)
      {
	만약 (entry.data.markIndex != 0xFFFF)
	{
	  const Lookup<HBGlyphID16> &lookup = subs[entry.data.markIndex];
	  replacement = lookup.get_value (buffer->info[mark].codepoint, driver->num_glyphs);
	}
      }
      아니면
      {
	만약 (entry.data.markIndex)
	{
	  정수 offset = (정수) entry.data.markIndex + buffer->info[mark].codepoint;
	  const UnsizedArrayOf<HBGlyphID16> &subs_old = (const UnsizedArrayOf<HBGlyphID16> &) subs;
	  replacement = &subs_old[Types::wordOffsetToIndex (offset, table, subs_old.arrayZ)];
	  만약 (!(replacement->sanitize (&c->sanitizer) &&
		hb_barrier () &&
		*replacement))
	    replacement = nullptr;
	}
      }
      만약 (replacement)
      {
	buffer->unsafe_to_break (mark, hb_min (buffer->idx + 1, buffer->len));
	c->replace_glyph_inplace (mark, *replacement);
	ret = true;
      }

      replacement = nullptr;
      unsigned 정수 idx = hb_min (buffer->idx, buffer->len - 1);
      만약 (Types::extended)
      {
	만약 (entry.data.currentIndex != 0xFFFF)
	{
	  const Lookup<HBGlyphID16> &lookup = subs[entry.data.currentIndex];
	  replacement = lookup.get_value (buffer->info[idx].codepoint, driver->num_glyphs);
	}
      }
      아니면
      {
	만약 (entry.data.currentIndex)
	{
	  정수 offset = (정수) entry.data.currentIndex + buffer->info[idx].codepoint;
	  const UnsizedArrayOf<HBGlyphID16> &subs_old = (const UnsizedArrayOf<HBGlyphID16> &) subs;
	  replacement = &subs_old[Types::wordOffsetToIndex (offset, table, subs_old.arrayZ)];
	  만약 (!(replacement->sanitize (&c->sanitizer) &&
		hb_barrier () &&
		*replacement))
	    replacement = nullptr;
	}
      }
      만약 (replacement)
      {
	c->replace_glyph_inplace (idx, *replacement);
	ret = true;
      }

      만약 (entry.flags & SetMark)
      {
	mark_set = true;
	mark = buffer->idx;
      }
    }

    public:
    bool ret;
    hb_aat_apply_context_t *c;
    const ContextualSubtable *table;
    private:
    bool mark_set;
    unsigned 정수 mark;
    const UnsizedListOfOffset16To<Lookup<HBGlyphID16>, HBUINT, void, false> &subs;
  };

  bool apply (hb_aat_apply_context_t *c) const
  {
    TRACE_APPLY (this);

    driver_context_t dc (this, c);

    StateTableDriver<Types, EntryData, Flags> driver (machine, c->face);

    driver.drive (&dc, c);

    return_trace (dc.ret);
  }

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);

    unsigned 정수 num_entries = 0;
    만약 (unlikely (!machine.sanitize (c, &num_entries))) return_trace (false);
    hb_barrier ();

    만약 (!Types::extended)
      return_trace (substitutionTables.sanitize (c, this, 0));

    unsigned 정수 num_lookups = 0;

    const Entry<EntryData> *entries = machine.get_entries ();
    위해 (unsigned 정수 i = 0; i < num_entries; i++)
    {
      const EntryData &data = entries[i].data;

      만약 (data.markIndex != 0xFFFF)
	num_lookups = hb_max (num_lookups, 1u + data.markIndex);
      만약 (data.currentIndex != 0xFFFF)
	num_lookups = hb_max (num_lookups, 1u + data.currentIndex);
    }

    return_trace (substitutionTables.sanitize (c, this, num_lookups));
  }

  public:
  StateTable<Types, EntryData>
		machine;
  protected:
  NNOffsetTo<UnsizedListOfOffset16To<Lookup<HBGlyphID16>, HBUINT, void, false>, HBUINT>
		substitutionTables;
  public:
  DEFINE_SIZE_STATIC ((StateTable<Types, EntryData>::static_size + HBUINT::static_size));
};


template <bool extended>
struct LigatureEntry;

template <>
struct LigatureEntry<true>
{

  struct EntryData
  {
    HBUINT16	ligActionIndex;	/* Index to the first ligActionTable entry
				 * for processing this group, if indicated
				 * by the flags. */
    public:
    DEFINE_SIZE_STATIC (2);
  };

  enum Flags
  {
    SetComponent	= 0x8000,	/* Push this glyph onto the component stack for
					 * eventual processing. */
    DontAdvance		= 0x4000,	/* Leave the glyph pointer at this glyph for the
					   next iteration. */
    PerformAction	= 0x2000,	/* Use the ligActionIndex to process a ligature
					 * group. */
    Reserved		= 0x1FFF,	/* These bits are reserved and should be set to 0. */
  };

  static bool initiateAction (const Entry<EntryData> &entry)
  { 반환 entry.flags & SetComponent; }

  static bool performAction (const Entry<EntryData> &entry)
  { 반환 entry.flags & PerformAction; }

  static unsigned 정수 ligActionIndex (const Entry<EntryData> &entry)
  { 반환 entry.data.ligActionIndex; }
};
template <>
struct LigatureEntry<false>
{
  typedef void EntryData;

  enum Flags
  {
    SetComponent	= 0x8000,	/* Push this glyph onto the component stack for
					 * eventual processing. */
    DontAdvance		= 0x4000,	/* Leave the glyph pointer at this glyph for the
					   next iteration. */
    Offset		= 0x3FFF,	/* Byte offset from beginning of subtable to the
					 * ligature action list. This value must be a
					 * multiple of 4. */
  };

  static bool initiateAction (const Entry<EntryData> &entry)
  { 반환 entry.flags & SetComponent; }

  static bool performAction (const Entry<EntryData> &entry)
  { 반환 entry.flags & Offset; }

  static unsigned 정수 ligActionIndex (const Entry<EntryData> &entry)
  { 반환 entry.flags & Offset; }
};


template <typename Types>
struct LigatureSubtable
{
  typedef typename Types::HBUINT HBUINT;

  typedef LigatureEntry<Types::extended> LigatureEntryT;
  typedef typename LigatureEntryT::EntryData EntryData;

  enum Flags
  {
    DontAdvance	= LigatureEntryT::DontAdvance,
  };

  bool is_action_initiable (const Entry<EntryData> &entry) const
  {
    반환 LigatureEntryT::initiateAction (entry);
  }
  bool is_actionable (const Entry<EntryData> &entry) const
  {
    반환 LigatureEntryT::performAction (entry);
  }

  struct driver_context_t
  {
    static constexpr bool in_place = false;
    enum LigActionFlags
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

    driver_context_t (const LigatureSubtable *table_,
		      hb_aat_apply_context_t *c_) :
	ret (false),
	c (c_),
	table (table_),
	ligAction (table+table->ligAction),
	component (table+table->component),
	ligature (table+table->ligature),
	match_length (0) {}

    HB_AAT_TRANSITION_INLINE
    void transition (hb_buffer_t *buffer,
		     StateTableDriver<Types, EntryData, Flags> *driver,
		     const Entry<EntryData> &entry)
    {
      DEBUG_MSG (APPLY, nullptr, "Ligature transition at %u", buffer->idx);
      만약 (entry.flags & LigatureEntryT::SetComponent)
      {
	/* Never mark same index twice, in case DontAdvance was used... */
	만약 (unlikely (match_length && match_positions[(match_length - 1u) % ARRAY_LENGTH (match_positions)] == buffer->out_len))
	  match_length--;

	match_positions[match_length++ % ARRAY_LENGTH (match_positions)] = buffer->out_len;
	DEBUG_MSG (APPLY, nullptr, "Set component at %u", buffer->out_len);
      }

      만약 (LigatureEntryT::performAction (entry))
      {
	DEBUG_MSG (APPLY, nullptr, "Perform action with %u", match_length);
	unsigned 정수 end = buffer->out_len;

	만약 (unlikely (!match_length))
	  반환;

	만약 (buffer->idx >= buffer->len)
	  반환; /* TODO Work on previous instead? */

	unsigned 정수 cursor = match_length;

	unsigned 정수 action_idx = LigatureEntryT::ligActionIndex (entry);
	action_idx = Types::offsetToIndex (action_idx, table, ligAction.arrayZ);
	const HBUINT32 *actionData = &ligAction[action_idx];

	unsigned 정수 ligature_idx = 0;
	unsigned 정수 action;
	do
	{
	  만약 (unlikely (!cursor))
	  {
	    /* Stack underflow.  Clear the stack. */
	    DEBUG_MSG (APPLY, nullptr, "Stack underflow");
	    match_length = 0;
	    break;
	  }

	  DEBUG_MSG (APPLY, nullptr, "Moving to stack position %u", cursor - 1);
	  만약 (unlikely (!buffer->move_to (match_positions[--cursor % ARRAY_LENGTH (match_positions)]))) 반환;

	  만약 (unlikely (!actionData->sanitize (&c->sanitizer))) break;
	  hb_barrier ();
	  action = *actionData;

	  uint32_t uoffset = action & LigActionOffset;
	  만약 (uoffset & 0x20000000)
	    uoffset |= 0xC0000000; /* Sign-extend. */
	  int32_t offset = (int32_t) uoffset;
	  unsigned 정수 component_idx = buffer->cur().codepoint + offset;
	  component_idx = Types::wordOffsetToIndex (component_idx, table, component.arrayZ);
	  const HBUINT16 &componentData = component[component_idx];
	  만약 (unlikely (!componentData.sanitize (&c->sanitizer))) break;
	  hb_barrier ();
	  ligature_idx += componentData;

	  DEBUG_MSG (APPLY, nullptr, "Action store %d last %d",
		     bool (action & LigActionStore),
		     bool (action & LigActionLast));
	  만약 (action & (LigActionStore | LigActionLast))
	  {
	    ligature_idx = Types::offsetToIndex (ligature_idx, table, ligature.arrayZ);
	    const HBGlyphID16 &ligatureData = ligature[ligature_idx];
	    만약 (unlikely (!ligatureData.sanitize (&c->sanitizer))) break;
	    hb_barrier ();
	    hb_codepoint_t lig = ligatureData;

	    DEBUG_MSG (APPLY, nullptr, "Produced ligature %u", lig);
	    만약 (unlikely (!c->replace_glyph (lig))) 반환;

	    unsigned 정수 lig_end = match_positions[(match_length - 1u) % ARRAY_LENGTH (match_positions)] + 1u;
	    /* Now go and delete all subsequent components. */
	    동안 (match_length - 1u > cursor)
	    {
	      DEBUG_MSG (APPLY, nullptr, "Skipping ligature component");
	      만약 (unlikely (!buffer->move_to (match_positions[--match_length % ARRAY_LENGTH (match_positions)]))) 반환;
	      만약 (!c->delete_glyph ()) 반환;
	    }

	    만약 (unlikely (!buffer->move_to (lig_end))) 반환;
	    buffer->merge_out_clusters (match_positions[cursor % ARRAY_LENGTH (match_positions)], buffer->out_len);
	  }

	  actionData++;
	}
	동안 (!(action & LigActionLast));
	만약 (unlikely (!buffer->move_to (end))) 반환;
      }
    }

    public:
    bool ret;
    hb_aat_apply_context_t *c;
    const LigatureSubtable *table;
    private:
    const UnsizedArrayOf<HBUINT32> &ligAction;
    const UnsizedArrayOf<HBUINT16> &component;
    const UnsizedArrayOf<HBGlyphID16> &ligature;
    unsigned 정수 match_length;
    unsigned 정수 match_positions[HB_MAX_CONTEXT_LENGTH];
  };

  bool apply (hb_aat_apply_context_t *c) const
  {
    TRACE_APPLY (this);

    driver_context_t dc (this, c);

    StateTableDriver<Types, EntryData, Flags> driver (machine, c->face);

    driver.drive (&dc, c);

    return_trace (dc.ret);
  }

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    /* The rest of array sanitizations are done at run-time. */
    return_trace (c->check_struct (this) && machine.sanitize (c) &&
		  hb_barrier () &&
		  ligAction && component && ligature);
  }

  public:
  StateTable<Types, EntryData>
		machine;
  protected:
  NNOffsetTo<UnsizedArrayOf<HBUINT32>, HBUINT>
		ligAction;	/* Offset to the ligature action table. */
  NNOffsetTo<UnsizedArrayOf<HBUINT16>, HBUINT>
		component;	/* Offset to the component table. */
  NNOffsetTo<UnsizedArrayOf<HBGlyphID16>, HBUINT>
		ligature;	/* Offset to the actual ligature lists. */
  public:
  DEFINE_SIZE_STATIC ((StateTable<Types, EntryData>::static_size + 3 * HBUINT::static_size));
};

template <typename Types>
struct NoncontextualSubtable
{
  bool apply (hb_aat_apply_context_t *c) const
  {
    TRACE_APPLY (this);

    bool ret = false;
    unsigned 정수 num_glyphs = c->face->get_num_glyphs ();

    hb_glyph_info_t *info = c->buffer->info;
    unsigned 정수 count = c->buffer->len;
    // If there's only one range, we already checked the flag.
    auto *last_range = c->range_flags && (c->range_flags->length > 1) ? &(*c->range_flags)[0] : nullptr;
    위해 (unsigned 정수 i = 0; i < count; i++)
    {
      /* This block copied from StateTableDriver::drive. Keep in sync. */
      만약 (unlikely (last_range))
      {
	auto *range = last_range;
	{
	  unsigned cluster = info[i].cluster;
	  동안 (cluster < range->cluster_first)
	    range--;
	  동안 (cluster > range->cluster_last)
	    range++;

	  last_range = range;
	}
	만약 (!(range->flags & c->subtable_flags))
	  continue;
      }

      const HBGlyphID16 *replacement = substitute.get_value (info[i].codepoint, num_glyphs);
      만약 (replacement)
      {
	c->replace_glyph_inplace (i, *replacement);
	ret = true;
      }
    }

    return_trace (ret);
  }

  template <typename set_t>
  void collect_initial_glyphs (set_t &glyphs, unsigned num_glyphs) const
  {
    substitute.collect_glyphs (glyphs, num_glyphs);
  }

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (substitute.sanitize (c));
  }

  protected:
  Lookup<HBGlyphID16>	substitute;
  public:
  DEFINE_SIZE_MIN (2);
};

template <typename Types>
struct InsertionSubtable
{
  typedef typename Types::HBUINT HBUINT;

  struct EntryData
  {
    HBUINT16	currentInsertIndex;	/* Zero-based index into the insertion glyph table.
					 * The number of glyphs to be inserted is contained
					 * in the currentInsertCount field in the flags.
					 * A value of 0xFFFF indicates no insertion is to
					 * be done. */
    HBUINT16	markedInsertIndex;	/* Zero-based index into the insertion glyph table.
					 * The number of glyphs to be inserted is contained
					 * in the markedInsertCount field in the flags.
					 * A value of 0xFFFF indicates no insertion is to
					 * be done. */
    public:
    DEFINE_SIZE_STATIC (4);
  };

  enum Flags
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
    CurrentInsertBefore= 0x0800,      /* If set, specifies that insertions are to be made
				       * to the left of the current glyph. If clear,
				       * they're made to the right of the current glyph. */
    MarkedInsertBefore= 0x0400,	      /* If set, specifies that insertions are to be
				       * made to the left of the marked glyph. If clear,
				       * they're made to the right of the marked glyph. */
    CurrentInsertCount= 0x3E0,	      /* This 5-bit field is treated as a count of the
				       * number of glyphs to insert at the current
				       * position. Since zero means no insertions, the
				       * largest number of insertions at any given
				       * current location is 31 glyphs. */
    MarkedInsertCount= 0x001F,	      /* This 5-bit field is treated as a count of the
				       * number of glyphs to insert at the marked
				       * position. Since zero means no insertions, the
				       * largest number of insertions at any given
				       * marked location is 31 glyphs. */
  };

  bool is_action_initiable (const Entry<EntryData> &entry) const
  {
    반환 (entry.flags & SetMark);
  }
  bool is_actionable (const Entry<EntryData> &entry) const
  {
    반환 (entry.flags & (CurrentInsertCount | MarkedInsertCount)) &&
	   (entry.data.currentInsertIndex != 0xFFFF ||entry.data.markedInsertIndex != 0xFFFF);
  }

  struct driver_context_t
  {
    static constexpr bool in_place = false;

    driver_context_t (const InsertionSubtable *table_,
		      hb_aat_apply_context_t *c_) :
	ret (false),
	c (c_),
	table (table_),
	mark (0),
	insertionAction (table+table->insertionAction) {}

    HB_AAT_TRANSITION_INLINE
    void transition (hb_buffer_t *buffer,
		     StateTableDriver<Types, EntryData, Flags> *driver,
		     const Entry<EntryData> &entry)
    {
      unsigned 정수 flags = entry.flags;

      unsigned mark_loc = buffer->out_len;

      만약 (entry.data.markedInsertIndex != 0xFFFF)
      {
	unsigned 정수 count = (flags & MarkedInsertCount);
	만약 (unlikely ((buffer->max_ops -= count) <= 0)) 반환;
	unsigned 정수 start = entry.data.markedInsertIndex;
	const HBGlyphID16 *glyphs = &insertionAction[start];
	만약 (unlikely (!c->sanitizer.check_array (glyphs, count))) count = 0;
	hb_barrier ();

	bool before = flags & MarkedInsertBefore;

	unsigned 정수 end = buffer->out_len;
	만약 (unlikely (!buffer->move_to (mark))) 반환;

	만약 (buffer->idx < buffer->len && !before)
	  만약 (unlikely (!buffer->copy_glyph ())) 반환;
	/* TODO We ignore KashidaLike setting. */
	만약 (unlikely (!c->output_glyphs (count, glyphs))) 반환;
	ret = true;
	만약 (buffer->idx < buffer->len && !before)
	  buffer->skip_glyph ();

	만약 (unlikely (!buffer->move_to (end + count))) 반환;

	buffer->unsafe_to_break_from_outbuffer (mark, hb_min (buffer->idx + 1, buffer->len));
      }

      만약 (flags & SetMark)
	mark = mark_loc;

      만약 (entry.data.currentInsertIndex != 0xFFFF)
      {
	unsigned 정수 count = (flags & CurrentInsertCount) >> 5;
	만약 (unlikely ((buffer->max_ops -= count) <= 0)) 반환;
	unsigned 정수 start = entry.data.currentInsertIndex;
	const HBGlyphID16 *glyphs = &insertionAction[start];
	만약 (unlikely (!c->sanitizer.check_array (glyphs, count))) count = 0;
	hb_barrier ();

	bool before = flags & CurrentInsertBefore;

	unsigned 정수 end = buffer->out_len;

	만약 (buffer->idx < buffer->len && !before)
	  만약 (unlikely (!buffer->copy_glyph ())) 반환;
	/* TODO We ignore KashidaLike setting. */
	만약 (unlikely (!c->output_glyphs (count, glyphs))) 반환;
	ret = true;
	만약 (buffer->idx < buffer->len && !before)
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
	만약 (unlikely (!buffer->move_to ((flags & DontAdvance) ? end : end + count))) 반환;
      }
    }

    public:
    bool ret;
    hb_aat_apply_context_t *c;
    const InsertionSubtable *table;
    private:
    unsigned 정수 mark;
    const UnsizedArrayOf<HBGlyphID16> &insertionAction;
  };

  bool apply (hb_aat_apply_context_t *c) const
  {
    TRACE_APPLY (this);

    driver_context_t dc (this, c);

    StateTableDriver<Types, EntryData, Flags> driver (machine, c->face);

    driver.drive (&dc, c);

    return_trace (dc.ret);
  }

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    /* The rest of array sanitizations are done at run-time. */
    return_trace (c->check_struct (this) && machine.sanitize (c) &&
		  hb_barrier () &&
		  insertionAction);
  }

  public:
  StateTable<Types, EntryData>
		machine;
  protected:
  NNOffsetTo<UnsizedArrayOf<HBGlyphID16>, HBUINT>
		insertionAction;	/* Byte offset from stateHeader to the start of
					 * the insertion glyph table. */
  public:
  DEFINE_SIZE_STATIC ((StateTable<Types, EntryData>::static_size + HBUINT::static_size));
};


struct Feature
{
  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (c->check_struct (this));
  }

  public:
  HBUINT16	featureType;	/* The type of feature. */
  HBUINT16	featureSetting;	/* The feature's setting (aka selector). */
  HBUINT32	enableFlags;	/* Flags for the settings that this feature
				 * and setting enables. */
  HBUINT32	disableFlags;	/* Complement of flags for the settings that this
				 * feature and setting disable. */

  public:
  DEFINE_SIZE_STATIC (12);
};


struct hb_accelerate_subtables_context_t :
       hb_dispatch_context_t<hb_accelerate_subtables_context_t>
{
  struct hb_applicable_t
  {
    friend struct hb_accelerate_subtables_context_t;
    friend struct hb_aat_layout_lookup_accelerator_t;

    public:
    hb_bit_set_t glyph_set;
    mutable hb_aat_class_cache_t class_cache;

    template <typename T>
    auto init_ (const T &obj_, unsigned num_glyphs, hb_priority<1>) HB_AUTO_RETURN
    (
      obj_.machine.collect_initial_glyphs (glyph_set, num_glyphs, obj_)
    )

    template <typename T>
    void init_ (const T &obj_, unsigned num_glyphs, hb_priority<0>)
    {
      obj_.collect_initial_glyphs (glyph_set, num_glyphs);
    }

    template <typename T>
    void init (const T &obj_, unsigned num_glyphs)
    {
      glyph_set.init ();
      init_ (obj_, num_glyphs, hb_prioritize);
      class_cache.clear ();
    }

    void
    fini ()
    {
      glyph_set.fini ();
    }
  };

  /* Dispatch interface. */
  template <typename T>
  return_t dispatch (const T &obj)
  {
    hb_applicable_t *entry = &array[i++];

    entry->init (obj, num_glyphs);

    반환 hb_empty_t ();
  }
  static return_t default_return_value () { 반환 hb_empty_t (); }

  bool stop_sublookup_iteration (return_t r) const { 반환 false; }

  hb_accelerate_subtables_context_t (hb_applicable_t *array_, unsigned num_glyphs_) :
				     hb_dispatch_context_t<hb_accelerate_subtables_context_t> (),
				     array (array_), num_glyphs (num_glyphs_) {}

  hb_applicable_t *array;
  unsigned num_glyphs;
  unsigned i = 0;
};

struct hb_aat_layout_chain_accelerator_t
{
  template <typename TChain>
  static hb_aat_layout_chain_accelerator_t *create (const TChain &chain, unsigned num_glyphs)
  {
    unsigned count = chain.get_subtable_count ();

    unsigned product;
    만약 (unlikely (hb_unsigned_mul_overflows (count,
        sizeof (hb_accelerate_subtables_context_t::hb_applicable_t), &product)))
      반환 nullptr;

    unsigned size;
    만약 (unlikely (hb_unsigned_add_overflows (sizeof (hb_aat_layout_chain_accelerator_t) -
        HB_VAR_ARRAY * sizeof (hb_accelerate_subtables_context_t::hb_applicable_t), product, &size))) {
      반환 nullptr;
    }

    /* The following is a calloc because when we are collecting subtables,
     * some of them might be invalid and hence not collect; as a result,
     * we might not fill in all the count entries of the subtables array.
     * Zeroing it allows the set digest to gatekeep it without having to
     * initialize it further. */
    auto *thiz = (hb_aat_layout_chain_accelerator_t *) hb_calloc (1, size);
    만약 (unlikely (!thiz))
      반환 nullptr;

    thiz->count = count;

    hb_accelerate_subtables_context_t c_accelerate_subtables (thiz->subtables, num_glyphs);
    chain.dispatch (&c_accelerate_subtables);

    반환 thiz;
  }

  void destroy ()
  {
    위해 (unsigned i = 0; i < count; i++)
      subtables[i].fini ();
  }

  unsigned count;
  hb_accelerate_subtables_context_t::hb_applicable_t subtables[HB_VAR_ARRAY];
};

template <typename Types>
struct ChainSubtable
{
  typedef typename Types::HBUINT HBUINT;

  template <typename T>
  friend struct Chain;

  size_t get_size () const     { 반환 length; }
  unsigned 정수 get_type () const     { 반환 coverage & 0xFF; }
  unsigned 정수 get_coverage () const { 반환 coverage >> (sizeof (HBUINT) * 8 - 8); }

  enum Coverage
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
  enum Type
  {
    Rearrangement	= 0,
    Contextual		= 1,
    Ligature		= 2,
    Noncontextual	= 4,
    Insertion		= 5
  };

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) const
  {
    unsigned 정수 subtable_type = get_type ();
    TRACE_DISPATCH (this, subtable_type);
    switch (subtable_type) {
    case Rearrangement:		hb_barrier (); return_trace (c->dispatch (u.rearrangement, std::forward<Ts> (ds)...));
    case Contextual:		hb_barrier (); return_trace (c->dispatch (u.contextual, std::forward<Ts> (ds)...));
    case Ligature:		hb_barrier (); return_trace (c->dispatch (u.ligature, std::forward<Ts> (ds)...));
    case Noncontextual:		hb_barrier (); return_trace (c->dispatch (u.noncontextual, std::forward<Ts> (ds)...));
    case Insertion:		hb_barrier (); return_trace (c->dispatch (u.insertion, std::forward<Ts> (ds)...));
    default:			return_trace (c->default_return_value ());
    }
  }

  bool apply (hb_aat_apply_context_t *c) const
  {
    TRACE_APPLY (this);
    // Disabled for https://github.com/harfbuzz/harfbuzz/issues/4873
    //hb_sanitize_with_object_t with (&c->sanitizer, this);
    return_trace (dispatch (c));
  }

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    만약 (!(length.sanitize (c) &&
	  hb_barrier () &&
	  length >= min_size &&
	  c->check_range (this, length)))
      return_trace (false);

    // Disabled for https://github.com/harfbuzz/harfbuzz/issues/4873
    //hb_sanitize_with_object_t with (c, this);
    return_trace (dispatch (c));
  }

  protected:
  HBUINT	length;		/* Total subtable length, including this header. */
  HBUINT	coverage;	/* Coverage flags and subtable type. */
  HBUINT32	subFeatureFlags;/* The 32-bit mask identifying which subtable this is. */
  union {
  RearrangementSubtable<Types>	rearrangement;
  ContextualSubtable<Types>	contextual;
  LigatureSubtable<Types>	ligature;
  NoncontextualSubtable<Types>	noncontextual;
  InsertionSubtable<Types>	insertion;
  } u;
  public:
  DEFINE_SIZE_MIN (2 * sizeof (HBUINT) + 4);
};

template <typename Types>
struct Chain
{
  typedef typename Types::HBUINT HBUINT;

  unsigned get_subtable_count () const { 반환 subtableCount; }

  hb_mask_t compile_flags (const hb_aat_map_builder_t *map) const
  {
    hb_mask_t flags = defaultFlags;
    {
      unsigned 정수 count = featureCount;
      위해 (unsigned i = 0; i < count; i++)
      {
	const Feature &feature = featureZ[i];
	hb_aat_layout_feature_type_t type = (hb_aat_layout_feature_type_t) (unsigned 정수) feature.featureType;
	hb_aat_layout_feature_selector_t setting = (hb_aat_layout_feature_selector_t) (unsigned 정수) feature.featureSetting;
      retry:
	// Check whether this type/setting pair was requested in the map, and if so, apply its flags.
	// (The search here only looks at the type and setting fields of feature_info_t.)
	hb_aat_map_builder_t::feature_info_t info = { type, setting, false, 0 };
	만약 (map->current_features.bsearch (info))
	{
	  flags &= feature.disableFlags;
	  flags |= feature.enableFlags;
	}
	아니면 만약 (type == HB_AAT_LAYOUT_FEATURE_TYPE_LETTER_CASE && setting == HB_AAT_LAYOUT_FEATURE_SELECTOR_SMALL_CAPS)
	{
	  /* Deprecated. https://github.com/harfbuzz/harfbuzz/issues/1342 */
	  type = HB_AAT_LAYOUT_FEATURE_TYPE_LOWER_CASE;
	  setting = HB_AAT_LAYOUT_FEATURE_SELECTOR_LOWER_CASE_SMALL_CAPS;
	  goto retry;
	}
#ifndef HB_NO_AAT
	아니면 만약 (type == HB_AAT_LAYOUT_FEATURE_TYPE_LANGUAGE_TAG_TYPE && setting &&
		 /* TODO: Rudimentary language matching. */
		 hb_language_matches (map->face->table.ltag->get_language (setting - 1), map->props.language))
	{
	  flags &= feature.disableFlags;
	  flags |= feature.enableFlags;
	}
#endif
      }
    }
    반환 flags;
  }

  void apply (hb_aat_apply_context_t *c,
	      const hb_aat_layout_chain_accelerator_t *accel) const
  {
    const ChainSubtable<Types> *subtable = &StructAfter<ChainSubtable<Types>> (featureZ.as_array (featureCount));
    unsigned 정수 count = subtableCount;
    위해 (unsigned 정수 i = 0; i < count; i++)
    {
      bool reverse;

      auto coverage = subtable->get_coverage ();

      hb_mask_t subtable_flags = subtable->subFeatureFlags;
      만약 (hb_none (hb_iter (c->range_flags) |
		   hb_map ([subtable_flags] (const hb_aat_map_t::range_flags_t _) -> bool { 반환 subtable_flags & (_.flags); })))
	goto skip;

      만약 (!(coverage & ChainSubtable<Types>::AllDirections) &&
	  HB_DIRECTION_IS_VERTICAL (c->buffer->props.direction) !=
	  bool (coverage & ChainSubtable<Types>::Vertical))
	goto skip;

      c->subtable_flags = subtable_flags;
      c->first_set = accel ? &accel->subtables[i].glyph_set : &Null(hb_bit_set_t);
      c->machine_class_cache = accel ? &accel->subtables[i].class_cache : nullptr;

      만약 (!c->buffer_intersects_machine ())
      {
	(void) c->buffer->message (c->font, "skipped chainsubtable %u because no glyph matches", c->lookup_index);
	goto skip;
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
		HB_DIRECTION_IS_BACKWARD (c->buffer->props.direction);

      만약 (!c->buffer->message (c->font, "start chainsubtable %u", c->lookup_index))
	goto skip;

      만약 (reverse != c->buffer_is_reversed)
        c->reverse_buffer ();

      subtable->apply (c);

      (void) c->buffer->message (c->font, "end chainsubtable %u", c->lookup_index);

      만약 (unlikely (!c->buffer->successful)) break;

    skip:
      subtable = &StructAfter<ChainSubtable<Types>> (*subtable);
      c->set_lookup_index (c->lookup_index + 1);
    }
    만약 (c->buffer_is_reversed)
      c->reverse_buffer ();
  }

  size_t get_size () const { 반환 length; }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) const
  {
    const ChainSubtable<Types> *subtable = &StructAfter<ChainSubtable<Types>> (featureZ.as_array (featureCount));
    unsigned 정수 count = subtableCount;
    위해 (unsigned 정수 i = 0; i < count; i++)
    {
      typename context_t::return_t ret = subtable->dispatch (c, std::forward<Ts> (ds)...);
      만약 (c->stop_sublookup_iteration (ret))
	반환 ret;
      subtable = &StructAfter<ChainSubtable<Types>> (*subtable);
    }
    반환 c->default_return_value ();
  }

  bool sanitize (hb_sanitize_context_t *c, unsigned 정수 version) const
  {
    TRACE_SANITIZE (this);
    만약 (!(length.sanitize (c) &&
	  hb_barrier () &&
	  length >= min_size &&
	  c->check_range (this, length)))
      return_trace (false);

    만약 (!c->check_array (featureZ.arrayZ, featureCount))
      return_trace (false);

    const ChainSubtable<Types> *subtable = &StructAfter<ChainSubtable<Types>> (featureZ.as_array (featureCount));
    unsigned 정수 count = subtableCount;
    위해 (unsigned 정수 i = 0; i < count; i++)
    {
      만약 (!subtable->sanitize (c))
	return_trace (false);
      hb_barrier ();
      subtable = &StructAfter<ChainSubtable<Types>> (*subtable);
    }

    만약 (version >= 3)
    {
      const SubtableGlyphCoverage *coverage = (const SubtableGlyphCoverage *) subtable;
      만약 (!coverage->sanitize (c, count))
        return_trace (false);
    }

    return_trace (true);
  }

  protected:
  HBUINT32	defaultFlags;	/* The default specification for subtables. */
  HBUINT32	length;		/* Total byte count, including this header. */
  HBUINT	featureCount;	/* Number of feature subtable entries. */
  HBUINT	subtableCount;	/* The number of subtables in the chain. */

  UnsizedArrayOf<Feature>	featureZ;	/* Features. */
/*ChainSubtable	firstSubtable;*//* Subtables. */
/*SubtableGlyphCoverage coverages*//* Only if version >= 3. */

  public:
  DEFINE_SIZE_MIN (8 + 2 * sizeof (HBUINT));
};


/*
 * The 'mort'/'morx' Table
 */

template <typename T, typename Types, hb_tag_t TAG>
struct mortmorx
{
  static constexpr hb_tag_t tableTag = TAG;

  bool has_data () const { 반환 version != 0; }

  struct accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      hb_sanitize_context_t sc;
      this->table = sc.reference_table<T> (face);

      만약 (unlikely (this->table->is_blocklisted (this->table.get_blob (), face)))
      {
        hb_blob_destroy (this->table.get_blob ());
        this->table = hb_blob_get_empty ();
      }

      this->chain_count = table->get_chain_count ();

      this->accels = (hb_atomic_t<hb_aat_layout_chain_accelerator_t *> *) hb_calloc (this->chain_count, sizeof (*accels));
      만약 (unlikely (!this->accels))
      {
	this->chain_count = 0;
	this->table.destroy ();
	this->table = hb_blob_get_empty ();
      }
    }
    ~accelerator_t ()
    {
      위해 (unsigned 정수 i = 0; i < this->chain_count; i++)
      {
	만약 (this->accels[i])
	  this->accels[i]->destroy ();
	hb_free (this->accels[i]);
      }
      hb_free (this->accels);
      this->table.destroy ();
    }

    hb_blob_t *get_blob () const { 반환 table.get_blob (); }

    template <typename Chain>
    hb_aat_layout_chain_accelerator_t *get_accel (unsigned chain_index, const Chain &chain, unsigned num_glyphs) const
    {
      만약 (unlikely (chain_index >= chain_count)) 반환 nullptr;

    retry:
      auto *accel = accels[chain_index].get_acquire ();
      만약 (unlikely (!accel))
      {
	accel = hb_aat_layout_chain_accelerator_t::create (chain, num_glyphs);
	만약 (unlikely (!accel))
	  반환 nullptr;

	만약 (unlikely (!accels[chain_index].cmpexch (nullptr, accel)))
	{
	  hb_free (accel);
	  goto retry;
	}
      }

      반환 accel;
    }

    hb_blob_ptr_t<T> table;
    unsigned 정수 chain_count;
    hb_atomic_t<hb_aat_layout_chain_accelerator_t *> *accels;
    hb_aat_scratch_t scratch;
  };


  void compile_flags (const hb_aat_map_builder_t *mapper,
		      hb_aat_map_t *map) const
  {
    const Chain<Types> *chain = &firstChain;
    unsigned 정수 count = chainCount;
    만약 (unlikely (!map->chain_flags.resize (count)))
      반환;
    위해 (unsigned 정수 i = 0; i < count; i++)
    {
      map->chain_flags[i].push (hb_aat_map_t::range_flags_t {chain->compile_flags (mapper),
							     mapper->range_first,
							     mapper->range_last});
      chain = &StructAfter<Chain<Types>> (*chain);
    }
  }

  unsigned get_chain_count () const
  {
    반환 chainCount;
  }
  void apply (hb_aat_apply_context_t *c,
	      const hb_aat_map_t &map,
	      const accelerator_t &accel) const
  {
    만약 (unlikely (!c->buffer->successful)) 반환;

    c->buffer->unsafe_to_concat ();

    c->setup_buffer_glyph_set ();

    c->set_lookup_index (0);
    const Chain<Types> *chain = &firstChain;
    unsigned 정수 count = chainCount;
    위해 (unsigned 정수 i = 0; i < count; i++)
    {
      auto *chain_accel = accel.get_accel (i, *chain, c->face->get_num_glyphs ());
      c->range_flags = &map.chain_flags[i];
      chain->apply (c, chain_accel);
      만약 (unlikely (!c->buffer->successful)) 반환;
      chain = &StructAfter<Chain<Types>> (*chain);
    }
  }

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    만약 (!(version.sanitize (c) &&
	  hb_barrier () &&
	  version &&
	  chainCount.sanitize (c)))
      return_trace (false);

    const Chain<Types> *chain = &firstChain;
    unsigned 정수 count = chainCount;
    위해 (unsigned 정수 i = 0; i < count; i++)
    {
      만약 (!chain->sanitize (c, version))
	return_trace (false);
      hb_barrier ();
      chain = &StructAfter<Chain<Types>> (*chain);
    }

    return_trace (true);
  }

  protected:
  HBUINT16	version;	/* Version number of the glyph metamorphosis table.
				 * 1, 2, or 3. */
  HBUINT16	unused;		/* Set to 0. */
  HBUINT32	chainCount;	/* Number of metamorphosis chains contained in this
				 * table. */
  Chain<Types>	firstChain;	/* Chains. */

  public:
  DEFINE_SIZE_MIN (8);
};

struct morx : mortmorx<morx, ExtendedTypes, HB_AAT_TAG_morx>
{
  HB_INTERNAL bool is_blocklisted (hb_blob_t *blob,
                                   hb_face_t *face) const;
};

struct mort : mortmorx<mort, ObsoleteTypes, HB_AAT_TAG_mort>
{
  HB_INTERNAL bool is_blocklisted (hb_blob_t *blob,
                                   hb_face_t *face) const;
};

struct morx_accelerator_t : morx::accelerator_t {
  morx_accelerator_t (hb_face_t *face) : morx::accelerator_t (face) {}
};
struct mort_accelerator_t : mort::accelerator_t {
  mort_accelerator_t (hb_face_t *face) : mort::accelerator_t (face) {}
};


} /* namespace AAT */


#endif /* HB_AAT_LAYOUT_MORX_TABLE_HH */
