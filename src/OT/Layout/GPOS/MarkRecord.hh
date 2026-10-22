#ifndef OT_LAYOUT_GPOS_MARKRECORD_HH
#define OT_LAYOUT_GPOS_MARKRECORD_HH

নামাকাশ OT {
নামাকাশ Layout {
নামাকাশ GPOS_impl {

template <typename Types> সংরচনা MarkArray;

template <typename Types>
সংরচনা MarkRecord
{
  friend সংরচনা MarkArray<Types>;

  সার্ৱজনিক:
  HBUINT16      klass;                  /* Class defined for this mark */
  typename Types::template OffsetTo<Anchor, MarkArray<Types>>
                markAnchor;             /* Offset to Anchor table--from
                                         * beginning of MarkArray table */
  সার্ৱজনিক:
  DEFINE_SIZE_STATIC (2 + Types::size);

  unsigned get_class () ধ্রুৱ { ঘূৰাই (unsigned) klass; }
  তার্কিক sanitize (hb_sanitize_context_t *c, ধ্রুৱ MarkArray<Types> *base) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্) && markAnchor.sanitize (c, base));
  }

  তার্কিক subset (hb_subset_context_t    *c,
	       ধ্রুৱ MarkArray<Types> *src_base,
	       ধ্রুৱ hb_map_t         *klass_mapping) ধ্রুৱ
  {
    TRACE_SUBSET (এতত্);
    auto *out = c->serializer->embed (এতত্);
    যদি (unlikely (!out)) return_trace (মিছা);

    out->klass = klass_mapping->get (klass);
    return_trace (out->markAnchor.serialize_subset (c, markAnchor, src_base));
  }

  শূন্য collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  ধ্রুৱ MarkArray<Types> *src_base) ধ্রুৱ
  {
    (src_base+markAnchor).collect_variation_indices (c);
  }
};


}
}
}

#endif /* OT_LAYOUT_GPOS_MARKRECORD_HH */
