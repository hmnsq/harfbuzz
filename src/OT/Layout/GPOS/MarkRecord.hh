#ifndef OT_LAYOUT_GPOS_MARKRECORD_HH
#define OT_LAYOUT_GPOS_MARKRECORD_HH

നാമാകാശ OT {
നാമാകാശ Layout {
നാമാകാശ GPOS_impl {

template <typename Types> ഘടന MarkArray;

template <typename Types>
ഘടന MarkRecord
{
  friend ഘടന MarkArray<Types>;

  പൊതു:
  HBUINT16      klass;                  /* Class defined for this mark */
  typename Types::template OffsetTo<Anchor, MarkArray<Types>>
                markAnchor;             /* Offset to Anchor table--from
                                         * beginning of MarkArray table */
  പൊതു:
  DEFINE_SIZE_STATIC (2 + Types::size);

  unsigned get_class () സ്ഥിരം { തിരികെ (unsigned) klass; }
  തർക്കം sanitize (hb_sanitize_context_t *c, സ്ഥിരം MarkArray<Types> *base) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_struct (ഇത്) && markAnchor.sanitize (c, base));
  }

  തർക്കം subset (hb_subset_context_t    *c,
	       സ്ഥിരം MarkArray<Types> *src_base,
	       സ്ഥിരം hb_map_t         *klass_mapping) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);
    auto *out = c->serializer->embed (ഇത്);
    എങ്കിൽ (unlikely (!out)) return_trace (അസത്യം);

    out->klass = klass_mapping->get (klass);
    return_trace (out->markAnchor.serialize_subset (c, markAnchor, src_base));
  }

  ശൂന്യം collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  സ്ഥിരം MarkArray<Types> *src_base) സ്ഥിരം
  {
    (src_base+markAnchor).collect_variation_indices (c);
  }
};


}
}
}

#endif /* OT_LAYOUT_GPOS_MARKRECORD_HH */
