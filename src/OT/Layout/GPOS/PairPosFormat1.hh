#اگر_تعریف_نشده OT_LAYOUT_GPOS_PAIRPOSFORMAT1_HH
#تعریف_کن OT_LAYOUT_GPOS_PAIRPOSFORMAT1_HH

#ضمن "PairSet.hh"

نطاق OT {
نطاق Layout {
نطاق GPOS_impl {


قالب <نام_نوع Types>
بنية PairPosFormat1_3
{
  استفاده Pair_مجموعه = GPOS_impl::Pair_مجموعه<Types>;
  استفاده Pair_مقدار_رکورد = GPOS_impl::Pair_مقدار_رکورد<Types>;

  محمي:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  نام_نوع Types::قالب LOffset_مقصد<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of subtable */
  مقدار_Format   مقدار_Format_2[2];         /* [0] Defines the types of data in
                                         * ValueRecord1--for the first glyph
                                         * in the pair--may be zero (0) */
                                        /* [1] Defines the types of data in
                                         * ValueRecord2--for the second glyph
                                         * in the pair--may be zero (0) */
  نام_نوع Types::قالب Array_از<نام_نوع Types::قالب جابه_جایی_مقصد<Pair_مجموعه>>
                pair_مجموعه;                /* Array of PairSet tables ordered by
                                         * Coverage Index */
  عام:
  DEFINE_اندازه_ARRAY (2 + Types::LOffset::static_اندازه + 2 * مقدار_Format::static_اندازه + Types::HBUINT::static_اندازه, pair_مجموعه);

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);

    إذا (!c->check_struct (هذا)) return_trace (خطأ);
    hb_barrier ();

    بیعلامت صحيح len1 = مقدار_Format_2[0].get_len ();
    بیعلامت صحيح len2 = مقدار_Format_2[1].get_len ();
    نام_نوع Pair_مجموعه::sanitize_بستگی_t بستگی =
    {
      مقدار_Format_2,
      len1,
      Pair_مجموعه::get_اندازه (len1, len2)
    };

    return_trace (coverage.sanitize (c, هذا) && pair_مجموعه.sanitize (c, هذا, &بستگی));
  }

  منطقي intersects (ثابت hb_مجموعه_t *glyphs) ثابت
  {
    خودکار &cov = هذا+coverage;

    إذا (pair_مجموعه.len > glyphs->get_population () * hb_بیت_storage ((بیعلامت) pair_مجموعه.len))
    {
      لكل (hb_codepoint_t g : glyphs->iter ())
      {
	بیعلامت i = cov.get_coverage (g);
	إذا ((هذا+pair_مجموعه[i]).intersects (glyphs, مقدار_Format_2))
	  أرجع درست;
      }
      أرجع خطأ;
    }

    أرجع
    + hb_zip (cov, pair_مجموعه)
    | hb_filter (*glyphs, hb_اولی)
    | hb_map (hb_دومی)
    | hb_map ([glyphs, هذا] (ثابت نام_نوع Types::قالب جابه_جایی_مقصد<Pair_مجموعه> &_)
              { أرجع (هذا+_).intersects (glyphs, مقدار_Format_2); })
    | hb_any
    ;
  }

  فراغ بستگی_lookups (hb_بستگی_lookups_context_t *c) ثابت {}
  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    إذا ((!مقدار_Format_2[0].has_device ()) && (!مقدار_Format_2[1].has_device ())) أرجع;

    خودکار it =
    + hb_zip (هذا+coverage, pair_مجموعه)
    | hb_filter (c->glyph_مجموعه, hb_اولی)
    | hb_map (hb_دومی)
    ;

    إذا (!it) أرجع;
    + it
    | hb_map (hb_جمع_کن (هذا))
    | hb_apply ([&] (ثابت Pair_مجموعه& _) { _.collect_variation_indices (c, مقدار_Format_2); })
    ;
  }

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    إذا (unlikely (!(هذا+coverage).collect_coverage (c->درون_داد))) أرجع;
    بیعلامت صحيح شمار = pair_مجموعه.len;
    لكل (بیعلامت صحيح i = 0; i < شمار; i++)
      (هذا+pair_مجموعه[i]).collect_glyphs (c, مقدار_Format_2);
  }

  فراغ collect_دومی_glyphs (hb_مجموعه_digest_t *digest) ثابت
  {
    بیعلامت شمار = pair_مجموعه.len;
    لكل (بیعلامت i = 0; i < شمار; i++)
      (هذا+pair_مجموعه[i]).collect_دومی_glyphs (digest, مقدار_Format_2);
  }

  ثابت Coverage &get_coverage () ثابت { أرجع هذا+coverage; }

  بنية external_cache_t
  {
    hb_ot_layout_mapping_cache_t coverage;
    hb_مجموعه_digest_t pair_sets[HB_VAR_ARRAY];
  };
  فراغ *external_cache_create () ثابت
  {
    بیعلامت شمار = pair_مجموعه.len;
    بیعلامت اندازه = حجم (external_cache_t) -
		    HB_VAR_ARRAY * حجم (hb_مجموعه_digest_t) +
		    شمار * حجم (hb_مجموعه_digest_t);
    external_cache_t *cache = (external_cache_t *) hb_malloc (اندازه);
    إذا (likely (cache))
    {
      cache->coverage.clear ();
      لكل (بیعلامت i = 0; i < شمار; i++)
      {
	cache->pair_sets[i].init ();
	(هذا+pair_مجموعه[i]).collect_دومی_glyphs (&cache->pair_sets[i], مقدار_Format_2);
      }
    }
    أرجع cache;
  }

  منطقي apply (hb_ot_apply_context_t *c, فراغ *external_cache) ثابت
  {
    TRACE_APPLY (هذا);

    hb_میانگیر_t *میانگیر = c->میانگیر;

#اگر_تعریف_نشده HB_NO_OT_LAYOUT_LOOKUP_CACHE
    external_cache_t *cache = (external_cache_t *) external_cache;
    بیعلامت صحيح شاخص_2 = (هذا+coverage).get_coverage  (میانگیر->cur().codepoint, cache ? &cache->coverage : اشاره_تهی);
#وإلا
    بیعلامت صحيح شاخص_2 = (هذا+coverage).get_coverage  (میانگیر->cur().codepoint);
#پایان_اگر
    إذا (شاخص_2 == NOT_COVERED) return_trace (خطأ);

    خودکار &skippy_iter = c->iter_درون_داد;
    skippy_iter.reset_fast (میانگیر->idx);
    بیعلامت unsafe_مقصد;
    إذا (unlikely (!skippy_iter.next (&unsafe_مقصد)))
    {
      میانگیر->unsafe_مقصد_concat (میانگیر->idx, unsafe_مقصد);
      return_trace (خطأ);
    }

#اگر_تعریف_نشده HB_NO_OT_LAYOUT_LOOKUP_CACHE
    إذا (cache &&
	شاخص_2 < pair_مجموعه.len &&
	!cache->pair_sets[شاخص_2].may_have (میانگیر->info[skippy_iter.idx].codepoint))
    {
      میانگیر->unsafe_مقصد_concat (میانگیر->idx, skippy_iter.idx + 1);
      return_trace (خطأ);
    }
#پایان_اگر

    return_trace ((هذا+pair_مجموعه[شاخص_2]).apply (c, مقدار_Format_2, skippy_iter.idx));
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);

    ثابت hb_مجموعه_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    خودکار *بیرون = c->serializer->شروع_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (بیرون))) return_trace (خطأ);
    بیرون->format = format;

    hb_pair_t<بیعلامت, بیعلامت> newFormats = hb_pair (مقدار_Format_2[0], مقدار_Format_2[1]);

    إذا (c->plan->normalized_coords)
    {
      /* all device flags will be dropped when full instancing, no need to strip
       * hints, also do not strip emtpy cause we don't compute the new default
       * value during stripping */
      newFormats = compute_effective_مقدار_formats (glyphset, خطأ, خطأ, &c->plan->layout_variation_idx_delta_map);
    }
    /* do not strip hints for VF */
    وإلا إذا (c->plan->flags & HB_SUBSET_FLAGS_NO_HINTING)
    {
      hb_blob_t* blob = hb_face_reference_جدول (c->plan->منبع, HB_برچسب ('f','v','a','r'));
      منطقي has_fvar = (blob != hb_blob_get_خالی ());
      hb_blob_destroy (blob);

      منطقي strip = !has_fvar;
      /* special case: strip hints when a VF has no GDEF varstore after
       * subsetting*/
      إذا (has_fvar && !c->plan->has_gdef_varstore)
        strip = درست;
      newFormats = compute_effective_مقدار_formats (glyphset, strip, درست);
    }

    بیرون->مقدار_Format_2[0] = newFormats.اولی;
    بیرون->مقدار_Format_2[1] = newFormats.second;

    hb_sorted_وکتور_t<hb_codepoint_t> new_coverage;

    + hb_zip (هذا+coverage, pair_مجموعه)
    | hb_filter (glyphset, hb_اولی)
    | hb_filter ([هذا, c, بیرون] (ثابت نام_نوع Types::قالب جابه_جایی_مقصد<Pair_مجموعه>& _)
                 {
                   خودکار snap = c->serializer->snapshot ();
                   خودکار *o = بیرون->pair_مجموعه.serialize_append (c->serializer);
                   إذا (unlikely (!o)) أرجع خطأ;
                   منطقي ret = o->serialize_subset (c, _, هذا, مقدار_Format_2, بیرون->مقدار_Format_2);
                   إذا (!ret)
                   {
                     بیرون->pair_مجموعه.برداشت ();
                     c->serializer->revert (snap);
                   }
                   أرجع ret;
                 },
                 hb_دومی)
    | hb_map (hb_اولی)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    بیرون->coverage.serialize_serialize (c->serializer, new_coverage.iter ());

    return_trace (منطقي (new_coverage));
  }


  hb_pair_t<بیعلامت, بیعلامت> compute_effective_مقدار_formats (ثابت hb_مجموعه_t& glyphset,
                                                                 منطقي strip_hints, منطقي strip_خالی,
                                                                 ثابت hb_hashmap_t<بیعلامت, hb_pair_t<بیعلامت, صحيح>> *varidx_delta_map = اشاره_تهی) ثابت
  {
    بیعلامت format1 = 0;
    بیعلامت format2 = 0;
    لكل (ثابت خودکار & _ :
	  + hb_zip (هذا+coverage, pair_مجموعه)
	  | hb_filter (glyphset, hb_اولی)
	  | hb_map (hb_دومی)
	)
    {
      ثابت Pair_مجموعه& مجموعه = (هذا + _);
      خودکار formats = مجموعه.get_مقادیر_view (مقدار_Format_2).compute_effective_مقدار_formats (
          glyphset, مقدار_Format_2, strip_hints, strip_خالی, varidx_delta_map);
      format1 |= formats.اولی;
      format2 |= formats.second;

      إذا (!varidx_delta_map &&
          format1 == مقدار_Format_2[0] && format2 == مقدار_Format_2[1])
        اكسر;
    }

    أرجع hb_pair (format1, format2);
  }
};


}
}
}

#پایان_اگر  // OT_LAYOUT_GPOS_PAIRPOSFORMAT1_HH
