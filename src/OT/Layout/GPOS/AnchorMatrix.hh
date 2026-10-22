#جے_بنا_تعریف OT_LAYOUT_GPOS_ANCHORMATRIX_HH
#تعریف_کرو OT_LAYOUT_GPOS_ANCHORMATRIX_HH

نطاق OT {
نطاق Layout {
نطاق GPOS_impl {

HB_اندرونی مضمن
فراغ collect_ligature_anchor_indexes (بنانشانی صفاں,
                                     بنانشانی class_گنتی,
                                     ثابت hb_map_t &klass_mapping,
                                     hb_sorted_سمتیہ_t<بنانشانی> &indexes)
{
  لكل (بنانشانی صف = 0; صف < صفاں; صف++)
    لكل (بنانشانی خانہ = 0; خانہ < class_گنتی; خانہ++)
      إذا (klass_mapping.has (خانہ))
        indexes.push (صف * class_گنتی + خانہ);
}

HB_اندرونی مضمن
فراغ collect_anchor_variation_indices (hb_collect_variation_indices_context_t *c,
                                      ثابت فراغ *ادھار,
                                      glyph_انکڑے_t offsets,
                                      hb_array_t<ثابت بنانشانی> indexes)
{
  لكل (بنانشانی i : indexes)
  {
    بنانشانی وتھ = offsets[i];
    إذا (وتھ)
      Struct_At_وتھ<ثابت Anchor> (ادھار, وتھ).collect_variation_indices (c);
  }
}

HB_اندرونی مضمن
فراغ collect_anchor_indexes (ثابت Coverage &coverage,
                             بنانشانی صفاں,
                             بنانشانی class_گنتی,
                             ثابت hb_سیٹ_t &glyphset,
                             ثابت hb_map_t &klass_mapping,
                             hb_sorted_سمتیہ_t<بنانشانی> &indexes)
{
  لكل (ثابت خودکار _ : hb_zip (coverage, hb_range (صفاں)))
    إذا (glyphset.has (_.پہلا))
      لكل (بنانشانی خانہ = 0; خانہ < class_گنتی; خانہ++)
        إذا (klass_mapping.has (خانہ))
          indexes.push (_.second * class_گنتی + خانہ);
}

HB_اندرونی مضمن
فراغ collect_subset_anchor_indexes (ثابت Coverage &coverage,
                                    بنانشانی صفاں,
                                    بنانشانی class_گنتی,
                                    glyph_انکڑے_t offsets,
                                    ثابت hb_سیٹ_t &glyphset,
                                    ثابت hb_map_t &glyph_map,
                                    ثابت hb_map_t &klass_mapping,
                                    hb_sorted_سمتیہ_t<hb_codepoint_t> &new_coverage,
                                    hb_sorted_سمتیہ_t<بنانشانی> &indexes)
{
  لكل (ثابت خودکار _ : hb_zip (coverage, hb_range (صفاں)))
  {
    إذا (!glyphset.has (_.پہلا)) استمر;
    بنانشانی صف = _.second;
    منطقي non_خالی_تھاواں = خطأ;
    لكل (بنانشانی خانہ = 0; خانہ < class_گنتی; خانہ++)
      إذا (klass_mapping.has (خانہ) && offsets[صف * class_گنتی + خانہ])
      {
        non_خالی_تھاواں = سچ;
        اكسر;
      }
    إذا (!non_خالی_تھاواں) استمر;

    new_coverage.push (glyph_map.get (_.پہلا));
    لكل (بنانشانی خانہ = 0; خانہ < class_گنتی; خانہ++)
      إذا (klass_mapping.has (خانہ))
        indexes.push (صف * class_گنتی + خانہ);
  }
}

سانچا <قسم_ناں Types, قسم_ناں صف_گنتی=قسم_ناں Types::HBUINT>
بنية Anchor_میٹرکس
{
  صف_گنتی      صفاں;                   /* Number of rows */
  Unsized_Array_دا<قسم_ناں Types::سانچا وتھ_منزل<Anchor, Anchor_میٹرکس>>
                میٹرکس_Z;                /* Matrix of offsets to Anchor tables--
                                         * from beginning of AnchorMatrix table */
  عام:
  DEFINE_ناپ_ARRAY (حجم (صف_گنتی), میٹرکس_Z);

  منطقي sanitize (hb_sanitize_context_t *c, بنانشانی صحيح خانے_گنتی) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (!c->check_struct (هذا)) return_trace (خطأ);
    hb_barrier ();
    إذا (unlikely (hb_unsigned_mul_overflows (صفاں, خانے_گنتی))) return_trace (خطأ);
    بنانشانی صحيح گنتی = صفاں * خانے_گنتی;
    إذا (!c->check_array (میٹرکس_Z.arrayZ, گنتی)) return_trace (خطأ);

    إذا (c->lazy_some_gpos)
      return_trace (سچ);

    hb_barrier ();
    لكل (بنانشانی صحيح i = 0; i < گنتی; i++)
      إذا (!میٹرکس_Z[i].sanitize (c, هذا)) return_trace (خطأ);
    return_trace (سچ);
  }

  ثابت Anchor& get_anchor (hb_ot_apply_context_t *c,
			    بنانشانی صحيح صف, بنانشانی صحيح خانہ,
			    بنانشانی صحيح خانے_گنتی, منطقي *لبھیا) ثابت
  {
    *لبھیا = خطأ;
    إذا (unlikely (صف >= صفاں || خانہ >= خانے_گنتی)) أرجع Null (Anchor);
    خودکار &وتھ = میٹرکس_Z[صف * خانے_گنتی + خانہ];
    إذا (unlikely (!وتھ.sanitize (&c->sanitizer, هذا))) أرجع Null (Anchor);
    hb_barrier ();
    *لبھیا = !وتھ.is_null ();
    أرجع هذا+وتھ;
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  hb_array_t<ثابت بنانشانی> indexes) ثابت
  {
    collect_anchor_variation_indices (c, هذا, میٹرکس_Z.arrayZ, indexes);
  }

  سانچا <قسم_ناں Iterator,
      hb_requires (hb_is_iterator (Iterator))>
  منطقي subset (hb_subset_context_t *c,
               بنانشانی             عدد_صفاں,
               Iterator             اشاریہ_iter) ثابت
  {
    TRACE_SUBSET (هذا);

    خودکار *باہرلا = c->serializer->شروع_embed (هذا);

    إذا (!اشاریہ_iter) return_trace (خطأ);
    إذا (unlikely (!c->serializer->extend_min (باہرلا)))  return_trace (خطأ);

    باہرلا->صفاں = عدد_صفاں;
    لكل (ثابت بنانشانی i : اشاریہ_iter)
    {
      خودکار *وتھ = c->serializer->embed (میٹرکس_Z[i]);
      إذا (!وتھ) return_trace (خطأ);
      وتھ->serialize_subset (c, میٹرکس_Z[i], هذا);
    }

    return_trace (سچ);
  }

  منطقي وتھ_is_null (بنانشانی صف, بنانشانی خانہ, بنانشانی عدد_خانے_گنتی) ثابت
  {
    إذا (unlikely (صف >= صفاں || خانہ >= عدد_خانے_گنتی)) أرجع سچ;
    خودکار &وتھ = میٹرکس_Z[صف * عدد_خانے_گنتی + خانہ];
    أرجع وتھ.is_null ();
  }
};


}
}
}

#جے_انت /* OT_LAYOUT_GPOS_ANCHORMATRIX_HH */
