#جيڪڏهن_وصف_نه_ٿيل OT_LAYOUT_GPOS_LIGATUREARRAY_HH
#وصف_ڪريو OT_LAYOUT_GPOS_LIGATUREARRAY_HH

نطاق OT {
نطاق Layout {
نطاق GPOS_impl {

HB_اندروني مضمن
فراغ collect_ligature_variation_indices (hb_collect_variation_indices_context_t *c,
                                        ثابت Coverage &coverage,
                                        ثابت فراغ *بنياد,
                                        glyph_معلومات_t offsets,
                                        بنانشان ڳڻپ,
                                        بنانشان class_ڳڻپ,
                                        ثابت hb_map_t &klass_mapping)
{
  لكل (ثابت خودڪار _ : hb_zip (coverage, hb_range (ڳڻپ)))
  {
    إذا (!c->glyph_سيٽ->has (_.پهريون)) استمر;
    hb_barrier ();
    بنانشان وٿي = offsets[_.second];
    إذا (!وٿي) استمر;

    ثابت فراغ *ميٽرڪس = &Struct_At_وٿي<ثابت HBUINT16> (بنياد, وٿي);
    /* LigatureAttach always has a 16-bit component count; its anchor
     * offsets have the same width as the LigatureArray offsets. */
    بنانشان صفون = *static_cast<ثابت HBUINT16 *> (ميٽرڪس);
    glyph_معلومات_t anchors = offsets;
    anchors.معلومات = (ثابت حرف *) ميٽرڪس + HBUINT16::static_سائيز;

    hb_sorted_ويڪٽر_t<بنانشان> indexes;
    collect_ligature_anchor_indexes (صفون, class_ڳڻپ, klass_mapping, indexes);
    collect_anchor_variation_indices (c, ميٽرڪس, anchors, indexes.iter ());
  }
}


/* Array of LigatureAttach tables ordered by LigatureCoverage Index */
سانچو <قسم_نالو Types>
بنية LigatureArray :
  Types::سانچو Array_جو<قسم_نالو Types::سانچو وٿي_منزل<Anchor_ميٽرڪس<Types, HBUINT16>>>
{
  تعريفنوع Anchor_ميٽرڪس<Types, HBUINT16> LigatureAttach;
  تعريفنوع قسم_نالو Types::سانچو وٿي_منزل<LigatureAttach> Ligature_Attach_وٿي;
  تعريفنوع قسم_نالو Types::سانچو Array_جو<Ligature_Attach_وٿي> Super;

  ثابت LigatureAttach& عامل [] (صحيح i_) ثابت
  {
    بنانشان صحيح i = (بنانشان صحيح) i_;
    إذا (unlikely (i >= هذا->len)) أرجع Null (LigatureAttach);
    hb_barrier ();
    أرجع هذا+هذا->arrayZ[i];
  }

  سانچو <قسم_نالو ...Ts>
  منطقي sanitize (hb_sanitize_context_t *c, Ts&&... ds) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (Super::sanitize (c, هذا, std::forward<Ts> (ds)...));
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  ثابت Coverage &coverage,
                                  بنانشان class_ڳڻپ,
                                  ثابت hb_map_t &klass_mapping) ثابت
  {
    collect_ligature_variation_indices (c, coverage, هذا, هذا->arrayZ,
                                        هذا->len, class_ڳڻپ, klass_mapping);
  }

  سانچو <قسم_نالو Iterator,
            hb_requires (hb_is_iterator (Iterator))>
  منطقي subset (hb_subset_context_t *c,
               Iterator             coverage,
               بنانشان             class_ڳڻپ,
               ثابت hb_map_t      *klass_mapping,
               hb_sorted_ويڪٽر_t<hb_codepoint_t> &new_coverage /*ٻاهريون*/) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_map_t &glyph_map = c->plan->glyph_map_gsub;

    خودڪار *ٻاهريون = c->serializer->شروعات_embed (هذا);
    إذا (unlikely (!c->serializer->extend_min (ٻاهريون)))  return_trace (خطأ);

    منطقي ret = خطأ;
    لكل (ثابت خودڪار _ : + hb_zip (coverage, *هذا)
                        | hb_filter (glyph_map, hb_پهريون))
    {
      ثابت LigatureAttach& src = (هذا + _.second);
      منطقي non_خالي_جايون = + hb_range (src.صفون * class_ڳڻپ)
                       | hb_filter ([=] (بنانشان اشاريو_2) { أرجع klass_mapping->has (اشاريو_2 % class_ڳڻپ); })
                       | hb_map ([&] (ثابت بنانشان اشاريو_2) { أرجع !src.وٿي_is_null (اشاريو_2 / class_ڳڻپ, اشاريو_2 % class_ڳڻپ, class_ڳڻپ); })
                       | hb_any;

      إذا (!non_خالي_جايون) استمر;

      خودڪار *ميٽرڪس = ٻاهريون->serialize_append (c->serializer);
      إذا (unlikely (!ميٽرڪس)) return_trace (خطأ);

      خودڪار indexes =
          + hb_range (src.صفون * class_ڳڻپ)
          | hb_filter ([=] (بنانشان اشاريو_2) { أرجع klass_mapping->has (اشاريو_2 % class_ڳڻپ); })
          ;
      ret |= ميٽرڪس->serialize_subset (c,
				       _.second,
				       هذا,
				       src.صفون,
				       indexes);

      hb_codepoint_t new_gid = glyph_map.get (_.پهريون);
      new_coverage.push (new_gid);
    }
    return_trace (ret);
  }
};


}
}
}

#جيڪڏهن_پورو /* OT_LAYOUT_GPOS_LIGATUREARRAY_HH */
