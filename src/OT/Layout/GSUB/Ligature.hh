#ifndef OT_LAYOUT_GSUB_LIGATURE_HH
#define OT_LAYOUT_GSUB_LIGATURE_HH

#ضمن "Common.hh"

نطاق OT {
نطاق Layout {
نطاق GSUB_impl {

template <typename Types>
بنية Ligature
{
  عام:
  typename Types::HBGlyphID
		ligGlyph;               /* GlyphID of ligature to substitute */
  HeadlessArray16Of<typename Types::HBGlyphID>
		component;              /* Array of component GlyphIDs--start
                                         * with the second  component--ordered
                                         * in writing direction */
  عام:
  DEFINE_SIZE_ARRAY (Types::size + 2, component);

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (ligGlyph.sanitize (c) && component.sanitize (c));
  }

  منطقي intersects (ثابت hb_set_t *glyphs) ثابت
  { ܦܢܐ hb_all (component, glyphs); }

  منطقي intersects_lig_glyph (ثابت hb_set_t *glyphs) ثابت
  { ܦܢܐ glyphs->has(ligGlyph); }

  فراغ closure (hb_closure_context_t *c) ثابت
  {
    ܐܢ (!intersects (c->glyphs)) ܦܢܐ;
    c->output->add (ligGlyph);
  }

  فراغ depend (hb_depend_context_t *c, hb_codepoint_t first) ثابت
  {
    // Build the complete ligature set upfront before adding any edges
    hb_set_t complete_ligset;
    complete_ligset.add (first);
    + hb_iter (component) | hb_sink (complete_ligset);
    ܐܢ (unlikely (complete_ligset.in_error ()))
    {
      c->depend_data->fail ();
      ܦܢܐ;
    }

    منطقي ligset_created;
    hb_codepoint_t ligset_idx = c->depend_data->find_or_create_set (complete_ligset,
                                                                    &ligset_created);
    ܐܢ (unlikely (ligset_idx == HB_CODEPOINT_INVALID))
      ܦܢܐ;

    // Track whether any edge using this ligset_idx was actually added
    منطقي any_added = ܕܓܠܐ;

    // Now add one edge for each glyph in the complete, immutable set
    + hb_iter (complete_ligset)
    | hb_apply ([&] (hb_codepoint_t gid) {
        ܐܢ (c->depend_data->add_gsub_lookup (gid, c->lookup_index, ligGlyph, ligset_idx))
          any_added = ܫܪܪܐ;
      })
    ;

    // If no edges were added, a newly allocated set is unused - free it for reuse
    ܐܢ (!any_added && ligset_created)
      c->depend_data->discard_set (ligset_idx);
  }

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    c->input->add_array (component.arrayZ, component.get_length ());
    c->output->add (ligGlyph);
  }

  template <typename set_t>
  فراغ collect_second (set_t &s) ثابت
  {
    ܐܢ (unlikely (!component.get_length ()))
    {
      // A ligature without any components. Anything matches.
      s = set_t::full ();
      ܦܢܐ;
    }
    s.add (component.arrayZ[0]);
  }

  منطقي would_apply (hb_would_apply_context_t *c) ثابت
  {
    ܐܢ (c->len != component.lenP1)
      ܦܢܐ ܕܓܠܐ;

    لكل (unsigned صحيح i = 1; i < c->len; i++)
      ܐܢ (likely (c->glyphs[i] != component[i]))
        ܦܢܐ ܕܓܠܐ;

    ܦܢܐ ܫܪܪܐ;
  }

  منطقي apply (hb_ot_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);
    unsigned صحيح count = component.lenP1;

    ܐܢ (unlikely (!count)) return_trace (ܕܓܠܐ);

    /* Special-case to make it in-place and not consider this
     * as a "ligated" substitution. */
    ܐܢ (unlikely (count == 1))
    {

      ܐܢ (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
      {
	c->buffer->sync_so_far ();
	c->buffer->message (c->font,
			    "replacing glyph at %u (ligature substitution)",
			    c->buffer->idx);
      }

      c->replace_glyph (ligGlyph);

      ܐܢ (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
      {
	c->buffer->message (c->font,
			    "replaced glyph at %u (ligature substitution)",
			    c->buffer->idx - 1u);
      }

      return_trace (ܫܪܪܐ);
    }

    unsigned صحيح total_component_count = 0;

    ܐܢ (unlikely (count > HB_MAX_CONTEXT_LENGTH)) ܦܢܐ ܕܓܠܐ;
    unsigned صحيح match_end = 0;

    ܐܢ (likely (!match_input (c, count,
                              &component[1],
                              match_glyph,
                              nullptr,
                              &match_end,
                              &total_component_count)))
    {
      c->buffer->unsafe_to_concat (c->buffer->idx, match_end);
      return_trace (ܕܓܠܐ);
    }

    unsigned pos = 0;
    ܐܢ (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      unsigned delta = c->buffer->sync_so_far ();

      pos = c->buffer->idx;

      حرف buf[HB_MAX_CONTEXT_LENGTH * 16] = {0};
      حرف *p = buf;

      match_end += delta;
      لكل (unsigned i = 0; i < count; i++)
      {
	c->match_positions[i] += delta;
	ܐܢ (i)
	  *p++ = ',';
	snprintf (p, حجم(buf) - (p - buf), "%u", c->match_positions[i]);
	p += strlen(p);
      }

      c->buffer->message (c->font,
			  "ligating glyphs at %s",
			  buf);
    }

    ligate_input (c,
                  count,
                  match_end,
                  ligGlyph,
                  total_component_count);

    ܐܢ (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->sync_so_far ();
      c->buffer->message (c->font,
			  "ligated glyph at %u",
			  pos);
    }

    return_trace (ܫܪܪܐ);
  }

  template <typename Iterator,
            hb_requires (hb_is_source_of (Iterator, hb_codepoint_t))>
  منطقي serialize (hb_serialize_context_t *c,
                  hb_codepoint_t ligature,
                  Iterator components /* Starting from second */)
  {
    TRACE_SERIALIZE (هذا);
    ܐܢ (unlikely (!c->extend_min (هذا))) return_trace (ܕܓܠܐ);
    ligGlyph = ligature;
    ܐܢ (unlikely (!component.serialize (c, components))) return_trace (ܕܓܠܐ);
    return_trace (ܫܪܪܐ);
  }

  منطقي subset (hb_subset_context_t *c, unsigned coverage_idx) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    ܐܢ (!intersects (&glyphset) || !glyphset.has (ligGlyph)) return_trace (ܕܓܠܐ);
    // Ensure Coverage table is always packed after this.
    c->serializer->add_virtual_link (coverage_idx);

    auto it =
      + hb_iter (component)
      | hb_map (glyph_map)
      ;

    auto *out = c->serializer->start_embed (*هذا);
    return_trace (out->serialize (c->serializer,
                                  glyph_map[ligGlyph],
                                  it));  }
};


}
}
}

#endif  /* OT_LAYOUT_GSUB_LIGATURE_HH */
