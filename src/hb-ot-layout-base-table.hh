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

#شامل "hb-open-type.hh"
#شامل "hb-ot-layout-common.hh"

فضاینام OT {

/*
 * BASE -- Baseline
 * https://docs.microsoft.com/en-us/typography/opentype/spec/base
 */

ساختار BaseCoordFormat1
{
  hb_position_t get_coord (hb_font_t *font, hb_direction_t direction) ثابت
  {
    بازگشت HB_DIRECTION_IS_HORIZONTAL (direction) ? font->em_scale_y (coordinate) : font->em_scale_x (coordinate);
  }

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (این);
    return_trace ((منطقی) c->serializer->embed (*این));
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این));
  }

  محافظتشده:
  HBUINT16	format;		/* Format identifier--format = 1 */
  FWORD		coordinate;	/* X or Y value, in design units */
  عمومی:
  DEFINE_SIZE_STATIC (4);
};

template <typename Types>
ساختار BaseCoordFormat2_4
{
  hb_position_t get_coord (hb_font_t *font, hb_direction_t direction) ثابت
  {
    /* TODO */
    بازگشت HB_DIRECTION_IS_HORIZONTAL (direction) ? font->em_scale_y (coordinate) : font->em_scale_x (coordinate);
  }

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (این);
    auto *out = c->serializer->embed (*این);
    اگر (unlikely (!out)) return_trace (نادرست);

    hb_codepoint_t *new_gid;
    اگر (!c->plan->glyph_map->has (referenceGlyph, &new_gid))
      return_trace (نادرست);

    return_trace (c->serializer->check_assign (out->referenceGlyph,
                                               *new_gid,
                                               HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این));
  }

  محافظتشده:
  HBUINT16	format;		/* Format identifier--format = 2 or 4 */
  FWORD		coordinate;	/* X or Y value, in design units */
  typename Types::HBGlyphID
		referenceGlyph;	/* Glyph ID of control glyph */
  HBUINT16	coordPoint;	/* Index of contour point on the
				 * reference glyph */
  عمومی:
  DEFINE_SIZE_STATIC (6 + Types::HBGlyphID::static_size);
};

ساختار BaseCoordFormat3
{
  hb_position_t get_coord (hb_font_t *font,
			   ثابت ItemVariationStore &var_store,
			   hb_direction_t direction) ثابت
  {
    ثابت Device &device = این+deviceTable;

    بازگشت HB_DIRECTION_IS_HORIZONTAL (direction)
	 ? hb_saturate_add (font->em_scale_y (coordinate), device.get_y_delta (font, var_store))
	 : hb_saturate_add (font->em_scale_x (coordinate), device.get_x_delta (font, var_store));
  }

  تهی collect_variation_indices (hb_set_t& varidx_set /* OUT */) ثابت
  {
    unsigned varidx = (این+deviceTable).get_variation_index ();
    varidx_set.add (varidx);
  }

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (این);
    auto *out = c->serializer->embed (*این);
    اگر (unlikely (!out)) return_trace (نادرست);

    اگر (!c->plan->pinned_at_default)
    {
      unsigned var_idx = (این+deviceTable).get_variation_index ();
      اگر (var_idx != VarIdx::NO_VARIATION)
      {
        hb_pair_t<unsigned, صحیح> *v;
        اگر (!c->plan->base_variation_idx_map.has (var_idx, &v))
          return_trace (نادرست);
        
        اگر (unlikely (!c->serializer->check_assign (out->coordinate, coordinate + hb_second (*v),
                                                    HB_SERIALIZE_ERROR_INT_OVERFLOW)))
          return_trace (نادرست);
      }
    }
    
    out->deviceTable.serialize_copy (c->serializer, deviceTable,
                                     این, 0,
                                     hb_serialize_context_t::Head,
                                     &c->plan->base_variation_idx_map);
    return_trace (درست);
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (likely (c->check_struct (این) &&
			  deviceTable.sanitize (c, این)));
  }

  محافظتشده:
  HBUINT16	format;		/* Format identifier--format = 3 */
  FWORD		coordinate;	/* X or Y value, in design units */
  Offset16To<Device>
		deviceTable;	/* Offset to Device table for X or
				 * Y value, from beginning of
				 * BaseCoord table (may be NULL). */
  عمومی:
  DEFINE_SIZE_STATIC (6);
};

ساختار BaseCoord
{
  منطقی has_data () ثابت { بازگشت u.format.v; }

  hb_position_t get_coord (hb_font_t            *font,
			   ثابت ItemVariationStore &var_store,
			   hb_direction_t        direction) ثابت
  {
    انتخاب (u.format.v) {
    حالت 1: hb_barrier (); بازگشت u.format1.get_coord (font, direction);
    حالت 2: hb_barrier (); بازگشت u.format2.get_coord (font, direction);
    حالت 3: hb_barrier (); بازگشت u.format3.get_coord (font, var_store, direction);
#ifndef HB_NO_BEYOND_64K
    حالت 4: hb_barrier (); بازگشت u.format4.get_coord (font, direction);
#endif
    پیشفرض:بازگشت 0;
    }
  }

  تهی collect_variation_indices (hb_set_t& varidx_set /* OUT */) ثابت
  {
    انتخاب (u.format.v) {
    حالت 3: hb_barrier (); u.format3.collect_variation_indices (varidx_set); بازگشت;
    پیشفرض:بازگشت;
    }
  }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    اگر (unlikely (!c->may_dispatch (این, &u.format.v))) بازگشت c->no_dispatch_return_مقدار ();
    TRACE_DISPATCH (این, u.format.v);
    انتخاب (u.format.v) {
    حالت 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    حالت 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    حالت 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
#ifndef HB_NO_BEYOND_64K
    حالت 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
#endif
    پیشفرض:return_trace (c->default_return_مقدار ());
    }
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    اگر (unlikely (!u.format.v.sanitize (c))) return_trace (نادرست);
    hb_barrier ();
    انتخاب (u.format.v) {
    حالت 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    حالت 2: hb_barrier (); return_trace (u.format2.sanitize (c));
    حالت 3: hb_barrier (); return_trace (u.format3.sanitize (c));
#ifndef HB_NO_BEYOND_64K
    حالت 4: hb_barrier (); return_trace (u.format4.sanitize (c));
#endif
    پیشفرض:return_trace (نادرست);
    }
  }

  محافظتشده:
  اجتماع {
  ساختار { HBUINT16 v; }	format;
  BaseCoordFormat1	format1;
  BaseCoordFormat2_4<Layout::SmallTypes>
			format2;
  BaseCoordFormat3	format3;
#ifndef HB_NO_BEYOND_64K
  BaseCoordFormat2_4<Layout::MediumTypes>
			format4;
#endif
  } u;
  عمومی:
  DEFINE_SIZE_UNION (2, format.v);
};

ساختار FeatMinMaxRecord
{
  صحیح cmp (hb_tag_t key) ثابت { بازگشت tag.cmp (key); }

  منطقی has_data () ثابت { بازگشت tag; }

  hb_tag_t get_feature_tag () ثابت { بازگشت tag; }

  تهی get_min_max (ثابت BaseCoord **min, ثابت BaseCoord **max) ثابت
  {
    اگر (likely (min)) *min = &(این+minCoord);
    اگر (likely (max)) *max = &(این+maxCoord);
  }

  تهی collect_variation_indices (ثابت hb_subset_plan_t* plan,
                                  ثابت تهی *base,
                                  hb_set_t& varidx_set /* OUT */) ثابت
  {
    اگر (!plan->layout_features.has (tag))
      بازگشت;

    (base+minCoord).collect_variation_indices (varidx_set);
    (base+maxCoord).collect_variation_indices (varidx_set);
  }

  منطقی subset (hb_subset_context_t *c,
               ثابت تهی *base) ثابت
  {
    TRACE_SUBSET (این);
    auto *out = c->serializer->embed (*این);
    اگر (unlikely (!out)) return_trace (نادرست);

    منطقی ret = out->minCoord.serialize_subset (c, minCoord, base);
    ret |= out->maxCoord.serialize_subset (c, maxCoord, base);
    return_trace (ret);
  }

  منطقی sanitize (hb_sanitize_context_t *c, ثابت تهی *base) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (likely (c->check_struct (این) &&
			  minCoord.sanitize (c, base) &&
			  maxCoord.sanitize (c, base)));
  }

  محافظتشده:
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
  عمومی:
  DEFINE_SIZE_STATIC (8);
};

ساختار MinMax
{
  تهی get_min_max (hb_tag_t          feature_tag,
		    ثابت BaseCoord **min,
		    ثابت BaseCoord **max) ثابت
  {
    ثابت FeatMinMaxRecord &minMaxCoord = featMinMaxRecords.bsearch (feature_tag);
    اگر (minMaxCoord.has_data ())
      minMaxCoord.get_min_max (min, max);
    وگرنه
    {
      اگر (likely (min)) *min = &(این+minCoord);
      اگر (likely (max)) *max = &(این+maxCoord);
    }
  }

  تهی collect_variation_indices (ثابت hb_subset_plan_t* plan,
                                  hb_set_t& varidx_set /* OUT */) ثابت
  {
    (این+minCoord).collect_variation_indices (varidx_set);
    (این+maxCoord).collect_variation_indices (varidx_set);
    برای (ثابت FeatMinMaxRecord& record : featMinMaxRecords)
      record.collect_variation_indices (plan, این, varidx_set);
  }

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (این);
    auto *out = c->serializer->start_embed (*این);
    اگر (unlikely (!out || !c->serializer->extend_min (out))) return_trace (نادرست);

    منطقی ret = out->minCoord.serialize_subset (c, minCoord, این);
    ret |= out->maxCoord.serialize_subset (c, maxCoord, این);

    unsigned len = 0;
    برای (ثابت FeatMinMaxRecord& _ : featMinMaxRecords)
    {
      hb_tag_t feature_tag = _.get_feature_tag ();
      اگر (!c->plan->layout_features.has (feature_tag))
        ادامه;

      auto snap = c->serializer->snapshot ();
      اگر (!_.subset (c, این)) c->serializer->revert (snap);
      وگرنه len++;
    }

    اگر (len > 0) ret = درست;
    اگر (!ret) return_trace (نادرست);
    return_trace (c->serializer->check_assign (out->featMinMaxRecords.len, len,
                                               HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (likely (c->check_struct (این) &&
			  minCoord.sanitize (c, این) &&
			  maxCoord.sanitize (c, این) &&
			  featMinMaxRecords.sanitize (c, این)));
  }

  محافظتشده:
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
  عمومی:
  DEFINE_SIZE_ARRAY (6, featMinMaxRecords);
};

ساختار Base_مقادیر
{
  ثابت BaseCoord &get_base_coord (صحیح baseline_tag_index) ثابت
  {
    اگر (baseline_tag_index == -1) baseline_tag_index = defaultIndex;
    بازگشت این+baseCoords[baseline_tag_index];
  }

  تهی collect_variation_indices (hb_set_t& varidx_set /* OUT */) ثابت
  {
    برای (ثابت auto& _ : baseCoords)
      (این+_).collect_variation_indices (varidx_set);
  }

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (این);
    auto *out = c->serializer->start_embed (*این);
    اگر (unlikely (!out || !c->serializer->extend_min (out))) return_trace (نادرست);
    out->defaultIndex = defaultIndex;

    منطقی ret = نادرست;
    برای (ثابت auto& _ : baseCoords)
    {
      auto *o = out->baseCoords.serialize_append (c->serializer);
      اگر (unlikely (!o)) return_trace (نادرست);
       ret |= o->serialize_subset (c, _, این);
    }

    return_trace (ret);
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (likely (c->check_struct (این) &&
			  baseCoords.sanitize (c, این)));
  }

  محافظتشده:
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
  عمومی:
  DEFINE_SIZE_ARRAY (4, baseCoords);
};

ساختار BaseLangSysRecord
{
  صحیح cmp (hb_tag_t key) ثابت { بازگشت baseLangSysTag.cmp (key); }

  منطقی has_data () ثابت { بازگشت baseLangSysTag; }

  ثابت MinMax &get_min_max (ثابت تهی* base) ثابت { بازگشت base+minMax; }

  تهی collect_variation_indices (ثابت تهی* base,
                                  ثابت hb_subset_plan_t* plan,
                                  hb_set_t& varidx_set /* OUT */) ثابت
  { (base+minMax).collect_variation_indices (plan, varidx_set); }

  منطقی subset (hb_subset_context_t *c,
               ثابت تهی *base) ثابت
  {
    TRACE_SUBSET (این);
    auto *out = c->serializer->embed (*این);
    اگر (unlikely (!out)) return_trace (نادرست);

    return_trace (out->minMax.serialize_subset (c, minMax, base));
  }

  منطقی sanitize (hb_sanitize_context_t *c, ثابت تهی *base) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (likely (c->check_struct (این) &&
			  minMax.sanitize (c, base)));
  }

  محافظتشده:
  Tag		baseLangSysTag;	/* 4-byte language system identification tag */
  Offset16To<MinMax>
		minMax;		/* Offset to MinMax table, from beginning
				 * of BaseScript table */
  عمومی:
  DEFINE_SIZE_STATIC (6);
};

ساختار BaseScript
{
  ثابت MinMax &get_min_max (hb_tag_t language_tag) ثابت
  {
    ثابت BaseLangSysRecord& record = baseLangSysRecords.bsearch (language_tag);
    بازگشت record.has_data () ? record.get_min_max (این) : این+defaultMinMax;
  }

  ثابت BaseCoord &get_base_coord (صحیح baseline_tag_index) ثابت
  { بازگشت (این+base_مقادیر).get_base_coord (baseline_tag_index); }

  منطقی has_مقادیر () ثابت { بازگشت base_مقادیر; }
  منطقی has_min_max () ثابت { بازگشت defaultMinMax || baseLangSysRecords; }

  تهی collect_variation_indices (ثابت hb_subset_plan_t* plan,
                                  hb_set_t& varidx_set /* OUT */) ثابت
  {
    (این+base_مقادیر).collect_variation_indices (varidx_set);
    (این+defaultMinMax).collect_variation_indices (plan, varidx_set);
    
    برای (ثابت BaseLangSysRecord& _ : baseLangSysRecords)
      _.collect_variation_indices (این, plan, varidx_set);
  }

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (این);
    auto *out = c->serializer->start_embed (*این);
    اگر (unlikely (!out || !c->serializer->extend_min (out))) return_trace (نادرست);

    منطقی ret = out->base_مقادیر.serialize_subset (c, base_مقادیر, این);
    ret |= out->defaultMinMax.serialize_subset (c, defaultMinMax, این);

    unsigned len = 0;
    برای (ثابت auto& _ : baseLangSysRecords)
    {
      auto snap = c->serializer->snapshot ();
      اگر (_.subset (c, این)) {
        ret = درست;
        len++;
      }
      وگرنه c->serializer->revert (snap);
    }

    اگر (!ret) return_trace (نادرست);
    return_trace (c->serializer->check_assign (out->baseLangSysRecords.len, len,
                                               HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (likely (c->check_struct (این) &&
			  base_مقادیر.sanitize (c, این) &&
			  defaultMinMax.sanitize (c, این) &&
			  baseLangSysRecords.sanitize (c, این)));
  }

  محافظتشده:
  Offset16To<Base_مقادیر>
		base_مقادیر;	/* Offset to BaseValues table, from beginning
				 * of BaseScript table (may be NULL) */
  Offset16To<MinMax>
		defaultMinMax;	/* Offset to MinMax table, from beginning of
				 * BaseScript table (may be NULL) */
  SortedArray16Of<BaseLangSysRecord>
		baseLangSysRecords;
				/* Number of BaseLangSysRecords
				 * defined — may be zero (0) */

  عمومی:
  DEFINE_SIZE_ARRAY (6, baseLangSysRecords);
};

ساختار BaseScriptList;
ساختار BaseScriptRecord
{
  صحیح cmp (hb_tag_t key) ثابت { بازگشت baseScriptTag.cmp (key); }

  منطقی has_data () ثابت { بازگشت baseScriptTag; }

  hb_tag_t get_script_tag () ثابت { بازگشت baseScriptTag; }

  ثابت BaseScript &get_base_script (ثابت BaseScriptList *list) ثابت
  { بازگشت list+baseScript; }

  تهی collect_variation_indices (ثابت hb_subset_plan_t* plan,
                                  ثابت تهی* list,
                                  hb_set_t& varidx_set /* OUT */) ثابت
  {
    اگر (!plan->layout_scripts.has (baseScriptTag))
      بازگشت;

    (list+baseScript).collect_variation_indices (plan, varidx_set);
  }

  منطقی subset (hb_subset_context_t *c,
               ثابت تهی *base) ثابت
  {
    TRACE_SUBSET (این);
    auto *out = c->serializer->embed (*این);
    اگر (unlikely (!out)) return_trace (نادرست);

    return_trace (out->baseScript.serialize_subset (c, baseScript, base));
  }

  منطقی sanitize (hb_sanitize_context_t *c, ثابت تهی *base) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (likely (c->check_struct (این) &&
			  baseScript.sanitize (c, base)));
  }

  محافظتشده:
  Tag		baseScriptTag;	/* 4-byte script identification tag */
  Offset16To<BaseScript>
		baseScript;	/* Offset to BaseScript table, from beginning
				 * of BaseScriptList */

  عمومی:
  DEFINE_SIZE_STATIC (6);
};

ساختار BaseScriptList
{
  ثابت BaseScript &get_base_script (hb_tag_t script) ثابت
  {
    ثابت BaseScriptRecord *record = &baseScriptRecords.bsearch (script);
    اگر (!record->has_data ()) record = &baseScriptRecords.bsearch (HB_TAG ('D','F','L','T'));
    بازگشت record->has_data () ? record->get_base_script (این) : Null (BaseScript);
  }

  تهی collect_variation_indices (ثابت hb_subset_plan_t* plan,
                                  hb_set_t& varidx_set /* OUT */) ثابت
  {
    برای (ثابت BaseScriptRecord& _ : baseScriptRecords)
      _.collect_variation_indices (plan, این, varidx_set);
  }

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (این);
    auto *out = c->serializer->start_embed (*این);
    اگر (unlikely (!out || !c->serializer->extend_min (out))) return_trace (نادرست);

    unsigned len = 0;
    برای (ثابت BaseScriptRecord& _ : baseScriptRecords)
    {
      hb_tag_t script_tag = _.get_script_tag ();
      اگر (!c->plan->layout_scripts.has (script_tag))
        ادامه;

      auto snap = c->serializer->snapshot ();
      اگر (!_.subset (c, این)) c->serializer->revert (snap);
      وگرنه len++;
    }

    اگر (!len) return_trace (نادرست);
    return_trace (c->serializer->check_assign (out->baseScriptRecords.len, len,
                                               HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این) &&
		  baseScriptRecords.sanitize (c, این));
  }

  محافظتشده:
  SortedArray16Of<BaseScriptRecord>
			baseScriptRecords;

  عمومی:
  DEFINE_SIZE_ARRAY (2, baseScriptRecords);
};

ساختار Axis
{
  منطقی get_baseline (hb_tag_t          baseline_tag,
		     hb_tag_t          script_tag,
		     hb_tag_t          language_tag,
		     ثابت BaseCoord **coord) ثابت
  {
    ثابت BaseScript &base_script = (این+baseScriptList).get_base_script (script_tag);
    اگر (!base_script.has_مقادیر ())
    {
      *coord = nullptr;
      بازگشت نادرست;
    }

    اگر (likely (coord))
    {
      unsigned صحیح tag_index = 0;
      اگر (!(این+baseTagList).bfind (baseline_tag, &tag_index))
      {
        *coord = nullptr;
        بازگشت نادرست;
      }
      *coord = &base_script.get_base_coord (tag_index);
    }

    بازگشت درست;
  }

  منطقی get_min_max (hb_tag_t          script_tag,
		    hb_tag_t          language_tag,
		    hb_tag_t          feature_tag,
		    ثابت BaseCoord **min_coord,
		    ثابت BaseCoord **max_coord) ثابت
  {
    ثابت BaseScript &base_script = (این+baseScriptList).get_base_script (script_tag);
    اگر (!base_script.has_min_max ())
    {
      *min_coord = *max_coord = nullptr;
      بازگشت نادرست;
    }

    base_script.get_min_max (language_tag).get_min_max (feature_tag, min_coord, max_coord);

    بازگشت درست;
  }

  تهی collect_variation_indices (ثابت hb_subset_plan_t* plan,
                                  hb_set_t& varidx_set /* OUT */) ثابت
  { (این+baseScriptList).collect_variation_indices (plan, varidx_set); }

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (این);
    auto *out = c->serializer->embed (*این);
    اگر (unlikely (!out)) return_trace (نادرست);

    منطقی ret = out->baseTagList.serialize_copy (c->serializer, baseTagList, این);
    ret |= out->baseScriptList.serialize_subset (c, baseScriptList, این);
    return_trace (ret);
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (likely (c->check_struct (این) &&
			  baseTagList.sanitize (c, این) &&
			  baseScriptList.sanitize (c, این)));
  }

  محافظتشده:
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

  عمومی:
  DEFINE_SIZE_STATIC (4);
};

ساختار BASE
{
  ایستا constexpr hb_tag_t tableTag = HB_OT_TAG_BASE;

  ثابت Axis &get_axis (hb_direction_t direction) ثابت
  { بازگشت HB_DIRECTION_IS_VERTICAL (direction) ? این+vAxis : این+hAxis; }

  منطقی has_var_store () ثابت
  { بازگشت version.to_int () >= 0x00010001u && varStore != 0; }

  ثابت ItemVariationStore &get_var_store () ثابت
  { بازگشت version.to_int () < 0x00010001u ? Null (ItemVariationStore) : این+varStore; }

  تهی collect_variation_indices (ثابت hb_subset_plan_t* plan,
                                  hb_set_t& varidx_set /* OUT */) ثابت
  {
    (این+hAxis).collect_variation_indices (plan, varidx_set);
    (این+vAxis).collect_variation_indices (plan, varidx_set);
  }

  منطقی subset_varstore (hb_subset_context_t *c,
                        BASE *out /* OUT */) ثابت
  {
    TRACE_SUBSET (این);
    اگر (!c->serializer->allocate_size<Offset32To<ItemVariationStore>> (Offset32To<ItemVariationStore>::static_size))
        return_trace (نادرست);
    اگر (!c->plan->normalized_coords)
      return_trace (out->varStore.serialize_subset (c, varStore, این, c->plan->base_varstore_inner_maps.as_array ()));

    اگر (c->plan->all_axes_pinned)
      return_trace (درست);

    item_variations_t item_vars;
    اگر (!item_vars.instantiate (این+varStore, c->plan, درست, درست,
                                c->plan->base_varstore_inner_maps.as_array ()))
      return_trace (نادرست);

    اگر (!out->varStore.serialize_serialize (c->serializer,
                                            item_vars.has_long_word (),
                                            c->plan->axis_tags,
                                            item_vars.get_region_list (),
                                            item_vars.get_vardata_encodings ()))
      return_trace (نادرست);

    ثابت hb_map_t &varidx_map = item_vars.get_varidx_map ();
    /* base_variation_idx_map in the plan is old_varidx->(varidx, delta)
     * mapping, new varidx is generated for subsetting, we need to remap this
     * after instancing */
    برای (auto _ : c->plan->base_variation_idx_map.iter_ref ())
    {
      uint32_t varidx = _.second.first;
      uint32_t *new_varidx;
      اگر (varidx_map.has (varidx, &new_varidx))
        _.second.first = *new_varidx;
      وگرنه
        _.second.first = HB_OT_LAYOUT_NO_VARIATIONS_INDEX;
    }
    return_trace (درست);
  }

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (این);
    auto *out = c->serializer->start_embed (*این);
    اگر (unlikely (!out || !c->serializer->extend_min (out))) return_trace (نادرست);

    out->version = version;
    منطقی ret = نادرست;
    اگر (version.to_int () >= 0x00010001u) ret |= subset_varstore (c, out);

    ret |= out->hAxis.serialize_subset (c, hAxis, این);
    ret |= out->vAxis.serialize_subset (c, vAxis, این);

    return_trace (ret);
  }

  منطقی get_baseline (hb_font_t      *font,
		     hb_tag_t        baseline_tag,
		     hb_direction_t  direction,
		     hb_tag_t        script_tag,
		     hb_tag_t        language_tag,
		     hb_position_t  *base) ثابت
  {
    ثابت BaseCoord *base_coord = nullptr;
    اگر (unlikely (!get_axis (direction).get_baseline (baseline_tag, script_tag, language_tag, &base_coord) ||
		  !base_coord || !base_coord->has_data ()))
      بازگشت نادرست;

    اگر (likely (base))
      *base = base_coord->get_coord (font, get_var_store (), direction);

    بازگشت درست;
  }

  منطقی get_min_max (hb_font_t      *font,
		    hb_direction_t  direction,
		    hb_tag_t        script_tag,
		    hb_tag_t        language_tag,
		    hb_tag_t        feature_tag,
		    hb_position_t  *min,
		    hb_position_t  *max) ثابت
  {
    ثابت BaseCoord *min_coord, *max_coord;
    اگر (!get_axis (direction).get_min_max (script_tag, language_tag, feature_tag,
					   &min_coord, &max_coord))
      بازگشت نادرست;

    ثابت ItemVariationStore &var_store = get_var_store ();
    اگر (likely (min && min_coord)) *min = min_coord->get_coord (font, var_store, direction);
    اگر (likely (max && max_coord)) *max = max_coord->get_coord (font, var_store, direction);
    بازگشت درست;
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (likely (c->check_struct (این) &&
			  hb_barrier () &&
			  likely (version.major == 1) &&
			  hAxis.sanitize (c, این) &&
			  vAxis.sanitize (c, این) &&
			  (version.to_int () < 0x00010001u || varStore.sanitize (c, این))));
  }

  محافظتشده:
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
  عمومی:
  DEFINE_SIZE_MIN (8);
};


} /* namespace OT */


#endif /* HB_OT_LAYOUT_BASE_TABLE_HH */
