/*
 * Copyright © 2016  Elie Roux <elie.roux@telecom-bretagne.eu>
 * Copyright © 2018  Google, Inc.
 * Copyright © 2018-2019  Ebrahim Byagowi
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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_OT_LAYOUT_ਅਧਾਰ_ਸਾਰਣੀ_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_LAYOUT_ਅਧਾਰ_ਸਾਰਣੀ_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-open-type.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-layout-common.hh"

ਨਾਮਾਕਾਸ਼ OT {

/*
 * BASE -- Baseline
 * https://docs.microsoft.com/en-us/typography/opentype/spec/base
 */

ਬਣਤਰ ਅਧਾਰ_Coord_Format1
{
  hb_position_t get_coord (hb_font_t *font, hb_direction_t direction) ਅਚਲ
  {
    ਵਾਪਸ HB_DIRECTION_IS_HORIZONTAL (direction) ? font->em_scale_y (coordinate) : font->em_scale_x (coordinate);
  }

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    return_trace ((ਤਾਰਕਿਕ) c->serializer->embed (*ਇਹ));
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ));
  }

  ਸੁਰੱਖਿਅਤ:
  HBUINT16	format;		/* Format identifier--format = 1 */
  FWORD		coordinate;	/* X or Y value, in design units */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (4);
};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Types>
ਬਣਤਰ ਅਧਾਰ_Coord_Format2_4
{
  hb_position_t get_coord (hb_font_t *font, hb_direction_t direction) ਅਚਲ
  {
    /* TODO */
    ਵਾਪਸ HB_DIRECTION_IS_HORIZONTAL (direction) ? font->em_scale_y (coordinate) : font->em_scale_x (coordinate);
  }

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->serializer->embed (*ਇਹ);
    ਜੇ (unlikely (!ਬਾਹਰਲਾ)) return_trace (ਝੂਠ);

    hb_codepoint_t *new_gid;
    ਜੇ (!c->plan->glyph_map->has (referenceGlyph, &new_gid))
      return_trace (ਝੂਠ);

    return_trace (c->serializer->check_ਸੌਂਪੋ (ਬਾਹਰਲਾ->referenceGlyph,
                                               *new_gid,
                                               HB_SERIALIZE_ERROR_INT_ਉਛਾਲ));
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ));
  }

  ਸੁਰੱਖਿਅਤ:
  HBUINT16	format;		/* Format identifier--format = 2 or 4 */
  FWORD		coordinate;	/* X or Y value, in design units */
  ਕਿਸਮ_ਨਾਂ Types::HBGlyphID
		referenceGlyph;	/* Glyph ID of control glyph */
  HBUINT16	coord_ਬਿੰਦੂ;	/* Index of contour point on the
				 * reference glyph */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (6 + Types::HBGlyphID::static_ਮਾਪ);
};

ਬਣਤਰ ਅਧਾਰ_Coord_Format3
{
  hb_position_t get_coord (hb_font_t *font,
			   ਅਚਲ ਵਸਤੂ_Variation_Store &var_store,
			   hb_direction_t direction) ਅਚਲ
  {
    ਅਚਲ Device &device = ਇਹ+device_ਸਾਰਣੀ;

    ਵਾਪਸ HB_DIRECTION_IS_HORIZONTAL (direction)
	 ? hb_saturate_ਜੋੜੋ (font->em_scale_y (coordinate), device.get_y_delta (font, var_store))
	 : hb_saturate_ਜੋੜੋ (font->em_scale_x (coordinate), device.get_x_delta (font, var_store));
  }

  ਖਾਲੀ collect_variation_indices (hb_ਸੈੱਟ_t& varidx_ਸੈੱਟ /*ਬਾਹਰਲਾ*/) ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ varidx = (ਇਹ+device_ਸਾਰਣੀ).get_variation_ਸੂਚਕਾਂਕ ();
    varidx_ਸੈੱਟ.ਜੋੜੋ (varidx);
  }

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->serializer->embed (*ਇਹ);
    ਜੇ (unlikely (!ਬਾਹਰਲਾ)) return_trace (ਝੂਠ);

    ਜੇ (!c->plan->pinned_at_default)
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ var_idx = (ਇਹ+device_ਸਾਰਣੀ).get_variation_ਸੂਚਕਾਂਕ ();
      ਜੇ (var_idx != VarIdx::NO_VARIATION)
      {
        hb_pair_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਪੂਰਨ> *v;
        ਜੇ (!c->plan->ਅਧਾਰ_variation_idx_map.has (var_idx, &v))
          return_trace (ਝੂਠ);
        
        ਜੇ (unlikely (!c->serializer->check_ਸੌਂਪੋ (ਬਾਹਰਲਾ->coordinate, coordinate + hb_ਦੂਜਾ (*v),
                                                    HB_SERIALIZE_ERROR_INT_ਉਛਾਲ)))
          return_trace (ਝੂਠ);
      }
    }
    
    ਬਾਹਰਲਾ->device_ਸਾਰਣੀ.serialize_copy (c->serializer, device_ਸਾਰਣੀ,
                                     ਇਹ, 0,
                                     hb_serialize_context_t::ਸਿਰ,
                                     &c->plan->ਅਧਾਰ_variation_idx_map);
    return_trace (ਸੱਚ);
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (likely (c->check_struct (ਇਹ) &&
			  device_ਸਾਰਣੀ.sanitize (c, ਇਹ)));
  }

  ਸੁਰੱਖਿਅਤ:
  HBUINT16	format;		/* Format identifier--format = 3 */
  FWORD		coordinate;	/* X or Y value, in design units */
  Offset16_ਮੰਜ਼ਿਲ<Device>
		device_ਸਾਰਣੀ;	/* Offset to Device table for X or
				 * Y value, from beginning of
				 * BaseCoord table (may be NULL). */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (6);
};

ਬਣਤਰ ਅਧਾਰ_Coord
{
  ਤਾਰਕਿਕ has_ਅੰਕੜੇ () ਅਚਲ { ਵਾਪਸ u.format.v; }

  hb_position_t get_coord (hb_font_t            *font,
			   ਅਚਲ ਵਸਤੂ_Variation_Store &var_store,
			   hb_direction_t        direction) ਅਚਲ
  {
    ਚੋਣ (u.format.v) {
    ਹਾਲਤ 1: hb_barrier (); ਵਾਪਸ u.format1.get_coord (font, direction);
    ਹਾਲਤ 2: hb_barrier (); ਵਾਪਸ u.format2.get_coord (font, direction);
    ਹਾਲਤ 3: hb_barrier (); ਵਾਪਸ u.format3.get_coord (font, var_store, direction);
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
    ਹਾਲਤ 4: hb_barrier (); ਵਾਪਸ u.format4.get_coord (font, direction);
#ਜੇ_ਅੰਤ
    ਮੂਲ:ਵਾਪਸ 0;
    }
  }

  ਖਾਲੀ collect_variation_indices (hb_ਸੈੱਟ_t& varidx_ਸੈੱਟ /*ਬਾਹਰਲਾ*/) ਅਚਲ
  {
    ਚੋਣ (u.format.v) {
    ਹਾਲਤ 3: hb_barrier (); u.format3.collect_variation_indices (varidx_ਸੈੱਟ); ਵਾਪਸ;
    ਮੂਲ:ਵਾਪਸ;
    }
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ context_t, ਕਿਸਮ_ਨਾਂ ...Ts>
  ਕਿਸਮ_ਨਾਂ context_t::return_t dispatch (context_t *c, Ts&&... ds) ਅਚਲ
  {
    ਜੇ (unlikely (!c->may_dispatch (ਇਹ, &u.format.v))) ਵਾਪਸ c->no_dispatch_return_ਮੁੱਲ ();
    TRACE_DISPATCH (ਇਹ, u.format.v);
    ਚੋਣ (u.format.v) {
    ਹਾਲਤ 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    ਹਾਲਤ 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    ਹਾਲਤ 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
    ਹਾਲਤ 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
#ਜੇ_ਅੰਤ
    ਮੂਲ:return_trace (c->default_return_ਮੁੱਲ ());
    }
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    ਜੇ (unlikely (!u.format.v.sanitize (c))) return_trace (ਝੂਠ);
    hb_barrier ();
    ਚੋਣ (u.format.v) {
    ਹਾਲਤ 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    ਹਾਲਤ 2: hb_barrier (); return_trace (u.format2.sanitize (c));
    ਹਾਲਤ 3: hb_barrier (); return_trace (u.format3.sanitize (c));
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
    ਹਾਲਤ 4: hb_barrier (); return_trace (u.format4.sanitize (c));
#ਜੇ_ਅੰਤ
    ਮੂਲ:return_trace (ਝੂਠ);
    }
  }

  ਸੁਰੱਖਿਅਤ:
  ਸੰਘ {
  ਬਣਤਰ { HBUINT16 v; }	format;
  ਅਧਾਰ_Coord_Format1	format1;
  ਅਧਾਰ_Coord_Format2_4<Layout::SmallTypes>
			format2;
  ਅਧਾਰ_Coord_Format3	format3;
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
  ਅਧਾਰ_Coord_Format2_4<Layout::MediumTypes>
			format4;
#ਜੇ_ਅੰਤ
  } u;
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_UNION (2, format.v);
};

ਬਣਤਰ Feat_Min_Max_ਰਿਕਾਰਡ
{
  ਪੂਰਨ cmp (hb_ਟੈਗ_t key) ਅਚਲ { ਵਾਪਸ ਟੈਗ_2.cmp (key); }

  ਤਾਰਕਿਕ has_ਅੰਕੜੇ () ਅਚਲ { ਵਾਪਸ ਟੈਗ_2; }

  hb_ਟੈਗ_t get_feature_ਟੈਗ () ਅਚਲ { ਵਾਪਸ ਟੈਗ_2; }

  ਖਾਲੀ get_min_max (ਅਚਲ ਅਧਾਰ_Coord **min, ਅਚਲ ਅਧਾਰ_Coord **max) ਅਚਲ
  {
    ਜੇ (likely (min)) *min = &(ਇਹ+minCoord);
    ਜੇ (likely (max)) *max = &(ਇਹ+maxCoord);
  }

  ਖਾਲੀ collect_variation_indices (ਅਚਲ hb_subset_plan_t* plan,
                                  ਅਚਲ ਖਾਲੀ *ਅਧਾਰ,
                                  hb_ਸੈੱਟ_t& varidx_ਸੈੱਟ /*ਬਾਹਰਲਾ*/) ਅਚਲ
  {
    ਜੇ (!plan->layout_features.has (ਟੈਗ_2))
      ਵਾਪਸ;

    (ਅਧਾਰ+minCoord).collect_variation_indices (varidx_ਸੈੱਟ);
    (ਅਧਾਰ+maxCoord).collect_variation_indices (varidx_ਸੈੱਟ);
  }

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c,
               ਅਚਲ ਖਾਲੀ *ਅਧਾਰ) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->serializer->embed (*ਇਹ);
    ਜੇ (unlikely (!ਬਾਹਰਲਾ)) return_trace (ਝੂਠ);

    ਤਾਰਕਿਕ ret = ਬਾਹਰਲਾ->minCoord.serialize_subset (c, minCoord, ਅਧਾਰ);
    ret |= ਬਾਹਰਲਾ->maxCoord.serialize_subset (c, maxCoord, ਅਧਾਰ);
    return_trace (ret);
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c, ਅਚਲ ਖਾਲੀ *ਅਧਾਰ) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (likely (c->check_struct (ਇਹ) &&
			  minCoord.sanitize (c, ਅਧਾਰ) &&
			  maxCoord.sanitize (c, ਅਧਾਰ)));
  }

  ਸੁਰੱਖਿਅਤ:
  ਟੈਗ		ਟੈਗ_2;		/* 4-byte feature identification tag--must
				 * match feature tag in FeatureList */
  Offset16_ਮੰਜ਼ਿਲ<ਅਧਾਰ_Coord>
		minCoord;	/* Offset to BaseCoord table that defines
				 * the minimum extent value, from beginning
				 * of MinMax table (may be NULL) */
  Offset16_ਮੰਜ਼ਿਲ<ਅਧਾਰ_Coord>
		maxCoord;	/* Offset to BaseCoord table that defines
				 * the maximum extent value, from beginning
				 * of MinMax table (may be NULL) */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (8);
};

ਬਣਤਰ MinMax
{
  ਖਾਲੀ get_min_max (hb_ਟੈਗ_t          feature_ਟੈਗ,
		    ਅਚਲ ਅਧਾਰ_Coord **min,
		    ਅਚਲ ਅਧਾਰ_Coord **max) ਅਚਲ
  {
    ਅਚਲ Feat_Min_Max_ਰਿਕਾਰਡ &minMaxCoord = feat_Min_Max_ਰਿਕਾਰਡਾਂ.bsearch (feature_ਟੈਗ);
    ਜੇ (minMaxCoord.has_ਅੰਕੜੇ ())
      minMaxCoord.get_min_max (min, max);
    ਨਹੀਂਤਾਂ
    {
      ਜੇ (likely (min)) *min = &(ਇਹ+minCoord);
      ਜੇ (likely (max)) *max = &(ਇਹ+maxCoord);
    }
  }

  ਖਾਲੀ collect_variation_indices (ਅਚਲ hb_subset_plan_t* plan,
                                  hb_ਸੈੱਟ_t& varidx_ਸੈੱਟ /*ਬਾਹਰਲਾ*/) ਅਚਲ
  {
    (ਇਹ+minCoord).collect_variation_indices (varidx_ਸੈੱਟ);
    (ਇਹ+maxCoord).collect_variation_indices (varidx_ਸੈੱਟ);
    ਲਈ (ਅਚਲ Feat_Min_Max_ਰਿਕਾਰਡ& ਰਿਕਾਰਡ : feat_Min_Max_ਰਿਕਾਰਡਾਂ)
      ਰਿਕਾਰਡ.collect_variation_indices (plan, ਇਹ, varidx_ਸੈੱਟ);
  }

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->serializer->ਸ਼ੁਰੂ_embed (*ਇਹ);
    ਜੇ (unlikely (!ਬਾਹਰਲਾ || !c->serializer->extend_min (ਬਾਹਰਲਾ))) return_trace (ਝੂਠ);

    ਤਾਰਕਿਕ ret = ਬਾਹਰਲਾ->minCoord.serialize_subset (c, minCoord, ਇਹ);
    ret |= ਬਾਹਰਲਾ->maxCoord.serialize_subset (c, maxCoord, ਇਹ);

    ਬਿਨਾਨਿਸ਼ਾਨੀ len = 0;
    ਲਈ (ਅਚਲ Feat_Min_Max_ਰਿਕਾਰਡ& _ : feat_Min_Max_ਰਿਕਾਰਡਾਂ)
    {
      hb_ਟੈਗ_t feature_ਟੈਗ = _.get_feature_ਟੈਗ ();
      ਜੇ (!c->plan->layout_features.has (feature_ਟੈਗ))
        ਜਾਰੀ;

      ਸਵੈਚਾਲਿਤ snap = c->serializer->snapshot ();
      ਜੇ (!_.subset (c, ਇਹ)) c->serializer->revert (snap);
      ਨਹੀਂਤਾਂ len++;
    }

    ਜੇ (len > 0) ret = ਸੱਚ;
    ਜੇ (!ret) return_trace (ਝੂਠ);
    return_trace (c->serializer->check_ਸੌਂਪੋ (ਬਾਹਰਲਾ->feat_Min_Max_ਰਿਕਾਰਡਾਂ.len, len,
                                               HB_SERIALIZE_ERROR_INT_ਉਛਾਲ));
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (likely (c->check_struct (ਇਹ) &&
			  minCoord.sanitize (c, ਇਹ) &&
			  maxCoord.sanitize (c, ਇਹ) &&
			  feat_Min_Max_ਰਿਕਾਰਡਾਂ.sanitize (c, ਇਹ)));
  }

  ਸੁਰੱਖਿਅਤ:
  Offset16_ਮੰਜ਼ਿਲ<ਅਧਾਰ_Coord>
		minCoord;	/* Offset to BaseCoord table that defines
				 * minimum extent value, from the beginning
				 * of MinMax table (may be NULL) */
  Offset16_ਮੰਜ਼ਿਲ<ਅਧਾਰ_Coord>
		maxCoord;	/* Offset to BaseCoord table that defines
				 * maximum extent value, from the beginning
				 * of MinMax table (may be NULL) */
  Sorted_Array16_ਦਾ<Feat_Min_Max_ਰਿਕਾਰਡ>
		feat_Min_Max_ਰਿਕਾਰਡਾਂ;
				/* Array of FeatMinMaxRecords, in alphabetical
				 * order by featureTableTag */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_ARRAY (6, feat_Min_Max_ਰਿਕਾਰਡਾਂ);
};

ਬਣਤਰ ਅਧਾਰ_ਮੁੱਲਾਂ
{
  ਅਚਲ ਅਧਾਰ_Coord &get_ਅਧਾਰ_coord (ਪੂਰਨ baseline_ਟੈਗ_ਸੂਚਕਾਂਕ) ਅਚਲ
  {
    ਜੇ (baseline_ਟੈਗ_ਸੂਚਕਾਂਕ == -1) baseline_ਟੈਗ_ਸੂਚਕਾਂਕ = default_ਸੂਚਕਾਂਕ_2;
    ਵਾਪਸ ਇਹ+ਅਧਾਰ_Coords[baseline_ਟੈਗ_ਸੂਚਕਾਂਕ];
  }

  ਖਾਲੀ collect_variation_indices (hb_ਸੈੱਟ_t& varidx_ਸੈੱਟ /*ਬਾਹਰਲਾ*/) ਅਚਲ
  {
    ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ& _ : ਅਧਾਰ_Coords)
      (ਇਹ+_).collect_variation_indices (varidx_ਸੈੱਟ);
  }

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->serializer->ਸ਼ੁਰੂ_embed (*ਇਹ);
    ਜੇ (unlikely (!ਬਾਹਰਲਾ || !c->serializer->extend_min (ਬਾਹਰਲਾ))) return_trace (ਝੂਠ);
    ਬਾਹਰਲਾ->default_ਸੂਚਕਾਂਕ_2 = default_ਸੂਚਕਾਂਕ_2;

    ਤਾਰਕਿਕ ret = ਝੂਠ;
    ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ& _ : ਅਧਾਰ_Coords)
    {
      ਸਵੈਚਾਲਿਤ *o = ਬਾਹਰਲਾ->ਅਧਾਰ_Coords.serialize_append (c->serializer);
      ਜੇ (unlikely (!o)) return_trace (ਝੂਠ);
       ret |= o->serialize_subset (c, _, ਇਹ);
    }

    return_trace (ret);
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (likely (c->check_struct (ਇਹ) &&
			  ਅਧਾਰ_Coords.sanitize (c, ਇਹ)));
  }

  ਸੁਰੱਖਿਅਤ:
  ਸੂਚਕਾਂਕ		default_ਸੂਚਕਾਂਕ_2;	/* Index number of default baseline for this
				 * script — equals index position of baseline tag
				 * in baselineTags array of the BaseTagList */
  Array16_ਦਾ_Offset16_ਮੰਜ਼ਿਲ<ਅਧਾਰ_Coord>
		ਅਧਾਰ_Coords;	/* Number of BaseCoord tables defined — should equal
				 * baseTagCount in the BaseTagList
				 *
				 * Array of offsets to BaseCoord tables, from beginning of
				 * BaseValues table — order matches baselineTags array in
				 * the BaseTagList */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_ARRAY (4, ਅਧਾਰ_Coords);
};

ਬਣਤਰ ਅਧਾਰ_Lang_Sys_ਰਿਕਾਰਡ
{
  ਪੂਰਨ cmp (hb_ਟੈਗ_t key) ਅਚਲ { ਵਾਪਸ ਅਧਾਰ_Lang_Sys_ਟੈਗ.cmp (key); }

  ਤਾਰਕਿਕ has_ਅੰਕੜੇ () ਅਚਲ { ਵਾਪਸ ਅਧਾਰ_Lang_Sys_ਟੈਗ; }

  ਅਚਲ MinMax &get_min_max (ਅਚਲ ਖਾਲੀ* ਅਧਾਰ) ਅਚਲ { ਵਾਪਸ ਅਧਾਰ+minMax; }

  ਖਾਲੀ collect_variation_indices (ਅਚਲ ਖਾਲੀ* ਅਧਾਰ,
                                  ਅਚਲ hb_subset_plan_t* plan,
                                  hb_ਸੈੱਟ_t& varidx_ਸੈੱਟ /*ਬਾਹਰਲਾ*/) ਅਚਲ
  { (ਅਧਾਰ+minMax).collect_variation_indices (plan, varidx_ਸੈੱਟ); }

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c,
               ਅਚਲ ਖਾਲੀ *ਅਧਾਰ) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->serializer->embed (*ਇਹ);
    ਜੇ (unlikely (!ਬਾਹਰਲਾ)) return_trace (ਝੂਠ);

    return_trace (ਬਾਹਰਲਾ->minMax.serialize_subset (c, minMax, ਅਧਾਰ));
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c, ਅਚਲ ਖਾਲੀ *ਅਧਾਰ) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (likely (c->check_struct (ਇਹ) &&
			  minMax.sanitize (c, ਅਧਾਰ)));
  }

  ਸੁਰੱਖਿਅਤ:
  ਟੈਗ		ਅਧਾਰ_Lang_Sys_ਟੈਗ;	/* 4-byte language system identification tag */
  Offset16_ਮੰਜ਼ਿਲ<MinMax>
		minMax;		/* Offset to MinMax table, from beginning
				 * of BaseScript table */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (6);
};

ਬਣਤਰ ਅਧਾਰ_Script
{
  ਅਚਲ MinMax &get_min_max (hb_ਟੈਗ_t language_ਟੈਗ) ਅਚਲ
  {
    ਅਚਲ ਅਧਾਰ_Lang_Sys_ਰਿਕਾਰਡ& ਰਿਕਾਰਡ = ਅਧਾਰ_Lang_Sys_ਰਿਕਾਰਡਾਂ.bsearch (language_ਟੈਗ);
    ਵਾਪਸ ਰਿਕਾਰਡ.has_ਅੰਕੜੇ () ? ਰਿਕਾਰਡ.get_min_max (ਇਹ) : ਇਹ+defaultMinMax;
  }

  ਅਚਲ ਅਧਾਰ_Coord &get_ਅਧਾਰ_coord (ਪੂਰਨ baseline_ਟੈਗ_ਸੂਚਕਾਂਕ) ਅਚਲ
  { ਵਾਪਸ (ਇਹ+ਅਧਾਰ_ਮੁੱਲਾਂ_2).get_ਅਧਾਰ_coord (baseline_ਟੈਗ_ਸੂਚਕਾਂਕ); }

  ਤਾਰਕਿਕ has_ਮੁੱਲਾਂ () ਅਚਲ { ਵਾਪਸ ਅਧਾਰ_ਮੁੱਲਾਂ_2; }
  ਤਾਰਕਿਕ has_min_max () ਅਚਲ { ਵਾਪਸ defaultMinMax || ਅਧਾਰ_Lang_Sys_ਰਿਕਾਰਡਾਂ; }

  ਖਾਲੀ collect_variation_indices (ਅਚਲ hb_subset_plan_t* plan,
                                  hb_ਸੈੱਟ_t& varidx_ਸੈੱਟ /*ਬਾਹਰਲਾ*/) ਅਚਲ
  {
    (ਇਹ+ਅਧਾਰ_ਮੁੱਲਾਂ_2).collect_variation_indices (varidx_ਸੈੱਟ);
    (ਇਹ+defaultMinMax).collect_variation_indices (plan, varidx_ਸੈੱਟ);
    
    ਲਈ (ਅਚਲ ਅਧਾਰ_Lang_Sys_ਰਿਕਾਰਡ& _ : ਅਧਾਰ_Lang_Sys_ਰਿਕਾਰਡਾਂ)
      _.collect_variation_indices (ਇਹ, plan, varidx_ਸੈੱਟ);
  }

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->serializer->ਸ਼ੁਰੂ_embed (*ਇਹ);
    ਜੇ (unlikely (!ਬਾਹਰਲਾ || !c->serializer->extend_min (ਬਾਹਰਲਾ))) return_trace (ਝੂਠ);

    ਤਾਰਕਿਕ ret = ਬਾਹਰਲਾ->ਅਧਾਰ_ਮੁੱਲਾਂ_2.serialize_subset (c, ਅਧਾਰ_ਮੁੱਲਾਂ_2, ਇਹ);
    ret |= ਬਾਹਰਲਾ->defaultMinMax.serialize_subset (c, defaultMinMax, ਇਹ);

    ਬਿਨਾਨਿਸ਼ਾਨੀ len = 0;
    ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ& _ : ਅਧਾਰ_Lang_Sys_ਰਿਕਾਰਡਾਂ)
    {
      ਸਵੈਚਾਲਿਤ snap = c->serializer->snapshot ();
      ਜੇ (_.subset (c, ਇਹ)) {
        ret = ਸੱਚ;
        len++;
      }
      ਨਹੀਂਤਾਂ c->serializer->revert (snap);
    }

    ਜੇ (!ret) return_trace (ਝੂਠ);
    return_trace (c->serializer->check_ਸੌਂਪੋ (ਬਾਹਰਲਾ->ਅਧਾਰ_Lang_Sys_ਰਿਕਾਰਡਾਂ.len, len,
                                               HB_SERIALIZE_ERROR_INT_ਉਛਾਲ));
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (likely (c->check_struct (ਇਹ) &&
			  ਅਧਾਰ_ਮੁੱਲਾਂ_2.sanitize (c, ਇਹ) &&
			  defaultMinMax.sanitize (c, ਇਹ) &&
			  ਅਧਾਰ_Lang_Sys_ਰਿਕਾਰਡਾਂ.sanitize (c, ਇਹ)));
  }

  ਸੁਰੱਖਿਅਤ:
  Offset16_ਮੰਜ਼ਿਲ<ਅਧਾਰ_ਮੁੱਲਾਂ>
		ਅਧਾਰ_ਮੁੱਲਾਂ_2;	/* Offset to BaseValues table, from beginning
				 * of BaseScript table (may be NULL) */
  Offset16_ਮੰਜ਼ਿਲ<MinMax>
		defaultMinMax;	/* Offset to MinMax table, from beginning of
				 * BaseScript table (may be NULL) */
  Sorted_Array16_ਦਾ<ਅਧਾਰ_Lang_Sys_ਰਿਕਾਰਡ>
		ਅਧਾਰ_Lang_Sys_ਰਿਕਾਰਡਾਂ;
				/* Number of BaseLangSysRecords
				 * defined — may be zero (0) */

  ਜਨਤਕ:
  DEFINE_ਮਾਪ_ARRAY (6, ਅਧਾਰ_Lang_Sys_ਰਿਕਾਰਡਾਂ);
};

ਬਣਤਰ ਅਧਾਰ_Script_List;
ਬਣਤਰ ਅਧਾਰ_Script_ਰਿਕਾਰਡ
{
  ਪੂਰਨ cmp (hb_ਟੈਗ_t key) ਅਚਲ { ਵਾਪਸ ਅਧਾਰ_Script_ਟੈਗ.cmp (key); }

  ਤਾਰਕਿਕ has_ਅੰਕੜੇ () ਅਚਲ { ਵਾਪਸ ਅਧਾਰ_Script_ਟੈਗ; }

  hb_ਟੈਗ_t get_script_ਟੈਗ () ਅਚਲ { ਵਾਪਸ ਅਧਾਰ_Script_ਟੈਗ; }

  ਅਚਲ ਅਧਾਰ_Script &get_ਅਧਾਰ_script (ਅਚਲ ਅਧਾਰ_Script_List *list) ਅਚਲ
  { ਵਾਪਸ list+ਅਧਾਰ_Script_2; }

  ਖਾਲੀ collect_variation_indices (ਅਚਲ hb_subset_plan_t* plan,
                                  ਅਚਲ ਖਾਲੀ* list,
                                  hb_ਸੈੱਟ_t& varidx_ਸੈੱਟ /*ਬਾਹਰਲਾ*/) ਅਚਲ
  {
    ਜੇ (!plan->layout_scripts.has (ਅਧਾਰ_Script_ਟੈਗ))
      ਵਾਪਸ;

    (list+ਅਧਾਰ_Script_2).collect_variation_indices (plan, varidx_ਸੈੱਟ);
  }

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c,
               ਅਚਲ ਖਾਲੀ *ਅਧਾਰ) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->serializer->embed (*ਇਹ);
    ਜੇ (unlikely (!ਬਾਹਰਲਾ)) return_trace (ਝੂਠ);

    return_trace (ਬਾਹਰਲਾ->ਅਧਾਰ_Script_2.serialize_subset (c, ਅਧਾਰ_Script_2, ਅਧਾਰ));
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c, ਅਚਲ ਖਾਲੀ *ਅਧਾਰ) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (likely (c->check_struct (ਇਹ) &&
			  ਅਧਾਰ_Script_2.sanitize (c, ਅਧਾਰ)));
  }

  ਸੁਰੱਖਿਅਤ:
  ਟੈਗ		ਅਧਾਰ_Script_ਟੈਗ;	/* 4-byte script identification tag */
  Offset16_ਮੰਜ਼ਿਲ<ਅਧਾਰ_Script>
		ਅਧਾਰ_Script_2;	/* Offset to BaseScript table, from beginning
				 * of BaseScriptList */

  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (6);
};

ਬਣਤਰ ਅਧਾਰ_Script_List
{
  ਅਚਲ ਅਧਾਰ_Script &get_ਅਧਾਰ_script (hb_ਟੈਗ_t script) ਅਚਲ
  {
    ਅਚਲ ਅਧਾਰ_Script_ਰਿਕਾਰਡ *ਰਿਕਾਰਡ = &ਅਧਾਰ_Script_ਰਿਕਾਰਡਾਂ.bsearch (script);
    ਜੇ (!ਰਿਕਾਰਡ->has_ਅੰਕੜੇ ()) ਰਿਕਾਰਡ = &ਅਧਾਰ_Script_ਰਿਕਾਰਡਾਂ.bsearch (HB_ਟੈਗ ('D','F','L','T'));
    ਵਾਪਸ ਰਿਕਾਰਡ->has_ਅੰਕੜੇ () ? ਰਿਕਾਰਡ->get_ਅਧਾਰ_script (ਇਹ) : Null (ਅਧਾਰ_Script);
  }

  ਖਾਲੀ collect_variation_indices (ਅਚਲ hb_subset_plan_t* plan,
                                  hb_ਸੈੱਟ_t& varidx_ਸੈੱਟ /*ਬਾਹਰਲਾ*/) ਅਚਲ
  {
    ਲਈ (ਅਚਲ ਅਧਾਰ_Script_ਰਿਕਾਰਡ& _ : ਅਧਾਰ_Script_ਰਿਕਾਰਡਾਂ)
      _.collect_variation_indices (plan, ਇਹ, varidx_ਸੈੱਟ);
  }

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->serializer->ਸ਼ੁਰੂ_embed (*ਇਹ);
    ਜੇ (unlikely (!ਬਾਹਰਲਾ || !c->serializer->extend_min (ਬਾਹਰਲਾ))) return_trace (ਝੂਠ);

    ਬਿਨਾਨਿਸ਼ਾਨੀ len = 0;
    ਲਈ (ਅਚਲ ਅਧਾਰ_Script_ਰਿਕਾਰਡ& _ : ਅਧਾਰ_Script_ਰਿਕਾਰਡਾਂ)
    {
      hb_ਟੈਗ_t script_ਟੈਗ = _.get_script_ਟੈਗ ();
      ਜੇ (!c->plan->layout_scripts.has (script_ਟੈਗ))
        ਜਾਰੀ;

      ਸਵੈਚਾਲਿਤ snap = c->serializer->snapshot ();
      ਜੇ (!_.subset (c, ਇਹ)) c->serializer->revert (snap);
      ਨਹੀਂਤਾਂ len++;
    }

    ਜੇ (!len) return_trace (ਝੂਠ);
    return_trace (c->serializer->check_ਸੌਂਪੋ (ਬਾਹਰਲਾ->ਅਧਾਰ_Script_ਰਿਕਾਰਡਾਂ.len, len,
                                               HB_SERIALIZE_ERROR_INT_ਉਛਾਲ));
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ) &&
		  ਅਧਾਰ_Script_ਰਿਕਾਰਡਾਂ.sanitize (c, ਇਹ));
  }

  ਸੁਰੱਖਿਅਤ:
  Sorted_Array16_ਦਾ<ਅਧਾਰ_Script_ਰਿਕਾਰਡ>
			ਅਧਾਰ_Script_ਰਿਕਾਰਡਾਂ;

  ਜਨਤਕ:
  DEFINE_ਮਾਪ_ARRAY (2, ਅਧਾਰ_Script_ਰਿਕਾਰਡਾਂ);
};

ਬਣਤਰ Axis
{
  ਤਾਰਕਿਕ get_baseline (hb_ਟੈਗ_t          baseline_ਟੈਗ,
		     hb_ਟੈਗ_t          script_ਟੈਗ,
		     hb_ਟੈਗ_t          language_ਟੈਗ,
		     ਅਚਲ ਅਧਾਰ_Coord **coord) ਅਚਲ
  {
    ਅਚਲ ਅਧਾਰ_Script &ਅਧਾਰ_script = (ਇਹ+ਅਧਾਰ_Script_List_2).get_ਅਧਾਰ_script (script_ਟੈਗ);
    ਜੇ (!ਅਧਾਰ_script.has_ਮੁੱਲਾਂ ())
    {
      *coord = ਖਾਲੀ_ਸੂਚਕ;
      ਵਾਪਸ ਝੂਠ;
    }

    ਜੇ (likely (coord))
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਟੈਗ_ਸੂਚਕਾਂਕ = 0;
      ਜੇ (!(ਇਹ+ਅਧਾਰ_ਟੈਗ_List).bfind (baseline_ਟੈਗ, &ਟੈਗ_ਸੂਚਕਾਂਕ))
      {
        *coord = ਖਾਲੀ_ਸੂਚਕ;
        ਵਾਪਸ ਝੂਠ;
      }
      *coord = &ਅਧਾਰ_script.get_ਅਧਾਰ_coord (ਟੈਗ_ਸੂਚਕਾਂਕ);
    }

    ਵਾਪਸ ਸੱਚ;
  }

  ਤਾਰਕਿਕ get_min_max (hb_ਟੈਗ_t          script_ਟੈਗ,
		    hb_ਟੈਗ_t          language_ਟੈਗ,
		    hb_ਟੈਗ_t          feature_ਟੈਗ,
		    ਅਚਲ ਅਧਾਰ_Coord **min_coord,
		    ਅਚਲ ਅਧਾਰ_Coord **max_coord) ਅਚਲ
  {
    ਅਚਲ ਅਧਾਰ_Script &ਅਧਾਰ_script = (ਇਹ+ਅਧਾਰ_Script_List_2).get_ਅਧਾਰ_script (script_ਟੈਗ);
    ਜੇ (!ਅਧਾਰ_script.has_min_max ())
    {
      *min_coord = *max_coord = ਖਾਲੀ_ਸੂਚਕ;
      ਵਾਪਸ ਝੂਠ;
    }

    ਅਧਾਰ_script.get_min_max (language_ਟੈਗ).get_min_max (feature_ਟੈਗ, min_coord, max_coord);

    ਵਾਪਸ ਸੱਚ;
  }

  ਖਾਲੀ collect_variation_indices (ਅਚਲ hb_subset_plan_t* plan,
                                  hb_ਸੈੱਟ_t& varidx_ਸੈੱਟ /*ਬਾਹਰਲਾ*/) ਅਚਲ
  { (ਇਹ+ਅਧਾਰ_Script_List_2).collect_variation_indices (plan, varidx_ਸੈੱਟ); }

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->serializer->embed (*ਇਹ);
    ਜੇ (unlikely (!ਬਾਹਰਲਾ)) return_trace (ਝੂਠ);

    ਤਾਰਕਿਕ ret = ਬਾਹਰਲਾ->ਅਧਾਰ_ਟੈਗ_List.serialize_copy (c->serializer, ਅਧਾਰ_ਟੈਗ_List, ਇਹ);
    ret |= ਬਾਹਰਲਾ->ਅਧਾਰ_Script_List_2.serialize_subset (c, ਅਧਾਰ_Script_List_2, ਇਹ);
    return_trace (ret);
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (likely (c->check_struct (ਇਹ) &&
			  ਅਧਾਰ_ਟੈਗ_List.sanitize (c, ਇਹ) &&
			  ਅਧਾਰ_Script_List_2.sanitize (c, ਇਹ)));
  }

  ਸੁਰੱਖਿਅਤ:
  Offset16_ਮੰਜ਼ਿਲ<Sorted_Array16_ਦਾ<ਟੈਗ>>
		ਅਧਾਰ_ਟੈਗ_List;	/* Offset to BaseTagList table, from beginning
				 * of Axis table (may be NULL)
				 * Array of 4-byte baseline identification tags — must
				 * be in alphabetical order */
  Offset16_ਮੰਜ਼ਿਲ<ਅਧਾਰ_Script_List>
		ਅਧਾਰ_Script_List_2;	/* Offset to BaseScriptList table, from beginning
				 * of Axis table
				 * Array of BaseScriptRecords, in alphabetical order
				 * by baseScriptTag */

  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (4);
};

ਬਣਤਰ ਅਧਾਰ_4
{
  ਸਥਿਰ constexpr hb_ਟੈਗ_t ਸਾਰਣੀ_ਟੈਗ = HB_OT_ਟੈਗ_ਅਧਾਰ;

  ਅਚਲ Axis &get_axis (hb_direction_t direction) ਅਚਲ
  { ਵਾਪਸ HB_DIRECTION_IS_VERTICAL (direction) ? ਇਹ+vAxis : ਇਹ+hAxis; }

  ਤਾਰਕਿਕ has_var_store () ਅਚਲ
  { ਵਾਪਸ ਸੰਸਕਰਣ.ਮੰਜ਼ਿਲ_int () >= 0x00010001u && varStore != 0; }

  ਅਚਲ ਵਸਤੂ_Variation_Store &get_var_store () ਅਚਲ
  { ਵਾਪਸ ਸੰਸਕਰਣ.ਮੰਜ਼ਿਲ_int () < 0x00010001u ? Null (ਵਸਤੂ_Variation_Store) : ਇਹ+varStore; }

  ਖਾਲੀ collect_variation_indices (ਅਚਲ hb_subset_plan_t* plan,
                                  hb_ਸੈੱਟ_t& varidx_ਸੈੱਟ /*ਬਾਹਰਲਾ*/) ਅਚਲ
  {
    (ਇਹ+hAxis).collect_variation_indices (plan, varidx_ਸੈੱਟ);
    (ਇਹ+vAxis).collect_variation_indices (plan, varidx_ਸੈੱਟ);
  }

  ਤਾਰਕਿਕ subset_varstore (hb_subset_context_t *c,
                        ਅਧਾਰ_4 *ਬਾਹਰਲਾ /*ਬਾਹਰਲਾ*/) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਜੇ (!c->serializer->allocate_ਮਾਪ<Offset32_ਮੰਜ਼ਿਲ<ਵਸਤੂ_Variation_Store>> (Offset32_ਮੰਜ਼ਿਲ<ਵਸਤੂ_Variation_Store>::static_ਮਾਪ))
        return_trace (ਝੂਠ);
    ਜੇ (!c->plan->normalized_coords)
      return_trace (ਬਾਹਰਲਾ->varStore.serialize_subset (c, varStore, ਇਹ, c->plan->ਅਧਾਰ_varstore_inner_maps.as_array ()));

    ਜੇ (c->plan->all_axes_pinned)
      return_trace (ਸੱਚ);

    ਵਸਤੂ_variations_t ਵਸਤੂ_vars;
    ਜੇ (!ਵਸਤੂ_vars.instantiate (ਇਹ+varStore, c->plan, ਸੱਚ, ਸੱਚ,
                                c->plan->ਅਧਾਰ_varstore_inner_maps.as_array ()))
      return_trace (ਝੂਠ);

    ਜੇ (!ਬਾਹਰਲਾ->varStore.serialize_serialize (c->serializer,
                                            ਵਸਤੂ_vars.has_long_ਸ਼ਬਦ (),
                                            c->plan->axis_ਟੈਗ_ਸੂਚੀ,
                                            ਵਸਤੂ_vars.get_region_list (),
                                            ਵਸਤੂ_vars.get_vardata_encodings ()))
      return_trace (ਝੂਠ);

    ਅਚਲ hb_map_t &varidx_map = ਵਸਤੂ_vars.get_varidx_map ();
    /* base_variation_idx_map in the plan is old_varidx->(varidx, delta)
     * mapping, new varidx is generated for subsetting, we need to remap this
     * after instancing */
    ਲਈ (ਸਵੈਚਾਲਿਤ _ : c->plan->ਅਧਾਰ_variation_idx_map.iter_ref ())
    {
      uint32_t varidx = _.second.ਪਹਿਲਾ;
      uint32_t *new_varidx;
      ਜੇ (varidx_map.has (varidx, &new_varidx))
        _.second.ਪਹਿਲਾ = *new_varidx;
      ਨਹੀਂਤਾਂ
        _.second.ਪਹਿਲਾ = HB_OT_LAYOUT_NO_VARIATIONS_ਸੂਚਕਾਂਕ;
    }
    return_trace (ਸੱਚ);
  }

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->serializer->ਸ਼ੁਰੂ_embed (*ਇਹ);
    ਜੇ (unlikely (!ਬਾਹਰਲਾ || !c->serializer->extend_min (ਬਾਹਰਲਾ))) return_trace (ਝੂਠ);

    ਬਾਹਰਲਾ->ਸੰਸਕਰਣ = ਸੰਸਕਰਣ;
    ਤਾਰਕਿਕ ret = ਝੂਠ;
    ਜੇ (ਸੰਸਕਰਣ.ਮੰਜ਼ਿਲ_int () >= 0x00010001u) ret |= subset_varstore (c, ਬਾਹਰਲਾ);

    ret |= ਬਾਹਰਲਾ->hAxis.serialize_subset (c, hAxis, ਇਹ);
    ret |= ਬਾਹਰਲਾ->vAxis.serialize_subset (c, vAxis, ਇਹ);

    return_trace (ret);
  }

  ਤਾਰਕਿਕ get_baseline (hb_font_t      *font,
		     hb_ਟੈਗ_t        baseline_ਟੈਗ,
		     hb_direction_t  direction,
		     hb_ਟੈਗ_t        script_ਟੈਗ,
		     hb_ਟੈਗ_t        language_ਟੈਗ,
		     hb_position_t  *ਅਧਾਰ) ਅਚਲ
  {
    ਅਚਲ ਅਧਾਰ_Coord *ਅਧਾਰ_coord = ਖਾਲੀ_ਸੂਚਕ;
    ਜੇ (unlikely (!get_axis (direction).get_baseline (baseline_ਟੈਗ, script_ਟੈਗ, language_ਟੈਗ, &ਅਧਾਰ_coord) ||
		  !ਅਧਾਰ_coord || !ਅਧਾਰ_coord->has_ਅੰਕੜੇ ()))
      ਵਾਪਸ ਝੂਠ;

    ਜੇ (likely (ਅਧਾਰ))
      *ਅਧਾਰ = ਅਧਾਰ_coord->get_coord (font, get_var_store (), direction);

    ਵਾਪਸ ਸੱਚ;
  }

  ਤਾਰਕਿਕ get_min_max (hb_font_t      *font,
		    hb_direction_t  direction,
		    hb_ਟੈਗ_t        script_ਟੈਗ,
		    hb_ਟੈਗ_t        language_ਟੈਗ,
		    hb_ਟੈਗ_t        feature_ਟੈਗ,
		    hb_position_t  *min,
		    hb_position_t  *max) ਅਚਲ
  {
    ਅਚਲ ਅਧਾਰ_Coord *min_coord, *max_coord;
    ਜੇ (!get_axis (direction).get_min_max (script_ਟੈਗ, language_ਟੈਗ, feature_ਟੈਗ,
					   &min_coord, &max_coord))
      ਵਾਪਸ ਝੂਠ;

    ਅਚਲ ਵਸਤੂ_Variation_Store &var_store = get_var_store ();
    ਜੇ (likely (min && min_coord)) *min = min_coord->get_coord (font, var_store, direction);
    ਜੇ (likely (max && max_coord)) *max = max_coord->get_coord (font, var_store, direction);
    ਵਾਪਸ ਸੱਚ;
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (likely (c->check_struct (ਇਹ) &&
			  hb_barrier () &&
			  likely (ਸੰਸਕਰਣ.major == 1) &&
			  hAxis.sanitize (c, ਇਹ) &&
			  vAxis.sanitize (c, ਇਹ) &&
			  (ਸੰਸਕਰਣ.ਮੰਜ਼ਿਲ_int () < 0x00010001u || varStore.sanitize (c, ਇਹ))));
  }

  ਸੁਰੱਖਿਅਤ:
  Fixed_ਸੰਸਕਰਣ<>ਸੰਸਕਰਣ;	/* Version of the BASE table */
  Offset16_ਮੰਜ਼ਿਲ<Axis>hAxis;		/* Offset to horizontal Axis table, from beginning
				 * of BASE table (may be NULL) */
  Offset16_ਮੰਜ਼ਿਲ<Axis>vAxis;		/* Offset to vertical Axis table, from beginning
				 * of BASE table (may be NULL) */
  Offset32_ਮੰਜ਼ਿਲ<ਵਸਤੂ_Variation_Store>
		varStore;	/* Offset to the table of Item Variation
				 * Store--from beginning of BASE
				 * header (may be NULL).  Introduced
				 * in version 0x00010001. */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_MIN (8);
};


} /* namespace OT */


#ਜੇ_ਅੰਤ /* HB_OT_LAYOUT_BASE_TABLE_HH */
