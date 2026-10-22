#ifndef OT_LAYOUT_GSUB_ALTERNATESET_HH
#define OT_LAYOUT_GSUB_ALTERNATESET_HH

#அந்தர்பாவ "Common.hh"

நாமாகாஶ OT {
நாமாகாஶ Layout {
நாமாகாஶ GSUB_impl {

template <typename Types>
கட்டமைப்பு AlternateSet
{
  பாதுகாக்கப்பட்ட:
  Array16Of<typename Types::HBGlyphID>
                alternates;             /* Array of alternate GlyphIDs--in
                                         * arbitrary order */
  பொது:
  DEFINE_SIZE_ARRAY (2, alternates);

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (alternates.sanitize (c));
  }

  தருக்க intersects (மாறிலி hb_set_t *glyphs) மாறிலி
  { திரும்பு hb_any (alternates, glyphs); }

  வெறுமை closure (hb_closure_context_t *c) மாறிலி
  { c->output->add_array (alternates.arrayZ, alternates.len); }

  வெறுமை depend (hb_depend_context_t *c, hb_codepoint_t source) மாறிலி
  {
    + hb_iter (alternates)
    | hb_apply ([&] (மாறிலி hb_codepoint_t &target) { c->depend_data->add_gsub_lookup (source, c->lookup_index, target); })
    ;
  }

  வெறுமை collect_glyphs (hb_collect_glyphs_context_t *c) மாறிலி
  { c->output->add_array (alternates.arrayZ, alternates.len); }

  தருக்க apply (hb_ot_apply_context_t *c) மாறிலி
  {
    TRACE_APPLY (இது);
    unsigned முழு மொத்தம் = alternates.len;

    எனில் (unlikely (!மொத்தம்)) return_trace (பொய்);

    hb_mask_t glyph_mask = c->buffer->cur().mask;
    hb_mask_t lookup_mask = c->lookup_mask;

    /* Note: This breaks badly if two features enabled this lookup together. */
    unsigned முழு shift = hb_ctz (lookup_mask);
    unsigned முழு alt_index = ((lookup_mask & glyph_mask) >> shift);

    /* If alt_index is MAX_VALUE, randomize feature if it is the rand feature. */
    எனில் (alt_index == HB_OT_MAP_MAX_மதிப்பு && c->random)
    {
      /* Maybe we can do better than unsafe-to-break all; but since we are
       * changing random state, it would be hard to track that.  Good 'nough. */
      c->buffer->unsafe_to_break (0, c->buffer->len);
      alt_index = c->random_எண் () % மொத்தம் + 1;
    }

    எனில் (unlikely (alt_index > மொத்தம் || alt_index == 0)) return_trace (பொய்);

    எனில் (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->sync_so_far ();
      c->buffer->message (c->font,
			  "replacing glyph at %u (alternate substitution)",
			  c->buffer->idx);
    }

    c->replace_glyph (alternates[alt_index - 1]);

    எனில் (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->message (c->font,
			  "replaced glyph at %u (alternate substitution)",
			  c->buffer->idx - 1u);
    }

    return_trace (உண்மை);
  }

  unsigned
  get_alternates (unsigned        start_offset,
                  unsigned       *alternate_மொத்தம்  /* IN/OUT.  May be NULL. */,
                  hb_codepoint_t *alternate_glyphs /* OUT.     May be NULL. */) மாறிலி
  {
    எனில் (alternates.len && alternate_மொத்தம் && alternate_glyphs)
    {
      + alternates.as_array ().sub_array (start_offset, alternate_மொத்தம்)
      | hb_sink (hb_array (alternate_glyphs, *alternate_மொத்தம்))
      ;
    }
    திரும்பு alternates.len;
  }

  வெறுமை
  collect_alternates (hb_codepoint_t gid,
		      hb_map_t  *alternate_மொத்தம் /* IN/OUT */,
		      hb_map_t  *alternate_glyphs /* IN/OUT */) மாறிலி
  {
    + hb_enumerate (alternates)
    | hb_map ([gid] (hb_pair_t<unsigned, hb_codepoint_t> _) { திரும்பு hb_pair (gid + (_.first << 24), _.second); })
    | hb_apply ([&] (மாறிலி hb_pair_t<hb_codepoint_t, hb_codepoint_t> &p) -> வெறுமை
		{ _hb_collect_glyph_alternates_add (p.first, p.second,
						    alternate_மொத்தம், alternate_glyphs); })
    ;
  }

  template <typename Iterator,
            hb_requires (hb_is_source_of (Iterator, hb_codepoint_t))>
  தருக்க serialize (hb_serialize_context_t *c,
                  Iterator alts)
  {
    TRACE_SERIALIZE (இது);
    return_trace (alternates.serialize (c, alts));
  }

  தருக்க subset (hb_subset_context_t *c) மாறிலி
  {
    TRACE_SUBSET (இது);
    மாறிலி hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    மாறிலி hb_map_t &glyph_map = *c->plan->glyph_map;

    auto it =
      + hb_iter (alternates)
      | hb_filter (glyphset)
      | hb_map (glyph_map)
      ;

    auto *out = c->serializer->start_embed (*இது);
    return_trace (out->serialize (c->serializer, it) &&
                  out->alternates);
  }
};

}
}
}


#endif /* OT_LAYOUT_GSUB_ALTERNATESET_HH */
