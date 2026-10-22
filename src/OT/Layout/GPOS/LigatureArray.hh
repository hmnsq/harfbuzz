#که_نه_تعریف_شوی OT_LAYOUT_GPOS_LIGATUREARRAY_HH
#تعریف_کړه OT_LAYOUT_GPOS_LIGATUREARRAY_HH

نطاق OT {
نطاق Layout {
نطاق GPOS_impl {

HB_دننی مضمن
فراغ collect_ligature_variation_indices (hb_collect_variation_indices_context_t *c,
                                        ثابت Coverage &coverage,
                                        ثابت فراغ *بنسټ,
                                        glyph_معلومات_t offsets,
                                        بینښه شمېر,
                                        بینښه class_شمېر,
                                        ثابت hb_map_t &klass_mapping)
{
  لكل (ثابت خپلکاری _ : hb_zip (coverage, hb_range (شمېر)))
  {
    إذا (!c->glyph_سېټ->has (_.لومړی)) استمر;
    hb_barrier ();
    بینښه دننه_واټن = offsets[_.second];
    إذا (!دننه_واټن) استمر;

    ثابت فراغ *ماتریکس = &Struct_At_دننه_واټن<ثابت HBUINT16> (بنسټ, دننه_واټن);
    /* LigatureAttach always has a 16-bit component count; its anchor
     * offsets have the same width as the LigatureArray offsets. */
    بینښه لیکې = *static_cast<ثابت HBUINT16 *> (ماتریکس);
    glyph_معلومات_t anchors = offsets;
    anchors.معلومات = (ثابت حرف *) ماتریکس + HBUINT16::static_اندازه;

    hb_sorted_ویکټور_t<بینښه> indexes;
    collect_ligature_anchor_indexes (لیکې, class_شمېر, klass_mapping, indexes);
    collect_anchor_variation_indices (c, ماتریکس, anchors, indexes.iter ());
  }
}


/* Array of LigatureAttach tables ordered by LigatureCoverage Index */
کينډۍ <ډول_نوم Types>
بنية LigatureArray :
  Types::کينډۍ Array_د<ډول_نوم Types::کينډۍ دننه_واټن_منزل<Anchor_ماتریکس<Types, HBUINT16>>>
{
  تعريفنوع Anchor_ماتریکس<Types, HBUINT16> LigatureAttach;
  تعريفنوع ډول_نوم Types::کينډۍ دننه_واټن_منزل<LigatureAttach> Ligature_Attach_دننه_واټن;
  تعريفنوع ډول_نوم Types::کينډۍ Array_د<Ligature_Attach_دننه_واټن> Super;

  ثابت LigatureAttach& چلوونکی [] (صحيح i_) ثابت
  {
    بینښه صحيح i = (بینښه صحيح) i_;
    إذا (unlikely (i >= هذا->len)) أرجع Null (LigatureAttach);
    hb_barrier ();
    أرجع هذا+هذا->arrayZ[i];
  }

  کينډۍ <ډول_نوم ...Ts>
  منطقي sanitize (hb_sanitize_context_t *c, Ts&&... ds) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (Super::sanitize (c, هذا, std::forward<Ts> (ds)...));
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  ثابت Coverage &coverage,
                                  بینښه class_شمېر,
                                  ثابت hb_map_t &klass_mapping) ثابت
  {
    collect_ligature_variation_indices (c, coverage, هذا, هذا->arrayZ,
                                        هذا->len, class_شمېر, klass_mapping);
  }

  کينډۍ <ډول_نوم Iterator,
            hb_requires (hb_is_iterator (Iterator))>
  منطقي subset (hb_subset_context_t *c,
               Iterator             coverage,
               بینښه             class_شمېر,
               ثابت hb_map_t      *klass_mapping,
               hb_sorted_ویکټور_t<hb_codepoint_t> &new_coverage /*بهر*/) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_map_t &glyph_map = c->plan->glyph_map_gsub;

    خپلکاری *بهر = c->serializer->پیل_embed (هذا);
    إذا (unlikely (!c->serializer->extend_min (بهر)))  return_trace (خطأ);

    منطقي ret = خطأ;
    لكل (ثابت خپلکاری _ : + hb_zip (coverage, *هذا)
                        | hb_filter (glyph_map, hb_لومړی))
    {
      ثابت LigatureAttach& src = (هذا + _.second);
      منطقي non_تشه = + hb_range (src.لیکې * class_شمېر)
                       | hb_filter ([=] (بینښه شاخص_2) { أرجع klass_mapping->has (شاخص_2 % class_شمېر); })
                       | hb_map ([&] (ثابت بینښه شاخص_2) { أرجع !src.دننه_واټن_is_null (شاخص_2 / class_شمېر, شاخص_2 % class_شمېر, class_شمېر); })
                       | hb_any;

      إذا (!non_تشه) استمر;

      خپلکاری *ماتریکس = بهر->serialize_append (c->serializer);
      إذا (unlikely (!ماتریکس)) return_trace (خطأ);

      خپلکاری indexes =
          + hb_range (src.لیکې * class_شمېر)
          | hb_filter ([=] (بینښه شاخص_2) { أرجع klass_mapping->has (شاخص_2 % class_شمېر); })
          ;
      ret |= ماتریکس->serialize_subset (c,
				       _.second,
				       هذا,
				       src.لیکې,
				       indexes);

      hb_codepoint_t new_gid = glyph_map.get (_.لومړی);
      new_coverage.push (new_gid);
    }
    return_trace (ret);
  }
};


}
}
}

#د_که_پای /* OT_LAYOUT_GPOS_LIGATUREARRAY_HH */
