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
    यदि (!intersects (c->glyphs)) प्रत्यागम;
    c->output->add (ligGlyph);
  }

  शून्य depend (hb_depend_context_t *c, hb_codepoint_t first) ध्रुव
  {
    // Build the complete ligature set upfront before adding any edges
    hb_set_t complete_ligset;
    complete_ligset.add (first);
    + hb_iter (component) | hb_sink (complete_ligset);
    यदि (unlikely (complete_ligset.in_error ()))
    {
      c->depend_data->fail ();
      प्रत्यागम;
    }

    तार्किक ligset_created;
    hb_codepoint_t ligset_idx = c->depend_data->find_or_create_set (complete_ligset,
                                                                    &ligset_created);
    यदि (unlikely (ligset_idx == HB_CODEPOINT_INVALID))
      प्रत्यागम;

    // Track whether any edge using this ligset_idx was actually added
    तार्किक any_added = असत्य;

    // Now add one edge for each glyph in the complete, immutable set
    + hb_iter (complete_ligset)
    | hb_apply ([&] (hb_codepoint_t gid) {
        यदि (c->depend_data->add_gsub_lookup (gid, c->lookup_index, ligGlyph, ligset_idx))
          any_added = सत्य;
      })
    ;

    // If no edges were added, a newly allocated set is unused - free it for reuse
    यदि (!any_added && ligset_created)
      c->depend_data->discard_set (ligset_idx);
  }

  शून्य collect_glyphs (hb_collect_glyphs_context_t *c) ध्रुव
  {
    c->निवेश->add_array (component.arrayZ, component.get_length ());
    c->output->add (ligGlyph);
  }

  ढाँचा <typename set_t>
  शून्य collect_second (set_t &s) ध्रुव
  {
    यदि (unlikely (!component.get_length ()))
    {
      // A ligature without any components. Anything matches.
      s = set_t::full ();
      प्रत्यागम;
    }
    s.add (component.arrayZ[0]);
  }

  तार्किक would_apply (hb_would_apply_context_t *c) ध्रुव
  {
    यदि (c->len != component.lenP1)
      प्रत्यागम असत्य;

    प्रति (अचिन्हित पूर्णाङ्क i = 1; i < c->len; i++)
      यदि (likely (c->glyphs[i] != component[i]))
        प्रत्यागम असत्य;

    प्रत्यागम सत्य;
  }

  तार्किक apply (hb_ot_apply_context_t *c) ध्रुव
  {
    TRACE_APPLY (एतत्);
    अचिन्हित पूर्णाङ्क गिनती = component.lenP1;

    यदि (unlikely (!गिनती)) return_trace (असत्य);

    /* Special-case to make it in-place and not consider this
     * as a "ligated" substitution. */
    यदि (unlikely (गिनती == 1))
    {

      यदि (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
      {
	c->buffer->sync_so_far ();
	c->buffer->message (c->font,
			    "replacing glyph at %u (ligature substitution)",
			    c->buffer->idx);
      }

      c->replace_glyph (ligGlyph);

      यदि (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
      {
	c->buffer->message (c->font,
			    "replaced glyph at %u (ligature substitution)",
			    c->buffer->idx - 1u);
      }

      return_trace (सत्य);
    }

    अचिन्हित पूर्णाङ्क total_component_गिनती = 0;

    यदि (unlikely (गिनती > HB_MAX_CONTEXT_LENGTH)) प्रत्यागम असत्य;
    अचिन्हित पूर्णाङ्क match_end = 0;

    यदि (likely (!match_निवेश (c, गिनती,
                              &component[1],
                              match_glyph,
                              nullptr,
                              &match_end,
                              &total_component_गिनती)))
    {
      c->buffer->unsafe_to_concat (c->buffer->idx, match_end);
      return_trace (असत्य);
    }

    अचिन्हित pos = 0;
    यदि (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      अचिन्हित delta = c->buffer->sync_so_far ();

      pos = c->buffer->idx;

      वर्ण buf[HB_MAX_CONTEXT_LENGTH * 16] = {0};
      वर्ण *p = buf;

      match_end += delta;
      प्रति (अचिन्हित i = 0; i < गिनती; i++)
      {
	c->match_positions[i] += delta;
	यदि (i)
	  *p++ = ',';
	snprintf (p, परिमाण(buf) - (p - buf), "%u", c->match_positions[i]);
	p += म_माप(p);
      }

      c->buffer->message (c->font,
			  "ligating glyphs at %s",
			  buf);
    }

    ligate_निवेश (c,
                  गिनती,
                  match_end,
                  ligGlyph,
                  total_component_गिनती);

    यदि (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
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
    TRACE_SERIALIZE (एतत्);
    यदि (unlikely (!c->extend_min (एतत्))) return_trace (असत्य);
    ligGlyph = ligature;
    यदि (unlikely (!component.serialize (c, components))) return_trace (असत्य);
    return_trace (सत्य);
  }

  तार्किक subset (hb_subset_context_t *c, अचिन्हित coverage_idx) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    ध्रुव hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ध्रुव hb_map_t &glyph_map = *c->plan->glyph_map;

    यदि (!intersects (&glyphset) || !glyphset.has (ligGlyph)) return_trace (असत्य);
    // Ensure Coverage table is always packed after this.
    c->serializer->add_virtual_link (coverage_idx);

    स्वतः it =
      + hb_iter (component)
      | hb_map (glyph_map)
      ;

    स्वतः *out = c->serializer->start_embed (*एतत्);
    return_trace (out->serialize (c->serializer,
                                  glyph_map[ligGlyph],
                                  it));  }
};


}
}
}

#endif  /* OT_LAYOUT_GSUB_LIGATURE_HH */
