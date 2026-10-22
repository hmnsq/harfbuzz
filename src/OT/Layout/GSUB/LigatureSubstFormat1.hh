#ifndef OT_LAYOUT_GSUB_LIGATURESUBSTFORMAT1_HH
#define OT_LAYOUT_GSUB_LIGATURESUBSTFORMAT1_HH

#ଅନ୍ତର୍ଭାଵ "Common.hh"
#ଅନ୍ତର୍ଭାଵ "LigatureSet.hh"

ନାମାକାଶ OT {
ନାମାକାଶ Layout {
ନାମାକାଶ GSUB_impl {

template <typename Types>
ସଂରଚନା LigatureSubstFormat1_2
{
  ସୁରକ୍ଷିତ:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  typename Types::template LOffsetTo<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of Substitution table */
  typename Types::template ArrayOf<typename Types::template OffsetTo<LigatureSet<Types>>>
                ligatureSet;            /* Array LigatureSet tables
                                         * ordered by Coverage Index */
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_ARRAY (2 + Types::LOffset::static_size + Types::HBUINT::static_size, ligatureSet);

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (coverage.sanitize (c, ଏହି) && ligatureSet.sanitize (c, ଏହି));
  }

  ତାର୍କିକ intersects (ଅଚଳ hb_set_t *glyphs) ଅଚଳ
  {
    ଫେରନ୍ତୁ
    + hb_zip (ଏହି+coverage, ligatureSet)
    | hb_filter (*glyphs, hb_first)
    | hb_map (hb_second)
    | hb_map ([ଏହି, glyphs] (ଅଚଳ typename Types::template OffsetTo<LigatureSet<Types>> &_)
              { ଫେରନ୍ତୁ (ଏହି+_).intersects (glyphs); })
    | hb_any
    ;
  }

  ତାର୍କିକ may_have_non_1to1 () ଅଚଳ
  { ଫେରନ୍ତୁ ସତ୍ୟ; }

  ଶୂନ୍ୟ depend (hb_depend_context_t *c) ଅଚଳ
  {
    + hb_zip (ଏହି+coverage, ligatureSet)
    | hb_filter (c->parent_active_glyphs (), hb_first)
    | hb_apply ([&] (ଅଚଳ hb_pair_t<hb_codepoint_t, ଅଚଳ typename Types::template OffsetTo<LigatureSet<Types>>&> &_)
                {
                  ଅଚଳ LigatureSet<Types>& ls = ଏହି+_.second;
                  ls.depend (c, _.first);
                })
    ;
  }

  ଶୂନ୍ୟ closure (hb_closure_context_t *c) ଅଚଳ
  {
    + hb_zip (ଏହି+coverage, ligatureSet)
    | hb_filter (c->parent_active_glyphs (), hb_first)
    | hb_map (hb_second)
    | hb_map (hb_add (ଏହି))
    | hb_apply ([c] (ଅଚଳ LigatureSet<Types> &_) { _.closure (c); })
    ;

  }

  ଶୂନ୍ୟ closure_lookups (hb_closure_lookups_context_t *c) ଅଚଳ {}

  ଶୂନ୍ୟ collect_glyphs (hb_collect_glyphs_context_t *c) ଅଚଳ
  {
    ଯଦି (unlikely (!(ଏହି+coverage).collect_coverage (c->ନିବେଶ))) ଫେରନ୍ତୁ;

    + hb_zip (ଏହି+coverage, ligatureSet)
    | hb_map (hb_second)
    | hb_map (hb_add (ଏହି))
    | hb_apply ([c] (ଅଚଳ LigatureSet<Types> &_) { _.collect_glyphs (c); })
    ;
  }

  ଶୂନ୍ୟ collect_second_glyphs (hb_set_digest_t *digest) ଅଚଳ
  {
    + hb_iter (ligatureSet)
    | hb_map (hb_add (ଏହି))
    | hb_apply ([digest] (ଅଚଳ LigatureSet<Types> &_) { _.collect_seconds (*digest); })
    ;
  }

  ଅଚଳ Coverage &get_coverage () ଅଚଳ { ଫେରନ୍ତୁ ଏହି+coverage; }

  ତାର୍କିକ would_apply (hb_would_apply_context_t *c) ଅଚଳ
  {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା index = (ଏହି+coverage).get_coverage (c->glyphs[0]);
    ଯଦି (likely (index == NOT_COVERED)) ଫେରନ୍ତୁ ମିଥ୍ୟା;

    ଅଚଳ auto &lig_set = ଏହି+ligatureSet[index];
    ଫେରନ୍ତୁ lig_set.would_apply (c);
  }

  ସଂରଚନା external_cache_t
  {
    hb_ot_layout_mapping_cache_t coverage;
    hb_set_digest_t seconds;
  };
  ଶୂନ୍ୟ *external_cache_create () ଅଚଳ
  {
    external_cache_t *cache = (external_cache_t *) hb_malloc (ଆକାର (external_cache_t));
    ଯଦି (likely (cache))
    {
      cache->coverage.clear ();

      cache->seconds.init ();
      + hb_iter (ligatureSet)
      | hb_map (hb_add (ଏହି))
      | hb_apply ([cache] (ଅଚଳ LigatureSet<Types> &_) { _.collect_seconds (cache->seconds); })
      ;
    }
    ଫେରନ୍ତୁ cache;
  }

  ତାର୍କିକ apply (hb_ot_apply_context_t *c, ଶୂନ୍ୟ *external_cache) ଅଚଳ
  {
    TRACE_APPLY (ଏହି);
    hb_buffer_t *buffer = c->buffer;

#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    external_cache_t *cache = (external_cache_t *) external_cache;
    ଅଚଳ hb_set_digest_t *seconds = cache ? &cache->seconds : nullptr;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା index = (ଏହି+coverage).get_coverage (buffer->cur().codepoint, cache ? &cache->coverage : nullptr);
#ଅନ୍ୟଥା
    ଅଚଳ hb_set_digest_t *seconds = nullptr;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା index = (ଏହି+coverage).get_coverage (buffer->cur().codepoint);
#endif
    ଯଦି (index == NOT_COVERED) return_trace (ମିଥ୍ୟା);

    ଅଚଳ auto &lig_set = ଏହି+ligatureSet[index];
    return_trace (lig_set.apply (c, seconds));
  }

  template <typename GlyphID>
  ତାର୍କିକ serialize (hb_serialize_context_t *c,
                  hb_sorted_array_t<GlyphID> first_glyphs,
                  hb_array_t<ଅଚଳ unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା> ligature_per_first_glyph_ଗଣନା_list,
                  hb_array_t<GlyphID> ligatures_list,
                  hb_array_t<ଅଚଳ unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା> component_ଗଣନା_list,
                  hb_array_t<GlyphID> component_list /* Starting from second for each ligature */)
  {
    TRACE_SERIALIZE (ଏହି);
    ଯଦି (unlikely (!c->extend_min (ଏହି))) return_trace (ମିଥ୍ୟା);
    ଯଦି (unlikely (!ligatureSet.serialize (c, first_glyphs.length))) return_trace (ମିଥ୍ୟା);
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < first_glyphs.length; i++)
    {
      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ligature_ଗଣନା = ligature_per_first_glyph_ଗଣନା_list[i];
      ଯଦି (unlikely (!ligatureSet[i]
                        .serialize_serialize (c,
                                              ligatures_list.sub_array (0, ligature_ଗଣନା),
                                              component_ଗଣନା_list.sub_array (0, ligature_ଗଣନା),
                                              component_list))) return_trace (ମିଥ୍ୟା);
      ligatures_list += ligature_ଗଣନା;
      component_ଗଣନା_list += ligature_ଗଣନା;
    }
    return_trace (coverage.serialize_serialize (c, first_glyphs));
  }

  ତାର୍କିକ subset (hb_subset_context_t *c) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    ଅଚଳ hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ଅଚଳ hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*ଏହି);
    ଯଦି (unlikely (!c->serializer->extend_min (out))) return_trace (ମିଥ୍ୟା);
    out->format = format;

    // Due to a bug in some older versions of windows 7 the Coverage table must be
    // packed after the LigatureSet and Ligature tables, so serialize Coverage first
    // which places it last in the packed order.
    hb_set_t new_coverage;
    + hb_zip (ଏହି+coverage, hb_iter (ligatureSet) | hb_map (hb_add (ଏହି)))
    | hb_filter (glyphset, hb_first)
    | hb_filter ([&] (ଅଚଳ LigatureSet<Types>& _) {
      ଫେରନ୍ତୁ _.intersects_lig_glyph (&glyphset);
    }, hb_second)
    | hb_map (hb_first)
    | hb_sink (new_coverage);

    ଯଦି (!c->serializer->push<Coverage> ()
        ->serialize (c->serializer,
                     + new_coverage.iter () | hb_map_retains_sorting (glyph_map)))
    {
      c->serializer->କାଢ଼_discard ();
      return_trace (ମିଥ୍ୟା);
    }

    unsigned coverage_idx = c->serializer->କାଢ଼_pack ();
     c->serializer->add_link (out->coverage, coverage_idx);

    + hb_zip (ଏହି+coverage, ligatureSet)
    | hb_filter (new_coverage, hb_first)
    | hb_map (hb_second)
    // to ensure that the repacker always orders the coverage table after the LigatureSet
    // and LigatureSubtable's they will be linked to the Coverage table via a virtual link
    // the coverage table object idx is passed down to facilitate this.
    | hb_apply (subset_offset_array (c, out->ligatureSet, ଏହି, coverage_idx))
    ;

    return_trace (ତାର୍କିକ (new_coverage));
  }
};

}
}
}

#endif  /* OT_LAYOUT_GSUB_LIGATURESUBSTFORMAT1_HH */
