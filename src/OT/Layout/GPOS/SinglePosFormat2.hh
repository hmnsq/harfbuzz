#جے_بنا_تعریف OT_LAYOUT_GPOS_SINGLEPOSFORMAT2_HH
#تعریف_کرو OT_LAYOUT_GPOS_SINGLEPOSFORMAT2_HH

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
  ثابت بنانشانی &sub_لمائی;

  hb_pair_t<hb_codepoint_t, hb_array_t<ثابت قدر>>
  عامل () (ثابت hb_pair_t<hb_codepoint_t, بنانشانی> &_) ثابت
  {
    أرجع hb_pair (glyph_map[_.پہلا],
                    values.sub_array (_.second * sub_لمائی, sub_لمائی));
  }
};

سانچا <قسم_ناں Types>
بنية SinglePosFormat2_4 : قدر_ادھار
{
  محمي:
  HBUINT16      format;                 /* Format identifier--format = 2 */
  قسم_ناں Types::سانچا LOffset_منزل<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of subtable */
  قدر_Format   قدر_Format_2;            /* Defines the types of data in the
                                         * ValueRecord */
  قسم_ناں Types::HBUINT
                قدر_گنتی;             /* Number of ValueRecords */
  قدر_ریکارڈ   values;                 /* Array of ValueRecords--positioning
                                         * values applied to glyphs */
  عام:
  DEFINE_ناپ_ARRAY (2 + Types::LOffset::static_ناپ + قدر_Format::static_ناپ + Types::HBUINT::static_ناپ, values);

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
                  coverage.sanitize (c, هذا) &&
                  قدر_Format_2.sanitize_اقدار (c, هذا, values, قدر_گنتی));
  }

  منطقي intersects (ثابت hb_سیٹ_t *glyphs) ثابت
  { أرجع (هذا+coverage).intersects (glyphs); }

  فراغ گھیرا_lookups (hb_گھیرا_lookups_context_t *c) ثابت {}
  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    إذا (!قدر_Format_2.has_device ()) أرجع;

    خودکار it =
    + hb_zip (هذا+coverage, hb_range ((بنانشانی) قدر_گنتی))
    | hb_filter (c->glyph_سیٹ, hb_پہلا)
    ;

    إذا (!it) أرجع;

    بنانشانی sub_لمائی = قدر_Format_2.get_len ();
    ثابت hb_array_t<ثابت قدر> اقدار_array = values.as_array (قدر_گنتی * sub_لمائی);

    لكل (بنانشانی i : + it
                      | hb_map (hb_دوجا))
      قدر_Format_2.collect_variation_indices (c, هذا, اقدار_array.sub_array (i * sub_لمائی, sub_لمائی));

  }

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  { إذا (unlikely (!(هذا+coverage).collect_coverage (c->داخلہ))) أرجع; }

  ثابت Coverage &get_coverage () ثابت { أرجع هذا+coverage; }

  قدر_Format get_قدر_format () ثابت { أرجع قدر_Format_2; }

  منطقي apply (hb_ot_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);
    hb_بفر_t *بفر = c->بفر;
    بنانشانی صحيح اشاریہ_2 = (هذا+coverage).get_coverage  (بفر->cur().codepoint);
    إذا (اشاریہ_2 == NOT_COVERED) return_trace (خطأ);

    إذا (unlikely (اشاریہ_2 >= قدر_گنتی)) return_trace (خطأ);

    إذا (HB_بفر_سنیہا_ہور && c->بفر->messaging ())
    {
      c->بفر->سنیہا (c->font,
			  "positioning glyph at %u",
			  c->بفر->idx);
    }

    قدر_Format_2.apply_قدر (c, هذا,
                             &values[اشاریہ_2 * قدر_Format_2.get_len ()],
                             بفر->cur_pos());

    إذا (HB_بفر_سنیہا_ہور && c->بفر->messaging ())
    {
      c->بفر->سنیہا (c->font,
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
    بنانشانی صحيح اشاریہ_2 = (هذا+coverage).get_coverage  (gid);
    إذا (likely (اشاریہ_2 == NOT_COVERED)) أرجع خطأ;
    إذا (unlikely (اشاریہ_2 >= قدر_گنتی)) أرجع خطأ;

    قدر_Format_2.position_single (font, جدول_blob, direction, هذا,
                                 &values[اشاریہ_2 * قدر_Format_2.get_len ()], pos);
    أرجع سچ;
  }


  سانچا<قسم_ناں Iterator,
      hb_requires (hb_is_iterator (Iterator))>
  فراغ serialize (hb_serialize_context_t *c,
                  ثابت قدر_ادھار *src,
                  قدر_Format src_format,
                  Iterator it,
                  قدر_Format newFormat,
                  ثابت hb_hashmap_t<بنانشانی, hb_pair_t<بنانشانی, صحيح>> *layout_variation_idx_delta_map)
  {
    خودکار باہرلا = c->extend_min (هذا);
    إذا (unlikely (!باہرلا)) أرجع;
    إذا (unlikely (!c->check_سونپو (قدر_Format_2, newFormat, HB_SERIALIZE_ERROR_INT_حد_توں_ودھ))) أرجع;
    إذا (unlikely (!c->check_سونپو (قدر_گنتی, it.len (), HB_SERIALIZE_ERROR_ARRAY_حد_توں_ودھ))) أرجع;

    + it
    | hb_map (hb_دوجا)
    | hb_apply ([&] (hb_array_t<ثابت قدر> _)
    { src_format.copy_اقدار (c, newFormat, src, &_, layout_variation_idx_delta_map); })
    ;

    خودکار glyphs =
    + it
    | hb_map_retains_چھانٹی (hb_پہلا)
    ;

    coverage.serialize_serialize (c, glyphs);
  }

  سانچا<قسم_ناں Iterator,
      hb_requires (hb_is_iterator (Iterator))>
  بنانشانی compute_effective_format (ثابت hb_face_t *face,
                                     Iterator it,
                                     منطقي is_instancing, منطقي strip_hints,
                                     منطقي has_gdef_varstore,
                                     ثابت hb_hashmap_t<بنانشانی, hb_pair_t<بنانشانی, صحيح>> *varidx_delta_map) ثابت
  {
    hb_blob_t* blob = hb_face_reference_جدول (face, HB_ٹیگ ('f','v','a','r'));
    منطقي has_fvar = (blob != hb_blob_get_خالی_تھاواں ());
    hb_blob_destroy (blob);

    بنانشانی new_format = 0;
    إذا (is_instancing)
    {
      new_format = new_format | قدر_Format_2.get_effective_format (+ it | hb_map (hb_دوجا), خطأ, خطأ, هذا, varidx_delta_map);
    }
    /* do not strip hints for VF */
    وإلا إذا (strip_hints)
    {
      منطقي strip = !has_fvar;
      إذا (has_fvar && !has_gdef_varstore)
        strip = سچ;
      new_format = new_format | قدر_Format_2.get_effective_format (+ it | hb_map (hb_دوجا), strip, سچ, هذا, خالی_سوچک);
    }
    وإلا
      new_format = قدر_Format_2;

    أرجع new_format;
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_سیٹ_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    بنانشانی sub_لمائی = قدر_Format_2.get_len ();
    خودکار اقدار_array = values.as_array ((بنانشانی) قدر_گنتی * sub_لمائی);

    خودکار it =
    + hb_zip (هذا+coverage, hb_range ((بنانشانی) قدر_گنتی))
    | hb_filter (glyphset, hb_پہلا)
    | hb_map_retains_چھانٹی (single_pos_قدر_mapper_t {glyph_map,
                                                       اقدار_array,
                                                       sub_لمائی})
    ;

    بنانشانی new_format = compute_effective_format (c->plan->سروت, it,
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

#جے_انت /* OT_LAYOUT_GPOS_SINGLEPOSFORMAT2_HH */
