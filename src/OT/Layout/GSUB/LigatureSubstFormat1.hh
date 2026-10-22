#ifndef OT_LAYOUT_GSUB_LIGATURESUBSTFORMAT1_HH
#define OT_LAYOUT_GSUB_LIGATURESUBSTFORMAT1_HH

#অন্তর্ভাব "Common.hh"
#অন্তর্ভাব "LigatureSet.hh"

নামাকাশ OT {
নামাকাশ Layout {
নামাকাশ GSUB_impl {

template <typename Types>
কাঠামো LigatureSubstFormat1_2
{
  সুরক্ষিত:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  typename Types::template LOffsetTo<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of Substitution table */
  typename Types::template ArrayOf<typename Types::template OffsetTo<LigatureSet<Types>>>
                ligatureSet;            /* Array LigatureSet tables
                                         * ordered by Coverage Index */
  সর্বজনীন:
  DEFINE_আকার_ARRAY (2 + Types::LOffset::static_আকার + Types::HBUINT::static_আকার, ligatureSet);

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (coverage.sanitize (c, এই) && ligatureSet.sanitize (c, এই));
  }

  যৌক্তিক intersects (ধ্রুবক hb_set_t *glyphs) ধ্রুবক
  {
    ফেরত
    + hb_zip (এই+coverage, ligatureSet)
    | hb_filter (*glyphs, hb_first)
    | hb_map (hb_second)
    | hb_map ([এই, glyphs] (ধ্রুবক typename Types::template OffsetTo<LigatureSet<Types>> &_)
              { ফেরত (এই+_).intersects (glyphs); })
    | hb_any
    ;
  }

  যৌক্তিক may_have_non_1to1 () ধ্রুবক
  { ফেরত সত্য; }

  শূন্য depend (hb_depend_context_t *c) ধ্রুবক
  {
    + hb_zip (এই+coverage, ligatureSet)
    | hb_filter (c->parent_active_glyphs (), hb_first)
    | hb_apply ([&] (ধ্রুবক hb_pair_t<hb_codepoint_t, ধ্রুবক typename Types::template OffsetTo<LigatureSet<Types>>&> &_)
                {
                  ধ্রুবক LigatureSet<Types>& ls = এই+_.second;
                  ls.depend (c, _.first);
                })
    ;
  }

  শূন্য closure (hb_closure_context_t *c) ধ্রুবক
  {
    + hb_zip (এই+coverage, ligatureSet)
    | hb_filter (c->parent_active_glyphs (), hb_first)
    | hb_map (hb_second)
    | hb_map (hb_add (এই))
    | hb_apply ([c] (ধ্রুবক LigatureSet<Types> &_) { _.closure (c); })
    ;

  }

  শূন্য closure_lookups (hb_closure_lookups_context_t *c) ধ্রুবক {}

  শূন্য collect_glyphs (hb_collect_glyphs_context_t *c) ধ্রুবক
  {
    যদি (unlikely (!(এই+coverage).collect_coverage (c->প্রবেশ))) ফেরত;

    + hb_zip (এই+coverage, ligatureSet)
    | hb_map (hb_second)
    | hb_map (hb_add (এই))
    | hb_apply ([c] (ধ্রুবক LigatureSet<Types> &_) { _.collect_glyphs (c); })
    ;
  }

  শূন্য collect_second_glyphs (hb_set_digest_t *digest) ধ্রুবক
  {
    + hb_iter (ligatureSet)
    | hb_map (hb_add (এই))
    | hb_apply ([digest] (ধ্রুবক LigatureSet<Types> &_) { _.collect_seconds (*digest); })
    ;
  }

  ধ্রুবক Coverage &get_coverage () ধ্রুবক { ফেরত এই+coverage; }

  যৌক্তিক would_apply (hb_would_apply_context_t *c) ধ্রুবক
  {
    unsigned পূর্ণসংখ্যা সূচক_2 = (এই+coverage).get_coverage (c->glyphs[0]);
    যদি (likely (সূচক_2 == NOT_COVERED)) ফেরত মিথ্যা;

    ধ্রুবক auto &lig_set = এই+ligatureSet[সূচক_2];
    ফেরত lig_set.would_apply (c);
  }

  কাঠামো external_cache_t
  {
    hb_ot_layout_mapping_cache_t coverage;
    hb_set_digest_t seconds;
  };
  শূন্য *external_cache_create () ধ্রুবক
  {
    external_cache_t *cache = (external_cache_t *) hb_malloc (আকার (external_cache_t));
    যদি (likely (cache))
    {
      cache->coverage.clear ();

      cache->seconds.init ();
      + hb_iter (ligatureSet)
      | hb_map (hb_add (এই))
      | hb_apply ([cache] (ধ্রুবক LigatureSet<Types> &_) { _.collect_seconds (cache->seconds); })
      ;
    }
    ফেরত cache;
  }

  যৌক্তিক apply (hb_ot_apply_context_t *c, শূন্য *external_cache) ধ্রুবক
  {
    TRACE_APPLY (এই);
    hb_বাফার_t *বাফার = c->বাফার;

#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    external_cache_t *cache = (external_cache_t *) external_cache;
    ধ্রুবক hb_set_digest_t *seconds = cache ? &cache->seconds : nullptr;
    unsigned পূর্ণসংখ্যা সূচক_2 = (এই+coverage).get_coverage (বাফার->cur().codepoint, cache ? &cache->coverage : nullptr);
#অন্যথা
    ধ্রুবক hb_set_digest_t *seconds = nullptr;
    unsigned পূর্ণসংখ্যা সূচক_2 = (এই+coverage).get_coverage (বাফার->cur().codepoint);
#endif
    যদি (সূচক_2 == NOT_COVERED) return_trace (মিথ্যা);

    ধ্রুবক auto &lig_set = এই+ligatureSet[সূচক_2];
    return_trace (lig_set.apply (c, seconds));
  }

  template <typename GlyphID>
  যৌক্তিক serialize (hb_serialize_context_t *c,
                  hb_sorted_array_t<GlyphID> first_glyphs,
                  hb_array_t<ধ্রুবক unsigned পূর্ণসংখ্যা> ligature_per_first_glyph_গুনতি_list,
                  hb_array_t<GlyphID> ligatures_list,
                  hb_array_t<ধ্রুবক unsigned পূর্ণসংখ্যা> component_গুনতি_list,
                  hb_array_t<GlyphID> component_list /* Starting from second for each ligature */)
  {
    TRACE_SERIALIZE (এই);
    যদি (unlikely (!c->extend_min (এই))) return_trace (মিথ্যা);
    যদি (unlikely (!ligatureSet.serialize (c, first_glyphs.দৈর্ঘ্য))) return_trace (মিথ্যা);
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < first_glyphs.দৈর্ঘ্য; i++)
    {
      unsigned পূর্ণসংখ্যা ligature_গুনতি = ligature_per_first_glyph_গুনতি_list[i];
      যদি (unlikely (!ligatureSet[i]
                        .serialize_serialize (c,
                                              ligatures_list.sub_array (0, ligature_গুনতি),
                                              component_গুনতি_list.sub_array (0, ligature_গুনতি),
                                              component_list))) return_trace (মিথ্যা);
      ligatures_list += ligature_গুনতি;
      component_গুনতি_list += ligature_গুনতি;
    }
    return_trace (coverage.serialize_serialize (c, first_glyphs));
  }

  যৌক্তিক subset (hb_subset_context_t *c) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    ধ্রুবক hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ধ্রুবক hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*এই);
    যদি (unlikely (!c->serializer->extend_min (out))) return_trace (মিথ্যা);
    out->format = format;

    // Due to a bug in some older versions of windows 7 the Coverage table must be
    // packed after the LigatureSet and Ligature tables, so serialize Coverage first
    // which places it last in the packed order.
    hb_set_t new_coverage;
    + hb_zip (এই+coverage, hb_iter (ligatureSet) | hb_map (hb_add (এই)))
    | hb_filter (glyphset, hb_first)
    | hb_filter ([&] (ধ্রুবক LigatureSet<Types>& _) {
      ফেরত _.intersects_lig_glyph (&glyphset);
    }, hb_second)
    | hb_map (hb_first)
    | hb_sink (new_coverage);

    যদি (!c->serializer->push<Coverage> ()
        ->serialize (c->serializer,
                     + new_coverage.iter () | hb_map_retains_sorting (glyph_map)))
    {
      c->serializer->বের_করো_discard ();
      return_trace (মিথ্যা);
    }

    unsigned coverage_idx = c->serializer->বের_করো_pack ();
     c->serializer->add_link (out->coverage, coverage_idx);

    + hb_zip (এই+coverage, ligatureSet)
    | hb_filter (new_coverage, hb_first)
    | hb_map (hb_second)
    // to ensure that the repacker always orders the coverage table after the LigatureSet
    // and LigatureSubtable's they will be linked to the Coverage table via a virtual link
    // the coverage table object idx is passed down to facilitate this.
    | hb_apply (subset_offset_array (c, out->ligatureSet, এই, coverage_idx))
    ;

    return_trace (যৌক্তিক (new_coverage));
  }
};

}
}
}

#endif  /* OT_LAYOUT_GSUB_LIGATURESUBSTFORMAT1_HH */
