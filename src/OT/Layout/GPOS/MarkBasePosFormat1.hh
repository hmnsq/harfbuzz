#ifndef OT_LAYOUT_GPOS_MARKBASEPOSFORMAT1_HH
#define OT_LAYOUT_GPOS_MARKBASEPOSFORMAT1_HH

#അന്തര്ഭാവ "MarkArray.hh"

നാമാകാശ OT {
നാമാകാശ Layout {
നാമാകാശ GPOS_impl {

template <typename Types>
ഘടന MarkBasePosFormat1_2
{
  സംരക്ഷിത:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  typename Types::template LOffsetTo<Coverage>
                markCoverage;           /* Offset to MarkCoverage table--from
                                         * beginning of MarkBasePos subtable */
  typename Types::template LOffsetTo<Coverage>
                baseCoverage;           /* Offset to BaseCoverage table--from
                                         * beginning of MarkBasePos subtable */
  HBUINT16      class_എണ്ണം_2;             /* Number of classes defined for marks */
  typename Types::template LOffsetTo<MarkArray<Types>>
                markArray;              /* Offset to MarkArray table--from
                                         * beginning of MarkBasePos subtable */
  typename Types::template LOffsetTo<AnchorMatrix<Types>>
                baseArray;              /* Offset to BaseArray table--from
                                         * beginning of MarkBasePos subtable */

  പൊതു:
  DEFINE_SIZE_STATIC (4 + 4 * Types::LOffset::static_size);

    തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_struct (ഇത്) &&
                  markCoverage.sanitize (c, ഇത്) &&
                  baseCoverage.sanitize (c, ഇത്) &&
                  markArray.sanitize (c, ഇത്) &&
                  baseArray.sanitize (c, ഇത്, (unsigned പൂർണ്ണസംഖ്യ) class_എണ്ണം_2));
  }

  തർക്കം intersects (സ്ഥിരം hb_set_t *glyphs) സ്ഥിരം
  {
    തിരികെ (ഇത്+markCoverage).intersects (glyphs) &&
           (ഇത്+baseCoverage).intersects (glyphs);
  }

  ശൂന്യം closure_lookups (hb_closure_lookups_context_t *c) സ്ഥിരം {}

  ശൂന്യം collect_variation_indices (hb_collect_variation_indices_context_t *c) സ്ഥിരം
  {
    (ഇത്+markArray).collect_variation_indices (c, ഇത്+markCoverage);

    hb_map_t klass_mapping;
    Markclass_closure_and_remap_indexes (ഇത്+markCoverage, ഇത്+markArray, *c->glyph_set, &klass_mapping);

    hb_sorted_vector_t<unsigned> base_indexes;
    collect_anchor_indexes (ഇത്+baseCoverage, (ഇത്+baseArray).rows,
                            class_എണ്ണം_2, *c->glyph_set, klass_mapping,
                            base_indexes);
    (ഇത്+baseArray).collect_variation_indices (c, base_indexes.iter ());
  }

  ശൂന്യം collect_glyphs (hb_collect_glyphs_context_t *c) സ്ഥിരം
  {
    എങ്കിൽ (unlikely (!(ഇത്+markCoverage).collect_coverage (c->നിവേശം))) തിരികെ;
    എങ്കിൽ (unlikely (!(ഇത്+baseCoverage).collect_coverage (c->നിവേശം))) തിരികെ;
  }

  ശൂന്യം collect_second_glyphs (hb_set_digest_t *digest) സ്ഥിരം
  { (ഇത്+baseCoverage).collect_coverage (digest); }

  സ്ഥിരം Coverage &get_coverage () സ്ഥിരം { തിരികെ ഇത്+markCoverage; }

  സ്ഥിതം വരിയിൽ തർക്കം accept (hb_buffer_t *buffer, unsigned idx)
  {
    /* We only want to attach to the first of a MultipleSubst sequence.
     * https://github.com/harfbuzz/harfbuzz/issues/740
     * Reject others...
     * ...but stop if we find a mark in the MultipleSubst sequence:
     * https://github.com/harfbuzz/harfbuzz/issues/1020 */
    തിരികെ !_hb_glyph_info_multiplied (&buffer->info[idx]) ||
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

  തർക്കം apply (hb_ot_apply_context_t *c) സ്ഥിരം
  {
    TRACE_APPLY (ഇത്);
    hb_buffer_t *buffer = c->buffer;
    unsigned പൂർണ്ണസംഖ്യ mark_index = (ഇത്+markCoverage).get_coverage  (buffer->cur().codepoint);
    എങ്കിൽ (likely (mark_index == NOT_COVERED)) return_trace (അസത്യം);

    /* Now we search backwards for a non-mark glyph.
     * We don't use skippy_iter.prev() to avoid O(n^2) behavior. */

    auto &skippy_iter = c->iter_നിവേശം;
    skippy_iter.set_lookup_props (LookupFlag::IgnoreMarks);

    എങ്കിൽ (c->last_base_until > buffer->idx)
    {
      c->last_base_until = 0;
      c->last_base = -1;
    }
    unsigned j;
    വേണ്ടി (j = buffer->idx; j > c->last_base_until; j--)
    {
      auto match = skippy_iter.match (buffer->info[j - 1]);
      എങ്കിൽ (match == skippy_iter.MATCH)
      {
        // https://github.com/harfbuzz/harfbuzz/issues/4124
	എങ്കിൽ (!accept (buffer, j - 1) &&
	    NOT_COVERED == (ഇത്+baseCoverage).get_coverage  (buffer->info[j - 1].codepoint))
	  match = skippy_iter.SKIP;
      }
      എങ്കിൽ (match == skippy_iter.MATCH)
      {
	c->last_base = (signed) j - 1;
	നിർത്തുക;
      }
    }
    c->last_base_until = buffer->idx;
    എങ്കിൽ (c->last_base == -1)
    {
      buffer->unsafe_to_concat_from_outbuffer (0, buffer->idx + 1);
      return_trace (അസത്യം);
    }

    unsigned idx = (unsigned) c->last_base;

    /* Checking that matched glyph is actually a base glyph by GDEF is too strong; disabled */
    //if (!_hb_glyph_info_is_base_glyph (&buffer->info[idx])) { return_trace (false); }

    unsigned പൂർണ്ണസംഖ്യ base_index = (ഇത്+baseCoverage).get_coverage  (buffer->info[idx].codepoint);
    എങ്കിൽ (base_index == NOT_COVERED)
    {
      buffer->unsafe_to_concat_from_outbuffer (idx, buffer->idx + 1);
      return_trace (അസത്യം);
    }

    return_trace ((ഇത്+markArray).apply (c, mark_index, base_index, ഇത്+baseArray, class_എണ്ണം_2, idx));
  }

  തർക്കം subset (hb_subset_context_t *c) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);
    സ്ഥിരം hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    സ്ഥിരം hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*ഇത്);
    എങ്കിൽ (unlikely (!c->serializer->extend_min (out))) return_trace (അസത്യം);
    out->format = format;

    hb_map_t klass_mapping;
    Markclass_closure_and_remap_indexes (ഇത്+markCoverage, ഇത്+markArray, glyphset, &klass_mapping);

    എങ്കിൽ (!klass_mapping.get_population ()) return_trace (അസത്യം);
    out->class_എണ്ണം_2 = klass_mapping.get_population ();

    auto mark_iter =
    + hb_zip (ഇത്+markCoverage, ഇത്+markArray)
    | hb_filter (glyphset, hb_first)
    ;

    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + mark_iter
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    എങ്കിൽ (!out->markCoverage.serialize_serialize (c->serializer, new_coverage.iter ()))
      return_trace (അസത്യം);

    എങ്കിൽ (unlikely (!out->markArray.serialize_subset (c, markArray, ഇത്,
						    (ഇത്+markCoverage).iter (),
						    &klass_mapping)))
      return_trace (അസത്യം);

    new_coverage.reset ();
    hb_sorted_vector_t<unsigned> base_indexes;
    സ്ഥിരം auto &base_array = ഇത്+baseArray;
    collect_subset_anchor_indexes (ഇത്+baseCoverage, base_array.rows,
                                   class_എണ്ണം_2, base_array.matrixZ.arrayZ,
                                   glyphset, glyph_map, klass_mapping,
                                   new_coverage, base_indexes);

    എങ്കിൽ (!new_coverage) return_trace (അസത്യം);
    എങ്കിൽ (!out->baseCoverage.serialize_serialize (c->serializer, new_coverage.iter ()))
      return_trace (അസത്യം);

    return_trace (out->baseArray.serialize_subset (c, baseArray, ഇത്,
						   new_coverage.length,
						   base_indexes.iter ()));
  }
};


}
}
}

#endif /* OT_LAYOUT_GPOS_MARKBASEPOSFORMAT1_HH */
