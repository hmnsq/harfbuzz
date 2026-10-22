#ifndef OT_LAYOUT_GPOS_MARKMARKPOSFORMAT1_HH
#define OT_LAYOUT_GPOS_MARKMARKPOSFORMAT1_HH

#અન્તર્ભાવ "MarkMarkPosFormat1.hh"

નામાકાશ OT {
નામાકાશ Layout {
નામાકાશ GPOS_impl {

template <typename Types>
સંરચના MarkMarkPosFormat1_2
{
  સુરક્ષિત:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  typename Types::template LOffsetTo<Coverage>
                mark1Coverage;          /* Offset to Combining Mark1 Coverage
                                         * table--from beginning of MarkMarkPos
                                         * subtable */
  typename Types::template LOffsetTo<Coverage>
                mark2Coverage;          /* Offset to Combining Mark2 Coverage
                                         * table--from beginning of MarkMarkPos
                                         * subtable */
  HBUINT16      class_ગણતરી_2;             /* Number of defined mark classes */
  typename Types::template LOffsetTo<MarkArray<Types>>
                mark1Array;             /* Offset to Mark1Array table--from
                                         * beginning of MarkMarkPos subtable */
  typename Types::template LOffsetTo<AnchorMatrix<Types, HBUINT16>>
                mark2Array;             /* Offset to Mark2Array table--from
                                         * beginning of MarkMarkPos subtable */
  જાહેર:
  DEFINE_SIZE_STATIC (4 + 4 * Types::LOffset::static_size);

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ) &&
                  mark1Coverage.sanitize (c, આ) &&
                  mark2Coverage.sanitize (c, આ) &&
                  mark1Array.sanitize (c, આ) &&
		  hb_barrier () &&
                  mark2Array.sanitize (c, આ, (unsigned પૂર્ણાંક) class_ગણતરી_2));
  }

  તાર્કિક intersects (અચલ hb_set_t *glyphs) અચલ
  {
    પાછા (આ+mark1Coverage).intersects (glyphs) &&
           (આ+mark2Coverage).intersects (glyphs);
  }

  શૂન્ય closure_lookups (hb_closure_lookups_context_t *c) અચલ {}

  શૂન્ય collect_variation_indices (hb_collect_variation_indices_context_t *c) અચલ
  {
    (આ+mark1Array).collect_variation_indices (c, આ+mark1Coverage);

    hb_map_t klass_mapping;
    Markclass_closure_and_remap_indexes (આ+mark1Coverage, આ+mark1Array, *c->glyph_set, &klass_mapping);

    hb_sorted_vector_t<unsigned> mark2_indexes;
    collect_anchor_indexes (આ+mark2Coverage, (આ+mark2Array).rows,
                            class_ગણતરી_2, *c->glyph_set, klass_mapping,
                            mark2_indexes);
    (આ+mark2Array).collect_variation_indices (c, mark2_indexes.iter ());
  }

  શૂન્ય collect_glyphs (hb_collect_glyphs_context_t *c) અચલ
  {
    જો (unlikely (!(આ+mark1Coverage).collect_coverage (c->આગત))) પાછા;
    જો (unlikely (!(આ+mark2Coverage).collect_coverage (c->આગત))) પાછા;
  }

  શૂન્ય collect_second_glyphs (hb_set_digest_t *digest) અચલ
  { (આ+mark2Coverage).collect_coverage (digest); }

  અચલ Coverage &get_coverage () અચલ { પાછા આ+mark1Coverage; }

  તાર્કિક apply (hb_ot_apply_context_t *c) અચલ
  {
    TRACE_APPLY (આ);
    hb_buffer_t *buffer = c->buffer;
    unsigned પૂર્ણાંક mark1_index = (આ+mark1Coverage).get_coverage  (buffer->cur().codepoint);
    જો (likely (mark1_index == NOT_COVERED)) return_trace (ખોટું);

    /* now we search backwards for a suitable mark glyph until a non-mark glyph */
    auto &skippy_iter = c->iter_આગત;
    skippy_iter.reset_fast (buffer->idx);
    skippy_iter.set_lookup_props (c->lookup_props & ~(uint32_t)LookupFlag::IgnoreFlags);
    unsigned unsafe_from;
    જો (unlikely (!skippy_iter.prev (&unsafe_from)))
    {
      buffer->unsafe_to_concat_from_outbuffer (unsafe_from, buffer->idx + 1);
      return_trace (ખોટું);
    }

    જો (likely (!_hb_glyph_info_is_mark (&buffer->info[skippy_iter.idx])))
    {
      buffer->unsafe_to_concat_from_outbuffer (skippy_iter.idx, buffer->idx + 1);
      return_trace (ખોટું);
    }

    unsigned પૂર્ણાંક j = skippy_iter.idx;

    unsigned પૂર્ણાંક id1 = _hb_glyph_info_get_lig_id (&buffer->cur());
    unsigned પૂર્ણાંક id2 = _hb_glyph_info_get_lig_id (&buffer->info[j]);
    unsigned પૂર્ણાંક comp1 = _hb_glyph_info_get_lig_comp (&buffer->cur());
    unsigned પૂર્ણાંક comp2 = _hb_glyph_info_get_lig_comp (&buffer->info[j]);

    જો (likely (id1 == id2))
    {
      જો (id1 == 0) /* Marks belonging to the same base. */
        જાઓ good;
      નહીંતર જો (comp1 == comp2) /* Marks belonging to the same ligature component. */
        જાઓ good;
    }
    નહીંતર
    {
      /* If ligature ids don't match, it may be the case that one of the marks
       * itself is a ligature.  In which case match. */
      જો ((id1 > 0 && !comp1) || (id2 > 0 && !comp2))
        જાઓ good;
    }

    /* Didn't match. */
    buffer->unsafe_to_concat_from_outbuffer (skippy_iter.idx, buffer->idx + 1);
    return_trace (ખોટું);

    good:
    unsigned પૂર્ણાંક mark2_index = (આ+mark2Coverage).get_coverage  (buffer->info[j].codepoint);
    જો (mark2_index == NOT_COVERED)
    {
      buffer->unsafe_to_concat_from_outbuffer (skippy_iter.idx, buffer->idx + 1);
      return_trace (ખોટું);
    }

    return_trace ((આ+mark1Array).apply (c, mark1_index, mark2_index, આ+mark2Array, class_ગણતરી_2, j));
  }

  તાર્કિક subset (hb_subset_context_t *c) અચલ
  {
    TRACE_SUBSET (આ);
    અચલ hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    અચલ hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*આ);
    જો (unlikely (!c->serializer->extend_min (out))) return_trace (ખોટું);
    out->format = format;

    hb_map_t klass_mapping;
    Markclass_closure_and_remap_indexes (આ+mark1Coverage, આ+mark1Array, glyphset, &klass_mapping);

    જો (!klass_mapping.get_population ()) return_trace (ખોટું);
    out->class_ગણતરી_2 = klass_mapping.get_population ();

    auto mark1_iter =
    + hb_zip (આ+mark1Coverage, આ+mark1Array)
    | hb_filter (glyphset, hb_first)
    ;

    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + mark1_iter
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    જો (!out->mark1Coverage.serialize_serialize (c->serializer, new_coverage.iter ()))
      return_trace (ખોટું);

    જો (unlikely (!out->mark1Array.serialize_subset (c, mark1Array, આ,
						     (આ+mark1Coverage).iter (),
						     &klass_mapping)))
      return_trace (ખોટું);

    new_coverage.reset ();
    hb_sorted_vector_t<unsigned> mark2_indexes;
    અચલ auto &mark2_array = આ+mark2Array;
    collect_subset_anchor_indexes (આ+mark2Coverage, mark2_array.rows,
                                   class_ગણતરી_2, mark2_array.matrixZ.arrayZ,
                                   glyphset, glyph_map, klass_mapping,
                                   new_coverage, mark2_indexes);

    જો (!new_coverage) return_trace (ખોટું);
    જો (!out->mark2Coverage.serialize_serialize (c->serializer, new_coverage.iter ()))
      return_trace (ખોટું);

    return_trace (out->mark2Array.serialize_subset (c, mark2Array, આ,
						    new_coverage.length,
						    mark2_indexes.iter ()));

  }
};


}
}
}

#endif /* OT_LAYOUT_GPOS_MARKMARKPOSFORMAT1_HH */
