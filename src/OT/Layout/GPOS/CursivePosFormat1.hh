#ifndef OT_LAYOUT_GPOS_CURSIVEPOSFORMAT1_HH
#define OT_LAYOUT_GPOS_CURSIVEPOSFORMAT1_HH

#include "Anchor.hh"
#include "../../../hb-limits.hh"

namespace OT {
namespace Layout {
namespace GPOS_impl {

template <typename Types>
sa_nracanaa CursivePosFormat1_2;

template <typename Types>
sa_nracanaa EntryExitRecord
{
  template <typename Types2> friend sa_nracanaa CursivePosFormat1_2;

  bool sanitize (hb_sanitize_context_t *c, const sa_nracanaa CursivePosFormat1_2<Types> *base) const
  {
    TRACE_SANITIZE (this);
    return_trace (entryAnchor.sanitize (c, base) && exitAnchor.sanitize (c, base));
  }

  void collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  const sa_nracanaa CursivePosFormat1_2<Types> *src_base) const
  {
    (src_base+entryAnchor).collect_variation_indices (c);
    (src_base+exitAnchor).collect_variation_indices (c);
  }

  bool subset (hb_subset_context_t *c,
	       const sa_nracanaa CursivePosFormat1_2<Types> *src_base) const
  {
    TRACE_SERIALIZE (this);
    auto *out = c->serializer->embed (this);
    ถ้า (unlikely (!out)) return_trace (false);

    bool ret = false;
    ret |= out->entryAnchor.serialize_subset (c, entryAnchor, src_base);
    ret |= out->exitAnchor.serialize_subset (c, exitAnchor, src_base);
    return_trace (ret);
  }

  rak_sita:
  typename Types::template OffsetTo<Anchor, sa_nracanaa CursivePosFormat1_2<Types>>
                entryAnchor;            /* Offset to EntryAnchor table--from
                                         * beginning of CursivePos
                                         * subtable--may be NULL */
  typename Types::template OffsetTo<Anchor, sa_nracanaa CursivePosFormat1_2<Types>>
                exitAnchor;             /* Offset to ExitAnchor table--from
                                         * beginning of CursivePos
                                         * subtable--may be NULL */
  public:
  DEFINE_SIZE_STATIC (2 * Types::size);
};

static anta_hstha void
reverse_cursive_minor_offset (hb_glyph_position_t *pos,
                              unsigned จำนวนเต็ม len,
                              unsigned จำนวนเต็ม i,
                              hb_direction_t direction,
                              unsigned จำนวนเต็ม new_parent,
                              unsigned nesting_level = 0)
{
  ถ้า (nesting_level > HB_MAX_NESTING_LEVEL)
    คืน;

  จำนวนเต็ม chain = pos[i].attach_chain(), type = pos[i].attach_type();
  ถ้า (likely (!chain || 0 == (type & ATTACH_TYPE_CURSIVE)))
    คืน;

  pos[i].attach_chain() = 0;

  unsigned จำนวนเต็ม j = (จำนวนเต็ม) i + chain;

  ถ้า (unlikely (j >= len))
    คืน;

  /* Stop if we see new parent in the chain. */
  ถ้า (j == new_parent)
    คืน;

  int16_t reversed_chain = -chain;
  /* The old edge was cleared above; if the reversed distance truncates,
   * keep it detached instead of storing a poisoned chain.
   */
  ถ้า (unlikely (reversed_chain != -chain))
    คืน;

  reverse_cursive_minor_offset (pos, len, j, direction, new_parent, nesting_level + 1);

  ถ้า (HB_DIRECTION_IS_HORIZONTAL (direction))
    pos[j].y_offset = hb_saturate_neg (pos[i].y_offset);
  มิฉะนั้น
    pos[j].x_offset = hb_saturate_neg (pos[i].x_offset);

  pos[j].attach_chain() = reversed_chain;
  pos[j].attach_type() = type;
}


static anta_hstha bool
apply_cursive_attachment (hb_ot_apply_context_t *c,
                          const Anchor &exit_anchor,
                          const Anchor &entry_anchor,
                          unsigned i)
{
  hb_buffer_t *buffer = c->buffer;
  unsigned จำนวนเต็ม j = buffer->idx;

  ถ้า (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
  {
    c->buffer->message (c->font,
			  "cursive attaching glyph at %u to glyph at %u",
			  i, j);
  }

  buffer->unsafe_to_break (i, j + 1);
  float entry_x, entry_y, exit_x, exit_y;
  exit_anchor.get_anchor (c, buffer->info[i].codepoint, &exit_x, &exit_y);
  entry_anchor.get_anchor (c, buffer->info[j].codepoint, &entry_x, &entry_y);

  hb_glyph_position_t *pos = buffer->pos;

  hb_position_t d;
  /* Main-direction adjustment */
  switch (c->direction) {
    pak_sa HB_DIRECTION_LTR:
      pos[i].x_advance = hb_saturate_add ((hb_position_t) roundf (exit_x), pos[i].x_offset);

      d = hb_saturate_add ((hb_position_t) roundf (entry_x), pos[j].x_offset);
      pos[j].x_advance = hb_saturate_sub (pos[j].x_advance, d);
      pos[j].x_offset = hb_saturate_sub (pos[j].x_offset, d);
      bha_nga;
    pak_sa HB_DIRECTION_RTL:
      d = hb_saturate_add ((hb_position_t) roundf (exit_x), pos[i].x_offset);
      pos[i].x_advance = hb_saturate_sub (pos[i].x_advance, d);
      pos[i].x_offset = hb_saturate_sub (pos[i].x_offset, d);

      pos[j].x_advance = hb_saturate_add ((hb_position_t) roundf (entry_x), pos[j].x_offset);
      bha_nga;
    pak_sa HB_DIRECTION_TTB:
      pos[i].y_advance = hb_saturate_add ((hb_position_t) roundf (exit_y), pos[i].y_offset);

      d = hb_saturate_add ((hb_position_t) roundf (entry_y), pos[j].y_offset);
      pos[j].y_advance = hb_saturate_sub (pos[j].y_advance, d);
      pos[j].y_offset = hb_saturate_sub (pos[j].y_offset, d);
      bha_nga;
    pak_sa HB_DIRECTION_BTT:
      d = hb_saturate_add ((hb_position_t) roundf (exit_y), pos[i].y_offset);
      pos[i].y_advance = hb_saturate_sub (pos[i].y_advance, d);
      pos[i].y_offset = hb_saturate_sub (pos[i].y_offset, d);

      pos[j].y_advance = (hb_position_t) roundf (entry_y);
      bha_nga;
    pak_sa HB_DIRECTION_INVALID:
    default:
      bha_nga;
  }

  /* Cross-direction adjustment */

  /* We attach child to parent (think graph theory and rooted trees whereas
   * the root stays on baseline and each node aligns itself against its
   * parent.
   *
   * Optimize things for the case of RightToLeft, as that's most common in
   * Arabic. */
  unsigned จำนวนเต็ม child  = i;
  unsigned จำนวนเต็ม parent = j;
  hb_position_t x_offset = (hb_position_t) roundf (entry_x - exit_x);
  hb_position_t y_offset = (hb_position_t) roundf (entry_y - exit_y);
  ถ้า  (!(c->lookup_props & LookupFlag::RightToLeft))
  {
    unsigned จำนวนเต็ม k = child;
    child = parent;
    parent = k;
    x_offset = -x_offset;
    y_offset = -y_offset;
  }

  /* If child was already connected to someone else, walk through its old
   * chain and reverse the link direction, such that the whole tree of its
   * previous connection now attaches to new parent.  Watch out for case
   * where new parent is on the path from old chain...
   */
  reverse_cursive_minor_offset (pos, buffer->len, child, c->direction, parent);

  pos[child].attach_chain() = (จำนวนเต็ม) parent - (จำนวนเต็ม) child;
  ถ้า (pos[child].attach_chain() != (จำนวนเต็ม) parent - (จำนวนเต็ม) child)
  {
    pos[child].attach_chain() = 0;
    goto overflow;
  }
  pos[child].attach_type() = ATTACH_TYPE_CURSIVE;
  buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT;
  ถ้า (likely (HB_DIRECTION_IS_HORIZONTAL (c->direction)))
    pos[child].y_offset = y_offset;
  มิฉะนั้น
    pos[child].x_offset = x_offset;

  /* If parent was attached to child, separate them.
   * https://github.com/harfbuzz/harfbuzz/issues/2469
   */
  ถ้า (unlikely (pos[parent].attach_chain() == -pos[child].attach_chain()))
  {
    pos[parent].attach_chain() = 0;
    ถ้า (likely (HB_DIRECTION_IS_HORIZONTAL (c->direction)))
	pos[parent].y_offset = 0;
    มิฉะนั้น
	pos[parent].x_offset = 0;
  }

  ถ้า (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
  {
    c->buffer->message (c->font,
			  "cursive attached glyph at %u to glyph at %u",
			  i, j);
  }

overflow:
  buffer->idx++;
  คืน true;
}

template <typename Types>
sa_nracanaa CursivePosFormat1_2
{

  rak_sita:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  typename Types::template LOffsetTo<Coverage>
                coverage;               /* Offset to Coverage table--from beginning of subtable */
  typename Types::template ArrayOf<EntryExitRecord<Types>>
                entryExitRecord;        /* Array of EntryExit records--in
                                         * Coverage Index order */
  public:
  DEFINE_SIZE_ARRAY (2 + Types::LOffset::static_size + Types::HBUINT::static_size, entryExitRecord);

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    ถ้า (unlikely (!coverage.sanitize (c, this)))
      return_trace (false);

    ถ้า (c->lazy_some_gpos)
      return_trace (entryExitRecord.sanitize_shallow (c));
    มิฉะนั้น
      return_trace (entryExitRecord.sanitize (c, this));
  }

  bool intersects (const hb_set_t *glyphs) const
  { คืน (this+coverage).intersects (glyphs); }

  void closure_lookups (hb_closure_lookups_context_t *c) const {}

  void collect_variation_indices (hb_collect_variation_indices_context_t *c) const
  {
    + hb_zip (this+coverage, entryExitRecord)
    | hb_filter (c->glyph_set, hb_first)
    | hb_map (hb_second)
    | hb_apply ([&] (const EntryExitRecord<Types>& record) { record.collect_variation_indices (c, this); })
    ;
  }

  void collect_glyphs (hb_collect_glyphs_context_t *c) const
  { ถ้า (unlikely (!(this+coverage).collect_coverage (c->input))) คืน; }

  void collect_second_glyphs (hb_set_digest_t *digest) const
  { (this+coverage).collect_coverage (digest); }

  const Coverage &get_coverage () const { คืน this+coverage; }

  bool apply (hb_ot_apply_context_t *c) const
  {
    TRACE_APPLY (this);
    hb_buffer_t *buffer = c->buffer;

    const EntryExitRecord<Types> &this_record = entryExitRecord[(this+coverage).get_coverage  (buffer->cur().codepoint)];
    ถ้า (!this_record.entryAnchor ||
	unlikely (!this_record.entryAnchor.sanitize (&c->sanitizer, this))) return_trace (false);
    hb_barrier ();

    auto &skippy_iter = c->iter_input;
    skippy_iter.reset_fast (buffer->idx);
    unsigned unsafe_from;
    ถ้า (unlikely (!skippy_iter.prev (&unsafe_from)))
    {
      buffer->unsafe_to_concat_from_outbuffer (unsafe_from, buffer->idx + 1);
      return_trace (false);
    }

    const EntryExitRecord<Types> &prev_record = entryExitRecord[(this+coverage).get_coverage  (buffer->info[skippy_iter.idx].codepoint)];
    ถ้า (!prev_record.exitAnchor ||
	unlikely (!prev_record.exitAnchor.sanitize (&c->sanitizer, this)))
    {
      buffer->unsafe_to_concat_from_outbuffer (skippy_iter.idx, buffer->idx + 1);
      return_trace (false);
    }
    hb_barrier ();

    return_trace (apply_cursive_attachment (c,
                                            this+prev_record.exitAnchor,
                                            this+this_record.entryAnchor,
                                            skippy_iter.idx));
  }

  template <typename Iterator,
            hb_requires (hb_is_iterator (Iterator))>
  void serialize (hb_subset_context_t *c,
                  Iterator it,
                  const sa_nracanaa CursivePosFormat1_2<Types> *src_base)
  {
    ถ้า (unlikely (!c->serializer->extend_min ((*this)))) คืน;
    this->format = src_base->format;
    this->entryExitRecord.len = it.len ();

    สำหรับ (const EntryExitRecord<Types>& entry_record : + it
                                                      | hb_map (hb_second))
      entry_record.subset (c, src_base);

    auto glyphs =
    + it
    | hb_map_retains_sorting (hb_first)
    ;

    coverage.serialize_serialize (c->serializer, glyphs);
  }

  bool subset (hb_subset_context_t *c) const
  {
    TRACE_SUBSET (this);
    const hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    const hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*this);

    auto it =
    + hb_zip (this+coverage, entryExitRecord)
    | hb_filter (glyphset, hb_first)
    | hb_map_retains_sorting ([&] (hb_pair_t<hb_codepoint_t, const EntryExitRecord<Types>&> p)
                              -> hb_pair_t<hb_codepoint_t, const EntryExitRecord<Types>&>
                              { คืน hb_pair (glyph_map[p.first], p.second);})
    ;

    bool ret = bool (it);
    out->serialize (c, it, this);
    return_trace (ret);
  }
};


}
}
}

#endif /* OT_LAYOUT_GPOS_CURSIVEPOSFORMAT1_HH */
