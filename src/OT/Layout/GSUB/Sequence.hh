#నిర్వచితం_కాకపోతే OT_LAYOUT_GSUB_SEQUENCE_HH
#నిర్వచించు OT_LAYOUT_GSUB_SEQUENCE_HH

#అన్తర్భావ "Common.hh"

నామాకాశ OT {
నామాకాశ Layout {
నామాకాశ GSUB_impl {

మూస <రకం_పేరు Types>
నిర్మాణం Sequence
{
  రక్షిత:
  Array16_యొక్క<రకం_పేరు Types::HBGlyphID>
                substitute;             /* String of GlyphIDs to substitute */
  బహిరంగ:
  DEFINE_కొలత_ARRAY (2, substitute);

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (substitute.sanitize (c));
  }

  తార్కిక intersects (స్థిరరాశి hb_సమితి_t *glyphs) స్థిరరాశి
  { తిరిగి hb_all (substitute, glyphs); }

  శూన్య సంవృతం (hb_సంవృతం_context_t *c) స్థిరరాశి
  { c->output->కూడు_array (substitute.arrayZ, substitute.len); }

  శూన్య depend (hb_depend_context_t *c, hb_codepoint_t ఆధారం) స్థిరరాశి
  {
    + hb_iter (substitute)
    | hb_apply ([&] (స్థిరరాశి hb_codepoint_t &target) { c->depend_దత్తాంశం->కూడు_gsub_lookup (ఆధారం, c->lookup_సూచీ, target); })
    ;
  }

  శూన్య collect_glyphs (hb_collect_glyphs_context_t *c) స్థిరరాశి
  { c->output->కూడు_array (substitute.arrayZ, substitute.len); }

  తార్కిక apply (hb_ot_apply_context_t *c) స్థిరరాశి
  {
    TRACE_APPLY (ఇది);
    గుర్తులేని పూర్ణాంకం లెక్క = substitute.len;

    /* Special-case to make it in-place and not consider this
     * as a "multiplied" substitution. */
    ఒకవేళ (unlikely (లెక్క == 1))
    {
      ఒకవేళ (HB_బఫరు_సందేశం_మరిన్ని && c->బఫరు->messaging ())
      {
	c->బఫరు->sync_so_far ();
	c->బఫరు->సందేశం (c->font,
			    "replacing glyph at %u (multiple substitution)",
			    c->బఫరు->idx);
      }

      c->replace_glyph (substitute.arrayZ[0]);

      ఒకవేళ (HB_బఫరు_సందేశం_మరిన్ని && c->బఫరు->messaging ())
      {
	c->బఫరు->సందేశం (c->font,
			    "replaced glyph at %u (multiple substitution)",
			    c->బఫరు->idx - 1u);
      }

      return_trace (నిజం);
    }
    /* Spec disallows this, but Uniscribe allows it.
     * https://github.com/harfbuzz/harfbuzz/issues/253 */
    లేకపోతే ఒకవేళ (unlikely (లెక్క == 0))
    {
      ఒకవేళ (HB_బఫరు_సందేశం_మరిన్ని && c->బఫరు->messaging ())
      {
	c->బఫరు->sync_so_far ();
	c->బఫరు->సందేశం (c->font,
			    "deleting glyph at %u (multiple substitution)",
			    c->బఫరు->idx);
      }

      c->బఫరు->delete_glyph ();

      ఒకవేళ (HB_బఫరు_సందేశం_మరిన్ని && c->బఫరు->messaging ())
      {
	c->బఫరు->sync_so_far ();
	c->బఫరు->సందేశం (c->font,
			    "deleted glyph at %u (multiple substitution)",
			    c->బఫరు->idx);
      }

      return_trace (నిజం);
    }

    ఒకవేళ (HB_బఫరు_సందేశం_మరిన్ని && c->బఫరు->messaging ())
    {
      c->బఫరు->sync_so_far ();
      c->బఫరు->సందేశం (c->font,
			  "multiplying glyph at %u",
			  c->బఫరు->idx);
    }

    గుర్తులేని పూర్ణాంకం klass = _hb_glyph_info_is_ligature (&c->బఫరు->cur()) ?
                         HB_OT_LAYOUT_GLYPH_PROPS_పునాది_GLYPH : 0;
    గుర్తులేని lig_id = _hb_glyph_info_get_lig_id (&c->బఫరు->cur());

    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++)
    {
      /* If is attached to a ligature, don't disturb that.
       * https://github.com/harfbuzz/harfbuzz/issues/3069 */
      ఒకవేళ (!lig_id)
        hb_glyph_info_సమితి_lig_props_for_component (&c->బఫరు->cur(), i);
      c->output_glyph_for_component (substitute.arrayZ[i], klass);
    }
    c->బఫరు->skip_glyph ();

    ఒకవేళ (HB_బఫరు_సందేశం_మరిన్ని && c->బఫరు->messaging ())
    {
      c->బఫరు->sync_so_far ();

      అక్షరం buf[HB_MAX_CONTEXT_నిడివి * 16] = {0};
      అక్షరం *p = buf;

      కొరకు (గుర్తులేని i = c->బఫరు->idx - లెక్క; i < c->బఫరు->idx; i++)
      {
	ఒకవేళ (buf < p && పరిమాణం(buf) - 1u > గుర్తులేని (p - buf))
	  *p++ = ',';
	snprintf (p, పరిమాణం(buf) - (p - buf), "%u", i);
	p += అక్షరశ్రేణి_పొడవు(p);
      }

      c->బఫరు->సందేశం (c->font,
			  "multiplied glyphs at %s",
			  buf);
    }

    return_trace (నిజం);
  }

  మూస <రకం_పేరు Iterator,
            hb_requires (hb_is_ఆధారం_యొక్క (Iterator, hb_codepoint_t))>
  తార్కిక serialize (hb_serialize_context_t *c,
                  Iterator subst)
  {
    TRACE_SERIALIZE (ఇది);
    return_trace (substitute.serialize (c, subst));
  }

  తార్కిక subset (hb_subset_context_t *c) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    స్థిరరాశి hb_సమితి_t &glyphset = *c->plan->glyphset_gsub ();
    స్థిరరాశి hb_map_t &glyph_map = *c->plan->glyph_map;

    ఒకవేళ (!intersects (&glyphset)) return_trace (అబద్ధం);

    స్వయంచాలక it =
    + hb_iter (substitute)
    | hb_map (glyph_map)
    ;

    స్వయంచాలక *బహిర్గతం = c->serializer->ప్రారంభం_embed (*ఇది);
    return_trace (బహిర్గతం->serialize (c->serializer, it));
  }
};


}
}
}


#అయితే_ముగింపు /* OT_LAYOUT_GSUB_SEQUENCE_HH */
