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

#اگر_تعریف_نشده HB_OT_LAYOUT_پایه_جدول_HH
#تعریف_کن HB_OT_LAYOUT_پایه_جدول_HH

#ضمن "hb-open-type.hh"
#ضمن "hb-ot-layout-common.hh"

نطاق OT {

/*
 * BASE -- Baseline
 * https://docs.microsoft.com/en-us/typography/opentype/spec/base
 */

بنية پایه_Coord_Format1
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
  DEFINE_اندازه_STATIC (4);
};

قالب <نام_نوع Types>
بنية پایه_Coord_Format2_4
{
  hb_position_t get_coord (hb_font_t *font, hb_direction_t direction) ثابت
  {
    /* TODO */
    أرجع HB_DIRECTION_IS_HORIZONTAL (direction) ? font->em_scale_y (coordinate) : font->em_scale_x (coordinate);
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *بیرون = c->serializer->embed (*هذا);
    إذا (unlikely (!بیرون)) return_trace (خطأ);

    hb_codepoint_t *new_gid;
    إذا (!c->plan->glyph_map->has (referenceGlyph, &new_gid))
      return_trace (خطأ);

    return_trace (c->serializer->check_تخصیص_بده (بیرون->referenceGlyph,
                                               *new_gid,
                                               HB_SERIALIZE_ERROR_INT_سرریز));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  محمي:
  HBUINT16	format;		/* Format identifier--format = 2 or 4 */
  FWORD		coordinate;	/* X or Y value, in design units */
  نام_نوع Types::HBGlyphID
		referenceGlyph;	/* Glyph ID of control glyph */
  HBUINT16	coord_نقطه;	/* Index of contour point on the
				 * reference glyph */
  عام:
  DEFINE_اندازه_STATIC (6 + Types::HBGlyphID::static_اندازه);
};

بنية پایه_Coord_Format3
{
  hb_position_t get_coord (hb_font_t *font,
			   ثابت قلم_Variation_Store &var_store,
			   hb_direction_t direction) ثابت
  {
    ثابت Device &device = هذا+device_جدول;

    أرجع HB_DIRECTION_IS_HORIZONTAL (direction)
	 ? hb_saturate_جمع_کن (font->em_scale_y (coordinate), device.get_y_delta (font, var_store))
	 : hb_saturate_جمع_کن (font->em_scale_x (coordinate), device.get_x_delta (font, var_store));
  }

  فراغ collect_variation_indices (hb_مجموعه_t& varidx_مجموعه /*بیرون*/) ثابت
  {
    بیعلامت varidx = (هذا+device_جدول).get_variation_شاخص ();
    varidx_مجموعه.جمع_کن (varidx);
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *بیرون = c->serializer->embed (*هذا);
    إذا (unlikely (!بیرون)) return_trace (خطأ);

    إذا (!c->plan->pinned_at_default)
    {
      بیعلامت var_idx = (هذا+device_جدول).get_variation_شاخص ();
      إذا (var_idx != VarIdx::NO_VARIATION)
      {
        hb_pair_t<بیعلامت, صحيح> *v;
        إذا (!c->plan->پایه_variation_idx_map.has (var_idx, &v))
          return_trace (خطأ);
        
        إذا (unlikely (!c->serializer->check_تخصیص_بده (بیرون->coordinate, coordinate + hb_دومی (*v),
                                                    HB_SERIALIZE_ERROR_INT_سرریز)))
          return_trace (خطأ);
      }
    }
    
    بیرون->device_جدول.serialize_copy (c->serializer, device_جدول,
                                     هذا, 0,
                                     hb_serialize_context_t::سر,
                                     &c->plan->پایه_variation_idx_map);
    return_trace (درست);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (c->check_struct (هذا) &&
			  device_جدول.sanitize (c, هذا)));
  }

  محمي:
  HBUINT16	format;		/* Format identifier--format = 3 */
  FWORD		coordinate;	/* X or Y value, in design units */
  Offset16_مقصد<Device>
		device_جدول;	/* Offset to Device table for X or
				 * Y value, from beginning of
				 * BaseCoord table (may be NULL). */
  عام:
  DEFINE_اندازه_STATIC (6);
};

بنية پایه_Coord
{
  منطقي has_داده () ثابت { أرجع u.format.v; }

  hb_position_t get_coord (hb_font_t            *font,
			   ثابت قلم_Variation_Store &var_store,
			   hb_direction_t        direction) ثابت
  {
    بدل (u.format.v) {
    حالة 1: hb_barrier (); أرجع u.format1.get_coord (font, direction);
    حالة 2: hb_barrier (); أرجع u.format2.get_coord (font, direction);
    حالة 3: hb_barrier (); أرجع u.format3.get_coord (font, var_store, direction);
#اگر_تعریف_نشده HB_NO_BEYOND_64K
    حالة 4: hb_barrier (); أرجع u.format4.get_coord (font, direction);
#پایان_اگر
    افتراضي:أرجع 0;
    }
  }

  فراغ collect_variation_indices (hb_مجموعه_t& varidx_مجموعه /*بیرون*/) ثابت
  {
    بدل (u.format.v) {
    حالة 3: hb_barrier (); u.format3.collect_variation_indices (varidx_مجموعه); أرجع;
    افتراضي:أرجع;
    }
  }

  قالب <نام_نوع context_t, نام_نوع ...Ts>
  نام_نوع context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    إذا (unlikely (!c->may_dispatch (هذا, &u.format.v))) أرجع c->no_dispatch_return_مقدار ();
    TRACE_DISPATCH (هذا, u.format.v);
    بدل (u.format.v) {
    حالة 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    حالة 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    حالة 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
#اگر_تعریف_نشده HB_NO_BEYOND_64K
    حالة 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
#پایان_اگر
    افتراضي:return_trace (c->default_return_مقدار ());
    }
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (unlikely (!u.format.v.sanitize (c))) return_trace (خطأ);
    hb_barrier ();
    بدل (u.format.v) {
    حالة 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    حالة 2: hb_barrier (); return_trace (u.format2.sanitize (c));
    حالة 3: hb_barrier (); return_trace (u.format3.sanitize (c));
#اگر_تعریف_نشده HB_NO_BEYOND_64K
    حالة 4: hb_barrier (); return_trace (u.format4.sanitize (c));
#پایان_اگر
    افتراضي:return_trace (خطأ);
    }
  }

  محمي:
  اتحاد {
  بنية { HBUINT16 v; }	format;
  پایه_Coord_Format1	format1;
  پایه_Coord_Format2_4<Layout::SmallTypes>
			format2;
  پایه_Coord_Format3	format3;
#اگر_تعریف_نشده HB_NO_BEYOND_64K
  پایه_Coord_Format2_4<Layout::MediumTypes>
			format4;
#پایان_اگر
  } u;
  عام:
  DEFINE_اندازه_UNION (2, format.v);
};

بنية Feat_Min_Max_رکورد
{
  صحيح cmp (hb_برچسب_t key) ثابت { أرجع برچسب_2.cmp (key); }

  منطقي has_داده () ثابت { أرجع برچسب_2; }

  hb_برچسب_t get_feature_برچسب () ثابت { أرجع برچسب_2; }

  فراغ get_min_max (ثابت پایه_Coord **min, ثابت پایه_Coord **max) ثابت
  {
    إذا (likely (min)) *min = &(هذا+minCoord);
    إذا (likely (max)) *max = &(هذا+maxCoord);
  }

  فراغ collect_variation_indices (ثابت hb_subset_plan_t* plan,
                                  ثابت فراغ *پایه,
                                  hb_مجموعه_t& varidx_مجموعه /*بیرون*/) ثابت
  {
    إذا (!plan->layout_features.has (برچسب_2))
      أرجع;

    (پایه+minCoord).collect_variation_indices (varidx_مجموعه);
    (پایه+maxCoord).collect_variation_indices (varidx_مجموعه);
  }

  منطقي subset (hb_subset_context_t *c,
               ثابت فراغ *پایه) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *بیرون = c->serializer->embed (*هذا);
    إذا (unlikely (!بیرون)) return_trace (خطأ);

    منطقي ret = بیرون->minCoord.serialize_subset (c, minCoord, پایه);
    ret |= بیرون->maxCoord.serialize_subset (c, maxCoord, پایه);
    return_trace (ret);
  }

  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *پایه) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (c->check_struct (هذا) &&
			  minCoord.sanitize (c, پایه) &&
			  maxCoord.sanitize (c, پایه)));
  }

  محمي:
  برچسب		برچسب_2;		/* 4-byte feature identification tag--must
				 * match feature tag in FeatureList */
  Offset16_مقصد<پایه_Coord>
		minCoord;	/* Offset to BaseCoord table that defines
				 * the minimum extent value, from beginning
				 * of MinMax table (may be NULL) */
  Offset16_مقصد<پایه_Coord>
		maxCoord;	/* Offset to BaseCoord table that defines
				 * the maximum extent value, from beginning
				 * of MinMax table (may be NULL) */
  عام:
  DEFINE_اندازه_STATIC (8);
};

بنية MinMax
{
  فراغ get_min_max (hb_برچسب_t          feature_برچسب,
		    ثابت پایه_Coord **min,
		    ثابت پایه_Coord **max) ثابت
  {
    ثابت Feat_Min_Max_رکورد &minMaxCoord = feat_Min_Max_رکوردها.bsearch (feature_برچسب);
    إذا (minMaxCoord.has_داده ())
      minMaxCoord.get_min_max (min, max);
    وإلا
    {
      إذا (likely (min)) *min = &(هذا+minCoord);
      إذا (likely (max)) *max = &(هذا+maxCoord);
    }
  }

  فراغ collect_variation_indices (ثابت hb_subset_plan_t* plan,
                                  hb_مجموعه_t& varidx_مجموعه /*بیرون*/) ثابت
  {
    (هذا+minCoord).collect_variation_indices (varidx_مجموعه);
    (هذا+maxCoord).collect_variation_indices (varidx_مجموعه);
    لكل (ثابت Feat_Min_Max_رکورد& رکورد : feat_Min_Max_رکوردها)
      رکورد.collect_variation_indices (plan, هذا, varidx_مجموعه);
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *بیرون = c->serializer->شروع_embed (*هذا);
    إذا (unlikely (!بیرون || !c->serializer->extend_min (بیرون))) return_trace (خطأ);

    منطقي ret = بیرون->minCoord.serialize_subset (c, minCoord, هذا);
    ret |= بیرون->maxCoord.serialize_subset (c, maxCoord, هذا);

    بیعلامت len = 0;
    لكل (ثابت Feat_Min_Max_رکورد& _ : feat_Min_Max_رکوردها)
    {
      hb_برچسب_t feature_برچسب = _.get_feature_برچسب ();
      إذا (!c->plan->layout_features.has (feature_برچسب))
        استمر;

      خودکار snap = c->serializer->snapshot ();
      إذا (!_.subset (c, هذا)) c->serializer->revert (snap);
      وإلا len++;
    }

    إذا (len > 0) ret = درست;
    إذا (!ret) return_trace (خطأ);
    return_trace (c->serializer->check_تخصیص_بده (بیرون->feat_Min_Max_رکوردها.len, len,
                                               HB_SERIALIZE_ERROR_INT_سرریز));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (c->check_struct (هذا) &&
			  minCoord.sanitize (c, هذا) &&
			  maxCoord.sanitize (c, هذا) &&
			  feat_Min_Max_رکوردها.sanitize (c, هذا)));
  }

  محمي:
  Offset16_مقصد<پایه_Coord>
		minCoord;	/* Offset to BaseCoord table that defines
				 * minimum extent value, from the beginning
				 * of MinMax table (may be NULL) */
  Offset16_مقصد<پایه_Coord>
		maxCoord;	/* Offset to BaseCoord table that defines
				 * maximum extent value, from the beginning
				 * of MinMax table (may be NULL) */
  Sorted_Array16_از<Feat_Min_Max_رکورد>
		feat_Min_Max_رکوردها;
				/* Array of FeatMinMaxRecords, in alphabetical
				 * order by featureTableTag */
  عام:
  DEFINE_اندازه_ARRAY (6, feat_Min_Max_رکوردها);
};

بنية پایه_مقادیر
{
  ثابت پایه_Coord &get_پایه_coord (صحيح baseline_برچسب_شاخص) ثابت
  {
    إذا (baseline_برچسب_شاخص == -1) baseline_برچسب_شاخص = default_شاخص_2;
    أرجع هذا+پایه_Coords[baseline_برچسب_شاخص];
  }

  فراغ collect_variation_indices (hb_مجموعه_t& varidx_مجموعه /*بیرون*/) ثابت
  {
    لكل (ثابت خودکار& _ : پایه_Coords)
      (هذا+_).collect_variation_indices (varidx_مجموعه);
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *بیرون = c->serializer->شروع_embed (*هذا);
    إذا (unlikely (!بیرون || !c->serializer->extend_min (بیرون))) return_trace (خطأ);
    بیرون->default_شاخص_2 = default_شاخص_2;

    منطقي ret = خطأ;
    لكل (ثابت خودکار& _ : پایه_Coords)
    {
      خودکار *o = بیرون->پایه_Coords.serialize_append (c->serializer);
      إذا (unlikely (!o)) return_trace (خطأ);
       ret |= o->serialize_subset (c, _, هذا);
    }

    return_trace (ret);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (c->check_struct (هذا) &&
			  پایه_Coords.sanitize (c, هذا)));
  }

  محمي:
  شاخص		default_شاخص_2;	/* Index number of default baseline for this
				 * script — equals index position of baseline tag
				 * in baselineTags array of the BaseTagList */
  Array16_از_Offset16_مقصد<پایه_Coord>
		پایه_Coords;	/* Number of BaseCoord tables defined — should equal
				 * baseTagCount in the BaseTagList
				 *
				 * Array of offsets to BaseCoord tables, from beginning of
				 * BaseValues table — order matches baselineTags array in
				 * the BaseTagList */
  عام:
  DEFINE_اندازه_ARRAY (4, پایه_Coords);
};

بنية پایه_Lang_Sys_رکورد
{
  صحيح cmp (hb_برچسب_t key) ثابت { أرجع پایه_Lang_Sys_برچسب.cmp (key); }

  منطقي has_داده () ثابت { أرجع پایه_Lang_Sys_برچسب; }

  ثابت MinMax &get_min_max (ثابت فراغ* پایه) ثابت { أرجع پایه+minMax; }

  فراغ collect_variation_indices (ثابت فراغ* پایه,
                                  ثابت hb_subset_plan_t* plan,
                                  hb_مجموعه_t& varidx_مجموعه /*بیرون*/) ثابت
  { (پایه+minMax).collect_variation_indices (plan, varidx_مجموعه); }

  منطقي subset (hb_subset_context_t *c,
               ثابت فراغ *پایه) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *بیرون = c->serializer->embed (*هذا);
    إذا (unlikely (!بیرون)) return_trace (خطأ);

    return_trace (بیرون->minMax.serialize_subset (c, minMax, پایه));
  }

  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *پایه) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (c->check_struct (هذا) &&
			  minMax.sanitize (c, پایه)));
  }

  محمي:
  برچسب		پایه_Lang_Sys_برچسب;	/* 4-byte language system identification tag */
  Offset16_مقصد<MinMax>
		minMax;		/* Offset to MinMax table, from beginning
				 * of BaseScript table */
  عام:
  DEFINE_اندازه_STATIC (6);
};

بنية پایه_Script
{
  ثابت MinMax &get_min_max (hb_برچسب_t language_برچسب) ثابت
  {
    ثابت پایه_Lang_Sys_رکورد& رکورد = پایه_Lang_Sys_رکوردها.bsearch (language_برچسب);
    أرجع رکورد.has_داده () ? رکورد.get_min_max (هذا) : هذا+defaultMinMax;
  }

  ثابت پایه_Coord &get_پایه_coord (صحيح baseline_برچسب_شاخص) ثابت
  { أرجع (هذا+پایه_مقادیر_2).get_پایه_coord (baseline_برچسب_شاخص); }

  منطقي has_مقادیر () ثابت { أرجع پایه_مقادیر_2; }
  منطقي has_min_max () ثابت { أرجع defaultMinMax || پایه_Lang_Sys_رکوردها; }

  فراغ collect_variation_indices (ثابت hb_subset_plan_t* plan,
                                  hb_مجموعه_t& varidx_مجموعه /*بیرون*/) ثابت
  {
    (هذا+پایه_مقادیر_2).collect_variation_indices (varidx_مجموعه);
    (هذا+defaultMinMax).collect_variation_indices (plan, varidx_مجموعه);
    
    لكل (ثابت پایه_Lang_Sys_رکورد& _ : پایه_Lang_Sys_رکوردها)
      _.collect_variation_indices (هذا, plan, varidx_مجموعه);
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *بیرون = c->serializer->شروع_embed (*هذا);
    إذا (unlikely (!بیرون || !c->serializer->extend_min (بیرون))) return_trace (خطأ);

    منطقي ret = بیرون->پایه_مقادیر_2.serialize_subset (c, پایه_مقادیر_2, هذا);
    ret |= بیرون->defaultMinMax.serialize_subset (c, defaultMinMax, هذا);

    بیعلامت len = 0;
    لكل (ثابت خودکار& _ : پایه_Lang_Sys_رکوردها)
    {
      خودکار snap = c->serializer->snapshot ();
      إذا (_.subset (c, هذا)) {
        ret = درست;
        len++;
      }
      وإلا c->serializer->revert (snap);
    }

    إذا (!ret) return_trace (خطأ);
    return_trace (c->serializer->check_تخصیص_بده (بیرون->پایه_Lang_Sys_رکوردها.len, len,
                                               HB_SERIALIZE_ERROR_INT_سرریز));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (c->check_struct (هذا) &&
			  پایه_مقادیر_2.sanitize (c, هذا) &&
			  defaultMinMax.sanitize (c, هذا) &&
			  پایه_Lang_Sys_رکوردها.sanitize (c, هذا)));
  }

  محمي:
  Offset16_مقصد<پایه_مقادیر>
		پایه_مقادیر_2;	/* Offset to BaseValues table, from beginning
				 * of BaseScript table (may be NULL) */
  Offset16_مقصد<MinMax>
		defaultMinMax;	/* Offset to MinMax table, from beginning of
				 * BaseScript table (may be NULL) */
  Sorted_Array16_از<پایه_Lang_Sys_رکورد>
		پایه_Lang_Sys_رکوردها;
				/* Number of BaseLangSysRecords
				 * defined — may be zero (0) */

  عام:
  DEFINE_اندازه_ARRAY (6, پایه_Lang_Sys_رکوردها);
};

بنية پایه_Script_List;
بنية پایه_Script_رکورد
{
  صحيح cmp (hb_برچسب_t key) ثابت { أرجع پایه_Script_برچسب.cmp (key); }

  منطقي has_داده () ثابت { أرجع پایه_Script_برچسب; }

  hb_برچسب_t get_script_برچسب () ثابت { أرجع پایه_Script_برچسب; }

  ثابت پایه_Script &get_پایه_script (ثابت پایه_Script_List *list) ثابت
  { أرجع list+پایه_Script_2; }

  فراغ collect_variation_indices (ثابت hb_subset_plan_t* plan,
                                  ثابت فراغ* list,
                                  hb_مجموعه_t& varidx_مجموعه /*بیرون*/) ثابت
  {
    إذا (!plan->layout_scripts.has (پایه_Script_برچسب))
      أرجع;

    (list+پایه_Script_2).collect_variation_indices (plan, varidx_مجموعه);
  }

  منطقي subset (hb_subset_context_t *c,
               ثابت فراغ *پایه) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *بیرون = c->serializer->embed (*هذا);
    إذا (unlikely (!بیرون)) return_trace (خطأ);

    return_trace (بیرون->پایه_Script_2.serialize_subset (c, پایه_Script_2, پایه));
  }

  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *پایه) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (c->check_struct (هذا) &&
			  پایه_Script_2.sanitize (c, پایه)));
  }

  محمي:
  برچسب		پایه_Script_برچسب;	/* 4-byte script identification tag */
  Offset16_مقصد<پایه_Script>
		پایه_Script_2;	/* Offset to BaseScript table, from beginning
				 * of BaseScriptList */

  عام:
  DEFINE_اندازه_STATIC (6);
};

بنية پایه_Script_List
{
  ثابت پایه_Script &get_پایه_script (hb_برچسب_t script) ثابت
  {
    ثابت پایه_Script_رکورد *رکورد = &پایه_Script_رکوردها.bsearch (script);
    إذا (!رکورد->has_داده ()) رکورد = &پایه_Script_رکوردها.bsearch (HB_برچسب ('D','F','L','T'));
    أرجع رکورد->has_داده () ? رکورد->get_پایه_script (هذا) : Null (پایه_Script);
  }

  فراغ collect_variation_indices (ثابت hb_subset_plan_t* plan,
                                  hb_مجموعه_t& varidx_مجموعه /*بیرون*/) ثابت
  {
    لكل (ثابت پایه_Script_رکورد& _ : پایه_Script_رکوردها)
      _.collect_variation_indices (plan, هذا, varidx_مجموعه);
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *بیرون = c->serializer->شروع_embed (*هذا);
    إذا (unlikely (!بیرون || !c->serializer->extend_min (بیرون))) return_trace (خطأ);

    بیعلامت len = 0;
    لكل (ثابت پایه_Script_رکورد& _ : پایه_Script_رکوردها)
    {
      hb_برچسب_t script_برچسب = _.get_script_برچسب ();
      إذا (!c->plan->layout_scripts.has (script_برچسب))
        استمر;

      خودکار snap = c->serializer->snapshot ();
      إذا (!_.subset (c, هذا)) c->serializer->revert (snap);
      وإلا len++;
    }

    إذا (!len) return_trace (خطأ);
    return_trace (c->serializer->check_تخصیص_بده (بیرون->پایه_Script_رکوردها.len, len,
                                               HB_SERIALIZE_ERROR_INT_سرریز));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  پایه_Script_رکوردها.sanitize (c, هذا));
  }

  محمي:
  Sorted_Array16_از<پایه_Script_رکورد>
			پایه_Script_رکوردها;

  عام:
  DEFINE_اندازه_ARRAY (2, پایه_Script_رکوردها);
};

بنية Axis
{
  منطقي get_baseline (hb_برچسب_t          baseline_برچسب,
		     hb_برچسب_t          script_برچسب,
		     hb_برچسب_t          language_برچسب,
		     ثابت پایه_Coord **coord) ثابت
  {
    ثابت پایه_Script &پایه_script = (هذا+پایه_Script_List_2).get_پایه_script (script_برچسب);
    إذا (!پایه_script.has_مقادیر ())
    {
      *coord = اشاره_تهی;
      أرجع خطأ;
    }

    إذا (likely (coord))
    {
      بیعلامت صحيح برچسب_شاخص = 0;
      إذا (!(هذا+پایه_برچسب_List).bfind (baseline_برچسب, &برچسب_شاخص))
      {
        *coord = اشاره_تهی;
        أرجع خطأ;
      }
      *coord = &پایه_script.get_پایه_coord (برچسب_شاخص);
    }

    أرجع درست;
  }

  منطقي get_min_max (hb_برچسب_t          script_برچسب,
		    hb_برچسب_t          language_برچسب,
		    hb_برچسب_t          feature_برچسب,
		    ثابت پایه_Coord **min_coord,
		    ثابت پایه_Coord **max_coord) ثابت
  {
    ثابت پایه_Script &پایه_script = (هذا+پایه_Script_List_2).get_پایه_script (script_برچسب);
    إذا (!پایه_script.has_min_max ())
    {
      *min_coord = *max_coord = اشاره_تهی;
      أرجع خطأ;
    }

    پایه_script.get_min_max (language_برچسب).get_min_max (feature_برچسب, min_coord, max_coord);

    أرجع درست;
  }

  فراغ collect_variation_indices (ثابت hb_subset_plan_t* plan,
                                  hb_مجموعه_t& varidx_مجموعه /*بیرون*/) ثابت
  { (هذا+پایه_Script_List_2).collect_variation_indices (plan, varidx_مجموعه); }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *بیرون = c->serializer->embed (*هذا);
    إذا (unlikely (!بیرون)) return_trace (خطأ);

    منطقي ret = بیرون->پایه_برچسب_List.serialize_copy (c->serializer, پایه_برچسب_List, هذا);
    ret |= بیرون->پایه_Script_List_2.serialize_subset (c, پایه_Script_List_2, هذا);
    return_trace (ret);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (c->check_struct (هذا) &&
			  پایه_برچسب_List.sanitize (c, هذا) &&
			  پایه_Script_List_2.sanitize (c, هذا)));
  }

  محمي:
  Offset16_مقصد<Sorted_Array16_از<برچسب>>
		پایه_برچسب_List;	/* Offset to BaseTagList table, from beginning
				 * of Axis table (may be NULL)
				 * Array of 4-byte baseline identification tags — must
				 * be in alphabetical order */
  Offset16_مقصد<پایه_Script_List>
		پایه_Script_List_2;	/* Offset to BaseScriptList table, from beginning
				 * of Axis table
				 * Array of BaseScriptRecords, in alphabetical order
				 * by baseScriptTag */

  عام:
  DEFINE_اندازه_STATIC (4);
};

بنية پایه_4
{
  ساكن constexpr hb_برچسب_t جدول_برچسب = HB_OT_برچسب_پایه;

  ثابت Axis &get_axis (hb_direction_t direction) ثابت
  { أرجع HB_DIRECTION_IS_VERTICAL (direction) ? هذا+vAxis : هذا+hAxis; }

  منطقي has_var_store () ثابت
  { أرجع نسخه.مقصد_int () >= 0x00010001u && varStore != 0; }

  ثابت قلم_Variation_Store &get_var_store () ثابت
  { أرجع نسخه.مقصد_int () < 0x00010001u ? Null (قلم_Variation_Store) : هذا+varStore; }

  فراغ collect_variation_indices (ثابت hb_subset_plan_t* plan,
                                  hb_مجموعه_t& varidx_مجموعه /*بیرون*/) ثابت
  {
    (هذا+hAxis).collect_variation_indices (plan, varidx_مجموعه);
    (هذا+vAxis).collect_variation_indices (plan, varidx_مجموعه);
  }

  منطقي subset_varstore (hb_subset_context_t *c,
                        پایه_4 *بیرون /*بیرون*/) ثابت
  {
    TRACE_SUBSET (هذا);
    إذا (!c->serializer->allocate_اندازه<Offset32_مقصد<قلم_Variation_Store>> (Offset32_مقصد<قلم_Variation_Store>::static_اندازه))
        return_trace (خطأ);
    إذا (!c->plan->normalized_coords)
      return_trace (بیرون->varStore.serialize_subset (c, varStore, هذا, c->plan->پایه_varstore_inner_maps.as_array ()));

    إذا (c->plan->all_axes_pinned)
      return_trace (درست);

    قلم_variations_t قلم_vars;
    إذا (!قلم_vars.instantiate (هذا+varStore, c->plan, درست, درست,
                                c->plan->پایه_varstore_inner_maps.as_array ()))
      return_trace (خطأ);

    إذا (!بیرون->varStore.serialize_serialize (c->serializer,
                                            قلم_vars.has_long_کلمه (),
                                            c->plan->axis_برچسبها,
                                            قلم_vars.get_region_list (),
                                            قلم_vars.get_vardata_encodings ()))
      return_trace (خطأ);

    ثابت hb_map_t &varidx_map = قلم_vars.get_varidx_map ();
    /* base_variation_idx_map in the plan is old_varidx->(varidx, delta)
     * mapping, new varidx is generated for subsetting, we need to remap this
     * after instancing */
    لكل (خودکار _ : c->plan->پایه_variation_idx_map.iter_ref ())
    {
      uint32_t varidx = _.second.اولی;
      uint32_t *new_varidx;
      إذا (varidx_map.has (varidx, &new_varidx))
        _.second.اولی = *new_varidx;
      وإلا
        _.second.اولی = HB_OT_LAYOUT_NO_VARIATIONS_شاخص;
    }
    return_trace (درست);
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *بیرون = c->serializer->شروع_embed (*هذا);
    إذا (unlikely (!بیرون || !c->serializer->extend_min (بیرون))) return_trace (خطأ);

    بیرون->نسخه = نسخه;
    منطقي ret = خطأ;
    إذا (نسخه.مقصد_int () >= 0x00010001u) ret |= subset_varstore (c, بیرون);

    ret |= بیرون->hAxis.serialize_subset (c, hAxis, هذا);
    ret |= بیرون->vAxis.serialize_subset (c, vAxis, هذا);

    return_trace (ret);
  }

  منطقي get_baseline (hb_font_t      *font,
		     hb_برچسب_t        baseline_برچسب,
		     hb_direction_t  direction,
		     hb_برچسب_t        script_برچسب,
		     hb_برچسب_t        language_برچسب,
		     hb_position_t  *پایه) ثابت
  {
    ثابت پایه_Coord *پایه_coord = اشاره_تهی;
    إذا (unlikely (!get_axis (direction).get_baseline (baseline_برچسب, script_برچسب, language_برچسب, &پایه_coord) ||
		  !پایه_coord || !پایه_coord->has_داده ()))
      أرجع خطأ;

    إذا (likely (پایه))
      *پایه = پایه_coord->get_coord (font, get_var_store (), direction);

    أرجع درست;
  }

  منطقي get_min_max (hb_font_t      *font,
		    hb_direction_t  direction,
		    hb_برچسب_t        script_برچسب,
		    hb_برچسب_t        language_برچسب,
		    hb_برچسب_t        feature_برچسب,
		    hb_position_t  *min,
		    hb_position_t  *max) ثابت
  {
    ثابت پایه_Coord *min_coord, *max_coord;
    إذا (!get_axis (direction).get_min_max (script_برچسب, language_برچسب, feature_برچسب,
					   &min_coord, &max_coord))
      أرجع خطأ;

    ثابت قلم_Variation_Store &var_store = get_var_store ();
    إذا (likely (min && min_coord)) *min = min_coord->get_coord (font, var_store, direction);
    إذا (likely (max && max_coord)) *max = max_coord->get_coord (font, var_store, direction);
    أرجع درست;
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (c->check_struct (هذا) &&
			  hb_barrier () &&
			  likely (نسخه.major == 1) &&
			  hAxis.sanitize (c, هذا) &&
			  vAxis.sanitize (c, هذا) &&
			  (نسخه.مقصد_int () < 0x00010001u || varStore.sanitize (c, هذا))));
  }

  محمي:
  Fixed_نسخه<>نسخه;	/* Version of the BASE table */
  Offset16_مقصد<Axis>hAxis;		/* Offset to horizontal Axis table, from beginning
				 * of BASE table (may be NULL) */
  Offset16_مقصد<Axis>vAxis;		/* Offset to vertical Axis table, from beginning
				 * of BASE table (may be NULL) */
  Offset32_مقصد<قلم_Variation_Store>
		varStore;	/* Offset to the table of Item Variation
				 * Store--from beginning of BASE
				 * header (may be NULL).  Introduced
				 * in version 0x00010001. */
  عام:
  DEFINE_اندازه_MIN (8);
};


} /* namespace OT */


#پایان_اگر /* HB_OT_LAYOUT_BASE_TABLE_HH */
