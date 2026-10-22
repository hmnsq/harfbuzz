#నిర్వచితం_కాకపోతే OT_LAYOUT_GPOS_POSLOOKUPSUBTABLE_HH
#నిర్వచించు OT_LAYOUT_GPOS_POSLOOKUPSUBTABLE_HH

#అన్తర్భావ "SinglePos.hh"
#అన్తర్భావ "PairPos.hh"
#అన్తర్భావ "CursivePos.hh"
#అన్తర్భావ "MarkBasePos.hh"
#అన్తర్భావ "MarkLigPos.hh"
#అన్తర్భావ "MarkMarkPos.hh"
#అన్తర్భావ "ContextPos.hh"
#అన్తర్భావ "ChainContextPos.hh"
#అన్తర్భావ "ExtensionPos.hh"

నామాకాశ OT {
నామాకాశ Layout {
నామాకాశ GPOS_impl {

నిర్మాణం Pos_Lookup_Sub_పట్టిక
{
  మిత్ర నిర్మాణం ::OT::Lookup;
  మిత్ర నిర్మాణం PosLookup;

  గణన Type {
    Single              = 1,
    Pair                = 2,
    Cursive             = 3,
    Mark_పునాది            = 4,
    MarkLig             = 5,
    MarkMark            = 6,
    Context             = 7,
    ChainContext        = 8,
    Extension           = 9
  };

  మూస <రకం_పేరు context_t, రకం_పేరు ...Ts>
  రకం_పేరు context_t::return_t dispatch (context_t *c, గుర్తులేని పూర్ణాంకం lookup_type, Ts&&... ds) స్థిరరాశి
  {
    TRACE_DISPATCH (ఇది, lookup_type);
    ఎంపిక (lookup_type) {
    స్థితి Single:                hb_barrier (); return_trace (u.single.dispatch (c, std::forward<Ts> (ds)...));
    స్థితి Pair:                  hb_barrier (); return_trace (u.pair.dispatch (c, std::forward<Ts> (ds)...));
    స్థితి Cursive:               hb_barrier (); return_trace (u.cursive.dispatch (c, std::forward<Ts> (ds)...));
    స్థితి Mark_పునాది:              hb_barrier (); return_trace (u.mark_పునాది.dispatch (c, std::forward<Ts> (ds)...));
    స్థితి MarkLig:               hb_barrier (); return_trace (u.markLig.dispatch (c, std::forward<Ts> (ds)...));
    స్థితి MarkMark:              hb_barrier (); return_trace (u.markMark.dispatch (c, std::forward<Ts> (ds)...));
    స్థితి Context:               hb_barrier (); return_trace (u.context.dispatch (c, std::forward<Ts> (ds)...));
    స్థితి ChainContext:          hb_barrier (); return_trace (u.chainContext.dispatch (c, std::forward<Ts> (ds)...));
    స్థితి Extension:             hb_barrier (); return_trace (u.extension.dispatch (c, std::forward<Ts> (ds)...));
    అప్రమేయ:                    return_trace (c->default_return_విలువ ());
    }
  }

  తార్కిక intersects (స్థిరరాశి hb_సమితి_t *glyphs, గుర్తులేని పూర్ణాంకం lookup_type) స్థిరరాశి
  {
    hb_intersects_context_t c (glyphs);
    తిరిగి dispatch (&c, lookup_type);
  }

  రక్షిత:
  సంఘం {
  SinglePos             single;
  PairPos               pair;
  CursivePos            cursive;
  Mark_పునాది_Pos           mark_పునాది;
  MarkLigPos            markLig;
  MarkMarkPos           markMark;
  ContextPos            context;
  ChainContextPos       chainContext;
  ExtensionPos          extension;
  } u;
  బహిరంగ:
  DEFINE_కొలత_MIN (0);
};

}
}
}

#అయితే_ముగింపు  /* HB_OT_LAYOUT_GPOS_POSLOOKUPSUBTABLE_HH */
