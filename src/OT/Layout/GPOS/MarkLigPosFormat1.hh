#اگر_تعریف_نشده OT_LAYOUT_GPOS_MARKLIGPOSFORMAT1_HH
#تعریف_کن OT_LAYOUT_GPOS_MARKLIGPOSFORMAT1_HH

#ضمن "LigatureArray.hh"

نطاق OT {
نطاق Layout {
نطاق GPOS_impl {


قالب <نام_نوع Types>
بنية MarkLigPosFormat1_2
{
  محمي:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  نام_نوع Types::قالب LOffset_مقصد<Coverage>
                markCoverage;           /* Offset to Mark Coverage table--from
                                         * beginning of MarkLigPos subtable */
  نام_نوع Types::قالب LOffset_مقصد<Coverage>
                ligatureCoverage;       /* Offset to Ligature Coverage
                                         * table--from beginning of MarkLigPos
                                         * subtable */
  HBUINT16      class_شمار_2;             /* Number of defined mark classes */
  نام_نوع Types::قالب LOffset_مقصد<MarkArray<Types>>
                markArray;              /* Offset to MarkArray table--from
                                         * beginning of MarkLigPos subtable */
  نام_نوع Types::قالب LOffset_مقصد<LigatureArray<Types>>
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
                  ligatureArray.sanitize (c, هذا, (بیعلامت صحيح) class_شمار_2));
  }

  منطقي intersects (ثابت hb_مجموعه_t *glyphs) ثابت
  {
    أرجع (هذا+markCoverage).intersects (glyphs) &&
           (هذا+ligatureCoverage).intersects (glyphs);
  }

  فراغ بستگی_lookups (hb_بستگی_lookups_context_t *c) ثابت {}

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    (هذا+markArray).collect_variation_indices (c, هذا+markCoverage);

    hb_map_t klass_mapping;
    Markclass_بستگی_and_remap_indexes (هذا+markCoverage, هذا+markArray, *c->glyph_مجموعه, &klass_mapping);

    (هذا+ligatureArray).collect_variation_indices (c, هذا+ligatureCoverage,
                                                   class_شمار_2, klass_mapping);
  }

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    إذا (unlikely (!(هذا+markCoverage).collect_coverage (c->درون_داد))) أرجع;
    إذا (unlikely (!(هذا+ligatureCoverage).collect_coverage (c->درون_داد))) أرجع;
  }

  فراغ collect_دومی_glyphs (hb_مجموعه_digest_t *digest) ثابت
  { (هذا+ligatureCoverage).collect_coverage (digest); }

  ثابت Coverage &get_coverage () ثابت { أرجع هذا+markCoverage; }

  ساكن مضمن منطقي accept (hb_میانگیر_t *میانگیر, بیعلامت idx)
  {
    /* We only want to attach to the first of a MultipleSubst sequence,
     * which might have been ligated into a preceding ligature, and in that
     * case the mark should attach to that ligature.
     * https://github.com/harfbuzz/harfbuzz/issues/4969
     * Reject others... */
    أرجع !_hb_glyph_info_multiplied (&میانگیر->info[idx]) ||
	   0 == _hb_glyph_info_get_lig_comp (&میانگیر->info[idx]);
  }

  منطقي apply (hb_ot_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);
    hb_میانگیر_t *میانگیر = c->میانگیر;
    بیعلامت صحيح mark_شاخص = (هذا+markCoverage).get_coverage  (میانگیر->cur().codepoint);
    إذا (likely (mark_شاخص == NOT_COVERED)) return_trace (خطأ);

    /* Now we search backwards for a non-mark glyph */

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
	    NOT_COVERED == (هذا+ligatureCoverage).get_coverage  (میانگیر->info[j - 1].codepoint))
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

    /* Checking that matched glyph is actually a ligature by GDEF is too strong; disabled */
    //if (!_hb_glyph_info_is_ligature (&buffer->info[idx])) { return_trace (false); }

    بیعلامت صحيح lig_شاخص = (هذا+ligatureCoverage).get_coverage  (میانگیر->info[idx].codepoint);
    إذا (lig_شاخص == NOT_COVERED)
    {
      میانگیر->unsafe_مقصد_concat_from_outbuffer (idx, میانگیر->idx + 1);
      return_trace (خطأ);
    }

    ثابت LigatureArray<Types>& lig_array = هذا+ligatureArray;
    ثابت Anchor_ماتریس<Types, HBUINT16>& lig_attach = lig_array[lig_شاخص];

    /* Find component to attach to */
    بیعلامت صحيح comp_شمار = lig_attach.ردیفها;
    إذا (unlikely (!comp_شمار))
    {
      میانگیر->unsafe_مقصد_concat_from_outbuffer (idx, میانگیر->idx + 1);
      return_trace (خطأ);
    }

    /* We must now check whether the ligature ID of the current mark glyph
     * is identical to the ligature ID of the found ligature.  If yes, we
     * can directly use the component index.  If not, we attach the mark
     * glyph to the last component of the ligature. */
    بیعلامت صحيح comp_شاخص;
    بیعلامت صحيح lig_id = _hb_glyph_info_get_lig_id (&میانگیر->info[idx]);
    بیعلامت صحيح mark_id = _hb_glyph_info_get_lig_id (&میانگیر->cur());
    بیعلامت صحيح mark_comp = _hb_glyph_info_get_lig_comp (&میانگیر->cur());
    إذا (lig_id && lig_id == mark_id && mark_comp > 0)
      comp_شاخص = hb_min (comp_شمار, _hb_glyph_info_get_lig_comp (&میانگیر->cur())) - 1;
    وإلا
      comp_شاخص = comp_شمار - 1;

    return_trace ((هذا+markArray).apply (c, mark_شاخص, comp_شاخص, lig_attach, class_شمار_2, idx));
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_مجموعه_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = c->plan->glyph_map_gsub;

    خودکار *بیرون = c->serializer->شروع_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (بیرون))) return_trace (خطأ);
    بیرون->format = format;

    hb_map_t klass_mapping;
    Markclass_بستگی_and_remap_indexes (هذا+markCoverage, هذا+markArray, glyphset, &klass_mapping);

    إذا (!klass_mapping.get_population ()) return_trace (خطأ);
    بیرون->class_شمار_2 = klass_mapping.get_population ();

    خودکار mark_iter =
    + hb_zip (هذا+markCoverage, hb_range ((بیعلامت) (هذا+markArray).len))
    | hb_filter (glyphset, hb_اولی)
    ;

    خودکار new_mark_coverage =
    + mark_iter
    | hb_map_retains_ترتیبدهی (hb_اولی)
    | hb_map_retains_ترتیبدهی (glyph_map)
    ;

    إذا (!بیرون->markCoverage.serialize_serialize (c->serializer, new_mark_coverage))
      return_trace (خطأ);

    إذا (unlikely (!بیرون->markArray.serialize_subset (c, markArray, هذا,
						    (هذا+markCoverage).iter (),
						    &klass_mapping)))
      return_trace (خطأ);

    hb_sorted_وکتور_t<hb_codepoint_t> new_lig_coverage;
    إذا (!بیرون->ligatureArray.serialize_subset (c, ligatureArray, هذا,
					      hb_iter (هذا+ligatureCoverage),
					      class_شمار_2, &klass_mapping, new_lig_coverage))
      return_trace (خطأ);

    return_trace (بیرون->ligatureCoverage.serialize_serialize (c->serializer, new_lig_coverage.iter ()));
  }

};

}
}
}

#پایان_اگر /* OT_LAYOUT_GPOS_MARKLIGPOSFORMAT1_HH */
