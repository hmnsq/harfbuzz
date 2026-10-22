#جيڪڏهن_وصف_نه_ٿيل OT_LAYOUT_GSUB_SINGLESUBSTFORMAT1_HH
#وصف_ڪريو OT_LAYOUT_GSUB_SINGLESUBSTFORMAT1_HH

#ضمن "Common.hh"

نطاق OT {
نطاق Layout {
نطاق GSUB_impl {

ساكن مضمن منطقي
subset_single_subst (hb_subset_context_t *c,
                     ثابت Coverage &coverage,
                     hb_codepoint_t d,
                     hb_codepoint_t نقاب)
{
  ثابت hb_سيٽ_t &glyphset = *c->plan->glyphset_gsub ();
  ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

  hb_سيٽ_t intersection;
  coverage.intersect_سيٽ (glyphset, intersection);

  خودڪار it =
  + hb_iter (intersection)
  | hb_map_retains_ترتيب_ڏيڻ ([d, نقاب] (hb_codepoint_t g) {
                              أرجع hb_codepoint_pair_t (g,
                                                          (g + d) & نقاب); })
  | hb_filter (glyphset, hb_ٻيو)
  | hb_map_retains_ترتيب_ڏيڻ ([&] (hb_codepoint_pair_t p) -> hb_codepoint_pair_t
                            { أرجع hb_pair (glyph_map[p.پهريون], glyph_map[p.second]); })
  ;

  منطقي ret = منطقي (it);
  SingleSubst_serialize (c->serializer, it);
  أرجع ret;
}

HB_اندروني مضمن
فراغ single_subst_depend (hb_depend_context_t *c,
                          ثابت Coverage &coverage,
                          hb_codepoint_t d, hb_codepoint_t نقاب)
{
  بنانشان انگ_glyphs = c->face->get_انگ_glyphs ();

  // Filter coverage by active glyphs (like closure does)
  hb_سيٽ_t intersection;
  coverage.intersect_سيٽ (c->والد_سرگرم_glyphs (), intersection);

  + hb_iter (intersection)
  | hb_map ([&] (hb_codepoint_t _) { أرجع hb_codepoint_pair_t (_, (_ + d) & نقاب); })
  | hb_filter ([انگ_glyphs] (ثابت hb_codepoint_pair_t &_) { أرجع _.second < انگ_glyphs; })
  | hb_apply ([&] (ثابت hb_codepoint_pair_t &_) { c->depend_معلومات->جوڙيو_gsub_lookup(_.پهريون, c->lookup_اشاريو, _.second); })
  ;
}

HB_اندروني مضمن
فراغ single_subst_گهيرو (hb_گهيرو_context_t *c,
                           ثابت Coverage &coverage,
                           hb_codepoint_t d, hb_codepoint_t نقاب)
{
  /* Help fuzzer avoid this function as much. */
  بنانشان ڪڍو = coverage.get_population ();
  إذا (ڪڍو >= نقاب)
    أرجع;

  hb_سيٽ_t intersection;
  coverage.intersect_سيٽ (c->والد_سرگرم_glyphs (), intersection);

  /* In degenerate fuzzer-found fonts, but not real fonts,
   * this table can keep adding new glyphs in each round of closure.
   * Refuse to close-over, if it maps glyph range to overlapping range. */
  hb_codepoint_t min_before = intersection.get_min ();
  hb_codepoint_t max_before = intersection.get_max ();
  hb_codepoint_t min_after = (min_before + d) & نقاب;
  hb_codepoint_t max_after = (max_before + d) & نقاب;
  إذا (intersection.get_population () == max_before - min_before + 1 &&
	((min_before <= min_after && min_after <= max_before) ||
	 (min_before <= max_after && max_after <= max_before)))
    أرجع;

  + hb_iter (intersection)
  | hb_map ([d, نقاب] (hb_codepoint_t g) { أرجع (g + d) & نقاب; })
  | hb_sink (c->output)
  ;
}

HB_اندروني مضمن
فراغ single_subst_collect_glyphs (hb_collect_glyphs_context_t *c,
                                  ثابت Coverage &coverage,
                                  hb_codepoint_t d, hb_codepoint_t نقاب)
{
  إذا (unlikely (!coverage.collect_coverage (c->داخلا_2))) أرجع;

  + hb_iter (coverage)
  | hb_map ([d, نقاب] (hb_codepoint_t g) { أرجع (g + d) & نقاب; })
  | hb_sink (c->output)
  ;
}

سانچو <قسم_نالو Types>
بنية SingleSubstFormat1_3
{
  محمي:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  قسم_نالو Types::سانچو LOffset_منزل<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of Substitution table */
  قسم_نالو Types::HBUINT
                deltaGlyphID;           /* Add to original GlyphID to get
                                         * substitute GlyphID, modulo 0x10000 */

  عام:
  DEFINE_سائيز_STATIC (2 + Types::LOffset::static_سائيز + Types::HBUINT::static_سائيز);

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
                  coverage.sanitize (c, هذا) &&
                  /* The coverage  table may use a range to represent a set
                   * of glyphs, which means a small number of bytes can
                   * generate a large glyph set. Manually modify the
                   * sanitizer max ops to take this into account.
                   *
                   * Note: This check *must* be right after coverage sanitize. */
                  c->check_ops ((هذا + coverage).get_population () >> 1));
  }

  hb_codepoint_t get_نقاب () ثابت
  { أرجع (1 << (8 * Types::سائيز)) - 1; }

  منطقي intersects (ثابت hb_سيٽ_t *glyphs) ثابت
  { أرجع (هذا+coverage).intersects (glyphs); }

  منطقي may_have_non_1to1 () ثابت
  { أرجع خطأ; }

  فراغ depend (hb_depend_context_t *c) ثابت
  {
    single_subst_depend (c, هذا+coverage, deltaGlyphID, get_نقاب ());
  }

  فراغ گهيرو (hb_گهيرو_context_t *c) ثابت
  {
    single_subst_گهيرو (c, هذا+coverage, deltaGlyphID, get_نقاب ());
  }

  فراغ گهيرو_lookups (hb_گهيرو_lookups_context_t *c) ثابت {}

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    single_subst_collect_glyphs (c, هذا+coverage, deltaGlyphID, get_نقاب ());
  }

  ثابت Coverage &get_coverage () ثابت { أرجع هذا+coverage; }

  منطقي would_apply (hb_would_apply_context_t *c) ثابت
  { أرجع c->len == 1 && (هذا+coverage).get_coverage (c->glyphs[0]) != NOT_COVERED; }

  بنانشان
  get_glyph_alternates (hb_codepoint_t  glyph_id,
                        بنانشان        شروعات_وٿي,
                        بنانشان       *alternate_ڳڻپ  /* IN/OUT.  May be NULL. */,
                        hb_codepoint_t *alternate_glyphs /* OUT.     May be NULL. */) ثابت
  {
    بنانشان صحيح اشاريو_2 = (هذا+coverage).get_coverage (glyph_id);
    إذا (likely (اشاريو_2 == NOT_COVERED))
    {
      إذا (alternate_ڳڻپ)
        *alternate_ڳڻپ = 0;
      أرجع 0;
    }

    إذا (alternate_ڳڻپ && *alternate_ڳڻپ && alternate_glyphs)
    {
      hb_codepoint_t d = deltaGlyphID;
      hb_codepoint_t نقاب = get_نقاب ();

      glyph_id = (glyph_id + d) & نقاب;

      *alternate_glyphs = glyph_id;
      *alternate_ڳڻپ = 1;
    }

    أرجع 1;
  }

  فراغ
  collect_glyph_alternates (hb_map_t  *alternate_ڳڻپ /* IN/OUT */,
			    hb_map_t  *alternate_glyphs /* IN/OUT */) ثابت
  {
    hb_codepoint_t d = deltaGlyphID;
    hb_codepoint_t نقاب = get_نقاب ();

    + hb_iter (هذا+coverage)
    | hb_map ([d, نقاب] (hb_codepoint_t g) { أرجع hb_pair (g, (g + d) & نقاب); })
    | hb_apply ([&] (ثابت hb_pair_t<hb_codepoint_t, hb_codepoint_t> &p) -> فراغ
		{ hb_collect_glyph_alternates_جوڙيو (p.پهريون, p.second,
						    alternate_ڳڻپ, alternate_glyphs); })
    ;
  }

  منطقي apply (hb_ot_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);
    hb_codepoint_t glyph_id = c->بفر->cur().codepoint;
    بنانشان صحيح اشاريو_2 = (هذا+coverage).get_coverage (glyph_id);
    إذا (اشاريو_2 == NOT_COVERED) return_trace (خطأ);

    hb_codepoint_t d = deltaGlyphID;
    hb_codepoint_t نقاب = get_نقاب ();

    glyph_id = (glyph_id + d) & نقاب;

    إذا (HB_بفر_پيغام_وڌيڪ && c->بفر->messaging ())
    {
      c->بفر->sync_so_far ();
      c->بفر->پيغام (c->font,
			  "replacing glyph at %u (single substitution)",
			  c->بفر->idx);
    }

    c->replace_glyph (glyph_id);

    إذا (HB_بفر_پيغام_وڌيڪ && c->بفر->messaging ())
    {
      c->بفر->پيغام (c->font,
			  "replaced glyph at %u (single substitution)",
			  c->بفر->idx - 1u);
    }

    return_trace (سچ);
  }

  سانچو<قسم_نالو Iterator,
           hb_requires (hb_is_sorted_ذريعو_جو (Iterator, hb_codepoint_t))>
  منطقي serialize (hb_serialize_context_t *c,
                  Iterator glyphs,
                  بنانشان delta)
  {
    TRACE_SERIALIZE (هذا);
    إذا (unlikely (!c->extend_min (هذا))) return_trace (خطأ);
    إذا (unlikely (!coverage.serialize_serialize (c, glyphs))) return_trace (خطأ);
    c->check_سونپيو (deltaGlyphID, delta, HB_SERIALIZE_ERROR_INT_حد_کان_مٿي);
    return_trace (سچ);
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    return_trace (subset_single_subst (c, هذا+coverage, deltaGlyphID, get_نقاب ()));
  }
};

}
}
}


#جيڪڏهن_پورو /* OT_LAYOUT_GSUB_SINGLESUBSTFORMAT1_HH */
