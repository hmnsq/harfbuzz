#ifndef OT_LAYOUT_GPOS_MARKMARKPOSFORMAT1_HH
#define OT_LAYOUT_GPOS_MARKMARKPOSFORMAT1_HH

#অন্তর্ভাৱ "MarkMarkPosFormat1.hh"

নামাকাশ OT {
নামাকাশ Layout {
নামাকাশ GPOS_impl {

template <typename Types>
সংরচনা MarkMarkPosFormat1_2
{
  রক্ষিত:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  typename Types::template LOffsetTo<Coverage>
                mark1Coverage;          /* Offset to Combining Mark1 Coverage
                                         * table--from beginning of MarkMarkPos
                                         * subtable */
  typename Types::template LOffsetTo<Coverage>
                mark2Coverage;          /* Offset to Combining Mark2 Coverage
                                         * table--from beginning of MarkMarkPos
                                         * subtable */
  HBUINT16      class_গুনতি_2;             /* Number of defined mark classes */
  typename Types::template LOffsetTo<MarkArray<Types>>
                mark1Array;             /* Offset to Mark1Array table--from
                                         * beginning of MarkMarkPos subtable */
  typename Types::template LOffsetTo<AnchorMatrix<Types, HBUINT16>>
                mark2Array;             /* Offset to Mark2Array table--from
                                         * beginning of MarkMarkPos subtable */
  সার্ৱজনিক:
  DEFINE_SIZE_STATIC (4 + 4 * Types::LOffset::static_size);

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্) &&
                  mark1Coverage.sanitize (c, এতত্) &&
                  mark2Coverage.sanitize (c, এতত্) &&
                  mark1Array.sanitize (c, এতত্) &&
		  hb_barrier () &&
                  mark2Array.sanitize (c, এতত্, (unsigned পূর্ণাঙ্ক) class_গুনতি_2));
  }

  তার্কিক intersects (ধ্রুৱ hb_set_t *glyphs) ধ্রুৱ
  {
    ঘূৰাই (এতত্+mark1Coverage).intersects (glyphs) &&
           (এতত্+mark2Coverage).intersects (glyphs);
  }

  শূন্য closure_lookups (hb_closure_lookups_context_t *c) ধ্রুৱ {}

  শূন্য collect_variation_indices (hb_collect_variation_indices_context_t *c) ধ্রুৱ
  {
    (এতত্+mark1Array).collect_variation_indices (c, এতত্+mark1Coverage);

    hb_map_t klass_mapping;
    Markclass_closure_and_remap_indexes (এতত্+mark1Coverage, এতত্+mark1Array, *c->glyph_set, &klass_mapping);

    hb_sorted_vector_t<unsigned> mark2_indexes;
    collect_anchor_indexes (এতত্+mark2Coverage, (এতত্+mark2Array).rows,
                            class_গুনতি_2, *c->glyph_set, klass_mapping,
                            mark2_indexes);
    (এতত্+mark2Array).collect_variation_indices (c, mark2_indexes.iter ());
  }

  শূন্য collect_glyphs (hb_collect_glyphs_context_t *c) ধ্রুৱ
  {
    যদি (unlikely (!(এতত্+mark1Coverage).collect_coverage (c->প্ৰবেশ))) ঘূৰাই;
    যদি (unlikely (!(এতত্+mark2Coverage).collect_coverage (c->প্ৰবেশ))) ঘূৰাই;
  }

  শূন্য collect_second_glyphs (hb_set_digest_t *digest) ধ্রুৱ
  { (এতত্+mark2Coverage).collect_coverage (digest); }

  ধ্রুৱ Coverage &get_coverage () ধ্রুৱ { ঘূৰাই এতত্+mark1Coverage; }

  তার্কিক apply (hb_ot_apply_context_t *c) ধ্রুৱ
  {
    TRACE_APPLY (এতত্);
    hb_buffer_t *buffer = c->buffer;
    unsigned পূর্ণাঙ্ক mark1_index = (এতত্+mark1Coverage).get_coverage  (buffer->cur().codepoint);
    যদি (likely (mark1_index == NOT_COVERED)) return_trace (মিছা);

    /* now we search backwards for a suitable mark glyph until a non-mark glyph */
    auto &skippy_iter = c->iter_প্ৰবেশ;
    skippy_iter.reset_fast (buffer->idx);
    skippy_iter.set_lookup_props (c->lookup_props & ~(uint32_t)LookupFlag::IgnoreFlags);
    unsigned unsafe_from;
    যদি (unlikely (!skippy_iter.prev (&unsafe_from)))
    {
      buffer->unsafe_to_concat_from_outbuffer (unsafe_from, buffer->idx + 1);
      return_trace (মিছা);
    }

    যদি (likely (!_hb_glyph_info_is_mark (&buffer->info[skippy_iter.idx])))
    {
      buffer->unsafe_to_concat_from_outbuffer (skippy_iter.idx, buffer->idx + 1);
      return_trace (মিছা);
    }

    unsigned পূর্ণাঙ্ক j = skippy_iter.idx;

    unsigned পূর্ণাঙ্ক id1 = _hb_glyph_info_get_lig_id (&buffer->cur());
    unsigned পূর্ণাঙ্ক id2 = _hb_glyph_info_get_lig_id (&buffer->info[j]);
    unsigned পূর্ণাঙ্ক comp1 = _hb_glyph_info_get_lig_comp (&buffer->cur());
    unsigned পূর্ণাঙ্ক comp2 = _hb_glyph_info_get_lig_comp (&buffer->info[j]);

    যদি (likely (id1 == id2))
    {
      যদি (id1 == 0) /* Marks belonging to the same base. */
        গচ্ছ good;
      অন্যথা যদি (comp1 == comp2) /* Marks belonging to the same ligature component. */
        গচ্ছ good;
    }
    অন্যথা
    {
      /* If ligature ids don't match, it may be the case that one of the marks
       * itself is a ligature.  In which case match. */
      যদি ((id1 > 0 && !comp1) || (id2 > 0 && !comp2))
        গচ্ছ good;
    }

    /* Didn't match. */
    buffer->unsafe_to_concat_from_outbuffer (skippy_iter.idx, buffer->idx + 1);
    return_trace (মিছা);

    good:
    unsigned পূর্ণাঙ্ক mark2_index = (এতত্+mark2Coverage).get_coverage  (buffer->info[j].codepoint);
    যদি (mark2_index == NOT_COVERED)
    {
      buffer->unsafe_to_concat_from_outbuffer (skippy_iter.idx, buffer->idx + 1);
      return_trace (মিছা);
    }

    return_trace ((এতত্+mark1Array).apply (c, mark1_index, mark2_index, এতত্+mark2Array, class_গুনতি_2, j));
  }

  তার্কিক subset (hb_subset_context_t *c) ধ্রুৱ
  {
    TRACE_SUBSET (এতত্);
    ধ্রুৱ hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ধ্রুৱ hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*এতত্);
    যদি (unlikely (!c->serializer->extend_min (out))) return_trace (মিছা);
    out->format = format;

    hb_map_t klass_mapping;
    Markclass_closure_and_remap_indexes (এতত্+mark1Coverage, এতত্+mark1Array, glyphset, &klass_mapping);

    যদি (!klass_mapping.get_population ()) return_trace (মিছা);
    out->class_গুনতি_2 = klass_mapping.get_population ();

    auto mark1_iter =
    + hb_zip (এতত্+mark1Coverage, এতত্+mark1Array)
    | hb_filter (glyphset, hb_first)
    ;

    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + mark1_iter
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    যদি (!out->mark1Coverage.serialize_serialize (c->serializer, new_coverage.iter ()))
      return_trace (মিছা);

    যদি (unlikely (!out->mark1Array.serialize_subset (c, mark1Array, এতত্,
						     (এতত্+mark1Coverage).iter (),
						     &klass_mapping)))
      return_trace (মিছা);

    new_coverage.reset ();
    hb_sorted_vector_t<unsigned> mark2_indexes;
    ধ্রুৱ auto &mark2_array = এতত্+mark2Array;
    collect_subset_anchor_indexes (এতত্+mark2Coverage, mark2_array.rows,
                                   class_গুনতি_2, mark2_array.matrixZ.arrayZ,
                                   glyphset, glyph_map, klass_mapping,
                                   new_coverage, mark2_indexes);

    যদি (!new_coverage) return_trace (মিছা);
    যদি (!out->mark2Coverage.serialize_serialize (c->serializer, new_coverage.iter ()))
      return_trace (মিছা);

    return_trace (out->mark2Array.serialize_subset (c, mark2Array, এতত্,
						    new_coverage.length,
						    mark2_indexes.iter ()));

  }
};


}
}
}

#endif /* OT_LAYOUT_GPOS_MARKMARKPOSFORMAT1_HH */
