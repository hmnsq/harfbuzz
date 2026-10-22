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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_OT_VAR_AVAR_ਸਾਰਣੀ_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_VAR_AVAR_ਸਾਰਣੀ_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-open-type.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-var-common.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-var-fvar-table.hh"


/*
 * avar -- Axis Variations
 * https://docs.microsoft.com/en-us/typography/opentype/spec/avar
 */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_ਟੈਗ_avar HB_ਟੈਗ('a','v','a','r')


ਨਾਮਾਕਾਸ਼ OT {


/* "Spec": https://github.com/be-fonts/boring-expansion-spec/issues/14 */
ਬਣਤਰ avar_V2_ਪੂਛ
{
  ਮਿੱਤਰ ਬਣਤਰ avar;

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c,
		 ਅਚਲ ਖਾਲੀ *ਅਧਾਰ) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (varIdxMap.sanitize (c, ਅਧਾਰ) &&
		  varStore.sanitize (c, ਅਧਾਰ));
  }

  ਸੁਰੱਖਿਅਤ:
  Offset32_ਮੰਜ਼ਿਲ<Delta_ਸੈੱਟ_ਸੂਚਕਾਂਕ_Map>	varIdxMap;	/* Offset from the beginning of 'avar' table. */
  Offset32_ਮੰਜ਼ਿਲ<ਵਸਤੂ_Variation_Store>	varStore;	/* Offset from the beginning of 'avar' table. */

  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (8);
};


ਬਣਤਰ Axis_ਮੁੱਲ_Map
{
  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ));
  }

  ਖਾਲੀ ਸੈੱਟ_mapping (ਦਸ਼ਮਲਵ from_coord, ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_coord)
  {
    coords[0].ਸੈੱਟ_float (from_coord);
    coords[1].ਸੈੱਟ_float (ਮੰਜ਼ਿਲ_coord);
  }

  ਤਾਰਕਿਕ is_outside_axis_range (ਅਚਲ Triple& axis_range) ਅਚਲ
  {
    ਦੁੱਗਣਾ from_coord = (ਦੁੱਗਣਾ) coords[0].ਮੰਜ਼ਿਲ_float ();
    ਵਾਪਸ !axis_range.ਸ਼ਾਮਲ_ਹੈ (from_coord);
  }

  ਤਾਰਕਿਕ must_include () ਅਚਲ
  {
    ਦਸ਼ਮਲਵ from_coord = coords[0].ਮੰਜ਼ਿਲ_float ();
    ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_coord = coords[1].ਮੰਜ਼ਿਲ_float ();
    ਵਾਪਸ (from_coord == -1.f && ਮੰਜ਼ਿਲ_coord == -1.f) ||
           (from_coord == 0.f && ਮੰਜ਼ਿਲ_coord == 0.f) ||
           (from_coord == 1.f && ਮੰਜ਼ਿਲ_coord == 1.f);
  }

  ਖਾਲੀ instantiate (ਅਚਲ Triple& axis_range,
                    ਅਚਲ Triple& unmapped_range,
                    ਅਚਲ TripleDistances& triple_distances)
  {
    ਦਸ਼ਮਲਵ from_coord = coords[0].ਮੰਜ਼ਿਲ_float ();
    ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_coord = coords[1].ਮੰਜ਼ਿਲ_float ();

    from_coord = renormalize_ਮੁੱਲ ((ਦੁੱਗਣਾ) from_coord, unmapped_range, triple_distances);
    ਮੰਜ਼ਿਲ_coord = renormalize_ਮੁੱਲ ((ਦੁੱਗਣਾ) ਮੰਜ਼ਿਲ_coord, axis_range, triple_distances);

    /* Malformed input can renormalize outside [-1,1], which would
     * overflow F2DOT14. */
    from_coord = hb_clamp (from_coord, -1.f, 1.f);
    ਮੰਜ਼ਿਲ_coord = hb_clamp (ਮੰਜ਼ਿਲ_coord, -1.f, 1.f);

    coords[0].ਸੈੱਟ_float (from_coord);
    coords[1].ਸੈੱਟ_float (ਮੰਜ਼ਿਲ_coord);
  }

  HB_ਅੰਦਰੂਨੀ ਸਥਿਰ ਪੂਰਨ cmp (ਅਚਲ ਖਾਲੀ *pa, ਅਚਲ ਖਾਲੀ *pb)
  {
    ਅਚਲ Axis_ਮੁੱਲ_Map *a = (ਅਚਲ Axis_ਮੁੱਲ_Map *) pa;
    ਅਚਲ Axis_ਮੁੱਲ_Map *b = (ਅਚਲ Axis_ਮੁੱਲ_Map *) pb;

    ਪੂਰਨ a_from = a->coords[0].ਮੰਜ਼ਿਲ_int ();
    ਪੂਰਨ b_from = b->coords[0].ਮੰਜ਼ਿਲ_int ();
    ਜੇ (a_from != b_from)
      ਵਾਪਸ a_from - b_from;

    /* this should never be reached. according to the spec, all of the axis
     * value map records for a given axis must have different fromCoord values
     * */
    ਪੂਰਨ a_ਮੰਜ਼ਿਲ = a->coords[1].ਮੰਜ਼ਿਲ_int ();
    ਪੂਰਨ b_ਮੰਜ਼ਿਲ = b->coords[1].ਮੰਜ਼ਿਲ_int ();
    ਵਾਪਸ a_ਮੰਜ਼ਿਲ - b_ਮੰਜ਼ਿਲ;
  }

  ਤਾਰਕਿਕ serialize (hb_serialize_context_t *c) ਅਚਲ
  {
    TRACE_SERIALIZE (ਇਹ);
    return_trace (c->embed (ਇਹ));
  }

  ਜਨਤਕ:
  F2DOT14	coords[2];
//   F2DOT14	fromCoord;	/* A normalized coordinate value obtained using
//				 * default normalization. */
//   F2DOT14	toCoord;	/* The modified, normalized coordinate value. */

  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (4);
};

ਬਣਤਰ SegmentMaps : Array16_ਦਾ<Axis_ਮੁੱਲ_Map>
{
  ਦਸ਼ਮਲਵ map_float (ਦਸ਼ਮਲਵ value, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ from_ਵਿੱਥ = 0, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਮੰਜ਼ਿਲ_ਵਿੱਥ = 1) ਅਚਲ
  {
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ fromCoord coords[from_ਵਿੱਥ].ਮੰਜ਼ਿਲ_float ()
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਮੰਜ਼ਿਲ_Coord coords[ਮੰਜ਼ਿਲ_ਵਿੱਥ].ਮੰਜ਼ਿਲ_float ()

    ਅਚਲ ਸਵੈਚਾਲਿਤ *map = arrayZ;

    /* The following special-cases are not part of OpenType, which requires
     * that at least -1, 0, and +1 must be mapped. But we include these as
     * part of a better error recovery scheme. */
    ਜੇ (len < 2)
    {
      ਜੇ (!len)
	ਵਾਪਸ value;
      ਨਹੀਂਤਾਂ /* len == 1*/
	ਵਾਪਸ value - map[0].fromCoord + map[0].ਮੰਜ਼ਿਲ_Coord;
    }

    // At least two mappings now.

    /* CoreText is wild...
     * PingFangUI avar needs all this special-casing...
     * So we implement an extended version of the spec here,
     * which is more robust and more likely to be compatible with
     * the wild. */

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਸ਼ੁਰੂ = 0;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਸਿਰਾ = len;
    ਜੇ (map[ਸ਼ੁਰੂ].fromCoord == -1 && map[ਸ਼ੁਰੂ].ਮੰਜ਼ਿਲ_Coord == -1 && map[ਸ਼ੁਰੂ+1].fromCoord == -1)
      ਸ਼ੁਰੂ++;
    ਜੇ (map[ਸਿਰਾ-1].fromCoord == +1 && map[ਸਿਰਾ-1].ਮੰਜ਼ਿਲ_Coord == +1 && map[ਸਿਰਾ-2].fromCoord == +1)
      ਸਿਰਾ--;

    /* Look for exact match first, and do lots of special-casing. */
    ਬਿਨਾਨਿਸ਼ਾਨੀ i;
    ਲਈ (i = ਸ਼ੁਰੂ; i < ਸਿਰਾ; i++)
      ਜੇ (value == map[i].fromCoord)
	ਰੋਕੋ;
    ਜੇ (i < ਸਿਰਾ)
    {
      // There's at least one exact match. See if there are more.
      ਬਿਨਾਨਿਸ਼ਾਨੀ j = i;
      ਲਈ (; j + 1 < ਸਿਰਾ; j++)
	ਜੇ (value != map[j + 1].fromCoord)
	  ਰੋਕੋ;

      // [i,j] inclusive are all exact matches:

      // If there's only one, return it. This is the only spec-compliant case.
      ਜੇ (i == j)
	ਵਾਪਸ map[i].ਮੰਜ਼ਿਲ_Coord;
      // If there's exactly three, return the middle one.
      ਜੇ (i + 2 == j)
	ਵਾਪਸ map[i + 1].ਮੰਜ਼ਿਲ_Coord;

      // Ignore the middle ones. Return the one mapping closer to 0.
      ਜੇ (value < 0) ਵਾਪਸ map[j].ਮੰਜ਼ਿਲ_Coord;
      ਜੇ (value > 0) ਵਾਪਸ map[i].ਮੰਜ਼ਿਲ_Coord;

      // Mapping 0? CoreText seems confused. It seems to prefer 0 here...
      // So we'll just return the smallest one. lol
      ਵਾਪਸ fabsf (map[i].ਮੰਜ਼ਿਲ_Coord) < fabsf (map[j].ਮੰਜ਼ਿਲ_Coord) ? map[i].ਮੰਜ਼ਿਲ_Coord : map[j].ਮੰਜ਼ਿਲ_Coord;

      // Mapping 0? Return one not mapping to 0.
      ਜੇ (map[i].ਮੰਜ਼ਿਲ_Coord == 0)
	ਵਾਪਸ map[j].ਮੰਜ਼ਿਲ_Coord;
      ਨਹੀਂਤਾਂ
	ਵਾਪਸ map[i].ਮੰਜ਼ਿਲ_Coord;
    }

    /* There's at least two and we're not an exact match. Prepare to lerp. */

    // Find the segment we're in.
    ਲਈ (i = ਸ਼ੁਰੂ; i < ਸਿਰਾ; i++)
      ਜੇ (value < map[i].fromCoord)
	ਰੋਕੋ;

    ਜੇ (i == ਸ਼ੁਰੂ)
    {
      // Value before all segments; Shift.
      ਵਾਪਸ value - map[ਸ਼ੁਰੂ].fromCoord + map[ਸ਼ੁਰੂ].ਮੰਜ਼ਿਲ_Coord;
    }
    ਜੇ (i == ਸਿਰਾ)
    {
      // Value after all segments; Shift.
      ਵਾਪਸ value - map[ਸਿਰਾ - 1].fromCoord + map[ਸਿਰਾ - 1].ਮੰਜ਼ਿਲ_Coord;
    }

    // Actually interpolate.
    ਸਵੈਚਾਲਿਤ &before = map[i-1];
    ਸਵੈਚਾਲਿਤ &after = map[i];
    ਦਸ਼ਮਲਵ denom = after.fromCoord - before.fromCoord; // Can't be zero by now.
    ਵਾਪਸ before.ਮੰਜ਼ਿਲ_Coord + ((after.ਮੰਜ਼ਿਲ_Coord - before.ਮੰਜ਼ਿਲ_Coord) * (value - before.fromCoord)) / denom;

#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ ਮੰਜ਼ਿਲ_Coord
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ fromCoord
  }

  ਦਸ਼ਮਲਵ unmap_float (ਦਸ਼ਮਲਵ value) ਅਚਲ { ਵਾਪਸ map_float (value, 1, 0); }


  // TODO Kill this.
  Triple unmap_axis_range (ਅਚਲ Triple& axis_range) ਅਚਲ
  {
    ਦਸ਼ਮਲਵ unmapped_min = unmap_float (axis_range.minimum);
    ਦਸ਼ਮਲਵ unmapped_ਵਿਚਕਾਰ = unmap_float (axis_range.ਵਿਚਕਾਰ);
    ਦਸ਼ਮਲਵ unmapped_max = unmap_float (axis_range.ਵੱਧ_ਤੋਂ_ਵੱਧ);

    ਵਾਪਸ Triple{unmapped_min, unmapped_ਵਿਚਕਾਰ, unmapped_max};
  }

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c, hb_ਟੈਗ_t axis_ਟੈਗ_2,
               hb_ਸਦਿਸ਼_t<Axis_ਮੁੱਲ_Map> *ਬਾਹਰਲਾ_mappings = ਖਾਲੀ_ਸੂਚਕ) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);

    /* This function cannot work on avar2 table (and currently doesn't).
     * We should instead keep the design coords in the shape plan and use
     * those. unmap_axis_range needs to be killed. */

    /* avar mapped normalized axis range. Under avar2, axes_location holds
     * only the self-contained pins for the other tables; avar itself uses
     * the intermediate-space ranges of all restricted axes. */
    ਅਚਲ ਸਵੈਚਾਲਿਤ &axes_location = c->plan->has_avar2
				? c->plan->old_intermediates
				: c->plan->axes_location;
    Triple *axis_range;
    ਜੇ (!axes_location.has (axis_ਟੈਗ_2, &axis_range))
      ਵਾਪਸ c->serializer->embed (*ਇਹ);

    TripleDistances *axis_triple_distances;
    ਜੇ (!c->plan->axes_triple_distances.has (axis_ਟੈਗ_2, &axis_triple_distances))
      return_trace (ਝੂਠ);

    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->serializer->ਸ਼ੁਰੂ_embed (ਇਹ);
    ਜੇ (unlikely (!c->serializer->extend_min (ਬਾਹਰਲਾ))) return_trace (ਝੂਠ);

    Triple unmapped_range = unmap_axis_range (*axis_range);

    /* create a vector of retained mappings and sort */
    hb_ਸਦਿਸ਼_t<Axis_ਮੁੱਲ_Map> ਮੁੱਲ_mappings;
    ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ& _ : as_array ())
    {
      ਜੇ (_.is_outside_axis_range (unmapped_range))
        ਜਾਰੀ;
      Axis_ਮੁੱਲ_Map mapping;
      mapping = _;
      mapping.instantiate (*axis_range, unmapped_range, *axis_triple_distances);
      /* (-1, -1), (0, 0), (1, 1) mappings will be added later, so avoid
       * duplicates here */
      ਜੇ (mapping.must_include ())
        ਜਾਰੀ;
      ਮੁੱਲ_mappings.push (mapping);
    }

    Axis_ਮੁੱਲ_Map m;
    m.ਸੈੱਟ_mapping (-1.f, -1.f);
    ਮੁੱਲ_mappings.push (m);

    m.ਸੈੱਟ_mapping (0.f, 0.f);
    ਮੁੱਲ_mappings.push (m);

    m.ਸੈੱਟ_mapping (1.f, 1.f);
    ਮੁੱਲ_mappings.push (m);

    ਮੁੱਲ_mappings.qsort ();

    ਜੇ (unlikely (ਮੁੱਲ_mappings.ਵਿੱਚ_error ()))
      return_trace (ਝੂਠ);

    ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ& _ : ਮੁੱਲ_mappings)
    {
      ਜੇ (!_.serialize (c->serializer))
        return_trace (ਝੂਠ);
    }
    ਜੇ (!c->serializer->check_ਸੌਂਪੋ (ਬਾਹਰਲਾ->len, ਮੁੱਲ_mappings.ਲੰਮਾਈ, HB_SERIALIZE_ERROR_INT_ਉਛਾਲ))
      return_trace (ਝੂਠ);
    /* Hand the instantiated mappings to the caller; avar2 offset
     * compensation needs them to locate the new mapping's kinks. */
    ਜੇ (ਬਾਹਰਲਾ_mappings)
      *ਬਾਹਰਲਾ_mappings = std::move (ਮੁੱਲ_mappings);
    return_trace (ਸੱਚ);
  }

  ਜਨਤਕ:
  DEFINE_ਮਾਪ_ARRAY (2, *ਇਹ);
};

/* One knot of the avar2 offset-compensation function
 * offset(z) = inv_renorm(z) - z, in new intermediate space. */
ਬਣਤਰ avar2_ਵਿੱਥ_knot_t
{
  ਦੁੱਗਣਾ z;
  ਦੁੱਗਣਾ ਵਿੱਥ;
};

/* Insert a knot keeping the vector sorted by z; first insertion wins on
 * duplicate z. */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
avar2_ਜੋੜੋ_knot (hb_ਸਦਿਸ਼_t<avar2_ਵਿੱਥ_knot_t> &knots, ਦੁੱਗਣਾ z, ਦੁੱਗਣਾ ਵਿੱਥ)
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0;
  ਜਦੋਂ (i < knots.ਲੰਮਾਈ && knots.arrayZ[i].z < z) i++;
  ਜੇ (i < knots.ਲੰਮਾਈ && knots.arrayZ[i].z == z) ਵਾਪਸ;
  knots.push (avar2_ਵਿੱਥ_knot_t {0.0, 0.0});
  ਜੇ (unlikely (knots.ਵਿੱਚ_error ())) ਵਾਪਸ;
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ j = knots.ਲੰਮਾਈ - 1; j > i; j--)
    knots.arrayZ[j] = knots.arrayZ[j - 1];
  knots.arrayZ[i] = avar2_ਵਿੱਥ_knot_t {z, ਵਿੱਥ};
}

/* Piecewise-linear evaluation over the sorted knots. Inputs are always
 * within [-1, +1] and anchor knots at -1/0/+1 always exist. */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਦੁੱਗਣਾ
avar2_eval_ਵਿੱਥ (ਅਚਲ hb_ਸਦਿਸ਼_t<avar2_ਵਿੱਥ_knot_t> &knots, ਦੁੱਗਣਾ z)
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ len = knots.ਲੰਮਾਈ;
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < len; i++)
  {
    ਜੇ (z == knots.arrayZ[i].z) ਵਾਪਸ knots.arrayZ[i].ਵਿੱਥ;
    ਜੇ (z < knots.arrayZ[i].z)
    {
      ਜੇ (!i) ਵਾਪਸ knots.arrayZ[0].ਵਿੱਥ;
      ਅਚਲ ਸਵੈਚਾਲਿਤ &before = knots.arrayZ[i - 1];
      ਅਚਲ ਸਵੈਚਾਲਿਤ &after = knots.arrayZ[i];
      ਦੁੱਗਣਾ denom = after.z - before.z;
      ਵਾਪਸ before.ਵਿੱਥ + (after.ਵਿੱਥ - before.ਵਿੱਥ) * (z - before.z) / denom;
    }
  }
  ਵਾਪਸ len ? knots.arrayZ[len - 1].ਵਿੱਥ : 0.0;
}

/* Piecewise-linear evaluation over instantiated avar v1 mappings, as
 * produced by SegmentMaps::subset (sorted, with -1/0/+1 anchors).
 * Matches SegmentMaps::map_float for such well-formed mappings. */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਦੁੱਗਣਾ
_avar2_map_new_mapping (ਅਚਲ hb_ਸਦਿਸ਼_t<Axis_ਮੁੱਲ_Map> &mappings, ਦੁੱਗਣਾ v,
			ਬਿਨਾਨਿਸ਼ਾਨੀ from_ਵਿੱਥ = 0, ਬਿਨਾਨਿਸ਼ਾਨੀ ਮੰਜ਼ਿਲ_ਵਿੱਥ = 1)
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ len = mappings.ਲੰਮਾਈ;
  ਜੇ (!len) ਵਾਪਸ v;

  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < len; i++)
  {
    ਦੁੱਗਣਾ from = (ਦੁੱਗਣਾ) mappings.arrayZ[i].coords[from_ਵਿੱਥ].ਮੰਜ਼ਿਲ_float ();
    ਜੇ (v == from)
      ਵਾਪਸ (ਦੁੱਗਣਾ) mappings.arrayZ[i].coords[ਮੰਜ਼ਿਲ_ਵਿੱਥ].ਮੰਜ਼ਿਲ_float ();
    ਜੇ (v < from)
    {
      ਦੁੱਗਣਾ to = (ਦੁੱਗਣਾ) mappings.arrayZ[i].coords[ਮੰਜ਼ਿਲ_ਵਿੱਥ].ਮੰਜ਼ਿਲ_float ();
      ਜੇ (!i) ਵਾਪਸ v - from + to;
      ਦੁੱਗਣਾ prev_from = (ਦੁੱਗਣਾ) mappings.arrayZ[i - 1].coords[from_ਵਿੱਥ].ਮੰਜ਼ਿਲ_float ();
      ਦੁੱਗਣਾ prev_ਮੰਜ਼ਿਲ = (ਦੁੱਗਣਾ) mappings.arrayZ[i - 1].coords[ਮੰਜ਼ਿਲ_ਵਿੱਥ].ਮੰਜ਼ਿਲ_float ();
      ਦੁੱਗਣਾ denom = from - prev_from;
      ਜੇ (denom == 0.0) ਵਾਪਸ prev_ਮੰਜ਼ਿਲ;
      ਵਾਪਸ prev_ਮੰਜ਼ਿਲ + (to - prev_ਮੰਜ਼ਿਲ) * (v - prev_from) / denom;
    }
  }
  ਵਾਪਸ v - (ਦੁੱਗਣਾ) mappings.arrayZ[len - 1].coords[from_ਵਿੱਥ].ਮੰਜ਼ਿਲ_float ()
           + (ਦੁੱਗਣਾ) mappings.arrayZ[len - 1].coords[ਮੰਜ਼ਿਲ_ਵਿੱਥ].ਮੰਜ਼ਿਲ_float ();
}

/* Inverse of _avar2_map_new_mapping: pull an output coordinate back to a
 * preimage input coordinate. For a non-strictly-monotone mapping this picks
 * one preimage, which is fine for its only use (augmenting the error
 * estimator's sample set). */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਦੁੱਗਣਾ
_avar2_unmap_new_mapping (ਅਚਲ hb_ਸਦਿਸ਼_t<Axis_ਮੁੱਲ_Map> &mappings, ਦੁੱਗਣਾ v)
{
  ਵਾਪਸ _avar2_map_new_mapping (mappings, v, 1, 0);
}

/* Plain (non-avar) fvar-style normalization of a user value against a
 * user-space (min, default, max) triple. */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਦੁੱਗਣਾ
avar2_normalize_ਮੁੱਲ (ਦੁੱਗਣਾ v, ਦੁੱਗਣਾ min, ਦੁੱਗਣਾ def, ਦੁੱਗਣਾ max)
{
  v = hb_clamp (v, min, max);
  ਜੇ (v == def) ਵਾਪਸ 0.0;
  ਜੇ (v < def) ਵਾਪਸ def == min ? 0.0 : (v - def) / (def - min);
  ਵਾਪਸ def == max ? 0.0 : (v - def) / (max - def);
}

/* Inverse of the above: map a normalized value back to user space. */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਦੁੱਗਣਾ
avar2_denormalize_ਮੁੱਲ (ਦੁੱਗਣਾ v, ਦੁੱਗਣਾ min, ਦੁੱਗਣਾ def, ਦੁੱਗਣਾ max)
{
  ਜੇ (v == 0.0) ਵਾਪਸ def;
  ਵਾਪਸ v < 0.0 ? def + v * (def - min) : def + v * (max - def);
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
ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਿਨਾਨਿਸ਼ਾਨੀ
avar2_estimate_ਵਿੱਥ_error (ਅਚਲ SegmentMaps &old_seg,
                              ਅਚਲ hb_ਸਦਿਸ਼_t<Axis_ਮੁੱਲ_Map> &new_mapping,
                              ਦੁੱਗਣਾ old_min, ਦੁੱਗਣਾ old_def, ਦੁੱਗਣਾ old_max,
                              ਦੁੱਗਣਾ new_min, ਦੁੱਗਣਾ new_def, ਦੁੱਗਣਾ new_max,
                              ਅਚਲ hb_ਸਦਿਸ਼_t<avar2_ਵਿੱਥ_knot_t> &knots)
{
  constexpr ਬਿਨਾਨਿਸ਼ਾਨੀ samples = 257;
  hb_ਸਦਿਸ਼_t<ਦੁੱਗਣਾ> us;
  ਜੇ (unlikely (!us.alloc (samples + old_seg.as_array ().ਲੰਮਾਈ +
                           new_mapping.ਲੰਮਾਈ + knots.ਲੰਮਾਈ)))
    ਵਾਪਸ UINT_MAX; /* estimator only; fail towards "worse" */
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < samples; i++)
    us.push (new_min + (new_max - new_min) * i / (samples - 1));
  ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ &_ : old_seg.as_array ())
    us.push (avar2_denormalize_ਮੁੱਲ ((ਦੁੱਗਣਾ) _.coords[0].ਮੰਜ਼ਿਲ_float (),
                                       old_min, old_def, old_max));
  ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ &_ : new_mapping)
    us.push (avar2_denormalize_ਮੁੱਲ ((ਦੁੱਗਣਾ) _.coords[0].ਮੰਜ਼ਿਲ_float (),
                                       new_min, new_def, new_max));
  ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ &knot : knots)
    us.push (avar2_denormalize_ਮੁੱਲ (
               _avar2_unmap_new_mapping (new_mapping, knot.z),
               new_min, new_def, new_max));
  ਜੇ (unlikely (us.ਵਿੱਚ_error ())) ਵਾਪਸ UINT_MAX;

  ਬਿਨਾਨਿਸ਼ਾਨੀ max_err = 0;
  ਲਈ (ਦੁੱਗਣਾ u : us)
  {
    ਜੇ (u < new_min || u > new_max) ਜਾਰੀ;
    ਦੁੱਗਣਾ n_old = avar2_normalize_ਮੁੱਲ (u, old_min, old_def, old_max);
    ਦੁੱਗਣਾ old_final = (ਦੁੱਗਣਾ) old_seg.map_float ((ਦਸ਼ਮਲਵ) n_old);
    ਦੁੱਗਣਾ n_new = avar2_normalize_ਮੁੱਲ (u, new_min, new_def, new_max);
    ਦੁੱਗਣਾ z = _avar2_map_new_mapping (new_mapping, n_new);
    ਦੁੱਗਣਾ new_final = z + avar2_eval_ਵਿੱਥ (knots, z);
    ਪੂਰਨ err = ਨਿਰਪੇਖ ((ਪੂਰਨ) roundf ((ਦਸ਼ਮਲਵ) (old_final * 16384.0)) -
                   (ਪੂਰਨ) roundf ((ਦਸ਼ਮਲਵ) (new_final * 16384.0)));
    ਜੇ ((ਬਿਨਾਨਿਸ਼ਾਨੀ) err > max_err) max_err = err;
  }
  ਵਾਪਸ max_err;
}

ਬਣਤਰ avar
{
  ਸਥਿਰ constexpr hb_ਟੈਗ_t ਸਾਰਣੀ_ਟੈਗ = HB_OT_ਟੈਗ_avar;

  ਤਾਰਕਿਕ has_ਅੰਕੜੇ () ਅਚਲ { ਵਾਪਸ ਸੰਸਕਰਣ.ਮੰਜ਼ਿਲ_int (); }

  ਅਚਲ SegmentMaps* get_segment_maps () ਅਚਲ
  { ਵਾਪਸ &ਪਹਿਲਾ_Axis_Segment_Maps; }

  ਬਿਨਾਨਿਸ਼ਾਨੀ get_axis_ਗਿਣਤੀ () ਅਚਲ
  { ਵਾਪਸ axis_ਗਿਣਤੀ_2; }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    ਜੇ (!(ਸੰਸਕਰਣ.sanitize (c) &&
	  hb_barrier () &&
	  (ਸੰਸਕਰਣ.major == 1
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_AVAR2
	   || ਸੰਸਕਰਣ.major == 2
#ਜੇ_ਅੰਤ
	   ) &&
	  c->check_struct (ਇਹ)))
      return_trace (ਝੂਠ);

    ਅਚਲ SegmentMaps *map = &ਪਹਿਲਾ_Axis_Segment_Maps;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2 = axis_ਗਿਣਤੀ_2;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++)
    {
      ਜੇ (unlikely (!map->sanitize (c)))
	return_trace (ਝੂਠ);
      map = &StructAfter<SegmentMaps> (*map);
    }

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_AVAR2
    ਜੇ (ਸੰਸਕਰਣ.major < 2)
      return_trace (ਸੱਚ);
    hb_barrier ();

    ਅਚਲ ਸਵੈਚਾਲਿਤ &v2 = * (ਅਚਲ avar_V2_ਪੂਛ *) map;
    ਜੇ (unlikely (!v2.sanitize (c, ਇਹ)))
      return_trace (ਝੂਠ);
#ਜੇ_ਅੰਤ

    return_trace (ਸੱਚ);
  }

  ਖਾਲੀ map_coords_16_16 (ਪੂਰਨ *coords, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ coords_ਲੰਮਾਈ) ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2 = hb_min (coords_ਲੰਮਾਈ, axis_ਗਿਣਤੀ_2);

    ਅਚਲ SegmentMaps *map = &ਪਹਿਲਾ_Axis_Segment_Maps;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++)
    {
      coords[i] = roundf (map->map_float (coords[i] / 65536.f) * 65536.f);
      map = &StructAfter<SegmentMaps> (*map);
    }

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_AVAR2
    ਜੇ (ਸੰਸਕਰਣ.major < 2)
      ਵਾਪਸ;
    hb_barrier ();

    ਲਈ (; ਗਿਣਤੀ_2 < axis_ਗਿਣਤੀ_2; ਗਿਣਤੀ_2++)
      map = &StructAfter<SegmentMaps> (*map);

    ਅਚਲ ਸਵੈਚਾਲਿਤ &v2 = * (ਅਚਲ avar_V2_ਪੂਛ *) map;

    ਅਚਲ ਸਵੈਚਾਲਿਤ &varidx_map = ਇਹ+v2.varIdxMap;
    ਅਚਲ ਸਵੈਚਾਲਿਤ &var_store = ਇਹ+v2.varStore;
    ਸਵੈਚਾਲਿਤ *var_store_cache = var_store.create_cache ();

    hb_ਸਦਿਸ਼_t<ਪੂਰਨ> coords_2_14;
    coords_2_14.resize (coords_ਲੰਮਾਈ);
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < coords_ਲੰਮਾਈ; i++)
      coords_2_14[i] = roundf (coords[i] / 4.f); // 16.16 -> 2.14

    hb_ਸਦਿਸ਼_t<ਪੂਰਨ> ਬਾਹਰਲਾ;
    ਬਾਹਰਲਾ.alloc (coords_ਲੰਮਾਈ);
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < coords_ਲੰਮਾਈ; i++)
    {
      ਪੂਰਨ v = coords[i];
      uint32_t varidx = varidx_map.map (i);
      ਦਸ਼ਮਲਵ delta = var_store.get_delta (varidx, coords_2_14.arrayZ, coords_2_14.ਲੰਮਾਈ, var_store_cache);
      /* Apply the delta unclamped and clamp only the result to [-1, +1],
       * matching fontTools. Since inputs and results are in [-1, +1],
       * deltas beyond ±2 are equivalent to ±2; clamp to that range only
       * to keep the float->int conversion safe. */
      ਦਸ਼ਮਲਵ d = hb_clamp (delta * 4, -(ਦਸ਼ਮਲਵ) (1<<17), +(ਦਸ਼ਮਲਵ) (1<<17)); // 2.14 -> 16.16
      v += (ਪੂਰਨ) roundf (d);
      v = hb_clamp (v, -(1<<16), +(1<<16));
      ਬਾਹਰਲਾ.push (v);
    }
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < coords_ਲੰਮਾਈ; i++)
      coords[i] = ਬਾਹਰਲਾ[i];

    OT::ਵਸਤੂ_Variation_Store::destroy_cache (var_store_cache);
#ਜੇ_ਅੰਤ
  }

  /* An avar version 2 font is never downgraded to v1: even with a NULL
   * varStore offset (legal; maps like plain v1) the avar2 subsetting path
   * applies, with "no variation" semantics for rows that don't resolve —
   * offset compensation then creates delta rows on demand. */
  ਤਾਰਕਿਕ has_v2_ਅੰਕੜੇ () ਅਚਲ { ਵਾਪਸ ਸੰਸਕਰਣ.major > 1; }

  /* Resolve the avar2 VarStore and VarIdxMap, for the subset planner.
   * Either pointer may be to the Null object (nullable offsets). */
  ਤਾਰਕਿਕ get_v2_store_and_map (ਅਚਲ ਵਸਤੂ_Variation_Store **store,
                             ਅਚਲ Delta_ਸੈੱਟ_ਸੂਚਕਾਂਕ_Map **varidx_map) ਅਚਲ
  {
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_AVAR2
    ਜੇ (ਸੰਸਕਰਣ.major < 2) ਵਾਪਸ ਝੂਠ;
    ਅਚਲ SegmentMaps *map = &ਪਹਿਲਾ_Axis_Segment_Maps;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < axis_ਗਿਣਤੀ_2; i++)
      map = &StructAfter<SegmentMaps> (*map);
    ਅਚਲ ਸਵੈਚਾਲਿਤ &v2 = * (ਅਚਲ avar_V2_ਪੂਛ *) map;
    *store = &(ਇਹ+v2.varStore);
    *varidx_map = &(ਇਹ+v2.varIdxMap);
    ਵਾਪਸ ਸੱਚ;
#ਨਹੀਂਤਾਂ
    ਵਾਪਸ ਝੂਠ;
#ਜੇ_ਅੰਤ
  }

  // axis normalization is done in 2.14 here
  // TODO: deprecate this API once fonttools is updated to use 16.16 normalization
  ਤਾਰਕਿਕ map_coords_2_14 (ਦਸ਼ਮਲਵ *coords, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ coords_ਲੰਮਾਈ,
			ਤਾਰਕਿਕ v1_only = ਝੂਠ) ਅਚਲ
  {
    hb_ਸਦਿਸ਼_t<ਪੂਰਨ> coords_2_14;
    ਜੇ (!v1_only && !coords_2_14.resize (coords_ਲੰਮਾਈ)) ਵਾਪਸ ਝੂਠ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2 = hb_min (coords_ਲੰਮਾਈ, axis_ਗਿਣਤੀ_2);

    ਅਚਲ SegmentMaps *map = &ਪਹਿਲਾ_Axis_Segment_Maps;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++)
    {
      ਪੂਰਨ v = roundf (map->map_float (coords[i]) * 16384.f);
      ਜੇ (!v1_only)
	coords_2_14[i] = v;
      coords[i] = v / 16384.f;
      map = &StructAfter<SegmentMaps> (*map);
    }

    ਜੇ (v1_only)
      ਵਾਪਸ ਸੱਚ;

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_AVAR2
    ਜੇ (ਸੰਸਕਰਣ.major < 2)
      ਵਾਪਸ ਸੱਚ;
    hb_barrier ();

    ਲਈ (; ਗਿਣਤੀ_2 < axis_ਗਿਣਤੀ_2; ਗਿਣਤੀ_2++)
      map = &StructAfter<SegmentMaps> (*map);

    ਅਚਲ ਸਵੈਚਾਲਿਤ &v2 = * (ਅਚਲ avar_V2_ਪੂਛ *) map;

    ਅਚਲ ਸਵੈਚਾਲਿਤ &varidx_map = ਇਹ+v2.varIdxMap;
    ਅਚਲ ਸਵੈਚਾਲਿਤ &var_store = ਇਹ+v2.varStore;
    ਸਵੈਚਾਲਿਤ *var_store_cache = var_store.create_cache ();

    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < coords_ਲੰਮਾਈ; i++)
    {
      ਪੂਰਨ v = coords_2_14[i];
      uint32_t varidx = varidx_map.map (i);
      ਦਸ਼ਮਲਵ delta = var_store.get_delta (varidx, coords_2_14.arrayZ, coords_2_14.ਲੰਮਾਈ, var_store_cache);
      /* As above: apply the delta unclamped (±2 covers every useful case)
       * and clamp the result to [-1, +1], matching fontTools. */
      v += (ਪੂਰਨ) hb_clamp (roundf (delta), -(ਦਸ਼ਮਲਵ) (1<<15), +(ਦਸ਼ਮਲਵ) (1<<15));
      v = hb_clamp (v, -(1<<14), +(1<<14));
      coords[i] = v / 16384.f;
    }

    OT::ਵਸਤੂ_Variation_Store::destroy_cache (var_store_cache);
    ਵਾਪਸ ਸੱਚ;
#ਨਹੀਂਤਾਂ
    ਵਾਪਸ ਸੰਸਕਰਣ.major < 2;
#ਜੇ_ਅੰਤ
  }

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਬਿਨਾਨਿਸ਼ਾਨੀ retained_axis_ਗਿਣਤੀ = c->plan->axes_ਸੂਚਕਾਂਕ_map.get_population ();
    ਜੇ (!retained_axis_ਗਿਣਤੀ) //all axes are pinned/dropped
      return_trace (ਝੂਠ);

    avar *ਬਾਹਰਲਾ = c->serializer->allocate_min<avar> ();
    ਜੇ (unlikely (!ਬਾਹਰਲਾ)) return_trace (ਝੂਠ);

    ਬਾਹਰਲਾ->ਸੰਸਕਰਣ.major = c->plan->has_avar2 ? 2 : 1;
    ਬਾਹਰਲਾ->ਸੰਸਕਰਣ.minor = 0;
    ਜੇ (!c->serializer->check_ਸੌਂਪੋ (ਬਾਹਰਲਾ->axis_ਗਿਣਤੀ_2, retained_axis_ਗਿਣਤੀ, HB_SERIALIZE_ERROR_INT_ਉਛਾਲ))
      return_trace (ਝੂਠ);

    /* For avar2, keep the instantiated v1 mappings around; offset
     * compensation needs them to locate the new mappings' kinks. */
    hb_ਸਦਿਸ਼_t<hb_ਸਦਿਸ਼_t<Axis_ਮੁੱਲ_Map>> new_mappings;
    ਜੇ (c->plan->has_avar2 && !new_mappings.resize (axis_ਗਿਣਤੀ_2))
      return_trace (ਝੂਠ);

    ਅਚਲ hb_map_t& axes_ਸੂਚਕਾਂਕ_map = c->plan->axes_ਸੂਚਕਾਂਕ_map;
    ਅਚਲ SegmentMaps *map = &ਪਹਿਲਾ_Axis_Segment_Maps;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2 = axis_ਗਿਣਤੀ_2;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++)
    {
      ਜੇ (axes_ਸੂਚਕਾਂਕ_map.has (i))
      {
        hb_ਟੈਗ_t *axis_ਟੈਗ_2;
        ਜੇ (!c->plan->axes_old_ਸੂਚਕਾਂਕ_ਟੈਗ_map.has (i, &axis_ਟੈਗ_2))
          return_trace (ਝੂਠ);

        Triple *axis_location;
        ਜੇ (c->plan->has_avar2 &&
            c->plan->user_axes_location.has (*axis_ਟੈਗ_2, &axis_location) &&
            axis_location->is_ਬਿੰਦੂ ())
        {
          /* Pinned axis in avar2 mode: serialize identity segment map
           * {-1->-1, 0->0, 1->1}. The axis is kept in fvar as hidden,
           * so avar needs a segment map entry for it. */
          ਸਵੈਚਾਲਿਤ *ਪਛਾਣ_ਤੱਤ_map = c->serializer->ਸ਼ੁਰੂ_embed<SegmentMaps> ();
          ਜੇ (unlikely (!c->serializer->extend_min (ਪਛਾਣ_ਤੱਤ_map)))
            return_trace (ਝੂਠ);
          Axis_ਮੁੱਲ_Map m;
          m.ਸੈੱਟ_mapping (-1.f, -1.f);
          ਜੇ (!m.serialize (c->serializer)) return_trace (ਝੂਠ);
          m.ਸੈੱਟ_mapping (0.f, 0.f);
          ਜੇ (!m.serialize (c->serializer)) return_trace (ਝੂਠ);
          m.ਸੈੱਟ_mapping (1.f, 1.f);
          ਜੇ (!m.serialize (c->serializer)) return_trace (ਝੂਠ);
          ਜੇ (!c->serializer->check_ਸੌਂਪੋ (ਪਛਾਣ_ਤੱਤ_map->len, 3u,
                                            HB_SERIALIZE_ERROR_INT_ਉਛਾਲ))
            return_trace (ਝੂਠ);
        }
        ਨਹੀਂਤਾਂ
        {
          /* Restricted or free axis: use standard SegmentMaps::subset() */
          ਜੇ (!map->subset (c, *axis_ਟੈਗ_2,
                            c->plan->has_avar2 ? &new_mappings[i] : ਖਾਲੀ_ਸੂਚਕ))
            return_trace (ਝੂਠ);
        }
      }
      map = &StructAfter<SegmentMaps> (*map);
    }

    ਜੇ (c->plan->has_avar2)
      return_trace (_subset_avar2 (c, new_mappings));

    return_trace (ਸੱਚ);
  }

  ਨਿਜੀ:
  ਬਣਤਰ avar2_ਸੂਚਕਾਂਕ_map_plan_t
  {
    ਤਾਰਕਿਕ init (ਅਚਲ hb_ਸਦਿਸ਼_t<uint32_t> &varidx_mapping,
	       ਅਚਲ hb_map_t &axes_ਸੂਚਕਾਂਕ_map,
	       ਬਿਨਾਨਿਸ਼ਾਨੀ axis_ਗਿਣਤੀ)
    {
      ਜੇ (!output_map.alloc (axes_ਸੂਚਕਾਂਕ_map.get_population ()))
	ਵਾਪਸ ਝੂਠ;

      ਤਾਰਕਿਕ has_no_variation = ਝੂਠ;
      ਬਿਨਾਨਿਸ਼ਾਨੀ max_outer = 0, max_inner = 0;
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < axis_ਗਿਣਤੀ; i++)
      {
	ਜੇ (!axes_ਸੂਚਕਾਂਕ_map.has (i)) ਜਾਰੀ;
	uint32_t varidx = varidx_mapping[i];
	output_map.push (varidx);
	ਜੇ (varidx == HB_OT_LAYOUT_NO_VARIATIONS_ਸੂਚਕਾਂਕ)
	{
	  has_no_variation = ਸੱਚ;
	  ਜਾਰੀ;
	}
	max_outer = hb_max (max_outer, varidx >> 16);
	max_inner = hb_max (max_inner, varidx & 0xFFFF);
      }
      ਜੇ (output_map.ਵਿੱਚ_error () ||
	  output_map.ਲੰਮਾਈ != axes_ਸੂਚਕਾਂਕ_map.get_population ())
	ਵਾਪਸ ਝੂਠ;

      ਜੇ (has_no_variation)
      {
	ਚੌੜਾਈ = 4;
	inner_ਦੋਆਧਾਰੀ_ਅੰਕ_ਗਿਣਤੀ = 16;
      }
      ਨਹੀਂਤਾਂ
      {
	inner_ਦੋਆਧਾਰੀ_ਅੰਕ_ਗਿਣਤੀ = hb_max (1u, hb_ਦੋਆਧਾਰੀ_ਅੰਕ_storage (max_inner));
	ਬਿਨਾਨਿਸ਼ਾਨੀ outer_ਦੋਆਧਾਰੀ_ਅੰਕ_ਗਿਣਤੀ = hb_max (1u, hb_ਦੋਆਧਾਰੀ_ਅੰਕ_storage (max_outer));
	ਚੌੜਾਈ = hb_clamp ((inner_ਦੋਆਧਾਰੀ_ਅੰਕ_ਗਿਣਤੀ + outer_ਦੋਆਧਾਰੀ_ਅੰਕ_ਗਿਣਤੀ + 7) / 8, 1u, 4u);
	ਜੇ (inner_ਦੋਆਧਾਰੀ_ਅੰਕ_ਗਿਣਤੀ + outer_ਦੋਆਧਾਰੀ_ਅੰਕ_ਗਿਣਤੀ > ਚੌੜਾਈ * 8)
	  inner_ਦੋਆਧਾਰੀ_ਅੰਕ_ਗਿਣਤੀ = ਚੌੜਾਈ * 8 - outer_ਦੋਆਧਾਰੀ_ਅੰਕ_ਗਿਣਤੀ;
      }
      ਵਾਪਸ ਸੱਚ;
    }

    ਬਿਨਾਨਿਸ਼ਾਨੀ get_inner_ਦੋਆਧਾਰੀ_ਅੰਕ_ਗਿਣਤੀ () ਅਚਲ { ਵਾਪਸ inner_ਦੋਆਧਾਰੀ_ਅੰਕ_ਗਿਣਤੀ; }
    ਬਿਨਾਨਿਸ਼ਾਨੀ get_ਚੌੜਾਈ () ਅਚਲ { ਵਾਪਸ ਚੌੜਾਈ; }
    hb_array_t<ਅਚਲ uint32_t> get_output_map () ਅਚਲ { ਵਾਪਸ output_map.as_array (); }

    ਬਿਨਾਨਿਸ਼ਾਨੀ inner_ਦੋਆਧਾਰੀ_ਅੰਕ_ਗਿਣਤੀ = 1;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਚੌੜਾਈ = 1;
    hb_ਸਦਿਸ਼_t<uint32_t> output_map;
  };

  ਤਾਰਕਿਕ _subset_avar2 (hb_subset_context_t *c,
                      ਅਚਲ hb_ਸਦਿਸ਼_t<hb_ਸਦਿਸ਼_t<Axis_ਮੁੱਲ_Map>> &new_mappings) ਅਚਲ
  {
#ਜੇ defined (HB_NO_VAR) || defined (HB_NO_AVAR2)
    /* Not reachable: the plan never sets has_avar2 in these configurations. */
    ਵਾਪਸ ਝੂਠ;
#ਨਹੀਂਤਾਂ

    /* 1. Locate original avar2 data, keeping per-axis old segment maps */
    hb_ਸਦਿਸ਼_t<ਅਚਲ SegmentMaps *> old_seg_maps;
    ਜੇ (!old_seg_maps.alloc (axis_ਗਿਣਤੀ_2)) ਵਾਪਸ ਝੂਠ;
    ਅਚਲ SegmentMaps *map = &ਪਹਿਲਾ_Axis_Segment_Maps;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < axis_ਗਿਣਤੀ_2; i++)
    {
      old_seg_maps.push (map);
      map = &StructAfter<SegmentMaps> (*map);
    }

    ਅਚਲ ਸਵੈਚਾਲਿਤ &v2 = * (ਅਚਲ avar_V2_ਪੂਛ *) map;
    ਅਚਲ ਸਵੈਚਾਲਿਤ &varidx_map = ਇਹ+v2.varIdxMap;
    ਅਚਲ ਸਵੈਚਾਲਿਤ &var_store = ਇਹ+v2.varStore;

    ਸਵੈਚਾਲਿਤ fvar_axes = c->plan->ਸਰੋਤ->ਸਾਰਣੀ.fvar->get_axes ();

    /* 2. Compute default deltas by evaluating VarStore at old defaults */
    hb_ਸਦਿਸ਼_t<ਪੂਰਨ> default_coords;
    ਜੇ (!default_coords.resize (axis_ਗਿਣਤੀ_2)) ਵਾਪਸ ਝੂਠ;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < axis_ਗਿਣਤੀ_2; i++)
    {
      hb_ਟੈਗ_t *axis_ਟੈਗ_2;
      ਜੇ (c->plan->axes_old_ਸੂਚਕਾਂਕ_ਟੈਗ_map.has (i, &axis_ਟੈਗ_2) &&
          c->plan->old_intermediates.has (*axis_ਟੈਗ_2))
      {
        ਦਸ਼ਮਲਵ d_i = (ਦਸ਼ਮਲਵ) c->plan->old_intermediates.get (*axis_ਟੈਗ_2).ਵਿਚਕਾਰ;
        default_coords[i] = roundf (d_i * 16384.f);
      }
      ਨਹੀਂਤਾਂ
        default_coords[i] = 0;
    }

    ਸਵੈਚਾਲਿਤ *store_cache = var_store.create_cache ();
    hb_ਸਦਿਸ਼_t<ਦਸ਼ਮਲਵ> default_deltas;
    ਜੇ (!default_deltas.resize (axis_ਗਿਣਤੀ_2)) {
      ਵਸਤੂ_Variation_Store::destroy_cache (store_cache);
      ਵਾਪਸ ਝੂਠ;
    }
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < axis_ਗਿਣਤੀ_2; i++)
    {
      uint32_t varidx = varidx_map.map (i);
      ਜੇ (varidx == HB_OT_LAYOUT_NO_VARIATIONS_ਸੂਚਕਾਂਕ)
        default_deltas[i] = 0.f;
      ਨਹੀਂਤਾਂ
        default_deltas[i] = var_store.get_delta (varidx, default_coords.arrayZ,
                                                  default_coords.ਲੰਮਾਈ, store_cache);
    }
    ਵਸਤੂ_Variation_Store::destroy_cache (store_cache);

    /* 3. Rebase IVS regions */
    ਵਸਤੂ_variations_t ਵਸਤੂ_vars;
    ਜੇ (!ਵਸਤੂ_vars.create_from_ਵਸਤੂ_varstore (var_store, c->plan->axes_old_ਸੂਚਕਾਂਕ_ਟੈਗ_map))
      ਵਾਪਸ ਝੂਠ;
    ਜੇ (!ਵਸਤੂ_vars.instantiate_tuple_vars (c->plan->old_intermediates,
                                           c->plan->axes_triple_distances,
                                           ਝੂਠ))
      ਵਾਪਸ ਝੂਠ;

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
    hb_ਸਦਿਸ਼_t<uint32_t> new_varidx_mapping;
    ਜੇ (!new_varidx_mapping.resize (axis_ਗਿਣਤੀ_2)) ਵਾਪਸ ਝੂਠ;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < axis_ਗਿਣਤੀ_2; i++)
    {
      uint32_t varidx = varidx_map.map (i);
      ਜੇ (varidx != HB_OT_LAYOUT_NO_VARIATIONS_ਸੂਚਕਾਂਕ &&
	  !var_store.has_delta_ਸੈੱਟ (varidx))
	varidx = HB_OT_LAYOUT_NO_VARIATIONS_ਸੂਚਕਾਂਕ;
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
    hb_hashmap_t<uint32_t, ਬਿਨਾਨਿਸ਼ਾਨੀ> varidx_ref_ਗਿਣਤੀ;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < axis_ਗਿਣਤੀ_2; i++)
    {
      ਜੇ (!c->plan->axes_ਸੂਚਕਾਂਕ_map.has (i)) ਜਾਰੀ; /* self-contained: dropped */
      uint32_t varidx = new_varidx_mapping[i];
      ਜੇ (varidx == HB_OT_LAYOUT_NO_VARIATIONS_ਸੂਚਕਾਂਕ) ਜਾਰੀ;
      ਬਿਨਾਨਿਸ਼ਾਨੀ *ਗਿਣਤੀ_2;
      ਜੇ (varidx_ref_ਗਿਣਤੀ.has (varidx, &ਗਿਣਤੀ_2))
        (*ਗਿਣਤੀ_2)++;
      ਨਹੀਂਤਾਂ ਜੇ (!varidx_ref_ਗਿਣਤੀ.ਸੈੱਟ (varidx, 1))
        ਵਾਪਸ ਝੂਠ;
    }
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < axis_ਗਿਣਤੀ_2; i++)
    {
      hb_ਟੈਗ_t *axis_ਟੈਗ_ptr;
      ਜੇ (!c->plan->axes_old_ਸੂਚਕਾਂਕ_ਟੈਗ_map.has (i, &axis_ਟੈਗ_ptr))
        ਵਾਪਸ ਝੂਠ;
      /* Only axes that will receive offset compensation (restricted or
       * pinned) can contaminate a shared row. */
      ਜੇ (!c->plan->axes_ਸੂਚਕਾਂਕ_map.has (i) ||
          !c->plan->user_axes_location.has (*axis_ਟੈਗ_ptr))
        ਜਾਰੀ;
      uint32_t varidx = new_varidx_mapping[i];
      ਜੇ (varidx == HB_OT_LAYOUT_NO_VARIATIONS_ਸੂਚਕਾਂਕ)
        ਜਾਰੀ; /* gets a fresh, private VarData in the offset loop below */
      ਬਿਨਾਨਿਸ਼ਾਨੀ *ਗਿਣਤੀ_2;
      ਜੇ (!varidx_ref_ਗਿਣਤੀ.has (varidx, &ਗਿਣਤੀ_2) || *ਗਿਣਤੀ_2 <= 1)
        ਜਾਰੀ; /* sole owner: safe to write offsets in place */
      ਬਿਨਾਨਿਸ਼ਾਨੀ outer = varidx >> 16;
      ਬਿਨਾਨਿਸ਼ਾਨੀ new_inner = ਵਸਤੂ_vars.duplicate_ਪੰਕਤੀ (outer, varidx & 0xFFFF);
      ਜੇ (unlikely (new_inner == (ਬਿਨਾਨਿਸ਼ਾਨੀ) -1)) ਵਾਪਸ ਝੂਠ;
      new_varidx_mapping[i] = (outer << 16) | new_inner;
      (*ਗਿਣਤੀ_2)--;
    }

    /* 6. Add offset compensation tuples.
     * Track processed (outer,inner) pairs to avoid adding duplicate biases
     * when multiple axes share the same varIdx. */
    hb_ਸੈੱਟ_t processed_varidxes;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < axis_ਗਿਣਤੀ_2; i++)
    {
      hb_ਟੈਗ_t *axis_ਟੈਗ_ptr;
      ਜੇ (!c->plan->axes_old_ਸੂਚਕਾਂਕ_ਟੈਗ_map.has (i, &axis_ਟੈਗ_ptr))
        ਵਾਪਸ ਝੂਠ;
      hb_ਟੈਗ_t axis_ਟੈਗ_2 = *axis_ਟੈਗ_ptr;

      /* Self-contained pinned axes are removed from fvar/avar; their
       * contribution is baked into the variation tables instead. */
      ਜੇ (!c->plan->axes_ਸੂਚਕਾਂਕ_map.has (i))
        ਜਾਰੀ;

      Triple *new_user;
      ਜੇ (c->plan->user_axes_location.has (axis_ਟੈਗ_2, &new_user))
      {
        /* This axis is being restricted or pinned */
        Triple *old_int;
        ਜੇ (!c->plan->old_intermediates.has (axis_ਟੈਗ_2, &old_int))
          ਵਾਪਸ ਝੂਠ;

        ਦਸ਼ਮਲਵ a_i = (ਦਸ਼ਮਲਵ) old_int->minimum;
        ਦਸ਼ਮਲਵ d_i = (ਦਸ਼ਮਲਵ) old_int->ਵਿਚਕਾਰ;
        ਦਸ਼ਮਲਵ b_i = (ਦਸ਼ਮਲਵ) old_int->ਵੱਧ_ਤੋਂ_ਵੱਧ;

        ਪੂਰਨ d_int = roundf (d_i * 16384.f);

        ਤਾਰਕਿਕ is_pinned = new_user->is_ਬਿੰਦੂ ();

        uint32_t varidx = new_varidx_mapping[i];
        ਬਿਨਾਨਿਸ਼ਾਨੀ outer, inner, ਵਸਤੂ_ਗਿਣਤੀ_2;

        ਜੇ (varidx == HB_OT_LAYOUT_NO_VARIATIONS_ਸੂਚਕਾਂਕ)
        {
          /* No existing avar2 mapping. Create new VarData. */
          outer = ਵਸਤੂ_vars.ਜੋੜੋ_vardata (1);
          inner = 0;
          ਵਸਤੂ_ਗਿਣਤੀ_2 = 1;
          new_varidx_mapping[i] = (outer << 16) | inner;
          default_deltas[i] = 0.f; /* no prior default delta */
        }
        ਨਹੀਂਤਾਂ
        {
          outer = varidx >> 16;
          inner = varidx & 0xFFFF;
          ਵਸਤੂ_ਗਿਣਤੀ_2 = ਵਸਤੂ_vars.get_ਵਸਤੂ_ਗਿਣਤੀ (outer);
        }

        /* Empty-region bias: d_int + round(defaultDelta) */
        ਪੂਰਨ bias = d_int + (ਪੂਰਨ) roundf (default_deltas[i]);
        ਜੇ (bias != 0)
        {
          hb_hashmap_t<hb_ਟੈਗ_t, Triple> ਖਾਲੀ_ਥਾਵਾਂ_region;
          ਵਸਤੂ_vars.ਜੋੜੋ_tuple (outer, std::move (ਖਾਲੀ_ਥਾਵਾਂ_region),
                               inner, bias, ਵਸਤੂ_ਗਿਣਤੀ_2);
        }

        ਜੇ (!is_pinned)
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
          hb_ਸਦਿਸ਼_t<avar2_ਵਿੱਥ_knot_t> knots;
          avar2_ਜੋੜੋ_knot (knots, -1.0, (ਦੁੱਗਣਾ) a_i + 1.0);
          avar2_ਜੋੜੋ_knot (knots, 0.0, (ਦੁੱਗਣਾ) d_i);
          avar2_ਜੋੜੋ_knot (knots, 1.0, (ਦੁੱਗਣਾ) b_i - 1.0);

          ਅਚਲ hb_ਸਦਿਸ਼_t<Axis_ਮੁੱਲ_Map> &new_mapping = new_mappings[i];

          ਦਸ਼ਮਲਵ min_f = 0.f, def_f = 0.f, max_f = 0.f;
          ਜੇ (likely (i < fvar_axes.ਲੰਮਾਈ))
            fvar_axes[i].get_coordinates (min_f, def_f, max_f);
          ਦੁੱਗਣਾ old_min = (ਦੁੱਗਣਾ) min_f;
          ਦੁੱਗਣਾ old_def = (ਦੁੱਗਣਾ) def_f;
          ਦੁੱਗਣਾ old_max = (ਦੁੱਗਣਾ) max_f;

          /* If the axis default MOVED, inv_renorm also kinks where the OLD
           * default lands in the new space (the old intermediate coordinate
           * crosses 0 there), at
           *   z = z_old  ->  -z_old
           * Omitting that knot (as a plain two-tent encoding would) makes
           * interior coordinates wrong. */
          ਦੁੱਗਣਾ z_old = avar2_normalize_ਮੁੱਲ ((ਦੁੱਗਣਾ) old_def,
                                                 (ਦੁੱਗਣਾ) new_user->minimum,
                                                 (ਦੁੱਗਣਾ) new_user->ਵਿਚਕਾਰ,
                                                 (ਦੁੱਗਣਾ) new_user->ਵੱਧ_ਤੋਂ_ਵੱਧ);
          z_old = _avar2_map_new_mapping (new_mapping, z_old);
          z_old = (ਦੁੱਗਣਾ) roundf ((ਦਸ਼ਮਲਵ) (z_old * 16384.0)) / 16384.0;
          ਜੇ (-1.0 < z_old && z_old < 1.0)
            avar2_ਜੋੜੋ_knot (knots, z_old, -z_old);

          /* Interior avar v1 breakpoints inside the retained range each put
           * a kink in offset(z). Sampling only {-1, 0, +1, z_old} would
           * linearly interpolate across those kinks. The instantiated
           * mapping keeps exactly the in-range old breakpoints, and the new
           * mapping kinks at each one's output coordinate; add that z with
           * its old intermediate value so offset(z) is reproduced at every
           * kink. */
          hb_ਸਦਿਸ਼_t<avar2_ਵਿੱਥ_knot_t> with_breakpoints (knots);
          ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ &m : new_mapping)
          {
            ਦੁੱਗਣਾ from = (ਦੁੱਗਣਾ) m.coords[0].ਮੰਜ਼ਿਲ_float ();
            ਜੇ (from == -1.0 || from == 0.0 || from == 1.0)
              ਜਾਰੀ; /* anchors already seeded */
            ਦੁੱਗਣਾ z = (ਦੁੱਗਣਾ) m.coords[1].ਮੰਜ਼ਿਲ_float ();
            ਜੇ (!(-1.0 < z && z < 1.0))
              ਜਾਰੀ;
            ਦੁੱਗਣਾ user = avar2_denormalize_ਮੁੱਲ (from,
                                                    (ਦੁੱਗਣਾ) new_user->minimum,
                                                    (ਦੁੱਗਣਾ) new_user->ਵਿਚਕਾਰ,
                                                    (ਦੁੱਗਣਾ) new_user->ਵੱਧ_ਤੋਂ_ਵੱਧ);
            ਦੁੱਗਣਾ n_old = avar2_normalize_ਮੁੱਲ (user, old_min, old_def, old_max);
            ਦੁੱਗਣਾ x_old = (ਦੁੱਗਣਾ) old_seg_maps[i]->map_float ((ਦਸ਼ਮਲਵ) n_old);
            x_old = (ਦੁੱਗਣਾ) roundf ((ਦਸ਼ਮਲਵ) (x_old * 16384.0)) / 16384.0;
            avar2_ਜੋੜੋ_knot (with_breakpoints, z, x_old - z);
          }

          ਜੇ (unlikely (knots.ਵਿੱਚ_error () || with_breakpoints.ਵਿੱਚ_error ()))
            ਵਾਪਸ ਝੂਠ;

          /* Extra tents cost F2Dot14 rounding, so for a steep segment they
           * can add more quantization noise than the structural error they
           * remove. Keep the interior breakpoints only when they do not
           * increase the estimated residual; this makes the collection a
           * strict (never-worse) improvement over the {-1, 0, +1, z_old}
           * anchors. Warn when even the better choice is not bit-exact (a
           * steep retained segment that cannot be reproduced in F2Dot14). */
          ਬਿਨਾਨਿਸ਼ਾਨੀ err = avar2_estimate_ਵਿੱਥ_error (*old_seg_maps[i], new_mapping,
                                                       old_min, old_def, old_max,
                                                       (ਦੁੱਗਣਾ) new_user->minimum,
                                                       (ਦੁੱਗਣਾ) new_user->ਵਿਚਕਾਰ,
                                                       (ਦੁੱਗਣਾ) new_user->ਵੱਧ_ਤੋਂ_ਵੱਧ,
                                                       knots);
          ਜੇ (with_breakpoints.ਲੰਮਾਈ != knots.ਲੰਮਾਈ)
          {
            ਬਿਨਾਨਿਸ਼ਾਨੀ err_with = avar2_estimate_ਵਿੱਥ_error (*old_seg_maps[i], new_mapping,
                                                              old_min, old_def, old_max,
                                                              (ਦੁੱਗਣਾ) new_user->minimum,
                                                              (ਦੁੱਗਣਾ) new_user->ਵਿਚਕਾਰ,
                                                              (ਦੁੱਗਣਾ) new_user->ਵੱਧ_ਤੋਂ_ਵੱਧ,
                                                              with_breakpoints);
            ਜੇ (err_with <= err)
            {
              knots = std::move (with_breakpoints);
              err = err_with;
            }
          }
          ਜੇ (err > 8)
            DEBUG_ਸੁਨੇਹਾ (SUBSET, ਖਾਲੀ_ਸੂਚਕ,
                       "avar2 offset compensation is approximate for axis %c%c%c%c: "
                       "max residual %u F2Dot14 units",
                       HB_UNTAG (axis_ਟੈਗ_2), err);

          /* Synthesize tents. Adjacent tents evaluate to zero at each
           * other's peaks, so each knot's delta is offset(z) - offset(0);
           * the base value offset(0) = d_i is carried by the empty-region
           * bias above. This reduces to the classic pair of tents
           * (-1,-1,0) / (0,+1,+1) when the default is unchanged and there
           * are no interior knots. */
          ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ k = 0; k < knots.ਲੰਮਾਈ; k++)
          {
            ਦੁੱਗਣਾ z = knots.arrayZ[k].z;
            ਜੇ (z == 0.0) ਜਾਰੀ;
            ਪੂਰਨ delta = (ਪੂਰਨ) roundf ((ਦਸ਼ਮਲਵ) ((knots.arrayZ[k].ਵਿੱਥ - (ਦੁੱਗਣਾ) d_i) * 16384.0));
            ਜੇ (!delta) ਜਾਰੀ;
            ਦੁੱਗਣਾ ਹੇਠਲਾ, ਉੱਤਲਾ;
            ਜੇ (z > 0.0)
            {
              ਹੇਠਲਾ = knots.arrayZ[k - 1].z;
              ਉੱਤਲਾ = k + 1 < knots.ਲੰਮਾਈ ? knots.arrayZ[k + 1].z : z;
            }
            ਨਹੀਂਤਾਂ
            {
              ਉੱਤਲਾ = knots.arrayZ[k + 1].z;
              ਹੇਠਲਾ = k > 0 ? knots.arrayZ[k - 1].z : z;
            }
            hb_hashmap_t<hb_ਟੈਗ_t, Triple> region;
            ਜੇ (unlikely (!region.ਸੈੱਟ (axis_ਟੈਗ_2, Triple (ਹੇਠਲਾ, z, ਉੱਤਲਾ))))
              ਵਾਪਸ ਝੂਠ;
            ਵਸਤੂ_vars.ਜੋੜੋ_tuple (outer, std::move (region),
                                 inner, delta, ਵਸਤੂ_ਗਿਣਤੀ_2);
          }
        }
      }
      ਨਹੀਂਤਾਂ
      {
        /* Free or private axis — not being restricted.
         * If it has a non-zero default delta, add it back as a bias.
         * Skip if this (outer,inner) was already processed (shared varIdx). */
        uint32_t varidx = new_varidx_mapping[i];
        ਜੇ (varidx == HB_OT_LAYOUT_NO_VARIATIONS_ਸੂਚਕਾਂਕ)
          ਜਾਰੀ;

        ਜੇ (processed_varidxes.has (varidx))
          ਜਾਰੀ;
        processed_varidxes.ਜੋੜੋ (varidx);

        ਬਿਨਾਨਿਸ਼ਾਨੀ outer = varidx >> 16;
        ਬਿਨਾਨਿਸ਼ਾਨੀ inner = varidx & 0xFFFF;
        ਪੂਰਨ dd = (ਪੂਰਨ) roundf (default_deltas[i]);
        ਜੇ (dd != 0)
        {
          ਬਿਨਾਨਿਸ਼ਾਨੀ ਵਸਤੂ_ਗਿਣਤੀ_2 = ਵਸਤੂ_vars.get_ਵਸਤੂ_ਗਿਣਤੀ (outer);
          hb_hashmap_t<hb_ਟੈਗ_t, Triple> ਖਾਲੀ_ਥਾਵਾਂ_region;
          ਵਸਤੂ_vars.ਜੋੜੋ_tuple (outer, std::move (ਖਾਲੀ_ਥਾਵਾਂ_region),
                               inner, dd, ਵਸਤੂ_ਗਿਣਤੀ_2);
        }
      }
    }

    /* 7. Finalize: build region list + convert to varstore */
    ਜੇ (!ਵਸਤੂ_vars.build_region_list ()) ਵਾਪਸ ਝੂਠ;
    ਜੇ (!ਵਸਤੂ_vars.as_ਵਸਤੂ_varstore (ਸੱਚ /* optimize */,
                                     ਝੂਠ /* use_no_variation_idx */))
      ਵਾਪਸ ਝੂਠ;

    /* 8. Apply varidx_map optimization remapping */
    ਅਚਲ ਸਵੈਚਾਲਿਤ &opt_varidx_map = ਵਸਤੂ_vars.get_varidx_map ();
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < axis_ਗਿਣਤੀ_2; i++)
    {
      uint32_t varidx = new_varidx_mapping[i];
      ਜੇ (varidx == HB_OT_LAYOUT_NO_VARIATIONS_ਸੂਚਕਾਂਕ)
        ਜਾਰੀ;
      uint32_t *new_idx;
      ਜੇ (opt_varidx_map.has (varidx, &new_idx))
        new_varidx_mapping[i] = *new_idx;
    }

    /* 9. Serialize avarV2Tail. Entries cover the retained axes only;
     * self-contained pinned axes are removed from fvar. */
    avar2_ਸੂਚਕਾਂਕ_map_plan_t ਸੂਚਕਾਂਕ_map_plan;
    ਜੇ (!ਸੂਚਕਾਂਕ_map_plan.init (new_varidx_mapping,
			      c->plan->axes_ਸੂਚਕਾਂਕ_map,
			      axis_ਗਿਣਤੀ_2))
      ਵਾਪਸ ਝੂਠ;

    ਸਵੈਚਾਲਿਤ *ਪੂਛ = c->serializer->allocate_ਮਾਪ<avar_V2_ਪੂਛ> (avar_V2_ਪੂਛ::static_ਮਾਪ);
    ਜੇ (unlikely (!ਪੂਛ)) ਵਾਪਸ ਝੂਠ;
    ਜੇ (!ਪੂਛ->varIdxMap.serialize_serialize (c->serializer, ਸੂਚਕਾਂਕ_map_plan))
      ਵਾਪਸ ਝੂਠ;
    ਜੇ (!ਪੂਛ->varStore.serialize_serialize (c->serializer,
					     ਵਸਤੂ_vars.has_long_ਸ਼ਬਦ (),
					     c->plan->axis_ਟੈਗ_ਸੂਚੀ,
					     ਵਸਤੂ_vars.get_region_list (),
					     ਵਸਤੂ_vars.get_vardata_encodings ()))
      ਵਾਪਸ ਝੂਠ;

    ਵਾਪਸ ਸੱਚ;
#ਜੇ_ਅੰਤ
  }

  ਜਨਤਕ:

  ਸੁਰੱਖਿਅਤ:
  Fixed_ਸੰਸਕਰਣ<>ਸੰਸਕਰਣ;	/* Version of the avar table
				 * initially set to 0x00010000u */
  HBUINT16	reserved;	/* This field is permanently reserved. Set to 0. */
  HBUINT16	axis_ਗਿਣਤੀ_2;	/* The number of variation axes in the font. This
				 * must be the same number as axisCount in the
				 * 'fvar' table. */
  SegmentMaps	ਪਹਿਲਾ_Axis_Segment_Maps;

  ਜਨਤਕ:
  DEFINE_ਮਾਪ_MIN (8);
};

} /* namespace OT */


#ਜੇ_ਅੰਤ /* HB_OT_VAR_AVAR_TABLE_HH */
