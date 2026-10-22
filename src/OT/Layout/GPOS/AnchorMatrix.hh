#که_نه_تعریف_شوی OT_LAYOUT_GPOS_ANCHORMATRIX_HH
#تعریف_کړه OT_LAYOUT_GPOS_ANCHORMATRIX_HH

نطاق OT {
نطاق Layout {
نطاق GPOS_impl {

HB_دننی مضمن
فراغ collect_ligature_anchor_indexes (بینښه لیکې,
                                     بینښه class_شمېر,
                                     ثابت hb_map_t &klass_mapping,
                                     hb_sorted_ویکټور_t<بینښه> &indexes)
{
  لكل (بینښه لیکه = 0; لیکه < لیکې; لیکه++)
    لكل (بینښه ستون = 0; ستون < class_شمېر; ستون++)
      إذا (klass_mapping.has (ستون))
        indexes.push (لیکه * class_شمېر + ستون);
}

HB_دننی مضمن
فراغ collect_anchor_variation_indices (hb_collect_variation_indices_context_t *c,
                                      ثابت فراغ *بنسټ,
                                      glyph_معلومات_t offsets,
                                      hb_array_t<ثابت بینښه> indexes)
{
  لكل (بینښه i : indexes)
  {
    بینښه دننه_واټن = offsets[i];
    إذا (دننه_واټن)
      Struct_At_دننه_واټن<ثابت Anchor> (بنسټ, دننه_واټن).collect_variation_indices (c);
  }
}

HB_دننی مضمن
فراغ collect_anchor_indexes (ثابت Coverage &coverage,
                             بینښه لیکې,
                             بینښه class_شمېر,
                             ثابت hb_سېټ_t &glyphset,
                             ثابت hb_map_t &klass_mapping,
                             hb_sorted_ویکټور_t<بینښه> &indexes)
{
  لكل (ثابت خپلکاری _ : hb_zip (coverage, hb_range (لیکې)))
    إذا (glyphset.has (_.لومړی))
      لكل (بینښه ستون = 0; ستون < class_شمېر; ستون++)
        إذا (klass_mapping.has (ستون))
          indexes.push (_.second * class_شمېر + ستون);
}

HB_دننی مضمن
فراغ collect_subset_anchor_indexes (ثابت Coverage &coverage,
                                    بینښه لیکې,
                                    بینښه class_شمېر,
                                    glyph_معلومات_t offsets,
                                    ثابت hb_سېټ_t &glyphset,
                                    ثابت hb_map_t &glyph_map,
                                    ثابت hb_map_t &klass_mapping,
                                    hb_sorted_ویکټور_t<hb_codepoint_t> &new_coverage,
                                    hb_sorted_ویکټور_t<بینښه> &indexes)
{
  لكل (ثابت خپلکاری _ : hb_zip (coverage, hb_range (لیکې)))
  {
    إذا (!glyphset.has (_.لومړی)) استمر;
    بینښه لیکه = _.second;
    منطقي non_تشه = خطأ;
    لكل (بینښه ستون = 0; ستون < class_شمېر; ستون++)
      إذا (klass_mapping.has (ستون) && offsets[لیکه * class_شمېر + ستون])
      {
        non_تشه = ریښتیا;
        اكسر;
      }
    إذا (!non_تشه) استمر;

    new_coverage.push (glyph_map.get (_.لومړی));
    لكل (بینښه ستون = 0; ستون < class_شمېر; ستون++)
      إذا (klass_mapping.has (ستون))
        indexes.push (لیکه * class_شمېر + ستون);
  }
}

کينډۍ <ډول_نوم Types, ډول_نوم لیکه_شمېر=ډول_نوم Types::HBUINT>
بنية Anchor_ماتریکس
{
  لیکه_شمېر      لیکې;                   /* Number of rows */
  Unsized_Array_د<ډول_نوم Types::کينډۍ دننه_واټن_منزل<Anchor, Anchor_ماتریکس>>
                ماتریکس_Z;                /* Matrix of offsets to Anchor tables--
                                         * from beginning of AnchorMatrix table */
  عام:
  DEFINE_اندازه_ARRAY (حجم (لیکه_شمېر), ماتریکس_Z);

  منطقي sanitize (hb_sanitize_context_t *c, بینښه صحيح ستون_شمېر) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (!c->check_struct (هذا)) return_trace (خطأ);
    hb_barrier ();
    إذا (unlikely (hb_unsigned_mul_overflows (لیکې, ستون_شمېر))) return_trace (خطأ);
    بینښه صحيح شمېر = لیکې * ستون_شمېر;
    إذا (!c->check_array (ماتریکس_Z.arrayZ, شمېر)) return_trace (خطأ);

    إذا (c->lazy_some_gpos)
      return_trace (ریښتیا);

    hb_barrier ();
    لكل (بینښه صحيح i = 0; i < شمېر; i++)
      إذا (!ماتریکس_Z[i].sanitize (c, هذا)) return_trace (خطأ);
    return_trace (ریښتیا);
  }

  ثابت Anchor& get_anchor (hb_ot_apply_context_t *c,
			    بینښه صحيح لیکه, بینښه صحيح ستون,
			    بینښه صحيح ستون_شمېر, منطقي *وموندل) ثابت
  {
    *وموندل = خطأ;
    إذا (unlikely (لیکه >= لیکې || ستون >= ستون_شمېر)) أرجع Null (Anchor);
    خپلکاری &دننه_واټن = ماتریکس_Z[لیکه * ستون_شمېر + ستون];
    إذا (unlikely (!دننه_واټن.sanitize (&c->sanitizer, هذا))) أرجع Null (Anchor);
    hb_barrier ();
    *وموندل = !دننه_واټن.is_null ();
    أرجع هذا+دننه_واټن;
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  hb_array_t<ثابت بینښه> indexes) ثابت
  {
    collect_anchor_variation_indices (c, هذا, ماتریکس_Z.arrayZ, indexes);
  }

  کينډۍ <ډول_نوم Iterator,
      hb_requires (hb_is_iterator (Iterator))>
  منطقي subset (hb_subset_context_t *c,
               بینښه             عدد_لیکې,
               Iterator             شاخص_iter) ثابت
  {
    TRACE_SUBSET (هذا);

    خپلکاری *بهر = c->serializer->پیل_embed (هذا);

    إذا (!شاخص_iter) return_trace (خطأ);
    إذا (unlikely (!c->serializer->extend_min (بهر)))  return_trace (خطأ);

    بهر->لیکې = عدد_لیکې;
    لكل (ثابت بینښه i : شاخص_iter)
    {
      خپلکاری *دننه_واټن = c->serializer->embed (ماتریکس_Z[i]);
      إذا (!دننه_واټن) return_trace (خطأ);
      دننه_واټن->serialize_subset (c, ماتریکس_Z[i], هذا);
    }

    return_trace (ریښتیا);
  }

  منطقي دننه_واټن_is_null (بینښه لیکه, بینښه ستون, بینښه عدد_ستون_شمېر) ثابت
  {
    إذا (unlikely (لیکه >= لیکې || ستون >= عدد_ستون_شمېر)) أرجع ریښتیا;
    خپلکاری &دننه_واټن = ماتریکس_Z[لیکه * عدد_ستون_شمېر + ستون];
    أرجع دننه_واټن.is_null ();
  }
};


}
}
}

#د_که_پای /* OT_LAYOUT_GPOS_ANCHORMATRIX_HH */
