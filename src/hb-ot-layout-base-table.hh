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

#جے_بنا_تعریف HB_OT_LAYOUT_ادھار_جدول_HH
#تعریف_کرو HB_OT_LAYOUT_ادھار_جدول_HH

#ضمن "hb-open-type.hh"
#ضمن "hb-ot-layout-common.hh"

نطاق OT {

/*
 * BASE -- Baseline
 * https://docs.microsoft.com/en-us/typography/opentype/spec/base
 */

بنية ادھار_Coord_Format1
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
  DEFINE_ناپ_STATIC (4);
};

سانچا <قسم_ناں Types>
بنية ادھار_Coord_Format2_4
{
  hb_position_t get_coord (hb_font_t *font, hb_direction_t direction) ثابت
  {
    /* TODO */
    أرجع HB_DIRECTION_IS_HORIZONTAL (direction) ? font->em_scale_y (coordinate) : font->em_scale_x (coordinate);
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *باہرلا = c->serializer->embed (*هذا);
    إذا (unlikely (!باہرلا)) return_trace (خطأ);

    hb_codepoint_t *new_gid;
    إذا (!c->plan->glyph_map->has (referenceGlyph, &new_gid))
      return_trace (خطأ);

    return_trace (c->serializer->check_سونپو (باہرلا->referenceGlyph,
                                               *new_gid,
                                               HB_SERIALIZE_ERROR_INT_حد_توں_ودھ));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  محمي:
  HBUINT16	format;		/* Format identifier--format = 2 or 4 */
  FWORD		coordinate;	/* X or Y value, in design units */
  قسم_ناں Types::HBGlyphID
		referenceGlyph;	/* Glyph ID of control glyph */
  HBUINT16	coord_نقطہ;	/* Index of contour point on the
				 * reference glyph */
  عام:
  DEFINE_ناپ_STATIC (6 + Types::HBGlyphID::static_ناپ);
};

بنية ادھار_Coord_Format3
{
  hb_position_t get_coord (hb_font_t *font,
			   ثابت شے_Variation_Store &var_store,
			   hb_direction_t direction) ثابت
  {
    ثابت Device &device = هذا+device_جدول;

    أرجع HB_DIRECTION_IS_HORIZONTAL (direction)
	 ? hb_saturate_جوڑو (font->em_scale_y (coordinate), device.get_y_delta (font, var_store))
	 : hb_saturate_جوڑو (font->em_scale_x (coordinate), device.get_x_delta (font, var_store));
  }

  فراغ collect_variation_indices (hb_سیٹ_t& varidx_سیٹ /*باہرلا*/) ثابت
  {
    بنانشانی varidx = (هذا+device_جدول).get_variation_اشاریہ ();
    varidx_سیٹ.جوڑو (varidx);
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *باہرلا = c->serializer->embed (*هذا);
    إذا (unlikely (!باہرلا)) return_trace (خطأ);

    إذا (!c->plan->pinned_at_default)
    {
      بنانشانی var_idx = (هذا+device_جدول).get_variation_اشاریہ ();
      إذا (var_idx != VarIdx::NO_VARIATION)
      {
        hb_pair_t<بنانشانی, صحيح> *v;
        إذا (!c->plan->ادھار_variation_idx_map.has (var_idx, &v))
          return_trace (خطأ);
        
        إذا (unlikely (!c->serializer->check_سونپو (باہرلا->coordinate, coordinate + hb_دوجا (*v),
                                                    HB_SERIALIZE_ERROR_INT_حد_توں_ودھ)))
          return_trace (خطأ);
      }
    }
    
    باہرلا->device_جدول.serialize_copy (c->serializer, device_جدول,
                                     هذا, 0,
                                     hb_serialize_context_t::سر,
                                     &c->plan->ادھار_variation_idx_map);
    return_trace (سچ);
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
  DEFINE_ناپ_STATIC (6);
};

بنية ادھار_Coord
{
  منطقي has_انکڑے () ثابت { أرجع u.format.v; }

  hb_position_t get_coord (hb_font_t            *font,
			   ثابت شے_Variation_Store &var_store,
			   hb_direction_t        direction) ثابت
  {
    بدل (u.format.v) {
    حالة 1: hb_barrier (); أرجع u.format1.get_coord (font, direction);
    حالة 2: hb_barrier (); أرجع u.format2.get_coord (font, direction);
    حالة 3: hb_barrier (); أرجع u.format3.get_coord (font, var_store, direction);
#جے_بنا_تعریف HB_NO_BEYOND_64K
    حالة 4: hb_barrier (); أرجع u.format4.get_coord (font, direction);
#جے_انت
    افتراضي:أرجع 0;
    }
  }

  فراغ collect_variation_indices (hb_سیٹ_t& varidx_سیٹ /*باہرلا*/) ثابت
  {
    بدل (u.format.v) {
    حالة 3: hb_barrier (); u.format3.collect_variation_indices (varidx_سیٹ); أرجع;
    افتراضي:أرجع;
    }
  }

  سانچا <قسم_ناں context_t, قسم_ناں ...Ts>
  قسم_ناں context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    إذا (unlikely (!c->may_dispatch (هذا, &u.format.v))) أرجع c->no_dispatch_return_قدر ();
    TRACE_DISPATCH (هذا, u.format.v);
    بدل (u.format.v) {
    حالة 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    حالة 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    حالة 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
#جے_بنا_تعریف HB_NO_BEYOND_64K
    حالة 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
#جے_انت
    افتراضي:return_trace (c->default_return_قدر ());
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
#جے_بنا_تعریف HB_NO_BEYOND_64K
    حالة 4: hb_barrier (); return_trace (u.format4.sanitize (c));
#جے_انت
    افتراضي:return_trace (خطأ);
    }
  }

  محمي:
  اتحاد {
  بنية { HBUINT16 v; }	format;
  ادھار_Coord_Format1	format1;
  ادھار_Coord_Format2_4<Layout::SmallTypes>
			format2;
  ادھار_Coord_Format3	format3;
#جے_بنا_تعریف HB_NO_BEYOND_64K
  ادھار_Coord_Format2_4<Layout::MediumTypes>
			format4;
#جے_انت
  } u;
  عام:
  DEFINE_ناپ_UNION (2, format.v);
};

بنية Feat_Min_Max_ریکارڈ
{
  صحيح cmp (hb_ٹیگ_t key) ثابت { أرجع ٹیگ_2.cmp (key); }

  منطقي has_انکڑے () ثابت { أرجع ٹیگ_2; }

  hb_ٹیگ_t get_feature_ٹیگ () ثابت { أرجع ٹیگ_2; }

  فراغ get_min_max (ثابت ادھار_Coord **min, ثابت ادھار_Coord **max) ثابت
  {
    إذا (likely (min)) *min = &(هذا+minCoord);
    إذا (likely (max)) *max = &(هذا+maxCoord);
  }

  فراغ collect_variation_indices (ثابت hb_subset_plan_t* plan,
                                  ثابت فراغ *ادھار,
                                  hb_سیٹ_t& varidx_سیٹ /*باہرلا*/) ثابت
  {
    إذا (!plan->layout_features.has (ٹیگ_2))
      أرجع;

    (ادھار+minCoord).collect_variation_indices (varidx_سیٹ);
    (ادھار+maxCoord).collect_variation_indices (varidx_سیٹ);
  }

  منطقي subset (hb_subset_context_t *c,
               ثابت فراغ *ادھار) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *باہرلا = c->serializer->embed (*هذا);
    إذا (unlikely (!باہرلا)) return_trace (خطأ);

    منطقي ret = باہرلا->minCoord.serialize_subset (c, minCoord, ادھار);
    ret |= باہرلا->maxCoord.serialize_subset (c, maxCoord, ادھار);
    return_trace (ret);
  }

  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *ادھار) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (c->check_struct (هذا) &&
			  minCoord.sanitize (c, ادھار) &&
			  maxCoord.sanitize (c, ادھار)));
  }

  محمي:
  ٹیگ		ٹیگ_2;		/* 4-byte feature identification tag--must
				 * match feature tag in FeatureList */
  Offset16_منزل<ادھار_Coord>
		minCoord;	/* Offset to BaseCoord table that defines
				 * the minimum extent value, from beginning
				 * of MinMax table (may be NULL) */
  Offset16_منزل<ادھار_Coord>
		maxCoord;	/* Offset to BaseCoord table that defines
				 * the maximum extent value, from beginning
				 * of MinMax table (may be NULL) */
  عام:
  DEFINE_ناپ_STATIC (8);
};

بنية MinMax
{
  فراغ get_min_max (hb_ٹیگ_t          feature_ٹیگ,
		    ثابت ادھار_Coord **min,
		    ثابت ادھار_Coord **max) ثابت
  {
    ثابت Feat_Min_Max_ریکارڈ &minMaxCoord = feat_Min_Max_ریکارڈاں.bsearch (feature_ٹیگ);
    إذا (minMaxCoord.has_انکڑے ())
      minMaxCoord.get_min_max (min, max);
    وإلا
    {
      إذا (likely (min)) *min = &(هذا+minCoord);
      إذا (likely (max)) *max = &(هذا+maxCoord);
    }
  }

  فراغ collect_variation_indices (ثابت hb_subset_plan_t* plan,
                                  hb_سیٹ_t& varidx_سیٹ /*باہرلا*/) ثابت
  {
    (هذا+minCoord).collect_variation_indices (varidx_سیٹ);
    (هذا+maxCoord).collect_variation_indices (varidx_سیٹ);
    لكل (ثابت Feat_Min_Max_ریکارڈ& ریکارڈ : feat_Min_Max_ریکارڈاں)
      ریکارڈ.collect_variation_indices (plan, هذا, varidx_سیٹ);
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *باہرلا = c->serializer->شروع_embed (*هذا);
    إذا (unlikely (!باہرلا || !c->serializer->extend_min (باہرلا))) return_trace (خطأ);

    منطقي ret = باہرلا->minCoord.serialize_subset (c, minCoord, هذا);
    ret |= باہرلا->maxCoord.serialize_subset (c, maxCoord, هذا);

    بنانشانی len = 0;
    لكل (ثابت Feat_Min_Max_ریکارڈ& _ : feat_Min_Max_ریکارڈاں)
    {
      hb_ٹیگ_t feature_ٹیگ = _.get_feature_ٹیگ ();
      إذا (!c->plan->layout_features.has (feature_ٹیگ))
        استمر;

      خودکار snap = c->serializer->snapshot ();
      إذا (!_.subset (c, هذا)) c->serializer->revert (snap);
      وإلا len++;
    }

    إذا (len > 0) ret = سچ;
    إذا (!ret) return_trace (خطأ);
    return_trace (c->serializer->check_سونپو (باہرلا->feat_Min_Max_ریکارڈاں.len, len,
                                               HB_SERIALIZE_ERROR_INT_حد_توں_ودھ));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (c->check_struct (هذا) &&
			  minCoord.sanitize (c, هذا) &&
			  maxCoord.sanitize (c, هذا) &&
			  feat_Min_Max_ریکارڈاں.sanitize (c, هذا)));
  }

  محمي:
  Offset16_منزل<ادھار_Coord>
		minCoord;	/* Offset to BaseCoord table that defines
				 * minimum extent value, from the beginning
				 * of MinMax table (may be NULL) */
  Offset16_منزل<ادھار_Coord>
		maxCoord;	/* Offset to BaseCoord table that defines
				 * maximum extent value, from the beginning
				 * of MinMax table (may be NULL) */
  Sorted_Array16_دا<Feat_Min_Max_ریکارڈ>
		feat_Min_Max_ریکارڈاں;
				/* Array of FeatMinMaxRecords, in alphabetical
				 * order by featureTableTag */
  عام:
  DEFINE_ناپ_ARRAY (6, feat_Min_Max_ریکارڈاں);
};

بنية ادھار_اقدار
{
  ثابت ادھار_Coord &get_ادھار_coord (صحيح baseline_ٹیگ_اشاریہ) ثابت
  {
    إذا (baseline_ٹیگ_اشاریہ == -1) baseline_ٹیگ_اشاریہ = default_اشاریہ_2;
    أرجع هذا+ادھار_Coords[baseline_ٹیگ_اشاریہ];
  }

  فراغ collect_variation_indices (hb_سیٹ_t& varidx_سیٹ /*باہرلا*/) ثابت
  {
    لكل (ثابت خودکار& _ : ادھار_Coords)
      (هذا+_).collect_variation_indices (varidx_سیٹ);
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *باہرلا = c->serializer->شروع_embed (*هذا);
    إذا (unlikely (!باہرلا || !c->serializer->extend_min (باہرلا))) return_trace (خطأ);
    باہرلا->default_اشاریہ_2 = default_اشاریہ_2;

    منطقي ret = خطأ;
    لكل (ثابت خودکار& _ : ادھار_Coords)
    {
      خودکار *o = باہرلا->ادھار_Coords.serialize_append (c->serializer);
      إذا (unlikely (!o)) return_trace (خطأ);
       ret |= o->serialize_subset (c, _, هذا);
    }

    return_trace (ret);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (c->check_struct (هذا) &&
			  ادھار_Coords.sanitize (c, هذا)));
  }

  محمي:
  اشاریہ		default_اشاریہ_2;	/* Index number of default baseline for this
				 * script — equals index position of baseline tag
				 * in baselineTags array of the BaseTagList */
  Array16_دا_Offset16_منزل<ادھار_Coord>
		ادھار_Coords;	/* Number of BaseCoord tables defined — should equal
				 * baseTagCount in the BaseTagList
				 *
				 * Array of offsets to BaseCoord tables, from beginning of
				 * BaseValues table — order matches baselineTags array in
				 * the BaseTagList */
  عام:
  DEFINE_ناپ_ARRAY (4, ادھار_Coords);
};

بنية ادھار_Lang_Sys_ریکارڈ
{
  صحيح cmp (hb_ٹیگ_t key) ثابت { أرجع ادھار_Lang_Sys_ٹیگ.cmp (key); }

  منطقي has_انکڑے () ثابت { أرجع ادھار_Lang_Sys_ٹیگ; }

  ثابت MinMax &get_min_max (ثابت فراغ* ادھار) ثابت { أرجع ادھار+minMax; }

  فراغ collect_variation_indices (ثابت فراغ* ادھار,
                                  ثابت hb_subset_plan_t* plan,
                                  hb_سیٹ_t& varidx_سیٹ /*باہرلا*/) ثابت
  { (ادھار+minMax).collect_variation_indices (plan, varidx_سیٹ); }

  منطقي subset (hb_subset_context_t *c,
               ثابت فراغ *ادھار) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *باہرلا = c->serializer->embed (*هذا);
    إذا (unlikely (!باہرلا)) return_trace (خطأ);

    return_trace (باہرلا->minMax.serialize_subset (c, minMax, ادھار));
  }

  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *ادھار) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (c->check_struct (هذا) &&
			  minMax.sanitize (c, ادھار)));
  }

  محمي:
  ٹیگ		ادھار_Lang_Sys_ٹیگ;	/* 4-byte language system identification tag */
  Offset16_منزل<MinMax>
		minMax;		/* Offset to MinMax table, from beginning
				 * of BaseScript table */
  عام:
  DEFINE_ناپ_STATIC (6);
};

بنية ادھار_Script
{
  ثابت MinMax &get_min_max (hb_ٹیگ_t language_ٹیگ) ثابت
  {
    ثابت ادھار_Lang_Sys_ریکارڈ& ریکارڈ = ادھار_Lang_Sys_ریکارڈاں.bsearch (language_ٹیگ);
    أرجع ریکارڈ.has_انکڑے () ? ریکارڈ.get_min_max (هذا) : هذا+defaultMinMax;
  }

  ثابت ادھار_Coord &get_ادھار_coord (صحيح baseline_ٹیگ_اشاریہ) ثابت
  { أرجع (هذا+ادھار_اقدار_2).get_ادھار_coord (baseline_ٹیگ_اشاریہ); }

  منطقي has_اقدار () ثابت { أرجع ادھار_اقدار_2; }
  منطقي has_min_max () ثابت { أرجع defaultMinMax || ادھار_Lang_Sys_ریکارڈاں; }

  فراغ collect_variation_indices (ثابت hb_subset_plan_t* plan,
                                  hb_سیٹ_t& varidx_سیٹ /*باہرلا*/) ثابت
  {
    (هذا+ادھار_اقدار_2).collect_variation_indices (varidx_سیٹ);
    (هذا+defaultMinMax).collect_variation_indices (plan, varidx_سیٹ);
    
    لكل (ثابت ادھار_Lang_Sys_ریکارڈ& _ : ادھار_Lang_Sys_ریکارڈاں)
      _.collect_variation_indices (هذا, plan, varidx_سیٹ);
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *باہرلا = c->serializer->شروع_embed (*هذا);
    إذا (unlikely (!باہرلا || !c->serializer->extend_min (باہرلا))) return_trace (خطأ);

    منطقي ret = باہرلا->ادھار_اقدار_2.serialize_subset (c, ادھار_اقدار_2, هذا);
    ret |= باہرلا->defaultMinMax.serialize_subset (c, defaultMinMax, هذا);

    بنانشانی len = 0;
    لكل (ثابت خودکار& _ : ادھار_Lang_Sys_ریکارڈاں)
    {
      خودکار snap = c->serializer->snapshot ();
      إذا (_.subset (c, هذا)) {
        ret = سچ;
        len++;
      }
      وإلا c->serializer->revert (snap);
    }

    إذا (!ret) return_trace (خطأ);
    return_trace (c->serializer->check_سونپو (باہرلا->ادھار_Lang_Sys_ریکارڈاں.len, len,
                                               HB_SERIALIZE_ERROR_INT_حد_توں_ودھ));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (c->check_struct (هذا) &&
			  ادھار_اقدار_2.sanitize (c, هذا) &&
			  defaultMinMax.sanitize (c, هذا) &&
			  ادھار_Lang_Sys_ریکارڈاں.sanitize (c, هذا)));
  }

  محمي:
  Offset16_منزل<ادھار_اقدار>
		ادھار_اقدار_2;	/* Offset to BaseValues table, from beginning
				 * of BaseScript table (may be NULL) */
  Offset16_منزل<MinMax>
		defaultMinMax;	/* Offset to MinMax table, from beginning of
				 * BaseScript table (may be NULL) */
  Sorted_Array16_دا<ادھار_Lang_Sys_ریکارڈ>
		ادھار_Lang_Sys_ریکارڈاں;
				/* Number of BaseLangSysRecords
				 * defined — may be zero (0) */

  عام:
  DEFINE_ناپ_ARRAY (6, ادھار_Lang_Sys_ریکارڈاں);
};

بنية ادھار_Script_List;
بنية ادھار_Script_ریکارڈ
{
  صحيح cmp (hb_ٹیگ_t key) ثابت { أرجع ادھار_Script_ٹیگ.cmp (key); }

  منطقي has_انکڑے () ثابت { أرجع ادھار_Script_ٹیگ; }

  hb_ٹیگ_t get_script_ٹیگ () ثابت { أرجع ادھار_Script_ٹیگ; }

  ثابت ادھار_Script &get_ادھار_script (ثابت ادھار_Script_List *list) ثابت
  { أرجع list+ادھار_Script_2; }

  فراغ collect_variation_indices (ثابت hb_subset_plan_t* plan,
                                  ثابت فراغ* list,
                                  hb_سیٹ_t& varidx_سیٹ /*باہرلا*/) ثابت
  {
    إذا (!plan->layout_scripts.has (ادھار_Script_ٹیگ))
      أرجع;

    (list+ادھار_Script_2).collect_variation_indices (plan, varidx_سیٹ);
  }

  منطقي subset (hb_subset_context_t *c,
               ثابت فراغ *ادھار) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *باہرلا = c->serializer->embed (*هذا);
    إذا (unlikely (!باہرلا)) return_trace (خطأ);

    return_trace (باہرلا->ادھار_Script_2.serialize_subset (c, ادھار_Script_2, ادھار));
  }

  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *ادھار) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (c->check_struct (هذا) &&
			  ادھار_Script_2.sanitize (c, ادھار)));
  }

  محمي:
  ٹیگ		ادھار_Script_ٹیگ;	/* 4-byte script identification tag */
  Offset16_منزل<ادھار_Script>
		ادھار_Script_2;	/* Offset to BaseScript table, from beginning
				 * of BaseScriptList */

  عام:
  DEFINE_ناپ_STATIC (6);
};

بنية ادھار_Script_List
{
  ثابت ادھار_Script &get_ادھار_script (hb_ٹیگ_t script) ثابت
  {
    ثابت ادھار_Script_ریکارڈ *ریکارڈ = &ادھار_Script_ریکارڈاں.bsearch (script);
    إذا (!ریکارڈ->has_انکڑے ()) ریکارڈ = &ادھار_Script_ریکارڈاں.bsearch (HB_ٹیگ ('D','F','L','T'));
    أرجع ریکارڈ->has_انکڑے () ? ریکارڈ->get_ادھار_script (هذا) : Null (ادھار_Script);
  }

  فراغ collect_variation_indices (ثابت hb_subset_plan_t* plan,
                                  hb_سیٹ_t& varidx_سیٹ /*باہرلا*/) ثابت
  {
    لكل (ثابت ادھار_Script_ریکارڈ& _ : ادھار_Script_ریکارڈاں)
      _.collect_variation_indices (plan, هذا, varidx_سیٹ);
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *باہرلا = c->serializer->شروع_embed (*هذا);
    إذا (unlikely (!باہرلا || !c->serializer->extend_min (باہرلا))) return_trace (خطأ);

    بنانشانی len = 0;
    لكل (ثابت ادھار_Script_ریکارڈ& _ : ادھار_Script_ریکارڈاں)
    {
      hb_ٹیگ_t script_ٹیگ = _.get_script_ٹیگ ();
      إذا (!c->plan->layout_scripts.has (script_ٹیگ))
        استمر;

      خودکار snap = c->serializer->snapshot ();
      إذا (!_.subset (c, هذا)) c->serializer->revert (snap);
      وإلا len++;
    }

    إذا (!len) return_trace (خطأ);
    return_trace (c->serializer->check_سونپو (باہرلا->ادھار_Script_ریکارڈاں.len, len,
                                               HB_SERIALIZE_ERROR_INT_حد_توں_ودھ));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  ادھار_Script_ریکارڈاں.sanitize (c, هذا));
  }

  محمي:
  Sorted_Array16_دا<ادھار_Script_ریکارڈ>
			ادھار_Script_ریکارڈاں;

  عام:
  DEFINE_ناپ_ARRAY (2, ادھار_Script_ریکارڈاں);
};

بنية Axis
{
  منطقي get_baseline (hb_ٹیگ_t          baseline_ٹیگ,
		     hb_ٹیگ_t          script_ٹیگ,
		     hb_ٹیگ_t          language_ٹیگ,
		     ثابت ادھار_Coord **coord) ثابت
  {
    ثابت ادھار_Script &ادھار_script = (هذا+ادھار_Script_List_2).get_ادھار_script (script_ٹیگ);
    إذا (!ادھار_script.has_اقدار ())
    {
      *coord = خالی_سوچک;
      أرجع خطأ;
    }

    إذا (likely (coord))
    {
      بنانشانی صحيح ٹیگ_اشاریہ = 0;
      إذا (!(هذا+ادھار_ٹیگ_List).bfind (baseline_ٹیگ, &ٹیگ_اشاریہ))
      {
        *coord = خالی_سوچک;
        أرجع خطأ;
      }
      *coord = &ادھار_script.get_ادھار_coord (ٹیگ_اشاریہ);
    }

    أرجع سچ;
  }

  منطقي get_min_max (hb_ٹیگ_t          script_ٹیگ,
		    hb_ٹیگ_t          language_ٹیگ,
		    hb_ٹیگ_t          feature_ٹیگ,
		    ثابت ادھار_Coord **min_coord,
		    ثابت ادھار_Coord **max_coord) ثابت
  {
    ثابت ادھار_Script &ادھار_script = (هذا+ادھار_Script_List_2).get_ادھار_script (script_ٹیگ);
    إذا (!ادھار_script.has_min_max ())
    {
      *min_coord = *max_coord = خالی_سوچک;
      أرجع خطأ;
    }

    ادھار_script.get_min_max (language_ٹیگ).get_min_max (feature_ٹیگ, min_coord, max_coord);

    أرجع سچ;
  }

  فراغ collect_variation_indices (ثابت hb_subset_plan_t* plan,
                                  hb_سیٹ_t& varidx_سیٹ /*باہرلا*/) ثابت
  { (هذا+ادھار_Script_List_2).collect_variation_indices (plan, varidx_سیٹ); }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *باہرلا = c->serializer->embed (*هذا);
    إذا (unlikely (!باہرلا)) return_trace (خطأ);

    منطقي ret = باہرلا->ادھار_ٹیگ_List.serialize_copy (c->serializer, ادھار_ٹیگ_List, هذا);
    ret |= باہرلا->ادھار_Script_List_2.serialize_subset (c, ادھار_Script_List_2, هذا);
    return_trace (ret);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (c->check_struct (هذا) &&
			  ادھار_ٹیگ_List.sanitize (c, هذا) &&
			  ادھار_Script_List_2.sanitize (c, هذا)));
  }

  محمي:
  Offset16_منزل<Sorted_Array16_دا<ٹیگ>>
		ادھار_ٹیگ_List;	/* Offset to BaseTagList table, from beginning
				 * of Axis table (may be NULL)
				 * Array of 4-byte baseline identification tags — must
				 * be in alphabetical order */
  Offset16_منزل<ادھار_Script_List>
		ادھار_Script_List_2;	/* Offset to BaseScriptList table, from beginning
				 * of Axis table
				 * Array of BaseScriptRecords, in alphabetical order
				 * by baseScriptTag */

  عام:
  DEFINE_ناپ_STATIC (4);
};

بنية ادھار_4
{
  ساكن constexpr hb_ٹیگ_t جدول_ٹیگ = HB_OT_ٹیگ_ادھار;

  ثابت Axis &get_axis (hb_direction_t direction) ثابت
  { أرجع HB_DIRECTION_IS_VERTICAL (direction) ? هذا+vAxis : هذا+hAxis; }

  منطقي has_var_store () ثابت
  { أرجع نسخہ.منزل_int () >= 0x00010001u && varStore != 0; }

  ثابت شے_Variation_Store &get_var_store () ثابت
  { أرجع نسخہ.منزل_int () < 0x00010001u ? Null (شے_Variation_Store) : هذا+varStore; }

  فراغ collect_variation_indices (ثابت hb_subset_plan_t* plan,
                                  hb_سیٹ_t& varidx_سیٹ /*باہرلا*/) ثابت
  {
    (هذا+hAxis).collect_variation_indices (plan, varidx_سیٹ);
    (هذا+vAxis).collect_variation_indices (plan, varidx_سیٹ);
  }

  منطقي subset_varstore (hb_subset_context_t *c,
                        ادھار_4 *باہرلا /*باہرلا*/) ثابت
  {
    TRACE_SUBSET (هذا);
    إذا (!c->serializer->allocate_ناپ<Offset32_منزل<شے_Variation_Store>> (Offset32_منزل<شے_Variation_Store>::static_ناپ))
        return_trace (خطأ);
    إذا (!c->plan->normalized_coords)
      return_trace (باہرلا->varStore.serialize_subset (c, varStore, هذا, c->plan->ادھار_varstore_inner_maps.as_array ()));

    إذا (c->plan->all_axes_pinned)
      return_trace (سچ);

    شے_variations_t شے_vars;
    إذا (!شے_vars.instantiate (هذا+varStore, c->plan, سچ, سچ,
                                c->plan->ادھار_varstore_inner_maps.as_array ()))
      return_trace (خطأ);

    إذا (!باہرلا->varStore.serialize_serialize (c->serializer,
                                            شے_vars.has_long_لفظ (),
                                            c->plan->axis_ٹیگ_سوچی,
                                            شے_vars.get_region_list (),
                                            شے_vars.get_vardata_encodings ()))
      return_trace (خطأ);

    ثابت hb_map_t &varidx_map = شے_vars.get_varidx_map ();
    /* base_variation_idx_map in the plan is old_varidx->(varidx, delta)
     * mapping, new varidx is generated for subsetting, we need to remap this
     * after instancing */
    لكل (خودکار _ : c->plan->ادھار_variation_idx_map.iter_ref ())
    {
      uint32_t varidx = _.second.پہلا;
      uint32_t *new_varidx;
      إذا (varidx_map.has (varidx, &new_varidx))
        _.second.پہلا = *new_varidx;
      وإلا
        _.second.پہلا = HB_OT_LAYOUT_NO_VARIATIONS_اشاریہ;
    }
    return_trace (سچ);
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *باہرلا = c->serializer->شروع_embed (*هذا);
    إذا (unlikely (!باہرلا || !c->serializer->extend_min (باہرلا))) return_trace (خطأ);

    باہرلا->نسخہ = نسخہ;
    منطقي ret = خطأ;
    إذا (نسخہ.منزل_int () >= 0x00010001u) ret |= subset_varstore (c, باہرلا);

    ret |= باہرلا->hAxis.serialize_subset (c, hAxis, هذا);
    ret |= باہرلا->vAxis.serialize_subset (c, vAxis, هذا);

    return_trace (ret);
  }

  منطقي get_baseline (hb_font_t      *font,
		     hb_ٹیگ_t        baseline_ٹیگ,
		     hb_direction_t  direction,
		     hb_ٹیگ_t        script_ٹیگ,
		     hb_ٹیگ_t        language_ٹیگ,
		     hb_position_t  *ادھار) ثابت
  {
    ثابت ادھار_Coord *ادھار_coord = خالی_سوچک;
    إذا (unlikely (!get_axis (direction).get_baseline (baseline_ٹیگ, script_ٹیگ, language_ٹیگ, &ادھار_coord) ||
		  !ادھار_coord || !ادھار_coord->has_انکڑے ()))
      أرجع خطأ;

    إذا (likely (ادھار))
      *ادھار = ادھار_coord->get_coord (font, get_var_store (), direction);

    أرجع سچ;
  }

  منطقي get_min_max (hb_font_t      *font,
		    hb_direction_t  direction,
		    hb_ٹیگ_t        script_ٹیگ,
		    hb_ٹیگ_t        language_ٹیگ,
		    hb_ٹیگ_t        feature_ٹیگ,
		    hb_position_t  *min,
		    hb_position_t  *max) ثابت
  {
    ثابت ادھار_Coord *min_coord, *max_coord;
    إذا (!get_axis (direction).get_min_max (script_ٹیگ, language_ٹیگ, feature_ٹیگ,
					   &min_coord, &max_coord))
      أرجع خطأ;

    ثابت شے_Variation_Store &var_store = get_var_store ();
    إذا (likely (min && min_coord)) *min = min_coord->get_coord (font, var_store, direction);
    إذا (likely (max && max_coord)) *max = max_coord->get_coord (font, var_store, direction);
    أرجع سچ;
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (c->check_struct (هذا) &&
			  hb_barrier () &&
			  likely (نسخہ.major == 1) &&
			  hAxis.sanitize (c, هذا) &&
			  vAxis.sanitize (c, هذا) &&
			  (نسخہ.منزل_int () < 0x00010001u || varStore.sanitize (c, هذا))));
  }

  محمي:
  Fixed_نسخہ<>نسخہ;	/* Version of the BASE table */
  Offset16_منزل<Axis>hAxis;		/* Offset to horizontal Axis table, from beginning
				 * of BASE table (may be NULL) */
  Offset16_منزل<Axis>vAxis;		/* Offset to vertical Axis table, from beginning
				 * of BASE table (may be NULL) */
  Offset32_منزل<شے_Variation_Store>
		varStore;	/* Offset to the table of Item Variation
				 * Store--from beginning of BASE
				 * header (may be NULL).  Introduced
				 * in version 0x00010001. */
  عام:
  DEFINE_ناپ_MIN (8);
};


} /* namespace OT */


#جے_انت /* HB_OT_LAYOUT_BASE_TABLE_HH */
