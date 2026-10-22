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

#અન્તર્ભાવ "hb-open-type.hh"
#અન્તર્ભાવ "hb-ot-var-common.hh"
#અન્તર્ભાવ "hb-ot-var-fvar-table.hh"


/*
 * avar -- Axis Variations
 * https://docs.microsoft.com/en-us/typography/opentype/spec/avar
 */

#define HB_OT_TAG_avar HB_TAG('a','v','a','r')


નામાકાશ OT {


/* "Spec": https://github.com/be-fonts/boring-expansion-spec/issues/14 */
સંરચના avarV2Tail
{
  friend સંરચના avar;

  તાર્કિક sanitize (hb_sanitize_context_t *c,
		 અચલ શૂન્ય *base) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (varIdxMap.sanitize (c, base) &&
		  varStore.sanitize (c, base));
  }

  સુરક્ષિત:
  Offset32To<DeltaSetIndexMap>	varIdxMap;	/* Offset from the beginning of 'avar' table. */
  Offset32To<ItemVariationStore>	varStore;	/* Offset from the beginning of 'avar' table. */

  જાહેર:
  DEFINE_SIZE_STATIC (8);
};


સંરચના Axis_કિંમત_Map
{
  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ));
  }

  શૂન્ય set_mapping (દશાંશ from_coord, દશાંશ to_coord)
  {
    coords[0].set_float (from_coord);
    coords[1].set_float (to_coord);
  }

  તાર્કિક is_outside_axis_range (અચલ Triple& axis_range) અચલ
  {
    દ્વિગુણ from_coord = (દ્વિગુણ) coords[0].to_float ();
    પાછા !axis_range.contains (from_coord);
  }

  તાર્કિક must_include () અચલ
  {
    દશાંશ from_coord = coords[0].to_float ();
    દશાંશ to_coord = coords[1].to_float ();
    પાછા (from_coord == -1.f && to_coord == -1.f) ||
           (from_coord == 0.f && to_coord == 0.f) ||
           (from_coord == 1.f && to_coord == 1.f);
  }

  શૂન્ય instantiate (અચલ Triple& axis_range,
                    અચલ Triple& unmapped_range,
                    અચલ TripleDistances& triple_distances)
  {
    દશાંશ from_coord = coords[0].to_float ();
    દશાંશ to_coord = coords[1].to_float ();

    from_coord = renormalize_કિંમત ((દ્વિગુણ) from_coord, unmapped_range, triple_distances);
    to_coord = renormalize_કિંમત ((દ્વિગુણ) to_coord, axis_range, triple_distances);

    /* Malformed input can renormalize outside [-1,1], which would
     * overflow F2DOT14. */
    from_coord = hb_clamp (from_coord, -1.f, 1.f);
    to_coord = hb_clamp (to_coord, -1.f, 1.f);

    coords[0].set_float (from_coord);
    coords[1].set_float (to_coord);
  }

  HB_INTERNAL સ્થિર પૂર્ણાંક cmp (અચલ શૂન્ય *pa, અચલ શૂન્ય *pb)
  {
    અચલ Axis_કિંમત_Map *a = (અચલ Axis_કિંમત_Map *) pa;
    અચલ Axis_કિંમત_Map *b = (અચલ Axis_કિંમત_Map *) pb;

    પૂર્ણાંક a_from = a->coords[0].to_int ();
    પૂર્ણાંક b_from = b->coords[0].to_int ();
    જો (a_from != b_from)
      પાછા a_from - b_from;

    /* this should never be reached. according to the spec, all of the axis
     * value map records for a given axis must have different fromCoord values
     * */
    પૂર્ણાંક a_to = a->coords[1].to_int ();
    પૂર્ણાંક b_to = b->coords[1].to_int ();
    પાછા a_to - b_to;
  }

  તાર્કિક serialize (hb_serialize_context_t *c) અચલ
  {
    TRACE_SERIALIZE (આ);
    return_trace (c->embed (આ));
  }

  જાહેર:
  F2DOT14	coords[2];
//   F2DOT14	fromCoord;	/* A normalized coordinate value obtained using
//				 * default normalization. */
//   F2DOT14	toCoord;	/* The modified, normalized coordinate value. */

  જાહેર:
  DEFINE_SIZE_STATIC (4);
};

સંરચના SegmentMaps : Array16Of<Axis_કિંમત_Map>
{
  દશાંશ map_float (દશાંશ value, unsigned પૂર્ણાંક from_offset = 0, unsigned પૂર્ણાંક to_offset = 1) અચલ
  {
#define fromCoord coords[from_offset].to_float ()
#define toCoord coords[to_offset].to_float ()

    અચલ auto *map = arrayZ;

    /* The following special-cases are not part of OpenType, which requires
     * that at least -1, 0, and +1 must be mapped. But we include these as
     * part of a better error recovery scheme. */
    જો (len < 2)
    {
      જો (!len)
	પાછા value;
      નહીંતર /* len == 1*/
	પાછા value - map[0].fromCoord + map[0].toCoord;
    }

    // At least two mappings now.

    /* CoreText is wild...
     * PingFangUI avar needs all this special-casing...
     * So we implement an extended version of the spec here,
     * which is more robust and more likely to be compatible with
     * the wild. */

    unsigned start = 0;
    unsigned end = len;
    જો (map[start].fromCoord == -1 && map[start].toCoord == -1 && map[start+1].fromCoord == -1)
      start++;
    જો (map[end-1].fromCoord == +1 && map[end-1].toCoord == +1 && map[end-2].fromCoord == +1)
      end--;

    /* Look for exact match first, and do lots of special-casing. */
    unsigned i;
    માટે (i = start; i < end; i++)
      જો (value == map[i].fromCoord)
	તોડો;
    જો (i < end)
    {
      // There's at least one exact match. See if there are more.
      unsigned j = i;
      માટે (; j + 1 < end; j++)
	જો (value != map[j + 1].fromCoord)
	  તોડો;

      // [i,j] inclusive are all exact matches:

      // If there's only one, return it. This is the only spec-compliant case.
      જો (i == j)
	પાછા map[i].toCoord;
      // If there's exactly three, return the middle one.
      જો (i + 2 == j)
	પાછા map[i + 1].toCoord;

      // Ignore the middle ones. Return the one mapping closer to 0.
      જો (value < 0) પાછા map[j].toCoord;
      જો (value > 0) પાછા map[i].toCoord;

      // Mapping 0? CoreText seems confused. It seems to prefer 0 here...
      // So we'll just return the smallest one. lol
      પાછા fabsf (map[i].toCoord) < fabsf (map[j].toCoord) ? map[i].toCoord : map[j].toCoord;

      // Mapping 0? Return one not mapping to 0.
      જો (map[i].toCoord == 0)
	પાછા map[j].toCoord;
      નહીંતર
	પાછા map[i].toCoord;
    }

    /* There's at least two and we're not an exact match. Prepare to lerp. */

    // Find the segment we're in.
    માટે (i = start; i < end; i++)
      જો (value < map[i].fromCoord)
	તોડો;

    જો (i == start)
    {
      // Value before all segments; Shift.
      પાછા value - map[start].fromCoord + map[start].toCoord;
    }
    જો (i == end)
    {
      // Value after all segments; Shift.
      પાછા value - map[end - 1].fromCoord + map[end - 1].toCoord;
    }

    // Actually interpolate.
    auto &before = map[i-1];
    auto &after = map[i];
    દશાંશ denom = after.fromCoord - before.fromCoord; // Can't be zero by now.
    પાછા before.toCoord + ((after.toCoord - before.toCoord) * (value - before.fromCoord)) / denom;

#undef toCoord
#undef fromCoord
  }

  દશાંશ unmap_float (દશાંશ value) અચલ { પાછા map_float (value, 1, 0); }


  // TODO Kill this.
  Triple unmap_axis_range (અચલ Triple& axis_range) અચલ
  {
    દશાંશ unmapped_min = unmap_float (axis_range.minimum);
    દશાંશ unmapped_મધ્ય = unmap_float (axis_range.મધ્ય);
    દશાંશ unmapped_max = unmap_float (axis_range.maximum);

    પાછા Triple{unmapped_min, unmapped_મધ્ય, unmapped_max};
  }

  તાર્કિક subset (hb_subset_context_t *c, hb_tag_t axis_tag,
               hb_vector_t<Axis_કિંમત_Map> *out_mappings = nullptr) અચલ
  {
    TRACE_SUBSET (આ);

    /* This function cannot work on avar2 table (and currently doesn't).
     * We should instead keep the design coords in the shape plan and use
     * those. unmap_axis_range needs to be killed. */

    /* avar mapped normalized axis range. Under avar2, axes_location holds
     * only the self-contained pins for the other tables; avar itself uses
     * the intermediate-space ranges of all restricted axes. */
    અચલ auto &axes_location = c->plan->has_avar2
				? c->plan->old_intermediates
				: c->plan->axes_location;
    Triple *axis_range;
    જો (!axes_location.has (axis_tag, &axis_range))
      પાછા c->serializer->embed (*આ);

    TripleDistances *axis_triple_distances;
    જો (!c->plan->axes_triple_distances.has (axis_tag, &axis_triple_distances))
      return_trace (ખોટું);

    auto *out = c->serializer->start_embed (આ);
    જો (unlikely (!c->serializer->extend_min (out))) return_trace (ખોટું);

    Triple unmapped_range = unmap_axis_range (*axis_range);

    /* create a vector of retained mappings and sort */
    hb_vector_t<Axis_કિંમત_Map> કિંમત_mappings;
    માટે (અચલ auto& _ : as_array ())
    {
      જો (_.is_outside_axis_range (unmapped_range))
        ચાલુ;
      Axis_કિંમત_Map mapping;
      mapping = _;
      mapping.instantiate (*axis_range, unmapped_range, *axis_triple_distances);
      /* (-1, -1), (0, 0), (1, 1) mappings will be added later, so avoid
       * duplicates here */
      જો (mapping.must_include ())
        ચાલુ;
      કિંમત_mappings.push (mapping);
    }

    Axis_કિંમત_Map m;
    m.set_mapping (-1.f, -1.f);
    કિંમત_mappings.push (m);

    m.set_mapping (0.f, 0.f);
    કિંમત_mappings.push (m);

    m.set_mapping (1.f, 1.f);
    કિંમત_mappings.push (m);

    કિંમત_mappings.qsort ();

    જો (unlikely (કિંમત_mappings.in_error ()))
      return_trace (ખોટું);

    માટે (અચલ auto& _ : કિંમત_mappings)
    {
      જો (!_.serialize (c->serializer))
        return_trace (ખોટું);
    }
    જો (!c->serializer->check_assign (out->len, કિંમત_mappings.length, HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (ખોટું);
    /* Hand the instantiated mappings to the caller; avar2 offset
     * compensation needs them to locate the new mapping's kinks. */
    જો (out_mappings)
      *out_mappings = std::move (કિંમત_mappings);
    return_trace (સાચું);
  }

  જાહેર:
  DEFINE_SIZE_ARRAY (2, *આ);
};

/* One knot of the avar2 offset-compensation function
 * offset(z) = inv_renorm(z) - z, in new intermediate space. */
સંરચના avar2_offset_knot_t
{
  દ્વિગુણ z;
  દ્વિગુણ offset;
};

/* Insert a knot keeping the vector sorted by z; first insertion wins on
 * duplicate z. */
સ્થિર અંતઃરેખ શૂન્ય
_avar2_add_knot (hb_vector_t<avar2_offset_knot_t> &knots, દ્વિગુણ z, દ્વિગુણ offset)
{
  unsigned i = 0;
  જ્યારે (i < knots.length && knots.arrayZ[i].z < z) i++;
  જો (i < knots.length && knots.arrayZ[i].z == z) પાછા;
  knots.push (avar2_offset_knot_t {0.0, 0.0});
  જો (unlikely (knots.in_error ())) પાછા;
  માટે (unsigned j = knots.length - 1; j > i; j--)
    knots.arrayZ[j] = knots.arrayZ[j - 1];
  knots.arrayZ[i] = avar2_offset_knot_t {z, offset};
}

/* Piecewise-linear evaluation over the sorted knots. Inputs are always
 * within [-1, +1] and anchor knots at -1/0/+1 always exist. */
સ્થિર અંતઃરેખ દ્વિગુણ
_avar2_eval_offset (અચલ hb_vector_t<avar2_offset_knot_t> &knots, દ્વિગુણ z)
{
  unsigned len = knots.length;
  માટે (unsigned i = 0; i < len; i++)
  {
    જો (z == knots.arrayZ[i].z) પાછા knots.arrayZ[i].offset;
    જો (z < knots.arrayZ[i].z)
    {
      જો (!i) પાછા knots.arrayZ[0].offset;
      અચલ auto &before = knots.arrayZ[i - 1];
      અચલ auto &after = knots.arrayZ[i];
      દ્વિગુણ denom = after.z - before.z;
      પાછા before.offset + (after.offset - before.offset) * (z - before.z) / denom;
    }
  }
  પાછા len ? knots.arrayZ[len - 1].offset : 0.0;
}

/* Piecewise-linear evaluation over instantiated avar v1 mappings, as
 * produced by SegmentMaps::subset (sorted, with -1/0/+1 anchors).
 * Matches SegmentMaps::map_float for such well-formed mappings. */
સ્થિર અંતઃરેખ દ્વિગુણ
_avar2_map_new_mapping (અચલ hb_vector_t<Axis_કિંમત_Map> &mappings, દ્વિગુણ v,
			unsigned from_offset = 0, unsigned to_offset = 1)
{
  unsigned len = mappings.length;
  જો (!len) પાછા v;

  માટે (unsigned i = 0; i < len; i++)
  {
    દ્વિગુણ from = (દ્વિગુણ) mappings.arrayZ[i].coords[from_offset].to_float ();
    જો (v == from)
      પાછા (દ્વિગુણ) mappings.arrayZ[i].coords[to_offset].to_float ();
    જો (v < from)
    {
      દ્વિગુણ to = (દ્વિગુણ) mappings.arrayZ[i].coords[to_offset].to_float ();
      જો (!i) પાછા v - from + to;
      દ્વિગુણ prev_from = (દ્વિગુણ) mappings.arrayZ[i - 1].coords[from_offset].to_float ();
      દ્વિગુણ prev_to = (દ્વિગુણ) mappings.arrayZ[i - 1].coords[to_offset].to_float ();
      દ્વિગુણ denom = from - prev_from;
      જો (denom == 0.0) પાછા prev_to;
      પાછા prev_to + (to - prev_to) * (v - prev_from) / denom;
    }
  }
  પાછા v - (દ્વિગુણ) mappings.arrayZ[len - 1].coords[from_offset].to_float ()
           + (દ્વિગુણ) mappings.arrayZ[len - 1].coords[to_offset].to_float ();
}

/* Inverse of _avar2_map_new_mapping: pull an output coordinate back to a
 * preimage input coordinate. For a non-strictly-monotone mapping this picks
 * one preimage, which is fine for its only use (augmenting the error
 * estimator's sample set). */
સ્થિર અંતઃરેખ દ્વિગુણ
_avar2_unmap_new_mapping (અચલ hb_vector_t<Axis_કિંમત_Map> &mappings, દ્વિગુણ v)
{
  પાછા _avar2_map_new_mapping (mappings, v, 1, 0);
}

/* Plain (non-avar) fvar-style normalization of a user value against a
 * user-space (min, default, max) triple. */
સ્થિર અંતઃરેખ દ્વિગુણ
avar2_normalize_કિંમત (દ્વિગુણ v, દ્વિગુણ min, દ્વિગુણ def, દ્વિગુણ max)
{
  v = hb_clamp (v, min, max);
  જો (v == def) પાછા 0.0;
  જો (v < def) પાછા def == min ? 0.0 : (v - def) / (def - min);
  પાછા def == max ? 0.0 : (v - def) / (max - def);
}

/* Inverse of the above: map a normalized value back to user space. */
સ્થિર અંતઃરેખ દ્વિગુણ
avar2_denormalize_કિંમત (દ્વિગુણ v, દ્વિગુણ min, દ્વિગુણ def, દ્વિગુણ max)
{
  જો (v == 0.0) પાછા def;
  પાછા v < 0.0 ? def + v * (def - min) : def + v * (max - def);
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
સ્થિર અંતઃરેખ unsigned
_avar2_estimate_offset_error (અચલ SegmentMaps &old_seg,
                              અચલ hb_vector_t<Axis_કિંમત_Map> &new_mapping,
                              દ્વિગુણ old_min, દ્વિગુણ old_def, દ્વિગુણ old_max,
                              દ્વિગુણ new_min, દ્વિગુણ new_def, દ્વિગુણ new_max,
                              અચલ hb_vector_t<avar2_offset_knot_t> &knots)
{
  constexpr unsigned samples = 257;
  hb_vector_t<દ્વિગુણ> us;
  જો (unlikely (!us.alloc (samples + old_seg.as_array ().length +
                           new_mapping.length + knots.length)))
    પાછા UINT_MAX; /* estimator only; fail towards "worse" */
  માટે (unsigned i = 0; i < samples; i++)
    us.push (new_min + (new_max - new_min) * i / (samples - 1));
  માટે (અચલ auto &_ : old_seg.as_array ())
    us.push (avar2_denormalize_કિંમત ((દ્વિગુણ) _.coords[0].to_float (),
                                       old_min, old_def, old_max));
  માટે (અચલ auto &_ : new_mapping)
    us.push (avar2_denormalize_કિંમત ((દ્વિગુણ) _.coords[0].to_float (),
                                       new_min, new_def, new_max));
  માટે (અચલ auto &knot : knots)
    us.push (avar2_denormalize_કિંમત (
               _avar2_unmap_new_mapping (new_mapping, knot.z),
               new_min, new_def, new_max));
  જો (unlikely (us.in_error ())) પાછા UINT_MAX;

  unsigned max_err = 0;
  માટે (દ્વિગુણ u : us)
  {
    જો (u < new_min || u > new_max) ચાલુ;
    દ્વિગુણ n_old = avar2_normalize_કિંમત (u, old_min, old_def, old_max);
    દ્વિગુણ old_final = (દ્વિગુણ) old_seg.map_float ((દશાંશ) n_old);
    દ્વિગુણ n_new = avar2_normalize_કિંમત (u, new_min, new_def, new_max);
    દ્વિગુણ z = _avar2_map_new_mapping (new_mapping, n_new);
    દ્વિગુણ new_final = z + _avar2_eval_offset (knots, z);
    પૂર્ણાંક err = abs ((પૂર્ણાંક) roundf ((દશાંશ) (old_final * 16384.0)) -
                   (પૂર્ણાંક) roundf ((દશાંશ) (new_final * 16384.0)));
    જો ((unsigned) err > max_err) max_err = err;
  }
  પાછા max_err;
}

સંરચના avar
{
  સ્થિર constexpr hb_tag_t tableTag = HB_OT_TAG_avar;

  તાર્કિક has_data () અચલ { પાછા version.to_int (); }

  અચલ SegmentMaps* get_segment_maps () અચલ
  { પાછા &firstAxisSegmentMaps; }

  unsigned get_axis_ગણતરી () અચલ
  { પાછા axis_ગણતરી_2; }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    જો (!(version.sanitize (c) &&
	  hb_barrier () &&
	  (version.major == 1
#ifndef HB_NO_AVAR2
	   || version.major == 2
#endif
	   ) &&
	  c->check_struct (આ)))
      return_trace (ખોટું);

    અચલ SegmentMaps *map = &firstAxisSegmentMaps;
    unsigned પૂર્ણાંક ગણતરી = axis_ગણતરી_2;
    માટે (unsigned પૂર્ણાંક i = 0; i < ગણતરી; i++)
    {
      જો (unlikely (!map->sanitize (c)))
	return_trace (ખોટું);
      map = &StructAfter<SegmentMaps> (*map);
    }

#ifndef HB_NO_AVAR2
    જો (version.major < 2)
      return_trace (સાચું);
    hb_barrier ();

    અચલ auto &v2 = * (અચલ avarV2Tail *) map;
    જો (unlikely (!v2.sanitize (c, આ)))
      return_trace (ખોટું);
#endif

    return_trace (સાચું);
  }

  શૂન્ય map_coords_16_16 (પૂર્ણાંક *coords, unsigned પૂર્ણાંક coords_length) અચલ
  {
    unsigned પૂર્ણાંક ગણતરી = hb_min (coords_length, axis_ગણતરી_2);

    અચલ SegmentMaps *map = &firstAxisSegmentMaps;
    માટે (unsigned પૂર્ણાંક i = 0; i < ગણતરી; i++)
    {
      coords[i] = roundf (map->map_float (coords[i] / 65536.f) * 65536.f);
      map = &StructAfter<SegmentMaps> (*map);
    }

#ifndef HB_NO_AVAR2
    જો (version.major < 2)
      પાછા;
    hb_barrier ();

    માટે (; ગણતરી < axis_ગણતરી_2; ગણતરી++)
      map = &StructAfter<SegmentMaps> (*map);

    અચલ auto &v2 = * (અચલ avarV2Tail *) map;

    અચલ auto &varidx_map = આ+v2.varIdxMap;
    અચલ auto &var_store = આ+v2.varStore;
    auto *var_store_cache = var_store.create_cache ();

    hb_vector_t<પૂર્ણાંક> coords_2_14;
    coords_2_14.resize (coords_length);
    માટે (unsigned i = 0; i < coords_length; i++)
      coords_2_14[i] = roundf (coords[i] / 4.f); // 16.16 -> 2.14

    hb_vector_t<પૂર્ણાંક> out;
    out.alloc (coords_length);
    માટે (unsigned i = 0; i < coords_length; i++)
    {
      પૂર્ણાંક v = coords[i];
      uint32_t varidx = varidx_map.map (i);
      દશાંશ delta = var_store.get_delta (varidx, coords_2_14.arrayZ, coords_2_14.length, var_store_cache);
      /* Apply the delta unclamped and clamp only the result to [-1, +1],
       * matching fontTools. Since inputs and results are in [-1, +1],
       * deltas beyond ±2 are equivalent to ±2; clamp to that range only
       * to keep the float->int conversion safe. */
      દશાંશ d = hb_clamp (delta * 4, -(દશાંશ) (1<<17), +(દશાંશ) (1<<17)); // 2.14 -> 16.16
      v += (પૂર્ણાંક) roundf (d);
      v = hb_clamp (v, -(1<<16), +(1<<16));
      out.push (v);
    }
    માટે (unsigned i = 0; i < coords_length; i++)
      coords[i] = out[i];

    OT::ItemVariationStore::destroy_cache (var_store_cache);
#endif
  }

  /* An avar version 2 font is never downgraded to v1: even with a NULL
   * varStore offset (legal; maps like plain v1) the avar2 subsetting path
   * applies, with "no variation" semantics for rows that don't resolve —
   * offset compensation then creates delta rows on demand. */
  તાર્કિક has_v2_data () અચલ { પાછા version.major > 1; }

  /* Resolve the avar2 VarStore and VarIdxMap, for the subset planner.
   * Either pointer may be to the Null object (nullable offsets). */
  તાર્કિક get_v2_store_and_map (અચલ ItemVariationStore **store,
                             અચલ DeltaSetIndexMap **varidx_map) અચલ
  {
#ifndef HB_NO_AVAR2
    જો (version.major < 2) પાછા ખોટું;
    અચલ SegmentMaps *map = &firstAxisSegmentMaps;
    માટે (unsigned i = 0; i < axis_ગણતરી_2; i++)
      map = &StructAfter<SegmentMaps> (*map);
    અચલ auto &v2 = * (અચલ avarV2Tail *) map;
    *store = &(આ+v2.varStore);
    *varidx_map = &(આ+v2.varIdxMap);
    પાછા સાચું;
#નહીંતર
    પાછા ખોટું;
#endif
  }

  // axis normalization is done in 2.14 here
  // TODO: deprecate this API once fonttools is updated to use 16.16 normalization
  તાર્કિક map_coords_2_14 (દશાંશ *coords, unsigned પૂર્ણાંક coords_length,
			તાર્કિક v1_only = ખોટું) અચલ
  {
    hb_vector_t<પૂર્ણાંક> coords_2_14;
    જો (!v1_only && !coords_2_14.resize (coords_length)) પાછા ખોટું;
    unsigned પૂર્ણાંક ગણતરી = hb_min (coords_length, axis_ગણતરી_2);

    અચલ SegmentMaps *map = &firstAxisSegmentMaps;
    માટે (unsigned પૂર્ણાંક i = 0; i < ગણતરી; i++)
    {
      પૂર્ણાંક v = roundf (map->map_float (coords[i]) * 16384.f);
      જો (!v1_only)
	coords_2_14[i] = v;
      coords[i] = v / 16384.f;
      map = &StructAfter<SegmentMaps> (*map);
    }

    જો (v1_only)
      પાછા સાચું;

#ifndef HB_NO_AVAR2
    જો (version.major < 2)
      પાછા સાચું;
    hb_barrier ();

    માટે (; ગણતરી < axis_ગણતરી_2; ગણતરી++)
      map = &StructAfter<SegmentMaps> (*map);

    અચલ auto &v2 = * (અચલ avarV2Tail *) map;

    અચલ auto &varidx_map = આ+v2.varIdxMap;
    અચલ auto &var_store = આ+v2.varStore;
    auto *var_store_cache = var_store.create_cache ();

    માટે (unsigned i = 0; i < coords_length; i++)
    {
      પૂર્ણાંક v = coords_2_14[i];
      uint32_t varidx = varidx_map.map (i);
      દશાંશ delta = var_store.get_delta (varidx, coords_2_14.arrayZ, coords_2_14.length, var_store_cache);
      /* As above: apply the delta unclamped (±2 covers every useful case)
       * and clamp the result to [-1, +1], matching fontTools. */
      v += (પૂર્ણાંક) hb_clamp (roundf (delta), -(દશાંશ) (1<<15), +(દશાંશ) (1<<15));
      v = hb_clamp (v, -(1<<14), +(1<<14));
      coords[i] = v / 16384.f;
    }

    OT::ItemVariationStore::destroy_cache (var_store_cache);
    પાછા સાચું;
#નહીંતર
    પાછા version.major < 2;
#endif
  }

  તાર્કિક subset (hb_subset_context_t *c) અચલ
  {
    TRACE_SUBSET (આ);
    unsigned retained_axis_ગણતરી = c->plan->axes_index_map.get_population ();
    જો (!retained_axis_ગણતરી) //all axes are pinned/dropped
      return_trace (ખોટું);

    avar *out = c->serializer->allocate_min<avar> ();
    જો (unlikely (!out)) return_trace (ખોટું);

    out->version.major = c->plan->has_avar2 ? 2 : 1;
    out->version.minor = 0;
    જો (!c->serializer->check_assign (out->axis_ગણતરી_2, retained_axis_ગણતરી, HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (ખોટું);

    /* For avar2, keep the instantiated v1 mappings around; offset
     * compensation needs them to locate the new mappings' kinks. */
    hb_vector_t<hb_vector_t<Axis_કિંમત_Map>> new_mappings;
    જો (c->plan->has_avar2 && !new_mappings.resize (axis_ગણતરી_2))
      return_trace (ખોટું);

    અચલ hb_map_t& axes_index_map = c->plan->axes_index_map;
    અચલ SegmentMaps *map = &firstAxisSegmentMaps;
    unsigned ગણતરી = axis_ગણતરી_2;
    માટે (unsigned પૂર્ણાંક i = 0; i < ગણતરી; i++)
    {
      જો (axes_index_map.has (i))
      {
        hb_tag_t *axis_tag;
        જો (!c->plan->axes_old_index_tag_map.has (i, &axis_tag))
          return_trace (ખોટું);

        Triple *axis_location;
        જો (c->plan->has_avar2 &&
            c->plan->user_axes_location.has (*axis_tag, &axis_location) &&
            axis_location->is_point ())
        {
          /* Pinned axis in avar2 mode: serialize identity segment map
           * {-1->-1, 0->0, 1->1}. The axis is kept in fvar as hidden,
           * so avar needs a segment map entry for it. */
          auto *identity_map = c->serializer->start_embed<SegmentMaps> ();
          જો (unlikely (!c->serializer->extend_min (identity_map)))
            return_trace (ખોટું);
          Axis_કિંમત_Map m;
          m.set_mapping (-1.f, -1.f);
          જો (!m.serialize (c->serializer)) return_trace (ખોટું);
          m.set_mapping (0.f, 0.f);
          જો (!m.serialize (c->serializer)) return_trace (ખોટું);
          m.set_mapping (1.f, 1.f);
          જો (!m.serialize (c->serializer)) return_trace (ખોટું);
          જો (!c->serializer->check_assign (identity_map->len, 3u,
                                            HB_SERIALIZE_ERROR_INT_OVERFLOW))
            return_trace (ખોટું);
        }
        નહીંતર
        {
          /* Restricted or free axis: use standard SegmentMaps::subset() */
          જો (!map->subset (c, *axis_tag,
                            c->plan->has_avar2 ? &new_mappings[i] : nullptr))
            return_trace (ખોટું);
        }
      }
      map = &StructAfter<SegmentMaps> (*map);
    }

    જો (c->plan->has_avar2)
      return_trace (_subset_avar2 (c, new_mappings));

    return_trace (સાચું);
  }

  ખાનગી:
  સંરચના avar2_index_map_plan_t
  {
    તાર્કિક init (અચલ hb_vector_t<uint32_t> &varidx_mapping,
	       અચલ hb_map_t &axes_index_map,
	       unsigned axis_ગણતરી)
    {
      જો (!output_map.alloc (axes_index_map.get_population ()))
	પાછા ખોટું;

      તાર્કિક has_no_variation = ખોટું;
      unsigned max_outer = 0, max_inner = 0;
      માટે (unsigned i = 0; i < axis_ગણતરી; i++)
      {
	જો (!axes_index_map.has (i)) ચાલુ;
	uint32_t varidx = varidx_mapping[i];
	output_map.push (varidx);
	જો (varidx == HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
	{
	  has_no_variation = સાચું;
	  ચાલુ;
	}
	max_outer = hb_max (max_outer, varidx >> 16);
	max_inner = hb_max (max_inner, varidx & 0xFFFF);
      }
      જો (output_map.in_error () ||
	  output_map.length != axes_index_map.get_population ())
	પાછા ખોટું;

      જો (has_no_variation)
      {
	width = 4;
	inner_bit_ગણતરી = 16;
      }
      નહીંતર
      {
	inner_bit_ગણતરી = hb_max (1u, hb_bit_storage (max_inner));
	unsigned outer_bit_ગણતરી = hb_max (1u, hb_bit_storage (max_outer));
	width = hb_clamp ((inner_bit_ગણતરી + outer_bit_ગણતરી + 7) / 8, 1u, 4u);
	જો (inner_bit_ગણતરી + outer_bit_ગણતરી > width * 8)
	  inner_bit_ગણતરી = width * 8 - outer_bit_ગણતરી;
      }
      પાછા સાચું;
    }

    unsigned get_inner_bit_ગણતરી () અચલ { પાછા inner_bit_ગણતરી; }
    unsigned get_width () અચલ { પાછા width; }
    hb_array_t<અચલ uint32_t> get_output_map () અચલ { પાછા output_map.as_array (); }

    unsigned inner_bit_ગણતરી = 1;
    unsigned width = 1;
    hb_vector_t<uint32_t> output_map;
  };

  તાર્કિક _subset_avar2 (hb_subset_context_t *c,
                      અચલ hb_vector_t<hb_vector_t<Axis_કિંમત_Map>> &new_mappings) અચલ
  {
#જો defined (HB_NO_VAR) || defined (HB_NO_AVAR2)
    /* Not reachable: the plan never sets has_avar2 in these configurations. */
    પાછા ખોટું;
#નહીંતર

    /* 1. Locate original avar2 data, keeping per-axis old segment maps */
    hb_vector_t<અચલ SegmentMaps *> old_seg_maps;
    જો (!old_seg_maps.alloc (axis_ગણતરી_2)) પાછા ખોટું;
    અચલ SegmentMaps *map = &firstAxisSegmentMaps;
    માટે (unsigned i = 0; i < axis_ગણતરી_2; i++)
    {
      old_seg_maps.push (map);
      map = &StructAfter<SegmentMaps> (*map);
    }

    અચલ auto &v2 = * (અચલ avarV2Tail *) map;
    અચલ auto &varidx_map = આ+v2.varIdxMap;
    અચલ auto &var_store = આ+v2.varStore;

    auto fvar_axes = c->plan->source->table.fvar->get_axes ();

    /* 2. Compute default deltas by evaluating VarStore at old defaults */
    hb_vector_t<પૂર્ણાંક> default_coords;
    જો (!default_coords.resize (axis_ગણતરી_2)) પાછા ખોટું;
    માટે (unsigned i = 0; i < axis_ગણતરી_2; i++)
    {
      hb_tag_t *axis_tag;
      જો (c->plan->axes_old_index_tag_map.has (i, &axis_tag) &&
          c->plan->old_intermediates.has (*axis_tag))
      {
        દશાંશ d_i = (દશાંશ) c->plan->old_intermediates.get (*axis_tag).મધ્ય;
        default_coords[i] = roundf (d_i * 16384.f);
      }
      નહીંતર
        default_coords[i] = 0;
    }

    auto *store_cache = var_store.create_cache ();
    hb_vector_t<દશાંશ> default_deltas;
    જો (!default_deltas.resize (axis_ગણતરી_2)) {
      ItemVariationStore::destroy_cache (store_cache);
      પાછા ખોટું;
    }
    માટે (unsigned i = 0; i < axis_ગણતરી_2; i++)
    {
      uint32_t varidx = varidx_map.map (i);
      જો (varidx == HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
        default_deltas[i] = 0.f;
      નહીંતર
        default_deltas[i] = var_store.get_delta (varidx, default_coords.arrayZ,
                                                  default_coords.length, store_cache);
    }
    ItemVariationStore::destroy_cache (store_cache);

    /* 3. Rebase IVS regions */
    item_variations_t item_vars;
    જો (!item_vars.create_from_item_varstore (var_store, c->plan->axes_old_index_tag_map))
      પાછા ખોટું;
    જો (!item_vars.instantiate_tuple_vars (c->plan->old_intermediates,
                                           c->plan->axes_triple_distances,
                                           ખોટું))
      પાછા ખોટું;

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
    જો (!new_varidx_mapping.resize (axis_ગણતરી_2)) પાછા ખોટું;
    માટે (unsigned i = 0; i < axis_ગણતરી_2; i++)
    {
      uint32_t varidx = varidx_map.map (i);
      જો (varidx != HB_OT_LAYOUT_NO_VARIATIONS_INDEX &&
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
    hb_hashmap_t<uint32_t, unsigned> varidx_ref_ગણતરી;
    માટે (unsigned i = 0; i < axis_ગણતરી_2; i++)
    {
      જો (!c->plan->axes_index_map.has (i)) ચાલુ; /* self-contained: dropped */
      uint32_t varidx = new_varidx_mapping[i];
      જો (varidx == HB_OT_LAYOUT_NO_VARIATIONS_INDEX) ચાલુ;
      unsigned *ગણતરી;
      જો (varidx_ref_ગણતરી.has (varidx, &ગણતરી))
        (*ગણતરી)++;
      નહીંતર જો (!varidx_ref_ગણતરી.set (varidx, 1))
        પાછા ખોટું;
    }
    માટે (unsigned i = 0; i < axis_ગણતરી_2; i++)
    {
      hb_tag_t *axis_tag_ptr;
      જો (!c->plan->axes_old_index_tag_map.has (i, &axis_tag_ptr))
        પાછા ખોટું;
      /* Only axes that will receive offset compensation (restricted or
       * pinned) can contaminate a shared row. */
      જો (!c->plan->axes_index_map.has (i) ||
          !c->plan->user_axes_location.has (*axis_tag_ptr))
        ચાલુ;
      uint32_t varidx = new_varidx_mapping[i];
      જો (varidx == HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
        ચાલુ; /* gets a fresh, private VarData in the offset loop below */
      unsigned *ગણતરી;
      જો (!varidx_ref_ગણતરી.has (varidx, &ગણતરી) || *ગણતરી <= 1)
        ચાલુ; /* sole owner: safe to write offsets in place */
      unsigned outer = varidx >> 16;
      unsigned new_inner = item_vars.duplicate_row (outer, varidx & 0xFFFF);
      જો (unlikely (new_inner == (unsigned) -1)) પાછા ખોટું;
      new_varidx_mapping[i] = (outer << 16) | new_inner;
      (*ગણતરી)--;
    }

    /* 6. Add offset compensation tuples.
     * Track processed (outer,inner) pairs to avoid adding duplicate biases
     * when multiple axes share the same varIdx. */
    hb_set_t processed_varidxes;
    માટે (unsigned i = 0; i < axis_ગણતરી_2; i++)
    {
      hb_tag_t *axis_tag_ptr;
      જો (!c->plan->axes_old_index_tag_map.has (i, &axis_tag_ptr))
        પાછા ખોટું;
      hb_tag_t axis_tag = *axis_tag_ptr;

      /* Self-contained pinned axes are removed from fvar/avar; their
       * contribution is baked into the variation tables instead. */
      જો (!c->plan->axes_index_map.has (i))
        ચાલુ;

      Triple *new_user;
      જો (c->plan->user_axes_location.has (axis_tag, &new_user))
      {
        /* This axis is being restricted or pinned */
        Triple *old_int;
        જો (!c->plan->old_intermediates.has (axis_tag, &old_int))
          પાછા ખોટું;

        દશાંશ a_i = (દશાંશ) old_int->minimum;
        દશાંશ d_i = (દશાંશ) old_int->મધ્ય;
        દશાંશ b_i = (દશાંશ) old_int->maximum;

        પૂર્ણાંક d_int = roundf (d_i * 16384.f);

        તાર્કિક is_pinned = new_user->is_point ();

        uint32_t varidx = new_varidx_mapping[i];
        unsigned outer, inner, item_ગણતરી_2;

        જો (varidx == HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
        {
          /* No existing avar2 mapping. Create new VarData. */
          outer = item_vars.add_vardata (1);
          inner = 0;
          item_ગણતરી_2 = 1;
          new_varidx_mapping[i] = (outer << 16) | inner;
          default_deltas[i] = 0.f; /* no prior default delta */
        }
        નહીંતર
        {
          outer = varidx >> 16;
          inner = varidx & 0xFFFF;
          item_ગણતરી_2 = item_vars.get_item_ગણતરી (outer);
        }

        /* Empty-region bias: d_int + round(defaultDelta) */
        પૂર્ણાંક bias = d_int + (પૂર્ણાંક) roundf (default_deltas[i]);
        જો (bias != 0)
        {
          hb_hashmap_t<hb_tag_t, Triple> empty_region;
          item_vars.add_tuple (outer, std::move (empty_region),
                               inner, bias, item_ગણતરી_2);
        }

        જો (!is_pinned)
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
          _avar2_add_knot (knots, -1.0, (દ્વિગુણ) a_i + 1.0);
          _avar2_add_knot (knots, 0.0, (દ્વિગુણ) d_i);
          _avar2_add_knot (knots, 1.0, (દ્વિગુણ) b_i - 1.0);

          અચલ hb_vector_t<Axis_કિંમત_Map> &new_mapping = new_mappings[i];

          દશાંશ min_f = 0.f, def_f = 0.f, max_f = 0.f;
          જો (likely (i < fvar_axes.length))
            fvar_axes[i].get_coordinates (min_f, def_f, max_f);
          દ્વિગુણ old_min = (દ્વિગુણ) min_f;
          દ્વિગુણ old_def = (દ્વિગુણ) def_f;
          દ્વિગુણ old_max = (દ્વિગુણ) max_f;

          /* If the axis default MOVED, inv_renorm also kinks where the OLD
           * default lands in the new space (the old intermediate coordinate
           * crosses 0 there), at
           *   z = z_old  ->  -z_old
           * Omitting that knot (as a plain two-tent encoding would) makes
           * interior coordinates wrong. */
          દ્વિગુણ z_old = avar2_normalize_કિંમત ((દ્વિગુણ) old_def,
                                                 (દ્વિગુણ) new_user->minimum,
                                                 (દ્વિગુણ) new_user->મધ્ય,
                                                 (દ્વિગુણ) new_user->maximum);
          z_old = _avar2_map_new_mapping (new_mapping, z_old);
          z_old = (દ્વિગુણ) roundf ((દશાંશ) (z_old * 16384.0)) / 16384.0;
          જો (-1.0 < z_old && z_old < 1.0)
            _avar2_add_knot (knots, z_old, -z_old);

          /* Interior avar v1 breakpoints inside the retained range each put
           * a kink in offset(z). Sampling only {-1, 0, +1, z_old} would
           * linearly interpolate across those kinks. The instantiated
           * mapping keeps exactly the in-range old breakpoints, and the new
           * mapping kinks at each one's output coordinate; add that z with
           * its old intermediate value so offset(z) is reproduced at every
           * kink. */
          hb_vector_t<avar2_offset_knot_t> with_breakpoints (knots);
          માટે (અચલ auto &m : new_mapping)
          {
            દ્વિગુણ from = (દ્વિગુણ) m.coords[0].to_float ();
            જો (from == -1.0 || from == 0.0 || from == 1.0)
              ચાલુ; /* anchors already seeded */
            દ્વિગુણ z = (દ્વિગુણ) m.coords[1].to_float ();
            જો (!(-1.0 < z && z < 1.0))
              ચાલુ;
            દ્વિગુણ user = avar2_denormalize_કિંમત (from,
                                                    (દ્વિગુણ) new_user->minimum,
                                                    (દ્વિગુણ) new_user->મધ્ય,
                                                    (દ્વિગુણ) new_user->maximum);
            દ્વિગુણ n_old = avar2_normalize_કિંમત (user, old_min, old_def, old_max);
            દ્વિગુણ x_old = (દ્વિગુણ) old_seg_maps[i]->map_float ((દશાંશ) n_old);
            x_old = (દ્વિગુણ) roundf ((દશાંશ) (x_old * 16384.0)) / 16384.0;
            _avar2_add_knot (with_breakpoints, z, x_old - z);
          }

          જો (unlikely (knots.in_error () || with_breakpoints.in_error ()))
            પાછા ખોટું;

          /* Extra tents cost F2Dot14 rounding, so for a steep segment they
           * can add more quantization noise than the structural error they
           * remove. Keep the interior breakpoints only when they do not
           * increase the estimated residual; this makes the collection a
           * strict (never-worse) improvement over the {-1, 0, +1, z_old}
           * anchors. Warn when even the better choice is not bit-exact (a
           * steep retained segment that cannot be reproduced in F2Dot14). */
          unsigned err = _avar2_estimate_offset_error (*old_seg_maps[i], new_mapping,
                                                       old_min, old_def, old_max,
                                                       (દ્વિગુણ) new_user->minimum,
                                                       (દ્વિગુણ) new_user->મધ્ય,
                                                       (દ્વિગુણ) new_user->maximum,
                                                       knots);
          જો (with_breakpoints.length != knots.length)
          {
            unsigned err_with = _avar2_estimate_offset_error (*old_seg_maps[i], new_mapping,
                                                              old_min, old_def, old_max,
                                                              (દ્વિગુણ) new_user->minimum,
                                                              (દ્વિગુણ) new_user->મધ્ય,
                                                              (દ્વિગુણ) new_user->maximum,
                                                              with_breakpoints);
            જો (err_with <= err)
            {
              knots = std::move (with_breakpoints);
              err = err_with;
            }
          }
          જો (err > 8)
            DEBUG_સંદેશ (SUBSET, nullptr,
                       "avar2 offset compensation is approximate for axis %c%c%c%c: "
                       "max residual %u F2Dot14 units",
                       HB_UNTAG (axis_tag), err);

          /* Synthesize tents. Adjacent tents evaluate to zero at each
           * other's peaks, so each knot's delta is offset(z) - offset(0);
           * the base value offset(0) = d_i is carried by the empty-region
           * bias above. This reduces to the classic pair of tents
           * (-1,-1,0) / (0,+1,+1) when the default is unchanged and there
           * are no interior knots. */
          માટે (unsigned k = 0; k < knots.length; k++)
          {
            દ્વિગુણ z = knots.arrayZ[k].z;
            જો (z == 0.0) ચાલુ;
            પૂર્ણાંક delta = (પૂર્ણાંક) roundf ((દશાંશ) ((knots.arrayZ[k].offset - (દ્વિગુણ) d_i) * 16384.0));
            જો (!delta) ચાલુ;
            દ્વિગુણ lower, upper;
            જો (z > 0.0)
            {
              lower = knots.arrayZ[k - 1].z;
              upper = k + 1 < knots.length ? knots.arrayZ[k + 1].z : z;
            }
            નહીંતર
            {
              upper = knots.arrayZ[k + 1].z;
              lower = k > 0 ? knots.arrayZ[k - 1].z : z;
            }
            hb_hashmap_t<hb_tag_t, Triple> region;
            જો (unlikely (!region.set (axis_tag, Triple (lower, z, upper))))
              પાછા ખોટું;
            item_vars.add_tuple (outer, std::move (region),
                                 inner, delta, item_ગણતરી_2);
          }
        }
      }
      નહીંતર
      {
        /* Free or private axis — not being restricted.
         * If it has a non-zero default delta, add it back as a bias.
         * Skip if this (outer,inner) was already processed (shared varIdx). */
        uint32_t varidx = new_varidx_mapping[i];
        જો (varidx == HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
          ચાલુ;

        જો (processed_varidxes.has (varidx))
          ચાલુ;
        processed_varidxes.add (varidx);

        unsigned outer = varidx >> 16;
        unsigned inner = varidx & 0xFFFF;
        પૂર્ણાંક dd = (પૂર્ણાંક) roundf (default_deltas[i]);
        જો (dd != 0)
        {
          unsigned item_ગણતરી_2 = item_vars.get_item_ગણતરી (outer);
          hb_hashmap_t<hb_tag_t, Triple> empty_region;
          item_vars.add_tuple (outer, std::move (empty_region),
                               inner, dd, item_ગણતરી_2);
        }
      }
    }

    /* 7. Finalize: build region list + convert to varstore */
    જો (!item_vars.build_region_list ()) પાછા ખોટું;
    જો (!item_vars.as_item_varstore (સાચું /* optimize */,
                                     ખોટું /* use_no_variation_idx */))
      પાછા ખોટું;

    /* 8. Apply varidx_map optimization remapping */
    અચલ auto &opt_varidx_map = item_vars.get_varidx_map ();
    માટે (unsigned i = 0; i < axis_ગણતરી_2; i++)
    {
      uint32_t varidx = new_varidx_mapping[i];
      જો (varidx == HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
        ચાલુ;
      uint32_t *new_idx;
      જો (opt_varidx_map.has (varidx, &new_idx))
        new_varidx_mapping[i] = *new_idx;
    }

    /* 9. Serialize avarV2Tail. Entries cover the retained axes only;
     * self-contained pinned axes are removed from fvar. */
    avar2_index_map_plan_t index_map_plan;
    જો (!index_map_plan.init (new_varidx_mapping,
			      c->plan->axes_index_map,
			      axis_ગણતરી_2))
      પાછા ખોટું;

    auto *tail = c->serializer->allocate_size<avarV2Tail> (avarV2Tail::static_size);
    જો (unlikely (!tail)) પાછા ખોટું;
    જો (!tail->varIdxMap.serialize_serialize (c->serializer, index_map_plan))
      પાછા ખોટું;
    જો (!tail->varStore.serialize_serialize (c->serializer,
					     item_vars.has_long_word (),
					     c->plan->axis_tags,
					     item_vars.get_region_list (),
					     item_vars.get_vardata_encodings ()))
      પાછા ખોટું;

    પાછા સાચું;
#endif
  }

  જાહેર:

  સુરક્ષિત:
  FixedVersion<>version;	/* Version of the avar table
				 * initially set to 0x00010000u */
  HBUINT16	reserved;	/* This field is permanently reserved. Set to 0. */
  HBUINT16	axis_ગણતરી_2;	/* The number of variation axes in the font. This
				 * must be the same number as axisCount in the
				 * 'fvar' table. */
  SegmentMaps	firstAxisSegmentMaps;

  જાહેર:
  DEFINE_SIZE_MIN (8);
};

} /* namespace OT */


#endif /* HB_OT_VAR_AVAR_TABLE_HH */
