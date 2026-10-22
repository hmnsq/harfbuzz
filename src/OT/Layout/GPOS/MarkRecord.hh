#ifndef OT_LAYOUT_GPOS_MARKRECORD_HH
#define OT_LAYOUT_GPOS_MARKRECORD_HH

நாமாகாஶ OT {
நாமாகாஶ Layout {
நாமாகாஶ GPOS_impl {

template <typename Types> கட்டமைப்பு MarkArray;

template <typename Types>
கட்டமைப்பு MarkRecord
{
  friend கட்டமைப்பு MarkArray<Types>;

  பொது:
  HBUINT16      klass;                  /* Class defined for this mark */
  typename Types::template OffsetTo<Anchor, MarkArray<Types>>
                markAnchor;             /* Offset to Anchor table--from
                                         * beginning of MarkArray table */
  பொது:
  DEFINE_SIZE_STATIC (2 + Types::size);

  unsigned get_class () மாறிலி { திரும்பு (unsigned) klass; }
  தருக்க sanitize (hb_sanitize_context_t *c, மாறிலி MarkArray<Types> *base) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது) && markAnchor.sanitize (c, base));
  }

  தருக்க subset (hb_subset_context_t    *c,
	       மாறிலி MarkArray<Types> *src_base,
	       மாறிலி hb_map_t         *klass_mapping) மாறிலி
  {
    TRACE_SUBSET (இது);
    auto *out = c->serializer->embed (இது);
    எனில் (unlikely (!out)) return_trace (பொய்);

    out->klass = klass_mapping->get (klass);
    return_trace (out->markAnchor.serialize_subset (c, markAnchor, src_base));
  }

  வெறுமை collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  மாறிலி MarkArray<Types> *src_base) மாறிலி
  {
    (src_base+markAnchor).collect_variation_indices (c);
  }
};


}
}
}

#endif /* OT_LAYOUT_GPOS_MARKRECORD_HH */
