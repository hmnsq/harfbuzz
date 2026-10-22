#अगर_अघोषित OT_LAYOUT_GPOS_MARKLIGPOSFORMAT1_HH
#घोषणा OT_LAYOUT_GPOS_MARKLIGPOSFORMAT1_HH

#समावेश "LigatureArray.hh"

namespace OT {
namespace Layout {
namespace GPOS_impl {


ढाँचा <typename Types>
काष्ठा MarkLigPosFormat1_2
{
  रक्षित:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  typename Types::ढाँचा LOffsetTo<Coverage>
                markCoverage;           /* Offset to Mark Coverage table--from
                                         * beginning of MarkLigPos subtable */
  typename Types::ढाँचा LOffsetTo<Coverage>
                ligatureCoverage;       /* Offset to Ligature Coverage
                                         * table--from beginning of MarkLigPos
                                         * subtable */
  HBUINT16      class_गिनती_2;             /* Number of defined mark classes */
  typename Types::ढाँचा LOffsetTo<MarkArray<Types>>
                markArray;              /* Offset to MarkArray table--from
                                         * beginning of MarkLigPos subtable */
  typename Types::ढाँचा LOffsetTo<LigatureArray<Types>>
                ligatureArray;          /* Offset to LigatureArray table--from
                                         * beginning of MarkLigPos subtable */
  खुला:
  DEFINE_आकार_STATIC (4 + 4 * Types::LOffset::static_आकार);

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह) &&
                  markCoverage.sanitize (c, यह) &&
                  ligatureCoverage.sanitize (c, यह) &&
                  markArray.sanitize (c, यह) &&
                  ligatureArray.sanitize (c, यह, (अचिन्हित पूर्णांक) class_गिनती_2));
  }

  bool intersects (स्थिर hb_set_t *glyphs) स्थिर
  {
    वापस (यह+markCoverage).intersects (glyphs) &&
           (यह+ligatureCoverage).intersects (glyphs);
  }

  व्योम closure_lookups (hb_closure_lookups_context_t *c) स्थिर {}

  व्योम collect_variation_indices (hb_collect_variation_indices_context_t *c) स्थिर
  {
    (यह+markArray).collect_variation_indices (c, यह+markCoverage);

    hb_map_t klass_mapping;
    Markclass_closure_and_remap_indexes (यह+markCoverage, यह+markArray, *c->glyph_set, &klass_mapping);

    (यह+ligatureArray).collect_variation_indices (c, यह+ligatureCoverage,
                                                   class_गिनती_2, klass_mapping);
  }

  व्योम collect_glyphs (hb_collect_glyphs_context_t *c) स्थिर
  {
    अगर (unlikely (!(यह+markCoverage).collect_coverage (c->निवेश))) वापस;
    अगर (unlikely (!(यह+ligatureCoverage).collect_coverage (c->निवेश))) वापस;
  }

  व्योम collect_second_glyphs (hb_set_digest_t *digest) स्थिर
  { (यह+ligatureCoverage).collect_coverage (digest); }

  स्थिर Coverage &get_coverage () स्थिर { वापस यह+markCoverage; }

  अटल अंतरभूत bool accept (hb_बफर_t *बफर, अचिन्हित idx)
  {
    /* We only want to attach to the first of a MultipleSubst sequence,
     * which might have been ligated into a preceding ligature, and in that
     * case the mark should attach to that ligature.
     * https://github.com/harfbuzz/harfbuzz/issues/4969
     * Reject others... */
    वापस !_hb_glyph_info_multiplied (&बफर->info[idx]) ||
	   0 == _hb_glyph_info_get_lig_comp (&बफर->info[idx]);
  }

  bool apply (hb_ot_apply_context_t *c) स्थिर
  {
    TRACE_APPLY (यह);
    hb_बफर_t *बफर = c->बफर;
    अचिन्हित पूर्णांक mark_सूचकांक = (यह+markCoverage).get_coverage  (बफर->cur().codepoint);
    अगर (likely (mark_सूचकांक == NOT_COVERED)) return_trace (false);

    /* Now we search backwards for a non-mark glyph */

    स्वतः &skippy_iter = c->iter_निवेश;
    skippy_iter.set_lookup_props (LookupFlag::IgnoreMarks);

    अगर (c->last_base_until > बफर->idx)
    {
      c->last_base_until = 0;
      c->last_base = -1;
    }
    अचिन्हित j;
    क्रम (j = बफर->idx; j > c->last_base_until; j--)
    {
      स्वतः match = skippy_iter.match (बफर->info[j - 1]);
      अगर (match == skippy_iter.MATCH)
      {
        // https://github.com/harfbuzz/harfbuzz/issues/4124
	अगर (!accept (बफर, j - 1) &&
	    NOT_COVERED == (यह+ligatureCoverage).get_coverage  (बफर->info[j - 1].codepoint))
	  match = skippy_iter.SKIP;
      }
      अगर (match == skippy_iter.MATCH)
      {
	c->last_base = (चिन्हित) j - 1;
	अवरोध;
      }
    }
    c->last_base_until = बफर->idx;
    अगर (c->last_base == -1)
    {
      बफर->unsafe_to_concat_from_outbuffer (0, बफर->idx + 1);
      return_trace (false);
    }

    अचिन्हित idx = (अचिन्हित) c->last_base;

    /* Checking that matched glyph is actually a ligature by GDEF is too strong; disabled */
    //if (!_hb_glyph_info_is_ligature (&buffer->info[idx])) { return_trace (false); }

    अचिन्हित पूर्णांक lig_सूचकांक = (यह+ligatureCoverage).get_coverage  (बफर->info[idx].codepoint);
    अगर (lig_सूचकांक == NOT_COVERED)
    {
      बफर->unsafe_to_concat_from_outbuffer (idx, बफर->idx + 1);
      return_trace (false);
    }

    स्थिर LigatureArray<Types>& lig_array = यह+ligatureArray;
    स्थिर AnchorMatrix<Types, HBUINT16>& lig_attach = lig_array[lig_सूचकांक];

    /* Find component to attach to */
    अचिन्हित पूर्णांक comp_गिनती = lig_attach.rows;
    अगर (unlikely (!comp_गिनती))
    {
      बफर->unsafe_to_concat_from_outbuffer (idx, बफर->idx + 1);
      return_trace (false);
    }

    /* We must now check whether the ligature ID of the current mark glyph
     * is identical to the ligature ID of the found ligature.  If yes, we
     * can directly use the component index.  If not, we attach the mark
     * glyph to the last component of the ligature. */
    अचिन्हित पूर्णांक comp_सूचकांक;
    अचिन्हित पूर्णांक lig_id = _hb_glyph_info_get_lig_id (&बफर->info[idx]);
    अचिन्हित पूर्णांक mark_id = _hb_glyph_info_get_lig_id (&बफर->cur());
    अचिन्हित पूर्णांक mark_comp = _hb_glyph_info_get_lig_comp (&बफर->cur());
    अगर (lig_id && lig_id == mark_id && mark_comp > 0)
      comp_सूचकांक = hb_min (comp_गिनती, _hb_glyph_info_get_lig_comp (&बफर->cur())) - 1;
    अन्यथा
      comp_सूचकांक = comp_गिनती - 1;

    return_trace ((यह+markArray).apply (c, mark_सूचकांक, comp_सूचकांक, lig_attach, class_गिनती_2, idx));
  }

  bool subset (hb_subset_context_t *c) स्थिर
  {
    TRACE_SUBSET (यह);
    स्थिर hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    स्थिर hb_map_t &glyph_map = c->plan->glyph_map_gsub;

    स्वतः *out = c->serializer->start_embed (*यह);
    अगर (unlikely (!c->serializer->extend_min (out))) return_trace (false);
    out->format = format;

    hb_map_t klass_mapping;
    Markclass_closure_and_remap_indexes (यह+markCoverage, यह+markArray, glyphset, &klass_mapping);

    अगर (!klass_mapping.get_population ()) return_trace (false);
    out->class_गिनती_2 = klass_mapping.get_population ();

    स्वतः mark_iter =
    + hb_zip (यह+markCoverage, hb_range ((अचिन्हित) (यह+markArray).len))
    | hb_filter (glyphset, hb_first)
    ;

    स्वतः new_mark_coverage =
    + mark_iter
    | hb_map_retains_sorting (hb_first)
    | hb_map_retains_sorting (glyph_map)
    ;

    अगर (!out->markCoverage.serialize_serialize (c->serializer, new_mark_coverage))
      return_trace (false);

    अगर (unlikely (!out->markArray.serialize_subset (c, markArray, यह,
						    (यह+markCoverage).iter (),
						    &klass_mapping)))
      return_trace (false);

    hb_sorted_vector_t<hb_codepoint_t> new_lig_coverage;
    अगर (!out->ligatureArray.serialize_subset (c, ligatureArray, यह,
					      hb_iter (यह+ligatureCoverage),
					      class_गिनती_2, &klass_mapping, new_lig_coverage))
      return_trace (false);

    return_trace (out->ligatureCoverage.serialize_serialize (c->serializer, new_lig_coverage.iter ()));
  }

};

}
}
}

#पूर्ण_अगर /* OT_LAYOUT_GPOS_MARKLIGPOSFORMAT1_HH */
