#که_نه_تعریف_شوی OT_LAYOUT_GPOS_MARKLIGPOSFORMAT1_HH
#تعریف_کړه OT_LAYOUT_GPOS_MARKLIGPOSFORMAT1_HH

#ضمن "LigatureArray.hh"

نطاق OT {
نطاق Layout {
نطاق GPOS_impl {


کينډۍ <ډول_نوم Types>
بنية MarkLigPosFormat1_2
{
  محمي:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  ډول_نوم Types::کينډۍ LOffset_منزل<Coverage>
                markCoverage;           /* Offset to Mark Coverage table--from
                                         * beginning of MarkLigPos subtable */
  ډول_نوم Types::کينډۍ LOffset_منزل<Coverage>
                ligatureCoverage;       /* Offset to Ligature Coverage
                                         * table--from beginning of MarkLigPos
                                         * subtable */
  HBUINT16      class_شمېر_2;             /* Number of defined mark classes */
  ډول_نوم Types::کينډۍ LOffset_منزل<MarkArray<Types>>
                markArray;              /* Offset to MarkArray table--from
                                         * beginning of MarkLigPos subtable */
  ډول_نوم Types::کينډۍ LOffset_منزل<LigatureArray<Types>>
                ligatureArray;          /* Offset to LigatureArray table--from
                                         * beginning of MarkLigPos subtable */
  عام:
  DEFINE_اندازه_STATIC (4 + 4 * Types::LOffset::static_اندازه);

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
                  markCoverage.sanitize (c, هذا) &&
                  ligatureCoverage.sanitize (c, هذا) &&
                  markArray.sanitize (c, هذا) &&
                  ligatureArray.sanitize (c, هذا, (بینښه صحيح) class_شمېر_2));
  }

  منطقي intersects (ثابت hb_سېټ_t *glyphs) ثابت
  {
    أرجع (هذا+markCoverage).intersects (glyphs) &&
           (هذا+ligatureCoverage).intersects (glyphs);
  }

  فراغ تړون_lookups (hb_تړون_lookups_context_t *c) ثابت {}

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    (هذا+markArray).collect_variation_indices (c, هذا+markCoverage);

    hb_map_t klass_mapping;
    Markclass_تړون_and_remap_indexes (هذا+markCoverage, هذا+markArray, *c->glyph_سېټ, &klass_mapping);

    (هذا+ligatureArray).collect_variation_indices (c, هذا+ligatureCoverage,
                                                   class_شمېر_2, klass_mapping);
  }

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    إذا (unlikely (!(هذا+markCoverage).collect_coverage (c->ننوت))) أرجع;
    إذا (unlikely (!(هذا+ligatureCoverage).collect_coverage (c->ننوت))) أرجع;
  }

  فراغ collect_دوهم_glyphs (hb_سېټ_digest_t *digest) ثابت
  { (هذا+ligatureCoverage).collect_coverage (digest); }

  ثابت Coverage &get_coverage () ثابت { أرجع هذا+markCoverage; }

  ساكن مضمن منطقي accept (hb_بفر_t *بفر, بینښه idx)
  {
    /* We only want to attach to the first of a MultipleSubst sequence,
     * which might have been ligated into a preceding ligature, and in that
     * case the mark should attach to that ligature.
     * https://github.com/harfbuzz/harfbuzz/issues/4969
     * Reject others... */
    أرجع !_hb_glyph_info_multiplied (&بفر->info[idx]) ||
	   0 == _hb_glyph_info_get_lig_comp (&بفر->info[idx]);
  }

  منطقي apply (hb_ot_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);
    hb_بفر_t *بفر = c->بفر;
    بینښه صحيح mark_شاخص = (هذا+markCoverage).get_coverage  (بفر->cur().codepoint);
    إذا (likely (mark_شاخص == NOT_COVERED)) return_trace (خطأ);

    /* Now we search backwards for a non-mark glyph */

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
	    NOT_COVERED == (هذا+ligatureCoverage).get_coverage  (بفر->info[j - 1].codepoint))
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

    /* Checking that matched glyph is actually a ligature by GDEF is too strong; disabled */
    //if (!_hb_glyph_info_is_ligature (&buffer->info[idx])) { return_trace (false); }

    بینښه صحيح lig_شاخص = (هذا+ligatureCoverage).get_coverage  (بفر->info[idx].codepoint);
    إذا (lig_شاخص == NOT_COVERED)
    {
      بفر->unsafe_منزل_concat_from_outbuffer (idx, بفر->idx + 1);
      return_trace (خطأ);
    }

    ثابت LigatureArray<Types>& lig_array = هذا+ligatureArray;
    ثابت Anchor_ماتریکس<Types, HBUINT16>& lig_attach = lig_array[lig_شاخص];

    /* Find component to attach to */
    بینښه صحيح comp_شمېر = lig_attach.لیکې;
    إذا (unlikely (!comp_شمېر))
    {
      بفر->unsafe_منزل_concat_from_outbuffer (idx, بفر->idx + 1);
      return_trace (خطأ);
    }

    /* We must now check whether the ligature ID of the current mark glyph
     * is identical to the ligature ID of the found ligature.  If yes, we
     * can directly use the component index.  If not, we attach the mark
     * glyph to the last component of the ligature. */
    بینښه صحيح comp_شاخص;
    بینښه صحيح lig_id = _hb_glyph_info_get_lig_id (&بفر->info[idx]);
    بینښه صحيح mark_id = _hb_glyph_info_get_lig_id (&بفر->cur());
    بینښه صحيح mark_comp = _hb_glyph_info_get_lig_comp (&بفر->cur());
    إذا (lig_id && lig_id == mark_id && mark_comp > 0)
      comp_شاخص = hb_min (comp_شمېر, _hb_glyph_info_get_lig_comp (&بفر->cur())) - 1;
    وإلا
      comp_شاخص = comp_شمېر - 1;

    return_trace ((هذا+markArray).apply (c, mark_شاخص, comp_شاخص, lig_attach, class_شمېر_2, idx));
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_سېټ_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = c->plan->glyph_map_gsub;

    خپلکاری *بهر = c->serializer->پیل_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (بهر))) return_trace (خطأ);
    بهر->format = format;

    hb_map_t klass_mapping;
    Markclass_تړون_and_remap_indexes (هذا+markCoverage, هذا+markArray, glyphset, &klass_mapping);

    إذا (!klass_mapping.get_population ()) return_trace (خطأ);
    بهر->class_شمېر_2 = klass_mapping.get_population ();

    خپلکاری mark_iter =
    + hb_zip (هذا+markCoverage, hb_range ((بینښه) (هذا+markArray).len))
    | hb_filter (glyphset, hb_لومړی)
    ;

    خپلکاری new_mark_coverage =
    + mark_iter
    | hb_map_retains_ترتیبونه (hb_لومړی)
    | hb_map_retains_ترتیبونه (glyph_map)
    ;

    إذا (!بهر->markCoverage.serialize_serialize (c->serializer, new_mark_coverage))
      return_trace (خطأ);

    إذا (unlikely (!بهر->markArray.serialize_subset (c, markArray, هذا,
						    (هذا+markCoverage).iter (),
						    &klass_mapping)))
      return_trace (خطأ);

    hb_sorted_ویکټور_t<hb_codepoint_t> new_lig_coverage;
    إذا (!بهر->ligatureArray.serialize_subset (c, ligatureArray, هذا,
					      hb_iter (هذا+ligatureCoverage),
					      class_شمېر_2, &klass_mapping, new_lig_coverage))
      return_trace (خطأ);

    return_trace (بهر->ligatureCoverage.serialize_serialize (c->serializer, new_lig_coverage.iter ()));
  }

};

}
}
}

#د_که_پای /* OT_LAYOUT_GPOS_MARKLIGPOSFORMAT1_HH */
