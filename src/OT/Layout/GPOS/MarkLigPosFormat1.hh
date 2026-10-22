#ifndef OT_LAYOUT_GPOS_MARKLIGPOSFORMAT1_HH
#define OT_LAYOUT_GPOS_MARKLIGPOSFORMAT1_HH

#ಅನ್ತರ್ಭಾವ "LigatureArray.hh"

ನಾಮಾಕಾಶ OT {
ನಾಮಾಕಾಶ Layout {
ನಾಮಾಕಾಶ GPOS_impl {


template <typename Types>
ಸಂರಚನಾ MarkLigPosFormat1_2
{
  ರಕ್ಷಿತ:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  typename Types::template LOffsetTo<Coverage>
                markCoverage;           /* Offset to Mark Coverage table--from
                                         * beginning of MarkLigPos subtable */
  typename Types::template LOffsetTo<Coverage>
                ligatureCoverage;       /* Offset to Ligature Coverage
                                         * table--from beginning of MarkLigPos
                                         * subtable */
  HBUINT16      class_ಎಣಿಕೆ_2;             /* Number of defined mark classes */
  typename Types::template LOffsetTo<MarkArray<Types>>
                markArray;              /* Offset to MarkArray table--from
                                         * beginning of MarkLigPos subtable */
  typename Types::template LOffsetTo<LigatureArray<Types>>
                ligatureArray;          /* Offset to LigatureArray table--from
                                         * beginning of MarkLigPos subtable */
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (4 + 4 * Types::LOffset::static_size);

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್) &&
                  markCoverage.sanitize (c, ಏತತ್) &&
                  ligatureCoverage.sanitize (c, ಏತತ್) &&
                  markArray.sanitize (c, ಏತತ್) &&
                  ligatureArray.sanitize (c, ಏತತ್, (unsigned ಪೂರ್ಣಾಙ್ಕ) class_ಎಣಿಕೆ_2));
  }

  ತಾರ್ಕಿಕ intersects (ಧ್ರುವ hb_set_t *glyphs) ಧ್ರುವ
  {
    ಪ್ರತ್ಯಾಗಮ (ಏತತ್+markCoverage).intersects (glyphs) &&
           (ಏತತ್+ligatureCoverage).intersects (glyphs);
  }

  ಶೂನ್ಯ closure_lookups (hb_closure_lookups_context_t *c) ಧ್ರುವ {}

  ಶೂನ್ಯ collect_variation_indices (hb_collect_variation_indices_context_t *c) ಧ್ರುವ
  {
    (ಏತತ್+markArray).collect_variation_indices (c, ಏತತ್+markCoverage);

    hb_map_t klass_mapping;
    Markclass_closure_and_remap_indexes (ಏತತ್+markCoverage, ಏತತ್+markArray, *c->glyph_set, &klass_mapping);

    (ಏತತ್+ligatureArray).collect_variation_indices (c, ಏತತ್+ligatureCoverage,
                                                   class_ಎಣಿಕೆ_2, klass_mapping);
  }

  ಶೂನ್ಯ collect_glyphs (hb_collect_glyphs_context_t *c) ಧ್ರುವ
  {
    ಯದಿ (unlikely (!(ಏತತ್+markCoverage).collect_coverage (c->ಒಳಹರಿವು))) ಪ್ರತ್ಯಾಗಮ;
    ಯದಿ (unlikely (!(ಏತತ್+ligatureCoverage).collect_coverage (c->ಒಳಹರಿವು))) ಪ್ರತ್ಯಾಗಮ;
  }

  ಶೂನ್ಯ collect_second_glyphs (hb_set_digest_t *digest) ಧ್ರುವ
  { (ಏತತ್+ligatureCoverage).collect_coverage (digest); }

  ಧ್ರುವ Coverage &get_coverage () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ ಏತತ್+markCoverage; }

  ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ತಾರ್ಕಿಕ accept (hb_buffer_t *buffer, unsigned idx)
  {
    /* We only want to attach to the first of a MultipleSubst sequence,
     * which might have been ligated into a preceding ligature, and in that
     * case the mark should attach to that ligature.
     * https://github.com/harfbuzz/harfbuzz/issues/4969
     * Reject others... */
    ಪ್ರತ್ಯಾಗಮ !_hb_glyph_info_multiplied (&buffer->info[idx]) ||
	   0 == _hb_glyph_info_get_lig_comp (&buffer->info[idx]);
  }

  ತಾರ್ಕಿಕ apply (hb_ot_apply_context_t *c) ಧ್ರುವ
  {
    TRACE_APPLY (ಏತತ್);
    hb_buffer_t *buffer = c->buffer;
    unsigned ಪೂರ್ಣಾಙ್ಕ mark_index = (ಏತತ್+markCoverage).get_coverage  (buffer->cur().codepoint);
    ಯದಿ (likely (mark_index == NOT_COVERED)) return_trace (ಅಸತ್ಯ);

    /* Now we search backwards for a non-mark glyph */

    auto &skippy_iter = c->iter_ಒಳಹರಿವು;
    skippy_iter.set_lookup_props (LookupFlag::IgnoreMarks);

    ಯದಿ (c->last_base_until > buffer->idx)
    {
      c->last_base_until = 0;
      c->last_base = -1;
    }
    unsigned j;
    ಪ್ರತಿ (j = buffer->idx; j > c->last_base_until; j--)
    {
      auto match = skippy_iter.match (buffer->info[j - 1]);
      ಯದಿ (match == skippy_iter.MATCH)
      {
        // https://github.com/harfbuzz/harfbuzz/issues/4124
	ಯದಿ (!accept (buffer, j - 1) &&
	    NOT_COVERED == (ಏತತ್+ligatureCoverage).get_coverage  (buffer->info[j - 1].codepoint))
	  match = skippy_iter.SKIP;
      }
      ಯದಿ (match == skippy_iter.MATCH)
      {
	c->last_base = (signed) j - 1;
	ಭಙ್ಗ;
      }
    }
    c->last_base_until = buffer->idx;
    ಯದಿ (c->last_base == -1)
    {
      buffer->unsafe_to_concat_from_outbuffer (0, buffer->idx + 1);
      return_trace (ಅಸತ್ಯ);
    }

    unsigned idx = (unsigned) c->last_base;

    /* Checking that matched glyph is actually a ligature by GDEF is too strong; disabled */
    //if (!_hb_glyph_info_is_ligature (&buffer->info[idx])) { return_trace (false); }

    unsigned ಪೂರ್ಣಾಙ್ಕ lig_index = (ಏತತ್+ligatureCoverage).get_coverage  (buffer->info[idx].codepoint);
    ಯದಿ (lig_index == NOT_COVERED)
    {
      buffer->unsafe_to_concat_from_outbuffer (idx, buffer->idx + 1);
      return_trace (ಅಸತ್ಯ);
    }

    ಧ್ರುವ LigatureArray<Types>& lig_array = ಏತತ್+ligatureArray;
    ಧ್ರುವ AnchorMatrix<Types, HBUINT16>& lig_attach = lig_array[lig_index];

    /* Find component to attach to */
    unsigned ಪೂರ್ಣಾಙ್ಕ comp_ಎಣಿಕೆ = lig_attach.rows;
    ಯದಿ (unlikely (!comp_ಎಣಿಕೆ))
    {
      buffer->unsafe_to_concat_from_outbuffer (idx, buffer->idx + 1);
      return_trace (ಅಸತ್ಯ);
    }

    /* We must now check whether the ligature ID of the current mark glyph
     * is identical to the ligature ID of the found ligature.  If yes, we
     * can directly use the component index.  If not, we attach the mark
     * glyph to the last component of the ligature. */
    unsigned ಪೂರ್ಣಾಙ್ಕ comp_index;
    unsigned ಪೂರ್ಣಾಙ್ಕ lig_id = _hb_glyph_info_get_lig_id (&buffer->info[idx]);
    unsigned ಪೂರ್ಣಾಙ್ಕ mark_id = _hb_glyph_info_get_lig_id (&buffer->cur());
    unsigned ಪೂರ್ಣಾಙ್ಕ mark_comp = _hb_glyph_info_get_lig_comp (&buffer->cur());
    ಯದಿ (lig_id && lig_id == mark_id && mark_comp > 0)
      comp_index = hb_min (comp_ಎಣಿಕೆ, _hb_glyph_info_get_lig_comp (&buffer->cur())) - 1;
    ಅನ್ಯಥಾ
      comp_index = comp_ಎಣಿಕೆ - 1;

    return_trace ((ಏತತ್+markArray).apply (c, mark_index, comp_index, lig_attach, class_ಎಣಿಕೆ_2, idx));
  }

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    ಧ್ರುವ hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ಧ್ರುವ hb_map_t &glyph_map = c->plan->glyph_map_gsub;

    auto *out = c->serializer->start_embed (*ಏತತ್);
    ಯದಿ (unlikely (!c->serializer->extend_min (out))) return_trace (ಅಸತ್ಯ);
    out->format = format;

    hb_map_t klass_mapping;
    Markclass_closure_and_remap_indexes (ಏತತ್+markCoverage, ಏತತ್+markArray, glyphset, &klass_mapping);

    ಯದಿ (!klass_mapping.get_population ()) return_trace (ಅಸತ್ಯ);
    out->class_ಎಣಿಕೆ_2 = klass_mapping.get_population ();

    auto mark_iter =
    + hb_zip (ಏತತ್+markCoverage, hb_range ((unsigned) (ಏತತ್+markArray).len))
    | hb_filter (glyphset, hb_first)
    ;

    auto new_mark_coverage =
    + mark_iter
    | hb_map_retains_sorting (hb_first)
    | hb_map_retains_sorting (glyph_map)
    ;

    ಯದಿ (!out->markCoverage.serialize_serialize (c->serializer, new_mark_coverage))
      return_trace (ಅಸತ್ಯ);

    ಯದಿ (unlikely (!out->markArray.serialize_subset (c, markArray, ಏತತ್,
						    (ಏತತ್+markCoverage).iter (),
						    &klass_mapping)))
      return_trace (ಅಸತ್ಯ);

    hb_sorted_vector_t<hb_codepoint_t> new_lig_coverage;
    ಯದಿ (!out->ligatureArray.serialize_subset (c, ligatureArray, ಏತತ್,
					      hb_iter (ಏತತ್+ligatureCoverage),
					      class_ಎಣಿಕೆ_2, &klass_mapping, new_lig_coverage))
      return_trace (ಅಸತ್ಯ);

    return_trace (out->ligatureCoverage.serialize_serialize (c->serializer, new_lig_coverage.iter ()));
  }

};

}
}
}

#endif /* OT_LAYOUT_GPOS_MARKLIGPOSFORMAT1_HH */
