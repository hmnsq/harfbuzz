#నిర్వచితం_కాకపోతే OT_LAYOUT_GSUB_LIGATURESUBST_HH
#నిర్వచించు OT_LAYOUT_GSUB_LIGATURESUBST_HH

#అన్తర్భావ "Common.hh"
#అన్తర్భావ "LigatureSubstFormat1.hh"

నామాకాశ OT {
నామాకాశ Layout {
నామాకాశ GSUB_impl {

నిర్మాణం LigatureSubst
{
  రక్షిత:
  సంఘం {
  నిర్మాణం { HBUINT16 v; }		format;         /* Format identifier */
  LigatureSubstFormat1_2<SmallTypes>	format1;
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
  LigatureSubstFormat1_2<MediumTypes>	format2;
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

  /* TODO This function should be done using iterators. While at it perhaps
   * using iterator of arrays of hb_codepoint_t instead. */
  మూస <రకం_పేరు GlyphID>
  తార్కిక serialize (hb_serialize_context_t *c,
                  hb_sorted_array_t<GlyphID> మొదటి_glyphs,
                  hb_array_t<స్థిరరాశి గుర్తులేని పూర్ణాంకం> ligature_చొప్పున_మొదటి_glyph_లెక్క_list,
                  hb_array_t<GlyphID> ligatures_list,
                  hb_array_t<స్థిరరాశి గుర్తులేని పూర్ణాంకం> component_లెక్క_list,
                  hb_array_t<GlyphID> component_list /* Starting from second for each ligature */)
  {
    TRACE_SERIALIZE (ఇది);
    ఒకవేళ (unlikely (!c->extend_min (u.format.v))) return_trace (అబద్ధం);
    గుర్తులేని పూర్ణాంకం format = 1;
    u.format.v = format;
    ఎంపిక (u.format.v) {
    స్థితి 1: hb_barrier (); return_trace (u.format1.serialize (c,
                                               మొదటి_glyphs,
                                               ligature_చొప్పున_మొదటి_glyph_లెక్క_list,
                                               ligatures_list,
                                               component_లెక్క_list,
                                               component_list));
    అప్రమేయ:return_trace (అబద్ధం);
    }
  }

  /* TODO subset() should choose format. */

};


}
}
}

#అయితే_ముగింపు  /* OT_LAYOUT_GSUB_LIGATURESUBST_HH */
