#ifndef OT_LAYOUT_GPOS_MARKMARKPOSFORMAT1_HH
#define OT_LAYOUT_GPOS_MARKMARKPOSFORMAT1_HH

#अन्तर्भाव "MarkMarkPosFormat1.hh"

नामाकाश OT {
नामाकाश Layout {
नामाकाश GPOS_impl {

ढाँचा <typename Types>
संरचना MarkMarkPosFormat1_2
{
  रक्षित:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  typename Types::ढाँचा LOffsetTo<Coverage>
                mark1Coverage;          /* Offset to Combining Mark1 Coverage
                                         * table--from beginning of MarkMarkPos
                                         * subtable */
  typename Types::ढाँचा LOffsetTo<Coverage>
                mark2Coverage;          /* Offset to Combining Mark2 Coverage
                                         * table--from beginning of MarkMarkPos
                                         * subtable */
  HBUINT16      class_गन्ती_2;             /* Number of defined mark classes */
  typename Types::ढाँचा LOffsetTo<MarkArray<Types>>
                mark1Array;             /* Offset to Mark1Array table--from
                                         * beginning of MarkMarkPos subtable */
  typename Types::ढाँचा LOffsetTo<AnchorMatrix<Types, HBUINT16>>
                mark2Array;             /* Offset to Mark2Array table--from
                                         * beginning of MarkMarkPos subtable */
  सार्वजनिक:
  DEFINE_SIZE_STATIC (4 + 4 * Types::LOffset::static_size);

  तार्किक sanitize (hb_sanitize_context_t *c) अचर
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) &&
                  mark1Coverage.sanitize (c, एतत्) &&
                  mark2Coverage.sanitize (c, एतत्) &&
                  mark1Array.sanitize (c, एतत्) &&
		  hb_barrier () &&
                  mark2Array.sanitize (c, एतत्, (अचिन्हित पूर्णाङ्क) class_गन्ती_2));
  }

  तार्किक intersects (अचर hb_set_t *glyphs) अचर
  {
    फर्काऊ (एतत्+mark1Coverage).intersects (glyphs) &&
           (एतत्+mark2Coverage).intersects (glyphs);
  }

  शून्य closure_lookups (hb_closure_lookups_context_t *c) अचर {}

  शून्य collect_variation_indices (hb_collect_variation_indices_context_t *c) अचर
  {
    (एतत्+mark1Array).collect_variation_indices (c, एतत्+mark1Coverage);

    hb_map_t klass_mapping;
    Markclass_closure_and_remap_indexes (एतत्+mark1Coverage, एतत्+mark1Array, *c->glyph_set, &klass_mapping);

    hb_sorted_vector_t<अचिन्हित> mark2_indexes;
    collect_anchor_indexes (एतत्+mark2Coverage, (एतत्+mark2Array).rows,
                            class_गन्ती_2, *c->glyph_set, klass_mapping,
                            mark2_indexes);
    (एतत्+mark2Array).collect_variation_indices (c, mark2_indexes.iter ());
  }

  शून्य collect_glyphs (hb_collect_glyphs_context_t *c) अचर
  {
    यदि (unlikely (!(एतत्+mark1Coverage).collect_coverage (c->आगत))) फर्काऊ;
    यदि (unlikely (!(एतत्+mark2Coverage).collect_coverage (c->आगत))) फर्काऊ;
  }

  शून्य collect_second_glyphs (hb_set_digest_t *digest) अचर
  { (एतत्+mark2Coverage).collect_coverage (digest); }

  अचर Coverage &get_coverage () अचर { फर्काऊ एतत्+mark1Coverage; }

  तार्किक apply (hb_ot_apply_context_t *c) अचर
  {
    TRACE_APPLY (एतत्);
    hb_buffer_t *buffer = c->buffer;
    अचिन्हित पूर्णाङ्क mark1_index = (एतत्+mark1Coverage).get_coverage  (buffer->cur().codepoint);
    यदि (likely (mark1_index == NOT_COVERED)) return_trace (असत्य);

    /* now we search backwards for a suitable mark glyph until a non-mark glyph */
    स्वतः &skippy_iter = c->iter_आगत;
    skippy_iter.reset_fast (buffer->idx);
    skippy_iter.set_lookup_props (c->lookup_props & ~(uint32_t)LookupFlag::IgnoreFlags);
    अचिन्हित unsafe_from;
    यदि (unlikely (!skippy_iter.prev (&unsafe_from)))
    {
      buffer->unsafe_to_concat_from_outbuffer (unsafe_from, buffer->idx + 1);
      return_trace (असत्य);
    }

    यदि (likely (!_hb_glyph_info_is_mark (&buffer->info[skippy_iter.idx])))
    {
      buffer->unsafe_to_concat_from_outbuffer (skippy_iter.idx, buffer->idx + 1);
      return_trace (असत्य);
    }

    अचिन्हित पूर्णाङ्क j = skippy_iter.idx;

    अचिन्हित पूर्णाङ्क id1 = _hb_glyph_info_get_lig_id (&buffer->cur());
    अचिन्हित पूर्णाङ्क id2 = _hb_glyph_info_get_lig_id (&buffer->info[j]);
    अचिन्हित पूर्णाङ्क comp1 = _hb_glyph_info_get_lig_comp (&buffer->cur());
    अचिन्हित पूर्णाङ्क comp2 = _hb_glyph_info_get_lig_comp (&buffer->info[j]);

    यदि (likely (id1 == id2))
    {
      यदि (id1 == 0) /* Marks belonging to the same base. */
        गच्छ good;
      अन्यथा यदि (comp1 == comp2) /* Marks belonging to the same ligature component. */
        गच्छ good;
    }
    अन्यथा
    {
      /* If ligature ids don't match, it may be the case that one of the marks
       * itself is a ligature.  In which case match. */
      यदि ((id1 > 0 && !comp1) || (id2 > 0 && !comp2))
        गच्छ good;
    }

    /* Didn't match. */
    buffer->unsafe_to_concat_from_outbuffer (skippy_iter.idx, buffer->idx + 1);
    return_trace (असत्य);

    good:
    अचिन्हित पूर्णाङ्क mark2_index = (एतत्+mark2Coverage).get_coverage  (buffer->info[j].codepoint);
    यदि (mark2_index == NOT_COVERED)
    {
      buffer->unsafe_to_concat_from_outbuffer (skippy_iter.idx, buffer->idx + 1);
      return_trace (असत्य);
    }

    return_trace ((एतत्+mark1Array).apply (c, mark1_index, mark2_index, एतत्+mark2Array, class_गन्ती_2, j));
  }

  तार्किक subset (hb_subset_context_t *c) अचर
  {
    TRACE_SUBSET (एतत्);
    अचर hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    अचर hb_map_t &glyph_map = *c->plan->glyph_map;

    स्वतः *out = c->serializer->start_embed (*एतत्);
    यदि (unlikely (!c->serializer->extend_min (out))) return_trace (असत्य);
    out->format = format;

    hb_map_t klass_mapping;
    Markclass_closure_and_remap_indexes (एतत्+mark1Coverage, एतत्+mark1Array, glyphset, &klass_mapping);

    यदि (!klass_mapping.get_population ()) return_trace (असत्य);
    out->class_गन्ती_2 = klass_mapping.get_population ();

    स्वतः mark1_iter =
    + hb_zip (एतत्+mark1Coverage, एतत्+mark1Array)
    | hb_filter (glyphset, hb_first)
    ;

    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + mark1_iter
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    यदि (!out->mark1Coverage.serialize_serialize (c->serializer, new_coverage.iter ()))
      return_trace (असत्य);

    यदि (unlikely (!out->mark1Array.serialize_subset (c, mark1Array, एतत्,
						     (एतत्+mark1Coverage).iter (),
						     &klass_mapping)))
      return_trace (असत्य);

    new_coverage.reset ();
    hb_sorted_vector_t<अचिन्हित> mark2_indexes;
    अचर स्वतः &mark2_array = एतत्+mark2Array;
    collect_subset_anchor_indexes (एतत्+mark2Coverage, mark2_array.rows,
                                   class_गन्ती_2, mark2_array.matrixZ.arrayZ,
                                   glyphset, glyph_map, klass_mapping,
                                   new_coverage, mark2_indexes);

    यदि (!new_coverage) return_trace (असत्य);
    यदि (!out->mark2Coverage.serialize_serialize (c->serializer, new_coverage.iter ()))
      return_trace (असत्य);

    return_trace (out->mark2Array.serialize_subset (c, mark2Array, एतत्,
						    new_coverage.length,
						    mark2_indexes.iter ()));

  }
};


}
}
}

#endif /* OT_LAYOUT_GPOS_MARKMARKPOSFORMAT1_HH */
