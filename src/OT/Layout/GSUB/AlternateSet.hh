#ifndef OT_LAYOUT_GSUB_ALTERNATESET_HH
#define OT_LAYOUT_GSUB_ALTERNATESET_HH

#ಅನ್ತರ್ಭಾವ "Common.hh"

ನಾಮಾಕಾಶ OT {
ನಾಮಾಕಾಶ Layout {
ನಾಮಾಕಾಶ GSUB_impl {

template <typename Types>
ಸಂರಚನಾ AlternateSet
{
  ರಕ್ಷಿತ:
  Array16Of<typename Types::HBGlyphID>
                alternates;             /* Array of alternate GlyphIDs--in
                                         * arbitrary order */
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_ARRAY (2, alternates);

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (alternates.sanitize (c));
  }

  ತಾರ್ಕಿಕ intersects (ಧ್ರುವ hb_set_t *glyphs) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ hb_any (alternates, glyphs); }

  ಶೂನ್ಯ closure (hb_closure_context_t *c) ಧ್ರುವ
  { c->output->add_array (alternates.arrayZ, alternates.len); }

  ಶೂನ್ಯ depend (hb_depend_context_t *c, hb_codepoint_t source) ಧ್ರುವ
  {
    + hb_iter (alternates)
    | hb_apply ([&] (ಧ್ರುವ hb_codepoint_t &target) { c->depend_data->add_gsub_lookup (source, c->lookup_index, target); })
    ;
  }

  ಶೂನ್ಯ collect_glyphs (hb_collect_glyphs_context_t *c) ಧ್ರುವ
  { c->output->add_array (alternates.arrayZ, alternates.len); }

  ತಾರ್ಕಿಕ apply (hb_ot_apply_context_t *c) ಧ್ರುವ
  {
    TRACE_APPLY (ಏತತ್);
    unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = alternates.len;

    ಯದಿ (unlikely (!ಎಣಿಕೆ)) return_trace (ಅಸತ್ಯ);

    hb_mask_t glyph_mask = c->buffer->cur().mask;
    hb_mask_t lookup_mask = c->lookup_mask;

    /* Note: This breaks badly if two features enabled this lookup together. */
    unsigned ಪೂರ್ಣಾಙ್ಕ shift = hb_ctz (lookup_mask);
    unsigned ಪೂರ್ಣಾಙ್ಕ alt_index = ((lookup_mask & glyph_mask) >> shift);

    /* If alt_index is MAX_VALUE, randomize feature if it is the rand feature. */
    ಯದಿ (alt_index == HB_OT_MAP_MAX_ಮೌಲ್ಯ && c->random)
    {
      /* Maybe we can do better than unsafe-to-break all; but since we are
       * changing random state, it would be hard to track that.  Good 'nough. */
      c->buffer->unsafe_to_break (0, c->buffer->len);
      alt_index = c->random_ಸಂಖ್ಯೆ () % ಎಣಿಕೆ + 1;
    }

    ಯದಿ (unlikely (alt_index > ಎಣಿಕೆ || alt_index == 0)) return_trace (ಅಸತ್ಯ);

    ಯದಿ (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->sync_so_far ();
      c->buffer->message (c->font,
			  "replacing glyph at %u (alternate substitution)",
			  c->buffer->idx);
    }

    c->replace_glyph (alternates[alt_index - 1]);

    ಯದಿ (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->message (c->font,
			  "replaced glyph at %u (alternate substitution)",
			  c->buffer->idx - 1u);
    }

    return_trace (ಸತ್ಯ);
  }

  unsigned
  get_alternates (unsigned        start_offset,
                  unsigned       *alternate_ಎಣಿಕೆ  /* IN/OUT.  May be NULL. */,
                  hb_codepoint_t *alternate_glyphs /* OUT.     May be NULL. */) ಧ್ರುವ
  {
    ಯದಿ (alternates.len && alternate_ಎಣಿಕೆ && alternate_glyphs)
    {
      + alternates.as_array ().sub_array (start_offset, alternate_ಎಣಿಕೆ)
      | hb_sink (hb_array (alternate_glyphs, *alternate_ಎಣಿಕೆ))
      ;
    }
    ಪ್ರತ್ಯಾಗಮ alternates.len;
  }

  ಶೂನ್ಯ
  collect_alternates (hb_codepoint_t gid,
		      hb_map_t  *alternate_ಎಣಿಕೆ /* IN/OUT */,
		      hb_map_t  *alternate_glyphs /* IN/OUT */) ಧ್ರುವ
  {
    + hb_enumerate (alternates)
    | hb_map ([gid] (hb_pair_t<unsigned, hb_codepoint_t> _) { ಪ್ರತ್ಯಾಗಮ hb_pair (gid + (_.first << 24), _.second); })
    | hb_apply ([&] (ಧ್ರುವ hb_pair_t<hb_codepoint_t, hb_codepoint_t> &p) -> ಶೂನ್ಯ
		{ _hb_collect_glyph_alternates_add (p.first, p.second,
						    alternate_ಎಣಿಕೆ, alternate_glyphs); })
    ;
  }

  template <typename Iterator,
            hb_requires (hb_is_source_of (Iterator, hb_codepoint_t))>
  ತಾರ್ಕಿಕ serialize (hb_serialize_context_t *c,
                  Iterator alts)
  {
    TRACE_SERIALIZE (ಏತತ್);
    return_trace (alternates.serialize (c, alts));
  }

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    ಧ್ರುವ hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ಧ್ರುವ hb_map_t &glyph_map = *c->plan->glyph_map;

    auto it =
      + hb_iter (alternates)
      | hb_filter (glyphset)
      | hb_map (glyph_map)
      ;

    auto *out = c->serializer->start_embed (*ಏತತ್);
    return_trace (out->serialize (c->serializer, it) &&
                  out->alternates);
  }
};

}
}
}


#endif /* OT_LAYOUT_GSUB_ALTERNATESET_HH */
