#ifndef OT_LAYOUT_GSUB_SINGLESUBSTFORMAT1_HH
#define OT_LAYOUT_GSUB_SINGLESUBSTFORMAT1_HH

#ಅನ್ತರ್ಭಾವ "Common.hh"

ನಾಮಾಕಾಶ OT {
ನಾಮಾಕಾಶ Layout {
ನಾಮಾಕಾಶ GSUB_impl {

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ತಾರ್ಕಿಕ
subset_single_subst (hb_subset_context_t *c,
                     ಧ್ರುವ Coverage &coverage,
                     hb_codepoint_t d,
                     hb_codepoint_t mask)
{
  ಧ್ರುವ hb_set_t &glyphset = *c->plan->glyphset_gsub ();
  ಧ್ರುವ hb_map_t &glyph_map = *c->plan->glyph_map;

  hb_set_t intersection;
  coverage.intersect_set (glyphset, intersection);

  auto it =
  + hb_iter (intersection)
  | hb_map_retains_sorting ([d, mask] (hb_codepoint_t g) {
                              ಪ್ರತ್ಯಾಗಮ hb_codepoint_pair_t (g,
                                                          (g + d) & mask); })
  | hb_filter (glyphset, hb_second)
  | hb_map_retains_sorting ([&] (hb_codepoint_pair_t p) -> hb_codepoint_pair_t
                            { ಪ್ರತ್ಯಾಗಮ hb_pair (glyph_map[p.first], glyph_map[p.second]); })
  ;

  ತಾರ್ಕಿಕ ret = ತಾರ್ಕಿಕ (it);
  SingleSubst_serialize (c->serializer, it);
  ಪ್ರತ್ಯಾಗಮ ret;
}

HB_INTERNAL ಅನ್ತಃಸ್ಥ
ಶೂನ್ಯ single_subst_depend (hb_depend_context_t *c,
                          ಧ್ರುವ Coverage &coverage,
                          hb_codepoint_t d, hb_codepoint_t mask)
{
  unsigned ಅಂಕೆ_glyphs = c->face->get_ಅಂಕೆ_glyphs ();

  // Filter coverage by active glyphs (like closure does)
  hb_set_t intersection;
  coverage.intersect_set (c->parent_active_glyphs (), intersection);

  + hb_iter (intersection)
  | hb_map ([&] (hb_codepoint_t _) { ಪ್ರತ್ಯಾಗಮ hb_codepoint_pair_t (_, (_ + d) & mask); })
  | hb_filter ([ಅಂಕೆ_glyphs] (ಧ್ರುವ hb_codepoint_pair_t &_) { ಪ್ರತ್ಯಾಗಮ _.second < ಅಂಕೆ_glyphs; })
  | hb_apply ([&] (ಧ್ರುವ hb_codepoint_pair_t &_) { c->depend_data->add_gsub_lookup(_.first, c->lookup_index, _.second); })
  ;
}

HB_INTERNAL ಅನ್ತಃಸ್ಥ
ಶೂನ್ಯ single_subst_closure (hb_closure_context_t *c,
                           ಧ್ರುವ Coverage &coverage,
                           hb_codepoint_t d, hb_codepoint_t mask)
{
  /* Help fuzzer avoid this function as much. */
  unsigned ತೆಗೆ = coverage.get_population ();
  ಯದಿ (ತೆಗೆ >= mask)
    ಪ್ರತ್ಯಾಗಮ;

  hb_set_t intersection;
  coverage.intersect_set (c->parent_active_glyphs (), intersection);

  /* In degenerate fuzzer-found fonts, but not real fonts,
   * this table can keep adding new glyphs in each round of closure.
   * Refuse to close-over, if it maps glyph range to overlapping range. */
  hb_codepoint_t min_before = intersection.get_min ();
  hb_codepoint_t max_before = intersection.get_max ();
  hb_codepoint_t min_after = (min_before + d) & mask;
  hb_codepoint_t max_after = (max_before + d) & mask;
  ಯದಿ (intersection.get_population () == max_before - min_before + 1 &&
	((min_before <= min_after && min_after <= max_before) ||
	 (min_before <= max_after && max_after <= max_before)))
    ಪ್ರತ್ಯಾಗಮ;

  + hb_iter (intersection)
  | hb_map ([d, mask] (hb_codepoint_t g) { ಪ್ರತ್ಯಾಗಮ (g + d) & mask; })
  | hb_sink (c->output)
  ;
}

HB_INTERNAL ಅನ್ತಃಸ್ಥ
ಶೂನ್ಯ single_subst_collect_glyphs (hb_collect_glyphs_context_t *c,
                                  ಧ್ರುವ Coverage &coverage,
                                  hb_codepoint_t d, hb_codepoint_t mask)
{
  ಯದಿ (unlikely (!coverage.collect_coverage (c->ಒಳಹರಿವು))) ಪ್ರತ್ಯಾಗಮ;

  + hb_iter (coverage)
  | hb_map ([d, mask] (hb_codepoint_t g) { ಪ್ರತ್ಯಾಗಮ (g + d) & mask; })
  | hb_sink (c->output)
  ;
}

template <typename Types>
ಸಂರಚನಾ SingleSubstFormat1_3
{
  ರಕ್ಷಿತ:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  typename Types::template LOffsetTo<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of Substitution table */
  typename Types::HBUINT
                deltaGlyphID;           /* Add to original GlyphID to get
                                         * substitute GlyphID, modulo 0x10000 */

  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (2 + Types::LOffset::static_size + Types::HBUINT::static_size);

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್) &&
                  coverage.sanitize (c, ಏತತ್) &&
                  /* The coverage  table may use a range to represent a set
                   * of glyphs, which means a small number of bytes can
                   * generate a large glyph set. Manually modify the
                   * sanitizer max ops to take this into account.
                   *
                   * Note: This check *must* be right after coverage sanitize. */
                  c->check_ops ((ಏತತ್ + coverage).get_population () >> 1));
  }

  hb_codepoint_t get_mask () ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ (1 << (8 * Types::size)) - 1; }

  ತಾರ್ಕಿಕ intersects (ಧ್ರುವ hb_set_t *glyphs) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ (ಏತತ್+coverage).intersects (glyphs); }

  ತಾರ್ಕಿಕ may_have_non_1to1 () ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ; }

  ಶೂನ್ಯ depend (hb_depend_context_t *c) ಧ್ರುವ
  {
    single_subst_depend (c, ಏತತ್+coverage, deltaGlyphID, get_mask ());
  }

  ಶೂನ್ಯ closure (hb_closure_context_t *c) ಧ್ರುವ
  {
    single_subst_closure (c, ಏತತ್+coverage, deltaGlyphID, get_mask ());
  }

  ಶೂನ್ಯ closure_lookups (hb_closure_lookups_context_t *c) ಧ್ರುವ {}

  ಶೂನ್ಯ collect_glyphs (hb_collect_glyphs_context_t *c) ಧ್ರುವ
  {
    single_subst_collect_glyphs (c, ಏತತ್+coverage, deltaGlyphID, get_mask ());
  }

  ಧ್ರುವ Coverage &get_coverage () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ ಏತತ್+coverage; }

  ತಾರ್ಕಿಕ would_apply (hb_would_apply_context_t *c) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ c->len == 1 && (ಏತತ್+coverage).get_coverage (c->glyphs[0]) != NOT_COVERED; }

  unsigned
  get_glyph_alternates (hb_codepoint_t  glyph_id,
                        unsigned        start_offset,
                        unsigned       *alternate_ಎಣಿಕೆ  /* IN/OUT.  May be NULL. */,
                        hb_codepoint_t *alternate_glyphs /* OUT.     May be NULL. */) ಧ್ರುವ
  {
    unsigned ಪೂರ್ಣಾಙ್ಕ index = (ಏತತ್+coverage).get_coverage (glyph_id);
    ಯದಿ (likely (index == NOT_COVERED))
    {
      ಯದಿ (alternate_ಎಣಿಕೆ)
        *alternate_ಎಣಿಕೆ = 0;
      ಪ್ರತ್ಯಾಗಮ 0;
    }

    ಯದಿ (alternate_ಎಣಿಕೆ && *alternate_ಎಣಿಕೆ && alternate_glyphs)
    {
      hb_codepoint_t d = deltaGlyphID;
      hb_codepoint_t mask = get_mask ();

      glyph_id = (glyph_id + d) & mask;

      *alternate_glyphs = glyph_id;
      *alternate_ಎಣಿಕೆ = 1;
    }

    ಪ್ರತ್ಯಾಗಮ 1;
  }

  ಶೂನ್ಯ
  collect_glyph_alternates (hb_map_t  *alternate_ಎಣಿಕೆ /* IN/OUT */,
			    hb_map_t  *alternate_glyphs /* IN/OUT */) ಧ್ರುವ
  {
    hb_codepoint_t d = deltaGlyphID;
    hb_codepoint_t mask = get_mask ();

    + hb_iter (ಏತತ್+coverage)
    | hb_map ([d, mask] (hb_codepoint_t g) { ಪ್ರತ್ಯಾಗಮ hb_pair (g, (g + d) & mask); })
    | hb_apply ([&] (ಧ್ರುವ hb_pair_t<hb_codepoint_t, hb_codepoint_t> &p) -> ಶೂನ್ಯ
		{ _hb_collect_glyph_alternates_add (p.first, p.second,
						    alternate_ಎಣಿಕೆ, alternate_glyphs); })
    ;
  }

  ತಾರ್ಕಿಕ apply (hb_ot_apply_context_t *c) ಧ್ರುವ
  {
    TRACE_APPLY (ಏತತ್);
    hb_codepoint_t glyph_id = c->buffer->cur().codepoint;
    unsigned ಪೂರ್ಣಾಙ್ಕ index = (ಏತತ್+coverage).get_coverage (glyph_id);
    ಯದಿ (index == NOT_COVERED) return_trace (ಅಸತ್ಯ);

    hb_codepoint_t d = deltaGlyphID;
    hb_codepoint_t mask = get_mask ();

    glyph_id = (glyph_id + d) & mask;

    ಯದಿ (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->sync_so_far ();
      c->buffer->message (c->font,
			  "replacing glyph at %u (single substitution)",
			  c->buffer->idx);
    }

    c->replace_glyph (glyph_id);

    ಯದಿ (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->message (c->font,
			  "replaced glyph at %u (single substitution)",
			  c->buffer->idx - 1u);
    }

    return_trace (ಸತ್ಯ);
  }

  template<typename Iterator,
           hb_requires (hb_is_sorted_source_of (Iterator, hb_codepoint_t))>
  ತಾರ್ಕಿಕ serialize (hb_serialize_context_t *c,
                  Iterator glyphs,
                  unsigned delta)
  {
    TRACE_SERIALIZE (ಏತತ್);
    ಯದಿ (unlikely (!c->extend_min (ಏತತ್))) return_trace (ಅಸತ್ಯ);
    ಯದಿ (unlikely (!coverage.serialize_serialize (c, glyphs))) return_trace (ಅಸತ್ಯ);
    c->check_assign (deltaGlyphID, delta, HB_SERIALIZE_ERROR_INT_OVERFLOW);
    return_trace (ಸತ್ಯ);
  }

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    return_trace (subset_single_subst (c, ಏತತ್+coverage, deltaGlyphID, get_mask ()));
  }
};

}
}
}


#endif /* OT_LAYOUT_GSUB_SINGLESUBSTFORMAT1_HH */
