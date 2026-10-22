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

#که_نه_تعریف_شوی HB_OT_LAYOUT_بنسټ_جدول_HH
#تعریف_کړه HB_OT_LAYOUT_بنسټ_جدول_HH

#ضمن "hb-open-type.hh"
#ضمن "hb-ot-layout-common.hh"

نطاق OT {

/*
 * BASE -- Baseline
 * https://docs.microsoft.com/en-us/typography/opentype/spec/base
 */

بنية بنسټ_Coord_Format1
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

کينډۍ <ډول_نوم Types>
بنية بنسټ_Coord_Format2_4
{
  hb_position_t get_coord (hb_font_t *font, hb_direction_t direction) ثابت
  {
    /* TODO */
    أرجع HB_DIRECTION_IS_HORIZONTAL (direction) ? font->em_scale_y (coordinate) : font->em_scale_x (coordinate);
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    خپلکاری *بهر = c->serializer->embed (*هذا);
    إذا (unlikely (!بهر)) return_trace (خطأ);

    hb_codepoint_t *new_gid;
    إذا (!c->plan->glyph_map->has (referenceGlyph, &new_gid))
      return_trace (خطأ);

    return_trace (c->serializer->check_ټاکل (بهر->referenceGlyph,
                                               *new_gid,
                                               HB_SERIALIZE_ERROR_INT_ډکېدنه));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  محمي:
  HBUINT16	format;		/* Format identifier--format = 2 or 4 */
  FWORD		coordinate;	/* X or Y value, in design units */
  ډول_نوم Types::HBGlyphID
		referenceGlyph;	/* Glyph ID of control glyph */
  HBUINT16	coord_ټکی;	/* Index of contour point on the
				 * reference glyph */
  عام:
  DEFINE_اندازه_STATIC (6 + Types::HBGlyphID::static_اندازه);
};

بنية بنسټ_Coord_Format3
{
  hb_position_t get_coord (hb_font_t *font,
			   ثابت توکی_Variation_Store &var_store,
			   hb_direction_t direction) ثابت
  {
    ثابت Device &device = هذا+device_جدول;

    أرجع HB_DIRECTION_IS_HORIZONTAL (direction)
	 ? hb_saturate_جمع_کړه (font->em_scale_y (coordinate), device.get_y_delta (font, var_store))
	 : hb_saturate_جمع_کړه (font->em_scale_x (coordinate), device.get_x_delta (font, var_store));
  }

  فراغ collect_variation_indices (hb_سېټ_t& varidx_سېټ /*بهر*/) ثابت
  {
    بینښه varidx = (هذا+device_جدول).get_variation_شاخص ();
    varidx_سېټ.جمع_کړه (varidx);
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    خپلکاری *بهر = c->serializer->embed (*هذا);
    إذا (unlikely (!بهر)) return_trace (خطأ);

    إذا (!c->plan->pinned_at_default)
    {
      بینښه var_idx = (هذا+device_جدول).get_variation_شاخص ();
      إذا (var_idx != VarIdx::NO_VARIATION)
      {
        hb_pair_t<بینښه, صحيح> *v;
        إذا (!c->plan->بنسټ_variation_idx_map.has (var_idx, &v))
          return_trace (خطأ);
        
        إذا (unlikely (!c->serializer->check_ټاکل (بهر->coordinate, coordinate + hb_دوهم (*v),
                                                    HB_SERIALIZE_ERROR_INT_ډکېدنه)))
          return_trace (خطأ);
      }
    }
    
    بهر->device_جدول.serialize_copy (c->serializer, device_جدول,
                                     هذا, 0,
                                     hb_serialize_context_t::سر,
                                     &c->plan->بنسټ_variation_idx_map);
    return_trace (ریښتیا);
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
  Offset16_منزل<Device>
		device_جدول;	/* Offset to Device table for X or
				 * Y value, from beginning of
				 * BaseCoord table (may be NULL). */
  عام:
  DEFINE_اندازه_STATIC (6);
};

بنية بنسټ_Coord
{
  منطقي has_معلومات () ثابت { أرجع u.format.v; }

  hb_position_t get_coord (hb_font_t            *font,
			   ثابت توکی_Variation_Store &var_store,
			   hb_direction_t        direction) ثابت
  {
    بدل (u.format.v) {
    حالة 1: hb_barrier (); أرجع u.format1.get_coord (font, direction);
    حالة 2: hb_barrier (); أرجع u.format2.get_coord (font, direction);
    حالة 3: hb_barrier (); أرجع u.format3.get_coord (font, var_store, direction);
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
    حالة 4: hb_barrier (); أرجع u.format4.get_coord (font, direction);
#د_که_پای
    افتراضي:أرجع 0;
    }
  }

  فراغ collect_variation_indices (hb_سېټ_t& varidx_سېټ /*بهر*/) ثابت
  {
    بدل (u.format.v) {
    حالة 3: hb_barrier (); u.format3.collect_variation_indices (varidx_سېټ); أرجع;
    افتراضي:أرجع;
    }
  }

  کينډۍ <ډول_نوم context_t, ډول_نوم ...Ts>
  ډول_نوم context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    إذا (unlikely (!c->may_dispatch (هذا, &u.format.v))) أرجع c->no_dispatch_return_ارزښت ();
    TRACE_DISPATCH (هذا, u.format.v);
    بدل (u.format.v) {
    حالة 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    حالة 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    حالة 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
    حالة 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
#د_که_پای
    افتراضي:return_trace (c->default_return_ارزښت ());
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
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
    حالة 4: hb_barrier (); return_trace (u.format4.sanitize (c));
#د_که_پای
    افتراضي:return_trace (خطأ);
    }
  }

  محمي:
  اتحاد {
  بنية { HBUINT16 v; }	format;
  بنسټ_Coord_Format1	format1;
  بنسټ_Coord_Format2_4<Layout::SmallTypes>
			format2;
  بنسټ_Coord_Format3	format3;
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
  بنسټ_Coord_Format2_4<Layout::MediumTypes>
			format4;
#د_که_پای
  } u;
  عام:
  DEFINE_اندازه_UNION (2, format.v);
};

بنية Feat_Min_Max_ریکارډ
{
  صحيح cmp (hb_نښان_t key) ثابت { أرجع نښان_2.cmp (key); }

  منطقي has_معلومات () ثابت { أرجع نښان_2; }

  hb_نښان_t get_feature_نښان () ثابت { أرجع نښان_2; }

  فراغ get_min_max (ثابت بنسټ_Coord **min, ثابت بنسټ_Coord **max) ثابت
  {
    إذا (likely (min)) *min = &(هذا+minCoord);
    إذا (likely (max)) *max = &(هذا+maxCoord);
  }

  فراغ collect_variation_indices (ثابت hb_subset_plan_t* plan,
                                  ثابت فراغ *بنسټ,
                                  hb_سېټ_t& varidx_سېټ /*بهر*/) ثابت
  {
    إذا (!plan->layout_features.has (نښان_2))
      أرجع;

    (بنسټ+minCoord).collect_variation_indices (varidx_سېټ);
    (بنسټ+maxCoord).collect_variation_indices (varidx_سېټ);
  }

  منطقي subset (hb_subset_context_t *c,
               ثابت فراغ *بنسټ) ثابت
  {
    TRACE_SUBSET (هذا);
    خپلکاری *بهر = c->serializer->embed (*هذا);
    إذا (unlikely (!بهر)) return_trace (خطأ);

    منطقي ret = بهر->minCoord.serialize_subset (c, minCoord, بنسټ);
    ret |= بهر->maxCoord.serialize_subset (c, maxCoord, بنسټ);
    return_trace (ret);
  }

  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *بنسټ) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (c->check_struct (هذا) &&
			  minCoord.sanitize (c, بنسټ) &&
			  maxCoord.sanitize (c, بنسټ)));
  }

  محمي:
  نښان		نښان_2;		/* 4-byte feature identification tag--must
				 * match feature tag in FeatureList */
  Offset16_منزل<بنسټ_Coord>
		minCoord;	/* Offset to BaseCoord table that defines
				 * the minimum extent value, from beginning
				 * of MinMax table (may be NULL) */
  Offset16_منزل<بنسټ_Coord>
		maxCoord;	/* Offset to BaseCoord table that defines
				 * the maximum extent value, from beginning
				 * of MinMax table (may be NULL) */
  عام:
  DEFINE_اندازه_STATIC (8);
};

بنية MinMax
{
  فراغ get_min_max (hb_نښان_t          feature_نښان,
		    ثابت بنسټ_Coord **min,
		    ثابت بنسټ_Coord **max) ثابت
  {
    ثابت Feat_Min_Max_ریکارډ &minMaxCoord = feat_Min_Max_ریکارډونه.bsearch (feature_نښان);
    إذا (minMaxCoord.has_معلومات ())
      minMaxCoord.get_min_max (min, max);
    وإلا
    {
      إذا (likely (min)) *min = &(هذا+minCoord);
      إذا (likely (max)) *max = &(هذا+maxCoord);
    }
  }

  فراغ collect_variation_indices (ثابت hb_subset_plan_t* plan,
                                  hb_سېټ_t& varidx_سېټ /*بهر*/) ثابت
  {
    (هذا+minCoord).collect_variation_indices (varidx_سېټ);
    (هذا+maxCoord).collect_variation_indices (varidx_سېټ);
    لكل (ثابت Feat_Min_Max_ریکارډ& ریکارډ : feat_Min_Max_ریکارډونه)
      ریکارډ.collect_variation_indices (plan, هذا, varidx_سېټ);
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    خپلکاری *بهر = c->serializer->پیل_embed (*هذا);
    إذا (unlikely (!بهر || !c->serializer->extend_min (بهر))) return_trace (خطأ);

    منطقي ret = بهر->minCoord.serialize_subset (c, minCoord, هذا);
    ret |= بهر->maxCoord.serialize_subset (c, maxCoord, هذا);

    بینښه len = 0;
    لكل (ثابت Feat_Min_Max_ریکارډ& _ : feat_Min_Max_ریکارډونه)
    {
      hb_نښان_t feature_نښان = _.get_feature_نښان ();
      إذا (!c->plan->layout_features.has (feature_نښان))
        استمر;

      خپلکاری snap = c->serializer->snapshot ();
      إذا (!_.subset (c, هذا)) c->serializer->revert (snap);
      وإلا len++;
    }

    إذا (len > 0) ret = ریښتیا;
    إذا (!ret) return_trace (خطأ);
    return_trace (c->serializer->check_ټاکل (بهر->feat_Min_Max_ریکارډونه.len, len,
                                               HB_SERIALIZE_ERROR_INT_ډکېدنه));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (c->check_struct (هذا) &&
			  minCoord.sanitize (c, هذا) &&
			  maxCoord.sanitize (c, هذا) &&
			  feat_Min_Max_ریکارډونه.sanitize (c, هذا)));
  }

  محمي:
  Offset16_منزل<بنسټ_Coord>
		minCoord;	/* Offset to BaseCoord table that defines
				 * minimum extent value, from the beginning
				 * of MinMax table (may be NULL) */
  Offset16_منزل<بنسټ_Coord>
		maxCoord;	/* Offset to BaseCoord table that defines
				 * maximum extent value, from the beginning
				 * of MinMax table (may be NULL) */
  Sorted_Array16_د<Feat_Min_Max_ریکارډ>
		feat_Min_Max_ریکارډونه;
				/* Array of FeatMinMaxRecords, in alphabetical
				 * order by featureTableTag */
  عام:
  DEFINE_اندازه_ARRAY (6, feat_Min_Max_ریکارډونه);
};

بنية بنسټ_ارزښتونه
{
  ثابت بنسټ_Coord &get_بنسټ_coord (صحيح baseline_نښان_شاخص) ثابت
  {
    إذا (baseline_نښان_شاخص == -1) baseline_نښان_شاخص = default_شاخص_2;
    أرجع هذا+بنسټ_Coords[baseline_نښان_شاخص];
  }

  فراغ collect_variation_indices (hb_سېټ_t& varidx_سېټ /*بهر*/) ثابت
  {
    لكل (ثابت خپلکاری& _ : بنسټ_Coords)
      (هذا+_).collect_variation_indices (varidx_سېټ);
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    خپلکاری *بهر = c->serializer->پیل_embed (*هذا);
    إذا (unlikely (!بهر || !c->serializer->extend_min (بهر))) return_trace (خطأ);
    بهر->default_شاخص_2 = default_شاخص_2;

    منطقي ret = خطأ;
    لكل (ثابت خپلکاری& _ : بنسټ_Coords)
    {
      خپلکاری *o = بهر->بنسټ_Coords.serialize_append (c->serializer);
      إذا (unlikely (!o)) return_trace (خطأ);
       ret |= o->serialize_subset (c, _, هذا);
    }

    return_trace (ret);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (c->check_struct (هذا) &&
			  بنسټ_Coords.sanitize (c, هذا)));
  }

  محمي:
  شاخص		default_شاخص_2;	/* Index number of default baseline for this
				 * script — equals index position of baseline tag
				 * in baselineTags array of the BaseTagList */
  Array16_د_Offset16_منزل<بنسټ_Coord>
		بنسټ_Coords;	/* Number of BaseCoord tables defined — should equal
				 * baseTagCount in the BaseTagList
				 *
				 * Array of offsets to BaseCoord tables, from beginning of
				 * BaseValues table — order matches baselineTags array in
				 * the BaseTagList */
  عام:
  DEFINE_اندازه_ARRAY (4, بنسټ_Coords);
};

بنية بنسټ_Lang_Sys_ریکارډ
{
  صحيح cmp (hb_نښان_t key) ثابت { أرجع بنسټ_Lang_Sys_نښان.cmp (key); }

  منطقي has_معلومات () ثابت { أرجع بنسټ_Lang_Sys_نښان; }

  ثابت MinMax &get_min_max (ثابت فراغ* بنسټ) ثابت { أرجع بنسټ+minMax; }

  فراغ collect_variation_indices (ثابت فراغ* بنسټ,
                                  ثابت hb_subset_plan_t* plan,
                                  hb_سېټ_t& varidx_سېټ /*بهر*/) ثابت
  { (بنسټ+minMax).collect_variation_indices (plan, varidx_سېټ); }

  منطقي subset (hb_subset_context_t *c,
               ثابت فراغ *بنسټ) ثابت
  {
    TRACE_SUBSET (هذا);
    خپلکاری *بهر = c->serializer->embed (*هذا);
    إذا (unlikely (!بهر)) return_trace (خطأ);

    return_trace (بهر->minMax.serialize_subset (c, minMax, بنسټ));
  }

  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *بنسټ) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (c->check_struct (هذا) &&
			  minMax.sanitize (c, بنسټ)));
  }

  محمي:
  نښان		بنسټ_Lang_Sys_نښان;	/* 4-byte language system identification tag */
  Offset16_منزل<MinMax>
		minMax;		/* Offset to MinMax table, from beginning
				 * of BaseScript table */
  عام:
  DEFINE_اندازه_STATIC (6);
};

بنية بنسټ_Script
{
  ثابت MinMax &get_min_max (hb_نښان_t language_نښان) ثابت
  {
    ثابت بنسټ_Lang_Sys_ریکارډ& ریکارډ = بنسټ_Lang_Sys_ریکارډونه.bsearch (language_نښان);
    أرجع ریکارډ.has_معلومات () ? ریکارډ.get_min_max (هذا) : هذا+defaultMinMax;
  }

  ثابت بنسټ_Coord &get_بنسټ_coord (صحيح baseline_نښان_شاخص) ثابت
  { أرجع (هذا+بنسټ_ارزښتونه_2).get_بنسټ_coord (baseline_نښان_شاخص); }

  منطقي has_ارزښتونه () ثابت { أرجع بنسټ_ارزښتونه_2; }
  منطقي has_min_max () ثابت { أرجع defaultMinMax || بنسټ_Lang_Sys_ریکارډونه; }

  فراغ collect_variation_indices (ثابت hb_subset_plan_t* plan,
                                  hb_سېټ_t& varidx_سېټ /*بهر*/) ثابت
  {
    (هذا+بنسټ_ارزښتونه_2).collect_variation_indices (varidx_سېټ);
    (هذا+defaultMinMax).collect_variation_indices (plan, varidx_سېټ);
    
    لكل (ثابت بنسټ_Lang_Sys_ریکارډ& _ : بنسټ_Lang_Sys_ریکارډونه)
      _.collect_variation_indices (هذا, plan, varidx_سېټ);
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    خپلکاری *بهر = c->serializer->پیل_embed (*هذا);
    إذا (unlikely (!بهر || !c->serializer->extend_min (بهر))) return_trace (خطأ);

    منطقي ret = بهر->بنسټ_ارزښتونه_2.serialize_subset (c, بنسټ_ارزښتونه_2, هذا);
    ret |= بهر->defaultMinMax.serialize_subset (c, defaultMinMax, هذا);

    بینښه len = 0;
    لكل (ثابت خپلکاری& _ : بنسټ_Lang_Sys_ریکارډونه)
    {
      خپلکاری snap = c->serializer->snapshot ();
      إذا (_.subset (c, هذا)) {
        ret = ریښتیا;
        len++;
      }
      وإلا c->serializer->revert (snap);
    }

    إذا (!ret) return_trace (خطأ);
    return_trace (c->serializer->check_ټاکل (بهر->بنسټ_Lang_Sys_ریکارډونه.len, len,
                                               HB_SERIALIZE_ERROR_INT_ډکېدنه));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (c->check_struct (هذا) &&
			  بنسټ_ارزښتونه_2.sanitize (c, هذا) &&
			  defaultMinMax.sanitize (c, هذا) &&
			  بنسټ_Lang_Sys_ریکارډونه.sanitize (c, هذا)));
  }

  محمي:
  Offset16_منزل<بنسټ_ارزښتونه>
		بنسټ_ارزښتونه_2;	/* Offset to BaseValues table, from beginning
				 * of BaseScript table (may be NULL) */
  Offset16_منزل<MinMax>
		defaultMinMax;	/* Offset to MinMax table, from beginning of
				 * BaseScript table (may be NULL) */
  Sorted_Array16_د<بنسټ_Lang_Sys_ریکارډ>
		بنسټ_Lang_Sys_ریکارډونه;
				/* Number of BaseLangSysRecords
				 * defined — may be zero (0) */

  عام:
  DEFINE_اندازه_ARRAY (6, بنسټ_Lang_Sys_ریکارډونه);
};

بنية بنسټ_Script_List;
بنية بنسټ_Script_ریکارډ
{
  صحيح cmp (hb_نښان_t key) ثابت { أرجع بنسټ_Script_نښان.cmp (key); }

  منطقي has_معلومات () ثابت { أرجع بنسټ_Script_نښان; }

  hb_نښان_t get_script_نښان () ثابت { أرجع بنسټ_Script_نښان; }

  ثابت بنسټ_Script &get_بنسټ_script (ثابت بنسټ_Script_List *list) ثابت
  { أرجع list+بنسټ_Script_2; }

  فراغ collect_variation_indices (ثابت hb_subset_plan_t* plan,
                                  ثابت فراغ* list,
                                  hb_سېټ_t& varidx_سېټ /*بهر*/) ثابت
  {
    إذا (!plan->layout_scripts.has (بنسټ_Script_نښان))
      أرجع;

    (list+بنسټ_Script_2).collect_variation_indices (plan, varidx_سېټ);
  }

  منطقي subset (hb_subset_context_t *c,
               ثابت فراغ *بنسټ) ثابت
  {
    TRACE_SUBSET (هذا);
    خپلکاری *بهر = c->serializer->embed (*هذا);
    إذا (unlikely (!بهر)) return_trace (خطأ);

    return_trace (بهر->بنسټ_Script_2.serialize_subset (c, بنسټ_Script_2, بنسټ));
  }

  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *بنسټ) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (c->check_struct (هذا) &&
			  بنسټ_Script_2.sanitize (c, بنسټ)));
  }

  محمي:
  نښان		بنسټ_Script_نښان;	/* 4-byte script identification tag */
  Offset16_منزل<بنسټ_Script>
		بنسټ_Script_2;	/* Offset to BaseScript table, from beginning
				 * of BaseScriptList */

  عام:
  DEFINE_اندازه_STATIC (6);
};

بنية بنسټ_Script_List
{
  ثابت بنسټ_Script &get_بنسټ_script (hb_نښان_t script) ثابت
  {
    ثابت بنسټ_Script_ریکارډ *ریکارډ = &بنسټ_Script_ریکارډونه.bsearch (script);
    إذا (!ریکارډ->has_معلومات ()) ریکارډ = &بنسټ_Script_ریکارډونه.bsearch (HB_نښان ('D','F','L','T'));
    أرجع ریکارډ->has_معلومات () ? ریکارډ->get_بنسټ_script (هذا) : Null (بنسټ_Script);
  }

  فراغ collect_variation_indices (ثابت hb_subset_plan_t* plan,
                                  hb_سېټ_t& varidx_سېټ /*بهر*/) ثابت
  {
    لكل (ثابت بنسټ_Script_ریکارډ& _ : بنسټ_Script_ریکارډونه)
      _.collect_variation_indices (plan, هذا, varidx_سېټ);
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    خپلکاری *بهر = c->serializer->پیل_embed (*هذا);
    إذا (unlikely (!بهر || !c->serializer->extend_min (بهر))) return_trace (خطأ);

    بینښه len = 0;
    لكل (ثابت بنسټ_Script_ریکارډ& _ : بنسټ_Script_ریکارډونه)
    {
      hb_نښان_t script_نښان = _.get_script_نښان ();
      إذا (!c->plan->layout_scripts.has (script_نښان))
        استمر;

      خپلکاری snap = c->serializer->snapshot ();
      إذا (!_.subset (c, هذا)) c->serializer->revert (snap);
      وإلا len++;
    }

    إذا (!len) return_trace (خطأ);
    return_trace (c->serializer->check_ټاکل (بهر->بنسټ_Script_ریکارډونه.len, len,
                                               HB_SERIALIZE_ERROR_INT_ډکېدنه));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  بنسټ_Script_ریکارډونه.sanitize (c, هذا));
  }

  محمي:
  Sorted_Array16_د<بنسټ_Script_ریکارډ>
			بنسټ_Script_ریکارډونه;

  عام:
  DEFINE_اندازه_ARRAY (2, بنسټ_Script_ریکارډونه);
};

بنية Axis
{
  منطقي get_baseline (hb_نښان_t          baseline_نښان,
		     hb_نښان_t          script_نښان,
		     hb_نښان_t          language_نښان,
		     ثابت بنسټ_Coord **coord) ثابت
  {
    ثابت بنسټ_Script &بنسټ_script = (هذا+بنسټ_Script_List_2).get_بنسټ_script (script_نښان);
    إذا (!بنسټ_script.has_ارزښتونه ())
    {
      *coord = تش_اشاره;
      أرجع خطأ;
    }

    إذا (likely (coord))
    {
      بینښه صحيح نښان_شاخص = 0;
      إذا (!(هذا+بنسټ_نښان_List).bfind (baseline_نښان, &نښان_شاخص))
      {
        *coord = تش_اشاره;
        أرجع خطأ;
      }
      *coord = &بنسټ_script.get_بنسټ_coord (نښان_شاخص);
    }

    أرجع ریښتیا;
  }

  منطقي get_min_max (hb_نښان_t          script_نښان,
		    hb_نښان_t          language_نښان,
		    hb_نښان_t          feature_نښان,
		    ثابت بنسټ_Coord **min_coord,
		    ثابت بنسټ_Coord **max_coord) ثابت
  {
    ثابت بنسټ_Script &بنسټ_script = (هذا+بنسټ_Script_List_2).get_بنسټ_script (script_نښان);
    إذا (!بنسټ_script.has_min_max ())
    {
      *min_coord = *max_coord = تش_اشاره;
      أرجع خطأ;
    }

    بنسټ_script.get_min_max (language_نښان).get_min_max (feature_نښان, min_coord, max_coord);

    أرجع ریښتیا;
  }

  فراغ collect_variation_indices (ثابت hb_subset_plan_t* plan,
                                  hb_سېټ_t& varidx_سېټ /*بهر*/) ثابت
  { (هذا+بنسټ_Script_List_2).collect_variation_indices (plan, varidx_سېټ); }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    خپلکاری *بهر = c->serializer->embed (*هذا);
    إذا (unlikely (!بهر)) return_trace (خطأ);

    منطقي ret = بهر->بنسټ_نښان_List.serialize_copy (c->serializer, بنسټ_نښان_List, هذا);
    ret |= بهر->بنسټ_Script_List_2.serialize_subset (c, بنسټ_Script_List_2, هذا);
    return_trace (ret);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (c->check_struct (هذا) &&
			  بنسټ_نښان_List.sanitize (c, هذا) &&
			  بنسټ_Script_List_2.sanitize (c, هذا)));
  }

  محمي:
  Offset16_منزل<Sorted_Array16_د<نښان>>
		بنسټ_نښان_List;	/* Offset to BaseTagList table, from beginning
				 * of Axis table (may be NULL)
				 * Array of 4-byte baseline identification tags — must
				 * be in alphabetical order */
  Offset16_منزل<بنسټ_Script_List>
		بنسټ_Script_List_2;	/* Offset to BaseScriptList table, from beginning
				 * of Axis table
				 * Array of BaseScriptRecords, in alphabetical order
				 * by baseScriptTag */

  عام:
  DEFINE_اندازه_STATIC (4);
};

بنية بنسټ_4
{
  ساكن constexpr hb_نښان_t جدول_نښان = HB_OT_نښان_بنسټ;

  ثابت Axis &get_axis (hb_direction_t direction) ثابت
  { أرجع HB_DIRECTION_IS_VERTICAL (direction) ? هذا+vAxis : هذا+hAxis; }

  منطقي has_var_store () ثابت
  { أرجع نسخه.منزل_int () >= 0x00010001u && varStore != 0; }

  ثابت توکی_Variation_Store &get_var_store () ثابت
  { أرجع نسخه.منزل_int () < 0x00010001u ? Null (توکی_Variation_Store) : هذا+varStore; }

  فراغ collect_variation_indices (ثابت hb_subset_plan_t* plan,
                                  hb_سېټ_t& varidx_سېټ /*بهر*/) ثابت
  {
    (هذا+hAxis).collect_variation_indices (plan, varidx_سېټ);
    (هذا+vAxis).collect_variation_indices (plan, varidx_سېټ);
  }

  منطقي subset_varstore (hb_subset_context_t *c,
                        بنسټ_4 *بهر /*بهر*/) ثابت
  {
    TRACE_SUBSET (هذا);
    إذا (!c->serializer->allocate_اندازه<Offset32_منزل<توکی_Variation_Store>> (Offset32_منزل<توکی_Variation_Store>::static_اندازه))
        return_trace (خطأ);
    إذا (!c->plan->normalized_coords)
      return_trace (بهر->varStore.serialize_subset (c, varStore, هذا, c->plan->بنسټ_varstore_inner_maps.as_array ()));

    إذا (c->plan->all_axes_pinned)
      return_trace (ریښتیا);

    توکی_variations_t توکی_vars;
    إذا (!توکی_vars.instantiate (هذا+varStore, c->plan, ریښتیا, ریښتیا,
                                c->plan->بنسټ_varstore_inner_maps.as_array ()))
      return_trace (خطأ);

    إذا (!بهر->varStore.serialize_serialize (c->serializer,
                                            توکی_vars.has_long_کلمه (),
                                            c->plan->axis_نښانونه,
                                            توکی_vars.get_region_list (),
                                            توکی_vars.get_vardata_encodings ()))
      return_trace (خطأ);

    ثابت hb_map_t &varidx_map = توکی_vars.get_varidx_map ();
    /* base_variation_idx_map in the plan is old_varidx->(varidx, delta)
     * mapping, new varidx is generated for subsetting, we need to remap this
     * after instancing */
    لكل (خپلکاری _ : c->plan->بنسټ_variation_idx_map.iter_ref ())
    {
      uint32_t varidx = _.second.لومړی;
      uint32_t *new_varidx;
      إذا (varidx_map.has (varidx, &new_varidx))
        _.second.لومړی = *new_varidx;
      وإلا
        _.second.لومړی = HB_OT_LAYOUT_NO_VARIATIONS_شاخص;
    }
    return_trace (ریښتیا);
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    خپلکاری *بهر = c->serializer->پیل_embed (*هذا);
    إذا (unlikely (!بهر || !c->serializer->extend_min (بهر))) return_trace (خطأ);

    بهر->نسخه = نسخه;
    منطقي ret = خطأ;
    إذا (نسخه.منزل_int () >= 0x00010001u) ret |= subset_varstore (c, بهر);

    ret |= بهر->hAxis.serialize_subset (c, hAxis, هذا);
    ret |= بهر->vAxis.serialize_subset (c, vAxis, هذا);

    return_trace (ret);
  }

  منطقي get_baseline (hb_font_t      *font,
		     hb_نښان_t        baseline_نښان,
		     hb_direction_t  direction,
		     hb_نښان_t        script_نښان,
		     hb_نښان_t        language_نښان,
		     hb_position_t  *بنسټ) ثابت
  {
    ثابت بنسټ_Coord *بنسټ_coord = تش_اشاره;
    إذا (unlikely (!get_axis (direction).get_baseline (baseline_نښان, script_نښان, language_نښان, &بنسټ_coord) ||
		  !بنسټ_coord || !بنسټ_coord->has_معلومات ()))
      أرجع خطأ;

    إذا (likely (بنسټ))
      *بنسټ = بنسټ_coord->get_coord (font, get_var_store (), direction);

    أرجع ریښتیا;
  }

  منطقي get_min_max (hb_font_t      *font,
		    hb_direction_t  direction,
		    hb_نښان_t        script_نښان,
		    hb_نښان_t        language_نښان,
		    hb_نښان_t        feature_نښان,
		    hb_position_t  *min,
		    hb_position_t  *max) ثابت
  {
    ثابت بنسټ_Coord *min_coord, *max_coord;
    إذا (!get_axis (direction).get_min_max (script_نښان, language_نښان, feature_نښان,
					   &min_coord, &max_coord))
      أرجع خطأ;

    ثابت توکی_Variation_Store &var_store = get_var_store ();
    إذا (likely (min && min_coord)) *min = min_coord->get_coord (font, var_store, direction);
    إذا (likely (max && max_coord)) *max = max_coord->get_coord (font, var_store, direction);
    أرجع ریښتیا;
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (c->check_struct (هذا) &&
			  hb_barrier () &&
			  likely (نسخه.major == 1) &&
			  hAxis.sanitize (c, هذا) &&
			  vAxis.sanitize (c, هذا) &&
			  (نسخه.منزل_int () < 0x00010001u || varStore.sanitize (c, هذا))));
  }

  محمي:
  Fixed_نسخه<>نسخه;	/* Version of the BASE table */
  Offset16_منزل<Axis>hAxis;		/* Offset to horizontal Axis table, from beginning
				 * of BASE table (may be NULL) */
  Offset16_منزل<Axis>vAxis;		/* Offset to vertical Axis table, from beginning
				 * of BASE table (may be NULL) */
  Offset32_منزل<توکی_Variation_Store>
		varStore;	/* Offset to the table of Item Variation
				 * Store--from beginning of BASE
				 * header (may be NULL).  Introduced
				 * in version 0x00010001. */
  عام:
  DEFINE_اندازه_MIN (8);
};


} /* namespace OT */


#د_که_پای /* HB_OT_LAYOUT_BASE_TABLE_HH */
