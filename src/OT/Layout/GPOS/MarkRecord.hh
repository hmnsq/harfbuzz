#ifndef OT_LAYOUT_GPOS_MARKRECORD_HH
#define OT_LAYOUT_GPOS_MARKRECORD_HH

نطاق OT {
نطاق Layout {
نطاق GPOS_impl {

template <typename Types> بنية MarkArray;

template <typename Types>
بنية MarkRecord
{
  friend بنية MarkArray<Types>;

  عام:
  HBUINT16      klass;                  /* Class defined for this mark */
  typename Types::template OffsetTo<Anchor, MarkArray<Types>>
                markAnchor;             /* Offset to Anchor table--from
                                         * beginning of MarkArray table */
  عام:
  DEFINE_SIZE_STATIC (2 + Types::size);

  unsigned get_class () ثابت { החזר (unsigned) klass; }
  منطقي sanitize (hb_sanitize_context_t *c, ثابت MarkArray<Types> *base) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && markAnchor.sanitize (c, base));
  }

  منطقي subset (hb_subset_context_t    *c,
	       ثابت MarkArray<Types> *src_base,
	       ثابت hb_map_t         *klass_mapping) ثابت
  {
    TRACE_SUBSET (هذا);
    auto *out = c->serializer->embed (هذا);
    אם (unlikely (!out)) return_trace (שקר);

    out->klass = klass_mapping->get (klass);
    return_trace (out->markAnchor.serialize_subset (c, markAnchor, src_base));
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  ثابت MarkArray<Types> *src_base) ثابت
  {
    (src_base+markAnchor).collect_variation_indices (c);
  }
};


}
}
}

#endif /* OT_LAYOUT_GPOS_MARKRECORD_HH */
