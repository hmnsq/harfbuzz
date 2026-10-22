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
  friend संरचना avar;

  तार्किक sanitize (hb_sanitize_context_t *c,
		 ध्रुव शून्य *base) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (varIdxMap.sanitize (c, base) &&
		  varStore.sanitize (c, base));
  }

  रक्षित:
  Offset32To<DeltaSetIndexMap>	varIdxMap;	/* Offset from the beginning of 'avar' table. */
  Offset32To<ItemVariationStore>	varStore;	/* Offset from the beginning of 'avar' table. */

  सार्वजनिक:
  DEFINE_SIZE_STATIC (8);
};


संरचना Axis_मानम्_Map
{
  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्));
  }

  शून्य set_mapping (भिन्न from_coord, भिन्न to_coord)
  {
    coords[0].set_float (from_coord);
    coords[1].set_float (to_coord);
  }

  तार्किक is_outside_axis_range (ध्रुव Triple& axis_range) ध्रुव
  {
    द्विगुण from_coord = (द्विगुण) coords[0].to_float ();
    पटिनिवत्त !axis_range.contains (from_coord);
  }

  तार्किक must_include () ध्रुव
  {
    भिन्न from_coord = coords[0].to_float ();
    भिन्न to_coord = coords[1].to_float ();
    पटिनिवत्त (from_coord == -1.f && to_coord == -1.f) ||
           (from_coord == 0.f && to_coord == 0.f) ||
           (from_coord == 1.f && to_coord == 1.f);
  }

  शून्य instantiate (ध्रुव Triple& axis_range,
                    ध्रुव Triple& unmapped_range,
                    ध्रुव TripleDistances& triple_distances)
  {
    भिन्न from_coord = coords[0].to_float ();
    भिन्न to_coord = coords[1].to_float ();

    from_coord = renormalize_मानम् ((द्विगुण) from_coord, unmapped_range, triple_distances);
    to_coord = renormalize_मानम् ((द्विगुण) to_coord, axis_range, triple_distances);

    /* Malformed input can renormalize outside [-1,1], which would
     * overflow F2DOT14. */
    from_coord = hb_clamp (from_coord, -1.f, 1.f);
    to_coord = hb_clamp (to_coord, -1.f, 1.f);

    coords[0].set_float (from_coord);
    coords[1].set_float (to_coord);
  }

  HB_INTERNAL स्थिर पूर्णाङ्क cmp (ध्रुव शून्य *pa, ध्रुव शून्य *pb)
  {
    ध्रुव Axis_मानम्_Map *a = (ध्रुव Axis_मानम्_Map *) pa;
    ध्रुव Axis_मानम्_Map *b = (ध्रुव Axis_मानम्_Map *) pb;

    पूर्णाङ्क a_from = a->coords[0].to_int ();
    पूर्णाङ्क b_from = b->coords[0].to_int ();
    सचे (a_from != b_from)
      पटिनिवत्त a_from - b_from;

    /* this should never be reached. according to the spec, all of the axis
     * value map records for a given axis must have different fromCoord values
     * */
    पूर्णाङ्क a_to = a->coords[1].to_int ();
    पूर्णाङ्क b_to = b->coords[1].to_int ();
    पटिनिवत्त a_to - b_to;
  }

  तार्किक serialize (hb_serialize_context_t *c) ध्रुव
  {
    TRACE_SERIALIZE (एतत्);
    return_trace (c->embed (एतत्));
  }

  सार्वजनिक:
  F2DOT14	coords[2];
//   F2DOT14	fromCoord;	/* A normalized coordinate value obtained using
//				 * default normalization. */
//   F2DOT14	toCoord;	/* The modified, normalized coordinate value. */

  सार्वजनिक:
  DEFINE_SIZE_STATIC (4);
};

संरचना SegmentMaps : Array16Of<Axis_मानम्_Map>
{
  भिन्न map_float (भिन्न value, unsigned पूर्णाङ्क from_offset = 0, unsigned पूर्णाङ्क to_offset = 1) ध्रुव
  {
#define fromCoord coords[from_offset].to_float ()
#define toCoord coords[to_offset].to_float ()

    ध्रुव auto *map = arrayZ;

    /* The following special-cases are not part of OpenType, which requires
     * that at least -1, 0, and +1 must be mapped. But we include these as
     * part of a better error recovery scheme. */
    सचे (len < 2)
    {
      सचे (!len)
	पटिनिवत्त value;
      अञ्ञथा /* len == 1*/
	पटिनिवत्त value - map[0].fromCoord + map[0].toCoord;
    }

    // At least two mappings now.

    /* CoreText is wild...
     * PingFangUI avar needs all this special-casing...
     * So we implement an extended version of the spec here,
     * which is more robust and more likely to be compatible with
     * the wild. */

    unsigned start = 0;
    unsigned end = len;
    सचे (map[start].fromCoord == -1 && map[start].toCoord == -1 && map[start+1].fromCoord == -1)
      start++;
    सचे (map[end-1].fromCoord == +1 && map[end-1].toCoord == +1 && map[end-2].fromCoord == +1)
      end--;

    /* Look for exact match first, and do lots of special-casing. */
    unsigned i;
    अत्थाय (i = start; i < end; i++)
      सचे (value == map[i].fromCoord)
	भङ्ग;
    सचे (i < end)
    {
      // There's at least one exact match. See if there are more.
      unsigned j = i;
      अत्थाय (; j + 1 < end; j++)
	सचे (value != map[j + 1].fromCoord)
	  भङ्ग;

      // [i,j] inclusive are all exact matches:

      // If there's only one, return it. This is the only spec-compliant case.
      सचे (i == j)
	पटिनिवत्त map[i].toCoord;
      // If there's exactly three, return the middle one.
      सचे (i + 2 == j)
	पटिनिवत्त map[i + 1].toCoord;

      // Ignore the middle ones. Return the one mapping closer to 0.
      सचे (value < 0) पटिनिवत्त map[j].toCoord;
      सचे (value > 0) पटिनिवत्त map[i].toCoord;

      // Mapping 0? CoreText seems confused. It seems to prefer 0 here...
      // So we'll just return the smallest one. lol
      पटिनिवत्त fabsf (map[i].toCoord) < fabsf (map[j].toCoord) ? map[i].toCoord : map[j].toCoord;

      // Mapping 0? Return one not mapping to 0.
      सचे (map[i].toCoord == 0)
	पटिनिवत्त map[j].toCoord;
      अञ्ञथा
	पटिनिवत्त map[i].toCoord;
    }

    /* There's at least two and we're not an exact match. Prepare to lerp. */

    // Find the segment we're in.
    अत्थाय (i = start; i < end; i++)
      सचे (value < map[i].fromCoord)
	भङ्ग;

    सचे (i == start)
    {
      // Value before all segments; Shift.
      पटिनिवत्त value - map[start].fromCoord + map[start].toCoord;
    }
    सचे (i == end)
    {
      // Value after all segments; Shift.
      पटिनिवत्त value - map[end - 1].fromCoord + map[end - 1].toCoord;
    }

    // Actually interpolate.
    auto &before = map[i-1];
    auto &after = map[i];
    भिन्न denom = after.fromCoord - before.fromCoord; // Can't be zero by now.
    पटिनिवत्त before.toCoord + ((after.toCoord - before.toCoord) * (value - before.fromCoord)) / denom;

#undef toCoord
#undef fromCoord
  }

  भिन्न unmap_float (भिन्न value) ध्रुव { पटिनिवत्त map_float (value, 1, 0); }


  // TODO Kill this.
  Triple unmap_axis_range (ध्रुव Triple& axis_range) ध्रुव
  {
    भिन्न unmapped_min = unmap_float (axis_range.minimum);
    भिन्न unmapped_मध्यः = unmap_float (axis_range.मध्यः);
    भिन्न unmapped_max = unmap_float (axis_range.maximum);

    पटिनिवत्त Triple{unmapped_min, unmapped_मध्यः, unmapped_max};
  }

  तार्किक subset (hb_subset_context_t *c, hb_tag_t axis_tag,
               hb_vector_t<Axis_मानम्_Map> *out_mappings = nullptr) ध्रुव
  {
    TRACE_SUBSET (एतत्);

    /* This function cannot work on avar2 table (and currently doesn't).
     * We should instead keep the design coords in the shape plan and use
     * those. unmap_axis_range needs to be killed. */

    /* avar mapped normalized axis range. Under avar2, axes_location holds
     * only the self-contained pins for the other tables; avar itself uses
     * the intermediate-space ranges of all restricted axes. */
    ध्रुव auto &axes_location = c->plan->has_avar2
				? c->plan->old_intermediates
				: c->plan->axes_location;
    Triple *axis_range;
    सचे (!axes_location.has (axis_tag, &axis_range))
      पटिनिवत्त c->serializer->embed (*एतत्);

    TripleDistances *axis_triple_distances;
    सचे (!c->plan->axes_triple_distances.has (axis_tag, &axis_triple_distances))
      return_trace (मिच्छा);

    auto *out = c->serializer->start_embed (एतत्);
    सचे (unlikely (!c->serializer->extend_min (out))) return_trace (मिच्छा);

    Triple unmapped_range = unmap_axis_range (*axis_range);

    /* create a vector of retained mappings and sort */
    hb_vector_t<Axis_मानम्_Map> मानम्_mappings;
    अत्थाय (ध्रुव auto& _ : as_array ())
    {
      सचे (_.is_outside_axis_range (unmapped_range))
        अनुवर्त;
      Axis_मानम्_Map mapping;
      mapping = _;
      mapping.instantiate (*axis_range, unmapped_range, *axis_triple_distances);
      /* (-1, -1), (0, 0), (1, 1) mappings will be added later, so avoid
       * duplicates here */
      सचे (mapping.must_include ())
        अनुवर्त;
      मानम्_mappings.push (mapping);
    }

    Axis_मानम्_Map m;
    m.set_mapping (-1.f, -1.f);
    मानम्_mappings.push (m);

    m.set_mapping (0.f, 0.f);
    मानम्_mappings.push (m);

    m.set_mapping (1.f, 1.f);
    मानम्_mappings.push (m);

    मानम्_mappings.qsort ();

    सचे (unlikely (मानम्_mappings.in_error ()))
      return_trace (मिच्छा);

    अत्थाय (ध्रुव auto& _ : मानम्_mappings)
    {
      सचे (!_.serialize (c->serializer))
        return_trace (मिच्छा);
    }
    सचे (!c->serializer->check_assign (out->len, मानम्_mappings.length, HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (मिच्छा);
    /* Hand the instantiated mappings to the caller; avar2 offset
     * compensation needs them to locate the new mapping's kinks. */
    सचे (out_mappings)
      *out_mappings = std::move (मानम्_mappings);
    return_trace (सच्च);
  }

  सार्वजनिक:
  DEFINE_SIZE_ARRAY (2, *एतत्);
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
स्थिर अन्तःस्थ शून्य
_avar2_add_knot (hb_vector_t<avar2_offset_knot_t> &knots, द्विगुण z, द्विगुण offset)
{
  unsigned i = 0;
  याव (i < knots.length && knots.arrayZ[i].z < z) i++;
  सचे (i < knots.length && knots.arrayZ[i].z == z) पटिनिवत्त;
  knots.push (avar2_offset_knot_t {0.0, 0.0});
  सचे (unlikely (knots.in_error ())) पटिनिवत्त;
  अत्थाय (unsigned j = knots.length - 1; j > i; j--)
    knots.arrayZ[j] = knots.arrayZ[j - 1];
  knots.arrayZ[i] = avar2_offset_knot_t {z, offset};
}

/* Piecewise-linear evaluation over the sorted knots. Inputs are always
 * within [-1, +1] and anchor knots at -1/0/+1 always exist. */
स्थिर अन्तःस्थ द्विगुण
_avar2_eval_offset (ध्रुव hb_vector_t<avar2_offset_knot_t> &knots, द्विगुण z)
{
  unsigned len = knots.length;
  अत्थाय (unsigned i = 0; i < len; i++)
  {
    सचे (z == knots.arrayZ[i].z) पटिनिवत्त knots.arrayZ[i].offset;
    सचे (z < knots.arrayZ[i].z)
    {
      सचे (!i) पटिनिवत्त knots.arrayZ[0].offset;
      ध्रुव auto &before = knots.arrayZ[i - 1];
      ध्रुव auto &after = knots.arrayZ[i];
      द्विगुण denom = after.z - before.z;
      पटिनिवत्त before.offset + (after.offset - before.offset) * (z - before.z) / denom;
    }
  }
  पटिनिवत्त len ? knots.arrayZ[len - 1].offset : 0.0;
}

/* Piecewise-linear evaluation over instantiated avar v1 mappings, as
 * produced by SegmentMaps::subset (sorted, with -1/0/+1 anchors).
 * Matches SegmentMaps::map_float for such well-formed mappings. */
स्थिर अन्तःस्थ द्विगुण
_avar2_map_new_mapping (ध्रुव hb_vector_t<Axis_मानम्_Map> &mappings, द्विगुण v,
			unsigned from_offset = 0, unsigned to_offset = 1)
{
  unsigned len = mappings.length;
  सचे (!len) पटिनिवत्त v;

  अत्थाय (unsigned i = 0; i < len; i++)
  {
    द्विगुण from = (द्विगुण) mappings.arrayZ[i].coords[from_offset].to_float ();
    सचे (v == from)
      पटिनिवत्त (द्विगुण) mappings.arrayZ[i].coords[to_offset].to_float ();
    सचे (v < from)
    {
      द्विगुण to = (द्विगुण) mappings.arrayZ[i].coords[to_offset].to_float ();
      सचे (!i) पटिनिवत्त v - from + to;
      द्विगुण prev_from = (द्विगुण) mappings.arrayZ[i - 1].coords[from_offset].to_float ();
      द्विगुण prev_to = (द्विगुण) mappings.arrayZ[i - 1].coords[to_offset].to_float ();
      द्विगुण denom = from - prev_from;
      सचे (denom == 0.0) पटिनिवत्त prev_to;
      पटिनिवत्त prev_to + (to - prev_to) * (v - prev_from) / denom;
    }
  }
  पटिनिवत्त v - (द्विगुण) mappings.arrayZ[len - 1].coords[from_offset].to_float ()
           + (द्विगुण) mappings.arrayZ[len - 1].coords[to_offset].to_float ();
}

/* Inverse of _avar2_map_new_mapping: pull an output coordinate back to a
 * preimage input coordinate. For a non-strictly-monotone mapping this picks
 * one preimage, which is fine for its only use (augmenting the error
 * estimator's sample set). */
स्थिर अन्तःस्थ द्विगुण
_avar2_unmap_new_mapping (ध्रुव hb_vector_t<Axis_मानम्_Map> &mappings, द्विगुण v)
{
  पटिनिवत्त _avar2_map_new_mapping (mappings, v, 1, 0);
}

/* Plain (non-avar) fvar-style normalization of a user value against a
 * user-space (min, default, max) triple. */
स्थिर अन्तःस्थ द्विगुण
avar2_normalize_मानम् (द्विगुण v, द्विगुण min, द्विगुण def, द्विगुण max)
{
  v = hb_clamp (v, min, max);
  सचे (v == def) पटिनिवत्त 0.0;
  सचे (v < def) पटिनिवत्त def == min ? 0.0 : (v - def) / (def - min);
  पटिनिवत्त def == max ? 0.0 : (v - def) / (max - def);
}

/* Inverse of the above: map a normalized value back to user space. */
स्थिर अन्तःस्थ द्विगुण
avar2_denormalize_मानम् (द्विगुण v, द्विगुण min, द्विगुण def, द्विगुण max)
{
  सचे (v == 0.0) पटिनिवत्त def;
  पटिनिवत्त v < 0.0 ? def + v * (def - min) : def + v * (max - def);
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
स्थिर अन्तःस्थ unsigned
_avar2_estimate_offset_error (ध्रुव SegmentMaps &old_seg,
                              ध्रुव hb_vector_t<Axis_मानम्_Map> &new_mapping,
                              द्विगुण old_min, द्विगुण old_def, द्विगुण old_max,
                              द्विगुण new_min, द्विगुण new_def, द्विगुण new_max,
                              ध्रुव hb_vector_t<avar2_offset_knot_t> &knots)
{
  constexpr unsigned samples = 257;
  hb_vector_t<द्विगुण> us;
  सचे (unlikely (!us.alloc (samples + old_seg.as_array ().length +
                           new_mapping.length + knots.length)))
    पटिनिवत्त UINT_MAX; /* estimator only; fail towards "worse" */
  अत्थाय (unsigned i = 0; i < samples; i++)
    us.push (new_min + (new_max - new_min) * i / (samples - 1));
  अत्थाय (ध्रुव auto &_ : old_seg.as_array ())
    us.push (avar2_denormalize_मानम् ((द्विगुण) _.coords[0].to_float (),
                                       old_min, old_def, old_max));
  अत्थाय (ध्रुव auto &_ : new_mapping)
    us.push (avar2_denormalize_मानम् ((द्विगुण) _.coords[0].to_float (),
                                       new_min, new_def, new_max));
  अत्थाय (ध्रुव auto &knot : knots)
    us.push (avar2_denormalize_मानम् (
               _avar2_unmap_new_mapping (new_mapping, knot.z),
               new_min, new_def, new_max));
  सचे (unlikely (us.in_error ())) पटिनिवत्त UINT_MAX;

  unsigned max_err = 0;
  अत्थाय (द्विगुण u : us)
  {
    सचे (u < new_min || u > new_max) अनुवर्त;
    द्विगुण n_old = avar2_normalize_मानम् (u, old_min, old_def, old_max);
    द्विगुण old_final = (द्विगुण) old_seg.map_float ((भिन्न) n_old);
    द्विगुण n_new = avar2_normalize_मानम् (u, new_min, new_def, new_max);
    द्विगुण z = _avar2_map_new_mapping (new_mapping, n_new);
    द्विगुण new_final = z + _avar2_eval_offset (knots, z);
    पूर्णाङ्क err = abs ((पूर्णाङ्क) roundf ((भिन्न) (old_final * 16384.0)) -
                   (पूर्णाङ्क) roundf ((भिन्न) (new_final * 16384.0)));
    सचे ((unsigned) err > max_err) max_err = err;
  }
  पटिनिवत्त max_err;
}

संरचना avar
{
  स्थिर constexpr hb_tag_t tableTag = HB_OT_TAG_avar;

  तार्किक has_data () ध्रुव { पटिनिवत्त version.to_int (); }

  ध्रुव SegmentMaps* get_segment_maps () ध्रुव
  { पटिनिवत्त &firstAxisSegmentMaps; }

  unsigned get_axis_गणनम् () ध्रुव
  { पटिनिवत्त axis_गणनम्_2; }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    सचे (!(version.sanitize (c) &&
	  hb_barrier () &&
	  (version.major == 1
#ifndef HB_NO_AVAR2
	   || version.major == 2
#endif
	   ) &&
	  c->check_struct (एतत्)))
      return_trace (मिच्छा);

    ध्रुव SegmentMaps *map = &firstAxisSegmentMaps;
    unsigned पूर्णाङ्क गणनम् = axis_गणनम्_2;
    अत्थाय (unsigned पूर्णाङ्क i = 0; i < गणनम्; i++)
    {
      सचे (unlikely (!map->sanitize (c)))
	return_trace (मिच्छा);
      map = &StructAfter<SegmentMaps> (*map);
    }

#ifndef HB_NO_AVAR2
    सचे (version.major < 2)
      return_trace (सच्च);
    hb_barrier ();

    ध्रुव auto &v2 = * (ध्रुव avarV2Tail *) map;
    सचे (unlikely (!v2.sanitize (c, एतत्)))
      return_trace (मिच्छा);
#endif

    return_trace (सच्च);
  }

  शून्य map_coords_16_16 (पूर्णाङ्क *coords, unsigned पूर्णाङ्क coords_length) ध्रुव
  {
    unsigned पूर्णाङ्क गणनम् = hb_min (coords_length, axis_गणनम्_2);

    ध्रुव SegmentMaps *map = &firstAxisSegmentMaps;
    अत्थाय (unsigned पूर्णाङ्क i = 0; i < गणनम्; i++)
    {
      coords[i] = roundf (map->map_float (coords[i] / 65536.f) * 65536.f);
      map = &StructAfter<SegmentMaps> (*map);
    }

#ifndef HB_NO_AVAR2
    सचे (version.major < 2)
      पटिनिवत्त;
    hb_barrier ();

    अत्थाय (; गणनम् < axis_गणनम्_2; गणनम्++)
      map = &StructAfter<SegmentMaps> (*map);

    ध्रुव auto &v2 = * (ध्रुव avarV2Tail *) map;

    ध्रुव auto &varidx_map = एतत्+v2.varIdxMap;
    ध्रुव auto &var_store = एतत्+v2.varStore;
    auto *var_store_cache = var_store.create_cache ();

    hb_vector_t<पूर्णाङ्क> coords_2_14;
    coords_2_14.resize (coords_length);
    अत्थाय (unsigned i = 0; i < coords_length; i++)
      coords_2_14[i] = roundf (coords[i] / 4.f); // 16.16 -> 2.14

    hb_vector_t<पूर्णाङ्क> out;
    out.alloc (coords_length);
    अत्थाय (unsigned i = 0; i < coords_length; i++)
    {
      पूर्णाङ्क v = coords[i];
      uint32_t varidx = varidx_map.map (i);
      भिन्न delta = var_store.get_delta (varidx, coords_2_14.arrayZ, coords_2_14.length, var_store_cache);
      /* Apply the delta unclamped and clamp only the result to [-1, +1],
       * matching fontTools. Since inputs and results are in [-1, +1],
       * deltas beyond ±2 are equivalent to ±2; clamp to that range only
       * to keep the float->int conversion safe. */
      भिन्न d = hb_clamp (delta * 4, -(भिन्न) (1<<17), +(भिन्न) (1<<17)); // 2.14 -> 16.16
      v += (पूर्णाङ्क) roundf (d);
      v = hb_clamp (v, -(1<<16), +(1<<16));
      out.push (v);
    }
    अत्थाय (unsigned i = 0; i < coords_length; i++)
      coords[i] = out[i];

    OT::ItemVariationStore::destroy_cache (var_store_cache);
#endif
  }

  /* An avar version 2 font is never downgraded to v1: even with a NULL
   * varStore offset (legal; maps like plain v1) the avar2 subsetting path
   * applies, with "no variation" semantics for rows that don't resolve —
   * offset compensation then creates delta rows on demand. */
  तार्किक has_v2_data () ध्रुव { पटिनिवत्त version.major > 1; }

  /* Resolve the avar2 VarStore and VarIdxMap, for the subset planner.
   * Either pointer may be to the Null object (nullable offsets). */
  तार्किक get_v2_store_and_map (ध्रुव ItemVariationStore **store,
                             ध्रुव DeltaSetIndexMap **varidx_map) ध्रुव
  {
#ifndef HB_NO_AVAR2
    सचे (version.major < 2) पटिनिवत्त मिच्छा;
    ध्रुव SegmentMaps *map = &firstAxisSegmentMaps;
    अत्थाय (unsigned i = 0; i < axis_गणनम्_2; i++)
      map = &StructAfter<SegmentMaps> (*map);
    ध्रुव auto &v2 = * (ध्रुव avarV2Tail *) map;
    *store = &(एतत्+v2.varStore);
    *varidx_map = &(एतत्+v2.varIdxMap);
    पटिनिवत्त सच्च;
#अञ्ञथा
    पटिनिवत्त मिच्छा;
#endif
  }

  // axis normalization is done in 2.14 here
  // TODO: deprecate this API once fonttools is updated to use 16.16 normalization
  तार्किक map_coords_2_14 (भिन्न *coords, unsigned पूर्णाङ्क coords_length,
			तार्किक v1_only = मिच्छा) ध्रुव
  {
    hb_vector_t<पूर्णाङ्क> coords_2_14;
    सचे (!v1_only && !coords_2_14.resize (coords_length)) पटिनिवत्त मिच्छा;
    unsigned पूर्णाङ्क गणनम् = hb_min (coords_length, axis_गणनम्_2);

    ध्रुव SegmentMaps *map = &firstAxisSegmentMaps;
    अत्थाय (unsigned पूर्णाङ्क i = 0; i < गणनम्; i++)
    {
      पूर्णाङ्क v = roundf (map->map_float (coords[i]) * 16384.f);
      सचे (!v1_only)
	coords_2_14[i] = v;
      coords[i] = v / 16384.f;
      map = &StructAfter<SegmentMaps> (*map);
    }

    सचे (v1_only)
      पटिनिवत्त सच्च;

#ifndef HB_NO_AVAR2
    सचे (version.major < 2)
      पटिनिवत्त सच्च;
    hb_barrier ();

    अत्थाय (; गणनम् < axis_गणनम्_2; गणनम्++)
      map = &StructAfter<SegmentMaps> (*map);

    ध्रुव auto &v2 = * (ध्रुव avarV2Tail *) map;

    ध्रुव auto &varidx_map = एतत्+v2.varIdxMap;
    ध्रुव auto &var_store = एतत्+v2.varStore;
    auto *var_store_cache = var_store.create_cache ();

    अत्थाय (unsigned i = 0; i < coords_length; i++)
    {
      पूर्णाङ्क v = coords_2_14[i];
      uint32_t varidx = varidx_map.map (i);
      भिन्न delta = var_store.get_delta (varidx, coords_2_14.arrayZ, coords_2_14.length, var_store_cache);
      /* As above: apply the delta unclamped (±2 covers every useful case)
       * and clamp the result to [-1, +1], matching fontTools. */
      v += (पूर्णाङ्क) hb_clamp (roundf (delta), -(भिन्न) (1<<15), +(भिन्न) (1<<15));
      v = hb_clamp (v, -(1<<14), +(1<<14));
      coords[i] = v / 16384.f;
    }

    OT::ItemVariationStore::destroy_cache (var_store_cache);
    पटिनिवत्त सच्च;
#अञ्ञथा
    पटिनिवत्त version.major < 2;
#endif
  }

  तार्किक subset (hb_subset_context_t *c) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    unsigned retained_axis_गणनम् = c->plan->axes_index_map.get_population ();
    सचे (!retained_axis_गणनम्) //all axes are pinned/dropped
      return_trace (मिच्छा);

    avar *out = c->serializer->allocate_min<avar> ();
    सचे (unlikely (!out)) return_trace (मिच्छा);

    out->version.major = c->plan->has_avar2 ? 2 : 1;
    out->version.minor = 0;
    सचे (!c->serializer->check_assign (out->axis_गणनम्_2, retained_axis_गणनम्, HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (मिच्छा);

    /* For avar2, keep the instantiated v1 mappings around; offset
     * compensation needs them to locate the new mappings' kinks. */
    hb_vector_t<hb_vector_t<Axis_मानम्_Map>> new_mappings;
    सचे (c->plan->has_avar2 && !new_mappings.resize (axis_गणनम्_2))
      return_trace (मिच्छा);

    ध्रुव hb_map_t& axes_index_map = c->plan->axes_index_map;
    ध्रुव SegmentMaps *map = &firstAxisSegmentMaps;
    unsigned गणनम् = axis_गणनम्_2;
    अत्थाय (unsigned पूर्णाङ्क i = 0; i < गणनम्; i++)
    {
      सचे (axes_index_map.has (i))
      {
        hb_tag_t *axis_tag;
        सचे (!c->plan->axes_old_index_tag_map.has (i, &axis_tag))
          return_trace (मिच्छा);

        Triple *axis_location;
        सचे (c->plan->has_avar2 &&
            c->plan->user_axes_location.has (*axis_tag, &axis_location) &&
            axis_location->is_point ())
        {
          /* Pinned axis in avar2 mode: serialize identity segment map
           * {-1->-1, 0->0, 1->1}. The axis is kept in fvar as hidden,
           * so avar needs a segment map entry for it. */
          auto *identity_map = c->serializer->start_embed<SegmentMaps> ();
          सचे (unlikely (!c->serializer->extend_min (identity_map)))
            return_trace (मिच्छा);
          Axis_मानम्_Map m;
          m.set_mapping (-1.f, -1.f);
          सचे (!m.serialize (c->serializer)) return_trace (मिच्छा);
          m.set_mapping (0.f, 0.f);
          सचे (!m.serialize (c->serializer)) return_trace (मिच्छा);
          m.set_mapping (1.f, 1.f);
          सचे (!m.serialize (c->serializer)) return_trace (मिच्छा);
          सचे (!c->serializer->check_assign (identity_map->len, 3u,
                                            HB_SERIALIZE_ERROR_INT_OVERFLOW))
            return_trace (मिच्छा);
        }
        अञ्ञथा
        {
          /* Restricted or free axis: use standard SegmentMaps::subset() */
          सचे (!map->subset (c, *axis_tag,
                            c->plan->has_avar2 ? &new_mappings[i] : nullptr))
            return_trace (मिच्छा);
        }
      }
      map = &StructAfter<SegmentMaps> (*map);
    }

    सचे (c->plan->has_avar2)
      return_trace (_subset_avar2 (c, new_mappings));

    return_trace (सच्च);
  }

  गुप्त:
  संरचना avar2_index_map_plan_t
  {
    तार्किक init (ध्रुव hb_vector_t<uint32_t> &varidx_mapping,
	       ध्रुव hb_map_t &axes_index_map,
	       unsigned axis_गणनम्)
    {
      सचे (!output_map.alloc (axes_index_map.get_population ()))
	पटिनिवत्त मिच्छा;

      तार्किक has_no_variation = मिच्छा;
      unsigned max_outer = 0, max_inner = 0;
      अत्थाय (unsigned i = 0; i < axis_गणनम्; i++)
      {
	सचे (!axes_index_map.has (i)) अनुवर्त;
	uint32_t varidx = varidx_mapping[i];
	output_map.push (varidx);
	सचे (varidx == HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
	{
	  has_no_variation = सच्च;
	  अनुवर्त;
	}
	max_outer = hb_max (max_outer, varidx >> 16);
	max_inner = hb_max (max_inner, varidx & 0xFFFF);
      }
      सचे (output_map.in_error () ||
	  output_map.length != axes_index_map.get_population ())
	पटिनिवत्त मिच्छा;

      सचे (has_no_variation)
      {
	width = 4;
	inner_bit_गणनम् = 16;
      }
      अञ्ञथा
      {
	inner_bit_गणनम् = hb_max (1u, hb_bit_storage (max_inner));
	unsigned outer_bit_गणनम् = hb_max (1u, hb_bit_storage (max_outer));
	width = hb_clamp ((inner_bit_गणनम् + outer_bit_गणनम् + 7) / 8, 1u, 4u);
	सचे (inner_bit_गणनम् + outer_bit_गणनम् > width * 8)
	  inner_bit_गणनम् = width * 8 - outer_bit_गणनम्;
      }
      पटिनिवत्त सच्च;
    }

    unsigned get_inner_bit_गणनम् () ध्रुव { पटिनिवत्त inner_bit_गणनम्; }
    unsigned get_width () ध्रुव { पटिनिवत्त width; }
    hb_array_t<ध्रुव uint32_t> get_output_map () ध्रुव { पटिनिवत्त output_map.as_array (); }

    unsigned inner_bit_गणनम् = 1;
    unsigned width = 1;
    hb_vector_t<uint32_t> output_map;
  };

  तार्किक _subset_avar2 (hb_subset_context_t *c,
                      ध्रुव hb_vector_t<hb_vector_t<Axis_मानम्_Map>> &new_mappings) ध्रुव
  {
#सचे defined (HB_NO_VAR) || defined (HB_NO_AVAR2)
    /* Not reachable: the plan never sets has_avar2 in these configurations. */
    पटिनिवत्त मिच्छा;
#अञ्ञथा

    /* 1. Locate original avar2 data, keeping per-axis old segment maps */
    hb_vector_t<ध्रुव SegmentMaps *> old_seg_maps;
    सचे (!old_seg_maps.alloc (axis_गणनम्_2)) पटिनिवत्त मिच्छा;
    ध्रुव SegmentMaps *map = &firstAxisSegmentMaps;
    अत्थाय (unsigned i = 0; i < axis_गणनम्_2; i++)
    {
      old_seg_maps.push (map);
      map = &StructAfter<SegmentMaps> (*map);
    }

    ध्रुव auto &v2 = * (ध्रुव avarV2Tail *) map;
    ध्रुव auto &varidx_map = एतत्+v2.varIdxMap;
    ध्रुव auto &var_store = एतत्+v2.varStore;

    auto fvar_axes = c->plan->source->table.fvar->get_axes ();

    /* 2. Compute default deltas by evaluating VarStore at old defaults */
    hb_vector_t<पूर्णाङ्क> default_coords;
    सचे (!default_coords.resize (axis_गणनम्_2)) पटिनिवत्त मिच्छा;
    अत्थाय (unsigned i = 0; i < axis_गणनम्_2; i++)
    {
      hb_tag_t *axis_tag;
      सचे (c->plan->axes_old_index_tag_map.has (i, &axis_tag) &&
          c->plan->old_intermediates.has (*axis_tag))
      {
        भिन्न d_i = (भिन्न) c->plan->old_intermediates.get (*axis_tag).मध्यः;
        default_coords[i] = roundf (d_i * 16384.f);
      }
      अञ्ञथा
        default_coords[i] = 0;
    }

    auto *store_cache = var_store.create_cache ();
    hb_vector_t<भिन्न> default_deltas;
    सचे (!default_deltas.resize (axis_गणनम्_2)) {
      ItemVariationStore::destroy_cache (store_cache);
      पटिनिवत्त मिच्छा;
    }
    अत्थाय (unsigned i = 0; i < axis_गणनम्_2; i++)
    {
      uint32_t varidx = varidx_map.map (i);
      सचे (varidx == HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
        default_deltas[i] = 0.f;
      अञ्ञथा
        default_deltas[i] = var_store.get_delta (varidx, default_coords.arrayZ,
                                                  default_coords.length, store_cache);
    }
    ItemVariationStore::destroy_cache (store_cache);

    /* 3. Rebase IVS regions */
    item_variations_t item_vars;
    सचे (!item_vars.create_from_item_varstore (var_store, c->plan->axes_old_index_tag_map))
      पटिनिवत्त मिच्छा;
    सचे (!item_vars.instantiate_tuple_vars (c->plan->old_intermediates,
                                           c->plan->axes_triple_distances,
                                           मिच्छा))
      पटिनिवत्त मिच्छा;

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
    सचे (!new_varidx_mapping.resize (axis_गणनम्_2)) पटिनिवत्त मिच्छा;
    अत्थाय (unsigned i = 0; i < axis_गणनम्_2; i++)
    {
      uint32_t varidx = varidx_map.map (i);
      सचे (varidx != HB_OT_LAYOUT_NO_VARIATIONS_INDEX &&
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
    hb_hashmap_t<uint32_t, unsigned> varidx_ref_गणनम्;
    अत्थाय (unsigned i = 0; i < axis_गणनम्_2; i++)
    {
      सचे (!c->plan->axes_index_map.has (i)) अनुवर्त; /* self-contained: dropped */
      uint32_t varidx = new_varidx_mapping[i];
      सचे (varidx == HB_OT_LAYOUT_NO_VARIATIONS_INDEX) अनुवर्त;
      unsigned *गणनम्;
      सचे (varidx_ref_गणनम्.has (varidx, &गणनम्))
        (*गणनम्)++;
      अञ्ञथा सचे (!varidx_ref_गणनम्.set (varidx, 1))
        पटिनिवत्त मिच्छा;
    }
    अत्थाय (unsigned i = 0; i < axis_गणनम्_2; i++)
    {
      hb_tag_t *axis_tag_ptr;
      सचे (!c->plan->axes_old_index_tag_map.has (i, &axis_tag_ptr))
        पटिनिवत्त मिच्छा;
      /* Only axes that will receive offset compensation (restricted or
       * pinned) can contaminate a shared row. */
      सचे (!c->plan->axes_index_map.has (i) ||
          !c->plan->user_axes_location.has (*axis_tag_ptr))
        अनुवर्त;
      uint32_t varidx = new_varidx_mapping[i];
      सचे (varidx == HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
        अनुवर्त; /* gets a fresh, private VarData in the offset loop below */
      unsigned *गणनम्;
      सचे (!varidx_ref_गणनम्.has (varidx, &गणनम्) || *गणनम् <= 1)
        अनुवर्त; /* sole owner: safe to write offsets in place */
      unsigned outer = varidx >> 16;
      unsigned new_inner = item_vars.duplicate_row (outer, varidx & 0xFFFF);
      सचे (unlikely (new_inner == (unsigned) -1)) पटिनिवत्त मिच्छा;
      new_varidx_mapping[i] = (outer << 16) | new_inner;
      (*गणनम्)--;
    }

    /* 6. Add offset compensation tuples.
     * Track processed (outer,inner) pairs to avoid adding duplicate biases
     * when multiple axes share the same varIdx. */
    hb_set_t processed_varidxes;
    अत्थाय (unsigned i = 0; i < axis_गणनम्_2; i++)
    {
      hb_tag_t *axis_tag_ptr;
      सचे (!c->plan->axes_old_index_tag_map.has (i, &axis_tag_ptr))
        पटिनिवत्त मिच्छा;
      hb_tag_t axis_tag = *axis_tag_ptr;

      /* Self-contained pinned axes are removed from fvar/avar; their
       * contribution is baked into the variation tables instead. */
      सचे (!c->plan->axes_index_map.has (i))
        अनुवर्त;

      Triple *new_user;
      सचे (c->plan->user_axes_location.has (axis_tag, &new_user))
      {
        /* This axis is being restricted or pinned */
        Triple *old_int;
        सचे (!c->plan->old_intermediates.has (axis_tag, &old_int))
          पटिनिवत्त मिच्छा;

        भिन्न a_i = (भिन्न) old_int->minimum;
        भिन्न d_i = (भिन्न) old_int->मध्यः;
        भिन्न b_i = (भिन्न) old_int->maximum;

        पूर्णाङ्क d_int = roundf (d_i * 16384.f);

        तार्किक is_pinned = new_user->is_point ();

        uint32_t varidx = new_varidx_mapping[i];
        unsigned outer, inner, item_गणनम्_2;

        सचे (varidx == HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
        {
          /* No existing avar2 mapping. Create new VarData. */
          outer = item_vars.add_vardata (1);
          inner = 0;
          item_गणनम्_2 = 1;
          new_varidx_mapping[i] = (outer << 16) | inner;
          default_deltas[i] = 0.f; /* no prior default delta */
        }
        अञ्ञथा
        {
          outer = varidx >> 16;
          inner = varidx & 0xFFFF;
          item_गणनम्_2 = item_vars.get_item_गणनम् (outer);
        }

        /* Empty-region bias: d_int + round(defaultDelta) */
        पूर्णाङ्क bias = d_int + (पूर्णाङ्क) roundf (default_deltas[i]);
        सचे (bias != 0)
        {
          hb_hashmap_t<hb_tag_t, Triple> empty_region;
          item_vars.add_tuple (outer, std::move (empty_region),
                               inner, bias, item_गणनम्_2);
        }

        सचे (!is_pinned)
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

          ध्रुव hb_vector_t<Axis_मानम्_Map> &new_mapping = new_mappings[i];

          भिन्न min_f = 0.f, def_f = 0.f, max_f = 0.f;
          सचे (likely (i < fvar_axes.length))
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
          द्विगुण z_old = avar2_normalize_मानम् ((द्विगुण) old_def,
                                                 (द्विगुण) new_user->minimum,
                                                 (द्विगुण) new_user->मध्यः,
                                                 (द्विगुण) new_user->maximum);
          z_old = _avar2_map_new_mapping (new_mapping, z_old);
          z_old = (द्विगुण) roundf ((भिन्न) (z_old * 16384.0)) / 16384.0;
          सचे (-1.0 < z_old && z_old < 1.0)
            _avar2_add_knot (knots, z_old, -z_old);

          /* Interior avar v1 breakpoints inside the retained range each put
           * a kink in offset(z). Sampling only {-1, 0, +1, z_old} would
           * linearly interpolate across those kinks. The instantiated
           * mapping keeps exactly the in-range old breakpoints, and the new
           * mapping kinks at each one's output coordinate; add that z with
           * its old intermediate value so offset(z) is reproduced at every
           * kink. */
          hb_vector_t<avar2_offset_knot_t> with_breakpoints (knots);
          अत्थाय (ध्रुव auto &m : new_mapping)
          {
            द्विगुण from = (द्विगुण) m.coords[0].to_float ();
            सचे (from == -1.0 || from == 0.0 || from == 1.0)
              अनुवर्त; /* anchors already seeded */
            द्विगुण z = (द्विगुण) m.coords[1].to_float ();
            सचे (!(-1.0 < z && z < 1.0))
              अनुवर्त;
            द्विगुण user = avar2_denormalize_मानम् (from,
                                                    (द्विगुण) new_user->minimum,
                                                    (द्विगुण) new_user->मध्यः,
                                                    (द्विगुण) new_user->maximum);
            द्विगुण n_old = avar2_normalize_मानम् (user, old_min, old_def, old_max);
            द्विगुण x_old = (द्विगुण) old_seg_maps[i]->map_float ((भिन्न) n_old);
            x_old = (द्विगुण) roundf ((भिन्न) (x_old * 16384.0)) / 16384.0;
            _avar2_add_knot (with_breakpoints, z, x_old - z);
          }

          सचे (unlikely (knots.in_error () || with_breakpoints.in_error ()))
            पटिनिवत्त मिच्छा;

          /* Extra tents cost F2Dot14 rounding, so for a steep segment they
           * can add more quantization noise than the structural error they
           * remove. Keep the interior breakpoints only when they do not
           * increase the estimated residual; this makes the collection a
           * strict (never-worse) improvement over the {-1, 0, +1, z_old}
           * anchors. Warn when even the better choice is not bit-exact (a
           * steep retained segment that cannot be reproduced in F2Dot14). */
          unsigned err = _avar2_estimate_offset_error (*old_seg_maps[i], new_mapping,
                                                       old_min, old_def, old_max,
                                                       (द्विगुण) new_user->minimum,
                                                       (द्विगुण) new_user->मध्यः,
                                                       (द्विगुण) new_user->maximum,
                                                       knots);
          सचे (with_breakpoints.length != knots.length)
          {
            unsigned err_with = _avar2_estimate_offset_error (*old_seg_maps[i], new_mapping,
                                                              old_min, old_def, old_max,
                                                              (द्विगुण) new_user->minimum,
                                                              (द्विगुण) new_user->मध्यः,
                                                              (द्विगुण) new_user->maximum,
                                                              with_breakpoints);
            सचे (err_with <= err)
            {
              knots = std::move (with_breakpoints);
              err = err_with;
            }
          }
          सचे (err > 8)
            DEBUG_सन्देशः (SUBSET, nullptr,
                       "avar2 offset compensation is approximate for axis %c%c%c%c: "
                       "max residual %u F2Dot14 units",
                       HB_UNTAG (axis_tag), err);

          /* Synthesize tents. Adjacent tents evaluate to zero at each
           * other's peaks, so each knot's delta is offset(z) - offset(0);
           * the base value offset(0) = d_i is carried by the empty-region
           * bias above. This reduces to the classic pair of tents
           * (-1,-1,0) / (0,+1,+1) when the default is unchanged and there
           * are no interior knots. */
          अत्थाय (unsigned k = 0; k < knots.length; k++)
          {
            द्विगुण z = knots.arrayZ[k].z;
            सचे (z == 0.0) अनुवर्त;
            पूर्णाङ्क delta = (पूर्णाङ्क) roundf ((भिन्न) ((knots.arrayZ[k].offset - (द्विगुण) d_i) * 16384.0));
            सचे (!delta) अनुवर्त;
            द्विगुण lower, upper;
            सचे (z > 0.0)
            {
              lower = knots.arrayZ[k - 1].z;
              upper = k + 1 < knots.length ? knots.arrayZ[k + 1].z : z;
            }
            अञ्ञथा
            {
              upper = knots.arrayZ[k + 1].z;
              lower = k > 0 ? knots.arrayZ[k - 1].z : z;
            }
            hb_hashmap_t<hb_tag_t, Triple> region;
            सचे (unlikely (!region.set (axis_tag, Triple (lower, z, upper))))
              पटिनिवत्त मिच्छा;
            item_vars.add_tuple (outer, std::move (region),
                                 inner, delta, item_गणनम्_2);
          }
        }
      }
      अञ्ञथा
      {
        /* Free or private axis — not being restricted.
         * If it has a non-zero default delta, add it back as a bias.
         * Skip if this (outer,inner) was already processed (shared varIdx). */
        uint32_t varidx = new_varidx_mapping[i];
        सचे (varidx == HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
          अनुवर्त;

        सचे (processed_varidxes.has (varidx))
          अनुवर्त;
        processed_varidxes.add (varidx);

        unsigned outer = varidx >> 16;
        unsigned inner = varidx & 0xFFFF;
        पूर्णाङ्क dd = (पूर्णाङ्क) roundf (default_deltas[i]);
        सचे (dd != 0)
        {
          unsigned item_गणनम्_2 = item_vars.get_item_गणनम् (outer);
          hb_hashmap_t<hb_tag_t, Triple> empty_region;
          item_vars.add_tuple (outer, std::move (empty_region),
                               inner, dd, item_गणनम्_2);
        }
      }
    }

    /* 7. Finalize: build region list + convert to varstore */
    सचे (!item_vars.build_region_list ()) पटिनिवत्त मिच्छा;
    सचे (!item_vars.as_item_varstore (सच्च /* optimize */,
                                     मिच्छा /* use_no_variation_idx */))
      पटिनिवत्त मिच्छा;

    /* 8. Apply varidx_map optimization remapping */
    ध्रुव auto &opt_varidx_map = item_vars.get_varidx_map ();
    अत्थाय (unsigned i = 0; i < axis_गणनम्_2; i++)
    {
      uint32_t varidx = new_varidx_mapping[i];
      सचे (varidx == HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
        अनुवर्त;
      uint32_t *new_idx;
      सचे (opt_varidx_map.has (varidx, &new_idx))
        new_varidx_mapping[i] = *new_idx;
    }

    /* 9. Serialize avarV2Tail. Entries cover the retained axes only;
     * self-contained pinned axes are removed from fvar. */
    avar2_index_map_plan_t index_map_plan;
    सचे (!index_map_plan.init (new_varidx_mapping,
			      c->plan->axes_index_map,
			      axis_गणनम्_2))
      पटिनिवत्त मिच्छा;

    auto *tail = c->serializer->allocate_size<avarV2Tail> (avarV2Tail::static_size);
    सचे (unlikely (!tail)) पटिनिवत्त मिच्छा;
    सचे (!tail->varIdxMap.serialize_serialize (c->serializer, index_map_plan))
      पटिनिवत्त मिच्छा;
    सचे (!tail->varStore.serialize_serialize (c->serializer,
					     item_vars.has_long_word (),
					     c->plan->axis_tags,
					     item_vars.get_region_list (),
					     item_vars.get_vardata_encodings ()))
      पटिनिवत्त मिच्छा;

    पटिनिवत्त सच्च;
#endif
  }

  सार्वजनिक:

  रक्षित:
  FixedVersion<>version;	/* Version of the avar table
				 * initially set to 0x00010000u */
  HBUINT16	reserved;	/* This field is permanently reserved. Set to 0. */
  HBUINT16	axis_गणनम्_2;	/* The number of variation axes in the font. This
				 * must be the same number as axisCount in the
				 * 'fvar' table. */
  SegmentMaps	firstAxisSegmentMaps;

  सार्वजनिक:
  DEFINE_SIZE_MIN (8);
};

} /* namespace OT */


#endif /* HB_OT_VAR_AVAR_TABLE_HH */
