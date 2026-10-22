#که_نه_تعریف_شوی OT_LAYOUT_GPOS_SINGLEPOSFORMAT1_HH
#تعریف_کړه OT_LAYOUT_GPOS_SINGLEPOSFORMAT1_HH

#ضمن "Common.hh"
#ضمن "ValueFormat.hh"

نطاق OT {
نطاق Layout {
نطاق GPOS_impl {

کينډۍ <ډول_نوم Types>
بنية SinglePosFormat1_3 : ارزښت_بنسټ
{
  محمي:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  ډول_نوم Types::کينډۍ LOffset_منزل<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of subtable */
  ارزښت_Format   ارزښت_Format_2;            /* Defines the types of data in the
                                         * ValueRecord */
  ارزښت_ریکارډ   values;                 /* Defines positioning
                                         * value(s)--applied to all glyphs in
                                         * the Coverage table */
  عام:
  DEFINE_اندازه_ARRAY (2 + Types::LOffset::static_اندازه + ارزښت_Format::static_اندازه, values);

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
                  ارزښت_Format_2.sanitize_ارزښت (c, هذا, values));

  }

  منطقي intersects (ثابت hb_سېټ_t *glyphs) ثابت
  { أرجع (هذا+coverage).intersects (glyphs); }

  فراغ تړون_lookups (hb_تړون_lookups_context_t *c) ثابت {}
  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    إذا (!ارزښت_Format_2.has_device ()) أرجع;

    hb_سېټ_t intersection;
    (هذا+coverage).intersect_سېټ (*c->glyph_سېټ, intersection);
    إذا (!intersection) أرجع;

    ارزښت_Format_2.collect_variation_indices (c, هذا, values.as_array (ارزښت_Format_2.get_len ()));
  }

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  { إذا (unlikely (!(هذا+coverage).collect_coverage (c->ننوت))) أرجع; }

  ثابت Coverage &get_coverage () ثابت { أرجع هذا+coverage; }

  ارزښت_Format get_ارزښت_format () ثابت { أرجع ارزښت_Format_2; }

  منطقي apply (hb_ot_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);
    hb_بفر_t *بفر = c->بفر;
    بینښه صحيح شاخص_2 = (هذا+coverage).get_coverage  (بفر->cur().codepoint);
    إذا (شاخص_2 == NOT_COVERED) return_trace (خطأ);

    إذا (HB_بفر_پیغام_نور && c->بفر->messaging ())
    {
      c->بفر->پیغام (c->font,
			  "positioning glyph at %u",
			  c->بفر->idx);
    }

    ارزښت_Format_2.apply_ارزښت (c, هذا, values, بفر->cur_pos());

    إذا (HB_بفر_پیغام_نور && c->بفر->messaging ())
    {
      c->بفر->پیغام (c->font,
			  "positioned glyph at %u",
			  c->بفر->idx);
    }

    بفر->idx++;
    return_trace (ریښتیا);
  }

  منطقي
  position_single (hb_font_t           *font,
		   hb_blob_t           *جدول_blob,
		   hb_direction_t       direction,
		   hb_codepoint_t       gid,
		   hb_glyph_position_t &pos) ثابت
  {
    بینښه صحيح شاخص_2 = (هذا+coverage).get_coverage  (gid);
    إذا (likely (شاخص_2 == NOT_COVERED)) أرجع خطأ;

    ارزښت_Format_2.position_single (font, جدول_blob, direction, هذا,
                                 values, pos);
    أرجع ریښتیا;
  }

  کينډۍ<ډول_نوم Iterator,
      hb_requires (hb_is_iterator (Iterator))>
  فراغ serialize (hb_serialize_context_t *c,
                  ثابت ارزښت_بنسټ *src,
                  ارزښت_Format src_format,
                  Iterator it,
                  ارزښت_Format newFormat,
                  ثابت hb_hashmap_t<بینښه, hb_pair_t<بینښه, صحيح>> *layout_variation_idx_delta_map)
  {
    إذا (unlikely (!c->extend_min (هذا))) أرجع;
    إذا (unlikely (!c->check_ټاکل (ارزښت_Format_2,
                                    newFormat,
                                    HB_SERIALIZE_ERROR_INT_ډکېدنه))) أرجع;

    لكل (ثابت hb_array_t<ثابت ارزښت>& _ : + it | hb_map (hb_دوهم))
    {
      src_format.copy_ارزښتونه (c, newFormat, src,  &_, layout_variation_idx_delta_map);
      // Only serialize the first entry in the iterator, the rest are assumed to
      // be the same.
      اكسر;
    }

    خپلکاری glyphs =
    + it
    | hb_map_retains_ترتیبونه (hb_لومړی)
    ;

    coverage.serialize_serialize (c, glyphs);
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_سېټ_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    hb_سېټ_t intersection;
    (هذا+coverage).intersect_سېټ (glyphset, intersection);

    بینښه new_format = ارزښت_Format_2;

    إذا (c->plan->normalized_coords)
    {
      new_format = ارزښت_Format_2.get_effective_format (values.arrayZ, خطأ, خطأ, هذا, &c->plan->layout_variation_idx_delta_map);
    }
    /* do not strip hints for VF */
    وإلا إذا (c->plan->flags & HB_SUBSET_FLAGS_NO_HINTING)
    {
      hb_blob_t* blob = hb_face_reference_جدول (c->plan->سرچینه, HB_نښان ('f','v','a','r'));
      منطقي has_fvar = (blob != hb_blob_get_تشه ());
      hb_blob_destroy (blob);

      منطقي strip = !has_fvar;
      /* special case: strip hints when a VF has no GDEF varstore after
       * subsetting*/
      إذا (has_fvar && !c->plan->has_gdef_varstore)
        strip = ریښتیا;
      new_format = ارزښت_Format_2.get_effective_format (values.arrayZ,
                                                     strip, /* strip hints */
                                                     ریښتیا, /* strip empty */
                                                     هذا, تش_اشاره);
    }

    خپلکاری it =
    + hb_iter (intersection)
    | hb_map_retains_ترتیبونه (glyph_map)
    | hb_zip (hb_repeat (values.as_array (ارزښت_Format_2.get_len ())))
    ;

    منطقي ret = منطقي (it);
    SinglePos_serialize (c->serializer, هذا, ارزښت_Format_2, it, &c->plan->layout_variation_idx_delta_map, new_format);
    return_trace (ret);
  }
};

}
}
}

#د_که_پای /* OT_LAYOUT_GPOS_SINGLEPOSFORMAT1_HH */
