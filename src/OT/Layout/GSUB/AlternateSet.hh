#ifndef OT_LAYOUT_GSUB_ALTERNATESET_HH
#define OT_LAYOUT_GSUB_ALTERNATESET_HH

#অন্তর্ভাব "Common.hh"

নামাকাশ OT {
নামাকাশ Layout {
নামাকাশ GSUB_impl {

template <typename Types>
কাঠামো AlternateSet
{
  সুরক্ষিত:
  Array16Of<typename Types::HBGlyphID>
                alternates;             /* Array of alternate GlyphIDs--in
                                         * arbitrary order */
  সর্বজনীন:
  DEFINE_আকার_ARRAY (2, alternates);

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (alternates.sanitize (c));
  }

  যৌক্তিক intersects (ধ্রুবক hb_set_t *glyphs) ধ্রুবক
  { ফেরত hb_any (alternates, glyphs); }

  শূন্য closure (hb_closure_context_t *c) ধ্রুবক
  { c->output->add_array (alternates.arrayZ, alternates.len); }

  শূন্য depend (hb_depend_context_t *c, hb_codepoint_t source) ধ্রুবক
  {
    + hb_iter (alternates)
    | hb_apply ([&] (ধ্রুবক hb_codepoint_t &target) { c->depend_তথ্য->add_gsub_lookup (source, c->lookup_সূচক, target); })
    ;
  }

  শূন্য collect_glyphs (hb_collect_glyphs_context_t *c) ধ্রুবক
  { c->output->add_array (alternates.arrayZ, alternates.len); }

  যৌক্তিক apply (hb_ot_apply_context_t *c) ধ্রুবক
  {
    TRACE_APPLY (এই);
    unsigned পূর্ণসংখ্যা গুনতি = alternates.len;

    যদি (unlikely (!গুনতি)) return_trace (মিথ্যা);

    hb_mask_t glyph_mask = c->বাফার->cur().mask;
    hb_mask_t lookup_mask = c->lookup_mask;

    /* Note: This breaks badly if two features enabled this lookup together. */
    unsigned পূর্ণসংখ্যা shift = hb_ctz (lookup_mask);
    unsigned পূর্ণসংখ্যা alt_সূচক = ((lookup_mask & glyph_mask) >> shift);

    /* If alt_index is MAX_VALUE, randomize feature if it is the rand feature. */
    যদি (alt_সূচক == HB_OT_MAP_MAX_মান && c->random)
    {
      /* Maybe we can do better than unsafe-to-break all; but since we are
       * changing random state, it would be hard to track that.  Good 'nough. */
      c->বাফার->unsafe_to_break (0, c->বাফার->len);
      alt_সূচক = c->random_সংখ্যা () % গুনতি + 1;
    }

    যদি (unlikely (alt_সূচক > গুনতি || alt_সূচক == 0)) return_trace (মিথ্যা);

    যদি (HB_বাফার_MESSAGE_MORE && c->বাফার->messaging ())
    {
      c->বাফার->sync_so_far ();
      c->বাফার->message (c->font,
			  "replacing glyph at %u (alternate substitution)",
			  c->বাফার->idx);
    }

    c->replace_glyph (alternates[alt_সূচক - 1]);

    যদি (HB_বাফার_MESSAGE_MORE && c->বাফার->messaging ())
    {
      c->বাফার->message (c->font,
			  "replaced glyph at %u (alternate substitution)",
			  c->বাফার->idx - 1u);
    }

    return_trace (সত্য);
  }

  unsigned
  get_alternates (unsigned        start_offset,
                  unsigned       *alternate_গুনতি  /* IN/OUT.  May be NULL. */,
                  hb_codepoint_t *alternate_glyphs /* OUT.     May be NULL. */) ধ্রুবক
  {
    যদি (alternates.len && alternate_গুনতি && alternate_glyphs)
    {
      + alternates.as_array ().sub_array (start_offset, alternate_গুনতি)
      | hb_sink (hb_array (alternate_glyphs, *alternate_গুনতি))
      ;
    }
    ফেরত alternates.len;
  }

  শূন্য
  collect_alternates (hb_codepoint_t gid,
		      hb_map_t  *alternate_গুনতি /* IN/OUT */,
		      hb_map_t  *alternate_glyphs /* IN/OUT */) ধ্রুবক
  {
    + hb_enumerate (alternates)
    | hb_map ([gid] (hb_pair_t<unsigned, hb_codepoint_t> _) { ফেরত hb_pair (gid + (_.first << 24), _.second); })
    | hb_apply ([&] (ধ্রুবক hb_pair_t<hb_codepoint_t, hb_codepoint_t> &p) -> শূন্য
		{ _hb_collect_glyph_alternates_add (p.first, p.second,
						    alternate_গুনতি, alternate_glyphs); })
    ;
  }

  template <typename Iterator,
            hb_requires (hb_is_source_of (Iterator, hb_codepoint_t))>
  যৌক্তিক serialize (hb_serialize_context_t *c,
                  Iterator alts)
  {
    TRACE_SERIALIZE (এই);
    return_trace (alternates.serialize (c, alts));
  }

  যৌক্তিক subset (hb_subset_context_t *c) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    ধ্রুবক hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ধ্রুবক hb_map_t &glyph_map = *c->plan->glyph_map;

    auto it =
      + hb_iter (alternates)
      | hb_filter (glyphset)
      | hb_map (glyph_map)
      ;

    auto *out = c->serializer->start_embed (*এই);
    return_trace (out->serialize (c->serializer, it) &&
                  out->alternates);
  }
};

}
}
}


#endif /* OT_LAYOUT_GSUB_ALTERNATESET_HH */
