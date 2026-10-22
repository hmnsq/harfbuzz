#ifndef OT_LAYOUT_GPOS_MARKMARKPOSFORMAT1_HH
#define OT_LAYOUT_GPOS_MARKMARKPOSFORMAT1_HH

#அந்தர்பாவ "MarkMarkPosFormat1.hh"

நாமாகாஶ OT {
நாமாகாஶ Layout {
நாமாகாஶ GPOS_impl {

template <typename Types>
கட்டமைப்பு MarkMarkPosFormat1_2
{
  பாதுகாக்கப்பட்ட:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  typename Types::template LOffsetTo<Coverage>
                mark1Coverage;          /* Offset to Combining Mark1 Coverage
                                         * table--from beginning of MarkMarkPos
                                         * subtable */
  typename Types::template LOffsetTo<Coverage>
                mark2Coverage;          /* Offset to Combining Mark2 Coverage
                                         * table--from beginning of MarkMarkPos
                                         * subtable */
  HBUINT16      class_மொத்தம்_2;             /* Number of defined mark classes */
  typename Types::template LOffsetTo<MarkArray<Types>>
                mark1Array;             /* Offset to Mark1Array table--from
                                         * beginning of MarkMarkPos subtable */
  typename Types::template LOffsetTo<AnchorMatrix<Types, HBUINT16>>
                mark2Array;             /* Offset to Mark2Array table--from
                                         * beginning of MarkMarkPos subtable */
  பொது:
  DEFINE_SIZE_STATIC (4 + 4 * Types::LOffset::static_size);

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது) &&
                  mark1Coverage.sanitize (c, இது) &&
                  mark2Coverage.sanitize (c, இது) &&
                  mark1Array.sanitize (c, இது) &&
		  hb_barrier () &&
                  mark2Array.sanitize (c, இது, (unsigned முழு) class_மொத்தம்_2));
  }

  தருக்க intersects (மாறிலி hb_set_t *glyphs) மாறிலி
  {
    திரும்பு (இது+mark1Coverage).intersects (glyphs) &&
           (இது+mark2Coverage).intersects (glyphs);
  }

  வெறுமை closure_lookups (hb_closure_lookups_context_t *c) மாறிலி {}

  வெறுமை collect_variation_indices (hb_collect_variation_indices_context_t *c) மாறிலி
  {
    (இது+mark1Array).collect_variation_indices (c, இது+mark1Coverage);

    hb_map_t klass_mapping;
    Markclass_closure_and_remap_indexes (இது+mark1Coverage, இது+mark1Array, *c->glyph_set, &klass_mapping);

    hb_sorted_vector_t<unsigned> mark2_indexes;
    collect_anchor_indexes (இது+mark2Coverage, (இது+mark2Array).rows,
                            class_மொத்தம்_2, *c->glyph_set, klass_mapping,
                            mark2_indexes);
    (இது+mark2Array).collect_variation_indices (c, mark2_indexes.iter ());
  }

  வெறுமை collect_glyphs (hb_collect_glyphs_context_t *c) மாறிலி
  {
    எனில் (unlikely (!(இது+mark1Coverage).collect_coverage (c->உள்ளீடு))) திரும்பு;
    எனில் (unlikely (!(இது+mark2Coverage).collect_coverage (c->உள்ளீடு))) திரும்பு;
  }

  வெறுமை collect_second_glyphs (hb_set_digest_t *digest) மாறிலி
  { (இது+mark2Coverage).collect_coverage (digest); }

  மாறிலி Coverage &get_coverage () மாறிலி { திரும்பு இது+mark1Coverage; }

  தருக்க apply (hb_ot_apply_context_t *c) மாறிலி
  {
    TRACE_APPLY (இது);
    hb_buffer_t *buffer = c->buffer;
    unsigned முழு mark1_index = (இது+mark1Coverage).get_coverage  (buffer->cur().codepoint);
    எனில் (likely (mark1_index == NOT_COVERED)) return_trace (பொய்);

    /* now we search backwards for a suitable mark glyph until a non-mark glyph */
    auto &skippy_iter = c->iter_உள்ளீடு;
    skippy_iter.reset_fast (buffer->idx);
    skippy_iter.set_lookup_props (c->lookup_props & ~(uint32_t)LookupFlag::IgnoreFlags);
    unsigned unsafe_from;
    எனில் (unlikely (!skippy_iter.prev (&unsafe_from)))
    {
      buffer->unsafe_to_concat_from_outbuffer (unsafe_from, buffer->idx + 1);
      return_trace (பொய்);
    }

    எனில் (likely (!_hb_glyph_info_is_mark (&buffer->info[skippy_iter.idx])))
    {
      buffer->unsafe_to_concat_from_outbuffer (skippy_iter.idx, buffer->idx + 1);
      return_trace (பொய்);
    }

    unsigned முழு j = skippy_iter.idx;

    unsigned முழு id1 = _hb_glyph_info_get_lig_id (&buffer->cur());
    unsigned முழு id2 = _hb_glyph_info_get_lig_id (&buffer->info[j]);
    unsigned முழு comp1 = _hb_glyph_info_get_lig_comp (&buffer->cur());
    unsigned முழு comp2 = _hb_glyph_info_get_lig_comp (&buffer->info[j]);

    எனில் (likely (id1 == id2))
    {
      எனில் (id1 == 0) /* Marks belonging to the same base. */
        செல் good;
      இல்லையெனில் எனில் (comp1 == comp2) /* Marks belonging to the same ligature component. */
        செல் good;
    }
    இல்லையெனில்
    {
      /* If ligature ids don't match, it may be the case that one of the marks
       * itself is a ligature.  In which case match. */
      எனில் ((id1 > 0 && !comp1) || (id2 > 0 && !comp2))
        செல் good;
    }

    /* Didn't match. */
    buffer->unsafe_to_concat_from_outbuffer (skippy_iter.idx, buffer->idx + 1);
    return_trace (பொய்);

    good:
    unsigned முழு mark2_index = (இது+mark2Coverage).get_coverage  (buffer->info[j].codepoint);
    எனில் (mark2_index == NOT_COVERED)
    {
      buffer->unsafe_to_concat_from_outbuffer (skippy_iter.idx, buffer->idx + 1);
      return_trace (பொய்);
    }

    return_trace ((இது+mark1Array).apply (c, mark1_index, mark2_index, இது+mark2Array, class_மொத்தம்_2, j));
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
    Markclass_closure_and_remap_indexes (இது+mark1Coverage, இது+mark1Array, glyphset, &klass_mapping);

    எனில் (!klass_mapping.get_population ()) return_trace (பொய்);
    out->class_மொத்தம்_2 = klass_mapping.get_population ();

    auto mark1_iter =
    + hb_zip (இது+mark1Coverage, இது+mark1Array)
    | hb_filter (glyphset, hb_first)
    ;

    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + mark1_iter
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    எனில் (!out->mark1Coverage.serialize_serialize (c->serializer, new_coverage.iter ()))
      return_trace (பொய்);

    எனில் (unlikely (!out->mark1Array.serialize_subset (c, mark1Array, இது,
						     (இது+mark1Coverage).iter (),
						     &klass_mapping)))
      return_trace (பொய்);

    new_coverage.reset ();
    hb_sorted_vector_t<unsigned> mark2_indexes;
    மாறிலி auto &mark2_array = இது+mark2Array;
    collect_subset_anchor_indexes (இது+mark2Coverage, mark2_array.rows,
                                   class_மொத்தம்_2, mark2_array.matrixZ.arrayZ,
                                   glyphset, glyph_map, klass_mapping,
                                   new_coverage, mark2_indexes);

    எனில் (!new_coverage) return_trace (பொய்);
    எனில் (!out->mark2Coverage.serialize_serialize (c->serializer, new_coverage.iter ()))
      return_trace (பொய்);

    return_trace (out->mark2Array.serialize_subset (c, mark2Array, இது,
						    new_coverage.length,
						    mark2_indexes.iter ()));

  }
};


}
}
}

#endif /* OT_LAYOUT_GPOS_MARKMARKPOSFORMAT1_HH */
