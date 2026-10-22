#ifndef OT_LAYOUT_GPOS_PAIRPOSFORMAT1_HH
#define OT_LAYOUT_GPOS_PAIRPOSFORMAT1_HH

#अन्तर्भाव "PairSet.hh"

नामाकाश OT {
नामाकाश Layout {
नामाकाश GPOS_impl {


template <typename Types>
संरचना PairPosFormat1_3
{
  using PairSet = GPOS_impl::PairSet<Types>;
  using Pair_मानम्_Record = GPOS_impl::Pair_मानम्_Record<Types>;

  रक्षित:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  typename Types::template LOffsetTo<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of subtable */
  मानम्_Format   मानम्_Format_2[2];         /* [0] Defines the types of data in
                                         * ValueRecord1--for the first glyph
                                         * in the pair--may be zero (0) */
                                        /* [1] Defines the types of data in
                                         * ValueRecord2--for the second glyph
                                         * in the pair--may be zero (0) */
  typename Types::template ArrayOf<typename Types::template OffsetTo<PairSet>>
                pairSet;                /* Array of PairSet tables ordered by
                                         * Coverage Index */
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (2 + Types::LOffset::static_size + 2 * मानम्_Format::static_size + Types::HBUINT::static_size, pairSet);

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);

    जइ (!c->check_struct (एतत्)) return_trace (मिच्छ);
    hb_barrier ();

    unsigned पूर्णाङ्क len1 = मानम्_Format_2[0].get_len ();
    unsigned पूर्णाङ्क len2 = मानम्_Format_2[1].get_len ();
    typename PairSet::sanitize_closure_t closure =
    {
      मानम्_Format_2,
      len1,
      PairSet::get_size (len1, len2)
    };

    return_trace (coverage.sanitize (c, एतत्) && pairSet.sanitize (c, एतत्, &closure));
  }

  तार्किक intersects (ध्रुव hb_set_t *glyphs) ध्रुव
  {
    auto &cov = एतत्+coverage;

    जइ (pairSet.len > glyphs->get_population () * hb_bit_storage ((unsigned) pairSet.len))
    {
      प्रति (hb_codepoint_t g : glyphs->iter ())
      {
	unsigned i = cov.get_coverage (g);
	जइ ((एतत्+pairSet[i]).intersects (glyphs, मानम्_Format_2))
	  प्रत्यागम सच्च;
      }
      प्रत्यागम मिच्छ;
    }

    प्रत्यागम
    + hb_zip (cov, pairSet)
    | hb_filter (*glyphs, hb_first)
    | hb_map (hb_second)
    | hb_map ([glyphs, एतत्] (ध्रुव typename Types::template OffsetTo<PairSet> &_)
              { प्रत्यागम (एतत्+_).intersects (glyphs, मानम्_Format_2); })
    | hb_any
    ;
  }

  शून्य closure_lookups (hb_closure_lookups_context_t *c) ध्रुव {}
  शून्य collect_variation_indices (hb_collect_variation_indices_context_t *c) ध्रुव
  {
    जइ ((!मानम्_Format_2[0].has_device ()) && (!मानम्_Format_2[1].has_device ())) प्रत्यागम;

    auto it =
    + hb_zip (एतत्+coverage, pairSet)
    | hb_filter (c->glyph_set, hb_first)
    | hb_map (hb_second)
    ;

    जइ (!it) प्रत्यागम;
    + it
    | hb_map (hb_add (एतत्))
    | hb_apply ([&] (ध्रुव PairSet& _) { _.collect_variation_indices (c, मानम्_Format_2); })
    ;
  }

  शून्य collect_glyphs (hb_collect_glyphs_context_t *c) ध्रुव
  {
    जइ (unlikely (!(एतत्+coverage).collect_coverage (c->निवेशः))) प्रत्यागम;
    unsigned पूर्णाङ्क गणनम् = pairSet.len;
    प्रति (unsigned पूर्णाङ्क i = 0; i < गणनम्; i++)
      (एतत्+pairSet[i]).collect_glyphs (c, मानम्_Format_2);
  }

  शून्य collect_second_glyphs (hb_set_digest_t *digest) ध्रुव
  {
    unsigned गणनम् = pairSet.len;
    प्रति (unsigned i = 0; i < गणनम्; i++)
      (एतत्+pairSet[i]).collect_second_glyphs (digest, मानम्_Format_2);
  }

  ध्रुव Coverage &get_coverage () ध्रुव { प्रत्यागम एतत्+coverage; }

  संरचना external_cache_t
  {
    hb_ot_layout_mapping_cache_t coverage;
    hb_set_digest_t pair_sets[HB_VAR_ARRAY];
  };
  शून्य *external_cache_create () ध्रुव
  {
    unsigned गणनम् = pairSet.len;
    unsigned size = परिमाण (external_cache_t) -
		    HB_VAR_ARRAY * परिमाण (hb_set_digest_t) +
		    गणनम् * परिमाण (hb_set_digest_t);
    external_cache_t *cache = (external_cache_t *) hb_malloc (size);
    जइ (likely (cache))
    {
      cache->coverage.clear ();
      प्रति (unsigned i = 0; i < गणनम्; i++)
      {
	cache->pair_sets[i].init ();
	(एतत्+pairSet[i]).collect_second_glyphs (&cache->pair_sets[i], मानम्_Format_2);
      }
    }
    प्रत्यागम cache;
  }

  तार्किक apply (hb_ot_apply_context_t *c, शून्य *external_cache) ध्रुव
  {
    TRACE_APPLY (एतत्);

    hb_buffer_t *buffer = c->buffer;

#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    external_cache_t *cache = (external_cache_t *) external_cache;
    unsigned पूर्णाङ्क index = (एतत्+coverage).get_coverage  (buffer->cur().codepoint, cache ? &cache->coverage : nullptr);
#अण्णहा
    unsigned पूर्णाङ्क index = (एतत्+coverage).get_coverage  (buffer->cur().codepoint);
#endif
    जइ (index == NOT_COVERED) return_trace (मिच्छ);

    auto &skippy_iter = c->iter_निवेशः;
    skippy_iter.reset_fast (buffer->idx);
    unsigned unsafe_to;
    जइ (unlikely (!skippy_iter.next (&unsafe_to)))
    {
      buffer->unsafe_to_concat (buffer->idx, unsafe_to);
      return_trace (मिच्छ);
    }

#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    जइ (cache &&
	index < pairSet.len &&
	!cache->pair_sets[index].may_have (buffer->info[skippy_iter.idx].codepoint))
    {
      buffer->unsafe_to_concat (buffer->idx, skippy_iter.idx + 1);
      return_trace (मिच्छ);
    }
#endif

    return_trace ((एतत्+pairSet[index]).apply (c, मानम्_Format_2, skippy_iter.idx));
  }

  तार्किक subset (hb_subset_context_t *c) ध्रुव
  {
    TRACE_SUBSET (एतत्);

    ध्रुव hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ध्रुव hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*एतत्);
    जइ (unlikely (!c->serializer->extend_min (out))) return_trace (मिच्छ);
    out->format = format;

    hb_pair_t<unsigned, unsigned> newFormats = hb_pair (मानम्_Format_2[0], मानम्_Format_2[1]);

    जइ (c->plan->normalized_coords)
    {
      /* all device flags will be dropped when full instancing, no need to strip
       * hints, also do not strip emtpy cause we don't compute the new default
       * value during stripping */
      newFormats = compute_effective_मानम्_formats (glyphset, मिच्छ, मिच्छ, &c->plan->layout_variation_idx_delta_map);
    }
    /* do not strip hints for VF */
    अण्णहा जइ (c->plan->flags & HB_SUBSET_FLAGS_NO_HINTING)
    {
      hb_blob_t* blob = hb_face_reference_table (c->plan->source, HB_TAG ('f','v','a','r'));
      तार्किक has_fvar = (blob != hb_blob_get_empty ());
      hb_blob_destroy (blob);

      तार्किक strip = !has_fvar;
      /* special case: strip hints when a VF has no GDEF varstore after
       * subsetting*/
      जइ (has_fvar && !c->plan->has_gdef_varstore)
        strip = सच्च;
      newFormats = compute_effective_मानम्_formats (glyphset, strip, सच्च);
    }

    out->मानम्_Format_2[0] = newFormats.first;
    out->मानम्_Format_2[1] = newFormats.second;

    hb_sorted_vector_t<hb_codepoint_t> new_coverage;

    + hb_zip (एतत्+coverage, pairSet)
    | hb_filter (glyphset, hb_first)
    | hb_filter ([एतत्, c, out] (ध्रुव typename Types::template OffsetTo<PairSet>& _)
                 {
                   auto snap = c->serializer->snapshot ();
                   auto *o = out->pairSet.serialize_append (c->serializer);
                   जइ (unlikely (!o)) प्रत्यागम मिच्छ;
                   तार्किक ret = o->serialize_subset (c, _, एतत्, मानम्_Format_2, out->मानम्_Format_2);
                   जइ (!ret)
                   {
                     out->pairSet.निष्कासय ();
                     c->serializer->revert (snap);
                   }
                   प्रत्यागम ret;
                 },
                 hb_second)
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    out->coverage.serialize_serialize (c->serializer, new_coverage.iter ());

    return_trace (तार्किक (new_coverage));
  }


  hb_pair_t<unsigned, unsigned> compute_effective_मानम्_formats (ध्रुव hb_set_t& glyphset,
                                                                 तार्किक strip_hints, तार्किक strip_empty,
                                                                 ध्रुव hb_hashmap_t<unsigned, hb_pair_t<unsigned, पूर्णाङ्क>> *varidx_delta_map = nullptr) ध्रुव
  {
    unsigned format1 = 0;
    unsigned format2 = 0;
    प्रति (ध्रुव auto & _ :
	  + hb_zip (एतत्+coverage, pairSet)
	  | hb_filter (glyphset, hb_first)
	  | hb_map (hb_second)
	)
    {
      ध्रुव PairSet& set = (एतत् + _);
      auto formats = set.get_मानानि_view (मानम्_Format_2).compute_effective_मानम्_formats (
          glyphset, मानम्_Format_2, strip_hints, strip_empty, varidx_delta_map);
      format1 |= formats.first;
      format2 |= formats.second;

      जइ (!varidx_delta_map &&
          format1 == मानम्_Format_2[0] && format2 == मानम्_Format_2[1])
        भङ्ग;
    }

    प्रत्यागम hb_pair (format1, format2);
  }
};


}
}
}

#endif  // OT_LAYOUT_GPOS_PAIRPOSFORMAT1_HH
