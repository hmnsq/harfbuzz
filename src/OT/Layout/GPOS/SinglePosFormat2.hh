#اگر_تعریف_نشده OT_LAYOUT_GPOS_SINGLEPOSFORMAT2_HH
#تعریف_کن OT_LAYOUT_GPOS_SINGLEPOSFORMAT2_HH

#ضمن "Common.hh"

نطاق OT {
نطاق Layout {
نطاق GPOS_impl {

/* Keep the iterator type independent of the source table width so
 * the output serializers can share their instantiations. */
بنية single_pos_مقدار_mapper_t
{
  ثابت hb_map_t &glyph_map;
  ثابت hb_array_t<ثابت مقدار> &values;
  ثابت بیعلامت &sub_درازی;

  hb_pair_t<hb_codepoint_t, hb_array_t<ثابت مقدار>>
  عملگر () (ثابت hb_pair_t<hb_codepoint_t, بیعلامت> &_) ثابت
  {
    أرجع hb_pair (glyph_map[_.اولی],
                    values.sub_array (_.second * sub_درازی, sub_درازی));
  }
};

قالب <نام_نوع Types>
بنية SinglePosFormat2_4 : مقدار_پایه
{
  محمي:
  HBUINT16      format;                 /* Format identifier--format = 2 */
  نام_نوع Types::قالب LOffset_مقصد<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of subtable */
  مقدار_Format   مقدار_Format_2;            /* Defines the types of data in the
                                         * ValueRecord */
  نام_نوع Types::HBUINT
                مقدار_شمار;             /* Number of ValueRecords */
  مقدار_رکورد   values;                 /* Array of ValueRecords--positioning
                                         * values applied to glyphs */
  عام:
  DEFINE_اندازه_ARRAY (2 + Types::LOffset::static_اندازه + مقدار_Format::static_اندازه + Types::HBUINT::static_اندازه, values);

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
                  coverage.sanitize (c, هذا) &&
                  مقدار_Format_2.sanitize_مقادیر (c, هذا, values, مقدار_شمار));
  }

  منطقي intersects (ثابت hb_مجموعه_t *glyphs) ثابت
  { أرجع (هذا+coverage).intersects (glyphs); }

  فراغ بستگی_lookups (hb_بستگی_lookups_context_t *c) ثابت {}
  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    إذا (!مقدار_Format_2.has_device ()) أرجع;

    خودکار it =
    + hb_zip (هذا+coverage, hb_range ((بیعلامت) مقدار_شمار))
    | hb_filter (c->glyph_مجموعه, hb_اولی)
    ;

    إذا (!it) أرجع;

    بیعلامت sub_درازی = مقدار_Format_2.get_len ();
    ثابت hb_array_t<ثابت مقدار> مقادیر_array = values.as_array (مقدار_شمار * sub_درازی);

    لكل (بیعلامت i : + it
                      | hb_map (hb_دومی))
      مقدار_Format_2.collect_variation_indices (c, هذا, مقادیر_array.sub_array (i * sub_درازی, sub_درازی));

  }

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  { إذا (unlikely (!(هذا+coverage).collect_coverage (c->درون_داد))) أرجع; }

  ثابت Coverage &get_coverage () ثابت { أرجع هذا+coverage; }

  مقدار_Format get_مقدار_format () ثابت { أرجع مقدار_Format_2; }

  منطقي apply (hb_ot_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);
    hb_میانگیر_t *میانگیر = c->میانگیر;
    بیعلامت صحيح شاخص_2 = (هذا+coverage).get_coverage  (میانگیر->cur().codepoint);
    إذا (شاخص_2 == NOT_COVERED) return_trace (خطأ);

    إذا (unlikely (شاخص_2 >= مقدار_شمار)) return_trace (خطأ);

    إذا (HB_میانگیر_پیام_بیشتر && c->میانگیر->messaging ())
    {
      c->میانگیر->پیام (c->font,
			  "positioning glyph at %u",
			  c->میانگیر->idx);
    }

    مقدار_Format_2.apply_مقدار (c, هذا,
                             &values[شاخص_2 * مقدار_Format_2.get_len ()],
                             میانگیر->cur_pos());

    إذا (HB_میانگیر_پیام_بیشتر && c->میانگیر->messaging ())
    {
      c->میانگیر->پیام (c->font,
			  "positioned glyph at %u",
			  c->میانگیر->idx);
    }

    میانگیر->idx++;
    return_trace (درست);
  }

  منطقي
  position_single (hb_font_t           *font,
		   hb_blob_t           *جدول_blob,
		   hb_direction_t       direction,
		   hb_codepoint_t       gid,
		   hb_glyph_position_t &pos) ثابت
  {
    بیعلامت صحيح شاخص_2 = (هذا+coverage).get_coverage  (gid);
    إذا (likely (شاخص_2 == NOT_COVERED)) أرجع خطأ;
    إذا (unlikely (شاخص_2 >= مقدار_شمار)) أرجع خطأ;

    مقدار_Format_2.position_single (font, جدول_blob, direction, هذا,
                                 &values[شاخص_2 * مقدار_Format_2.get_len ()], pos);
    أرجع درست;
  }


  قالب<نام_نوع Iterator,
      hb_requires (hb_is_iterator (Iterator))>
  فراغ serialize (hb_serialize_context_t *c,
                  ثابت مقدار_پایه *src,
                  مقدار_Format src_format,
                  Iterator it,
                  مقدار_Format newFormat,
                  ثابت hb_hashmap_t<بیعلامت, hb_pair_t<بیعلامت, صحيح>> *layout_variation_idx_delta_map)
  {
    خودکار بیرون = c->extend_min (هذا);
    إذا (unlikely (!بیرون)) أرجع;
    إذا (unlikely (!c->check_تخصیص_بده (مقدار_Format_2, newFormat, HB_SERIALIZE_ERROR_INT_سرریز))) أرجع;
    إذا (unlikely (!c->check_تخصیص_بده (مقدار_شمار, it.len (), HB_SERIALIZE_ERROR_ARRAY_سرریز))) أرجع;

    + it
    | hb_map (hb_دومی)
    | hb_apply ([&] (hb_array_t<ثابت مقدار> _)
    { src_format.copy_مقادیر (c, newFormat, src, &_, layout_variation_idx_delta_map); })
    ;

    خودکار glyphs =
    + it
    | hb_map_retains_ترتیبدهی (hb_اولی)
    ;

    coverage.serialize_serialize (c, glyphs);
  }

  قالب<نام_نوع Iterator,
      hb_requires (hb_is_iterator (Iterator))>
  بیعلامت compute_effective_format (ثابت hb_face_t *face,
                                     Iterator it,
                                     منطقي is_instancing, منطقي strip_hints,
                                     منطقي has_gdef_varstore,
                                     ثابت hb_hashmap_t<بیعلامت, hb_pair_t<بیعلامت, صحيح>> *varidx_delta_map) ثابت
  {
    hb_blob_t* blob = hb_face_reference_جدول (face, HB_برچسب ('f','v','a','r'));
    منطقي has_fvar = (blob != hb_blob_get_خالی ());
    hb_blob_destroy (blob);

    بیعلامت new_format = 0;
    إذا (is_instancing)
    {
      new_format = new_format | مقدار_Format_2.get_effective_format (+ it | hb_map (hb_دومی), خطأ, خطأ, هذا, varidx_delta_map);
    }
    /* do not strip hints for VF */
    وإلا إذا (strip_hints)
    {
      منطقي strip = !has_fvar;
      إذا (has_fvar && !has_gdef_varstore)
        strip = درست;
      new_format = new_format | مقدار_Format_2.get_effective_format (+ it | hb_map (hb_دومی), strip, درست, هذا, اشاره_تهی);
    }
    وإلا
      new_format = مقدار_Format_2;

    أرجع new_format;
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_مجموعه_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    بیعلامت sub_درازی = مقدار_Format_2.get_len ();
    خودکار مقادیر_array = values.as_array ((بیعلامت) مقدار_شمار * sub_درازی);

    خودکار it =
    + hb_zip (هذا+coverage, hb_range ((بیعلامت) مقدار_شمار))
    | hb_filter (glyphset, hb_اولی)
    | hb_map_retains_ترتیبدهی (single_pos_مقدار_mapper_t {glyph_map,
                                                       مقادیر_array,
                                                       sub_درازی})
    ;

    بیعلامت new_format = compute_effective_format (c->plan->منبع, it,
                                                    منطقي (c->plan->normalized_coords),
                                                    منطقي (c->plan->flags & HB_SUBSET_FLAGS_NO_HINTING),
                                                    c->plan->has_gdef_varstore,
                                                    &c->plan->layout_variation_idx_delta_map);
    منطقي ret = منطقي (it);
    SinglePos_serialize (c->serializer, هذا, مقدار_Format_2, it, &c->plan->layout_variation_idx_delta_map, new_format);
    return_trace (ret);
  }
};


}
}
}

#پایان_اگر /* OT_LAYOUT_GPOS_SINGLEPOSFORMAT2_HH */
