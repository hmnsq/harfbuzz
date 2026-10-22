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

#ضمن "hb-open-type.hh"
#ضمن "hb-ot-layout-common.hh"

نطاق OT {

/*
 * BASE -- Baseline
 * https://docs.microsoft.com/en-us/typography/opentype/spec/base
 */

بنية BaseCoordFormat1
{
  hb_position_t get_coord (hb_font_t *font, hb_direction_t direction) ثابت
  {
    أرجع HB_DIRECTION_IS_HORIZONTAL (direction) ? font->em_scale_y (coordinate) : font->em_scale_x (coordinate);
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    return_trace ((منطقي) c->serializer->embed (*هذا));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  محمي:
  HBUINT16	format;		/* Format identifier--format = 1 */
  FWORD		coordinate;	/* X or Y value, in design units */
  عام:
  DEFINE_SIZE_STATIC (4);
};

template <typename Types>
بنية BaseCoordFormat2_4
{
  hb_position_t get_coord (hb_font_t *font, hb_direction_t direction) ثابت
  {
    /* TODO */
    أرجع HB_DIRECTION_IS_HORIZONTAL (direction) ? font->em_scale_y (coordinate) : font->em_scale_x (coordinate);
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    auto *out = c->serializer->embed (*هذا);
    𐡄𐡍 (unlikely (!out)) return_trace (خطأ);

    hb_codepoint_t *new_gid;
    𐡄𐡍 (!c->plan->glyph_map->has (referenceGlyph, &new_gid))
      return_trace (خطأ);

    return_trace (c->serializer->check_assign (out->referenceGlyph,
                                               *new_gid,
                                               HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  محمي:
  HBUINT16	format;		/* Format identifier--format = 2 or 4 */
  FWORD		coordinate;	/* X or Y value, in design units */
  typename Types::HBGlyphID
		referenceGlyph;	/* Glyph ID of control glyph */
  HBUINT16	coordPoint;	/* Index of contour point on the
				 * reference glyph */
  عام:
  DEFINE_SIZE_STATIC (6 + Types::HBGlyphID::static_size);
};

بنية BaseCoordFormat3
{
  hb_position_t get_coord (hb_font_t *font,
			   ثابت ItemVariationStore &var_store,
			   hb_direction_t direction) ثابت
  {
    ثابت Device &device = هذا+deviceTable;

    أرجع HB_DIRECTION_IS_HORIZONTAL (direction)
	 ? hb_saturate_add (font->em_scale_y (coordinate), device.get_y_delta (font, var_store))
	 : hb_saturate_add (font->em_scale_x (coordinate), device.get_x_delta (font, var_store));
  }

  فراغ collect_variation_indices (hb_set_t& varidx_set /* OUT */) ثابت
  {
    unsigned varidx = (هذا+deviceTable).get_variation_index ();
    varidx_set.add (varidx);
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    auto *out = c->serializer->embed (*هذا);
    𐡄𐡍 (unlikely (!out)) return_trace (خطأ);

    𐡄𐡍 (!c->plan->pinned_at_default)
    {
      unsigned var_idx = (هذا+deviceTable).get_variation_index ();
      𐡄𐡍 (var_idx != VarIdx::NO_VARIATION)
      {
        hb_pair_t<unsigned, صحيح> *v;
        𐡄𐡍 (!c->plan->base_variation_idx_map.has (var_idx, &v))
          return_trace (خطأ);
        
        𐡄𐡍 (unlikely (!c->serializer->check_assign (out->coordinate, coordinate + hb_second (*v),
                                                    HB_SERIALIZE_ERROR_INT_OVERFLOW)))
          return_trace (خطأ);
      }
    }
    
    out->deviceTable.serialize_copy (c->serializer, deviceTable,
                                     هذا, 0,
                                     hb_serialize_context_t::Head,
                                     &c->plan->base_variation_idx_map);
    return_trace (𐡒𐡔𐡈);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (c->check_struct (هذا) &&
			  deviceTable.sanitize (c, هذا)));
  }

  محمي:
  HBUINT16	format;		/* Format identifier--format = 3 */
  FWORD		coordinate;	/* X or Y value, in design units */
  Offset16To<Device>
		deviceTable;	/* Offset to Device table for X or
				 * Y value, from beginning of
				 * BaseCoord table (may be NULL). */
  عام:
  DEFINE_SIZE_STATIC (6);
};

بنية BaseCoord
{
  منطقي has_data () ثابت { أرجع u.format.v; }

  hb_position_t get_coord (hb_font_t            *font,
			   ثابت ItemVariationStore &var_store,
			   hb_direction_t        direction) ثابت
  {
    بدل (u.format.v) {
    حالة 1: hb_barrier (); أرجع u.format1.get_coord (font, direction);
    حالة 2: hb_barrier (); أرجع u.format2.get_coord (font, direction);
    حالة 3: hb_barrier (); أرجع u.format3.get_coord (font, var_store, direction);
#ifndef HB_NO_BEYOND_64K
    حالة 4: hb_barrier (); أرجع u.format4.get_coord (font, direction);
#endif
    افتراضي:أرجع 0;
    }
  }

  فراغ collect_variation_indices (hb_set_t& varidx_set /* OUT */) ثابت
  {
    بدل (u.format.v) {
    حالة 3: hb_barrier (); u.format3.collect_variation_indices (varidx_set); أرجع;
    افتراضي:أرجع;
    }
  }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    𐡄𐡍 (unlikely (!c->may_dispatch (هذا, &u.format.v))) أرجع c->no_dispatch_return_value ();
    TRACE_DISPATCH (هذا, u.format.v);
    بدل (u.format.v) {
    حالة 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    حالة 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    حالة 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
#ifndef HB_NO_BEYOND_64K
    حالة 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
#endif
    افتراضي:return_trace (c->default_return_value ());
    }
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    𐡄𐡍 (unlikely (!u.format.v.sanitize (c))) return_trace (خطأ);
    hb_barrier ();
    بدل (u.format.v) {
    حالة 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    حالة 2: hb_barrier (); return_trace (u.format2.sanitize (c));
    حالة 3: hb_barrier (); return_trace (u.format3.sanitize (c));
#ifndef HB_NO_BEYOND_64K
    حالة 4: hb_barrier (); return_trace (u.format4.sanitize (c));
#endif
    افتراضي:return_trace (خطأ);
    }
  }

  محمي:
  اتحاد {
  بنية { HBUINT16 v; }	format;
  BaseCoordFormat1	format1;
  BaseCoordFormat2_4<Layout::SmallTypes>
			format2;
  BaseCoordFormat3	format3;
#ifndef HB_NO_BEYOND_64K
  BaseCoordFormat2_4<Layout::MediumTypes>
			format4;
#endif
  } u;
  عام:
  DEFINE_SIZE_UNION (2, format.v);
};

بنية FeatMinMaxRecord
{
  صحيح cmp (hb_tag_t key) ثابت { أرجع tag.cmp (key); }

  منطقي has_data () ثابت { أرجع tag; }

  hb_tag_t get_feature_tag () ثابت { أرجع tag; }

  فراغ get_min_max (ثابت BaseCoord **min, ثابت BaseCoord **max) ثابت
  {
    𐡄𐡍 (likely (min)) *min = &(هذا+minCoord);
    𐡄𐡍 (likely (max)) *max = &(هذا+maxCoord);
  }

  فراغ collect_variation_indices (ثابت hb_subset_plan_t* plan,
                                  ثابت فراغ *base,
                                  hb_set_t& varidx_set /* OUT */) ثابت
  {
    𐡄𐡍 (!plan->layout_features.has (tag))
      أرجع;

    (base+minCoord).collect_variation_indices (varidx_set);
    (base+maxCoord).collect_variation_indices (varidx_set);
  }

  منطقي subset (hb_subset_context_t *c,
               ثابت فراغ *base) ثابت
  {
    TRACE_SUBSET (هذا);
    auto *out = c->serializer->embed (*هذا);
    𐡄𐡍 (unlikely (!out)) return_trace (خطأ);

    منطقي ret = out->minCoord.serialize_subset (c, minCoord, base);
    ret |= out->maxCoord.serialize_subset (c, maxCoord, base);
    return_trace (ret);
  }

  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *base) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (c->check_struct (هذا) &&
			  minCoord.sanitize (c, base) &&
			  maxCoord.sanitize (c, base)));
  }

  محمي:
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
  عام:
  DEFINE_SIZE_STATIC (8);
};

بنية MinMax
{
  فراغ get_min_max (hb_tag_t          feature_tag,
		    ثابت BaseCoord **min,
		    ثابت BaseCoord **max) ثابت
  {
    ثابت FeatMinMaxRecord &minMaxCoord = featMinMaxRecords.bsearch (feature_tag);
    𐡄𐡍 (minMaxCoord.has_data ())
      minMaxCoord.get_min_max (min, max);
    وإلا
    {
      𐡄𐡍 (likely (min)) *min = &(هذا+minCoord);
      𐡄𐡍 (likely (max)) *max = &(هذا+maxCoord);
    }
  }

  فراغ collect_variation_indices (ثابت hb_subset_plan_t* plan,
                                  hb_set_t& varidx_set /* OUT */) ثابت
  {
    (هذا+minCoord).collect_variation_indices (varidx_set);
    (هذا+maxCoord).collect_variation_indices (varidx_set);
    لكل (ثابت FeatMinMaxRecord& record : featMinMaxRecords)
      record.collect_variation_indices (plan, هذا, varidx_set);
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    auto *out = c->serializer->start_embed (*هذا);
    𐡄𐡍 (unlikely (!out || !c->serializer->extend_min (out))) return_trace (خطأ);

    منطقي ret = out->minCoord.serialize_subset (c, minCoord, هذا);
    ret |= out->maxCoord.serialize_subset (c, maxCoord, هذا);

    unsigned len = 0;
    لكل (ثابت FeatMinMaxRecord& _ : featMinMaxRecords)
    {
      hb_tag_t feature_tag = _.get_feature_tag ();
      𐡄𐡍 (!c->plan->layout_features.has (feature_tag))
        استمر;

      auto snap = c->serializer->snapshot ();
      𐡄𐡍 (!_.subset (c, هذا)) c->serializer->revert (snap);
      وإلا len++;
    }

    𐡄𐡍 (len > 0) ret = 𐡒𐡔𐡈;
    𐡄𐡍 (!ret) return_trace (خطأ);
    return_trace (c->serializer->check_assign (out->featMinMaxRecords.len, len,
                                               HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (c->check_struct (هذا) &&
			  minCoord.sanitize (c, هذا) &&
			  maxCoord.sanitize (c, هذا) &&
			  featMinMaxRecords.sanitize (c, هذا)));
  }

  محمي:
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
  عام:
  DEFINE_SIZE_ARRAY (6, featMinMaxRecords);
};

بنية BaseValues
{
  ثابت BaseCoord &get_base_coord (صحيح baseline_tag_index) ثابت
  {
    𐡄𐡍 (baseline_tag_index == -1) baseline_tag_index = defaultIndex;
    أرجع هذا+baseCoords[baseline_tag_index];
  }

  فراغ collect_variation_indices (hb_set_t& varidx_set /* OUT */) ثابت
  {
    لكل (ثابت auto& _ : baseCoords)
      (هذا+_).collect_variation_indices (varidx_set);
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    auto *out = c->serializer->start_embed (*هذا);
    𐡄𐡍 (unlikely (!out || !c->serializer->extend_min (out))) return_trace (خطأ);
    out->defaultIndex = defaultIndex;

    منطقي ret = خطأ;
    لكل (ثابت auto& _ : baseCoords)
    {
      auto *o = out->baseCoords.serialize_append (c->serializer);
      𐡄𐡍 (unlikely (!o)) return_trace (خطأ);
       ret |= o->serialize_subset (c, _, هذا);
    }

    return_trace (ret);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (c->check_struct (هذا) &&
			  baseCoords.sanitize (c, هذا)));
  }

  محمي:
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
  عام:
  DEFINE_SIZE_ARRAY (4, baseCoords);
};

بنية BaseLangSysRecord
{
  صحيح cmp (hb_tag_t key) ثابت { أرجع baseLangSysTag.cmp (key); }

  منطقي has_data () ثابت { أرجع baseLangSysTag; }

  ثابت MinMax &get_min_max (ثابت فراغ* base) ثابت { أرجع base+minMax; }

  فراغ collect_variation_indices (ثابت فراغ* base,
                                  ثابت hb_subset_plan_t* plan,
                                  hb_set_t& varidx_set /* OUT */) ثابت
  { (base+minMax).collect_variation_indices (plan, varidx_set); }

  منطقي subset (hb_subset_context_t *c,
               ثابت فراغ *base) ثابت
  {
    TRACE_SUBSET (هذا);
    auto *out = c->serializer->embed (*هذا);
    𐡄𐡍 (unlikely (!out)) return_trace (خطأ);

    return_trace (out->minMax.serialize_subset (c, minMax, base));
  }

  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *base) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (c->check_struct (هذا) &&
			  minMax.sanitize (c, base)));
  }

  محمي:
  Tag		baseLangSysTag;	/* 4-byte language system identification tag */
  Offset16To<MinMax>
		minMax;		/* Offset to MinMax table, from beginning
				 * of BaseScript table */
  عام:
  DEFINE_SIZE_STATIC (6);
};

بنية BaseScript
{
  ثابت MinMax &get_min_max (hb_tag_t language_tag) ثابت
  {
    ثابت BaseLangSysRecord& record = baseLangSysRecords.bsearch (language_tag);
    أرجع record.has_data () ? record.get_min_max (هذا) : هذا+defaultMinMax;
  }

  ثابت BaseCoord &get_base_coord (صحيح baseline_tag_index) ثابت
  { أرجع (هذا+baseValues).get_base_coord (baseline_tag_index); }

  منطقي has_values () ثابت { أرجع baseValues; }
  منطقي has_min_max () ثابت { أرجع defaultMinMax || baseLangSysRecords; }

  فراغ collect_variation_indices (ثابت hb_subset_plan_t* plan,
                                  hb_set_t& varidx_set /* OUT */) ثابت
  {
    (هذا+baseValues).collect_variation_indices (varidx_set);
    (هذا+defaultMinMax).collect_variation_indices (plan, varidx_set);
    
    لكل (ثابت BaseLangSysRecord& _ : baseLangSysRecords)
      _.collect_variation_indices (هذا, plan, varidx_set);
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    auto *out = c->serializer->start_embed (*هذا);
    𐡄𐡍 (unlikely (!out || !c->serializer->extend_min (out))) return_trace (خطأ);

    منطقي ret = out->baseValues.serialize_subset (c, baseValues, هذا);
    ret |= out->defaultMinMax.serialize_subset (c, defaultMinMax, هذا);

    unsigned len = 0;
    لكل (ثابت auto& _ : baseLangSysRecords)
    {
      auto snap = c->serializer->snapshot ();
      𐡄𐡍 (_.subset (c, هذا)) {
        ret = 𐡒𐡔𐡈;
        len++;
      }
      وإلا c->serializer->revert (snap);
    }

    𐡄𐡍 (!ret) return_trace (خطأ);
    return_trace (c->serializer->check_assign (out->baseLangSysRecords.len, len,
                                               HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (c->check_struct (هذا) &&
			  baseValues.sanitize (c, هذا) &&
			  defaultMinMax.sanitize (c, هذا) &&
			  baseLangSysRecords.sanitize (c, هذا)));
  }

  محمي:
  Offset16To<BaseValues>
		baseValues;	/* Offset to BaseValues table, from beginning
				 * of BaseScript table (may be NULL) */
  Offset16To<MinMax>
		defaultMinMax;	/* Offset to MinMax table, from beginning of
				 * BaseScript table (may be NULL) */
  SortedArray16Of<BaseLangSysRecord>
		baseLangSysRecords;
				/* Number of BaseLangSysRecords
				 * defined — may be zero (0) */

  عام:
  DEFINE_SIZE_ARRAY (6, baseLangSysRecords);
};

بنية BaseScriptList;
بنية BaseScriptRecord
{
  صحيح cmp (hb_tag_t key) ثابت { أرجع baseScriptTag.cmp (key); }

  منطقي has_data () ثابت { أرجع baseScriptTag; }

  hb_tag_t get_script_tag () ثابت { أرجع baseScriptTag; }

  ثابت BaseScript &get_base_script (ثابت BaseScriptList *list) ثابت
  { أرجع list+baseScript; }

  فراغ collect_variation_indices (ثابت hb_subset_plan_t* plan,
                                  ثابت فراغ* list,
                                  hb_set_t& varidx_set /* OUT */) ثابت
  {
    𐡄𐡍 (!plan->layout_scripts.has (baseScriptTag))
      أرجع;

    (list+baseScript).collect_variation_indices (plan, varidx_set);
  }

  منطقي subset (hb_subset_context_t *c,
               ثابت فراغ *base) ثابت
  {
    TRACE_SUBSET (هذا);
    auto *out = c->serializer->embed (*هذا);
    𐡄𐡍 (unlikely (!out)) return_trace (خطأ);

    return_trace (out->baseScript.serialize_subset (c, baseScript, base));
  }

  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *base) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (c->check_struct (هذا) &&
			  baseScript.sanitize (c, base)));
  }

  محمي:
  Tag		baseScriptTag;	/* 4-byte script identification tag */
  Offset16To<BaseScript>
		baseScript;	/* Offset to BaseScript table, from beginning
				 * of BaseScriptList */

  عام:
  DEFINE_SIZE_STATIC (6);
};

بنية BaseScriptList
{
  ثابت BaseScript &get_base_script (hb_tag_t script) ثابت
  {
    ثابت BaseScriptRecord *record = &baseScriptRecords.bsearch (script);
    𐡄𐡍 (!record->has_data ()) record = &baseScriptRecords.bsearch (HB_TAG ('D','F','L','T'));
    أرجع record->has_data () ? record->get_base_script (هذا) : Null (BaseScript);
  }

  فراغ collect_variation_indices (ثابت hb_subset_plan_t* plan,
                                  hb_set_t& varidx_set /* OUT */) ثابت
  {
    لكل (ثابت BaseScriptRecord& _ : baseScriptRecords)
      _.collect_variation_indices (plan, هذا, varidx_set);
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    auto *out = c->serializer->start_embed (*هذا);
    𐡄𐡍 (unlikely (!out || !c->serializer->extend_min (out))) return_trace (خطأ);

    unsigned len = 0;
    لكل (ثابت BaseScriptRecord& _ : baseScriptRecords)
    {
      hb_tag_t script_tag = _.get_script_tag ();
      𐡄𐡍 (!c->plan->layout_scripts.has (script_tag))
        استمر;

      auto snap = c->serializer->snapshot ();
      𐡄𐡍 (!_.subset (c, هذا)) c->serializer->revert (snap);
      وإلا len++;
    }

    𐡄𐡍 (!len) return_trace (خطأ);
    return_trace (c->serializer->check_assign (out->baseScriptRecords.len, len,
                                               HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  baseScriptRecords.sanitize (c, هذا));
  }

  محمي:
  SortedArray16Of<BaseScriptRecord>
			baseScriptRecords;

  عام:
  DEFINE_SIZE_ARRAY (2, baseScriptRecords);
};

بنية Axis
{
  منطقي get_baseline (hb_tag_t          baseline_tag,
		     hb_tag_t          script_tag,
		     hb_tag_t          language_tag,
		     ثابت BaseCoord **coord) ثابت
  {
    ثابت BaseScript &base_script = (هذا+baseScriptList).get_base_script (script_tag);
    𐡄𐡍 (!base_script.has_values ())
    {
      *coord = nullptr;
      أرجع خطأ;
    }

    𐡄𐡍 (likely (coord))
    {
      unsigned صحيح tag_index = 0;
      𐡄𐡍 (!(هذا+baseTagList).bfind (baseline_tag, &tag_index))
      {
        *coord = nullptr;
        أرجع خطأ;
      }
      *coord = &base_script.get_base_coord (tag_index);
    }

    أرجع 𐡒𐡔𐡈;
  }

  منطقي get_min_max (hb_tag_t          script_tag,
		    hb_tag_t          language_tag,
		    hb_tag_t          feature_tag,
		    ثابت BaseCoord **min_coord,
		    ثابت BaseCoord **max_coord) ثابت
  {
    ثابت BaseScript &base_script = (هذا+baseScriptList).get_base_script (script_tag);
    𐡄𐡍 (!base_script.has_min_max ())
    {
      *min_coord = *max_coord = nullptr;
      أرجع خطأ;
    }

    base_script.get_min_max (language_tag).get_min_max (feature_tag, min_coord, max_coord);

    أرجع 𐡒𐡔𐡈;
  }

  فراغ collect_variation_indices (ثابت hb_subset_plan_t* plan,
                                  hb_set_t& varidx_set /* OUT */) ثابت
  { (هذا+baseScriptList).collect_variation_indices (plan, varidx_set); }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    auto *out = c->serializer->embed (*هذا);
    𐡄𐡍 (unlikely (!out)) return_trace (خطأ);

    منطقي ret = out->baseTagList.serialize_copy (c->serializer, baseTagList, هذا);
    ret |= out->baseScriptList.serialize_subset (c, baseScriptList, هذا);
    return_trace (ret);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (c->check_struct (هذا) &&
			  baseTagList.sanitize (c, هذا) &&
			  baseScriptList.sanitize (c, هذا)));
  }

  محمي:
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

  عام:
  DEFINE_SIZE_STATIC (4);
};

بنية BASE
{
  ساكن constexpr hb_tag_t tableTag = HB_OT_TAG_BASE;

  ثابت Axis &get_axis (hb_direction_t direction) ثابت
  { أرجع HB_DIRECTION_IS_VERTICAL (direction) ? هذا+vAxis : هذا+hAxis; }

  منطقي has_var_store () ثابت
  { أرجع version.to_int () >= 0x00010001u && varStore != 0; }

  ثابت ItemVariationStore &get_var_store () ثابت
  { أرجع version.to_int () < 0x00010001u ? Null (ItemVariationStore) : هذا+varStore; }

  فراغ collect_variation_indices (ثابت hb_subset_plan_t* plan,
                                  hb_set_t& varidx_set /* OUT */) ثابت
  {
    (هذا+hAxis).collect_variation_indices (plan, varidx_set);
    (هذا+vAxis).collect_variation_indices (plan, varidx_set);
  }

  منطقي subset_varstore (hb_subset_context_t *c,
                        BASE *out /* OUT */) ثابت
  {
    TRACE_SUBSET (هذا);
    𐡄𐡍 (!c->serializer->allocate_size<Offset32To<ItemVariationStore>> (Offset32To<ItemVariationStore>::static_size))
        return_trace (خطأ);
    𐡄𐡍 (!c->plan->normalized_coords)
      return_trace (out->varStore.serialize_subset (c, varStore, هذا, c->plan->base_varstore_inner_maps.as_array ()));

    𐡄𐡍 (c->plan->all_axes_pinned)
      return_trace (𐡒𐡔𐡈);

    item_variations_t item_vars;
    𐡄𐡍 (!item_vars.instantiate (هذا+varStore, c->plan, 𐡒𐡔𐡈, 𐡒𐡔𐡈,
                                c->plan->base_varstore_inner_maps.as_array ()))
      return_trace (خطأ);

    𐡄𐡍 (!out->varStore.serialize_serialize (c->serializer,
                                            item_vars.has_long_word (),
                                            c->plan->axis_tags,
                                            item_vars.get_region_list (),
                                            item_vars.get_vardata_encodings ()))
      return_trace (خطأ);

    ثابت hb_map_t &varidx_map = item_vars.get_varidx_map ();
    /* base_variation_idx_map in the plan is old_varidx->(varidx, delta)
     * mapping, new varidx is generated for subsetting, we need to remap this
     * after instancing */
    لكل (auto _ : c->plan->base_variation_idx_map.iter_ref ())
    {
      uint32_t varidx = _.second.first;
      uint32_t *new_varidx;
      𐡄𐡍 (varidx_map.has (varidx, &new_varidx))
        _.second.first = *new_varidx;
      وإلا
        _.second.first = HB_OT_LAYOUT_NO_VARIATIONS_INDEX;
    }
    return_trace (𐡒𐡔𐡈);
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    auto *out = c->serializer->start_embed (*هذا);
    𐡄𐡍 (unlikely (!out || !c->serializer->extend_min (out))) return_trace (خطأ);

    out->version = version;
    منطقي ret = خطأ;
    𐡄𐡍 (version.to_int () >= 0x00010001u) ret |= subset_varstore (c, out);

    ret |= out->hAxis.serialize_subset (c, hAxis, هذا);
    ret |= out->vAxis.serialize_subset (c, vAxis, هذا);

    return_trace (ret);
  }

  منطقي get_baseline (hb_font_t      *font,
		     hb_tag_t        baseline_tag,
		     hb_direction_t  direction,
		     hb_tag_t        script_tag,
		     hb_tag_t        language_tag,
		     hb_position_t  *base) ثابت
  {
    ثابت BaseCoord *base_coord = nullptr;
    𐡄𐡍 (unlikely (!get_axis (direction).get_baseline (baseline_tag, script_tag, language_tag, &base_coord) ||
		  !base_coord || !base_coord->has_data ()))
      أرجع خطأ;

    𐡄𐡍 (likely (base))
      *base = base_coord->get_coord (font, get_var_store (), direction);

    أرجع 𐡒𐡔𐡈;
  }

  منطقي get_min_max (hb_font_t      *font,
		    hb_direction_t  direction,
		    hb_tag_t        script_tag,
		    hb_tag_t        language_tag,
		    hb_tag_t        feature_tag,
		    hb_position_t  *min,
		    hb_position_t  *max) ثابت
  {
    ثابت BaseCoord *min_coord, *max_coord;
    𐡄𐡍 (!get_axis (direction).get_min_max (script_tag, language_tag, feature_tag,
					   &min_coord, &max_coord))
      أرجع خطأ;

    ثابت ItemVariationStore &var_store = get_var_store ();
    𐡄𐡍 (likely (min && min_coord)) *min = min_coord->get_coord (font, var_store, direction);
    𐡄𐡍 (likely (max && max_coord)) *max = max_coord->get_coord (font, var_store, direction);
    أرجع 𐡒𐡔𐡈;
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (c->check_struct (هذا) &&
			  hb_barrier () &&
			  likely (version.major == 1) &&
			  hAxis.sanitize (c, هذا) &&
			  vAxis.sanitize (c, هذا) &&
			  (version.to_int () < 0x00010001u || varStore.sanitize (c, هذا))));
  }

  محمي:
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
  عام:
  DEFINE_SIZE_MIN (8);
};


} /* namespace OT */


#endif /* HB_OT_LAYOUT_BASE_TABLE_HH */
