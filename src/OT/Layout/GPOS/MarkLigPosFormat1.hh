#ifndef OT_LAYOUT_GPOS_MARKLIGPOSFORMAT1_HH
#define OT_LAYOUT_GPOS_MARKLIGPOSFORMAT1_HH

#شامل "LigatureArray.hh"

فضاینام OT {
فضاینام Layout {
فضاینام GPOS_impl {


template <typename Types>
ساختار MarkLigPosFormat1_2
{
  محافظتشده:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  typename Types::template LOffsetTo<Coverage>
                markCoverage;           /* Offset to Mark Coverage table--from
                                         * beginning of MarkLigPos subtable */
  typename Types::template LOffsetTo<Coverage>
                ligatureCoverage;       /* Offset to Ligature Coverage
                                         * table--from beginning of MarkLigPos
                                         * subtable */
  HBUINT16      class_شمار_2;             /* Number of defined mark classes */
  typename Types::template LOffsetTo<MarkArray<Types>>
                markArray;              /* Offset to MarkArray table--from
                                         * beginning of MarkLigPos subtable */
  typename Types::template LOffsetTo<LigatureArray<Types>>
                ligatureArray;          /* Offset to LigatureArray table--from
                                         * beginning of MarkLigPos subtable */
  عمومی:
  DEFINE_SIZE_STATIC (4 + 4 * Types::LOffset::static_size);

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این) &&
                  markCoverage.sanitize (c, این) &&
                  ligatureCoverage.sanitize (c, این) &&
                  markArray.sanitize (c, این) &&
                  ligatureArray.sanitize (c, این, (unsigned صحیح) class_شمار_2));
  }

  منطقی intersects (ثابت hb_set_t *glyphs) ثابت
  {
    بازگشت (این+markCoverage).intersects (glyphs) &&
           (این+ligatureCoverage).intersects (glyphs);
  }

  تهی closure_lookups (hb_closure_lookups_context_t *c) ثابت {}

  تهی collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    (این+markArray).collect_variation_indices (c, این+markCoverage);

    hb_map_t klass_mapping;
    Markclass_closure_and_remap_indexes (این+markCoverage, این+markArray, *c->glyph_set, &klass_mapping);

    (این+ligatureArray).collect_variation_indices (c, این+ligatureCoverage,
                                                   class_شمار_2, klass_mapping);
  }

  تهی collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    اگر (unlikely (!(این+markCoverage).collect_coverage (c->درون_داد))) بازگشت;
    اگر (unlikely (!(این+ligatureCoverage).collect_coverage (c->درون_داد))) بازگشت;
  }

  تهی collect_second_glyphs (hb_set_digest_t *digest) ثابت
  { (این+ligatureCoverage).collect_coverage (digest); }

  ثابت Coverage &get_coverage () ثابت { بازگشت این+markCoverage; }

  ایستا درونخطی منطقی accept (hb_buffer_t *buffer, unsigned idx)
  {
    /* We only want to attach to the first of a MultipleSubst sequence,
     * which might have been ligated into a preceding ligature, and in that
     * case the mark should attach to that ligature.
     * https://github.com/harfbuzz/harfbuzz/issues/4969
     * Reject others... */
    بازگشت !_hb_glyph_info_multiplied (&buffer->info[idx]) ||
	   0 == _hb_glyph_info_get_lig_comp (&buffer->info[idx]);
  }

  منطقی apply (hb_ot_apply_context_t *c) ثابت
  {
    TRACE_APPLY (این);
    hb_buffer_t *buffer = c->buffer;
    unsigned صحیح mark_index = (این+markCoverage).get_coverage  (buffer->cur().codepoint);
    اگر (likely (mark_index == NOT_COVERED)) return_trace (نادرست);

    /* Now we search backwards for a non-mark glyph */

    auto &skippy_iter = c->iter_درون_داد;
    skippy_iter.set_lookup_props (LookupFlag::IgnoreMarks);

    اگر (c->last_base_until > buffer->idx)
    {
      c->last_base_until = 0;
      c->last_base = -1;
    }
    unsigned j;
    برای (j = buffer->idx; j > c->last_base_until; j--)
    {
      auto match = skippy_iter.match (buffer->info[j - 1]);
      اگر (match == skippy_iter.MATCH)
      {
        // https://github.com/harfbuzz/harfbuzz/issues/4124
	اگر (!accept (buffer, j - 1) &&
	    NOT_COVERED == (این+ligatureCoverage).get_coverage  (buffer->info[j - 1].codepoint))
	  match = skippy_iter.SKIP;
      }
      اگر (match == skippy_iter.MATCH)
      {
	c->last_base = (signed) j - 1;
	بشکن;
      }
    }
    c->last_base_until = buffer->idx;
    اگر (c->last_base == -1)
    {
      buffer->unsafe_to_concat_from_outbuffer (0, buffer->idx + 1);
      return_trace (نادرست);
    }

    unsigned idx = (unsigned) c->last_base;

    /* Checking that matched glyph is actually a ligature by GDEF is too strong; disabled */
    //if (!_hb_glyph_info_is_ligature (&buffer->info[idx])) { return_trace (false); }

    unsigned صحیح lig_index = (این+ligatureCoverage).get_coverage  (buffer->info[idx].codepoint);
    اگر (lig_index == NOT_COVERED)
    {
      buffer->unsafe_to_concat_from_outbuffer (idx, buffer->idx + 1);
      return_trace (نادرست);
    }

    ثابت LigatureArray<Types>& lig_array = این+ligatureArray;
    ثابت AnchorMatrix<Types, HBUINT16>& lig_attach = lig_array[lig_index];

    /* Find component to attach to */
    unsigned صحیح comp_شمار = lig_attach.rows;
    اگر (unlikely (!comp_شمار))
    {
      buffer->unsafe_to_concat_from_outbuffer (idx, buffer->idx + 1);
      return_trace (نادرست);
    }

    /* We must now check whether the ligature ID of the current mark glyph
     * is identical to the ligature ID of the found ligature.  If yes, we
     * can directly use the component index.  If not, we attach the mark
     * glyph to the last component of the ligature. */
    unsigned صحیح comp_index;
    unsigned صحیح lig_id = _hb_glyph_info_get_lig_id (&buffer->info[idx]);
    unsigned صحیح mark_id = _hb_glyph_info_get_lig_id (&buffer->cur());
    unsigned صحیح mark_comp = _hb_glyph_info_get_lig_comp (&buffer->cur());
    اگر (lig_id && lig_id == mark_id && mark_comp > 0)
      comp_index = hb_min (comp_شمار, _hb_glyph_info_get_lig_comp (&buffer->cur())) - 1;
    وگرنه
      comp_index = comp_شمار - 1;

    return_trace ((این+markArray).apply (c, mark_index, comp_index, lig_attach, class_شمار_2, idx));
  }

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (این);
    ثابت hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = c->plan->glyph_map_gsub;

    auto *out = c->serializer->start_embed (*این);
    اگر (unlikely (!c->serializer->extend_min (out))) return_trace (نادرست);
    out->format = format;

    hb_map_t klass_mapping;
    Markclass_closure_and_remap_indexes (این+markCoverage, این+markArray, glyphset, &klass_mapping);

    اگر (!klass_mapping.get_population ()) return_trace (نادرست);
    out->class_شمار_2 = klass_mapping.get_population ();

    auto mark_iter =
    + hb_zip (این+markCoverage, hb_range ((unsigned) (این+markArray).len))
    | hb_filter (glyphset, hb_first)
    ;

    auto new_mark_coverage =
    + mark_iter
    | hb_map_retains_sorting (hb_first)
    | hb_map_retains_sorting (glyph_map)
    ;

    اگر (!out->markCoverage.serialize_serialize (c->serializer, new_mark_coverage))
      return_trace (نادرست);

    اگر (unlikely (!out->markArray.serialize_subset (c, markArray, این,
						    (این+markCoverage).iter (),
						    &klass_mapping)))
      return_trace (نادرست);

    hb_sorted_vector_t<hb_codepoint_t> new_lig_coverage;
    اگر (!out->ligatureArray.serialize_subset (c, ligatureArray, این,
					      hb_iter (این+ligatureCoverage),
					      class_شمار_2, &klass_mapping, new_lig_coverage))
      return_trace (نادرست);

    return_trace (out->ligatureCoverage.serialize_serialize (c->serializer, new_lig_coverage.iter ()));
  }

};

}
}
}

#endif /* OT_LAYOUT_GPOS_MARKLIGPOSFORMAT1_HH */
