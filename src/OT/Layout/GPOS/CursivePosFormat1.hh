#ifndef OT_LAYOUT_GPOS_CURSIVEPOSFORMAT1_HH
#define OT_LAYOUT_GPOS_CURSIVEPOSFORMAT1_HH

#ଅନ୍ତର୍ଭାଵ "Anchor.hh"
#ଅନ୍ତର୍ଭାଵ "../../../hb-limits.hh"

ନାମାକାଶ OT {
ନାମାକାଶ Layout {
ନାମାକାଶ GPOS_impl {

template <typename Types>
ସଂରଚନା CursivePosFormat1_2;

template <typename Types>
ସଂରଚନା EntryExitRecord
{
  template <typename Types2> friend ସଂରଚନା CursivePosFormat1_2;

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c, ଅଚଳ ସଂରଚନା CursivePosFormat1_2<Types> *base) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (entryAnchor.sanitize (c, base) && exitAnchor.sanitize (c, base));
  }

  ଶୂନ୍ୟ collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  ଅଚଳ ସଂରଚନା CursivePosFormat1_2<Types> *src_base) ଅଚଳ
  {
    (src_base+entryAnchor).collect_variation_indices (c);
    (src_base+exitAnchor).collect_variation_indices (c);
  }

  ତାର୍କିକ subset (hb_subset_context_t *c,
	       ଅଚଳ ସଂରଚନା CursivePosFormat1_2<Types> *src_base) ଅଚଳ
  {
    TRACE_SERIALIZE (ଏହି);
    auto *out = c->serializer->embed (ଏହି);
    ଯଦି (unlikely (!out)) return_trace (ମିଥ୍ୟା);

    ତାର୍କିକ ret = ମିଥ୍ୟା;
    ret |= out->entryAnchor.serialize_subset (c, entryAnchor, src_base);
    ret |= out->exitAnchor.serialize_subset (c, exitAnchor, src_base);
    return_trace (ret);
  }

  ସୁରକ୍ଷିତ:
  typename Types::template OffsetTo<Anchor, ସଂରଚନା CursivePosFormat1_2<Types>>
                entryAnchor;            /* Offset to EntryAnchor table--from
                                         * beginning of CursivePos
                                         * subtable--may be NULL */
  typename Types::template OffsetTo<Anchor, ସଂରଚନା CursivePosFormat1_2<Types>>
                exitAnchor;             /* Offset to ExitAnchor table--from
                                         * beginning of CursivePos
                                         * subtable--may be NULL */
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (2 * Types::size);
};

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
reverse_cursive_minor_offset (hb_glyph_position_t *pos,
                              unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା len,
                              unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i,
                              hb_direction_t direction,
                              unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା new_parent,
                              unsigned nesting_level = 0)
{
  ଯଦି (nesting_level > HB_MAX_NESTING_LEVEL)
    ଫେରନ୍ତୁ;

  ପୂର୍ଣ୍ଣସଂଖ୍ୟା chain = pos[i].attach_chain(), type = pos[i].attach_type();
  ଯଦି (likely (!chain || 0 == (type & ATTACH_TYPE_CURSIVE)))
    ଫେରନ୍ତୁ;

  pos[i].attach_chain() = 0;

  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) i + chain;

  ଯଦି (unlikely (j >= len))
    ଫେରନ୍ତୁ;

  /* Stop if we see new parent in the chain. */
  ଯଦି (j == new_parent)
    ଫେରନ୍ତୁ;

  int16_t reversed_chain = -chain;
  /* The old edge was cleared above; if the reversed distance truncates,
   * keep it detached instead of storing a poisoned chain.
   */
  ଯଦି (unlikely (reversed_chain != -chain))
    ଫେରନ୍ତୁ;

  reverse_cursive_minor_offset (pos, len, j, direction, new_parent, nesting_level + 1);

  ଯଦି (HB_DIRECTION_IS_HORIZONTAL (direction))
    pos[j].y_offset = hb_saturate_neg (pos[i].y_offset);
  ଅନ୍ୟଥା
    pos[j].x_offset = hb_saturate_neg (pos[i].x_offset);

  pos[j].attach_chain() = reversed_chain;
  pos[j].attach_type() = type;
}


ସ୍ଥିର ଧାଡ଼ିଭିତରେ ତାର୍କିକ
apply_cursive_attachment (hb_ot_apply_context_t *c,
                          ଅଚଳ Anchor &exit_anchor,
                          ଅଚଳ Anchor &entry_anchor,
                          unsigned i)
{
  hb_buffer_t *buffer = c->buffer;
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = buffer->idx;

  ଯଦି (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
  {
    c->buffer->message (c->font,
			  "cursive attaching glyph at %u to glyph at %u",
			  i, j);
  }

  buffer->unsafe_to_break (i, j + 1);
  ଦଶମିକ entry_x, entry_y, exit_x, exit_y;
  exit_anchor.get_anchor (c, buffer->info[i].codepoint, &exit_x, &exit_y);
  entry_anchor.get_anchor (c, buffer->info[j].codepoint, &entry_x, &entry_y);

  hb_glyph_position_t *pos = buffer->pos;

  hb_position_t d;
  /* Main-direction adjustment */
  ବାଛ (c->direction) {
    ସ୍ଥିତି HB_DIRECTION_LTR:
      pos[i].x_advance = hb_saturate_add ((hb_position_t) roundf (exit_x), pos[i].x_offset);

      d = hb_saturate_add ((hb_position_t) roundf (entry_x), pos[j].x_offset);
      pos[j].x_advance = hb_saturate_sub (pos[j].x_advance, d);
      pos[j].x_offset = hb_saturate_sub (pos[j].x_offset, d);
      ଭାଙ୍ଗ;
    ସ୍ଥିତି HB_DIRECTION_RTL:
      d = hb_saturate_add ((hb_position_t) roundf (exit_x), pos[i].x_offset);
      pos[i].x_advance = hb_saturate_sub (pos[i].x_advance, d);
      pos[i].x_offset = hb_saturate_sub (pos[i].x_offset, d);

      pos[j].x_advance = hb_saturate_add ((hb_position_t) roundf (entry_x), pos[j].x_offset);
      ଭାଙ୍ଗ;
    ସ୍ଥିତି HB_DIRECTION_TTB:
      pos[i].y_advance = hb_saturate_add ((hb_position_t) roundf (exit_y), pos[i].y_offset);

      d = hb_saturate_add ((hb_position_t) roundf (entry_y), pos[j].y_offset);
      pos[j].y_advance = hb_saturate_sub (pos[j].y_advance, d);
      pos[j].y_offset = hb_saturate_sub (pos[j].y_offset, d);
      ଭାଙ୍ଗ;
    ସ୍ଥିତି HB_DIRECTION_BTT:
      d = hb_saturate_add ((hb_position_t) roundf (exit_y), pos[i].y_offset);
      pos[i].y_advance = hb_saturate_sub (pos[i].y_advance, d);
      pos[i].y_offset = hb_saturate_sub (pos[i].y_offset, d);

      pos[j].y_advance = (hb_position_t) roundf (entry_y);
      ଭାଙ୍ଗ;
    ସ୍ଥିତି HB_DIRECTION_INVALID:
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
      ଭାଙ୍ଗ;
  }

  /* Cross-direction adjustment */

  /* We attach child to parent (think graph theory and rooted trees whereas
   * the root stays on baseline and each node aligns itself against its
   * parent.
   *
   * Optimize things for the case of RightToLeft, as that's most common in
   * Arabic. */
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା child  = i;
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା parent = j;
  hb_position_t x_offset = (hb_position_t) roundf (entry_x - exit_x);
  hb_position_t y_offset = (hb_position_t) roundf (entry_y - exit_y);
  ଯଦି  (!(c->lookup_props & LookupFlag::ଡାହାଣ_To_ବାମ))
  {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = child;
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

  pos[child].attach_chain() = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) parent - (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) child;
  ଯଦି (pos[child].attach_chain() != (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) parent - (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) child)
  {
    pos[child].attach_chain() = 0;
    ଯାଅ overflow;
  }
  pos[child].attach_type() = ATTACH_TYPE_CURSIVE;
  buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT;
  ଯଦି (likely (HB_DIRECTION_IS_HORIZONTAL (c->direction)))
    pos[child].y_offset = y_offset;
  ଅନ୍ୟଥା
    pos[child].x_offset = x_offset;

  /* If parent was attached to child, separate them.
   * https://github.com/harfbuzz/harfbuzz/issues/2469
   */
  ଯଦି (unlikely (pos[parent].attach_chain() == -pos[child].attach_chain()))
  {
    pos[parent].attach_chain() = 0;
    ଯଦି (likely (HB_DIRECTION_IS_HORIZONTAL (c->direction)))
	pos[parent].y_offset = 0;
    ଅନ୍ୟଥା
	pos[parent].x_offset = 0;
  }

  ଯଦି (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
  {
    c->buffer->message (c->font,
			  "cursive attached glyph at %u to glyph at %u",
			  i, j);
  }

overflow:
  buffer->idx++;
  ଫେରନ୍ତୁ ସତ୍ୟ;
}

template <typename Types>
ସଂରଚନା CursivePosFormat1_2
{

  ସୁରକ୍ଷିତ:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  typename Types::template LOffsetTo<Coverage>
                coverage;               /* Offset to Coverage table--from beginning of subtable */
  typename Types::template ArrayOf<EntryExitRecord<Types>>
                entryExitRecord;        /* Array of EntryExit records--in
                                         * Coverage Index order */
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_ARRAY (2 + Types::LOffset::static_size + Types::HBUINT::static_size, entryExitRecord);

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    ଯଦି (unlikely (!coverage.sanitize (c, ଏହି)))
      return_trace (ମିଥ୍ୟା);

    ଯଦି (c->lazy_some_gpos)
      return_trace (entryExitRecord.sanitize_shallow (c));
    ଅନ୍ୟଥା
      return_trace (entryExitRecord.sanitize (c, ଏହି));
  }

  ତାର୍କିକ intersects (ଅଚଳ hb_set_t *glyphs) ଅଚଳ
  { ଫେରନ୍ତୁ (ଏହି+coverage).intersects (glyphs); }

  ଶୂନ୍ୟ closure_lookups (hb_closure_lookups_context_t *c) ଅଚଳ {}

  ଶୂନ୍ୟ collect_variation_indices (hb_collect_variation_indices_context_t *c) ଅଚଳ
  {
    + hb_zip (ଏହି+coverage, entryExitRecord)
    | hb_filter (c->glyph_set, hb_first)
    | hb_map (hb_second)
    | hb_apply ([&] (ଅଚଳ EntryExitRecord<Types>& record) { record.collect_variation_indices (c, ଏହି); })
    ;
  }

  ଶୂନ୍ୟ collect_glyphs (hb_collect_glyphs_context_t *c) ଅଚଳ
  { ଯଦି (unlikely (!(ଏହି+coverage).collect_coverage (c->ନିବେଶ))) ଫେରନ୍ତୁ; }

  ଶୂନ୍ୟ collect_second_glyphs (hb_set_digest_t *digest) ଅଚଳ
  { (ଏହି+coverage).collect_coverage (digest); }

  ଅଚଳ Coverage &get_coverage () ଅଚଳ { ଫେରନ୍ତୁ ଏହି+coverage; }

  ତାର୍କିକ apply (hb_ot_apply_context_t *c) ଅଚଳ
  {
    TRACE_APPLY (ଏହି);
    hb_buffer_t *buffer = c->buffer;

    ଅଚଳ EntryExitRecord<Types> &this_record = entryExitRecord[(ଏହି+coverage).get_coverage  (buffer->cur().codepoint)];
    ଯଦି (!this_record.entryAnchor ||
	unlikely (!this_record.entryAnchor.sanitize (&c->sanitizer, ଏହି))) return_trace (ମିଥ୍ୟା);
    hb_barrier ();

    auto &skippy_iter = c->iter_ନିବେଶ;
    skippy_iter.reset_fast (buffer->idx);
    unsigned unsafe_from;
    ଯଦି (unlikely (!skippy_iter.prev (&unsafe_from)))
    {
      buffer->unsafe_to_concat_from_outbuffer (unsafe_from, buffer->idx + 1);
      return_trace (ମିଥ୍ୟା);
    }

    ଅଚଳ EntryExitRecord<Types> &prev_record = entryExitRecord[(ଏହି+coverage).get_coverage  (buffer->info[skippy_iter.idx].codepoint)];
    ଯଦି (!prev_record.exitAnchor ||
	unlikely (!prev_record.exitAnchor.sanitize (&c->sanitizer, ଏହି)))
    {
      buffer->unsafe_to_concat_from_outbuffer (skippy_iter.idx, buffer->idx + 1);
      return_trace (ମିଥ୍ୟା);
    }
    hb_barrier ();

    return_trace (apply_cursive_attachment (c,
                                            ଏହି+prev_record.exitAnchor,
                                            ଏହି+this_record.entryAnchor,
                                            skippy_iter.idx));
  }

  template <typename Iterator,
            hb_requires (hb_is_iterator (Iterator))>
  ଶୂନ୍ୟ serialize (hb_subset_context_t *c,
                  Iterator it,
                  ଅଚଳ ସଂରଚନା CursivePosFormat1_2<Types> *src_base)
  {
    ଯଦି (unlikely (!c->serializer->extend_min ((*ଏହି)))) ଫେରନ୍ତୁ;
    ଏହି->format = src_base->format;
    ଏହି->entryExitRecord.len = it.len ();

    ପାଇଁ (ଅଚଳ EntryExitRecord<Types>& entry_record : + it
                                                      | hb_map (hb_second))
      entry_record.subset (c, src_base);

    auto glyphs =
    + it
    | hb_map_retains_sorting (hb_first)
    ;

    coverage.serialize_serialize (c->serializer, glyphs);
  }

  ତାର୍କିକ subset (hb_subset_context_t *c) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    ଅଚଳ hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ଅଚଳ hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*ଏହି);

    auto it =
    + hb_zip (ଏହି+coverage, entryExitRecord)
    | hb_filter (glyphset, hb_first)
    | hb_map_retains_sorting ([&] (hb_pair_t<hb_codepoint_t, ଅଚଳ EntryExitRecord<Types>&> p)
                              -> hb_pair_t<hb_codepoint_t, ଅଚଳ EntryExitRecord<Types>&>
                              { ଫେରନ୍ତୁ hb_pair (glyph_map[p.first], p.second);})
    ;

    ତାର୍କିକ ret = ତାର୍କିକ (it);
    out->serialize (c, it, ଏହି);
    return_trace (ret);
  }
};


}
}
}

#endif /* OT_LAYOUT_GPOS_CURSIVEPOSFORMAT1_HH */
