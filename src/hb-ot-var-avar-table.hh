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

#அந்தர்பாவ "hb-open-type.hh"
#அந்தர்பாவ "hb-ot-var-common.hh"
#அந்தர்பாவ "hb-ot-var-fvar-table.hh"


/*
 * avar -- Axis Variations
 * https://docs.microsoft.com/en-us/typography/opentype/spec/avar
 */

#define HB_OT_TAG_avar HB_TAG('a','v','a','r')


நாமாகாஶ OT {


/* "Spec": https://github.com/be-fonts/boring-expansion-spec/issues/14 */
கட்டமைப்பு avarV2Tail
{
  friend கட்டமைப்பு avar;

  தருக்க sanitize (hb_sanitize_context_t *c,
		 மாறிலி வெறுமை *base) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (varIdxMap.sanitize (c, base) &&
		  varStore.sanitize (c, base));
  }

  பாதுகாக்கப்பட்ட:
  Offset32To<DeltaSetIndexMap>	varIdxMap;	/* Offset from the beginning of 'avar' table. */
  Offset32To<ItemVariationStore>	varStore;	/* Offset from the beginning of 'avar' table. */

  பொது:
  DEFINE_SIZE_STATIC (8);
};


கட்டமைப்பு Axis_மதிப்பு_Map
{
  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது));
  }

  வெறுமை set_mapping (தசம from_coord, தசம to_coord)
  {
    coords[0].set_float (from_coord);
    coords[1].set_float (to_coord);
  }

  தருக்க is_outside_axis_range (மாறிலி Triple& axis_range) மாறிலி
  {
    இரட்டை from_coord = (இரட்டை) coords[0].to_float ();
    திரும்பு !axis_range.contains (from_coord);
  }

  தருக்க must_include () மாறிலி
  {
    தசம from_coord = coords[0].to_float ();
    தசம to_coord = coords[1].to_float ();
    திரும்பு (from_coord == -1.f && to_coord == -1.f) ||
           (from_coord == 0.f && to_coord == 0.f) ||
           (from_coord == 1.f && to_coord == 1.f);
  }

  வெறுமை instantiate (மாறிலி Triple& axis_range,
                    மாறிலி Triple& unmapped_range,
                    மாறிலி TripleDistances& triple_distances)
  {
    தசம from_coord = coords[0].to_float ();
    தசம to_coord = coords[1].to_float ();

    from_coord = renormalize_மதிப்பு ((இரட்டை) from_coord, unmapped_range, triple_distances);
    to_coord = renormalize_மதிப்பு ((இரட்டை) to_coord, axis_range, triple_distances);

    /* Malformed input can renormalize outside [-1,1], which would
     * overflow F2DOT14. */
    from_coord = hb_clamp (from_coord, -1.f, 1.f);
    to_coord = hb_clamp (to_coord, -1.f, 1.f);

    coords[0].set_float (from_coord);
    coords[1].set_float (to_coord);
  }

  HB_INTERNAL நிலையான முழு cmp (மாறிலி வெறுமை *pa, மாறிலி வெறுமை *pb)
  {
    மாறிலி Axis_மதிப்பு_Map *a = (மாறிலி Axis_மதிப்பு_Map *) pa;
    மாறிலி Axis_மதிப்பு_Map *b = (மாறிலி Axis_மதிப்பு_Map *) pb;

    முழு a_from = a->coords[0].to_int ();
    முழு b_from = b->coords[0].to_int ();
    எனில் (a_from != b_from)
      திரும்பு a_from - b_from;

    /* this should never be reached. according to the spec, all of the axis
     * value map records for a given axis must have different fromCoord values
     * */
    முழு a_to = a->coords[1].to_int ();
    முழு b_to = b->coords[1].to_int ();
    திரும்பு a_to - b_to;
  }

  தருக்க serialize (hb_serialize_context_t *c) மாறிலி
  {
    TRACE_SERIALIZE (இது);
    return_trace (c->embed (இது));
  }

  பொது:
  F2DOT14	coords[2];
//   F2DOT14	fromCoord;	/* A normalized coordinate value obtained using
//				 * default normalization. */
//   F2DOT14	toCoord;	/* The modified, normalized coordinate value. */

  பொது:
  DEFINE_SIZE_STATIC (4);
};

கட்டமைப்பு SegmentMaps : Array16Of<Axis_மதிப்பு_Map>
{
  தசம map_float (தசம value, unsigned முழு from_offset = 0, unsigned முழு to_offset = 1) மாறிலி
  {
#define fromCoord coords[from_offset].to_float ()
#define toCoord coords[to_offset].to_float ()

    மாறிலி auto *map = arrayZ;

    /* The following special-cases are not part of OpenType, which requires
     * that at least -1, 0, and +1 must be mapped. But we include these as
     * part of a better error recovery scheme. */
    எனில் (len < 2)
    {
      எனில் (!len)
	திரும்பு value;
      இல்லையெனில் /* len == 1*/
	திரும்பு value - map[0].fromCoord + map[0].toCoord;
    }

    // At least two mappings now.

    /* CoreText is wild...
     * PingFangUI avar needs all this special-casing...
     * So we implement an extended version of the spec here,
     * which is more robust and more likely to be compatible with
     * the wild. */

    unsigned start = 0;
    unsigned end = len;
    எனில் (map[start].fromCoord == -1 && map[start].toCoord == -1 && map[start+1].fromCoord == -1)
      start++;
    எனில் (map[end-1].fromCoord == +1 && map[end-1].toCoord == +1 && map[end-2].fromCoord == +1)
      end--;

    /* Look for exact match first, and do lots of special-casing. */
    unsigned i;
    க்கு (i = start; i < end; i++)
      எனில் (value == map[i].fromCoord)
	நிறுத்து;
    எனில் (i < end)
    {
      // There's at least one exact match. See if there are more.
      unsigned j = i;
      க்கு (; j + 1 < end; j++)
	எனில் (value != map[j + 1].fromCoord)
	  நிறுத்து;

      // [i,j] inclusive are all exact matches:

      // If there's only one, return it. This is the only spec-compliant case.
      எனில் (i == j)
	திரும்பு map[i].toCoord;
      // If there's exactly three, return the middle one.
      எனில் (i + 2 == j)
	திரும்பு map[i + 1].toCoord;

      // Ignore the middle ones. Return the one mapping closer to 0.
      எனில் (value < 0) திரும்பு map[j].toCoord;
      எனில் (value > 0) திரும்பு map[i].toCoord;

      // Mapping 0? CoreText seems confused. It seems to prefer 0 here...
      // So we'll just return the smallest one. lol
      திரும்பு fabsf (map[i].toCoord) < fabsf (map[j].toCoord) ? map[i].toCoord : map[j].toCoord;

      // Mapping 0? Return one not mapping to 0.
      எனில் (map[i].toCoord == 0)
	திரும்பு map[j].toCoord;
      இல்லையெனில்
	திரும்பு map[i].toCoord;
    }

    /* There's at least two and we're not an exact match. Prepare to lerp. */

    // Find the segment we're in.
    க்கு (i = start; i < end; i++)
      எனில் (value < map[i].fromCoord)
	நிறுத்து;

    எனில் (i == start)
    {
      // Value before all segments; Shift.
      திரும்பு value - map[start].fromCoord + map[start].toCoord;
    }
    எனில் (i == end)
    {
      // Value after all segments; Shift.
      திரும்பு value - map[end - 1].fromCoord + map[end - 1].toCoord;
    }

    // Actually interpolate.
    auto &before = map[i-1];
    auto &after = map[i];
    தசம denom = after.fromCoord - before.fromCoord; // Can't be zero by now.
    திரும்பு before.toCoord + ((after.toCoord - before.toCoord) * (value - before.fromCoord)) / denom;

#undef toCoord
#undef fromCoord
  }

  தசம unmap_float (தசம value) மாறிலி { திரும்பு map_float (value, 1, 0); }


  // TODO Kill this.
  Triple unmap_axis_range (மாறிலி Triple& axis_range) மாறிலி
  {
    தசம unmapped_min = unmap_float (axis_range.minimum);
    தசம unmapped_நடு = unmap_float (axis_range.நடு);
    தசம unmapped_max = unmap_float (axis_range.maximum);

    திரும்பு Triple{unmapped_min, unmapped_நடு, unmapped_max};
  }

  தருக்க subset (hb_subset_context_t *c, hb_tag_t axis_tag,
               hb_vector_t<Axis_மதிப்பு_Map> *out_mappings = nullptr) மாறிலி
  {
    TRACE_SUBSET (இது);

    /* This function cannot work on avar2 table (and currently doesn't).
     * We should instead keep the design coords in the shape plan and use
     * those. unmap_axis_range needs to be killed. */

    /* avar mapped normalized axis range. Under avar2, axes_location holds
     * only the self-contained pins for the other tables; avar itself uses
     * the intermediate-space ranges of all restricted axes. */
    மாறிலி auto &axes_location = c->plan->has_avar2
				? c->plan->old_intermediates
				: c->plan->axes_location;
    Triple *axis_range;
    எனில் (!axes_location.has (axis_tag, &axis_range))
      திரும்பு c->serializer->embed (*இது);

    TripleDistances *axis_triple_distances;
    எனில் (!c->plan->axes_triple_distances.has (axis_tag, &axis_triple_distances))
      return_trace (பொய்);

    auto *out = c->serializer->start_embed (இது);
    எனில் (unlikely (!c->serializer->extend_min (out))) return_trace (பொய்);

    Triple unmapped_range = unmap_axis_range (*axis_range);

    /* create a vector of retained mappings and sort */
    hb_vector_t<Axis_மதிப்பு_Map> மதிப்பு_mappings;
    க்கு (மாறிலி auto& _ : as_array ())
    {
      எனில் (_.is_outside_axis_range (unmapped_range))
        தொடர்;
      Axis_மதிப்பு_Map mapping;
      mapping = _;
      mapping.instantiate (*axis_range, unmapped_range, *axis_triple_distances);
      /* (-1, -1), (0, 0), (1, 1) mappings will be added later, so avoid
       * duplicates here */
      எனில் (mapping.must_include ())
        தொடர்;
      மதிப்பு_mappings.push (mapping);
    }

    Axis_மதிப்பு_Map m;
    m.set_mapping (-1.f, -1.f);
    மதிப்பு_mappings.push (m);

    m.set_mapping (0.f, 0.f);
    மதிப்பு_mappings.push (m);

    m.set_mapping (1.f, 1.f);
    மதிப்பு_mappings.push (m);

    மதிப்பு_mappings.qsort ();

    எனில் (unlikely (மதிப்பு_mappings.in_error ()))
      return_trace (பொய்);

    க்கு (மாறிலி auto& _ : மதிப்பு_mappings)
    {
      எனில் (!_.serialize (c->serializer))
        return_trace (பொய்);
    }
    எனில் (!c->serializer->check_assign (out->len, மதிப்பு_mappings.length, HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (பொய்);
    /* Hand the instantiated mappings to the caller; avar2 offset
     * compensation needs them to locate the new mapping's kinks. */
    எனில் (out_mappings)
      *out_mappings = std::move (மதிப்பு_mappings);
    return_trace (உண்மை);
  }

  பொது:
  DEFINE_SIZE_ARRAY (2, *இது);
};

/* One knot of the avar2 offset-compensation function
 * offset(z) = inv_renorm(z) - z, in new intermediate space. */
கட்டமைப்பு avar2_offset_knot_t
{
  இரட்டை z;
  இரட்டை offset;
};

/* Insert a knot keeping the vector sorted by z; first insertion wins on
 * duplicate z. */
நிலையான உள்வரி வெறுமை
_avar2_add_knot (hb_vector_t<avar2_offset_knot_t> &knots, இரட்டை z, இரட்டை offset)
{
  unsigned i = 0;
  போது (i < knots.length && knots.arrayZ[i].z < z) i++;
  எனில் (i < knots.length && knots.arrayZ[i].z == z) திரும்பு;
  knots.push (avar2_offset_knot_t {0.0, 0.0});
  எனில் (unlikely (knots.in_error ())) திரும்பு;
  க்கு (unsigned j = knots.length - 1; j > i; j--)
    knots.arrayZ[j] = knots.arrayZ[j - 1];
  knots.arrayZ[i] = avar2_offset_knot_t {z, offset};
}

/* Piecewise-linear evaluation over the sorted knots. Inputs are always
 * within [-1, +1] and anchor knots at -1/0/+1 always exist. */
நிலையான உள்வரி இரட்டை
_avar2_eval_offset (மாறிலி hb_vector_t<avar2_offset_knot_t> &knots, இரட்டை z)
{
  unsigned len = knots.length;
  க்கு (unsigned i = 0; i < len; i++)
  {
    எனில் (z == knots.arrayZ[i].z) திரும்பு knots.arrayZ[i].offset;
    எனில் (z < knots.arrayZ[i].z)
    {
      எனில் (!i) திரும்பு knots.arrayZ[0].offset;
      மாறிலி auto &before = knots.arrayZ[i - 1];
      மாறிலி auto &after = knots.arrayZ[i];
      இரட்டை denom = after.z - before.z;
      திரும்பு before.offset + (after.offset - before.offset) * (z - before.z) / denom;
    }
  }
  திரும்பு len ? knots.arrayZ[len - 1].offset : 0.0;
}

/* Piecewise-linear evaluation over instantiated avar v1 mappings, as
 * produced by SegmentMaps::subset (sorted, with -1/0/+1 anchors).
 * Matches SegmentMaps::map_float for such well-formed mappings. */
நிலையான உள்வரி இரட்டை
_avar2_map_new_mapping (மாறிலி hb_vector_t<Axis_மதிப்பு_Map> &mappings, இரட்டை v,
			unsigned from_offset = 0, unsigned to_offset = 1)
{
  unsigned len = mappings.length;
  எனில் (!len) திரும்பு v;

  க்கு (unsigned i = 0; i < len; i++)
  {
    இரட்டை from = (இரட்டை) mappings.arrayZ[i].coords[from_offset].to_float ();
    எனில் (v == from)
      திரும்பு (இரட்டை) mappings.arrayZ[i].coords[to_offset].to_float ();
    எனில் (v < from)
    {
      இரட்டை to = (இரட்டை) mappings.arrayZ[i].coords[to_offset].to_float ();
      எனில் (!i) திரும்பு v - from + to;
      இரட்டை prev_from = (இரட்டை) mappings.arrayZ[i - 1].coords[from_offset].to_float ();
      இரட்டை prev_to = (இரட்டை) mappings.arrayZ[i - 1].coords[to_offset].to_float ();
      இரட்டை denom = from - prev_from;
      எனில் (denom == 0.0) திரும்பு prev_to;
      திரும்பு prev_to + (to - prev_to) * (v - prev_from) / denom;
    }
  }
  திரும்பு v - (இரட்டை) mappings.arrayZ[len - 1].coords[from_offset].to_float ()
           + (இரட்டை) mappings.arrayZ[len - 1].coords[to_offset].to_float ();
}

/* Inverse of _avar2_map_new_mapping: pull an output coordinate back to a
 * preimage input coordinate. For a non-strictly-monotone mapping this picks
 * one preimage, which is fine for its only use (augmenting the error
 * estimator's sample set). */
நிலையான உள்வரி இரட்டை
_avar2_unmap_new_mapping (மாறிலி hb_vector_t<Axis_மதிப்பு_Map> &mappings, இரட்டை v)
{
  திரும்பு _avar2_map_new_mapping (mappings, v, 1, 0);
}

/* Plain (non-avar) fvar-style normalization of a user value against a
 * user-space (min, default, max) triple. */
நிலையான உள்வரி இரட்டை
avar2_normalize_மதிப்பு (இரட்டை v, இரட்டை min, இரட்டை def, இரட்டை max)
{
  v = hb_clamp (v, min, max);
  எனில் (v == def) திரும்பு 0.0;
  எனில் (v < def) திரும்பு def == min ? 0.0 : (v - def) / (def - min);
  திரும்பு def == max ? 0.0 : (v - def) / (max - def);
}

/* Inverse of the above: map a normalized value back to user space. */
நிலையான உள்வரி இரட்டை
avar2_denormalize_மதிப்பு (இரட்டை v, இரட்டை min, இரட்டை def, இரட்டை max)
{
  எனில் (v == 0.0) திரும்பு def;
  திரும்பு v < 0.0 ? def + v * (def - min) : def + v * (max - def);
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
நிலையான உள்வரி unsigned
_avar2_estimate_offset_error (மாறிலி SegmentMaps &old_seg,
                              மாறிலி hb_vector_t<Axis_மதிப்பு_Map> &new_mapping,
                              இரட்டை old_min, இரட்டை old_def, இரட்டை old_max,
                              இரட்டை new_min, இரட்டை new_def, இரட்டை new_max,
                              மாறிலி hb_vector_t<avar2_offset_knot_t> &knots)
{
  constexpr unsigned samples = 257;
  hb_vector_t<இரட்டை> us;
  எனில் (unlikely (!us.alloc (samples + old_seg.as_array ().length +
                           new_mapping.length + knots.length)))
    திரும்பு UINT_MAX; /* estimator only; fail towards "worse" */
  க்கு (unsigned i = 0; i < samples; i++)
    us.push (new_min + (new_max - new_min) * i / (samples - 1));
  க்கு (மாறிலி auto &_ : old_seg.as_array ())
    us.push (avar2_denormalize_மதிப்பு ((இரட்டை) _.coords[0].to_float (),
                                       old_min, old_def, old_max));
  க்கு (மாறிலி auto &_ : new_mapping)
    us.push (avar2_denormalize_மதிப்பு ((இரட்டை) _.coords[0].to_float (),
                                       new_min, new_def, new_max));
  க்கு (மாறிலி auto &knot : knots)
    us.push (avar2_denormalize_மதிப்பு (
               _avar2_unmap_new_mapping (new_mapping, knot.z),
               new_min, new_def, new_max));
  எனில் (unlikely (us.in_error ())) திரும்பு UINT_MAX;

  unsigned max_err = 0;
  க்கு (இரட்டை u : us)
  {
    எனில் (u < new_min || u > new_max) தொடர்;
    இரட்டை n_old = avar2_normalize_மதிப்பு (u, old_min, old_def, old_max);
    இரட்டை old_final = (இரட்டை) old_seg.map_float ((தசம) n_old);
    இரட்டை n_new = avar2_normalize_மதிப்பு (u, new_min, new_def, new_max);
    இரட்டை z = _avar2_map_new_mapping (new_mapping, n_new);
    இரட்டை new_final = z + _avar2_eval_offset (knots, z);
    முழு err = abs ((முழு) roundf ((தசம) (old_final * 16384.0)) -
                   (முழு) roundf ((தசம) (new_final * 16384.0)));
    எனில் ((unsigned) err > max_err) max_err = err;
  }
  திரும்பு max_err;
}

கட்டமைப்பு avar
{
  நிலையான constexpr hb_tag_t tableTag = HB_OT_TAG_avar;

  தருக்க has_data () மாறிலி { திரும்பு version.to_int (); }

  மாறிலி SegmentMaps* get_segment_maps () மாறிலி
  { திரும்பு &firstAxisSegmentMaps; }

  unsigned get_axis_மொத்தம் () மாறிலி
  { திரும்பு axis_மொத்தம்_2; }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    எனில் (!(version.sanitize (c) &&
	  hb_barrier () &&
	  (version.major == 1
#ifndef HB_NO_AVAR2
	   || version.major == 2
#endif
	   ) &&
	  c->check_struct (இது)))
      return_trace (பொய்);

    மாறிலி SegmentMaps *map = &firstAxisSegmentMaps;
    unsigned முழு மொத்தம் = axis_மொத்தம்_2;
    க்கு (unsigned முழு i = 0; i < மொத்தம்; i++)
    {
      எனில் (unlikely (!map->sanitize (c)))
	return_trace (பொய்);
      map = &StructAfter<SegmentMaps> (*map);
    }

#ifndef HB_NO_AVAR2
    எனில் (version.major < 2)
      return_trace (உண்மை);
    hb_barrier ();

    மாறிலி auto &v2 = * (மாறிலி avarV2Tail *) map;
    எனில் (unlikely (!v2.sanitize (c, இது)))
      return_trace (பொய்);
#endif

    return_trace (உண்மை);
  }

  வெறுமை map_coords_16_16 (முழு *coords, unsigned முழு coords_length) மாறிலி
  {
    unsigned முழு மொத்தம் = hb_min (coords_length, axis_மொத்தம்_2);

    மாறிலி SegmentMaps *map = &firstAxisSegmentMaps;
    க்கு (unsigned முழு i = 0; i < மொத்தம்; i++)
    {
      coords[i] = roundf (map->map_float (coords[i] / 65536.f) * 65536.f);
      map = &StructAfter<SegmentMaps> (*map);
    }

#ifndef HB_NO_AVAR2
    எனில் (version.major < 2)
      திரும்பு;
    hb_barrier ();

    க்கு (; மொத்தம் < axis_மொத்தம்_2; மொத்தம்++)
      map = &StructAfter<SegmentMaps> (*map);

    மாறிலி auto &v2 = * (மாறிலி avarV2Tail *) map;

    மாறிலி auto &varidx_map = இது+v2.varIdxMap;
    மாறிலி auto &var_store = இது+v2.varStore;
    auto *var_store_cache = var_store.create_cache ();

    hb_vector_t<முழு> coords_2_14;
    coords_2_14.resize (coords_length);
    க்கு (unsigned i = 0; i < coords_length; i++)
      coords_2_14[i] = roundf (coords[i] / 4.f); // 16.16 -> 2.14

    hb_vector_t<முழு> out;
    out.alloc (coords_length);
    க்கு (unsigned i = 0; i < coords_length; i++)
    {
      முழு v = coords[i];
      uint32_t varidx = varidx_map.map (i);
      தசம delta = var_store.get_delta (varidx, coords_2_14.arrayZ, coords_2_14.length, var_store_cache);
      /* Apply the delta unclamped and clamp only the result to [-1, +1],
       * matching fontTools. Since inputs and results are in [-1, +1],
       * deltas beyond ±2 are equivalent to ±2; clamp to that range only
       * to keep the float->int conversion safe. */
      தசம d = hb_clamp (delta * 4, -(தசம) (1<<17), +(தசம) (1<<17)); // 2.14 -> 16.16
      v += (முழு) roundf (d);
      v = hb_clamp (v, -(1<<16), +(1<<16));
      out.push (v);
    }
    க்கு (unsigned i = 0; i < coords_length; i++)
      coords[i] = out[i];

    OT::ItemVariationStore::destroy_cache (var_store_cache);
#endif
  }

  /* An avar version 2 font is never downgraded to v1: even with a NULL
   * varStore offset (legal; maps like plain v1) the avar2 subsetting path
   * applies, with "no variation" semantics for rows that don't resolve —
   * offset compensation then creates delta rows on demand. */
  தருக்க has_v2_data () மாறிலி { திரும்பு version.major > 1; }

  /* Resolve the avar2 VarStore and VarIdxMap, for the subset planner.
   * Either pointer may be to the Null object (nullable offsets). */
  தருக்க get_v2_store_and_map (மாறிலி ItemVariationStore **store,
                             மாறிலி DeltaSetIndexMap **varidx_map) மாறிலி
  {
#ifndef HB_NO_AVAR2
    எனில் (version.major < 2) திரும்பு பொய்;
    மாறிலி SegmentMaps *map = &firstAxisSegmentMaps;
    க்கு (unsigned i = 0; i < axis_மொத்தம்_2; i++)
      map = &StructAfter<SegmentMaps> (*map);
    மாறிலி auto &v2 = * (மாறிலி avarV2Tail *) map;
    *store = &(இது+v2.varStore);
    *varidx_map = &(இது+v2.varIdxMap);
    திரும்பு உண்மை;
#இல்லையெனில்
    திரும்பு பொய்;
#endif
  }

  // axis normalization is done in 2.14 here
  // TODO: deprecate this API once fonttools is updated to use 16.16 normalization
  தருக்க map_coords_2_14 (தசம *coords, unsigned முழு coords_length,
			தருக்க v1_only = பொய்) மாறிலி
  {
    hb_vector_t<முழு> coords_2_14;
    எனில் (!v1_only && !coords_2_14.resize (coords_length)) திரும்பு பொய்;
    unsigned முழு மொத்தம் = hb_min (coords_length, axis_மொத்தம்_2);

    மாறிலி SegmentMaps *map = &firstAxisSegmentMaps;
    க்கு (unsigned முழு i = 0; i < மொத்தம்; i++)
    {
      முழு v = roundf (map->map_float (coords[i]) * 16384.f);
      எனில் (!v1_only)
	coords_2_14[i] = v;
      coords[i] = v / 16384.f;
      map = &StructAfter<SegmentMaps> (*map);
    }

    எனில் (v1_only)
      திரும்பு உண்மை;

#ifndef HB_NO_AVAR2
    எனில் (version.major < 2)
      திரும்பு உண்மை;
    hb_barrier ();

    க்கு (; மொத்தம் < axis_மொத்தம்_2; மொத்தம்++)
      map = &StructAfter<SegmentMaps> (*map);

    மாறிலி auto &v2 = * (மாறிலி avarV2Tail *) map;

    மாறிலி auto &varidx_map = இது+v2.varIdxMap;
    மாறிலி auto &var_store = இது+v2.varStore;
    auto *var_store_cache = var_store.create_cache ();

    க்கு (unsigned i = 0; i < coords_length; i++)
    {
      முழு v = coords_2_14[i];
      uint32_t varidx = varidx_map.map (i);
      தசம delta = var_store.get_delta (varidx, coords_2_14.arrayZ, coords_2_14.length, var_store_cache);
      /* As above: apply the delta unclamped (±2 covers every useful case)
       * and clamp the result to [-1, +1], matching fontTools. */
      v += (முழு) hb_clamp (roundf (delta), -(தசம) (1<<15), +(தசம) (1<<15));
      v = hb_clamp (v, -(1<<14), +(1<<14));
      coords[i] = v / 16384.f;
    }

    OT::ItemVariationStore::destroy_cache (var_store_cache);
    திரும்பு உண்மை;
#இல்லையெனில்
    திரும்பு version.major < 2;
#endif
  }

  தருக்க subset (hb_subset_context_t *c) மாறிலி
  {
    TRACE_SUBSET (இது);
    unsigned retained_axis_மொத்தம் = c->plan->axes_index_map.get_population ();
    எனில் (!retained_axis_மொத்தம்) //all axes are pinned/dropped
      return_trace (பொய்);

    avar *out = c->serializer->allocate_min<avar> ();
    எனில் (unlikely (!out)) return_trace (பொய்);

    out->version.major = c->plan->has_avar2 ? 2 : 1;
    out->version.minor = 0;
    எனில் (!c->serializer->check_assign (out->axis_மொத்தம்_2, retained_axis_மொத்தம், HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (பொய்);

    /* For avar2, keep the instantiated v1 mappings around; offset
     * compensation needs them to locate the new mappings' kinks. */
    hb_vector_t<hb_vector_t<Axis_மதிப்பு_Map>> new_mappings;
    எனில் (c->plan->has_avar2 && !new_mappings.resize (axis_மொத்தம்_2))
      return_trace (பொய்);

    மாறிலி hb_map_t& axes_index_map = c->plan->axes_index_map;
    மாறிலி SegmentMaps *map = &firstAxisSegmentMaps;
    unsigned மொத்தம் = axis_மொத்தம்_2;
    க்கு (unsigned முழு i = 0; i < மொத்தம்; i++)
    {
      எனில் (axes_index_map.has (i))
      {
        hb_tag_t *axis_tag;
        எனில் (!c->plan->axes_old_index_tag_map.has (i, &axis_tag))
          return_trace (பொய்);

        Triple *axis_location;
        எனில் (c->plan->has_avar2 &&
            c->plan->user_axes_location.has (*axis_tag, &axis_location) &&
            axis_location->is_point ())
        {
          /* Pinned axis in avar2 mode: serialize identity segment map
           * {-1->-1, 0->0, 1->1}. The axis is kept in fvar as hidden,
           * so avar needs a segment map entry for it. */
          auto *identity_map = c->serializer->start_embed<SegmentMaps> ();
          எனில் (unlikely (!c->serializer->extend_min (identity_map)))
            return_trace (பொய்);
          Axis_மதிப்பு_Map m;
          m.set_mapping (-1.f, -1.f);
          எனில் (!m.serialize (c->serializer)) return_trace (பொய்);
          m.set_mapping (0.f, 0.f);
          எனில் (!m.serialize (c->serializer)) return_trace (பொய்);
          m.set_mapping (1.f, 1.f);
          எனில் (!m.serialize (c->serializer)) return_trace (பொய்);
          எனில் (!c->serializer->check_assign (identity_map->len, 3u,
                                            HB_SERIALIZE_ERROR_INT_OVERFLOW))
            return_trace (பொய்);
        }
        இல்லையெனில்
        {
          /* Restricted or free axis: use standard SegmentMaps::subset() */
          எனில் (!map->subset (c, *axis_tag,
                            c->plan->has_avar2 ? &new_mappings[i] : nullptr))
            return_trace (பொய்);
        }
      }
      map = &StructAfter<SegmentMaps> (*map);
    }

    எனில் (c->plan->has_avar2)
      return_trace (_subset_avar2 (c, new_mappings));

    return_trace (உண்மை);
  }

  தனிப்பட்ட:
  கட்டமைப்பு avar2_index_map_plan_t
  {
    தருக்க init (மாறிலி hb_vector_t<uint32_t> &varidx_mapping,
	       மாறிலி hb_map_t &axes_index_map,
	       unsigned axis_மொத்தம்)
    {
      எனில் (!output_map.alloc (axes_index_map.get_population ()))
	திரும்பு பொய்;

      தருக்க has_no_variation = பொய்;
      unsigned max_outer = 0, max_inner = 0;
      க்கு (unsigned i = 0; i < axis_மொத்தம்; i++)
      {
	எனில் (!axes_index_map.has (i)) தொடர்;
	uint32_t varidx = varidx_mapping[i];
	output_map.push (varidx);
	எனில் (varidx == HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
	{
	  has_no_variation = உண்மை;
	  தொடர்;
	}
	max_outer = hb_max (max_outer, varidx >> 16);
	max_inner = hb_max (max_inner, varidx & 0xFFFF);
      }
      எனில் (output_map.in_error () ||
	  output_map.length != axes_index_map.get_population ())
	திரும்பு பொய்;

      எனில் (has_no_variation)
      {
	width = 4;
	inner_bit_மொத்தம் = 16;
      }
      இல்லையெனில்
      {
	inner_bit_மொத்தம் = hb_max (1u, hb_bit_storage (max_inner));
	unsigned outer_bit_மொத்தம் = hb_max (1u, hb_bit_storage (max_outer));
	width = hb_clamp ((inner_bit_மொத்தம் + outer_bit_மொத்தம் + 7) / 8, 1u, 4u);
	எனில் (inner_bit_மொத்தம் + outer_bit_மொத்தம் > width * 8)
	  inner_bit_மொத்தம் = width * 8 - outer_bit_மொத்தம்;
      }
      திரும்பு உண்மை;
    }

    unsigned get_inner_bit_மொத்தம் () மாறிலி { திரும்பு inner_bit_மொத்தம்; }
    unsigned get_width () மாறிலி { திரும்பு width; }
    hb_array_t<மாறிலி uint32_t> get_output_map () மாறிலி { திரும்பு output_map.as_array (); }

    unsigned inner_bit_மொத்தம் = 1;
    unsigned width = 1;
    hb_vector_t<uint32_t> output_map;
  };

  தருக்க _subset_avar2 (hb_subset_context_t *c,
                      மாறிலி hb_vector_t<hb_vector_t<Axis_மதிப்பு_Map>> &new_mappings) மாறிலி
  {
#எனில் defined (HB_NO_VAR) || defined (HB_NO_AVAR2)
    /* Not reachable: the plan never sets has_avar2 in these configurations. */
    திரும்பு பொய்;
#இல்லையெனில்

    /* 1. Locate original avar2 data, keeping per-axis old segment maps */
    hb_vector_t<மாறிலி SegmentMaps *> old_seg_maps;
    எனில் (!old_seg_maps.alloc (axis_மொத்தம்_2)) திரும்பு பொய்;
    மாறிலி SegmentMaps *map = &firstAxisSegmentMaps;
    க்கு (unsigned i = 0; i < axis_மொத்தம்_2; i++)
    {
      old_seg_maps.push (map);
      map = &StructAfter<SegmentMaps> (*map);
    }

    மாறிலி auto &v2 = * (மாறிலி avarV2Tail *) map;
    மாறிலி auto &varidx_map = இது+v2.varIdxMap;
    மாறிலி auto &var_store = இது+v2.varStore;

    auto fvar_axes = c->plan->source->table.fvar->get_axes ();

    /* 2. Compute default deltas by evaluating VarStore at old defaults */
    hb_vector_t<முழு> default_coords;
    எனில் (!default_coords.resize (axis_மொத்தம்_2)) திரும்பு பொய்;
    க்கு (unsigned i = 0; i < axis_மொத்தம்_2; i++)
    {
      hb_tag_t *axis_tag;
      எனில் (c->plan->axes_old_index_tag_map.has (i, &axis_tag) &&
          c->plan->old_intermediates.has (*axis_tag))
      {
        தசம d_i = (தசம) c->plan->old_intermediates.get (*axis_tag).நடு;
        default_coords[i] = roundf (d_i * 16384.f);
      }
      இல்லையெனில்
        default_coords[i] = 0;
    }

    auto *store_cache = var_store.create_cache ();
    hb_vector_t<தசம> default_deltas;
    எனில் (!default_deltas.resize (axis_மொத்தம்_2)) {
      ItemVariationStore::destroy_cache (store_cache);
      திரும்பு பொய்;
    }
    க்கு (unsigned i = 0; i < axis_மொத்தம்_2; i++)
    {
      uint32_t varidx = varidx_map.map (i);
      எனில் (varidx == HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
        default_deltas[i] = 0.f;
      இல்லையெனில்
        default_deltas[i] = var_store.get_delta (varidx, default_coords.arrayZ,
                                                  default_coords.length, store_cache);
    }
    ItemVariationStore::destroy_cache (store_cache);

    /* 3. Rebase IVS regions */
    item_variations_t item_vars;
    எனில் (!item_vars.create_from_item_varstore (var_store, c->plan->axes_old_index_tag_map))
      திரும்பு பொய்;
    எனில் (!item_vars.instantiate_tuple_vars (c->plan->old_intermediates,
                                           c->plan->axes_triple_distances,
                                           பொய்))
      திரும்பு பொய்;

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
    எனில் (!new_varidx_mapping.resize (axis_மொத்தம்_2)) திரும்பு பொய்;
    க்கு (unsigned i = 0; i < axis_மொத்தம்_2; i++)
    {
      uint32_t varidx = varidx_map.map (i);
      எனில் (varidx != HB_OT_LAYOUT_NO_VARIATIONS_INDEX &&
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
    hb_hashmap_t<uint32_t, unsigned> varidx_ref_மொத்தம்;
    க்கு (unsigned i = 0; i < axis_மொத்தம்_2; i++)
    {
      எனில் (!c->plan->axes_index_map.has (i)) தொடர்; /* self-contained: dropped */
      uint32_t varidx = new_varidx_mapping[i];
      எனில் (varidx == HB_OT_LAYOUT_NO_VARIATIONS_INDEX) தொடர்;
      unsigned *மொத்தம்;
      எனில் (varidx_ref_மொத்தம்.has (varidx, &மொத்தம்))
        (*மொத்தம்)++;
      இல்லையெனில் எனில் (!varidx_ref_மொத்தம்.set (varidx, 1))
        திரும்பு பொய்;
    }
    க்கு (unsigned i = 0; i < axis_மொத்தம்_2; i++)
    {
      hb_tag_t *axis_tag_ptr;
      எனில் (!c->plan->axes_old_index_tag_map.has (i, &axis_tag_ptr))
        திரும்பு பொய்;
      /* Only axes that will receive offset compensation (restricted or
       * pinned) can contaminate a shared row. */
      எனில் (!c->plan->axes_index_map.has (i) ||
          !c->plan->user_axes_location.has (*axis_tag_ptr))
        தொடர்;
      uint32_t varidx = new_varidx_mapping[i];
      எனில் (varidx == HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
        தொடர்; /* gets a fresh, private VarData in the offset loop below */
      unsigned *மொத்தம்;
      எனில் (!varidx_ref_மொத்தம்.has (varidx, &மொத்தம்) || *மொத்தம் <= 1)
        தொடர்; /* sole owner: safe to write offsets in place */
      unsigned outer = varidx >> 16;
      unsigned new_inner = item_vars.duplicate_row (outer, varidx & 0xFFFF);
      எனில் (unlikely (new_inner == (unsigned) -1)) திரும்பு பொய்;
      new_varidx_mapping[i] = (outer << 16) | new_inner;
      (*மொத்தம்)--;
    }

    /* 6. Add offset compensation tuples.
     * Track processed (outer,inner) pairs to avoid adding duplicate biases
     * when multiple axes share the same varIdx. */
    hb_set_t processed_varidxes;
    க்கு (unsigned i = 0; i < axis_மொத்தம்_2; i++)
    {
      hb_tag_t *axis_tag_ptr;
      எனில் (!c->plan->axes_old_index_tag_map.has (i, &axis_tag_ptr))
        திரும்பு பொய்;
      hb_tag_t axis_tag = *axis_tag_ptr;

      /* Self-contained pinned axes are removed from fvar/avar; their
       * contribution is baked into the variation tables instead. */
      எனில் (!c->plan->axes_index_map.has (i))
        தொடர்;

      Triple *new_user;
      எனில் (c->plan->user_axes_location.has (axis_tag, &new_user))
      {
        /* This axis is being restricted or pinned */
        Triple *old_int;
        எனில் (!c->plan->old_intermediates.has (axis_tag, &old_int))
          திரும்பு பொய்;

        தசம a_i = (தசம) old_int->minimum;
        தசம d_i = (தசம) old_int->நடு;
        தசம b_i = (தசம) old_int->maximum;

        முழு d_int = roundf (d_i * 16384.f);

        தருக்க is_pinned = new_user->is_point ();

        uint32_t varidx = new_varidx_mapping[i];
        unsigned outer, inner, item_மொத்தம்_2;

        எனில் (varidx == HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
        {
          /* No existing avar2 mapping. Create new VarData. */
          outer = item_vars.add_vardata (1);
          inner = 0;
          item_மொத்தம்_2 = 1;
          new_varidx_mapping[i] = (outer << 16) | inner;
          default_deltas[i] = 0.f; /* no prior default delta */
        }
        இல்லையெனில்
        {
          outer = varidx >> 16;
          inner = varidx & 0xFFFF;
          item_மொத்தம்_2 = item_vars.get_item_மொத்தம் (outer);
        }

        /* Empty-region bias: d_int + round(defaultDelta) */
        முழு bias = d_int + (முழு) roundf (default_deltas[i]);
        எனில் (bias != 0)
        {
          hb_hashmap_t<hb_tag_t, Triple> empty_region;
          item_vars.add_tuple (outer, std::move (empty_region),
                               inner, bias, item_மொத்தம்_2);
        }

        எனில் (!is_pinned)
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
          _avar2_add_knot (knots, -1.0, (இரட்டை) a_i + 1.0);
          _avar2_add_knot (knots, 0.0, (இரட்டை) d_i);
          _avar2_add_knot (knots, 1.0, (இரட்டை) b_i - 1.0);

          மாறிலி hb_vector_t<Axis_மதிப்பு_Map> &new_mapping = new_mappings[i];

          தசம min_f = 0.f, def_f = 0.f, max_f = 0.f;
          எனில் (likely (i < fvar_axes.length))
            fvar_axes[i].get_coordinates (min_f, def_f, max_f);
          இரட்டை old_min = (இரட்டை) min_f;
          இரட்டை old_def = (இரட்டை) def_f;
          இரட்டை old_max = (இரட்டை) max_f;

          /* If the axis default MOVED, inv_renorm also kinks where the OLD
           * default lands in the new space (the old intermediate coordinate
           * crosses 0 there), at
           *   z = z_old  ->  -z_old
           * Omitting that knot (as a plain two-tent encoding would) makes
           * interior coordinates wrong. */
          இரட்டை z_old = avar2_normalize_மதிப்பு ((இரட்டை) old_def,
                                                 (இரட்டை) new_user->minimum,
                                                 (இரட்டை) new_user->நடு,
                                                 (இரட்டை) new_user->maximum);
          z_old = _avar2_map_new_mapping (new_mapping, z_old);
          z_old = (இரட்டை) roundf ((தசம) (z_old * 16384.0)) / 16384.0;
          எனில் (-1.0 < z_old && z_old < 1.0)
            _avar2_add_knot (knots, z_old, -z_old);

          /* Interior avar v1 breakpoints inside the retained range each put
           * a kink in offset(z). Sampling only {-1, 0, +1, z_old} would
           * linearly interpolate across those kinks. The instantiated
           * mapping keeps exactly the in-range old breakpoints, and the new
           * mapping kinks at each one's output coordinate; add that z with
           * its old intermediate value so offset(z) is reproduced at every
           * kink. */
          hb_vector_t<avar2_offset_knot_t> with_breakpoints (knots);
          க்கு (மாறிலி auto &m : new_mapping)
          {
            இரட்டை from = (இரட்டை) m.coords[0].to_float ();
            எனில் (from == -1.0 || from == 0.0 || from == 1.0)
              தொடர்; /* anchors already seeded */
            இரட்டை z = (இரட்டை) m.coords[1].to_float ();
            எனில் (!(-1.0 < z && z < 1.0))
              தொடர்;
            இரட்டை user = avar2_denormalize_மதிப்பு (from,
                                                    (இரட்டை) new_user->minimum,
                                                    (இரட்டை) new_user->நடு,
                                                    (இரட்டை) new_user->maximum);
            இரட்டை n_old = avar2_normalize_மதிப்பு (user, old_min, old_def, old_max);
            இரட்டை x_old = (இரட்டை) old_seg_maps[i]->map_float ((தசம) n_old);
            x_old = (இரட்டை) roundf ((தசம) (x_old * 16384.0)) / 16384.0;
            _avar2_add_knot (with_breakpoints, z, x_old - z);
          }

          எனில் (unlikely (knots.in_error () || with_breakpoints.in_error ()))
            திரும்பு பொய்;

          /* Extra tents cost F2Dot14 rounding, so for a steep segment they
           * can add more quantization noise than the structural error they
           * remove. Keep the interior breakpoints only when they do not
           * increase the estimated residual; this makes the collection a
           * strict (never-worse) improvement over the {-1, 0, +1, z_old}
           * anchors. Warn when even the better choice is not bit-exact (a
           * steep retained segment that cannot be reproduced in F2Dot14). */
          unsigned err = _avar2_estimate_offset_error (*old_seg_maps[i], new_mapping,
                                                       old_min, old_def, old_max,
                                                       (இரட்டை) new_user->minimum,
                                                       (இரட்டை) new_user->நடு,
                                                       (இரட்டை) new_user->maximum,
                                                       knots);
          எனில் (with_breakpoints.length != knots.length)
          {
            unsigned err_with = _avar2_estimate_offset_error (*old_seg_maps[i], new_mapping,
                                                              old_min, old_def, old_max,
                                                              (இரட்டை) new_user->minimum,
                                                              (இரட்டை) new_user->நடு,
                                                              (இரட்டை) new_user->maximum,
                                                              with_breakpoints);
            எனில் (err_with <= err)
            {
              knots = std::move (with_breakpoints);
              err = err_with;
            }
          }
          எனில் (err > 8)
            DEBUG_செய்தி (SUBSET, nullptr,
                       "avar2 offset compensation is approximate for axis %c%c%c%c: "
                       "max residual %u F2Dot14 units",
                       HB_UNTAG (axis_tag), err);

          /* Synthesize tents. Adjacent tents evaluate to zero at each
           * other's peaks, so each knot's delta is offset(z) - offset(0);
           * the base value offset(0) = d_i is carried by the empty-region
           * bias above. This reduces to the classic pair of tents
           * (-1,-1,0) / (0,+1,+1) when the default is unchanged and there
           * are no interior knots. */
          க்கு (unsigned k = 0; k < knots.length; k++)
          {
            இரட்டை z = knots.arrayZ[k].z;
            எனில் (z == 0.0) தொடர்;
            முழு delta = (முழு) roundf ((தசம) ((knots.arrayZ[k].offset - (இரட்டை) d_i) * 16384.0));
            எனில் (!delta) தொடர்;
            இரட்டை lower, upper;
            எனில் (z > 0.0)
            {
              lower = knots.arrayZ[k - 1].z;
              upper = k + 1 < knots.length ? knots.arrayZ[k + 1].z : z;
            }
            இல்லையெனில்
            {
              upper = knots.arrayZ[k + 1].z;
              lower = k > 0 ? knots.arrayZ[k - 1].z : z;
            }
            hb_hashmap_t<hb_tag_t, Triple> region;
            எனில் (unlikely (!region.set (axis_tag, Triple (lower, z, upper))))
              திரும்பு பொய்;
            item_vars.add_tuple (outer, std::move (region),
                                 inner, delta, item_மொத்தம்_2);
          }
        }
      }
      இல்லையெனில்
      {
        /* Free or private axis — not being restricted.
         * If it has a non-zero default delta, add it back as a bias.
         * Skip if this (outer,inner) was already processed (shared varIdx). */
        uint32_t varidx = new_varidx_mapping[i];
        எனில் (varidx == HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
          தொடர்;

        எனில் (processed_varidxes.has (varidx))
          தொடர்;
        processed_varidxes.add (varidx);

        unsigned outer = varidx >> 16;
        unsigned inner = varidx & 0xFFFF;
        முழு dd = (முழு) roundf (default_deltas[i]);
        எனில் (dd != 0)
        {
          unsigned item_மொத்தம்_2 = item_vars.get_item_மொத்தம் (outer);
          hb_hashmap_t<hb_tag_t, Triple> empty_region;
          item_vars.add_tuple (outer, std::move (empty_region),
                               inner, dd, item_மொத்தம்_2);
        }
      }
    }

    /* 7. Finalize: build region list + convert to varstore */
    எனில் (!item_vars.build_region_list ()) திரும்பு பொய்;
    எனில் (!item_vars.as_item_varstore (உண்மை /* optimize */,
                                     பொய் /* use_no_variation_idx */))
      திரும்பு பொய்;

    /* 8. Apply varidx_map optimization remapping */
    மாறிலி auto &opt_varidx_map = item_vars.get_varidx_map ();
    க்கு (unsigned i = 0; i < axis_மொத்தம்_2; i++)
    {
      uint32_t varidx = new_varidx_mapping[i];
      எனில் (varidx == HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
        தொடர்;
      uint32_t *new_idx;
      எனில் (opt_varidx_map.has (varidx, &new_idx))
        new_varidx_mapping[i] = *new_idx;
    }

    /* 9. Serialize avarV2Tail. Entries cover the retained axes only;
     * self-contained pinned axes are removed from fvar. */
    avar2_index_map_plan_t index_map_plan;
    எனில் (!index_map_plan.init (new_varidx_mapping,
			      c->plan->axes_index_map,
			      axis_மொத்தம்_2))
      திரும்பு பொய்;

    auto *tail = c->serializer->allocate_size<avarV2Tail> (avarV2Tail::static_size);
    எனில் (unlikely (!tail)) திரும்பு பொய்;
    எனில் (!tail->varIdxMap.serialize_serialize (c->serializer, index_map_plan))
      திரும்பு பொய்;
    எனில் (!tail->varStore.serialize_serialize (c->serializer,
					     item_vars.has_long_word (),
					     c->plan->axis_tags,
					     item_vars.get_region_list (),
					     item_vars.get_vardata_encodings ()))
      திரும்பு பொய்;

    திரும்பு உண்மை;
#endif
  }

  பொது:

  பாதுகாக்கப்பட்ட:
  FixedVersion<>version;	/* Version of the avar table
				 * initially set to 0x00010000u */
  HBUINT16	reserved;	/* This field is permanently reserved. Set to 0. */
  HBUINT16	axis_மொத்தம்_2;	/* The number of variation axes in the font. This
				 * must be the same number as axisCount in the
				 * 'fvar' table. */
  SegmentMaps	firstAxisSegmentMaps;

  பொது:
  DEFINE_SIZE_MIN (8);
};

} /* namespace OT */


#endif /* HB_OT_VAR_AVAR_TABLE_HH */
