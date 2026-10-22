#నిర్వచితం_కాకపోతే OT_LAYOUT_GSUB_ALTERNATESET_HH
#నిర్వచించు OT_LAYOUT_GSUB_ALTERNATESET_HH

#అన్తర్భావ "Common.hh"

నామాకాశ OT {
నామాకాశ Layout {
నామాకాశ GSUB_impl {

మూస <రకం_పేరు Types>
నిర్మాణం Alternate_సమితి
{
  రక్షిత:
  Array16_యొక్క<రకం_పేరు Types::HBGlyphID>
                alternates;             /* Array of alternate GlyphIDs--in
                                         * arbitrary order */
  బహిరంగ:
  DEFINE_కొలత_ARRAY (2, alternates);

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (alternates.sanitize (c));
  }

  తార్కిక intersects (స్థిరరాశి hb_సమితి_t *glyphs) స్థిరరాశి
  { తిరిగి hb_any (alternates, glyphs); }

  శూన్య సంవృతం (hb_సంవృతం_context_t *c) స్థిరరాశి
  { c->output->కూడు_array (alternates.arrayZ, alternates.len); }

  శూన్య depend (hb_depend_context_t *c, hb_codepoint_t ఆధారం) స్థిరరాశి
  {
    + hb_iter (alternates)
    | hb_apply ([&] (స్థిరరాశి hb_codepoint_t &target) { c->depend_దత్తాంశం->కూడు_gsub_lookup (ఆధారం, c->lookup_సూచీ, target); })
    ;
  }

  శూన్య collect_glyphs (hb_collect_glyphs_context_t *c) స్థిరరాశి
  { c->output->కూడు_array (alternates.arrayZ, alternates.len); }

  తార్కిక apply (hb_ot_apply_context_t *c) స్థిరరాశి
  {
    TRACE_APPLY (ఇది);
    గుర్తులేని పూర్ణాంకం లెక్క = alternates.len;

    ఒకవేళ (unlikely (!లెక్క)) return_trace (అబద్ధం);

    hb_ముసుగు_t glyph_ముసుగు = c->బఫరు->cur().ముసుగు;
    hb_ముసుగు_t lookup_ముసుగు = c->lookup_ముసుగు;

    /* Note: This breaks badly if two features enabled this lookup together. */
    గుర్తులేని పూర్ణాంకం జరుపు = hb_ctz (lookup_ముసుగు);
    గుర్తులేని పూర్ణాంకం alt_సూచీ = ((lookup_ముసుగు & glyph_ముసుగు) >> జరుపు);

    /* If alt_index is MAX_VALUE, randomize feature if it is the rand feature. */
    ఒకవేళ (alt_సూచీ == HB_OT_MAP_MAX_విలువ && c->random)
    {
      /* Maybe we can do better than unsafe-to-break all; but since we are
       * changing random state, it would be hard to track that.  Good 'nough. */
      c->బఫరు->unsafe_గమ్యం_break (0, c->బఫరు->len);
      alt_సూచీ = c->random_సంఖ్య () % లెక్క + 1;
    }

    ఒకవేళ (unlikely (alt_సూచీ > లెక్క || alt_సూచీ == 0)) return_trace (అబద్ధం);

    ఒకవేళ (HB_బఫరు_సందేశం_మరిన్ని && c->బఫరు->messaging ())
    {
      c->బఫరు->sync_so_far ();
      c->బఫరు->సందేశం (c->font,
			  "replacing glyph at %u (alternate substitution)",
			  c->బఫరు->idx);
    }

    c->replace_glyph (alternates[alt_సూచీ - 1]);

    ఒకవేళ (HB_బఫరు_సందేశం_మరిన్ని && c->బఫరు->messaging ())
    {
      c->బఫరు->సందేశం (c->font,
			  "replaced glyph at %u (alternate substitution)",
			  c->బఫరు->idx - 1u);
    }

    return_trace (నిజం);
  }

  గుర్తులేని
  get_alternates (గుర్తులేని        ప్రారంభం_స్థానభ్రంశం,
                  గుర్తులేని       *alternate_లెక్క  /* IN/OUT.  May be NULL. */,
                  hb_codepoint_t *alternate_glyphs /* OUT.     May be NULL. */) స్థిరరాశి
  {
    ఒకవేళ (alternates.len && alternate_లెక్క && alternate_glyphs)
    {
      + alternates.as_array ().sub_array (ప్రారంభం_స్థానభ్రంశం, alternate_లెక్క)
      | hb_sink (hb_array (alternate_glyphs, *alternate_లెక్క))
      ;
    }
    తిరిగి alternates.len;
  }

  శూన్య
  collect_alternates (hb_codepoint_t gid,
		      hb_map_t  *alternate_లెక్క /* IN/OUT */,
		      hb_map_t  *alternate_glyphs /* IN/OUT */) స్థిరరాశి
  {
    + hb_enumerate (alternates)
    | hb_map ([gid] (hb_pair_t<గుర్తులేని, hb_codepoint_t> _) { తిరిగి hb_pair (gid + (_.మొదటి << 24), _.second); })
    | hb_apply ([&] (స్థిరరాశి hb_pair_t<hb_codepoint_t, hb_codepoint_t> &p) -> శూన్య
		{ hb_collect_glyph_alternates_కూడు (p.మొదటి, p.second,
						    alternate_లెక్క, alternate_glyphs); })
    ;
  }

  మూస <రకం_పేరు Iterator,
            hb_requires (hb_is_ఆధారం_యొక్క (Iterator, hb_codepoint_t))>
  తార్కిక serialize (hb_serialize_context_t *c,
                  Iterator alts)
  {
    TRACE_SERIALIZE (ఇది);
    return_trace (alternates.serialize (c, alts));
  }

  తార్కిక subset (hb_subset_context_t *c) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    స్థిరరాశి hb_సమితి_t &glyphset = *c->plan->glyphset_gsub ();
    స్థిరరాశి hb_map_t &glyph_map = *c->plan->glyph_map;

    స్వయంచాలక it =
      + hb_iter (alternates)
      | hb_filter (glyphset)
      | hb_map (glyph_map)
      ;

    స్వయంచాలక *బహిర్గతం = c->serializer->ప్రారంభం_embed (*ఇది);
    return_trace (బహిర్గతం->serialize (c->serializer, it) &&
                  బహిర్గతం->alternates);
  }
};

}
}
}


#అయితే_ముగింపు /* OT_LAYOUT_GSUB_ALTERNATESET_HH */
