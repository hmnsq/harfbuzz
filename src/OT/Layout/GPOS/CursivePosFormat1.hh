#ifndef OT_LAYOUT_GPOS_CURSIVEPOSFORMAT1_HH
#define OT_LAYOUT_GPOS_CURSIVEPOSFORMAT1_HH

#अन्तर्भाव "Anchor.hh"
#अन्तर्भाव "../../../hb-limits.hh"

नामाकाश OT {
नामाकाश Layout {
नामाकाश GPOS_impl {

template <typename Types>
संरचना CursivePosFormat1_2;

template <typename Types>
संरचना EntryExitRecord
{
  template <typename Types2> friend संरचना CursivePosFormat1_2;

  तार्किक sanitize (hb_sanitize_context_t *c, ध्रुव संरचना CursivePosFormat1_2<Types> *base) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (entryAnchor.sanitize (c, base) && exitAnchor.sanitize (c, base));
  }

  शून्य collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  ध्रुव संरचना CursivePosFormat1_2<Types> *src_base) ध्रुव
  {
    (src_base+entryAnchor).collect_variation_indices (c);
    (src_base+exitAnchor).collect_variation_indices (c);
  }

  तार्किक subset (hb_subset_context_t *c,
	       ध्रुव संरचना CursivePosFormat1_2<Types> *src_base) ध्रुव
  {
    TRACE_SERIALIZE (एतत्);
    auto *out = c->serializer->embed (एतत्);
    जइ (unlikely (!out)) return_trace (मिच्छ);

    तार्किक ret = मिच्छ;
    ret |= out->entryAnchor.serialize_subset (c, entryAnchor, src_base);
    ret |= out->exitAnchor.serialize_subset (c, exitAnchor, src_base);
    return_trace (ret);
  }

  रक्षित:
  typename Types::template OffsetTo<Anchor, संरचना CursivePosFormat1_2<Types>>
                entryAnchor;            /* Offset to EntryAnchor table--from
                                         * beginning of CursivePos
                                         * subtable--may be NULL */
  typename Types::template OffsetTo<Anchor, संरचना CursivePosFormat1_2<Types>>
                exitAnchor;             /* Offset to ExitAnchor table--from
                                         * beginning of CursivePos
                                         * subtable--may be NULL */
  सार्वजनिक:
  DEFINE_SIZE_STATIC (2 * Types::size);
};

स्थिर अन्तःस्थ शून्य
reverse_cursive_minor_offset (hb_glyph_position_t *pos,
                              unsigned पूर्णाङ्क len,
                              unsigned पूर्णाङ्क i,
                              hb_direction_t direction,
                              unsigned पूर्णाङ्क new_parent,
                              unsigned nesting_level = 0)
{
  जइ (nesting_level > HB_MAX_NESTING_LEVEL)
    प्रत्यागम;

  पूर्णाङ्क chain = pos[i].attach_chain(), type = pos[i].attach_type();
  जइ (likely (!chain || 0 == (type & ATTACH_TYPE_CURSIVE)))
    प्रत्यागम;

  pos[i].attach_chain() = 0;

  unsigned पूर्णाङ्क j = (पूर्णाङ्क) i + chain;

  जइ (unlikely (j >= len))
    प्रत्यागम;

  /* Stop if we see new parent in the chain. */
  जइ (j == new_parent)
    प्रत्यागम;

  int16_t reversed_chain = -chain;
  /* The old edge was cleared above; if the reversed distance truncates,
   * keep it detached instead of storing a poisoned chain.
   */
  जइ (unlikely (reversed_chain != -chain))
    प्रत्यागम;

  reverse_cursive_minor_offset (pos, len, j, direction, new_parent, nesting_level + 1);

  जइ (HB_DIRECTION_IS_HORIZONTAL (direction))
    pos[j].y_offset = hb_saturate_neg (pos[i].y_offset);
  अण्णहा
    pos[j].x_offset = hb_saturate_neg (pos[i].x_offset);

  pos[j].attach_chain() = reversed_chain;
  pos[j].attach_type() = type;
}


स्थिर अन्तःस्थ तार्किक
apply_cursive_attachment (hb_ot_apply_context_t *c,
                          ध्रुव Anchor &exit_anchor,
                          ध्रुव Anchor &entry_anchor,
                          unsigned i)
{
  hb_buffer_t *buffer = c->buffer;
  unsigned पूर्णाङ्क j = buffer->idx;

  जइ (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
  {
    c->buffer->message (c->font,
			  "cursive attaching glyph at %u to glyph at %u",
			  i, j);
  }

  buffer->unsafe_to_break (i, j + 1);
  भिन्न entry_x, entry_y, exit_x, exit_y;
  exit_anchor.get_anchor (c, buffer->info[i].codepoint, &exit_x, &exit_y);
  entry_anchor.get_anchor (c, buffer->info[j].codepoint, &entry_x, &entry_y);

  hb_glyph_position_t *pos = buffer->pos;

  hb_position_t d;
  /* Main-direction adjustment */
  विकल्प (c->direction) {
    पक्ष HB_DIRECTION_LTR:
      pos[i].x_advance = hb_saturate_add ((hb_position_t) roundf (exit_x), pos[i].x_offset);

      d = hb_saturate_add ((hb_position_t) roundf (entry_x), pos[j].x_offset);
      pos[j].x_advance = hb_saturate_sub (pos[j].x_advance, d);
      pos[j].x_offset = hb_saturate_sub (pos[j].x_offset, d);
      भङ्ग;
    पक्ष HB_DIRECTION_RTL:
      d = hb_saturate_add ((hb_position_t) roundf (exit_x), pos[i].x_offset);
      pos[i].x_advance = hb_saturate_sub (pos[i].x_advance, d);
      pos[i].x_offset = hb_saturate_sub (pos[i].x_offset, d);

      pos[j].x_advance = hb_saturate_add ((hb_position_t) roundf (entry_x), pos[j].x_offset);
      भङ्ग;
    पक्ष HB_DIRECTION_TTB:
      pos[i].y_advance = hb_saturate_add ((hb_position_t) roundf (exit_y), pos[i].y_offset);

      d = hb_saturate_add ((hb_position_t) roundf (entry_y), pos[j].y_offset);
      pos[j].y_advance = hb_saturate_sub (pos[j].y_advance, d);
      pos[j].y_offset = hb_saturate_sub (pos[j].y_offset, d);
      भङ्ग;
    पक्ष HB_DIRECTION_BTT:
      d = hb_saturate_add ((hb_position_t) roundf (exit_y), pos[i].y_offset);
      pos[i].y_advance = hb_saturate_sub (pos[i].y_advance, d);
      pos[i].y_offset = hb_saturate_sub (pos[i].y_offset, d);

      pos[j].y_advance = (hb_position_t) roundf (entry_y);
      भङ्ग;
    पक्ष HB_DIRECTION_INVALID:
    स्वभाव:
      भङ्ग;
  }

  /* Cross-direction adjustment */

  /* We attach child to parent (think graph theory and rooted trees whereas
   * the root stays on baseline and each node aligns itself against its
   * parent.
   *
   * Optimize things for the case of RightToLeft, as that's most common in
   * Arabic. */
  unsigned पूर्णाङ्क child  = i;
  unsigned पूर्णाङ्क parent = j;
  hb_position_t x_offset = (hb_position_t) roundf (entry_x - exit_x);
  hb_position_t y_offset = (hb_position_t) roundf (entry_y - exit_y);
  जइ  (!(c->lookup_props & LookupFlag::दक्षिणः_To_वामः))
  {
    unsigned पूर्णाङ्क k = child;
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

  pos[child].attach_chain() = (पूर्णाङ्क) parent - (पूर्णाङ्क) child;
  जइ (pos[child].attach_chain() != (पूर्णाङ्क) parent - (पूर्णाङ्क) child)
  {
    pos[child].attach_chain() = 0;
    गच्छ overflow;
  }
  pos[child].attach_type() = ATTACH_TYPE_CURSIVE;
  buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT;
  जइ (likely (HB_DIRECTION_IS_HORIZONTAL (c->direction)))
    pos[child].y_offset = y_offset;
  अण्णहा
    pos[child].x_offset = x_offset;

  /* If parent was attached to child, separate them.
   * https://github.com/harfbuzz/harfbuzz/issues/2469
   */
  जइ (unlikely (pos[parent].attach_chain() == -pos[child].attach_chain()))
  {
    pos[parent].attach_chain() = 0;
    जइ (likely (HB_DIRECTION_IS_HORIZONTAL (c->direction)))
	pos[parent].y_offset = 0;
    अण्णहा
	pos[parent].x_offset = 0;
  }

  जइ (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
  {
    c->buffer->message (c->font,
			  "cursive attached glyph at %u to glyph at %u",
			  i, j);
  }

overflow:
  buffer->idx++;
  प्रत्यागम सच्च;
}

template <typename Types>
संरचना CursivePosFormat1_2
{

  रक्षित:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  typename Types::template LOffsetTo<Coverage>
                coverage;               /* Offset to Coverage table--from beginning of subtable */
  typename Types::template ArrayOf<EntryExitRecord<Types>>
                entryExitRecord;        /* Array of EntryExit records--in
                                         * Coverage Index order */
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (2 + Types::LOffset::static_size + Types::HBUINT::static_size, entryExitRecord);

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    जइ (unlikely (!coverage.sanitize (c, एतत्)))
      return_trace (मिच्छ);

    जइ (c->lazy_some_gpos)
      return_trace (entryExitRecord.sanitize_shallow (c));
    अण्णहा
      return_trace (entryExitRecord.sanitize (c, एतत्));
  }

  तार्किक intersects (ध्रुव hb_set_t *glyphs) ध्रुव
  { प्रत्यागम (एतत्+coverage).intersects (glyphs); }

  शून्य closure_lookups (hb_closure_lookups_context_t *c) ध्रुव {}

  शून्य collect_variation_indices (hb_collect_variation_indices_context_t *c) ध्रुव
  {
    + hb_zip (एतत्+coverage, entryExitRecord)
    | hb_filter (c->glyph_set, hb_first)
    | hb_map (hb_second)
    | hb_apply ([&] (ध्रुव EntryExitRecord<Types>& record) { record.collect_variation_indices (c, एतत्); })
    ;
  }

  शून्य collect_glyphs (hb_collect_glyphs_context_t *c) ध्रुव
  { जइ (unlikely (!(एतत्+coverage).collect_coverage (c->निवेशः))) प्रत्यागम; }

  शून्य collect_second_glyphs (hb_set_digest_t *digest) ध्रुव
  { (एतत्+coverage).collect_coverage (digest); }

  ध्रुव Coverage &get_coverage () ध्रुव { प्रत्यागम एतत्+coverage; }

  तार्किक apply (hb_ot_apply_context_t *c) ध्रुव
  {
    TRACE_APPLY (एतत्);
    hb_buffer_t *buffer = c->buffer;

    ध्रुव EntryExitRecord<Types> &this_record = entryExitRecord[(एतत्+coverage).get_coverage  (buffer->cur().codepoint)];
    जइ (!this_record.entryAnchor ||
	unlikely (!this_record.entryAnchor.sanitize (&c->sanitizer, एतत्))) return_trace (मिच्छ);
    hb_barrier ();

    auto &skippy_iter = c->iter_निवेशः;
    skippy_iter.reset_fast (buffer->idx);
    unsigned unsafe_from;
    जइ (unlikely (!skippy_iter.prev (&unsafe_from)))
    {
      buffer->unsafe_to_concat_from_outbuffer (unsafe_from, buffer->idx + 1);
      return_trace (मिच्छ);
    }

    ध्रुव EntryExitRecord<Types> &prev_record = entryExitRecord[(एतत्+coverage).get_coverage  (buffer->info[skippy_iter.idx].codepoint)];
    जइ (!prev_record.exitAnchor ||
	unlikely (!prev_record.exitAnchor.sanitize (&c->sanitizer, एतत्)))
    {
      buffer->unsafe_to_concat_from_outbuffer (skippy_iter.idx, buffer->idx + 1);
      return_trace (मिच्छ);
    }
    hb_barrier ();

    return_trace (apply_cursive_attachment (c,
                                            एतत्+prev_record.exitAnchor,
                                            एतत्+this_record.entryAnchor,
                                            skippy_iter.idx));
  }

  template <typename Iterator,
            hb_requires (hb_is_iterator (Iterator))>
  शून्य serialize (hb_subset_context_t *c,
                  Iterator it,
                  ध्रुव संरचना CursivePosFormat1_2<Types> *src_base)
  {
    जइ (unlikely (!c->serializer->extend_min ((*एतत्)))) प्रत्यागम;
    एतत्->format = src_base->format;
    एतत्->entryExitRecord.len = it.len ();

    प्रति (ध्रुव EntryExitRecord<Types>& entry_record : + it
                                                      | hb_map (hb_second))
      entry_record.subset (c, src_base);

    auto glyphs =
    + it
    | hb_map_retains_sorting (hb_first)
    ;

    coverage.serialize_serialize (c->serializer, glyphs);
  }

  तार्किक subset (hb_subset_context_t *c) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    ध्रुव hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ध्रुव hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*एतत्);

    auto it =
    + hb_zip (एतत्+coverage, entryExitRecord)
    | hb_filter (glyphset, hb_first)
    | hb_map_retains_sorting ([&] (hb_pair_t<hb_codepoint_t, ध्रुव EntryExitRecord<Types>&> p)
                              -> hb_pair_t<hb_codepoint_t, ध्रुव EntryExitRecord<Types>&>
                              { प्रत्यागम hb_pair (glyph_map[p.first], p.second);})
    ;

    तार्किक ret = तार्किक (it);
    out->serialize (c, it, एतत्);
    return_trace (ret);
  }
};


}
}
}

#endif /* OT_LAYOUT_GPOS_CURSIVEPOSFORMAT1_HH */
