#ifndef OT_LAYOUT_GPOS_MARKLIGPOSFORMAT1_HH
#define OT_LAYOUT_GPOS_MARKLIGPOSFORMAT1_HH

#अन्तर्भाव "LigatureArray.hh"

नामाकाश OT {
नामाकाश Layout {
नामाकाश GPOS_impl {


template <typename Types>
संरचना MarkLigPosFormat1_2
{
  रक्षित:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  typename Types::template LOffsetTo<Coverage>
                markCoverage;           /* Offset to Mark Coverage table--from
                                         * beginning of MarkLigPos subtable */
  typename Types::template LOffsetTo<Coverage>
                ligatureCoverage;       /* Offset to Ligature Coverage
                                         * table--from beginning of MarkLigPos
                                         * subtable */
  HBUINT16      class_गिनती_2;             /* Number of defined mark classes */
  typename Types::template LOffsetTo<MarkArray<Types>>
                markArray;              /* Offset to MarkArray table--from
                                         * beginning of MarkLigPos subtable */
  typename Types::template LOffsetTo<LigatureArray<Types>>
                ligatureArray;          /* Offset to LigatureArray table--from
                                         * beginning of MarkLigPos subtable */
  सार्वजनिक:
  DEFINE_SIZE_STATIC (4 + 4 * Types::LOffset::static_size);

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) &&
                  markCoverage.sanitize (c, एतत्) &&
                  ligatureCoverage.sanitize (c, एतत्) &&
                  markArray.sanitize (c, एतत्) &&
                  ligatureArray.sanitize (c, एतत्, (unsigned पूर्णाङ्क) class_गिनती_2));
  }

  तार्किक intersects (ध्रुव hb_set_t *glyphs) ध्रुव
  {
    प्रत्यागम (एतत्+markCoverage).intersects (glyphs) &&
           (एतत्+ligatureCoverage).intersects (glyphs);
  }

  शून्य closure_lookups (hb_closure_lookups_context_t *c) ध्रुव {}

  शून्य collect_variation_indices (hb_collect_variation_indices_context_t *c) ध्रुव
  {
    (एतत्+markArray).collect_variation_indices (c, एतत्+markCoverage);

    hb_map_t klass_mapping;
    Markclass_closure_and_remap_indexes (एतत्+markCoverage, एतत्+markArray, *c->glyph_set, &klass_mapping);

    (एतत्+ligatureArray).collect_variation_indices (c, एतत्+ligatureCoverage,
                                                   class_गिनती_2, klass_mapping);
  }

  शून्य collect_glyphs (hb_collect_glyphs_context_t *c) ध्रुव
  {
    यदि (unlikely (!(एतत्+markCoverage).collect_coverage (c->निवेश))) प्रत्यागम;
    यदि (unlikely (!(एतत्+ligatureCoverage).collect_coverage (c->निवेश))) प्रत्यागम;
  }

  शून्य collect_second_glyphs (hb_set_digest_t *digest) ध्रुव
  { (एतत्+ligatureCoverage).collect_coverage (digest); }

  ध्रुव Coverage &get_coverage () ध्रुव { प्रत्यागम एतत्+markCoverage; }

  स्थिर अन्तःस्थ तार्किक accept (hb_buffer_t *buffer, unsigned idx)
  {
    /* We only want to attach to the first of a MultipleSubst sequence,
     * which might have been ligated into a preceding ligature, and in that
     * case the mark should attach to that ligature.
     * https://github.com/harfbuzz/harfbuzz/issues/4969
     * Reject others... */
    प्रत्यागम !_hb_glyph_info_multiplied (&buffer->info[idx]) ||
	   0 == _hb_glyph_info_get_lig_comp (&buffer->info[idx]);
  }

  तार्किक apply (hb_ot_apply_context_t *c) ध्रुव
  {
    TRACE_APPLY (एतत्);
    hb_buffer_t *buffer = c->buffer;
    unsigned पूर्णाङ्क mark_index = (एतत्+markCoverage).get_coverage  (buffer->cur().codepoint);
    यदि (likely (mark_index == NOT_COVERED)) return_trace (असत्य);

    /* Now we search backwards for a non-mark glyph */

    auto &skippy_iter = c->iter_निवेश;
    skippy_iter.set_lookup_props (LookupFlag::IgnoreMarks);

    यदि (c->last_base_until > buffer->idx)
    {
      c->last_base_until = 0;
      c->last_base = -1;
    }
    unsigned j;
    प्रति (j = buffer->idx; j > c->last_base_until; j--)
    {
      auto match = skippy_iter.match (buffer->info[j - 1]);
      यदि (match == skippy_iter.MATCH)
      {
        // https://github.com/harfbuzz/harfbuzz/issues/4124
	यदि (!accept (buffer, j - 1) &&
	    NOT_COVERED == (एतत्+ligatureCoverage).get_coverage  (buffer->info[j - 1].codepoint))
	  match = skippy_iter.SKIP;
      }
      यदि (match == skippy_iter.MATCH)
      {
	c->last_base = (signed) j - 1;
	भङ्ग;
      }
    }
    c->last_base_until = buffer->idx;
    यदि (c->last_base == -1)
    {
      buffer->unsafe_to_concat_from_outbuffer (0, buffer->idx + 1);
      return_trace (असत्य);
    }

    unsigned idx = (unsigned) c->last_base;

    /* Checking that matched glyph is actually a ligature by GDEF is too strong; disabled */
    //if (!_hb_glyph_info_is_ligature (&buffer->info[idx])) { return_trace (false); }

    unsigned पूर्णाङ्क lig_index = (एतत्+ligatureCoverage).get_coverage  (buffer->info[idx].codepoint);
    यदि (lig_index == NOT_COVERED)
    {
      buffer->unsafe_to_concat_from_outbuffer (idx, buffer->idx + 1);
      return_trace (असत्य);
    }

    ध्रुव LigatureArray<Types>& lig_array = एतत्+ligatureArray;
    ध्रुव AnchorMatrix<Types, HBUINT16>& lig_attach = lig_array[lig_index];

    /* Find component to attach to */
    unsigned पूर्णाङ्क comp_गिनती = lig_attach.rows;
    यदि (unlikely (!comp_गिनती))
    {
      buffer->unsafe_to_concat_from_outbuffer (idx, buffer->idx + 1);
      return_trace (असत्य);
    }

    /* We must now check whether the ligature ID of the current mark glyph
     * is identical to the ligature ID of the found ligature.  If yes, we
     * can directly use the component index.  If not, we attach the mark
     * glyph to the last component of the ligature. */
    unsigned पूर्णाङ्क comp_index;
    unsigned पूर्णाङ्क lig_id = _hb_glyph_info_get_lig_id (&buffer->info[idx]);
    unsigned पूर्णाङ्क mark_id = _hb_glyph_info_get_lig_id (&buffer->cur());
    unsigned पूर्णाङ्क mark_comp = _hb_glyph_info_get_lig_comp (&buffer->cur());
    यदि (lig_id && lig_id == mark_id && mark_comp > 0)
      comp_index = hb_min (comp_गिनती, _hb_glyph_info_get_lig_comp (&buffer->cur())) - 1;
    अन्यथा
      comp_index = comp_गिनती - 1;

    return_trace ((एतत्+markArray).apply (c, mark_index, comp_index, lig_attach, class_गिनती_2, idx));
  }

  तार्किक subset (hb_subset_context_t *c) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    ध्रुव hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ध्रुव hb_map_t &glyph_map = c->plan->glyph_map_gsub;

    auto *out = c->serializer->start_embed (*एतत्);
    यदि (unlikely (!c->serializer->extend_min (out))) return_trace (असत्य);
    out->format = format;

    hb_map_t klass_mapping;
    Markclass_closure_and_remap_indexes (एतत्+markCoverage, एतत्+markArray, glyphset, &klass_mapping);

    यदि (!klass_mapping.get_population ()) return_trace (असत्य);
    out->class_गिनती_2 = klass_mapping.get_population ();

    auto mark_iter =
    + hb_zip (एतत्+markCoverage, hb_range ((unsigned) (एतत्+markArray).len))
    | hb_filter (glyphset, hb_first)
    ;

    auto new_mark_coverage =
    + mark_iter
    | hb_map_retains_sorting (hb_first)
    | hb_map_retains_sorting (glyph_map)
    ;

    यदि (!out->markCoverage.serialize_serialize (c->serializer, new_mark_coverage))
      return_trace (असत्य);

    यदि (unlikely (!out->markArray.serialize_subset (c, markArray, एतत्,
						    (एतत्+markCoverage).iter (),
						    &klass_mapping)))
      return_trace (असत्य);

    hb_sorted_vector_t<hb_codepoint_t> new_lig_coverage;
    यदि (!out->ligatureArray.serialize_subset (c, ligatureArray, एतत्,
					      hb_iter (एतत्+ligatureCoverage),
					      class_गिनती_2, &klass_mapping, new_lig_coverage))
      return_trace (असत्य);

    return_trace (out->ligatureCoverage.serialize_serialize (c->serializer, new_lig_coverage.iter ()));
  }

};

}
}
}

#endif /* OT_LAYOUT_GPOS_MARKLIGPOSFORMAT1_HH */
