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
  { أرجع hb_all (component, glyphs); }

  منطقي intersects_lig_glyph (ثابت hb_set_t *glyphs) ثابت
  { أرجع glyphs->has(ligGlyph); }

  فراغ closure (hb_closure_context_t *c) ثابت
  {
    إذا (!intersects (c->glyphs)) أرجع;
    c->output->add (ligGlyph);
  }

  فراغ depend (hb_depend_context_t *c, hb_codepoint_t first) ثابت
  {
    // Build the complete ligature set upfront before adding any edges
    hb_set_t complete_ligset;
    complete_ligset.add (first);
    + hb_iter (component) | hb_sink (complete_ligset);
    إذا (unlikely (complete_ligset.in_error ()))
    {
      c->depend_data->fail ();
      أرجع;
    }

    منطقي ligset_created;
    hb_codepoint_t ligset_idx = c->depend_data->find_or_create_set (complete_ligset,
                                                                    &ligset_created);
    إذا (unlikely (ligset_idx == HB_CODEPOINT_INVALID))
      أرجع;

    // Track whether any edge using this ligset_idx was actually added
    منطقي any_added = خطأ;

    // Now add one edge for each glyph in the complete, immutable set
    + hb_iter (complete_ligset)
    | hb_apply ([&] (hb_codepoint_t gid) {
        إذا (c->depend_data->add_gsub_lookup (gid, c->lookup_index, ligGlyph, ligset_idx))
          any_added = true;
      })
    ;

    // If no edges were added, a newly allocated set is unused - free it for reuse
    إذا (!any_added && ligset_created)
      c->depend_data->discard_set (ligset_idx);
  }

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    c->تێخستن->add_array (component.arrayZ, component.get_length ());
    c->output->add (ligGlyph);
  }

  template <typename set_t>
  فراغ collect_second (set_t &s) ثابت
  {
    إذا (unlikely (!component.get_length ()))
    {
      // A ligature without any components. Anything matches.
      s = set_t::full ();
      أرجع;
    }
    s.add (component.arrayZ[0]);
  }

  منطقي would_apply (hb_would_apply_context_t *c) ثابت
  {
    إذا (c->len != component.lenP1)
      أرجع خطأ;

    لكل (unsigned صحيح i = 1; i < c->len; i++)
      إذا (likely (c->glyphs[i] != component[i]))
        أرجع خطأ;

    أرجع true;
  }

  منطقي apply (hb_ot_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);
    unsigned صحيح ژماردن = component.lenP1;

    إذا (unlikely (!ژماردن)) return_trace (خطأ);

    /* Special-case to make it in-place and not consider this
     * as a "ligated" substitution. */
    إذا (unlikely (ژماردن == 1))
    {

      إذا (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
      {
	c->buffer->sync_so_far ();
	c->buffer->message (c->font,
			    "replacing glyph at %u (ligature substitution)",
			    c->buffer->idx);
      }

      c->replace_glyph (ligGlyph);

      إذا (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
      {
	c->buffer->message (c->font,
			    "replaced glyph at %u (ligature substitution)",
			    c->buffer->idx - 1u);
      }

      return_trace (true);
    }

    unsigned صحيح total_component_ژماردن = 0;

    إذا (unlikely (ژماردن > HB_MAX_CONTEXT_LENGTH)) أرجع خطأ;
    unsigned صحيح match_end = 0;

    إذا (likely (!match_تێخستن (c, ژماردن,
                              &component[1],
                              match_glyph,
                              nullptr,
                              &match_end,
                              &total_component_ژماردن)))
    {
      c->buffer->unsafe_to_concat (c->buffer->idx, match_end);
      return_trace (خطأ);
    }

    unsigned pos = 0;
    إذا (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      unsigned delta = c->buffer->sync_so_far ();

      pos = c->buffer->idx;

      حرف buf[HB_MAX_CONTEXT_LENGTH * 16] = {0};
      حرف *p = buf;

      match_end += delta;
      لكل (unsigned i = 0; i < ژماردن; i++)
      {
	c->match_positions[i] += delta;
	إذا (i)
	  *p++ = ',';
	snprintf (p, حجم(buf) - (p - buf), "%u", c->match_positions[i]);
	p += strlen(p);
      }

      c->buffer->message (c->font,
			  "ligating glyphs at %s",
			  buf);
    }

    ligate_تێخستن (c,
                  ژماردن,
                  match_end,
                  ligGlyph,
                  total_component_ژماردن);

    إذا (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->sync_so_far ();
      c->buffer->message (c->font,
			  "ligated glyph at %u",
			  pos);
    }

    return_trace (true);
  }

  template <typename Iterator,
            hb_requires (hb_is_source_of (Iterator, hb_codepoint_t))>
  منطقي serialize (hb_serialize_context_t *c,
                  hb_codepoint_t ligature,
                  Iterator components /* Starting from second */)
  {
    TRACE_SERIALIZE (هذا);
    إذا (unlikely (!c->extend_min (هذا))) return_trace (خطأ);
    ligGlyph = ligature;
    إذا (unlikely (!component.serialize (c, components))) return_trace (خطأ);
    return_trace (true);
  }

  منطقي subset (hb_subset_context_t *c, unsigned coverage_idx) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    إذا (!intersects (&glyphset) || !glyphset.has (ligGlyph)) return_trace (خطأ);
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
