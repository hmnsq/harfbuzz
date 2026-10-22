#నిర్వచితం_కాకపోతే OT_LAYOUT_GPOS_MARKRECORD_HH
#నిర్వచించు OT_LAYOUT_GPOS_MARKRECORD_HH

నామాకాశ OT {
నామాకాశ Layout {
నామాకాశ GPOS_impl {

మూస <రకం_పేరు Types> నిర్మాణం MarkArray;

మూస <రకం_పేరు Types>
నిర్మాణం Mark_రికార్డు
{
  మిత్ర నిర్మాణం MarkArray<Types>;

  బహిరంగ:
  HBUINT16      klass;                  /* Class defined for this mark */
  రకం_పేరు Types::మూస స్థానభ్రంశం_గమ్యం<Anchor, MarkArray<Types>>
                markAnchor;             /* Offset to Anchor table--from
                                         * beginning of MarkArray table */
  బహిరంగ:
  DEFINE_కొలత_STATIC (2 + Types::కొలత);

  గుర్తులేని get_class () స్థిరరాశి { తిరిగి (గుర్తులేని) klass; }
  తార్కిక sanitize (hb_sanitize_context_t *c, స్థిరరాశి MarkArray<Types> *పునాది) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది) && markAnchor.sanitize (c, పునాది));
  }

  తార్కిక subset (hb_subset_context_t    *c,
	       స్థిరరాశి MarkArray<Types> *src_పునాది,
	       స్థిరరాశి hb_map_t         *klass_mapping) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    స్వయంచాలక *బహిర్గతం = c->serializer->embed (ఇది);
    ఒకవేళ (unlikely (!బహిర్గతం)) return_trace (అబద్ధం);

    బహిర్గతం->klass = klass_mapping->get (klass);
    return_trace (బహిర్గతం->markAnchor.serialize_subset (c, markAnchor, src_పునాది));
  }

  శూన్య collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  స్థిరరాశి MarkArray<Types> *src_పునాది) స్థిరరాశి
  {
    (src_పునాది+markAnchor).collect_variation_indices (c);
  }
};


}
}
}

#అయితే_ముగింపు /* OT_LAYOUT_GPOS_MARKRECORD_HH */
