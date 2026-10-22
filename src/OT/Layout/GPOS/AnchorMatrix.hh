#جيڪڏهن_وصف_نه_ٿيل OT_LAYOUT_GPOS_ANCHORMATRIX_HH
#وصف_ڪريو OT_LAYOUT_GPOS_ANCHORMATRIX_HH

نطاق OT {
نطاق Layout {
نطاق GPOS_impl {

HB_اندروني مضمن
فراغ collect_ligature_anchor_indexes (بنانشان صفون,
                                     بنانشان class_ڳڻپ,
                                     ثابت hb_map_t &klass_mapping,
                                     hb_sorted_ويڪٽر_t<بنانشان> &indexes)
{
  لكل (بنانشان صف = 0; صف < صفون; صف++)
    لكل (بنانشان کانو = 0; کانو < class_ڳڻپ; کانو++)
      إذا (klass_mapping.has (کانو))
        indexes.push (صف * class_ڳڻپ + کانو);
}

HB_اندروني مضمن
فراغ collect_anchor_variation_indices (hb_collect_variation_indices_context_t *c,
                                      ثابت فراغ *بنياد,
                                      glyph_معلومات_t offsets,
                                      hb_array_t<ثابت بنانشان> indexes)
{
  لكل (بنانشان i : indexes)
  {
    بنانشان وٿي = offsets[i];
    إذا (وٿي)
      Struct_At_وٿي<ثابت Anchor> (بنياد, وٿي).collect_variation_indices (c);
  }
}

HB_اندروني مضمن
فراغ collect_anchor_indexes (ثابت Coverage &coverage,
                             بنانشان صفون,
                             بنانشان class_ڳڻپ,
                             ثابت hb_سيٽ_t &glyphset,
                             ثابت hb_map_t &klass_mapping,
                             hb_sorted_ويڪٽر_t<بنانشان> &indexes)
{
  لكل (ثابت خودڪار _ : hb_zip (coverage, hb_range (صفون)))
    إذا (glyphset.has (_.پهريون))
      لكل (بنانشان کانو = 0; کانو < class_ڳڻپ; کانو++)
        إذا (klass_mapping.has (کانو))
          indexes.push (_.second * class_ڳڻپ + کانو);
}

HB_اندروني مضمن
فراغ collect_subset_anchor_indexes (ثابت Coverage &coverage,
                                    بنانشان صفون,
                                    بنانشان class_ڳڻپ,
                                    glyph_معلومات_t offsets,
                                    ثابت hb_سيٽ_t &glyphset,
                                    ثابت hb_map_t &glyph_map,
                                    ثابت hb_map_t &klass_mapping,
                                    hb_sorted_ويڪٽر_t<hb_codepoint_t> &new_coverage,
                                    hb_sorted_ويڪٽر_t<بنانشان> &indexes)
{
  لكل (ثابت خودڪار _ : hb_zip (coverage, hb_range (صفون)))
  {
    إذا (!glyphset.has (_.پهريون)) استمر;
    بنانشان صف = _.second;
    منطقي non_خالي_جايون = خطأ;
    لكل (بنانشان کانو = 0; کانو < class_ڳڻپ; کانو++)
      إذا (klass_mapping.has (کانو) && offsets[صف * class_ڳڻپ + کانو])
      {
        non_خالي_جايون = سچ;
        اكسر;
      }
    إذا (!non_خالي_جايون) استمر;

    new_coverage.push (glyph_map.get (_.پهريون));
    لكل (بنانشان کانو = 0; کانو < class_ڳڻپ; کانو++)
      إذا (klass_mapping.has (کانو))
        indexes.push (صف * class_ڳڻپ + کانو);
  }
}

سانچو <قسم_نالو Types, قسم_نالو صف_ڳڻپ=قسم_نالو Types::HBUINT>
بنية Anchor_ميٽرڪس
{
  صف_ڳڻپ      صفون;                   /* Number of rows */
  Unsized_Array_جو<قسم_نالو Types::سانچو وٿي_منزل<Anchor, Anchor_ميٽرڪس>>
                ميٽرڪس_Z;                /* Matrix of offsets to Anchor tables--
                                         * from beginning of AnchorMatrix table */
  عام:
  DEFINE_سائيز_ARRAY (حجم (صف_ڳڻپ), ميٽرڪس_Z);

  منطقي sanitize (hb_sanitize_context_t *c, بنانشان صحيح کانن_ڳڻپ) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (!c->check_struct (هذا)) return_trace (خطأ);
    hb_barrier ();
    إذا (unlikely (hb_unsigned_mul_overflows (صفون, کانن_ڳڻپ))) return_trace (خطأ);
    بنانشان صحيح ڳڻپ = صفون * کانن_ڳڻپ;
    إذا (!c->check_array (ميٽرڪس_Z.arrayZ, ڳڻپ)) return_trace (خطأ);

    إذا (c->lazy_some_gpos)
      return_trace (سچ);

    hb_barrier ();
    لكل (بنانشان صحيح i = 0; i < ڳڻپ; i++)
      إذا (!ميٽرڪس_Z[i].sanitize (c, هذا)) return_trace (خطأ);
    return_trace (سچ);
  }

  ثابت Anchor& get_anchor (hb_ot_apply_context_t *c,
			    بنانشان صحيح صف, بنانشان صحيح کانو,
			    بنانشان صحيح کانن_ڳڻپ, منطقي *مليو) ثابت
  {
    *مليو = خطأ;
    إذا (unlikely (صف >= صفون || کانو >= کانن_ڳڻپ)) أرجع Null (Anchor);
    خودڪار &وٿي = ميٽرڪس_Z[صف * کانن_ڳڻپ + کانو];
    إذا (unlikely (!وٿي.sanitize (&c->sanitizer, هذا))) أرجع Null (Anchor);
    hb_barrier ();
    *مليو = !وٿي.is_null ();
    أرجع هذا+وٿي;
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  hb_array_t<ثابت بنانشان> indexes) ثابت
  {
    collect_anchor_variation_indices (c, هذا, ميٽرڪس_Z.arrayZ, indexes);
  }

  سانچو <قسم_نالو Iterator,
      hb_requires (hb_is_iterator (Iterator))>
  منطقي subset (hb_subset_context_t *c,
               بنانشان             انگ_صفون,
               Iterator             اشاريو_iter) ثابت
  {
    TRACE_SUBSET (هذا);

    خودڪار *ٻاهريون = c->serializer->شروعات_embed (هذا);

    إذا (!اشاريو_iter) return_trace (خطأ);
    إذا (unlikely (!c->serializer->extend_min (ٻاهريون)))  return_trace (خطأ);

    ٻاهريون->صفون = انگ_صفون;
    لكل (ثابت بنانشان i : اشاريو_iter)
    {
      خودڪار *وٿي = c->serializer->embed (ميٽرڪس_Z[i]);
      إذا (!وٿي) return_trace (خطأ);
      وٿي->serialize_subset (c, ميٽرڪس_Z[i], هذا);
    }

    return_trace (سچ);
  }

  منطقي وٿي_is_null (بنانشان صف, بنانشان کانو, بنانشان انگ_کانن_ڳڻپ) ثابت
  {
    إذا (unlikely (صف >= صفون || کانو >= انگ_کانن_ڳڻپ)) أرجع سچ;
    خودڪار &وٿي = ميٽرڪس_Z[صف * انگ_کانن_ڳڻپ + کانو];
    أرجع وٿي.is_null ();
  }
};


}
}
}

#جيڪڏهن_پورو /* OT_LAYOUT_GPOS_ANCHORMATRIX_HH */
