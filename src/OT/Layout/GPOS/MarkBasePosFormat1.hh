#ifndef OT_LAYOUT_GPOS_MARKBASEPOSFORMAT1_HH
#define OT_LAYOUT_GPOS_MARKBASEPOSFORMAT1_HH

#ಅನ್ತರ್ಭಾವ "MarkArray.hh"

ನಾಮಾಕಾಶ OT {
ನಾಮಾಕಾಶ Layout {
ನಾಮಾಕಾಶ GPOS_impl {

template <typename Types>
ಸಂರಚನಾ MarkBasePosFormat1_2
{
  ರಕ್ಷಿತ:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  typename Types::template LOffsetTo<Coverage>
                markCoverage;           /* Offset to MarkCoverage table--from
                                         * beginning of MarkBasePos subtable */
  typename Types::template LOffsetTo<Coverage>
                baseCoverage;           /* Offset to BaseCoverage table--from
                                         * beginning of MarkBasePos subtable */
  HBUINT16      class_ಎಣಿಕೆ_2;             /* Number of classes defined for marks */
  typename Types::template LOffsetTo<MarkArray<Types>>
                markArray;              /* Offset to MarkArray table--from
                                         * beginning of MarkBasePos subtable */
  typename Types::template LOffsetTo<AnchorMatrix<Types>>
                baseArray;              /* Offset to BaseArray table--from
                                         * beginning of MarkBasePos subtable */

  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (4 + 4 * Types::LOffset::static_size);

    ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್) &&
                  markCoverage.sanitize (c, ಏತತ್) &&
                  baseCoverage.sanitize (c, ಏತತ್) &&
                  markArray.sanitize (c, ಏತತ್) &&
                  baseArray.sanitize (c, ಏತತ್, (unsigned ಪೂರ್ಣಾಙ್ಕ) class_ಎಣಿಕೆ_2));
  }

  ತಾರ್ಕಿಕ intersects (ಧ್ರುವ hb_set_t *glyphs) ಧ್ರುವ
  {
    ಪ್ರತ್ಯಾಗಮ (ಏತತ್+markCoverage).intersects (glyphs) &&
           (ಏತತ್+baseCoverage).intersects (glyphs);
  }

  ಶೂನ್ಯ closure_lookups (hb_closure_lookups_context_t *c) ಧ್ರುವ {}

  ಶೂನ್ಯ collect_variation_indices (hb_collect_variation_indices_context_t *c) ಧ್ರುವ
  {
    (ಏತತ್+markArray).collect_variation_indices (c, ಏತತ್+markCoverage);

    hb_map_t klass_mapping;
    Markclass_closure_and_remap_indexes (ಏತತ್+markCoverage, ಏತತ್+markArray, *c->glyph_set, &klass_mapping);

    hb_sorted_vector_t<unsigned> base_indexes;
    collect_anchor_indexes (ಏತತ್+baseCoverage, (ಏತತ್+baseArray).rows,
                            class_ಎಣಿಕೆ_2, *c->glyph_set, klass_mapping,
                            base_indexes);
    (ಏತತ್+baseArray).collect_variation_indices (c, base_indexes.iter ());
  }

  ಶೂನ್ಯ collect_glyphs (hb_collect_glyphs_context_t *c) ಧ್ರುವ
  {
    ಯದಿ (unlikely (!(ಏತತ್+markCoverage).collect_coverage (c->ಒಳಹರಿವು))) ಪ್ರತ್ಯಾಗಮ;
    ಯದಿ (unlikely (!(ಏತತ್+baseCoverage).collect_coverage (c->ಒಳಹರಿವು))) ಪ್ರತ್ಯಾಗಮ;
  }

  ಶೂನ್ಯ collect_second_glyphs (hb_set_digest_t *digest) ಧ್ರುವ
  { (ಏತತ್+baseCoverage).collect_coverage (digest); }

  ಧ್ರುವ Coverage &get_coverage () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ ಏತತ್+markCoverage; }

  ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ತಾರ್ಕಿಕ accept (hb_buffer_t *buffer, unsigned idx)
  {
    /* We only want to attach to the first of a MultipleSubst sequence.
     * https://github.com/harfbuzz/harfbuzz/issues/740
     * Reject others...
     * ...but stop if we find a mark in the MultipleSubst sequence:
     * https://github.com/harfbuzz/harfbuzz/issues/1020 */
    ಪ್ರತ್ಯಾಗಮ !_hb_glyph_info_multiplied (&buffer->info[idx]) ||
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

  ತಾರ್ಕಿಕ apply (hb_ot_apply_context_t *c) ಧ್ರುವ
  {
    TRACE_APPLY (ಏತತ್);
    hb_buffer_t *buffer = c->buffer;
    unsigned ಪೂರ್ಣಾಙ್ಕ mark_index = (ಏತತ್+markCoverage).get_coverage  (buffer->cur().codepoint);
    ಯದಿ (likely (mark_index == NOT_COVERED)) return_trace (ಅಸತ್ಯ);

    /* Now we search backwards for a non-mark glyph.
     * We don't use skippy_iter.prev() to avoid O(n^2) behavior. */

    auto &skippy_iter = c->iter_ಒಳಹರಿವು;
    skippy_iter.set_lookup_props (LookupFlag::IgnoreMarks);

    ಯದಿ (c->last_base_until > buffer->idx)
    {
      c->last_base_until = 0;
      c->last_base = -1;
    }
    unsigned j;
    ಪ್ರತಿ (j = buffer->idx; j > c->last_base_until; j--)
    {
      auto match = skippy_iter.match (buffer->info[j - 1]);
      ಯದಿ (match == skippy_iter.MATCH)
      {
        // https://github.com/harfbuzz/harfbuzz/issues/4124
	ಯದಿ (!accept (buffer, j - 1) &&
	    NOT_COVERED == (ಏತತ್+baseCoverage).get_coverage  (buffer->info[j - 1].codepoint))
	  match = skippy_iter.SKIP;
      }
      ಯದಿ (match == skippy_iter.MATCH)
      {
	c->last_base = (signed) j - 1;
	ಭಙ್ಗ;
      }
    }
    c->last_base_until = buffer->idx;
    ಯದಿ (c->last_base == -1)
    {
      buffer->unsafe_to_concat_from_outbuffer (0, buffer->idx + 1);
      return_trace (ಅಸತ್ಯ);
    }

    unsigned idx = (unsigned) c->last_base;

    /* Checking that matched glyph is actually a base glyph by GDEF is too strong; disabled */
    //if (!_hb_glyph_info_is_base_glyph (&buffer->info[idx])) { return_trace (false); }

    unsigned ಪೂರ್ಣಾಙ್ಕ base_index = (ಏತತ್+baseCoverage).get_coverage  (buffer->info[idx].codepoint);
    ಯದಿ (base_index == NOT_COVERED)
    {
      buffer->unsafe_to_concat_from_outbuffer (idx, buffer->idx + 1);
      return_trace (ಅಸತ್ಯ);
    }

    return_trace ((ಏತತ್+markArray).apply (c, mark_index, base_index, ಏತತ್+baseArray, class_ಎಣಿಕೆ_2, idx));
  }

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    ಧ್ರುವ hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ಧ್ರುವ hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*ಏತತ್);
    ಯದಿ (unlikely (!c->serializer->extend_min (out))) return_trace (ಅಸತ್ಯ);
    out->format = format;

    hb_map_t klass_mapping;
    Markclass_closure_and_remap_indexes (ಏತತ್+markCoverage, ಏತತ್+markArray, glyphset, &klass_mapping);

    ಯದಿ (!klass_mapping.get_population ()) return_trace (ಅಸತ್ಯ);
    out->class_ಎಣಿಕೆ_2 = klass_mapping.get_population ();

    auto mark_iter =
    + hb_zip (ಏತತ್+markCoverage, ಏತತ್+markArray)
    | hb_filter (glyphset, hb_first)
    ;

    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + mark_iter
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    ಯದಿ (!out->markCoverage.serialize_serialize (c->serializer, new_coverage.iter ()))
      return_trace (ಅಸತ್ಯ);

    ಯದಿ (unlikely (!out->markArray.serialize_subset (c, markArray, ಏತತ್,
						    (ಏತತ್+markCoverage).iter (),
						    &klass_mapping)))
      return_trace (ಅಸತ್ಯ);

    new_coverage.reset ();
    hb_sorted_vector_t<unsigned> base_indexes;
    ಧ್ರುವ auto &base_array = ಏತತ್+baseArray;
    collect_subset_anchor_indexes (ಏತತ್+baseCoverage, base_array.rows,
                                   class_ಎಣಿಕೆ_2, base_array.matrixZ.arrayZ,
                                   glyphset, glyph_map, klass_mapping,
                                   new_coverage, base_indexes);

    ಯದಿ (!new_coverage) return_trace (ಅಸತ್ಯ);
    ಯದಿ (!out->baseCoverage.serialize_serialize (c->serializer, new_coverage.iter ()))
      return_trace (ಅಸತ್ಯ);

    return_trace (out->baseArray.serialize_subset (c, baseArray, ಏತತ್,
						   new_coverage.length,
						   base_indexes.iter ()));
  }
};


}
}
}

#endif /* OT_LAYOUT_GPOS_MARKBASEPOSFORMAT1_HH */
