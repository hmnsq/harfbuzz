#اگر_غیر_تعریف_شدہ OT_LAYOUT_GPOS_SINGLEPOSFORMAT1_HH
#تعریف_کرو OT_LAYOUT_GPOS_SINGLEPOSFORMAT1_HH

#شامل "Common.hh"
#شامل "ValueFormat.hh"

نامخلا OT {
نامخلا Layout {
نامخلا GPOS_impl {

سانچہ <قسم_نام Types>
ساخت SinglePosFormat1_3 : قدر_بنیاد
{
  محفوظ:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  قسم_نام Types::سانچہ LOffset_مقصد<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of subtable */
  قدر_Format   قدر_Format_2;            /* Defines the types of data in the
                                         * ValueRecord */
  قدر_ریکارڈ   values;                 /* Defines positioning
                                         * value(s)--applied to all glyphs in
                                         * the Coverage table */
  عوامی:
  DEFINE_سائز_ARRAY (2 + Types::LOffset::static_سائز + قدر_Format::static_سائز, values);

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ) &&
                  coverage.sanitize (c, یہ) &&
		  hb_barrier () &&
                  /* The coverage  table may use a range to represent a set
                   * of glyphs, which means a small number of bytes can
                   * generate a large glyph set. Manually modify the
                   * sanitizer max ops to take this into account.
                   *
                   * Note: This check *must* be right after coverage sanitize. */
                  c->check_ops ((یہ + coverage).get_population () >> 1) &&
                  قدر_Format_2.sanitize_قدر (c, یہ, values));

  }

  منطقی intersects (ثابت hb_سیٹ_t *glyphs) ثابت
  { واپس (یہ+coverage).intersects (glyphs); }

  خالی احاطہ_lookups (hb_احاطہ_lookups_context_t *c) ثابت {}
  خالی collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    اگر (!قدر_Format_2.has_device ()) واپس;

    hb_سیٹ_t intersection;
    (یہ+coverage).intersect_سیٹ (*c->glyph_سیٹ, intersection);
    اگر (!intersection) واپس;

    قدر_Format_2.collect_variation_indices (c, یہ, values.as_array (قدر_Format_2.get_len ()));
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

    اگر (HB_بفر_پیغام_مزید && c->بفر->messaging ())
    {
      c->بفر->پیغام (c->font,
			  "positioning glyph at %u",
			  c->بفر->idx);
    }

    قدر_Format_2.apply_قدر (c, یہ, values, بفر->cur_pos());

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

    قدر_Format_2.position_single (font, جدول_blob, direction, یہ,
                                 values, pos);
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
    اگر (unlikely (!c->extend_min (یہ))) واپس;
    اگر (unlikely (!c->check_تفویض (قدر_Format_2,
                                    newFormat,
                                    HB_SERIALIZE_ERROR_INT_حد_سے_تجاوز))) واپس;

    برائے (ثابت hb_array_t<ثابت قدر>& _ : + it | hb_map (hb_دوسرا))
    {
      src_format.copy_اقدار (c, newFormat, src,  &_, layout_variation_idx_delta_map);
      // Only serialize the first entry in the iterator, the rest are assumed to
      // be the same.
      توڑو;
    }

    خودکار glyphs =
    + it
    | hb_map_retains_ترتیب_کاری (hb_پہلا)
    ;

    coverage.serialize_serialize (c, glyphs);
  }

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (یہ);
    ثابت hb_سیٹ_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    hb_سیٹ_t intersection;
    (یہ+coverage).intersect_سیٹ (glyphset, intersection);

    غیرعلامتی new_format = قدر_Format_2;

    اگر (c->plan->normalized_coords)
    {
      new_format = قدر_Format_2.get_effective_format (values.arrayZ, جھوٹ, جھوٹ, یہ, &c->plan->layout_variation_idx_delta_map);
    }
    /* do not strip hints for VF */
    ورنہ اگر (c->plan->flags & HB_SUBSET_FLAGS_NO_HINTING)
    {
      hb_blob_t* blob = hb_face_reference_جدول (c->plan->ماخذ, HB_لیبل ('f','v','a','r'));
      منطقی has_fvar = (blob != hb_blob_get_خالی_خانے ());
      hb_blob_destroy (blob);

      منطقی strip = !has_fvar;
      /* special case: strip hints when a VF has no GDEF varstore after
       * subsetting*/
      اگر (has_fvar && !c->plan->has_gdef_varstore)
        strip = سچ;
      new_format = قدر_Format_2.get_effective_format (values.arrayZ,
                                                     strip, /* strip hints */
                                                     سچ, /* strip empty */
                                                     یہ, خالی_اشاریہ);
    }

    خودکار it =
    + hb_iter (intersection)
    | hb_map_retains_ترتیب_کاری (glyph_map)
    | hb_zip (hb_repeat (values.as_array (قدر_Format_2.get_len ())))
    ;

    منطقی ret = منطقی (it);
    SinglePos_serialize (c->serializer, یہ, قدر_Format_2, it, &c->plan->layout_variation_idx_delta_map, new_format);
    return_trace (ret);
  }
};

}
}
}

#اگر_ختم /* OT_LAYOUT_GPOS_SINGLEPOSFORMAT1_HH */
