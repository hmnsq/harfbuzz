#నిర్వచితం_కాకపోతే OT_LAYOUT_GPOS_SINGLEPOSFORMAT2_HH
#నిర్వచించు OT_LAYOUT_GPOS_SINGLEPOSFORMAT2_HH

#అన్తర్భావ "Common.hh"

నామాకాశ OT {
నామాకాశ Layout {
నామాకాశ GPOS_impl {

/* Keep the iterator type independent of the source table width so
 * the output serializers can share their instantiations. */
నిర్మాణం single_pos_విలువ_mapper_t
{
  స్థిరరాశి hb_map_t &glyph_map;
  స్థిరరాశి hb_array_t<స్థిరరాశి విలువ> &values;
  స్థిరరాశి గుర్తులేని &sub_నిడివి;

  hb_pair_t<hb_codepoint_t, hb_array_t<స్థిరరాశి విలువ>>
  క్రియాకారకం () (స్థిరరాశి hb_pair_t<hb_codepoint_t, గుర్తులేని> &_) స్థిరరాశి
  {
    తిరిగి hb_pair (glyph_map[_.మొదటి],
                    values.sub_array (_.second * sub_నిడివి, sub_నిడివి));
  }
};

మూస <రకం_పేరు Types>
నిర్మాణం SinglePosFormat2_4 : విలువ_పునాది
{
  రక్షిత:
  HBUINT16      format;                 /* Format identifier--format = 2 */
  రకం_పేరు Types::మూస LOffset_గమ్యం<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of subtable */
  విలువ_Format   విలువ_Format_2;            /* Defines the types of data in the
                                         * ValueRecord */
  రకం_పేరు Types::HBUINT
                విలువ_లెక్క;             /* Number of ValueRecords */
  విలువ_రికార్డు   values;                 /* Array of ValueRecords--positioning
                                         * values applied to glyphs */
  బహిరంగ:
  DEFINE_కొలత_ARRAY (2 + Types::LOffset::static_కొలత + విలువ_Format::static_కొలత + Types::HBUINT::static_కొలత, values);

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది) &&
                  coverage.sanitize (c, ఇది) &&
                  విలువ_Format_2.sanitize_విలువలు (c, ఇది, values, విలువ_లెక్క));
  }

  తార్కిక intersects (స్థిరరాశి hb_సమితి_t *glyphs) స్థిరరాశి
  { తిరిగి (ఇది+coverage).intersects (glyphs); }

  శూన్య సంవృతం_lookups (hb_సంవృతం_lookups_context_t *c) స్థిరరాశి {}
  శూన్య collect_variation_indices (hb_collect_variation_indices_context_t *c) స్థిరరాశి
  {
    ఒకవేళ (!విలువ_Format_2.has_device ()) తిరిగి;

    స్వయంచాలక it =
    + hb_zip (ఇది+coverage, hb_range ((గుర్తులేని) విలువ_లెక్క))
    | hb_filter (c->glyph_సమితి, hb_మొదటి)
    ;

    ఒకవేళ (!it) తిరిగి;

    గుర్తులేని sub_నిడివి = విలువ_Format_2.get_len ();
    స్థిరరాశి hb_array_t<స్థిరరాశి విలువ> విలువలు_array = values.as_array (విలువ_లెక్క * sub_నిడివి);

    కొరకు (గుర్తులేని i : + it
                      | hb_map (hb_రెండవ))
      విలువ_Format_2.collect_variation_indices (c, ఇది, విలువలు_array.sub_array (i * sub_నిడివి, sub_నిడివి));

  }

  శూన్య collect_glyphs (hb_collect_glyphs_context_t *c) స్థిరరాశి
  { ఒకవేళ (unlikely (!(ఇది+coverage).collect_coverage (c->ఆదానం))) తిరిగి; }

  స్థిరరాశి Coverage &get_coverage () స్థిరరాశి { తిరిగి ఇది+coverage; }

  విలువ_Format get_విలువ_format () స్థిరరాశి { తిరిగి విలువ_Format_2; }

  తార్కిక apply (hb_ot_apply_context_t *c) స్థిరరాశి
  {
    TRACE_APPLY (ఇది);
    hb_బఫరు_t *బఫరు = c->బఫరు;
    గుర్తులేని పూర్ణాంకం సూచీ_2 = (ఇది+coverage).get_coverage  (బఫరు->cur().codepoint);
    ఒకవేళ (సూచీ_2 == NOT_COVERED) return_trace (అబద్ధం);

    ఒకవేళ (unlikely (సూచీ_2 >= విలువ_లెక్క)) return_trace (అబద్ధం);

    ఒకవేళ (HB_బఫరు_సందేశం_మరిన్ని && c->బఫరు->messaging ())
    {
      c->బఫరు->సందేశం (c->font,
			  "positioning glyph at %u",
			  c->బఫరు->idx);
    }

    విలువ_Format_2.apply_విలువ (c, ఇది,
                             &values[సూచీ_2 * విలువ_Format_2.get_len ()],
                             బఫరు->cur_pos());

    ఒకవేళ (HB_బఫరు_సందేశం_మరిన్ని && c->బఫరు->messaging ())
    {
      c->బఫరు->సందేశం (c->font,
			  "positioned glyph at %u",
			  c->బఫరు->idx);
    }

    బఫరు->idx++;
    return_trace (నిజం);
  }

  తార్కిక
  position_single (hb_font_t           *font,
		   hb_blob_t           *పట్టిక_blob,
		   hb_direction_t       direction,
		   hb_codepoint_t       gid,
		   hb_glyph_position_t &pos) స్థిరరాశి
  {
    గుర్తులేని పూర్ణాంకం సూచీ_2 = (ఇది+coverage).get_coverage  (gid);
    ఒకవేళ (likely (సూచీ_2 == NOT_COVERED)) తిరిగి అబద్ధం;
    ఒకవేళ (unlikely (సూచీ_2 >= విలువ_లెక్క)) తిరిగి అబద్ధం;

    విలువ_Format_2.position_single (font, పట్టిక_blob, direction, ఇది,
                                 &values[సూచీ_2 * విలువ_Format_2.get_len ()], pos);
    తిరిగి నిజం;
  }


  మూస<రకం_పేరు Iterator,
      hb_requires (hb_is_iterator (Iterator))>
  శూన్య serialize (hb_serialize_context_t *c,
                  స్థిరరాశి విలువ_పునాది *src,
                  విలువ_Format src_format,
                  Iterator it,
                  విలువ_Format newFormat,
                  స్థిరరాశి hb_hashmap_t<గుర్తులేని, hb_pair_t<గుర్తులేని, పూర్ణాంకం>> *layout_variation_idx_delta_map)
  {
    స్వయంచాలక బహిర్గతం = c->extend_min (ఇది);
    ఒకవేళ (unlikely (!బహిర్గతం)) తిరిగి;
    ఒకవేళ (unlikely (!c->check_కేటాయించు (విలువ_Format_2, newFormat, HB_SERIALIZE_ERROR_INT_అధికప్రవాహం))) తిరిగి;
    ఒకవేళ (unlikely (!c->check_కేటాయించు (విలువ_లెక్క, it.len (), HB_SERIALIZE_ERROR_ARRAY_అధికప్రవాహం))) తిరిగి;

    + it
    | hb_map (hb_రెండవ)
    | hb_apply ([&] (hb_array_t<స్థిరరాశి విలువ> _)
    { src_format.copy_విలువలు (c, newFormat, src, &_, layout_variation_idx_delta_map); })
    ;

    స్వయంచాలక glyphs =
    + it
    | hb_map_retains_క్రమీకరణ (hb_మొదటి)
    ;

    coverage.serialize_serialize (c, glyphs);
  }

  మూస<రకం_పేరు Iterator,
      hb_requires (hb_is_iterator (Iterator))>
  గుర్తులేని compute_effective_format (స్థిరరాశి hb_face_t *face,
                                     Iterator it,
                                     తార్కిక is_instancing, తార్కిక strip_hints,
                                     తార్కిక has_gdef_varstore,
                                     స్థిరరాశి hb_hashmap_t<గుర్తులేని, hb_pair_t<గుర్తులేని, పూర్ణాంకం>> *varidx_delta_map) స్థిరరాశి
  {
    hb_blob_t* blob = hb_face_reference_పట్టిక (face, HB_ట్యాగ్ ('f','v','a','r'));
    తార్కిక has_fvar = (blob != hb_blob_get_ఖాళీలు ());
    hb_blob_destroy (blob);

    గుర్తులేని new_format = 0;
    ఒకవేళ (is_instancing)
    {
      new_format = new_format | విలువ_Format_2.get_effective_format (+ it | hb_map (hb_రెండవ), అబద్ధం, అబద్ధం, ఇది, varidx_delta_map);
    }
    /* do not strip hints for VF */
    లేకపోతే ఒకవేళ (strip_hints)
    {
      తార్కిక strip = !has_fvar;
      ఒకవేళ (has_fvar && !has_gdef_varstore)
        strip = నిజం;
      new_format = new_format | విలువ_Format_2.get_effective_format (+ it | hb_map (hb_రెండవ), strip, నిజం, ఇది, శూన్యసూచిక);
    }
    లేకపోతే
      new_format = విలువ_Format_2;

    తిరిగి new_format;
  }

  తార్కిక subset (hb_subset_context_t *c) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    స్థిరరాశి hb_సమితి_t &glyphset = *c->plan->glyphset_gsub ();
    స్థిరరాశి hb_map_t &glyph_map = *c->plan->glyph_map;

    గుర్తులేని sub_నిడివి = విలువ_Format_2.get_len ();
    స్వయంచాలక విలువలు_array = values.as_array ((గుర్తులేని) విలువ_లెక్క * sub_నిడివి);

    స్వయంచాలక it =
    + hb_zip (ఇది+coverage, hb_range ((గుర్తులేని) విలువ_లెక్క))
    | hb_filter (glyphset, hb_మొదటి)
    | hb_map_retains_క్రమీకరణ (single_pos_విలువ_mapper_t {glyph_map,
                                                       విలువలు_array,
                                                       sub_నిడివి})
    ;

    గుర్తులేని new_format = compute_effective_format (c->plan->ఆధారం, it,
                                                    తార్కిక (c->plan->normalized_coords),
                                                    తార్కిక (c->plan->flags & HB_SUBSET_FLAGS_NO_HINTING),
                                                    c->plan->has_gdef_varstore,
                                                    &c->plan->layout_variation_idx_delta_map);
    తార్కిక ret = తార్కిక (it);
    SinglePos_serialize (c->serializer, ఇది, విలువ_Format_2, it, &c->plan->layout_variation_idx_delta_map, new_format);
    return_trace (ret);
  }
};


}
}
}

#అయితే_ముగింపు /* OT_LAYOUT_GPOS_SINGLEPOSFORMAT2_HH */
