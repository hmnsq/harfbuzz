#ifndef OT_LAYOUT_GSUB_LIGATURE_HH
#define OT_LAYOUT_GSUB_LIGATURE_HH

#अन्तर्भाव "Common.hh"

नामाकाश OT {
नामाकाश Layout {
नामाकाश GSUB_impl {

template <typename Types>
संरचना Ligature
{
  सार्वजनिक:
  typename Types::HBGlyphID
		ligGlyph;               /* GlyphID of ligature to substitute */
  HeadlessArray16Of<typename Types::HBGlyphID>
		component;              /* Array of component GlyphIDs--start
                                         * with the second  component--ordered
                                         * in writing direction */
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (Types::size + 2, component);

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (ligGlyph.sanitize (c) && component.sanitize (c));
  }

  तार्किक intersects (ध्रुव hb_set_t *glyphs) ध्रुव
  { प्रत्यागम hb_all (component, glyphs); }

  तार्किक intersects_lig_glyph (ध्रुव hb_set_t *glyphs) ध्रुव
  { प्रत्यागम glyphs->has(ligGlyph); }

  शून्य closure (hb_closure_context_t *c) ध्रुव
  {
    जइ (!intersects (c->glyphs)) प्रत्यागम;
    c->output->add (ligGlyph);
  }

  शून्य depend (hb_depend_context_t *c, hb_codepoint_t first) ध्रुव
  {
    // Build the complete ligature set upfront before adding any edges
    hb_set_t complete_ligset;
    complete_ligset.add (first);
    + hb_iter (component) | hb_sink (complete_ligset);
    जइ (unlikely (complete_ligset.in_error ()))
    {
      c->depend_data->fail ();
      प्रत्यागम;
    }

    तार्किक ligset_created;
    hb_codepoint_t ligset_idx = c->depend_data->find_or_create_set (complete_ligset,
                                                                    &ligset_created);
    जइ (unlikely (ligset_idx == HB_CODEPOINT_INVALID))
      प्रत्यागम;

    // Track whether any edge using this ligset_idx was actually added
    तार्किक any_added = मिच्छ;

    // Now add one edge for each glyph in the complete, immutable set
    + hb_iter (complete_ligset)
    | hb_apply ([&] (hb_codepoint_t gid) {
        जइ (c->depend_data->add_gsub_lookup (gid, c->lookup_index, ligGlyph, ligset_idx))
          any_added = सच्च;
      })
    ;

    // If no edges were added, a newly allocated set is unused - free it for reuse
    जइ (!any_added && ligset_created)
      c->depend_data->discard_set (ligset_idx);
  }

  शून्य collect_glyphs (hb_collect_glyphs_context_t *c) ध्रुव
  {
    c->निवेशः->add_array (component.arrayZ, component.get_length ());
    c->output->add (ligGlyph);
  }

  template <typename set_t>
  शून्य collect_second (set_t &s) ध्रुव
  {
    जइ (unlikely (!component.get_length ()))
    {
      // A ligature without any components. Anything matches.
      s = set_t::full ();
      प्रत्यागम;
    }
    s.add (component.arrayZ[0]);
  }

  तार्किक would_apply (hb_would_apply_context_t *c) ध्रुव
  {
    जइ (c->len != component.lenP1)
      प्रत्यागम मिच्छ;

    प्रति (unsigned पूर्णाङ्क i = 1; i < c->len; i++)
      जइ (likely (c->glyphs[i] != component[i]))
        प्रत्यागम मिच्छ;

    प्रत्यागम सच्च;
  }

  तार्किक apply (hb_ot_apply_context_t *c) ध्रुव
  {
    TRACE_APPLY (एतत्);
    unsigned पूर्णाङ्क गणनम् = component.lenP1;

    जइ (unlikely (!गणनम्)) return_trace (मिच्छ);

    /* Special-case to make it in-place and not consider this
     * as a "ligated" substitution. */
    जइ (unlikely (गणनम् == 1))
    {

      जइ (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
      {
	c->buffer->sync_so_far ();
	c->buffer->message (c->font,
			    "replacing glyph at %u (ligature substitution)",
			    c->buffer->idx);
      }

      c->replace_glyph (ligGlyph);

      जइ (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
      {
	c->buffer->message (c->font,
			    "replaced glyph at %u (ligature substitution)",
			    c->buffer->idx - 1u);
      }

      return_trace (सच्च);
    }

    unsigned पूर्णाङ्क total_component_गणनम् = 0;

    जइ (unlikely (गणनम् > HB_MAX_CONTEXT_LENGTH)) प्रत्यागम मिच्छ;
    unsigned पूर्णाङ्क match_end = 0;

    जइ (likely (!match_निवेशः (c, गणनम्,
                              &component[1],
                              match_glyph,
                              nullptr,
                              &match_end,
                              &total_component_गणनम्)))
    {
      c->buffer->unsafe_to_concat (c->buffer->idx, match_end);
      return_trace (मिच्छ);
    }

    unsigned pos = 0;
    जइ (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      unsigned delta = c->buffer->sync_so_far ();

      pos = c->buffer->idx;

      वर्ण buf[HB_MAX_CONTEXT_LENGTH * 16] = {0};
      वर्ण *p = buf;

      match_end += delta;
      प्रति (unsigned i = 0; i < गणनम्; i++)
      {
	c->match_positions[i] += delta;
	जइ (i)
	  *p++ = ',';
	snprintf (p, परिमाण(buf) - (p - buf), "%u", c->match_positions[i]);
	p += strlen(p);
      }

      c->buffer->message (c->font,
			  "ligating glyphs at %s",
			  buf);
    }

    ligate_निवेशः (c,
                  गणनम्,
                  match_end,
                  ligGlyph,
                  total_component_गणनम्);

    जइ (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->sync_so_far ();
      c->buffer->message (c->font,
			  "ligated glyph at %u",
			  pos);
    }

    return_trace (सच्च);
  }

  template <typename Iterator,
            hb_requires (hb_is_source_of (Iterator, hb_codepoint_t))>
  तार्किक serialize (hb_serialize_context_t *c,
                  hb_codepoint_t ligature,
                  Iterator components /* Starting from second */)
  {
    TRACE_SERIALIZE (एतत्);
    जइ (unlikely (!c->extend_min (एतत्))) return_trace (मिच्छ);
    ligGlyph = ligature;
    जइ (unlikely (!component.serialize (c, components))) return_trace (मिच्छ);
    return_trace (सच्च);
  }

  तार्किक subset (hb_subset_context_t *c, unsigned coverage_idx) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    ध्रुव hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ध्रुव hb_map_t &glyph_map = *c->plan->glyph_map;

    जइ (!intersects (&glyphset) || !glyphset.has (ligGlyph)) return_trace (मिच्छ);
    // Ensure Coverage table is always packed after this.
    c->serializer->add_virtual_link (coverage_idx);

    auto it =
      + hb_iter (component)
      | hb_map (glyph_map)
      ;

    auto *out = c->serializer->start_embed (*एतत्);
    return_trace (out->serialize (c->serializer,
                                  glyph_map[ligGlyph],
                                  it));  }
};


}
}
}

#endif  /* OT_LAYOUT_GSUB_LIGATURE_HH */
