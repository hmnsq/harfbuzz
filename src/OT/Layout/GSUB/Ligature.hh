#ifndef OT_LAYOUT_GSUB_LIGATURE_HH
#define OT_LAYOUT_GSUB_LIGATURE_HH

#அந்தர்பாவ "Common.hh"

நாமாகாஶ OT {
நாமாகாஶ Layout {
நாமாகாஶ GSUB_impl {

template <typename Types>
கட்டமைப்பு Ligature
{
  பொது:
  typename Types::HBGlyphID
		ligGlyph;               /* GlyphID of ligature to substitute */
  HeadlessArray16Of<typename Types::HBGlyphID>
		component;              /* Array of component GlyphIDs--start
                                         * with the second  component--ordered
                                         * in writing direction */
  பொது:
  DEFINE_SIZE_ARRAY (Types::size + 2, component);

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (ligGlyph.sanitize (c) && component.sanitize (c));
  }

  தருக்க intersects (மாறிலி hb_set_t *glyphs) மாறிலி
  { திரும்பு hb_all (component, glyphs); }

  தருக்க intersects_lig_glyph (மாறிலி hb_set_t *glyphs) மாறிலி
  { திரும்பு glyphs->has(ligGlyph); }

  வெறுமை closure (hb_closure_context_t *c) மாறிலி
  {
    எனில் (!intersects (c->glyphs)) திரும்பு;
    c->output->add (ligGlyph);
  }

  வெறுமை depend (hb_depend_context_t *c, hb_codepoint_t first) மாறிலி
  {
    // Build the complete ligature set upfront before adding any edges
    hb_set_t complete_ligset;
    complete_ligset.add (first);
    + hb_iter (component) | hb_sink (complete_ligset);
    எனில் (unlikely (complete_ligset.in_error ()))
    {
      c->depend_data->fail ();
      திரும்பு;
    }

    தருக்க ligset_created;
    hb_codepoint_t ligset_idx = c->depend_data->find_or_create_set (complete_ligset,
                                                                    &ligset_created);
    எனில் (unlikely (ligset_idx == HB_CODEPOINT_INVALID))
      திரும்பு;

    // Track whether any edge using this ligset_idx was actually added
    தருக்க any_added = பொய்;

    // Now add one edge for each glyph in the complete, immutable set
    + hb_iter (complete_ligset)
    | hb_apply ([&] (hb_codepoint_t gid) {
        எனில் (c->depend_data->add_gsub_lookup (gid, c->lookup_index, ligGlyph, ligset_idx))
          any_added = உண்மை;
      })
    ;

    // If no edges were added, a newly allocated set is unused - free it for reuse
    எனில் (!any_added && ligset_created)
      c->depend_data->discard_set (ligset_idx);
  }

  வெறுமை collect_glyphs (hb_collect_glyphs_context_t *c) மாறிலி
  {
    c->உள்ளீடு->add_array (component.arrayZ, component.get_length ());
    c->output->add (ligGlyph);
  }

  template <typename set_t>
  வெறுமை collect_second (set_t &s) மாறிலி
  {
    எனில் (unlikely (!component.get_length ()))
    {
      // A ligature without any components. Anything matches.
      s = set_t::full ();
      திரும்பு;
    }
    s.add (component.arrayZ[0]);
  }

  தருக்க would_apply (hb_would_apply_context_t *c) மாறிலி
  {
    எனில் (c->len != component.lenP1)
      திரும்பு பொய்;

    க்கு (unsigned முழு i = 1; i < c->len; i++)
      எனில் (likely (c->glyphs[i] != component[i]))
        திரும்பு பொய்;

    திரும்பு உண்மை;
  }

  தருக்க apply (hb_ot_apply_context_t *c) மாறிலி
  {
    TRACE_APPLY (இது);
    unsigned முழு மொத்தம் = component.lenP1;

    எனில் (unlikely (!மொத்தம்)) return_trace (பொய்);

    /* Special-case to make it in-place and not consider this
     * as a "ligated" substitution. */
    எனில் (unlikely (மொத்தம் == 1))
    {

      எனில் (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
      {
	c->buffer->sync_so_far ();
	c->buffer->message (c->font,
			    "replacing glyph at %u (ligature substitution)",
			    c->buffer->idx);
      }

      c->replace_glyph (ligGlyph);

      எனில் (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
      {
	c->buffer->message (c->font,
			    "replaced glyph at %u (ligature substitution)",
			    c->buffer->idx - 1u);
      }

      return_trace (உண்மை);
    }

    unsigned முழு total_component_மொத்தம் = 0;

    எனில் (unlikely (மொத்தம் > HB_MAX_CONTEXT_LENGTH)) திரும்பு பொய்;
    unsigned முழு match_end = 0;

    எனில் (likely (!match_உள்ளீடு (c, மொத்தம்,
                              &component[1],
                              match_glyph,
                              nullptr,
                              &match_end,
                              &total_component_மொத்தம்)))
    {
      c->buffer->unsafe_to_concat (c->buffer->idx, match_end);
      return_trace (பொய்);
    }

    unsigned pos = 0;
    எனில் (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      unsigned delta = c->buffer->sync_so_far ();

      pos = c->buffer->idx;

      எழுத்து buf[HB_MAX_CONTEXT_LENGTH * 16] = {0};
      எழுத்து *p = buf;

      match_end += delta;
      க்கு (unsigned i = 0; i < மொத்தம்; i++)
      {
	c->match_positions[i] += delta;
	எனில் (i)
	  *p++ = ',';
	snprintf (p, அளவு(buf) - (p - buf), "%u", c->match_positions[i]);
	p += strlen(p);
      }

      c->buffer->message (c->font,
			  "ligating glyphs at %s",
			  buf);
    }

    ligate_உள்ளீடு (c,
                  மொத்தம்,
                  match_end,
                  ligGlyph,
                  total_component_மொத்தம்);

    எனில் (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->sync_so_far ();
      c->buffer->message (c->font,
			  "ligated glyph at %u",
			  pos);
    }

    return_trace (உண்மை);
  }

  template <typename Iterator,
            hb_requires (hb_is_source_of (Iterator, hb_codepoint_t))>
  தருக்க serialize (hb_serialize_context_t *c,
                  hb_codepoint_t ligature,
                  Iterator components /* Starting from second */)
  {
    TRACE_SERIALIZE (இது);
    எனில் (unlikely (!c->extend_min (இது))) return_trace (பொய்);
    ligGlyph = ligature;
    எனில் (unlikely (!component.serialize (c, components))) return_trace (பொய்);
    return_trace (உண்மை);
  }

  தருக்க subset (hb_subset_context_t *c, unsigned coverage_idx) மாறிலி
  {
    TRACE_SUBSET (இது);
    மாறிலி hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    மாறிலி hb_map_t &glyph_map = *c->plan->glyph_map;

    எனில் (!intersects (&glyphset) || !glyphset.has (ligGlyph)) return_trace (பொய்);
    // Ensure Coverage table is always packed after this.
    c->serializer->add_virtual_link (coverage_idx);

    auto it =
      + hb_iter (component)
      | hb_map (glyph_map)
      ;

    auto *out = c->serializer->start_embed (*இது);
    return_trace (out->serialize (c->serializer,
                                  glyph_map[ligGlyph],
                                  it));  }
};


}
}
}

#endif  /* OT_LAYOUT_GSUB_LIGATURE_HH */
