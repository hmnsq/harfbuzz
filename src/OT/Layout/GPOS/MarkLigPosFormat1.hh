#جے_بنا_تعریف OT_LAYOUT_GPOS_MARKLIGPOSFORMAT1_HH
#تعریف_کرو OT_LAYOUT_GPOS_MARKLIGPOSFORMAT1_HH

#ضمن "LigatureArray.hh"

نطاق OT {
نطاق Layout {
نطاق GPOS_impl {


سانچا <قسم_ناں Types>
بنية MarkLigPosFormat1_2
{
  محمي:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  قسم_ناں Types::سانچا LOffset_منزل<Coverage>
                markCoverage;           /* Offset to Mark Coverage table--from
                                         * beginning of MarkLigPos subtable */
  قسم_ناں Types::سانچا LOffset_منزل<Coverage>
                ligatureCoverage;       /* Offset to Ligature Coverage
                                         * table--from beginning of MarkLigPos
                                         * subtable */
  HBUINT16      class_گنتی_2;             /* Number of defined mark classes */
  قسم_ناں Types::سانچا LOffset_منزل<MarkArray<Types>>
                markArray;              /* Offset to MarkArray table--from
                                         * beginning of MarkLigPos subtable */
  قسم_ناں Types::سانچا LOffset_منزل<LigatureArray<Types>>
                ligatureArray;          /* Offset to LigatureArray table--from
                                         * beginning of MarkLigPos subtable */
  عام:
  DEFINE_ناپ_STATIC (4 + 4 * Types::LOffset::static_ناپ);

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
                  markCoverage.sanitize (c, هذا) &&
                  ligatureCoverage.sanitize (c, هذا) &&
                  markArray.sanitize (c, هذا) &&
                  ligatureArray.sanitize (c, هذا, (بنانشانی صحيح) class_گنتی_2));
  }

  منطقي intersects (ثابت hb_سیٹ_t *glyphs) ثابت
  {
    أرجع (هذا+markCoverage).intersects (glyphs) &&
           (هذا+ligatureCoverage).intersects (glyphs);
  }

  فراغ گھیرا_lookups (hb_گھیرا_lookups_context_t *c) ثابت {}

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    (هذا+markArray).collect_variation_indices (c, هذا+markCoverage);

    hb_map_t klass_mapping;
    Markclass_گھیرا_and_remap_indexes (هذا+markCoverage, هذا+markArray, *c->glyph_سیٹ, &klass_mapping);

    (هذا+ligatureArray).collect_variation_indices (c, هذا+ligatureCoverage,
                                                   class_گنتی_2, klass_mapping);
  }

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    إذا (unlikely (!(هذا+markCoverage).collect_coverage (c->داخلہ))) أرجع;
    إذا (unlikely (!(هذا+ligatureCoverage).collect_coverage (c->داخلہ))) أرجع;
  }

  فراغ collect_دوجا_glyphs (hb_سیٹ_digest_t *digest) ثابت
  { (هذا+ligatureCoverage).collect_coverage (digest); }

  ثابت Coverage &get_coverage () ثابت { أرجع هذا+markCoverage; }

  ساكن مضمن منطقي accept (hb_بفر_t *بفر, بنانشانی idx)
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
    بنانشانی صحيح mark_اشاریہ = (هذا+markCoverage).get_coverage  (بفر->cur().codepoint);
    إذا (likely (mark_اشاریہ == NOT_COVERED)) return_trace (خطأ);

    /* Now we search backwards for a non-mark glyph */

    خودکار &skippy_iter = c->iter_داخلہ;
    skippy_iter.سیٹ_lookup_props (LookupFlag::IgnoreMarks);

    إذا (c->آخری_ادھار_until > بفر->idx)
    {
      c->آخری_ادھار_until = 0;
      c->آخری_ادھار = -1;
    }
    بنانشانی j;
    لكل (j = بفر->idx; j > c->آخری_ادھار_until; j--)
    {
      خودکار میل = skippy_iter.میل (بفر->info[j - 1]);
      إذا (میل == skippy_iter.میل_2)
      {
        // https://github.com/harfbuzz/harfbuzz/issues/4124
	إذا (!accept (بفر, j - 1) &&
	    NOT_COVERED == (هذا+ligatureCoverage).get_coverage  (بفر->info[j - 1].codepoint))
	  میل = skippy_iter.SKIP;
      }
      إذا (میل == skippy_iter.میل_2)
      {
	c->آخری_ادھار = (نشانیوالا) j - 1;
	اكسر;
      }
    }
    c->آخری_ادھار_until = بفر->idx;
    إذا (c->آخری_ادھار == -1)
    {
      بفر->unsafe_منزل_concat_from_outbuffer (0, بفر->idx + 1);
      return_trace (خطأ);
    }

    بنانشانی idx = (بنانشانی) c->آخری_ادھار;

    /* Checking that matched glyph is actually a ligature by GDEF is too strong; disabled */
    //if (!_hb_glyph_info_is_ligature (&buffer->info[idx])) { return_trace (false); }

    بنانشانی صحيح lig_اشاریہ = (هذا+ligatureCoverage).get_coverage  (بفر->info[idx].codepoint);
    إذا (lig_اشاریہ == NOT_COVERED)
    {
      بفر->unsafe_منزل_concat_from_outbuffer (idx, بفر->idx + 1);
      return_trace (خطأ);
    }

    ثابت LigatureArray<Types>& lig_array = هذا+ligatureArray;
    ثابت Anchor_میٹرکس<Types, HBUINT16>& lig_attach = lig_array[lig_اشاریہ];

    /* Find component to attach to */
    بنانشانی صحيح comp_گنتی = lig_attach.صفاں;
    إذا (unlikely (!comp_گنتی))
    {
      بفر->unsafe_منزل_concat_from_outbuffer (idx, بفر->idx + 1);
      return_trace (خطأ);
    }

    /* We must now check whether the ligature ID of the current mark glyph
     * is identical to the ligature ID of the found ligature.  If yes, we
     * can directly use the component index.  If not, we attach the mark
     * glyph to the last component of the ligature. */
    بنانشانی صحيح comp_اشاریہ;
    بنانشانی صحيح lig_id = _hb_glyph_info_get_lig_id (&بفر->info[idx]);
    بنانشانی صحيح mark_id = _hb_glyph_info_get_lig_id (&بفر->cur());
    بنانشانی صحيح mark_comp = _hb_glyph_info_get_lig_comp (&بفر->cur());
    إذا (lig_id && lig_id == mark_id && mark_comp > 0)
      comp_اشاریہ = hb_min (comp_گنتی, _hb_glyph_info_get_lig_comp (&بفر->cur())) - 1;
    وإلا
      comp_اشاریہ = comp_گنتی - 1;

    return_trace ((هذا+markArray).apply (c, mark_اشاریہ, comp_اشاریہ, lig_attach, class_گنتی_2, idx));
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_سیٹ_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = c->plan->glyph_map_gsub;

    خودکار *باہرلا = c->serializer->شروع_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (باہرلا))) return_trace (خطأ);
    باہرلا->format = format;

    hb_map_t klass_mapping;
    Markclass_گھیرا_and_remap_indexes (هذا+markCoverage, هذا+markArray, glyphset, &klass_mapping);

    إذا (!klass_mapping.get_population ()) return_trace (خطأ);
    باہرلا->class_گنتی_2 = klass_mapping.get_population ();

    خودکار mark_iter =
    + hb_zip (هذا+markCoverage, hb_range ((بنانشانی) (هذا+markArray).len))
    | hb_filter (glyphset, hb_پہلا)
    ;

    خودکار new_mark_coverage =
    + mark_iter
    | hb_map_retains_چھانٹی (hb_پہلا)
    | hb_map_retains_چھانٹی (glyph_map)
    ;

    إذا (!باہرلا->markCoverage.serialize_serialize (c->serializer, new_mark_coverage))
      return_trace (خطأ);

    إذا (unlikely (!باہرلا->markArray.serialize_subset (c, markArray, هذا,
						    (هذا+markCoverage).iter (),
						    &klass_mapping)))
      return_trace (خطأ);

    hb_sorted_سمتیہ_t<hb_codepoint_t> new_lig_coverage;
    إذا (!باہرلا->ligatureArray.serialize_subset (c, ligatureArray, هذا,
					      hb_iter (هذا+ligatureCoverage),
					      class_گنتی_2, &klass_mapping, new_lig_coverage))
      return_trace (خطأ);

    return_trace (باہرلا->ligatureCoverage.serialize_serialize (c->serializer, new_lig_coverage.iter ()));
  }

};

}
}
}

#جے_انت /* OT_LAYOUT_GPOS_MARKLIGPOSFORMAT1_HH */
