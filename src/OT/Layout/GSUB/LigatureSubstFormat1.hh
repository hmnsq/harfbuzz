#నిర్వచితం_కాకపోతే OT_LAYOUT_GSUB_LIGATURESUBSTFORMAT1_HH
#నిర్వచించు OT_LAYOUT_GSUB_LIGATURESUBSTFORMAT1_HH

#అన్తర్భావ "Common.hh"
#అన్తర్భావ "LigatureSet.hh"

నామాకాశ OT {
నామాకాశ Layout {
నామాకాశ GSUB_impl {

మూస <రకం_పేరు Types>
నిర్మాణం LigatureSubstFormat1_2
{
  రక్షిత:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  రకం_పేరు Types::మూస LOffset_గమ్యం<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of Substitution table */
  రకం_పేరు Types::మూస Array_యొక్క<రకం_పేరు Types::మూస స్థానభ్రంశం_గమ్యం<Ligature_సమితి<Types>>>
                ligature_సమితి;            /* Array LigatureSet tables
                                         * ordered by Coverage Index */
  బహిరంగ:
  DEFINE_కొలత_ARRAY (2 + Types::LOffset::static_కొలత + Types::HBUINT::static_కొలత, ligature_సమితి);

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (coverage.sanitize (c, ఇది) && ligature_సమితి.sanitize (c, ఇది));
  }

  తార్కిక intersects (స్థిరరాశి hb_సమితి_t *glyphs) స్థిరరాశి
  {
    తిరిగి
    + hb_zip (ఇది+coverage, ligature_సమితి)
    | hb_filter (*glyphs, hb_మొదటి)
    | hb_map (hb_రెండవ)
    | hb_map ([ఇది, glyphs] (స్థిరరాశి రకం_పేరు Types::మూస స్థానభ్రంశం_గమ్యం<Ligature_సమితి<Types>> &_)
              { తిరిగి (ఇది+_).intersects (glyphs); })
    | hb_any
    ;
  }

  తార్కిక may_have_non_1to1 () స్థిరరాశి
  { తిరిగి నిజం; }

  శూన్య depend (hb_depend_context_t *c) స్థిరరాశి
  {
    + hb_zip (ఇది+coverage, ligature_సమితి)
    | hb_filter (c->మాతృ_చురుకైన_glyphs (), hb_మొదటి)
    | hb_apply ([&] (స్థిరరాశి hb_pair_t<hb_codepoint_t, స్థిరరాశి రకం_పేరు Types::మూస స్థానభ్రంశం_గమ్యం<Ligature_సమితి<Types>>&> &_)
                {
                  స్థిరరాశి Ligature_సమితి<Types>& ls = ఇది+_.second;
                  ls.depend (c, _.మొదటి);
                })
    ;
  }

  శూన్య సంవృతం (hb_సంవృతం_context_t *c) స్థిరరాశి
  {
    + hb_zip (ఇది+coverage, ligature_సమితి)
    | hb_filter (c->మాతృ_చురుకైన_glyphs (), hb_మొదటి)
    | hb_map (hb_రెండవ)
    | hb_map (hb_కూడు (ఇది))
    | hb_apply ([c] (స్థిరరాశి Ligature_సమితి<Types> &_) { _.సంవృతం (c); })
    ;

  }

  శూన్య సంవృతం_lookups (hb_సంవృతం_lookups_context_t *c) స్థిరరాశి {}

  శూన్య collect_glyphs (hb_collect_glyphs_context_t *c) స్థిరరాశి
  {
    ఒకవేళ (unlikely (!(ఇది+coverage).collect_coverage (c->ఆదానం))) తిరిగి;

    + hb_zip (ఇది+coverage, ligature_సమితి)
    | hb_map (hb_రెండవ)
    | hb_map (hb_కూడు (ఇది))
    | hb_apply ([c] (స్థిరరాశి Ligature_సమితి<Types> &_) { _.collect_glyphs (c); })
    ;
  }

  శూన్య collect_రెండవ_glyphs (hb_సమితి_digest_t *digest) స్థిరరాశి
  {
    + hb_iter (ligature_సమితి)
    | hb_map (hb_కూడు (ఇది))
    | hb_apply ([digest] (స్థిరరాశి Ligature_సమితి<Types> &_) { _.collect_seconds (*digest); })
    ;
  }

  స్థిరరాశి Coverage &get_coverage () స్థిరరాశి { తిరిగి ఇది+coverage; }

  తార్కిక would_apply (hb_would_apply_context_t *c) స్థిరరాశి
  {
    గుర్తులేని పూర్ణాంకం సూచీ_2 = (ఇది+coverage).get_coverage (c->glyphs[0]);
    ఒకవేళ (likely (సూచీ_2 == NOT_COVERED)) తిరిగి అబద్ధం;

    స్థిరరాశి స్వయంచాలక &lig_సమితి = ఇది+ligature_సమితి[సూచీ_2];
    తిరిగి lig_సమితి.would_apply (c);
  }

  నిర్మాణం external_cache_t
  {
    hb_ot_layout_mapping_cache_t coverage;
    hb_సమితి_digest_t seconds;
  };
  శూన్య *external_cache_create () స్థిరరాశి
  {
    external_cache_t *cache = (external_cache_t *) hb_malloc (పరిమాణం (external_cache_t));
    ఒకవేళ (likely (cache))
    {
      cache->coverage.clear ();

      cache->seconds.init ();
      + hb_iter (ligature_సమితి)
      | hb_map (hb_కూడు (ఇది))
      | hb_apply ([cache] (స్థిరరాశి Ligature_సమితి<Types> &_) { _.collect_seconds (cache->seconds); })
      ;
    }
    తిరిగి cache;
  }

  తార్కిక apply (hb_ot_apply_context_t *c, శూన్య *external_cache) స్థిరరాశి
  {
    TRACE_APPLY (ఇది);
    hb_బఫరు_t *బఫరు = c->బఫరు;

#నిర్వచితం_కాకపోతే HB_NO_OT_LAYOUT_LOOKUP_CACHE
    external_cache_t *cache = (external_cache_t *) external_cache;
    స్థిరరాశి hb_సమితి_digest_t *seconds = cache ? &cache->seconds : శూన్యసూచిక;
    గుర్తులేని పూర్ణాంకం సూచీ_2 = (ఇది+coverage).get_coverage (బఫరు->cur().codepoint, cache ? &cache->coverage : శూన్యసూచిక);
#లేకపోతే
    స్థిరరాశి hb_సమితి_digest_t *seconds = శూన్యసూచిక;
    గుర్తులేని పూర్ణాంకం సూచీ_2 = (ఇది+coverage).get_coverage (బఫరు->cur().codepoint);
#అయితే_ముగింపు
    ఒకవేళ (సూచీ_2 == NOT_COVERED) return_trace (అబద్ధం);

    స్థిరరాశి స్వయంచాలక &lig_సమితి = ఇది+ligature_సమితి[సూచీ_2];
    return_trace (lig_సమితి.apply (c, seconds));
  }

  మూస <రకం_పేరు GlyphID>
  తార్కిక serialize (hb_serialize_context_t *c,
                  hb_sorted_array_t<GlyphID> మొదటి_glyphs,
                  hb_array_t<స్థిరరాశి గుర్తులేని పూర్ణాంకం> ligature_చొప్పున_మొదటి_glyph_లెక్క_list,
                  hb_array_t<GlyphID> ligatures_list,
                  hb_array_t<స్థిరరాశి గుర్తులేని పూర్ణాంకం> component_లెక్క_list,
                  hb_array_t<GlyphID> component_list /* Starting from second for each ligature */)
  {
    TRACE_SERIALIZE (ఇది);
    ఒకవేళ (unlikely (!c->extend_min (ఇది))) return_trace (అబద్ధం);
    ఒకవేళ (unlikely (!ligature_సమితి.serialize (c, మొదటి_glyphs.నిడివి))) return_trace (అబద్ధం);
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < మొదటి_glyphs.నిడివి; i++)
    {
      గుర్తులేని పూర్ణాంకం ligature_లెక్క = ligature_చొప్పున_మొదటి_glyph_లెక్క_list[i];
      ఒకవేళ (unlikely (!ligature_సమితి[i]
                        .serialize_serialize (c,
                                              ligatures_list.sub_array (0, ligature_లెక్క),
                                              component_లెక్క_list.sub_array (0, ligature_లెక్క),
                                              component_list))) return_trace (అబద్ధం);
      ligatures_list += ligature_లెక్క;
      component_లెక్క_list += ligature_లెక్క;
    }
    return_trace (coverage.serialize_serialize (c, మొదటి_glyphs));
  }

  తార్కిక subset (hb_subset_context_t *c) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    స్థిరరాశి hb_సమితి_t &glyphset = *c->plan->glyphset_gsub ();
    స్థిరరాశి hb_map_t &glyph_map = *c->plan->glyph_map;

    స్వయంచాలక *బహిర్గతం = c->serializer->ప్రారంభం_embed (*ఇది);
    ఒకవేళ (unlikely (!c->serializer->extend_min (బహిర్గతం))) return_trace (అబద్ధం);
    బహిర్గతం->format = format;

    // Due to a bug in some older versions of windows 7 the Coverage table must be
    // packed after the LigatureSet and Ligature tables, so serialize Coverage first
    // which places it last in the packed order.
    hb_సమితి_t new_coverage;
    + hb_zip (ఇది+coverage, hb_iter (ligature_సమితి) | hb_map (hb_కూడు (ఇది)))
    | hb_filter (glyphset, hb_మొదటి)
    | hb_filter ([&] (స్థిరరాశి Ligature_సమితి<Types>& _) {
      తిరిగి _.intersects_lig_glyph (&glyphset);
    }, hb_రెండవ)
    | hb_map (hb_మొదటి)
    | hb_sink (new_coverage);

    ఒకవేళ (!c->serializer->push<Coverage> ()
        ->serialize (c->serializer,
                     + new_coverage.iter () | hb_map_retains_క్రమీకరణ (glyph_map)))
    {
      c->serializer->తీయి_discard ();
      return_trace (అబద్ధం);
    }

    గుర్తులేని coverage_idx = c->serializer->తీయి_pack ();
     c->serializer->కూడు_link (బహిర్గతం->coverage, coverage_idx);

    + hb_zip (ఇది+coverage, ligature_సమితి)
    | hb_filter (new_coverage, hb_మొదటి)
    | hb_map (hb_రెండవ)
    // to ensure that the repacker always orders the coverage table after the LigatureSet
    // and LigatureSubtable's they will be linked to the Coverage table via a virtual link
    // the coverage table object idx is passed down to facilitate this.
    | hb_apply (subset_స్థానభ్రంశం_array (c, బహిర్గతం->ligature_సమితి, ఇది, coverage_idx))
    ;

    return_trace (తార్కిక (new_coverage));
  }
};

}
}
}

#అయితే_ముగింపు  /* OT_LAYOUT_GSUB_LIGATURESUBSTFORMAT1_HH */
