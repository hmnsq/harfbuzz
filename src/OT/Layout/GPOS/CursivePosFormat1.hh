#ifndef OT_LAYOUT_GPOS_CURSIVEPOSFORMAT1_HH
#define OT_LAYOUT_GPOS_CURSIVEPOSFORMAT1_HH

#અન્તર્ભાવ "Anchor.hh"
#અન્તર્ભાવ "../../../hb-limits.hh"

નામાકાશ OT {
નામાકાશ Layout {
નામાકાશ GPOS_impl {

template <typename Types>
સંરચના CursivePosFormat1_2;

template <typename Types>
સંરચના EntryExitRecord
{
  template <typename Types2> friend સંરચના CursivePosFormat1_2;

  તાર્કિક sanitize (hb_sanitize_context_t *c, અચલ સંરચના CursivePosFormat1_2<Types> *base) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (entryAnchor.sanitize (c, base) && exitAnchor.sanitize (c, base));
  }

  શૂન્ય collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  અચલ સંરચના CursivePosFormat1_2<Types> *src_base) અચલ
  {
    (src_base+entryAnchor).collect_variation_indices (c);
    (src_base+exitAnchor).collect_variation_indices (c);
  }

  તાર્કિક subset (hb_subset_context_t *c,
	       અચલ સંરચના CursivePosFormat1_2<Types> *src_base) અચલ
  {
    TRACE_SERIALIZE (આ);
    auto *out = c->serializer->embed (આ);
    જો (unlikely (!out)) return_trace (ખોટું);

    તાર્કિક ret = ખોટું;
    ret |= out->entryAnchor.serialize_subset (c, entryAnchor, src_base);
    ret |= out->exitAnchor.serialize_subset (c, exitAnchor, src_base);
    return_trace (ret);
  }

  સુરક્ષિત:
  typename Types::template OffsetTo<Anchor, સંરચના CursivePosFormat1_2<Types>>
                entryAnchor;            /* Offset to EntryAnchor table--from
                                         * beginning of CursivePos
                                         * subtable--may be NULL */
  typename Types::template OffsetTo<Anchor, સંરચના CursivePosFormat1_2<Types>>
                exitAnchor;             /* Offset to ExitAnchor table--from
                                         * beginning of CursivePos
                                         * subtable--may be NULL */
  જાહેર:
  DEFINE_SIZE_STATIC (2 * Types::size);
};

સ્થિર અંતઃરેખ શૂન્ય
reverse_cursive_minor_offset (hb_glyph_position_t *pos,
                              unsigned પૂર્ણાંક len,
                              unsigned પૂર્ણાંક i,
                              hb_direction_t direction,
                              unsigned પૂર્ણાંક new_parent,
                              unsigned nesting_level = 0)
{
  જો (nesting_level > HB_MAX_NESTING_LEVEL)
    પાછા;

  પૂર્ણાંક chain = pos[i].attach_chain(), type = pos[i].attach_type();
  જો (likely (!chain || 0 == (type & ATTACH_TYPE_CURSIVE)))
    પાછા;

  pos[i].attach_chain() = 0;

  unsigned પૂર્ણાંક j = (પૂર્ણાંક) i + chain;

  જો (unlikely (j >= len))
    પાછા;

  /* Stop if we see new parent in the chain. */
  જો (j == new_parent)
    પાછા;

  int16_t reversed_chain = -chain;
  /* The old edge was cleared above; if the reversed distance truncates,
   * keep it detached instead of storing a poisoned chain.
   */
  જો (unlikely (reversed_chain != -chain))
    પાછા;

  reverse_cursive_minor_offset (pos, len, j, direction, new_parent, nesting_level + 1);

  જો (HB_DIRECTION_IS_HORIZONTAL (direction))
    pos[j].y_offset = hb_saturate_neg (pos[i].y_offset);
  નહીંતર
    pos[j].x_offset = hb_saturate_neg (pos[i].x_offset);

  pos[j].attach_chain() = reversed_chain;
  pos[j].attach_type() = type;
}


સ્થિર અંતઃરેખ તાર્કિક
apply_cursive_attachment (hb_ot_apply_context_t *c,
                          અચલ Anchor &exit_anchor,
                          અચલ Anchor &entry_anchor,
                          unsigned i)
{
  hb_buffer_t *buffer = c->buffer;
  unsigned પૂર્ણાંક j = buffer->idx;

  જો (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
  {
    c->buffer->message (c->font,
			  "cursive attaching glyph at %u to glyph at %u",
			  i, j);
  }

  buffer->unsafe_to_break (i, j + 1);
  દશાંશ entry_x, entry_y, exit_x, exit_y;
  exit_anchor.get_anchor (c, buffer->info[i].codepoint, &exit_x, &exit_y);
  entry_anchor.get_anchor (c, buffer->info[j].codepoint, &entry_x, &entry_y);

  hb_glyph_position_t *pos = buffer->pos;

  hb_position_t d;
  /* Main-direction adjustment */
  પસંદગી (c->direction) {
    સ્થિતિ HB_DIRECTION_LTR:
      pos[i].x_advance = hb_saturate_add ((hb_position_t) roundf (exit_x), pos[i].x_offset);

      d = hb_saturate_add ((hb_position_t) roundf (entry_x), pos[j].x_offset);
      pos[j].x_advance = hb_saturate_sub (pos[j].x_advance, d);
      pos[j].x_offset = hb_saturate_sub (pos[j].x_offset, d);
      તોડો;
    સ્થિતિ HB_DIRECTION_RTL:
      d = hb_saturate_add ((hb_position_t) roundf (exit_x), pos[i].x_offset);
      pos[i].x_advance = hb_saturate_sub (pos[i].x_advance, d);
      pos[i].x_offset = hb_saturate_sub (pos[i].x_offset, d);

      pos[j].x_advance = hb_saturate_add ((hb_position_t) roundf (entry_x), pos[j].x_offset);
      તોડો;
    સ્થિતિ HB_DIRECTION_TTB:
      pos[i].y_advance = hb_saturate_add ((hb_position_t) roundf (exit_y), pos[i].y_offset);

      d = hb_saturate_add ((hb_position_t) roundf (entry_y), pos[j].y_offset);
      pos[j].y_advance = hb_saturate_sub (pos[j].y_advance, d);
      pos[j].y_offset = hb_saturate_sub (pos[j].y_offset, d);
      તોડો;
    સ્થિતિ HB_DIRECTION_BTT:
      d = hb_saturate_add ((hb_position_t) roundf (exit_y), pos[i].y_offset);
      pos[i].y_advance = hb_saturate_sub (pos[i].y_advance, d);
      pos[i].y_offset = hb_saturate_sub (pos[i].y_offset, d);

      pos[j].y_advance = (hb_position_t) roundf (entry_y);
      તોડો;
    સ્થિતિ HB_DIRECTION_INVALID:
    મૂળભૂત:
      તોડો;
  }

  /* Cross-direction adjustment */

  /* We attach child to parent (think graph theory and rooted trees whereas
   * the root stays on baseline and each node aligns itself against its
   * parent.
   *
   * Optimize things for the case of RightToLeft, as that's most common in
   * Arabic. */
  unsigned પૂર્ણાંક child  = i;
  unsigned પૂર્ણાંક parent = j;
  hb_position_t x_offset = (hb_position_t) roundf (entry_x - exit_x);
  hb_position_t y_offset = (hb_position_t) roundf (entry_y - exit_y);
  જો  (!(c->lookup_props & LookupFlag::જમણું_To_ડાબું))
  {
    unsigned પૂર્ણાંક k = child;
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

  pos[child].attach_chain() = (પૂર્ણાંક) parent - (પૂર્ણાંક) child;
  જો (pos[child].attach_chain() != (પૂર્ણાંક) parent - (પૂર્ણાંક) child)
  {
    pos[child].attach_chain() = 0;
    જાઓ overflow;
  }
  pos[child].attach_type() = ATTACH_TYPE_CURSIVE;
  buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT;
  જો (likely (HB_DIRECTION_IS_HORIZONTAL (c->direction)))
    pos[child].y_offset = y_offset;
  નહીંતર
    pos[child].x_offset = x_offset;

  /* If parent was attached to child, separate them.
   * https://github.com/harfbuzz/harfbuzz/issues/2469
   */
  જો (unlikely (pos[parent].attach_chain() == -pos[child].attach_chain()))
  {
    pos[parent].attach_chain() = 0;
    જો (likely (HB_DIRECTION_IS_HORIZONTAL (c->direction)))
	pos[parent].y_offset = 0;
    નહીંતર
	pos[parent].x_offset = 0;
  }

  જો (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
  {
    c->buffer->message (c->font,
			  "cursive attached glyph at %u to glyph at %u",
			  i, j);
  }

overflow:
  buffer->idx++;
  પાછા સાચું;
}

template <typename Types>
સંરચના CursivePosFormat1_2
{

  સુરક્ષિત:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  typename Types::template LOffsetTo<Coverage>
                coverage;               /* Offset to Coverage table--from beginning of subtable */
  typename Types::template ArrayOf<EntryExitRecord<Types>>
                entryExitRecord;        /* Array of EntryExit records--in
                                         * Coverage Index order */
  જાહેર:
  DEFINE_SIZE_ARRAY (2 + Types::LOffset::static_size + Types::HBUINT::static_size, entryExitRecord);

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    જો (unlikely (!coverage.sanitize (c, આ)))
      return_trace (ખોટું);

    જો (c->lazy_some_gpos)
      return_trace (entryExitRecord.sanitize_shallow (c));
    નહીંતર
      return_trace (entryExitRecord.sanitize (c, આ));
  }

  તાર્કિક intersects (અચલ hb_set_t *glyphs) અચલ
  { પાછા (આ+coverage).intersects (glyphs); }

  શૂન્ય closure_lookups (hb_closure_lookups_context_t *c) અચલ {}

  શૂન્ય collect_variation_indices (hb_collect_variation_indices_context_t *c) અચલ
  {
    + hb_zip (આ+coverage, entryExitRecord)
    | hb_filter (c->glyph_set, hb_first)
    | hb_map (hb_second)
    | hb_apply ([&] (અચલ EntryExitRecord<Types>& record) { record.collect_variation_indices (c, આ); })
    ;
  }

  શૂન્ય collect_glyphs (hb_collect_glyphs_context_t *c) અચલ
  { જો (unlikely (!(આ+coverage).collect_coverage (c->આગત))) પાછા; }

  શૂન્ય collect_second_glyphs (hb_set_digest_t *digest) અચલ
  { (આ+coverage).collect_coverage (digest); }

  અચલ Coverage &get_coverage () અચલ { પાછા આ+coverage; }

  તાર્કિક apply (hb_ot_apply_context_t *c) અચલ
  {
    TRACE_APPLY (આ);
    hb_buffer_t *buffer = c->buffer;

    અચલ EntryExitRecord<Types> &this_record = entryExitRecord[(આ+coverage).get_coverage  (buffer->cur().codepoint)];
    જો (!this_record.entryAnchor ||
	unlikely (!this_record.entryAnchor.sanitize (&c->sanitizer, આ))) return_trace (ખોટું);
    hb_barrier ();

    auto &skippy_iter = c->iter_આગત;
    skippy_iter.reset_fast (buffer->idx);
    unsigned unsafe_from;
    જો (unlikely (!skippy_iter.prev (&unsafe_from)))
    {
      buffer->unsafe_to_concat_from_outbuffer (unsafe_from, buffer->idx + 1);
      return_trace (ખોટું);
    }

    અચલ EntryExitRecord<Types> &prev_record = entryExitRecord[(આ+coverage).get_coverage  (buffer->info[skippy_iter.idx].codepoint)];
    જો (!prev_record.exitAnchor ||
	unlikely (!prev_record.exitAnchor.sanitize (&c->sanitizer, આ)))
    {
      buffer->unsafe_to_concat_from_outbuffer (skippy_iter.idx, buffer->idx + 1);
      return_trace (ખોટું);
    }
    hb_barrier ();

    return_trace (apply_cursive_attachment (c,
                                            આ+prev_record.exitAnchor,
                                            આ+this_record.entryAnchor,
                                            skippy_iter.idx));
  }

  template <typename Iterator,
            hb_requires (hb_is_iterator (Iterator))>
  શૂન્ય serialize (hb_subset_context_t *c,
                  Iterator it,
                  અચલ સંરચના CursivePosFormat1_2<Types> *src_base)
  {
    જો (unlikely (!c->serializer->extend_min ((*આ)))) પાછા;
    આ->format = src_base->format;
    આ->entryExitRecord.len = it.len ();

    માટે (અચલ EntryExitRecord<Types>& entry_record : + it
                                                      | hb_map (hb_second))
      entry_record.subset (c, src_base);

    auto glyphs =
    + it
    | hb_map_retains_sorting (hb_first)
    ;

    coverage.serialize_serialize (c->serializer, glyphs);
  }

  તાર્કિક subset (hb_subset_context_t *c) અચલ
  {
    TRACE_SUBSET (આ);
    અચલ hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    અચલ hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*આ);

    auto it =
    + hb_zip (આ+coverage, entryExitRecord)
    | hb_filter (glyphset, hb_first)
    | hb_map_retains_sorting ([&] (hb_pair_t<hb_codepoint_t, અચલ EntryExitRecord<Types>&> p)
                              -> hb_pair_t<hb_codepoint_t, અચલ EntryExitRecord<Types>&>
                              { પાછા hb_pair (glyph_map[p.first], p.second);})
    ;

    તાર્કિક ret = તાર્કિક (it);
    out->serialize (c, it, આ);
    return_trace (ret);
  }
};


}
}
}

#endif /* OT_LAYOUT_GPOS_CURSIVEPOSFORMAT1_HH */
