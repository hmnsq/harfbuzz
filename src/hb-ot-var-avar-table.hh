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

#अगर_अघोषित HB_OT_VAR_AVAR_TABLE_HH
#घोषणा HB_OT_VAR_AVAR_TABLE_HH

#समावेश "hb-open-type.hh"
#समावेश "hb-ot-var-common.hh"
#समावेश "hb-ot-var-fvar-table.hh"


/*
 * avar -- Axis Variations
 * https://docs.microsoft.com/en-us/typography/opentype/spec/avar
 */

#घोषणा HB_OT_TAG_avar HB_TAG('a','v','a','r')


namespace OT {


/* "Spec": https://github.com/be-fonts/boring-expansion-spec/issues/14 */
काष्ठा avar_V2_पुच्छ
{
  मित्र काष्ठा avar;

  bool sanitize (hb_sanitize_context_t *c,
		 स्थिर व्योम *base) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (varIdxMap.sanitize (c, base) &&
		  varStore.sanitize (c, base));
  }

  रक्षित:
  Offset32To<Delta_Set_सूचकांक_Map>	varIdxMap;	/* Offset from the beginning of 'avar' table. */
  Offset32To<ItemVariationStore>	varStore;	/* Offset from the beginning of 'avar' table. */

  खुला:
  DEFINE_आकार_STATIC (8);
};


काष्ठा Axis_मूल्य_Map
{
  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह));
  }

  व्योम set_mapping (भग्न from_coord, भग्न to_coord)
  {
    coords[0].set_float (from_coord);
    coords[1].set_float (to_coord);
  }

  bool is_outside_axis_range (स्थिर Triple& axis_range) स्थिर
  {
    द्विगुन from_coord = (द्विगुन) coords[0].to_float ();
    वापस !axis_range.contains (from_coord);
  }

  bool must_include () स्थिर
  {
    भग्न from_coord = coords[0].to_float ();
    भग्न to_coord = coords[1].to_float ();
    वापस (from_coord == -1.f && to_coord == -1.f) ||
           (from_coord == 0.f && to_coord == 0.f) ||
           (from_coord == 1.f && to_coord == 1.f);
  }

  व्योम instantiate (स्थिर Triple& axis_range,
                    स्थिर Triple& unmapped_range,
                    स्थिर TripleDistances& triple_distances)
  {
    भग्न from_coord = coords[0].to_float ();
    भग्न to_coord = coords[1].to_float ();

    from_coord = renormalize_मूल्य ((द्विगुन) from_coord, unmapped_range, triple_distances);
    to_coord = renormalize_मूल्य ((द्विगुन) to_coord, axis_range, triple_distances);

    /* Malformed input can renormalize outside [-1,1], which would
     * overflow F2DOT14. */
    from_coord = hb_clamp (from_coord, -1.f, 1.f);
    to_coord = hb_clamp (to_coord, -1.f, 1.f);

    coords[0].set_float (from_coord);
    coords[1].set_float (to_coord);
  }

  HB_INTERNAL अटल पूर्णांक cmp (स्थिर व्योम *pa, स्थिर व्योम *pb)
  {
    स्थिर Axis_मूल्य_Map *a = (स्थिर Axis_मूल्य_Map *) pa;
    स्थिर Axis_मूल्य_Map *b = (स्थिर Axis_मूल्य_Map *) pb;

    पूर्णांक a_from = a->coords[0].to_int ();
    पूर्णांक b_from = b->coords[0].to_int ();
    अगर (a_from != b_from)
      वापस a_from - b_from;

    /* this should never be reached. according to the spec, all of the axis
     * value map records for a given axis must have different fromCoord values
     * */
    पूर्णांक a_to = a->coords[1].to_int ();
    पूर्णांक b_to = b->coords[1].to_int ();
    वापस a_to - b_to;
  }

  bool serialize (hb_serialize_context_t *c) स्थिर
  {
    TRACE_SERIALIZE (यह);
    return_trace (c->embed (यह));
  }

  खुला:
  F2DOT14	coords[2];
//   F2DOT14	fromCoord;	/* A normalized coordinate value obtained using
//				 * default normalization. */
//   F2DOT14	toCoord;	/* The modified, normalized coordinate value. */

  खुला:
  DEFINE_आकार_STATIC (4);
};

काष्ठा SegmentMaps : Array16Of<Axis_मूल्य_Map>
{
  भग्न map_float (भग्न value, अचिन्हित पूर्णांक from_offset = 0, अचिन्हित पूर्णांक to_offset = 1) स्थिर
  {
#घोषणा fromCoord coords[from_offset].to_float ()
#घोषणा toCoord coords[to_offset].to_float ()

    स्थिर स्वतः *map = arrayZ;

    /* The following special-cases are not part of OpenType, which requires
     * that at least -1, 0, and +1 must be mapped. But we include these as
     * part of a better error recovery scheme. */
    अगर (len < 2)
    {
      अगर (!len)
	वापस value;
      अन्यथा /* len == 1*/
	वापस value - map[0].fromCoord + map[0].toCoord;
    }

    // At least two mappings now.

    /* CoreText is wild...
     * PingFangUI avar needs all this special-casing...
     * So we implement an extended version of the spec here,
     * which is more robust and more likely to be compatible with
     * the wild. */

    अचिन्हित start = 0;
    अचिन्हित end = len;
    अगर (map[start].fromCoord == -1 && map[start].toCoord == -1 && map[start+1].fromCoord == -1)
      start++;
    अगर (map[end-1].fromCoord == +1 && map[end-1].toCoord == +1 && map[end-2].fromCoord == +1)
      end--;

    /* Look for exact match first, and do lots of special-casing. */
    अचिन्हित i;
    क्रम (i = start; i < end; i++)
      अगर (value == map[i].fromCoord)
	अवरोध;
    अगर (i < end)
    {
      // There's at least one exact match. See if there are more.
      अचिन्हित j = i;
      क्रम (; j + 1 < end; j++)
	अगर (value != map[j + 1].fromCoord)
	  अवरोध;

      // [i,j] inclusive are all exact matches:

      // If there's only one, return it. This is the only spec-compliant case.
      अगर (i == j)
	वापस map[i].toCoord;
      // If there's exactly three, return the middle one.
      अगर (i + 2 == j)
	वापस map[i + 1].toCoord;

      // Ignore the middle ones. Return the one mapping closer to 0.
      अगर (value < 0) वापस map[j].toCoord;
      अगर (value > 0) वापस map[i].toCoord;

      // Mapping 0? CoreText seems confused. It seems to prefer 0 here...
      // So we'll just return the smallest one. lol
      वापस fabsf (map[i].toCoord) < fabsf (map[j].toCoord) ? map[i].toCoord : map[j].toCoord;

      // Mapping 0? Return one not mapping to 0.
      अगर (map[i].toCoord == 0)
	वापस map[j].toCoord;
      अन्यथा
	वापस map[i].toCoord;
    }

    /* There's at least two and we're not an exact match. Prepare to lerp. */

    // Find the segment we're in.
    क्रम (i = start; i < end; i++)
      अगर (value < map[i].fromCoord)
	अवरोध;

    अगर (i == start)
    {
      // Value before all segments; Shift.
      वापस value - map[start].fromCoord + map[start].toCoord;
    }
    अगर (i == end)
    {
      // Value after all segments; Shift.
      वापस value - map[end - 1].fromCoord + map[end - 1].toCoord;
    }

    // Actually interpolate.
    स्वतः &before = map[i-1];
    स्वतः &after = map[i];
    भग्न denom = after.fromCoord - before.fromCoord; // Can't be zero by now.
    वापस before.toCoord + ((after.toCoord - before.toCoord) * (value - before.fromCoord)) / denom;

#अघोषित toCoord
#अघोषित fromCoord
  }

  भग्न unmap_float (भग्न value) स्थिर { वापस map_float (value, 1, 0); }


  // TODO Kill this.
  Triple unmap_axis_range (स्थिर Triple& axis_range) स्थिर
  {
    भग्न unmapped_min = unmap_float (axis_range.minimum);
    भग्न unmapped_मध्य = unmap_float (axis_range.मध्य);
    भग्न unmapped_max = unmap_float (axis_range.maximum);

    वापस Triple{unmapped_min, unmapped_मध्य, unmapped_max};
  }

  bool subset (hb_subset_context_t *c, hb_tag_t axis_tag,
               hb_vector_t<Axis_मूल्य_Map> *out_mappings = nullptr) स्थिर
  {
    TRACE_SUBSET (यह);

    /* This function cannot work on avar2 table (and currently doesn't).
     * We should instead keep the design coords in the shape plan and use
     * those. unmap_axis_range needs to be killed. */

    /* avar mapped normalized axis range. Under avar2, axes_location holds
     * only the self-contained pins for the other tables; avar itself uses
     * the intermediate-space ranges of all restricted axes. */
    स्थिर स्वतः &axes_location = c->plan->has_avar2
				? c->plan->old_intermediates
				: c->plan->axes_location;
    Triple *axis_range;
    अगर (!axes_location.has (axis_tag, &axis_range))
      वापस c->serializer->embed (*यह);

    TripleDistances *axis_triple_distances;
    अगर (!c->plan->axes_triple_distances.has (axis_tag, &axis_triple_distances))
      return_trace (false);

    स्वतः *out = c->serializer->start_embed (यह);
    अगर (unlikely (!c->serializer->extend_min (out))) return_trace (false);

    Triple unmapped_range = unmap_axis_range (*axis_range);

    /* create a vector of retained mappings and sort */
    hb_vector_t<Axis_मूल्य_Map> मूल्य_mappings;
    क्रम (स्थिर स्वतः& _ : as_array ())
    {
      अगर (_.is_outside_axis_range (unmapped_range))
        जारी;
      Axis_मूल्य_Map mapping;
      mapping = _;
      mapping.instantiate (*axis_range, unmapped_range, *axis_triple_distances);
      /* (-1, -1), (0, 0), (1, 1) mappings will be added later, so avoid
       * duplicates here */
      अगर (mapping.must_include ())
        जारी;
      मूल्य_mappings.push (mapping);
    }

    Axis_मूल्य_Map m;
    m.set_mapping (-1.f, -1.f);
    मूल्य_mappings.push (m);

    m.set_mapping (0.f, 0.f);
    मूल्य_mappings.push (m);

    m.set_mapping (1.f, 1.f);
    मूल्य_mappings.push (m);

    मूल्य_mappings.क्विक ();

    अगर (unlikely (मूल्य_mappings.in_error ()))
      return_trace (false);

    क्रम (स्थिर स्वतः& _ : मूल्य_mappings)
    {
      अगर (!_.serialize (c->serializer))
        return_trace (false);
    }
    अगर (!c->serializer->check_assign (out->len, मूल्य_mappings.लंबाई, HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (false);
    /* Hand the instantiated mappings to the caller; avar2 offset
     * compensation needs them to locate the new mapping's kinks. */
    अगर (out_mappings)
      *out_mappings = std::move (मूल्य_mappings);
    return_trace (true);
  }

  खुला:
  DEFINE_आकार_ARRAY (2, *यह);
};

/* One knot of the avar2 offset-compensation function
 * offset(z) = inv_renorm(z) - z, in new intermediate space. */
काष्ठा avar2_offset_knot_t
{
  द्विगुन z;
  द्विगुन offset;
};

/* Insert a knot keeping the vector sorted by z; first insertion wins on
 * duplicate z. */
अटल अंतरभूत व्योम
_avar2_add_knot (hb_vector_t<avar2_offset_knot_t> &knots, द्विगुन z, द्विगुन offset)
{
  अचिन्हित i = 0;
  जबतक (i < knots.लंबाई && knots.arrayZ[i].z < z) i++;
  अगर (i < knots.लंबाई && knots.arrayZ[i].z == z) वापस;
  knots.push (avar2_offset_knot_t {0.0, 0.0});
  अगर (unlikely (knots.in_error ())) वापस;
  क्रम (अचिन्हित j = knots.लंबाई - 1; j > i; j--)
    knots.arrayZ[j] = knots.arrayZ[j - 1];
  knots.arrayZ[i] = avar2_offset_knot_t {z, offset};
}

/* Piecewise-linear evaluation over the sorted knots. Inputs are always
 * within [-1, +1] and anchor knots at -1/0/+1 always exist. */
अटल अंतरभूत द्विगुन
_avar2_eval_offset (स्थिर hb_vector_t<avar2_offset_knot_t> &knots, द्विगुन z)
{
  अचिन्हित len = knots.लंबाई;
  क्रम (अचिन्हित i = 0; i < len; i++)
  {
    अगर (z == knots.arrayZ[i].z) वापस knots.arrayZ[i].offset;
    अगर (z < knots.arrayZ[i].z)
    {
      अगर (!i) वापस knots.arrayZ[0].offset;
      स्थिर स्वतः &before = knots.arrayZ[i - 1];
      स्थिर स्वतः &after = knots.arrayZ[i];
      द्विगुन denom = after.z - before.z;
      वापस before.offset + (after.offset - before.offset) * (z - before.z) / denom;
    }
  }
  वापस len ? knots.arrayZ[len - 1].offset : 0.0;
}

/* Piecewise-linear evaluation over instantiated avar v1 mappings, as
 * produced by SegmentMaps::subset (sorted, with -1/0/+1 anchors).
 * Matches SegmentMaps::map_float for such well-formed mappings. */
अटल अंतरभूत द्विगुन
_avar2_map_new_mapping (स्थिर hb_vector_t<Axis_मूल्य_Map> &mappings, द्विगुन v,
			अचिन्हित from_offset = 0, अचिन्हित to_offset = 1)
{
  अचिन्हित len = mappings.लंबाई;
  अगर (!len) वापस v;

  क्रम (अचिन्हित i = 0; i < len; i++)
  {
    द्विगुन from = (द्विगुन) mappings.arrayZ[i].coords[from_offset].to_float ();
    अगर (v == from)
      वापस (द्विगुन) mappings.arrayZ[i].coords[to_offset].to_float ();
    अगर (v < from)
    {
      द्विगुन to = (द्विगुन) mappings.arrayZ[i].coords[to_offset].to_float ();
      अगर (!i) वापस v - from + to;
      द्विगुन पूर्व_from = (द्विगुन) mappings.arrayZ[i - 1].coords[from_offset].to_float ();
      द्विगुन पूर्व_to = (द्विगुन) mappings.arrayZ[i - 1].coords[to_offset].to_float ();
      द्विगुन denom = from - पूर्व_from;
      अगर (denom == 0.0) वापस पूर्व_to;
      वापस पूर्व_to + (to - पूर्व_to) * (v - पूर्व_from) / denom;
    }
  }
  वापस v - (द्विगुन) mappings.arrayZ[len - 1].coords[from_offset].to_float ()
           + (द्विगुन) mappings.arrayZ[len - 1].coords[to_offset].to_float ();
}

/* Inverse of _avar2_map_new_mapping: pull an output coordinate back to a
 * preimage input coordinate. For a non-strictly-monotone mapping this picks
 * one preimage, which is fine for its only use (augmenting the error
 * estimator's sample set). */
अटल अंतरभूत द्विगुन
_avar2_unmap_new_mapping (स्थिर hb_vector_t<Axis_मूल्य_Map> &mappings, द्विगुन v)
{
  वापस _avar2_map_new_mapping (mappings, v, 1, 0);
}

/* Plain (non-avar) fvar-style normalization of a user value against a
 * user-space (min, default, max) triple. */
अटल अंतरभूत द्विगुन
avar2_normalize_मूल्य (द्विगुन v, द्विगुन min, द्विगुन def, द्विगुन max)
{
  v = hb_clamp (v, min, max);
  अगर (v == def) वापस 0.0;
  अगर (v < def) वापस def == min ? 0.0 : (v - def) / (def - min);
  वापस def == max ? 0.0 : (v - def) / (max - def);
}

/* Inverse of the above: map a normalized value back to user space. */
अटल अंतरभूत द्विगुन
avar2_denormalize_मूल्य (द्विगुन v, द्विगुन min, द्विगुन def, द्विगुन max)
{
  अगर (v == 0.0) वापस def;
  वापस v < 0.0 ? def + v * (def - min) : def + v * (max - def);
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
अटल अंतरभूत अचिन्हित
_avar2_estimate_offset_error (स्थिर SegmentMaps &old_seg,
                              स्थिर hb_vector_t<Axis_मूल्य_Map> &new_mapping,
                              द्विगुन old_min, द्विगुन old_def, द्विगुन old_max,
                              द्विगुन new_min, द्विगुन new_def, द्विगुन new_max,
                              स्थिर hb_vector_t<avar2_offset_knot_t> &knots)
{
  constexpr अचिन्हित samples = 257;
  hb_vector_t<द्विगुन> us;
  अगर (unlikely (!us.alloc (samples + old_seg.as_array ().लंबाई +
                           new_mapping.लंबाई + knots.लंबाई)))
    वापस अच_पूर्णांक_उच्च; /* estimator only; fail towards "worse" */
  क्रम (अचिन्हित i = 0; i < samples; i++)
    us.push (new_min + (new_max - new_min) * i / (samples - 1));
  क्रम (स्थिर स्वतः &_ : old_seg.as_array ())
    us.push (avar2_denormalize_मूल्य ((द्विगुन) _.coords[0].to_float (),
                                       old_min, old_def, old_max));
  क्रम (स्थिर स्वतः &_ : new_mapping)
    us.push (avar2_denormalize_मूल्य ((द्विगुन) _.coords[0].to_float (),
                                       new_min, new_def, new_max));
  क्रम (स्थिर स्वतः &knot : knots)
    us.push (avar2_denormalize_मूल्य (
               _avar2_unmap_new_mapping (new_mapping, knot.z),
               new_min, new_def, new_max));
  अगर (unlikely (us.in_error ())) वापस अच_पूर्णांक_उच्च;

  अचिन्हित max_err = 0;
  क्रम (द्विगुन u : us)
  {
    अगर (u < new_min || u > new_max) जारी;
    द्विगुन n_old = avar2_normalize_मूल्य (u, old_min, old_def, old_max);
    द्विगुन old_final = (द्विगुन) old_seg.map_float ((भग्न) n_old);
    द्विगुन n_new = avar2_normalize_मूल्य (u, new_min, new_def, new_max);
    द्विगुन z = _avar2_map_new_mapping (new_mapping, n_new);
    द्विगुन new_final = z + _avar2_eval_offset (knots, z);
    पूर्णांक err = असल ((पूर्णांक) roundf ((भग्न) (old_final * 16384.0)) -
                   (पूर्णांक) roundf ((भग्न) (new_final * 16384.0)));
    अगर ((अचिन्हित) err > max_err) max_err = err;
  }
  वापस max_err;
}

काष्ठा avar
{
  अटल constexpr hb_tag_t tableTag = HB_OT_TAG_avar;

  bool has_तथ्य () स्थिर { वापस version.to_int (); }

  स्थिर SegmentMaps* get_segment_maps () स्थिर
  { वापस &firstAxisSegmentMaps; }

  अचिन्हित get_axis_गिनती () स्थिर
  { वापस axis_गिनती_2; }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    अगर (!(version.sanitize (c) &&
	  hb_barrier () &&
	  (version.major == 1
#अगर_अघोषित HB_NO_AVAR2
	   || version.major == 2
#पूर्ण_अगर
	   ) &&
	  c->check_struct (यह)))
      return_trace (false);

    स्थिर SegmentMaps *map = &firstAxisSegmentMaps;
    अचिन्हित पूर्णांक गिनती = axis_गिनती_2;
    क्रम (अचिन्हित पूर्णांक i = 0; i < गिनती; i++)
    {
      अगर (unlikely (!map->sanitize (c)))
	return_trace (false);
      map = &StructAfter<SegmentMaps> (*map);
    }

#अगर_अघोषित HB_NO_AVAR2
    अगर (version.major < 2)
      return_trace (true);
    hb_barrier ();

    स्थिर स्वतः &v2 = * (स्थिर avar_V2_पुच्छ *) map;
    अगर (unlikely (!v2.sanitize (c, यह)))
      return_trace (false);
#पूर्ण_अगर

    return_trace (true);
  }

  व्योम map_coords_16_16 (पूर्णांक *coords, अचिन्हित पूर्णांक coords_लंबाई) स्थिर
  {
    अचिन्हित पूर्णांक गिनती = hb_min (coords_लंबाई, axis_गिनती_2);

    स्थिर SegmentMaps *map = &firstAxisSegmentMaps;
    क्रम (अचिन्हित पूर्णांक i = 0; i < गिनती; i++)
    {
      coords[i] = roundf (map->map_float (coords[i] / 65536.f) * 65536.f);
      map = &StructAfter<SegmentMaps> (*map);
    }

#अगर_अघोषित HB_NO_AVAR2
    अगर (version.major < 2)
      वापस;
    hb_barrier ();

    क्रम (; गिनती < axis_गिनती_2; गिनती++)
      map = &StructAfter<SegmentMaps> (*map);

    स्थिर स्वतः &v2 = * (स्थिर avar_V2_पुच्छ *) map;

    स्थिर स्वतः &varidx_map = यह+v2.varIdxMap;
    स्थिर स्वतः &var_store = यह+v2.varStore;
    स्वतः *var_store_cache = var_store.create_cache ();

    hb_vector_t<पूर्णांक> coords_2_14;
    coords_2_14.resize (coords_लंबाई);
    क्रम (अचिन्हित i = 0; i < coords_लंबाई; i++)
      coords_2_14[i] = roundf (coords[i] / 4.f); // 16.16 -> 2.14

    hb_vector_t<पूर्णांक> out;
    out.alloc (coords_लंबाई);
    क्रम (अचिन्हित i = 0; i < coords_लंबाई; i++)
    {
      पूर्णांक v = coords[i];
      uint32_t varidx = varidx_map.map (i);
      भग्न delta = var_store.get_delta (varidx, coords_2_14.arrayZ, coords_2_14.लंबाई, var_store_cache);
      /* Apply the delta unclamped and clamp only the result to [-1, +1],
       * matching fontTools. Since inputs and results are in [-1, +1],
       * deltas beyond ±2 are equivalent to ±2; clamp to that range only
       * to keep the float->int conversion safe. */
      भग्न d = hb_clamp (delta * 4, -(भग्न) (1<<17), +(भग्न) (1<<17)); // 2.14 -> 16.16
      v += (पूर्णांक) roundf (d);
      v = hb_clamp (v, -(1<<16), +(1<<16));
      out.push (v);
    }
    क्रम (अचिन्हित i = 0; i < coords_लंबाई; i++)
      coords[i] = out[i];

    OT::ItemVariationStore::destroy_cache (var_store_cache);
#पूर्ण_अगर
  }

  /* An avar version 2 font is never downgraded to v1: even with a NULL
   * varStore offset (legal; maps like plain v1) the avar2 subsetting path
   * applies, with "no variation" semantics for rows that don't resolve —
   * offset compensation then creates delta rows on demand. */
  bool has_v2_तथ्य () स्थिर { वापस version.major > 1; }

  /* Resolve the avar2 VarStore and VarIdxMap, for the subset planner.
   * Either pointer may be to the Null object (nullable offsets). */
  bool get_v2_store_and_map (स्थिर ItemVariationStore **store,
                             स्थिर Delta_Set_सूचकांक_Map **varidx_map) स्थिर
  {
#अगर_अघोषित HB_NO_AVAR2
    अगर (version.major < 2) वापस false;
    स्थिर SegmentMaps *map = &firstAxisSegmentMaps;
    क्रम (अचिन्हित i = 0; i < axis_गिनती_2; i++)
      map = &StructAfter<SegmentMaps> (*map);
    स्थिर स्वतः &v2 = * (स्थिर avar_V2_पुच्छ *) map;
    *store = &(यह+v2.varStore);
    *varidx_map = &(यह+v2.varIdxMap);
    वापस true;
#अन्यथा
    वापस false;
#पूर्ण_अगर
  }

  // axis normalization is done in 2.14 here
  // TODO: deprecate this API once fonttools is updated to use 16.16 normalization
  bool map_coords_2_14 (भग्न *coords, अचिन्हित पूर्णांक coords_लंबाई,
			bool v1_only = false) स्थिर
  {
    hb_vector_t<पूर्णांक> coords_2_14;
    अगर (!v1_only && !coords_2_14.resize (coords_लंबाई)) वापस false;
    अचिन्हित पूर्णांक गिनती = hb_min (coords_लंबाई, axis_गिनती_2);

    स्थिर SegmentMaps *map = &firstAxisSegmentMaps;
    क्रम (अचिन्हित पूर्णांक i = 0; i < गिनती; i++)
    {
      पूर्णांक v = roundf (map->map_float (coords[i]) * 16384.f);
      अगर (!v1_only)
	coords_2_14[i] = v;
      coords[i] = v / 16384.f;
      map = &StructAfter<SegmentMaps> (*map);
    }

    अगर (v1_only)
      वापस true;

#अगर_अघोषित HB_NO_AVAR2
    अगर (version.major < 2)
      वापस true;
    hb_barrier ();

    क्रम (; गिनती < axis_गिनती_2; गिनती++)
      map = &StructAfter<SegmentMaps> (*map);

    स्थिर स्वतः &v2 = * (स्थिर avar_V2_पुच्छ *) map;

    स्थिर स्वतः &varidx_map = यह+v2.varIdxMap;
    स्थिर स्वतः &var_store = यह+v2.varStore;
    स्वतः *var_store_cache = var_store.create_cache ();

    क्रम (अचिन्हित i = 0; i < coords_लंबाई; i++)
    {
      पूर्णांक v = coords_2_14[i];
      uint32_t varidx = varidx_map.map (i);
      भग्न delta = var_store.get_delta (varidx, coords_2_14.arrayZ, coords_2_14.लंबाई, var_store_cache);
      /* As above: apply the delta unclamped (±2 covers every useful case)
       * and clamp the result to [-1, +1], matching fontTools. */
      v += (पूर्णांक) hb_clamp (roundf (delta), -(भग्न) (1<<15), +(भग्न) (1<<15));
      v = hb_clamp (v, -(1<<14), +(1<<14));
      coords[i] = v / 16384.f;
    }

    OT::ItemVariationStore::destroy_cache (var_store_cache);
    वापस true;
#अन्यथा
    वापस version.major < 2;
#पूर्ण_अगर
  }

  bool subset (hb_subset_context_t *c) स्थिर
  {
    TRACE_SUBSET (यह);
    अचिन्हित retained_axis_गिनती = c->plan->axes_सूचकांक_map.get_population ();
    अगर (!retained_axis_गिनती) //all axes are pinned/dropped
      return_trace (false);

    avar *out = c->serializer->allocate_min<avar> ();
    अगर (unlikely (!out)) return_trace (false);

    out->version.major = c->plan->has_avar2 ? 2 : 1;
    out->version.minor = 0;
    अगर (!c->serializer->check_assign (out->axis_गिनती_2, retained_axis_गिनती, HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (false);

    /* For avar2, keep the instantiated v1 mappings around; offset
     * compensation needs them to locate the new mappings' kinks. */
    hb_vector_t<hb_vector_t<Axis_मूल्य_Map>> new_mappings;
    अगर (c->plan->has_avar2 && !new_mappings.resize (axis_गिनती_2))
      return_trace (false);

    स्थिर hb_map_t& axes_सूचकांक_map = c->plan->axes_सूचकांक_map;
    स्थिर SegmentMaps *map = &firstAxisSegmentMaps;
    अचिन्हित गिनती = axis_गिनती_2;
    क्रम (अचिन्हित पूर्णांक i = 0; i < गिनती; i++)
    {
      अगर (axes_सूचकांक_map.has (i))
      {
        hb_tag_t *axis_tag;
        अगर (!c->plan->axes_old_सूचकांक_tag_map.has (i, &axis_tag))
          return_trace (false);

        Triple *axis_location;
        अगर (c->plan->has_avar2 &&
            c->plan->user_axes_location.has (*axis_tag, &axis_location) &&
            axis_location->is_point ())
        {
          /* Pinned axis in avar2 mode: serialize identity segment map
           * {-1->-1, 0->0, 1->1}. The axis is kept in fvar as hidden,
           * so avar needs a segment map entry for it. */
          स्वतः *identity_map = c->serializer->start_embed<SegmentMaps> ();
          अगर (unlikely (!c->serializer->extend_min (identity_map)))
            return_trace (false);
          Axis_मूल्य_Map m;
          m.set_mapping (-1.f, -1.f);
          अगर (!m.serialize (c->serializer)) return_trace (false);
          m.set_mapping (0.f, 0.f);
          अगर (!m.serialize (c->serializer)) return_trace (false);
          m.set_mapping (1.f, 1.f);
          अगर (!m.serialize (c->serializer)) return_trace (false);
          अगर (!c->serializer->check_assign (identity_map->len, 3u,
                                            HB_SERIALIZE_ERROR_INT_OVERFLOW))
            return_trace (false);
        }
        अन्यथा
        {
          /* Restricted or free axis: use standard SegmentMaps::subset() */
          अगर (!map->subset (c, *axis_tag,
                            c->plan->has_avar2 ? &new_mappings[i] : nullptr))
            return_trace (false);
        }
      }
      map = &StructAfter<SegmentMaps> (*map);
    }

    अगर (c->plan->has_avar2)
      return_trace (_subset_avar2 (c, new_mappings));

    return_trace (true);
  }

  निजी:
  काष्ठा avar2_सूचकांक_map_plan_t
  {
    bool init (स्थिर hb_vector_t<uint32_t> &varidx_mapping,
	       स्थिर hb_map_t &axes_सूचकांक_map,
	       अचिन्हित axis_गिनती)
    {
      अगर (!output_map.alloc (axes_सूचकांक_map.get_population ()))
	वापस false;

      bool has_no_variation = false;
      अचिन्हित max_outer = 0, max_inner = 0;
      क्रम (अचिन्हित i = 0; i < axis_गिनती; i++)
      {
	अगर (!axes_सूचकांक_map.has (i)) जारी;
	uint32_t varidx = varidx_mapping[i];
	output_map.push (varidx);
	अगर (varidx == HB_OT_LAYOUT_NO_VARIATIONS_सूचकांक)
	{
	  has_no_variation = true;
	  जारी;
	}
	max_outer = hb_max (max_outer, varidx >> 16);
	max_inner = hb_max (max_inner, varidx & 0xFFFF);
      }
      अगर (output_map.in_error () ||
	  output_map.लंबाई != axes_सूचकांक_map.get_population ())
	वापस false;

      अगर (has_no_variation)
      {
	width = 4;
	inner_bit_गिनती = 16;
      }
      अन्यथा
      {
	inner_bit_गिनती = hb_max (1u, hb_bit_storage (max_inner));
	अचिन्हित outer_bit_गिनती = hb_max (1u, hb_bit_storage (max_outer));
	width = hb_clamp ((inner_bit_गिनती + outer_bit_गिनती + 7) / 8, 1u, 4u);
	अगर (inner_bit_गिनती + outer_bit_गिनती > width * 8)
	  inner_bit_गिनती = width * 8 - outer_bit_गिनती;
      }
      वापस true;
    }

    अचिन्हित get_inner_bit_गिनती () स्थिर { वापस inner_bit_गिनती; }
    अचिन्हित get_width () स्थिर { वापस width; }
    hb_array_t<स्थिर uint32_t> get_output_map () स्थिर { वापस output_map.as_array (); }

    अचिन्हित inner_bit_गिनती = 1;
    अचिन्हित width = 1;
    hb_vector_t<uint32_t> output_map;
  };

  bool _subset_avar2 (hb_subset_context_t *c,
                      स्थिर hb_vector_t<hb_vector_t<Axis_मूल्य_Map>> &new_mappings) स्थिर
  {
#अगर defined (HB_NO_VAR) || defined (HB_NO_AVAR2)
    /* Not reachable: the plan never sets has_avar2 in these configurations. */
    वापस false;
#अन्यथा

    /* 1. Locate original avar2 data, keeping per-axis old segment maps */
    hb_vector_t<स्थिर SegmentMaps *> old_seg_maps;
    अगर (!old_seg_maps.alloc (axis_गिनती_2)) वापस false;
    स्थिर SegmentMaps *map = &firstAxisSegmentMaps;
    क्रम (अचिन्हित i = 0; i < axis_गिनती_2; i++)
    {
      old_seg_maps.push (map);
      map = &StructAfter<SegmentMaps> (*map);
    }

    स्थिर स्वतः &v2 = * (स्थिर avar_V2_पुच्छ *) map;
    स्थिर स्वतः &varidx_map = यह+v2.varIdxMap;
    स्थिर स्वतः &var_store = यह+v2.varStore;

    स्वतः fvar_axes = c->plan->source->table.fvar->get_axes ();

    /* 2. Compute default deltas by evaluating VarStore at old defaults */
    hb_vector_t<पूर्णांक> default_coords;
    अगर (!default_coords.resize (axis_गिनती_2)) वापस false;
    क्रम (अचिन्हित i = 0; i < axis_गिनती_2; i++)
    {
      hb_tag_t *axis_tag;
      अगर (c->plan->axes_old_सूचकांक_tag_map.has (i, &axis_tag) &&
          c->plan->old_intermediates.has (*axis_tag))
      {
        भग्न d_i = (भग्न) c->plan->old_intermediates.लो (*axis_tag).मध्य;
        default_coords[i] = roundf (d_i * 16384.f);
      }
      अन्यथा
        default_coords[i] = 0;
    }

    स्वतः *store_cache = var_store.create_cache ();
    hb_vector_t<भग्न> default_deltas;
    अगर (!default_deltas.resize (axis_गिनती_2)) {
      ItemVariationStore::destroy_cache (store_cache);
      वापस false;
    }
    क्रम (अचिन्हित i = 0; i < axis_गिनती_2; i++)
    {
      uint32_t varidx = varidx_map.map (i);
      अगर (varidx == HB_OT_LAYOUT_NO_VARIATIONS_सूचकांक)
        default_deltas[i] = 0.f;
      अन्यथा
        default_deltas[i] = var_store.get_delta (varidx, default_coords.arrayZ,
                                                  default_coords.लंबाई, store_cache);
    }
    ItemVariationStore::destroy_cache (store_cache);

    /* 3. Rebase IVS regions */
    item_variations_t item_vars;
    अगर (!item_vars.create_from_item_varstore (var_store, c->plan->axes_old_सूचकांक_tag_map))
      वापस false;
    अगर (!item_vars.instantiate_tuple_vars (c->plan->old_intermediates,
                                           c->plan->axes_triple_distances,
                                           false))
      वापस false;

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
    अगर (!new_varidx_mapping.resize (axis_गिनती_2)) वापस false;
    क्रम (अचिन्हित i = 0; i < axis_गिनती_2; i++)
    {
      uint32_t varidx = varidx_map.map (i);
      अगर (varidx != HB_OT_LAYOUT_NO_VARIATIONS_सूचकांक &&
	  !var_store.has_delta_set (varidx))
	varidx = HB_OT_LAYOUT_NO_VARIATIONS_सूचकांक;
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
    hb_hashmap_t<uint32_t, अचिन्हित> varidx_ref_गिनती;
    क्रम (अचिन्हित i = 0; i < axis_गिनती_2; i++)
    {
      अगर (!c->plan->axes_सूचकांक_map.has (i)) जारी; /* self-contained: dropped */
      uint32_t varidx = new_varidx_mapping[i];
      अगर (varidx == HB_OT_LAYOUT_NO_VARIATIONS_सूचकांक) जारी;
      अचिन्हित *गिनती;
      अगर (varidx_ref_गिनती.has (varidx, &गिनती))
        (*गिनती)++;
      अन्यथा अगर (!varidx_ref_गिनती.set (varidx, 1))
        वापस false;
    }
    क्रम (अचिन्हित i = 0; i < axis_गिनती_2; i++)
    {
      hb_tag_t *axis_tag_ptr;
      अगर (!c->plan->axes_old_सूचकांक_tag_map.has (i, &axis_tag_ptr))
        वापस false;
      /* Only axes that will receive offset compensation (restricted or
       * pinned) can contaminate a shared row. */
      अगर (!c->plan->axes_सूचकांक_map.has (i) ||
          !c->plan->user_axes_location.has (*axis_tag_ptr))
        जारी;
      uint32_t varidx = new_varidx_mapping[i];
      अगर (varidx == HB_OT_LAYOUT_NO_VARIATIONS_सूचकांक)
        जारी; /* gets a fresh, private VarData in the offset loop below */
      अचिन्हित *गिनती;
      अगर (!varidx_ref_गिनती.has (varidx, &गिनती) || *गिनती <= 1)
        जारी; /* sole owner: safe to write offsets in place */
      अचिन्हित outer = varidx >> 16;
      अचिन्हित new_inner = item_vars.duplicate_row (outer, varidx & 0xFFFF);
      अगर (unlikely (new_inner == (अचिन्हित) -1)) वापस false;
      new_varidx_mapping[i] = (outer << 16) | new_inner;
      (*गिनती)--;
    }

    /* 6. Add offset compensation tuples.
     * Track processed (outer,inner) pairs to avoid adding duplicate biases
     * when multiple axes share the same varIdx. */
    hb_set_t processed_varidxes;
    क्रम (अचिन्हित i = 0; i < axis_गिनती_2; i++)
    {
      hb_tag_t *axis_tag_ptr;
      अगर (!c->plan->axes_old_सूचकांक_tag_map.has (i, &axis_tag_ptr))
        वापस false;
      hb_tag_t axis_tag = *axis_tag_ptr;

      /* Self-contained pinned axes are removed from fvar/avar; their
       * contribution is baked into the variation tables instead. */
      अगर (!c->plan->axes_सूचकांक_map.has (i))
        जारी;

      Triple *new_user;
      अगर (c->plan->user_axes_location.has (axis_tag, &new_user))
      {
        /* This axis is being restricted or pinned */
        Triple *old_int;
        अगर (!c->plan->old_intermediates.has (axis_tag, &old_int))
          वापस false;

        भग्न a_i = (भग्न) old_int->minimum;
        भग्न d_i = (भग्न) old_int->मध्य;
        भग्न b_i = (भग्न) old_int->maximum;

        पूर्णांक d_int = roundf (d_i * 16384.f);

        bool is_pinned = new_user->is_point ();

        uint32_t varidx = new_varidx_mapping[i];
        अचिन्हित outer, inner, item_गिनती_2;

        अगर (varidx == HB_OT_LAYOUT_NO_VARIATIONS_सूचकांक)
        {
          /* No existing avar2 mapping. Create new VarData. */
          outer = item_vars.add_vardata (1);
          inner = 0;
          item_गिनती_2 = 1;
          new_varidx_mapping[i] = (outer << 16) | inner;
          default_deltas[i] = 0.f; /* no prior default delta */
        }
        अन्यथा
        {
          outer = varidx >> 16;
          inner = varidx & 0xFFFF;
          item_गिनती_2 = item_vars.get_item_गिनती (outer);
        }

        /* Empty-region bias: d_int + round(defaultDelta) */
        पूर्णांक bias = d_int + (पूर्णांक) roundf (default_deltas[i]);
        अगर (bias != 0)
        {
          hb_hashmap_t<hb_tag_t, Triple> empty_region;
          item_vars.add_tuple (outer, std::move (empty_region),
                               inner, bias, item_गिनती_2);
        }

        अगर (!is_pinned)
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
          _avar2_add_knot (knots, -1.0, (द्विगुन) a_i + 1.0);
          _avar2_add_knot (knots, 0.0, (द्विगुन) d_i);
          _avar2_add_knot (knots, 1.0, (द्विगुन) b_i - 1.0);

          स्थिर hb_vector_t<Axis_मूल्य_Map> &new_mapping = new_mappings[i];

          भग्न min_f = 0.f, def_f = 0.f, max_f = 0.f;
          अगर (likely (i < fvar_axes.लंबाई))
            fvar_axes[i].get_coordinates (min_f, def_f, max_f);
          द्विगुन old_min = (द्विगुन) min_f;
          द्विगुन old_def = (द्विगुन) def_f;
          द्विगुन old_max = (द्विगुन) max_f;

          /* If the axis default MOVED, inv_renorm also kinks where the OLD
           * default lands in the new space (the old intermediate coordinate
           * crosses 0 there), at
           *   z = z_old  ->  -z_old
           * Omitting that knot (as a plain two-tent encoding would) makes
           * interior coordinates wrong. */
          द्विगुन z_old = avar2_normalize_मूल्य ((द्विगुन) old_def,
                                                 (द्विगुन) new_user->minimum,
                                                 (द्विगुन) new_user->मध्य,
                                                 (द्विगुन) new_user->maximum);
          z_old = _avar2_map_new_mapping (new_mapping, z_old);
          z_old = (द्विगुन) roundf ((भग्न) (z_old * 16384.0)) / 16384.0;
          अगर (-1.0 < z_old && z_old < 1.0)
            _avar2_add_knot (knots, z_old, -z_old);

          /* Interior avar v1 breakpoints inside the retained range each put
           * a kink in offset(z). Sampling only {-1, 0, +1, z_old} would
           * linearly interpolate across those kinks. The instantiated
           * mapping keeps exactly the in-range old breakpoints, and the new
           * mapping kinks at each one's output coordinate; add that z with
           * its old intermediate value so offset(z) is reproduced at every
           * kink. */
          hb_vector_t<avar2_offset_knot_t> with_breakpoints (knots);
          क्रम (स्थिर स्वतः &m : new_mapping)
          {
            द्विगुन from = (द्विगुन) m.coords[0].to_float ();
            अगर (from == -1.0 || from == 0.0 || from == 1.0)
              जारी; /* anchors already seeded */
            द्विगुन z = (द्विगुन) m.coords[1].to_float ();
            अगर (!(-1.0 < z && z < 1.0))
              जारी;
            द्विगुन user = avar2_denormalize_मूल्य (from,
                                                    (द्विगुन) new_user->minimum,
                                                    (द्विगुन) new_user->मध्य,
                                                    (द्विगुन) new_user->maximum);
            द्विगुन n_old = avar2_normalize_मूल्य (user, old_min, old_def, old_max);
            द्विगुन x_old = (द्विगुन) old_seg_maps[i]->map_float ((भग्न) n_old);
            x_old = (द्विगुन) roundf ((भग्न) (x_old * 16384.0)) / 16384.0;
            _avar2_add_knot (with_breakpoints, z, x_old - z);
          }

          अगर (unlikely (knots.in_error () || with_breakpoints.in_error ()))
            वापस false;

          /* Extra tents cost F2Dot14 rounding, so for a steep segment they
           * can add more quantization noise than the structural error they
           * remove. Keep the interior breakpoints only when they do not
           * increase the estimated residual; this makes the collection a
           * strict (never-worse) improvement over the {-1, 0, +1, z_old}
           * anchors. Warn when even the better choice is not bit-exact (a
           * steep retained segment that cannot be reproduced in F2Dot14). */
          अचिन्हित err = _avar2_estimate_offset_error (*old_seg_maps[i], new_mapping,
                                                       old_min, old_def, old_max,
                                                       (द्विगुन) new_user->minimum,
                                                       (द्विगुन) new_user->मध्य,
                                                       (द्विगुन) new_user->maximum,
                                                       knots);
          अगर (with_breakpoints.लंबाई != knots.लंबाई)
          {
            अचिन्हित err_with = _avar2_estimate_offset_error (*old_seg_maps[i], new_mapping,
                                                              old_min, old_def, old_max,
                                                              (द्विगुन) new_user->minimum,
                                                              (द्विगुन) new_user->मध्य,
                                                              (द्विगुन) new_user->maximum,
                                                              with_breakpoints);
            अगर (err_with <= err)
            {
              knots = std::move (with_breakpoints);
              err = err_with;
            }
          }
          अगर (err > 8)
            DEBUG_संदेश (SUBSET, nullptr,
                       "avar2 offset compensation is approximate for axis %c%c%c%c: "
                       "max residual %u F2Dot14 units",
                       HB_UNTAG (axis_tag), err);

          /* Synthesize tents. Adjacent tents evaluate to zero at each
           * other's peaks, so each knot's delta is offset(z) - offset(0);
           * the base value offset(0) = d_i is carried by the empty-region
           * bias above. This reduces to the classic pair of tents
           * (-1,-1,0) / (0,+1,+1) when the default is unchanged and there
           * are no interior knots. */
          क्रम (अचिन्हित k = 0; k < knots.लंबाई; k++)
          {
            द्विगुन z = knots.arrayZ[k].z;
            अगर (z == 0.0) जारी;
            पूर्णांक delta = (पूर्णांक) roundf ((भग्न) ((knots.arrayZ[k].offset - (द्विगुन) d_i) * 16384.0));
            अगर (!delta) जारी;
            द्विगुन lower, upper;
            अगर (z > 0.0)
            {
              lower = knots.arrayZ[k - 1].z;
              upper = k + 1 < knots.लंबाई ? knots.arrayZ[k + 1].z : z;
            }
            अन्यथा
            {
              upper = knots.arrayZ[k + 1].z;
              lower = k > 0 ? knots.arrayZ[k - 1].z : z;
            }
            hb_hashmap_t<hb_tag_t, Triple> region;
            अगर (unlikely (!region.set (axis_tag, Triple (lower, z, upper))))
              वापस false;
            item_vars.add_tuple (outer, std::move (region),
                                 inner, delta, item_गिनती_2);
          }
        }
      }
      अन्यथा
      {
        /* Free or private axis — not being restricted.
         * If it has a non-zero default delta, add it back as a bias.
         * Skip if this (outer,inner) was already processed (shared varIdx). */
        uint32_t varidx = new_varidx_mapping[i];
        अगर (varidx == HB_OT_LAYOUT_NO_VARIATIONS_सूचकांक)
          जारी;

        अगर (processed_varidxes.has (varidx))
          जारी;
        processed_varidxes.add (varidx);

        अचिन्हित outer = varidx >> 16;
        अचिन्हित inner = varidx & 0xFFFF;
        पूर्णांक dd = (पूर्णांक) roundf (default_deltas[i]);
        अगर (dd != 0)
        {
          अचिन्हित item_गिनती_2 = item_vars.get_item_गिनती (outer);
          hb_hashmap_t<hb_tag_t, Triple> empty_region;
          item_vars.add_tuple (outer, std::move (empty_region),
                               inner, dd, item_गिनती_2);
        }
      }
    }

    /* 7. Finalize: build region list + convert to varstore */
    अगर (!item_vars.build_region_list ()) वापस false;
    अगर (!item_vars.as_item_varstore (true /* optimize */,
                                     false /* use_no_variation_idx */))
      वापस false;

    /* 8. Apply varidx_map optimization remapping */
    स्थिर स्वतः &opt_varidx_map = item_vars.get_varidx_map ();
    क्रम (अचिन्हित i = 0; i < axis_गिनती_2; i++)
    {
      uint32_t varidx = new_varidx_mapping[i];
      अगर (varidx == HB_OT_LAYOUT_NO_VARIATIONS_सूचकांक)
        जारी;
      uint32_t *new_idx;
      अगर (opt_varidx_map.has (varidx, &new_idx))
        new_varidx_mapping[i] = *new_idx;
    }

    /* 9. Serialize avarV2Tail. Entries cover the retained axes only;
     * self-contained pinned axes are removed from fvar. */
    avar2_सूचकांक_map_plan_t सूचकांक_map_plan;
    अगर (!सूचकांक_map_plan.init (new_varidx_mapping,
			      c->plan->axes_सूचकांक_map,
			      axis_गिनती_2))
      वापस false;

    स्वतः *पुच्छ = c->serializer->allocate_आकार<avar_V2_पुच्छ> (avar_V2_पुच्छ::static_आकार);
    अगर (unlikely (!पुच्छ)) वापस false;
    अगर (!पुच्छ->varIdxMap.serialize_serialize (c->serializer, सूचकांक_map_plan))
      वापस false;
    अगर (!पुच्छ->varStore.serialize_serialize (c->serializer,
					     item_vars.has_long_word (),
					     c->plan->axis_tags,
					     item_vars.get_region_list (),
					     item_vars.get_vardata_encodings ()))
      वापस false;

    वापस true;
#पूर्ण_अगर
  }

  खुला:

  रक्षित:
  FixedVersion<>version;	/* Version of the avar table
				 * initially set to 0x00010000u */
  HBUINT16	reserved;	/* This field is permanently reserved. Set to 0. */
  HBUINT16	axis_गिनती_2;	/* The number of variation axes in the font. This
				 * must be the same number as axisCount in the
				 * 'fvar' table. */
  SegmentMaps	firstAxisSegmentMaps;

  खुला:
  DEFINE_आकार_MIN (8);
};

} /* namespace OT */


#पूर्ण_अगर /* HB_OT_VAR_AVAR_TABLE_HH */
