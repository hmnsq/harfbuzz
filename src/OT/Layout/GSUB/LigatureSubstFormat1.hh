#ifndef OT_LAYOUT_GSUB_LIGATURESUBSTFORMAT1_HH
#define OT_LAYOUT_GSUB_LIGATURESUBSTFORMAT1_HH

#ضمن "Common.hh"
#ضمن "LigatureSet.hh"

نطاق OT {
نطاق Layout {
نطاق GSUB_impl {

template <typename Types>
بنية LigatureSubstFormat1_2
{
  محمي:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  typename Types::template LOffsetTo<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of Substitution table */
  typename Types::template ArrayOf<typename Types::template OffsetTo<LigatureSet<Types>>>
                ligatureSet;            /* Array LigatureSet tables
                                         * ordered by Coverage Index */
  عام:
  DEFINE_SIZE_ARRAY (2 + Types::LOffset::static_size + Types::HBUINT::static_size, ligatureSet);

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (coverage.sanitize (c, هذا) && ligatureSet.sanitize (c, هذا));
  }

  منطقي intersects (ثابت hb_set_t *glyphs) ثابت
  {
    أرجع
    + hb_zip (هذا+coverage, ligatureSet)
    | hb_filter (*glyphs, hb_first)
    | hb_map (hb_second)
    | hb_map ([هذا, glyphs] (ثابت typename Types::template OffsetTo<LigatureSet<Types>> &_)
              { أرجع (هذا+_).intersects (glyphs); })
    | hb_any
    ;
  }

  منطقي may_have_non_1to1 () ثابت
  { أرجع true; }

  فراغ depend (hb_depend_context_t *c) ثابت
  {
    + hb_zip (هذا+coverage, ligatureSet)
    | hb_filter (c->parent_active_glyphs (), hb_first)
    | hb_apply ([&] (ثابت hb_pair_t<hb_codepoint_t, ثابت typename Types::template OffsetTo<LigatureSet<Types>>&> &_)
                {
                  ثابت LigatureSet<Types>& ls = هذا+_.second;
                  ls.depend (c, _.first);
                })
    ;
  }

  فراغ closure (hb_closure_context_t *c) ثابت
  {
    + hb_zip (هذا+coverage, ligatureSet)
    | hb_filter (c->parent_active_glyphs (), hb_first)
    | hb_map (hb_second)
    | hb_map (hb_add (هذا))
    | hb_apply ([c] (ثابت LigatureSet<Types> &_) { _.closure (c); })
    ;

  }

  فراغ closure_lookups (hb_closure_lookups_context_t *c) ثابت {}

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    إذا (unlikely (!(هذا+coverage).collect_coverage (c->كىرگۈزۈش))) أرجع;

    + hb_zip (هذا+coverage, ligatureSet)
    | hb_map (hb_second)
    | hb_map (hb_add (هذا))
    | hb_apply ([c] (ثابت LigatureSet<Types> &_) { _.collect_glyphs (c); })
    ;
  }

  فراغ collect_second_glyphs (hb_set_digest_t *digest) ثابت
  {
    + hb_iter (ligatureSet)
    | hb_map (hb_add (هذا))
    | hb_apply ([digest] (ثابت LigatureSet<Types> &_) { _.collect_seconds (*digest); })
    ;
  }

  ثابت Coverage &get_coverage () ثابت { أرجع هذا+coverage; }

  منطقي would_apply (hb_would_apply_context_t *c) ثابت
  {
    unsigned صحيح index = (هذا+coverage).get_coverage (c->glyphs[0]);
    إذا (likely (index == NOT_COVERED)) أرجع خطأ;

    ثابت auto &lig_set = هذا+ligatureSet[index];
    أرجع lig_set.would_apply (c);
  }

  بنية external_cache_t
  {
    hb_ot_layout_mapping_cache_t coverage;
    hb_set_digest_t seconds;
  };
  فراغ *external_cache_create () ثابت
  {
    external_cache_t *cache = (external_cache_t *) hb_malloc (حجم (external_cache_t));
    إذا (likely (cache))
    {
      cache->coverage.clear ();

      cache->seconds.init ();
      + hb_iter (ligatureSet)
      | hb_map (hb_add (هذا))
      | hb_apply ([cache] (ثابت LigatureSet<Types> &_) { _.collect_seconds (cache->seconds); })
      ;
    }
    أرجع cache;
  }

  منطقي apply (hb_ot_apply_context_t *c, فراغ *external_cache) ثابت
  {
    TRACE_APPLY (هذا);
    hb_buffer_t *buffer = c->buffer;

#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    external_cache_t *cache = (external_cache_t *) external_cache;
    ثابت hb_set_digest_t *seconds = cache ? &cache->seconds : nullptr;
    unsigned صحيح index = (هذا+coverage).get_coverage (buffer->cur().codepoint, cache ? &cache->coverage : nullptr);
#وإلا
    ثابت hb_set_digest_t *seconds = nullptr;
    unsigned صحيح index = (هذا+coverage).get_coverage (buffer->cur().codepoint);
#endif
    إذا (index == NOT_COVERED) return_trace (خطأ);

    ثابت auto &lig_set = هذا+ligatureSet[index];
    return_trace (lig_set.apply (c, seconds));
  }

  template <typename GlyphID>
  منطقي serialize (hb_serialize_context_t *c,
                  hb_sorted_array_t<GlyphID> first_glyphs,
                  hb_array_t<ثابت unsigned صحيح> ligature_per_first_glyph_سانى_list,
                  hb_array_t<GlyphID> ligatures_list,
                  hb_array_t<ثابت unsigned صحيح> component_سانى_list,
                  hb_array_t<GlyphID> component_list /* Starting from second for each ligature */)
  {
    TRACE_SERIALIZE (هذا);
    إذا (unlikely (!c->extend_min (هذا))) return_trace (خطأ);
    إذا (unlikely (!ligatureSet.serialize (c, first_glyphs.length))) return_trace (خطأ);
    لكل (unsigned صحيح i = 0; i < first_glyphs.length; i++)
    {
      unsigned صحيح ligature_سانى = ligature_per_first_glyph_سانى_list[i];
      إذا (unlikely (!ligatureSet[i]
                        .serialize_serialize (c,
                                              ligatures_list.sub_array (0, ligature_سانى),
                                              component_سانى_list.sub_array (0, ligature_سانى),
                                              component_list))) return_trace (خطأ);
      ligatures_list += ligature_سانى;
      component_سانى_list += ligature_سانى;
    }
    return_trace (coverage.serialize_serialize (c, first_glyphs));
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (out))) return_trace (خطأ);
    out->format = format;

    // Due to a bug in some older versions of windows 7 the Coverage table must be
    // packed after the LigatureSet and Ligature tables, so serialize Coverage first
    // which places it last in the packed order.
    hb_set_t new_coverage;
    + hb_zip (هذا+coverage, hb_iter (ligatureSet) | hb_map (hb_add (هذا)))
    | hb_filter (glyphset, hb_first)
    | hb_filter ([&] (ثابت LigatureSet<Types>& _) {
      أرجع _.intersects_lig_glyph (&glyphset);
    }, hb_second)
    | hb_map (hb_first)
    | hb_sink (new_coverage);

    إذا (!c->serializer->push<Coverage> ()
        ->serialize (c->serializer,
                     + new_coverage.iter () | hb_map_retains_sorting (glyph_map)))
    {
      c->serializer->چىقىرىش_discard ();
      return_trace (خطأ);
    }

    unsigned coverage_idx = c->serializer->چىقىرىش_pack ();
     c->serializer->add_link (out->coverage, coverage_idx);

    + hb_zip (هذا+coverage, ligatureSet)
    | hb_filter (new_coverage, hb_first)
    | hb_map (hb_second)
    // to ensure that the repacker always orders the coverage table after the LigatureSet
    // and LigatureSubtable's they will be linked to the Coverage table via a virtual link
    // the coverage table object idx is passed down to facilitate this.
    | hb_apply (subset_offset_array (c, out->ligatureSet, هذا, coverage_idx))
    ;

    return_trace (منطقي (new_coverage));
  }
};

}
}
}

#endif  /* OT_LAYOUT_GSUB_LIGATURESUBSTFORMAT1_HH */
