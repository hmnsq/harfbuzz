#నిర్వచితం_కాకపోతే OT_LAYOUT_GSUB_SINGLESUBSTFORMAT1_HH
#నిర్వచించు OT_LAYOUT_GSUB_SINGLESUBSTFORMAT1_HH

#అన్తర్భావ "Common.hh"

నామాకాశ OT {
నామాకాశ Layout {
నామాకాశ GSUB_impl {

స్థిర అంతఃవరుస తార్కిక
subset_single_subst (hb_subset_context_t *c,
                     స్థిరరాశి Coverage &coverage,
                     hb_codepoint_t d,
                     hb_codepoint_t ముసుగు)
{
  స్థిరరాశి hb_సమితి_t &glyphset = *c->plan->glyphset_gsub ();
  స్థిరరాశి hb_map_t &glyph_map = *c->plan->glyph_map;

  hb_సమితి_t intersection;
  coverage.intersect_సమితి (glyphset, intersection);

  స్వయంచాలక it =
  + hb_iter (intersection)
  | hb_map_retains_క్రమీకరణ ([d, ముసుగు] (hb_codepoint_t g) {
                              తిరిగి hb_codepoint_pair_t (g,
                                                          (g + d) & ముసుగు); })
  | hb_filter (glyphset, hb_రెండవ)
  | hb_map_retains_క్రమీకరణ ([&] (hb_codepoint_pair_t p) -> hb_codepoint_pair_t
                            { తిరిగి hb_pair (glyph_map[p.మొదటి], glyph_map[p.second]); })
  ;

  తార్కిక ret = తార్కిక (it);
  SingleSubst_serialize (c->serializer, it);
  తిరిగి ret;
}

HB_అంతర్గత అంతఃవరుస
శూన్య single_subst_depend (hb_depend_context_t *c,
                          స్థిరరాశి Coverage &coverage,
                          hb_codepoint_t d, hb_codepoint_t ముసుగు)
{
  గుర్తులేని అంకె_glyphs = c->face->get_అంకె_glyphs ();

  // Filter coverage by active glyphs (like closure does)
  hb_సమితి_t intersection;
  coverage.intersect_సమితి (c->మాతృ_చురుకైన_glyphs (), intersection);

  + hb_iter (intersection)
  | hb_map ([&] (hb_codepoint_t _) { తిరిగి hb_codepoint_pair_t (_, (_ + d) & ముసుగు); })
  | hb_filter ([అంకె_glyphs] (స్థిరరాశి hb_codepoint_pair_t &_) { తిరిగి _.second < అంకె_glyphs; })
  | hb_apply ([&] (స్థిరరాశి hb_codepoint_pair_t &_) { c->depend_దత్తాంశం->కూడు_gsub_lookup(_.మొదటి, c->lookup_సూచీ, _.second); })
  ;
}

HB_అంతర్గత అంతఃవరుస
శూన్య single_subst_సంవృతం (hb_సంవృతం_context_t *c,
                           స్థిరరాశి Coverage &coverage,
                           hb_codepoint_t d, hb_codepoint_t ముసుగు)
{
  /* Help fuzzer avoid this function as much. */
  గుర్తులేని తీయి = coverage.get_population ();
  ఒకవేళ (తీయి >= ముసుగు)
    తిరిగి;

  hb_సమితి_t intersection;
  coverage.intersect_సమితి (c->మాతృ_చురుకైన_glyphs (), intersection);

  /* In degenerate fuzzer-found fonts, but not real fonts,
   * this table can keep adding new glyphs in each round of closure.
   * Refuse to close-over, if it maps glyph range to overlapping range. */
  hb_codepoint_t min_before = intersection.get_min ();
  hb_codepoint_t max_before = intersection.get_max ();
  hb_codepoint_t min_after = (min_before + d) & ముసుగు;
  hb_codepoint_t max_after = (max_before + d) & ముసుగు;
  ఒకవేళ (intersection.get_population () == max_before - min_before + 1 &&
	((min_before <= min_after && min_after <= max_before) ||
	 (min_before <= max_after && max_after <= max_before)))
    తిరిగి;

  + hb_iter (intersection)
  | hb_map ([d, ముసుగు] (hb_codepoint_t g) { తిరిగి (g + d) & ముసుగు; })
  | hb_sink (c->output)
  ;
}

HB_అంతర్గత అంతఃవరుస
శూన్య single_subst_collect_glyphs (hb_collect_glyphs_context_t *c,
                                  స్థిరరాశి Coverage &coverage,
                                  hb_codepoint_t d, hb_codepoint_t ముసుగు)
{
  ఒకవేళ (unlikely (!coverage.collect_coverage (c->ఆదానం))) తిరిగి;

  + hb_iter (coverage)
  | hb_map ([d, ముసుగు] (hb_codepoint_t g) { తిరిగి (g + d) & ముసుగు; })
  | hb_sink (c->output)
  ;
}

మూస <రకం_పేరు Types>
నిర్మాణం SingleSubstFormat1_3
{
  రక్షిత:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  రకం_పేరు Types::మూస LOffset_గమ్యం<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of Substitution table */
  రకం_పేరు Types::HBUINT
                deltaGlyphID;           /* Add to original GlyphID to get
                                         * substitute GlyphID, modulo 0x10000 */

  బహిరంగ:
  DEFINE_కొలత_STATIC (2 + Types::LOffset::static_కొలత + Types::HBUINT::static_కొలత);

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది) &&
                  coverage.sanitize (c, ఇది) &&
                  /* The coverage  table may use a range to represent a set
                   * of glyphs, which means a small number of bytes can
                   * generate a large glyph set. Manually modify the
                   * sanitizer max ops to take this into account.
                   *
                   * Note: This check *must* be right after coverage sanitize. */
                  c->check_ops ((ఇది + coverage).get_population () >> 1));
  }

  hb_codepoint_t get_ముసుగు () స్థిరరాశి
  { తిరిగి (1 << (8 * Types::కొలత)) - 1; }

  తార్కిక intersects (స్థిరరాశి hb_సమితి_t *glyphs) స్థిరరాశి
  { తిరిగి (ఇది+coverage).intersects (glyphs); }

  తార్కిక may_have_non_1to1 () స్థిరరాశి
  { తిరిగి అబద్ధం; }

  శూన్య depend (hb_depend_context_t *c) స్థిరరాశి
  {
    single_subst_depend (c, ఇది+coverage, deltaGlyphID, get_ముసుగు ());
  }

  శూన్య సంవృతం (hb_సంవృతం_context_t *c) స్థిరరాశి
  {
    single_subst_సంవృతం (c, ఇది+coverage, deltaGlyphID, get_ముసుగు ());
  }

  శూన్య సంవృతం_lookups (hb_సంవృతం_lookups_context_t *c) స్థిరరాశి {}

  శూన్య collect_glyphs (hb_collect_glyphs_context_t *c) స్థిరరాశి
  {
    single_subst_collect_glyphs (c, ఇది+coverage, deltaGlyphID, get_ముసుగు ());
  }

  స్థిరరాశి Coverage &get_coverage () స్థిరరాశి { తిరిగి ఇది+coverage; }

  తార్కిక would_apply (hb_would_apply_context_t *c) స్థిరరాశి
  { తిరిగి c->len == 1 && (ఇది+coverage).get_coverage (c->glyphs[0]) != NOT_COVERED; }

  గుర్తులేని
  get_glyph_alternates (hb_codepoint_t  glyph_id,
                        గుర్తులేని        ప్రారంభం_స్థానభ్రంశం,
                        గుర్తులేని       *alternate_లెక్క  /* IN/OUT.  May be NULL. */,
                        hb_codepoint_t *alternate_glyphs /* OUT.     May be NULL. */) స్థిరరాశి
  {
    గుర్తులేని పూర్ణాంకం సూచీ_2 = (ఇది+coverage).get_coverage (glyph_id);
    ఒకవేళ (likely (సూచీ_2 == NOT_COVERED))
    {
      ఒకవేళ (alternate_లెక్క)
        *alternate_లెక్క = 0;
      తిరిగి 0;
    }

    ఒకవేళ (alternate_లెక్క && *alternate_లెక్క && alternate_glyphs)
    {
      hb_codepoint_t d = deltaGlyphID;
      hb_codepoint_t ముసుగు = get_ముసుగు ();

      glyph_id = (glyph_id + d) & ముసుగు;

      *alternate_glyphs = glyph_id;
      *alternate_లెక్క = 1;
    }

    తిరిగి 1;
  }

  శూన్య
  collect_glyph_alternates (hb_map_t  *alternate_లెక్క /* IN/OUT */,
			    hb_map_t  *alternate_glyphs /* IN/OUT */) స్థిరరాశి
  {
    hb_codepoint_t d = deltaGlyphID;
    hb_codepoint_t ముసుగు = get_ముసుగు ();

    + hb_iter (ఇది+coverage)
    | hb_map ([d, ముసుగు] (hb_codepoint_t g) { తిరిగి hb_pair (g, (g + d) & ముసుగు); })
    | hb_apply ([&] (స్థిరరాశి hb_pair_t<hb_codepoint_t, hb_codepoint_t> &p) -> శూన్య
		{ hb_collect_glyph_alternates_కూడు (p.మొదటి, p.second,
						    alternate_లెక్క, alternate_glyphs); })
    ;
  }

  తార్కిక apply (hb_ot_apply_context_t *c) స్థిరరాశి
  {
    TRACE_APPLY (ఇది);
    hb_codepoint_t glyph_id = c->బఫరు->cur().codepoint;
    గుర్తులేని పూర్ణాంకం సూచీ_2 = (ఇది+coverage).get_coverage (glyph_id);
    ఒకవేళ (సూచీ_2 == NOT_COVERED) return_trace (అబద్ధం);

    hb_codepoint_t d = deltaGlyphID;
    hb_codepoint_t ముసుగు = get_ముసుగు ();

    glyph_id = (glyph_id + d) & ముసుగు;

    ఒకవేళ (HB_బఫరు_సందేశం_మరిన్ని && c->బఫరు->messaging ())
    {
      c->బఫరు->sync_so_far ();
      c->బఫరు->సందేశం (c->font,
			  "replacing glyph at %u (single substitution)",
			  c->బఫరు->idx);
    }

    c->replace_glyph (glyph_id);

    ఒకవేళ (HB_బఫరు_సందేశం_మరిన్ని && c->బఫరు->messaging ())
    {
      c->బఫరు->సందేశం (c->font,
			  "replaced glyph at %u (single substitution)",
			  c->బఫరు->idx - 1u);
    }

    return_trace (నిజం);
  }

  మూస<రకం_పేరు Iterator,
           hb_requires (hb_is_sorted_ఆధారం_యొక్క (Iterator, hb_codepoint_t))>
  తార్కిక serialize (hb_serialize_context_t *c,
                  Iterator glyphs,
                  గుర్తులేని delta)
  {
    TRACE_SERIALIZE (ఇది);
    ఒకవేళ (unlikely (!c->extend_min (ఇది))) return_trace (అబద్ధం);
    ఒకవేళ (unlikely (!coverage.serialize_serialize (c, glyphs))) return_trace (అబద్ధం);
    c->check_కేటాయించు (deltaGlyphID, delta, HB_SERIALIZE_ERROR_INT_అధికప్రవాహం);
    return_trace (నిజం);
  }

  తార్కిక subset (hb_subset_context_t *c) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    return_trace (subset_single_subst (c, ఇది+coverage, deltaGlyphID, get_ముసుగు ()));
  }
};

}
}
}


#అయితే_ముగింపు /* OT_LAYOUT_GSUB_SINGLESUBSTFORMAT1_HH */
