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

#که_نه_تعریف_شوی HB_OT_VAR_AVAR_جدول_HH
#تعریف_کړه HB_OT_VAR_AVAR_جدول_HH

#ضمن "hb-open-type.hh"
#ضمن "hb-ot-var-common.hh"
#ضمن "hb-ot-var-fvar-table.hh"


/*
 * avar -- Axis Variations
 * https://docs.microsoft.com/en-us/typography/opentype/spec/avar
 */

#تعریف_کړه HB_OT_نښان_avar HB_نښان('a','v','a','r')


نطاق OT {


/* "Spec": https://github.com/be-fonts/boring-expansion-spec/issues/14 */
بنية avar_V2_لکۍ
{
  ملګری بنية avar;

  منطقي sanitize (hb_sanitize_context_t *c,
		 ثابت فراغ *بنسټ) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (varIdxMap.sanitize (c, بنسټ) &&
		  varStore.sanitize (c, بنسټ));
  }

  محمي:
  Offset32_منزل<Delta_سېټ_شاخص_Map>	varIdxMap;	/* Offset from the beginning of 'avar' table. */
  Offset32_منزل<توکی_Variation_Store>	varStore;	/* Offset from the beginning of 'avar' table. */

  عام:
  DEFINE_اندازه_STATIC (8);
};


بنية Axis_ارزښت_Map
{
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  فراغ سېټ_mapping (عائم from_coord, عائم منزل_coord)
  {
    coords[0].سېټ_float (from_coord);
    coords[1].سېټ_float (منزل_coord);
  }

  منطقي is_outside_axis_range (ثابت Triple& axis_range) ثابت
  {
    مزدوج from_coord = (مزدوج) coords[0].منزل_float ();
    أرجع !axis_range.لري (from_coord);
  }

  منطقي must_include () ثابت
  {
    عائم from_coord = coords[0].منزل_float ();
    عائم منزل_coord = coords[1].منزل_float ();
    أرجع (from_coord == -1.f && منزل_coord == -1.f) ||
           (from_coord == 0.f && منزل_coord == 0.f) ||
           (from_coord == 1.f && منزل_coord == 1.f);
  }

  فراغ instantiate (ثابت Triple& axis_range,
                    ثابت Triple& unmapped_range,
                    ثابت TripleDistances& triple_distances)
  {
    عائم from_coord = coords[0].منزل_float ();
    عائم منزل_coord = coords[1].منزل_float ();

    from_coord = renormalize_ارزښت ((مزدوج) from_coord, unmapped_range, triple_distances);
    منزل_coord = renormalize_ارزښت ((مزدوج) منزل_coord, axis_range, triple_distances);

    /* Malformed input can renormalize outside [-1,1], which would
     * overflow F2DOT14. */
    from_coord = hb_clamp (from_coord, -1.f, 1.f);
    منزل_coord = hb_clamp (منزل_coord, -1.f, 1.f);

    coords[0].سېټ_float (from_coord);
    coords[1].سېټ_float (منزل_coord);
  }

  HB_دننی ساكن صحيح cmp (ثابت فراغ *pa, ثابت فراغ *pb)
  {
    ثابت Axis_ارزښت_Map *a = (ثابت Axis_ارزښت_Map *) pa;
    ثابت Axis_ارزښت_Map *b = (ثابت Axis_ارزښت_Map *) pb;

    صحيح a_from = a->coords[0].منزل_int ();
    صحيح b_from = b->coords[0].منزل_int ();
    إذا (a_from != b_from)
      أرجع a_from - b_from;

    /* this should never be reached. according to the spec, all of the axis
     * value map records for a given axis must have different fromCoord values
     * */
    صحيح a_منزل = a->coords[1].منزل_int ();
    صحيح b_منزل = b->coords[1].منزل_int ();
    أرجع a_منزل - b_منزل;
  }

  منطقي serialize (hb_serialize_context_t *c) ثابت
  {
    TRACE_SERIALIZE (هذا);
    return_trace (c->embed (هذا));
  }

  عام:
  F2DOT14	coords[2];
//   F2DOT14	fromCoord;	/* A normalized coordinate value obtained using
//				 * default normalization. */
//   F2DOT14	toCoord;	/* The modified, normalized coordinate value. */

  عام:
  DEFINE_اندازه_STATIC (4);
};

بنية SegmentMaps : Array16_د<Axis_ارزښت_Map>
{
  عائم map_float (عائم value, بینښه صحيح from_دننه_واټن = 0, بینښه صحيح منزل_دننه_واټن = 1) ثابت
  {
#تعریف_کړه fromCoord coords[from_دننه_واټن].منزل_float ()
#تعریف_کړه منزل_Coord coords[منزل_دننه_واټن].منزل_float ()

    ثابت خپلکاری *map = arrayZ;

    /* The following special-cases are not part of OpenType, which requires
     * that at least -1, 0, and +1 must be mapped. But we include these as
     * part of a better error recovery scheme. */
    إذا (len < 2)
    {
      إذا (!len)
	أرجع value;
      وإلا /* len == 1*/
	أرجع value - map[0].fromCoord + map[0].منزل_Coord;
    }

    // At least two mappings now.

    /* CoreText is wild...
     * PingFangUI avar needs all this special-casing...
     * So we implement an extended version of the spec here,
     * which is more robust and more likely to be compatible with
     * the wild. */

    بینښه پیل = 0;
    بینښه پای_ټکی = len;
    إذا (map[پیل].fromCoord == -1 && map[پیل].منزل_Coord == -1 && map[پیل+1].fromCoord == -1)
      پیل++;
    إذا (map[پای_ټکی-1].fromCoord == +1 && map[پای_ټکی-1].منزل_Coord == +1 && map[پای_ټکی-2].fromCoord == +1)
      پای_ټکی--;

    /* Look for exact match first, and do lots of special-casing. */
    بینښه i;
    لكل (i = پیل; i < پای_ټکی; i++)
      إذا (value == map[i].fromCoord)
	اكسر;
    إذا (i < پای_ټکی)
    {
      // There's at least one exact match. See if there are more.
      بینښه j = i;
      لكل (; j + 1 < پای_ټکی; j++)
	إذا (value != map[j + 1].fromCoord)
	  اكسر;

      // [i,j] inclusive are all exact matches:

      // If there's only one, return it. This is the only spec-compliant case.
      إذا (i == j)
	أرجع map[i].منزل_Coord;
      // If there's exactly three, return the middle one.
      إذا (i + 2 == j)
	أرجع map[i + 1].منزل_Coord;

      // Ignore the middle ones. Return the one mapping closer to 0.
      إذا (value < 0) أرجع map[j].منزل_Coord;
      إذا (value > 0) أرجع map[i].منزل_Coord;

      // Mapping 0? CoreText seems confused. It seems to prefer 0 here...
      // So we'll just return the smallest one. lol
      أرجع fabsf (map[i].منزل_Coord) < fabsf (map[j].منزل_Coord) ? map[i].منزل_Coord : map[j].منزل_Coord;

      // Mapping 0? Return one not mapping to 0.
      إذا (map[i].منزل_Coord == 0)
	أرجع map[j].منزل_Coord;
      وإلا
	أرجع map[i].منزل_Coord;
    }

    /* There's at least two and we're not an exact match. Prepare to lerp. */

    // Find the segment we're in.
    لكل (i = پیل; i < پای_ټکی; i++)
      إذا (value < map[i].fromCoord)
	اكسر;

    إذا (i == پیل)
    {
      // Value before all segments; Shift.
      أرجع value - map[پیل].fromCoord + map[پیل].منزل_Coord;
    }
    إذا (i == پای_ټکی)
    {
      // Value after all segments; Shift.
      أرجع value - map[پای_ټکی - 1].fromCoord + map[پای_ټکی - 1].منزل_Coord;
    }

    // Actually interpolate.
    خپلکاری &before = map[i-1];
    خپلکاری &after = map[i];
    عائم denom = after.fromCoord - before.fromCoord; // Can't be zero by now.
    أرجع before.منزل_Coord + ((after.منزل_Coord - before.منزل_Coord) * (value - before.fromCoord)) / denom;

#تعریف_لغوه منزل_Coord
#تعریف_لغوه fromCoord
  }

  عائم unmap_float (عائم value) ثابت { أرجع map_float (value, 1, 0); }


  // TODO Kill this.
  Triple unmap_axis_range (ثابت Triple& axis_range) ثابت
  {
    عائم unmapped_min = unmap_float (axis_range.minimum);
    عائم unmapped_منځ = unmap_float (axis_range.منځ);
    عائم unmapped_max = unmap_float (axis_range.اعظمي);

    أرجع Triple{unmapped_min, unmapped_منځ, unmapped_max};
  }

  منطقي subset (hb_subset_context_t *c, hb_نښان_t axis_نښان_2,
               hb_ویکټور_t<Axis_ارزښت_Map> *بهر_mappings = تش_اشاره) ثابت
  {
    TRACE_SUBSET (هذا);

    /* This function cannot work on avar2 table (and currently doesn't).
     * We should instead keep the design coords in the shape plan and use
     * those. unmap_axis_range needs to be killed. */

    /* avar mapped normalized axis range. Under avar2, axes_location holds
     * only the self-contained pins for the other tables; avar itself uses
     * the intermediate-space ranges of all restricted axes. */
    ثابت خپلکاری &axes_location = c->plan->has_avar2
				? c->plan->old_intermediates
				: c->plan->axes_location;
    Triple *axis_range;
    إذا (!axes_location.has (axis_نښان_2, &axis_range))
      أرجع c->serializer->embed (*هذا);

    TripleDistances *axis_triple_distances;
    إذا (!c->plan->axes_triple_distances.has (axis_نښان_2, &axis_triple_distances))
      return_trace (خطأ);

    خپلکاری *بهر = c->serializer->پیل_embed (هذا);
    إذا (unlikely (!c->serializer->extend_min (بهر))) return_trace (خطأ);

    Triple unmapped_range = unmap_axis_range (*axis_range);

    /* create a vector of retained mappings and sort */
    hb_ویکټور_t<Axis_ارزښت_Map> ارزښت_mappings;
    لكل (ثابت خپلکاری& _ : as_array ())
    {
      إذا (_.is_outside_axis_range (unmapped_range))
        استمر;
      Axis_ارزښت_Map mapping;
      mapping = _;
      mapping.instantiate (*axis_range, unmapped_range, *axis_triple_distances);
      /* (-1, -1), (0, 0), (1, 1) mappings will be added later, so avoid
       * duplicates here */
      إذا (mapping.must_include ())
        استمر;
      ارزښت_mappings.push (mapping);
    }

    Axis_ارزښت_Map m;
    m.سېټ_mapping (-1.f, -1.f);
    ارزښت_mappings.push (m);

    m.سېټ_mapping (0.f, 0.f);
    ارزښت_mappings.push (m);

    m.سېټ_mapping (1.f, 1.f);
    ارزښت_mappings.push (m);

    ارزښت_mappings.qsort ();

    إذا (unlikely (ارزښت_mappings.دننه_error ()))
      return_trace (خطأ);

    لكل (ثابت خپلکاری& _ : ارزښت_mappings)
    {
      إذا (!_.serialize (c->serializer))
        return_trace (خطأ);
    }
    إذا (!c->serializer->check_ټاکل (بهر->len, ارزښت_mappings.اوږدتیا, HB_SERIALIZE_ERROR_INT_ډکېدنه))
      return_trace (خطأ);
    /* Hand the instantiated mappings to the caller; avar2 offset
     * compensation needs them to locate the new mapping's kinks. */
    إذا (بهر_mappings)
      *بهر_mappings = std::move (ارزښت_mappings);
    return_trace (ریښتیا);
  }

  عام:
  DEFINE_اندازه_ARRAY (2, *هذا);
};

/* One knot of the avar2 offset-compensation function
 * offset(z) = inv_renorm(z) - z, in new intermediate space. */
بنية avar2_دننه_واټن_knot_t
{
  مزدوج z;
  مزدوج دننه_واټن;
};

/* Insert a knot keeping the vector sorted by z; first insertion wins on
 * duplicate z. */
ساكن مضمن فراغ
avar2_جمع_کړه_knot (hb_ویکټور_t<avar2_دننه_واټن_knot_t> &knots, مزدوج z, مزدوج دننه_واټن)
{
  بینښه i = 0;
  طالما (i < knots.اوږدتیا && knots.arrayZ[i].z < z) i++;
  إذا (i < knots.اوږدتیا && knots.arrayZ[i].z == z) أرجع;
  knots.push (avar2_دننه_واټن_knot_t {0.0, 0.0});
  إذا (unlikely (knots.دننه_error ())) أرجع;
  لكل (بینښه j = knots.اوږدتیا - 1; j > i; j--)
    knots.arrayZ[j] = knots.arrayZ[j - 1];
  knots.arrayZ[i] = avar2_دننه_واټن_knot_t {z, دننه_واټن};
}

/* Piecewise-linear evaluation over the sorted knots. Inputs are always
 * within [-1, +1] and anchor knots at -1/0/+1 always exist. */
ساكن مضمن مزدوج
avar2_eval_دننه_واټن (ثابت hb_ویکټور_t<avar2_دننه_واټن_knot_t> &knots, مزدوج z)
{
  بینښه len = knots.اوږدتیا;
  لكل (بینښه i = 0; i < len; i++)
  {
    إذا (z == knots.arrayZ[i].z) أرجع knots.arrayZ[i].دننه_واټن;
    إذا (z < knots.arrayZ[i].z)
    {
      إذا (!i) أرجع knots.arrayZ[0].دننه_واټن;
      ثابت خپلکاری &before = knots.arrayZ[i - 1];
      ثابت خپلکاری &after = knots.arrayZ[i];
      مزدوج denom = after.z - before.z;
      أرجع before.دننه_واټن + (after.دننه_واټن - before.دننه_واټن) * (z - before.z) / denom;
    }
  }
  أرجع len ? knots.arrayZ[len - 1].دننه_واټن : 0.0;
}

/* Piecewise-linear evaluation over instantiated avar v1 mappings, as
 * produced by SegmentMaps::subset (sorted, with -1/0/+1 anchors).
 * Matches SegmentMaps::map_float for such well-formed mappings. */
ساكن مضمن مزدوج
_avar2_map_new_mapping (ثابت hb_ویکټور_t<Axis_ارزښت_Map> &mappings, مزدوج v,
			بینښه from_دننه_واټن = 0, بینښه منزل_دننه_واټن = 1)
{
  بینښه len = mappings.اوږدتیا;
  إذا (!len) أرجع v;

  لكل (بینښه i = 0; i < len; i++)
  {
    مزدوج from = (مزدوج) mappings.arrayZ[i].coords[from_دننه_واټن].منزل_float ();
    إذا (v == from)
      أرجع (مزدوج) mappings.arrayZ[i].coords[منزل_دننه_واټن].منزل_float ();
    إذا (v < from)
    {
      مزدوج to = (مزدوج) mappings.arrayZ[i].coords[منزل_دننه_واټن].منزل_float ();
      إذا (!i) أرجع v - from + to;
      مزدوج prev_from = (مزدوج) mappings.arrayZ[i - 1].coords[from_دننه_واټن].منزل_float ();
      مزدوج prev_منزل = (مزدوج) mappings.arrayZ[i - 1].coords[منزل_دننه_واټن].منزل_float ();
      مزدوج denom = from - prev_from;
      إذا (denom == 0.0) أرجع prev_منزل;
      أرجع prev_منزل + (to - prev_منزل) * (v - prev_from) / denom;
    }
  }
  أرجع v - (مزدوج) mappings.arrayZ[len - 1].coords[from_دننه_واټن].منزل_float ()
           + (مزدوج) mappings.arrayZ[len - 1].coords[منزل_دننه_واټن].منزل_float ();
}

/* Inverse of _avar2_map_new_mapping: pull an output coordinate back to a
 * preimage input coordinate. For a non-strictly-monotone mapping this picks
 * one preimage, which is fine for its only use (augmenting the error
 * estimator's sample set). */
ساكن مضمن مزدوج
_avar2_unmap_new_mapping (ثابت hb_ویکټور_t<Axis_ارزښت_Map> &mappings, مزدوج v)
{
  أرجع _avar2_map_new_mapping (mappings, v, 1, 0);
}

/* Plain (non-avar) fvar-style normalization of a user value against a
 * user-space (min, default, max) triple. */
ساكن مضمن مزدوج
avar2_normalize_ارزښت (مزدوج v, مزدوج min, مزدوج def, مزدوج max)
{
  v = hb_clamp (v, min, max);
  إذا (v == def) أرجع 0.0;
  إذا (v < def) أرجع def == min ? 0.0 : (v - def) / (def - min);
  أرجع def == max ? 0.0 : (v - def) / (max - def);
}

/* Inverse of the above: map a normalized value back to user space. */
ساكن مضمن مزدوج
avar2_denormalize_ارزښت (مزدوج v, مزدوج min, مزدوج def, مزدوج max)
{
  إذا (v == 0.0) أرجع def;
  أرجع v < 0.0 ? def + v * (def - min) : def + v * (max - def);
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
ساكن مضمن بینښه
avar2_estimate_دننه_واټن_error (ثابت SegmentMaps &old_seg,
                              ثابت hb_ویکټور_t<Axis_ارزښت_Map> &new_mapping,
                              مزدوج old_min, مزدوج old_def, مزدوج old_max,
                              مزدوج new_min, مزدوج new_def, مزدوج new_max,
                              ثابت hb_ویکټور_t<avar2_دننه_واټن_knot_t> &knots)
{
  constexpr بینښه samples = 257;
  hb_ویکټور_t<مزدوج> us;
  إذا (unlikely (!us.alloc (samples + old_seg.as_array ().اوږدتیا +
                           new_mapping.اوږدتیا + knots.اوږدتیا)))
    أرجع UINT_MAX; /* estimator only; fail towards "worse" */
  لكل (بینښه i = 0; i < samples; i++)
    us.push (new_min + (new_max - new_min) * i / (samples - 1));
  لكل (ثابت خپلکاری &_ : old_seg.as_array ())
    us.push (avar2_denormalize_ارزښت ((مزدوج) _.coords[0].منزل_float (),
                                       old_min, old_def, old_max));
  لكل (ثابت خپلکاری &_ : new_mapping)
    us.push (avar2_denormalize_ارزښت ((مزدوج) _.coords[0].منزل_float (),
                                       new_min, new_def, new_max));
  لكل (ثابت خپلکاری &knot : knots)
    us.push (avar2_denormalize_ارزښت (
               _avar2_unmap_new_mapping (new_mapping, knot.z),
               new_min, new_def, new_max));
  إذا (unlikely (us.دننه_error ())) أرجع UINT_MAX;

  بینښه max_err = 0;
  لكل (مزدوج u : us)
  {
    إذا (u < new_min || u > new_max) استمر;
    مزدوج n_old = avar2_normalize_ارزښت (u, old_min, old_def, old_max);
    مزدوج old_final = (مزدوج) old_seg.map_float ((عائم) n_old);
    مزدوج n_new = avar2_normalize_ارزښت (u, new_min, new_def, new_max);
    مزدوج z = _avar2_map_new_mapping (new_mapping, n_new);
    مزدوج new_final = z + avar2_eval_دننه_واټن (knots, z);
    صحيح err = مطلق_قیمت ((صحيح) roundf ((عائم) (old_final * 16384.0)) -
                   (صحيح) roundf ((عائم) (new_final * 16384.0)));
    إذا ((بینښه) err > max_err) max_err = err;
  }
  أرجع max_err;
}

بنية avar
{
  ساكن constexpr hb_نښان_t جدول_نښان = HB_OT_نښان_avar;

  منطقي has_معلومات () ثابت { أرجع نسخه.منزل_int (); }

  ثابت SegmentMaps* get_segment_maps () ثابت
  { أرجع &لومړی_Axis_Segment_Maps; }

  بینښه get_axis_شمېر () ثابت
  { أرجع axis_شمېر_2; }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (!(نسخه.sanitize (c) &&
	  hb_barrier () &&
	  (نسخه.major == 1
#که_نه_تعریف_شوی HB_NO_AVAR2
	   || نسخه.major == 2
#د_که_پای
	   ) &&
	  c->check_struct (هذا)))
      return_trace (خطأ);

    ثابت SegmentMaps *map = &لومړی_Axis_Segment_Maps;
    بینښه صحيح شمېر = axis_شمېر_2;
    لكل (بینښه صحيح i = 0; i < شمېر; i++)
    {
      إذا (unlikely (!map->sanitize (c)))
	return_trace (خطأ);
      map = &StructAfter<SegmentMaps> (*map);
    }

#که_نه_تعریف_شوی HB_NO_AVAR2
    إذا (نسخه.major < 2)
      return_trace (ریښتیا);
    hb_barrier ();

    ثابت خپلکاری &v2 = * (ثابت avar_V2_لکۍ *) map;
    إذا (unlikely (!v2.sanitize (c, هذا)))
      return_trace (خطأ);
#د_که_پای

    return_trace (ریښتیا);
  }

  فراغ map_coords_16_16 (صحيح *coords, بینښه صحيح coords_اوږدتیا) ثابت
  {
    بینښه صحيح شمېر = hb_min (coords_اوږدتیا, axis_شمېر_2);

    ثابت SegmentMaps *map = &لومړی_Axis_Segment_Maps;
    لكل (بینښه صحيح i = 0; i < شمېر; i++)
    {
      coords[i] = roundf (map->map_float (coords[i] / 65536.f) * 65536.f);
      map = &StructAfter<SegmentMaps> (*map);
    }

#که_نه_تعریف_شوی HB_NO_AVAR2
    إذا (نسخه.major < 2)
      أرجع;
    hb_barrier ();

    لكل (; شمېر < axis_شمېر_2; شمېر++)
      map = &StructAfter<SegmentMaps> (*map);

    ثابت خپلکاری &v2 = * (ثابت avar_V2_لکۍ *) map;

    ثابت خپلکاری &varidx_map = هذا+v2.varIdxMap;
    ثابت خپلکاری &var_store = هذا+v2.varStore;
    خپلکاری *var_store_cache = var_store.create_cache ();

    hb_ویکټور_t<صحيح> coords_2_14;
    coords_2_14.resize (coords_اوږدتیا);
    لكل (بینښه i = 0; i < coords_اوږدتیا; i++)
      coords_2_14[i] = roundf (coords[i] / 4.f); // 16.16 -> 2.14

    hb_ویکټور_t<صحيح> بهر;
    بهر.alloc (coords_اوږدتیا);
    لكل (بینښه i = 0; i < coords_اوږدتیا; i++)
    {
      صحيح v = coords[i];
      uint32_t varidx = varidx_map.map (i);
      عائم delta = var_store.get_delta (varidx, coords_2_14.arrayZ, coords_2_14.اوږدتیا, var_store_cache);
      /* Apply the delta unclamped and clamp only the result to [-1, +1],
       * matching fontTools. Since inputs and results are in [-1, +1],
       * deltas beyond ±2 are equivalent to ±2; clamp to that range only
       * to keep the float->int conversion safe. */
      عائم d = hb_clamp (delta * 4, -(عائم) (1<<17), +(عائم) (1<<17)); // 2.14 -> 16.16
      v += (صحيح) roundf (d);
      v = hb_clamp (v, -(1<<16), +(1<<16));
      بهر.push (v);
    }
    لكل (بینښه i = 0; i < coords_اوږدتیا; i++)
      coords[i] = بهر[i];

    OT::توکی_Variation_Store::destroy_cache (var_store_cache);
#د_که_پای
  }

  /* An avar version 2 font is never downgraded to v1: even with a NULL
   * varStore offset (legal; maps like plain v1) the avar2 subsetting path
   * applies, with "no variation" semantics for rows that don't resolve —
   * offset compensation then creates delta rows on demand. */
  منطقي has_v2_معلومات () ثابت { أرجع نسخه.major > 1; }

  /* Resolve the avar2 VarStore and VarIdxMap, for the subset planner.
   * Either pointer may be to the Null object (nullable offsets). */
  منطقي get_v2_store_and_map (ثابت توکی_Variation_Store **store,
                             ثابت Delta_سېټ_شاخص_Map **varidx_map) ثابت
  {
#که_نه_تعریف_شوی HB_NO_AVAR2
    إذا (نسخه.major < 2) أرجع خطأ;
    ثابت SegmentMaps *map = &لومړی_Axis_Segment_Maps;
    لكل (بینښه i = 0; i < axis_شمېر_2; i++)
      map = &StructAfter<SegmentMaps> (*map);
    ثابت خپلکاری &v2 = * (ثابت avar_V2_لکۍ *) map;
    *store = &(هذا+v2.varStore);
    *varidx_map = &(هذا+v2.varIdxMap);
    أرجع ریښتیا;
#وإلا
    أرجع خطأ;
#د_که_پای
  }

  // axis normalization is done in 2.14 here
  // TODO: deprecate this API once fonttools is updated to use 16.16 normalization
  منطقي map_coords_2_14 (عائم *coords, بینښه صحيح coords_اوږدتیا,
			منطقي v1_only = خطأ) ثابت
  {
    hb_ویکټور_t<صحيح> coords_2_14;
    إذا (!v1_only && !coords_2_14.resize (coords_اوږدتیا)) أرجع خطأ;
    بینښه صحيح شمېر = hb_min (coords_اوږدتیا, axis_شمېر_2);

    ثابت SegmentMaps *map = &لومړی_Axis_Segment_Maps;
    لكل (بینښه صحيح i = 0; i < شمېر; i++)
    {
      صحيح v = roundf (map->map_float (coords[i]) * 16384.f);
      إذا (!v1_only)
	coords_2_14[i] = v;
      coords[i] = v / 16384.f;
      map = &StructAfter<SegmentMaps> (*map);
    }

    إذا (v1_only)
      أرجع ریښتیا;

#که_نه_تعریف_شوی HB_NO_AVAR2
    إذا (نسخه.major < 2)
      أرجع ریښتیا;
    hb_barrier ();

    لكل (; شمېر < axis_شمېر_2; شمېر++)
      map = &StructAfter<SegmentMaps> (*map);

    ثابت خپلکاری &v2 = * (ثابت avar_V2_لکۍ *) map;

    ثابت خپلکاری &varidx_map = هذا+v2.varIdxMap;
    ثابت خپلکاری &var_store = هذا+v2.varStore;
    خپلکاری *var_store_cache = var_store.create_cache ();

    لكل (بینښه i = 0; i < coords_اوږدتیا; i++)
    {
      صحيح v = coords_2_14[i];
      uint32_t varidx = varidx_map.map (i);
      عائم delta = var_store.get_delta (varidx, coords_2_14.arrayZ, coords_2_14.اوږدتیا, var_store_cache);
      /* As above: apply the delta unclamped (±2 covers every useful case)
       * and clamp the result to [-1, +1], matching fontTools. */
      v += (صحيح) hb_clamp (roundf (delta), -(عائم) (1<<15), +(عائم) (1<<15));
      v = hb_clamp (v, -(1<<14), +(1<<14));
      coords[i] = v / 16384.f;
    }

    OT::توکی_Variation_Store::destroy_cache (var_store_cache);
    أرجع ریښتیا;
#وإلا
    أرجع نسخه.major < 2;
#د_که_پای
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    بینښه retained_axis_شمېر = c->plan->axes_شاخص_map.get_population ();
    إذا (!retained_axis_شمېر) //all axes are pinned/dropped
      return_trace (خطأ);

    avar *بهر = c->serializer->allocate_min<avar> ();
    إذا (unlikely (!بهر)) return_trace (خطأ);

    بهر->نسخه.major = c->plan->has_avar2 ? 2 : 1;
    بهر->نسخه.minor = 0;
    إذا (!c->serializer->check_ټاکل (بهر->axis_شمېر_2, retained_axis_شمېر, HB_SERIALIZE_ERROR_INT_ډکېدنه))
      return_trace (خطأ);

    /* For avar2, keep the instantiated v1 mappings around; offset
     * compensation needs them to locate the new mappings' kinks. */
    hb_ویکټور_t<hb_ویکټور_t<Axis_ارزښت_Map>> new_mappings;
    إذا (c->plan->has_avar2 && !new_mappings.resize (axis_شمېر_2))
      return_trace (خطأ);

    ثابت hb_map_t& axes_شاخص_map = c->plan->axes_شاخص_map;
    ثابت SegmentMaps *map = &لومړی_Axis_Segment_Maps;
    بینښه شمېر = axis_شمېر_2;
    لكل (بینښه صحيح i = 0; i < شمېر; i++)
    {
      إذا (axes_شاخص_map.has (i))
      {
        hb_نښان_t *axis_نښان_2;
        إذا (!c->plan->axes_old_شاخص_نښان_map.has (i, &axis_نښان_2))
          return_trace (خطأ);

        Triple *axis_location;
        إذا (c->plan->has_avar2 &&
            c->plan->user_axes_location.has (*axis_نښان_2, &axis_location) &&
            axis_location->is_ټکی ())
        {
          /* Pinned axis in avar2 mode: serialize identity segment map
           * {-1->-1, 0->0, 1->1}. The axis is kept in fvar as hidden,
           * so avar needs a segment map entry for it. */
          خپلکاری *بې_اغېزه_عنصر_map = c->serializer->پیل_embed<SegmentMaps> ();
          إذا (unlikely (!c->serializer->extend_min (بې_اغېزه_عنصر_map)))
            return_trace (خطأ);
          Axis_ارزښت_Map m;
          m.سېټ_mapping (-1.f, -1.f);
          إذا (!m.serialize (c->serializer)) return_trace (خطأ);
          m.سېټ_mapping (0.f, 0.f);
          إذا (!m.serialize (c->serializer)) return_trace (خطأ);
          m.سېټ_mapping (1.f, 1.f);
          إذا (!m.serialize (c->serializer)) return_trace (خطأ);
          إذا (!c->serializer->check_ټاکل (بې_اغېزه_عنصر_map->len, 3u,
                                            HB_SERIALIZE_ERROR_INT_ډکېدنه))
            return_trace (خطأ);
        }
        وإلا
        {
          /* Restricted or free axis: use standard SegmentMaps::subset() */
          إذا (!map->subset (c, *axis_نښان_2,
                            c->plan->has_avar2 ? &new_mappings[i] : تش_اشاره))
            return_trace (خطأ);
        }
      }
      map = &StructAfter<SegmentMaps> (*map);
    }

    إذا (c->plan->has_avar2)
      return_trace (_subset_avar2 (c, new_mappings));

    return_trace (ریښتیا);
  }

  خاص:
  بنية avar2_شاخص_map_plan_t
  {
    منطقي init (ثابت hb_ویکټور_t<uint32_t> &varidx_mapping,
	       ثابت hb_map_t &axes_شاخص_map,
	       بینښه axis_شمېر)
    {
      إذا (!output_map.alloc (axes_شاخص_map.get_population ()))
	أرجع خطأ;

      منطقي has_no_variation = خطأ;
      بینښه max_outer = 0, max_inner = 0;
      لكل (بینښه i = 0; i < axis_شمېر; i++)
      {
	إذا (!axes_شاخص_map.has (i)) استمر;
	uint32_t varidx = varidx_mapping[i];
	output_map.push (varidx);
	إذا (varidx == HB_OT_LAYOUT_NO_VARIATIONS_شاخص)
	{
	  has_no_variation = ریښتیا;
	  استمر;
	}
	max_outer = hb_max (max_outer, varidx >> 16);
	max_inner = hb_max (max_inner, varidx & 0xFFFF);
      }
      إذا (output_map.دننه_error () ||
	  output_map.اوږدتیا != axes_شاخص_map.get_population ())
	أرجع خطأ;

      إذا (has_no_variation)
      {
	پلنوالی = 4;
	inner_بټ_شمېر = 16;
      }
      وإلا
      {
	inner_بټ_شمېر = hb_max (1u, hb_بټ_storage (max_inner));
	بینښه outer_بټ_شمېر = hb_max (1u, hb_بټ_storage (max_outer));
	پلنوالی = hb_clamp ((inner_بټ_شمېر + outer_بټ_شمېر + 7) / 8, 1u, 4u);
	إذا (inner_بټ_شمېر + outer_بټ_شمېر > پلنوالی * 8)
	  inner_بټ_شمېر = پلنوالی * 8 - outer_بټ_شمېر;
      }
      أرجع ریښتیا;
    }

    بینښه get_inner_بټ_شمېر () ثابت { أرجع inner_بټ_شمېر; }
    بینښه get_پلنوالی () ثابت { أرجع پلنوالی; }
    hb_array_t<ثابت uint32_t> get_output_map () ثابت { أرجع output_map.as_array (); }

    بینښه inner_بټ_شمېر = 1;
    بینښه پلنوالی = 1;
    hb_ویکټور_t<uint32_t> output_map;
  };

  منطقي _subset_avar2 (hb_subset_context_t *c,
                      ثابت hb_ویکټور_t<hb_ویکټور_t<Axis_ارزښت_Map>> &new_mappings) ثابت
  {
#إذا defined (HB_NO_VAR) || defined (HB_NO_AVAR2)
    /* Not reachable: the plan never sets has_avar2 in these configurations. */
    أرجع خطأ;
#وإلا

    /* 1. Locate original avar2 data, keeping per-axis old segment maps */
    hb_ویکټور_t<ثابت SegmentMaps *> old_seg_maps;
    إذا (!old_seg_maps.alloc (axis_شمېر_2)) أرجع خطأ;
    ثابت SegmentMaps *map = &لومړی_Axis_Segment_Maps;
    لكل (بینښه i = 0; i < axis_شمېر_2; i++)
    {
      old_seg_maps.push (map);
      map = &StructAfter<SegmentMaps> (*map);
    }

    ثابت خپلکاری &v2 = * (ثابت avar_V2_لکۍ *) map;
    ثابت خپلکاری &varidx_map = هذا+v2.varIdxMap;
    ثابت خپلکاری &var_store = هذا+v2.varStore;

    خپلکاری fvar_axes = c->plan->سرچینه->جدول.fvar->get_axes ();

    /* 2. Compute default deltas by evaluating VarStore at old defaults */
    hb_ویکټور_t<صحيح> default_coords;
    إذا (!default_coords.resize (axis_شمېر_2)) أرجع خطأ;
    لكل (بینښه i = 0; i < axis_شمېر_2; i++)
    {
      hb_نښان_t *axis_نښان_2;
      إذا (c->plan->axes_old_شاخص_نښان_map.has (i, &axis_نښان_2) &&
          c->plan->old_intermediates.has (*axis_نښان_2))
      {
        عائم d_i = (عائم) c->plan->old_intermediates.get (*axis_نښان_2).منځ;
        default_coords[i] = roundf (d_i * 16384.f);
      }
      وإلا
        default_coords[i] = 0;
    }

    خپلکاری *store_cache = var_store.create_cache ();
    hb_ویکټور_t<عائم> default_deltas;
    إذا (!default_deltas.resize (axis_شمېر_2)) {
      توکی_Variation_Store::destroy_cache (store_cache);
      أرجع خطأ;
    }
    لكل (بینښه i = 0; i < axis_شمېر_2; i++)
    {
      uint32_t varidx = varidx_map.map (i);
      إذا (varidx == HB_OT_LAYOUT_NO_VARIATIONS_شاخص)
        default_deltas[i] = 0.f;
      وإلا
        default_deltas[i] = var_store.get_delta (varidx, default_coords.arrayZ,
                                                  default_coords.اوږدتیا, store_cache);
    }
    توکی_Variation_Store::destroy_cache (store_cache);

    /* 3. Rebase IVS regions */
    توکی_variations_t توکی_vars;
    إذا (!توکی_vars.create_from_توکی_varstore (var_store, c->plan->axes_old_شاخص_نښان_map))
      أرجع خطأ;
    إذا (!توکی_vars.instantiate_tuple_vars (c->plan->old_intermediates,
                                           c->plan->axes_triple_distances,
                                           خطأ))
      أرجع خطأ;

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
    hb_ویکټور_t<uint32_t> new_varidx_mapping;
    إذا (!new_varidx_mapping.resize (axis_شمېر_2)) أرجع خطأ;
    لكل (بینښه i = 0; i < axis_شمېر_2; i++)
    {
      uint32_t varidx = varidx_map.map (i);
      إذا (varidx != HB_OT_LAYOUT_NO_VARIATIONS_شاخص &&
	  !var_store.has_delta_سېټ (varidx))
	varidx = HB_OT_LAYOUT_NO_VARIATIONS_شاخص;
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
    hb_hashmap_t<uint32_t, بینښه> varidx_ref_شمېر;
    لكل (بینښه i = 0; i < axis_شمېر_2; i++)
    {
      إذا (!c->plan->axes_شاخص_map.has (i)) استمر; /* self-contained: dropped */
      uint32_t varidx = new_varidx_mapping[i];
      إذا (varidx == HB_OT_LAYOUT_NO_VARIATIONS_شاخص) استمر;
      بینښه *شمېر;
      إذا (varidx_ref_شمېر.has (varidx, &شمېر))
        (*شمېر)++;
      وإلا إذا (!varidx_ref_شمېر.سېټ (varidx, 1))
        أرجع خطأ;
    }
    لكل (بینښه i = 0; i < axis_شمېر_2; i++)
    {
      hb_نښان_t *axis_نښان_ptr;
      إذا (!c->plan->axes_old_شاخص_نښان_map.has (i, &axis_نښان_ptr))
        أرجع خطأ;
      /* Only axes that will receive offset compensation (restricted or
       * pinned) can contaminate a shared row. */
      إذا (!c->plan->axes_شاخص_map.has (i) ||
          !c->plan->user_axes_location.has (*axis_نښان_ptr))
        استمر;
      uint32_t varidx = new_varidx_mapping[i];
      إذا (varidx == HB_OT_LAYOUT_NO_VARIATIONS_شاخص)
        استمر; /* gets a fresh, private VarData in the offset loop below */
      بینښه *شمېر;
      إذا (!varidx_ref_شمېر.has (varidx, &شمېر) || *شمېر <= 1)
        استمر; /* sole owner: safe to write offsets in place */
      بینښه outer = varidx >> 16;
      بینښه new_inner = توکی_vars.duplicate_لیکه (outer, varidx & 0xFFFF);
      إذا (unlikely (new_inner == (بینښه) -1)) أرجع خطأ;
      new_varidx_mapping[i] = (outer << 16) | new_inner;
      (*شمېر)--;
    }

    /* 6. Add offset compensation tuples.
     * Track processed (outer,inner) pairs to avoid adding duplicate biases
     * when multiple axes share the same varIdx. */
    hb_سېټ_t processed_varidxes;
    لكل (بینښه i = 0; i < axis_شمېر_2; i++)
    {
      hb_نښان_t *axis_نښان_ptr;
      إذا (!c->plan->axes_old_شاخص_نښان_map.has (i, &axis_نښان_ptr))
        أرجع خطأ;
      hb_نښان_t axis_نښان_2 = *axis_نښان_ptr;

      /* Self-contained pinned axes are removed from fvar/avar; their
       * contribution is baked into the variation tables instead. */
      إذا (!c->plan->axes_شاخص_map.has (i))
        استمر;

      Triple *new_user;
      إذا (c->plan->user_axes_location.has (axis_نښان_2, &new_user))
      {
        /* This axis is being restricted or pinned */
        Triple *old_int;
        إذا (!c->plan->old_intermediates.has (axis_نښان_2, &old_int))
          أرجع خطأ;

        عائم a_i = (عائم) old_int->minimum;
        عائم d_i = (عائم) old_int->منځ;
        عائم b_i = (عائم) old_int->اعظمي;

        صحيح d_int = roundf (d_i * 16384.f);

        منطقي is_pinned = new_user->is_ټکی ();

        uint32_t varidx = new_varidx_mapping[i];
        بینښه outer, inner, توکی_شمېر_2;

        إذا (varidx == HB_OT_LAYOUT_NO_VARIATIONS_شاخص)
        {
          /* No existing avar2 mapping. Create new VarData. */
          outer = توکی_vars.جمع_کړه_vardata (1);
          inner = 0;
          توکی_شمېر_2 = 1;
          new_varidx_mapping[i] = (outer << 16) | inner;
          default_deltas[i] = 0.f; /* no prior default delta */
        }
        وإلا
        {
          outer = varidx >> 16;
          inner = varidx & 0xFFFF;
          توکی_شمېر_2 = توکی_vars.get_توکی_شمېر (outer);
        }

        /* Empty-region bias: d_int + round(defaultDelta) */
        صحيح bias = d_int + (صحيح) roundf (default_deltas[i]);
        إذا (bias != 0)
        {
          hb_hashmap_t<hb_نښان_t, Triple> تشه_region;
          توکی_vars.جمع_کړه_tuple (outer, std::move (تشه_region),
                               inner, bias, توکی_شمېر_2);
        }

        إذا (!is_pinned)
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
          hb_ویکټور_t<avar2_دننه_واټن_knot_t> knots;
          avar2_جمع_کړه_knot (knots, -1.0, (مزدوج) a_i + 1.0);
          avar2_جمع_کړه_knot (knots, 0.0, (مزدوج) d_i);
          avar2_جمع_کړه_knot (knots, 1.0, (مزدوج) b_i - 1.0);

          ثابت hb_ویکټور_t<Axis_ارزښت_Map> &new_mapping = new_mappings[i];

          عائم min_f = 0.f, def_f = 0.f, max_f = 0.f;
          إذا (likely (i < fvar_axes.اوږدتیا))
            fvar_axes[i].get_coordinates (min_f, def_f, max_f);
          مزدوج old_min = (مزدوج) min_f;
          مزدوج old_def = (مزدوج) def_f;
          مزدوج old_max = (مزدوج) max_f;

          /* If the axis default MOVED, inv_renorm also kinks where the OLD
           * default lands in the new space (the old intermediate coordinate
           * crosses 0 there), at
           *   z = z_old  ->  -z_old
           * Omitting that knot (as a plain two-tent encoding would) makes
           * interior coordinates wrong. */
          مزدوج z_old = avar2_normalize_ارزښت ((مزدوج) old_def,
                                                 (مزدوج) new_user->minimum,
                                                 (مزدوج) new_user->منځ,
                                                 (مزدوج) new_user->اعظمي);
          z_old = _avar2_map_new_mapping (new_mapping, z_old);
          z_old = (مزدوج) roundf ((عائم) (z_old * 16384.0)) / 16384.0;
          إذا (-1.0 < z_old && z_old < 1.0)
            avar2_جمع_کړه_knot (knots, z_old, -z_old);

          /* Interior avar v1 breakpoints inside the retained range each put
           * a kink in offset(z). Sampling only {-1, 0, +1, z_old} would
           * linearly interpolate across those kinks. The instantiated
           * mapping keeps exactly the in-range old breakpoints, and the new
           * mapping kinks at each one's output coordinate; add that z with
           * its old intermediate value so offset(z) is reproduced at every
           * kink. */
          hb_ویکټور_t<avar2_دننه_واټن_knot_t> with_breakpoints (knots);
          لكل (ثابت خپلکاری &m : new_mapping)
          {
            مزدوج from = (مزدوج) m.coords[0].منزل_float ();
            إذا (from == -1.0 || from == 0.0 || from == 1.0)
              استمر; /* anchors already seeded */
            مزدوج z = (مزدوج) m.coords[1].منزل_float ();
            إذا (!(-1.0 < z && z < 1.0))
              استمر;
            مزدوج user = avar2_denormalize_ارزښت (from,
                                                    (مزدوج) new_user->minimum,
                                                    (مزدوج) new_user->منځ,
                                                    (مزدوج) new_user->اعظمي);
            مزدوج n_old = avar2_normalize_ارزښت (user, old_min, old_def, old_max);
            مزدوج x_old = (مزدوج) old_seg_maps[i]->map_float ((عائم) n_old);
            x_old = (مزدوج) roundf ((عائم) (x_old * 16384.0)) / 16384.0;
            avar2_جمع_کړه_knot (with_breakpoints, z, x_old - z);
          }

          إذا (unlikely (knots.دننه_error () || with_breakpoints.دننه_error ()))
            أرجع خطأ;

          /* Extra tents cost F2Dot14 rounding, so for a steep segment they
           * can add more quantization noise than the structural error they
           * remove. Keep the interior breakpoints only when they do not
           * increase the estimated residual; this makes the collection a
           * strict (never-worse) improvement over the {-1, 0, +1, z_old}
           * anchors. Warn when even the better choice is not bit-exact (a
           * steep retained segment that cannot be reproduced in F2Dot14). */
          بینښه err = avar2_estimate_دننه_واټن_error (*old_seg_maps[i], new_mapping,
                                                       old_min, old_def, old_max,
                                                       (مزدوج) new_user->minimum,
                                                       (مزدوج) new_user->منځ,
                                                       (مزدوج) new_user->اعظمي,
                                                       knots);
          إذا (with_breakpoints.اوږدتیا != knots.اوږدتیا)
          {
            بینښه err_with = avar2_estimate_دننه_واټن_error (*old_seg_maps[i], new_mapping,
                                                              old_min, old_def, old_max,
                                                              (مزدوج) new_user->minimum,
                                                              (مزدوج) new_user->منځ,
                                                              (مزدوج) new_user->اعظمي,
                                                              with_breakpoints);
            إذا (err_with <= err)
            {
              knots = std::move (with_breakpoints);
              err = err_with;
            }
          }
          إذا (err > 8)
            DEBUG_پیغام (SUBSET, تش_اشاره,
                       "avar2 offset compensation is approximate for axis %c%c%c%c: "
                       "max residual %u F2Dot14 units",
                       HB_UNTAG (axis_نښان_2), err);

          /* Synthesize tents. Adjacent tents evaluate to zero at each
           * other's peaks, so each knot's delta is offset(z) - offset(0);
           * the base value offset(0) = d_i is carried by the empty-region
           * bias above. This reduces to the classic pair of tents
           * (-1,-1,0) / (0,+1,+1) when the default is unchanged and there
           * are no interior knots. */
          لكل (بینښه k = 0; k < knots.اوږدتیا; k++)
          {
            مزدوج z = knots.arrayZ[k].z;
            إذا (z == 0.0) استمر;
            صحيح delta = (صحيح) roundf ((عائم) ((knots.arrayZ[k].دننه_واټن - (مزدوج) d_i) * 16384.0));
            إذا (!delta) استمر;
            مزدوج ښکتنی, پاسنی;
            إذا (z > 0.0)
            {
              ښکتنی = knots.arrayZ[k - 1].z;
              پاسنی = k + 1 < knots.اوږدتیا ? knots.arrayZ[k + 1].z : z;
            }
            وإلا
            {
              پاسنی = knots.arrayZ[k + 1].z;
              ښکتنی = k > 0 ? knots.arrayZ[k - 1].z : z;
            }
            hb_hashmap_t<hb_نښان_t, Triple> region;
            إذا (unlikely (!region.سېټ (axis_نښان_2, Triple (ښکتنی, z, پاسنی))))
              أرجع خطأ;
            توکی_vars.جمع_کړه_tuple (outer, std::move (region),
                                 inner, delta, توکی_شمېر_2);
          }
        }
      }
      وإلا
      {
        /* Free or private axis — not being restricted.
         * If it has a non-zero default delta, add it back as a bias.
         * Skip if this (outer,inner) was already processed (shared varIdx). */
        uint32_t varidx = new_varidx_mapping[i];
        إذا (varidx == HB_OT_LAYOUT_NO_VARIATIONS_شاخص)
          استمر;

        إذا (processed_varidxes.has (varidx))
          استمر;
        processed_varidxes.جمع_کړه (varidx);

        بینښه outer = varidx >> 16;
        بینښه inner = varidx & 0xFFFF;
        صحيح dd = (صحيح) roundf (default_deltas[i]);
        إذا (dd != 0)
        {
          بینښه توکی_شمېر_2 = توکی_vars.get_توکی_شمېر (outer);
          hb_hashmap_t<hb_نښان_t, Triple> تشه_region;
          توکی_vars.جمع_کړه_tuple (outer, std::move (تشه_region),
                               inner, dd, توکی_شمېر_2);
        }
      }
    }

    /* 7. Finalize: build region list + convert to varstore */
    إذا (!توکی_vars.build_region_list ()) أرجع خطأ;
    إذا (!توکی_vars.as_توکی_varstore (ریښتیا /* optimize */,
                                     خطأ /* use_no_variation_idx */))
      أرجع خطأ;

    /* 8. Apply varidx_map optimization remapping */
    ثابت خپلکاری &opt_varidx_map = توکی_vars.get_varidx_map ();
    لكل (بینښه i = 0; i < axis_شمېر_2; i++)
    {
      uint32_t varidx = new_varidx_mapping[i];
      إذا (varidx == HB_OT_LAYOUT_NO_VARIATIONS_شاخص)
        استمر;
      uint32_t *new_idx;
      إذا (opt_varidx_map.has (varidx, &new_idx))
        new_varidx_mapping[i] = *new_idx;
    }

    /* 9. Serialize avarV2Tail. Entries cover the retained axes only;
     * self-contained pinned axes are removed from fvar. */
    avar2_شاخص_map_plan_t شاخص_map_plan;
    إذا (!شاخص_map_plan.init (new_varidx_mapping,
			      c->plan->axes_شاخص_map,
			      axis_شمېر_2))
      أرجع خطأ;

    خپلکاری *لکۍ = c->serializer->allocate_اندازه<avar_V2_لکۍ> (avar_V2_لکۍ::static_اندازه);
    إذا (unlikely (!لکۍ)) أرجع خطأ;
    إذا (!لکۍ->varIdxMap.serialize_serialize (c->serializer, شاخص_map_plan))
      أرجع خطأ;
    إذا (!لکۍ->varStore.serialize_serialize (c->serializer,
					     توکی_vars.has_long_کلمه (),
					     c->plan->axis_نښانونه,
					     توکی_vars.get_region_list (),
					     توکی_vars.get_vardata_encodings ()))
      أرجع خطأ;

    أرجع ریښتیا;
#د_که_پای
  }

  عام:

  محمي:
  Fixed_نسخه<>نسخه;	/* Version of the avar table
				 * initially set to 0x00010000u */
  HBUINT16	reserved;	/* This field is permanently reserved. Set to 0. */
  HBUINT16	axis_شمېر_2;	/* The number of variation axes in the font. This
				 * must be the same number as axisCount in the
				 * 'fvar' table. */
  SegmentMaps	لومړی_Axis_Segment_Maps;

  عام:
  DEFINE_اندازه_MIN (8);
};

} /* namespace OT */


#د_که_پای /* HB_OT_VAR_AVAR_TABLE_HH */
