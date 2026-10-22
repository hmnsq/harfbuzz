#ifndef OT_LAYOUT_GSUB_SINGLESUBSTFORMAT2_HH
#define OT_LAYOUT_GSUB_SINGLESUBSTFORMAT2_HH

#ଅନ୍ତର୍ଭାଵ "Common.hh"

ନାମାକାଶ OT {
ନାମାକାଶ Layout {
ନାମାକାଶ GSUB_impl {

template <typename Types>
ସଂରଚନା SingleSubstFormat2_4
{
  ସୁରକ୍ଷିତ:
  HBUINT16      format;                 /* Format identifier--format = 2 */
  typename Types::template LOffsetTo<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of Substitution table */
  typename Types::template ArrayOf<typename Types::HBGlyphID>
                substitute;             /* Array of substitute
                                         * GlyphIDs--ordered by Coverage Index */

  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_ARRAY (2 + Types::LOffset::static_size + Types::HBUINT::static_size, substitute);

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (coverage.sanitize (c, ଏହି) && substitute.sanitize (c));
  }

  ତାର୍କିକ intersects (ଅଚଳ hb_set_t *glyphs) ଅଚଳ
  { ଫେରନ୍ତୁ (ଏହି+coverage).intersects (glyphs); }

  ତାର୍କିକ may_have_non_1to1 () ଅଚଳ
  { ଫେରନ୍ତୁ ମିଥ୍ୟା; }

  ଶୂନ୍ୟ depend (hb_depend_context_t *c) ଅଚଳ
  {
    auto &cov = ଏହି+coverage;
    auto &glyph_set = c->parent_active_glyphs ();

    // Filter by active glyphs like closure does
    ଯଦି (substitute.len > glyph_set.get_population () * 4)
    {
      ପାଇଁ (auto g : glyph_set)
      {
	unsigned i = cov.get_coverage (g);
	ଯଦି (i == NOT_COVERED || i >= substitute.len)
	  ଜାରି;
	c->depend_data->add_gsub_lookup (g, c->lookup_index, substitute.arrayZ[i]);
      }

      ଫେରନ୍ତୁ;
    }

    + hb_zip (cov, substitute)
    | hb_filter (glyph_set, hb_first)
    | hb_apply ([&] (ଅଚଳ hb_codepoint_pair_t &_) { c->depend_data->add_gsub_lookup (_.first, c->lookup_index, _.second); })
    ;
  }

  ଶୂନ୍ୟ closure (hb_closure_context_t *c) ଅଚଳ
  {
    auto &cov = ଏହି+coverage;
    auto &glyph_set = c->parent_active_glyphs ();

    ଯଦି (substitute.len > glyph_set.get_population () * 4)
    {
      ପାଇଁ (auto g : glyph_set)
      {
	unsigned i = cov.get_coverage (g);
	ଯଦି (i == NOT_COVERED || i >= substitute.len)
	  ଜାରି;
	c->output->add (substitute.arrayZ[i]);
      }

      ଫେରନ୍ତୁ;
    }

    + hb_zip (cov, substitute)
    | hb_filter (glyph_set, hb_first)
    | hb_map (hb_second)
    | hb_sink (c->output)
    ;
  }

  ଶୂନ୍ୟ closure_lookups (hb_closure_lookups_context_t *c) ଅଚଳ {}

  ଶୂନ୍ୟ collect_glyphs (hb_collect_glyphs_context_t *c) ଅଚଳ
  {
    ଯଦି (unlikely (!(ଏହି+coverage).collect_coverage (c->ନିବେଶ))) ଫେରନ୍ତୁ;
    + hb_zip (ଏହି+coverage, substitute)
    | hb_map (hb_second)
    | hb_sink (c->output)
    ;
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
      glyph_id = substitute[index];

      *alternate_glyphs = glyph_id;
      *alternate_ଗଣନା = 1;
    }

    ଫେରନ୍ତୁ 1;
  }

  ଶୂନ୍ୟ
  collect_glyph_alternates (hb_map_t  *alternate_ଗଣନା /* IN/OUT */,
			    hb_map_t  *alternate_glyphs /* IN/OUT */) ଅଚଳ
  {
    + hb_zip (ଏହି+coverage, substitute)
    | hb_apply ([&] (ଅଚଳ hb_pair_t<hb_codepoint_t, hb_codepoint_t> &p) -> ଶୂନ୍ୟ
		{ _hb_collect_glyph_alternates_add (p.first, p.second,
						    alternate_ଗଣନା, alternate_glyphs); })
    ;
  }

  ତାର୍କିକ apply (hb_ot_apply_context_t *c) ଅଚଳ
  {
    TRACE_APPLY (ଏହି);
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା index = (ଏହି+coverage).get_coverage (c->buffer->cur().codepoint);
    ଯଦି (index == NOT_COVERED) return_trace (ମିଥ୍ୟା);

    ଯଦି (unlikely (index >= substitute.len)) return_trace (ମିଥ୍ୟା);

    ଯଦି (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->sync_so_far ();
      c->buffer->message (c->font,
			  "replacing glyph at %u (single substitution)",
			  c->buffer->idx);
    }

    c->replace_glyph (substitute[index]);

    ଯଦି (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->message (c->font,
			  "replaced glyph at %u (single substitution)",
			  c->buffer->idx - 1u);
    }

    return_trace (ସତ୍ୟ);
  }

  template<typename Iterator,
           hb_requires (hb_is_sorted_source_of (Iterator,
                                                hb_codepoint_pair_t))>
  ତାର୍କିକ serialize (hb_serialize_context_t *c,
                  Iterator it)
  {
    TRACE_SERIALIZE (ଏହି);
    auto substitutes =
      + it
      | hb_map (hb_second)
      ;
    auto glyphs =
      + it
      | hb_map_retains_sorting (hb_first)
      ;
    ଯଦି (unlikely (!c->extend_min (ଏହି))) return_trace (ମିଥ୍ୟା);
    ଯଦି (unlikely (!substitute.serialize (c, substitutes))) return_trace (ମିଥ୍ୟା);
    ଯଦି (unlikely (!coverage.serialize_serialize (c, glyphs))) return_trace (ମିଥ୍ୟା);
    return_trace (ସତ୍ୟ);
  }

  ତାର୍କିକ subset (hb_subset_context_t *c) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    ଅଚଳ hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ଅଚଳ hb_map_t &glyph_map = *c->plan->glyph_map;

    auto it =
    + hb_zip (ଏହି+coverage, substitute)
    | hb_filter (glyphset, hb_first)
    | hb_filter (glyphset, hb_second)
    | hb_map_retains_sorting ([&] (hb_pair_t<hb_codepoint_t, ଅଚଳ typename Types::HBGlyphID &> p) -> hb_codepoint_pair_t
                              { ଫେରନ୍ତୁ hb_pair (glyph_map[p.first], glyph_map[p.second]); })
    ;

    ତାର୍କିକ ret = ତାର୍କିକ (it);
    SingleSubst_serialize (c->serializer, it);
    return_trace (ret);
  }
};

}
}
}

#endif /* OT_LAYOUT_GSUB_SINGLESUBSTFORMAT2_HH */
