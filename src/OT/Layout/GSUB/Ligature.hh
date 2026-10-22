#ifndef OT_LAYOUT_GSUB_LIGATURE_HH
#define OT_LAYOUT_GSUB_LIGATURE_HH

#অন্তর্ভাৱ "Common.hh"

নামাকাশ OT {
নামাকাশ Layout {
নামাকাশ GSUB_impl {

template <typename Types>
সংরচনা Ligature
{
  সার্ৱজনিক:
  typename Types::HBGlyphID
		ligGlyph;               /* GlyphID of ligature to substitute */
  HeadlessArray16Of<typename Types::HBGlyphID>
		component;              /* Array of component GlyphIDs--start
                                         * with the second  component--ordered
                                         * in writing direction */
  সার্ৱজনিক:
  DEFINE_SIZE_ARRAY (Types::size + 2, component);

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (ligGlyph.sanitize (c) && component.sanitize (c));
  }

  তার্কিক intersects (ধ্রুৱ hb_set_t *glyphs) ধ্রুৱ
  { ঘূৰাই hb_all (component, glyphs); }

  তার্কিক intersects_lig_glyph (ধ্রুৱ hb_set_t *glyphs) ধ্রুৱ
  { ঘূৰাই glyphs->has(ligGlyph); }

  শূন্য closure (hb_closure_context_t *c) ধ্রুৱ
  {
    যদি (!intersects (c->glyphs)) ঘূৰাই;
    c->output->add (ligGlyph);
  }

  শূন্য depend (hb_depend_context_t *c, hb_codepoint_t first) ধ্রুৱ
  {
    // Build the complete ligature set upfront before adding any edges
    hb_set_t complete_ligset;
    complete_ligset.add (first);
    + hb_iter (component) | hb_sink (complete_ligset);
    যদি (unlikely (complete_ligset.in_error ()))
    {
      c->depend_data->fail ();
      ঘূৰাই;
    }

    তার্কিক ligset_created;
    hb_codepoint_t ligset_idx = c->depend_data->find_or_create_set (complete_ligset,
                                                                    &ligset_created);
    যদি (unlikely (ligset_idx == HB_CODEPOINT_INVALID))
      ঘূৰাই;

    // Track whether any edge using this ligset_idx was actually added
    তার্কিক any_added = মিছা;

    // Now add one edge for each glyph in the complete, immutable set
    + hb_iter (complete_ligset)
    | hb_apply ([&] (hb_codepoint_t gid) {
        যদি (c->depend_data->add_gsub_lookup (gid, c->lookup_index, ligGlyph, ligset_idx))
          any_added = সঁচা;
      })
    ;

    // If no edges were added, a newly allocated set is unused - free it for reuse
    যদি (!any_added && ligset_created)
      c->depend_data->discard_set (ligset_idx);
  }

  শূন্য collect_glyphs (hb_collect_glyphs_context_t *c) ধ্রুৱ
  {
    c->প্ৰবেশ->add_array (component.arrayZ, component.get_length ());
    c->output->add (ligGlyph);
  }

  template <typename set_t>
  শূন্য collect_second (set_t &s) ধ্রুৱ
  {
    যদি (unlikely (!component.get_length ()))
    {
      // A ligature without any components. Anything matches.
      s = set_t::full ();
      ঘূৰাই;
    }
    s.add (component.arrayZ[0]);
  }

  তার্কিক would_apply (hb_would_apply_context_t *c) ধ্রুৱ
  {
    যদি (c->len != component.lenP1)
      ঘূৰাই মিছা;

    বাবে (unsigned পূর্ণাঙ্ক i = 1; i < c->len; i++)
      যদি (likely (c->glyphs[i] != component[i]))
        ঘূৰাই মিছা;

    ঘূৰাই সঁচা;
  }

  তার্কিক apply (hb_ot_apply_context_t *c) ধ্রুৱ
  {
    TRACE_APPLY (এতত্);
    unsigned পূর্ণাঙ্ক গুনতি = component.lenP1;

    যদি (unlikely (!গুনতি)) return_trace (মিছা);

    /* Special-case to make it in-place and not consider this
     * as a "ligated" substitution. */
    যদি (unlikely (গুনতি == 1))
    {

      যদি (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
      {
	c->buffer->sync_so_far ();
	c->buffer->message (c->font,
			    "replacing glyph at %u (ligature substitution)",
			    c->buffer->idx);
      }

      c->replace_glyph (ligGlyph);

      যদি (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
      {
	c->buffer->message (c->font,
			    "replaced glyph at %u (ligature substitution)",
			    c->buffer->idx - 1u);
      }

      return_trace (সঁচা);
    }

    unsigned পূর্ণাঙ্ক total_component_গুনতি = 0;

    যদি (unlikely (গুনতি > HB_MAX_CONTEXT_LENGTH)) ঘূৰাই মিছা;
    unsigned পূর্ণাঙ্ক match_end = 0;

    যদি (likely (!match_প্ৰবেশ (c, গুনতি,
                              &component[1],
                              match_glyph,
                              nullptr,
                              &match_end,
                              &total_component_গুনতি)))
    {
      c->buffer->unsafe_to_concat (c->buffer->idx, match_end);
      return_trace (মিছা);
    }

    unsigned pos = 0;
    যদি (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      unsigned delta = c->buffer->sync_so_far ();

      pos = c->buffer->idx;

      ৱর্ণ buf[HB_MAX_CONTEXT_LENGTH * 16] = {0};
      ৱর্ণ *p = buf;

      match_end += delta;
      বাবে (unsigned i = 0; i < গুনতি; i++)
      {
	c->match_positions[i] += delta;
	যদি (i)
	  *p++ = ',';
	snprintf (p, পরিমাণ(buf) - (p - buf), "%u", c->match_positions[i]);
	p += strlen(p);
      }

      c->buffer->message (c->font,
			  "ligating glyphs at %s",
			  buf);
    }

    ligate_প্ৰবেশ (c,
                  গুনতি,
                  match_end,
                  ligGlyph,
                  total_component_গুনতি);

    যদি (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->sync_so_far ();
      c->buffer->message (c->font,
			  "ligated glyph at %u",
			  pos);
    }

    return_trace (সঁচা);
  }

  template <typename Iterator,
            hb_requires (hb_is_source_of (Iterator, hb_codepoint_t))>
  তার্কিক serialize (hb_serialize_context_t *c,
                  hb_codepoint_t ligature,
                  Iterator components /* Starting from second */)
  {
    TRACE_SERIALIZE (এতত্);
    যদি (unlikely (!c->extend_min (এতত্))) return_trace (মিছা);
    ligGlyph = ligature;
    যদি (unlikely (!component.serialize (c, components))) return_trace (মিছা);
    return_trace (সঁচা);
  }

  তার্কিক subset (hb_subset_context_t *c, unsigned coverage_idx) ধ্রুৱ
  {
    TRACE_SUBSET (এতত্);
    ধ্রুৱ hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ধ্রুৱ hb_map_t &glyph_map = *c->plan->glyph_map;

    যদি (!intersects (&glyphset) || !glyphset.has (ligGlyph)) return_trace (মিছা);
    // Ensure Coverage table is always packed after this.
    c->serializer->add_virtual_link (coverage_idx);

    auto it =
      + hb_iter (component)
      | hb_map (glyph_map)
      ;

    auto *out = c->serializer->start_embed (*এতত্);
    return_trace (out->serialize (c->serializer,
                                  glyph_map[ligGlyph],
                                  it));  }
};


}
}
}

#endif  /* OT_LAYOUT_GSUB_LIGATURE_HH */
