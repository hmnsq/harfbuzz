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

#অন্তর্ভাব "hb-open-type.hh"
#অন্তর্ভাব "hb-ot-var-common.hh"
#অন্তর্ভাব "hb-ot-var-fvar-table.hh"


/*
 * avar -- Axis Variations
 * https://docs.microsoft.com/en-us/typography/opentype/spec/avar
 */

#define HB_OT_TAG_avar HB_TAG('a','v','a','r')


নামাকাশ OT {


/* "Spec": https://github.com/be-fonts/boring-expansion-spec/issues/14 */
কাঠামো avar_V2_লেজ
{
  friend কাঠামো avar;

  যৌক্তিক sanitize (hb_sanitize_context_t *c,
		 ধ্রুবক শূন্য *base) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (varIdxMap.sanitize (c, base) &&
		  varStore.sanitize (c, base));
  }

  সুরক্ষিত:
  Offset32To<Delta_Set_সূচক_Map>	varIdxMap;	/* Offset from the beginning of 'avar' table. */
  Offset32To<ItemVariationStore>	varStore;	/* Offset from the beginning of 'avar' table. */

  সর্বজনীন:
  DEFINE_আকার_STATIC (8);
};


কাঠামো Axis_মান_Map
{
  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই));
  }

  শূন্য set_mapping (দশমিক from_coord, দশমিক to_coord)
  {
    coords[0].set_float (from_coord);
    coords[1].set_float (to_coord);
  }

  যৌক্তিক is_outside_axis_range (ধ্রুবক Triple& axis_range) ধ্রুবক
  {
    দ্বিগুণ from_coord = (দ্বিগুণ) coords[0].to_float ();
    ফেরত !axis_range.contains (from_coord);
  }

  যৌক্তিক must_include () ধ্রুবক
  {
    দশমিক from_coord = coords[0].to_float ();
    দশমিক to_coord = coords[1].to_float ();
    ফেরত (from_coord == -1.f && to_coord == -1.f) ||
           (from_coord == 0.f && to_coord == 0.f) ||
           (from_coord == 1.f && to_coord == 1.f);
  }

  শূন্য instantiate (ধ্রুবক Triple& axis_range,
                    ধ্রুবক Triple& unmapped_range,
                    ধ্রুবক TripleDistances& triple_distances)
  {
    দশমিক from_coord = coords[0].to_float ();
    দশমিক to_coord = coords[1].to_float ();

    from_coord = renormalize_মান ((দ্বিগুণ) from_coord, unmapped_range, triple_distances);
    to_coord = renormalize_মান ((দ্বিগুণ) to_coord, axis_range, triple_distances);

    /* Malformed input can renormalize outside [-1,1], which would
     * overflow F2DOT14. */
    from_coord = hb_clamp (from_coord, -1.f, 1.f);
    to_coord = hb_clamp (to_coord, -1.f, 1.f);

    coords[0].set_float (from_coord);
    coords[1].set_float (to_coord);
  }

  HB_INTERNAL স্থির পূর্ণসংখ্যা cmp (ধ্রুবক শূন্য *pa, ধ্রুবক শূন্য *pb)
  {
    ধ্রুবক Axis_মান_Map *a = (ধ্রুবক Axis_মান_Map *) pa;
    ধ্রুবক Axis_মান_Map *b = (ধ্রুবক Axis_মান_Map *) pb;

    পূর্ণসংখ্যা a_from = a->coords[0].to_int ();
    পূর্ণসংখ্যা b_from = b->coords[0].to_int ();
    যদি (a_from != b_from)
      ফেরত a_from - b_from;

    /* this should never be reached. according to the spec, all of the axis
     * value map records for a given axis must have different fromCoord values
     * */
    পূর্ণসংখ্যা a_to = a->coords[1].to_int ();
    পূর্ণসংখ্যা b_to = b->coords[1].to_int ();
    ফেরত a_to - b_to;
  }

  যৌক্তিক serialize (hb_serialize_context_t *c) ধ্রুবক
  {
    TRACE_SERIALIZE (এই);
    return_trace (c->embed (এই));
  }

  সর্বজনীন:
  F2DOT14	coords[2];
//   F2DOT14	fromCoord;	/* A normalized coordinate value obtained using
//				 * default normalization. */
//   F2DOT14	toCoord;	/* The modified, normalized coordinate value. */

  সর্বজনীন:
  DEFINE_আকার_STATIC (4);
};

কাঠামো SegmentMaps : Array16Of<Axis_মান_Map>
{
  দশমিক map_float (দশমিক value, unsigned পূর্ণসংখ্যা from_offset = 0, unsigned পূর্ণসংখ্যা to_offset = 1) ধ্রুবক
  {
#define fromCoord coords[from_offset].to_float ()
#define toCoord coords[to_offset].to_float ()

    ধ্রুবক auto *map = arrayZ;

    /* The following special-cases are not part of OpenType, which requires
     * that at least -1, 0, and +1 must be mapped. But we include these as
     * part of a better error recovery scheme. */
    যদি (len < 2)
    {
      যদি (!len)
	ফেরত value;
      অন্যথা /* len == 1*/
	ফেরত value - map[0].fromCoord + map[0].toCoord;
    }

    // At least two mappings now.

    /* CoreText is wild...
     * PingFangUI avar needs all this special-casing...
     * So we implement an extended version of the spec here,
     * which is more robust and more likely to be compatible with
     * the wild. */

    unsigned start = 0;
    unsigned end = len;
    যদি (map[start].fromCoord == -1 && map[start].toCoord == -1 && map[start+1].fromCoord == -1)
      start++;
    যদি (map[end-1].fromCoord == +1 && map[end-1].toCoord == +1 && map[end-2].fromCoord == +1)
      end--;

    /* Look for exact match first, and do lots of special-casing. */
    unsigned i;
    জন্য (i = start; i < end; i++)
      যদি (value == map[i].fromCoord)
	বিরতি;
    যদি (i < end)
    {
      // There's at least one exact match. See if there are more.
      unsigned j = i;
      জন্য (; j + 1 < end; j++)
	যদি (value != map[j + 1].fromCoord)
	  বিরতি;

      // [i,j] inclusive are all exact matches:

      // If there's only one, return it. This is the only spec-compliant case.
      যদি (i == j)
	ফেরত map[i].toCoord;
      // If there's exactly three, return the middle one.
      যদি (i + 2 == j)
	ফেরত map[i + 1].toCoord;

      // Ignore the middle ones. Return the one mapping closer to 0.
      যদি (value < 0) ফেরত map[j].toCoord;
      যদি (value > 0) ফেরত map[i].toCoord;

      // Mapping 0? CoreText seems confused. It seems to prefer 0 here...
      // So we'll just return the smallest one. lol
      ফেরত fabsf (map[i].toCoord) < fabsf (map[j].toCoord) ? map[i].toCoord : map[j].toCoord;

      // Mapping 0? Return one not mapping to 0.
      যদি (map[i].toCoord == 0)
	ফেরত map[j].toCoord;
      অন্যথা
	ফেরত map[i].toCoord;
    }

    /* There's at least two and we're not an exact match. Prepare to lerp. */

    // Find the segment we're in.
    জন্য (i = start; i < end; i++)
      যদি (value < map[i].fromCoord)
	বিরতি;

    যদি (i == start)
    {
      // Value before all segments; Shift.
      ফেরত value - map[start].fromCoord + map[start].toCoord;
    }
    যদি (i == end)
    {
      // Value after all segments; Shift.
      ফেরত value - map[end - 1].fromCoord + map[end - 1].toCoord;
    }

    // Actually interpolate.
    auto &before = map[i-1];
    auto &after = map[i];
    দশমিক denom = after.fromCoord - before.fromCoord; // Can't be zero by now.
    ফেরত before.toCoord + ((after.toCoord - before.toCoord) * (value - before.fromCoord)) / denom;

#undef toCoord
#undef fromCoord
  }

  দশমিক unmap_float (দশমিক value) ধ্রুবক { ফেরত map_float (value, 1, 0); }


  // TODO Kill this.
  Triple unmap_axis_range (ধ্রুবক Triple& axis_range) ধ্রুবক
  {
    দশমিক unmapped_min = unmap_float (axis_range.minimum);
    দশমিক unmapped_মধ্য = unmap_float (axis_range.মধ্য);
    দশমিক unmapped_max = unmap_float (axis_range.maximum);

    ফেরত Triple{unmapped_min, unmapped_মধ্য, unmapped_max};
  }

  যৌক্তিক subset (hb_subset_context_t *c, hb_tag_t axis_tag,
               hb_vector_t<Axis_মান_Map> *out_mappings = nullptr) ধ্রুবক
  {
    TRACE_SUBSET (এই);

    /* This function cannot work on avar2 table (and currently doesn't).
     * We should instead keep the design coords in the shape plan and use
     * those. unmap_axis_range needs to be killed. */

    /* avar mapped normalized axis range. Under avar2, axes_location holds
     * only the self-contained pins for the other tables; avar itself uses
     * the intermediate-space ranges of all restricted axes. */
    ধ্রুবক auto &axes_location = c->plan->has_avar2
				? c->plan->old_intermediates
				: c->plan->axes_location;
    Triple *axis_range;
    যদি (!axes_location.has (axis_tag, &axis_range))
      ফেরত c->serializer->embed (*এই);

    TripleDistances *axis_triple_distances;
    যদি (!c->plan->axes_triple_distances.has (axis_tag, &axis_triple_distances))
      return_trace (মিথ্যা);

    auto *out = c->serializer->start_embed (এই);
    যদি (unlikely (!c->serializer->extend_min (out))) return_trace (মিথ্যা);

    Triple unmapped_range = unmap_axis_range (*axis_range);

    /* create a vector of retained mappings and sort */
    hb_vector_t<Axis_মান_Map> মান_mappings;
    জন্য (ধ্রুবক auto& _ : as_array ())
    {
      যদি (_.is_outside_axis_range (unmapped_range))
        চালু;
      Axis_মান_Map mapping;
      mapping = _;
      mapping.instantiate (*axis_range, unmapped_range, *axis_triple_distances);
      /* (-1, -1), (0, 0), (1, 1) mappings will be added later, so avoid
       * duplicates here */
      যদি (mapping.must_include ())
        চালু;
      মান_mappings.push (mapping);
    }

    Axis_মান_Map m;
    m.set_mapping (-1.f, -1.f);
    মান_mappings.push (m);

    m.set_mapping (0.f, 0.f);
    মান_mappings.push (m);

    m.set_mapping (1.f, 1.f);
    মান_mappings.push (m);

    মান_mappings.qsort ();

    যদি (unlikely (মান_mappings.in_error ()))
      return_trace (মিথ্যা);

    জন্য (ধ্রুবক auto& _ : মান_mappings)
    {
      যদি (!_.serialize (c->serializer))
        return_trace (মিথ্যা);
    }
    যদি (!c->serializer->check_assign (out->len, মান_mappings.দৈর্ঘ্য, HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (মিথ্যা);
    /* Hand the instantiated mappings to the caller; avar2 offset
     * compensation needs them to locate the new mapping's kinks. */
    যদি (out_mappings)
      *out_mappings = std::move (মান_mappings);
    return_trace (সত্য);
  }

  সর্বজনীন:
  DEFINE_আকার_ARRAY (2, *এই);
};

/* One knot of the avar2 offset-compensation function
 * offset(z) = inv_renorm(z) - z, in new intermediate space. */
কাঠামো avar2_offset_knot_t
{
  দ্বিগুণ z;
  দ্বিগুণ offset;
};

/* Insert a knot keeping the vector sorted by z; first insertion wins on
 * duplicate z. */
স্থির অন্তঃরেখ শূন্য
_avar2_add_knot (hb_vector_t<avar2_offset_knot_t> &knots, দ্বিগুণ z, দ্বিগুণ offset)
{
  unsigned i = 0;
  যখন (i < knots.দৈর্ঘ্য && knots.arrayZ[i].z < z) i++;
  যদি (i < knots.দৈর্ঘ্য && knots.arrayZ[i].z == z) ফেরত;
  knots.push (avar2_offset_knot_t {0.0, 0.0});
  যদি (unlikely (knots.in_error ())) ফেরত;
  জন্য (unsigned j = knots.দৈর্ঘ্য - 1; j > i; j--)
    knots.arrayZ[j] = knots.arrayZ[j - 1];
  knots.arrayZ[i] = avar2_offset_knot_t {z, offset};
}

/* Piecewise-linear evaluation over the sorted knots. Inputs are always
 * within [-1, +1] and anchor knots at -1/0/+1 always exist. */
স্থির অন্তঃরেখ দ্বিগুণ
_avar2_eval_offset (ধ্রুবক hb_vector_t<avar2_offset_knot_t> &knots, দ্বিগুণ z)
{
  unsigned len = knots.দৈর্ঘ্য;
  জন্য (unsigned i = 0; i < len; i++)
  {
    যদি (z == knots.arrayZ[i].z) ফেরত knots.arrayZ[i].offset;
    যদি (z < knots.arrayZ[i].z)
    {
      যদি (!i) ফেরত knots.arrayZ[0].offset;
      ধ্রুবক auto &before = knots.arrayZ[i - 1];
      ধ্রুবক auto &after = knots.arrayZ[i];
      দ্বিগুণ denom = after.z - before.z;
      ফেরত before.offset + (after.offset - before.offset) * (z - before.z) / denom;
    }
  }
  ফেরত len ? knots.arrayZ[len - 1].offset : 0.0;
}

/* Piecewise-linear evaluation over instantiated avar v1 mappings, as
 * produced by SegmentMaps::subset (sorted, with -1/0/+1 anchors).
 * Matches SegmentMaps::map_float for such well-formed mappings. */
স্থির অন্তঃরেখ দ্বিগুণ
_avar2_map_new_mapping (ধ্রুবক hb_vector_t<Axis_মান_Map> &mappings, দ্বিগুণ v,
			unsigned from_offset = 0, unsigned to_offset = 1)
{
  unsigned len = mappings.দৈর্ঘ্য;
  যদি (!len) ফেরত v;

  জন্য (unsigned i = 0; i < len; i++)
  {
    দ্বিগুণ from = (দ্বিগুণ) mappings.arrayZ[i].coords[from_offset].to_float ();
    যদি (v == from)
      ফেরত (দ্বিগুণ) mappings.arrayZ[i].coords[to_offset].to_float ();
    যদি (v < from)
    {
      দ্বিগুণ to = (দ্বিগুণ) mappings.arrayZ[i].coords[to_offset].to_float ();
      যদি (!i) ফেরত v - from + to;
      দ্বিগুণ পূর্ব_from = (দ্বিগুণ) mappings.arrayZ[i - 1].coords[from_offset].to_float ();
      দ্বিগুণ পূর্ব_to = (দ্বিগুণ) mappings.arrayZ[i - 1].coords[to_offset].to_float ();
      দ্বিগুণ denom = from - পূর্ব_from;
      যদি (denom == 0.0) ফেরত পূর্ব_to;
      ফেরত পূর্ব_to + (to - পূর্ব_to) * (v - পূর্ব_from) / denom;
    }
  }
  ফেরত v - (দ্বিগুণ) mappings.arrayZ[len - 1].coords[from_offset].to_float ()
           + (দ্বিগুণ) mappings.arrayZ[len - 1].coords[to_offset].to_float ();
}

/* Inverse of _avar2_map_new_mapping: pull an output coordinate back to a
 * preimage input coordinate. For a non-strictly-monotone mapping this picks
 * one preimage, which is fine for its only use (augmenting the error
 * estimator's sample set). */
স্থির অন্তঃরেখ দ্বিগুণ
_avar2_unmap_new_mapping (ধ্রুবক hb_vector_t<Axis_মান_Map> &mappings, দ্বিগুণ v)
{
  ফেরত _avar2_map_new_mapping (mappings, v, 1, 0);
}

/* Plain (non-avar) fvar-style normalization of a user value against a
 * user-space (min, default, max) triple. */
স্থির অন্তঃরেখ দ্বিগুণ
avar2_normalize_মান (দ্বিগুণ v, দ্বিগুণ min, দ্বিগুণ def, দ্বিগুণ max)
{
  v = hb_clamp (v, min, max);
  যদি (v == def) ফেরত 0.0;
  যদি (v < def) ফেরত def == min ? 0.0 : (v - def) / (def - min);
  ফেরত def == max ? 0.0 : (v - def) / (max - def);
}

/* Inverse of the above: map a normalized value back to user space. */
স্থির অন্তঃরেখ দ্বিগুণ
avar2_denormalize_মান (দ্বিগুণ v, দ্বিগুণ min, দ্বিগুণ def, দ্বিগুণ max)
{
  যদি (v == 0.0) ফেরত def;
  ফেরত v < 0.0 ? def + v * (def - min) : def + v * (max - def);
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
স্থির অন্তঃরেখ unsigned
_avar2_estimate_offset_error (ধ্রুবক SegmentMaps &old_seg,
                              ধ্রুবক hb_vector_t<Axis_মান_Map> &new_mapping,
                              দ্বিগুণ old_min, দ্বিগুণ old_def, দ্বিগুণ old_max,
                              দ্বিগুণ new_min, দ্বিগুণ new_def, দ্বিগুণ new_max,
                              ধ্রুবক hb_vector_t<avar2_offset_knot_t> &knots)
{
  constexpr unsigned samples = 257;
  hb_vector_t<দ্বিগুণ> us;
  যদি (unlikely (!us.alloc (samples + old_seg.as_array ().দৈর্ঘ্য +
                           new_mapping.দৈর্ঘ্য + knots.দৈর্ঘ্য)))
    ফেরত UINT_MAX; /* estimator only; fail towards "worse" */
  জন্য (unsigned i = 0; i < samples; i++)
    us.push (new_min + (new_max - new_min) * i / (samples - 1));
  জন্য (ধ্রুবক auto &_ : old_seg.as_array ())
    us.push (avar2_denormalize_মান ((দ্বিগুণ) _.coords[0].to_float (),
                                       old_min, old_def, old_max));
  জন্য (ধ্রুবক auto &_ : new_mapping)
    us.push (avar2_denormalize_মান ((দ্বিগুণ) _.coords[0].to_float (),
                                       new_min, new_def, new_max));
  জন্য (ধ্রুবক auto &knot : knots)
    us.push (avar2_denormalize_মান (
               _avar2_unmap_new_mapping (new_mapping, knot.z),
               new_min, new_def, new_max));
  যদি (unlikely (us.in_error ())) ফেরত UINT_MAX;

  unsigned max_err = 0;
  জন্য (দ্বিগুণ u : us)
  {
    যদি (u < new_min || u > new_max) চালু;
    দ্বিগুণ n_old = avar2_normalize_মান (u, old_min, old_def, old_max);
    দ্বিগুণ old_final = (দ্বিগুণ) old_seg.map_float ((দশমিক) n_old);
    দ্বিগুণ n_new = avar2_normalize_মান (u, new_min, new_def, new_max);
    দ্বিগুণ z = _avar2_map_new_mapping (new_mapping, n_new);
    দ্বিগুণ new_final = z + _avar2_eval_offset (knots, z);
    পূর্ণসংখ্যা err = abs ((পূর্ণসংখ্যা) roundf ((দশমিক) (old_final * 16384.0)) -
                   (পূর্ণসংখ্যা) roundf ((দশমিক) (new_final * 16384.0)));
    যদি ((unsigned) err > max_err) max_err = err;
  }
  ফেরত max_err;
}

কাঠামো avar
{
  স্থির constexpr hb_tag_t tableTag = HB_OT_TAG_avar;

  যৌক্তিক has_তথ্য () ধ্রুবক { ফেরত version.to_int (); }

  ধ্রুবক SegmentMaps* get_segment_maps () ধ্রুবক
  { ফেরত &firstAxisSegmentMaps; }

  unsigned get_axis_গুনতি () ধ্রুবক
  { ফেরত axis_গুনতি_2; }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    যদি (!(version.sanitize (c) &&
	  hb_barrier () &&
	  (version.major == 1
#ifndef HB_NO_AVAR2
	   || version.major == 2
#endif
	   ) &&
	  c->check_struct (এই)))
      return_trace (মিথ্যা);

    ধ্রুবক SegmentMaps *map = &firstAxisSegmentMaps;
    unsigned পূর্ণসংখ্যা গুনতি = axis_গুনতি_2;
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < গুনতি; i++)
    {
      যদি (unlikely (!map->sanitize (c)))
	return_trace (মিথ্যা);
      map = &StructAfter<SegmentMaps> (*map);
    }

#ifndef HB_NO_AVAR2
    যদি (version.major < 2)
      return_trace (সত্য);
    hb_barrier ();

    ধ্রুবক auto &v2 = * (ধ্রুবক avar_V2_লেজ *) map;
    যদি (unlikely (!v2.sanitize (c, এই)))
      return_trace (মিথ্যা);
#endif

    return_trace (সত্য);
  }

  শূন্য map_coords_16_16 (পূর্ণসংখ্যা *coords, unsigned পূর্ণসংখ্যা coords_দৈর্ঘ্য) ধ্রুবক
  {
    unsigned পূর্ণসংখ্যা গুনতি = hb_min (coords_দৈর্ঘ্য, axis_গুনতি_2);

    ধ্রুবক SegmentMaps *map = &firstAxisSegmentMaps;
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < গুনতি; i++)
    {
      coords[i] = roundf (map->map_float (coords[i] / 65536.f) * 65536.f);
      map = &StructAfter<SegmentMaps> (*map);
    }

#ifndef HB_NO_AVAR2
    যদি (version.major < 2)
      ফেরত;
    hb_barrier ();

    জন্য (; গুনতি < axis_গুনতি_2; গুনতি++)
      map = &StructAfter<SegmentMaps> (*map);

    ধ্রুবক auto &v2 = * (ধ্রুবক avar_V2_লেজ *) map;

    ধ্রুবক auto &varidx_map = এই+v2.varIdxMap;
    ধ্রুবক auto &var_store = এই+v2.varStore;
    auto *var_store_cache = var_store.create_cache ();

    hb_vector_t<পূর্ণসংখ্যা> coords_2_14;
    coords_2_14.resize (coords_দৈর্ঘ্য);
    জন্য (unsigned i = 0; i < coords_দৈর্ঘ্য; i++)
      coords_2_14[i] = roundf (coords[i] / 4.f); // 16.16 -> 2.14

    hb_vector_t<পূর্ণসংখ্যা> out;
    out.alloc (coords_দৈর্ঘ্য);
    জন্য (unsigned i = 0; i < coords_দৈর্ঘ্য; i++)
    {
      পূর্ণসংখ্যা v = coords[i];
      uint32_t varidx = varidx_map.map (i);
      দশমিক delta = var_store.get_delta (varidx, coords_2_14.arrayZ, coords_2_14.দৈর্ঘ্য, var_store_cache);
      /* Apply the delta unclamped and clamp only the result to [-1, +1],
       * matching fontTools. Since inputs and results are in [-1, +1],
       * deltas beyond ±2 are equivalent to ±2; clamp to that range only
       * to keep the float->int conversion safe. */
      দশমিক d = hb_clamp (delta * 4, -(দশমিক) (1<<17), +(দশমিক) (1<<17)); // 2.14 -> 16.16
      v += (পূর্ণসংখ্যা) roundf (d);
      v = hb_clamp (v, -(1<<16), +(1<<16));
      out.push (v);
    }
    জন্য (unsigned i = 0; i < coords_দৈর্ঘ্য; i++)
      coords[i] = out[i];

    OT::ItemVariationStore::destroy_cache (var_store_cache);
#endif
  }

  /* An avar version 2 font is never downgraded to v1: even with a NULL
   * varStore offset (legal; maps like plain v1) the avar2 subsetting path
   * applies, with "no variation" semantics for rows that don't resolve —
   * offset compensation then creates delta rows on demand. */
  যৌক্তিক has_v2_তথ্য () ধ্রুবক { ফেরত version.major > 1; }

  /* Resolve the avar2 VarStore and VarIdxMap, for the subset planner.
   * Either pointer may be to the Null object (nullable offsets). */
  যৌক্তিক get_v2_store_and_map (ধ্রুবক ItemVariationStore **store,
                             ধ্রুবক Delta_Set_সূচক_Map **varidx_map) ধ্রুবক
  {
#ifndef HB_NO_AVAR2
    যদি (version.major < 2) ফেরত মিথ্যা;
    ধ্রুবক SegmentMaps *map = &firstAxisSegmentMaps;
    জন্য (unsigned i = 0; i < axis_গুনতি_2; i++)
      map = &StructAfter<SegmentMaps> (*map);
    ধ্রুবক auto &v2 = * (ধ্রুবক avar_V2_লেজ *) map;
    *store = &(এই+v2.varStore);
    *varidx_map = &(এই+v2.varIdxMap);
    ফেরত সত্য;
#অন্যথা
    ফেরত মিথ্যা;
#endif
  }

  // axis normalization is done in 2.14 here
  // TODO: deprecate this API once fonttools is updated to use 16.16 normalization
  যৌক্তিক map_coords_2_14 (দশমিক *coords, unsigned পূর্ণসংখ্যা coords_দৈর্ঘ্য,
			যৌক্তিক v1_only = মিথ্যা) ধ্রুবক
  {
    hb_vector_t<পূর্ণসংখ্যা> coords_2_14;
    যদি (!v1_only && !coords_2_14.resize (coords_দৈর্ঘ্য)) ফেরত মিথ্যা;
    unsigned পূর্ণসংখ্যা গুনতি = hb_min (coords_দৈর্ঘ্য, axis_গুনতি_2);

    ধ্রুবক SegmentMaps *map = &firstAxisSegmentMaps;
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < গুনতি; i++)
    {
      পূর্ণসংখ্যা v = roundf (map->map_float (coords[i]) * 16384.f);
      যদি (!v1_only)
	coords_2_14[i] = v;
      coords[i] = v / 16384.f;
      map = &StructAfter<SegmentMaps> (*map);
    }

    যদি (v1_only)
      ফেরত সত্য;

#ifndef HB_NO_AVAR2
    যদি (version.major < 2)
      ফেরত সত্য;
    hb_barrier ();

    জন্য (; গুনতি < axis_গুনতি_2; গুনতি++)
      map = &StructAfter<SegmentMaps> (*map);

    ধ্রুবক auto &v2 = * (ধ্রুবক avar_V2_লেজ *) map;

    ধ্রুবক auto &varidx_map = এই+v2.varIdxMap;
    ধ্রুবক auto &var_store = এই+v2.varStore;
    auto *var_store_cache = var_store.create_cache ();

    জন্য (unsigned i = 0; i < coords_দৈর্ঘ্য; i++)
    {
      পূর্ণসংখ্যা v = coords_2_14[i];
      uint32_t varidx = varidx_map.map (i);
      দশমিক delta = var_store.get_delta (varidx, coords_2_14.arrayZ, coords_2_14.দৈর্ঘ্য, var_store_cache);
      /* As above: apply the delta unclamped (±2 covers every useful case)
       * and clamp the result to [-1, +1], matching fontTools. */
      v += (পূর্ণসংখ্যা) hb_clamp (roundf (delta), -(দশমিক) (1<<15), +(দশমিক) (1<<15));
      v = hb_clamp (v, -(1<<14), +(1<<14));
      coords[i] = v / 16384.f;
    }

    OT::ItemVariationStore::destroy_cache (var_store_cache);
    ফেরত সত্য;
#অন্যথা
    ফেরত version.major < 2;
#endif
  }

  যৌক্তিক subset (hb_subset_context_t *c) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    unsigned retained_axis_গুনতি = c->plan->axes_সূচক_map.get_population ();
    যদি (!retained_axis_গুনতি) //all axes are pinned/dropped
      return_trace (মিথ্যা);

    avar *out = c->serializer->allocate_min<avar> ();
    যদি (unlikely (!out)) return_trace (মিথ্যা);

    out->version.major = c->plan->has_avar2 ? 2 : 1;
    out->version.minor = 0;
    যদি (!c->serializer->check_assign (out->axis_গুনতি_2, retained_axis_গুনতি, HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (মিথ্যা);

    /* For avar2, keep the instantiated v1 mappings around; offset
     * compensation needs them to locate the new mappings' kinks. */
    hb_vector_t<hb_vector_t<Axis_মান_Map>> new_mappings;
    যদি (c->plan->has_avar2 && !new_mappings.resize (axis_গুনতি_2))
      return_trace (মিথ্যা);

    ধ্রুবক hb_map_t& axes_সূচক_map = c->plan->axes_সূচক_map;
    ধ্রুবক SegmentMaps *map = &firstAxisSegmentMaps;
    unsigned গুনতি = axis_গুনতি_2;
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < গুনতি; i++)
    {
      যদি (axes_সূচক_map.has (i))
      {
        hb_tag_t *axis_tag;
        যদি (!c->plan->axes_old_সূচক_tag_map.has (i, &axis_tag))
          return_trace (মিথ্যা);

        Triple *axis_location;
        যদি (c->plan->has_avar2 &&
            c->plan->user_axes_location.has (*axis_tag, &axis_location) &&
            axis_location->is_point ())
        {
          /* Pinned axis in avar2 mode: serialize identity segment map
           * {-1->-1, 0->0, 1->1}. The axis is kept in fvar as hidden,
           * so avar needs a segment map entry for it. */
          auto *identity_map = c->serializer->start_embed<SegmentMaps> ();
          যদি (unlikely (!c->serializer->extend_min (identity_map)))
            return_trace (মিথ্যা);
          Axis_মান_Map m;
          m.set_mapping (-1.f, -1.f);
          যদি (!m.serialize (c->serializer)) return_trace (মিথ্যা);
          m.set_mapping (0.f, 0.f);
          যদি (!m.serialize (c->serializer)) return_trace (মিথ্যা);
          m.set_mapping (1.f, 1.f);
          যদি (!m.serialize (c->serializer)) return_trace (মিথ্যা);
          যদি (!c->serializer->check_assign (identity_map->len, 3u,
                                            HB_SERIALIZE_ERROR_INT_OVERFLOW))
            return_trace (মিথ্যা);
        }
        অন্যথা
        {
          /* Restricted or free axis: use standard SegmentMaps::subset() */
          যদি (!map->subset (c, *axis_tag,
                            c->plan->has_avar2 ? &new_mappings[i] : nullptr))
            return_trace (মিথ্যা);
        }
      }
      map = &StructAfter<SegmentMaps> (*map);
    }

    যদি (c->plan->has_avar2)
      return_trace (_subset_avar2 (c, new_mappings));

    return_trace (সত্য);
  }

  ব্যক্তিগত:
  কাঠামো avar2_সূচক_map_plan_t
  {
    যৌক্তিক init (ধ্রুবক hb_vector_t<uint32_t> &varidx_mapping,
	       ধ্রুবক hb_map_t &axes_সূচক_map,
	       unsigned axis_গুনতি)
    {
      যদি (!output_map.alloc (axes_সূচক_map.get_population ()))
	ফেরত মিথ্যা;

      যৌক্তিক has_no_variation = মিথ্যা;
      unsigned max_outer = 0, max_inner = 0;
      জন্য (unsigned i = 0; i < axis_গুনতি; i++)
      {
	যদি (!axes_সূচক_map.has (i)) চালু;
	uint32_t varidx = varidx_mapping[i];
	output_map.push (varidx);
	যদি (varidx == HB_OT_LAYOUT_NO_VARIATIONS_সূচক)
	{
	  has_no_variation = সত্য;
	  চালু;
	}
	max_outer = hb_max (max_outer, varidx >> 16);
	max_inner = hb_max (max_inner, varidx & 0xFFFF);
      }
      যদি (output_map.in_error () ||
	  output_map.দৈর্ঘ্য != axes_সূচক_map.get_population ())
	ফেরত মিথ্যা;

      যদি (has_no_variation)
      {
	width = 4;
	inner_bit_গুনতি = 16;
      }
      অন্যথা
      {
	inner_bit_গুনতি = hb_max (1u, hb_bit_storage (max_inner));
	unsigned outer_bit_গুনতি = hb_max (1u, hb_bit_storage (max_outer));
	width = hb_clamp ((inner_bit_গুনতি + outer_bit_গুনতি + 7) / 8, 1u, 4u);
	যদি (inner_bit_গুনতি + outer_bit_গুনতি > width * 8)
	  inner_bit_গুনতি = width * 8 - outer_bit_গুনতি;
      }
      ফেরত সত্য;
    }

    unsigned get_inner_bit_গুনতি () ধ্রুবক { ফেরত inner_bit_গুনতি; }
    unsigned get_width () ধ্রুবক { ফেরত width; }
    hb_array_t<ধ্রুবক uint32_t> get_output_map () ধ্রুবক { ফেরত output_map.as_array (); }

    unsigned inner_bit_গুনতি = 1;
    unsigned width = 1;
    hb_vector_t<uint32_t> output_map;
  };

  যৌক্তিক _subset_avar2 (hb_subset_context_t *c,
                      ধ্রুবক hb_vector_t<hb_vector_t<Axis_মান_Map>> &new_mappings) ধ্রুবক
  {
#যদি defined (HB_NO_VAR) || defined (HB_NO_AVAR2)
    /* Not reachable: the plan never sets has_avar2 in these configurations. */
    ফেরত মিথ্যা;
#অন্যথা

    /* 1. Locate original avar2 data, keeping per-axis old segment maps */
    hb_vector_t<ধ্রুবক SegmentMaps *> old_seg_maps;
    যদি (!old_seg_maps.alloc (axis_গুনতি_2)) ফেরত মিথ্যা;
    ধ্রুবক SegmentMaps *map = &firstAxisSegmentMaps;
    জন্য (unsigned i = 0; i < axis_গুনতি_2; i++)
    {
      old_seg_maps.push (map);
      map = &StructAfter<SegmentMaps> (*map);
    }

    ধ্রুবক auto &v2 = * (ধ্রুবক avar_V2_লেজ *) map;
    ধ্রুবক auto &varidx_map = এই+v2.varIdxMap;
    ধ্রুবক auto &var_store = এই+v2.varStore;

    auto fvar_axes = c->plan->source->table.fvar->get_axes ();

    /* 2. Compute default deltas by evaluating VarStore at old defaults */
    hb_vector_t<পূর্ণসংখ্যা> default_coords;
    যদি (!default_coords.resize (axis_গুনতি_2)) ফেরত মিথ্যা;
    জন্য (unsigned i = 0; i < axis_গুনতি_2; i++)
    {
      hb_tag_t *axis_tag;
      যদি (c->plan->axes_old_সূচক_tag_map.has (i, &axis_tag) &&
          c->plan->old_intermediates.has (*axis_tag))
      {
        দশমিক d_i = (দশমিক) c->plan->old_intermediates.get (*axis_tag).মধ্য;
        default_coords[i] = roundf (d_i * 16384.f);
      }
      অন্যথা
        default_coords[i] = 0;
    }

    auto *store_cache = var_store.create_cache ();
    hb_vector_t<দশমিক> default_deltas;
    যদি (!default_deltas.resize (axis_গুনতি_2)) {
      ItemVariationStore::destroy_cache (store_cache);
      ফেরত মিথ্যা;
    }
    জন্য (unsigned i = 0; i < axis_গুনতি_2; i++)
    {
      uint32_t varidx = varidx_map.map (i);
      যদি (varidx == HB_OT_LAYOUT_NO_VARIATIONS_সূচক)
        default_deltas[i] = 0.f;
      অন্যথা
        default_deltas[i] = var_store.get_delta (varidx, default_coords.arrayZ,
                                                  default_coords.দৈর্ঘ্য, store_cache);
    }
    ItemVariationStore::destroy_cache (store_cache);

    /* 3. Rebase IVS regions */
    item_variations_t item_vars;
    যদি (!item_vars.create_from_item_varstore (var_store, c->plan->axes_old_সূচক_tag_map))
      ফেরত মিথ্যা;
    যদি (!item_vars.instantiate_tuple_vars (c->plan->old_intermediates,
                                           c->plan->axes_triple_distances,
                                           মিথ্যা))
      ফেরত মিথ্যা;

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
    যদি (!new_varidx_mapping.resize (axis_গুনতি_2)) ফেরত মিথ্যা;
    জন্য (unsigned i = 0; i < axis_গুনতি_2; i++)
    {
      uint32_t varidx = varidx_map.map (i);
      যদি (varidx != HB_OT_LAYOUT_NO_VARIATIONS_সূচক &&
	  !var_store.has_delta_set (varidx))
	varidx = HB_OT_LAYOUT_NO_VARIATIONS_সূচক;
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
    hb_hashmap_t<uint32_t, unsigned> varidx_ref_গুনতি;
    জন্য (unsigned i = 0; i < axis_গুনতি_2; i++)
    {
      যদি (!c->plan->axes_সূচক_map.has (i)) চালু; /* self-contained: dropped */
      uint32_t varidx = new_varidx_mapping[i];
      যদি (varidx == HB_OT_LAYOUT_NO_VARIATIONS_সূচক) চালু;
      unsigned *গুনতি;
      যদি (varidx_ref_গুনতি.has (varidx, &গুনতি))
        (*গুনতি)++;
      অন্যথা যদি (!varidx_ref_গুনতি.set (varidx, 1))
        ফেরত মিথ্যা;
    }
    জন্য (unsigned i = 0; i < axis_গুনতি_2; i++)
    {
      hb_tag_t *axis_tag_ptr;
      যদি (!c->plan->axes_old_সূচক_tag_map.has (i, &axis_tag_ptr))
        ফেরত মিথ্যা;
      /* Only axes that will receive offset compensation (restricted or
       * pinned) can contaminate a shared row. */
      যদি (!c->plan->axes_সূচক_map.has (i) ||
          !c->plan->user_axes_location.has (*axis_tag_ptr))
        চালু;
      uint32_t varidx = new_varidx_mapping[i];
      যদি (varidx == HB_OT_LAYOUT_NO_VARIATIONS_সূচক)
        চালু; /* gets a fresh, private VarData in the offset loop below */
      unsigned *গুনতি;
      যদি (!varidx_ref_গুনতি.has (varidx, &গুনতি) || *গুনতি <= 1)
        চালু; /* sole owner: safe to write offsets in place */
      unsigned outer = varidx >> 16;
      unsigned new_inner = item_vars.duplicate_row (outer, varidx & 0xFFFF);
      যদি (unlikely (new_inner == (unsigned) -1)) ফেরত মিথ্যা;
      new_varidx_mapping[i] = (outer << 16) | new_inner;
      (*গুনতি)--;
    }

    /* 6. Add offset compensation tuples.
     * Track processed (outer,inner) pairs to avoid adding duplicate biases
     * when multiple axes share the same varIdx. */
    hb_set_t processed_varidxes;
    জন্য (unsigned i = 0; i < axis_গুনতি_2; i++)
    {
      hb_tag_t *axis_tag_ptr;
      যদি (!c->plan->axes_old_সূচক_tag_map.has (i, &axis_tag_ptr))
        ফেরত মিথ্যা;
      hb_tag_t axis_tag = *axis_tag_ptr;

      /* Self-contained pinned axes are removed from fvar/avar; their
       * contribution is baked into the variation tables instead. */
      যদি (!c->plan->axes_সূচক_map.has (i))
        চালু;

      Triple *new_user;
      যদি (c->plan->user_axes_location.has (axis_tag, &new_user))
      {
        /* This axis is being restricted or pinned */
        Triple *old_int;
        যদি (!c->plan->old_intermediates.has (axis_tag, &old_int))
          ফেরত মিথ্যা;

        দশমিক a_i = (দশমিক) old_int->minimum;
        দশমিক d_i = (দশমিক) old_int->মধ্য;
        দশমিক b_i = (দশমিক) old_int->maximum;

        পূর্ণসংখ্যা d_int = roundf (d_i * 16384.f);

        যৌক্তিক is_pinned = new_user->is_point ();

        uint32_t varidx = new_varidx_mapping[i];
        unsigned outer, inner, item_গুনতি_2;

        যদি (varidx == HB_OT_LAYOUT_NO_VARIATIONS_সূচক)
        {
          /* No existing avar2 mapping. Create new VarData. */
          outer = item_vars.add_vardata (1);
          inner = 0;
          item_গুনতি_2 = 1;
          new_varidx_mapping[i] = (outer << 16) | inner;
          default_deltas[i] = 0.f; /* no prior default delta */
        }
        অন্যথা
        {
          outer = varidx >> 16;
          inner = varidx & 0xFFFF;
          item_গুনতি_2 = item_vars.get_item_গুনতি (outer);
        }

        /* Empty-region bias: d_int + round(defaultDelta) */
        পূর্ণসংখ্যা bias = d_int + (পূর্ণসংখ্যা) roundf (default_deltas[i]);
        যদি (bias != 0)
        {
          hb_hashmap_t<hb_tag_t, Triple> empty_region;
          item_vars.add_tuple (outer, std::move (empty_region),
                               inner, bias, item_গুনতি_2);
        }

        যদি (!is_pinned)
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
          _avar2_add_knot (knots, -1.0, (দ্বিগুণ) a_i + 1.0);
          _avar2_add_knot (knots, 0.0, (দ্বিগুণ) d_i);
          _avar2_add_knot (knots, 1.0, (দ্বিগুণ) b_i - 1.0);

          ধ্রুবক hb_vector_t<Axis_মান_Map> &new_mapping = new_mappings[i];

          দশমিক min_f = 0.f, def_f = 0.f, max_f = 0.f;
          যদি (likely (i < fvar_axes.দৈর্ঘ্য))
            fvar_axes[i].get_coordinates (min_f, def_f, max_f);
          দ্বিগুণ old_min = (দ্বিগুণ) min_f;
          দ্বিগুণ old_def = (দ্বিগুণ) def_f;
          দ্বিগুণ old_max = (দ্বিগুণ) max_f;

          /* If the axis default MOVED, inv_renorm also kinks where the OLD
           * default lands in the new space (the old intermediate coordinate
           * crosses 0 there), at
           *   z = z_old  ->  -z_old
           * Omitting that knot (as a plain two-tent encoding would) makes
           * interior coordinates wrong. */
          দ্বিগুণ z_old = avar2_normalize_মান ((দ্বিগুণ) old_def,
                                                 (দ্বিগুণ) new_user->minimum,
                                                 (দ্বিগুণ) new_user->মধ্য,
                                                 (দ্বিগুণ) new_user->maximum);
          z_old = _avar2_map_new_mapping (new_mapping, z_old);
          z_old = (দ্বিগুণ) roundf ((দশমিক) (z_old * 16384.0)) / 16384.0;
          যদি (-1.0 < z_old && z_old < 1.0)
            _avar2_add_knot (knots, z_old, -z_old);

          /* Interior avar v1 breakpoints inside the retained range each put
           * a kink in offset(z). Sampling only {-1, 0, +1, z_old} would
           * linearly interpolate across those kinks. The instantiated
           * mapping keeps exactly the in-range old breakpoints, and the new
           * mapping kinks at each one's output coordinate; add that z with
           * its old intermediate value so offset(z) is reproduced at every
           * kink. */
          hb_vector_t<avar2_offset_knot_t> with_breakpoints (knots);
          জন্য (ধ্রুবক auto &m : new_mapping)
          {
            দ্বিগুণ from = (দ্বিগুণ) m.coords[0].to_float ();
            যদি (from == -1.0 || from == 0.0 || from == 1.0)
              চালু; /* anchors already seeded */
            দ্বিগুণ z = (দ্বিগুণ) m.coords[1].to_float ();
            যদি (!(-1.0 < z && z < 1.0))
              চালু;
            দ্বিগুণ user = avar2_denormalize_মান (from,
                                                    (দ্বিগুণ) new_user->minimum,
                                                    (দ্বিগুণ) new_user->মধ্য,
                                                    (দ্বিগুণ) new_user->maximum);
            দ্বিগুণ n_old = avar2_normalize_মান (user, old_min, old_def, old_max);
            দ্বিগুণ x_old = (দ্বিগুণ) old_seg_maps[i]->map_float ((দশমিক) n_old);
            x_old = (দ্বিগুণ) roundf ((দশমিক) (x_old * 16384.0)) / 16384.0;
            _avar2_add_knot (with_breakpoints, z, x_old - z);
          }

          যদি (unlikely (knots.in_error () || with_breakpoints.in_error ()))
            ফেরত মিথ্যা;

          /* Extra tents cost F2Dot14 rounding, so for a steep segment they
           * can add more quantization noise than the structural error they
           * remove. Keep the interior breakpoints only when they do not
           * increase the estimated residual; this makes the collection a
           * strict (never-worse) improvement over the {-1, 0, +1, z_old}
           * anchors. Warn when even the better choice is not bit-exact (a
           * steep retained segment that cannot be reproduced in F2Dot14). */
          unsigned err = _avar2_estimate_offset_error (*old_seg_maps[i], new_mapping,
                                                       old_min, old_def, old_max,
                                                       (দ্বিগুণ) new_user->minimum,
                                                       (দ্বিগুণ) new_user->মধ্য,
                                                       (দ্বিগুণ) new_user->maximum,
                                                       knots);
          যদি (with_breakpoints.দৈর্ঘ্য != knots.দৈর্ঘ্য)
          {
            unsigned err_with = _avar2_estimate_offset_error (*old_seg_maps[i], new_mapping,
                                                              old_min, old_def, old_max,
                                                              (দ্বিগুণ) new_user->minimum,
                                                              (দ্বিগুণ) new_user->মধ্য,
                                                              (দ্বিগুণ) new_user->maximum,
                                                              with_breakpoints);
            যদি (err_with <= err)
            {
              knots = std::move (with_breakpoints);
              err = err_with;
            }
          }
          যদি (err > 8)
            DEBUG_বার্তা (SUBSET, nullptr,
                       "avar2 offset compensation is approximate for axis %c%c%c%c: "
                       "max residual %u F2Dot14 units",
                       HB_UNTAG (axis_tag), err);

          /* Synthesize tents. Adjacent tents evaluate to zero at each
           * other's peaks, so each knot's delta is offset(z) - offset(0);
           * the base value offset(0) = d_i is carried by the empty-region
           * bias above. This reduces to the classic pair of tents
           * (-1,-1,0) / (0,+1,+1) when the default is unchanged and there
           * are no interior knots. */
          জন্য (unsigned k = 0; k < knots.দৈর্ঘ্য; k++)
          {
            দ্বিগুণ z = knots.arrayZ[k].z;
            যদি (z == 0.0) চালু;
            পূর্ণসংখ্যা delta = (পূর্ণসংখ্যা) roundf ((দশমিক) ((knots.arrayZ[k].offset - (দ্বিগুণ) d_i) * 16384.0));
            যদি (!delta) চালু;
            দ্বিগুণ lower, upper;
            যদি (z > 0.0)
            {
              lower = knots.arrayZ[k - 1].z;
              upper = k + 1 < knots.দৈর্ঘ্য ? knots.arrayZ[k + 1].z : z;
            }
            অন্যথা
            {
              upper = knots.arrayZ[k + 1].z;
              lower = k > 0 ? knots.arrayZ[k - 1].z : z;
            }
            hb_hashmap_t<hb_tag_t, Triple> region;
            যদি (unlikely (!region.set (axis_tag, Triple (lower, z, upper))))
              ফেরত মিথ্যা;
            item_vars.add_tuple (outer, std::move (region),
                                 inner, delta, item_গুনতি_2);
          }
        }
      }
      অন্যথা
      {
        /* Free or private axis — not being restricted.
         * If it has a non-zero default delta, add it back as a bias.
         * Skip if this (outer,inner) was already processed (shared varIdx). */
        uint32_t varidx = new_varidx_mapping[i];
        যদি (varidx == HB_OT_LAYOUT_NO_VARIATIONS_সূচক)
          চালু;

        যদি (processed_varidxes.has (varidx))
          চালু;
        processed_varidxes.add (varidx);

        unsigned outer = varidx >> 16;
        unsigned inner = varidx & 0xFFFF;
        পূর্ণসংখ্যা dd = (পূর্ণসংখ্যা) roundf (default_deltas[i]);
        যদি (dd != 0)
        {
          unsigned item_গুনতি_2 = item_vars.get_item_গুনতি (outer);
          hb_hashmap_t<hb_tag_t, Triple> empty_region;
          item_vars.add_tuple (outer, std::move (empty_region),
                               inner, dd, item_গুনতি_2);
        }
      }
    }

    /* 7. Finalize: build region list + convert to varstore */
    যদি (!item_vars.build_region_list ()) ফেরত মিথ্যা;
    যদি (!item_vars.as_item_varstore (সত্য /* optimize */,
                                     মিথ্যা /* use_no_variation_idx */))
      ফেরত মিথ্যা;

    /* 8. Apply varidx_map optimization remapping */
    ধ্রুবক auto &opt_varidx_map = item_vars.get_varidx_map ();
    জন্য (unsigned i = 0; i < axis_গুনতি_2; i++)
    {
      uint32_t varidx = new_varidx_mapping[i];
      যদি (varidx == HB_OT_LAYOUT_NO_VARIATIONS_সূচক)
        চালু;
      uint32_t *new_idx;
      যদি (opt_varidx_map.has (varidx, &new_idx))
        new_varidx_mapping[i] = *new_idx;
    }

    /* 9. Serialize avarV2Tail. Entries cover the retained axes only;
     * self-contained pinned axes are removed from fvar. */
    avar2_সূচক_map_plan_t সূচক_map_plan;
    যদি (!সূচক_map_plan.init (new_varidx_mapping,
			      c->plan->axes_সূচক_map,
			      axis_গুনতি_2))
      ফেরত মিথ্যা;

    auto *লেজ = c->serializer->allocate_আকার<avar_V2_লেজ> (avar_V2_লেজ::static_আকার);
    যদি (unlikely (!লেজ)) ফেরত মিথ্যা;
    যদি (!লেজ->varIdxMap.serialize_serialize (c->serializer, সূচক_map_plan))
      ফেরত মিথ্যা;
    যদি (!লেজ->varStore.serialize_serialize (c->serializer,
					     item_vars.has_long_word (),
					     c->plan->axis_tags,
					     item_vars.get_region_list (),
					     item_vars.get_vardata_encodings ()))
      ফেরত মিথ্যা;

    ফেরত সত্য;
#endif
  }

  সর্বজনীন:

  সুরক্ষিত:
  FixedVersion<>version;	/* Version of the avar table
				 * initially set to 0x00010000u */
  HBUINT16	reserved;	/* This field is permanently reserved. Set to 0. */
  HBUINT16	axis_গুনতি_2;	/* The number of variation axes in the font. This
				 * must be the same number as axisCount in the
				 * 'fvar' table. */
  SegmentMaps	firstAxisSegmentMaps;

  সর্বজনীন:
  DEFINE_আকার_MIN (8);
};

} /* namespace OT */


#endif /* HB_OT_VAR_AVAR_TABLE_HH */
