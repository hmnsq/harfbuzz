#ifndef OT_LAYOUT_GPOS_MARKBASEPOSFORMAT1_HH
#define OT_LAYOUT_GPOS_MARKBASEPOSFORMAT1_HH

#অন্তর্ভাব "MarkArray.hh"

নামাকাশ OT {
নামাকাশ Layout {
নামাকাশ GPOS_impl {

template <typename Types>
কাঠামো MarkBasePosFormat1_2
{
  সুরক্ষিত:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  typename Types::template LOffsetTo<Coverage>
                markCoverage;           /* Offset to MarkCoverage table--from
                                         * beginning of MarkBasePos subtable */
  typename Types::template LOffsetTo<Coverage>
                baseCoverage;           /* Offset to BaseCoverage table--from
                                         * beginning of MarkBasePos subtable */
  HBUINT16      class_গুনতি_2;             /* Number of classes defined for marks */
  typename Types::template LOffsetTo<MarkArray<Types>>
                markArray;              /* Offset to MarkArray table--from
                                         * beginning of MarkBasePos subtable */
  typename Types::template LOffsetTo<AnchorMatrix<Types>>
                baseArray;              /* Offset to BaseArray table--from
                                         * beginning of MarkBasePos subtable */

  সর্বজনীন:
  DEFINE_আকার_STATIC (4 + 4 * Types::LOffset::static_আকার);

    যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই) &&
                  markCoverage.sanitize (c, এই) &&
                  baseCoverage.sanitize (c, এই) &&
                  markArray.sanitize (c, এই) &&
                  baseArray.sanitize (c, এই, (unsigned পূর্ণসংখ্যা) class_গুনতি_2));
  }

  যৌক্তিক intersects (ধ্রুবক hb_set_t *glyphs) ধ্রুবক
  {
    ফেরত (এই+markCoverage).intersects (glyphs) &&
           (এই+baseCoverage).intersects (glyphs);
  }

  শূন্য closure_lookups (hb_closure_lookups_context_t *c) ধ্রুবক {}

  শূন্য collect_variation_indices (hb_collect_variation_indices_context_t *c) ধ্রুবক
  {
    (এই+markArray).collect_variation_indices (c, এই+markCoverage);

    hb_map_t klass_mapping;
    Markclass_closure_and_remap_indexes (এই+markCoverage, এই+markArray, *c->glyph_set, &klass_mapping);

    hb_sorted_vector_t<unsigned> base_indexes;
    collect_anchor_indexes (এই+baseCoverage, (এই+baseArray).rows,
                            class_গুনতি_2, *c->glyph_set, klass_mapping,
                            base_indexes);
    (এই+baseArray).collect_variation_indices (c, base_indexes.iter ());
  }

  শূন্য collect_glyphs (hb_collect_glyphs_context_t *c) ধ্রুবক
  {
    যদি (unlikely (!(এই+markCoverage).collect_coverage (c->প্রবেশ))) ফেরত;
    যদি (unlikely (!(এই+baseCoverage).collect_coverage (c->প্রবেশ))) ফেরত;
  }

  শূন্য collect_second_glyphs (hb_set_digest_t *digest) ধ্রুবক
  { (এই+baseCoverage).collect_coverage (digest); }

  ধ্রুবক Coverage &get_coverage () ধ্রুবক { ফেরত এই+markCoverage; }

  স্থির অন্তঃরেখ যৌক্তিক accept (hb_বাফার_t *বাফার, unsigned idx)
  {
    /* We only want to attach to the first of a MultipleSubst sequence.
     * https://github.com/harfbuzz/harfbuzz/issues/740
     * Reject others...
     * ...but stop if we find a mark in the MultipleSubst sequence:
     * https://github.com/harfbuzz/harfbuzz/issues/1020 */
    ফেরত !_hb_glyph_info_multiplied (&বাফার->info[idx]) ||
	   0 == _hb_glyph_info_get_lig_comp (&বাফার->info[idx]) ||
	   (idx == 0 ||
	    _hb_glyph_info_is_mark (&বাফার->info[idx - 1]) ||
	    !_hb_glyph_info_multiplied (&বাফার->info[idx - 1]) ||
	    _hb_glyph_info_get_lig_id (&বাফার->info[idx]) !=
	    _hb_glyph_info_get_lig_id (&বাফার->info[idx - 1]) ||
	    _hb_glyph_info_get_lig_comp (&বাফার->info[idx]) !=
	    _hb_glyph_info_get_lig_comp (&বাফার->info[idx - 1]) + 1
	    );
  }

  যৌক্তিক apply (hb_ot_apply_context_t *c) ধ্রুবক
  {
    TRACE_APPLY (এই);
    hb_বাফার_t *বাফার = c->বাফার;
    unsigned পূর্ণসংখ্যা mark_সূচক = (এই+markCoverage).get_coverage  (বাফার->cur().codepoint);
    যদি (likely (mark_সূচক == NOT_COVERED)) return_trace (মিথ্যা);

    /* Now we search backwards for a non-mark glyph.
     * We don't use skippy_iter.prev() to avoid O(n^2) behavior. */

    auto &skippy_iter = c->iter_প্রবেশ;
    skippy_iter.set_lookup_props (LookupFlag::IgnoreMarks);

    যদি (c->last_base_until > বাফার->idx)
    {
      c->last_base_until = 0;
      c->last_base = -1;
    }
    unsigned j;
    জন্য (j = বাফার->idx; j > c->last_base_until; j--)
    {
      auto match = skippy_iter.match (বাফার->info[j - 1]);
      যদি (match == skippy_iter.MATCH)
      {
        // https://github.com/harfbuzz/harfbuzz/issues/4124
	যদি (!accept (বাফার, j - 1) &&
	    NOT_COVERED == (এই+baseCoverage).get_coverage  (বাফার->info[j - 1].codepoint))
	  match = skippy_iter.SKIP;
      }
      যদি (match == skippy_iter.MATCH)
      {
	c->last_base = (signed) j - 1;
	বিরতি;
      }
    }
    c->last_base_until = বাফার->idx;
    যদি (c->last_base == -1)
    {
      বাফার->unsafe_to_concat_from_outbuffer (0, বাফার->idx + 1);
      return_trace (মিথ্যা);
    }

    unsigned idx = (unsigned) c->last_base;

    /* Checking that matched glyph is actually a base glyph by GDEF is too strong; disabled */
    //if (!_hb_glyph_info_is_base_glyph (&buffer->info[idx])) { return_trace (false); }

    unsigned পূর্ণসংখ্যা base_সূচক = (এই+baseCoverage).get_coverage  (বাফার->info[idx].codepoint);
    যদি (base_সূচক == NOT_COVERED)
    {
      বাফার->unsafe_to_concat_from_outbuffer (idx, বাফার->idx + 1);
      return_trace (মিথ্যা);
    }

    return_trace ((এই+markArray).apply (c, mark_সূচক, base_সূচক, এই+baseArray, class_গুনতি_2, idx));
  }

  যৌক্তিক subset (hb_subset_context_t *c) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    ধ্রুবক hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ধ্রুবক hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*এই);
    যদি (unlikely (!c->serializer->extend_min (out))) return_trace (মিথ্যা);
    out->format = format;

    hb_map_t klass_mapping;
    Markclass_closure_and_remap_indexes (এই+markCoverage, এই+markArray, glyphset, &klass_mapping);

    যদি (!klass_mapping.get_population ()) return_trace (মিথ্যা);
    out->class_গুনতি_2 = klass_mapping.get_population ();

    auto mark_iter =
    + hb_zip (এই+markCoverage, এই+markArray)
    | hb_filter (glyphset, hb_first)
    ;

    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + mark_iter
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    যদি (!out->markCoverage.serialize_serialize (c->serializer, new_coverage.iter ()))
      return_trace (মিথ্যা);

    যদি (unlikely (!out->markArray.serialize_subset (c, markArray, এই,
						    (এই+markCoverage).iter (),
						    &klass_mapping)))
      return_trace (মিথ্যা);

    new_coverage.reset ();
    hb_sorted_vector_t<unsigned> base_indexes;
    ধ্রুবক auto &base_array = এই+baseArray;
    collect_subset_anchor_indexes (এই+baseCoverage, base_array.rows,
                                   class_গুনতি_2, base_array.matrixZ.arrayZ,
                                   glyphset, glyph_map, klass_mapping,
                                   new_coverage, base_indexes);

    যদি (!new_coverage) return_trace (মিথ্যা);
    যদি (!out->baseCoverage.serialize_serialize (c->serializer, new_coverage.iter ()))
      return_trace (মিথ্যা);

    return_trace (out->baseArray.serialize_subset (c, baseArray, এই,
						   new_coverage.দৈর্ঘ্য,
						   base_indexes.iter ()));
  }
};


}
}
}

#endif /* OT_LAYOUT_GPOS_MARKBASEPOSFORMAT1_HH */
