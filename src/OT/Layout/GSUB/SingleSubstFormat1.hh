#ifndef OT_LAYOUT_GSUB_SINGLESUBSTFORMAT1_HH
#define OT_LAYOUT_GSUB_SINGLESUBSTFORMAT1_HH

#অন্তর্ভাব "Common.hh"

নামাকাশ OT {
নামাকাশ Layout {
নামাকাশ GSUB_impl {

স্থির অন্তঃরেখ যৌক্তিক
subset_single_subst (hb_subset_context_t *c,
                     ধ্রুবক Coverage &coverage,
                     hb_codepoint_t d,
                     hb_codepoint_t mask)
{
  ধ্রুবক hb_set_t &glyphset = *c->plan->glyphset_gsub ();
  ধ্রুবক hb_map_t &glyph_map = *c->plan->glyph_map;

  hb_set_t intersection;
  coverage.intersect_set (glyphset, intersection);

  auto it =
  + hb_iter (intersection)
  | hb_map_retains_sorting ([d, mask] (hb_codepoint_t g) {
                              ফেরত hb_codepoint_pair_t (g,
                                                          (g + d) & mask); })
  | hb_filter (glyphset, hb_second)
  | hb_map_retains_sorting ([&] (hb_codepoint_pair_t p) -> hb_codepoint_pair_t
                            { ফেরত hb_pair (glyph_map[p.first], glyph_map[p.second]); })
  ;

  যৌক্তিক ret = যৌক্তিক (it);
  SingleSubst_serialize (c->serializer, it);
  ফেরত ret;
}

HB_INTERNAL অন্তঃরেখ
শূন্য single_subst_depend (hb_depend_context_t *c,
                          ধ্রুবক Coverage &coverage,
                          hb_codepoint_t d, hb_codepoint_t mask)
{
  unsigned অঙ্ক_glyphs = c->face->get_অঙ্ক_glyphs ();

  // Filter coverage by active glyphs (like closure does)
  hb_set_t intersection;
  coverage.intersect_set (c->parent_active_glyphs (), intersection);

  + hb_iter (intersection)
  | hb_map ([&] (hb_codepoint_t _) { ফেরত hb_codepoint_pair_t (_, (_ + d) & mask); })
  | hb_filter ([অঙ্ক_glyphs] (ধ্রুবক hb_codepoint_pair_t &_) { ফেরত _.second < অঙ্ক_glyphs; })
  | hb_apply ([&] (ধ্রুবক hb_codepoint_pair_t &_) { c->depend_তথ্য->add_gsub_lookup(_.first, c->lookup_সূচক, _.second); })
  ;
}

HB_INTERNAL অন্তঃরেখ
শূন্য single_subst_closure (hb_closure_context_t *c,
                           ধ্রুবক Coverage &coverage,
                           hb_codepoint_t d, hb_codepoint_t mask)
{
  /* Help fuzzer avoid this function as much. */
  unsigned বের_করো = coverage.get_population ();
  যদি (বের_করো >= mask)
    ফেরত;

  hb_set_t intersection;
  coverage.intersect_set (c->parent_active_glyphs (), intersection);

  /* In degenerate fuzzer-found fonts, but not real fonts,
   * this table can keep adding new glyphs in each round of closure.
   * Refuse to close-over, if it maps glyph range to overlapping range. */
  hb_codepoint_t min_before = intersection.get_min ();
  hb_codepoint_t max_before = intersection.get_max ();
  hb_codepoint_t min_after = (min_before + d) & mask;
  hb_codepoint_t max_after = (max_before + d) & mask;
  যদি (intersection.get_population () == max_before - min_before + 1 &&
	((min_before <= min_after && min_after <= max_before) ||
	 (min_before <= max_after && max_after <= max_before)))
    ফেরত;

  + hb_iter (intersection)
  | hb_map ([d, mask] (hb_codepoint_t g) { ফেরত (g + d) & mask; })
  | hb_sink (c->output)
  ;
}

HB_INTERNAL অন্তঃরেখ
শূন্য single_subst_collect_glyphs (hb_collect_glyphs_context_t *c,
                                  ধ্রুবক Coverage &coverage,
                                  hb_codepoint_t d, hb_codepoint_t mask)
{
  যদি (unlikely (!coverage.collect_coverage (c->প্রবেশ))) ফেরত;

  + hb_iter (coverage)
  | hb_map ([d, mask] (hb_codepoint_t g) { ফেরত (g + d) & mask; })
  | hb_sink (c->output)
  ;
}

template <typename Types>
কাঠামো SingleSubstFormat1_3
{
  সুরক্ষিত:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  typename Types::template LOffsetTo<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of Substitution table */
  typename Types::HBUINT
                deltaGlyphID;           /* Add to original GlyphID to get
                                         * substitute GlyphID, modulo 0x10000 */

  সর্বজনীন:
  DEFINE_আকার_STATIC (2 + Types::LOffset::static_আকার + Types::HBUINT::static_আকার);

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই) &&
                  coverage.sanitize (c, এই) &&
                  /* The coverage  table may use a range to represent a set
                   * of glyphs, which means a small number of bytes can
                   * generate a large glyph set. Manually modify the
                   * sanitizer max ops to take this into account.
                   *
                   * Note: This check *must* be right after coverage sanitize. */
                  c->check_ops ((এই + coverage).get_population () >> 1));
  }

  hb_codepoint_t get_mask () ধ্রুবক
  { ফেরত (1 << (8 * Types::আকার_2)) - 1; }

  যৌক্তিক intersects (ধ্রুবক hb_set_t *glyphs) ধ্রুবক
  { ফেরত (এই+coverage).intersects (glyphs); }

  যৌক্তিক may_have_non_1to1 () ধ্রুবক
  { ফেরত মিথ্যা; }

  শূন্য depend (hb_depend_context_t *c) ধ্রুবক
  {
    single_subst_depend (c, এই+coverage, deltaGlyphID, get_mask ());
  }

  শূন্য closure (hb_closure_context_t *c) ধ্রুবক
  {
    single_subst_closure (c, এই+coverage, deltaGlyphID, get_mask ());
  }

  শূন্য closure_lookups (hb_closure_lookups_context_t *c) ধ্রুবক {}

  শূন্য collect_glyphs (hb_collect_glyphs_context_t *c) ধ্রুবক
  {
    single_subst_collect_glyphs (c, এই+coverage, deltaGlyphID, get_mask ());
  }

  ধ্রুবক Coverage &get_coverage () ধ্রুবক { ফেরত এই+coverage; }

  যৌক্তিক would_apply (hb_would_apply_context_t *c) ধ্রুবক
  { ফেরত c->len == 1 && (এই+coverage).get_coverage (c->glyphs[0]) != NOT_COVERED; }

  unsigned
  get_glyph_alternates (hb_codepoint_t  glyph_id,
                        unsigned        start_offset,
                        unsigned       *alternate_গুনতি  /* IN/OUT.  May be NULL. */,
                        hb_codepoint_t *alternate_glyphs /* OUT.     May be NULL. */) ধ্রুবক
  {
    unsigned পূর্ণসংখ্যা সূচক_2 = (এই+coverage).get_coverage (glyph_id);
    যদি (likely (সূচক_2 == NOT_COVERED))
    {
      যদি (alternate_গুনতি)
        *alternate_গুনতি = 0;
      ফেরত 0;
    }

    যদি (alternate_গুনতি && *alternate_গুনতি && alternate_glyphs)
    {
      hb_codepoint_t d = deltaGlyphID;
      hb_codepoint_t mask = get_mask ();

      glyph_id = (glyph_id + d) & mask;

      *alternate_glyphs = glyph_id;
      *alternate_গুনতি = 1;
    }

    ফেরত 1;
  }

  শূন্য
  collect_glyph_alternates (hb_map_t  *alternate_গুনতি /* IN/OUT */,
			    hb_map_t  *alternate_glyphs /* IN/OUT */) ধ্রুবক
  {
    hb_codepoint_t d = deltaGlyphID;
    hb_codepoint_t mask = get_mask ();

    + hb_iter (এই+coverage)
    | hb_map ([d, mask] (hb_codepoint_t g) { ফেরত hb_pair (g, (g + d) & mask); })
    | hb_apply ([&] (ধ্রুবক hb_pair_t<hb_codepoint_t, hb_codepoint_t> &p) -> শূন্য
		{ _hb_collect_glyph_alternates_add (p.first, p.second,
						    alternate_গুনতি, alternate_glyphs); })
    ;
  }

  যৌক্তিক apply (hb_ot_apply_context_t *c) ধ্রুবক
  {
    TRACE_APPLY (এই);
    hb_codepoint_t glyph_id = c->বাফার->cur().codepoint;
    unsigned পূর্ণসংখ্যা সূচক_2 = (এই+coverage).get_coverage (glyph_id);
    যদি (সূচক_2 == NOT_COVERED) return_trace (মিথ্যা);

    hb_codepoint_t d = deltaGlyphID;
    hb_codepoint_t mask = get_mask ();

    glyph_id = (glyph_id + d) & mask;

    যদি (HB_বাফার_MESSAGE_MORE && c->বাফার->messaging ())
    {
      c->বাফার->sync_so_far ();
      c->বাফার->message (c->font,
			  "replacing glyph at %u (single substitution)",
			  c->বাফার->idx);
    }

    c->replace_glyph (glyph_id);

    যদি (HB_বাফার_MESSAGE_MORE && c->বাফার->messaging ())
    {
      c->বাফার->message (c->font,
			  "replaced glyph at %u (single substitution)",
			  c->বাফার->idx - 1u);
    }

    return_trace (সত্য);
  }

  template<typename Iterator,
           hb_requires (hb_is_sorted_source_of (Iterator, hb_codepoint_t))>
  যৌক্তিক serialize (hb_serialize_context_t *c,
                  Iterator glyphs,
                  unsigned delta)
  {
    TRACE_SERIALIZE (এই);
    যদি (unlikely (!c->extend_min (এই))) return_trace (মিথ্যা);
    যদি (unlikely (!coverage.serialize_serialize (c, glyphs))) return_trace (মিথ্যা);
    c->check_assign (deltaGlyphID, delta, HB_SERIALIZE_ERROR_INT_OVERFLOW);
    return_trace (সত্য);
  }

  যৌক্তিক subset (hb_subset_context_t *c) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    return_trace (subset_single_subst (c, এই+coverage, deltaGlyphID, get_mask ()));
  }
};

}
}
}


#endif /* OT_LAYOUT_GSUB_SINGLESUBSTFORMAT1_HH */
