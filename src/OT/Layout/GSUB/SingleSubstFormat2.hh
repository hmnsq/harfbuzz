#ifndef OT_LAYOUT_GSUB_SINGLESUBSTFORMAT2_HH
#define OT_LAYOUT_GSUB_SINGLESUBSTFORMAT2_HH

#அந்தர்பாவ "Common.hh"

நாமாகாஶ OT {
நாமாகாஶ Layout {
நாமாகாஶ GSUB_impl {

template <typename Types>
கட்டமைப்பு SingleSubstFormat2_4
{
  பாதுகாக்கப்பட்ட:
  HBUINT16      format;                 /* Format identifier--format = 2 */
  typename Types::template LOffsetTo<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of Substitution table */
  typename Types::template ArrayOf<typename Types::HBGlyphID>
                substitute;             /* Array of substitute
                                         * GlyphIDs--ordered by Coverage Index */

  பொது:
  DEFINE_SIZE_ARRAY (2 + Types::LOffset::static_size + Types::HBUINT::static_size, substitute);

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (coverage.sanitize (c, இது) && substitute.sanitize (c));
  }

  தருக்க intersects (மாறிலி hb_set_t *glyphs) மாறிலி
  { திரும்பு (இது+coverage).intersects (glyphs); }

  தருக்க may_have_non_1to1 () மாறிலி
  { திரும்பு பொய்; }

  வெறுமை depend (hb_depend_context_t *c) மாறிலி
  {
    auto &cov = இது+coverage;
    auto &glyph_set = c->parent_active_glyphs ();

    // Filter by active glyphs like closure does
    எனில் (substitute.len > glyph_set.get_population () * 4)
    {
      க்கு (auto g : glyph_set)
      {
	unsigned i = cov.get_coverage (g);
	எனில் (i == NOT_COVERED || i >= substitute.len)
	  தொடர்;
	c->depend_data->add_gsub_lookup (g, c->lookup_index, substitute.arrayZ[i]);
      }

      திரும்பு;
    }

    + hb_zip (cov, substitute)
    | hb_filter (glyph_set, hb_first)
    | hb_apply ([&] (மாறிலி hb_codepoint_pair_t &_) { c->depend_data->add_gsub_lookup (_.first, c->lookup_index, _.second); })
    ;
  }

  வெறுமை closure (hb_closure_context_t *c) மாறிலி
  {
    auto &cov = இது+coverage;
    auto &glyph_set = c->parent_active_glyphs ();

    எனில் (substitute.len > glyph_set.get_population () * 4)
    {
      க்கு (auto g : glyph_set)
      {
	unsigned i = cov.get_coverage (g);
	எனில் (i == NOT_COVERED || i >= substitute.len)
	  தொடர்;
	c->output->add (substitute.arrayZ[i]);
      }

      திரும்பு;
    }

    + hb_zip (cov, substitute)
    | hb_filter (glyph_set, hb_first)
    | hb_map (hb_second)
    | hb_sink (c->output)
    ;
  }

  வெறுமை closure_lookups (hb_closure_lookups_context_t *c) மாறிலி {}

  வெறுமை collect_glyphs (hb_collect_glyphs_context_t *c) மாறிலி
  {
    எனில் (unlikely (!(இது+coverage).collect_coverage (c->உள்ளீடு))) திரும்பு;
    + hb_zip (இது+coverage, substitute)
    | hb_map (hb_second)
    | hb_sink (c->output)
    ;
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
      glyph_id = substitute[index];

      *alternate_glyphs = glyph_id;
      *alternate_மொத்தம் = 1;
    }

    திரும்பு 1;
  }

  வெறுமை
  collect_glyph_alternates (hb_map_t  *alternate_மொத்தம் /* IN/OUT */,
			    hb_map_t  *alternate_glyphs /* IN/OUT */) மாறிலி
  {
    + hb_zip (இது+coverage, substitute)
    | hb_apply ([&] (மாறிலி hb_pair_t<hb_codepoint_t, hb_codepoint_t> &p) -> வெறுமை
		{ _hb_collect_glyph_alternates_add (p.first, p.second,
						    alternate_மொத்தம், alternate_glyphs); })
    ;
  }

  தருக்க apply (hb_ot_apply_context_t *c) மாறிலி
  {
    TRACE_APPLY (இது);
    unsigned முழு index = (இது+coverage).get_coverage (c->buffer->cur().codepoint);
    எனில் (index == NOT_COVERED) return_trace (பொய்);

    எனில் (unlikely (index >= substitute.len)) return_trace (பொய்);

    எனில் (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->sync_so_far ();
      c->buffer->message (c->font,
			  "replacing glyph at %u (single substitution)",
			  c->buffer->idx);
    }

    c->replace_glyph (substitute[index]);

    எனில் (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->message (c->font,
			  "replaced glyph at %u (single substitution)",
			  c->buffer->idx - 1u);
    }

    return_trace (உண்மை);
  }

  template<typename Iterator,
           hb_requires (hb_is_sorted_source_of (Iterator,
                                                hb_codepoint_pair_t))>
  தருக்க serialize (hb_serialize_context_t *c,
                  Iterator it)
  {
    TRACE_SERIALIZE (இது);
    auto substitutes =
      + it
      | hb_map (hb_second)
      ;
    auto glyphs =
      + it
      | hb_map_retains_sorting (hb_first)
      ;
    எனில் (unlikely (!c->extend_min (இது))) return_trace (பொய்);
    எனில் (unlikely (!substitute.serialize (c, substitutes))) return_trace (பொய்);
    எனில் (unlikely (!coverage.serialize_serialize (c, glyphs))) return_trace (பொய்);
    return_trace (உண்மை);
  }

  தருக்க subset (hb_subset_context_t *c) மாறிலி
  {
    TRACE_SUBSET (இது);
    மாறிலி hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    மாறிலி hb_map_t &glyph_map = *c->plan->glyph_map;

    auto it =
    + hb_zip (இது+coverage, substitute)
    | hb_filter (glyphset, hb_first)
    | hb_filter (glyphset, hb_second)
    | hb_map_retains_sorting ([&] (hb_pair_t<hb_codepoint_t, மாறிலி typename Types::HBGlyphID &> p) -> hb_codepoint_pair_t
                              { திரும்பு hb_pair (glyph_map[p.first], glyph_map[p.second]); })
    ;

    தருக்க ret = தருக்க (it);
    SingleSubst_serialize (c->serializer, it);
    return_trace (ret);
  }
};

}
}
}

#endif /* OT_LAYOUT_GSUB_SINGLESUBSTFORMAT2_HH */
