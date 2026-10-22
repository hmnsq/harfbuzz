#నిర్వచితం_కాకపోతే OT_LAYOUT_GPOS_SINGLEPOSFORMAT1_HH
#నిర్వచించు OT_LAYOUT_GPOS_SINGLEPOSFORMAT1_HH

#అన్తర్భావ "Common.hh"
#అన్తర్భావ "ValueFormat.hh"

నామాకాశ OT {
నామాకాశ Layout {
నామాకాశ GPOS_impl {

మూస <రకం_పేరు Types>
నిర్మాణం SinglePosFormat1_3 : విలువ_పునాది
{
  రక్షిత:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  రకం_పేరు Types::మూస LOffset_గమ్యం<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of subtable */
  విలువ_Format   విలువ_Format_2;            /* Defines the types of data in the
                                         * ValueRecord */
  విలువ_రికార్డు   values;                 /* Defines positioning
                                         * value(s)--applied to all glyphs in
                                         * the Coverage table */
  బహిరంగ:
  DEFINE_కొలత_ARRAY (2 + Types::LOffset::static_కొలత + విలువ_Format::static_కొలత, values);

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది) &&
                  coverage.sanitize (c, ఇది) &&
		  hb_barrier () &&
                  /* The coverage  table may use a range to represent a set
                   * of glyphs, which means a small number of bytes can
                   * generate a large glyph set. Manually modify the
                   * sanitizer max ops to take this into account.
                   *
                   * Note: This check *must* be right after coverage sanitize. */
                  c->check_ops ((ఇది + coverage).get_population () >> 1) &&
                  విలువ_Format_2.sanitize_విలువ (c, ఇది, values));

  }

  తార్కిక intersects (స్థిరరాశి hb_సమితి_t *glyphs) స్థిరరాశి
  { తిరిగి (ఇది+coverage).intersects (glyphs); }

  శూన్య సంవృతం_lookups (hb_సంవృతం_lookups_context_t *c) స్థిరరాశి {}
  శూన్య collect_variation_indices (hb_collect_variation_indices_context_t *c) స్థిరరాశి
  {
    ఒకవేళ (!విలువ_Format_2.has_device ()) తిరిగి;

    hb_సమితి_t intersection;
    (ఇది+coverage).intersect_సమితి (*c->glyph_సమితి, intersection);
    ఒకవేళ (!intersection) తిరిగి;

    విలువ_Format_2.collect_variation_indices (c, ఇది, values.as_array (విలువ_Format_2.get_len ()));
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

    ఒకవేళ (HB_బఫరు_సందేశం_మరిన్ని && c->బఫరు->messaging ())
    {
      c->బఫరు->సందేశం (c->font,
			  "positioning glyph at %u",
			  c->బఫరు->idx);
    }

    విలువ_Format_2.apply_విలువ (c, ఇది, values, బఫరు->cur_pos());

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

    విలువ_Format_2.position_single (font, పట్టిక_blob, direction, ఇది,
                                 values, pos);
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
    ఒకవేళ (unlikely (!c->extend_min (ఇది))) తిరిగి;
    ఒకవేళ (unlikely (!c->check_కేటాయించు (విలువ_Format_2,
                                    newFormat,
                                    HB_SERIALIZE_ERROR_INT_అధికప్రవాహం))) తిరిగి;

    కొరకు (స్థిరరాశి hb_array_t<స్థిరరాశి విలువ>& _ : + it | hb_map (hb_రెండవ))
    {
      src_format.copy_విలువలు (c, newFormat, src,  &_, layout_variation_idx_delta_map);
      // Only serialize the first entry in the iterator, the rest are assumed to
      // be the same.
      ఆపు;
    }

    స్వయంచాలక glyphs =
    + it
    | hb_map_retains_క్రమీకరణ (hb_మొదటి)
    ;

    coverage.serialize_serialize (c, glyphs);
  }

  తార్కిక subset (hb_subset_context_t *c) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    స్థిరరాశి hb_సమితి_t &glyphset = *c->plan->glyphset_gsub ();
    స్థిరరాశి hb_map_t &glyph_map = *c->plan->glyph_map;

    hb_సమితి_t intersection;
    (ఇది+coverage).intersect_సమితి (glyphset, intersection);

    గుర్తులేని new_format = విలువ_Format_2;

    ఒకవేళ (c->plan->normalized_coords)
    {
      new_format = విలువ_Format_2.get_effective_format (values.arrayZ, అబద్ధం, అబద్ధం, ఇది, &c->plan->layout_variation_idx_delta_map);
    }
    /* do not strip hints for VF */
    లేకపోతే ఒకవేళ (c->plan->flags & HB_SUBSET_FLAGS_NO_HINTING)
    {
      hb_blob_t* blob = hb_face_reference_పట్టిక (c->plan->ఆధారం, HB_ట్యాగ్ ('f','v','a','r'));
      తార్కిక has_fvar = (blob != hb_blob_get_ఖాళీలు ());
      hb_blob_destroy (blob);

      తార్కిక strip = !has_fvar;
      /* special case: strip hints when a VF has no GDEF varstore after
       * subsetting*/
      ఒకవేళ (has_fvar && !c->plan->has_gdef_varstore)
        strip = నిజం;
      new_format = విలువ_Format_2.get_effective_format (values.arrayZ,
                                                     strip, /* strip hints */
                                                     నిజం, /* strip empty */
                                                     ఇది, శూన్యసూచిక);
    }

    స్వయంచాలక it =
    + hb_iter (intersection)
    | hb_map_retains_క్రమీకరణ (glyph_map)
    | hb_zip (hb_repeat (values.as_array (విలువ_Format_2.get_len ())))
    ;

    తార్కిక ret = తార్కిక (it);
    SinglePos_serialize (c->serializer, ఇది, విలువ_Format_2, it, &c->plan->layout_variation_idx_delta_map, new_format);
    return_trace (ret);
  }
};

}
}
}

#అయితే_ముగింపు /* OT_LAYOUT_GPOS_SINGLEPOSFORMAT1_HH */
