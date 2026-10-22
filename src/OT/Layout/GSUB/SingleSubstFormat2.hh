#ifndef OT_LAYOUT_GSUB_SINGLESUBSTFORMAT2_HH
#define OT_LAYOUT_GSUB_SINGLESUBSTFORMAT2_HH

#അന്തര്ഭാവ "Common.hh"

നാമാകാശ OT {
നാമാകാശ Layout {
നാമാകാശ GSUB_impl {

template <typename Types>
ഘടന SingleSubstFormat2_4
{
  സംരക്ഷിത:
  HBUINT16      format;                 /* Format identifier--format = 2 */
  typename Types::template LOffsetTo<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of Substitution table */
  typename Types::template ArrayOf<typename Types::HBGlyphID>
                substitute;             /* Array of substitute
                                         * GlyphIDs--ordered by Coverage Index */

  പൊതു:
  DEFINE_SIZE_ARRAY (2 + Types::LOffset::static_size + Types::HBUINT::static_size, substitute);

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (coverage.sanitize (c, ഇത്) && substitute.sanitize (c));
  }

  തർക്കം intersects (സ്ഥിരം hb_set_t *glyphs) സ്ഥിരം
  { തിരികെ (ഇത്+coverage).intersects (glyphs); }

  തർക്കം may_have_non_1to1 () സ്ഥിരം
  { തിരികെ അസത്യം; }

  ശൂന്യം depend (hb_depend_context_t *c) സ്ഥിരം
  {
    auto &cov = ഇത്+coverage;
    auto &glyph_set = c->parent_active_glyphs ();

    // Filter by active glyphs like closure does
    എങ്കിൽ (substitute.len > glyph_set.get_population () * 4)
    {
      വേണ്ടി (auto g : glyph_set)
      {
	unsigned i = cov.get_coverage (g);
	എങ്കിൽ (i == NOT_COVERED || i >= substitute.len)
	  തുടരുക;
	c->depend_data->add_gsub_lookup (g, c->lookup_index, substitute.arrayZ[i]);
      }

      തിരികെ;
    }

    + hb_zip (cov, substitute)
    | hb_filter (glyph_set, hb_first)
    | hb_apply ([&] (സ്ഥിരം hb_codepoint_pair_t &_) { c->depend_data->add_gsub_lookup (_.first, c->lookup_index, _.second); })
    ;
  }

  ശൂന്യം closure (hb_closure_context_t *c) സ്ഥിരം
  {
    auto &cov = ഇത്+coverage;
    auto &glyph_set = c->parent_active_glyphs ();

    എങ്കിൽ (substitute.len > glyph_set.get_population () * 4)
    {
      വേണ്ടി (auto g : glyph_set)
      {
	unsigned i = cov.get_coverage (g);
	എങ്കിൽ (i == NOT_COVERED || i >= substitute.len)
	  തുടരുക;
	c->output->add (substitute.arrayZ[i]);
      }

      തിരികെ;
    }

    + hb_zip (cov, substitute)
    | hb_filter (glyph_set, hb_first)
    | hb_map (hb_second)
    | hb_sink (c->output)
    ;
  }

  ശൂന്യം closure_lookups (hb_closure_lookups_context_t *c) സ്ഥിരം {}

  ശൂന്യം collect_glyphs (hb_collect_glyphs_context_t *c) സ്ഥിരം
  {
    എങ്കിൽ (unlikely (!(ഇത്+coverage).collect_coverage (c->നിവേശം))) തിരികെ;
    + hb_zip (ഇത്+coverage, substitute)
    | hb_map (hb_second)
    | hb_sink (c->output)
    ;
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
      glyph_id = substitute[index];

      *alternate_glyphs = glyph_id;
      *alternate_എണ്ണം = 1;
    }

    തിരികെ 1;
  }

  ശൂന്യം
  collect_glyph_alternates (hb_map_t  *alternate_എണ്ണം /* IN/OUT */,
			    hb_map_t  *alternate_glyphs /* IN/OUT */) സ്ഥിരം
  {
    + hb_zip (ഇത്+coverage, substitute)
    | hb_apply ([&] (സ്ഥിരം hb_pair_t<hb_codepoint_t, hb_codepoint_t> &p) -> ശൂന്യം
		{ _hb_collect_glyph_alternates_add (p.first, p.second,
						    alternate_എണ്ണം, alternate_glyphs); })
    ;
  }

  തർക്കം apply (hb_ot_apply_context_t *c) സ്ഥിരം
  {
    TRACE_APPLY (ഇത്);
    unsigned പൂർണ്ണസംഖ്യ index = (ഇത്+coverage).get_coverage (c->buffer->cur().codepoint);
    എങ്കിൽ (index == NOT_COVERED) return_trace (അസത്യം);

    എങ്കിൽ (unlikely (index >= substitute.len)) return_trace (അസത്യം);

    എങ്കിൽ (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->sync_so_far ();
      c->buffer->message (c->font,
			  "replacing glyph at %u (single substitution)",
			  c->buffer->idx);
    }

    c->replace_glyph (substitute[index]);

    എങ്കിൽ (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->message (c->font,
			  "replaced glyph at %u (single substitution)",
			  c->buffer->idx - 1u);
    }

    return_trace (സത്യം);
  }

  template<typename Iterator,
           hb_requires (hb_is_sorted_source_of (Iterator,
                                                hb_codepoint_pair_t))>
  തർക്കം serialize (hb_serialize_context_t *c,
                  Iterator it)
  {
    TRACE_SERIALIZE (ഇത്);
    auto substitutes =
      + it
      | hb_map (hb_second)
      ;
    auto glyphs =
      + it
      | hb_map_retains_sorting (hb_first)
      ;
    എങ്കിൽ (unlikely (!c->extend_min (ഇത്))) return_trace (അസത്യം);
    എങ്കിൽ (unlikely (!substitute.serialize (c, substitutes))) return_trace (അസത്യം);
    എങ്കിൽ (unlikely (!coverage.serialize_serialize (c, glyphs))) return_trace (അസത്യം);
    return_trace (സത്യം);
  }

  തർക്കം subset (hb_subset_context_t *c) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);
    സ്ഥിരം hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    സ്ഥിരം hb_map_t &glyph_map = *c->plan->glyph_map;

    auto it =
    + hb_zip (ഇത്+coverage, substitute)
    | hb_filter (glyphset, hb_first)
    | hb_filter (glyphset, hb_second)
    | hb_map_retains_sorting ([&] (hb_pair_t<hb_codepoint_t, സ്ഥിരം typename Types::HBGlyphID &> p) -> hb_codepoint_pair_t
                              { തിരികെ hb_pair (glyph_map[p.first], glyph_map[p.second]); })
    ;

    തർക്കം ret = തർക്കം (it);
    SingleSubst_serialize (c->serializer, it);
    return_trace (ret);
  }
};

}
}
}

#endif /* OT_LAYOUT_GSUB_SINGLESUBSTFORMAT2_HH */
