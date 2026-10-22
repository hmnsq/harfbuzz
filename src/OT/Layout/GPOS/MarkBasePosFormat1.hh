#నిర్వచితం_కాకపోతే OT_LAYOUT_GPOS_MARKBASEPOSFORMAT1_HH
#నిర్వచించు OT_LAYOUT_GPOS_MARKBASEPOSFORMAT1_HH

#అన్తర్భావ "MarkArray.hh"

నామాకాశ OT {
నామాకాశ Layout {
నామాకాశ GPOS_impl {

మూస <రకం_పేరు Types>
నిర్మాణం Mark_పునాది_Pos_Format1_2
{
  రక్షిత:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  రకం_పేరు Types::మూస LOffset_గమ్యం<Coverage>
                markCoverage;           /* Offset to MarkCoverage table--from
                                         * beginning of MarkBasePos subtable */
  రకం_పేరు Types::మూస LOffset_గమ్యం<Coverage>
                పునాది_Coverage;           /* Offset to BaseCoverage table--from
                                         * beginning of MarkBasePos subtable */
  HBUINT16      class_లెక్క_2;             /* Number of classes defined for marks */
  రకం_పేరు Types::మూస LOffset_గమ్యం<MarkArray<Types>>
                markArray;              /* Offset to MarkArray table--from
                                         * beginning of MarkBasePos subtable */
  రకం_పేరు Types::మూస LOffset_గమ్యం<Anchor_మాతృక<Types>>
                పునాది_Array;              /* Offset to BaseArray table--from
                                         * beginning of MarkBasePos subtable */

  బహిరంగ:
  DEFINE_కొలత_STATIC (4 + 4 * Types::LOffset::static_కొలత);

    తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది) &&
                  markCoverage.sanitize (c, ఇది) &&
                  పునాది_Coverage.sanitize (c, ఇది) &&
                  markArray.sanitize (c, ఇది) &&
                  పునాది_Array.sanitize (c, ఇది, (గుర్తులేని పూర్ణాంకం) class_లెక్క_2));
  }

  తార్కిక intersects (స్థిరరాశి hb_సమితి_t *glyphs) స్థిరరాశి
  {
    తిరిగి (ఇది+markCoverage).intersects (glyphs) &&
           (ఇది+పునాది_Coverage).intersects (glyphs);
  }

  శూన్య సంవృతం_lookups (hb_సంవృతం_lookups_context_t *c) స్థిరరాశి {}

  శూన్య collect_variation_indices (hb_collect_variation_indices_context_t *c) స్థిరరాశి
  {
    (ఇది+markArray).collect_variation_indices (c, ఇది+markCoverage);

    hb_map_t klass_mapping;
    Markclass_సంవృతం_and_remap_indexes (ఇది+markCoverage, ఇది+markArray, *c->glyph_సమితి, &klass_mapping);

    hb_sorted_సదిశ_t<గుర్తులేని> పునాది_indexes;
    collect_anchor_indexes (ఇది+పునాది_Coverage, (ఇది+పునాది_Array).అడ్డువరుసలు,
                            class_లెక్క_2, *c->glyph_సమితి, klass_mapping,
                            పునాది_indexes);
    (ఇది+పునాది_Array).collect_variation_indices (c, పునాది_indexes.iter ());
  }

  శూన్య collect_glyphs (hb_collect_glyphs_context_t *c) స్థిరరాశి
  {
    ఒకవేళ (unlikely (!(ఇది+markCoverage).collect_coverage (c->ఆదానం))) తిరిగి;
    ఒకవేళ (unlikely (!(ఇది+పునాది_Coverage).collect_coverage (c->ఆదానం))) తిరిగి;
  }

  శూన్య collect_రెండవ_glyphs (hb_సమితి_digest_t *digest) స్థిరరాశి
  { (ఇది+పునాది_Coverage).collect_coverage (digest); }

  స్థిరరాశి Coverage &get_coverage () స్థిరరాశి { తిరిగి ఇది+markCoverage; }

  స్థిర అంతఃవరుస తార్కిక accept (hb_బఫరు_t *బఫరు, గుర్తులేని idx)
  {
    /* We only want to attach to the first of a MultipleSubst sequence.
     * https://github.com/harfbuzz/harfbuzz/issues/740
     * Reject others...
     * ...but stop if we find a mark in the MultipleSubst sequence:
     * https://github.com/harfbuzz/harfbuzz/issues/1020 */
    తిరిగి !_hb_glyph_info_multiplied (&బఫరు->info[idx]) ||
	   0 == _hb_glyph_info_get_lig_comp (&బఫరు->info[idx]) ||
	   (idx == 0 ||
	    _hb_glyph_info_is_mark (&బఫరు->info[idx - 1]) ||
	    !_hb_glyph_info_multiplied (&బఫరు->info[idx - 1]) ||
	    _hb_glyph_info_get_lig_id (&బఫరు->info[idx]) !=
	    _hb_glyph_info_get_lig_id (&బఫరు->info[idx - 1]) ||
	    _hb_glyph_info_get_lig_comp (&బఫరు->info[idx]) !=
	    _hb_glyph_info_get_lig_comp (&బఫరు->info[idx - 1]) + 1
	    );
  }

  తార్కిక apply (hb_ot_apply_context_t *c) స్థిరరాశి
  {
    TRACE_APPLY (ఇది);
    hb_బఫరు_t *బఫరు = c->బఫరు;
    గుర్తులేని పూర్ణాంకం mark_సూచీ = (ఇది+markCoverage).get_coverage  (బఫరు->cur().codepoint);
    ఒకవేళ (likely (mark_సూచీ == NOT_COVERED)) return_trace (అబద్ధం);

    /* Now we search backwards for a non-mark glyph.
     * We don't use skippy_iter.prev() to avoid O(n^2) behavior. */

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
	    NOT_COVERED == (ఇది+పునాది_Coverage).get_coverage  (బఫరు->info[j - 1].codepoint))
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

    /* Checking that matched glyph is actually a base glyph by GDEF is too strong; disabled */
    //if (!_hb_glyph_info_is_base_glyph (&buffer->info[idx])) { return_trace (false); }

    గుర్తులేని పూర్ణాంకం పునాది_సూచీ = (ఇది+పునాది_Coverage).get_coverage  (బఫరు->info[idx].codepoint);
    ఒకవేళ (పునాది_సూచీ == NOT_COVERED)
    {
      బఫరు->unsafe_గమ్యం_concat_from_outbuffer (idx, బఫరు->idx + 1);
      return_trace (అబద్ధం);
    }

    return_trace ((ఇది+markArray).apply (c, mark_సూచీ, పునాది_సూచీ, ఇది+పునాది_Array, class_లెక్క_2, idx));
  }

  తార్కిక subset (hb_subset_context_t *c) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    స్థిరరాశి hb_సమితి_t &glyphset = *c->plan->glyphset_gsub ();
    స్థిరరాశి hb_map_t &glyph_map = *c->plan->glyph_map;

    స్వయంచాలక *బహిర్గతం = c->serializer->ప్రారంభం_embed (*ఇది);
    ఒకవేళ (unlikely (!c->serializer->extend_min (బహిర్గతం))) return_trace (అబద్ధం);
    బహిర్గతం->format = format;

    hb_map_t klass_mapping;
    Markclass_సంవృతం_and_remap_indexes (ఇది+markCoverage, ఇది+markArray, glyphset, &klass_mapping);

    ఒకవేళ (!klass_mapping.get_population ()) return_trace (అబద్ధం);
    బహిర్గతం->class_లెక్క_2 = klass_mapping.get_population ();

    స్వయంచాలక mark_iter =
    + hb_zip (ఇది+markCoverage, ఇది+markArray)
    | hb_filter (glyphset, hb_మొదటి)
    ;

    hb_sorted_సదిశ_t<hb_codepoint_t> new_coverage;
    + mark_iter
    | hb_map (hb_మొదటి)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    ఒకవేళ (!బహిర్గతం->markCoverage.serialize_serialize (c->serializer, new_coverage.iter ()))
      return_trace (అబద్ధం);

    ఒకవేళ (unlikely (!బహిర్గతం->markArray.serialize_subset (c, markArray, ఇది,
						    (ఇది+markCoverage).iter (),
						    &klass_mapping)))
      return_trace (అబద్ధం);

    new_coverage.reset ();
    hb_sorted_సదిశ_t<గుర్తులేని> పునాది_indexes;
    స్థిరరాశి స్వయంచాలక &పునాది_array = ఇది+పునాది_Array;
    collect_subset_anchor_indexes (ఇది+పునాది_Coverage, పునాది_array.అడ్డువరుసలు,
                                   class_లెక్క_2, పునాది_array.మాతృక_Z.arrayZ,
                                   glyphset, glyph_map, klass_mapping,
                                   new_coverage, పునాది_indexes);

    ఒకవేళ (!new_coverage) return_trace (అబద్ధం);
    ఒకవేళ (!బహిర్గతం->పునాది_Coverage.serialize_serialize (c->serializer, new_coverage.iter ()))
      return_trace (అబద్ధం);

    return_trace (బహిర్గతం->పునాది_Array.serialize_subset (c, పునాది_Array, ఇది,
						   new_coverage.నిడివి,
						   పునాది_indexes.iter ()));
  }
};


}
}
}

#అయితే_ముగింపు /* OT_LAYOUT_GPOS_MARKBASEPOSFORMAT1_HH */
