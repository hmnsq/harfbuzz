#که_نه_تعریف_شوی OT_LAYOUT_GPOS_MARKBASEPOSFORMAT1_HH
#تعریف_کړه OT_LAYOUT_GPOS_MARKBASEPOSFORMAT1_HH

#ضمن "MarkArray.hh"

نطاق OT {
نطاق Layout {
نطاق GPOS_impl {

کينډۍ <ډول_نوم Types>
بنية Mark_بنسټ_Pos_Format1_2
{
  محمي:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  ډول_نوم Types::کينډۍ LOffset_منزل<Coverage>
                markCoverage;           /* Offset to MarkCoverage table--from
                                         * beginning of MarkBasePos subtable */
  ډول_نوم Types::کينډۍ LOffset_منزل<Coverage>
                بنسټ_Coverage;           /* Offset to BaseCoverage table--from
                                         * beginning of MarkBasePos subtable */
  HBUINT16      class_شمېر_2;             /* Number of classes defined for marks */
  ډول_نوم Types::کينډۍ LOffset_منزل<MarkArray<Types>>
                markArray;              /* Offset to MarkArray table--from
                                         * beginning of MarkBasePos subtable */
  ډول_نوم Types::کينډۍ LOffset_منزل<Anchor_ماتریکس<Types>>
                بنسټ_Array;              /* Offset to BaseArray table--from
                                         * beginning of MarkBasePos subtable */

  عام:
  DEFINE_اندازه_STATIC (4 + 4 * Types::LOffset::static_اندازه);

    منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
                  markCoverage.sanitize (c, هذا) &&
                  بنسټ_Coverage.sanitize (c, هذا) &&
                  markArray.sanitize (c, هذا) &&
                  بنسټ_Array.sanitize (c, هذا, (بینښه صحيح) class_شمېر_2));
  }

  منطقي intersects (ثابت hb_سېټ_t *glyphs) ثابت
  {
    أرجع (هذا+markCoverage).intersects (glyphs) &&
           (هذا+بنسټ_Coverage).intersects (glyphs);
  }

  فراغ تړون_lookups (hb_تړون_lookups_context_t *c) ثابت {}

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    (هذا+markArray).collect_variation_indices (c, هذا+markCoverage);

    hb_map_t klass_mapping;
    Markclass_تړون_and_remap_indexes (هذا+markCoverage, هذا+markArray, *c->glyph_سېټ, &klass_mapping);

    hb_sorted_ویکټور_t<بینښه> بنسټ_indexes;
    collect_anchor_indexes (هذا+بنسټ_Coverage, (هذا+بنسټ_Array).لیکې,
                            class_شمېر_2, *c->glyph_سېټ, klass_mapping,
                            بنسټ_indexes);
    (هذا+بنسټ_Array).collect_variation_indices (c, بنسټ_indexes.iter ());
  }

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    إذا (unlikely (!(هذا+markCoverage).collect_coverage (c->ننوت))) أرجع;
    إذا (unlikely (!(هذا+بنسټ_Coverage).collect_coverage (c->ننوت))) أرجع;
  }

  فراغ collect_دوهم_glyphs (hb_سېټ_digest_t *digest) ثابت
  { (هذا+بنسټ_Coverage).collect_coverage (digest); }

  ثابت Coverage &get_coverage () ثابت { أرجع هذا+markCoverage; }

  ساكن مضمن منطقي accept (hb_بفر_t *بفر, بینښه idx)
  {
    /* We only want to attach to the first of a MultipleSubst sequence.
     * https://github.com/harfbuzz/harfbuzz/issues/740
     * Reject others...
     * ...but stop if we find a mark in the MultipleSubst sequence:
     * https://github.com/harfbuzz/harfbuzz/issues/1020 */
    أرجع !_hb_glyph_info_multiplied (&بفر->info[idx]) ||
	   0 == _hb_glyph_info_get_lig_comp (&بفر->info[idx]) ||
	   (idx == 0 ||
	    _hb_glyph_info_is_mark (&بفر->info[idx - 1]) ||
	    !_hb_glyph_info_multiplied (&بفر->info[idx - 1]) ||
	    _hb_glyph_info_get_lig_id (&بفر->info[idx]) !=
	    _hb_glyph_info_get_lig_id (&بفر->info[idx - 1]) ||
	    _hb_glyph_info_get_lig_comp (&بفر->info[idx]) !=
	    _hb_glyph_info_get_lig_comp (&بفر->info[idx - 1]) + 1
	    );
  }

  منطقي apply (hb_ot_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);
    hb_بفر_t *بفر = c->بفر;
    بینښه صحيح mark_شاخص = (هذا+markCoverage).get_coverage  (بفر->cur().codepoint);
    إذا (likely (mark_شاخص == NOT_COVERED)) return_trace (خطأ);

    /* Now we search backwards for a non-mark glyph.
     * We don't use skippy_iter.prev() to avoid O(n^2) behavior. */

    خپلکاری &skippy_iter = c->iter_ننوت;
    skippy_iter.سېټ_lookup_props (LookupFlag::IgnoreMarks);

    إذا (c->وروستی_بنسټ_until > بفر->idx)
    {
      c->وروستی_بنسټ_until = 0;
      c->وروستی_بنسټ = -1;
    }
    بینښه j;
    لكل (j = بفر->idx; j > c->وروستی_بنسټ_until; j--)
    {
      خپلکاری سمون = skippy_iter.سمون (بفر->info[j - 1]);
      إذا (سمون == skippy_iter.سمون_2)
      {
        // https://github.com/harfbuzz/harfbuzz/issues/4124
	إذا (!accept (بفر, j - 1) &&
	    NOT_COVERED == (هذا+بنسټ_Coverage).get_coverage  (بفر->info[j - 1].codepoint))
	  سمون = skippy_iter.SKIP;
      }
      إذا (سمون == skippy_iter.سمون_2)
      {
	c->وروستی_بنسټ = (نښهدار) j - 1;
	اكسر;
      }
    }
    c->وروستی_بنسټ_until = بفر->idx;
    إذا (c->وروستی_بنسټ == -1)
    {
      بفر->unsafe_منزل_concat_from_outbuffer (0, بفر->idx + 1);
      return_trace (خطأ);
    }

    بینښه idx = (بینښه) c->وروستی_بنسټ;

    /* Checking that matched glyph is actually a base glyph by GDEF is too strong; disabled */
    //if (!_hb_glyph_info_is_base_glyph (&buffer->info[idx])) { return_trace (false); }

    بینښه صحيح بنسټ_شاخص = (هذا+بنسټ_Coverage).get_coverage  (بفر->info[idx].codepoint);
    إذا (بنسټ_شاخص == NOT_COVERED)
    {
      بفر->unsafe_منزل_concat_from_outbuffer (idx, بفر->idx + 1);
      return_trace (خطأ);
    }

    return_trace ((هذا+markArray).apply (c, mark_شاخص, بنسټ_شاخص, هذا+بنسټ_Array, class_شمېر_2, idx));
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_سېټ_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    خپلکاری *بهر = c->serializer->پیل_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (بهر))) return_trace (خطأ);
    بهر->format = format;

    hb_map_t klass_mapping;
    Markclass_تړون_and_remap_indexes (هذا+markCoverage, هذا+markArray, glyphset, &klass_mapping);

    إذا (!klass_mapping.get_population ()) return_trace (خطأ);
    بهر->class_شمېر_2 = klass_mapping.get_population ();

    خپلکاری mark_iter =
    + hb_zip (هذا+markCoverage, هذا+markArray)
    | hb_filter (glyphset, hb_لومړی)
    ;

    hb_sorted_ویکټور_t<hb_codepoint_t> new_coverage;
    + mark_iter
    | hb_map (hb_لومړی)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    إذا (!بهر->markCoverage.serialize_serialize (c->serializer, new_coverage.iter ()))
      return_trace (خطأ);

    إذا (unlikely (!بهر->markArray.serialize_subset (c, markArray, هذا,
						    (هذا+markCoverage).iter (),
						    &klass_mapping)))
      return_trace (خطأ);

    new_coverage.reset ();
    hb_sorted_ویکټور_t<بینښه> بنسټ_indexes;
    ثابت خپلکاری &بنسټ_array = هذا+بنسټ_Array;
    collect_subset_anchor_indexes (هذا+بنسټ_Coverage, بنسټ_array.لیکې,
                                   class_شمېر_2, بنسټ_array.ماتریکس_Z.arrayZ,
                                   glyphset, glyph_map, klass_mapping,
                                   new_coverage, بنسټ_indexes);

    إذا (!new_coverage) return_trace (خطأ);
    إذا (!بهر->بنسټ_Coverage.serialize_serialize (c->serializer, new_coverage.iter ()))
      return_trace (خطأ);

    return_trace (بهر->بنسټ_Array.serialize_subset (c, بنسټ_Array, هذا,
						   new_coverage.اوږدتیا,
						   بنسټ_indexes.iter ()));
  }
};


}
}
}

#د_که_پای /* OT_LAYOUT_GPOS_MARKBASEPOSFORMAT1_HH */
