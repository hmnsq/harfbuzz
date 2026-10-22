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

#ಅನ್ತರ್ಭಾವ "hb-open-type.hh"
#ಅನ್ತರ್ಭಾವ "hb-ot-var-common.hh"
#ಅನ್ತರ್ಭಾವ "hb-ot-var-fvar-table.hh"


/*
 * avar -- Axis Variations
 * https://docs.microsoft.com/en-us/typography/opentype/spec/avar
 */

#define HB_OT_TAG_avar HB_TAG('a','v','a','r')


ನಾಮಾಕಾಶ OT {


/* "Spec": https://github.com/be-fonts/boring-expansion-spec/issues/14 */
ಸಂರಚನಾ avarV2Tail
{
  friend ಸಂರಚನಾ avar;

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c,
		 ಧ್ರುವ ಶೂನ್ಯ *base) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (varIdxMap.sanitize (c, base) &&
		  varStore.sanitize (c, base));
  }

  ರಕ್ಷಿತ:
  Offset32To<DeltaSetIndexMap>	varIdxMap;	/* Offset from the beginning of 'avar' table. */
  Offset32To<ItemVariationStore>	varStore;	/* Offset from the beginning of 'avar' table. */

  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (8);
};


ಸಂರಚನಾ Axis_ಮೌಲ್ಯ_Map
{
  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್));
  }

  ಶೂನ್ಯ set_mapping (ಭಿನ್ನ from_coord, ಭಿನ್ನ to_coord)
  {
    coords[0].set_float (from_coord);
    coords[1].set_float (to_coord);
  }

  ತಾರ್ಕಿಕ is_outside_axis_range (ಧ್ರುವ Triple& axis_range) ಧ್ರುವ
  {
    ದ್ವಿಗುಣ from_coord = (ದ್ವಿಗುಣ) coords[0].to_float ();
    ಪ್ರತ್ಯಾಗಮ !axis_range.contains (from_coord);
  }

  ತಾರ್ಕಿಕ must_include () ಧ್ರುವ
  {
    ಭಿನ್ನ from_coord = coords[0].to_float ();
    ಭಿನ್ನ to_coord = coords[1].to_float ();
    ಪ್ರತ್ಯಾಗಮ (from_coord == -1.f && to_coord == -1.f) ||
           (from_coord == 0.f && to_coord == 0.f) ||
           (from_coord == 1.f && to_coord == 1.f);
  }

  ಶೂನ್ಯ instantiate (ಧ್ರುವ Triple& axis_range,
                    ಧ್ರುವ Triple& unmapped_range,
                    ಧ್ರುವ TripleDistances& triple_distances)
  {
    ಭಿನ್ನ from_coord = coords[0].to_float ();
    ಭಿನ್ನ to_coord = coords[1].to_float ();

    from_coord = renormalize_ಮೌಲ್ಯ ((ದ್ವಿಗುಣ) from_coord, unmapped_range, triple_distances);
    to_coord = renormalize_ಮೌಲ್ಯ ((ದ್ವಿಗುಣ) to_coord, axis_range, triple_distances);

    /* Malformed input can renormalize outside [-1,1], which would
     * overflow F2DOT14. */
    from_coord = hb_clamp (from_coord, -1.f, 1.f);
    to_coord = hb_clamp (to_coord, -1.f, 1.f);

    coords[0].set_float (from_coord);
    coords[1].set_float (to_coord);
  }

  HB_INTERNAL ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ cmp (ಧ್ರುವ ಶೂನ್ಯ *pa, ಧ್ರುವ ಶೂನ್ಯ *pb)
  {
    ಧ್ರುವ Axis_ಮೌಲ್ಯ_Map *a = (ಧ್ರುವ Axis_ಮೌಲ್ಯ_Map *) pa;
    ಧ್ರುವ Axis_ಮೌಲ್ಯ_Map *b = (ಧ್ರುವ Axis_ಮೌಲ್ಯ_Map *) pb;

    ಪೂರ್ಣಾಙ್ಕ a_from = a->coords[0].to_int ();
    ಪೂರ್ಣಾಙ್ಕ b_from = b->coords[0].to_int ();
    ಯದಿ (a_from != b_from)
      ಪ್ರತ್ಯಾಗಮ a_from - b_from;

    /* this should never be reached. according to the spec, all of the axis
     * value map records for a given axis must have different fromCoord values
     * */
    ಪೂರ್ಣಾಙ್ಕ a_to = a->coords[1].to_int ();
    ಪೂರ್ಣಾಙ್ಕ b_to = b->coords[1].to_int ();
    ಪ್ರತ್ಯಾಗಮ a_to - b_to;
  }

  ತಾರ್ಕಿಕ serialize (hb_serialize_context_t *c) ಧ್ರುವ
  {
    TRACE_SERIALIZE (ಏತತ್);
    return_trace (c->embed (ಏತತ್));
  }

  ಸಾರ್ವಜನಿಕ:
  F2DOT14	coords[2];
//   F2DOT14	fromCoord;	/* A normalized coordinate value obtained using
//				 * default normalization. */
//   F2DOT14	toCoord;	/* The modified, normalized coordinate value. */

  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (4);
};

ಸಂರಚನಾ SegmentMaps : Array16Of<Axis_ಮೌಲ್ಯ_Map>
{
  ಭಿನ್ನ map_float (ಭಿನ್ನ value, unsigned ಪೂರ್ಣಾಙ್ಕ from_offset = 0, unsigned ಪೂರ್ಣಾಙ್ಕ to_offset = 1) ಧ್ರುವ
  {
#define fromCoord coords[from_offset].to_float ()
#define toCoord coords[to_offset].to_float ()

    ಧ್ರುವ auto *map = arrayZ;

    /* The following special-cases are not part of OpenType, which requires
     * that at least -1, 0, and +1 must be mapped. But we include these as
     * part of a better error recovery scheme. */
    ಯದಿ (len < 2)
    {
      ಯದಿ (!len)
	ಪ್ರತ್ಯಾಗಮ value;
      ಅನ್ಯಥಾ /* len == 1*/
	ಪ್ರತ್ಯಾಗಮ value - map[0].fromCoord + map[0].toCoord;
    }

    // At least two mappings now.

    /* CoreText is wild...
     * PingFangUI avar needs all this special-casing...
     * So we implement an extended version of the spec here,
     * which is more robust and more likely to be compatible with
     * the wild. */

    unsigned start = 0;
    unsigned end = len;
    ಯದಿ (map[start].fromCoord == -1 && map[start].toCoord == -1 && map[start+1].fromCoord == -1)
      start++;
    ಯದಿ (map[end-1].fromCoord == +1 && map[end-1].toCoord == +1 && map[end-2].fromCoord == +1)
      end--;

    /* Look for exact match first, and do lots of special-casing. */
    unsigned i;
    ಪ್ರತಿ (i = start; i < end; i++)
      ಯದಿ (value == map[i].fromCoord)
	ಭಙ್ಗ;
    ಯದಿ (i < end)
    {
      // There's at least one exact match. See if there are more.
      unsigned j = i;
      ಪ್ರತಿ (; j + 1 < end; j++)
	ಯದಿ (value != map[j + 1].fromCoord)
	  ಭಙ್ಗ;

      // [i,j] inclusive are all exact matches:

      // If there's only one, return it. This is the only spec-compliant case.
      ಯದಿ (i == j)
	ಪ್ರತ್ಯಾಗಮ map[i].toCoord;
      // If there's exactly three, return the middle one.
      ಯದಿ (i + 2 == j)
	ಪ್ರತ್ಯಾಗಮ map[i + 1].toCoord;

      // Ignore the middle ones. Return the one mapping closer to 0.
      ಯದಿ (value < 0) ಪ್ರತ್ಯಾಗಮ map[j].toCoord;
      ಯದಿ (value > 0) ಪ್ರತ್ಯಾಗಮ map[i].toCoord;

      // Mapping 0? CoreText seems confused. It seems to prefer 0 here...
      // So we'll just return the smallest one. lol
      ಪ್ರತ್ಯಾಗಮ fabsf (map[i].toCoord) < fabsf (map[j].toCoord) ? map[i].toCoord : map[j].toCoord;

      // Mapping 0? Return one not mapping to 0.
      ಯದಿ (map[i].toCoord == 0)
	ಪ್ರತ್ಯಾಗಮ map[j].toCoord;
      ಅನ್ಯಥಾ
	ಪ್ರತ್ಯಾಗಮ map[i].toCoord;
    }

    /* There's at least two and we're not an exact match. Prepare to lerp. */

    // Find the segment we're in.
    ಪ್ರತಿ (i = start; i < end; i++)
      ಯದಿ (value < map[i].fromCoord)
	ಭಙ್ಗ;

    ಯದಿ (i == start)
    {
      // Value before all segments; Shift.
      ಪ್ರತ್ಯಾಗಮ value - map[start].fromCoord + map[start].toCoord;
    }
    ಯದಿ (i == end)
    {
      // Value after all segments; Shift.
      ಪ್ರತ್ಯಾಗಮ value - map[end - 1].fromCoord + map[end - 1].toCoord;
    }

    // Actually interpolate.
    auto &before = map[i-1];
    auto &after = map[i];
    ಭಿನ್ನ denom = after.fromCoord - before.fromCoord; // Can't be zero by now.
    ಪ್ರತ್ಯಾಗಮ before.toCoord + ((after.toCoord - before.toCoord) * (value - before.fromCoord)) / denom;

#undef toCoord
#undef fromCoord
  }

  ಭಿನ್ನ unmap_float (ಭಿನ್ನ value) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ map_float (value, 1, 0); }


  // TODO Kill this.
  Triple unmap_axis_range (ಧ್ರುವ Triple& axis_range) ಧ್ರುವ
  {
    ಭಿನ್ನ unmapped_min = unmap_float (axis_range.minimum);
    ಭಿನ್ನ unmapped_ಮಧ್ಯ = unmap_float (axis_range.ಮಧ್ಯ);
    ಭಿನ್ನ unmapped_max = unmap_float (axis_range.maximum);

    ಪ್ರತ್ಯಾಗಮ Triple{unmapped_min, unmapped_ಮಧ್ಯ, unmapped_max};
  }

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c, hb_tag_t axis_tag,
               hb_vector_t<Axis_ಮೌಲ್ಯ_Map> *out_mappings = nullptr) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);

    /* This function cannot work on avar2 table (and currently doesn't).
     * We should instead keep the design coords in the shape plan and use
     * those. unmap_axis_range needs to be killed. */

    /* avar mapped normalized axis range. Under avar2, axes_location holds
     * only the self-contained pins for the other tables; avar itself uses
     * the intermediate-space ranges of all restricted axes. */
    ಧ್ರುವ auto &axes_location = c->plan->has_avar2
				? c->plan->old_intermediates
				: c->plan->axes_location;
    Triple *axis_range;
    ಯದಿ (!axes_location.has (axis_tag, &axis_range))
      ಪ್ರತ್ಯಾಗಮ c->serializer->embed (*ಏತತ್);

    TripleDistances *axis_triple_distances;
    ಯದಿ (!c->plan->axes_triple_distances.has (axis_tag, &axis_triple_distances))
      return_trace (ಅಸತ್ಯ);

    auto *out = c->serializer->start_embed (ಏತತ್);
    ಯದಿ (unlikely (!c->serializer->extend_min (out))) return_trace (ಅಸತ್ಯ);

    Triple unmapped_range = unmap_axis_range (*axis_range);

    /* create a vector of retained mappings and sort */
    hb_vector_t<Axis_ಮೌಲ್ಯ_Map> ಮೌಲ್ಯ_mappings;
    ಪ್ರತಿ (ಧ್ರುವ auto& _ : as_array ())
    {
      ಯದಿ (_.is_outside_axis_range (unmapped_range))
        ಅನುವರ್ತ;
      Axis_ಮೌಲ್ಯ_Map mapping;
      mapping = _;
      mapping.instantiate (*axis_range, unmapped_range, *axis_triple_distances);
      /* (-1, -1), (0, 0), (1, 1) mappings will be added later, so avoid
       * duplicates here */
      ಯದಿ (mapping.must_include ())
        ಅನುವರ್ತ;
      ಮೌಲ್ಯ_mappings.push (mapping);
    }

    Axis_ಮೌಲ್ಯ_Map m;
    m.set_mapping (-1.f, -1.f);
    ಮೌಲ್ಯ_mappings.push (m);

    m.set_mapping (0.f, 0.f);
    ಮೌಲ್ಯ_mappings.push (m);

    m.set_mapping (1.f, 1.f);
    ಮೌಲ್ಯ_mappings.push (m);

    ಮೌಲ್ಯ_mappings.qsort ();

    ಯದಿ (unlikely (ಮೌಲ್ಯ_mappings.in_error ()))
      return_trace (ಅಸತ್ಯ);

    ಪ್ರತಿ (ಧ್ರುವ auto& _ : ಮೌಲ್ಯ_mappings)
    {
      ಯದಿ (!_.serialize (c->serializer))
        return_trace (ಅಸತ್ಯ);
    }
    ಯದಿ (!c->serializer->check_assign (out->len, ಮೌಲ್ಯ_mappings.length, HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (ಅಸತ್ಯ);
    /* Hand the instantiated mappings to the caller; avar2 offset
     * compensation needs them to locate the new mapping's kinks. */
    ಯದಿ (out_mappings)
      *out_mappings = std::move (ಮೌಲ್ಯ_mappings);
    return_trace (ಸತ್ಯ);
  }

  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_ARRAY (2, *ಏತತ್);
};

/* One knot of the avar2 offset-compensation function
 * offset(z) = inv_renorm(z) - z, in new intermediate space. */
ಸಂರಚನಾ avar2_offset_knot_t
{
  ದ್ವಿಗುಣ z;
  ದ್ವಿಗುಣ offset;
};

/* Insert a knot keeping the vector sorted by z; first insertion wins on
 * duplicate z. */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ
_avar2_add_knot (hb_vector_t<avar2_offset_knot_t> &knots, ದ್ವಿಗುಣ z, ದ್ವಿಗುಣ offset)
{
  unsigned i = 0;
  ಯಾವತ್ (i < knots.length && knots.arrayZ[i].z < z) i++;
  ಯದಿ (i < knots.length && knots.arrayZ[i].z == z) ಪ್ರತ್ಯಾಗಮ;
  knots.push (avar2_offset_knot_t {0.0, 0.0});
  ಯದಿ (unlikely (knots.in_error ())) ಪ್ರತ್ಯಾಗಮ;
  ಪ್ರತಿ (unsigned j = knots.length - 1; j > i; j--)
    knots.arrayZ[j] = knots.arrayZ[j - 1];
  knots.arrayZ[i] = avar2_offset_knot_t {z, offset};
}

/* Piecewise-linear evaluation over the sorted knots. Inputs are always
 * within [-1, +1] and anchor knots at -1/0/+1 always exist. */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ದ್ವಿಗುಣ
_avar2_eval_offset (ಧ್ರುವ hb_vector_t<avar2_offset_knot_t> &knots, ದ್ವಿಗುಣ z)
{
  unsigned len = knots.length;
  ಪ್ರತಿ (unsigned i = 0; i < len; i++)
  {
    ಯದಿ (z == knots.arrayZ[i].z) ಪ್ರತ್ಯಾಗಮ knots.arrayZ[i].offset;
    ಯದಿ (z < knots.arrayZ[i].z)
    {
      ಯದಿ (!i) ಪ್ರತ್ಯಾಗಮ knots.arrayZ[0].offset;
      ಧ್ರುವ auto &before = knots.arrayZ[i - 1];
      ಧ್ರುವ auto &after = knots.arrayZ[i];
      ದ್ವಿಗುಣ denom = after.z - before.z;
      ಪ್ರತ್ಯಾಗಮ before.offset + (after.offset - before.offset) * (z - before.z) / denom;
    }
  }
  ಪ್ರತ್ಯಾಗಮ len ? knots.arrayZ[len - 1].offset : 0.0;
}

/* Piecewise-linear evaluation over instantiated avar v1 mappings, as
 * produced by SegmentMaps::subset (sorted, with -1/0/+1 anchors).
 * Matches SegmentMaps::map_float for such well-formed mappings. */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ದ್ವಿಗುಣ
_avar2_map_new_mapping (ಧ್ರುವ hb_vector_t<Axis_ಮೌಲ್ಯ_Map> &mappings, ದ್ವಿಗುಣ v,
			unsigned from_offset = 0, unsigned to_offset = 1)
{
  unsigned len = mappings.length;
  ಯದಿ (!len) ಪ್ರತ್ಯಾಗಮ v;

  ಪ್ರತಿ (unsigned i = 0; i < len; i++)
  {
    ದ್ವಿಗುಣ from = (ದ್ವಿಗುಣ) mappings.arrayZ[i].coords[from_offset].to_float ();
    ಯದಿ (v == from)
      ಪ್ರತ್ಯಾಗಮ (ದ್ವಿಗುಣ) mappings.arrayZ[i].coords[to_offset].to_float ();
    ಯದಿ (v < from)
    {
      ದ್ವಿಗುಣ to = (ದ್ವಿಗುಣ) mappings.arrayZ[i].coords[to_offset].to_float ();
      ಯದಿ (!i) ಪ್ರತ್ಯಾಗಮ v - from + to;
      ದ್ವಿಗುಣ prev_from = (ದ್ವಿಗುಣ) mappings.arrayZ[i - 1].coords[from_offset].to_float ();
      ದ್ವಿಗುಣ prev_to = (ದ್ವಿಗುಣ) mappings.arrayZ[i - 1].coords[to_offset].to_float ();
      ದ್ವಿಗುಣ denom = from - prev_from;
      ಯದಿ (denom == 0.0) ಪ್ರತ್ಯಾಗಮ prev_to;
      ಪ್ರತ್ಯಾಗಮ prev_to + (to - prev_to) * (v - prev_from) / denom;
    }
  }
  ಪ್ರತ್ಯಾಗಮ v - (ದ್ವಿಗುಣ) mappings.arrayZ[len - 1].coords[from_offset].to_float ()
           + (ದ್ವಿಗುಣ) mappings.arrayZ[len - 1].coords[to_offset].to_float ();
}

/* Inverse of _avar2_map_new_mapping: pull an output coordinate back to a
 * preimage input coordinate. For a non-strictly-monotone mapping this picks
 * one preimage, which is fine for its only use (augmenting the error
 * estimator's sample set). */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ದ್ವಿಗುಣ
_avar2_unmap_new_mapping (ಧ್ರುವ hb_vector_t<Axis_ಮೌಲ್ಯ_Map> &mappings, ದ್ವಿಗುಣ v)
{
  ಪ್ರತ್ಯಾಗಮ _avar2_map_new_mapping (mappings, v, 1, 0);
}

/* Plain (non-avar) fvar-style normalization of a user value against a
 * user-space (min, default, max) triple. */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ದ್ವಿಗುಣ
avar2_normalize_ಮೌಲ್ಯ (ದ್ವಿಗುಣ v, ದ್ವಿಗುಣ min, ದ್ವಿಗುಣ def, ದ್ವಿಗುಣ max)
{
  v = hb_clamp (v, min, max);
  ಯದಿ (v == def) ಪ್ರತ್ಯಾಗಮ 0.0;
  ಯದಿ (v < def) ಪ್ರತ್ಯಾಗಮ def == min ? 0.0 : (v - def) / (def - min);
  ಪ್ರತ್ಯಾಗಮ def == max ? 0.0 : (v - def) / (max - def);
}

/* Inverse of the above: map a normalized value back to user space. */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ದ್ವಿಗುಣ
avar2_denormalize_ಮೌಲ್ಯ (ದ್ವಿಗುಣ v, ದ್ವಿಗುಣ min, ದ್ವಿಗುಣ def, ದ್ವಿಗುಣ max)
{
  ಯದಿ (v == 0.0) ಪ್ರತ್ಯಾಗಮ def;
  ಪ್ರತ್ಯಾಗಮ v < 0.0 ? def + v * (def - min) : def + v * (max - def);
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
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ unsigned
_avar2_estimate_offset_error (ಧ್ರುವ SegmentMaps &old_seg,
                              ಧ್ರುವ hb_vector_t<Axis_ಮೌಲ್ಯ_Map> &new_mapping,
                              ದ್ವಿಗುಣ old_min, ದ್ವಿಗುಣ old_def, ದ್ವಿಗುಣ old_max,
                              ದ್ವಿಗುಣ new_min, ದ್ವಿಗುಣ new_def, ದ್ವಿಗುಣ new_max,
                              ಧ್ರುವ hb_vector_t<avar2_offset_knot_t> &knots)
{
  constexpr unsigned samples = 257;
  hb_vector_t<ದ್ವಿಗುಣ> us;
  ಯದಿ (unlikely (!us.alloc (samples + old_seg.as_array ().length +
                           new_mapping.length + knots.length)))
    ಪ್ರತ್ಯಾಗಮ UINT_MAX; /* estimator only; fail towards "worse" */
  ಪ್ರತಿ (unsigned i = 0; i < samples; i++)
    us.push (new_min + (new_max - new_min) * i / (samples - 1));
  ಪ್ರತಿ (ಧ್ರುವ auto &_ : old_seg.as_array ())
    us.push (avar2_denormalize_ಮೌಲ್ಯ ((ದ್ವಿಗುಣ) _.coords[0].to_float (),
                                       old_min, old_def, old_max));
  ಪ್ರತಿ (ಧ್ರುವ auto &_ : new_mapping)
    us.push (avar2_denormalize_ಮೌಲ್ಯ ((ದ್ವಿಗುಣ) _.coords[0].to_float (),
                                       new_min, new_def, new_max));
  ಪ್ರತಿ (ಧ್ರುವ auto &knot : knots)
    us.push (avar2_denormalize_ಮೌಲ್ಯ (
               _avar2_unmap_new_mapping (new_mapping, knot.z),
               new_min, new_def, new_max));
  ಯದಿ (unlikely (us.in_error ())) ಪ್ರತ್ಯಾಗಮ UINT_MAX;

  unsigned max_err = 0;
  ಪ್ರತಿ (ದ್ವಿಗುಣ u : us)
  {
    ಯದಿ (u < new_min || u > new_max) ಅನುವರ್ತ;
    ದ್ವಿಗುಣ n_old = avar2_normalize_ಮೌಲ್ಯ (u, old_min, old_def, old_max);
    ದ್ವಿಗುಣ old_final = (ದ್ವಿಗುಣ) old_seg.map_float ((ಭಿನ್ನ) n_old);
    ದ್ವಿಗುಣ n_new = avar2_normalize_ಮೌಲ್ಯ (u, new_min, new_def, new_max);
    ದ್ವಿಗುಣ z = _avar2_map_new_mapping (new_mapping, n_new);
    ದ್ವಿಗುಣ new_final = z + _avar2_eval_offset (knots, z);
    ಪೂರ್ಣಾಙ್ಕ err = abs ((ಪೂರ್ಣಾಙ್ಕ) roundf ((ಭಿನ್ನ) (old_final * 16384.0)) -
                   (ಪೂರ್ಣಾಙ್ಕ) roundf ((ಭಿನ್ನ) (new_final * 16384.0)));
    ಯದಿ ((unsigned) err > max_err) max_err = err;
  }
  ಪ್ರತ್ಯಾಗಮ max_err;
}

ಸಂರಚನಾ avar
{
  ಸ್ಥಿರ constexpr hb_tag_t tableTag = HB_OT_TAG_avar;

  ತಾರ್ಕಿಕ has_data () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ version.to_int (); }

  ಧ್ರುವ SegmentMaps* get_segment_maps () ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ &firstAxisSegmentMaps; }

  unsigned get_axis_ಎಣಿಕೆ () ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ axis_ಎಣಿಕೆ_2; }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    ಯದಿ (!(version.sanitize (c) &&
	  hb_barrier () &&
	  (version.major == 1
#ifndef HB_NO_AVAR2
	   || version.major == 2
#endif
	   ) &&
	  c->check_struct (ಏತತ್)))
      return_trace (ಅಸತ್ಯ);

    ಧ್ರುವ SegmentMaps *map = &firstAxisSegmentMaps;
    unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = axis_ಎಣಿಕೆ_2;
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಎಣಿಕೆ; i++)
    {
      ಯದಿ (unlikely (!map->sanitize (c)))
	return_trace (ಅಸತ್ಯ);
      map = &StructAfter<SegmentMaps> (*map);
    }

#ifndef HB_NO_AVAR2
    ಯದಿ (version.major < 2)
      return_trace (ಸತ್ಯ);
    hb_barrier ();

    ಧ್ರುವ auto &v2 = * (ಧ್ರುವ avarV2Tail *) map;
    ಯದಿ (unlikely (!v2.sanitize (c, ಏತತ್)))
      return_trace (ಅಸತ್ಯ);
#endif

    return_trace (ಸತ್ಯ);
  }

  ಶೂನ್ಯ map_coords_16_16 (ಪೂರ್ಣಾಙ್ಕ *coords, unsigned ಪೂರ್ಣಾಙ್ಕ coords_length) ಧ್ರುವ
  {
    unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = hb_min (coords_length, axis_ಎಣಿಕೆ_2);

    ಧ್ರುವ SegmentMaps *map = &firstAxisSegmentMaps;
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಎಣಿಕೆ; i++)
    {
      coords[i] = roundf (map->map_float (coords[i] / 65536.f) * 65536.f);
      map = &StructAfter<SegmentMaps> (*map);
    }

#ifndef HB_NO_AVAR2
    ಯದಿ (version.major < 2)
      ಪ್ರತ್ಯಾಗಮ;
    hb_barrier ();

    ಪ್ರತಿ (; ಎಣಿಕೆ < axis_ಎಣಿಕೆ_2; ಎಣಿಕೆ++)
      map = &StructAfter<SegmentMaps> (*map);

    ಧ್ರುವ auto &v2 = * (ಧ್ರುವ avarV2Tail *) map;

    ಧ್ರುವ auto &varidx_map = ಏತತ್+v2.varIdxMap;
    ಧ್ರುವ auto &var_store = ಏತತ್+v2.varStore;
    auto *var_store_cache = var_store.create_cache ();

    hb_vector_t<ಪೂರ್ಣಾಙ್ಕ> coords_2_14;
    coords_2_14.resize (coords_length);
    ಪ್ರತಿ (unsigned i = 0; i < coords_length; i++)
      coords_2_14[i] = roundf (coords[i] / 4.f); // 16.16 -> 2.14

    hb_vector_t<ಪೂರ್ಣಾಙ್ಕ> out;
    out.alloc (coords_length);
    ಪ್ರತಿ (unsigned i = 0; i < coords_length; i++)
    {
      ಪೂರ್ಣಾಙ್ಕ v = coords[i];
      uint32_t varidx = varidx_map.map (i);
      ಭಿನ್ನ delta = var_store.get_delta (varidx, coords_2_14.arrayZ, coords_2_14.length, var_store_cache);
      /* Apply the delta unclamped and clamp only the result to [-1, +1],
       * matching fontTools. Since inputs and results are in [-1, +1],
       * deltas beyond ±2 are equivalent to ±2; clamp to that range only
       * to keep the float->int conversion safe. */
      ಭಿನ್ನ d = hb_clamp (delta * 4, -(ಭಿನ್ನ) (1<<17), +(ಭಿನ್ನ) (1<<17)); // 2.14 -> 16.16
      v += (ಪೂರ್ಣಾಙ್ಕ) roundf (d);
      v = hb_clamp (v, -(1<<16), +(1<<16));
      out.push (v);
    }
    ಪ್ರತಿ (unsigned i = 0; i < coords_length; i++)
      coords[i] = out[i];

    OT::ItemVariationStore::destroy_cache (var_store_cache);
#endif
  }

  /* An avar version 2 font is never downgraded to v1: even with a NULL
   * varStore offset (legal; maps like plain v1) the avar2 subsetting path
   * applies, with "no variation" semantics for rows that don't resolve —
   * offset compensation then creates delta rows on demand. */
  ತಾರ್ಕಿಕ has_v2_data () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ version.major > 1; }

  /* Resolve the avar2 VarStore and VarIdxMap, for the subset planner.
   * Either pointer may be to the Null object (nullable offsets). */
  ತಾರ್ಕಿಕ get_v2_store_and_map (ಧ್ರುವ ItemVariationStore **store,
                             ಧ್ರುವ DeltaSetIndexMap **varidx_map) ಧ್ರುವ
  {
#ifndef HB_NO_AVAR2
    ಯದಿ (version.major < 2) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    ಧ್ರುವ SegmentMaps *map = &firstAxisSegmentMaps;
    ಪ್ರತಿ (unsigned i = 0; i < axis_ಎಣಿಕೆ_2; i++)
      map = &StructAfter<SegmentMaps> (*map);
    ಧ್ರುವ auto &v2 = * (ಧ್ರುವ avarV2Tail *) map;
    *store = &(ಏತತ್+v2.varStore);
    *varidx_map = &(ಏತತ್+v2.varIdxMap);
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
#ಅನ್ಯಥಾ
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
#endif
  }

  // axis normalization is done in 2.14 here
  // TODO: deprecate this API once fonttools is updated to use 16.16 normalization
  ತಾರ್ಕಿಕ map_coords_2_14 (ಭಿನ್ನ *coords, unsigned ಪೂರ್ಣಾಙ್ಕ coords_length,
			ತಾರ್ಕಿಕ v1_only = ಅಸತ್ಯ) ಧ್ರುವ
  {
    hb_vector_t<ಪೂರ್ಣಾಙ್ಕ> coords_2_14;
    ಯದಿ (!v1_only && !coords_2_14.resize (coords_length)) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = hb_min (coords_length, axis_ಎಣಿಕೆ_2);

    ಧ್ರುವ SegmentMaps *map = &firstAxisSegmentMaps;
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಎಣಿಕೆ; i++)
    {
      ಪೂರ್ಣಾಙ್ಕ v = roundf (map->map_float (coords[i]) * 16384.f);
      ಯದಿ (!v1_only)
	coords_2_14[i] = v;
      coords[i] = v / 16384.f;
      map = &StructAfter<SegmentMaps> (*map);
    }

    ಯದಿ (v1_only)
      ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;

#ifndef HB_NO_AVAR2
    ಯದಿ (version.major < 2)
      ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    hb_barrier ();

    ಪ್ರತಿ (; ಎಣಿಕೆ < axis_ಎಣಿಕೆ_2; ಎಣಿಕೆ++)
      map = &StructAfter<SegmentMaps> (*map);

    ಧ್ರುವ auto &v2 = * (ಧ್ರುವ avarV2Tail *) map;

    ಧ್ರುವ auto &varidx_map = ಏತತ್+v2.varIdxMap;
    ಧ್ರುವ auto &var_store = ಏತತ್+v2.varStore;
    auto *var_store_cache = var_store.create_cache ();

    ಪ್ರತಿ (unsigned i = 0; i < coords_length; i++)
    {
      ಪೂರ್ಣಾಙ್ಕ v = coords_2_14[i];
      uint32_t varidx = varidx_map.map (i);
      ಭಿನ್ನ delta = var_store.get_delta (varidx, coords_2_14.arrayZ, coords_2_14.length, var_store_cache);
      /* As above: apply the delta unclamped (±2 covers every useful case)
       * and clamp the result to [-1, +1], matching fontTools. */
      v += (ಪೂರ್ಣಾಙ್ಕ) hb_clamp (roundf (delta), -(ಭಿನ್ನ) (1<<15), +(ಭಿನ್ನ) (1<<15));
      v = hb_clamp (v, -(1<<14), +(1<<14));
      coords[i] = v / 16384.f;
    }

    OT::ItemVariationStore::destroy_cache (var_store_cache);
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
#ಅನ್ಯಥಾ
    ಪ್ರತ್ಯಾಗಮ version.major < 2;
#endif
  }

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    unsigned retained_axis_ಎಣಿಕೆ = c->plan->axes_index_map.get_population ();
    ಯದಿ (!retained_axis_ಎಣಿಕೆ) //all axes are pinned/dropped
      return_trace (ಅಸತ್ಯ);

    avar *out = c->serializer->allocate_min<avar> ();
    ಯದಿ (unlikely (!out)) return_trace (ಅಸತ್ಯ);

    out->version.major = c->plan->has_avar2 ? 2 : 1;
    out->version.minor = 0;
    ಯದಿ (!c->serializer->check_assign (out->axis_ಎಣಿಕೆ_2, retained_axis_ಎಣಿಕೆ, HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (ಅಸತ್ಯ);

    /* For avar2, keep the instantiated v1 mappings around; offset
     * compensation needs them to locate the new mappings' kinks. */
    hb_vector_t<hb_vector_t<Axis_ಮೌಲ್ಯ_Map>> new_mappings;
    ಯದಿ (c->plan->has_avar2 && !new_mappings.resize (axis_ಎಣಿಕೆ_2))
      return_trace (ಅಸತ್ಯ);

    ಧ್ರುವ hb_map_t& axes_index_map = c->plan->axes_index_map;
    ಧ್ರುವ SegmentMaps *map = &firstAxisSegmentMaps;
    unsigned ಎಣಿಕೆ = axis_ಎಣಿಕೆ_2;
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಎಣಿಕೆ; i++)
    {
      ಯದಿ (axes_index_map.has (i))
      {
        hb_tag_t *axis_tag;
        ಯದಿ (!c->plan->axes_old_index_tag_map.has (i, &axis_tag))
          return_trace (ಅಸತ್ಯ);

        Triple *axis_location;
        ಯದಿ (c->plan->has_avar2 &&
            c->plan->user_axes_location.has (*axis_tag, &axis_location) &&
            axis_location->is_point ())
        {
          /* Pinned axis in avar2 mode: serialize identity segment map
           * {-1->-1, 0->0, 1->1}. The axis is kept in fvar as hidden,
           * so avar needs a segment map entry for it. */
          auto *identity_map = c->serializer->start_embed<SegmentMaps> ();
          ಯದಿ (unlikely (!c->serializer->extend_min (identity_map)))
            return_trace (ಅಸತ್ಯ);
          Axis_ಮೌಲ್ಯ_Map m;
          m.set_mapping (-1.f, -1.f);
          ಯದಿ (!m.serialize (c->serializer)) return_trace (ಅಸತ್ಯ);
          m.set_mapping (0.f, 0.f);
          ಯದಿ (!m.serialize (c->serializer)) return_trace (ಅಸತ್ಯ);
          m.set_mapping (1.f, 1.f);
          ಯದಿ (!m.serialize (c->serializer)) return_trace (ಅಸತ್ಯ);
          ಯದಿ (!c->serializer->check_assign (identity_map->len, 3u,
                                            HB_SERIALIZE_ERROR_INT_OVERFLOW))
            return_trace (ಅಸತ್ಯ);
        }
        ಅನ್ಯಥಾ
        {
          /* Restricted or free axis: use standard SegmentMaps::subset() */
          ಯದಿ (!map->subset (c, *axis_tag,
                            c->plan->has_avar2 ? &new_mappings[i] : nullptr))
            return_trace (ಅಸತ್ಯ);
        }
      }
      map = &StructAfter<SegmentMaps> (*map);
    }

    ಯದಿ (c->plan->has_avar2)
      return_trace (_subset_avar2 (c, new_mappings));

    return_trace (ಸತ್ಯ);
  }

  ಗುಪ್ತ:
  ಸಂರಚನಾ avar2_index_map_plan_t
  {
    ತಾರ್ಕಿಕ init (ಧ್ರುವ hb_vector_t<uint32_t> &varidx_mapping,
	       ಧ್ರುವ hb_map_t &axes_index_map,
	       unsigned axis_ಎಣಿಕೆ)
    {
      ಯದಿ (!output_map.alloc (axes_index_map.get_population ()))
	ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

      ತಾರ್ಕಿಕ has_no_variation = ಅಸತ್ಯ;
      unsigned max_outer = 0, max_inner = 0;
      ಪ್ರತಿ (unsigned i = 0; i < axis_ಎಣಿಕೆ; i++)
      {
	ಯದಿ (!axes_index_map.has (i)) ಅನುವರ್ತ;
	uint32_t varidx = varidx_mapping[i];
	output_map.push (varidx);
	ಯದಿ (varidx == HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
	{
	  has_no_variation = ಸತ್ಯ;
	  ಅನುವರ್ತ;
	}
	max_outer = hb_max (max_outer, varidx >> 16);
	max_inner = hb_max (max_inner, varidx & 0xFFFF);
      }
      ಯದಿ (output_map.in_error () ||
	  output_map.length != axes_index_map.get_population ())
	ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

      ಯದಿ (has_no_variation)
      {
	width = 4;
	inner_bit_ಎಣಿಕೆ = 16;
      }
      ಅನ್ಯಥಾ
      {
	inner_bit_ಎಣಿಕೆ = hb_max (1u, hb_bit_storage (max_inner));
	unsigned outer_bit_ಎಣಿಕೆ = hb_max (1u, hb_bit_storage (max_outer));
	width = hb_clamp ((inner_bit_ಎಣಿಕೆ + outer_bit_ಎಣಿಕೆ + 7) / 8, 1u, 4u);
	ಯದಿ (inner_bit_ಎಣಿಕೆ + outer_bit_ಎಣಿಕೆ > width * 8)
	  inner_bit_ಎಣಿಕೆ = width * 8 - outer_bit_ಎಣಿಕೆ;
      }
      ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    }

    unsigned get_inner_bit_ಎಣಿಕೆ () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ inner_bit_ಎಣಿಕೆ; }
    unsigned get_width () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ width; }
    hb_array_t<ಧ್ರುವ uint32_t> get_output_map () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ output_map.as_array (); }

    unsigned inner_bit_ಎಣಿಕೆ = 1;
    unsigned width = 1;
    hb_vector_t<uint32_t> output_map;
  };

  ತಾರ್ಕಿಕ _subset_avar2 (hb_subset_context_t *c,
                      ಧ್ರುವ hb_vector_t<hb_vector_t<Axis_ಮೌಲ್ಯ_Map>> &new_mappings) ಧ್ರುವ
  {
#ಯದಿ defined (HB_NO_VAR) || defined (HB_NO_AVAR2)
    /* Not reachable: the plan never sets has_avar2 in these configurations. */
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
#ಅನ್ಯಥಾ

    /* 1. Locate original avar2 data, keeping per-axis old segment maps */
    hb_vector_t<ಧ್ರುವ SegmentMaps *> old_seg_maps;
    ಯದಿ (!old_seg_maps.alloc (axis_ಎಣಿಕೆ_2)) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    ಧ್ರುವ SegmentMaps *map = &firstAxisSegmentMaps;
    ಪ್ರತಿ (unsigned i = 0; i < axis_ಎಣಿಕೆ_2; i++)
    {
      old_seg_maps.push (map);
      map = &StructAfter<SegmentMaps> (*map);
    }

    ಧ್ರುವ auto &v2 = * (ಧ್ರುವ avarV2Tail *) map;
    ಧ್ರುವ auto &varidx_map = ಏತತ್+v2.varIdxMap;
    ಧ್ರುವ auto &var_store = ಏತತ್+v2.varStore;

    auto fvar_axes = c->plan->source->table.fvar->get_axes ();

    /* 2. Compute default deltas by evaluating VarStore at old defaults */
    hb_vector_t<ಪೂರ್ಣಾಙ್ಕ> default_coords;
    ಯದಿ (!default_coords.resize (axis_ಎಣಿಕೆ_2)) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    ಪ್ರತಿ (unsigned i = 0; i < axis_ಎಣಿಕೆ_2; i++)
    {
      hb_tag_t *axis_tag;
      ಯದಿ (c->plan->axes_old_index_tag_map.has (i, &axis_tag) &&
          c->plan->old_intermediates.has (*axis_tag))
      {
        ಭಿನ್ನ d_i = (ಭಿನ್ನ) c->plan->old_intermediates.get (*axis_tag).ಮಧ್ಯ;
        default_coords[i] = roundf (d_i * 16384.f);
      }
      ಅನ್ಯಥಾ
        default_coords[i] = 0;
    }

    auto *store_cache = var_store.create_cache ();
    hb_vector_t<ಭಿನ್ನ> default_deltas;
    ಯದಿ (!default_deltas.resize (axis_ಎಣಿಕೆ_2)) {
      ItemVariationStore::destroy_cache (store_cache);
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }
    ಪ್ರತಿ (unsigned i = 0; i < axis_ಎಣಿಕೆ_2; i++)
    {
      uint32_t varidx = varidx_map.map (i);
      ಯದಿ (varidx == HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
        default_deltas[i] = 0.f;
      ಅನ್ಯಥಾ
        default_deltas[i] = var_store.get_delta (varidx, default_coords.arrayZ,
                                                  default_coords.length, store_cache);
    }
    ItemVariationStore::destroy_cache (store_cache);

    /* 3. Rebase IVS regions */
    item_variations_t item_vars;
    ಯದಿ (!item_vars.create_from_item_varstore (var_store, c->plan->axes_old_index_tag_map))
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    ಯದಿ (!item_vars.instantiate_tuple_vars (c->plan->old_intermediates,
                                           c->plan->axes_triple_distances,
                                           ಅಸತ್ಯ))
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

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
    ಯದಿ (!new_varidx_mapping.resize (axis_ಎಣಿಕೆ_2)) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    ಪ್ರತಿ (unsigned i = 0; i < axis_ಎಣಿಕೆ_2; i++)
    {
      uint32_t varidx = varidx_map.map (i);
      ಯದಿ (varidx != HB_OT_LAYOUT_NO_VARIATIONS_INDEX &&
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
    hb_hashmap_t<uint32_t, unsigned> varidx_ref_ಎಣಿಕೆ;
    ಪ್ರತಿ (unsigned i = 0; i < axis_ಎಣಿಕೆ_2; i++)
    {
      ಯದಿ (!c->plan->axes_index_map.has (i)) ಅನುವರ್ತ; /* self-contained: dropped */
      uint32_t varidx = new_varidx_mapping[i];
      ಯದಿ (varidx == HB_OT_LAYOUT_NO_VARIATIONS_INDEX) ಅನುವರ್ತ;
      unsigned *ಎಣಿಕೆ;
      ಯದಿ (varidx_ref_ಎಣಿಕೆ.has (varidx, &ಎಣಿಕೆ))
        (*ಎಣಿಕೆ)++;
      ಅನ್ಯಥಾ ಯದಿ (!varidx_ref_ಎಣಿಕೆ.set (varidx, 1))
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }
    ಪ್ರತಿ (unsigned i = 0; i < axis_ಎಣಿಕೆ_2; i++)
    {
      hb_tag_t *axis_tag_ptr;
      ಯದಿ (!c->plan->axes_old_index_tag_map.has (i, &axis_tag_ptr))
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
      /* Only axes that will receive offset compensation (restricted or
       * pinned) can contaminate a shared row. */
      ಯದಿ (!c->plan->axes_index_map.has (i) ||
          !c->plan->user_axes_location.has (*axis_tag_ptr))
        ಅನುವರ್ತ;
      uint32_t varidx = new_varidx_mapping[i];
      ಯದಿ (varidx == HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
        ಅನುವರ್ತ; /* gets a fresh, private VarData in the offset loop below */
      unsigned *ಎಣಿಕೆ;
      ಯದಿ (!varidx_ref_ಎಣಿಕೆ.has (varidx, &ಎಣಿಕೆ) || *ಎಣಿಕೆ <= 1)
        ಅನುವರ್ತ; /* sole owner: safe to write offsets in place */
      unsigned outer = varidx >> 16;
      unsigned new_inner = item_vars.duplicate_row (outer, varidx & 0xFFFF);
      ಯದಿ (unlikely (new_inner == (unsigned) -1)) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
      new_varidx_mapping[i] = (outer << 16) | new_inner;
      (*ಎಣಿಕೆ)--;
    }

    /* 6. Add offset compensation tuples.
     * Track processed (outer,inner) pairs to avoid adding duplicate biases
     * when multiple axes share the same varIdx. */
    hb_set_t processed_varidxes;
    ಪ್ರತಿ (unsigned i = 0; i < axis_ಎಣಿಕೆ_2; i++)
    {
      hb_tag_t *axis_tag_ptr;
      ಯದಿ (!c->plan->axes_old_index_tag_map.has (i, &axis_tag_ptr))
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
      hb_tag_t axis_tag = *axis_tag_ptr;

      /* Self-contained pinned axes are removed from fvar/avar; their
       * contribution is baked into the variation tables instead. */
      ಯದಿ (!c->plan->axes_index_map.has (i))
        ಅನುವರ್ತ;

      Triple *new_user;
      ಯದಿ (c->plan->user_axes_location.has (axis_tag, &new_user))
      {
        /* This axis is being restricted or pinned */
        Triple *old_int;
        ಯದಿ (!c->plan->old_intermediates.has (axis_tag, &old_int))
          ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

        ಭಿನ್ನ a_i = (ಭಿನ್ನ) old_int->minimum;
        ಭಿನ್ನ d_i = (ಭಿನ್ನ) old_int->ಮಧ್ಯ;
        ಭಿನ್ನ b_i = (ಭಿನ್ನ) old_int->maximum;

        ಪೂರ್ಣಾಙ್ಕ d_int = roundf (d_i * 16384.f);

        ತಾರ್ಕಿಕ is_pinned = new_user->is_point ();

        uint32_t varidx = new_varidx_mapping[i];
        unsigned outer, inner, item_ಎಣಿಕೆ_2;

        ಯದಿ (varidx == HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
        {
          /* No existing avar2 mapping. Create new VarData. */
          outer = item_vars.add_vardata (1);
          inner = 0;
          item_ಎಣಿಕೆ_2 = 1;
          new_varidx_mapping[i] = (outer << 16) | inner;
          default_deltas[i] = 0.f; /* no prior default delta */
        }
        ಅನ್ಯಥಾ
        {
          outer = varidx >> 16;
          inner = varidx & 0xFFFF;
          item_ಎಣಿಕೆ_2 = item_vars.get_item_ಎಣಿಕೆ (outer);
        }

        /* Empty-region bias: d_int + round(defaultDelta) */
        ಪೂರ್ಣಾಙ್ಕ bias = d_int + (ಪೂರ್ಣಾಙ್ಕ) roundf (default_deltas[i]);
        ಯದಿ (bias != 0)
        {
          hb_hashmap_t<hb_tag_t, Triple> empty_region;
          item_vars.add_tuple (outer, std::move (empty_region),
                               inner, bias, item_ಎಣಿಕೆ_2);
        }

        ಯದಿ (!is_pinned)
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
          _avar2_add_knot (knots, -1.0, (ದ್ವಿಗುಣ) a_i + 1.0);
          _avar2_add_knot (knots, 0.0, (ದ್ವಿಗುಣ) d_i);
          _avar2_add_knot (knots, 1.0, (ದ್ವಿಗುಣ) b_i - 1.0);

          ಧ್ರುವ hb_vector_t<Axis_ಮೌಲ್ಯ_Map> &new_mapping = new_mappings[i];

          ಭಿನ್ನ min_f = 0.f, def_f = 0.f, max_f = 0.f;
          ಯದಿ (likely (i < fvar_axes.length))
            fvar_axes[i].get_coordinates (min_f, def_f, max_f);
          ದ್ವಿಗುಣ old_min = (ದ್ವಿಗುಣ) min_f;
          ದ್ವಿಗುಣ old_def = (ದ್ವಿಗುಣ) def_f;
          ದ್ವಿಗುಣ old_max = (ದ್ವಿಗುಣ) max_f;

          /* If the axis default MOVED, inv_renorm also kinks where the OLD
           * default lands in the new space (the old intermediate coordinate
           * crosses 0 there), at
           *   z = z_old  ->  -z_old
           * Omitting that knot (as a plain two-tent encoding would) makes
           * interior coordinates wrong. */
          ದ್ವಿಗುಣ z_old = avar2_normalize_ಮೌಲ್ಯ ((ದ್ವಿಗುಣ) old_def,
                                                 (ದ್ವಿಗುಣ) new_user->minimum,
                                                 (ದ್ವಿಗುಣ) new_user->ಮಧ್ಯ,
                                                 (ದ್ವಿಗುಣ) new_user->maximum);
          z_old = _avar2_map_new_mapping (new_mapping, z_old);
          z_old = (ದ್ವಿಗುಣ) roundf ((ಭಿನ್ನ) (z_old * 16384.0)) / 16384.0;
          ಯದಿ (-1.0 < z_old && z_old < 1.0)
            _avar2_add_knot (knots, z_old, -z_old);

          /* Interior avar v1 breakpoints inside the retained range each put
           * a kink in offset(z). Sampling only {-1, 0, +1, z_old} would
           * linearly interpolate across those kinks. The instantiated
           * mapping keeps exactly the in-range old breakpoints, and the new
           * mapping kinks at each one's output coordinate; add that z with
           * its old intermediate value so offset(z) is reproduced at every
           * kink. */
          hb_vector_t<avar2_offset_knot_t> with_breakpoints (knots);
          ಪ್ರತಿ (ಧ್ರುವ auto &m : new_mapping)
          {
            ದ್ವಿಗುಣ from = (ದ್ವಿಗುಣ) m.coords[0].to_float ();
            ಯದಿ (from == -1.0 || from == 0.0 || from == 1.0)
              ಅನುವರ್ತ; /* anchors already seeded */
            ದ್ವಿಗುಣ z = (ದ್ವಿಗುಣ) m.coords[1].to_float ();
            ಯದಿ (!(-1.0 < z && z < 1.0))
              ಅನುವರ್ತ;
            ದ್ವಿಗುಣ user = avar2_denormalize_ಮೌಲ್ಯ (from,
                                                    (ದ್ವಿಗುಣ) new_user->minimum,
                                                    (ದ್ವಿಗುಣ) new_user->ಮಧ್ಯ,
                                                    (ದ್ವಿಗುಣ) new_user->maximum);
            ದ್ವಿಗುಣ n_old = avar2_normalize_ಮೌಲ್ಯ (user, old_min, old_def, old_max);
            ದ್ವಿಗುಣ x_old = (ದ್ವಿಗುಣ) old_seg_maps[i]->map_float ((ಭಿನ್ನ) n_old);
            x_old = (ದ್ವಿಗುಣ) roundf ((ಭಿನ್ನ) (x_old * 16384.0)) / 16384.0;
            _avar2_add_knot (with_breakpoints, z, x_old - z);
          }

          ಯದಿ (unlikely (knots.in_error () || with_breakpoints.in_error ()))
            ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

          /* Extra tents cost F2Dot14 rounding, so for a steep segment they
           * can add more quantization noise than the structural error they
           * remove. Keep the interior breakpoints only when they do not
           * increase the estimated residual; this makes the collection a
           * strict (never-worse) improvement over the {-1, 0, +1, z_old}
           * anchors. Warn when even the better choice is not bit-exact (a
           * steep retained segment that cannot be reproduced in F2Dot14). */
          unsigned err = _avar2_estimate_offset_error (*old_seg_maps[i], new_mapping,
                                                       old_min, old_def, old_max,
                                                       (ದ್ವಿಗುಣ) new_user->minimum,
                                                       (ದ್ವಿಗುಣ) new_user->ಮಧ್ಯ,
                                                       (ದ್ವಿಗುಣ) new_user->maximum,
                                                       knots);
          ಯದಿ (with_breakpoints.length != knots.length)
          {
            unsigned err_with = _avar2_estimate_offset_error (*old_seg_maps[i], new_mapping,
                                                              old_min, old_def, old_max,
                                                              (ದ್ವಿಗುಣ) new_user->minimum,
                                                              (ದ್ವಿಗುಣ) new_user->ಮಧ್ಯ,
                                                              (ದ್ವಿಗುಣ) new_user->maximum,
                                                              with_breakpoints);
            ಯದಿ (err_with <= err)
            {
              knots = std::move (with_breakpoints);
              err = err_with;
            }
          }
          ಯದಿ (err > 8)
            DEBUG_ಸಂದೇಶ (SUBSET, nullptr,
                       "avar2 offset compensation is approximate for axis %c%c%c%c: "
                       "max residual %u F2Dot14 units",
                       HB_UNTAG (axis_tag), err);

          /* Synthesize tents. Adjacent tents evaluate to zero at each
           * other's peaks, so each knot's delta is offset(z) - offset(0);
           * the base value offset(0) = d_i is carried by the empty-region
           * bias above. This reduces to the classic pair of tents
           * (-1,-1,0) / (0,+1,+1) when the default is unchanged and there
           * are no interior knots. */
          ಪ್ರತಿ (unsigned k = 0; k < knots.length; k++)
          {
            ದ್ವಿಗುಣ z = knots.arrayZ[k].z;
            ಯದಿ (z == 0.0) ಅನುವರ್ತ;
            ಪೂರ್ಣಾಙ್ಕ delta = (ಪೂರ್ಣಾಙ್ಕ) roundf ((ಭಿನ್ನ) ((knots.arrayZ[k].offset - (ದ್ವಿಗುಣ) d_i) * 16384.0));
            ಯದಿ (!delta) ಅನುವರ್ತ;
            ದ್ವಿಗುಣ lower, upper;
            ಯದಿ (z > 0.0)
            {
              lower = knots.arrayZ[k - 1].z;
              upper = k + 1 < knots.length ? knots.arrayZ[k + 1].z : z;
            }
            ಅನ್ಯಥಾ
            {
              upper = knots.arrayZ[k + 1].z;
              lower = k > 0 ? knots.arrayZ[k - 1].z : z;
            }
            hb_hashmap_t<hb_tag_t, Triple> region;
            ಯದಿ (unlikely (!region.set (axis_tag, Triple (lower, z, upper))))
              ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
            item_vars.add_tuple (outer, std::move (region),
                                 inner, delta, item_ಎಣಿಕೆ_2);
          }
        }
      }
      ಅನ್ಯಥಾ
      {
        /* Free or private axis — not being restricted.
         * If it has a non-zero default delta, add it back as a bias.
         * Skip if this (outer,inner) was already processed (shared varIdx). */
        uint32_t varidx = new_varidx_mapping[i];
        ಯದಿ (varidx == HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
          ಅನುವರ್ತ;

        ಯದಿ (processed_varidxes.has (varidx))
          ಅನುವರ್ತ;
        processed_varidxes.add (varidx);

        unsigned outer = varidx >> 16;
        unsigned inner = varidx & 0xFFFF;
        ಪೂರ್ಣಾಙ್ಕ dd = (ಪೂರ್ಣಾಙ್ಕ) roundf (default_deltas[i]);
        ಯದಿ (dd != 0)
        {
          unsigned item_ಎಣಿಕೆ_2 = item_vars.get_item_ಎಣಿಕೆ (outer);
          hb_hashmap_t<hb_tag_t, Triple> empty_region;
          item_vars.add_tuple (outer, std::move (empty_region),
                               inner, dd, item_ಎಣಿಕೆ_2);
        }
      }
    }

    /* 7. Finalize: build region list + convert to varstore */
    ಯದಿ (!item_vars.build_region_list ()) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    ಯದಿ (!item_vars.as_item_varstore (ಸತ್ಯ /* optimize */,
                                     ಅಸತ್ಯ /* use_no_variation_idx */))
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    /* 8. Apply varidx_map optimization remapping */
    ಧ್ರುವ auto &opt_varidx_map = item_vars.get_varidx_map ();
    ಪ್ರತಿ (unsigned i = 0; i < axis_ಎಣಿಕೆ_2; i++)
    {
      uint32_t varidx = new_varidx_mapping[i];
      ಯದಿ (varidx == HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
        ಅನುವರ್ತ;
      uint32_t *new_idx;
      ಯದಿ (opt_varidx_map.has (varidx, &new_idx))
        new_varidx_mapping[i] = *new_idx;
    }

    /* 9. Serialize avarV2Tail. Entries cover the retained axes only;
     * self-contained pinned axes are removed from fvar. */
    avar2_index_map_plan_t index_map_plan;
    ಯದಿ (!index_map_plan.init (new_varidx_mapping,
			      c->plan->axes_index_map,
			      axis_ಎಣಿಕೆ_2))
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    auto *tail = c->serializer->allocate_size<avarV2Tail> (avarV2Tail::static_size);
    ಯದಿ (unlikely (!tail)) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    ಯದಿ (!tail->varIdxMap.serialize_serialize (c->serializer, index_map_plan))
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    ಯದಿ (!tail->varStore.serialize_serialize (c->serializer,
					     item_vars.has_long_word (),
					     c->plan->axis_tags,
					     item_vars.get_region_list (),
					     item_vars.get_vardata_encodings ()))
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
#endif
  }

  ಸಾರ್ವಜನಿಕ:

  ರಕ್ಷಿತ:
  FixedVersion<>version;	/* Version of the avar table
				 * initially set to 0x00010000u */
  HBUINT16	reserved;	/* This field is permanently reserved. Set to 0. */
  HBUINT16	axis_ಎಣಿಕೆ_2;	/* The number of variation axes in the font. This
				 * must be the same number as axisCount in the
				 * 'fvar' table. */
  SegmentMaps	firstAxisSegmentMaps;

  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_MIN (8);
};

} /* namespace OT */


#endif /* HB_OT_VAR_AVAR_TABLE_HH */
