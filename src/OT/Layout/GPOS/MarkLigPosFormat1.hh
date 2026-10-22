#ifndef OT_LAYOUT_GPOS_MARKLIGPOSFORMAT1_HH
#define OT_LAYOUT_GPOS_MARKLIGPOSFORMAT1_HH

#ଅନ୍ତର୍ଭାଵ "LigatureArray.hh"

ନାମାକାଶ OT {
ନାମାକାଶ Layout {
ନାମାକାଶ GPOS_impl {


template <typename Types>
ସଂରଚନା MarkLigPosFormat1_2
{
  ସୁରକ୍ଷିତ:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  typename Types::template LOffsetTo<Coverage>
                markCoverage;           /* Offset to Mark Coverage table--from
                                         * beginning of MarkLigPos subtable */
  typename Types::template LOffsetTo<Coverage>
                ligatureCoverage;       /* Offset to Ligature Coverage
                                         * table--from beginning of MarkLigPos
                                         * subtable */
  HBUINT16      class_ଗଣନା_2;             /* Number of defined mark classes */
  typename Types::template LOffsetTo<MarkArray<Types>>
                markArray;              /* Offset to MarkArray table--from
                                         * beginning of MarkLigPos subtable */
  typename Types::template LOffsetTo<LigatureArray<Types>>
                ligatureArray;          /* Offset to LigatureArray table--from
                                         * beginning of MarkLigPos subtable */
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (4 + 4 * Types::LOffset::static_size);

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି) &&
                  markCoverage.sanitize (c, ଏହି) &&
                  ligatureCoverage.sanitize (c, ଏହି) &&
                  markArray.sanitize (c, ଏହି) &&
                  ligatureArray.sanitize (c, ଏହି, (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା) class_ଗଣନା_2));
  }

  ତାର୍କିକ intersects (ଅଚଳ hb_set_t *glyphs) ଅଚଳ
  {
    ଫେରନ୍ତୁ (ଏହି+markCoverage).intersects (glyphs) &&
           (ଏହି+ligatureCoverage).intersects (glyphs);
  }

  ଶୂନ୍ୟ closure_lookups (hb_closure_lookups_context_t *c) ଅଚଳ {}

  ଶୂନ୍ୟ collect_variation_indices (hb_collect_variation_indices_context_t *c) ଅଚଳ
  {
    (ଏହି+markArray).collect_variation_indices (c, ଏହି+markCoverage);

    hb_map_t klass_mapping;
    Markclass_closure_and_remap_indexes (ଏହି+markCoverage, ଏହି+markArray, *c->glyph_set, &klass_mapping);

    (ଏହି+ligatureArray).collect_variation_indices (c, ଏହି+ligatureCoverage,
                                                   class_ଗଣନା_2, klass_mapping);
  }

  ଶୂନ୍ୟ collect_glyphs (hb_collect_glyphs_context_t *c) ଅଚଳ
  {
    ଯଦି (unlikely (!(ଏହି+markCoverage).collect_coverage (c->ନିବେଶ))) ଫେରନ୍ତୁ;
    ଯଦି (unlikely (!(ଏହି+ligatureCoverage).collect_coverage (c->ନିବେଶ))) ଫେରନ୍ତୁ;
  }

  ଶୂନ୍ୟ collect_second_glyphs (hb_set_digest_t *digest) ଅଚଳ
  { (ଏହି+ligatureCoverage).collect_coverage (digest); }

  ଅଚଳ Coverage &get_coverage () ଅଚଳ { ଫେରନ୍ତୁ ଏହି+markCoverage; }

  ସ୍ଥିର ଧାଡ଼ିଭିତରେ ତାର୍କିକ accept (hb_buffer_t *buffer, unsigned idx)
  {
    /* We only want to attach to the first of a MultipleSubst sequence,
     * which might have been ligated into a preceding ligature, and in that
     * case the mark should attach to that ligature.
     * https://github.com/harfbuzz/harfbuzz/issues/4969
     * Reject others... */
    ଫେରନ୍ତୁ !_hb_glyph_info_multiplied (&buffer->info[idx]) ||
	   0 == _hb_glyph_info_get_lig_comp (&buffer->info[idx]);
  }

  ତାର୍କିକ apply (hb_ot_apply_context_t *c) ଅଚଳ
  {
    TRACE_APPLY (ଏହି);
    hb_buffer_t *buffer = c->buffer;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା mark_index = (ଏହି+markCoverage).get_coverage  (buffer->cur().codepoint);
    ଯଦି (likely (mark_index == NOT_COVERED)) return_trace (ମିଥ୍ୟା);

    /* Now we search backwards for a non-mark glyph */

    auto &skippy_iter = c->iter_ନିବେଶ;
    skippy_iter.set_lookup_props (LookupFlag::IgnoreMarks);

    ଯଦି (c->last_base_until > buffer->idx)
    {
      c->last_base_until = 0;
      c->last_base = -1;
    }
    unsigned j;
    ପାଇଁ (j = buffer->idx; j > c->last_base_until; j--)
    {
      auto match = skippy_iter.match (buffer->info[j - 1]);
      ଯଦି (match == skippy_iter.MATCH)
      {
        // https://github.com/harfbuzz/harfbuzz/issues/4124
	ଯଦି (!accept (buffer, j - 1) &&
	    NOT_COVERED == (ଏହି+ligatureCoverage).get_coverage  (buffer->info[j - 1].codepoint))
	  match = skippy_iter.SKIP;
      }
      ଯଦି (match == skippy_iter.MATCH)
      {
	c->last_base = (signed) j - 1;
	ଭାଙ୍ଗ;
      }
    }
    c->last_base_until = buffer->idx;
    ଯଦି (c->last_base == -1)
    {
      buffer->unsafe_to_concat_from_outbuffer (0, buffer->idx + 1);
      return_trace (ମିଥ୍ୟା);
    }

    unsigned idx = (unsigned) c->last_base;

    /* Checking that matched glyph is actually a ligature by GDEF is too strong; disabled */
    //if (!_hb_glyph_info_is_ligature (&buffer->info[idx])) { return_trace (false); }

    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା lig_index = (ଏହି+ligatureCoverage).get_coverage  (buffer->info[idx].codepoint);
    ଯଦି (lig_index == NOT_COVERED)
    {
      buffer->unsafe_to_concat_from_outbuffer (idx, buffer->idx + 1);
      return_trace (ମିଥ୍ୟା);
    }

    ଅଚଳ LigatureArray<Types>& lig_array = ଏହି+ligatureArray;
    ଅଚଳ AnchorMatrix<Types, HBUINT16>& lig_attach = lig_array[lig_index];

    /* Find component to attach to */
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା comp_ଗଣନା = lig_attach.rows;
    ଯଦି (unlikely (!comp_ଗଣନା))
    {
      buffer->unsafe_to_concat_from_outbuffer (idx, buffer->idx + 1);
      return_trace (ମିଥ୍ୟା);
    }

    /* We must now check whether the ligature ID of the current mark glyph
     * is identical to the ligature ID of the found ligature.  If yes, we
     * can directly use the component index.  If not, we attach the mark
     * glyph to the last component of the ligature. */
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା comp_index;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା lig_id = _hb_glyph_info_get_lig_id (&buffer->info[idx]);
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା mark_id = _hb_glyph_info_get_lig_id (&buffer->cur());
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା mark_comp = _hb_glyph_info_get_lig_comp (&buffer->cur());
    ଯଦି (lig_id && lig_id == mark_id && mark_comp > 0)
      comp_index = hb_min (comp_ଗଣନା, _hb_glyph_info_get_lig_comp (&buffer->cur())) - 1;
    ଅନ୍ୟଥା
      comp_index = comp_ଗଣନା - 1;

    return_trace ((ଏହି+markArray).apply (c, mark_index, comp_index, lig_attach, class_ଗଣନା_2, idx));
  }

  ତାର୍କିକ subset (hb_subset_context_t *c) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    ଅଚଳ hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ଅଚଳ hb_map_t &glyph_map = c->plan->glyph_map_gsub;

    auto *out = c->serializer->start_embed (*ଏହି);
    ଯଦି (unlikely (!c->serializer->extend_min (out))) return_trace (ମିଥ୍ୟା);
    out->format = format;

    hb_map_t klass_mapping;
    Markclass_closure_and_remap_indexes (ଏହି+markCoverage, ଏହି+markArray, glyphset, &klass_mapping);

    ଯଦି (!klass_mapping.get_population ()) return_trace (ମିଥ୍ୟା);
    out->class_ଗଣନା_2 = klass_mapping.get_population ();

    auto mark_iter =
    + hb_zip (ଏହି+markCoverage, hb_range ((unsigned) (ଏହି+markArray).len))
    | hb_filter (glyphset, hb_first)
    ;

    auto new_mark_coverage =
    + mark_iter
    | hb_map_retains_sorting (hb_first)
    | hb_map_retains_sorting (glyph_map)
    ;

    ଯଦି (!out->markCoverage.serialize_serialize (c->serializer, new_mark_coverage))
      return_trace (ମିଥ୍ୟା);

    ଯଦି (unlikely (!out->markArray.serialize_subset (c, markArray, ଏହି,
						    (ଏହି+markCoverage).iter (),
						    &klass_mapping)))
      return_trace (ମିଥ୍ୟା);

    hb_sorted_vector_t<hb_codepoint_t> new_lig_coverage;
    ଯଦି (!out->ligatureArray.serialize_subset (c, ligatureArray, ଏହି,
					      hb_iter (ଏହି+ligatureCoverage),
					      class_ଗଣନା_2, &klass_mapping, new_lig_coverage))
      return_trace (ମିଥ୍ୟା);

    return_trace (out->ligatureCoverage.serialize_serialize (c->serializer, new_lig_coverage.iter ()));
  }

};

}
}
}

#endif /* OT_LAYOUT_GPOS_MARKLIGPOSFORMAT1_HH */
