#ifndef OT_LAYOUT_GPOS_PAIRPOSFORMAT1_HH
#define OT_LAYOUT_GPOS_PAIRPOSFORMAT1_HH

#অন্তর্ভাব "PairSet.hh"

নামাকাশ OT {
নামাকাশ Layout {
নামাকাশ GPOS_impl {


template <typename Types>
কাঠামো PairPosFormat1_3
{
  using PairSet = GPOS_impl::PairSet<Types>;
  using Pair_মান_Record = GPOS_impl::Pair_মান_Record<Types>;

  সুরক্ষিত:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  typename Types::template LOffsetTo<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of subtable */
  মান_Format   মান_Format_2[2];         /* [0] Defines the types of data in
                                         * ValueRecord1--for the first glyph
                                         * in the pair--may be zero (0) */
                                        /* [1] Defines the types of data in
                                         * ValueRecord2--for the second glyph
                                         * in the pair--may be zero (0) */
  typename Types::template ArrayOf<typename Types::template OffsetTo<PairSet>>
                pairSet;                /* Array of PairSet tables ordered by
                                         * Coverage Index */
  সর্বজনীন:
  DEFINE_আকার_ARRAY (2 + Types::LOffset::static_আকার + 2 * মান_Format::static_আকার + Types::HBUINT::static_আকার, pairSet);

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);

    যদি (!c->check_struct (এই)) return_trace (মিথ্যা);
    hb_barrier ();

    unsigned পূর্ণসংখ্যা len1 = মান_Format_2[0].get_len ();
    unsigned পূর্ণসংখ্যা len2 = মান_Format_2[1].get_len ();
    typename PairSet::sanitize_closure_t closure =
    {
      মান_Format_2,
      len1,
      PairSet::get_আকার (len1, len2)
    };

    return_trace (coverage.sanitize (c, এই) && pairSet.sanitize (c, এই, &closure));
  }

  যৌক্তিক intersects (ধ্রুবক hb_set_t *glyphs) ধ্রুবক
  {
    auto &cov = এই+coverage;

    যদি (pairSet.len > glyphs->get_population () * hb_bit_storage ((unsigned) pairSet.len))
    {
      জন্য (hb_codepoint_t g : glyphs->iter ())
      {
	unsigned i = cov.get_coverage (g);
	যদি ((এই+pairSet[i]).intersects (glyphs, মান_Format_2))
	  ফেরত সত্য;
      }
      ফেরত মিথ্যা;
    }

    ফেরত
    + hb_zip (cov, pairSet)
    | hb_filter (*glyphs, hb_first)
    | hb_map (hb_second)
    | hb_map ([glyphs, এই] (ধ্রুবক typename Types::template OffsetTo<PairSet> &_)
              { ফেরত (এই+_).intersects (glyphs, মান_Format_2); })
    | hb_any
    ;
  }

  শূন্য closure_lookups (hb_closure_lookups_context_t *c) ধ্রুবক {}
  শূন্য collect_variation_indices (hb_collect_variation_indices_context_t *c) ধ্রুবক
  {
    যদি ((!মান_Format_2[0].has_device ()) && (!মান_Format_2[1].has_device ())) ফেরত;

    auto it =
    + hb_zip (এই+coverage, pairSet)
    | hb_filter (c->glyph_set, hb_first)
    | hb_map (hb_second)
    ;

    যদি (!it) ফেরত;
    + it
    | hb_map (hb_add (এই))
    | hb_apply ([&] (ধ্রুবক PairSet& _) { _.collect_variation_indices (c, মান_Format_2); })
    ;
  }

  শূন্য collect_glyphs (hb_collect_glyphs_context_t *c) ধ্রুবক
  {
    যদি (unlikely (!(এই+coverage).collect_coverage (c->প্রবেশ))) ফেরত;
    unsigned পূর্ণসংখ্যা গুনতি = pairSet.len;
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < গুনতি; i++)
      (এই+pairSet[i]).collect_glyphs (c, মান_Format_2);
  }

  শূন্য collect_second_glyphs (hb_set_digest_t *digest) ধ্রুবক
  {
    unsigned গুনতি = pairSet.len;
    জন্য (unsigned i = 0; i < গুনতি; i++)
      (এই+pairSet[i]).collect_second_glyphs (digest, মান_Format_2);
  }

  ধ্রুবক Coverage &get_coverage () ধ্রুবক { ফেরত এই+coverage; }

  কাঠামো external_cache_t
  {
    hb_ot_layout_mapping_cache_t coverage;
    hb_set_digest_t pair_sets[HB_VAR_ARRAY];
  };
  শূন্য *external_cache_create () ধ্রুবক
  {
    unsigned গুনতি = pairSet.len;
    unsigned আকার_2 = আকার (external_cache_t) -
		    HB_VAR_ARRAY * আকার (hb_set_digest_t) +
		    গুনতি * আকার (hb_set_digest_t);
    external_cache_t *cache = (external_cache_t *) hb_malloc (আকার_2);
    যদি (likely (cache))
    {
      cache->coverage.clear ();
      জন্য (unsigned i = 0; i < গুনতি; i++)
      {
	cache->pair_sets[i].init ();
	(এই+pairSet[i]).collect_second_glyphs (&cache->pair_sets[i], মান_Format_2);
      }
    }
    ফেরত cache;
  }

  যৌক্তিক apply (hb_ot_apply_context_t *c, শূন্য *external_cache) ধ্রুবক
  {
    TRACE_APPLY (এই);

    hb_বাফার_t *বাফার = c->বাফার;

#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    external_cache_t *cache = (external_cache_t *) external_cache;
    unsigned পূর্ণসংখ্যা সূচক_2 = (এই+coverage).get_coverage  (বাফার->cur().codepoint, cache ? &cache->coverage : nullptr);
#অন্যথা
    unsigned পূর্ণসংখ্যা সূচক_2 = (এই+coverage).get_coverage  (বাফার->cur().codepoint);
#endif
    যদি (সূচক_2 == NOT_COVERED) return_trace (মিথ্যা);

    auto &skippy_iter = c->iter_প্রবেশ;
    skippy_iter.reset_fast (বাফার->idx);
    unsigned unsafe_to;
    যদি (unlikely (!skippy_iter.next (&unsafe_to)))
    {
      বাফার->unsafe_to_concat (বাফার->idx, unsafe_to);
      return_trace (মিথ্যা);
    }

#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    যদি (cache &&
	সূচক_2 < pairSet.len &&
	!cache->pair_sets[সূচক_2].may_have (বাফার->info[skippy_iter.idx].codepoint))
    {
      বাফার->unsafe_to_concat (বাফার->idx, skippy_iter.idx + 1);
      return_trace (মিথ্যা);
    }
#endif

    return_trace ((এই+pairSet[সূচক_2]).apply (c, মান_Format_2, skippy_iter.idx));
  }

  যৌক্তিক subset (hb_subset_context_t *c) ধ্রুবক
  {
    TRACE_SUBSET (এই);

    ধ্রুবক hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ধ্রুবক hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*এই);
    যদি (unlikely (!c->serializer->extend_min (out))) return_trace (মিথ্যা);
    out->format = format;

    hb_pair_t<unsigned, unsigned> newFormats = hb_pair (মান_Format_2[0], মান_Format_2[1]);

    যদি (c->plan->normalized_coords)
    {
      /* all device flags will be dropped when full instancing, no need to strip
       * hints, also do not strip emtpy cause we don't compute the new default
       * value during stripping */
      newFormats = compute_effective_মান_formats (glyphset, মিথ্যা, মিথ্যা, &c->plan->layout_variation_idx_delta_map);
    }
    /* do not strip hints for VF */
    অন্যথা যদি (c->plan->flags & HB_SUBSET_FLAGS_NO_HINTING)
    {
      hb_blob_t* blob = hb_face_reference_table (c->plan->source, HB_TAG ('f','v','a','r'));
      যৌক্তিক has_fvar = (blob != hb_blob_get_empty ());
      hb_blob_destroy (blob);

      যৌক্তিক strip = !has_fvar;
      /* special case: strip hints when a VF has no GDEF varstore after
       * subsetting*/
      যদি (has_fvar && !c->plan->has_gdef_varstore)
        strip = সত্য;
      newFormats = compute_effective_মান_formats (glyphset, strip, সত্য);
    }

    out->মান_Format_2[0] = newFormats.first;
    out->মান_Format_2[1] = newFormats.second;

    hb_sorted_vector_t<hb_codepoint_t> new_coverage;

    + hb_zip (এই+coverage, pairSet)
    | hb_filter (glyphset, hb_first)
    | hb_filter ([এই, c, out] (ধ্রুবক typename Types::template OffsetTo<PairSet>& _)
                 {
                   auto snap = c->serializer->snapshot ();
                   auto *o = out->pairSet.serialize_append (c->serializer);
                   যদি (unlikely (!o)) ফেরত মিথ্যা;
                   যৌক্তিক ret = o->serialize_subset (c, _, এই, মান_Format_2, out->মান_Format_2);
                   যদি (!ret)
                   {
                     out->pairSet.বের_করো ();
                     c->serializer->revert (snap);
                   }
                   ফেরত ret;
                 },
                 hb_second)
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    out->coverage.serialize_serialize (c->serializer, new_coverage.iter ());

    return_trace (যৌক্তিক (new_coverage));
  }


  hb_pair_t<unsigned, unsigned> compute_effective_মান_formats (ধ্রুবক hb_set_t& glyphset,
                                                                 যৌক্তিক strip_hints, যৌক্তিক strip_empty,
                                                                 ধ্রুবক hb_hashmap_t<unsigned, hb_pair_t<unsigned, পূর্ণসংখ্যা>> *varidx_delta_map = nullptr) ধ্রুবক
  {
    unsigned format1 = 0;
    unsigned format2 = 0;
    জন্য (ধ্রুবক auto & _ :
	  + hb_zip (এই+coverage, pairSet)
	  | hb_filter (glyphset, hb_first)
	  | hb_map (hb_second)
	)
    {
      ধ্রুবক PairSet& set = (এই + _);
      auto formats = set.get_মানসমূহ_view (মান_Format_2).compute_effective_মান_formats (
          glyphset, মান_Format_2, strip_hints, strip_empty, varidx_delta_map);
      format1 |= formats.first;
      format2 |= formats.second;

      যদি (!varidx_delta_map &&
          format1 == মান_Format_2[0] && format2 == মান_Format_2[1])
        বিরতি;
    }

    ফেরত hb_pair (format1, format2);
  }
};


}
}
}

#endif  // OT_LAYOUT_GPOS_PAIRPOSFORMAT1_HH
