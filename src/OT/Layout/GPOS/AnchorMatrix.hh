#اگر_غیر_تعریف_شدہ OT_LAYOUT_GPOS_ANCHORMATRIX_HH
#تعریف_کرو OT_LAYOUT_GPOS_ANCHORMATRIX_HH

نامخلا OT {
نامخلا Layout {
نامخلا GPOS_impl {

HB_اندرونی اندرونی
خالی collect_ligature_anchor_indexes (غیرعلامتی صفیں,
                                     غیرعلامتی class_گنتی,
                                     ثابت hb_map_t &klass_mapping,
                                     hb_sorted_سمتیہ_t<غیرعلامتی> &indexes)
{
  برائے (غیرعلامتی صف = 0; صف < صفیں; صف++)
    برائے (غیرعلامتی ستون = 0; ستون < class_گنتی; ستون++)
      اگر (klass_mapping.has (ستون))
        indexes.push (صف * class_گنتی + ستون);
}

HB_اندرونی اندرونی
خالی collect_anchor_variation_indices (hb_collect_variation_indices_context_t *c,
                                      ثابت خالی *بنیاد,
                                      glyph_ڈیٹا_t offsets,
                                      hb_array_t<ثابت غیرعلامتی> indexes)
{
  برائے (غیرعلامتی i : indexes)
  {
    غیرعلامتی انحراف = offsets[i];
    اگر (انحراف)
      Struct_At_انحراف<ثابت Anchor> (بنیاد, انحراف).collect_variation_indices (c);
  }
}

HB_اندرونی اندرونی
خالی collect_anchor_indexes (ثابت Coverage &coverage,
                             غیرعلامتی صفیں,
                             غیرعلامتی class_گنتی,
                             ثابت hb_سیٹ_t &glyphset,
                             ثابت hb_map_t &klass_mapping,
                             hb_sorted_سمتیہ_t<غیرعلامتی> &indexes)
{
  برائے (ثابت خودکار _ : hb_zip (coverage, hb_range (صفیں)))
    اگر (glyphset.has (_.پہلا))
      برائے (غیرعلامتی ستون = 0; ستون < class_گنتی; ستون++)
        اگر (klass_mapping.has (ستون))
          indexes.push (_.second * class_گنتی + ستون);
}

HB_اندرونی اندرونی
خالی collect_subset_anchor_indexes (ثابت Coverage &coverage,
                                    غیرعلامتی صفیں,
                                    غیرعلامتی class_گنتی,
                                    glyph_ڈیٹا_t offsets,
                                    ثابت hb_سیٹ_t &glyphset,
                                    ثابت hb_map_t &glyph_map,
                                    ثابت hb_map_t &klass_mapping,
                                    hb_sorted_سمتیہ_t<hb_codepoint_t> &new_coverage,
                                    hb_sorted_سمتیہ_t<غیرعلامتی> &indexes)
{
  برائے (ثابت خودکار _ : hb_zip (coverage, hb_range (صفیں)))
  {
    اگر (!glyphset.has (_.پہلا)) جاری;
    غیرعلامتی صف = _.second;
    منطقی non_خالی_خانے = جھوٹ;
    برائے (غیرعلامتی ستون = 0; ستون < class_گنتی; ستون++)
      اگر (klass_mapping.has (ستون) && offsets[صف * class_گنتی + ستون])
      {
        non_خالی_خانے = سچ;
        توڑو;
      }
    اگر (!non_خالی_خانے) جاری;

    new_coverage.push (glyph_map.get (_.پہلا));
    برائے (غیرعلامتی ستون = 0; ستون < class_گنتی; ستون++)
      اگر (klass_mapping.has (ستون))
        indexes.push (صف * class_گنتی + ستون);
  }
}

سانچہ <قسم_نام Types, قسم_نام صف_گنتی=قسم_نام Types::HBUINT>
ساخت Anchor_میٹرکس
{
  صف_گنتی      صفیں;                   /* Number of rows */
  Unsized_Array_کا<قسم_نام Types::سانچہ انحراف_مقصد<Anchor, Anchor_میٹرکس>>
                میٹرکس_Z;                /* Matrix of offsets to Anchor tables--
                                         * from beginning of AnchorMatrix table */
  عوامی:
  DEFINE_سائز_ARRAY (حجم (صف_گنتی), میٹرکس_Z);

  منطقی sanitize (hb_sanitize_context_t *c, غیرعلامتی صحیح ستون_تعداد) ثابت
  {
    TRACE_SANITIZE (یہ);
    اگر (!c->check_struct (یہ)) return_trace (جھوٹ);
    hb_barrier ();
    اگر (unlikely (hb_unsigned_mul_overflows (صفیں, ستون_تعداد))) return_trace (جھوٹ);
    غیرعلامتی صحیح گنتی = صفیں * ستون_تعداد;
    اگر (!c->check_array (میٹرکس_Z.arrayZ, گنتی)) return_trace (جھوٹ);

    اگر (c->lazy_some_gpos)
      return_trace (سچ);

    hb_barrier ();
    برائے (غیرعلامتی صحیح i = 0; i < گنتی; i++)
      اگر (!میٹرکس_Z[i].sanitize (c, یہ)) return_trace (جھوٹ);
    return_trace (سچ);
  }

  ثابت Anchor& get_anchor (hb_ot_apply_context_t *c,
			    غیرعلامتی صحیح صف, غیرعلامتی صحیح ستون,
			    غیرعلامتی صحیح ستون_تعداد, منطقی *ملا) ثابت
  {
    *ملا = جھوٹ;
    اگر (unlikely (صف >= صفیں || ستون >= ستون_تعداد)) واپس Null (Anchor);
    خودکار &انحراف = میٹرکس_Z[صف * ستون_تعداد + ستون];
    اگر (unlikely (!انحراف.sanitize (&c->sanitizer, یہ))) واپس Null (Anchor);
    hb_barrier ();
    *ملا = !انحراف.is_null ();
    واپس یہ+انحراف;
  }

  خالی collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  hb_array_t<ثابت غیرعلامتی> indexes) ثابت
  {
    collect_anchor_variation_indices (c, یہ, میٹرکس_Z.arrayZ, indexes);
  }

  سانچہ <قسم_نام Iterator,
      hb_requires (hb_is_iterator (Iterator))>
  منطقی subset (hb_subset_context_t *c,
               غیرعلامتی             عدد_صفیں,
               Iterator             اشاریہ_iter) ثابت
  {
    TRACE_SUBSET (یہ);

    خودکار *برآمد = c->serializer->آغاز_embed (یہ);

    اگر (!اشاریہ_iter) return_trace (جھوٹ);
    اگر (unlikely (!c->serializer->extend_min (برآمد)))  return_trace (جھوٹ);

    برآمد->صفیں = عدد_صفیں;
    برائے (ثابت غیرعلامتی i : اشاریہ_iter)
    {
      خودکار *انحراف = c->serializer->embed (میٹرکس_Z[i]);
      اگر (!انحراف) return_trace (جھوٹ);
      انحراف->serialize_subset (c, میٹرکس_Z[i], یہ);
    }

    return_trace (سچ);
  }

  منطقی انحراف_is_null (غیرعلامتی صف, غیرعلامتی ستون, غیرعلامتی عدد_ستون_تعداد) ثابت
  {
    اگر (unlikely (صف >= صفیں || ستون >= عدد_ستون_تعداد)) واپس سچ;
    خودکار &انحراف = میٹرکس_Z[صف * عدد_ستون_تعداد + ستون];
    واپس انحراف.is_null ();
  }
};


}
}
}

#اگر_ختم /* OT_LAYOUT_GPOS_ANCHORMATRIX_HH */
