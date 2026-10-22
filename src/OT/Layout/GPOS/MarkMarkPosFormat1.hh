#ifndef OT_LAYOUT_GPOS_MARKMARKPOSFORMAT1_HH
#define OT_LAYOUT_GPOS_MARKMARKPOSFORMAT1_HH

#ଅନ୍ତର୍ଭାଵ "MarkMarkPosFormat1.hh"

ନାମାକାଶ OT {
ନାମାକାଶ Layout {
ନାମାକାଶ GPOS_impl {

template <typename Types>
ସଂରଚନା MarkMarkPosFormat1_2
{
  ସୁରକ୍ଷିତ:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  typename Types::template LOffsetTo<Coverage>
                mark1Coverage;          /* Offset to Combining Mark1 Coverage
                                         * table--from beginning of MarkMarkPos
                                         * subtable */
  typename Types::template LOffsetTo<Coverage>
                mark2Coverage;          /* Offset to Combining Mark2 Coverage
                                         * table--from beginning of MarkMarkPos
                                         * subtable */
  HBUINT16      class_ଗଣନା_2;             /* Number of defined mark classes */
  typename Types::template LOffsetTo<MarkArray<Types>>
                mark1Array;             /* Offset to Mark1Array table--from
                                         * beginning of MarkMarkPos subtable */
  typename Types::template LOffsetTo<AnchorMatrix<Types, HBUINT16>>
                mark2Array;             /* Offset to Mark2Array table--from
                                         * beginning of MarkMarkPos subtable */
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (4 + 4 * Types::LOffset::static_size);

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି) &&
                  mark1Coverage.sanitize (c, ଏହି) &&
                  mark2Coverage.sanitize (c, ଏହି) &&
                  mark1Array.sanitize (c, ଏହି) &&
		  hb_barrier () &&
                  mark2Array.sanitize (c, ଏହି, (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା) class_ଗଣନା_2));
  }

  ତାର୍କିକ intersects (ଅଚଳ hb_set_t *glyphs) ଅଚଳ
  {
    ଫେରନ୍ତୁ (ଏହି+mark1Coverage).intersects (glyphs) &&
           (ଏହି+mark2Coverage).intersects (glyphs);
  }

  ଶୂନ୍ୟ closure_lookups (hb_closure_lookups_context_t *c) ଅଚଳ {}

  ଶୂନ୍ୟ collect_variation_indices (hb_collect_variation_indices_context_t *c) ଅଚଳ
  {
    (ଏହି+mark1Array).collect_variation_indices (c, ଏହି+mark1Coverage);

    hb_map_t klass_mapping;
    Markclass_closure_and_remap_indexes (ଏହି+mark1Coverage, ଏହି+mark1Array, *c->glyph_set, &klass_mapping);

    hb_sorted_vector_t<unsigned> mark2_indexes;
    collect_anchor_indexes (ଏହି+mark2Coverage, (ଏହି+mark2Array).rows,
                            class_ଗଣନା_2, *c->glyph_set, klass_mapping,
                            mark2_indexes);
    (ଏହି+mark2Array).collect_variation_indices (c, mark2_indexes.iter ());
  }

  ଶୂନ୍ୟ collect_glyphs (hb_collect_glyphs_context_t *c) ଅଚଳ
  {
    ଯଦି (unlikely (!(ଏହି+mark1Coverage).collect_coverage (c->ନିବେଶ))) ଫେରନ୍ତୁ;
    ଯଦି (unlikely (!(ଏହି+mark2Coverage).collect_coverage (c->ନିବେଶ))) ଫେରନ୍ତୁ;
  }

  ଶୂନ୍ୟ collect_second_glyphs (hb_set_digest_t *digest) ଅଚଳ
  { (ଏହି+mark2Coverage).collect_coverage (digest); }

  ଅଚଳ Coverage &get_coverage () ଅଚଳ { ଫେରନ୍ତୁ ଏହି+mark1Coverage; }

  ତାର୍କିକ apply (hb_ot_apply_context_t *c) ଅଚଳ
  {
    TRACE_APPLY (ଏହି);
    hb_buffer_t *buffer = c->buffer;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା mark1_index = (ଏହି+mark1Coverage).get_coverage  (buffer->cur().codepoint);
    ଯଦି (likely (mark1_index == NOT_COVERED)) return_trace (ମିଥ୍ୟା);

    /* now we search backwards for a suitable mark glyph until a non-mark glyph */
    auto &skippy_iter = c->iter_ନିବେଶ;
    skippy_iter.reset_fast (buffer->idx);
    skippy_iter.set_lookup_props (c->lookup_props & ~(uint32_t)LookupFlag::IgnoreFlags);
    unsigned unsafe_from;
    ଯଦି (unlikely (!skippy_iter.prev (&unsafe_from)))
    {
      buffer->unsafe_to_concat_from_outbuffer (unsafe_from, buffer->idx + 1);
      return_trace (ମିଥ୍ୟା);
    }

    ଯଦି (likely (!_hb_glyph_info_is_mark (&buffer->info[skippy_iter.idx])))
    {
      buffer->unsafe_to_concat_from_outbuffer (skippy_iter.idx, buffer->idx + 1);
      return_trace (ମିଥ୍ୟା);
    }

    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = skippy_iter.idx;

    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା id1 = _hb_glyph_info_get_lig_id (&buffer->cur());
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା id2 = _hb_glyph_info_get_lig_id (&buffer->info[j]);
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା comp1 = _hb_glyph_info_get_lig_comp (&buffer->cur());
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା comp2 = _hb_glyph_info_get_lig_comp (&buffer->info[j]);

    ଯଦି (likely (id1 == id2))
    {
      ଯଦି (id1 == 0) /* Marks belonging to the same base. */
        ଯାଅ good;
      ଅନ୍ୟଥା ଯଦି (comp1 == comp2) /* Marks belonging to the same ligature component. */
        ଯାଅ good;
    }
    ଅନ୍ୟଥା
    {
      /* If ligature ids don't match, it may be the case that one of the marks
       * itself is a ligature.  In which case match. */
      ଯଦି ((id1 > 0 && !comp1) || (id2 > 0 && !comp2))
        ଯାଅ good;
    }

    /* Didn't match. */
    buffer->unsafe_to_concat_from_outbuffer (skippy_iter.idx, buffer->idx + 1);
    return_trace (ମିଥ୍ୟା);

    good:
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା mark2_index = (ଏହି+mark2Coverage).get_coverage  (buffer->info[j].codepoint);
    ଯଦି (mark2_index == NOT_COVERED)
    {
      buffer->unsafe_to_concat_from_outbuffer (skippy_iter.idx, buffer->idx + 1);
      return_trace (ମିଥ୍ୟା);
    }

    return_trace ((ଏହି+mark1Array).apply (c, mark1_index, mark2_index, ଏହି+mark2Array, class_ଗଣନା_2, j));
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
    Markclass_closure_and_remap_indexes (ଏହି+mark1Coverage, ଏହି+mark1Array, glyphset, &klass_mapping);

    ଯଦି (!klass_mapping.get_population ()) return_trace (ମିଥ୍ୟା);
    out->class_ଗଣନା_2 = klass_mapping.get_population ();

    auto mark1_iter =
    + hb_zip (ଏହି+mark1Coverage, ଏହି+mark1Array)
    | hb_filter (glyphset, hb_first)
    ;

    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + mark1_iter
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    ଯଦି (!out->mark1Coverage.serialize_serialize (c->serializer, new_coverage.iter ()))
      return_trace (ମିଥ୍ୟା);

    ଯଦି (unlikely (!out->mark1Array.serialize_subset (c, mark1Array, ଏହି,
						     (ଏହି+mark1Coverage).iter (),
						     &klass_mapping)))
      return_trace (ମିଥ୍ୟା);

    new_coverage.reset ();
    hb_sorted_vector_t<unsigned> mark2_indexes;
    ଅଚଳ auto &mark2_array = ଏହି+mark2Array;
    collect_subset_anchor_indexes (ଏହି+mark2Coverage, mark2_array.rows,
                                   class_ଗଣନା_2, mark2_array.matrixZ.arrayZ,
                                   glyphset, glyph_map, klass_mapping,
                                   new_coverage, mark2_indexes);

    ଯଦି (!new_coverage) return_trace (ମିଥ୍ୟା);
    ଯଦି (!out->mark2Coverage.serialize_serialize (c->serializer, new_coverage.iter ()))
      return_trace (ମିଥ୍ୟା);

    return_trace (out->mark2Array.serialize_subset (c, mark2Array, ଏହି,
						    new_coverage.length,
						    mark2_indexes.iter ()));

  }
};


}
}
}

#endif /* OT_LAYOUT_GPOS_MARKMARKPOSFORMAT1_HH */
