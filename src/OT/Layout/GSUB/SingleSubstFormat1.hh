#ifndef OT_LAYOUT_GSUB_SINGLESUBSTFORMAT1_HH
#define OT_LAYOUT_GSUB_SINGLESUBSTFORMAT1_HH

#ଅନ୍ତର୍ଭାଵ "Common.hh"

ନାମାକାଶ OT {
ନାମାକାଶ Layout {
ନାମାକାଶ GSUB_impl {

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ତାର୍କିକ
subset_single_subst (hb_subset_context_t *c,
                     ଅଚଳ Coverage &coverage,
                     hb_codepoint_t d,
                     hb_codepoint_t mask)
{
  ଅଚଳ hb_set_t &glyphset = *c->plan->glyphset_gsub ();
  ଅଚଳ hb_map_t &glyph_map = *c->plan->glyph_map;

  hb_set_t intersection;
  coverage.intersect_set (glyphset, intersection);

  auto it =
  + hb_iter (intersection)
  | hb_map_retains_sorting ([d, mask] (hb_codepoint_t g) {
                              ଫେରନ୍ତୁ hb_codepoint_pair_t (g,
                                                          (g + d) & mask); })
  | hb_filter (glyphset, hb_second)
  | hb_map_retains_sorting ([&] (hb_codepoint_pair_t p) -> hb_codepoint_pair_t
                            { ଫେରନ୍ତୁ hb_pair (glyph_map[p.first], glyph_map[p.second]); })
  ;

  ତାର୍କିକ ret = ତାର୍କିକ (it);
  SingleSubst_serialize (c->serializer, it);
  ଫେରନ୍ତୁ ret;
}

HB_INTERNAL ଧାଡ଼ିଭିତରେ
ଶୂନ୍ୟ single_subst_depend (hb_depend_context_t *c,
                          ଅଚଳ Coverage &coverage,
                          hb_codepoint_t d, hb_codepoint_t mask)
{
  unsigned ଅଙ୍କ_glyphs = c->face->get_ଅଙ୍କ_glyphs ();

  // Filter coverage by active glyphs (like closure does)
  hb_set_t intersection;
  coverage.intersect_set (c->parent_active_glyphs (), intersection);

  + hb_iter (intersection)
  | hb_map ([&] (hb_codepoint_t _) { ଫେରନ୍ତୁ hb_codepoint_pair_t (_, (_ + d) & mask); })
  | hb_filter ([ଅଙ୍କ_glyphs] (ଅଚଳ hb_codepoint_pair_t &_) { ଫେରନ୍ତୁ _.second < ଅଙ୍କ_glyphs; })
  | hb_apply ([&] (ଅଚଳ hb_codepoint_pair_t &_) { c->depend_data->add_gsub_lookup(_.first, c->lookup_index, _.second); })
  ;
}

HB_INTERNAL ଧାଡ଼ିଭିତରେ
ଶୂନ୍ୟ single_subst_closure (hb_closure_context_t *c,
                           ଅଚଳ Coverage &coverage,
                           hb_codepoint_t d, hb_codepoint_t mask)
{
  /* Help fuzzer avoid this function as much. */
  unsigned କାଢ଼ = coverage.get_population ();
  ଯଦି (କାଢ଼ >= mask)
    ଫେରନ୍ତୁ;

  hb_set_t intersection;
  coverage.intersect_set (c->parent_active_glyphs (), intersection);

  /* In degenerate fuzzer-found fonts, but not real fonts,
   * this table can keep adding new glyphs in each round of closure.
   * Refuse to close-over, if it maps glyph range to overlapping range. */
  hb_codepoint_t min_before = intersection.get_min ();
  hb_codepoint_t max_before = intersection.get_max ();
  hb_codepoint_t min_after = (min_before + d) & mask;
  hb_codepoint_t max_after = (max_before + d) & mask;
  ଯଦି (intersection.get_population () == max_before - min_before + 1 &&
	((min_before <= min_after && min_after <= max_before) ||
	 (min_before <= max_after && max_after <= max_before)))
    ଫେରନ୍ତୁ;

  + hb_iter (intersection)
  | hb_map ([d, mask] (hb_codepoint_t g) { ଫେରନ୍ତୁ (g + d) & mask; })
  | hb_sink (c->output)
  ;
}

HB_INTERNAL ଧାଡ଼ିଭିତରେ
ଶୂନ୍ୟ single_subst_collect_glyphs (hb_collect_glyphs_context_t *c,
                                  ଅଚଳ Coverage &coverage,
                                  hb_codepoint_t d, hb_codepoint_t mask)
{
  ଯଦି (unlikely (!coverage.collect_coverage (c->ନିବେଶ))) ଫେରନ୍ତୁ;

  + hb_iter (coverage)
  | hb_map ([d, mask] (hb_codepoint_t g) { ଫେରନ୍ତୁ (g + d) & mask; })
  | hb_sink (c->output)
  ;
}

template <typename Types>
ସଂରଚନା SingleSubstFormat1_3
{
  ସୁରକ୍ଷିତ:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  typename Types::template LOffsetTo<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of Substitution table */
  typename Types::HBUINT
                deltaGlyphID;           /* Add to original GlyphID to get
                                         * substitute GlyphID, modulo 0x10000 */

  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (2 + Types::LOffset::static_size + Types::HBUINT::static_size);

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି) &&
                  coverage.sanitize (c, ଏହି) &&
                  /* The coverage  table may use a range to represent a set
                   * of glyphs, which means a small number of bytes can
                   * generate a large glyph set. Manually modify the
                   * sanitizer max ops to take this into account.
                   *
                   * Note: This check *must* be right after coverage sanitize. */
                  c->check_ops ((ଏହି + coverage).get_population () >> 1));
  }

  hb_codepoint_t get_mask () ଅଚଳ
  { ଫେରନ୍ତୁ (1 << (8 * Types::size)) - 1; }

  ତାର୍କିକ intersects (ଅଚଳ hb_set_t *glyphs) ଅଚଳ
  { ଫେରନ୍ତୁ (ଏହି+coverage).intersects (glyphs); }

  ତାର୍କିକ may_have_non_1to1 () ଅଚଳ
  { ଫେରନ୍ତୁ ମିଥ୍ୟା; }

  ଶୂନ୍ୟ depend (hb_depend_context_t *c) ଅଚଳ
  {
    single_subst_depend (c, ଏହି+coverage, deltaGlyphID, get_mask ());
  }

  ଶୂନ୍ୟ closure (hb_closure_context_t *c) ଅଚଳ
  {
    single_subst_closure (c, ଏହି+coverage, deltaGlyphID, get_mask ());
  }

  ଶୂନ୍ୟ closure_lookups (hb_closure_lookups_context_t *c) ଅଚଳ {}

  ଶୂନ୍ୟ collect_glyphs (hb_collect_glyphs_context_t *c) ଅଚଳ
  {
    single_subst_collect_glyphs (c, ଏହି+coverage, deltaGlyphID, get_mask ());
  }

  ଅଚଳ Coverage &get_coverage () ଅଚଳ { ଫେରନ୍ତୁ ଏହି+coverage; }

  ତାର୍କିକ would_apply (hb_would_apply_context_t *c) ଅଚଳ
  { ଫେରନ୍ତୁ c->len == 1 && (ଏହି+coverage).get_coverage (c->glyphs[0]) != NOT_COVERED; }

  unsigned
  get_glyph_alternates (hb_codepoint_t  glyph_id,
                        unsigned        start_offset,
                        unsigned       *alternate_ଗଣନା  /* IN/OUT.  May be NULL. */,
                        hb_codepoint_t *alternate_glyphs /* OUT.     May be NULL. */) ଅଚଳ
  {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା index = (ଏହି+coverage).get_coverage (glyph_id);
    ଯଦି (likely (index == NOT_COVERED))
    {
      ଯଦି (alternate_ଗଣନା)
        *alternate_ଗଣନା = 0;
      ଫେରନ୍ତୁ 0;
    }

    ଯଦି (alternate_ଗଣନା && *alternate_ଗଣନା && alternate_glyphs)
    {
      hb_codepoint_t d = deltaGlyphID;
      hb_codepoint_t mask = get_mask ();

      glyph_id = (glyph_id + d) & mask;

      *alternate_glyphs = glyph_id;
      *alternate_ଗଣନା = 1;
    }

    ଫେରନ୍ତୁ 1;
  }

  ଶୂନ୍ୟ
  collect_glyph_alternates (hb_map_t  *alternate_ଗଣନା /* IN/OUT */,
			    hb_map_t  *alternate_glyphs /* IN/OUT */) ଅଚଳ
  {
    hb_codepoint_t d = deltaGlyphID;
    hb_codepoint_t mask = get_mask ();

    + hb_iter (ଏହି+coverage)
    | hb_map ([d, mask] (hb_codepoint_t g) { ଫେରନ୍ତୁ hb_pair (g, (g + d) & mask); })
    | hb_apply ([&] (ଅଚଳ hb_pair_t<hb_codepoint_t, hb_codepoint_t> &p) -> ଶୂନ୍ୟ
		{ _hb_collect_glyph_alternates_add (p.first, p.second,
						    alternate_ଗଣନା, alternate_glyphs); })
    ;
  }

  ତାର୍କିକ apply (hb_ot_apply_context_t *c) ଅଚଳ
  {
    TRACE_APPLY (ଏହି);
    hb_codepoint_t glyph_id = c->buffer->cur().codepoint;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା index = (ଏହି+coverage).get_coverage (glyph_id);
    ଯଦି (index == NOT_COVERED) return_trace (ମିଥ୍ୟା);

    hb_codepoint_t d = deltaGlyphID;
    hb_codepoint_t mask = get_mask ();

    glyph_id = (glyph_id + d) & mask;

    ଯଦି (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->sync_so_far ();
      c->buffer->message (c->font,
			  "replacing glyph at %u (single substitution)",
			  c->buffer->idx);
    }

    c->replace_glyph (glyph_id);

    ଯଦି (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->message (c->font,
			  "replaced glyph at %u (single substitution)",
			  c->buffer->idx - 1u);
    }

    return_trace (ସତ୍ୟ);
  }

  template<typename Iterator,
           hb_requires (hb_is_sorted_source_of (Iterator, hb_codepoint_t))>
  ତାର୍କିକ serialize (hb_serialize_context_t *c,
                  Iterator glyphs,
                  unsigned delta)
  {
    TRACE_SERIALIZE (ଏହି);
    ଯଦି (unlikely (!c->extend_min (ଏହି))) return_trace (ମିଥ୍ୟା);
    ଯଦି (unlikely (!coverage.serialize_serialize (c, glyphs))) return_trace (ମିଥ୍ୟା);
    c->check_assign (deltaGlyphID, delta, HB_SERIALIZE_ERROR_INT_OVERFLOW);
    return_trace (ସତ୍ୟ);
  }

  ତାର୍କିକ subset (hb_subset_context_t *c) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    return_trace (subset_single_subst (c, ଏହି+coverage, deltaGlyphID, get_mask ()));
  }
};

}
}
}


#endif /* OT_LAYOUT_GSUB_SINGLESUBSTFORMAT1_HH */
