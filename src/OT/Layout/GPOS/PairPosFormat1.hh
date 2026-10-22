#ifndef OT_LAYOUT_GPOS_PAIRPOSFORMAT1_HH
#define OT_LAYOUT_GPOS_PAIRPOSFORMAT1_HH

#অন্তর্ভাব "PairSet.hh"

নামাকাশ OT {
নামাকাশ Layout {
নামাকাশ GPOS_impl {


template <typename Types>
সংরচনা PairPosFormat1_3
{
  using PairSet = GPOS_impl::PairSet<Types>;
  using Pair_মান_Record = GPOS_impl::Pair_মান_Record<Types>;

  রক্ষিত:
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
  সার্বজনিক:
  DEFINE_SIZE_ARRAY (2 + Types::LOffset::static_size + 2 * মান_Format::static_size + Types::HBUINT::static_size, pairSet);

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);

    যদি (!c->check_struct (এতত্)) return_trace (অসত্য);
    hb_barrier ();

    unsigned পূর্ণাঙ্ক len1 = মান_Format_2[0].get_len ();
    unsigned পূর্ণাঙ্ক len2 = মান_Format_2[1].get_len ();
    typename PairSet::sanitize_closure_t closure =
    {
      মান_Format_2,
      len1,
      PairSet::get_size (len1, len2)
    };

    return_trace (coverage.sanitize (c, এতত্) && pairSet.sanitize (c, এতত্, &closure));
  }

  তার্কিক intersects (ধ্রুব hb_set_t *glyphs) ধ্রুব
  {
    auto &cov = এতত্+coverage;

    যদি (pairSet.len > glyphs->get_population () * hb_bit_storage ((unsigned) pairSet.len))
    {
      প্রতি (hb_codepoint_t g : glyphs->iter ())
      {
	unsigned i = cov.get_coverage (g);
	যদি ((এতত্+pairSet[i]).intersects (glyphs, মান_Format_2))
	  প্রত্যাগম সত্য;
      }
      প্রত্যাগম অসত্য;
    }

    প্রত্যাগম
    + hb_zip (cov, pairSet)
    | hb_filter (*glyphs, hb_first)
    | hb_map (hb_second)
    | hb_map ([glyphs, এতত্] (ধ্রুব typename Types::template OffsetTo<PairSet> &_)
              { প্রত্যাগম (এতত্+_).intersects (glyphs, মান_Format_2); })
    | hb_any
    ;
  }

  শূন্য closure_lookups (hb_closure_lookups_context_t *c) ধ্রুব {}
  শূন্য collect_variation_indices (hb_collect_variation_indices_context_t *c) ধ্রুব
  {
    যদি ((!মান_Format_2[0].has_device ()) && (!মান_Format_2[1].has_device ())) প্রত্যাগম;

    auto it =
    + hb_zip (এতত্+coverage, pairSet)
    | hb_filter (c->glyph_set, hb_first)
    | hb_map (hb_second)
    ;

    যদি (!it) প্রত্যাগম;
    + it
    | hb_map (hb_add (এতত্))
    | hb_apply ([&] (ধ্রুব PairSet& _) { _.collect_variation_indices (c, মান_Format_2); })
    ;
  }

  শূন্য collect_glyphs (hb_collect_glyphs_context_t *c) ধ্রুব
  {
    যদি (unlikely (!(এতত্+coverage).collect_coverage (c->প্রবেশ))) প্রত্যাগম;
    unsigned পূর্ণাঙ্ক গুনতি = pairSet.len;
    প্রতি (unsigned পূর্ণাঙ্ক i = 0; i < গুনতি; i++)
      (এতত্+pairSet[i]).collect_glyphs (c, মান_Format_2);
  }

  শূন্য collect_second_glyphs (hb_set_digest_t *digest) ধ্রুব
  {
    unsigned গুনতি = pairSet.len;
    প্রতি (unsigned i = 0; i < গুনতি; i++)
      (এতত্+pairSet[i]).collect_second_glyphs (digest, মান_Format_2);
  }

  ধ্রুব Coverage &get_coverage () ধ্রুব { প্রত্যাগম এতত্+coverage; }

  সংরচনা external_cache_t
  {
    hb_ot_layout_mapping_cache_t coverage;
    hb_set_digest_t pair_sets[HB_VAR_ARRAY];
  };
  শূন্য *external_cache_create () ধ্রুব
  {
    unsigned গুনতি = pairSet.len;
    unsigned size = পরিমাণ (external_cache_t) -
		    HB_VAR_ARRAY * পরিমাণ (hb_set_digest_t) +
		    গুনতি * পরিমাণ (hb_set_digest_t);
    external_cache_t *cache = (external_cache_t *) hb_malloc (size);
    যদি (likely (cache))
    {
      cache->coverage.clear ();
      প্রতি (unsigned i = 0; i < গুনতি; i++)
      {
	cache->pair_sets[i].init ();
	(এতত্+pairSet[i]).collect_second_glyphs (&cache->pair_sets[i], মান_Format_2);
      }
    }
    প্রত্যাগম cache;
  }

  তার্কিক apply (hb_ot_apply_context_t *c, শূন্য *external_cache) ধ্রুব
  {
    TRACE_APPLY (এতত্);

    hb_buffer_t *buffer = c->buffer;

#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    external_cache_t *cache = (external_cache_t *) external_cache;
    unsigned পূর্ণাঙ্ক index = (এতত্+coverage).get_coverage  (buffer->cur().codepoint, cache ? &cache->coverage : nullptr);
#অন্যথা
    unsigned পূর্ণাঙ্ক index = (এতত্+coverage).get_coverage  (buffer->cur().codepoint);
#endif
    যদি (index == NOT_COVERED) return_trace (অসত্য);

    auto &skippy_iter = c->iter_প্রবেশ;
    skippy_iter.reset_fast (buffer->idx);
    unsigned unsafe_to;
    যদি (unlikely (!skippy_iter.next (&unsafe_to)))
    {
      buffer->unsafe_to_concat (buffer->idx, unsafe_to);
      return_trace (অসত্য);
    }

#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    যদি (cache &&
	index < pairSet.len &&
	!cache->pair_sets[index].may_have (buffer->info[skippy_iter.idx].codepoint))
    {
      buffer->unsafe_to_concat (buffer->idx, skippy_iter.idx + 1);
      return_trace (অসত্য);
    }
#endif

    return_trace ((এতত্+pairSet[index]).apply (c, মান_Format_2, skippy_iter.idx));
  }

  তার্কিক subset (hb_subset_context_t *c) ধ্রুব
  {
    TRACE_SUBSET (এতত্);

    ধ্রুব hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ধ্রুব hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*এতত্);
    যদি (unlikely (!c->serializer->extend_min (out))) return_trace (অসত্য);
    out->format = format;

    hb_pair_t<unsigned, unsigned> newFormats = hb_pair (মান_Format_2[0], মান_Format_2[1]);

    যদি (c->plan->normalized_coords)
    {
      /* all device flags will be dropped when full instancing, no need to strip
       * hints, also do not strip emtpy cause we don't compute the new default
       * value during stripping */
      newFormats = compute_effective_মান_formats (glyphset, অসত্য, অসত্য, &c->plan->layout_variation_idx_delta_map);
    }
    /* do not strip hints for VF */
    অন্যথা যদি (c->plan->flags & HB_SUBSET_FLAGS_NO_HINTING)
    {
      hb_blob_t* blob = hb_face_reference_table (c->plan->source, HB_TAG ('f','v','a','r'));
      তার্কিক has_fvar = (blob != hb_blob_get_empty ());
      hb_blob_destroy (blob);

      তার্কিক strip = !has_fvar;
      /* special case: strip hints when a VF has no GDEF varstore after
       * subsetting*/
      যদি (has_fvar && !c->plan->has_gdef_varstore)
        strip = সত্য;
      newFormats = compute_effective_মান_formats (glyphset, strip, সত্য);
    }

    out->মান_Format_2[0] = newFormats.first;
    out->মান_Format_2[1] = newFormats.second;

    hb_sorted_vector_t<hb_codepoint_t> new_coverage;

    + hb_zip (এতত্+coverage, pairSet)
    | hb_filter (glyphset, hb_first)
    | hb_filter ([এতত্, c, out] (ধ্রুব typename Types::template OffsetTo<PairSet>& _)
                 {
                   auto snap = c->serializer->snapshot ();
                   auto *o = out->pairSet.serialize_append (c->serializer);
                   যদি (unlikely (!o)) প্রত্যাগম অসত্য;
                   তার্কিক ret = o->serialize_subset (c, _, এতত্, মান_Format_2, out->মান_Format_2);
                   যদি (!ret)
                   {
                     out->pairSet.বের_করো ();
                     c->serializer->revert (snap);
                   }
                   প্রত্যাগম ret;
                 },
                 hb_second)
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    out->coverage.serialize_serialize (c->serializer, new_coverage.iter ());

    return_trace (তার্কিক (new_coverage));
  }


  hb_pair_t<unsigned, unsigned> compute_effective_মান_formats (ধ্রুব hb_set_t& glyphset,
                                                                 তার্কিক strip_hints, তার্কিক strip_empty,
                                                                 ধ্রুব hb_hashmap_t<unsigned, hb_pair_t<unsigned, পূর্ণাঙ্ক>> *varidx_delta_map = nullptr) ধ্রুব
  {
    unsigned format1 = 0;
    unsigned format2 = 0;
    প্রতি (ধ্রুব auto & _ :
	  + hb_zip (এতত্+coverage, pairSet)
	  | hb_filter (glyphset, hb_first)
	  | hb_map (hb_second)
	)
    {
      ধ্রুব PairSet& set = (এতত্ + _);
      auto formats = set.get_মানসমূহ_view (মান_Format_2).compute_effective_মান_formats (
          glyphset, মান_Format_2, strip_hints, strip_empty, varidx_delta_map);
      format1 |= formats.first;
      format2 |= formats.second;

      যদি (!varidx_delta_map &&
          format1 == মান_Format_2[0] && format2 == মান_Format_2[1])
        ভঙ্গ;
    }

    প্রত্যাগম hb_pair (format1, format2);
  }
};


}
}
}

#endif  // OT_LAYOUT_GPOS_PAIRPOSFORMAT1_HH
