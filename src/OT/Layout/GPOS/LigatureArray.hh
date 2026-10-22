#جے_بنا_تعریف OT_LAYOUT_GPOS_LIGATUREARRAY_HH
#تعریف_کرو OT_LAYOUT_GPOS_LIGATUREARRAY_HH

نطاق OT {
نطاق Layout {
نطاق GPOS_impl {

HB_اندرونی مضمن
فراغ collect_ligature_variation_indices (hb_collect_variation_indices_context_t *c,
                                        ثابت Coverage &coverage,
                                        ثابت فراغ *ادھار,
                                        glyph_انکڑے_t offsets,
                                        بنانشانی گنتی,
                                        بنانشانی class_گنتی,
                                        ثابت hb_map_t &klass_mapping)
{
  لكل (ثابت خودکار _ : hb_zip (coverage, hb_range (گنتی)))
  {
    إذا (!c->glyph_سیٹ->has (_.پہلا)) استمر;
    hb_barrier ();
    بنانشانی وتھ = offsets[_.second];
    إذا (!وتھ) استمر;

    ثابت فراغ *میٹرکس = &Struct_At_وتھ<ثابت HBUINT16> (ادھار, وتھ);
    /* LigatureAttach always has a 16-bit component count; its anchor
     * offsets have the same width as the LigatureArray offsets. */
    بنانشانی صفاں = *static_cast<ثابت HBUINT16 *> (میٹرکس);
    glyph_انکڑے_t anchors = offsets;
    anchors.انکڑے = (ثابت حرف *) میٹرکس + HBUINT16::static_ناپ;

    hb_sorted_سمتیہ_t<بنانشانی> indexes;
    collect_ligature_anchor_indexes (صفاں, class_گنتی, klass_mapping, indexes);
    collect_anchor_variation_indices (c, میٹرکس, anchors, indexes.iter ());
  }
}


/* Array of LigatureAttach tables ordered by LigatureCoverage Index */
سانچا <قسم_ناں Types>
بنية LigatureArray :
  Types::سانچا Array_دا<قسم_ناں Types::سانچا وتھ_منزل<Anchor_میٹرکس<Types, HBUINT16>>>
{
  تعريفنوع Anchor_میٹرکس<Types, HBUINT16> LigatureAttach;
  تعريفنوع قسم_ناں Types::سانچا وتھ_منزل<LigatureAttach> Ligature_Attach_وتھ;
  تعريفنوع قسم_ناں Types::سانچا Array_دا<Ligature_Attach_وتھ> Super;

  ثابت LigatureAttach& عامل [] (صحيح i_) ثابت
  {
    بنانشانی صحيح i = (بنانشانی صحيح) i_;
    إذا (unlikely (i >= هذا->len)) أرجع Null (LigatureAttach);
    hb_barrier ();
    أرجع هذا+هذا->arrayZ[i];
  }

  سانچا <قسم_ناں ...Ts>
  منطقي sanitize (hb_sanitize_context_t *c, Ts&&... ds) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (Super::sanitize (c, هذا, std::forward<Ts> (ds)...));
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  ثابت Coverage &coverage,
                                  بنانشانی class_گنتی,
                                  ثابت hb_map_t &klass_mapping) ثابت
  {
    collect_ligature_variation_indices (c, coverage, هذا, هذا->arrayZ,
                                        هذا->len, class_گنتی, klass_mapping);
  }

  سانچا <قسم_ناں Iterator,
            hb_requires (hb_is_iterator (Iterator))>
  منطقي subset (hb_subset_context_t *c,
               Iterator             coverage,
               بنانشانی             class_گنتی,
               ثابت hb_map_t      *klass_mapping,
               hb_sorted_سمتیہ_t<hb_codepoint_t> &new_coverage /*باہرلا*/) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_map_t &glyph_map = c->plan->glyph_map_gsub;

    خودکار *باہرلا = c->serializer->شروع_embed (هذا);
    إذا (unlikely (!c->serializer->extend_min (باہرلا)))  return_trace (خطأ);

    منطقي ret = خطأ;
    لكل (ثابت خودکار _ : + hb_zip (coverage, *هذا)
                        | hb_filter (glyph_map, hb_پہلا))
    {
      ثابت LigatureAttach& src = (هذا + _.second);
      منطقي non_خالی_تھاواں = + hb_range (src.صفاں * class_گنتی)
                       | hb_filter ([=] (بنانشانی اشاریہ_2) { أرجع klass_mapping->has (اشاریہ_2 % class_گنتی); })
                       | hb_map ([&] (ثابت بنانشانی اشاریہ_2) { أرجع !src.وتھ_is_null (اشاریہ_2 / class_گنتی, اشاریہ_2 % class_گنتی, class_گنتی); })
                       | hb_any;

      إذا (!non_خالی_تھاواں) استمر;

      خودکار *میٹرکس = باہرلا->serialize_append (c->serializer);
      إذا (unlikely (!میٹرکس)) return_trace (خطأ);

      خودکار indexes =
          + hb_range (src.صفاں * class_گنتی)
          | hb_filter ([=] (بنانشانی اشاریہ_2) { أرجع klass_mapping->has (اشاریہ_2 % class_گنتی); })
          ;
      ret |= میٹرکس->serialize_subset (c,
				       _.second,
				       هذا,
				       src.صفاں,
				       indexes);

      hb_codepoint_t new_gid = glyph_map.get (_.پہلا);
      new_coverage.push (new_gid);
    }
    return_trace (ret);
  }
};


}
}
}

#جے_انت /* OT_LAYOUT_GPOS_LIGATUREARRAY_HH */
