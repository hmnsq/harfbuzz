#ifndef OT_LAYOUT_GPOS_MARKRECORD_HH
#define OT_LAYOUT_GPOS_MARKRECORD_HH

નામાકાશ OT {
નામાકાશ Layout {
નામાકાશ GPOS_impl {

template <typename Types> સંરચના MarkArray;

template <typename Types>
સંરચના MarkRecord
{
  friend સંરચના MarkArray<Types>;

  જાહેર:
  HBUINT16      klass;                  /* Class defined for this mark */
  typename Types::template OffsetTo<Anchor, MarkArray<Types>>
                markAnchor;             /* Offset to Anchor table--from
                                         * beginning of MarkArray table */
  જાહેર:
  DEFINE_SIZE_STATIC (2 + Types::size);

  unsigned get_class () અચલ { પાછા (unsigned) klass; }
  તાર્કિક sanitize (hb_sanitize_context_t *c, અચલ MarkArray<Types> *base) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ) && markAnchor.sanitize (c, base));
  }

  તાર્કિક subset (hb_subset_context_t    *c,
	       અચલ MarkArray<Types> *src_base,
	       અચલ hb_map_t         *klass_mapping) અચલ
  {
    TRACE_SUBSET (આ);
    auto *out = c->serializer->embed (આ);
    જો (unlikely (!out)) return_trace (ખોટું);

    out->klass = klass_mapping->get (klass);
    return_trace (out->markAnchor.serialize_subset (c, markAnchor, src_base));
  }

  શૂન્ય collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  અચલ MarkArray<Types> *src_base) અચલ
  {
    (src_base+markAnchor).collect_variation_indices (c);
  }
};


}
}
}

#endif /* OT_LAYOUT_GPOS_MARKRECORD_HH */
