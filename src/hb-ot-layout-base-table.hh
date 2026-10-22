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

#அந்தர்பாவ "hb-open-type.hh"
#அந்தர்பாவ "hb-ot-layout-common.hh"

நாமாகாஶ OT {

/*
 * BASE -- Baseline
 * https://docs.microsoft.com/en-us/typography/opentype/spec/base
 */

கட்டமைப்பு BaseCoordFormat1
{
  hb_position_t get_coord (hb_font_t *font, hb_direction_t direction) மாறிலி
  {
    திரும்பு HB_DIRECTION_IS_HORIZONTAL (direction) ? font->em_scale_y (coordinate) : font->em_scale_x (coordinate);
  }

  தருக்க subset (hb_subset_context_t *c) மாறிலி
  {
    TRACE_SUBSET (இது);
    return_trace ((தருக்க) c->serializer->embed (*இது));
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது));
  }

  பாதுகாக்கப்பட்ட:
  HBUINT16	format;		/* Format identifier--format = 1 */
  FWORD		coordinate;	/* X or Y value, in design units */
  பொது:
  DEFINE_SIZE_STATIC (4);
};

template <typename Types>
கட்டமைப்பு BaseCoordFormat2_4
{
  hb_position_t get_coord (hb_font_t *font, hb_direction_t direction) மாறிலி
  {
    /* TODO */
    திரும்பு HB_DIRECTION_IS_HORIZONTAL (direction) ? font->em_scale_y (coordinate) : font->em_scale_x (coordinate);
  }

  தருக்க subset (hb_subset_context_t *c) மாறிலி
  {
    TRACE_SUBSET (இது);
    auto *out = c->serializer->embed (*இது);
    எனில் (unlikely (!out)) return_trace (பொய்);

    hb_codepoint_t *new_gid;
    எனில் (!c->plan->glyph_map->has (referenceGlyph, &new_gid))
      return_trace (பொய்);

    return_trace (c->serializer->check_assign (out->referenceGlyph,
                                               *new_gid,
                                               HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது));
  }

  பாதுகாக்கப்பட்ட:
  HBUINT16	format;		/* Format identifier--format = 2 or 4 */
  FWORD		coordinate;	/* X or Y value, in design units */
  typename Types::HBGlyphID
		referenceGlyph;	/* Glyph ID of control glyph */
  HBUINT16	coordPoint;	/* Index of contour point on the
				 * reference glyph */
  பொது:
  DEFINE_SIZE_STATIC (6 + Types::HBGlyphID::static_size);
};

கட்டமைப்பு BaseCoordFormat3
{
  hb_position_t get_coord (hb_font_t *font,
			   மாறிலி ItemVariationStore &var_store,
			   hb_direction_t direction) மாறிலி
  {
    மாறிலி Device &device = இது+deviceTable;

    திரும்பு HB_DIRECTION_IS_HORIZONTAL (direction)
	 ? hb_saturate_add (font->em_scale_y (coordinate), device.get_y_delta (font, var_store))
	 : hb_saturate_add (font->em_scale_x (coordinate), device.get_x_delta (font, var_store));
  }

  வெறுமை collect_variation_indices (hb_set_t& varidx_set /* OUT */) மாறிலி
  {
    unsigned varidx = (இது+deviceTable).get_variation_index ();
    varidx_set.add (varidx);
  }

  தருக்க subset (hb_subset_context_t *c) மாறிலி
  {
    TRACE_SUBSET (இது);
    auto *out = c->serializer->embed (*இது);
    எனில் (unlikely (!out)) return_trace (பொய்);

    எனில் (!c->plan->pinned_at_default)
    {
      unsigned var_idx = (இது+deviceTable).get_variation_index ();
      எனில் (var_idx != VarIdx::NO_VARIATION)
      {
        hb_pair_t<unsigned, முழு> *v;
        எனில் (!c->plan->base_variation_idx_map.has (var_idx, &v))
          return_trace (பொய்);
        
        எனில் (unlikely (!c->serializer->check_assign (out->coordinate, coordinate + hb_second (*v),
                                                    HB_SERIALIZE_ERROR_INT_OVERFLOW)))
          return_trace (பொய்);
      }
    }
    
    out->deviceTable.serialize_copy (c->serializer, deviceTable,
                                     இது, 0,
                                     hb_serialize_context_t::Head,
                                     &c->plan->base_variation_idx_map);
    return_trace (உண்மை);
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (likely (c->check_struct (இது) &&
			  deviceTable.sanitize (c, இது)));
  }

  பாதுகாக்கப்பட்ட:
  HBUINT16	format;		/* Format identifier--format = 3 */
  FWORD		coordinate;	/* X or Y value, in design units */
  Offset16To<Device>
		deviceTable;	/* Offset to Device table for X or
				 * Y value, from beginning of
				 * BaseCoord table (may be NULL). */
  பொது:
  DEFINE_SIZE_STATIC (6);
};

கட்டமைப்பு BaseCoord
{
  தருக்க has_data () மாறிலி { திரும்பு u.format.v; }

  hb_position_t get_coord (hb_font_t            *font,
			   மாறிலி ItemVariationStore &var_store,
			   hb_direction_t        direction) மாறிலி
  {
    தேர்வு (u.format.v) {
    நிலை 1: hb_barrier (); திரும்பு u.format1.get_coord (font, direction);
    நிலை 2: hb_barrier (); திரும்பு u.format2.get_coord (font, direction);
    நிலை 3: hb_barrier (); திரும்பு u.format3.get_coord (font, var_store, direction);
#ifndef HB_NO_BEYOND_64K
    நிலை 4: hb_barrier (); திரும்பு u.format4.get_coord (font, direction);
#endif
    இயல்பு:திரும்பு 0;
    }
  }

  வெறுமை collect_variation_indices (hb_set_t& varidx_set /* OUT */) மாறிலி
  {
    தேர்வு (u.format.v) {
    நிலை 3: hb_barrier (); u.format3.collect_variation_indices (varidx_set); திரும்பு;
    இயல்பு:திரும்பு;
    }
  }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) மாறிலி
  {
    எனில் (unlikely (!c->may_dispatch (இது, &u.format.v))) திரும்பு c->no_dispatch_return_மதிப்பு ();
    TRACE_DISPATCH (இது, u.format.v);
    தேர்வு (u.format.v) {
    நிலை 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    நிலை 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    நிலை 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
#ifndef HB_NO_BEYOND_64K
    நிலை 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
#endif
    இயல்பு:return_trace (c->default_return_மதிப்பு ());
    }
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    எனில் (unlikely (!u.format.v.sanitize (c))) return_trace (பொய்);
    hb_barrier ();
    தேர்வு (u.format.v) {
    நிலை 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    நிலை 2: hb_barrier (); return_trace (u.format2.sanitize (c));
    நிலை 3: hb_barrier (); return_trace (u.format3.sanitize (c));
#ifndef HB_NO_BEYOND_64K
    நிலை 4: hb_barrier (); return_trace (u.format4.sanitize (c));
#endif
    இயல்பு:return_trace (பொய்);
    }
  }

  பாதுகாக்கப்பட்ட:
  ஒன்றிப்பு {
  கட்டமைப்பு { HBUINT16 v; }	format;
  BaseCoordFormat1	format1;
  BaseCoordFormat2_4<Layout::SmallTypes>
			format2;
  BaseCoordFormat3	format3;
#ifndef HB_NO_BEYOND_64K
  BaseCoordFormat2_4<Layout::MediumTypes>
			format4;
#endif
  } u;
  பொது:
  DEFINE_SIZE_UNION (2, format.v);
};

கட்டமைப்பு FeatMinMaxRecord
{
  முழு cmp (hb_tag_t key) மாறிலி { திரும்பு tag.cmp (key); }

  தருக்க has_data () மாறிலி { திரும்பு tag; }

  hb_tag_t get_feature_tag () மாறிலி { திரும்பு tag; }

  வெறுமை get_min_max (மாறிலி BaseCoord **min, மாறிலி BaseCoord **max) மாறிலி
  {
    எனில் (likely (min)) *min = &(இது+minCoord);
    எனில் (likely (max)) *max = &(இது+maxCoord);
  }

  வெறுமை collect_variation_indices (மாறிலி hb_subset_plan_t* plan,
                                  மாறிலி வெறுமை *base,
                                  hb_set_t& varidx_set /* OUT */) மாறிலி
  {
    எனில் (!plan->layout_features.has (tag))
      திரும்பு;

    (base+minCoord).collect_variation_indices (varidx_set);
    (base+maxCoord).collect_variation_indices (varidx_set);
  }

  தருக்க subset (hb_subset_context_t *c,
               மாறிலி வெறுமை *base) மாறிலி
  {
    TRACE_SUBSET (இது);
    auto *out = c->serializer->embed (*இது);
    எனில் (unlikely (!out)) return_trace (பொய்);

    தருக்க ret = out->minCoord.serialize_subset (c, minCoord, base);
    ret |= out->maxCoord.serialize_subset (c, maxCoord, base);
    return_trace (ret);
  }

  தருக்க sanitize (hb_sanitize_context_t *c, மாறிலி வெறுமை *base) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (likely (c->check_struct (இது) &&
			  minCoord.sanitize (c, base) &&
			  maxCoord.sanitize (c, base)));
  }

  பாதுகாக்கப்பட்ட:
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
  பொது:
  DEFINE_SIZE_STATIC (8);
};

கட்டமைப்பு MinMax
{
  வெறுமை get_min_max (hb_tag_t          feature_tag,
		    மாறிலி BaseCoord **min,
		    மாறிலி BaseCoord **max) மாறிலி
  {
    மாறிலி FeatMinMaxRecord &minMaxCoord = featMinMaxRecords.bsearch (feature_tag);
    எனில் (minMaxCoord.has_data ())
      minMaxCoord.get_min_max (min, max);
    இல்லையெனில்
    {
      எனில் (likely (min)) *min = &(இது+minCoord);
      எனில் (likely (max)) *max = &(இது+maxCoord);
    }
  }

  வெறுமை collect_variation_indices (மாறிலி hb_subset_plan_t* plan,
                                  hb_set_t& varidx_set /* OUT */) மாறிலி
  {
    (இது+minCoord).collect_variation_indices (varidx_set);
    (இது+maxCoord).collect_variation_indices (varidx_set);
    க்கு (மாறிலி FeatMinMaxRecord& record : featMinMaxRecords)
      record.collect_variation_indices (plan, இது, varidx_set);
  }

  தருக்க subset (hb_subset_context_t *c) மாறிலி
  {
    TRACE_SUBSET (இது);
    auto *out = c->serializer->start_embed (*இது);
    எனில் (unlikely (!out || !c->serializer->extend_min (out))) return_trace (பொய்);

    தருக்க ret = out->minCoord.serialize_subset (c, minCoord, இது);
    ret |= out->maxCoord.serialize_subset (c, maxCoord, இது);

    unsigned len = 0;
    க்கு (மாறிலி FeatMinMaxRecord& _ : featMinMaxRecords)
    {
      hb_tag_t feature_tag = _.get_feature_tag ();
      எனில் (!c->plan->layout_features.has (feature_tag))
        தொடர்;

      auto snap = c->serializer->snapshot ();
      எனில் (!_.subset (c, இது)) c->serializer->revert (snap);
      இல்லையெனில் len++;
    }

    எனில் (len > 0) ret = உண்மை;
    எனில் (!ret) return_trace (பொய்);
    return_trace (c->serializer->check_assign (out->featMinMaxRecords.len, len,
                                               HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (likely (c->check_struct (இது) &&
			  minCoord.sanitize (c, இது) &&
			  maxCoord.sanitize (c, இது) &&
			  featMinMaxRecords.sanitize (c, இது)));
  }

  பாதுகாக்கப்பட்ட:
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
  பொது:
  DEFINE_SIZE_ARRAY (6, featMinMaxRecords);
};

கட்டமைப்பு Base_மதிப்புகள்
{
  மாறிலி BaseCoord &get_base_coord (முழு baseline_tag_index) மாறிலி
  {
    எனில் (baseline_tag_index == -1) baseline_tag_index = defaultIndex;
    திரும்பு இது+baseCoords[baseline_tag_index];
  }

  வெறுமை collect_variation_indices (hb_set_t& varidx_set /* OUT */) மாறிலி
  {
    க்கு (மாறிலி auto& _ : baseCoords)
      (இது+_).collect_variation_indices (varidx_set);
  }

  தருக்க subset (hb_subset_context_t *c) மாறிலி
  {
    TRACE_SUBSET (இது);
    auto *out = c->serializer->start_embed (*இது);
    எனில் (unlikely (!out || !c->serializer->extend_min (out))) return_trace (பொய்);
    out->defaultIndex = defaultIndex;

    தருக்க ret = பொய்;
    க்கு (மாறிலி auto& _ : baseCoords)
    {
      auto *o = out->baseCoords.serialize_append (c->serializer);
      எனில் (unlikely (!o)) return_trace (பொய்);
       ret |= o->serialize_subset (c, _, இது);
    }

    return_trace (ret);
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (likely (c->check_struct (இது) &&
			  baseCoords.sanitize (c, இது)));
  }

  பாதுகாக்கப்பட்ட:
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
  பொது:
  DEFINE_SIZE_ARRAY (4, baseCoords);
};

கட்டமைப்பு BaseLangSysRecord
{
  முழு cmp (hb_tag_t key) மாறிலி { திரும்பு baseLangSysTag.cmp (key); }

  தருக்க has_data () மாறிலி { திரும்பு baseLangSysTag; }

  மாறிலி MinMax &get_min_max (மாறிலி வெறுமை* base) மாறிலி { திரும்பு base+minMax; }

  வெறுமை collect_variation_indices (மாறிலி வெறுமை* base,
                                  மாறிலி hb_subset_plan_t* plan,
                                  hb_set_t& varidx_set /* OUT */) மாறிலி
  { (base+minMax).collect_variation_indices (plan, varidx_set); }

  தருக்க subset (hb_subset_context_t *c,
               மாறிலி வெறுமை *base) மாறிலி
  {
    TRACE_SUBSET (இது);
    auto *out = c->serializer->embed (*இது);
    எனில் (unlikely (!out)) return_trace (பொய்);

    return_trace (out->minMax.serialize_subset (c, minMax, base));
  }

  தருக்க sanitize (hb_sanitize_context_t *c, மாறிலி வெறுமை *base) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (likely (c->check_struct (இது) &&
			  minMax.sanitize (c, base)));
  }

  பாதுகாக்கப்பட்ட:
  Tag		baseLangSysTag;	/* 4-byte language system identification tag */
  Offset16To<MinMax>
		minMax;		/* Offset to MinMax table, from beginning
				 * of BaseScript table */
  பொது:
  DEFINE_SIZE_STATIC (6);
};

கட்டமைப்பு BaseScript
{
  மாறிலி MinMax &get_min_max (hb_tag_t language_tag) மாறிலி
  {
    மாறிலி BaseLangSysRecord& record = baseLangSysRecords.bsearch (language_tag);
    திரும்பு record.has_data () ? record.get_min_max (இது) : இது+defaultMinMax;
  }

  மாறிலி BaseCoord &get_base_coord (முழு baseline_tag_index) மாறிலி
  { திரும்பு (இது+base_மதிப்புகள்).get_base_coord (baseline_tag_index); }

  தருக்க has_மதிப்புகள் () மாறிலி { திரும்பு base_மதிப்புகள்; }
  தருக்க has_min_max () மாறிலி { திரும்பு defaultMinMax || baseLangSysRecords; }

  வெறுமை collect_variation_indices (மாறிலி hb_subset_plan_t* plan,
                                  hb_set_t& varidx_set /* OUT */) மாறிலி
  {
    (இது+base_மதிப்புகள்).collect_variation_indices (varidx_set);
    (இது+defaultMinMax).collect_variation_indices (plan, varidx_set);
    
    க்கு (மாறிலி BaseLangSysRecord& _ : baseLangSysRecords)
      _.collect_variation_indices (இது, plan, varidx_set);
  }

  தருக்க subset (hb_subset_context_t *c) மாறிலி
  {
    TRACE_SUBSET (இது);
    auto *out = c->serializer->start_embed (*இது);
    எனில் (unlikely (!out || !c->serializer->extend_min (out))) return_trace (பொய்);

    தருக்க ret = out->base_மதிப்புகள்.serialize_subset (c, base_மதிப்புகள், இது);
    ret |= out->defaultMinMax.serialize_subset (c, defaultMinMax, இது);

    unsigned len = 0;
    க்கு (மாறிலி auto& _ : baseLangSysRecords)
    {
      auto snap = c->serializer->snapshot ();
      எனில் (_.subset (c, இது)) {
        ret = உண்மை;
        len++;
      }
      இல்லையெனில் c->serializer->revert (snap);
    }

    எனில் (!ret) return_trace (பொய்);
    return_trace (c->serializer->check_assign (out->baseLangSysRecords.len, len,
                                               HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (likely (c->check_struct (இது) &&
			  base_மதிப்புகள்.sanitize (c, இது) &&
			  defaultMinMax.sanitize (c, இது) &&
			  baseLangSysRecords.sanitize (c, இது)));
  }

  பாதுகாக்கப்பட்ட:
  Offset16To<Base_மதிப்புகள்>
		base_மதிப்புகள்;	/* Offset to BaseValues table, from beginning
				 * of BaseScript table (may be NULL) */
  Offset16To<MinMax>
		defaultMinMax;	/* Offset to MinMax table, from beginning of
				 * BaseScript table (may be NULL) */
  SortedArray16Of<BaseLangSysRecord>
		baseLangSysRecords;
				/* Number of BaseLangSysRecords
				 * defined — may be zero (0) */

  பொது:
  DEFINE_SIZE_ARRAY (6, baseLangSysRecords);
};

கட்டமைப்பு BaseScriptList;
கட்டமைப்பு BaseScriptRecord
{
  முழு cmp (hb_tag_t key) மாறிலி { திரும்பு baseScriptTag.cmp (key); }

  தருக்க has_data () மாறிலி { திரும்பு baseScriptTag; }

  hb_tag_t get_script_tag () மாறிலி { திரும்பு baseScriptTag; }

  மாறிலி BaseScript &get_base_script (மாறிலி BaseScriptList *list) மாறிலி
  { திரும்பு list+baseScript; }

  வெறுமை collect_variation_indices (மாறிலி hb_subset_plan_t* plan,
                                  மாறிலி வெறுமை* list,
                                  hb_set_t& varidx_set /* OUT */) மாறிலி
  {
    எனில் (!plan->layout_scripts.has (baseScriptTag))
      திரும்பு;

    (list+baseScript).collect_variation_indices (plan, varidx_set);
  }

  தருக்க subset (hb_subset_context_t *c,
               மாறிலி வெறுமை *base) மாறிலி
  {
    TRACE_SUBSET (இது);
    auto *out = c->serializer->embed (*இது);
    எனில் (unlikely (!out)) return_trace (பொய்);

    return_trace (out->baseScript.serialize_subset (c, baseScript, base));
  }

  தருக்க sanitize (hb_sanitize_context_t *c, மாறிலி வெறுமை *base) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (likely (c->check_struct (இது) &&
			  baseScript.sanitize (c, base)));
  }

  பாதுகாக்கப்பட்ட:
  Tag		baseScriptTag;	/* 4-byte script identification tag */
  Offset16To<BaseScript>
		baseScript;	/* Offset to BaseScript table, from beginning
				 * of BaseScriptList */

  பொது:
  DEFINE_SIZE_STATIC (6);
};

கட்டமைப்பு BaseScriptList
{
  மாறிலி BaseScript &get_base_script (hb_tag_t script) மாறிலி
  {
    மாறிலி BaseScriptRecord *record = &baseScriptRecords.bsearch (script);
    எனில் (!record->has_data ()) record = &baseScriptRecords.bsearch (HB_TAG ('D','F','L','T'));
    திரும்பு record->has_data () ? record->get_base_script (இது) : Null (BaseScript);
  }

  வெறுமை collect_variation_indices (மாறிலி hb_subset_plan_t* plan,
                                  hb_set_t& varidx_set /* OUT */) மாறிலி
  {
    க்கு (மாறிலி BaseScriptRecord& _ : baseScriptRecords)
      _.collect_variation_indices (plan, இது, varidx_set);
  }

  தருக்க subset (hb_subset_context_t *c) மாறிலி
  {
    TRACE_SUBSET (இது);
    auto *out = c->serializer->start_embed (*இது);
    எனில் (unlikely (!out || !c->serializer->extend_min (out))) return_trace (பொய்);

    unsigned len = 0;
    க்கு (மாறிலி BaseScriptRecord& _ : baseScriptRecords)
    {
      hb_tag_t script_tag = _.get_script_tag ();
      எனில் (!c->plan->layout_scripts.has (script_tag))
        தொடர்;

      auto snap = c->serializer->snapshot ();
      எனில் (!_.subset (c, இது)) c->serializer->revert (snap);
      இல்லையெனில் len++;
    }

    எனில் (!len) return_trace (பொய்);
    return_trace (c->serializer->check_assign (out->baseScriptRecords.len, len,
                                               HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது) &&
		  baseScriptRecords.sanitize (c, இது));
  }

  பாதுகாக்கப்பட்ட:
  SortedArray16Of<BaseScriptRecord>
			baseScriptRecords;

  பொது:
  DEFINE_SIZE_ARRAY (2, baseScriptRecords);
};

கட்டமைப்பு Axis
{
  தருக்க get_baseline (hb_tag_t          baseline_tag,
		     hb_tag_t          script_tag,
		     hb_tag_t          language_tag,
		     மாறிலி BaseCoord **coord) மாறிலி
  {
    மாறிலி BaseScript &base_script = (இது+baseScriptList).get_base_script (script_tag);
    எனில் (!base_script.has_மதிப்புகள் ())
    {
      *coord = nullptr;
      திரும்பு பொய்;
    }

    எனில் (likely (coord))
    {
      unsigned முழு tag_index = 0;
      எனில் (!(இது+baseTagList).bfind (baseline_tag, &tag_index))
      {
        *coord = nullptr;
        திரும்பு பொய்;
      }
      *coord = &base_script.get_base_coord (tag_index);
    }

    திரும்பு உண்மை;
  }

  தருக்க get_min_max (hb_tag_t          script_tag,
		    hb_tag_t          language_tag,
		    hb_tag_t          feature_tag,
		    மாறிலி BaseCoord **min_coord,
		    மாறிலி BaseCoord **max_coord) மாறிலி
  {
    மாறிலி BaseScript &base_script = (இது+baseScriptList).get_base_script (script_tag);
    எனில் (!base_script.has_min_max ())
    {
      *min_coord = *max_coord = nullptr;
      திரும்பு பொய்;
    }

    base_script.get_min_max (language_tag).get_min_max (feature_tag, min_coord, max_coord);

    திரும்பு உண்மை;
  }

  வெறுமை collect_variation_indices (மாறிலி hb_subset_plan_t* plan,
                                  hb_set_t& varidx_set /* OUT */) மாறிலி
  { (இது+baseScriptList).collect_variation_indices (plan, varidx_set); }

  தருக்க subset (hb_subset_context_t *c) மாறிலி
  {
    TRACE_SUBSET (இது);
    auto *out = c->serializer->embed (*இது);
    எனில் (unlikely (!out)) return_trace (பொய்);

    தருக்க ret = out->baseTagList.serialize_copy (c->serializer, baseTagList, இது);
    ret |= out->baseScriptList.serialize_subset (c, baseScriptList, இது);
    return_trace (ret);
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (likely (c->check_struct (இது) &&
			  baseTagList.sanitize (c, இது) &&
			  baseScriptList.sanitize (c, இது)));
  }

  பாதுகாக்கப்பட்ட:
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

  பொது:
  DEFINE_SIZE_STATIC (4);
};

கட்டமைப்பு BASE
{
  நிலையான constexpr hb_tag_t tableTag = HB_OT_TAG_BASE;

  மாறிலி Axis &get_axis (hb_direction_t direction) மாறிலி
  { திரும்பு HB_DIRECTION_IS_VERTICAL (direction) ? இது+vAxis : இது+hAxis; }

  தருக்க has_var_store () மாறிலி
  { திரும்பு version.to_int () >= 0x00010001u && varStore != 0; }

  மாறிலி ItemVariationStore &get_var_store () மாறிலி
  { திரும்பு version.to_int () < 0x00010001u ? Null (ItemVariationStore) : இது+varStore; }

  வெறுமை collect_variation_indices (மாறிலி hb_subset_plan_t* plan,
                                  hb_set_t& varidx_set /* OUT */) மாறிலி
  {
    (இது+hAxis).collect_variation_indices (plan, varidx_set);
    (இது+vAxis).collect_variation_indices (plan, varidx_set);
  }

  தருக்க subset_varstore (hb_subset_context_t *c,
                        BASE *out /* OUT */) மாறிலி
  {
    TRACE_SUBSET (இது);
    எனில் (!c->serializer->allocate_size<Offset32To<ItemVariationStore>> (Offset32To<ItemVariationStore>::static_size))
        return_trace (பொய்);
    எனில் (!c->plan->normalized_coords)
      return_trace (out->varStore.serialize_subset (c, varStore, இது, c->plan->base_varstore_inner_maps.as_array ()));

    எனில் (c->plan->all_axes_pinned)
      return_trace (உண்மை);

    item_variations_t item_vars;
    எனில் (!item_vars.instantiate (இது+varStore, c->plan, உண்மை, உண்மை,
                                c->plan->base_varstore_inner_maps.as_array ()))
      return_trace (பொய்);

    எனில் (!out->varStore.serialize_serialize (c->serializer,
                                            item_vars.has_long_word (),
                                            c->plan->axis_tags,
                                            item_vars.get_region_list (),
                                            item_vars.get_vardata_encodings ()))
      return_trace (பொய்);

    மாறிலி hb_map_t &varidx_map = item_vars.get_varidx_map ();
    /* base_variation_idx_map in the plan is old_varidx->(varidx, delta)
     * mapping, new varidx is generated for subsetting, we need to remap this
     * after instancing */
    க்கு (auto _ : c->plan->base_variation_idx_map.iter_ref ())
    {
      uint32_t varidx = _.second.first;
      uint32_t *new_varidx;
      எனில் (varidx_map.has (varidx, &new_varidx))
        _.second.first = *new_varidx;
      இல்லையெனில்
        _.second.first = HB_OT_LAYOUT_NO_VARIATIONS_INDEX;
    }
    return_trace (உண்மை);
  }

  தருக்க subset (hb_subset_context_t *c) மாறிலி
  {
    TRACE_SUBSET (இது);
    auto *out = c->serializer->start_embed (*இது);
    எனில் (unlikely (!out || !c->serializer->extend_min (out))) return_trace (பொய்);

    out->version = version;
    தருக்க ret = பொய்;
    எனில் (version.to_int () >= 0x00010001u) ret |= subset_varstore (c, out);

    ret |= out->hAxis.serialize_subset (c, hAxis, இது);
    ret |= out->vAxis.serialize_subset (c, vAxis, இது);

    return_trace (ret);
  }

  தருக்க get_baseline (hb_font_t      *font,
		     hb_tag_t        baseline_tag,
		     hb_direction_t  direction,
		     hb_tag_t        script_tag,
		     hb_tag_t        language_tag,
		     hb_position_t  *base) மாறிலி
  {
    மாறிலி BaseCoord *base_coord = nullptr;
    எனில் (unlikely (!get_axis (direction).get_baseline (baseline_tag, script_tag, language_tag, &base_coord) ||
		  !base_coord || !base_coord->has_data ()))
      திரும்பு பொய்;

    எனில் (likely (base))
      *base = base_coord->get_coord (font, get_var_store (), direction);

    திரும்பு உண்மை;
  }

  தருக்க get_min_max (hb_font_t      *font,
		    hb_direction_t  direction,
		    hb_tag_t        script_tag,
		    hb_tag_t        language_tag,
		    hb_tag_t        feature_tag,
		    hb_position_t  *min,
		    hb_position_t  *max) மாறிலி
  {
    மாறிலி BaseCoord *min_coord, *max_coord;
    எனில் (!get_axis (direction).get_min_max (script_tag, language_tag, feature_tag,
					   &min_coord, &max_coord))
      திரும்பு பொய்;

    மாறிலி ItemVariationStore &var_store = get_var_store ();
    எனில் (likely (min && min_coord)) *min = min_coord->get_coord (font, var_store, direction);
    எனில் (likely (max && max_coord)) *max = max_coord->get_coord (font, var_store, direction);
    திரும்பு உண்மை;
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (likely (c->check_struct (இது) &&
			  hb_barrier () &&
			  likely (version.major == 1) &&
			  hAxis.sanitize (c, இது) &&
			  vAxis.sanitize (c, இது) &&
			  (version.to_int () < 0x00010001u || varStore.sanitize (c, இது))));
  }

  பாதுகாக்கப்பட்ட:
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
  பொது:
  DEFINE_SIZE_MIN (8);
};


} /* namespace OT */


#endif /* HB_OT_LAYOUT_BASE_TABLE_HH */
