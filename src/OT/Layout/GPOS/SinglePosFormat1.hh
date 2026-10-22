#جيڪڏهن_وصف_نه_ٿيل OT_LAYOUT_GPOS_SINGLEPOSFORMAT1_HH
#وصف_ڪريو OT_LAYOUT_GPOS_SINGLEPOSFORMAT1_HH

#ضمن "Common.hh"
#ضمن "ValueFormat.hh"

نطاق OT {
نطاق Layout {
نطاق GPOS_impl {

سانچو <قسم_نالو Types>
بنية SinglePosFormat1_3 : قدر_بنياد
{
  محمي:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  قسم_نالو Types::سانچو LOffset_منزل<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of subtable */
  قدر_Format   قدر_Format_2;            /* Defines the types of data in the
                                         * ValueRecord */
  قدر_رڪارڊ   values;                 /* Defines positioning
                                         * value(s)--applied to all glyphs in
                                         * the Coverage table */
  عام:
  DEFINE_سائيز_ARRAY (2 + Types::LOffset::static_سائيز + قدر_Format::static_سائيز, values);

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
                  قدر_Format_2.sanitize_قدر (c, هذا, values));

  }

  منطقي intersects (ثابت hb_سيٽ_t *glyphs) ثابت
  { أرجع (هذا+coverage).intersects (glyphs); }

  فراغ گهيرو_lookups (hb_گهيرو_lookups_context_t *c) ثابت {}
  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    إذا (!قدر_Format_2.has_device ()) أرجع;

    hb_سيٽ_t intersection;
    (هذا+coverage).intersect_سيٽ (*c->glyph_سيٽ, intersection);
    إذا (!intersection) أرجع;

    قدر_Format_2.collect_variation_indices (c, هذا, values.as_array (قدر_Format_2.get_len ()));
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

    إذا (HB_بفر_پيغام_وڌيڪ && c->بفر->messaging ())
    {
      c->بفر->پيغام (c->font,
			  "positioning glyph at %u",
			  c->بفر->idx);
    }

    قدر_Format_2.apply_قدر (c, هذا, values, بفر->cur_pos());

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

    قدر_Format_2.position_single (font, جدول_blob, direction, هذا,
                                 values, pos);
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
    إذا (unlikely (!c->extend_min (هذا))) أرجع;
    إذا (unlikely (!c->check_سونپيو (قدر_Format_2,
                                    newFormat,
                                    HB_SERIALIZE_ERROR_INT_حد_کان_مٿي))) أرجع;

    لكل (ثابت hb_array_t<ثابت قدر>& _ : + it | hb_map (hb_ٻيو))
    {
      src_format.copy_قدرون (c, newFormat, src,  &_, layout_variation_idx_delta_map);
      // Only serialize the first entry in the iterator, the rest are assumed to
      // be the same.
      اكسر;
    }

    خودڪار glyphs =
    + it
    | hb_map_retains_ترتيب_ڏيڻ (hb_پهريون)
    ;

    coverage.serialize_serialize (c, glyphs);
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_سيٽ_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    hb_سيٽ_t intersection;
    (هذا+coverage).intersect_سيٽ (glyphset, intersection);

    بنانشان new_format = قدر_Format_2;

    إذا (c->plan->normalized_coords)
    {
      new_format = قدر_Format_2.get_effective_format (values.arrayZ, خطأ, خطأ, هذا, &c->plan->layout_variation_idx_delta_map);
    }
    /* do not strip hints for VF */
    وإلا إذا (c->plan->flags & HB_SUBSET_FLAGS_NO_HINTING)
    {
      hb_blob_t* blob = hb_face_reference_جدول (c->plan->ذريعو, HB_ٽيگ ('f','v','a','r'));
      منطقي has_fvar = (blob != hb_blob_get_خالي_جايون ());
      hb_blob_destroy (blob);

      منطقي strip = !has_fvar;
      /* special case: strip hints when a VF has no GDEF varstore after
       * subsetting*/
      إذا (has_fvar && !c->plan->has_gdef_varstore)
        strip = سچ;
      new_format = قدر_Format_2.get_effective_format (values.arrayZ,
                                                     strip, /* strip hints */
                                                     سچ, /* strip empty */
                                                     هذا, خالي_اشارو);
    }

    خودڪار it =
    + hb_iter (intersection)
    | hb_map_retains_ترتيب_ڏيڻ (glyph_map)
    | hb_zip (hb_repeat (values.as_array (قدر_Format_2.get_len ())))
    ;

    منطقي ret = منطقي (it);
    SinglePos_serialize (c->serializer, هذا, قدر_Format_2, it, &c->plan->layout_variation_idx_delta_map, new_format);
    return_trace (ret);
  }
};

}
}
}

#جيڪڏهن_پورو /* OT_LAYOUT_GPOS_SINGLEPOSFORMAT1_HH */
