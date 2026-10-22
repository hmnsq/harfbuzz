#ifndef OT_LAYOUT_GPOS_MARKMARKPOSFORMAT1_HH
#define OT_LAYOUT_GPOS_MARKMARKPOSFORMAT1_HH

#अन्तर्भाव "MarkMarkPosFormat1.hh"

नामाकाश OT {
नामाकाश Layout {
नामाकाश GPOS_impl {

ढाँचा <typename Types>
संरचना MarkMarkPosFormat1_2
{
  संरक्षित:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  typename Types::ढाँचा LOffsetTo<Coverage>
                mark1Coverage;          /* Offset to Combining Mark1 Coverage
                                         * table--from beginning of MarkMarkPos
                                         * subtable */
  typename Types::ढाँचा LOffsetTo<Coverage>
                mark2Coverage;          /* Offset to Combining Mark2 Coverage
                                         * table--from beginning of MarkMarkPos
                                         * subtable */
  HBUINT16      class_मोजणी_2;             /* Number of defined mark classes */
  typename Types::ढाँचा LOffsetTo<MarkArray<Types>>
                mark1Array;             /* Offset to Mark1Array table--from
                                         * beginning of MarkMarkPos subtable */
  typename Types::ढाँचा LOffsetTo<AnchorMatrix<Types, HBUINT16>>
                mark2Array;             /* Offset to Mark2Array table--from
                                         * beginning of MarkMarkPos subtable */
  सार्वजनिक:
  DEFINE_SIZE_STATIC (4 + 4 * Types::LOffset::static_size);

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे) &&
                  mark1Coverage.sanitize (c, हे) &&
                  mark2Coverage.sanitize (c, हे) &&
                  mark1Array.sanitize (c, हे) &&
		  hb_barrier () &&
                  mark2Array.sanitize (c, हे, (अचिन्हित पूर्णांक) class_मोजणी_2));
  }

  तार्किक intersects (स्थिरांक hb_set_t *glyphs) स्थिरांक
  {
    परत (हे+mark1Coverage).intersects (glyphs) &&
           (हे+mark2Coverage).intersects (glyphs);
  }

  शून्य closure_lookups (hb_closure_lookups_context_t *c) स्थिरांक {}

  शून्य collect_variation_indices (hb_collect_variation_indices_context_t *c) स्थिरांक
  {
    (हे+mark1Array).collect_variation_indices (c, हे+mark1Coverage);

    hb_map_t klass_mapping;
    Markclass_closure_and_remap_indexes (हे+mark1Coverage, हे+mark1Array, *c->glyph_set, &klass_mapping);

    hb_sorted_vector_t<अचिन्हित> mark2_indexes;
    collect_anchor_indexes (हे+mark2Coverage, (हे+mark2Array).rows,
                            class_मोजणी_2, *c->glyph_set, klass_mapping,
                            mark2_indexes);
    (हे+mark2Array).collect_variation_indices (c, mark2_indexes.iter ());
  }

  शून्य collect_glyphs (hb_collect_glyphs_context_t *c) स्थिरांक
  {
    जर (unlikely (!(हे+mark1Coverage).collect_coverage (c->आदान))) परत;
    जर (unlikely (!(हे+mark2Coverage).collect_coverage (c->आदान))) परत;
  }

  शून्य collect_second_glyphs (hb_set_digest_t *digest) स्थिरांक
  { (हे+mark2Coverage).collect_coverage (digest); }

  स्थिरांक Coverage &get_coverage () स्थिरांक { परत हे+mark1Coverage; }

  तार्किक apply (hb_ot_apply_context_t *c) स्थिरांक
  {
    TRACE_APPLY (हे);
    hb_buffer_t *buffer = c->buffer;
    अचिन्हित पूर्णांक mark1_index = (हे+mark1Coverage).get_coverage  (buffer->cur().codepoint);
    जर (likely (mark1_index == NOT_COVERED)) return_trace (असत्य);

    /* now we search backwards for a suitable mark glyph until a non-mark glyph */
    स्वतः &skippy_iter = c->iter_आदान;
    skippy_iter.reset_fast (buffer->idx);
    skippy_iter.set_lookup_props (c->lookup_props & ~(uint32_t)LookupFlag::IgnoreFlags);
    अचिन्हित unsafe_from;
    जर (unlikely (!skippy_iter.prev (&unsafe_from)))
    {
      buffer->unsafe_to_concat_from_outbuffer (unsafe_from, buffer->idx + 1);
      return_trace (असत्य);
    }

    जर (likely (!_hb_glyph_info_is_mark (&buffer->info[skippy_iter.idx])))
    {
      buffer->unsafe_to_concat_from_outbuffer (skippy_iter.idx, buffer->idx + 1);
      return_trace (असत्य);
    }

    अचिन्हित पूर्णांक j = skippy_iter.idx;

    अचिन्हित पूर्णांक id1 = _hb_glyph_info_get_lig_id (&buffer->cur());
    अचिन्हित पूर्णांक id2 = _hb_glyph_info_get_lig_id (&buffer->info[j]);
    अचिन्हित पूर्णांक comp1 = _hb_glyph_info_get_lig_comp (&buffer->cur());
    अचिन्हित पूर्णांक comp2 = _hb_glyph_info_get_lig_comp (&buffer->info[j]);

    जर (likely (id1 == id2))
    {
      जर (id1 == 0) /* Marks belonging to the same base. */
        जा good;
      अन्यथा जर (comp1 == comp2) /* Marks belonging to the same ligature component. */
        जा good;
    }
    अन्यथा
    {
      /* If ligature ids don't match, it may be the case that one of the marks
       * itself is a ligature.  In which case match. */
      जर ((id1 > 0 && !comp1) || (id2 > 0 && !comp2))
        जा good;
    }

    /* Didn't match. */
    buffer->unsafe_to_concat_from_outbuffer (skippy_iter.idx, buffer->idx + 1);
    return_trace (असत्य);

    good:
    अचिन्हित पूर्णांक mark2_index = (हे+mark2Coverage).get_coverage  (buffer->info[j].codepoint);
    जर (mark2_index == NOT_COVERED)
    {
      buffer->unsafe_to_concat_from_outbuffer (skippy_iter.idx, buffer->idx + 1);
      return_trace (असत्य);
    }

    return_trace ((हे+mark1Array).apply (c, mark1_index, mark2_index, हे+mark2Array, class_मोजणी_2, j));
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
    Markclass_closure_and_remap_indexes (हे+mark1Coverage, हे+mark1Array, glyphset, &klass_mapping);

    जर (!klass_mapping.get_population ()) return_trace (असत्य);
    out->class_मोजणी_2 = klass_mapping.get_population ();

    स्वतः mark1_iter =
    + hb_zip (हे+mark1Coverage, हे+mark1Array)
    | hb_filter (glyphset, hb_first)
    ;

    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + mark1_iter
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    जर (!out->mark1Coverage.serialize_serialize (c->serializer, new_coverage.iter ()))
      return_trace (असत्य);

    जर (unlikely (!out->mark1Array.serialize_subset (c, mark1Array, हे,
						     (हे+mark1Coverage).iter (),
						     &klass_mapping)))
      return_trace (असत्य);

    new_coverage.reset ();
    hb_sorted_vector_t<अचिन्हित> mark2_indexes;
    स्थिरांक स्वतः &mark2_array = हे+mark2Array;
    collect_subset_anchor_indexes (हे+mark2Coverage, mark2_array.rows,
                                   class_मोजणी_2, mark2_array.matrixZ.arrayZ,
                                   glyphset, glyph_map, klass_mapping,
                                   new_coverage, mark2_indexes);

    जर (!new_coverage) return_trace (असत्य);
    जर (!out->mark2Coverage.serialize_serialize (c->serializer, new_coverage.iter ()))
      return_trace (असत्य);

    return_trace (out->mark2Array.serialize_subset (c, mark2Array, हे,
						    new_coverage.length,
						    mark2_indexes.iter ()));

  }
};


}
}
}

#endif /* OT_LAYOUT_GPOS_MARKMARKPOSFORMAT1_HH */
