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

#अन्तर्भाव "hb-open-type.hh"
#अन्तर्भाव "hb-ot-var-common.hh"
#अन्तर्भाव "hb-ot-var-fvar-table.hh"


/*
 * avar -- Axis Variations
 * https://docs.microsoft.com/en-us/typography/opentype/spec/avar
 */

#define HB_OT_TAG_avar HB_TAG('a','v','a','r')


नामाकाश OT {


/* "Spec": https://github.com/be-fonts/boring-expansion-spec/issues/14 */
संरचना avarV2Tail
{
  मित्र संरचना avar;

  तार्किक sanitize (hb_sanitize_context_t *c,
		 स्थिरांक शून्य *base) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (varIdxMap.sanitize (c, base) &&
		  varStore.sanitize (c, base));
  }

  संरक्षित:
  Offset32To<DeltaSetIndexMap>	varIdxMap;	/* Offset from the beginning of 'avar' table. */
  Offset32To<ItemVariationStore>	varStore;	/* Offset from the beginning of 'avar' table. */

  सार्वजनिक:
  DEFINE_SIZE_STATIC (8);
};


संरचना Axis_मूल्य_Map
{
  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे));
  }

  शून्य set_mapping (दशांश from_coord, दशांश to_coord)
  {
    coords[0].set_float (from_coord);
    coords[1].set_float (to_coord);
  }

  तार्किक is_outside_axis_range (स्थिरांक Triple& axis_range) स्थिरांक
  {
    द्विगुण from_coord = (द्विगुण) coords[0].to_float ();
    परत !axis_range.contains (from_coord);
  }

  तार्किक must_include () स्थिरांक
  {
    दशांश from_coord = coords[0].to_float ();
    दशांश to_coord = coords[1].to_float ();
    परत (from_coord == -1.f && to_coord == -1.f) ||
           (from_coord == 0.f && to_coord == 0.f) ||
           (from_coord == 1.f && to_coord == 1.f);
  }

  शून्य instantiate (स्थिरांक Triple& axis_range,
                    स्थिरांक Triple& unmapped_range,
                    स्थिरांक TripleDistances& triple_distances)
  {
    दशांश from_coord = coords[0].to_float ();
    दशांश to_coord = coords[1].to_float ();

    from_coord = renormalize_मूल्य ((द्विगुण) from_coord, unmapped_range, triple_distances);
    to_coord = renormalize_मूल्य ((द्विगुण) to_coord, axis_range, triple_distances);

    /* Malformed input can renormalize outside [-1,1], which would
     * overflow F2DOT14. */
    from_coord = hb_clamp (from_coord, -1.f, 1.f);
    to_coord = hb_clamp (to_coord, -1.f, 1.f);

    coords[0].set_float (from_coord);
    coords[1].set_float (to_coord);
  }

  HB_INTERNAL स्थिर पूर्णांक cmp (स्थिरांक शून्य *pa, स्थिरांक शून्य *pb)
  {
    स्थिरांक Axis_मूल्य_Map *a = (स्थिरांक Axis_मूल्य_Map *) pa;
    स्थिरांक Axis_मूल्य_Map *b = (स्थिरांक Axis_मूल्य_Map *) pb;

    पूर्णांक a_from = a->coords[0].to_int ();
    पूर्णांक b_from = b->coords[0].to_int ();
    जर (a_from != b_from)
      परत a_from - b_from;

    /* this should never be reached. according to the spec, all of the axis
     * value map records for a given axis must have different fromCoord values
     * */
    पूर्णांक a_to = a->coords[1].to_int ();
    पूर्णांक b_to = b->coords[1].to_int ();
    परत a_to - b_to;
  }

  तार्किक serialize (hb_serialize_context_t *c) स्थिरांक
  {
    TRACE_SERIALIZE (हे);
    return_trace (c->embed (हे));
  }

  सार्वजनिक:
  F2DOT14	coords[2];
//   F2DOT14	fromCoord;	/* A normalized coordinate value obtained using
//				 * default normalization. */
//   F2DOT14	toCoord;	/* The modified, normalized coordinate value. */

  सार्वजनिक:
  DEFINE_SIZE_STATIC (4);
};

संरचना SegmentMaps : Array16Of<Axis_मूल्य_Map>
{
  दशांश map_float (दशांश value, अचिन्हित पूर्णांक from_offset = 0, अचिन्हित पूर्णांक to_offset = 1) स्थिरांक
  {
#define fromCoord coords[from_offset].to_float ()
#define toCoord coords[to_offset].to_float ()

    स्थिरांक स्वतः *map = arrayZ;

    /* The following special-cases are not part of OpenType, which requires
     * that at least -1, 0, and +1 must be mapped. But we include these as
     * part of a better error recovery scheme. */
    जर (len < 2)
    {
      जर (!len)
	परत value;
      अन्यथा /* len == 1*/
	परत value - map[0].fromCoord + map[0].toCoord;
    }

    // At least two mappings now.

    /* CoreText is wild...
     * PingFangUI avar needs all this special-casing...
     * So we implement an extended version of the spec here,
     * which is more robust and more likely to be compatible with
     * the wild. */

    अचिन्हित start = 0;
    अचिन्हित end = len;
    जर (map[start].fromCoord == -1 && map[start].toCoord == -1 && map[start+1].fromCoord == -1)
      start++;
    जर (map[end-1].fromCoord == +1 && map[end-1].toCoord == +1 && map[end-2].fromCoord == +1)
      end--;

    /* Look for exact match first, and do lots of special-casing. */
    अचिन्हित i;
    साठी (i = start; i < end; i++)
      जर (value == map[i].fromCoord)
	थांबा;
    जर (i < end)
    {
      // There's at least one exact match. See if there are more.
      अचिन्हित j = i;
      साठी (; j + 1 < end; j++)
	जर (value != map[j + 1].fromCoord)
	  थांबा;

      // [i,j] inclusive are all exact matches:

      // If there's only one, return it. This is the only spec-compliant case.
      जर (i == j)
	परत map[i].toCoord;
      // If there's exactly three, return the middle one.
      जर (i + 2 == j)
	परत map[i + 1].toCoord;

      // Ignore the middle ones. Return the one mapping closer to 0.
      जर (value < 0) परत map[j].toCoord;
      जर (value > 0) परत map[i].toCoord;

      // Mapping 0? CoreText seems confused. It seems to prefer 0 here...
      // So we'll just return the smallest one. lol
      परत fabsf (map[i].toCoord) < fabsf (map[j].toCoord) ? map[i].toCoord : map[j].toCoord;

      // Mapping 0? Return one not mapping to 0.
      जर (map[i].toCoord == 0)
	परत map[j].toCoord;
      अन्यथा
	परत map[i].toCoord;
    }

    /* There's at least two and we're not an exact match. Prepare to lerp. */

    // Find the segment we're in.
    साठी (i = start; i < end; i++)
      जर (value < map[i].fromCoord)
	थांबा;

    जर (i == start)
    {
      // Value before all segments; Shift.
      परत value - map[start].fromCoord + map[start].toCoord;
    }
    जर (i == end)
    {
      // Value after all segments; Shift.
      परत value - map[end - 1].fromCoord + map[end - 1].toCoord;
    }

    // Actually interpolate.
    स्वतः &before = map[i-1];
    स्वतः &after = map[i];
    दशांश denom = after.fromCoord - before.fromCoord; // Can't be zero by now.
    परत before.toCoord + ((after.toCoord - before.toCoord) * (value - before.fromCoord)) / denom;

#undef toCoord
#undef fromCoord
  }

  दशांश unmap_float (दशांश value) स्थिरांक { परत map_float (value, 1, 0); }


  // TODO Kill this.
  Triple unmap_axis_range (स्थिरांक Triple& axis_range) स्थिरांक
  {
    दशांश unmapped_min = unmap_float (axis_range.minimum);
    दशांश unmapped_मधला = unmap_float (axis_range.मधला);
    दशांश unmapped_max = unmap_float (axis_range.maximum);

    परत Triple{unmapped_min, unmapped_मधला, unmapped_max};
  }

  तार्किक subset (hb_subset_context_t *c, hb_tag_t axis_tag,
               hb_vector_t<Axis_मूल्य_Map> *out_mappings = nullptr) स्थिरांक
  {
    TRACE_SUBSET (हे);

    /* This function cannot work on avar2 table (and currently doesn't).
     * We should instead keep the design coords in the shape plan and use
     * those. unmap_axis_range needs to be killed. */

    /* avar mapped normalized axis range. Under avar2, axes_location holds
     * only the self-contained pins for the other tables; avar itself uses
     * the intermediate-space ranges of all restricted axes. */
    स्थिरांक स्वतः &axes_location = c->plan->has_avar2
				? c->plan->old_intermediates
				: c->plan->axes_location;
    Triple *axis_range;
    जर (!axes_location.has (axis_tag, &axis_range))
      परत c->serializer->embed (*हे);

    TripleDistances *axis_triple_distances;
    जर (!c->plan->axes_triple_distances.has (axis_tag, &axis_triple_distances))
      return_trace (असत्य);

    स्वतः *out = c->serializer->start_embed (हे);
    जर (unlikely (!c->serializer->extend_min (out))) return_trace (असत्य);

    Triple unmapped_range = unmap_axis_range (*axis_range);

    /* create a vector of retained mappings and sort */
    hb_vector_t<Axis_मूल्य_Map> मूल्य_mappings;
    साठी (स्थिरांक स्वतः& _ : as_array ())
    {
      जर (_.is_outside_axis_range (unmapped_range))
        सुरूठेवा;
      Axis_मूल्य_Map mapping;
      mapping = _;
      mapping.instantiate (*axis_range, unmapped_range, *axis_triple_distances);
      /* (-1, -1), (0, 0), (1, 1) mappings will be added later, so avoid
       * duplicates here */
      जर (mapping.must_include ())
        सुरूठेवा;
      मूल्य_mappings.push (mapping);
    }

    Axis_मूल्य_Map m;
    m.set_mapping (-1.f, -1.f);
    मूल्य_mappings.push (m);

    m.set_mapping (0.f, 0.f);
    मूल्य_mappings.push (m);

    m.set_mapping (1.f, 1.f);
    मूल्य_mappings.push (m);

    मूल्य_mappings.क्विक ();

    जर (unlikely (मूल्य_mappings.in_error ()))
      return_trace (असत्य);

    साठी (स्थिरांक स्वतः& _ : मूल्य_mappings)
    {
      जर (!_.serialize (c->serializer))
        return_trace (असत्य);
    }
    जर (!c->serializer->check_assign (out->len, मूल्य_mappings.length, HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (असत्य);
    /* Hand the instantiated mappings to the caller; avar2 offset
     * compensation needs them to locate the new mapping's kinks. */
    जर (out_mappings)
      *out_mappings = std::move (मूल्य_mappings);
    return_trace (सत्य);
  }

  सार्वजनिक:
  DEFINE_SIZE_ARRAY (2, *हे);
};

/* One knot of the avar2 offset-compensation function
 * offset(z) = inv_renorm(z) - z, in new intermediate space. */
संरचना avar2_offset_knot_t
{
  द्विगुण z;
  द्विगुण offset;
};

/* Insert a knot keeping the vector sorted by z; first insertion wins on
 * duplicate z. */
स्थिर ओळीत शून्य
_avar2_add_knot (hb_vector_t<avar2_offset_knot_t> &knots, द्विगुण z, द्विगुण offset)
{
  अचिन्हित i = 0;
  पर्यंत (i < knots.length && knots.arrayZ[i].z < z) i++;
  जर (i < knots.length && knots.arrayZ[i].z == z) परत;
  knots.push (avar2_offset_knot_t {0.0, 0.0});
  जर (unlikely (knots.in_error ())) परत;
  साठी (अचिन्हित j = knots.length - 1; j > i; j--)
    knots.arrayZ[j] = knots.arrayZ[j - 1];
  knots.arrayZ[i] = avar2_offset_knot_t {z, offset};
}

/* Piecewise-linear evaluation over the sorted knots. Inputs are always
 * within [-1, +1] and anchor knots at -1/0/+1 always exist. */
स्थिर ओळीत द्विगुण
_avar2_eval_offset (स्थिरांक hb_vector_t<avar2_offset_knot_t> &knots, द्विगुण z)
{
  अचिन्हित len = knots.length;
  साठी (अचिन्हित i = 0; i < len; i++)
  {
    जर (z == knots.arrayZ[i].z) परत knots.arrayZ[i].offset;
    जर (z < knots.arrayZ[i].z)
    {
      जर (!i) परत knots.arrayZ[0].offset;
      स्थिरांक स्वतः &before = knots.arrayZ[i - 1];
      स्थिरांक स्वतः &after = knots.arrayZ[i];
      द्विगुण denom = after.z - before.z;
      परत before.offset + (after.offset - before.offset) * (z - before.z) / denom;
    }
  }
  परत len ? knots.arrayZ[len - 1].offset : 0.0;
}

/* Piecewise-linear evaluation over instantiated avar v1 mappings, as
 * produced by SegmentMaps::subset (sorted, with -1/0/+1 anchors).
 * Matches SegmentMaps::map_float for such well-formed mappings. */
स्थिर ओळीत द्विगुण
_avar2_map_new_mapping (स्थिरांक hb_vector_t<Axis_मूल्य_Map> &mappings, द्विगुण v,
			अचिन्हित from_offset = 0, अचिन्हित to_offset = 1)
{
  अचिन्हित len = mappings.length;
  जर (!len) परत v;

  साठी (अचिन्हित i = 0; i < len; i++)
  {
    द्विगुण from = (द्विगुण) mappings.arrayZ[i].coords[from_offset].to_float ();
    जर (v == from)
      परत (द्विगुण) mappings.arrayZ[i].coords[to_offset].to_float ();
    जर (v < from)
    {
      द्विगुण to = (द्विगुण) mappings.arrayZ[i].coords[to_offset].to_float ();
      जर (!i) परत v - from + to;
      द्विगुण prev_from = (द्विगुण) mappings.arrayZ[i - 1].coords[from_offset].to_float ();
      द्विगुण prev_to = (द्विगुण) mappings.arrayZ[i - 1].coords[to_offset].to_float ();
      द्विगुण denom = from - prev_from;
      जर (denom == 0.0) परत prev_to;
      परत prev_to + (to - prev_to) * (v - prev_from) / denom;
    }
  }
  परत v - (द्विगुण) mappings.arrayZ[len - 1].coords[from_offset].to_float ()
           + (द्विगुण) mappings.arrayZ[len - 1].coords[to_offset].to_float ();
}

/* Inverse of _avar2_map_new_mapping: pull an output coordinate back to a
 * preimage input coordinate. For a non-strictly-monotone mapping this picks
 * one preimage, which is fine for its only use (augmenting the error
 * estimator's sample set). */
स्थिर ओळीत द्विगुण
_avar2_unmap_new_mapping (स्थिरांक hb_vector_t<Axis_मूल्य_Map> &mappings, द्विगुण v)
{
  परत _avar2_map_new_mapping (mappings, v, 1, 0);
}

/* Plain (non-avar) fvar-style normalization of a user value against a
 * user-space (min, default, max) triple. */
स्थिर ओळीत द्विगुण
avar2_normalize_मूल्य (द्विगुण v, द्विगुण min, द्विगुण def, द्विगुण max)
{
  v = hb_clamp (v, min, max);
  जर (v == def) परत 0.0;
  जर (v < def) परत def == min ? 0.0 : (v - def) / (def - min);
  परत def == max ? 0.0 : (v - def) / (max - def);
}

/* Inverse of the above: map a normalized value back to user space. */
स्थिर ओळीत द्विगुण
avar2_denormalize_मूल्य (द्विगुण v, द्विगुण min, द्विगुण def, द्विगुण max)
{
  जर (v == 0.0) परत def;
  परत v < 0.0 ? def + v * (def - min) : def + v * (max - def);
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
स्थिर ओळीत अचिन्हित
_avar2_estimate_offset_error (स्थिरांक SegmentMaps &old_seg,
                              स्थिरांक hb_vector_t<Axis_मूल्य_Map> &new_mapping,
                              द्विगुण old_min, द्विगुण old_def, द्विगुण old_max,
                              द्विगुण new_min, द्विगुण new_def, द्विगुण new_max,
                              स्थिरांक hb_vector_t<avar2_offset_knot_t> &knots)
{
  constexpr अचिन्हित samples = 257;
  hb_vector_t<द्विगुण> us;
  जर (unlikely (!us.alloc (samples + old_seg.as_array ().length +
                           new_mapping.length + knots.length)))
    परत अच_पूर्णांक_उच्च; /* estimator only; fail towards "worse" */
  साठी (अचिन्हित i = 0; i < samples; i++)
    us.push (new_min + (new_max - new_min) * i / (samples - 1));
  साठी (स्थिरांक स्वतः &_ : old_seg.as_array ())
    us.push (avar2_denormalize_मूल्य ((द्विगुण) _.coords[0].to_float (),
                                       old_min, old_def, old_max));
  साठी (स्थिरांक स्वतः &_ : new_mapping)
    us.push (avar2_denormalize_मूल्य ((द्विगुण) _.coords[0].to_float (),
                                       new_min, new_def, new_max));
  साठी (स्थिरांक स्वतः &knot : knots)
    us.push (avar2_denormalize_मूल्य (
               _avar2_unmap_new_mapping (new_mapping, knot.z),
               new_min, new_def, new_max));
  जर (unlikely (us.in_error ())) परत अच_पूर्णांक_उच्च;

  अचिन्हित max_err = 0;
  साठी (द्विगुण u : us)
  {
    जर (u < new_min || u > new_max) सुरूठेवा;
    द्विगुण n_old = avar2_normalize_मूल्य (u, old_min, old_def, old_max);
    द्विगुण old_final = (द्विगुण) old_seg.map_float ((दशांश) n_old);
    द्विगुण n_new = avar2_normalize_मूल्य (u, new_min, new_def, new_max);
    द्विगुण z = _avar2_map_new_mapping (new_mapping, n_new);
    द्विगुण new_final = z + _avar2_eval_offset (knots, z);
    पूर्णांक err = असल ((पूर्णांक) roundf ((दशांश) (old_final * 16384.0)) -
                   (पूर्णांक) roundf ((दशांश) (new_final * 16384.0)));
    जर ((अचिन्हित) err > max_err) max_err = err;
  }
  परत max_err;
}

संरचना avar
{
  स्थिर constexpr hb_tag_t tableTag = HB_OT_TAG_avar;

  तार्किक has_data () स्थिरांक { परत version.to_int (); }

  स्थिरांक SegmentMaps* get_segment_maps () स्थिरांक
  { परत &firstAxisSegmentMaps; }

  अचिन्हित get_axis_मोजणी () स्थिरांक
  { परत axis_मोजणी_2; }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    जर (!(version.sanitize (c) &&
	  hb_barrier () &&
	  (version.major == 1
#ifndef HB_NO_AVAR2
	   || version.major == 2
#endif
	   ) &&
	  c->check_struct (हे)))
      return_trace (असत्य);

    स्थिरांक SegmentMaps *map = &firstAxisSegmentMaps;
    अचिन्हित पूर्णांक मोजणी = axis_मोजणी_2;
    साठी (अचिन्हित पूर्णांक i = 0; i < मोजणी; i++)
    {
      जर (unlikely (!map->sanitize (c)))
	return_trace (असत्य);
      map = &StructAfter<SegmentMaps> (*map);
    }

#ifndef HB_NO_AVAR2
    जर (version.major < 2)
      return_trace (सत्य);
    hb_barrier ();

    स्थिरांक स्वतः &v2 = * (स्थिरांक avarV2Tail *) map;
    जर (unlikely (!v2.sanitize (c, हे)))
      return_trace (असत्य);
#endif

    return_trace (सत्य);
  }

  शून्य map_coords_16_16 (पूर्णांक *coords, अचिन्हित पूर्णांक coords_length) स्थिरांक
  {
    अचिन्हित पूर्णांक मोजणी = hb_min (coords_length, axis_मोजणी_2);

    स्थिरांक SegmentMaps *map = &firstAxisSegmentMaps;
    साठी (अचिन्हित पूर्णांक i = 0; i < मोजणी; i++)
    {
      coords[i] = roundf (map->map_float (coords[i] / 65536.f) * 65536.f);
      map = &StructAfter<SegmentMaps> (*map);
    }

#ifndef HB_NO_AVAR2
    जर (version.major < 2)
      परत;
    hb_barrier ();

    साठी (; मोजणी < axis_मोजणी_2; मोजणी++)
      map = &StructAfter<SegmentMaps> (*map);

    स्थिरांक स्वतः &v2 = * (स्थिरांक avarV2Tail *) map;

    स्थिरांक स्वतः &varidx_map = हे+v2.varIdxMap;
    स्थिरांक स्वतः &var_store = हे+v2.varStore;
    स्वतः *var_store_cache = var_store.create_cache ();

    hb_vector_t<पूर्णांक> coords_2_14;
    coords_2_14.resize (coords_length);
    साठी (अचिन्हित i = 0; i < coords_length; i++)
      coords_2_14[i] = roundf (coords[i] / 4.f); // 16.16 -> 2.14

    hb_vector_t<पूर्णांक> out;
    out.alloc (coords_length);
    साठी (अचिन्हित i = 0; i < coords_length; i++)
    {
      पूर्णांक v = coords[i];
      uint32_t varidx = varidx_map.map (i);
      दशांश delta = var_store.get_delta (varidx, coords_2_14.arrayZ, coords_2_14.length, var_store_cache);
      /* Apply the delta unclamped and clamp only the result to [-1, +1],
       * matching fontTools. Since inputs and results are in [-1, +1],
       * deltas beyond ±2 are equivalent to ±2; clamp to that range only
       * to keep the float->int conversion safe. */
      दशांश d = hb_clamp (delta * 4, -(दशांश) (1<<17), +(दशांश) (1<<17)); // 2.14 -> 16.16
      v += (पूर्णांक) roundf (d);
      v = hb_clamp (v, -(1<<16), +(1<<16));
      out.push (v);
    }
    साठी (अचिन्हित i = 0; i < coords_length; i++)
      coords[i] = out[i];

    OT::ItemVariationStore::destroy_cache (var_store_cache);
#endif
  }

  /* An avar version 2 font is never downgraded to v1: even with a NULL
   * varStore offset (legal; maps like plain v1) the avar2 subsetting path
   * applies, with "no variation" semantics for rows that don't resolve —
   * offset compensation then creates delta rows on demand. */
  तार्किक has_v2_data () स्थिरांक { परत version.major > 1; }

  /* Resolve the avar2 VarStore and VarIdxMap, for the subset planner.
   * Either pointer may be to the Null object (nullable offsets). */
  तार्किक get_v2_store_and_map (स्थिरांक ItemVariationStore **store,
                             स्थिरांक DeltaSetIndexMap **varidx_map) स्थिरांक
  {
#ifndef HB_NO_AVAR2
    जर (version.major < 2) परत असत्य;
    स्थिरांक SegmentMaps *map = &firstAxisSegmentMaps;
    साठी (अचिन्हित i = 0; i < axis_मोजणी_2; i++)
      map = &StructAfter<SegmentMaps> (*map);
    स्थिरांक स्वतः &v2 = * (स्थिरांक avarV2Tail *) map;
    *store = &(हे+v2.varStore);
    *varidx_map = &(हे+v2.varIdxMap);
    परत सत्य;
#अन्यथा
    परत असत्य;
#endif
  }

  // axis normalization is done in 2.14 here
  // TODO: deprecate this API once fonttools is updated to use 16.16 normalization
  तार्किक map_coords_2_14 (दशांश *coords, अचिन्हित पूर्णांक coords_length,
			तार्किक v1_only = असत्य) स्थिरांक
  {
    hb_vector_t<पूर्णांक> coords_2_14;
    जर (!v1_only && !coords_2_14.resize (coords_length)) परत असत्य;
    अचिन्हित पूर्णांक मोजणी = hb_min (coords_length, axis_मोजणी_2);

    स्थिरांक SegmentMaps *map = &firstAxisSegmentMaps;
    साठी (अचिन्हित पूर्णांक i = 0; i < मोजणी; i++)
    {
      पूर्णांक v = roundf (map->map_float (coords[i]) * 16384.f);
      जर (!v1_only)
	coords_2_14[i] = v;
      coords[i] = v / 16384.f;
      map = &StructAfter<SegmentMaps> (*map);
    }

    जर (v1_only)
      परत सत्य;

#ifndef HB_NO_AVAR2
    जर (version.major < 2)
      परत सत्य;
    hb_barrier ();

    साठी (; मोजणी < axis_मोजणी_2; मोजणी++)
      map = &StructAfter<SegmentMaps> (*map);

    स्थिरांक स्वतः &v2 = * (स्थिरांक avarV2Tail *) map;

    स्थिरांक स्वतः &varidx_map = हे+v2.varIdxMap;
    स्थिरांक स्वतः &var_store = हे+v2.varStore;
    स्वतः *var_store_cache = var_store.create_cache ();

    साठी (अचिन्हित i = 0; i < coords_length; i++)
    {
      पूर्णांक v = coords_2_14[i];
      uint32_t varidx = varidx_map.map (i);
      दशांश delta = var_store.get_delta (varidx, coords_2_14.arrayZ, coords_2_14.length, var_store_cache);
      /* As above: apply the delta unclamped (±2 covers every useful case)
       * and clamp the result to [-1, +1], matching fontTools. */
      v += (पूर्णांक) hb_clamp (roundf (delta), -(दशांश) (1<<15), +(दशांश) (1<<15));
      v = hb_clamp (v, -(1<<14), +(1<<14));
      coords[i] = v / 16384.f;
    }

    OT::ItemVariationStore::destroy_cache (var_store_cache);
    परत सत्य;
#अन्यथा
    परत version.major < 2;
#endif
  }

  तार्किक subset (hb_subset_context_t *c) स्थिरांक
  {
    TRACE_SUBSET (हे);
    अचिन्हित retained_axis_मोजणी = c->plan->axes_index_map.get_population ();
    जर (!retained_axis_मोजणी) //all axes are pinned/dropped
      return_trace (असत्य);

    avar *out = c->serializer->allocate_min<avar> ();
    जर (unlikely (!out)) return_trace (असत्य);

    out->version.major = c->plan->has_avar2 ? 2 : 1;
    out->version.minor = 0;
    जर (!c->serializer->check_assign (out->axis_मोजणी_2, retained_axis_मोजणी, HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (असत्य);

    /* For avar2, keep the instantiated v1 mappings around; offset
     * compensation needs them to locate the new mappings' kinks. */
    hb_vector_t<hb_vector_t<Axis_मूल्य_Map>> new_mappings;
    जर (c->plan->has_avar2 && !new_mappings.resize (axis_मोजणी_2))
      return_trace (असत्य);

    स्थिरांक hb_map_t& axes_index_map = c->plan->axes_index_map;
    स्थिरांक SegmentMaps *map = &firstAxisSegmentMaps;
    अचिन्हित मोजणी = axis_मोजणी_2;
    साठी (अचिन्हित पूर्णांक i = 0; i < मोजणी; i++)
    {
      जर (axes_index_map.has (i))
      {
        hb_tag_t *axis_tag;
        जर (!c->plan->axes_old_index_tag_map.has (i, &axis_tag))
          return_trace (असत्य);

        Triple *axis_location;
        जर (c->plan->has_avar2 &&
            c->plan->user_axes_location.has (*axis_tag, &axis_location) &&
            axis_location->is_point ())
        {
          /* Pinned axis in avar2 mode: serialize identity segment map
           * {-1->-1, 0->0, 1->1}. The axis is kept in fvar as hidden,
           * so avar needs a segment map entry for it. */
          स्वतः *identity_map = c->serializer->start_embed<SegmentMaps> ();
          जर (unlikely (!c->serializer->extend_min (identity_map)))
            return_trace (असत्य);
          Axis_मूल्य_Map m;
          m.set_mapping (-1.f, -1.f);
          जर (!m.serialize (c->serializer)) return_trace (असत्य);
          m.set_mapping (0.f, 0.f);
          जर (!m.serialize (c->serializer)) return_trace (असत्य);
          m.set_mapping (1.f, 1.f);
          जर (!m.serialize (c->serializer)) return_trace (असत्य);
          जर (!c->serializer->check_assign (identity_map->len, 3u,
                                            HB_SERIALIZE_ERROR_INT_OVERFLOW))
            return_trace (असत्य);
        }
        अन्यथा
        {
          /* Restricted or free axis: use standard SegmentMaps::subset() */
          जर (!map->subset (c, *axis_tag,
                            c->plan->has_avar2 ? &new_mappings[i] : nullptr))
            return_trace (असत्य);
        }
      }
      map = &StructAfter<SegmentMaps> (*map);
    }

    जर (c->plan->has_avar2)
      return_trace (_subset_avar2 (c, new_mappings));

    return_trace (सत्य);
  }

  खाजगी:
  संरचना avar2_index_map_plan_t
  {
    तार्किक init (स्थिरांक hb_vector_t<uint32_t> &varidx_mapping,
	       स्थिरांक hb_map_t &axes_index_map,
	       अचिन्हित axis_मोजणी)
    {
      जर (!output_map.alloc (axes_index_map.get_population ()))
	परत असत्य;

      तार्किक has_no_variation = असत्य;
      अचिन्हित max_outer = 0, max_inner = 0;
      साठी (अचिन्हित i = 0; i < axis_मोजणी; i++)
      {
	जर (!axes_index_map.has (i)) सुरूठेवा;
	uint32_t varidx = varidx_mapping[i];
	output_map.push (varidx);
	जर (varidx == HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
	{
	  has_no_variation = सत्य;
	  सुरूठेवा;
	}
	max_outer = hb_max (max_outer, varidx >> 16);
	max_inner = hb_max (max_inner, varidx & 0xFFFF);
      }
      जर (output_map.in_error () ||
	  output_map.length != axes_index_map.get_population ())
	परत असत्य;

      जर (has_no_variation)
      {
	width = 4;
	inner_bit_मोजणी = 16;
      }
      अन्यथा
      {
	inner_bit_मोजणी = hb_max (1u, hb_bit_storage (max_inner));
	अचिन्हित outer_bit_मोजणी = hb_max (1u, hb_bit_storage (max_outer));
	width = hb_clamp ((inner_bit_मोजणी + outer_bit_मोजणी + 7) / 8, 1u, 4u);
	जर (inner_bit_मोजणी + outer_bit_मोजणी > width * 8)
	  inner_bit_मोजणी = width * 8 - outer_bit_मोजणी;
      }
      परत सत्य;
    }

    अचिन्हित get_inner_bit_मोजणी () स्थिरांक { परत inner_bit_मोजणी; }
    अचिन्हित get_width () स्थिरांक { परत width; }
    hb_array_t<स्थिरांक uint32_t> get_output_map () स्थिरांक { परत output_map.as_array (); }

    अचिन्हित inner_bit_मोजणी = 1;
    अचिन्हित width = 1;
    hb_vector_t<uint32_t> output_map;
  };

  तार्किक _subset_avar2 (hb_subset_context_t *c,
                      स्थिरांक hb_vector_t<hb_vector_t<Axis_मूल्य_Map>> &new_mappings) स्थिरांक
  {
#जर defined (HB_NO_VAR) || defined (HB_NO_AVAR2)
    /* Not reachable: the plan never sets has_avar2 in these configurations. */
    परत असत्य;
#अन्यथा

    /* 1. Locate original avar2 data, keeping per-axis old segment maps */
    hb_vector_t<स्थिरांक SegmentMaps *> old_seg_maps;
    जर (!old_seg_maps.alloc (axis_मोजणी_2)) परत असत्य;
    स्थिरांक SegmentMaps *map = &firstAxisSegmentMaps;
    साठी (अचिन्हित i = 0; i < axis_मोजणी_2; i++)
    {
      old_seg_maps.push (map);
      map = &StructAfter<SegmentMaps> (*map);
    }

    स्थिरांक स्वतः &v2 = * (स्थिरांक avarV2Tail *) map;
    स्थिरांक स्वतः &varidx_map = हे+v2.varIdxMap;
    स्थिरांक स्वतः &var_store = हे+v2.varStore;

    स्वतः fvar_axes = c->plan->source->table.fvar->get_axes ();

    /* 2. Compute default deltas by evaluating VarStore at old defaults */
    hb_vector_t<पूर्णांक> default_coords;
    जर (!default_coords.resize (axis_मोजणी_2)) परत असत्य;
    साठी (अचिन्हित i = 0; i < axis_मोजणी_2; i++)
    {
      hb_tag_t *axis_tag;
      जर (c->plan->axes_old_index_tag_map.has (i, &axis_tag) &&
          c->plan->old_intermediates.has (*axis_tag))
      {
        दशांश d_i = (दशांश) c->plan->old_intermediates.get (*axis_tag).मधला;
        default_coords[i] = roundf (d_i * 16384.f);
      }
      अन्यथा
        default_coords[i] = 0;
    }

    स्वतः *store_cache = var_store.create_cache ();
    hb_vector_t<दशांश> default_deltas;
    जर (!default_deltas.resize (axis_मोजणी_2)) {
      ItemVariationStore::destroy_cache (store_cache);
      परत असत्य;
    }
    साठी (अचिन्हित i = 0; i < axis_मोजणी_2; i++)
    {
      uint32_t varidx = varidx_map.map (i);
      जर (varidx == HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
        default_deltas[i] = 0.f;
      अन्यथा
        default_deltas[i] = var_store.get_delta (varidx, default_coords.arrayZ,
                                                  default_coords.length, store_cache);
    }
    ItemVariationStore::destroy_cache (store_cache);

    /* 3. Rebase IVS regions */
    item_variations_t item_vars;
    जर (!item_vars.create_from_item_varstore (var_store, c->plan->axes_old_index_tag_map))
      परत असत्य;
    जर (!item_vars.instantiate_tuple_vars (c->plan->old_intermediates,
                                           c->plan->axes_triple_distances,
                                           असत्य))
      परत असत्य;

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
    जर (!new_varidx_mapping.resize (axis_मोजणी_2)) परत असत्य;
    साठी (अचिन्हित i = 0; i < axis_मोजणी_2; i++)
    {
      uint32_t varidx = varidx_map.map (i);
      जर (varidx != HB_OT_LAYOUT_NO_VARIATIONS_INDEX &&
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
    hb_hashmap_t<uint32_t, अचिन्हित> varidx_ref_मोजणी;
    साठी (अचिन्हित i = 0; i < axis_मोजणी_2; i++)
    {
      जर (!c->plan->axes_index_map.has (i)) सुरूठेवा; /* self-contained: dropped */
      uint32_t varidx = new_varidx_mapping[i];
      जर (varidx == HB_OT_LAYOUT_NO_VARIATIONS_INDEX) सुरूठेवा;
      अचिन्हित *मोजणी;
      जर (varidx_ref_मोजणी.has (varidx, &मोजणी))
        (*मोजणी)++;
      अन्यथा जर (!varidx_ref_मोजणी.set (varidx, 1))
        परत असत्य;
    }
    साठी (अचिन्हित i = 0; i < axis_मोजणी_2; i++)
    {
      hb_tag_t *axis_tag_ptr;
      जर (!c->plan->axes_old_index_tag_map.has (i, &axis_tag_ptr))
        परत असत्य;
      /* Only axes that will receive offset compensation (restricted or
       * pinned) can contaminate a shared row. */
      जर (!c->plan->axes_index_map.has (i) ||
          !c->plan->user_axes_location.has (*axis_tag_ptr))
        सुरूठेवा;
      uint32_t varidx = new_varidx_mapping[i];
      जर (varidx == HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
        सुरूठेवा; /* gets a fresh, private VarData in the offset loop below */
      अचिन्हित *मोजणी;
      जर (!varidx_ref_मोजणी.has (varidx, &मोजणी) || *मोजणी <= 1)
        सुरूठेवा; /* sole owner: safe to write offsets in place */
      अचिन्हित outer = varidx >> 16;
      अचिन्हित new_inner = item_vars.duplicate_row (outer, varidx & 0xFFFF);
      जर (unlikely (new_inner == (अचिन्हित) -1)) परत असत्य;
      new_varidx_mapping[i] = (outer << 16) | new_inner;
      (*मोजणी)--;
    }

    /* 6. Add offset compensation tuples.
     * Track processed (outer,inner) pairs to avoid adding duplicate biases
     * when multiple axes share the same varIdx. */
    hb_set_t processed_varidxes;
    साठी (अचिन्हित i = 0; i < axis_मोजणी_2; i++)
    {
      hb_tag_t *axis_tag_ptr;
      जर (!c->plan->axes_old_index_tag_map.has (i, &axis_tag_ptr))
        परत असत्य;
      hb_tag_t axis_tag = *axis_tag_ptr;

      /* Self-contained pinned axes are removed from fvar/avar; their
       * contribution is baked into the variation tables instead. */
      जर (!c->plan->axes_index_map.has (i))
        सुरूठेवा;

      Triple *new_user;
      जर (c->plan->user_axes_location.has (axis_tag, &new_user))
      {
        /* This axis is being restricted or pinned */
        Triple *old_int;
        जर (!c->plan->old_intermediates.has (axis_tag, &old_int))
          परत असत्य;

        दशांश a_i = (दशांश) old_int->minimum;
        दशांश d_i = (दशांश) old_int->मधला;
        दशांश b_i = (दशांश) old_int->maximum;

        पूर्णांक d_int = roundf (d_i * 16384.f);

        तार्किक is_pinned = new_user->is_point ();

        uint32_t varidx = new_varidx_mapping[i];
        अचिन्हित outer, inner, item_मोजणी_2;

        जर (varidx == HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
        {
          /* No existing avar2 mapping. Create new VarData. */
          outer = item_vars.add_vardata (1);
          inner = 0;
          item_मोजणी_2 = 1;
          new_varidx_mapping[i] = (outer << 16) | inner;
          default_deltas[i] = 0.f; /* no prior default delta */
        }
        अन्यथा
        {
          outer = varidx >> 16;
          inner = varidx & 0xFFFF;
          item_मोजणी_2 = item_vars.get_item_मोजणी (outer);
        }

        /* Empty-region bias: d_int + round(defaultDelta) */
        पूर्णांक bias = d_int + (पूर्णांक) roundf (default_deltas[i]);
        जर (bias != 0)
        {
          hb_hashmap_t<hb_tag_t, Triple> empty_region;
          item_vars.add_tuple (outer, std::move (empty_region),
                               inner, bias, item_मोजणी_2);
        }

        जर (!is_pinned)
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
          _avar2_add_knot (knots, -1.0, (द्विगुण) a_i + 1.0);
          _avar2_add_knot (knots, 0.0, (द्विगुण) d_i);
          _avar2_add_knot (knots, 1.0, (द्विगुण) b_i - 1.0);

          स्थिरांक hb_vector_t<Axis_मूल्य_Map> &new_mapping = new_mappings[i];

          दशांश min_f = 0.f, def_f = 0.f, max_f = 0.f;
          जर (likely (i < fvar_axes.length))
            fvar_axes[i].get_coordinates (min_f, def_f, max_f);
          द्विगुण old_min = (द्विगुण) min_f;
          द्विगुण old_def = (द्विगुण) def_f;
          द्विगुण old_max = (द्विगुण) max_f;

          /* If the axis default MOVED, inv_renorm also kinks where the OLD
           * default lands in the new space (the old intermediate coordinate
           * crosses 0 there), at
           *   z = z_old  ->  -z_old
           * Omitting that knot (as a plain two-tent encoding would) makes
           * interior coordinates wrong. */
          द्विगुण z_old = avar2_normalize_मूल्य ((द्विगुण) old_def,
                                                 (द्विगुण) new_user->minimum,
                                                 (द्विगुण) new_user->मधला,
                                                 (द्विगुण) new_user->maximum);
          z_old = _avar2_map_new_mapping (new_mapping, z_old);
          z_old = (द्विगुण) roundf ((दशांश) (z_old * 16384.0)) / 16384.0;
          जर (-1.0 < z_old && z_old < 1.0)
            _avar2_add_knot (knots, z_old, -z_old);

          /* Interior avar v1 breakpoints inside the retained range each put
           * a kink in offset(z). Sampling only {-1, 0, +1, z_old} would
           * linearly interpolate across those kinks. The instantiated
           * mapping keeps exactly the in-range old breakpoints, and the new
           * mapping kinks at each one's output coordinate; add that z with
           * its old intermediate value so offset(z) is reproduced at every
           * kink. */
          hb_vector_t<avar2_offset_knot_t> with_breakpoints (knots);
          साठी (स्थिरांक स्वतः &m : new_mapping)
          {
            द्विगुण from = (द्विगुण) m.coords[0].to_float ();
            जर (from == -1.0 || from == 0.0 || from == 1.0)
              सुरूठेवा; /* anchors already seeded */
            द्विगुण z = (द्विगुण) m.coords[1].to_float ();
            जर (!(-1.0 < z && z < 1.0))
              सुरूठेवा;
            द्विगुण user = avar2_denormalize_मूल्य (from,
                                                    (द्विगुण) new_user->minimum,
                                                    (द्विगुण) new_user->मधला,
                                                    (द्विगुण) new_user->maximum);
            द्विगुण n_old = avar2_normalize_मूल्य (user, old_min, old_def, old_max);
            द्विगुण x_old = (द्विगुण) old_seg_maps[i]->map_float ((दशांश) n_old);
            x_old = (द्विगुण) roundf ((दशांश) (x_old * 16384.0)) / 16384.0;
            _avar2_add_knot (with_breakpoints, z, x_old - z);
          }

          जर (unlikely (knots.in_error () || with_breakpoints.in_error ()))
            परत असत्य;

          /* Extra tents cost F2Dot14 rounding, so for a steep segment they
           * can add more quantization noise than the structural error they
           * remove. Keep the interior breakpoints only when they do not
           * increase the estimated residual; this makes the collection a
           * strict (never-worse) improvement over the {-1, 0, +1, z_old}
           * anchors. Warn when even the better choice is not bit-exact (a
           * steep retained segment that cannot be reproduced in F2Dot14). */
          अचिन्हित err = _avar2_estimate_offset_error (*old_seg_maps[i], new_mapping,
                                                       old_min, old_def, old_max,
                                                       (द्विगुण) new_user->minimum,
                                                       (द्विगुण) new_user->मधला,
                                                       (द्विगुण) new_user->maximum,
                                                       knots);
          जर (with_breakpoints.length != knots.length)
          {
            अचिन्हित err_with = _avar2_estimate_offset_error (*old_seg_maps[i], new_mapping,
                                                              old_min, old_def, old_max,
                                                              (द्विगुण) new_user->minimum,
                                                              (द्विगुण) new_user->मधला,
                                                              (द्विगुण) new_user->maximum,
                                                              with_breakpoints);
            जर (err_with <= err)
            {
              knots = std::move (with_breakpoints);
              err = err_with;
            }
          }
          जर (err > 8)
            DEBUG_संदेश (SUBSET, nullptr,
                       "avar2 offset compensation is approximate for axis %c%c%c%c: "
                       "max residual %u F2Dot14 units",
                       HB_UNTAG (axis_tag), err);

          /* Synthesize tents. Adjacent tents evaluate to zero at each
           * other's peaks, so each knot's delta is offset(z) - offset(0);
           * the base value offset(0) = d_i is carried by the empty-region
           * bias above. This reduces to the classic pair of tents
           * (-1,-1,0) / (0,+1,+1) when the default is unchanged and there
           * are no interior knots. */
          साठी (अचिन्हित k = 0; k < knots.length; k++)
          {
            द्विगुण z = knots.arrayZ[k].z;
            जर (z == 0.0) सुरूठेवा;
            पूर्णांक delta = (पूर्णांक) roundf ((दशांश) ((knots.arrayZ[k].offset - (द्विगुण) d_i) * 16384.0));
            जर (!delta) सुरूठेवा;
            द्विगुण lower, upper;
            जर (z > 0.0)
            {
              lower = knots.arrayZ[k - 1].z;
              upper = k + 1 < knots.length ? knots.arrayZ[k + 1].z : z;
            }
            अन्यथा
            {
              upper = knots.arrayZ[k + 1].z;
              lower = k > 0 ? knots.arrayZ[k - 1].z : z;
            }
            hb_hashmap_t<hb_tag_t, Triple> region;
            जर (unlikely (!region.set (axis_tag, Triple (lower, z, upper))))
              परत असत्य;
            item_vars.add_tuple (outer, std::move (region),
                                 inner, delta, item_मोजणी_2);
          }
        }
      }
      अन्यथा
      {
        /* Free or private axis — not being restricted.
         * If it has a non-zero default delta, add it back as a bias.
         * Skip if this (outer,inner) was already processed (shared varIdx). */
        uint32_t varidx = new_varidx_mapping[i];
        जर (varidx == HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
          सुरूठेवा;

        जर (processed_varidxes.has (varidx))
          सुरूठेवा;
        processed_varidxes.add (varidx);

        अचिन्हित outer = varidx >> 16;
        अचिन्हित inner = varidx & 0xFFFF;
        पूर्णांक dd = (पूर्णांक) roundf (default_deltas[i]);
        जर (dd != 0)
        {
          अचिन्हित item_मोजणी_2 = item_vars.get_item_मोजणी (outer);
          hb_hashmap_t<hb_tag_t, Triple> empty_region;
          item_vars.add_tuple (outer, std::move (empty_region),
                               inner, dd, item_मोजणी_2);
        }
      }
    }

    /* 7. Finalize: build region list + convert to varstore */
    जर (!item_vars.build_region_list ()) परत असत्य;
    जर (!item_vars.as_item_varstore (सत्य /* optimize */,
                                     असत्य /* use_no_variation_idx */))
      परत असत्य;

    /* 8. Apply varidx_map optimization remapping */
    स्थिरांक स्वतः &opt_varidx_map = item_vars.get_varidx_map ();
    साठी (अचिन्हित i = 0; i < axis_मोजणी_2; i++)
    {
      uint32_t varidx = new_varidx_mapping[i];
      जर (varidx == HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
        सुरूठेवा;
      uint32_t *new_idx;
      जर (opt_varidx_map.has (varidx, &new_idx))
        new_varidx_mapping[i] = *new_idx;
    }

    /* 9. Serialize avarV2Tail. Entries cover the retained axes only;
     * self-contained pinned axes are removed from fvar. */
    avar2_index_map_plan_t index_map_plan;
    जर (!index_map_plan.init (new_varidx_mapping,
			      c->plan->axes_index_map,
			      axis_मोजणी_2))
      परत असत्य;

    स्वतः *tail = c->serializer->allocate_size<avarV2Tail> (avarV2Tail::static_size);
    जर (unlikely (!tail)) परत असत्य;
    जर (!tail->varIdxMap.serialize_serialize (c->serializer, index_map_plan))
      परत असत्य;
    जर (!tail->varStore.serialize_serialize (c->serializer,
					     item_vars.has_long_word (),
					     c->plan->axis_tags,
					     item_vars.get_region_list (),
					     item_vars.get_vardata_encodings ()))
      परत असत्य;

    परत सत्य;
#endif
  }

  सार्वजनिक:

  संरक्षित:
  FixedVersion<>version;	/* Version of the avar table
				 * initially set to 0x00010000u */
  HBUINT16	reserved;	/* This field is permanently reserved. Set to 0. */
  HBUINT16	axis_मोजणी_2;	/* The number of variation axes in the font. This
				 * must be the same number as axisCount in the
				 * 'fvar' table. */
  SegmentMaps	firstAxisSegmentMaps;

  सार्वजनिक:
  DEFINE_SIZE_MIN (8);
};

} /* namespace OT */


#endif /* HB_OT_VAR_AVAR_TABLE_HH */
