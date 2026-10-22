#ifndef OT_LAYOUT_GSUB_ALTERNATESET_HH
#define OT_LAYOUT_GSUB_ALTERNATESET_HH

#अन्तर्भाव "Common.hh"

नामाकाश OT {
नामाकाश Layout {
नामाकाश GSUB_impl {

template <typename Types>
संरचना AlternateSet
{
  रक्षित:
  Array16Of<typename Types::HBGlyphID>
                alternates;             /* Array of alternate GlyphIDs--in
                                         * arbitrary order */
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (2, alternates);

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (alternates.sanitize (c));
  }

  तार्किक intersects (ध्रुव hb_set_t *glyphs) ध्रुव
  { प्रत्यागम hb_any (alternates, glyphs); }

  शून्य closure (hb_closure_context_t *c) ध्रुव
  { c->output->add_array (alternates.arrayZ, alternates.len); }

  शून्य depend (hb_depend_context_t *c, hb_codepoint_t source) ध्रुव
  {
    + hb_iter (alternates)
    | hb_apply ([&] (ध्रुव hb_codepoint_t &target) { c->depend_data->add_gsub_lookup (source, c->lookup_index, target); })
    ;
  }

  शून्य collect_glyphs (hb_collect_glyphs_context_t *c) ध्रुव
  { c->output->add_array (alternates.arrayZ, alternates.len); }

  तार्किक apply (hb_ot_apply_context_t *c) ध्रुव
  {
    TRACE_APPLY (एतत्);
    unsigned पूर्णाङ्क गणनम् = alternates.len;

    जइ (unlikely (!गणनम्)) return_trace (मिच्छ);

    hb_mask_t glyph_mask = c->buffer->cur().mask;
    hb_mask_t lookup_mask = c->lookup_mask;

    /* Note: This breaks badly if two features enabled this lookup together. */
    unsigned पूर्णाङ्क shift = hb_ctz (lookup_mask);
    unsigned पूर्णाङ्क alt_index = ((lookup_mask & glyph_mask) >> shift);

    /* If alt_index is MAX_VALUE, randomize feature if it is the rand feature. */
    जइ (alt_index == HB_OT_MAP_MAX_मानम् && c->random)
    {
      /* Maybe we can do better than unsafe-to-break all; but since we are
       * changing random state, it would be hard to track that.  Good 'nough. */
      c->buffer->unsafe_to_break (0, c->buffer->len);
      alt_index = c->random_सङ्ख्या () % गणनम् + 1;
    }

    जइ (unlikely (alt_index > गणनम् || alt_index == 0)) return_trace (मिच्छ);

    जइ (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->sync_so_far ();
      c->buffer->message (c->font,
			  "replacing glyph at %u (alternate substitution)",
			  c->buffer->idx);
    }

    c->replace_glyph (alternates[alt_index - 1]);

    जइ (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->message (c->font,
			  "replaced glyph at %u (alternate substitution)",
			  c->buffer->idx - 1u);
    }

    return_trace (सच्च);
  }

  unsigned
  get_alternates (unsigned        start_offset,
                  unsigned       *alternate_गणनम्  /* IN/OUT.  May be NULL. */,
                  hb_codepoint_t *alternate_glyphs /* OUT.     May be NULL. */) ध्रुव
  {
    जइ (alternates.len && alternate_गणनम् && alternate_glyphs)
    {
      + alternates.as_array ().sub_array (start_offset, alternate_गणनम्)
      | hb_sink (hb_array (alternate_glyphs, *alternate_गणनम्))
      ;
    }
    प्रत्यागम alternates.len;
  }

  शून्य
  collect_alternates (hb_codepoint_t gid,
		      hb_map_t  *alternate_गणनम् /* IN/OUT */,
		      hb_map_t  *alternate_glyphs /* IN/OUT */) ध्रुव
  {
    + hb_enumerate (alternates)
    | hb_map ([gid] (hb_pair_t<unsigned, hb_codepoint_t> _) { प्रत्यागम hb_pair (gid + (_.first << 24), _.second); })
    | hb_apply ([&] (ध्रुव hb_pair_t<hb_codepoint_t, hb_codepoint_t> &p) -> शून्य
		{ _hb_collect_glyph_alternates_add (p.first, p.second,
						    alternate_गणनम्, alternate_glyphs); })
    ;
  }

  template <typename Iterator,
            hb_requires (hb_is_source_of (Iterator, hb_codepoint_t))>
  तार्किक serialize (hb_serialize_context_t *c,
                  Iterator alts)
  {
    TRACE_SERIALIZE (एतत्);
    return_trace (alternates.serialize (c, alts));
  }

  तार्किक subset (hb_subset_context_t *c) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    ध्रुव hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ध्रुव hb_map_t &glyph_map = *c->plan->glyph_map;

    auto it =
      + hb_iter (alternates)
      | hb_filter (glyphset)
      | hb_map (glyph_map)
      ;

    auto *out = c->serializer->start_embed (*एतत्);
    return_trace (out->serialize (c->serializer, it) &&
                  out->alternates);
  }
};

}
}
}


#endif /* OT_LAYOUT_GSUB_ALTERNATESET_HH */
