#నిర్వచితం_కాకపోతే OT_LAYOUT_GSUB_SINGLESUBST_HH
#నిర్వచించు OT_LAYOUT_GSUB_SINGLESUBST_HH

#అన్తర్భావ "Common.hh"
#అన్తర్భావ "SingleSubstFormat1.hh"
#అన్తర్భావ "SingleSubstFormat2.hh"

నామాకాశ OT {
నామాకాశ Layout {
నామాకాశ GSUB_impl {

నిర్మాణం SingleSubst
{
  రక్షిత:
  సంఘం {
  నిర్మాణం { HBUINT16 v; }		format;         /* Format identifier */
  SingleSubstFormat1_3<SmallTypes>	format1;
  SingleSubstFormat2_4<SmallTypes>	format2;
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
  SingleSubstFormat1_3<MediumTypes>	format3;
  SingleSubstFormat2_4<MediumTypes>	format4;
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
    స్థితి 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
    స్థితి 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
    స్థితి 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
#అయితే_ముగింపు
    అప్రమేయ:return_trace (c->default_return_విలువ ());
    }
  }

  మూస<రకం_పేరు Iterator,
           hb_requires (hb_is_sorted_ఆధారం_యొక్క (Iterator,
                                                స్థిరరాశి hb_codepoint_pair_t))>
  తార్కిక serialize (hb_serialize_context_t *c,
                  Iterator glyphs)
  {
    TRACE_SERIALIZE (ఇది);
    ఒకవేళ (unlikely (!c->extend_min (u.format.v))) return_trace (అబద్ధం);
    గుర్తులేని format = 2;
    గుర్తులేని delta = 0;
    ఒకవేళ (glyphs)
    {
      format = 1;
      hb_codepoint_t ముసుగు = 0xFFFFu;

#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
       ఒకవేళ (+ glyphs
	   | hb_map_retains_క్రమీకరణ (hb_రెండవ)
	   | hb_filter ([] (hb_codepoint_t gid) { తిరిగి gid > 0xFFFFu; }))
       {
	 format += 2;
	 ముసుగు = 0xFFFFFFu;
       }
#అయితే_ముగింపు

      స్వయంచాలక get_delta = [=] (hb_codepoint_pair_t _)
                       { తిరిగి (గుర్తులేని) (_.second - _.మొదటి) & ముసుగు; };
      delta = get_delta (*glyphs);
      ఒకవేళ (!hb_all (++(+glyphs), delta, get_delta)) format += 1;
    }

    u.format.v = format;
    ఎంపిక (u.format.v) {
    స్థితి 1: hb_barrier (); return_trace (u.format1.serialize (c,
                                               + glyphs
                                               | hb_map_retains_క్రమీకరణ (hb_మొదటి),
                                               delta));
    స్థితి 2: hb_barrier (); return_trace (u.format2.serialize (c, glyphs));
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
    స్థితి 3: hb_barrier (); return_trace (u.format3.serialize (c,
                                               + glyphs
                                               | hb_map_retains_క్రమీకరణ (hb_మొదటి),
                                               delta));
    స్థితి 4: hb_barrier (); return_trace (u.format4.serialize (c, glyphs));
#అయితే_ముగింపు
    అప్రమేయ:return_trace (అబద్ధం);
    }
  }
};

మూస<రకం_పేరు Iterator>
స్థిర శూన్య
SingleSubst_serialize (hb_serialize_context_t *c,
                       Iterator it)
{ c->ప్రారంభం_embed<SingleSubst> ()->serialize (c, it); }

}
}
}

#అయితే_ముగింపు /* OT_LAYOUT_GSUB_SINGLESUBST_HH */
