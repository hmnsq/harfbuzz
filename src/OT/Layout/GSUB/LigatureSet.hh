#నిర్వచితం_కాకపోతే OT_LAYOUT_GSUB_LIGATURESET_HH
#నిర్వచించు OT_LAYOUT_GSUB_LIGATURESET_HH

#అన్తర్భావ "Common.hh"
#అన్తర్భావ "Ligature.hh"

నామాకాశ OT {
నామాకాశ Layout {
నామాకాశ GSUB_impl {

మూస <రకం_పేరు Types>
నిర్మాణం Ligature_సమితి
{
  బహిరంగ:
  Array16_యొక్క<రకం_పేరు Types::మూస స్థానభ్రంశం_గమ్యం<Ligature<Types>>>
                ligature;               /* Array LigatureSet tables
                                         * ordered by preference */
  
  DEFINE_కొలత_ARRAY (2, ligature);

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (ligature.sanitize (c, ఇది));
  }

  తార్కిక intersects (స్థిరరాశి hb_సమితి_t *glyphs) స్థిరరాశి
  {
    తిరిగి
    + hb_iter (ligature)
    | hb_map (hb_కూడు (ఇది))
    | hb_map ([glyphs] (స్థిరరాశి Ligature<Types> &_) { తిరిగి _.intersects (glyphs); })
    | hb_any
    ;
  }

  తార్కిక intersects_lig_glyph (స్థిరరాశి hb_సమితి_t *glyphs) స్థిరరాశి
  {
    తిరిగి
    + hb_iter (ligature)
    | hb_map (hb_కూడు (ఇది))
    | hb_map ([glyphs] (స్థిరరాశి Ligature<Types> &_) { 
      తిరిగి _.intersects_lig_glyph (glyphs) && _.intersects (glyphs);
    })
    | hb_any
    ;
  }

  శూన్య సంవృతం (hb_సంవృతం_context_t *c) స్థిరరాశి
  {
    + hb_iter (ligature)
    | hb_map (hb_కూడు (ఇది))
    | hb_apply ([c] (స్థిరరాశి Ligature<Types> &_) { _.సంవృతం (c); })
    ;
  }

  శూన్య depend (hb_depend_context_t *c, hb_codepoint_t మొదటి) స్థిరరాశి
  {
    + hb_iter (ligature)
    | hb_map (hb_కూడు (ఇది))
    | hb_apply ([&] (స్థిరరాశి Ligature<Types> &_) { _.depend (c, మొదటి); })
    ;
  }

  శూన్య collect_glyphs (hb_collect_glyphs_context_t *c) స్థిరరాశి
  {
    + hb_iter (ligature)
    | hb_map (hb_కూడు (ఇది))
    | hb_apply ([c] (స్థిరరాశి Ligature<Types> &_) { _.collect_glyphs (c); })
    ;
  }

  మూస <రకం_పేరు సమితి_t>
  శూన్య collect_seconds (సమితి_t &s) స్థిరరాశి
  {
    + hb_iter (ligature)
    | hb_map (hb_కూడు (ఇది))
    | hb_apply ([&s] (స్థిరరాశి Ligature<Types> &_) { _.collect_రెండవ (s); })
    ;
  }

  తార్కిక would_apply (hb_would_apply_context_t *c) స్థిరరాశి
  {
    తిరిగి
    + hb_iter (ligature)
    | hb_map (hb_కూడు (ఇది))
    | hb_map ([c] (స్థిరరాశి Ligature<Types> &_) { తిరిగి _.would_apply (c); })
    | hb_any
    ;
  }

  తార్కిక apply (hb_ot_apply_context_t *c, స్థిరరాశి hb_సమితి_digest_t *seconds = శూన్యసూచిక) స్థిరరాశి
  {
    TRACE_APPLY (ఇది);

    గుర్తులేని పూర్ణాంకం అంకె_ligs = ligature.len;

#నిర్వచితం_కాకపోతే HB_NO_OT_RULESETS_FAST_మార్గం
    ఒకవేళ (HB_OPTIMIZE_కొలత_VAL || అంకె_ligs <= 1)
#అయితే_ముగింపు
    {
    slow:
      కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < అంకె_ligs; i++)
      {
	స్థిరరాశి స్వయంచాలక &lig = ఇది+ligature.arrayZ[i];
	ఒకవేళ (lig.apply (c)) return_trace (నిజం);
      }
      return_trace (అబద్ధం);
    }

    /* This version is optimized for speed by matching the second component
     * of the ligature here, instead of calling into the ligation code.
     *
     * This is replicated in ChainRuleSet and RuleSet. */

    స్వయంచాలక &skippy_iter = c->iter_context;
    skippy_iter.reset (c->బఫరు->idx);
    skippy_iter.సమితి_సరిపోలిక_func (సరిపోలిక_always, శూన్యసూచిక);
    skippy_iter.సమితి_glyph_దత్తాంశం ((HBUINT16 *) శూన్యసూచిక);
    గుర్తులేని unsafe_గమ్యం;
    hb_codepoint_t second = (గుర్తులేని) -1;
    తార్కిక matched = skippy_iter.next (&unsafe_గమ్యం);
    ఒకవేళ (likely (matched))
    {
      second = c->బఫరు->info[skippy_iter.idx].codepoint;
      unsafe_గమ్యం = skippy_iter.idx + 1;

      ఒకవేళ (skippy_iter.may_skip (c->బఫరు->info[skippy_iter.idx]))
      {
	/* Can't use the fast path if eg. the next char is a default-ignorable
	 * or other skippable. */
        వెళ్ళు slow;
      }
    }
    లేకపోతే
      వెళ్ళు slow;

    ఒకవేళ (seconds && !seconds->may_have (second))
    {
      c->బఫరు->unsafe_గమ్యం_concat (c->బఫరు->idx, unsafe_గమ్యం);
      return_trace (అబద్ధం);
    }
    తార్కిక unsafe_గమ్యం_concat = అబద్ధం;
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < అంకె_ligs; i++)
    {
      స్థిరరాశి స్వయంచాలక &lig = ఇది+ligature.arrayZ[i];
      ఒకవేళ (unlikely (lig.component.lenP1 <= 1) ||
	  lig.component.arrayZ[0] == second)
      {
	ఒకవేళ (lig.apply (c))
	{
	  ఒకవేళ (unsafe_గమ్యం_concat)
	    c->బఫరు->unsafe_గమ్యం_concat (c->బఫరు->idx, unsafe_గమ్యం);
	  return_trace (నిజం);
	}
      }
      లేకపోతే ఒకవేళ (likely (lig.component.lenP1 > 1))
        unsafe_గమ్యం_concat = నిజం;
    }
    ఒకవేళ (likely (unsafe_గమ్యం_concat))
      c->బఫరు->unsafe_గమ్యం_concat (c->బఫరు->idx, unsafe_గమ్యం);

    return_trace (అబద్ధం);
  }

  మూస <రకం_పేరు GlyphID>
  తార్కిక serialize (hb_serialize_context_t *c,
                  hb_array_t<GlyphID> ligatures,
                  hb_array_t<స్థిరరాశి గుర్తులేని పూర్ణాంకం> component_లెక్క_list,
                  hb_array_t<GlyphID> &component_list /* Starting from second for each ligature */)
  {
    TRACE_SERIALIZE (ఇది);
    ఒకవేళ (unlikely (!c->extend_min (ఇది))) return_trace (అబద్ధం);
    ఒకవేళ (unlikely (!ligature.serialize (c, ligatures.నిడివి))) return_trace (అబద్ధం);
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < ligatures.నిడివి; i++)
    {
      గుర్తులేని పూర్ణాంకం component_లెక్క = (గుర్తులేని) hb_max ((పూర్ణాంకం) component_లెక్క_list[i] - 1, 0);
      ఒకవేళ (unlikely (!ligature[i].serialize_serialize (c,
                                                      ligatures[i],
                                                      component_list.sub_array (0, component_లెక్క))))
        return_trace (అబద్ధం);
      component_list += component_లెక్క;
    }
    return_trace (నిజం);
  }

  తార్కిక subset (hb_subset_context_t *c, గుర్తులేని coverage_idx) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    స్వయంచాలక *బహిర్గతం = c->serializer->ప్రారంభం_embed (*ఇది);
    ఒకవేళ (unlikely (!c->serializer->extend_min (బహిర్గతం))) return_trace (అబద్ధం);

    + hb_iter (ligature)
    | hb_filter (subset_స్థానభ్రంశం_array (c, బహిర్గతం->ligature, ఇది, coverage_idx))
    | hb_drain
    ;

    ఒకవేళ (తార్కిక (బహిర్గతం->ligature))
      // Ensure Coverage table is always packed after this.
      c->serializer->కూడు_virtual_link (coverage_idx);

    return_trace (తార్కిక (బహిర్గతం->ligature));
  }
};

}
}
}

#అయితే_ముగింపు  /* OT_LAYOUT_GSUB_LIGATURESET_HH */
