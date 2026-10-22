#اگر_تعریف_نشده OT_LAYOUT_GPOS_ANCHORMATRIX_HH
#تعریف_کن OT_LAYOUT_GPOS_ANCHORMATRIX_HH

نطاق OT {
نطاق Layout {
نطاق GPOS_impl {

HB_داخلی مضمن
فراغ collect_ligature_anchor_indexes (بیعلامت ردیفها,
                                     بیعلامت class_شمار,
                                     ثابت hb_map_t &klass_mapping,
                                     hb_sorted_وکتور_t<بیعلامت> &indexes)
{
  لكل (بیعلامت ردیف = 0; ردیف < ردیفها; ردیف++)
    لكل (بیعلامت ستون = 0; ستون < class_شمار; ستون++)
      إذا (klass_mapping.has (ستون))
        indexes.push (ردیف * class_شمار + ستون);
}

HB_داخلی مضمن
فراغ collect_anchor_variation_indices (hb_collect_variation_indices_context_t *c,
                                      ثابت فراغ *پایه,
                                      glyph_داده_t offsets,
                                      hb_array_t<ثابت بیعلامت> indexes)
{
  لكل (بیعلامت i : indexes)
  {
    بیعلامت جابه_جایی = offsets[i];
    إذا (جابه_جایی)
      Struct_At_جابه_جایی<ثابت Anchor> (پایه, جابه_جایی).collect_variation_indices (c);
  }
}

HB_داخلی مضمن
فراغ collect_anchor_indexes (ثابت Coverage &coverage,
                             بیعلامت ردیفها,
                             بیعلامت class_شمار,
                             ثابت hb_مجموعه_t &glyphset,
                             ثابت hb_map_t &klass_mapping,
                             hb_sorted_وکتور_t<بیعلامت> &indexes)
{
  لكل (ثابت خودکار _ : hb_zip (coverage, hb_range (ردیفها)))
    إذا (glyphset.has (_.اولی))
      لكل (بیعلامت ستون = 0; ستون < class_شمار; ستون++)
        إذا (klass_mapping.has (ستون))
          indexes.push (_.second * class_شمار + ستون);
}

HB_داخلی مضمن
فراغ collect_subset_anchor_indexes (ثابت Coverage &coverage,
                                    بیعلامت ردیفها,
                                    بیعلامت class_شمار,
                                    glyph_داده_t offsets,
                                    ثابت hb_مجموعه_t &glyphset,
                                    ثابت hb_map_t &glyph_map,
                                    ثابت hb_map_t &klass_mapping,
                                    hb_sorted_وکتور_t<hb_codepoint_t> &new_coverage,
                                    hb_sorted_وکتور_t<بیعلامت> &indexes)
{
  لكل (ثابت خودکار _ : hb_zip (coverage, hb_range (ردیفها)))
  {
    إذا (!glyphset.has (_.اولی)) استمر;
    بیعلامت ردیف = _.second;
    منطقي non_خالی = خطأ;
    لكل (بیعلامت ستون = 0; ستون < class_شمار; ستون++)
      إذا (klass_mapping.has (ستون) && offsets[ردیف * class_شمار + ستون])
      {
        non_خالی = درست;
        اكسر;
      }
    إذا (!non_خالی) استمر;

    new_coverage.push (glyph_map.get (_.اولی));
    لكل (بیعلامت ستون = 0; ستون < class_شمار; ستون++)
      إذا (klass_mapping.has (ستون))
        indexes.push (ردیف * class_شمار + ستون);
  }
}

قالب <نام_نوع Types, نام_نوع ردیف_شمار=نام_نوع Types::HBUINT>
بنية Anchor_ماتریس
{
  ردیف_شمار      ردیفها;                   /* Number of rows */
  Unsized_Array_از<نام_نوع Types::قالب جابه_جایی_مقصد<Anchor, Anchor_ماتریس>>
                ماتریس_Z;                /* Matrix of offsets to Anchor tables--
                                         * from beginning of AnchorMatrix table */
  عام:
  DEFINE_اندازه_ARRAY (حجم (ردیف_شمار), ماتریس_Z);

  منطقي sanitize (hb_sanitize_context_t *c, بیعلامت صحيح تعداد_ستون) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (!c->check_struct (هذا)) return_trace (خطأ);
    hb_barrier ();
    إذا (unlikely (hb_unsigned_mul_overflows (ردیفها, تعداد_ستون))) return_trace (خطأ);
    بیعلامت صحيح شمار = ردیفها * تعداد_ستون;
    إذا (!c->check_array (ماتریس_Z.arrayZ, شمار)) return_trace (خطأ);

    إذا (c->lazy_some_gpos)
      return_trace (درست);

    hb_barrier ();
    لكل (بیعلامت صحيح i = 0; i < شمار; i++)
      إذا (!ماتریس_Z[i].sanitize (c, هذا)) return_trace (خطأ);
    return_trace (درست);
  }

  ثابت Anchor& get_anchor (hb_ot_apply_context_t *c,
			    بیعلامت صحيح ردیف, بیعلامت صحيح ستون,
			    بیعلامت صحيح تعداد_ستون, منطقي *یافت) ثابت
  {
    *یافت = خطأ;
    إذا (unlikely (ردیف >= ردیفها || ستون >= تعداد_ستون)) أرجع Null (Anchor);
    خودکار &جابه_جایی = ماتریس_Z[ردیف * تعداد_ستون + ستون];
    إذا (unlikely (!جابه_جایی.sanitize (&c->sanitizer, هذا))) أرجع Null (Anchor);
    hb_barrier ();
    *یافت = !جابه_جایی.is_null ();
    أرجع هذا+جابه_جایی;
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  hb_array_t<ثابت بیعلامت> indexes) ثابت
  {
    collect_anchor_variation_indices (c, هذا, ماتریس_Z.arrayZ, indexes);
  }

  قالب <نام_نوع Iterator,
      hb_requires (hb_is_iterator (Iterator))>
  منطقي subset (hb_subset_context_t *c,
               بیعلامت             رقم_ردیفها,
               Iterator             شاخص_iter) ثابت
  {
    TRACE_SUBSET (هذا);

    خودکار *بیرون = c->serializer->شروع_embed (هذا);

    إذا (!شاخص_iter) return_trace (خطأ);
    إذا (unlikely (!c->serializer->extend_min (بیرون)))  return_trace (خطأ);

    بیرون->ردیفها = رقم_ردیفها;
    لكل (ثابت بیعلامت i : شاخص_iter)
    {
      خودکار *جابه_جایی = c->serializer->embed (ماتریس_Z[i]);
      إذا (!جابه_جایی) return_trace (خطأ);
      جابه_جایی->serialize_subset (c, ماتریس_Z[i], هذا);
    }

    return_trace (درست);
  }

  منطقي جابه_جایی_is_null (بیعلامت ردیف, بیعلامت ستون, بیعلامت رقم_تعداد_ستون) ثابت
  {
    إذا (unlikely (ردیف >= ردیفها || ستون >= رقم_تعداد_ستون)) أرجع درست;
    خودکار &جابه_جایی = ماتریس_Z[ردیف * رقم_تعداد_ستون + ستون];
    أرجع جابه_جایی.is_null ();
  }
};


}
}
}

#پایان_اگر /* OT_LAYOUT_GPOS_ANCHORMATRIX_HH */
