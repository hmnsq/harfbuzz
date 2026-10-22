#నిర్వచితం_కాకపోతే OT_LAYOUT_GPOS_SINGLEPOS_HH
#నిర్వచించు OT_LAYOUT_GPOS_SINGLEPOS_HH

#అన్తర్భావ "SinglePosFormat1.hh"
#అన్తర్భావ "SinglePosFormat2.hh"

నామాకాశ OT {
నామాకాశ Layout {
నామాకాశ GPOS_impl {

నిర్మాణం SinglePos
{
  రక్షిత:
  సంఘం {
  నిర్మాణం { HBUINT16 v; } format;        /* Format identifier */
  SinglePosFormat1_3<SmallTypes> format1;
  SinglePosFormat2_4<SmallTypes> format2;
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
  SinglePosFormat1_3<MediumTypes> format3;
  SinglePosFormat2_4<MediumTypes> format4;
#అయితే_ముగింపు
  } u;

  బహిరంగ:
  మూస<రకం_పేరు Iterator,
           hb_requires (hb_is_iterator (Iterator))>
  గుర్తులేని get_format (Iterator glyph_val_iter_జతలు)
  {
    hb_array_t<స్థిరరాశి విలువ> మొదటి_val_iter = hb_రెండవ (*glyph_val_iter_జతలు);

    కొరకు (స్థిరరాశి స్వయంచాలక iter : glyph_val_iter_జతలు)
      కొరకు (స్థిరరాశి స్వయంచాలక _ : hb_zip (iter.second, మొదటి_val_iter))
        ఒకవేళ (_.మొదటి != _.second)
          తిరిగి 2;

    తిరిగి 1;
  }

  మూస<రకం_పేరు Iterator,
      hb_requires (hb_is_iterator (Iterator))>
  శూన్య serialize (hb_serialize_context_t *c,
                  స్థిరరాశి విలువ_పునాది* src,
                  విలువ_Format src_format,
                  Iterator glyph_val_iter_జతలు,
                  స్థిరరాశి hb_hashmap_t<గుర్తులేని, hb_pair_t<గుర్తులేని, పూర్ణాంకం>> *layout_variation_idx_delta_map,
                  గుర్తులేని newFormat)
  {
    ఒకవేళ (unlikely (!c->extend_min (u.format.v))) తిరిగి;
    గుర్తులేని format = 2;
    విలువ_Format new_format;
    new_format = newFormat;

    ఒకవేళ (glyph_val_iter_జతలు)
      format = get_format (glyph_val_iter_జతలు);

#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
    ఒకవేళ (+ glyph_val_iter_జతలు
	| hb_map_retains_క్రమీకరణ (hb_మొదటి)
	| hb_filter ([] (hb_codepoint_t gid) { తిరిగి gid > 0xFFFFu; }))
      format += 2;
#అయితే_ముగింపు

    u.format.v = format;
    ఎంపిక (u.format.v) {
    స్థితి 1: hb_barrier (); u.format1.serialize (c,
                                 src, src_format,
                                 glyph_val_iter_జతలు,
                                 new_format,
                                 layout_variation_idx_delta_map);
      తిరిగి;
    స్థితి 2: hb_barrier (); u.format2.serialize (c,
                                 src, src_format,
                                 glyph_val_iter_జతలు,
                                 new_format,
                                 layout_variation_idx_delta_map);
      తిరిగి;
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
    స్థితి 3: u.format3.serialize (c,
                                 src, src_format,
                                 glyph_val_iter_జతలు,
                                 new_format,
                                 layout_variation_idx_delta_map);
      తిరిగి;
    స్థితి 4: u.format4.serialize (c,
                                 src, src_format,
                                 glyph_val_iter_జతలు,
                                 new_format,
                                 layout_variation_idx_delta_map);
      తిరిగి;
#అయితే_ముగింపు
    అప్రమేయ:తిరిగి;
    }
  }

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
};


మూస<రకం_పేరు Iterator>
స్థిర శూన్య
SinglePos_serialize (hb_serialize_context_t *c,
                     స్థిరరాశి విలువ_పునాది *src,
                     విలువ_Format src_format,
                     Iterator it,
                     స్థిరరాశి hb_hashmap_t<గుర్తులేని, hb_pair_t<గుర్తులేని, పూర్ణాంకం>> *layout_variation_idx_delta_map,
                     గుర్తులేని new_format)
{ c->ప్రారంభం_embed<SinglePos> ()->serialize (c, src, src_format, it, layout_variation_idx_delta_map, new_format); }


}
}
}

#అయితే_ముగింపు /* OT_LAYOUT_GPOS_SINGLEPOS_HH */
