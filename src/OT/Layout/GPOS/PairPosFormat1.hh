#اگر_غیر_تعریف_شدہ OT_LAYOUT_GPOS_PAIRPOSFORMAT1_HH
#تعریف_کرو OT_LAYOUT_GPOS_PAIRPOSFORMAT1_HH

#شامل "PairSet.hh"

نامخلا OT {
نامخلا Layout {
نامخلا GPOS_impl {


سانچہ <قسم_نام Types>
ساخت PairPosFormat1_3
{
  استعمال Pair_سیٹ = GPOS_impl::Pair_سیٹ<Types>;
  استعمال Pair_قدر_ریکارڈ = GPOS_impl::Pair_قدر_ریکارڈ<Types>;

  محفوظ:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  قسم_نام Types::سانچہ LOffset_مقصد<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of subtable */
  قدر_Format   قدر_Format_2[2];         /* [0] Defines the types of data in
                                         * ValueRecord1--for the first glyph
                                         * in the pair--may be zero (0) */
                                        /* [1] Defines the types of data in
                                         * ValueRecord2--for the second glyph
                                         * in the pair--may be zero (0) */
  قسم_نام Types::سانچہ Array_کا<قسم_نام Types::سانچہ انحراف_مقصد<Pair_سیٹ>>
                pair_سیٹ;                /* Array of PairSet tables ordered by
                                         * Coverage Index */
  عوامی:
  DEFINE_سائز_ARRAY (2 + Types::LOffset::static_سائز + 2 * قدر_Format::static_سائز + Types::HBUINT::static_سائز, pair_سیٹ);

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);

    اگر (!c->check_struct (یہ)) return_trace (جھوٹ);
    hb_barrier ();

    غیرعلامتی صحیح len1 = قدر_Format_2[0].get_len ();
    غیرعلامتی صحیح len2 = قدر_Format_2[1].get_len ();
    قسم_نام Pair_سیٹ::sanitize_احاطہ_t احاطہ =
    {
      قدر_Format_2,
      len1,
      Pair_سیٹ::get_سائز (len1, len2)
    };

    return_trace (coverage.sanitize (c, یہ) && pair_سیٹ.sanitize (c, یہ, &احاطہ));
  }

  منطقی intersects (ثابت hb_سیٹ_t *glyphs) ثابت
  {
    خودکار &cov = یہ+coverage;

    اگر (pair_سیٹ.len > glyphs->get_population () * hb_ثنائی_ہندسہ_storage ((غیرعلامتی) pair_سیٹ.len))
    {
      برائے (hb_codepoint_t g : glyphs->iter ())
      {
	غیرعلامتی i = cov.get_coverage (g);
	اگر ((یہ+pair_سیٹ[i]).intersects (glyphs, قدر_Format_2))
	  واپس سچ;
      }
      واپس جھوٹ;
    }

    واپس
    + hb_zip (cov, pair_سیٹ)
    | hb_filter (*glyphs, hb_پہلا)
    | hb_map (hb_دوسرا)
    | hb_map ([glyphs, یہ] (ثابت قسم_نام Types::سانچہ انحراف_مقصد<Pair_سیٹ> &_)
              { واپس (یہ+_).intersects (glyphs, قدر_Format_2); })
    | hb_any
    ;
  }

  خالی احاطہ_lookups (hb_احاطہ_lookups_context_t *c) ثابت {}
  خالی collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    اگر ((!قدر_Format_2[0].has_device ()) && (!قدر_Format_2[1].has_device ())) واپس;

    خودکار it =
    + hb_zip (یہ+coverage, pair_سیٹ)
    | hb_filter (c->glyph_سیٹ, hb_پہلا)
    | hb_map (hb_دوسرا)
    ;

    اگر (!it) واپس;
    + it
    | hb_map (hb_جوڑو (یہ))
    | hb_apply ([&] (ثابت Pair_سیٹ& _) { _.collect_variation_indices (c, قدر_Format_2); })
    ;
  }

  خالی collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    اگر (unlikely (!(یہ+coverage).collect_coverage (c->داخلہ))) واپس;
    غیرعلامتی صحیح گنتی = pair_سیٹ.len;
    برائے (غیرعلامتی صحیح i = 0; i < گنتی; i++)
      (یہ+pair_سیٹ[i]).collect_glyphs (c, قدر_Format_2);
  }

  خالی collect_دوسرا_glyphs (hb_سیٹ_digest_t *digest) ثابت
  {
    غیرعلامتی گنتی = pair_سیٹ.len;
    برائے (غیرعلامتی i = 0; i < گنتی; i++)
      (یہ+pair_سیٹ[i]).collect_دوسرا_glyphs (digest, قدر_Format_2);
  }

  ثابت Coverage &get_coverage () ثابت { واپس یہ+coverage; }

  ساخت external_cache_t
  {
    hb_ot_layout_mapping_cache_t coverage;
    hb_سیٹ_digest_t pair_sets[HB_VAR_ARRAY];
  };
  خالی *external_cache_create () ثابت
  {
    غیرعلامتی گنتی = pair_سیٹ.len;
    غیرعلامتی سائز = حجم (external_cache_t) -
		    HB_VAR_ARRAY * حجم (hb_سیٹ_digest_t) +
		    گنتی * حجم (hb_سیٹ_digest_t);
    external_cache_t *cache = (external_cache_t *) hb_malloc (سائز);
    اگر (likely (cache))
    {
      cache->coverage.clear ();
      برائے (غیرعلامتی i = 0; i < گنتی; i++)
      {
	cache->pair_sets[i].init ();
	(یہ+pair_سیٹ[i]).collect_دوسرا_glyphs (&cache->pair_sets[i], قدر_Format_2);
      }
    }
    واپس cache;
  }

  منطقی apply (hb_ot_apply_context_t *c, خالی *external_cache) ثابت
  {
    TRACE_APPLY (یہ);

    hb_بفر_t *بفر = c->بفر;

#اگر_غیر_تعریف_شدہ HB_NO_OT_LAYOUT_LOOKUP_CACHE
    external_cache_t *cache = (external_cache_t *) external_cache;
    غیرعلامتی صحیح اشاریہ_2 = (یہ+coverage).get_coverage  (بفر->cur().codepoint, cache ? &cache->coverage : خالی_اشاریہ);
#ورنہ
    غیرعلامتی صحیح اشاریہ_2 = (یہ+coverage).get_coverage  (بفر->cur().codepoint);
#اگر_ختم
    اگر (اشاریہ_2 == NOT_COVERED) return_trace (جھوٹ);

    خودکار &skippy_iter = c->iter_داخلہ;
    skippy_iter.reset_fast (بفر->idx);
    غیرعلامتی unsafe_مقصد;
    اگر (unlikely (!skippy_iter.next (&unsafe_مقصد)))
    {
      بفر->unsafe_مقصد_concat (بفر->idx, unsafe_مقصد);
      return_trace (جھوٹ);
    }

#اگر_غیر_تعریف_شدہ HB_NO_OT_LAYOUT_LOOKUP_CACHE
    اگر (cache &&
	اشاریہ_2 < pair_سیٹ.len &&
	!cache->pair_sets[اشاریہ_2].may_have (بفر->info[skippy_iter.idx].codepoint))
    {
      بفر->unsafe_مقصد_concat (بفر->idx, skippy_iter.idx + 1);
      return_trace (جھوٹ);
    }
#اگر_ختم

    return_trace ((یہ+pair_سیٹ[اشاریہ_2]).apply (c, قدر_Format_2, skippy_iter.idx));
  }

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (یہ);

    ثابت hb_سیٹ_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    خودکار *برآمد = c->serializer->آغاز_embed (*یہ);
    اگر (unlikely (!c->serializer->extend_min (برآمد))) return_trace (جھوٹ);
    برآمد->format = format;

    hb_pair_t<غیرعلامتی, غیرعلامتی> newFormats = hb_pair (قدر_Format_2[0], قدر_Format_2[1]);

    اگر (c->plan->normalized_coords)
    {
      /* all device flags will be dropped when full instancing, no need to strip
       * hints, also do not strip emtpy cause we don't compute the new default
       * value during stripping */
      newFormats = compute_effective_قدر_formats (glyphset, جھوٹ, جھوٹ, &c->plan->layout_variation_idx_delta_map);
    }
    /* do not strip hints for VF */
    ورنہ اگر (c->plan->flags & HB_SUBSET_FLAGS_NO_HINTING)
    {
      hb_blob_t* blob = hb_face_reference_جدول (c->plan->ماخذ, HB_لیبل ('f','v','a','r'));
      منطقی has_fvar = (blob != hb_blob_get_خالی_خانے ());
      hb_blob_destroy (blob);

      منطقی strip = !has_fvar;
      /* special case: strip hints when a VF has no GDEF varstore after
       * subsetting*/
      اگر (has_fvar && !c->plan->has_gdef_varstore)
        strip = سچ;
      newFormats = compute_effective_قدر_formats (glyphset, strip, سچ);
    }

    برآمد->قدر_Format_2[0] = newFormats.پہلا;
    برآمد->قدر_Format_2[1] = newFormats.second;

    hb_sorted_سمتیہ_t<hb_codepoint_t> new_coverage;

    + hb_zip (یہ+coverage, pair_سیٹ)
    | hb_filter (glyphset, hb_پہلا)
    | hb_filter ([یہ, c, برآمد] (ثابت قسم_نام Types::سانچہ انحراف_مقصد<Pair_سیٹ>& _)
                 {
                   خودکار snap = c->serializer->snapshot ();
                   خودکار *o = برآمد->pair_سیٹ.serialize_append (c->serializer);
                   اگر (unlikely (!o)) واپس جھوٹ;
                   منطقی ret = o->serialize_subset (c, _, یہ, قدر_Format_2, برآمد->قدر_Format_2);
                   اگر (!ret)
                   {
                     برآمد->pair_سیٹ.نکالو ();
                     c->serializer->revert (snap);
                   }
                   واپس ret;
                 },
                 hb_دوسرا)
    | hb_map (hb_پہلا)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    برآمد->coverage.serialize_serialize (c->serializer, new_coverage.iter ());

    return_trace (منطقی (new_coverage));
  }


  hb_pair_t<غیرعلامتی, غیرعلامتی> compute_effective_قدر_formats (ثابت hb_سیٹ_t& glyphset,
                                                                 منطقی strip_hints, منطقی strip_خالی_خانے,
                                                                 ثابت hb_hashmap_t<غیرعلامتی, hb_pair_t<غیرعلامتی, صحیح>> *varidx_delta_map = خالی_اشاریہ) ثابت
  {
    غیرعلامتی format1 = 0;
    غیرعلامتی format2 = 0;
    برائے (ثابت خودکار & _ :
	  + hb_zip (یہ+coverage, pair_سیٹ)
	  | hb_filter (glyphset, hb_پہلا)
	  | hb_map (hb_دوسرا)
	)
    {
      ثابت Pair_سیٹ& سیٹ = (یہ + _);
      خودکار formats = سیٹ.get_اقدار_view (قدر_Format_2).compute_effective_قدر_formats (
          glyphset, قدر_Format_2, strip_hints, strip_خالی_خانے, varidx_delta_map);
      format1 |= formats.پہلا;
      format2 |= formats.second;

      اگر (!varidx_delta_map &&
          format1 == قدر_Format_2[0] && format2 == قدر_Format_2[1])
        توڑو;
    }

    واپس hb_pair (format1, format2);
  }
};


}
}
}

#اگر_ختم  // OT_LAYOUT_GPOS_PAIRPOSFORMAT1_HH
