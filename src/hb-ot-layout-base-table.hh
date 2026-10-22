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

#ifndef HB_OT_LAYOUT_BASE_TABLE_HH
#define HB_OT_LAYOUT_BASE_TABLE_HH

#ಅನ್ತರ್ಭಾವ "hb-open-type.hh"
#ಅನ್ತರ್ಭಾವ "hb-ot-layout-common.hh"

ನಾಮಾಕಾಶ OT {

/*
 * BASE -- Baseline
 * https://docs.microsoft.com/en-us/typography/opentype/spec/base
 */

ಸಂರಚನಾ BaseCoordFormat1
{
  hb_position_t get_coord (hb_font_t *font, hb_direction_t direction) ಧ್ರುವ
  {
    ಪ್ರತ್ಯಾಗಮ HB_DIRECTION_IS_HORIZONTAL (direction) ? font->em_scale_y (coordinate) : font->em_scale_x (coordinate);
  }

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    return_trace ((ತಾರ್ಕಿಕ) c->serializer->embed (*ಏತತ್));
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್));
  }

  ರಕ್ಷಿತ:
  HBUINT16	format;		/* Format identifier--format = 1 */
  FWORD		coordinate;	/* X or Y value, in design units */
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (4);
};

template <typename Types>
ಸಂರಚನಾ BaseCoordFormat2_4
{
  hb_position_t get_coord (hb_font_t *font, hb_direction_t direction) ಧ್ರುವ
  {
    /* TODO */
    ಪ್ರತ್ಯಾಗಮ HB_DIRECTION_IS_HORIZONTAL (direction) ? font->em_scale_y (coordinate) : font->em_scale_x (coordinate);
  }

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    auto *out = c->serializer->embed (*ಏತತ್);
    ಯದಿ (unlikely (!out)) return_trace (ಅಸತ್ಯ);

    hb_codepoint_t *new_gid;
    ಯದಿ (!c->plan->glyph_map->has (referenceGlyph, &new_gid))
      return_trace (ಅಸತ್ಯ);

    return_trace (c->serializer->check_assign (out->referenceGlyph,
                                               *new_gid,
                                               HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್));
  }

  ರಕ್ಷಿತ:
  HBUINT16	format;		/* Format identifier--format = 2 or 4 */
  FWORD		coordinate;	/* X or Y value, in design units */
  typename Types::HBGlyphID
		referenceGlyph;	/* Glyph ID of control glyph */
  HBUINT16	coordPoint;	/* Index of contour point on the
				 * reference glyph */
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (6 + Types::HBGlyphID::static_size);
};

ಸಂರಚನಾ BaseCoordFormat3
{
  hb_position_t get_coord (hb_font_t *font,
			   ಧ್ರುವ ItemVariationStore &var_store,
			   hb_direction_t direction) ಧ್ರುವ
  {
    ಧ್ರುವ Device &device = ಏತತ್+deviceTable;

    ಪ್ರತ್ಯಾಗಮ HB_DIRECTION_IS_HORIZONTAL (direction)
	 ? hb_saturate_add (font->em_scale_y (coordinate), device.get_y_delta (font, var_store))
	 : hb_saturate_add (font->em_scale_x (coordinate), device.get_x_delta (font, var_store));
  }

  ಶೂನ್ಯ collect_variation_indices (hb_set_t& varidx_set /* OUT */) ಧ್ರುವ
  {
    unsigned varidx = (ಏತತ್+deviceTable).get_variation_index ();
    varidx_set.add (varidx);
  }

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    auto *out = c->serializer->embed (*ಏತತ್);
    ಯದಿ (unlikely (!out)) return_trace (ಅಸತ್ಯ);

    ಯದಿ (!c->plan->pinned_at_default)
    {
      unsigned var_idx = (ಏತತ್+deviceTable).get_variation_index ();
      ಯದಿ (var_idx != VarIdx::NO_VARIATION)
      {
        hb_pair_t<unsigned, ಪೂರ್ಣಾಙ್ಕ> *v;
        ಯದಿ (!c->plan->base_variation_idx_map.has (var_idx, &v))
          return_trace (ಅಸತ್ಯ);
        
        ಯದಿ (unlikely (!c->serializer->check_assign (out->coordinate, coordinate + hb_second (*v),
                                                    HB_SERIALIZE_ERROR_INT_OVERFLOW)))
          return_trace (ಅಸತ್ಯ);
      }
    }
    
    out->deviceTable.serialize_copy (c->serializer, deviceTable,
                                     ಏತತ್, 0,
                                     hb_serialize_context_t::Head,
                                     &c->plan->base_variation_idx_map);
    return_trace (ಸತ್ಯ);
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (likely (c->check_struct (ಏತತ್) &&
			  deviceTable.sanitize (c, ಏತತ್)));
  }

  ರಕ್ಷಿತ:
  HBUINT16	format;		/* Format identifier--format = 3 */
  FWORD		coordinate;	/* X or Y value, in design units */
  Offset16To<Device>
		deviceTable;	/* Offset to Device table for X or
				 * Y value, from beginning of
				 * BaseCoord table (may be NULL). */
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (6);
};

ಸಂರಚನಾ BaseCoord
{
  ತಾರ್ಕಿಕ has_data () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ u.format.v; }

  hb_position_t get_coord (hb_font_t            *font,
			   ಧ್ರುವ ItemVariationStore &var_store,
			   hb_direction_t        direction) ಧ್ರುವ
  {
    ವಿಕಲ್ಪ (u.format.v) {
    ಪಕ್ಷ 1: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format1.get_coord (font, direction);
    ಪಕ್ಷ 2: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format2.get_coord (font, direction);
    ಪಕ್ಷ 3: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format3.get_coord (font, var_store, direction);
#ifndef HB_NO_BEYOND_64K
    ಪಕ್ಷ 4: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format4.get_coord (font, direction);
#endif
    ಸ್ವಭಾವ:ಪ್ರತ್ಯಾಗಮ 0;
    }
  }

  ಶೂನ್ಯ collect_variation_indices (hb_set_t& varidx_set /* OUT */) ಧ್ರುವ
  {
    ವಿಕಲ್ಪ (u.format.v) {
    ಪಕ್ಷ 3: hb_barrier (); u.format3.collect_variation_indices (varidx_set); ಪ್ರತ್ಯಾಗಮ;
    ಸ್ವಭಾವ:ಪ್ರತ್ಯಾಗಮ;
    }
  }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ಧ್ರುವ
  {
    ಯದಿ (unlikely (!c->may_dispatch (ಏತತ್, &u.format.v))) ಪ್ರತ್ಯಾಗಮ c->no_dispatch_return_ಮೌಲ್ಯ ();
    TRACE_DISPATCH (ಏತತ್, u.format.v);
    ವಿಕಲ್ಪ (u.format.v) {
    ಪಕ್ಷ 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    ಪಕ್ಷ 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    ಪಕ್ಷ 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
#ifndef HB_NO_BEYOND_64K
    ಪಕ್ಷ 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
#endif
    ಸ್ವಭಾವ:return_trace (c->default_return_ಮೌಲ್ಯ ());
    }
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    ಯದಿ (unlikely (!u.format.v.sanitize (c))) return_trace (ಅಸತ್ಯ);
    hb_barrier ();
    ವಿಕಲ್ಪ (u.format.v) {
    ಪಕ್ಷ 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    ಪಕ್ಷ 2: hb_barrier (); return_trace (u.format2.sanitize (c));
    ಪಕ್ಷ 3: hb_barrier (); return_trace (u.format3.sanitize (c));
#ifndef HB_NO_BEYOND_64K
    ಪಕ್ಷ 4: hb_barrier (); return_trace (u.format4.sanitize (c));
#endif
    ಸ್ವಭಾವ:return_trace (ಅಸತ್ಯ);
    }
  }

  ರಕ್ಷಿತ:
  ಸಙ್ಘ {
  ಸಂರಚನಾ { HBUINT16 v; }	format;
  BaseCoordFormat1	format1;
  BaseCoordFormat2_4<Layout::SmallTypes>
			format2;
  BaseCoordFormat3	format3;
#ifndef HB_NO_BEYOND_64K
  BaseCoordFormat2_4<Layout::MediumTypes>
			format4;
#endif
  } u;
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_UNION (2, format.v);
};

ಸಂರಚನಾ FeatMinMaxRecord
{
  ಪೂರ್ಣಾಙ್ಕ cmp (hb_tag_t key) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ tag.cmp (key); }

  ತಾರ್ಕಿಕ has_data () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ tag; }

  hb_tag_t get_feature_tag () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ tag; }

  ಶೂನ್ಯ get_min_max (ಧ್ರುವ BaseCoord **min, ಧ್ರುವ BaseCoord **max) ಧ್ರುವ
  {
    ಯದಿ (likely (min)) *min = &(ಏತತ್+minCoord);
    ಯದಿ (likely (max)) *max = &(ಏತತ್+maxCoord);
  }

  ಶೂನ್ಯ collect_variation_indices (ಧ್ರುವ hb_subset_plan_t* plan,
                                  ಧ್ರುವ ಶೂನ್ಯ *base,
                                  hb_set_t& varidx_set /* OUT */) ಧ್ರುವ
  {
    ಯದಿ (!plan->layout_features.has (tag))
      ಪ್ರತ್ಯಾಗಮ;

    (base+minCoord).collect_variation_indices (varidx_set);
    (base+maxCoord).collect_variation_indices (varidx_set);
  }

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c,
               ಧ್ರುವ ಶೂನ್ಯ *base) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    auto *out = c->serializer->embed (*ಏತತ್);
    ಯದಿ (unlikely (!out)) return_trace (ಅಸತ್ಯ);

    ತಾರ್ಕಿಕ ret = out->minCoord.serialize_subset (c, minCoord, base);
    ret |= out->maxCoord.serialize_subset (c, maxCoord, base);
    return_trace (ret);
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c, ಧ್ರುವ ಶೂನ್ಯ *base) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (likely (c->check_struct (ಏತತ್) &&
			  minCoord.sanitize (c, base) &&
			  maxCoord.sanitize (c, base)));
  }

  ರಕ್ಷಿತ:
  Tag		tag;		/* 4-byte feature identification tag--must
				 * match feature tag in FeatureList */
  Offset16To<BaseCoord>
		minCoord;	/* Offset to BaseCoord table that defines
				 * the minimum extent value, from beginning
				 * of MinMax table (may be NULL) */
  Offset16To<BaseCoord>
		maxCoord;	/* Offset to BaseCoord table that defines
				 * the maximum extent value, from beginning
				 * of MinMax table (may be NULL) */
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (8);
};

ಸಂರಚನಾ MinMax
{
  ಶೂನ್ಯ get_min_max (hb_tag_t          feature_tag,
		    ಧ್ರುವ BaseCoord **min,
		    ಧ್ರುವ BaseCoord **max) ಧ್ರುವ
  {
    ಧ್ರುವ FeatMinMaxRecord &minMaxCoord = featMinMaxRecords.bsearch (feature_tag);
    ಯದಿ (minMaxCoord.has_data ())
      minMaxCoord.get_min_max (min, max);
    ಅನ್ಯಥಾ
    {
      ಯದಿ (likely (min)) *min = &(ಏತತ್+minCoord);
      ಯದಿ (likely (max)) *max = &(ಏತತ್+maxCoord);
    }
  }

  ಶೂನ್ಯ collect_variation_indices (ಧ್ರುವ hb_subset_plan_t* plan,
                                  hb_set_t& varidx_set /* OUT */) ಧ್ರುವ
  {
    (ಏತತ್+minCoord).collect_variation_indices (varidx_set);
    (ಏತತ್+maxCoord).collect_variation_indices (varidx_set);
    ಪ್ರತಿ (ಧ್ರುವ FeatMinMaxRecord& record : featMinMaxRecords)
      record.collect_variation_indices (plan, ಏತತ್, varidx_set);
  }

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    auto *out = c->serializer->start_embed (*ಏತತ್);
    ಯದಿ (unlikely (!out || !c->serializer->extend_min (out))) return_trace (ಅಸತ್ಯ);

    ತಾರ್ಕಿಕ ret = out->minCoord.serialize_subset (c, minCoord, ಏತತ್);
    ret |= out->maxCoord.serialize_subset (c, maxCoord, ಏತತ್);

    unsigned len = 0;
    ಪ್ರತಿ (ಧ್ರುವ FeatMinMaxRecord& _ : featMinMaxRecords)
    {
      hb_tag_t feature_tag = _.get_feature_tag ();
      ಯದಿ (!c->plan->layout_features.has (feature_tag))
        ಅನುವರ್ತ;

      auto snap = c->serializer->snapshot ();
      ಯದಿ (!_.subset (c, ಏತತ್)) c->serializer->revert (snap);
      ಅನ್ಯಥಾ len++;
    }

    ಯದಿ (len > 0) ret = ಸತ್ಯ;
    ಯದಿ (!ret) return_trace (ಅಸತ್ಯ);
    return_trace (c->serializer->check_assign (out->featMinMaxRecords.len, len,
                                               HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (likely (c->check_struct (ಏತತ್) &&
			  minCoord.sanitize (c, ಏತತ್) &&
			  maxCoord.sanitize (c, ಏತತ್) &&
			  featMinMaxRecords.sanitize (c, ಏತತ್)));
  }

  ರಕ್ಷಿತ:
  Offset16To<BaseCoord>
		minCoord;	/* Offset to BaseCoord table that defines
				 * minimum extent value, from the beginning
				 * of MinMax table (may be NULL) */
  Offset16To<BaseCoord>
		maxCoord;	/* Offset to BaseCoord table that defines
				 * maximum extent value, from the beginning
				 * of MinMax table (may be NULL) */
  SortedArray16Of<FeatMinMaxRecord>
		featMinMaxRecords;
				/* Array of FeatMinMaxRecords, in alphabetical
				 * order by featureTableTag */
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_ARRAY (6, featMinMaxRecords);
};

ಸಂರಚನಾ Base_ಮೌಲ್ಯಗಳು
{
  ಧ್ರುವ BaseCoord &get_base_coord (ಪೂರ್ಣಾಙ್ಕ baseline_tag_index) ಧ್ರುವ
  {
    ಯದಿ (baseline_tag_index == -1) baseline_tag_index = defaultIndex;
    ಪ್ರತ್ಯಾಗಮ ಏತತ್+baseCoords[baseline_tag_index];
  }

  ಶೂನ್ಯ collect_variation_indices (hb_set_t& varidx_set /* OUT */) ಧ್ರುವ
  {
    ಪ್ರತಿ (ಧ್ರುವ auto& _ : baseCoords)
      (ಏತತ್+_).collect_variation_indices (varidx_set);
  }

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    auto *out = c->serializer->start_embed (*ಏತತ್);
    ಯದಿ (unlikely (!out || !c->serializer->extend_min (out))) return_trace (ಅಸತ್ಯ);
    out->defaultIndex = defaultIndex;

    ತಾರ್ಕಿಕ ret = ಅಸತ್ಯ;
    ಪ್ರತಿ (ಧ್ರುವ auto& _ : baseCoords)
    {
      auto *o = out->baseCoords.serialize_append (c->serializer);
      ಯದಿ (unlikely (!o)) return_trace (ಅಸತ್ಯ);
       ret |= o->serialize_subset (c, _, ಏತತ್);
    }

    return_trace (ret);
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (likely (c->check_struct (ಏತತ್) &&
			  baseCoords.sanitize (c, ಏತತ್)));
  }

  ರಕ್ಷಿತ:
  Index		defaultIndex;	/* Index number of default baseline for this
				 * script — equals index position of baseline tag
				 * in baselineTags array of the BaseTagList */
  Array16OfOffset16To<BaseCoord>
		baseCoords;	/* Number of BaseCoord tables defined — should equal
				 * baseTagCount in the BaseTagList
				 *
				 * Array of offsets to BaseCoord tables, from beginning of
				 * BaseValues table — order matches baselineTags array in
				 * the BaseTagList */
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_ARRAY (4, baseCoords);
};

ಸಂರಚನಾ BaseLangSysRecord
{
  ಪೂರ್ಣಾಙ್ಕ cmp (hb_tag_t key) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ baseLangSysTag.cmp (key); }

  ತಾರ್ಕಿಕ has_data () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ baseLangSysTag; }

  ಧ್ರುವ MinMax &get_min_max (ಧ್ರುವ ಶೂನ್ಯ* base) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ base+minMax; }

  ಶೂನ್ಯ collect_variation_indices (ಧ್ರುವ ಶೂನ್ಯ* base,
                                  ಧ್ರುವ hb_subset_plan_t* plan,
                                  hb_set_t& varidx_set /* OUT */) ಧ್ರುವ
  { (base+minMax).collect_variation_indices (plan, varidx_set); }

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c,
               ಧ್ರುವ ಶೂನ್ಯ *base) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    auto *out = c->serializer->embed (*ಏತತ್);
    ಯದಿ (unlikely (!out)) return_trace (ಅಸತ್ಯ);

    return_trace (out->minMax.serialize_subset (c, minMax, base));
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c, ಧ್ರುವ ಶೂನ್ಯ *base) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (likely (c->check_struct (ಏತತ್) &&
			  minMax.sanitize (c, base)));
  }

  ರಕ್ಷಿತ:
  Tag		baseLangSysTag;	/* 4-byte language system identification tag */
  Offset16To<MinMax>
		minMax;		/* Offset to MinMax table, from beginning
				 * of BaseScript table */
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (6);
};

ಸಂರಚನಾ BaseScript
{
  ಧ್ರುವ MinMax &get_min_max (hb_tag_t language_tag) ಧ್ರುವ
  {
    ಧ್ರುವ BaseLangSysRecord& record = baseLangSysRecords.bsearch (language_tag);
    ಪ್ರತ್ಯಾಗಮ record.has_data () ? record.get_min_max (ಏತತ್) : ಏತತ್+defaultMinMax;
  }

  ಧ್ರುವ BaseCoord &get_base_coord (ಪೂರ್ಣಾಙ್ಕ baseline_tag_index) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ (ಏತತ್+base_ಮೌಲ್ಯಗಳು).get_base_coord (baseline_tag_index); }

  ತಾರ್ಕಿಕ has_ಮೌಲ್ಯಗಳು () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ base_ಮೌಲ್ಯಗಳು; }
  ತಾರ್ಕಿಕ has_min_max () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ defaultMinMax || baseLangSysRecords; }

  ಶೂನ್ಯ collect_variation_indices (ಧ್ರುವ hb_subset_plan_t* plan,
                                  hb_set_t& varidx_set /* OUT */) ಧ್ರುವ
  {
    (ಏತತ್+base_ಮೌಲ್ಯಗಳು).collect_variation_indices (varidx_set);
    (ಏತತ್+defaultMinMax).collect_variation_indices (plan, varidx_set);
    
    ಪ್ರತಿ (ಧ್ರುವ BaseLangSysRecord& _ : baseLangSysRecords)
      _.collect_variation_indices (ಏತತ್, plan, varidx_set);
  }

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    auto *out = c->serializer->start_embed (*ಏತತ್);
    ಯದಿ (unlikely (!out || !c->serializer->extend_min (out))) return_trace (ಅಸತ್ಯ);

    ತಾರ್ಕಿಕ ret = out->base_ಮೌಲ್ಯಗಳು.serialize_subset (c, base_ಮೌಲ್ಯಗಳು, ಏತತ್);
    ret |= out->defaultMinMax.serialize_subset (c, defaultMinMax, ಏತತ್);

    unsigned len = 0;
    ಪ್ರತಿ (ಧ್ರುವ auto& _ : baseLangSysRecords)
    {
      auto snap = c->serializer->snapshot ();
      ಯದಿ (_.subset (c, ಏತತ್)) {
        ret = ಸತ್ಯ;
        len++;
      }
      ಅನ್ಯಥಾ c->serializer->revert (snap);
    }

    ಯದಿ (!ret) return_trace (ಅಸತ್ಯ);
    return_trace (c->serializer->check_assign (out->baseLangSysRecords.len, len,
                                               HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (likely (c->check_struct (ಏತತ್) &&
			  base_ಮೌಲ್ಯಗಳು.sanitize (c, ಏತತ್) &&
			  defaultMinMax.sanitize (c, ಏತತ್) &&
			  baseLangSysRecords.sanitize (c, ಏತತ್)));
  }

  ರಕ್ಷಿತ:
  Offset16To<Base_ಮೌಲ್ಯಗಳು>
		base_ಮೌಲ್ಯಗಳು;	/* Offset to BaseValues table, from beginning
				 * of BaseScript table (may be NULL) */
  Offset16To<MinMax>
		defaultMinMax;	/* Offset to MinMax table, from beginning of
				 * BaseScript table (may be NULL) */
  SortedArray16Of<BaseLangSysRecord>
		baseLangSysRecords;
				/* Number of BaseLangSysRecords
				 * defined — may be zero (0) */

  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_ARRAY (6, baseLangSysRecords);
};

ಸಂರಚನಾ BaseScriptList;
ಸಂರಚನಾ BaseScriptRecord
{
  ಪೂರ್ಣಾಙ್ಕ cmp (hb_tag_t key) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ baseScriptTag.cmp (key); }

  ತಾರ್ಕಿಕ has_data () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ baseScriptTag; }

  hb_tag_t get_script_tag () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ baseScriptTag; }

  ಧ್ರುವ BaseScript &get_base_script (ಧ್ರುವ BaseScriptList *list) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ list+baseScript; }

  ಶೂನ್ಯ collect_variation_indices (ಧ್ರುವ hb_subset_plan_t* plan,
                                  ಧ್ರುವ ಶೂನ್ಯ* list,
                                  hb_set_t& varidx_set /* OUT */) ಧ್ರುವ
  {
    ಯದಿ (!plan->layout_scripts.has (baseScriptTag))
      ಪ್ರತ್ಯಾಗಮ;

    (list+baseScript).collect_variation_indices (plan, varidx_set);
  }

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c,
               ಧ್ರುವ ಶೂನ್ಯ *base) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    auto *out = c->serializer->embed (*ಏತತ್);
    ಯದಿ (unlikely (!out)) return_trace (ಅಸತ್ಯ);

    return_trace (out->baseScript.serialize_subset (c, baseScript, base));
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c, ಧ್ರುವ ಶೂನ್ಯ *base) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (likely (c->check_struct (ಏತತ್) &&
			  baseScript.sanitize (c, base)));
  }

  ರಕ್ಷಿತ:
  Tag		baseScriptTag;	/* 4-byte script identification tag */
  Offset16To<BaseScript>
		baseScript;	/* Offset to BaseScript table, from beginning
				 * of BaseScriptList */

  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (6);
};

ಸಂರಚನಾ BaseScriptList
{
  ಧ್ರುವ BaseScript &get_base_script (hb_tag_t script) ಧ್ರುವ
  {
    ಧ್ರುವ BaseScriptRecord *record = &baseScriptRecords.bsearch (script);
    ಯದಿ (!record->has_data ()) record = &baseScriptRecords.bsearch (HB_TAG ('D','F','L','T'));
    ಪ್ರತ್ಯಾಗಮ record->has_data () ? record->get_base_script (ಏತತ್) : Null (BaseScript);
  }

  ಶೂನ್ಯ collect_variation_indices (ಧ್ರುವ hb_subset_plan_t* plan,
                                  hb_set_t& varidx_set /* OUT */) ಧ್ರುವ
  {
    ಪ್ರತಿ (ಧ್ರುವ BaseScriptRecord& _ : baseScriptRecords)
      _.collect_variation_indices (plan, ಏತತ್, varidx_set);
  }

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    auto *out = c->serializer->start_embed (*ಏತತ್);
    ಯದಿ (unlikely (!out || !c->serializer->extend_min (out))) return_trace (ಅಸತ್ಯ);

    unsigned len = 0;
    ಪ್ರತಿ (ಧ್ರುವ BaseScriptRecord& _ : baseScriptRecords)
    {
      hb_tag_t script_tag = _.get_script_tag ();
      ಯದಿ (!c->plan->layout_scripts.has (script_tag))
        ಅನುವರ್ತ;

      auto snap = c->serializer->snapshot ();
      ಯದಿ (!_.subset (c, ಏತತ್)) c->serializer->revert (snap);
      ಅನ್ಯಥಾ len++;
    }

    ಯದಿ (!len) return_trace (ಅಸತ್ಯ);
    return_trace (c->serializer->check_assign (out->baseScriptRecords.len, len,
                                               HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್) &&
		  baseScriptRecords.sanitize (c, ಏತತ್));
  }

  ರಕ್ಷಿತ:
  SortedArray16Of<BaseScriptRecord>
			baseScriptRecords;

  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_ARRAY (2, baseScriptRecords);
};

ಸಂರಚನಾ Axis
{
  ತಾರ್ಕಿಕ get_baseline (hb_tag_t          baseline_tag,
		     hb_tag_t          script_tag,
		     hb_tag_t          language_tag,
		     ಧ್ರುವ BaseCoord **coord) ಧ್ರುವ
  {
    ಧ್ರುವ BaseScript &base_script = (ಏತತ್+baseScriptList).get_base_script (script_tag);
    ಯದಿ (!base_script.has_ಮೌಲ್ಯಗಳು ())
    {
      *coord = nullptr;
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }

    ಯದಿ (likely (coord))
    {
      unsigned ಪೂರ್ಣಾಙ್ಕ tag_index = 0;
      ಯದಿ (!(ಏತತ್+baseTagList).bfind (baseline_tag, &tag_index))
      {
        *coord = nullptr;
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
      }
      *coord = &base_script.get_base_coord (tag_index);
    }

    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }

  ತಾರ್ಕಿಕ get_min_max (hb_tag_t          script_tag,
		    hb_tag_t          language_tag,
		    hb_tag_t          feature_tag,
		    ಧ್ರುವ BaseCoord **min_coord,
		    ಧ್ರುವ BaseCoord **max_coord) ಧ್ರುವ
  {
    ಧ್ರುವ BaseScript &base_script = (ಏತತ್+baseScriptList).get_base_script (script_tag);
    ಯದಿ (!base_script.has_min_max ())
    {
      *min_coord = *max_coord = nullptr;
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }

    base_script.get_min_max (language_tag).get_min_max (feature_tag, min_coord, max_coord);

    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }

  ಶೂನ್ಯ collect_variation_indices (ಧ್ರುವ hb_subset_plan_t* plan,
                                  hb_set_t& varidx_set /* OUT */) ಧ್ರುವ
  { (ಏತತ್+baseScriptList).collect_variation_indices (plan, varidx_set); }

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    auto *out = c->serializer->embed (*ಏತತ್);
    ಯದಿ (unlikely (!out)) return_trace (ಅಸತ್ಯ);

    ತಾರ್ಕಿಕ ret = out->baseTagList.serialize_copy (c->serializer, baseTagList, ಏತತ್);
    ret |= out->baseScriptList.serialize_subset (c, baseScriptList, ಏತತ್);
    return_trace (ret);
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (likely (c->check_struct (ಏತತ್) &&
			  baseTagList.sanitize (c, ಏತತ್) &&
			  baseScriptList.sanitize (c, ಏತತ್)));
  }

  ರಕ್ಷಿತ:
  Offset16To<SortedArray16Of<Tag>>
		baseTagList;	/* Offset to BaseTagList table, from beginning
				 * of Axis table (may be NULL)
				 * Array of 4-byte baseline identification tags — must
				 * be in alphabetical order */
  Offset16To<BaseScriptList>
		baseScriptList;	/* Offset to BaseScriptList table, from beginning
				 * of Axis table
				 * Array of BaseScriptRecords, in alphabetical order
				 * by baseScriptTag */

  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (4);
};

ಸಂರಚನಾ BASE
{
  ಸ್ಥಿರ constexpr hb_tag_t tableTag = HB_OT_TAG_BASE;

  ಧ್ರುವ Axis &get_axis (hb_direction_t direction) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ HB_DIRECTION_IS_VERTICAL (direction) ? ಏತತ್+vAxis : ಏತತ್+hAxis; }

  ತಾರ್ಕಿಕ has_var_store () ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ version.to_int () >= 0x00010001u && varStore != 0; }

  ಧ್ರುವ ItemVariationStore &get_var_store () ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ version.to_int () < 0x00010001u ? Null (ItemVariationStore) : ಏತತ್+varStore; }

  ಶೂನ್ಯ collect_variation_indices (ಧ್ರುವ hb_subset_plan_t* plan,
                                  hb_set_t& varidx_set /* OUT */) ಧ್ರುವ
  {
    (ಏತತ್+hAxis).collect_variation_indices (plan, varidx_set);
    (ಏತತ್+vAxis).collect_variation_indices (plan, varidx_set);
  }

  ತಾರ್ಕಿಕ subset_varstore (hb_subset_context_t *c,
                        BASE *out /* OUT */) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    ಯದಿ (!c->serializer->allocate_size<Offset32To<ItemVariationStore>> (Offset32To<ItemVariationStore>::static_size))
        return_trace (ಅಸತ್ಯ);
    ಯದಿ (!c->plan->normalized_coords)
      return_trace (out->varStore.serialize_subset (c, varStore, ಏತತ್, c->plan->base_varstore_inner_maps.as_array ()));

    ಯದಿ (c->plan->all_axes_pinned)
      return_trace (ಸತ್ಯ);

    item_variations_t item_vars;
    ಯದಿ (!item_vars.instantiate (ಏತತ್+varStore, c->plan, ಸತ್ಯ, ಸತ್ಯ,
                                c->plan->base_varstore_inner_maps.as_array ()))
      return_trace (ಅಸತ್ಯ);

    ಯದಿ (!out->varStore.serialize_serialize (c->serializer,
                                            item_vars.has_long_word (),
                                            c->plan->axis_tags,
                                            item_vars.get_region_list (),
                                            item_vars.get_vardata_encodings ()))
      return_trace (ಅಸತ್ಯ);

    ಧ್ರುವ hb_map_t &varidx_map = item_vars.get_varidx_map ();
    /* base_variation_idx_map in the plan is old_varidx->(varidx, delta)
     * mapping, new varidx is generated for subsetting, we need to remap this
     * after instancing */
    ಪ್ರತಿ (auto _ : c->plan->base_variation_idx_map.iter_ref ())
    {
      uint32_t varidx = _.second.first;
      uint32_t *new_varidx;
      ಯದಿ (varidx_map.has (varidx, &new_varidx))
        _.second.first = *new_varidx;
      ಅನ್ಯಥಾ
        _.second.first = HB_OT_LAYOUT_NO_VARIATIONS_INDEX;
    }
    return_trace (ಸತ್ಯ);
  }

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    auto *out = c->serializer->start_embed (*ಏತತ್);
    ಯದಿ (unlikely (!out || !c->serializer->extend_min (out))) return_trace (ಅಸತ್ಯ);

    out->version = version;
    ತಾರ್ಕಿಕ ret = ಅಸತ್ಯ;
    ಯದಿ (version.to_int () >= 0x00010001u) ret |= subset_varstore (c, out);

    ret |= out->hAxis.serialize_subset (c, hAxis, ಏತತ್);
    ret |= out->vAxis.serialize_subset (c, vAxis, ಏತತ್);

    return_trace (ret);
  }

  ತಾರ್ಕಿಕ get_baseline (hb_font_t      *font,
		     hb_tag_t        baseline_tag,
		     hb_direction_t  direction,
		     hb_tag_t        script_tag,
		     hb_tag_t        language_tag,
		     hb_position_t  *base) ಧ್ರುವ
  {
    ಧ್ರುವ BaseCoord *base_coord = nullptr;
    ಯದಿ (unlikely (!get_axis (direction).get_baseline (baseline_tag, script_tag, language_tag, &base_coord) ||
		  !base_coord || !base_coord->has_data ()))
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    ಯದಿ (likely (base))
      *base = base_coord->get_coord (font, get_var_store (), direction);

    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }

  ತಾರ್ಕಿಕ get_min_max (hb_font_t      *font,
		    hb_direction_t  direction,
		    hb_tag_t        script_tag,
		    hb_tag_t        language_tag,
		    hb_tag_t        feature_tag,
		    hb_position_t  *min,
		    hb_position_t  *max) ಧ್ರುವ
  {
    ಧ್ರುವ BaseCoord *min_coord, *max_coord;
    ಯದಿ (!get_axis (direction).get_min_max (script_tag, language_tag, feature_tag,
					   &min_coord, &max_coord))
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    ಧ್ರುವ ItemVariationStore &var_store = get_var_store ();
    ಯದಿ (likely (min && min_coord)) *min = min_coord->get_coord (font, var_store, direction);
    ಯದಿ (likely (max && max_coord)) *max = max_coord->get_coord (font, var_store, direction);
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (likely (c->check_struct (ಏತತ್) &&
			  hb_barrier () &&
			  likely (version.major == 1) &&
			  hAxis.sanitize (c, ಏತತ್) &&
			  vAxis.sanitize (c, ಏತತ್) &&
			  (version.to_int () < 0x00010001u || varStore.sanitize (c, ಏತತ್))));
  }

  ರಕ್ಷಿತ:
  FixedVersion<>version;	/* Version of the BASE table */
  Offset16To<Axis>hAxis;		/* Offset to horizontal Axis table, from beginning
				 * of BASE table (may be NULL) */
  Offset16To<Axis>vAxis;		/* Offset to vertical Axis table, from beginning
				 * of BASE table (may be NULL) */
  Offset32To<ItemVariationStore>
		varStore;	/* Offset to the table of Item Variation
				 * Store--from beginning of BASE
				 * header (may be NULL).  Introduced
				 * in version 0x00010001. */
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_MIN (8);
};


} /* namespace OT */


#endif /* HB_OT_LAYOUT_BASE_TABLE_HH */
