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

#ଅନ୍ତର୍ଭାଵ "hb-open-type.hh"
#ଅନ୍ତର୍ଭାଵ "hb-ot-var-common.hh"
#ଅନ୍ତର୍ଭାଵ "hb-ot-var-fvar-table.hh"


/*
 * avar -- Axis Variations
 * https://docs.microsoft.com/en-us/typography/opentype/spec/avar
 */

#define HB_OT_TAG_avar HB_TAG('a','v','a','r')


ନାମାକାଶ OT {


/* "Spec": https://github.com/be-fonts/boring-expansion-spec/issues/14 */
ସଂରଚନା avarV2Tail
{
  friend ସଂରଚନା avar;

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c,
		 ଅଚଳ ଶୂନ୍ୟ *base) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (varIdxMap.sanitize (c, base) &&
		  varStore.sanitize (c, base));
  }

  ସୁରକ୍ଷିତ:
  Offset32To<DeltaSetIndexMap>	varIdxMap;	/* Offset from the beginning of 'avar' table. */
  Offset32To<ItemVariationStore>	varStore;	/* Offset from the beginning of 'avar' table. */

  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (8);
};


ସଂରଚନା Axis_ମୂଲ୍ୟ_Map
{
  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି));
  }

  ଶୂନ୍ୟ set_mapping (ଦଶମିକ from_coord, ଦଶମିକ to_coord)
  {
    coords[0].set_float (from_coord);
    coords[1].set_float (to_coord);
  }

  ତାର୍କିକ is_outside_axis_range (ଅଚଳ Triple& axis_range) ଅଚଳ
  {
    ଦ୍ୱିଗୁଣ from_coord = (ଦ୍ୱିଗୁଣ) coords[0].to_float ();
    ଫେରନ୍ତୁ !axis_range.contains (from_coord);
  }

  ତାର୍କିକ must_include () ଅଚଳ
  {
    ଦଶମିକ from_coord = coords[0].to_float ();
    ଦଶମିକ to_coord = coords[1].to_float ();
    ଫେରନ୍ତୁ (from_coord == -1.f && to_coord == -1.f) ||
           (from_coord == 0.f && to_coord == 0.f) ||
           (from_coord == 1.f && to_coord == 1.f);
  }

  ଶୂନ୍ୟ instantiate (ଅଚଳ Triple& axis_range,
                    ଅଚଳ Triple& unmapped_range,
                    ଅଚଳ TripleDistances& triple_distances)
  {
    ଦଶମିକ from_coord = coords[0].to_float ();
    ଦଶମିକ to_coord = coords[1].to_float ();

    from_coord = renormalize_ମୂଲ୍ୟ ((ଦ୍ୱିଗୁଣ) from_coord, unmapped_range, triple_distances);
    to_coord = renormalize_ମୂଲ୍ୟ ((ଦ୍ୱିଗୁଣ) to_coord, axis_range, triple_distances);

    /* Malformed input can renormalize outside [-1,1], which would
     * overflow F2DOT14. */
    from_coord = hb_clamp (from_coord, -1.f, 1.f);
    to_coord = hb_clamp (to_coord, -1.f, 1.f);

    coords[0].set_float (from_coord);
    coords[1].set_float (to_coord);
  }

  HB_INTERNAL ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmp (ଅଚଳ ଶୂନ୍ୟ *pa, ଅଚଳ ଶୂନ୍ୟ *pb)
  {
    ଅଚଳ Axis_ମୂଲ୍ୟ_Map *a = (ଅଚଳ Axis_ମୂଲ୍ୟ_Map *) pa;
    ଅଚଳ Axis_ମୂଲ୍ୟ_Map *b = (ଅଚଳ Axis_ମୂଲ୍ୟ_Map *) pb;

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା a_from = a->coords[0].to_int ();
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା b_from = b->coords[0].to_int ();
    ଯଦି (a_from != b_from)
      ଫେରନ୍ତୁ a_from - b_from;

    /* this should never be reached. according to the spec, all of the axis
     * value map records for a given axis must have different fromCoord values
     * */
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା a_to = a->coords[1].to_int ();
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା b_to = b->coords[1].to_int ();
    ଫେରନ୍ତୁ a_to - b_to;
  }

  ତାର୍କିକ serialize (hb_serialize_context_t *c) ଅଚଳ
  {
    TRACE_SERIALIZE (ଏହି);
    return_trace (c->embed (ଏହି));
  }

  ସର୍ବସାଧାରଣ:
  F2DOT14	coords[2];
//   F2DOT14	fromCoord;	/* A normalized coordinate value obtained using
//				 * default normalization. */
//   F2DOT14	toCoord;	/* The modified, normalized coordinate value. */

  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (4);
};

ସଂରଚନା SegmentMaps : Array16Of<Axis_ମୂଲ୍ୟ_Map>
{
  ଦଶମିକ map_float (ଦଶମିକ value, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା from_offset = 0, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା to_offset = 1) ଅଚଳ
  {
#define fromCoord coords[from_offset].to_float ()
#define toCoord coords[to_offset].to_float ()

    ଅଚଳ auto *map = arrayZ;

    /* The following special-cases are not part of OpenType, which requires
     * that at least -1, 0, and +1 must be mapped. But we include these as
     * part of a better error recovery scheme. */
    ଯଦି (len < 2)
    {
      ଯଦି (!len)
	ଫେରନ୍ତୁ value;
      ଅନ୍ୟଥା /* len == 1*/
	ଫେରନ୍ତୁ value - map[0].fromCoord + map[0].toCoord;
    }

    // At least two mappings now.

    /* CoreText is wild...
     * PingFangUI avar needs all this special-casing...
     * So we implement an extended version of the spec here,
     * which is more robust and more likely to be compatible with
     * the wild. */

    unsigned start = 0;
    unsigned end = len;
    ଯଦି (map[start].fromCoord == -1 && map[start].toCoord == -1 && map[start+1].fromCoord == -1)
      start++;
    ଯଦି (map[end-1].fromCoord == +1 && map[end-1].toCoord == +1 && map[end-2].fromCoord == +1)
      end--;

    /* Look for exact match first, and do lots of special-casing. */
    unsigned i;
    ପାଇଁ (i = start; i < end; i++)
      ଯଦି (value == map[i].fromCoord)
	ଭାଙ୍ଗ;
    ଯଦି (i < end)
    {
      // There's at least one exact match. See if there are more.
      unsigned j = i;
      ପାଇଁ (; j + 1 < end; j++)
	ଯଦି (value != map[j + 1].fromCoord)
	  ଭାଙ୍ଗ;

      // [i,j] inclusive are all exact matches:

      // If there's only one, return it. This is the only spec-compliant case.
      ଯଦି (i == j)
	ଫେରନ୍ତୁ map[i].toCoord;
      // If there's exactly three, return the middle one.
      ଯଦି (i + 2 == j)
	ଫେରନ୍ତୁ map[i + 1].toCoord;

      // Ignore the middle ones. Return the one mapping closer to 0.
      ଯଦି (value < 0) ଫେରନ୍ତୁ map[j].toCoord;
      ଯଦି (value > 0) ଫେରନ୍ତୁ map[i].toCoord;

      // Mapping 0? CoreText seems confused. It seems to prefer 0 here...
      // So we'll just return the smallest one. lol
      ଫେରନ୍ତୁ fabsf (map[i].toCoord) < fabsf (map[j].toCoord) ? map[i].toCoord : map[j].toCoord;

      // Mapping 0? Return one not mapping to 0.
      ଯଦି (map[i].toCoord == 0)
	ଫେରନ୍ତୁ map[j].toCoord;
      ଅନ୍ୟଥା
	ଫେରନ୍ତୁ map[i].toCoord;
    }

    /* There's at least two and we're not an exact match. Prepare to lerp. */

    // Find the segment we're in.
    ପାଇଁ (i = start; i < end; i++)
      ଯଦି (value < map[i].fromCoord)
	ଭାଙ୍ଗ;

    ଯଦି (i == start)
    {
      // Value before all segments; Shift.
      ଫେରନ୍ତୁ value - map[start].fromCoord + map[start].toCoord;
    }
    ଯଦି (i == end)
    {
      // Value after all segments; Shift.
      ଫେରନ୍ତୁ value - map[end - 1].fromCoord + map[end - 1].toCoord;
    }

    // Actually interpolate.
    auto &before = map[i-1];
    auto &after = map[i];
    ଦଶମିକ denom = after.fromCoord - before.fromCoord; // Can't be zero by now.
    ଫେରନ୍ତୁ before.toCoord + ((after.toCoord - before.toCoord) * (value - before.fromCoord)) / denom;

#undef toCoord
#undef fromCoord
  }

  ଦଶମିକ unmap_float (ଦଶମିକ value) ଅଚଳ { ଫେରନ୍ତୁ map_float (value, 1, 0); }


  // TODO Kill this.
  Triple unmap_axis_range (ଅଚଳ Triple& axis_range) ଅଚଳ
  {
    ଦଶମିକ unmapped_min = unmap_float (axis_range.minimum);
    ଦଶମିକ unmapped_ମଧ୍ୟ = unmap_float (axis_range.ମଧ୍ୟ);
    ଦଶମିକ unmapped_max = unmap_float (axis_range.maximum);

    ଫେରନ୍ତୁ Triple{unmapped_min, unmapped_ମଧ୍ୟ, unmapped_max};
  }

  ତାର୍କିକ subset (hb_subset_context_t *c, hb_tag_t axis_tag,
               hb_vector_t<Axis_ମୂଲ୍ୟ_Map> *out_mappings = nullptr) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);

    /* This function cannot work on avar2 table (and currently doesn't).
     * We should instead keep the design coords in the shape plan and use
     * those. unmap_axis_range needs to be killed. */

    /* avar mapped normalized axis range. Under avar2, axes_location holds
     * only the self-contained pins for the other tables; avar itself uses
     * the intermediate-space ranges of all restricted axes. */
    ଅଚଳ auto &axes_location = c->plan->has_avar2
				? c->plan->old_intermediates
				: c->plan->axes_location;
    Triple *axis_range;
    ଯଦି (!axes_location.has (axis_tag, &axis_range))
      ଫେରନ୍ତୁ c->serializer->embed (*ଏହି);

    TripleDistances *axis_triple_distances;
    ଯଦି (!c->plan->axes_triple_distances.has (axis_tag, &axis_triple_distances))
      return_trace (ମିଥ୍ୟା);

    auto *out = c->serializer->start_embed (ଏହି);
    ଯଦି (unlikely (!c->serializer->extend_min (out))) return_trace (ମିଥ୍ୟା);

    Triple unmapped_range = unmap_axis_range (*axis_range);

    /* create a vector of retained mappings and sort */
    hb_vector_t<Axis_ମୂଲ୍ୟ_Map> ମୂଲ୍ୟ_mappings;
    ପାଇଁ (ଅଚଳ auto& _ : as_array ())
    {
      ଯଦି (_.is_outside_axis_range (unmapped_range))
        ଜାରି;
      Axis_ମୂଲ୍ୟ_Map mapping;
      mapping = _;
      mapping.instantiate (*axis_range, unmapped_range, *axis_triple_distances);
      /* (-1, -1), (0, 0), (1, 1) mappings will be added later, so avoid
       * duplicates here */
      ଯଦି (mapping.must_include ())
        ଜାରି;
      ମୂଲ୍ୟ_mappings.push (mapping);
    }

    Axis_ମୂଲ୍ୟ_Map m;
    m.set_mapping (-1.f, -1.f);
    ମୂଲ୍ୟ_mappings.push (m);

    m.set_mapping (0.f, 0.f);
    ମୂଲ୍ୟ_mappings.push (m);

    m.set_mapping (1.f, 1.f);
    ମୂଲ୍ୟ_mappings.push (m);

    ମୂଲ୍ୟ_mappings.qsort ();

    ଯଦି (unlikely (ମୂଲ୍ୟ_mappings.in_error ()))
      return_trace (ମିଥ୍ୟା);

    ପାଇଁ (ଅଚଳ auto& _ : ମୂଲ୍ୟ_mappings)
    {
      ଯଦି (!_.serialize (c->serializer))
        return_trace (ମିଥ୍ୟା);
    }
    ଯଦି (!c->serializer->check_assign (out->len, ମୂଲ୍ୟ_mappings.length, HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (ମିଥ୍ୟା);
    /* Hand the instantiated mappings to the caller; avar2 offset
     * compensation needs them to locate the new mapping's kinks. */
    ଯଦି (out_mappings)
      *out_mappings = std::move (ମୂଲ୍ୟ_mappings);
    return_trace (ସତ୍ୟ);
  }

  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_ARRAY (2, *ଏହି);
};

/* One knot of the avar2 offset-compensation function
 * offset(z) = inv_renorm(z) - z, in new intermediate space. */
ସଂରଚନା avar2_offset_knot_t
{
  ଦ୍ୱିଗୁଣ z;
  ଦ୍ୱିଗୁଣ offset;
};

/* Insert a knot keeping the vector sorted by z; first insertion wins on
 * duplicate z. */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
_avar2_add_knot (hb_vector_t<avar2_offset_knot_t> &knots, ଦ୍ୱିଗୁଣ z, ଦ୍ୱିଗୁଣ offset)
{
  unsigned i = 0;
  ଯେତେବେଳେ (i < knots.length && knots.arrayZ[i].z < z) i++;
  ଯଦି (i < knots.length && knots.arrayZ[i].z == z) ଫେରନ୍ତୁ;
  knots.push (avar2_offset_knot_t {0.0, 0.0});
  ଯଦି (unlikely (knots.in_error ())) ଫେରନ୍ତୁ;
  ପାଇଁ (unsigned j = knots.length - 1; j > i; j--)
    knots.arrayZ[j] = knots.arrayZ[j - 1];
  knots.arrayZ[i] = avar2_offset_knot_t {z, offset};
}

/* Piecewise-linear evaluation over the sorted knots. Inputs are always
 * within [-1, +1] and anchor knots at -1/0/+1 always exist. */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଦ୍ୱିଗୁଣ
_avar2_eval_offset (ଅଚଳ hb_vector_t<avar2_offset_knot_t> &knots, ଦ୍ୱିଗୁଣ z)
{
  unsigned len = knots.length;
  ପାଇଁ (unsigned i = 0; i < len; i++)
  {
    ଯଦି (z == knots.arrayZ[i].z) ଫେରନ୍ତୁ knots.arrayZ[i].offset;
    ଯଦି (z < knots.arrayZ[i].z)
    {
      ଯଦି (!i) ଫେରନ୍ତୁ knots.arrayZ[0].offset;
      ଅଚଳ auto &before = knots.arrayZ[i - 1];
      ଅଚଳ auto &after = knots.arrayZ[i];
      ଦ୍ୱିଗୁଣ denom = after.z - before.z;
      ଫେରନ୍ତୁ before.offset + (after.offset - before.offset) * (z - before.z) / denom;
    }
  }
  ଫେରନ୍ତୁ len ? knots.arrayZ[len - 1].offset : 0.0;
}

/* Piecewise-linear evaluation over instantiated avar v1 mappings, as
 * produced by SegmentMaps::subset (sorted, with -1/0/+1 anchors).
 * Matches SegmentMaps::map_float for such well-formed mappings. */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଦ୍ୱିଗୁଣ
_avar2_map_new_mapping (ଅଚଳ hb_vector_t<Axis_ମୂଲ୍ୟ_Map> &mappings, ଦ୍ୱିଗୁଣ v,
			unsigned from_offset = 0, unsigned to_offset = 1)
{
  unsigned len = mappings.length;
  ଯଦି (!len) ଫେରନ୍ତୁ v;

  ପାଇଁ (unsigned i = 0; i < len; i++)
  {
    ଦ୍ୱିଗୁଣ from = (ଦ୍ୱିଗୁଣ) mappings.arrayZ[i].coords[from_offset].to_float ();
    ଯଦି (v == from)
      ଫେରନ୍ତୁ (ଦ୍ୱିଗୁଣ) mappings.arrayZ[i].coords[to_offset].to_float ();
    ଯଦି (v < from)
    {
      ଦ୍ୱିଗୁଣ to = (ଦ୍ୱିଗୁଣ) mappings.arrayZ[i].coords[to_offset].to_float ();
      ଯଦି (!i) ଫେରନ୍ତୁ v - from + to;
      ଦ୍ୱିଗୁଣ prev_from = (ଦ୍ୱିଗୁଣ) mappings.arrayZ[i - 1].coords[from_offset].to_float ();
      ଦ୍ୱିଗୁଣ prev_to = (ଦ୍ୱିଗୁଣ) mappings.arrayZ[i - 1].coords[to_offset].to_float ();
      ଦ୍ୱିଗୁଣ denom = from - prev_from;
      ଯଦି (denom == 0.0) ଫେରନ୍ତୁ prev_to;
      ଫେରନ୍ତୁ prev_to + (to - prev_to) * (v - prev_from) / denom;
    }
  }
  ଫେରନ୍ତୁ v - (ଦ୍ୱିଗୁଣ) mappings.arrayZ[len - 1].coords[from_offset].to_float ()
           + (ଦ୍ୱିଗୁଣ) mappings.arrayZ[len - 1].coords[to_offset].to_float ();
}

/* Inverse of _avar2_map_new_mapping: pull an output coordinate back to a
 * preimage input coordinate. For a non-strictly-monotone mapping this picks
 * one preimage, which is fine for its only use (augmenting the error
 * estimator's sample set). */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଦ୍ୱିଗୁଣ
_avar2_unmap_new_mapping (ଅଚଳ hb_vector_t<Axis_ମୂଲ୍ୟ_Map> &mappings, ଦ୍ୱିଗୁଣ v)
{
  ଫେରନ୍ତୁ _avar2_map_new_mapping (mappings, v, 1, 0);
}

/* Plain (non-avar) fvar-style normalization of a user value against a
 * user-space (min, default, max) triple. */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଦ୍ୱିଗୁଣ
avar2_normalize_ମୂଲ୍ୟ (ଦ୍ୱିଗୁଣ v, ଦ୍ୱିଗୁଣ min, ଦ୍ୱିଗୁଣ def, ଦ୍ୱିଗୁଣ max)
{
  v = hb_clamp (v, min, max);
  ଯଦି (v == def) ଫେରନ୍ତୁ 0.0;
  ଯଦି (v < def) ଫେରନ୍ତୁ def == min ? 0.0 : (v - def) / (def - min);
  ଫେରନ୍ତୁ def == max ? 0.0 : (v - def) / (max - def);
}

/* Inverse of the above: map a normalized value back to user space. */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଦ୍ୱିଗୁଣ
avar2_denormalize_ମୂଲ୍ୟ (ଦ୍ୱିଗୁଣ v, ଦ୍ୱିଗୁଣ min, ଦ୍ୱିଗୁଣ def, ଦ୍ୱିଗୁଣ max)
{
  ଯଦି (v == 0.0) ଫେରନ୍ତୁ def;
  ଫେରନ୍ତୁ v < 0.0 ? def + v * (def - min) : def + v * (max - def);
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
ସ୍ଥିର ଧାଡ଼ିଭିତରେ unsigned
_avar2_estimate_offset_error (ଅଚଳ SegmentMaps &old_seg,
                              ଅଚଳ hb_vector_t<Axis_ମୂଲ୍ୟ_Map> &new_mapping,
                              ଦ୍ୱିଗୁଣ old_min, ଦ୍ୱିଗୁଣ old_def, ଦ୍ୱିଗୁଣ old_max,
                              ଦ୍ୱିଗୁଣ new_min, ଦ୍ୱିଗୁଣ new_def, ଦ୍ୱିଗୁଣ new_max,
                              ଅଚଳ hb_vector_t<avar2_offset_knot_t> &knots)
{
  constexpr unsigned samples = 257;
  hb_vector_t<ଦ୍ୱିଗୁଣ> us;
  ଯଦି (unlikely (!us.alloc (samples + old_seg.as_array ().length +
                           new_mapping.length + knots.length)))
    ଫେରନ୍ତୁ UINT_MAX; /* estimator only; fail towards "worse" */
  ପାଇଁ (unsigned i = 0; i < samples; i++)
    us.push (new_min + (new_max - new_min) * i / (samples - 1));
  ପାଇଁ (ଅଚଳ auto &_ : old_seg.as_array ())
    us.push (avar2_denormalize_ମୂଲ୍ୟ ((ଦ୍ୱିଗୁଣ) _.coords[0].to_float (),
                                       old_min, old_def, old_max));
  ପାଇଁ (ଅଚଳ auto &_ : new_mapping)
    us.push (avar2_denormalize_ମୂଲ୍ୟ ((ଦ୍ୱିଗୁଣ) _.coords[0].to_float (),
                                       new_min, new_def, new_max));
  ପାଇଁ (ଅଚଳ auto &knot : knots)
    us.push (avar2_denormalize_ମୂଲ୍ୟ (
               _avar2_unmap_new_mapping (new_mapping, knot.z),
               new_min, new_def, new_max));
  ଯଦି (unlikely (us.in_error ())) ଫେରନ୍ତୁ UINT_MAX;

  unsigned max_err = 0;
  ପାଇଁ (ଦ୍ୱିଗୁଣ u : us)
  {
    ଯଦି (u < new_min || u > new_max) ଜାରି;
    ଦ୍ୱିଗୁଣ n_old = avar2_normalize_ମୂଲ୍ୟ (u, old_min, old_def, old_max);
    ଦ୍ୱିଗୁଣ old_final = (ଦ୍ୱିଗୁଣ) old_seg.map_float ((ଦଶମିକ) n_old);
    ଦ୍ୱିଗୁଣ n_new = avar2_normalize_ମୂଲ୍ୟ (u, new_min, new_def, new_max);
    ଦ୍ୱିଗୁଣ z = _avar2_map_new_mapping (new_mapping, n_new);
    ଦ୍ୱିଗୁଣ new_final = z + _avar2_eval_offset (knots, z);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା err = abs ((ପୂର୍ଣ୍ଣସଂଖ୍ୟା) roundf ((ଦଶମିକ) (old_final * 16384.0)) -
                   (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) roundf ((ଦଶମିକ) (new_final * 16384.0)));
    ଯଦି ((unsigned) err > max_err) max_err = err;
  }
  ଫେରନ୍ତୁ max_err;
}

ସଂରଚନା avar
{
  ସ୍ଥିର constexpr hb_tag_t tableTag = HB_OT_TAG_avar;

  ତାର୍କିକ has_data () ଅଚଳ { ଫେରନ୍ତୁ version.to_int (); }

  ଅଚଳ SegmentMaps* get_segment_maps () ଅଚଳ
  { ଫେରନ୍ତୁ &firstAxisSegmentMaps; }

  unsigned get_axis_ଗଣନା () ଅଚଳ
  { ଫେରନ୍ତୁ axis_ଗଣନା_2; }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    ଯଦି (!(version.sanitize (c) &&
	  hb_barrier () &&
	  (version.major == 1
#ifndef HB_NO_AVAR2
	   || version.major == 2
#endif
	   ) &&
	  c->check_struct (ଏହି)))
      return_trace (ମିଥ୍ୟା);

    ଅଚଳ SegmentMaps *map = &firstAxisSegmentMaps;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = axis_ଗଣନା_2;
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଗଣନା_2; i++)
    {
      ଯଦି (unlikely (!map->sanitize (c)))
	return_trace (ମିଥ୍ୟା);
      map = &StructAfter<SegmentMaps> (*map);
    }

#ifndef HB_NO_AVAR2
    ଯଦି (version.major < 2)
      return_trace (ସତ୍ୟ);
    hb_barrier ();

    ଅଚଳ auto &v2 = * (ଅଚଳ avarV2Tail *) map;
    ଯଦି (unlikely (!v2.sanitize (c, ଏହି)))
      return_trace (ମିଥ୍ୟା);
#endif

    return_trace (ସତ୍ୟ);
  }

  ଶୂନ୍ୟ map_coords_16_16 (ପୂର୍ଣ୍ଣସଂଖ୍ୟା *coords, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା coords_length) ଅଚଳ
  {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = hb_min (coords_length, axis_ଗଣନା_2);

    ଅଚଳ SegmentMaps *map = &firstAxisSegmentMaps;
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଗଣନା_2; i++)
    {
      coords[i] = roundf (map->map_float (coords[i] / 65536.f) * 65536.f);
      map = &StructAfter<SegmentMaps> (*map);
    }

#ifndef HB_NO_AVAR2
    ଯଦି (version.major < 2)
      ଫେରନ୍ତୁ;
    hb_barrier ();

    ପାଇଁ (; ଗଣନା_2 < axis_ଗଣନା_2; ଗଣନା_2++)
      map = &StructAfter<SegmentMaps> (*map);

    ଅଚଳ auto &v2 = * (ଅଚଳ avarV2Tail *) map;

    ଅଚଳ auto &varidx_map = ଏହି+v2.varIdxMap;
    ଅଚଳ auto &var_store = ଏହି+v2.varStore;
    auto *var_store_cache = var_store.create_cache ();

    hb_vector_t<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> coords_2_14;
    coords_2_14.resize (coords_length);
    ପାଇଁ (unsigned i = 0; i < coords_length; i++)
      coords_2_14[i] = roundf (coords[i] / 4.f); // 16.16 -> 2.14

    hb_vector_t<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> out;
    out.alloc (coords_length);
    ପାଇଁ (unsigned i = 0; i < coords_length; i++)
    {
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା v = coords[i];
      uint32_t varidx = varidx_map.map (i);
      ଦଶମିକ delta = var_store.get_delta (varidx, coords_2_14.arrayZ, coords_2_14.length, var_store_cache);
      /* Apply the delta unclamped and clamp only the result to [-1, +1],
       * matching fontTools. Since inputs and results are in [-1, +1],
       * deltas beyond ±2 are equivalent to ±2; clamp to that range only
       * to keep the float->int conversion safe. */
      ଦଶମିକ d = hb_clamp (delta * 4, -(ଦଶମିକ) (1<<17), +(ଦଶମିକ) (1<<17)); // 2.14 -> 16.16
      v += (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) roundf (d);
      v = hb_clamp (v, -(1<<16), +(1<<16));
      out.push (v);
    }
    ପାଇଁ (unsigned i = 0; i < coords_length; i++)
      coords[i] = out[i];

    OT::ItemVariationStore::destroy_cache (var_store_cache);
#endif
  }

  /* An avar version 2 font is never downgraded to v1: even with a NULL
   * varStore offset (legal; maps like plain v1) the avar2 subsetting path
   * applies, with "no variation" semantics for rows that don't resolve —
   * offset compensation then creates delta rows on demand. */
  ତାର୍କିକ has_v2_data () ଅଚଳ { ଫେରନ୍ତୁ version.major > 1; }

  /* Resolve the avar2 VarStore and VarIdxMap, for the subset planner.
   * Either pointer may be to the Null object (nullable offsets). */
  ତାର୍କିକ get_v2_store_and_map (ଅଚଳ ItemVariationStore **store,
                             ଅଚଳ DeltaSetIndexMap **varidx_map) ଅଚଳ
  {
#ifndef HB_NO_AVAR2
    ଯଦି (version.major < 2) ଫେରନ୍ତୁ ମିଥ୍ୟା;
    ଅଚଳ SegmentMaps *map = &firstAxisSegmentMaps;
    ପାଇଁ (unsigned i = 0; i < axis_ଗଣନା_2; i++)
      map = &StructAfter<SegmentMaps> (*map);
    ଅଚଳ auto &v2 = * (ଅଚଳ avarV2Tail *) map;
    *store = &(ଏହି+v2.varStore);
    *varidx_map = &(ଏହି+v2.varIdxMap);
    ଫେରନ୍ତୁ ସତ୍ୟ;
#ଅନ୍ୟଥା
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
#endif
  }

  // axis normalization is done in 2.14 here
  // TODO: deprecate this API once fonttools is updated to use 16.16 normalization
  ତାର୍କିକ map_coords_2_14 (ଦଶମିକ *coords, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା coords_length,
			ତାର୍କିକ v1_only = ମିଥ୍ୟା) ଅଚଳ
  {
    hb_vector_t<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> coords_2_14;
    ଯଦି (!v1_only && !coords_2_14.resize (coords_length)) ଫେରନ୍ତୁ ମିଥ୍ୟା;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = hb_min (coords_length, axis_ଗଣନା_2);

    ଅଚଳ SegmentMaps *map = &firstAxisSegmentMaps;
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଗଣନା_2; i++)
    {
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା v = roundf (map->map_float (coords[i]) * 16384.f);
      ଯଦି (!v1_only)
	coords_2_14[i] = v;
      coords[i] = v / 16384.f;
      map = &StructAfter<SegmentMaps> (*map);
    }

    ଯଦି (v1_only)
      ଫେରନ୍ତୁ ସତ୍ୟ;

#ifndef HB_NO_AVAR2
    ଯଦି (version.major < 2)
      ଫେରନ୍ତୁ ସତ୍ୟ;
    hb_barrier ();

    ପାଇଁ (; ଗଣନା_2 < axis_ଗଣନା_2; ଗଣନା_2++)
      map = &StructAfter<SegmentMaps> (*map);

    ଅଚଳ auto &v2 = * (ଅଚଳ avarV2Tail *) map;

    ଅଚଳ auto &varidx_map = ଏହି+v2.varIdxMap;
    ଅଚଳ auto &var_store = ଏହି+v2.varStore;
    auto *var_store_cache = var_store.create_cache ();

    ପାଇଁ (unsigned i = 0; i < coords_length; i++)
    {
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା v = coords_2_14[i];
      uint32_t varidx = varidx_map.map (i);
      ଦଶମିକ delta = var_store.get_delta (varidx, coords_2_14.arrayZ, coords_2_14.length, var_store_cache);
      /* As above: apply the delta unclamped (±2 covers every useful case)
       * and clamp the result to [-1, +1], matching fontTools. */
      v += (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) hb_clamp (roundf (delta), -(ଦଶମିକ) (1<<15), +(ଦଶମିକ) (1<<15));
      v = hb_clamp (v, -(1<<14), +(1<<14));
      coords[i] = v / 16384.f;
    }

    OT::ItemVariationStore::destroy_cache (var_store_cache);
    ଫେରନ୍ତୁ ସତ୍ୟ;
#ଅନ୍ୟଥା
    ଫେରନ୍ତୁ version.major < 2;
#endif
  }

  ତାର୍କିକ subset (hb_subset_context_t *c) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    unsigned retained_axis_ଗଣନା = c->plan->axes_index_map.get_population ();
    ଯଦି (!retained_axis_ଗଣନା) //all axes are pinned/dropped
      return_trace (ମିଥ୍ୟା);

    avar *out = c->serializer->allocate_min<avar> ();
    ଯଦି (unlikely (!out)) return_trace (ମିଥ୍ୟା);

    out->version.major = c->plan->has_avar2 ? 2 : 1;
    out->version.minor = 0;
    ଯଦି (!c->serializer->check_assign (out->axis_ଗଣନା_2, retained_axis_ଗଣନା, HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (ମିଥ୍ୟା);

    /* For avar2, keep the instantiated v1 mappings around; offset
     * compensation needs them to locate the new mappings' kinks. */
    hb_vector_t<hb_vector_t<Axis_ମୂଲ୍ୟ_Map>> new_mappings;
    ଯଦି (c->plan->has_avar2 && !new_mappings.resize (axis_ଗଣନା_2))
      return_trace (ମିଥ୍ୟା);

    ଅଚଳ hb_map_t& axes_index_map = c->plan->axes_index_map;
    ଅଚଳ SegmentMaps *map = &firstAxisSegmentMaps;
    unsigned ଗଣନା_2 = axis_ଗଣନା_2;
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଗଣନା_2; i++)
    {
      ଯଦି (axes_index_map.has (i))
      {
        hb_tag_t *axis_tag;
        ଯଦି (!c->plan->axes_old_index_tag_map.has (i, &axis_tag))
          return_trace (ମିଥ୍ୟା);

        Triple *axis_location;
        ଯଦି (c->plan->has_avar2 &&
            c->plan->user_axes_location.has (*axis_tag, &axis_location) &&
            axis_location->is_point ())
        {
          /* Pinned axis in avar2 mode: serialize identity segment map
           * {-1->-1, 0->0, 1->1}. The axis is kept in fvar as hidden,
           * so avar needs a segment map entry for it. */
          auto *identity_map = c->serializer->start_embed<SegmentMaps> ();
          ଯଦି (unlikely (!c->serializer->extend_min (identity_map)))
            return_trace (ମିଥ୍ୟା);
          Axis_ମୂଲ୍ୟ_Map m;
          m.set_mapping (-1.f, -1.f);
          ଯଦି (!m.serialize (c->serializer)) return_trace (ମିଥ୍ୟା);
          m.set_mapping (0.f, 0.f);
          ଯଦି (!m.serialize (c->serializer)) return_trace (ମିଥ୍ୟା);
          m.set_mapping (1.f, 1.f);
          ଯଦି (!m.serialize (c->serializer)) return_trace (ମିଥ୍ୟା);
          ଯଦି (!c->serializer->check_assign (identity_map->len, 3u,
                                            HB_SERIALIZE_ERROR_INT_OVERFLOW))
            return_trace (ମିଥ୍ୟା);
        }
        ଅନ୍ୟଥା
        {
          /* Restricted or free axis: use standard SegmentMaps::subset() */
          ଯଦି (!map->subset (c, *axis_tag,
                            c->plan->has_avar2 ? &new_mappings[i] : nullptr))
            return_trace (ମିଥ୍ୟା);
        }
      }
      map = &StructAfter<SegmentMaps> (*map);
    }

    ଯଦି (c->plan->has_avar2)
      return_trace (_subset_avar2 (c, new_mappings));

    return_trace (ସତ୍ୟ);
  }

  ବ୍ୟକ୍ତିଗତ:
  ସଂରଚନା avar2_index_map_plan_t
  {
    ତାର୍କିକ init (ଅଚଳ hb_vector_t<uint32_t> &varidx_mapping,
	       ଅଚଳ hb_map_t &axes_index_map,
	       unsigned axis_ଗଣନା)
    {
      ଯଦି (!output_map.alloc (axes_index_map.get_population ()))
	ଫେରନ୍ତୁ ମିଥ୍ୟା;

      ତାର୍କିକ has_no_variation = ମିଥ୍ୟା;
      unsigned max_outer = 0, max_inner = 0;
      ପାଇଁ (unsigned i = 0; i < axis_ଗଣନା; i++)
      {
	ଯଦି (!axes_index_map.has (i)) ଜାରି;
	uint32_t varidx = varidx_mapping[i];
	output_map.push (varidx);
	ଯଦି (varidx == HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
	{
	  has_no_variation = ସତ୍ୟ;
	  ଜାରି;
	}
	max_outer = hb_max (max_outer, varidx >> 16);
	max_inner = hb_max (max_inner, varidx & 0xFFFF);
      }
      ଯଦି (output_map.in_error () ||
	  output_map.length != axes_index_map.get_population ())
	ଫେରନ୍ତୁ ମିଥ୍ୟା;

      ଯଦି (has_no_variation)
      {
	width = 4;
	inner_bit_ଗଣନା = 16;
      }
      ଅନ୍ୟଥା
      {
	inner_bit_ଗଣନା = hb_max (1u, hb_bit_storage (max_inner));
	unsigned outer_bit_ଗଣନା = hb_max (1u, hb_bit_storage (max_outer));
	width = hb_clamp ((inner_bit_ଗଣନା + outer_bit_ଗଣନା + 7) / 8, 1u, 4u);
	ଯଦି (inner_bit_ଗଣନା + outer_bit_ଗଣନା > width * 8)
	  inner_bit_ଗଣନା = width * 8 - outer_bit_ଗଣନା;
      }
      ଫେରନ୍ତୁ ସତ୍ୟ;
    }

    unsigned get_inner_bit_ଗଣନା () ଅଚଳ { ଫେରନ୍ତୁ inner_bit_ଗଣନା; }
    unsigned get_width () ଅଚଳ { ଫେରନ୍ତୁ width; }
    hb_array_t<ଅଚଳ uint32_t> get_output_map () ଅଚଳ { ଫେରନ୍ତୁ output_map.as_array (); }

    unsigned inner_bit_ଗଣନା = 1;
    unsigned width = 1;
    hb_vector_t<uint32_t> output_map;
  };

  ତାର୍କିକ _subset_avar2 (hb_subset_context_t *c,
                      ଅଚଳ hb_vector_t<hb_vector_t<Axis_ମୂଲ୍ୟ_Map>> &new_mappings) ଅଚଳ
  {
#ଯଦି defined (HB_NO_VAR) || defined (HB_NO_AVAR2)
    /* Not reachable: the plan never sets has_avar2 in these configurations. */
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
#ଅନ୍ୟଥା

    /* 1. Locate original avar2 data, keeping per-axis old segment maps */
    hb_vector_t<ଅଚଳ SegmentMaps *> old_seg_maps;
    ଯଦି (!old_seg_maps.alloc (axis_ଗଣନା_2)) ଫେରନ୍ତୁ ମିଥ୍ୟା;
    ଅଚଳ SegmentMaps *map = &firstAxisSegmentMaps;
    ପାଇଁ (unsigned i = 0; i < axis_ଗଣନା_2; i++)
    {
      old_seg_maps.push (map);
      map = &StructAfter<SegmentMaps> (*map);
    }

    ଅଚଳ auto &v2 = * (ଅଚଳ avarV2Tail *) map;
    ଅଚଳ auto &varidx_map = ଏହି+v2.varIdxMap;
    ଅଚଳ auto &var_store = ଏହି+v2.varStore;

    auto fvar_axes = c->plan->source->table.fvar->get_axes ();

    /* 2. Compute default deltas by evaluating VarStore at old defaults */
    hb_vector_t<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> default_coords;
    ଯଦି (!default_coords.resize (axis_ଗଣନା_2)) ଫେରନ୍ତୁ ମିଥ୍ୟା;
    ପାଇଁ (unsigned i = 0; i < axis_ଗଣନା_2; i++)
    {
      hb_tag_t *axis_tag;
      ଯଦି (c->plan->axes_old_index_tag_map.has (i, &axis_tag) &&
          c->plan->old_intermediates.has (*axis_tag))
      {
        ଦଶମିକ d_i = (ଦଶମିକ) c->plan->old_intermediates.get (*axis_tag).ମଧ୍ୟ;
        default_coords[i] = roundf (d_i * 16384.f);
      }
      ଅନ୍ୟଥା
        default_coords[i] = 0;
    }

    auto *store_cache = var_store.create_cache ();
    hb_vector_t<ଦଶମିକ> default_deltas;
    ଯଦି (!default_deltas.resize (axis_ଗଣନା_2)) {
      ItemVariationStore::destroy_cache (store_cache);
      ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }
    ପାଇଁ (unsigned i = 0; i < axis_ଗଣନା_2; i++)
    {
      uint32_t varidx = varidx_map.map (i);
      ଯଦି (varidx == HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
        default_deltas[i] = 0.f;
      ଅନ୍ୟଥା
        default_deltas[i] = var_store.get_delta (varidx, default_coords.arrayZ,
                                                  default_coords.length, store_cache);
    }
    ItemVariationStore::destroy_cache (store_cache);

    /* 3. Rebase IVS regions */
    item_variations_t item_vars;
    ଯଦି (!item_vars.create_from_item_varstore (var_store, c->plan->axes_old_index_tag_map))
      ଫେରନ୍ତୁ ମିଥ୍ୟା;
    ଯଦି (!item_vars.instantiate_tuple_vars (c->plan->old_intermediates,
                                           c->plan->axes_triple_distances,
                                           ମିଥ୍ୟା))
      ଫେରନ୍ତୁ ମିଥ୍ୟା;

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
    ଯଦି (!new_varidx_mapping.resize (axis_ଗଣନା_2)) ଫେରନ୍ତୁ ମିଥ୍ୟା;
    ପାଇଁ (unsigned i = 0; i < axis_ଗଣନା_2; i++)
    {
      uint32_t varidx = varidx_map.map (i);
      ଯଦି (varidx != HB_OT_LAYOUT_NO_VARIATIONS_INDEX &&
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
    hb_hashmap_t<uint32_t, unsigned> varidx_ref_ଗଣନା;
    ପାଇଁ (unsigned i = 0; i < axis_ଗଣନା_2; i++)
    {
      ଯଦି (!c->plan->axes_index_map.has (i)) ଜାରି; /* self-contained: dropped */
      uint32_t varidx = new_varidx_mapping[i];
      ଯଦି (varidx == HB_OT_LAYOUT_NO_VARIATIONS_INDEX) ଜାରି;
      unsigned *ଗଣନା_2;
      ଯଦି (varidx_ref_ଗଣନା.has (varidx, &ଗଣନା_2))
        (*ଗଣନା_2)++;
      ଅନ୍ୟଥା ଯଦି (!varidx_ref_ଗଣନା.set (varidx, 1))
        ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }
    ପାଇଁ (unsigned i = 0; i < axis_ଗଣନା_2; i++)
    {
      hb_tag_t *axis_tag_ptr;
      ଯଦି (!c->plan->axes_old_index_tag_map.has (i, &axis_tag_ptr))
        ଫେରନ୍ତୁ ମିଥ୍ୟା;
      /* Only axes that will receive offset compensation (restricted or
       * pinned) can contaminate a shared row. */
      ଯଦି (!c->plan->axes_index_map.has (i) ||
          !c->plan->user_axes_location.has (*axis_tag_ptr))
        ଜାରି;
      uint32_t varidx = new_varidx_mapping[i];
      ଯଦି (varidx == HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
        ଜାରି; /* gets a fresh, private VarData in the offset loop below */
      unsigned *ଗଣନା_2;
      ଯଦି (!varidx_ref_ଗଣନା.has (varidx, &ଗଣନା_2) || *ଗଣନା_2 <= 1)
        ଜାରି; /* sole owner: safe to write offsets in place */
      unsigned outer = varidx >> 16;
      unsigned new_inner = item_vars.duplicate_row (outer, varidx & 0xFFFF);
      ଯଦି (unlikely (new_inner == (unsigned) -1)) ଫେରନ୍ତୁ ମିଥ୍ୟା;
      new_varidx_mapping[i] = (outer << 16) | new_inner;
      (*ଗଣନା_2)--;
    }

    /* 6. Add offset compensation tuples.
     * Track processed (outer,inner) pairs to avoid adding duplicate biases
     * when multiple axes share the same varIdx. */
    hb_set_t processed_varidxes;
    ପାଇଁ (unsigned i = 0; i < axis_ଗଣନା_2; i++)
    {
      hb_tag_t *axis_tag_ptr;
      ଯଦି (!c->plan->axes_old_index_tag_map.has (i, &axis_tag_ptr))
        ଫେରନ୍ତୁ ମିଥ୍ୟା;
      hb_tag_t axis_tag = *axis_tag_ptr;

      /* Self-contained pinned axes are removed from fvar/avar; their
       * contribution is baked into the variation tables instead. */
      ଯଦି (!c->plan->axes_index_map.has (i))
        ଜାରି;

      Triple *new_user;
      ଯଦି (c->plan->user_axes_location.has (axis_tag, &new_user))
      {
        /* This axis is being restricted or pinned */
        Triple *old_int;
        ଯଦି (!c->plan->old_intermediates.has (axis_tag, &old_int))
          ଫେରନ୍ତୁ ମିଥ୍ୟା;

        ଦଶମିକ a_i = (ଦଶମିକ) old_int->minimum;
        ଦଶମିକ d_i = (ଦଶମିକ) old_int->ମଧ୍ୟ;
        ଦଶମିକ b_i = (ଦଶମିକ) old_int->maximum;

        ପୂର୍ଣ୍ଣସଂଖ୍ୟା d_int = roundf (d_i * 16384.f);

        ତାର୍କିକ is_pinned = new_user->is_point ();

        uint32_t varidx = new_varidx_mapping[i];
        unsigned outer, inner, item_ଗଣନା_2;

        ଯଦି (varidx == HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
        {
          /* No existing avar2 mapping. Create new VarData. */
          outer = item_vars.add_vardata (1);
          inner = 0;
          item_ଗଣନା_2 = 1;
          new_varidx_mapping[i] = (outer << 16) | inner;
          default_deltas[i] = 0.f; /* no prior default delta */
        }
        ଅନ୍ୟଥା
        {
          outer = varidx >> 16;
          inner = varidx & 0xFFFF;
          item_ଗଣନା_2 = item_vars.get_item_ଗଣନା (outer);
        }

        /* Empty-region bias: d_int + round(defaultDelta) */
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା bias = d_int + (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) roundf (default_deltas[i]);
        ଯଦି (bias != 0)
        {
          hb_hashmap_t<hb_tag_t, Triple> empty_region;
          item_vars.add_tuple (outer, std::move (empty_region),
                               inner, bias, item_ଗଣନା_2);
        }

        ଯଦି (!is_pinned)
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
          _avar2_add_knot (knots, -1.0, (ଦ୍ୱିଗୁଣ) a_i + 1.0);
          _avar2_add_knot (knots, 0.0, (ଦ୍ୱିଗୁଣ) d_i);
          _avar2_add_knot (knots, 1.0, (ଦ୍ୱିଗୁଣ) b_i - 1.0);

          ଅଚଳ hb_vector_t<Axis_ମୂଲ୍ୟ_Map> &new_mapping = new_mappings[i];

          ଦଶମିକ min_f = 0.f, def_f = 0.f, max_f = 0.f;
          ଯଦି (likely (i < fvar_axes.length))
            fvar_axes[i].get_coordinates (min_f, def_f, max_f);
          ଦ୍ୱିଗୁଣ old_min = (ଦ୍ୱିଗୁଣ) min_f;
          ଦ୍ୱିଗୁଣ old_def = (ଦ୍ୱିଗୁଣ) def_f;
          ଦ୍ୱିଗୁଣ old_max = (ଦ୍ୱିଗୁଣ) max_f;

          /* If the axis default MOVED, inv_renorm also kinks where the OLD
           * default lands in the new space (the old intermediate coordinate
           * crosses 0 there), at
           *   z = z_old  ->  -z_old
           * Omitting that knot (as a plain two-tent encoding would) makes
           * interior coordinates wrong. */
          ଦ୍ୱିଗୁଣ z_old = avar2_normalize_ମୂଲ୍ୟ ((ଦ୍ୱିଗୁଣ) old_def,
                                                 (ଦ୍ୱିଗୁଣ) new_user->minimum,
                                                 (ଦ୍ୱିଗୁଣ) new_user->ମଧ୍ୟ,
                                                 (ଦ୍ୱିଗୁଣ) new_user->maximum);
          z_old = _avar2_map_new_mapping (new_mapping, z_old);
          z_old = (ଦ୍ୱିଗୁଣ) roundf ((ଦଶମିକ) (z_old * 16384.0)) / 16384.0;
          ଯଦି (-1.0 < z_old && z_old < 1.0)
            _avar2_add_knot (knots, z_old, -z_old);

          /* Interior avar v1 breakpoints inside the retained range each put
           * a kink in offset(z). Sampling only {-1, 0, +1, z_old} would
           * linearly interpolate across those kinks. The instantiated
           * mapping keeps exactly the in-range old breakpoints, and the new
           * mapping kinks at each one's output coordinate; add that z with
           * its old intermediate value so offset(z) is reproduced at every
           * kink. */
          hb_vector_t<avar2_offset_knot_t> with_breakpoints (knots);
          ପାଇଁ (ଅଚଳ auto &m : new_mapping)
          {
            ଦ୍ୱିଗୁଣ from = (ଦ୍ୱିଗୁଣ) m.coords[0].to_float ();
            ଯଦି (from == -1.0 || from == 0.0 || from == 1.0)
              ଜାରି; /* anchors already seeded */
            ଦ୍ୱିଗୁଣ z = (ଦ୍ୱିଗୁଣ) m.coords[1].to_float ();
            ଯଦି (!(-1.0 < z && z < 1.0))
              ଜାରି;
            ଦ୍ୱିଗୁଣ user = avar2_denormalize_ମୂଲ୍ୟ (from,
                                                    (ଦ୍ୱିଗୁଣ) new_user->minimum,
                                                    (ଦ୍ୱିଗୁଣ) new_user->ମଧ୍ୟ,
                                                    (ଦ୍ୱିଗୁଣ) new_user->maximum);
            ଦ୍ୱିଗୁଣ n_old = avar2_normalize_ମୂଲ୍ୟ (user, old_min, old_def, old_max);
            ଦ୍ୱିଗୁଣ x_old = (ଦ୍ୱିଗୁଣ) old_seg_maps[i]->map_float ((ଦଶମିକ) n_old);
            x_old = (ଦ୍ୱିଗୁଣ) roundf ((ଦଶମିକ) (x_old * 16384.0)) / 16384.0;
            _avar2_add_knot (with_breakpoints, z, x_old - z);
          }

          ଯଦି (unlikely (knots.in_error () || with_breakpoints.in_error ()))
            ଫେରନ୍ତୁ ମିଥ୍ୟା;

          /* Extra tents cost F2Dot14 rounding, so for a steep segment they
           * can add more quantization noise than the structural error they
           * remove. Keep the interior breakpoints only when they do not
           * increase the estimated residual; this makes the collection a
           * strict (never-worse) improvement over the {-1, 0, +1, z_old}
           * anchors. Warn when even the better choice is not bit-exact (a
           * steep retained segment that cannot be reproduced in F2Dot14). */
          unsigned err = _avar2_estimate_offset_error (*old_seg_maps[i], new_mapping,
                                                       old_min, old_def, old_max,
                                                       (ଦ୍ୱିଗୁଣ) new_user->minimum,
                                                       (ଦ୍ୱିଗୁଣ) new_user->ମଧ୍ୟ,
                                                       (ଦ୍ୱିଗୁଣ) new_user->maximum,
                                                       knots);
          ଯଦି (with_breakpoints.length != knots.length)
          {
            unsigned err_with = _avar2_estimate_offset_error (*old_seg_maps[i], new_mapping,
                                                              old_min, old_def, old_max,
                                                              (ଦ୍ୱିଗୁଣ) new_user->minimum,
                                                              (ଦ୍ୱିଗୁଣ) new_user->ମଧ୍ୟ,
                                                              (ଦ୍ୱିଗୁଣ) new_user->maximum,
                                                              with_breakpoints);
            ଯଦି (err_with <= err)
            {
              knots = std::move (with_breakpoints);
              err = err_with;
            }
          }
          ଯଦି (err > 8)
            DEBUG_ସନ୍ଦେଶ (SUBSET, nullptr,
                       "avar2 offset compensation is approximate for axis %c%c%c%c: "
                       "max residual %u F2Dot14 units",
                       HB_UNTAG (axis_tag), err);

          /* Synthesize tents. Adjacent tents evaluate to zero at each
           * other's peaks, so each knot's delta is offset(z) - offset(0);
           * the base value offset(0) = d_i is carried by the empty-region
           * bias above. This reduces to the classic pair of tents
           * (-1,-1,0) / (0,+1,+1) when the default is unchanged and there
           * are no interior knots. */
          ପାଇଁ (unsigned k = 0; k < knots.length; k++)
          {
            ଦ୍ୱିଗୁଣ z = knots.arrayZ[k].z;
            ଯଦି (z == 0.0) ଜାରି;
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା delta = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) roundf ((ଦଶମିକ) ((knots.arrayZ[k].offset - (ଦ୍ୱିଗୁଣ) d_i) * 16384.0));
            ଯଦି (!delta) ଜାରି;
            ଦ୍ୱିଗୁଣ lower, upper;
            ଯଦି (z > 0.0)
            {
              lower = knots.arrayZ[k - 1].z;
              upper = k + 1 < knots.length ? knots.arrayZ[k + 1].z : z;
            }
            ଅନ୍ୟଥା
            {
              upper = knots.arrayZ[k + 1].z;
              lower = k > 0 ? knots.arrayZ[k - 1].z : z;
            }
            hb_hashmap_t<hb_tag_t, Triple> region;
            ଯଦି (unlikely (!region.set (axis_tag, Triple (lower, z, upper))))
              ଫେରନ୍ତୁ ମିଥ୍ୟା;
            item_vars.add_tuple (outer, std::move (region),
                                 inner, delta, item_ଗଣନା_2);
          }
        }
      }
      ଅନ୍ୟଥା
      {
        /* Free or private axis — not being restricted.
         * If it has a non-zero default delta, add it back as a bias.
         * Skip if this (outer,inner) was already processed (shared varIdx). */
        uint32_t varidx = new_varidx_mapping[i];
        ଯଦି (varidx == HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
          ଜାରି;

        ଯଦି (processed_varidxes.has (varidx))
          ଜାରି;
        processed_varidxes.add (varidx);

        unsigned outer = varidx >> 16;
        unsigned inner = varidx & 0xFFFF;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା dd = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) roundf (default_deltas[i]);
        ଯଦି (dd != 0)
        {
          unsigned item_ଗଣନା_2 = item_vars.get_item_ଗଣନା (outer);
          hb_hashmap_t<hb_tag_t, Triple> empty_region;
          item_vars.add_tuple (outer, std::move (empty_region),
                               inner, dd, item_ଗଣନା_2);
        }
      }
    }

    /* 7. Finalize: build region list + convert to varstore */
    ଯଦି (!item_vars.build_region_list ()) ଫେରନ୍ତୁ ମିଥ୍ୟା;
    ଯଦି (!item_vars.as_item_varstore (ସତ୍ୟ /* optimize */,
                                     ମିଥ୍ୟା /* use_no_variation_idx */))
      ଫେରନ୍ତୁ ମିଥ୍ୟା;

    /* 8. Apply varidx_map optimization remapping */
    ଅଚଳ auto &opt_varidx_map = item_vars.get_varidx_map ();
    ପାଇଁ (unsigned i = 0; i < axis_ଗଣନା_2; i++)
    {
      uint32_t varidx = new_varidx_mapping[i];
      ଯଦି (varidx == HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
        ଜାରି;
      uint32_t *new_idx;
      ଯଦି (opt_varidx_map.has (varidx, &new_idx))
        new_varidx_mapping[i] = *new_idx;
    }

    /* 9. Serialize avarV2Tail. Entries cover the retained axes only;
     * self-contained pinned axes are removed from fvar. */
    avar2_index_map_plan_t index_map_plan;
    ଯଦି (!index_map_plan.init (new_varidx_mapping,
			      c->plan->axes_index_map,
			      axis_ଗଣନା_2))
      ଫେରନ୍ତୁ ମିଥ୍ୟା;

    auto *tail = c->serializer->allocate_size<avarV2Tail> (avarV2Tail::static_size);
    ଯଦି (unlikely (!tail)) ଫେରନ୍ତୁ ମିଥ୍ୟା;
    ଯଦି (!tail->varIdxMap.serialize_serialize (c->serializer, index_map_plan))
      ଫେରନ୍ତୁ ମିଥ୍ୟା;
    ଯଦି (!tail->varStore.serialize_serialize (c->serializer,
					     item_vars.has_long_word (),
					     c->plan->axis_tags,
					     item_vars.get_region_list (),
					     item_vars.get_vardata_encodings ()))
      ଫେରନ୍ତୁ ମିଥ୍ୟା;

    ଫେରନ୍ତୁ ସତ୍ୟ;
#endif
  }

  ସର୍ବସାଧାରଣ:

  ସୁରକ୍ଷିତ:
  FixedVersion<>version;	/* Version of the avar table
				 * initially set to 0x00010000u */
  HBUINT16	reserved;	/* This field is permanently reserved. Set to 0. */
  HBUINT16	axis_ଗଣନା_2;	/* The number of variation axes in the font. This
				 * must be the same number as axisCount in the
				 * 'fvar' table. */
  SegmentMaps	firstAxisSegmentMaps;

  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_MIN (8);
};

} /* namespace OT */


#endif /* HB_OT_VAR_AVAR_TABLE_HH */
