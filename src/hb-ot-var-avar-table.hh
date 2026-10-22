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

#ifndef HB_OT_VAR_AVAR_TABLE_HH
#define HB_OT_VAR_AVAR_TABLE_HH

#شامل "hb-open-type.hh"
#شامل "hb-ot-var-common.hh"
#شامل "hb-ot-var-fvar-table.hh"


/*
 * avar -- Axis Variations
 * https://docs.microsoft.com/en-us/typography/opentype/spec/avar
 */

#define HB_OT_TAG_avar HB_TAG('a','v','a','r')


فضاینام OT {


/* "Spec": https://github.com/be-fonts/boring-expansion-spec/issues/14 */
ساختار avarV2Tail
{
  friend ساختار avar;

  منطقی sanitize (hb_sanitize_context_t *c,
		 ثابت تهی *base) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (varIdxMap.sanitize (c, base) &&
		  varStore.sanitize (c, base));
  }

  محافظتشده:
  Offset32To<DeltaSetIndexMap>	varIdxMap;	/* Offset from the beginning of 'avar' table. */
  Offset32To<ItemVariationStore>	varStore;	/* Offset from the beginning of 'avar' table. */

  عمومی:
  DEFINE_SIZE_STATIC (8);
};


ساختار Axis_مقدار_Map
{
  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این));
  }

  تهی set_mapping (اعشاری from_coord, اعشاری to_coord)
  {
    coords[0].set_float (from_coord);
    coords[1].set_float (to_coord);
  }

  منطقی is_outside_axis_range (ثابت Triple& axis_range) ثابت
  {
    دوگانه from_coord = (دوگانه) coords[0].to_float ();
    بازگشت !axis_range.contains (from_coord);
  }

  منطقی must_include () ثابت
  {
    اعشاری from_coord = coords[0].to_float ();
    اعشاری to_coord = coords[1].to_float ();
    بازگشت (from_coord == -1.f && to_coord == -1.f) ||
           (from_coord == 0.f && to_coord == 0.f) ||
           (from_coord == 1.f && to_coord == 1.f);
  }

  تهی instantiate (ثابت Triple& axis_range,
                    ثابت Triple& unmapped_range,
                    ثابت TripleDistances& triple_distances)
  {
    اعشاری from_coord = coords[0].to_float ();
    اعشاری to_coord = coords[1].to_float ();

    from_coord = renormalize_مقدار ((دوگانه) from_coord, unmapped_range, triple_distances);
    to_coord = renormalize_مقدار ((دوگانه) to_coord, axis_range, triple_distances);

    /* Malformed input can renormalize outside [-1,1], which would
     * overflow F2DOT14. */
    from_coord = hb_clamp (from_coord, -1.f, 1.f);
    to_coord = hb_clamp (to_coord, -1.f, 1.f);

    coords[0].set_float (from_coord);
    coords[1].set_float (to_coord);
  }

  HB_INTERNAL ایستا صحیح cmp (ثابت تهی *pa, ثابت تهی *pb)
  {
    ثابت Axis_مقدار_Map *a = (ثابت Axis_مقدار_Map *) pa;
    ثابت Axis_مقدار_Map *b = (ثابت Axis_مقدار_Map *) pb;

    صحیح a_from = a->coords[0].to_int ();
    صحیح b_from = b->coords[0].to_int ();
    اگر (a_from != b_from)
      بازگشت a_from - b_from;

    /* this should never be reached. according to the spec, all of the axis
     * value map records for a given axis must have different fromCoord values
     * */
    صحیح a_to = a->coords[1].to_int ();
    صحیح b_to = b->coords[1].to_int ();
    بازگشت a_to - b_to;
  }

  منطقی serialize (hb_serialize_context_t *c) ثابت
  {
    TRACE_SERIALIZE (این);
    return_trace (c->embed (این));
  }

  عمومی:
  F2DOT14	coords[2];
//   F2DOT14	fromCoord;	/* A normalized coordinate value obtained using
//				 * default normalization. */
//   F2DOT14	toCoord;	/* The modified, normalized coordinate value. */

  عمومی:
  DEFINE_SIZE_STATIC (4);
};

ساختار SegmentMaps : Array16Of<Axis_مقدار_Map>
{
  اعشاری map_float (اعشاری value, unsigned صحیح from_offset = 0, unsigned صحیح to_offset = 1) ثابت
  {
#define fromCoord coords[from_offset].to_float ()
#define toCoord coords[to_offset].to_float ()

    ثابت auto *map = arrayZ;

    /* The following special-cases are not part of OpenType, which requires
     * that at least -1, 0, and +1 must be mapped. But we include these as
     * part of a better error recovery scheme. */
    اگر (len < 2)
    {
      اگر (!len)
	بازگشت value;
      وگرنه /* len == 1*/
	بازگشت value - map[0].fromCoord + map[0].toCoord;
    }

    // At least two mappings now.

    /* CoreText is wild...
     * PingFangUI avar needs all this special-casing...
     * So we implement an extended version of the spec here,
     * which is more robust and more likely to be compatible with
     * the wild. */

    unsigned start = 0;
    unsigned end = len;
    اگر (map[start].fromCoord == -1 && map[start].toCoord == -1 && map[start+1].fromCoord == -1)
      start++;
    اگر (map[end-1].fromCoord == +1 && map[end-1].toCoord == +1 && map[end-2].fromCoord == +1)
      end--;

    /* Look for exact match first, and do lots of special-casing. */
    unsigned i;
    برای (i = start; i < end; i++)
      اگر (value == map[i].fromCoord)
	بشکن;
    اگر (i < end)
    {
      // There's at least one exact match. See if there are more.
      unsigned j = i;
      برای (; j + 1 < end; j++)
	اگر (value != map[j + 1].fromCoord)
	  بشکن;

      // [i,j] inclusive are all exact matches:

      // If there's only one, return it. This is the only spec-compliant case.
      اگر (i == j)
	بازگشت map[i].toCoord;
      // If there's exactly three, return the middle one.
      اگر (i + 2 == j)
	بازگشت map[i + 1].toCoord;

      // Ignore the middle ones. Return the one mapping closer to 0.
      اگر (value < 0) بازگشت map[j].toCoord;
      اگر (value > 0) بازگشت map[i].toCoord;

      // Mapping 0? CoreText seems confused. It seems to prefer 0 here...
      // So we'll just return the smallest one. lol
      بازگشت fabsf (map[i].toCoord) < fabsf (map[j].toCoord) ? map[i].toCoord : map[j].toCoord;

      // Mapping 0? Return one not mapping to 0.
      اگر (map[i].toCoord == 0)
	بازگشت map[j].toCoord;
      وگرنه
	بازگشت map[i].toCoord;
    }

    /* There's at least two and we're not an exact match. Prepare to lerp. */

    // Find the segment we're in.
    برای (i = start; i < end; i++)
      اگر (value < map[i].fromCoord)
	بشکن;

    اگر (i == start)
    {
      // Value before all segments; Shift.
      بازگشت value - map[start].fromCoord + map[start].toCoord;
    }
    اگر (i == end)
    {
      // Value after all segments; Shift.
      بازگشت value - map[end - 1].fromCoord + map[end - 1].toCoord;
    }

    // Actually interpolate.
    auto &before = map[i-1];
    auto &after = map[i];
    اعشاری denom = after.fromCoord - before.fromCoord; // Can't be zero by now.
    بازگشت before.toCoord + ((after.toCoord - before.toCoord) * (value - before.fromCoord)) / denom;

#undef toCoord
#undef fromCoord
  }

  اعشاری unmap_float (اعشاری value) ثابت { بازگشت map_float (value, 1, 0); }


  // TODO Kill this.
  Triple unmap_axis_range (ثابت Triple& axis_range) ثابت
  {
    اعشاری unmapped_min = unmap_float (axis_range.minimum);
    اعشاری unmapped_میانه = unmap_float (axis_range.میانه);
    اعشاری unmapped_max = unmap_float (axis_range.maximum);

    بازگشت Triple{unmapped_min, unmapped_میانه, unmapped_max};
  }

  منطقی subset (hb_subset_context_t *c, hb_tag_t axis_tag,
               hb_vector_t<Axis_مقدار_Map> *out_mappings = nullptr) ثابت
  {
    TRACE_SUBSET (این);

    /* This function cannot work on avar2 table (and currently doesn't).
     * We should instead keep the design coords in the shape plan and use
     * those. unmap_axis_range needs to be killed. */

    /* avar mapped normalized axis range. Under avar2, axes_location holds
     * only the self-contained pins for the other tables; avar itself uses
     * the intermediate-space ranges of all restricted axes. */
    ثابت auto &axes_location = c->plan->has_avar2
				? c->plan->old_intermediates
				: c->plan->axes_location;
    Triple *axis_range;
    اگر (!axes_location.has (axis_tag, &axis_range))
      بازگشت c->serializer->embed (*این);

    TripleDistances *axis_triple_distances;
    اگر (!c->plan->axes_triple_distances.has (axis_tag, &axis_triple_distances))
      return_trace (نادرست);

    auto *out = c->serializer->start_embed (این);
    اگر (unlikely (!c->serializer->extend_min (out))) return_trace (نادرست);

    Triple unmapped_range = unmap_axis_range (*axis_range);

    /* create a vector of retained mappings and sort */
    hb_vector_t<Axis_مقدار_Map> مقدار_mappings;
    برای (ثابت auto& _ : as_array ())
    {
      اگر (_.is_outside_axis_range (unmapped_range))
        ادامه;
      Axis_مقدار_Map mapping;
      mapping = _;
      mapping.instantiate (*axis_range, unmapped_range, *axis_triple_distances);
      /* (-1, -1), (0, 0), (1, 1) mappings will be added later, so avoid
       * duplicates here */
      اگر (mapping.must_include ())
        ادامه;
      مقدار_mappings.push (mapping);
    }

    Axis_مقدار_Map m;
    m.set_mapping (-1.f, -1.f);
    مقدار_mappings.push (m);

    m.set_mapping (0.f, 0.f);
    مقدار_mappings.push (m);

    m.set_mapping (1.f, 1.f);
    مقدار_mappings.push (m);

    مقدار_mappings.qsort ();

    اگر (unlikely (مقدار_mappings.in_error ()))
      return_trace (نادرست);

    برای (ثابت auto& _ : مقدار_mappings)
    {
      اگر (!_.serialize (c->serializer))
        return_trace (نادرست);
    }
    اگر (!c->serializer->check_assign (out->len, مقدار_mappings.length, HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (نادرست);
    /* Hand the instantiated mappings to the caller; avar2 offset
     * compensation needs them to locate the new mapping's kinks. */
    اگر (out_mappings)
      *out_mappings = std::move (مقدار_mappings);
    return_trace (درست);
  }

  عمومی:
  DEFINE_SIZE_ARRAY (2, *این);
};

/* One knot of the avar2 offset-compensation function
 * offset(z) = inv_renorm(z) - z, in new intermediate space. */
ساختار avar2_offset_knot_t
{
  دوگانه z;
  دوگانه offset;
};

/* Insert a knot keeping the vector sorted by z; first insertion wins on
 * duplicate z. */
ایستا درونخطی تهی
_avar2_add_knot (hb_vector_t<avar2_offset_knot_t> &knots, دوگانه z, دوگانه offset)
{
  unsigned i = 0;
  وقتی (i < knots.length && knots.arrayZ[i].z < z) i++;
  اگر (i < knots.length && knots.arrayZ[i].z == z) بازگشت;
  knots.push (avar2_offset_knot_t {0.0, 0.0});
  اگر (unlikely (knots.in_error ())) بازگشت;
  برای (unsigned j = knots.length - 1; j > i; j--)
    knots.arrayZ[j] = knots.arrayZ[j - 1];
  knots.arrayZ[i] = avar2_offset_knot_t {z, offset};
}

/* Piecewise-linear evaluation over the sorted knots. Inputs are always
 * within [-1, +1] and anchor knots at -1/0/+1 always exist. */
ایستا درونخطی دوگانه
_avar2_eval_offset (ثابت hb_vector_t<avar2_offset_knot_t> &knots, دوگانه z)
{
  unsigned len = knots.length;
  برای (unsigned i = 0; i < len; i++)
  {
    اگر (z == knots.arrayZ[i].z) بازگشت knots.arrayZ[i].offset;
    اگر (z < knots.arrayZ[i].z)
    {
      اگر (!i) بازگشت knots.arrayZ[0].offset;
      ثابت auto &before = knots.arrayZ[i - 1];
      ثابت auto &after = knots.arrayZ[i];
      دوگانه denom = after.z - before.z;
      بازگشت before.offset + (after.offset - before.offset) * (z - before.z) / denom;
    }
  }
  بازگشت len ? knots.arrayZ[len - 1].offset : 0.0;
}

/* Piecewise-linear evaluation over instantiated avar v1 mappings, as
 * produced by SegmentMaps::subset (sorted, with -1/0/+1 anchors).
 * Matches SegmentMaps::map_float for such well-formed mappings. */
ایستا درونخطی دوگانه
_avar2_map_new_mapping (ثابت hb_vector_t<Axis_مقدار_Map> &mappings, دوگانه v,
			unsigned from_offset = 0, unsigned to_offset = 1)
{
  unsigned len = mappings.length;
  اگر (!len) بازگشت v;

  برای (unsigned i = 0; i < len; i++)
  {
    دوگانه from = (دوگانه) mappings.arrayZ[i].coords[from_offset].to_float ();
    اگر (v == from)
      بازگشت (دوگانه) mappings.arrayZ[i].coords[to_offset].to_float ();
    اگر (v < from)
    {
      دوگانه to = (دوگانه) mappings.arrayZ[i].coords[to_offset].to_float ();
      اگر (!i) بازگشت v - from + to;
      دوگانه prev_from = (دوگانه) mappings.arrayZ[i - 1].coords[from_offset].to_float ();
      دوگانه prev_to = (دوگانه) mappings.arrayZ[i - 1].coords[to_offset].to_float ();
      دوگانه denom = from - prev_from;
      اگر (denom == 0.0) بازگشت prev_to;
      بازگشت prev_to + (to - prev_to) * (v - prev_from) / denom;
    }
  }
  بازگشت v - (دوگانه) mappings.arrayZ[len - 1].coords[from_offset].to_float ()
           + (دوگانه) mappings.arrayZ[len - 1].coords[to_offset].to_float ();
}

/* Inverse of _avar2_map_new_mapping: pull an output coordinate back to a
 * preimage input coordinate. For a non-strictly-monotone mapping this picks
 * one preimage, which is fine for its only use (augmenting the error
 * estimator's sample set). */
ایستا درونخطی دوگانه
_avar2_unmap_new_mapping (ثابت hb_vector_t<Axis_مقدار_Map> &mappings, دوگانه v)
{
  بازگشت _avar2_map_new_mapping (mappings, v, 1, 0);
}

/* Plain (non-avar) fvar-style normalization of a user value against a
 * user-space (min, default, max) triple. */
ایستا درونخطی دوگانه
avar2_normalize_مقدار (دوگانه v, دوگانه min, دوگانه def, دوگانه max)
{
  v = hb_clamp (v, min, max);
  اگر (v == def) بازگشت 0.0;
  اگر (v < def) بازگشت def == min ? 0.0 : (v - def) / (def - min);
  بازگشت def == max ? 0.0 : (v - def) / (max - def);
}

/* Inverse of the above: map a normalized value back to user space. */
ایستا درونخطی دوگانه
avar2_denormalize_مقدار (دوگانه v, دوگانه min, دوگانه def, دوگانه max)
{
  اگر (v == 0.0) بازگشت def;
  بازگشت v < 0.0 ? def + v * (def - min) : def + v * (max - def);
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
ایستا درونخطی unsigned
_avar2_estimate_offset_error (ثابت SegmentMaps &old_seg,
                              ثابت hb_vector_t<Axis_مقدار_Map> &new_mapping,
                              دوگانه old_min, دوگانه old_def, دوگانه old_max,
                              دوگانه new_min, دوگانه new_def, دوگانه new_max,
                              ثابت hb_vector_t<avar2_offset_knot_t> &knots)
{
  constexpr unsigned samples = 257;
  hb_vector_t<دوگانه> us;
  اگر (unlikely (!us.alloc (samples + old_seg.as_array ().length +
                           new_mapping.length + knots.length)))
    بازگشت UINT_MAX; /* estimator only; fail towards "worse" */
  برای (unsigned i = 0; i < samples; i++)
    us.push (new_min + (new_max - new_min) * i / (samples - 1));
  برای (ثابت auto &_ : old_seg.as_array ())
    us.push (avar2_denormalize_مقدار ((دوگانه) _.coords[0].to_float (),
                                       old_min, old_def, old_max));
  برای (ثابت auto &_ : new_mapping)
    us.push (avar2_denormalize_مقدار ((دوگانه) _.coords[0].to_float (),
                                       new_min, new_def, new_max));
  برای (ثابت auto &knot : knots)
    us.push (avar2_denormalize_مقدار (
               _avar2_unmap_new_mapping (new_mapping, knot.z),
               new_min, new_def, new_max));
  اگر (unlikely (us.in_error ())) بازگشت UINT_MAX;

  unsigned max_err = 0;
  برای (دوگانه u : us)
  {
    اگر (u < new_min || u > new_max) ادامه;
    دوگانه n_old = avar2_normalize_مقدار (u, old_min, old_def, old_max);
    دوگانه old_final = (دوگانه) old_seg.map_float ((اعشاری) n_old);
    دوگانه n_new = avar2_normalize_مقدار (u, new_min, new_def, new_max);
    دوگانه z = _avar2_map_new_mapping (new_mapping, n_new);
    دوگانه new_final = z + _avar2_eval_offset (knots, z);
    صحیح err = abs ((صحیح) roundf ((اعشاری) (old_final * 16384.0)) -
                   (صحیح) roundf ((اعشاری) (new_final * 16384.0)));
    اگر ((unsigned) err > max_err) max_err = err;
  }
  بازگشت max_err;
}

ساختار avar
{
  ایستا constexpr hb_tag_t tableTag = HB_OT_TAG_avar;

  منطقی has_data () ثابت { بازگشت version.to_int (); }

  ثابت SegmentMaps* get_segment_maps () ثابت
  { بازگشت &firstAxisSegmentMaps; }

  unsigned get_axis_شمار () ثابت
  { بازگشت axis_شمار_2; }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    اگر (!(version.sanitize (c) &&
	  hb_barrier () &&
	  (version.major == 1
#ifndef HB_NO_AVAR2
	   || version.major == 2
#endif
	   ) &&
	  c->check_struct (این)))
      return_trace (نادرست);

    ثابت SegmentMaps *map = &firstAxisSegmentMaps;
    unsigned صحیح شمار = axis_شمار_2;
    برای (unsigned صحیح i = 0; i < شمار; i++)
    {
      اگر (unlikely (!map->sanitize (c)))
	return_trace (نادرست);
      map = &StructAfter<SegmentMaps> (*map);
    }

#ifndef HB_NO_AVAR2
    اگر (version.major < 2)
      return_trace (درست);
    hb_barrier ();

    ثابت auto &v2 = * (ثابت avarV2Tail *) map;
    اگر (unlikely (!v2.sanitize (c, این)))
      return_trace (نادرست);
#endif

    return_trace (درست);
  }

  تهی map_coords_16_16 (صحیح *coords, unsigned صحیح coords_length) ثابت
  {
    unsigned صحیح شمار = hb_min (coords_length, axis_شمار_2);

    ثابت SegmentMaps *map = &firstAxisSegmentMaps;
    برای (unsigned صحیح i = 0; i < شمار; i++)
    {
      coords[i] = roundf (map->map_float (coords[i] / 65536.f) * 65536.f);
      map = &StructAfter<SegmentMaps> (*map);
    }

#ifndef HB_NO_AVAR2
    اگر (version.major < 2)
      بازگشت;
    hb_barrier ();

    برای (; شمار < axis_شمار_2; شمار++)
      map = &StructAfter<SegmentMaps> (*map);

    ثابت auto &v2 = * (ثابت avarV2Tail *) map;

    ثابت auto &varidx_map = این+v2.varIdxMap;
    ثابت auto &var_store = این+v2.varStore;
    auto *var_store_cache = var_store.create_cache ();

    hb_vector_t<صحیح> coords_2_14;
    coords_2_14.resize (coords_length);
    برای (unsigned i = 0; i < coords_length; i++)
      coords_2_14[i] = roundf (coords[i] / 4.f); // 16.16 -> 2.14

    hb_vector_t<صحیح> out;
    out.alloc (coords_length);
    برای (unsigned i = 0; i < coords_length; i++)
    {
      صحیح v = coords[i];
      uint32_t varidx = varidx_map.map (i);
      اعشاری delta = var_store.get_delta (varidx, coords_2_14.arrayZ, coords_2_14.length, var_store_cache);
      /* Apply the delta unclamped and clamp only the result to [-1, +1],
       * matching fontTools. Since inputs and results are in [-1, +1],
       * deltas beyond ±2 are equivalent to ±2; clamp to that range only
       * to keep the float->int conversion safe. */
      اعشاری d = hb_clamp (delta * 4, -(اعشاری) (1<<17), +(اعشاری) (1<<17)); // 2.14 -> 16.16
      v += (صحیح) roundf (d);
      v = hb_clamp (v, -(1<<16), +(1<<16));
      out.push (v);
    }
    برای (unsigned i = 0; i < coords_length; i++)
      coords[i] = out[i];

    OT::ItemVariationStore::destroy_cache (var_store_cache);
#endif
  }

  /* An avar version 2 font is never downgraded to v1: even with a NULL
   * varStore offset (legal; maps like plain v1) the avar2 subsetting path
   * applies, with "no variation" semantics for rows that don't resolve —
   * offset compensation then creates delta rows on demand. */
  منطقی has_v2_data () ثابت { بازگشت version.major > 1; }

  /* Resolve the avar2 VarStore and VarIdxMap, for the subset planner.
   * Either pointer may be to the Null object (nullable offsets). */
  منطقی get_v2_store_and_map (ثابت ItemVariationStore **store,
                             ثابت DeltaSetIndexMap **varidx_map) ثابت
  {
#ifndef HB_NO_AVAR2
    اگر (version.major < 2) بازگشت نادرست;
    ثابت SegmentMaps *map = &firstAxisSegmentMaps;
    برای (unsigned i = 0; i < axis_شمار_2; i++)
      map = &StructAfter<SegmentMaps> (*map);
    ثابت auto &v2 = * (ثابت avarV2Tail *) map;
    *store = &(این+v2.varStore);
    *varidx_map = &(این+v2.varIdxMap);
    بازگشت درست;
#وگرنه
    بازگشت نادرست;
#endif
  }

  // axis normalization is done in 2.14 here
  // TODO: deprecate this API once fonttools is updated to use 16.16 normalization
  منطقی map_coords_2_14 (اعشاری *coords, unsigned صحیح coords_length,
			منطقی v1_only = نادرست) ثابت
  {
    hb_vector_t<صحیح> coords_2_14;
    اگر (!v1_only && !coords_2_14.resize (coords_length)) بازگشت نادرست;
    unsigned صحیح شمار = hb_min (coords_length, axis_شمار_2);

    ثابت SegmentMaps *map = &firstAxisSegmentMaps;
    برای (unsigned صحیح i = 0; i < شمار; i++)
    {
      صحیح v = roundf (map->map_float (coords[i]) * 16384.f);
      اگر (!v1_only)
	coords_2_14[i] = v;
      coords[i] = v / 16384.f;
      map = &StructAfter<SegmentMaps> (*map);
    }

    اگر (v1_only)
      بازگشت درست;

#ifndef HB_NO_AVAR2
    اگر (version.major < 2)
      بازگشت درست;
    hb_barrier ();

    برای (; شمار < axis_شمار_2; شمار++)
      map = &StructAfter<SegmentMaps> (*map);

    ثابت auto &v2 = * (ثابت avarV2Tail *) map;

    ثابت auto &varidx_map = این+v2.varIdxMap;
    ثابت auto &var_store = این+v2.varStore;
    auto *var_store_cache = var_store.create_cache ();

    برای (unsigned i = 0; i < coords_length; i++)
    {
      صحیح v = coords_2_14[i];
      uint32_t varidx = varidx_map.map (i);
      اعشاری delta = var_store.get_delta (varidx, coords_2_14.arrayZ, coords_2_14.length, var_store_cache);
      /* As above: apply the delta unclamped (±2 covers every useful case)
       * and clamp the result to [-1, +1], matching fontTools. */
      v += (صحیح) hb_clamp (roundf (delta), -(اعشاری) (1<<15), +(اعشاری) (1<<15));
      v = hb_clamp (v, -(1<<14), +(1<<14));
      coords[i] = v / 16384.f;
    }

    OT::ItemVariationStore::destroy_cache (var_store_cache);
    بازگشت درست;
#وگرنه
    بازگشت version.major < 2;
#endif
  }

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (این);
    unsigned retained_axis_شمار = c->plan->axes_index_map.get_population ();
    اگر (!retained_axis_شمار) //all axes are pinned/dropped
      return_trace (نادرست);

    avar *out = c->serializer->allocate_min<avar> ();
    اگر (unlikely (!out)) return_trace (نادرست);

    out->version.major = c->plan->has_avar2 ? 2 : 1;
    out->version.minor = 0;
    اگر (!c->serializer->check_assign (out->axis_شمار_2, retained_axis_شمار, HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (نادرست);

    /* For avar2, keep the instantiated v1 mappings around; offset
     * compensation needs them to locate the new mappings' kinks. */
    hb_vector_t<hb_vector_t<Axis_مقدار_Map>> new_mappings;
    اگر (c->plan->has_avar2 && !new_mappings.resize (axis_شمار_2))
      return_trace (نادرست);

    ثابت hb_map_t& axes_index_map = c->plan->axes_index_map;
    ثابت SegmentMaps *map = &firstAxisSegmentMaps;
    unsigned شمار = axis_شمار_2;
    برای (unsigned صحیح i = 0; i < شمار; i++)
    {
      اگر (axes_index_map.has (i))
      {
        hb_tag_t *axis_tag;
        اگر (!c->plan->axes_old_index_tag_map.has (i, &axis_tag))
          return_trace (نادرست);

        Triple *axis_location;
        اگر (c->plan->has_avar2 &&
            c->plan->user_axes_location.has (*axis_tag, &axis_location) &&
            axis_location->is_point ())
        {
          /* Pinned axis in avar2 mode: serialize identity segment map
           * {-1->-1, 0->0, 1->1}. The axis is kept in fvar as hidden,
           * so avar needs a segment map entry for it. */
          auto *identity_map = c->serializer->start_embed<SegmentMaps> ();
          اگر (unlikely (!c->serializer->extend_min (identity_map)))
            return_trace (نادرست);
          Axis_مقدار_Map m;
          m.set_mapping (-1.f, -1.f);
          اگر (!m.serialize (c->serializer)) return_trace (نادرست);
          m.set_mapping (0.f, 0.f);
          اگر (!m.serialize (c->serializer)) return_trace (نادرست);
          m.set_mapping (1.f, 1.f);
          اگر (!m.serialize (c->serializer)) return_trace (نادرست);
          اگر (!c->serializer->check_assign (identity_map->len, 3u,
                                            HB_SERIALIZE_ERROR_INT_OVERFLOW))
            return_trace (نادرست);
        }
        وگرنه
        {
          /* Restricted or free axis: use standard SegmentMaps::subset() */
          اگر (!map->subset (c, *axis_tag,
                            c->plan->has_avar2 ? &new_mappings[i] : nullptr))
            return_trace (نادرست);
        }
      }
      map = &StructAfter<SegmentMaps> (*map);
    }

    اگر (c->plan->has_avar2)
      return_trace (_subset_avar2 (c, new_mappings));

    return_trace (درست);
  }

  خصوصی:
  ساختار avar2_index_map_plan_t
  {
    منطقی init (ثابت hb_vector_t<uint32_t> &varidx_mapping,
	       ثابت hb_map_t &axes_index_map,
	       unsigned axis_شمار)
    {
      اگر (!output_map.alloc (axes_index_map.get_population ()))
	بازگشت نادرست;

      منطقی has_no_variation = نادرست;
      unsigned max_outer = 0, max_inner = 0;
      برای (unsigned i = 0; i < axis_شمار; i++)
      {
	اگر (!axes_index_map.has (i)) ادامه;
	uint32_t varidx = varidx_mapping[i];
	output_map.push (varidx);
	اگر (varidx == HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
	{
	  has_no_variation = درست;
	  ادامه;
	}
	max_outer = hb_max (max_outer, varidx >> 16);
	max_inner = hb_max (max_inner, varidx & 0xFFFF);
      }
      اگر (output_map.in_error () ||
	  output_map.length != axes_index_map.get_population ())
	بازگشت نادرست;

      اگر (has_no_variation)
      {
	width = 4;
	inner_bit_شمار = 16;
      }
      وگرنه
      {
	inner_bit_شمار = hb_max (1u, hb_bit_storage (max_inner));
	unsigned outer_bit_شمار = hb_max (1u, hb_bit_storage (max_outer));
	width = hb_clamp ((inner_bit_شمار + outer_bit_شمار + 7) / 8, 1u, 4u);
	اگر (inner_bit_شمار + outer_bit_شمار > width * 8)
	  inner_bit_شمار = width * 8 - outer_bit_شمار;
      }
      بازگشت درست;
    }

    unsigned get_inner_bit_شمار () ثابت { بازگشت inner_bit_شمار; }
    unsigned get_width () ثابت { بازگشت width; }
    hb_array_t<ثابت uint32_t> get_output_map () ثابت { بازگشت output_map.as_array (); }

    unsigned inner_bit_شمار = 1;
    unsigned width = 1;
    hb_vector_t<uint32_t> output_map;
  };

  منطقی _subset_avar2 (hb_subset_context_t *c,
                      ثابت hb_vector_t<hb_vector_t<Axis_مقدار_Map>> &new_mappings) ثابت
  {
#اگر defined (HB_NO_VAR) || defined (HB_NO_AVAR2)
    /* Not reachable: the plan never sets has_avar2 in these configurations. */
    بازگشت نادرست;
#وگرنه

    /* 1. Locate original avar2 data, keeping per-axis old segment maps */
    hb_vector_t<ثابت SegmentMaps *> old_seg_maps;
    اگر (!old_seg_maps.alloc (axis_شمار_2)) بازگشت نادرست;
    ثابت SegmentMaps *map = &firstAxisSegmentMaps;
    برای (unsigned i = 0; i < axis_شمار_2; i++)
    {
      old_seg_maps.push (map);
      map = &StructAfter<SegmentMaps> (*map);
    }

    ثابت auto &v2 = * (ثابت avarV2Tail *) map;
    ثابت auto &varidx_map = این+v2.varIdxMap;
    ثابت auto &var_store = این+v2.varStore;

    auto fvar_axes = c->plan->source->table.fvar->get_axes ();

    /* 2. Compute default deltas by evaluating VarStore at old defaults */
    hb_vector_t<صحیح> default_coords;
    اگر (!default_coords.resize (axis_شمار_2)) بازگشت نادرست;
    برای (unsigned i = 0; i < axis_شمار_2; i++)
    {
      hb_tag_t *axis_tag;
      اگر (c->plan->axes_old_index_tag_map.has (i, &axis_tag) &&
          c->plan->old_intermediates.has (*axis_tag))
      {
        اعشاری d_i = (اعشاری) c->plan->old_intermediates.get (*axis_tag).میانه;
        default_coords[i] = roundf (d_i * 16384.f);
      }
      وگرنه
        default_coords[i] = 0;
    }

    auto *store_cache = var_store.create_cache ();
    hb_vector_t<اعشاری> default_deltas;
    اگر (!default_deltas.resize (axis_شمار_2)) {
      ItemVariationStore::destroy_cache (store_cache);
      بازگشت نادرست;
    }
    برای (unsigned i = 0; i < axis_شمار_2; i++)
    {
      uint32_t varidx = varidx_map.map (i);
      اگر (varidx == HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
        default_deltas[i] = 0.f;
      وگرنه
        default_deltas[i] = var_store.get_delta (varidx, default_coords.arrayZ,
                                                  default_coords.length, store_cache);
    }
    ItemVariationStore::destroy_cache (store_cache);

    /* 3. Rebase IVS regions */
    item_variations_t item_vars;
    اگر (!item_vars.create_from_item_varstore (var_store, c->plan->axes_old_index_tag_map))
      بازگشت نادرست;
    اگر (!item_vars.instantiate_tuple_vars (c->plan->old_intermediates,
                                           c->plan->axes_triple_distances,
                                           نادرست))
      بازگشت نادرست;

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
    hb_vector_t<uint32_t> new_varidx_mapping;
    اگر (!new_varidx_mapping.resize (axis_شمار_2)) بازگشت نادرست;
    برای (unsigned i = 0; i < axis_شمار_2; i++)
    {
      uint32_t varidx = varidx_map.map (i);
      اگر (varidx != HB_OT_LAYOUT_NO_VARIATIONS_INDEX &&
	  !var_store.has_delta_set (varidx))
	varidx = HB_OT_LAYOUT_NO_VARIATIONS_INDEX;
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
    hb_hashmap_t<uint32_t, unsigned> varidx_ref_شمار;
    برای (unsigned i = 0; i < axis_شمار_2; i++)
    {
      اگر (!c->plan->axes_index_map.has (i)) ادامه; /* self-contained: dropped */
      uint32_t varidx = new_varidx_mapping[i];
      اگر (varidx == HB_OT_LAYOUT_NO_VARIATIONS_INDEX) ادامه;
      unsigned *شمار;
      اگر (varidx_ref_شمار.has (varidx, &شمار))
        (*شمار)++;
      وگرنه اگر (!varidx_ref_شمار.set (varidx, 1))
        بازگشت نادرست;
    }
    برای (unsigned i = 0; i < axis_شمار_2; i++)
    {
      hb_tag_t *axis_tag_ptr;
      اگر (!c->plan->axes_old_index_tag_map.has (i, &axis_tag_ptr))
        بازگشت نادرست;
      /* Only axes that will receive offset compensation (restricted or
       * pinned) can contaminate a shared row. */
      اگر (!c->plan->axes_index_map.has (i) ||
          !c->plan->user_axes_location.has (*axis_tag_ptr))
        ادامه;
      uint32_t varidx = new_varidx_mapping[i];
      اگر (varidx == HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
        ادامه; /* gets a fresh, private VarData in the offset loop below */
      unsigned *شمار;
      اگر (!varidx_ref_شمار.has (varidx, &شمار) || *شمار <= 1)
        ادامه; /* sole owner: safe to write offsets in place */
      unsigned outer = varidx >> 16;
      unsigned new_inner = item_vars.duplicate_row (outer, varidx & 0xFFFF);
      اگر (unlikely (new_inner == (unsigned) -1)) بازگشت نادرست;
      new_varidx_mapping[i] = (outer << 16) | new_inner;
      (*شمار)--;
    }

    /* 6. Add offset compensation tuples.
     * Track processed (outer,inner) pairs to avoid adding duplicate biases
     * when multiple axes share the same varIdx. */
    hb_set_t processed_varidxes;
    برای (unsigned i = 0; i < axis_شمار_2; i++)
    {
      hb_tag_t *axis_tag_ptr;
      اگر (!c->plan->axes_old_index_tag_map.has (i, &axis_tag_ptr))
        بازگشت نادرست;
      hb_tag_t axis_tag = *axis_tag_ptr;

      /* Self-contained pinned axes are removed from fvar/avar; their
       * contribution is baked into the variation tables instead. */
      اگر (!c->plan->axes_index_map.has (i))
        ادامه;

      Triple *new_user;
      اگر (c->plan->user_axes_location.has (axis_tag, &new_user))
      {
        /* This axis is being restricted or pinned */
        Triple *old_int;
        اگر (!c->plan->old_intermediates.has (axis_tag, &old_int))
          بازگشت نادرست;

        اعشاری a_i = (اعشاری) old_int->minimum;
        اعشاری d_i = (اعشاری) old_int->میانه;
        اعشاری b_i = (اعشاری) old_int->maximum;

        صحیح d_int = roundf (d_i * 16384.f);

        منطقی is_pinned = new_user->is_point ();

        uint32_t varidx = new_varidx_mapping[i];
        unsigned outer, inner, item_شمار_2;

        اگر (varidx == HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
        {
          /* No existing avar2 mapping. Create new VarData. */
          outer = item_vars.add_vardata (1);
          inner = 0;
          item_شمار_2 = 1;
          new_varidx_mapping[i] = (outer << 16) | inner;
          default_deltas[i] = 0.f; /* no prior default delta */
        }
        وگرنه
        {
          outer = varidx >> 16;
          inner = varidx & 0xFFFF;
          item_شمار_2 = item_vars.get_item_شمار (outer);
        }

        /* Empty-region bias: d_int + round(defaultDelta) */
        صحیح bias = d_int + (صحیح) roundf (default_deltas[i]);
        اگر (bias != 0)
        {
          hb_hashmap_t<hb_tag_t, Triple> empty_region;
          item_vars.add_tuple (outer, std::move (empty_region),
                               inner, bias, item_شمار_2);
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
          hb_vector_t<avar2_offset_knot_t> knots;
          _avar2_add_knot (knots, -1.0, (دوگانه) a_i + 1.0);
          _avar2_add_knot (knots, 0.0, (دوگانه) d_i);
          _avar2_add_knot (knots, 1.0, (دوگانه) b_i - 1.0);

          ثابت hb_vector_t<Axis_مقدار_Map> &new_mapping = new_mappings[i];

          اعشاری min_f = 0.f, def_f = 0.f, max_f = 0.f;
          اگر (likely (i < fvar_axes.length))
            fvar_axes[i].get_coordinates (min_f, def_f, max_f);
          دوگانه old_min = (دوگانه) min_f;
          دوگانه old_def = (دوگانه) def_f;
          دوگانه old_max = (دوگانه) max_f;

          /* If the axis default MOVED, inv_renorm also kinks where the OLD
           * default lands in the new space (the old intermediate coordinate
           * crosses 0 there), at
           *   z = z_old  ->  -z_old
           * Omitting that knot (as a plain two-tent encoding would) makes
           * interior coordinates wrong. */
          دوگانه z_old = avar2_normalize_مقدار ((دوگانه) old_def,
                                                 (دوگانه) new_user->minimum,
                                                 (دوگانه) new_user->میانه,
                                                 (دوگانه) new_user->maximum);
          z_old = _avar2_map_new_mapping (new_mapping, z_old);
          z_old = (دوگانه) roundf ((اعشاری) (z_old * 16384.0)) / 16384.0;
          اگر (-1.0 < z_old && z_old < 1.0)
            _avar2_add_knot (knots, z_old, -z_old);

          /* Interior avar v1 breakpoints inside the retained range each put
           * a kink in offset(z). Sampling only {-1, 0, +1, z_old} would
           * linearly interpolate across those kinks. The instantiated
           * mapping keeps exactly the in-range old breakpoints, and the new
           * mapping kinks at each one's output coordinate; add that z with
           * its old intermediate value so offset(z) is reproduced at every
           * kink. */
          hb_vector_t<avar2_offset_knot_t> with_breakpoints (knots);
          برای (ثابت auto &m : new_mapping)
          {
            دوگانه from = (دوگانه) m.coords[0].to_float ();
            اگر (from == -1.0 || from == 0.0 || from == 1.0)
              ادامه; /* anchors already seeded */
            دوگانه z = (دوگانه) m.coords[1].to_float ();
            اگر (!(-1.0 < z && z < 1.0))
              ادامه;
            دوگانه user = avar2_denormalize_مقدار (from,
                                                    (دوگانه) new_user->minimum,
                                                    (دوگانه) new_user->میانه,
                                                    (دوگانه) new_user->maximum);
            دوگانه n_old = avar2_normalize_مقدار (user, old_min, old_def, old_max);
            دوگانه x_old = (دوگانه) old_seg_maps[i]->map_float ((اعشاری) n_old);
            x_old = (دوگانه) roundf ((اعشاری) (x_old * 16384.0)) / 16384.0;
            _avar2_add_knot (with_breakpoints, z, x_old - z);
          }

          اگر (unlikely (knots.in_error () || with_breakpoints.in_error ()))
            بازگشت نادرست;

          /* Extra tents cost F2Dot14 rounding, so for a steep segment they
           * can add more quantization noise than the structural error they
           * remove. Keep the interior breakpoints only when they do not
           * increase the estimated residual; this makes the collection a
           * strict (never-worse) improvement over the {-1, 0, +1, z_old}
           * anchors. Warn when even the better choice is not bit-exact (a
           * steep retained segment that cannot be reproduced in F2Dot14). */
          unsigned err = _avar2_estimate_offset_error (*old_seg_maps[i], new_mapping,
                                                       old_min, old_def, old_max,
                                                       (دوگانه) new_user->minimum,
                                                       (دوگانه) new_user->میانه,
                                                       (دوگانه) new_user->maximum,
                                                       knots);
          اگر (with_breakpoints.length != knots.length)
          {
            unsigned err_with = _avar2_estimate_offset_error (*old_seg_maps[i], new_mapping,
                                                              old_min, old_def, old_max,
                                                              (دوگانه) new_user->minimum,
                                                              (دوگانه) new_user->میانه,
                                                              (دوگانه) new_user->maximum,
                                                              with_breakpoints);
            اگر (err_with <= err)
            {
              knots = std::move (with_breakpoints);
              err = err_with;
            }
          }
          اگر (err > 8)
            DEBUG_پیام (SUBSET, nullptr,
                       "avar2 offset compensation is approximate for axis %c%c%c%c: "
                       "max residual %u F2Dot14 units",
                       HB_UNTAG (axis_tag), err);

          /* Synthesize tents. Adjacent tents evaluate to zero at each
           * other's peaks, so each knot's delta is offset(z) - offset(0);
           * the base value offset(0) = d_i is carried by the empty-region
           * bias above. This reduces to the classic pair of tents
           * (-1,-1,0) / (0,+1,+1) when the default is unchanged and there
           * are no interior knots. */
          برای (unsigned k = 0; k < knots.length; k++)
          {
            دوگانه z = knots.arrayZ[k].z;
            اگر (z == 0.0) ادامه;
            صحیح delta = (صحیح) roundf ((اعشاری) ((knots.arrayZ[k].offset - (دوگانه) d_i) * 16384.0));
            اگر (!delta) ادامه;
            دوگانه lower, upper;
            اگر (z > 0.0)
            {
              lower = knots.arrayZ[k - 1].z;
              upper = k + 1 < knots.length ? knots.arrayZ[k + 1].z : z;
            }
            وگرنه
            {
              upper = knots.arrayZ[k + 1].z;
              lower = k > 0 ? knots.arrayZ[k - 1].z : z;
            }
            hb_hashmap_t<hb_tag_t, Triple> region;
            اگر (unlikely (!region.set (axis_tag, Triple (lower, z, upper))))
              بازگشت نادرست;
            item_vars.add_tuple (outer, std::move (region),
                                 inner, delta, item_شمار_2);
          }
        }
      }
      وگرنه
      {
        /* Free or private axis — not being restricted.
         * If it has a non-zero default delta, add it back as a bias.
         * Skip if this (outer,inner) was already processed (shared varIdx). */
        uint32_t varidx = new_varidx_mapping[i];
        اگر (varidx == HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
          ادامه;

        اگر (processed_varidxes.has (varidx))
          ادامه;
        processed_varidxes.add (varidx);

        unsigned outer = varidx >> 16;
        unsigned inner = varidx & 0xFFFF;
        صحیح dd = (صحیح) roundf (default_deltas[i]);
        اگر (dd != 0)
        {
          unsigned item_شمار_2 = item_vars.get_item_شمار (outer);
          hb_hashmap_t<hb_tag_t, Triple> empty_region;
          item_vars.add_tuple (outer, std::move (empty_region),
                               inner, dd, item_شمار_2);
        }
      }
    }

    /* 7. Finalize: build region list + convert to varstore */
    اگر (!item_vars.build_region_list ()) بازگشت نادرست;
    اگر (!item_vars.as_item_varstore (درست /* optimize */,
                                     نادرست /* use_no_variation_idx */))
      بازگشت نادرست;

    /* 8. Apply varidx_map optimization remapping */
    ثابت auto &opt_varidx_map = item_vars.get_varidx_map ();
    برای (unsigned i = 0; i < axis_شمار_2; i++)
    {
      uint32_t varidx = new_varidx_mapping[i];
      اگر (varidx == HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
        ادامه;
      uint32_t *new_idx;
      اگر (opt_varidx_map.has (varidx, &new_idx))
        new_varidx_mapping[i] = *new_idx;
    }

    /* 9. Serialize avarV2Tail. Entries cover the retained axes only;
     * self-contained pinned axes are removed from fvar. */
    avar2_index_map_plan_t index_map_plan;
    اگر (!index_map_plan.init (new_varidx_mapping,
			      c->plan->axes_index_map,
			      axis_شمار_2))
      بازگشت نادرست;

    auto *tail = c->serializer->allocate_size<avarV2Tail> (avarV2Tail::static_size);
    اگر (unlikely (!tail)) بازگشت نادرست;
    اگر (!tail->varIdxMap.serialize_serialize (c->serializer, index_map_plan))
      بازگشت نادرست;
    اگر (!tail->varStore.serialize_serialize (c->serializer,
					     item_vars.has_long_word (),
					     c->plan->axis_tags,
					     item_vars.get_region_list (),
					     item_vars.get_vardata_encodings ()))
      بازگشت نادرست;

    بازگشت درست;
#endif
  }

  عمومی:

  محافظتشده:
  FixedVersion<>version;	/* Version of the avar table
				 * initially set to 0x00010000u */
  HBUINT16	reserved;	/* This field is permanently reserved. Set to 0. */
  HBUINT16	axis_شمار_2;	/* The number of variation axes in the font. This
				 * must be the same number as axisCount in the
				 * 'fvar' table. */
  SegmentMaps	firstAxisSegmentMaps;

  عمومی:
  DEFINE_SIZE_MIN (8);
};

} /* namespace OT */


#endif /* HB_OT_VAR_AVAR_TABLE_HH */
