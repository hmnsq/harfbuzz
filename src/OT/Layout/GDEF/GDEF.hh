/*
 * Copyright © 2007,2008,2009  Red Hat, Inc.
 * Copyright © 2010,2011,2012  Google, Inc.
 *
 *  This is part of HarfBuzz, a text shaping library.
 *
 * Permission is hereby granted, without written agreement and without
 * license or royalty fees, to use, copy, modify, and distribute this
 * software and its documentation for any purpose, provided that the
 * above copyright notice and the following two paragraphs appear in
 * all copies of this software.
 *
 * IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE TO ANY PARTY FOR
 * DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES
 * ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN
 * IF THE COPYRIGHT HOLDER HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 *
 * THE COPYRIGHT HOLDER SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING,
 * BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS
 * ON AN "AS IS" BASIS, AND THE COPYRIGHT HOLDER HAS NO OBLIGATION TO
 * PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
 *
 * Red Hat Author(s): Behdad Esfahbod
 * Google Author(s): Behdad Esfahbod
 */

#నిర్వచితం_కాకపోతే OT_LAYOUT_GDEF_GDEF_HH
#నిర్వచించు OT_LAYOUT_GDEF_GDEF_HH

#అన్తర్భావ "../../../hb-ot-var-common.hh"

#అన్తర్భావ "../../../hb-font.hh"
#అన్తర్భావ "../../../hb-cache.hh"


నామాకాశ OT {


/*
 * Attachment List Table
 */

/* Array of contour point indices--in increasing numerical order */
నిర్మాణం Attach_బిందువు : Array16_యొక్క<HBUINT16>
{
  తార్కిక subset (hb_subset_context_t *c) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    స్వయంచాలక *బహిర్గతం = c->serializer->ప్రారంభం_embed (*ఇది);
    return_trace (బహిర్గతం->serialize (c->serializer, + iter ()));
  }
};

/* AttachList is referenced by both GDEF attachListOffset and
 * attachListOffset2. The v1.4 top-level offset widens, but the
 * AttachList table itself keeps Offset16 internals. */
నిర్మాణం AttachList
{
  గుర్తులేని పూర్ణాంకం get_attach_points (hb_codepoint_t glyph_id,
				  గుర్తులేని పూర్ణాంకం ప్రారంభం_స్థానభ్రంశం,
				  గుర్తులేని పూర్ణాంకం *బిందువు_లెక్క /* IN/OUT */,
				  గుర్తులేని పూర్ణాంకం *బిందువు_array /*బహిర్గతం*/) స్థిరరాశి
  {
    గుర్తులేని పూర్ణాంకం సూచీ_2 = (ఇది+coverage).get_coverage (glyph_id);
    ఒకవేళ (సూచీ_2 == NOT_COVERED)
    {
      ఒకవేళ (బిందువు_లెక్క)
	*బిందువు_లెక్క = 0;
      తిరిగి 0;
    }

    స్థిరరాశి Attach_బిందువు &points = ఇది+attach_బిందువు[సూచీ_2];

    ఒకవేళ (బిందువు_లెక్క && బిందువు_array)
    {
      + points.as_array ().sub_array (ప్రారంభం_స్థానభ్రంశం, బిందువు_లెక్క)
      | hb_sink (hb_array (బిందువు_array, *బిందువు_లెక్క))
      ;
    }

    తిరిగి points.len;
  }

  తార్కిక subset (hb_subset_context_t *c) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    స్థిరరాశి hb_సమితి_t &glyphset = *c->plan->glyphset_gsub ();
    స్థిరరాశి hb_map_t &glyph_map = *c->plan->glyph_map;

    స్వయంచాలక *బహిర్గతం = c->serializer->ప్రారంభం_embed (*ఇది);
    ఒకవేళ (unlikely (!c->serializer->extend_min (బహిర్గతం))) return_trace (అబద్ధం);

    hb_sorted_సదిశ_t<hb_codepoint_t> new_coverage;
    + hb_zip (ఇది+coverage, attach_బిందువు)
    | hb_filter (glyphset, hb_మొదటి)
    | hb_filter (subset_స్థానభ్రంశం_array (c, బహిర్గతం->attach_బిందువు, ఇది), hb_రెండవ)
    | hb_map (hb_మొదటి)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;
    బహిర్గతం->coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (తార్కిక (new_coverage));
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (coverage.sanitize (c, ఇది) && attach_బిందువు.sanitize (c, ఇది));
  }

  రక్షిత:
  Offset16_గమ్యం<Coverage>
		coverage;		/* Offset to Coverage table -- from
					 * beginning of AttachList table */
  Array16_యొక్క_Offset16_గమ్యం<Attach_బిందువు>
		attach_బిందువు;		/* Array of AttachPoint tables
					 * in Coverage Index order */
  బహిరంగ:
  DEFINE_కొలత_ARRAY (4, attach_బిందువు);
};

/*
 * Ligature Caret Table
 */

నిర్మాణం Caret_విలువ_Format1
{
  మిత్ర నిర్మాణం Caret_విలువ;
  తార్కిక subset (hb_subset_context_t *c) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    స్వయంచాలక *బహిర్గతం = c->serializer->embed (ఇది);
    ఒకవేళ (unlikely (!బహిర్గతం)) return_trace (అబద్ధం);
    return_trace (నిజం);
  }

  అంతరంగ:
  hb_position_t get_caret_విలువ (hb_font_t *font, hb_direction_t direction) స్థిరరాశి
  {
    తిరిగి HB_DIRECTION_IS_HORIZONTAL (direction) ? font->em_scale_x (coordinate) : font->em_scale_y (coordinate);
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది));
  }

  రక్షిత:
  HBUINT16	caret_విలువ_Format;	/* Format identifier--format = 1 */
  FWORD		coordinate;		/* X or Y value, in design units */
  బహిరంగ:
  DEFINE_కొలత_STATIC (4);
};

నిర్మాణం Caret_విలువ_Format2
{
  మిత్ర నిర్మాణం Caret_విలువ;
  తార్కిక subset (hb_subset_context_t *c) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    స్వయంచాలక *బహిర్గతం = c->serializer->embed (ఇది);
    ఒకవేళ (unlikely (!బహిర్గతం)) return_trace (అబద్ధం);
    return_trace (నిజం);
  }

  అంతరంగ:
  hb_position_t get_caret_విలువ (hb_font_t *font, hb_direction_t direction, hb_codepoint_t glyph_id) స్థిరరాశి
  {
    hb_position_t x, y;
    font->get_glyph_contour_బిందువు_for_origin (glyph_id, caret_విలువ_బిందువు, direction, &x, &y);
    తిరిగి HB_DIRECTION_IS_HORIZONTAL (direction) ? x : y;
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది));
  }

  రక్షిత:
  HBUINT16	caret_విలువ_Format;	/* Format identifier--format = 2 */
  HBUINT16	caret_విలువ_బిందువు;	/* Contour point index on glyph */
  బహిరంగ:
  DEFINE_కొలత_STATIC (4);
};

నిర్మాణం Caret_విలువ_Format3
{
  మిత్ర నిర్మాణం Caret_విలువ;

  hb_position_t get_caret_విలువ (hb_font_t *font, hb_direction_t direction,
				 స్థిరరాశి అంశం_Variation_Store &var_store) స్థిరరాశి
  {
    తిరిగి HB_DIRECTION_IS_HORIZONTAL (direction) ?
	   hb_saturate_కూడు (font->em_scale_x (coordinate), (ఇది+device_పట్టిక).get_x_delta (font, var_store)) :
	   hb_saturate_కూడు (font->em_scale_y (coordinate), (ఇది+device_పట్టిక).get_y_delta (font, var_store));
  }

  తార్కిక subset (hb_subset_context_t *c) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    స్వయంచాలక *బహిర్గతం = c->serializer->ప్రారంభం_embed (*ఇది);
    ఒకవేళ (!c->serializer->embed (caret_విలువ_Format)) return_trace (అబద్ధం);
    ఒకవేళ (!c->serializer->embed (coordinate)) return_trace (అబద్ధం);

    గుర్తులేని varidx = (ఇది+device_పట్టిక).get_variation_సూచీ ();
    hb_pair_t<గుర్తులేని, పూర్ణాంకం> *new_varidx_delta;
    ఒకవేళ (c->plan->layout_variation_idx_delta_map.has (varidx, &new_varidx_delta)) {
      uint32_t new_varidx = hb_మొదటి (*new_varidx_delta);
      పూర్ణాంకం delta = hb_రెండవ (*new_varidx_delta);
      ఒకవేళ (delta != 0)
      {
        ఒకవేళ (!c->serializer->check_కేటాయించు (బహిర్గతం->coordinate, coordinate + delta, HB_SERIALIZE_ERROR_INT_అధికప్రవాహం))
          return_trace (అబద్ధం);
      }

      ఒకవేళ (new_varidx == HB_OT_LAYOUT_NO_VARIATIONS_సూచీ)
        return_trace (c->serializer->check_కేటాయించు (బహిర్గతం->caret_విలువ_Format, 1, HB_SERIALIZE_ERROR_INT_అధికప్రవాహం));
    }

    ఒకవేళ (!c->serializer->embed (device_పట్టిక))
      return_trace (అబద్ధం);

    return_trace (బహిర్గతం->device_పట్టిక.serialize_copy (c->serializer, device_పట్టిక, ఇది, c->serializer->గమ్యం_bias (బహిర్గతం),
						   hb_serialize_context_t::తల, &c->plan->layout_variation_idx_delta_map));
  }

  శూన్య collect_variation_indices (hb_collect_variation_indices_context_t *c) స్థిరరాశి
  { (ఇది+device_పట్టిక).collect_variation_indices (c); }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది) && device_పట్టిక.sanitize (c, ఇది));
  }

  రక్షిత:
  HBUINT16	caret_విలువ_Format;	/* Format identifier--format = 3 */
  FWORD		coordinate;		/* X or Y value, in design units */
  Offset16_గమ్యం<Device>
		device_పట్టిక;		/* Offset to Device table for X or Y
					 * value--from beginning of CaretValue
					 * table */
  బహిరంగ:
  DEFINE_కొలత_STATIC (6);
};

నిర్మాణం Caret_విలువ
{
  hb_position_t get_caret_విలువ (hb_font_t *font,
				 hb_direction_t direction,
				 hb_codepoint_t glyph_id,
				 స్థిరరాశి అంశం_Variation_Store &var_store) స్థిరరాశి
  {
    ఎంపిక (u.format.v) {
    స్థితి 1: hb_barrier (); తిరిగి u.format1.get_caret_విలువ (font, direction);
    స్థితి 2: hb_barrier (); తిరిగి u.format2.get_caret_విలువ (font, direction, glyph_id);
    స్థితి 3: hb_barrier (); తిరిగి u.format3.get_caret_విలువ (font, direction, var_store);
    అప్రమేయ:తిరిగి 0;
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
    స్థితి 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
    అప్రమేయ:return_trace (c->default_return_విలువ ());
    }
  }

  శూన్య collect_variation_indices (hb_collect_variation_indices_context_t *c) స్థిరరాశి
  {
    ఎంపిక (u.format.v) {
    స్థితి 1:
    స్థితి 2:
      తిరిగి;
    స్థితి 3:
      hb_barrier ();
      u.format3.collect_variation_indices (c);
      తిరిగి;
    అప్రమేయ: తిరిగి;
    }
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    ఒకవేళ (!u.format.v.sanitize (c)) return_trace (అబద్ధం);
    hb_barrier ();
    ఎంపిక (u.format.v) {
    స్థితి 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    స్థితి 2: hb_barrier (); return_trace (u.format2.sanitize (c));
    స్థితి 3: hb_barrier (); return_trace (u.format3.sanitize (c));
    అప్రమేయ:return_trace (నిజం);
    }
  }

  రక్షిత:
  సంఘం {
  నిర్మాణం { HBUINT16 v; }	format;		/* Format identifier */
  Caret_విలువ_Format1	format1;
  Caret_విలువ_Format2	format2;
  Caret_విలువ_Format3	format3;
  } u;
  బహిరంగ:
  DEFINE_కొలత_UNION (2, format.v);
};

నిర్మాణం LigGlyph
{
  గుర్తులేని get_lig_carets (hb_font_t            *font,
			   hb_direction_t        direction,
			   hb_codepoint_t        glyph_id,
			   స్థిరరాశి అంశం_Variation_Store &var_store,
			   గుర్తులేని              ప్రారంభం_స్థానభ్రంశం,
			   గుర్తులేని             *caret_లెక్క /* IN/OUT */,
			   hb_position_t        *caret_array /*బహిర్గతం*/) స్థిరరాశి
  {
    ఒకవేళ (caret_లెక్క && caret_array)
    {
      + carets.as_array ().sub_array (ప్రారంభం_స్థానభ్రంశం, caret_లెక్క)
      | hb_map (hb_కూడు (ఇది))
      | hb_map ([&] (స్థిరరాశి Caret_విలువ &value) { తిరిగి value.get_caret_విలువ (font, direction, glyph_id, var_store); })
      | hb_sink (hb_array (caret_array, *caret_లెక్క))
      ;
    }

    తిరిగి carets.len;
  }

  తార్కిక subset (hb_subset_context_t *c) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    స్వయంచాలక *బహిర్గతం = c->serializer->ప్రారంభం_embed (*ఇది);
    ఒకవేళ (unlikely (!c->serializer->extend_min (బహిర్గతం))) return_trace (అబద్ధం);

    + hb_iter (carets)
    | hb_apply (subset_స్థానభ్రంశం_array (c, బహిర్గతం->carets, ఇది))
    ;

    return_trace (తార్కిక (బహిర్గతం->carets));
  }

  శూన్య collect_variation_indices (hb_collect_variation_indices_context_t *c) స్థిరరాశి
  {
    కొరకు (స్థిరరాశి Offset16_గమ్యం<Caret_విలువ>& స్థానభ్రంశం : carets.iter ())
      (ఇది+స్థానభ్రంశం).collect_variation_indices (c);
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (carets.sanitize (c, ఇది));
  }

  రక్షిత:
  Array16_యొక్క_Offset16_గమ్యం<Caret_విలువ>
		carets;			/* Offset array of CaretValue tables
					 * --from beginning of LigGlyph table
					 * --in increasing coordinate order */
  బహిరంగ:
  DEFINE_కొలత_ARRAY (2, carets);
};

/* LigCaretList2 widens the LigCaretList coverage, count, and LigGlyph
 * offsets. The LigGlyph table itself keeps Offset16 caretValueOffsets. */
మూస <రకం_పేరు Types>
నిర్మాణం LigCaretList
{
  గుర్తులేని పూర్ణాంకం get_lig_carets (hb_font_t *font,
			       hb_direction_t direction,
			       hb_codepoint_t glyph_id,
			       స్థిరరాశి అంశం_Variation_Store &var_store,
			       గుర్తులేని పూర్ణాంకం ప్రారంభం_స్థానభ్రంశం,
			       గుర్తులేని పూర్ణాంకం *caret_లెక్క /* IN/OUT */,
			       hb_position_t *caret_array /*బహిర్గతం*/) స్థిరరాశి
  {
    గుర్తులేని పూర్ణాంకం సూచీ_2 = (ఇది+coverage).get_coverage (glyph_id);
    ఒకవేళ (సూచీ_2 == NOT_COVERED)
    {
      ఒకవేళ (caret_లెక్క)
	*caret_లెక్క = 0;
      తిరిగి 0;
    }
    స్థిరరాశి LigGlyph &lig_glyph = ఇది+ligGlyph[సూచీ_2];
    తిరిగి lig_glyph.get_lig_carets (font, direction, glyph_id, var_store, ప్రారంభం_స్థానభ్రంశం, caret_లెక్క, caret_array);
  }

  తార్కిక subset (hb_subset_context_t *c) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    స్థిరరాశి hb_సమితి_t &glyphset = *c->plan->glyphset_gsub ();
    స్థిరరాశి hb_map_t &glyph_map = *c->plan->glyph_map;

    స్వయంచాలక *బహిర్గతం = c->serializer->ప్రారంభం_embed (*ఇది);
    ఒకవేళ (unlikely (!c->serializer->extend_min (బహిర్గతం))) return_trace (అబద్ధం);

    hb_sorted_సదిశ_t<hb_codepoint_t> new_coverage;
    + hb_zip (ఇది+coverage, ligGlyph)
    | hb_filter (glyphset, hb_మొదటి)
    | hb_filter (subset_స్థానభ్రంశం_array (c, బహిర్గతం->ligGlyph, ఇది), hb_రెండవ)
    | hb_map (hb_మొదటి)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;
    బహిర్గతం->coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (తార్కిక (new_coverage));
  }

  శూన్య collect_variation_indices (hb_collect_variation_indices_context_t *c) స్థిరరాశి
  {
    + hb_zip (ఇది+coverage, ligGlyph)
    | hb_filter (c->glyph_సమితి, hb_మొదటి)
    | hb_map (hb_రెండవ)
    | hb_map (hb_కూడు (ఇది))
    | hb_apply ([c] (స్థిరరాశి LigGlyph& _) { _.collect_variation_indices (c); })
    ;
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (coverage.sanitize (c, ఇది) && ligGlyph.sanitize (c, ఇది));
  }

  రక్షిత:
  రకం_పేరు Types::మూస LOffset_గమ్యం<Coverage>
		coverage;		/* Offset to Coverage table--from
					 * beginning of LigCaretList table */
  రకం_పేరు Types::మూస Array_యొక్క<రకం_పేరు Types::మూస స్థానభ్రంశం_గమ్యం<LigGlyph>>
		ligGlyph;		/* Array of LigGlyph tables
					 * in Coverage Index order */
  బహిరంగ:
  DEFINE_కొలత_ARRAY (Types::LOffset::static_కొలత + Types::HBUINT::static_కొలత, ligGlyph);
};

#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
static_assert (LigCaretList<MediumTypes>::min_కొలత ==
	       Offset32::static_కొలత + HBUINT24::static_కొలత, "");
#అయితే_ముగింపు


నిర్మాణం MarkGlyphSetsFormat1
{
  తార్కిక covers (గుర్తులేని పూర్ణాంకం సమితి_సూచీ, hb_codepoint_t glyph_id) స్థిరరాశి
  { తిరిగి (ఇది+coverage[సమితి_సూచీ]).get_coverage (glyph_id) != NOT_COVERED; }

  శూన్య collect_వాడిన_mark_sets (స్థిరరాశి hb_సమితి_t& glyph_సమితి,
                               hb_సమితి_t& వాడిన_mark_sets /*బహిర్గతం*/) స్థిరరాశి
  {
    గుర్తులేని i = 0;
    కొరకు (స్థిరరాశి స్వయంచాలక &స్థానభ్రంశం : coverage)
     {
       స్థిరరాశి స్వయంచాలక &cov = ఇది+స్థానభ్రంశం;
       ఒకవేళ (cov.intersects (&glyph_సమితి))
         వాడిన_mark_sets.కూడు (i);

       i++;
     }
  }

  మూస <రకం_పేరు సమితి_t>
  శూన్య collect_coverage (hb_సదిశ_t<సమితి_t> &sets) స్థిరరాశి
  {
     కొరకు (స్థిరరాశి స్వయంచాలక &స్థానభ్రంశం : coverage)
     {
       స్థిరరాశి స్వయంచాలక &cov = ఇది+స్థానభ్రంశం;
       cov.collect_coverage (sets.push ());
     }
  }

  తార్కిక subset (hb_subset_context_t *c) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    స్వయంచాలక *బహిర్గతం = c->serializer->ప్రారంభం_embed (*ఇది);
    ఒకవేళ (unlikely (!c->serializer->extend_min (బహిర్గతం))) return_trace (అబద్ధం);
    బహిర్గతం->format = format;

    తార్కిక ret = నిజం;
    కొరకు (స్థిరరాశి Offset32_గమ్యం<Coverage>& స్థానభ్రంశం : coverage.iter ())
    {
      స్వయంచాలక snap = c->serializer->snapshot ();
      స్వయంచాలక *o = బహిర్గతం->coverage.serialize_append (c->serializer);
      ఒకవేళ (unlikely (!o))
      {
	ret = అబద్ధం;
	ఆపు;
      }

      //skip empty coverage
      c->serializer->push ();
      తార్కిక res = అబద్ధం;
      ఒకవేళ (స్థానభ్రంశం) res = c->dispatch (ఇది+స్థానభ్రంశం);
      ఒకవేళ (!res)
      {
        c->serializer->తీయి_discard ();
        c->serializer->revert (snap);
        (బహిర్గతం->coverage.len)--;
        కొనసాగు;
      }
      c->serializer->కూడు_link (*o, c->serializer->తీయి_pack ());
    }

    return_trace (ret && బహిర్గతం->coverage.len);
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (coverage.sanitize (c, ఇది));
  }

  రక్షిత:
  HBUINT16	format;			/* Format identifier--format = 1 */
  Array16_యొక్క<Offset32_గమ్యం<Coverage>>
		coverage;		/* Array of long offsets to mark set
					 * coverage tables */
  బహిరంగ:
  DEFINE_కొలత_ARRAY (4, coverage);
};

నిర్మాణం MarkGlyphSets
{
  తార్కిక covers (గుర్తులేని పూర్ణాంకం సమితి_సూచీ, hb_codepoint_t glyph_id) స్థిరరాశి
  {
    ఎంపిక (u.format.v) {
    స్థితి 1: hb_barrier (); తిరిగి u.format1.covers (సమితి_సూచీ, glyph_id);
    అప్రమేయ:తిరిగి అబద్ధం;
    }
  }

  మూస <రకం_పేరు సమితి_t>
  శూన్య collect_coverage (hb_సదిశ_t<సమితి_t> &sets) స్థిరరాశి
  {
    ఎంపిక (u.format.v) {
    స్థితి 1: hb_barrier (); u.format1.collect_coverage (sets); తిరిగి;
    అప్రమేయ:తిరిగి;
    }
  }

  శూన్య collect_వాడిన_mark_sets (స్థిరరాశి hb_సమితి_t& glyph_సమితి,
                               hb_సమితి_t& వాడిన_mark_sets /*బహిర్గతం*/) స్థిరరాశి
  {
    ఎంపిక (u.format.v) {
    స్థితి 1: hb_barrier (); u.format1.collect_వాడిన_mark_sets (glyph_సమితి, వాడిన_mark_sets); తిరిగి;
    అప్రమేయ:తిరిగి;
    }
  }

  తార్కిక subset (hb_subset_context_t *c) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    ఎంపిక (u.format.v) {
    స్థితి 1: hb_barrier (); return_trace (u.format1.subset (c));
    అప్రమేయ:return_trace (అబద్ధం);
    }
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    ఒకవేళ (!u.format.v.sanitize (c)) return_trace (అబద్ధం);
    hb_barrier ();
    ఎంపిక (u.format.v) {
    స్థితి 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    అప్రమేయ:return_trace (నిజం);
    }
  }

  రక్షిత:
  సంఘం {
  నిర్మాణం { HBUINT16 v; }	format;		/* Format identifier */
  MarkGlyphSetsFormat1	format1;
  } u;
  బహిరంగ:
  DEFINE_కొలత_UNION (2, format.v);
};


/*
 * GDEF -- Glyph Definition
 * https://docs.microsoft.com/en-us/typography/opentype/spec/gdef
 */


నిర్మాణం GDEF
{
  స్థిర constexpr hb_ట్యాగ్_t పట్టిక_ట్యాగ్ = HB_OT_ట్యాగ్_GDEF;

  గణన GlyphClasses {
    UnclassifiedGlyph	= 0,
    పునాది_Glyph		= 1,
    LigatureGlyph	= 2,
    MarkGlyph		= 3,
    ComponentGlyph	= 4
  };

  size_t get_కొలత () స్థిరరాశి
  {
    ఒకవేళ (సంస్కరణ.major != 1) తిరిగి సంస్కరణ.static_కొలత;

    తిరిగి min_కొలత +
	   (సంస్కరణ.గమ్యం_int () >= 0x00010002u ? markGlyphSetsDef.static_కొలత : 0) +
	   (సంస్కరణ.గమ్యం_int () >= 0x00010003u ? varStore.static_కొలత : 0) +
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
	   (సంస్కరణ.గమ్యం_int () >= 0x00010004u ? 5 * Offset32::static_కొలత : 0)
#లేకపోతే
	   0
#అయితే_ముగింపు
	   ;
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    ఒకవేళ (unlikely (!సంస్కరణ.sanitize (c) ||
		  సంస్కరణ.major != 1 ||
		  !c->check_range (ఇది, get_కొలత ())))
      return_trace (అబద్ధం);
    hb_barrier ();

    తార్కిక glyph_class_def_ok = glyphClassDef.sanitize (c, ఇది);
    తార్కిక attach_list_ok = attachList.sanitize (c, ఇది);
    తార్కిక lig_caret_list_ok = ligCaretList.sanitize (c, ఇది);
    తార్కిక mark_attach_class_def_ok = markAttachClassDef.sanitize (c, ఇది);
    తార్కిక mark_glyph_sets_def_ok = సంస్కరణ.గమ్యం_int () < 0x00010002u ||
				  markGlyphSetsDef.sanitize (c, ఇది);

#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
    ఒకవేళ (సంస్కరణ.గమ్యం_int () >= 0x00010004u)
    {
      ఒకవేళ (glyphClassDef2) glyph_class_def_ok = glyphClassDef2.sanitize (c, ఇది);
      ఒకవేళ (attachList2) attach_list_ok = attachList2.sanitize (c, ఇది);
      ఒకవేళ (ligCaretList2) lig_caret_list_ok = ligCaretList2.sanitize (c, ఇది);
      ఒకవేళ (markAttachClassDef2) mark_attach_class_def_ok = markAttachClassDef2.sanitize (c, ఇది);
      ఒకవేళ (markGlyphSetsDef2) mark_glyph_sets_def_ok = markGlyphSetsDef2.sanitize (c, ఇది);
    }
#అయితే_ముగింపు

    return_trace (glyph_class_def_ok &&
		  attach_list_ok &&
		  lig_caret_list_ok &&
		  mark_attach_class_def_ok &&
		  mark_glyph_sets_def_ok &&
		  (సంస్కరణ.గమ్యం_int () < 0x00010003u || varStore.sanitize (c, ఇది)));
  }

  తార్కిక subset (hb_subset_context_t *c) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    ఒకవేళ (సంస్కరణ.major != 1) return_trace (అబద్ధం);

    స్వయంచాలక *బహిర్గతం = c->serializer->ప్రారంభం_embed (*ఇది);
    ఒకవేళ (unlikely (!c->serializer->extend_min (బహిర్గతం))) return_trace (అబద్ధం);

    // Push var store first (if it's needed) so that it's last in the
    // serialization order. Some font consumers assume that varstore runs to
    // the end of the GDEF table.
    // See: https://github.com/harfbuzz/harfbuzz/issues/4636
    స్వయంచాలక snapshot_version0 = c->serializer->snapshot ();
    ఒకవేళ (unlikely (సంస్కరణ.గమ్యం_int () >= 0x00010002u && hb_barrier () &&
		  !c->serializer->embed (markGlyphSetsDef)))
      return_trace (అబద్ధం);

    స్వయంచాలక snapshot_version2 = c->serializer->snapshot ();
    ఒకవేళ (unlikely (సంస్కరణ.గమ్యం_int () >= 0x00010003u && hb_barrier () &&
		  !c->serializer->embed (varStore)))
      return_trace (అబద్ధం);

    తార్కిక use_v14 = అబద్ధం;
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
    use_v14 = సంస్కరణ.గమ్యం_int () >= 0x00010004u &&
	      (glyphClassDef2 || attachList2 || ligCaretList2 ||
	       markAttachClassDef2 || markGlyphSetsDef2);
    ఒకవేళ (unlikely (సంస్కరణ.గమ్యం_int () >= 0x00010004u && hb_barrier () &&
		  !c->serializer->extend_కొలత (బహిర్గతం, get_కొలత ())))
      return_trace (అబద్ధం);
#అయితే_ముగింపు

    తార్కిక subset_varstore = అబద్ధం;
    గుర్తులేని varstore_సూచీ = (గుర్తులేని) -1;
    ఒకవేళ (సంస్కరణ.గమ్యం_int () >= 0x00010003u && hb_barrier ())
    {
      ఒకవేళ (c->plan->all_axes_pinned)
	బహిర్గతం->varStore = 0;
      లేకపోతే ఒకవేళ (c->plan->normalized_coords)
      {
	ఒకవేళ (varStore)
	{
	  అంశం_variations_t అంశం_vars;
	  ఒకవేళ (unlikely (!c->serializer->check_success (
                  అంశం_vars.instantiate (ఇది+varStore, c->plan, నిజం, నిజం,
                                          c->plan->gdef_varstore_inner_maps.as_array (), నిజం))))
            return_trace (అబద్ధం);
	  subset_varstore = బహిర్గతం->varStore.serialize_serialize (c->serializer,
							     అంశం_vars.has_long_మాట (),
							     c->plan->axis_ట్యాగులు,
							     అంశం_vars.get_region_list (),
							     అంశం_vars.get_vardata_encodings ());
	  varstore_సూచీ = c->serializer->చివరి_added_బిడ్డ_సూచీ();
	  remap_varidx_after_instantiation (అంశం_vars.get_varidx_map (),
					    c->plan->layout_variation_idx_delta_map);
          remap_varidx_after_instantiation (అంశం_vars.get_varidx_map (),
                                            c->plan->layout_condition_idx_విలువ_map);
	}
      }
      లేకపోతే
      {
	subset_varstore = బహిర్గతం->varStore.serialize_subset (c, varStore, ఇది,
							  c->plan->gdef_varstore_inner_maps.as_array ());
	varstore_సూచీ = c->serializer->చివరి_added_బిడ్డ_సూచీ();
      }
    }

    బహిర్గతం->సంస్కరణ.major = సంస్కరణ.major;
    బహిర్గతం->సంస్కరణ.minor = సంస్కరణ.minor;

    ఒకవేళ (!subset_varstore && సంస్కరణ.గమ్యం_int () >= 0x00010002u && !use_v14) {
      c->serializer->revert (snapshot_version2);
    }
    ఒకవేళ (!subset_varstore)
      బహిర్గతం->varStore = 0;
    లేకపోతే
      c->plan->has_gdef_varstore = నిజం;

    తార్కిక subset_markglyphsetsdef = అబద్ధం;
    ఒకవేళ (సంస్కరణ.గమ్యం_int () >= 0x00010002u && hb_barrier ())
    {
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
      ఒకవేళ (use_v14 && markGlyphSetsDef2)
      {
	బహిర్గతం->markGlyphSetsDef = 0;
	subset_markglyphsetsdef = serialize_subset_స్థానభ్రంశం (c, బహిర్గతం->markGlyphSetsDef2,
							   markGlyphSetsDef2, ఇది);
      }
      లేకపోతే
#అయితే_ముగింపు
      {
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
	ఒకవేళ (use_v14) బహిర్గతం->markGlyphSetsDef2 = 0;
#అయితే_ముగింపు
	subset_markglyphsetsdef = బహిర్గతం->markGlyphSetsDef.serialize_subset (c, markGlyphSetsDef, ఇది);
      }
    }

    ఒకవేళ (use_v14)
    {
      బహిర్గతం->సంస్కరణ.minor = 4;
    } లేకపోతే ఒకవేళ (subset_varstore) {
      బహిర్గతం->సంస్కరణ.minor = 3;
    } లేకపోతే ఒకవేళ (subset_markglyphsetsdef) {
      బహిర్గతం->సంస్కరణ.minor = 2;
    } లేకపోతే  {
      బహిర్గతం->సంస్కరణ.minor = 0;
      c->serializer->revert (snapshot_version0);
    }

    తార్కిక subset_glyphclassdef = అబద్ధం;
    తార్కిక subset_attachlist = అబద్ధం;
    తార్కిక subset_markattachclassdef = అబద్ధం;
    తార్కిక subset_ligcaretlist = అబద్ధం;

#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
    ఒకవేళ (use_v14 && glyphClassDef2)
    {
      బహిర్గతం->glyphClassDef = 0;
      subset_glyphclassdef = serialize_subset_స్థానభ్రంశం (c, బహిర్గతం->glyphClassDef2,
						      glyphClassDef2, ఇది, శూన్యసూచిక, అబద్ధం, నిజం);
    }
    లేకపోతే
#అయితే_ముగింపు
    {
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
      ఒకవేళ (use_v14) బహిర్గతం->glyphClassDef2 = 0;
#అయితే_ముగింపు
      subset_glyphclassdef = బహిర్గతం->glyphClassDef.serialize_subset (c, glyphClassDef, ఇది, శూన్యసూచిక, అబద్ధం, నిజం);
    }

#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
    ఒకవేళ (use_v14 && attachList2)
    {
      బహిర్గతం->attachList = 0;
      subset_attachlist = serialize_subset_స్థానభ్రంశం (c, బహిర్గతం->attachList2, attachList2, ఇది);
    }
    లేకపోతే
#అయితే_ముగింపు
    {
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
      ఒకవేళ (use_v14) బహిర్గతం->attachList2 = 0;
#అయితే_ముగింపు
      subset_attachlist = బహిర్గతం->attachList.serialize_subset (c, attachList, ఇది);
    }

#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
    ఒకవేళ (use_v14 && markAttachClassDef2)
    {
      బహిర్గతం->markAttachClassDef = 0;
      subset_markattachclassdef = serialize_subset_స్థానభ్రంశం (c, బహిర్గతం->markAttachClassDef2,
							   markAttachClassDef2, ఇది, శూన్యసూచిక, అబద్ధం, నిజం);
    }
    లేకపోతే
#అయితే_ముగింపు
    {
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
      ఒకవేళ (use_v14) బహిర్గతం->markAttachClassDef2 = 0;
#అయితే_ముగింపు
      subset_markattachclassdef = బహిర్గతం->markAttachClassDef.serialize_subset (c, markAttachClassDef,
									   ఇది, శూన్యసూచిక, అబద్ధం, నిజం);
    }

#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
    ఒకవేళ (use_v14 && ligCaretList2)
    {
      బహిర్గతం->ligCaretList = 0;
      subset_ligcaretlist = serialize_subset_స్థానభ్రంశం (c, బహిర్గతం->ligCaretList2, ligCaretList2, ఇది);
    }
    లేకపోతే
#అయితే_ముగింపు
    {
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
      ఒకవేళ (use_v14) బహిర్గతం->ligCaretList2 = 0;
#అయితే_ముగింపు
      subset_ligcaretlist = బహిర్గతం->ligCaretList.serialize_subset (c, ligCaretList, ఇది);
    }

    ఒకవేళ (subset_varstore && varstore_సూచీ != (గుర్తులేని) -1) {
      c->serializer->repack_చివరి(varstore_సూచీ);
    }

    return_trace (subset_glyphclassdef || subset_attachlist ||
		  subset_ligcaretlist || subset_markattachclassdef ||
		  (బహిర్గతం->సంస్కరణ.గమ్యం_int () >= 0x00010002u && subset_markglyphsetsdef) ||
		  (బహిర్గతం->సంస్కరణ.గమ్యం_int () >= 0x00010003u && subset_varstore));
  }

  తార్కిక has_glyph_classes () స్థిరరాశి
  {
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
    ఒకవేళ (సంస్కరణ.గమ్యం_int () >= 0x00010004u && glyphClassDef2) తిరిగి నిజం;
#అయితే_ముగింపు
    తిరిగి glyphClassDef != 0;
  }
  స్థిరరాశి ClassDef &get_glyph_class_def () స్థిరరాశి
  {
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
    ఒకవేళ (సంస్కరణ.గమ్యం_int () >= 0x00010004u && glyphClassDef2) తిరిగి ఇది+glyphClassDef2;
#అయితే_ముగింపు
    తిరిగి ఇది+glyphClassDef;
  }
  తార్కిక has_attach_list () స్థిరరాశి
  {
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
    ఒకవేళ (సంస్కరణ.గమ్యం_int () >= 0x00010004u && attachList2) తిరిగి నిజం;
#అయితే_ముగింపు
    తిరిగి attachList != 0;
  }
  స్థిరరాశి AttachList &get_attach_list () స్థిరరాశి
  {
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
    ఒకవేళ (సంస్కరణ.గమ్యం_int () >= 0x00010004u && attachList2) తిరిగి ఇది+attachList2;
#అయితే_ముగింపు
    తిరిగి ఇది+attachList;
  }
  తార్కిక has_lig_carets () స్థిరరాశి
  {
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
    ఒకవేళ (సంస్కరణ.గమ్యం_int () >= 0x00010004u && ligCaretList2) తిరిగి నిజం;
#అయితే_ముగింపు
    తిరిగి ligCaretList != 0;
  }
  తార్కిక has_mark_attachment_types () స్థిరరాశి
  {
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
    ఒకవేళ (సంస్కరణ.గమ్యం_int () >= 0x00010004u && markAttachClassDef2) తిరిగి నిజం;
#అయితే_ముగింపు
    తిరిగి markAttachClassDef != 0;
  }
  స్థిరరాశి ClassDef &get_mark_attach_class_def () స్థిరరాశి
  {
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
    ఒకవేళ (సంస్కరణ.గమ్యం_int () >= 0x00010004u && markAttachClassDef2) తిరిగి ఇది+markAttachClassDef2;
#అయితే_ముగింపు
    తిరిగి ఇది+markAttachClassDef;
  }
  తార్కిక has_mark_glyph_sets () స్థిరరాశి
  {
    ఒకవేళ (సంస్కరణ.గమ్యం_int () < 0x00010002u) తిరిగి అబద్ధం;
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
    ఒకవేళ (సంస్కరణ.గమ్యం_int () >= 0x00010004u && markGlyphSetsDef2) తిరిగి నిజం;
#అయితే_ముగింపు
    తిరిగి markGlyphSetsDef != 0;
  }
  స్థిరరాశి MarkGlyphSets &get_mark_glyph_sets () స్థిరరాశి
  {
    ఒకవేళ (సంస్కరణ.గమ్యం_int () < 0x00010002u) తిరిగి Null(MarkGlyphSets);
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
    ఒకవేళ (సంస్కరణ.గమ్యం_int () >= 0x00010004u && markGlyphSetsDef2) తిరిగి ఇది+markGlyphSetsDef2;
#అయితే_ముగింపు
    తిరిగి ఇది+markGlyphSetsDef;
  }
  తార్కిక has_var_store () స్థిరరాశి
  {
    తిరిగి సంస్కరణ.గమ్యం_int () >= 0x00010003u && hb_barrier () && varStore != 0;
  }
  స్థిరరాశి అంశం_Variation_Store &get_var_store () స్థిరరాశి
  {
    తిరిగి సంస్కరణ.గమ్యం_int () >= 0x00010003u && hb_barrier () ? ఇది+varStore : Null(అంశం_Variation_Store);
  }


  తార్కిక has_దత్తాంశం () స్థిరరాశి { తిరిగి సంస్కరణ.గమ్యం_int (); }
  గుర్తులేని పూర్ణాంకం get_glyph_class (hb_codepoint_t glyph) స్థిరరాశి
  { తిరిగి get_glyph_class_def ().get_class (glyph); }
  శూన్య get_glyphs_లోపల_class (గుర్తులేని పూర్ణాంకం klass, hb_సమితి_t *glyphs) స్థిరరాశి
  { get_glyph_class_def ().collect_class (glyphs, klass); }

  గుర్తులేని పూర్ణాంకం get_mark_attachment_type (hb_codepoint_t glyph) స్థిరరాశి
  { తిరిగి get_mark_attach_class_def ().get_class (glyph); }

  గుర్తులేని పూర్ణాంకం get_attach_points (hb_codepoint_t glyph_id,
				  గుర్తులేని పూర్ణాంకం ప్రారంభం_స్థానభ్రంశం,
				  గుర్తులేని పూర్ణాంకం *బిందువు_లెక్క /* IN/OUT */,
				  గుర్తులేని పూర్ణాంకం *బిందువు_array /*బహిర్గతం*/) స్థిరరాశి
  { తిరిగి get_attach_list ().get_attach_points (glyph_id, ప్రారంభం_స్థానభ్రంశం, బిందువు_లెక్క, బిందువు_array); }

  గుర్తులేని పూర్ణాంకం get_lig_carets (hb_font_t *font,
			       hb_direction_t direction,
			       hb_codepoint_t glyph_id,
			       గుర్తులేని పూర్ణాంకం ప్రారంభం_స్థానభ్రంశం,
			       గుర్తులేని పూర్ణాంకం *caret_లెక్క /* IN/OUT */,
			       hb_position_t *caret_array /*బహిర్గతం*/) స్థిరరాశి
  {
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
    ఒకవేళ (సంస్కరణ.గమ్యం_int () >= 0x00010004u && ligCaretList2)
      తిరిగి (ఇది+ligCaretList2).get_lig_carets (font, direction, glyph_id, get_var_store (),
						  ప్రారంభం_స్థానభ్రంశం, caret_లెక్క, caret_array);
#అయితే_ముగింపు
    తిరిగి (ఇది+ligCaretList).get_lig_carets (font, direction, glyph_id, get_var_store (),
					       ప్రారంభం_స్థానభ్రంశం, caret_లెక్క, caret_array);
  }

  తార్కిక mark_సమితి_covers (గుర్తులేని పూర్ణాంకం సమితి_సూచీ, hb_codepoint_t glyph_id) స్థిరరాశి
  { తిరిగి get_mark_glyph_sets ().covers (సమితి_సూచీ, glyph_id); }

  /* glyph_props is a 16-bit integer where the lower 8-bit have bits representing
   * glyph class and other bits, and high 8-bit the mark attachment type (if any).
   * Not to be confused with lookup_props which is very similar. */
  గుర్తులేని పూర్ణాంకం get_glyph_props (hb_codepoint_t glyph) స్థిరరాశి
  {
    గుర్తులేని పూర్ణాంకం klass = get_glyph_class (glyph);

    static_assert (((గుర్తులేని పూర్ణాంకం) HB_OT_LAYOUT_GLYPH_PROPS_పునాది_GLYPH == (గుర్తులేని పూర్ణాంకం) LookupFlag::Ignore_పునాది_Glyphs), "");
    static_assert (((గుర్తులేని పూర్ణాంకం) HB_OT_LAYOUT_GLYPH_PROPS_LIGATURE == (గుర్తులేని పూర్ణాంకం) LookupFlag::IgnoreLigatures), "");
    static_assert (((గుర్తులేని పూర్ణాంకం) HB_OT_LAYOUT_GLYPH_PROPS_MARK == (గుర్తులేని పూర్ణాంకం) LookupFlag::IgnoreMarks), "");

    ఎంపిక (klass) {
    అప్రమేయ:			తిరిగి HB_OT_LAYOUT_GLYPH_CLASS_UNCLASSIFIED;
    స్థితి పునాది_Glyph:		తిరిగి HB_OT_LAYOUT_GLYPH_PROPS_పునాది_GLYPH;
    స్థితి LigatureGlyph:		తిరిగి HB_OT_LAYOUT_GLYPH_PROPS_LIGATURE;
    స్థితి MarkGlyph:
	  klass = get_mark_attachment_type (glyph);
	  తిరిగి HB_OT_LAYOUT_GLYPH_PROPS_MARK | (klass << 8);
    }
  }

  HB_అంతర్గత తార్కిక is_blocklisted (hb_blob_t *blob,
				   hb_face_t *face) స్థిరరాశి;

  నిర్మాణం accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      పట్టిక = hb_sanitize_context_t ().reference_పట్టిక<GDEF> (face);
      ఒకవేళ (unlikely (పట్టిక->is_blocklisted (పట్టిక.get_blob (), face)))
      {
	hb_blob_destroy (పట్టిక.get_blob ());
	పట్టిక = hb_blob_get_ఖాళీలు ();
      }

#నిర్వచితం_కాకపోతే HB_NO_GDEF_CACHE
      /* Flatten each mark set whose glyphs span fewer than PAGE_BITS
       * gids into a single bit-page biased at its first glyph, for
       * O(1) exact answers.  Wider sets, or on allocation failure,
       * fall back to the table. */
      {
	hb_సదిశ_t<hb_సమితి_t> sets;
	పట్టిక->get_mark_glyph_sets ().collect_coverage (sets);
	ఒకవేళ (!sets.లోపల_error () &&
	    mark_glyph_సమితి_bitmaps.resize (sets.నిడివి))
	  కొరకు (గుర్తులేని i = 0; i < sets.నిడివి; i++)
	  {
	    స్వయంచాలక &bitmap = mark_glyph_సమితి_bitmaps.arrayZ[i];
	    bitmap.bias = HB_సమితి_విలువ_INVALID;
	    స్థిరరాశి స్వయంచాలక &సమితి = sets.arrayZ[i];
	    ఒకవేళ (unlikely (సమితి.లోపల_error ())) కొనసాగు;
	    hb_codepoint_t min_gid = HB_సమితి_విలువ_INVALID;
	    hb_codepoint_t max_gid = HB_సమితి_విలువ_INVALID;
	    hb_సమితి_తదుపరి (&సమితి, &min_gid);
	    hb_సమితి_మునుపటి (&సమితి, &max_gid);
	    ఒకవేళ (max_gid == HB_సమితి_విలువ_INVALID)
	      కొనసాగు;
	    ఒకవేళ (max_gid - min_gid >= hb_బిట్టు_పుట_t::పుట_బిట్లు)
	    {
	      /* Too wide for one page; a digest pre-filter instead. */
	      కొరకు (hb_codepoint_t gid : సమితి)
		bitmap.u.digest.కూడు (gid);
	      కొనసాగు;
	    }
	    bitmap.bias = min_gid;
	    కొరకు (hb_codepoint_t gid : సమితి)
	      bitmap.u.పుట.కూడు (gid - min_gid);
	  }
      }
#అయితే_ముగింపు
    }
    ~accelerator_t () { పట్టిక.destroy (); }

    గుర్తులేని పూర్ణాంకం get_glyph_props (hb_codepoint_t glyph) స్థిరరాశి
    {
      గుర్తులేని v;

#నిర్వచితం_కాకపోతే HB_NO_GDEF_CACHE
      ఒకవేళ (glyph_props_cache.get (glyph, &v))
        తిరిగి v;
#అయితే_ముగింపు

      v = పట్టిక->get_glyph_props (glyph);

#నిర్వచితం_కాకపోతే HB_NO_GDEF_CACHE
      ఒకవేళ (likely (పట్టిక.get_blob ())) // Don't try setting if we are the null instance!
	glyph_props_cache.సమితి (glyph, v);
#అయితే_ముగింపు

      తిరిగి v;

    }

    HB_ALWAYS_INLINE
    తార్కిక mark_సమితి_covers (గుర్తులేని పూర్ణాంకం సమితి_సూచీ, hb_codepoint_t glyph_id) స్థిరరాశి
    {
#నిర్వచితం_కాకపోతే HB_NO_GDEF_CACHE
      // We can access arrayZ directly because of sanitize_lookup_props() guarantee.
      స్థిరరాశి స్వయంచాలక &bitmap = mark_glyph_సమితి_bitmaps.arrayZ[సమితి_సూచీ];
      ఒకవేళ (likely (bitmap.bias != HB_సమితి_విలువ_INVALID))
      {
	hb_codepoint_t biased = glyph_id - bitmap.bias;
	తిరిగి biased < hb_బిట్టు_పుట_t::పుట_బిట్లు &&
	       bitmap.u.పుట.get (biased);
      }
      తిరిగి mark_సమితి_covers_slow (సమితి_సూచీ, glyph_id);
#లేకపోతే
      తిరిగి పట్టిక->mark_సమితి_covers (సమితి_సూచీ, glyph_id);
#అయితే_ముగింపు
    }

#నిర్వచితం_కాకపోతే HB_NO_GDEF_CACHE
    HB_NEVER_INLINE
    తార్కిక mark_సమితి_covers_slow (గుర్తులేని పూర్ణాంకం సమితి_సూచీ, hb_codepoint_t glyph_id) స్థిరరాశి
    {
      స్థిరరాశి స్వయంచాలక &bitmap = mark_glyph_సమితి_bitmaps.arrayZ[సమితి_సూచీ];
      ఒకవేళ (!bitmap.u.digest.may_have (glyph_id))
	తిరిగి అబద్ధం;
      తిరిగి పట్టిక->mark_సమితి_covers (సమితి_సూచీ, glyph_id);
    }
#అయితే_ముగింపు

    గుర్తులేని sanitize_lookup_props (గుర్తులేని lookup_props) స్థిరరాశి
    {
#నిర్వచితం_కాకపోతే HB_NO_GDEF_CACHE
      ఒకవేళ (lookup_props & LookupFlag::Use_Mark_Filtering_సమితి &&
	  (lookup_props >> 16) >= mark_glyph_సమితి_bitmaps.నిడివి)
      {
        // Invalid mark filtering set index; unset the flag.
	lookup_props &= ~LookupFlag::Use_Mark_Filtering_సమితి;
      }
#అయితే_ముగింపు
      తిరిగి lookup_props;
    }

    hb_blob_ptr_t<GDEF> పట్టిక;
#నిర్వచితం_కాకపోతే HB_NO_GDEF_CACHE
    నిర్మాణం mark_glyph_సమితి_bitmap_t
    {
      /* Gid of the first bit (not page-aligned) for an exact one-page
       * set, or HB_SET_VALUE_INVALID; then digest is a pre-filter for
       * the table.  The all-zeros state is valid for both members. */
      hb_codepoint_t bias = HB_సమితి_విలువ_INVALID;
      సంఘం u_t {
	u_t () : పుట () {}
	hb_బిట్టు_పుట_t పుట;
	hb_సమితి_digest_t digest;
      } u;
    };

    hb_సదిశ_t<mark_glyph_సమితి_bitmap_t> mark_glyph_సమితి_bitmaps;
    మార్చదగిన hb_cache_t<21, 3> glyph_props_cache;
    static_assert (పరిమాణం (glyph_props_cache) == 512, "");
#అయితే_ముగింపు
  };

  శూన్య collect_variation_indices (hb_collect_variation_indices_context_t *c) స్థిరరాశి
  {
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
    ఒకవేళ (సంస్కరణ.గమ్యం_int () >= 0x00010004u && ligCaretList2)
    {
      (ఇది+ligCaretList2).collect_variation_indices (c);
      తిరిగి;
    }
#అయితే_ముగింపు
    (ఇది+ligCaretList).collect_variation_indices (c);
  }

  రక్షిత:
  మూస <రకం_పేరు కీలకం>
  స్థిర శూన్య remap_varidx_after_instantiation (స్థిరరాశి hb_map_t& varidx_map,
                                                hb_hashmap_t<కీలకం, hb_pair_t<గుర్తులేని, పూర్ణాంకం>>& layout_variation_idx_delta_map /* IN/OUT */)
  {
    /* varidx_map is empty which means varstore is empty after instantiation,
     * no variations, map all varidx to HB_OT_LAYOUT_NO_VARIATIONS_INDEX.
     * varidx_map doesn't have original varidx, indicating delta row is all
     * zeros, map varidx to HB_OT_LAYOUT_NO_VARIATIONS_INDEX */
    కొరకు (స్వయంచాలక _ : layout_variation_idx_delta_map.iter_ref ())
    {
      /* old_varidx->(varidx, delta) mapping generated for subsetting, then this
       * varidx is used as key of varidx_map during instantiation */
      uint32_t varidx = _.second.మొదటి;
      uint32_t *new_varidx;
      ఒకవేళ (varidx_map.has (varidx, &new_varidx))
	_.second.మొదటి = *new_varidx;
      లేకపోతే
	_.second.మొదటి = HB_OT_LAYOUT_NO_VARIATIONS_సూచీ;
    }
  }

  మూస <రకం_పేరు స్థానభ్రంశం_బహిర్గతం, రకం_పేరు స్థానభ్రంశం_లోపల, రకం_పేరు పునాది_3, రకం_పేరు ...Ts>
  స్థిర తార్కిక serialize_subset_స్థానభ్రంశం (hb_subset_context_t *c,
				       స్థానభ్రంశం_బహిర్గతం& బహిర్గతం,
				       స్థిరరాశి స్థానభ్రంశం_లోపల& లోపల,
				       స్థిరరాశి పునాది_3 *పునాది,
				       Ts&&... ds)
  {
    బహిర్గతం = 0;
    ఒకవేళ (లోపల.is_null ()) తిరిగి అబద్ధం;

    స్వయంచాలక *s = c->serializer;
    s->push ();
    తార్కిక ret = c->dispatch (పునాది+లోపల, std::forward<Ts> (ds)...);
    ఒకవేళ (ret) s->కూడు_link (బహిర్గతం, s->తీయి_pack ());
    లేకపోతే s->తీయి_discard ();
    తిరిగి ret;
  }

  Fixed_సంస్కరణ<>సంస్కరణ;		/* Version of the GDEF table--currently
					 * 0x00010004u */
  Offset16_గమ్యం<ClassDef>
		glyphClassDef;		/* Offset to class definition table
					 * for glyph type--from beginning of
					 * GDEF header (may be Null) */
  Offset16_గమ్యం<AttachList>
		attachList;		/* Offset to list of glyphs with
					 * attachment points--from beginning
					 * of GDEF header (may be Null) */
  Offset16_గమ్యం<LigCaretList<SmallTypes>>
		ligCaretList;		/* Offset to list of positioning points
					 * for ligature carets--from beginning
					 * of GDEF header (may be Null) */
  Offset16_గమ్యం<ClassDef>
		markAttachClassDef;	/* Offset to class definition table for
					 * mark attachment type--from beginning
					 * of GDEF header (may be Null) */
  Offset16_గమ్యం<MarkGlyphSets>
		markGlyphSetsDef;	/* Offset to the table of mark set
					 * definitions--from beginning of GDEF
					 * header (may be NULL).  Introduced
					 * in version 0x00010002. */
  Offset32_గమ్యం<అంశం_Variation_Store>
		varStore;		/* Offset to the table of Item Variation
					 * Store--from beginning of GDEF
					 * header (may be NULL).  Introduced
					 * in version 0x00010003. */
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
  Offset32_గమ్యం<ClassDef>
		glyphClassDef2;		/* 32-bit offset to class definition
					 * table for glyph type.  Introduced
					 * in version 0x00010004. */
  Offset32_గమ్యం<AttachList>
		attachList2;		/* 32-bit offset to list of glyphs
					 * with attachment points.  Introduced
					 * in version 0x00010004. */
  Offset32_గమ్యం<LigCaretList<MediumTypes>>
		ligCaretList2;		/* 32-bit offset to LigCaretList2.
					 * Introduced in version 0x00010004. */
  Offset32_గమ్యం<ClassDef>
		markAttachClassDef2;	/* 32-bit offset to class definition
					 * table for mark attachment type.
					 * Introduced in version 0x00010004. */
  Offset32_గమ్యం<MarkGlyphSets>
		markGlyphSetsDef2;	/* 32-bit offset to the table of mark
					 * set definitions.  Introduced in
					 * version 0x00010004. */
#అయితే_ముగింపు
  బహిరంగ:
  DEFINE_కొలత_MIN (12);
};

నిర్మాణం GDEF_accelerator_t : GDEF::accelerator_t {
  GDEF_accelerator_t (hb_face_t *face) : GDEF::accelerator_t (face) {}
};

} /* namespace OT */


#అయితే_ముగింపు /* OT_LAYOUT_GDEF_GDEF_HH */
