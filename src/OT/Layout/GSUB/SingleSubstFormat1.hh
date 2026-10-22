#ifndef OT_LAYOUT_GSUB_SINGLESUBSTFORMAT1_HH
#define OT_LAYOUT_GSUB_SINGLESUBSTFORMAT1_HH

#അന്തര്ഭാവ "Common.hh"

നാമാകാശ OT {
നാമാകാശ Layout {
നാമാകാശ GSUB_impl {

സ്ഥിതം വരിയിൽ തർക്കം
subset_single_subst (hb_subset_context_t *c,
                     സ്ഥിരം Coverage &coverage,
                     hb_codepoint_t d,
                     hb_codepoint_t mask)
{
  സ്ഥിരം hb_set_t &glyphset = *c->plan->glyphset_gsub ();
  സ്ഥിരം hb_map_t &glyph_map = *c->plan->glyph_map;

  hb_set_t intersection;
  coverage.intersect_set (glyphset, intersection);

  auto it =
  + hb_iter (intersection)
  | hb_map_retains_sorting ([d, mask] (hb_codepoint_t g) {
                              തിരികെ hb_codepoint_pair_t (g,
                                                          (g + d) & mask); })
  | hb_filter (glyphset, hb_second)
  | hb_map_retains_sorting ([&] (hb_codepoint_pair_t p) -> hb_codepoint_pair_t
                            { തിരികെ hb_pair (glyph_map[p.first], glyph_map[p.second]); })
  ;

  തർക്കം ret = തർക്കം (it);
  SingleSubst_serialize (c->serializer, it);
  തിരികെ ret;
}

HB_INTERNAL വരിയിൽ
ശൂന്യം single_subst_depend (hb_depend_context_t *c,
                          സ്ഥിരം Coverage &coverage,
                          hb_codepoint_t d, hb_codepoint_t mask)
{
  unsigned അക്കം_glyphs = c->face->get_അക്കം_glyphs ();

  // Filter coverage by active glyphs (like closure does)
  hb_set_t intersection;
  coverage.intersect_set (c->parent_active_glyphs (), intersection);

  + hb_iter (intersection)
  | hb_map ([&] (hb_codepoint_t _) { തിരികെ hb_codepoint_pair_t (_, (_ + d) & mask); })
  | hb_filter ([അക്കം_glyphs] (സ്ഥിരം hb_codepoint_pair_t &_) { തിരികെ _.second < അക്കം_glyphs; })
  | hb_apply ([&] (സ്ഥിരം hb_codepoint_pair_t &_) { c->depend_data->add_gsub_lookup(_.first, c->lookup_index, _.second); })
  ;
}

HB_INTERNAL വരിയിൽ
ശൂന്യം single_subst_closure (hb_closure_context_t *c,
                           സ്ഥിരം Coverage &coverage,
                           hb_codepoint_t d, hb_codepoint_t mask)
{
  /* Help fuzzer avoid this function as much. */
  unsigned എടുക്കുക = coverage.get_population ();
  എങ്കിൽ (എടുക്കുക >= mask)
    തിരികെ;

  hb_set_t intersection;
  coverage.intersect_set (c->parent_active_glyphs (), intersection);

  /* In degenerate fuzzer-found fonts, but not real fonts,
   * this table can keep adding new glyphs in each round of closure.
   * Refuse to close-over, if it maps glyph range to overlapping range. */
  hb_codepoint_t min_before = intersection.get_min ();
  hb_codepoint_t max_before = intersection.get_max ();
  hb_codepoint_t min_after = (min_before + d) & mask;
  hb_codepoint_t max_after = (max_before + d) & mask;
  എങ്കിൽ (intersection.get_population () == max_before - min_before + 1 &&
	((min_before <= min_after && min_after <= max_before) ||
	 (min_before <= max_after && max_after <= max_before)))
    തിരികെ;

  + hb_iter (intersection)
  | hb_map ([d, mask] (hb_codepoint_t g) { തിരികെ (g + d) & mask; })
  | hb_sink (c->output)
  ;
}

HB_INTERNAL വരിയിൽ
ശൂന്യം single_subst_collect_glyphs (hb_collect_glyphs_context_t *c,
                                  സ്ഥിരം Coverage &coverage,
                                  hb_codepoint_t d, hb_codepoint_t mask)
{
  എങ്കിൽ (unlikely (!coverage.collect_coverage (c->നിവേശം))) തിരികെ;

  + hb_iter (coverage)
  | hb_map ([d, mask] (hb_codepoint_t g) { തിരികെ (g + d) & mask; })
  | hb_sink (c->output)
  ;
}

template <typename Types>
ഘടന SingleSubstFormat1_3
{
  സംരക്ഷിത:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  typename Types::template LOffsetTo<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of Substitution table */
  typename Types::HBUINT
                deltaGlyphID;           /* Add to original GlyphID to get
                                         * substitute GlyphID, modulo 0x10000 */

  പൊതു:
  DEFINE_SIZE_STATIC (2 + Types::LOffset::static_size + Types::HBUINT::static_size);

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_struct (ഇത്) &&
                  coverage.sanitize (c, ഇത്) &&
                  /* The coverage  table may use a range to represent a set
                   * of glyphs, which means a small number of bytes can
                   * generate a large glyph set. Manually modify the
                   * sanitizer max ops to take this into account.
                   *
                   * Note: This check *must* be right after coverage sanitize. */
                  c->check_ops ((ഇത് + coverage).get_population () >> 1));
  }

  hb_codepoint_t get_mask () സ്ഥിരം
  { തിരികെ (1 << (8 * Types::size)) - 1; }

  തർക്കം intersects (സ്ഥിരം hb_set_t *glyphs) സ്ഥിരം
  { തിരികെ (ഇത്+coverage).intersects (glyphs); }

  തർക്കം may_have_non_1to1 () സ്ഥിരം
  { തിരികെ അസത്യം; }

  ശൂന്യം depend (hb_depend_context_t *c) സ്ഥിരം
  {
    single_subst_depend (c, ഇത്+coverage, deltaGlyphID, get_mask ());
  }

  ശൂന്യം closure (hb_closure_context_t *c) സ്ഥിരം
  {
    single_subst_closure (c, ഇത്+coverage, deltaGlyphID, get_mask ());
  }

  ശൂന്യം closure_lookups (hb_closure_lookups_context_t *c) സ്ഥിരം {}

  ശൂന്യം collect_glyphs (hb_collect_glyphs_context_t *c) സ്ഥിരം
  {
    single_subst_collect_glyphs (c, ഇത്+coverage, deltaGlyphID, get_mask ());
  }

  സ്ഥിരം Coverage &get_coverage () സ്ഥിരം { തിരികെ ഇത്+coverage; }

  തർക്കം would_apply (hb_would_apply_context_t *c) സ്ഥിരം
  { തിരികെ c->len == 1 && (ഇത്+coverage).get_coverage (c->glyphs[0]) != NOT_COVERED; }

  unsigned
  get_glyph_alternates (hb_codepoint_t  glyph_id,
                        unsigned        start_offset,
                        unsigned       *alternate_എണ്ണം  /* IN/OUT.  May be NULL. */,
                        hb_codepoint_t *alternate_glyphs /* OUT.     May be NULL. */) സ്ഥിരം
  {
    unsigned പൂർണ്ണസംഖ്യ index = (ഇത്+coverage).get_coverage (glyph_id);
    എങ്കിൽ (likely (index == NOT_COVERED))
    {
      എങ്കിൽ (alternate_എണ്ണം)
        *alternate_എണ്ണം = 0;
      തിരികെ 0;
    }

    എങ്കിൽ (alternate_എണ്ണം && *alternate_എണ്ണം && alternate_glyphs)
    {
      hb_codepoint_t d = deltaGlyphID;
      hb_codepoint_t mask = get_mask ();

      glyph_id = (glyph_id + d) & mask;

      *alternate_glyphs = glyph_id;
      *alternate_എണ്ണം = 1;
    }

    തിരികെ 1;
  }

  ശൂന്യം
  collect_glyph_alternates (hb_map_t  *alternate_എണ്ണം /* IN/OUT */,
			    hb_map_t  *alternate_glyphs /* IN/OUT */) സ്ഥിരം
  {
    hb_codepoint_t d = deltaGlyphID;
    hb_codepoint_t mask = get_mask ();

    + hb_iter (ഇത്+coverage)
    | hb_map ([d, mask] (hb_codepoint_t g) { തിരികെ hb_pair (g, (g + d) & mask); })
    | hb_apply ([&] (സ്ഥിരം hb_pair_t<hb_codepoint_t, hb_codepoint_t> &p) -> ശൂന്യം
		{ _hb_collect_glyph_alternates_add (p.first, p.second,
						    alternate_എണ്ണം, alternate_glyphs); })
    ;
  }

  തർക്കം apply (hb_ot_apply_context_t *c) സ്ഥിരം
  {
    TRACE_APPLY (ഇത്);
    hb_codepoint_t glyph_id = c->buffer->cur().codepoint;
    unsigned പൂർണ്ണസംഖ്യ index = (ഇത്+coverage).get_coverage (glyph_id);
    എങ്കിൽ (index == NOT_COVERED) return_trace (അസത്യം);

    hb_codepoint_t d = deltaGlyphID;
    hb_codepoint_t mask = get_mask ();

    glyph_id = (glyph_id + d) & mask;

    എങ്കിൽ (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->sync_so_far ();
      c->buffer->message (c->font,
			  "replacing glyph at %u (single substitution)",
			  c->buffer->idx);
    }

    c->replace_glyph (glyph_id);

    എങ്കിൽ (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->message (c->font,
			  "replaced glyph at %u (single substitution)",
			  c->buffer->idx - 1u);
    }

    return_trace (സത്യം);
  }

  template<typename Iterator,
           hb_requires (hb_is_sorted_source_of (Iterator, hb_codepoint_t))>
  തർക്കം serialize (hb_serialize_context_t *c,
                  Iterator glyphs,
                  unsigned delta)
  {
    TRACE_SERIALIZE (ഇത്);
    എങ്കിൽ (unlikely (!c->extend_min (ഇത്))) return_trace (അസത്യം);
    എങ്കിൽ (unlikely (!coverage.serialize_serialize (c, glyphs))) return_trace (അസത്യം);
    c->check_assign (deltaGlyphID, delta, HB_SERIALIZE_ERROR_INT_OVERFLOW);
    return_trace (സത്യം);
  }

  തർക്കം subset (hb_subset_context_t *c) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);
    return_trace (subset_single_subst (c, ഇത്+coverage, deltaGlyphID, get_mask ()));
  }
};

}
}
}


#endif /* OT_LAYOUT_GSUB_SINGLESUBSTFORMAT1_HH */
