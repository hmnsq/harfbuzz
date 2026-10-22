#اگر_تعریف_نشده OT_LAYOUT_GSUB_ALTERNATESUBSTFORMAT1_HH
#تعریف_کن OT_LAYOUT_GSUB_ALTERNATESUBSTFORMAT1_HH

#ضمن "AlternateSet.hh"
#ضمن "Common.hh"

نطاق OT {
نطاق Layout {
نطاق GSUB_impl {

قالب <نام_نوع Types>
بنية AlternateSubstFormat1_2
{
  محمي:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  نام_نوع Types::قالب LOffset_مقصد<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of Substitution table */
  نام_نوع Types::قالب Array_از<نام_نوع Types::قالب جابه_جایی_مقصد<Alternate_مجموعه<Types>>>
                alternate_مجموعه;           /* Array of AlternateSet tables
                                         * ordered by Coverage Index */
  عام:
  DEFINE_اندازه_ARRAY (2 + Types::LOffset::static_اندازه + Types::HBUINT::static_اندازه, alternate_مجموعه);

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (coverage.sanitize (c, هذا) && alternate_مجموعه.sanitize (c, هذا));
  }

  منطقي intersects (ثابت hb_مجموعه_t *glyphs) ثابت
  { أرجع (هذا+coverage).intersects (glyphs); }

  منطقي may_have_non_1to1 () ثابت
  { أرجع خطأ; }

  فراغ depend (hb_depend_context_t *c) ثابت
  {
    // Filter by parent_active_glyphs like closure does
    + hb_zip (هذا+coverage, alternate_مجموعه)
    | hb_filter (c->والد_فعال_glyphs (), hb_اولی)
    | hb_apply ([&] (ثابت hb_pair_t<hb_codepoint_t, ثابت نام_نوع Types::قالب جابه_جایی_مقصد<Alternate_مجموعه<Types>>&> &_)
                {
                  ثابت Alternate_مجموعه<Types>& as = هذا+_.second;
                  as.depend (c, _.اولی);
                })
    ;
  }

  فراغ بستگی (hb_بستگی_context_t *c) ثابت
  {
    + hb_zip (هذا+coverage, alternate_مجموعه)
    | hb_filter (c->والد_فعال_glyphs (), hb_اولی)
    | hb_map (hb_دومی)
    | hb_map (hb_جمع_کن (هذا))
    | hb_apply ([c] (ثابت Alternate_مجموعه<Types> &_) { _.بستگی (c); })
    ;
  }

  فراغ بستگی_lookups (hb_بستگی_lookups_context_t *c) ثابت {}

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    إذا (unlikely (!(هذا+coverage).collect_coverage (c->درون_داد))) أرجع;
    + hb_zip (هذا+coverage, alternate_مجموعه)
    | hb_map (hb_دومی)
    | hb_map (hb_جمع_کن (هذا))
    | hb_apply ([c] (ثابت Alternate_مجموعه<Types> &_) { _.collect_glyphs (c); })
    ;
  }

  ثابت Coverage &get_coverage () ثابت { أرجع هذا+coverage; }

  منطقي would_apply (hb_would_apply_context_t *c) ثابت
  { أرجع c->len == 1 && (هذا+coverage).get_coverage (c->glyphs[0]) != NOT_COVERED; }

  بیعلامت
  get_glyph_alternates (hb_codepoint_t  gid,
                        بیعلامت        شروع_جابه_جایی,
                        بیعلامت       *alternate_شمار  /* IN/OUT.  May be NULL. */,
                        hb_codepoint_t *alternate_glyphs /* OUT.     May be NULL. */) ثابت
  { أرجع (هذا+alternate_مجموعه[(هذا+coverage).get_coverage (gid)])
           .get_alternates (شروع_جابه_جایی, alternate_شمار, alternate_glyphs); }

  فراغ
  collect_glyph_alternates (hb_map_t  *alternate_شمار /* IN/OUT */,
			    hb_map_t  *alternate_glyphs /* IN/OUT */) ثابت
  {
    + hb_iter (alternate_مجموعه)
    | hb_map (hb_جمع_کن (هذا))
    | hb_zip (هذا+coverage)
    | hb_apply ([&] (ثابت hb_pair_t<ثابت Alternate_مجموعه<Types> &, hb_codepoint_t> _) {
		  _.اولی.collect_alternates (_.second, alternate_شمار, alternate_glyphs);
		})
    ;
  }

  منطقي apply (hb_ot_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);

    بیعلامت صحيح شاخص_2 = (هذا+coverage).get_coverage (c->میانگیر->cur().codepoint);
    إذا (شاخص_2 == NOT_COVERED) return_trace (خطأ);

    return_trace ((هذا+alternate_مجموعه[شاخص_2]).apply (c));
  }

  قالب <نام_نوع GlyphID>
  منطقي serialize (hb_serialize_context_t *c,
                  hb_sorted_array_t<GlyphID> glyphs,
                  hb_array_t<ثابت بیعلامت صحيح> alternate_len_list,
                  hb_array_t<GlyphID> alternate_glyphs_list)
  {
    TRACE_SERIALIZE (هذا);
    إذا (unlikely (!c->extend_min (هذا))) return_trace (خطأ);
    إذا (unlikely (!alternate_مجموعه.serialize (c, glyphs.درازی))) return_trace (خطأ);
    لكل (بیعلامت صحيح i = 0; i < glyphs.درازی; i++)
    {
      بیعلامت صحيح alternate_len = alternate_len_list[i];
      إذا (unlikely (!alternate_مجموعه[i]
                        .serialize_serialize (c, alternate_glyphs_list.sub_array (0, alternate_len))))
        return_trace (خطأ);
      alternate_glyphs_list += alternate_len;
    }
    return_trace (coverage.serialize_serialize (c, glyphs));
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_مجموعه_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    خودکار *بیرون = c->serializer->شروع_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (بیرون))) return_trace (خطأ);
    بیرون->format = format;

    hb_sorted_وکتور_t<hb_codepoint_t> new_coverage;
    + hb_zip (هذا+coverage, alternate_مجموعه)
    | hb_filter (glyphset, hb_اولی)
    | hb_filter (subset_جابه_جایی_array (c, بیرون->alternate_مجموعه, هذا), hb_دومی)
    | hb_map (hb_اولی)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;
    بیرون->coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (منطقي (new_coverage));
  }
};

}
}
}

#پایان_اگر  /* OT_LAYOUT_GSUB_ALTERNATESUBSTFORMAT1_HH */
