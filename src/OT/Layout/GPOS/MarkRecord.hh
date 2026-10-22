#ifndef OT_LAYOUT_GPOS_MARKRECORD_HH
#define OT_LAYOUT_GPOS_MARKRECORD_HH

নামাকাশ OT {
নামাকাশ Layout {
নামাকাশ GPOS_impl {

template <typename Types> কাঠামো MarkArray;

template <typename Types>
কাঠামো MarkRecord
{
  friend কাঠামো MarkArray<Types>;

  সর্বজনীন:
  HBUINT16      klass;                  /* Class defined for this mark */
  typename Types::template OffsetTo<Anchor, MarkArray<Types>>
                markAnchor;             /* Offset to Anchor table--from
                                         * beginning of MarkArray table */
  সর্বজনীন:
  DEFINE_আকার_STATIC (2 + Types::আকার_2);

  unsigned get_class () ধ্রুবক { ফেরত (unsigned) klass; }
  যৌক্তিক sanitize (hb_sanitize_context_t *c, ধ্রুবক MarkArray<Types> *base) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই) && markAnchor.sanitize (c, base));
  }

  যৌক্তিক subset (hb_subset_context_t    *c,
	       ধ্রুবক MarkArray<Types> *src_base,
	       ধ্রুবক hb_map_t         *klass_mapping) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    auto *out = c->serializer->embed (এই);
    যদি (unlikely (!out)) return_trace (মিথ্যা);

    out->klass = klass_mapping->get (klass);
    return_trace (out->markAnchor.serialize_subset (c, markAnchor, src_base));
  }

  শূন্য collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  ধ্রুবক MarkArray<Types> *src_base) ধ্রুবক
  {
    (src_base+markAnchor).collect_variation_indices (c);
  }
};


}
}
}

#endif /* OT_LAYOUT_GPOS_MARKRECORD_HH */
