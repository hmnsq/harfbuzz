#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ OT_LAYOUT_GPOS_PAIRPOSFORMAT1_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OT_LAYOUT_GPOS_PAIRPOSFORMAT1_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "PairSet.hh"

ਨਾਮਾਕਾਸ਼ OT {
ਨਾਮਾਕਾਸ਼ Layout {
ਨਾਮਾਕਾਸ਼ GPOS_impl {


ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Types>
ਬਣਤਰ PairPosFormat1_3
{
  ਵਰਤੋਂ Pair_ਸੈੱਟ = GPOS_impl::Pair_ਸੈੱਟ<Types>;
  ਵਰਤੋਂ Pair_ਮੁੱਲ_ਰਿਕਾਰਡ = GPOS_impl::Pair_ਮੁੱਲ_ਰਿਕਾਰਡ<Types>;

  ਸੁਰੱਖਿਅਤ:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  ਕਿਸਮ_ਨਾਂ Types::ਸਾਂਚਾ LOffset_ਮੰਜ਼ਿਲ<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of subtable */
  ਮੁੱਲ_Format   ਮੁੱਲ_Format_2[2];         /* [0] Defines the types of data in
                                         * ValueRecord1--for the first glyph
                                         * in the pair--may be zero (0) */
                                        /* [1] Defines the types of data in
                                         * ValueRecord2--for the second glyph
                                         * in the pair--may be zero (0) */
  ਕਿਸਮ_ਨਾਂ Types::ਸਾਂਚਾ Array_ਦਾ<ਕਿਸਮ_ਨਾਂ Types::ਸਾਂਚਾ ਵਿੱਥ_ਮੰਜ਼ਿਲ<Pair_ਸੈੱਟ>>
                pair_ਸੈੱਟ;                /* Array of PairSet tables ordered by
                                         * Coverage Index */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_ARRAY (2 + Types::LOffset::static_ਮਾਪ + 2 * ਮੁੱਲ_Format::static_ਮਾਪ + Types::HBUINT::static_ਮਾਪ, pair_ਸੈੱਟ);

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);

    ਜੇ (!c->check_struct (ਇਹ)) return_trace (ਝੂਠ);
    hb_barrier ();

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ len1 = ਮੁੱਲ_Format_2[0].get_len ();
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ len2 = ਮੁੱਲ_Format_2[1].get_len ();
    ਕਿਸਮ_ਨਾਂ Pair_ਸੈੱਟ::sanitize_ਘੇਰਾ_t ਘੇਰਾ =
    {
      ਮੁੱਲ_Format_2,
      len1,
      Pair_ਸੈੱਟ::get_ਮਾਪ (len1, len2)
    };

    return_trace (coverage.sanitize (c, ਇਹ) && pair_ਸੈੱਟ.sanitize (c, ਇਹ, &ਘੇਰਾ));
  }

  ਤਾਰਕਿਕ intersects (ਅਚਲ hb_ਸੈੱਟ_t *glyphs) ਅਚਲ
  {
    ਸਵੈਚਾਲਿਤ &cov = ਇਹ+coverage;

    ਜੇ (pair_ਸੈੱਟ.len > glyphs->get_population () * hb_ਦੋਆਧਾਰੀ_ਅੰਕ_storage ((ਬਿਨਾਨਿਸ਼ਾਨੀ) pair_ਸੈੱਟ.len))
    {
      ਲਈ (hb_codepoint_t g : glyphs->iter ())
      {
	ਬਿਨਾਨਿਸ਼ਾਨੀ i = cov.get_coverage (g);
	ਜੇ ((ਇਹ+pair_ਸੈੱਟ[i]).intersects (glyphs, ਮੁੱਲ_Format_2))
	  ਵਾਪਸ ਸੱਚ;
      }
      ਵਾਪਸ ਝੂਠ;
    }

    ਵਾਪਸ
    + hb_zip (cov, pair_ਸੈੱਟ)
    | hb_filter (*glyphs, hb_ਪਹਿਲਾ)
    | hb_map (hb_ਦੂਜਾ)
    | hb_map ([glyphs, ਇਹ] (ਅਚਲ ਕਿਸਮ_ਨਾਂ Types::ਸਾਂਚਾ ਵਿੱਥ_ਮੰਜ਼ਿਲ<Pair_ਸੈੱਟ> &_)
              { ਵਾਪਸ (ਇਹ+_).intersects (glyphs, ਮੁੱਲ_Format_2); })
    | hb_any
    ;
  }

  ਖਾਲੀ ਘੇਰਾ_lookups (hb_ਘੇਰਾ_lookups_context_t *c) ਅਚਲ {}
  ਖਾਲੀ collect_variation_indices (hb_collect_variation_indices_context_t *c) ਅਚਲ
  {
    ਜੇ ((!ਮੁੱਲ_Format_2[0].has_device ()) && (!ਮੁੱਲ_Format_2[1].has_device ())) ਵਾਪਸ;

    ਸਵੈਚਾਲਿਤ it =
    + hb_zip (ਇਹ+coverage, pair_ਸੈੱਟ)
    | hb_filter (c->glyph_ਸੈੱਟ, hb_ਪਹਿਲਾ)
    | hb_map (hb_ਦੂਜਾ)
    ;

    ਜੇ (!it) ਵਾਪਸ;
    + it
    | hb_map (hb_ਜੋੜੋ (ਇਹ))
    | hb_apply ([&] (ਅਚਲ Pair_ਸੈੱਟ& _) { _.collect_variation_indices (c, ਮੁੱਲ_Format_2); })
    ;
  }

  ਖਾਲੀ collect_glyphs (hb_collect_glyphs_context_t *c) ਅਚਲ
  {
    ਜੇ (unlikely (!(ਇਹ+coverage).collect_coverage (c->ਨਿਵੇਸ਼))) ਵਾਪਸ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2 = pair_ਸੈੱਟ.len;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++)
      (ਇਹ+pair_ਸੈੱਟ[i]).collect_glyphs (c, ਮੁੱਲ_Format_2);
  }

  ਖਾਲੀ collect_ਦੂਜਾ_glyphs (hb_ਸੈੱਟ_digest_t *digest) ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2 = pair_ਸੈੱਟ.len;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਗਿਣਤੀ_2; i++)
      (ਇਹ+pair_ਸੈੱਟ[i]).collect_ਦੂਜਾ_glyphs (digest, ਮੁੱਲ_Format_2);
  }

  ਅਚਲ Coverage &get_coverage () ਅਚਲ { ਵਾਪਸ ਇਹ+coverage; }

  ਬਣਤਰ external_cache_t
  {
    hb_ot_layout_mapping_cache_t coverage;
    hb_ਸੈੱਟ_digest_t pair_sets[HB_VAR_ARRAY];
  };
  ਖਾਲੀ *external_cache_create () ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2 = pair_ਸੈੱਟ.len;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਮਾਪ = ਆਕਾਰ (external_cache_t) -
		    HB_VAR_ARRAY * ਆਕਾਰ (hb_ਸੈੱਟ_digest_t) +
		    ਗਿਣਤੀ_2 * ਆਕਾਰ (hb_ਸੈੱਟ_digest_t);
    external_cache_t *cache = (external_cache_t *) hb_malloc (ਮਾਪ);
    ਜੇ (likely (cache))
    {
      cache->coverage.clear ();
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਗਿਣਤੀ_2; i++)
      {
	cache->pair_sets[i].init ();
	(ਇਹ+pair_ਸੈੱਟ[i]).collect_ਦੂਜਾ_glyphs (&cache->pair_sets[i], ਮੁੱਲ_Format_2);
      }
    }
    ਵਾਪਸ cache;
  }

  ਤਾਰਕਿਕ apply (hb_ot_apply_context_t *c, ਖਾਲੀ *external_cache) ਅਚਲ
  {
    TRACE_APPLY (ਇਹ);

    hb_ਬਫਰ_t *ਬਫਰ = c->ਬਫਰ;

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_OT_LAYOUT_LOOKUP_CACHE
    external_cache_t *cache = (external_cache_t *) external_cache;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸੂਚਕਾਂਕ_2 = (ਇਹ+coverage).get_coverage  (ਬਫਰ->cur().codepoint, cache ? &cache->coverage : ਖਾਲੀ_ਸੂਚਕ);
#ਨਹੀਂਤਾਂ
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸੂਚਕਾਂਕ_2 = (ਇਹ+coverage).get_coverage  (ਬਫਰ->cur().codepoint);
#ਜੇ_ਅੰਤ
    ਜੇ (ਸੂਚਕਾਂਕ_2 == NOT_COVERED) return_trace (ਝੂਠ);

    ਸਵੈਚਾਲਿਤ &skippy_iter = c->iter_ਨਿਵੇਸ਼;
    skippy_iter.reset_fast (ਬਫਰ->idx);
    ਬਿਨਾਨਿਸ਼ਾਨੀ unsafe_ਮੰਜ਼ਿਲ;
    ਜੇ (unlikely (!skippy_iter.next (&unsafe_ਮੰਜ਼ਿਲ)))
    {
      ਬਫਰ->unsafe_ਮੰਜ਼ਿਲ_concat (ਬਫਰ->idx, unsafe_ਮੰਜ਼ਿਲ);
      return_trace (ਝੂਠ);
    }

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_OT_LAYOUT_LOOKUP_CACHE
    ਜੇ (cache &&
	ਸੂਚਕਾਂਕ_2 < pair_ਸੈੱਟ.len &&
	!cache->pair_sets[ਸੂਚਕਾਂਕ_2].may_have (ਬਫਰ->info[skippy_iter.idx].codepoint))
    {
      ਬਫਰ->unsafe_ਮੰਜ਼ਿਲ_concat (ਬਫਰ->idx, skippy_iter.idx + 1);
      return_trace (ਝੂਠ);
    }
#ਜੇ_ਅੰਤ

    return_trace ((ਇਹ+pair_ਸੈੱਟ[ਸੂਚਕਾਂਕ_2]).apply (c, ਮੁੱਲ_Format_2, skippy_iter.idx));
  }

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);

    ਅਚਲ hb_ਸੈੱਟ_t &glyphset = *c->plan->glyphset_gsub ();
    ਅਚਲ hb_map_t &glyph_map = *c->plan->glyph_map;

    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->serializer->ਸ਼ੁਰੂ_embed (*ਇਹ);
    ਜੇ (unlikely (!c->serializer->extend_min (ਬਾਹਰਲਾ))) return_trace (ਝੂਠ);
    ਬਾਹਰਲਾ->format = format;

    hb_pair_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਬਿਨਾਨਿਸ਼ਾਨੀ> newFormats = hb_pair (ਮੁੱਲ_Format_2[0], ਮੁੱਲ_Format_2[1]);

    ਜੇ (c->plan->normalized_coords)
    {
      /* all device flags will be dropped when full instancing, no need to strip
       * hints, also do not strip emtpy cause we don't compute the new default
       * value during stripping */
      newFormats = compute_effective_ਮੁੱਲ_formats (glyphset, ਝੂਠ, ਝੂਠ, &c->plan->layout_variation_idx_delta_map);
    }
    /* do not strip hints for VF */
    ਨਹੀਂਤਾਂ ਜੇ (c->plan->flags & HB_SUBSET_FLAGS_NO_HINTING)
    {
      hb_blob_t* blob = hb_face_reference_ਸਾਰਣੀ (c->plan->ਸਰੋਤ, HB_ਟੈਗ ('f','v','a','r'));
      ਤਾਰਕਿਕ has_fvar = (blob != hb_blob_get_ਖਾਲੀ_ਥਾਵਾਂ ());
      hb_blob_destroy (blob);

      ਤਾਰਕਿਕ strip = !has_fvar;
      /* special case: strip hints when a VF has no GDEF varstore after
       * subsetting*/
      ਜੇ (has_fvar && !c->plan->has_gdef_varstore)
        strip = ਸੱਚ;
      newFormats = compute_effective_ਮੁੱਲ_formats (glyphset, strip, ਸੱਚ);
    }

    ਬਾਹਰਲਾ->ਮੁੱਲ_Format_2[0] = newFormats.ਪਹਿਲਾ;
    ਬਾਹਰਲਾ->ਮੁੱਲ_Format_2[1] = newFormats.second;

    hb_sorted_ਸਦਿਸ਼_t<hb_codepoint_t> new_coverage;

    + hb_zip (ਇਹ+coverage, pair_ਸੈੱਟ)
    | hb_filter (glyphset, hb_ਪਹਿਲਾ)
    | hb_filter ([ਇਹ, c, ਬਾਹਰਲਾ] (ਅਚਲ ਕਿਸਮ_ਨਾਂ Types::ਸਾਂਚਾ ਵਿੱਥ_ਮੰਜ਼ਿਲ<Pair_ਸੈੱਟ>& _)
                 {
                   ਸਵੈਚਾਲਿਤ snap = c->serializer->snapshot ();
                   ਸਵੈਚਾਲਿਤ *o = ਬਾਹਰਲਾ->pair_ਸੈੱਟ.serialize_append (c->serializer);
                   ਜੇ (unlikely (!o)) ਵਾਪਸ ਝੂਠ;
                   ਤਾਰਕਿਕ ret = o->serialize_subset (c, _, ਇਹ, ਮੁੱਲ_Format_2, ਬਾਹਰਲਾ->ਮੁੱਲ_Format_2);
                   ਜੇ (!ret)
                   {
                     ਬਾਹਰਲਾ->pair_ਸੈੱਟ.ਕੱਢੋ ();
                     c->serializer->revert (snap);
                   }
                   ਵਾਪਸ ret;
                 },
                 hb_ਦੂਜਾ)
    | hb_map (hb_ਪਹਿਲਾ)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    ਬਾਹਰਲਾ->coverage.serialize_serialize (c->serializer, new_coverage.iter ());

    return_trace (ਤਾਰਕਿਕ (new_coverage));
  }


  hb_pair_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਬਿਨਾਨਿਸ਼ਾਨੀ> compute_effective_ਮੁੱਲ_formats (ਅਚਲ hb_ਸੈੱਟ_t& glyphset,
                                                                 ਤਾਰਕਿਕ strip_hints, ਤਾਰਕਿਕ strip_ਖਾਲੀ_ਥਾਵਾਂ,
                                                                 ਅਚਲ hb_hashmap_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, hb_pair_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਪੂਰਨ>> *varidx_delta_map = ਖਾਲੀ_ਸੂਚਕ) ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ format1 = 0;
    ਬਿਨਾਨਿਸ਼ਾਨੀ format2 = 0;
    ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ & _ :
	  + hb_zip (ਇਹ+coverage, pair_ਸੈੱਟ)
	  | hb_filter (glyphset, hb_ਪਹਿਲਾ)
	  | hb_map (hb_ਦੂਜਾ)
	)
    {
      ਅਚਲ Pair_ਸੈੱਟ& ਸੈੱਟ = (ਇਹ + _);
      ਸਵੈਚਾਲਿਤ formats = ਸੈੱਟ.get_ਮੁੱਲਾਂ_view (ਮੁੱਲ_Format_2).compute_effective_ਮੁੱਲ_formats (
          glyphset, ਮੁੱਲ_Format_2, strip_hints, strip_ਖਾਲੀ_ਥਾਵਾਂ, varidx_delta_map);
      format1 |= formats.ਪਹਿਲਾ;
      format2 |= formats.second;

      ਜੇ (!varidx_delta_map &&
          format1 == ਮੁੱਲ_Format_2[0] && format2 == ਮੁੱਲ_Format_2[1])
        ਰੋਕੋ;
    }

    ਵਾਪਸ hb_pair (format1, format2);
  }
};


}
}
}

#ਜੇ_ਅੰਤ  // OT_LAYOUT_GPOS_PAIRPOSFORMAT1_HH
