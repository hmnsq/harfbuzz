/*
 * Copyright © 2016  Elie Roux <elie.roux@telecom-bretagne.eu>
 * Copyright © 2018  Google, Inc.
 * Copyright © 2018-2019  Ebrahim Byagowi
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
 * Google Author(s): Behdad Esfahbod
 */

#నిర్వచితం_కాకపోతే HB_OT_LAYOUT_పునాది_పట్టిక_HH
#నిర్వచించు HB_OT_LAYOUT_పునాది_పట్టిక_HH

#అన్తర్భావ "hb-open-type.hh"
#అన్తర్భావ "hb-ot-layout-common.hh"

నామాకాశ OT {

/*
 * BASE -- Baseline
 * https://docs.microsoft.com/en-us/typography/opentype/spec/base
 */

నిర్మాణం పునాది_Coord_Format1
{
  hb_position_t get_coord (hb_font_t *font, hb_direction_t direction) స్థిరరాశి
  {
    తిరిగి HB_DIRECTION_IS_HORIZONTAL (direction) ? font->em_scale_y (coordinate) : font->em_scale_x (coordinate);
  }

  తార్కిక subset (hb_subset_context_t *c) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    return_trace ((తార్కిక) c->serializer->embed (*ఇది));
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది));
  }

  రక్షిత:
  HBUINT16	format;		/* Format identifier--format = 1 */
  FWORD		coordinate;	/* X or Y value, in design units */
  బహిరంగ:
  DEFINE_కొలత_STATIC (4);
};

మూస <రకం_పేరు Types>
నిర్మాణం పునాది_Coord_Format2_4
{
  hb_position_t get_coord (hb_font_t *font, hb_direction_t direction) స్థిరరాశి
  {
    /* TODO */
    తిరిగి HB_DIRECTION_IS_HORIZONTAL (direction) ? font->em_scale_y (coordinate) : font->em_scale_x (coordinate);
  }

  తార్కిక subset (hb_subset_context_t *c) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    స్వయంచాలక *బహిర్గతం = c->serializer->embed (*ఇది);
    ఒకవేళ (unlikely (!బహిర్గతం)) return_trace (అబద్ధం);

    hb_codepoint_t *new_gid;
    ఒకవేళ (!c->plan->glyph_map->has (referenceGlyph, &new_gid))
      return_trace (అబద్ధం);

    return_trace (c->serializer->check_కేటాయించు (బహిర్గతం->referenceGlyph,
                                               *new_gid,
                                               HB_SERIALIZE_ERROR_INT_అధికప్రవాహం));
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది));
  }

  రక్షిత:
  HBUINT16	format;		/* Format identifier--format = 2 or 4 */
  FWORD		coordinate;	/* X or Y value, in design units */
  రకం_పేరు Types::HBGlyphID
		referenceGlyph;	/* Glyph ID of control glyph */
  HBUINT16	coord_బిందువు;	/* Index of contour point on the
				 * reference glyph */
  బహిరంగ:
  DEFINE_కొలత_STATIC (6 + Types::HBGlyphID::static_కొలత);
};

నిర్మాణం పునాది_Coord_Format3
{
  hb_position_t get_coord (hb_font_t *font,
			   స్థిరరాశి అంశం_Variation_Store &var_store,
			   hb_direction_t direction) స్థిరరాశి
  {
    స్థిరరాశి Device &device = ఇది+device_పట్టిక;

    తిరిగి HB_DIRECTION_IS_HORIZONTAL (direction)
	 ? hb_saturate_కూడు (font->em_scale_y (coordinate), device.get_y_delta (font, var_store))
	 : hb_saturate_కూడు (font->em_scale_x (coordinate), device.get_x_delta (font, var_store));
  }

  శూన్య collect_variation_indices (hb_సమితి_t& varidx_సమితి /*బహిర్గతం*/) స్థిరరాశి
  {
    గుర్తులేని varidx = (ఇది+device_పట్టిక).get_variation_సూచీ ();
    varidx_సమితి.కూడు (varidx);
  }

  తార్కిక subset (hb_subset_context_t *c) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    స్వయంచాలక *బహిర్గతం = c->serializer->embed (*ఇది);
    ఒకవేళ (unlikely (!బహిర్గతం)) return_trace (అబద్ధం);

    ఒకవేళ (!c->plan->pinned_at_default)
    {
      గుర్తులేని var_idx = (ఇది+device_పట్టిక).get_variation_సూచీ ();
      ఒకవేళ (var_idx != VarIdx::NO_VARIATION)
      {
        hb_pair_t<గుర్తులేని, పూర్ణాంకం> *v;
        ఒకవేళ (!c->plan->పునాది_variation_idx_map.has (var_idx, &v))
          return_trace (అబద్ధం);
        
        ఒకవేళ (unlikely (!c->serializer->check_కేటాయించు (బహిర్గతం->coordinate, coordinate + hb_రెండవ (*v),
                                                    HB_SERIALIZE_ERROR_INT_అధికప్రవాహం)))
          return_trace (అబద్ధం);
      }
    }
    
    బహిర్గతం->device_పట్టిక.serialize_copy (c->serializer, device_పట్టిక,
                                     ఇది, 0,
                                     hb_serialize_context_t::తల,
                                     &c->plan->పునాది_variation_idx_map);
    return_trace (నిజం);
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (likely (c->check_struct (ఇది) &&
			  device_పట్టిక.sanitize (c, ఇది)));
  }

  రక్షిత:
  HBUINT16	format;		/* Format identifier--format = 3 */
  FWORD		coordinate;	/* X or Y value, in design units */
  Offset16_గమ్యం<Device>
		device_పట్టిక;	/* Offset to Device table for X or
				 * Y value, from beginning of
				 * BaseCoord table (may be NULL). */
  బహిరంగ:
  DEFINE_కొలత_STATIC (6);
};

నిర్మాణం పునాది_Coord
{
  తార్కిక has_దత్తాంశం () స్థిరరాశి { తిరిగి u.format.v; }

  hb_position_t get_coord (hb_font_t            *font,
			   స్థిరరాశి అంశం_Variation_Store &var_store,
			   hb_direction_t        direction) స్థిరరాశి
  {
    ఎంపిక (u.format.v) {
    స్థితి 1: hb_barrier (); తిరిగి u.format1.get_coord (font, direction);
    స్థితి 2: hb_barrier (); తిరిగి u.format2.get_coord (font, direction);
    స్థితి 3: hb_barrier (); తిరిగి u.format3.get_coord (font, var_store, direction);
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
    స్థితి 4: hb_barrier (); తిరిగి u.format4.get_coord (font, direction);
#అయితే_ముగింపు
    అప్రమేయ:తిరిగి 0;
    }
  }

  శూన్య collect_variation_indices (hb_సమితి_t& varidx_సమితి /*బహిర్గతం*/) స్థిరరాశి
  {
    ఎంపిక (u.format.v) {
    స్థితి 3: hb_barrier (); u.format3.collect_variation_indices (varidx_సమితి); తిరిగి;
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
    స్థితి 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
    స్థితి 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
#అయితే_ముగింపు
    అప్రమేయ:return_trace (c->default_return_విలువ ());
    }
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    ఒకవేళ (unlikely (!u.format.v.sanitize (c))) return_trace (అబద్ధం);
    hb_barrier ();
    ఎంపిక (u.format.v) {
    స్థితి 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    స్థితి 2: hb_barrier (); return_trace (u.format2.sanitize (c));
    స్థితి 3: hb_barrier (); return_trace (u.format3.sanitize (c));
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
    స్థితి 4: hb_barrier (); return_trace (u.format4.sanitize (c));
#అయితే_ముగింపు
    అప్రమేయ:return_trace (అబద్ధం);
    }
  }

  రక్షిత:
  సంఘం {
  నిర్మాణం { HBUINT16 v; }	format;
  పునాది_Coord_Format1	format1;
  పునాది_Coord_Format2_4<Layout::SmallTypes>
			format2;
  పునాది_Coord_Format3	format3;
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
  పునాది_Coord_Format2_4<Layout::MediumTypes>
			format4;
#అయితే_ముగింపు
  } u;
  బహిరంగ:
  DEFINE_కొలత_UNION (2, format.v);
};

నిర్మాణం Feat_Min_Max_రికార్డు
{
  పూర్ణాంకం cmp (hb_ట్యాగ్_t key) స్థిరరాశి { తిరిగి ట్యాగ్_2.cmp (key); }

  తార్కిక has_దత్తాంశం () స్థిరరాశి { తిరిగి ట్యాగ్_2; }

  hb_ట్యాగ్_t get_feature_ట్యాగ్ () స్థిరరాశి { తిరిగి ట్యాగ్_2; }

  శూన్య get_min_max (స్థిరరాశి పునాది_Coord **min, స్థిరరాశి పునాది_Coord **max) స్థిరరాశి
  {
    ఒకవేళ (likely (min)) *min = &(ఇది+minCoord);
    ఒకవేళ (likely (max)) *max = &(ఇది+maxCoord);
  }

  శూన్య collect_variation_indices (స్థిరరాశి hb_subset_plan_t* plan,
                                  స్థిరరాశి శూన్య *పునాది,
                                  hb_సమితి_t& varidx_సమితి /*బహిర్గతం*/) స్థిరరాశి
  {
    ఒకవేళ (!plan->layout_features.has (ట్యాగ్_2))
      తిరిగి;

    (పునాది+minCoord).collect_variation_indices (varidx_సమితి);
    (పునాది+maxCoord).collect_variation_indices (varidx_సమితి);
  }

  తార్కిక subset (hb_subset_context_t *c,
               స్థిరరాశి శూన్య *పునాది) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    స్వయంచాలక *బహిర్గతం = c->serializer->embed (*ఇది);
    ఒకవేళ (unlikely (!బహిర్గతం)) return_trace (అబద్ధం);

    తార్కిక ret = బహిర్గతం->minCoord.serialize_subset (c, minCoord, పునాది);
    ret |= బహిర్గతం->maxCoord.serialize_subset (c, maxCoord, పునాది);
    return_trace (ret);
  }

  తార్కిక sanitize (hb_sanitize_context_t *c, స్థిరరాశి శూన్య *పునాది) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (likely (c->check_struct (ఇది) &&
			  minCoord.sanitize (c, పునాది) &&
			  maxCoord.sanitize (c, పునాది)));
  }

  రక్షిత:
  ట్యాగ్		ట్యాగ్_2;		/* 4-byte feature identification tag--must
				 * match feature tag in FeatureList */
  Offset16_గమ్యం<పునాది_Coord>
		minCoord;	/* Offset to BaseCoord table that defines
				 * the minimum extent value, from beginning
				 * of MinMax table (may be NULL) */
  Offset16_గమ్యం<పునాది_Coord>
		maxCoord;	/* Offset to BaseCoord table that defines
				 * the maximum extent value, from beginning
				 * of MinMax table (may be NULL) */
  బహిరంగ:
  DEFINE_కొలత_STATIC (8);
};

నిర్మాణం MinMax
{
  శూన్య get_min_max (hb_ట్యాగ్_t          feature_ట్యాగ్,
		    స్థిరరాశి పునాది_Coord **min,
		    స్థిరరాశి పునాది_Coord **max) స్థిరరాశి
  {
    స్థిరరాశి Feat_Min_Max_రికార్డు &minMaxCoord = feat_Min_Max_రికార్డులు.bsearch (feature_ట్యాగ్);
    ఒకవేళ (minMaxCoord.has_దత్తాంశం ())
      minMaxCoord.get_min_max (min, max);
    లేకపోతే
    {
      ఒకవేళ (likely (min)) *min = &(ఇది+minCoord);
      ఒకవేళ (likely (max)) *max = &(ఇది+maxCoord);
    }
  }

  శూన్య collect_variation_indices (స్థిరరాశి hb_subset_plan_t* plan,
                                  hb_సమితి_t& varidx_సమితి /*బహిర్గతం*/) స్థిరరాశి
  {
    (ఇది+minCoord).collect_variation_indices (varidx_సమితి);
    (ఇది+maxCoord).collect_variation_indices (varidx_సమితి);
    కొరకు (స్థిరరాశి Feat_Min_Max_రికార్డు& రికార్డు : feat_Min_Max_రికార్డులు)
      రికార్డు.collect_variation_indices (plan, ఇది, varidx_సమితి);
  }

  తార్కిక subset (hb_subset_context_t *c) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    స్వయంచాలక *బహిర్గతం = c->serializer->ప్రారంభం_embed (*ఇది);
    ఒకవేళ (unlikely (!బహిర్గతం || !c->serializer->extend_min (బహిర్గతం))) return_trace (అబద్ధం);

    తార్కిక ret = బహిర్గతం->minCoord.serialize_subset (c, minCoord, ఇది);
    ret |= బహిర్గతం->maxCoord.serialize_subset (c, maxCoord, ఇది);

    గుర్తులేని len = 0;
    కొరకు (స్థిరరాశి Feat_Min_Max_రికార్డు& _ : feat_Min_Max_రికార్డులు)
    {
      hb_ట్యాగ్_t feature_ట్యాగ్ = _.get_feature_ట్యాగ్ ();
      ఒకవేళ (!c->plan->layout_features.has (feature_ట్యాగ్))
        కొనసాగు;

      స్వయంచాలక snap = c->serializer->snapshot ();
      ఒకవేళ (!_.subset (c, ఇది)) c->serializer->revert (snap);
      లేకపోతే len++;
    }

    ఒకవేళ (len > 0) ret = నిజం;
    ఒకవేళ (!ret) return_trace (అబద్ధం);
    return_trace (c->serializer->check_కేటాయించు (బహిర్గతం->feat_Min_Max_రికార్డులు.len, len,
                                               HB_SERIALIZE_ERROR_INT_అధికప్రవాహం));
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (likely (c->check_struct (ఇది) &&
			  minCoord.sanitize (c, ఇది) &&
			  maxCoord.sanitize (c, ఇది) &&
			  feat_Min_Max_రికార్డులు.sanitize (c, ఇది)));
  }

  రక్షిత:
  Offset16_గమ్యం<పునాది_Coord>
		minCoord;	/* Offset to BaseCoord table that defines
				 * minimum extent value, from the beginning
				 * of MinMax table (may be NULL) */
  Offset16_గమ్యం<పునాది_Coord>
		maxCoord;	/* Offset to BaseCoord table that defines
				 * maximum extent value, from the beginning
				 * of MinMax table (may be NULL) */
  Sorted_Array16_యొక్క<Feat_Min_Max_రికార్డు>
		feat_Min_Max_రికార్డులు;
				/* Array of FeatMinMaxRecords, in alphabetical
				 * order by featureTableTag */
  బహిరంగ:
  DEFINE_కొలత_ARRAY (6, feat_Min_Max_రికార్డులు);
};

నిర్మాణం పునాది_విలువలు
{
  స్థిరరాశి పునాది_Coord &get_పునాది_coord (పూర్ణాంకం baseline_ట్యాగ్_సూచీ) స్థిరరాశి
  {
    ఒకవేళ (baseline_ట్యాగ్_సూచీ == -1) baseline_ట్యాగ్_సూచీ = default_సూచీ_2;
    తిరిగి ఇది+పునాది_Coords[baseline_ట్యాగ్_సూచీ];
  }

  శూన్య collect_variation_indices (hb_సమితి_t& varidx_సమితి /*బహిర్గతం*/) స్థిరరాశి
  {
    కొరకు (స్థిరరాశి స్వయంచాలక& _ : పునాది_Coords)
      (ఇది+_).collect_variation_indices (varidx_సమితి);
  }

  తార్కిక subset (hb_subset_context_t *c) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    స్వయంచాలక *బహిర్గతం = c->serializer->ప్రారంభం_embed (*ఇది);
    ఒకవేళ (unlikely (!బహిర్గతం || !c->serializer->extend_min (బహిర్గతం))) return_trace (అబద్ధం);
    బహిర్గతం->default_సూచీ_2 = default_సూచీ_2;

    తార్కిక ret = అబద్ధం;
    కొరకు (స్థిరరాశి స్వయంచాలక& _ : పునాది_Coords)
    {
      స్వయంచాలక *o = బహిర్గతం->పునాది_Coords.serialize_append (c->serializer);
      ఒకవేళ (unlikely (!o)) return_trace (అబద్ధం);
       ret |= o->serialize_subset (c, _, ఇది);
    }

    return_trace (ret);
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (likely (c->check_struct (ఇది) &&
			  పునాది_Coords.sanitize (c, ఇది)));
  }

  రక్షిత:
  సూచీ		default_సూచీ_2;	/* Index number of default baseline for this
				 * script — equals index position of baseline tag
				 * in baselineTags array of the BaseTagList */
  Array16_యొక్క_Offset16_గమ్యం<పునాది_Coord>
		పునాది_Coords;	/* Number of BaseCoord tables defined — should equal
				 * baseTagCount in the BaseTagList
				 *
				 * Array of offsets to BaseCoord tables, from beginning of
				 * BaseValues table — order matches baselineTags array in
				 * the BaseTagList */
  బహిరంగ:
  DEFINE_కొలత_ARRAY (4, పునాది_Coords);
};

నిర్మాణం పునాది_Lang_Sys_రికార్డు
{
  పూర్ణాంకం cmp (hb_ట్యాగ్_t key) స్థిరరాశి { తిరిగి పునాది_Lang_Sys_ట్యాగ్.cmp (key); }

  తార్కిక has_దత్తాంశం () స్థిరరాశి { తిరిగి పునాది_Lang_Sys_ట్యాగ్; }

  స్థిరరాశి MinMax &get_min_max (స్థిరరాశి శూన్య* పునాది) స్థిరరాశి { తిరిగి పునాది+minMax; }

  శూన్య collect_variation_indices (స్థిరరాశి శూన్య* పునాది,
                                  స్థిరరాశి hb_subset_plan_t* plan,
                                  hb_సమితి_t& varidx_సమితి /*బహిర్గతం*/) స్థిరరాశి
  { (పునాది+minMax).collect_variation_indices (plan, varidx_సమితి); }

  తార్కిక subset (hb_subset_context_t *c,
               స్థిరరాశి శూన్య *పునాది) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    స్వయంచాలక *బహిర్గతం = c->serializer->embed (*ఇది);
    ఒకవేళ (unlikely (!బహిర్గతం)) return_trace (అబద్ధం);

    return_trace (బహిర్గతం->minMax.serialize_subset (c, minMax, పునాది));
  }

  తార్కిక sanitize (hb_sanitize_context_t *c, స్థిరరాశి శూన్య *పునాది) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (likely (c->check_struct (ఇది) &&
			  minMax.sanitize (c, పునాది)));
  }

  రక్షిత:
  ట్యాగ్		పునాది_Lang_Sys_ట్యాగ్;	/* 4-byte language system identification tag */
  Offset16_గమ్యం<MinMax>
		minMax;		/* Offset to MinMax table, from beginning
				 * of BaseScript table */
  బహిరంగ:
  DEFINE_కొలత_STATIC (6);
};

నిర్మాణం పునాది_Script
{
  స్థిరరాశి MinMax &get_min_max (hb_ట్యాగ్_t language_ట్యాగ్) స్థిరరాశి
  {
    స్థిరరాశి పునాది_Lang_Sys_రికార్డు& రికార్డు = పునాది_Lang_Sys_రికార్డులు.bsearch (language_ట్యాగ్);
    తిరిగి రికార్డు.has_దత్తాంశం () ? రికార్డు.get_min_max (ఇది) : ఇది+defaultMinMax;
  }

  స్థిరరాశి పునాది_Coord &get_పునాది_coord (పూర్ణాంకం baseline_ట్యాగ్_సూచీ) స్థిరరాశి
  { తిరిగి (ఇది+పునాది_విలువలు_2).get_పునాది_coord (baseline_ట్యాగ్_సూచీ); }

  తార్కిక has_విలువలు () స్థిరరాశి { తిరిగి పునాది_విలువలు_2; }
  తార్కిక has_min_max () స్థిరరాశి { తిరిగి defaultMinMax || పునాది_Lang_Sys_రికార్డులు; }

  శూన్య collect_variation_indices (స్థిరరాశి hb_subset_plan_t* plan,
                                  hb_సమితి_t& varidx_సమితి /*బహిర్గతం*/) స్థిరరాశి
  {
    (ఇది+పునాది_విలువలు_2).collect_variation_indices (varidx_సమితి);
    (ఇది+defaultMinMax).collect_variation_indices (plan, varidx_సమితి);
    
    కొరకు (స్థిరరాశి పునాది_Lang_Sys_రికార్డు& _ : పునాది_Lang_Sys_రికార్డులు)
      _.collect_variation_indices (ఇది, plan, varidx_సమితి);
  }

  తార్కిక subset (hb_subset_context_t *c) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    స్వయంచాలక *బహిర్గతం = c->serializer->ప్రారంభం_embed (*ఇది);
    ఒకవేళ (unlikely (!బహిర్గతం || !c->serializer->extend_min (బహిర్గతం))) return_trace (అబద్ధం);

    తార్కిక ret = బహిర్గతం->పునాది_విలువలు_2.serialize_subset (c, పునాది_విలువలు_2, ఇది);
    ret |= బహిర్గతం->defaultMinMax.serialize_subset (c, defaultMinMax, ఇది);

    గుర్తులేని len = 0;
    కొరకు (స్థిరరాశి స్వయంచాలక& _ : పునాది_Lang_Sys_రికార్డులు)
    {
      స్వయంచాలక snap = c->serializer->snapshot ();
      ఒకవేళ (_.subset (c, ఇది)) {
        ret = నిజం;
        len++;
      }
      లేకపోతే c->serializer->revert (snap);
    }

    ఒకవేళ (!ret) return_trace (అబద్ధం);
    return_trace (c->serializer->check_కేటాయించు (బహిర్గతం->పునాది_Lang_Sys_రికార్డులు.len, len,
                                               HB_SERIALIZE_ERROR_INT_అధికప్రవాహం));
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (likely (c->check_struct (ఇది) &&
			  పునాది_విలువలు_2.sanitize (c, ఇది) &&
			  defaultMinMax.sanitize (c, ఇది) &&
			  పునాది_Lang_Sys_రికార్డులు.sanitize (c, ఇది)));
  }

  రక్షిత:
  Offset16_గమ్యం<పునాది_విలువలు>
		పునాది_విలువలు_2;	/* Offset to BaseValues table, from beginning
				 * of BaseScript table (may be NULL) */
  Offset16_గమ్యం<MinMax>
		defaultMinMax;	/* Offset to MinMax table, from beginning of
				 * BaseScript table (may be NULL) */
  Sorted_Array16_యొక్క<పునాది_Lang_Sys_రికార్డు>
		పునాది_Lang_Sys_రికార్డులు;
				/* Number of BaseLangSysRecords
				 * defined — may be zero (0) */

  బహిరంగ:
  DEFINE_కొలత_ARRAY (6, పునాది_Lang_Sys_రికార్డులు);
};

నిర్మాణం పునాది_Script_List;
నిర్మాణం పునాది_Script_రికార్డు
{
  పూర్ణాంకం cmp (hb_ట్యాగ్_t key) స్థిరరాశి { తిరిగి పునాది_Script_ట్యాగ్.cmp (key); }

  తార్కిక has_దత్తాంశం () స్థిరరాశి { తిరిగి పునాది_Script_ట్యాగ్; }

  hb_ట్యాగ్_t get_script_ట్యాగ్ () స్థిరరాశి { తిరిగి పునాది_Script_ట్యాగ్; }

  స్థిరరాశి పునాది_Script &get_పునాది_script (స్థిరరాశి పునాది_Script_List *list) స్థిరరాశి
  { తిరిగి list+పునాది_Script_2; }

  శూన్య collect_variation_indices (స్థిరరాశి hb_subset_plan_t* plan,
                                  స్థిరరాశి శూన్య* list,
                                  hb_సమితి_t& varidx_సమితి /*బహిర్గతం*/) స్థిరరాశి
  {
    ఒకవేళ (!plan->layout_scripts.has (పునాది_Script_ట్యాగ్))
      తిరిగి;

    (list+పునాది_Script_2).collect_variation_indices (plan, varidx_సమితి);
  }

  తార్కిక subset (hb_subset_context_t *c,
               స్థిరరాశి శూన్య *పునాది) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    స్వయంచాలక *బహిర్గతం = c->serializer->embed (*ఇది);
    ఒకవేళ (unlikely (!బహిర్గతం)) return_trace (అబద్ధం);

    return_trace (బహిర్గతం->పునాది_Script_2.serialize_subset (c, పునాది_Script_2, పునాది));
  }

  తార్కిక sanitize (hb_sanitize_context_t *c, స్థిరరాశి శూన్య *పునాది) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (likely (c->check_struct (ఇది) &&
			  పునాది_Script_2.sanitize (c, పునాది)));
  }

  రక్షిత:
  ట్యాగ్		పునాది_Script_ట్యాగ్;	/* 4-byte script identification tag */
  Offset16_గమ్యం<పునాది_Script>
		పునాది_Script_2;	/* Offset to BaseScript table, from beginning
				 * of BaseScriptList */

  బహిరంగ:
  DEFINE_కొలత_STATIC (6);
};

నిర్మాణం పునాది_Script_List
{
  స్థిరరాశి పునాది_Script &get_పునాది_script (hb_ట్యాగ్_t script) స్థిరరాశి
  {
    స్థిరరాశి పునాది_Script_రికార్డు *రికార్డు = &పునాది_Script_రికార్డులు.bsearch (script);
    ఒకవేళ (!రికార్డు->has_దత్తాంశం ()) రికార్డు = &పునాది_Script_రికార్డులు.bsearch (HB_ట్యాగ్ ('D','F','L','T'));
    తిరిగి రికార్డు->has_దత్తాంశం () ? రికార్డు->get_పునాది_script (ఇది) : Null (పునాది_Script);
  }

  శూన్య collect_variation_indices (స్థిరరాశి hb_subset_plan_t* plan,
                                  hb_సమితి_t& varidx_సమితి /*బహిర్గతం*/) స్థిరరాశి
  {
    కొరకు (స్థిరరాశి పునాది_Script_రికార్డు& _ : పునాది_Script_రికార్డులు)
      _.collect_variation_indices (plan, ఇది, varidx_సమితి);
  }

  తార్కిక subset (hb_subset_context_t *c) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    స్వయంచాలక *బహిర్గతం = c->serializer->ప్రారంభం_embed (*ఇది);
    ఒకవేళ (unlikely (!బహిర్గతం || !c->serializer->extend_min (బహిర్గతం))) return_trace (అబద్ధం);

    గుర్తులేని len = 0;
    కొరకు (స్థిరరాశి పునాది_Script_రికార్డు& _ : పునాది_Script_రికార్డులు)
    {
      hb_ట్యాగ్_t script_ట్యాగ్ = _.get_script_ట్యాగ్ ();
      ఒకవేళ (!c->plan->layout_scripts.has (script_ట్యాగ్))
        కొనసాగు;

      స్వయంచాలక snap = c->serializer->snapshot ();
      ఒకవేళ (!_.subset (c, ఇది)) c->serializer->revert (snap);
      లేకపోతే len++;
    }

    ఒకవేళ (!len) return_trace (అబద్ధం);
    return_trace (c->serializer->check_కేటాయించు (బహిర్గతం->పునాది_Script_రికార్డులు.len, len,
                                               HB_SERIALIZE_ERROR_INT_అధికప్రవాహం));
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది) &&
		  పునాది_Script_రికార్డులు.sanitize (c, ఇది));
  }

  రక్షిత:
  Sorted_Array16_యొక్క<పునాది_Script_రికార్డు>
			పునాది_Script_రికార్డులు;

  బహిరంగ:
  DEFINE_కొలత_ARRAY (2, పునాది_Script_రికార్డులు);
};

నిర్మాణం Axis
{
  తార్కిక get_baseline (hb_ట్యాగ్_t          baseline_ట్యాగ్,
		     hb_ట్యాగ్_t          script_ట్యాగ్,
		     hb_ట్యాగ్_t          language_ట్యాగ్,
		     స్థిరరాశి పునాది_Coord **coord) స్థిరరాశి
  {
    స్థిరరాశి పునాది_Script &పునాది_script = (ఇది+పునాది_Script_List_2).get_పునాది_script (script_ట్యాగ్);
    ఒకవేళ (!పునాది_script.has_విలువలు ())
    {
      *coord = శూన్యసూచిక;
      తిరిగి అబద్ధం;
    }

    ఒకవేళ (likely (coord))
    {
      గుర్తులేని పూర్ణాంకం ట్యాగ్_సూచీ = 0;
      ఒకవేళ (!(ఇది+పునాది_ట్యాగ్_List).bfind (baseline_ట్యాగ్, &ట్యాగ్_సూచీ))
      {
        *coord = శూన్యసూచిక;
        తిరిగి అబద్ధం;
      }
      *coord = &పునాది_script.get_పునాది_coord (ట్యాగ్_సూచీ);
    }

    తిరిగి నిజం;
  }

  తార్కిక get_min_max (hb_ట్యాగ్_t          script_ట్యాగ్,
		    hb_ట్యాగ్_t          language_ట్యాగ్,
		    hb_ట్యాగ్_t          feature_ట్యాగ్,
		    స్థిరరాశి పునాది_Coord **min_coord,
		    స్థిరరాశి పునాది_Coord **max_coord) స్థిరరాశి
  {
    స్థిరరాశి పునాది_Script &పునాది_script = (ఇది+పునాది_Script_List_2).get_పునాది_script (script_ట్యాగ్);
    ఒకవేళ (!పునాది_script.has_min_max ())
    {
      *min_coord = *max_coord = శూన్యసూచిక;
      తిరిగి అబద్ధం;
    }

    పునాది_script.get_min_max (language_ట్యాగ్).get_min_max (feature_ట్యాగ్, min_coord, max_coord);

    తిరిగి నిజం;
  }

  శూన్య collect_variation_indices (స్థిరరాశి hb_subset_plan_t* plan,
                                  hb_సమితి_t& varidx_సమితి /*బహిర్గతం*/) స్థిరరాశి
  { (ఇది+పునాది_Script_List_2).collect_variation_indices (plan, varidx_సమితి); }

  తార్కిక subset (hb_subset_context_t *c) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    స్వయంచాలక *బహిర్గతం = c->serializer->embed (*ఇది);
    ఒకవేళ (unlikely (!బహిర్గతం)) return_trace (అబద్ధం);

    తార్కిక ret = బహిర్గతం->పునాది_ట్యాగ్_List.serialize_copy (c->serializer, పునాది_ట్యాగ్_List, ఇది);
    ret |= బహిర్గతం->పునాది_Script_List_2.serialize_subset (c, పునాది_Script_List_2, ఇది);
    return_trace (ret);
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (likely (c->check_struct (ఇది) &&
			  పునాది_ట్యాగ్_List.sanitize (c, ఇది) &&
			  పునాది_Script_List_2.sanitize (c, ఇది)));
  }

  రక్షిత:
  Offset16_గమ్యం<Sorted_Array16_యొక్క<ట్యాగ్>>
		పునాది_ట్యాగ్_List;	/* Offset to BaseTagList table, from beginning
				 * of Axis table (may be NULL)
				 * Array of 4-byte baseline identification tags — must
				 * be in alphabetical order */
  Offset16_గమ్యం<పునాది_Script_List>
		పునాది_Script_List_2;	/* Offset to BaseScriptList table, from beginning
				 * of Axis table
				 * Array of BaseScriptRecords, in alphabetical order
				 * by baseScriptTag */

  బహిరంగ:
  DEFINE_కొలత_STATIC (4);
};

నిర్మాణం పునాది_4
{
  స్థిర constexpr hb_ట్యాగ్_t పట్టిక_ట్యాగ్ = HB_OT_ట్యాగ్_పునాది;

  స్థిరరాశి Axis &get_axis (hb_direction_t direction) స్థిరరాశి
  { తిరిగి HB_DIRECTION_IS_VERTICAL (direction) ? ఇది+vAxis : ఇది+hAxis; }

  తార్కిక has_var_store () స్థిరరాశి
  { తిరిగి సంస్కరణ.గమ్యం_int () >= 0x00010001u && varStore != 0; }

  స్థిరరాశి అంశం_Variation_Store &get_var_store () స్థిరరాశి
  { తిరిగి సంస్కరణ.గమ్యం_int () < 0x00010001u ? Null (అంశం_Variation_Store) : ఇది+varStore; }

  శూన్య collect_variation_indices (స్థిరరాశి hb_subset_plan_t* plan,
                                  hb_సమితి_t& varidx_సమితి /*బహిర్గతం*/) స్థిరరాశి
  {
    (ఇది+hAxis).collect_variation_indices (plan, varidx_సమితి);
    (ఇది+vAxis).collect_variation_indices (plan, varidx_సమితి);
  }

  తార్కిక subset_varstore (hb_subset_context_t *c,
                        పునాది_4 *బహిర్గతం /*బహిర్గతం*/) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    ఒకవేళ (!c->serializer->allocate_కొలత<Offset32_గమ్యం<అంశం_Variation_Store>> (Offset32_గమ్యం<అంశం_Variation_Store>::static_కొలత))
        return_trace (అబద్ధం);
    ఒకవేళ (!c->plan->normalized_coords)
      return_trace (బహిర్గతం->varStore.serialize_subset (c, varStore, ఇది, c->plan->పునాది_varstore_inner_maps.as_array ()));

    ఒకవేళ (c->plan->all_axes_pinned)
      return_trace (నిజం);

    అంశం_variations_t అంశం_vars;
    ఒకవేళ (!అంశం_vars.instantiate (ఇది+varStore, c->plan, నిజం, నిజం,
                                c->plan->పునాది_varstore_inner_maps.as_array ()))
      return_trace (అబద్ధం);

    ఒకవేళ (!బహిర్గతం->varStore.serialize_serialize (c->serializer,
                                            అంశం_vars.has_long_మాట (),
                                            c->plan->axis_ట్యాగులు,
                                            అంశం_vars.get_region_list (),
                                            అంశం_vars.get_vardata_encodings ()))
      return_trace (అబద్ధం);

    స్థిరరాశి hb_map_t &varidx_map = అంశం_vars.get_varidx_map ();
    /* base_variation_idx_map in the plan is old_varidx->(varidx, delta)
     * mapping, new varidx is generated for subsetting, we need to remap this
     * after instancing */
    కొరకు (స్వయంచాలక _ : c->plan->పునాది_variation_idx_map.iter_ref ())
    {
      uint32_t varidx = _.second.మొదటి;
      uint32_t *new_varidx;
      ఒకవేళ (varidx_map.has (varidx, &new_varidx))
        _.second.మొదటి = *new_varidx;
      లేకపోతే
        _.second.మొదటి = HB_OT_LAYOUT_NO_VARIATIONS_సూచీ;
    }
    return_trace (నిజం);
  }

  తార్కిక subset (hb_subset_context_t *c) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    స్వయంచాలక *బహిర్గతం = c->serializer->ప్రారంభం_embed (*ఇది);
    ఒకవేళ (unlikely (!బహిర్గతం || !c->serializer->extend_min (బహిర్గతం))) return_trace (అబద్ధం);

    బహిర్గతం->సంస్కరణ = సంస్కరణ;
    తార్కిక ret = అబద్ధం;
    ఒకవేళ (సంస్కరణ.గమ్యం_int () >= 0x00010001u) ret |= subset_varstore (c, బహిర్గతం);

    ret |= బహిర్గతం->hAxis.serialize_subset (c, hAxis, ఇది);
    ret |= బహిర్గతం->vAxis.serialize_subset (c, vAxis, ఇది);

    return_trace (ret);
  }

  తార్కిక get_baseline (hb_font_t      *font,
		     hb_ట్యాగ్_t        baseline_ట్యాగ్,
		     hb_direction_t  direction,
		     hb_ట్యాగ్_t        script_ట్యాగ్,
		     hb_ట్యాగ్_t        language_ట్యాగ్,
		     hb_position_t  *పునాది) స్థిరరాశి
  {
    స్థిరరాశి పునాది_Coord *పునాది_coord = శూన్యసూచిక;
    ఒకవేళ (unlikely (!get_axis (direction).get_baseline (baseline_ట్యాగ్, script_ట్యాగ్, language_ట్యాగ్, &పునాది_coord) ||
		  !పునాది_coord || !పునాది_coord->has_దత్తాంశం ()))
      తిరిగి అబద్ధం;

    ఒకవేళ (likely (పునాది))
      *పునాది = పునాది_coord->get_coord (font, get_var_store (), direction);

    తిరిగి నిజం;
  }

  తార్కిక get_min_max (hb_font_t      *font,
		    hb_direction_t  direction,
		    hb_ట్యాగ్_t        script_ట్యాగ్,
		    hb_ట్యాగ్_t        language_ట్యాగ్,
		    hb_ట్యాగ్_t        feature_ట్యాగ్,
		    hb_position_t  *min,
		    hb_position_t  *max) స్థిరరాశి
  {
    స్థిరరాశి పునాది_Coord *min_coord, *max_coord;
    ఒకవేళ (!get_axis (direction).get_min_max (script_ట్యాగ్, language_ట్యాగ్, feature_ట్యాగ్,
					   &min_coord, &max_coord))
      తిరిగి అబద్ధం;

    స్థిరరాశి అంశం_Variation_Store &var_store = get_var_store ();
    ఒకవేళ (likely (min && min_coord)) *min = min_coord->get_coord (font, var_store, direction);
    ఒకవేళ (likely (max && max_coord)) *max = max_coord->get_coord (font, var_store, direction);
    తిరిగి నిజం;
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (likely (c->check_struct (ఇది) &&
			  hb_barrier () &&
			  likely (సంస్కరణ.major == 1) &&
			  hAxis.sanitize (c, ఇది) &&
			  vAxis.sanitize (c, ఇది) &&
			  (సంస్కరణ.గమ్యం_int () < 0x00010001u || varStore.sanitize (c, ఇది))));
  }

  రక్షిత:
  Fixed_సంస్కరణ<>సంస్కరణ;	/* Version of the BASE table */
  Offset16_గమ్యం<Axis>hAxis;		/* Offset to horizontal Axis table, from beginning
				 * of BASE table (may be NULL) */
  Offset16_గమ్యం<Axis>vAxis;		/* Offset to vertical Axis table, from beginning
				 * of BASE table (may be NULL) */
  Offset32_గమ్యం<అంశం_Variation_Store>
		varStore;	/* Offset to the table of Item Variation
				 * Store--from beginning of BASE
				 * header (may be NULL).  Introduced
				 * in version 0x00010001. */
  బహిరంగ:
  DEFINE_కొలత_MIN (8);
};


} /* namespace OT */


#అయితే_ముగింపు /* HB_OT_LAYOUT_BASE_TABLE_HH */
