#ifndef OT_LAYOUT_GSUB_LIGATURESUBSTFORMAT1_HH
#define OT_LAYOUT_GSUB_LIGATURESUBSTFORMAT1_HH

#અન્તર્ભાવ "Common.hh"
#અન્તર્ભાવ "LigatureSet.hh"

નામાકાશ OT {
નામાકાશ Layout {
નામાકાશ GSUB_impl {

template <typename Types>
સંરચના LigatureSubstFormat1_2
{
  સુરક્ષિત:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  typename Types::template LOffsetTo<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of Substitution table */
  typename Types::template ArrayOf<typename Types::template OffsetTo<LigatureSet<Types>>>
                ligatureSet;            /* Array LigatureSet tables
                                         * ordered by Coverage Index */
  જાહેર:
  DEFINE_SIZE_ARRAY (2 + Types::LOffset::static_size + Types::HBUINT::static_size, ligatureSet);

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (coverage.sanitize (c, આ) && ligatureSet.sanitize (c, આ));
  }

  તાર્કિક intersects (અચલ hb_set_t *glyphs) અચલ
  {
    પાછા
    + hb_zip (આ+coverage, ligatureSet)
    | hb_filter (*glyphs, hb_first)
    | hb_map (hb_second)
    | hb_map ([આ, glyphs] (અચલ typename Types::template OffsetTo<LigatureSet<Types>> &_)
              { પાછા (આ+_).intersects (glyphs); })
    | hb_any
    ;
  }

  તાર્કિક may_have_non_1to1 () અચલ
  { પાછા સાચું; }

  શૂન્ય depend (hb_depend_context_t *c) અચલ
  {
    + hb_zip (આ+coverage, ligatureSet)
    | hb_filter (c->parent_active_glyphs (), hb_first)
    | hb_apply ([&] (અચલ hb_pair_t<hb_codepoint_t, અચલ typename Types::template OffsetTo<LigatureSet<Types>>&> &_)
                {
                  અચલ LigatureSet<Types>& ls = આ+_.second;
                  ls.depend (c, _.first);
                })
    ;
  }

  શૂન્ય closure (hb_closure_context_t *c) અચલ
  {
    + hb_zip (આ+coverage, ligatureSet)
    | hb_filter (c->parent_active_glyphs (), hb_first)
    | hb_map (hb_second)
    | hb_map (hb_add (આ))
    | hb_apply ([c] (અચલ LigatureSet<Types> &_) { _.closure (c); })
    ;

  }

  શૂન્ય closure_lookups (hb_closure_lookups_context_t *c) અચલ {}

  શૂન્ય collect_glyphs (hb_collect_glyphs_context_t *c) અચલ
  {
    જો (unlikely (!(આ+coverage).collect_coverage (c->આગત))) પાછા;

    + hb_zip (આ+coverage, ligatureSet)
    | hb_map (hb_second)
    | hb_map (hb_add (આ))
    | hb_apply ([c] (અચલ LigatureSet<Types> &_) { _.collect_glyphs (c); })
    ;
  }

  શૂન્ય collect_second_glyphs (hb_set_digest_t *digest) અચલ
  {
    + hb_iter (ligatureSet)
    | hb_map (hb_add (આ))
    | hb_apply ([digest] (અચલ LigatureSet<Types> &_) { _.collect_seconds (*digest); })
    ;
  }

  અચલ Coverage &get_coverage () અચલ { પાછા આ+coverage; }

  તાર્કિક would_apply (hb_would_apply_context_t *c) અચલ
  {
    unsigned પૂર્ણાંક index = (આ+coverage).get_coverage (c->glyphs[0]);
    જો (likely (index == NOT_COVERED)) પાછા ખોટું;

    અચલ auto &lig_set = આ+ligatureSet[index];
    પાછા lig_set.would_apply (c);
  }

  સંરચના external_cache_t
  {
    hb_ot_layout_mapping_cache_t coverage;
    hb_set_digest_t seconds;
  };
  શૂન્ય *external_cache_create () અચલ
  {
    external_cache_t *cache = (external_cache_t *) hb_malloc (કદ (external_cache_t));
    જો (likely (cache))
    {
      cache->coverage.clear ();

      cache->seconds.init ();
      + hb_iter (ligatureSet)
      | hb_map (hb_add (આ))
      | hb_apply ([cache] (અચલ LigatureSet<Types> &_) { _.collect_seconds (cache->seconds); })
      ;
    }
    પાછા cache;
  }

  તાર્કિક apply (hb_ot_apply_context_t *c, શૂન્ય *external_cache) અચલ
  {
    TRACE_APPLY (આ);
    hb_buffer_t *buffer = c->buffer;

#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    external_cache_t *cache = (external_cache_t *) external_cache;
    અચલ hb_set_digest_t *seconds = cache ? &cache->seconds : nullptr;
    unsigned પૂર્ણાંક index = (આ+coverage).get_coverage (buffer->cur().codepoint, cache ? &cache->coverage : nullptr);
#નહીંતર
    અચલ hb_set_digest_t *seconds = nullptr;
    unsigned પૂર્ણાંક index = (આ+coverage).get_coverage (buffer->cur().codepoint);
#endif
    જો (index == NOT_COVERED) return_trace (ખોટું);

    અચલ auto &lig_set = આ+ligatureSet[index];
    return_trace (lig_set.apply (c, seconds));
  }

  template <typename GlyphID>
  તાર્કિક serialize (hb_serialize_context_t *c,
                  hb_sorted_array_t<GlyphID> first_glyphs,
                  hb_array_t<અચલ unsigned પૂર્ણાંક> ligature_per_first_glyph_ગણતરી_list,
                  hb_array_t<GlyphID> ligatures_list,
                  hb_array_t<અચલ unsigned પૂર્ણાંક> component_ગણતરી_list,
                  hb_array_t<GlyphID> component_list /* Starting from second for each ligature */)
  {
    TRACE_SERIALIZE (આ);
    જો (unlikely (!c->extend_min (આ))) return_trace (ખોટું);
    જો (unlikely (!ligatureSet.serialize (c, first_glyphs.length))) return_trace (ખોટું);
    માટે (unsigned પૂર્ણાંક i = 0; i < first_glyphs.length; i++)
    {
      unsigned પૂર્ણાંક ligature_ગણતરી = ligature_per_first_glyph_ગણતરી_list[i];
      જો (unlikely (!ligatureSet[i]
                        .serialize_serialize (c,
                                              ligatures_list.sub_array (0, ligature_ગણતરી),
                                              component_ગણતરી_list.sub_array (0, ligature_ગણતરી),
                                              component_list))) return_trace (ખોટું);
      ligatures_list += ligature_ગણતરી;
      component_ગણતરી_list += ligature_ગણતરી;
    }
    return_trace (coverage.serialize_serialize (c, first_glyphs));
  }

  તાર્કિક subset (hb_subset_context_t *c) અચલ
  {
    TRACE_SUBSET (આ);
    અચલ hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    અચલ hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*આ);
    જો (unlikely (!c->serializer->extend_min (out))) return_trace (ખોટું);
    out->format = format;

    // Due to a bug in some older versions of windows 7 the Coverage table must be
    // packed after the LigatureSet and Ligature tables, so serialize Coverage first
    // which places it last in the packed order.
    hb_set_t new_coverage;
    + hb_zip (આ+coverage, hb_iter (ligatureSet) | hb_map (hb_add (આ)))
    | hb_filter (glyphset, hb_first)
    | hb_filter ([&] (અચલ LigatureSet<Types>& _) {
      પાછા _.intersects_lig_glyph (&glyphset);
    }, hb_second)
    | hb_map (hb_first)
    | hb_sink (new_coverage);

    જો (!c->serializer->push<Coverage> ()
        ->serialize (c->serializer,
                     + new_coverage.iter () | hb_map_retains_sorting (glyph_map)))
    {
      c->serializer->કાઢો_discard ();
      return_trace (ખોટું);
    }

    unsigned coverage_idx = c->serializer->કાઢો_pack ();
     c->serializer->add_link (out->coverage, coverage_idx);

    + hb_zip (આ+coverage, ligatureSet)
    | hb_filter (new_coverage, hb_first)
    | hb_map (hb_second)
    // to ensure that the repacker always orders the coverage table after the LigatureSet
    // and LigatureSubtable's they will be linked to the Coverage table via a virtual link
    // the coverage table object idx is passed down to facilitate this.
    | hb_apply (subset_offset_array (c, out->ligatureSet, આ, coverage_idx))
    ;

    return_trace (તાર્કિક (new_coverage));
  }
};

}
}
}

#endif  /* OT_LAYOUT_GSUB_LIGATURESUBSTFORMAT1_HH */
