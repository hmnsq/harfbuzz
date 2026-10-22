#ifndef OT_LAYOUT_GPOS_MARKBASEPOSFORMAT1_HH
#define OT_LAYOUT_GPOS_MARKBASEPOSFORMAT1_HH

#ضمن "MarkArray.hh"

نطاق OT {
نطاق Layout {
نطاق GPOS_impl {

template <typename Types>
بنية MarkBasePosFormat1_2
{
  محمي:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  typename Types::template LOffsetTo<Coverage>
                markCoverage;           /* Offset to MarkCoverage table--from
                                         * beginning of MarkBasePos subtable */
  typename Types::template LOffsetTo<Coverage>
                baseCoverage;           /* Offset to BaseCoverage table--from
                                         * beginning of MarkBasePos subtable */
  HBUINT16      classCount;             /* Number of classes defined for marks */
  typename Types::template LOffsetTo<MarkArray<Types>>
                markArray;              /* Offset to MarkArray table--from
                                         * beginning of MarkBasePos subtable */
  typename Types::template LOffsetTo<AnchorMatrix<Types>>
                baseArray;              /* Offset to BaseArray table--from
                                         * beginning of MarkBasePos subtable */

  عام:
  DEFINE_SIZE_STATIC (4 + 4 * Types::LOffset::static_size);

    منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
                  markCoverage.sanitize (c, هذا) &&
                  baseCoverage.sanitize (c, هذا) &&
                  markArray.sanitize (c, هذا) &&
                  baseArray.sanitize (c, هذا, (unsigned صحيح) classCount));
  }

  منطقي intersects (ثابت hb_set_t *glyphs) ثابت
  {
    ܦܢܐ (هذا+markCoverage).intersects (glyphs) &&
           (هذا+baseCoverage).intersects (glyphs);
  }

  فراغ closure_lookups (hb_closure_lookups_context_t *c) ثابت {}

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    (هذا+markArray).collect_variation_indices (c, هذا+markCoverage);

    hb_map_t klass_mapping;
    Markclass_closure_and_remap_indexes (هذا+markCoverage, هذا+markArray, *c->glyph_set, &klass_mapping);

    hb_sorted_vector_t<unsigned> base_indexes;
    collect_anchor_indexes (هذا+baseCoverage, (هذا+baseArray).rows,
                            classCount, *c->glyph_set, klass_mapping,
                            base_indexes);
    (هذا+baseArray).collect_variation_indices (c, base_indexes.iter ());
  }

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    ܐܢ (unlikely (!(هذا+markCoverage).collect_coverage (c->input))) ܦܢܐ;
    ܐܢ (unlikely (!(هذا+baseCoverage).collect_coverage (c->input))) ܦܢܐ;
  }

  فراغ collect_second_glyphs (hb_set_digest_t *digest) ثابت
  { (هذا+baseCoverage).collect_coverage (digest); }

  ثابت Coverage &get_coverage () ثابت { ܦܢܐ هذا+markCoverage; }

  ساكن مضمن منطقي accept (hb_buffer_t *buffer, unsigned idx)
  {
    /* We only want to attach to the first of a MultipleSubst sequence.
     * https://github.com/harfbuzz/harfbuzz/issues/740
     * Reject others...
     * ...but stop if we find a mark in the MultipleSubst sequence:
     * https://github.com/harfbuzz/harfbuzz/issues/1020 */
    ܦܢܐ !_hb_glyph_info_multiplied (&buffer->info[idx]) ||
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

  منطقي apply (hb_ot_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);
    hb_buffer_t *buffer = c->buffer;
    unsigned صحيح mark_index = (هذا+markCoverage).get_coverage  (buffer->cur().codepoint);
    ܐܢ (likely (mark_index == NOT_COVERED)) return_trace (ܕܓܠܐ);

    /* Now we search backwards for a non-mark glyph.
     * We don't use skippy_iter.prev() to avoid O(n^2) behavior. */

    auto &skippy_iter = c->iter_input;
    skippy_iter.set_lookup_props (LookupFlag::IgnoreMarks);

    ܐܢ (c->last_base_until > buffer->idx)
    {
      c->last_base_until = 0;
      c->last_base = -1;
    }
    unsigned j;
    لكل (j = buffer->idx; j > c->last_base_until; j--)
    {
      auto match = skippy_iter.match (buffer->info[j - 1]);
      ܐܢ (match == skippy_iter.MATCH)
      {
        // https://github.com/harfbuzz/harfbuzz/issues/4124
	ܐܢ (!accept (buffer, j - 1) &&
	    NOT_COVERED == (هذا+baseCoverage).get_coverage  (buffer->info[j - 1].codepoint))
	  match = skippy_iter.SKIP;
      }
      ܐܢ (match == skippy_iter.MATCH)
      {
	c->last_base = (signed) j - 1;
	اكسر;
      }
    }
    c->last_base_until = buffer->idx;
    ܐܢ (c->last_base == -1)
    {
      buffer->unsafe_to_concat_from_outbuffer (0, buffer->idx + 1);
      return_trace (ܕܓܠܐ);
    }

    unsigned idx = (unsigned) c->last_base;

    /* Checking that matched glyph is actually a base glyph by GDEF is too strong; disabled */
    //if (!_hb_glyph_info_is_base_glyph (&buffer->info[idx])) { return_trace (false); }

    unsigned صحيح base_index = (هذا+baseCoverage).get_coverage  (buffer->info[idx].codepoint);
    ܐܢ (base_index == NOT_COVERED)
    {
      buffer->unsafe_to_concat_from_outbuffer (idx, buffer->idx + 1);
      return_trace (ܕܓܠܐ);
    }

    return_trace ((هذا+markArray).apply (c, mark_index, base_index, هذا+baseArray, classCount, idx));
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*هذا);
    ܐܢ (unlikely (!c->serializer->extend_min (out))) return_trace (ܕܓܠܐ);
    out->format = format;

    hb_map_t klass_mapping;
    Markclass_closure_and_remap_indexes (هذا+markCoverage, هذا+markArray, glyphset, &klass_mapping);

    ܐܢ (!klass_mapping.get_population ()) return_trace (ܕܓܠܐ);
    out->classCount = klass_mapping.get_population ();

    auto mark_iter =
    + hb_zip (هذا+markCoverage, هذا+markArray)
    | hb_filter (glyphset, hb_first)
    ;

    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + mark_iter
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    ܐܢ (!out->markCoverage.serialize_serialize (c->serializer, new_coverage.iter ()))
      return_trace (ܕܓܠܐ);

    ܐܢ (unlikely (!out->markArray.serialize_subset (c, markArray, هذا,
						    (هذا+markCoverage).iter (),
						    &klass_mapping)))
      return_trace (ܕܓܠܐ);

    new_coverage.reset ();
    hb_sorted_vector_t<unsigned> base_indexes;
    ثابت auto &base_array = هذا+baseArray;
    collect_subset_anchor_indexes (هذا+baseCoverage, base_array.rows,
                                   classCount, base_array.matrixZ.arrayZ,
                                   glyphset, glyph_map, klass_mapping,
                                   new_coverage, base_indexes);

    ܐܢ (!new_coverage) return_trace (ܕܓܠܐ);
    ܐܢ (!out->baseCoverage.serialize_serialize (c->serializer, new_coverage.iter ()))
      return_trace (ܕܓܠܐ);

    return_trace (out->baseArray.serialize_subset (c, baseArray, هذا,
						   new_coverage.length,
						   base_indexes.iter ()));
  }
};


}
}
}

#endif /* OT_LAYOUT_GPOS_MARKBASEPOSFORMAT1_HH */
