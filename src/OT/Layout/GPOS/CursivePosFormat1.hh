#ifndef OT_LAYOUT_GPOS_CURSIVEPOSFORMAT1_HH
#define OT_LAYOUT_GPOS_CURSIVEPOSFORMAT1_HH

#അന്തര്ഭാവ "Anchor.hh"
#അന്തര്ഭാവ "../../../hb-limits.hh"

നാമാകാശ OT {
നാമാകാശ Layout {
നാമാകാശ GPOS_impl {

template <typename Types>
ഘടന CursivePosFormat1_2;

template <typename Types>
ഘടന EntryExitRecord
{
  template <typename Types2> friend ഘടന CursivePosFormat1_2;

  തർക്കം sanitize (hb_sanitize_context_t *c, സ്ഥിരം ഘടന CursivePosFormat1_2<Types> *base) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (entryAnchor.sanitize (c, base) && exitAnchor.sanitize (c, base));
  }

  ശൂന്യം collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  സ്ഥിരം ഘടന CursivePosFormat1_2<Types> *src_base) സ്ഥിരം
  {
    (src_base+entryAnchor).collect_variation_indices (c);
    (src_base+exitAnchor).collect_variation_indices (c);
  }

  തർക്കം subset (hb_subset_context_t *c,
	       സ്ഥിരം ഘടന CursivePosFormat1_2<Types> *src_base) സ്ഥിരം
  {
    TRACE_SERIALIZE (ഇത്);
    auto *out = c->serializer->embed (ഇത്);
    എങ്കിൽ (unlikely (!out)) return_trace (അസത്യം);

    തർക്കം ret = അസത്യം;
    ret |= out->entryAnchor.serialize_subset (c, entryAnchor, src_base);
    ret |= out->exitAnchor.serialize_subset (c, exitAnchor, src_base);
    return_trace (ret);
  }

  സംരക്ഷിത:
  typename Types::template OffsetTo<Anchor, ഘടന CursivePosFormat1_2<Types>>
                entryAnchor;            /* Offset to EntryAnchor table--from
                                         * beginning of CursivePos
                                         * subtable--may be NULL */
  typename Types::template OffsetTo<Anchor, ഘടന CursivePosFormat1_2<Types>>
                exitAnchor;             /* Offset to ExitAnchor table--from
                                         * beginning of CursivePos
                                         * subtable--may be NULL */
  പൊതു:
  DEFINE_SIZE_STATIC (2 * Types::size);
};

സ്ഥിതം വരിയിൽ ശൂന്യം
reverse_cursive_minor_offset (hb_glyph_position_t *pos,
                              unsigned പൂർണ്ണസംഖ്യ len,
                              unsigned പൂർണ്ണസംഖ്യ i,
                              hb_direction_t direction,
                              unsigned പൂർണ്ണസംഖ്യ new_parent,
                              unsigned nesting_level = 0)
{
  എങ്കിൽ (nesting_level > HB_MAX_NESTING_LEVEL)
    തിരികെ;

  പൂർണ്ണസംഖ്യ chain = pos[i].attach_chain(), type = pos[i].attach_type();
  എങ്കിൽ (likely (!chain || 0 == (type & ATTACH_TYPE_CURSIVE)))
    തിരികെ;

  pos[i].attach_chain() = 0;

  unsigned പൂർണ്ണസംഖ്യ j = (പൂർണ്ണസംഖ്യ) i + chain;

  എങ്കിൽ (unlikely (j >= len))
    തിരികെ;

  /* Stop if we see new parent in the chain. */
  എങ്കിൽ (j == new_parent)
    തിരികെ;

  int16_t reversed_chain = -chain;
  /* The old edge was cleared above; if the reversed distance truncates,
   * keep it detached instead of storing a poisoned chain.
   */
  എങ്കിൽ (unlikely (reversed_chain != -chain))
    തിരികെ;

  reverse_cursive_minor_offset (pos, len, j, direction, new_parent, nesting_level + 1);

  എങ്കിൽ (HB_DIRECTION_IS_HORIZONTAL (direction))
    pos[j].y_offset = hb_saturate_neg (pos[i].y_offset);
  അല്ലെങ്കിൽ
    pos[j].x_offset = hb_saturate_neg (pos[i].x_offset);

  pos[j].attach_chain() = reversed_chain;
  pos[j].attach_type() = type;
}


സ്ഥിതം വരിയിൽ തർക്കം
apply_cursive_attachment (hb_ot_apply_context_t *c,
                          സ്ഥിരം Anchor &exit_anchor,
                          സ്ഥിരം Anchor &entry_anchor,
                          unsigned i)
{
  hb_buffer_t *buffer = c->buffer;
  unsigned പൂർണ്ണസംഖ്യ j = buffer->idx;

  എങ്കിൽ (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
  {
    c->buffer->message (c->font,
			  "cursive attaching glyph at %u to glyph at %u",
			  i, j);
  }

  buffer->unsafe_to_break (i, j + 1);
  ദശാംശം entry_x, entry_y, exit_x, exit_y;
  exit_anchor.get_anchor (c, buffer->info[i].codepoint, &exit_x, &exit_y);
  entry_anchor.get_anchor (c, buffer->info[j].codepoint, &entry_x, &entry_y);

  hb_glyph_position_t *pos = buffer->pos;

  hb_position_t d;
  /* Main-direction adjustment */
  തിരഞ്ഞെടുപ്പ് (c->direction) {
    അവസ്ഥ HB_DIRECTION_LTR:
      pos[i].x_advance = hb_saturate_add ((hb_position_t) roundf (exit_x), pos[i].x_offset);

      d = hb_saturate_add ((hb_position_t) roundf (entry_x), pos[j].x_offset);
      pos[j].x_advance = hb_saturate_sub (pos[j].x_advance, d);
      pos[j].x_offset = hb_saturate_sub (pos[j].x_offset, d);
      നിർത്തുക;
    അവസ്ഥ HB_DIRECTION_RTL:
      d = hb_saturate_add ((hb_position_t) roundf (exit_x), pos[i].x_offset);
      pos[i].x_advance = hb_saturate_sub (pos[i].x_advance, d);
      pos[i].x_offset = hb_saturate_sub (pos[i].x_offset, d);

      pos[j].x_advance = hb_saturate_add ((hb_position_t) roundf (entry_x), pos[j].x_offset);
      നിർത്തുക;
    അവസ്ഥ HB_DIRECTION_TTB:
      pos[i].y_advance = hb_saturate_add ((hb_position_t) roundf (exit_y), pos[i].y_offset);

      d = hb_saturate_add ((hb_position_t) roundf (entry_y), pos[j].y_offset);
      pos[j].y_advance = hb_saturate_sub (pos[j].y_advance, d);
      pos[j].y_offset = hb_saturate_sub (pos[j].y_offset, d);
      നിർത്തുക;
    അവസ്ഥ HB_DIRECTION_BTT:
      d = hb_saturate_add ((hb_position_t) roundf (exit_y), pos[i].y_offset);
      pos[i].y_advance = hb_saturate_sub (pos[i].y_advance, d);
      pos[i].y_offset = hb_saturate_sub (pos[i].y_offset, d);

      pos[j].y_advance = (hb_position_t) roundf (entry_y);
      നിർത്തുക;
    അവസ്ഥ HB_DIRECTION_INVALID:
    സ്ഥിരസ്ഥിതി:
      നിർത്തുക;
  }

  /* Cross-direction adjustment */

  /* We attach child to parent (think graph theory and rooted trees whereas
   * the root stays on baseline and each node aligns itself against its
   * parent.
   *
   * Optimize things for the case of RightToLeft, as that's most common in
   * Arabic. */
  unsigned പൂർണ്ണസംഖ്യ child  = i;
  unsigned പൂർണ്ണസംഖ്യ parent = j;
  hb_position_t x_offset = (hb_position_t) roundf (entry_x - exit_x);
  hb_position_t y_offset = (hb_position_t) roundf (entry_y - exit_y);
  എങ്കിൽ  (!(c->lookup_props & LookupFlag::വലത്_To_ഇടത്))
  {
    unsigned പൂർണ്ണസംഖ്യ k = child;
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

  pos[child].attach_chain() = (പൂർണ്ണസംഖ്യ) parent - (പൂർണ്ണസംഖ്യ) child;
  എങ്കിൽ (pos[child].attach_chain() != (പൂർണ്ണസംഖ്യ) parent - (പൂർണ്ണസംഖ്യ) child)
  {
    pos[child].attach_chain() = 0;
    പോകുക overflow;
  }
  pos[child].attach_type() = ATTACH_TYPE_CURSIVE;
  buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT;
  എങ്കിൽ (likely (HB_DIRECTION_IS_HORIZONTAL (c->direction)))
    pos[child].y_offset = y_offset;
  അല്ലെങ്കിൽ
    pos[child].x_offset = x_offset;

  /* If parent was attached to child, separate them.
   * https://github.com/harfbuzz/harfbuzz/issues/2469
   */
  എങ്കിൽ (unlikely (pos[parent].attach_chain() == -pos[child].attach_chain()))
  {
    pos[parent].attach_chain() = 0;
    എങ്കിൽ (likely (HB_DIRECTION_IS_HORIZONTAL (c->direction)))
	pos[parent].y_offset = 0;
    അല്ലെങ്കിൽ
	pos[parent].x_offset = 0;
  }

  എങ്കിൽ (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
  {
    c->buffer->message (c->font,
			  "cursive attached glyph at %u to glyph at %u",
			  i, j);
  }

overflow:
  buffer->idx++;
  തിരികെ സത്യം;
}

template <typename Types>
ഘടന CursivePosFormat1_2
{

  സംരക്ഷിത:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  typename Types::template LOffsetTo<Coverage>
                coverage;               /* Offset to Coverage table--from beginning of subtable */
  typename Types::template ArrayOf<EntryExitRecord<Types>>
                entryExitRecord;        /* Array of EntryExit records--in
                                         * Coverage Index order */
  പൊതു:
  DEFINE_SIZE_ARRAY (2 + Types::LOffset::static_size + Types::HBUINT::static_size, entryExitRecord);

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    എങ്കിൽ (unlikely (!coverage.sanitize (c, ഇത്)))
      return_trace (അസത്യം);

    എങ്കിൽ (c->lazy_some_gpos)
      return_trace (entryExitRecord.sanitize_shallow (c));
    അല്ലെങ്കിൽ
      return_trace (entryExitRecord.sanitize (c, ഇത്));
  }

  തർക്കം intersects (സ്ഥിരം hb_set_t *glyphs) സ്ഥിരം
  { തിരികെ (ഇത്+coverage).intersects (glyphs); }

  ശൂന്യം closure_lookups (hb_closure_lookups_context_t *c) സ്ഥിരം {}

  ശൂന്യം collect_variation_indices (hb_collect_variation_indices_context_t *c) സ്ഥിരം
  {
    + hb_zip (ഇത്+coverage, entryExitRecord)
    | hb_filter (c->glyph_set, hb_first)
    | hb_map (hb_second)
    | hb_apply ([&] (സ്ഥിരം EntryExitRecord<Types>& record) { record.collect_variation_indices (c, ഇത്); })
    ;
  }

  ശൂന്യം collect_glyphs (hb_collect_glyphs_context_t *c) സ്ഥിരം
  { എങ്കിൽ (unlikely (!(ഇത്+coverage).collect_coverage (c->നിവേശം))) തിരികെ; }

  ശൂന്യം collect_second_glyphs (hb_set_digest_t *digest) സ്ഥിരം
  { (ഇത്+coverage).collect_coverage (digest); }

  സ്ഥിരം Coverage &get_coverage () സ്ഥിരം { തിരികെ ഇത്+coverage; }

  തർക്കം apply (hb_ot_apply_context_t *c) സ്ഥിരം
  {
    TRACE_APPLY (ഇത്);
    hb_buffer_t *buffer = c->buffer;

    സ്ഥിരം EntryExitRecord<Types> &this_record = entryExitRecord[(ഇത്+coverage).get_coverage  (buffer->cur().codepoint)];
    എങ്കിൽ (!this_record.entryAnchor ||
	unlikely (!this_record.entryAnchor.sanitize (&c->sanitizer, ഇത്))) return_trace (അസത്യം);
    hb_barrier ();

    auto &skippy_iter = c->iter_നിവേശം;
    skippy_iter.reset_fast (buffer->idx);
    unsigned unsafe_from;
    എങ്കിൽ (unlikely (!skippy_iter.prev (&unsafe_from)))
    {
      buffer->unsafe_to_concat_from_outbuffer (unsafe_from, buffer->idx + 1);
      return_trace (അസത്യം);
    }

    സ്ഥിരം EntryExitRecord<Types> &prev_record = entryExitRecord[(ഇത്+coverage).get_coverage  (buffer->info[skippy_iter.idx].codepoint)];
    എങ്കിൽ (!prev_record.exitAnchor ||
	unlikely (!prev_record.exitAnchor.sanitize (&c->sanitizer, ഇത്)))
    {
      buffer->unsafe_to_concat_from_outbuffer (skippy_iter.idx, buffer->idx + 1);
      return_trace (അസത്യം);
    }
    hb_barrier ();

    return_trace (apply_cursive_attachment (c,
                                            ഇത്+prev_record.exitAnchor,
                                            ഇത്+this_record.entryAnchor,
                                            skippy_iter.idx));
  }

  template <typename Iterator,
            hb_requires (hb_is_iterator (Iterator))>
  ശൂന്യം serialize (hb_subset_context_t *c,
                  Iterator it,
                  സ്ഥിരം ഘടന CursivePosFormat1_2<Types> *src_base)
  {
    എങ്കിൽ (unlikely (!c->serializer->extend_min ((*ഇത്)))) തിരികെ;
    ഇത്->format = src_base->format;
    ഇത്->entryExitRecord.len = it.len ();

    വേണ്ടി (സ്ഥിരം EntryExitRecord<Types>& entry_record : + it
                                                      | hb_map (hb_second))
      entry_record.subset (c, src_base);

    auto glyphs =
    + it
    | hb_map_retains_sorting (hb_first)
    ;

    coverage.serialize_serialize (c->serializer, glyphs);
  }

  തർക്കം subset (hb_subset_context_t *c) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);
    സ്ഥിരം hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    സ്ഥിരം hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*ഇത്);

    auto it =
    + hb_zip (ഇത്+coverage, entryExitRecord)
    | hb_filter (glyphset, hb_first)
    | hb_map_retains_sorting ([&] (hb_pair_t<hb_codepoint_t, സ്ഥിരം EntryExitRecord<Types>&> p)
                              -> hb_pair_t<hb_codepoint_t, സ്ഥിരം EntryExitRecord<Types>&>
                              { തിരികെ hb_pair (glyph_map[p.first], p.second);})
    ;

    തർക്കം ret = തർക്കം (it);
    out->serialize (c, it, ഇത്);
    return_trace (ret);
  }
};


}
}
}

#endif /* OT_LAYOUT_GPOS_CURSIVEPOSFORMAT1_HH */
