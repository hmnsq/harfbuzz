#ifndef OT_LAYOUT_GPOS_MARKBASEPOSFORMAT1_HH
#define OT_LAYOUT_GPOS_MARKBASEPOSFORMAT1_HH

#ଅନ୍ତର୍ଭାଵ "MarkArray.hh"

ନାମାକାଶ OT {
ନାମାକାଶ Layout {
ନାମାକାଶ GPOS_impl {

template <typename Types>
ସଂରଚନା MarkBasePosFormat1_2
{
  ସୁରକ୍ଷିତ:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  typename Types::template LOffsetTo<Coverage>
                markCoverage;           /* Offset to MarkCoverage table--from
                                         * beginning of MarkBasePos subtable */
  typename Types::template LOffsetTo<Coverage>
                baseCoverage;           /* Offset to BaseCoverage table--from
                                         * beginning of MarkBasePos subtable */
  HBUINT16      class_ଗଣନା_2;             /* Number of classes defined for marks */
  typename Types::template LOffsetTo<MarkArray<Types>>
                markArray;              /* Offset to MarkArray table--from
                                         * beginning of MarkBasePos subtable */
  typename Types::template LOffsetTo<AnchorMatrix<Types>>
                baseArray;              /* Offset to BaseArray table--from
                                         * beginning of MarkBasePos subtable */

  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (4 + 4 * Types::LOffset::static_size);

    ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି) &&
                  markCoverage.sanitize (c, ଏହି) &&
                  baseCoverage.sanitize (c, ଏହି) &&
                  markArray.sanitize (c, ଏହି) &&
                  baseArray.sanitize (c, ଏହି, (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା) class_ଗଣନା_2));
  }

  ତାର୍କିକ intersects (ଅଚଳ hb_set_t *glyphs) ଅଚଳ
  {
    ଫେରନ୍ତୁ (ଏହି+markCoverage).intersects (glyphs) &&
           (ଏହି+baseCoverage).intersects (glyphs);
  }

  ଶୂନ୍ୟ closure_lookups (hb_closure_lookups_context_t *c) ଅଚଳ {}

  ଶୂନ୍ୟ collect_variation_indices (hb_collect_variation_indices_context_t *c) ଅଚଳ
  {
    (ଏହି+markArray).collect_variation_indices (c, ଏହି+markCoverage);

    hb_map_t klass_mapping;
    Markclass_closure_and_remap_indexes (ଏହି+markCoverage, ଏହି+markArray, *c->glyph_set, &klass_mapping);

    hb_sorted_vector_t<unsigned> base_indexes;
    collect_anchor_indexes (ଏହି+baseCoverage, (ଏହି+baseArray).rows,
                            class_ଗଣନା_2, *c->glyph_set, klass_mapping,
                            base_indexes);
    (ଏହି+baseArray).collect_variation_indices (c, base_indexes.iter ());
  }

  ଶୂନ୍ୟ collect_glyphs (hb_collect_glyphs_context_t *c) ଅଚଳ
  {
    ଯଦି (unlikely (!(ଏହି+markCoverage).collect_coverage (c->ନିବେଶ))) ଫେରନ୍ତୁ;
    ଯଦି (unlikely (!(ଏହି+baseCoverage).collect_coverage (c->ନିବେଶ))) ଫେରନ୍ତୁ;
  }

  ଶୂନ୍ୟ collect_second_glyphs (hb_set_digest_t *digest) ଅଚଳ
  { (ଏହି+baseCoverage).collect_coverage (digest); }

  ଅଚଳ Coverage &get_coverage () ଅଚଳ { ଫେରନ୍ତୁ ଏହି+markCoverage; }

  ସ୍ଥିର ଧାଡ଼ିଭିତରେ ତାର୍କିକ accept (hb_buffer_t *buffer, unsigned idx)
  {
    /* We only want to attach to the first of a MultipleSubst sequence.
     * https://github.com/harfbuzz/harfbuzz/issues/740
     * Reject others...
     * ...but stop if we find a mark in the MultipleSubst sequence:
     * https://github.com/harfbuzz/harfbuzz/issues/1020 */
    ଫେରନ୍ତୁ !_hb_glyph_info_multiplied (&buffer->info[idx]) ||
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

  ତାର୍କିକ apply (hb_ot_apply_context_t *c) ଅଚଳ
  {
    TRACE_APPLY (ଏହି);
    hb_buffer_t *buffer = c->buffer;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା mark_index = (ଏହି+markCoverage).get_coverage  (buffer->cur().codepoint);
    ଯଦି (likely (mark_index == NOT_COVERED)) return_trace (ମିଥ୍ୟା);

    /* Now we search backwards for a non-mark glyph.
     * We don't use skippy_iter.prev() to avoid O(n^2) behavior. */

    auto &skippy_iter = c->iter_ନିବେଶ;
    skippy_iter.set_lookup_props (LookupFlag::IgnoreMarks);

    ଯଦି (c->last_base_until > buffer->idx)
    {
      c->last_base_until = 0;
      c->last_base = -1;
    }
    unsigned j;
    ପାଇଁ (j = buffer->idx; j > c->last_base_until; j--)
    {
      auto match = skippy_iter.match (buffer->info[j - 1]);
      ଯଦି (match == skippy_iter.MATCH)
      {
        // https://github.com/harfbuzz/harfbuzz/issues/4124
	ଯଦି (!accept (buffer, j - 1) &&
	    NOT_COVERED == (ଏହି+baseCoverage).get_coverage  (buffer->info[j - 1].codepoint))
	  match = skippy_iter.SKIP;
      }
      ଯଦି (match == skippy_iter.MATCH)
      {
	c->last_base = (signed) j - 1;
	ଭାଙ୍ଗ;
      }
    }
    c->last_base_until = buffer->idx;
    ଯଦି (c->last_base == -1)
    {
      buffer->unsafe_to_concat_from_outbuffer (0, buffer->idx + 1);
      return_trace (ମିଥ୍ୟା);
    }

    unsigned idx = (unsigned) c->last_base;

    /* Checking that matched glyph is actually a base glyph by GDEF is too strong; disabled */
    //if (!_hb_glyph_info_is_base_glyph (&buffer->info[idx])) { return_trace (false); }

    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା base_index = (ଏହି+baseCoverage).get_coverage  (buffer->info[idx].codepoint);
    ଯଦି (base_index == NOT_COVERED)
    {
      buffer->unsafe_to_concat_from_outbuffer (idx, buffer->idx + 1);
      return_trace (ମିଥ୍ୟା);
    }

    return_trace ((ଏହି+markArray).apply (c, mark_index, base_index, ଏହି+baseArray, class_ଗଣନା_2, idx));
  }

  ତାର୍କିକ subset (hb_subset_context_t *c) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    ଅଚଳ hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ଅଚଳ hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*ଏହି);
    ଯଦି (unlikely (!c->serializer->extend_min (out))) return_trace (ମିଥ୍ୟା);
    out->format = format;

    hb_map_t klass_mapping;
    Markclass_closure_and_remap_indexes (ଏହି+markCoverage, ଏହି+markArray, glyphset, &klass_mapping);

    ଯଦି (!klass_mapping.get_population ()) return_trace (ମିଥ୍ୟା);
    out->class_ଗଣନା_2 = klass_mapping.get_population ();

    auto mark_iter =
    + hb_zip (ଏହି+markCoverage, ଏହି+markArray)
    | hb_filter (glyphset, hb_first)
    ;

    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + mark_iter
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    ଯଦି (!out->markCoverage.serialize_serialize (c->serializer, new_coverage.iter ()))
      return_trace (ମିଥ୍ୟା);

    ଯଦି (unlikely (!out->markArray.serialize_subset (c, markArray, ଏହି,
						    (ଏହି+markCoverage).iter (),
						    &klass_mapping)))
      return_trace (ମିଥ୍ୟା);

    new_coverage.reset ();
    hb_sorted_vector_t<unsigned> base_indexes;
    ଅଚଳ auto &base_array = ଏହି+baseArray;
    collect_subset_anchor_indexes (ଏହି+baseCoverage, base_array.rows,
                                   class_ଗଣନା_2, base_array.matrixZ.arrayZ,
                                   glyphset, glyph_map, klass_mapping,
                                   new_coverage, base_indexes);

    ଯଦି (!new_coverage) return_trace (ମିଥ୍ୟା);
    ଯଦି (!out->baseCoverage.serialize_serialize (c->serializer, new_coverage.iter ()))
      return_trace (ମିଥ୍ୟା);

    return_trace (out->baseArray.serialize_subset (c, baseArray, ଏହି,
						   new_coverage.length,
						   base_indexes.iter ()));
  }
};


}
}
}

#endif /* OT_LAYOUT_GPOS_MARKBASEPOSFORMAT1_HH */
