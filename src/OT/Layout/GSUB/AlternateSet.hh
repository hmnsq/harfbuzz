#ifndef OT_LAYOUT_GSUB_ALTERNATESET_HH
#define OT_LAYOUT_GSUB_ALTERNATESET_HH

#ଅନ୍ତର୍ଭାଵ "Common.hh"

ନାମାକାଶ OT {
ନାମାକାଶ Layout {
ନାମାକାଶ GSUB_impl {

template <typename Types>
ସଂରଚନା AlternateSet
{
  ସୁରକ୍ଷିତ:
  Array16Of<typename Types::HBGlyphID>
                alternates;             /* Array of alternate GlyphIDs--in
                                         * arbitrary order */
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_ARRAY (2, alternates);

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (alternates.sanitize (c));
  }

  ତାର୍କିକ intersects (ଅଚଳ hb_set_t *glyphs) ଅଚଳ
  { ଫେରନ୍ତୁ hb_any (alternates, glyphs); }

  ଶୂନ୍ୟ closure (hb_closure_context_t *c) ଅଚଳ
  { c->output->add_array (alternates.arrayZ, alternates.len); }

  ଶୂନ୍ୟ depend (hb_depend_context_t *c, hb_codepoint_t source) ଅଚଳ
  {
    + hb_iter (alternates)
    | hb_apply ([&] (ଅଚଳ hb_codepoint_t &target) { c->depend_data->add_gsub_lookup (source, c->lookup_index, target); })
    ;
  }

  ଶୂନ୍ୟ collect_glyphs (hb_collect_glyphs_context_t *c) ଅଚଳ
  { c->output->add_array (alternates.arrayZ, alternates.len); }

  ତାର୍କିକ apply (hb_ot_apply_context_t *c) ଅଚଳ
  {
    TRACE_APPLY (ଏହି);
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = alternates.len;

    ଯଦି (unlikely (!ଗଣନା_2)) return_trace (ମିଥ୍ୟା);

    hb_mask_t glyph_mask = c->buffer->cur().mask;
    hb_mask_t lookup_mask = c->lookup_mask;

    /* Note: This breaks badly if two features enabled this lookup together. */
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା shift = hb_ctz (lookup_mask);
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା alt_index = ((lookup_mask & glyph_mask) >> shift);

    /* If alt_index is MAX_VALUE, randomize feature if it is the rand feature. */
    ଯଦି (alt_index == HB_OT_MAP_MAX_ମୂଲ୍ୟ && c->random)
    {
      /* Maybe we can do better than unsafe-to-break all; but since we are
       * changing random state, it would be hard to track that.  Good 'nough. */
      c->buffer->unsafe_to_break (0, c->buffer->len);
      alt_index = c->random_ସଂଖ୍ୟା () % ଗଣନା_2 + 1;
    }

    ଯଦି (unlikely (alt_index > ଗଣନା_2 || alt_index == 0)) return_trace (ମିଥ୍ୟା);

    ଯଦି (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->sync_so_far ();
      c->buffer->message (c->font,
			  "replacing glyph at %u (alternate substitution)",
			  c->buffer->idx);
    }

    c->replace_glyph (alternates[alt_index - 1]);

    ଯଦି (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->message (c->font,
			  "replaced glyph at %u (alternate substitution)",
			  c->buffer->idx - 1u);
    }

    return_trace (ସତ୍ୟ);
  }

  unsigned
  get_alternates (unsigned        start_offset,
                  unsigned       *alternate_ଗଣନା  /* IN/OUT.  May be NULL. */,
                  hb_codepoint_t *alternate_glyphs /* OUT.     May be NULL. */) ଅଚଳ
  {
    ଯଦି (alternates.len && alternate_ଗଣନା && alternate_glyphs)
    {
      + alternates.as_array ().sub_array (start_offset, alternate_ଗଣନା)
      | hb_sink (hb_array (alternate_glyphs, *alternate_ଗଣନା))
      ;
    }
    ଫେରନ୍ତୁ alternates.len;
  }

  ଶୂନ୍ୟ
  collect_alternates (hb_codepoint_t gid,
		      hb_map_t  *alternate_ଗଣନା /* IN/OUT */,
		      hb_map_t  *alternate_glyphs /* IN/OUT */) ଅଚଳ
  {
    + hb_enumerate (alternates)
    | hb_map ([gid] (hb_pair_t<unsigned, hb_codepoint_t> _) { ଫେରନ୍ତୁ hb_pair (gid + (_.first << 24), _.second); })
    | hb_apply ([&] (ଅଚଳ hb_pair_t<hb_codepoint_t, hb_codepoint_t> &p) -> ଶୂନ୍ୟ
		{ _hb_collect_glyph_alternates_add (p.first, p.second,
						    alternate_ଗଣନା, alternate_glyphs); })
    ;
  }

  template <typename Iterator,
            hb_requires (hb_is_source_of (Iterator, hb_codepoint_t))>
  ତାର୍କିକ serialize (hb_serialize_context_t *c,
                  Iterator alts)
  {
    TRACE_SERIALIZE (ଏହି);
    return_trace (alternates.serialize (c, alts));
  }

  ତାର୍କିକ subset (hb_subset_context_t *c) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    ଅଚଳ hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ଅଚଳ hb_map_t &glyph_map = *c->plan->glyph_map;

    auto it =
      + hb_iter (alternates)
      | hb_filter (glyphset)
      | hb_map (glyph_map)
      ;

    auto *out = c->serializer->start_embed (*ଏହି);
    return_trace (out->serialize (c->serializer, it) &&
                  out->alternates);
  }
};

}
}
}


#endif /* OT_LAYOUT_GSUB_ALTERNATESET_HH */
