#ifndef OT_LAYOUT_GSUB_SINGLESUBSTFORMAT1_HH
#define OT_LAYOUT_GSUB_SINGLESUBSTFORMAT1_HH

#अन्तर्भाव "Common.hh"

नामाकाश OT {
नामाकाश Layout {
नामाकाश GSUB_impl {

स्थिर अन्तःस्थ तार्किक
subset_single_subst (hb_subset_context_t *c,
                     ध्रुव Coverage &coverage,
                     hb_codepoint_t d,
                     hb_codepoint_t mask)
{
  ध्रुव hb_set_t &glyphset = *c->plan->glyphset_gsub ();
  ध्रुव hb_map_t &glyph_map = *c->plan->glyph_map;

  hb_set_t intersection;
  coverage.intersect_set (glyphset, intersection);

  auto it =
  + hb_iter (intersection)
  | hb_map_retains_sorting ([d, mask] (hb_codepoint_t g) {
                              प्रत्यागम hb_codepoint_pair_t (g,
                                                          (g + d) & mask); })
  | hb_filter (glyphset, hb_second)
  | hb_map_retains_sorting ([&] (hb_codepoint_pair_t p) -> hb_codepoint_pair_t
                            { प्रत्यागम hb_pair (glyph_map[p.first], glyph_map[p.second]); })
  ;

  तार्किक ret = तार्किक (it);
  SingleSubst_serialize (c->serializer, it);
  प्रत्यागम ret;
}

HB_INTERNAL अन्तःस्थ
शून्य single_subst_depend (hb_depend_context_t *c,
                          ध्रुव Coverage &coverage,
                          hb_codepoint_t d, hb_codepoint_t mask)
{
  unsigned अङ्कः_glyphs = c->face->get_अङ्कः_glyphs ();

  // Filter coverage by active glyphs (like closure does)
  hb_set_t intersection;
  coverage.intersect_set (c->parent_active_glyphs (), intersection);

  + hb_iter (intersection)
  | hb_map ([&] (hb_codepoint_t _) { प्रत्यागम hb_codepoint_pair_t (_, (_ + d) & mask); })
  | hb_filter ([अङ्कः_glyphs] (ध्रुव hb_codepoint_pair_t &_) { प्रत्यागम _.second < अङ्कः_glyphs; })
  | hb_apply ([&] (ध्रुव hb_codepoint_pair_t &_) { c->depend_data->add_gsub_lookup(_.first, c->lookup_index, _.second); })
  ;
}

HB_INTERNAL अन्तःस्थ
शून्य single_subst_closure (hb_closure_context_t *c,
                           ध्रुव Coverage &coverage,
                           hb_codepoint_t d, hb_codepoint_t mask)
{
  /* Help fuzzer avoid this function as much. */
  unsigned निष्कासय = coverage.get_population ();
  जइ (निष्कासय >= mask)
    प्रत्यागम;

  hb_set_t intersection;
  coverage.intersect_set (c->parent_active_glyphs (), intersection);

  /* In degenerate fuzzer-found fonts, but not real fonts,
   * this table can keep adding new glyphs in each round of closure.
   * Refuse to close-over, if it maps glyph range to overlapping range. */
  hb_codepoint_t min_before = intersection.get_min ();
  hb_codepoint_t max_before = intersection.get_max ();
  hb_codepoint_t min_after = (min_before + d) & mask;
  hb_codepoint_t max_after = (max_before + d) & mask;
  जइ (intersection.get_population () == max_before - min_before + 1 &&
	((min_before <= min_after && min_after <= max_before) ||
	 (min_before <= max_after && max_after <= max_before)))
    प्रत्यागम;

  + hb_iter (intersection)
  | hb_map ([d, mask] (hb_codepoint_t g) { प्रत्यागम (g + d) & mask; })
  | hb_sink (c->output)
  ;
}

HB_INTERNAL अन्तःस्थ
शून्य single_subst_collect_glyphs (hb_collect_glyphs_context_t *c,
                                  ध्रुव Coverage &coverage,
                                  hb_codepoint_t d, hb_codepoint_t mask)
{
  जइ (unlikely (!coverage.collect_coverage (c->निवेशः))) प्रत्यागम;

  + hb_iter (coverage)
  | hb_map ([d, mask] (hb_codepoint_t g) { प्रत्यागम (g + d) & mask; })
  | hb_sink (c->output)
  ;
}

template <typename Types>
संरचना SingleSubstFormat1_3
{
  रक्षित:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  typename Types::template LOffsetTo<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of Substitution table */
  typename Types::HBUINT
                deltaGlyphID;           /* Add to original GlyphID to get
                                         * substitute GlyphID, modulo 0x10000 */

  सार्वजनिक:
  DEFINE_SIZE_STATIC (2 + Types::LOffset::static_size + Types::HBUINT::static_size);

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) &&
                  coverage.sanitize (c, एतत्) &&
                  /* The coverage  table may use a range to represent a set
                   * of glyphs, which means a small number of bytes can
                   * generate a large glyph set. Manually modify the
                   * sanitizer max ops to take this into account.
                   *
                   * Note: This check *must* be right after coverage sanitize. */
                  c->check_ops ((एतत् + coverage).get_population () >> 1));
  }

  hb_codepoint_t get_mask () ध्रुव
  { प्रत्यागम (1 << (8 * Types::size)) - 1; }

  तार्किक intersects (ध्रुव hb_set_t *glyphs) ध्रुव
  { प्रत्यागम (एतत्+coverage).intersects (glyphs); }

  तार्किक may_have_non_1to1 () ध्रुव
  { प्रत्यागम मिच्छ; }

  शून्य depend (hb_depend_context_t *c) ध्रुव
  {
    single_subst_depend (c, एतत्+coverage, deltaGlyphID, get_mask ());
  }

  शून्य closure (hb_closure_context_t *c) ध्रुव
  {
    single_subst_closure (c, एतत्+coverage, deltaGlyphID, get_mask ());
  }

  शून्य closure_lookups (hb_closure_lookups_context_t *c) ध्रुव {}

  शून्य collect_glyphs (hb_collect_glyphs_context_t *c) ध्रुव
  {
    single_subst_collect_glyphs (c, एतत्+coverage, deltaGlyphID, get_mask ());
  }

  ध्रुव Coverage &get_coverage () ध्रुव { प्रत्यागम एतत्+coverage; }

  तार्किक would_apply (hb_would_apply_context_t *c) ध्रुव
  { प्रत्यागम c->len == 1 && (एतत्+coverage).get_coverage (c->glyphs[0]) != NOT_COVERED; }

  unsigned
  get_glyph_alternates (hb_codepoint_t  glyph_id,
                        unsigned        start_offset,
                        unsigned       *alternate_गणनम्  /* IN/OUT.  May be NULL. */,
                        hb_codepoint_t *alternate_glyphs /* OUT.     May be NULL. */) ध्रुव
  {
    unsigned पूर्णाङ्क index = (एतत्+coverage).get_coverage (glyph_id);
    जइ (likely (index == NOT_COVERED))
    {
      जइ (alternate_गणनम्)
        *alternate_गणनम् = 0;
      प्रत्यागम 0;
    }

    जइ (alternate_गणनम् && *alternate_गणनम् && alternate_glyphs)
    {
      hb_codepoint_t d = deltaGlyphID;
      hb_codepoint_t mask = get_mask ();

      glyph_id = (glyph_id + d) & mask;

      *alternate_glyphs = glyph_id;
      *alternate_गणनम् = 1;
    }

    प्रत्यागम 1;
  }

  शून्य
  collect_glyph_alternates (hb_map_t  *alternate_गणनम् /* IN/OUT */,
			    hb_map_t  *alternate_glyphs /* IN/OUT */) ध्रुव
  {
    hb_codepoint_t d = deltaGlyphID;
    hb_codepoint_t mask = get_mask ();

    + hb_iter (एतत्+coverage)
    | hb_map ([d, mask] (hb_codepoint_t g) { प्रत्यागम hb_pair (g, (g + d) & mask); })
    | hb_apply ([&] (ध्रुव hb_pair_t<hb_codepoint_t, hb_codepoint_t> &p) -> शून्य
		{ _hb_collect_glyph_alternates_add (p.first, p.second,
						    alternate_गणनम्, alternate_glyphs); })
    ;
  }

  तार्किक apply (hb_ot_apply_context_t *c) ध्रुव
  {
    TRACE_APPLY (एतत्);
    hb_codepoint_t glyph_id = c->buffer->cur().codepoint;
    unsigned पूर्णाङ्क index = (एतत्+coverage).get_coverage (glyph_id);
    जइ (index == NOT_COVERED) return_trace (मिच्छ);

    hb_codepoint_t d = deltaGlyphID;
    hb_codepoint_t mask = get_mask ();

    glyph_id = (glyph_id + d) & mask;

    जइ (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->sync_so_far ();
      c->buffer->message (c->font,
			  "replacing glyph at %u (single substitution)",
			  c->buffer->idx);
    }

    c->replace_glyph (glyph_id);

    जइ (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->message (c->font,
			  "replaced glyph at %u (single substitution)",
			  c->buffer->idx - 1u);
    }

    return_trace (सच्च);
  }

  template<typename Iterator,
           hb_requires (hb_is_sorted_source_of (Iterator, hb_codepoint_t))>
  तार्किक serialize (hb_serialize_context_t *c,
                  Iterator glyphs,
                  unsigned delta)
  {
    TRACE_SERIALIZE (एतत्);
    जइ (unlikely (!c->extend_min (एतत्))) return_trace (मिच्छ);
    जइ (unlikely (!coverage.serialize_serialize (c, glyphs))) return_trace (मिच्छ);
    c->check_assign (deltaGlyphID, delta, HB_SERIALIZE_ERROR_INT_OVERFLOW);
    return_trace (सच्च);
  }

  तार्किक subset (hb_subset_context_t *c) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    return_trace (subset_single_subst (c, एतत्+coverage, deltaGlyphID, get_mask ()));
  }
};

}
}
}


#endif /* OT_LAYOUT_GSUB_SINGLESUBSTFORMAT1_HH */
