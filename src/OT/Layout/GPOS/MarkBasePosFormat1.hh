#ifndef OT_LAYOUT_GPOS_MARKBASEPOSFORMAT1_HH
#define OT_LAYOUT_GPOS_MARKBASEPOSFORMAT1_HH

#अन्तर्भाव "MarkArray.hh"

नामाकाश OT {
नामाकाश Layout {
नामाकाश GPOS_impl {

ढाँचा <typename Types>
संरचना MarkBasePosFormat1_2
{
  संरक्षित:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  typename Types::ढाँचा LOffsetTo<Coverage>
                markCoverage;           /* Offset to MarkCoverage table--from
                                         * beginning of MarkBasePos subtable */
  typename Types::ढाँचा LOffsetTo<Coverage>
                baseCoverage;           /* Offset to BaseCoverage table--from
                                         * beginning of MarkBasePos subtable */
  HBUINT16      class_मोजणी_2;             /* Number of classes defined for marks */
  typename Types::ढाँचा LOffsetTo<MarkArray<Types>>
                markArray;              /* Offset to MarkArray table--from
                                         * beginning of MarkBasePos subtable */
  typename Types::ढाँचा LOffsetTo<AnchorMatrix<Types>>
                baseArray;              /* Offset to BaseArray table--from
                                         * beginning of MarkBasePos subtable */

  सार्वजनिक:
  DEFINE_SIZE_STATIC (4 + 4 * Types::LOffset::static_size);

    तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे) &&
                  markCoverage.sanitize (c, हे) &&
                  baseCoverage.sanitize (c, हे) &&
                  markArray.sanitize (c, हे) &&
                  baseArray.sanitize (c, हे, (अचिन्हित पूर्णांक) class_मोजणी_2));
  }

  तार्किक intersects (स्थिरांक hb_set_t *glyphs) स्थिरांक
  {
    परत (हे+markCoverage).intersects (glyphs) &&
           (हे+baseCoverage).intersects (glyphs);
  }

  शून्य closure_lookups (hb_closure_lookups_context_t *c) स्थिरांक {}

  शून्य collect_variation_indices (hb_collect_variation_indices_context_t *c) स्थिरांक
  {
    (हे+markArray).collect_variation_indices (c, हे+markCoverage);

    hb_map_t klass_mapping;
    Markclass_closure_and_remap_indexes (हे+markCoverage, हे+markArray, *c->glyph_set, &klass_mapping);

    hb_sorted_vector_t<अचिन्हित> base_indexes;
    collect_anchor_indexes (हे+baseCoverage, (हे+baseArray).rows,
                            class_मोजणी_2, *c->glyph_set, klass_mapping,
                            base_indexes);
    (हे+baseArray).collect_variation_indices (c, base_indexes.iter ());
  }

  शून्य collect_glyphs (hb_collect_glyphs_context_t *c) स्थिरांक
  {
    जर (unlikely (!(हे+markCoverage).collect_coverage (c->आदान))) परत;
    जर (unlikely (!(हे+baseCoverage).collect_coverage (c->आदान))) परत;
  }

  शून्य collect_second_glyphs (hb_set_digest_t *digest) स्थिरांक
  { (हे+baseCoverage).collect_coverage (digest); }

  स्थिरांक Coverage &get_coverage () स्थिरांक { परत हे+markCoverage; }

  स्थिर ओळीत तार्किक accept (hb_buffer_t *buffer, अचिन्हित idx)
  {
    /* We only want to attach to the first of a MultipleSubst sequence.
     * https://github.com/harfbuzz/harfbuzz/issues/740
     * Reject others...
     * ...but stop if we find a mark in the MultipleSubst sequence:
     * https://github.com/harfbuzz/harfbuzz/issues/1020 */
    परत !_hb_glyph_info_multiplied (&buffer->info[idx]) ||
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

  तार्किक apply (hb_ot_apply_context_t *c) स्थिरांक
  {
    TRACE_APPLY (हे);
    hb_buffer_t *buffer = c->buffer;
    अचिन्हित पूर्णांक mark_index = (हे+markCoverage).get_coverage  (buffer->cur().codepoint);
    जर (likely (mark_index == NOT_COVERED)) return_trace (असत्य);

    /* Now we search backwards for a non-mark glyph.
     * We don't use skippy_iter.prev() to avoid O(n^2) behavior. */

    स्वतः &skippy_iter = c->iter_आदान;
    skippy_iter.set_lookup_props (LookupFlag::IgnoreMarks);

    जर (c->last_base_until > buffer->idx)
    {
      c->last_base_until = 0;
      c->last_base = -1;
    }
    अचिन्हित j;
    साठी (j = buffer->idx; j > c->last_base_until; j--)
    {
      स्वतः match = skippy_iter.match (buffer->info[j - 1]);
      जर (match == skippy_iter.MATCH)
      {
        // https://github.com/harfbuzz/harfbuzz/issues/4124
	जर (!accept (buffer, j - 1) &&
	    NOT_COVERED == (हे+baseCoverage).get_coverage  (buffer->info[j - 1].codepoint))
	  match = skippy_iter.SKIP;
      }
      जर (match == skippy_iter.MATCH)
      {
	c->last_base = (चिन्हित) j - 1;
	थांबा;
      }
    }
    c->last_base_until = buffer->idx;
    जर (c->last_base == -1)
    {
      buffer->unsafe_to_concat_from_outbuffer (0, buffer->idx + 1);
      return_trace (असत्य);
    }

    अचिन्हित idx = (अचिन्हित) c->last_base;

    /* Checking that matched glyph is actually a base glyph by GDEF is too strong; disabled */
    //if (!_hb_glyph_info_is_base_glyph (&buffer->info[idx])) { return_trace (false); }

    अचिन्हित पूर्णांक base_index = (हे+baseCoverage).get_coverage  (buffer->info[idx].codepoint);
    जर (base_index == NOT_COVERED)
    {
      buffer->unsafe_to_concat_from_outbuffer (idx, buffer->idx + 1);
      return_trace (असत्य);
    }

    return_trace ((हे+markArray).apply (c, mark_index, base_index, हे+baseArray, class_मोजणी_2, idx));
  }

  तार्किक subset (hb_subset_context_t *c) स्थिरांक
  {
    TRACE_SUBSET (हे);
    स्थिरांक hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    स्थिरांक hb_map_t &glyph_map = *c->plan->glyph_map;

    स्वतः *out = c->serializer->start_embed (*हे);
    जर (unlikely (!c->serializer->extend_min (out))) return_trace (असत्य);
    out->format = format;

    hb_map_t klass_mapping;
    Markclass_closure_and_remap_indexes (हे+markCoverage, हे+markArray, glyphset, &klass_mapping);

    जर (!klass_mapping.get_population ()) return_trace (असत्य);
    out->class_मोजणी_2 = klass_mapping.get_population ();

    स्वतः mark_iter =
    + hb_zip (हे+markCoverage, हे+markArray)
    | hb_filter (glyphset, hb_first)
    ;

    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + mark_iter
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    जर (!out->markCoverage.serialize_serialize (c->serializer, new_coverage.iter ()))
      return_trace (असत्य);

    जर (unlikely (!out->markArray.serialize_subset (c, markArray, हे,
						    (हे+markCoverage).iter (),
						    &klass_mapping)))
      return_trace (असत्य);

    new_coverage.reset ();
    hb_sorted_vector_t<अचिन्हित> base_indexes;
    स्थिरांक स्वतः &base_array = हे+baseArray;
    collect_subset_anchor_indexes (हे+baseCoverage, base_array.rows,
                                   class_मोजणी_2, base_array.matrixZ.arrayZ,
                                   glyphset, glyph_map, klass_mapping,
                                   new_coverage, base_indexes);

    जर (!new_coverage) return_trace (असत्य);
    जर (!out->baseCoverage.serialize_serialize (c->serializer, new_coverage.iter ()))
      return_trace (असत्य);

    return_trace (out->baseArray.serialize_subset (c, baseArray, हे,
						   new_coverage.length,
						   base_indexes.iter ()));
  }
};


}
}
}

#endif /* OT_LAYOUT_GPOS_MARKBASEPOSFORMAT1_HH */
