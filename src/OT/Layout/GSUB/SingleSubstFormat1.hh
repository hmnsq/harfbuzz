#که_نه_تعریف_شوی OT_LAYOUT_GSUB_SINGLESUBSTFORMAT1_HH
#تعریف_کړه OT_LAYOUT_GSUB_SINGLESUBSTFORMAT1_HH

#ضمن "Common.hh"

نطاق OT {
نطاق Layout {
نطاق GSUB_impl {

ساكن مضمن منطقي
subset_single_subst (hb_subset_context_t *c,
                     ثابت Coverage &coverage,
                     hb_codepoint_t d,
                     hb_codepoint_t ماسک)
{
  ثابت hb_سېټ_t &glyphset = *c->plan->glyphset_gsub ();
  ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

  hb_سېټ_t intersection;
  coverage.intersect_سېټ (glyphset, intersection);

  خپلکاری it =
  + hb_iter (intersection)
  | hb_map_retains_ترتیبونه ([d, ماسک] (hb_codepoint_t g) {
                              أرجع hb_codepoint_pair_t (g,
                                                          (g + d) & ماسک); })
  | hb_filter (glyphset, hb_دوهم)
  | hb_map_retains_ترتیبونه ([&] (hb_codepoint_pair_t p) -> hb_codepoint_pair_t
                            { أرجع hb_pair (glyph_map[p.لومړی], glyph_map[p.second]); })
  ;

  منطقي ret = منطقي (it);
  SingleSubst_serialize (c->serializer, it);
  أرجع ret;
}

HB_دننی مضمن
فراغ single_subst_depend (hb_depend_context_t *c,
                          ثابت Coverage &coverage,
                          hb_codepoint_t d, hb_codepoint_t ماسک)
{
  بینښه عدد_glyphs = c->face->get_عدد_glyphs ();

  // Filter coverage by active glyphs (like closure does)
  hb_سېټ_t intersection;
  coverage.intersect_سېټ (c->پلار_فعال_glyphs (), intersection);

  + hb_iter (intersection)
  | hb_map ([&] (hb_codepoint_t _) { أرجع hb_codepoint_pair_t (_, (_ + d) & ماسک); })
  | hb_filter ([عدد_glyphs] (ثابت hb_codepoint_pair_t &_) { أرجع _.second < عدد_glyphs; })
  | hb_apply ([&] (ثابت hb_codepoint_pair_t &_) { c->depend_معلومات->جمع_کړه_gsub_lookup(_.لومړی, c->lookup_شاخص, _.second); })
  ;
}

HB_دننی مضمن
فراغ single_subst_تړون (hb_تړون_context_t *c,
                           ثابت Coverage &coverage,
                           hb_codepoint_t d, hb_codepoint_t ماسک)
{
  /* Help fuzzer avoid this function as much. */
  بینښه راوباسه = coverage.get_population ();
  إذا (راوباسه >= ماسک)
    أرجع;

  hb_سېټ_t intersection;
  coverage.intersect_سېټ (c->پلار_فعال_glyphs (), intersection);

  /* In degenerate fuzzer-found fonts, but not real fonts,
   * this table can keep adding new glyphs in each round of closure.
   * Refuse to close-over, if it maps glyph range to overlapping range. */
  hb_codepoint_t min_before = intersection.get_min ();
  hb_codepoint_t max_before = intersection.get_max ();
  hb_codepoint_t min_after = (min_before + d) & ماسک;
  hb_codepoint_t max_after = (max_before + d) & ماسک;
  إذا (intersection.get_population () == max_before - min_before + 1 &&
	((min_before <= min_after && min_after <= max_before) ||
	 (min_before <= max_after && max_after <= max_before)))
    أرجع;

  + hb_iter (intersection)
  | hb_map ([d, ماسک] (hb_codepoint_t g) { أرجع (g + d) & ماسک; })
  | hb_sink (c->output)
  ;
}

HB_دننی مضمن
فراغ single_subst_collect_glyphs (hb_collect_glyphs_context_t *c,
                                  ثابت Coverage &coverage,
                                  hb_codepoint_t d, hb_codepoint_t ماسک)
{
  إذا (unlikely (!coverage.collect_coverage (c->ننوت))) أرجع;

  + hb_iter (coverage)
  | hb_map ([d, ماسک] (hb_codepoint_t g) { أرجع (g + d) & ماسک; })
  | hb_sink (c->output)
  ;
}

کينډۍ <ډول_نوم Types>
بنية SingleSubstFormat1_3
{
  محمي:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  ډول_نوم Types::کينډۍ LOffset_منزل<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of Substitution table */
  ډول_نوم Types::HBUINT
                deltaGlyphID;           /* Add to original GlyphID to get
                                         * substitute GlyphID, modulo 0x10000 */

  عام:
  DEFINE_اندازه_STATIC (2 + Types::LOffset::static_اندازه + Types::HBUINT::static_اندازه);

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

  hb_codepoint_t get_ماسک () ثابت
  { أرجع (1 << (8 * Types::اندازه)) - 1; }

  منطقي intersects (ثابت hb_سېټ_t *glyphs) ثابت
  { أرجع (هذا+coverage).intersects (glyphs); }

  منطقي may_have_non_1to1 () ثابت
  { أرجع خطأ; }

  فراغ depend (hb_depend_context_t *c) ثابت
  {
    single_subst_depend (c, هذا+coverage, deltaGlyphID, get_ماسک ());
  }

  فراغ تړون (hb_تړون_context_t *c) ثابت
  {
    single_subst_تړون (c, هذا+coverage, deltaGlyphID, get_ماسک ());
  }

  فراغ تړون_lookups (hb_تړون_lookups_context_t *c) ثابت {}

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    single_subst_collect_glyphs (c, هذا+coverage, deltaGlyphID, get_ماسک ());
  }

  ثابت Coverage &get_coverage () ثابت { أرجع هذا+coverage; }

  منطقي would_apply (hb_would_apply_context_t *c) ثابت
  { أرجع c->len == 1 && (هذا+coverage).get_coverage (c->glyphs[0]) != NOT_COVERED; }

  بینښه
  get_glyph_alternates (hb_codepoint_t  glyph_id,
                        بینښه        پیل_دننه_واټن,
                        بینښه       *alternate_شمېر  /* IN/OUT.  May be NULL. */,
                        hb_codepoint_t *alternate_glyphs /* OUT.     May be NULL. */) ثابت
  {
    بینښه صحيح شاخص_2 = (هذا+coverage).get_coverage (glyph_id);
    إذا (likely (شاخص_2 == NOT_COVERED))
    {
      إذا (alternate_شمېر)
        *alternate_شمېر = 0;
      أرجع 0;
    }

    إذا (alternate_شمېر && *alternate_شمېر && alternate_glyphs)
    {
      hb_codepoint_t d = deltaGlyphID;
      hb_codepoint_t ماسک = get_ماسک ();

      glyph_id = (glyph_id + d) & ماسک;

      *alternate_glyphs = glyph_id;
      *alternate_شمېر = 1;
    }

    أرجع 1;
  }

  فراغ
  collect_glyph_alternates (hb_map_t  *alternate_شمېر /* IN/OUT */,
			    hb_map_t  *alternate_glyphs /* IN/OUT */) ثابت
  {
    hb_codepoint_t d = deltaGlyphID;
    hb_codepoint_t ماسک = get_ماسک ();

    + hb_iter (هذا+coverage)
    | hb_map ([d, ماسک] (hb_codepoint_t g) { أرجع hb_pair (g, (g + d) & ماسک); })
    | hb_apply ([&] (ثابت hb_pair_t<hb_codepoint_t, hb_codepoint_t> &p) -> فراغ
		{ hb_collect_glyph_alternates_جمع_کړه (p.لومړی, p.second,
						    alternate_شمېر, alternate_glyphs); })
    ;
  }

  منطقي apply (hb_ot_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);
    hb_codepoint_t glyph_id = c->بفر->cur().codepoint;
    بینښه صحيح شاخص_2 = (هذا+coverage).get_coverage (glyph_id);
    إذا (شاخص_2 == NOT_COVERED) return_trace (خطأ);

    hb_codepoint_t d = deltaGlyphID;
    hb_codepoint_t ماسک = get_ماسک ();

    glyph_id = (glyph_id + d) & ماسک;

    إذا (HB_بفر_پیغام_نور && c->بفر->messaging ())
    {
      c->بفر->sync_so_far ();
      c->بفر->پیغام (c->font,
			  "replacing glyph at %u (single substitution)",
			  c->بفر->idx);
    }

    c->replace_glyph (glyph_id);

    إذا (HB_بفر_پیغام_نور && c->بفر->messaging ())
    {
      c->بفر->پیغام (c->font,
			  "replaced glyph at %u (single substitution)",
			  c->بفر->idx - 1u);
    }

    return_trace (ریښتیا);
  }

  کينډۍ<ډول_نوم Iterator,
           hb_requires (hb_is_sorted_سرچینه_د (Iterator, hb_codepoint_t))>
  منطقي serialize (hb_serialize_context_t *c,
                  Iterator glyphs,
                  بینښه delta)
  {
    TRACE_SERIALIZE (هذا);
    إذا (unlikely (!c->extend_min (هذا))) return_trace (خطأ);
    إذا (unlikely (!coverage.serialize_serialize (c, glyphs))) return_trace (خطأ);
    c->check_ټاکل (deltaGlyphID, delta, HB_SERIALIZE_ERROR_INT_ډکېدنه);
    return_trace (ریښتیا);
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    return_trace (subset_single_subst (c, هذا+coverage, deltaGlyphID, get_ماسک ()));
  }
};

}
}
}


#د_که_پای /* OT_LAYOUT_GSUB_SINGLESUBSTFORMAT1_HH */
