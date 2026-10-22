#ifndef OT_LAYOUT_GPOS_CURSIVEPOSFORMAT1_HH
#define OT_LAYOUT_GPOS_CURSIVEPOSFORMAT1_HH

#ಅನ್ತರ್ಭಾವ "Anchor.hh"
#ಅನ್ತರ್ಭಾವ "../../../hb-limits.hh"

ನಾಮಾಕಾಶ OT {
ನಾಮಾಕಾಶ Layout {
ನಾಮಾಕಾಶ GPOS_impl {

template <typename Types>
ಸಂರಚನಾ CursivePosFormat1_2;

template <typename Types>
ಸಂರಚನಾ EntryExitRecord
{
  template <typename Types2> friend ಸಂರಚನಾ CursivePosFormat1_2;

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c, ಧ್ರುವ ಸಂರಚನಾ CursivePosFormat1_2<Types> *base) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (entryAnchor.sanitize (c, base) && exitAnchor.sanitize (c, base));
  }

  ಶೂನ್ಯ collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  ಧ್ರುವ ಸಂರಚನಾ CursivePosFormat1_2<Types> *src_base) ಧ್ರುವ
  {
    (src_base+entryAnchor).collect_variation_indices (c);
    (src_base+exitAnchor).collect_variation_indices (c);
  }

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c,
	       ಧ್ರುವ ಸಂರಚನಾ CursivePosFormat1_2<Types> *src_base) ಧ್ರುವ
  {
    TRACE_SERIALIZE (ಏತತ್);
    auto *out = c->serializer->embed (ಏತತ್);
    ಯದಿ (unlikely (!out)) return_trace (ಅಸತ್ಯ);

    ತಾರ್ಕಿಕ ret = ಅಸತ್ಯ;
    ret |= out->entryAnchor.serialize_subset (c, entryAnchor, src_base);
    ret |= out->exitAnchor.serialize_subset (c, exitAnchor, src_base);
    return_trace (ret);
  }

  ರಕ್ಷಿತ:
  typename Types::template OffsetTo<Anchor, ಸಂರಚನಾ CursivePosFormat1_2<Types>>
                entryAnchor;            /* Offset to EntryAnchor table--from
                                         * beginning of CursivePos
                                         * subtable--may be NULL */
  typename Types::template OffsetTo<Anchor, ಸಂರಚನಾ CursivePosFormat1_2<Types>>
                exitAnchor;             /* Offset to ExitAnchor table--from
                                         * beginning of CursivePos
                                         * subtable--may be NULL */
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (2 * Types::size);
};

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ
reverse_cursive_minor_offset (hb_glyph_position_t *pos,
                              unsigned ಪೂರ್ಣಾಙ್ಕ len,
                              unsigned ಪೂರ್ಣಾಙ್ಕ i,
                              hb_direction_t direction,
                              unsigned ಪೂರ್ಣಾಙ್ಕ new_parent,
                              unsigned nesting_level = 0)
{
  ಯದಿ (nesting_level > HB_MAX_NESTING_LEVEL)
    ಪ್ರತ್ಯಾಗಮ;

  ಪೂರ್ಣಾಙ್ಕ chain = pos[i].attach_chain(), type = pos[i].attach_type();
  ಯದಿ (likely (!chain || 0 == (type & ATTACH_TYPE_CURSIVE)))
    ಪ್ರತ್ಯಾಗಮ;

  pos[i].attach_chain() = 0;

  unsigned ಪೂರ್ಣಾಙ್ಕ j = (ಪೂರ್ಣಾಙ್ಕ) i + chain;

  ಯದಿ (unlikely (j >= len))
    ಪ್ರತ್ಯಾಗಮ;

  /* Stop if we see new parent in the chain. */
  ಯದಿ (j == new_parent)
    ಪ್ರತ್ಯಾಗಮ;

  int16_t reversed_chain = -chain;
  /* The old edge was cleared above; if the reversed distance truncates,
   * keep it detached instead of storing a poisoned chain.
   */
  ಯದಿ (unlikely (reversed_chain != -chain))
    ಪ್ರತ್ಯಾಗಮ;

  reverse_cursive_minor_offset (pos, len, j, direction, new_parent, nesting_level + 1);

  ಯದಿ (HB_DIRECTION_IS_HORIZONTAL (direction))
    pos[j].y_offset = hb_saturate_neg (pos[i].y_offset);
  ಅನ್ಯಥಾ
    pos[j].x_offset = hb_saturate_neg (pos[i].x_offset);

  pos[j].attach_chain() = reversed_chain;
  pos[j].attach_type() = type;
}


ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ತಾರ್ಕಿಕ
apply_cursive_attachment (hb_ot_apply_context_t *c,
                          ಧ್ರುವ Anchor &exit_anchor,
                          ಧ್ರುವ Anchor &entry_anchor,
                          unsigned i)
{
  hb_buffer_t *buffer = c->buffer;
  unsigned ಪೂರ್ಣಾಙ್ಕ j = buffer->idx;

  ಯದಿ (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
  {
    c->buffer->message (c->font,
			  "cursive attaching glyph at %u to glyph at %u",
			  i, j);
  }

  buffer->unsafe_to_break (i, j + 1);
  ಭಿನ್ನ entry_x, entry_y, exit_x, exit_y;
  exit_anchor.get_anchor (c, buffer->info[i].codepoint, &exit_x, &exit_y);
  entry_anchor.get_anchor (c, buffer->info[j].codepoint, &entry_x, &entry_y);

  hb_glyph_position_t *pos = buffer->pos;

  hb_position_t d;
  /* Main-direction adjustment */
  ವಿಕಲ್ಪ (c->direction) {
    ಪಕ್ಷ HB_DIRECTION_LTR:
      pos[i].x_advance = hb_saturate_add ((hb_position_t) roundf (exit_x), pos[i].x_offset);

      d = hb_saturate_add ((hb_position_t) roundf (entry_x), pos[j].x_offset);
      pos[j].x_advance = hb_saturate_sub (pos[j].x_advance, d);
      pos[j].x_offset = hb_saturate_sub (pos[j].x_offset, d);
      ಭಙ್ಗ;
    ಪಕ್ಷ HB_DIRECTION_RTL:
      d = hb_saturate_add ((hb_position_t) roundf (exit_x), pos[i].x_offset);
      pos[i].x_advance = hb_saturate_sub (pos[i].x_advance, d);
      pos[i].x_offset = hb_saturate_sub (pos[i].x_offset, d);

      pos[j].x_advance = hb_saturate_add ((hb_position_t) roundf (entry_x), pos[j].x_offset);
      ಭಙ್ಗ;
    ಪಕ್ಷ HB_DIRECTION_TTB:
      pos[i].y_advance = hb_saturate_add ((hb_position_t) roundf (exit_y), pos[i].y_offset);

      d = hb_saturate_add ((hb_position_t) roundf (entry_y), pos[j].y_offset);
      pos[j].y_advance = hb_saturate_sub (pos[j].y_advance, d);
      pos[j].y_offset = hb_saturate_sub (pos[j].y_offset, d);
      ಭಙ್ಗ;
    ಪಕ್ಷ HB_DIRECTION_BTT:
      d = hb_saturate_add ((hb_position_t) roundf (exit_y), pos[i].y_offset);
      pos[i].y_advance = hb_saturate_sub (pos[i].y_advance, d);
      pos[i].y_offset = hb_saturate_sub (pos[i].y_offset, d);

      pos[j].y_advance = (hb_position_t) roundf (entry_y);
      ಭಙ್ಗ;
    ಪಕ್ಷ HB_DIRECTION_INVALID:
    ಸ್ವಭಾವ:
      ಭಙ್ಗ;
  }

  /* Cross-direction adjustment */

  /* We attach child to parent (think graph theory and rooted trees whereas
   * the root stays on baseline and each node aligns itself against its
   * parent.
   *
   * Optimize things for the case of RightToLeft, as that's most common in
   * Arabic. */
  unsigned ಪೂರ್ಣಾಙ್ಕ child  = i;
  unsigned ಪೂರ್ಣಾಙ್ಕ parent = j;
  hb_position_t x_offset = (hb_position_t) roundf (entry_x - exit_x);
  hb_position_t y_offset = (hb_position_t) roundf (entry_y - exit_y);
  ಯದಿ  (!(c->lookup_props & LookupFlag::ಬಲ_To_ಎಡ))
  {
    unsigned ಪೂರ್ಣಾಙ್ಕ k = child;
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

  pos[child].attach_chain() = (ಪೂರ್ಣಾಙ್ಕ) parent - (ಪೂರ್ಣಾಙ್ಕ) child;
  ಯದಿ (pos[child].attach_chain() != (ಪೂರ್ಣಾಙ್ಕ) parent - (ಪೂರ್ಣಾಙ್ಕ) child)
  {
    pos[child].attach_chain() = 0;
    ಗಚ್ಛ overflow;
  }
  pos[child].attach_type() = ATTACH_TYPE_CURSIVE;
  buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT;
  ಯದಿ (likely (HB_DIRECTION_IS_HORIZONTAL (c->direction)))
    pos[child].y_offset = y_offset;
  ಅನ್ಯಥಾ
    pos[child].x_offset = x_offset;

  /* If parent was attached to child, separate them.
   * https://github.com/harfbuzz/harfbuzz/issues/2469
   */
  ಯದಿ (unlikely (pos[parent].attach_chain() == -pos[child].attach_chain()))
  {
    pos[parent].attach_chain() = 0;
    ಯದಿ (likely (HB_DIRECTION_IS_HORIZONTAL (c->direction)))
	pos[parent].y_offset = 0;
    ಅನ್ಯಥಾ
	pos[parent].x_offset = 0;
  }

  ಯದಿ (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
  {
    c->buffer->message (c->font,
			  "cursive attached glyph at %u to glyph at %u",
			  i, j);
  }

overflow:
  buffer->idx++;
  ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
}

template <typename Types>
ಸಂರಚನಾ CursivePosFormat1_2
{

  ರಕ್ಷಿತ:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  typename Types::template LOffsetTo<Coverage>
                coverage;               /* Offset to Coverage table--from beginning of subtable */
  typename Types::template ArrayOf<EntryExitRecord<Types>>
                entryExitRecord;        /* Array of EntryExit records--in
                                         * Coverage Index order */
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_ARRAY (2 + Types::LOffset::static_size + Types::HBUINT::static_size, entryExitRecord);

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    ಯದಿ (unlikely (!coverage.sanitize (c, ಏತತ್)))
      return_trace (ಅಸತ್ಯ);

    ಯದಿ (c->lazy_some_gpos)
      return_trace (entryExitRecord.sanitize_shallow (c));
    ಅನ್ಯಥಾ
      return_trace (entryExitRecord.sanitize (c, ಏತತ್));
  }

  ತಾರ್ಕಿಕ intersects (ಧ್ರುವ hb_set_t *glyphs) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ (ಏತತ್+coverage).intersects (glyphs); }

  ಶೂನ್ಯ closure_lookups (hb_closure_lookups_context_t *c) ಧ್ರುವ {}

  ಶೂನ್ಯ collect_variation_indices (hb_collect_variation_indices_context_t *c) ಧ್ರುವ
  {
    + hb_zip (ಏತತ್+coverage, entryExitRecord)
    | hb_filter (c->glyph_set, hb_first)
    | hb_map (hb_second)
    | hb_apply ([&] (ಧ್ರುವ EntryExitRecord<Types>& record) { record.collect_variation_indices (c, ಏತತ್); })
    ;
  }

  ಶೂನ್ಯ collect_glyphs (hb_collect_glyphs_context_t *c) ಧ್ರುವ
  { ಯದಿ (unlikely (!(ಏತತ್+coverage).collect_coverage (c->ಒಳಹರಿವು))) ಪ್ರತ್ಯಾಗಮ; }

  ಶೂನ್ಯ collect_second_glyphs (hb_set_digest_t *digest) ಧ್ರುವ
  { (ಏತತ್+coverage).collect_coverage (digest); }

  ಧ್ರುವ Coverage &get_coverage () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ ಏತತ್+coverage; }

  ತಾರ್ಕಿಕ apply (hb_ot_apply_context_t *c) ಧ್ರುವ
  {
    TRACE_APPLY (ಏತತ್);
    hb_buffer_t *buffer = c->buffer;

    ಧ್ರುವ EntryExitRecord<Types> &this_record = entryExitRecord[(ಏತತ್+coverage).get_coverage  (buffer->cur().codepoint)];
    ಯದಿ (!this_record.entryAnchor ||
	unlikely (!this_record.entryAnchor.sanitize (&c->sanitizer, ಏತತ್))) return_trace (ಅಸತ್ಯ);
    hb_barrier ();

    auto &skippy_iter = c->iter_ಒಳಹರಿವು;
    skippy_iter.reset_fast (buffer->idx);
    unsigned unsafe_from;
    ಯದಿ (unlikely (!skippy_iter.prev (&unsafe_from)))
    {
      buffer->unsafe_to_concat_from_outbuffer (unsafe_from, buffer->idx + 1);
      return_trace (ಅಸತ್ಯ);
    }

    ಧ್ರುವ EntryExitRecord<Types> &prev_record = entryExitRecord[(ಏತತ್+coverage).get_coverage  (buffer->info[skippy_iter.idx].codepoint)];
    ಯದಿ (!prev_record.exitAnchor ||
	unlikely (!prev_record.exitAnchor.sanitize (&c->sanitizer, ಏತತ್)))
    {
      buffer->unsafe_to_concat_from_outbuffer (skippy_iter.idx, buffer->idx + 1);
      return_trace (ಅಸತ್ಯ);
    }
    hb_barrier ();

    return_trace (apply_cursive_attachment (c,
                                            ಏತತ್+prev_record.exitAnchor,
                                            ಏತತ್+this_record.entryAnchor,
                                            skippy_iter.idx));
  }

  template <typename Iterator,
            hb_requires (hb_is_iterator (Iterator))>
  ಶೂನ್ಯ serialize (hb_subset_context_t *c,
                  Iterator it,
                  ಧ್ರುವ ಸಂರಚನಾ CursivePosFormat1_2<Types> *src_base)
  {
    ಯದಿ (unlikely (!c->serializer->extend_min ((*ಏತತ್)))) ಪ್ರತ್ಯಾಗಮ;
    ಏತತ್->format = src_base->format;
    ಏತತ್->entryExitRecord.len = it.len ();

    ಪ್ರತಿ (ಧ್ರುವ EntryExitRecord<Types>& entry_record : + it
                                                      | hb_map (hb_second))
      entry_record.subset (c, src_base);

    auto glyphs =
    + it
    | hb_map_retains_sorting (hb_first)
    ;

    coverage.serialize_serialize (c->serializer, glyphs);
  }

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    ಧ್ರುವ hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ಧ್ರುವ hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*ಏತತ್);

    auto it =
    + hb_zip (ಏತತ್+coverage, entryExitRecord)
    | hb_filter (glyphset, hb_first)
    | hb_map_retains_sorting ([&] (hb_pair_t<hb_codepoint_t, ಧ್ರುವ EntryExitRecord<Types>&> p)
                              -> hb_pair_t<hb_codepoint_t, ಧ್ರುವ EntryExitRecord<Types>&>
                              { ಪ್ರತ್ಯಾಗಮ hb_pair (glyph_map[p.first], p.second);})
    ;

    ತಾರ್ಕಿಕ ret = ತಾರ್ಕಿಕ (it);
    out->serialize (c, it, ಏತತ್);
    return_trace (ret);
  }
};


}
}
}

#endif /* OT_LAYOUT_GPOS_CURSIVEPOSFORMAT1_HH */
