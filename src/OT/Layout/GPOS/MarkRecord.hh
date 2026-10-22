#جے_بنا_تعریف OT_LAYOUT_GPOS_MARKRECORD_HH
#تعریف_کرو OT_LAYOUT_GPOS_MARKRECORD_HH

نطاق OT {
نطاق Layout {
نطاق GPOS_impl {

سانچا <قسم_ناں Types> بنية MarkArray;

سانچا <قسم_ناں Types>
بنية Mark_ریکارڈ
{
  یار بنية MarkArray<Types>;

  عام:
  HBUINT16      klass;                  /* Class defined for this mark */
  قسم_ناں Types::سانچا وتھ_منزل<Anchor, MarkArray<Types>>
                markAnchor;             /* Offset to Anchor table--from
                                         * beginning of MarkArray table */
  عام:
  DEFINE_ناپ_STATIC (2 + Types::ناپ);

  بنانشانی get_class () ثابت { أرجع (بنانشانی) klass; }
  منطقي sanitize (hb_sanitize_context_t *c, ثابت MarkArray<Types> *ادھار) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && markAnchor.sanitize (c, ادھار));
  }

  منطقي subset (hb_subset_context_t    *c,
	       ثابت MarkArray<Types> *src_ادھار,
	       ثابت hb_map_t         *klass_mapping) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *باہرلا = c->serializer->embed (هذا);
    إذا (unlikely (!باہرلا)) return_trace (خطأ);

    باہرلا->klass = klass_mapping->get (klass);
    return_trace (باہرلا->markAnchor.serialize_subset (c, markAnchor, src_ادھار));
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  ثابت MarkArray<Types> *src_ادھار) ثابت
  {
    (src_ادھار+markAnchor).collect_variation_indices (c);
  }
};


}
}
}

#جے_انت /* OT_LAYOUT_GPOS_MARKRECORD_HH */
