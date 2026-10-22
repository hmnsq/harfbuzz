#ifndef OT_LAYOUT_GSUB_SINGLESUBSTFORMAT2_HH
#define OT_LAYOUT_GSUB_SINGLESUBSTFORMAT2_HH

#અન્તર્ભાવ "Common.hh"

નામાકાશ OT {
નામાકાશ Layout {
નામાકાશ GSUB_impl {

template <typename Types>
સંરચના SingleSubstFormat2_4
{
  સુરક્ષિત:
  HBUINT16      format;                 /* Format identifier--format = 2 */
  typename Types::template LOffsetTo<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of Substitution table */
  typename Types::template ArrayOf<typename Types::HBGlyphID>
                substitute;             /* Array of substitute
                                         * GlyphIDs--ordered by Coverage Index */

  જાહેર:
  DEFINE_SIZE_ARRAY (2 + Types::LOffset::static_size + Types::HBUINT::static_size, substitute);

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (coverage.sanitize (c, આ) && substitute.sanitize (c));
  }

  તાર્કિક intersects (અચલ hb_set_t *glyphs) અચલ
  { પાછા (આ+coverage).intersects (glyphs); }

  તાર્કિક may_have_non_1to1 () અચલ
  { પાછા ખોટું; }

  શૂન્ય depend (hb_depend_context_t *c) અચલ
  {
    auto &cov = આ+coverage;
    auto &glyph_set = c->parent_active_glyphs ();

    // Filter by active glyphs like closure does
    જો (substitute.len > glyph_set.get_population () * 4)
    {
      માટે (auto g : glyph_set)
      {
	unsigned i = cov.get_coverage (g);
	જો (i == NOT_COVERED || i >= substitute.len)
	  ચાલુ;
	c->depend_data->add_gsub_lookup (g, c->lookup_index, substitute.arrayZ[i]);
      }

      પાછા;
    }

    + hb_zip (cov, substitute)
    | hb_filter (glyph_set, hb_first)
    | hb_apply ([&] (અચલ hb_codepoint_pair_t &_) { c->depend_data->add_gsub_lookup (_.first, c->lookup_index, _.second); })
    ;
  }

  શૂન્ય closure (hb_closure_context_t *c) અચલ
  {
    auto &cov = આ+coverage;
    auto &glyph_set = c->parent_active_glyphs ();

    જો (substitute.len > glyph_set.get_population () * 4)
    {
      માટે (auto g : glyph_set)
      {
	unsigned i = cov.get_coverage (g);
	જો (i == NOT_COVERED || i >= substitute.len)
	  ચાલુ;
	c->output->add (substitute.arrayZ[i]);
      }

      પાછા;
    }

    + hb_zip (cov, substitute)
    | hb_filter (glyph_set, hb_first)
    | hb_map (hb_second)
    | hb_sink (c->output)
    ;
  }

  શૂન્ય closure_lookups (hb_closure_lookups_context_t *c) અચલ {}

  શૂન્ય collect_glyphs (hb_collect_glyphs_context_t *c) અચલ
  {
    જો (unlikely (!(આ+coverage).collect_coverage (c->આગત))) પાછા;
    + hb_zip (આ+coverage, substitute)
    | hb_map (hb_second)
    | hb_sink (c->output)
    ;
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
      glyph_id = substitute[index];

      *alternate_glyphs = glyph_id;
      *alternate_ગણતરી = 1;
    }

    પાછા 1;
  }

  શૂન્ય
  collect_glyph_alternates (hb_map_t  *alternate_ગણતરી /* IN/OUT */,
			    hb_map_t  *alternate_glyphs /* IN/OUT */) અચલ
  {
    + hb_zip (આ+coverage, substitute)
    | hb_apply ([&] (અચલ hb_pair_t<hb_codepoint_t, hb_codepoint_t> &p) -> શૂન્ય
		{ _hb_collect_glyph_alternates_add (p.first, p.second,
						    alternate_ગણતરી, alternate_glyphs); })
    ;
  }

  તાર્કિક apply (hb_ot_apply_context_t *c) અચલ
  {
    TRACE_APPLY (આ);
    unsigned પૂર્ણાંક index = (આ+coverage).get_coverage (c->buffer->cur().codepoint);
    જો (index == NOT_COVERED) return_trace (ખોટું);

    જો (unlikely (index >= substitute.len)) return_trace (ખોટું);

    જો (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->sync_so_far ();
      c->buffer->message (c->font,
			  "replacing glyph at %u (single substitution)",
			  c->buffer->idx);
    }

    c->replace_glyph (substitute[index]);

    જો (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->message (c->font,
			  "replaced glyph at %u (single substitution)",
			  c->buffer->idx - 1u);
    }

    return_trace (સાચું);
  }

  template<typename Iterator,
           hb_requires (hb_is_sorted_source_of (Iterator,
                                                hb_codepoint_pair_t))>
  તાર્કિક serialize (hb_serialize_context_t *c,
                  Iterator it)
  {
    TRACE_SERIALIZE (આ);
    auto substitutes =
      + it
      | hb_map (hb_second)
      ;
    auto glyphs =
      + it
      | hb_map_retains_sorting (hb_first)
      ;
    જો (unlikely (!c->extend_min (આ))) return_trace (ખોટું);
    જો (unlikely (!substitute.serialize (c, substitutes))) return_trace (ખોટું);
    જો (unlikely (!coverage.serialize_serialize (c, glyphs))) return_trace (ખોટું);
    return_trace (સાચું);
  }

  તાર્કિક subset (hb_subset_context_t *c) અચલ
  {
    TRACE_SUBSET (આ);
    અચલ hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    અચલ hb_map_t &glyph_map = *c->plan->glyph_map;

    auto it =
    + hb_zip (આ+coverage, substitute)
    | hb_filter (glyphset, hb_first)
    | hb_filter (glyphset, hb_second)
    | hb_map_retains_sorting ([&] (hb_pair_t<hb_codepoint_t, અચલ typename Types::HBGlyphID &> p) -> hb_codepoint_pair_t
                              { પાછા hb_pair (glyph_map[p.first], glyph_map[p.second]); })
    ;

    તાર્કિક ret = તાર્કિક (it);
    SingleSubst_serialize (c->serializer, it);
    return_trace (ret);
  }
};

}
}
}

#endif /* OT_LAYOUT_GSUB_SINGLESUBSTFORMAT2_HH */
