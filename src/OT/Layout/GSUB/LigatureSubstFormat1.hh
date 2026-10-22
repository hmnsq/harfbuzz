#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ OT_LAYOUT_GSUB_LIGATURESUBSTFORMAT1_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OT_LAYOUT_GSUB_LIGATURESUBSTFORMAT1_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "Common.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "LigatureSet.hh"

ਨਾਮਾਕਾਸ਼ OT {
ਨਾਮਾਕਾਸ਼ Layout {
ਨਾਮਾਕਾਸ਼ GSUB_impl {

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Types>
ਬਣਤਰ LigatureSubstFormat1_2
{
  ਸੁਰੱਖਿਅਤ:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  ਕਿਸਮ_ਨਾਂ Types::ਸਾਂਚਾ LOffset_ਮੰਜ਼ਿਲ<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of Substitution table */
  ਕਿਸਮ_ਨਾਂ Types::ਸਾਂਚਾ Array_ਦਾ<ਕਿਸਮ_ਨਾਂ Types::ਸਾਂਚਾ ਵਿੱਥ_ਮੰਜ਼ਿਲ<Ligature_ਸੈੱਟ<Types>>>
                ligature_ਸੈੱਟ;            /* Array LigatureSet tables
                                         * ordered by Coverage Index */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_ARRAY (2 + Types::LOffset::static_ਮਾਪ + Types::HBUINT::static_ਮਾਪ, ligature_ਸੈੱਟ);

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (coverage.sanitize (c, ਇਹ) && ligature_ਸੈੱਟ.sanitize (c, ਇਹ));
  }

  ਤਾਰਕਿਕ intersects (ਅਚਲ hb_ਸੈੱਟ_t *glyphs) ਅਚਲ
  {
    ਵਾਪਸ
    + hb_zip (ਇਹ+coverage, ligature_ਸੈੱਟ)
    | hb_filter (*glyphs, hb_ਪਹਿਲਾ)
    | hb_map (hb_ਦੂਜਾ)
    | hb_map ([ਇਹ, glyphs] (ਅਚਲ ਕਿਸਮ_ਨਾਂ Types::ਸਾਂਚਾ ਵਿੱਥ_ਮੰਜ਼ਿਲ<Ligature_ਸੈੱਟ<Types>> &_)
              { ਵਾਪਸ (ਇਹ+_).intersects (glyphs); })
    | hb_any
    ;
  }

  ਤਾਰਕਿਕ may_have_non_1to1 () ਅਚਲ
  { ਵਾਪਸ ਸੱਚ; }

  ਖਾਲੀ depend (hb_depend_context_t *c) ਅਚਲ
  {
    + hb_zip (ਇਹ+coverage, ligature_ਸੈੱਟ)
    | hb_filter (c->ਮਾਪਾ_ਸਰਗਰਮ_glyphs (), hb_ਪਹਿਲਾ)
    | hb_apply ([&] (ਅਚਲ hb_pair_t<hb_codepoint_t, ਅਚਲ ਕਿਸਮ_ਨਾਂ Types::ਸਾਂਚਾ ਵਿੱਥ_ਮੰਜ਼ਿਲ<Ligature_ਸੈੱਟ<Types>>&> &_)
                {
                  ਅਚਲ Ligature_ਸੈੱਟ<Types>& ls = ਇਹ+_.second;
                  ls.depend (c, _.ਪਹਿਲਾ);
                })
    ;
  }

  ਖਾਲੀ ਘੇਰਾ (hb_ਘੇਰਾ_context_t *c) ਅਚਲ
  {
    + hb_zip (ਇਹ+coverage, ligature_ਸੈੱਟ)
    | hb_filter (c->ਮਾਪਾ_ਸਰਗਰਮ_glyphs (), hb_ਪਹਿਲਾ)
    | hb_map (hb_ਦੂਜਾ)
    | hb_map (hb_ਜੋੜੋ (ਇਹ))
    | hb_apply ([c] (ਅਚਲ Ligature_ਸੈੱਟ<Types> &_) { _.ਘੇਰਾ (c); })
    ;

  }

  ਖਾਲੀ ਘੇਰਾ_lookups (hb_ਘੇਰਾ_lookups_context_t *c) ਅਚਲ {}

  ਖਾਲੀ collect_glyphs (hb_collect_glyphs_context_t *c) ਅਚਲ
  {
    ਜੇ (unlikely (!(ਇਹ+coverage).collect_coverage (c->ਨਿਵੇਸ਼))) ਵਾਪਸ;

    + hb_zip (ਇਹ+coverage, ligature_ਸੈੱਟ)
    | hb_map (hb_ਦੂਜਾ)
    | hb_map (hb_ਜੋੜੋ (ਇਹ))
    | hb_apply ([c] (ਅਚਲ Ligature_ਸੈੱਟ<Types> &_) { _.collect_glyphs (c); })
    ;
  }

  ਖਾਲੀ collect_ਦੂਜਾ_glyphs (hb_ਸੈੱਟ_digest_t *digest) ਅਚਲ
  {
    + hb_iter (ligature_ਸੈੱਟ)
    | hb_map (hb_ਜੋੜੋ (ਇਹ))
    | hb_apply ([digest] (ਅਚਲ Ligature_ਸੈੱਟ<Types> &_) { _.collect_seconds (*digest); })
    ;
  }

  ਅਚਲ Coverage &get_coverage () ਅਚਲ { ਵਾਪਸ ਇਹ+coverage; }

  ਤਾਰਕਿਕ would_apply (hb_would_apply_context_t *c) ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸੂਚਕਾਂਕ_2 = (ਇਹ+coverage).get_coverage (c->glyphs[0]);
    ਜੇ (likely (ਸੂਚਕਾਂਕ_2 == NOT_COVERED)) ਵਾਪਸ ਝੂਠ;

    ਅਚਲ ਸਵੈਚਾਲਿਤ &lig_ਸੈੱਟ = ਇਹ+ligature_ਸੈੱਟ[ਸੂਚਕਾਂਕ_2];
    ਵਾਪਸ lig_ਸੈੱਟ.would_apply (c);
  }

  ਬਣਤਰ external_cache_t
  {
    hb_ot_layout_mapping_cache_t coverage;
    hb_ਸੈੱਟ_digest_t seconds;
  };
  ਖਾਲੀ *external_cache_create () ਅਚਲ
  {
    external_cache_t *cache = (external_cache_t *) hb_malloc (ਆਕਾਰ (external_cache_t));
    ਜੇ (likely (cache))
    {
      cache->coverage.clear ();

      cache->seconds.init ();
      + hb_iter (ligature_ਸੈੱਟ)
      | hb_map (hb_ਜੋੜੋ (ਇਹ))
      | hb_apply ([cache] (ਅਚਲ Ligature_ਸੈੱਟ<Types> &_) { _.collect_seconds (cache->seconds); })
      ;
    }
    ਵਾਪਸ cache;
  }

  ਤਾਰਕਿਕ apply (hb_ot_apply_context_t *c, ਖਾਲੀ *external_cache) ਅਚਲ
  {
    TRACE_APPLY (ਇਹ);
    hb_ਬਫਰ_t *ਬਫਰ = c->ਬਫਰ;

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_OT_LAYOUT_LOOKUP_CACHE
    external_cache_t *cache = (external_cache_t *) external_cache;
    ਅਚਲ hb_ਸੈੱਟ_digest_t *seconds = cache ? &cache->seconds : ਖਾਲੀ_ਸੂਚਕ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸੂਚਕਾਂਕ_2 = (ਇਹ+coverage).get_coverage (ਬਫਰ->cur().codepoint, cache ? &cache->coverage : ਖਾਲੀ_ਸੂਚਕ);
#ਨਹੀਂਤਾਂ
    ਅਚਲ hb_ਸੈੱਟ_digest_t *seconds = ਖਾਲੀ_ਸੂਚਕ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸੂਚਕਾਂਕ_2 = (ਇਹ+coverage).get_coverage (ਬਫਰ->cur().codepoint);
#ਜੇ_ਅੰਤ
    ਜੇ (ਸੂਚਕਾਂਕ_2 == NOT_COVERED) return_trace (ਝੂਠ);

    ਅਚਲ ਸਵੈਚਾਲਿਤ &lig_ਸੈੱਟ = ਇਹ+ligature_ਸੈੱਟ[ਸੂਚਕਾਂਕ_2];
    return_trace (lig_ਸੈੱਟ.apply (c, seconds));
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ GlyphID>
  ਤਾਰਕਿਕ serialize (hb_serialize_context_t *c,
                  hb_sorted_array_t<GlyphID> ਪਹਿਲਾ_glyphs,
                  hb_array_t<ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ> ligature_ਪ੍ਰਤੀ_ਪਹਿਲਾ_glyph_ਗਿਣਤੀ_list,
                  hb_array_t<GlyphID> ligatures_list,
                  hb_array_t<ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ> component_ਗਿਣਤੀ_list,
                  hb_array_t<GlyphID> component_list /* Starting from second for each ligature */)
  {
    TRACE_SERIALIZE (ਇਹ);
    ਜੇ (unlikely (!c->extend_min (ਇਹ))) return_trace (ਝੂਠ);
    ਜੇ (unlikely (!ligature_ਸੈੱਟ.serialize (c, ਪਹਿਲਾ_glyphs.ਲੰਮਾਈ))) return_trace (ਝੂਠ);
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਪਹਿਲਾ_glyphs.ਲੰਮਾਈ; i++)
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ligature_ਗਿਣਤੀ = ligature_ਪ੍ਰਤੀ_ਪਹਿਲਾ_glyph_ਗਿਣਤੀ_list[i];
      ਜੇ (unlikely (!ligature_ਸੈੱਟ[i]
                        .serialize_serialize (c,
                                              ligatures_list.sub_array (0, ligature_ਗਿਣਤੀ),
                                              component_ਗਿਣਤੀ_list.sub_array (0, ligature_ਗਿਣਤੀ),
                                              component_list))) return_trace (ਝੂਠ);
      ligatures_list += ligature_ਗਿਣਤੀ;
      component_ਗਿਣਤੀ_list += ligature_ਗਿਣਤੀ;
    }
    return_trace (coverage.serialize_serialize (c, ਪਹਿਲਾ_glyphs));
  }

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਅਚਲ hb_ਸੈੱਟ_t &glyphset = *c->plan->glyphset_gsub ();
    ਅਚਲ hb_map_t &glyph_map = *c->plan->glyph_map;

    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->serializer->ਸ਼ੁਰੂ_embed (*ਇਹ);
    ਜੇ (unlikely (!c->serializer->extend_min (ਬਾਹਰਲਾ))) return_trace (ਝੂਠ);
    ਬਾਹਰਲਾ->format = format;

    // Due to a bug in some older versions of windows 7 the Coverage table must be
    // packed after the LigatureSet and Ligature tables, so serialize Coverage first
    // which places it last in the packed order.
    hb_ਸੈੱਟ_t new_coverage;
    + hb_zip (ਇਹ+coverage, hb_iter (ligature_ਸੈੱਟ) | hb_map (hb_ਜੋੜੋ (ਇਹ)))
    | hb_filter (glyphset, hb_ਪਹਿਲਾ)
    | hb_filter ([&] (ਅਚਲ Ligature_ਸੈੱਟ<Types>& _) {
      ਵਾਪਸ _.intersects_lig_glyph (&glyphset);
    }, hb_ਦੂਜਾ)
    | hb_map (hb_ਪਹਿਲਾ)
    | hb_sink (new_coverage);

    ਜੇ (!c->serializer->push<Coverage> ()
        ->serialize (c->serializer,
                     + new_coverage.iter () | hb_map_retains_ਛਾਂਟੀ (glyph_map)))
    {
      c->serializer->ਕੱਢੋ_discard ();
      return_trace (ਝੂਠ);
    }

    ਬਿਨਾਨਿਸ਼ਾਨੀ coverage_idx = c->serializer->ਕੱਢੋ_pack ();
     c->serializer->ਜੋੜੋ_link (ਬਾਹਰਲਾ->coverage, coverage_idx);

    + hb_zip (ਇਹ+coverage, ligature_ਸੈੱਟ)
    | hb_filter (new_coverage, hb_ਪਹਿਲਾ)
    | hb_map (hb_ਦੂਜਾ)
    // to ensure that the repacker always orders the coverage table after the LigatureSet
    // and LigatureSubtable's they will be linked to the Coverage table via a virtual link
    // the coverage table object idx is passed down to facilitate this.
    | hb_apply (subset_ਵਿੱਥ_array (c, ਬਾਹਰਲਾ->ligature_ਸੈੱਟ, ਇਹ, coverage_idx))
    ;

    return_trace (ਤਾਰਕਿਕ (new_coverage));
  }
};

}
}
}

#ਜੇ_ਅੰਤ  /* OT_LAYOUT_GSUB_LIGATURESUBSTFORMAT1_HH */
