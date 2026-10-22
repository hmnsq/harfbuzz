#నిర్వచితం_కాకపోతే OT_LAYOUT_GPOS_LIGATUREARRAY_HH
#నిర్వచించు OT_LAYOUT_GPOS_LIGATUREARRAY_HH

నామాకాశ OT {
నామాకాశ Layout {
నామాకాశ GPOS_impl {

HB_అంతర్గత అంతఃవరుస
శూన్య collect_ligature_variation_indices (hb_collect_variation_indices_context_t *c,
                                        స్థిరరాశి Coverage &coverage,
                                        స్థిరరాశి శూన్య *పునాది,
                                        glyph_దత్తాంశం_t offsets,
                                        గుర్తులేని లెక్క,
                                        గుర్తులేని class_లెక్క,
                                        స్థిరరాశి hb_map_t &klass_mapping)
{
  కొరకు (స్థిరరాశి స్వయంచాలక _ : hb_zip (coverage, hb_range (లెక్క)))
  {
    ఒకవేళ (!c->glyph_సమితి->has (_.మొదటి)) కొనసాగు;
    hb_barrier ();
    గుర్తులేని స్థానభ్రంశం = offsets[_.second];
    ఒకవేళ (!స్థానభ్రంశం) కొనసాగు;

    స్థిరరాశి శూన్య *మాతృక = &Struct_At_స్థానభ్రంశం<స్థిరరాశి HBUINT16> (పునాది, స్థానభ్రంశం);
    /* LigatureAttach always has a 16-bit component count; its anchor
     * offsets have the same width as the LigatureArray offsets. */
    గుర్తులేని అడ్డువరుసలు = *static_cast<స్థిరరాశి HBUINT16 *> (మాతృక);
    glyph_దత్తాంశం_t anchors = offsets;
    anchors.దత్తాంశం = (స్థిరరాశి అక్షరం *) మాతృక + HBUINT16::static_కొలత;

    hb_sorted_సదిశ_t<గుర్తులేని> indexes;
    collect_ligature_anchor_indexes (అడ్డువరుసలు, class_లెక్క, klass_mapping, indexes);
    collect_anchor_variation_indices (c, మాతృక, anchors, indexes.iter ());
  }
}


/* Array of LigatureAttach tables ordered by LigatureCoverage Index */
మూస <రకం_పేరు Types>
నిర్మాణం LigatureArray :
  Types::మూస Array_యొక్క<రకం_పేరు Types::మూస స్థానభ్రంశం_గమ్యం<Anchor_మాతృక<Types, HBUINT16>>>
{
  రకనిర్వచనం Anchor_మాతృక<Types, HBUINT16> LigatureAttach;
  రకనిర్వచనం రకం_పేరు Types::మూస స్థానభ్రంశం_గమ్యం<LigatureAttach> Ligature_Attach_స్థానభ్రంశం;
  రకనిర్వచనం రకం_పేరు Types::మూస Array_యొక్క<Ligature_Attach_స్థానభ్రంశం> Super;

  స్థిరరాశి LigatureAttach& క్రియాకారకం [] (పూర్ణాంకం i_) స్థిరరాశి
  {
    గుర్తులేని పూర్ణాంకం i = (గుర్తులేని పూర్ణాంకం) i_;
    ఒకవేళ (unlikely (i >= ఇది->len)) తిరిగి Null (LigatureAttach);
    hb_barrier ();
    తిరిగి ఇది+ఇది->arrayZ[i];
  }

  మూస <రకం_పేరు ...Ts>
  తార్కిక sanitize (hb_sanitize_context_t *c, Ts&&... ds) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (Super::sanitize (c, ఇది, std::forward<Ts> (ds)...));
  }

  శూన్య collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  స్థిరరాశి Coverage &coverage,
                                  గుర్తులేని class_లెక్క,
                                  స్థిరరాశి hb_map_t &klass_mapping) స్థిరరాశి
  {
    collect_ligature_variation_indices (c, coverage, ఇది, ఇది->arrayZ,
                                        ఇది->len, class_లెక్క, klass_mapping);
  }

  మూస <రకం_పేరు Iterator,
            hb_requires (hb_is_iterator (Iterator))>
  తార్కిక subset (hb_subset_context_t *c,
               Iterator             coverage,
               గుర్తులేని             class_లెక్క,
               స్థిరరాశి hb_map_t      *klass_mapping,
               hb_sorted_సదిశ_t<hb_codepoint_t> &new_coverage /*బహిర్గతం*/) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    స్థిరరాశి hb_map_t &glyph_map = c->plan->glyph_map_gsub;

    స్వయంచాలక *బహిర్గతం = c->serializer->ప్రారంభం_embed (ఇది);
    ఒకవేళ (unlikely (!c->serializer->extend_min (బహిర్గతం)))  return_trace (అబద్ధం);

    తార్కిక ret = అబద్ధం;
    కొరకు (స్థిరరాశి స్వయంచాలక _ : + hb_zip (coverage, *ఇది)
                        | hb_filter (glyph_map, hb_మొదటి))
    {
      స్థిరరాశి LigatureAttach& src = (ఇది + _.second);
      తార్కిక non_ఖాళీలు = + hb_range (src.అడ్డువరుసలు * class_లెక్క)
                       | hb_filter ([=] (గుర్తులేని సూచీ_2) { తిరిగి klass_mapping->has (సూచీ_2 % class_లెక్క); })
                       | hb_map ([&] (స్థిరరాశి గుర్తులేని సూచీ_2) { తిరిగి !src.స్థానభ్రంశం_is_null (సూచీ_2 / class_లెక్క, సూచీ_2 % class_లెక్క, class_లెక్క); })
                       | hb_any;

      ఒకవేళ (!non_ఖాళీలు) కొనసాగు;

      స్వయంచాలక *మాతృక = బహిర్గతం->serialize_append (c->serializer);
      ఒకవేళ (unlikely (!మాతృక)) return_trace (అబద్ధం);

      స్వయంచాలక indexes =
          + hb_range (src.అడ్డువరుసలు * class_లెక్క)
          | hb_filter ([=] (గుర్తులేని సూచీ_2) { తిరిగి klass_mapping->has (సూచీ_2 % class_లెక్క); })
          ;
      ret |= మాతృక->serialize_subset (c,
				       _.second,
				       ఇది,
				       src.అడ్డువరుసలు,
				       indexes);

      hb_codepoint_t new_gid = glyph_map.get (_.మొదటి);
      new_coverage.push (new_gid);
    }
    return_trace (ret);
  }
};


}
}
}

#అయితే_ముగింపు /* OT_LAYOUT_GPOS_LIGATUREARRAY_HH */
