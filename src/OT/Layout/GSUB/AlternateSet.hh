#ifndef OT_LAYOUT_GSUB_ALTERNATESET_HH
#define OT_LAYOUT_GSUB_ALTERNATESET_HH

#अन्तर्भाव "Common.hh"

नामाकाश OT {
नामाकाश Layout {
नामाकाश GSUB_impl {

ढाँचा <typename Types>
संरचना AlternateSet
{
  संरक्षित:
  Array16Of<typename Types::HBGlyphID>
                alternates;             /* Array of alternate GlyphIDs--in
                                         * arbitrary order */
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (2, alternates);

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (alternates.sanitize (c));
  }

  तार्किक intersects (स्थिरांक hb_set_t *glyphs) स्थिरांक
  { परत hb_any (alternates, glyphs); }

  शून्य closure (hb_closure_context_t *c) स्थिरांक
  { c->output->add_array (alternates.arrayZ, alternates.len); }

  शून्य depend (hb_depend_context_t *c, hb_codepoint_t source) स्थिरांक
  {
    + hb_iter (alternates)
    | hb_apply ([&] (स्थिरांक hb_codepoint_t &target) { c->depend_data->add_gsub_lookup (source, c->lookup_index, target); })
    ;
  }

  शून्य collect_glyphs (hb_collect_glyphs_context_t *c) स्थिरांक
  { c->output->add_array (alternates.arrayZ, alternates.len); }

  तार्किक apply (hb_ot_apply_context_t *c) स्थिरांक
  {
    TRACE_APPLY (हे);
    अचिन्हित पूर्णांक मोजणी = alternates.len;

    जर (unlikely (!मोजणी)) return_trace (असत्य);

    hb_mask_t glyph_mask = c->buffer->cur().mask;
    hb_mask_t lookup_mask = c->lookup_mask;

    /* Note: This breaks badly if two features enabled this lookup together. */
    अचिन्हित पूर्णांक shift = hb_ctz (lookup_mask);
    अचिन्हित पूर्णांक alt_index = ((lookup_mask & glyph_mask) >> shift);

    /* If alt_index is MAX_VALUE, randomize feature if it is the rand feature. */
    जर (alt_index == HB_OT_MAP_MAX_मूल्य && c->random)
    {
      /* Maybe we can do better than unsafe-to-break all; but since we are
       * changing random state, it would be hard to track that.  Good 'nough. */
      c->buffer->unsafe_to_break (0, c->buffer->len);
      alt_index = c->random_संख्या () % मोजणी + 1;
    }

    जर (unlikely (alt_index > मोजणी || alt_index == 0)) return_trace (असत्य);

    जर (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->sync_so_far ();
      c->buffer->message (c->font,
			  "replacing glyph at %u (alternate substitution)",
			  c->buffer->idx);
    }

    c->replace_glyph (alternates[alt_index - 1]);

    जर (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->message (c->font,
			  "replaced glyph at %u (alternate substitution)",
			  c->buffer->idx - 1u);
    }

    return_trace (सत्य);
  }

  अचिन्हित
  get_alternates (अचिन्हित        start_offset,
                  अचिन्हित       *alternate_मोजणी  /* IN/OUT.  May be NULL. */,
                  hb_codepoint_t *alternate_glyphs /* OUT.     May be NULL. */) स्थिरांक
  {
    जर (alternates.len && alternate_मोजणी && alternate_glyphs)
    {
      + alternates.as_array ().sub_array (start_offset, alternate_मोजणी)
      | hb_sink (hb_array (alternate_glyphs, *alternate_मोजणी))
      ;
    }
    परत alternates.len;
  }

  शून्य
  collect_alternates (hb_codepoint_t gid,
		      hb_map_t  *alternate_मोजणी /* IN/OUT */,
		      hb_map_t  *alternate_glyphs /* IN/OUT */) स्थिरांक
  {
    + hb_enumerate (alternates)
    | hb_map ([gid] (hb_pair_t<अचिन्हित, hb_codepoint_t> _) { परत hb_pair (gid + (_.first << 24), _.second); })
    | hb_apply ([&] (स्थिरांक hb_pair_t<hb_codepoint_t, hb_codepoint_t> &p) -> शून्य
		{ _hb_collect_glyph_alternates_add (p.first, p.second,
						    alternate_मोजणी, alternate_glyphs); })
    ;
  }

  ढाँचा <typename Iterator,
            hb_requires (hb_is_source_of (Iterator, hb_codepoint_t))>
  तार्किक serialize (hb_serialize_context_t *c,
                  Iterator alts)
  {
    TRACE_SERIALIZE (हे);
    return_trace (alternates.serialize (c, alts));
  }

  तार्किक subset (hb_subset_context_t *c) स्थिरांक
  {
    TRACE_SUBSET (हे);
    स्थिरांक hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    स्थिरांक hb_map_t &glyph_map = *c->plan->glyph_map;

    स्वतः it =
      + hb_iter (alternates)
      | hb_filter (glyphset)
      | hb_map (glyph_map)
      ;

    स्वतः *out = c->serializer->start_embed (*हे);
    return_trace (out->serialize (c->serializer, it) &&
                  out->alternates);
  }
};

}
}
}


#endif /* OT_LAYOUT_GSUB_ALTERNATESET_HH */
