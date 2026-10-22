#अगर_अघोषित OT_LAYOUT_GSUB_LIGATURESUBSTFORMAT1_HH
#घोषणा OT_LAYOUT_GSUB_LIGATURESUBSTFORMAT1_HH

#समावेश "Common.hh"
#समावेश "LigatureSet.hh"

namespace OT {
namespace Layout {
namespace GSUB_impl {

ढाँचा <typename Types>
काष्ठा LigatureSubstFormat1_2
{
  रक्षित:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  typename Types::ढाँचा LOffsetTo<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of Substitution table */
  typename Types::ढाँचा ArrayOf<typename Types::ढाँचा OffsetTo<LigatureSet<Types>>>
                ligatureSet;            /* Array LigatureSet tables
                                         * ordered by Coverage Index */
  खुला:
  DEFINE_आकार_ARRAY (2 + Types::LOffset::static_आकार + Types::HBUINT::static_आकार, ligatureSet);

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (coverage.sanitize (c, यह) && ligatureSet.sanitize (c, यह));
  }

  bool intersects (स्थिर hb_set_t *glyphs) स्थिर
  {
    वापस
    + hb_zip (यह+coverage, ligatureSet)
    | hb_filter (*glyphs, hb_first)
    | hb_map (hb_second)
    | hb_map ([यह, glyphs] (स्थिर typename Types::ढाँचा OffsetTo<LigatureSet<Types>> &_)
              { वापस (यह+_).intersects (glyphs); })
    | hb_any
    ;
  }

  bool may_have_non_1to1 () स्थिर
  { वापस true; }

  व्योम depend (hb_depend_context_t *c) स्थिर
  {
    + hb_zip (यह+coverage, ligatureSet)
    | hb_filter (c->parent_active_glyphs (), hb_first)
    | hb_apply ([&] (स्थिर hb_pair_t<hb_codepoint_t, स्थिर typename Types::ढाँचा OffsetTo<LigatureSet<Types>>&> &_)
                {
                  स्थिर LigatureSet<Types>& ls = यह+_.second;
                  ls.depend (c, _.first);
                })
    ;
  }

  व्योम closure (hb_closure_context_t *c) स्थिर
  {
    + hb_zip (यह+coverage, ligatureSet)
    | hb_filter (c->parent_active_glyphs (), hb_first)
    | hb_map (hb_second)
    | hb_map (hb_add (यह))
    | hb_apply ([c] (स्थिर LigatureSet<Types> &_) { _.closure (c); })
    ;

  }

  व्योम closure_lookups (hb_closure_lookups_context_t *c) स्थिर {}

  व्योम collect_glyphs (hb_collect_glyphs_context_t *c) स्थिर
  {
    अगर (unlikely (!(यह+coverage).collect_coverage (c->निवेश))) वापस;

    + hb_zip (यह+coverage, ligatureSet)
    | hb_map (hb_second)
    | hb_map (hb_add (यह))
    | hb_apply ([c] (स्थिर LigatureSet<Types> &_) { _.collect_glyphs (c); })
    ;
  }

  व्योम collect_second_glyphs (hb_set_digest_t *digest) स्थिर
  {
    + hb_iter (ligatureSet)
    | hb_map (hb_add (यह))
    | hb_apply ([digest] (स्थिर LigatureSet<Types> &_) { _.collect_seconds (*digest); })
    ;
  }

  स्थिर Coverage &get_coverage () स्थिर { वापस यह+coverage; }

  bool would_apply (hb_would_apply_context_t *c) स्थिर
  {
    अचिन्हित पूर्णांक सूचकांक_2 = (यह+coverage).get_coverage (c->glyphs[0]);
    अगर (likely (सूचकांक_2 == NOT_COVERED)) वापस false;

    स्थिर स्वतः &lig_set = यह+ligatureSet[सूचकांक_2];
    वापस lig_set.would_apply (c);
  }

  काष्ठा external_cache_t
  {
    hb_ot_layout_mapping_cache_t coverage;
    hb_set_digest_t seconds;
  };
  व्योम *external_cache_create () स्थिर
  {
    external_cache_t *cache = (external_cache_t *) hb_malloc (माप (external_cache_t));
    अगर (likely (cache))
    {
      cache->coverage.सफा ();

      cache->seconds.init ();
      + hb_iter (ligatureSet)
      | hb_map (hb_add (यह))
      | hb_apply ([cache] (स्थिर LigatureSet<Types> &_) { _.collect_seconds (cache->seconds); })
      ;
    }
    वापस cache;
  }

  bool apply (hb_ot_apply_context_t *c, व्योम *external_cache) स्थिर
  {
    TRACE_APPLY (यह);
    hb_बफर_t *बफर = c->बफर;

#अगर_अघोषित HB_NO_OT_LAYOUT_LOOKUP_CACHE
    external_cache_t *cache = (external_cache_t *) external_cache;
    स्थिर hb_set_digest_t *seconds = cache ? &cache->seconds : nullptr;
    अचिन्हित पूर्णांक सूचकांक_2 = (यह+coverage).get_coverage (बफर->cur().codepoint, cache ? &cache->coverage : nullptr);
#अन्यथा
    स्थिर hb_set_digest_t *seconds = nullptr;
    अचिन्हित पूर्णांक सूचकांक_2 = (यह+coverage).get_coverage (बफर->cur().codepoint);
#पूर्ण_अगर
    अगर (सूचकांक_2 == NOT_COVERED) return_trace (false);

    स्थिर स्वतः &lig_set = यह+ligatureSet[सूचकांक_2];
    return_trace (lig_set.apply (c, seconds));
  }

  ढाँचा <typename GlyphID>
  bool serialize (hb_serialize_context_t *c,
                  hb_sorted_array_t<GlyphID> first_glyphs,
                  hb_array_t<स्थिर अचिन्हित पूर्णांक> ligature_per_first_glyph_गिनती_list,
                  hb_array_t<GlyphID> ligatures_list,
                  hb_array_t<स्थिर अचिन्हित पूर्णांक> component_गिनती_list,
                  hb_array_t<GlyphID> component_list /* Starting from second for each ligature */)
  {
    TRACE_SERIALIZE (यह);
    अगर (unlikely (!c->extend_min (यह))) return_trace (false);
    अगर (unlikely (!ligatureSet.serialize (c, first_glyphs.लंबाई))) return_trace (false);
    क्रम (अचिन्हित पूर्णांक i = 0; i < first_glyphs.लंबाई; i++)
    {
      अचिन्हित पूर्णांक ligature_गिनती = ligature_per_first_glyph_गिनती_list[i];
      अगर (unlikely (!ligatureSet[i]
                        .serialize_serialize (c,
                                              ligatures_list.sub_array (0, ligature_गिनती),
                                              component_गिनती_list.sub_array (0, ligature_गिनती),
                                              component_list))) return_trace (false);
      ligatures_list += ligature_गिनती;
      component_गिनती_list += ligature_गिनती;
    }
    return_trace (coverage.serialize_serialize (c, first_glyphs));
  }

  bool subset (hb_subset_context_t *c) स्थिर
  {
    TRACE_SUBSET (यह);
    स्थिर hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    स्थिर hb_map_t &glyph_map = *c->plan->glyph_map;

    स्वतः *out = c->serializer->start_embed (*यह);
    अगर (unlikely (!c->serializer->extend_min (out))) return_trace (false);
    out->format = format;

    // Due to a bug in some older versions of windows 7 the Coverage table must be
    // packed after the LigatureSet and Ligature tables, so serialize Coverage first
    // which places it last in the packed order.
    hb_set_t new_coverage;
    + hb_zip (यह+coverage, hb_iter (ligatureSet) | hb_map (hb_add (यह)))
    | hb_filter (glyphset, hb_first)
    | hb_filter ([&] (स्थिर LigatureSet<Types>& _) {
      वापस _.intersects_lig_glyph (&glyphset);
    }, hb_second)
    | hb_map (hb_first)
    | hb_sink (new_coverage);

    अगर (!c->serializer->push<Coverage> ()
        ->serialize (c->serializer,
                     + new_coverage.iter () | hb_map_retains_sorting (glyph_map)))
    {
      c->serializer->निकालो_discard ();
      return_trace (false);
    }

    अचिन्हित coverage_idx = c->serializer->निकालो_pack ();
     c->serializer->add_link (out->coverage, coverage_idx);

    + hb_zip (यह+coverage, ligatureSet)
    | hb_filter (new_coverage, hb_first)
    | hb_map (hb_second)
    // to ensure that the repacker always orders the coverage table after the LigatureSet
    // and LigatureSubtable's they will be linked to the Coverage table via a virtual link
    // the coverage table object idx is passed down to facilitate this.
    | hb_apply (subset_offset_array (c, out->ligatureSet, यह, coverage_idx))
    ;

    return_trace (bool (new_coverage));
  }
};

}
}
}

#पूर्ण_अगर  /* OT_LAYOUT_GSUB_LIGATURESUBSTFORMAT1_HH */
