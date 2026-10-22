#ifndef OT_LAYOUT_GPOS_MARKMARKPOSFORMAT1_HH
#define OT_LAYOUT_GPOS_MARKMARKPOSFORMAT1_HH

#অন্তর্ভাব "MarkMarkPosFormat1.hh"

নামাকাশ OT {
নামাকাশ Layout {
নামাকাশ GPOS_impl {

template <typename Types>
কাঠামো MarkMarkPosFormat1_2
{
  সুরক্ষিত:
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
  সর্বজনীন:
  DEFINE_আকার_STATIC (4 + 4 * Types::LOffset::static_আকার);

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই) &&
                  mark1Coverage.sanitize (c, এই) &&
                  mark2Coverage.sanitize (c, এই) &&
                  mark1Array.sanitize (c, এই) &&
		  hb_barrier () &&
                  mark2Array.sanitize (c, এই, (unsigned পূর্ণসংখ্যা) class_গুনতি_2));
  }

  যৌক্তিক intersects (ধ্রুবক hb_set_t *glyphs) ধ্রুবক
  {
    ফেরত (এই+mark1Coverage).intersects (glyphs) &&
           (এই+mark2Coverage).intersects (glyphs);
  }

  শূন্য closure_lookups (hb_closure_lookups_context_t *c) ধ্রুবক {}

  শূন্য collect_variation_indices (hb_collect_variation_indices_context_t *c) ধ্রুবক
  {
    (এই+mark1Array).collect_variation_indices (c, এই+mark1Coverage);

    hb_map_t klass_mapping;
    Markclass_closure_and_remap_indexes (এই+mark1Coverage, এই+mark1Array, *c->glyph_set, &klass_mapping);

    hb_sorted_vector_t<unsigned> mark2_indexes;
    collect_anchor_indexes (এই+mark2Coverage, (এই+mark2Array).rows,
                            class_গুনতি_2, *c->glyph_set, klass_mapping,
                            mark2_indexes);
    (এই+mark2Array).collect_variation_indices (c, mark2_indexes.iter ());
  }

  শূন্য collect_glyphs (hb_collect_glyphs_context_t *c) ধ্রুবক
  {
    যদি (unlikely (!(এই+mark1Coverage).collect_coverage (c->প্রবেশ))) ফেরত;
    যদি (unlikely (!(এই+mark2Coverage).collect_coverage (c->প্রবেশ))) ফেরত;
  }

  শূন্য collect_second_glyphs (hb_set_digest_t *digest) ধ্রুবক
  { (এই+mark2Coverage).collect_coverage (digest); }

  ধ্রুবক Coverage &get_coverage () ধ্রুবক { ফেরত এই+mark1Coverage; }

  যৌক্তিক apply (hb_ot_apply_context_t *c) ধ্রুবক
  {
    TRACE_APPLY (এই);
    hb_বাফার_t *বাফার = c->বাফার;
    unsigned পূর্ণসংখ্যা mark1_সূচক = (এই+mark1Coverage).get_coverage  (বাফার->cur().codepoint);
    যদি (likely (mark1_সূচক == NOT_COVERED)) return_trace (মিথ্যা);

    /* now we search backwards for a suitable mark glyph until a non-mark glyph */
    auto &skippy_iter = c->iter_প্রবেশ;
    skippy_iter.reset_fast (বাফার->idx);
    skippy_iter.set_lookup_props (c->lookup_props & ~(uint32_t)LookupFlag::IgnoreFlags);
    unsigned unsafe_from;
    যদি (unlikely (!skippy_iter.prev (&unsafe_from)))
    {
      বাফার->unsafe_to_concat_from_outbuffer (unsafe_from, বাফার->idx + 1);
      return_trace (মিথ্যা);
    }

    যদি (likely (!_hb_glyph_info_is_mark (&বাফার->info[skippy_iter.idx])))
    {
      বাফার->unsafe_to_concat_from_outbuffer (skippy_iter.idx, বাফার->idx + 1);
      return_trace (মিথ্যা);
    }

    unsigned পূর্ণসংখ্যা j = skippy_iter.idx;

    unsigned পূর্ণসংখ্যা id1 = _hb_glyph_info_get_lig_id (&বাফার->cur());
    unsigned পূর্ণসংখ্যা id2 = _hb_glyph_info_get_lig_id (&বাফার->info[j]);
    unsigned পূর্ণসংখ্যা comp1 = _hb_glyph_info_get_lig_comp (&বাফার->cur());
    unsigned পূর্ণসংখ্যা comp2 = _hb_glyph_info_get_lig_comp (&বাফার->info[j]);

    যদি (likely (id1 == id2))
    {
      যদি (id1 == 0) /* Marks belonging to the same base. */
        যাও good;
      অন্যথা যদি (comp1 == comp2) /* Marks belonging to the same ligature component. */
        যাও good;
    }
    অন্যথা
    {
      /* If ligature ids don't match, it may be the case that one of the marks
       * itself is a ligature.  In which case match. */
      যদি ((id1 > 0 && !comp1) || (id2 > 0 && !comp2))
        যাও good;
    }

    /* Didn't match. */
    বাফার->unsafe_to_concat_from_outbuffer (skippy_iter.idx, বাফার->idx + 1);
    return_trace (মিথ্যা);

    good:
    unsigned পূর্ণসংখ্যা mark2_সূচক = (এই+mark2Coverage).get_coverage  (বাফার->info[j].codepoint);
    যদি (mark2_সূচক == NOT_COVERED)
    {
      বাফার->unsafe_to_concat_from_outbuffer (skippy_iter.idx, বাফার->idx + 1);
      return_trace (মিথ্যা);
    }

    return_trace ((এই+mark1Array).apply (c, mark1_সূচক, mark2_সূচক, এই+mark2Array, class_গুনতি_2, j));
  }

  যৌক্তিক subset (hb_subset_context_t *c) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    ধ্রুবক hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ধ্রুবক hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*এই);
    যদি (unlikely (!c->serializer->extend_min (out))) return_trace (মিথ্যা);
    out->format = format;

    hb_map_t klass_mapping;
    Markclass_closure_and_remap_indexes (এই+mark1Coverage, এই+mark1Array, glyphset, &klass_mapping);

    যদি (!klass_mapping.get_population ()) return_trace (মিথ্যা);
    out->class_গুনতি_2 = klass_mapping.get_population ();

    auto mark1_iter =
    + hb_zip (এই+mark1Coverage, এই+mark1Array)
    | hb_filter (glyphset, hb_first)
    ;

    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + mark1_iter
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    যদি (!out->mark1Coverage.serialize_serialize (c->serializer, new_coverage.iter ()))
      return_trace (মিথ্যা);

    যদি (unlikely (!out->mark1Array.serialize_subset (c, mark1Array, এই,
						     (এই+mark1Coverage).iter (),
						     &klass_mapping)))
      return_trace (মিথ্যা);

    new_coverage.reset ();
    hb_sorted_vector_t<unsigned> mark2_indexes;
    ধ্রুবক auto &mark2_array = এই+mark2Array;
    collect_subset_anchor_indexes (এই+mark2Coverage, mark2_array.rows,
                                   class_গুনতি_2, mark2_array.matrixZ.arrayZ,
                                   glyphset, glyph_map, klass_mapping,
                                   new_coverage, mark2_indexes);

    যদি (!new_coverage) return_trace (মিথ্যা);
    যদি (!out->mark2Coverage.serialize_serialize (c->serializer, new_coverage.iter ()))
      return_trace (মিথ্যা);

    return_trace (out->mark2Array.serialize_subset (c, mark2Array, এই,
						    new_coverage.দৈর্ঘ্য,
						    mark2_indexes.iter ()));

  }
};


}
}
}

#endif /* OT_LAYOUT_GPOS_MARKMARKPOSFORMAT1_HH */
