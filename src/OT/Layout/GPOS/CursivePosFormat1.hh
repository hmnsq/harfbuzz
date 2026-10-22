#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ OT_LAYOUT_GPOS_CURSIVEPOSFORMAT1_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OT_LAYOUT_GPOS_CURSIVEPOSFORMAT1_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "Anchor.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "../../../hb-limits.hh"

ਨਾਮਾਕਾਸ਼ OT {
ਨਾਮਾਕਾਸ਼ Layout {
ਨਾਮਾਕਾਸ਼ GPOS_impl {

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Types>
ਬਣਤਰ CursivePosFormat1_2;

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Types>
ਬਣਤਰ ਇੰਦਰਾਜ_Exit_ਰਿਕਾਰਡ
{
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Types2> ਮਿੱਤਰ ਬਣਤਰ CursivePosFormat1_2;

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c, ਅਚਲ ਬਣਤਰ CursivePosFormat1_2<Types> *ਅਧਾਰ) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (ਇੰਦਰਾਜ_Anchor.sanitize (c, ਅਧਾਰ) && exitAnchor.sanitize (c, ਅਧਾਰ));
  }

  ਖਾਲੀ collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  ਅਚਲ ਬਣਤਰ CursivePosFormat1_2<Types> *src_ਅਧਾਰ) ਅਚਲ
  {
    (src_ਅਧਾਰ+ਇੰਦਰਾਜ_Anchor).collect_variation_indices (c);
    (src_ਅਧਾਰ+exitAnchor).collect_variation_indices (c);
  }

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c,
	       ਅਚਲ ਬਣਤਰ CursivePosFormat1_2<Types> *src_ਅਧਾਰ) ਅਚਲ
  {
    TRACE_SERIALIZE (ਇਹ);
    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->serializer->embed (ਇਹ);
    ਜੇ (unlikely (!ਬਾਹਰਲਾ)) return_trace (ਝੂਠ);

    ਤਾਰਕਿਕ ret = ਝੂਠ;
    ret |= ਬਾਹਰਲਾ->ਇੰਦਰਾਜ_Anchor.serialize_subset (c, ਇੰਦਰਾਜ_Anchor, src_ਅਧਾਰ);
    ret |= ਬਾਹਰਲਾ->exitAnchor.serialize_subset (c, exitAnchor, src_ਅਧਾਰ);
    return_trace (ret);
  }

  ਸੁਰੱਖਿਅਤ:
  ਕਿਸਮ_ਨਾਂ Types::ਸਾਂਚਾ ਵਿੱਥ_ਮੰਜ਼ਿਲ<Anchor, ਬਣਤਰ CursivePosFormat1_2<Types>>
                ਇੰਦਰਾਜ_Anchor;            /* Offset to EntryAnchor table--from
                                         * beginning of CursivePos
                                         * subtable--may be NULL */
  ਕਿਸਮ_ਨਾਂ Types::ਸਾਂਚਾ ਵਿੱਥ_ਮੰਜ਼ਿਲ<Anchor, ਬਣਤਰ CursivePosFormat1_2<Types>>
                exitAnchor;             /* Offset to ExitAnchor table--from
                                         * beginning of CursivePos
                                         * subtable--may be NULL */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (2 * Types::ਮਾਪ);
};

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
ਉਲਟ_cursive_minor_ਵਿੱਥ (hb_glyph_position_t *pos,
                              ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ len,
                              ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i,
                              hb_direction_t direction,
                              ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ new_ਮਾਪਾ,
                              ਬਿਨਾਨਿਸ਼ਾਨੀ nesting_level = 0)
{
  ਜੇ (nesting_level > HB_MAX_NESTING_LEVEL)
    ਵਾਪਸ;

  ਪੂਰਨ chain = pos[i].attach_chain(), type = pos[i].attach_type();
  ਜੇ (likely (!chain || 0 == (type & ATTACH_TYPE_CURSIVE)))
    ਵਾਪਸ;

  pos[i].attach_chain() = 0;

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ j = (ਪੂਰਨ) i + chain;

  ਜੇ (unlikely (j >= len))
    ਵਾਪਸ;

  /* Stop if we see new parent in the chain. */
  ਜੇ (j == new_ਮਾਪਾ)
    ਵਾਪਸ;

  int16_t reversed_chain = -chain;
  /* The old edge was cleared above; if the reversed distance truncates,
   * keep it detached instead of storing a poisoned chain.
   */
  ਜੇ (unlikely (reversed_chain != -chain))
    ਵਾਪਸ;

  ਉਲਟ_cursive_minor_ਵਿੱਥ (pos, len, j, direction, new_ਮਾਪਾ, nesting_level + 1);

  ਜੇ (HB_DIRECTION_IS_HORIZONTAL (direction))
    pos[j].y_ਵਿੱਥ_2 = hb_saturate_neg (pos[i].y_ਵਿੱਥ_2);
  ਨਹੀਂਤਾਂ
    pos[j].x_ਵਿੱਥ_2 = hb_saturate_neg (pos[i].x_ਵਿੱਥ_2);

  pos[j].attach_chain() = reversed_chain;
  pos[j].attach_type() = type;
}


ਸਥਿਰ ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ
apply_cursive_attachment (hb_ot_apply_context_t *c,
                          ਅਚਲ Anchor &exit_anchor,
                          ਅਚਲ Anchor &ਇੰਦਰਾਜ_anchor,
                          ਬਿਨਾਨਿਸ਼ਾਨੀ i)
{
  hb_ਬਫਰ_t *ਬਫਰ = c->ਬਫਰ;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ j = ਬਫਰ->idx;

  ਜੇ (HB_ਬਫਰ_ਸੁਨੇਹਾ_ਹੋਰ && c->ਬਫਰ->messaging ())
  {
    c->ਬਫਰ->ਸੁਨੇਹਾ (c->font,
			  "cursive attaching glyph at %u to glyph at %u",
			  i, j);
  }

  ਬਫਰ->unsafe_ਮੰਜ਼ਿਲ_break (i, j + 1);
  ਦਸ਼ਮਲਵ ਇੰਦਰਾਜ_x, ਇੰਦਰਾਜ_y, exit_x, exit_y;
  exit_anchor.get_anchor (c, ਬਫਰ->info[i].codepoint, &exit_x, &exit_y);
  ਇੰਦਰਾਜ_anchor.get_anchor (c, ਬਫਰ->info[j].codepoint, &ਇੰਦਰਾਜ_x, &ਇੰਦਰਾਜ_y);

  hb_glyph_position_t *pos = ਬਫਰ->pos;

  hb_position_t d;
  /* Main-direction adjustment */
  ਚੋਣ (c->direction) {
    ਹਾਲਤ HB_DIRECTION_LTR:
      pos[i].x_advance = hb_saturate_ਜੋੜੋ ((hb_position_t) roundf (exit_x), pos[i].x_ਵਿੱਥ_2);

      d = hb_saturate_ਜੋੜੋ ((hb_position_t) roundf (ਇੰਦਰਾਜ_x), pos[j].x_ਵਿੱਥ_2);
      pos[j].x_advance = hb_saturate_sub (pos[j].x_advance, d);
      pos[j].x_ਵਿੱਥ_2 = hb_saturate_sub (pos[j].x_ਵਿੱਥ_2, d);
      ਰੋਕੋ;
    ਹਾਲਤ HB_DIRECTION_RTL:
      d = hb_saturate_ਜੋੜੋ ((hb_position_t) roundf (exit_x), pos[i].x_ਵਿੱਥ_2);
      pos[i].x_advance = hb_saturate_sub (pos[i].x_advance, d);
      pos[i].x_ਵਿੱਥ_2 = hb_saturate_sub (pos[i].x_ਵਿੱਥ_2, d);

      pos[j].x_advance = hb_saturate_ਜੋੜੋ ((hb_position_t) roundf (ਇੰਦਰਾਜ_x), pos[j].x_ਵਿੱਥ_2);
      ਰੋਕੋ;
    ਹਾਲਤ HB_DIRECTION_TTB:
      pos[i].y_advance = hb_saturate_ਜੋੜੋ ((hb_position_t) roundf (exit_y), pos[i].y_ਵਿੱਥ_2);

      d = hb_saturate_ਜੋੜੋ ((hb_position_t) roundf (ਇੰਦਰਾਜ_y), pos[j].y_ਵਿੱਥ_2);
      pos[j].y_advance = hb_saturate_sub (pos[j].y_advance, d);
      pos[j].y_ਵਿੱਥ_2 = hb_saturate_sub (pos[j].y_ਵਿੱਥ_2, d);
      ਰੋਕੋ;
    ਹਾਲਤ HB_DIRECTION_BTT:
      d = hb_saturate_ਜੋੜੋ ((hb_position_t) roundf (exit_y), pos[i].y_ਵਿੱਥ_2);
      pos[i].y_advance = hb_saturate_sub (pos[i].y_advance, d);
      pos[i].y_ਵਿੱਥ_2 = hb_saturate_sub (pos[i].y_ਵਿੱਥ_2, d);

      pos[j].y_advance = (hb_position_t) roundf (ਇੰਦਰਾਜ_y);
      ਰੋਕੋ;
    ਹਾਲਤ HB_DIRECTION_INVALID:
    ਮੂਲ:
      ਰੋਕੋ;
  }

  /* Cross-direction adjustment */

  /* We attach child to parent (think graph theory and rooted trees whereas
   * the root stays on baseline and each node aligns itself against its
   * parent.
   *
   * Optimize things for the case of RightToLeft, as that's most common in
   * Arabic. */
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਬੱਚਾ  = i;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਮਾਪਾ = j;
  hb_position_t x_ਵਿੱਥ_2 = (hb_position_t) roundf (ਇੰਦਰਾਜ_x - exit_x);
  hb_position_t y_ਵਿੱਥ_2 = (hb_position_t) roundf (ਇੰਦਰਾਜ_y - exit_y);
  ਜੇ  (!(c->lookup_props & LookupFlag::ਸੱਜਾ_ਮੰਜ਼ਿਲ_ਖੱਬਾ))
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ k = ਬੱਚਾ;
    ਬੱਚਾ = ਮਾਪਾ;
    ਮਾਪਾ = k;
    x_ਵਿੱਥ_2 = -x_ਵਿੱਥ_2;
    y_ਵਿੱਥ_2 = -y_ਵਿੱਥ_2;
  }

  /* If child was already connected to someone else, walk through its old
   * chain and reverse the link direction, such that the whole tree of its
   * previous connection now attaches to new parent.  Watch out for case
   * where new parent is on the path from old chain...
   */
  ਉਲਟ_cursive_minor_ਵਿੱਥ (pos, ਬਫਰ->len, ਬੱਚਾ, c->direction, ਮਾਪਾ);

  pos[ਬੱਚਾ].attach_chain() = (ਪੂਰਨ) ਮਾਪਾ - (ਪੂਰਨ) ਬੱਚਾ;
  ਜੇ (pos[ਬੱਚਾ].attach_chain() != (ਪੂਰਨ) ਮਾਪਾ - (ਪੂਰਨ) ਬੱਚਾ)
  {
    pos[ਬੱਚਾ].attach_chain() = 0;
    ਜਾਓ ਉਛਾਲ;
  }
  pos[ਬੱਚਾ].attach_type() = ATTACH_TYPE_CURSIVE;
  ਬਫਰ->scratch_flags |= HB_ਬਫਰ_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT;
  ਜੇ (likely (HB_DIRECTION_IS_HORIZONTAL (c->direction)))
    pos[ਬੱਚਾ].y_ਵਿੱਥ_2 = y_ਵਿੱਥ_2;
  ਨਹੀਂਤਾਂ
    pos[ਬੱਚਾ].x_ਵਿੱਥ_2 = x_ਵਿੱਥ_2;

  /* If parent was attached to child, separate them.
   * https://github.com/harfbuzz/harfbuzz/issues/2469
   */
  ਜੇ (unlikely (pos[ਮਾਪਾ].attach_chain() == -pos[ਬੱਚਾ].attach_chain()))
  {
    pos[ਮਾਪਾ].attach_chain() = 0;
    ਜੇ (likely (HB_DIRECTION_IS_HORIZONTAL (c->direction)))
	pos[ਮਾਪਾ].y_ਵਿੱਥ_2 = 0;
    ਨਹੀਂਤਾਂ
	pos[ਮਾਪਾ].x_ਵਿੱਥ_2 = 0;
  }

  ਜੇ (HB_ਬਫਰ_ਸੁਨੇਹਾ_ਹੋਰ && c->ਬਫਰ->messaging ())
  {
    c->ਬਫਰ->ਸੁਨੇਹਾ (c->font,
			  "cursive attached glyph at %u to glyph at %u",
			  i, j);
  }

ਉਛਾਲ:
  ਬਫਰ->idx++;
  ਵਾਪਸ ਸੱਚ;
}

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Types>
ਬਣਤਰ CursivePosFormat1_2
{

  ਸੁਰੱਖਿਅਤ:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  ਕਿਸਮ_ਨਾਂ Types::ਸਾਂਚਾ LOffset_ਮੰਜ਼ਿਲ<Coverage>
                coverage;               /* Offset to Coverage table--from beginning of subtable */
  ਕਿਸਮ_ਨਾਂ Types::ਸਾਂਚਾ Array_ਦਾ<ਇੰਦਰਾਜ_Exit_ਰਿਕਾਰਡ<Types>>
                ਇੰਦਰਾਜ_Exit_ਰਿਕਾਰਡ_2;        /* Array of EntryExit records--in
                                         * Coverage Index order */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_ARRAY (2 + Types::LOffset::static_ਮਾਪ + Types::HBUINT::static_ਮਾਪ, ਇੰਦਰਾਜ_Exit_ਰਿਕਾਰਡ_2);

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    ਜੇ (unlikely (!coverage.sanitize (c, ਇਹ)))
      return_trace (ਝੂਠ);

    ਜੇ (c->lazy_some_gpos)
      return_trace (ਇੰਦਰਾਜ_Exit_ਰਿਕਾਰਡ_2.sanitize_shallow (c));
    ਨਹੀਂਤਾਂ
      return_trace (ਇੰਦਰਾਜ_Exit_ਰਿਕਾਰਡ_2.sanitize (c, ਇਹ));
  }

  ਤਾਰਕਿਕ intersects (ਅਚਲ hb_ਸੈੱਟ_t *glyphs) ਅਚਲ
  { ਵਾਪਸ (ਇਹ+coverage).intersects (glyphs); }

  ਖਾਲੀ ਘੇਰਾ_lookups (hb_ਘੇਰਾ_lookups_context_t *c) ਅਚਲ {}

  ਖਾਲੀ collect_variation_indices (hb_collect_variation_indices_context_t *c) ਅਚਲ
  {
    + hb_zip (ਇਹ+coverage, ਇੰਦਰਾਜ_Exit_ਰਿਕਾਰਡ_2)
    | hb_filter (c->glyph_ਸੈੱਟ, hb_ਪਹਿਲਾ)
    | hb_map (hb_ਦੂਜਾ)
    | hb_apply ([&] (ਅਚਲ ਇੰਦਰਾਜ_Exit_ਰਿਕਾਰਡ<Types>& ਰਿਕਾਰਡ) { ਰਿਕਾਰਡ.collect_variation_indices (c, ਇਹ); })
    ;
  }

  ਖਾਲੀ collect_glyphs (hb_collect_glyphs_context_t *c) ਅਚਲ
  { ਜੇ (unlikely (!(ਇਹ+coverage).collect_coverage (c->ਨਿਵੇਸ਼))) ਵਾਪਸ; }

  ਖਾਲੀ collect_ਦੂਜਾ_glyphs (hb_ਸੈੱਟ_digest_t *digest) ਅਚਲ
  { (ਇਹ+coverage).collect_coverage (digest); }

  ਅਚਲ Coverage &get_coverage () ਅਚਲ { ਵਾਪਸ ਇਹ+coverage; }

  ਤਾਰਕਿਕ apply (hb_ot_apply_context_t *c) ਅਚਲ
  {
    TRACE_APPLY (ਇਹ);
    hb_ਬਫਰ_t *ਬਫਰ = c->ਬਫਰ;

    ਅਚਲ ਇੰਦਰਾਜ_Exit_ਰਿਕਾਰਡ<Types> &this_ਰਿਕਾਰਡ = ਇੰਦਰਾਜ_Exit_ਰਿਕਾਰਡ_2[(ਇਹ+coverage).get_coverage  (ਬਫਰ->cur().codepoint)];
    ਜੇ (!this_ਰਿਕਾਰਡ.ਇੰਦਰਾਜ_Anchor ||
	unlikely (!this_ਰਿਕਾਰਡ.ਇੰਦਰਾਜ_Anchor.sanitize (&c->sanitizer, ਇਹ))) return_trace (ਝੂਠ);
    hb_barrier ();

    ਸਵੈਚਾਲਿਤ &skippy_iter = c->iter_ਨਿਵੇਸ਼;
    skippy_iter.reset_fast (ਬਫਰ->idx);
    ਬਿਨਾਨਿਸ਼ਾਨੀ unsafe_from;
    ਜੇ (unlikely (!skippy_iter.prev (&unsafe_from)))
    {
      ਬਫਰ->unsafe_ਮੰਜ਼ਿਲ_concat_from_outbuffer (unsafe_from, ਬਫਰ->idx + 1);
      return_trace (ਝੂਠ);
    }

    ਅਚਲ ਇੰਦਰਾਜ_Exit_ਰਿਕਾਰਡ<Types> &prev_ਰਿਕਾਰਡ = ਇੰਦਰਾਜ_Exit_ਰਿਕਾਰਡ_2[(ਇਹ+coverage).get_coverage  (ਬਫਰ->info[skippy_iter.idx].codepoint)];
    ਜੇ (!prev_ਰਿਕਾਰਡ.exitAnchor ||
	unlikely (!prev_ਰਿਕਾਰਡ.exitAnchor.sanitize (&c->sanitizer, ਇਹ)))
    {
      ਬਫਰ->unsafe_ਮੰਜ਼ਿਲ_concat_from_outbuffer (skippy_iter.idx, ਬਫਰ->idx + 1);
      return_trace (ਝੂਠ);
    }
    hb_barrier ();

    return_trace (apply_cursive_attachment (c,
                                            ਇਹ+prev_ਰਿਕਾਰਡ.exitAnchor,
                                            ਇਹ+this_ਰਿਕਾਰਡ.ਇੰਦਰਾਜ_Anchor,
                                            skippy_iter.idx));
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Iterator,
            hb_requires (hb_is_iterator (Iterator))>
  ਖਾਲੀ serialize (hb_subset_context_t *c,
                  Iterator it,
                  ਅਚਲ ਬਣਤਰ CursivePosFormat1_2<Types> *src_ਅਧਾਰ)
  {
    ਜੇ (unlikely (!c->serializer->extend_min ((*ਇਹ)))) ਵਾਪਸ;
    ਇਹ->format = src_ਅਧਾਰ->format;
    ਇਹ->ਇੰਦਰਾਜ_Exit_ਰਿਕਾਰਡ_2.len = it.len ();

    ਲਈ (ਅਚਲ ਇੰਦਰਾਜ_Exit_ਰਿਕਾਰਡ<Types>& ਇੰਦਰਾਜ_ਰਿਕਾਰਡ : + it
                                                      | hb_map (hb_ਦੂਜਾ))
      ਇੰਦਰਾਜ_ਰਿਕਾਰਡ.subset (c, src_ਅਧਾਰ);

    ਸਵੈਚਾਲਿਤ glyphs =
    + it
    | hb_map_retains_ਛਾਂਟੀ (hb_ਪਹਿਲਾ)
    ;

    coverage.serialize_serialize (c->serializer, glyphs);
  }

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਅਚਲ hb_ਸੈੱਟ_t &glyphset = *c->plan->glyphset_gsub ();
    ਅਚਲ hb_map_t &glyph_map = *c->plan->glyph_map;

    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->serializer->ਸ਼ੁਰੂ_embed (*ਇਹ);

    ਸਵੈਚਾਲਿਤ it =
    + hb_zip (ਇਹ+coverage, ਇੰਦਰਾਜ_Exit_ਰਿਕਾਰਡ_2)
    | hb_filter (glyphset, hb_ਪਹਿਲਾ)
    | hb_map_retains_ਛਾਂਟੀ ([&] (hb_pair_t<hb_codepoint_t, ਅਚਲ ਇੰਦਰਾਜ_Exit_ਰਿਕਾਰਡ<Types>&> p)
                              -> hb_pair_t<hb_codepoint_t, ਅਚਲ ਇੰਦਰਾਜ_Exit_ਰਿਕਾਰਡ<Types>&>
                              { ਵਾਪਸ hb_pair (glyph_map[p.ਪਹਿਲਾ], p.second);})
    ;

    ਤਾਰਕਿਕ ret = ਤਾਰਕਿਕ (it);
    ਬਾਹਰਲਾ->serialize (c, it, ਇਹ);
    return_trace (ret);
  }
};


}
}
}

#ਜੇ_ਅੰਤ /* OT_LAYOUT_GPOS_CURSIVEPOSFORMAT1_HH */
