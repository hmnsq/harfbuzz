#ifndef OT_LAYOUT_GPOS_PAIRPOSFORMAT1_HH
#define OT_LAYOUT_GPOS_PAIRPOSFORMAT1_HH

#અન્તર્ભાવ "PairSet.hh"

નામાકાશ OT {
નામાકાશ Layout {
નામાકાશ GPOS_impl {


template <typename Types>
સંરચના PairPosFormat1_3
{
  using PairSet = GPOS_impl::PairSet<Types>;
  using Pair_કિંમત_Record = GPOS_impl::Pair_કિંમત_Record<Types>;

  સુરક્ષિત:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  typename Types::template LOffsetTo<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of subtable */
  કિંમત_Format   કિંમત_Format_2[2];         /* [0] Defines the types of data in
                                         * ValueRecord1--for the first glyph
                                         * in the pair--may be zero (0) */
                                        /* [1] Defines the types of data in
                                         * ValueRecord2--for the second glyph
                                         * in the pair--may be zero (0) */
  typename Types::template ArrayOf<typename Types::template OffsetTo<PairSet>>
                pairSet;                /* Array of PairSet tables ordered by
                                         * Coverage Index */
  જાહેર:
  DEFINE_SIZE_ARRAY (2 + Types::LOffset::static_size + 2 * કિંમત_Format::static_size + Types::HBUINT::static_size, pairSet);

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);

    જો (!c->check_struct (આ)) return_trace (ખોટું);
    hb_barrier ();

    unsigned પૂર્ણાંક len1 = કિંમત_Format_2[0].get_len ();
    unsigned પૂર્ણાંક len2 = કિંમત_Format_2[1].get_len ();
    typename PairSet::sanitize_closure_t closure =
    {
      કિંમત_Format_2,
      len1,
      PairSet::get_size (len1, len2)
    };

    return_trace (coverage.sanitize (c, આ) && pairSet.sanitize (c, આ, &closure));
  }

  તાર્કિક intersects (અચલ hb_set_t *glyphs) અચલ
  {
    auto &cov = આ+coverage;

    જો (pairSet.len > glyphs->get_population () * hb_bit_storage ((unsigned) pairSet.len))
    {
      માટે (hb_codepoint_t g : glyphs->iter ())
      {
	unsigned i = cov.get_coverage (g);
	જો ((આ+pairSet[i]).intersects (glyphs, કિંમત_Format_2))
	  પાછા સાચું;
      }
      પાછા ખોટું;
    }

    પાછા
    + hb_zip (cov, pairSet)
    | hb_filter (*glyphs, hb_first)
    | hb_map (hb_second)
    | hb_map ([glyphs, આ] (અચલ typename Types::template OffsetTo<PairSet> &_)
              { પાછા (આ+_).intersects (glyphs, કિંમત_Format_2); })
    | hb_any
    ;
  }

  શૂન્ય closure_lookups (hb_closure_lookups_context_t *c) અચલ {}
  શૂન્ય collect_variation_indices (hb_collect_variation_indices_context_t *c) અચલ
  {
    જો ((!કિંમત_Format_2[0].has_device ()) && (!કિંમત_Format_2[1].has_device ())) પાછા;

    auto it =
    + hb_zip (આ+coverage, pairSet)
    | hb_filter (c->glyph_set, hb_first)
    | hb_map (hb_second)
    ;

    જો (!it) પાછા;
    + it
    | hb_map (hb_add (આ))
    | hb_apply ([&] (અચલ PairSet& _) { _.collect_variation_indices (c, કિંમત_Format_2); })
    ;
  }

  શૂન્ય collect_glyphs (hb_collect_glyphs_context_t *c) અચલ
  {
    જો (unlikely (!(આ+coverage).collect_coverage (c->આગત))) પાછા;
    unsigned પૂર્ણાંક ગણતરી = pairSet.len;
    માટે (unsigned પૂર્ણાંક i = 0; i < ગણતરી; i++)
      (આ+pairSet[i]).collect_glyphs (c, કિંમત_Format_2);
  }

  શૂન્ય collect_second_glyphs (hb_set_digest_t *digest) અચલ
  {
    unsigned ગણતરી = pairSet.len;
    માટે (unsigned i = 0; i < ગણતરી; i++)
      (આ+pairSet[i]).collect_second_glyphs (digest, કિંમત_Format_2);
  }

  અચલ Coverage &get_coverage () અચલ { પાછા આ+coverage; }

  સંરચના external_cache_t
  {
    hb_ot_layout_mapping_cache_t coverage;
    hb_set_digest_t pair_sets[HB_VAR_ARRAY];
  };
  શૂન્ય *external_cache_create () અચલ
  {
    unsigned ગણતરી = pairSet.len;
    unsigned size = કદ (external_cache_t) -
		    HB_VAR_ARRAY * કદ (hb_set_digest_t) +
		    ગણતરી * કદ (hb_set_digest_t);
    external_cache_t *cache = (external_cache_t *) hb_malloc (size);
    જો (likely (cache))
    {
      cache->coverage.clear ();
      માટે (unsigned i = 0; i < ગણતરી; i++)
      {
	cache->pair_sets[i].init ();
	(આ+pairSet[i]).collect_second_glyphs (&cache->pair_sets[i], કિંમત_Format_2);
      }
    }
    પાછા cache;
  }

  તાર્કિક apply (hb_ot_apply_context_t *c, શૂન્ય *external_cache) અચલ
  {
    TRACE_APPLY (આ);

    hb_buffer_t *buffer = c->buffer;

#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    external_cache_t *cache = (external_cache_t *) external_cache;
    unsigned પૂર્ણાંક index = (આ+coverage).get_coverage  (buffer->cur().codepoint, cache ? &cache->coverage : nullptr);
#નહીંતર
    unsigned પૂર્ણાંક index = (આ+coverage).get_coverage  (buffer->cur().codepoint);
#endif
    જો (index == NOT_COVERED) return_trace (ખોટું);

    auto &skippy_iter = c->iter_આગત;
    skippy_iter.reset_fast (buffer->idx);
    unsigned unsafe_to;
    જો (unlikely (!skippy_iter.next (&unsafe_to)))
    {
      buffer->unsafe_to_concat (buffer->idx, unsafe_to);
      return_trace (ખોટું);
    }

#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    જો (cache &&
	index < pairSet.len &&
	!cache->pair_sets[index].may_have (buffer->info[skippy_iter.idx].codepoint))
    {
      buffer->unsafe_to_concat (buffer->idx, skippy_iter.idx + 1);
      return_trace (ખોટું);
    }
#endif

    return_trace ((આ+pairSet[index]).apply (c, કિંમત_Format_2, skippy_iter.idx));
  }

  તાર્કિક subset (hb_subset_context_t *c) અચલ
  {
    TRACE_SUBSET (આ);

    અચલ hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    અચલ hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*આ);
    જો (unlikely (!c->serializer->extend_min (out))) return_trace (ખોટું);
    out->format = format;

    hb_pair_t<unsigned, unsigned> newFormats = hb_pair (કિંમત_Format_2[0], કિંમત_Format_2[1]);

    જો (c->plan->normalized_coords)
    {
      /* all device flags will be dropped when full instancing, no need to strip
       * hints, also do not strip emtpy cause we don't compute the new default
       * value during stripping */
      newFormats = compute_effective_કિંમત_formats (glyphset, ખોટું, ખોટું, &c->plan->layout_variation_idx_delta_map);
    }
    /* do not strip hints for VF */
    નહીંતર જો (c->plan->flags & HB_SUBSET_FLAGS_NO_HINTING)
    {
      hb_blob_t* blob = hb_face_reference_table (c->plan->source, HB_TAG ('f','v','a','r'));
      તાર્કિક has_fvar = (blob != hb_blob_get_empty ());
      hb_blob_destroy (blob);

      તાર્કિક strip = !has_fvar;
      /* special case: strip hints when a VF has no GDEF varstore after
       * subsetting*/
      જો (has_fvar && !c->plan->has_gdef_varstore)
        strip = સાચું;
      newFormats = compute_effective_કિંમત_formats (glyphset, strip, સાચું);
    }

    out->કિંમત_Format_2[0] = newFormats.first;
    out->કિંમત_Format_2[1] = newFormats.second;

    hb_sorted_vector_t<hb_codepoint_t> new_coverage;

    + hb_zip (આ+coverage, pairSet)
    | hb_filter (glyphset, hb_first)
    | hb_filter ([આ, c, out] (અચલ typename Types::template OffsetTo<PairSet>& _)
                 {
                   auto snap = c->serializer->snapshot ();
                   auto *o = out->pairSet.serialize_append (c->serializer);
                   જો (unlikely (!o)) પાછા ખોટું;
                   તાર્કિક ret = o->serialize_subset (c, _, આ, કિંમત_Format_2, out->કિંમત_Format_2);
                   જો (!ret)
                   {
                     out->pairSet.કાઢો ();
                     c->serializer->revert (snap);
                   }
                   પાછા ret;
                 },
                 hb_second)
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    out->coverage.serialize_serialize (c->serializer, new_coverage.iter ());

    return_trace (તાર્કિક (new_coverage));
  }


  hb_pair_t<unsigned, unsigned> compute_effective_કિંમત_formats (અચલ hb_set_t& glyphset,
                                                                 તાર્કિક strip_hints, તાર્કિક strip_empty,
                                                                 અચલ hb_hashmap_t<unsigned, hb_pair_t<unsigned, પૂર્ણાંક>> *varidx_delta_map = nullptr) અચલ
  {
    unsigned format1 = 0;
    unsigned format2 = 0;
    માટે (અચલ auto & _ :
	  + hb_zip (આ+coverage, pairSet)
	  | hb_filter (glyphset, hb_first)
	  | hb_map (hb_second)
	)
    {
      અચલ PairSet& set = (આ + _);
      auto formats = set.get_કિંમતો_view (કિંમત_Format_2).compute_effective_કિંમત_formats (
          glyphset, કિંમત_Format_2, strip_hints, strip_empty, varidx_delta_map);
      format1 |= formats.first;
      format2 |= formats.second;

      જો (!varidx_delta_map &&
          format1 == કિંમત_Format_2[0] && format2 == કિંમત_Format_2[1])
        તોડો;
    }

    પાછા hb_pair (format1, format2);
  }
};


}
}
}

#endif  // OT_LAYOUT_GPOS_PAIRPOSFORMAT1_HH
