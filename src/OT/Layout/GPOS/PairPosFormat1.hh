#ifndef OT_LAYOUT_GPOS_PAIRPOSFORMAT1_HH
#define OT_LAYOUT_GPOS_PAIRPOSFORMAT1_HH

#ضمن "PairSet.hh"

نطاق OT {
نطاق Layout {
نطاق GPOS_impl {


template <typename Types>
بنية PairPosFormat1_3
{
  using PairSet = GPOS_impl::PairSet<Types>;
  using Pair_بەها_Record = GPOS_impl::Pair_بەها_Record<Types>;

  محمي:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  typename Types::template LOffsetTo<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of subtable */
  بەها_Format   بەها_Format_2[2];         /* [0] Defines the types of data in
                                         * ValueRecord1--for the first glyph
                                         * in the pair--may be zero (0) */
                                        /* [1] Defines the types of data in
                                         * ValueRecord2--for the second glyph
                                         * in the pair--may be zero (0) */
  typename Types::template ArrayOf<typename Types::template OffsetTo<PairSet>>
                pairSet;                /* Array of PairSet tables ordered by
                                         * Coverage Index */
  عام:
  DEFINE_SIZE_ARRAY (2 + Types::LOffset::static_size + 2 * بەها_Format::static_size + Types::HBUINT::static_size, pairSet);

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);

    إذا (!c->check_struct (هذا)) return_trace (خطأ);
    hb_barrier ();

    unsigned صحيح len1 = بەها_Format_2[0].get_len ();
    unsigned صحيح len2 = بەها_Format_2[1].get_len ();
    typename PairSet::sanitize_closure_t closure =
    {
      بەها_Format_2,
      len1,
      PairSet::get_size (len1, len2)
    };

    return_trace (coverage.sanitize (c, هذا) && pairSet.sanitize (c, هذا, &closure));
  }

  منطقي intersects (ثابت hb_set_t *glyphs) ثابت
  {
    auto &cov = هذا+coverage;

    إذا (pairSet.len > glyphs->get_population () * hb_bit_storage ((unsigned) pairSet.len))
    {
      لكل (hb_codepoint_t g : glyphs->iter ())
      {
	unsigned i = cov.get_coverage (g);
	إذا ((هذا+pairSet[i]).intersects (glyphs, بەها_Format_2))
	  أرجع true;
      }
      أرجع خطأ;
    }

    أرجع
    + hb_zip (cov, pairSet)
    | hb_filter (*glyphs, hb_first)
    | hb_map (hb_second)
    | hb_map ([glyphs, هذا] (ثابت typename Types::template OffsetTo<PairSet> &_)
              { أرجع (هذا+_).intersects (glyphs, بەها_Format_2); })
    | hb_any
    ;
  }

  فراغ closure_lookups (hb_closure_lookups_context_t *c) ثابت {}
  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    إذا ((!بەها_Format_2[0].has_device ()) && (!بەها_Format_2[1].has_device ())) أرجع;

    auto it =
    + hb_zip (هذا+coverage, pairSet)
    | hb_filter (c->glyph_set, hb_first)
    | hb_map (hb_second)
    ;

    إذا (!it) أرجع;
    + it
    | hb_map (hb_add (هذا))
    | hb_apply ([&] (ثابت PairSet& _) { _.collect_variation_indices (c, بەها_Format_2); })
    ;
  }

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    إذا (unlikely (!(هذا+coverage).collect_coverage (c->تێخستن))) أرجع;
    unsigned صحيح ژماردن = pairSet.len;
    لكل (unsigned صحيح i = 0; i < ژماردن; i++)
      (هذا+pairSet[i]).collect_glyphs (c, بەها_Format_2);
  }

  فراغ collect_second_glyphs (hb_set_digest_t *digest) ثابت
  {
    unsigned ژماردن = pairSet.len;
    لكل (unsigned i = 0; i < ژماردن; i++)
      (هذا+pairSet[i]).collect_second_glyphs (digest, بەها_Format_2);
  }

  ثابت Coverage &get_coverage () ثابت { أرجع هذا+coverage; }

  بنية external_cache_t
  {
    hb_ot_layout_mapping_cache_t coverage;
    hb_set_digest_t pair_sets[HB_VAR_ARRAY];
  };
  فراغ *external_cache_create () ثابت
  {
    unsigned ژماردن = pairSet.len;
    unsigned size = حجم (external_cache_t) -
		    HB_VAR_ARRAY * حجم (hb_set_digest_t) +
		    ژماردن * حجم (hb_set_digest_t);
    external_cache_t *cache = (external_cache_t *) hb_malloc (size);
    إذا (likely (cache))
    {
      cache->coverage.clear ();
      لكل (unsigned i = 0; i < ژماردن; i++)
      {
	cache->pair_sets[i].init ();
	(هذا+pairSet[i]).collect_second_glyphs (&cache->pair_sets[i], بەها_Format_2);
      }
    }
    أرجع cache;
  }

  منطقي apply (hb_ot_apply_context_t *c, فراغ *external_cache) ثابت
  {
    TRACE_APPLY (هذا);

    hb_buffer_t *buffer = c->buffer;

#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    external_cache_t *cache = (external_cache_t *) external_cache;
    unsigned صحيح index = (هذا+coverage).get_coverage  (buffer->cur().codepoint, cache ? &cache->coverage : nullptr);
#وإلا
    unsigned صحيح index = (هذا+coverage).get_coverage  (buffer->cur().codepoint);
#endif
    إذا (index == NOT_COVERED) return_trace (خطأ);

    auto &skippy_iter = c->iter_تێخستن;
    skippy_iter.reset_fast (buffer->idx);
    unsigned unsafe_to;
    إذا (unlikely (!skippy_iter.next (&unsafe_to)))
    {
      buffer->unsafe_to_concat (buffer->idx, unsafe_to);
      return_trace (خطأ);
    }

#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    إذا (cache &&
	index < pairSet.len &&
	!cache->pair_sets[index].may_have (buffer->info[skippy_iter.idx].codepoint))
    {
      buffer->unsafe_to_concat (buffer->idx, skippy_iter.idx + 1);
      return_trace (خطأ);
    }
#endif

    return_trace ((هذا+pairSet[index]).apply (c, بەها_Format_2, skippy_iter.idx));
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);

    ثابت hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (out))) return_trace (خطأ);
    out->format = format;

    hb_pair_t<unsigned, unsigned> newFormats = hb_pair (بەها_Format_2[0], بەها_Format_2[1]);

    إذا (c->plan->normalized_coords)
    {
      /* all device flags will be dropped when full instancing, no need to strip
       * hints, also do not strip emtpy cause we don't compute the new default
       * value during stripping */
      newFormats = compute_effective_بەها_formats (glyphset, خطأ, خطأ, &c->plan->layout_variation_idx_delta_map);
    }
    /* do not strip hints for VF */
    وإلا إذا (c->plan->flags & HB_SUBSET_FLAGS_NO_HINTING)
    {
      hb_blob_t* blob = hb_face_reference_table (c->plan->source, HB_TAG ('f','v','a','r'));
      منطقي has_fvar = (blob != hb_blob_get_empty ());
      hb_blob_destroy (blob);

      منطقي strip = !has_fvar;
      /* special case: strip hints when a VF has no GDEF varstore after
       * subsetting*/
      إذا (has_fvar && !c->plan->has_gdef_varstore)
        strip = true;
      newFormats = compute_effective_بەها_formats (glyphset, strip, true);
    }

    out->بەها_Format_2[0] = newFormats.first;
    out->بەها_Format_2[1] = newFormats.second;

    hb_sorted_vector_t<hb_codepoint_t> new_coverage;

    + hb_zip (هذا+coverage, pairSet)
    | hb_filter (glyphset, hb_first)
    | hb_filter ([هذا, c, out] (ثابت typename Types::template OffsetTo<PairSet>& _)
                 {
                   auto snap = c->serializer->snapshot ();
                   auto *o = out->pairSet.serialize_append (c->serializer);
                   إذا (unlikely (!o)) أرجع خطأ;
                   منطقي ret = o->serialize_subset (c, _, هذا, بەها_Format_2, out->بەها_Format_2);
                   إذا (!ret)
                   {
                     out->pairSet.دەرهێنان ();
                     c->serializer->revert (snap);
                   }
                   أرجع ret;
                 },
                 hb_second)
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    out->coverage.serialize_serialize (c->serializer, new_coverage.iter ());

    return_trace (منطقي (new_coverage));
  }


  hb_pair_t<unsigned, unsigned> compute_effective_بەها_formats (ثابت hb_set_t& glyphset,
                                                                 منطقي strip_hints, منطقي strip_empty,
                                                                 ثابت hb_hashmap_t<unsigned, hb_pair_t<unsigned, صحيح>> *varidx_delta_map = nullptr) ثابت
  {
    unsigned format1 = 0;
    unsigned format2 = 0;
    لكل (ثابت auto & _ :
	  + hb_zip (هذا+coverage, pairSet)
	  | hb_filter (glyphset, hb_first)
	  | hb_map (hb_second)
	)
    {
      ثابت PairSet& set = (هذا + _);
      auto formats = set.get_بەهاکان_view (بەها_Format_2).compute_effective_بەها_formats (
          glyphset, بەها_Format_2, strip_hints, strip_empty, varidx_delta_map);
      format1 |= formats.first;
      format2 |= formats.second;

      إذا (!varidx_delta_map &&
          format1 == بەها_Format_2[0] && format2 == بەها_Format_2[1])
        اكسر;
    }

    أرجع hb_pair (format1, format2);
  }
};


}
}
}

#endif  // OT_LAYOUT_GPOS_PAIRPOSFORMAT1_HH
