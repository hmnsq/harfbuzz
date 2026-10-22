#ifndef OT_LAYOUT_GSUB_SINGLESUBSTFORMAT2_HH
#define OT_LAYOUT_GSUB_SINGLESUBSTFORMAT2_HH

#अन्तर्भाव "Common.hh"

नामाकाश OT {
नामाकाश Layout {
नामाकाश GSUB_impl {

template <typename Types>
संरचना SingleSubstFormat2_4
{
  रक्षित:
  HBUINT16      format;                 /* Format identifier--format = 2 */
  typename Types::template LOffsetTo<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of Substitution table */
  typename Types::template ArrayOf<typename Types::HBGlyphID>
                substitute;             /* Array of substitute
                                         * GlyphIDs--ordered by Coverage Index */

  सार्वजनिक:
  DEFINE_SIZE_ARRAY (2 + Types::LOffset::static_size + Types::HBUINT::static_size, substitute);

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (coverage.sanitize (c, एतत्) && substitute.sanitize (c));
  }

  तार्किक intersects (ध्रुव hb_set_t *glyphs) ध्रुव
  { पटिनिवत्त (एतत्+coverage).intersects (glyphs); }

  तार्किक may_have_non_1to1 () ध्रुव
  { पटिनिवत्त मिच्छा; }

  शून्य depend (hb_depend_context_t *c) ध्रुव
  {
    auto &cov = एतत्+coverage;
    auto &glyph_set = c->parent_active_glyphs ();

    // Filter by active glyphs like closure does
    सचे (substitute.len > glyph_set.get_population () * 4)
    {
      अत्थाय (auto g : glyph_set)
      {
	unsigned i = cov.get_coverage (g);
	सचे (i == NOT_COVERED || i >= substitute.len)
	  अनुवर्त;
	c->depend_data->add_gsub_lookup (g, c->lookup_index, substitute.arrayZ[i]);
      }

      पटिनिवत्त;
    }

    + hb_zip (cov, substitute)
    | hb_filter (glyph_set, hb_first)
    | hb_apply ([&] (ध्रुव hb_codepoint_pair_t &_) { c->depend_data->add_gsub_lookup (_.first, c->lookup_index, _.second); })
    ;
  }

  शून्य closure (hb_closure_context_t *c) ध्रुव
  {
    auto &cov = एतत्+coverage;
    auto &glyph_set = c->parent_active_glyphs ();

    सचे (substitute.len > glyph_set.get_population () * 4)
    {
      अत्थाय (auto g : glyph_set)
      {
	unsigned i = cov.get_coverage (g);
	सचे (i == NOT_COVERED || i >= substitute.len)
	  अनुवर्त;
	c->output->add (substitute.arrayZ[i]);
      }

      पटिनिवत्त;
    }

    + hb_zip (cov, substitute)
    | hb_filter (glyph_set, hb_first)
    | hb_map (hb_second)
    | hb_sink (c->output)
    ;
  }

  शून्य closure_lookups (hb_closure_lookups_context_t *c) ध्रुव {}

  शून्य collect_glyphs (hb_collect_glyphs_context_t *c) ध्रुव
  {
    सचे (unlikely (!(एतत्+coverage).collect_coverage (c->निवेशः))) पटिनिवत्त;
    + hb_zip (एतत्+coverage, substitute)
    | hb_map (hb_second)
    | hb_sink (c->output)
    ;
  }

  ध्रुव Coverage &get_coverage () ध्रुव { पटिनिवत्त एतत्+coverage; }

  तार्किक would_apply (hb_would_apply_context_t *c) ध्रुव
  { पटिनिवत्त c->len == 1 && (एतत्+coverage).get_coverage (c->glyphs[0]) != NOT_COVERED; }

  unsigned
  get_glyph_alternates (hb_codepoint_t  glyph_id,
                        unsigned        start_offset,
                        unsigned       *alternate_गणनम्  /* IN/OUT.  May be NULL. */,
                        hb_codepoint_t *alternate_glyphs /* OUT.     May be NULL. */) ध्रुव
  {
    unsigned पूर्णाङ्क index = (एतत्+coverage).get_coverage (glyph_id);
    सचे (likely (index == NOT_COVERED))
    {
      सचे (alternate_गणनम्)
        *alternate_गणनम् = 0;
      पटिनिवत्त 0;
    }

    सचे (alternate_गणनम् && *alternate_गणनम् && alternate_glyphs)
    {
      glyph_id = substitute[index];

      *alternate_glyphs = glyph_id;
      *alternate_गणनम् = 1;
    }

    पटिनिवत्त 1;
  }

  शून्य
  collect_glyph_alternates (hb_map_t  *alternate_गणनम् /* IN/OUT */,
			    hb_map_t  *alternate_glyphs /* IN/OUT */) ध्रुव
  {
    + hb_zip (एतत्+coverage, substitute)
    | hb_apply ([&] (ध्रुव hb_pair_t<hb_codepoint_t, hb_codepoint_t> &p) -> शून्य
		{ _hb_collect_glyph_alternates_add (p.first, p.second,
						    alternate_गणनम्, alternate_glyphs); })
    ;
  }

  तार्किक apply (hb_ot_apply_context_t *c) ध्रुव
  {
    TRACE_APPLY (एतत्);
    unsigned पूर्णाङ्क index = (एतत्+coverage).get_coverage (c->buffer->cur().codepoint);
    सचे (index == NOT_COVERED) return_trace (मिच्छा);

    सचे (unlikely (index >= substitute.len)) return_trace (मिच्छा);

    सचे (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->sync_so_far ();
      c->buffer->message (c->font,
			  "replacing glyph at %u (single substitution)",
			  c->buffer->idx);
    }

    c->replace_glyph (substitute[index]);

    सचे (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->message (c->font,
			  "replaced glyph at %u (single substitution)",
			  c->buffer->idx - 1u);
    }

    return_trace (सच्च);
  }

  template<typename Iterator,
           hb_requires (hb_is_sorted_source_of (Iterator,
                                                hb_codepoint_pair_t))>
  तार्किक serialize (hb_serialize_context_t *c,
                  Iterator it)
  {
    TRACE_SERIALIZE (एतत्);
    auto substitutes =
      + it
      | hb_map (hb_second)
      ;
    auto glyphs =
      + it
      | hb_map_retains_sorting (hb_first)
      ;
    सचे (unlikely (!c->extend_min (एतत्))) return_trace (मिच्छा);
    सचे (unlikely (!substitute.serialize (c, substitutes))) return_trace (मिच्छा);
    सचे (unlikely (!coverage.serialize_serialize (c, glyphs))) return_trace (मिच्छा);
    return_trace (सच्च);
  }

  तार्किक subset (hb_subset_context_t *c) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    ध्रुव hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ध्रुव hb_map_t &glyph_map = *c->plan->glyph_map;

    auto it =
    + hb_zip (एतत्+coverage, substitute)
    | hb_filter (glyphset, hb_first)
    | hb_filter (glyphset, hb_second)
    | hb_map_retains_sorting ([&] (hb_pair_t<hb_codepoint_t, ध्रुव typename Types::HBGlyphID &> p) -> hb_codepoint_pair_t
                              { पटिनिवत्त hb_pair (glyph_map[p.first], glyph_map[p.second]); })
    ;

    तार्किक ret = तार्किक (it);
    SingleSubst_serialize (c->serializer, it);
    return_trace (ret);
  }
};

}
}
}

#endif /* OT_LAYOUT_GSUB_SINGLESUBSTFORMAT2_HH */
