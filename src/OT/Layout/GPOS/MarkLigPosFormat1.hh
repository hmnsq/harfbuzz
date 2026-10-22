#ifndef OT_LAYOUT_GPOS_MARKLIGPOSFORMAT1_HH
#define OT_LAYOUT_GPOS_MARKLIGPOSFORMAT1_HH

#অন্তর্ভাৱ "LigatureArray.hh"

নামাকাশ OT {
নামাকাশ Layout {
নামাকাশ GPOS_impl {


template <typename Types>
সংরচনা MarkLigPosFormat1_2
{
  রক্ষিত:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  typename Types::template LOffsetTo<Coverage>
                markCoverage;           /* Offset to Mark Coverage table--from
                                         * beginning of MarkLigPos subtable */
  typename Types::template LOffsetTo<Coverage>
                ligatureCoverage;       /* Offset to Ligature Coverage
                                         * table--from beginning of MarkLigPos
                                         * subtable */
  HBUINT16      class_গুনতি_2;             /* Number of defined mark classes */
  typename Types::template LOffsetTo<MarkArray<Types>>
                markArray;              /* Offset to MarkArray table--from
                                         * beginning of MarkLigPos subtable */
  typename Types::template LOffsetTo<LigatureArray<Types>>
                ligatureArray;          /* Offset to LigatureArray table--from
                                         * beginning of MarkLigPos subtable */
  সার্ৱজনিক:
  DEFINE_SIZE_STATIC (4 + 4 * Types::LOffset::static_size);

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্) &&
                  markCoverage.sanitize (c, এতত্) &&
                  ligatureCoverage.sanitize (c, এতত্) &&
                  markArray.sanitize (c, এতত্) &&
                  ligatureArray.sanitize (c, এতত্, (unsigned পূর্ণাঙ্ক) class_গুনতি_2));
  }

  তার্কিক intersects (ধ্রুৱ hb_set_t *glyphs) ধ্রুৱ
  {
    ঘূৰাই (এতত্+markCoverage).intersects (glyphs) &&
           (এতত্+ligatureCoverage).intersects (glyphs);
  }

  শূন্য closure_lookups (hb_closure_lookups_context_t *c) ধ্রুৱ {}

  শূন্য collect_variation_indices (hb_collect_variation_indices_context_t *c) ধ্রুৱ
  {
    (এতত্+markArray).collect_variation_indices (c, এতত্+markCoverage);

    hb_map_t klass_mapping;
    Markclass_closure_and_remap_indexes (এতত্+markCoverage, এতত্+markArray, *c->glyph_set, &klass_mapping);

    (এতত্+ligatureArray).collect_variation_indices (c, এতত্+ligatureCoverage,
                                                   class_গুনতি_2, klass_mapping);
  }

  শূন্য collect_glyphs (hb_collect_glyphs_context_t *c) ধ্রুৱ
  {
    যদি (unlikely (!(এতত্+markCoverage).collect_coverage (c->প্ৰবেশ))) ঘূৰাই;
    যদি (unlikely (!(এতত্+ligatureCoverage).collect_coverage (c->প্ৰবেশ))) ঘূৰাই;
  }

  শূন্য collect_second_glyphs (hb_set_digest_t *digest) ধ্রুৱ
  { (এতত্+ligatureCoverage).collect_coverage (digest); }

  ধ্রুৱ Coverage &get_coverage () ধ্রুৱ { ঘূৰাই এতত্+markCoverage; }

  স্থির অন্তঃস্থ তার্কিক accept (hb_buffer_t *buffer, unsigned idx)
  {
    /* We only want to attach to the first of a MultipleSubst sequence,
     * which might have been ligated into a preceding ligature, and in that
     * case the mark should attach to that ligature.
     * https://github.com/harfbuzz/harfbuzz/issues/4969
     * Reject others... */
    ঘূৰাই !_hb_glyph_info_multiplied (&buffer->info[idx]) ||
	   0 == _hb_glyph_info_get_lig_comp (&buffer->info[idx]);
  }

  তার্কিক apply (hb_ot_apply_context_t *c) ধ্রুৱ
  {
    TRACE_APPLY (এতত্);
    hb_buffer_t *buffer = c->buffer;
    unsigned পূর্ণাঙ্ক mark_index = (এতত্+markCoverage).get_coverage  (buffer->cur().codepoint);
    যদি (likely (mark_index == NOT_COVERED)) return_trace (মিছা);

    /* Now we search backwards for a non-mark glyph */

    auto &skippy_iter = c->iter_প্ৰবেশ;
    skippy_iter.set_lookup_props (LookupFlag::IgnoreMarks);

    যদি (c->last_base_until > buffer->idx)
    {
      c->last_base_until = 0;
      c->last_base = -1;
    }
    unsigned j;
    বাবে (j = buffer->idx; j > c->last_base_until; j--)
    {
      auto match = skippy_iter.match (buffer->info[j - 1]);
      যদি (match == skippy_iter.MATCH)
      {
        // https://github.com/harfbuzz/harfbuzz/issues/4124
	যদি (!accept (buffer, j - 1) &&
	    NOT_COVERED == (এতত্+ligatureCoverage).get_coverage  (buffer->info[j - 1].codepoint))
	  match = skippy_iter.SKIP;
      }
      যদি (match == skippy_iter.MATCH)
      {
	c->last_base = (signed) j - 1;
	ভঙ্গ;
      }
    }
    c->last_base_until = buffer->idx;
    যদি (c->last_base == -1)
    {
      buffer->unsafe_to_concat_from_outbuffer (0, buffer->idx + 1);
      return_trace (মিছা);
    }

    unsigned idx = (unsigned) c->last_base;

    /* Checking that matched glyph is actually a ligature by GDEF is too strong; disabled */
    //if (!_hb_glyph_info_is_ligature (&buffer->info[idx])) { return_trace (false); }

    unsigned পূর্ণাঙ্ক lig_index = (এতত্+ligatureCoverage).get_coverage  (buffer->info[idx].codepoint);
    যদি (lig_index == NOT_COVERED)
    {
      buffer->unsafe_to_concat_from_outbuffer (idx, buffer->idx + 1);
      return_trace (মিছা);
    }

    ধ্রুৱ LigatureArray<Types>& lig_array = এতত্+ligatureArray;
    ধ্রুৱ AnchorMatrix<Types, HBUINT16>& lig_attach = lig_array[lig_index];

    /* Find component to attach to */
    unsigned পূর্ণাঙ্ক comp_গুনতি = lig_attach.rows;
    যদি (unlikely (!comp_গুনতি))
    {
      buffer->unsafe_to_concat_from_outbuffer (idx, buffer->idx + 1);
      return_trace (মিছা);
    }

    /* We must now check whether the ligature ID of the current mark glyph
     * is identical to the ligature ID of the found ligature.  If yes, we
     * can directly use the component index.  If not, we attach the mark
     * glyph to the last component of the ligature. */
    unsigned পূর্ণাঙ্ক comp_index;
    unsigned পূর্ণাঙ্ক lig_id = _hb_glyph_info_get_lig_id (&buffer->info[idx]);
    unsigned পূর্ণাঙ্ক mark_id = _hb_glyph_info_get_lig_id (&buffer->cur());
    unsigned পূর্ণাঙ্ক mark_comp = _hb_glyph_info_get_lig_comp (&buffer->cur());
    যদি (lig_id && lig_id == mark_id && mark_comp > 0)
      comp_index = hb_min (comp_গুনতি, _hb_glyph_info_get_lig_comp (&buffer->cur())) - 1;
    অন্যথা
      comp_index = comp_গুনতি - 1;

    return_trace ((এতত্+markArray).apply (c, mark_index, comp_index, lig_attach, class_গুনতি_2, idx));
  }

  তার্কিক subset (hb_subset_context_t *c) ধ্রুৱ
  {
    TRACE_SUBSET (এতত্);
    ধ্রুৱ hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ধ্রুৱ hb_map_t &glyph_map = c->plan->glyph_map_gsub;

    auto *out = c->serializer->start_embed (*এতত্);
    যদি (unlikely (!c->serializer->extend_min (out))) return_trace (মিছা);
    out->format = format;

    hb_map_t klass_mapping;
    Markclass_closure_and_remap_indexes (এতত্+markCoverage, এতত্+markArray, glyphset, &klass_mapping);

    যদি (!klass_mapping.get_population ()) return_trace (মিছা);
    out->class_গুনতি_2 = klass_mapping.get_population ();

    auto mark_iter =
    + hb_zip (এতত্+markCoverage, hb_range ((unsigned) (এতত্+markArray).len))
    | hb_filter (glyphset, hb_first)
    ;

    auto new_mark_coverage =
    + mark_iter
    | hb_map_retains_sorting (hb_first)
    | hb_map_retains_sorting (glyph_map)
    ;

    যদি (!out->markCoverage.serialize_serialize (c->serializer, new_mark_coverage))
      return_trace (মিছা);

    যদি (unlikely (!out->markArray.serialize_subset (c, markArray, এতত্,
						    (এতত্+markCoverage).iter (),
						    &klass_mapping)))
      return_trace (মিছা);

    hb_sorted_vector_t<hb_codepoint_t> new_lig_coverage;
    যদি (!out->ligatureArray.serialize_subset (c, ligatureArray, এতত্,
					      hb_iter (এতত্+ligatureCoverage),
					      class_গুনতি_2, &klass_mapping, new_lig_coverage))
      return_trace (মিছা);

    return_trace (out->ligatureCoverage.serialize_serialize (c->serializer, new_lig_coverage.iter ()));
  }

};

}
}
}

#endif /* OT_LAYOUT_GPOS_MARKLIGPOSFORMAT1_HH */
