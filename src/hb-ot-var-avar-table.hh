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

#اگر_غیر_تعریف_شدہ HB_OT_VAR_AVAR_جدول_HH
#تعریف_کرو HB_OT_VAR_AVAR_جدول_HH

#شامل "hb-open-type.hh"
#شامل "hb-ot-var-common.hh"
#شامل "hb-ot-var-fvar-table.hh"


/*
 * avar -- Axis Variations
 * https://docs.microsoft.com/en-us/typography/opentype/spec/avar
 */

#تعریف_کرو HB_OT_لیبل_avar HB_لیبل('a','v','a','r')


نامخلا OT {


/* "Spec": https://github.com/be-fonts/boring-expansion-spec/issues/14 */
ساخت avar_V2_دم
{
  دوست ساخت avar;

  منطقی sanitize (hb_sanitize_context_t *c,
		 ثابت خالی *بنیاد) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (varIdxMap.sanitize (c, بنیاد) &&
		  varStore.sanitize (c, بنیاد));
  }

  محفوظ:
  Offset32_مقصد<Delta_سیٹ_اشاریہ_Map>	varIdxMap;	/* Offset from the beginning of 'avar' table. */
  Offset32_مقصد<شے_Variation_Store>	varStore;	/* Offset from the beginning of 'avar' table. */

  عوامی:
  DEFINE_سائز_STATIC (8);
};


ساخت Axis_قدر_Map
{
  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ));
  }

  خالی سیٹ_mapping (اعشاری from_coord, اعشاری مقصد_coord)
  {
    coords[0].سیٹ_float (from_coord);
    coords[1].سیٹ_float (مقصد_coord);
  }

  منطقی is_outside_axis_range (ثابت Triple& axis_range) ثابت
  {
    دوہرا from_coord = (دوہرا) coords[0].مقصد_float ();
    واپس !axis_range.شامل_ہے (from_coord);
  }

  منطقی must_include () ثابت
  {
    اعشاری from_coord = coords[0].مقصد_float ();
    اعشاری مقصد_coord = coords[1].مقصد_float ();
    واپس (from_coord == -1.f && مقصد_coord == -1.f) ||
           (from_coord == 0.f && مقصد_coord == 0.f) ||
           (from_coord == 1.f && مقصد_coord == 1.f);
  }

  خالی instantiate (ثابت Triple& axis_range,
                    ثابت Triple& unmapped_range,
                    ثابت TripleDistances& triple_distances)
  {
    اعشاری from_coord = coords[0].مقصد_float ();
    اعشاری مقصد_coord = coords[1].مقصد_float ();

    from_coord = renormalize_قدر ((دوہرا) from_coord, unmapped_range, triple_distances);
    مقصد_coord = renormalize_قدر ((دوہرا) مقصد_coord, axis_range, triple_distances);

    /* Malformed input can renormalize outside [-1,1], which would
     * overflow F2DOT14. */
    from_coord = hb_clamp (from_coord, -1.f, 1.f);
    مقصد_coord = hb_clamp (مقصد_coord, -1.f, 1.f);

    coords[0].سیٹ_float (from_coord);
    coords[1].سیٹ_float (مقصد_coord);
  }

  HB_اندرونی ساکن صحیح cmp (ثابت خالی *pa, ثابت خالی *pb)
  {
    ثابت Axis_قدر_Map *a = (ثابت Axis_قدر_Map *) pa;
    ثابت Axis_قدر_Map *b = (ثابت Axis_قدر_Map *) pb;

    صحیح a_from = a->coords[0].مقصد_int ();
    صحیح b_from = b->coords[0].مقصد_int ();
    اگر (a_from != b_from)
      واپس a_from - b_from;

    /* this should never be reached. according to the spec, all of the axis
     * value map records for a given axis must have different fromCoord values
     * */
    صحیح a_مقصد = a->coords[1].مقصد_int ();
    صحیح b_مقصد = b->coords[1].مقصد_int ();
    واپس a_مقصد - b_مقصد;
  }

  منطقی serialize (hb_serialize_context_t *c) ثابت
  {
    TRACE_SERIALIZE (یہ);
    return_trace (c->embed (یہ));
  }

  عوامی:
  F2DOT14	coords[2];
//   F2DOT14	fromCoord;	/* A normalized coordinate value obtained using
//				 * default normalization. */
//   F2DOT14	toCoord;	/* The modified, normalized coordinate value. */

  عوامی:
  DEFINE_سائز_STATIC (4);
};

ساخت SegmentMaps : Array16_کا<Axis_قدر_Map>
{
  اعشاری map_float (اعشاری value, غیرعلامتی صحیح from_انحراف = 0, غیرعلامتی صحیح مقصد_انحراف = 1) ثابت
  {
#تعریف_کرو fromCoord coords[from_انحراف].مقصد_float ()
#تعریف_کرو مقصد_Coord coords[مقصد_انحراف].مقصد_float ()

    ثابت خودکار *map = arrayZ;

    /* The following special-cases are not part of OpenType, which requires
     * that at least -1, 0, and +1 must be mapped. But we include these as
     * part of a better error recovery scheme. */
    اگر (len < 2)
    {
      اگر (!len)
	واپس value;
      ورنہ /* len == 1*/
	واپس value - map[0].fromCoord + map[0].مقصد_Coord;
    }

    // At least two mappings now.

    /* CoreText is wild...
     * PingFangUI avar needs all this special-casing...
     * So we implement an extended version of the spec here,
     * which is more robust and more likely to be compatible with
     * the wild. */

    غیرعلامتی آغاز = 0;
    غیرعلامتی انتہا = len;
    اگر (map[آغاز].fromCoord == -1 && map[آغاز].مقصد_Coord == -1 && map[آغاز+1].fromCoord == -1)
      آغاز++;
    اگر (map[انتہا-1].fromCoord == +1 && map[انتہا-1].مقصد_Coord == +1 && map[انتہا-2].fromCoord == +1)
      انتہا--;

    /* Look for exact match first, and do lots of special-casing. */
    غیرعلامتی i;
    برائے (i = آغاز; i < انتہا; i++)
      اگر (value == map[i].fromCoord)
	توڑو;
    اگر (i < انتہا)
    {
      // There's at least one exact match. See if there are more.
      غیرعلامتی j = i;
      برائے (; j + 1 < انتہا; j++)
	اگر (value != map[j + 1].fromCoord)
	  توڑو;

      // [i,j] inclusive are all exact matches:

      // If there's only one, return it. This is the only spec-compliant case.
      اگر (i == j)
	واپس map[i].مقصد_Coord;
      // If there's exactly three, return the middle one.
      اگر (i + 2 == j)
	واپس map[i + 1].مقصد_Coord;

      // Ignore the middle ones. Return the one mapping closer to 0.
      اگر (value < 0) واپس map[j].مقصد_Coord;
      اگر (value > 0) واپس map[i].مقصد_Coord;

      // Mapping 0? CoreText seems confused. It seems to prefer 0 here...
      // So we'll just return the smallest one. lol
      واپس fabsf (map[i].مقصد_Coord) < fabsf (map[j].مقصد_Coord) ? map[i].مقصد_Coord : map[j].مقصد_Coord;

      // Mapping 0? Return one not mapping to 0.
      اگر (map[i].مقصد_Coord == 0)
	واپس map[j].مقصد_Coord;
      ورنہ
	واپس map[i].مقصد_Coord;
    }

    /* There's at least two and we're not an exact match. Prepare to lerp. */

    // Find the segment we're in.
    برائے (i = آغاز; i < انتہا; i++)
      اگر (value < map[i].fromCoord)
	توڑو;

    اگر (i == آغاز)
    {
      // Value before all segments; Shift.
      واپس value - map[آغاز].fromCoord + map[آغاز].مقصد_Coord;
    }
    اگر (i == انتہا)
    {
      // Value after all segments; Shift.
      واپس value - map[انتہا - 1].fromCoord + map[انتہا - 1].مقصد_Coord;
    }

    // Actually interpolate.
    خودکار &before = map[i-1];
    خودکار &after = map[i];
    اعشاری denom = after.fromCoord - before.fromCoord; // Can't be zero by now.
    واپس before.مقصد_Coord + ((after.مقصد_Coord - before.مقصد_Coord) * (value - before.fromCoord)) / denom;

#تعریف_منسوخ مقصد_Coord
#تعریف_منسوخ fromCoord
  }

  اعشاری unmap_float (اعشاری value) ثابت { واپس map_float (value, 1, 0); }


  // TODO Kill this.
  Triple unmap_axis_range (ثابت Triple& axis_range) ثابت
  {
    اعشاری unmapped_min = unmap_float (axis_range.minimum);
    اعشاری unmapped_درمیان = unmap_float (axis_range.درمیان);
    اعشاری unmapped_max = unmap_float (axis_range.زیادہ_سے_زیادہ);

    واپس Triple{unmapped_min, unmapped_درمیان, unmapped_max};
  }

  منطقی subset (hb_subset_context_t *c, hb_لیبل_t axis_لیبل_2,
               hb_سمتیہ_t<Axis_قدر_Map> *برآمد_mappings = خالی_اشاریہ) ثابت
  {
    TRACE_SUBSET (یہ);

    /* This function cannot work on avar2 table (and currently doesn't).
     * We should instead keep the design coords in the shape plan and use
     * those. unmap_axis_range needs to be killed. */

    /* avar mapped normalized axis range. Under avar2, axes_location holds
     * only the self-contained pins for the other tables; avar itself uses
     * the intermediate-space ranges of all restricted axes. */
    ثابت خودکار &axes_location = c->plan->has_avar2
				? c->plan->old_intermediates
				: c->plan->axes_location;
    Triple *axis_range;
    اگر (!axes_location.has (axis_لیبل_2, &axis_range))
      واپس c->serializer->embed (*یہ);

    TripleDistances *axis_triple_distances;
    اگر (!c->plan->axes_triple_distances.has (axis_لیبل_2, &axis_triple_distances))
      return_trace (جھوٹ);

    خودکار *برآمد = c->serializer->آغاز_embed (یہ);
    اگر (unlikely (!c->serializer->extend_min (برآمد))) return_trace (جھوٹ);

    Triple unmapped_range = unmap_axis_range (*axis_range);

    /* create a vector of retained mappings and sort */
    hb_سمتیہ_t<Axis_قدر_Map> قدر_mappings;
    برائے (ثابت خودکار& _ : as_array ())
    {
      اگر (_.is_outside_axis_range (unmapped_range))
        جاری;
      Axis_قدر_Map mapping;
      mapping = _;
      mapping.instantiate (*axis_range, unmapped_range, *axis_triple_distances);
      /* (-1, -1), (0, 0), (1, 1) mappings will be added later, so avoid
       * duplicates here */
      اگر (mapping.must_include ())
        جاری;
      قدر_mappings.push (mapping);
    }

    Axis_قدر_Map m;
    m.سیٹ_mapping (-1.f, -1.f);
    قدر_mappings.push (m);

    m.سیٹ_mapping (0.f, 0.f);
    قدر_mappings.push (m);

    m.سیٹ_mapping (1.f, 1.f);
    قدر_mappings.push (m);

    قدر_mappings.qsort ();

    اگر (unlikely (قدر_mappings.اندر_error ()))
      return_trace (جھوٹ);

    برائے (ثابت خودکار& _ : قدر_mappings)
    {
      اگر (!_.serialize (c->serializer))
        return_trace (جھوٹ);
    }
    اگر (!c->serializer->check_تفویض (برآمد->len, قدر_mappings.لمبائی, HB_SERIALIZE_ERROR_INT_حد_سے_تجاوز))
      return_trace (جھوٹ);
    /* Hand the instantiated mappings to the caller; avar2 offset
     * compensation needs them to locate the new mapping's kinks. */
    اگر (برآمد_mappings)
      *برآمد_mappings = std::move (قدر_mappings);
    return_trace (سچ);
  }

  عوامی:
  DEFINE_سائز_ARRAY (2, *یہ);
};

/* One knot of the avar2 offset-compensation function
 * offset(z) = inv_renorm(z) - z, in new intermediate space. */
ساخت avar2_انحراف_knot_t
{
  دوہرا z;
  دوہرا انحراف;
};

/* Insert a knot keeping the vector sorted by z; first insertion wins on
 * duplicate z. */
ساکن اندرونی خالی
avar2_جوڑو_knot (hb_سمتیہ_t<avar2_انحراف_knot_t> &knots, دوہرا z, دوہرا انحراف)
{
  غیرعلامتی i = 0;
  جب (i < knots.لمبائی && knots.arrayZ[i].z < z) i++;
  اگر (i < knots.لمبائی && knots.arrayZ[i].z == z) واپس;
  knots.push (avar2_انحراف_knot_t {0.0, 0.0});
  اگر (unlikely (knots.اندر_error ())) واپس;
  برائے (غیرعلامتی j = knots.لمبائی - 1; j > i; j--)
    knots.arrayZ[j] = knots.arrayZ[j - 1];
  knots.arrayZ[i] = avar2_انحراف_knot_t {z, انحراف};
}

/* Piecewise-linear evaluation over the sorted knots. Inputs are always
 * within [-1, +1] and anchor knots at -1/0/+1 always exist. */
ساکن اندرونی دوہرا
avar2_eval_انحراف (ثابت hb_سمتیہ_t<avar2_انحراف_knot_t> &knots, دوہرا z)
{
  غیرعلامتی len = knots.لمبائی;
  برائے (غیرعلامتی i = 0; i < len; i++)
  {
    اگر (z == knots.arrayZ[i].z) واپس knots.arrayZ[i].انحراف;
    اگر (z < knots.arrayZ[i].z)
    {
      اگر (!i) واپس knots.arrayZ[0].انحراف;
      ثابت خودکار &before = knots.arrayZ[i - 1];
      ثابت خودکار &after = knots.arrayZ[i];
      دوہرا denom = after.z - before.z;
      واپس before.انحراف + (after.انحراف - before.انحراف) * (z - before.z) / denom;
    }
  }
  واپس len ? knots.arrayZ[len - 1].انحراف : 0.0;
}

/* Piecewise-linear evaluation over instantiated avar v1 mappings, as
 * produced by SegmentMaps::subset (sorted, with -1/0/+1 anchors).
 * Matches SegmentMaps::map_float for such well-formed mappings. */
ساکن اندرونی دوہرا
_avar2_map_new_mapping (ثابت hb_سمتیہ_t<Axis_قدر_Map> &mappings, دوہرا v,
			غیرعلامتی from_انحراف = 0, غیرعلامتی مقصد_انحراف = 1)
{
  غیرعلامتی len = mappings.لمبائی;
  اگر (!len) واپس v;

  برائے (غیرعلامتی i = 0; i < len; i++)
  {
    دوہرا from = (دوہرا) mappings.arrayZ[i].coords[from_انحراف].مقصد_float ();
    اگر (v == from)
      واپس (دوہرا) mappings.arrayZ[i].coords[مقصد_انحراف].مقصد_float ();
    اگر (v < from)
    {
      دوہرا to = (دوہرا) mappings.arrayZ[i].coords[مقصد_انحراف].مقصد_float ();
      اگر (!i) واپس v - from + to;
      دوہرا پچھلا_from = (دوہرا) mappings.arrayZ[i - 1].coords[from_انحراف].مقصد_float ();
      دوہرا پچھلا_مقصد = (دوہرا) mappings.arrayZ[i - 1].coords[مقصد_انحراف].مقصد_float ();
      دوہرا denom = from - پچھلا_from;
      اگر (denom == 0.0) واپس پچھلا_مقصد;
      واپس پچھلا_مقصد + (to - پچھلا_مقصد) * (v - پچھلا_from) / denom;
    }
  }
  واپس v - (دوہرا) mappings.arrayZ[len - 1].coords[from_انحراف].مقصد_float ()
           + (دوہرا) mappings.arrayZ[len - 1].coords[مقصد_انحراف].مقصد_float ();
}

/* Inverse of _avar2_map_new_mapping: pull an output coordinate back to a
 * preimage input coordinate. For a non-strictly-monotone mapping this picks
 * one preimage, which is fine for its only use (augmenting the error
 * estimator's sample set). */
ساکن اندرونی دوہرا
_avar2_unmap_new_mapping (ثابت hb_سمتیہ_t<Axis_قدر_Map> &mappings, دوہرا v)
{
  واپس _avar2_map_new_mapping (mappings, v, 1, 0);
}

/* Plain (non-avar) fvar-style normalization of a user value against a
 * user-space (min, default, max) triple. */
ساکن اندرونی دوہرا
avar2_normalize_قدر (دوہرا v, دوہرا min, دوہرا def, دوہرا max)
{
  v = hb_clamp (v, min, max);
  اگر (v == def) واپس 0.0;
  اگر (v < def) واپس def == min ? 0.0 : (v - def) / (def - min);
  واپس def == max ? 0.0 : (v - def) / (max - def);
}

/* Inverse of the above: map a normalized value back to user space. */
ساکن اندرونی دوہرا
avar2_denormalize_قدر (دوہرا v, دوہرا min, دوہرا def, دوہرا max)
{
  اگر (v == 0.0) واپس def;
  واپس v < 0.0 ? def + v * (def - min) : def + v * (max - def);
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
ساکن اندرونی غیرعلامتی
avar2_estimate_انحراف_error (ثابت SegmentMaps &old_seg,
                              ثابت hb_سمتیہ_t<Axis_قدر_Map> &new_mapping,
                              دوہرا old_min, دوہرا old_def, دوہرا old_max,
                              دوہرا new_min, دوہرا new_def, دوہرا new_max,
                              ثابت hb_سمتیہ_t<avar2_انحراف_knot_t> &knots)
{
  constexpr غیرعلامتی samples = 257;
  hb_سمتیہ_t<دوہرا> us;
  اگر (unlikely (!us.alloc (samples + old_seg.as_array ().لمبائی +
                           new_mapping.لمبائی + knots.لمبائی)))
    واپس UINT_MAX; /* estimator only; fail towards "worse" */
  برائے (غیرعلامتی i = 0; i < samples; i++)
    us.push (new_min + (new_max - new_min) * i / (samples - 1));
  برائے (ثابت خودکار &_ : old_seg.as_array ())
    us.push (avar2_denormalize_قدر ((دوہرا) _.coords[0].مقصد_float (),
                                       old_min, old_def, old_max));
  برائے (ثابت خودکار &_ : new_mapping)
    us.push (avar2_denormalize_قدر ((دوہرا) _.coords[0].مقصد_float (),
                                       new_min, new_def, new_max));
  برائے (ثابت خودکار &knot : knots)
    us.push (avar2_denormalize_قدر (
               _avar2_unmap_new_mapping (new_mapping, knot.z),
               new_min, new_def, new_max));
  اگر (unlikely (us.اندر_error ())) واپس UINT_MAX;

  غیرعلامتی max_err = 0;
  برائے (دوہرا u : us)
  {
    اگر (u < new_min || u > new_max) جاری;
    دوہرا n_old = avar2_normalize_قدر (u, old_min, old_def, old_max);
    دوہرا old_final = (دوہرا) old_seg.map_float ((اعشاری) n_old);
    دوہرا n_new = avar2_normalize_قدر (u, new_min, new_def, new_max);
    دوہرا z = _avar2_map_new_mapping (new_mapping, n_new);
    دوہرا new_final = z + avar2_eval_انحراف (knots, z);
    صحیح err = مطلق_قدر ((صحیح) roundf ((اعشاری) (old_final * 16384.0)) -
                   (صحیح) roundf ((اعشاری) (new_final * 16384.0)));
    اگر ((غیرعلامتی) err > max_err) max_err = err;
  }
  واپس max_err;
}

ساخت avar
{
  ساکن constexpr hb_لیبل_t جدول_لیبل = HB_OT_لیبل_avar;

  منطقی has_ڈیٹا () ثابت { واپس نسخہ.مقصد_int (); }

  ثابت SegmentMaps* get_segment_maps () ثابت
  { واپس &پہلا_Axis_Segment_Maps; }

  غیرعلامتی get_axis_گنتی () ثابت
  { واپس axis_گنتی_2; }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    اگر (!(نسخہ.sanitize (c) &&
	  hb_barrier () &&
	  (نسخہ.major == 1
#اگر_غیر_تعریف_شدہ HB_NO_AVAR2
	   || نسخہ.major == 2
#اگر_ختم
	   ) &&
	  c->check_struct (یہ)))
      return_trace (جھوٹ);

    ثابت SegmentMaps *map = &پہلا_Axis_Segment_Maps;
    غیرعلامتی صحیح گنتی = axis_گنتی_2;
    برائے (غیرعلامتی صحیح i = 0; i < گنتی; i++)
    {
      اگر (unlikely (!map->sanitize (c)))
	return_trace (جھوٹ);
      map = &StructAfter<SegmentMaps> (*map);
    }

#اگر_غیر_تعریف_شدہ HB_NO_AVAR2
    اگر (نسخہ.major < 2)
      return_trace (سچ);
    hb_barrier ();

    ثابت خودکار &v2 = * (ثابت avar_V2_دم *) map;
    اگر (unlikely (!v2.sanitize (c, یہ)))
      return_trace (جھوٹ);
#اگر_ختم

    return_trace (سچ);
  }

  خالی map_coords_16_16 (صحیح *coords, غیرعلامتی صحیح coords_لمبائی) ثابت
  {
    غیرعلامتی صحیح گنتی = hb_min (coords_لمبائی, axis_گنتی_2);

    ثابت SegmentMaps *map = &پہلا_Axis_Segment_Maps;
    برائے (غیرعلامتی صحیح i = 0; i < گنتی; i++)
    {
      coords[i] = roundf (map->map_float (coords[i] / 65536.f) * 65536.f);
      map = &StructAfter<SegmentMaps> (*map);
    }

#اگر_غیر_تعریف_شدہ HB_NO_AVAR2
    اگر (نسخہ.major < 2)
      واپس;
    hb_barrier ();

    برائے (; گنتی < axis_گنتی_2; گنتی++)
      map = &StructAfter<SegmentMaps> (*map);

    ثابت خودکار &v2 = * (ثابت avar_V2_دم *) map;

    ثابت خودکار &varidx_map = یہ+v2.varIdxMap;
    ثابت خودکار &var_store = یہ+v2.varStore;
    خودکار *var_store_cache = var_store.create_cache ();

    hb_سمتیہ_t<صحیح> coords_2_14;
    coords_2_14.resize (coords_لمبائی);
    برائے (غیرعلامتی i = 0; i < coords_لمبائی; i++)
      coords_2_14[i] = roundf (coords[i] / 4.f); // 16.16 -> 2.14

    hb_سمتیہ_t<صحیح> برآمد;
    برآمد.alloc (coords_لمبائی);
    برائے (غیرعلامتی i = 0; i < coords_لمبائی; i++)
    {
      صحیح v = coords[i];
      uint32_t varidx = varidx_map.map (i);
      اعشاری delta = var_store.get_delta (varidx, coords_2_14.arrayZ, coords_2_14.لمبائی, var_store_cache);
      /* Apply the delta unclamped and clamp only the result to [-1, +1],
       * matching fontTools. Since inputs and results are in [-1, +1],
       * deltas beyond ±2 are equivalent to ±2; clamp to that range only
       * to keep the float->int conversion safe. */
      اعشاری d = hb_clamp (delta * 4, -(اعشاری) (1<<17), +(اعشاری) (1<<17)); // 2.14 -> 16.16
      v += (صحیح) roundf (d);
      v = hb_clamp (v, -(1<<16), +(1<<16));
      برآمد.push (v);
    }
    برائے (غیرعلامتی i = 0; i < coords_لمبائی; i++)
      coords[i] = برآمد[i];

    OT::شے_Variation_Store::destroy_cache (var_store_cache);
#اگر_ختم
  }

  /* An avar version 2 font is never downgraded to v1: even with a NULL
   * varStore offset (legal; maps like plain v1) the avar2 subsetting path
   * applies, with "no variation" semantics for rows that don't resolve —
   * offset compensation then creates delta rows on demand. */
  منطقی has_v2_ڈیٹا () ثابت { واپس نسخہ.major > 1; }

  /* Resolve the avar2 VarStore and VarIdxMap, for the subset planner.
   * Either pointer may be to the Null object (nullable offsets). */
  منطقی get_v2_store_and_map (ثابت شے_Variation_Store **store,
                             ثابت Delta_سیٹ_اشاریہ_Map **varidx_map) ثابت
  {
#اگر_غیر_تعریف_شدہ HB_NO_AVAR2
    اگر (نسخہ.major < 2) واپس جھوٹ;
    ثابت SegmentMaps *map = &پہلا_Axis_Segment_Maps;
    برائے (غیرعلامتی i = 0; i < axis_گنتی_2; i++)
      map = &StructAfter<SegmentMaps> (*map);
    ثابت خودکار &v2 = * (ثابت avar_V2_دم *) map;
    *store = &(یہ+v2.varStore);
    *varidx_map = &(یہ+v2.varIdxMap);
    واپس سچ;
#ورنہ
    واپس جھوٹ;
#اگر_ختم
  }

  // axis normalization is done in 2.14 here
  // TODO: deprecate this API once fonttools is updated to use 16.16 normalization
  منطقی map_coords_2_14 (اعشاری *coords, غیرعلامتی صحیح coords_لمبائی,
			منطقی v1_only = جھوٹ) ثابت
  {
    hb_سمتیہ_t<صحیح> coords_2_14;
    اگر (!v1_only && !coords_2_14.resize (coords_لمبائی)) واپس جھوٹ;
    غیرعلامتی صحیح گنتی = hb_min (coords_لمبائی, axis_گنتی_2);

    ثابت SegmentMaps *map = &پہلا_Axis_Segment_Maps;
    برائے (غیرعلامتی صحیح i = 0; i < گنتی; i++)
    {
      صحیح v = roundf (map->map_float (coords[i]) * 16384.f);
      اگر (!v1_only)
	coords_2_14[i] = v;
      coords[i] = v / 16384.f;
      map = &StructAfter<SegmentMaps> (*map);
    }

    اگر (v1_only)
      واپس سچ;

#اگر_غیر_تعریف_شدہ HB_NO_AVAR2
    اگر (نسخہ.major < 2)
      واپس سچ;
    hb_barrier ();

    برائے (; گنتی < axis_گنتی_2; گنتی++)
      map = &StructAfter<SegmentMaps> (*map);

    ثابت خودکار &v2 = * (ثابت avar_V2_دم *) map;

    ثابت خودکار &varidx_map = یہ+v2.varIdxMap;
    ثابت خودکار &var_store = یہ+v2.varStore;
    خودکار *var_store_cache = var_store.create_cache ();

    برائے (غیرعلامتی i = 0; i < coords_لمبائی; i++)
    {
      صحیح v = coords_2_14[i];
      uint32_t varidx = varidx_map.map (i);
      اعشاری delta = var_store.get_delta (varidx, coords_2_14.arrayZ, coords_2_14.لمبائی, var_store_cache);
      /* As above: apply the delta unclamped (±2 covers every useful case)
       * and clamp the result to [-1, +1], matching fontTools. */
      v += (صحیح) hb_clamp (roundf (delta), -(اعشاری) (1<<15), +(اعشاری) (1<<15));
      v = hb_clamp (v, -(1<<14), +(1<<14));
      coords[i] = v / 16384.f;
    }

    OT::شے_Variation_Store::destroy_cache (var_store_cache);
    واپس سچ;
#ورنہ
    واپس نسخہ.major < 2;
#اگر_ختم
  }

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (یہ);
    غیرعلامتی retained_axis_گنتی = c->plan->axes_اشاریہ_map.get_population ();
    اگر (!retained_axis_گنتی) //all axes are pinned/dropped
      return_trace (جھوٹ);

    avar *برآمد = c->serializer->allocate_min<avar> ();
    اگر (unlikely (!برآمد)) return_trace (جھوٹ);

    برآمد->نسخہ.major = c->plan->has_avar2 ? 2 : 1;
    برآمد->نسخہ.minor = 0;
    اگر (!c->serializer->check_تفویض (برآمد->axis_گنتی_2, retained_axis_گنتی, HB_SERIALIZE_ERROR_INT_حد_سے_تجاوز))
      return_trace (جھوٹ);

    /* For avar2, keep the instantiated v1 mappings around; offset
     * compensation needs them to locate the new mappings' kinks. */
    hb_سمتیہ_t<hb_سمتیہ_t<Axis_قدر_Map>> new_mappings;
    اگر (c->plan->has_avar2 && !new_mappings.resize (axis_گنتی_2))
      return_trace (جھوٹ);

    ثابت hb_map_t& axes_اشاریہ_map = c->plan->axes_اشاریہ_map;
    ثابت SegmentMaps *map = &پہلا_Axis_Segment_Maps;
    غیرعلامتی گنتی = axis_گنتی_2;
    برائے (غیرعلامتی صحیح i = 0; i < گنتی; i++)
    {
      اگر (axes_اشاریہ_map.has (i))
      {
        hb_لیبل_t *axis_لیبل_2;
        اگر (!c->plan->axes_old_اشاریہ_لیبل_map.has (i, &axis_لیبل_2))
          return_trace (جھوٹ);

        Triple *axis_location;
        اگر (c->plan->has_avar2 &&
            c->plan->user_axes_location.has (*axis_لیبل_2, &axis_location) &&
            axis_location->is_نقطہ ())
        {
          /* Pinned axis in avar2 mode: serialize identity segment map
           * {-1->-1, 0->0, 1->1}. The axis is kept in fvar as hidden,
           * so avar needs a segment map entry for it. */
          خودکار *شناختی_عنصر_map = c->serializer->آغاز_embed<SegmentMaps> ();
          اگر (unlikely (!c->serializer->extend_min (شناختی_عنصر_map)))
            return_trace (جھوٹ);
          Axis_قدر_Map m;
          m.سیٹ_mapping (-1.f, -1.f);
          اگر (!m.serialize (c->serializer)) return_trace (جھوٹ);
          m.سیٹ_mapping (0.f, 0.f);
          اگر (!m.serialize (c->serializer)) return_trace (جھوٹ);
          m.سیٹ_mapping (1.f, 1.f);
          اگر (!m.serialize (c->serializer)) return_trace (جھوٹ);
          اگر (!c->serializer->check_تفویض (شناختی_عنصر_map->len, 3u,
                                            HB_SERIALIZE_ERROR_INT_حد_سے_تجاوز))
            return_trace (جھوٹ);
        }
        ورنہ
        {
          /* Restricted or free axis: use standard SegmentMaps::subset() */
          اگر (!map->subset (c, *axis_لیبل_2,
                            c->plan->has_avar2 ? &new_mappings[i] : خالی_اشاریہ))
            return_trace (جھوٹ);
        }
      }
      map = &StructAfter<SegmentMaps> (*map);
    }

    اگر (c->plan->has_avar2)
      return_trace (_subset_avar2 (c, new_mappings));

    return_trace (سچ);
  }

  نجی:
  ساخت avar2_اشاریہ_map_plan_t
  {
    منطقی init (ثابت hb_سمتیہ_t<uint32_t> &varidx_mapping,
	       ثابت hb_map_t &axes_اشاریہ_map,
	       غیرعلامتی axis_گنتی)
    {
      اگر (!output_map.alloc (axes_اشاریہ_map.get_population ()))
	واپس جھوٹ;

      منطقی has_no_variation = جھوٹ;
      غیرعلامتی max_outer = 0, max_inner = 0;
      برائے (غیرعلامتی i = 0; i < axis_گنتی; i++)
      {
	اگر (!axes_اشاریہ_map.has (i)) جاری;
	uint32_t varidx = varidx_mapping[i];
	output_map.push (varidx);
	اگر (varidx == HB_OT_LAYOUT_NO_VARIATIONS_اشاریہ)
	{
	  has_no_variation = سچ;
	  جاری;
	}
	max_outer = hb_max (max_outer, varidx >> 16);
	max_inner = hb_max (max_inner, varidx & 0xFFFF);
      }
      اگر (output_map.اندر_error () ||
	  output_map.لمبائی != axes_اشاریہ_map.get_population ())
	واپس جھوٹ;

      اگر (has_no_variation)
      {
	چوڑائی = 4;
	inner_ثنائی_ہندسہ_گنتی = 16;
      }
      ورنہ
      {
	inner_ثنائی_ہندسہ_گنتی = hb_max (1u, hb_ثنائی_ہندسہ_storage (max_inner));
	غیرعلامتی outer_ثنائی_ہندسہ_گنتی = hb_max (1u, hb_ثنائی_ہندسہ_storage (max_outer));
	چوڑائی = hb_clamp ((inner_ثنائی_ہندسہ_گنتی + outer_ثنائی_ہندسہ_گنتی + 7) / 8, 1u, 4u);
	اگر (inner_ثنائی_ہندسہ_گنتی + outer_ثنائی_ہندسہ_گنتی > چوڑائی * 8)
	  inner_ثنائی_ہندسہ_گنتی = چوڑائی * 8 - outer_ثنائی_ہندسہ_گنتی;
      }
      واپس سچ;
    }

    غیرعلامتی get_inner_ثنائی_ہندسہ_گنتی () ثابت { واپس inner_ثنائی_ہندسہ_گنتی; }
    غیرعلامتی get_چوڑائی () ثابت { واپس چوڑائی; }
    hb_array_t<ثابت uint32_t> get_output_map () ثابت { واپس output_map.as_array (); }

    غیرعلامتی inner_ثنائی_ہندسہ_گنتی = 1;
    غیرعلامتی چوڑائی = 1;
    hb_سمتیہ_t<uint32_t> output_map;
  };

  منطقی _subset_avar2 (hb_subset_context_t *c,
                      ثابت hb_سمتیہ_t<hb_سمتیہ_t<Axis_قدر_Map>> &new_mappings) ثابت
  {
#اگر defined (HB_NO_VAR) || defined (HB_NO_AVAR2)
    /* Not reachable: the plan never sets has_avar2 in these configurations. */
    واپس جھوٹ;
#ورنہ

    /* 1. Locate original avar2 data, keeping per-axis old segment maps */
    hb_سمتیہ_t<ثابت SegmentMaps *> old_seg_maps;
    اگر (!old_seg_maps.alloc (axis_گنتی_2)) واپس جھوٹ;
    ثابت SegmentMaps *map = &پہلا_Axis_Segment_Maps;
    برائے (غیرعلامتی i = 0; i < axis_گنتی_2; i++)
    {
      old_seg_maps.push (map);
      map = &StructAfter<SegmentMaps> (*map);
    }

    ثابت خودکار &v2 = * (ثابت avar_V2_دم *) map;
    ثابت خودکار &varidx_map = یہ+v2.varIdxMap;
    ثابت خودکار &var_store = یہ+v2.varStore;

    خودکار fvar_axes = c->plan->ماخذ->جدول.fvar->get_axes ();

    /* 2. Compute default deltas by evaluating VarStore at old defaults */
    hb_سمتیہ_t<صحیح> default_coords;
    اگر (!default_coords.resize (axis_گنتی_2)) واپس جھوٹ;
    برائے (غیرعلامتی i = 0; i < axis_گنتی_2; i++)
    {
      hb_لیبل_t *axis_لیبل_2;
      اگر (c->plan->axes_old_اشاریہ_لیبل_map.has (i, &axis_لیبل_2) &&
          c->plan->old_intermediates.has (*axis_لیبل_2))
      {
        اعشاری d_i = (اعشاری) c->plan->old_intermediates.get (*axis_لیبل_2).درمیان;
        default_coords[i] = roundf (d_i * 16384.f);
      }
      ورنہ
        default_coords[i] = 0;
    }

    خودکار *store_cache = var_store.create_cache ();
    hb_سمتیہ_t<اعشاری> default_deltas;
    اگر (!default_deltas.resize (axis_گنتی_2)) {
      شے_Variation_Store::destroy_cache (store_cache);
      واپس جھوٹ;
    }
    برائے (غیرعلامتی i = 0; i < axis_گنتی_2; i++)
    {
      uint32_t varidx = varidx_map.map (i);
      اگر (varidx == HB_OT_LAYOUT_NO_VARIATIONS_اشاریہ)
        default_deltas[i] = 0.f;
      ورنہ
        default_deltas[i] = var_store.get_delta (varidx, default_coords.arrayZ,
                                                  default_coords.لمبائی, store_cache);
    }
    شے_Variation_Store::destroy_cache (store_cache);

    /* 3. Rebase IVS regions */
    شے_variations_t شے_vars;
    اگر (!شے_vars.create_from_شے_varstore (var_store, c->plan->axes_old_اشاریہ_لیبل_map))
      واپس جھوٹ;
    اگر (!شے_vars.instantiate_tuple_vars (c->plan->old_intermediates,
                                           c->plan->axes_triple_distances,
                                           جھوٹ))
      واپس جھوٹ;

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
    hb_سمتیہ_t<uint32_t> new_varidx_mapping;
    اگر (!new_varidx_mapping.resize (axis_گنتی_2)) واپس جھوٹ;
    برائے (غیرعلامتی i = 0; i < axis_گنتی_2; i++)
    {
      uint32_t varidx = varidx_map.map (i);
      اگر (varidx != HB_OT_LAYOUT_NO_VARIATIONS_اشاریہ &&
	  !var_store.has_delta_سیٹ (varidx))
	varidx = HB_OT_LAYOUT_NO_VARIATIONS_اشاریہ;
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
    hb_hashmap_t<uint32_t, غیرعلامتی> varidx_ref_گنتی;
    برائے (غیرعلامتی i = 0; i < axis_گنتی_2; i++)
    {
      اگر (!c->plan->axes_اشاریہ_map.has (i)) جاری; /* self-contained: dropped */
      uint32_t varidx = new_varidx_mapping[i];
      اگر (varidx == HB_OT_LAYOUT_NO_VARIATIONS_اشاریہ) جاری;
      غیرعلامتی *گنتی;
      اگر (varidx_ref_گنتی.has (varidx, &گنتی))
        (*گنتی)++;
      ورنہ اگر (!varidx_ref_گنتی.سیٹ (varidx, 1))
        واپس جھوٹ;
    }
    برائے (غیرعلامتی i = 0; i < axis_گنتی_2; i++)
    {
      hb_لیبل_t *axis_لیبل_ptr;
      اگر (!c->plan->axes_old_اشاریہ_لیبل_map.has (i, &axis_لیبل_ptr))
        واپس جھوٹ;
      /* Only axes that will receive offset compensation (restricted or
       * pinned) can contaminate a shared row. */
      اگر (!c->plan->axes_اشاریہ_map.has (i) ||
          !c->plan->user_axes_location.has (*axis_لیبل_ptr))
        جاری;
      uint32_t varidx = new_varidx_mapping[i];
      اگر (varidx == HB_OT_LAYOUT_NO_VARIATIONS_اشاریہ)
        جاری; /* gets a fresh, private VarData in the offset loop below */
      غیرعلامتی *گنتی;
      اگر (!varidx_ref_گنتی.has (varidx, &گنتی) || *گنتی <= 1)
        جاری; /* sole owner: safe to write offsets in place */
      غیرعلامتی outer = varidx >> 16;
      غیرعلامتی new_inner = شے_vars.duplicate_صف (outer, varidx & 0xFFFF);
      اگر (unlikely (new_inner == (غیرعلامتی) -1)) واپس جھوٹ;
      new_varidx_mapping[i] = (outer << 16) | new_inner;
      (*گنتی)--;
    }

    /* 6. Add offset compensation tuples.
     * Track processed (outer,inner) pairs to avoid adding duplicate biases
     * when multiple axes share the same varIdx. */
    hb_سیٹ_t processed_varidxes;
    برائے (غیرعلامتی i = 0; i < axis_گنتی_2; i++)
    {
      hb_لیبل_t *axis_لیبل_ptr;
      اگر (!c->plan->axes_old_اشاریہ_لیبل_map.has (i, &axis_لیبل_ptr))
        واپس جھوٹ;
      hb_لیبل_t axis_لیبل_2 = *axis_لیبل_ptr;

      /* Self-contained pinned axes are removed from fvar/avar; their
       * contribution is baked into the variation tables instead. */
      اگر (!c->plan->axes_اشاریہ_map.has (i))
        جاری;

      Triple *new_user;
      اگر (c->plan->user_axes_location.has (axis_لیبل_2, &new_user))
      {
        /* This axis is being restricted or pinned */
        Triple *old_int;
        اگر (!c->plan->old_intermediates.has (axis_لیبل_2, &old_int))
          واپس جھوٹ;

        اعشاری a_i = (اعشاری) old_int->minimum;
        اعشاری d_i = (اعشاری) old_int->درمیان;
        اعشاری b_i = (اعشاری) old_int->زیادہ_سے_زیادہ;

        صحیح d_int = roundf (d_i * 16384.f);

        منطقی is_pinned = new_user->is_نقطہ ();

        uint32_t varidx = new_varidx_mapping[i];
        غیرعلامتی outer, inner, شے_گنتی_2;

        اگر (varidx == HB_OT_LAYOUT_NO_VARIATIONS_اشاریہ)
        {
          /* No existing avar2 mapping. Create new VarData. */
          outer = شے_vars.جوڑو_vardata (1);
          inner = 0;
          شے_گنتی_2 = 1;
          new_varidx_mapping[i] = (outer << 16) | inner;
          default_deltas[i] = 0.f; /* no prior default delta */
        }
        ورنہ
        {
          outer = varidx >> 16;
          inner = varidx & 0xFFFF;
          شے_گنتی_2 = شے_vars.get_شے_گنتی (outer);
        }

        /* Empty-region bias: d_int + round(defaultDelta) */
        صحیح bias = d_int + (صحیح) roundf (default_deltas[i]);
        اگر (bias != 0)
        {
          hb_hashmap_t<hb_لیبل_t, Triple> خالی_خانے_region;
          شے_vars.جوڑو_tuple (outer, std::move (خالی_خانے_region),
                               inner, bias, شے_گنتی_2);
        }

        اگر (!is_pinned)
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
          hb_سمتیہ_t<avar2_انحراف_knot_t> knots;
          avar2_جوڑو_knot (knots, -1.0, (دوہرا) a_i + 1.0);
          avar2_جوڑو_knot (knots, 0.0, (دوہرا) d_i);
          avar2_جوڑو_knot (knots, 1.0, (دوہرا) b_i - 1.0);

          ثابت hb_سمتیہ_t<Axis_قدر_Map> &new_mapping = new_mappings[i];

          اعشاری min_f = 0.f, def_f = 0.f, max_f = 0.f;
          اگر (likely (i < fvar_axes.لمبائی))
            fvar_axes[i].get_coordinates (min_f, def_f, max_f);
          دوہرا old_min = (دوہرا) min_f;
          دوہرا old_def = (دوہرا) def_f;
          دوہرا old_max = (دوہرا) max_f;

          /* If the axis default MOVED, inv_renorm also kinks where the OLD
           * default lands in the new space (the old intermediate coordinate
           * crosses 0 there), at
           *   z = z_old  ->  -z_old
           * Omitting that knot (as a plain two-tent encoding would) makes
           * interior coordinates wrong. */
          دوہرا z_old = avar2_normalize_قدر ((دوہرا) old_def,
                                                 (دوہرا) new_user->minimum,
                                                 (دوہرا) new_user->درمیان,
                                                 (دوہرا) new_user->زیادہ_سے_زیادہ);
          z_old = _avar2_map_new_mapping (new_mapping, z_old);
          z_old = (دوہرا) roundf ((اعشاری) (z_old * 16384.0)) / 16384.0;
          اگر (-1.0 < z_old && z_old < 1.0)
            avar2_جوڑو_knot (knots, z_old, -z_old);

          /* Interior avar v1 breakpoints inside the retained range each put
           * a kink in offset(z). Sampling only {-1, 0, +1, z_old} would
           * linearly interpolate across those kinks. The instantiated
           * mapping keeps exactly the in-range old breakpoints, and the new
           * mapping kinks at each one's output coordinate; add that z with
           * its old intermediate value so offset(z) is reproduced at every
           * kink. */
          hb_سمتیہ_t<avar2_انحراف_knot_t> with_breakpoints (knots);
          برائے (ثابت خودکار &m : new_mapping)
          {
            دوہرا from = (دوہرا) m.coords[0].مقصد_float ();
            اگر (from == -1.0 || from == 0.0 || from == 1.0)
              جاری; /* anchors already seeded */
            دوہرا z = (دوہرا) m.coords[1].مقصد_float ();
            اگر (!(-1.0 < z && z < 1.0))
              جاری;
            دوہرا user = avar2_denormalize_قدر (from,
                                                    (دوہرا) new_user->minimum,
                                                    (دوہرا) new_user->درمیان,
                                                    (دوہرا) new_user->زیادہ_سے_زیادہ);
            دوہرا n_old = avar2_normalize_قدر (user, old_min, old_def, old_max);
            دوہرا x_old = (دوہرا) old_seg_maps[i]->map_float ((اعشاری) n_old);
            x_old = (دوہرا) roundf ((اعشاری) (x_old * 16384.0)) / 16384.0;
            avar2_جوڑو_knot (with_breakpoints, z, x_old - z);
          }

          اگر (unlikely (knots.اندر_error () || with_breakpoints.اندر_error ()))
            واپس جھوٹ;

          /* Extra tents cost F2Dot14 rounding, so for a steep segment they
           * can add more quantization noise than the structural error they
           * remove. Keep the interior breakpoints only when they do not
           * increase the estimated residual; this makes the collection a
           * strict (never-worse) improvement over the {-1, 0, +1, z_old}
           * anchors. Warn when even the better choice is not bit-exact (a
           * steep retained segment that cannot be reproduced in F2Dot14). */
          غیرعلامتی err = avar2_estimate_انحراف_error (*old_seg_maps[i], new_mapping,
                                                       old_min, old_def, old_max,
                                                       (دوہرا) new_user->minimum,
                                                       (دوہرا) new_user->درمیان,
                                                       (دوہرا) new_user->زیادہ_سے_زیادہ,
                                                       knots);
          اگر (with_breakpoints.لمبائی != knots.لمبائی)
          {
            غیرعلامتی err_with = avar2_estimate_انحراف_error (*old_seg_maps[i], new_mapping,
                                                              old_min, old_def, old_max,
                                                              (دوہرا) new_user->minimum,
                                                              (دوہرا) new_user->درمیان,
                                                              (دوہرا) new_user->زیادہ_سے_زیادہ,
                                                              with_breakpoints);
            اگر (err_with <= err)
            {
              knots = std::move (with_breakpoints);
              err = err_with;
            }
          }
          اگر (err > 8)
            DEBUG_پیغام (SUBSET, خالی_اشاریہ,
                       "avar2 offset compensation is approximate for axis %c%c%c%c: "
                       "max residual %u F2Dot14 units",
                       HB_UNTAG (axis_لیبل_2), err);

          /* Synthesize tents. Adjacent tents evaluate to zero at each
           * other's peaks, so each knot's delta is offset(z) - offset(0);
           * the base value offset(0) = d_i is carried by the empty-region
           * bias above. This reduces to the classic pair of tents
           * (-1,-1,0) / (0,+1,+1) when the default is unchanged and there
           * are no interior knots. */
          برائے (غیرعلامتی k = 0; k < knots.لمبائی; k++)
          {
            دوہرا z = knots.arrayZ[k].z;
            اگر (z == 0.0) جاری;
            صحیح delta = (صحیح) roundf ((اعشاری) ((knots.arrayZ[k].انحراف - (دوہرا) d_i) * 16384.0));
            اگر (!delta) جاری;
            دوہرا زیریں, بالائی;
            اگر (z > 0.0)
            {
              زیریں = knots.arrayZ[k - 1].z;
              بالائی = k + 1 < knots.لمبائی ? knots.arrayZ[k + 1].z : z;
            }
            ورنہ
            {
              بالائی = knots.arrayZ[k + 1].z;
              زیریں = k > 0 ? knots.arrayZ[k - 1].z : z;
            }
            hb_hashmap_t<hb_لیبل_t, Triple> region;
            اگر (unlikely (!region.سیٹ (axis_لیبل_2, Triple (زیریں, z, بالائی))))
              واپس جھوٹ;
            شے_vars.جوڑو_tuple (outer, std::move (region),
                                 inner, delta, شے_گنتی_2);
          }
        }
      }
      ورنہ
      {
        /* Free or private axis — not being restricted.
         * If it has a non-zero default delta, add it back as a bias.
         * Skip if this (outer,inner) was already processed (shared varIdx). */
        uint32_t varidx = new_varidx_mapping[i];
        اگر (varidx == HB_OT_LAYOUT_NO_VARIATIONS_اشاریہ)
          جاری;

        اگر (processed_varidxes.has (varidx))
          جاری;
        processed_varidxes.جوڑو (varidx);

        غیرعلامتی outer = varidx >> 16;
        غیرعلامتی inner = varidx & 0xFFFF;
        صحیح dd = (صحیح) roundf (default_deltas[i]);
        اگر (dd != 0)
        {
          غیرعلامتی شے_گنتی_2 = شے_vars.get_شے_گنتی (outer);
          hb_hashmap_t<hb_لیبل_t, Triple> خالی_خانے_region;
          شے_vars.جوڑو_tuple (outer, std::move (خالی_خانے_region),
                               inner, dd, شے_گنتی_2);
        }
      }
    }

    /* 7. Finalize: build region list + convert to varstore */
    اگر (!شے_vars.build_region_list ()) واپس جھوٹ;
    اگر (!شے_vars.as_شے_varstore (سچ /* optimize */,
                                     جھوٹ /* use_no_variation_idx */))
      واپس جھوٹ;

    /* 8. Apply varidx_map optimization remapping */
    ثابت خودکار &opt_varidx_map = شے_vars.get_varidx_map ();
    برائے (غیرعلامتی i = 0; i < axis_گنتی_2; i++)
    {
      uint32_t varidx = new_varidx_mapping[i];
      اگر (varidx == HB_OT_LAYOUT_NO_VARIATIONS_اشاریہ)
        جاری;
      uint32_t *new_idx;
      اگر (opt_varidx_map.has (varidx, &new_idx))
        new_varidx_mapping[i] = *new_idx;
    }

    /* 9. Serialize avarV2Tail. Entries cover the retained axes only;
     * self-contained pinned axes are removed from fvar. */
    avar2_اشاریہ_map_plan_t اشاریہ_map_plan;
    اگر (!اشاریہ_map_plan.init (new_varidx_mapping,
			      c->plan->axes_اشاریہ_map,
			      axis_گنتی_2))
      واپس جھوٹ;

    خودکار *دم = c->serializer->allocate_سائز<avar_V2_دم> (avar_V2_دم::static_سائز);
    اگر (unlikely (!دم)) واپس جھوٹ;
    اگر (!دم->varIdxMap.serialize_serialize (c->serializer, اشاریہ_map_plan))
      واپس جھوٹ;
    اگر (!دم->varStore.serialize_serialize (c->serializer,
					     شے_vars.has_long_لفظ (),
					     c->plan->axis_لیبل_فہرست,
					     شے_vars.get_region_list (),
					     شے_vars.get_vardata_encodings ()))
      واپس جھوٹ;

    واپس سچ;
#اگر_ختم
  }

  عوامی:

  محفوظ:
  Fixed_نسخہ<>نسخہ;	/* Version of the avar table
				 * initially set to 0x00010000u */
  HBUINT16	reserved;	/* This field is permanently reserved. Set to 0. */
  HBUINT16	axis_گنتی_2;	/* The number of variation axes in the font. This
				 * must be the same number as axisCount in the
				 * 'fvar' table. */
  SegmentMaps	پہلا_Axis_Segment_Maps;

  عوامی:
  DEFINE_سائز_MIN (8);
};

} /* namespace OT */


#اگر_ختم /* HB_OT_VAR_AVAR_TABLE_HH */
