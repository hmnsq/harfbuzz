#اگر_تعریف_نشده OT_LAYOUT_GPOS_MARKRECORD_HH
#تعریف_کن OT_LAYOUT_GPOS_MARKRECORD_HH

نطاق OT {
نطاق Layout {
نطاق GPOS_impl {

قالب <نام_نوع Types> بنية MarkArray;

قالب <نام_نوع Types>
بنية Mark_رکورد
{
  دوست بنية MarkArray<Types>;

  عام:
  HBUINT16      klass;                  /* Class defined for this mark */
  نام_نوع Types::قالب جابه_جایی_مقصد<Anchor, MarkArray<Types>>
                markAnchor;             /* Offset to Anchor table--from
                                         * beginning of MarkArray table */
  عام:
  DEFINE_اندازه_STATIC (2 + Types::اندازه);

  بیعلامت get_class () ثابت { أرجع (بیعلامت) klass; }
  منطقي sanitize (hb_sanitize_context_t *c, ثابت MarkArray<Types> *پایه) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && markAnchor.sanitize (c, پایه));
  }

  منطقي subset (hb_subset_context_t    *c,
	       ثابت MarkArray<Types> *src_پایه,
	       ثابت hb_map_t         *klass_mapping) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *بیرون = c->serializer->embed (هذا);
    إذا (unlikely (!بیرون)) return_trace (خطأ);

    بیرون->klass = klass_mapping->get (klass);
    return_trace (بیرون->markAnchor.serialize_subset (c, markAnchor, src_پایه));
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  ثابت MarkArray<Types> *src_پایه) ثابت
  {
    (src_پایه+markAnchor).collect_variation_indices (c);
  }
};


}
}
}

#پایان_اگر /* OT_LAYOUT_GPOS_MARKRECORD_HH */
