#اگر_تعریف_نشده OT_LAYOUT_GPOS_SINGLEPOSFORMAT1_HH
#تعریف_کن OT_LAYOUT_GPOS_SINGLEPOSFORMAT1_HH

#ضمن "Common.hh"
#ضمن "ValueFormat.hh"

نطاق OT {
نطاق Layout {
نطاق GPOS_impl {

قالب <نام_نوع Types>
بنية SinglePosFormat1_3 : مقدار_پایه
{
  محمي:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  نام_نوع Types::قالب LOffset_مقصد<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of subtable */
  مقدار_Format   مقدار_Format_2;            /* Defines the types of data in the
                                         * ValueRecord */
  مقدار_رکورد   values;                 /* Defines positioning
                                         * value(s)--applied to all glyphs in
                                         * the Coverage table */
  عام:
  DEFINE_اندازه_ARRAY (2 + Types::LOffset::static_اندازه + مقدار_Format::static_اندازه, values);

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
                  coverage.sanitize (c, هذا) &&
		  hb_barrier () &&
                  /* The coverage  table may use a range to represent a set
                   * of glyphs, which means a small number of bytes can
                   * generate a large glyph set. Manually modify the
                   * sanitizer max ops to take this into account.
                   *
                   * Note: This check *must* be right after coverage sanitize. */
                  c->check_ops ((هذا + coverage).get_population () >> 1) &&
                  مقدار_Format_2.sanitize_مقدار (c, هذا, values));

  }

  منطقي intersects (ثابت hb_مجموعه_t *glyphs) ثابت
  { أرجع (هذا+coverage).intersects (glyphs); }

  فراغ بستگی_lookups (hb_بستگی_lookups_context_t *c) ثابت {}
  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    إذا (!مقدار_Format_2.has_device ()) أرجع;

    hb_مجموعه_t intersection;
    (هذا+coverage).intersect_مجموعه (*c->glyph_مجموعه, intersection);
    إذا (!intersection) أرجع;

    مقدار_Format_2.collect_variation_indices (c, هذا, values.as_array (مقدار_Format_2.get_len ()));
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

    إذا (HB_میانگیر_پیام_بیشتر && c->میانگیر->messaging ())
    {
      c->میانگیر->پیام (c->font,
			  "positioning glyph at %u",
			  c->میانگیر->idx);
    }

    مقدار_Format_2.apply_مقدار (c, هذا, values, میانگیر->cur_pos());

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

    مقدار_Format_2.position_single (font, جدول_blob, direction, هذا,
                                 values, pos);
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
    إذا (unlikely (!c->extend_min (هذا))) أرجع;
    إذا (unlikely (!c->check_تخصیص_بده (مقدار_Format_2,
                                    newFormat,
                                    HB_SERIALIZE_ERROR_INT_سرریز))) أرجع;

    لكل (ثابت hb_array_t<ثابت مقدار>& _ : + it | hb_map (hb_دومی))
    {
      src_format.copy_مقادیر (c, newFormat, src,  &_, layout_variation_idx_delta_map);
      // Only serialize the first entry in the iterator, the rest are assumed to
      // be the same.
      اكسر;
    }

    خودکار glyphs =
    + it
    | hb_map_retains_ترتیبدهی (hb_اولی)
    ;

    coverage.serialize_serialize (c, glyphs);
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_مجموعه_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    hb_مجموعه_t intersection;
    (هذا+coverage).intersect_مجموعه (glyphset, intersection);

    بیعلامت new_format = مقدار_Format_2;

    إذا (c->plan->normalized_coords)
    {
      new_format = مقدار_Format_2.get_effective_format (values.arrayZ, خطأ, خطأ, هذا, &c->plan->layout_variation_idx_delta_map);
    }
    /* do not strip hints for VF */
    وإلا إذا (c->plan->flags & HB_SUBSET_FLAGS_NO_HINTING)
    {
      hb_blob_t* blob = hb_face_reference_جدول (c->plan->منبع, HB_برچسب ('f','v','a','r'));
      منطقي has_fvar = (blob != hb_blob_get_خالی ());
      hb_blob_destroy (blob);

      منطقي strip = !has_fvar;
      /* special case: strip hints when a VF has no GDEF varstore after
       * subsetting*/
      إذا (has_fvar && !c->plan->has_gdef_varstore)
        strip = درست;
      new_format = مقدار_Format_2.get_effective_format (values.arrayZ,
                                                     strip, /* strip hints */
                                                     درست, /* strip empty */
                                                     هذا, اشاره_تهی);
    }

    خودکار it =
    + hb_iter (intersection)
    | hb_map_retains_ترتیبدهی (glyph_map)
    | hb_zip (hb_repeat (values.as_array (مقدار_Format_2.get_len ())))
    ;

    منطقي ret = منطقي (it);
    SinglePos_serialize (c->serializer, هذا, مقدار_Format_2, it, &c->plan->layout_variation_idx_delta_map, new_format);
    return_trace (ret);
  }
};

}
}
}

#پایان_اگر /* OT_LAYOUT_GPOS_SINGLEPOSFORMAT1_HH */
