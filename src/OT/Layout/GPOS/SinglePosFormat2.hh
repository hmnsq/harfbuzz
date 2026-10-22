#جيڪڏهن_وصف_نه_ٿيل OT_LAYOUT_GPOS_SINGLEPOSFORMAT2_HH
#وصف_ڪريو OT_LAYOUT_GPOS_SINGLEPOSFORMAT2_HH

#ضمن "Common.hh"

نطاق OT {
نطاق Layout {
نطاق GPOS_impl {

/* Keep the iterator type independent of the source table width so
 * the output serializers can share their instantiations. */
بنية single_pos_قدر_mapper_t
{
  ثابت hb_map_t &glyph_map;
  ثابت hb_array_t<ثابت قدر> &values;
  ثابت بنانشان &sub_ڊيگهائي;

  hb_pair_t<hb_codepoint_t, hb_array_t<ثابت قدر>>
  عامل () (ثابت hb_pair_t<hb_codepoint_t, بنانشان> &_) ثابت
  {
    أرجع hb_pair (glyph_map[_.پهريون],
                    values.sub_array (_.second * sub_ڊيگهائي, sub_ڊيگهائي));
  }
};

سانچو <قسم_نالو Types>
بنية SinglePosFormat2_4 : قدر_بنياد
{
  محمي:
  HBUINT16      format;                 /* Format identifier--format = 2 */
  قسم_نالو Types::سانچو LOffset_منزل<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of subtable */
  قدر_Format   قدر_Format_2;            /* Defines the types of data in the
                                         * ValueRecord */
  قسم_نالو Types::HBUINT
                قدر_ڳڻپ;             /* Number of ValueRecords */
  قدر_رڪارڊ   values;                 /* Array of ValueRecords--positioning
                                         * values applied to glyphs */
  عام:
  DEFINE_سائيز_ARRAY (2 + Types::LOffset::static_سائيز + قدر_Format::static_سائيز + Types::HBUINT::static_سائيز, values);

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
                  coverage.sanitize (c, هذا) &&
                  قدر_Format_2.sanitize_قدرون (c, هذا, values, قدر_ڳڻپ));
  }

  منطقي intersects (ثابت hb_سيٽ_t *glyphs) ثابت
  { أرجع (هذا+coverage).intersects (glyphs); }

  فراغ گهيرو_lookups (hb_گهيرو_lookups_context_t *c) ثابت {}
  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    إذا (!قدر_Format_2.has_device ()) أرجع;

    خودڪار it =
    + hb_zip (هذا+coverage, hb_range ((بنانشان) قدر_ڳڻپ))
    | hb_filter (c->glyph_سيٽ, hb_پهريون)
    ;

    إذا (!it) أرجع;

    بنانشان sub_ڊيگهائي = قدر_Format_2.get_len ();
    ثابت hb_array_t<ثابت قدر> قدرون_array = values.as_array (قدر_ڳڻپ * sub_ڊيگهائي);

    لكل (بنانشان i : + it
                      | hb_map (hb_ٻيو))
      قدر_Format_2.collect_variation_indices (c, هذا, قدرون_array.sub_array (i * sub_ڊيگهائي, sub_ڊيگهائي));

  }

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  { إذا (unlikely (!(هذا+coverage).collect_coverage (c->داخلا_2))) أرجع; }

  ثابت Coverage &get_coverage () ثابت { أرجع هذا+coverage; }

  قدر_Format get_قدر_format () ثابت { أرجع قدر_Format_2; }

  منطقي apply (hb_ot_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);
    hb_بفر_t *بفر = c->بفر;
    بنانشان صحيح اشاريو_2 = (هذا+coverage).get_coverage  (بفر->cur().codepoint);
    إذا (اشاريو_2 == NOT_COVERED) return_trace (خطأ);

    إذا (unlikely (اشاريو_2 >= قدر_ڳڻپ)) return_trace (خطأ);

    إذا (HB_بفر_پيغام_وڌيڪ && c->بفر->messaging ())
    {
      c->بفر->پيغام (c->font,
			  "positioning glyph at %u",
			  c->بفر->idx);
    }

    قدر_Format_2.apply_قدر (c, هذا,
                             &values[اشاريو_2 * قدر_Format_2.get_len ()],
                             بفر->cur_pos());

    إذا (HB_بفر_پيغام_وڌيڪ && c->بفر->messaging ())
    {
      c->بفر->پيغام (c->font,
			  "positioned glyph at %u",
			  c->بفر->idx);
    }

    بفر->idx++;
    return_trace (سچ);
  }

  منطقي
  position_single (hb_font_t           *font,
		   hb_blob_t           *جدول_blob,
		   hb_direction_t       direction,
		   hb_codepoint_t       gid,
		   hb_glyph_position_t &pos) ثابت
  {
    بنانشان صحيح اشاريو_2 = (هذا+coverage).get_coverage  (gid);
    إذا (likely (اشاريو_2 == NOT_COVERED)) أرجع خطأ;
    إذا (unlikely (اشاريو_2 >= قدر_ڳڻپ)) أرجع خطأ;

    قدر_Format_2.position_single (font, جدول_blob, direction, هذا,
                                 &values[اشاريو_2 * قدر_Format_2.get_len ()], pos);
    أرجع سچ;
  }


  سانچو<قسم_نالو Iterator,
      hb_requires (hb_is_iterator (Iterator))>
  فراغ serialize (hb_serialize_context_t *c,
                  ثابت قدر_بنياد *src,
                  قدر_Format src_format,
                  Iterator it,
                  قدر_Format newFormat,
                  ثابت hb_hashmap_t<بنانشان, hb_pair_t<بنانشان, صحيح>> *layout_variation_idx_delta_map)
  {
    خودڪار ٻاهريون = c->extend_min (هذا);
    إذا (unlikely (!ٻاهريون)) أرجع;
    إذا (unlikely (!c->check_سونپيو (قدر_Format_2, newFormat, HB_SERIALIZE_ERROR_INT_حد_کان_مٿي))) أرجع;
    إذا (unlikely (!c->check_سونپيو (قدر_ڳڻپ, it.len (), HB_SERIALIZE_ERROR_ARRAY_حد_کان_مٿي))) أرجع;

    + it
    | hb_map (hb_ٻيو)
    | hb_apply ([&] (hb_array_t<ثابت قدر> _)
    { src_format.copy_قدرون (c, newFormat, src, &_, layout_variation_idx_delta_map); })
    ;

    خودڪار glyphs =
    + it
    | hb_map_retains_ترتيب_ڏيڻ (hb_پهريون)
    ;

    coverage.serialize_serialize (c, glyphs);
  }

  سانچو<قسم_نالو Iterator,
      hb_requires (hb_is_iterator (Iterator))>
  بنانشان compute_effective_format (ثابت hb_face_t *face,
                                     Iterator it,
                                     منطقي is_instancing, منطقي strip_hints,
                                     منطقي has_gdef_varstore,
                                     ثابت hb_hashmap_t<بنانشان, hb_pair_t<بنانشان, صحيح>> *varidx_delta_map) ثابت
  {
    hb_blob_t* blob = hb_face_reference_جدول (face, HB_ٽيگ ('f','v','a','r'));
    منطقي has_fvar = (blob != hb_blob_get_خالي_جايون ());
    hb_blob_destroy (blob);

    بنانشان new_format = 0;
    إذا (is_instancing)
    {
      new_format = new_format | قدر_Format_2.get_effective_format (+ it | hb_map (hb_ٻيو), خطأ, خطأ, هذا, varidx_delta_map);
    }
    /* do not strip hints for VF */
    وإلا إذا (strip_hints)
    {
      منطقي strip = !has_fvar;
      إذا (has_fvar && !has_gdef_varstore)
        strip = سچ;
      new_format = new_format | قدر_Format_2.get_effective_format (+ it | hb_map (hb_ٻيو), strip, سچ, هذا, خالي_اشارو);
    }
    وإلا
      new_format = قدر_Format_2;

    أرجع new_format;
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_سيٽ_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    بنانشان sub_ڊيگهائي = قدر_Format_2.get_len ();
    خودڪار قدرون_array = values.as_array ((بنانشان) قدر_ڳڻپ * sub_ڊيگهائي);

    خودڪار it =
    + hb_zip (هذا+coverage, hb_range ((بنانشان) قدر_ڳڻپ))
    | hb_filter (glyphset, hb_پهريون)
    | hb_map_retains_ترتيب_ڏيڻ (single_pos_قدر_mapper_t {glyph_map,
                                                       قدرون_array,
                                                       sub_ڊيگهائي})
    ;

    بنانشان new_format = compute_effective_format (c->plan->ذريعو, it,
                                                    منطقي (c->plan->normalized_coords),
                                                    منطقي (c->plan->flags & HB_SUBSET_FLAGS_NO_HINTING),
                                                    c->plan->has_gdef_varstore,
                                                    &c->plan->layout_variation_idx_delta_map);
    منطقي ret = منطقي (it);
    SinglePos_serialize (c->serializer, هذا, قدر_Format_2, it, &c->plan->layout_variation_idx_delta_map, new_format);
    return_trace (ret);
  }
};


}
}
}

#جيڪڏهن_پورو /* OT_LAYOUT_GPOS_SINGLEPOSFORMAT2_HH */
