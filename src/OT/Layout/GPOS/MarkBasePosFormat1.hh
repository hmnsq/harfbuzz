#جے_بنا_تعریف OT_LAYOUT_GPOS_MARKBASEPOSFORMAT1_HH
#تعریف_کرو OT_LAYOUT_GPOS_MARKBASEPOSFORMAT1_HH

#ضمن "MarkArray.hh"

نطاق OT {
نطاق Layout {
نطاق GPOS_impl {

سانچا <قسم_ناں Types>
بنية Mark_ادھار_Pos_Format1_2
{
  محمي:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  قسم_ناں Types::سانچا LOffset_منزل<Coverage>
                markCoverage;           /* Offset to MarkCoverage table--from
                                         * beginning of MarkBasePos subtable */
  قسم_ناں Types::سانچا LOffset_منزل<Coverage>
                ادھار_Coverage;           /* Offset to BaseCoverage table--from
                                         * beginning of MarkBasePos subtable */
  HBUINT16      class_گنتی_2;             /* Number of classes defined for marks */
  قسم_ناں Types::سانچا LOffset_منزل<MarkArray<Types>>
                markArray;              /* Offset to MarkArray table--from
                                         * beginning of MarkBasePos subtable */
  قسم_ناں Types::سانچا LOffset_منزل<Anchor_میٹرکس<Types>>
                ادھار_Array;              /* Offset to BaseArray table--from
                                         * beginning of MarkBasePos subtable */

  عام:
  DEFINE_ناپ_STATIC (4 + 4 * Types::LOffset::static_ناپ);

    منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
                  markCoverage.sanitize (c, هذا) &&
                  ادھار_Coverage.sanitize (c, هذا) &&
                  markArray.sanitize (c, هذا) &&
                  ادھار_Array.sanitize (c, هذا, (بنانشانی صحيح) class_گنتی_2));
  }

  منطقي intersects (ثابت hb_سیٹ_t *glyphs) ثابت
  {
    أرجع (هذا+markCoverage).intersects (glyphs) &&
           (هذا+ادھار_Coverage).intersects (glyphs);
  }

  فراغ گھیرا_lookups (hb_گھیرا_lookups_context_t *c) ثابت {}

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    (هذا+markArray).collect_variation_indices (c, هذا+markCoverage);

    hb_map_t klass_mapping;
    Markclass_گھیرا_and_remap_indexes (هذا+markCoverage, هذا+markArray, *c->glyph_سیٹ, &klass_mapping);

    hb_sorted_سمتیہ_t<بنانشانی> ادھار_indexes;
    collect_anchor_indexes (هذا+ادھار_Coverage, (هذا+ادھار_Array).صفاں,
                            class_گنتی_2, *c->glyph_سیٹ, klass_mapping,
                            ادھار_indexes);
    (هذا+ادھار_Array).collect_variation_indices (c, ادھار_indexes.iter ());
  }

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    إذا (unlikely (!(هذا+markCoverage).collect_coverage (c->داخلہ))) أرجع;
    إذا (unlikely (!(هذا+ادھار_Coverage).collect_coverage (c->داخلہ))) أرجع;
  }

  فراغ collect_دوجا_glyphs (hb_سیٹ_digest_t *digest) ثابت
  { (هذا+ادھار_Coverage).collect_coverage (digest); }

  ثابت Coverage &get_coverage () ثابت { أرجع هذا+markCoverage; }

  ساكن مضمن منطقي accept (hb_بفر_t *بفر, بنانشانی idx)
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
    بنانشانی صحيح mark_اشاریہ = (هذا+markCoverage).get_coverage  (بفر->cur().codepoint);
    إذا (likely (mark_اشاریہ == NOT_COVERED)) return_trace (خطأ);

    /* Now we search backwards for a non-mark glyph.
     * We don't use skippy_iter.prev() to avoid O(n^2) behavior. */

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
	    NOT_COVERED == (هذا+ادھار_Coverage).get_coverage  (بفر->info[j - 1].codepoint))
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

    /* Checking that matched glyph is actually a base glyph by GDEF is too strong; disabled */
    //if (!_hb_glyph_info_is_base_glyph (&buffer->info[idx])) { return_trace (false); }

    بنانشانی صحيح ادھار_اشاریہ = (هذا+ادھار_Coverage).get_coverage  (بفر->info[idx].codepoint);
    إذا (ادھار_اشاریہ == NOT_COVERED)
    {
      بفر->unsafe_منزل_concat_from_outbuffer (idx, بفر->idx + 1);
      return_trace (خطأ);
    }

    return_trace ((هذا+markArray).apply (c, mark_اشاریہ, ادھار_اشاریہ, هذا+ادھار_Array, class_گنتی_2, idx));
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_سیٹ_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    خودکار *باہرلا = c->serializer->شروع_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (باہرلا))) return_trace (خطأ);
    باہرلا->format = format;

    hb_map_t klass_mapping;
    Markclass_گھیرا_and_remap_indexes (هذا+markCoverage, هذا+markArray, glyphset, &klass_mapping);

    إذا (!klass_mapping.get_population ()) return_trace (خطأ);
    باہرلا->class_گنتی_2 = klass_mapping.get_population ();

    خودکار mark_iter =
    + hb_zip (هذا+markCoverage, هذا+markArray)
    | hb_filter (glyphset, hb_پہلا)
    ;

    hb_sorted_سمتیہ_t<hb_codepoint_t> new_coverage;
    + mark_iter
    | hb_map (hb_پہلا)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    إذا (!باہرلا->markCoverage.serialize_serialize (c->serializer, new_coverage.iter ()))
      return_trace (خطأ);

    إذا (unlikely (!باہرلا->markArray.serialize_subset (c, markArray, هذا,
						    (هذا+markCoverage).iter (),
						    &klass_mapping)))
      return_trace (خطأ);

    new_coverage.reset ();
    hb_sorted_سمتیہ_t<بنانشانی> ادھار_indexes;
    ثابت خودکار &ادھار_array = هذا+ادھار_Array;
    collect_subset_anchor_indexes (هذا+ادھار_Coverage, ادھار_array.صفاں,
                                   class_گنتی_2, ادھار_array.میٹرکس_Z.arrayZ,
                                   glyphset, glyph_map, klass_mapping,
                                   new_coverage, ادھار_indexes);

    إذا (!new_coverage) return_trace (خطأ);
    إذا (!باہرلا->ادھار_Coverage.serialize_serialize (c->serializer, new_coverage.iter ()))
      return_trace (خطأ);

    return_trace (باہرلا->ادھار_Array.serialize_subset (c, ادھار_Array, هذا,
						   new_coverage.لمائی,
						   ادھار_indexes.iter ()));
  }
};


}
}
}

#جے_انت /* OT_LAYOUT_GPOS_MARKBASEPOSFORMAT1_HH */
