#నిర్వచితం_కాకపోతే OT_LAYOUT_GSUB_ALTERNATESUBSTFORMAT1_HH
#నిర్వచించు OT_LAYOUT_GSUB_ALTERNATESUBSTFORMAT1_HH

#అన్తర్భావ "AlternateSet.hh"
#అన్తర్భావ "Common.hh"

నామాకాశ OT {
నామాకాశ Layout {
నామాకాశ GSUB_impl {

మూస <రకం_పేరు Types>
నిర్మాణం AlternateSubstFormat1_2
{
  రక్షిత:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  రకం_పేరు Types::మూస LOffset_గమ్యం<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of Substitution table */
  రకం_పేరు Types::మూస Array_యొక్క<రకం_పేరు Types::మూస స్థానభ్రంశం_గమ్యం<Alternate_సమితి<Types>>>
                alternate_సమితి;           /* Array of AlternateSet tables
                                         * ordered by Coverage Index */
  బహిరంగ:
  DEFINE_కొలత_ARRAY (2 + Types::LOffset::static_కొలత + Types::HBUINT::static_కొలత, alternate_సమితి);

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (coverage.sanitize (c, ఇది) && alternate_సమితి.sanitize (c, ఇది));
  }

  తార్కిక intersects (స్థిరరాశి hb_సమితి_t *glyphs) స్థిరరాశి
  { తిరిగి (ఇది+coverage).intersects (glyphs); }

  తార్కిక may_have_non_1to1 () స్థిరరాశి
  { తిరిగి అబద్ధం; }

  శూన్య depend (hb_depend_context_t *c) స్థిరరాశి
  {
    // Filter by parent_active_glyphs like closure does
    + hb_zip (ఇది+coverage, alternate_సమితి)
    | hb_filter (c->మాతృ_చురుకైన_glyphs (), hb_మొదటి)
    | hb_apply ([&] (స్థిరరాశి hb_pair_t<hb_codepoint_t, స్థిరరాశి రకం_పేరు Types::మూస స్థానభ్రంశం_గమ్యం<Alternate_సమితి<Types>>&> &_)
                {
                  స్థిరరాశి Alternate_సమితి<Types>& as = ఇది+_.second;
                  as.depend (c, _.మొదటి);
                })
    ;
  }

  శూన్య సంవృతం (hb_సంవృతం_context_t *c) స్థిరరాశి
  {
    + hb_zip (ఇది+coverage, alternate_సమితి)
    | hb_filter (c->మాతృ_చురుకైన_glyphs (), hb_మొదటి)
    | hb_map (hb_రెండవ)
    | hb_map (hb_కూడు (ఇది))
    | hb_apply ([c] (స్థిరరాశి Alternate_సమితి<Types> &_) { _.సంవృతం (c); })
    ;
  }

  శూన్య సంవృతం_lookups (hb_సంవృతం_lookups_context_t *c) స్థిరరాశి {}

  శూన్య collect_glyphs (hb_collect_glyphs_context_t *c) స్థిరరాశి
  {
    ఒకవేళ (unlikely (!(ఇది+coverage).collect_coverage (c->ఆదానం))) తిరిగి;
    + hb_zip (ఇది+coverage, alternate_సమితి)
    | hb_map (hb_రెండవ)
    | hb_map (hb_కూడు (ఇది))
    | hb_apply ([c] (స్థిరరాశి Alternate_సమితి<Types> &_) { _.collect_glyphs (c); })
    ;
  }

  స్థిరరాశి Coverage &get_coverage () స్థిరరాశి { తిరిగి ఇది+coverage; }

  తార్కిక would_apply (hb_would_apply_context_t *c) స్థిరరాశి
  { తిరిగి c->len == 1 && (ఇది+coverage).get_coverage (c->glyphs[0]) != NOT_COVERED; }

  గుర్తులేని
  get_glyph_alternates (hb_codepoint_t  gid,
                        గుర్తులేని        ప్రారంభం_స్థానభ్రంశం,
                        గుర్తులేని       *alternate_లెక్క  /* IN/OUT.  May be NULL. */,
                        hb_codepoint_t *alternate_glyphs /* OUT.     May be NULL. */) స్థిరరాశి
  { తిరిగి (ఇది+alternate_సమితి[(ఇది+coverage).get_coverage (gid)])
           .get_alternates (ప్రారంభం_స్థానభ్రంశం, alternate_లెక్క, alternate_glyphs); }

  శూన్య
  collect_glyph_alternates (hb_map_t  *alternate_లెక్క /* IN/OUT */,
			    hb_map_t  *alternate_glyphs /* IN/OUT */) స్థిరరాశి
  {
    + hb_iter (alternate_సమితి)
    | hb_map (hb_కూడు (ఇది))
    | hb_zip (ఇది+coverage)
    | hb_apply ([&] (స్థిరరాశి hb_pair_t<స్థిరరాశి Alternate_సమితి<Types> &, hb_codepoint_t> _) {
		  _.మొదటి.collect_alternates (_.second, alternate_లెక్క, alternate_glyphs);
		})
    ;
  }

  తార్కిక apply (hb_ot_apply_context_t *c) స్థిరరాశి
  {
    TRACE_APPLY (ఇది);

    గుర్తులేని పూర్ణాంకం సూచీ_2 = (ఇది+coverage).get_coverage (c->బఫరు->cur().codepoint);
    ఒకవేళ (సూచీ_2 == NOT_COVERED) return_trace (అబద్ధం);

    return_trace ((ఇది+alternate_సమితి[సూచీ_2]).apply (c));
  }

  మూస <రకం_పేరు GlyphID>
  తార్కిక serialize (hb_serialize_context_t *c,
                  hb_sorted_array_t<GlyphID> glyphs,
                  hb_array_t<స్థిరరాశి గుర్తులేని పూర్ణాంకం> alternate_len_list,
                  hb_array_t<GlyphID> alternate_glyphs_list)
  {
    TRACE_SERIALIZE (ఇది);
    ఒకవేళ (unlikely (!c->extend_min (ఇది))) return_trace (అబద్ధం);
    ఒకవేళ (unlikely (!alternate_సమితి.serialize (c, glyphs.నిడివి))) return_trace (అబద్ధం);
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < glyphs.నిడివి; i++)
    {
      గుర్తులేని పూర్ణాంకం alternate_len = alternate_len_list[i];
      ఒకవేళ (unlikely (!alternate_సమితి[i]
                        .serialize_serialize (c, alternate_glyphs_list.sub_array (0, alternate_len))))
        return_trace (అబద్ధం);
      alternate_glyphs_list += alternate_len;
    }
    return_trace (coverage.serialize_serialize (c, glyphs));
  }

  తార్కిక subset (hb_subset_context_t *c) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    స్థిరరాశి hb_సమితి_t &glyphset = *c->plan->glyphset_gsub ();
    స్థిరరాశి hb_map_t &glyph_map = *c->plan->glyph_map;

    స్వయంచాలక *బహిర్గతం = c->serializer->ప్రారంభం_embed (*ఇది);
    ఒకవేళ (unlikely (!c->serializer->extend_min (బహిర్గతం))) return_trace (అబద్ధం);
    బహిర్గతం->format = format;

    hb_sorted_సదిశ_t<hb_codepoint_t> new_coverage;
    + hb_zip (ఇది+coverage, alternate_సమితి)
    | hb_filter (glyphset, hb_మొదటి)
    | hb_filter (subset_స్థానభ్రంశం_array (c, బహిర్గతం->alternate_సమితి, ఇది), hb_రెండవ)
    | hb_map (hb_మొదటి)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;
    బహిర్గతం->coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (తార్కిక (new_coverage));
  }
};

}
}
}

#అయితే_ముగింపు  /* OT_LAYOUT_GSUB_ALTERNATESUBSTFORMAT1_HH */
