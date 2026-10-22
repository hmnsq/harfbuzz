#که_نه_تعریف_شوی OT_LAYOUT_GPOS_SINGLEPOSFORMAT2_HH
#تعریف_کړه OT_LAYOUT_GPOS_SINGLEPOSFORMAT2_HH

#ضمن "Common.hh"

نطاق OT {
نطاق Layout {
نطاق GPOS_impl {

/* Keep the iterator type independent of the source table width so
 * the output serializers can share their instantiations. */
بنية single_pos_ارزښت_mapper_t
{
  ثابت hb_map_t &glyph_map;
  ثابت hb_array_t<ثابت ارزښت> &values;
  ثابت بینښه &sub_اوږدتیا;

  hb_pair_t<hb_codepoint_t, hb_array_t<ثابت ارزښت>>
  چلوونکی () (ثابت hb_pair_t<hb_codepoint_t, بینښه> &_) ثابت
  {
    أرجع hb_pair (glyph_map[_.لومړی],
                    values.sub_array (_.second * sub_اوږدتیا, sub_اوږدتیا));
  }
};

کينډۍ <ډول_نوم Types>
بنية SinglePosFormat2_4 : ارزښت_بنسټ
{
  محمي:
  HBUINT16      format;                 /* Format identifier--format = 2 */
  ډول_نوم Types::کينډۍ LOffset_منزل<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of subtable */
  ارزښت_Format   ارزښت_Format_2;            /* Defines the types of data in the
                                         * ValueRecord */
  ډول_نوم Types::HBUINT
                ارزښت_شمېر;             /* Number of ValueRecords */
  ارزښت_ریکارډ   values;                 /* Array of ValueRecords--positioning
                                         * values applied to glyphs */
  عام:
  DEFINE_اندازه_ARRAY (2 + Types::LOffset::static_اندازه + ارزښت_Format::static_اندازه + Types::HBUINT::static_اندازه, values);

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
                  coverage.sanitize (c, هذا) &&
                  ارزښت_Format_2.sanitize_ارزښتونه (c, هذا, values, ارزښت_شمېر));
  }

  منطقي intersects (ثابت hb_سېټ_t *glyphs) ثابت
  { أرجع (هذا+coverage).intersects (glyphs); }

  فراغ تړون_lookups (hb_تړون_lookups_context_t *c) ثابت {}
  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    إذا (!ارزښت_Format_2.has_device ()) أرجع;

    خپلکاری it =
    + hb_zip (هذا+coverage, hb_range ((بینښه) ارزښت_شمېر))
    | hb_filter (c->glyph_سېټ, hb_لومړی)
    ;

    إذا (!it) أرجع;

    بینښه sub_اوږدتیا = ارزښت_Format_2.get_len ();
    ثابت hb_array_t<ثابت ارزښت> ارزښتونه_array = values.as_array (ارزښت_شمېر * sub_اوږدتیا);

    لكل (بینښه i : + it
                      | hb_map (hb_دوهم))
      ارزښت_Format_2.collect_variation_indices (c, هذا, ارزښتونه_array.sub_array (i * sub_اوږدتیا, sub_اوږدتیا));

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

    إذا (unlikely (شاخص_2 >= ارزښت_شمېر)) return_trace (خطأ);

    إذا (HB_بفر_پیغام_نور && c->بفر->messaging ())
    {
      c->بفر->پیغام (c->font,
			  "positioning glyph at %u",
			  c->بفر->idx);
    }

    ارزښت_Format_2.apply_ارزښت (c, هذا,
                             &values[شاخص_2 * ارزښت_Format_2.get_len ()],
                             بفر->cur_pos());

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
    إذا (unlikely (شاخص_2 >= ارزښت_شمېر)) أرجع خطأ;

    ارزښت_Format_2.position_single (font, جدول_blob, direction, هذا,
                                 &values[شاخص_2 * ارزښت_Format_2.get_len ()], pos);
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
    خپلکاری بهر = c->extend_min (هذا);
    إذا (unlikely (!بهر)) أرجع;
    إذا (unlikely (!c->check_ټاکل (ارزښت_Format_2, newFormat, HB_SERIALIZE_ERROR_INT_ډکېدنه))) أرجع;
    إذا (unlikely (!c->check_ټاکل (ارزښت_شمېر, it.len (), HB_SERIALIZE_ERROR_ARRAY_ډکېدنه))) أرجع;

    + it
    | hb_map (hb_دوهم)
    | hb_apply ([&] (hb_array_t<ثابت ارزښت> _)
    { src_format.copy_ارزښتونه (c, newFormat, src, &_, layout_variation_idx_delta_map); })
    ;

    خپلکاری glyphs =
    + it
    | hb_map_retains_ترتیبونه (hb_لومړی)
    ;

    coverage.serialize_serialize (c, glyphs);
  }

  کينډۍ<ډول_نوم Iterator,
      hb_requires (hb_is_iterator (Iterator))>
  بینښه compute_effective_format (ثابت hb_face_t *face,
                                     Iterator it,
                                     منطقي is_instancing, منطقي strip_hints,
                                     منطقي has_gdef_varstore,
                                     ثابت hb_hashmap_t<بینښه, hb_pair_t<بینښه, صحيح>> *varidx_delta_map) ثابت
  {
    hb_blob_t* blob = hb_face_reference_جدول (face, HB_نښان ('f','v','a','r'));
    منطقي has_fvar = (blob != hb_blob_get_تشه ());
    hb_blob_destroy (blob);

    بینښه new_format = 0;
    إذا (is_instancing)
    {
      new_format = new_format | ارزښت_Format_2.get_effective_format (+ it | hb_map (hb_دوهم), خطأ, خطأ, هذا, varidx_delta_map);
    }
    /* do not strip hints for VF */
    وإلا إذا (strip_hints)
    {
      منطقي strip = !has_fvar;
      إذا (has_fvar && !has_gdef_varstore)
        strip = ریښتیا;
      new_format = new_format | ارزښت_Format_2.get_effective_format (+ it | hb_map (hb_دوهم), strip, ریښتیا, هذا, تش_اشاره);
    }
    وإلا
      new_format = ارزښت_Format_2;

    أرجع new_format;
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_سېټ_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    بینښه sub_اوږدتیا = ارزښت_Format_2.get_len ();
    خپلکاری ارزښتونه_array = values.as_array ((بینښه) ارزښت_شمېر * sub_اوږدتیا);

    خپلکاری it =
    + hb_zip (هذا+coverage, hb_range ((بینښه) ارزښت_شمېر))
    | hb_filter (glyphset, hb_لومړی)
    | hb_map_retains_ترتیبونه (single_pos_ارزښت_mapper_t {glyph_map,
                                                       ارزښتونه_array,
                                                       sub_اوږدتیا})
    ;

    بینښه new_format = compute_effective_format (c->plan->سرچینه, it,
                                                    منطقي (c->plan->normalized_coords),
                                                    منطقي (c->plan->flags & HB_SUBSET_FLAGS_NO_HINTING),
                                                    c->plan->has_gdef_varstore,
                                                    &c->plan->layout_variation_idx_delta_map);
    منطقي ret = منطقي (it);
    SinglePos_serialize (c->serializer, هذا, ارزښت_Format_2, it, &c->plan->layout_variation_idx_delta_map, new_format);
    return_trace (ret);
  }
};


}
}
}

#د_که_پای /* OT_LAYOUT_GPOS_SINGLEPOSFORMAT2_HH */
