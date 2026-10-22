#ifndef OT_LAYOUT_GPOS_MARKMARKPOSFORMAT1_HH
#define OT_LAYOUT_GPOS_MARKMARKPOSFORMAT1_HH

#ضمن "MarkMarkPosFormat1.hh"

نطاق OT {
نطاق Layout {
نطاق GPOS_impl {

template <typename Types>
بنية MarkMarkPosFormat1_2
{
  محمي:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  typename Types::template LOffsetTo<Coverage>
                mark1Coverage;          /* Offset to Combining Mark1 Coverage
                                         * table--from beginning of MarkMarkPos
                                         * subtable */
  typename Types::template LOffsetTo<Coverage>
                mark2Coverage;          /* Offset to Combining Mark2 Coverage
                                         * table--from beginning of MarkMarkPos
                                         * subtable */
  HBUINT16      classCount;             /* Number of defined mark classes */
  typename Types::template LOffsetTo<MarkArray<Types>>
                mark1Array;             /* Offset to Mark1Array table--from
                                         * beginning of MarkMarkPos subtable */
  typename Types::template LOffsetTo<AnchorMatrix<Types, HBUINT16>>
                mark2Array;             /* Offset to Mark2Array table--from
                                         * beginning of MarkMarkPos subtable */
  عام:
  DEFINE_SIZE_STATIC (4 + 4 * Types::LOffset::static_size);

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
                  mark1Coverage.sanitize (c, هذا) &&
                  mark2Coverage.sanitize (c, هذا) &&
                  mark1Array.sanitize (c, هذا) &&
		  hb_barrier () &&
                  mark2Array.sanitize (c, هذا, (unsigned صحيح) classCount));
  }

  منطقي intersects (ثابت hb_set_t *glyphs) ثابت
  {
    ܦܢܐ (هذا+mark1Coverage).intersects (glyphs) &&
           (هذا+mark2Coverage).intersects (glyphs);
  }

  فراغ closure_lookups (hb_closure_lookups_context_t *c) ثابت {}

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    (هذا+mark1Array).collect_variation_indices (c, هذا+mark1Coverage);

    hb_map_t klass_mapping;
    Markclass_closure_and_remap_indexes (هذا+mark1Coverage, هذا+mark1Array, *c->glyph_set, &klass_mapping);

    hb_sorted_vector_t<unsigned> mark2_indexes;
    collect_anchor_indexes (هذا+mark2Coverage, (هذا+mark2Array).rows,
                            classCount, *c->glyph_set, klass_mapping,
                            mark2_indexes);
    (هذا+mark2Array).collect_variation_indices (c, mark2_indexes.iter ());
  }

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    ܐܢ (unlikely (!(هذا+mark1Coverage).collect_coverage (c->input))) ܦܢܐ;
    ܐܢ (unlikely (!(هذا+mark2Coverage).collect_coverage (c->input))) ܦܢܐ;
  }

  فراغ collect_second_glyphs (hb_set_digest_t *digest) ثابت
  { (هذا+mark2Coverage).collect_coverage (digest); }

  ثابت Coverage &get_coverage () ثابت { ܦܢܐ هذا+mark1Coverage; }

  منطقي apply (hb_ot_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);
    hb_buffer_t *buffer = c->buffer;
    unsigned صحيح mark1_index = (هذا+mark1Coverage).get_coverage  (buffer->cur().codepoint);
    ܐܢ (likely (mark1_index == NOT_COVERED)) return_trace (ܕܓܠܐ);

    /* now we search backwards for a suitable mark glyph until a non-mark glyph */
    auto &skippy_iter = c->iter_input;
    skippy_iter.reset_fast (buffer->idx);
    skippy_iter.set_lookup_props (c->lookup_props & ~(uint32_t)LookupFlag::IgnoreFlags);
    unsigned unsafe_from;
    ܐܢ (unlikely (!skippy_iter.prev (&unsafe_from)))
    {
      buffer->unsafe_to_concat_from_outbuffer (unsafe_from, buffer->idx + 1);
      return_trace (ܕܓܠܐ);
    }

    ܐܢ (likely (!_hb_glyph_info_is_mark (&buffer->info[skippy_iter.idx])))
    {
      buffer->unsafe_to_concat_from_outbuffer (skippy_iter.idx, buffer->idx + 1);
      return_trace (ܕܓܠܐ);
    }

    unsigned صحيح j = skippy_iter.idx;

    unsigned صحيح id1 = _hb_glyph_info_get_lig_id (&buffer->cur());
    unsigned صحيح id2 = _hb_glyph_info_get_lig_id (&buffer->info[j]);
    unsigned صحيح comp1 = _hb_glyph_info_get_lig_comp (&buffer->cur());
    unsigned صحيح comp2 = _hb_glyph_info_get_lig_comp (&buffer->info[j]);

    ܐܢ (likely (id1 == id2))
    {
      ܐܢ (id1 == 0) /* Marks belonging to the same base. */
        اذهب good;
      ܐܚܪܢܐ ܐܢ (comp1 == comp2) /* Marks belonging to the same ligature component. */
        اذهب good;
    }
    ܐܚܪܢܐ
    {
      /* If ligature ids don't match, it may be the case that one of the marks
       * itself is a ligature.  In which case match. */
      ܐܢ ((id1 > 0 && !comp1) || (id2 > 0 && !comp2))
        اذهب good;
    }

    /* Didn't match. */
    buffer->unsafe_to_concat_from_outbuffer (skippy_iter.idx, buffer->idx + 1);
    return_trace (ܕܓܠܐ);

    good:
    unsigned صحيح mark2_index = (هذا+mark2Coverage).get_coverage  (buffer->info[j].codepoint);
    ܐܢ (mark2_index == NOT_COVERED)
    {
      buffer->unsafe_to_concat_from_outbuffer (skippy_iter.idx, buffer->idx + 1);
      return_trace (ܕܓܠܐ);
    }

    return_trace ((هذا+mark1Array).apply (c, mark1_index, mark2_index, هذا+mark2Array, classCount, j));
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*هذا);
    ܐܢ (unlikely (!c->serializer->extend_min (out))) return_trace (ܕܓܠܐ);
    out->format = format;

    hb_map_t klass_mapping;
    Markclass_closure_and_remap_indexes (هذا+mark1Coverage, هذا+mark1Array, glyphset, &klass_mapping);

    ܐܢ (!klass_mapping.get_population ()) return_trace (ܕܓܠܐ);
    out->classCount = klass_mapping.get_population ();

    auto mark1_iter =
    + hb_zip (هذا+mark1Coverage, هذا+mark1Array)
    | hb_filter (glyphset, hb_first)
    ;

    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + mark1_iter
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    ܐܢ (!out->mark1Coverage.serialize_serialize (c->serializer, new_coverage.iter ()))
      return_trace (ܕܓܠܐ);

    ܐܢ (unlikely (!out->mark1Array.serialize_subset (c, mark1Array, هذا,
						     (هذا+mark1Coverage).iter (),
						     &klass_mapping)))
      return_trace (ܕܓܠܐ);

    new_coverage.reset ();
    hb_sorted_vector_t<unsigned> mark2_indexes;
    ثابت auto &mark2_array = هذا+mark2Array;
    collect_subset_anchor_indexes (هذا+mark2Coverage, mark2_array.rows,
                                   classCount, mark2_array.matrixZ.arrayZ,
                                   glyphset, glyph_map, klass_mapping,
                                   new_coverage, mark2_indexes);

    ܐܢ (!new_coverage) return_trace (ܕܓܠܐ);
    ܐܢ (!out->mark2Coverage.serialize_serialize (c->serializer, new_coverage.iter ()))
      return_trace (ܕܓܠܐ);

    return_trace (out->mark2Array.serialize_subset (c, mark2Array, هذا,
						    new_coverage.length,
						    mark2_indexes.iter ()));

  }
};


}
}
}

#endif /* OT_LAYOUT_GPOS_MARKMARKPOSFORMAT1_HH */
