#ifndef OT_LAYOUT_GSUB_SINGLESUBSTFORMAT1_HH
#define OT_LAYOUT_GSUB_SINGLESUBSTFORMAT1_HH

#अन्तर्भाव "Common.hh"

नामाकाश OT {
नामाकाश Layout {
नामाकाश GSUB_impl {

स्थिर ओळीत तार्किक
subset_single_subst (hb_subset_context_t *c,
                     स्थिरांक Coverage &coverage,
                     hb_codepoint_t d,
                     hb_codepoint_t mask)
{
  स्थिरांक hb_set_t &glyphset = *c->plan->glyphset_gsub ();
  स्थिरांक hb_map_t &glyph_map = *c->plan->glyph_map;

  hb_set_t intersection;
  coverage.intersect_set (glyphset, intersection);

  स्वतः it =
  + hb_iter (intersection)
  | hb_map_retains_sorting ([d, mask] (hb_codepoint_t g) {
                              परत hb_codepoint_pair_t (g,
                                                          (g + d) & mask); })
  | hb_filter (glyphset, hb_second)
  | hb_map_retains_sorting ([&] (hb_codepoint_pair_t p) -> hb_codepoint_pair_t
                            { परत hb_pair (glyph_map[p.first], glyph_map[p.second]); })
  ;

  तार्किक ret = तार्किक (it);
  SingleSubst_serialize (c->serializer, it);
  परत ret;
}

HB_INTERNAL ओळीत
शून्य single_subst_depend (hb_depend_context_t *c,
                          स्थिरांक Coverage &coverage,
                          hb_codepoint_t d, hb_codepoint_t mask)
{
  अचिन्हित अंक_glyphs = c->face->get_अंक_glyphs ();

  // Filter coverage by active glyphs (like closure does)
  hb_set_t intersection;
  coverage.intersect_set (c->parent_active_glyphs (), intersection);

  + hb_iter (intersection)
  | hb_map ([&] (hb_codepoint_t _) { परत hb_codepoint_pair_t (_, (_ + d) & mask); })
  | hb_filter ([अंक_glyphs] (स्थिरांक hb_codepoint_pair_t &_) { परत _.second < अंक_glyphs; })
  | hb_apply ([&] (स्थिरांक hb_codepoint_pair_t &_) { c->depend_data->add_gsub_lookup(_.first, c->lookup_index, _.second); })
  ;
}

HB_INTERNAL ओळीत
शून्य single_subst_closure (hb_closure_context_t *c,
                           स्थिरांक Coverage &coverage,
                           hb_codepoint_t d, hb_codepoint_t mask)
{
  /* Help fuzzer avoid this function as much. */
  अचिन्हित काढा = coverage.get_population ();
  जर (काढा >= mask)
    परत;

  hb_set_t intersection;
  coverage.intersect_set (c->parent_active_glyphs (), intersection);

  /* In degenerate fuzzer-found fonts, but not real fonts,
   * this table can keep adding new glyphs in each round of closure.
   * Refuse to close-over, if it maps glyph range to overlapping range. */
  hb_codepoint_t min_before = intersection.get_min ();
  hb_codepoint_t max_before = intersection.get_max ();
  hb_codepoint_t min_after = (min_before + d) & mask;
  hb_codepoint_t max_after = (max_before + d) & mask;
  जर (intersection.get_population () == max_before - min_before + 1 &&
	((min_before <= min_after && min_after <= max_before) ||
	 (min_before <= max_after && max_after <= max_before)))
    परत;

  + hb_iter (intersection)
  | hb_map ([d, mask] (hb_codepoint_t g) { परत (g + d) & mask; })
  | hb_sink (c->output)
  ;
}

HB_INTERNAL ओळीत
शून्य single_subst_collect_glyphs (hb_collect_glyphs_context_t *c,
                                  स्थिरांक Coverage &coverage,
                                  hb_codepoint_t d, hb_codepoint_t mask)
{
  जर (unlikely (!coverage.collect_coverage (c->आदान))) परत;

  + hb_iter (coverage)
  | hb_map ([d, mask] (hb_codepoint_t g) { परत (g + d) & mask; })
  | hb_sink (c->output)
  ;
}

ढाँचा <typename Types>
संरचना SingleSubstFormat1_3
{
  संरक्षित:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  typename Types::ढाँचा LOffsetTo<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of Substitution table */
  typename Types::HBUINT
                deltaGlyphID;           /* Add to original GlyphID to get
                                         * substitute GlyphID, modulo 0x10000 */

  सार्वजनिक:
  DEFINE_SIZE_STATIC (2 + Types::LOffset::static_size + Types::HBUINT::static_size);

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे) &&
                  coverage.sanitize (c, हे) &&
                  /* The coverage  table may use a range to represent a set
                   * of glyphs, which means a small number of bytes can
                   * generate a large glyph set. Manually modify the
                   * sanitizer max ops to take this into account.
                   *
                   * Note: This check *must* be right after coverage sanitize. */
                  c->check_ops ((हे + coverage).get_population () >> 1));
  }

  hb_codepoint_t get_mask () स्थिरांक
  { परत (1 << (8 * Types::size)) - 1; }

  तार्किक intersects (स्थिरांक hb_set_t *glyphs) स्थिरांक
  { परत (हे+coverage).intersects (glyphs); }

  तार्किक may_have_non_1to1 () स्थिरांक
  { परत असत्य; }

  शून्य depend (hb_depend_context_t *c) स्थिरांक
  {
    single_subst_depend (c, हे+coverage, deltaGlyphID, get_mask ());
  }

  शून्य closure (hb_closure_context_t *c) स्थिरांक
  {
    single_subst_closure (c, हे+coverage, deltaGlyphID, get_mask ());
  }

  शून्य closure_lookups (hb_closure_lookups_context_t *c) स्थिरांक {}

  शून्य collect_glyphs (hb_collect_glyphs_context_t *c) स्थिरांक
  {
    single_subst_collect_glyphs (c, हे+coverage, deltaGlyphID, get_mask ());
  }

  स्थिरांक Coverage &get_coverage () स्थिरांक { परत हे+coverage; }

  तार्किक would_apply (hb_would_apply_context_t *c) स्थिरांक
  { परत c->len == 1 && (हे+coverage).get_coverage (c->glyphs[0]) != NOT_COVERED; }

  अचिन्हित
  get_glyph_alternates (hb_codepoint_t  glyph_id,
                        अचिन्हित        start_offset,
                        अचिन्हित       *alternate_मोजणी  /* IN/OUT.  May be NULL. */,
                        hb_codepoint_t *alternate_glyphs /* OUT.     May be NULL. */) स्थिरांक
  {
    अचिन्हित पूर्णांक index = (हे+coverage).get_coverage (glyph_id);
    जर (likely (index == NOT_COVERED))
    {
      जर (alternate_मोजणी)
        *alternate_मोजणी = 0;
      परत 0;
    }

    जर (alternate_मोजणी && *alternate_मोजणी && alternate_glyphs)
    {
      hb_codepoint_t d = deltaGlyphID;
      hb_codepoint_t mask = get_mask ();

      glyph_id = (glyph_id + d) & mask;

      *alternate_glyphs = glyph_id;
      *alternate_मोजणी = 1;
    }

    परत 1;
  }

  शून्य
  collect_glyph_alternates (hb_map_t  *alternate_मोजणी /* IN/OUT */,
			    hb_map_t  *alternate_glyphs /* IN/OUT */) स्थिरांक
  {
    hb_codepoint_t d = deltaGlyphID;
    hb_codepoint_t mask = get_mask ();

    + hb_iter (हे+coverage)
    | hb_map ([d, mask] (hb_codepoint_t g) { परत hb_pair (g, (g + d) & mask); })
    | hb_apply ([&] (स्थिरांक hb_pair_t<hb_codepoint_t, hb_codepoint_t> &p) -> शून्य
		{ _hb_collect_glyph_alternates_add (p.first, p.second,
						    alternate_मोजणी, alternate_glyphs); })
    ;
  }

  तार्किक apply (hb_ot_apply_context_t *c) स्थिरांक
  {
    TRACE_APPLY (हे);
    hb_codepoint_t glyph_id = c->buffer->cur().codepoint;
    अचिन्हित पूर्णांक index = (हे+coverage).get_coverage (glyph_id);
    जर (index == NOT_COVERED) return_trace (असत्य);

    hb_codepoint_t d = deltaGlyphID;
    hb_codepoint_t mask = get_mask ();

    glyph_id = (glyph_id + d) & mask;

    जर (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->sync_so_far ();
      c->buffer->message (c->font,
			  "replacing glyph at %u (single substitution)",
			  c->buffer->idx);
    }

    c->replace_glyph (glyph_id);

    जर (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->message (c->font,
			  "replaced glyph at %u (single substitution)",
			  c->buffer->idx - 1u);
    }

    return_trace (सत्य);
  }

  ढाँचा<typename Iterator,
           hb_requires (hb_is_sorted_source_of (Iterator, hb_codepoint_t))>
  तार्किक serialize (hb_serialize_context_t *c,
                  Iterator glyphs,
                  अचिन्हित delta)
  {
    TRACE_SERIALIZE (हे);
    जर (unlikely (!c->extend_min (हे))) return_trace (असत्य);
    जर (unlikely (!coverage.serialize_serialize (c, glyphs))) return_trace (असत्य);
    c->check_assign (deltaGlyphID, delta, HB_SERIALIZE_ERROR_INT_OVERFLOW);
    return_trace (सत्य);
  }

  तार्किक subset (hb_subset_context_t *c) स्थिरांक
  {
    TRACE_SUBSET (हे);
    return_trace (subset_single_subst (c, हे+coverage, deltaGlyphID, get_mask ()));
  }
};

}
}
}


#endif /* OT_LAYOUT_GSUB_SINGLESUBSTFORMAT1_HH */
