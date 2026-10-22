#ifndef OT_LAYOUT_GSUB_ALTERNATESET_HH
#define OT_LAYOUT_GSUB_ALTERNATESET_HH

#അന്തര്ഭാവ "Common.hh"

നാമാകാശ OT {
നാമാകാശ Layout {
നാമാകാശ GSUB_impl {

template <typename Types>
ഘടന AlternateSet
{
  സംരക്ഷിത:
  Array16Of<typename Types::HBGlyphID>
                alternates;             /* Array of alternate GlyphIDs--in
                                         * arbitrary order */
  പൊതു:
  DEFINE_SIZE_ARRAY (2, alternates);

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (alternates.sanitize (c));
  }

  തർക്കം intersects (സ്ഥിരം hb_set_t *glyphs) സ്ഥിരം
  { തിരികെ hb_any (alternates, glyphs); }

  ശൂന്യം closure (hb_closure_context_t *c) സ്ഥിരം
  { c->output->add_array (alternates.arrayZ, alternates.len); }

  ശൂന്യം depend (hb_depend_context_t *c, hb_codepoint_t source) സ്ഥിരം
  {
    + hb_iter (alternates)
    | hb_apply ([&] (സ്ഥിരം hb_codepoint_t &target) { c->depend_data->add_gsub_lookup (source, c->lookup_index, target); })
    ;
  }

  ശൂന്യം collect_glyphs (hb_collect_glyphs_context_t *c) സ്ഥിരം
  { c->output->add_array (alternates.arrayZ, alternates.len); }

  തർക്കം apply (hb_ot_apply_context_t *c) സ്ഥിരം
  {
    TRACE_APPLY (ഇത്);
    unsigned പൂർണ്ണസംഖ്യ എണ്ണം = alternates.len;

    എങ്കിൽ (unlikely (!എണ്ണം)) return_trace (അസത്യം);

    hb_mask_t glyph_mask = c->buffer->cur().mask;
    hb_mask_t lookup_mask = c->lookup_mask;

    /* Note: This breaks badly if two features enabled this lookup together. */
    unsigned പൂർണ്ണസംഖ്യ shift = hb_ctz (lookup_mask);
    unsigned പൂർണ്ണസംഖ്യ alt_index = ((lookup_mask & glyph_mask) >> shift);

    /* If alt_index is MAX_VALUE, randomize feature if it is the rand feature. */
    എങ്കിൽ (alt_index == HB_OT_MAP_MAX_മൂല്യം && c->random)
    {
      /* Maybe we can do better than unsafe-to-break all; but since we are
       * changing random state, it would be hard to track that.  Good 'nough. */
      c->buffer->unsafe_to_break (0, c->buffer->len);
      alt_index = c->random_സംഖ്യ () % എണ്ണം + 1;
    }

    എങ്കിൽ (unlikely (alt_index > എണ്ണം || alt_index == 0)) return_trace (അസത്യം);

    എങ്കിൽ (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->sync_so_far ();
      c->buffer->message (c->font,
			  "replacing glyph at %u (alternate substitution)",
			  c->buffer->idx);
    }

    c->replace_glyph (alternates[alt_index - 1]);

    എങ്കിൽ (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->message (c->font,
			  "replaced glyph at %u (alternate substitution)",
			  c->buffer->idx - 1u);
    }

    return_trace (സത്യം);
  }

  unsigned
  get_alternates (unsigned        start_offset,
                  unsigned       *alternate_എണ്ണം  /* IN/OUT.  May be NULL. */,
                  hb_codepoint_t *alternate_glyphs /* OUT.     May be NULL. */) സ്ഥിരം
  {
    എങ്കിൽ (alternates.len && alternate_എണ്ണം && alternate_glyphs)
    {
      + alternates.as_array ().sub_array (start_offset, alternate_എണ്ണം)
      | hb_sink (hb_array (alternate_glyphs, *alternate_എണ്ണം))
      ;
    }
    തിരികെ alternates.len;
  }

  ശൂന്യം
  collect_alternates (hb_codepoint_t gid,
		      hb_map_t  *alternate_എണ്ണം /* IN/OUT */,
		      hb_map_t  *alternate_glyphs /* IN/OUT */) സ്ഥിരം
  {
    + hb_enumerate (alternates)
    | hb_map ([gid] (hb_pair_t<unsigned, hb_codepoint_t> _) { തിരികെ hb_pair (gid + (_.first << 24), _.second); })
    | hb_apply ([&] (സ്ഥിരം hb_pair_t<hb_codepoint_t, hb_codepoint_t> &p) -> ശൂന്യം
		{ _hb_collect_glyph_alternates_add (p.first, p.second,
						    alternate_എണ്ണം, alternate_glyphs); })
    ;
  }

  template <typename Iterator,
            hb_requires (hb_is_source_of (Iterator, hb_codepoint_t))>
  തർക്കം serialize (hb_serialize_context_t *c,
                  Iterator alts)
  {
    TRACE_SERIALIZE (ഇത്);
    return_trace (alternates.serialize (c, alts));
  }

  തർക്കം subset (hb_subset_context_t *c) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);
    സ്ഥിരം hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    സ്ഥിരം hb_map_t &glyph_map = *c->plan->glyph_map;

    auto it =
      + hb_iter (alternates)
      | hb_filter (glyphset)
      | hb_map (glyph_map)
      ;

    auto *out = c->serializer->start_embed (*ഇത്);
    return_trace (out->serialize (c->serializer, it) &&
                  out->alternates);
  }
};

}
}
}


#endif /* OT_LAYOUT_GSUB_ALTERNATESET_HH */
