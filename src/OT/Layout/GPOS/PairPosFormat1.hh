#ifndef OT_LAYOUT_GPOS_PAIRPOSFORMAT1_HH
#define OT_LAYOUT_GPOS_PAIRPOSFORMAT1_HH

#അന്തര്ഭാവ "PairSet.hh"

നാമാകാശ OT {
നാമാകാശ Layout {
നാമാകാശ GPOS_impl {


template <typename Types>
ഘടന PairPosFormat1_3
{
  using PairSet = GPOS_impl::PairSet<Types>;
  using Pair_മൂല്യം_Record = GPOS_impl::Pair_മൂല്യം_Record<Types>;

  സംരക്ഷിത:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  typename Types::template LOffsetTo<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of subtable */
  മൂല്യം_Format   മൂല്യം_Format_2[2];         /* [0] Defines the types of data in
                                         * ValueRecord1--for the first glyph
                                         * in the pair--may be zero (0) */
                                        /* [1] Defines the types of data in
                                         * ValueRecord2--for the second glyph
                                         * in the pair--may be zero (0) */
  typename Types::template ArrayOf<typename Types::template OffsetTo<PairSet>>
                pairSet;                /* Array of PairSet tables ordered by
                                         * Coverage Index */
  പൊതു:
  DEFINE_SIZE_ARRAY (2 + Types::LOffset::static_size + 2 * മൂല്യം_Format::static_size + Types::HBUINT::static_size, pairSet);

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);

    എങ്കിൽ (!c->check_struct (ഇത്)) return_trace (അസത്യം);
    hb_barrier ();

    unsigned പൂർണ്ണസംഖ്യ len1 = മൂല്യം_Format_2[0].get_len ();
    unsigned പൂർണ്ണസംഖ്യ len2 = മൂല്യം_Format_2[1].get_len ();
    typename PairSet::sanitize_closure_t closure =
    {
      മൂല്യം_Format_2,
      len1,
      PairSet::get_size (len1, len2)
    };

    return_trace (coverage.sanitize (c, ഇത്) && pairSet.sanitize (c, ഇത്, &closure));
  }

  തർക്കം intersects (സ്ഥിരം hb_set_t *glyphs) സ്ഥിരം
  {
    auto &cov = ഇത്+coverage;

    എങ്കിൽ (pairSet.len > glyphs->get_population () * hb_bit_storage ((unsigned) pairSet.len))
    {
      വേണ്ടി (hb_codepoint_t g : glyphs->iter ())
      {
	unsigned i = cov.get_coverage (g);
	എങ്കിൽ ((ഇത്+pairSet[i]).intersects (glyphs, മൂല്യം_Format_2))
	  തിരികെ സത്യം;
      }
      തിരികെ അസത്യം;
    }

    തിരികെ
    + hb_zip (cov, pairSet)
    | hb_filter (*glyphs, hb_first)
    | hb_map (hb_second)
    | hb_map ([glyphs, ഇത്] (സ്ഥിരം typename Types::template OffsetTo<PairSet> &_)
              { തിരികെ (ഇത്+_).intersects (glyphs, മൂല്യം_Format_2); })
    | hb_any
    ;
  }

  ശൂന്യം closure_lookups (hb_closure_lookups_context_t *c) സ്ഥിരം {}
  ശൂന്യം collect_variation_indices (hb_collect_variation_indices_context_t *c) സ്ഥിരം
  {
    എങ്കിൽ ((!മൂല്യം_Format_2[0].has_device ()) && (!മൂല്യം_Format_2[1].has_device ())) തിരികെ;

    auto it =
    + hb_zip (ഇത്+coverage, pairSet)
    | hb_filter (c->glyph_set, hb_first)
    | hb_map (hb_second)
    ;

    എങ്കിൽ (!it) തിരികെ;
    + it
    | hb_map (hb_add (ഇത്))
    | hb_apply ([&] (സ്ഥിരം PairSet& _) { _.collect_variation_indices (c, മൂല്യം_Format_2); })
    ;
  }

  ശൂന്യം collect_glyphs (hb_collect_glyphs_context_t *c) സ്ഥിരം
  {
    എങ്കിൽ (unlikely (!(ഇത്+coverage).collect_coverage (c->നിവേശം))) തിരികെ;
    unsigned പൂർണ്ണസംഖ്യ എണ്ണം = pairSet.len;
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < എണ്ണം; i++)
      (ഇത്+pairSet[i]).collect_glyphs (c, മൂല്യം_Format_2);
  }

  ശൂന്യം collect_second_glyphs (hb_set_digest_t *digest) സ്ഥിരം
  {
    unsigned എണ്ണം = pairSet.len;
    വേണ്ടി (unsigned i = 0; i < എണ്ണം; i++)
      (ഇത്+pairSet[i]).collect_second_glyphs (digest, മൂല്യം_Format_2);
  }

  സ്ഥിരം Coverage &get_coverage () സ്ഥിരം { തിരികെ ഇത്+coverage; }

  ഘടന external_cache_t
  {
    hb_ot_layout_mapping_cache_t coverage;
    hb_set_digest_t pair_sets[HB_VAR_ARRAY];
  };
  ശൂന്യം *external_cache_create () സ്ഥിരം
  {
    unsigned എണ്ണം = pairSet.len;
    unsigned size = വലുപ്പം (external_cache_t) -
		    HB_VAR_ARRAY * വലുപ്പം (hb_set_digest_t) +
		    എണ്ണം * വലുപ്പം (hb_set_digest_t);
    external_cache_t *cache = (external_cache_t *) hb_malloc (size);
    എങ്കിൽ (likely (cache))
    {
      cache->coverage.clear ();
      വേണ്ടി (unsigned i = 0; i < എണ്ണം; i++)
      {
	cache->pair_sets[i].init ();
	(ഇത്+pairSet[i]).collect_second_glyphs (&cache->pair_sets[i], മൂല്യം_Format_2);
      }
    }
    തിരികെ cache;
  }

  തർക്കം apply (hb_ot_apply_context_t *c, ശൂന്യം *external_cache) സ്ഥിരം
  {
    TRACE_APPLY (ഇത്);

    hb_buffer_t *buffer = c->buffer;

#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    external_cache_t *cache = (external_cache_t *) external_cache;
    unsigned പൂർണ്ണസംഖ്യ index = (ഇത്+coverage).get_coverage  (buffer->cur().codepoint, cache ? &cache->coverage : nullptr);
#അല്ലെങ്കിൽ
    unsigned പൂർണ്ണസംഖ്യ index = (ഇത്+coverage).get_coverage  (buffer->cur().codepoint);
#endif
    എങ്കിൽ (index == NOT_COVERED) return_trace (അസത്യം);

    auto &skippy_iter = c->iter_നിവേശം;
    skippy_iter.reset_fast (buffer->idx);
    unsigned unsafe_to;
    എങ്കിൽ (unlikely (!skippy_iter.next (&unsafe_to)))
    {
      buffer->unsafe_to_concat (buffer->idx, unsafe_to);
      return_trace (അസത്യം);
    }

#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    എങ്കിൽ (cache &&
	index < pairSet.len &&
	!cache->pair_sets[index].may_have (buffer->info[skippy_iter.idx].codepoint))
    {
      buffer->unsafe_to_concat (buffer->idx, skippy_iter.idx + 1);
      return_trace (അസത്യം);
    }
#endif

    return_trace ((ഇത്+pairSet[index]).apply (c, മൂല്യം_Format_2, skippy_iter.idx));
  }

  തർക്കം subset (hb_subset_context_t *c) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);

    സ്ഥിരം hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    സ്ഥിരം hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*ഇത്);
    എങ്കിൽ (unlikely (!c->serializer->extend_min (out))) return_trace (അസത്യം);
    out->format = format;

    hb_pair_t<unsigned, unsigned> newFormats = hb_pair (മൂല്യം_Format_2[0], മൂല്യം_Format_2[1]);

    എങ്കിൽ (c->plan->normalized_coords)
    {
      /* all device flags will be dropped when full instancing, no need to strip
       * hints, also do not strip emtpy cause we don't compute the new default
       * value during stripping */
      newFormats = compute_effective_മൂല്യം_formats (glyphset, അസത്യം, അസത്യം, &c->plan->layout_variation_idx_delta_map);
    }
    /* do not strip hints for VF */
    അല്ലെങ്കിൽ എങ്കിൽ (c->plan->flags & HB_SUBSET_FLAGS_NO_HINTING)
    {
      hb_blob_t* blob = hb_face_reference_table (c->plan->source, HB_TAG ('f','v','a','r'));
      തർക്കം has_fvar = (blob != hb_blob_get_empty ());
      hb_blob_destroy (blob);

      തർക്കം strip = !has_fvar;
      /* special case: strip hints when a VF has no GDEF varstore after
       * subsetting*/
      എങ്കിൽ (has_fvar && !c->plan->has_gdef_varstore)
        strip = സത്യം;
      newFormats = compute_effective_മൂല്യം_formats (glyphset, strip, സത്യം);
    }

    out->മൂല്യം_Format_2[0] = newFormats.first;
    out->മൂല്യം_Format_2[1] = newFormats.second;

    hb_sorted_vector_t<hb_codepoint_t> new_coverage;

    + hb_zip (ഇത്+coverage, pairSet)
    | hb_filter (glyphset, hb_first)
    | hb_filter ([ഇത്, c, out] (സ്ഥിരം typename Types::template OffsetTo<PairSet>& _)
                 {
                   auto snap = c->serializer->snapshot ();
                   auto *o = out->pairSet.serialize_append (c->serializer);
                   എങ്കിൽ (unlikely (!o)) തിരികെ അസത്യം;
                   തർക്കം ret = o->serialize_subset (c, _, ഇത്, മൂല്യം_Format_2, out->മൂല്യം_Format_2);
                   എങ്കിൽ (!ret)
                   {
                     out->pairSet.എടുക്കുക ();
                     c->serializer->revert (snap);
                   }
                   തിരികെ ret;
                 },
                 hb_second)
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    out->coverage.serialize_serialize (c->serializer, new_coverage.iter ());

    return_trace (തർക്കം (new_coverage));
  }


  hb_pair_t<unsigned, unsigned> compute_effective_മൂല്യം_formats (സ്ഥിരം hb_set_t& glyphset,
                                                                 തർക്കം strip_hints, തർക്കം strip_empty,
                                                                 സ്ഥിരം hb_hashmap_t<unsigned, hb_pair_t<unsigned, പൂർണ്ണസംഖ്യ>> *varidx_delta_map = nullptr) സ്ഥിരം
  {
    unsigned format1 = 0;
    unsigned format2 = 0;
    വേണ്ടി (സ്ഥിരം auto & _ :
	  + hb_zip (ഇത്+coverage, pairSet)
	  | hb_filter (glyphset, hb_first)
	  | hb_map (hb_second)
	)
    {
      സ്ഥിരം PairSet& set = (ഇത് + _);
      auto formats = set.get_മൂല്യങ്ങൾ_view (മൂല്യം_Format_2).compute_effective_മൂല്യം_formats (
          glyphset, മൂല്യം_Format_2, strip_hints, strip_empty, varidx_delta_map);
      format1 |= formats.first;
      format2 |= formats.second;

      എങ്കിൽ (!varidx_delta_map &&
          format1 == മൂല്യം_Format_2[0] && format2 == മൂല്യം_Format_2[1])
        നിർത്തുക;
    }

    തിരികെ hb_pair (format1, format2);
  }
};


}
}
}

#endif  // OT_LAYOUT_GPOS_PAIRPOSFORMAT1_HH
