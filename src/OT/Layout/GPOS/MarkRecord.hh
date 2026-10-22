#اگر_غیر_تعریف_شدہ OT_LAYOUT_GPOS_MARKRECORD_HH
#تعریف_کرو OT_LAYOUT_GPOS_MARKRECORD_HH

نامخلا OT {
نامخلا Layout {
نامخلا GPOS_impl {

سانچہ <قسم_نام Types> ساخت MarkArray;

سانچہ <قسم_نام Types>
ساخت Mark_ریکارڈ
{
  دوست ساخت MarkArray<Types>;

  عوامی:
  HBUINT16      klass;                  /* Class defined for this mark */
  قسم_نام Types::سانچہ انحراف_مقصد<Anchor, MarkArray<Types>>
                markAnchor;             /* Offset to Anchor table--from
                                         * beginning of MarkArray table */
  عوامی:
  DEFINE_سائز_STATIC (2 + Types::سائز);

  غیرعلامتی get_class () ثابت { واپس (غیرعلامتی) klass; }
  منطقی sanitize (hb_sanitize_context_t *c, ثابت MarkArray<Types> *بنیاد) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ) && markAnchor.sanitize (c, بنیاد));
  }

  منطقی subset (hb_subset_context_t    *c,
	       ثابت MarkArray<Types> *src_بنیاد,
	       ثابت hb_map_t         *klass_mapping) ثابت
  {
    TRACE_SUBSET (یہ);
    خودکار *برآمد = c->serializer->embed (یہ);
    اگر (unlikely (!برآمد)) return_trace (جھوٹ);

    برآمد->klass = klass_mapping->get (klass);
    return_trace (برآمد->markAnchor.serialize_subset (c, markAnchor, src_بنیاد));
  }

  خالی collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  ثابت MarkArray<Types> *src_بنیاد) ثابت
  {
    (src_بنیاد+markAnchor).collect_variation_indices (c);
  }
};


}
}
}

#اگر_ختم /* OT_LAYOUT_GPOS_MARKRECORD_HH */
