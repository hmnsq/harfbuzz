#अगर_अघोषित OT_LAYOUT_GPOS_MARKBASEPOSFORMAT1_HH
#घोषणा OT_LAYOUT_GPOS_MARKBASEPOSFORMAT1_HH

#समावेश "MarkArray.hh"

namespace OT {
namespace Layout {
namespace GPOS_impl {

ढाँचा <typename Types>
काष्ठा MarkBasePosFormat1_2
{
  रक्षित:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  typename Types::ढाँचा LOffsetTo<Coverage>
                markCoverage;           /* Offset to MarkCoverage table--from
                                         * beginning of MarkBasePos subtable */
  typename Types::ढाँचा LOffsetTo<Coverage>
                baseCoverage;           /* Offset to BaseCoverage table--from
                                         * beginning of MarkBasePos subtable */
  HBUINT16      class_गिनती_2;             /* Number of classes defined for marks */
  typename Types::ढाँचा LOffsetTo<MarkArray<Types>>
                markArray;              /* Offset to MarkArray table--from
                                         * beginning of MarkBasePos subtable */
  typename Types::ढाँचा LOffsetTo<AnchorMatrix<Types>>
                baseArray;              /* Offset to BaseArray table--from
                                         * beginning of MarkBasePos subtable */

  खुला:
  DEFINE_आकार_STATIC (4 + 4 * Types::LOffset::static_आकार);

    bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह) &&
                  markCoverage.sanitize (c, यह) &&
                  baseCoverage.sanitize (c, यह) &&
                  markArray.sanitize (c, यह) &&
                  baseArray.sanitize (c, यह, (अचिन्हित पूर्णांक) class_गिनती_2));
  }

  bool intersects (स्थिर hb_set_t *glyphs) स्थिर
  {
    वापस (यह+markCoverage).intersects (glyphs) &&
           (यह+baseCoverage).intersects (glyphs);
  }

  व्योम closure_lookups (hb_closure_lookups_context_t *c) स्थिर {}

  व्योम collect_variation_indices (hb_collect_variation_indices_context_t *c) स्थिर
  {
    (यह+markArray).collect_variation_indices (c, यह+markCoverage);

    hb_map_t klass_mapping;
    Markclass_closure_and_remap_indexes (यह+markCoverage, यह+markArray, *c->glyph_set, &klass_mapping);

    hb_sorted_vector_t<अचिन्हित> base_indexes;
    collect_anchor_indexes (यह+baseCoverage, (यह+baseArray).rows,
                            class_गिनती_2, *c->glyph_set, klass_mapping,
                            base_indexes);
    (यह+baseArray).collect_variation_indices (c, base_indexes.iter ());
  }

  व्योम collect_glyphs (hb_collect_glyphs_context_t *c) स्थिर
  {
    अगर (unlikely (!(यह+markCoverage).collect_coverage (c->निवेश))) वापस;
    अगर (unlikely (!(यह+baseCoverage).collect_coverage (c->निवेश))) वापस;
  }

  व्योम collect_second_glyphs (hb_set_digest_t *digest) स्थिर
  { (यह+baseCoverage).collect_coverage (digest); }

  स्थिर Coverage &get_coverage () स्थिर { वापस यह+markCoverage; }

  अटल अंतरभूत bool accept (hb_बफर_t *बफर, अचिन्हित idx)
  {
    /* We only want to attach to the first of a MultipleSubst sequence.
     * https://github.com/harfbuzz/harfbuzz/issues/740
     * Reject others...
     * ...but stop if we find a mark in the MultipleSubst sequence:
     * https://github.com/harfbuzz/harfbuzz/issues/1020 */
    वापस !_hb_glyph_info_multiplied (&बफर->info[idx]) ||
	   0 == _hb_glyph_info_get_lig_comp (&बफर->info[idx]) ||
	   (idx == 0 ||
	    _hb_glyph_info_is_mark (&बफर->info[idx - 1]) ||
	    !_hb_glyph_info_multiplied (&बफर->info[idx - 1]) ||
	    _hb_glyph_info_get_lig_id (&बफर->info[idx]) !=
	    _hb_glyph_info_get_lig_id (&बफर->info[idx - 1]) ||
	    _hb_glyph_info_get_lig_comp (&बफर->info[idx]) !=
	    _hb_glyph_info_get_lig_comp (&बफर->info[idx - 1]) + 1
	    );
  }

  bool apply (hb_ot_apply_context_t *c) स्थिर
  {
    TRACE_APPLY (यह);
    hb_बफर_t *बफर = c->बफर;
    अचिन्हित पूर्णांक mark_सूचकांक = (यह+markCoverage).get_coverage  (बफर->cur().codepoint);
    अगर (likely (mark_सूचकांक == NOT_COVERED)) return_trace (false);

    /* Now we search backwards for a non-mark glyph.
     * We don't use skippy_iter.prev() to avoid O(n^2) behavior. */

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
	    NOT_COVERED == (यह+baseCoverage).get_coverage  (बफर->info[j - 1].codepoint))
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

    /* Checking that matched glyph is actually a base glyph by GDEF is too strong; disabled */
    //if (!_hb_glyph_info_is_base_glyph (&buffer->info[idx])) { return_trace (false); }

    अचिन्हित पूर्णांक base_सूचकांक = (यह+baseCoverage).get_coverage  (बफर->info[idx].codepoint);
    अगर (base_सूचकांक == NOT_COVERED)
    {
      बफर->unsafe_to_concat_from_outbuffer (idx, बफर->idx + 1);
      return_trace (false);
    }

    return_trace ((यह+markArray).apply (c, mark_सूचकांक, base_सूचकांक, यह+baseArray, class_गिनती_2, idx));
  }

  bool subset (hb_subset_context_t *c) स्थिर
  {
    TRACE_SUBSET (यह);
    स्थिर hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    स्थिर hb_map_t &glyph_map = *c->plan->glyph_map;

    स्वतः *out = c->serializer->start_embed (*यह);
    अगर (unlikely (!c->serializer->extend_min (out))) return_trace (false);
    out->format = format;

    hb_map_t klass_mapping;
    Markclass_closure_and_remap_indexes (यह+markCoverage, यह+markArray, glyphset, &klass_mapping);

    अगर (!klass_mapping.get_population ()) return_trace (false);
    out->class_गिनती_2 = klass_mapping.get_population ();

    स्वतः mark_iter =
    + hb_zip (यह+markCoverage, यह+markArray)
    | hb_filter (glyphset, hb_first)
    ;

    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + mark_iter
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    अगर (!out->markCoverage.serialize_serialize (c->serializer, new_coverage.iter ()))
      return_trace (false);

    अगर (unlikely (!out->markArray.serialize_subset (c, markArray, यह,
						    (यह+markCoverage).iter (),
						    &klass_mapping)))
      return_trace (false);

    new_coverage.reset ();
    hb_sorted_vector_t<अचिन्हित> base_indexes;
    स्थिर स्वतः &base_array = यह+baseArray;
    collect_subset_anchor_indexes (यह+baseCoverage, base_array.rows,
                                   class_गिनती_2, base_array.matrixZ.arrayZ,
                                   glyphset, glyph_map, klass_mapping,
                                   new_coverage, base_indexes);

    अगर (!new_coverage) return_trace (false);
    अगर (!out->baseCoverage.serialize_serialize (c->serializer, new_coverage.iter ()))
      return_trace (false);

    return_trace (out->baseArray.serialize_subset (c, baseArray, यह,
						   new_coverage.लंबाई,
						   base_indexes.iter ()));
  }
};


}
}
}

#पूर्ण_अगर /* OT_LAYOUT_GPOS_MARKBASEPOSFORMAT1_HH */
