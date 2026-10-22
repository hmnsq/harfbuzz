#ifndef OT_LAYOUT_GSUB_LIGATURE_HH
#define OT_LAYOUT_GSUB_LIGATURE_HH

#অন্তর্ভাব "Common.hh"

নামাকাশ OT {
নামাকাশ Layout {
নামাকাশ GSUB_impl {

template <typename Types>
কাঠামো Ligature
{
  সর্বজনীন:
  typename Types::HBGlyphID
		ligGlyph;               /* GlyphID of ligature to substitute */
  HeadlessArray16Of<typename Types::HBGlyphID>
		component;              /* Array of component GlyphIDs--start
                                         * with the second  component--ordered
                                         * in writing direction */
  সর্বজনীন:
  DEFINE_আকার_ARRAY (Types::আকার_2 + 2, component);

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (ligGlyph.sanitize (c) && component.sanitize (c));
  }

  যৌক্তিক intersects (ধ্রুবক hb_set_t *glyphs) ধ্রুবক
  { ফেরত hb_all (component, glyphs); }

  যৌক্তিক intersects_lig_glyph (ধ্রুবক hb_set_t *glyphs) ধ্রুবক
  { ফেরত glyphs->has(ligGlyph); }

  শূন্য closure (hb_closure_context_t *c) ধ্রুবক
  {
    যদি (!intersects (c->glyphs)) ফেরত;
    c->output->add (ligGlyph);
  }

  শূন্য depend (hb_depend_context_t *c, hb_codepoint_t first) ধ্রুবক
  {
    // Build the complete ligature set upfront before adding any edges
    hb_set_t complete_ligset;
    complete_ligset.add (first);
    + hb_iter (component) | hb_sink (complete_ligset);
    যদি (unlikely (complete_ligset.in_error ()))
    {
      c->depend_তথ্য->fail ();
      ফেরত;
    }

    যৌক্তিক ligset_created;
    hb_codepoint_t ligset_idx = c->depend_তথ্য->find_or_create_set (complete_ligset,
                                                                    &ligset_created);
    যদি (unlikely (ligset_idx == HB_CODEPOINT_INVALID))
      ফেরত;

    // Track whether any edge using this ligset_idx was actually added
    যৌক্তিক any_added = মিথ্যা;

    // Now add one edge for each glyph in the complete, immutable set
    + hb_iter (complete_ligset)
    | hb_apply ([&] (hb_codepoint_t gid) {
        যদি (c->depend_তথ্য->add_gsub_lookup (gid, c->lookup_সূচক, ligGlyph, ligset_idx))
          any_added = সত্য;
      })
    ;

    // If no edges were added, a newly allocated set is unused - free it for reuse
    যদি (!any_added && ligset_created)
      c->depend_তথ্য->discard_set (ligset_idx);
  }

  শূন্য collect_glyphs (hb_collect_glyphs_context_t *c) ধ্রুবক
  {
    c->প্রবেশ->add_array (component.arrayZ, component.get_দৈর্ঘ্য ());
    c->output->add (ligGlyph);
  }

  template <typename set_t>
  শূন্য collect_second (set_t &s) ধ্রুবক
  {
    যদি (unlikely (!component.get_দৈর্ঘ্য ()))
    {
      // A ligature without any components. Anything matches.
      s = set_t::full ();
      ফেরত;
    }
    s.add (component.arrayZ[0]);
  }

  যৌক্তিক would_apply (hb_would_apply_context_t *c) ধ্রুবক
  {
    যদি (c->len != component.lenP1)
      ফেরত মিথ্যা;

    জন্য (unsigned পূর্ণসংখ্যা i = 1; i < c->len; i++)
      যদি (likely (c->glyphs[i] != component[i]))
        ফেরত মিথ্যা;

    ফেরত সত্য;
  }

  যৌক্তিক apply (hb_ot_apply_context_t *c) ধ্রুবক
  {
    TRACE_APPLY (এই);
    unsigned পূর্ণসংখ্যা গুনতি = component.lenP1;

    যদি (unlikely (!গুনতি)) return_trace (মিথ্যা);

    /* Special-case to make it in-place and not consider this
     * as a "ligated" substitution. */
    যদি (unlikely (গুনতি == 1))
    {

      যদি (HB_বাফার_MESSAGE_MORE && c->বাফার->messaging ())
      {
	c->বাফার->sync_so_far ();
	c->বাফার->message (c->font,
			    "replacing glyph at %u (ligature substitution)",
			    c->বাফার->idx);
      }

      c->replace_glyph (ligGlyph);

      যদি (HB_বাফার_MESSAGE_MORE && c->বাফার->messaging ())
      {
	c->বাফার->message (c->font,
			    "replaced glyph at %u (ligature substitution)",
			    c->বাফার->idx - 1u);
      }

      return_trace (সত্য);
    }

    unsigned পূর্ণসংখ্যা যোগ_component_গুনতি = 0;

    যদি (unlikely (গুনতি > HB_MAX_CONTEXT_দৈর্ঘ্য)) ফেরত মিথ্যা;
    unsigned পূর্ণসংখ্যা match_end = 0;

    যদি (likely (!match_প্রবেশ (c, গুনতি,
                              &component[1],
                              match_glyph,
                              nullptr,
                              &match_end,
                              &যোগ_component_গুনতি)))
    {
      c->বাফার->unsafe_to_concat (c->বাফার->idx, match_end);
      return_trace (মিথ্যা);
    }

    unsigned pos = 0;
    যদি (HB_বাফার_MESSAGE_MORE && c->বাফার->messaging ())
    {
      unsigned delta = c->বাফার->sync_so_far ();

      pos = c->বাফার->idx;

      অক্ষর buf[HB_MAX_CONTEXT_দৈর্ঘ্য * 16] = {0};
      অক্ষর *p = buf;

      match_end += delta;
      জন্য (unsigned i = 0; i < গুনতি; i++)
      {
	c->match_positions[i] += delta;
	যদি (i)
	  *p++ = ',';
	snprintf (p, আকার(buf) - (p - buf), "%u", c->match_positions[i]);
	p += strlen(p);
      }

      c->বাফার->message (c->font,
			  "ligating glyphs at %s",
			  buf);
    }

    ligate_প্রবেশ (c,
                  গুনতি,
                  match_end,
                  ligGlyph,
                  যোগ_component_গুনতি);

    যদি (HB_বাফার_MESSAGE_MORE && c->বাফার->messaging ())
    {
      c->বাফার->sync_so_far ();
      c->বাফার->message (c->font,
			  "ligated glyph at %u",
			  pos);
    }

    return_trace (সত্য);
  }

  template <typename Iterator,
            hb_requires (hb_is_source_of (Iterator, hb_codepoint_t))>
  যৌক্তিক serialize (hb_serialize_context_t *c,
                  hb_codepoint_t ligature,
                  Iterator components /* Starting from second */)
  {
    TRACE_SERIALIZE (এই);
    যদি (unlikely (!c->extend_min (এই))) return_trace (মিথ্যা);
    ligGlyph = ligature;
    যদি (unlikely (!component.serialize (c, components))) return_trace (মিথ্যা);
    return_trace (সত্য);
  }

  যৌক্তিক subset (hb_subset_context_t *c, unsigned coverage_idx) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    ধ্রুবক hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ধ্রুবক hb_map_t &glyph_map = *c->plan->glyph_map;

    যদি (!intersects (&glyphset) || !glyphset.has (ligGlyph)) return_trace (মিথ্যা);
    // Ensure Coverage table is always packed after this.
    c->serializer->add_virtual_link (coverage_idx);

    auto it =
      + hb_iter (component)
      | hb_map (glyph_map)
      ;

    auto *out = c->serializer->start_embed (*এই);
    return_trace (out->serialize (c->serializer,
                                  glyph_map[ligGlyph],
                                  it));  }
};


}
}
}

#endif  /* OT_LAYOUT_GSUB_LIGATURE_HH */
