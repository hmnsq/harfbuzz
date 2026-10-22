#ifndef OT_LAYOUT_GSUB_LIGATURE_HH
#define OT_LAYOUT_GSUB_LIGATURE_HH

#ଅନ୍ତର୍ଭାଵ "Common.hh"

ନାମାକାଶ OT {
ନାମାକାଶ Layout {
ନାମାକାଶ GSUB_impl {

template <typename Types>
ସଂରଚନା Ligature
{
  ସର୍ବସାଧାରଣ:
  typename Types::HBGlyphID
		ligGlyph;               /* GlyphID of ligature to substitute */
  HeadlessArray16Of<typename Types::HBGlyphID>
		component;              /* Array of component GlyphIDs--start
                                         * with the second  component--ordered
                                         * in writing direction */
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_ARRAY (Types::size + 2, component);

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (ligGlyph.sanitize (c) && component.sanitize (c));
  }

  ତାର୍କିକ intersects (ଅଚଳ hb_set_t *glyphs) ଅଚଳ
  { ଫେରନ୍ତୁ hb_all (component, glyphs); }

  ତାର୍କିକ intersects_lig_glyph (ଅଚଳ hb_set_t *glyphs) ଅଚଳ
  { ଫେରନ୍ତୁ glyphs->has(ligGlyph); }

  ଶୂନ୍ୟ closure (hb_closure_context_t *c) ଅଚଳ
  {
    ଯଦି (!intersects (c->glyphs)) ଫେରନ୍ତୁ;
    c->output->add (ligGlyph);
  }

  ଶୂନ୍ୟ depend (hb_depend_context_t *c, hb_codepoint_t first) ଅଚଳ
  {
    // Build the complete ligature set upfront before adding any edges
    hb_set_t complete_ligset;
    complete_ligset.add (first);
    + hb_iter (component) | hb_sink (complete_ligset);
    ଯଦି (unlikely (complete_ligset.in_error ()))
    {
      c->depend_data->fail ();
      ଫେରନ୍ତୁ;
    }

    ତାର୍କିକ ligset_created;
    hb_codepoint_t ligset_idx = c->depend_data->find_or_create_set (complete_ligset,
                                                                    &ligset_created);
    ଯଦି (unlikely (ligset_idx == HB_CODEPOINT_INVALID))
      ଫେରନ୍ତୁ;

    // Track whether any edge using this ligset_idx was actually added
    ତାର୍କିକ any_added = ମିଥ୍ୟା;

    // Now add one edge for each glyph in the complete, immutable set
    + hb_iter (complete_ligset)
    | hb_apply ([&] (hb_codepoint_t gid) {
        ଯଦି (c->depend_data->add_gsub_lookup (gid, c->lookup_index, ligGlyph, ligset_idx))
          any_added = ସତ୍ୟ;
      })
    ;

    // If no edges were added, a newly allocated set is unused - free it for reuse
    ଯଦି (!any_added && ligset_created)
      c->depend_data->discard_set (ligset_idx);
  }

  ଶୂନ୍ୟ collect_glyphs (hb_collect_glyphs_context_t *c) ଅଚଳ
  {
    c->ନିବେଶ->add_array (component.arrayZ, component.get_length ());
    c->output->add (ligGlyph);
  }

  template <typename set_t>
  ଶୂନ୍ୟ collect_second (set_t &s) ଅଚଳ
  {
    ଯଦି (unlikely (!component.get_length ()))
    {
      // A ligature without any components. Anything matches.
      s = set_t::full ();
      ଫେରନ୍ତୁ;
    }
    s.add (component.arrayZ[0]);
  }

  ତାର୍କିକ would_apply (hb_would_apply_context_t *c) ଅଚଳ
  {
    ଯଦି (c->len != component.lenP1)
      ଫେରନ୍ତୁ ମିଥ୍ୟା;

    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 1; i < c->len; i++)
      ଯଦି (likely (c->glyphs[i] != component[i]))
        ଫେରନ୍ତୁ ମିଥ୍ୟା;

    ଫେରନ୍ତୁ ସତ୍ୟ;
  }

  ତାର୍କିକ apply (hb_ot_apply_context_t *c) ଅଚଳ
  {
    TRACE_APPLY (ଏହି);
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = component.lenP1;

    ଯଦି (unlikely (!ଗଣନା_2)) return_trace (ମିଥ୍ୟା);

    /* Special-case to make it in-place and not consider this
     * as a "ligated" substitution. */
    ଯଦି (unlikely (ଗଣନା_2 == 1))
    {

      ଯଦି (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
      {
	c->buffer->sync_so_far ();
	c->buffer->message (c->font,
			    "replacing glyph at %u (ligature substitution)",
			    c->buffer->idx);
      }

      c->replace_glyph (ligGlyph);

      ଯଦି (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
      {
	c->buffer->message (c->font,
			    "replaced glyph at %u (ligature substitution)",
			    c->buffer->idx - 1u);
      }

      return_trace (ସତ୍ୟ);
    }

    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା total_component_ଗଣନା = 0;

    ଯଦି (unlikely (ଗଣନା_2 > HB_MAX_CONTEXT_LENGTH)) ଫେରନ୍ତୁ ମିଥ୍ୟା;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା match_end = 0;

    ଯଦି (likely (!match_ନିବେଶ (c, ଗଣନା_2,
                              &component[1],
                              match_glyph,
                              nullptr,
                              &match_end,
                              &total_component_ଗଣନା)))
    {
      c->buffer->unsafe_to_concat (c->buffer->idx, match_end);
      return_trace (ମିଥ୍ୟା);
    }

    unsigned pos = 0;
    ଯଦି (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      unsigned delta = c->buffer->sync_so_far ();

      pos = c->buffer->idx;

      ଅକ୍ଷର buf[HB_MAX_CONTEXT_LENGTH * 16] = {0};
      ଅକ୍ଷର *p = buf;

      match_end += delta;
      ପାଇଁ (unsigned i = 0; i < ଗଣନା_2; i++)
      {
	c->match_positions[i] += delta;
	ଯଦି (i)
	  *p++ = ',';
	snprintf (p, ଆକାର(buf) - (p - buf), "%u", c->match_positions[i]);
	p += strlen(p);
      }

      c->buffer->message (c->font,
			  "ligating glyphs at %s",
			  buf);
    }

    ligate_ନିବେଶ (c,
                  ଗଣନା_2,
                  match_end,
                  ligGlyph,
                  total_component_ଗଣନା);

    ଯଦି (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->sync_so_far ();
      c->buffer->message (c->font,
			  "ligated glyph at %u",
			  pos);
    }

    return_trace (ସତ୍ୟ);
  }

  template <typename Iterator,
            hb_requires (hb_is_source_of (Iterator, hb_codepoint_t))>
  ତାର୍କିକ serialize (hb_serialize_context_t *c,
                  hb_codepoint_t ligature,
                  Iterator components /* Starting from second */)
  {
    TRACE_SERIALIZE (ଏହି);
    ଯଦି (unlikely (!c->extend_min (ଏହି))) return_trace (ମିଥ୍ୟା);
    ligGlyph = ligature;
    ଯଦି (unlikely (!component.serialize (c, components))) return_trace (ମିଥ୍ୟା);
    return_trace (ସତ୍ୟ);
  }

  ତାର୍କିକ subset (hb_subset_context_t *c, unsigned coverage_idx) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    ଅଚଳ hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ଅଚଳ hb_map_t &glyph_map = *c->plan->glyph_map;

    ଯଦି (!intersects (&glyphset) || !glyphset.has (ligGlyph)) return_trace (ମିଥ୍ୟା);
    // Ensure Coverage table is always packed after this.
    c->serializer->add_virtual_link (coverage_idx);

    auto it =
      + hb_iter (component)
      | hb_map (glyph_map)
      ;

    auto *out = c->serializer->start_embed (*ଏହି);
    return_trace (out->serialize (c->serializer,
                                  glyph_map[ligGlyph],
                                  it));  }
};


}
}
}

#endif  /* OT_LAYOUT_GSUB_LIGATURE_HH */
