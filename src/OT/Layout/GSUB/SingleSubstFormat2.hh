#ifndef OT_LAYOUT_GSUB_SINGLESUBSTFORMAT2_HH
#define OT_LAYOUT_GSUB_SINGLESUBSTFORMAT2_HH

#ಅನ್ತರ್ಭಾವ "Common.hh"

ನಾಮಾಕಾಶ OT {
ನಾಮಾಕಾಶ Layout {
ನಾಮಾಕಾಶ GSUB_impl {

template <typename Types>
ಸಂರಚನಾ SingleSubstFormat2_4
{
  ರಕ್ಷಿತ:
  HBUINT16      format;                 /* Format identifier--format = 2 */
  typename Types::template LOffsetTo<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of Substitution table */
  typename Types::template ArrayOf<typename Types::HBGlyphID>
                substitute;             /* Array of substitute
                                         * GlyphIDs--ordered by Coverage Index */

  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_ARRAY (2 + Types::LOffset::static_size + Types::HBUINT::static_size, substitute);

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (coverage.sanitize (c, ಏತತ್) && substitute.sanitize (c));
  }

  ತಾರ್ಕಿಕ intersects (ಧ್ರುವ hb_set_t *glyphs) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ (ಏತತ್+coverage).intersects (glyphs); }

  ತಾರ್ಕಿಕ may_have_non_1to1 () ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ; }

  ಶೂನ್ಯ depend (hb_depend_context_t *c) ಧ್ರುವ
  {
    auto &cov = ಏತತ್+coverage;
    auto &glyph_set = c->parent_active_glyphs ();

    // Filter by active glyphs like closure does
    ಯದಿ (substitute.len > glyph_set.get_population () * 4)
    {
      ಪ್ರತಿ (auto g : glyph_set)
      {
	unsigned i = cov.get_coverage (g);
	ಯದಿ (i == NOT_COVERED || i >= substitute.len)
	  ಅನುವರ್ತ;
	c->depend_data->add_gsub_lookup (g, c->lookup_index, substitute.arrayZ[i]);
      }

      ಪ್ರತ್ಯಾಗಮ;
    }

    + hb_zip (cov, substitute)
    | hb_filter (glyph_set, hb_first)
    | hb_apply ([&] (ಧ್ರುವ hb_codepoint_pair_t &_) { c->depend_data->add_gsub_lookup (_.first, c->lookup_index, _.second); })
    ;
  }

  ಶೂನ್ಯ closure (hb_closure_context_t *c) ಧ್ರುವ
  {
    auto &cov = ಏತತ್+coverage;
    auto &glyph_set = c->parent_active_glyphs ();

    ಯದಿ (substitute.len > glyph_set.get_population () * 4)
    {
      ಪ್ರತಿ (auto g : glyph_set)
      {
	unsigned i = cov.get_coverage (g);
	ಯದಿ (i == NOT_COVERED || i >= substitute.len)
	  ಅನುವರ್ತ;
	c->output->add (substitute.arrayZ[i]);
      }

      ಪ್ರತ್ಯಾಗಮ;
    }

    + hb_zip (cov, substitute)
    | hb_filter (glyph_set, hb_first)
    | hb_map (hb_second)
    | hb_sink (c->output)
    ;
  }

  ಶೂನ್ಯ closure_lookups (hb_closure_lookups_context_t *c) ಧ್ರುವ {}

  ಶೂನ್ಯ collect_glyphs (hb_collect_glyphs_context_t *c) ಧ್ರುವ
  {
    ಯದಿ (unlikely (!(ಏತತ್+coverage).collect_coverage (c->ಒಳಹರಿವು))) ಪ್ರತ್ಯಾಗಮ;
    + hb_zip (ಏತತ್+coverage, substitute)
    | hb_map (hb_second)
    | hb_sink (c->output)
    ;
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
      glyph_id = substitute[index];

      *alternate_glyphs = glyph_id;
      *alternate_ಎಣಿಕೆ = 1;
    }

    ಪ್ರತ್ಯಾಗಮ 1;
  }

  ಶೂನ್ಯ
  collect_glyph_alternates (hb_map_t  *alternate_ಎಣಿಕೆ /* IN/OUT */,
			    hb_map_t  *alternate_glyphs /* IN/OUT */) ಧ್ರುವ
  {
    + hb_zip (ಏತತ್+coverage, substitute)
    | hb_apply ([&] (ಧ್ರುವ hb_pair_t<hb_codepoint_t, hb_codepoint_t> &p) -> ಶೂನ್ಯ
		{ _hb_collect_glyph_alternates_add (p.first, p.second,
						    alternate_ಎಣಿಕೆ, alternate_glyphs); })
    ;
  }

  ತಾರ್ಕಿಕ apply (hb_ot_apply_context_t *c) ಧ್ರುವ
  {
    TRACE_APPLY (ಏತತ್);
    unsigned ಪೂರ್ಣಾಙ್ಕ index = (ಏತತ್+coverage).get_coverage (c->buffer->cur().codepoint);
    ಯದಿ (index == NOT_COVERED) return_trace (ಅಸತ್ಯ);

    ಯದಿ (unlikely (index >= substitute.len)) return_trace (ಅಸತ್ಯ);

    ಯದಿ (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->sync_so_far ();
      c->buffer->message (c->font,
			  "replacing glyph at %u (single substitution)",
			  c->buffer->idx);
    }

    c->replace_glyph (substitute[index]);

    ಯದಿ (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->message (c->font,
			  "replaced glyph at %u (single substitution)",
			  c->buffer->idx - 1u);
    }

    return_trace (ಸತ್ಯ);
  }

  template<typename Iterator,
           hb_requires (hb_is_sorted_source_of (Iterator,
                                                hb_codepoint_pair_t))>
  ತಾರ್ಕಿಕ serialize (hb_serialize_context_t *c,
                  Iterator it)
  {
    TRACE_SERIALIZE (ಏತತ್);
    auto substitutes =
      + it
      | hb_map (hb_second)
      ;
    auto glyphs =
      + it
      | hb_map_retains_sorting (hb_first)
      ;
    ಯದಿ (unlikely (!c->extend_min (ಏತತ್))) return_trace (ಅಸತ್ಯ);
    ಯದಿ (unlikely (!substitute.serialize (c, substitutes))) return_trace (ಅಸತ್ಯ);
    ಯದಿ (unlikely (!coverage.serialize_serialize (c, glyphs))) return_trace (ಅಸತ್ಯ);
    return_trace (ಸತ್ಯ);
  }

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    ಧ್ರುವ hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ಧ್ರುವ hb_map_t &glyph_map = *c->plan->glyph_map;

    auto it =
    + hb_zip (ಏತತ್+coverage, substitute)
    | hb_filter (glyphset, hb_first)
    | hb_filter (glyphset, hb_second)
    | hb_map_retains_sorting ([&] (hb_pair_t<hb_codepoint_t, ಧ್ರುವ typename Types::HBGlyphID &> p) -> hb_codepoint_pair_t
                              { ಪ್ರತ್ಯಾಗಮ hb_pair (glyph_map[p.first], glyph_map[p.second]); })
    ;

    ತಾರ್ಕಿಕ ret = ತಾರ್ಕಿಕ (it);
    SingleSubst_serialize (c->serializer, it);
    return_trace (ret);
  }
};

}
}
}

#endif /* OT_LAYOUT_GSUB_SINGLESUBSTFORMAT2_HH */
