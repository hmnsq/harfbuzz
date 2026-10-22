#अगर_अघोषित OT_LAYOUT_GSUB_SINGLESUBSTFORMAT1_HH
#घोषणा OT_LAYOUT_GSUB_SINGLESUBSTFORMAT1_HH

#समावेश "Common.hh"

namespace OT {
namespace Layout {
namespace GSUB_impl {

अटल अंतरभूत bool
subset_single_subst (hb_subset_context_t *c,
                     स्थिर Coverage &coverage,
                     hb_codepoint_t d,
                     hb_codepoint_t mask)
{
  स्थिर hb_set_t &glyphset = *c->plan->glyphset_gsub ();
  स्थिर hb_map_t &glyph_map = *c->plan->glyph_map;

  hb_set_t intersection;
  coverage.intersect_set (glyphset, intersection);

  स्वतः it =
  + hb_iter (intersection)
  | hb_map_retains_sorting ([d, mask] (hb_codepoint_t g) {
                              वापस hb_codepoint_pair_t (g,
                                                          (g + d) & mask); })
  | hb_filter (glyphset, hb_second)
  | hb_map_retains_sorting ([&] (hb_codepoint_pair_t p) -> hb_codepoint_pair_t
                            { वापस hb_pair (glyph_map[p.first], glyph_map[p.second]); })
  ;

  bool ret = bool (it);
  SingleSubst_serialize (c->serializer, it);
  वापस ret;
}

HB_INTERNAL अंतरभूत
व्योम single_subst_depend (hb_depend_context_t *c,
                          स्थिर Coverage &coverage,
                          hb_codepoint_t d, hb_codepoint_t mask)
{
  अचिन्हित अंक_glyphs = c->face->get_अंक_glyphs ();

  // Filter coverage by active glyphs (like closure does)
  hb_set_t intersection;
  coverage.intersect_set (c->parent_active_glyphs (), intersection);

  + hb_iter (intersection)
  | hb_map ([&] (hb_codepoint_t _) { वापस hb_codepoint_pair_t (_, (_ + d) & mask); })
  | hb_filter ([अंक_glyphs] (स्थिर hb_codepoint_pair_t &_) { वापस _.second < अंक_glyphs; })
  | hb_apply ([&] (स्थिर hb_codepoint_pair_t &_) { c->depend_तथ्य->add_gsub_lookup(_.first, c->lookup_सूचकांक, _.second); })
  ;
}

HB_INTERNAL अंतरभूत
व्योम single_subst_closure (hb_closure_context_t *c,
                           स्थिर Coverage &coverage,
                           hb_codepoint_t d, hb_codepoint_t mask)
{
  /* Help fuzzer avoid this function as much. */
  अचिन्हित निकालो = coverage.get_population ();
  अगर (निकालो >= mask)
    वापस;

  hb_set_t intersection;
  coverage.intersect_set (c->parent_active_glyphs (), intersection);

  /* In degenerate fuzzer-found fonts, but not real fonts,
   * this table can keep adding new glyphs in each round of closure.
   * Refuse to close-over, if it maps glyph range to overlapping range. */
  hb_codepoint_t min_before = intersection.get_min ();
  hb_codepoint_t max_before = intersection.get_max ();
  hb_codepoint_t min_after = (min_before + d) & mask;
  hb_codepoint_t max_after = (max_before + d) & mask;
  अगर (intersection.get_population () == max_before - min_before + 1 &&
	((min_before <= min_after && min_after <= max_before) ||
	 (min_before <= max_after && max_after <= max_before)))
    वापस;

  + hb_iter (intersection)
  | hb_map ([d, mask] (hb_codepoint_t g) { वापस (g + d) & mask; })
  | hb_sink (c->output)
  ;
}

HB_INTERNAL अंतरभूत
व्योम single_subst_collect_glyphs (hb_collect_glyphs_context_t *c,
                                  स्थिर Coverage &coverage,
                                  hb_codepoint_t d, hb_codepoint_t mask)
{
  अगर (unlikely (!coverage.collect_coverage (c->निवेश))) वापस;

  + hb_iter (coverage)
  | hb_map ([d, mask] (hb_codepoint_t g) { वापस (g + d) & mask; })
  | hb_sink (c->output)
  ;
}

ढाँचा <typename Types>
काष्ठा SingleSubstFormat1_3
{
  रक्षित:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  typename Types::ढाँचा LOffsetTo<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of Substitution table */
  typename Types::HBUINT
                deltaGlyphID;           /* Add to original GlyphID to get
                                         * substitute GlyphID, modulo 0x10000 */

  खुला:
  DEFINE_आकार_STATIC (2 + Types::LOffset::static_आकार + Types::HBUINT::static_आकार);

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह) &&
                  coverage.sanitize (c, यह) &&
                  /* The coverage  table may use a range to represent a set
                   * of glyphs, which means a small number of bytes can
                   * generate a large glyph set. Manually modify the
                   * sanitizer max ops to take this into account.
                   *
                   * Note: This check *must* be right after coverage sanitize. */
                  c->check_ops ((यह + coverage).get_population () >> 1));
  }

  hb_codepoint_t get_mask () स्थिर
  { वापस (1 << (8 * Types::आकार)) - 1; }

  bool intersects (स्थिर hb_set_t *glyphs) स्थिर
  { वापस (यह+coverage).intersects (glyphs); }

  bool may_have_non_1to1 () स्थिर
  { वापस false; }

  व्योम depend (hb_depend_context_t *c) स्थिर
  {
    single_subst_depend (c, यह+coverage, deltaGlyphID, get_mask ());
  }

  व्योम closure (hb_closure_context_t *c) स्थिर
  {
    single_subst_closure (c, यह+coverage, deltaGlyphID, get_mask ());
  }

  व्योम closure_lookups (hb_closure_lookups_context_t *c) स्थिर {}

  व्योम collect_glyphs (hb_collect_glyphs_context_t *c) स्थिर
  {
    single_subst_collect_glyphs (c, यह+coverage, deltaGlyphID, get_mask ());
  }

  स्थिर Coverage &get_coverage () स्थिर { वापस यह+coverage; }

  bool would_apply (hb_would_apply_context_t *c) स्थिर
  { वापस c->len == 1 && (यह+coverage).get_coverage (c->glyphs[0]) != NOT_COVERED; }

  अचिन्हित
  get_glyph_alternates (hb_codepoint_t  glyph_id,
                        अचिन्हित        start_offset,
                        अचिन्हित       *alternate_गिनती  /* IN/OUT.  May be NULL. */,
                        hb_codepoint_t *alternate_glyphs /* OUT.     May be NULL. */) स्थिर
  {
    अचिन्हित पूर्णांक सूचकांक_2 = (यह+coverage).get_coverage (glyph_id);
    अगर (likely (सूचकांक_2 == NOT_COVERED))
    {
      अगर (alternate_गिनती)
        *alternate_गिनती = 0;
      वापस 0;
    }

    अगर (alternate_गिनती && *alternate_गिनती && alternate_glyphs)
    {
      hb_codepoint_t d = deltaGlyphID;
      hb_codepoint_t mask = get_mask ();

      glyph_id = (glyph_id + d) & mask;

      *alternate_glyphs = glyph_id;
      *alternate_गिनती = 1;
    }

    वापस 1;
  }

  व्योम
  collect_glyph_alternates (hb_map_t  *alternate_गिनती /* IN/OUT */,
			    hb_map_t  *alternate_glyphs /* IN/OUT */) स्थिर
  {
    hb_codepoint_t d = deltaGlyphID;
    hb_codepoint_t mask = get_mask ();

    + hb_iter (यह+coverage)
    | hb_map ([d, mask] (hb_codepoint_t g) { वापस hb_pair (g, (g + d) & mask); })
    | hb_apply ([&] (स्थिर hb_pair_t<hb_codepoint_t, hb_codepoint_t> &p) -> व्योम
		{ _hb_collect_glyph_alternates_add (p.first, p.second,
						    alternate_गिनती, alternate_glyphs); })
    ;
  }

  bool apply (hb_ot_apply_context_t *c) स्थिर
  {
    TRACE_APPLY (यह);
    hb_codepoint_t glyph_id = c->बफर->cur().codepoint;
    अचिन्हित पूर्णांक सूचकांक_2 = (यह+coverage).get_coverage (glyph_id);
    अगर (सूचकांक_2 == NOT_COVERED) return_trace (false);

    hb_codepoint_t d = deltaGlyphID;
    hb_codepoint_t mask = get_mask ();

    glyph_id = (glyph_id + d) & mask;

    अगर (HB_बफर_MESSAGE_MORE && c->बफर->messaging ())
    {
      c->बफर->sync_so_far ();
      c->बफर->message (c->font,
			  "replacing glyph at %u (single substitution)",
			  c->बफर->idx);
    }

    c->replace_glyph (glyph_id);

    अगर (HB_बफर_MESSAGE_MORE && c->बफर->messaging ())
    {
      c->बफर->message (c->font,
			  "replaced glyph at %u (single substitution)",
			  c->बफर->idx - 1u);
    }

    return_trace (true);
  }

  ढाँचा<typename Iterator,
           hb_requires (hb_is_sorted_source_of (Iterator, hb_codepoint_t))>
  bool serialize (hb_serialize_context_t *c,
                  Iterator glyphs,
                  अचिन्हित delta)
  {
    TRACE_SERIALIZE (यह);
    अगर (unlikely (!c->extend_min (यह))) return_trace (false);
    अगर (unlikely (!coverage.serialize_serialize (c, glyphs))) return_trace (false);
    c->check_assign (deltaGlyphID, delta, HB_SERIALIZE_ERROR_INT_OVERFLOW);
    return_trace (true);
  }

  bool subset (hb_subset_context_t *c) स्थिर
  {
    TRACE_SUBSET (यह);
    return_trace (subset_single_subst (c, यह+coverage, deltaGlyphID, get_mask ()));
  }
};

}
}
}


#पूर्ण_अगर /* OT_LAYOUT_GSUB_SINGLESUBSTFORMAT1_HH */
