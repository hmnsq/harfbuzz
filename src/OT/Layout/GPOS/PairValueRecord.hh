#నిర్వచితం_కాకపోతే OT_LAYOUT_GPOS_PAIRVALUERECORD_HH
#నిర్వచించు OT_LAYOUT_GPOS_PAIRVALUERECORD_HH

#అన్తర్భావ "ValueFormat.hh"

నామాకాశ OT {
నామాకాశ Layout {
నామాకాశ GPOS_impl {


నిర్మాణం pair_విలువ_రికార్డు_context_t
{
  స్థిరరాశి విలువ_పునాది     *పునాది;
  స్థిరరాశి విలువ_Format   *విలువ_Formats;
  స్థిరరాశి విలువ_Format   *newFormats;
  గుర్తులేని            len1; /* valueFormats[0].get_len() */
  స్థిరరాశి hb_map_t      *glyph_map;
  స్థిరరాశి hb_hashmap_t<గుర్తులేని, hb_pair_t<గుర్తులేని, పూర్ణాంకం>> *layout_variation_idx_delta_map;

  శూన్య copy_విలువలు (hb_serialize_context_t *s, స్థిరరాశి విలువ *values) స్థిరరాశి
  {
    విలువ_Formats[0].copy_విలువలు (s, newFormats[0], పునాది, values,
                                layout_variation_idx_delta_map);
    విలువ_Formats[1].copy_విలువలు (s, newFormats[1], పునాది, values + len1,
                                layout_variation_idx_delta_map);
  }
};

మూస <రకం_పేరు Types>
నిర్మాణం Pair_విలువ_రికార్డు
{
  మూస <రకం_పేరు Types2>
  మిత్ర నిర్మాణం Pair_సమితి;

  రక్షిత:
  రకం_పేరు Types::HBGlyphID
	        రెండవ_Glyph;            /* GlyphID of second glyph in the
                                         * pair--first glyph is listed in the
                                         * Coverage table */
  విలువ_రికార్డు   values;                 /* Positioning data for the first glyph
                                         * followed by for second glyph */
  బహిరంగ:
  DEFINE_కొలత_ARRAY (Types::HBGlyphID::static_కొలత, values);

  పూర్ణాంకం cmp (hb_codepoint_t k) స్థిరరాశి
  { తిరిగి రెండవ_Glyph.cmp (k); }

  తార్కిక subset (hb_subset_context_t *c,
               pair_విలువ_రికార్డు_context_t *సంవృతం) స్థిరరాశి
  {
    TRACE_SERIALIZE (ఇది);
    స్వయంచాలక *s = c->serializer;
    స్వయంచాలక *బహిర్గతం = s->ప్రారంభం_embed (*ఇది);
    ఒకవేళ (unlikely (!s->extend_min (బహిర్గతం))) return_trace (అబద్ధం);

    బహిర్గతం->రెండవ_Glyph = (*సంవృతం->glyph_map)[రెండవ_Glyph];

    సంవృతం->copy_విలువలు (s, values);

    return_trace (నిజం);
  }

  శూన్య collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  స్థిరరాశి విలువ_Format *విలువ_Formats,
                                  స్థిరరాశి విలువ_పునాది *పునాది) స్థిరరాశి
  {
    గుర్తులేని record1_len = విలువ_Formats[0].get_len ();
    గుర్తులేని record2_len = విలువ_Formats[1].get_len ();
    స్థిరరాశి hb_array_t<స్థిరరాశి విలువ> విలువలు_array = values.as_array (record1_len + record2_len);

    ఒకవేళ (విలువ_Formats[0].has_device ())
      విలువ_Formats[0].collect_variation_indices (c, పునాది, విలువలు_array.sub_array (0, record1_len));

    ఒకవేళ (విలువ_Formats[1].has_device ())
      విలువ_Formats[1].collect_variation_indices (c, పునాది, విలువలు_array.sub_array (record1_len, record2_len));
  }

  తార్కిక intersects (స్థిరరాశి hb_సమితి_t& glyphset) స్థిరరాశి
  {
    తిరిగి glyphset.has(రెండవ_Glyph);
  }

  స్థిరరాశి విలువ* get_విలువలు_1 () స్థిరరాశి
  {
    తిరిగి &values[0];
  }

  స్థిరరాశి విలువ* get_విలువలు_2 (విలువ_Format format1) స్థిరరాశి
  {
    తిరిగి &values[format1.get_len ()];
  }
};


}
}
}

#అయితే_ముగింపు  // OT_LAYOUT_GPOS_PAIRVALUERECORD_HH
