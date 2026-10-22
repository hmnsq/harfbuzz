#ifndef OT_LAYOUT_GSUB_ALTERNATESET_HH
#define OT_LAYOUT_GSUB_ALTERNATESET_HH

#شامل "Common.hh"

فضاینام OT {
فضاینام Layout {
فضاینام GSUB_impl {

template <typename Types>
ساختار AlternateSet
{
  محافظتشده:
  Array16Of<typename Types::HBGlyphID>
                alternates;             /* Array of alternate GlyphIDs--in
                                         * arbitrary order */
  عمومی:
  DEFINE_SIZE_ARRAY (2, alternates);

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (alternates.sanitize (c));
  }

  منطقی intersects (ثابت hb_set_t *glyphs) ثابت
  { بازگشت hb_any (alternates, glyphs); }

  تهی closure (hb_closure_context_t *c) ثابت
  { c->output->add_array (alternates.arrayZ, alternates.len); }

  تهی depend (hb_depend_context_t *c, hb_codepoint_t source) ثابت
  {
    + hb_iter (alternates)
    | hb_apply ([&] (ثابت hb_codepoint_t &target) { c->depend_data->add_gsub_lookup (source, c->lookup_index, target); })
    ;
  }

  تهی collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  { c->output->add_array (alternates.arrayZ, alternates.len); }

  منطقی apply (hb_ot_apply_context_t *c) ثابت
  {
    TRACE_APPLY (این);
    unsigned صحیح شمار = alternates.len;

    اگر (unlikely (!شمار)) return_trace (نادرست);

    hb_mask_t glyph_mask = c->buffer->cur().mask;
    hb_mask_t lookup_mask = c->lookup_mask;

    /* Note: This breaks badly if two features enabled this lookup together. */
    unsigned صحیح shift = hb_ctz (lookup_mask);
    unsigned صحیح alt_index = ((lookup_mask & glyph_mask) >> shift);

    /* If alt_index is MAX_VALUE, randomize feature if it is the rand feature. */
    اگر (alt_index == HB_OT_MAP_MAX_مقدار && c->random)
    {
      /* Maybe we can do better than unsafe-to-break all; but since we are
       * changing random state, it would be hard to track that.  Good 'nough. */
      c->buffer->unsafe_to_break (0, c->buffer->len);
      alt_index = c->random_عدد () % شمار + 1;
    }

    اگر (unlikely (alt_index > شمار || alt_index == 0)) return_trace (نادرست);

    اگر (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->sync_so_far ();
      c->buffer->message (c->font,
			  "replacing glyph at %u (alternate substitution)",
			  c->buffer->idx);
    }

    c->replace_glyph (alternates[alt_index - 1]);

    اگر (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->message (c->font,
			  "replaced glyph at %u (alternate substitution)",
			  c->buffer->idx - 1u);
    }

    return_trace (درست);
  }

  unsigned
  get_alternates (unsigned        start_offset,
                  unsigned       *alternate_شمار  /* IN/OUT.  May be NULL. */,
                  hb_codepoint_t *alternate_glyphs /* OUT.     May be NULL. */) ثابت
  {
    اگر (alternates.len && alternate_شمار && alternate_glyphs)
    {
      + alternates.as_array ().sub_array (start_offset, alternate_شمار)
      | hb_sink (hb_array (alternate_glyphs, *alternate_شمار))
      ;
    }
    بازگشت alternates.len;
  }

  تهی
  collect_alternates (hb_codepoint_t gid,
		      hb_map_t  *alternate_شمار /* IN/OUT */,
		      hb_map_t  *alternate_glyphs /* IN/OUT */) ثابت
  {
    + hb_enumerate (alternates)
    | hb_map ([gid] (hb_pair_t<unsigned, hb_codepoint_t> _) { بازگشت hb_pair (gid + (_.first << 24), _.second); })
    | hb_apply ([&] (ثابت hb_pair_t<hb_codepoint_t, hb_codepoint_t> &p) -> تهی
		{ _hb_collect_glyph_alternates_add (p.first, p.second,
						    alternate_شمار, alternate_glyphs); })
    ;
  }

  template <typename Iterator,
            hb_requires (hb_is_source_of (Iterator, hb_codepoint_t))>
  منطقی serialize (hb_serialize_context_t *c,
                  Iterator alts)
  {
    TRACE_SERIALIZE (این);
    return_trace (alternates.serialize (c, alts));
  }

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (این);
    ثابت hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    auto it =
      + hb_iter (alternates)
      | hb_filter (glyphset)
      | hb_map (glyph_map)
      ;

    auto *out = c->serializer->start_embed (*این);
    return_trace (out->serialize (c->serializer, it) &&
                  out->alternates);
  }
};

}
}
}


#endif /* OT_LAYOUT_GSUB_ALTERNATESET_HH */
