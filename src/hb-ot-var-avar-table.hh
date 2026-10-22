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

#അന്തര്ഭാവ "hb-open-type.hh"
#അന്തര്ഭാവ "hb-ot-var-common.hh"
#അന്തര്ഭാവ "hb-ot-var-fvar-table.hh"


/*
 * avar -- Axis Variations
 * https://docs.microsoft.com/en-us/typography/opentype/spec/avar
 */

#define HB_OT_TAG_avar HB_TAG('a','v','a','r')


നാമാകാശ OT {


/* "Spec": https://github.com/be-fonts/boring-expansion-spec/issues/14 */
ഘടന avarV2Tail
{
  friend ഘടന avar;

  തർക്കം sanitize (hb_sanitize_context_t *c,
		 സ്ഥിരം ശൂന്യം *base) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (varIdxMap.sanitize (c, base) &&
		  varStore.sanitize (c, base));
  }

  സംരക്ഷിത:
  Offset32To<DeltaSetIndexMap>	varIdxMap;	/* Offset from the beginning of 'avar' table. */
  Offset32To<ItemVariationStore>	varStore;	/* Offset from the beginning of 'avar' table. */

  പൊതു:
  DEFINE_SIZE_STATIC (8);
};


ഘടന Axis_മൂല്യം_Map
{
  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_struct (ഇത്));
  }

  ശൂന്യം set_mapping (ദശാംശം from_coord, ദശാംശം to_coord)
  {
    coords[0].set_float (from_coord);
    coords[1].set_float (to_coord);
  }

  തർക്കം is_outside_axis_range (സ്ഥിരം Triple& axis_range) സ്ഥിരം
  {
    ഇരട്ട from_coord = (ഇരട്ട) coords[0].to_float ();
    തിരികെ !axis_range.contains (from_coord);
  }

  തർക്കം must_include () സ്ഥിരം
  {
    ദശാംശം from_coord = coords[0].to_float ();
    ദശാംശം to_coord = coords[1].to_float ();
    തിരികെ (from_coord == -1.f && to_coord == -1.f) ||
           (from_coord == 0.f && to_coord == 0.f) ||
           (from_coord == 1.f && to_coord == 1.f);
  }

  ശൂന്യം instantiate (സ്ഥിരം Triple& axis_range,
                    സ്ഥിരം Triple& unmapped_range,
                    സ്ഥിരം TripleDistances& triple_distances)
  {
    ദശാംശം from_coord = coords[0].to_float ();
    ദശാംശം to_coord = coords[1].to_float ();

    from_coord = renormalize_മൂല്യം ((ഇരട്ട) from_coord, unmapped_range, triple_distances);
    to_coord = renormalize_മൂല്യം ((ഇരട്ട) to_coord, axis_range, triple_distances);

    /* Malformed input can renormalize outside [-1,1], which would
     * overflow F2DOT14. */
    from_coord = hb_clamp (from_coord, -1.f, 1.f);
    to_coord = hb_clamp (to_coord, -1.f, 1.f);

    coords[0].set_float (from_coord);
    coords[1].set_float (to_coord);
  }

  HB_INTERNAL സ്ഥിതം പൂർണ്ണസംഖ്യ cmp (സ്ഥിരം ശൂന്യം *pa, സ്ഥിരം ശൂന്യം *pb)
  {
    സ്ഥിരം Axis_മൂല്യം_Map *a = (സ്ഥിരം Axis_മൂല്യം_Map *) pa;
    സ്ഥിരം Axis_മൂല്യം_Map *b = (സ്ഥിരം Axis_മൂല്യം_Map *) pb;

    പൂർണ്ണസംഖ്യ a_from = a->coords[0].to_int ();
    പൂർണ്ണസംഖ്യ b_from = b->coords[0].to_int ();
    എങ്കിൽ (a_from != b_from)
      തിരികെ a_from - b_from;

    /* this should never be reached. according to the spec, all of the axis
     * value map records for a given axis must have different fromCoord values
     * */
    പൂർണ്ണസംഖ്യ a_to = a->coords[1].to_int ();
    പൂർണ്ണസംഖ്യ b_to = b->coords[1].to_int ();
    തിരികെ a_to - b_to;
  }

  തർക്കം serialize (hb_serialize_context_t *c) സ്ഥിരം
  {
    TRACE_SERIALIZE (ഇത്);
    return_trace (c->embed (ഇത്));
  }

  പൊതു:
  F2DOT14	coords[2];
//   F2DOT14	fromCoord;	/* A normalized coordinate value obtained using
//				 * default normalization. */
//   F2DOT14	toCoord;	/* The modified, normalized coordinate value. */

  പൊതു:
  DEFINE_SIZE_STATIC (4);
};

ഘടന SegmentMaps : Array16Of<Axis_മൂല്യം_Map>
{
  ദശാംശം map_float (ദശാംശം value, unsigned പൂർണ്ണസംഖ്യ from_offset = 0, unsigned പൂർണ്ണസംഖ്യ to_offset = 1) സ്ഥിരം
  {
#define fromCoord coords[from_offset].to_float ()
#define toCoord coords[to_offset].to_float ()

    സ്ഥിരം auto *map = arrayZ;

    /* The following special-cases are not part of OpenType, which requires
     * that at least -1, 0, and +1 must be mapped. But we include these as
     * part of a better error recovery scheme. */
    എങ്കിൽ (len < 2)
    {
      എങ്കിൽ (!len)
	തിരികെ value;
      അല്ലെങ്കിൽ /* len == 1*/
	തിരികെ value - map[0].fromCoord + map[0].toCoord;
    }

    // At least two mappings now.

    /* CoreText is wild...
     * PingFangUI avar needs all this special-casing...
     * So we implement an extended version of the spec here,
     * which is more robust and more likely to be compatible with
     * the wild. */

    unsigned start = 0;
    unsigned end = len;
    എങ്കിൽ (map[start].fromCoord == -1 && map[start].toCoord == -1 && map[start+1].fromCoord == -1)
      start++;
    എങ്കിൽ (map[end-1].fromCoord == +1 && map[end-1].toCoord == +1 && map[end-2].fromCoord == +1)
      end--;

    /* Look for exact match first, and do lots of special-casing. */
    unsigned i;
    വേണ്ടി (i = start; i < end; i++)
      എങ്കിൽ (value == map[i].fromCoord)
	നിർത്തുക;
    എങ്കിൽ (i < end)
    {
      // There's at least one exact match. See if there are more.
      unsigned j = i;
      വേണ്ടി (; j + 1 < end; j++)
	എങ്കിൽ (value != map[j + 1].fromCoord)
	  നിർത്തുക;

      // [i,j] inclusive are all exact matches:

      // If there's only one, return it. This is the only spec-compliant case.
      എങ്കിൽ (i == j)
	തിരികെ map[i].toCoord;
      // If there's exactly three, return the middle one.
      എങ്കിൽ (i + 2 == j)
	തിരികെ map[i + 1].toCoord;

      // Ignore the middle ones. Return the one mapping closer to 0.
      എങ്കിൽ (value < 0) തിരികെ map[j].toCoord;
      എങ്കിൽ (value > 0) തിരികെ map[i].toCoord;

      // Mapping 0? CoreText seems confused. It seems to prefer 0 here...
      // So we'll just return the smallest one. lol
      തിരികെ fabsf (map[i].toCoord) < fabsf (map[j].toCoord) ? map[i].toCoord : map[j].toCoord;

      // Mapping 0? Return one not mapping to 0.
      എങ്കിൽ (map[i].toCoord == 0)
	തിരികെ map[j].toCoord;
      അല്ലെങ്കിൽ
	തിരികെ map[i].toCoord;
    }

    /* There's at least two and we're not an exact match. Prepare to lerp. */

    // Find the segment we're in.
    വേണ്ടി (i = start; i < end; i++)
      എങ്കിൽ (value < map[i].fromCoord)
	നിർത്തുക;

    എങ്കിൽ (i == start)
    {
      // Value before all segments; Shift.
      തിരികെ value - map[start].fromCoord + map[start].toCoord;
    }
    എങ്കിൽ (i == end)
    {
      // Value after all segments; Shift.
      തിരികെ value - map[end - 1].fromCoord + map[end - 1].toCoord;
    }

    // Actually interpolate.
    auto &before = map[i-1];
    auto &after = map[i];
    ദശാംശം denom = after.fromCoord - before.fromCoord; // Can't be zero by now.
    തിരികെ before.toCoord + ((after.toCoord - before.toCoord) * (value - before.fromCoord)) / denom;

#undef toCoord
#undef fromCoord
  }

  ദശാംശം unmap_float (ദശാംശം value) സ്ഥിരം { തിരികെ map_float (value, 1, 0); }


  // TODO Kill this.
  Triple unmap_axis_range (സ്ഥിരം Triple& axis_range) സ്ഥിരം
  {
    ദശാംശം unmapped_min = unmap_float (axis_range.minimum);
    ദശാംശം unmapped_നടുവ് = unmap_float (axis_range.നടുവ്);
    ദശാംശം unmapped_max = unmap_float (axis_range.maximum);

    തിരികെ Triple{unmapped_min, unmapped_നടുവ്, unmapped_max};
  }

  തർക്കം subset (hb_subset_context_t *c, hb_tag_t axis_tag,
               hb_vector_t<Axis_മൂല്യം_Map> *out_mappings = nullptr) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);

    /* This function cannot work on avar2 table (and currently doesn't).
     * We should instead keep the design coords in the shape plan and use
     * those. unmap_axis_range needs to be killed. */

    /* avar mapped normalized axis range. Under avar2, axes_location holds
     * only the self-contained pins for the other tables; avar itself uses
     * the intermediate-space ranges of all restricted axes. */
    സ്ഥിരം auto &axes_location = c->plan->has_avar2
				? c->plan->old_intermediates
				: c->plan->axes_location;
    Triple *axis_range;
    എങ്കിൽ (!axes_location.has (axis_tag, &axis_range))
      തിരികെ c->serializer->embed (*ഇത്);

    TripleDistances *axis_triple_distances;
    എങ്കിൽ (!c->plan->axes_triple_distances.has (axis_tag, &axis_triple_distances))
      return_trace (അസത്യം);

    auto *out = c->serializer->start_embed (ഇത്);
    എങ്കിൽ (unlikely (!c->serializer->extend_min (out))) return_trace (അസത്യം);

    Triple unmapped_range = unmap_axis_range (*axis_range);

    /* create a vector of retained mappings and sort */
    hb_vector_t<Axis_മൂല്യം_Map> മൂല്യം_mappings;
    വേണ്ടി (സ്ഥിരം auto& _ : as_array ())
    {
      എങ്കിൽ (_.is_outside_axis_range (unmapped_range))
        തുടരുക;
      Axis_മൂല്യം_Map mapping;
      mapping = _;
      mapping.instantiate (*axis_range, unmapped_range, *axis_triple_distances);
      /* (-1, -1), (0, 0), (1, 1) mappings will be added later, so avoid
       * duplicates here */
      എങ്കിൽ (mapping.must_include ())
        തുടരുക;
      മൂല്യം_mappings.push (mapping);
    }

    Axis_മൂല്യം_Map m;
    m.set_mapping (-1.f, -1.f);
    മൂല്യം_mappings.push (m);

    m.set_mapping (0.f, 0.f);
    മൂല്യം_mappings.push (m);

    m.set_mapping (1.f, 1.f);
    മൂല്യം_mappings.push (m);

    മൂല്യം_mappings.qsort ();

    എങ്കിൽ (unlikely (മൂല്യം_mappings.in_error ()))
      return_trace (അസത്യം);

    വേണ്ടി (സ്ഥിരം auto& _ : മൂല്യം_mappings)
    {
      എങ്കിൽ (!_.serialize (c->serializer))
        return_trace (അസത്യം);
    }
    എങ്കിൽ (!c->serializer->check_assign (out->len, മൂല്യം_mappings.length, HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (അസത്യം);
    /* Hand the instantiated mappings to the caller; avar2 offset
     * compensation needs them to locate the new mapping's kinks. */
    എങ്കിൽ (out_mappings)
      *out_mappings = std::move (മൂല്യം_mappings);
    return_trace (സത്യം);
  }

  പൊതു:
  DEFINE_SIZE_ARRAY (2, *ഇത്);
};

/* One knot of the avar2 offset-compensation function
 * offset(z) = inv_renorm(z) - z, in new intermediate space. */
ഘടന avar2_offset_knot_t
{
  ഇരട്ട z;
  ഇരട്ട offset;
};

/* Insert a knot keeping the vector sorted by z; first insertion wins on
 * duplicate z. */
സ്ഥിതം വരിയിൽ ശൂന്യം
_avar2_add_knot (hb_vector_t<avar2_offset_knot_t> &knots, ഇരട്ട z, ഇരട്ട offset)
{
  unsigned i = 0;
  ആകുമ്പോൾ (i < knots.length && knots.arrayZ[i].z < z) i++;
  എങ്കിൽ (i < knots.length && knots.arrayZ[i].z == z) തിരികെ;
  knots.push (avar2_offset_knot_t {0.0, 0.0});
  എങ്കിൽ (unlikely (knots.in_error ())) തിരികെ;
  വേണ്ടി (unsigned j = knots.length - 1; j > i; j--)
    knots.arrayZ[j] = knots.arrayZ[j - 1];
  knots.arrayZ[i] = avar2_offset_knot_t {z, offset};
}

/* Piecewise-linear evaluation over the sorted knots. Inputs are always
 * within [-1, +1] and anchor knots at -1/0/+1 always exist. */
സ്ഥിതം വരിയിൽ ഇരട്ട
_avar2_eval_offset (സ്ഥിരം hb_vector_t<avar2_offset_knot_t> &knots, ഇരട്ട z)
{
  unsigned len = knots.length;
  വേണ്ടി (unsigned i = 0; i < len; i++)
  {
    എങ്കിൽ (z == knots.arrayZ[i].z) തിരികെ knots.arrayZ[i].offset;
    എങ്കിൽ (z < knots.arrayZ[i].z)
    {
      എങ്കിൽ (!i) തിരികെ knots.arrayZ[0].offset;
      സ്ഥിരം auto &before = knots.arrayZ[i - 1];
      സ്ഥിരം auto &after = knots.arrayZ[i];
      ഇരട്ട denom = after.z - before.z;
      തിരികെ before.offset + (after.offset - before.offset) * (z - before.z) / denom;
    }
  }
  തിരികെ len ? knots.arrayZ[len - 1].offset : 0.0;
}

/* Piecewise-linear evaluation over instantiated avar v1 mappings, as
 * produced by SegmentMaps::subset (sorted, with -1/0/+1 anchors).
 * Matches SegmentMaps::map_float for such well-formed mappings. */
സ്ഥിതം വരിയിൽ ഇരട്ട
_avar2_map_new_mapping (സ്ഥിരം hb_vector_t<Axis_മൂല്യം_Map> &mappings, ഇരട്ട v,
			unsigned from_offset = 0, unsigned to_offset = 1)
{
  unsigned len = mappings.length;
  എങ്കിൽ (!len) തിരികെ v;

  വേണ്ടി (unsigned i = 0; i < len; i++)
  {
    ഇരട്ട from = (ഇരട്ട) mappings.arrayZ[i].coords[from_offset].to_float ();
    എങ്കിൽ (v == from)
      തിരികെ (ഇരട്ട) mappings.arrayZ[i].coords[to_offset].to_float ();
    എങ്കിൽ (v < from)
    {
      ഇരട്ട to = (ഇരട്ട) mappings.arrayZ[i].coords[to_offset].to_float ();
      എങ്കിൽ (!i) തിരികെ v - from + to;
      ഇരട്ട prev_from = (ഇരട്ട) mappings.arrayZ[i - 1].coords[from_offset].to_float ();
      ഇരട്ട prev_to = (ഇരട്ട) mappings.arrayZ[i - 1].coords[to_offset].to_float ();
      ഇരട്ട denom = from - prev_from;
      എങ്കിൽ (denom == 0.0) തിരികെ prev_to;
      തിരികെ prev_to + (to - prev_to) * (v - prev_from) / denom;
    }
  }
  തിരികെ v - (ഇരട്ട) mappings.arrayZ[len - 1].coords[from_offset].to_float ()
           + (ഇരട്ട) mappings.arrayZ[len - 1].coords[to_offset].to_float ();
}

/* Inverse of _avar2_map_new_mapping: pull an output coordinate back to a
 * preimage input coordinate. For a non-strictly-monotone mapping this picks
 * one preimage, which is fine for its only use (augmenting the error
 * estimator's sample set). */
സ്ഥിതം വരിയിൽ ഇരട്ട
_avar2_unmap_new_mapping (സ്ഥിരം hb_vector_t<Axis_മൂല്യം_Map> &mappings, ഇരട്ട v)
{
  തിരികെ _avar2_map_new_mapping (mappings, v, 1, 0);
}

/* Plain (non-avar) fvar-style normalization of a user value against a
 * user-space (min, default, max) triple. */
സ്ഥിതം വരിയിൽ ഇരട്ട
avar2_normalize_മൂല്യം (ഇരട്ട v, ഇരട്ട min, ഇരട്ട def, ഇരട്ട max)
{
  v = hb_clamp (v, min, max);
  എങ്കിൽ (v == def) തിരികെ 0.0;
  എങ്കിൽ (v < def) തിരികെ def == min ? 0.0 : (v - def) / (def - min);
  തിരികെ def == max ? 0.0 : (v - def) / (max - def);
}

/* Inverse of the above: map a normalized value back to user space. */
സ്ഥിതം വരിയിൽ ഇരട്ട
avar2_denormalize_മൂല്യം (ഇരട്ട v, ഇരട്ട min, ഇരട്ട def, ഇരട്ട max)
{
  എങ്കിൽ (v == 0.0) തിരികെ def;
  തിരികെ v < 0.0 ? def + v * (def - min) : def + v * (max - def);
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
സ്ഥിതം വരിയിൽ unsigned
_avar2_estimate_offset_error (സ്ഥിരം SegmentMaps &old_seg,
                              സ്ഥിരം hb_vector_t<Axis_മൂല്യം_Map> &new_mapping,
                              ഇരട്ട old_min, ഇരട്ട old_def, ഇരട്ട old_max,
                              ഇരട്ട new_min, ഇരട്ട new_def, ഇരട്ട new_max,
                              സ്ഥിരം hb_vector_t<avar2_offset_knot_t> &knots)
{
  constexpr unsigned samples = 257;
  hb_vector_t<ഇരട്ട> us;
  എങ്കിൽ (unlikely (!us.alloc (samples + old_seg.as_array ().length +
                           new_mapping.length + knots.length)))
    തിരികെ UINT_MAX; /* estimator only; fail towards "worse" */
  വേണ്ടി (unsigned i = 0; i < samples; i++)
    us.push (new_min + (new_max - new_min) * i / (samples - 1));
  വേണ്ടി (സ്ഥിരം auto &_ : old_seg.as_array ())
    us.push (avar2_denormalize_മൂല്യം ((ഇരട്ട) _.coords[0].to_float (),
                                       old_min, old_def, old_max));
  വേണ്ടി (സ്ഥിരം auto &_ : new_mapping)
    us.push (avar2_denormalize_മൂല്യം ((ഇരട്ട) _.coords[0].to_float (),
                                       new_min, new_def, new_max));
  വേണ്ടി (സ്ഥിരം auto &knot : knots)
    us.push (avar2_denormalize_മൂല്യം (
               _avar2_unmap_new_mapping (new_mapping, knot.z),
               new_min, new_def, new_max));
  എങ്കിൽ (unlikely (us.in_error ())) തിരികെ UINT_MAX;

  unsigned max_err = 0;
  വേണ്ടി (ഇരട്ട u : us)
  {
    എങ്കിൽ (u < new_min || u > new_max) തുടരുക;
    ഇരട്ട n_old = avar2_normalize_മൂല്യം (u, old_min, old_def, old_max);
    ഇരട്ട old_final = (ഇരട്ട) old_seg.map_float ((ദശാംശം) n_old);
    ഇരട്ട n_new = avar2_normalize_മൂല്യം (u, new_min, new_def, new_max);
    ഇരട്ട z = _avar2_map_new_mapping (new_mapping, n_new);
    ഇരട്ട new_final = z + _avar2_eval_offset (knots, z);
    പൂർണ്ണസംഖ്യ err = abs ((പൂർണ്ണസംഖ്യ) roundf ((ദശാംശം) (old_final * 16384.0)) -
                   (പൂർണ്ണസംഖ്യ) roundf ((ദശാംശം) (new_final * 16384.0)));
    എങ്കിൽ ((unsigned) err > max_err) max_err = err;
  }
  തിരികെ max_err;
}

ഘടന avar
{
  സ്ഥിതം constexpr hb_tag_t tableTag = HB_OT_TAG_avar;

  തർക്കം has_data () സ്ഥിരം { തിരികെ version.to_int (); }

  സ്ഥിരം SegmentMaps* get_segment_maps () സ്ഥിരം
  { തിരികെ &firstAxisSegmentMaps; }

  unsigned get_axis_എണ്ണം () സ്ഥിരം
  { തിരികെ axis_എണ്ണം_2; }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    എങ്കിൽ (!(version.sanitize (c) &&
	  hb_barrier () &&
	  (version.major == 1
#ifndef HB_NO_AVAR2
	   || version.major == 2
#endif
	   ) &&
	  c->check_struct (ഇത്)))
      return_trace (അസത്യം);

    സ്ഥിരം SegmentMaps *map = &firstAxisSegmentMaps;
    unsigned പൂർണ്ണസംഖ്യ എണ്ണം = axis_എണ്ണം_2;
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < എണ്ണം; i++)
    {
      എങ്കിൽ (unlikely (!map->sanitize (c)))
	return_trace (അസത്യം);
      map = &StructAfter<SegmentMaps> (*map);
    }

#ifndef HB_NO_AVAR2
    എങ്കിൽ (version.major < 2)
      return_trace (സത്യം);
    hb_barrier ();

    സ്ഥിരം auto &v2 = * (സ്ഥിരം avarV2Tail *) map;
    എങ്കിൽ (unlikely (!v2.sanitize (c, ഇത്)))
      return_trace (അസത്യം);
#endif

    return_trace (സത്യം);
  }

  ശൂന്യം map_coords_16_16 (പൂർണ്ണസംഖ്യ *coords, unsigned പൂർണ്ണസംഖ്യ coords_length) സ്ഥിരം
  {
    unsigned പൂർണ്ണസംഖ്യ എണ്ണം = hb_min (coords_length, axis_എണ്ണം_2);

    സ്ഥിരം SegmentMaps *map = &firstAxisSegmentMaps;
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < എണ്ണം; i++)
    {
      coords[i] = roundf (map->map_float (coords[i] / 65536.f) * 65536.f);
      map = &StructAfter<SegmentMaps> (*map);
    }

#ifndef HB_NO_AVAR2
    എങ്കിൽ (version.major < 2)
      തിരികെ;
    hb_barrier ();

    വേണ്ടി (; എണ്ണം < axis_എണ്ണം_2; എണ്ണം++)
      map = &StructAfter<SegmentMaps> (*map);

    സ്ഥിരം auto &v2 = * (സ്ഥിരം avarV2Tail *) map;

    സ്ഥിരം auto &varidx_map = ഇത്+v2.varIdxMap;
    സ്ഥിരം auto &var_store = ഇത്+v2.varStore;
    auto *var_store_cache = var_store.create_cache ();

    hb_vector_t<പൂർണ്ണസംഖ്യ> coords_2_14;
    coords_2_14.resize (coords_length);
    വേണ്ടി (unsigned i = 0; i < coords_length; i++)
      coords_2_14[i] = roundf (coords[i] / 4.f); // 16.16 -> 2.14

    hb_vector_t<പൂർണ്ണസംഖ്യ> out;
    out.alloc (coords_length);
    വേണ്ടി (unsigned i = 0; i < coords_length; i++)
    {
      പൂർണ്ണസംഖ്യ v = coords[i];
      uint32_t varidx = varidx_map.map (i);
      ദശാംശം delta = var_store.get_delta (varidx, coords_2_14.arrayZ, coords_2_14.length, var_store_cache);
      /* Apply the delta unclamped and clamp only the result to [-1, +1],
       * matching fontTools. Since inputs and results are in [-1, +1],
       * deltas beyond ±2 are equivalent to ±2; clamp to that range only
       * to keep the float->int conversion safe. */
      ദശാംശം d = hb_clamp (delta * 4, -(ദശാംശം) (1<<17), +(ദശാംശം) (1<<17)); // 2.14 -> 16.16
      v += (പൂർണ്ണസംഖ്യ) roundf (d);
      v = hb_clamp (v, -(1<<16), +(1<<16));
      out.push (v);
    }
    വേണ്ടി (unsigned i = 0; i < coords_length; i++)
      coords[i] = out[i];

    OT::ItemVariationStore::destroy_cache (var_store_cache);
#endif
  }

  /* An avar version 2 font is never downgraded to v1: even with a NULL
   * varStore offset (legal; maps like plain v1) the avar2 subsetting path
   * applies, with "no variation" semantics for rows that don't resolve —
   * offset compensation then creates delta rows on demand. */
  തർക്കം has_v2_data () സ്ഥിരം { തിരികെ version.major > 1; }

  /* Resolve the avar2 VarStore and VarIdxMap, for the subset planner.
   * Either pointer may be to the Null object (nullable offsets). */
  തർക്കം get_v2_store_and_map (സ്ഥിരം ItemVariationStore **store,
                             സ്ഥിരം DeltaSetIndexMap **varidx_map) സ്ഥിരം
  {
#ifndef HB_NO_AVAR2
    എങ്കിൽ (version.major < 2) തിരികെ അസത്യം;
    സ്ഥിരം SegmentMaps *map = &firstAxisSegmentMaps;
    വേണ്ടി (unsigned i = 0; i < axis_എണ്ണം_2; i++)
      map = &StructAfter<SegmentMaps> (*map);
    സ്ഥിരം auto &v2 = * (സ്ഥിരം avarV2Tail *) map;
    *store = &(ഇത്+v2.varStore);
    *varidx_map = &(ഇത്+v2.varIdxMap);
    തിരികെ സത്യം;
#അല്ലെങ്കിൽ
    തിരികെ അസത്യം;
#endif
  }

  // axis normalization is done in 2.14 here
  // TODO: deprecate this API once fonttools is updated to use 16.16 normalization
  തർക്കം map_coords_2_14 (ദശാംശം *coords, unsigned പൂർണ്ണസംഖ്യ coords_length,
			തർക്കം v1_only = അസത്യം) സ്ഥിരം
  {
    hb_vector_t<പൂർണ്ണസംഖ്യ> coords_2_14;
    എങ്കിൽ (!v1_only && !coords_2_14.resize (coords_length)) തിരികെ അസത്യം;
    unsigned പൂർണ്ണസംഖ്യ എണ്ണം = hb_min (coords_length, axis_എണ്ണം_2);

    സ്ഥിരം SegmentMaps *map = &firstAxisSegmentMaps;
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < എണ്ണം; i++)
    {
      പൂർണ്ണസംഖ്യ v = roundf (map->map_float (coords[i]) * 16384.f);
      എങ്കിൽ (!v1_only)
	coords_2_14[i] = v;
      coords[i] = v / 16384.f;
      map = &StructAfter<SegmentMaps> (*map);
    }

    എങ്കിൽ (v1_only)
      തിരികെ സത്യം;

#ifndef HB_NO_AVAR2
    എങ്കിൽ (version.major < 2)
      തിരികെ സത്യം;
    hb_barrier ();

    വേണ്ടി (; എണ്ണം < axis_എണ്ണം_2; എണ്ണം++)
      map = &StructAfter<SegmentMaps> (*map);

    സ്ഥിരം auto &v2 = * (സ്ഥിരം avarV2Tail *) map;

    സ്ഥിരം auto &varidx_map = ഇത്+v2.varIdxMap;
    സ്ഥിരം auto &var_store = ഇത്+v2.varStore;
    auto *var_store_cache = var_store.create_cache ();

    വേണ്ടി (unsigned i = 0; i < coords_length; i++)
    {
      പൂർണ്ണസംഖ്യ v = coords_2_14[i];
      uint32_t varidx = varidx_map.map (i);
      ദശാംശം delta = var_store.get_delta (varidx, coords_2_14.arrayZ, coords_2_14.length, var_store_cache);
      /* As above: apply the delta unclamped (±2 covers every useful case)
       * and clamp the result to [-1, +1], matching fontTools. */
      v += (പൂർണ്ണസംഖ്യ) hb_clamp (roundf (delta), -(ദശാംശം) (1<<15), +(ദശാംശം) (1<<15));
      v = hb_clamp (v, -(1<<14), +(1<<14));
      coords[i] = v / 16384.f;
    }

    OT::ItemVariationStore::destroy_cache (var_store_cache);
    തിരികെ സത്യം;
#അല്ലെങ്കിൽ
    തിരികെ version.major < 2;
#endif
  }

  തർക്കം subset (hb_subset_context_t *c) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);
    unsigned retained_axis_എണ്ണം = c->plan->axes_index_map.get_population ();
    എങ്കിൽ (!retained_axis_എണ്ണം) //all axes are pinned/dropped
      return_trace (അസത്യം);

    avar *out = c->serializer->allocate_min<avar> ();
    എങ്കിൽ (unlikely (!out)) return_trace (അസത്യം);

    out->version.major = c->plan->has_avar2 ? 2 : 1;
    out->version.minor = 0;
    എങ്കിൽ (!c->serializer->check_assign (out->axis_എണ്ണം_2, retained_axis_എണ്ണം, HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (അസത്യം);

    /* For avar2, keep the instantiated v1 mappings around; offset
     * compensation needs them to locate the new mappings' kinks. */
    hb_vector_t<hb_vector_t<Axis_മൂല്യം_Map>> new_mappings;
    എങ്കിൽ (c->plan->has_avar2 && !new_mappings.resize (axis_എണ്ണം_2))
      return_trace (അസത്യം);

    സ്ഥിരം hb_map_t& axes_index_map = c->plan->axes_index_map;
    സ്ഥിരം SegmentMaps *map = &firstAxisSegmentMaps;
    unsigned എണ്ണം = axis_എണ്ണം_2;
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < എണ്ണം; i++)
    {
      എങ്കിൽ (axes_index_map.has (i))
      {
        hb_tag_t *axis_tag;
        എങ്കിൽ (!c->plan->axes_old_index_tag_map.has (i, &axis_tag))
          return_trace (അസത്യം);

        Triple *axis_location;
        എങ്കിൽ (c->plan->has_avar2 &&
            c->plan->user_axes_location.has (*axis_tag, &axis_location) &&
            axis_location->is_point ())
        {
          /* Pinned axis in avar2 mode: serialize identity segment map
           * {-1->-1, 0->0, 1->1}. The axis is kept in fvar as hidden,
           * so avar needs a segment map entry for it. */
          auto *identity_map = c->serializer->start_embed<SegmentMaps> ();
          എങ്കിൽ (unlikely (!c->serializer->extend_min (identity_map)))
            return_trace (അസത്യം);
          Axis_മൂല്യം_Map m;
          m.set_mapping (-1.f, -1.f);
          എങ്കിൽ (!m.serialize (c->serializer)) return_trace (അസത്യം);
          m.set_mapping (0.f, 0.f);
          എങ്കിൽ (!m.serialize (c->serializer)) return_trace (അസത്യം);
          m.set_mapping (1.f, 1.f);
          എങ്കിൽ (!m.serialize (c->serializer)) return_trace (അസത്യം);
          എങ്കിൽ (!c->serializer->check_assign (identity_map->len, 3u,
                                            HB_SERIALIZE_ERROR_INT_OVERFLOW))
            return_trace (അസത്യം);
        }
        അല്ലെങ്കിൽ
        {
          /* Restricted or free axis: use standard SegmentMaps::subset() */
          എങ്കിൽ (!map->subset (c, *axis_tag,
                            c->plan->has_avar2 ? &new_mappings[i] : nullptr))
            return_trace (അസത്യം);
        }
      }
      map = &StructAfter<SegmentMaps> (*map);
    }

    എങ്കിൽ (c->plan->has_avar2)
      return_trace (_subset_avar2 (c, new_mappings));

    return_trace (സത്യം);
  }

  സ്വകാര്യ:
  ഘടന avar2_index_map_plan_t
  {
    തർക്കം init (സ്ഥിരം hb_vector_t<uint32_t> &varidx_mapping,
	       സ്ഥിരം hb_map_t &axes_index_map,
	       unsigned axis_എണ്ണം)
    {
      എങ്കിൽ (!output_map.alloc (axes_index_map.get_population ()))
	തിരികെ അസത്യം;

      തർക്കം has_no_variation = അസത്യം;
      unsigned max_outer = 0, max_inner = 0;
      വേണ്ടി (unsigned i = 0; i < axis_എണ്ണം; i++)
      {
	എങ്കിൽ (!axes_index_map.has (i)) തുടരുക;
	uint32_t varidx = varidx_mapping[i];
	output_map.push (varidx);
	എങ്കിൽ (varidx == HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
	{
	  has_no_variation = സത്യം;
	  തുടരുക;
	}
	max_outer = hb_max (max_outer, varidx >> 16);
	max_inner = hb_max (max_inner, varidx & 0xFFFF);
      }
      എങ്കിൽ (output_map.in_error () ||
	  output_map.length != axes_index_map.get_population ())
	തിരികെ അസത്യം;

      എങ്കിൽ (has_no_variation)
      {
	width = 4;
	inner_bit_എണ്ണം = 16;
      }
      അല്ലെങ്കിൽ
      {
	inner_bit_എണ്ണം = hb_max (1u, hb_bit_storage (max_inner));
	unsigned outer_bit_എണ്ണം = hb_max (1u, hb_bit_storage (max_outer));
	width = hb_clamp ((inner_bit_എണ്ണം + outer_bit_എണ്ണം + 7) / 8, 1u, 4u);
	എങ്കിൽ (inner_bit_എണ്ണം + outer_bit_എണ്ണം > width * 8)
	  inner_bit_എണ്ണം = width * 8 - outer_bit_എണ്ണം;
      }
      തിരികെ സത്യം;
    }

    unsigned get_inner_bit_എണ്ണം () സ്ഥിരം { തിരികെ inner_bit_എണ്ണം; }
    unsigned get_width () സ്ഥിരം { തിരികെ width; }
    hb_array_t<സ്ഥിരം uint32_t> get_output_map () സ്ഥിരം { തിരികെ output_map.as_array (); }

    unsigned inner_bit_എണ്ണം = 1;
    unsigned width = 1;
    hb_vector_t<uint32_t> output_map;
  };

  തർക്കം _subset_avar2 (hb_subset_context_t *c,
                      സ്ഥിരം hb_vector_t<hb_vector_t<Axis_മൂല്യം_Map>> &new_mappings) സ്ഥിരം
  {
#എങ്കിൽ defined (HB_NO_VAR) || defined (HB_NO_AVAR2)
    /* Not reachable: the plan never sets has_avar2 in these configurations. */
    തിരികെ അസത്യം;
#അല്ലെങ്കിൽ

    /* 1. Locate original avar2 data, keeping per-axis old segment maps */
    hb_vector_t<സ്ഥിരം SegmentMaps *> old_seg_maps;
    എങ്കിൽ (!old_seg_maps.alloc (axis_എണ്ണം_2)) തിരികെ അസത്യം;
    സ്ഥിരം SegmentMaps *map = &firstAxisSegmentMaps;
    വേണ്ടി (unsigned i = 0; i < axis_എണ്ണം_2; i++)
    {
      old_seg_maps.push (map);
      map = &StructAfter<SegmentMaps> (*map);
    }

    സ്ഥിരം auto &v2 = * (സ്ഥിരം avarV2Tail *) map;
    സ്ഥിരം auto &varidx_map = ഇത്+v2.varIdxMap;
    സ്ഥിരം auto &var_store = ഇത്+v2.varStore;

    auto fvar_axes = c->plan->source->table.fvar->get_axes ();

    /* 2. Compute default deltas by evaluating VarStore at old defaults */
    hb_vector_t<പൂർണ്ണസംഖ്യ> default_coords;
    എങ്കിൽ (!default_coords.resize (axis_എണ്ണം_2)) തിരികെ അസത്യം;
    വേണ്ടി (unsigned i = 0; i < axis_എണ്ണം_2; i++)
    {
      hb_tag_t *axis_tag;
      എങ്കിൽ (c->plan->axes_old_index_tag_map.has (i, &axis_tag) &&
          c->plan->old_intermediates.has (*axis_tag))
      {
        ദശാംശം d_i = (ദശാംശം) c->plan->old_intermediates.get (*axis_tag).നടുവ്;
        default_coords[i] = roundf (d_i * 16384.f);
      }
      അല്ലെങ്കിൽ
        default_coords[i] = 0;
    }

    auto *store_cache = var_store.create_cache ();
    hb_vector_t<ദശാംശം> default_deltas;
    എങ്കിൽ (!default_deltas.resize (axis_എണ്ണം_2)) {
      ItemVariationStore::destroy_cache (store_cache);
      തിരികെ അസത്യം;
    }
    വേണ്ടി (unsigned i = 0; i < axis_എണ്ണം_2; i++)
    {
      uint32_t varidx = varidx_map.map (i);
      എങ്കിൽ (varidx == HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
        default_deltas[i] = 0.f;
      അല്ലെങ്കിൽ
        default_deltas[i] = var_store.get_delta (varidx, default_coords.arrayZ,
                                                  default_coords.length, store_cache);
    }
    ItemVariationStore::destroy_cache (store_cache);

    /* 3. Rebase IVS regions */
    item_variations_t item_vars;
    എങ്കിൽ (!item_vars.create_from_item_varstore (var_store, c->plan->axes_old_index_tag_map))
      തിരികെ അസത്യം;
    എങ്കിൽ (!item_vars.instantiate_tuple_vars (c->plan->old_intermediates,
                                           c->plan->axes_triple_distances,
                                           അസത്യം))
      തിരികെ അസത്യം;

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
    എങ്കിൽ (!new_varidx_mapping.resize (axis_എണ്ണം_2)) തിരികെ അസത്യം;
    വേണ്ടി (unsigned i = 0; i < axis_എണ്ണം_2; i++)
    {
      uint32_t varidx = varidx_map.map (i);
      എങ്കിൽ (varidx != HB_OT_LAYOUT_NO_VARIATIONS_INDEX &&
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
    hb_hashmap_t<uint32_t, unsigned> varidx_ref_എണ്ണം;
    വേണ്ടി (unsigned i = 0; i < axis_എണ്ണം_2; i++)
    {
      എങ്കിൽ (!c->plan->axes_index_map.has (i)) തുടരുക; /* self-contained: dropped */
      uint32_t varidx = new_varidx_mapping[i];
      എങ്കിൽ (varidx == HB_OT_LAYOUT_NO_VARIATIONS_INDEX) തുടരുക;
      unsigned *എണ്ണം;
      എങ്കിൽ (varidx_ref_എണ്ണം.has (varidx, &എണ്ണം))
        (*എണ്ണം)++;
      അല്ലെങ്കിൽ എങ്കിൽ (!varidx_ref_എണ്ണം.set (varidx, 1))
        തിരികെ അസത്യം;
    }
    വേണ്ടി (unsigned i = 0; i < axis_എണ്ണം_2; i++)
    {
      hb_tag_t *axis_tag_ptr;
      എങ്കിൽ (!c->plan->axes_old_index_tag_map.has (i, &axis_tag_ptr))
        തിരികെ അസത്യം;
      /* Only axes that will receive offset compensation (restricted or
       * pinned) can contaminate a shared row. */
      എങ്കിൽ (!c->plan->axes_index_map.has (i) ||
          !c->plan->user_axes_location.has (*axis_tag_ptr))
        തുടരുക;
      uint32_t varidx = new_varidx_mapping[i];
      എങ്കിൽ (varidx == HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
        തുടരുക; /* gets a fresh, private VarData in the offset loop below */
      unsigned *എണ്ണം;
      എങ്കിൽ (!varidx_ref_എണ്ണം.has (varidx, &എണ്ണം) || *എണ്ണം <= 1)
        തുടരുക; /* sole owner: safe to write offsets in place */
      unsigned outer = varidx >> 16;
      unsigned new_inner = item_vars.duplicate_row (outer, varidx & 0xFFFF);
      എങ്കിൽ (unlikely (new_inner == (unsigned) -1)) തിരികെ അസത്യം;
      new_varidx_mapping[i] = (outer << 16) | new_inner;
      (*എണ്ണം)--;
    }

    /* 6. Add offset compensation tuples.
     * Track processed (outer,inner) pairs to avoid adding duplicate biases
     * when multiple axes share the same varIdx. */
    hb_set_t processed_varidxes;
    വേണ്ടി (unsigned i = 0; i < axis_എണ്ണം_2; i++)
    {
      hb_tag_t *axis_tag_ptr;
      എങ്കിൽ (!c->plan->axes_old_index_tag_map.has (i, &axis_tag_ptr))
        തിരികെ അസത്യം;
      hb_tag_t axis_tag = *axis_tag_ptr;

      /* Self-contained pinned axes are removed from fvar/avar; their
       * contribution is baked into the variation tables instead. */
      എങ്കിൽ (!c->plan->axes_index_map.has (i))
        തുടരുക;

      Triple *new_user;
      എങ്കിൽ (c->plan->user_axes_location.has (axis_tag, &new_user))
      {
        /* This axis is being restricted or pinned */
        Triple *old_int;
        എങ്കിൽ (!c->plan->old_intermediates.has (axis_tag, &old_int))
          തിരികെ അസത്യം;

        ദശാംശം a_i = (ദശാംശം) old_int->minimum;
        ദശാംശം d_i = (ദശാംശം) old_int->നടുവ്;
        ദശാംശം b_i = (ദശാംശം) old_int->maximum;

        പൂർണ്ണസംഖ്യ d_int = roundf (d_i * 16384.f);

        തർക്കം is_pinned = new_user->is_point ();

        uint32_t varidx = new_varidx_mapping[i];
        unsigned outer, inner, item_എണ്ണം_2;

        എങ്കിൽ (varidx == HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
        {
          /* No existing avar2 mapping. Create new VarData. */
          outer = item_vars.add_vardata (1);
          inner = 0;
          item_എണ്ണം_2 = 1;
          new_varidx_mapping[i] = (outer << 16) | inner;
          default_deltas[i] = 0.f; /* no prior default delta */
        }
        അല്ലെങ്കിൽ
        {
          outer = varidx >> 16;
          inner = varidx & 0xFFFF;
          item_എണ്ണം_2 = item_vars.get_item_എണ്ണം (outer);
        }

        /* Empty-region bias: d_int + round(defaultDelta) */
        പൂർണ്ണസംഖ്യ bias = d_int + (പൂർണ്ണസംഖ്യ) roundf (default_deltas[i]);
        എങ്കിൽ (bias != 0)
        {
          hb_hashmap_t<hb_tag_t, Triple> empty_region;
          item_vars.add_tuple (outer, std::move (empty_region),
                               inner, bias, item_എണ്ണം_2);
        }

        എങ്കിൽ (!is_pinned)
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
          _avar2_add_knot (knots, -1.0, (ഇരട്ട) a_i + 1.0);
          _avar2_add_knot (knots, 0.0, (ഇരട്ട) d_i);
          _avar2_add_knot (knots, 1.0, (ഇരട്ട) b_i - 1.0);

          സ്ഥിരം hb_vector_t<Axis_മൂല്യം_Map> &new_mapping = new_mappings[i];

          ദശാംശം min_f = 0.f, def_f = 0.f, max_f = 0.f;
          എങ്കിൽ (likely (i < fvar_axes.length))
            fvar_axes[i].get_coordinates (min_f, def_f, max_f);
          ഇരട്ട old_min = (ഇരട്ട) min_f;
          ഇരട്ട old_def = (ഇരട്ട) def_f;
          ഇരട്ട old_max = (ഇരട്ട) max_f;

          /* If the axis default MOVED, inv_renorm also kinks where the OLD
           * default lands in the new space (the old intermediate coordinate
           * crosses 0 there), at
           *   z = z_old  ->  -z_old
           * Omitting that knot (as a plain two-tent encoding would) makes
           * interior coordinates wrong. */
          ഇരട്ട z_old = avar2_normalize_മൂല്യം ((ഇരട്ട) old_def,
                                                 (ഇരട്ട) new_user->minimum,
                                                 (ഇരട്ട) new_user->നടുവ്,
                                                 (ഇരട്ട) new_user->maximum);
          z_old = _avar2_map_new_mapping (new_mapping, z_old);
          z_old = (ഇരട്ട) roundf ((ദശാംശം) (z_old * 16384.0)) / 16384.0;
          എങ്കിൽ (-1.0 < z_old && z_old < 1.0)
            _avar2_add_knot (knots, z_old, -z_old);

          /* Interior avar v1 breakpoints inside the retained range each put
           * a kink in offset(z). Sampling only {-1, 0, +1, z_old} would
           * linearly interpolate across those kinks. The instantiated
           * mapping keeps exactly the in-range old breakpoints, and the new
           * mapping kinks at each one's output coordinate; add that z with
           * its old intermediate value so offset(z) is reproduced at every
           * kink. */
          hb_vector_t<avar2_offset_knot_t> with_breakpoints (knots);
          വേണ്ടി (സ്ഥിരം auto &m : new_mapping)
          {
            ഇരട്ട from = (ഇരട്ട) m.coords[0].to_float ();
            എങ്കിൽ (from == -1.0 || from == 0.0 || from == 1.0)
              തുടരുക; /* anchors already seeded */
            ഇരട്ട z = (ഇരട്ട) m.coords[1].to_float ();
            എങ്കിൽ (!(-1.0 < z && z < 1.0))
              തുടരുക;
            ഇരട്ട user = avar2_denormalize_മൂല്യം (from,
                                                    (ഇരട്ട) new_user->minimum,
                                                    (ഇരട്ട) new_user->നടുവ്,
                                                    (ഇരട്ട) new_user->maximum);
            ഇരട്ട n_old = avar2_normalize_മൂല്യം (user, old_min, old_def, old_max);
            ഇരട്ട x_old = (ഇരട്ട) old_seg_maps[i]->map_float ((ദശാംശം) n_old);
            x_old = (ഇരട്ട) roundf ((ദശാംശം) (x_old * 16384.0)) / 16384.0;
            _avar2_add_knot (with_breakpoints, z, x_old - z);
          }

          എങ്കിൽ (unlikely (knots.in_error () || with_breakpoints.in_error ()))
            തിരികെ അസത്യം;

          /* Extra tents cost F2Dot14 rounding, so for a steep segment they
           * can add more quantization noise than the structural error they
           * remove. Keep the interior breakpoints only when they do not
           * increase the estimated residual; this makes the collection a
           * strict (never-worse) improvement over the {-1, 0, +1, z_old}
           * anchors. Warn when even the better choice is not bit-exact (a
           * steep retained segment that cannot be reproduced in F2Dot14). */
          unsigned err = _avar2_estimate_offset_error (*old_seg_maps[i], new_mapping,
                                                       old_min, old_def, old_max,
                                                       (ഇരട്ട) new_user->minimum,
                                                       (ഇരട്ട) new_user->നടുവ്,
                                                       (ഇരട്ട) new_user->maximum,
                                                       knots);
          എങ്കിൽ (with_breakpoints.length != knots.length)
          {
            unsigned err_with = _avar2_estimate_offset_error (*old_seg_maps[i], new_mapping,
                                                              old_min, old_def, old_max,
                                                              (ഇരട്ട) new_user->minimum,
                                                              (ഇരട്ട) new_user->നടുവ്,
                                                              (ഇരട്ട) new_user->maximum,
                                                              with_breakpoints);
            എങ്കിൽ (err_with <= err)
            {
              knots = std::move (with_breakpoints);
              err = err_with;
            }
          }
          എങ്കിൽ (err > 8)
            DEBUG_സന്ദേശം (SUBSET, nullptr,
                       "avar2 offset compensation is approximate for axis %c%c%c%c: "
                       "max residual %u F2Dot14 units",
                       HB_UNTAG (axis_tag), err);

          /* Synthesize tents. Adjacent tents evaluate to zero at each
           * other's peaks, so each knot's delta is offset(z) - offset(0);
           * the base value offset(0) = d_i is carried by the empty-region
           * bias above. This reduces to the classic pair of tents
           * (-1,-1,0) / (0,+1,+1) when the default is unchanged and there
           * are no interior knots. */
          വേണ്ടി (unsigned k = 0; k < knots.length; k++)
          {
            ഇരട്ട z = knots.arrayZ[k].z;
            എങ്കിൽ (z == 0.0) തുടരുക;
            പൂർണ്ണസംഖ്യ delta = (പൂർണ്ണസംഖ്യ) roundf ((ദശാംശം) ((knots.arrayZ[k].offset - (ഇരട്ട) d_i) * 16384.0));
            എങ്കിൽ (!delta) തുടരുക;
            ഇരട്ട lower, upper;
            എങ്കിൽ (z > 0.0)
            {
              lower = knots.arrayZ[k - 1].z;
              upper = k + 1 < knots.length ? knots.arrayZ[k + 1].z : z;
            }
            അല്ലെങ്കിൽ
            {
              upper = knots.arrayZ[k + 1].z;
              lower = k > 0 ? knots.arrayZ[k - 1].z : z;
            }
            hb_hashmap_t<hb_tag_t, Triple> region;
            എങ്കിൽ (unlikely (!region.set (axis_tag, Triple (lower, z, upper))))
              തിരികെ അസത്യം;
            item_vars.add_tuple (outer, std::move (region),
                                 inner, delta, item_എണ്ണം_2);
          }
        }
      }
      അല്ലെങ്കിൽ
      {
        /* Free or private axis — not being restricted.
         * If it has a non-zero default delta, add it back as a bias.
         * Skip if this (outer,inner) was already processed (shared varIdx). */
        uint32_t varidx = new_varidx_mapping[i];
        എങ്കിൽ (varidx == HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
          തുടരുക;

        എങ്കിൽ (processed_varidxes.has (varidx))
          തുടരുക;
        processed_varidxes.add (varidx);

        unsigned outer = varidx >> 16;
        unsigned inner = varidx & 0xFFFF;
        പൂർണ്ണസംഖ്യ dd = (പൂർണ്ണസംഖ്യ) roundf (default_deltas[i]);
        എങ്കിൽ (dd != 0)
        {
          unsigned item_എണ്ണം_2 = item_vars.get_item_എണ്ണം (outer);
          hb_hashmap_t<hb_tag_t, Triple> empty_region;
          item_vars.add_tuple (outer, std::move (empty_region),
                               inner, dd, item_എണ്ണം_2);
        }
      }
    }

    /* 7. Finalize: build region list + convert to varstore */
    എങ്കിൽ (!item_vars.build_region_list ()) തിരികെ അസത്യം;
    എങ്കിൽ (!item_vars.as_item_varstore (സത്യം /* optimize */,
                                     അസത്യം /* use_no_variation_idx */))
      തിരികെ അസത്യം;

    /* 8. Apply varidx_map optimization remapping */
    സ്ഥിരം auto &opt_varidx_map = item_vars.get_varidx_map ();
    വേണ്ടി (unsigned i = 0; i < axis_എണ്ണം_2; i++)
    {
      uint32_t varidx = new_varidx_mapping[i];
      എങ്കിൽ (varidx == HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
        തുടരുക;
      uint32_t *new_idx;
      എങ്കിൽ (opt_varidx_map.has (varidx, &new_idx))
        new_varidx_mapping[i] = *new_idx;
    }

    /* 9. Serialize avarV2Tail. Entries cover the retained axes only;
     * self-contained pinned axes are removed from fvar. */
    avar2_index_map_plan_t index_map_plan;
    എങ്കിൽ (!index_map_plan.init (new_varidx_mapping,
			      c->plan->axes_index_map,
			      axis_എണ്ണം_2))
      തിരികെ അസത്യം;

    auto *tail = c->serializer->allocate_size<avarV2Tail> (avarV2Tail::static_size);
    എങ്കിൽ (unlikely (!tail)) തിരികെ അസത്യം;
    എങ്കിൽ (!tail->varIdxMap.serialize_serialize (c->serializer, index_map_plan))
      തിരികെ അസത്യം;
    എങ്കിൽ (!tail->varStore.serialize_serialize (c->serializer,
					     item_vars.has_long_word (),
					     c->plan->axis_tags,
					     item_vars.get_region_list (),
					     item_vars.get_vardata_encodings ()))
      തിരികെ അസത്യം;

    തിരികെ സത്യം;
#endif
  }

  പൊതു:

  സംരക്ഷിത:
  FixedVersion<>version;	/* Version of the avar table
				 * initially set to 0x00010000u */
  HBUINT16	reserved;	/* This field is permanently reserved. Set to 0. */
  HBUINT16	axis_എണ്ണം_2;	/* The number of variation axes in the font. This
				 * must be the same number as axisCount in the
				 * 'fvar' table. */
  SegmentMaps	firstAxisSegmentMaps;

  പൊതു:
  DEFINE_SIZE_MIN (8);
};

} /* namespace OT */


#endif /* HB_OT_VAR_AVAR_TABLE_HH */
