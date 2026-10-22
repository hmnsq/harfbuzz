#اگر_تعریف_نشده OT_LAYOUT_GSUB_LIGATURESUBSTFORMAT1_HH
#تعریف_کن OT_LAYOUT_GSUB_LIGATURESUBSTFORMAT1_HH

#ضمن "Common.hh"
#ضمن "LigatureSet.hh"

نطاق OT {
نطاق Layout {
نطاق GSUB_impl {

قالب <نام_نوع Types>
بنية LigatureSubstFormat1_2
{
  محمي:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  نام_نوع Types::قالب LOffset_مقصد<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of Substitution table */
  نام_نوع Types::قالب Array_از<نام_نوع Types::قالب جابه_جایی_مقصد<Ligature_مجموعه<Types>>>
                ligature_مجموعه;            /* Array LigatureSet tables
                                         * ordered by Coverage Index */
  عام:
  DEFINE_اندازه_ARRAY (2 + Types::LOffset::static_اندازه + Types::HBUINT::static_اندازه, ligature_مجموعه);

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (coverage.sanitize (c, هذا) && ligature_مجموعه.sanitize (c, هذا));
  }

  منطقي intersects (ثابت hb_مجموعه_t *glyphs) ثابت
  {
    أرجع
    + hb_zip (هذا+coverage, ligature_مجموعه)
    | hb_filter (*glyphs, hb_اولی)
    | hb_map (hb_دومی)
    | hb_map ([هذا, glyphs] (ثابت نام_نوع Types::قالب جابه_جایی_مقصد<Ligature_مجموعه<Types>> &_)
              { أرجع (هذا+_).intersects (glyphs); })
    | hb_any
    ;
  }

  منطقي may_have_non_1to1 () ثابت
  { أرجع درست; }

  فراغ depend (hb_depend_context_t *c) ثابت
  {
    + hb_zip (هذا+coverage, ligature_مجموعه)
    | hb_filter (c->والد_فعال_glyphs (), hb_اولی)
    | hb_apply ([&] (ثابت hb_pair_t<hb_codepoint_t, ثابت نام_نوع Types::قالب جابه_جایی_مقصد<Ligature_مجموعه<Types>>&> &_)
                {
                  ثابت Ligature_مجموعه<Types>& ls = هذا+_.second;
                  ls.depend (c, _.اولی);
                })
    ;
  }

  فراغ بستگی (hb_بستگی_context_t *c) ثابت
  {
    + hb_zip (هذا+coverage, ligature_مجموعه)
    | hb_filter (c->والد_فعال_glyphs (), hb_اولی)
    | hb_map (hb_دومی)
    | hb_map (hb_جمع_کن (هذا))
    | hb_apply ([c] (ثابت Ligature_مجموعه<Types> &_) { _.بستگی (c); })
    ;

  }

  فراغ بستگی_lookups (hb_بستگی_lookups_context_t *c) ثابت {}

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    إذا (unlikely (!(هذا+coverage).collect_coverage (c->درون_داد))) أرجع;

    + hb_zip (هذا+coverage, ligature_مجموعه)
    | hb_map (hb_دومی)
    | hb_map (hb_جمع_کن (هذا))
    | hb_apply ([c] (ثابت Ligature_مجموعه<Types> &_) { _.collect_glyphs (c); })
    ;
  }

  فراغ collect_دومی_glyphs (hb_مجموعه_digest_t *digest) ثابت
  {
    + hb_iter (ligature_مجموعه)
    | hb_map (hb_جمع_کن (هذا))
    | hb_apply ([digest] (ثابت Ligature_مجموعه<Types> &_) { _.collect_seconds (*digest); })
    ;
  }

  ثابت Coverage &get_coverage () ثابت { أرجع هذا+coverage; }

  منطقي would_apply (hb_would_apply_context_t *c) ثابت
  {
    بیعلامت صحيح شاخص_2 = (هذا+coverage).get_coverage (c->glyphs[0]);
    إذا (likely (شاخص_2 == NOT_COVERED)) أرجع خطأ;

    ثابت خودکار &lig_مجموعه = هذا+ligature_مجموعه[شاخص_2];
    أرجع lig_مجموعه.would_apply (c);
  }

  بنية external_cache_t
  {
    hb_ot_layout_mapping_cache_t coverage;
    hb_مجموعه_digest_t seconds;
  };
  فراغ *external_cache_create () ثابت
  {
    external_cache_t *cache = (external_cache_t *) hb_malloc (حجم (external_cache_t));
    إذا (likely (cache))
    {
      cache->coverage.clear ();

      cache->seconds.init ();
      + hb_iter (ligature_مجموعه)
      | hb_map (hb_جمع_کن (هذا))
      | hb_apply ([cache] (ثابت Ligature_مجموعه<Types> &_) { _.collect_seconds (cache->seconds); })
      ;
    }
    أرجع cache;
  }

  منطقي apply (hb_ot_apply_context_t *c, فراغ *external_cache) ثابت
  {
    TRACE_APPLY (هذا);
    hb_میانگیر_t *میانگیر = c->میانگیر;

#اگر_تعریف_نشده HB_NO_OT_LAYOUT_LOOKUP_CACHE
    external_cache_t *cache = (external_cache_t *) external_cache;
    ثابت hb_مجموعه_digest_t *seconds = cache ? &cache->seconds : اشاره_تهی;
    بیعلامت صحيح شاخص_2 = (هذا+coverage).get_coverage (میانگیر->cur().codepoint, cache ? &cache->coverage : اشاره_تهی);
#وإلا
    ثابت hb_مجموعه_digest_t *seconds = اشاره_تهی;
    بیعلامت صحيح شاخص_2 = (هذا+coverage).get_coverage (میانگیر->cur().codepoint);
#پایان_اگر
    إذا (شاخص_2 == NOT_COVERED) return_trace (خطأ);

    ثابت خودکار &lig_مجموعه = هذا+ligature_مجموعه[شاخص_2];
    return_trace (lig_مجموعه.apply (c, seconds));
  }

  قالب <نام_نوع GlyphID>
  منطقي serialize (hb_serialize_context_t *c,
                  hb_sorted_array_t<GlyphID> اولی_glyphs,
                  hb_array_t<ثابت بیعلامت صحيح> ligature_در_هر_اولی_glyph_شمار_list,
                  hb_array_t<GlyphID> ligatures_list,
                  hb_array_t<ثابت بیعلامت صحيح> component_شمار_list,
                  hb_array_t<GlyphID> component_list /* Starting from second for each ligature */)
  {
    TRACE_SERIALIZE (هذا);
    إذا (unlikely (!c->extend_min (هذا))) return_trace (خطأ);
    إذا (unlikely (!ligature_مجموعه.serialize (c, اولی_glyphs.درازی))) return_trace (خطأ);
    لكل (بیعلامت صحيح i = 0; i < اولی_glyphs.درازی; i++)
    {
      بیعلامت صحيح ligature_شمار = ligature_در_هر_اولی_glyph_شمار_list[i];
      إذا (unlikely (!ligature_مجموعه[i]
                        .serialize_serialize (c,
                                              ligatures_list.sub_array (0, ligature_شمار),
                                              component_شمار_list.sub_array (0, ligature_شمار),
                                              component_list))) return_trace (خطأ);
      ligatures_list += ligature_شمار;
      component_شمار_list += ligature_شمار;
    }
    return_trace (coverage.serialize_serialize (c, اولی_glyphs));
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_مجموعه_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    خودکار *بیرون = c->serializer->شروع_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (بیرون))) return_trace (خطأ);
    بیرون->format = format;

    // Due to a bug in some older versions of windows 7 the Coverage table must be
    // packed after the LigatureSet and Ligature tables, so serialize Coverage first
    // which places it last in the packed order.
    hb_مجموعه_t new_coverage;
    + hb_zip (هذا+coverage, hb_iter (ligature_مجموعه) | hb_map (hb_جمع_کن (هذا)))
    | hb_filter (glyphset, hb_اولی)
    | hb_filter ([&] (ثابت Ligature_مجموعه<Types>& _) {
      أرجع _.intersects_lig_glyph (&glyphset);
    }, hb_دومی)
    | hb_map (hb_اولی)
    | hb_sink (new_coverage);

    إذا (!c->serializer->push<Coverage> ()
        ->serialize (c->serializer,
                     + new_coverage.iter () | hb_map_retains_ترتیبدهی (glyph_map)))
    {
      c->serializer->برداشت_discard ();
      return_trace (خطأ);
    }

    بیعلامت coverage_idx = c->serializer->برداشت_pack ();
     c->serializer->جمع_کن_link (بیرون->coverage, coverage_idx);

    + hb_zip (هذا+coverage, ligature_مجموعه)
    | hb_filter (new_coverage, hb_اولی)
    | hb_map (hb_دومی)
    // to ensure that the repacker always orders the coverage table after the LigatureSet
    // and LigatureSubtable's they will be linked to the Coverage table via a virtual link
    // the coverage table object idx is passed down to facilitate this.
    | hb_apply (subset_جابه_جایی_array (c, بیرون->ligature_مجموعه, هذا, coverage_idx))
    ;

    return_trace (منطقي (new_coverage));
  }
};

}
}
}

#پایان_اگر  /* OT_LAYOUT_GSUB_LIGATURESUBSTFORMAT1_HH */
