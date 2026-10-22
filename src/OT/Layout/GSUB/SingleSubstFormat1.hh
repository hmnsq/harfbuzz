#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ OT_LAYOUT_GSUB_SINGLESUBSTFORMAT1_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OT_LAYOUT_GSUB_SINGLESUBSTFORMAT1_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "Common.hh"

ਨਾਮਾਕਾਸ਼ OT {
ਨਾਮਾਕਾਸ਼ Layout {
ਨਾਮਾਕਾਸ਼ GSUB_impl {

ਸਥਿਰ ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ
subset_single_subst (hb_subset_context_t *c,
                     ਅਚਲ Coverage &coverage,
                     hb_codepoint_t d,
                     hb_codepoint_t ਨਕਾਬ)
{
  ਅਚਲ hb_ਸੈੱਟ_t &glyphset = *c->plan->glyphset_gsub ();
  ਅਚਲ hb_map_t &glyph_map = *c->plan->glyph_map;

  hb_ਸੈੱਟ_t intersection;
  coverage.intersect_ਸੈੱਟ (glyphset, intersection);

  ਸਵੈਚਾਲਿਤ it =
  + hb_iter (intersection)
  | hb_map_retains_ਛਾਂਟੀ ([d, ਨਕਾਬ] (hb_codepoint_t g) {
                              ਵਾਪਸ hb_codepoint_pair_t (g,
                                                          (g + d) & ਨਕਾਬ); })
  | hb_filter (glyphset, hb_ਦੂਜਾ)
  | hb_map_retains_ਛਾਂਟੀ ([&] (hb_codepoint_pair_t p) -> hb_codepoint_pair_t
                            { ਵਾਪਸ hb_pair (glyph_map[p.ਪਹਿਲਾ], glyph_map[p.second]); })
  ;

  ਤਾਰਕਿਕ ret = ਤਾਰਕਿਕ (it);
  SingleSubst_serialize (c->serializer, it);
  ਵਾਪਸ ret;
}

HB_ਅੰਦਰੂਨੀ ਸਤਰਅੰਦਰ
ਖਾਲੀ single_subst_depend (hb_depend_context_t *c,
                          ਅਚਲ Coverage &coverage,
                          hb_codepoint_t d, hb_codepoint_t ਨਕਾਬ)
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_glyphs = c->face->get_ਅੰਕ_glyphs ();

  // Filter coverage by active glyphs (like closure does)
  hb_ਸੈੱਟ_t intersection;
  coverage.intersect_ਸੈੱਟ (c->ਮਾਪਾ_ਸਰਗਰਮ_glyphs (), intersection);

  + hb_iter (intersection)
  | hb_map ([&] (hb_codepoint_t _) { ਵਾਪਸ hb_codepoint_pair_t (_, (_ + d) & ਨਕਾਬ); })
  | hb_filter ([ਅੰਕ_glyphs] (ਅਚਲ hb_codepoint_pair_t &_) { ਵਾਪਸ _.second < ਅੰਕ_glyphs; })
  | hb_apply ([&] (ਅਚਲ hb_codepoint_pair_t &_) { c->depend_ਅੰਕੜੇ->ਜੋੜੋ_gsub_lookup(_.ਪਹਿਲਾ, c->lookup_ਸੂਚਕਾਂਕ, _.second); })
  ;
}

HB_ਅੰਦਰੂਨੀ ਸਤਰਅੰਦਰ
ਖਾਲੀ single_subst_ਘੇਰਾ (hb_ਘੇਰਾ_context_t *c,
                           ਅਚਲ Coverage &coverage,
                           hb_codepoint_t d, hb_codepoint_t ਨਕਾਬ)
{
  /* Help fuzzer avoid this function as much. */
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਕੱਢੋ = coverage.get_population ();
  ਜੇ (ਕੱਢੋ >= ਨਕਾਬ)
    ਵਾਪਸ;

  hb_ਸੈੱਟ_t intersection;
  coverage.intersect_ਸੈੱਟ (c->ਮਾਪਾ_ਸਰਗਰਮ_glyphs (), intersection);

  /* In degenerate fuzzer-found fonts, but not real fonts,
   * this table can keep adding new glyphs in each round of closure.
   * Refuse to close-over, if it maps glyph range to overlapping range. */
  hb_codepoint_t min_before = intersection.get_min ();
  hb_codepoint_t max_before = intersection.get_max ();
  hb_codepoint_t min_after = (min_before + d) & ਨਕਾਬ;
  hb_codepoint_t max_after = (max_before + d) & ਨਕਾਬ;
  ਜੇ (intersection.get_population () == max_before - min_before + 1 &&
	((min_before <= min_after && min_after <= max_before) ||
	 (min_before <= max_after && max_after <= max_before)))
    ਵਾਪਸ;

  + hb_iter (intersection)
  | hb_map ([d, ਨਕਾਬ] (hb_codepoint_t g) { ਵਾਪਸ (g + d) & ਨਕਾਬ; })
  | hb_sink (c->output)
  ;
}

HB_ਅੰਦਰੂਨੀ ਸਤਰਅੰਦਰ
ਖਾਲੀ single_subst_collect_glyphs (hb_collect_glyphs_context_t *c,
                                  ਅਚਲ Coverage &coverage,
                                  hb_codepoint_t d, hb_codepoint_t ਨਕਾਬ)
{
  ਜੇ (unlikely (!coverage.collect_coverage (c->ਨਿਵੇਸ਼))) ਵਾਪਸ;

  + hb_iter (coverage)
  | hb_map ([d, ਨਕਾਬ] (hb_codepoint_t g) { ਵਾਪਸ (g + d) & ਨਕਾਬ; })
  | hb_sink (c->output)
  ;
}

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Types>
ਬਣਤਰ SingleSubstFormat1_3
{
  ਸੁਰੱਖਿਅਤ:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  ਕਿਸਮ_ਨਾਂ Types::ਸਾਂਚਾ LOffset_ਮੰਜ਼ਿਲ<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of Substitution table */
  ਕਿਸਮ_ਨਾਂ Types::HBUINT
                deltaGlyphID;           /* Add to original GlyphID to get
                                         * substitute GlyphID, modulo 0x10000 */

  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (2 + Types::LOffset::static_ਮਾਪ + Types::HBUINT::static_ਮਾਪ);

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ) &&
                  coverage.sanitize (c, ਇਹ) &&
                  /* The coverage  table may use a range to represent a set
                   * of glyphs, which means a small number of bytes can
                   * generate a large glyph set. Manually modify the
                   * sanitizer max ops to take this into account.
                   *
                   * Note: This check *must* be right after coverage sanitize. */
                  c->check_ops ((ਇਹ + coverage).get_population () >> 1));
  }

  hb_codepoint_t get_ਨਕਾਬ () ਅਚਲ
  { ਵਾਪਸ (1 << (8 * Types::ਮਾਪ)) - 1; }

  ਤਾਰਕਿਕ intersects (ਅਚਲ hb_ਸੈੱਟ_t *glyphs) ਅਚਲ
  { ਵਾਪਸ (ਇਹ+coverage).intersects (glyphs); }

  ਤਾਰਕਿਕ may_have_non_1to1 () ਅਚਲ
  { ਵਾਪਸ ਝੂਠ; }

  ਖਾਲੀ depend (hb_depend_context_t *c) ਅਚਲ
  {
    single_subst_depend (c, ਇਹ+coverage, deltaGlyphID, get_ਨਕਾਬ ());
  }

  ਖਾਲੀ ਘੇਰਾ (hb_ਘੇਰਾ_context_t *c) ਅਚਲ
  {
    single_subst_ਘੇਰਾ (c, ਇਹ+coverage, deltaGlyphID, get_ਨਕਾਬ ());
  }

  ਖਾਲੀ ਘੇਰਾ_lookups (hb_ਘੇਰਾ_lookups_context_t *c) ਅਚਲ {}

  ਖਾਲੀ collect_glyphs (hb_collect_glyphs_context_t *c) ਅਚਲ
  {
    single_subst_collect_glyphs (c, ਇਹ+coverage, deltaGlyphID, get_ਨਕਾਬ ());
  }

  ਅਚਲ Coverage &get_coverage () ਅਚਲ { ਵਾਪਸ ਇਹ+coverage; }

  ਤਾਰਕਿਕ would_apply (hb_would_apply_context_t *c) ਅਚਲ
  { ਵਾਪਸ c->len == 1 && (ਇਹ+coverage).get_coverage (c->glyphs[0]) != NOT_COVERED; }

  ਬਿਨਾਨਿਸ਼ਾਨੀ
  get_glyph_alternates (hb_codepoint_t  glyph_id,
                        ਬਿਨਾਨਿਸ਼ਾਨੀ        ਸ਼ੁਰੂ_ਵਿੱਥ,
                        ਬਿਨਾਨਿਸ਼ਾਨੀ       *alternate_ਗਿਣਤੀ  /* IN/OUT.  May be NULL. */,
                        hb_codepoint_t *alternate_glyphs /* OUT.     May be NULL. */) ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸੂਚਕਾਂਕ_2 = (ਇਹ+coverage).get_coverage (glyph_id);
    ਜੇ (likely (ਸੂਚਕਾਂਕ_2 == NOT_COVERED))
    {
      ਜੇ (alternate_ਗਿਣਤੀ)
        *alternate_ਗਿਣਤੀ = 0;
      ਵਾਪਸ 0;
    }

    ਜੇ (alternate_ਗਿਣਤੀ && *alternate_ਗਿਣਤੀ && alternate_glyphs)
    {
      hb_codepoint_t d = deltaGlyphID;
      hb_codepoint_t ਨਕਾਬ = get_ਨਕਾਬ ();

      glyph_id = (glyph_id + d) & ਨਕਾਬ;

      *alternate_glyphs = glyph_id;
      *alternate_ਗਿਣਤੀ = 1;
    }

    ਵਾਪਸ 1;
  }

  ਖਾਲੀ
  collect_glyph_alternates (hb_map_t  *alternate_ਗਿਣਤੀ /* IN/OUT */,
			    hb_map_t  *alternate_glyphs /* IN/OUT */) ਅਚਲ
  {
    hb_codepoint_t d = deltaGlyphID;
    hb_codepoint_t ਨਕਾਬ = get_ਨਕਾਬ ();

    + hb_iter (ਇਹ+coverage)
    | hb_map ([d, ਨਕਾਬ] (hb_codepoint_t g) { ਵਾਪਸ hb_pair (g, (g + d) & ਨਕਾਬ); })
    | hb_apply ([&] (ਅਚਲ hb_pair_t<hb_codepoint_t, hb_codepoint_t> &p) -> ਖਾਲੀ
		{ hb_collect_glyph_alternates_ਜੋੜੋ (p.ਪਹਿਲਾ, p.second,
						    alternate_ਗਿਣਤੀ, alternate_glyphs); })
    ;
  }

  ਤਾਰਕਿਕ apply (hb_ot_apply_context_t *c) ਅਚਲ
  {
    TRACE_APPLY (ਇਹ);
    hb_codepoint_t glyph_id = c->ਬਫਰ->cur().codepoint;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸੂਚਕਾਂਕ_2 = (ਇਹ+coverage).get_coverage (glyph_id);
    ਜੇ (ਸੂਚਕਾਂਕ_2 == NOT_COVERED) return_trace (ਝੂਠ);

    hb_codepoint_t d = deltaGlyphID;
    hb_codepoint_t ਨਕਾਬ = get_ਨਕਾਬ ();

    glyph_id = (glyph_id + d) & ਨਕਾਬ;

    ਜੇ (HB_ਬਫਰ_ਸੁਨੇਹਾ_ਹੋਰ && c->ਬਫਰ->messaging ())
    {
      c->ਬਫਰ->sync_so_far ();
      c->ਬਫਰ->ਸੁਨੇਹਾ (c->font,
			  "replacing glyph at %u (single substitution)",
			  c->ਬਫਰ->idx);
    }

    c->replace_glyph (glyph_id);

    ਜੇ (HB_ਬਫਰ_ਸੁਨੇਹਾ_ਹੋਰ && c->ਬਫਰ->messaging ())
    {
      c->ਬਫਰ->ਸੁਨੇਹਾ (c->font,
			  "replaced glyph at %u (single substitution)",
			  c->ਬਫਰ->idx - 1u);
    }

    return_trace (ਸੱਚ);
  }

  ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ Iterator,
           hb_requires (hb_is_sorted_ਸਰੋਤ_ਦਾ (Iterator, hb_codepoint_t))>
  ਤਾਰਕਿਕ serialize (hb_serialize_context_t *c,
                  Iterator glyphs,
                  ਬਿਨਾਨਿਸ਼ਾਨੀ delta)
  {
    TRACE_SERIALIZE (ਇਹ);
    ਜੇ (unlikely (!c->extend_min (ਇਹ))) return_trace (ਝੂਠ);
    ਜੇ (unlikely (!coverage.serialize_serialize (c, glyphs))) return_trace (ਝੂਠ);
    c->check_ਸੌਂਪੋ (deltaGlyphID, delta, HB_SERIALIZE_ERROR_INT_ਉਛਾਲ);
    return_trace (ਸੱਚ);
  }

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    return_trace (subset_single_subst (c, ਇਹ+coverage, deltaGlyphID, get_ਨਕਾਬ ()));
  }
};

}
}
}


#ਜੇ_ਅੰਤ /* OT_LAYOUT_GSUB_SINGLESUBSTFORMAT1_HH */
