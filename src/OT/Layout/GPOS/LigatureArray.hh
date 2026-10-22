#اگر_غیر_تعریف_شدہ OT_LAYOUT_GPOS_LIGATUREARRAY_HH
#تعریف_کرو OT_LAYOUT_GPOS_LIGATUREARRAY_HH

نامخلا OT {
نامخلا Layout {
نامخلا GPOS_impl {

HB_اندرونی اندرونی
خالی collect_ligature_variation_indices (hb_collect_variation_indices_context_t *c,
                                        ثابت Coverage &coverage,
                                        ثابت خالی *بنیاد,
                                        glyph_ڈیٹا_t offsets,
                                        غیرعلامتی گنتی,
                                        غیرعلامتی class_گنتی,
                                        ثابت hb_map_t &klass_mapping)
{
  برائے (ثابت خودکار _ : hb_zip (coverage, hb_range (گنتی)))
  {
    اگر (!c->glyph_سیٹ->has (_.پہلا)) جاری;
    hb_barrier ();
    غیرعلامتی انحراف = offsets[_.second];
    اگر (!انحراف) جاری;

    ثابت خالی *میٹرکس = &Struct_At_انحراف<ثابت HBUINT16> (بنیاد, انحراف);
    /* LigatureAttach always has a 16-bit component count; its anchor
     * offsets have the same width as the LigatureArray offsets. */
    غیرعلامتی صفیں = *static_cast<ثابت HBUINT16 *> (میٹرکس);
    glyph_ڈیٹا_t anchors = offsets;
    anchors.ڈیٹا = (ثابت حرف *) میٹرکس + HBUINT16::static_سائز;

    hb_sorted_سمتیہ_t<غیرعلامتی> indexes;
    collect_ligature_anchor_indexes (صفیں, class_گنتی, klass_mapping, indexes);
    collect_anchor_variation_indices (c, میٹرکس, anchors, indexes.iter ());
  }
}


/* Array of LigatureAttach tables ordered by LigatureCoverage Index */
سانچہ <قسم_نام Types>
ساخت LigatureArray :
  Types::سانچہ Array_کا<قسم_نام Types::سانچہ انحراف_مقصد<Anchor_میٹرکس<Types, HBUINT16>>>
{
  قسمتعریف Anchor_میٹرکس<Types, HBUINT16> LigatureAttach;
  قسمتعریف قسم_نام Types::سانچہ انحراف_مقصد<LigatureAttach> Ligature_Attach_انحراف;
  قسمتعریف قسم_نام Types::سانچہ Array_کا<Ligature_Attach_انحراف> Super;

  ثابت LigatureAttach& عامل [] (صحیح i_) ثابت
  {
    غیرعلامتی صحیح i = (غیرعلامتی صحیح) i_;
    اگر (unlikely (i >= یہ->len)) واپس Null (LigatureAttach);
    hb_barrier ();
    واپس یہ+یہ->arrayZ[i];
  }

  سانچہ <قسم_نام ...Ts>
  منطقی sanitize (hb_sanitize_context_t *c, Ts&&... ds) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (Super::sanitize (c, یہ, std::forward<Ts> (ds)...));
  }

  خالی collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  ثابت Coverage &coverage,
                                  غیرعلامتی class_گنتی,
                                  ثابت hb_map_t &klass_mapping) ثابت
  {
    collect_ligature_variation_indices (c, coverage, یہ, یہ->arrayZ,
                                        یہ->len, class_گنتی, klass_mapping);
  }

  سانچہ <قسم_نام Iterator,
            hb_requires (hb_is_iterator (Iterator))>
  منطقی subset (hb_subset_context_t *c,
               Iterator             coverage,
               غیرعلامتی             class_گنتی,
               ثابت hb_map_t      *klass_mapping,
               hb_sorted_سمتیہ_t<hb_codepoint_t> &new_coverage /*برآمد*/) ثابت
  {
    TRACE_SUBSET (یہ);
    ثابت hb_map_t &glyph_map = c->plan->glyph_map_gsub;

    خودکار *برآمد = c->serializer->آغاز_embed (یہ);
    اگر (unlikely (!c->serializer->extend_min (برآمد)))  return_trace (جھوٹ);

    منطقی ret = جھوٹ;
    برائے (ثابت خودکار _ : + hb_zip (coverage, *یہ)
                        | hb_filter (glyph_map, hb_پہلا))
    {
      ثابت LigatureAttach& src = (یہ + _.second);
      منطقی non_خالی_خانے = + hb_range (src.صفیں * class_گنتی)
                       | hb_filter ([=] (غیرعلامتی اشاریہ_2) { واپس klass_mapping->has (اشاریہ_2 % class_گنتی); })
                       | hb_map ([&] (ثابت غیرعلامتی اشاریہ_2) { واپس !src.انحراف_is_null (اشاریہ_2 / class_گنتی, اشاریہ_2 % class_گنتی, class_گنتی); })
                       | hb_any;

      اگر (!non_خالی_خانے) جاری;

      خودکار *میٹرکس = برآمد->serialize_append (c->serializer);
      اگر (unlikely (!میٹرکس)) return_trace (جھوٹ);

      خودکار indexes =
          + hb_range (src.صفیں * class_گنتی)
          | hb_filter ([=] (غیرعلامتی اشاریہ_2) { واپس klass_mapping->has (اشاریہ_2 % class_گنتی); })
          ;
      ret |= میٹرکس->serialize_subset (c,
				       _.second,
				       یہ,
				       src.صفیں,
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

#اگر_ختم /* OT_LAYOUT_GPOS_LIGATUREARRAY_HH */
