#ifndef OT_LAYOUT_GPOS_MARKRECORD_HH
#define OT_LAYOUT_GPOS_MARKRECORD_HH

नामाकाश OT {
नामाकाश Layout {
नामाकाश GPOS_impl {

template <typename Types> संरचना MarkArray;

template <typename Types>
संरचना MarkRecord
{
  friend संरचना MarkArray<Types>;

  सार्वजनिक:
  HBUINT16      klass;                  /* Class defined for this mark */
  typename Types::template OffsetTo<Anchor, MarkArray<Types>>
                markAnchor;             /* Offset to Anchor table--from
                                         * beginning of MarkArray table */
  सार्वजनिक:
  DEFINE_SIZE_STATIC (2 + Types::size);

  unsigned get_class () ध्रुव { पटिनिवत्त (unsigned) klass; }
  तार्किक sanitize (hb_sanitize_context_t *c, ध्रुव MarkArray<Types> *base) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) && markAnchor.sanitize (c, base));
  }

  तार्किक subset (hb_subset_context_t    *c,
	       ध्रुव MarkArray<Types> *src_base,
	       ध्रुव hb_map_t         *klass_mapping) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    auto *out = c->serializer->embed (एतत्);
    सचे (unlikely (!out)) return_trace (मिच्छा);

    out->klass = klass_mapping->get (klass);
    return_trace (out->markAnchor.serialize_subset (c, markAnchor, src_base));
  }

  शून्य collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  ध्रुव MarkArray<Types> *src_base) ध्रुव
  {
    (src_base+markAnchor).collect_variation_indices (c);
  }
};


}
}
}

#endif /* OT_LAYOUT_GPOS_MARKRECORD_HH */
