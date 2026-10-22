#جيڪڏهن_وصف_نه_ٿيل OT_LAYOUT_GSUB_LIGATURESUBSTFORMAT1_HH
#وصف_ڪريو OT_LAYOUT_GSUB_LIGATURESUBSTFORMAT1_HH

#ضمن "Common.hh"
#ضمن "LigatureSet.hh"

نطاق OT {
نطاق Layout {
نطاق GSUB_impl {

سانچو <قسم_نالو Types>
بنية LigatureSubstFormat1_2
{
  محمي:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  قسم_نالو Types::سانچو LOffset_منزل<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of Substitution table */
  قسم_نالو Types::سانچو Array_جو<قسم_نالو Types::سانچو وٿي_منزل<Ligature_سيٽ<Types>>>
                ligature_سيٽ;            /* Array LigatureSet tables
                                         * ordered by Coverage Index */
  عام:
  DEFINE_سائيز_ARRAY (2 + Types::LOffset::static_سائيز + Types::HBUINT::static_سائيز, ligature_سيٽ);

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (coverage.sanitize (c, هذا) && ligature_سيٽ.sanitize (c, هذا));
  }

  منطقي intersects (ثابت hb_سيٽ_t *glyphs) ثابت
  {
    أرجع
    + hb_zip (هذا+coverage, ligature_سيٽ)
    | hb_filter (*glyphs, hb_پهريون)
    | hb_map (hb_ٻيو)
    | hb_map ([هذا, glyphs] (ثابت قسم_نالو Types::سانچو وٿي_منزل<Ligature_سيٽ<Types>> &_)
              { أرجع (هذا+_).intersects (glyphs); })
    | hb_any
    ;
  }

  منطقي may_have_non_1to1 () ثابت
  { أرجع سچ; }

  فراغ depend (hb_depend_context_t *c) ثابت
  {
    + hb_zip (هذا+coverage, ligature_سيٽ)
    | hb_filter (c->والد_سرگرم_glyphs (), hb_پهريون)
    | hb_apply ([&] (ثابت hb_pair_t<hb_codepoint_t, ثابت قسم_نالو Types::سانچو وٿي_منزل<Ligature_سيٽ<Types>>&> &_)
                {
                  ثابت Ligature_سيٽ<Types>& ls = هذا+_.second;
                  ls.depend (c, _.پهريون);
                })
    ;
  }

  فراغ گهيرو (hb_گهيرو_context_t *c) ثابت
  {
    + hb_zip (هذا+coverage, ligature_سيٽ)
    | hb_filter (c->والد_سرگرم_glyphs (), hb_پهريون)
    | hb_map (hb_ٻيو)
    | hb_map (hb_جوڙيو (هذا))
    | hb_apply ([c] (ثابت Ligature_سيٽ<Types> &_) { _.گهيرو (c); })
    ;

  }

  فراغ گهيرو_lookups (hb_گهيرو_lookups_context_t *c) ثابت {}

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    إذا (unlikely (!(هذا+coverage).collect_coverage (c->داخلا_2))) أرجع;

    + hb_zip (هذا+coverage, ligature_سيٽ)
    | hb_map (hb_ٻيو)
    | hb_map (hb_جوڙيو (هذا))
    | hb_apply ([c] (ثابت Ligature_سيٽ<Types> &_) { _.collect_glyphs (c); })
    ;
  }

  فراغ collect_ٻيو_glyphs (hb_سيٽ_digest_t *digest) ثابت
  {
    + hb_iter (ligature_سيٽ)
    | hb_map (hb_جوڙيو (هذا))
    | hb_apply ([digest] (ثابت Ligature_سيٽ<Types> &_) { _.collect_seconds (*digest); })
    ;
  }

  ثابت Coverage &get_coverage () ثابت { أرجع هذا+coverage; }

  منطقي would_apply (hb_would_apply_context_t *c) ثابت
  {
    بنانشان صحيح اشاريو_2 = (هذا+coverage).get_coverage (c->glyphs[0]);
    إذا (likely (اشاريو_2 == NOT_COVERED)) أرجع خطأ;

    ثابت خودڪار &lig_سيٽ = هذا+ligature_سيٽ[اشاريو_2];
    أرجع lig_سيٽ.would_apply (c);
  }

  بنية external_cache_t
  {
    hb_ot_layout_mapping_cache_t coverage;
    hb_سيٽ_digest_t seconds;
  };
  فراغ *external_cache_create () ثابت
  {
    external_cache_t *cache = (external_cache_t *) hb_malloc (حجم (external_cache_t));
    إذا (likely (cache))
    {
      cache->coverage.clear ();

      cache->seconds.init ();
      + hb_iter (ligature_سيٽ)
      | hb_map (hb_جوڙيو (هذا))
      | hb_apply ([cache] (ثابت Ligature_سيٽ<Types> &_) { _.collect_seconds (cache->seconds); })
      ;
    }
    أرجع cache;
  }

  منطقي apply (hb_ot_apply_context_t *c, فراغ *external_cache) ثابت
  {
    TRACE_APPLY (هذا);
    hb_بفر_t *بفر = c->بفر;

#جيڪڏهن_وصف_نه_ٿيل HB_NO_OT_LAYOUT_LOOKUP_CACHE
    external_cache_t *cache = (external_cache_t *) external_cache;
    ثابت hb_سيٽ_digest_t *seconds = cache ? &cache->seconds : خالي_اشارو;
    بنانشان صحيح اشاريو_2 = (هذا+coverage).get_coverage (بفر->cur().codepoint, cache ? &cache->coverage : خالي_اشارو);
#وإلا
    ثابت hb_سيٽ_digest_t *seconds = خالي_اشارو;
    بنانشان صحيح اشاريو_2 = (هذا+coverage).get_coverage (بفر->cur().codepoint);
#جيڪڏهن_پورو
    إذا (اشاريو_2 == NOT_COVERED) return_trace (خطأ);

    ثابت خودڪار &lig_سيٽ = هذا+ligature_سيٽ[اشاريو_2];
    return_trace (lig_سيٽ.apply (c, seconds));
  }

  سانچو <قسم_نالو GlyphID>
  منطقي serialize (hb_serialize_context_t *c,
                  hb_sorted_array_t<GlyphID> پهريون_glyphs,
                  hb_array_t<ثابت بنانشان صحيح> ligature_في_پهريون_glyph_ڳڻپ_list,
                  hb_array_t<GlyphID> ligatures_list,
                  hb_array_t<ثابت بنانشان صحيح> component_ڳڻپ_list,
                  hb_array_t<GlyphID> component_list /* Starting from second for each ligature */)
  {
    TRACE_SERIALIZE (هذا);
    إذا (unlikely (!c->extend_min (هذا))) return_trace (خطأ);
    إذا (unlikely (!ligature_سيٽ.serialize (c, پهريون_glyphs.ڊيگهائي))) return_trace (خطأ);
    لكل (بنانشان صحيح i = 0; i < پهريون_glyphs.ڊيگهائي; i++)
    {
      بنانشان صحيح ligature_ڳڻپ = ligature_في_پهريون_glyph_ڳڻپ_list[i];
      إذا (unlikely (!ligature_سيٽ[i]
                        .serialize_serialize (c,
                                              ligatures_list.sub_array (0, ligature_ڳڻپ),
                                              component_ڳڻپ_list.sub_array (0, ligature_ڳڻپ),
                                              component_list))) return_trace (خطأ);
      ligatures_list += ligature_ڳڻپ;
      component_ڳڻپ_list += ligature_ڳڻپ;
    }
    return_trace (coverage.serialize_serialize (c, پهريون_glyphs));
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_سيٽ_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    خودڪار *ٻاهريون = c->serializer->شروعات_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (ٻاهريون))) return_trace (خطأ);
    ٻاهريون->format = format;

    // Due to a bug in some older versions of windows 7 the Coverage table must be
    // packed after the LigatureSet and Ligature tables, so serialize Coverage first
    // which places it last in the packed order.
    hb_سيٽ_t new_coverage;
    + hb_zip (هذا+coverage, hb_iter (ligature_سيٽ) | hb_map (hb_جوڙيو (هذا)))
    | hb_filter (glyphset, hb_پهريون)
    | hb_filter ([&] (ثابت Ligature_سيٽ<Types>& _) {
      أرجع _.intersects_lig_glyph (&glyphset);
    }, hb_ٻيو)
    | hb_map (hb_پهريون)
    | hb_sink (new_coverage);

    إذا (!c->serializer->push<Coverage> ()
        ->serialize (c->serializer,
                     + new_coverage.iter () | hb_map_retains_ترتيب_ڏيڻ (glyph_map)))
    {
      c->serializer->ڪڍو_discard ();
      return_trace (خطأ);
    }

    بنانشان coverage_idx = c->serializer->ڪڍو_pack ();
     c->serializer->جوڙيو_link (ٻاهريون->coverage, coverage_idx);

    + hb_zip (هذا+coverage, ligature_سيٽ)
    | hb_filter (new_coverage, hb_پهريون)
    | hb_map (hb_ٻيو)
    // to ensure that the repacker always orders the coverage table after the LigatureSet
    // and LigatureSubtable's they will be linked to the Coverage table via a virtual link
    // the coverage table object idx is passed down to facilitate this.
    | hb_apply (subset_وٿي_array (c, ٻاهريون->ligature_سيٽ, هذا, coverage_idx))
    ;

    return_trace (منطقي (new_coverage));
  }
};

}
}
}

#جيڪڏهن_پورو  /* OT_LAYOUT_GSUB_LIGATURESUBSTFORMAT1_HH */
