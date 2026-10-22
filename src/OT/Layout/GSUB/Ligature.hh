#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ OT_LAYOUT_GSUB_LIGATURE_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OT_LAYOUT_GSUB_LIGATURE_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "Common.hh"

ਨਾਮਾਕਾਸ਼ OT {
ਨਾਮਾਕਾਸ਼ Layout {
ਨਾਮਾਕਾਸ਼ GSUB_impl {

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Types>
ਬਣਤਰ Ligature
{
  ਜਨਤਕ:
  ਕਿਸਮ_ਨਾਂ Types::HBGlyphID
		ligGlyph;               /* GlyphID of ligature to substitute */
  Headless_Array16_ਦਾ<ਕਿਸਮ_ਨਾਂ Types::HBGlyphID>
		component;              /* Array of component GlyphIDs--start
                                         * with the second  component--ordered
                                         * in writing direction */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_ARRAY (Types::ਮਾਪ + 2, component);

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (ligGlyph.sanitize (c) && component.sanitize (c));
  }

  ਤਾਰਕਿਕ intersects (ਅਚਲ hb_ਸੈੱਟ_t *glyphs) ਅਚਲ
  { ਵਾਪਸ hb_all (component, glyphs); }

  ਤਾਰਕਿਕ intersects_lig_glyph (ਅਚਲ hb_ਸੈੱਟ_t *glyphs) ਅਚਲ
  { ਵਾਪਸ glyphs->has(ligGlyph); }

  ਖਾਲੀ ਘੇਰਾ (hb_ਘੇਰਾ_context_t *c) ਅਚਲ
  {
    ਜੇ (!intersects (c->glyphs)) ਵਾਪਸ;
    c->output->ਜੋੜੋ (ligGlyph);
  }

  ਖਾਲੀ depend (hb_depend_context_t *c, hb_codepoint_t ਪਹਿਲਾ) ਅਚਲ
  {
    // Build the complete ligature set upfront before adding any edges
    hb_ਸੈੱਟ_t complete_ligset;
    complete_ligset.ਜੋੜੋ (ਪਹਿਲਾ);
    + hb_iter (component) | hb_sink (complete_ligset);
    ਜੇ (unlikely (complete_ligset.ਵਿੱਚ_error ()))
    {
      c->depend_ਅੰਕੜੇ->fail ();
      ਵਾਪਸ;
    }

    ਤਾਰਕਿਕ ligset_created;
    hb_codepoint_t ligset_idx = c->depend_ਅੰਕੜੇ->ਲੱਭੋ_or_create_ਸੈੱਟ (complete_ligset,
                                                                    &ligset_created);
    ਜੇ (unlikely (ligset_idx == HB_CODEPOINT_INVALID))
      ਵਾਪਸ;

    // Track whether any edge using this ligset_idx was actually added
    ਤਾਰਕਿਕ any_added = ਝੂਠ;

    // Now add one edge for each glyph in the complete, immutable set
    + hb_iter (complete_ligset)
    | hb_apply ([&] (hb_codepoint_t gid) {
        ਜੇ (c->depend_ਅੰਕੜੇ->ਜੋੜੋ_gsub_lookup (gid, c->lookup_ਸੂਚਕਾਂਕ, ligGlyph, ligset_idx))
          any_added = ਸੱਚ;
      })
    ;

    // If no edges were added, a newly allocated set is unused - free it for reuse
    ਜੇ (!any_added && ligset_created)
      c->depend_ਅੰਕੜੇ->discard_ਸੈੱਟ (ligset_idx);
  }

  ਖਾਲੀ collect_glyphs (hb_collect_glyphs_context_t *c) ਅਚਲ
  {
    c->ਨਿਵੇਸ਼->ਜੋੜੋ_array (component.arrayZ, component.get_ਲੰਮਾਈ ());
    c->output->ਜੋੜੋ (ligGlyph);
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ ਸੈੱਟ_t>
  ਖਾਲੀ collect_ਦੂਜਾ (ਸੈੱਟ_t &s) ਅਚਲ
  {
    ਜੇ (unlikely (!component.get_ਲੰਮਾਈ ()))
    {
      // A ligature without any components. Anything matches.
      s = ਸੈੱਟ_t::ਭਰਿਆ ();
      ਵਾਪਸ;
    }
    s.ਜੋੜੋ (component.arrayZ[0]);
  }

  ਤਾਰਕਿਕ would_apply (hb_would_apply_context_t *c) ਅਚਲ
  {
    ਜੇ (c->len != component.lenP1)
      ਵਾਪਸ ਝੂਠ;

    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 1; i < c->len; i++)
      ਜੇ (likely (c->glyphs[i] != component[i]))
        ਵਾਪਸ ਝੂਠ;

    ਵਾਪਸ ਸੱਚ;
  }

  ਤਾਰਕਿਕ apply (hb_ot_apply_context_t *c) ਅਚਲ
  {
    TRACE_APPLY (ਇਹ);
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2 = component.lenP1;

    ਜੇ (unlikely (!ਗਿਣਤੀ_2)) return_trace (ਝੂਠ);

    /* Special-case to make it in-place and not consider this
     * as a "ligated" substitution. */
    ਜੇ (unlikely (ਗਿਣਤੀ_2 == 1))
    {

      ਜੇ (HB_ਬਫਰ_ਸੁਨੇਹਾ_ਹੋਰ && c->ਬਫਰ->messaging ())
      {
	c->ਬਫਰ->sync_so_far ();
	c->ਬਫਰ->ਸੁਨੇਹਾ (c->font,
			    "replacing glyph at %u (ligature substitution)",
			    c->ਬਫਰ->idx);
      }

      c->replace_glyph (ligGlyph);

      ਜੇ (HB_ਬਫਰ_ਸੁਨੇਹਾ_ਹੋਰ && c->ਬਫਰ->messaging ())
      {
	c->ਬਫਰ->ਸੁਨੇਹਾ (c->font,
			    "replaced glyph at %u (ligature substitution)",
			    c->ਬਫਰ->idx - 1u);
      }

      return_trace (ਸੱਚ);
    }

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਕੁੱਲ_component_ਗਿਣਤੀ = 0;

    ਜੇ (unlikely (ਗਿਣਤੀ_2 > HB_MAX_CONTEXT_ਲੰਮਾਈ)) ਵਾਪਸ ਝੂਠ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਮੇਲ_ਸਿਰਾ = 0;

    ਜੇ (likely (!ਮੇਲ_ਨਿਵੇਸ਼ (c, ਗਿਣਤੀ_2,
                              &component[1],
                              ਮੇਲ_glyph,
                              ਖਾਲੀ_ਸੂਚਕ,
                              &ਮੇਲ_ਸਿਰਾ,
                              &ਕੁੱਲ_component_ਗਿਣਤੀ)))
    {
      c->ਬਫਰ->unsafe_ਮੰਜ਼ਿਲ_concat (c->ਬਫਰ->idx, ਮੇਲ_ਸਿਰਾ);
      return_trace (ਝੂਠ);
    }

    ਬਿਨਾਨਿਸ਼ਾਨੀ pos = 0;
    ਜੇ (HB_ਬਫਰ_ਸੁਨੇਹਾ_ਹੋਰ && c->ਬਫਰ->messaging ())
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ delta = c->ਬਫਰ->sync_so_far ();

      pos = c->ਬਫਰ->idx;

      ਅੱਖਰ buf[HB_MAX_CONTEXT_ਲੰਮਾਈ * 16] = {0};
      ਅੱਖਰ *p = buf;

      ਮੇਲ_ਸਿਰਾ += delta;
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਗਿਣਤੀ_2; i++)
      {
	c->ਮੇਲ_positions[i] += delta;
	ਜੇ (i)
	  *p++ = ',';
	snprintf (p, ਆਕਾਰ(buf) - (p - buf), "%u", c->ਮੇਲ_positions[i]);
	p += ਲੜੀ_ਲੰਬਾਈ(p);
      }

      c->ਬਫਰ->ਸੁਨੇਹਾ (c->font,
			  "ligating glyphs at %s",
			  buf);
    }

    ligate_ਨਿਵੇਸ਼ (c,
                  ਗਿਣਤੀ_2,
                  ਮੇਲ_ਸਿਰਾ,
                  ligGlyph,
                  ਕੁੱਲ_component_ਗਿਣਤੀ);

    ਜੇ (HB_ਬਫਰ_ਸੁਨੇਹਾ_ਹੋਰ && c->ਬਫਰ->messaging ())
    {
      c->ਬਫਰ->sync_so_far ();
      c->ਬਫਰ->ਸੁਨੇਹਾ (c->font,
			  "ligated glyph at %u",
			  pos);
    }

    return_trace (ਸੱਚ);
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Iterator,
            hb_requires (hb_is_ਸਰੋਤ_ਦਾ (Iterator, hb_codepoint_t))>
  ਤਾਰਕਿਕ serialize (hb_serialize_context_t *c,
                  hb_codepoint_t ligature,
                  Iterator components /* Starting from second */)
  {
    TRACE_SERIALIZE (ਇਹ);
    ਜੇ (unlikely (!c->extend_min (ਇਹ))) return_trace (ਝੂਠ);
    ligGlyph = ligature;
    ਜੇ (unlikely (!component.serialize (c, components))) return_trace (ਝੂਠ);
    return_trace (ਸੱਚ);
  }

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c, ਬਿਨਾਨਿਸ਼ਾਨੀ coverage_idx) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਅਚਲ hb_ਸੈੱਟ_t &glyphset = *c->plan->glyphset_gsub ();
    ਅਚਲ hb_map_t &glyph_map = *c->plan->glyph_map;

    ਜੇ (!intersects (&glyphset) || !glyphset.has (ligGlyph)) return_trace (ਝੂਠ);
    // Ensure Coverage table is always packed after this.
    c->serializer->ਜੋੜੋ_virtual_link (coverage_idx);

    ਸਵੈਚਾਲਿਤ it =
      + hb_iter (component)
      | hb_map (glyph_map)
      ;

    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->serializer->ਸ਼ੁਰੂ_embed (*ਇਹ);
    return_trace (ਬਾਹਰਲਾ->serialize (c->serializer,
                                  glyph_map[ligGlyph],
                                  it));  }
};


}
}
}

#ਜੇ_ਅੰਤ  /* OT_LAYOUT_GSUB_LIGATURE_HH */
