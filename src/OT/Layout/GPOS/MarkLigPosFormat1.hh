#ifndef OT_LAYOUT_GPOS_MARKLIGPOSFORMAT1_HH
#define OT_LAYOUT_GPOS_MARKLIGPOSFORMAT1_HH

#ضمن "LigatureArray.hh"

نطاق OT {
نطاق Layout {
نطاق GPOS_impl {


template <typename Types>
بنية MarkLigPosFormat1_2
{
  محمي:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  typename Types::template LOffsetTo<Coverage>
                markCoverage;           /* Offset to Mark Coverage table--from
                                         * beginning of MarkLigPos subtable */
  typename Types::template LOffsetTo<Coverage>
                ligatureCoverage;       /* Offset to Ligature Coverage
                                         * table--from beginning of MarkLigPos
                                         * subtable */
  HBUINT16      classCount;             /* Number of defined mark classes */
  typename Types::template LOffsetTo<MarkArray<Types>>
                markArray;              /* Offset to MarkArray table--from
                                         * beginning of MarkLigPos subtable */
  typename Types::template LOffsetTo<LigatureArray<Types>>
                ligatureArray;          /* Offset to LigatureArray table--from
                                         * beginning of MarkLigPos subtable */
  عام:
  DEFINE_SIZE_STATIC (4 + 4 * Types::LOffset::static_size);

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
                  markCoverage.sanitize (c, هذا) &&
                  ligatureCoverage.sanitize (c, هذا) &&
                  markArray.sanitize (c, هذا) &&
                  ligatureArray.sanitize (c, هذا, (unsigned صحيح) classCount));
  }

  منطقي intersects (ثابت hb_set_t *glyphs) ثابت
  {
    أرجع (هذا+markCoverage).intersects (glyphs) &&
           (هذا+ligatureCoverage).intersects (glyphs);
  }

  فراغ closure_lookups (hb_closure_lookups_context_t *c) ثابت {}

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    (هذا+markArray).collect_variation_indices (c, هذا+markCoverage);

    hb_map_t klass_mapping;
    Markclass_closure_and_remap_indexes (هذا+markCoverage, هذا+markArray, *c->glyph_set, &klass_mapping);

    (هذا+ligatureArray).collect_variation_indices (c, هذا+ligatureCoverage,
                                                   classCount, klass_mapping);
  }

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    𐡄𐡍 (unlikely (!(هذا+markCoverage).collect_coverage (c->input))) أرجع;
    𐡄𐡍 (unlikely (!(هذا+ligatureCoverage).collect_coverage (c->input))) أرجع;
  }

  فراغ collect_second_glyphs (hb_set_digest_t *digest) ثابت
  { (هذا+ligatureCoverage).collect_coverage (digest); }

  ثابت Coverage &get_coverage () ثابت { أرجع هذا+markCoverage; }

  ساكن مضمن منطقي accept (hb_buffer_t *buffer, unsigned idx)
  {
    /* We only want to attach to the first of a MultipleSubst sequence,
     * which might have been ligated into a preceding ligature, and in that
     * case the mark should attach to that ligature.
     * https://github.com/harfbuzz/harfbuzz/issues/4969
     * Reject others... */
    أرجع !_hb_glyph_info_multiplied (&buffer->info[idx]) ||
	   0 == _hb_glyph_info_get_lig_comp (&buffer->info[idx]);
  }

  منطقي apply (hb_ot_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);
    hb_buffer_t *buffer = c->buffer;
    unsigned صحيح mark_index = (هذا+markCoverage).get_coverage  (buffer->cur().codepoint);
    𐡄𐡍 (likely (mark_index == NOT_COVERED)) return_trace (خطأ);

    /* Now we search backwards for a non-mark glyph */

    auto &skippy_iter = c->iter_input;
    skippy_iter.set_lookup_props (LookupFlag::IgnoreMarks);

    𐡄𐡍 (c->last_base_until > buffer->idx)
    {
      c->last_base_until = 0;
      c->last_base = -1;
    }
    unsigned j;
    لكل (j = buffer->idx; j > c->last_base_until; j--)
    {
      auto match = skippy_iter.match (buffer->info[j - 1]);
      𐡄𐡍 (match == skippy_iter.MATCH)
      {
        // https://github.com/harfbuzz/harfbuzz/issues/4124
	𐡄𐡍 (!accept (buffer, j - 1) &&
	    NOT_COVERED == (هذا+ligatureCoverage).get_coverage  (buffer->info[j - 1].codepoint))
	  match = skippy_iter.SKIP;
      }
      𐡄𐡍 (match == skippy_iter.MATCH)
      {
	c->last_base = (signed) j - 1;
	اكسر;
      }
    }
    c->last_base_until = buffer->idx;
    𐡄𐡍 (c->last_base == -1)
    {
      buffer->unsafe_to_concat_from_outbuffer (0, buffer->idx + 1);
      return_trace (خطأ);
    }

    unsigned idx = (unsigned) c->last_base;

    /* Checking that matched glyph is actually a ligature by GDEF is too strong; disabled */
    //if (!_hb_glyph_info_is_ligature (&buffer->info[idx])) { return_trace (false); }

    unsigned صحيح lig_index = (هذا+ligatureCoverage).get_coverage  (buffer->info[idx].codepoint);
    𐡄𐡍 (lig_index == NOT_COVERED)
    {
      buffer->unsafe_to_concat_from_outbuffer (idx, buffer->idx + 1);
      return_trace (خطأ);
    }

    ثابت LigatureArray<Types>& lig_array = هذا+ligatureArray;
    ثابت AnchorMatrix<Types, HBUINT16>& lig_attach = lig_array[lig_index];

    /* Find component to attach to */
    unsigned صحيح comp_count = lig_attach.rows;
    𐡄𐡍 (unlikely (!comp_count))
    {
      buffer->unsafe_to_concat_from_outbuffer (idx, buffer->idx + 1);
      return_trace (خطأ);
    }

    /* We must now check whether the ligature ID of the current mark glyph
     * is identical to the ligature ID of the found ligature.  If yes, we
     * can directly use the component index.  If not, we attach the mark
     * glyph to the last component of the ligature. */
    unsigned صحيح comp_index;
    unsigned صحيح lig_id = _hb_glyph_info_get_lig_id (&buffer->info[idx]);
    unsigned صحيح mark_id = _hb_glyph_info_get_lig_id (&buffer->cur());
    unsigned صحيح mark_comp = _hb_glyph_info_get_lig_comp (&buffer->cur());
    𐡄𐡍 (lig_id && lig_id == mark_id && mark_comp > 0)
      comp_index = hb_min (comp_count, _hb_glyph_info_get_lig_comp (&buffer->cur())) - 1;
    وإلا
      comp_index = comp_count - 1;

    return_trace ((هذا+markArray).apply (c, mark_index, comp_index, lig_attach, classCount, idx));
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = c->plan->glyph_map_gsub;

    auto *out = c->serializer->start_embed (*هذا);
    𐡄𐡍 (unlikely (!c->serializer->extend_min (out))) return_trace (خطأ);
    out->format = format;

    hb_map_t klass_mapping;
    Markclass_closure_and_remap_indexes (هذا+markCoverage, هذا+markArray, glyphset, &klass_mapping);

    𐡄𐡍 (!klass_mapping.get_population ()) return_trace (خطأ);
    out->classCount = klass_mapping.get_population ();

    auto mark_iter =
    + hb_zip (هذا+markCoverage, hb_range ((unsigned) (هذا+markArray).len))
    | hb_filter (glyphset, hb_first)
    ;

    auto new_mark_coverage =
    + mark_iter
    | hb_map_retains_sorting (hb_first)
    | hb_map_retains_sorting (glyph_map)
    ;

    𐡄𐡍 (!out->markCoverage.serialize_serialize (c->serializer, new_mark_coverage))
      return_trace (خطأ);

    𐡄𐡍 (unlikely (!out->markArray.serialize_subset (c, markArray, هذا,
						    (هذا+markCoverage).iter (),
						    &klass_mapping)))
      return_trace (خطأ);

    hb_sorted_vector_t<hb_codepoint_t> new_lig_coverage;
    𐡄𐡍 (!out->ligatureArray.serialize_subset (c, ligatureArray, هذا,
					      hb_iter (هذا+ligatureCoverage),
					      classCount, &klass_mapping, new_lig_coverage))
      return_trace (خطأ);

    return_trace (out->ligatureCoverage.serialize_serialize (c->serializer, new_lig_coverage.iter ()));
  }

};

}
}
}

#endif /* OT_LAYOUT_GPOS_MARKLIGPOSFORMAT1_HH */
