#اگر_غیر_تعریف_شدہ OT_LAYOUT_GPOS_SINGLEPOSFORMAT2_HH
#تعریف_کرو OT_LAYOUT_GPOS_SINGLEPOSFORMAT2_HH

#شامل "Common.hh"

نامخلا OT {
نامخلا Layout {
نامخلا GPOS_impl {

/* Keep the iterator type independent of the source table width so
 * the output serializers can share their instantiations. */
ساخت single_pos_قدر_mapper_t
{
  ثابت hb_map_t &glyph_map;
  ثابت hb_array_t<ثابت قدر> &values;
  ثابت غیرعلامتی &sub_لمبائی;

  hb_pair_t<hb_codepoint_t, hb_array_t<ثابت قدر>>
  عامل () (ثابت hb_pair_t<hb_codepoint_t, غیرعلامتی> &_) ثابت
  {
    واپس hb_pair (glyph_map[_.پہلا],
                    values.sub_array (_.second * sub_لمبائی, sub_لمبائی));
  }
};

سانچہ <قسم_نام Types>
ساخت SinglePosFormat2_4 : قدر_بنیاد
{
  محفوظ:
  HBUINT16      format;                 /* Format identifier--format = 2 */
  قسم_نام Types::سانچہ LOffset_مقصد<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of subtable */
  قدر_Format   قدر_Format_2;            /* Defines the types of data in the
                                         * ValueRecord */
  قسم_نام Types::HBUINT
                قدر_گنتی;             /* Number of ValueRecords */
  قدر_ریکارڈ   values;                 /* Array of ValueRecords--positioning
                                         * values applied to glyphs */
  عوامی:
  DEFINE_سائز_ARRAY (2 + Types::LOffset::static_سائز + قدر_Format::static_سائز + Types::HBUINT::static_سائز, values);

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ) &&
                  coverage.sanitize (c, یہ) &&
                  قدر_Format_2.sanitize_اقدار (c, یہ, values, قدر_گنتی));
  }

  منطقی intersects (ثابت hb_سیٹ_t *glyphs) ثابت
  { واپس (یہ+coverage).intersects (glyphs); }

  خالی احاطہ_lookups (hb_احاطہ_lookups_context_t *c) ثابت {}
  خالی collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    اگر (!قدر_Format_2.has_device ()) واپس;

    خودکار it =
    + hb_zip (یہ+coverage, hb_range ((غیرعلامتی) قدر_گنتی))
    | hb_filter (c->glyph_سیٹ, hb_پہلا)
    ;

    اگر (!it) واپس;

    غیرعلامتی sub_لمبائی = قدر_Format_2.get_len ();
    ثابت hb_array_t<ثابت قدر> اقدار_array = values.as_array (قدر_گنتی * sub_لمبائی);

    برائے (غیرعلامتی i : + it
                      | hb_map (hb_دوسرا))
      قدر_Format_2.collect_variation_indices (c, یہ, اقدار_array.sub_array (i * sub_لمبائی, sub_لمبائی));

  }

  خالی collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  { اگر (unlikely (!(یہ+coverage).collect_coverage (c->داخلہ))) واپس; }

  ثابت Coverage &get_coverage () ثابت { واپس یہ+coverage; }

  قدر_Format get_قدر_format () ثابت { واپس قدر_Format_2; }

  منطقی apply (hb_ot_apply_context_t *c) ثابت
  {
    TRACE_APPLY (یہ);
    hb_بفر_t *بفر = c->بفر;
    غیرعلامتی صحیح اشاریہ_2 = (یہ+coverage).get_coverage  (بفر->cur().codepoint);
    اگر (اشاریہ_2 == NOT_COVERED) return_trace (جھوٹ);

    اگر (unlikely (اشاریہ_2 >= قدر_گنتی)) return_trace (جھوٹ);

    اگر (HB_بفر_پیغام_مزید && c->بفر->messaging ())
    {
      c->بفر->پیغام (c->font,
			  "positioning glyph at %u",
			  c->بفر->idx);
    }

    قدر_Format_2.apply_قدر (c, یہ,
                             &values[اشاریہ_2 * قدر_Format_2.get_len ()],
                             بفر->cur_pos());

    اگر (HB_بفر_پیغام_مزید && c->بفر->messaging ())
    {
      c->بفر->پیغام (c->font,
			  "positioned glyph at %u",
			  c->بفر->idx);
    }

    بفر->idx++;
    return_trace (سچ);
  }

  منطقی
  position_single (hb_font_t           *font,
		   hb_blob_t           *جدول_blob,
		   hb_direction_t       direction,
		   hb_codepoint_t       gid,
		   hb_glyph_position_t &pos) ثابت
  {
    غیرعلامتی صحیح اشاریہ_2 = (یہ+coverage).get_coverage  (gid);
    اگر (likely (اشاریہ_2 == NOT_COVERED)) واپس جھوٹ;
    اگر (unlikely (اشاریہ_2 >= قدر_گنتی)) واپس جھوٹ;

    قدر_Format_2.position_single (font, جدول_blob, direction, یہ,
                                 &values[اشاریہ_2 * قدر_Format_2.get_len ()], pos);
    واپس سچ;
  }


  سانچہ<قسم_نام Iterator,
      hb_requires (hb_is_iterator (Iterator))>
  خالی serialize (hb_serialize_context_t *c,
                  ثابت قدر_بنیاد *src,
                  قدر_Format src_format,
                  Iterator it,
                  قدر_Format newFormat,
                  ثابت hb_hashmap_t<غیرعلامتی, hb_pair_t<غیرعلامتی, صحیح>> *layout_variation_idx_delta_map)
  {
    خودکار برآمد = c->extend_min (یہ);
    اگر (unlikely (!برآمد)) واپس;
    اگر (unlikely (!c->check_تفویض (قدر_Format_2, newFormat, HB_SERIALIZE_ERROR_INT_حد_سے_تجاوز))) واپس;
    اگر (unlikely (!c->check_تفویض (قدر_گنتی, it.len (), HB_SERIALIZE_ERROR_ARRAY_حد_سے_تجاوز))) واپس;

    + it
    | hb_map (hb_دوسرا)
    | hb_apply ([&] (hb_array_t<ثابت قدر> _)
    { src_format.copy_اقدار (c, newFormat, src, &_, layout_variation_idx_delta_map); })
    ;

    خودکار glyphs =
    + it
    | hb_map_retains_ترتیب_کاری (hb_پہلا)
    ;

    coverage.serialize_serialize (c, glyphs);
  }

  سانچہ<قسم_نام Iterator,
      hb_requires (hb_is_iterator (Iterator))>
  غیرعلامتی compute_effective_format (ثابت hb_face_t *face,
                                     Iterator it,
                                     منطقی is_instancing, منطقی strip_hints,
                                     منطقی has_gdef_varstore,
                                     ثابت hb_hashmap_t<غیرعلامتی, hb_pair_t<غیرعلامتی, صحیح>> *varidx_delta_map) ثابت
  {
    hb_blob_t* blob = hb_face_reference_جدول (face, HB_لیبل ('f','v','a','r'));
    منطقی has_fvar = (blob != hb_blob_get_خالی_خانے ());
    hb_blob_destroy (blob);

    غیرعلامتی new_format = 0;
    اگر (is_instancing)
    {
      new_format = new_format | قدر_Format_2.get_effective_format (+ it | hb_map (hb_دوسرا), جھوٹ, جھوٹ, یہ, varidx_delta_map);
    }
    /* do not strip hints for VF */
    ورنہ اگر (strip_hints)
    {
      منطقی strip = !has_fvar;
      اگر (has_fvar && !has_gdef_varstore)
        strip = سچ;
      new_format = new_format | قدر_Format_2.get_effective_format (+ it | hb_map (hb_دوسرا), strip, سچ, یہ, خالی_اشاریہ);
    }
    ورنہ
      new_format = قدر_Format_2;

    واپس new_format;
  }

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (یہ);
    ثابت hb_سیٹ_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    غیرعلامتی sub_لمبائی = قدر_Format_2.get_len ();
    خودکار اقدار_array = values.as_array ((غیرعلامتی) قدر_گنتی * sub_لمبائی);

    خودکار it =
    + hb_zip (یہ+coverage, hb_range ((غیرعلامتی) قدر_گنتی))
    | hb_filter (glyphset, hb_پہلا)
    | hb_map_retains_ترتیب_کاری (single_pos_قدر_mapper_t {glyph_map,
                                                       اقدار_array,
                                                       sub_لمبائی})
    ;

    غیرعلامتی new_format = compute_effective_format (c->plan->ماخذ, it,
                                                    منطقی (c->plan->normalized_coords),
                                                    منطقی (c->plan->flags & HB_SUBSET_FLAGS_NO_HINTING),
                                                    c->plan->has_gdef_varstore,
                                                    &c->plan->layout_variation_idx_delta_map);
    منطقی ret = منطقی (it);
    SinglePos_serialize (c->serializer, یہ, قدر_Format_2, it, &c->plan->layout_variation_idx_delta_map, new_format);
    return_trace (ret);
  }
};


}
}
}

#اگر_ختم /* OT_LAYOUT_GPOS_SINGLEPOSFORMAT2_HH */
