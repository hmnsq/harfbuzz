/*
 * Copyright © 2017  Google, Inc.
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

#నిర్వచితం_కాకపోతే HB_OT_VAR_AVAR_పట్టిక_HH
#నిర్వచించు HB_OT_VAR_AVAR_పట్టిక_HH

#అన్తర్భావ "hb-open-type.hh"
#అన్తర్భావ "hb-ot-var-common.hh"
#అన్తర్భావ "hb-ot-var-fvar-table.hh"


/*
 * avar -- Axis Variations
 * https://docs.microsoft.com/en-us/typography/opentype/spec/avar
 */

#నిర్వచించు HB_OT_ట్యాగ్_avar HB_ట్యాగ్('a','v','a','r')


నామాకాశ OT {


/* "Spec": https://github.com/be-fonts/boring-expansion-spec/issues/14 */
నిర్మాణం avar_V2_తోక
{
  మిత్ర నిర్మాణం avar;

  తార్కిక sanitize (hb_sanitize_context_t *c,
		 స్థిరరాశి శూన్య *పునాది) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (varIdxMap.sanitize (c, పునాది) &&
		  varStore.sanitize (c, పునాది));
  }

  రక్షిత:
  Offset32_గమ్యం<Delta_సమితి_సూచీ_Map>	varIdxMap;	/* Offset from the beginning of 'avar' table. */
  Offset32_గమ్యం<అంశం_Variation_Store>	varStore;	/* Offset from the beginning of 'avar' table. */

  బహిరంగ:
  DEFINE_కొలత_STATIC (8);
};


నిర్మాణం Axis_విలువ_Map
{
  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది));
  }

  శూన్య సమితి_mapping (దశాంశం from_coord, దశాంశం గమ్యం_coord)
  {
    coords[0].సమితి_float (from_coord);
    coords[1].సమితి_float (గమ్యం_coord);
  }

  తార్కిక is_outside_axis_range (స్థిరరాశి Triple& axis_range) స్థిరరాశి
  {
    ద్విగుణ from_coord = (ద్విగుణ) coords[0].గమ్యం_float ();
    తిరిగి !axis_range.కలిగియుంది (from_coord);
  }

  తార్కిక must_include () స్థిరరాశి
  {
    దశాంశం from_coord = coords[0].గమ్యం_float ();
    దశాంశం గమ్యం_coord = coords[1].గమ్యం_float ();
    తిరిగి (from_coord == -1.f && గమ్యం_coord == -1.f) ||
           (from_coord == 0.f && గమ్యం_coord == 0.f) ||
           (from_coord == 1.f && గమ్యం_coord == 1.f);
  }

  శూన్య instantiate (స్థిరరాశి Triple& axis_range,
                    స్థిరరాశి Triple& unmapped_range,
                    స్థిరరాశి TripleDistances& triple_distances)
  {
    దశాంశం from_coord = coords[0].గమ్యం_float ();
    దశాంశం గమ్యం_coord = coords[1].గమ్యం_float ();

    from_coord = renormalize_విలువ ((ద్విగుణ) from_coord, unmapped_range, triple_distances);
    గమ్యం_coord = renormalize_విలువ ((ద్విగుణ) గమ్యం_coord, axis_range, triple_distances);

    /* Malformed input can renormalize outside [-1,1], which would
     * overflow F2DOT14. */
    from_coord = hb_clamp (from_coord, -1.f, 1.f);
    గమ్యం_coord = hb_clamp (గమ్యం_coord, -1.f, 1.f);

    coords[0].సమితి_float (from_coord);
    coords[1].సమితి_float (గమ్యం_coord);
  }

  HB_అంతర్గత స్థిర పూర్ణాంకం cmp (స్థిరరాశి శూన్య *pa, స్థిరరాశి శూన్య *pb)
  {
    స్థిరరాశి Axis_విలువ_Map *a = (స్థిరరాశి Axis_విలువ_Map *) pa;
    స్థిరరాశి Axis_విలువ_Map *b = (స్థిరరాశి Axis_విలువ_Map *) pb;

    పూర్ణాంకం a_from = a->coords[0].గమ్యం_int ();
    పూర్ణాంకం b_from = b->coords[0].గమ్యం_int ();
    ఒకవేళ (a_from != b_from)
      తిరిగి a_from - b_from;

    /* this should never be reached. according to the spec, all of the axis
     * value map records for a given axis must have different fromCoord values
     * */
    పూర్ణాంకం a_గమ్యం = a->coords[1].గమ్యం_int ();
    పూర్ణాంకం b_గమ్యం = b->coords[1].గమ్యం_int ();
    తిరిగి a_గమ్యం - b_గమ్యం;
  }

  తార్కిక serialize (hb_serialize_context_t *c) స్థిరరాశి
  {
    TRACE_SERIALIZE (ఇది);
    return_trace (c->embed (ఇది));
  }

  బహిరంగ:
  F2DOT14	coords[2];
//   F2DOT14	fromCoord;	/* A normalized coordinate value obtained using
//				 * default normalization. */
//   F2DOT14	toCoord;	/* The modified, normalized coordinate value. */

  బహిరంగ:
  DEFINE_కొలత_STATIC (4);
};

నిర్మాణం SegmentMaps : Array16_యొక్క<Axis_విలువ_Map>
{
  దశాంశం map_float (దశాంశం value, గుర్తులేని పూర్ణాంకం from_స్థానభ్రంశం = 0, గుర్తులేని పూర్ణాంకం గమ్యం_స్థానభ్రంశం = 1) స్థిరరాశి
  {
#నిర్వచించు fromCoord coords[from_స్థానభ్రంశం].గమ్యం_float ()
#నిర్వచించు గమ్యం_Coord coords[గమ్యం_స్థానభ్రంశం].గమ్యం_float ()

    స్థిరరాశి స్వయంచాలక *map = arrayZ;

    /* The following special-cases are not part of OpenType, which requires
     * that at least -1, 0, and +1 must be mapped. But we include these as
     * part of a better error recovery scheme. */
    ఒకవేళ (len < 2)
    {
      ఒకవేళ (!len)
	తిరిగి value;
      లేకపోతే /* len == 1*/
	తిరిగి value - map[0].fromCoord + map[0].గమ్యం_Coord;
    }

    // At least two mappings now.

    /* CoreText is wild...
     * PingFangUI avar needs all this special-casing...
     * So we implement an extended version of the spec here,
     * which is more robust and more likely to be compatible with
     * the wild. */

    గుర్తులేని ప్రారంభం = 0;
    గుర్తులేని చివర = len;
    ఒకవేళ (map[ప్రారంభం].fromCoord == -1 && map[ప్రారంభం].గమ్యం_Coord == -1 && map[ప్రారంభం+1].fromCoord == -1)
      ప్రారంభం++;
    ఒకవేళ (map[చివర-1].fromCoord == +1 && map[చివర-1].గమ్యం_Coord == +1 && map[చివర-2].fromCoord == +1)
      చివర--;

    /* Look for exact match first, and do lots of special-casing. */
    గుర్తులేని i;
    కొరకు (i = ప్రారంభం; i < చివర; i++)
      ఒకవేళ (value == map[i].fromCoord)
	ఆపు;
    ఒకవేళ (i < చివర)
    {
      // There's at least one exact match. See if there are more.
      గుర్తులేని j = i;
      కొరకు (; j + 1 < చివర; j++)
	ఒకవేళ (value != map[j + 1].fromCoord)
	  ఆపు;

      // [i,j] inclusive are all exact matches:

      // If there's only one, return it. This is the only spec-compliant case.
      ఒకవేళ (i == j)
	తిరిగి map[i].గమ్యం_Coord;
      // If there's exactly three, return the middle one.
      ఒకవేళ (i + 2 == j)
	తిరిగి map[i + 1].గమ్యం_Coord;

      // Ignore the middle ones. Return the one mapping closer to 0.
      ఒకవేళ (value < 0) తిరిగి map[j].గమ్యం_Coord;
      ఒకవేళ (value > 0) తిరిగి map[i].గమ్యం_Coord;

      // Mapping 0? CoreText seems confused. It seems to prefer 0 here...
      // So we'll just return the smallest one. lol
      తిరిగి fabsf (map[i].గమ్యం_Coord) < fabsf (map[j].గమ్యం_Coord) ? map[i].గమ్యం_Coord : map[j].గమ్యం_Coord;

      // Mapping 0? Return one not mapping to 0.
      ఒకవేళ (map[i].గమ్యం_Coord == 0)
	తిరిగి map[j].గమ్యం_Coord;
      లేకపోతే
	తిరిగి map[i].గమ్యం_Coord;
    }

    /* There's at least two and we're not an exact match. Prepare to lerp. */

    // Find the segment we're in.
    కొరకు (i = ప్రారంభం; i < చివర; i++)
      ఒకవేళ (value < map[i].fromCoord)
	ఆపు;

    ఒకవేళ (i == ప్రారంభం)
    {
      // Value before all segments; Shift.
      తిరిగి value - map[ప్రారంభం].fromCoord + map[ప్రారంభం].గమ్యం_Coord;
    }
    ఒకవేళ (i == చివర)
    {
      // Value after all segments; Shift.
      తిరిగి value - map[చివర - 1].fromCoord + map[చివర - 1].గమ్యం_Coord;
    }

    // Actually interpolate.
    స్వయంచాలక &before = map[i-1];
    స్వయంచాలక &after = map[i];
    దశాంశం denom = after.fromCoord - before.fromCoord; // Can't be zero by now.
    తిరిగి before.గమ్యం_Coord + ((after.గమ్యం_Coord - before.గమ్యం_Coord) * (value - before.fromCoord)) / denom;

#నిర్వచనం_రద్దు గమ్యం_Coord
#నిర్వచనం_రద్దు fromCoord
  }

  దశాంశం unmap_float (దశాంశం value) స్థిరరాశి { తిరిగి map_float (value, 1, 0); }


  // TODO Kill this.
  Triple unmap_axis_range (స్థిరరాశి Triple& axis_range) స్థిరరాశి
  {
    దశాంశం unmapped_min = unmap_float (axis_range.minimum);
    దశాంశం unmapped_మధ్య = unmap_float (axis_range.మధ్య);
    దశాంశం unmapped_max = unmap_float (axis_range.గరిష్ఠ);

    తిరిగి Triple{unmapped_min, unmapped_మధ్య, unmapped_max};
  }

  తార్కిక subset (hb_subset_context_t *c, hb_ట్యాగ్_t axis_ట్యాగ్_2,
               hb_సదిశ_t<Axis_విలువ_Map> *బహిర్గతం_mappings = శూన్యసూచిక) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);

    /* This function cannot work on avar2 table (and currently doesn't).
     * We should instead keep the design coords in the shape plan and use
     * those. unmap_axis_range needs to be killed. */

    /* avar mapped normalized axis range. Under avar2, axes_location holds
     * only the self-contained pins for the other tables; avar itself uses
     * the intermediate-space ranges of all restricted axes. */
    స్థిరరాశి స్వయంచాలక &axes_location = c->plan->has_avar2
				? c->plan->old_intermediates
				: c->plan->axes_location;
    Triple *axis_range;
    ఒకవేళ (!axes_location.has (axis_ట్యాగ్_2, &axis_range))
      తిరిగి c->serializer->embed (*ఇది);

    TripleDistances *axis_triple_distances;
    ఒకవేళ (!c->plan->axes_triple_distances.has (axis_ట్యాగ్_2, &axis_triple_distances))
      return_trace (అబద్ధం);

    స్వయంచాలక *బహిర్గతం = c->serializer->ప్రారంభం_embed (ఇది);
    ఒకవేళ (unlikely (!c->serializer->extend_min (బహిర్గతం))) return_trace (అబద్ధం);

    Triple unmapped_range = unmap_axis_range (*axis_range);

    /* create a vector of retained mappings and sort */
    hb_సదిశ_t<Axis_విలువ_Map> విలువ_mappings;
    కొరకు (స్థిరరాశి స్వయంచాలక& _ : as_array ())
    {
      ఒకవేళ (_.is_outside_axis_range (unmapped_range))
        కొనసాగు;
      Axis_విలువ_Map mapping;
      mapping = _;
      mapping.instantiate (*axis_range, unmapped_range, *axis_triple_distances);
      /* (-1, -1), (0, 0), (1, 1) mappings will be added later, so avoid
       * duplicates here */
      ఒకవేళ (mapping.must_include ())
        కొనసాగు;
      విలువ_mappings.push (mapping);
    }

    Axis_విలువ_Map m;
    m.సమితి_mapping (-1.f, -1.f);
    విలువ_mappings.push (m);

    m.సమితి_mapping (0.f, 0.f);
    విలువ_mappings.push (m);

    m.సమితి_mapping (1.f, 1.f);
    విలువ_mappings.push (m);

    విలువ_mappings.qsort ();

    ఒకవేళ (unlikely (విలువ_mappings.లోపల_error ()))
      return_trace (అబద్ధం);

    కొరకు (స్థిరరాశి స్వయంచాలక& _ : విలువ_mappings)
    {
      ఒకవేళ (!_.serialize (c->serializer))
        return_trace (అబద్ధం);
    }
    ఒకవేళ (!c->serializer->check_కేటాయించు (బహిర్గతం->len, విలువ_mappings.నిడివి, HB_SERIALIZE_ERROR_INT_అధికప్రవాహం))
      return_trace (అబద్ధం);
    /* Hand the instantiated mappings to the caller; avar2 offset
     * compensation needs them to locate the new mapping's kinks. */
    ఒకవేళ (బహిర్గతం_mappings)
      *బహిర్గతం_mappings = std::move (విలువ_mappings);
    return_trace (నిజం);
  }

  బహిరంగ:
  DEFINE_కొలత_ARRAY (2, *ఇది);
};

/* One knot of the avar2 offset-compensation function
 * offset(z) = inv_renorm(z) - z, in new intermediate space. */
నిర్మాణం avar2_స్థానభ్రంశం_knot_t
{
  ద్విగుణ z;
  ద్విగుణ స్థానభ్రంశం;
};

/* Insert a knot keeping the vector sorted by z; first insertion wins on
 * duplicate z. */
స్థిర అంతఃవరుస శూన్య
avar2_కూడు_knot (hb_సదిశ_t<avar2_స్థానభ్రంశం_knot_t> &knots, ద్విగుణ z, ద్విగుణ స్థానభ్రంశం)
{
  గుర్తులేని i = 0;
  అయితే (i < knots.నిడివి && knots.arrayZ[i].z < z) i++;
  ఒకవేళ (i < knots.నిడివి && knots.arrayZ[i].z == z) తిరిగి;
  knots.push (avar2_స్థానభ్రంశం_knot_t {0.0, 0.0});
  ఒకవేళ (unlikely (knots.లోపల_error ())) తిరిగి;
  కొరకు (గుర్తులేని j = knots.నిడివి - 1; j > i; j--)
    knots.arrayZ[j] = knots.arrayZ[j - 1];
  knots.arrayZ[i] = avar2_స్థానభ్రంశం_knot_t {z, స్థానభ్రంశం};
}

/* Piecewise-linear evaluation over the sorted knots. Inputs are always
 * within [-1, +1] and anchor knots at -1/0/+1 always exist. */
స్థిర అంతఃవరుస ద్విగుణ
avar2_eval_స్థానభ్రంశం (స్థిరరాశి hb_సదిశ_t<avar2_స్థానభ్రంశం_knot_t> &knots, ద్విగుణ z)
{
  గుర్తులేని len = knots.నిడివి;
  కొరకు (గుర్తులేని i = 0; i < len; i++)
  {
    ఒకవేళ (z == knots.arrayZ[i].z) తిరిగి knots.arrayZ[i].స్థానభ్రంశం;
    ఒకవేళ (z < knots.arrayZ[i].z)
    {
      ఒకవేళ (!i) తిరిగి knots.arrayZ[0].స్థానభ్రంశం;
      స్థిరరాశి స్వయంచాలక &before = knots.arrayZ[i - 1];
      స్థిరరాశి స్వయంచాలక &after = knots.arrayZ[i];
      ద్విగుణ denom = after.z - before.z;
      తిరిగి before.స్థానభ్రంశం + (after.స్థానభ్రంశం - before.స్థానభ్రంశం) * (z - before.z) / denom;
    }
  }
  తిరిగి len ? knots.arrayZ[len - 1].స్థానభ్రంశం : 0.0;
}

/* Piecewise-linear evaluation over instantiated avar v1 mappings, as
 * produced by SegmentMaps::subset (sorted, with -1/0/+1 anchors).
 * Matches SegmentMaps::map_float for such well-formed mappings. */
స్థిర అంతఃవరుస ద్విగుణ
_avar2_map_new_mapping (స్థిరరాశి hb_సదిశ_t<Axis_విలువ_Map> &mappings, ద్విగుణ v,
			గుర్తులేని from_స్థానభ్రంశం = 0, గుర్తులేని గమ్యం_స్థానభ్రంశం = 1)
{
  గుర్తులేని len = mappings.నిడివి;
  ఒకవేళ (!len) తిరిగి v;

  కొరకు (గుర్తులేని i = 0; i < len; i++)
  {
    ద్విగుణ from = (ద్విగుణ) mappings.arrayZ[i].coords[from_స్థానభ్రంశం].గమ్యం_float ();
    ఒకవేళ (v == from)
      తిరిగి (ద్విగుణ) mappings.arrayZ[i].coords[గమ్యం_స్థానభ్రంశం].గమ్యం_float ();
    ఒకవేళ (v < from)
    {
      ద్విగుణ to = (ద్విగుణ) mappings.arrayZ[i].coords[గమ్యం_స్థానభ్రంశం].గమ్యం_float ();
      ఒకవేళ (!i) తిరిగి v - from + to;
      ద్విగుణ prev_from = (ద్విగుణ) mappings.arrayZ[i - 1].coords[from_స్థానభ్రంశం].గమ్యం_float ();
      ద్విగుణ prev_గమ్యం = (ద్విగుణ) mappings.arrayZ[i - 1].coords[గమ్యం_స్థానభ్రంశం].గమ్యం_float ();
      ద్విగుణ denom = from - prev_from;
      ఒకవేళ (denom == 0.0) తిరిగి prev_గమ్యం;
      తిరిగి prev_గమ్యం + (to - prev_గమ్యం) * (v - prev_from) / denom;
    }
  }
  తిరిగి v - (ద్విగుణ) mappings.arrayZ[len - 1].coords[from_స్థానభ్రంశం].గమ్యం_float ()
           + (ద్విగుణ) mappings.arrayZ[len - 1].coords[గమ్యం_స్థానభ్రంశం].గమ్యం_float ();
}

/* Inverse of _avar2_map_new_mapping: pull an output coordinate back to a
 * preimage input coordinate. For a non-strictly-monotone mapping this picks
 * one preimage, which is fine for its only use (augmenting the error
 * estimator's sample set). */
స్థిర అంతఃవరుస ద్విగుణ
_avar2_unmap_new_mapping (స్థిరరాశి hb_సదిశ_t<Axis_విలువ_Map> &mappings, ద్విగుణ v)
{
  తిరిగి _avar2_map_new_mapping (mappings, v, 1, 0);
}

/* Plain (non-avar) fvar-style normalization of a user value against a
 * user-space (min, default, max) triple. */
స్థిర అంతఃవరుస ద్విగుణ
avar2_normalize_విలువ (ద్విగుణ v, ద్విగుణ min, ద్విగుణ def, ద్విగుణ max)
{
  v = hb_clamp (v, min, max);
  ఒకవేళ (v == def) తిరిగి 0.0;
  ఒకవేళ (v < def) తిరిగి def == min ? 0.0 : (v - def) / (def - min);
  తిరిగి def == max ? 0.0 : (v - def) / (max - def);
}

/* Inverse of the above: map a normalized value back to user space. */
స్థిర అంతఃవరుస ద్విగుణ
avar2_denormalize_విలువ (ద్విగుణ v, ద్విగుణ min, ద్విగుణ def, ద్విగుణ max)
{
  ఒకవేళ (v == 0.0) తిరిగి def;
  తిరిగి v < 0.0 ? def + v * (def - min) : def + v * (max - def);
}

/* Estimate the residual offset-compensation error for one restricted axis:
 * the max |old-avar1-final - (new-avar1 + offset)| over the retained user
 * range, in F2Dot14 units. offset(z) is the piecewise-linear function
 * through the knots. The residual is dominated by F2Dot14 requantization of
 * a steep retained avar v1 segment (e.g. a moved default compressing part
 * of the axis into a narrow z band); offset compensation cannot remove it.
 * Used only to pick the better knot set and decide whether to warn.
 *
 * Sampled on a uniform grid augmented with the user-space preimages of
 * every kink of the residual (old/new avar v1 breakpoints and offset(z)
 * knots), so the worst kink cannot fall between uniform samples. The
 * pointwise F2Dot14 rounding makes this an estimate rather than an exact
 * bound, but every piecewise-linear extremum is visited. */
స్థిర అంతఃవరుస గుర్తులేని
avar2_estimate_స్థానభ్రంశం_error (స్థిరరాశి SegmentMaps &old_seg,
                              స్థిరరాశి hb_సదిశ_t<Axis_విలువ_Map> &new_mapping,
                              ద్విగుణ old_min, ద్విగుణ old_def, ద్విగుణ old_max,
                              ద్విగుణ new_min, ద్విగుణ new_def, ద్విగుణ new_max,
                              స్థిరరాశి hb_సదిశ_t<avar2_స్థానభ్రంశం_knot_t> &knots)
{
  constexpr గుర్తులేని samples = 257;
  hb_సదిశ_t<ద్విగుణ> us;
  ఒకవేళ (unlikely (!us.alloc (samples + old_seg.as_array ().నిడివి +
                           new_mapping.నిడివి + knots.నిడివి)))
    తిరిగి UINT_MAX; /* estimator only; fail towards "worse" */
  కొరకు (గుర్తులేని i = 0; i < samples; i++)
    us.push (new_min + (new_max - new_min) * i / (samples - 1));
  కొరకు (స్థిరరాశి స్వయంచాలక &_ : old_seg.as_array ())
    us.push (avar2_denormalize_విలువ ((ద్విగుణ) _.coords[0].గమ్యం_float (),
                                       old_min, old_def, old_max));
  కొరకు (స్థిరరాశి స్వయంచాలక &_ : new_mapping)
    us.push (avar2_denormalize_విలువ ((ద్విగుణ) _.coords[0].గమ్యం_float (),
                                       new_min, new_def, new_max));
  కొరకు (స్థిరరాశి స్వయంచాలక &knot : knots)
    us.push (avar2_denormalize_విలువ (
               _avar2_unmap_new_mapping (new_mapping, knot.z),
               new_min, new_def, new_max));
  ఒకవేళ (unlikely (us.లోపల_error ())) తిరిగి UINT_MAX;

  గుర్తులేని max_err = 0;
  కొరకు (ద్విగుణ u : us)
  {
    ఒకవేళ (u < new_min || u > new_max) కొనసాగు;
    ద్విగుణ n_old = avar2_normalize_విలువ (u, old_min, old_def, old_max);
    ద్విగుణ old_final = (ద్విగుణ) old_seg.map_float ((దశాంశం) n_old);
    ద్విగుణ n_new = avar2_normalize_విలువ (u, new_min, new_def, new_max);
    ద్విగుణ z = _avar2_map_new_mapping (new_mapping, n_new);
    ద్విగుణ new_final = z + avar2_eval_స్థానభ్రంశం (knots, z);
    పూర్ణాంకం err = నిరపేక్ష ((పూర్ణాంకం) roundf ((దశాంశం) (old_final * 16384.0)) -
                   (పూర్ణాంకం) roundf ((దశాంశం) (new_final * 16384.0)));
    ఒకవేళ ((గుర్తులేని) err > max_err) max_err = err;
  }
  తిరిగి max_err;
}

నిర్మాణం avar
{
  స్థిర constexpr hb_ట్యాగ్_t పట్టిక_ట్యాగ్ = HB_OT_ట్యాగ్_avar;

  తార్కిక has_దత్తాంశం () స్థిరరాశి { తిరిగి సంస్కరణ.గమ్యం_int (); }

  స్థిరరాశి SegmentMaps* get_segment_maps () స్థిరరాశి
  { తిరిగి &మొదటి_Axis_Segment_Maps; }

  గుర్తులేని get_axis_లెక్క () స్థిరరాశి
  { తిరిగి axis_లెక్క_2; }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    ఒకవేళ (!(సంస్కరణ.sanitize (c) &&
	  hb_barrier () &&
	  (సంస్కరణ.major == 1
#నిర్వచితం_కాకపోతే HB_NO_AVAR2
	   || సంస్కరణ.major == 2
#అయితే_ముగింపు
	   ) &&
	  c->check_struct (ఇది)))
      return_trace (అబద్ధం);

    స్థిరరాశి SegmentMaps *map = &మొదటి_Axis_Segment_Maps;
    గుర్తులేని పూర్ణాంకం లెక్క = axis_లెక్క_2;
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++)
    {
      ఒకవేళ (unlikely (!map->sanitize (c)))
	return_trace (అబద్ధం);
      map = &StructAfter<SegmentMaps> (*map);
    }

#నిర్వచితం_కాకపోతే HB_NO_AVAR2
    ఒకవేళ (సంస్కరణ.major < 2)
      return_trace (నిజం);
    hb_barrier ();

    స్థిరరాశి స్వయంచాలక &v2 = * (స్థిరరాశి avar_V2_తోక *) map;
    ఒకవేళ (unlikely (!v2.sanitize (c, ఇది)))
      return_trace (అబద్ధం);
#అయితే_ముగింపు

    return_trace (నిజం);
  }

  శూన్య map_coords_16_16 (పూర్ణాంకం *coords, గుర్తులేని పూర్ణాంకం coords_నిడివి) స్థిరరాశి
  {
    గుర్తులేని పూర్ణాంకం లెక్క = hb_min (coords_నిడివి, axis_లెక్క_2);

    స్థిరరాశి SegmentMaps *map = &మొదటి_Axis_Segment_Maps;
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++)
    {
      coords[i] = roundf (map->map_float (coords[i] / 65536.f) * 65536.f);
      map = &StructAfter<SegmentMaps> (*map);
    }

#నిర్వచితం_కాకపోతే HB_NO_AVAR2
    ఒకవేళ (సంస్కరణ.major < 2)
      తిరిగి;
    hb_barrier ();

    కొరకు (; లెక్క < axis_లెక్క_2; లెక్క++)
      map = &StructAfter<SegmentMaps> (*map);

    స్థిరరాశి స్వయంచాలక &v2 = * (స్థిరరాశి avar_V2_తోక *) map;

    స్థిరరాశి స్వయంచాలక &varidx_map = ఇది+v2.varIdxMap;
    స్థిరరాశి స్వయంచాలక &var_store = ఇది+v2.varStore;
    స్వయంచాలక *var_store_cache = var_store.create_cache ();

    hb_సదిశ_t<పూర్ణాంకం> coords_2_14;
    coords_2_14.resize (coords_నిడివి);
    కొరకు (గుర్తులేని i = 0; i < coords_నిడివి; i++)
      coords_2_14[i] = roundf (coords[i] / 4.f); // 16.16 -> 2.14

    hb_సదిశ_t<పూర్ణాంకం> బహిర్గతం;
    బహిర్గతం.alloc (coords_నిడివి);
    కొరకు (గుర్తులేని i = 0; i < coords_నిడివి; i++)
    {
      పూర్ణాంకం v = coords[i];
      uint32_t varidx = varidx_map.map (i);
      దశాంశం delta = var_store.get_delta (varidx, coords_2_14.arrayZ, coords_2_14.నిడివి, var_store_cache);
      /* Apply the delta unclamped and clamp only the result to [-1, +1],
       * matching fontTools. Since inputs and results are in [-1, +1],
       * deltas beyond ±2 are equivalent to ±2; clamp to that range only
       * to keep the float->int conversion safe. */
      దశాంశం d = hb_clamp (delta * 4, -(దశాంశం) (1<<17), +(దశాంశం) (1<<17)); // 2.14 -> 16.16
      v += (పూర్ణాంకం) roundf (d);
      v = hb_clamp (v, -(1<<16), +(1<<16));
      బహిర్గతం.push (v);
    }
    కొరకు (గుర్తులేని i = 0; i < coords_నిడివి; i++)
      coords[i] = బహిర్గతం[i];

    OT::అంశం_Variation_Store::destroy_cache (var_store_cache);
#అయితే_ముగింపు
  }

  /* An avar version 2 font is never downgraded to v1: even with a NULL
   * varStore offset (legal; maps like plain v1) the avar2 subsetting path
   * applies, with "no variation" semantics for rows that don't resolve —
   * offset compensation then creates delta rows on demand. */
  తార్కిక has_v2_దత్తాంశం () స్థిరరాశి { తిరిగి సంస్కరణ.major > 1; }

  /* Resolve the avar2 VarStore and VarIdxMap, for the subset planner.
   * Either pointer may be to the Null object (nullable offsets). */
  తార్కిక get_v2_store_and_map (స్థిరరాశి అంశం_Variation_Store **store,
                             స్థిరరాశి Delta_సమితి_సూచీ_Map **varidx_map) స్థిరరాశి
  {
#నిర్వచితం_కాకపోతే HB_NO_AVAR2
    ఒకవేళ (సంస్కరణ.major < 2) తిరిగి అబద్ధం;
    స్థిరరాశి SegmentMaps *map = &మొదటి_Axis_Segment_Maps;
    కొరకు (గుర్తులేని i = 0; i < axis_లెక్క_2; i++)
      map = &StructAfter<SegmentMaps> (*map);
    స్థిరరాశి స్వయంచాలక &v2 = * (స్థిరరాశి avar_V2_తోక *) map;
    *store = &(ఇది+v2.varStore);
    *varidx_map = &(ఇది+v2.varIdxMap);
    తిరిగి నిజం;
#లేకపోతే
    తిరిగి అబద్ధం;
#అయితే_ముగింపు
  }

  // axis normalization is done in 2.14 here
  // TODO: deprecate this API once fonttools is updated to use 16.16 normalization
  తార్కిక map_coords_2_14 (దశాంశం *coords, గుర్తులేని పూర్ణాంకం coords_నిడివి,
			తార్కిక v1_only = అబద్ధం) స్థిరరాశి
  {
    hb_సదిశ_t<పూర్ణాంకం> coords_2_14;
    ఒకవేళ (!v1_only && !coords_2_14.resize (coords_నిడివి)) తిరిగి అబద్ధం;
    గుర్తులేని పూర్ణాంకం లెక్క = hb_min (coords_నిడివి, axis_లెక్క_2);

    స్థిరరాశి SegmentMaps *map = &మొదటి_Axis_Segment_Maps;
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++)
    {
      పూర్ణాంకం v = roundf (map->map_float (coords[i]) * 16384.f);
      ఒకవేళ (!v1_only)
	coords_2_14[i] = v;
      coords[i] = v / 16384.f;
      map = &StructAfter<SegmentMaps> (*map);
    }

    ఒకవేళ (v1_only)
      తిరిగి నిజం;

#నిర్వచితం_కాకపోతే HB_NO_AVAR2
    ఒకవేళ (సంస్కరణ.major < 2)
      తిరిగి నిజం;
    hb_barrier ();

    కొరకు (; లెక్క < axis_లెక్క_2; లెక్క++)
      map = &StructAfter<SegmentMaps> (*map);

    స్థిరరాశి స్వయంచాలక &v2 = * (స్థిరరాశి avar_V2_తోక *) map;

    స్థిరరాశి స్వయంచాలక &varidx_map = ఇది+v2.varIdxMap;
    స్థిరరాశి స్వయంచాలక &var_store = ఇది+v2.varStore;
    స్వయంచాలక *var_store_cache = var_store.create_cache ();

    కొరకు (గుర్తులేని i = 0; i < coords_నిడివి; i++)
    {
      పూర్ణాంకం v = coords_2_14[i];
      uint32_t varidx = varidx_map.map (i);
      దశాంశం delta = var_store.get_delta (varidx, coords_2_14.arrayZ, coords_2_14.నిడివి, var_store_cache);
      /* As above: apply the delta unclamped (±2 covers every useful case)
       * and clamp the result to [-1, +1], matching fontTools. */
      v += (పూర్ణాంకం) hb_clamp (roundf (delta), -(దశాంశం) (1<<15), +(దశాంశం) (1<<15));
      v = hb_clamp (v, -(1<<14), +(1<<14));
      coords[i] = v / 16384.f;
    }

    OT::అంశం_Variation_Store::destroy_cache (var_store_cache);
    తిరిగి నిజం;
#లేకపోతే
    తిరిగి సంస్కరణ.major < 2;
#అయితే_ముగింపు
  }

  తార్కిక subset (hb_subset_context_t *c) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    గుర్తులేని retained_axis_లెక్క = c->plan->axes_సూచీ_map.get_population ();
    ఒకవేళ (!retained_axis_లెక్క) //all axes are pinned/dropped
      return_trace (అబద్ధం);

    avar *బహిర్గతం = c->serializer->allocate_min<avar> ();
    ఒకవేళ (unlikely (!బహిర్గతం)) return_trace (అబద్ధం);

    బహిర్గతం->సంస్కరణ.major = c->plan->has_avar2 ? 2 : 1;
    బహిర్గతం->సంస్కరణ.minor = 0;
    ఒకవేళ (!c->serializer->check_కేటాయించు (బహిర్గతం->axis_లెక్క_2, retained_axis_లెక్క, HB_SERIALIZE_ERROR_INT_అధికప్రవాహం))
      return_trace (అబద్ధం);

    /* For avar2, keep the instantiated v1 mappings around; offset
     * compensation needs them to locate the new mappings' kinks. */
    hb_సదిశ_t<hb_సదిశ_t<Axis_విలువ_Map>> new_mappings;
    ఒకవేళ (c->plan->has_avar2 && !new_mappings.resize (axis_లెక్క_2))
      return_trace (అబద్ధం);

    స్థిరరాశి hb_map_t& axes_సూచీ_map = c->plan->axes_సూచీ_map;
    స్థిరరాశి SegmentMaps *map = &మొదటి_Axis_Segment_Maps;
    గుర్తులేని లెక్క = axis_లెక్క_2;
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++)
    {
      ఒకవేళ (axes_సూచీ_map.has (i))
      {
        hb_ట్యాగ్_t *axis_ట్యాగ్_2;
        ఒకవేళ (!c->plan->axes_old_సూచీ_ట్యాగ్_map.has (i, &axis_ట్యాగ్_2))
          return_trace (అబద్ధం);

        Triple *axis_location;
        ఒకవేళ (c->plan->has_avar2 &&
            c->plan->user_axes_location.has (*axis_ట్యాగ్_2, &axis_location) &&
            axis_location->is_బిందువు ())
        {
          /* Pinned axis in avar2 mode: serialize identity segment map
           * {-1->-1, 0->0, 1->1}. The axis is kept in fvar as hidden,
           * so avar needs a segment map entry for it. */
          స్వయంచాలక *తటస్థం_map = c->serializer->ప్రారంభం_embed<SegmentMaps> ();
          ఒకవేళ (unlikely (!c->serializer->extend_min (తటస్థం_map)))
            return_trace (అబద్ధం);
          Axis_విలువ_Map m;
          m.సమితి_mapping (-1.f, -1.f);
          ఒకవేళ (!m.serialize (c->serializer)) return_trace (అబద్ధం);
          m.సమితి_mapping (0.f, 0.f);
          ఒకవేళ (!m.serialize (c->serializer)) return_trace (అబద్ధం);
          m.సమితి_mapping (1.f, 1.f);
          ఒకవేళ (!m.serialize (c->serializer)) return_trace (అబద్ధం);
          ఒకవేళ (!c->serializer->check_కేటాయించు (తటస్థం_map->len, 3u,
                                            HB_SERIALIZE_ERROR_INT_అధికప్రవాహం))
            return_trace (అబద్ధం);
        }
        లేకపోతే
        {
          /* Restricted or free axis: use standard SegmentMaps::subset() */
          ఒకవేళ (!map->subset (c, *axis_ట్యాగ్_2,
                            c->plan->has_avar2 ? &new_mappings[i] : శూన్యసూచిక))
            return_trace (అబద్ధం);
        }
      }
      map = &StructAfter<SegmentMaps> (*map);
    }

    ఒకవేళ (c->plan->has_avar2)
      return_trace (_subset_avar2 (c, new_mappings));

    return_trace (నిజం);
  }

  అంతరంగ:
  నిర్మాణం avar2_సూచీ_map_plan_t
  {
    తార్కిక init (స్థిరరాశి hb_సదిశ_t<uint32_t> &varidx_mapping,
	       స్థిరరాశి hb_map_t &axes_సూచీ_map,
	       గుర్తులేని axis_లెక్క)
    {
      ఒకవేళ (!output_map.alloc (axes_సూచీ_map.get_population ()))
	తిరిగి అబద్ధం;

      తార్కిక has_no_variation = అబద్ధం;
      గుర్తులేని max_outer = 0, max_inner = 0;
      కొరకు (గుర్తులేని i = 0; i < axis_లెక్క; i++)
      {
	ఒకవేళ (!axes_సూచీ_map.has (i)) కొనసాగు;
	uint32_t varidx = varidx_mapping[i];
	output_map.push (varidx);
	ఒకవేళ (varidx == HB_OT_LAYOUT_NO_VARIATIONS_సూచీ)
	{
	  has_no_variation = నిజం;
	  కొనసాగు;
	}
	max_outer = hb_max (max_outer, varidx >> 16);
	max_inner = hb_max (max_inner, varidx & 0xFFFF);
      }
      ఒకవేళ (output_map.లోపల_error () ||
	  output_map.నిడివి != axes_సూచీ_map.get_population ())
	తిరిగి అబద్ధం;

      ఒకవేళ (has_no_variation)
      {
	వెడల్పు = 4;
	inner_బిట్టు_లెక్క = 16;
      }
      లేకపోతే
      {
	inner_బిట్టు_లెక్క = hb_max (1u, hb_బిట్టు_storage (max_inner));
	గుర్తులేని outer_బిట్టు_లెక్క = hb_max (1u, hb_బిట్టు_storage (max_outer));
	వెడల్పు = hb_clamp ((inner_బిట్టు_లెక్క + outer_బిట్టు_లెక్క + 7) / 8, 1u, 4u);
	ఒకవేళ (inner_బిట్టు_లెక్క + outer_బిట్టు_లెక్క > వెడల్పు * 8)
	  inner_బిట్టు_లెక్క = వెడల్పు * 8 - outer_బిట్టు_లెక్క;
      }
      తిరిగి నిజం;
    }

    గుర్తులేని get_inner_బిట్టు_లెక్క () స్థిరరాశి { తిరిగి inner_బిట్టు_లెక్క; }
    గుర్తులేని get_వెడల్పు () స్థిరరాశి { తిరిగి వెడల్పు; }
    hb_array_t<స్థిరరాశి uint32_t> get_output_map () స్థిరరాశి { తిరిగి output_map.as_array (); }

    గుర్తులేని inner_బిట్టు_లెక్క = 1;
    గుర్తులేని వెడల్పు = 1;
    hb_సదిశ_t<uint32_t> output_map;
  };

  తార్కిక _subset_avar2 (hb_subset_context_t *c,
                      స్థిరరాశి hb_సదిశ_t<hb_సదిశ_t<Axis_విలువ_Map>> &new_mappings) స్థిరరాశి
  {
#ఒకవేళ defined (HB_NO_VAR) || defined (HB_NO_AVAR2)
    /* Not reachable: the plan never sets has_avar2 in these configurations. */
    తిరిగి అబద్ధం;
#లేకపోతే

    /* 1. Locate original avar2 data, keeping per-axis old segment maps */
    hb_సదిశ_t<స్థిరరాశి SegmentMaps *> old_seg_maps;
    ఒకవేళ (!old_seg_maps.alloc (axis_లెక్క_2)) తిరిగి అబద్ధం;
    స్థిరరాశి SegmentMaps *map = &మొదటి_Axis_Segment_Maps;
    కొరకు (గుర్తులేని i = 0; i < axis_లెక్క_2; i++)
    {
      old_seg_maps.push (map);
      map = &StructAfter<SegmentMaps> (*map);
    }

    స్థిరరాశి స్వయంచాలక &v2 = * (స్థిరరాశి avar_V2_తోక *) map;
    స్థిరరాశి స్వయంచాలక &varidx_map = ఇది+v2.varIdxMap;
    స్థిరరాశి స్వయంచాలక &var_store = ఇది+v2.varStore;

    స్వయంచాలక fvar_axes = c->plan->ఆధారం->పట్టిక.fvar->get_axes ();

    /* 2. Compute default deltas by evaluating VarStore at old defaults */
    hb_సదిశ_t<పూర్ణాంకం> default_coords;
    ఒకవేళ (!default_coords.resize (axis_లెక్క_2)) తిరిగి అబద్ధం;
    కొరకు (గుర్తులేని i = 0; i < axis_లెక్క_2; i++)
    {
      hb_ట్యాగ్_t *axis_ట్యాగ్_2;
      ఒకవేళ (c->plan->axes_old_సూచీ_ట్యాగ్_map.has (i, &axis_ట్యాగ్_2) &&
          c->plan->old_intermediates.has (*axis_ట్యాగ్_2))
      {
        దశాంశం d_i = (దశాంశం) c->plan->old_intermediates.get (*axis_ట్యాగ్_2).మధ్య;
        default_coords[i] = roundf (d_i * 16384.f);
      }
      లేకపోతే
        default_coords[i] = 0;
    }

    స్వయంచాలక *store_cache = var_store.create_cache ();
    hb_సదిశ_t<దశాంశం> default_deltas;
    ఒకవేళ (!default_deltas.resize (axis_లెక్క_2)) {
      అంశం_Variation_Store::destroy_cache (store_cache);
      తిరిగి అబద్ధం;
    }
    కొరకు (గుర్తులేని i = 0; i < axis_లెక్క_2; i++)
    {
      uint32_t varidx = varidx_map.map (i);
      ఒకవేళ (varidx == HB_OT_LAYOUT_NO_VARIATIONS_సూచీ)
        default_deltas[i] = 0.f;
      లేకపోతే
        default_deltas[i] = var_store.get_delta (varidx, default_coords.arrayZ,
                                                  default_coords.నిడివి, store_cache);
    }
    అంశం_Variation_Store::destroy_cache (store_cache);

    /* 3. Rebase IVS regions */
    అంశం_variations_t అంశం_vars;
    ఒకవేళ (!అంశం_vars.create_from_అంశం_varstore (var_store, c->plan->axes_old_సూచీ_ట్యాగ్_map))
      తిరిగి అబద్ధం;
    ఒకవేళ (!అంశం_vars.instantiate_tuple_vars (c->plan->old_intermediates,
                                           c->plan->axes_triple_distances,
                                           అబద్ధం))
      తిరిగి అబద్ధం;

    /* 4. Self-contained pinned axes (whose final coordinate is constant over
     * the retained box) were detected at plan time
     * (_compute_avar2_reachable_ranges) and removed from axes_index_map.
     * They are skipped below; their constant contribution is baked into the
     * other variation tables by standard instancing at the plan's
     * axes_location/normalized_coords. */

    /* 5. Build per-axis varIdx mapping (may create new VarDatas).
     * Entries (or the implicit identity mapping) that don't resolve to a
     * real store row behave as "no variation" at runtime; normalize them to
     * NO_VARIATIONS_INDEX so the offset loop creates fresh rows instead of
     * writing into nonexistent ones. This also covers a NULL VarStore
     * (never downgraded: rows are created on demand). */
    hb_సదిశ_t<uint32_t> new_varidx_mapping;
    ఒకవేళ (!new_varidx_mapping.resize (axis_లెక్క_2)) తిరిగి అబద్ధం;
    కొరకు (గుర్తులేని i = 0; i < axis_లెక్క_2; i++)
    {
      uint32_t varidx = varidx_map.map (i);
      ఒకవేళ (varidx != HB_OT_LAYOUT_NO_VARIATIONS_సూచీ &&
	  !var_store.has_delta_సమితి (varidx))
	varidx = HB_OT_LAYOUT_NO_VARIATIONS_సూచీ;
      new_varidx_mapping[i] = varidx;
    }

    /* 5.5. Privatize shared varIdx delta rows before adding offset
     * compensation. avar2's VarIdxMap may map several fvar axes to the SAME
     * IVS delta row. Writing one axis's offset-compensation deltas into a
     * shared row would corrupt every other axis that reads that row. So give
     * each offset-receiving axis whose row is shared its own private copy of
     * the row (identical contents, preserving the rebased deltas), then
     * repoint its varIdx. Sharers keep the clean row; the varstore
     * optimization pass re-merges identical rows afterwards. */
    hb_hashmap_t<uint32_t, గుర్తులేని> varidx_ref_లెక్క;
    కొరకు (గుర్తులేని i = 0; i < axis_లెక్క_2; i++)
    {
      ఒకవేళ (!c->plan->axes_సూచీ_map.has (i)) కొనసాగు; /* self-contained: dropped */
      uint32_t varidx = new_varidx_mapping[i];
      ఒకవేళ (varidx == HB_OT_LAYOUT_NO_VARIATIONS_సూచీ) కొనసాగు;
      గుర్తులేని *లెక్క;
      ఒకవేళ (varidx_ref_లెక్క.has (varidx, &లెక్క))
        (*లెక్క)++;
      లేకపోతే ఒకవేళ (!varidx_ref_లెక్క.సమితి (varidx, 1))
        తిరిగి అబద్ధం;
    }
    కొరకు (గుర్తులేని i = 0; i < axis_లెక్క_2; i++)
    {
      hb_ట్యాగ్_t *axis_ట్యాగ్_ptr;
      ఒకవేళ (!c->plan->axes_old_సూచీ_ట్యాగ్_map.has (i, &axis_ట్యాగ్_ptr))
        తిరిగి అబద్ధం;
      /* Only axes that will receive offset compensation (restricted or
       * pinned) can contaminate a shared row. */
      ఒకవేళ (!c->plan->axes_సూచీ_map.has (i) ||
          !c->plan->user_axes_location.has (*axis_ట్యాగ్_ptr))
        కొనసాగు;
      uint32_t varidx = new_varidx_mapping[i];
      ఒకవేళ (varidx == HB_OT_LAYOUT_NO_VARIATIONS_సూచీ)
        కొనసాగు; /* gets a fresh, private VarData in the offset loop below */
      గుర్తులేని *లెక్క;
      ఒకవేళ (!varidx_ref_లెక్క.has (varidx, &లెక్క) || *లెక్క <= 1)
        కొనసాగు; /* sole owner: safe to write offsets in place */
      గుర్తులేని outer = varidx >> 16;
      గుర్తులేని new_inner = అంశం_vars.duplicate_అడ్డువరుస (outer, varidx & 0xFFFF);
      ఒకవేళ (unlikely (new_inner == (గుర్తులేని) -1)) తిరిగి అబద్ధం;
      new_varidx_mapping[i] = (outer << 16) | new_inner;
      (*లెక్క)--;
    }

    /* 6. Add offset compensation tuples.
     * Track processed (outer,inner) pairs to avoid adding duplicate biases
     * when multiple axes share the same varIdx. */
    hb_సమితి_t processed_varidxes;
    కొరకు (గుర్తులేని i = 0; i < axis_లెక్క_2; i++)
    {
      hb_ట్యాగ్_t *axis_ట్యాగ్_ptr;
      ఒకవేళ (!c->plan->axes_old_సూచీ_ట్యాగ్_map.has (i, &axis_ట్యాగ్_ptr))
        తిరిగి అబద్ధం;
      hb_ట్యాగ్_t axis_ట్యాగ్_2 = *axis_ట్యాగ్_ptr;

      /* Self-contained pinned axes are removed from fvar/avar; their
       * contribution is baked into the variation tables instead. */
      ఒకవేళ (!c->plan->axes_సూచీ_map.has (i))
        కొనసాగు;

      Triple *new_user;
      ఒకవేళ (c->plan->user_axes_location.has (axis_ట్యాగ్_2, &new_user))
      {
        /* This axis is being restricted or pinned */
        Triple *old_int;
        ఒకవేళ (!c->plan->old_intermediates.has (axis_ట్యాగ్_2, &old_int))
          తిరిగి అబద్ధం;

        దశాంశం a_i = (దశాంశం) old_int->minimum;
        దశాంశం d_i = (దశాంశం) old_int->మధ్య;
        దశాంశం b_i = (దశాంశం) old_int->గరిష్ఠ;

        పూర్ణాంకం d_int = roundf (d_i * 16384.f);

        తార్కిక is_pinned = new_user->is_బిందువు ();

        uint32_t varidx = new_varidx_mapping[i];
        గుర్తులేని outer, inner, అంశం_లెక్క_2;

        ఒకవేళ (varidx == HB_OT_LAYOUT_NO_VARIATIONS_సూచీ)
        {
          /* No existing avar2 mapping. Create new VarData. */
          outer = అంశం_vars.కూడు_vardata (1);
          inner = 0;
          అంశం_లెక్క_2 = 1;
          new_varidx_mapping[i] = (outer << 16) | inner;
          default_deltas[i] = 0.f; /* no prior default delta */
        }
        లేకపోతే
        {
          outer = varidx >> 16;
          inner = varidx & 0xFFFF;
          అంశం_లెక్క_2 = అంశం_vars.get_అంశం_లెక్క (outer);
        }

        /* Empty-region bias: d_int + round(defaultDelta) */
        పూర్ణాంకం bias = d_int + (పూర్ణాంకం) roundf (default_deltas[i]);
        ఒకవేళ (bias != 0)
        {
          hb_hashmap_t<hb_ట్యాగ్_t, Triple> ఖాళీలు_region;
          అంశం_vars.కూడు_tuple (outer, std::move (ఖాళీలు_region),
                               inner, bias, అంశం_లెక్క_2);
        }

        ఒకవేళ (!is_pinned)
        {
          /* Offset compensation encodes, as avar2 deltas on this axis, the
           * piecewise-linear function offset(z) = inv_renorm(z) - z, where
           * inv_renorm maps a new intermediate coordinate z back to the old
           * intermediate coordinate. It is known at these knots in the new
           * intermediate space:
           *   z = -1  ->  a_i + 1   (new minimum)
           *   z =  0  ->  d_i       (new default)
           *   z = +1  ->  b_i - 1   (new maximum)
           */
          hb_సదిశ_t<avar2_స్థానభ్రంశం_knot_t> knots;
          avar2_కూడు_knot (knots, -1.0, (ద్విగుణ) a_i + 1.0);
          avar2_కూడు_knot (knots, 0.0, (ద్విగుణ) d_i);
          avar2_కూడు_knot (knots, 1.0, (ద్విగుణ) b_i - 1.0);

          స్థిరరాశి hb_సదిశ_t<Axis_విలువ_Map> &new_mapping = new_mappings[i];

          దశాంశం min_f = 0.f, def_f = 0.f, max_f = 0.f;
          ఒకవేళ (likely (i < fvar_axes.నిడివి))
            fvar_axes[i].get_coordinates (min_f, def_f, max_f);
          ద్విగుణ old_min = (ద్విగుణ) min_f;
          ద్విగుణ old_def = (ద్విగుణ) def_f;
          ద్విగుణ old_max = (ద్విగుణ) max_f;

          /* If the axis default MOVED, inv_renorm also kinks where the OLD
           * default lands in the new space (the old intermediate coordinate
           * crosses 0 there), at
           *   z = z_old  ->  -z_old
           * Omitting that knot (as a plain two-tent encoding would) makes
           * interior coordinates wrong. */
          ద్విగుణ z_old = avar2_normalize_విలువ ((ద్విగుణ) old_def,
                                                 (ద్విగుణ) new_user->minimum,
                                                 (ద్విగుణ) new_user->మధ్య,
                                                 (ద్విగుణ) new_user->గరిష్ఠ);
          z_old = _avar2_map_new_mapping (new_mapping, z_old);
          z_old = (ద్విగుణ) roundf ((దశాంశం) (z_old * 16384.0)) / 16384.0;
          ఒకవేళ (-1.0 < z_old && z_old < 1.0)
            avar2_కూడు_knot (knots, z_old, -z_old);

          /* Interior avar v1 breakpoints inside the retained range each put
           * a kink in offset(z). Sampling only {-1, 0, +1, z_old} would
           * linearly interpolate across those kinks. The instantiated
           * mapping keeps exactly the in-range old breakpoints, and the new
           * mapping kinks at each one's output coordinate; add that z with
           * its old intermediate value so offset(z) is reproduced at every
           * kink. */
          hb_సదిశ_t<avar2_స్థానభ్రంశం_knot_t> with_breakpoints (knots);
          కొరకు (స్థిరరాశి స్వయంచాలక &m : new_mapping)
          {
            ద్విగుణ from = (ద్విగుణ) m.coords[0].గమ్యం_float ();
            ఒకవేళ (from == -1.0 || from == 0.0 || from == 1.0)
              కొనసాగు; /* anchors already seeded */
            ద్విగుణ z = (ద్విగుణ) m.coords[1].గమ్యం_float ();
            ఒకవేళ (!(-1.0 < z && z < 1.0))
              కొనసాగు;
            ద్విగుణ user = avar2_denormalize_విలువ (from,
                                                    (ద్విగుణ) new_user->minimum,
                                                    (ద్విగుణ) new_user->మధ్య,
                                                    (ద్విగుణ) new_user->గరిష్ఠ);
            ద్విగుణ n_old = avar2_normalize_విలువ (user, old_min, old_def, old_max);
            ద్విగుణ x_old = (ద్విగుణ) old_seg_maps[i]->map_float ((దశాంశం) n_old);
            x_old = (ద్విగుణ) roundf ((దశాంశం) (x_old * 16384.0)) / 16384.0;
            avar2_కూడు_knot (with_breakpoints, z, x_old - z);
          }

          ఒకవేళ (unlikely (knots.లోపల_error () || with_breakpoints.లోపల_error ()))
            తిరిగి అబద్ధం;

          /* Extra tents cost F2Dot14 rounding, so for a steep segment they
           * can add more quantization noise than the structural error they
           * remove. Keep the interior breakpoints only when they do not
           * increase the estimated residual; this makes the collection a
           * strict (never-worse) improvement over the {-1, 0, +1, z_old}
           * anchors. Warn when even the better choice is not bit-exact (a
           * steep retained segment that cannot be reproduced in F2Dot14). */
          గుర్తులేని err = avar2_estimate_స్థానభ్రంశం_error (*old_seg_maps[i], new_mapping,
                                                       old_min, old_def, old_max,
                                                       (ద్విగుణ) new_user->minimum,
                                                       (ద్విగుణ) new_user->మధ్య,
                                                       (ద్విగుణ) new_user->గరిష్ఠ,
                                                       knots);
          ఒకవేళ (with_breakpoints.నిడివి != knots.నిడివి)
          {
            గుర్తులేని err_with = avar2_estimate_స్థానభ్రంశం_error (*old_seg_maps[i], new_mapping,
                                                              old_min, old_def, old_max,
                                                              (ద్విగుణ) new_user->minimum,
                                                              (ద్విగుణ) new_user->మధ్య,
                                                              (ద్విగుణ) new_user->గరిష్ఠ,
                                                              with_breakpoints);
            ఒకవేళ (err_with <= err)
            {
              knots = std::move (with_breakpoints);
              err = err_with;
            }
          }
          ఒకవేళ (err > 8)
            DEBUG_సందేశం (SUBSET, శూన్యసూచిక,
                       "avar2 offset compensation is approximate for axis %c%c%c%c: "
                       "max residual %u F2Dot14 units",
                       HB_UNTAG (axis_ట్యాగ్_2), err);

          /* Synthesize tents. Adjacent tents evaluate to zero at each
           * other's peaks, so each knot's delta is offset(z) - offset(0);
           * the base value offset(0) = d_i is carried by the empty-region
           * bias above. This reduces to the classic pair of tents
           * (-1,-1,0) / (0,+1,+1) when the default is unchanged and there
           * are no interior knots. */
          కొరకు (గుర్తులేని k = 0; k < knots.నిడివి; k++)
          {
            ద్విగుణ z = knots.arrayZ[k].z;
            ఒకవేళ (z == 0.0) కొనసాగు;
            పూర్ణాంకం delta = (పూర్ణాంకం) roundf ((దశాంశం) ((knots.arrayZ[k].స్థానభ్రంశం - (ద్విగుణ) d_i) * 16384.0));
            ఒకవేళ (!delta) కొనసాగు;
            ద్విగుణ క్రింది, పై;
            ఒకవేళ (z > 0.0)
            {
              క్రింది = knots.arrayZ[k - 1].z;
              పై = k + 1 < knots.నిడివి ? knots.arrayZ[k + 1].z : z;
            }
            లేకపోతే
            {
              పై = knots.arrayZ[k + 1].z;
              క్రింది = k > 0 ? knots.arrayZ[k - 1].z : z;
            }
            hb_hashmap_t<hb_ట్యాగ్_t, Triple> region;
            ఒకవేళ (unlikely (!region.సమితి (axis_ట్యాగ్_2, Triple (క్రింది, z, పై))))
              తిరిగి అబద్ధం;
            అంశం_vars.కూడు_tuple (outer, std::move (region),
                                 inner, delta, అంశం_లెక్క_2);
          }
        }
      }
      లేకపోతే
      {
        /* Free or private axis — not being restricted.
         * If it has a non-zero default delta, add it back as a bias.
         * Skip if this (outer,inner) was already processed (shared varIdx). */
        uint32_t varidx = new_varidx_mapping[i];
        ఒకవేళ (varidx == HB_OT_LAYOUT_NO_VARIATIONS_సూచీ)
          కొనసాగు;

        ఒకవేళ (processed_varidxes.has (varidx))
          కొనసాగు;
        processed_varidxes.కూడు (varidx);

        గుర్తులేని outer = varidx >> 16;
        గుర్తులేని inner = varidx & 0xFFFF;
        పూర్ణాంకం dd = (పూర్ణాంకం) roundf (default_deltas[i]);
        ఒకవేళ (dd != 0)
        {
          గుర్తులేని అంశం_లెక్క_2 = అంశం_vars.get_అంశం_లెక్క (outer);
          hb_hashmap_t<hb_ట్యాగ్_t, Triple> ఖాళీలు_region;
          అంశం_vars.కూడు_tuple (outer, std::move (ఖాళీలు_region),
                               inner, dd, అంశం_లెక్క_2);
        }
      }
    }

    /* 7. Finalize: build region list + convert to varstore */
    ఒకవేళ (!అంశం_vars.build_region_list ()) తిరిగి అబద్ధం;
    ఒకవేళ (!అంశం_vars.as_అంశం_varstore (నిజం /* optimize */,
                                     అబద్ధం /* use_no_variation_idx */))
      తిరిగి అబద్ధం;

    /* 8. Apply varidx_map optimization remapping */
    స్థిరరాశి స్వయంచాలక &opt_varidx_map = అంశం_vars.get_varidx_map ();
    కొరకు (గుర్తులేని i = 0; i < axis_లెక్క_2; i++)
    {
      uint32_t varidx = new_varidx_mapping[i];
      ఒకవేళ (varidx == HB_OT_LAYOUT_NO_VARIATIONS_సూచీ)
        కొనసాగు;
      uint32_t *new_idx;
      ఒకవేళ (opt_varidx_map.has (varidx, &new_idx))
        new_varidx_mapping[i] = *new_idx;
    }

    /* 9. Serialize avarV2Tail. Entries cover the retained axes only;
     * self-contained pinned axes are removed from fvar. */
    avar2_సూచీ_map_plan_t సూచీ_map_plan;
    ఒకవేళ (!సూచీ_map_plan.init (new_varidx_mapping,
			      c->plan->axes_సూచీ_map,
			      axis_లెక్క_2))
      తిరిగి అబద్ధం;

    స్వయంచాలక *తోక = c->serializer->allocate_కొలత<avar_V2_తోక> (avar_V2_తోక::static_కొలత);
    ఒకవేళ (unlikely (!తోక)) తిరిగి అబద్ధం;
    ఒకవేళ (!తోక->varIdxMap.serialize_serialize (c->serializer, సూచీ_map_plan))
      తిరిగి అబద్ధం;
    ఒకవేళ (!తోక->varStore.serialize_serialize (c->serializer,
					     అంశం_vars.has_long_మాట (),
					     c->plan->axis_ట్యాగులు,
					     అంశం_vars.get_region_list (),
					     అంశం_vars.get_vardata_encodings ()))
      తిరిగి అబద్ధం;

    తిరిగి నిజం;
#అయితే_ముగింపు
  }

  బహిరంగ:

  రక్షిత:
  Fixed_సంస్కరణ<>సంస్కరణ;	/* Version of the avar table
				 * initially set to 0x00010000u */
  HBUINT16	reserved;	/* This field is permanently reserved. Set to 0. */
  HBUINT16	axis_లెక్క_2;	/* The number of variation axes in the font. This
				 * must be the same number as axisCount in the
				 * 'fvar' table. */
  SegmentMaps	మొదటి_Axis_Segment_Maps;

  బహిరంగ:
  DEFINE_కొలత_MIN (8);
};

} /* namespace OT */


#అయితే_ముగింపు /* HB_OT_VAR_AVAR_TABLE_HH */
