#నిర్వచితం_కాకపోతే OT_LAYOUT_GSUB_MULTIPLESUBSTFORMAT1_HH
#నిర్వచించు OT_LAYOUT_GSUB_MULTIPLESUBSTFORMAT1_HH

#అన్తర్భావ "Common.hh"
#అన్తర్భావ "Sequence.hh"

నామాకాశ OT {
నామాకాశ Layout {
నామాకాశ GSUB_impl {

మూస <రకం_పేరు Types>
నిర్మాణం గుణిజం_Subst_Format1_2
{
  రక్షిత:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  రకం_పేరు Types::మూస LOffset_గమ్యం<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of Substitution table */
  రకం_పేరు Types::మూస Array_యొక్క<రకం_పేరు Types::మూస స్థానభ్రంశం_గమ్యం<Sequence<Types>>>
                sequence;               /* Array of Sequence tables
                                         * ordered by Coverage Index */
  బహిరంగ:
  DEFINE_కొలత_ARRAY (2 + Types::LOffset::static_కొలత + Types::HBUINT::static_కొలత, sequence);

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (coverage.sanitize (c, ఇది) && sequence.sanitize (c, ఇది));
  }

  తార్కిక intersects (స్థిరరాశి hb_సమితి_t *glyphs) స్థిరరాశి
  { తిరిగి (ఇది+coverage).intersects (glyphs); }

  తార్కిక may_have_non_1to1 () స్థిరరాశి
  { తిరిగి నిజం; }

  శూన్య సంవృతం (hb_సంవృతం_context_t *c) స్థిరరాశి
  {
    + hb_zip (ఇది+coverage, sequence)
    | hb_filter (c->మాతృ_చురుకైన_glyphs (), hb_మొదటి)
    | hb_map (hb_రెండవ)
    | hb_map (hb_కూడు (ఇది))
    | hb_apply ([c] (స్థిరరాశి Sequence<Types> &_) { _.సంవృతం (c); })
    ;
  }

  శూన్య depend (hb_depend_context_t *c) స్థిరరాశి
  {
    // Filter by parent_active_glyphs like closure does
    + hb_zip (ఇది+coverage, sequence)
    | hb_filter (c->మాతృ_చురుకైన_glyphs (), hb_మొదటి)
    | hb_apply ([&] (స్థిరరాశి hb_pair_t<hb_codepoint_t, స్థిరరాశి రకం_పేరు Types::మూస స్థానభ్రంశం_గమ్యం<Sequence<Types>>&> &_)
                {
                  స్థిరరాశి Sequence<Types>& s = ఇది+_.second;
                  s.depend (c, _.మొదటి);
                })
    ;
  }

  శూన్య సంవృతం_lookups (hb_సంవృతం_lookups_context_t *c) స్థిరరాశి {}

  శూన్య collect_glyphs (hb_collect_glyphs_context_t *c) స్థిరరాశి
  {
    ఒకవేళ (unlikely (!(ఇది+coverage).collect_coverage (c->ఆదానం))) తిరిగి;
    + hb_zip (ఇది+coverage, sequence)
    | hb_map (hb_రెండవ)
    | hb_map (hb_కూడు (ఇది))
    | hb_apply ([c] (స్థిరరాశి Sequence<Types> &_) { _.collect_glyphs (c); })
    ;
  }

  స్థిరరాశి Coverage &get_coverage () స్థిరరాశి { తిరిగి ఇది+coverage; }

  తార్కిక would_apply (hb_would_apply_context_t *c) స్థిరరాశి
  { తిరిగి c->len == 1 && (ఇది+coverage).get_coverage (c->glyphs[0]) != NOT_COVERED; }

  తార్కిక apply (hb_ot_apply_context_t *c) స్థిరరాశి
  {
    TRACE_APPLY (ఇది);

    గుర్తులేని పూర్ణాంకం సూచీ_2 = (ఇది+coverage).get_coverage (c->బఫరు->cur().codepoint);
    ఒకవేళ (సూచీ_2 == NOT_COVERED) return_trace (అబద్ధం);

    return_trace ((ఇది+sequence[సూచీ_2]).apply (c));
  }

  మూస<రకం_పేరు Iterator,
           hb_requires (hb_is_sorted_iterator (Iterator))>
  తార్కిక serialize (hb_serialize_context_t *c,
		  Iterator it)
  {
    TRACE_SERIALIZE (ఇది);
    స్వయంచాలక sequences =
      + it
      | hb_map (hb_రెండవ)
      ;
    స్వయంచాలక glyphs =
      + it
      | hb_map_retains_క్రమీకరణ (hb_మొదటి)
      ;
    ఒకవేళ (unlikely (!c->extend_min (ఇది))) return_trace (అబద్ధం);

    ఒకవేళ (unlikely (!sequence.serialize (c, sequences.నిడివి))) return_trace (అబద్ధం);

    కొరకు (స్వయంచాలక& pair : hb_zip (sequences, sequence))
    {
      ఒకవేళ (unlikely (!pair.second
		    .serialize_serialize (c, pair.మొదటి)))
        return_trace (అబద్ధం);
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
    + hb_zip (ఇది+coverage, sequence)
    | hb_filter (glyphset, hb_మొదటి)
    | hb_filter (subset_స్థానభ్రంశం_array (c, బహిర్గతం->sequence, ఇది), hb_రెండవ)
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


#అయితే_ముగింపు /* OT_LAYOUT_GSUB_MULTIPLESUBSTFORMAT1_HH */
