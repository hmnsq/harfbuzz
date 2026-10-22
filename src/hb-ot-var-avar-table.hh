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
		 अचर शून्य *base) अचर
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


संरचना Axis_मान_Map
{
  तार्किक sanitize (hb_sanitize_context_t *c) अचर
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्));
  }

  शून्य set_mapping (भिन्न from_coord, भिन्न to_coord)
  {
    coords[0].set_float (from_coord);
    coords[1].set_float (to_coord);
  }

  तार्किक is_outside_axis_range (अचर Triple& axis_range) अचर
  {
    द्विगुण from_coord = (द्विगुण) coords[0].to_float ();
    फर्काऊ !axis_range.contains (from_coord);
  }

  तार्किक must_include () अचर
  {
    भिन्न from_coord = coords[0].to_float ();
    भिन्न to_coord = coords[1].to_float ();
    फर्काऊ (from_coord == -1.f && to_coord == -1.f) ||
           (from_coord == 0.f && to_coord == 0.f) ||
           (from_coord == 1.f && to_coord == 1.f);
  }

  शून्य instantiate (अचर Triple& axis_range,
                    अचर Triple& unmapped_range,
                    अचर TripleDistances& triple_distances)
  {
    भिन्न from_coord = coords[0].to_float ();
    भिन्न to_coord = coords[1].to_float ();

    from_coord = renormalize_मान ((द्विगुण) from_coord, unmapped_range, triple_distances);
    to_coord = renormalize_मान ((द्विगुण) to_coord, axis_range, triple_distances);

    /* Malformed input can renormalize outside [-1,1], which would
     * overflow F2DOT14. */
    from_coord = hb_clamp (from_coord, -1.f, 1.f);
    to_coord = hb_clamp (to_coord, -1.f, 1.f);

    coords[0].set_float (from_coord);
    coords[1].set_float (to_coord);
  }

  HB_INTERNAL स्थिर पूर्णाङ्क cmp (अचर शून्य *pa, अचर शून्य *pb)
  {
    अचर Axis_मान_Map *a = (अचर Axis_मान_Map *) pa;
    अचर Axis_मान_Map *b = (अचर Axis_मान_Map *) pb;

    पूर्णाङ्क a_from = a->coords[0].to_int ();
    पूर्णाङ्क b_from = b->coords[0].to_int ();
    यदि (a_from != b_from)
      फर्काऊ a_from - b_from;

    /* this should never be reached. according to the spec, all of the axis
     * value map records for a given axis must have different fromCoord values
     * */
    पूर्णाङ्क a_to = a->coords[1].to_int ();
    पूर्णाङ्क b_to = b->coords[1].to_int ();
    फर्काऊ a_to - b_to;
  }

  तार्किक serialize (hb_serialize_context_t *c) अचर
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

संरचना SegmentMaps : Array16Of<Axis_मान_Map>
{
  भिन्न map_float (भिन्न value, अचिन्हित पूर्णाङ्क from_offset = 0, अचिन्हित पूर्णाङ्क to_offset = 1) अचर
  {
#define fromCoord coords[from_offset].to_float ()
#define toCoord coords[to_offset].to_float ()

    अचर स्वतः *map = arrayZ;

    /* The following special-cases are not part of OpenType, which requires
     * that at least -1, 0, and +1 must be mapped. But we include these as
     * part of a better error recovery scheme. */
    यदि (len < 2)
    {
      यदि (!len)
	फर्काऊ value;
      अन्यथा /* len == 1*/
	फर्काऊ value - map[0].fromCoord + map[0].toCoord;
    }

    // At least two mappings now.

    /* CoreText is wild...
     * PingFangUI avar needs all this special-casing...
     * So we implement an extended version of the spec here,
     * which is more robust and more likely to be compatible with
     * the wild. */

    अचिन्हित start = 0;
    अचिन्हित end = len;
    यदि (map[start].fromCoord == -1 && map[start].toCoord == -1 && map[start+1].fromCoord == -1)
      start++;
    यदि (map[end-1].fromCoord == +1 && map[end-1].toCoord == +1 && map[end-2].fromCoord == +1)
      end--;

    /* Look for exact match first, and do lots of special-casing. */
    अचिन्हित i;
    लागि (i = start; i < end; i++)
      यदि (value == map[i].fromCoord)
	रोक;
    यदि (i < end)
    {
      // There's at least one exact match. See if there are more.
      अचिन्हित j = i;
      लागि (; j + 1 < end; j++)
	यदि (value != map[j + 1].fromCoord)
	  रोक;

      // [i,j] inclusive are all exact matches:

      // If there's only one, return it. This is the only spec-compliant case.
      यदि (i == j)
	फर्काऊ map[i].toCoord;
      // If there's exactly three, return the middle one.
      यदि (i + 2 == j)
	फर्काऊ map[i + 1].toCoord;

      // Ignore the middle ones. Return the one mapping closer to 0.
      यदि (value < 0) फर्काऊ map[j].toCoord;
      यदि (value > 0) फर्काऊ map[i].toCoord;

      // Mapping 0? CoreText seems confused. It seems to prefer 0 here...
      // So we'll just return the smallest one. lol
      फर्काऊ fabsf (map[i].toCoord) < fabsf (map[j].toCoord) ? map[i].toCoord : map[j].toCoord;

      // Mapping 0? Return one not mapping to 0.
      यदि (map[i].toCoord == 0)
	फर्काऊ map[j].toCoord;
      अन्यथा
	फर्काऊ map[i].toCoord;
    }

    /* There's at least two and we're not an exact match. Prepare to lerp. */

    // Find the segment we're in.
    लागि (i = start; i < end; i++)
      यदि (value < map[i].fromCoord)
	रोक;

    यदि (i == start)
    {
      // Value before all segments; Shift.
      फर्काऊ value - map[start].fromCoord + map[start].toCoord;
    }
    यदि (i == end)
    {
      // Value after all segments; Shift.
      फर्काऊ value - map[end - 1].fromCoord + map[end - 1].toCoord;
    }

    // Actually interpolate.
    स्वतः &before = map[i-1];
    स्वतः &after = map[i];
    भिन्न denom = after.fromCoord - before.fromCoord; // Can't be zero by now.
    फर्काऊ before.toCoord + ((after.toCoord - before.toCoord) * (value - before.fromCoord)) / denom;

#undef toCoord
#undef fromCoord
  }

  भिन्न unmap_float (भिन्न value) अचर { फर्काऊ map_float (value, 1, 0); }


  // TODO Kill this.
  Triple unmap_axis_range (अचर Triple& axis_range) अचर
  {
    भिन्न unmapped_min = unmap_float (axis_range.minimum);
    भिन्न unmapped_बीच = unmap_float (axis_range.बीच);
    भिन्न unmapped_max = unmap_float (axis_range.maximum);

    फर्काऊ Triple{unmapped_min, unmapped_बीच, unmapped_max};
  }

  तार्किक subset (hb_subset_context_t *c, hb_tag_t axis_tag,
               hb_vector_t<Axis_मान_Map> *out_mappings = nullptr) अचर
  {
    TRACE_SUBSET (एतत्);

    /* This function cannot work on avar2 table (and currently doesn't).
     * We should instead keep the design coords in the shape plan and use
     * those. unmap_axis_range needs to be killed. */

    /* avar mapped normalized axis range. Under avar2, axes_location holds
     * only the self-contained pins for the other tables; avar itself uses
     * the intermediate-space ranges of all restricted axes. */
    अचर स्वतः &axes_location = c->plan->has_avar2
				? c->plan->old_intermediates
				: c->plan->axes_location;
    Triple *axis_range;
    यदि (!axes_location.has (axis_tag, &axis_range))
      फर्काऊ c->serializer->embed (*एतत्);

    TripleDistances *axis_triple_distances;
    यदि (!c->plan->axes_triple_distances.has (axis_tag, &axis_triple_distances))
      return_trace (असत्य);

    स्वतः *out = c->serializer->start_embed (एतत्);
    यदि (unlikely (!c->serializer->extend_min (out))) return_trace (असत्य);

    Triple unmapped_range = unmap_axis_range (*axis_range);

    /* create a vector of retained mappings and sort */
    hb_vector_t<Axis_मान_Map> मान_mappings;
    लागि (अचर स्वतः& _ : as_array ())
    {
      यदि (_.is_outside_axis_range (unmapped_range))
        जारी;
      Axis_मान_Map mapping;
      mapping = _;
      mapping.instantiate (*axis_range, unmapped_range, *axis_triple_distances);
      /* (-1, -1), (0, 0), (1, 1) mappings will be added later, so avoid
       * duplicates here */
      यदि (mapping.must_include ())
        जारी;
      मान_mappings.push (mapping);
    }

    Axis_मान_Map m;
    m.set_mapping (-1.f, -1.f);
    मान_mappings.push (m);

    m.set_mapping (0.f, 0.f);
    मान_mappings.push (m);

    m.set_mapping (1.f, 1.f);
    मान_mappings.push (m);

    मान_mappings.क्विक ();

    यदि (unlikely (मान_mappings.in_error ()))
      return_trace (असत्य);

    लागि (अचर स्वतः& _ : मान_mappings)
    {
      यदि (!_.serialize (c->serializer))
        return_trace (असत्य);
    }
    यदि (!c->serializer->check_assign (out->len, मान_mappings.length, HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (असत्य);
    /* Hand the instantiated mappings to the caller; avar2 offset
     * compensation needs them to locate the new mapping's kinks. */
    यदि (out_mappings)
      *out_mappings = std::move (मान_mappings);
    return_trace (सत्य);
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
  अचिन्हित i = 0;
  जबसम्म (i < knots.length && knots.arrayZ[i].z < z) i++;
  यदि (i < knots.length && knots.arrayZ[i].z == z) फर्काऊ;
  knots.push (avar2_offset_knot_t {0.0, 0.0});
  यदि (unlikely (knots.in_error ())) फर्काऊ;
  लागि (अचिन्हित j = knots.length - 1; j > i; j--)
    knots.arrayZ[j] = knots.arrayZ[j - 1];
  knots.arrayZ[i] = avar2_offset_knot_t {z, offset};
}

/* Piecewise-linear evaluation over the sorted knots. Inputs are always
 * within [-1, +1] and anchor knots at -1/0/+1 always exist. */
स्थिर अन्तःस्थ द्विगुण
_avar2_eval_offset (अचर hb_vector_t<avar2_offset_knot_t> &knots, द्विगुण z)
{
  अचिन्हित len = knots.length;
  लागि (अचिन्हित i = 0; i < len; i++)
  {
    यदि (z == knots.arrayZ[i].z) फर्काऊ knots.arrayZ[i].offset;
    यदि (z < knots.arrayZ[i].z)
    {
      यदि (!i) फर्काऊ knots.arrayZ[0].offset;
      अचर स्वतः &before = knots.arrayZ[i - 1];
      अचर स्वतः &after = knots.arrayZ[i];
      द्विगुण denom = after.z - before.z;
      फर्काऊ before.offset + (after.offset - before.offset) * (z - before.z) / denom;
    }
  }
  फर्काऊ len ? knots.arrayZ[len - 1].offset : 0.0;
}

/* Piecewise-linear evaluation over instantiated avar v1 mappings, as
 * produced by SegmentMaps::subset (sorted, with -1/0/+1 anchors).
 * Matches SegmentMaps::map_float for such well-formed mappings. */
स्थिर अन्तःस्थ द्विगुण
_avar2_map_new_mapping (अचर hb_vector_t<Axis_मान_Map> &mappings, द्विगुण v,
			अचिन्हित from_offset = 0, अचिन्हित to_offset = 1)
{
  अचिन्हित len = mappings.length;
  यदि (!len) फर्काऊ v;

  लागि (अचिन्हित i = 0; i < len; i++)
  {
    द्विगुण from = (द्विगुण) mappings.arrayZ[i].coords[from_offset].to_float ();
    यदि (v == from)
      फर्काऊ (द्विगुण) mappings.arrayZ[i].coords[to_offset].to_float ();
    यदि (v < from)
    {
      द्विगुण to = (द्विगुण) mappings.arrayZ[i].coords[to_offset].to_float ();
      यदि (!i) फर्काऊ v - from + to;
      द्विगुण prev_from = (द्विगुण) mappings.arrayZ[i - 1].coords[from_offset].to_float ();
      द्विगुण prev_to = (द्विगुण) mappings.arrayZ[i - 1].coords[to_offset].to_float ();
      द्विगुण denom = from - prev_from;
      यदि (denom == 0.0) फर्काऊ prev_to;
      फर्काऊ prev_to + (to - prev_to) * (v - prev_from) / denom;
    }
  }
  फर्काऊ v - (द्विगुण) mappings.arrayZ[len - 1].coords[from_offset].to_float ()
           + (द्विगुण) mappings.arrayZ[len - 1].coords[to_offset].to_float ();
}

/* Inverse of _avar2_map_new_mapping: pull an output coordinate back to a
 * preimage input coordinate. For a non-strictly-monotone mapping this picks
 * one preimage, which is fine for its only use (augmenting the error
 * estimator's sample set). */
स्थिर अन्तःस्थ द्विगुण
_avar2_unmap_new_mapping (अचर hb_vector_t<Axis_मान_Map> &mappings, द्विगुण v)
{
  फर्काऊ _avar2_map_new_mapping (mappings, v, 1, 0);
}

/* Plain (non-avar) fvar-style normalization of a user value against a
 * user-space (min, default, max) triple. */
स्थिर अन्तःस्थ द्विगुण
avar2_normalize_मान (द्विगुण v, द्विगुण min, द्विगुण def, द्विगुण max)
{
  v = hb_clamp (v, min, max);
  यदि (v == def) फर्काऊ 0.0;
  यदि (v < def) फर्काऊ def == min ? 0.0 : (v - def) / (def - min);
  फर्काऊ def == max ? 0.0 : (v - def) / (max - def);
}

/* Inverse of the above: map a normalized value back to user space. */
स्थिर अन्तःस्थ द्विगुण
avar2_denormalize_मान (द्विगुण v, द्विगुण min, द्विगुण def, द्विगुण max)
{
  यदि (v == 0.0) फर्काऊ def;
  फर्काऊ v < 0.0 ? def + v * (def - min) : def + v * (max - def);
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
स्थिर अन्तःस्थ अचिन्हित
_avar2_estimate_offset_error (अचर SegmentMaps &old_seg,
                              अचर hb_vector_t<Axis_मान_Map> &new_mapping,
                              द्विगुण old_min, द्विगुण old_def, द्विगुण old_max,
                              द्विगुण new_min, द्विगुण new_def, द्विगुण new_max,
                              अचर hb_vector_t<avar2_offset_knot_t> &knots)
{
  constexpr अचिन्हित samples = 257;
  hb_vector_t<द्विगुण> us;
  यदि (unlikely (!us.alloc (samples + old_seg.as_array ().length +
                           new_mapping.length + knots.length)))
    फर्काऊ अच_पूर्णांक_उच्च; /* estimator only; fail towards "worse" */
  लागि (अचिन्हित i = 0; i < samples; i++)
    us.push (new_min + (new_max - new_min) * i / (samples - 1));
  लागि (अचर स्वतः &_ : old_seg.as_array ())
    us.push (avar2_denormalize_मान ((द्विगुण) _.coords[0].to_float (),
                                       old_min, old_def, old_max));
  लागि (अचर स्वतः &_ : new_mapping)
    us.push (avar2_denormalize_मान ((द्विगुण) _.coords[0].to_float (),
                                       new_min, new_def, new_max));
  लागि (अचर स्वतः &knot : knots)
    us.push (avar2_denormalize_मान (
               _avar2_unmap_new_mapping (new_mapping, knot.z),
               new_min, new_def, new_max));
  यदि (unlikely (us.in_error ())) फर्काऊ अच_पूर्णांक_उच्च;

  अचिन्हित max_err = 0;
  लागि (द्विगुण u : us)
  {
    यदि (u < new_min || u > new_max) जारी;
    द्विगुण n_old = avar2_normalize_मान (u, old_min, old_def, old_max);
    द्विगुण old_final = (द्विगुण) old_seg.map_float ((भिन्न) n_old);
    द्विगुण n_new = avar2_normalize_मान (u, new_min, new_def, new_max);
    द्विगुण z = _avar2_map_new_mapping (new_mapping, n_new);
    द्विगुण new_final = z + _avar2_eval_offset (knots, z);
    पूर्णाङ्क err = असल ((पूर्णाङ्क) roundf ((भिन्न) (old_final * 16384.0)) -
                   (पूर्णाङ्क) roundf ((भिन्न) (new_final * 16384.0)));
    यदि ((अचिन्हित) err > max_err) max_err = err;
  }
  फर्काऊ max_err;
}

संरचना avar
{
  स्थिर constexpr hb_tag_t tableTag = HB_OT_TAG_avar;

  तार्किक has_data () अचर { फर्काऊ version.to_int (); }

  अचर SegmentMaps* get_segment_maps () अचर
  { फर्काऊ &firstAxisSegmentMaps; }

  अचिन्हित get_axis_गन्ती () अचर
  { फर्काऊ axis_गन्ती_2; }

  तार्किक sanitize (hb_sanitize_context_t *c) अचर
  {
    TRACE_SANITIZE (एतत्);
    यदि (!(version.sanitize (c) &&
	  hb_barrier () &&
	  (version.major == 1
#ifndef HB_NO_AVAR2
	   || version.major == 2
#endif
	   ) &&
	  c->check_struct (एतत्)))
      return_trace (असत्य);

    अचर SegmentMaps *map = &firstAxisSegmentMaps;
    अचिन्हित पूर्णाङ्क गन्ती = axis_गन्ती_2;
    लागि (अचिन्हित पूर्णाङ्क i = 0; i < गन्ती; i++)
    {
      यदि (unlikely (!map->sanitize (c)))
	return_trace (असत्य);
      map = &StructAfter<SegmentMaps> (*map);
    }

#ifndef HB_NO_AVAR2
    यदि (version.major < 2)
      return_trace (सत्य);
    hb_barrier ();

    अचर स्वतः &v2 = * (अचर avarV2Tail *) map;
    यदि (unlikely (!v2.sanitize (c, एतत्)))
      return_trace (असत्य);
#endif

    return_trace (सत्य);
  }

  शून्य map_coords_16_16 (पूर्णाङ्क *coords, अचिन्हित पूर्णाङ्क coords_length) अचर
  {
    अचिन्हित पूर्णाङ्क गन्ती = hb_min (coords_length, axis_गन्ती_2);

    अचर SegmentMaps *map = &firstAxisSegmentMaps;
    लागि (अचिन्हित पूर्णाङ्क i = 0; i < गन्ती; i++)
    {
      coords[i] = roundf (map->map_float (coords[i] / 65536.f) * 65536.f);
      map = &StructAfter<SegmentMaps> (*map);
    }

#ifndef HB_NO_AVAR2
    यदि (version.major < 2)
      फर्काऊ;
    hb_barrier ();

    लागि (; गन्ती < axis_गन्ती_2; गन्ती++)
      map = &StructAfter<SegmentMaps> (*map);

    अचर स्वतः &v2 = * (अचर avarV2Tail *) map;

    अचर स्वतः &varidx_map = एतत्+v2.varIdxMap;
    अचर स्वतः &var_store = एतत्+v2.varStore;
    स्वतः *var_store_cache = var_store.create_cache ();

    hb_vector_t<पूर्णाङ्क> coords_2_14;
    coords_2_14.resize (coords_length);
    लागि (अचिन्हित i = 0; i < coords_length; i++)
      coords_2_14[i] = roundf (coords[i] / 4.f); // 16.16 -> 2.14

    hb_vector_t<पूर्णाङ्क> out;
    out.alloc (coords_length);
    लागि (अचिन्हित i = 0; i < coords_length; i++)
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
    लागि (अचिन्हित i = 0; i < coords_length; i++)
      coords[i] = out[i];

    OT::ItemVariationStore::destroy_cache (var_store_cache);
#endif
  }

  /* An avar version 2 font is never downgraded to v1: even with a NULL
   * varStore offset (legal; maps like plain v1) the avar2 subsetting path
   * applies, with "no variation" semantics for rows that don't resolve —
   * offset compensation then creates delta rows on demand. */
  तार्किक has_v2_data () अचर { फर्काऊ version.major > 1; }

  /* Resolve the avar2 VarStore and VarIdxMap, for the subset planner.
   * Either pointer may be to the Null object (nullable offsets). */
  तार्किक get_v2_store_and_map (अचर ItemVariationStore **store,
                             अचर DeltaSetIndexMap **varidx_map) अचर
  {
#ifndef HB_NO_AVAR2
    यदि (version.major < 2) फर्काऊ असत्य;
    अचर SegmentMaps *map = &firstAxisSegmentMaps;
    लागि (अचिन्हित i = 0; i < axis_गन्ती_2; i++)
      map = &StructAfter<SegmentMaps> (*map);
    अचर स्वतः &v2 = * (अचर avarV2Tail *) map;
    *store = &(एतत्+v2.varStore);
    *varidx_map = &(एतत्+v2.varIdxMap);
    फर्काऊ सत्य;
#अन्यथा
    फर्काऊ असत्य;
#endif
  }

  // axis normalization is done in 2.14 here
  // TODO: deprecate this API once fonttools is updated to use 16.16 normalization
  तार्किक map_coords_2_14 (भिन्न *coords, अचिन्हित पूर्णाङ्क coords_length,
			तार्किक v1_only = असत्य) अचर
  {
    hb_vector_t<पूर्णाङ्क> coords_2_14;
    यदि (!v1_only && !coords_2_14.resize (coords_length)) फर्काऊ असत्य;
    अचिन्हित पूर्णाङ्क गन्ती = hb_min (coords_length, axis_गन्ती_2);

    अचर SegmentMaps *map = &firstAxisSegmentMaps;
    लागि (अचिन्हित पूर्णाङ्क i = 0; i < गन्ती; i++)
    {
      पूर्णाङ्क v = roundf (map->map_float (coords[i]) * 16384.f);
      यदि (!v1_only)
	coords_2_14[i] = v;
      coords[i] = v / 16384.f;
      map = &StructAfter<SegmentMaps> (*map);
    }

    यदि (v1_only)
      फर्काऊ सत्य;

#ifndef HB_NO_AVAR2
    यदि (version.major < 2)
      फर्काऊ सत्य;
    hb_barrier ();

    लागि (; गन्ती < axis_गन्ती_2; गन्ती++)
      map = &StructAfter<SegmentMaps> (*map);

    अचर स्वतः &v2 = * (अचर avarV2Tail *) map;

    अचर स्वतः &varidx_map = एतत्+v2.varIdxMap;
    अचर स्वतः &var_store = एतत्+v2.varStore;
    स्वतः *var_store_cache = var_store.create_cache ();

    लागि (अचिन्हित i = 0; i < coords_length; i++)
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
    फर्काऊ सत्य;
#अन्यथा
    फर्काऊ version.major < 2;
#endif
  }

  तार्किक subset (hb_subset_context_t *c) अचर
  {
    TRACE_SUBSET (एतत्);
    अचिन्हित retained_axis_गन्ती = c->plan->axes_index_map.get_population ();
    यदि (!retained_axis_गन्ती) //all axes are pinned/dropped
      return_trace (असत्य);

    avar *out = c->serializer->allocate_min<avar> ();
    यदि (unlikely (!out)) return_trace (असत्य);

    out->version.major = c->plan->has_avar2 ? 2 : 1;
    out->version.minor = 0;
    यदि (!c->serializer->check_assign (out->axis_गन्ती_2, retained_axis_गन्ती, HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (असत्य);

    /* For avar2, keep the instantiated v1 mappings around; offset
     * compensation needs them to locate the new mappings' kinks. */
    hb_vector_t<hb_vector_t<Axis_मान_Map>> new_mappings;
    यदि (c->plan->has_avar2 && !new_mappings.resize (axis_गन्ती_2))
      return_trace (असत्य);

    अचर hb_map_t& axes_index_map = c->plan->axes_index_map;
    अचर SegmentMaps *map = &firstAxisSegmentMaps;
    अचिन्हित गन्ती = axis_गन्ती_2;
    लागि (अचिन्हित पूर्णाङ्क i = 0; i < गन्ती; i++)
    {
      यदि (axes_index_map.has (i))
      {
        hb_tag_t *axis_tag;
        यदि (!c->plan->axes_old_index_tag_map.has (i, &axis_tag))
          return_trace (असत्य);

        Triple *axis_location;
        यदि (c->plan->has_avar2 &&
            c->plan->user_axes_location.has (*axis_tag, &axis_location) &&
            axis_location->is_point ())
        {
          /* Pinned axis in avar2 mode: serialize identity segment map
           * {-1->-1, 0->0, 1->1}. The axis is kept in fvar as hidden,
           * so avar needs a segment map entry for it. */
          स्वतः *identity_map = c->serializer->start_embed<SegmentMaps> ();
          यदि (unlikely (!c->serializer->extend_min (identity_map)))
            return_trace (असत्य);
          Axis_मान_Map m;
          m.set_mapping (-1.f, -1.f);
          यदि (!m.serialize (c->serializer)) return_trace (असत्य);
          m.set_mapping (0.f, 0.f);
          यदि (!m.serialize (c->serializer)) return_trace (असत्य);
          m.set_mapping (1.f, 1.f);
          यदि (!m.serialize (c->serializer)) return_trace (असत्य);
          यदि (!c->serializer->check_assign (identity_map->len, 3u,
                                            HB_SERIALIZE_ERROR_INT_OVERFLOW))
            return_trace (असत्य);
        }
        अन्यथा
        {
          /* Restricted or free axis: use standard SegmentMaps::subset() */
          यदि (!map->subset (c, *axis_tag,
                            c->plan->has_avar2 ? &new_mappings[i] : nullptr))
            return_trace (असत्य);
        }
      }
      map = &StructAfter<SegmentMaps> (*map);
    }

    यदि (c->plan->has_avar2)
      return_trace (_subset_avar2 (c, new_mappings));

    return_trace (सत्य);
  }

  गुप्त:
  संरचना avar2_index_map_plan_t
  {
    तार्किक init (अचर hb_vector_t<uint32_t> &varidx_mapping,
	       अचर hb_map_t &axes_index_map,
	       अचिन्हित axis_गन्ती)
    {
      यदि (!output_map.alloc (axes_index_map.get_population ()))
	फर्काऊ असत्य;

      तार्किक has_no_variation = असत्य;
      अचिन्हित max_outer = 0, max_inner = 0;
      लागि (अचिन्हित i = 0; i < axis_गन्ती; i++)
      {
	यदि (!axes_index_map.has (i)) जारी;
	uint32_t varidx = varidx_mapping[i];
	output_map.push (varidx);
	यदि (varidx == HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
	{
	  has_no_variation = सत्य;
	  जारी;
	}
	max_outer = hb_max (max_outer, varidx >> 16);
	max_inner = hb_max (max_inner, varidx & 0xFFFF);
      }
      यदि (output_map.in_error () ||
	  output_map.length != axes_index_map.get_population ())
	फर्काऊ असत्य;

      यदि (has_no_variation)
      {
	width = 4;
	inner_bit_गन्ती = 16;
      }
      अन्यथा
      {
	inner_bit_गन्ती = hb_max (1u, hb_bit_storage (max_inner));
	अचिन्हित outer_bit_गन्ती = hb_max (1u, hb_bit_storage (max_outer));
	width = hb_clamp ((inner_bit_गन्ती + outer_bit_गन्ती + 7) / 8, 1u, 4u);
	यदि (inner_bit_गन्ती + outer_bit_गन्ती > width * 8)
	  inner_bit_गन्ती = width * 8 - outer_bit_गन्ती;
      }
      फर्काऊ सत्य;
    }

    अचिन्हित get_inner_bit_गन्ती () अचर { फर्काऊ inner_bit_गन्ती; }
    अचिन्हित get_width () अचर { फर्काऊ width; }
    hb_array_t<अचर uint32_t> get_output_map () अचर { फर्काऊ output_map.as_array (); }

    अचिन्हित inner_bit_गन्ती = 1;
    अचिन्हित width = 1;
    hb_vector_t<uint32_t> output_map;
  };

  तार्किक _subset_avar2 (hb_subset_context_t *c,
                      अचर hb_vector_t<hb_vector_t<Axis_मान_Map>> &new_mappings) अचर
  {
#यदि defined (HB_NO_VAR) || defined (HB_NO_AVAR2)
    /* Not reachable: the plan never sets has_avar2 in these configurations. */
    फर्काऊ असत्य;
#अन्यथा

    /* 1. Locate original avar2 data, keeping per-axis old segment maps */
    hb_vector_t<अचर SegmentMaps *> old_seg_maps;
    यदि (!old_seg_maps.alloc (axis_गन्ती_2)) फर्काऊ असत्य;
    अचर SegmentMaps *map = &firstAxisSegmentMaps;
    लागि (अचिन्हित i = 0; i < axis_गन्ती_2; i++)
    {
      old_seg_maps.push (map);
      map = &StructAfter<SegmentMaps> (*map);
    }

    अचर स्वतः &v2 = * (अचर avarV2Tail *) map;
    अचर स्वतः &varidx_map = एतत्+v2.varIdxMap;
    अचर स्वतः &var_store = एतत्+v2.varStore;

    स्वतः fvar_axes = c->plan->source->table.fvar->get_axes ();

    /* 2. Compute default deltas by evaluating VarStore at old defaults */
    hb_vector_t<पूर्णाङ्क> default_coords;
    यदि (!default_coords.resize (axis_गन्ती_2)) फर्काऊ असत्य;
    लागि (अचिन्हित i = 0; i < axis_गन्ती_2; i++)
    {
      hb_tag_t *axis_tag;
      यदि (c->plan->axes_old_index_tag_map.has (i, &axis_tag) &&
          c->plan->old_intermediates.has (*axis_tag))
      {
        भिन्न d_i = (भिन्न) c->plan->old_intermediates.get (*axis_tag).बीच;
        default_coords[i] = roundf (d_i * 16384.f);
      }
      अन्यथा
        default_coords[i] = 0;
    }

    स्वतः *store_cache = var_store.create_cache ();
    hb_vector_t<भिन्न> default_deltas;
    यदि (!default_deltas.resize (axis_गन्ती_2)) {
      ItemVariationStore::destroy_cache (store_cache);
      फर्काऊ असत्य;
    }
    लागि (अचिन्हित i = 0; i < axis_गन्ती_2; i++)
    {
      uint32_t varidx = varidx_map.map (i);
      यदि (varidx == HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
        default_deltas[i] = 0.f;
      अन्यथा
        default_deltas[i] = var_store.get_delta (varidx, default_coords.arrayZ,
                                                  default_coords.length, store_cache);
    }
    ItemVariationStore::destroy_cache (store_cache);

    /* 3. Rebase IVS regions */
    item_variations_t item_vars;
    यदि (!item_vars.create_from_item_varstore (var_store, c->plan->axes_old_index_tag_map))
      फर्काऊ असत्य;
    यदि (!item_vars.instantiate_tuple_vars (c->plan->old_intermediates,
                                           c->plan->axes_triple_distances,
                                           असत्य))
      फर्काऊ असत्य;

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
    यदि (!new_varidx_mapping.resize (axis_गन्ती_2)) फर्काऊ असत्य;
    लागि (अचिन्हित i = 0; i < axis_गन्ती_2; i++)
    {
      uint32_t varidx = varidx_map.map (i);
      यदि (varidx != HB_OT_LAYOUT_NO_VARIATIONS_INDEX &&
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
    hb_hashmap_t<uint32_t, अचिन्हित> varidx_ref_गन्ती;
    लागि (अचिन्हित i = 0; i < axis_गन्ती_2; i++)
    {
      यदि (!c->plan->axes_index_map.has (i)) जारी; /* self-contained: dropped */
      uint32_t varidx = new_varidx_mapping[i];
      यदि (varidx == HB_OT_LAYOUT_NO_VARIATIONS_INDEX) जारी;
      अचिन्हित *गन्ती;
      यदि (varidx_ref_गन्ती.has (varidx, &गन्ती))
        (*गन्ती)++;
      अन्यथा यदि (!varidx_ref_गन्ती.set (varidx, 1))
        फर्काऊ असत्य;
    }
    लागि (अचिन्हित i = 0; i < axis_गन्ती_2; i++)
    {
      hb_tag_t *axis_tag_ptr;
      यदि (!c->plan->axes_old_index_tag_map.has (i, &axis_tag_ptr))
        फर्काऊ असत्य;
      /* Only axes that will receive offset compensation (restricted or
       * pinned) can contaminate a shared row. */
      यदि (!c->plan->axes_index_map.has (i) ||
          !c->plan->user_axes_location.has (*axis_tag_ptr))
        जारी;
      uint32_t varidx = new_varidx_mapping[i];
      यदि (varidx == HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
        जारी; /* gets a fresh, private VarData in the offset loop below */
      अचिन्हित *गन्ती;
      यदि (!varidx_ref_गन्ती.has (varidx, &गन्ती) || *गन्ती <= 1)
        जारी; /* sole owner: safe to write offsets in place */
      अचिन्हित outer = varidx >> 16;
      अचिन्हित new_inner = item_vars.duplicate_row (outer, varidx & 0xFFFF);
      यदि (unlikely (new_inner == (अचिन्हित) -1)) फर्काऊ असत्य;
      new_varidx_mapping[i] = (outer << 16) | new_inner;
      (*गन्ती)--;
    }

    /* 6. Add offset compensation tuples.
     * Track processed (outer,inner) pairs to avoid adding duplicate biases
     * when multiple axes share the same varIdx. */
    hb_set_t processed_varidxes;
    लागि (अचिन्हित i = 0; i < axis_गन्ती_2; i++)
    {
      hb_tag_t *axis_tag_ptr;
      यदि (!c->plan->axes_old_index_tag_map.has (i, &axis_tag_ptr))
        फर्काऊ असत्य;
      hb_tag_t axis_tag = *axis_tag_ptr;

      /* Self-contained pinned axes are removed from fvar/avar; their
       * contribution is baked into the variation tables instead. */
      यदि (!c->plan->axes_index_map.has (i))
        जारी;

      Triple *new_user;
      यदि (c->plan->user_axes_location.has (axis_tag, &new_user))
      {
        /* This axis is being restricted or pinned */
        Triple *old_int;
        यदि (!c->plan->old_intermediates.has (axis_tag, &old_int))
          फर्काऊ असत्य;

        भिन्न a_i = (भिन्न) old_int->minimum;
        भिन्न d_i = (भिन्न) old_int->बीच;
        भिन्न b_i = (भिन्न) old_int->maximum;

        पूर्णाङ्क d_int = roundf (d_i * 16384.f);

        तार्किक is_pinned = new_user->is_point ();

        uint32_t varidx = new_varidx_mapping[i];
        अचिन्हित outer, inner, item_गन्ती_2;

        यदि (varidx == HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
        {
          /* No existing avar2 mapping. Create new VarData. */
          outer = item_vars.add_vardata (1);
          inner = 0;
          item_गन्ती_2 = 1;
          new_varidx_mapping[i] = (outer << 16) | inner;
          default_deltas[i] = 0.f; /* no prior default delta */
        }
        अन्यथा
        {
          outer = varidx >> 16;
          inner = varidx & 0xFFFF;
          item_गन्ती_2 = item_vars.get_item_गन्ती (outer);
        }

        /* Empty-region bias: d_int + round(defaultDelta) */
        पूर्णाङ्क bias = d_int + (पूर्णाङ्क) roundf (default_deltas[i]);
        यदि (bias != 0)
        {
          hb_hashmap_t<hb_tag_t, Triple> empty_region;
          item_vars.add_tuple (outer, std::move (empty_region),
                               inner, bias, item_गन्ती_2);
        }

        यदि (!is_pinned)
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

          अचर hb_vector_t<Axis_मान_Map> &new_mapping = new_mappings[i];

          भिन्न min_f = 0.f, def_f = 0.f, max_f = 0.f;
          यदि (likely (i < fvar_axes.length))
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
          द्विगुण z_old = avar2_normalize_मान ((द्विगुण) old_def,
                                                 (द्विगुण) new_user->minimum,
                                                 (द्विगुण) new_user->बीच,
                                                 (द्विगुण) new_user->maximum);
          z_old = _avar2_map_new_mapping (new_mapping, z_old);
          z_old = (द्विगुण) roundf ((भिन्न) (z_old * 16384.0)) / 16384.0;
          यदि (-1.0 < z_old && z_old < 1.0)
            _avar2_add_knot (knots, z_old, -z_old);

          /* Interior avar v1 breakpoints inside the retained range each put
           * a kink in offset(z). Sampling only {-1, 0, +1, z_old} would
           * linearly interpolate across those kinks. The instantiated
           * mapping keeps exactly the in-range old breakpoints, and the new
           * mapping kinks at each one's output coordinate; add that z with
           * its old intermediate value so offset(z) is reproduced at every
           * kink. */
          hb_vector_t<avar2_offset_knot_t> with_breakpoints (knots);
          लागि (अचर स्वतः &m : new_mapping)
          {
            द्विगुण from = (द्विगुण) m.coords[0].to_float ();
            यदि (from == -1.0 || from == 0.0 || from == 1.0)
              जारी; /* anchors already seeded */
            द्विगुण z = (द्विगुण) m.coords[1].to_float ();
            यदि (!(-1.0 < z && z < 1.0))
              जारी;
            द्विगुण user = avar2_denormalize_मान (from,
                                                    (द्विगुण) new_user->minimum,
                                                    (द्विगुण) new_user->बीच,
                                                    (द्विगुण) new_user->maximum);
            द्विगुण n_old = avar2_normalize_मान (user, old_min, old_def, old_max);
            द्विगुण x_old = (द्विगुण) old_seg_maps[i]->map_float ((भिन्न) n_old);
            x_old = (द्विगुण) roundf ((भिन्न) (x_old * 16384.0)) / 16384.0;
            _avar2_add_knot (with_breakpoints, z, x_old - z);
          }

          यदि (unlikely (knots.in_error () || with_breakpoints.in_error ()))
            फर्काऊ असत्य;

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
                                                       (द्विगुण) new_user->बीच,
                                                       (द्विगुण) new_user->maximum,
                                                       knots);
          यदि (with_breakpoints.length != knots.length)
          {
            अचिन्हित err_with = _avar2_estimate_offset_error (*old_seg_maps[i], new_mapping,
                                                              old_min, old_def, old_max,
                                                              (द्विगुण) new_user->minimum,
                                                              (द्विगुण) new_user->बीच,
                                                              (द्विगुण) new_user->maximum,
                                                              with_breakpoints);
            यदि (err_with <= err)
            {
              knots = std::move (with_breakpoints);
              err = err_with;
            }
          }
          यदि (err > 8)
            DEBUG_सन्देश (SUBSET, nullptr,
                       "avar2 offset compensation is approximate for axis %c%c%c%c: "
                       "max residual %u F2Dot14 units",
                       HB_UNTAG (axis_tag), err);

          /* Synthesize tents. Adjacent tents evaluate to zero at each
           * other's peaks, so each knot's delta is offset(z) - offset(0);
           * the base value offset(0) = d_i is carried by the empty-region
           * bias above. This reduces to the classic pair of tents
           * (-1,-1,0) / (0,+1,+1) when the default is unchanged and there
           * are no interior knots. */
          लागि (अचिन्हित k = 0; k < knots.length; k++)
          {
            द्विगुण z = knots.arrayZ[k].z;
            यदि (z == 0.0) जारी;
            पूर्णाङ्क delta = (पूर्णाङ्क) roundf ((भिन्न) ((knots.arrayZ[k].offset - (द्विगुण) d_i) * 16384.0));
            यदि (!delta) जारी;
            द्विगुण lower, upper;
            यदि (z > 0.0)
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
            यदि (unlikely (!region.set (axis_tag, Triple (lower, z, upper))))
              फर्काऊ असत्य;
            item_vars.add_tuple (outer, std::move (region),
                                 inner, delta, item_गन्ती_2);
          }
        }
      }
      अन्यथा
      {
        /* Free or private axis — not being restricted.
         * If it has a non-zero default delta, add it back as a bias.
         * Skip if this (outer,inner) was already processed (shared varIdx). */
        uint32_t varidx = new_varidx_mapping[i];
        यदि (varidx == HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
          जारी;

        यदि (processed_varidxes.has (varidx))
          जारी;
        processed_varidxes.add (varidx);

        अचिन्हित outer = varidx >> 16;
        अचिन्हित inner = varidx & 0xFFFF;
        पूर्णाङ्क dd = (पूर्णाङ्क) roundf (default_deltas[i]);
        यदि (dd != 0)
        {
          अचिन्हित item_गन्ती_2 = item_vars.get_item_गन्ती (outer);
          hb_hashmap_t<hb_tag_t, Triple> empty_region;
          item_vars.add_tuple (outer, std::move (empty_region),
                               inner, dd, item_गन्ती_2);
        }
      }
    }

    /* 7. Finalize: build region list + convert to varstore */
    यदि (!item_vars.build_region_list ()) फर्काऊ असत्य;
    यदि (!item_vars.as_item_varstore (सत्य /* optimize */,
                                     असत्य /* use_no_variation_idx */))
      फर्काऊ असत्य;

    /* 8. Apply varidx_map optimization remapping */
    अचर स्वतः &opt_varidx_map = item_vars.get_varidx_map ();
    लागि (अचिन्हित i = 0; i < axis_गन्ती_2; i++)
    {
      uint32_t varidx = new_varidx_mapping[i];
      यदि (varidx == HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
        जारी;
      uint32_t *new_idx;
      यदि (opt_varidx_map.has (varidx, &new_idx))
        new_varidx_mapping[i] = *new_idx;
    }

    /* 9. Serialize avarV2Tail. Entries cover the retained axes only;
     * self-contained pinned axes are removed from fvar. */
    avar2_index_map_plan_t index_map_plan;
    यदि (!index_map_plan.init (new_varidx_mapping,
			      c->plan->axes_index_map,
			      axis_गन्ती_2))
      फर्काऊ असत्य;

    स्वतः *tail = c->serializer->allocate_size<avarV2Tail> (avarV2Tail::static_size);
    यदि (unlikely (!tail)) फर्काऊ असत्य;
    यदि (!tail->varIdxMap.serialize_serialize (c->serializer, index_map_plan))
      फर्काऊ असत्य;
    यदि (!tail->varStore.serialize_serialize (c->serializer,
					     item_vars.has_long_word (),
					     c->plan->axis_tags,
					     item_vars.get_region_list (),
					     item_vars.get_vardata_encodings ()))
      फर्काऊ असत्य;

    फर्काऊ सत्य;
#endif
  }

  सार्वजनिक:

  रक्षित:
  FixedVersion<>version;	/* Version of the avar table
				 * initially set to 0x00010000u */
  HBUINT16	reserved;	/* This field is permanently reserved. Set to 0. */
  HBUINT16	axis_गन्ती_2;	/* The number of variation axes in the font. This
				 * must be the same number as axisCount in the
				 * 'fvar' table. */
  SegmentMaps	firstAxisSegmentMaps;

  सार्वजनिक:
  DEFINE_SIZE_MIN (8);
};

} /* namespace OT */


#endif /* HB_OT_VAR_AVAR_TABLE_HH */
