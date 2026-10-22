#ifndef OT_LAYOUT_GPOS_CURSIVEPOSFORMAT1_HH
#define OT_LAYOUT_GPOS_CURSIVEPOSFORMAT1_HH

#ضمن "Anchor.hh"
#ضمن "../../../hb-limits.hh"

نطاق OT {
نطاق Layout {
نطاق GPOS_impl {

template <typename Types>
بنية CursivePosFormat1_2;

template <typename Types>
بنية EntryExitRecord
{
  template <typename Types2> friend بنية CursivePosFormat1_2;

  منطقي sanitize (hb_sanitize_context_t *c, ثابت بنية CursivePosFormat1_2<Types> *base) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (entryAnchor.sanitize (c, base) && exitAnchor.sanitize (c, base));
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  ثابت بنية CursivePosFormat1_2<Types> *src_base) ثابت
  {
    (src_base+entryAnchor).collect_variation_indices (c);
    (src_base+exitAnchor).collect_variation_indices (c);
  }

  منطقي subset (hb_subset_context_t *c,
	       ثابت بنية CursivePosFormat1_2<Types> *src_base) ثابت
  {
    TRACE_SERIALIZE (هذا);
    auto *out = c->serializer->embed (هذا);
    𐡄𐡍 (unlikely (!out)) return_trace (خطأ);

    منطقي ret = خطأ;
    ret |= out->entryAnchor.serialize_subset (c, entryAnchor, src_base);
    ret |= out->exitAnchor.serialize_subset (c, exitAnchor, src_base);
    return_trace (ret);
  }

  محمي:
  typename Types::template OffsetTo<Anchor, بنية CursivePosFormat1_2<Types>>
                entryAnchor;            /* Offset to EntryAnchor table--from
                                         * beginning of CursivePos
                                         * subtable--may be NULL */
  typename Types::template OffsetTo<Anchor, بنية CursivePosFormat1_2<Types>>
                exitAnchor;             /* Offset to ExitAnchor table--from
                                         * beginning of CursivePos
                                         * subtable--may be NULL */
  عام:
  DEFINE_SIZE_STATIC (2 * Types::size);
};

ساكن مضمن فراغ
reverse_cursive_minor_offset (hb_glyph_position_t *pos,
                              unsigned صحيح len,
                              unsigned صحيح i,
                              hb_direction_t direction,
                              unsigned صحيح new_parent,
                              unsigned nesting_level = 0)
{
  𐡄𐡍 (nesting_level > HB_MAX_NESTING_LEVEL)
    أرجع;

  صحيح chain = pos[i].attach_chain(), type = pos[i].attach_type();
  𐡄𐡍 (likely (!chain || 0 == (type & ATTACH_TYPE_CURSIVE)))
    أرجع;

  pos[i].attach_chain() = 0;

  unsigned صحيح j = (صحيح) i + chain;

  𐡄𐡍 (unlikely (j >= len))
    أرجع;

  /* Stop if we see new parent in the chain. */
  𐡄𐡍 (j == new_parent)
    أرجع;

  int16_t reversed_chain = -chain;
  /* The old edge was cleared above; if the reversed distance truncates,
   * keep it detached instead of storing a poisoned chain.
   */
  𐡄𐡍 (unlikely (reversed_chain != -chain))
    أرجع;

  reverse_cursive_minor_offset (pos, len, j, direction, new_parent, nesting_level + 1);

  𐡄𐡍 (HB_DIRECTION_IS_HORIZONTAL (direction))
    pos[j].y_offset = hb_saturate_neg (pos[i].y_offset);
  وإلا
    pos[j].x_offset = hb_saturate_neg (pos[i].x_offset);

  pos[j].attach_chain() = reversed_chain;
  pos[j].attach_type() = type;
}


ساكن مضمن منطقي
apply_cursive_attachment (hb_ot_apply_context_t *c,
                          ثابت Anchor &exit_anchor,
                          ثابت Anchor &entry_anchor,
                          unsigned i)
{
  hb_buffer_t *buffer = c->buffer;
  unsigned صحيح j = buffer->idx;

  𐡄𐡍 (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
  {
    c->buffer->message (c->font,
			  "cursive attaching glyph at %u to glyph at %u",
			  i, j);
  }

  buffer->unsafe_to_break (i, j + 1);
  عائم entry_x, entry_y, exit_x, exit_y;
  exit_anchor.get_anchor (c, buffer->info[i].codepoint, &exit_x, &exit_y);
  entry_anchor.get_anchor (c, buffer->info[j].codepoint, &entry_x, &entry_y);

  hb_glyph_position_t *pos = buffer->pos;

  hb_position_t d;
  /* Main-direction adjustment */
  بدل (c->direction) {
    حالة HB_DIRECTION_LTR:
      pos[i].x_advance = hb_saturate_add ((hb_position_t) roundf (exit_x), pos[i].x_offset);

      d = hb_saturate_add ((hb_position_t) roundf (entry_x), pos[j].x_offset);
      pos[j].x_advance = hb_saturate_sub (pos[j].x_advance, d);
      pos[j].x_offset = hb_saturate_sub (pos[j].x_offset, d);
      اكسر;
    حالة HB_DIRECTION_RTL:
      d = hb_saturate_add ((hb_position_t) roundf (exit_x), pos[i].x_offset);
      pos[i].x_advance = hb_saturate_sub (pos[i].x_advance, d);
      pos[i].x_offset = hb_saturate_sub (pos[i].x_offset, d);

      pos[j].x_advance = hb_saturate_add ((hb_position_t) roundf (entry_x), pos[j].x_offset);
      اكسر;
    حالة HB_DIRECTION_TTB:
      pos[i].y_advance = hb_saturate_add ((hb_position_t) roundf (exit_y), pos[i].y_offset);

      d = hb_saturate_add ((hb_position_t) roundf (entry_y), pos[j].y_offset);
      pos[j].y_advance = hb_saturate_sub (pos[j].y_advance, d);
      pos[j].y_offset = hb_saturate_sub (pos[j].y_offset, d);
      اكسر;
    حالة HB_DIRECTION_BTT:
      d = hb_saturate_add ((hb_position_t) roundf (exit_y), pos[i].y_offset);
      pos[i].y_advance = hb_saturate_sub (pos[i].y_advance, d);
      pos[i].y_offset = hb_saturate_sub (pos[i].y_offset, d);

      pos[j].y_advance = (hb_position_t) roundf (entry_y);
      اكسر;
    حالة HB_DIRECTION_INVALID:
    افتراضي:
      اكسر;
  }

  /* Cross-direction adjustment */

  /* We attach child to parent (think graph theory and rooted trees whereas
   * the root stays on baseline and each node aligns itself against its
   * parent.
   *
   * Optimize things for the case of RightToLeft, as that's most common in
   * Arabic. */
  unsigned صحيح child  = i;
  unsigned صحيح parent = j;
  hb_position_t x_offset = (hb_position_t) roundf (entry_x - exit_x);
  hb_position_t y_offset = (hb_position_t) roundf (entry_y - exit_y);
  𐡄𐡍  (!(c->lookup_props & LookupFlag::RightToLeft))
  {
    unsigned صحيح k = child;
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

  pos[child].attach_chain() = (صحيح) parent - (صحيح) child;
  𐡄𐡍 (pos[child].attach_chain() != (صحيح) parent - (صحيح) child)
  {
    pos[child].attach_chain() = 0;
    اذهب overflow;
  }
  pos[child].attach_type() = ATTACH_TYPE_CURSIVE;
  buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT;
  𐡄𐡍 (likely (HB_DIRECTION_IS_HORIZONTAL (c->direction)))
    pos[child].y_offset = y_offset;
  وإلا
    pos[child].x_offset = x_offset;

  /* If parent was attached to child, separate them.
   * https://github.com/harfbuzz/harfbuzz/issues/2469
   */
  𐡄𐡍 (unlikely (pos[parent].attach_chain() == -pos[child].attach_chain()))
  {
    pos[parent].attach_chain() = 0;
    𐡄𐡍 (likely (HB_DIRECTION_IS_HORIZONTAL (c->direction)))
	pos[parent].y_offset = 0;
    وإلا
	pos[parent].x_offset = 0;
  }

  𐡄𐡍 (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
  {
    c->buffer->message (c->font,
			  "cursive attached glyph at %u to glyph at %u",
			  i, j);
  }

overflow:
  buffer->idx++;
  أرجع 𐡒𐡔𐡈;
}

template <typename Types>
بنية CursivePosFormat1_2
{

  محمي:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  typename Types::template LOffsetTo<Coverage>
                coverage;               /* Offset to Coverage table--from beginning of subtable */
  typename Types::template ArrayOf<EntryExitRecord<Types>>
                entryExitRecord;        /* Array of EntryExit records--in
                                         * Coverage Index order */
  عام:
  DEFINE_SIZE_ARRAY (2 + Types::LOffset::static_size + Types::HBUINT::static_size, entryExitRecord);

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    𐡄𐡍 (unlikely (!coverage.sanitize (c, هذا)))
      return_trace (خطأ);

    𐡄𐡍 (c->lazy_some_gpos)
      return_trace (entryExitRecord.sanitize_shallow (c));
    وإلا
      return_trace (entryExitRecord.sanitize (c, هذا));
  }

  منطقي intersects (ثابت hb_set_t *glyphs) ثابت
  { أرجع (هذا+coverage).intersects (glyphs); }

  فراغ closure_lookups (hb_closure_lookups_context_t *c) ثابت {}

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    + hb_zip (هذا+coverage, entryExitRecord)
    | hb_filter (c->glyph_set, hb_first)
    | hb_map (hb_second)
    | hb_apply ([&] (ثابت EntryExitRecord<Types>& record) { record.collect_variation_indices (c, هذا); })
    ;
  }

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  { 𐡄𐡍 (unlikely (!(هذا+coverage).collect_coverage (c->input))) أرجع; }

  فراغ collect_second_glyphs (hb_set_digest_t *digest) ثابت
  { (هذا+coverage).collect_coverage (digest); }

  ثابت Coverage &get_coverage () ثابت { أرجع هذا+coverage; }

  منطقي apply (hb_ot_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);
    hb_buffer_t *buffer = c->buffer;

    ثابت EntryExitRecord<Types> &this_record = entryExitRecord[(هذا+coverage).get_coverage  (buffer->cur().codepoint)];
    𐡄𐡍 (!this_record.entryAnchor ||
	unlikely (!this_record.entryAnchor.sanitize (&c->sanitizer, هذا))) return_trace (خطأ);
    hb_barrier ();

    auto &skippy_iter = c->iter_input;
    skippy_iter.reset_fast (buffer->idx);
    unsigned unsafe_from;
    𐡄𐡍 (unlikely (!skippy_iter.prev (&unsafe_from)))
    {
      buffer->unsafe_to_concat_from_outbuffer (unsafe_from, buffer->idx + 1);
      return_trace (خطأ);
    }

    ثابت EntryExitRecord<Types> &prev_record = entryExitRecord[(هذا+coverage).get_coverage  (buffer->info[skippy_iter.idx].codepoint)];
    𐡄𐡍 (!prev_record.exitAnchor ||
	unlikely (!prev_record.exitAnchor.sanitize (&c->sanitizer, هذا)))
    {
      buffer->unsafe_to_concat_from_outbuffer (skippy_iter.idx, buffer->idx + 1);
      return_trace (خطأ);
    }
    hb_barrier ();

    return_trace (apply_cursive_attachment (c,
                                            هذا+prev_record.exitAnchor,
                                            هذا+this_record.entryAnchor,
                                            skippy_iter.idx));
  }

  template <typename Iterator,
            hb_requires (hb_is_iterator (Iterator))>
  فراغ serialize (hb_subset_context_t *c,
                  Iterator it,
                  ثابت بنية CursivePosFormat1_2<Types> *src_base)
  {
    𐡄𐡍 (unlikely (!c->serializer->extend_min ((*هذا)))) أرجع;
    هذا->format = src_base->format;
    هذا->entryExitRecord.len = it.len ();

    لكل (ثابت EntryExitRecord<Types>& entry_record : + it
                                                      | hb_map (hb_second))
      entry_record.subset (c, src_base);

    auto glyphs =
    + it
    | hb_map_retains_sorting (hb_first)
    ;

    coverage.serialize_serialize (c->serializer, glyphs);
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*هذا);

    auto it =
    + hb_zip (هذا+coverage, entryExitRecord)
    | hb_filter (glyphset, hb_first)
    | hb_map_retains_sorting ([&] (hb_pair_t<hb_codepoint_t, ثابت EntryExitRecord<Types>&> p)
                              -> hb_pair_t<hb_codepoint_t, ثابت EntryExitRecord<Types>&>
                              { أرجع hb_pair (glyph_map[p.first], p.second);})
    ;

    منطقي ret = منطقي (it);
    out->serialize (c, it, هذا);
    return_trace (ret);
  }
};


}
}
}

#endif /* OT_LAYOUT_GPOS_CURSIVEPOSFORMAT1_HH */
