#నిర్వచితం_కాకపోతే OT_LAYOUT_GPOS_MARKLIGPOSFORMAT1_HH
#నిర్వచించు OT_LAYOUT_GPOS_MARKLIGPOSFORMAT1_HH

#అన్తర్భావ "LigatureArray.hh"

నామాకాశ OT {
నామాకాశ Layout {
నామాకాశ GPOS_impl {


మూస <రకం_పేరు Types>
నిర్మాణం MarkLigPosFormat1_2
{
  రక్షిత:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  రకం_పేరు Types::మూస LOffset_గమ్యం<Coverage>
                markCoverage;           /* Offset to Mark Coverage table--from
                                         * beginning of MarkLigPos subtable */
  రకం_పేరు Types::మూస LOffset_గమ్యం<Coverage>
                ligatureCoverage;       /* Offset to Ligature Coverage
                                         * table--from beginning of MarkLigPos
                                         * subtable */
  HBUINT16      class_లెక్క_2;             /* Number of defined mark classes */
  రకం_పేరు Types::మూస LOffset_గమ్యం<MarkArray<Types>>
                markArray;              /* Offset to MarkArray table--from
                                         * beginning of MarkLigPos subtable */
  రకం_పేరు Types::మూస LOffset_గమ్యం<LigatureArray<Types>>
                ligatureArray;          /* Offset to LigatureArray table--from
                                         * beginning of MarkLigPos subtable */
  బహిరంగ:
  DEFINE_కొలత_STATIC (4 + 4 * Types::LOffset::static_కొలత);

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది) &&
                  markCoverage.sanitize (c, ఇది) &&
                  ligatureCoverage.sanitize (c, ఇది) &&
                  markArray.sanitize (c, ఇది) &&
                  ligatureArray.sanitize (c, ఇది, (గుర్తులేని పూర్ణాంకం) class_లెక్క_2));
  }

  తార్కిక intersects (స్థిరరాశి hb_సమితి_t *glyphs) స్థిరరాశి
  {
    తిరిగి (ఇది+markCoverage).intersects (glyphs) &&
           (ఇది+ligatureCoverage).intersects (glyphs);
  }

  శూన్య సంవృతం_lookups (hb_సంవృతం_lookups_context_t *c) స్థిరరాశి {}

  శూన్య collect_variation_indices (hb_collect_variation_indices_context_t *c) స్థిరరాశి
  {
    (ఇది+markArray).collect_variation_indices (c, ఇది+markCoverage);

    hb_map_t klass_mapping;
    Markclass_సంవృతం_and_remap_indexes (ఇది+markCoverage, ఇది+markArray, *c->glyph_సమితి, &klass_mapping);

    (ఇది+ligatureArray).collect_variation_indices (c, ఇది+ligatureCoverage,
                                                   class_లెక్క_2, klass_mapping);
  }

  శూన్య collect_glyphs (hb_collect_glyphs_context_t *c) స్థిరరాశి
  {
    ఒకవేళ (unlikely (!(ఇది+markCoverage).collect_coverage (c->ఆదానం))) తిరిగి;
    ఒకవేళ (unlikely (!(ఇది+ligatureCoverage).collect_coverage (c->ఆదానం))) తిరిగి;
  }

  శూన్య collect_రెండవ_glyphs (hb_సమితి_digest_t *digest) స్థిరరాశి
  { (ఇది+ligatureCoverage).collect_coverage (digest); }

  స్థిరరాశి Coverage &get_coverage () స్థిరరాశి { తిరిగి ఇది+markCoverage; }

  స్థిర అంతఃవరుస తార్కిక accept (hb_బఫరు_t *బఫరు, గుర్తులేని idx)
  {
    /* We only want to attach to the first of a MultipleSubst sequence,
     * which might have been ligated into a preceding ligature, and in that
     * case the mark should attach to that ligature.
     * https://github.com/harfbuzz/harfbuzz/issues/4969
     * Reject others... */
    తిరిగి !_hb_glyph_info_multiplied (&బఫరు->info[idx]) ||
	   0 == _hb_glyph_info_get_lig_comp (&బఫరు->info[idx]);
  }

  తార్కిక apply (hb_ot_apply_context_t *c) స్థిరరాశి
  {
    TRACE_APPLY (ఇది);
    hb_బఫరు_t *బఫరు = c->బఫరు;
    గుర్తులేని పూర్ణాంకం mark_సూచీ = (ఇది+markCoverage).get_coverage  (బఫరు->cur().codepoint);
    ఒకవేళ (likely (mark_సూచీ == NOT_COVERED)) return_trace (అబద్ధం);

    /* Now we search backwards for a non-mark glyph */

    స్వయంచాలక &skippy_iter = c->iter_ఆదానం;
    skippy_iter.సమితి_lookup_props (LookupFlag::IgnoreMarks);

    ఒకవేళ (c->చివరి_పునాది_until > బఫరు->idx)
    {
      c->చివరి_పునాది_until = 0;
      c->చివరి_పునాది = -1;
    }
    గుర్తులేని j;
    కొరకు (j = బఫరు->idx; j > c->చివరి_పునాది_until; j--)
    {
      స్వయంచాలక సరిపోలిక = skippy_iter.సరిపోలిక (బఫరు->info[j - 1]);
      ఒకవేళ (సరిపోలిక == skippy_iter.సరిపోలిక_2)
      {
        // https://github.com/harfbuzz/harfbuzz/issues/4124
	ఒకవేళ (!accept (బఫరు, j - 1) &&
	    NOT_COVERED == (ఇది+ligatureCoverage).get_coverage  (బఫరు->info[j - 1].codepoint))
	  సరిపోలిక = skippy_iter.SKIP;
      }
      ఒకవేళ (సరిపోలిక == skippy_iter.సరిపోలిక_2)
      {
	c->చివరి_పునాది = (గుర్తుగల) j - 1;
	ఆపు;
      }
    }
    c->చివరి_పునాది_until = బఫరు->idx;
    ఒకవేళ (c->చివరి_పునాది == -1)
    {
      బఫరు->unsafe_గమ్యం_concat_from_outbuffer (0, బఫరు->idx + 1);
      return_trace (అబద్ధం);
    }

    గుర్తులేని idx = (గుర్తులేని) c->చివరి_పునాది;

    /* Checking that matched glyph is actually a ligature by GDEF is too strong; disabled */
    //if (!_hb_glyph_info_is_ligature (&buffer->info[idx])) { return_trace (false); }

    గుర్తులేని పూర్ణాంకం lig_సూచీ = (ఇది+ligatureCoverage).get_coverage  (బఫరు->info[idx].codepoint);
    ఒకవేళ (lig_సూచీ == NOT_COVERED)
    {
      బఫరు->unsafe_గమ్యం_concat_from_outbuffer (idx, బఫరు->idx + 1);
      return_trace (అబద్ధం);
    }

    స్థిరరాశి LigatureArray<Types>& lig_array = ఇది+ligatureArray;
    స్థిరరాశి Anchor_మాతృక<Types, HBUINT16>& lig_attach = lig_array[lig_సూచీ];

    /* Find component to attach to */
    గుర్తులేని పూర్ణాంకం comp_లెక్క = lig_attach.అడ్డువరుసలు;
    ఒకవేళ (unlikely (!comp_లెక్క))
    {
      బఫరు->unsafe_గమ్యం_concat_from_outbuffer (idx, బఫరు->idx + 1);
      return_trace (అబద్ధం);
    }

    /* We must now check whether the ligature ID of the current mark glyph
     * is identical to the ligature ID of the found ligature.  If yes, we
     * can directly use the component index.  If not, we attach the mark
     * glyph to the last component of the ligature. */
    గుర్తులేని పూర్ణాంకం comp_సూచీ;
    గుర్తులేని పూర్ణాంకం lig_id = _hb_glyph_info_get_lig_id (&బఫరు->info[idx]);
    గుర్తులేని పూర్ణాంకం mark_id = _hb_glyph_info_get_lig_id (&బఫరు->cur());
    గుర్తులేని పూర్ణాంకం mark_comp = _hb_glyph_info_get_lig_comp (&బఫరు->cur());
    ఒకవేళ (lig_id && lig_id == mark_id && mark_comp > 0)
      comp_సూచీ = hb_min (comp_లెక్క, _hb_glyph_info_get_lig_comp (&బఫరు->cur())) - 1;
    లేకపోతే
      comp_సూచీ = comp_లెక్క - 1;

    return_trace ((ఇది+markArray).apply (c, mark_సూచీ, comp_సూచీ, lig_attach, class_లెక్క_2, idx));
  }

  తార్కిక subset (hb_subset_context_t *c) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    స్థిరరాశి hb_సమితి_t &glyphset = *c->plan->glyphset_gsub ();
    స్థిరరాశి hb_map_t &glyph_map = c->plan->glyph_map_gsub;

    స్వయంచాలక *బహిర్గతం = c->serializer->ప్రారంభం_embed (*ఇది);
    ఒకవేళ (unlikely (!c->serializer->extend_min (బహిర్గతం))) return_trace (అబద్ధం);
    బహిర్గతం->format = format;

    hb_map_t klass_mapping;
    Markclass_సంవృతం_and_remap_indexes (ఇది+markCoverage, ఇది+markArray, glyphset, &klass_mapping);

    ఒకవేళ (!klass_mapping.get_population ()) return_trace (అబద్ధం);
    బహిర్గతం->class_లెక్క_2 = klass_mapping.get_population ();

    స్వయంచాలక mark_iter =
    + hb_zip (ఇది+markCoverage, hb_range ((గుర్తులేని) (ఇది+markArray).len))
    | hb_filter (glyphset, hb_మొదటి)
    ;

    స్వయంచాలక new_mark_coverage =
    + mark_iter
    | hb_map_retains_క్రమీకరణ (hb_మొదటి)
    | hb_map_retains_క్రమీకరణ (glyph_map)
    ;

    ఒకవేళ (!బహిర్గతం->markCoverage.serialize_serialize (c->serializer, new_mark_coverage))
      return_trace (అబద్ధం);

    ఒకవేళ (unlikely (!బహిర్గతం->markArray.serialize_subset (c, markArray, ఇది,
						    (ఇది+markCoverage).iter (),
						    &klass_mapping)))
      return_trace (అబద్ధం);

    hb_sorted_సదిశ_t<hb_codepoint_t> new_lig_coverage;
    ఒకవేళ (!బహిర్గతం->ligatureArray.serialize_subset (c, ligatureArray, ఇది,
					      hb_iter (ఇది+ligatureCoverage),
					      class_లెక్క_2, &klass_mapping, new_lig_coverage))
      return_trace (అబద్ధం);

    return_trace (బహిర్గతం->ligatureCoverage.serialize_serialize (c->serializer, new_lig_coverage.iter ()));
  }

};

}
}
}

#అయితే_ముగింపు /* OT_LAYOUT_GPOS_MARKLIGPOSFORMAT1_HH */
