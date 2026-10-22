#ifndef OT_LAYOUT_GPOS_MARKRECORD_HH
#define OT_LAYOUT_GPOS_MARKRECORD_HH

ନାମାକାଶ OT {
ନାମାକାଶ Layout {
ନାମାକାଶ GPOS_impl {

template <typename Types> ସଂରଚନା MarkArray;

template <typename Types>
ସଂରଚନା MarkRecord
{
  friend ସଂରଚନା MarkArray<Types>;

  ସର୍ବସାଧାରଣ:
  HBUINT16      klass;                  /* Class defined for this mark */
  typename Types::template OffsetTo<Anchor, MarkArray<Types>>
                markAnchor;             /* Offset to Anchor table--from
                                         * beginning of MarkArray table */
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (2 + Types::size);

  unsigned get_class () ଅଚଳ { ଫେରନ୍ତୁ (unsigned) klass; }
  ତାର୍କିକ sanitize (hb_sanitize_context_t *c, ଅଚଳ MarkArray<Types> *base) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି) && markAnchor.sanitize (c, base));
  }

  ତାର୍କିକ subset (hb_subset_context_t    *c,
	       ଅଚଳ MarkArray<Types> *src_base,
	       ଅଚଳ hb_map_t         *klass_mapping) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    auto *out = c->serializer->embed (ଏହି);
    ଯଦି (unlikely (!out)) return_trace (ମିଥ୍ୟା);

    out->klass = klass_mapping->get (klass);
    return_trace (out->markAnchor.serialize_subset (c, markAnchor, src_base));
  }

  ଶୂନ୍ୟ collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  ଅଚଳ MarkArray<Types> *src_base) ଅଚଳ
  {
    (src_base+markAnchor).collect_variation_indices (c);
  }
};


}
}
}

#endif /* OT_LAYOUT_GPOS_MARKRECORD_HH */
