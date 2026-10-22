#ifndef OT_LAYOUT_GSUB_LIGATURE_HH
#define OT_LAYOUT_GSUB_LIGATURE_HH

#അന്തര്ഭാവ "Common.hh"

നാമാകാശ OT {
നാമാകാശ Layout {
നാമാകാശ GSUB_impl {

template <typename Types>
ഘടന Ligature
{
  പൊതു:
  typename Types::HBGlyphID
		ligGlyph;               /* GlyphID of ligature to substitute */
  HeadlessArray16Of<typename Types::HBGlyphID>
		component;              /* Array of component GlyphIDs--start
                                         * with the second  component--ordered
                                         * in writing direction */
  പൊതു:
  DEFINE_SIZE_ARRAY (Types::size + 2, component);

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (ligGlyph.sanitize (c) && component.sanitize (c));
  }

  തർക്കം intersects (സ്ഥിരം hb_set_t *glyphs) സ്ഥിരം
  { തിരികെ hb_all (component, glyphs); }

  തർക്കം intersects_lig_glyph (സ്ഥിരം hb_set_t *glyphs) സ്ഥിരം
  { തിരികെ glyphs->has(ligGlyph); }

  ശൂന്യം closure (hb_closure_context_t *c) സ്ഥിരം
  {
    എങ്കിൽ (!intersects (c->glyphs)) തിരികെ;
    c->output->add (ligGlyph);
  }

  ശൂന്യം depend (hb_depend_context_t *c, hb_codepoint_t first) സ്ഥിരം
  {
    // Build the complete ligature set upfront before adding any edges
    hb_set_t complete_ligset;
    complete_ligset.add (first);
    + hb_iter (component) | hb_sink (complete_ligset);
    എങ്കിൽ (unlikely (complete_ligset.in_error ()))
    {
      c->depend_data->fail ();
      തിരികെ;
    }

    തർക്കം ligset_created;
    hb_codepoint_t ligset_idx = c->depend_data->find_or_create_set (complete_ligset,
                                                                    &ligset_created);
    എങ്കിൽ (unlikely (ligset_idx == HB_CODEPOINT_INVALID))
      തിരികെ;

    // Track whether any edge using this ligset_idx was actually added
    തർക്കം any_added = അസത്യം;

    // Now add one edge for each glyph in the complete, immutable set
    + hb_iter (complete_ligset)
    | hb_apply ([&] (hb_codepoint_t gid) {
        എങ്കിൽ (c->depend_data->add_gsub_lookup (gid, c->lookup_index, ligGlyph, ligset_idx))
          any_added = സത്യം;
      })
    ;

    // If no edges were added, a newly allocated set is unused - free it for reuse
    എങ്കിൽ (!any_added && ligset_created)
      c->depend_data->discard_set (ligset_idx);
  }

  ശൂന്യം collect_glyphs (hb_collect_glyphs_context_t *c) സ്ഥിരം
  {
    c->നിവേശം->add_array (component.arrayZ, component.get_length ());
    c->output->add (ligGlyph);
  }

  template <typename set_t>
  ശൂന്യം collect_second (set_t &s) സ്ഥിരം
  {
    എങ്കിൽ (unlikely (!component.get_length ()))
    {
      // A ligature without any components. Anything matches.
      s = set_t::full ();
      തിരികെ;
    }
    s.add (component.arrayZ[0]);
  }

  തർക്കം would_apply (hb_would_apply_context_t *c) സ്ഥിരം
  {
    എങ്കിൽ (c->len != component.lenP1)
      തിരികെ അസത്യം;

    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 1; i < c->len; i++)
      എങ്കിൽ (likely (c->glyphs[i] != component[i]))
        തിരികെ അസത്യം;

    തിരികെ സത്യം;
  }

  തർക്കം apply (hb_ot_apply_context_t *c) സ്ഥിരം
  {
    TRACE_APPLY (ഇത്);
    unsigned പൂർണ്ണസംഖ്യ എണ്ണം = component.lenP1;

    എങ്കിൽ (unlikely (!എണ്ണം)) return_trace (അസത്യം);

    /* Special-case to make it in-place and not consider this
     * as a "ligated" substitution. */
    എങ്കിൽ (unlikely (എണ്ണം == 1))
    {

      എങ്കിൽ (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
      {
	c->buffer->sync_so_far ();
	c->buffer->message (c->font,
			    "replacing glyph at %u (ligature substitution)",
			    c->buffer->idx);
      }

      c->replace_glyph (ligGlyph);

      എങ്കിൽ (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
      {
	c->buffer->message (c->font,
			    "replaced glyph at %u (ligature substitution)",
			    c->buffer->idx - 1u);
      }

      return_trace (സത്യം);
    }

    unsigned പൂർണ്ണസംഖ്യ total_component_എണ്ണം = 0;

    എങ്കിൽ (unlikely (എണ്ണം > HB_MAX_CONTEXT_LENGTH)) തിരികെ അസത്യം;
    unsigned പൂർണ്ണസംഖ്യ match_end = 0;

    എങ്കിൽ (likely (!match_നിവേശം (c, എണ്ണം,
                              &component[1],
                              match_glyph,
                              nullptr,
                              &match_end,
                              &total_component_എണ്ണം)))
    {
      c->buffer->unsafe_to_concat (c->buffer->idx, match_end);
      return_trace (അസത്യം);
    }

    unsigned pos = 0;
    എങ്കിൽ (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      unsigned delta = c->buffer->sync_so_far ();

      pos = c->buffer->idx;

      അക്ഷരം buf[HB_MAX_CONTEXT_LENGTH * 16] = {0};
      അക്ഷരം *p = buf;

      match_end += delta;
      വേണ്ടി (unsigned i = 0; i < എണ്ണം; i++)
      {
	c->match_positions[i] += delta;
	എങ്കിൽ (i)
	  *p++ = ',';
	snprintf (p, വലുപ്പം(buf) - (p - buf), "%u", c->match_positions[i]);
	p += strlen(p);
      }

      c->buffer->message (c->font,
			  "ligating glyphs at %s",
			  buf);
    }

    ligate_നിവേശം (c,
                  എണ്ണം,
                  match_end,
                  ligGlyph,
                  total_component_എണ്ണം);

    എങ്കിൽ (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->sync_so_far ();
      c->buffer->message (c->font,
			  "ligated glyph at %u",
			  pos);
    }

    return_trace (സത്യം);
  }

  template <typename Iterator,
            hb_requires (hb_is_source_of (Iterator, hb_codepoint_t))>
  തർക്കം serialize (hb_serialize_context_t *c,
                  hb_codepoint_t ligature,
                  Iterator components /* Starting from second */)
  {
    TRACE_SERIALIZE (ഇത്);
    എങ്കിൽ (unlikely (!c->extend_min (ഇത്))) return_trace (അസത്യം);
    ligGlyph = ligature;
    എങ്കിൽ (unlikely (!component.serialize (c, components))) return_trace (അസത്യം);
    return_trace (സത്യം);
  }

  തർക്കം subset (hb_subset_context_t *c, unsigned coverage_idx) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);
    സ്ഥിരം hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    സ്ഥിരം hb_map_t &glyph_map = *c->plan->glyph_map;

    എങ്കിൽ (!intersects (&glyphset) || !glyphset.has (ligGlyph)) return_trace (അസത്യം);
    // Ensure Coverage table is always packed after this.
    c->serializer->add_virtual_link (coverage_idx);

    auto it =
      + hb_iter (component)
      | hb_map (glyph_map)
      ;

    auto *out = c->serializer->start_embed (*ഇത്);
    return_trace (out->serialize (c->serializer,
                                  glyph_map[ligGlyph],
                                  it));  }
};


}
}
}

#endif  /* OT_LAYOUT_GSUB_LIGATURE_HH */
