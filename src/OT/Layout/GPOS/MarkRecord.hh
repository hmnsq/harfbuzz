#ifndef OT_LAYOUT_GPOS_MARKRECORD_HH
#define OT_LAYOUT_GPOS_MARKRECORD_HH

नामाकाश OT {
नामाकाश Layout {
नामाकाश GPOS_impl {

ढाँचा <typename Types> संरचना MarkArray;

ढाँचा <typename Types>
संरचना MarkRecord
{
  मित्र संरचना MarkArray<Types>;

  सार्वजनिक:
  HBUINT16      klass;                  /* Class defined for this mark */
  typename Types::ढाँचा OffsetTo<Anchor, MarkArray<Types>>
                markAnchor;             /* Offset to Anchor table--from
                                         * beginning of MarkArray table */
  सार्वजनिक:
  DEFINE_SIZE_STATIC (2 + Types::size);

  अचिन्हित get_class () अचर { फर्काऊ (अचिन्हित) klass; }
  तार्किक sanitize (hb_sanitize_context_t *c, अचर MarkArray<Types> *base) अचर
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) && markAnchor.sanitize (c, base));
  }

  तार्किक subset (hb_subset_context_t    *c,
	       अचर MarkArray<Types> *src_base,
	       अचर hb_map_t         *klass_mapping) अचर
  {
    TRACE_SUBSET (एतत्);
    स्वतः *out = c->serializer->embed (एतत्);
    यदि (unlikely (!out)) return_trace (असत्य);

    out->klass = klass_mapping->get (klass);
    return_trace (out->markAnchor.serialize_subset (c, markAnchor, src_base));
  }

  शून्य collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  अचर MarkArray<Types> *src_base) अचर
  {
    (src_base+markAnchor).collect_variation_indices (c);
  }
};


}
}
}

#endif /* OT_LAYOUT_GPOS_MARKRECORD_HH */
