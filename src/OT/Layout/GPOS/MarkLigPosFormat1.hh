#ifndef OT_LAYOUT_GPOS_MARKLIGPOSFORMAT1_HH
#define OT_LAYOUT_GPOS_MARKLIGPOSFORMAT1_HH

#अन्तर्भाव "LigatureArray.hh"

नामाकाश OT {
नामाकाश Layout {
नामाकाश GPOS_impl {


ढाँचा <typename Types>
संरचना MarkLigPosFormat1_2
{
  संरक्षित:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  typename Types::ढाँचा LOffsetTo<Coverage>
                markCoverage;           /* Offset to Mark Coverage table--from
                                         * beginning of MarkLigPos subtable */
  typename Types::ढाँचा LOffsetTo<Coverage>
                ligatureCoverage;       /* Offset to Ligature Coverage
                                         * table--from beginning of MarkLigPos
                                         * subtable */
  HBUINT16      class_मोजणी_2;             /* Number of defined mark classes */
  typename Types::ढाँचा LOffsetTo<MarkArray<Types>>
                markArray;              /* Offset to MarkArray table--from
                                         * beginning of MarkLigPos subtable */
  typename Types::ढाँचा LOffsetTo<LigatureArray<Types>>
                ligatureArray;          /* Offset to LigatureArray table--from
                                         * beginning of MarkLigPos subtable */
  सार्वजनिक:
  DEFINE_SIZE_STATIC (4 + 4 * Types::LOffset::static_size);

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे) &&
                  markCoverage.sanitize (c, हे) &&
                  ligatureCoverage.sanitize (c, हे) &&
                  markArray.sanitize (c, हे) &&
                  ligatureArray.sanitize (c, हे, (अचिन्हित पूर्णांक) class_मोजणी_2));
  }

  तार्किक intersects (स्थिरांक hb_set_t *glyphs) स्थिरांक
  {
    परत (हे+markCoverage).intersects (glyphs) &&
           (हे+ligatureCoverage).intersects (glyphs);
  }

  शून्य closure_lookups (hb_closure_lookups_context_t *c) स्थिरांक {}

  शून्य collect_variation_indices (hb_collect_variation_indices_context_t *c) स्थिरांक
  {
    (हे+markArray).collect_variation_indices (c, हे+markCoverage);

    hb_map_t klass_mapping;
    Markclass_closure_and_remap_indexes (हे+markCoverage, हे+markArray, *c->glyph_set, &klass_mapping);

    (हे+ligatureArray).collect_variation_indices (c, हे+ligatureCoverage,
                                                   class_मोजणी_2, klass_mapping);
  }

  शून्य collect_glyphs (hb_collect_glyphs_context_t *c) स्थिरांक
  {
    जर (unlikely (!(हे+markCoverage).collect_coverage (c->आदान))) परत;
    जर (unlikely (!(हे+ligatureCoverage).collect_coverage (c->आदान))) परत;
  }

  शून्य collect_second_glyphs (hb_set_digest_t *digest) स्थिरांक
  { (हे+ligatureCoverage).collect_coverage (digest); }

  स्थिरांक Coverage &get_coverage () स्थिरांक { परत हे+markCoverage; }

  स्थिर ओळीत तार्किक accept (hb_buffer_t *buffer, अचिन्हित idx)
  {
    /* We only want to attach to the first of a MultipleSubst sequence,
     * which might have been ligated into a preceding ligature, and in that
     * case the mark should attach to that ligature.
     * https://github.com/harfbuzz/harfbuzz/issues/4969
     * Reject others... */
    परत !_hb_glyph_info_multiplied (&buffer->info[idx]) ||
	   0 == _hb_glyph_info_get_lig_comp (&buffer->info[idx]);
  }

  तार्किक apply (hb_ot_apply_context_t *c) स्थिरांक
  {
    TRACE_APPLY (हे);
    hb_buffer_t *buffer = c->buffer;
    अचिन्हित पूर्णांक mark_index = (हे+markCoverage).get_coverage  (buffer->cur().codepoint);
    जर (likely (mark_index == NOT_COVERED)) return_trace (असत्य);

    /* Now we search backwards for a non-mark glyph */

    स्वतः &skippy_iter = c->iter_आदान;
    skippy_iter.set_lookup_props (LookupFlag::IgnoreMarks);

    जर (c->last_base_until > buffer->idx)
    {
      c->last_base_until = 0;
      c->last_base = -1;
    }
    अचिन्हित j;
    साठी (j = buffer->idx; j > c->last_base_until; j--)
    {
      स्वतः match = skippy_iter.match (buffer->info[j - 1]);
      जर (match == skippy_iter.MATCH)
      {
        // https://github.com/harfbuzz/harfbuzz/issues/4124
	जर (!accept (buffer, j - 1) &&
	    NOT_COVERED == (हे+ligatureCoverage).get_coverage  (buffer->info[j - 1].codepoint))
	  match = skippy_iter.SKIP;
      }
      जर (match == skippy_iter.MATCH)
      {
	c->last_base = (चिन्हित) j - 1;
	थांबा;
      }
    }
    c->last_base_until = buffer->idx;
    जर (c->last_base == -1)
    {
      buffer->unsafe_to_concat_from_outbuffer (0, buffer->idx + 1);
      return_trace (असत्य);
    }

    अचिन्हित idx = (अचिन्हित) c->last_base;

    /* Checking that matched glyph is actually a ligature by GDEF is too strong; disabled */
    //if (!_hb_glyph_info_is_ligature (&buffer->info[idx])) { return_trace (false); }

    अचिन्हित पूर्णांक lig_index = (हे+ligatureCoverage).get_coverage  (buffer->info[idx].codepoint);
    जर (lig_index == NOT_COVERED)
    {
      buffer->unsafe_to_concat_from_outbuffer (idx, buffer->idx + 1);
      return_trace (असत्य);
    }

    स्थिरांक LigatureArray<Types>& lig_array = हे+ligatureArray;
    स्थिरांक AnchorMatrix<Types, HBUINT16>& lig_attach = lig_array[lig_index];

    /* Find component to attach to */
    अचिन्हित पूर्णांक comp_मोजणी = lig_attach.rows;
    जर (unlikely (!comp_मोजणी))
    {
      buffer->unsafe_to_concat_from_outbuffer (idx, buffer->idx + 1);
      return_trace (असत्य);
    }

    /* We must now check whether the ligature ID of the current mark glyph
     * is identical to the ligature ID of the found ligature.  If yes, we
     * can directly use the component index.  If not, we attach the mark
     * glyph to the last component of the ligature. */
    अचिन्हित पूर्णांक comp_index;
    अचिन्हित पूर्णांक lig_id = _hb_glyph_info_get_lig_id (&buffer->info[idx]);
    अचिन्हित पूर्णांक mark_id = _hb_glyph_info_get_lig_id (&buffer->cur());
    अचिन्हित पूर्णांक mark_comp = _hb_glyph_info_get_lig_comp (&buffer->cur());
    जर (lig_id && lig_id == mark_id && mark_comp > 0)
      comp_index = hb_min (comp_मोजणी, _hb_glyph_info_get_lig_comp (&buffer->cur())) - 1;
    अन्यथा
      comp_index = comp_मोजणी - 1;

    return_trace ((हे+markArray).apply (c, mark_index, comp_index, lig_attach, class_मोजणी_2, idx));
  }

  तार्किक subset (hb_subset_context_t *c) स्थिरांक
  {
    TRACE_SUBSET (हे);
    स्थिरांक hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    स्थिरांक hb_map_t &glyph_map = c->plan->glyph_map_gsub;

    स्वतः *out = c->serializer->start_embed (*हे);
    जर (unlikely (!c->serializer->extend_min (out))) return_trace (असत्य);
    out->format = format;

    hb_map_t klass_mapping;
    Markclass_closure_and_remap_indexes (हे+markCoverage, हे+markArray, glyphset, &klass_mapping);

    जर (!klass_mapping.get_population ()) return_trace (असत्य);
    out->class_मोजणी_2 = klass_mapping.get_population ();

    स्वतः mark_iter =
    + hb_zip (हे+markCoverage, hb_range ((अचिन्हित) (हे+markArray).len))
    | hb_filter (glyphset, hb_first)
    ;

    स्वतः new_mark_coverage =
    + mark_iter
    | hb_map_retains_sorting (hb_first)
    | hb_map_retains_sorting (glyph_map)
    ;

    जर (!out->markCoverage.serialize_serialize (c->serializer, new_mark_coverage))
      return_trace (असत्य);

    जर (unlikely (!out->markArray.serialize_subset (c, markArray, हे,
						    (हे+markCoverage).iter (),
						    &klass_mapping)))
      return_trace (असत्य);

    hb_sorted_vector_t<hb_codepoint_t> new_lig_coverage;
    जर (!out->ligatureArray.serialize_subset (c, ligatureArray, हे,
					      hb_iter (हे+ligatureCoverage),
					      class_मोजणी_2, &klass_mapping, new_lig_coverage))
      return_trace (असत्य);

    return_trace (out->ligatureCoverage.serialize_serialize (c->serializer, new_lig_coverage.iter ()));
  }

};

}
}
}

#endif /* OT_LAYOUT_GPOS_MARKLIGPOSFORMAT1_HH */
