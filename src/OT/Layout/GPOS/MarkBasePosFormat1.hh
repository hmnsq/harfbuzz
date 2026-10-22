#ifndef OT_LAYOUT_GPOS_MARKBASEPOSFORMAT1_HH
#define OT_LAYOUT_GPOS_MARKBASEPOSFORMAT1_HH

#अन्तर्भाव "MarkArray.hh"

नामाकाश OT {
नामाकाश Layout {
नामाकाश GPOS_impl {

template <typename Types>
संरचना MarkBasePosFormat1_2
{
  रक्षित:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  typename Types::template LOffsetTo<Coverage>
                markCoverage;           /* Offset to MarkCoverage table--from
                                         * beginning of MarkBasePos subtable */
  typename Types::template LOffsetTo<Coverage>
                baseCoverage;           /* Offset to BaseCoverage table--from
                                         * beginning of MarkBasePos subtable */
  HBUINT16      class_गणनम्_2;             /* Number of classes defined for marks */
  typename Types::template LOffsetTo<MarkArray<Types>>
                markArray;              /* Offset to MarkArray table--from
                                         * beginning of MarkBasePos subtable */
  typename Types::template LOffsetTo<AnchorMatrix<Types>>
                baseArray;              /* Offset to BaseArray table--from
                                         * beginning of MarkBasePos subtable */

  सार्वजनिक:
  DEFINE_SIZE_STATIC (4 + 4 * Types::LOffset::static_size);

    तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) &&
                  markCoverage.sanitize (c, एतत्) &&
                  baseCoverage.sanitize (c, एतत्) &&
                  markArray.sanitize (c, एतत्) &&
                  baseArray.sanitize (c, एतत्, (unsigned पूर्णाङ्क) class_गणनम्_2));
  }

  तार्किक intersects (ध्रुव hb_set_t *glyphs) ध्रुव
  {
    पटिनिवत्त (एतत्+markCoverage).intersects (glyphs) &&
           (एतत्+baseCoverage).intersects (glyphs);
  }

  शून्य closure_lookups (hb_closure_lookups_context_t *c) ध्रुव {}

  शून्य collect_variation_indices (hb_collect_variation_indices_context_t *c) ध्रुव
  {
    (एतत्+markArray).collect_variation_indices (c, एतत्+markCoverage);

    hb_map_t klass_mapping;
    Markclass_closure_and_remap_indexes (एतत्+markCoverage, एतत्+markArray, *c->glyph_set, &klass_mapping);

    hb_sorted_vector_t<unsigned> base_indexes;
    collect_anchor_indexes (एतत्+baseCoverage, (एतत्+baseArray).rows,
                            class_गणनम्_2, *c->glyph_set, klass_mapping,
                            base_indexes);
    (एतत्+baseArray).collect_variation_indices (c, base_indexes.iter ());
  }

  शून्य collect_glyphs (hb_collect_glyphs_context_t *c) ध्रुव
  {
    सचे (unlikely (!(एतत्+markCoverage).collect_coverage (c->निवेशः))) पटिनिवत्त;
    सचे (unlikely (!(एतत्+baseCoverage).collect_coverage (c->निवेशः))) पटिनिवत्त;
  }

  शून्य collect_second_glyphs (hb_set_digest_t *digest) ध्रुव
  { (एतत्+baseCoverage).collect_coverage (digest); }

  ध्रुव Coverage &get_coverage () ध्रुव { पटिनिवत्त एतत्+markCoverage; }

  स्थिर अन्तःस्थ तार्किक accept (hb_buffer_t *buffer, unsigned idx)
  {
    /* We only want to attach to the first of a MultipleSubst sequence.
     * https://github.com/harfbuzz/harfbuzz/issues/740
     * Reject others...
     * ...but stop if we find a mark in the MultipleSubst sequence:
     * https://github.com/harfbuzz/harfbuzz/issues/1020 */
    पटिनिवत्त !_hb_glyph_info_multiplied (&buffer->info[idx]) ||
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

  तार्किक apply (hb_ot_apply_context_t *c) ध्रुव
  {
    TRACE_APPLY (एतत्);
    hb_buffer_t *buffer = c->buffer;
    unsigned पूर्णाङ्क mark_index = (एतत्+markCoverage).get_coverage  (buffer->cur().codepoint);
    सचे (likely (mark_index == NOT_COVERED)) return_trace (मिच्छा);

    /* Now we search backwards for a non-mark glyph.
     * We don't use skippy_iter.prev() to avoid O(n^2) behavior. */

    auto &skippy_iter = c->iter_निवेशः;
    skippy_iter.set_lookup_props (LookupFlag::IgnoreMarks);

    सचे (c->last_base_until > buffer->idx)
    {
      c->last_base_until = 0;
      c->last_base = -1;
    }
    unsigned j;
    अत्थाय (j = buffer->idx; j > c->last_base_until; j--)
    {
      auto match = skippy_iter.match (buffer->info[j - 1]);
      सचे (match == skippy_iter.MATCH)
      {
        // https://github.com/harfbuzz/harfbuzz/issues/4124
	सचे (!accept (buffer, j - 1) &&
	    NOT_COVERED == (एतत्+baseCoverage).get_coverage  (buffer->info[j - 1].codepoint))
	  match = skippy_iter.SKIP;
      }
      सचे (match == skippy_iter.MATCH)
      {
	c->last_base = (signed) j - 1;
	भङ्ग;
      }
    }
    c->last_base_until = buffer->idx;
    सचे (c->last_base == -1)
    {
      buffer->unsafe_to_concat_from_outbuffer (0, buffer->idx + 1);
      return_trace (मिच्छा);
    }

    unsigned idx = (unsigned) c->last_base;

    /* Checking that matched glyph is actually a base glyph by GDEF is too strong; disabled */
    //if (!_hb_glyph_info_is_base_glyph (&buffer->info[idx])) { return_trace (false); }

    unsigned पूर्णाङ्क base_index = (एतत्+baseCoverage).get_coverage  (buffer->info[idx].codepoint);
    सचे (base_index == NOT_COVERED)
    {
      buffer->unsafe_to_concat_from_outbuffer (idx, buffer->idx + 1);
      return_trace (मिच्छा);
    }

    return_trace ((एतत्+markArray).apply (c, mark_index, base_index, एतत्+baseArray, class_गणनम्_2, idx));
  }

  तार्किक subset (hb_subset_context_t *c) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    ध्रुव hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ध्रुव hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*एतत्);
    सचे (unlikely (!c->serializer->extend_min (out))) return_trace (मिच्छा);
    out->format = format;

    hb_map_t klass_mapping;
    Markclass_closure_and_remap_indexes (एतत्+markCoverage, एतत्+markArray, glyphset, &klass_mapping);

    सचे (!klass_mapping.get_population ()) return_trace (मिच्छा);
    out->class_गणनम्_2 = klass_mapping.get_population ();

    auto mark_iter =
    + hb_zip (एतत्+markCoverage, एतत्+markArray)
    | hb_filter (glyphset, hb_first)
    ;

    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + mark_iter
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    सचे (!out->markCoverage.serialize_serialize (c->serializer, new_coverage.iter ()))
      return_trace (मिच्छा);

    सचे (unlikely (!out->markArray.serialize_subset (c, markArray, एतत्,
						    (एतत्+markCoverage).iter (),
						    &klass_mapping)))
      return_trace (मिच्छा);

    new_coverage.reset ();
    hb_sorted_vector_t<unsigned> base_indexes;
    ध्रुव auto &base_array = एतत्+baseArray;
    collect_subset_anchor_indexes (एतत्+baseCoverage, base_array.rows,
                                   class_गणनम्_2, base_array.matrixZ.arrayZ,
                                   glyphset, glyph_map, klass_mapping,
                                   new_coverage, base_indexes);

    सचे (!new_coverage) return_trace (मिच्छा);
    सचे (!out->baseCoverage.serialize_serialize (c->serializer, new_coverage.iter ()))
      return_trace (मिच्छा);

    return_trace (out->baseArray.serialize_subset (c, baseArray, एतत्,
						   new_coverage.length,
						   base_indexes.iter ()));
  }
};


}
}
}

#endif /* OT_LAYOUT_GPOS_MARKBASEPOSFORMAT1_HH */
