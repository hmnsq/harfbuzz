#ifndef OT_LAYOUT_GPOS_MARKRECORD_HH
#define OT_LAYOUT_GPOS_MARKRECORD_HH

فضاینام OT {
فضاینام Layout {
فضاینام GPOS_impl {

template <typename Types> ساختار MarkArray;

template <typename Types>
ساختار MarkRecord
{
  friend ساختار MarkArray<Types>;

  عمومی:
  HBUINT16      klass;                  /* Class defined for this mark */
  typename Types::template OffsetTo<Anchor, MarkArray<Types>>
                markAnchor;             /* Offset to Anchor table--from
                                         * beginning of MarkArray table */
  عمومی:
  DEFINE_SIZE_STATIC (2 + Types::size);

  unsigned get_class () ثابت { بازگشت (unsigned) klass; }
  منطقی sanitize (hb_sanitize_context_t *c, ثابت MarkArray<Types> *base) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این) && markAnchor.sanitize (c, base));
  }

  منطقی subset (hb_subset_context_t    *c,
	       ثابت MarkArray<Types> *src_base,
	       ثابت hb_map_t         *klass_mapping) ثابت
  {
    TRACE_SUBSET (این);
    auto *out = c->serializer->embed (این);
    اگر (unlikely (!out)) return_trace (نادرست);

    out->klass = klass_mapping->get (klass);
    return_trace (out->markAnchor.serialize_subset (c, markAnchor, src_base));
  }

  تهی collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  ثابت MarkArray<Types> *src_base) ثابت
  {
    (src_base+markAnchor).collect_variation_indices (c);
  }
};


}
}
}

#endif /* OT_LAYOUT_GPOS_MARKRECORD_HH */
