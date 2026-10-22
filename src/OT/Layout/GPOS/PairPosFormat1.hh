#ifndef OT_LAYOUT_GPOS_PAIRPOSFORMAT1_HH
#define OT_LAYOUT_GPOS_PAIRPOSFORMAT1_HH

#அந்தர்பாவ "PairSet.hh"

நாமாகாஶ OT {
நாமாகாஶ Layout {
நாமாகாஶ GPOS_impl {


template <typename Types>
கட்டமைப்பு PairPosFormat1_3
{
  using PairSet = GPOS_impl::PairSet<Types>;
  using Pair_மதிப்பு_Record = GPOS_impl::Pair_மதிப்பு_Record<Types>;

  பாதுகாக்கப்பட்ட:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  typename Types::template LOffsetTo<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of subtable */
  மதிப்பு_Format   மதிப்பு_Format_2[2];         /* [0] Defines the types of data in
                                         * ValueRecord1--for the first glyph
                                         * in the pair--may be zero (0) */
                                        /* [1] Defines the types of data in
                                         * ValueRecord2--for the second glyph
                                         * in the pair--may be zero (0) */
  typename Types::template ArrayOf<typename Types::template OffsetTo<PairSet>>
                pairSet;                /* Array of PairSet tables ordered by
                                         * Coverage Index */
  பொது:
  DEFINE_SIZE_ARRAY (2 + Types::LOffset::static_size + 2 * மதிப்பு_Format::static_size + Types::HBUINT::static_size, pairSet);

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);

    எனில் (!c->check_struct (இது)) return_trace (பொய்);
    hb_barrier ();

    unsigned முழு len1 = மதிப்பு_Format_2[0].get_len ();
    unsigned முழு len2 = மதிப்பு_Format_2[1].get_len ();
    typename PairSet::sanitize_closure_t closure =
    {
      மதிப்பு_Format_2,
      len1,
      PairSet::get_size (len1, len2)
    };

    return_trace (coverage.sanitize (c, இது) && pairSet.sanitize (c, இது, &closure));
  }

  தருக்க intersects (மாறிலி hb_set_t *glyphs) மாறிலி
  {
    auto &cov = இது+coverage;

    எனில் (pairSet.len > glyphs->get_population () * hb_bit_storage ((unsigned) pairSet.len))
    {
      க்கு (hb_codepoint_t g : glyphs->iter ())
      {
	unsigned i = cov.get_coverage (g);
	எனில் ((இது+pairSet[i]).intersects (glyphs, மதிப்பு_Format_2))
	  திரும்பு உண்மை;
      }
      திரும்பு பொய்;
    }

    திரும்பு
    + hb_zip (cov, pairSet)
    | hb_filter (*glyphs, hb_first)
    | hb_map (hb_second)
    | hb_map ([glyphs, இது] (மாறிலி typename Types::template OffsetTo<PairSet> &_)
              { திரும்பு (இது+_).intersects (glyphs, மதிப்பு_Format_2); })
    | hb_any
    ;
  }

  வெறுமை closure_lookups (hb_closure_lookups_context_t *c) மாறிலி {}
  வெறுமை collect_variation_indices (hb_collect_variation_indices_context_t *c) மாறிலி
  {
    எனில் ((!மதிப்பு_Format_2[0].has_device ()) && (!மதிப்பு_Format_2[1].has_device ())) திரும்பு;

    auto it =
    + hb_zip (இது+coverage, pairSet)
    | hb_filter (c->glyph_set, hb_first)
    | hb_map (hb_second)
    ;

    எனில் (!it) திரும்பு;
    + it
    | hb_map (hb_add (இது))
    | hb_apply ([&] (மாறிலி PairSet& _) { _.collect_variation_indices (c, மதிப்பு_Format_2); })
    ;
  }

  வெறுமை collect_glyphs (hb_collect_glyphs_context_t *c) மாறிலி
  {
    எனில் (unlikely (!(இது+coverage).collect_coverage (c->உள்ளீடு))) திரும்பு;
    unsigned முழு மொத்தம் = pairSet.len;
    க்கு (unsigned முழு i = 0; i < மொத்தம்; i++)
      (இது+pairSet[i]).collect_glyphs (c, மதிப்பு_Format_2);
  }

  வெறுமை collect_second_glyphs (hb_set_digest_t *digest) மாறிலி
  {
    unsigned மொத்தம் = pairSet.len;
    க்கு (unsigned i = 0; i < மொத்தம்; i++)
      (இது+pairSet[i]).collect_second_glyphs (digest, மதிப்பு_Format_2);
  }

  மாறிலி Coverage &get_coverage () மாறிலி { திரும்பு இது+coverage; }

  கட்டமைப்பு external_cache_t
  {
    hb_ot_layout_mapping_cache_t coverage;
    hb_set_digest_t pair_sets[HB_VAR_ARRAY];
  };
  வெறுமை *external_cache_create () மாறிலி
  {
    unsigned மொத்தம் = pairSet.len;
    unsigned size = அளவு (external_cache_t) -
		    HB_VAR_ARRAY * அளவு (hb_set_digest_t) +
		    மொத்தம் * அளவு (hb_set_digest_t);
    external_cache_t *cache = (external_cache_t *) hb_malloc (size);
    எனில் (likely (cache))
    {
      cache->coverage.clear ();
      க்கு (unsigned i = 0; i < மொத்தம்; i++)
      {
	cache->pair_sets[i].init ();
	(இது+pairSet[i]).collect_second_glyphs (&cache->pair_sets[i], மதிப்பு_Format_2);
      }
    }
    திரும்பு cache;
  }

  தருக்க apply (hb_ot_apply_context_t *c, வெறுமை *external_cache) மாறிலி
  {
    TRACE_APPLY (இது);

    hb_buffer_t *buffer = c->buffer;

#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    external_cache_t *cache = (external_cache_t *) external_cache;
    unsigned முழு index = (இது+coverage).get_coverage  (buffer->cur().codepoint, cache ? &cache->coverage : nullptr);
#இல்லையெனில்
    unsigned முழு index = (இது+coverage).get_coverage  (buffer->cur().codepoint);
#endif
    எனில் (index == NOT_COVERED) return_trace (பொய்);

    auto &skippy_iter = c->iter_உள்ளீடு;
    skippy_iter.reset_fast (buffer->idx);
    unsigned unsafe_to;
    எனில் (unlikely (!skippy_iter.next (&unsafe_to)))
    {
      buffer->unsafe_to_concat (buffer->idx, unsafe_to);
      return_trace (பொய்);
    }

#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    எனில் (cache &&
	index < pairSet.len &&
	!cache->pair_sets[index].may_have (buffer->info[skippy_iter.idx].codepoint))
    {
      buffer->unsafe_to_concat (buffer->idx, skippy_iter.idx + 1);
      return_trace (பொய்);
    }
#endif

    return_trace ((இது+pairSet[index]).apply (c, மதிப்பு_Format_2, skippy_iter.idx));
  }

  தருக்க subset (hb_subset_context_t *c) மாறிலி
  {
    TRACE_SUBSET (இது);

    மாறிலி hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    மாறிலி hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*இது);
    எனில் (unlikely (!c->serializer->extend_min (out))) return_trace (பொய்);
    out->format = format;

    hb_pair_t<unsigned, unsigned> newFormats = hb_pair (மதிப்பு_Format_2[0], மதிப்பு_Format_2[1]);

    எனில் (c->plan->normalized_coords)
    {
      /* all device flags will be dropped when full instancing, no need to strip
       * hints, also do not strip emtpy cause we don't compute the new default
       * value during stripping */
      newFormats = compute_effective_மதிப்பு_formats (glyphset, பொய், பொய், &c->plan->layout_variation_idx_delta_map);
    }
    /* do not strip hints for VF */
    இல்லையெனில் எனில் (c->plan->flags & HB_SUBSET_FLAGS_NO_HINTING)
    {
      hb_blob_t* blob = hb_face_reference_table (c->plan->source, HB_TAG ('f','v','a','r'));
      தருக்க has_fvar = (blob != hb_blob_get_empty ());
      hb_blob_destroy (blob);

      தருக்க strip = !has_fvar;
      /* special case: strip hints when a VF has no GDEF varstore after
       * subsetting*/
      எனில் (has_fvar && !c->plan->has_gdef_varstore)
        strip = உண்மை;
      newFormats = compute_effective_மதிப்பு_formats (glyphset, strip, உண்மை);
    }

    out->மதிப்பு_Format_2[0] = newFormats.first;
    out->மதிப்பு_Format_2[1] = newFormats.second;

    hb_sorted_vector_t<hb_codepoint_t> new_coverage;

    + hb_zip (இது+coverage, pairSet)
    | hb_filter (glyphset, hb_first)
    | hb_filter ([இது, c, out] (மாறிலி typename Types::template OffsetTo<PairSet>& _)
                 {
                   auto snap = c->serializer->snapshot ();
                   auto *o = out->pairSet.serialize_append (c->serializer);
                   எனில் (unlikely (!o)) திரும்பு பொய்;
                   தருக்க ret = o->serialize_subset (c, _, இது, மதிப்பு_Format_2, out->மதிப்பு_Format_2);
                   எனில் (!ret)
                   {
                     out->pairSet.எடு ();
                     c->serializer->revert (snap);
                   }
                   திரும்பு ret;
                 },
                 hb_second)
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    out->coverage.serialize_serialize (c->serializer, new_coverage.iter ());

    return_trace (தருக்க (new_coverage));
  }


  hb_pair_t<unsigned, unsigned> compute_effective_மதிப்பு_formats (மாறிலி hb_set_t& glyphset,
                                                                 தருக்க strip_hints, தருக்க strip_empty,
                                                                 மாறிலி hb_hashmap_t<unsigned, hb_pair_t<unsigned, முழு>> *varidx_delta_map = nullptr) மாறிலி
  {
    unsigned format1 = 0;
    unsigned format2 = 0;
    க்கு (மாறிலி auto & _ :
	  + hb_zip (இது+coverage, pairSet)
	  | hb_filter (glyphset, hb_first)
	  | hb_map (hb_second)
	)
    {
      மாறிலி PairSet& set = (இது + _);
      auto formats = set.get_மதிப்புகள்_view (மதிப்பு_Format_2).compute_effective_மதிப்பு_formats (
          glyphset, மதிப்பு_Format_2, strip_hints, strip_empty, varidx_delta_map);
      format1 |= formats.first;
      format2 |= formats.second;

      எனில் (!varidx_delta_map &&
          format1 == மதிப்பு_Format_2[0] && format2 == மதிப்பு_Format_2[1])
        நிறுத்து;
    }

    திரும்பு hb_pair (format1, format2);
  }
};


}
}
}

#endif  // OT_LAYOUT_GPOS_PAIRPOSFORMAT1_HH
