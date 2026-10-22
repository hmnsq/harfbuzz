#ifndef OT_LAYOUT_GPOS_MARKLIGPOSFORMAT1_HH
#define OT_LAYOUT_GPOS_MARKLIGPOSFORMAT1_HH

#অন্তর্ভাব "LigatureArray.hh"

নামাকাশ OT {
নামাকাশ Layout {
নামাকাশ GPOS_impl {


template <typename Types>
কাঠামো MarkLigPosFormat1_2
{
  সুরক্ষিত:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  typename Types::template LOffsetTo<Coverage>
                markCoverage;           /* Offset to Mark Coverage table--from
                                         * beginning of MarkLigPos subtable */
  typename Types::template LOffsetTo<Coverage>
                ligatureCoverage;       /* Offset to Ligature Coverage
                                         * table--from beginning of MarkLigPos
                                         * subtable */
  HBUINT16      class_গুনতি_2;             /* Number of defined mark classes */
  typename Types::template LOffsetTo<MarkArray<Types>>
                markArray;              /* Offset to MarkArray table--from
                                         * beginning of MarkLigPos subtable */
  typename Types::template LOffsetTo<LigatureArray<Types>>
                ligatureArray;          /* Offset to LigatureArray table--from
                                         * beginning of MarkLigPos subtable */
  সর্বজনীন:
  DEFINE_আকার_STATIC (4 + 4 * Types::LOffset::static_আকার);

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই) &&
                  markCoverage.sanitize (c, এই) &&
                  ligatureCoverage.sanitize (c, এই) &&
                  markArray.sanitize (c, এই) &&
                  ligatureArray.sanitize (c, এই, (unsigned পূর্ণসংখ্যা) class_গুনতি_2));
  }

  যৌক্তিক intersects (ধ্রুবক hb_set_t *glyphs) ধ্রুবক
  {
    ফেরত (এই+markCoverage).intersects (glyphs) &&
           (এই+ligatureCoverage).intersects (glyphs);
  }

  শূন্য closure_lookups (hb_closure_lookups_context_t *c) ধ্রুবক {}

  শূন্য collect_variation_indices (hb_collect_variation_indices_context_t *c) ধ্রুবক
  {
    (এই+markArray).collect_variation_indices (c, এই+markCoverage);

    hb_map_t klass_mapping;
    Markclass_closure_and_remap_indexes (এই+markCoverage, এই+markArray, *c->glyph_set, &klass_mapping);

    (এই+ligatureArray).collect_variation_indices (c, এই+ligatureCoverage,
                                                   class_গুনতি_2, klass_mapping);
  }

  শূন্য collect_glyphs (hb_collect_glyphs_context_t *c) ধ্রুবক
  {
    যদি (unlikely (!(এই+markCoverage).collect_coverage (c->প্রবেশ))) ফেরত;
    যদি (unlikely (!(এই+ligatureCoverage).collect_coverage (c->প্রবেশ))) ফেরত;
  }

  শূন্য collect_second_glyphs (hb_set_digest_t *digest) ধ্রুবক
  { (এই+ligatureCoverage).collect_coverage (digest); }

  ধ্রুবক Coverage &get_coverage () ধ্রুবক { ফেরত এই+markCoverage; }

  স্থির অন্তঃরেখ যৌক্তিক accept (hb_বাফার_t *বাফার, unsigned idx)
  {
    /* We only want to attach to the first of a MultipleSubst sequence,
     * which might have been ligated into a preceding ligature, and in that
     * case the mark should attach to that ligature.
     * https://github.com/harfbuzz/harfbuzz/issues/4969
     * Reject others... */
    ফেরত !_hb_glyph_info_multiplied (&বাফার->info[idx]) ||
	   0 == _hb_glyph_info_get_lig_comp (&বাফার->info[idx]);
  }

  যৌক্তিক apply (hb_ot_apply_context_t *c) ধ্রুবক
  {
    TRACE_APPLY (এই);
    hb_বাফার_t *বাফার = c->বাফার;
    unsigned পূর্ণসংখ্যা mark_সূচক = (এই+markCoverage).get_coverage  (বাফার->cur().codepoint);
    যদি (likely (mark_সূচক == NOT_COVERED)) return_trace (মিথ্যা);

    /* Now we search backwards for a non-mark glyph */

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
	    NOT_COVERED == (এই+ligatureCoverage).get_coverage  (বাফার->info[j - 1].codepoint))
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

    /* Checking that matched glyph is actually a ligature by GDEF is too strong; disabled */
    //if (!_hb_glyph_info_is_ligature (&buffer->info[idx])) { return_trace (false); }

    unsigned পূর্ণসংখ্যা lig_সূচক = (এই+ligatureCoverage).get_coverage  (বাফার->info[idx].codepoint);
    যদি (lig_সূচক == NOT_COVERED)
    {
      বাফার->unsafe_to_concat_from_outbuffer (idx, বাফার->idx + 1);
      return_trace (মিথ্যা);
    }

    ধ্রুবক LigatureArray<Types>& lig_array = এই+ligatureArray;
    ধ্রুবক AnchorMatrix<Types, HBUINT16>& lig_attach = lig_array[lig_সূচক];

    /* Find component to attach to */
    unsigned পূর্ণসংখ্যা comp_গুনতি = lig_attach.rows;
    যদি (unlikely (!comp_গুনতি))
    {
      বাফার->unsafe_to_concat_from_outbuffer (idx, বাফার->idx + 1);
      return_trace (মিথ্যা);
    }

    /* We must now check whether the ligature ID of the current mark glyph
     * is identical to the ligature ID of the found ligature.  If yes, we
     * can directly use the component index.  If not, we attach the mark
     * glyph to the last component of the ligature. */
    unsigned পূর্ণসংখ্যা comp_সূচক;
    unsigned পূর্ণসংখ্যা lig_id = _hb_glyph_info_get_lig_id (&বাফার->info[idx]);
    unsigned পূর্ণসংখ্যা mark_id = _hb_glyph_info_get_lig_id (&বাফার->cur());
    unsigned পূর্ণসংখ্যা mark_comp = _hb_glyph_info_get_lig_comp (&বাফার->cur());
    যদি (lig_id && lig_id == mark_id && mark_comp > 0)
      comp_সূচক = hb_min (comp_গুনতি, _hb_glyph_info_get_lig_comp (&বাফার->cur())) - 1;
    অন্যথা
      comp_সূচক = comp_গুনতি - 1;

    return_trace ((এই+markArray).apply (c, mark_সূচক, comp_সূচক, lig_attach, class_গুনতি_2, idx));
  }

  যৌক্তিক subset (hb_subset_context_t *c) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    ধ্রুবক hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ধ্রুবক hb_map_t &glyph_map = c->plan->glyph_map_gsub;

    auto *out = c->serializer->start_embed (*এই);
    যদি (unlikely (!c->serializer->extend_min (out))) return_trace (মিথ্যা);
    out->format = format;

    hb_map_t klass_mapping;
    Markclass_closure_and_remap_indexes (এই+markCoverage, এই+markArray, glyphset, &klass_mapping);

    যদি (!klass_mapping.get_population ()) return_trace (মিথ্যা);
    out->class_গুনতি_2 = klass_mapping.get_population ();

    auto mark_iter =
    + hb_zip (এই+markCoverage, hb_range ((unsigned) (এই+markArray).len))
    | hb_filter (glyphset, hb_first)
    ;

    auto new_mark_coverage =
    + mark_iter
    | hb_map_retains_sorting (hb_first)
    | hb_map_retains_sorting (glyph_map)
    ;

    যদি (!out->markCoverage.serialize_serialize (c->serializer, new_mark_coverage))
      return_trace (মিথ্যা);

    যদি (unlikely (!out->markArray.serialize_subset (c, markArray, এই,
						    (এই+markCoverage).iter (),
						    &klass_mapping)))
      return_trace (মিথ্যা);

    hb_sorted_vector_t<hb_codepoint_t> new_lig_coverage;
    যদি (!out->ligatureArray.serialize_subset (c, ligatureArray, এই,
					      hb_iter (এই+ligatureCoverage),
					      class_গুনতি_2, &klass_mapping, new_lig_coverage))
      return_trace (মিথ্যা);

    return_trace (out->ligatureCoverage.serialize_serialize (c->serializer, new_lig_coverage.iter ()));
  }

};

}
}
}

#endif /* OT_LAYOUT_GPOS_MARKLIGPOSFORMAT1_HH */
