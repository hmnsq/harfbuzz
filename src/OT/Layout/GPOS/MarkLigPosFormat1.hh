#ifndef OT_LAYOUT_GPOS_MARKLIGPOSFORMAT1_HH
#define OT_LAYOUT_GPOS_MARKLIGPOSFORMAT1_HH

#અન્તર્ભાવ "LigatureArray.hh"

નામાકાશ OT {
નામાકાશ Layout {
નામાકાશ GPOS_impl {


template <typename Types>
સંરચના MarkLigPosFormat1_2
{
  સુરક્ષિત:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  typename Types::template LOffsetTo<Coverage>
                markCoverage;           /* Offset to Mark Coverage table--from
                                         * beginning of MarkLigPos subtable */
  typename Types::template LOffsetTo<Coverage>
                ligatureCoverage;       /* Offset to Ligature Coverage
                                         * table--from beginning of MarkLigPos
                                         * subtable */
  HBUINT16      class_ગણતરી_2;             /* Number of defined mark classes */
  typename Types::template LOffsetTo<MarkArray<Types>>
                markArray;              /* Offset to MarkArray table--from
                                         * beginning of MarkLigPos subtable */
  typename Types::template LOffsetTo<LigatureArray<Types>>
                ligatureArray;          /* Offset to LigatureArray table--from
                                         * beginning of MarkLigPos subtable */
  જાહેર:
  DEFINE_SIZE_STATIC (4 + 4 * Types::LOffset::static_size);

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ) &&
                  markCoverage.sanitize (c, આ) &&
                  ligatureCoverage.sanitize (c, આ) &&
                  markArray.sanitize (c, આ) &&
                  ligatureArray.sanitize (c, આ, (unsigned પૂર્ણાંક) class_ગણતરી_2));
  }

  તાર્કિક intersects (અચલ hb_set_t *glyphs) અચલ
  {
    પાછા (આ+markCoverage).intersects (glyphs) &&
           (આ+ligatureCoverage).intersects (glyphs);
  }

  શૂન્ય closure_lookups (hb_closure_lookups_context_t *c) અચલ {}

  શૂન્ય collect_variation_indices (hb_collect_variation_indices_context_t *c) અચલ
  {
    (આ+markArray).collect_variation_indices (c, આ+markCoverage);

    hb_map_t klass_mapping;
    Markclass_closure_and_remap_indexes (આ+markCoverage, આ+markArray, *c->glyph_set, &klass_mapping);

    (આ+ligatureArray).collect_variation_indices (c, આ+ligatureCoverage,
                                                   class_ગણતરી_2, klass_mapping);
  }

  શૂન્ય collect_glyphs (hb_collect_glyphs_context_t *c) અચલ
  {
    જો (unlikely (!(આ+markCoverage).collect_coverage (c->આગત))) પાછા;
    જો (unlikely (!(આ+ligatureCoverage).collect_coverage (c->આગત))) પાછા;
  }

  શૂન્ય collect_second_glyphs (hb_set_digest_t *digest) અચલ
  { (આ+ligatureCoverage).collect_coverage (digest); }

  અચલ Coverage &get_coverage () અચલ { પાછા આ+markCoverage; }

  સ્થિર અંતઃરેખ તાર્કિક accept (hb_buffer_t *buffer, unsigned idx)
  {
    /* We only want to attach to the first of a MultipleSubst sequence,
     * which might have been ligated into a preceding ligature, and in that
     * case the mark should attach to that ligature.
     * https://github.com/harfbuzz/harfbuzz/issues/4969
     * Reject others... */
    પાછા !_hb_glyph_info_multiplied (&buffer->info[idx]) ||
	   0 == _hb_glyph_info_get_lig_comp (&buffer->info[idx]);
  }

  તાર્કિક apply (hb_ot_apply_context_t *c) અચલ
  {
    TRACE_APPLY (આ);
    hb_buffer_t *buffer = c->buffer;
    unsigned પૂર્ણાંક mark_index = (આ+markCoverage).get_coverage  (buffer->cur().codepoint);
    જો (likely (mark_index == NOT_COVERED)) return_trace (ખોટું);

    /* Now we search backwards for a non-mark glyph */

    auto &skippy_iter = c->iter_આગત;
    skippy_iter.set_lookup_props (LookupFlag::IgnoreMarks);

    જો (c->last_base_until > buffer->idx)
    {
      c->last_base_until = 0;
      c->last_base = -1;
    }
    unsigned j;
    માટે (j = buffer->idx; j > c->last_base_until; j--)
    {
      auto match = skippy_iter.match (buffer->info[j - 1]);
      જો (match == skippy_iter.MATCH)
      {
        // https://github.com/harfbuzz/harfbuzz/issues/4124
	જો (!accept (buffer, j - 1) &&
	    NOT_COVERED == (આ+ligatureCoverage).get_coverage  (buffer->info[j - 1].codepoint))
	  match = skippy_iter.SKIP;
      }
      જો (match == skippy_iter.MATCH)
      {
	c->last_base = (signed) j - 1;
	તોડો;
      }
    }
    c->last_base_until = buffer->idx;
    જો (c->last_base == -1)
    {
      buffer->unsafe_to_concat_from_outbuffer (0, buffer->idx + 1);
      return_trace (ખોટું);
    }

    unsigned idx = (unsigned) c->last_base;

    /* Checking that matched glyph is actually a ligature by GDEF is too strong; disabled */
    //if (!_hb_glyph_info_is_ligature (&buffer->info[idx])) { return_trace (false); }

    unsigned પૂર્ણાંક lig_index = (આ+ligatureCoverage).get_coverage  (buffer->info[idx].codepoint);
    જો (lig_index == NOT_COVERED)
    {
      buffer->unsafe_to_concat_from_outbuffer (idx, buffer->idx + 1);
      return_trace (ખોટું);
    }

    અચલ LigatureArray<Types>& lig_array = આ+ligatureArray;
    અચલ AnchorMatrix<Types, HBUINT16>& lig_attach = lig_array[lig_index];

    /* Find component to attach to */
    unsigned પૂર્ણાંક comp_ગણતરી = lig_attach.rows;
    જો (unlikely (!comp_ગણતરી))
    {
      buffer->unsafe_to_concat_from_outbuffer (idx, buffer->idx + 1);
      return_trace (ખોટું);
    }

    /* We must now check whether the ligature ID of the current mark glyph
     * is identical to the ligature ID of the found ligature.  If yes, we
     * can directly use the component index.  If not, we attach the mark
     * glyph to the last component of the ligature. */
    unsigned પૂર્ણાંક comp_index;
    unsigned પૂર્ણાંક lig_id = _hb_glyph_info_get_lig_id (&buffer->info[idx]);
    unsigned પૂર્ણાંક mark_id = _hb_glyph_info_get_lig_id (&buffer->cur());
    unsigned પૂર્ણાંક mark_comp = _hb_glyph_info_get_lig_comp (&buffer->cur());
    જો (lig_id && lig_id == mark_id && mark_comp > 0)
      comp_index = hb_min (comp_ગણતરી, _hb_glyph_info_get_lig_comp (&buffer->cur())) - 1;
    નહીંતર
      comp_index = comp_ગણતરી - 1;

    return_trace ((આ+markArray).apply (c, mark_index, comp_index, lig_attach, class_ગણતરી_2, idx));
  }

  તાર્કિક subset (hb_subset_context_t *c) અચલ
  {
    TRACE_SUBSET (આ);
    અચલ hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    અચલ hb_map_t &glyph_map = c->plan->glyph_map_gsub;

    auto *out = c->serializer->start_embed (*આ);
    જો (unlikely (!c->serializer->extend_min (out))) return_trace (ખોટું);
    out->format = format;

    hb_map_t klass_mapping;
    Markclass_closure_and_remap_indexes (આ+markCoverage, આ+markArray, glyphset, &klass_mapping);

    જો (!klass_mapping.get_population ()) return_trace (ખોટું);
    out->class_ગણતરી_2 = klass_mapping.get_population ();

    auto mark_iter =
    + hb_zip (આ+markCoverage, hb_range ((unsigned) (આ+markArray).len))
    | hb_filter (glyphset, hb_first)
    ;

    auto new_mark_coverage =
    + mark_iter
    | hb_map_retains_sorting (hb_first)
    | hb_map_retains_sorting (glyph_map)
    ;

    જો (!out->markCoverage.serialize_serialize (c->serializer, new_mark_coverage))
      return_trace (ખોટું);

    જો (unlikely (!out->markArray.serialize_subset (c, markArray, આ,
						    (આ+markCoverage).iter (),
						    &klass_mapping)))
      return_trace (ખોટું);

    hb_sorted_vector_t<hb_codepoint_t> new_lig_coverage;
    જો (!out->ligatureArray.serialize_subset (c, ligatureArray, આ,
					      hb_iter (આ+ligatureCoverage),
					      class_ગણતરી_2, &klass_mapping, new_lig_coverage))
      return_trace (ખોટું);

    return_trace (out->ligatureCoverage.serialize_serialize (c->serializer, new_lig_coverage.iter ()));
  }

};

}
}
}

#endif /* OT_LAYOUT_GPOS_MARKLIGPOSFORMAT1_HH */
