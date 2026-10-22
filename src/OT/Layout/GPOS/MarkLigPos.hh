#నిర్వచితం_కాకపోతే OT_LAYOUT_GPOS_MARKLIGPOS_HH
#నిర్వచించు OT_LAYOUT_GPOS_MARKLIGPOS_HH

#అన్తర్భావ "MarkLigPosFormat1.hh"

నామాకాశ OT {
నామాకాశ Layout {
నామాకాశ GPOS_impl {

నిర్మాణం MarkLigPos
{
  రక్షిత:
  సంఘం {
  నిర్మాణం { HBUINT16 v; }		format;         /* Format identifier */
  MarkLigPosFormat1_2<SmallTypes>	format1;
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
  MarkLigPosFormat1_2<MediumTypes>	format2;
#అయితే_ముగింపు
  } u;

  బహిరంగ:
  మూస <రకం_పేరు context_t, రకం_పేరు ...Ts>
  రకం_పేరు context_t::return_t dispatch (context_t *c, Ts&&... ds) స్థిరరాశి
  {
    ఒకవేళ (unlikely (!c->may_dispatch (ఇది, &u.format.v))) తిరిగి c->no_dispatch_return_విలువ ();
    TRACE_DISPATCH (ఇది, u.format.v);
    ఎంపిక (u.format.v) {
    స్థితి 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
    స్థితి 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
#అయితే_ముగింపు
    అప్రమేయ:return_trace (c->default_return_విలువ ());
    }
  }
};

}
}
}

#అయితే_ముగింపు /* OT_LAYOUT_GPOS_MARKLIGPOS_HH */
