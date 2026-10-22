#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ OT_LAYOUT_GPOS_MARKRECORD_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OT_LAYOUT_GPOS_MARKRECORD_HH

ਨਾਮਾਕਾਸ਼ OT {
ਨਾਮਾਕਾਸ਼ Layout {
ਨਾਮਾਕਾਸ਼ GPOS_impl {

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Types> ਬਣਤਰ MarkArray;

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Types>
ਬਣਤਰ Mark_ਰਿਕਾਰਡ
{
  ਮਿੱਤਰ ਬਣਤਰ MarkArray<Types>;

  ਜਨਤਕ:
  HBUINT16      klass;                  /* Class defined for this mark */
  ਕਿਸਮ_ਨਾਂ Types::ਸਾਂਚਾ ਵਿੱਥ_ਮੰਜ਼ਿਲ<Anchor, MarkArray<Types>>
                markAnchor;             /* Offset to Anchor table--from
                                         * beginning of MarkArray table */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (2 + Types::ਮਾਪ);

  ਬਿਨਾਨਿਸ਼ਾਨੀ get_class () ਅਚਲ { ਵਾਪਸ (ਬਿਨਾਨਿਸ਼ਾਨੀ) klass; }
  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c, ਅਚਲ MarkArray<Types> *ਅਧਾਰ) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ) && markAnchor.sanitize (c, ਅਧਾਰ));
  }

  ਤਾਰਕਿਕ subset (hb_subset_context_t    *c,
	       ਅਚਲ MarkArray<Types> *src_ਅਧਾਰ,
	       ਅਚਲ hb_map_t         *klass_mapping) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->serializer->embed (ਇਹ);
    ਜੇ (unlikely (!ਬਾਹਰਲਾ)) return_trace (ਝੂਠ);

    ਬਾਹਰਲਾ->klass = klass_mapping->get (klass);
    return_trace (ਬਾਹਰਲਾ->markAnchor.serialize_subset (c, markAnchor, src_ਅਧਾਰ));
  }

  ਖਾਲੀ collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  ਅਚਲ MarkArray<Types> *src_ਅਧਾਰ) ਅਚਲ
  {
    (src_ਅਧਾਰ+markAnchor).collect_variation_indices (c);
  }
};


}
}
}

#ਜੇ_ਅੰਤ /* OT_LAYOUT_GPOS_MARKRECORD_HH */
