#ifndef OT_LAYOUT_GSUB_LIGATURESUBSTFORMAT1_HH
#define OT_LAYOUT_GSUB_LIGATURESUBSTFORMAT1_HH

#অন্তর্ভাব "Common.hh"
#অন্তর্ভাব "LigatureSet.hh"

নামাকাশ OT {
নামাকাশ Layout {
নামাকাশ GSUB_impl {

template <typename Types>
সংরচনা LigatureSubstFormat1_2
{
  রক্ষিত:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  typename Types::template LOffsetTo<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of Substitution table */
  typename Types::template ArrayOf<typename Types::template OffsetTo<LigatureSet<Types>>>
                ligatureSet;            /* Array LigatureSet tables
                                         * ordered by Coverage Index */
  সার্বজনিক:
  DEFINE_SIZE_ARRAY (2 + Types::LOffset::static_size + Types::HBUINT::static_size, ligatureSet);

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    return_trace (coverage.sanitize (c, এতত্) && ligatureSet.sanitize (c, এতত্));
  }

  তার্কিক intersects (ধ্রুব hb_set_t *glyphs) ধ্রুব
  {
    প্রত্যাগম
    + hb_zip (এতত্+coverage, ligatureSet)
    | hb_filter (*glyphs, hb_first)
    | hb_map (hb_second)
    | hb_map ([এতত্, glyphs] (ধ্রুব typename Types::template OffsetTo<LigatureSet<Types>> &_)
              { প্রত্যাগম (এতত্+_).intersects (glyphs); })
    | hb_any
    ;
  }

  তার্কিক may_have_non_1to1 () ধ্রুব
  { প্রত্যাগম সত্য; }

  শূন্য depend (hb_depend_context_t *c) ধ্রুব
  {
    + hb_zip (এতত্+coverage, ligatureSet)
    | hb_filter (c->parent_active_glyphs (), hb_first)
    | hb_apply ([&] (ধ্রুব hb_pair_t<hb_codepoint_t, ধ্রুব typename Types::template OffsetTo<LigatureSet<Types>>&> &_)
                {
                  ধ্রুব LigatureSet<Types>& ls = এতত্+_.second;
                  ls.depend (c, _.first);
                })
    ;
  }

  শূন্য closure (hb_closure_context_t *c) ধ্রুব
  {
    + hb_zip (এতত্+coverage, ligatureSet)
    | hb_filter (c->parent_active_glyphs (), hb_first)
    | hb_map (hb_second)
    | hb_map (hb_add (এতত্))
    | hb_apply ([c] (ধ্রুব LigatureSet<Types> &_) { _.closure (c); })
    ;

  }

  শূন্য closure_lookups (hb_closure_lookups_context_t *c) ধ্রুব {}

  শূন্য collect_glyphs (hb_collect_glyphs_context_t *c) ধ্রুব
  {
    যদি (unlikely (!(এতত্+coverage).collect_coverage (c->প্রবেশ))) প্রত্যাগম;

    + hb_zip (এতত্+coverage, ligatureSet)
    | hb_map (hb_second)
    | hb_map (hb_add (এতত্))
    | hb_apply ([c] (ধ্রুব LigatureSet<Types> &_) { _.collect_glyphs (c); })
    ;
  }

  শূন্য collect_second_glyphs (hb_set_digest_t *digest) ধ্রুব
  {
    + hb_iter (ligatureSet)
    | hb_map (hb_add (এতত্))
    | hb_apply ([digest] (ধ্রুব LigatureSet<Types> &_) { _.collect_seconds (*digest); })
    ;
  }

  ধ্রুব Coverage &get_coverage () ধ্রুব { প্রত্যাগম এতত্+coverage; }

  তার্কিক would_apply (hb_would_apply_context_t *c) ধ্রুব
  {
    unsigned পূর্ণাঙ্ক index = (এতত্+coverage).get_coverage (c->glyphs[0]);
    যদি (likely (index == NOT_COVERED)) প্রত্যাগম অসত্য;

    ধ্রুব auto &lig_set = এতত্+ligatureSet[index];
    প্রত্যাগম lig_set.would_apply (c);
  }

  সংরচনা external_cache_t
  {
    hb_ot_layout_mapping_cache_t coverage;
    hb_set_digest_t seconds;
  };
  শূন্য *external_cache_create () ধ্রুব
  {
    external_cache_t *cache = (external_cache_t *) hb_malloc (পরিমাণ (external_cache_t));
    যদি (likely (cache))
    {
      cache->coverage.clear ();

      cache->seconds.init ();
      + hb_iter (ligatureSet)
      | hb_map (hb_add (এতত্))
      | hb_apply ([cache] (ধ্রুব LigatureSet<Types> &_) { _.collect_seconds (cache->seconds); })
      ;
    }
    প্রত্যাগম cache;
  }

  তার্কিক apply (hb_ot_apply_context_t *c, শূন্য *external_cache) ধ্রুব
  {
    TRACE_APPLY (এতত্);
    hb_buffer_t *buffer = c->buffer;

#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    external_cache_t *cache = (external_cache_t *) external_cache;
    ধ্রুব hb_set_digest_t *seconds = cache ? &cache->seconds : nullptr;
    unsigned পূর্ণাঙ্ক index = (এতত্+coverage).get_coverage (buffer->cur().codepoint, cache ? &cache->coverage : nullptr);
#অন্যথা
    ধ্রুব hb_set_digest_t *seconds = nullptr;
    unsigned পূর্ণাঙ্ক index = (এতত্+coverage).get_coverage (buffer->cur().codepoint);
#endif
    যদি (index == NOT_COVERED) return_trace (অসত্য);

    ধ্রুব auto &lig_set = এতত্+ligatureSet[index];
    return_trace (lig_set.apply (c, seconds));
  }

  template <typename GlyphID>
  তার্কিক serialize (hb_serialize_context_t *c,
                  hb_sorted_array_t<GlyphID> first_glyphs,
                  hb_array_t<ধ্রুব unsigned পূর্ণাঙ্ক> ligature_per_first_glyph_গুনতি_list,
                  hb_array_t<GlyphID> ligatures_list,
                  hb_array_t<ধ্রুব unsigned পূর্ণাঙ্ক> component_গুনতি_list,
                  hb_array_t<GlyphID> component_list /* Starting from second for each ligature */)
  {
    TRACE_SERIALIZE (এতত্);
    যদি (unlikely (!c->extend_min (এতত্))) return_trace (অসত্য);
    যদি (unlikely (!ligatureSet.serialize (c, first_glyphs.length))) return_trace (অসত্য);
    প্রতি (unsigned পূর্ণাঙ্ক i = 0; i < first_glyphs.length; i++)
    {
      unsigned পূর্ণাঙ্ক ligature_গুনতি = ligature_per_first_glyph_গুনতি_list[i];
      যদি (unlikely (!ligatureSet[i]
                        .serialize_serialize (c,
                                              ligatures_list.sub_array (0, ligature_গুনতি),
                                              component_গুনতি_list.sub_array (0, ligature_গুনতি),
                                              component_list))) return_trace (অসত্য);
      ligatures_list += ligature_গুনতি;
      component_গুনতি_list += ligature_গুনতি;
    }
    return_trace (coverage.serialize_serialize (c, first_glyphs));
  }

  তার্কিক subset (hb_subset_context_t *c) ধ্রুব
  {
    TRACE_SUBSET (এতত্);
    ধ্রুব hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ধ্রুব hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*এতত্);
    যদি (unlikely (!c->serializer->extend_min (out))) return_trace (অসত্য);
    out->format = format;

    // Due to a bug in some older versions of windows 7 the Coverage table must be
    // packed after the LigatureSet and Ligature tables, so serialize Coverage first
    // which places it last in the packed order.
    hb_set_t new_coverage;
    + hb_zip (এতত্+coverage, hb_iter (ligatureSet) | hb_map (hb_add (এতত্)))
    | hb_filter (glyphset, hb_first)
    | hb_filter ([&] (ধ্রুব LigatureSet<Types>& _) {
      প্রত্যাগম _.intersects_lig_glyph (&glyphset);
    }, hb_second)
    | hb_map (hb_first)
    | hb_sink (new_coverage);

    যদি (!c->serializer->push<Coverage> ()
        ->serialize (c->serializer,
                     + new_coverage.iter () | hb_map_retains_sorting (glyph_map)))
    {
      c->serializer->বের_করো_discard ();
      return_trace (অসত্য);
    }

    unsigned coverage_idx = c->serializer->বের_করো_pack ();
     c->serializer->add_link (out->coverage, coverage_idx);

    + hb_zip (এতত্+coverage, ligatureSet)
    | hb_filter (new_coverage, hb_first)
    | hb_map (hb_second)
    // to ensure that the repacker always orders the coverage table after the LigatureSet
    // and LigatureSubtable's they will be linked to the Coverage table via a virtual link
    // the coverage table object idx is passed down to facilitate this.
    | hb_apply (subset_offset_array (c, out->ligatureSet, এতত্, coverage_idx))
    ;

    return_trace (তার্কিক (new_coverage));
  }
};

}
}
}

#endif  /* OT_LAYOUT_GSUB_LIGATURESUBSTFORMAT1_HH */
