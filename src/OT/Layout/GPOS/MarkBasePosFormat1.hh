#ifndef OT_LAYOUT_GPOS_MARKBASEPOSFORMAT1_HH
#define OT_LAYOUT_GPOS_MARKBASEPOSFORMAT1_HH

#شامل "MarkArray.hh"

فضاینام OT {
فضاینام Layout {
فضاینام GPOS_impl {

template <typename Types>
ساختار MarkBasePosFormat1_2
{
  محافظتشده:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  typename Types::template LOffsetTo<Coverage>
                markCoverage;           /* Offset to MarkCoverage table--from
                                         * beginning of MarkBasePos subtable */
  typename Types::template LOffsetTo<Coverage>
                baseCoverage;           /* Offset to BaseCoverage table--from
                                         * beginning of MarkBasePos subtable */
  HBUINT16      class_شمار_2;             /* Number of classes defined for marks */
  typename Types::template LOffsetTo<MarkArray<Types>>
                markArray;              /* Offset to MarkArray table--from
                                         * beginning of MarkBasePos subtable */
  typename Types::template LOffsetTo<AnchorMatrix<Types>>
                baseArray;              /* Offset to BaseArray table--from
                                         * beginning of MarkBasePos subtable */

  عمومی:
  DEFINE_SIZE_STATIC (4 + 4 * Types::LOffset::static_size);

    منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این) &&
                  markCoverage.sanitize (c, این) &&
                  baseCoverage.sanitize (c, این) &&
                  markArray.sanitize (c, این) &&
                  baseArray.sanitize (c, این, (unsigned صحیح) class_شمار_2));
  }

  منطقی intersects (ثابت hb_set_t *glyphs) ثابت
  {
    بازگشت (این+markCoverage).intersects (glyphs) &&
           (این+baseCoverage).intersects (glyphs);
  }

  تهی closure_lookups (hb_closure_lookups_context_t *c) ثابت {}

  تهی collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    (این+markArray).collect_variation_indices (c, این+markCoverage);

    hb_map_t klass_mapping;
    Markclass_closure_and_remap_indexes (این+markCoverage, این+markArray, *c->glyph_set, &klass_mapping);

    hb_sorted_vector_t<unsigned> base_indexes;
    collect_anchor_indexes (این+baseCoverage, (این+baseArray).rows,
                            class_شمار_2, *c->glyph_set, klass_mapping,
                            base_indexes);
    (این+baseArray).collect_variation_indices (c, base_indexes.iter ());
  }

  تهی collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    اگر (unlikely (!(این+markCoverage).collect_coverage (c->درون_داد))) بازگشت;
    اگر (unlikely (!(این+baseCoverage).collect_coverage (c->درون_داد))) بازگشت;
  }

  تهی collect_second_glyphs (hb_set_digest_t *digest) ثابت
  { (این+baseCoverage).collect_coverage (digest); }

  ثابت Coverage &get_coverage () ثابت { بازگشت این+markCoverage; }

  ایستا درونخطی منطقی accept (hb_buffer_t *buffer, unsigned idx)
  {
    /* We only want to attach to the first of a MultipleSubst sequence.
     * https://github.com/harfbuzz/harfbuzz/issues/740
     * Reject others...
     * ...but stop if we find a mark in the MultipleSubst sequence:
     * https://github.com/harfbuzz/harfbuzz/issues/1020 */
    بازگشت !_hb_glyph_info_multiplied (&buffer->info[idx]) ||
	   0 == _hb_glyph_info_get_lig_comp (&buffer->info[idx]) ||
	   (idx == 0 ||
	    _hb_glyph_info_is_mark (&buffer->info[idx - 1]) ||
	    !_hb_glyph_info_multiplied (&buffer->info[idx - 1]) ||
	    _hb_glyph_info_get_lig_id (&buffer->info[idx]) !=
	    _hb_glyph_info_get_lig_id (&buffer->info[idx - 1]) ||
	    _hb_glyph_info_get_lig_comp (&buffer->info[idx]) !=
	    _hb_glyph_info_get_lig_comp (&buffer->info[idx - 1]) + 1
	    );
  }

  منطقی apply (hb_ot_apply_context_t *c) ثابت
  {
    TRACE_APPLY (این);
    hb_buffer_t *buffer = c->buffer;
    unsigned صحیح mark_index = (این+markCoverage).get_coverage  (buffer->cur().codepoint);
    اگر (likely (mark_index == NOT_COVERED)) return_trace (نادرست);

    /* Now we search backwards for a non-mark glyph.
     * We don't use skippy_iter.prev() to avoid O(n^2) behavior. */

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
	    NOT_COVERED == (این+baseCoverage).get_coverage  (buffer->info[j - 1].codepoint))
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

    /* Checking that matched glyph is actually a base glyph by GDEF is too strong; disabled */
    //if (!_hb_glyph_info_is_base_glyph (&buffer->info[idx])) { return_trace (false); }

    unsigned صحیح base_index = (این+baseCoverage).get_coverage  (buffer->info[idx].codepoint);
    اگر (base_index == NOT_COVERED)
    {
      buffer->unsafe_to_concat_from_outbuffer (idx, buffer->idx + 1);
      return_trace (نادرست);
    }

    return_trace ((این+markArray).apply (c, mark_index, base_index, این+baseArray, class_شمار_2, idx));
  }

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (این);
    ثابت hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*این);
    اگر (unlikely (!c->serializer->extend_min (out))) return_trace (نادرست);
    out->format = format;

    hb_map_t klass_mapping;
    Markclass_closure_and_remap_indexes (این+markCoverage, این+markArray, glyphset, &klass_mapping);

    اگر (!klass_mapping.get_population ()) return_trace (نادرست);
    out->class_شمار_2 = klass_mapping.get_population ();

    auto mark_iter =
    + hb_zip (این+markCoverage, این+markArray)
    | hb_filter (glyphset, hb_first)
    ;

    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + mark_iter
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    اگر (!out->markCoverage.serialize_serialize (c->serializer, new_coverage.iter ()))
      return_trace (نادرست);

    اگر (unlikely (!out->markArray.serialize_subset (c, markArray, این,
						    (این+markCoverage).iter (),
						    &klass_mapping)))
      return_trace (نادرست);

    new_coverage.reset ();
    hb_sorted_vector_t<unsigned> base_indexes;
    ثابت auto &base_array = این+baseArray;
    collect_subset_anchor_indexes (این+baseCoverage, base_array.rows,
                                   class_شمار_2, base_array.matrixZ.arrayZ,
                                   glyphset, glyph_map, klass_mapping,
                                   new_coverage, base_indexes);

    اگر (!new_coverage) return_trace (نادرست);
    اگر (!out->baseCoverage.serialize_serialize (c->serializer, new_coverage.iter ()))
      return_trace (نادرست);

    return_trace (out->baseArray.serialize_subset (c, baseArray, این,
						   new_coverage.length,
						   base_indexes.iter ()));
  }
};


}
}
}

#endif /* OT_LAYOUT_GPOS_MARKBASEPOSFORMAT1_HH */
