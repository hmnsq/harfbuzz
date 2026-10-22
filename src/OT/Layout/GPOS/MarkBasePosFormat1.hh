#ifndef OT_LAYOUT_GPOS_MARKBASEPOSFORMAT1_HH
#define OT_LAYOUT_GPOS_MARKBASEPOSFORMAT1_HH

#அந்தர்பாவ "MarkArray.hh"

நாமாகாஶ OT {
நாமாகாஶ Layout {
நாமாகாஶ GPOS_impl {

template <typename Types>
கட்டமைப்பு MarkBasePosFormat1_2
{
  பாதுகாக்கப்பட்ட:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  typename Types::template LOffsetTo<Coverage>
                markCoverage;           /* Offset to MarkCoverage table--from
                                         * beginning of MarkBasePos subtable */
  typename Types::template LOffsetTo<Coverage>
                baseCoverage;           /* Offset to BaseCoverage table--from
                                         * beginning of MarkBasePos subtable */
  HBUINT16      class_மொத்தம்_2;             /* Number of classes defined for marks */
  typename Types::template LOffsetTo<MarkArray<Types>>
                markArray;              /* Offset to MarkArray table--from
                                         * beginning of MarkBasePos subtable */
  typename Types::template LOffsetTo<AnchorMatrix<Types>>
                baseArray;              /* Offset to BaseArray table--from
                                         * beginning of MarkBasePos subtable */

  பொது:
  DEFINE_SIZE_STATIC (4 + 4 * Types::LOffset::static_size);

    தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது) &&
                  markCoverage.sanitize (c, இது) &&
                  baseCoverage.sanitize (c, இது) &&
                  markArray.sanitize (c, இது) &&
                  baseArray.sanitize (c, இது, (unsigned முழு) class_மொத்தம்_2));
  }

  தருக்க intersects (மாறிலி hb_set_t *glyphs) மாறிலி
  {
    திரும்பு (இது+markCoverage).intersects (glyphs) &&
           (இது+baseCoverage).intersects (glyphs);
  }

  வெறுமை closure_lookups (hb_closure_lookups_context_t *c) மாறிலி {}

  வெறுமை collect_variation_indices (hb_collect_variation_indices_context_t *c) மாறிலி
  {
    (இது+markArray).collect_variation_indices (c, இது+markCoverage);

    hb_map_t klass_mapping;
    Markclass_closure_and_remap_indexes (இது+markCoverage, இது+markArray, *c->glyph_set, &klass_mapping);

    hb_sorted_vector_t<unsigned> base_indexes;
    collect_anchor_indexes (இது+baseCoverage, (இது+baseArray).rows,
                            class_மொத்தம்_2, *c->glyph_set, klass_mapping,
                            base_indexes);
    (இது+baseArray).collect_variation_indices (c, base_indexes.iter ());
  }

  வெறுமை collect_glyphs (hb_collect_glyphs_context_t *c) மாறிலி
  {
    எனில் (unlikely (!(இது+markCoverage).collect_coverage (c->உள்ளீடு))) திரும்பு;
    எனில் (unlikely (!(இது+baseCoverage).collect_coverage (c->உள்ளீடு))) திரும்பு;
  }

  வெறுமை collect_second_glyphs (hb_set_digest_t *digest) மாறிலி
  { (இது+baseCoverage).collect_coverage (digest); }

  மாறிலி Coverage &get_coverage () மாறிலி { திரும்பு இது+markCoverage; }

  நிலையான உள்வரி தருக்க accept (hb_buffer_t *buffer, unsigned idx)
  {
    /* We only want to attach to the first of a MultipleSubst sequence.
     * https://github.com/harfbuzz/harfbuzz/issues/740
     * Reject others...
     * ...but stop if we find a mark in the MultipleSubst sequence:
     * https://github.com/harfbuzz/harfbuzz/issues/1020 */
    திரும்பு !_hb_glyph_info_multiplied (&buffer->info[idx]) ||
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

  தருக்க apply (hb_ot_apply_context_t *c) மாறிலி
  {
    TRACE_APPLY (இது);
    hb_buffer_t *buffer = c->buffer;
    unsigned முழு mark_index = (இது+markCoverage).get_coverage  (buffer->cur().codepoint);
    எனில் (likely (mark_index == NOT_COVERED)) return_trace (பொய்);

    /* Now we search backwards for a non-mark glyph.
     * We don't use skippy_iter.prev() to avoid O(n^2) behavior. */

    auto &skippy_iter = c->iter_உள்ளீடு;
    skippy_iter.set_lookup_props (LookupFlag::IgnoreMarks);

    எனில் (c->last_base_until > buffer->idx)
    {
      c->last_base_until = 0;
      c->last_base = -1;
    }
    unsigned j;
    க்கு (j = buffer->idx; j > c->last_base_until; j--)
    {
      auto match = skippy_iter.match (buffer->info[j - 1]);
      எனில் (match == skippy_iter.MATCH)
      {
        // https://github.com/harfbuzz/harfbuzz/issues/4124
	எனில் (!accept (buffer, j - 1) &&
	    NOT_COVERED == (இது+baseCoverage).get_coverage  (buffer->info[j - 1].codepoint))
	  match = skippy_iter.SKIP;
      }
      எனில் (match == skippy_iter.MATCH)
      {
	c->last_base = (signed) j - 1;
	நிறுத்து;
      }
    }
    c->last_base_until = buffer->idx;
    எனில் (c->last_base == -1)
    {
      buffer->unsafe_to_concat_from_outbuffer (0, buffer->idx + 1);
      return_trace (பொய்);
    }

    unsigned idx = (unsigned) c->last_base;

    /* Checking that matched glyph is actually a base glyph by GDEF is too strong; disabled */
    //if (!_hb_glyph_info_is_base_glyph (&buffer->info[idx])) { return_trace (false); }

    unsigned முழு base_index = (இது+baseCoverage).get_coverage  (buffer->info[idx].codepoint);
    எனில் (base_index == NOT_COVERED)
    {
      buffer->unsafe_to_concat_from_outbuffer (idx, buffer->idx + 1);
      return_trace (பொய்);
    }

    return_trace ((இது+markArray).apply (c, mark_index, base_index, இது+baseArray, class_மொத்தம்_2, idx));
  }

  தருக்க subset (hb_subset_context_t *c) மாறிலி
  {
    TRACE_SUBSET (இது);
    மாறிலி hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    மாறிலி hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*இது);
    எனில் (unlikely (!c->serializer->extend_min (out))) return_trace (பொய்);
    out->format = format;

    hb_map_t klass_mapping;
    Markclass_closure_and_remap_indexes (இது+markCoverage, இது+markArray, glyphset, &klass_mapping);

    எனில் (!klass_mapping.get_population ()) return_trace (பொய்);
    out->class_மொத்தம்_2 = klass_mapping.get_population ();

    auto mark_iter =
    + hb_zip (இது+markCoverage, இது+markArray)
    | hb_filter (glyphset, hb_first)
    ;

    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + mark_iter
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    எனில் (!out->markCoverage.serialize_serialize (c->serializer, new_coverage.iter ()))
      return_trace (பொய்);

    எனில் (unlikely (!out->markArray.serialize_subset (c, markArray, இது,
						    (இது+markCoverage).iter (),
						    &klass_mapping)))
      return_trace (பொய்);

    new_coverage.reset ();
    hb_sorted_vector_t<unsigned> base_indexes;
    மாறிலி auto &base_array = இது+baseArray;
    collect_subset_anchor_indexes (இது+baseCoverage, base_array.rows,
                                   class_மொத்தம்_2, base_array.matrixZ.arrayZ,
                                   glyphset, glyph_map, klass_mapping,
                                   new_coverage, base_indexes);

    எனில் (!new_coverage) return_trace (பொய்);
    எனில் (!out->baseCoverage.serialize_serialize (c->serializer, new_coverage.iter ()))
      return_trace (பொய்);

    return_trace (out->baseArray.serialize_subset (c, baseArray, இது,
						   new_coverage.length,
						   base_indexes.iter ()));
  }
};


}
}
}

#endif /* OT_LAYOUT_GPOS_MARKBASEPOSFORMAT1_HH */
