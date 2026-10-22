#ifndef OT_LAYOUT_GPOS_MARKMARKPOSFORMAT1_HH
#define OT_LAYOUT_GPOS_MARKMARKPOSFORMAT1_HH

#ಅನ್ತರ್ಭಾವ "MarkMarkPosFormat1.hh"

ನಾಮಾಕಾಶ OT {
ನಾಮಾಕಾಶ Layout {
ನಾಮಾಕಾಶ GPOS_impl {

template <typename Types>
ಸಂರಚನಾ MarkMarkPosFormat1_2
{
  ರಕ್ಷಿತ:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  typename Types::template LOffsetTo<Coverage>
                mark1Coverage;          /* Offset to Combining Mark1 Coverage
                                         * table--from beginning of MarkMarkPos
                                         * subtable */
  typename Types::template LOffsetTo<Coverage>
                mark2Coverage;          /* Offset to Combining Mark2 Coverage
                                         * table--from beginning of MarkMarkPos
                                         * subtable */
  HBUINT16      class_ಎಣಿಕೆ_2;             /* Number of defined mark classes */
  typename Types::template LOffsetTo<MarkArray<Types>>
                mark1Array;             /* Offset to Mark1Array table--from
                                         * beginning of MarkMarkPos subtable */
  typename Types::template LOffsetTo<AnchorMatrix<Types, HBUINT16>>
                mark2Array;             /* Offset to Mark2Array table--from
                                         * beginning of MarkMarkPos subtable */
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (4 + 4 * Types::LOffset::static_size);

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್) &&
                  mark1Coverage.sanitize (c, ಏತತ್) &&
                  mark2Coverage.sanitize (c, ಏತತ್) &&
                  mark1Array.sanitize (c, ಏತತ್) &&
		  hb_barrier () &&
                  mark2Array.sanitize (c, ಏತತ್, (unsigned ಪೂರ್ಣಾಙ್ಕ) class_ಎಣಿಕೆ_2));
  }

  ತಾರ್ಕಿಕ intersects (ಧ್ರುವ hb_set_t *glyphs) ಧ್ರುವ
  {
    ಪ್ರತ್ಯಾಗಮ (ಏತತ್+mark1Coverage).intersects (glyphs) &&
           (ಏತತ್+mark2Coverage).intersects (glyphs);
  }

  ಶೂನ್ಯ closure_lookups (hb_closure_lookups_context_t *c) ಧ್ರುವ {}

  ಶೂನ್ಯ collect_variation_indices (hb_collect_variation_indices_context_t *c) ಧ್ರುವ
  {
    (ಏತತ್+mark1Array).collect_variation_indices (c, ಏತತ್+mark1Coverage);

    hb_map_t klass_mapping;
    Markclass_closure_and_remap_indexes (ಏತತ್+mark1Coverage, ಏತತ್+mark1Array, *c->glyph_set, &klass_mapping);

    hb_sorted_vector_t<unsigned> mark2_indexes;
    collect_anchor_indexes (ಏತತ್+mark2Coverage, (ಏತತ್+mark2Array).rows,
                            class_ಎಣಿಕೆ_2, *c->glyph_set, klass_mapping,
                            mark2_indexes);
    (ಏತತ್+mark2Array).collect_variation_indices (c, mark2_indexes.iter ());
  }

  ಶೂನ್ಯ collect_glyphs (hb_collect_glyphs_context_t *c) ಧ್ರುವ
  {
    ಯದಿ (unlikely (!(ಏತತ್+mark1Coverage).collect_coverage (c->ಒಳಹರಿವು))) ಪ್ರತ್ಯಾಗಮ;
    ಯದಿ (unlikely (!(ಏತತ್+mark2Coverage).collect_coverage (c->ಒಳಹರಿವು))) ಪ್ರತ್ಯಾಗಮ;
  }

  ಶೂನ್ಯ collect_second_glyphs (hb_set_digest_t *digest) ಧ್ರುವ
  { (ಏತತ್+mark2Coverage).collect_coverage (digest); }

  ಧ್ರುವ Coverage &get_coverage () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ ಏತತ್+mark1Coverage; }

  ತಾರ್ಕಿಕ apply (hb_ot_apply_context_t *c) ಧ್ರುವ
  {
    TRACE_APPLY (ಏತತ್);
    hb_buffer_t *buffer = c->buffer;
    unsigned ಪೂರ್ಣಾಙ್ಕ mark1_index = (ಏತತ್+mark1Coverage).get_coverage  (buffer->cur().codepoint);
    ಯದಿ (likely (mark1_index == NOT_COVERED)) return_trace (ಅಸತ್ಯ);

    /* now we search backwards for a suitable mark glyph until a non-mark glyph */
    auto &skippy_iter = c->iter_ಒಳಹರಿವು;
    skippy_iter.reset_fast (buffer->idx);
    skippy_iter.set_lookup_props (c->lookup_props & ~(uint32_t)LookupFlag::IgnoreFlags);
    unsigned unsafe_from;
    ಯದಿ (unlikely (!skippy_iter.prev (&unsafe_from)))
    {
      buffer->unsafe_to_concat_from_outbuffer (unsafe_from, buffer->idx + 1);
      return_trace (ಅಸತ್ಯ);
    }

    ಯದಿ (likely (!_hb_glyph_info_is_mark (&buffer->info[skippy_iter.idx])))
    {
      buffer->unsafe_to_concat_from_outbuffer (skippy_iter.idx, buffer->idx + 1);
      return_trace (ಅಸತ್ಯ);
    }

    unsigned ಪೂರ್ಣಾಙ್ಕ j = skippy_iter.idx;

    unsigned ಪೂರ್ಣಾಙ್ಕ id1 = _hb_glyph_info_get_lig_id (&buffer->cur());
    unsigned ಪೂರ್ಣಾಙ್ಕ id2 = _hb_glyph_info_get_lig_id (&buffer->info[j]);
    unsigned ಪೂರ್ಣಾಙ್ಕ comp1 = _hb_glyph_info_get_lig_comp (&buffer->cur());
    unsigned ಪೂರ್ಣಾಙ್ಕ comp2 = _hb_glyph_info_get_lig_comp (&buffer->info[j]);

    ಯದಿ (likely (id1 == id2))
    {
      ಯದಿ (id1 == 0) /* Marks belonging to the same base. */
        ಗಚ್ಛ good;
      ಅನ್ಯಥಾ ಯದಿ (comp1 == comp2) /* Marks belonging to the same ligature component. */
        ಗಚ್ಛ good;
    }
    ಅನ್ಯಥಾ
    {
      /* If ligature ids don't match, it may be the case that one of the marks
       * itself is a ligature.  In which case match. */
      ಯದಿ ((id1 > 0 && !comp1) || (id2 > 0 && !comp2))
        ಗಚ್ಛ good;
    }

    /* Didn't match. */
    buffer->unsafe_to_concat_from_outbuffer (skippy_iter.idx, buffer->idx + 1);
    return_trace (ಅಸತ್ಯ);

    good:
    unsigned ಪೂರ್ಣಾಙ್ಕ mark2_index = (ಏತತ್+mark2Coverage).get_coverage  (buffer->info[j].codepoint);
    ಯದಿ (mark2_index == NOT_COVERED)
    {
      buffer->unsafe_to_concat_from_outbuffer (skippy_iter.idx, buffer->idx + 1);
      return_trace (ಅಸತ್ಯ);
    }

    return_trace ((ಏತತ್+mark1Array).apply (c, mark1_index, mark2_index, ಏತತ್+mark2Array, class_ಎಣಿಕೆ_2, j));
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
    Markclass_closure_and_remap_indexes (ಏತತ್+mark1Coverage, ಏತತ್+mark1Array, glyphset, &klass_mapping);

    ಯದಿ (!klass_mapping.get_population ()) return_trace (ಅಸತ್ಯ);
    out->class_ಎಣಿಕೆ_2 = klass_mapping.get_population ();

    auto mark1_iter =
    + hb_zip (ಏತತ್+mark1Coverage, ಏತತ್+mark1Array)
    | hb_filter (glyphset, hb_first)
    ;

    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + mark1_iter
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    ಯದಿ (!out->mark1Coverage.serialize_serialize (c->serializer, new_coverage.iter ()))
      return_trace (ಅಸತ್ಯ);

    ಯದಿ (unlikely (!out->mark1Array.serialize_subset (c, mark1Array, ಏತತ್,
						     (ಏತತ್+mark1Coverage).iter (),
						     &klass_mapping)))
      return_trace (ಅಸತ್ಯ);

    new_coverage.reset ();
    hb_sorted_vector_t<unsigned> mark2_indexes;
    ಧ್ರುವ auto &mark2_array = ಏತತ್+mark2Array;
    collect_subset_anchor_indexes (ಏತತ್+mark2Coverage, mark2_array.rows,
                                   class_ಎಣಿಕೆ_2, mark2_array.matrixZ.arrayZ,
                                   glyphset, glyph_map, klass_mapping,
                                   new_coverage, mark2_indexes);

    ಯದಿ (!new_coverage) return_trace (ಅಸತ್ಯ);
    ಯದಿ (!out->mark2Coverage.serialize_serialize (c->serializer, new_coverage.iter ()))
      return_trace (ಅಸತ್ಯ);

    return_trace (out->mark2Array.serialize_subset (c, mark2Array, ಏತತ್,
						    new_coverage.length,
						    mark2_indexes.iter ()));

  }
};


}
}
}

#endif /* OT_LAYOUT_GPOS_MARKMARKPOSFORMAT1_HH */
