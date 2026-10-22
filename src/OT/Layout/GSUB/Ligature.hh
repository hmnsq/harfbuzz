#अगर_अघोषित OT_LAYOUT_GSUB_LIGATURE_HH
#घोषणा OT_LAYOUT_GSUB_LIGATURE_HH

#समावेश "Common.hh"

namespace OT {
namespace Layout {
namespace GSUB_impl {

ढाँचा <typename Types>
काष्ठा Ligature
{
  खुला:
  typename Types::HBGlyphID
		ligGlyph;               /* GlyphID of ligature to substitute */
  HeadlessArray16Of<typename Types::HBGlyphID>
		component;              /* Array of component GlyphIDs--start
                                         * with the second  component--ordered
                                         * in writing direction */
  खुला:
  DEFINE_आकार_ARRAY (Types::आकार + 2, component);

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (ligGlyph.sanitize (c) && component.sanitize (c));
  }

  bool intersects (स्थिर hb_set_t *glyphs) स्थिर
  { वापस hb_all (component, glyphs); }

  bool intersects_lig_glyph (स्थिर hb_set_t *glyphs) स्थिर
  { वापस glyphs->has(ligGlyph); }

  व्योम closure (hb_closure_context_t *c) स्थिर
  {
    अगर (!intersects (c->glyphs)) वापस;
    c->output->add (ligGlyph);
  }

  व्योम depend (hb_depend_context_t *c, hb_codepoint_t first) स्थिर
  {
    // Build the complete ligature set upfront before adding any edges
    hb_set_t complete_ligset;
    complete_ligset.add (first);
    + hb_iter (component) | hb_sink (complete_ligset);
    अगर (unlikely (complete_ligset.in_error ()))
    {
      c->depend_तथ्य->हार ();
      वापस;
    }

    bool ligset_created;
    hb_codepoint_t ligset_idx = c->depend_तथ्य->find_or_create_set (complete_ligset,
                                                                    &ligset_created);
    अगर (unlikely (ligset_idx == HB_CODEPOINT_INVALID))
      वापस;

    // Track whether any edge using this ligset_idx was actually added
    bool any_added = false;

    // Now add one edge for each glyph in the complete, immutable set
    + hb_iter (complete_ligset)
    | hb_apply ([&] (hb_codepoint_t gid) {
        अगर (c->depend_तथ्य->add_gsub_lookup (gid, c->lookup_सूचकांक, ligGlyph, ligset_idx))
          any_added = true;
      })
    ;

    // If no edges were added, a newly allocated set is unused - free it for reuse
    अगर (!any_added && ligset_created)
      c->depend_तथ्य->discard_set (ligset_idx);
  }

  व्योम collect_glyphs (hb_collect_glyphs_context_t *c) स्थिर
  {
    c->निवेश->add_array (component.arrayZ, component.get_लंबाई ());
    c->output->add (ligGlyph);
  }

  ढाँचा <typename set_t>
  व्योम collect_second (set_t &s) स्थिर
  {
    अगर (unlikely (!component.get_लंबाई ()))
    {
      // A ligature without any components. Anything matches.
      s = set_t::full ();
      वापस;
    }
    s.add (component.arrayZ[0]);
  }

  bool would_apply (hb_would_apply_context_t *c) स्थिर
  {
    अगर (c->len != component.lenP1)
      वापस false;

    क्रम (अचिन्हित पूर्णांक i = 1; i < c->len; i++)
      अगर (likely (c->glyphs[i] != component[i]))
        वापस false;

    वापस true;
  }

  bool apply (hb_ot_apply_context_t *c) स्थिर
  {
    TRACE_APPLY (यह);
    अचिन्हित पूर्णांक गिनती = component.lenP1;

    अगर (unlikely (!गिनती)) return_trace (false);

    /* Special-case to make it in-place and not consider this
     * as a "ligated" substitution. */
    अगर (unlikely (गिनती == 1))
    {

      अगर (HB_बफर_MESSAGE_MORE && c->बफर->messaging ())
      {
	c->बफर->sync_so_far ();
	c->बफर->message (c->font,
			    "replacing glyph at %u (ligature substitution)",
			    c->बफर->idx);
      }

      c->replace_glyph (ligGlyph);

      अगर (HB_बफर_MESSAGE_MORE && c->बफर->messaging ())
      {
	c->बफर->message (c->font,
			    "replaced glyph at %u (ligature substitution)",
			    c->बफर->idx - 1u);
      }

      return_trace (true);
    }

    अचिन्हित पूर्णांक योग_component_गिनती = 0;

    अगर (unlikely (गिनती > HB_MAX_CONTEXT_लंबाई)) वापस false;
    अचिन्हित पूर्णांक match_end = 0;

    अगर (likely (!match_निवेश (c, गिनती,
                              &component[1],
                              match_glyph,
                              nullptr,
                              &match_end,
                              &योग_component_गिनती)))
    {
      c->बफर->unsafe_to_concat (c->बफर->idx, match_end);
      return_trace (false);
    }

    अचिन्हित pos = 0;
    अगर (HB_बफर_MESSAGE_MORE && c->बफर->messaging ())
    {
      अचिन्हित delta = c->बफर->sync_so_far ();

      pos = c->बफर->idx;

      अक्षर buf[HB_MAX_CONTEXT_लंबाई * 16] = {0};
      अक्षर *p = buf;

      match_end += delta;
      क्रम (अचिन्हित i = 0; i < गिनती; i++)
      {
	c->match_positions[i] += delta;
	अगर (i)
	  *p++ = ',';
	snprintf (p, माप(buf) - (p - buf), "%u", c->match_positions[i]);
	p += म_माप(p);
      }

      c->बफर->message (c->font,
			  "ligating glyphs at %s",
			  buf);
    }

    ligate_निवेश (c,
                  गिनती,
                  match_end,
                  ligGlyph,
                  योग_component_गिनती);

    अगर (HB_बफर_MESSAGE_MORE && c->बफर->messaging ())
    {
      c->बफर->sync_so_far ();
      c->बफर->message (c->font,
			  "ligated glyph at %u",
			  pos);
    }

    return_trace (true);
  }

  ढाँचा <typename Iterator,
            hb_requires (hb_is_source_of (Iterator, hb_codepoint_t))>
  bool serialize (hb_serialize_context_t *c,
                  hb_codepoint_t ligature,
                  Iterator components /* Starting from second */)
  {
    TRACE_SERIALIZE (यह);
    अगर (unlikely (!c->extend_min (यह))) return_trace (false);
    ligGlyph = ligature;
    अगर (unlikely (!component.serialize (c, components))) return_trace (false);
    return_trace (true);
  }

  bool subset (hb_subset_context_t *c, अचिन्हित coverage_idx) स्थिर
  {
    TRACE_SUBSET (यह);
    स्थिर hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    स्थिर hb_map_t &glyph_map = *c->plan->glyph_map;

    अगर (!intersects (&glyphset) || !glyphset.has (ligGlyph)) return_trace (false);
    // Ensure Coverage table is always packed after this.
    c->serializer->add_virtual_link (coverage_idx);

    स्वतः it =
      + hb_iter (component)
      | hb_map (glyph_map)
      ;

    स्वतः *out = c->serializer->start_embed (*यह);
    return_trace (out->serialize (c->serializer,
                                  glyph_map[ligGlyph],
                                  it));  }
};


}
}
}

#पूर्ण_अगर  /* OT_LAYOUT_GSUB_LIGATURE_HH */
