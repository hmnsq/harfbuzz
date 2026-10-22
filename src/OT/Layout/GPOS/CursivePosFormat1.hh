#ifndef OT_LAYOUT_GPOS_CURSIVEPOSFORMAT1_HH
#define OT_LAYOUT_GPOS_CURSIVEPOSFORMAT1_HH

#অন্তর্ভাব "Anchor.hh"
#অন্তর্ভাব "../../../hb-limits.hh"

নামাকাশ OT {
নামাকাশ Layout {
নামাকাশ GPOS_impl {

template <typename Types>
সংরচনা CursivePosFormat1_2;

template <typename Types>
সংরচনা EntryExitRecord
{
  template <typename Types2> friend সংরচনা CursivePosFormat1_2;

  তার্কিক sanitize (hb_sanitize_context_t *c, ধ্রুব সংরচনা CursivePosFormat1_2<Types> *base) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    return_trace (entryAnchor.sanitize (c, base) && exitAnchor.sanitize (c, base));
  }

  শূন্য collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  ধ্রুব সংরচনা CursivePosFormat1_2<Types> *src_base) ধ্রুব
  {
    (src_base+entryAnchor).collect_variation_indices (c);
    (src_base+exitAnchor).collect_variation_indices (c);
  }

  তার্কিক subset (hb_subset_context_t *c,
	       ধ্রুব সংরচনা CursivePosFormat1_2<Types> *src_base) ধ্রুব
  {
    TRACE_SERIALIZE (এতত্);
    auto *out = c->serializer->embed (এতত্);
    যদি (unlikely (!out)) return_trace (অসত্য);

    তার্কিক ret = অসত্য;
    ret |= out->entryAnchor.serialize_subset (c, entryAnchor, src_base);
    ret |= out->exitAnchor.serialize_subset (c, exitAnchor, src_base);
    return_trace (ret);
  }

  রক্ষিত:
  typename Types::template OffsetTo<Anchor, সংরচনা CursivePosFormat1_2<Types>>
                entryAnchor;            /* Offset to EntryAnchor table--from
                                         * beginning of CursivePos
                                         * subtable--may be NULL */
  typename Types::template OffsetTo<Anchor, সংরচনা CursivePosFormat1_2<Types>>
                exitAnchor;             /* Offset to ExitAnchor table--from
                                         * beginning of CursivePos
                                         * subtable--may be NULL */
  সার্বজনিক:
  DEFINE_SIZE_STATIC (2 * Types::size);
};

স্থির অন্তঃস্থ শূন্য
reverse_cursive_minor_offset (hb_glyph_position_t *pos,
                              unsigned পূর্ণাঙ্ক len,
                              unsigned পূর্ণাঙ্ক i,
                              hb_direction_t direction,
                              unsigned পূর্ণাঙ্ক new_parent,
                              unsigned nesting_level = 0)
{
  যদি (nesting_level > HB_MAX_NESTING_LEVEL)
    প্রত্যাগম;

  পূর্ণাঙ্ক chain = pos[i].attach_chain(), type = pos[i].attach_type();
  যদি (likely (!chain || 0 == (type & ATTACH_TYPE_CURSIVE)))
    প্রত্যাগম;

  pos[i].attach_chain() = 0;

  unsigned পূর্ণাঙ্ক j = (পূর্ণাঙ্ক) i + chain;

  যদি (unlikely (j >= len))
    প্রত্যাগম;

  /* Stop if we see new parent in the chain. */
  যদি (j == new_parent)
    প্রত্যাগম;

  int16_t reversed_chain = -chain;
  /* The old edge was cleared above; if the reversed distance truncates,
   * keep it detached instead of storing a poisoned chain.
   */
  যদি (unlikely (reversed_chain != -chain))
    প্রত্যাগম;

  reverse_cursive_minor_offset (pos, len, j, direction, new_parent, nesting_level + 1);

  যদি (HB_DIRECTION_IS_HORIZONTAL (direction))
    pos[j].y_offset = hb_saturate_neg (pos[i].y_offset);
  অন্যথা
    pos[j].x_offset = hb_saturate_neg (pos[i].x_offset);

  pos[j].attach_chain() = reversed_chain;
  pos[j].attach_type() = type;
}


স্থির অন্তঃস্থ তার্কিক
apply_cursive_attachment (hb_ot_apply_context_t *c,
                          ধ্রুব Anchor &exit_anchor,
                          ধ্রুব Anchor &entry_anchor,
                          unsigned i)
{
  hb_buffer_t *buffer = c->buffer;
  unsigned পূর্ণাঙ্ক j = buffer->idx;

  যদি (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
  {
    c->buffer->message (c->font,
			  "cursive attaching glyph at %u to glyph at %u",
			  i, j);
  }

  buffer->unsafe_to_break (i, j + 1);
  ভিন্ন entry_x, entry_y, exit_x, exit_y;
  exit_anchor.get_anchor (c, buffer->info[i].codepoint, &exit_x, &exit_y);
  entry_anchor.get_anchor (c, buffer->info[j].codepoint, &entry_x, &entry_y);

  hb_glyph_position_t *pos = buffer->pos;

  hb_position_t d;
  /* Main-direction adjustment */
  বিকল্প (c->direction) {
    পক্ষ HB_DIRECTION_LTR:
      pos[i].x_advance = hb_saturate_add ((hb_position_t) roundf (exit_x), pos[i].x_offset);

      d = hb_saturate_add ((hb_position_t) roundf (entry_x), pos[j].x_offset);
      pos[j].x_advance = hb_saturate_sub (pos[j].x_advance, d);
      pos[j].x_offset = hb_saturate_sub (pos[j].x_offset, d);
      ভঙ্গ;
    পক্ষ HB_DIRECTION_RTL:
      d = hb_saturate_add ((hb_position_t) roundf (exit_x), pos[i].x_offset);
      pos[i].x_advance = hb_saturate_sub (pos[i].x_advance, d);
      pos[i].x_offset = hb_saturate_sub (pos[i].x_offset, d);

      pos[j].x_advance = hb_saturate_add ((hb_position_t) roundf (entry_x), pos[j].x_offset);
      ভঙ্গ;
    পক্ষ HB_DIRECTION_TTB:
      pos[i].y_advance = hb_saturate_add ((hb_position_t) roundf (exit_y), pos[i].y_offset);

      d = hb_saturate_add ((hb_position_t) roundf (entry_y), pos[j].y_offset);
      pos[j].y_advance = hb_saturate_sub (pos[j].y_advance, d);
      pos[j].y_offset = hb_saturate_sub (pos[j].y_offset, d);
      ভঙ্গ;
    পক্ষ HB_DIRECTION_BTT:
      d = hb_saturate_add ((hb_position_t) roundf (exit_y), pos[i].y_offset);
      pos[i].y_advance = hb_saturate_sub (pos[i].y_advance, d);
      pos[i].y_offset = hb_saturate_sub (pos[i].y_offset, d);

      pos[j].y_advance = (hb_position_t) roundf (entry_y);
      ভঙ্গ;
    পক্ষ HB_DIRECTION_INVALID:
    স্বভাব:
      ভঙ্গ;
  }

  /* Cross-direction adjustment */

  /* We attach child to parent (think graph theory and rooted trees whereas
   * the root stays on baseline and each node aligns itself against its
   * parent.
   *
   * Optimize things for the case of RightToLeft, as that's most common in
   * Arabic. */
  unsigned পূর্ণাঙ্ক child  = i;
  unsigned পূর্ণাঙ্ক parent = j;
  hb_position_t x_offset = (hb_position_t) roundf (entry_x - exit_x);
  hb_position_t y_offset = (hb_position_t) roundf (entry_y - exit_y);
  যদি  (!(c->lookup_props & LookupFlag::ডান_To_বাম))
  {
    unsigned পূর্ণাঙ্ক k = child;
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

  pos[child].attach_chain() = (পূর্ণাঙ্ক) parent - (পূর্ণাঙ্ক) child;
  যদি (pos[child].attach_chain() != (পূর্ণাঙ্ক) parent - (পূর্ণাঙ্ক) child)
  {
    pos[child].attach_chain() = 0;
    গচ্ছ overflow;
  }
  pos[child].attach_type() = ATTACH_TYPE_CURSIVE;
  buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT;
  যদি (likely (HB_DIRECTION_IS_HORIZONTAL (c->direction)))
    pos[child].y_offset = y_offset;
  অন্যথা
    pos[child].x_offset = x_offset;

  /* If parent was attached to child, separate them.
   * https://github.com/harfbuzz/harfbuzz/issues/2469
   */
  যদি (unlikely (pos[parent].attach_chain() == -pos[child].attach_chain()))
  {
    pos[parent].attach_chain() = 0;
    যদি (likely (HB_DIRECTION_IS_HORIZONTAL (c->direction)))
	pos[parent].y_offset = 0;
    অন্যথা
	pos[parent].x_offset = 0;
  }

  যদি (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
  {
    c->buffer->message (c->font,
			  "cursive attached glyph at %u to glyph at %u",
			  i, j);
  }

overflow:
  buffer->idx++;
  প্রত্যাগম সত্য;
}

template <typename Types>
সংরচনা CursivePosFormat1_2
{

  রক্ষিত:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  typename Types::template LOffsetTo<Coverage>
                coverage;               /* Offset to Coverage table--from beginning of subtable */
  typename Types::template ArrayOf<EntryExitRecord<Types>>
                entryExitRecord;        /* Array of EntryExit records--in
                                         * Coverage Index order */
  সার্বজনিক:
  DEFINE_SIZE_ARRAY (2 + Types::LOffset::static_size + Types::HBUINT::static_size, entryExitRecord);

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    যদি (unlikely (!coverage.sanitize (c, এতত্)))
      return_trace (অসত্য);

    যদি (c->lazy_some_gpos)
      return_trace (entryExitRecord.sanitize_shallow (c));
    অন্যথা
      return_trace (entryExitRecord.sanitize (c, এতত্));
  }

  তার্কিক intersects (ধ্রুব hb_set_t *glyphs) ধ্রুব
  { প্রত্যাগম (এতত্+coverage).intersects (glyphs); }

  শূন্য closure_lookups (hb_closure_lookups_context_t *c) ধ্রুব {}

  শূন্য collect_variation_indices (hb_collect_variation_indices_context_t *c) ধ্রুব
  {
    + hb_zip (এতত্+coverage, entryExitRecord)
    | hb_filter (c->glyph_set, hb_first)
    | hb_map (hb_second)
    | hb_apply ([&] (ধ্রুব EntryExitRecord<Types>& record) { record.collect_variation_indices (c, এতত্); })
    ;
  }

  শূন্য collect_glyphs (hb_collect_glyphs_context_t *c) ধ্রুব
  { যদি (unlikely (!(এতত্+coverage).collect_coverage (c->প্রবেশ))) প্রত্যাগম; }

  শূন্য collect_second_glyphs (hb_set_digest_t *digest) ধ্রুব
  { (এতত্+coverage).collect_coverage (digest); }

  ধ্রুব Coverage &get_coverage () ধ্রুব { প্রত্যাগম এতত্+coverage; }

  তার্কিক apply (hb_ot_apply_context_t *c) ধ্রুব
  {
    TRACE_APPLY (এতত্);
    hb_buffer_t *buffer = c->buffer;

    ধ্রুব EntryExitRecord<Types> &this_record = entryExitRecord[(এতত্+coverage).get_coverage  (buffer->cur().codepoint)];
    যদি (!this_record.entryAnchor ||
	unlikely (!this_record.entryAnchor.sanitize (&c->sanitizer, এতত্))) return_trace (অসত্য);
    hb_barrier ();

    auto &skippy_iter = c->iter_প্রবেশ;
    skippy_iter.reset_fast (buffer->idx);
    unsigned unsafe_from;
    যদি (unlikely (!skippy_iter.prev (&unsafe_from)))
    {
      buffer->unsafe_to_concat_from_outbuffer (unsafe_from, buffer->idx + 1);
      return_trace (অসত্য);
    }

    ধ্রুব EntryExitRecord<Types> &prev_record = entryExitRecord[(এতত্+coverage).get_coverage  (buffer->info[skippy_iter.idx].codepoint)];
    যদি (!prev_record.exitAnchor ||
	unlikely (!prev_record.exitAnchor.sanitize (&c->sanitizer, এতত্)))
    {
      buffer->unsafe_to_concat_from_outbuffer (skippy_iter.idx, buffer->idx + 1);
      return_trace (অসত্য);
    }
    hb_barrier ();

    return_trace (apply_cursive_attachment (c,
                                            এতত্+prev_record.exitAnchor,
                                            এতত্+this_record.entryAnchor,
                                            skippy_iter.idx));
  }

  template <typename Iterator,
            hb_requires (hb_is_iterator (Iterator))>
  শূন্য serialize (hb_subset_context_t *c,
                  Iterator it,
                  ধ্রুব সংরচনা CursivePosFormat1_2<Types> *src_base)
  {
    যদি (unlikely (!c->serializer->extend_min ((*এতত্)))) প্রত্যাগম;
    এতত্->format = src_base->format;
    এতত্->entryExitRecord.len = it.len ();

    প্রতি (ধ্রুব EntryExitRecord<Types>& entry_record : + it
                                                      | hb_map (hb_second))
      entry_record.subset (c, src_base);

    auto glyphs =
    + it
    | hb_map_retains_sorting (hb_first)
    ;

    coverage.serialize_serialize (c->serializer, glyphs);
  }

  তার্কিক subset (hb_subset_context_t *c) ধ্রুব
  {
    TRACE_SUBSET (এতত্);
    ধ্রুব hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ধ্রুব hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*এতত্);

    auto it =
    + hb_zip (এতত্+coverage, entryExitRecord)
    | hb_filter (glyphset, hb_first)
    | hb_map_retains_sorting ([&] (hb_pair_t<hb_codepoint_t, ধ্রুব EntryExitRecord<Types>&> p)
                              -> hb_pair_t<hb_codepoint_t, ধ্রুব EntryExitRecord<Types>&>
                              { প্রত্যাগম hb_pair (glyph_map[p.first], p.second);})
    ;

    তার্কিক ret = তার্কিক (it);
    out->serialize (c, it, এতত্);
    return_trace (ret);
  }
};


}
}
}

#endif /* OT_LAYOUT_GPOS_CURSIVEPOSFORMAT1_HH */
