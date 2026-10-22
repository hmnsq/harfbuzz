#నిర్వచితం_కాకపోతే OT_LAYOUT_GSUB_REVERSECHAINSINGLESUBSTFORMAT1_HH
#నిర్వచించు OT_LAYOUT_GSUB_REVERSECHAINSINGLESUBSTFORMAT1_HH

#అన్తర్భావ "Common.hh"

నామాకాశ OT {
నామాకాశ Layout {
నామాకాశ GSUB_impl {

మూస <రకం_పేరు Types>
నిర్మాణం తిరగేయి_Chain_Single_Subst_Format1_2
{
  రక్షిత:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  రకం_పేరు Types::మూస LOffset_గమ్యం<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of table */
  Array16_యొక్క<రకం_పేరు Types::మూస స్థానభ్రంశం_గమ్యం<Coverage>>
                backtrack;              /* Array of coverage tables
                                         * in backtracking sequence, in glyph
                                         * sequence order */
  /* Format 2 widens the backtrack and lookahead coverage offsets but
   * keeps their counts 16-bit. */
  Array16_యొక్క<రకం_పేరు Types::మూస స్థానభ్రంశం_గమ్యం<Coverage>>
                lookaheadX;             /* Array of coverage tables
                                         * in lookahead sequence, in glyph
                                         * sequence order */
  రకం_పేరు Types::మూస Array_యొక్క<రకం_పేరు Types::HBGlyphID>
                substituteX;            /* Array of substitute
                                         * GlyphIDs--ordered by Coverage Index */
  బహిరంగ:
  DEFINE_కొలత_MIN (6 + Types::LOffset::static_కొలత + Types::HBUINT::static_కొలత);

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    ఒకవేళ (!(coverage.sanitize (c, ఇది) && backtrack.sanitize (c, ఇది)))
      return_trace (అబద్ధం);
    hb_barrier ();
    స్థిరరాశి స్వయంచాలక &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);
    ఒకవేళ (!lookahead.sanitize (c, ఇది))
      return_trace (అబద్ధం);
    hb_barrier ();
    స్థిరరాశి స్వయంచాలక &substitute = StructAfter<decltype (substituteX)> (lookahead);
    return_trace (substitute.sanitize (c));
  }

  తార్కిక intersects (స్థిరరాశి hb_సమితి_t *glyphs) స్థిరరాశి
  {
    ఒకవేళ (!(ఇది+coverage).intersects (glyphs))
      తిరిగి అబద్ధం;

    స్థిరరాశి స్వయంచాలక &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);

    గుర్తులేని పూర్ణాంకం లెక్క;

    లెక్క = backtrack.len;
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++)
      ఒకవేళ (!(ఇది+backtrack[i]).intersects (glyphs))
        తిరిగి అబద్ధం;

    లెక్క = lookahead.len;
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++)
      ఒకవేళ (!(ఇది+lookahead[i]).intersects (glyphs))
        తిరిగి అబద్ధం;

    తిరిగి నిజం;
  }

  తార్కిక may_have_non_1to1 () స్థిరరాశి
  { తిరిగి అబద్ధం; }

  శూన్య depend (hb_depend_context_t *c) స్థిరరాశి {
    // Filter by intersects and parent_active_glyphs like closure does
    ఒకవేళ (!intersects (c->glyphs)) తిరిగి;

    స్థిరరాశి స్వయంచాలక &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);
    స్థిరరాశి స్వయంచాలక &substitute = StructAfter<decltype (substituteX)> (lookahead);

    /* Build context set from backtrack and lookahead coverages.
     * ReverseChainSingleSubst has contextual requirements - the substitution
     * only fires when backtrack and lookahead glyphs are present. */
    hb_సమితి_t context_glyphs;

    /* Add glyphs from backtrack coverages */
    గుర్తులేని పూర్ణాంకం లెక్క = backtrack.len;
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++)
    {
      hb_సమితి_t వెనుక_glyphs;
      (ఇది+backtrack[i]).intersect_సమితి (*c->glyphs, వెనుక_glyphs);
      ఒకవేళ (వెనుక_glyphs.get_population () == 1)
        context_glyphs.కూడు (వెనుక_glyphs.get_min ());
      లేకపోతే ఒకవేళ (వెనుక_glyphs.get_population () > 1)
      {
        hb_codepoint_t సమితి_idx = c->depend_దత్తాంశం->కనుగొను_or_create_context_సమితి (వెనుక_glyphs);
        ఒకవేళ (unlikely (సమితి_idx == HB_CODEPOINT_INVALID))
          తిరిగి;
        context_glyphs.కూడు (HB_DEPEND_CONTEXT_సమితి_FLAG | సమితి_idx);
      }
    }

    /* Add glyphs from lookahead coverages */
    లెక్క = lookahead.len;
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++)
    {
      hb_సమితి_t look_glyphs;
      (ఇది+lookahead[i]).intersect_సమితి (*c->glyphs, look_glyphs);
      ఒకవేళ (look_glyphs.get_population () == 1)
        context_glyphs.కూడు (look_glyphs.get_min ());
      లేకపోతే ఒకవేళ (look_glyphs.get_population () > 1)
      {
        hb_codepoint_t సమితి_idx = c->depend_దత్తాంశం->కనుగొను_or_create_context_సమితి (look_glyphs);
        ఒకవేళ (unlikely (సమితి_idx == HB_CODEPOINT_INVALID))
          తిరిగి;
        context_glyphs.కూడు (HB_DEPEND_CONTEXT_సమితి_FLAG | సమితి_idx);
      }
    }

    /* Allocate context set and save/restore around edge creation */
    ఒకవేళ (unlikely (context_glyphs.లోపల_error ()))
    {
      c->depend_దత్తాంశం->fail ();
      తిరిగి;
    }
    hb_codepoint_t context_సమితి_idx = context_glyphs.is_empty ()
      ? HB_CODEPOINT_INVALID
      : c->depend_దత్తాంశం->కనుగొను_or_create_context_సమితి (context_glyphs);
    ఒకవేళ (unlikely (!context_glyphs.is_empty () &&
                  context_సమితి_idx == HB_CODEPOINT_INVALID))
      తిరిగి;

    hb_codepoint_t saved_context = c->depend_దత్తాంశం->current_context_సమితి_సూచీ;
    c->depend_దత్తాంశం->current_context_సమితి_సూచీ = context_సమితి_idx;

    + hb_zip (ఇది+coverage, substitute)
    | hb_filter (c->మాతృ_చురుకైన_glyphs (), hb_మొదటి)
    | hb_apply ([&] (స్థిరరాశి hb_codepoint_pair_t &_) { c->depend_దత్తాంశం->కూడు_gsub_lookup (_.మొదటి, c->lookup_సూచీ, _.second); })
    ;

    c->depend_దత్తాంశం->current_context_సమితి_సూచీ = saved_context;
  }

  శూన్య సంవృతం (hb_సంవృతం_context_t *c) స్థిరరాశి
  {
    ఒకవేళ (!intersects (c->glyphs)) తిరిగి;

    స్థిరరాశి స్వయంచాలక &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);
    స్థిరరాశి స్వయంచాలక &substitute = StructAfter<decltype (substituteX)> (lookahead);

    + hb_zip (ఇది+coverage, substitute)
    | hb_filter (c->మాతృ_చురుకైన_glyphs (), hb_మొదటి)
    | hb_map (hb_రెండవ)
    | hb_sink (c->output)
    ;
  }

  శూన్య సంవృతం_lookups (hb_సంవృతం_lookups_context_t *c) స్థిరరాశి {}

  శూన్య collect_glyphs (hb_collect_glyphs_context_t *c) స్థిరరాశి
  {
    ఒకవేళ (unlikely (!(ఇది+coverage).collect_coverage (c->ఆదానం))) తిరిగి;

    గుర్తులేని పూర్ణాంకం లెక్క;

    లెక్క = backtrack.len;
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++)
      ఒకవేళ (unlikely (!(ఇది+backtrack[i]).collect_coverage (c->before))) తిరిగి;

    స్థిరరాశి స్వయంచాలక &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);
    లెక్క = lookahead.len;
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++)
      ఒకవేళ (unlikely (!(ఇది+lookahead[i]).collect_coverage (c->after))) తిరిగి;

    స్థిరరాశి స్వయంచాలక &substitute = StructAfter<decltype (substituteX)> (lookahead);
    లెక్క = substitute.len;
    c->output->కూడు_array (substitute.arrayZ, substitute.len);
  }

  స్థిరరాశి Coverage &get_coverage () స్థిరరాశి { తిరిగి ఇది+coverage; }

  తార్కిక would_apply (hb_would_apply_context_t *c) స్థిరరాశి
  { తిరిగి c->len == 1 && (ఇది+coverage).get_coverage (c->glyphs[0]) != NOT_COVERED; }

  తార్కిక apply (hb_ot_apply_context_t *c) స్థిరరాశి
  {
    TRACE_APPLY (ఇది);
    గుర్తులేని పూర్ణాంకం సూచీ_2 = (ఇది+coverage).get_coverage (c->బఫరు->cur ().codepoint);
    ఒకవేళ (సూచీ_2 == NOT_COVERED) return_trace (అబద్ధం);

    ఒకవేళ (unlikely (c->nesting_level_ఎడమ != HB_MAX_NESTING_LEVEL))
      return_trace (అబద్ధం); /* No chaining to this type */

    స్థిరరాశి స్వయంచాలక &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);
    స్థిరరాశి స్వయంచాలక &substitute = StructAfter<decltype (substituteX)> (lookahead);

    ఒకవేళ (unlikely (సూచీ_2 >= substitute.len)) return_trace (అబద్ధం);

    గుర్తులేని పూర్ణాంకం ప్రారంభం_సూచీ = 0, చివర_సూచీ = 0;
    ఒకవేళ (సరిపోలిక_backtrack (c,
                         backtrack.len, backtrack.arrayZ,
                         సరిపోలిక_coverage, ఇది,
                         &ప్రారంభం_సూచీ) &&
        సరిపోలిక_lookahead (c,
                         lookahead.len, lookahead.arrayZ,
                         సరిపోలిక_coverage, ఇది,
                         c->బఫరు->idx + 1, &చివర_సూచీ))
    {
      c->బఫరు->unsafe_గమ్యం_break_from_outbuffer (ప్రారంభం_సూచీ, చివర_సూచీ);

      ఒకవేళ (HB_బఫరు_సందేశం_మరిన్ని && c->బఫరు->messaging ())
      {
	c->బఫరు->సందేశం (c->font,
			    "replacing glyph at %u (reverse chaining substitution)",
			    c->బఫరు->idx);
      }

      c->replace_glyph_inplace (substitute[సూచీ_2]);

      ఒకవేళ (HB_బఫరు_సందేశం_మరిన్ని && c->బఫరు->messaging ())
      {
	c->బఫరు->సందేశం (c->font,
			    "replaced glyph at %u (reverse chaining substitution)",
			    c->బఫరు->idx);
      }

      /* Note: We DON'T decrease buffer->idx.  The main loop does it
       * for us.  This is useful for preventing surprises if someone
       * calls us through a Context lookup. */
      return_trace (నిజం);
    }
    లేకపోతే
    {
      c->బఫరు->unsafe_గమ్యం_concat_from_outbuffer (ప్రారంభం_సూచీ, చివర_సూచీ);
      return_trace (అబద్ధం);
    }
  }

  మూస<రకం_పేరు Iterator,
           hb_requires (hb_is_iterator (Iterator))>
  తార్కిక serialize_coverage_స్థానభ్రంశం_array (hb_subset_context_t *c, Iterator it) స్థిరరాశి
  {
    TRACE_SERIALIZE (ఇది);
    స్వయంచాలక *బహిర్గతం = c->serializer->ప్రారంభం_embed<Array16_యొక్క<రకం_పేరు Types::మూస స్థానభ్రంశం_గమ్యం<Coverage>>> ();
    ఒకవేళ (unlikely (!బహిర్గతం->serialize (c->serializer, 0u))) return_trace (అబద్ధం);

    కొరకు (స్వయంచాలక& స్థానభ్రంశం : it) {
      స్వయంచాలక *o = బహిర్గతం->serialize_append (c->serializer);
      ఒకవేళ (unlikely (!o) || !o->serialize_subset (c, స్థానభ్రంశం, ఇది))
        return_trace (అబద్ధం);
    }

    return_trace (నిజం);
  }

  మూస<రకం_పేరు Iterator, రకం_పేరు BacktrackIterator, రకం_పేరు LookaheadIterator,
           hb_requires (hb_is_sorted_ఆధారం_యొక్క (Iterator, hb_codepoint_pair_t)),
           hb_requires (hb_is_iterator (BacktrackIterator)),
           hb_requires (hb_is_iterator (LookaheadIterator))>
  తార్కిక serialize (hb_subset_context_t *c,
                  Iterator coverage_subst_iter,
                  BacktrackIterator backtrack_iter,
                  LookaheadIterator lookahead_iter) స్థిరరాశి
  {
    TRACE_SERIALIZE (ఇది);

    స్వయంచాలక *బహిర్గతం = c->serializer->ప్రారంభం_embed (ఇది);
    ఒకవేళ (unlikely (!c->serializer->embed (ఇది->format))) return_trace (అబద్ధం);
    ఒకవేళ (unlikely (!c->serializer->embed (ఇది->coverage))) return_trace (అబద్ధం);

    ఒకవేళ (!serialize_coverage_స్థానభ్రంశం_array (c, backtrack_iter)) return_trace (అబద్ధం);
    ఒకవేళ (!serialize_lookahead_coverage_స్థానభ్రంశం_array (c, lookahead_iter)) return_trace (అబద్ధం);

    స్వయంచాలక *substitute_బహిర్గతం = c->serializer->ప్రారంభం_embed<రకం_పేరు Types::మూస Array_యొక్క<రకం_పేరు Types::HBGlyphID>> ();
    స్వయంచాలక substitutes =
    + coverage_subst_iter
    | hb_map (hb_రెండవ)
    ;

    స్వయంచాలక glyphs =
    + coverage_subst_iter
    | hb_map_retains_క్రమీకరణ (hb_మొదటి)
    ;
    ఒకవేళ (unlikely (! c->serializer->check_success (substitute_బహిర్గతం->serialize (c->serializer, substitutes))))
        return_trace (అబద్ధం);

    ఒకవేళ (unlikely (!బహిర్గతం->coverage.serialize_serialize (c->serializer, glyphs)))
      return_trace (అబద్ధం);
    return_trace (నిజం);
  }

  తార్కిక subset (hb_subset_context_t *c) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    స్థిరరాశి hb_సమితి_t &glyphset = *c->plan->glyphset_gsub ();
    స్థిరరాశి hb_map_t &glyph_map = *c->plan->glyph_map;

    స్థిరరాశి స్వయంచాలక &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);
    స్థిరరాశి స్వయంచాలక &substitute = StructAfter<decltype (substituteX)> (lookahead);

    స్వయంచాలక it =
    + hb_zip (ఇది+coverage, substitute)
    | hb_filter (glyphset, hb_మొదటి)
    | hb_filter (glyphset, hb_రెండవ)
    | hb_map_retains_క్రమీకరణ ([&] (hb_pair_t<hb_codepoint_t, స్థిరరాశి రకం_పేరు Types::HBGlyphID &> p) -> hb_codepoint_pair_t
                              { తిరిగి hb_pair (glyph_map[p.మొదటి], glyph_map[p.second]); })
    ;

    return_trace (తార్కిక (it) && serialize (c, it, backtrack.iter (), lookahead.iter ()));
  }

  మూస<రకం_పేరు Iterator,
           hb_requires (hb_is_iterator (Iterator))>
  తార్కిక serialize_lookahead_coverage_స్థానభ్రంశం_array (hb_subset_context_t *c, Iterator it) స్థిరరాశి
  {
    TRACE_SERIALIZE (ఇది);
    స్వయంచాలక *బహిర్గతం = c->serializer->ప్రారంభం_embed<Array16_యొక్క<రకం_పేరు Types::మూస స్థానభ్రంశం_గమ్యం<Coverage>>> ();
    ఒకవేళ (unlikely (!బహిర్గతం->serialize (c->serializer, 0u))) return_trace (అబద్ధం);

    కొరకు (స్వయంచాలక& స్థానభ్రంశం : it) {
      స్వయంచాలక *o = బహిర్గతం->serialize_append (c->serializer);
      ఒకవేళ (unlikely (!o) || !o->serialize_subset (c, స్థానభ్రంశం, ఇది))
        return_trace (అబద్ధం);
    }

    return_trace (నిజం);
  }
};

}
}
}

#అయితే_ముగింపు  /* HB_OT_LAYOUT_GSUB_REVERSECHAINSINGLESUBSTFORMAT1_HH */
