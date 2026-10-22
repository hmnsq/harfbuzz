#ifndef OT_LAYOUT_GSUB_SINGLESUBSTFORMAT1_HH
#define OT_LAYOUT_GSUB_SINGLESUBSTFORMAT1_HH

#شامل "Common.hh"

فضاینام OT {
فضاینام Layout {
فضاینام GSUB_impl {

ایستا درونخطی منطقی
subset_single_subst (hb_subset_context_t *c,
                     ثابت Coverage &coverage,
                     hb_codepoint_t d,
                     hb_codepoint_t mask)
{
  ثابت hb_set_t &glyphset = *c->plan->glyphset_gsub ();
  ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

  hb_set_t intersection;
  coverage.intersect_set (glyphset, intersection);

  auto it =
  + hb_iter (intersection)
  | hb_map_retains_sorting ([d, mask] (hb_codepoint_t g) {
                              بازگشت hb_codepoint_pair_t (g,
                                                          (g + d) & mask); })
  | hb_filter (glyphset, hb_second)
  | hb_map_retains_sorting ([&] (hb_codepoint_pair_t p) -> hb_codepoint_pair_t
                            { بازگشت hb_pair (glyph_map[p.first], glyph_map[p.second]); })
  ;

  منطقی ret = منطقی (it);
  SingleSubst_serialize (c->serializer, it);
  بازگشت ret;
}

HB_INTERNAL درونخطی
تهی single_subst_depend (hb_depend_context_t *c,
                          ثابت Coverage &coverage,
                          hb_codepoint_t d, hb_codepoint_t mask)
{
  unsigned رقم_glyphs = c->face->get_رقم_glyphs ();

  // Filter coverage by active glyphs (like closure does)
  hb_set_t intersection;
  coverage.intersect_set (c->parent_active_glyphs (), intersection);

  + hb_iter (intersection)
  | hb_map ([&] (hb_codepoint_t _) { بازگشت hb_codepoint_pair_t (_, (_ + d) & mask); })
  | hb_filter ([رقم_glyphs] (ثابت hb_codepoint_pair_t &_) { بازگشت _.second < رقم_glyphs; })
  | hb_apply ([&] (ثابت hb_codepoint_pair_t &_) { c->depend_data->add_gsub_lookup(_.first, c->lookup_index, _.second); })
  ;
}

HB_INTERNAL درونخطی
تهی single_subst_closure (hb_closure_context_t *c,
                           ثابت Coverage &coverage,
                           hb_codepoint_t d, hb_codepoint_t mask)
{
  /* Help fuzzer avoid this function as much. */
  unsigned برداشت = coverage.get_population ();
  اگر (برداشت >= mask)
    بازگشت;

  hb_set_t intersection;
  coverage.intersect_set (c->parent_active_glyphs (), intersection);

  /* In degenerate fuzzer-found fonts, but not real fonts,
   * this table can keep adding new glyphs in each round of closure.
   * Refuse to close-over, if it maps glyph range to overlapping range. */
  hb_codepoint_t min_before = intersection.get_min ();
  hb_codepoint_t max_before = intersection.get_max ();
  hb_codepoint_t min_after = (min_before + d) & mask;
  hb_codepoint_t max_after = (max_before + d) & mask;
  اگر (intersection.get_population () == max_before - min_before + 1 &&
	((min_before <= min_after && min_after <= max_before) ||
	 (min_before <= max_after && max_after <= max_before)))
    بازگشت;

  + hb_iter (intersection)
  | hb_map ([d, mask] (hb_codepoint_t g) { بازگشت (g + d) & mask; })
  | hb_sink (c->output)
  ;
}

HB_INTERNAL درونخطی
تهی single_subst_collect_glyphs (hb_collect_glyphs_context_t *c,
                                  ثابت Coverage &coverage,
                                  hb_codepoint_t d, hb_codepoint_t mask)
{
  اگر (unlikely (!coverage.collect_coverage (c->درون_داد))) بازگشت;

  + hb_iter (coverage)
  | hb_map ([d, mask] (hb_codepoint_t g) { بازگشت (g + d) & mask; })
  | hb_sink (c->output)
  ;
}

template <typename Types>
ساختار SingleSubstFormat1_3
{
  محافظتشده:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  typename Types::template LOffsetTo<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of Substitution table */
  typename Types::HBUINT
                deltaGlyphID;           /* Add to original GlyphID to get
                                         * substitute GlyphID, modulo 0x10000 */

  عمومی:
  DEFINE_SIZE_STATIC (2 + Types::LOffset::static_size + Types::HBUINT::static_size);

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این) &&
                  coverage.sanitize (c, این) &&
                  /* The coverage  table may use a range to represent a set
                   * of glyphs, which means a small number of bytes can
                   * generate a large glyph set. Manually modify the
                   * sanitizer max ops to take this into account.
                   *
                   * Note: This check *must* be right after coverage sanitize. */
                  c->check_ops ((این + coverage).get_population () >> 1));
  }

  hb_codepoint_t get_mask () ثابت
  { بازگشت (1 << (8 * Types::size)) - 1; }

  منطقی intersects (ثابت hb_set_t *glyphs) ثابت
  { بازگشت (این+coverage).intersects (glyphs); }

  منطقی may_have_non_1to1 () ثابت
  { بازگشت نادرست; }

  تهی depend (hb_depend_context_t *c) ثابت
  {
    single_subst_depend (c, این+coverage, deltaGlyphID, get_mask ());
  }

  تهی closure (hb_closure_context_t *c) ثابت
  {
    single_subst_closure (c, این+coverage, deltaGlyphID, get_mask ());
  }

  تهی closure_lookups (hb_closure_lookups_context_t *c) ثابت {}

  تهی collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    single_subst_collect_glyphs (c, این+coverage, deltaGlyphID, get_mask ());
  }

  ثابت Coverage &get_coverage () ثابت { بازگشت این+coverage; }

  منطقی would_apply (hb_would_apply_context_t *c) ثابت
  { بازگشت c->len == 1 && (این+coverage).get_coverage (c->glyphs[0]) != NOT_COVERED; }

  unsigned
  get_glyph_alternates (hb_codepoint_t  glyph_id,
                        unsigned        start_offset,
                        unsigned       *alternate_شمار  /* IN/OUT.  May be NULL. */,
                        hb_codepoint_t *alternate_glyphs /* OUT.     May be NULL. */) ثابت
  {
    unsigned صحیح index = (این+coverage).get_coverage (glyph_id);
    اگر (likely (index == NOT_COVERED))
    {
      اگر (alternate_شمار)
        *alternate_شمار = 0;
      بازگشت 0;
    }

    اگر (alternate_شمار && *alternate_شمار && alternate_glyphs)
    {
      hb_codepoint_t d = deltaGlyphID;
      hb_codepoint_t mask = get_mask ();

      glyph_id = (glyph_id + d) & mask;

      *alternate_glyphs = glyph_id;
      *alternate_شمار = 1;
    }

    بازگشت 1;
  }

  تهی
  collect_glyph_alternates (hb_map_t  *alternate_شمار /* IN/OUT */,
			    hb_map_t  *alternate_glyphs /* IN/OUT */) ثابت
  {
    hb_codepoint_t d = deltaGlyphID;
    hb_codepoint_t mask = get_mask ();

    + hb_iter (این+coverage)
    | hb_map ([d, mask] (hb_codepoint_t g) { بازگشت hb_pair (g, (g + d) & mask); })
    | hb_apply ([&] (ثابت hb_pair_t<hb_codepoint_t, hb_codepoint_t> &p) -> تهی
		{ _hb_collect_glyph_alternates_add (p.first, p.second,
						    alternate_شمار, alternate_glyphs); })
    ;
  }

  منطقی apply (hb_ot_apply_context_t *c) ثابت
  {
    TRACE_APPLY (این);
    hb_codepoint_t glyph_id = c->buffer->cur().codepoint;
    unsigned صحیح index = (این+coverage).get_coverage (glyph_id);
    اگر (index == NOT_COVERED) return_trace (نادرست);

    hb_codepoint_t d = deltaGlyphID;
    hb_codepoint_t mask = get_mask ();

    glyph_id = (glyph_id + d) & mask;

    اگر (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->sync_so_far ();
      c->buffer->message (c->font,
			  "replacing glyph at %u (single substitution)",
			  c->buffer->idx);
    }

    c->replace_glyph (glyph_id);

    اگر (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->message (c->font,
			  "replaced glyph at %u (single substitution)",
			  c->buffer->idx - 1u);
    }

    return_trace (درست);
  }

  template<typename Iterator,
           hb_requires (hb_is_sorted_source_of (Iterator, hb_codepoint_t))>
  منطقی serialize (hb_serialize_context_t *c,
                  Iterator glyphs,
                  unsigned delta)
  {
    TRACE_SERIALIZE (این);
    اگر (unlikely (!c->extend_min (این))) return_trace (نادرست);
    اگر (unlikely (!coverage.serialize_serialize (c, glyphs))) return_trace (نادرست);
    c->check_assign (deltaGlyphID, delta, HB_SERIALIZE_ERROR_INT_OVERFLOW);
    return_trace (درست);
  }

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (این);
    return_trace (subset_single_subst (c, این+coverage, deltaGlyphID, get_mask ()));
  }
};

}
}
}


#endif /* OT_LAYOUT_GSUB_SINGLESUBSTFORMAT1_HH */
