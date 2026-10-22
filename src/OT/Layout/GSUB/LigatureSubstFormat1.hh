#ifndef OT_LAYOUT_GSUB_LIGATURESUBSTFORMAT1_HH
#define OT_LAYOUT_GSUB_LIGATURESUBSTFORMAT1_HH

#ಅನ್ತರ್ಭಾವ "Common.hh"
#ಅನ್ತರ್ಭಾವ "LigatureSet.hh"

ನಾಮಾಕಾಶ OT {
ನಾಮಾಕಾಶ Layout {
ನಾಮಾಕಾಶ GSUB_impl {

template <typename Types>
ಸಂರಚನಾ LigatureSubstFormat1_2
{
  ರಕ್ಷಿತ:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  typename Types::template LOffsetTo<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of Substitution table */
  typename Types::template ArrayOf<typename Types::template OffsetTo<LigatureSet<Types>>>
                ligatureSet;            /* Array LigatureSet tables
                                         * ordered by Coverage Index */
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_ARRAY (2 + Types::LOffset::static_size + Types::HBUINT::static_size, ligatureSet);

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (coverage.sanitize (c, ಏತತ್) && ligatureSet.sanitize (c, ಏತತ್));
  }

  ತಾರ್ಕಿಕ intersects (ಧ್ರುವ hb_set_t *glyphs) ಧ್ರುವ
  {
    ಪ್ರತ್ಯಾಗಮ
    + hb_zip (ಏತತ್+coverage, ligatureSet)
    | hb_filter (*glyphs, hb_first)
    | hb_map (hb_second)
    | hb_map ([ಏತತ್, glyphs] (ಧ್ರುವ typename Types::template OffsetTo<LigatureSet<Types>> &_)
              { ಪ್ರತ್ಯಾಗಮ (ಏತತ್+_).intersects (glyphs); })
    | hb_any
    ;
  }

  ತಾರ್ಕಿಕ may_have_non_1to1 () ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ; }

  ಶೂನ್ಯ depend (hb_depend_context_t *c) ಧ್ರುವ
  {
    + hb_zip (ಏತತ್+coverage, ligatureSet)
    | hb_filter (c->parent_active_glyphs (), hb_first)
    | hb_apply ([&] (ಧ್ರುವ hb_pair_t<hb_codepoint_t, ಧ್ರುವ typename Types::template OffsetTo<LigatureSet<Types>>&> &_)
                {
                  ಧ್ರುವ LigatureSet<Types>& ls = ಏತತ್+_.second;
                  ls.depend (c, _.first);
                })
    ;
  }

  ಶೂನ್ಯ closure (hb_closure_context_t *c) ಧ್ರುವ
  {
    + hb_zip (ಏತತ್+coverage, ligatureSet)
    | hb_filter (c->parent_active_glyphs (), hb_first)
    | hb_map (hb_second)
    | hb_map (hb_add (ಏತತ್))
    | hb_apply ([c] (ಧ್ರುವ LigatureSet<Types> &_) { _.closure (c); })
    ;

  }

  ಶೂನ್ಯ closure_lookups (hb_closure_lookups_context_t *c) ಧ್ರುವ {}

  ಶೂನ್ಯ collect_glyphs (hb_collect_glyphs_context_t *c) ಧ್ರುವ
  {
    ಯದಿ (unlikely (!(ಏತತ್+coverage).collect_coverage (c->ಒಳಹರಿವು))) ಪ್ರತ್ಯಾಗಮ;

    + hb_zip (ಏತತ್+coverage, ligatureSet)
    | hb_map (hb_second)
    | hb_map (hb_add (ಏತತ್))
    | hb_apply ([c] (ಧ್ರುವ LigatureSet<Types> &_) { _.collect_glyphs (c); })
    ;
  }

  ಶೂನ್ಯ collect_second_glyphs (hb_set_digest_t *digest) ಧ್ರುವ
  {
    + hb_iter (ligatureSet)
    | hb_map (hb_add (ಏತತ್))
    | hb_apply ([digest] (ಧ್ರುವ LigatureSet<Types> &_) { _.collect_seconds (*digest); })
    ;
  }

  ಧ್ರುವ Coverage &get_coverage () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ ಏತತ್+coverage; }

  ತಾರ್ಕಿಕ would_apply (hb_would_apply_context_t *c) ಧ್ರುವ
  {
    unsigned ಪೂರ್ಣಾಙ್ಕ index = (ಏತತ್+coverage).get_coverage (c->glyphs[0]);
    ಯದಿ (likely (index == NOT_COVERED)) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    ಧ್ರುವ auto &lig_set = ಏತತ್+ligatureSet[index];
    ಪ್ರತ್ಯಾಗಮ lig_set.would_apply (c);
  }

  ಸಂರಚನಾ external_cache_t
  {
    hb_ot_layout_mapping_cache_t coverage;
    hb_set_digest_t seconds;
  };
  ಶೂನ್ಯ *external_cache_create () ಧ್ರುವ
  {
    external_cache_t *cache = (external_cache_t *) hb_malloc (ಪರಿಮಾಣ (external_cache_t));
    ಯದಿ (likely (cache))
    {
      cache->coverage.clear ();

      cache->seconds.init ();
      + hb_iter (ligatureSet)
      | hb_map (hb_add (ಏತತ್))
      | hb_apply ([cache] (ಧ್ರುವ LigatureSet<Types> &_) { _.collect_seconds (cache->seconds); })
      ;
    }
    ಪ್ರತ್ಯಾಗಮ cache;
  }

  ತಾರ್ಕಿಕ apply (hb_ot_apply_context_t *c, ಶೂನ್ಯ *external_cache) ಧ್ರುವ
  {
    TRACE_APPLY (ಏತತ್);
    hb_buffer_t *buffer = c->buffer;

#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    external_cache_t *cache = (external_cache_t *) external_cache;
    ಧ್ರುವ hb_set_digest_t *seconds = cache ? &cache->seconds : nullptr;
    unsigned ಪೂರ್ಣಾಙ್ಕ index = (ಏತತ್+coverage).get_coverage (buffer->cur().codepoint, cache ? &cache->coverage : nullptr);
#ಅನ್ಯಥಾ
    ಧ್ರುವ hb_set_digest_t *seconds = nullptr;
    unsigned ಪೂರ್ಣಾಙ್ಕ index = (ಏತತ್+coverage).get_coverage (buffer->cur().codepoint);
#endif
    ಯದಿ (index == NOT_COVERED) return_trace (ಅಸತ್ಯ);

    ಧ್ರುವ auto &lig_set = ಏತತ್+ligatureSet[index];
    return_trace (lig_set.apply (c, seconds));
  }

  template <typename GlyphID>
  ತಾರ್ಕಿಕ serialize (hb_serialize_context_t *c,
                  hb_sorted_array_t<GlyphID> first_glyphs,
                  hb_array_t<ಧ್ರುವ unsigned ಪೂರ್ಣಾಙ್ಕ> ligature_per_first_glyph_ಎಣಿಕೆ_list,
                  hb_array_t<GlyphID> ligatures_list,
                  hb_array_t<ಧ್ರುವ unsigned ಪೂರ್ಣಾಙ್ಕ> component_ಎಣಿಕೆ_list,
                  hb_array_t<GlyphID> component_list /* Starting from second for each ligature */)
  {
    TRACE_SERIALIZE (ಏತತ್);
    ಯದಿ (unlikely (!c->extend_min (ಏತತ್))) return_trace (ಅಸತ್ಯ);
    ಯದಿ (unlikely (!ligatureSet.serialize (c, first_glyphs.length))) return_trace (ಅಸತ್ಯ);
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < first_glyphs.length; i++)
    {
      unsigned ಪೂರ್ಣಾಙ್ಕ ligature_ಎಣಿಕೆ = ligature_per_first_glyph_ಎಣಿಕೆ_list[i];
      ಯದಿ (unlikely (!ligatureSet[i]
                        .serialize_serialize (c,
                                              ligatures_list.sub_array (0, ligature_ಎಣಿಕೆ),
                                              component_ಎಣಿಕೆ_list.sub_array (0, ligature_ಎಣಿಕೆ),
                                              component_list))) return_trace (ಅಸತ್ಯ);
      ligatures_list += ligature_ಎಣಿಕೆ;
      component_ಎಣಿಕೆ_list += ligature_ಎಣಿಕೆ;
    }
    return_trace (coverage.serialize_serialize (c, first_glyphs));
  }

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    ಧ್ರುವ hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ಧ್ರುವ hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*ಏತತ್);
    ಯದಿ (unlikely (!c->serializer->extend_min (out))) return_trace (ಅಸತ್ಯ);
    out->format = format;

    // Due to a bug in some older versions of windows 7 the Coverage table must be
    // packed after the LigatureSet and Ligature tables, so serialize Coverage first
    // which places it last in the packed order.
    hb_set_t new_coverage;
    + hb_zip (ಏತತ್+coverage, hb_iter (ligatureSet) | hb_map (hb_add (ಏತತ್)))
    | hb_filter (glyphset, hb_first)
    | hb_filter ([&] (ಧ್ರುವ LigatureSet<Types>& _) {
      ಪ್ರತ್ಯಾಗಮ _.intersects_lig_glyph (&glyphset);
    }, hb_second)
    | hb_map (hb_first)
    | hb_sink (new_coverage);

    ಯದಿ (!c->serializer->push<Coverage> ()
        ->serialize (c->serializer,
                     + new_coverage.iter () | hb_map_retains_sorting (glyph_map)))
    {
      c->serializer->ತೆಗೆ_discard ();
      return_trace (ಅಸತ್ಯ);
    }

    unsigned coverage_idx = c->serializer->ತೆಗೆ_pack ();
     c->serializer->add_link (out->coverage, coverage_idx);

    + hb_zip (ಏತತ್+coverage, ligatureSet)
    | hb_filter (new_coverage, hb_first)
    | hb_map (hb_second)
    // to ensure that the repacker always orders the coverage table after the LigatureSet
    // and LigatureSubtable's they will be linked to the Coverage table via a virtual link
    // the coverage table object idx is passed down to facilitate this.
    | hb_apply (subset_offset_array (c, out->ligatureSet, ಏತತ್, coverage_idx))
    ;

    return_trace (ತಾರ್ಕಿಕ (new_coverage));
  }
};

}
}
}

#endif  /* OT_LAYOUT_GSUB_LIGATURESUBSTFORMAT1_HH */
