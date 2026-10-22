#ifndef OT_LAYOUT_GSUB_LIGATURE_HH
#define OT_LAYOUT_GSUB_LIGATURE_HH

#شامل "Common.hh"

فضاینام OT {
فضاینام Layout {
فضاینام GSUB_impl {

template <typename Types>
ساختار Ligature
{
  عمومی:
  typename Types::HBGlyphID
		ligGlyph;               /* GlyphID of ligature to substitute */
  HeadlessArray16Of<typename Types::HBGlyphID>
		component;              /* Array of component GlyphIDs--start
                                         * with the second  component--ordered
                                         * in writing direction */
  عمومی:
  DEFINE_SIZE_ARRAY (Types::size + 2, component);

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (ligGlyph.sanitize (c) && component.sanitize (c));
  }

  منطقی intersects (ثابت hb_set_t *glyphs) ثابت
  { بازگشت hb_all (component, glyphs); }

  منطقی intersects_lig_glyph (ثابت hb_set_t *glyphs) ثابت
  { بازگشت glyphs->has(ligGlyph); }

  تهی closure (hb_closure_context_t *c) ثابت
  {
    اگر (!intersects (c->glyphs)) بازگشت;
    c->output->add (ligGlyph);
  }

  تهی depend (hb_depend_context_t *c, hb_codepoint_t first) ثابت
  {
    // Build the complete ligature set upfront before adding any edges
    hb_set_t complete_ligset;
    complete_ligset.add (first);
    + hb_iter (component) | hb_sink (complete_ligset);
    اگر (unlikely (complete_ligset.in_error ()))
    {
      c->depend_data->fail ();
      بازگشت;
    }

    منطقی ligset_created;
    hb_codepoint_t ligset_idx = c->depend_data->find_or_create_set (complete_ligset,
                                                                    &ligset_created);
    اگر (unlikely (ligset_idx == HB_CODEPOINT_INVALID))
      بازگشت;

    // Track whether any edge using this ligset_idx was actually added
    منطقی any_added = نادرست;

    // Now add one edge for each glyph in the complete, immutable set
    + hb_iter (complete_ligset)
    | hb_apply ([&] (hb_codepoint_t gid) {
        اگر (c->depend_data->add_gsub_lookup (gid, c->lookup_index, ligGlyph, ligset_idx))
          any_added = درست;
      })
    ;

    // If no edges were added, a newly allocated set is unused - free it for reuse
    اگر (!any_added && ligset_created)
      c->depend_data->discard_set (ligset_idx);
  }

  تهی collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    c->درون_داد->add_array (component.arrayZ, component.get_length ());
    c->output->add (ligGlyph);
  }

  template <typename set_t>
  تهی collect_second (set_t &s) ثابت
  {
    اگر (unlikely (!component.get_length ()))
    {
      // A ligature without any components. Anything matches.
      s = set_t::full ();
      بازگشت;
    }
    s.add (component.arrayZ[0]);
  }

  منطقی would_apply (hb_would_apply_context_t *c) ثابت
  {
    اگر (c->len != component.lenP1)
      بازگشت نادرست;

    برای (unsigned صحیح i = 1; i < c->len; i++)
      اگر (likely (c->glyphs[i] != component[i]))
        بازگشت نادرست;

    بازگشت درست;
  }

  منطقی apply (hb_ot_apply_context_t *c) ثابت
  {
    TRACE_APPLY (این);
    unsigned صحیح شمار = component.lenP1;

    اگر (unlikely (!شمار)) return_trace (نادرست);

    /* Special-case to make it in-place and not consider this
     * as a "ligated" substitution. */
    اگر (unlikely (شمار == 1))
    {

      اگر (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
      {
	c->buffer->sync_so_far ();
	c->buffer->message (c->font,
			    "replacing glyph at %u (ligature substitution)",
			    c->buffer->idx);
      }

      c->replace_glyph (ligGlyph);

      اگر (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
      {
	c->buffer->message (c->font,
			    "replaced glyph at %u (ligature substitution)",
			    c->buffer->idx - 1u);
      }

      return_trace (درست);
    }

    unsigned صحیح total_component_شمار = 0;

    اگر (unlikely (شمار > HB_MAX_CONTEXT_LENGTH)) بازگشت نادرست;
    unsigned صحیح match_end = 0;

    اگر (likely (!match_درون_داد (c, شمار,
                              &component[1],
                              match_glyph,
                              nullptr,
                              &match_end,
                              &total_component_شمار)))
    {
      c->buffer->unsafe_to_concat (c->buffer->idx, match_end);
      return_trace (نادرست);
    }

    unsigned pos = 0;
    اگر (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      unsigned delta = c->buffer->sync_so_far ();

      pos = c->buffer->idx;

      نویسه buf[HB_MAX_CONTEXT_LENGTH * 16] = {0};
      نویسه *p = buf;

      match_end += delta;
      برای (unsigned i = 0; i < شمار; i++)
      {
	c->match_positions[i] += delta;
	اگر (i)
	  *p++ = ',';
	snprintf (p, اندازه(buf) - (p - buf), "%u", c->match_positions[i]);
	p += strlen(p);
      }

      c->buffer->message (c->font,
			  "ligating glyphs at %s",
			  buf);
    }

    ligate_درون_داد (c,
                  شمار,
                  match_end,
                  ligGlyph,
                  total_component_شمار);

    اگر (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->sync_so_far ();
      c->buffer->message (c->font,
			  "ligated glyph at %u",
			  pos);
    }

    return_trace (درست);
  }

  template <typename Iterator,
            hb_requires (hb_is_source_of (Iterator, hb_codepoint_t))>
  منطقی serialize (hb_serialize_context_t *c,
                  hb_codepoint_t ligature,
                  Iterator components /* Starting from second */)
  {
    TRACE_SERIALIZE (این);
    اگر (unlikely (!c->extend_min (این))) return_trace (نادرست);
    ligGlyph = ligature;
    اگر (unlikely (!component.serialize (c, components))) return_trace (نادرست);
    return_trace (درست);
  }

  منطقی subset (hb_subset_context_t *c, unsigned coverage_idx) ثابت
  {
    TRACE_SUBSET (این);
    ثابت hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    اگر (!intersects (&glyphset) || !glyphset.has (ligGlyph)) return_trace (نادرست);
    // Ensure Coverage table is always packed after this.
    c->serializer->add_virtual_link (coverage_idx);

    auto it =
      + hb_iter (component)
      | hb_map (glyph_map)
      ;

    auto *out = c->serializer->start_embed (*این);
    return_trace (out->serialize (c->serializer,
                                  glyph_map[ligGlyph],
                                  it));  }
};


}
}
}

#endif  /* OT_LAYOUT_GSUB_LIGATURE_HH */
