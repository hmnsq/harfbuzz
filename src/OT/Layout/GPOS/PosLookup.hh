#నిర్వచితం_కాకపోతే OT_LAYOUT_GPOS_POSLOOKUP_HH
#నిర్వచించు OT_LAYOUT_GPOS_POSLOOKUP_HH

#అన్తర్భావ "PosLookupSubTable.hh"
#అన్తర్భావ "../../../hb-ot-layout-common.hh"

నామాకాశ OT {
నామాకాశ Layout {
నామాకాశ GPOS_impl {

నిర్మాణం PosLookup : Lookup
{
  ఉపయోగించి Sub_పట్టిక = Pos_Lookup_Sub_పట్టిక;

  స్థిరరాశి Sub_పట్టిక& get_subtable (గుర్తులేని పూర్ణాంకం i) స్థిరరాశి
  { తిరిగి Lookup::get_subtable<Sub_పట్టిక> (i); }

  తార్కిక is_తిరగేయి () స్థిరరాశి
  {
    తిరిగి అబద్ధం;
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

  hb_collect_glyphs_context_t::return_t collect_glyphs (hb_collect_glyphs_context_t *c) స్థిరరాశి
  { తిరిగి dispatch (c); }

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

  మూస <రకం_పేరు సమితి_t>
  శూన్య collect_coverage (సమితి_t *glyphs) స్థిరరాశి
  {
    hb_collect_coverage_context_t<సమితి_t> c (glyphs);
    dispatch (&c);
  }

  మూస <రకం_పేరు context_t>
  స్థిర రకం_పేరు context_t::return_t dispatch_recurse_func (context_t *c, గుర్తులేని పూర్ణాంకం lookup_సూచీ);

  మూస <రకం_పేరు context_t, రకం_పేరు ...Ts>
  రకం_పేరు context_t::return_t dispatch (context_t *c, Ts&&... ds) స్థిరరాశి
  { తిరిగి Lookup::dispatch<Sub_పట్టిక> (c, std::forward<Ts> (ds)...); }

  తార్కిక subset (hb_subset_context_t *c) స్థిరరాశి
  { తిరిగి Lookup::subset<Sub_పట్టిక> (c); }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  { తిరిగి Lookup::sanitize<Sub_పట్టిక> (c); }
};

}
}
}

#అయితే_ముగింపు  /* OT_LAYOUT_GPOS_POSLOOKUP_HH */
