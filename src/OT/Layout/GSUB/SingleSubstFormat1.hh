#اگر_غیر_تعریف_شدہ OT_LAYOUT_GSUB_SINGLESUBSTFORMAT1_HH
#تعریف_کرو OT_LAYOUT_GSUB_SINGLESUBSTFORMAT1_HH

#شامل "Common.hh"

نامخلا OT {
نامخلا Layout {
نامخلا GSUB_impl {

ساکن اندرونی منطقی
subset_single_subst (hb_subset_context_t *c,
                     ثابت Coverage &coverage,
                     hb_codepoint_t d,
                     hb_codepoint_t نقاب)
{
  ثابت hb_سیٹ_t &glyphset = *c->plan->glyphset_gsub ();
  ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

  hb_سیٹ_t intersection;
  coverage.intersect_سیٹ (glyphset, intersection);

  خودکار it =
  + hb_iter (intersection)
  | hb_map_retains_ترتیب_کاری ([d, نقاب] (hb_codepoint_t g) {
                              واپس hb_codepoint_pair_t (g,
                                                          (g + d) & نقاب); })
  | hb_filter (glyphset, hb_دوسرا)
  | hb_map_retains_ترتیب_کاری ([&] (hb_codepoint_pair_t p) -> hb_codepoint_pair_t
                            { واپس hb_pair (glyph_map[p.پہلا], glyph_map[p.second]); })
  ;

  منطقی ret = منطقی (it);
  SingleSubst_serialize (c->serializer, it);
  واپس ret;
}

HB_اندرونی اندرونی
خالی single_subst_depend (hb_depend_context_t *c,
                          ثابت Coverage &coverage,
                          hb_codepoint_t d, hb_codepoint_t نقاب)
{
  غیرعلامتی عدد_glyphs = c->face->get_عدد_glyphs ();

  // Filter coverage by active glyphs (like closure does)
  hb_سیٹ_t intersection;
  coverage.intersect_سیٹ (c->والد_فعال_glyphs (), intersection);

  + hb_iter (intersection)
  | hb_map ([&] (hb_codepoint_t _) { واپس hb_codepoint_pair_t (_, (_ + d) & نقاب); })
  | hb_filter ([عدد_glyphs] (ثابت hb_codepoint_pair_t &_) { واپس _.second < عدد_glyphs; })
  | hb_apply ([&] (ثابت hb_codepoint_pair_t &_) { c->depend_ڈیٹا->جوڑو_gsub_lookup(_.پہلا, c->lookup_اشاریہ, _.second); })
  ;
}

HB_اندرونی اندرونی
خالی single_subst_احاطہ (hb_احاطہ_context_t *c,
                           ثابت Coverage &coverage,
                           hb_codepoint_t d, hb_codepoint_t نقاب)
{
  /* Help fuzzer avoid this function as much. */
  غیرعلامتی نکالو = coverage.get_population ();
  اگر (نکالو >= نقاب)
    واپس;

  hb_سیٹ_t intersection;
  coverage.intersect_سیٹ (c->والد_فعال_glyphs (), intersection);

  /* In degenerate fuzzer-found fonts, but not real fonts,
   * this table can keep adding new glyphs in each round of closure.
   * Refuse to close-over, if it maps glyph range to overlapping range. */
  hb_codepoint_t min_before = intersection.get_min ();
  hb_codepoint_t max_before = intersection.get_max ();
  hb_codepoint_t min_after = (min_before + d) & نقاب;
  hb_codepoint_t max_after = (max_before + d) & نقاب;
  اگر (intersection.get_population () == max_before - min_before + 1 &&
	((min_before <= min_after && min_after <= max_before) ||
	 (min_before <= max_after && max_after <= max_before)))
    واپس;

  + hb_iter (intersection)
  | hb_map ([d, نقاب] (hb_codepoint_t g) { واپس (g + d) & نقاب; })
  | hb_sink (c->output)
  ;
}

HB_اندرونی اندرونی
خالی single_subst_collect_glyphs (hb_collect_glyphs_context_t *c,
                                  ثابت Coverage &coverage,
                                  hb_codepoint_t d, hb_codepoint_t نقاب)
{
  اگر (unlikely (!coverage.collect_coverage (c->داخلہ))) واپس;

  + hb_iter (coverage)
  | hb_map ([d, نقاب] (hb_codepoint_t g) { واپس (g + d) & نقاب; })
  | hb_sink (c->output)
  ;
}

سانچہ <قسم_نام Types>
ساخت SingleSubstFormat1_3
{
  محفوظ:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  قسم_نام Types::سانچہ LOffset_مقصد<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of Substitution table */
  قسم_نام Types::HBUINT
                deltaGlyphID;           /* Add to original GlyphID to get
                                         * substitute GlyphID, modulo 0x10000 */

  عوامی:
  DEFINE_سائز_STATIC (2 + Types::LOffset::static_سائز + Types::HBUINT::static_سائز);

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ) &&
                  coverage.sanitize (c, یہ) &&
                  /* The coverage  table may use a range to represent a set
                   * of glyphs, which means a small number of bytes can
                   * generate a large glyph set. Manually modify the
                   * sanitizer max ops to take this into account.
                   *
                   * Note: This check *must* be right after coverage sanitize. */
                  c->check_ops ((یہ + coverage).get_population () >> 1));
  }

  hb_codepoint_t get_نقاب () ثابت
  { واپس (1 << (8 * Types::سائز)) - 1; }

  منطقی intersects (ثابت hb_سیٹ_t *glyphs) ثابت
  { واپس (یہ+coverage).intersects (glyphs); }

  منطقی may_have_non_1to1 () ثابت
  { واپس جھوٹ; }

  خالی depend (hb_depend_context_t *c) ثابت
  {
    single_subst_depend (c, یہ+coverage, deltaGlyphID, get_نقاب ());
  }

  خالی احاطہ (hb_احاطہ_context_t *c) ثابت
  {
    single_subst_احاطہ (c, یہ+coverage, deltaGlyphID, get_نقاب ());
  }

  خالی احاطہ_lookups (hb_احاطہ_lookups_context_t *c) ثابت {}

  خالی collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    single_subst_collect_glyphs (c, یہ+coverage, deltaGlyphID, get_نقاب ());
  }

  ثابت Coverage &get_coverage () ثابت { واپس یہ+coverage; }

  منطقی would_apply (hb_would_apply_context_t *c) ثابت
  { واپس c->len == 1 && (یہ+coverage).get_coverage (c->glyphs[0]) != NOT_COVERED; }

  غیرعلامتی
  get_glyph_alternates (hb_codepoint_t  glyph_id,
                        غیرعلامتی        آغاز_انحراف,
                        غیرعلامتی       *alternate_گنتی  /* IN/OUT.  May be NULL. */,
                        hb_codepoint_t *alternate_glyphs /* OUT.     May be NULL. */) ثابت
  {
    غیرعلامتی صحیح اشاریہ_2 = (یہ+coverage).get_coverage (glyph_id);
    اگر (likely (اشاریہ_2 == NOT_COVERED))
    {
      اگر (alternate_گنتی)
        *alternate_گنتی = 0;
      واپس 0;
    }

    اگر (alternate_گنتی && *alternate_گنتی && alternate_glyphs)
    {
      hb_codepoint_t d = deltaGlyphID;
      hb_codepoint_t نقاب = get_نقاب ();

      glyph_id = (glyph_id + d) & نقاب;

      *alternate_glyphs = glyph_id;
      *alternate_گنتی = 1;
    }

    واپس 1;
  }

  خالی
  collect_glyph_alternates (hb_map_t  *alternate_گنتی /* IN/OUT */,
			    hb_map_t  *alternate_glyphs /* IN/OUT */) ثابت
  {
    hb_codepoint_t d = deltaGlyphID;
    hb_codepoint_t نقاب = get_نقاب ();

    + hb_iter (یہ+coverage)
    | hb_map ([d, نقاب] (hb_codepoint_t g) { واپس hb_pair (g, (g + d) & نقاب); })
    | hb_apply ([&] (ثابت hb_pair_t<hb_codepoint_t, hb_codepoint_t> &p) -> خالی
		{ hb_collect_glyph_alternates_جوڑو (p.پہلا, p.second,
						    alternate_گنتی, alternate_glyphs); })
    ;
  }

  منطقی apply (hb_ot_apply_context_t *c) ثابت
  {
    TRACE_APPLY (یہ);
    hb_codepoint_t glyph_id = c->بفر->cur().codepoint;
    غیرعلامتی صحیح اشاریہ_2 = (یہ+coverage).get_coverage (glyph_id);
    اگر (اشاریہ_2 == NOT_COVERED) return_trace (جھوٹ);

    hb_codepoint_t d = deltaGlyphID;
    hb_codepoint_t نقاب = get_نقاب ();

    glyph_id = (glyph_id + d) & نقاب;

    اگر (HB_بفر_پیغام_مزید && c->بفر->messaging ())
    {
      c->بفر->sync_so_far ();
      c->بفر->پیغام (c->font,
			  "replacing glyph at %u (single substitution)",
			  c->بفر->idx);
    }

    c->replace_glyph (glyph_id);

    اگر (HB_بفر_پیغام_مزید && c->بفر->messaging ())
    {
      c->بفر->پیغام (c->font,
			  "replaced glyph at %u (single substitution)",
			  c->بفر->idx - 1u);
    }

    return_trace (سچ);
  }

  سانچہ<قسم_نام Iterator,
           hb_requires (hb_is_sorted_ماخذ_کا (Iterator, hb_codepoint_t))>
  منطقی serialize (hb_serialize_context_t *c,
                  Iterator glyphs,
                  غیرعلامتی delta)
  {
    TRACE_SERIALIZE (یہ);
    اگر (unlikely (!c->extend_min (یہ))) return_trace (جھوٹ);
    اگر (unlikely (!coverage.serialize_serialize (c, glyphs))) return_trace (جھوٹ);
    c->check_تفویض (deltaGlyphID, delta, HB_SERIALIZE_ERROR_INT_حد_سے_تجاوز);
    return_trace (سچ);
  }

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (یہ);
    return_trace (subset_single_subst (c, یہ+coverage, deltaGlyphID, get_نقاب ()));
  }
};

}
}
}


#اگر_ختم /* OT_LAYOUT_GSUB_SINGLESUBSTFORMAT1_HH */
