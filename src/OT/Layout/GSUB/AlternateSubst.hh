#నిర్వచితం_కాకపోతే OT_LAYOUT_GSUB_ALTERNATESUBST_HH
#నిర్వచించు OT_LAYOUT_GSUB_ALTERNATESUBST_HH

#అన్తర్భావ "AlternateSubstFormat1.hh"
#అన్తర్భావ "Common.hh"

నామాకాశ OT {
నామాకాశ Layout {
నామాకాశ GSUB_impl {

నిర్మాణం AlternateSubst
{
  రక్షిత:
  సంఘం {
  నిర్మాణం { HBUINT16 v; }		format;         /* Format identifier */
  AlternateSubstFormat1_2<SmallTypes>	format1;
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
  AlternateSubstFormat1_2<MediumTypes>	format2;
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

  /* TODO This function is unused and should be done using iterators. While
   * at it perhaps using iterator of arrays of hb_codepoint_t instead. */
  మూస <రకం_పేరు GlyphID>
  తార్కిక serialize (hb_serialize_context_t *c,
                  hb_sorted_array_t<GlyphID> glyphs,
                  hb_array_t<స్థిరరాశి గుర్తులేని పూర్ణాంకం> alternate_len_list,
                  hb_array_t<GlyphID> alternate_glyphs_list)
  {
    TRACE_SERIALIZE (ఇది);
    ఒకవేళ (unlikely (!c->extend_min (u.format.v))) return_trace (అబద్ధం);
    గుర్తులేని పూర్ణాంకం format = 1;
    u.format.v = format;
    ఎంపిక (u.format.v) {
    స్థితి 1: hb_barrier (); return_trace (u.format1.serialize (c, glyphs, alternate_len_list, alternate_glyphs_list));
    అప్రమేయ:return_trace (అబద్ధం);
    }
  }

  /* TODO subset() should choose format. */

};

}
}
}

#అయితే_ముగింపు  /* OT_LAYOUT_GSUB_ALTERNATESUBST_HH */
