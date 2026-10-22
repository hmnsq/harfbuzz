#ifndef OT_LAYOUT_GPOS_MARKLIGPOSFORMAT1_HH
#define OT_LAYOUT_GPOS_MARKLIGPOSFORMAT1_HH

#അന്തര്ഭാവ "LigatureArray.hh"

നാമാകാശ OT {
നാമാകാശ Layout {
നാമാകാശ GPOS_impl {


template <typename Types>
ഘടന MarkLigPosFormat1_2
{
  സംരക്ഷിത:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  typename Types::template LOffsetTo<Coverage>
                markCoverage;           /* Offset to Mark Coverage table--from
                                         * beginning of MarkLigPos subtable */
  typename Types::template LOffsetTo<Coverage>
                ligatureCoverage;       /* Offset to Ligature Coverage
                                         * table--from beginning of MarkLigPos
                                         * subtable */
  HBUINT16      class_എണ്ണം_2;             /* Number of defined mark classes */
  typename Types::template LOffsetTo<MarkArray<Types>>
                markArray;              /* Offset to MarkArray table--from
                                         * beginning of MarkLigPos subtable */
  typename Types::template LOffsetTo<LigatureArray<Types>>
                ligatureArray;          /* Offset to LigatureArray table--from
                                         * beginning of MarkLigPos subtable */
  പൊതു:
  DEFINE_SIZE_STATIC (4 + 4 * Types::LOffset::static_size);

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_struct (ഇത്) &&
                  markCoverage.sanitize (c, ഇത്) &&
                  ligatureCoverage.sanitize (c, ഇത്) &&
                  markArray.sanitize (c, ഇത്) &&
                  ligatureArray.sanitize (c, ഇത്, (unsigned പൂർണ്ണസംഖ്യ) class_എണ്ണം_2));
  }

  തർക്കം intersects (സ്ഥിരം hb_set_t *glyphs) സ്ഥിരം
  {
    തിരികെ (ഇത്+markCoverage).intersects (glyphs) &&
           (ഇത്+ligatureCoverage).intersects (glyphs);
  }

  ശൂന്യം closure_lookups (hb_closure_lookups_context_t *c) സ്ഥിരം {}

  ശൂന്യം collect_variation_indices (hb_collect_variation_indices_context_t *c) സ്ഥിരം
  {
    (ഇത്+markArray).collect_variation_indices (c, ഇത്+markCoverage);

    hb_map_t klass_mapping;
    Markclass_closure_and_remap_indexes (ഇത്+markCoverage, ഇത്+markArray, *c->glyph_set, &klass_mapping);

    (ഇത്+ligatureArray).collect_variation_indices (c, ഇത്+ligatureCoverage,
                                                   class_എണ്ണം_2, klass_mapping);
  }

  ശൂന്യം collect_glyphs (hb_collect_glyphs_context_t *c) സ്ഥിരം
  {
    എങ്കിൽ (unlikely (!(ഇത്+markCoverage).collect_coverage (c->നിവേശം))) തിരികെ;
    എങ്കിൽ (unlikely (!(ഇത്+ligatureCoverage).collect_coverage (c->നിവേശം))) തിരികെ;
  }

  ശൂന്യം collect_second_glyphs (hb_set_digest_t *digest) സ്ഥിരം
  { (ഇത്+ligatureCoverage).collect_coverage (digest); }

  സ്ഥിരം Coverage &get_coverage () സ്ഥിരം { തിരികെ ഇത്+markCoverage; }

  സ്ഥിതം വരിയിൽ തർക്കം accept (hb_buffer_t *buffer, unsigned idx)
  {
    /* We only want to attach to the first of a MultipleSubst sequence,
     * which might have been ligated into a preceding ligature, and in that
     * case the mark should attach to that ligature.
     * https://github.com/harfbuzz/harfbuzz/issues/4969
     * Reject others... */
    തിരികെ !_hb_glyph_info_multiplied (&buffer->info[idx]) ||
	   0 == _hb_glyph_info_get_lig_comp (&buffer->info[idx]);
  }

  തർക്കം apply (hb_ot_apply_context_t *c) സ്ഥിരം
  {
    TRACE_APPLY (ഇത്);
    hb_buffer_t *buffer = c->buffer;
    unsigned പൂർണ്ണസംഖ്യ mark_index = (ഇത്+markCoverage).get_coverage  (buffer->cur().codepoint);
    എങ്കിൽ (likely (mark_index == NOT_COVERED)) return_trace (അസത്യം);

    /* Now we search backwards for a non-mark glyph */

    auto &skippy_iter = c->iter_നിവേശം;
    skippy_iter.set_lookup_props (LookupFlag::IgnoreMarks);

    എങ്കിൽ (c->last_base_until > buffer->idx)
    {
      c->last_base_until = 0;
      c->last_base = -1;
    }
    unsigned j;
    വേണ്ടി (j = buffer->idx; j > c->last_base_until; j--)
    {
      auto match = skippy_iter.match (buffer->info[j - 1]);
      എങ്കിൽ (match == skippy_iter.MATCH)
      {
        // https://github.com/harfbuzz/harfbuzz/issues/4124
	എങ്കിൽ (!accept (buffer, j - 1) &&
	    NOT_COVERED == (ഇത്+ligatureCoverage).get_coverage  (buffer->info[j - 1].codepoint))
	  match = skippy_iter.SKIP;
      }
      എങ്കിൽ (match == skippy_iter.MATCH)
      {
	c->last_base = (signed) j - 1;
	നിർത്തുക;
      }
    }
    c->last_base_until = buffer->idx;
    എങ്കിൽ (c->last_base == -1)
    {
      buffer->unsafe_to_concat_from_outbuffer (0, buffer->idx + 1);
      return_trace (അസത്യം);
    }

    unsigned idx = (unsigned) c->last_base;

    /* Checking that matched glyph is actually a ligature by GDEF is too strong; disabled */
    //if (!_hb_glyph_info_is_ligature (&buffer->info[idx])) { return_trace (false); }

    unsigned പൂർണ്ണസംഖ്യ lig_index = (ഇത്+ligatureCoverage).get_coverage  (buffer->info[idx].codepoint);
    എങ്കിൽ (lig_index == NOT_COVERED)
    {
      buffer->unsafe_to_concat_from_outbuffer (idx, buffer->idx + 1);
      return_trace (അസത്യം);
    }

    സ്ഥിരം LigatureArray<Types>& lig_array = ഇത്+ligatureArray;
    സ്ഥിരം AnchorMatrix<Types, HBUINT16>& lig_attach = lig_array[lig_index];

    /* Find component to attach to */
    unsigned പൂർണ്ണസംഖ്യ comp_എണ്ണം = lig_attach.rows;
    എങ്കിൽ (unlikely (!comp_എണ്ണം))
    {
      buffer->unsafe_to_concat_from_outbuffer (idx, buffer->idx + 1);
      return_trace (അസത്യം);
    }

    /* We must now check whether the ligature ID of the current mark glyph
     * is identical to the ligature ID of the found ligature.  If yes, we
     * can directly use the component index.  If not, we attach the mark
     * glyph to the last component of the ligature. */
    unsigned പൂർണ്ണസംഖ്യ comp_index;
    unsigned പൂർണ്ണസംഖ്യ lig_id = _hb_glyph_info_get_lig_id (&buffer->info[idx]);
    unsigned പൂർണ്ണസംഖ്യ mark_id = _hb_glyph_info_get_lig_id (&buffer->cur());
    unsigned പൂർണ്ണസംഖ്യ mark_comp = _hb_glyph_info_get_lig_comp (&buffer->cur());
    എങ്കിൽ (lig_id && lig_id == mark_id && mark_comp > 0)
      comp_index = hb_min (comp_എണ്ണം, _hb_glyph_info_get_lig_comp (&buffer->cur())) - 1;
    അല്ലെങ്കിൽ
      comp_index = comp_എണ്ണം - 1;

    return_trace ((ഇത്+markArray).apply (c, mark_index, comp_index, lig_attach, class_എണ്ണം_2, idx));
  }

  തർക്കം subset (hb_subset_context_t *c) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);
    സ്ഥിരം hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    സ്ഥിരം hb_map_t &glyph_map = c->plan->glyph_map_gsub;

    auto *out = c->serializer->start_embed (*ഇത്);
    എങ്കിൽ (unlikely (!c->serializer->extend_min (out))) return_trace (അസത്യം);
    out->format = format;

    hb_map_t klass_mapping;
    Markclass_closure_and_remap_indexes (ഇത്+markCoverage, ഇത്+markArray, glyphset, &klass_mapping);

    എങ്കിൽ (!klass_mapping.get_population ()) return_trace (അസത്യം);
    out->class_എണ്ണം_2 = klass_mapping.get_population ();

    auto mark_iter =
    + hb_zip (ഇത്+markCoverage, hb_range ((unsigned) (ഇത്+markArray).len))
    | hb_filter (glyphset, hb_first)
    ;

    auto new_mark_coverage =
    + mark_iter
    | hb_map_retains_sorting (hb_first)
    | hb_map_retains_sorting (glyph_map)
    ;

    എങ്കിൽ (!out->markCoverage.serialize_serialize (c->serializer, new_mark_coverage))
      return_trace (അസത്യം);

    എങ്കിൽ (unlikely (!out->markArray.serialize_subset (c, markArray, ഇത്,
						    (ഇത്+markCoverage).iter (),
						    &klass_mapping)))
      return_trace (അസത്യം);

    hb_sorted_vector_t<hb_codepoint_t> new_lig_coverage;
    എങ്കിൽ (!out->ligatureArray.serialize_subset (c, ligatureArray, ഇത്,
					      hb_iter (ഇത്+ligatureCoverage),
					      class_എണ്ണം_2, &klass_mapping, new_lig_coverage))
      return_trace (അസത്യം);

    return_trace (out->ligatureCoverage.serialize_serialize (c->serializer, new_lig_coverage.iter ()));
  }

};

}
}
}

#endif /* OT_LAYOUT_GPOS_MARKLIGPOSFORMAT1_HH */
