#ifndef OT_LAYOUT_GSUB_LIGATURE_HH
#define OT_LAYOUT_GSUB_LIGATURE_HH

#ಅನ್ತರ್ಭಾವ "Common.hh"

ನಾಮಾಕಾಶ OT {
ನಾಮಾಕಾಶ Layout {
ನಾಮಾಕಾಶ GSUB_impl {

template <typename Types>
ಸಂರಚನಾ Ligature
{
  ಸಾರ್ವಜನಿಕ:
  typename Types::HBGlyphID
		ligGlyph;               /* GlyphID of ligature to substitute */
  HeadlessArray16Of<typename Types::HBGlyphID>
		component;              /* Array of component GlyphIDs--start
                                         * with the second  component--ordered
                                         * in writing direction */
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_ARRAY (Types::size + 2, component);

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (ligGlyph.sanitize (c) && component.sanitize (c));
  }

  ತಾರ್ಕಿಕ intersects (ಧ್ರುವ hb_set_t *glyphs) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ hb_all (component, glyphs); }

  ತಾರ್ಕಿಕ intersects_lig_glyph (ಧ್ರುವ hb_set_t *glyphs) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ glyphs->has(ligGlyph); }

  ಶೂನ್ಯ closure (hb_closure_context_t *c) ಧ್ರುವ
  {
    ಯದಿ (!intersects (c->glyphs)) ಪ್ರತ್ಯಾಗಮ;
    c->output->add (ligGlyph);
  }

  ಶೂನ್ಯ depend (hb_depend_context_t *c, hb_codepoint_t first) ಧ್ರುವ
  {
    // Build the complete ligature set upfront before adding any edges
    hb_set_t complete_ligset;
    complete_ligset.add (first);
    + hb_iter (component) | hb_sink (complete_ligset);
    ಯದಿ (unlikely (complete_ligset.in_error ()))
    {
      c->depend_data->fail ();
      ಪ್ರತ್ಯಾಗಮ;
    }

    ತಾರ್ಕಿಕ ligset_created;
    hb_codepoint_t ligset_idx = c->depend_data->find_or_create_set (complete_ligset,
                                                                    &ligset_created);
    ಯದಿ (unlikely (ligset_idx == HB_CODEPOINT_INVALID))
      ಪ್ರತ್ಯಾಗಮ;

    // Track whether any edge using this ligset_idx was actually added
    ತಾರ್ಕಿಕ any_added = ಅಸತ್ಯ;

    // Now add one edge for each glyph in the complete, immutable set
    + hb_iter (complete_ligset)
    | hb_apply ([&] (hb_codepoint_t gid) {
        ಯದಿ (c->depend_data->add_gsub_lookup (gid, c->lookup_index, ligGlyph, ligset_idx))
          any_added = ಸತ್ಯ;
      })
    ;

    // If no edges were added, a newly allocated set is unused - free it for reuse
    ಯದಿ (!any_added && ligset_created)
      c->depend_data->discard_set (ligset_idx);
  }

  ಶೂನ್ಯ collect_glyphs (hb_collect_glyphs_context_t *c) ಧ್ರುವ
  {
    c->ಒಳಹರಿವು->add_array (component.arrayZ, component.get_length ());
    c->output->add (ligGlyph);
  }

  template <typename set_t>
  ಶೂನ್ಯ collect_second (set_t &s) ಧ್ರುವ
  {
    ಯದಿ (unlikely (!component.get_length ()))
    {
      // A ligature without any components. Anything matches.
      s = set_t::full ();
      ಪ್ರತ್ಯಾಗಮ;
    }
    s.add (component.arrayZ[0]);
  }

  ತಾರ್ಕಿಕ would_apply (hb_would_apply_context_t *c) ಧ್ರುವ
  {
    ಯದಿ (c->len != component.lenP1)
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 1; i < c->len; i++)
      ಯದಿ (likely (c->glyphs[i] != component[i]))
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }

  ತಾರ್ಕಿಕ apply (hb_ot_apply_context_t *c) ಧ್ರುವ
  {
    TRACE_APPLY (ಏತತ್);
    unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = component.lenP1;

    ಯದಿ (unlikely (!ಎಣಿಕೆ)) return_trace (ಅಸತ್ಯ);

    /* Special-case to make it in-place and not consider this
     * as a "ligated" substitution. */
    ಯದಿ (unlikely (ಎಣಿಕೆ == 1))
    {

      ಯದಿ (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
      {
	c->buffer->sync_so_far ();
	c->buffer->message (c->font,
			    "replacing glyph at %u (ligature substitution)",
			    c->buffer->idx);
      }

      c->replace_glyph (ligGlyph);

      ಯದಿ (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
      {
	c->buffer->message (c->font,
			    "replaced glyph at %u (ligature substitution)",
			    c->buffer->idx - 1u);
      }

      return_trace (ಸತ್ಯ);
    }

    unsigned ಪೂರ್ಣಾಙ್ಕ total_component_ಎಣಿಕೆ = 0;

    ಯದಿ (unlikely (ಎಣಿಕೆ > HB_MAX_CONTEXT_LENGTH)) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    unsigned ಪೂರ್ಣಾಙ್ಕ match_end = 0;

    ಯದಿ (likely (!match_ಒಳಹರಿವು (c, ಎಣಿಕೆ,
                              &component[1],
                              match_glyph,
                              nullptr,
                              &match_end,
                              &total_component_ಎಣಿಕೆ)))
    {
      c->buffer->unsafe_to_concat (c->buffer->idx, match_end);
      return_trace (ಅಸತ್ಯ);
    }

    unsigned pos = 0;
    ಯದಿ (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      unsigned delta = c->buffer->sync_so_far ();

      pos = c->buffer->idx;

      ವರ್ಣ buf[HB_MAX_CONTEXT_LENGTH * 16] = {0};
      ವರ್ಣ *p = buf;

      match_end += delta;
      ಪ್ರತಿ (unsigned i = 0; i < ಎಣಿಕೆ; i++)
      {
	c->match_positions[i] += delta;
	ಯದಿ (i)
	  *p++ = ',';
	snprintf (p, ಪರಿಮಾಣ(buf) - (p - buf), "%u", c->match_positions[i]);
	p += strlen(p);
      }

      c->buffer->message (c->font,
			  "ligating glyphs at %s",
			  buf);
    }

    ligate_ಒಳಹರಿವು (c,
                  ಎಣಿಕೆ,
                  match_end,
                  ligGlyph,
                  total_component_ಎಣಿಕೆ);

    ಯದಿ (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->sync_so_far ();
      c->buffer->message (c->font,
			  "ligated glyph at %u",
			  pos);
    }

    return_trace (ಸತ್ಯ);
  }

  template <typename Iterator,
            hb_requires (hb_is_source_of (Iterator, hb_codepoint_t))>
  ತಾರ್ಕಿಕ serialize (hb_serialize_context_t *c,
                  hb_codepoint_t ligature,
                  Iterator components /* Starting from second */)
  {
    TRACE_SERIALIZE (ಏತತ್);
    ಯದಿ (unlikely (!c->extend_min (ಏತತ್))) return_trace (ಅಸತ್ಯ);
    ligGlyph = ligature;
    ಯದಿ (unlikely (!component.serialize (c, components))) return_trace (ಅಸತ್ಯ);
    return_trace (ಸತ್ಯ);
  }

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c, unsigned coverage_idx) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    ಧ್ರುವ hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ಧ್ರುವ hb_map_t &glyph_map = *c->plan->glyph_map;

    ಯದಿ (!intersects (&glyphset) || !glyphset.has (ligGlyph)) return_trace (ಅಸತ್ಯ);
    // Ensure Coverage table is always packed after this.
    c->serializer->add_virtual_link (coverage_idx);

    auto it =
      + hb_iter (component)
      | hb_map (glyph_map)
      ;

    auto *out = c->serializer->start_embed (*ಏತತ್);
    return_trace (out->serialize (c->serializer,
                                  glyph_map[ligGlyph],
                                  it));  }
};


}
}
}

#endif  /* OT_LAYOUT_GSUB_LIGATURE_HH */
