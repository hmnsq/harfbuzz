#ifndef OT_LAYOUT_GPOS_PAIRPOSFORMAT1_HH
#define OT_LAYOUT_GPOS_PAIRPOSFORMAT1_HH

#ଅନ୍ତର୍ଭାଵ "PairSet.hh"

ନାମାକାଶ OT {
ନାମାକାଶ Layout {
ନାମାକାଶ GPOS_impl {


template <typename Types>
ସଂରଚନା PairPosFormat1_3
{
  using PairSet = GPOS_impl::PairSet<Types>;
  using Pair_ମୂଲ୍ୟ_Record = GPOS_impl::Pair_ମୂଲ୍ୟ_Record<Types>;

  ସୁରକ୍ଷିତ:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  typename Types::template LOffsetTo<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of subtable */
  ମୂଲ୍ୟ_Format   ମୂଲ୍ୟ_Format_2[2];         /* [0] Defines the types of data in
                                         * ValueRecord1--for the first glyph
                                         * in the pair--may be zero (0) */
                                        /* [1] Defines the types of data in
                                         * ValueRecord2--for the second glyph
                                         * in the pair--may be zero (0) */
  typename Types::template ArrayOf<typename Types::template OffsetTo<PairSet>>
                pairSet;                /* Array of PairSet tables ordered by
                                         * Coverage Index */
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_ARRAY (2 + Types::LOffset::static_size + 2 * ମୂଲ୍ୟ_Format::static_size + Types::HBUINT::static_size, pairSet);

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);

    ଯଦି (!c->check_struct (ଏହି)) return_trace (ମିଥ୍ୟା);
    hb_barrier ();

    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା len1 = ମୂଲ୍ୟ_Format_2[0].get_len ();
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା len2 = ମୂଲ୍ୟ_Format_2[1].get_len ();
    typename PairSet::sanitize_closure_t closure =
    {
      ମୂଲ୍ୟ_Format_2,
      len1,
      PairSet::get_size (len1, len2)
    };

    return_trace (coverage.sanitize (c, ଏହି) && pairSet.sanitize (c, ଏହି, &closure));
  }

  ତାର୍କିକ intersects (ଅଚଳ hb_set_t *glyphs) ଅଚଳ
  {
    auto &cov = ଏହି+coverage;

    ଯଦି (pairSet.len > glyphs->get_population () * hb_bit_storage ((unsigned) pairSet.len))
    {
      ପାଇଁ (hb_codepoint_t g : glyphs->iter ())
      {
	unsigned i = cov.get_coverage (g);
	ଯଦି ((ଏହି+pairSet[i]).intersects (glyphs, ମୂଲ୍ୟ_Format_2))
	  ଫେରନ୍ତୁ ସତ୍ୟ;
      }
      ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }

    ଫେରନ୍ତୁ
    + hb_zip (cov, pairSet)
    | hb_filter (*glyphs, hb_first)
    | hb_map (hb_second)
    | hb_map ([glyphs, ଏହି] (ଅଚଳ typename Types::template OffsetTo<PairSet> &_)
              { ଫେରନ୍ତୁ (ଏହି+_).intersects (glyphs, ମୂଲ୍ୟ_Format_2); })
    | hb_any
    ;
  }

  ଶୂନ୍ୟ closure_lookups (hb_closure_lookups_context_t *c) ଅଚଳ {}
  ଶୂନ୍ୟ collect_variation_indices (hb_collect_variation_indices_context_t *c) ଅଚଳ
  {
    ଯଦି ((!ମୂଲ୍ୟ_Format_2[0].has_device ()) && (!ମୂଲ୍ୟ_Format_2[1].has_device ())) ଫେରନ୍ତୁ;

    auto it =
    + hb_zip (ଏହି+coverage, pairSet)
    | hb_filter (c->glyph_set, hb_first)
    | hb_map (hb_second)
    ;

    ଯଦି (!it) ଫେରନ୍ତୁ;
    + it
    | hb_map (hb_add (ଏହି))
    | hb_apply ([&] (ଅଚଳ PairSet& _) { _.collect_variation_indices (c, ମୂଲ୍ୟ_Format_2); })
    ;
  }

  ଶୂନ୍ୟ collect_glyphs (hb_collect_glyphs_context_t *c) ଅଚଳ
  {
    ଯଦି (unlikely (!(ଏହି+coverage).collect_coverage (c->ନିବେଶ))) ଫେରନ୍ତୁ;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = pairSet.len;
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଗଣନା_2; i++)
      (ଏହି+pairSet[i]).collect_glyphs (c, ମୂଲ୍ୟ_Format_2);
  }

  ଶୂନ୍ୟ collect_second_glyphs (hb_set_digest_t *digest) ଅଚଳ
  {
    unsigned ଗଣନା_2 = pairSet.len;
    ପାଇଁ (unsigned i = 0; i < ଗଣନା_2; i++)
      (ଏହି+pairSet[i]).collect_second_glyphs (digest, ମୂଲ୍ୟ_Format_2);
  }

  ଅଚଳ Coverage &get_coverage () ଅଚଳ { ଫେରନ୍ତୁ ଏହି+coverage; }

  ସଂରଚନା external_cache_t
  {
    hb_ot_layout_mapping_cache_t coverage;
    hb_set_digest_t pair_sets[HB_VAR_ARRAY];
  };
  ଶୂନ୍ୟ *external_cache_create () ଅଚଳ
  {
    unsigned ଗଣନା_2 = pairSet.len;
    unsigned size = ଆକାର (external_cache_t) -
		    HB_VAR_ARRAY * ଆକାର (hb_set_digest_t) +
		    ଗଣନା_2 * ଆକାର (hb_set_digest_t);
    external_cache_t *cache = (external_cache_t *) hb_malloc (size);
    ଯଦି (likely (cache))
    {
      cache->coverage.clear ();
      ପାଇଁ (unsigned i = 0; i < ଗଣନା_2; i++)
      {
	cache->pair_sets[i].init ();
	(ଏହି+pairSet[i]).collect_second_glyphs (&cache->pair_sets[i], ମୂଲ୍ୟ_Format_2);
      }
    }
    ଫେରନ୍ତୁ cache;
  }

  ତାର୍କିକ apply (hb_ot_apply_context_t *c, ଶୂନ୍ୟ *external_cache) ଅଚଳ
  {
    TRACE_APPLY (ଏହି);

    hb_buffer_t *buffer = c->buffer;

#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    external_cache_t *cache = (external_cache_t *) external_cache;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା index = (ଏହି+coverage).get_coverage  (buffer->cur().codepoint, cache ? &cache->coverage : nullptr);
#ଅନ୍ୟଥା
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା index = (ଏହି+coverage).get_coverage  (buffer->cur().codepoint);
#endif
    ଯଦି (index == NOT_COVERED) return_trace (ମିଥ୍ୟା);

    auto &skippy_iter = c->iter_ନିବେଶ;
    skippy_iter.reset_fast (buffer->idx);
    unsigned unsafe_to;
    ଯଦି (unlikely (!skippy_iter.next (&unsafe_to)))
    {
      buffer->unsafe_to_concat (buffer->idx, unsafe_to);
      return_trace (ମିଥ୍ୟା);
    }

#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    ଯଦି (cache &&
	index < pairSet.len &&
	!cache->pair_sets[index].may_have (buffer->info[skippy_iter.idx].codepoint))
    {
      buffer->unsafe_to_concat (buffer->idx, skippy_iter.idx + 1);
      return_trace (ମିଥ୍ୟା);
    }
#endif

    return_trace ((ଏହି+pairSet[index]).apply (c, ମୂଲ୍ୟ_Format_2, skippy_iter.idx));
  }

  ତାର୍କିକ subset (hb_subset_context_t *c) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);

    ଅଚଳ hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ଅଚଳ hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*ଏହି);
    ଯଦି (unlikely (!c->serializer->extend_min (out))) return_trace (ମିଥ୍ୟା);
    out->format = format;

    hb_pair_t<unsigned, unsigned> newFormats = hb_pair (ମୂଲ୍ୟ_Format_2[0], ମୂଲ୍ୟ_Format_2[1]);

    ଯଦି (c->plan->normalized_coords)
    {
      /* all device flags will be dropped when full instancing, no need to strip
       * hints, also do not strip emtpy cause we don't compute the new default
       * value during stripping */
      newFormats = compute_effective_ମୂଲ୍ୟ_formats (glyphset, ମିଥ୍ୟା, ମିଥ୍ୟା, &c->plan->layout_variation_idx_delta_map);
    }
    /* do not strip hints for VF */
    ଅନ୍ୟଥା ଯଦି (c->plan->flags & HB_SUBSET_FLAGS_NO_HINTING)
    {
      hb_blob_t* blob = hb_face_reference_table (c->plan->source, HB_TAG ('f','v','a','r'));
      ତାର୍କିକ has_fvar = (blob != hb_blob_get_empty ());
      hb_blob_destroy (blob);

      ତାର୍କିକ strip = !has_fvar;
      /* special case: strip hints when a VF has no GDEF varstore after
       * subsetting*/
      ଯଦି (has_fvar && !c->plan->has_gdef_varstore)
        strip = ସତ୍ୟ;
      newFormats = compute_effective_ମୂଲ୍ୟ_formats (glyphset, strip, ସତ୍ୟ);
    }

    out->ମୂଲ୍ୟ_Format_2[0] = newFormats.first;
    out->ମୂଲ୍ୟ_Format_2[1] = newFormats.second;

    hb_sorted_vector_t<hb_codepoint_t> new_coverage;

    + hb_zip (ଏହି+coverage, pairSet)
    | hb_filter (glyphset, hb_first)
    | hb_filter ([ଏହି, c, out] (ଅଚଳ typename Types::template OffsetTo<PairSet>& _)
                 {
                   auto snap = c->serializer->snapshot ();
                   auto *o = out->pairSet.serialize_append (c->serializer);
                   ଯଦି (unlikely (!o)) ଫେରନ୍ତୁ ମିଥ୍ୟା;
                   ତାର୍କିକ ret = o->serialize_subset (c, _, ଏହି, ମୂଲ୍ୟ_Format_2, out->ମୂଲ୍ୟ_Format_2);
                   ଯଦି (!ret)
                   {
                     out->pairSet.କାଢ଼ ();
                     c->serializer->revert (snap);
                   }
                   ଫେରନ୍ତୁ ret;
                 },
                 hb_second)
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    out->coverage.serialize_serialize (c->serializer, new_coverage.iter ());

    return_trace (ତାର୍କିକ (new_coverage));
  }


  hb_pair_t<unsigned, unsigned> compute_effective_ମୂଲ୍ୟ_formats (ଅଚଳ hb_set_t& glyphset,
                                                                 ତାର୍କିକ strip_hints, ତାର୍କିକ strip_empty,
                                                                 ଅଚଳ hb_hashmap_t<unsigned, hb_pair_t<unsigned, ପୂର୍ଣ୍ଣସଂଖ୍ୟା>> *varidx_delta_map = nullptr) ଅଚଳ
  {
    unsigned format1 = 0;
    unsigned format2 = 0;
    ପାଇଁ (ଅଚଳ auto & _ :
	  + hb_zip (ଏହି+coverage, pairSet)
	  | hb_filter (glyphset, hb_first)
	  | hb_map (hb_second)
	)
    {
      ଅଚଳ PairSet& set = (ଏହି + _);
      auto formats = set.get_ମୂଲ୍ୟଗୁଡ଼ିକ_view (ମୂଲ୍ୟ_Format_2).compute_effective_ମୂଲ୍ୟ_formats (
          glyphset, ମୂଲ୍ୟ_Format_2, strip_hints, strip_empty, varidx_delta_map);
      format1 |= formats.first;
      format2 |= formats.second;

      ଯଦି (!varidx_delta_map &&
          format1 == ମୂଲ୍ୟ_Format_2[0] && format2 == ମୂଲ୍ୟ_Format_2[1])
        ଭାଙ୍ଗ;
    }

    ଫେରନ୍ତୁ hb_pair (format1, format2);
  }
};


}
}
}

#endif  // OT_LAYOUT_GPOS_PAIRPOSFORMAT1_HH
