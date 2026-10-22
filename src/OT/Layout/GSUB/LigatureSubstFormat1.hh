#که_نه_تعریف_شوی OT_LAYOUT_GSUB_LIGATURESUBSTFORMAT1_HH
#تعریف_کړه OT_LAYOUT_GSUB_LIGATURESUBSTFORMAT1_HH

#ضمن "Common.hh"
#ضمن "LigatureSet.hh"

نطاق OT {
نطاق Layout {
نطاق GSUB_impl {

کينډۍ <ډول_نوم Types>
بنية LigatureSubstFormat1_2
{
  محمي:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  ډول_نوم Types::کينډۍ LOffset_منزل<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of Substitution table */
  ډول_نوم Types::کينډۍ Array_د<ډول_نوم Types::کينډۍ دننه_واټن_منزل<Ligature_سېټ<Types>>>
                ligature_سېټ;            /* Array LigatureSet tables
                                         * ordered by Coverage Index */
  عام:
  DEFINE_اندازه_ARRAY (2 + Types::LOffset::static_اندازه + Types::HBUINT::static_اندازه, ligature_سېټ);

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (coverage.sanitize (c, هذا) && ligature_سېټ.sanitize (c, هذا));
  }

  منطقي intersects (ثابت hb_سېټ_t *glyphs) ثابت
  {
    أرجع
    + hb_zip (هذا+coverage, ligature_سېټ)
    | hb_filter (*glyphs, hb_لومړی)
    | hb_map (hb_دوهم)
    | hb_map ([هذا, glyphs] (ثابت ډول_نوم Types::کينډۍ دننه_واټن_منزل<Ligature_سېټ<Types>> &_)
              { أرجع (هذا+_).intersects (glyphs); })
    | hb_any
    ;
  }

  منطقي may_have_non_1to1 () ثابت
  { أرجع ریښتیا; }

  فراغ depend (hb_depend_context_t *c) ثابت
  {
    + hb_zip (هذا+coverage, ligature_سېټ)
    | hb_filter (c->پلار_فعال_glyphs (), hb_لومړی)
    | hb_apply ([&] (ثابت hb_pair_t<hb_codepoint_t, ثابت ډول_نوم Types::کينډۍ دننه_واټن_منزل<Ligature_سېټ<Types>>&> &_)
                {
                  ثابت Ligature_سېټ<Types>& ls = هذا+_.second;
                  ls.depend (c, _.لومړی);
                })
    ;
  }

  فراغ تړون (hb_تړون_context_t *c) ثابت
  {
    + hb_zip (هذا+coverage, ligature_سېټ)
    | hb_filter (c->پلار_فعال_glyphs (), hb_لومړی)
    | hb_map (hb_دوهم)
    | hb_map (hb_جمع_کړه (هذا))
    | hb_apply ([c] (ثابت Ligature_سېټ<Types> &_) { _.تړون (c); })
    ;

  }

  فراغ تړون_lookups (hb_تړون_lookups_context_t *c) ثابت {}

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    إذا (unlikely (!(هذا+coverage).collect_coverage (c->ننوت))) أرجع;

    + hb_zip (هذا+coverage, ligature_سېټ)
    | hb_map (hb_دوهم)
    | hb_map (hb_جمع_کړه (هذا))
    | hb_apply ([c] (ثابت Ligature_سېټ<Types> &_) { _.collect_glyphs (c); })
    ;
  }

  فراغ collect_دوهم_glyphs (hb_سېټ_digest_t *digest) ثابت
  {
    + hb_iter (ligature_سېټ)
    | hb_map (hb_جمع_کړه (هذا))
    | hb_apply ([digest] (ثابت Ligature_سېټ<Types> &_) { _.collect_seconds (*digest); })
    ;
  }

  ثابت Coverage &get_coverage () ثابت { أرجع هذا+coverage; }

  منطقي would_apply (hb_would_apply_context_t *c) ثابت
  {
    بینښه صحيح شاخص_2 = (هذا+coverage).get_coverage (c->glyphs[0]);
    إذا (likely (شاخص_2 == NOT_COVERED)) أرجع خطأ;

    ثابت خپلکاری &lig_سېټ = هذا+ligature_سېټ[شاخص_2];
    أرجع lig_سېټ.would_apply (c);
  }

  بنية external_cache_t
  {
    hb_ot_layout_mapping_cache_t coverage;
    hb_سېټ_digest_t seconds;
  };
  فراغ *external_cache_create () ثابت
  {
    external_cache_t *cache = (external_cache_t *) hb_malloc (حجم (external_cache_t));
    إذا (likely (cache))
    {
      cache->coverage.clear ();

      cache->seconds.init ();
      + hb_iter (ligature_سېټ)
      | hb_map (hb_جمع_کړه (هذا))
      | hb_apply ([cache] (ثابت Ligature_سېټ<Types> &_) { _.collect_seconds (cache->seconds); })
      ;
    }
    أرجع cache;
  }

  منطقي apply (hb_ot_apply_context_t *c, فراغ *external_cache) ثابت
  {
    TRACE_APPLY (هذا);
    hb_بفر_t *بفر = c->بفر;

#که_نه_تعریف_شوی HB_NO_OT_LAYOUT_LOOKUP_CACHE
    external_cache_t *cache = (external_cache_t *) external_cache;
    ثابت hb_سېټ_digest_t *seconds = cache ? &cache->seconds : تش_اشاره;
    بینښه صحيح شاخص_2 = (هذا+coverage).get_coverage (بفر->cur().codepoint, cache ? &cache->coverage : تش_اشاره);
#وإلا
    ثابت hb_سېټ_digest_t *seconds = تش_اشاره;
    بینښه صحيح شاخص_2 = (هذا+coverage).get_coverage (بفر->cur().codepoint);
#د_که_پای
    إذا (شاخص_2 == NOT_COVERED) return_trace (خطأ);

    ثابت خپلکاری &lig_سېټ = هذا+ligature_سېټ[شاخص_2];
    return_trace (lig_سېټ.apply (c, seconds));
  }

  کينډۍ <ډول_نوم GlyphID>
  منطقي serialize (hb_serialize_context_t *c,
                  hb_sorted_array_t<GlyphID> لومړی_glyphs,
                  hb_array_t<ثابت بینښه صحيح> ligature_فی_لومړی_glyph_شمېر_list,
                  hb_array_t<GlyphID> ligatures_list,
                  hb_array_t<ثابت بینښه صحيح> component_شمېر_list,
                  hb_array_t<GlyphID> component_list /* Starting from second for each ligature */)
  {
    TRACE_SERIALIZE (هذا);
    إذا (unlikely (!c->extend_min (هذا))) return_trace (خطأ);
    إذا (unlikely (!ligature_سېټ.serialize (c, لومړی_glyphs.اوږدتیا))) return_trace (خطأ);
    لكل (بینښه صحيح i = 0; i < لومړی_glyphs.اوږدتیا; i++)
    {
      بینښه صحيح ligature_شمېر = ligature_فی_لومړی_glyph_شمېر_list[i];
      إذا (unlikely (!ligature_سېټ[i]
                        .serialize_serialize (c,
                                              ligatures_list.sub_array (0, ligature_شمېر),
                                              component_شمېر_list.sub_array (0, ligature_شمېر),
                                              component_list))) return_trace (خطأ);
      ligatures_list += ligature_شمېر;
      component_شمېر_list += ligature_شمېر;
    }
    return_trace (coverage.serialize_serialize (c, لومړی_glyphs));
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_سېټ_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    خپلکاری *بهر = c->serializer->پیل_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (بهر))) return_trace (خطأ);
    بهر->format = format;

    // Due to a bug in some older versions of windows 7 the Coverage table must be
    // packed after the LigatureSet and Ligature tables, so serialize Coverage first
    // which places it last in the packed order.
    hb_سېټ_t new_coverage;
    + hb_zip (هذا+coverage, hb_iter (ligature_سېټ) | hb_map (hb_جمع_کړه (هذا)))
    | hb_filter (glyphset, hb_لومړی)
    | hb_filter ([&] (ثابت Ligature_سېټ<Types>& _) {
      أرجع _.intersects_lig_glyph (&glyphset);
    }, hb_دوهم)
    | hb_map (hb_لومړی)
    | hb_sink (new_coverage);

    إذا (!c->serializer->push<Coverage> ()
        ->serialize (c->serializer,
                     + new_coverage.iter () | hb_map_retains_ترتیبونه (glyph_map)))
    {
      c->serializer->راوباسه_discard ();
      return_trace (خطأ);
    }

    بینښه coverage_idx = c->serializer->راوباسه_pack ();
     c->serializer->جمع_کړه_link (بهر->coverage, coverage_idx);

    + hb_zip (هذا+coverage, ligature_سېټ)
    | hb_filter (new_coverage, hb_لومړی)
    | hb_map (hb_دوهم)
    // to ensure that the repacker always orders the coverage table after the LigatureSet
    // and LigatureSubtable's they will be linked to the Coverage table via a virtual link
    // the coverage table object idx is passed down to facilitate this.
    | hb_apply (subset_دننه_واټن_array (c, بهر->ligature_سېټ, هذا, coverage_idx))
    ;

    return_trace (منطقي (new_coverage));
  }
};

}
}
}

#د_که_پای  /* OT_LAYOUT_GSUB_LIGATURESUBSTFORMAT1_HH */
