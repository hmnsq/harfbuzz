#که_نه_تعریف_شوی OT_LAYOUT_GPOS_MARKRECORD_HH
#تعریف_کړه OT_LAYOUT_GPOS_MARKRECORD_HH

نطاق OT {
نطاق Layout {
نطاق GPOS_impl {

کينډۍ <ډول_نوم Types> بنية MarkArray;

کينډۍ <ډول_نوم Types>
بنية Mark_ریکارډ
{
  ملګری بنية MarkArray<Types>;

  عام:
  HBUINT16      klass;                  /* Class defined for this mark */
  ډول_نوم Types::کينډۍ دننه_واټن_منزل<Anchor, MarkArray<Types>>
                markAnchor;             /* Offset to Anchor table--from
                                         * beginning of MarkArray table */
  عام:
  DEFINE_اندازه_STATIC (2 + Types::اندازه);

  بینښه get_class () ثابت { أرجع (بینښه) klass; }
  منطقي sanitize (hb_sanitize_context_t *c, ثابت MarkArray<Types> *بنسټ) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && markAnchor.sanitize (c, بنسټ));
  }

  منطقي subset (hb_subset_context_t    *c,
	       ثابت MarkArray<Types> *src_بنسټ,
	       ثابت hb_map_t         *klass_mapping) ثابت
  {
    TRACE_SUBSET (هذا);
    خپلکاری *بهر = c->serializer->embed (هذا);
    إذا (unlikely (!بهر)) return_trace (خطأ);

    بهر->klass = klass_mapping->get (klass);
    return_trace (بهر->markAnchor.serialize_subset (c, markAnchor, src_بنسټ));
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  ثابت MarkArray<Types> *src_بنسټ) ثابت
  {
    (src_بنسټ+markAnchor).collect_variation_indices (c);
  }
};


}
}
}

#د_که_پای /* OT_LAYOUT_GPOS_MARKRECORD_HH */
