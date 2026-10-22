#ifndef OT_LAYOUT_GPOS_CURSIVEPOSFORMAT1_HH
#define OT_LAYOUT_GPOS_CURSIVEPOSFORMAT1_HH

#அந்தர்பாவ "Anchor.hh"
#அந்தர்பாவ "../../../hb-limits.hh"

நாமாகாஶ OT {
நாமாகாஶ Layout {
நாமாகாஶ GPOS_impl {

template <typename Types>
கட்டமைப்பு CursivePosFormat1_2;

template <typename Types>
கட்டமைப்பு EntryExitRecord
{
  template <typename Types2> friend கட்டமைப்பு CursivePosFormat1_2;

  தருக்க sanitize (hb_sanitize_context_t *c, மாறிலி கட்டமைப்பு CursivePosFormat1_2<Types> *base) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (entryAnchor.sanitize (c, base) && exitAnchor.sanitize (c, base));
  }

  வெறுமை collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  மாறிலி கட்டமைப்பு CursivePosFormat1_2<Types> *src_base) மாறிலி
  {
    (src_base+entryAnchor).collect_variation_indices (c);
    (src_base+exitAnchor).collect_variation_indices (c);
  }

  தருக்க subset (hb_subset_context_t *c,
	       மாறிலி கட்டமைப்பு CursivePosFormat1_2<Types> *src_base) மாறிலி
  {
    TRACE_SERIALIZE (இது);
    auto *out = c->serializer->embed (இது);
    எனில் (unlikely (!out)) return_trace (பொய்);

    தருக்க ret = பொய்;
    ret |= out->entryAnchor.serialize_subset (c, entryAnchor, src_base);
    ret |= out->exitAnchor.serialize_subset (c, exitAnchor, src_base);
    return_trace (ret);
  }

  பாதுகாக்கப்பட்ட:
  typename Types::template OffsetTo<Anchor, கட்டமைப்பு CursivePosFormat1_2<Types>>
                entryAnchor;            /* Offset to EntryAnchor table--from
                                         * beginning of CursivePos
                                         * subtable--may be NULL */
  typename Types::template OffsetTo<Anchor, கட்டமைப்பு CursivePosFormat1_2<Types>>
                exitAnchor;             /* Offset to ExitAnchor table--from
                                         * beginning of CursivePos
                                         * subtable--may be NULL */
  பொது:
  DEFINE_SIZE_STATIC (2 * Types::size);
};

நிலையான உள்வரி வெறுமை
reverse_cursive_minor_offset (hb_glyph_position_t *pos,
                              unsigned முழு len,
                              unsigned முழு i,
                              hb_direction_t direction,
                              unsigned முழு new_parent,
                              unsigned nesting_level = 0)
{
  எனில் (nesting_level > HB_MAX_NESTING_LEVEL)
    திரும்பு;

  முழு chain = pos[i].attach_chain(), type = pos[i].attach_type();
  எனில் (likely (!chain || 0 == (type & ATTACH_TYPE_CURSIVE)))
    திரும்பு;

  pos[i].attach_chain() = 0;

  unsigned முழு j = (முழு) i + chain;

  எனில் (unlikely (j >= len))
    திரும்பு;

  /* Stop if we see new parent in the chain. */
  எனில் (j == new_parent)
    திரும்பு;

  int16_t reversed_chain = -chain;
  /* The old edge was cleared above; if the reversed distance truncates,
   * keep it detached instead of storing a poisoned chain.
   */
  எனில் (unlikely (reversed_chain != -chain))
    திரும்பு;

  reverse_cursive_minor_offset (pos, len, j, direction, new_parent, nesting_level + 1);

  எனில் (HB_DIRECTION_IS_HORIZONTAL (direction))
    pos[j].y_offset = hb_saturate_neg (pos[i].y_offset);
  இல்லையெனில்
    pos[j].x_offset = hb_saturate_neg (pos[i].x_offset);

  pos[j].attach_chain() = reversed_chain;
  pos[j].attach_type() = type;
}


நிலையான உள்வரி தருக்க
apply_cursive_attachment (hb_ot_apply_context_t *c,
                          மாறிலி Anchor &exit_anchor,
                          மாறிலி Anchor &entry_anchor,
                          unsigned i)
{
  hb_buffer_t *buffer = c->buffer;
  unsigned முழு j = buffer->idx;

  எனில் (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
  {
    c->buffer->message (c->font,
			  "cursive attaching glyph at %u to glyph at %u",
			  i, j);
  }

  buffer->unsafe_to_break (i, j + 1);
  தசம entry_x, entry_y, exit_x, exit_y;
  exit_anchor.get_anchor (c, buffer->info[i].codepoint, &exit_x, &exit_y);
  entry_anchor.get_anchor (c, buffer->info[j].codepoint, &entry_x, &entry_y);

  hb_glyph_position_t *pos = buffer->pos;

  hb_position_t d;
  /* Main-direction adjustment */
  தேர்வு (c->direction) {
    நிலை HB_DIRECTION_LTR:
      pos[i].x_advance = hb_saturate_add ((hb_position_t) roundf (exit_x), pos[i].x_offset);

      d = hb_saturate_add ((hb_position_t) roundf (entry_x), pos[j].x_offset);
      pos[j].x_advance = hb_saturate_sub (pos[j].x_advance, d);
      pos[j].x_offset = hb_saturate_sub (pos[j].x_offset, d);
      நிறுத்து;
    நிலை HB_DIRECTION_RTL:
      d = hb_saturate_add ((hb_position_t) roundf (exit_x), pos[i].x_offset);
      pos[i].x_advance = hb_saturate_sub (pos[i].x_advance, d);
      pos[i].x_offset = hb_saturate_sub (pos[i].x_offset, d);

      pos[j].x_advance = hb_saturate_add ((hb_position_t) roundf (entry_x), pos[j].x_offset);
      நிறுத்து;
    நிலை HB_DIRECTION_TTB:
      pos[i].y_advance = hb_saturate_add ((hb_position_t) roundf (exit_y), pos[i].y_offset);

      d = hb_saturate_add ((hb_position_t) roundf (entry_y), pos[j].y_offset);
      pos[j].y_advance = hb_saturate_sub (pos[j].y_advance, d);
      pos[j].y_offset = hb_saturate_sub (pos[j].y_offset, d);
      நிறுத்து;
    நிலை HB_DIRECTION_BTT:
      d = hb_saturate_add ((hb_position_t) roundf (exit_y), pos[i].y_offset);
      pos[i].y_advance = hb_saturate_sub (pos[i].y_advance, d);
      pos[i].y_offset = hb_saturate_sub (pos[i].y_offset, d);

      pos[j].y_advance = (hb_position_t) roundf (entry_y);
      நிறுத்து;
    நிலை HB_DIRECTION_INVALID:
    இயல்பு:
      நிறுத்து;
  }

  /* Cross-direction adjustment */

  /* We attach child to parent (think graph theory and rooted trees whereas
   * the root stays on baseline and each node aligns itself against its
   * parent.
   *
   * Optimize things for the case of RightToLeft, as that's most common in
   * Arabic. */
  unsigned முழு child  = i;
  unsigned முழு parent = j;
  hb_position_t x_offset = (hb_position_t) roundf (entry_x - exit_x);
  hb_position_t y_offset = (hb_position_t) roundf (entry_y - exit_y);
  எனில்  (!(c->lookup_props & LookupFlag::வலது_To_இடது))
  {
    unsigned முழு k = child;
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

  pos[child].attach_chain() = (முழு) parent - (முழு) child;
  எனில் (pos[child].attach_chain() != (முழு) parent - (முழு) child)
  {
    pos[child].attach_chain() = 0;
    செல் overflow;
  }
  pos[child].attach_type() = ATTACH_TYPE_CURSIVE;
  buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT;
  எனில் (likely (HB_DIRECTION_IS_HORIZONTAL (c->direction)))
    pos[child].y_offset = y_offset;
  இல்லையெனில்
    pos[child].x_offset = x_offset;

  /* If parent was attached to child, separate them.
   * https://github.com/harfbuzz/harfbuzz/issues/2469
   */
  எனில் (unlikely (pos[parent].attach_chain() == -pos[child].attach_chain()))
  {
    pos[parent].attach_chain() = 0;
    எனில் (likely (HB_DIRECTION_IS_HORIZONTAL (c->direction)))
	pos[parent].y_offset = 0;
    இல்லையெனில்
	pos[parent].x_offset = 0;
  }

  எனில் (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
  {
    c->buffer->message (c->font,
			  "cursive attached glyph at %u to glyph at %u",
			  i, j);
  }

overflow:
  buffer->idx++;
  திரும்பு உண்மை;
}

template <typename Types>
கட்டமைப்பு CursivePosFormat1_2
{

  பாதுகாக்கப்பட்ட:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  typename Types::template LOffsetTo<Coverage>
                coverage;               /* Offset to Coverage table--from beginning of subtable */
  typename Types::template ArrayOf<EntryExitRecord<Types>>
                entryExitRecord;        /* Array of EntryExit records--in
                                         * Coverage Index order */
  பொது:
  DEFINE_SIZE_ARRAY (2 + Types::LOffset::static_size + Types::HBUINT::static_size, entryExitRecord);

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    எனில் (unlikely (!coverage.sanitize (c, இது)))
      return_trace (பொய்);

    எனில் (c->lazy_some_gpos)
      return_trace (entryExitRecord.sanitize_shallow (c));
    இல்லையெனில்
      return_trace (entryExitRecord.sanitize (c, இது));
  }

  தருக்க intersects (மாறிலி hb_set_t *glyphs) மாறிலி
  { திரும்பு (இது+coverage).intersects (glyphs); }

  வெறுமை closure_lookups (hb_closure_lookups_context_t *c) மாறிலி {}

  வெறுமை collect_variation_indices (hb_collect_variation_indices_context_t *c) மாறிலி
  {
    + hb_zip (இது+coverage, entryExitRecord)
    | hb_filter (c->glyph_set, hb_first)
    | hb_map (hb_second)
    | hb_apply ([&] (மாறிலி EntryExitRecord<Types>& record) { record.collect_variation_indices (c, இது); })
    ;
  }

  வெறுமை collect_glyphs (hb_collect_glyphs_context_t *c) மாறிலி
  { எனில் (unlikely (!(இது+coverage).collect_coverage (c->உள்ளீடு))) திரும்பு; }

  வெறுமை collect_second_glyphs (hb_set_digest_t *digest) மாறிலி
  { (இது+coverage).collect_coverage (digest); }

  மாறிலி Coverage &get_coverage () மாறிலி { திரும்பு இது+coverage; }

  தருக்க apply (hb_ot_apply_context_t *c) மாறிலி
  {
    TRACE_APPLY (இது);
    hb_buffer_t *buffer = c->buffer;

    மாறிலி EntryExitRecord<Types> &this_record = entryExitRecord[(இது+coverage).get_coverage  (buffer->cur().codepoint)];
    எனில் (!this_record.entryAnchor ||
	unlikely (!this_record.entryAnchor.sanitize (&c->sanitizer, இது))) return_trace (பொய்);
    hb_barrier ();

    auto &skippy_iter = c->iter_உள்ளீடு;
    skippy_iter.reset_fast (buffer->idx);
    unsigned unsafe_from;
    எனில் (unlikely (!skippy_iter.prev (&unsafe_from)))
    {
      buffer->unsafe_to_concat_from_outbuffer (unsafe_from, buffer->idx + 1);
      return_trace (பொய்);
    }

    மாறிலி EntryExitRecord<Types> &prev_record = entryExitRecord[(இது+coverage).get_coverage  (buffer->info[skippy_iter.idx].codepoint)];
    எனில் (!prev_record.exitAnchor ||
	unlikely (!prev_record.exitAnchor.sanitize (&c->sanitizer, இது)))
    {
      buffer->unsafe_to_concat_from_outbuffer (skippy_iter.idx, buffer->idx + 1);
      return_trace (பொய்);
    }
    hb_barrier ();

    return_trace (apply_cursive_attachment (c,
                                            இது+prev_record.exitAnchor,
                                            இது+this_record.entryAnchor,
                                            skippy_iter.idx));
  }

  template <typename Iterator,
            hb_requires (hb_is_iterator (Iterator))>
  வெறுமை serialize (hb_subset_context_t *c,
                  Iterator it,
                  மாறிலி கட்டமைப்பு CursivePosFormat1_2<Types> *src_base)
  {
    எனில் (unlikely (!c->serializer->extend_min ((*இது)))) திரும்பு;
    இது->format = src_base->format;
    இது->entryExitRecord.len = it.len ();

    க்கு (மாறிலி EntryExitRecord<Types>& entry_record : + it
                                                      | hb_map (hb_second))
      entry_record.subset (c, src_base);

    auto glyphs =
    + it
    | hb_map_retains_sorting (hb_first)
    ;

    coverage.serialize_serialize (c->serializer, glyphs);
  }

  தருக்க subset (hb_subset_context_t *c) மாறிலி
  {
    TRACE_SUBSET (இது);
    மாறிலி hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    மாறிலி hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*இது);

    auto it =
    + hb_zip (இது+coverage, entryExitRecord)
    | hb_filter (glyphset, hb_first)
    | hb_map_retains_sorting ([&] (hb_pair_t<hb_codepoint_t, மாறிலி EntryExitRecord<Types>&> p)
                              -> hb_pair_t<hb_codepoint_t, மாறிலி EntryExitRecord<Types>&>
                              { திரும்பு hb_pair (glyph_map[p.first], p.second);})
    ;

    தருக்க ret = தருக்க (it);
    out->serialize (c, it, இது);
    return_trace (ret);
  }
};


}
}
}

#endif /* OT_LAYOUT_GPOS_CURSIVEPOSFORMAT1_HH */
