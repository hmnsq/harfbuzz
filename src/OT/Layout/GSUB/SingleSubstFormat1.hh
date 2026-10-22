#ifndef OT_LAYOUT_GSUB_SINGLESUBSTFORMAT1_HH
#define OT_LAYOUT_GSUB_SINGLESUBSTFORMAT1_HH

#அந்தர்பாவ "Common.hh"

நாமாகாஶ OT {
நாமாகாஶ Layout {
நாமாகாஶ GSUB_impl {

நிலையான உள்வரி தருக்க
subset_single_subst (hb_subset_context_t *c,
                     மாறிலி Coverage &coverage,
                     hb_codepoint_t d,
                     hb_codepoint_t mask)
{
  மாறிலி hb_set_t &glyphset = *c->plan->glyphset_gsub ();
  மாறிலி hb_map_t &glyph_map = *c->plan->glyph_map;

  hb_set_t intersection;
  coverage.intersect_set (glyphset, intersection);

  auto it =
  + hb_iter (intersection)
  | hb_map_retains_sorting ([d, mask] (hb_codepoint_t g) {
                              திரும்பு hb_codepoint_pair_t (g,
                                                          (g + d) & mask); })
  | hb_filter (glyphset, hb_second)
  | hb_map_retains_sorting ([&] (hb_codepoint_pair_t p) -> hb_codepoint_pair_t
                            { திரும்பு hb_pair (glyph_map[p.first], glyph_map[p.second]); })
  ;

  தருக்க ret = தருக்க (it);
  SingleSubst_serialize (c->serializer, it);
  திரும்பு ret;
}

HB_INTERNAL உள்வரி
வெறுமை single_subst_depend (hb_depend_context_t *c,
                          மாறிலி Coverage &coverage,
                          hb_codepoint_t d, hb_codepoint_t mask)
{
  unsigned இலக்கம்_glyphs = c->face->get_இலக்கம்_glyphs ();

  // Filter coverage by active glyphs (like closure does)
  hb_set_t intersection;
  coverage.intersect_set (c->parent_active_glyphs (), intersection);

  + hb_iter (intersection)
  | hb_map ([&] (hb_codepoint_t _) { திரும்பு hb_codepoint_pair_t (_, (_ + d) & mask); })
  | hb_filter ([இலக்கம்_glyphs] (மாறிலி hb_codepoint_pair_t &_) { திரும்பு _.second < இலக்கம்_glyphs; })
  | hb_apply ([&] (மாறிலி hb_codepoint_pair_t &_) { c->depend_data->add_gsub_lookup(_.first, c->lookup_index, _.second); })
  ;
}

HB_INTERNAL உள்வரி
வெறுமை single_subst_closure (hb_closure_context_t *c,
                           மாறிலி Coverage &coverage,
                           hb_codepoint_t d, hb_codepoint_t mask)
{
  /* Help fuzzer avoid this function as much. */
  unsigned எடு = coverage.get_population ();
  எனில் (எடு >= mask)
    திரும்பு;

  hb_set_t intersection;
  coverage.intersect_set (c->parent_active_glyphs (), intersection);

  /* In degenerate fuzzer-found fonts, but not real fonts,
   * this table can keep adding new glyphs in each round of closure.
   * Refuse to close-over, if it maps glyph range to overlapping range. */
  hb_codepoint_t min_before = intersection.get_min ();
  hb_codepoint_t max_before = intersection.get_max ();
  hb_codepoint_t min_after = (min_before + d) & mask;
  hb_codepoint_t max_after = (max_before + d) & mask;
  எனில் (intersection.get_population () == max_before - min_before + 1 &&
	((min_before <= min_after && min_after <= max_before) ||
	 (min_before <= max_after && max_after <= max_before)))
    திரும்பு;

  + hb_iter (intersection)
  | hb_map ([d, mask] (hb_codepoint_t g) { திரும்பு (g + d) & mask; })
  | hb_sink (c->output)
  ;
}

HB_INTERNAL உள்வரி
வெறுமை single_subst_collect_glyphs (hb_collect_glyphs_context_t *c,
                                  மாறிலி Coverage &coverage,
                                  hb_codepoint_t d, hb_codepoint_t mask)
{
  எனில் (unlikely (!coverage.collect_coverage (c->உள்ளீடு))) திரும்பு;

  + hb_iter (coverage)
  | hb_map ([d, mask] (hb_codepoint_t g) { திரும்பு (g + d) & mask; })
  | hb_sink (c->output)
  ;
}

template <typename Types>
கட்டமைப்பு SingleSubstFormat1_3
{
  பாதுகாக்கப்பட்ட:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  typename Types::template LOffsetTo<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of Substitution table */
  typename Types::HBUINT
                deltaGlyphID;           /* Add to original GlyphID to get
                                         * substitute GlyphID, modulo 0x10000 */

  பொது:
  DEFINE_SIZE_STATIC (2 + Types::LOffset::static_size + Types::HBUINT::static_size);

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது) &&
                  coverage.sanitize (c, இது) &&
                  /* The coverage  table may use a range to represent a set
                   * of glyphs, which means a small number of bytes can
                   * generate a large glyph set. Manually modify the
                   * sanitizer max ops to take this into account.
                   *
                   * Note: This check *must* be right after coverage sanitize. */
                  c->check_ops ((இது + coverage).get_population () >> 1));
  }

  hb_codepoint_t get_mask () மாறிலி
  { திரும்பு (1 << (8 * Types::size)) - 1; }

  தருக்க intersects (மாறிலி hb_set_t *glyphs) மாறிலி
  { திரும்பு (இது+coverage).intersects (glyphs); }

  தருக்க may_have_non_1to1 () மாறிலி
  { திரும்பு பொய்; }

  வெறுமை depend (hb_depend_context_t *c) மாறிலி
  {
    single_subst_depend (c, இது+coverage, deltaGlyphID, get_mask ());
  }

  வெறுமை closure (hb_closure_context_t *c) மாறிலி
  {
    single_subst_closure (c, இது+coverage, deltaGlyphID, get_mask ());
  }

  வெறுமை closure_lookups (hb_closure_lookups_context_t *c) மாறிலி {}

  வெறுமை collect_glyphs (hb_collect_glyphs_context_t *c) மாறிலி
  {
    single_subst_collect_glyphs (c, இது+coverage, deltaGlyphID, get_mask ());
  }

  மாறிலி Coverage &get_coverage () மாறிலி { திரும்பு இது+coverage; }

  தருக்க would_apply (hb_would_apply_context_t *c) மாறிலி
  { திரும்பு c->len == 1 && (இது+coverage).get_coverage (c->glyphs[0]) != NOT_COVERED; }

  unsigned
  get_glyph_alternates (hb_codepoint_t  glyph_id,
                        unsigned        start_offset,
                        unsigned       *alternate_மொத்தம்  /* IN/OUT.  May be NULL. */,
                        hb_codepoint_t *alternate_glyphs /* OUT.     May be NULL. */) மாறிலி
  {
    unsigned முழு index = (இது+coverage).get_coverage (glyph_id);
    எனில் (likely (index == NOT_COVERED))
    {
      எனில் (alternate_மொத்தம்)
        *alternate_மொத்தம் = 0;
      திரும்பு 0;
    }

    எனில் (alternate_மொத்தம் && *alternate_மொத்தம் && alternate_glyphs)
    {
      hb_codepoint_t d = deltaGlyphID;
      hb_codepoint_t mask = get_mask ();

      glyph_id = (glyph_id + d) & mask;

      *alternate_glyphs = glyph_id;
      *alternate_மொத்தம் = 1;
    }

    திரும்பு 1;
  }

  வெறுமை
  collect_glyph_alternates (hb_map_t  *alternate_மொத்தம் /* IN/OUT */,
			    hb_map_t  *alternate_glyphs /* IN/OUT */) மாறிலி
  {
    hb_codepoint_t d = deltaGlyphID;
    hb_codepoint_t mask = get_mask ();

    + hb_iter (இது+coverage)
    | hb_map ([d, mask] (hb_codepoint_t g) { திரும்பு hb_pair (g, (g + d) & mask); })
    | hb_apply ([&] (மாறிலி hb_pair_t<hb_codepoint_t, hb_codepoint_t> &p) -> வெறுமை
		{ _hb_collect_glyph_alternates_add (p.first, p.second,
						    alternate_மொத்தம், alternate_glyphs); })
    ;
  }

  தருக்க apply (hb_ot_apply_context_t *c) மாறிலி
  {
    TRACE_APPLY (இது);
    hb_codepoint_t glyph_id = c->buffer->cur().codepoint;
    unsigned முழு index = (இது+coverage).get_coverage (glyph_id);
    எனில் (index == NOT_COVERED) return_trace (பொய்);

    hb_codepoint_t d = deltaGlyphID;
    hb_codepoint_t mask = get_mask ();

    glyph_id = (glyph_id + d) & mask;

    எனில் (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->sync_so_far ();
      c->buffer->message (c->font,
			  "replacing glyph at %u (single substitution)",
			  c->buffer->idx);
    }

    c->replace_glyph (glyph_id);

    எனில் (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->message (c->font,
			  "replaced glyph at %u (single substitution)",
			  c->buffer->idx - 1u);
    }

    return_trace (உண்மை);
  }

  template<typename Iterator,
           hb_requires (hb_is_sorted_source_of (Iterator, hb_codepoint_t))>
  தருக்க serialize (hb_serialize_context_t *c,
                  Iterator glyphs,
                  unsigned delta)
  {
    TRACE_SERIALIZE (இது);
    எனில் (unlikely (!c->extend_min (இது))) return_trace (பொய்);
    எனில் (unlikely (!coverage.serialize_serialize (c, glyphs))) return_trace (பொய்);
    c->check_assign (deltaGlyphID, delta, HB_SERIALIZE_ERROR_INT_OVERFLOW);
    return_trace (உண்மை);
  }

  தருக்க subset (hb_subset_context_t *c) மாறிலி
  {
    TRACE_SUBSET (இது);
    return_trace (subset_single_subst (c, இது+coverage, deltaGlyphID, get_mask ()));
  }
};

}
}
}


#endif /* OT_LAYOUT_GSUB_SINGLESUBSTFORMAT1_HH */
