#جے_بنا_تعریف OT_LAYOUT_GSUB_ALTERNATESUBSTFORMAT1_HH
#تعریف_کرو OT_LAYOUT_GSUB_ALTERNATESUBSTFORMAT1_HH

#ضمن "AlternateSet.hh"
#ضمن "Common.hh"

نطاق OT {
نطاق Layout {
نطاق GSUB_impl {

سانچا <قسم_ناں Types>
بنية AlternateSubstFormat1_2
{
  محمي:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  قسم_ناں Types::سانچا LOffset_منزل<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of Substitution table */
  قسم_ناں Types::سانچا Array_دا<قسم_ناں Types::سانچا وتھ_منزل<Alternate_سیٹ<Types>>>
                alternate_سیٹ;           /* Array of AlternateSet tables
                                         * ordered by Coverage Index */
  عام:
  DEFINE_ناپ_ARRAY (2 + Types::LOffset::static_ناپ + Types::HBUINT::static_ناپ, alternate_سیٹ);

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (coverage.sanitize (c, هذا) && alternate_سیٹ.sanitize (c, هذا));
  }

  منطقي intersects (ثابت hb_سیٹ_t *glyphs) ثابت
  { أرجع (هذا+coverage).intersects (glyphs); }

  منطقي may_have_non_1to1 () ثابت
  { أرجع خطأ; }

  فراغ depend (hb_depend_context_t *c) ثابت
  {
    // Filter by parent_active_glyphs like closure does
    + hb_zip (هذا+coverage, alternate_سیٹ)
    | hb_filter (c->ماپا_سرگرم_glyphs (), hb_پہلا)
    | hb_apply ([&] (ثابت hb_pair_t<hb_codepoint_t, ثابت قسم_ناں Types::سانچا وتھ_منزل<Alternate_سیٹ<Types>>&> &_)
                {
                  ثابت Alternate_سیٹ<Types>& as = هذا+_.second;
                  as.depend (c, _.پہلا);
                })
    ;
  }

  فراغ گھیرا (hb_گھیرا_context_t *c) ثابت
  {
    + hb_zip (هذا+coverage, alternate_سیٹ)
    | hb_filter (c->ماپا_سرگرم_glyphs (), hb_پہلا)
    | hb_map (hb_دوجا)
    | hb_map (hb_جوڑو (هذا))
    | hb_apply ([c] (ثابت Alternate_سیٹ<Types> &_) { _.گھیرا (c); })
    ;
  }

  فراغ گھیرا_lookups (hb_گھیرا_lookups_context_t *c) ثابت {}

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    إذا (unlikely (!(هذا+coverage).collect_coverage (c->داخلہ))) أرجع;
    + hb_zip (هذا+coverage, alternate_سیٹ)
    | hb_map (hb_دوجا)
    | hb_map (hb_جوڑو (هذا))
    | hb_apply ([c] (ثابت Alternate_سیٹ<Types> &_) { _.collect_glyphs (c); })
    ;
  }

  ثابت Coverage &get_coverage () ثابت { أرجع هذا+coverage; }

  منطقي would_apply (hb_would_apply_context_t *c) ثابت
  { أرجع c->len == 1 && (هذا+coverage).get_coverage (c->glyphs[0]) != NOT_COVERED; }

  بنانشانی
  get_glyph_alternates (hb_codepoint_t  gid,
                        بنانشانی        شروع_وتھ,
                        بنانشانی       *alternate_گنتی  /* IN/OUT.  May be NULL. */,
                        hb_codepoint_t *alternate_glyphs /* OUT.     May be NULL. */) ثابت
  { أرجع (هذا+alternate_سیٹ[(هذا+coverage).get_coverage (gid)])
           .get_alternates (شروع_وتھ, alternate_گنتی, alternate_glyphs); }

  فراغ
  collect_glyph_alternates (hb_map_t  *alternate_گنتی /* IN/OUT */,
			    hb_map_t  *alternate_glyphs /* IN/OUT */) ثابت
  {
    + hb_iter (alternate_سیٹ)
    | hb_map (hb_جوڑو (هذا))
    | hb_zip (هذا+coverage)
    | hb_apply ([&] (ثابت hb_pair_t<ثابت Alternate_سیٹ<Types> &, hb_codepoint_t> _) {
		  _.پہلا.collect_alternates (_.second, alternate_گنتی, alternate_glyphs);
		})
    ;
  }

  منطقي apply (hb_ot_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);

    بنانشانی صحيح اشاریہ_2 = (هذا+coverage).get_coverage (c->بفر->cur().codepoint);
    إذا (اشاریہ_2 == NOT_COVERED) return_trace (خطأ);

    return_trace ((هذا+alternate_سیٹ[اشاریہ_2]).apply (c));
  }

  سانچا <قسم_ناں GlyphID>
  منطقي serialize (hb_serialize_context_t *c,
                  hb_sorted_array_t<GlyphID> glyphs,
                  hb_array_t<ثابت بنانشانی صحيح> alternate_len_list,
                  hb_array_t<GlyphID> alternate_glyphs_list)
  {
    TRACE_SERIALIZE (هذا);
    إذا (unlikely (!c->extend_min (هذا))) return_trace (خطأ);
    إذا (unlikely (!alternate_سیٹ.serialize (c, glyphs.لمائی))) return_trace (خطأ);
    لكل (بنانشانی صحيح i = 0; i < glyphs.لمائی; i++)
    {
      بنانشانی صحيح alternate_len = alternate_len_list[i];
      إذا (unlikely (!alternate_سیٹ[i]
                        .serialize_serialize (c, alternate_glyphs_list.sub_array (0, alternate_len))))
        return_trace (خطأ);
      alternate_glyphs_list += alternate_len;
    }
    return_trace (coverage.serialize_serialize (c, glyphs));
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_سیٹ_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    خودکار *باہرلا = c->serializer->شروع_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (باہرلا))) return_trace (خطأ);
    باہرلا->format = format;

    hb_sorted_سمتیہ_t<hb_codepoint_t> new_coverage;
    + hb_zip (هذا+coverage, alternate_سیٹ)
    | hb_filter (glyphset, hb_پہلا)
    | hb_filter (subset_وتھ_array (c, باہرلا->alternate_سیٹ, هذا), hb_دوجا)
    | hb_map (hb_پہلا)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;
    باہرلا->coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (منطقي (new_coverage));
  }
};

}
}
}

#جے_انت  /* OT_LAYOUT_GSUB_ALTERNATESUBSTFORMAT1_HH */
