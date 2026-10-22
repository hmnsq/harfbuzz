#ifndef OT_LAYOUT_GPOS_MARKMARKPOSFORMAT1_HH
#define OT_LAYOUT_GPOS_MARKMARKPOSFORMAT1_HH

#شامل "MarkMarkPosFormat1.hh"

فضاینام OT {
فضاینام Layout {
فضاینام GPOS_impl {

template <typename Types>
ساختار MarkMarkPosFormat1_2
{
  محافظتشده:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  typename Types::template LOffsetTo<Coverage>
                mark1Coverage;          /* Offset to Combining Mark1 Coverage
                                         * table--from beginning of MarkMarkPos
                                         * subtable */
  typename Types::template LOffsetTo<Coverage>
                mark2Coverage;          /* Offset to Combining Mark2 Coverage
                                         * table--from beginning of MarkMarkPos
                                         * subtable */
  HBUINT16      class_شمار_2;             /* Number of defined mark classes */
  typename Types::template LOffsetTo<MarkArray<Types>>
                mark1Array;             /* Offset to Mark1Array table--from
                                         * beginning of MarkMarkPos subtable */
  typename Types::template LOffsetTo<AnchorMatrix<Types, HBUINT16>>
                mark2Array;             /* Offset to Mark2Array table--from
                                         * beginning of MarkMarkPos subtable */
  عمومی:
  DEFINE_SIZE_STATIC (4 + 4 * Types::LOffset::static_size);

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این) &&
                  mark1Coverage.sanitize (c, این) &&
                  mark2Coverage.sanitize (c, این) &&
                  mark1Array.sanitize (c, این) &&
		  hb_barrier () &&
                  mark2Array.sanitize (c, این, (unsigned صحیح) class_شمار_2));
  }

  منطقی intersects (ثابت hb_set_t *glyphs) ثابت
  {
    بازگشت (این+mark1Coverage).intersects (glyphs) &&
           (این+mark2Coverage).intersects (glyphs);
  }

  تهی closure_lookups (hb_closure_lookups_context_t *c) ثابت {}

  تهی collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    (این+mark1Array).collect_variation_indices (c, این+mark1Coverage);

    hb_map_t klass_mapping;
    Markclass_closure_and_remap_indexes (این+mark1Coverage, این+mark1Array, *c->glyph_set, &klass_mapping);

    hb_sorted_vector_t<unsigned> mark2_indexes;
    collect_anchor_indexes (این+mark2Coverage, (این+mark2Array).rows,
                            class_شمار_2, *c->glyph_set, klass_mapping,
                            mark2_indexes);
    (این+mark2Array).collect_variation_indices (c, mark2_indexes.iter ());
  }

  تهی collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    اگر (unlikely (!(این+mark1Coverage).collect_coverage (c->درون_داد))) بازگشت;
    اگر (unlikely (!(این+mark2Coverage).collect_coverage (c->درون_داد))) بازگشت;
  }

  تهی collect_second_glyphs (hb_set_digest_t *digest) ثابت
  { (این+mark2Coverage).collect_coverage (digest); }

  ثابت Coverage &get_coverage () ثابت { بازگشت این+mark1Coverage; }

  منطقی apply (hb_ot_apply_context_t *c) ثابت
  {
    TRACE_APPLY (این);
    hb_buffer_t *buffer = c->buffer;
    unsigned صحیح mark1_index = (این+mark1Coverage).get_coverage  (buffer->cur().codepoint);
    اگر (likely (mark1_index == NOT_COVERED)) return_trace (نادرست);

    /* now we search backwards for a suitable mark glyph until a non-mark glyph */
    auto &skippy_iter = c->iter_درون_داد;
    skippy_iter.reset_fast (buffer->idx);
    skippy_iter.set_lookup_props (c->lookup_props & ~(uint32_t)LookupFlag::IgnoreFlags);
    unsigned unsafe_from;
    اگر (unlikely (!skippy_iter.prev (&unsafe_from)))
    {
      buffer->unsafe_to_concat_from_outbuffer (unsafe_from, buffer->idx + 1);
      return_trace (نادرست);
    }

    اگر (likely (!_hb_glyph_info_is_mark (&buffer->info[skippy_iter.idx])))
    {
      buffer->unsafe_to_concat_from_outbuffer (skippy_iter.idx, buffer->idx + 1);
      return_trace (نادرست);
    }

    unsigned صحیح j = skippy_iter.idx;

    unsigned صحیح id1 = _hb_glyph_info_get_lig_id (&buffer->cur());
    unsigned صحیح id2 = _hb_glyph_info_get_lig_id (&buffer->info[j]);
    unsigned صحیح comp1 = _hb_glyph_info_get_lig_comp (&buffer->cur());
    unsigned صحیح comp2 = _hb_glyph_info_get_lig_comp (&buffer->info[j]);

    اگر (likely (id1 == id2))
    {
      اگر (id1 == 0) /* Marks belonging to the same base. */
        برو good;
      وگرنه اگر (comp1 == comp2) /* Marks belonging to the same ligature component. */
        برو good;
    }
    وگرنه
    {
      /* If ligature ids don't match, it may be the case that one of the marks
       * itself is a ligature.  In which case match. */
      اگر ((id1 > 0 && !comp1) || (id2 > 0 && !comp2))
        برو good;
    }

    /* Didn't match. */
    buffer->unsafe_to_concat_from_outbuffer (skippy_iter.idx, buffer->idx + 1);
    return_trace (نادرست);

    good:
    unsigned صحیح mark2_index = (این+mark2Coverage).get_coverage  (buffer->info[j].codepoint);
    اگر (mark2_index == NOT_COVERED)
    {
      buffer->unsafe_to_concat_from_outbuffer (skippy_iter.idx, buffer->idx + 1);
      return_trace (نادرست);
    }

    return_trace ((این+mark1Array).apply (c, mark1_index, mark2_index, این+mark2Array, class_شمار_2, j));
  }

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (این);
    ثابت hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*این);
    اگر (unlikely (!c->serializer->extend_min (out))) return_trace (نادرست);
    out->format = format;

    hb_map_t klass_mapping;
    Markclass_closure_and_remap_indexes (این+mark1Coverage, این+mark1Array, glyphset, &klass_mapping);

    اگر (!klass_mapping.get_population ()) return_trace (نادرست);
    out->class_شمار_2 = klass_mapping.get_population ();

    auto mark1_iter =
    + hb_zip (این+mark1Coverage, این+mark1Array)
    | hb_filter (glyphset, hb_first)
    ;

    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + mark1_iter
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    اگر (!out->mark1Coverage.serialize_serialize (c->serializer, new_coverage.iter ()))
      return_trace (نادرست);

    اگر (unlikely (!out->mark1Array.serialize_subset (c, mark1Array, این,
						     (این+mark1Coverage).iter (),
						     &klass_mapping)))
      return_trace (نادرست);

    new_coverage.reset ();
    hb_sorted_vector_t<unsigned> mark2_indexes;
    ثابت auto &mark2_array = این+mark2Array;
    collect_subset_anchor_indexes (این+mark2Coverage, mark2_array.rows,
                                   class_شمار_2, mark2_array.matrixZ.arrayZ,
                                   glyphset, glyph_map, klass_mapping,
                                   new_coverage, mark2_indexes);

    اگر (!new_coverage) return_trace (نادرست);
    اگر (!out->mark2Coverage.serialize_serialize (c->serializer, new_coverage.iter ()))
      return_trace (نادرست);

    return_trace (out->mark2Array.serialize_subset (c, mark2Array, این,
						    new_coverage.length,
						    mark2_indexes.iter ()));

  }
};


}
}
}

#endif /* OT_LAYOUT_GPOS_MARKMARKPOSFORMAT1_HH */
