#अगर_अघोषित OT_LAYOUT_GPOS_MARKMARKPOSFORMAT1_HH
#घोषणा OT_LAYOUT_GPOS_MARKMARKPOSFORMAT1_HH

#समावेश "MarkMarkPosFormat1.hh"

namespace OT {
namespace Layout {
namespace GPOS_impl {

ढाँचा <typename Types>
काष्ठा MarkMarkPosFormat1_2
{
  रक्षित:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  typename Types::ढाँचा LOffsetTo<Coverage>
                mark1Coverage;          /* Offset to Combining Mark1 Coverage
                                         * table--from beginning of MarkMarkPos
                                         * subtable */
  typename Types::ढाँचा LOffsetTo<Coverage>
                mark2Coverage;          /* Offset to Combining Mark2 Coverage
                                         * table--from beginning of MarkMarkPos
                                         * subtable */
  HBUINT16      class_गिनती_2;             /* Number of defined mark classes */
  typename Types::ढाँचा LOffsetTo<MarkArray<Types>>
                mark1Array;             /* Offset to Mark1Array table--from
                                         * beginning of MarkMarkPos subtable */
  typename Types::ढाँचा LOffsetTo<AnchorMatrix<Types, HBUINT16>>
                mark2Array;             /* Offset to Mark2Array table--from
                                         * beginning of MarkMarkPos subtable */
  खुला:
  DEFINE_आकार_STATIC (4 + 4 * Types::LOffset::static_आकार);

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह) &&
                  mark1Coverage.sanitize (c, यह) &&
                  mark2Coverage.sanitize (c, यह) &&
                  mark1Array.sanitize (c, यह) &&
		  hb_barrier () &&
                  mark2Array.sanitize (c, यह, (अचिन्हित पूर्णांक) class_गिनती_2));
  }

  bool intersects (स्थिर hb_set_t *glyphs) स्थिर
  {
    वापस (यह+mark1Coverage).intersects (glyphs) &&
           (यह+mark2Coverage).intersects (glyphs);
  }

  व्योम closure_lookups (hb_closure_lookups_context_t *c) स्थिर {}

  व्योम collect_variation_indices (hb_collect_variation_indices_context_t *c) स्थिर
  {
    (यह+mark1Array).collect_variation_indices (c, यह+mark1Coverage);

    hb_map_t klass_mapping;
    Markclass_closure_and_remap_indexes (यह+mark1Coverage, यह+mark1Array, *c->glyph_set, &klass_mapping);

    hb_sorted_vector_t<अचिन्हित> mark2_indexes;
    collect_anchor_indexes (यह+mark2Coverage, (यह+mark2Array).rows,
                            class_गिनती_2, *c->glyph_set, klass_mapping,
                            mark2_indexes);
    (यह+mark2Array).collect_variation_indices (c, mark2_indexes.iter ());
  }

  व्योम collect_glyphs (hb_collect_glyphs_context_t *c) स्थिर
  {
    अगर (unlikely (!(यह+mark1Coverage).collect_coverage (c->निवेश))) वापस;
    अगर (unlikely (!(यह+mark2Coverage).collect_coverage (c->निवेश))) वापस;
  }

  व्योम collect_second_glyphs (hb_set_digest_t *digest) स्थिर
  { (यह+mark2Coverage).collect_coverage (digest); }

  स्थिर Coverage &get_coverage () स्थिर { वापस यह+mark1Coverage; }

  bool apply (hb_ot_apply_context_t *c) स्थिर
  {
    TRACE_APPLY (यह);
    hb_बफर_t *बफर = c->बफर;
    अचिन्हित पूर्णांक mark1_सूचकांक = (यह+mark1Coverage).get_coverage  (बफर->cur().codepoint);
    अगर (likely (mark1_सूचकांक == NOT_COVERED)) return_trace (false);

    /* now we search backwards for a suitable mark glyph until a non-mark glyph */
    स्वतः &skippy_iter = c->iter_निवेश;
    skippy_iter.reset_fast (बफर->idx);
    skippy_iter.set_lookup_props (c->lookup_props & ~(uint32_t)LookupFlag::IgnoreFlags);
    अचिन्हित unsafe_from;
    अगर (unlikely (!skippy_iter.prev (&unsafe_from)))
    {
      बफर->unsafe_to_concat_from_outbuffer (unsafe_from, बफर->idx + 1);
      return_trace (false);
    }

    अगर (likely (!_hb_glyph_info_is_mark (&बफर->info[skippy_iter.idx])))
    {
      बफर->unsafe_to_concat_from_outbuffer (skippy_iter.idx, बफर->idx + 1);
      return_trace (false);
    }

    अचिन्हित पूर्णांक j = skippy_iter.idx;

    अचिन्हित पूर्णांक id1 = _hb_glyph_info_get_lig_id (&बफर->cur());
    अचिन्हित पूर्णांक id2 = _hb_glyph_info_get_lig_id (&बफर->info[j]);
    अचिन्हित पूर्णांक comp1 = _hb_glyph_info_get_lig_comp (&बफर->cur());
    अचिन्हित पूर्णांक comp2 = _hb_glyph_info_get_lig_comp (&बफर->info[j]);

    अगर (likely (id1 == id2))
    {
      अगर (id1 == 0) /* Marks belonging to the same base. */
        जाओ अच्छा;
      अन्यथा अगर (comp1 == comp2) /* Marks belonging to the same ligature component. */
        जाओ अच्छा;
    }
    अन्यथा
    {
      /* If ligature ids don't match, it may be the case that one of the marks
       * itself is a ligature.  In which case match. */
      अगर ((id1 > 0 && !comp1) || (id2 > 0 && !comp2))
        जाओ अच्छा;
    }

    /* Didn't match. */
    बफर->unsafe_to_concat_from_outbuffer (skippy_iter.idx, बफर->idx + 1);
    return_trace (false);

    अच्छा:
    अचिन्हित पूर्णांक mark2_सूचकांक = (यह+mark2Coverage).get_coverage  (बफर->info[j].codepoint);
    अगर (mark2_सूचकांक == NOT_COVERED)
    {
      बफर->unsafe_to_concat_from_outbuffer (skippy_iter.idx, बफर->idx + 1);
      return_trace (false);
    }

    return_trace ((यह+mark1Array).apply (c, mark1_सूचकांक, mark2_सूचकांक, यह+mark2Array, class_गिनती_2, j));
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
    Markclass_closure_and_remap_indexes (यह+mark1Coverage, यह+mark1Array, glyphset, &klass_mapping);

    अगर (!klass_mapping.get_population ()) return_trace (false);
    out->class_गिनती_2 = klass_mapping.get_population ();

    स्वतः mark1_iter =
    + hb_zip (यह+mark1Coverage, यह+mark1Array)
    | hb_filter (glyphset, hb_first)
    ;

    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + mark1_iter
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    अगर (!out->mark1Coverage.serialize_serialize (c->serializer, new_coverage.iter ()))
      return_trace (false);

    अगर (unlikely (!out->mark1Array.serialize_subset (c, mark1Array, यह,
						     (यह+mark1Coverage).iter (),
						     &klass_mapping)))
      return_trace (false);

    new_coverage.reset ();
    hb_sorted_vector_t<अचिन्हित> mark2_indexes;
    स्थिर स्वतः &mark2_array = यह+mark2Array;
    collect_subset_anchor_indexes (यह+mark2Coverage, mark2_array.rows,
                                   class_गिनती_2, mark2_array.matrixZ.arrayZ,
                                   glyphset, glyph_map, klass_mapping,
                                   new_coverage, mark2_indexes);

    अगर (!new_coverage) return_trace (false);
    अगर (!out->mark2Coverage.serialize_serialize (c->serializer, new_coverage.iter ()))
      return_trace (false);

    return_trace (out->mark2Array.serialize_subset (c, mark2Array, यह,
						    new_coverage.लंबाई,
						    mark2_indexes.iter ()));

  }
};


}
}
}

#पूर्ण_अगर /* OT_LAYOUT_GPOS_MARKMARKPOSFORMAT1_HH */
