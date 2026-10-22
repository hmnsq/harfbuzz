#ifndef OT_LAYOUT_GPOS_PAIRPOSFORMAT1_HH
#define OT_LAYOUT_GPOS_PAIRPOSFORMAT1_HH

#شامل "PairSet.hh"

فضاینام OT {
فضاینام Layout {
فضاینام GPOS_impl {


template <typename Types>
ساختار PairPosFormat1_3
{
  using PairSet = GPOS_impl::PairSet<Types>;
  using Pair_مقدار_Record = GPOS_impl::Pair_مقدار_Record<Types>;

  محافظتشده:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  typename Types::template LOffsetTo<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of subtable */
  مقدار_Format   مقدار_Format_2[2];         /* [0] Defines the types of data in
                                         * ValueRecord1--for the first glyph
                                         * in the pair--may be zero (0) */
                                        /* [1] Defines the types of data in
                                         * ValueRecord2--for the second glyph
                                         * in the pair--may be zero (0) */
  typename Types::template ArrayOf<typename Types::template OffsetTo<PairSet>>
                pairSet;                /* Array of PairSet tables ordered by
                                         * Coverage Index */
  عمومی:
  DEFINE_SIZE_ARRAY (2 + Types::LOffset::static_size + 2 * مقدار_Format::static_size + Types::HBUINT::static_size, pairSet);

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);

    اگر (!c->check_struct (این)) return_trace (نادرست);
    hb_barrier ();

    unsigned صحیح len1 = مقدار_Format_2[0].get_len ();
    unsigned صحیح len2 = مقدار_Format_2[1].get_len ();
    typename PairSet::sanitize_closure_t closure =
    {
      مقدار_Format_2,
      len1,
      PairSet::get_size (len1, len2)
    };

    return_trace (coverage.sanitize (c, این) && pairSet.sanitize (c, این, &closure));
  }

  منطقی intersects (ثابت hb_set_t *glyphs) ثابت
  {
    auto &cov = این+coverage;

    اگر (pairSet.len > glyphs->get_population () * hb_bit_storage ((unsigned) pairSet.len))
    {
      برای (hb_codepoint_t g : glyphs->iter ())
      {
	unsigned i = cov.get_coverage (g);
	اگر ((این+pairSet[i]).intersects (glyphs, مقدار_Format_2))
	  بازگشت درست;
      }
      بازگشت نادرست;
    }

    بازگشت
    + hb_zip (cov, pairSet)
    | hb_filter (*glyphs, hb_first)
    | hb_map (hb_second)
    | hb_map ([glyphs, این] (ثابت typename Types::template OffsetTo<PairSet> &_)
              { بازگشت (این+_).intersects (glyphs, مقدار_Format_2); })
    | hb_any
    ;
  }

  تهی closure_lookups (hb_closure_lookups_context_t *c) ثابت {}
  تهی collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    اگر ((!مقدار_Format_2[0].has_device ()) && (!مقدار_Format_2[1].has_device ())) بازگشت;

    auto it =
    + hb_zip (این+coverage, pairSet)
    | hb_filter (c->glyph_set, hb_first)
    | hb_map (hb_second)
    ;

    اگر (!it) بازگشت;
    + it
    | hb_map (hb_add (این))
    | hb_apply ([&] (ثابت PairSet& _) { _.collect_variation_indices (c, مقدار_Format_2); })
    ;
  }

  تهی collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    اگر (unlikely (!(این+coverage).collect_coverage (c->درون_داد))) بازگشت;
    unsigned صحیح شمار = pairSet.len;
    برای (unsigned صحیح i = 0; i < شمار; i++)
      (این+pairSet[i]).collect_glyphs (c, مقدار_Format_2);
  }

  تهی collect_second_glyphs (hb_set_digest_t *digest) ثابت
  {
    unsigned شمار = pairSet.len;
    برای (unsigned i = 0; i < شمار; i++)
      (این+pairSet[i]).collect_second_glyphs (digest, مقدار_Format_2);
  }

  ثابت Coverage &get_coverage () ثابت { بازگشت این+coverage; }

  ساختار external_cache_t
  {
    hb_ot_layout_mapping_cache_t coverage;
    hb_set_digest_t pair_sets[HB_VAR_ARRAY];
  };
  تهی *external_cache_create () ثابت
  {
    unsigned شمار = pairSet.len;
    unsigned size = اندازه (external_cache_t) -
		    HB_VAR_ARRAY * اندازه (hb_set_digest_t) +
		    شمار * اندازه (hb_set_digest_t);
    external_cache_t *cache = (external_cache_t *) hb_malloc (size);
    اگر (likely (cache))
    {
      cache->coverage.clear ();
      برای (unsigned i = 0; i < شمار; i++)
      {
	cache->pair_sets[i].init ();
	(این+pairSet[i]).collect_second_glyphs (&cache->pair_sets[i], مقدار_Format_2);
      }
    }
    بازگشت cache;
  }

  منطقی apply (hb_ot_apply_context_t *c, تهی *external_cache) ثابت
  {
    TRACE_APPLY (این);

    hb_buffer_t *buffer = c->buffer;

#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    external_cache_t *cache = (external_cache_t *) external_cache;
    unsigned صحیح index = (این+coverage).get_coverage  (buffer->cur().codepoint, cache ? &cache->coverage : nullptr);
#وگرنه
    unsigned صحیح index = (این+coverage).get_coverage  (buffer->cur().codepoint);
#endif
    اگر (index == NOT_COVERED) return_trace (نادرست);

    auto &skippy_iter = c->iter_درون_داد;
    skippy_iter.reset_fast (buffer->idx);
    unsigned unsafe_to;
    اگر (unlikely (!skippy_iter.next (&unsafe_to)))
    {
      buffer->unsafe_to_concat (buffer->idx, unsafe_to);
      return_trace (نادرست);
    }

#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    اگر (cache &&
	index < pairSet.len &&
	!cache->pair_sets[index].may_have (buffer->info[skippy_iter.idx].codepoint))
    {
      buffer->unsafe_to_concat (buffer->idx, skippy_iter.idx + 1);
      return_trace (نادرست);
    }
#endif

    return_trace ((این+pairSet[index]).apply (c, مقدار_Format_2, skippy_iter.idx));
  }

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (این);

    ثابت hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*این);
    اگر (unlikely (!c->serializer->extend_min (out))) return_trace (نادرست);
    out->format = format;

    hb_pair_t<unsigned, unsigned> newFormats = hb_pair (مقدار_Format_2[0], مقدار_Format_2[1]);

    اگر (c->plan->normalized_coords)
    {
      /* all device flags will be dropped when full instancing, no need to strip
       * hints, also do not strip emtpy cause we don't compute the new default
       * value during stripping */
      newFormats = compute_effective_مقدار_formats (glyphset, نادرست, نادرست, &c->plan->layout_variation_idx_delta_map);
    }
    /* do not strip hints for VF */
    وگرنه اگر (c->plan->flags & HB_SUBSET_FLAGS_NO_HINTING)
    {
      hb_blob_t* blob = hb_face_reference_table (c->plan->source, HB_TAG ('f','v','a','r'));
      منطقی has_fvar = (blob != hb_blob_get_empty ());
      hb_blob_destroy (blob);

      منطقی strip = !has_fvar;
      /* special case: strip hints when a VF has no GDEF varstore after
       * subsetting*/
      اگر (has_fvar && !c->plan->has_gdef_varstore)
        strip = درست;
      newFormats = compute_effective_مقدار_formats (glyphset, strip, درست);
    }

    out->مقدار_Format_2[0] = newFormats.first;
    out->مقدار_Format_2[1] = newFormats.second;

    hb_sorted_vector_t<hb_codepoint_t> new_coverage;

    + hb_zip (این+coverage, pairSet)
    | hb_filter (glyphset, hb_first)
    | hb_filter ([این, c, out] (ثابت typename Types::template OffsetTo<PairSet>& _)
                 {
                   auto snap = c->serializer->snapshot ();
                   auto *o = out->pairSet.serialize_append (c->serializer);
                   اگر (unlikely (!o)) بازگشت نادرست;
                   منطقی ret = o->serialize_subset (c, _, این, مقدار_Format_2, out->مقدار_Format_2);
                   اگر (!ret)
                   {
                     out->pairSet.برداشت ();
                     c->serializer->revert (snap);
                   }
                   بازگشت ret;
                 },
                 hb_second)
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    out->coverage.serialize_serialize (c->serializer, new_coverage.iter ());

    return_trace (منطقی (new_coverage));
  }


  hb_pair_t<unsigned, unsigned> compute_effective_مقدار_formats (ثابت hb_set_t& glyphset,
                                                                 منطقی strip_hints, منطقی strip_empty,
                                                                 ثابت hb_hashmap_t<unsigned, hb_pair_t<unsigned, صحیح>> *varidx_delta_map = nullptr) ثابت
  {
    unsigned format1 = 0;
    unsigned format2 = 0;
    برای (ثابت auto & _ :
	  + hb_zip (این+coverage, pairSet)
	  | hb_filter (glyphset, hb_first)
	  | hb_map (hb_second)
	)
    {
      ثابت PairSet& set = (این + _);
      auto formats = set.get_مقادیر_view (مقدار_Format_2).compute_effective_مقدار_formats (
          glyphset, مقدار_Format_2, strip_hints, strip_empty, varidx_delta_map);
      format1 |= formats.first;
      format2 |= formats.second;

      اگر (!varidx_delta_map &&
          format1 == مقدار_Format_2[0] && format2 == مقدار_Format_2[1])
        بشکن;
    }

    بازگشت hb_pair (format1, format2);
  }
};


}
}
}

#endif  // OT_LAYOUT_GPOS_PAIRPOSFORMAT1_HH
