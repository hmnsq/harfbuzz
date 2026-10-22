#جيڪڏهن_وصف_نه_ٿيل OT_LAYOUT_GPOS_MARKRECORD_HH
#وصف_ڪريو OT_LAYOUT_GPOS_MARKRECORD_HH

نطاق OT {
نطاق Layout {
نطاق GPOS_impl {

سانچو <قسم_نالو Types> بنية MarkArray;

سانچو <قسم_نالو Types>
بنية Mark_رڪارڊ
{
  دوست بنية MarkArray<Types>;

  عام:
  HBUINT16      klass;                  /* Class defined for this mark */
  قسم_نالو Types::سانچو وٿي_منزل<Anchor, MarkArray<Types>>
                markAnchor;             /* Offset to Anchor table--from
                                         * beginning of MarkArray table */
  عام:
  DEFINE_سائيز_STATIC (2 + Types::سائيز);

  بنانشان get_class () ثابت { أرجع (بنانشان) klass; }
  منطقي sanitize (hb_sanitize_context_t *c, ثابت MarkArray<Types> *بنياد) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && markAnchor.sanitize (c, بنياد));
  }

  منطقي subset (hb_subset_context_t    *c,
	       ثابت MarkArray<Types> *src_بنياد,
	       ثابت hb_map_t         *klass_mapping) ثابت
  {
    TRACE_SUBSET (هذا);
    خودڪار *ٻاهريون = c->serializer->embed (هذا);
    إذا (unlikely (!ٻاهريون)) return_trace (خطأ);

    ٻاهريون->klass = klass_mapping->get (klass);
    return_trace (ٻاهريون->markAnchor.serialize_subset (c, markAnchor, src_بنياد));
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  ثابت MarkArray<Types> *src_بنياد) ثابت
  {
    (src_بنياد+markAnchor).collect_variation_indices (c);
  }
};


}
}
}

#جيڪڏهن_پورو /* OT_LAYOUT_GPOS_MARKRECORD_HH */
