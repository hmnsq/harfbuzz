#ifndef OT_LAYOUT_GSUB_ALTERNATESET_HH
#define OT_LAYOUT_GSUB_ALTERNATESET_HH

#અન્તર્ભાવ "Common.hh"

નામાકાશ OT {
નામાકાશ Layout {
નામાકાશ GSUB_impl {

template <typename Types>
સંરચના AlternateSet
{
  સુરક્ષિત:
  Array16Of<typename Types::HBGlyphID>
                alternates;             /* Array of alternate GlyphIDs--in
                                         * arbitrary order */
  જાહેર:
  DEFINE_SIZE_ARRAY (2, alternates);

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (alternates.sanitize (c));
  }

  તાર્કિક intersects (અચલ hb_set_t *glyphs) અચલ
  { પાછા hb_any (alternates, glyphs); }

  શૂન્ય closure (hb_closure_context_t *c) અચલ
  { c->output->add_array (alternates.arrayZ, alternates.len); }

  શૂન્ય depend (hb_depend_context_t *c, hb_codepoint_t source) અચલ
  {
    + hb_iter (alternates)
    | hb_apply ([&] (અચલ hb_codepoint_t &target) { c->depend_data->add_gsub_lookup (source, c->lookup_index, target); })
    ;
  }

  શૂન્ય collect_glyphs (hb_collect_glyphs_context_t *c) અચલ
  { c->output->add_array (alternates.arrayZ, alternates.len); }

  તાર્કિક apply (hb_ot_apply_context_t *c) અચલ
  {
    TRACE_APPLY (આ);
    unsigned પૂર્ણાંક ગણતરી = alternates.len;

    જો (unlikely (!ગણતરી)) return_trace (ખોટું);

    hb_mask_t glyph_mask = c->buffer->cur().mask;
    hb_mask_t lookup_mask = c->lookup_mask;

    /* Note: This breaks badly if two features enabled this lookup together. */
    unsigned પૂર્ણાંક shift = hb_ctz (lookup_mask);
    unsigned પૂર્ણાંક alt_index = ((lookup_mask & glyph_mask) >> shift);

    /* If alt_index is MAX_VALUE, randomize feature if it is the rand feature. */
    જો (alt_index == HB_OT_MAP_MAX_કિંમત && c->random)
    {
      /* Maybe we can do better than unsafe-to-break all; but since we are
       * changing random state, it would be hard to track that.  Good 'nough. */
      c->buffer->unsafe_to_break (0, c->buffer->len);
      alt_index = c->random_સંખ્યા () % ગણતરી + 1;
    }

    જો (unlikely (alt_index > ગણતરી || alt_index == 0)) return_trace (ખોટું);

    જો (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->sync_so_far ();
      c->buffer->message (c->font,
			  "replacing glyph at %u (alternate substitution)",
			  c->buffer->idx);
    }

    c->replace_glyph (alternates[alt_index - 1]);

    જો (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->message (c->font,
			  "replaced glyph at %u (alternate substitution)",
			  c->buffer->idx - 1u);
    }

    return_trace (સાચું);
  }

  unsigned
  get_alternates (unsigned        start_offset,
                  unsigned       *alternate_ગણતરી  /* IN/OUT.  May be NULL. */,
                  hb_codepoint_t *alternate_glyphs /* OUT.     May be NULL. */) અચલ
  {
    જો (alternates.len && alternate_ગણતરી && alternate_glyphs)
    {
      + alternates.as_array ().sub_array (start_offset, alternate_ગણતરી)
      | hb_sink (hb_array (alternate_glyphs, *alternate_ગણતરી))
      ;
    }
    પાછા alternates.len;
  }

  શૂન્ય
  collect_alternates (hb_codepoint_t gid,
		      hb_map_t  *alternate_ગણતરી /* IN/OUT */,
		      hb_map_t  *alternate_glyphs /* IN/OUT */) અચલ
  {
    + hb_enumerate (alternates)
    | hb_map ([gid] (hb_pair_t<unsigned, hb_codepoint_t> _) { પાછા hb_pair (gid + (_.first << 24), _.second); })
    | hb_apply ([&] (અચલ hb_pair_t<hb_codepoint_t, hb_codepoint_t> &p) -> શૂન્ય
		{ _hb_collect_glyph_alternates_add (p.first, p.second,
						    alternate_ગણતરી, alternate_glyphs); })
    ;
  }

  template <typename Iterator,
            hb_requires (hb_is_source_of (Iterator, hb_codepoint_t))>
  તાર્કિક serialize (hb_serialize_context_t *c,
                  Iterator alts)
  {
    TRACE_SERIALIZE (આ);
    return_trace (alternates.serialize (c, alts));
  }

  તાર્કિક subset (hb_subset_context_t *c) અચલ
  {
    TRACE_SUBSET (આ);
    અચલ hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    અચલ hb_map_t &glyph_map = *c->plan->glyph_map;

    auto it =
      + hb_iter (alternates)
      | hb_filter (glyphset)
      | hb_map (glyph_map)
      ;

    auto *out = c->serializer->start_embed (*આ);
    return_trace (out->serialize (c->serializer, it) &&
                  out->alternates);
  }
};

}
}
}


#endif /* OT_LAYOUT_GSUB_ALTERNATESET_HH */
