#ifndef OT_LAYOUT_GPOS_MARKMARKPOSFORMAT1_HH
#define OT_LAYOUT_GPOS_MARKMARKPOSFORMAT1_HH

#അന്തര്ഭാവ "MarkMarkPosFormat1.hh"

നാമാകാശ OT {
നാമാകാശ Layout {
നാമാകാശ GPOS_impl {

template <typename Types>
ഘടന MarkMarkPosFormat1_2
{
  സംരക്ഷിത:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  typename Types::template LOffsetTo<Coverage>
                mark1Coverage;          /* Offset to Combining Mark1 Coverage
                                         * table--from beginning of MarkMarkPos
                                         * subtable */
  typename Types::template LOffsetTo<Coverage>
                mark2Coverage;          /* Offset to Combining Mark2 Coverage
                                         * table--from beginning of MarkMarkPos
                                         * subtable */
  HBUINT16      class_എണ്ണം_2;             /* Number of defined mark classes */
  typename Types::template LOffsetTo<MarkArray<Types>>
                mark1Array;             /* Offset to Mark1Array table--from
                                         * beginning of MarkMarkPos subtable */
  typename Types::template LOffsetTo<AnchorMatrix<Types, HBUINT16>>
                mark2Array;             /* Offset to Mark2Array table--from
                                         * beginning of MarkMarkPos subtable */
  പൊതു:
  DEFINE_SIZE_STATIC (4 + 4 * Types::LOffset::static_size);

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_struct (ഇത്) &&
                  mark1Coverage.sanitize (c, ഇത്) &&
                  mark2Coverage.sanitize (c, ഇത്) &&
                  mark1Array.sanitize (c, ഇത്) &&
		  hb_barrier () &&
                  mark2Array.sanitize (c, ഇത്, (unsigned പൂർണ്ണസംഖ്യ) class_എണ്ണം_2));
  }

  തർക്കം intersects (സ്ഥിരം hb_set_t *glyphs) സ്ഥിരം
  {
    തിരികെ (ഇത്+mark1Coverage).intersects (glyphs) &&
           (ഇത്+mark2Coverage).intersects (glyphs);
  }

  ശൂന്യം closure_lookups (hb_closure_lookups_context_t *c) സ്ഥിരം {}

  ശൂന്യം collect_variation_indices (hb_collect_variation_indices_context_t *c) സ്ഥിരം
  {
    (ഇത്+mark1Array).collect_variation_indices (c, ഇത്+mark1Coverage);

    hb_map_t klass_mapping;
    Markclass_closure_and_remap_indexes (ഇത്+mark1Coverage, ഇത്+mark1Array, *c->glyph_set, &klass_mapping);

    hb_sorted_vector_t<unsigned> mark2_indexes;
    collect_anchor_indexes (ഇത്+mark2Coverage, (ഇത്+mark2Array).rows,
                            class_എണ്ണം_2, *c->glyph_set, klass_mapping,
                            mark2_indexes);
    (ഇത്+mark2Array).collect_variation_indices (c, mark2_indexes.iter ());
  }

  ശൂന്യം collect_glyphs (hb_collect_glyphs_context_t *c) സ്ഥിരം
  {
    എങ്കിൽ (unlikely (!(ഇത്+mark1Coverage).collect_coverage (c->നിവേശം))) തിരികെ;
    എങ്കിൽ (unlikely (!(ഇത്+mark2Coverage).collect_coverage (c->നിവേശം))) തിരികെ;
  }

  ശൂന്യം collect_second_glyphs (hb_set_digest_t *digest) സ്ഥിരം
  { (ഇത്+mark2Coverage).collect_coverage (digest); }

  സ്ഥിരം Coverage &get_coverage () സ്ഥിരം { തിരികെ ഇത്+mark1Coverage; }

  തർക്കം apply (hb_ot_apply_context_t *c) സ്ഥിരം
  {
    TRACE_APPLY (ഇത്);
    hb_buffer_t *buffer = c->buffer;
    unsigned പൂർണ്ണസംഖ്യ mark1_index = (ഇത്+mark1Coverage).get_coverage  (buffer->cur().codepoint);
    എങ്കിൽ (likely (mark1_index == NOT_COVERED)) return_trace (അസത്യം);

    /* now we search backwards for a suitable mark glyph until a non-mark glyph */
    auto &skippy_iter = c->iter_നിവേശം;
    skippy_iter.reset_fast (buffer->idx);
    skippy_iter.set_lookup_props (c->lookup_props & ~(uint32_t)LookupFlag::IgnoreFlags);
    unsigned unsafe_from;
    എങ്കിൽ (unlikely (!skippy_iter.prev (&unsafe_from)))
    {
      buffer->unsafe_to_concat_from_outbuffer (unsafe_from, buffer->idx + 1);
      return_trace (അസത്യം);
    }

    എങ്കിൽ (likely (!_hb_glyph_info_is_mark (&buffer->info[skippy_iter.idx])))
    {
      buffer->unsafe_to_concat_from_outbuffer (skippy_iter.idx, buffer->idx + 1);
      return_trace (അസത്യം);
    }

    unsigned പൂർണ്ണസംഖ്യ j = skippy_iter.idx;

    unsigned പൂർണ്ണസംഖ്യ id1 = _hb_glyph_info_get_lig_id (&buffer->cur());
    unsigned പൂർണ്ണസംഖ്യ id2 = _hb_glyph_info_get_lig_id (&buffer->info[j]);
    unsigned പൂർണ്ണസംഖ്യ comp1 = _hb_glyph_info_get_lig_comp (&buffer->cur());
    unsigned പൂർണ്ണസംഖ്യ comp2 = _hb_glyph_info_get_lig_comp (&buffer->info[j]);

    എങ്കിൽ (likely (id1 == id2))
    {
      എങ്കിൽ (id1 == 0) /* Marks belonging to the same base. */
        പോകുക good;
      അല്ലെങ്കിൽ എങ്കിൽ (comp1 == comp2) /* Marks belonging to the same ligature component. */
        പോകുക good;
    }
    അല്ലെങ്കിൽ
    {
      /* If ligature ids don't match, it may be the case that one of the marks
       * itself is a ligature.  In which case match. */
      എങ്കിൽ ((id1 > 0 && !comp1) || (id2 > 0 && !comp2))
        പോകുക good;
    }

    /* Didn't match. */
    buffer->unsafe_to_concat_from_outbuffer (skippy_iter.idx, buffer->idx + 1);
    return_trace (അസത്യം);

    good:
    unsigned പൂർണ്ണസംഖ്യ mark2_index = (ഇത്+mark2Coverage).get_coverage  (buffer->info[j].codepoint);
    എങ്കിൽ (mark2_index == NOT_COVERED)
    {
      buffer->unsafe_to_concat_from_outbuffer (skippy_iter.idx, buffer->idx + 1);
      return_trace (അസത്യം);
    }

    return_trace ((ഇത്+mark1Array).apply (c, mark1_index, mark2_index, ഇത്+mark2Array, class_എണ്ണം_2, j));
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
    Markclass_closure_and_remap_indexes (ഇത്+mark1Coverage, ഇത്+mark1Array, glyphset, &klass_mapping);

    എങ്കിൽ (!klass_mapping.get_population ()) return_trace (അസത്യം);
    out->class_എണ്ണം_2 = klass_mapping.get_population ();

    auto mark1_iter =
    + hb_zip (ഇത്+mark1Coverage, ഇത്+mark1Array)
    | hb_filter (glyphset, hb_first)
    ;

    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + mark1_iter
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    എങ്കിൽ (!out->mark1Coverage.serialize_serialize (c->serializer, new_coverage.iter ()))
      return_trace (അസത്യം);

    എങ്കിൽ (unlikely (!out->mark1Array.serialize_subset (c, mark1Array, ഇത്,
						     (ഇത്+mark1Coverage).iter (),
						     &klass_mapping)))
      return_trace (അസത്യം);

    new_coverage.reset ();
    hb_sorted_vector_t<unsigned> mark2_indexes;
    സ്ഥിരം auto &mark2_array = ഇത്+mark2Array;
    collect_subset_anchor_indexes (ഇത്+mark2Coverage, mark2_array.rows,
                                   class_എണ്ണം_2, mark2_array.matrixZ.arrayZ,
                                   glyphset, glyph_map, klass_mapping,
                                   new_coverage, mark2_indexes);

    എങ്കിൽ (!new_coverage) return_trace (അസത്യം);
    എങ്കിൽ (!out->mark2Coverage.serialize_serialize (c->serializer, new_coverage.iter ()))
      return_trace (അസത്യം);

    return_trace (out->mark2Array.serialize_subset (c, mark2Array, ഇത്,
						    new_coverage.length,
						    mark2_indexes.iter ()));

  }
};


}
}
}

#endif /* OT_LAYOUT_GPOS_MARKMARKPOSFORMAT1_HH */
