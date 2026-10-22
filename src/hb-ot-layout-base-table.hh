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

#اگر_غیر_تعریف_شدہ HB_OT_LAYOUT_بنیاد_جدول_HH
#تعریف_کرو HB_OT_LAYOUT_بنیاد_جدول_HH

#شامل "hb-open-type.hh"
#شامل "hb-ot-layout-common.hh"

نامخلا OT {

/*
 * BASE -- Baseline
 * https://docs.microsoft.com/en-us/typography/opentype/spec/base
 */

ساخت بنیاد_Coord_Format1
{
  hb_position_t get_coord (hb_font_t *font, hb_direction_t direction) ثابت
  {
    واپس HB_DIRECTION_IS_HORIZONTAL (direction) ? font->em_scale_y (coordinate) : font->em_scale_x (coordinate);
  }

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (یہ);
    return_trace ((منطقی) c->serializer->embed (*یہ));
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ));
  }

  محفوظ:
  HBUINT16	format;		/* Format identifier--format = 1 */
  FWORD		coordinate;	/* X or Y value, in design units */
  عوامی:
  DEFINE_سائز_STATIC (4);
};

سانچہ <قسم_نام Types>
ساخت بنیاد_Coord_Format2_4
{
  hb_position_t get_coord (hb_font_t *font, hb_direction_t direction) ثابت
  {
    /* TODO */
    واپس HB_DIRECTION_IS_HORIZONTAL (direction) ? font->em_scale_y (coordinate) : font->em_scale_x (coordinate);
  }

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (یہ);
    خودکار *برآمد = c->serializer->embed (*یہ);
    اگر (unlikely (!برآمد)) return_trace (جھوٹ);

    hb_codepoint_t *new_gid;
    اگر (!c->plan->glyph_map->has (referenceGlyph, &new_gid))
      return_trace (جھوٹ);

    return_trace (c->serializer->check_تفویض (برآمد->referenceGlyph,
                                               *new_gid,
                                               HB_SERIALIZE_ERROR_INT_حد_سے_تجاوز));
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ));
  }

  محفوظ:
  HBUINT16	format;		/* Format identifier--format = 2 or 4 */
  FWORD		coordinate;	/* X or Y value, in design units */
  قسم_نام Types::HBGlyphID
		referenceGlyph;	/* Glyph ID of control glyph */
  HBUINT16	coord_نقطہ;	/* Index of contour point on the
				 * reference glyph */
  عوامی:
  DEFINE_سائز_STATIC (6 + Types::HBGlyphID::static_سائز);
};

ساخت بنیاد_Coord_Format3
{
  hb_position_t get_coord (hb_font_t *font,
			   ثابت شے_Variation_Store &var_store,
			   hb_direction_t direction) ثابت
  {
    ثابت Device &device = یہ+device_جدول;

    واپس HB_DIRECTION_IS_HORIZONTAL (direction)
	 ? hb_saturate_جوڑو (font->em_scale_y (coordinate), device.get_y_delta (font, var_store))
	 : hb_saturate_جوڑو (font->em_scale_x (coordinate), device.get_x_delta (font, var_store));
  }

  خالی collect_variation_indices (hb_سیٹ_t& varidx_سیٹ /*برآمد*/) ثابت
  {
    غیرعلامتی varidx = (یہ+device_جدول).get_variation_اشاریہ ();
    varidx_سیٹ.جوڑو (varidx);
  }

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (یہ);
    خودکار *برآمد = c->serializer->embed (*یہ);
    اگر (unlikely (!برآمد)) return_trace (جھوٹ);

    اگر (!c->plan->pinned_at_default)
    {
      غیرعلامتی var_idx = (یہ+device_جدول).get_variation_اشاریہ ();
      اگر (var_idx != VarIdx::NO_VARIATION)
      {
        hb_pair_t<غیرعلامتی, صحیح> *v;
        اگر (!c->plan->بنیاد_variation_idx_map.has (var_idx, &v))
          return_trace (جھوٹ);
        
        اگر (unlikely (!c->serializer->check_تفویض (برآمد->coordinate, coordinate + hb_دوسرا (*v),
                                                    HB_SERIALIZE_ERROR_INT_حد_سے_تجاوز)))
          return_trace (جھوٹ);
      }
    }
    
    برآمد->device_جدول.serialize_copy (c->serializer, device_جدول,
                                     یہ, 0,
                                     hb_serialize_context_t::سر,
                                     &c->plan->بنیاد_variation_idx_map);
    return_trace (سچ);
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (likely (c->check_struct (یہ) &&
			  device_جدول.sanitize (c, یہ)));
  }

  محفوظ:
  HBUINT16	format;		/* Format identifier--format = 3 */
  FWORD		coordinate;	/* X or Y value, in design units */
  Offset16_مقصد<Device>
		device_جدول;	/* Offset to Device table for X or
				 * Y value, from beginning of
				 * BaseCoord table (may be NULL). */
  عوامی:
  DEFINE_سائز_STATIC (6);
};

ساخت بنیاد_Coord
{
  منطقی has_ڈیٹا () ثابت { واپس u.format.v; }

  hb_position_t get_coord (hb_font_t            *font,
			   ثابت شے_Variation_Store &var_store,
			   hb_direction_t        direction) ثابت
  {
    انتخاب (u.format.v) {
    حالت 1: hb_barrier (); واپس u.format1.get_coord (font, direction);
    حالت 2: hb_barrier (); واپس u.format2.get_coord (font, direction);
    حالت 3: hb_barrier (); واپس u.format3.get_coord (font, var_store, direction);
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
    حالت 4: hb_barrier (); واپس u.format4.get_coord (font, direction);
#اگر_ختم
    طےشدہ:واپس 0;
    }
  }

  خالی collect_variation_indices (hb_سیٹ_t& varidx_سیٹ /*برآمد*/) ثابت
  {
    انتخاب (u.format.v) {
    حالت 3: hb_barrier (); u.format3.collect_variation_indices (varidx_سیٹ); واپس;
    طےشدہ:واپس;
    }
  }

  سانچہ <قسم_نام context_t, قسم_نام ...Ts>
  قسم_نام context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    اگر (unlikely (!c->may_dispatch (یہ, &u.format.v))) واپس c->no_dispatch_return_قدر ();
    TRACE_DISPATCH (یہ, u.format.v);
    انتخاب (u.format.v) {
    حالت 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    حالت 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    حالت 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
    حالت 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
#اگر_ختم
    طےشدہ:return_trace (c->default_return_قدر ());
    }
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    اگر (unlikely (!u.format.v.sanitize (c))) return_trace (جھوٹ);
    hb_barrier ();
    انتخاب (u.format.v) {
    حالت 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    حالت 2: hb_barrier (); return_trace (u.format2.sanitize (c));
    حالت 3: hb_barrier (); return_trace (u.format3.sanitize (c));
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
    حالت 4: hb_barrier (); return_trace (u.format4.sanitize (c));
#اگر_ختم
    طےشدہ:return_trace (جھوٹ);
    }
  }

  محفوظ:
  اتحاد {
  ساخت { HBUINT16 v; }	format;
  بنیاد_Coord_Format1	format1;
  بنیاد_Coord_Format2_4<Layout::SmallTypes>
			format2;
  بنیاد_Coord_Format3	format3;
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
  بنیاد_Coord_Format2_4<Layout::MediumTypes>
			format4;
#اگر_ختم
  } u;
  عوامی:
  DEFINE_سائز_UNION (2, format.v);
};

ساخت Feat_Min_Max_ریکارڈ
{
  صحیح cmp (hb_لیبل_t key) ثابت { واپس لیبل_2.cmp (key); }

  منطقی has_ڈیٹا () ثابت { واپس لیبل_2; }

  hb_لیبل_t get_feature_لیبل () ثابت { واپس لیبل_2; }

  خالی get_min_max (ثابت بنیاد_Coord **min, ثابت بنیاد_Coord **max) ثابت
  {
    اگر (likely (min)) *min = &(یہ+minCoord);
    اگر (likely (max)) *max = &(یہ+maxCoord);
  }

  خالی collect_variation_indices (ثابت hb_subset_plan_t* plan,
                                  ثابت خالی *بنیاد,
                                  hb_سیٹ_t& varidx_سیٹ /*برآمد*/) ثابت
  {
    اگر (!plan->layout_features.has (لیبل_2))
      واپس;

    (بنیاد+minCoord).collect_variation_indices (varidx_سیٹ);
    (بنیاد+maxCoord).collect_variation_indices (varidx_سیٹ);
  }

  منطقی subset (hb_subset_context_t *c,
               ثابت خالی *بنیاد) ثابت
  {
    TRACE_SUBSET (یہ);
    خودکار *برآمد = c->serializer->embed (*یہ);
    اگر (unlikely (!برآمد)) return_trace (جھوٹ);

    منطقی ret = برآمد->minCoord.serialize_subset (c, minCoord, بنیاد);
    ret |= برآمد->maxCoord.serialize_subset (c, maxCoord, بنیاد);
    return_trace (ret);
  }

  منطقی sanitize (hb_sanitize_context_t *c, ثابت خالی *بنیاد) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (likely (c->check_struct (یہ) &&
			  minCoord.sanitize (c, بنیاد) &&
			  maxCoord.sanitize (c, بنیاد)));
  }

  محفوظ:
  لیبل		لیبل_2;		/* 4-byte feature identification tag--must
				 * match feature tag in FeatureList */
  Offset16_مقصد<بنیاد_Coord>
		minCoord;	/* Offset to BaseCoord table that defines
				 * the minimum extent value, from beginning
				 * of MinMax table (may be NULL) */
  Offset16_مقصد<بنیاد_Coord>
		maxCoord;	/* Offset to BaseCoord table that defines
				 * the maximum extent value, from beginning
				 * of MinMax table (may be NULL) */
  عوامی:
  DEFINE_سائز_STATIC (8);
};

ساخت MinMax
{
  خالی get_min_max (hb_لیبل_t          feature_لیبل,
		    ثابت بنیاد_Coord **min,
		    ثابت بنیاد_Coord **max) ثابت
  {
    ثابت Feat_Min_Max_ریکارڈ &minMaxCoord = feat_Min_Max_اندراجات.bsearch (feature_لیبل);
    اگر (minMaxCoord.has_ڈیٹا ())
      minMaxCoord.get_min_max (min, max);
    ورنہ
    {
      اگر (likely (min)) *min = &(یہ+minCoord);
      اگر (likely (max)) *max = &(یہ+maxCoord);
    }
  }

  خالی collect_variation_indices (ثابت hb_subset_plan_t* plan,
                                  hb_سیٹ_t& varidx_سیٹ /*برآمد*/) ثابت
  {
    (یہ+minCoord).collect_variation_indices (varidx_سیٹ);
    (یہ+maxCoord).collect_variation_indices (varidx_سیٹ);
    برائے (ثابت Feat_Min_Max_ریکارڈ& ریکارڈ : feat_Min_Max_اندراجات)
      ریکارڈ.collect_variation_indices (plan, یہ, varidx_سیٹ);
  }

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (یہ);
    خودکار *برآمد = c->serializer->آغاز_embed (*یہ);
    اگر (unlikely (!برآمد || !c->serializer->extend_min (برآمد))) return_trace (جھوٹ);

    منطقی ret = برآمد->minCoord.serialize_subset (c, minCoord, یہ);
    ret |= برآمد->maxCoord.serialize_subset (c, maxCoord, یہ);

    غیرعلامتی len = 0;
    برائے (ثابت Feat_Min_Max_ریکارڈ& _ : feat_Min_Max_اندراجات)
    {
      hb_لیبل_t feature_لیبل = _.get_feature_لیبل ();
      اگر (!c->plan->layout_features.has (feature_لیبل))
        جاری;

      خودکار snap = c->serializer->snapshot ();
      اگر (!_.subset (c, یہ)) c->serializer->revert (snap);
      ورنہ len++;
    }

    اگر (len > 0) ret = سچ;
    اگر (!ret) return_trace (جھوٹ);
    return_trace (c->serializer->check_تفویض (برآمد->feat_Min_Max_اندراجات.len, len,
                                               HB_SERIALIZE_ERROR_INT_حد_سے_تجاوز));
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (likely (c->check_struct (یہ) &&
			  minCoord.sanitize (c, یہ) &&
			  maxCoord.sanitize (c, یہ) &&
			  feat_Min_Max_اندراجات.sanitize (c, یہ)));
  }

  محفوظ:
  Offset16_مقصد<بنیاد_Coord>
		minCoord;	/* Offset to BaseCoord table that defines
				 * minimum extent value, from the beginning
				 * of MinMax table (may be NULL) */
  Offset16_مقصد<بنیاد_Coord>
		maxCoord;	/* Offset to BaseCoord table that defines
				 * maximum extent value, from the beginning
				 * of MinMax table (may be NULL) */
  Sorted_Array16_کا<Feat_Min_Max_ریکارڈ>
		feat_Min_Max_اندراجات;
				/* Array of FeatMinMaxRecords, in alphabetical
				 * order by featureTableTag */
  عوامی:
  DEFINE_سائز_ARRAY (6, feat_Min_Max_اندراجات);
};

ساخت بنیاد_اقدار
{
  ثابت بنیاد_Coord &get_بنیاد_coord (صحیح baseline_لیبل_اشاریہ) ثابت
  {
    اگر (baseline_لیبل_اشاریہ == -1) baseline_لیبل_اشاریہ = default_اشاریہ_2;
    واپس یہ+بنیاد_Coords[baseline_لیبل_اشاریہ];
  }

  خالی collect_variation_indices (hb_سیٹ_t& varidx_سیٹ /*برآمد*/) ثابت
  {
    برائے (ثابت خودکار& _ : بنیاد_Coords)
      (یہ+_).collect_variation_indices (varidx_سیٹ);
  }

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (یہ);
    خودکار *برآمد = c->serializer->آغاز_embed (*یہ);
    اگر (unlikely (!برآمد || !c->serializer->extend_min (برآمد))) return_trace (جھوٹ);
    برآمد->default_اشاریہ_2 = default_اشاریہ_2;

    منطقی ret = جھوٹ;
    برائے (ثابت خودکار& _ : بنیاد_Coords)
    {
      خودکار *o = برآمد->بنیاد_Coords.serialize_append (c->serializer);
      اگر (unlikely (!o)) return_trace (جھوٹ);
       ret |= o->serialize_subset (c, _, یہ);
    }

    return_trace (ret);
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (likely (c->check_struct (یہ) &&
			  بنیاد_Coords.sanitize (c, یہ)));
  }

  محفوظ:
  اشاریہ		default_اشاریہ_2;	/* Index number of default baseline for this
				 * script — equals index position of baseline tag
				 * in baselineTags array of the BaseTagList */
  Array16_کا_Offset16_مقصد<بنیاد_Coord>
		بنیاد_Coords;	/* Number of BaseCoord tables defined — should equal
				 * baseTagCount in the BaseTagList
				 *
				 * Array of offsets to BaseCoord tables, from beginning of
				 * BaseValues table — order matches baselineTags array in
				 * the BaseTagList */
  عوامی:
  DEFINE_سائز_ARRAY (4, بنیاد_Coords);
};

ساخت بنیاد_Lang_Sys_ریکارڈ
{
  صحیح cmp (hb_لیبل_t key) ثابت { واپس بنیاد_Lang_Sys_لیبل.cmp (key); }

  منطقی has_ڈیٹا () ثابت { واپس بنیاد_Lang_Sys_لیبل; }

  ثابت MinMax &get_min_max (ثابت خالی* بنیاد) ثابت { واپس بنیاد+minMax; }

  خالی collect_variation_indices (ثابت خالی* بنیاد,
                                  ثابت hb_subset_plan_t* plan,
                                  hb_سیٹ_t& varidx_سیٹ /*برآمد*/) ثابت
  { (بنیاد+minMax).collect_variation_indices (plan, varidx_سیٹ); }

  منطقی subset (hb_subset_context_t *c,
               ثابت خالی *بنیاد) ثابت
  {
    TRACE_SUBSET (یہ);
    خودکار *برآمد = c->serializer->embed (*یہ);
    اگر (unlikely (!برآمد)) return_trace (جھوٹ);

    return_trace (برآمد->minMax.serialize_subset (c, minMax, بنیاد));
  }

  منطقی sanitize (hb_sanitize_context_t *c, ثابت خالی *بنیاد) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (likely (c->check_struct (یہ) &&
			  minMax.sanitize (c, بنیاد)));
  }

  محفوظ:
  لیبل		بنیاد_Lang_Sys_لیبل;	/* 4-byte language system identification tag */
  Offset16_مقصد<MinMax>
		minMax;		/* Offset to MinMax table, from beginning
				 * of BaseScript table */
  عوامی:
  DEFINE_سائز_STATIC (6);
};

ساخت بنیاد_Script
{
  ثابت MinMax &get_min_max (hb_لیبل_t language_لیبل) ثابت
  {
    ثابت بنیاد_Lang_Sys_ریکارڈ& ریکارڈ = بنیاد_Lang_Sys_اندراجات.bsearch (language_لیبل);
    واپس ریکارڈ.has_ڈیٹا () ? ریکارڈ.get_min_max (یہ) : یہ+defaultMinMax;
  }

  ثابت بنیاد_Coord &get_بنیاد_coord (صحیح baseline_لیبل_اشاریہ) ثابت
  { واپس (یہ+بنیاد_اقدار_2).get_بنیاد_coord (baseline_لیبل_اشاریہ); }

  منطقی has_اقدار () ثابت { واپس بنیاد_اقدار_2; }
  منطقی has_min_max () ثابت { واپس defaultMinMax || بنیاد_Lang_Sys_اندراجات; }

  خالی collect_variation_indices (ثابت hb_subset_plan_t* plan,
                                  hb_سیٹ_t& varidx_سیٹ /*برآمد*/) ثابت
  {
    (یہ+بنیاد_اقدار_2).collect_variation_indices (varidx_سیٹ);
    (یہ+defaultMinMax).collect_variation_indices (plan, varidx_سیٹ);
    
    برائے (ثابت بنیاد_Lang_Sys_ریکارڈ& _ : بنیاد_Lang_Sys_اندراجات)
      _.collect_variation_indices (یہ, plan, varidx_سیٹ);
  }

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (یہ);
    خودکار *برآمد = c->serializer->آغاز_embed (*یہ);
    اگر (unlikely (!برآمد || !c->serializer->extend_min (برآمد))) return_trace (جھوٹ);

    منطقی ret = برآمد->بنیاد_اقدار_2.serialize_subset (c, بنیاد_اقدار_2, یہ);
    ret |= برآمد->defaultMinMax.serialize_subset (c, defaultMinMax, یہ);

    غیرعلامتی len = 0;
    برائے (ثابت خودکار& _ : بنیاد_Lang_Sys_اندراجات)
    {
      خودکار snap = c->serializer->snapshot ();
      اگر (_.subset (c, یہ)) {
        ret = سچ;
        len++;
      }
      ورنہ c->serializer->revert (snap);
    }

    اگر (!ret) return_trace (جھوٹ);
    return_trace (c->serializer->check_تفویض (برآمد->بنیاد_Lang_Sys_اندراجات.len, len,
                                               HB_SERIALIZE_ERROR_INT_حد_سے_تجاوز));
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (likely (c->check_struct (یہ) &&
			  بنیاد_اقدار_2.sanitize (c, یہ) &&
			  defaultMinMax.sanitize (c, یہ) &&
			  بنیاد_Lang_Sys_اندراجات.sanitize (c, یہ)));
  }

  محفوظ:
  Offset16_مقصد<بنیاد_اقدار>
		بنیاد_اقدار_2;	/* Offset to BaseValues table, from beginning
				 * of BaseScript table (may be NULL) */
  Offset16_مقصد<MinMax>
		defaultMinMax;	/* Offset to MinMax table, from beginning of
				 * BaseScript table (may be NULL) */
  Sorted_Array16_کا<بنیاد_Lang_Sys_ریکارڈ>
		بنیاد_Lang_Sys_اندراجات;
				/* Number of BaseLangSysRecords
				 * defined — may be zero (0) */

  عوامی:
  DEFINE_سائز_ARRAY (6, بنیاد_Lang_Sys_اندراجات);
};

ساخت بنیاد_Script_List;
ساخت بنیاد_Script_ریکارڈ
{
  صحیح cmp (hb_لیبل_t key) ثابت { واپس بنیاد_Script_لیبل.cmp (key); }

  منطقی has_ڈیٹا () ثابت { واپس بنیاد_Script_لیبل; }

  hb_لیبل_t get_script_لیبل () ثابت { واپس بنیاد_Script_لیبل; }

  ثابت بنیاد_Script &get_بنیاد_script (ثابت بنیاد_Script_List *list) ثابت
  { واپس list+بنیاد_Script_2; }

  خالی collect_variation_indices (ثابت hb_subset_plan_t* plan,
                                  ثابت خالی* list,
                                  hb_سیٹ_t& varidx_سیٹ /*برآمد*/) ثابت
  {
    اگر (!plan->layout_scripts.has (بنیاد_Script_لیبل))
      واپس;

    (list+بنیاد_Script_2).collect_variation_indices (plan, varidx_سیٹ);
  }

  منطقی subset (hb_subset_context_t *c,
               ثابت خالی *بنیاد) ثابت
  {
    TRACE_SUBSET (یہ);
    خودکار *برآمد = c->serializer->embed (*یہ);
    اگر (unlikely (!برآمد)) return_trace (جھوٹ);

    return_trace (برآمد->بنیاد_Script_2.serialize_subset (c, بنیاد_Script_2, بنیاد));
  }

  منطقی sanitize (hb_sanitize_context_t *c, ثابت خالی *بنیاد) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (likely (c->check_struct (یہ) &&
			  بنیاد_Script_2.sanitize (c, بنیاد)));
  }

  محفوظ:
  لیبل		بنیاد_Script_لیبل;	/* 4-byte script identification tag */
  Offset16_مقصد<بنیاد_Script>
		بنیاد_Script_2;	/* Offset to BaseScript table, from beginning
				 * of BaseScriptList */

  عوامی:
  DEFINE_سائز_STATIC (6);
};

ساخت بنیاد_Script_List
{
  ثابت بنیاد_Script &get_بنیاد_script (hb_لیبل_t script) ثابت
  {
    ثابت بنیاد_Script_ریکارڈ *ریکارڈ = &بنیاد_Script_اندراجات.bsearch (script);
    اگر (!ریکارڈ->has_ڈیٹا ()) ریکارڈ = &بنیاد_Script_اندراجات.bsearch (HB_لیبل ('D','F','L','T'));
    واپس ریکارڈ->has_ڈیٹا () ? ریکارڈ->get_بنیاد_script (یہ) : Null (بنیاد_Script);
  }

  خالی collect_variation_indices (ثابت hb_subset_plan_t* plan,
                                  hb_سیٹ_t& varidx_سیٹ /*برآمد*/) ثابت
  {
    برائے (ثابت بنیاد_Script_ریکارڈ& _ : بنیاد_Script_اندراجات)
      _.collect_variation_indices (plan, یہ, varidx_سیٹ);
  }

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (یہ);
    خودکار *برآمد = c->serializer->آغاز_embed (*یہ);
    اگر (unlikely (!برآمد || !c->serializer->extend_min (برآمد))) return_trace (جھوٹ);

    غیرعلامتی len = 0;
    برائے (ثابت بنیاد_Script_ریکارڈ& _ : بنیاد_Script_اندراجات)
    {
      hb_لیبل_t script_لیبل = _.get_script_لیبل ();
      اگر (!c->plan->layout_scripts.has (script_لیبل))
        جاری;

      خودکار snap = c->serializer->snapshot ();
      اگر (!_.subset (c, یہ)) c->serializer->revert (snap);
      ورنہ len++;
    }

    اگر (!len) return_trace (جھوٹ);
    return_trace (c->serializer->check_تفویض (برآمد->بنیاد_Script_اندراجات.len, len,
                                               HB_SERIALIZE_ERROR_INT_حد_سے_تجاوز));
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ) &&
		  بنیاد_Script_اندراجات.sanitize (c, یہ));
  }

  محفوظ:
  Sorted_Array16_کا<بنیاد_Script_ریکارڈ>
			بنیاد_Script_اندراجات;

  عوامی:
  DEFINE_سائز_ARRAY (2, بنیاد_Script_اندراجات);
};

ساخت Axis
{
  منطقی get_baseline (hb_لیبل_t          baseline_لیبل,
		     hb_لیبل_t          script_لیبل,
		     hb_لیبل_t          language_لیبل,
		     ثابت بنیاد_Coord **coord) ثابت
  {
    ثابت بنیاد_Script &بنیاد_script = (یہ+بنیاد_Script_List_2).get_بنیاد_script (script_لیبل);
    اگر (!بنیاد_script.has_اقدار ())
    {
      *coord = خالی_اشاریہ;
      واپس جھوٹ;
    }

    اگر (likely (coord))
    {
      غیرعلامتی صحیح لیبل_اشاریہ = 0;
      اگر (!(یہ+بنیاد_لیبل_List).bfind (baseline_لیبل, &لیبل_اشاریہ))
      {
        *coord = خالی_اشاریہ;
        واپس جھوٹ;
      }
      *coord = &بنیاد_script.get_بنیاد_coord (لیبل_اشاریہ);
    }

    واپس سچ;
  }

  منطقی get_min_max (hb_لیبل_t          script_لیبل,
		    hb_لیبل_t          language_لیبل,
		    hb_لیبل_t          feature_لیبل,
		    ثابت بنیاد_Coord **min_coord,
		    ثابت بنیاد_Coord **max_coord) ثابت
  {
    ثابت بنیاد_Script &بنیاد_script = (یہ+بنیاد_Script_List_2).get_بنیاد_script (script_لیبل);
    اگر (!بنیاد_script.has_min_max ())
    {
      *min_coord = *max_coord = خالی_اشاریہ;
      واپس جھوٹ;
    }

    بنیاد_script.get_min_max (language_لیبل).get_min_max (feature_لیبل, min_coord, max_coord);

    واپس سچ;
  }

  خالی collect_variation_indices (ثابت hb_subset_plan_t* plan,
                                  hb_سیٹ_t& varidx_سیٹ /*برآمد*/) ثابت
  { (یہ+بنیاد_Script_List_2).collect_variation_indices (plan, varidx_سیٹ); }

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (یہ);
    خودکار *برآمد = c->serializer->embed (*یہ);
    اگر (unlikely (!برآمد)) return_trace (جھوٹ);

    منطقی ret = برآمد->بنیاد_لیبل_List.serialize_copy (c->serializer, بنیاد_لیبل_List, یہ);
    ret |= برآمد->بنیاد_Script_List_2.serialize_subset (c, بنیاد_Script_List_2, یہ);
    return_trace (ret);
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (likely (c->check_struct (یہ) &&
			  بنیاد_لیبل_List.sanitize (c, یہ) &&
			  بنیاد_Script_List_2.sanitize (c, یہ)));
  }

  محفوظ:
  Offset16_مقصد<Sorted_Array16_کا<لیبل>>
		بنیاد_لیبل_List;	/* Offset to BaseTagList table, from beginning
				 * of Axis table (may be NULL)
				 * Array of 4-byte baseline identification tags — must
				 * be in alphabetical order */
  Offset16_مقصد<بنیاد_Script_List>
		بنیاد_Script_List_2;	/* Offset to BaseScriptList table, from beginning
				 * of Axis table
				 * Array of BaseScriptRecords, in alphabetical order
				 * by baseScriptTag */

  عوامی:
  DEFINE_سائز_STATIC (4);
};

ساخت بنیاد_4
{
  ساکن constexpr hb_لیبل_t جدول_لیبل = HB_OT_لیبل_بنیاد;

  ثابت Axis &get_axis (hb_direction_t direction) ثابت
  { واپس HB_DIRECTION_IS_VERTICAL (direction) ? یہ+vAxis : یہ+hAxis; }

  منطقی has_var_store () ثابت
  { واپس نسخہ.مقصد_int () >= 0x00010001u && varStore != 0; }

  ثابت شے_Variation_Store &get_var_store () ثابت
  { واپس نسخہ.مقصد_int () < 0x00010001u ? Null (شے_Variation_Store) : یہ+varStore; }

  خالی collect_variation_indices (ثابت hb_subset_plan_t* plan,
                                  hb_سیٹ_t& varidx_سیٹ /*برآمد*/) ثابت
  {
    (یہ+hAxis).collect_variation_indices (plan, varidx_سیٹ);
    (یہ+vAxis).collect_variation_indices (plan, varidx_سیٹ);
  }

  منطقی subset_varstore (hb_subset_context_t *c,
                        بنیاد_4 *برآمد /*برآمد*/) ثابت
  {
    TRACE_SUBSET (یہ);
    اگر (!c->serializer->allocate_سائز<Offset32_مقصد<شے_Variation_Store>> (Offset32_مقصد<شے_Variation_Store>::static_سائز))
        return_trace (جھوٹ);
    اگر (!c->plan->normalized_coords)
      return_trace (برآمد->varStore.serialize_subset (c, varStore, یہ, c->plan->بنیاد_varstore_inner_maps.as_array ()));

    اگر (c->plan->all_axes_pinned)
      return_trace (سچ);

    شے_variations_t شے_vars;
    اگر (!شے_vars.instantiate (یہ+varStore, c->plan, سچ, سچ,
                                c->plan->بنیاد_varstore_inner_maps.as_array ()))
      return_trace (جھوٹ);

    اگر (!برآمد->varStore.serialize_serialize (c->serializer,
                                            شے_vars.has_long_لفظ (),
                                            c->plan->axis_لیبل_فہرست,
                                            شے_vars.get_region_list (),
                                            شے_vars.get_vardata_encodings ()))
      return_trace (جھوٹ);

    ثابت hb_map_t &varidx_map = شے_vars.get_varidx_map ();
    /* base_variation_idx_map in the plan is old_varidx->(varidx, delta)
     * mapping, new varidx is generated for subsetting, we need to remap this
     * after instancing */
    برائے (خودکار _ : c->plan->بنیاد_variation_idx_map.iter_ref ())
    {
      uint32_t varidx = _.second.پہلا;
      uint32_t *new_varidx;
      اگر (varidx_map.has (varidx, &new_varidx))
        _.second.پہلا = *new_varidx;
      ورنہ
        _.second.پہلا = HB_OT_LAYOUT_NO_VARIATIONS_اشاریہ;
    }
    return_trace (سچ);
  }

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (یہ);
    خودکار *برآمد = c->serializer->آغاز_embed (*یہ);
    اگر (unlikely (!برآمد || !c->serializer->extend_min (برآمد))) return_trace (جھوٹ);

    برآمد->نسخہ = نسخہ;
    منطقی ret = جھوٹ;
    اگر (نسخہ.مقصد_int () >= 0x00010001u) ret |= subset_varstore (c, برآمد);

    ret |= برآمد->hAxis.serialize_subset (c, hAxis, یہ);
    ret |= برآمد->vAxis.serialize_subset (c, vAxis, یہ);

    return_trace (ret);
  }

  منطقی get_baseline (hb_font_t      *font,
		     hb_لیبل_t        baseline_لیبل,
		     hb_direction_t  direction,
		     hb_لیبل_t        script_لیبل,
		     hb_لیبل_t        language_لیبل,
		     hb_position_t  *بنیاد) ثابت
  {
    ثابت بنیاد_Coord *بنیاد_coord = خالی_اشاریہ;
    اگر (unlikely (!get_axis (direction).get_baseline (baseline_لیبل, script_لیبل, language_لیبل, &بنیاد_coord) ||
		  !بنیاد_coord || !بنیاد_coord->has_ڈیٹا ()))
      واپس جھوٹ;

    اگر (likely (بنیاد))
      *بنیاد = بنیاد_coord->get_coord (font, get_var_store (), direction);

    واپس سچ;
  }

  منطقی get_min_max (hb_font_t      *font,
		    hb_direction_t  direction,
		    hb_لیبل_t        script_لیبل,
		    hb_لیبل_t        language_لیبل,
		    hb_لیبل_t        feature_لیبل,
		    hb_position_t  *min,
		    hb_position_t  *max) ثابت
  {
    ثابت بنیاد_Coord *min_coord, *max_coord;
    اگر (!get_axis (direction).get_min_max (script_لیبل, language_لیبل, feature_لیبل,
					   &min_coord, &max_coord))
      واپس جھوٹ;

    ثابت شے_Variation_Store &var_store = get_var_store ();
    اگر (likely (min && min_coord)) *min = min_coord->get_coord (font, var_store, direction);
    اگر (likely (max && max_coord)) *max = max_coord->get_coord (font, var_store, direction);
    واپس سچ;
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (likely (c->check_struct (یہ) &&
			  hb_barrier () &&
			  likely (نسخہ.major == 1) &&
			  hAxis.sanitize (c, یہ) &&
			  vAxis.sanitize (c, یہ) &&
			  (نسخہ.مقصد_int () < 0x00010001u || varStore.sanitize (c, یہ))));
  }

  محفوظ:
  Fixed_نسخہ<>نسخہ;	/* Version of the BASE table */
  Offset16_مقصد<Axis>hAxis;		/* Offset to horizontal Axis table, from beginning
				 * of BASE table (may be NULL) */
  Offset16_مقصد<Axis>vAxis;		/* Offset to vertical Axis table, from beginning
				 * of BASE table (may be NULL) */
  Offset32_مقصد<شے_Variation_Store>
		varStore;	/* Offset to the table of Item Variation
				 * Store--from beginning of BASE
				 * header (may be NULL).  Introduced
				 * in version 0x00010001. */
  عوامی:
  DEFINE_سائز_MIN (8);
};


} /* namespace OT */


#اگر_ختم /* HB_OT_LAYOUT_BASE_TABLE_HH */
