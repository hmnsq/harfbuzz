#ifndef OT_LAYOUT_GSUB_SINGLESUBSTFORMAT1_HH
#define OT_LAYOUT_GSUB_SINGLESUBSTFORMAT1_HH

#অন্তর্ভাব "Common.hh"

নামাকাশ OT {
নামাকাশ Layout {
নামাকাশ GSUB_impl {

স্থির অন্তঃস্থ তার্কিক
subset_single_subst (hb_subset_context_t *c,
                     ধ্রুব Coverage &coverage,
                     hb_codepoint_t d,
                     hb_codepoint_t mask)
{
  ধ্রুব hb_set_t &glyphset = *c->plan->glyphset_gsub ();
  ধ্রুব hb_map_t &glyph_map = *c->plan->glyph_map;

  hb_set_t intersection;
  coverage.intersect_set (glyphset, intersection);

  auto it =
  + hb_iter (intersection)
  | hb_map_retains_sorting ([d, mask] (hb_codepoint_t g) {
                              প্রত্যাগম hb_codepoint_pair_t (g,
                                                          (g + d) & mask); })
  | hb_filter (glyphset, hb_second)
  | hb_map_retains_sorting ([&] (hb_codepoint_pair_t p) -> hb_codepoint_pair_t
                            { প্রত্যাগম hb_pair (glyph_map[p.first], glyph_map[p.second]); })
  ;

  তার্কিক ret = তার্কিক (it);
  SingleSubst_serialize (c->serializer, it);
  প্রত্যাগম ret;
}

HB_INTERNAL অন্তঃস্থ
শূন্য single_subst_depend (hb_depend_context_t *c,
                          ধ্রুব Coverage &coverage,
                          hb_codepoint_t d, hb_codepoint_t mask)
{
  unsigned অঙ্ক_glyphs = c->face->get_অঙ্ক_glyphs ();

  // Filter coverage by active glyphs (like closure does)
  hb_set_t intersection;
  coverage.intersect_set (c->parent_active_glyphs (), intersection);

  + hb_iter (intersection)
  | hb_map ([&] (hb_codepoint_t _) { প্রত্যাগম hb_codepoint_pair_t (_, (_ + d) & mask); })
  | hb_filter ([অঙ্ক_glyphs] (ধ্রুব hb_codepoint_pair_t &_) { প্রত্যাগম _.second < অঙ্ক_glyphs; })
  | hb_apply ([&] (ধ্রুব hb_codepoint_pair_t &_) { c->depend_data->add_gsub_lookup(_.first, c->lookup_index, _.second); })
  ;
}

HB_INTERNAL অন্তঃস্থ
শূন্য single_subst_closure (hb_closure_context_t *c,
                           ধ্রুব Coverage &coverage,
                           hb_codepoint_t d, hb_codepoint_t mask)
{
  /* Help fuzzer avoid this function as much. */
  unsigned বের_করো = coverage.get_population ();
  যদি (বের_করো >= mask)
    প্রত্যাগম;

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
    প্রত্যাগম;

  + hb_iter (intersection)
  | hb_map ([d, mask] (hb_codepoint_t g) { প্রত্যাগম (g + d) & mask; })
  | hb_sink (c->output)
  ;
}

HB_INTERNAL অন্তঃস্থ
শূন্য single_subst_collect_glyphs (hb_collect_glyphs_context_t *c,
                                  ধ্রুব Coverage &coverage,
                                  hb_codepoint_t d, hb_codepoint_t mask)
{
  যদি (unlikely (!coverage.collect_coverage (c->প্রবেশ))) প্রত্যাগম;

  + hb_iter (coverage)
  | hb_map ([d, mask] (hb_codepoint_t g) { প্রত্যাগম (g + d) & mask; })
  | hb_sink (c->output)
  ;
}

template <typename Types>
সংরচনা SingleSubstFormat1_3
{
  রক্ষিত:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  typename Types::template LOffsetTo<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of Substitution table */
  typename Types::HBUINT
                deltaGlyphID;           /* Add to original GlyphID to get
                                         * substitute GlyphID, modulo 0x10000 */

  সার্বজনিক:
  DEFINE_SIZE_STATIC (2 + Types::LOffset::static_size + Types::HBUINT::static_size);

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্) &&
                  coverage.sanitize (c, এতত্) &&
                  /* The coverage  table may use a range to represent a set
                   * of glyphs, which means a small number of bytes can
                   * generate a large glyph set. Manually modify the
                   * sanitizer max ops to take this into account.
                   *
                   * Note: This check *must* be right after coverage sanitize. */
                  c->check_ops ((এতত্ + coverage).get_population () >> 1));
  }

  hb_codepoint_t get_mask () ধ্রুব
  { প্রত্যাগম (1 << (8 * Types::size)) - 1; }

  তার্কিক intersects (ধ্রুব hb_set_t *glyphs) ধ্রুব
  { প্রত্যাগম (এতত্+coverage).intersects (glyphs); }

  তার্কিক may_have_non_1to1 () ধ্রুব
  { প্রত্যাগম অসত্য; }

  শূন্য depend (hb_depend_context_t *c) ধ্রুব
  {
    single_subst_depend (c, এতত্+coverage, deltaGlyphID, get_mask ());
  }

  শূন্য closure (hb_closure_context_t *c) ধ্রুব
  {
    single_subst_closure (c, এতত্+coverage, deltaGlyphID, get_mask ());
  }

  শূন্য closure_lookups (hb_closure_lookups_context_t *c) ধ্রুব {}

  শূন্য collect_glyphs (hb_collect_glyphs_context_t *c) ধ্রুব
  {
    single_subst_collect_glyphs (c, এতত্+coverage, deltaGlyphID, get_mask ());
  }

  ধ্রুব Coverage &get_coverage () ধ্রুব { প্রত্যাগম এতত্+coverage; }

  তার্কিক would_apply (hb_would_apply_context_t *c) ধ্রুব
  { প্রত্যাগম c->len == 1 && (এতত্+coverage).get_coverage (c->glyphs[0]) != NOT_COVERED; }

  unsigned
  get_glyph_alternates (hb_codepoint_t  glyph_id,
                        unsigned        start_offset,
                        unsigned       *alternate_গুনতি  /* IN/OUT.  May be NULL. */,
                        hb_codepoint_t *alternate_glyphs /* OUT.     May be NULL. */) ধ্রুব
  {
    unsigned পূর্ণাঙ্ক index = (এতত্+coverage).get_coverage (glyph_id);
    যদি (likely (index == NOT_COVERED))
    {
      যদি (alternate_গুনতি)
        *alternate_গুনতি = 0;
      প্রত্যাগম 0;
    }

    যদি (alternate_গুনতি && *alternate_গুনতি && alternate_glyphs)
    {
      hb_codepoint_t d = deltaGlyphID;
      hb_codepoint_t mask = get_mask ();

      glyph_id = (glyph_id + d) & mask;

      *alternate_glyphs = glyph_id;
      *alternate_গুনতি = 1;
    }

    প্রত্যাগম 1;
  }

  শূন্য
  collect_glyph_alternates (hb_map_t  *alternate_গুনতি /* IN/OUT */,
			    hb_map_t  *alternate_glyphs /* IN/OUT */) ধ্রুব
  {
    hb_codepoint_t d = deltaGlyphID;
    hb_codepoint_t mask = get_mask ();

    + hb_iter (এতত্+coverage)
    | hb_map ([d, mask] (hb_codepoint_t g) { প্রত্যাগম hb_pair (g, (g + d) & mask); })
    | hb_apply ([&] (ধ্রুব hb_pair_t<hb_codepoint_t, hb_codepoint_t> &p) -> শূন্য
		{ _hb_collect_glyph_alternates_add (p.first, p.second,
						    alternate_গুনতি, alternate_glyphs); })
    ;
  }

  তার্কিক apply (hb_ot_apply_context_t *c) ধ্রুব
  {
    TRACE_APPLY (এতত্);
    hb_codepoint_t glyph_id = c->buffer->cur().codepoint;
    unsigned পূর্ণাঙ্ক index = (এতত্+coverage).get_coverage (glyph_id);
    যদি (index == NOT_COVERED) return_trace (অসত্য);

    hb_codepoint_t d = deltaGlyphID;
    hb_codepoint_t mask = get_mask ();

    glyph_id = (glyph_id + d) & mask;

    যদি (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->sync_so_far ();
      c->buffer->message (c->font,
			  "replacing glyph at %u (single substitution)",
			  c->buffer->idx);
    }

    c->replace_glyph (glyph_id);

    যদি (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->message (c->font,
			  "replaced glyph at %u (single substitution)",
			  c->buffer->idx - 1u);
    }

    return_trace (সত্য);
  }

  template<typename Iterator,
           hb_requires (hb_is_sorted_source_of (Iterator, hb_codepoint_t))>
  তার্কিক serialize (hb_serialize_context_t *c,
                  Iterator glyphs,
                  unsigned delta)
  {
    TRACE_SERIALIZE (এতত্);
    যদি (unlikely (!c->extend_min (এতত্))) return_trace (অসত্য);
    যদি (unlikely (!coverage.serialize_serialize (c, glyphs))) return_trace (অসত্য);
    c->check_assign (deltaGlyphID, delta, HB_SERIALIZE_ERROR_INT_OVERFLOW);
    return_trace (সত্য);
  }

  তার্কিক subset (hb_subset_context_t *c) ধ্রুব
  {
    TRACE_SUBSET (এতত্);
    return_trace (subset_single_subst (c, এতত্+coverage, deltaGlyphID, get_mask ()));
  }
};

}
}
}


#endif /* OT_LAYOUT_GSUB_SINGLESUBSTFORMAT1_HH */
