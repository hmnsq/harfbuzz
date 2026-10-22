#ifndef OT_LAYOUT_GPOS_MARKBASEPOSFORMAT1_HH
#define OT_LAYOUT_GPOS_MARKBASEPOSFORMAT1_HH

#অন্তর্ভাব "MarkArray.hh"

নামাকাশ OT {
নামাকাশ Layout {
নামাকাশ GPOS_impl {

template <typename Types>
সংরচনা MarkBasePosFormat1_2
{
  রক্ষিত:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  typename Types::template LOffsetTo<Coverage>
                markCoverage;           /* Offset to MarkCoverage table--from
                                         * beginning of MarkBasePos subtable */
  typename Types::template LOffsetTo<Coverage>
                baseCoverage;           /* Offset to BaseCoverage table--from
                                         * beginning of MarkBasePos subtable */
  HBUINT16      class_গুনতি_2;             /* Number of classes defined for marks */
  typename Types::template LOffsetTo<MarkArray<Types>>
                markArray;              /* Offset to MarkArray table--from
                                         * beginning of MarkBasePos subtable */
  typename Types::template LOffsetTo<AnchorMatrix<Types>>
                baseArray;              /* Offset to BaseArray table--from
                                         * beginning of MarkBasePos subtable */

  সার্বজনিক:
  DEFINE_SIZE_STATIC (4 + 4 * Types::LOffset::static_size);

    তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্) &&
                  markCoverage.sanitize (c, এতত্) &&
                  baseCoverage.sanitize (c, এতত্) &&
                  markArray.sanitize (c, এতত্) &&
                  baseArray.sanitize (c, এতত্, (unsigned পূর্ণাঙ্ক) class_গুনতি_2));
  }

  তার্কিক intersects (ধ্রুব hb_set_t *glyphs) ধ্রুব
  {
    প্রত্যাগম (এতত্+markCoverage).intersects (glyphs) &&
           (এতত্+baseCoverage).intersects (glyphs);
  }

  শূন্য closure_lookups (hb_closure_lookups_context_t *c) ধ্রুব {}

  শূন্য collect_variation_indices (hb_collect_variation_indices_context_t *c) ধ্রুব
  {
    (এতত্+markArray).collect_variation_indices (c, এতত্+markCoverage);

    hb_map_t klass_mapping;
    Markclass_closure_and_remap_indexes (এতত্+markCoverage, এতত্+markArray, *c->glyph_set, &klass_mapping);

    hb_sorted_vector_t<unsigned> base_indexes;
    collect_anchor_indexes (এতত্+baseCoverage, (এতত্+baseArray).rows,
                            class_গুনতি_2, *c->glyph_set, klass_mapping,
                            base_indexes);
    (এতত্+baseArray).collect_variation_indices (c, base_indexes.iter ());
  }

  শূন্য collect_glyphs (hb_collect_glyphs_context_t *c) ধ্রুব
  {
    যদি (unlikely (!(এতত্+markCoverage).collect_coverage (c->প্রবেশ))) প্রত্যাগম;
    যদি (unlikely (!(এতত্+baseCoverage).collect_coverage (c->প্রবেশ))) প্রত্যাগম;
  }

  শূন্য collect_second_glyphs (hb_set_digest_t *digest) ধ্রুব
  { (এতত্+baseCoverage).collect_coverage (digest); }

  ধ্রুব Coverage &get_coverage () ধ্রুব { প্রত্যাগম এতত্+markCoverage; }

  স্থির অন্তঃস্থ তার্কিক accept (hb_buffer_t *buffer, unsigned idx)
  {
    /* We only want to attach to the first of a MultipleSubst sequence.
     * https://github.com/harfbuzz/harfbuzz/issues/740
     * Reject others...
     * ...but stop if we find a mark in the MultipleSubst sequence:
     * https://github.com/harfbuzz/harfbuzz/issues/1020 */
    প্রত্যাগম !_hb_glyph_info_multiplied (&buffer->info[idx]) ||
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

  তার্কিক apply (hb_ot_apply_context_t *c) ধ্রুব
  {
    TRACE_APPLY (এতত্);
    hb_buffer_t *buffer = c->buffer;
    unsigned পূর্ণাঙ্ক mark_index = (এতত্+markCoverage).get_coverage  (buffer->cur().codepoint);
    যদি (likely (mark_index == NOT_COVERED)) return_trace (অসত্য);

    /* Now we search backwards for a non-mark glyph.
     * We don't use skippy_iter.prev() to avoid O(n^2) behavior. */

    auto &skippy_iter = c->iter_প্রবেশ;
    skippy_iter.set_lookup_props (LookupFlag::IgnoreMarks);

    যদি (c->last_base_until > buffer->idx)
    {
      c->last_base_until = 0;
      c->last_base = -1;
    }
    unsigned j;
    প্রতি (j = buffer->idx; j > c->last_base_until; j--)
    {
      auto match = skippy_iter.match (buffer->info[j - 1]);
      যদি (match == skippy_iter.MATCH)
      {
        // https://github.com/harfbuzz/harfbuzz/issues/4124
	যদি (!accept (buffer, j - 1) &&
	    NOT_COVERED == (এতত্+baseCoverage).get_coverage  (buffer->info[j - 1].codepoint))
	  match = skippy_iter.SKIP;
      }
      যদি (match == skippy_iter.MATCH)
      {
	c->last_base = (signed) j - 1;
	ভঙ্গ;
      }
    }
    c->last_base_until = buffer->idx;
    যদি (c->last_base == -1)
    {
      buffer->unsafe_to_concat_from_outbuffer (0, buffer->idx + 1);
      return_trace (অসত্য);
    }

    unsigned idx = (unsigned) c->last_base;

    /* Checking that matched glyph is actually a base glyph by GDEF is too strong; disabled */
    //if (!_hb_glyph_info_is_base_glyph (&buffer->info[idx])) { return_trace (false); }

    unsigned পূর্ণাঙ্ক base_index = (এতত্+baseCoverage).get_coverage  (buffer->info[idx].codepoint);
    যদি (base_index == NOT_COVERED)
    {
      buffer->unsafe_to_concat_from_outbuffer (idx, buffer->idx + 1);
      return_trace (অসত্য);
    }

    return_trace ((এতত্+markArray).apply (c, mark_index, base_index, এতত্+baseArray, class_গুনতি_2, idx));
  }

  তার্কিক subset (hb_subset_context_t *c) ধ্রুব
  {
    TRACE_SUBSET (এতত্);
    ধ্রুব hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ধ্রুব hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*এতত্);
    যদি (unlikely (!c->serializer->extend_min (out))) return_trace (অসত্য);
    out->format = format;

    hb_map_t klass_mapping;
    Markclass_closure_and_remap_indexes (এতত্+markCoverage, এতত্+markArray, glyphset, &klass_mapping);

    যদি (!klass_mapping.get_population ()) return_trace (অসত্য);
    out->class_গুনতি_2 = klass_mapping.get_population ();

    auto mark_iter =
    + hb_zip (এতত্+markCoverage, এতত্+markArray)
    | hb_filter (glyphset, hb_first)
    ;

    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + mark_iter
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    যদি (!out->markCoverage.serialize_serialize (c->serializer, new_coverage.iter ()))
      return_trace (অসত্য);

    যদি (unlikely (!out->markArray.serialize_subset (c, markArray, এতত্,
						    (এতত্+markCoverage).iter (),
						    &klass_mapping)))
      return_trace (অসত্য);

    new_coverage.reset ();
    hb_sorted_vector_t<unsigned> base_indexes;
    ধ্রুব auto &base_array = এতত্+baseArray;
    collect_subset_anchor_indexes (এতত্+baseCoverage, base_array.rows,
                                   class_গুনতি_2, base_array.matrixZ.arrayZ,
                                   glyphset, glyph_map, klass_mapping,
                                   new_coverage, base_indexes);

    যদি (!new_coverage) return_trace (অসত্য);
    যদি (!out->baseCoverage.serialize_serialize (c->serializer, new_coverage.iter ()))
      return_trace (অসত্য);

    return_trace (out->baseArray.serialize_subset (c, baseArray, এতত্,
						   new_coverage.length,
						   base_indexes.iter ()));
  }
};


}
}
}

#endif /* OT_LAYOUT_GPOS_MARKBASEPOSFORMAT1_HH */
