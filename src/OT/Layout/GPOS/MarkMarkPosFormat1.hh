#ifndef OT_LAYOUT_GPOS_MARKMARKPOSFORMAT1_HH
#define OT_LAYOUT_GPOS_MARKMARKPOSFORMAT1_HH

#अन्तर्भाव "MarkMarkPosFormat1.hh"

नामाकाश OT {
नामाकाश Layout {
नामाकाश GPOS_impl {

template <typename Types>
संरचना MarkMarkPosFormat1_2
{
  रक्षित:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  typename Types::template LOffsetTo<Coverage>
                mark1Coverage;          /* Offset to Combining Mark1 Coverage
                                         * table--from beginning of MarkMarkPos
                                         * subtable */
  typename Types::template LOffsetTo<Coverage>
                mark2Coverage;          /* Offset to Combining Mark2 Coverage
                                         * table--from beginning of MarkMarkPos
                                         * subtable */
  HBUINT16      class_गणनम्_2;             /* Number of defined mark classes */
  typename Types::template LOffsetTo<MarkArray<Types>>
                mark1Array;             /* Offset to Mark1Array table--from
                                         * beginning of MarkMarkPos subtable */
  typename Types::template LOffsetTo<AnchorMatrix<Types, HBUINT16>>
                mark2Array;             /* Offset to Mark2Array table--from
                                         * beginning of MarkMarkPos subtable */
  सार्वजनिक:
  DEFINE_SIZE_STATIC (4 + 4 * Types::LOffset::static_size);

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) &&
                  mark1Coverage.sanitize (c, एतत्) &&
                  mark2Coverage.sanitize (c, एतत्) &&
                  mark1Array.sanitize (c, एतत्) &&
		  hb_barrier () &&
                  mark2Array.sanitize (c, एतत्, (unsigned पूर्णाङ्क) class_गणनम्_2));
  }

  तार्किक intersects (ध्रुव hb_set_t *glyphs) ध्रुव
  {
    प्रत्यागम (एतत्+mark1Coverage).intersects (glyphs) &&
           (एतत्+mark2Coverage).intersects (glyphs);
  }

  शून्य closure_lookups (hb_closure_lookups_context_t *c) ध्रुव {}

  शून्य collect_variation_indices (hb_collect_variation_indices_context_t *c) ध्रुव
  {
    (एतत्+mark1Array).collect_variation_indices (c, एतत्+mark1Coverage);

    hb_map_t klass_mapping;
    Markclass_closure_and_remap_indexes (एतत्+mark1Coverage, एतत्+mark1Array, *c->glyph_set, &klass_mapping);

    hb_sorted_vector_t<unsigned> mark2_indexes;
    collect_anchor_indexes (एतत्+mark2Coverage, (एतत्+mark2Array).rows,
                            class_गणनम्_2, *c->glyph_set, klass_mapping,
                            mark2_indexes);
    (एतत्+mark2Array).collect_variation_indices (c, mark2_indexes.iter ());
  }

  शून्य collect_glyphs (hb_collect_glyphs_context_t *c) ध्रुव
  {
    जइ (unlikely (!(एतत्+mark1Coverage).collect_coverage (c->निवेशः))) प्रत्यागम;
    जइ (unlikely (!(एतत्+mark2Coverage).collect_coverage (c->निवेशः))) प्रत्यागम;
  }

  शून्य collect_second_glyphs (hb_set_digest_t *digest) ध्रुव
  { (एतत्+mark2Coverage).collect_coverage (digest); }

  ध्रुव Coverage &get_coverage () ध्रुव { प्रत्यागम एतत्+mark1Coverage; }

  तार्किक apply (hb_ot_apply_context_t *c) ध्रुव
  {
    TRACE_APPLY (एतत्);
    hb_buffer_t *buffer = c->buffer;
    unsigned पूर्णाङ्क mark1_index = (एतत्+mark1Coverage).get_coverage  (buffer->cur().codepoint);
    जइ (likely (mark1_index == NOT_COVERED)) return_trace (मिच्छ);

    /* now we search backwards for a suitable mark glyph until a non-mark glyph */
    auto &skippy_iter = c->iter_निवेशः;
    skippy_iter.reset_fast (buffer->idx);
    skippy_iter.set_lookup_props (c->lookup_props & ~(uint32_t)LookupFlag::IgnoreFlags);
    unsigned unsafe_from;
    जइ (unlikely (!skippy_iter.prev (&unsafe_from)))
    {
      buffer->unsafe_to_concat_from_outbuffer (unsafe_from, buffer->idx + 1);
      return_trace (मिच्छ);
    }

    जइ (likely (!_hb_glyph_info_is_mark (&buffer->info[skippy_iter.idx])))
    {
      buffer->unsafe_to_concat_from_outbuffer (skippy_iter.idx, buffer->idx + 1);
      return_trace (मिच्छ);
    }

    unsigned पूर्णाङ्क j = skippy_iter.idx;

    unsigned पूर्णाङ्क id1 = _hb_glyph_info_get_lig_id (&buffer->cur());
    unsigned पूर्णाङ्क id2 = _hb_glyph_info_get_lig_id (&buffer->info[j]);
    unsigned पूर्णाङ्क comp1 = _hb_glyph_info_get_lig_comp (&buffer->cur());
    unsigned पूर्णाङ्क comp2 = _hb_glyph_info_get_lig_comp (&buffer->info[j]);

    जइ (likely (id1 == id2))
    {
      जइ (id1 == 0) /* Marks belonging to the same base. */
        गच्छ good;
      अण्णहा जइ (comp1 == comp2) /* Marks belonging to the same ligature component. */
        गच्छ good;
    }
    अण्णहा
    {
      /* If ligature ids don't match, it may be the case that one of the marks
       * itself is a ligature.  In which case match. */
      जइ ((id1 > 0 && !comp1) || (id2 > 0 && !comp2))
        गच्छ good;
    }

    /* Didn't match. */
    buffer->unsafe_to_concat_from_outbuffer (skippy_iter.idx, buffer->idx + 1);
    return_trace (मिच्छ);

    good:
    unsigned पूर्णाङ्क mark2_index = (एतत्+mark2Coverage).get_coverage  (buffer->info[j].codepoint);
    जइ (mark2_index == NOT_COVERED)
    {
      buffer->unsafe_to_concat_from_outbuffer (skippy_iter.idx, buffer->idx + 1);
      return_trace (मिच्छ);
    }

    return_trace ((एतत्+mark1Array).apply (c, mark1_index, mark2_index, एतत्+mark2Array, class_गणनम्_2, j));
  }

  तार्किक subset (hb_subset_context_t *c) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    ध्रुव hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ध्रुव hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*एतत्);
    जइ (unlikely (!c->serializer->extend_min (out))) return_trace (मिच्छ);
    out->format = format;

    hb_map_t klass_mapping;
    Markclass_closure_and_remap_indexes (एतत्+mark1Coverage, एतत्+mark1Array, glyphset, &klass_mapping);

    जइ (!klass_mapping.get_population ()) return_trace (मिच्छ);
    out->class_गणनम्_2 = klass_mapping.get_population ();

    auto mark1_iter =
    + hb_zip (एतत्+mark1Coverage, एतत्+mark1Array)
    | hb_filter (glyphset, hb_first)
    ;

    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + mark1_iter
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    जइ (!out->mark1Coverage.serialize_serialize (c->serializer, new_coverage.iter ()))
      return_trace (मिच्छ);

    जइ (unlikely (!out->mark1Array.serialize_subset (c, mark1Array, एतत्,
						     (एतत्+mark1Coverage).iter (),
						     &klass_mapping)))
      return_trace (मिच्छ);

    new_coverage.reset ();
    hb_sorted_vector_t<unsigned> mark2_indexes;
    ध्रुव auto &mark2_array = एतत्+mark2Array;
    collect_subset_anchor_indexes (एतत्+mark2Coverage, mark2_array.rows,
                                   class_गणनम्_2, mark2_array.matrixZ.arrayZ,
                                   glyphset, glyph_map, klass_mapping,
                                   new_coverage, mark2_indexes);

    जइ (!new_coverage) return_trace (मिच्छ);
    जइ (!out->mark2Coverage.serialize_serialize (c->serializer, new_coverage.iter ()))
      return_trace (मिच्छ);

    return_trace (out->mark2Array.serialize_subset (c, mark2Array, एतत्,
						    new_coverage.length,
						    mark2_indexes.iter ()));

  }
};


}
}
}

#endif /* OT_LAYOUT_GPOS_MARKMARKPOSFORMAT1_HH */
