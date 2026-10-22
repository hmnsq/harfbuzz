#నిర్వచితం_కాకపోతే OT_LAYOUT_GPOS_MARKMARKPOSFORMAT1_HH
#నిర్వచించు OT_LAYOUT_GPOS_MARKMARKPOSFORMAT1_HH

#అన్తర్భావ "MarkMarkPosFormat1.hh"

నామాకాశ OT {
నామాకాశ Layout {
నామాకాశ GPOS_impl {

మూస <రకం_పేరు Types>
నిర్మాణం MarkMarkPosFormat1_2
{
  రక్షిత:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  రకం_పేరు Types::మూస LOffset_గమ్యం<Coverage>
                mark1Coverage;          /* Offset to Combining Mark1 Coverage
                                         * table--from beginning of MarkMarkPos
                                         * subtable */
  రకం_పేరు Types::మూస LOffset_గమ్యం<Coverage>
                mark2Coverage;          /* Offset to Combining Mark2 Coverage
                                         * table--from beginning of MarkMarkPos
                                         * subtable */
  HBUINT16      class_లెక్క_2;             /* Number of defined mark classes */
  రకం_పేరు Types::మూస LOffset_గమ్యం<MarkArray<Types>>
                mark1Array;             /* Offset to Mark1Array table--from
                                         * beginning of MarkMarkPos subtable */
  రకం_పేరు Types::మూస LOffset_గమ్యం<Anchor_మాతృక<Types, HBUINT16>>
                mark2Array;             /* Offset to Mark2Array table--from
                                         * beginning of MarkMarkPos subtable */
  బహిరంగ:
  DEFINE_కొలత_STATIC (4 + 4 * Types::LOffset::static_కొలత);

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది) &&
                  mark1Coverage.sanitize (c, ఇది) &&
                  mark2Coverage.sanitize (c, ఇది) &&
                  mark1Array.sanitize (c, ఇది) &&
		  hb_barrier () &&
                  mark2Array.sanitize (c, ఇది, (గుర్తులేని పూర్ణాంకం) class_లెక్క_2));
  }

  తార్కిక intersects (స్థిరరాశి hb_సమితి_t *glyphs) స్థిరరాశి
  {
    తిరిగి (ఇది+mark1Coverage).intersects (glyphs) &&
           (ఇది+mark2Coverage).intersects (glyphs);
  }

  శూన్య సంవృతం_lookups (hb_సంవృతం_lookups_context_t *c) స్థిరరాశి {}

  శూన్య collect_variation_indices (hb_collect_variation_indices_context_t *c) స్థిరరాశి
  {
    (ఇది+mark1Array).collect_variation_indices (c, ఇది+mark1Coverage);

    hb_map_t klass_mapping;
    Markclass_సంవృతం_and_remap_indexes (ఇది+mark1Coverage, ఇది+mark1Array, *c->glyph_సమితి, &klass_mapping);

    hb_sorted_సదిశ_t<గుర్తులేని> mark2_indexes;
    collect_anchor_indexes (ఇది+mark2Coverage, (ఇది+mark2Array).అడ్డువరుసలు,
                            class_లెక్క_2, *c->glyph_సమితి, klass_mapping,
                            mark2_indexes);
    (ఇది+mark2Array).collect_variation_indices (c, mark2_indexes.iter ());
  }

  శూన్య collect_glyphs (hb_collect_glyphs_context_t *c) స్థిరరాశి
  {
    ఒకవేళ (unlikely (!(ఇది+mark1Coverage).collect_coverage (c->ఆదానం))) తిరిగి;
    ఒకవేళ (unlikely (!(ఇది+mark2Coverage).collect_coverage (c->ఆదానం))) తిరిగి;
  }

  శూన్య collect_రెండవ_glyphs (hb_సమితి_digest_t *digest) స్థిరరాశి
  { (ఇది+mark2Coverage).collect_coverage (digest); }

  స్థిరరాశి Coverage &get_coverage () స్థిరరాశి { తిరిగి ఇది+mark1Coverage; }

  తార్కిక apply (hb_ot_apply_context_t *c) స్థిరరాశి
  {
    TRACE_APPLY (ఇది);
    hb_బఫరు_t *బఫరు = c->బఫరు;
    గుర్తులేని పూర్ణాంకం mark1_సూచీ = (ఇది+mark1Coverage).get_coverage  (బఫరు->cur().codepoint);
    ఒకవేళ (likely (mark1_సూచీ == NOT_COVERED)) return_trace (అబద్ధం);

    /* now we search backwards for a suitable mark glyph until a non-mark glyph */
    స్వయంచాలక &skippy_iter = c->iter_ఆదానం;
    skippy_iter.reset_fast (బఫరు->idx);
    skippy_iter.సమితి_lookup_props (c->lookup_props & ~(uint32_t)LookupFlag::IgnoreFlags);
    గుర్తులేని unsafe_from;
    ఒకవేళ (unlikely (!skippy_iter.prev (&unsafe_from)))
    {
      బఫరు->unsafe_గమ్యం_concat_from_outbuffer (unsafe_from, బఫరు->idx + 1);
      return_trace (అబద్ధం);
    }

    ఒకవేళ (likely (!_hb_glyph_info_is_mark (&బఫరు->info[skippy_iter.idx])))
    {
      బఫరు->unsafe_గమ్యం_concat_from_outbuffer (skippy_iter.idx, బఫరు->idx + 1);
      return_trace (అబద్ధం);
    }

    గుర్తులేని పూర్ణాంకం j = skippy_iter.idx;

    గుర్తులేని పూర్ణాంకం id1 = _hb_glyph_info_get_lig_id (&బఫరు->cur());
    గుర్తులేని పూర్ణాంకం id2 = _hb_glyph_info_get_lig_id (&బఫరు->info[j]);
    గుర్తులేని పూర్ణాంకం comp1 = _hb_glyph_info_get_lig_comp (&బఫరు->cur());
    గుర్తులేని పూర్ణాంకం comp2 = _hb_glyph_info_get_lig_comp (&బఫరు->info[j]);

    ఒకవేళ (likely (id1 == id2))
    {
      ఒకవేళ (id1 == 0) /* Marks belonging to the same base. */
        వెళ్ళు good;
      లేకపోతే ఒకవేళ (comp1 == comp2) /* Marks belonging to the same ligature component. */
        వెళ్ళు good;
    }
    లేకపోతే
    {
      /* If ligature ids don't match, it may be the case that one of the marks
       * itself is a ligature.  In which case match. */
      ఒకవేళ ((id1 > 0 && !comp1) || (id2 > 0 && !comp2))
        వెళ్ళు good;
    }

    /* Didn't match. */
    బఫరు->unsafe_గమ్యం_concat_from_outbuffer (skippy_iter.idx, బఫరు->idx + 1);
    return_trace (అబద్ధం);

    good:
    గుర్తులేని పూర్ణాంకం mark2_సూచీ = (ఇది+mark2Coverage).get_coverage  (బఫరు->info[j].codepoint);
    ఒకవేళ (mark2_సూచీ == NOT_COVERED)
    {
      బఫరు->unsafe_గమ్యం_concat_from_outbuffer (skippy_iter.idx, బఫరు->idx + 1);
      return_trace (అబద్ధం);
    }

    return_trace ((ఇది+mark1Array).apply (c, mark1_సూచీ, mark2_సూచీ, ఇది+mark2Array, class_లెక్క_2, j));
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
    Markclass_సంవృతం_and_remap_indexes (ఇది+mark1Coverage, ఇది+mark1Array, glyphset, &klass_mapping);

    ఒకవేళ (!klass_mapping.get_population ()) return_trace (అబద్ధం);
    బహిర్గతం->class_లెక్క_2 = klass_mapping.get_population ();

    స్వయంచాలక mark1_iter =
    + hb_zip (ఇది+mark1Coverage, ఇది+mark1Array)
    | hb_filter (glyphset, hb_మొదటి)
    ;

    hb_sorted_సదిశ_t<hb_codepoint_t> new_coverage;
    + mark1_iter
    | hb_map (hb_మొదటి)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    ఒకవేళ (!బహిర్గతం->mark1Coverage.serialize_serialize (c->serializer, new_coverage.iter ()))
      return_trace (అబద్ధం);

    ఒకవేళ (unlikely (!బహిర్గతం->mark1Array.serialize_subset (c, mark1Array, ఇది,
						     (ఇది+mark1Coverage).iter (),
						     &klass_mapping)))
      return_trace (అబద్ధం);

    new_coverage.reset ();
    hb_sorted_సదిశ_t<గుర్తులేని> mark2_indexes;
    స్థిరరాశి స్వయంచాలక &mark2_array = ఇది+mark2Array;
    collect_subset_anchor_indexes (ఇది+mark2Coverage, mark2_array.అడ్డువరుసలు,
                                   class_లెక్క_2, mark2_array.మాతృక_Z.arrayZ,
                                   glyphset, glyph_map, klass_mapping,
                                   new_coverage, mark2_indexes);

    ఒకవేళ (!new_coverage) return_trace (అబద్ధం);
    ఒకవేళ (!బహిర్గతం->mark2Coverage.serialize_serialize (c->serializer, new_coverage.iter ()))
      return_trace (అబద్ధం);

    return_trace (బహిర్గతం->mark2Array.serialize_subset (c, mark2Array, ఇది,
						    new_coverage.నిడివి,
						    mark2_indexes.iter ()));

  }
};


}
}
}

#అయితే_ముగింపు /* OT_LAYOUT_GPOS_MARKMARKPOSFORMAT1_HH */
