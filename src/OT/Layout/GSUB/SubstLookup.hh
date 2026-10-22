#నిర్వచితం_కాకపోతే OT_LAYOUT_GSUB_SUBSTLOOKUP_HH
#నిర్వచించు OT_LAYOUT_GSUB_SUBSTLOOKUP_HH

#అన్తర్భావ "Common.hh"
#అన్తర్భావ "SubstLookupSubTable.hh"

నామాకాశ OT {
నామాకాశ Layout {
నామాకాశ GSUB_impl {

నిర్మాణం SubstLookup : Lookup
{
  ఉపయోగించి Sub_పట్టిక = Subst_Lookup_Sub_పట్టిక;

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  { తిరిగి Lookup::sanitize<Sub_పట్టిక> (c); }

  స్థిరరాశి Sub_పట్టిక& get_subtable (గుర్తులేని పూర్ణాంకం i) స్థిరరాశి
  { తిరిగి Lookup::get_subtable<Sub_పట్టిక> (i); }

  స్థిర అంతఃవరుస తార్కిక lookup_type_is_తిరగేయి (గుర్తులేని పూర్ణాంకం lookup_type)
  { తిరిగి lookup_type == Sub_పట్టిక::తిరగేయి_Chain_Single; }

  తార్కిక is_తిరగేయి () స్థిరరాశి
  {
    గుర్తులేని పూర్ణాంకం type = get_type ();
    ఒకవేళ (unlikely (type == Sub_పట్టిక::Extension) && hb_barrier ())
      తిరిగి get_subtable (0).u.extension.is_తిరగేయి ();
    తిరిగి lookup_type_is_తిరగేయి (type);
  }

  తార్కిక may_have_non_1to1 () స్థిరరాశి
  {
    hb_have_non_1to1_context_t c;
    తిరిగి dispatch (&c);
  }

  తార్కిక apply (hb_ot_apply_context_t *c) స్థిరరాశి
  {
    TRACE_APPLY (ఇది);
    return_trace (dispatch (c));
  }

  తార్కిక intersects (స్థిరరాశి hb_సమితి_t *glyphs) స్థిరరాశి
  {
    hb_intersects_context_t c (glyphs);
    తిరిగి dispatch (&c);
  }

  hb_సంవృతం_context_t::return_t సంవృతం (hb_సంవృతం_context_t *c, గుర్తులేని పూర్ణాంకం this_సూచీ) స్థిరరాశి
  {
    ఒకవేళ (!c->should_సందర్శించు_lookup (this_సూచీ))
      తిరిగి hb_సంవృతం_context_t::default_return_విలువ ();

    c->సమితి_recurse_func (dispatch_సంవృతం_recurse_func);

    hb_సంవృతం_context_t::return_t ret = dispatch (c);

    c->flush ();

    తిరిగి ret;
  }

  శూన్య depend (hb_depend_context_t *c) స్థిరరాశి
  {
    c->సమితి_recurse_func (dispatch_depend_recurse_func);
    dispatch (c);
  }

  hb_సంవృతం_lookups_context_t::return_t సంవృతం_lookups (hb_సంవృతం_lookups_context_t *c, గుర్తులేని this_సూచీ) స్థిరరాశి
  {
    ఒకవేళ (c->is_lookup_సందర్శించిన (this_సూచీ))
      తిరిగి hb_సంవృతం_lookups_context_t::default_return_విలువ ();

    c->సమితి_lookup_సందర్శించిన (this_సూచీ);
    ఒకవేళ (!intersects (c->glyphs))
    {
      c->సమితి_lookup_inactive (this_సూచీ);
      తిరిగి hb_సంవృతం_lookups_context_t::default_return_విలువ ();
    }

    hb_సంవృతం_lookups_context_t::return_t ret = dispatch (c);
    తిరిగి ret;
  }

  hb_collect_glyphs_context_t::return_t collect_glyphs (hb_collect_glyphs_context_t *c) స్థిరరాశి
  {
    c->సమితి_recurse_func (dispatch_recurse_func<hb_collect_glyphs_context_t>);
    తిరిగి dispatch (c);
  }

  మూస <రకం_పేరు సమితి_t>
  శూన్య collect_coverage (సమితి_t *glyphs) స్థిరరాశి
  {
    hb_collect_coverage_context_t<సమితి_t> c (glyphs);
    dispatch (&c);
  }

  తార్కిక would_apply (hb_would_apply_context_t *c,
                    స్థిరరాశి hb_ot_layout_lookup_accelerator_t *accel) స్థిరరాశి
  {
    ఒకవేళ (unlikely (!c->len)) తిరిగి అబద్ధం;
    ఒకవేళ (!accel->may_have (c->glyphs[0])) తిరిగి అబద్ధం;
      తిరిగి dispatch (c);
  }

  మూస<రకం_పేరు Glyphs, రకం_పేరు Substitutes,
	   hb_requires (hb_is_sorted_ఆధారం_యొక్క (Glyphs,
						స్థిరరాశి hb_codepoint_t) &&
			hb_is_ఆధారం_యొక్క (Substitutes,
					 స్థిరరాశి hb_codepoint_t))>
  తార్కిక serialize_single (hb_serialize_context_t *c,
                         uint32_t lookup_props,
                         Glyphs glyphs,
                         Substitutes substitutes)
  {
    TRACE_SERIALIZE (ఇది);
    ఒకవేళ (unlikely (!Lookup::serialize (c, Sub_పట్టిక::Single, lookup_props, 1))) return_trace (అబద్ధం);
    ఒకవేళ (c->push<Sub_పట్టిక> ()->u.single.serialize (c, hb_zip (glyphs, substitutes)))
    {
      c->కూడు_link (get_subtables<Sub_పట్టిక> ()[0], c->తీయి_pack ());
      return_trace (నిజం);
    }
    c->తీయి_discard ();
    return_trace (అబద్ధం);
  }

  మూస<రకం_పేరు Iterator,
           hb_requires (hb_is_sorted_iterator (Iterator))>
  తార్కిక serialize (hb_serialize_context_t *c,
		  uint32_t lookup_props,
		  Iterator it)
  {
    TRACE_SERIALIZE (ఇది);
    ఒకవేళ (unlikely (!Lookup::serialize (c, Sub_పట్టిక::గుణిజం, lookup_props, 1))) return_trace (అబద్ధం);
    ఒకవేళ (c->push<Sub_పట్టిక> ()->u.గుణిజం_2.
        serialize (c, it))
    {
      c->కూడు_link (get_subtables<Sub_పట్టిక> ()[0], c->తీయి_pack ());
      return_trace (నిజం);
    }
    c->తీయి_discard ();
    return_trace (అబద్ధం);
  }

  మూస <రకం_పేరు GlyphID>
  తార్కిక serialize_alternate (hb_serialize_context_t *c,
                            uint32_t lookup_props,
                            hb_sorted_array_t<GlyphID> glyphs,
                            hb_array_t<స్థిరరాశి గుర్తులేని పూర్ణాంకం> alternate_len_list,
                            hb_array_t<GlyphID> alternate_glyphs_list)
  {
    TRACE_SERIALIZE (ఇది);
    ఒకవేళ (unlikely (!Lookup::serialize (c, Sub_పట్టిక::Alternate, lookup_props, 1))) return_trace (అబద్ధం);

    ఒకవేళ (c->push<Sub_పట్టిక> ()->u.alternate.
        serialize (c,
                   glyphs,
                   alternate_len_list,
                   alternate_glyphs_list))
    {
      c->కూడు_link (get_subtables<Sub_పట్టిక> ()[0], c->తీయి_pack ());
      return_trace (నిజం);
    }
    c->తీయి_discard ();
    return_trace (అబద్ధం);
  }

  మూస <రకం_పేరు GlyphID>
  తార్కిక serialize_ligature (hb_serialize_context_t *c,
                           uint32_t lookup_props,
                           hb_sorted_array_t<GlyphID> మొదటి_glyphs,
                           hb_array_t<స్థిరరాశి గుర్తులేని పూర్ణాంకం> ligature_చొప్పున_మొదటి_glyph_లెక్క_list,
                           hb_array_t<GlyphID> ligatures_list,
                           hb_array_t<స్థిరరాశి గుర్తులేని పూర్ణాంకం> component_లెక్క_list,
                           hb_array_t<GlyphID> component_list /* Starting from second for each ligature */)
  {
    TRACE_SERIALIZE (ఇది);
    ఒకవేళ (unlikely (!Lookup::serialize (c, Sub_పట్టిక::Ligature, lookup_props, 1))) return_trace (అబద్ధం);
    ఒకవేళ (c->push<Sub_పట్టిక> ()->u.ligature.
        serialize (c,
                   మొదటి_glyphs,
                   ligature_చొప్పున_మొదటి_glyph_లెక్క_list,
                   ligatures_list,
                   component_లెక్క_list,
                   component_list))
    {
      c->కూడు_link (get_subtables<Sub_పట్టిక> ()[0], c->తీయి_pack ());
      return_trace (నిజం);
    }
    c->తీయి_discard ();
    return_trace (అబద్ధం);
  }

  మూస <రకం_పేరు context_t>
  స్థిర అంతఃవరుస రకం_పేరు context_t::return_t dispatch_recurse_func (context_t *c, గుర్తులేని పూర్ణాంకం lookup_సూచీ);

  స్థిర అంతఃవరుస రకం_పేరు hb_సంవృతం_context_t::return_t సంవృతం_glyphs_recurse_func (hb_సంవృతం_context_t *c, గుర్తులేని lookup_సూచీ, hb_సమితి_t *covered_seq_indices, గుర్తులేని seq_సూచీ, గుర్తులేని చివర_సూచీ);


  స్థిర అంతఃవరుస hb_సంవృతం_context_t::return_t dispatch_సంవృతం_recurse_func (hb_సంవృతం_context_t *c, గుర్తులేని lookup_సూచీ, hb_సమితి_t *covered_seq_indices, గుర్తులేని seq_సూచీ, గుర్తులేని చివర_సూచీ)
  {
    ఒకవేళ (!c->should_సందర్శించు_lookup (lookup_సూచీ))
      తిరిగి hb_ఖాళీలు_t ();

    hb_సంవృతం_context_t::return_t ret = సంవృతం_glyphs_recurse_func (c, lookup_సూచీ, covered_seq_indices, seq_సూచీ, చివర_సూచీ);

    /* While in theory we should flush here, it will cause timeouts because a recursive
     * lookup can keep growing the glyph set.  Skip, and outer loop will retry up to
     * HB_CLOSURE_MAX_STAGES time, which should be enough for every realistic font. */
    //c->flush ();

    తిరిగి ret;
  }

  స్థిర అంతఃవరుస hb_depend_context_t::return_t depend_glyphs_recurse_func (hb_depend_context_t *c, గుర్తులేని lookup_సూచీ, hb_బిట్టు_పుట_t *covered_seq_indices, గుర్తులేని seq_సూచీ, గుర్తులేని చివర_సూచీ);

  స్థిర అంతఃవరుస hb_depend_context_t::return_t dispatch_depend_recurse_func (hb_depend_context_t *c, గుర్తులేని lookup_సూచీ, hb_బిట్టు_పుట_t *covered_seq_indices, గుర్తులేని seq_సూచీ, గుర్తులేని చివర_సూచీ)
  {
    తిరిగి depend_glyphs_recurse_func (c, lookup_సూచీ, covered_seq_indices, seq_సూచీ, చివర_సూచీ);
  }

  మూస <రకం_పేరు context_t, రకం_పేరు ...Ts>
  రకం_పేరు context_t::return_t dispatch (context_t *c, Ts&&... ds) స్థిరరాశి
  { తిరిగి Lookup::dispatch<Sub_పట్టిక> (c, std::forward<Ts> (ds)...); }

  తార్కిక subset (hb_subset_context_t *c) స్థిరరాశి
  { తిరిగి Lookup::subset<Sub_పట్టిక> (c); }
};


}
}
}

#అయితే_ముగింపు  /* OT_LAYOUT_GSUB_SUBSTLOOKUP_HH */
