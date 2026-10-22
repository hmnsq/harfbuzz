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

#অন্তর্ভাৱ "hb-open-type.hh"
#অন্তর্ভাৱ "hb-ot-layout-common.hh"

নামাকাশ OT {

/*
 * BASE -- Baseline
 * https://docs.microsoft.com/en-us/typography/opentype/spec/base
 */

সংরচনা BaseCoordFormat1
{
  hb_position_t get_coord (hb_font_t *font, hb_direction_t direction) ধ্রুৱ
  {
    ঘূৰাই HB_DIRECTION_IS_HORIZONTAL (direction) ? font->em_scale_y (coordinate) : font->em_scale_x (coordinate);
  }

  তার্কিক subset (hb_subset_context_t *c) ধ্রুৱ
  {
    TRACE_SUBSET (এতত্);
    return_trace ((তার্কিক) c->serializer->embed (*এতত্));
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্));
  }

  রক্ষিত:
  HBUINT16	format;		/* Format identifier--format = 1 */
  FWORD		coordinate;	/* X or Y value, in design units */
  সার্ৱজনিক:
  DEFINE_SIZE_STATIC (4);
};

template <typename Types>
সংরচনা BaseCoordFormat2_4
{
  hb_position_t get_coord (hb_font_t *font, hb_direction_t direction) ধ্রুৱ
  {
    /* TODO */
    ঘূৰাই HB_DIRECTION_IS_HORIZONTAL (direction) ? font->em_scale_y (coordinate) : font->em_scale_x (coordinate);
  }

  তার্কিক subset (hb_subset_context_t *c) ধ্রুৱ
  {
    TRACE_SUBSET (এতত্);
    auto *out = c->serializer->embed (*এতত্);
    যদি (unlikely (!out)) return_trace (মিছা);

    hb_codepoint_t *new_gid;
    যদি (!c->plan->glyph_map->has (referenceGlyph, &new_gid))
      return_trace (মিছা);

    return_trace (c->serializer->check_assign (out->referenceGlyph,
                                               *new_gid,
                                               HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্));
  }

  রক্ষিত:
  HBUINT16	format;		/* Format identifier--format = 2 or 4 */
  FWORD		coordinate;	/* X or Y value, in design units */
  typename Types::HBGlyphID
		referenceGlyph;	/* Glyph ID of control glyph */
  HBUINT16	coordPoint;	/* Index of contour point on the
				 * reference glyph */
  সার্ৱজনিক:
  DEFINE_SIZE_STATIC (6 + Types::HBGlyphID::static_size);
};

সংরচনা BaseCoordFormat3
{
  hb_position_t get_coord (hb_font_t *font,
			   ধ্রুৱ ItemVariationStore &var_store,
			   hb_direction_t direction) ধ্রুৱ
  {
    ধ্রুৱ Device &device = এতত্+deviceTable;

    ঘূৰাই HB_DIRECTION_IS_HORIZONTAL (direction)
	 ? hb_saturate_add (font->em_scale_y (coordinate), device.get_y_delta (font, var_store))
	 : hb_saturate_add (font->em_scale_x (coordinate), device.get_x_delta (font, var_store));
  }

  শূন্য collect_variation_indices (hb_set_t& varidx_set /* OUT */) ধ্রুৱ
  {
    unsigned varidx = (এতত্+deviceTable).get_variation_index ();
    varidx_set.add (varidx);
  }

  তার্কিক subset (hb_subset_context_t *c) ধ্রুৱ
  {
    TRACE_SUBSET (এতত্);
    auto *out = c->serializer->embed (*এতত্);
    যদি (unlikely (!out)) return_trace (মিছা);

    যদি (!c->plan->pinned_at_default)
    {
      unsigned var_idx = (এতত্+deviceTable).get_variation_index ();
      যদি (var_idx != VarIdx::NO_VARIATION)
      {
        hb_pair_t<unsigned, পূর্ণাঙ্ক> *v;
        যদি (!c->plan->base_variation_idx_map.has (var_idx, &v))
          return_trace (মিছা);
        
        যদি (unlikely (!c->serializer->check_assign (out->coordinate, coordinate + hb_second (*v),
                                                    HB_SERIALIZE_ERROR_INT_OVERFLOW)))
          return_trace (মিছা);
      }
    }
    
    out->deviceTable.serialize_copy (c->serializer, deviceTable,
                                     এতত্, 0,
                                     hb_serialize_context_t::Head,
                                     &c->plan->base_variation_idx_map);
    return_trace (সঁচা);
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (likely (c->check_struct (এতত্) &&
			  deviceTable.sanitize (c, এতত্)));
  }

  রক্ষিত:
  HBUINT16	format;		/* Format identifier--format = 3 */
  FWORD		coordinate;	/* X or Y value, in design units */
  Offset16To<Device>
		deviceTable;	/* Offset to Device table for X or
				 * Y value, from beginning of
				 * BaseCoord table (may be NULL). */
  সার্ৱজনিক:
  DEFINE_SIZE_STATIC (6);
};

সংরচনা BaseCoord
{
  তার্কিক has_data () ধ্রুৱ { ঘূৰাই u.format.v; }

  hb_position_t get_coord (hb_font_t            *font,
			   ধ্রুৱ ItemVariationStore &var_store,
			   hb_direction_t        direction) ধ্রুৱ
  {
    ৱিকল্প (u.format.v) {
    পক্ষ 1: hb_barrier (); ঘূৰাই u.format1.get_coord (font, direction);
    পক্ষ 2: hb_barrier (); ঘূৰাই u.format2.get_coord (font, direction);
    পক্ষ 3: hb_barrier (); ঘূৰাই u.format3.get_coord (font, var_store, direction);
#ifndef HB_NO_BEYOND_64K
    পক্ষ 4: hb_barrier (); ঘূৰাই u.format4.get_coord (font, direction);
#endif
    স্ৱভাৱ:ঘূৰাই 0;
    }
  }

  শূন্য collect_variation_indices (hb_set_t& varidx_set /* OUT */) ধ্রুৱ
  {
    ৱিকল্প (u.format.v) {
    পক্ষ 3: hb_barrier (); u.format3.collect_variation_indices (varidx_set); ঘূৰাই;
    স্ৱভাৱ:ঘূৰাই;
    }
  }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ধ্রুৱ
  {
    যদি (unlikely (!c->may_dispatch (এতত্, &u.format.v))) ঘূৰাই c->no_dispatch_return_মান ();
    TRACE_DISPATCH (এতত্, u.format.v);
    ৱিকল্প (u.format.v) {
    পক্ষ 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    পক্ষ 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    পক্ষ 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
#ifndef HB_NO_BEYOND_64K
    পক্ষ 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
#endif
    স্ৱভাৱ:return_trace (c->default_return_মান ());
    }
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    যদি (unlikely (!u.format.v.sanitize (c))) return_trace (মিছা);
    hb_barrier ();
    ৱিকল্প (u.format.v) {
    পক্ষ 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    পক্ষ 2: hb_barrier (); return_trace (u.format2.sanitize (c));
    পক্ষ 3: hb_barrier (); return_trace (u.format3.sanitize (c));
#ifndef HB_NO_BEYOND_64K
    পক্ষ 4: hb_barrier (); return_trace (u.format4.sanitize (c));
#endif
    স্ৱভাৱ:return_trace (মিছা);
    }
  }

  রক্ষিত:
  সঙ্ঘ {
  সংরচনা { HBUINT16 v; }	format;
  BaseCoordFormat1	format1;
  BaseCoordFormat2_4<Layout::SmallTypes>
			format2;
  BaseCoordFormat3	format3;
#ifndef HB_NO_BEYOND_64K
  BaseCoordFormat2_4<Layout::MediumTypes>
			format4;
#endif
  } u;
  সার্ৱজনিক:
  DEFINE_SIZE_UNION (2, format.v);
};

সংরচনা FeatMinMaxRecord
{
  পূর্ণাঙ্ক cmp (hb_tag_t key) ধ্রুৱ { ঘূৰাই tag.cmp (key); }

  তার্কিক has_data () ধ্রুৱ { ঘূৰাই tag; }

  hb_tag_t get_feature_tag () ধ্রুৱ { ঘূৰাই tag; }

  শূন্য get_min_max (ধ্রুৱ BaseCoord **min, ধ্রুৱ BaseCoord **max) ধ্রুৱ
  {
    যদি (likely (min)) *min = &(এতত্+minCoord);
    যদি (likely (max)) *max = &(এতত্+maxCoord);
  }

  শূন্য collect_variation_indices (ধ্রুৱ hb_subset_plan_t* plan,
                                  ধ্রুৱ শূন্য *base,
                                  hb_set_t& varidx_set /* OUT */) ধ্রুৱ
  {
    যদি (!plan->layout_features.has (tag))
      ঘূৰাই;

    (base+minCoord).collect_variation_indices (varidx_set);
    (base+maxCoord).collect_variation_indices (varidx_set);
  }

  তার্কিক subset (hb_subset_context_t *c,
               ধ্রুৱ শূন্য *base) ধ্রুৱ
  {
    TRACE_SUBSET (এতত্);
    auto *out = c->serializer->embed (*এতত্);
    যদি (unlikely (!out)) return_trace (মিছা);

    তার্কিক ret = out->minCoord.serialize_subset (c, minCoord, base);
    ret |= out->maxCoord.serialize_subset (c, maxCoord, base);
    return_trace (ret);
  }

  তার্কিক sanitize (hb_sanitize_context_t *c, ধ্রুৱ শূন্য *base) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (likely (c->check_struct (এতত্) &&
			  minCoord.sanitize (c, base) &&
			  maxCoord.sanitize (c, base)));
  }

  রক্ষিত:
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
  সার্ৱজনিক:
  DEFINE_SIZE_STATIC (8);
};

সংরচনা MinMax
{
  শূন্য get_min_max (hb_tag_t          feature_tag,
		    ধ্রুৱ BaseCoord **min,
		    ধ্রুৱ BaseCoord **max) ধ্রুৱ
  {
    ধ্রুৱ FeatMinMaxRecord &minMaxCoord = featMinMaxRecords.bsearch (feature_tag);
    যদি (minMaxCoord.has_data ())
      minMaxCoord.get_min_max (min, max);
    অন্যথা
    {
      যদি (likely (min)) *min = &(এতত্+minCoord);
      যদি (likely (max)) *max = &(এতত্+maxCoord);
    }
  }

  শূন্য collect_variation_indices (ধ্রুৱ hb_subset_plan_t* plan,
                                  hb_set_t& varidx_set /* OUT */) ধ্রুৱ
  {
    (এতত্+minCoord).collect_variation_indices (varidx_set);
    (এতত্+maxCoord).collect_variation_indices (varidx_set);
    বাবে (ধ্রুৱ FeatMinMaxRecord& record : featMinMaxRecords)
      record.collect_variation_indices (plan, এতত্, varidx_set);
  }

  তার্কিক subset (hb_subset_context_t *c) ধ্রুৱ
  {
    TRACE_SUBSET (এতত্);
    auto *out = c->serializer->start_embed (*এতত্);
    যদি (unlikely (!out || !c->serializer->extend_min (out))) return_trace (মিছা);

    তার্কিক ret = out->minCoord.serialize_subset (c, minCoord, এতত্);
    ret |= out->maxCoord.serialize_subset (c, maxCoord, এতত্);

    unsigned len = 0;
    বাবে (ধ্রুৱ FeatMinMaxRecord& _ : featMinMaxRecords)
    {
      hb_tag_t feature_tag = _.get_feature_tag ();
      যদি (!c->plan->layout_features.has (feature_tag))
        অনুৱর্ত;

      auto snap = c->serializer->snapshot ();
      যদি (!_.subset (c, এতত্)) c->serializer->revert (snap);
      অন্যথা len++;
    }

    যদি (len > 0) ret = সঁচা;
    যদি (!ret) return_trace (মিছা);
    return_trace (c->serializer->check_assign (out->featMinMaxRecords.len, len,
                                               HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (likely (c->check_struct (এতত্) &&
			  minCoord.sanitize (c, এতত্) &&
			  maxCoord.sanitize (c, এতত্) &&
			  featMinMaxRecords.sanitize (c, এতত্)));
  }

  রক্ষিত:
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
  সার্ৱজনিক:
  DEFINE_SIZE_ARRAY (6, featMinMaxRecords);
};

সংরচনা Base_মানসমূহ
{
  ধ্রুৱ BaseCoord &get_base_coord (পূর্ণাঙ্ক baseline_tag_index) ধ্রুৱ
  {
    যদি (baseline_tag_index == -1) baseline_tag_index = defaultIndex;
    ঘূৰাই এতত্+baseCoords[baseline_tag_index];
  }

  শূন্য collect_variation_indices (hb_set_t& varidx_set /* OUT */) ধ্রুৱ
  {
    বাবে (ধ্রুৱ auto& _ : baseCoords)
      (এতত্+_).collect_variation_indices (varidx_set);
  }

  তার্কিক subset (hb_subset_context_t *c) ধ্রুৱ
  {
    TRACE_SUBSET (এতত্);
    auto *out = c->serializer->start_embed (*এতত্);
    যদি (unlikely (!out || !c->serializer->extend_min (out))) return_trace (মিছা);
    out->defaultIndex = defaultIndex;

    তার্কিক ret = মিছা;
    বাবে (ধ্রুৱ auto& _ : baseCoords)
    {
      auto *o = out->baseCoords.serialize_append (c->serializer);
      যদি (unlikely (!o)) return_trace (মিছা);
       ret |= o->serialize_subset (c, _, এতত্);
    }

    return_trace (ret);
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (likely (c->check_struct (এতত্) &&
			  baseCoords.sanitize (c, এতত্)));
  }

  রক্ষিত:
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
  সার্ৱজনিক:
  DEFINE_SIZE_ARRAY (4, baseCoords);
};

সংরচনা BaseLangSysRecord
{
  পূর্ণাঙ্ক cmp (hb_tag_t key) ধ্রুৱ { ঘূৰাই baseLangSysTag.cmp (key); }

  তার্কিক has_data () ধ্রুৱ { ঘূৰাই baseLangSysTag; }

  ধ্রুৱ MinMax &get_min_max (ধ্রুৱ শূন্য* base) ধ্রুৱ { ঘূৰাই base+minMax; }

  শূন্য collect_variation_indices (ধ্রুৱ শূন্য* base,
                                  ধ্রুৱ hb_subset_plan_t* plan,
                                  hb_set_t& varidx_set /* OUT */) ধ্রুৱ
  { (base+minMax).collect_variation_indices (plan, varidx_set); }

  তার্কিক subset (hb_subset_context_t *c,
               ধ্রুৱ শূন্য *base) ধ্রুৱ
  {
    TRACE_SUBSET (এতত্);
    auto *out = c->serializer->embed (*এতত্);
    যদি (unlikely (!out)) return_trace (মিছা);

    return_trace (out->minMax.serialize_subset (c, minMax, base));
  }

  তার্কিক sanitize (hb_sanitize_context_t *c, ধ্রুৱ শূন্য *base) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (likely (c->check_struct (এতত্) &&
			  minMax.sanitize (c, base)));
  }

  রক্ষিত:
  Tag		baseLangSysTag;	/* 4-byte language system identification tag */
  Offset16To<MinMax>
		minMax;		/* Offset to MinMax table, from beginning
				 * of BaseScript table */
  সার্ৱজনিক:
  DEFINE_SIZE_STATIC (6);
};

সংরচনা BaseScript
{
  ধ্রুৱ MinMax &get_min_max (hb_tag_t language_tag) ধ্রুৱ
  {
    ধ্রুৱ BaseLangSysRecord& record = baseLangSysRecords.bsearch (language_tag);
    ঘূৰাই record.has_data () ? record.get_min_max (এতত্) : এতত্+defaultMinMax;
  }

  ধ্রুৱ BaseCoord &get_base_coord (পূর্ণাঙ্ক baseline_tag_index) ধ্রুৱ
  { ঘূৰাই (এতত্+base_মানসমূহ).get_base_coord (baseline_tag_index); }

  তার্কিক has_মানসমূহ () ধ্রুৱ { ঘূৰাই base_মানসমূহ; }
  তার্কিক has_min_max () ধ্রুৱ { ঘূৰাই defaultMinMax || baseLangSysRecords; }

  শূন্য collect_variation_indices (ধ্রুৱ hb_subset_plan_t* plan,
                                  hb_set_t& varidx_set /* OUT */) ধ্রুৱ
  {
    (এতত্+base_মানসমূহ).collect_variation_indices (varidx_set);
    (এতত্+defaultMinMax).collect_variation_indices (plan, varidx_set);
    
    বাবে (ধ্রুৱ BaseLangSysRecord& _ : baseLangSysRecords)
      _.collect_variation_indices (এতত্, plan, varidx_set);
  }

  তার্কিক subset (hb_subset_context_t *c) ধ্রুৱ
  {
    TRACE_SUBSET (এতত্);
    auto *out = c->serializer->start_embed (*এতত্);
    যদি (unlikely (!out || !c->serializer->extend_min (out))) return_trace (মিছা);

    তার্কিক ret = out->base_মানসমূহ.serialize_subset (c, base_মানসমূহ, এতত্);
    ret |= out->defaultMinMax.serialize_subset (c, defaultMinMax, এতত্);

    unsigned len = 0;
    বাবে (ধ্রুৱ auto& _ : baseLangSysRecords)
    {
      auto snap = c->serializer->snapshot ();
      যদি (_.subset (c, এতত্)) {
        ret = সঁচা;
        len++;
      }
      অন্যথা c->serializer->revert (snap);
    }

    যদি (!ret) return_trace (মিছা);
    return_trace (c->serializer->check_assign (out->baseLangSysRecords.len, len,
                                               HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (likely (c->check_struct (এতত্) &&
			  base_মানসমূহ.sanitize (c, এতত্) &&
			  defaultMinMax.sanitize (c, এতত্) &&
			  baseLangSysRecords.sanitize (c, এতত্)));
  }

  রক্ষিত:
  Offset16To<Base_মানসমূহ>
		base_মানসমূহ;	/* Offset to BaseValues table, from beginning
				 * of BaseScript table (may be NULL) */
  Offset16To<MinMax>
		defaultMinMax;	/* Offset to MinMax table, from beginning of
				 * BaseScript table (may be NULL) */
  SortedArray16Of<BaseLangSysRecord>
		baseLangSysRecords;
				/* Number of BaseLangSysRecords
				 * defined — may be zero (0) */

  সার্ৱজনিক:
  DEFINE_SIZE_ARRAY (6, baseLangSysRecords);
};

সংরচনা BaseScriptList;
সংরচনা BaseScriptRecord
{
  পূর্ণাঙ্ক cmp (hb_tag_t key) ধ্রুৱ { ঘূৰাই baseScriptTag.cmp (key); }

  তার্কিক has_data () ধ্রুৱ { ঘূৰাই baseScriptTag; }

  hb_tag_t get_script_tag () ধ্রুৱ { ঘূৰাই baseScriptTag; }

  ধ্রুৱ BaseScript &get_base_script (ধ্রুৱ BaseScriptList *list) ধ্রুৱ
  { ঘূৰাই list+baseScript; }

  শূন্য collect_variation_indices (ধ্রুৱ hb_subset_plan_t* plan,
                                  ধ্রুৱ শূন্য* list,
                                  hb_set_t& varidx_set /* OUT */) ধ্রুৱ
  {
    যদি (!plan->layout_scripts.has (baseScriptTag))
      ঘূৰাই;

    (list+baseScript).collect_variation_indices (plan, varidx_set);
  }

  তার্কিক subset (hb_subset_context_t *c,
               ধ্রুৱ শূন্য *base) ধ্রুৱ
  {
    TRACE_SUBSET (এতত্);
    auto *out = c->serializer->embed (*এতত্);
    যদি (unlikely (!out)) return_trace (মিছা);

    return_trace (out->baseScript.serialize_subset (c, baseScript, base));
  }

  তার্কিক sanitize (hb_sanitize_context_t *c, ধ্রুৱ শূন্য *base) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (likely (c->check_struct (এতত্) &&
			  baseScript.sanitize (c, base)));
  }

  রক্ষিত:
  Tag		baseScriptTag;	/* 4-byte script identification tag */
  Offset16To<BaseScript>
		baseScript;	/* Offset to BaseScript table, from beginning
				 * of BaseScriptList */

  সার্ৱজনিক:
  DEFINE_SIZE_STATIC (6);
};

সংরচনা BaseScriptList
{
  ধ্রুৱ BaseScript &get_base_script (hb_tag_t script) ধ্রুৱ
  {
    ধ্রুৱ BaseScriptRecord *record = &baseScriptRecords.bsearch (script);
    যদি (!record->has_data ()) record = &baseScriptRecords.bsearch (HB_TAG ('D','F','L','T'));
    ঘূৰাই record->has_data () ? record->get_base_script (এতত্) : Null (BaseScript);
  }

  শূন্য collect_variation_indices (ধ্রুৱ hb_subset_plan_t* plan,
                                  hb_set_t& varidx_set /* OUT */) ধ্রুৱ
  {
    বাবে (ধ্রুৱ BaseScriptRecord& _ : baseScriptRecords)
      _.collect_variation_indices (plan, এতত্, varidx_set);
  }

  তার্কিক subset (hb_subset_context_t *c) ধ্রুৱ
  {
    TRACE_SUBSET (এতত্);
    auto *out = c->serializer->start_embed (*এতত্);
    যদি (unlikely (!out || !c->serializer->extend_min (out))) return_trace (মিছা);

    unsigned len = 0;
    বাবে (ধ্রুৱ BaseScriptRecord& _ : baseScriptRecords)
    {
      hb_tag_t script_tag = _.get_script_tag ();
      যদি (!c->plan->layout_scripts.has (script_tag))
        অনুৱর্ত;

      auto snap = c->serializer->snapshot ();
      যদি (!_.subset (c, এতত্)) c->serializer->revert (snap);
      অন্যথা len++;
    }

    যদি (!len) return_trace (মিছা);
    return_trace (c->serializer->check_assign (out->baseScriptRecords.len, len,
                                               HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্) &&
		  baseScriptRecords.sanitize (c, এতত্));
  }

  রক্ষিত:
  SortedArray16Of<BaseScriptRecord>
			baseScriptRecords;

  সার্ৱজনিক:
  DEFINE_SIZE_ARRAY (2, baseScriptRecords);
};

সংরচনা Axis
{
  তার্কিক get_baseline (hb_tag_t          baseline_tag,
		     hb_tag_t          script_tag,
		     hb_tag_t          language_tag,
		     ধ্রুৱ BaseCoord **coord) ধ্রুৱ
  {
    ধ্রুৱ BaseScript &base_script = (এতত্+baseScriptList).get_base_script (script_tag);
    যদি (!base_script.has_মানসমূহ ())
    {
      *coord = nullptr;
      ঘূৰাই মিছা;
    }

    যদি (likely (coord))
    {
      unsigned পূর্ণাঙ্ক tag_index = 0;
      যদি (!(এতত্+baseTagList).bfind (baseline_tag, &tag_index))
      {
        *coord = nullptr;
        ঘূৰাই মিছা;
      }
      *coord = &base_script.get_base_coord (tag_index);
    }

    ঘূৰাই সঁচা;
  }

  তার্কিক get_min_max (hb_tag_t          script_tag,
		    hb_tag_t          language_tag,
		    hb_tag_t          feature_tag,
		    ধ্রুৱ BaseCoord **min_coord,
		    ধ্রুৱ BaseCoord **max_coord) ধ্রুৱ
  {
    ধ্রুৱ BaseScript &base_script = (এতত্+baseScriptList).get_base_script (script_tag);
    যদি (!base_script.has_min_max ())
    {
      *min_coord = *max_coord = nullptr;
      ঘূৰাই মিছা;
    }

    base_script.get_min_max (language_tag).get_min_max (feature_tag, min_coord, max_coord);

    ঘূৰাই সঁচা;
  }

  শূন্য collect_variation_indices (ধ্রুৱ hb_subset_plan_t* plan,
                                  hb_set_t& varidx_set /* OUT */) ধ্রুৱ
  { (এতত্+baseScriptList).collect_variation_indices (plan, varidx_set); }

  তার্কিক subset (hb_subset_context_t *c) ধ্রুৱ
  {
    TRACE_SUBSET (এতত্);
    auto *out = c->serializer->embed (*এতত্);
    যদি (unlikely (!out)) return_trace (মিছা);

    তার্কিক ret = out->baseTagList.serialize_copy (c->serializer, baseTagList, এতত্);
    ret |= out->baseScriptList.serialize_subset (c, baseScriptList, এতত্);
    return_trace (ret);
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (likely (c->check_struct (এতত্) &&
			  baseTagList.sanitize (c, এতত্) &&
			  baseScriptList.sanitize (c, এতত্)));
  }

  রক্ষিত:
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

  সার্ৱজনিক:
  DEFINE_SIZE_STATIC (4);
};

সংরচনা BASE
{
  স্থির constexpr hb_tag_t tableTag = HB_OT_TAG_BASE;

  ধ্রুৱ Axis &get_axis (hb_direction_t direction) ধ্রুৱ
  { ঘূৰাই HB_DIRECTION_IS_VERTICAL (direction) ? এতত্+vAxis : এতত্+hAxis; }

  তার্কিক has_var_store () ধ্রুৱ
  { ঘূৰাই version.to_int () >= 0x00010001u && varStore != 0; }

  ধ্রুৱ ItemVariationStore &get_var_store () ধ্রুৱ
  { ঘূৰাই version.to_int () < 0x00010001u ? Null (ItemVariationStore) : এতত্+varStore; }

  শূন্য collect_variation_indices (ধ্রুৱ hb_subset_plan_t* plan,
                                  hb_set_t& varidx_set /* OUT */) ধ্রুৱ
  {
    (এতত্+hAxis).collect_variation_indices (plan, varidx_set);
    (এতত্+vAxis).collect_variation_indices (plan, varidx_set);
  }

  তার্কিক subset_varstore (hb_subset_context_t *c,
                        BASE *out /* OUT */) ধ্রুৱ
  {
    TRACE_SUBSET (এতত্);
    যদি (!c->serializer->allocate_size<Offset32To<ItemVariationStore>> (Offset32To<ItemVariationStore>::static_size))
        return_trace (মিছা);
    যদি (!c->plan->normalized_coords)
      return_trace (out->varStore.serialize_subset (c, varStore, এতত্, c->plan->base_varstore_inner_maps.as_array ()));

    যদি (c->plan->all_axes_pinned)
      return_trace (সঁচা);

    item_variations_t item_vars;
    যদি (!item_vars.instantiate (এতত্+varStore, c->plan, সঁচা, সঁচা,
                                c->plan->base_varstore_inner_maps.as_array ()))
      return_trace (মিছা);

    যদি (!out->varStore.serialize_serialize (c->serializer,
                                            item_vars.has_long_word (),
                                            c->plan->axis_tags,
                                            item_vars.get_region_list (),
                                            item_vars.get_vardata_encodings ()))
      return_trace (মিছা);

    ধ্রুৱ hb_map_t &varidx_map = item_vars.get_varidx_map ();
    /* base_variation_idx_map in the plan is old_varidx->(varidx, delta)
     * mapping, new varidx is generated for subsetting, we need to remap this
     * after instancing */
    বাবে (auto _ : c->plan->base_variation_idx_map.iter_ref ())
    {
      uint32_t varidx = _.second.first;
      uint32_t *new_varidx;
      যদি (varidx_map.has (varidx, &new_varidx))
        _.second.first = *new_varidx;
      অন্যথা
        _.second.first = HB_OT_LAYOUT_NO_VARIATIONS_INDEX;
    }
    return_trace (সঁচা);
  }

  তার্কিক subset (hb_subset_context_t *c) ধ্রুৱ
  {
    TRACE_SUBSET (এতত্);
    auto *out = c->serializer->start_embed (*এতত্);
    যদি (unlikely (!out || !c->serializer->extend_min (out))) return_trace (মিছা);

    out->version = version;
    তার্কিক ret = মিছা;
    যদি (version.to_int () >= 0x00010001u) ret |= subset_varstore (c, out);

    ret |= out->hAxis.serialize_subset (c, hAxis, এতত্);
    ret |= out->vAxis.serialize_subset (c, vAxis, এতত্);

    return_trace (ret);
  }

  তার্কিক get_baseline (hb_font_t      *font,
		     hb_tag_t        baseline_tag,
		     hb_direction_t  direction,
		     hb_tag_t        script_tag,
		     hb_tag_t        language_tag,
		     hb_position_t  *base) ধ্রুৱ
  {
    ধ্রুৱ BaseCoord *base_coord = nullptr;
    যদি (unlikely (!get_axis (direction).get_baseline (baseline_tag, script_tag, language_tag, &base_coord) ||
		  !base_coord || !base_coord->has_data ()))
      ঘূৰাই মিছা;

    যদি (likely (base))
      *base = base_coord->get_coord (font, get_var_store (), direction);

    ঘূৰাই সঁচা;
  }

  তার্কিক get_min_max (hb_font_t      *font,
		    hb_direction_t  direction,
		    hb_tag_t        script_tag,
		    hb_tag_t        language_tag,
		    hb_tag_t        feature_tag,
		    hb_position_t  *min,
		    hb_position_t  *max) ধ্রুৱ
  {
    ধ্রুৱ BaseCoord *min_coord, *max_coord;
    যদি (!get_axis (direction).get_min_max (script_tag, language_tag, feature_tag,
					   &min_coord, &max_coord))
      ঘূৰাই মিছা;

    ধ্রুৱ ItemVariationStore &var_store = get_var_store ();
    যদি (likely (min && min_coord)) *min = min_coord->get_coord (font, var_store, direction);
    যদি (likely (max && max_coord)) *max = max_coord->get_coord (font, var_store, direction);
    ঘূৰাই সঁচা;
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (likely (c->check_struct (এতত্) &&
			  hb_barrier () &&
			  likely (version.major == 1) &&
			  hAxis.sanitize (c, এতত্) &&
			  vAxis.sanitize (c, এতত্) &&
			  (version.to_int () < 0x00010001u || varStore.sanitize (c, এতত্))));
  }

  রক্ষিত:
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
  সার্ৱজনিক:
  DEFINE_SIZE_MIN (8);
};


} /* namespace OT */


#endif /* HB_OT_LAYOUT_BASE_TABLE_HH */
