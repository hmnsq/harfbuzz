#ifndef OT_LAYOUT_GPOS_MARKLIGPOSFORMAT1_HH
#define OT_LAYOUT_GPOS_MARKLIGPOSFORMAT1_HH

#அந்தர்பாவ "LigatureArray.hh"

நாமாகாஶ OT {
நாமாகாஶ Layout {
நாமாகாஶ GPOS_impl {


template <typename Types>
கட்டமைப்பு MarkLigPosFormat1_2
{
  பாதுகாக்கப்பட்ட:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  typename Types::template LOffsetTo<Coverage>
                markCoverage;           /* Offset to Mark Coverage table--from
                                         * beginning of MarkLigPos subtable */
  typename Types::template LOffsetTo<Coverage>
                ligatureCoverage;       /* Offset to Ligature Coverage
                                         * table--from beginning of MarkLigPos
                                         * subtable */
  HBUINT16      class_மொத்தம்_2;             /* Number of defined mark classes */
  typename Types::template LOffsetTo<MarkArray<Types>>
                markArray;              /* Offset to MarkArray table--from
                                         * beginning of MarkLigPos subtable */
  typename Types::template LOffsetTo<LigatureArray<Types>>
                ligatureArray;          /* Offset to LigatureArray table--from
                                         * beginning of MarkLigPos subtable */
  பொது:
  DEFINE_SIZE_STATIC (4 + 4 * Types::LOffset::static_size);

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது) &&
                  markCoverage.sanitize (c, இது) &&
                  ligatureCoverage.sanitize (c, இது) &&
                  markArray.sanitize (c, இது) &&
                  ligatureArray.sanitize (c, இது, (unsigned முழு) class_மொத்தம்_2));
  }

  தருக்க intersects (மாறிலி hb_set_t *glyphs) மாறிலி
  {
    திரும்பு (இது+markCoverage).intersects (glyphs) &&
           (இது+ligatureCoverage).intersects (glyphs);
  }

  வெறுமை closure_lookups (hb_closure_lookups_context_t *c) மாறிலி {}

  வெறுமை collect_variation_indices (hb_collect_variation_indices_context_t *c) மாறிலி
  {
    (இது+markArray).collect_variation_indices (c, இது+markCoverage);

    hb_map_t klass_mapping;
    Markclass_closure_and_remap_indexes (இது+markCoverage, இது+markArray, *c->glyph_set, &klass_mapping);

    (இது+ligatureArray).collect_variation_indices (c, இது+ligatureCoverage,
                                                   class_மொத்தம்_2, klass_mapping);
  }

  வெறுமை collect_glyphs (hb_collect_glyphs_context_t *c) மாறிலி
  {
    எனில் (unlikely (!(இது+markCoverage).collect_coverage (c->உள்ளீடு))) திரும்பு;
    எனில் (unlikely (!(இது+ligatureCoverage).collect_coverage (c->உள்ளீடு))) திரும்பு;
  }

  வெறுமை collect_second_glyphs (hb_set_digest_t *digest) மாறிலி
  { (இது+ligatureCoverage).collect_coverage (digest); }

  மாறிலி Coverage &get_coverage () மாறிலி { திரும்பு இது+markCoverage; }

  நிலையான உள்வரி தருக்க accept (hb_buffer_t *buffer, unsigned idx)
  {
    /* We only want to attach to the first of a MultipleSubst sequence,
     * which might have been ligated into a preceding ligature, and in that
     * case the mark should attach to that ligature.
     * https://github.com/harfbuzz/harfbuzz/issues/4969
     * Reject others... */
    திரும்பு !_hb_glyph_info_multiplied (&buffer->info[idx]) ||
	   0 == _hb_glyph_info_get_lig_comp (&buffer->info[idx]);
  }

  தருக்க apply (hb_ot_apply_context_t *c) மாறிலி
  {
    TRACE_APPLY (இது);
    hb_buffer_t *buffer = c->buffer;
    unsigned முழு mark_index = (இது+markCoverage).get_coverage  (buffer->cur().codepoint);
    எனில் (likely (mark_index == NOT_COVERED)) return_trace (பொய்);

    /* Now we search backwards for a non-mark glyph */

    auto &skippy_iter = c->iter_உள்ளீடு;
    skippy_iter.set_lookup_props (LookupFlag::IgnoreMarks);

    எனில் (c->last_base_until > buffer->idx)
    {
      c->last_base_until = 0;
      c->last_base = -1;
    }
    unsigned j;
    க்கு (j = buffer->idx; j > c->last_base_until; j--)
    {
      auto match = skippy_iter.match (buffer->info[j - 1]);
      எனில் (match == skippy_iter.MATCH)
      {
        // https://github.com/harfbuzz/harfbuzz/issues/4124
	எனில் (!accept (buffer, j - 1) &&
	    NOT_COVERED == (இது+ligatureCoverage).get_coverage  (buffer->info[j - 1].codepoint))
	  match = skippy_iter.SKIP;
      }
      எனில் (match == skippy_iter.MATCH)
      {
	c->last_base = (signed) j - 1;
	நிறுத்து;
      }
    }
    c->last_base_until = buffer->idx;
    எனில் (c->last_base == -1)
    {
      buffer->unsafe_to_concat_from_outbuffer (0, buffer->idx + 1);
      return_trace (பொய்);
    }

    unsigned idx = (unsigned) c->last_base;

    /* Checking that matched glyph is actually a ligature by GDEF is too strong; disabled */
    //if (!_hb_glyph_info_is_ligature (&buffer->info[idx])) { return_trace (false); }

    unsigned முழு lig_index = (இது+ligatureCoverage).get_coverage  (buffer->info[idx].codepoint);
    எனில் (lig_index == NOT_COVERED)
    {
      buffer->unsafe_to_concat_from_outbuffer (idx, buffer->idx + 1);
      return_trace (பொய்);
    }

    மாறிலி LigatureArray<Types>& lig_array = இது+ligatureArray;
    மாறிலி AnchorMatrix<Types, HBUINT16>& lig_attach = lig_array[lig_index];

    /* Find component to attach to */
    unsigned முழு comp_மொத்தம் = lig_attach.rows;
    எனில் (unlikely (!comp_மொத்தம்))
    {
      buffer->unsafe_to_concat_from_outbuffer (idx, buffer->idx + 1);
      return_trace (பொய்);
    }

    /* We must now check whether the ligature ID of the current mark glyph
     * is identical to the ligature ID of the found ligature.  If yes, we
     * can directly use the component index.  If not, we attach the mark
     * glyph to the last component of the ligature. */
    unsigned முழு comp_index;
    unsigned முழு lig_id = _hb_glyph_info_get_lig_id (&buffer->info[idx]);
    unsigned முழு mark_id = _hb_glyph_info_get_lig_id (&buffer->cur());
    unsigned முழு mark_comp = _hb_glyph_info_get_lig_comp (&buffer->cur());
    எனில் (lig_id && lig_id == mark_id && mark_comp > 0)
      comp_index = hb_min (comp_மொத்தம், _hb_glyph_info_get_lig_comp (&buffer->cur())) - 1;
    இல்லையெனில்
      comp_index = comp_மொத்தம் - 1;

    return_trace ((இது+markArray).apply (c, mark_index, comp_index, lig_attach, class_மொத்தம்_2, idx));
  }

  தருக்க subset (hb_subset_context_t *c) மாறிலி
  {
    TRACE_SUBSET (இது);
    மாறிலி hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    மாறிலி hb_map_t &glyph_map = c->plan->glyph_map_gsub;

    auto *out = c->serializer->start_embed (*இது);
    எனில் (unlikely (!c->serializer->extend_min (out))) return_trace (பொய்);
    out->format = format;

    hb_map_t klass_mapping;
    Markclass_closure_and_remap_indexes (இது+markCoverage, இது+markArray, glyphset, &klass_mapping);

    எனில் (!klass_mapping.get_population ()) return_trace (பொய்);
    out->class_மொத்தம்_2 = klass_mapping.get_population ();

    auto mark_iter =
    + hb_zip (இது+markCoverage, hb_range ((unsigned) (இது+markArray).len))
    | hb_filter (glyphset, hb_first)
    ;

    auto new_mark_coverage =
    + mark_iter
    | hb_map_retains_sorting (hb_first)
    | hb_map_retains_sorting (glyph_map)
    ;

    எனில் (!out->markCoverage.serialize_serialize (c->serializer, new_mark_coverage))
      return_trace (பொய்);

    எனில் (unlikely (!out->markArray.serialize_subset (c, markArray, இது,
						    (இது+markCoverage).iter (),
						    &klass_mapping)))
      return_trace (பொய்);

    hb_sorted_vector_t<hb_codepoint_t> new_lig_coverage;
    எனில் (!out->ligatureArray.serialize_subset (c, ligatureArray, இது,
					      hb_iter (இது+ligatureCoverage),
					      class_மொத்தம்_2, &klass_mapping, new_lig_coverage))
      return_trace (பொய்);

    return_trace (out->ligatureCoverage.serialize_serialize (c->serializer, new_lig_coverage.iter ()));
  }

};

}
}
}

#endif /* OT_LAYOUT_GPOS_MARKLIGPOSFORMAT1_HH */
