#ifndef OT_LAYOUT_GSUB_SINGLESUBSTFORMAT1_HH
#define OT_LAYOUT_GSUB_SINGLESUBSTFORMAT1_HH

#અન્તર્ભાવ "Common.hh"

નામાકાશ OT {
નામાકાશ Layout {
નામાકાશ GSUB_impl {

સ્થિર અંતઃરેખ તાર્કિક
subset_single_subst (hb_subset_context_t *c,
                     અચલ Coverage &coverage,
                     hb_codepoint_t d,
                     hb_codepoint_t mask)
{
  અચલ hb_set_t &glyphset = *c->plan->glyphset_gsub ();
  અચલ hb_map_t &glyph_map = *c->plan->glyph_map;

  hb_set_t intersection;
  coverage.intersect_set (glyphset, intersection);

  auto it =
  + hb_iter (intersection)
  | hb_map_retains_sorting ([d, mask] (hb_codepoint_t g) {
                              પાછા hb_codepoint_pair_t (g,
                                                          (g + d) & mask); })
  | hb_filter (glyphset, hb_second)
  | hb_map_retains_sorting ([&] (hb_codepoint_pair_t p) -> hb_codepoint_pair_t
                            { પાછા hb_pair (glyph_map[p.first], glyph_map[p.second]); })
  ;

  તાર્કિક ret = તાર્કિક (it);
  SingleSubst_serialize (c->serializer, it);
  પાછા ret;
}

HB_INTERNAL અંતઃરેખ
શૂન્ય single_subst_depend (hb_depend_context_t *c,
                          અચલ Coverage &coverage,
                          hb_codepoint_t d, hb_codepoint_t mask)
{
  unsigned અંક_glyphs = c->face->get_અંક_glyphs ();

  // Filter coverage by active glyphs (like closure does)
  hb_set_t intersection;
  coverage.intersect_set (c->parent_active_glyphs (), intersection);

  + hb_iter (intersection)
  | hb_map ([&] (hb_codepoint_t _) { પાછા hb_codepoint_pair_t (_, (_ + d) & mask); })
  | hb_filter ([અંક_glyphs] (અચલ hb_codepoint_pair_t &_) { પાછા _.second < અંક_glyphs; })
  | hb_apply ([&] (અચલ hb_codepoint_pair_t &_) { c->depend_data->add_gsub_lookup(_.first, c->lookup_index, _.second); })
  ;
}

HB_INTERNAL અંતઃરેખ
શૂન્ય single_subst_closure (hb_closure_context_t *c,
                           અચલ Coverage &coverage,
                           hb_codepoint_t d, hb_codepoint_t mask)
{
  /* Help fuzzer avoid this function as much. */
  unsigned કાઢો = coverage.get_population ();
  જો (કાઢો >= mask)
    પાછા;

  hb_set_t intersection;
  coverage.intersect_set (c->parent_active_glyphs (), intersection);

  /* In degenerate fuzzer-found fonts, but not real fonts,
   * this table can keep adding new glyphs in each round of closure.
   * Refuse to close-over, if it maps glyph range to overlapping range. */
  hb_codepoint_t min_before = intersection.get_min ();
  hb_codepoint_t max_before = intersection.get_max ();
  hb_codepoint_t min_after = (min_before + d) & mask;
  hb_codepoint_t max_after = (max_before + d) & mask;
  જો (intersection.get_population () == max_before - min_before + 1 &&
	((min_before <= min_after && min_after <= max_before) ||
	 (min_before <= max_after && max_after <= max_before)))
    પાછા;

  + hb_iter (intersection)
  | hb_map ([d, mask] (hb_codepoint_t g) { પાછા (g + d) & mask; })
  | hb_sink (c->output)
  ;
}

HB_INTERNAL અંતઃરેખ
શૂન્ય single_subst_collect_glyphs (hb_collect_glyphs_context_t *c,
                                  અચલ Coverage &coverage,
                                  hb_codepoint_t d, hb_codepoint_t mask)
{
  જો (unlikely (!coverage.collect_coverage (c->આગત))) પાછા;

  + hb_iter (coverage)
  | hb_map ([d, mask] (hb_codepoint_t g) { પાછા (g + d) & mask; })
  | hb_sink (c->output)
  ;
}

template <typename Types>
સંરચના SingleSubstFormat1_3
{
  સુરક્ષિત:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  typename Types::template LOffsetTo<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of Substitution table */
  typename Types::HBUINT
                deltaGlyphID;           /* Add to original GlyphID to get
                                         * substitute GlyphID, modulo 0x10000 */

  જાહેર:
  DEFINE_SIZE_STATIC (2 + Types::LOffset::static_size + Types::HBUINT::static_size);

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ) &&
                  coverage.sanitize (c, આ) &&
                  /* The coverage  table may use a range to represent a set
                   * of glyphs, which means a small number of bytes can
                   * generate a large glyph set. Manually modify the
                   * sanitizer max ops to take this into account.
                   *
                   * Note: This check *must* be right after coverage sanitize. */
                  c->check_ops ((આ + coverage).get_population () >> 1));
  }

  hb_codepoint_t get_mask () અચલ
  { પાછા (1 << (8 * Types::size)) - 1; }

  તાર્કિક intersects (અચલ hb_set_t *glyphs) અચલ
  { પાછા (આ+coverage).intersects (glyphs); }

  તાર્કિક may_have_non_1to1 () અચલ
  { પાછા ખોટું; }

  શૂન્ય depend (hb_depend_context_t *c) અચલ
  {
    single_subst_depend (c, આ+coverage, deltaGlyphID, get_mask ());
  }

  શૂન્ય closure (hb_closure_context_t *c) અચલ
  {
    single_subst_closure (c, આ+coverage, deltaGlyphID, get_mask ());
  }

  શૂન્ય closure_lookups (hb_closure_lookups_context_t *c) અચલ {}

  શૂન્ય collect_glyphs (hb_collect_glyphs_context_t *c) અચલ
  {
    single_subst_collect_glyphs (c, આ+coverage, deltaGlyphID, get_mask ());
  }

  અચલ Coverage &get_coverage () અચલ { પાછા આ+coverage; }

  તાર્કિક would_apply (hb_would_apply_context_t *c) અચલ
  { પાછા c->len == 1 && (આ+coverage).get_coverage (c->glyphs[0]) != NOT_COVERED; }

  unsigned
  get_glyph_alternates (hb_codepoint_t  glyph_id,
                        unsigned        start_offset,
                        unsigned       *alternate_ગણતરી  /* IN/OUT.  May be NULL. */,
                        hb_codepoint_t *alternate_glyphs /* OUT.     May be NULL. */) અચલ
  {
    unsigned પૂર્ણાંક index = (આ+coverage).get_coverage (glyph_id);
    જો (likely (index == NOT_COVERED))
    {
      જો (alternate_ગણતરી)
        *alternate_ગણતરી = 0;
      પાછા 0;
    }

    જો (alternate_ગણતરી && *alternate_ગણતરી && alternate_glyphs)
    {
      hb_codepoint_t d = deltaGlyphID;
      hb_codepoint_t mask = get_mask ();

      glyph_id = (glyph_id + d) & mask;

      *alternate_glyphs = glyph_id;
      *alternate_ગણતરી = 1;
    }

    પાછા 1;
  }

  શૂન્ય
  collect_glyph_alternates (hb_map_t  *alternate_ગણતરી /* IN/OUT */,
			    hb_map_t  *alternate_glyphs /* IN/OUT */) અચલ
  {
    hb_codepoint_t d = deltaGlyphID;
    hb_codepoint_t mask = get_mask ();

    + hb_iter (આ+coverage)
    | hb_map ([d, mask] (hb_codepoint_t g) { પાછા hb_pair (g, (g + d) & mask); })
    | hb_apply ([&] (અચલ hb_pair_t<hb_codepoint_t, hb_codepoint_t> &p) -> શૂન્ય
		{ _hb_collect_glyph_alternates_add (p.first, p.second,
						    alternate_ગણતરી, alternate_glyphs); })
    ;
  }

  તાર્કિક apply (hb_ot_apply_context_t *c) અચલ
  {
    TRACE_APPLY (આ);
    hb_codepoint_t glyph_id = c->buffer->cur().codepoint;
    unsigned પૂર્ણાંક index = (આ+coverage).get_coverage (glyph_id);
    જો (index == NOT_COVERED) return_trace (ખોટું);

    hb_codepoint_t d = deltaGlyphID;
    hb_codepoint_t mask = get_mask ();

    glyph_id = (glyph_id + d) & mask;

    જો (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->sync_so_far ();
      c->buffer->message (c->font,
			  "replacing glyph at %u (single substitution)",
			  c->buffer->idx);
    }

    c->replace_glyph (glyph_id);

    જો (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->message (c->font,
			  "replaced glyph at %u (single substitution)",
			  c->buffer->idx - 1u);
    }

    return_trace (સાચું);
  }

  template<typename Iterator,
           hb_requires (hb_is_sorted_source_of (Iterator, hb_codepoint_t))>
  તાર્કિક serialize (hb_serialize_context_t *c,
                  Iterator glyphs,
                  unsigned delta)
  {
    TRACE_SERIALIZE (આ);
    જો (unlikely (!c->extend_min (આ))) return_trace (ખોટું);
    જો (unlikely (!coverage.serialize_serialize (c, glyphs))) return_trace (ખોટું);
    c->check_assign (deltaGlyphID, delta, HB_SERIALIZE_ERROR_INT_OVERFLOW);
    return_trace (સાચું);
  }

  તાર્કિક subset (hb_subset_context_t *c) અચલ
  {
    TRACE_SUBSET (આ);
    return_trace (subset_single_subst (c, આ+coverage, deltaGlyphID, get_mask ()));
  }
};

}
}
}


#endif /* OT_LAYOUT_GSUB_SINGLESUBSTFORMAT1_HH */
