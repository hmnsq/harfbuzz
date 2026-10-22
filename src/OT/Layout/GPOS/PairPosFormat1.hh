#که_نه_تعریف_شوی OT_LAYOUT_GPOS_PAIRPOSFORMAT1_HH
#تعریف_کړه OT_LAYOUT_GPOS_PAIRPOSFORMAT1_HH

#ضمن "PairSet.hh"

نطاق OT {
نطاق Layout {
نطاق GPOS_impl {


کينډۍ <ډول_نوم Types>
بنية PairPosFormat1_3
{
  کارول Pair_سېټ = GPOS_impl::Pair_سېټ<Types>;
  کارول Pair_ارزښت_ریکارډ = GPOS_impl::Pair_ارزښت_ریکارډ<Types>;

  محمي:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  ډول_نوم Types::کينډۍ LOffset_منزل<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of subtable */
  ارزښت_Format   ارزښت_Format_2[2];         /* [0] Defines the types of data in
                                         * ValueRecord1--for the first glyph
                                         * in the pair--may be zero (0) */
                                        /* [1] Defines the types of data in
                                         * ValueRecord2--for the second glyph
                                         * in the pair--may be zero (0) */
  ډول_نوم Types::کينډۍ Array_د<ډول_نوم Types::کينډۍ دننه_واټن_منزل<Pair_سېټ>>
                pair_سېټ;                /* Array of PairSet tables ordered by
                                         * Coverage Index */
  عام:
  DEFINE_اندازه_ARRAY (2 + Types::LOffset::static_اندازه + 2 * ارزښت_Format::static_اندازه + Types::HBUINT::static_اندازه, pair_سېټ);

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);

    إذا (!c->check_struct (هذا)) return_trace (خطأ);
    hb_barrier ();

    بینښه صحيح len1 = ارزښت_Format_2[0].get_len ();
    بینښه صحيح len2 = ارزښت_Format_2[1].get_len ();
    ډول_نوم Pair_سېټ::sanitize_تړون_t تړون =
    {
      ارزښت_Format_2,
      len1,
      Pair_سېټ::get_اندازه (len1, len2)
    };

    return_trace (coverage.sanitize (c, هذا) && pair_سېټ.sanitize (c, هذا, &تړون));
  }

  منطقي intersects (ثابت hb_سېټ_t *glyphs) ثابت
  {
    خپلکاری &cov = هذا+coverage;

    إذا (pair_سېټ.len > glyphs->get_population () * hb_بټ_storage ((بینښه) pair_سېټ.len))
    {
      لكل (hb_codepoint_t g : glyphs->iter ())
      {
	بینښه i = cov.get_coverage (g);
	إذا ((هذا+pair_سېټ[i]).intersects (glyphs, ارزښت_Format_2))
	  أرجع ریښتیا;
      }
      أرجع خطأ;
    }

    أرجع
    + hb_zip (cov, pair_سېټ)
    | hb_filter (*glyphs, hb_لومړی)
    | hb_map (hb_دوهم)
    | hb_map ([glyphs, هذا] (ثابت ډول_نوم Types::کينډۍ دننه_واټن_منزل<Pair_سېټ> &_)
              { أرجع (هذا+_).intersects (glyphs, ارزښت_Format_2); })
    | hb_any
    ;
  }

  فراغ تړون_lookups (hb_تړون_lookups_context_t *c) ثابت {}
  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    إذا ((!ارزښت_Format_2[0].has_device ()) && (!ارزښت_Format_2[1].has_device ())) أرجع;

    خپلکاری it =
    + hb_zip (هذا+coverage, pair_سېټ)
    | hb_filter (c->glyph_سېټ, hb_لومړی)
    | hb_map (hb_دوهم)
    ;

    إذا (!it) أرجع;
    + it
    | hb_map (hb_جمع_کړه (هذا))
    | hb_apply ([&] (ثابت Pair_سېټ& _) { _.collect_variation_indices (c, ارزښت_Format_2); })
    ;
  }

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    إذا (unlikely (!(هذا+coverage).collect_coverage (c->ننوت))) أرجع;
    بینښه صحيح شمېر = pair_سېټ.len;
    لكل (بینښه صحيح i = 0; i < شمېر; i++)
      (هذا+pair_سېټ[i]).collect_glyphs (c, ارزښت_Format_2);
  }

  فراغ collect_دوهم_glyphs (hb_سېټ_digest_t *digest) ثابت
  {
    بینښه شمېر = pair_سېټ.len;
    لكل (بینښه i = 0; i < شمېر; i++)
      (هذا+pair_سېټ[i]).collect_دوهم_glyphs (digest, ارزښت_Format_2);
  }

  ثابت Coverage &get_coverage () ثابت { أرجع هذا+coverage; }

  بنية external_cache_t
  {
    hb_ot_layout_mapping_cache_t coverage;
    hb_سېټ_digest_t pair_sets[HB_VAR_ARRAY];
  };
  فراغ *external_cache_create () ثابت
  {
    بینښه شمېر = pair_سېټ.len;
    بینښه اندازه = حجم (external_cache_t) -
		    HB_VAR_ARRAY * حجم (hb_سېټ_digest_t) +
		    شمېر * حجم (hb_سېټ_digest_t);
    external_cache_t *cache = (external_cache_t *) hb_malloc (اندازه);
    إذا (likely (cache))
    {
      cache->coverage.clear ();
      لكل (بینښه i = 0; i < شمېر; i++)
      {
	cache->pair_sets[i].init ();
	(هذا+pair_سېټ[i]).collect_دوهم_glyphs (&cache->pair_sets[i], ارزښت_Format_2);
      }
    }
    أرجع cache;
  }

  منطقي apply (hb_ot_apply_context_t *c, فراغ *external_cache) ثابت
  {
    TRACE_APPLY (هذا);

    hb_بفر_t *بفر = c->بفر;

#که_نه_تعریف_شوی HB_NO_OT_LAYOUT_LOOKUP_CACHE
    external_cache_t *cache = (external_cache_t *) external_cache;
    بینښه صحيح شاخص_2 = (هذا+coverage).get_coverage  (بفر->cur().codepoint, cache ? &cache->coverage : تش_اشاره);
#وإلا
    بینښه صحيح شاخص_2 = (هذا+coverage).get_coverage  (بفر->cur().codepoint);
#د_که_پای
    إذا (شاخص_2 == NOT_COVERED) return_trace (خطأ);

    خپلکاری &skippy_iter = c->iter_ننوت;
    skippy_iter.reset_fast (بفر->idx);
    بینښه unsafe_منزل;
    إذا (unlikely (!skippy_iter.next (&unsafe_منزل)))
    {
      بفر->unsafe_منزل_concat (بفر->idx, unsafe_منزل);
      return_trace (خطأ);
    }

#که_نه_تعریف_شوی HB_NO_OT_LAYOUT_LOOKUP_CACHE
    إذا (cache &&
	شاخص_2 < pair_سېټ.len &&
	!cache->pair_sets[شاخص_2].may_have (بفر->info[skippy_iter.idx].codepoint))
    {
      بفر->unsafe_منزل_concat (بفر->idx, skippy_iter.idx + 1);
      return_trace (خطأ);
    }
#د_که_پای

    return_trace ((هذا+pair_سېټ[شاخص_2]).apply (c, ارزښت_Format_2, skippy_iter.idx));
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);

    ثابت hb_سېټ_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    خپلکاری *بهر = c->serializer->پیل_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (بهر))) return_trace (خطأ);
    بهر->format = format;

    hb_pair_t<بینښه, بینښه> newFormats = hb_pair (ارزښت_Format_2[0], ارزښت_Format_2[1]);

    إذا (c->plan->normalized_coords)
    {
      /* all device flags will be dropped when full instancing, no need to strip
       * hints, also do not strip emtpy cause we don't compute the new default
       * value during stripping */
      newFormats = compute_effective_ارزښت_formats (glyphset, خطأ, خطأ, &c->plan->layout_variation_idx_delta_map);
    }
    /* do not strip hints for VF */
    وإلا إذا (c->plan->flags & HB_SUBSET_FLAGS_NO_HINTING)
    {
      hb_blob_t* blob = hb_face_reference_جدول (c->plan->سرچینه, HB_نښان ('f','v','a','r'));
      منطقي has_fvar = (blob != hb_blob_get_تشه ());
      hb_blob_destroy (blob);

      منطقي strip = !has_fvar;
      /* special case: strip hints when a VF has no GDEF varstore after
       * subsetting*/
      إذا (has_fvar && !c->plan->has_gdef_varstore)
        strip = ریښتیا;
      newFormats = compute_effective_ارزښت_formats (glyphset, strip, ریښتیا);
    }

    بهر->ارزښت_Format_2[0] = newFormats.لومړی;
    بهر->ارزښت_Format_2[1] = newFormats.second;

    hb_sorted_ویکټور_t<hb_codepoint_t> new_coverage;

    + hb_zip (هذا+coverage, pair_سېټ)
    | hb_filter (glyphset, hb_لومړی)
    | hb_filter ([هذا, c, بهر] (ثابت ډول_نوم Types::کينډۍ دننه_واټن_منزل<Pair_سېټ>& _)
                 {
                   خپلکاری snap = c->serializer->snapshot ();
                   خپلکاری *o = بهر->pair_سېټ.serialize_append (c->serializer);
                   إذا (unlikely (!o)) أرجع خطأ;
                   منطقي ret = o->serialize_subset (c, _, هذا, ارزښت_Format_2, بهر->ارزښت_Format_2);
                   إذا (!ret)
                   {
                     بهر->pair_سېټ.راوباسه ();
                     c->serializer->revert (snap);
                   }
                   أرجع ret;
                 },
                 hb_دوهم)
    | hb_map (hb_لومړی)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    بهر->coverage.serialize_serialize (c->serializer, new_coverage.iter ());

    return_trace (منطقي (new_coverage));
  }


  hb_pair_t<بینښه, بینښه> compute_effective_ارزښت_formats (ثابت hb_سېټ_t& glyphset,
                                                                 منطقي strip_hints, منطقي strip_تشه,
                                                                 ثابت hb_hashmap_t<بینښه, hb_pair_t<بینښه, صحيح>> *varidx_delta_map = تش_اشاره) ثابت
  {
    بینښه format1 = 0;
    بینښه format2 = 0;
    لكل (ثابت خپلکاری & _ :
	  + hb_zip (هذا+coverage, pair_سېټ)
	  | hb_filter (glyphset, hb_لومړی)
	  | hb_map (hb_دوهم)
	)
    {
      ثابت Pair_سېټ& سېټ = (هذا + _);
      خپلکاری formats = سېټ.get_ارزښتونه_view (ارزښت_Format_2).compute_effective_ارزښت_formats (
          glyphset, ارزښت_Format_2, strip_hints, strip_تشه, varidx_delta_map);
      format1 |= formats.لومړی;
      format2 |= formats.second;

      إذا (!varidx_delta_map &&
          format1 == ارزښت_Format_2[0] && format2 == ارزښت_Format_2[1])
        اكسر;
    }

    أرجع hb_pair (format1, format2);
  }
};


}
}
}

#د_که_پای  // OT_LAYOUT_GPOS_PAIRPOSFORMAT1_HH
