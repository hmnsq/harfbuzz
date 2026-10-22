#جے_بنا_تعریف OT_LAYOUT_GSUB_LIGATURESUBSTFORMAT1_HH
#تعریف_کرو OT_LAYOUT_GSUB_LIGATURESUBSTFORMAT1_HH

#ضمن "Common.hh"
#ضمن "LigatureSet.hh"

نطاق OT {
نطاق Layout {
نطاق GSUB_impl {

سانچا <قسم_ناں Types>
بنية LigatureSubstFormat1_2
{
  محمي:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  قسم_ناں Types::سانچا LOffset_منزل<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of Substitution table */
  قسم_ناں Types::سانچا Array_دا<قسم_ناں Types::سانچا وتھ_منزل<Ligature_سیٹ<Types>>>
                ligature_سیٹ;            /* Array LigatureSet tables
                                         * ordered by Coverage Index */
  عام:
  DEFINE_ناپ_ARRAY (2 + Types::LOffset::static_ناپ + Types::HBUINT::static_ناپ, ligature_سیٹ);

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (coverage.sanitize (c, هذا) && ligature_سیٹ.sanitize (c, هذا));
  }

  منطقي intersects (ثابت hb_سیٹ_t *glyphs) ثابت
  {
    أرجع
    + hb_zip (هذا+coverage, ligature_سیٹ)
    | hb_filter (*glyphs, hb_پہلا)
    | hb_map (hb_دوجا)
    | hb_map ([هذا, glyphs] (ثابت قسم_ناں Types::سانچا وتھ_منزل<Ligature_سیٹ<Types>> &_)
              { أرجع (هذا+_).intersects (glyphs); })
    | hb_any
    ;
  }

  منطقي may_have_non_1to1 () ثابت
  { أرجع سچ; }

  فراغ depend (hb_depend_context_t *c) ثابت
  {
    + hb_zip (هذا+coverage, ligature_سیٹ)
    | hb_filter (c->ماپا_سرگرم_glyphs (), hb_پہلا)
    | hb_apply ([&] (ثابت hb_pair_t<hb_codepoint_t, ثابت قسم_ناں Types::سانچا وتھ_منزل<Ligature_سیٹ<Types>>&> &_)
                {
                  ثابت Ligature_سیٹ<Types>& ls = هذا+_.second;
                  ls.depend (c, _.پہلا);
                })
    ;
  }

  فراغ گھیرا (hb_گھیرا_context_t *c) ثابت
  {
    + hb_zip (هذا+coverage, ligature_سیٹ)
    | hb_filter (c->ماپا_سرگرم_glyphs (), hb_پہلا)
    | hb_map (hb_دوجا)
    | hb_map (hb_جوڑو (هذا))
    | hb_apply ([c] (ثابت Ligature_سیٹ<Types> &_) { _.گھیرا (c); })
    ;

  }

  فراغ گھیرا_lookups (hb_گھیرا_lookups_context_t *c) ثابت {}

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    إذا (unlikely (!(هذا+coverage).collect_coverage (c->داخلہ))) أرجع;

    + hb_zip (هذا+coverage, ligature_سیٹ)
    | hb_map (hb_دوجا)
    | hb_map (hb_جوڑو (هذا))
    | hb_apply ([c] (ثابت Ligature_سیٹ<Types> &_) { _.collect_glyphs (c); })
    ;
  }

  فراغ collect_دوجا_glyphs (hb_سیٹ_digest_t *digest) ثابت
  {
    + hb_iter (ligature_سیٹ)
    | hb_map (hb_جوڑو (هذا))
    | hb_apply ([digest] (ثابت Ligature_سیٹ<Types> &_) { _.collect_seconds (*digest); })
    ;
  }

  ثابت Coverage &get_coverage () ثابت { أرجع هذا+coverage; }

  منطقي would_apply (hb_would_apply_context_t *c) ثابت
  {
    بنانشانی صحيح اشاریہ_2 = (هذا+coverage).get_coverage (c->glyphs[0]);
    إذا (likely (اشاریہ_2 == NOT_COVERED)) أرجع خطأ;

    ثابت خودکار &lig_سیٹ = هذا+ligature_سیٹ[اشاریہ_2];
    أرجع lig_سیٹ.would_apply (c);
  }

  بنية external_cache_t
  {
    hb_ot_layout_mapping_cache_t coverage;
    hb_سیٹ_digest_t seconds;
  };
  فراغ *external_cache_create () ثابت
  {
    external_cache_t *cache = (external_cache_t *) hb_malloc (حجم (external_cache_t));
    إذا (likely (cache))
    {
      cache->coverage.clear ();

      cache->seconds.init ();
      + hb_iter (ligature_سیٹ)
      | hb_map (hb_جوڑو (هذا))
      | hb_apply ([cache] (ثابت Ligature_سیٹ<Types> &_) { _.collect_seconds (cache->seconds); })
      ;
    }
    أرجع cache;
  }

  منطقي apply (hb_ot_apply_context_t *c, فراغ *external_cache) ثابت
  {
    TRACE_APPLY (هذا);
    hb_بفر_t *بفر = c->بفر;

#جے_بنا_تعریف HB_NO_OT_LAYOUT_LOOKUP_CACHE
    external_cache_t *cache = (external_cache_t *) external_cache;
    ثابت hb_سیٹ_digest_t *seconds = cache ? &cache->seconds : خالی_سوچک;
    بنانشانی صحيح اشاریہ_2 = (هذا+coverage).get_coverage (بفر->cur().codepoint, cache ? &cache->coverage : خالی_سوچک);
#وإلا
    ثابت hb_سیٹ_digest_t *seconds = خالی_سوچک;
    بنانشانی صحيح اشاریہ_2 = (هذا+coverage).get_coverage (بفر->cur().codepoint);
#جے_انت
    إذا (اشاریہ_2 == NOT_COVERED) return_trace (خطأ);

    ثابت خودکار &lig_سیٹ = هذا+ligature_سیٹ[اشاریہ_2];
    return_trace (lig_سیٹ.apply (c, seconds));
  }

  سانچا <قسم_ناں GlyphID>
  منطقي serialize (hb_serialize_context_t *c,
                  hb_sorted_array_t<GlyphID> پہلا_glyphs,
                  hb_array_t<ثابت بنانشانی صحيح> ligature_فی_پہلا_glyph_گنتی_list,
                  hb_array_t<GlyphID> ligatures_list,
                  hb_array_t<ثابت بنانشانی صحيح> component_گنتی_list,
                  hb_array_t<GlyphID> component_list /* Starting from second for each ligature */)
  {
    TRACE_SERIALIZE (هذا);
    إذا (unlikely (!c->extend_min (هذا))) return_trace (خطأ);
    إذا (unlikely (!ligature_سیٹ.serialize (c, پہلا_glyphs.لمائی))) return_trace (خطأ);
    لكل (بنانشانی صحيح i = 0; i < پہلا_glyphs.لمائی; i++)
    {
      بنانشانی صحيح ligature_گنتی = ligature_فی_پہلا_glyph_گنتی_list[i];
      إذا (unlikely (!ligature_سیٹ[i]
                        .serialize_serialize (c,
                                              ligatures_list.sub_array (0, ligature_گنتی),
                                              component_گنتی_list.sub_array (0, ligature_گنتی),
                                              component_list))) return_trace (خطأ);
      ligatures_list += ligature_گنتی;
      component_گنتی_list += ligature_گنتی;
    }
    return_trace (coverage.serialize_serialize (c, پہلا_glyphs));
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_سیٹ_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    خودکار *باہرلا = c->serializer->شروع_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (باہرلا))) return_trace (خطأ);
    باہرلا->format = format;

    // Due to a bug in some older versions of windows 7 the Coverage table must be
    // packed after the LigatureSet and Ligature tables, so serialize Coverage first
    // which places it last in the packed order.
    hb_سیٹ_t new_coverage;
    + hb_zip (هذا+coverage, hb_iter (ligature_سیٹ) | hb_map (hb_جوڑو (هذا)))
    | hb_filter (glyphset, hb_پہلا)
    | hb_filter ([&] (ثابت Ligature_سیٹ<Types>& _) {
      أرجع _.intersects_lig_glyph (&glyphset);
    }, hb_دوجا)
    | hb_map (hb_پہلا)
    | hb_sink (new_coverage);

    إذا (!c->serializer->push<Coverage> ()
        ->serialize (c->serializer,
                     + new_coverage.iter () | hb_map_retains_چھانٹی (glyph_map)))
    {
      c->serializer->نکالو_discard ();
      return_trace (خطأ);
    }

    بنانشانی coverage_idx = c->serializer->نکالو_pack ();
     c->serializer->جوڑو_link (باہرلا->coverage, coverage_idx);

    + hb_zip (هذا+coverage, ligature_سیٹ)
    | hb_filter (new_coverage, hb_پہلا)
    | hb_map (hb_دوجا)
    // to ensure that the repacker always orders the coverage table after the LigatureSet
    // and LigatureSubtable's they will be linked to the Coverage table via a virtual link
    // the coverage table object idx is passed down to facilitate this.
    | hb_apply (subset_وتھ_array (c, باہرلا->ligature_سیٹ, هذا, coverage_idx))
    ;

    return_trace (منطقي (new_coverage));
  }
};

}
}
}

#جے_انت  /* OT_LAYOUT_GSUB_LIGATURESUBSTFORMAT1_HH */
