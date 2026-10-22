#ifndef OT_LAYOUT_GSUB_LIGATURESUBSTFORMAT1_HH
#define OT_LAYOUT_GSUB_LIGATURESUBSTFORMAT1_HH

#अन्तर्भाव "Common.hh"
#अन्तर्भाव "LigatureSet.hh"

नामाकाश OT {
नामाकाश Layout {
नामाकाश GSUB_impl {

ढाँचा <typename Types>
संरचना LigatureSubstFormat1_2
{
  संरक्षित:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  typename Types::ढाँचा LOffsetTo<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of Substitution table */
  typename Types::ढाँचा ArrayOf<typename Types::ढाँचा OffsetTo<LigatureSet<Types>>>
                ligatureSet;            /* Array LigatureSet tables
                                         * ordered by Coverage Index */
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (2 + Types::LOffset::static_size + Types::HBUINT::static_size, ligatureSet);

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (coverage.sanitize (c, हे) && ligatureSet.sanitize (c, हे));
  }

  तार्किक intersects (स्थिरांक hb_set_t *glyphs) स्थिरांक
  {
    परत
    + hb_zip (हे+coverage, ligatureSet)
    | hb_filter (*glyphs, hb_first)
    | hb_map (hb_second)
    | hb_map ([हे, glyphs] (स्थिरांक typename Types::ढाँचा OffsetTo<LigatureSet<Types>> &_)
              { परत (हे+_).intersects (glyphs); })
    | hb_any
    ;
  }

  तार्किक may_have_non_1to1 () स्थिरांक
  { परत सत्य; }

  शून्य depend (hb_depend_context_t *c) स्थिरांक
  {
    + hb_zip (हे+coverage, ligatureSet)
    | hb_filter (c->parent_active_glyphs (), hb_first)
    | hb_apply ([&] (स्थिरांक hb_pair_t<hb_codepoint_t, स्थिरांक typename Types::ढाँचा OffsetTo<LigatureSet<Types>>&> &_)
                {
                  स्थिरांक LigatureSet<Types>& ls = हे+_.second;
                  ls.depend (c, _.first);
                })
    ;
  }

  शून्य closure (hb_closure_context_t *c) स्थिरांक
  {
    + hb_zip (हे+coverage, ligatureSet)
    | hb_filter (c->parent_active_glyphs (), hb_first)
    | hb_map (hb_second)
    | hb_map (hb_add (हे))
    | hb_apply ([c] (स्थिरांक LigatureSet<Types> &_) { _.closure (c); })
    ;

  }

  शून्य closure_lookups (hb_closure_lookups_context_t *c) स्थिरांक {}

  शून्य collect_glyphs (hb_collect_glyphs_context_t *c) स्थिरांक
  {
    जर (unlikely (!(हे+coverage).collect_coverage (c->आदान))) परत;

    + hb_zip (हे+coverage, ligatureSet)
    | hb_map (hb_second)
    | hb_map (hb_add (हे))
    | hb_apply ([c] (स्थिरांक LigatureSet<Types> &_) { _.collect_glyphs (c); })
    ;
  }

  शून्य collect_second_glyphs (hb_set_digest_t *digest) स्थिरांक
  {
    + hb_iter (ligatureSet)
    | hb_map (hb_add (हे))
    | hb_apply ([digest] (स्थिरांक LigatureSet<Types> &_) { _.collect_seconds (*digest); })
    ;
  }

  स्थिरांक Coverage &get_coverage () स्थिरांक { परत हे+coverage; }

  तार्किक would_apply (hb_would_apply_context_t *c) स्थिरांक
  {
    अचिन्हित पूर्णांक index = (हे+coverage).get_coverage (c->glyphs[0]);
    जर (likely (index == NOT_COVERED)) परत असत्य;

    स्थिरांक स्वतः &lig_set = हे+ligatureSet[index];
    परत lig_set.would_apply (c);
  }

  संरचना external_cache_t
  {
    hb_ot_layout_mapping_cache_t coverage;
    hb_set_digest_t seconds;
  };
  शून्य *external_cache_create () स्थिरांक
  {
    external_cache_t *cache = (external_cache_t *) hb_malloc (आकार (external_cache_t));
    जर (likely (cache))
    {
      cache->coverage.clear ();

      cache->seconds.init ();
      + hb_iter (ligatureSet)
      | hb_map (hb_add (हे))
      | hb_apply ([cache] (स्थिरांक LigatureSet<Types> &_) { _.collect_seconds (cache->seconds); })
      ;
    }
    परत cache;
  }

  तार्किक apply (hb_ot_apply_context_t *c, शून्य *external_cache) स्थिरांक
  {
    TRACE_APPLY (हे);
    hb_buffer_t *buffer = c->buffer;

#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    external_cache_t *cache = (external_cache_t *) external_cache;
    स्थिरांक hb_set_digest_t *seconds = cache ? &cache->seconds : nullptr;
    अचिन्हित पूर्णांक index = (हे+coverage).get_coverage (buffer->cur().codepoint, cache ? &cache->coverage : nullptr);
#अन्यथा
    स्थिरांक hb_set_digest_t *seconds = nullptr;
    अचिन्हित पूर्णांक index = (हे+coverage).get_coverage (buffer->cur().codepoint);
#endif
    जर (index == NOT_COVERED) return_trace (असत्य);

    स्थिरांक स्वतः &lig_set = हे+ligatureSet[index];
    return_trace (lig_set.apply (c, seconds));
  }

  ढाँचा <typename GlyphID>
  तार्किक serialize (hb_serialize_context_t *c,
                  hb_sorted_array_t<GlyphID> first_glyphs,
                  hb_array_t<स्थिरांक अचिन्हित पूर्णांक> ligature_per_first_glyph_मोजणी_list,
                  hb_array_t<GlyphID> ligatures_list,
                  hb_array_t<स्थिरांक अचिन्हित पूर्णांक> component_मोजणी_list,
                  hb_array_t<GlyphID> component_list /* Starting from second for each ligature */)
  {
    TRACE_SERIALIZE (हे);
    जर (unlikely (!c->extend_min (हे))) return_trace (असत्य);
    जर (unlikely (!ligatureSet.serialize (c, first_glyphs.length))) return_trace (असत्य);
    साठी (अचिन्हित पूर्णांक i = 0; i < first_glyphs.length; i++)
    {
      अचिन्हित पूर्णांक ligature_मोजणी = ligature_per_first_glyph_मोजणी_list[i];
      जर (unlikely (!ligatureSet[i]
                        .serialize_serialize (c,
                                              ligatures_list.sub_array (0, ligature_मोजणी),
                                              component_मोजणी_list.sub_array (0, ligature_मोजणी),
                                              component_list))) return_trace (असत्य);
      ligatures_list += ligature_मोजणी;
      component_मोजणी_list += ligature_मोजणी;
    }
    return_trace (coverage.serialize_serialize (c, first_glyphs));
  }

  तार्किक subset (hb_subset_context_t *c) स्थिरांक
  {
    TRACE_SUBSET (हे);
    स्थिरांक hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    स्थिरांक hb_map_t &glyph_map = *c->plan->glyph_map;

    स्वतः *out = c->serializer->start_embed (*हे);
    जर (unlikely (!c->serializer->extend_min (out))) return_trace (असत्य);
    out->format = format;

    // Due to a bug in some older versions of windows 7 the Coverage table must be
    // packed after the LigatureSet and Ligature tables, so serialize Coverage first
    // which places it last in the packed order.
    hb_set_t new_coverage;
    + hb_zip (हे+coverage, hb_iter (ligatureSet) | hb_map (hb_add (हे)))
    | hb_filter (glyphset, hb_first)
    | hb_filter ([&] (स्थिरांक LigatureSet<Types>& _) {
      परत _.intersects_lig_glyph (&glyphset);
    }, hb_second)
    | hb_map (hb_first)
    | hb_sink (new_coverage);

    जर (!c->serializer->push<Coverage> ()
        ->serialize (c->serializer,
                     + new_coverage.iter () | hb_map_retains_sorting (glyph_map)))
    {
      c->serializer->काढा_discard ();
      return_trace (असत्य);
    }

    अचिन्हित coverage_idx = c->serializer->काढा_pack ();
     c->serializer->add_link (out->coverage, coverage_idx);

    + hb_zip (हे+coverage, ligatureSet)
    | hb_filter (new_coverage, hb_first)
    | hb_map (hb_second)
    // to ensure that the repacker always orders the coverage table after the LigatureSet
    // and LigatureSubtable's they will be linked to the Coverage table via a virtual link
    // the coverage table object idx is passed down to facilitate this.
    | hb_apply (subset_offset_array (c, out->ligatureSet, हे, coverage_idx))
    ;

    return_trace (तार्किक (new_coverage));
  }
};

}
}
}

#endif  /* OT_LAYOUT_GSUB_LIGATURESUBSTFORMAT1_HH */
