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

  সার্বজনিক:
  HBUINT16      klass;                  /* Class defined for this mark */
  typename Types::template OffsetTo<Anchor, MarkArray<Types>>
                markAnchor;             /* Offset to Anchor table--from
                                         * beginning of MarkArray table */
  সার্বজনিক:
  DEFINE_SIZE_STATIC (2 + Types::size);

  unsigned get_class () ধ্রুব { প্রত্যাগম (unsigned) klass; }
  তার্কিক sanitize (hb_sanitize_context_t *c, ধ্রুব MarkArray<Types> *base) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্) && markAnchor.sanitize (c, base));
  }

  তার্কিক subset (hb_subset_context_t    *c,
	       ধ্রুব MarkArray<Types> *src_base,
	       ধ্রুব hb_map_t         *klass_mapping) ধ্রুব
  {
    TRACE_SUBSET (এতত্);
    auto *out = c->serializer->embed (এতত্);
    যদি (unlikely (!out)) return_trace (অসত্য);

    out->klass = klass_mapping->get (klass);
    return_trace (out->markAnchor.serialize_subset (c, markAnchor, src_base));
  }

  শূন্য collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  ধ্রুব MarkArray<Types> *src_base) ধ্রুব
  {
    (src_base+markAnchor).collect_variation_indices (c);
  }
};


}
}
}

#endif /* OT_LAYOUT_GPOS_MARKRECORD_HH */
