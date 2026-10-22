#ifndef OT_LAYOUT_GSUB_LIGATURESUBSTFORMAT1_HH
#define OT_LAYOUT_GSUB_LIGATURESUBSTFORMAT1_HH

#அந்தர்பாவ "Common.hh"
#அந்தர்பாவ "LigatureSet.hh"

நாமாகாஶ OT {
நாமாகாஶ Layout {
நாமாகாஶ GSUB_impl {

template <typename Types>
கட்டமைப்பு LigatureSubstFormat1_2
{
  பாதுகாக்கப்பட்ட:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  typename Types::template LOffsetTo<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of Substitution table */
  typename Types::template ArrayOf<typename Types::template OffsetTo<LigatureSet<Types>>>
                ligatureSet;            /* Array LigatureSet tables
                                         * ordered by Coverage Index */
  பொது:
  DEFINE_SIZE_ARRAY (2 + Types::LOffset::static_size + Types::HBUINT::static_size, ligatureSet);

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (coverage.sanitize (c, இது) && ligatureSet.sanitize (c, இது));
  }

  தருக்க intersects (மாறிலி hb_set_t *glyphs) மாறிலி
  {
    திரும்பு
    + hb_zip (இது+coverage, ligatureSet)
    | hb_filter (*glyphs, hb_first)
    | hb_map (hb_second)
    | hb_map ([இது, glyphs] (மாறிலி typename Types::template OffsetTo<LigatureSet<Types>> &_)
              { திரும்பு (இது+_).intersects (glyphs); })
    | hb_any
    ;
  }

  தருக்க may_have_non_1to1 () மாறிலி
  { திரும்பு உண்மை; }

  வெறுமை depend (hb_depend_context_t *c) மாறிலி
  {
    + hb_zip (இது+coverage, ligatureSet)
    | hb_filter (c->parent_active_glyphs (), hb_first)
    | hb_apply ([&] (மாறிலி hb_pair_t<hb_codepoint_t, மாறிலி typename Types::template OffsetTo<LigatureSet<Types>>&> &_)
                {
                  மாறிலி LigatureSet<Types>& ls = இது+_.second;
                  ls.depend (c, _.first);
                })
    ;
  }

  வெறுமை closure (hb_closure_context_t *c) மாறிலி
  {
    + hb_zip (இது+coverage, ligatureSet)
    | hb_filter (c->parent_active_glyphs (), hb_first)
    | hb_map (hb_second)
    | hb_map (hb_add (இது))
    | hb_apply ([c] (மாறிலி LigatureSet<Types> &_) { _.closure (c); })
    ;

  }

  வெறுமை closure_lookups (hb_closure_lookups_context_t *c) மாறிலி {}

  வெறுமை collect_glyphs (hb_collect_glyphs_context_t *c) மாறிலி
  {
    எனில் (unlikely (!(இது+coverage).collect_coverage (c->உள்ளீடு))) திரும்பு;

    + hb_zip (இது+coverage, ligatureSet)
    | hb_map (hb_second)
    | hb_map (hb_add (இது))
    | hb_apply ([c] (மாறிலி LigatureSet<Types> &_) { _.collect_glyphs (c); })
    ;
  }

  வெறுமை collect_second_glyphs (hb_set_digest_t *digest) மாறிலி
  {
    + hb_iter (ligatureSet)
    | hb_map (hb_add (இது))
    | hb_apply ([digest] (மாறிலி LigatureSet<Types> &_) { _.collect_seconds (*digest); })
    ;
  }

  மாறிலி Coverage &get_coverage () மாறிலி { திரும்பு இது+coverage; }

  தருக்க would_apply (hb_would_apply_context_t *c) மாறிலி
  {
    unsigned முழு index = (இது+coverage).get_coverage (c->glyphs[0]);
    எனில் (likely (index == NOT_COVERED)) திரும்பு பொய்;

    மாறிலி auto &lig_set = இது+ligatureSet[index];
    திரும்பு lig_set.would_apply (c);
  }

  கட்டமைப்பு external_cache_t
  {
    hb_ot_layout_mapping_cache_t coverage;
    hb_set_digest_t seconds;
  };
  வெறுமை *external_cache_create () மாறிலி
  {
    external_cache_t *cache = (external_cache_t *) hb_malloc (அளவு (external_cache_t));
    எனில் (likely (cache))
    {
      cache->coverage.clear ();

      cache->seconds.init ();
      + hb_iter (ligatureSet)
      | hb_map (hb_add (இது))
      | hb_apply ([cache] (மாறிலி LigatureSet<Types> &_) { _.collect_seconds (cache->seconds); })
      ;
    }
    திரும்பு cache;
  }

  தருக்க apply (hb_ot_apply_context_t *c, வெறுமை *external_cache) மாறிலி
  {
    TRACE_APPLY (இது);
    hb_buffer_t *buffer = c->buffer;

#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    external_cache_t *cache = (external_cache_t *) external_cache;
    மாறிலி hb_set_digest_t *seconds = cache ? &cache->seconds : nullptr;
    unsigned முழு index = (இது+coverage).get_coverage (buffer->cur().codepoint, cache ? &cache->coverage : nullptr);
#இல்லையெனில்
    மாறிலி hb_set_digest_t *seconds = nullptr;
    unsigned முழு index = (இது+coverage).get_coverage (buffer->cur().codepoint);
#endif
    எனில் (index == NOT_COVERED) return_trace (பொய்);

    மாறிலி auto &lig_set = இது+ligatureSet[index];
    return_trace (lig_set.apply (c, seconds));
  }

  template <typename GlyphID>
  தருக்க serialize (hb_serialize_context_t *c,
                  hb_sorted_array_t<GlyphID> first_glyphs,
                  hb_array_t<மாறிலி unsigned முழு> ligature_per_first_glyph_மொத்தம்_list,
                  hb_array_t<GlyphID> ligatures_list,
                  hb_array_t<மாறிலி unsigned முழு> component_மொத்தம்_list,
                  hb_array_t<GlyphID> component_list /* Starting from second for each ligature */)
  {
    TRACE_SERIALIZE (இது);
    எனில் (unlikely (!c->extend_min (இது))) return_trace (பொய்);
    எனில் (unlikely (!ligatureSet.serialize (c, first_glyphs.length))) return_trace (பொய்);
    க்கு (unsigned முழு i = 0; i < first_glyphs.length; i++)
    {
      unsigned முழு ligature_மொத்தம் = ligature_per_first_glyph_மொத்தம்_list[i];
      எனில் (unlikely (!ligatureSet[i]
                        .serialize_serialize (c,
                                              ligatures_list.sub_array (0, ligature_மொத்தம்),
                                              component_மொத்தம்_list.sub_array (0, ligature_மொத்தம்),
                                              component_list))) return_trace (பொய்);
      ligatures_list += ligature_மொத்தம்;
      component_மொத்தம்_list += ligature_மொத்தம்;
    }
    return_trace (coverage.serialize_serialize (c, first_glyphs));
  }

  தருக்க subset (hb_subset_context_t *c) மாறிலி
  {
    TRACE_SUBSET (இது);
    மாறிலி hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    மாறிலி hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*இது);
    எனில் (unlikely (!c->serializer->extend_min (out))) return_trace (பொய்);
    out->format = format;

    // Due to a bug in some older versions of windows 7 the Coverage table must be
    // packed after the LigatureSet and Ligature tables, so serialize Coverage first
    // which places it last in the packed order.
    hb_set_t new_coverage;
    + hb_zip (இது+coverage, hb_iter (ligatureSet) | hb_map (hb_add (இது)))
    | hb_filter (glyphset, hb_first)
    | hb_filter ([&] (மாறிலி LigatureSet<Types>& _) {
      திரும்பு _.intersects_lig_glyph (&glyphset);
    }, hb_second)
    | hb_map (hb_first)
    | hb_sink (new_coverage);

    எனில் (!c->serializer->push<Coverage> ()
        ->serialize (c->serializer,
                     + new_coverage.iter () | hb_map_retains_sorting (glyph_map)))
    {
      c->serializer->எடு_discard ();
      return_trace (பொய்);
    }

    unsigned coverage_idx = c->serializer->எடு_pack ();
     c->serializer->add_link (out->coverage, coverage_idx);

    + hb_zip (இது+coverage, ligatureSet)
    | hb_filter (new_coverage, hb_first)
    | hb_map (hb_second)
    // to ensure that the repacker always orders the coverage table after the LigatureSet
    // and LigatureSubtable's they will be linked to the Coverage table via a virtual link
    // the coverage table object idx is passed down to facilitate this.
    | hb_apply (subset_offset_array (c, out->ligatureSet, இது, coverage_idx))
    ;

    return_trace (தருக்க (new_coverage));
  }
};

}
}
}

#endif  /* OT_LAYOUT_GSUB_LIGATURESUBSTFORMAT1_HH */
