#اگر_تعریف_نشده OT_LAYOUT_GPOS_LIGATUREARRAY_HH
#تعریف_کن OT_LAYOUT_GPOS_LIGATUREARRAY_HH

نطاق OT {
نطاق Layout {
نطاق GPOS_impl {

HB_داخلی مضمن
فراغ collect_ligature_variation_indices (hb_collect_variation_indices_context_t *c,
                                        ثابت Coverage &coverage,
                                        ثابت فراغ *پایه,
                                        glyph_داده_t offsets,
                                        بیعلامت شمار,
                                        بیعلامت class_شمار,
                                        ثابت hb_map_t &klass_mapping)
{
  لكل (ثابت خودکار _ : hb_zip (coverage, hb_range (شمار)))
  {
    إذا (!c->glyph_مجموعه->has (_.اولی)) استمر;
    hb_barrier ();
    بیعلامت جابه_جایی = offsets[_.second];
    إذا (!جابه_جایی) استمر;

    ثابت فراغ *ماتریس = &Struct_At_جابه_جایی<ثابت HBUINT16> (پایه, جابه_جایی);
    /* LigatureAttach always has a 16-bit component count; its anchor
     * offsets have the same width as the LigatureArray offsets. */
    بیعلامت ردیفها = *static_cast<ثابت HBUINT16 *> (ماتریس);
    glyph_داده_t anchors = offsets;
    anchors.داده = (ثابت حرف *) ماتریس + HBUINT16::static_اندازه;

    hb_sorted_وکتور_t<بیعلامت> indexes;
    collect_ligature_anchor_indexes (ردیفها, class_شمار, klass_mapping, indexes);
    collect_anchor_variation_indices (c, ماتریس, anchors, indexes.iter ());
  }
}


/* Array of LigatureAttach tables ordered by LigatureCoverage Index */
قالب <نام_نوع Types>
بنية LigatureArray :
  Types::قالب Array_از<نام_نوع Types::قالب جابه_جایی_مقصد<Anchor_ماتریس<Types, HBUINT16>>>
{
  تعريفنوع Anchor_ماتریس<Types, HBUINT16> LigatureAttach;
  تعريفنوع نام_نوع Types::قالب جابه_جایی_مقصد<LigatureAttach> Ligature_Attach_جابه_جایی;
  تعريفنوع نام_نوع Types::قالب Array_از<Ligature_Attach_جابه_جایی> Super;

  ثابت LigatureAttach& عملگر [] (صحيح i_) ثابت
  {
    بیعلامت صحيح i = (بیعلامت صحيح) i_;
    إذا (unlikely (i >= هذا->len)) أرجع Null (LigatureAttach);
    hb_barrier ();
    أرجع هذا+هذا->arrayZ[i];
  }

  قالب <نام_نوع ...Ts>
  منطقي sanitize (hb_sanitize_context_t *c, Ts&&... ds) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (Super::sanitize (c, هذا, std::forward<Ts> (ds)...));
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  ثابت Coverage &coverage,
                                  بیعلامت class_شمار,
                                  ثابت hb_map_t &klass_mapping) ثابت
  {
    collect_ligature_variation_indices (c, coverage, هذا, هذا->arrayZ,
                                        هذا->len, class_شمار, klass_mapping);
  }

  قالب <نام_نوع Iterator,
            hb_requires (hb_is_iterator (Iterator))>
  منطقي subset (hb_subset_context_t *c,
               Iterator             coverage,
               بیعلامت             class_شمار,
               ثابت hb_map_t      *klass_mapping,
               hb_sorted_وکتور_t<hb_codepoint_t> &new_coverage /*بیرون*/) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_map_t &glyph_map = c->plan->glyph_map_gsub;

    خودکار *بیرون = c->serializer->شروع_embed (هذا);
    إذا (unlikely (!c->serializer->extend_min (بیرون)))  return_trace (خطأ);

    منطقي ret = خطأ;
    لكل (ثابت خودکار _ : + hb_zip (coverage, *هذا)
                        | hb_filter (glyph_map, hb_اولی))
    {
      ثابت LigatureAttach& src = (هذا + _.second);
      منطقي non_خالی = + hb_range (src.ردیفها * class_شمار)
                       | hb_filter ([=] (بیعلامت شاخص_2) { أرجع klass_mapping->has (شاخص_2 % class_شمار); })
                       | hb_map ([&] (ثابت بیعلامت شاخص_2) { أرجع !src.جابه_جایی_is_null (شاخص_2 / class_شمار, شاخص_2 % class_شمار, class_شمار); })
                       | hb_any;

      إذا (!non_خالی) استمر;

      خودکار *ماتریس = بیرون->serialize_append (c->serializer);
      إذا (unlikely (!ماتریس)) return_trace (خطأ);

      خودکار indexes =
          + hb_range (src.ردیفها * class_شمار)
          | hb_filter ([=] (بیعلامت شاخص_2) { أرجع klass_mapping->has (شاخص_2 % class_شمار); })
          ;
      ret |= ماتریس->serialize_subset (c,
				       _.second,
				       هذا,
				       src.ردیفها,
				       indexes);

      hb_codepoint_t new_gid = glyph_map.get (_.اولی);
      new_coverage.push (new_gid);
    }
    return_trace (ret);
  }
};


}
}
}

#پایان_اگر /* OT_LAYOUT_GPOS_LIGATUREARRAY_HH */
