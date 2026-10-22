#ifndef OT_LAYOUT_GPOS_MARKRECORD_HH
#define OT_LAYOUT_GPOS_MARKRECORD_HH

ನಾಮಾಕಾಶ OT {
ನಾಮಾಕಾಶ Layout {
ನಾಮಾಕಾಶ GPOS_impl {

template <typename Types> ಸಂರಚನಾ MarkArray;

template <typename Types>
ಸಂರಚನಾ MarkRecord
{
  friend ಸಂರಚನಾ MarkArray<Types>;

  ಸಾರ್ವಜನಿಕ:
  HBUINT16      klass;                  /* Class defined for this mark */
  typename Types::template OffsetTo<Anchor, MarkArray<Types>>
                markAnchor;             /* Offset to Anchor table--from
                                         * beginning of MarkArray table */
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (2 + Types::size);

  unsigned get_class () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ (unsigned) klass; }
  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c, ಧ್ರುವ MarkArray<Types> *base) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್) && markAnchor.sanitize (c, base));
  }

  ತಾರ್ಕಿಕ subset (hb_subset_context_t    *c,
	       ಧ್ರುವ MarkArray<Types> *src_base,
	       ಧ್ರುವ hb_map_t         *klass_mapping) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    auto *out = c->serializer->embed (ಏತತ್);
    ಯದಿ (unlikely (!out)) return_trace (ಅಸತ್ಯ);

    out->klass = klass_mapping->get (klass);
    return_trace (out->markAnchor.serialize_subset (c, markAnchor, src_base));
  }

  ಶೂನ್ಯ collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  ಧ್ರುವ MarkArray<Types> *src_base) ಧ್ರುವ
  {
    (src_base+markAnchor).collect_variation_indices (c);
  }
};


}
}
}

#endif /* OT_LAYOUT_GPOS_MARKRECORD_HH */
