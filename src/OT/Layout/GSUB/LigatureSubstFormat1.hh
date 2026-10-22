#ifndef OT_LAYOUT_GSUB_LIGATURESUBSTFORMAT1_HH
#define OT_LAYOUT_GSUB_LIGATURESUBSTFORMAT1_HH

#അന്തര്ഭാവ "Common.hh"
#അന്തര്ഭാവ "LigatureSet.hh"

നാമാകാശ OT {
നാമാകാശ Layout {
നാമാകാശ GSUB_impl {

template <typename Types>
ഘടന LigatureSubstFormat1_2
{
  സംരക്ഷിത:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  typename Types::template LOffsetTo<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of Substitution table */
  typename Types::template ArrayOf<typename Types::template OffsetTo<LigatureSet<Types>>>
                ligatureSet;            /* Array LigatureSet tables
                                         * ordered by Coverage Index */
  പൊതു:
  DEFINE_SIZE_ARRAY (2 + Types::LOffset::static_size + Types::HBUINT::static_size, ligatureSet);

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (coverage.sanitize (c, ഇത്) && ligatureSet.sanitize (c, ഇത്));
  }

  തർക്കം intersects (സ്ഥിരം hb_set_t *glyphs) സ്ഥിരം
  {
    തിരികെ
    + hb_zip (ഇത്+coverage, ligatureSet)
    | hb_filter (*glyphs, hb_first)
    | hb_map (hb_second)
    | hb_map ([ഇത്, glyphs] (സ്ഥിരം typename Types::template OffsetTo<LigatureSet<Types>> &_)
              { തിരികെ (ഇത്+_).intersects (glyphs); })
    | hb_any
    ;
  }

  തർക്കം may_have_non_1to1 () സ്ഥിരം
  { തിരികെ സത്യം; }

  ശൂന്യം depend (hb_depend_context_t *c) സ്ഥിരം
  {
    + hb_zip (ഇത്+coverage, ligatureSet)
    | hb_filter (c->parent_active_glyphs (), hb_first)
    | hb_apply ([&] (സ്ഥിരം hb_pair_t<hb_codepoint_t, സ്ഥിരം typename Types::template OffsetTo<LigatureSet<Types>>&> &_)
                {
                  സ്ഥിരം LigatureSet<Types>& ls = ഇത്+_.second;
                  ls.depend (c, _.first);
                })
    ;
  }

  ശൂന്യം closure (hb_closure_context_t *c) സ്ഥിരം
  {
    + hb_zip (ഇത്+coverage, ligatureSet)
    | hb_filter (c->parent_active_glyphs (), hb_first)
    | hb_map (hb_second)
    | hb_map (hb_add (ഇത്))
    | hb_apply ([c] (സ്ഥിരം LigatureSet<Types> &_) { _.closure (c); })
    ;

  }

  ശൂന്യം closure_lookups (hb_closure_lookups_context_t *c) സ്ഥിരം {}

  ശൂന്യം collect_glyphs (hb_collect_glyphs_context_t *c) സ്ഥിരം
  {
    എങ്കിൽ (unlikely (!(ഇത്+coverage).collect_coverage (c->നിവേശം))) തിരികെ;

    + hb_zip (ഇത്+coverage, ligatureSet)
    | hb_map (hb_second)
    | hb_map (hb_add (ഇത്))
    | hb_apply ([c] (സ്ഥിരം LigatureSet<Types> &_) { _.collect_glyphs (c); })
    ;
  }

  ശൂന്യം collect_second_glyphs (hb_set_digest_t *digest) സ്ഥിരം
  {
    + hb_iter (ligatureSet)
    | hb_map (hb_add (ഇത്))
    | hb_apply ([digest] (സ്ഥിരം LigatureSet<Types> &_) { _.collect_seconds (*digest); })
    ;
  }

  സ്ഥിരം Coverage &get_coverage () സ്ഥിരം { തിരികെ ഇത്+coverage; }

  തർക്കം would_apply (hb_would_apply_context_t *c) സ്ഥിരം
  {
    unsigned പൂർണ്ണസംഖ്യ index = (ഇത്+coverage).get_coverage (c->glyphs[0]);
    എങ്കിൽ (likely (index == NOT_COVERED)) തിരികെ അസത്യം;

    സ്ഥിരം auto &lig_set = ഇത്+ligatureSet[index];
    തിരികെ lig_set.would_apply (c);
  }

  ഘടന external_cache_t
  {
    hb_ot_layout_mapping_cache_t coverage;
    hb_set_digest_t seconds;
  };
  ശൂന്യം *external_cache_create () സ്ഥിരം
  {
    external_cache_t *cache = (external_cache_t *) hb_malloc (വലുപ്പം (external_cache_t));
    എങ്കിൽ (likely (cache))
    {
      cache->coverage.clear ();

      cache->seconds.init ();
      + hb_iter (ligatureSet)
      | hb_map (hb_add (ഇത്))
      | hb_apply ([cache] (സ്ഥിരം LigatureSet<Types> &_) { _.collect_seconds (cache->seconds); })
      ;
    }
    തിരികെ cache;
  }

  തർക്കം apply (hb_ot_apply_context_t *c, ശൂന്യം *external_cache) സ്ഥിരം
  {
    TRACE_APPLY (ഇത്);
    hb_buffer_t *buffer = c->buffer;

#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    external_cache_t *cache = (external_cache_t *) external_cache;
    സ്ഥിരം hb_set_digest_t *seconds = cache ? &cache->seconds : nullptr;
    unsigned പൂർണ്ണസംഖ്യ index = (ഇത്+coverage).get_coverage (buffer->cur().codepoint, cache ? &cache->coverage : nullptr);
#അല്ലെങ്കിൽ
    സ്ഥിരം hb_set_digest_t *seconds = nullptr;
    unsigned പൂർണ്ണസംഖ്യ index = (ഇത്+coverage).get_coverage (buffer->cur().codepoint);
#endif
    എങ്കിൽ (index == NOT_COVERED) return_trace (അസത്യം);

    സ്ഥിരം auto &lig_set = ഇത്+ligatureSet[index];
    return_trace (lig_set.apply (c, seconds));
  }

  template <typename GlyphID>
  തർക്കം serialize (hb_serialize_context_t *c,
                  hb_sorted_array_t<GlyphID> first_glyphs,
                  hb_array_t<സ്ഥിരം unsigned പൂർണ്ണസംഖ്യ> ligature_per_first_glyph_എണ്ണം_list,
                  hb_array_t<GlyphID> ligatures_list,
                  hb_array_t<സ്ഥിരം unsigned പൂർണ്ണസംഖ്യ> component_എണ്ണം_list,
                  hb_array_t<GlyphID> component_list /* Starting from second for each ligature */)
  {
    TRACE_SERIALIZE (ഇത്);
    എങ്കിൽ (unlikely (!c->extend_min (ഇത്))) return_trace (അസത്യം);
    എങ്കിൽ (unlikely (!ligatureSet.serialize (c, first_glyphs.length))) return_trace (അസത്യം);
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < first_glyphs.length; i++)
    {
      unsigned പൂർണ്ണസംഖ്യ ligature_എണ്ണം = ligature_per_first_glyph_എണ്ണം_list[i];
      എങ്കിൽ (unlikely (!ligatureSet[i]
                        .serialize_serialize (c,
                                              ligatures_list.sub_array (0, ligature_എണ്ണം),
                                              component_എണ്ണം_list.sub_array (0, ligature_എണ്ണം),
                                              component_list))) return_trace (അസത്യം);
      ligatures_list += ligature_എണ്ണം;
      component_എണ്ണം_list += ligature_എണ്ണം;
    }
    return_trace (coverage.serialize_serialize (c, first_glyphs));
  }

  തർക്കം subset (hb_subset_context_t *c) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);
    സ്ഥിരം hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    സ്ഥിരം hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*ഇത്);
    എങ്കിൽ (unlikely (!c->serializer->extend_min (out))) return_trace (അസത്യം);
    out->format = format;

    // Due to a bug in some older versions of windows 7 the Coverage table must be
    // packed after the LigatureSet and Ligature tables, so serialize Coverage first
    // which places it last in the packed order.
    hb_set_t new_coverage;
    + hb_zip (ഇത്+coverage, hb_iter (ligatureSet) | hb_map (hb_add (ഇത്)))
    | hb_filter (glyphset, hb_first)
    | hb_filter ([&] (സ്ഥിരം LigatureSet<Types>& _) {
      തിരികെ _.intersects_lig_glyph (&glyphset);
    }, hb_second)
    | hb_map (hb_first)
    | hb_sink (new_coverage);

    എങ്കിൽ (!c->serializer->push<Coverage> ()
        ->serialize (c->serializer,
                     + new_coverage.iter () | hb_map_retains_sorting (glyph_map)))
    {
      c->serializer->എടുക്കുക_discard ();
      return_trace (അസത്യം);
    }

    unsigned coverage_idx = c->serializer->എടുക്കുക_pack ();
     c->serializer->add_link (out->coverage, coverage_idx);

    + hb_zip (ഇത്+coverage, ligatureSet)
    | hb_filter (new_coverage, hb_first)
    | hb_map (hb_second)
    // to ensure that the repacker always orders the coverage table after the LigatureSet
    // and LigatureSubtable's they will be linked to the Coverage table via a virtual link
    // the coverage table object idx is passed down to facilitate this.
    | hb_apply (subset_offset_array (c, out->ligatureSet, ഇത്, coverage_idx))
    ;

    return_trace (തർക്കം (new_coverage));
  }
};

}
}
}

#endif  /* OT_LAYOUT_GSUB_LIGATURESUBSTFORMAT1_HH */
