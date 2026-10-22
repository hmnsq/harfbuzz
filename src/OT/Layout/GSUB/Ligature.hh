#ifndef OT_LAYOUT_GSUB_LIGATURE_HH
#define OT_LAYOUT_GSUB_LIGATURE_HH

#અન્તર્ભાવ "Common.hh"

નામાકાશ OT {
નામાકાશ Layout {
નામાકાશ GSUB_impl {

template <typename Types>
સંરચના Ligature
{
  જાહેર:
  typename Types::HBGlyphID
		ligGlyph;               /* GlyphID of ligature to substitute */
  HeadlessArray16Of<typename Types::HBGlyphID>
		component;              /* Array of component GlyphIDs--start
                                         * with the second  component--ordered
                                         * in writing direction */
  જાહેર:
  DEFINE_SIZE_ARRAY (Types::size + 2, component);

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (ligGlyph.sanitize (c) && component.sanitize (c));
  }

  તાર્કિક intersects (અચલ hb_set_t *glyphs) અચલ
  { પાછા hb_all (component, glyphs); }

  તાર્કિક intersects_lig_glyph (અચલ hb_set_t *glyphs) અચલ
  { પાછા glyphs->has(ligGlyph); }

  શૂન્ય closure (hb_closure_context_t *c) અચલ
  {
    જો (!intersects (c->glyphs)) પાછા;
    c->output->add (ligGlyph);
  }

  શૂન્ય depend (hb_depend_context_t *c, hb_codepoint_t first) અચલ
  {
    // Build the complete ligature set upfront before adding any edges
    hb_set_t complete_ligset;
    complete_ligset.add (first);
    + hb_iter (component) | hb_sink (complete_ligset);
    જો (unlikely (complete_ligset.in_error ()))
    {
      c->depend_data->fail ();
      પાછા;
    }

    તાર્કિક ligset_created;
    hb_codepoint_t ligset_idx = c->depend_data->find_or_create_set (complete_ligset,
                                                                    &ligset_created);
    જો (unlikely (ligset_idx == HB_CODEPOINT_INVALID))
      પાછા;

    // Track whether any edge using this ligset_idx was actually added
    તાર્કિક any_added = ખોટું;

    // Now add one edge for each glyph in the complete, immutable set
    + hb_iter (complete_ligset)
    | hb_apply ([&] (hb_codepoint_t gid) {
        જો (c->depend_data->add_gsub_lookup (gid, c->lookup_index, ligGlyph, ligset_idx))
          any_added = સાચું;
      })
    ;

    // If no edges were added, a newly allocated set is unused - free it for reuse
    જો (!any_added && ligset_created)
      c->depend_data->discard_set (ligset_idx);
  }

  શૂન્ય collect_glyphs (hb_collect_glyphs_context_t *c) અચલ
  {
    c->આગત->add_array (component.arrayZ, component.get_length ());
    c->output->add (ligGlyph);
  }

  template <typename set_t>
  શૂન્ય collect_second (set_t &s) અચલ
  {
    જો (unlikely (!component.get_length ()))
    {
      // A ligature without any components. Anything matches.
      s = set_t::full ();
      પાછા;
    }
    s.add (component.arrayZ[0]);
  }

  તાર્કિક would_apply (hb_would_apply_context_t *c) અચલ
  {
    જો (c->len != component.lenP1)
      પાછા ખોટું;

    માટે (unsigned પૂર્ણાંક i = 1; i < c->len; i++)
      જો (likely (c->glyphs[i] != component[i]))
        પાછા ખોટું;

    પાછા સાચું;
  }

  તાર્કિક apply (hb_ot_apply_context_t *c) અચલ
  {
    TRACE_APPLY (આ);
    unsigned પૂર્ણાંક ગણતરી = component.lenP1;

    જો (unlikely (!ગણતરી)) return_trace (ખોટું);

    /* Special-case to make it in-place and not consider this
     * as a "ligated" substitution. */
    જો (unlikely (ગણતરી == 1))
    {

      જો (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
      {
	c->buffer->sync_so_far ();
	c->buffer->message (c->font,
			    "replacing glyph at %u (ligature substitution)",
			    c->buffer->idx);
      }

      c->replace_glyph (ligGlyph);

      જો (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
      {
	c->buffer->message (c->font,
			    "replaced glyph at %u (ligature substitution)",
			    c->buffer->idx - 1u);
      }

      return_trace (સાચું);
    }

    unsigned પૂર્ણાંક total_component_ગણતરી = 0;

    જો (unlikely (ગણતરી > HB_MAX_CONTEXT_LENGTH)) પાછા ખોટું;
    unsigned પૂર્ણાંક match_end = 0;

    જો (likely (!match_આગત (c, ગણતરી,
                              &component[1],
                              match_glyph,
                              nullptr,
                              &match_end,
                              &total_component_ગણતરી)))
    {
      c->buffer->unsafe_to_concat (c->buffer->idx, match_end);
      return_trace (ખોટું);
    }

    unsigned pos = 0;
    જો (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      unsigned delta = c->buffer->sync_so_far ();

      pos = c->buffer->idx;

      અક્ષર buf[HB_MAX_CONTEXT_LENGTH * 16] = {0};
      અક્ષર *p = buf;

      match_end += delta;
      માટે (unsigned i = 0; i < ગણતરી; i++)
      {
	c->match_positions[i] += delta;
	જો (i)
	  *p++ = ',';
	snprintf (p, કદ(buf) - (p - buf), "%u", c->match_positions[i]);
	p += strlen(p);
      }

      c->buffer->message (c->font,
			  "ligating glyphs at %s",
			  buf);
    }

    ligate_આગત (c,
                  ગણતરી,
                  match_end,
                  ligGlyph,
                  total_component_ગણતરી);

    જો (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->sync_so_far ();
      c->buffer->message (c->font,
			  "ligated glyph at %u",
			  pos);
    }

    return_trace (સાચું);
  }

  template <typename Iterator,
            hb_requires (hb_is_source_of (Iterator, hb_codepoint_t))>
  તાર્કિક serialize (hb_serialize_context_t *c,
                  hb_codepoint_t ligature,
                  Iterator components /* Starting from second */)
  {
    TRACE_SERIALIZE (આ);
    જો (unlikely (!c->extend_min (આ))) return_trace (ખોટું);
    ligGlyph = ligature;
    જો (unlikely (!component.serialize (c, components))) return_trace (ખોટું);
    return_trace (સાચું);
  }

  તાર્કિક subset (hb_subset_context_t *c, unsigned coverage_idx) અચલ
  {
    TRACE_SUBSET (આ);
    અચલ hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    અચલ hb_map_t &glyph_map = *c->plan->glyph_map;

    જો (!intersects (&glyphset) || !glyphset.has (ligGlyph)) return_trace (ખોટું);
    // Ensure Coverage table is always packed after this.
    c->serializer->add_virtual_link (coverage_idx);

    auto it =
      + hb_iter (component)
      | hb_map (glyph_map)
      ;

    auto *out = c->serializer->start_embed (*આ);
    return_trace (out->serialize (c->serializer,
                                  glyph_map[ligGlyph],
                                  it));  }
};


}
}
}

#endif  /* OT_LAYOUT_GSUB_LIGATURE_HH */
