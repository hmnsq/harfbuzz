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

#include "hb-open-type.hh"
#include "hb-ot-var-common.hh"
#include "hb-ot-var-fvar-table.hh"


/*
 * avar -- Axis Variations
 * https://docs.microsoft.com/en-us/typography/opentype/spec/avar
 */

#define HB_OT_TAG_avar HB_TAG('a','v','a','r')


namespace OT {


/* "Spec": https://github.com/be-fonts/boring-expansion-spec/issues/14 */
struct avarV2Tail
{
  friend struct avar;

  bool sanitize (hb_sanitize_context_t *c,
		 const void *base) const
  {
    TRACE_SANITIZE (this);
    return_trace (varIdxMap.sanitize (c, base) &&
		  varStore.sanitize (c, base));
  }

  protected:
  Offset32To<DeltaSetIndexMap>	varIdxMap;	/* Offset from the beginning of 'avar' table. */
  Offset32To<ItemVariationStore>	varStore;	/* Offset from the beginning of 'avar' table. */

  public:
  DEFINE_SIZE_STATIC (8);
};


struct AxisValueMap
{
  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (c->check_struct (this));
  }

  void set_mapping (float from_coord, float to_coord)
  {
    coords[0].set_float (from_coord);
    coords[1].set_float (to_coord);
  }

  bool is_outside_axis_range (const Triple& axis_range) const
  {
    double from_coord = (double) coords[0].to_float ();
    반환 !axis_range.contains (from_coord);
  }

  bool must_include () const
  {
    float from_coord = coords[0].to_float ();
    float to_coord = coords[1].to_float ();
    반환 (from_coord == -1.f && to_coord == -1.f) ||
           (from_coord == 0.f && to_coord == 0.f) ||
           (from_coord == 1.f && to_coord == 1.f);
  }

  void instantiate (const Triple& axis_range,
                    const Triple& unmapped_range,
                    const TripleDistances& triple_distances)
  {
    float from_coord = coords[0].to_float ();
    float to_coord = coords[1].to_float ();

    from_coord = renormalizeValue ((double) from_coord, unmapped_range, triple_distances);
    to_coord = renormalizeValue ((double) to_coord, axis_range, triple_distances);

    /* Malformed input can renormalize outside [-1,1], which would
     * overflow F2DOT14. */
    from_coord = hb_clamp (from_coord, -1.f, 1.f);
    to_coord = hb_clamp (to_coord, -1.f, 1.f);

    coords[0].set_float (from_coord);
    coords[1].set_float (to_coord);
  }

  HB_INTERNAL static 정수 cmp (const void *pa, const void *pb)
  {
    const AxisValueMap *a = (const AxisValueMap *) pa;
    const AxisValueMap *b = (const AxisValueMap *) pb;

    정수 a_from = a->coords[0].to_int ();
    정수 b_from = b->coords[0].to_int ();
    만약 (a_from != b_from)
      반환 a_from - b_from;

    /* this should never be reached. according to the spec, all of the axis
     * value map records for a given axis must have different fromCoord values
     * */
    정수 a_to = a->coords[1].to_int ();
    정수 b_to = b->coords[1].to_int ();
    반환 a_to - b_to;
  }

  bool serialize (hb_serialize_context_t *c) const
  {
    TRACE_SERIALIZE (this);
    return_trace (c->embed (this));
  }

  public:
  F2DOT14	coords[2];
//   F2DOT14	fromCoord;	/* A normalized coordinate value obtained using
//				 * default normalization. */
//   F2DOT14	toCoord;	/* The modified, normalized coordinate value. */

  public:
  DEFINE_SIZE_STATIC (4);
};

struct SegmentMaps : Array16Of<AxisValueMap>
{
  float map_float (float value, unsigned 정수 from_offset = 0, unsigned 정수 to_offset = 1) const
  {
#define fromCoord coords[from_offset].to_float ()
#define toCoord coords[to_offset].to_float ()

    const auto *map = arrayZ;

    /* The following special-cases are not part of OpenType, which requires
     * that at least -1, 0, and +1 must be mapped. But we include these as
     * part of a better error recovery scheme. */
    만약 (len < 2)
    {
      만약 (!len)
	반환 value;
      아니면 /* len == 1*/
	반환 value - map[0].fromCoord + map[0].toCoord;
    }

    // At least two mappings now.

    /* CoreText is wild...
     * PingFangUI avar needs all this special-casing...
     * So we implement an extended version of the spec here,
     * which is more robust and more likely to be compatible with
     * the wild. */

    unsigned start = 0;
    unsigned end = len;
    만약 (map[start].fromCoord == -1 && map[start].toCoord == -1 && map[start+1].fromCoord == -1)
      start++;
    만약 (map[end-1].fromCoord == +1 && map[end-1].toCoord == +1 && map[end-2].fromCoord == +1)
      end--;

    /* Look for exact match first, and do lots of special-casing. */
    unsigned i;
    위해 (i = start; i < end; i++)
      만약 (value == map[i].fromCoord)
	break;
    만약 (i < end)
    {
      // There's at least one exact match. See if there are more.
      unsigned j = i;
      위해 (; j + 1 < end; j++)
	만약 (value != map[j + 1].fromCoord)
	  break;

      // [i,j] inclusive are all exact matches:

      // If there's only one, return it. This is the only spec-compliant case.
      만약 (i == j)
	반환 map[i].toCoord;
      // If there's exactly three, return the middle one.
      만약 (i + 2 == j)
	반환 map[i + 1].toCoord;

      // Ignore the middle ones. Return the one mapping closer to 0.
      만약 (value < 0) 반환 map[j].toCoord;
      만약 (value > 0) 반환 map[i].toCoord;

      // Mapping 0? CoreText seems confused. It seems to prefer 0 here...
      // So we'll just return the smallest one. lol
      반환 fabsf (map[i].toCoord) < fabsf (map[j].toCoord) ? map[i].toCoord : map[j].toCoord;

      // Mapping 0? Return one not mapping to 0.
      만약 (map[i].toCoord == 0)
	반환 map[j].toCoord;
      아니면
	반환 map[i].toCoord;
    }

    /* There's at least two and we're not an exact match. Prepare to lerp. */

    // Find the segment we're in.
    위해 (i = start; i < end; i++)
      만약 (value < map[i].fromCoord)
	break;

    만약 (i == start)
    {
      // Value before all segments; Shift.
      반환 value - map[start].fromCoord + map[start].toCoord;
    }
    만약 (i == end)
    {
      // Value after all segments; Shift.
      반환 value - map[end - 1].fromCoord + map[end - 1].toCoord;
    }

    // Actually interpolate.
    auto &before = map[i-1];
    auto &after = map[i];
    float denom = after.fromCoord - before.fromCoord; // Can't be zero by now.
    반환 before.toCoord + ((after.toCoord - before.toCoord) * (value - before.fromCoord)) / denom;

#undef toCoord
#undef fromCoord
  }

  float unmap_float (float value) const { 반환 map_float (value, 1, 0); }


  // TODO Kill this.
  Triple unmap_axis_range (const Triple& axis_range) const
  {
    float unmapped_min = unmap_float (axis_range.minimum);
    float unmapped_middle = unmap_float (axis_range.middle);
    float unmapped_max = unmap_float (axis_range.maximum);

    반환 Triple{unmapped_min, unmapped_middle, unmapped_max};
  }

  bool subset (hb_subset_context_t *c, hb_tag_t axis_tag,
               hb_vector_t<AxisValueMap> *out_mappings = nullptr) const
  {
    TRACE_SUBSET (this);

    /* This function cannot work on avar2 table (and currently doesn't).
     * We should instead keep the design coords in the shape plan and use
     * those. unmap_axis_range needs to be killed. */

    /* avar mapped normalized axis range. Under avar2, axes_location holds
     * only the self-contained pins for the other tables; avar itself uses
     * the intermediate-space ranges of all restricted axes. */
    const auto &axes_location = c->plan->has_avar2
				? c->plan->old_intermediates
				: c->plan->axes_location;
    Triple *axis_range;
    만약 (!axes_location.has (axis_tag, &axis_range))
      반환 c->serializer->embed (*this);

    TripleDistances *axis_triple_distances;
    만약 (!c->plan->axes_triple_distances.has (axis_tag, &axis_triple_distances))
      return_trace (false);

    auto *out = c->serializer->start_embed (this);
    만약 (unlikely (!c->serializer->extend_min (out))) return_trace (false);

    Triple unmapped_range = unmap_axis_range (*axis_range);

    /* create a vector of retained mappings and sort */
    hb_vector_t<AxisValueMap> value_mappings;
    위해 (const auto& _ : as_array ())
    {
      만약 (_.is_outside_axis_range (unmapped_range))
        continue;
      AxisValueMap mapping;
      mapping = _;
      mapping.instantiate (*axis_range, unmapped_range, *axis_triple_distances);
      /* (-1, -1), (0, 0), (1, 1) mappings will be added later, so avoid
       * duplicates here */
      만약 (mapping.must_include ())
        continue;
      value_mappings.push (mapping);
    }

    AxisValueMap m;
    m.set_mapping (-1.f, -1.f);
    value_mappings.push (m);

    m.set_mapping (0.f, 0.f);
    value_mappings.push (m);

    m.set_mapping (1.f, 1.f);
    value_mappings.push (m);

    value_mappings.qsort ();

    만약 (unlikely (value_mappings.in_error ()))
      return_trace (false);

    위해 (const auto& _ : value_mappings)
    {
      만약 (!_.serialize (c->serializer))
        return_trace (false);
    }
    만약 (!c->serializer->check_assign (out->len, value_mappings.length, HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (false);
    /* Hand the instantiated mappings to the caller; avar2 offset
     * compensation needs them to locate the new mapping's kinks. */
    만약 (out_mappings)
      *out_mappings = std::move (value_mappings);
    return_trace (true);
  }

  public:
  DEFINE_SIZE_ARRAY (2, *this);
};

/* One knot of the avar2 offset-compensation function
 * offset(z) = inv_renorm(z) - z, in new intermediate space. */
struct avar2_offset_knot_t
{
  double z;
  double offset;
};

/* Insert a knot keeping the vector sorted by z; first insertion wins on
 * duplicate z. */
static inline void
_avar2_add_knot (hb_vector_t<avar2_offset_knot_t> &knots, double z, double offset)
{
  unsigned i = 0;
  동안 (i < knots.length && knots.arrayZ[i].z < z) i++;
  만약 (i < knots.length && knots.arrayZ[i].z == z) 반환;
  knots.push (avar2_offset_knot_t {0.0, 0.0});
  만약 (unlikely (knots.in_error ())) 반환;
  위해 (unsigned j = knots.length - 1; j > i; j--)
    knots.arrayZ[j] = knots.arrayZ[j - 1];
  knots.arrayZ[i] = avar2_offset_knot_t {z, offset};
}

/* Piecewise-linear evaluation over the sorted knots. Inputs are always
 * within [-1, +1] and anchor knots at -1/0/+1 always exist. */
static inline double
_avar2_eval_offset (const hb_vector_t<avar2_offset_knot_t> &knots, double z)
{
  unsigned len = knots.length;
  위해 (unsigned i = 0; i < len; i++)
  {
    만약 (z == knots.arrayZ[i].z) 반환 knots.arrayZ[i].offset;
    만약 (z < knots.arrayZ[i].z)
    {
      만약 (!i) 반환 knots.arrayZ[0].offset;
      const auto &before = knots.arrayZ[i - 1];
      const auto &after = knots.arrayZ[i];
      double denom = after.z - before.z;
      반환 before.offset + (after.offset - before.offset) * (z - before.z) / denom;
    }
  }
  반환 len ? knots.arrayZ[len - 1].offset : 0.0;
}

/* Piecewise-linear evaluation over instantiated avar v1 mappings, as
 * produced by SegmentMaps::subset (sorted, with -1/0/+1 anchors).
 * Matches SegmentMaps::map_float for such well-formed mappings. */
static inline double
_avar2_map_new_mapping (const hb_vector_t<AxisValueMap> &mappings, double v,
			unsigned from_offset = 0, unsigned to_offset = 1)
{
  unsigned len = mappings.length;
  만약 (!len) 반환 v;

  위해 (unsigned i = 0; i < len; i++)
  {
    double from = (double) mappings.arrayZ[i].coords[from_offset].to_float ();
    만약 (v == from)
      반환 (double) mappings.arrayZ[i].coords[to_offset].to_float ();
    만약 (v < from)
    {
      double to = (double) mappings.arrayZ[i].coords[to_offset].to_float ();
      만약 (!i) 반환 v - from + to;
      double prev_from = (double) mappings.arrayZ[i - 1].coords[from_offset].to_float ();
      double prev_to = (double) mappings.arrayZ[i - 1].coords[to_offset].to_float ();
      double denom = from - prev_from;
      만약 (denom == 0.0) 반환 prev_to;
      반환 prev_to + (to - prev_to) * (v - prev_from) / denom;
    }
  }
  반환 v - (double) mappings.arrayZ[len - 1].coords[from_offset].to_float ()
           + (double) mappings.arrayZ[len - 1].coords[to_offset].to_float ();
}

/* Inverse of _avar2_map_new_mapping: pull an output coordinate back to a
 * preimage input coordinate. For a non-strictly-monotone mapping this picks
 * one preimage, which is fine for its only use (augmenting the error
 * estimator's sample set). */
static inline double
_avar2_unmap_new_mapping (const hb_vector_t<AxisValueMap> &mappings, double v)
{
  반환 _avar2_map_new_mapping (mappings, v, 1, 0);
}

/* Plain (non-avar) fvar-style normalization of a user value against a
 * user-space (min, default, max) triple. */
static inline double
_avar2_normalize_value (double v, double min, double def, double max)
{
  v = hb_clamp (v, min, max);
  만약 (v == def) 반환 0.0;
  만약 (v < def) 반환 def == min ? 0.0 : (v - def) / (def - min);
  반환 def == max ? 0.0 : (v - def) / (max - def);
}

/* Inverse of the above: map a normalized value back to user space. */
static inline double
_avar2_denormalize_value (double v, double min, double def, double max)
{
  만약 (v == 0.0) 반환 def;
  반환 v < 0.0 ? def + v * (def - min) : def + v * (max - def);
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
static inline unsigned
_avar2_estimate_offset_error (const SegmentMaps &old_seg,
                              const hb_vector_t<AxisValueMap> &new_mapping,
                              double old_min, double old_def, double old_max,
                              double new_min, double new_def, double new_max,
                              const hb_vector_t<avar2_offset_knot_t> &knots)
{
  constexpr unsigned samples = 257;
  hb_vector_t<double> us;
  만약 (unlikely (!us.alloc (samples + old_seg.as_array ().length +
                           new_mapping.length + knots.length)))
    반환 UINT_MAX; /* estimator only; fail towards "worse" */
  위해 (unsigned i = 0; i < samples; i++)
    us.push (new_min + (new_max - new_min) * i / (samples - 1));
  위해 (const auto &_ : old_seg.as_array ())
    us.push (_avar2_denormalize_value ((double) _.coords[0].to_float (),
                                       old_min, old_def, old_max));
  위해 (const auto &_ : new_mapping)
    us.push (_avar2_denormalize_value ((double) _.coords[0].to_float (),
                                       new_min, new_def, new_max));
  위해 (const auto &knot : knots)
    us.push (_avar2_denormalize_value (
               _avar2_unmap_new_mapping (new_mapping, knot.z),
               new_min, new_def, new_max));
  만약 (unlikely (us.in_error ())) 반환 UINT_MAX;

  unsigned max_err = 0;
  위해 (double u : us)
  {
    만약 (u < new_min || u > new_max) continue;
    double n_old = _avar2_normalize_value (u, old_min, old_def, old_max);
    double old_final = (double) old_seg.map_float ((float) n_old);
    double n_new = _avar2_normalize_value (u, new_min, new_def, new_max);
    double z = _avar2_map_new_mapping (new_mapping, n_new);
    double new_final = z + _avar2_eval_offset (knots, z);
    정수 err = abs ((정수) roundf ((float) (old_final * 16384.0)) -
                   (정수) roundf ((float) (new_final * 16384.0)));
    만약 ((unsigned) err > max_err) max_err = err;
  }
  반환 max_err;
}

struct avar
{
  static constexpr hb_tag_t tableTag = HB_OT_TAG_avar;

  bool has_data () const { 반환 version.to_int (); }

  const SegmentMaps* get_segment_maps () const
  { 반환 &firstAxisSegmentMaps; }

  unsigned get_axis_count () const
  { 반환 axisCount; }

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    만약 (!(version.sanitize (c) &&
	  hb_barrier () &&
	  (version.major == 1
#ifndef HB_NO_AVAR2
	   || version.major == 2
#endif
	   ) &&
	  c->check_struct (this)))
      return_trace (false);

    const SegmentMaps *map = &firstAxisSegmentMaps;
    unsigned 정수 count = axisCount;
    위해 (unsigned 정수 i = 0; i < count; i++)
    {
      만약 (unlikely (!map->sanitize (c)))
	return_trace (false);
      map = &StructAfter<SegmentMaps> (*map);
    }

#ifndef HB_NO_AVAR2
    만약 (version.major < 2)
      return_trace (true);
    hb_barrier ();

    const auto &v2 = * (const avarV2Tail *) map;
    만약 (unlikely (!v2.sanitize (c, this)))
      return_trace (false);
#endif

    return_trace (true);
  }

  void map_coords_16_16 (정수 *coords, unsigned 정수 coords_length) const
  {
    unsigned 정수 count = hb_min (coords_length, axisCount);

    const SegmentMaps *map = &firstAxisSegmentMaps;
    위해 (unsigned 정수 i = 0; i < count; i++)
    {
      coords[i] = roundf (map->map_float (coords[i] / 65536.f) * 65536.f);
      map = &StructAfter<SegmentMaps> (*map);
    }

#ifndef HB_NO_AVAR2
    만약 (version.major < 2)
      반환;
    hb_barrier ();

    위해 (; count < axisCount; count++)
      map = &StructAfter<SegmentMaps> (*map);

    const auto &v2 = * (const avarV2Tail *) map;

    const auto &varidx_map = this+v2.varIdxMap;
    const auto &var_store = this+v2.varStore;
    auto *var_store_cache = var_store.create_cache ();

    hb_vector_t<정수> coords_2_14;
    coords_2_14.resize (coords_length);
    위해 (unsigned i = 0; i < coords_length; i++)
      coords_2_14[i] = roundf (coords[i] / 4.f); // 16.16 -> 2.14

    hb_vector_t<정수> out;
    out.alloc (coords_length);
    위해 (unsigned i = 0; i < coords_length; i++)
    {
      정수 v = coords[i];
      uint32_t varidx = varidx_map.map (i);
      float delta = var_store.get_delta (varidx, coords_2_14.arrayZ, coords_2_14.length, var_store_cache);
      /* Apply the delta unclamped and clamp only the result to [-1, +1],
       * matching fontTools. Since inputs and results are in [-1, +1],
       * deltas beyond ±2 are equivalent to ±2; clamp to that range only
       * to keep the float->int conversion safe. */
      float d = hb_clamp (delta * 4, -(float) (1<<17), +(float) (1<<17)); // 2.14 -> 16.16
      v += (정수) roundf (d);
      v = hb_clamp (v, -(1<<16), +(1<<16));
      out.push (v);
    }
    위해 (unsigned i = 0; i < coords_length; i++)
      coords[i] = out[i];

    OT::ItemVariationStore::destroy_cache (var_store_cache);
#endif
  }

  /* An avar version 2 font is never downgraded to v1: even with a NULL
   * varStore offset (legal; maps like plain v1) the avar2 subsetting path
   * applies, with "no variation" semantics for rows that don't resolve —
   * offset compensation then creates delta rows on demand. */
  bool has_v2_data () const { 반환 version.major > 1; }

  /* Resolve the avar2 VarStore and VarIdxMap, for the subset planner.
   * Either pointer may be to the Null object (nullable offsets). */
  bool get_v2_store_and_map (const ItemVariationStore **store,
                             const DeltaSetIndexMap **varidx_map) const
  {
#ifndef HB_NO_AVAR2
    만약 (version.major < 2) 반환 false;
    const SegmentMaps *map = &firstAxisSegmentMaps;
    위해 (unsigned i = 0; i < axisCount; i++)
      map = &StructAfter<SegmentMaps> (*map);
    const auto &v2 = * (const avarV2Tail *) map;
    *store = &(this+v2.varStore);
    *varidx_map = &(this+v2.varIdxMap);
    반환 true;
#아니면
    반환 false;
#endif
  }

  // axis normalization is done in 2.14 here
  // TODO: deprecate this API once fonttools is updated to use 16.16 normalization
  bool map_coords_2_14 (float *coords, unsigned 정수 coords_length,
			bool v1_only = false) const
  {
    hb_vector_t<정수> coords_2_14;
    만약 (!v1_only && !coords_2_14.resize (coords_length)) 반환 false;
    unsigned 정수 count = hb_min (coords_length, axisCount);

    const SegmentMaps *map = &firstAxisSegmentMaps;
    위해 (unsigned 정수 i = 0; i < count; i++)
    {
      정수 v = roundf (map->map_float (coords[i]) * 16384.f);
      만약 (!v1_only)
	coords_2_14[i] = v;
      coords[i] = v / 16384.f;
      map = &StructAfter<SegmentMaps> (*map);
    }

    만약 (v1_only)
      반환 true;

#ifndef HB_NO_AVAR2
    만약 (version.major < 2)
      반환 true;
    hb_barrier ();

    위해 (; count < axisCount; count++)
      map = &StructAfter<SegmentMaps> (*map);

    const auto &v2 = * (const avarV2Tail *) map;

    const auto &varidx_map = this+v2.varIdxMap;
    const auto &var_store = this+v2.varStore;
    auto *var_store_cache = var_store.create_cache ();

    위해 (unsigned i = 0; i < coords_length; i++)
    {
      정수 v = coords_2_14[i];
      uint32_t varidx = varidx_map.map (i);
      float delta = var_store.get_delta (varidx, coords_2_14.arrayZ, coords_2_14.length, var_store_cache);
      /* As above: apply the delta unclamped (±2 covers every useful case)
       * and clamp the result to [-1, +1], matching fontTools. */
      v += (정수) hb_clamp (roundf (delta), -(float) (1<<15), +(float) (1<<15));
      v = hb_clamp (v, -(1<<14), +(1<<14));
      coords[i] = v / 16384.f;
    }

    OT::ItemVariationStore::destroy_cache (var_store_cache);
    반환 true;
#아니면
    반환 version.major < 2;
#endif
  }

  bool subset (hb_subset_context_t *c) const
  {
    TRACE_SUBSET (this);
    unsigned retained_axis_count = c->plan->axes_index_map.get_population ();
    만약 (!retained_axis_count) //all axes are pinned/dropped
      return_trace (false);

    avar *out = c->serializer->allocate_min<avar> ();
    만약 (unlikely (!out)) return_trace (false);

    out->version.major = c->plan->has_avar2 ? 2 : 1;
    out->version.minor = 0;
    만약 (!c->serializer->check_assign (out->axisCount, retained_axis_count, HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (false);

    /* For avar2, keep the instantiated v1 mappings around; offset
     * compensation needs them to locate the new mappings' kinks. */
    hb_vector_t<hb_vector_t<AxisValueMap>> new_mappings;
    만약 (c->plan->has_avar2 && !new_mappings.resize (axisCount))
      return_trace (false);

    const hb_map_t& axes_index_map = c->plan->axes_index_map;
    const SegmentMaps *map = &firstAxisSegmentMaps;
    unsigned count = axisCount;
    위해 (unsigned 정수 i = 0; i < count; i++)
    {
      만약 (axes_index_map.has (i))
      {
        hb_tag_t *axis_tag;
        만약 (!c->plan->axes_old_index_tag_map.has (i, &axis_tag))
          return_trace (false);

        Triple *axis_location;
        만약 (c->plan->has_avar2 &&
            c->plan->user_axes_location.has (*axis_tag, &axis_location) &&
            axis_location->is_point ())
        {
          /* Pinned axis in avar2 mode: serialize identity segment map
           * {-1->-1, 0->0, 1->1}. The axis is kept in fvar as hidden,
           * so avar needs a segment map entry for it. */
          auto *identity_map = c->serializer->start_embed<SegmentMaps> ();
          만약 (unlikely (!c->serializer->extend_min (identity_map)))
            return_trace (false);
          AxisValueMap m;
          m.set_mapping (-1.f, -1.f);
          만약 (!m.serialize (c->serializer)) return_trace (false);
          m.set_mapping (0.f, 0.f);
          만약 (!m.serialize (c->serializer)) return_trace (false);
          m.set_mapping (1.f, 1.f);
          만약 (!m.serialize (c->serializer)) return_trace (false);
          만약 (!c->serializer->check_assign (identity_map->len, 3u,
                                            HB_SERIALIZE_ERROR_INT_OVERFLOW))
            return_trace (false);
        }
        아니면
        {
          /* Restricted or free axis: use standard SegmentMaps::subset() */
          만약 (!map->subset (c, *axis_tag,
                            c->plan->has_avar2 ? &new_mappings[i] : nullptr))
            return_trace (false);
        }
      }
      map = &StructAfter<SegmentMaps> (*map);
    }

    만약 (c->plan->has_avar2)
      return_trace (_subset_avar2 (c, new_mappings));

    return_trace (true);
  }

  private:
  struct avar2_index_map_plan_t
  {
    bool init (const hb_vector_t<uint32_t> &varidx_mapping,
	       const hb_map_t &axes_index_map,
	       unsigned axis_count)
    {
      만약 (!output_map.alloc (axes_index_map.get_population ()))
	반환 false;

      bool has_no_variation = false;
      unsigned max_outer = 0, max_inner = 0;
      위해 (unsigned i = 0; i < axis_count; i++)
      {
	만약 (!axes_index_map.has (i)) continue;
	uint32_t varidx = varidx_mapping[i];
	output_map.push (varidx);
	만약 (varidx == HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
	{
	  has_no_variation = true;
	  continue;
	}
	max_outer = hb_max (max_outer, varidx >> 16);
	max_inner = hb_max (max_inner, varidx & 0xFFFF);
      }
      만약 (output_map.in_error () ||
	  output_map.length != axes_index_map.get_population ())
	반환 false;

      만약 (has_no_variation)
      {
	width = 4;
	inner_bit_count = 16;
      }
      아니면
      {
	inner_bit_count = hb_max (1u, hb_bit_storage (max_inner));
	unsigned outer_bit_count = hb_max (1u, hb_bit_storage (max_outer));
	width = hb_clamp ((inner_bit_count + outer_bit_count + 7) / 8, 1u, 4u);
	만약 (inner_bit_count + outer_bit_count > width * 8)
	  inner_bit_count = width * 8 - outer_bit_count;
      }
      반환 true;
    }

    unsigned get_inner_bit_count () const { 반환 inner_bit_count; }
    unsigned get_width () const { 반환 width; }
    hb_array_t<const uint32_t> get_output_map () const { 반환 output_map.as_array (); }

    unsigned inner_bit_count = 1;
    unsigned width = 1;
    hb_vector_t<uint32_t> output_map;
  };

  bool _subset_avar2 (hb_subset_context_t *c,
                      const hb_vector_t<hb_vector_t<AxisValueMap>> &new_mappings) const
  {
#만약 defined (HB_NO_VAR) || defined (HB_NO_AVAR2)
    /* Not reachable: the plan never sets has_avar2 in these configurations. */
    반환 false;
#아니면

    /* 1. Locate original avar2 data, keeping per-axis old segment maps */
    hb_vector_t<const SegmentMaps *> old_seg_maps;
    만약 (!old_seg_maps.alloc (axisCount)) 반환 false;
    const SegmentMaps *map = &firstAxisSegmentMaps;
    위해 (unsigned i = 0; i < axisCount; i++)
    {
      old_seg_maps.push (map);
      map = &StructAfter<SegmentMaps> (*map);
    }

    const auto &v2 = * (const avarV2Tail *) map;
    const auto &varidx_map = this+v2.varIdxMap;
    const auto &var_store = this+v2.varStore;

    auto fvar_axes = c->plan->source->table.fvar->get_axes ();

    /* 2. Compute default deltas by evaluating VarStore at old defaults */
    hb_vector_t<정수> default_coords;
    만약 (!default_coords.resize (axisCount)) 반환 false;
    위해 (unsigned i = 0; i < axisCount; i++)
    {
      hb_tag_t *axis_tag;
      만약 (c->plan->axes_old_index_tag_map.has (i, &axis_tag) &&
          c->plan->old_intermediates.has (*axis_tag))
      {
        float d_i = (float) c->plan->old_intermediates.get (*axis_tag).middle;
        default_coords[i] = roundf (d_i * 16384.f);
      }
      아니면
        default_coords[i] = 0;
    }

    auto *store_cache = var_store.create_cache ();
    hb_vector_t<float> default_deltas;
    만약 (!default_deltas.resize (axisCount)) {
      ItemVariationStore::destroy_cache (store_cache);
      반환 false;
    }
    위해 (unsigned i = 0; i < axisCount; i++)
    {
      uint32_t varidx = varidx_map.map (i);
      만약 (varidx == HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
        default_deltas[i] = 0.f;
      아니면
        default_deltas[i] = var_store.get_delta (varidx, default_coords.arrayZ,
                                                  default_coords.length, store_cache);
    }
    ItemVariationStore::destroy_cache (store_cache);

    /* 3. Rebase IVS regions */
    item_variations_t item_vars;
    만약 (!item_vars.create_from_item_varstore (var_store, c->plan->axes_old_index_tag_map))
      반환 false;
    만약 (!item_vars.instantiate_tuple_vars (c->plan->old_intermediates,
                                           c->plan->axes_triple_distances,
                                           false))
      반환 false;

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
    만약 (!new_varidx_mapping.resize (axisCount)) 반환 false;
    위해 (unsigned i = 0; i < axisCount; i++)
    {
      uint32_t varidx = varidx_map.map (i);
      만약 (varidx != HB_OT_LAYOUT_NO_VARIATIONS_INDEX &&
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
    hb_hashmap_t<uint32_t, unsigned> varidx_ref_count;
    위해 (unsigned i = 0; i < axisCount; i++)
    {
      만약 (!c->plan->axes_index_map.has (i)) continue; /* self-contained: dropped */
      uint32_t varidx = new_varidx_mapping[i];
      만약 (varidx == HB_OT_LAYOUT_NO_VARIATIONS_INDEX) continue;
      unsigned *count;
      만약 (varidx_ref_count.has (varidx, &count))
        (*count)++;
      아니면 만약 (!varidx_ref_count.set (varidx, 1))
        반환 false;
    }
    위해 (unsigned i = 0; i < axisCount; i++)
    {
      hb_tag_t *axis_tag_ptr;
      만약 (!c->plan->axes_old_index_tag_map.has (i, &axis_tag_ptr))
        반환 false;
      /* Only axes that will receive offset compensation (restricted or
       * pinned) can contaminate a shared row. */
      만약 (!c->plan->axes_index_map.has (i) ||
          !c->plan->user_axes_location.has (*axis_tag_ptr))
        continue;
      uint32_t varidx = new_varidx_mapping[i];
      만약 (varidx == HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
        continue; /* gets a fresh, private VarData in the offset loop below */
      unsigned *count;
      만약 (!varidx_ref_count.has (varidx, &count) || *count <= 1)
        continue; /* sole owner: safe to write offsets in place */
      unsigned outer = varidx >> 16;
      unsigned new_inner = item_vars.duplicate_row (outer, varidx & 0xFFFF);
      만약 (unlikely (new_inner == (unsigned) -1)) 반환 false;
      new_varidx_mapping[i] = (outer << 16) | new_inner;
      (*count)--;
    }

    /* 6. Add offset compensation tuples.
     * Track processed (outer,inner) pairs to avoid adding duplicate biases
     * when multiple axes share the same varIdx. */
    hb_set_t processed_varidxes;
    위해 (unsigned i = 0; i < axisCount; i++)
    {
      hb_tag_t *axis_tag_ptr;
      만약 (!c->plan->axes_old_index_tag_map.has (i, &axis_tag_ptr))
        반환 false;
      hb_tag_t axis_tag = *axis_tag_ptr;

      /* Self-contained pinned axes are removed from fvar/avar; their
       * contribution is baked into the variation tables instead. */
      만약 (!c->plan->axes_index_map.has (i))
        continue;

      Triple *new_user;
      만약 (c->plan->user_axes_location.has (axis_tag, &new_user))
      {
        /* This axis is being restricted or pinned */
        Triple *old_int;
        만약 (!c->plan->old_intermediates.has (axis_tag, &old_int))
          반환 false;

        float a_i = (float) old_int->minimum;
        float d_i = (float) old_int->middle;
        float b_i = (float) old_int->maximum;

        정수 d_int = roundf (d_i * 16384.f);

        bool is_pinned = new_user->is_point ();

        uint32_t varidx = new_varidx_mapping[i];
        unsigned outer, inner, item_count;

        만약 (varidx == HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
        {
          /* No existing avar2 mapping. Create new VarData. */
          outer = item_vars.add_vardata (1);
          inner = 0;
          item_count = 1;
          new_varidx_mapping[i] = (outer << 16) | inner;
          default_deltas[i] = 0.f; /* no prior default delta */
        }
        아니면
        {
          outer = varidx >> 16;
          inner = varidx & 0xFFFF;
          item_count = item_vars.get_item_count (outer);
        }

        /* Empty-region bias: d_int + round(defaultDelta) */
        정수 bias = d_int + (정수) roundf (default_deltas[i]);
        만약 (bias != 0)
        {
          hb_hashmap_t<hb_tag_t, Triple> empty_region;
          item_vars.add_tuple (outer, std::move (empty_region),
                               inner, bias, item_count);
        }

        만약 (!is_pinned)
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
          _avar2_add_knot (knots, -1.0, (double) a_i + 1.0);
          _avar2_add_knot (knots, 0.0, (double) d_i);
          _avar2_add_knot (knots, 1.0, (double) b_i - 1.0);

          const hb_vector_t<AxisValueMap> &new_mapping = new_mappings[i];

          float min_f = 0.f, def_f = 0.f, max_f = 0.f;
          만약 (likely (i < fvar_axes.length))
            fvar_axes[i].get_coordinates (min_f, def_f, max_f);
          double old_min = (double) min_f;
          double old_def = (double) def_f;
          double old_max = (double) max_f;

          /* If the axis default MOVED, inv_renorm also kinks where the OLD
           * default lands in the new space (the old intermediate coordinate
           * crosses 0 there), at
           *   z = z_old  ->  -z_old
           * Omitting that knot (as a plain two-tent encoding would) makes
           * interior coordinates wrong. */
          double z_old = _avar2_normalize_value ((double) old_def,
                                                 (double) new_user->minimum,
                                                 (double) new_user->middle,
                                                 (double) new_user->maximum);
          z_old = _avar2_map_new_mapping (new_mapping, z_old);
          z_old = (double) roundf ((float) (z_old * 16384.0)) / 16384.0;
          만약 (-1.0 < z_old && z_old < 1.0)
            _avar2_add_knot (knots, z_old, -z_old);

          /* Interior avar v1 breakpoints inside the retained range each put
           * a kink in offset(z). Sampling only {-1, 0, +1, z_old} would
           * linearly interpolate across those kinks. The instantiated
           * mapping keeps exactly the in-range old breakpoints, and the new
           * mapping kinks at each one's output coordinate; add that z with
           * its old intermediate value so offset(z) is reproduced at every
           * kink. */
          hb_vector_t<avar2_offset_knot_t> with_breakpoints (knots);
          위해 (const auto &m : new_mapping)
          {
            double from = (double) m.coords[0].to_float ();
            만약 (from == -1.0 || from == 0.0 || from == 1.0)
              continue; /* anchors already seeded */
            double z = (double) m.coords[1].to_float ();
            만약 (!(-1.0 < z && z < 1.0))
              continue;
            double user = _avar2_denormalize_value (from,
                                                    (double) new_user->minimum,
                                                    (double) new_user->middle,
                                                    (double) new_user->maximum);
            double n_old = _avar2_normalize_value (user, old_min, old_def, old_max);
            double x_old = (double) old_seg_maps[i]->map_float ((float) n_old);
            x_old = (double) roundf ((float) (x_old * 16384.0)) / 16384.0;
            _avar2_add_knot (with_breakpoints, z, x_old - z);
          }

          만약 (unlikely (knots.in_error () || with_breakpoints.in_error ()))
            반환 false;

          /* Extra tents cost F2Dot14 rounding, so for a steep segment they
           * can add more quantization noise than the structural error they
           * remove. Keep the interior breakpoints only when they do not
           * increase the estimated residual; this makes the collection a
           * strict (never-worse) improvement over the {-1, 0, +1, z_old}
           * anchors. Warn when even the better choice is not bit-exact (a
           * steep retained segment that cannot be reproduced in F2Dot14). */
          unsigned err = _avar2_estimate_offset_error (*old_seg_maps[i], new_mapping,
                                                       old_min, old_def, old_max,
                                                       (double) new_user->minimum,
                                                       (double) new_user->middle,
                                                       (double) new_user->maximum,
                                                       knots);
          만약 (with_breakpoints.length != knots.length)
          {
            unsigned err_with = _avar2_estimate_offset_error (*old_seg_maps[i], new_mapping,
                                                              old_min, old_def, old_max,
                                                              (double) new_user->minimum,
                                                              (double) new_user->middle,
                                                              (double) new_user->maximum,
                                                              with_breakpoints);
            만약 (err_with <= err)
            {
              knots = std::move (with_breakpoints);
              err = err_with;
            }
          }
          만약 (err > 8)
            DEBUG_MSG (SUBSET, nullptr,
                       "avar2 offset compensation is approximate for axis %c%c%c%c: "
                       "max residual %u F2Dot14 units",
                       HB_UNTAG (axis_tag), err);

          /* Synthesize tents. Adjacent tents evaluate to zero at each
           * other's peaks, so each knot's delta is offset(z) - offset(0);
           * the base value offset(0) = d_i is carried by the empty-region
           * bias above. This reduces to the classic pair of tents
           * (-1,-1,0) / (0,+1,+1) when the default is unchanged and there
           * are no interior knots. */
          위해 (unsigned k = 0; k < knots.length; k++)
          {
            double z = knots.arrayZ[k].z;
            만약 (z == 0.0) continue;
            정수 delta = (정수) roundf ((float) ((knots.arrayZ[k].offset - (double) d_i) * 16384.0));
            만약 (!delta) continue;
            double lower, upper;
            만약 (z > 0.0)
            {
              lower = knots.arrayZ[k - 1].z;
              upper = k + 1 < knots.length ? knots.arrayZ[k + 1].z : z;
            }
            아니면
            {
              upper = knots.arrayZ[k + 1].z;
              lower = k > 0 ? knots.arrayZ[k - 1].z : z;
            }
            hb_hashmap_t<hb_tag_t, Triple> region;
            만약 (unlikely (!region.set (axis_tag, Triple (lower, z, upper))))
              반환 false;
            item_vars.add_tuple (outer, std::move (region),
                                 inner, delta, item_count);
          }
        }
      }
      아니면
      {
        /* Free or private axis — not being restricted.
         * If it has a non-zero default delta, add it back as a bias.
         * Skip if this (outer,inner) was already processed (shared varIdx). */
        uint32_t varidx = new_varidx_mapping[i];
        만약 (varidx == HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
          continue;

        만약 (processed_varidxes.has (varidx))
          continue;
        processed_varidxes.add (varidx);

        unsigned outer = varidx >> 16;
        unsigned inner = varidx & 0xFFFF;
        정수 dd = (정수) roundf (default_deltas[i]);
        만약 (dd != 0)
        {
          unsigned item_count = item_vars.get_item_count (outer);
          hb_hashmap_t<hb_tag_t, Triple> empty_region;
          item_vars.add_tuple (outer, std::move (empty_region),
                               inner, dd, item_count);
        }
      }
    }

    /* 7. Finalize: build region list + convert to varstore */
    만약 (!item_vars.build_region_list ()) 반환 false;
    만약 (!item_vars.as_item_varstore (true /* optimize */,
                                     false /* use_no_variation_idx */))
      반환 false;

    /* 8. Apply varidx_map optimization remapping */
    const auto &opt_varidx_map = item_vars.get_varidx_map ();
    위해 (unsigned i = 0; i < axisCount; i++)
    {
      uint32_t varidx = new_varidx_mapping[i];
      만약 (varidx == HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
        continue;
      uint32_t *new_idx;
      만약 (opt_varidx_map.has (varidx, &new_idx))
        new_varidx_mapping[i] = *new_idx;
    }

    /* 9. Serialize avarV2Tail. Entries cover the retained axes only;
     * self-contained pinned axes are removed from fvar. */
    avar2_index_map_plan_t index_map_plan;
    만약 (!index_map_plan.init (new_varidx_mapping,
			      c->plan->axes_index_map,
			      axisCount))
      반환 false;

    auto *tail = c->serializer->allocate_size<avarV2Tail> (avarV2Tail::static_size);
    만약 (unlikely (!tail)) 반환 false;
    만약 (!tail->varIdxMap.serialize_serialize (c->serializer, index_map_plan))
      반환 false;
    만약 (!tail->varStore.serialize_serialize (c->serializer,
					     item_vars.has_long_word (),
					     c->plan->axis_tags,
					     item_vars.get_region_list (),
					     item_vars.get_vardata_encodings ()))
      반환 false;

    반환 true;
#endif
  }

  public:

  protected:
  FixedVersion<>version;	/* Version of the avar table
				 * initially set to 0x00010000u */
  HBUINT16	reserved;	/* This field is permanently reserved. Set to 0. */
  HBUINT16	axisCount;	/* The number of variation axes in the font. This
				 * must be the same number as axisCount in the
				 * 'fvar' table. */
  SegmentMaps	firstAxisSegmentMaps;

  public:
  DEFINE_SIZE_MIN (8);
};

} /* namespace OT */


#endif /* HB_OT_VAR_AVAR_TABLE_HH */
