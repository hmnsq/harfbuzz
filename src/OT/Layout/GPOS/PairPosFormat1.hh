#ifndef OT_LAYOUT_GPOS_PAIRPOSFORMAT1_HH
#define OT_LAYOUT_GPOS_PAIRPOSFORMAT1_HH

#ಅನ್ತರ್ಭಾವ "PairSet.hh"

ನಾಮಾಕಾಶ OT {
ನಾಮಾಕಾಶ Layout {
ನಾಮಾಕಾಶ GPOS_impl {


template <typename Types>
ಸಂರಚನಾ PairPosFormat1_3
{
  using PairSet = GPOS_impl::PairSet<Types>;
  using Pair_ಮೌಲ್ಯ_Record = GPOS_impl::Pair_ಮೌಲ್ಯ_Record<Types>;

  ರಕ್ಷಿತ:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  typename Types::template LOffsetTo<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of subtable */
  ಮೌಲ್ಯ_Format   ಮೌಲ್ಯ_Format_2[2];         /* [0] Defines the types of data in
                                         * ValueRecord1--for the first glyph
                                         * in the pair--may be zero (0) */
                                        /* [1] Defines the types of data in
                                         * ValueRecord2--for the second glyph
                                         * in the pair--may be zero (0) */
  typename Types::template ArrayOf<typename Types::template OffsetTo<PairSet>>
                pairSet;                /* Array of PairSet tables ordered by
                                         * Coverage Index */
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_ARRAY (2 + Types::LOffset::static_size + 2 * ಮೌಲ್ಯ_Format::static_size + Types::HBUINT::static_size, pairSet);

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);

    ಯದಿ (!c->check_struct (ಏತತ್)) return_trace (ಅಸತ್ಯ);
    hb_barrier ();

    unsigned ಪೂರ್ಣಾಙ್ಕ len1 = ಮೌಲ್ಯ_Format_2[0].get_len ();
    unsigned ಪೂರ್ಣಾಙ್ಕ len2 = ಮೌಲ್ಯ_Format_2[1].get_len ();
    typename PairSet::sanitize_closure_t closure =
    {
      ಮೌಲ್ಯ_Format_2,
      len1,
      PairSet::get_size (len1, len2)
    };

    return_trace (coverage.sanitize (c, ಏತತ್) && pairSet.sanitize (c, ಏತತ್, &closure));
  }

  ತಾರ್ಕಿಕ intersects (ಧ್ರುವ hb_set_t *glyphs) ಧ್ರುವ
  {
    auto &cov = ಏತತ್+coverage;

    ಯದಿ (pairSet.len > glyphs->get_population () * hb_bit_storage ((unsigned) pairSet.len))
    {
      ಪ್ರತಿ (hb_codepoint_t g : glyphs->iter ())
      {
	unsigned i = cov.get_coverage (g);
	ಯದಿ ((ಏತತ್+pairSet[i]).intersects (glyphs, ಮೌಲ್ಯ_Format_2))
	  ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
      }
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }

    ಪ್ರತ್ಯಾಗಮ
    + hb_zip (cov, pairSet)
    | hb_filter (*glyphs, hb_first)
    | hb_map (hb_second)
    | hb_map ([glyphs, ಏತತ್] (ಧ್ರುವ typename Types::template OffsetTo<PairSet> &_)
              { ಪ್ರತ್ಯಾಗಮ (ಏತತ್+_).intersects (glyphs, ಮೌಲ್ಯ_Format_2); })
    | hb_any
    ;
  }

  ಶೂನ್ಯ closure_lookups (hb_closure_lookups_context_t *c) ಧ್ರುವ {}
  ಶೂನ್ಯ collect_variation_indices (hb_collect_variation_indices_context_t *c) ಧ್ರುವ
  {
    ಯದಿ ((!ಮೌಲ್ಯ_Format_2[0].has_device ()) && (!ಮೌಲ್ಯ_Format_2[1].has_device ())) ಪ್ರತ್ಯಾಗಮ;

    auto it =
    + hb_zip (ಏತತ್+coverage, pairSet)
    | hb_filter (c->glyph_set, hb_first)
    | hb_map (hb_second)
    ;

    ಯದಿ (!it) ಪ್ರತ್ಯಾಗಮ;
    + it
    | hb_map (hb_add (ಏತತ್))
    | hb_apply ([&] (ಧ್ರುವ PairSet& _) { _.collect_variation_indices (c, ಮೌಲ್ಯ_Format_2); })
    ;
  }

  ಶೂನ್ಯ collect_glyphs (hb_collect_glyphs_context_t *c) ಧ್ರುವ
  {
    ಯದಿ (unlikely (!(ಏತತ್+coverage).collect_coverage (c->ಒಳಹರಿವು))) ಪ್ರತ್ಯಾಗಮ;
    unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = pairSet.len;
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಎಣಿಕೆ; i++)
      (ಏತತ್+pairSet[i]).collect_glyphs (c, ಮೌಲ್ಯ_Format_2);
  }

  ಶೂನ್ಯ collect_second_glyphs (hb_set_digest_t *digest) ಧ್ರುವ
  {
    unsigned ಎಣಿಕೆ = pairSet.len;
    ಪ್ರತಿ (unsigned i = 0; i < ಎಣಿಕೆ; i++)
      (ಏತತ್+pairSet[i]).collect_second_glyphs (digest, ಮೌಲ್ಯ_Format_2);
  }

  ಧ್ರುವ Coverage &get_coverage () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ ಏತತ್+coverage; }

  ಸಂರಚನಾ external_cache_t
  {
    hb_ot_layout_mapping_cache_t coverage;
    hb_set_digest_t pair_sets[HB_VAR_ARRAY];
  };
  ಶೂನ್ಯ *external_cache_create () ಧ್ರುವ
  {
    unsigned ಎಣಿಕೆ = pairSet.len;
    unsigned size = ಪರಿಮಾಣ (external_cache_t) -
		    HB_VAR_ARRAY * ಪರಿಮಾಣ (hb_set_digest_t) +
		    ಎಣಿಕೆ * ಪರಿಮಾಣ (hb_set_digest_t);
    external_cache_t *cache = (external_cache_t *) hb_malloc (size);
    ಯದಿ (likely (cache))
    {
      cache->coverage.clear ();
      ಪ್ರತಿ (unsigned i = 0; i < ಎಣಿಕೆ; i++)
      {
	cache->pair_sets[i].init ();
	(ಏತತ್+pairSet[i]).collect_second_glyphs (&cache->pair_sets[i], ಮೌಲ್ಯ_Format_2);
      }
    }
    ಪ್ರತ್ಯಾಗಮ cache;
  }

  ತಾರ್ಕಿಕ apply (hb_ot_apply_context_t *c, ಶೂನ್ಯ *external_cache) ಧ್ರುವ
  {
    TRACE_APPLY (ಏತತ್);

    hb_buffer_t *buffer = c->buffer;

#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    external_cache_t *cache = (external_cache_t *) external_cache;
    unsigned ಪೂರ್ಣಾಙ್ಕ index = (ಏತತ್+coverage).get_coverage  (buffer->cur().codepoint, cache ? &cache->coverage : nullptr);
#ಅನ್ಯಥಾ
    unsigned ಪೂರ್ಣಾಙ್ಕ index = (ಏತತ್+coverage).get_coverage  (buffer->cur().codepoint);
#endif
    ಯದಿ (index == NOT_COVERED) return_trace (ಅಸತ್ಯ);

    auto &skippy_iter = c->iter_ಒಳಹರಿವು;
    skippy_iter.reset_fast (buffer->idx);
    unsigned unsafe_to;
    ಯದಿ (unlikely (!skippy_iter.next (&unsafe_to)))
    {
      buffer->unsafe_to_concat (buffer->idx, unsafe_to);
      return_trace (ಅಸತ್ಯ);
    }

#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    ಯದಿ (cache &&
	index < pairSet.len &&
	!cache->pair_sets[index].may_have (buffer->info[skippy_iter.idx].codepoint))
    {
      buffer->unsafe_to_concat (buffer->idx, skippy_iter.idx + 1);
      return_trace (ಅಸತ್ಯ);
    }
#endif

    return_trace ((ಏತತ್+pairSet[index]).apply (c, ಮೌಲ್ಯ_Format_2, skippy_iter.idx));
  }

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);

    ಧ್ರುವ hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ಧ್ರುವ hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*ಏತತ್);
    ಯದಿ (unlikely (!c->serializer->extend_min (out))) return_trace (ಅಸತ್ಯ);
    out->format = format;

    hb_pair_t<unsigned, unsigned> newFormats = hb_pair (ಮೌಲ್ಯ_Format_2[0], ಮೌಲ್ಯ_Format_2[1]);

    ಯದಿ (c->plan->normalized_coords)
    {
      /* all device flags will be dropped when full instancing, no need to strip
       * hints, also do not strip emtpy cause we don't compute the new default
       * value during stripping */
      newFormats = compute_effective_ಮೌಲ್ಯ_formats (glyphset, ಅಸತ್ಯ, ಅಸತ್ಯ, &c->plan->layout_variation_idx_delta_map);
    }
    /* do not strip hints for VF */
    ಅನ್ಯಥಾ ಯದಿ (c->plan->flags & HB_SUBSET_FLAGS_NO_HINTING)
    {
      hb_blob_t* blob = hb_face_reference_table (c->plan->source, HB_TAG ('f','v','a','r'));
      ತಾರ್ಕಿಕ has_fvar = (blob != hb_blob_get_empty ());
      hb_blob_destroy (blob);

      ತಾರ್ಕಿಕ strip = !has_fvar;
      /* special case: strip hints when a VF has no GDEF varstore after
       * subsetting*/
      ಯದಿ (has_fvar && !c->plan->has_gdef_varstore)
        strip = ಸತ್ಯ;
      newFormats = compute_effective_ಮೌಲ್ಯ_formats (glyphset, strip, ಸತ್ಯ);
    }

    out->ಮೌಲ್ಯ_Format_2[0] = newFormats.first;
    out->ಮೌಲ್ಯ_Format_2[1] = newFormats.second;

    hb_sorted_vector_t<hb_codepoint_t> new_coverage;

    + hb_zip (ಏತತ್+coverage, pairSet)
    | hb_filter (glyphset, hb_first)
    | hb_filter ([ಏತತ್, c, out] (ಧ್ರುವ typename Types::template OffsetTo<PairSet>& _)
                 {
                   auto snap = c->serializer->snapshot ();
                   auto *o = out->pairSet.serialize_append (c->serializer);
                   ಯದಿ (unlikely (!o)) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
                   ತಾರ್ಕಿಕ ret = o->serialize_subset (c, _, ಏತತ್, ಮೌಲ್ಯ_Format_2, out->ಮೌಲ್ಯ_Format_2);
                   ಯದಿ (!ret)
                   {
                     out->pairSet.ತೆಗೆ ();
                     c->serializer->revert (snap);
                   }
                   ಪ್ರತ್ಯಾಗಮ ret;
                 },
                 hb_second)
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    out->coverage.serialize_serialize (c->serializer, new_coverage.iter ());

    return_trace (ತಾರ್ಕಿಕ (new_coverage));
  }


  hb_pair_t<unsigned, unsigned> compute_effective_ಮೌಲ್ಯ_formats (ಧ್ರುವ hb_set_t& glyphset,
                                                                 ತಾರ್ಕಿಕ strip_hints, ತಾರ್ಕಿಕ strip_empty,
                                                                 ಧ್ರುವ hb_hashmap_t<unsigned, hb_pair_t<unsigned, ಪೂರ್ಣಾಙ್ಕ>> *varidx_delta_map = nullptr) ಧ್ರುವ
  {
    unsigned format1 = 0;
    unsigned format2 = 0;
    ಪ್ರತಿ (ಧ್ರುವ auto & _ :
	  + hb_zip (ಏತತ್+coverage, pairSet)
	  | hb_filter (glyphset, hb_first)
	  | hb_map (hb_second)
	)
    {
      ಧ್ರುವ PairSet& set = (ಏತತ್ + _);
      auto formats = set.get_ಮೌಲ್ಯಗಳು_view (ಮೌಲ್ಯ_Format_2).compute_effective_ಮೌಲ್ಯ_formats (
          glyphset, ಮೌಲ್ಯ_Format_2, strip_hints, strip_empty, varidx_delta_map);
      format1 |= formats.first;
      format2 |= formats.second;

      ಯದಿ (!varidx_delta_map &&
          format1 == ಮೌಲ್ಯ_Format_2[0] && format2 == ಮೌಲ್ಯ_Format_2[1])
        ಭಙ್ಗ;
    }

    ಪ್ರತ್ಯಾಗಮ hb_pair (format1, format2);
  }
};


}
}
}

#endif  // OT_LAYOUT_GPOS_PAIRPOSFORMAT1_HH
