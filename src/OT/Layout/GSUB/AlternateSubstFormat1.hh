#که_نه_تعریف_شوی OT_LAYOUT_GSUB_ALTERNATESUBSTFORMAT1_HH
#تعریف_کړه OT_LAYOUT_GSUB_ALTERNATESUBSTFORMAT1_HH

#ضمن "AlternateSet.hh"
#ضمن "Common.hh"

نطاق OT {
نطاق Layout {
نطاق GSUB_impl {

کينډۍ <ډول_نوم Types>
بنية AlternateSubstFormat1_2
{
  محمي:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  ډول_نوم Types::کينډۍ LOffset_منزل<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of Substitution table */
  ډول_نوم Types::کينډۍ Array_د<ډول_نوم Types::کينډۍ دننه_واټن_منزل<Alternate_سېټ<Types>>>
                alternate_سېټ;           /* Array of AlternateSet tables
                                         * ordered by Coverage Index */
  عام:
  DEFINE_اندازه_ARRAY (2 + Types::LOffset::static_اندازه + Types::HBUINT::static_اندازه, alternate_سېټ);

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (coverage.sanitize (c, هذا) && alternate_سېټ.sanitize (c, هذا));
  }

  منطقي intersects (ثابت hb_سېټ_t *glyphs) ثابت
  { أرجع (هذا+coverage).intersects (glyphs); }

  منطقي may_have_non_1to1 () ثابت
  { أرجع خطأ; }

  فراغ depend (hb_depend_context_t *c) ثابت
  {
    // Filter by parent_active_glyphs like closure does
    + hb_zip (هذا+coverage, alternate_سېټ)
    | hb_filter (c->پلار_فعال_glyphs (), hb_لومړی)
    | hb_apply ([&] (ثابت hb_pair_t<hb_codepoint_t, ثابت ډول_نوم Types::کينډۍ دننه_واټن_منزل<Alternate_سېټ<Types>>&> &_)
                {
                  ثابت Alternate_سېټ<Types>& as = هذا+_.second;
                  as.depend (c, _.لومړی);
                })
    ;
  }

  فراغ تړون (hb_تړون_context_t *c) ثابت
  {
    + hb_zip (هذا+coverage, alternate_سېټ)
    | hb_filter (c->پلار_فعال_glyphs (), hb_لومړی)
    | hb_map (hb_دوهم)
    | hb_map (hb_جمع_کړه (هذا))
    | hb_apply ([c] (ثابت Alternate_سېټ<Types> &_) { _.تړون (c); })
    ;
  }

  فراغ تړون_lookups (hb_تړون_lookups_context_t *c) ثابت {}

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    إذا (unlikely (!(هذا+coverage).collect_coverage (c->ننوت))) أرجع;
    + hb_zip (هذا+coverage, alternate_سېټ)
    | hb_map (hb_دوهم)
    | hb_map (hb_جمع_کړه (هذا))
    | hb_apply ([c] (ثابت Alternate_سېټ<Types> &_) { _.collect_glyphs (c); })
    ;
  }

  ثابت Coverage &get_coverage () ثابت { أرجع هذا+coverage; }

  منطقي would_apply (hb_would_apply_context_t *c) ثابت
  { أرجع c->len == 1 && (هذا+coverage).get_coverage (c->glyphs[0]) != NOT_COVERED; }

  بینښه
  get_glyph_alternates (hb_codepoint_t  gid,
                        بینښه        پیل_دننه_واټن,
                        بینښه       *alternate_شمېر  /* IN/OUT.  May be NULL. */,
                        hb_codepoint_t *alternate_glyphs /* OUT.     May be NULL. */) ثابت
  { أرجع (هذا+alternate_سېټ[(هذا+coverage).get_coverage (gid)])
           .get_alternates (پیل_دننه_واټن, alternate_شمېر, alternate_glyphs); }

  فراغ
  collect_glyph_alternates (hb_map_t  *alternate_شمېر /* IN/OUT */,
			    hb_map_t  *alternate_glyphs /* IN/OUT */) ثابت
  {
    + hb_iter (alternate_سېټ)
    | hb_map (hb_جمع_کړه (هذا))
    | hb_zip (هذا+coverage)
    | hb_apply ([&] (ثابت hb_pair_t<ثابت Alternate_سېټ<Types> &, hb_codepoint_t> _) {
		  _.لومړی.collect_alternates (_.second, alternate_شمېر, alternate_glyphs);
		})
    ;
  }

  منطقي apply (hb_ot_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);

    بینښه صحيح شاخص_2 = (هذا+coverage).get_coverage (c->بفر->cur().codepoint);
    إذا (شاخص_2 == NOT_COVERED) return_trace (خطأ);

    return_trace ((هذا+alternate_سېټ[شاخص_2]).apply (c));
  }

  کينډۍ <ډول_نوم GlyphID>
  منطقي serialize (hb_serialize_context_t *c,
                  hb_sorted_array_t<GlyphID> glyphs,
                  hb_array_t<ثابت بینښه صحيح> alternate_len_list,
                  hb_array_t<GlyphID> alternate_glyphs_list)
  {
    TRACE_SERIALIZE (هذا);
    إذا (unlikely (!c->extend_min (هذا))) return_trace (خطأ);
    إذا (unlikely (!alternate_سېټ.serialize (c, glyphs.اوږدتیا))) return_trace (خطأ);
    لكل (بینښه صحيح i = 0; i < glyphs.اوږدتیا; i++)
    {
      بینښه صحيح alternate_len = alternate_len_list[i];
      إذا (unlikely (!alternate_سېټ[i]
                        .serialize_serialize (c, alternate_glyphs_list.sub_array (0, alternate_len))))
        return_trace (خطأ);
      alternate_glyphs_list += alternate_len;
    }
    return_trace (coverage.serialize_serialize (c, glyphs));
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_سېټ_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    خپلکاری *بهر = c->serializer->پیل_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (بهر))) return_trace (خطأ);
    بهر->format = format;

    hb_sorted_ویکټور_t<hb_codepoint_t> new_coverage;
    + hb_zip (هذا+coverage, alternate_سېټ)
    | hb_filter (glyphset, hb_لومړی)
    | hb_filter (subset_دننه_واټن_array (c, بهر->alternate_سېټ, هذا), hb_دوهم)
    | hb_map (hb_لومړی)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;
    بهر->coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (منطقي (new_coverage));
  }
};

}
}
}

#د_که_پای  /* OT_LAYOUT_GSUB_ALTERNATESUBSTFORMAT1_HH */
