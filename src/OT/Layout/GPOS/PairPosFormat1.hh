#جيڪڏهن_وصف_نه_ٿيل OT_LAYOUT_GPOS_PAIRPOSFORMAT1_HH
#وصف_ڪريو OT_LAYOUT_GPOS_PAIRPOSFORMAT1_HH

#ضمن "PairSet.hh"

نطاق OT {
نطاق Layout {
نطاق GPOS_impl {


سانچو <قسم_نالو Types>
بنية PairPosFormat1_3
{
  استعمال Pair_سيٽ = GPOS_impl::Pair_سيٽ<Types>;
  استعمال Pair_قدر_رڪارڊ = GPOS_impl::Pair_قدر_رڪارڊ<Types>;

  محمي:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  قسم_نالو Types::سانچو LOffset_منزل<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of subtable */
  قدر_Format   قدر_Format_2[2];         /* [0] Defines the types of data in
                                         * ValueRecord1--for the first glyph
                                         * in the pair--may be zero (0) */
                                        /* [1] Defines the types of data in
                                         * ValueRecord2--for the second glyph
                                         * in the pair--may be zero (0) */
  قسم_نالو Types::سانچو Array_جو<قسم_نالو Types::سانچو وٿي_منزل<Pair_سيٽ>>
                pair_سيٽ;                /* Array of PairSet tables ordered by
                                         * Coverage Index */
  عام:
  DEFINE_سائيز_ARRAY (2 + Types::LOffset::static_سائيز + 2 * قدر_Format::static_سائيز + Types::HBUINT::static_سائيز, pair_سيٽ);

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);

    إذا (!c->check_struct (هذا)) return_trace (خطأ);
    hb_barrier ();

    بنانشان صحيح len1 = قدر_Format_2[0].get_len ();
    بنانشان صحيح len2 = قدر_Format_2[1].get_len ();
    قسم_نالو Pair_سيٽ::sanitize_گهيرو_t گهيرو =
    {
      قدر_Format_2,
      len1,
      Pair_سيٽ::get_سائيز (len1, len2)
    };

    return_trace (coverage.sanitize (c, هذا) && pair_سيٽ.sanitize (c, هذا, &گهيرو));
  }

  منطقي intersects (ثابت hb_سيٽ_t *glyphs) ثابت
  {
    خودڪار &cov = هذا+coverage;

    إذا (pair_سيٽ.len > glyphs->get_population () * hb_ٻٽو_انگ_storage ((بنانشان) pair_سيٽ.len))
    {
      لكل (hb_codepoint_t g : glyphs->iter ())
      {
	بنانشان i = cov.get_coverage (g);
	إذا ((هذا+pair_سيٽ[i]).intersects (glyphs, قدر_Format_2))
	  أرجع سچ;
      }
      أرجع خطأ;
    }

    أرجع
    + hb_zip (cov, pair_سيٽ)
    | hb_filter (*glyphs, hb_پهريون)
    | hb_map (hb_ٻيو)
    | hb_map ([glyphs, هذا] (ثابت قسم_نالو Types::سانچو وٿي_منزل<Pair_سيٽ> &_)
              { أرجع (هذا+_).intersects (glyphs, قدر_Format_2); })
    | hb_any
    ;
  }

  فراغ گهيرو_lookups (hb_گهيرو_lookups_context_t *c) ثابت {}
  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    إذا ((!قدر_Format_2[0].has_device ()) && (!قدر_Format_2[1].has_device ())) أرجع;

    خودڪار it =
    + hb_zip (هذا+coverage, pair_سيٽ)
    | hb_filter (c->glyph_سيٽ, hb_پهريون)
    | hb_map (hb_ٻيو)
    ;

    إذا (!it) أرجع;
    + it
    | hb_map (hb_جوڙيو (هذا))
    | hb_apply ([&] (ثابت Pair_سيٽ& _) { _.collect_variation_indices (c, قدر_Format_2); })
    ;
  }

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    إذا (unlikely (!(هذا+coverage).collect_coverage (c->داخلا_2))) أرجع;
    بنانشان صحيح ڳڻپ = pair_سيٽ.len;
    لكل (بنانشان صحيح i = 0; i < ڳڻپ; i++)
      (هذا+pair_سيٽ[i]).collect_glyphs (c, قدر_Format_2);
  }

  فراغ collect_ٻيو_glyphs (hb_سيٽ_digest_t *digest) ثابت
  {
    بنانشان ڳڻپ = pair_سيٽ.len;
    لكل (بنانشان i = 0; i < ڳڻپ; i++)
      (هذا+pair_سيٽ[i]).collect_ٻيو_glyphs (digest, قدر_Format_2);
  }

  ثابت Coverage &get_coverage () ثابت { أرجع هذا+coverage; }

  بنية external_cache_t
  {
    hb_ot_layout_mapping_cache_t coverage;
    hb_سيٽ_digest_t pair_sets[HB_VAR_ARRAY];
  };
  فراغ *external_cache_create () ثابت
  {
    بنانشان ڳڻپ = pair_سيٽ.len;
    بنانشان سائيز = حجم (external_cache_t) -
		    HB_VAR_ARRAY * حجم (hb_سيٽ_digest_t) +
		    ڳڻپ * حجم (hb_سيٽ_digest_t);
    external_cache_t *cache = (external_cache_t *) hb_malloc (سائيز);
    إذا (likely (cache))
    {
      cache->coverage.clear ();
      لكل (بنانشان i = 0; i < ڳڻپ; i++)
      {
	cache->pair_sets[i].init ();
	(هذا+pair_سيٽ[i]).collect_ٻيو_glyphs (&cache->pair_sets[i], قدر_Format_2);
      }
    }
    أرجع cache;
  }

  منطقي apply (hb_ot_apply_context_t *c, فراغ *external_cache) ثابت
  {
    TRACE_APPLY (هذا);

    hb_بفر_t *بفر = c->بفر;

#جيڪڏهن_وصف_نه_ٿيل HB_NO_OT_LAYOUT_LOOKUP_CACHE
    external_cache_t *cache = (external_cache_t *) external_cache;
    بنانشان صحيح اشاريو_2 = (هذا+coverage).get_coverage  (بفر->cur().codepoint, cache ? &cache->coverage : خالي_اشارو);
#وإلا
    بنانشان صحيح اشاريو_2 = (هذا+coverage).get_coverage  (بفر->cur().codepoint);
#جيڪڏهن_پورو
    إذا (اشاريو_2 == NOT_COVERED) return_trace (خطأ);

    خودڪار &skippy_iter = c->iter_داخلا;
    skippy_iter.reset_fast (بفر->idx);
    بنانشان unsafe_منزل;
    إذا (unlikely (!skippy_iter.next (&unsafe_منزل)))
    {
      بفر->unsafe_منزل_concat (بفر->idx, unsafe_منزل);
      return_trace (خطأ);
    }

#جيڪڏهن_وصف_نه_ٿيل HB_NO_OT_LAYOUT_LOOKUP_CACHE
    إذا (cache &&
	اشاريو_2 < pair_سيٽ.len &&
	!cache->pair_sets[اشاريو_2].may_have (بفر->info[skippy_iter.idx].codepoint))
    {
      بفر->unsafe_منزل_concat (بفر->idx, skippy_iter.idx + 1);
      return_trace (خطأ);
    }
#جيڪڏهن_پورو

    return_trace ((هذا+pair_سيٽ[اشاريو_2]).apply (c, قدر_Format_2, skippy_iter.idx));
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);

    ثابت hb_سيٽ_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    خودڪار *ٻاهريون = c->serializer->شروعات_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (ٻاهريون))) return_trace (خطأ);
    ٻاهريون->format = format;

    hb_pair_t<بنانشان, بنانشان> newFormats = hb_pair (قدر_Format_2[0], قدر_Format_2[1]);

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
      hb_blob_t* blob = hb_face_reference_جدول (c->plan->ذريعو, HB_ٽيگ ('f','v','a','r'));
      منطقي has_fvar = (blob != hb_blob_get_خالي_جايون ());
      hb_blob_destroy (blob);

      منطقي strip = !has_fvar;
      /* special case: strip hints when a VF has no GDEF varstore after
       * subsetting*/
      إذا (has_fvar && !c->plan->has_gdef_varstore)
        strip = سچ;
      newFormats = compute_effective_قدر_formats (glyphset, strip, سچ);
    }

    ٻاهريون->قدر_Format_2[0] = newFormats.پهريون;
    ٻاهريون->قدر_Format_2[1] = newFormats.second;

    hb_sorted_ويڪٽر_t<hb_codepoint_t> new_coverage;

    + hb_zip (هذا+coverage, pair_سيٽ)
    | hb_filter (glyphset, hb_پهريون)
    | hb_filter ([هذا, c, ٻاهريون] (ثابت قسم_نالو Types::سانچو وٿي_منزل<Pair_سيٽ>& _)
                 {
                   خودڪار snap = c->serializer->snapshot ();
                   خودڪار *o = ٻاهريون->pair_سيٽ.serialize_append (c->serializer);
                   إذا (unlikely (!o)) أرجع خطأ;
                   منطقي ret = o->serialize_subset (c, _, هذا, قدر_Format_2, ٻاهريون->قدر_Format_2);
                   إذا (!ret)
                   {
                     ٻاهريون->pair_سيٽ.ڪڍو ();
                     c->serializer->revert (snap);
                   }
                   أرجع ret;
                 },
                 hb_ٻيو)
    | hb_map (hb_پهريون)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    ٻاهريون->coverage.serialize_serialize (c->serializer, new_coverage.iter ());

    return_trace (منطقي (new_coverage));
  }


  hb_pair_t<بنانشان, بنانشان> compute_effective_قدر_formats (ثابت hb_سيٽ_t& glyphset,
                                                                 منطقي strip_hints, منطقي strip_خالي_جايون,
                                                                 ثابت hb_hashmap_t<بنانشان, hb_pair_t<بنانشان, صحيح>> *varidx_delta_map = خالي_اشارو) ثابت
  {
    بنانشان format1 = 0;
    بنانشان format2 = 0;
    لكل (ثابت خودڪار & _ :
	  + hb_zip (هذا+coverage, pair_سيٽ)
	  | hb_filter (glyphset, hb_پهريون)
	  | hb_map (hb_ٻيو)
	)
    {
      ثابت Pair_سيٽ& سيٽ = (هذا + _);
      خودڪار formats = سيٽ.get_قدرون_view (قدر_Format_2).compute_effective_قدر_formats (
          glyphset, قدر_Format_2, strip_hints, strip_خالي_جايون, varidx_delta_map);
      format1 |= formats.پهريون;
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

#جيڪڏهن_پورو  // OT_LAYOUT_GPOS_PAIRPOSFORMAT1_HH
