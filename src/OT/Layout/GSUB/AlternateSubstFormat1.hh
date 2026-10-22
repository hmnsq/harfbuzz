#جيڪڏهن_وصف_نه_ٿيل OT_LAYOUT_GSUB_ALTERNATESUBSTFORMAT1_HH
#وصف_ڪريو OT_LAYOUT_GSUB_ALTERNATESUBSTFORMAT1_HH

#ضمن "AlternateSet.hh"
#ضمن "Common.hh"

نطاق OT {
نطاق Layout {
نطاق GSUB_impl {

سانچو <قسم_نالو Types>
بنية AlternateSubstFormat1_2
{
  محمي:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  قسم_نالو Types::سانچو LOffset_منزل<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of Substitution table */
  قسم_نالو Types::سانچو Array_جو<قسم_نالو Types::سانچو وٿي_منزل<Alternate_سيٽ<Types>>>
                alternate_سيٽ;           /* Array of AlternateSet tables
                                         * ordered by Coverage Index */
  عام:
  DEFINE_سائيز_ARRAY (2 + Types::LOffset::static_سائيز + Types::HBUINT::static_سائيز, alternate_سيٽ);

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (coverage.sanitize (c, هذا) && alternate_سيٽ.sanitize (c, هذا));
  }

  منطقي intersects (ثابت hb_سيٽ_t *glyphs) ثابت
  { أرجع (هذا+coverage).intersects (glyphs); }

  منطقي may_have_non_1to1 () ثابت
  { أرجع خطأ; }

  فراغ depend (hb_depend_context_t *c) ثابت
  {
    // Filter by parent_active_glyphs like closure does
    + hb_zip (هذا+coverage, alternate_سيٽ)
    | hb_filter (c->والد_سرگرم_glyphs (), hb_پهريون)
    | hb_apply ([&] (ثابت hb_pair_t<hb_codepoint_t, ثابت قسم_نالو Types::سانچو وٿي_منزل<Alternate_سيٽ<Types>>&> &_)
                {
                  ثابت Alternate_سيٽ<Types>& as = هذا+_.second;
                  as.depend (c, _.پهريون);
                })
    ;
  }

  فراغ گهيرو (hb_گهيرو_context_t *c) ثابت
  {
    + hb_zip (هذا+coverage, alternate_سيٽ)
    | hb_filter (c->والد_سرگرم_glyphs (), hb_پهريون)
    | hb_map (hb_ٻيو)
    | hb_map (hb_جوڙيو (هذا))
    | hb_apply ([c] (ثابت Alternate_سيٽ<Types> &_) { _.گهيرو (c); })
    ;
  }

  فراغ گهيرو_lookups (hb_گهيرو_lookups_context_t *c) ثابت {}

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    إذا (unlikely (!(هذا+coverage).collect_coverage (c->داخلا_2))) أرجع;
    + hb_zip (هذا+coverage, alternate_سيٽ)
    | hb_map (hb_ٻيو)
    | hb_map (hb_جوڙيو (هذا))
    | hb_apply ([c] (ثابت Alternate_سيٽ<Types> &_) { _.collect_glyphs (c); })
    ;
  }

  ثابت Coverage &get_coverage () ثابت { أرجع هذا+coverage; }

  منطقي would_apply (hb_would_apply_context_t *c) ثابت
  { أرجع c->len == 1 && (هذا+coverage).get_coverage (c->glyphs[0]) != NOT_COVERED; }

  بنانشان
  get_glyph_alternates (hb_codepoint_t  gid,
                        بنانشان        شروعات_وٿي,
                        بنانشان       *alternate_ڳڻپ  /* IN/OUT.  May be NULL. */,
                        hb_codepoint_t *alternate_glyphs /* OUT.     May be NULL. */) ثابت
  { أرجع (هذا+alternate_سيٽ[(هذا+coverage).get_coverage (gid)])
           .get_alternates (شروعات_وٿي, alternate_ڳڻپ, alternate_glyphs); }

  فراغ
  collect_glyph_alternates (hb_map_t  *alternate_ڳڻپ /* IN/OUT */,
			    hb_map_t  *alternate_glyphs /* IN/OUT */) ثابت
  {
    + hb_iter (alternate_سيٽ)
    | hb_map (hb_جوڙيو (هذا))
    | hb_zip (هذا+coverage)
    | hb_apply ([&] (ثابت hb_pair_t<ثابت Alternate_سيٽ<Types> &, hb_codepoint_t> _) {
		  _.پهريون.collect_alternates (_.second, alternate_ڳڻپ, alternate_glyphs);
		})
    ;
  }

  منطقي apply (hb_ot_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);

    بنانشان صحيح اشاريو_2 = (هذا+coverage).get_coverage (c->بفر->cur().codepoint);
    إذا (اشاريو_2 == NOT_COVERED) return_trace (خطأ);

    return_trace ((هذا+alternate_سيٽ[اشاريو_2]).apply (c));
  }

  سانچو <قسم_نالو GlyphID>
  منطقي serialize (hb_serialize_context_t *c,
                  hb_sorted_array_t<GlyphID> glyphs,
                  hb_array_t<ثابت بنانشان صحيح> alternate_len_list,
                  hb_array_t<GlyphID> alternate_glyphs_list)
  {
    TRACE_SERIALIZE (هذا);
    إذا (unlikely (!c->extend_min (هذا))) return_trace (خطأ);
    إذا (unlikely (!alternate_سيٽ.serialize (c, glyphs.ڊيگهائي))) return_trace (خطأ);
    لكل (بنانشان صحيح i = 0; i < glyphs.ڊيگهائي; i++)
    {
      بنانشان صحيح alternate_len = alternate_len_list[i];
      إذا (unlikely (!alternate_سيٽ[i]
                        .serialize_serialize (c, alternate_glyphs_list.sub_array (0, alternate_len))))
        return_trace (خطأ);
      alternate_glyphs_list += alternate_len;
    }
    return_trace (coverage.serialize_serialize (c, glyphs));
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_سيٽ_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    خودڪار *ٻاهريون = c->serializer->شروعات_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (ٻاهريون))) return_trace (خطأ);
    ٻاهريون->format = format;

    hb_sorted_ويڪٽر_t<hb_codepoint_t> new_coverage;
    + hb_zip (هذا+coverage, alternate_سيٽ)
    | hb_filter (glyphset, hb_پهريون)
    | hb_filter (subset_وٿي_array (c, ٻاهريون->alternate_سيٽ, هذا), hb_ٻيو)
    | hb_map (hb_پهريون)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;
    ٻاهريون->coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (منطقي (new_coverage));
  }
};

}
}
}

#جيڪڏهن_پورو  /* OT_LAYOUT_GSUB_ALTERNATESUBSTFORMAT1_HH */
