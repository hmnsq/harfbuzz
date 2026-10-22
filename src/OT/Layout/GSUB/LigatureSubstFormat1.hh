#اگر_غیر_تعریف_شدہ OT_LAYOUT_GSUB_LIGATURESUBSTFORMAT1_HH
#تعریف_کرو OT_LAYOUT_GSUB_LIGATURESUBSTFORMAT1_HH

#شامل "Common.hh"
#شامل "LigatureSet.hh"

نامخلا OT {
نامخلا Layout {
نامخلا GSUB_impl {

سانچہ <قسم_نام Types>
ساخت LigatureSubstFormat1_2
{
  محفوظ:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  قسم_نام Types::سانچہ LOffset_مقصد<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of Substitution table */
  قسم_نام Types::سانچہ Array_کا<قسم_نام Types::سانچہ انحراف_مقصد<Ligature_سیٹ<Types>>>
                ligature_سیٹ;            /* Array LigatureSet tables
                                         * ordered by Coverage Index */
  عوامی:
  DEFINE_سائز_ARRAY (2 + Types::LOffset::static_سائز + Types::HBUINT::static_سائز, ligature_سیٹ);

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (coverage.sanitize (c, یہ) && ligature_سیٹ.sanitize (c, یہ));
  }

  منطقی intersects (ثابت hb_سیٹ_t *glyphs) ثابت
  {
    واپس
    + hb_zip (یہ+coverage, ligature_سیٹ)
    | hb_filter (*glyphs, hb_پہلا)
    | hb_map (hb_دوسرا)
    | hb_map ([یہ, glyphs] (ثابت قسم_نام Types::سانچہ انحراف_مقصد<Ligature_سیٹ<Types>> &_)
              { واپس (یہ+_).intersects (glyphs); })
    | hb_any
    ;
  }

  منطقی may_have_non_1to1 () ثابت
  { واپس سچ; }

  خالی depend (hb_depend_context_t *c) ثابت
  {
    + hb_zip (یہ+coverage, ligature_سیٹ)
    | hb_filter (c->والد_فعال_glyphs (), hb_پہلا)
    | hb_apply ([&] (ثابت hb_pair_t<hb_codepoint_t, ثابت قسم_نام Types::سانچہ انحراف_مقصد<Ligature_سیٹ<Types>>&> &_)
                {
                  ثابت Ligature_سیٹ<Types>& ls = یہ+_.second;
                  ls.depend (c, _.پہلا);
                })
    ;
  }

  خالی احاطہ (hb_احاطہ_context_t *c) ثابت
  {
    + hb_zip (یہ+coverage, ligature_سیٹ)
    | hb_filter (c->والد_فعال_glyphs (), hb_پہلا)
    | hb_map (hb_دوسرا)
    | hb_map (hb_جوڑو (یہ))
    | hb_apply ([c] (ثابت Ligature_سیٹ<Types> &_) { _.احاطہ (c); })
    ;

  }

  خالی احاطہ_lookups (hb_احاطہ_lookups_context_t *c) ثابت {}

  خالی collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    اگر (unlikely (!(یہ+coverage).collect_coverage (c->داخلہ))) واپس;

    + hb_zip (یہ+coverage, ligature_سیٹ)
    | hb_map (hb_دوسرا)
    | hb_map (hb_جوڑو (یہ))
    | hb_apply ([c] (ثابت Ligature_سیٹ<Types> &_) { _.collect_glyphs (c); })
    ;
  }

  خالی collect_دوسرا_glyphs (hb_سیٹ_digest_t *digest) ثابت
  {
    + hb_iter (ligature_سیٹ)
    | hb_map (hb_جوڑو (یہ))
    | hb_apply ([digest] (ثابت Ligature_سیٹ<Types> &_) { _.collect_seconds (*digest); })
    ;
  }

  ثابت Coverage &get_coverage () ثابت { واپس یہ+coverage; }

  منطقی would_apply (hb_would_apply_context_t *c) ثابت
  {
    غیرعلامتی صحیح اشاریہ_2 = (یہ+coverage).get_coverage (c->glyphs[0]);
    اگر (likely (اشاریہ_2 == NOT_COVERED)) واپس جھوٹ;

    ثابت خودکار &lig_سیٹ = یہ+ligature_سیٹ[اشاریہ_2];
    واپس lig_سیٹ.would_apply (c);
  }

  ساخت external_cache_t
  {
    hb_ot_layout_mapping_cache_t coverage;
    hb_سیٹ_digest_t seconds;
  };
  خالی *external_cache_create () ثابت
  {
    external_cache_t *cache = (external_cache_t *) hb_malloc (حجم (external_cache_t));
    اگر (likely (cache))
    {
      cache->coverage.clear ();

      cache->seconds.init ();
      + hb_iter (ligature_سیٹ)
      | hb_map (hb_جوڑو (یہ))
      | hb_apply ([cache] (ثابت Ligature_سیٹ<Types> &_) { _.collect_seconds (cache->seconds); })
      ;
    }
    واپس cache;
  }

  منطقی apply (hb_ot_apply_context_t *c, خالی *external_cache) ثابت
  {
    TRACE_APPLY (یہ);
    hb_بفر_t *بفر = c->بفر;

#اگر_غیر_تعریف_شدہ HB_NO_OT_LAYOUT_LOOKUP_CACHE
    external_cache_t *cache = (external_cache_t *) external_cache;
    ثابت hb_سیٹ_digest_t *seconds = cache ? &cache->seconds : خالی_اشاریہ;
    غیرعلامتی صحیح اشاریہ_2 = (یہ+coverage).get_coverage (بفر->cur().codepoint, cache ? &cache->coverage : خالی_اشاریہ);
#ورنہ
    ثابت hb_سیٹ_digest_t *seconds = خالی_اشاریہ;
    غیرعلامتی صحیح اشاریہ_2 = (یہ+coverage).get_coverage (بفر->cur().codepoint);
#اگر_ختم
    اگر (اشاریہ_2 == NOT_COVERED) return_trace (جھوٹ);

    ثابت خودکار &lig_سیٹ = یہ+ligature_سیٹ[اشاریہ_2];
    return_trace (lig_سیٹ.apply (c, seconds));
  }

  سانچہ <قسم_نام GlyphID>
  منطقی serialize (hb_serialize_context_t *c,
                  hb_sorted_array_t<GlyphID> پہلا_glyphs,
                  hb_array_t<ثابت غیرعلامتی صحیح> ligature_فی_پہلا_glyph_گنتی_list,
                  hb_array_t<GlyphID> ligatures_list,
                  hb_array_t<ثابت غیرعلامتی صحیح> component_گنتی_list,
                  hb_array_t<GlyphID> component_list /* Starting from second for each ligature */)
  {
    TRACE_SERIALIZE (یہ);
    اگر (unlikely (!c->extend_min (یہ))) return_trace (جھوٹ);
    اگر (unlikely (!ligature_سیٹ.serialize (c, پہلا_glyphs.لمبائی))) return_trace (جھوٹ);
    برائے (غیرعلامتی صحیح i = 0; i < پہلا_glyphs.لمبائی; i++)
    {
      غیرعلامتی صحیح ligature_گنتی = ligature_فی_پہلا_glyph_گنتی_list[i];
      اگر (unlikely (!ligature_سیٹ[i]
                        .serialize_serialize (c,
                                              ligatures_list.sub_array (0, ligature_گنتی),
                                              component_گنتی_list.sub_array (0, ligature_گنتی),
                                              component_list))) return_trace (جھوٹ);
      ligatures_list += ligature_گنتی;
      component_گنتی_list += ligature_گنتی;
    }
    return_trace (coverage.serialize_serialize (c, پہلا_glyphs));
  }

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (یہ);
    ثابت hb_سیٹ_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    خودکار *برآمد = c->serializer->آغاز_embed (*یہ);
    اگر (unlikely (!c->serializer->extend_min (برآمد))) return_trace (جھوٹ);
    برآمد->format = format;

    // Due to a bug in some older versions of windows 7 the Coverage table must be
    // packed after the LigatureSet and Ligature tables, so serialize Coverage first
    // which places it last in the packed order.
    hb_سیٹ_t new_coverage;
    + hb_zip (یہ+coverage, hb_iter (ligature_سیٹ) | hb_map (hb_جوڑو (یہ)))
    | hb_filter (glyphset, hb_پہلا)
    | hb_filter ([&] (ثابت Ligature_سیٹ<Types>& _) {
      واپس _.intersects_lig_glyph (&glyphset);
    }, hb_دوسرا)
    | hb_map (hb_پہلا)
    | hb_sink (new_coverage);

    اگر (!c->serializer->push<Coverage> ()
        ->serialize (c->serializer,
                     + new_coverage.iter () | hb_map_retains_ترتیب_کاری (glyph_map)))
    {
      c->serializer->نکالو_discard ();
      return_trace (جھوٹ);
    }

    غیرعلامتی coverage_idx = c->serializer->نکالو_pack ();
     c->serializer->جوڑو_link (برآمد->coverage, coverage_idx);

    + hb_zip (یہ+coverage, ligature_سیٹ)
    | hb_filter (new_coverage, hb_پہلا)
    | hb_map (hb_دوسرا)
    // to ensure that the repacker always orders the coverage table after the LigatureSet
    // and LigatureSubtable's they will be linked to the Coverage table via a virtual link
    // the coverage table object idx is passed down to facilitate this.
    | hb_apply (subset_انحراف_array (c, برآمد->ligature_سیٹ, یہ, coverage_idx))
    ;

    return_trace (منطقی (new_coverage));
  }
};

}
}
}

#اگر_ختم  /* OT_LAYOUT_GSUB_LIGATURESUBSTFORMAT1_HH */
