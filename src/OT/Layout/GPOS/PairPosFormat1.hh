#جے_بنا_تعریف OT_LAYOUT_GPOS_PAIRPOSFORMAT1_HH
#تعریف_کرو OT_LAYOUT_GPOS_PAIRPOSFORMAT1_HH

#ضمن "PairSet.hh"

نطاق OT {
نطاق Layout {
نطاق GPOS_impl {


سانچا <قسم_ناں Types>
بنية PairPosFormat1_3
{
  ورتوں Pair_سیٹ = GPOS_impl::Pair_سیٹ<Types>;
  ورتوں Pair_قدر_ریکارڈ = GPOS_impl::Pair_قدر_ریکارڈ<Types>;

  محمي:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  قسم_ناں Types::سانچا LOffset_منزل<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of subtable */
  قدر_Format   قدر_Format_2[2];         /* [0] Defines the types of data in
                                         * ValueRecord1--for the first glyph
                                         * in the pair--may be zero (0) */
                                        /* [1] Defines the types of data in
                                         * ValueRecord2--for the second glyph
                                         * in the pair--may be zero (0) */
  قسم_ناں Types::سانچا Array_دا<قسم_ناں Types::سانچا وتھ_منزل<Pair_سیٹ>>
                pair_سیٹ;                /* Array of PairSet tables ordered by
                                         * Coverage Index */
  عام:
  DEFINE_ناپ_ARRAY (2 + Types::LOffset::static_ناپ + 2 * قدر_Format::static_ناپ + Types::HBUINT::static_ناپ, pair_سیٹ);

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);

    إذا (!c->check_struct (هذا)) return_trace (خطأ);
    hb_barrier ();

    بنانشانی صحيح len1 = قدر_Format_2[0].get_len ();
    بنانشانی صحيح len2 = قدر_Format_2[1].get_len ();
    قسم_ناں Pair_سیٹ::sanitize_گھیرا_t گھیرا =
    {
      قدر_Format_2,
      len1,
      Pair_سیٹ::get_ناپ (len1, len2)
    };

    return_trace (coverage.sanitize (c, هذا) && pair_سیٹ.sanitize (c, هذا, &گھیرا));
  }

  منطقي intersects (ثابت hb_سیٹ_t *glyphs) ثابت
  {
    خودکار &cov = هذا+coverage;

    إذا (pair_سیٹ.len > glyphs->get_population () * hb_ثنائی_ہندسہ_storage ((بنانشانی) pair_سیٹ.len))
    {
      لكل (hb_codepoint_t g : glyphs->iter ())
      {
	بنانشانی i = cov.get_coverage (g);
	إذا ((هذا+pair_سیٹ[i]).intersects (glyphs, قدر_Format_2))
	  أرجع سچ;
      }
      أرجع خطأ;
    }

    أرجع
    + hb_zip (cov, pair_سیٹ)
    | hb_filter (*glyphs, hb_پہلا)
    | hb_map (hb_دوجا)
    | hb_map ([glyphs, هذا] (ثابت قسم_ناں Types::سانچا وتھ_منزل<Pair_سیٹ> &_)
              { أرجع (هذا+_).intersects (glyphs, قدر_Format_2); })
    | hb_any
    ;
  }

  فراغ گھیرا_lookups (hb_گھیرا_lookups_context_t *c) ثابت {}
  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    إذا ((!قدر_Format_2[0].has_device ()) && (!قدر_Format_2[1].has_device ())) أرجع;

    خودکار it =
    + hb_zip (هذا+coverage, pair_سیٹ)
    | hb_filter (c->glyph_سیٹ, hb_پہلا)
    | hb_map (hb_دوجا)
    ;

    إذا (!it) أرجع;
    + it
    | hb_map (hb_جوڑو (هذا))
    | hb_apply ([&] (ثابت Pair_سیٹ& _) { _.collect_variation_indices (c, قدر_Format_2); })
    ;
  }

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    إذا (unlikely (!(هذا+coverage).collect_coverage (c->داخلہ))) أرجع;
    بنانشانی صحيح گنتی = pair_سیٹ.len;
    لكل (بنانشانی صحيح i = 0; i < گنتی; i++)
      (هذا+pair_سیٹ[i]).collect_glyphs (c, قدر_Format_2);
  }

  فراغ collect_دوجا_glyphs (hb_سیٹ_digest_t *digest) ثابت
  {
    بنانشانی گنتی = pair_سیٹ.len;
    لكل (بنانشانی i = 0; i < گنتی; i++)
      (هذا+pair_سیٹ[i]).collect_دوجا_glyphs (digest, قدر_Format_2);
  }

  ثابت Coverage &get_coverage () ثابت { أرجع هذا+coverage; }

  بنية external_cache_t
  {
    hb_ot_layout_mapping_cache_t coverage;
    hb_سیٹ_digest_t pair_sets[HB_VAR_ARRAY];
  };
  فراغ *external_cache_create () ثابت
  {
    بنانشانی گنتی = pair_سیٹ.len;
    بنانشانی ناپ = حجم (external_cache_t) -
		    HB_VAR_ARRAY * حجم (hb_سیٹ_digest_t) +
		    گنتی * حجم (hb_سیٹ_digest_t);
    external_cache_t *cache = (external_cache_t *) hb_malloc (ناپ);
    إذا (likely (cache))
    {
      cache->coverage.clear ();
      لكل (بنانشانی i = 0; i < گنتی; i++)
      {
	cache->pair_sets[i].init ();
	(هذا+pair_سیٹ[i]).collect_دوجا_glyphs (&cache->pair_sets[i], قدر_Format_2);
      }
    }
    أرجع cache;
  }

  منطقي apply (hb_ot_apply_context_t *c, فراغ *external_cache) ثابت
  {
    TRACE_APPLY (هذا);

    hb_بفر_t *بفر = c->بفر;

#جے_بنا_تعریف HB_NO_OT_LAYOUT_LOOKUP_CACHE
    external_cache_t *cache = (external_cache_t *) external_cache;
    بنانشانی صحيح اشاریہ_2 = (هذا+coverage).get_coverage  (بفر->cur().codepoint, cache ? &cache->coverage : خالی_سوچک);
#وإلا
    بنانشانی صحيح اشاریہ_2 = (هذا+coverage).get_coverage  (بفر->cur().codepoint);
#جے_انت
    إذا (اشاریہ_2 == NOT_COVERED) return_trace (خطأ);

    خودکار &skippy_iter = c->iter_داخلہ;
    skippy_iter.reset_fast (بفر->idx);
    بنانشانی unsafe_منزل;
    إذا (unlikely (!skippy_iter.next (&unsafe_منزل)))
    {
      بفر->unsafe_منزل_concat (بفر->idx, unsafe_منزل);
      return_trace (خطأ);
    }

#جے_بنا_تعریف HB_NO_OT_LAYOUT_LOOKUP_CACHE
    إذا (cache &&
	اشاریہ_2 < pair_سیٹ.len &&
	!cache->pair_sets[اشاریہ_2].may_have (بفر->info[skippy_iter.idx].codepoint))
    {
      بفر->unsafe_منزل_concat (بفر->idx, skippy_iter.idx + 1);
      return_trace (خطأ);
    }
#جے_انت

    return_trace ((هذا+pair_سیٹ[اشاریہ_2]).apply (c, قدر_Format_2, skippy_iter.idx));
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);

    ثابت hb_سیٹ_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    خودکار *باہرلا = c->serializer->شروع_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (باہرلا))) return_trace (خطأ);
    باہرلا->format = format;

    hb_pair_t<بنانشانی, بنانشانی> newFormats = hb_pair (قدر_Format_2[0], قدر_Format_2[1]);

    إذا (c->plan->normalized_coords)
    {
      /* all device flags will be dropped when full instancing, no need to strip
       * hints, also do not strip emtpy cause we don't compute the new default
       * value during stripping */
      newFormats = compute_effective_قدر_formats (glyphset, خطأ, خطأ, &c->plan->layout_variation_idx_delta_map);
    }
    /* do not strip hints for VF */
    وإلا إذا (c->plan->flags & HB_SUBSET_FLAGS_NO_HINTING)
    {
      hb_blob_t* blob = hb_face_reference_جدول (c->plan->سروت, HB_ٹیگ ('f','v','a','r'));
      منطقي has_fvar = (blob != hb_blob_get_خالی_تھاواں ());
      hb_blob_destroy (blob);

      منطقي strip = !has_fvar;
      /* special case: strip hints when a VF has no GDEF varstore after
       * subsetting*/
      إذا (has_fvar && !c->plan->has_gdef_varstore)
        strip = سچ;
      newFormats = compute_effective_قدر_formats (glyphset, strip, سچ);
    }

    باہرلا->قدر_Format_2[0] = newFormats.پہلا;
    باہرلا->قدر_Format_2[1] = newFormats.second;

    hb_sorted_سمتیہ_t<hb_codepoint_t> new_coverage;

    + hb_zip (هذا+coverage, pair_سیٹ)
    | hb_filter (glyphset, hb_پہلا)
    | hb_filter ([هذا, c, باہرلا] (ثابت قسم_ناں Types::سانچا وتھ_منزل<Pair_سیٹ>& _)
                 {
                   خودکار snap = c->serializer->snapshot ();
                   خودکار *o = باہرلا->pair_سیٹ.serialize_append (c->serializer);
                   إذا (unlikely (!o)) أرجع خطأ;
                   منطقي ret = o->serialize_subset (c, _, هذا, قدر_Format_2, باہرلا->قدر_Format_2);
                   إذا (!ret)
                   {
                     باہرلا->pair_سیٹ.نکالو ();
                     c->serializer->revert (snap);
                   }
                   أرجع ret;
                 },
                 hb_دوجا)
    | hb_map (hb_پہلا)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    باہرلا->coverage.serialize_serialize (c->serializer, new_coverage.iter ());

    return_trace (منطقي (new_coverage));
  }


  hb_pair_t<بنانشانی, بنانشانی> compute_effective_قدر_formats (ثابت hb_سیٹ_t& glyphset,
                                                                 منطقي strip_hints, منطقي strip_خالی_تھاواں,
                                                                 ثابت hb_hashmap_t<بنانشانی, hb_pair_t<بنانشانی, صحيح>> *varidx_delta_map = خالی_سوچک) ثابت
  {
    بنانشانی format1 = 0;
    بنانشانی format2 = 0;
    لكل (ثابت خودکار & _ :
	  + hb_zip (هذا+coverage, pair_سیٹ)
	  | hb_filter (glyphset, hb_پہلا)
	  | hb_map (hb_دوجا)
	)
    {
      ثابت Pair_سیٹ& سیٹ = (هذا + _);
      خودکار formats = سیٹ.get_اقدار_view (قدر_Format_2).compute_effective_قدر_formats (
          glyphset, قدر_Format_2, strip_hints, strip_خالی_تھاواں, varidx_delta_map);
      format1 |= formats.پہلا;
      format2 |= formats.second;

      إذا (!varidx_delta_map &&
          format1 == قدر_Format_2[0] && format2 == قدر_Format_2[1])
        اكسر;
    }

    أرجع hb_pair (format1, format2);
  }
};


}
}
}

#جے_انت  // OT_LAYOUT_GPOS_PAIRPOSFORMAT1_HH
