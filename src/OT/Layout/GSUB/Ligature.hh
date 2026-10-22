#ifndef OT_LAYOUT_GSUB_LIGATURE_HH
#define OT_LAYOUT_GSUB_LIGATURE_HH

#अन्तर्भाव "Common.hh"

नामाकाश OT {
नामाकाश Layout {
नामाकाश GSUB_impl {

ढाँचा <typename Types>
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

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (ligGlyph.sanitize (c) && component.sanitize (c));
  }

  तार्किक intersects (स्थिरांक hb_set_t *glyphs) स्थिरांक
  { परत hb_all (component, glyphs); }

  तार्किक intersects_lig_glyph (स्थिरांक hb_set_t *glyphs) स्थिरांक
  { परत glyphs->has(ligGlyph); }

  शून्य closure (hb_closure_context_t *c) स्थिरांक
  {
    जर (!intersects (c->glyphs)) परत;
    c->output->add (ligGlyph);
  }

  शून्य depend (hb_depend_context_t *c, hb_codepoint_t first) स्थिरांक
  {
    // Build the complete ligature set upfront before adding any edges
    hb_set_t complete_ligset;
    complete_ligset.add (first);
    + hb_iter (component) | hb_sink (complete_ligset);
    जर (unlikely (complete_ligset.in_error ()))
    {
      c->depend_data->fail ();
      परत;
    }

    तार्किक ligset_created;
    hb_codepoint_t ligset_idx = c->depend_data->find_or_create_set (complete_ligset,
                                                                    &ligset_created);
    जर (unlikely (ligset_idx == HB_CODEPOINT_INVALID))
      परत;

    // Track whether any edge using this ligset_idx was actually added
    तार्किक any_added = असत्य;

    // Now add one edge for each glyph in the complete, immutable set
    + hb_iter (complete_ligset)
    | hb_apply ([&] (hb_codepoint_t gid) {
        जर (c->depend_data->add_gsub_lookup (gid, c->lookup_index, ligGlyph, ligset_idx))
          any_added = सत्य;
      })
    ;

    // If no edges were added, a newly allocated set is unused - free it for reuse
    जर (!any_added && ligset_created)
      c->depend_data->discard_set (ligset_idx);
  }

  शून्य collect_glyphs (hb_collect_glyphs_context_t *c) स्थिरांक
  {
    c->आदान->add_array (component.arrayZ, component.get_length ());
    c->output->add (ligGlyph);
  }

  ढाँचा <typename set_t>
  शून्य collect_second (set_t &s) स्थिरांक
  {
    जर (unlikely (!component.get_length ()))
    {
      // A ligature without any components. Anything matches.
      s = set_t::full ();
      परत;
    }
    s.add (component.arrayZ[0]);
  }

  तार्किक would_apply (hb_would_apply_context_t *c) स्थिरांक
  {
    जर (c->len != component.lenP1)
      परत असत्य;

    साठी (अचिन्हित पूर्णांक i = 1; i < c->len; i++)
      जर (likely (c->glyphs[i] != component[i]))
        परत असत्य;

    परत सत्य;
  }

  तार्किक apply (hb_ot_apply_context_t *c) स्थिरांक
  {
    TRACE_APPLY (हे);
    अचिन्हित पूर्णांक मोजणी = component.lenP1;

    जर (unlikely (!मोजणी)) return_trace (असत्य);

    /* Special-case to make it in-place and not consider this
     * as a "ligated" substitution. */
    जर (unlikely (मोजणी == 1))
    {

      जर (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
      {
	c->buffer->sync_so_far ();
	c->buffer->message (c->font,
			    "replacing glyph at %u (ligature substitution)",
			    c->buffer->idx);
      }

      c->replace_glyph (ligGlyph);

      जर (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
      {
	c->buffer->message (c->font,
			    "replaced glyph at %u (ligature substitution)",
			    c->buffer->idx - 1u);
      }

      return_trace (सत्य);
    }

    अचिन्हित पूर्णांक total_component_मोजणी = 0;

    जर (unlikely (मोजणी > HB_MAX_CONTEXT_LENGTH)) परत असत्य;
    अचिन्हित पूर्णांक match_end = 0;

    जर (likely (!match_आदान (c, मोजणी,
                              &component[1],
                              match_glyph,
                              nullptr,
                              &match_end,
                              &total_component_मोजणी)))
    {
      c->buffer->unsafe_to_concat (c->buffer->idx, match_end);
      return_trace (असत्य);
    }

    अचिन्हित pos = 0;
    जर (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      अचिन्हित delta = c->buffer->sync_so_far ();

      pos = c->buffer->idx;

      अक्षर buf[HB_MAX_CONTEXT_LENGTH * 16] = {0};
      अक्षर *p = buf;

      match_end += delta;
      साठी (अचिन्हित i = 0; i < मोजणी; i++)
      {
	c->match_positions[i] += delta;
	जर (i)
	  *p++ = ',';
	snprintf (p, आकार(buf) - (p - buf), "%u", c->match_positions[i]);
	p += म_माप(p);
      }

      c->buffer->message (c->font,
			  "ligating glyphs at %s",
			  buf);
    }

    ligate_आदान (c,
                  मोजणी,
                  match_end,
                  ligGlyph,
                  total_component_मोजणी);

    जर (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->sync_so_far ();
      c->buffer->message (c->font,
			  "ligated glyph at %u",
			  pos);
    }

    return_trace (सत्य);
  }

  ढाँचा <typename Iterator,
            hb_requires (hb_is_source_of (Iterator, hb_codepoint_t))>
  तार्किक serialize (hb_serialize_context_t *c,
                  hb_codepoint_t ligature,
                  Iterator components /* Starting from second */)
  {
    TRACE_SERIALIZE (हे);
    जर (unlikely (!c->extend_min (हे))) return_trace (असत्य);
    ligGlyph = ligature;
    जर (unlikely (!component.serialize (c, components))) return_trace (असत्य);
    return_trace (सत्य);
  }

  तार्किक subset (hb_subset_context_t *c, अचिन्हित coverage_idx) स्थिरांक
  {
    TRACE_SUBSET (हे);
    स्थिरांक hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    स्थिरांक hb_map_t &glyph_map = *c->plan->glyph_map;

    जर (!intersects (&glyphset) || !glyphset.has (ligGlyph)) return_trace (असत्य);
    // Ensure Coverage table is always packed after this.
    c->serializer->add_virtual_link (coverage_idx);

    स्वतः it =
      + hb_iter (component)
      | hb_map (glyph_map)
      ;

    स्वतः *out = c->serializer->start_embed (*हे);
    return_trace (out->serialize (c->serializer,
                                  glyph_map[ligGlyph],
                                  it));  }
};


}
}
}

#endif  /* OT_LAYOUT_GSUB_LIGATURE_HH */
