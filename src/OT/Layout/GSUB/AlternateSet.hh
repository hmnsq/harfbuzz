#ifndef OT_LAYOUT_GSUB_ALTERNATESET_HH
#define OT_LAYOUT_GSUB_ALTERNATESET_HH

#অন্তর্ভাব "Common.hh"

নামাকাশ OT {
নামাকাশ Layout {
নামাকাশ GSUB_impl {

template <typename Types>
সংরচনা AlternateSet
{
  রক্ষিত:
  Array16Of<typename Types::HBGlyphID>
                alternates;             /* Array of alternate GlyphIDs--in
                                         * arbitrary order */
  সার্বজনিক:
  DEFINE_SIZE_ARRAY (2, alternates);

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    return_trace (alternates.sanitize (c));
  }

  তার্কিক intersects (ধ্রুব hb_set_t *glyphs) ধ্রুব
  { প্রত্যাগম hb_any (alternates, glyphs); }

  শূন্য closure (hb_closure_context_t *c) ধ্রুব
  { c->output->add_array (alternates.arrayZ, alternates.len); }

  শূন্য depend (hb_depend_context_t *c, hb_codepoint_t source) ধ্রুব
  {
    + hb_iter (alternates)
    | hb_apply ([&] (ধ্রুব hb_codepoint_t &target) { c->depend_data->add_gsub_lookup (source, c->lookup_index, target); })
    ;
  }

  শূন্য collect_glyphs (hb_collect_glyphs_context_t *c) ধ্রুব
  { c->output->add_array (alternates.arrayZ, alternates.len); }

  তার্কিক apply (hb_ot_apply_context_t *c) ধ্রুব
  {
    TRACE_APPLY (এতত্);
    unsigned পূর্ণাঙ্ক গুনতি = alternates.len;

    যদি (unlikely (!গুনতি)) return_trace (অসত্য);

    hb_mask_t glyph_mask = c->buffer->cur().mask;
    hb_mask_t lookup_mask = c->lookup_mask;

    /* Note: This breaks badly if two features enabled this lookup together. */
    unsigned পূর্ণাঙ্ক shift = hb_ctz (lookup_mask);
    unsigned পূর্ণাঙ্ক alt_index = ((lookup_mask & glyph_mask) >> shift);

    /* If alt_index is MAX_VALUE, randomize feature if it is the rand feature. */
    যদি (alt_index == HB_OT_MAP_MAX_মান && c->random)
    {
      /* Maybe we can do better than unsafe-to-break all; but since we are
       * changing random state, it would be hard to track that.  Good 'nough. */
      c->buffer->unsafe_to_break (0, c->buffer->len);
      alt_index = c->random_সংখ্যা () % গুনতি + 1;
    }

    যদি (unlikely (alt_index > গুনতি || alt_index == 0)) return_trace (অসত্য);

    যদি (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->sync_so_far ();
      c->buffer->message (c->font,
			  "replacing glyph at %u (alternate substitution)",
			  c->buffer->idx);
    }

    c->replace_glyph (alternates[alt_index - 1]);

    যদি (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->message (c->font,
			  "replaced glyph at %u (alternate substitution)",
			  c->buffer->idx - 1u);
    }

    return_trace (সত্য);
  }

  unsigned
  get_alternates (unsigned        start_offset,
                  unsigned       *alternate_গুনতি  /* IN/OUT.  May be NULL. */,
                  hb_codepoint_t *alternate_glyphs /* OUT.     May be NULL. */) ধ্রুব
  {
    যদি (alternates.len && alternate_গুনতি && alternate_glyphs)
    {
      + alternates.as_array ().sub_array (start_offset, alternate_গুনতি)
      | hb_sink (hb_array (alternate_glyphs, *alternate_গুনতি))
      ;
    }
    প্রত্যাগম alternates.len;
  }

  শূন্য
  collect_alternates (hb_codepoint_t gid,
		      hb_map_t  *alternate_গুনতি /* IN/OUT */,
		      hb_map_t  *alternate_glyphs /* IN/OUT */) ধ্রুব
  {
    + hb_enumerate (alternates)
    | hb_map ([gid] (hb_pair_t<unsigned, hb_codepoint_t> _) { প্রত্যাগম hb_pair (gid + (_.first << 24), _.second); })
    | hb_apply ([&] (ধ্রুব hb_pair_t<hb_codepoint_t, hb_codepoint_t> &p) -> শূন্য
		{ _hb_collect_glyph_alternates_add (p.first, p.second,
						    alternate_গুনতি, alternate_glyphs); })
    ;
  }

  template <typename Iterator,
            hb_requires (hb_is_source_of (Iterator, hb_codepoint_t))>
  তার্কিক serialize (hb_serialize_context_t *c,
                  Iterator alts)
  {
    TRACE_SERIALIZE (এতত্);
    return_trace (alternates.serialize (c, alts));
  }

  তার্কিক subset (hb_subset_context_t *c) ধ্রুব
  {
    TRACE_SUBSET (এতত্);
    ধ্রুব hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ধ্রুব hb_map_t &glyph_map = *c->plan->glyph_map;

    auto it =
      + hb_iter (alternates)
      | hb_filter (glyphset)
      | hb_map (glyph_map)
      ;

    auto *out = c->serializer->start_embed (*এতত্);
    return_trace (out->serialize (c->serializer, it) &&
                  out->alternates);
  }
};

}
}
}


#endif /* OT_LAYOUT_GSUB_ALTERNATESET_HH */
