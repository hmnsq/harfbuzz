#నిర్వచితం_కాకపోతే OT_LAYOUT_GSUB_SINGLESUBSTFORMAT2_HH
#నిర్వచించు OT_LAYOUT_GSUB_SINGLESUBSTFORMAT2_HH

#అన్తర్భావ "Common.hh"

నామాకాశ OT {
నామాకాశ Layout {
నామాకాశ GSUB_impl {

మూస <రకం_పేరు Types>
నిర్మాణం SingleSubstFormat2_4
{
  రక్షిత:
  HBUINT16      format;                 /* Format identifier--format = 2 */
  రకం_పేరు Types::మూస LOffset_గమ్యం<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of Substitution table */
  రకం_పేరు Types::మూస Array_యొక్క<రకం_పేరు Types::HBGlyphID>
                substitute;             /* Array of substitute
                                         * GlyphIDs--ordered by Coverage Index */

  బహిరంగ:
  DEFINE_కొలత_ARRAY (2 + Types::LOffset::static_కొలత + Types::HBUINT::static_కొలత, substitute);

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (coverage.sanitize (c, ఇది) && substitute.sanitize (c));
  }

  తార్కిక intersects (స్థిరరాశి hb_సమితి_t *glyphs) స్థిరరాశి
  { తిరిగి (ఇది+coverage).intersects (glyphs); }

  తార్కిక may_have_non_1to1 () స్థిరరాశి
  { తిరిగి అబద్ధం; }

  శూన్య depend (hb_depend_context_t *c) స్థిరరాశి
  {
    స్వయంచాలక &cov = ఇది+coverage;
    స్వయంచాలక &glyph_సమితి = c->మాతృ_చురుకైన_glyphs ();

    // Filter by active glyphs like closure does
    ఒకవేళ (substitute.len > glyph_సమితి.get_population () * 4)
    {
      కొరకు (స్వయంచాలక g : glyph_సమితి)
      {
	గుర్తులేని i = cov.get_coverage (g);
	ఒకవేళ (i == NOT_COVERED || i >= substitute.len)
	  కొనసాగు;
	c->depend_దత్తాంశం->కూడు_gsub_lookup (g, c->lookup_సూచీ, substitute.arrayZ[i]);
      }

      తిరిగి;
    }

    + hb_zip (cov, substitute)
    | hb_filter (glyph_సమితి, hb_మొదటి)
    | hb_apply ([&] (స్థిరరాశి hb_codepoint_pair_t &_) { c->depend_దత్తాంశం->కూడు_gsub_lookup (_.మొదటి, c->lookup_సూచీ, _.second); })
    ;
  }

  శూన్య సంవృతం (hb_సంవృతం_context_t *c) స్థిరరాశి
  {
    స్వయంచాలక &cov = ఇది+coverage;
    స్వయంచాలక &glyph_సమితి = c->మాతృ_చురుకైన_glyphs ();

    ఒకవేళ (substitute.len > glyph_సమితి.get_population () * 4)
    {
      కొరకు (స్వయంచాలక g : glyph_సమితి)
      {
	గుర్తులేని i = cov.get_coverage (g);
	ఒకవేళ (i == NOT_COVERED || i >= substitute.len)
	  కొనసాగు;
	c->output->కూడు (substitute.arrayZ[i]);
      }

      తిరిగి;
    }

    + hb_zip (cov, substitute)
    | hb_filter (glyph_సమితి, hb_మొదటి)
    | hb_map (hb_రెండవ)
    | hb_sink (c->output)
    ;
  }

  శూన్య సంవృతం_lookups (hb_సంవృతం_lookups_context_t *c) స్థిరరాశి {}

  శూన్య collect_glyphs (hb_collect_glyphs_context_t *c) స్థిరరాశి
  {
    ఒకవేళ (unlikely (!(ఇది+coverage).collect_coverage (c->ఆదానం))) తిరిగి;
    + hb_zip (ఇది+coverage, substitute)
    | hb_map (hb_రెండవ)
    | hb_sink (c->output)
    ;
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
      glyph_id = substitute[సూచీ_2];

      *alternate_glyphs = glyph_id;
      *alternate_లెక్క = 1;
    }

    తిరిగి 1;
  }

  శూన్య
  collect_glyph_alternates (hb_map_t  *alternate_లెక్క /* IN/OUT */,
			    hb_map_t  *alternate_glyphs /* IN/OUT */) స్థిరరాశి
  {
    + hb_zip (ఇది+coverage, substitute)
    | hb_apply ([&] (స్థిరరాశి hb_pair_t<hb_codepoint_t, hb_codepoint_t> &p) -> శూన్య
		{ hb_collect_glyph_alternates_కూడు (p.మొదటి, p.second,
						    alternate_లెక్క, alternate_glyphs); })
    ;
  }

  తార్కిక apply (hb_ot_apply_context_t *c) స్థిరరాశి
  {
    TRACE_APPLY (ఇది);
    గుర్తులేని పూర్ణాంకం సూచీ_2 = (ఇది+coverage).get_coverage (c->బఫరు->cur().codepoint);
    ఒకవేళ (సూచీ_2 == NOT_COVERED) return_trace (అబద్ధం);

    ఒకవేళ (unlikely (సూచీ_2 >= substitute.len)) return_trace (అబద్ధం);

    ఒకవేళ (HB_బఫరు_సందేశం_మరిన్ని && c->బఫరు->messaging ())
    {
      c->బఫరు->sync_so_far ();
      c->బఫరు->సందేశం (c->font,
			  "replacing glyph at %u (single substitution)",
			  c->బఫరు->idx);
    }

    c->replace_glyph (substitute[సూచీ_2]);

    ఒకవేళ (HB_బఫరు_సందేశం_మరిన్ని && c->బఫరు->messaging ())
    {
      c->బఫరు->సందేశం (c->font,
			  "replaced glyph at %u (single substitution)",
			  c->బఫరు->idx - 1u);
    }

    return_trace (నిజం);
  }

  మూస<రకం_పేరు Iterator,
           hb_requires (hb_is_sorted_ఆధారం_యొక్క (Iterator,
                                                hb_codepoint_pair_t))>
  తార్కిక serialize (hb_serialize_context_t *c,
                  Iterator it)
  {
    TRACE_SERIALIZE (ఇది);
    స్వయంచాలక substitutes =
      + it
      | hb_map (hb_రెండవ)
      ;
    స్వయంచాలక glyphs =
      + it
      | hb_map_retains_క్రమీకరణ (hb_మొదటి)
      ;
    ఒకవేళ (unlikely (!c->extend_min (ఇది))) return_trace (అబద్ధం);
    ఒకవేళ (unlikely (!substitute.serialize (c, substitutes))) return_trace (అబద్ధం);
    ఒకవేళ (unlikely (!coverage.serialize_serialize (c, glyphs))) return_trace (అబద్ధం);
    return_trace (నిజం);
  }

  తార్కిక subset (hb_subset_context_t *c) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    స్థిరరాశి hb_సమితి_t &glyphset = *c->plan->glyphset_gsub ();
    స్థిరరాశి hb_map_t &glyph_map = *c->plan->glyph_map;

    స్వయంచాలక it =
    + hb_zip (ఇది+coverage, substitute)
    | hb_filter (glyphset, hb_మొదటి)
    | hb_filter (glyphset, hb_రెండవ)
    | hb_map_retains_క్రమీకరణ ([&] (hb_pair_t<hb_codepoint_t, స్థిరరాశి రకం_పేరు Types::HBGlyphID &> p) -> hb_codepoint_pair_t
                              { తిరిగి hb_pair (glyph_map[p.మొదటి], glyph_map[p.second]); })
    ;

    తార్కిక ret = తార్కిక (it);
    SingleSubst_serialize (c->serializer, it);
    return_trace (ret);
  }
};

}
}
}

#అయితే_ముగింపు /* OT_LAYOUT_GSUB_SINGLESUBSTFORMAT2_HH */
