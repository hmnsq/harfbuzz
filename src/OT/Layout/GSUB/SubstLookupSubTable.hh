#నిర్వచితం_కాకపోతే OT_LAYOUT_GSUB_SUBSTLOOKUPSUBTABLE_HH
#నిర్వచించు OT_LAYOUT_GSUB_SUBSTLOOKUPSUBTABLE_HH

#అన్తర్భావ "Common.hh"
#అన్తర్భావ "SingleSubst.hh"
#అన్తర్భావ "MultipleSubst.hh"
#అన్తర్భావ "AlternateSubst.hh"
#అన్తర్భావ "LigatureSubst.hh"
#అన్తర్భావ "ContextSubst.hh"
#అన్తర్భావ "ChainContextSubst.hh"
#అన్తర్భావ "ExtensionSubst.hh"
#అన్తర్భావ "ReverseChainSingleSubst.hh"

నామాకాశ OT {
నామాకాశ Layout {
నామాకాశ GSUB_impl {

నిర్మాణం Subst_Lookup_Sub_పట్టిక
{
  మిత్ర నిర్మాణం ::OT::Lookup;
  మిత్ర నిర్మాణం SubstLookup;

  రక్షిత:
  సంఘం {
  SingleSubst                   single;
  గుణిజం_Subst                 గుణిజం_2;
  AlternateSubst                alternate;
  LigatureSubst                 ligature;
  ContextSubst                  context;
  ChainContextSubst             chainContext;
  ExtensionSubst                extension;
  తిరగేయి_Chain_Single_Subst       తిరగేయి_Chain_Context_Single;
  } u;
  బహిరంగ:
  DEFINE_కొలత_MIN (0);

  గణన Type {
    Single              = 1,
    గుణిజం            = 2,
    Alternate           = 3,
    Ligature            = 4,
    Context             = 5,
    ChainContext        = 6,
    Extension           = 7,
    తిరగేయి_Chain_Single  = 8
  };

  మూస <రకం_పేరు context_t, రకం_పేరు ...Ts>
  రకం_పేరు context_t::return_t dispatch (context_t *c, గుర్తులేని పూర్ణాంకం lookup_type, Ts&&... ds) స్థిరరాశి
  {
    TRACE_DISPATCH (ఇది, lookup_type);
    ఎంపిక (lookup_type) {
    స్థితి Single:                hb_barrier (); return_trace (u.single.dispatch (c, std::forward<Ts> (ds)...));
    స్థితి గుణిజం:              hb_barrier (); return_trace (u.గుణిజం_2.dispatch (c, std::forward<Ts> (ds)...));
    స్థితి Alternate:             hb_barrier (); return_trace (u.alternate.dispatch (c, std::forward<Ts> (ds)...));
    స్థితి Ligature:              hb_barrier (); return_trace (u.ligature.dispatch (c, std::forward<Ts> (ds)...));
    స్థితి Context:               hb_barrier (); return_trace (u.context.dispatch (c, std::forward<Ts> (ds)...));
    స్థితి ChainContext:          hb_barrier (); return_trace (u.chainContext.dispatch (c, std::forward<Ts> (ds)...));
    స్థితి Extension:             hb_barrier (); return_trace (u.extension.dispatch (c, std::forward<Ts> (ds)...));
    స్థితి తిరగేయి_Chain_Single:    hb_barrier (); return_trace (u.తిరగేయి_Chain_Context_Single.dispatch (c, std::forward<Ts> (ds)...));
    అప్రమేయ:                    return_trace (c->default_return_విలువ ());
    }
  }

  తార్కిక intersects (స్థిరరాశి hb_సమితి_t *glyphs, గుర్తులేని పూర్ణాంకం lookup_type) స్థిరరాశి
  {
    hb_intersects_context_t c (glyphs);
    తిరిగి dispatch (&c, lookup_type);
  }
};


}
}
}

#అయితే_ముగింపు  /* HB_OT_LAYOUT_GSUB_SUBSTLOOKUPSUBTABLE_HH */
