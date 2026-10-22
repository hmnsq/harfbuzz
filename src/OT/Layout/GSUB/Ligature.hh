#నిర్వచితం_కాకపోతే OT_LAYOUT_GSUB_LIGATURE_HH
#నిర్వచించు OT_LAYOUT_GSUB_LIGATURE_HH

#అన్తర్భావ "Common.hh"

నామాకాశ OT {
నామాకాశ Layout {
నామాకాశ GSUB_impl {

మూస <రకం_పేరు Types>
నిర్మాణం Ligature
{
  బహిరంగ:
  రకం_పేరు Types::HBGlyphID
		ligGlyph;               /* GlyphID of ligature to substitute */
  Headless_Array16_యొక్క<రకం_పేరు Types::HBGlyphID>
		component;              /* Array of component GlyphIDs--start
                                         * with the second  component--ordered
                                         * in writing direction */
  బహిరంగ:
  DEFINE_కొలత_ARRAY (Types::కొలత + 2, component);

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (ligGlyph.sanitize (c) && component.sanitize (c));
  }

  తార్కిక intersects (స్థిరరాశి hb_సమితి_t *glyphs) స్థిరరాశి
  { తిరిగి hb_all (component, glyphs); }

  తార్కిక intersects_lig_glyph (స్థిరరాశి hb_సమితి_t *glyphs) స్థిరరాశి
  { తిరిగి glyphs->has(ligGlyph); }

  శూన్య సంవృతం (hb_సంవృతం_context_t *c) స్థిరరాశి
  {
    ఒకవేళ (!intersects (c->glyphs)) తిరిగి;
    c->output->కూడు (ligGlyph);
  }

  శూన్య depend (hb_depend_context_t *c, hb_codepoint_t మొదటి) స్థిరరాశి
  {
    // Build the complete ligature set upfront before adding any edges
    hb_సమితి_t complete_ligset;
    complete_ligset.కూడు (మొదటి);
    + hb_iter (component) | hb_sink (complete_ligset);
    ఒకవేళ (unlikely (complete_ligset.లోపల_error ()))
    {
      c->depend_దత్తాంశం->fail ();
      తిరిగి;
    }

    తార్కిక ligset_created;
    hb_codepoint_t ligset_idx = c->depend_దత్తాంశం->కనుగొను_or_create_సమితి (complete_ligset,
                                                                    &ligset_created);
    ఒకవేళ (unlikely (ligset_idx == HB_CODEPOINT_INVALID))
      తిరిగి;

    // Track whether any edge using this ligset_idx was actually added
    తార్కిక any_added = అబద్ధం;

    // Now add one edge for each glyph in the complete, immutable set
    + hb_iter (complete_ligset)
    | hb_apply ([&] (hb_codepoint_t gid) {
        ఒకవేళ (c->depend_దత్తాంశం->కూడు_gsub_lookup (gid, c->lookup_సూచీ, ligGlyph, ligset_idx))
          any_added = నిజం;
      })
    ;

    // If no edges were added, a newly allocated set is unused - free it for reuse
    ఒకవేళ (!any_added && ligset_created)
      c->depend_దత్తాంశం->discard_సమితి (ligset_idx);
  }

  శూన్య collect_glyphs (hb_collect_glyphs_context_t *c) స్థిరరాశి
  {
    c->ఆదానం->కూడు_array (component.arrayZ, component.get_నిడివి ());
    c->output->కూడు (ligGlyph);
  }

  మూస <రకం_పేరు సమితి_t>
  శూన్య collect_రెండవ (సమితి_t &s) స్థిరరాశి
  {
    ఒకవేళ (unlikely (!component.get_నిడివి ()))
    {
      // A ligature without any components. Anything matches.
      s = సమితి_t::నిండిన ();
      తిరిగి;
    }
    s.కూడు (component.arrayZ[0]);
  }

  తార్కిక would_apply (hb_would_apply_context_t *c) స్థిరరాశి
  {
    ఒకవేళ (c->len != component.lenP1)
      తిరిగి అబద్ధం;

    కొరకు (గుర్తులేని పూర్ణాంకం i = 1; i < c->len; i++)
      ఒకవేళ (likely (c->glyphs[i] != component[i]))
        తిరిగి అబద్ధం;

    తిరిగి నిజం;
  }

  తార్కిక apply (hb_ot_apply_context_t *c) స్థిరరాశి
  {
    TRACE_APPLY (ఇది);
    గుర్తులేని పూర్ణాంకం లెక్క = component.lenP1;

    ఒకవేళ (unlikely (!లెక్క)) return_trace (అబద్ధం);

    /* Special-case to make it in-place and not consider this
     * as a "ligated" substitution. */
    ఒకవేళ (unlikely (లెక్క == 1))
    {

      ఒకవేళ (HB_బఫరు_సందేశం_మరిన్ని && c->బఫరు->messaging ())
      {
	c->బఫరు->sync_so_far ();
	c->బఫరు->సందేశం (c->font,
			    "replacing glyph at %u (ligature substitution)",
			    c->బఫరు->idx);
      }

      c->replace_glyph (ligGlyph);

      ఒకవేళ (HB_బఫరు_సందేశం_మరిన్ని && c->బఫరు->messaging ())
      {
	c->బఫరు->సందేశం (c->font,
			    "replaced glyph at %u (ligature substitution)",
			    c->బఫరు->idx - 1u);
      }

      return_trace (నిజం);
    }

    గుర్తులేని పూర్ణాంకం మొత్తం_component_లెక్క = 0;

    ఒకవేళ (unlikely (లెక్క > HB_MAX_CONTEXT_నిడివి)) తిరిగి అబద్ధం;
    గుర్తులేని పూర్ణాంకం సరిపోలిక_చివర = 0;

    ఒకవేళ (likely (!సరిపోలిక_ఆదానం (c, లెక్క,
                              &component[1],
                              సరిపోలిక_glyph,
                              శూన్యసూచిక,
                              &సరిపోలిక_చివర,
                              &మొత్తం_component_లెక్క)))
    {
      c->బఫరు->unsafe_గమ్యం_concat (c->బఫరు->idx, సరిపోలిక_చివర);
      return_trace (అబద్ధం);
    }

    గుర్తులేని pos = 0;
    ఒకవేళ (HB_బఫరు_సందేశం_మరిన్ని && c->బఫరు->messaging ())
    {
      గుర్తులేని delta = c->బఫరు->sync_so_far ();

      pos = c->బఫరు->idx;

      అక్షరం buf[HB_MAX_CONTEXT_నిడివి * 16] = {0};
      అక్షరం *p = buf;

      సరిపోలిక_చివర += delta;
      కొరకు (గుర్తులేని i = 0; i < లెక్క; i++)
      {
	c->సరిపోలిక_positions[i] += delta;
	ఒకవేళ (i)
	  *p++ = ',';
	snprintf (p, పరిమాణం(buf) - (p - buf), "%u", c->సరిపోలిక_positions[i]);
	p += అక్షరశ్రేణి_పొడవు(p);
      }

      c->బఫరు->సందేశం (c->font,
			  "ligating glyphs at %s",
			  buf);
    }

    ligate_ఆదానం (c,
                  లెక్క,
                  సరిపోలిక_చివర,
                  ligGlyph,
                  మొత్తం_component_లెక్క);

    ఒకవేళ (HB_బఫరు_సందేశం_మరిన్ని && c->బఫరు->messaging ())
    {
      c->బఫరు->sync_so_far ();
      c->బఫరు->సందేశం (c->font,
			  "ligated glyph at %u",
			  pos);
    }

    return_trace (నిజం);
  }

  మూస <రకం_పేరు Iterator,
            hb_requires (hb_is_ఆధారం_యొక్క (Iterator, hb_codepoint_t))>
  తార్కిక serialize (hb_serialize_context_t *c,
                  hb_codepoint_t ligature,
                  Iterator components /* Starting from second */)
  {
    TRACE_SERIALIZE (ఇది);
    ఒకవేళ (unlikely (!c->extend_min (ఇది))) return_trace (అబద్ధం);
    ligGlyph = ligature;
    ఒకవేళ (unlikely (!component.serialize (c, components))) return_trace (అబద్ధం);
    return_trace (నిజం);
  }

  తార్కిక subset (hb_subset_context_t *c, గుర్తులేని coverage_idx) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    స్థిరరాశి hb_సమితి_t &glyphset = *c->plan->glyphset_gsub ();
    స్థిరరాశి hb_map_t &glyph_map = *c->plan->glyph_map;

    ఒకవేళ (!intersects (&glyphset) || !glyphset.has (ligGlyph)) return_trace (అబద్ధం);
    // Ensure Coverage table is always packed after this.
    c->serializer->కూడు_virtual_link (coverage_idx);

    స్వయంచాలక it =
      + hb_iter (component)
      | hb_map (glyph_map)
      ;

    స్వయంచాలక *బహిర్గతం = c->serializer->ప్రారంభం_embed (*ఇది);
    return_trace (బహిర్గతం->serialize (c->serializer,
                                  glyph_map[ligGlyph],
                                  it));  }
};


}
}
}

#అయితే_ముగింపు  /* OT_LAYOUT_GSUB_LIGATURE_HH */
