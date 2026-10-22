#اگر_تعریف_نشده OT_LAYOUT_GPOS_MARKBASEPOSFORMAT1_HH
#تعریف_کن OT_LAYOUT_GPOS_MARKBASEPOSFORMAT1_HH

#ضمن "MarkArray.hh"

نطاق OT {
نطاق Layout {
نطاق GPOS_impl {

قالب <نام_نوع Types>
بنية Mark_پایه_Pos_Format1_2
{
  محمي:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  نام_نوع Types::قالب LOffset_مقصد<Coverage>
                markCoverage;           /* Offset to MarkCoverage table--from
                                         * beginning of MarkBasePos subtable */
  نام_نوع Types::قالب LOffset_مقصد<Coverage>
                پایه_Coverage;           /* Offset to BaseCoverage table--from
                                         * beginning of MarkBasePos subtable */
  HBUINT16      class_شمار_2;             /* Number of classes defined for marks */
  نام_نوع Types::قالب LOffset_مقصد<MarkArray<Types>>
                markArray;              /* Offset to MarkArray table--from
                                         * beginning of MarkBasePos subtable */
  نام_نوع Types::قالب LOffset_مقصد<Anchor_ماتریس<Types>>
                پایه_Array;              /* Offset to BaseArray table--from
                                         * beginning of MarkBasePos subtable */

  عام:
  DEFINE_اندازه_STATIC (4 + 4 * Types::LOffset::static_اندازه);

    منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
                  markCoverage.sanitize (c, هذا) &&
                  پایه_Coverage.sanitize (c, هذا) &&
                  markArray.sanitize (c, هذا) &&
                  پایه_Array.sanitize (c, هذا, (بیعلامت صحيح) class_شمار_2));
  }

  منطقي intersects (ثابت hb_مجموعه_t *glyphs) ثابت
  {
    أرجع (هذا+markCoverage).intersects (glyphs) &&
           (هذا+پایه_Coverage).intersects (glyphs);
  }

  فراغ بستگی_lookups (hb_بستگی_lookups_context_t *c) ثابت {}

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    (هذا+markArray).collect_variation_indices (c, هذا+markCoverage);

    hb_map_t klass_mapping;
    Markclass_بستگی_and_remap_indexes (هذا+markCoverage, هذا+markArray, *c->glyph_مجموعه, &klass_mapping);

    hb_sorted_وکتور_t<بیعلامت> پایه_indexes;
    collect_anchor_indexes (هذا+پایه_Coverage, (هذا+پایه_Array).ردیفها,
                            class_شمار_2, *c->glyph_مجموعه, klass_mapping,
                            پایه_indexes);
    (هذا+پایه_Array).collect_variation_indices (c, پایه_indexes.iter ());
  }

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    إذا (unlikely (!(هذا+markCoverage).collect_coverage (c->درون_داد))) أرجع;
    إذا (unlikely (!(هذا+پایه_Coverage).collect_coverage (c->درون_داد))) أرجع;
  }

  فراغ collect_دومی_glyphs (hb_مجموعه_digest_t *digest) ثابت
  { (هذا+پایه_Coverage).collect_coverage (digest); }

  ثابت Coverage &get_coverage () ثابت { أرجع هذا+markCoverage; }

  ساكن مضمن منطقي accept (hb_میانگیر_t *میانگیر, بیعلامت idx)
  {
    /* We only want to attach to the first of a MultipleSubst sequence.
     * https://github.com/harfbuzz/harfbuzz/issues/740
     * Reject others...
     * ...but stop if we find a mark in the MultipleSubst sequence:
     * https://github.com/harfbuzz/harfbuzz/issues/1020 */
    أرجع !_hb_glyph_info_multiplied (&میانگیر->info[idx]) ||
	   0 == _hb_glyph_info_get_lig_comp (&میانگیر->info[idx]) ||
	   (idx == 0 ||
	    _hb_glyph_info_is_mark (&میانگیر->info[idx - 1]) ||
	    !_hb_glyph_info_multiplied (&میانگیر->info[idx - 1]) ||
	    _hb_glyph_info_get_lig_id (&میانگیر->info[idx]) !=
	    _hb_glyph_info_get_lig_id (&میانگیر->info[idx - 1]) ||
	    _hb_glyph_info_get_lig_comp (&میانگیر->info[idx]) !=
	    _hb_glyph_info_get_lig_comp (&میانگیر->info[idx - 1]) + 1
	    );
  }

  منطقي apply (hb_ot_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);
    hb_میانگیر_t *میانگیر = c->میانگیر;
    بیعلامت صحيح mark_شاخص = (هذا+markCoverage).get_coverage  (میانگیر->cur().codepoint);
    إذا (likely (mark_شاخص == NOT_COVERED)) return_trace (خطأ);

    /* Now we search backwards for a non-mark glyph.
     * We don't use skippy_iter.prev() to avoid O(n^2) behavior. */

    خودکار &skippy_iter = c->iter_درون_داد;
    skippy_iter.مجموعه_lookup_props (LookupFlag::IgnoreMarks);

    إذا (c->آخرین_پایه_until > میانگیر->idx)
    {
      c->آخرین_پایه_until = 0;
      c->آخرین_پایه = -1;
    }
    بیعلامت j;
    لكل (j = میانگیر->idx; j > c->آخرین_پایه_until; j--)
    {
      خودکار تطابق = skippy_iter.تطابق (میانگیر->info[j - 1]);
      إذا (تطابق == skippy_iter.تطابق_2)
      {
        // https://github.com/harfbuzz/harfbuzz/issues/4124
	إذا (!accept (میانگیر, j - 1) &&
	    NOT_COVERED == (هذا+پایه_Coverage).get_coverage  (میانگیر->info[j - 1].codepoint))
	  تطابق = skippy_iter.SKIP;
      }
      إذا (تطابق == skippy_iter.تطابق_2)
      {
	c->آخرین_پایه = (علامتدار) j - 1;
	اكسر;
      }
    }
    c->آخرین_پایه_until = میانگیر->idx;
    إذا (c->آخرین_پایه == -1)
    {
      میانگیر->unsafe_مقصد_concat_from_outbuffer (0, میانگیر->idx + 1);
      return_trace (خطأ);
    }

    بیعلامت idx = (بیعلامت) c->آخرین_پایه;

    /* Checking that matched glyph is actually a base glyph by GDEF is too strong; disabled */
    //if (!_hb_glyph_info_is_base_glyph (&buffer->info[idx])) { return_trace (false); }

    بیعلامت صحيح پایه_شاخص = (هذا+پایه_Coverage).get_coverage  (میانگیر->info[idx].codepoint);
    إذا (پایه_شاخص == NOT_COVERED)
    {
      میانگیر->unsafe_مقصد_concat_from_outbuffer (idx, میانگیر->idx + 1);
      return_trace (خطأ);
    }

    return_trace ((هذا+markArray).apply (c, mark_شاخص, پایه_شاخص, هذا+پایه_Array, class_شمار_2, idx));
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_مجموعه_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    خودکار *بیرون = c->serializer->شروع_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (بیرون))) return_trace (خطأ);
    بیرون->format = format;

    hb_map_t klass_mapping;
    Markclass_بستگی_and_remap_indexes (هذا+markCoverage, هذا+markArray, glyphset, &klass_mapping);

    إذا (!klass_mapping.get_population ()) return_trace (خطأ);
    بیرون->class_شمار_2 = klass_mapping.get_population ();

    خودکار mark_iter =
    + hb_zip (هذا+markCoverage, هذا+markArray)
    | hb_filter (glyphset, hb_اولی)
    ;

    hb_sorted_وکتور_t<hb_codepoint_t> new_coverage;
    + mark_iter
    | hb_map (hb_اولی)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    إذا (!بیرون->markCoverage.serialize_serialize (c->serializer, new_coverage.iter ()))
      return_trace (خطأ);

    إذا (unlikely (!بیرون->markArray.serialize_subset (c, markArray, هذا,
						    (هذا+markCoverage).iter (),
						    &klass_mapping)))
      return_trace (خطأ);

    new_coverage.reset ();
    hb_sorted_وکتور_t<بیعلامت> پایه_indexes;
    ثابت خودکار &پایه_array = هذا+پایه_Array;
    collect_subset_anchor_indexes (هذا+پایه_Coverage, پایه_array.ردیفها,
                                   class_شمار_2, پایه_array.ماتریس_Z.arrayZ,
                                   glyphset, glyph_map, klass_mapping,
                                   new_coverage, پایه_indexes);

    إذا (!new_coverage) return_trace (خطأ);
    إذا (!بیرون->پایه_Coverage.serialize_serialize (c->serializer, new_coverage.iter ()))
      return_trace (خطأ);

    return_trace (بیرون->پایه_Array.serialize_subset (c, پایه_Array, هذا,
						   new_coverage.درازی,
						   پایه_indexes.iter ()));
  }
};


}
}
}

#پایان_اگر /* OT_LAYOUT_GPOS_MARKBASEPOSFORMAT1_HH */
