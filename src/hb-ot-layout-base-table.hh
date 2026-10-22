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

#جيڪڏهن_وصف_نه_ٿيل HB_OT_LAYOUT_بنياد_جدول_HH
#وصف_ڪريو HB_OT_LAYOUT_بنياد_جدول_HH

#ضمن "hb-open-type.hh"
#ضمن "hb-ot-layout-common.hh"

نطاق OT {

/*
 * BASE -- Baseline
 * https://docs.microsoft.com/en-us/typography/opentype/spec/base
 */

بنية بنياد_Coord_Format1
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
  DEFINE_سائيز_STATIC (4);
};

سانچو <قسم_نالو Types>
بنية بنياد_Coord_Format2_4
{
  hb_position_t get_coord (hb_font_t *font, hb_direction_t direction) ثابت
  {
    /* TODO */
    أرجع HB_DIRECTION_IS_HORIZONTAL (direction) ? font->em_scale_y (coordinate) : font->em_scale_x (coordinate);
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    خودڪار *ٻاهريون = c->serializer->embed (*هذا);
    إذا (unlikely (!ٻاهريون)) return_trace (خطأ);

    hb_codepoint_t *new_gid;
    إذا (!c->plan->glyph_map->has (referenceGlyph, &new_gid))
      return_trace (خطأ);

    return_trace (c->serializer->check_سونپيو (ٻاهريون->referenceGlyph,
                                               *new_gid,
                                               HB_SERIALIZE_ERROR_INT_حد_کان_مٿي));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  محمي:
  HBUINT16	format;		/* Format identifier--format = 2 or 4 */
  FWORD		coordinate;	/* X or Y value, in design units */
  قسم_نالو Types::HBGlyphID
		referenceGlyph;	/* Glyph ID of control glyph */
  HBUINT16	coord_نقطو;	/* Index of contour point on the
				 * reference glyph */
  عام:
  DEFINE_سائيز_STATIC (6 + Types::HBGlyphID::static_سائيز);
};

بنية بنياد_Coord_Format3
{
  hb_position_t get_coord (hb_font_t *font,
			   ثابت شي_Variation_Store &var_store,
			   hb_direction_t direction) ثابت
  {
    ثابت Device &device = هذا+device_جدول;

    أرجع HB_DIRECTION_IS_HORIZONTAL (direction)
	 ? hb_saturate_جوڙيو (font->em_scale_y (coordinate), device.get_y_delta (font, var_store))
	 : hb_saturate_جوڙيو (font->em_scale_x (coordinate), device.get_x_delta (font, var_store));
  }

  فراغ collect_variation_indices (hb_سيٽ_t& varidx_سيٽ /*ٻاهريون*/) ثابت
  {
    بنانشان varidx = (هذا+device_جدول).get_variation_اشاريو ();
    varidx_سيٽ.جوڙيو (varidx);
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    خودڪار *ٻاهريون = c->serializer->embed (*هذا);
    إذا (unlikely (!ٻاهريون)) return_trace (خطأ);

    إذا (!c->plan->pinned_at_default)
    {
      بنانشان var_idx = (هذا+device_جدول).get_variation_اشاريو ();
      إذا (var_idx != VarIdx::NO_VARIATION)
      {
        hb_pair_t<بنانشان, صحيح> *v;
        إذا (!c->plan->بنياد_variation_idx_map.has (var_idx, &v))
          return_trace (خطأ);
        
        إذا (unlikely (!c->serializer->check_سونپيو (ٻاهريون->coordinate, coordinate + hb_ٻيو (*v),
                                                    HB_SERIALIZE_ERROR_INT_حد_کان_مٿي)))
          return_trace (خطأ);
      }
    }
    
    ٻاهريون->device_جدول.serialize_copy (c->serializer, device_جدول,
                                     هذا, 0,
                                     hb_serialize_context_t::سر,
                                     &c->plan->بنياد_variation_idx_map);
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
  DEFINE_سائيز_STATIC (6);
};

بنية بنياد_Coord
{
  منطقي has_معلومات () ثابت { أرجع u.format.v; }

  hb_position_t get_coord (hb_font_t            *font,
			   ثابت شي_Variation_Store &var_store,
			   hb_direction_t        direction) ثابت
  {
    بدل (u.format.v) {
    حالة 1: hb_barrier (); أرجع u.format1.get_coord (font, direction);
    حالة 2: hb_barrier (); أرجع u.format2.get_coord (font, direction);
    حالة 3: hb_barrier (); أرجع u.format3.get_coord (font, var_store, direction);
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
    حالة 4: hb_barrier (); أرجع u.format4.get_coord (font, direction);
#جيڪڏهن_پورو
    افتراضي:أرجع 0;
    }
  }

  فراغ collect_variation_indices (hb_سيٽ_t& varidx_سيٽ /*ٻاهريون*/) ثابت
  {
    بدل (u.format.v) {
    حالة 3: hb_barrier (); u.format3.collect_variation_indices (varidx_سيٽ); أرجع;
    افتراضي:أرجع;
    }
  }

  سانچو <قسم_نالو context_t, قسم_نالو ...Ts>
  قسم_نالو context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    إذا (unlikely (!c->may_dispatch (هذا, &u.format.v))) أرجع c->no_dispatch_return_قدر ();
    TRACE_DISPATCH (هذا, u.format.v);
    بدل (u.format.v) {
    حالة 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    حالة 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    حالة 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
    حالة 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
#جيڪڏهن_پورو
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
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
    حالة 4: hb_barrier (); return_trace (u.format4.sanitize (c));
#جيڪڏهن_پورو
    افتراضي:return_trace (خطأ);
    }
  }

  محمي:
  اتحاد {
  بنية { HBUINT16 v; }	format;
  بنياد_Coord_Format1	format1;
  بنياد_Coord_Format2_4<Layout::SmallTypes>
			format2;
  بنياد_Coord_Format3	format3;
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
  بنياد_Coord_Format2_4<Layout::MediumTypes>
			format4;
#جيڪڏهن_پورو
  } u;
  عام:
  DEFINE_سائيز_UNION (2, format.v);
};

بنية Feat_Min_Max_رڪارڊ
{
  صحيح cmp (hb_ٽيگ_t key) ثابت { أرجع ٽيگ_2.cmp (key); }

  منطقي has_معلومات () ثابت { أرجع ٽيگ_2; }

  hb_ٽيگ_t get_feature_ٽيگ () ثابت { أرجع ٽيگ_2; }

  فراغ get_min_max (ثابت بنياد_Coord **min, ثابت بنياد_Coord **max) ثابت
  {
    إذا (likely (min)) *min = &(هذا+minCoord);
    إذا (likely (max)) *max = &(هذا+maxCoord);
  }

  فراغ collect_variation_indices (ثابت hb_subset_plan_t* plan,
                                  ثابت فراغ *بنياد,
                                  hb_سيٽ_t& varidx_سيٽ /*ٻاهريون*/) ثابت
  {
    إذا (!plan->layout_features.has (ٽيگ_2))
      أرجع;

    (بنياد+minCoord).collect_variation_indices (varidx_سيٽ);
    (بنياد+maxCoord).collect_variation_indices (varidx_سيٽ);
  }

  منطقي subset (hb_subset_context_t *c,
               ثابت فراغ *بنياد) ثابت
  {
    TRACE_SUBSET (هذا);
    خودڪار *ٻاهريون = c->serializer->embed (*هذا);
    إذا (unlikely (!ٻاهريون)) return_trace (خطأ);

    منطقي ret = ٻاهريون->minCoord.serialize_subset (c, minCoord, بنياد);
    ret |= ٻاهريون->maxCoord.serialize_subset (c, maxCoord, بنياد);
    return_trace (ret);
  }

  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *بنياد) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (c->check_struct (هذا) &&
			  minCoord.sanitize (c, بنياد) &&
			  maxCoord.sanitize (c, بنياد)));
  }

  محمي:
  ٽيگ		ٽيگ_2;		/* 4-byte feature identification tag--must
				 * match feature tag in FeatureList */
  Offset16_منزل<بنياد_Coord>
		minCoord;	/* Offset to BaseCoord table that defines
				 * the minimum extent value, from beginning
				 * of MinMax table (may be NULL) */
  Offset16_منزل<بنياد_Coord>
		maxCoord;	/* Offset to BaseCoord table that defines
				 * the maximum extent value, from beginning
				 * of MinMax table (may be NULL) */
  عام:
  DEFINE_سائيز_STATIC (8);
};

بنية MinMax
{
  فراغ get_min_max (hb_ٽيگ_t          feature_ٽيگ,
		    ثابت بنياد_Coord **min,
		    ثابت بنياد_Coord **max) ثابت
  {
    ثابت Feat_Min_Max_رڪارڊ &minMaxCoord = feat_Min_Max_رڪارڊون.bsearch (feature_ٽيگ);
    إذا (minMaxCoord.has_معلومات ())
      minMaxCoord.get_min_max (min, max);
    وإلا
    {
      إذا (likely (min)) *min = &(هذا+minCoord);
      إذا (likely (max)) *max = &(هذا+maxCoord);
    }
  }

  فراغ collect_variation_indices (ثابت hb_subset_plan_t* plan,
                                  hb_سيٽ_t& varidx_سيٽ /*ٻاهريون*/) ثابت
  {
    (هذا+minCoord).collect_variation_indices (varidx_سيٽ);
    (هذا+maxCoord).collect_variation_indices (varidx_سيٽ);
    لكل (ثابت Feat_Min_Max_رڪارڊ& رڪارڊ : feat_Min_Max_رڪارڊون)
      رڪارڊ.collect_variation_indices (plan, هذا, varidx_سيٽ);
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    خودڪار *ٻاهريون = c->serializer->شروعات_embed (*هذا);
    إذا (unlikely (!ٻاهريون || !c->serializer->extend_min (ٻاهريون))) return_trace (خطأ);

    منطقي ret = ٻاهريون->minCoord.serialize_subset (c, minCoord, هذا);
    ret |= ٻاهريون->maxCoord.serialize_subset (c, maxCoord, هذا);

    بنانشان len = 0;
    لكل (ثابت Feat_Min_Max_رڪارڊ& _ : feat_Min_Max_رڪارڊون)
    {
      hb_ٽيگ_t feature_ٽيگ = _.get_feature_ٽيگ ();
      إذا (!c->plan->layout_features.has (feature_ٽيگ))
        استمر;

      خودڪار snap = c->serializer->snapshot ();
      إذا (!_.subset (c, هذا)) c->serializer->revert (snap);
      وإلا len++;
    }

    إذا (len > 0) ret = سچ;
    إذا (!ret) return_trace (خطأ);
    return_trace (c->serializer->check_سونپيو (ٻاهريون->feat_Min_Max_رڪارڊون.len, len,
                                               HB_SERIALIZE_ERROR_INT_حد_کان_مٿي));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (c->check_struct (هذا) &&
			  minCoord.sanitize (c, هذا) &&
			  maxCoord.sanitize (c, هذا) &&
			  feat_Min_Max_رڪارڊون.sanitize (c, هذا)));
  }

  محمي:
  Offset16_منزل<بنياد_Coord>
		minCoord;	/* Offset to BaseCoord table that defines
				 * minimum extent value, from the beginning
				 * of MinMax table (may be NULL) */
  Offset16_منزل<بنياد_Coord>
		maxCoord;	/* Offset to BaseCoord table that defines
				 * maximum extent value, from the beginning
				 * of MinMax table (may be NULL) */
  Sorted_Array16_جو<Feat_Min_Max_رڪارڊ>
		feat_Min_Max_رڪارڊون;
				/* Array of FeatMinMaxRecords, in alphabetical
				 * order by featureTableTag */
  عام:
  DEFINE_سائيز_ARRAY (6, feat_Min_Max_رڪارڊون);
};

بنية بنياد_قدرون
{
  ثابت بنياد_Coord &get_بنياد_coord (صحيح baseline_ٽيگ_اشاريو) ثابت
  {
    إذا (baseline_ٽيگ_اشاريو == -1) baseline_ٽيگ_اشاريو = default_اشاريو_2;
    أرجع هذا+بنياد_Coords[baseline_ٽيگ_اشاريو];
  }

  فراغ collect_variation_indices (hb_سيٽ_t& varidx_سيٽ /*ٻاهريون*/) ثابت
  {
    لكل (ثابت خودڪار& _ : بنياد_Coords)
      (هذا+_).collect_variation_indices (varidx_سيٽ);
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    خودڪار *ٻاهريون = c->serializer->شروعات_embed (*هذا);
    إذا (unlikely (!ٻاهريون || !c->serializer->extend_min (ٻاهريون))) return_trace (خطأ);
    ٻاهريون->default_اشاريو_2 = default_اشاريو_2;

    منطقي ret = خطأ;
    لكل (ثابت خودڪار& _ : بنياد_Coords)
    {
      خودڪار *o = ٻاهريون->بنياد_Coords.serialize_append (c->serializer);
      إذا (unlikely (!o)) return_trace (خطأ);
       ret |= o->serialize_subset (c, _, هذا);
    }

    return_trace (ret);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (c->check_struct (هذا) &&
			  بنياد_Coords.sanitize (c, هذا)));
  }

  محمي:
  اشاريو		default_اشاريو_2;	/* Index number of default baseline for this
				 * script — equals index position of baseline tag
				 * in baselineTags array of the BaseTagList */
  Array16_جو_Offset16_منزل<بنياد_Coord>
		بنياد_Coords;	/* Number of BaseCoord tables defined — should equal
				 * baseTagCount in the BaseTagList
				 *
				 * Array of offsets to BaseCoord tables, from beginning of
				 * BaseValues table — order matches baselineTags array in
				 * the BaseTagList */
  عام:
  DEFINE_سائيز_ARRAY (4, بنياد_Coords);
};

بنية بنياد_Lang_Sys_رڪارڊ
{
  صحيح cmp (hb_ٽيگ_t key) ثابت { أرجع بنياد_Lang_Sys_ٽيگ.cmp (key); }

  منطقي has_معلومات () ثابت { أرجع بنياد_Lang_Sys_ٽيگ; }

  ثابت MinMax &get_min_max (ثابت فراغ* بنياد) ثابت { أرجع بنياد+minMax; }

  فراغ collect_variation_indices (ثابت فراغ* بنياد,
                                  ثابت hb_subset_plan_t* plan,
                                  hb_سيٽ_t& varidx_سيٽ /*ٻاهريون*/) ثابت
  { (بنياد+minMax).collect_variation_indices (plan, varidx_سيٽ); }

  منطقي subset (hb_subset_context_t *c,
               ثابت فراغ *بنياد) ثابت
  {
    TRACE_SUBSET (هذا);
    خودڪار *ٻاهريون = c->serializer->embed (*هذا);
    إذا (unlikely (!ٻاهريون)) return_trace (خطأ);

    return_trace (ٻاهريون->minMax.serialize_subset (c, minMax, بنياد));
  }

  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *بنياد) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (c->check_struct (هذا) &&
			  minMax.sanitize (c, بنياد)));
  }

  محمي:
  ٽيگ		بنياد_Lang_Sys_ٽيگ;	/* 4-byte language system identification tag */
  Offset16_منزل<MinMax>
		minMax;		/* Offset to MinMax table, from beginning
				 * of BaseScript table */
  عام:
  DEFINE_سائيز_STATIC (6);
};

بنية بنياد_Script
{
  ثابت MinMax &get_min_max (hb_ٽيگ_t language_ٽيگ) ثابت
  {
    ثابت بنياد_Lang_Sys_رڪارڊ& رڪارڊ = بنياد_Lang_Sys_رڪارڊون.bsearch (language_ٽيگ);
    أرجع رڪارڊ.has_معلومات () ? رڪارڊ.get_min_max (هذا) : هذا+defaultMinMax;
  }

  ثابت بنياد_Coord &get_بنياد_coord (صحيح baseline_ٽيگ_اشاريو) ثابت
  { أرجع (هذا+بنياد_قدرون_2).get_بنياد_coord (baseline_ٽيگ_اشاريو); }

  منطقي has_قدرون () ثابت { أرجع بنياد_قدرون_2; }
  منطقي has_min_max () ثابت { أرجع defaultMinMax || بنياد_Lang_Sys_رڪارڊون; }

  فراغ collect_variation_indices (ثابت hb_subset_plan_t* plan,
                                  hb_سيٽ_t& varidx_سيٽ /*ٻاهريون*/) ثابت
  {
    (هذا+بنياد_قدرون_2).collect_variation_indices (varidx_سيٽ);
    (هذا+defaultMinMax).collect_variation_indices (plan, varidx_سيٽ);
    
    لكل (ثابت بنياد_Lang_Sys_رڪارڊ& _ : بنياد_Lang_Sys_رڪارڊون)
      _.collect_variation_indices (هذا, plan, varidx_سيٽ);
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    خودڪار *ٻاهريون = c->serializer->شروعات_embed (*هذا);
    إذا (unlikely (!ٻاهريون || !c->serializer->extend_min (ٻاهريون))) return_trace (خطأ);

    منطقي ret = ٻاهريون->بنياد_قدرون_2.serialize_subset (c, بنياد_قدرون_2, هذا);
    ret |= ٻاهريون->defaultMinMax.serialize_subset (c, defaultMinMax, هذا);

    بنانشان len = 0;
    لكل (ثابت خودڪار& _ : بنياد_Lang_Sys_رڪارڊون)
    {
      خودڪار snap = c->serializer->snapshot ();
      إذا (_.subset (c, هذا)) {
        ret = سچ;
        len++;
      }
      وإلا c->serializer->revert (snap);
    }

    إذا (!ret) return_trace (خطأ);
    return_trace (c->serializer->check_سونپيو (ٻاهريون->بنياد_Lang_Sys_رڪارڊون.len, len,
                                               HB_SERIALIZE_ERROR_INT_حد_کان_مٿي));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (c->check_struct (هذا) &&
			  بنياد_قدرون_2.sanitize (c, هذا) &&
			  defaultMinMax.sanitize (c, هذا) &&
			  بنياد_Lang_Sys_رڪارڊون.sanitize (c, هذا)));
  }

  محمي:
  Offset16_منزل<بنياد_قدرون>
		بنياد_قدرون_2;	/* Offset to BaseValues table, from beginning
				 * of BaseScript table (may be NULL) */
  Offset16_منزل<MinMax>
		defaultMinMax;	/* Offset to MinMax table, from beginning of
				 * BaseScript table (may be NULL) */
  Sorted_Array16_جو<بنياد_Lang_Sys_رڪارڊ>
		بنياد_Lang_Sys_رڪارڊون;
				/* Number of BaseLangSysRecords
				 * defined — may be zero (0) */

  عام:
  DEFINE_سائيز_ARRAY (6, بنياد_Lang_Sys_رڪارڊون);
};

بنية بنياد_Script_List;
بنية بنياد_Script_رڪارڊ
{
  صحيح cmp (hb_ٽيگ_t key) ثابت { أرجع بنياد_Script_ٽيگ.cmp (key); }

  منطقي has_معلومات () ثابت { أرجع بنياد_Script_ٽيگ; }

  hb_ٽيگ_t get_script_ٽيگ () ثابت { أرجع بنياد_Script_ٽيگ; }

  ثابت بنياد_Script &get_بنياد_script (ثابت بنياد_Script_List *list) ثابت
  { أرجع list+بنياد_Script_2; }

  فراغ collect_variation_indices (ثابت hb_subset_plan_t* plan,
                                  ثابت فراغ* list,
                                  hb_سيٽ_t& varidx_سيٽ /*ٻاهريون*/) ثابت
  {
    إذا (!plan->layout_scripts.has (بنياد_Script_ٽيگ))
      أرجع;

    (list+بنياد_Script_2).collect_variation_indices (plan, varidx_سيٽ);
  }

  منطقي subset (hb_subset_context_t *c,
               ثابت فراغ *بنياد) ثابت
  {
    TRACE_SUBSET (هذا);
    خودڪار *ٻاهريون = c->serializer->embed (*هذا);
    إذا (unlikely (!ٻاهريون)) return_trace (خطأ);

    return_trace (ٻاهريون->بنياد_Script_2.serialize_subset (c, بنياد_Script_2, بنياد));
  }

  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *بنياد) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (c->check_struct (هذا) &&
			  بنياد_Script_2.sanitize (c, بنياد)));
  }

  محمي:
  ٽيگ		بنياد_Script_ٽيگ;	/* 4-byte script identification tag */
  Offset16_منزل<بنياد_Script>
		بنياد_Script_2;	/* Offset to BaseScript table, from beginning
				 * of BaseScriptList */

  عام:
  DEFINE_سائيز_STATIC (6);
};

بنية بنياد_Script_List
{
  ثابت بنياد_Script &get_بنياد_script (hb_ٽيگ_t script) ثابت
  {
    ثابت بنياد_Script_رڪارڊ *رڪارڊ = &بنياد_Script_رڪارڊون.bsearch (script);
    إذا (!رڪارڊ->has_معلومات ()) رڪارڊ = &بنياد_Script_رڪارڊون.bsearch (HB_ٽيگ ('D','F','L','T'));
    أرجع رڪارڊ->has_معلومات () ? رڪارڊ->get_بنياد_script (هذا) : Null (بنياد_Script);
  }

  فراغ collect_variation_indices (ثابت hb_subset_plan_t* plan,
                                  hb_سيٽ_t& varidx_سيٽ /*ٻاهريون*/) ثابت
  {
    لكل (ثابت بنياد_Script_رڪارڊ& _ : بنياد_Script_رڪارڊون)
      _.collect_variation_indices (plan, هذا, varidx_سيٽ);
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    خودڪار *ٻاهريون = c->serializer->شروعات_embed (*هذا);
    إذا (unlikely (!ٻاهريون || !c->serializer->extend_min (ٻاهريون))) return_trace (خطأ);

    بنانشان len = 0;
    لكل (ثابت بنياد_Script_رڪارڊ& _ : بنياد_Script_رڪارڊون)
    {
      hb_ٽيگ_t script_ٽيگ = _.get_script_ٽيگ ();
      إذا (!c->plan->layout_scripts.has (script_ٽيگ))
        استمر;

      خودڪار snap = c->serializer->snapshot ();
      إذا (!_.subset (c, هذا)) c->serializer->revert (snap);
      وإلا len++;
    }

    إذا (!len) return_trace (خطأ);
    return_trace (c->serializer->check_سونپيو (ٻاهريون->بنياد_Script_رڪارڊون.len, len,
                                               HB_SERIALIZE_ERROR_INT_حد_کان_مٿي));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  بنياد_Script_رڪارڊون.sanitize (c, هذا));
  }

  محمي:
  Sorted_Array16_جو<بنياد_Script_رڪارڊ>
			بنياد_Script_رڪارڊون;

  عام:
  DEFINE_سائيز_ARRAY (2, بنياد_Script_رڪارڊون);
};

بنية Axis
{
  منطقي get_baseline (hb_ٽيگ_t          baseline_ٽيگ,
		     hb_ٽيگ_t          script_ٽيگ,
		     hb_ٽيگ_t          language_ٽيگ,
		     ثابت بنياد_Coord **coord) ثابت
  {
    ثابت بنياد_Script &بنياد_script = (هذا+بنياد_Script_List_2).get_بنياد_script (script_ٽيگ);
    إذا (!بنياد_script.has_قدرون ())
    {
      *coord = خالي_اشارو;
      أرجع خطأ;
    }

    إذا (likely (coord))
    {
      بنانشان صحيح ٽيگ_اشاريو = 0;
      إذا (!(هذا+بنياد_ٽيگ_List).bfind (baseline_ٽيگ, &ٽيگ_اشاريو))
      {
        *coord = خالي_اشارو;
        أرجع خطأ;
      }
      *coord = &بنياد_script.get_بنياد_coord (ٽيگ_اشاريو);
    }

    أرجع سچ;
  }

  منطقي get_min_max (hb_ٽيگ_t          script_ٽيگ,
		    hb_ٽيگ_t          language_ٽيگ,
		    hb_ٽيگ_t          feature_ٽيگ,
		    ثابت بنياد_Coord **min_coord,
		    ثابت بنياد_Coord **max_coord) ثابت
  {
    ثابت بنياد_Script &بنياد_script = (هذا+بنياد_Script_List_2).get_بنياد_script (script_ٽيگ);
    إذا (!بنياد_script.has_min_max ())
    {
      *min_coord = *max_coord = خالي_اشارو;
      أرجع خطأ;
    }

    بنياد_script.get_min_max (language_ٽيگ).get_min_max (feature_ٽيگ, min_coord, max_coord);

    أرجع سچ;
  }

  فراغ collect_variation_indices (ثابت hb_subset_plan_t* plan,
                                  hb_سيٽ_t& varidx_سيٽ /*ٻاهريون*/) ثابت
  { (هذا+بنياد_Script_List_2).collect_variation_indices (plan, varidx_سيٽ); }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    خودڪار *ٻاهريون = c->serializer->embed (*هذا);
    إذا (unlikely (!ٻاهريون)) return_trace (خطأ);

    منطقي ret = ٻاهريون->بنياد_ٽيگ_List.serialize_copy (c->serializer, بنياد_ٽيگ_List, هذا);
    ret |= ٻاهريون->بنياد_Script_List_2.serialize_subset (c, بنياد_Script_List_2, هذا);
    return_trace (ret);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (c->check_struct (هذا) &&
			  بنياد_ٽيگ_List.sanitize (c, هذا) &&
			  بنياد_Script_List_2.sanitize (c, هذا)));
  }

  محمي:
  Offset16_منزل<Sorted_Array16_جو<ٽيگ>>
		بنياد_ٽيگ_List;	/* Offset to BaseTagList table, from beginning
				 * of Axis table (may be NULL)
				 * Array of 4-byte baseline identification tags — must
				 * be in alphabetical order */
  Offset16_منزل<بنياد_Script_List>
		بنياد_Script_List_2;	/* Offset to BaseScriptList table, from beginning
				 * of Axis table
				 * Array of BaseScriptRecords, in alphabetical order
				 * by baseScriptTag */

  عام:
  DEFINE_سائيز_STATIC (4);
};

بنية بنياد_4
{
  ساكن constexpr hb_ٽيگ_t جدول_ٽيگ = HB_OT_ٽيگ_بنياد;

  ثابت Axis &get_axis (hb_direction_t direction) ثابت
  { أرجع HB_DIRECTION_IS_VERTICAL (direction) ? هذا+vAxis : هذا+hAxis; }

  منطقي has_var_store () ثابت
  { أرجع نسخو.منزل_int () >= 0x00010001u && varStore != 0; }

  ثابت شي_Variation_Store &get_var_store () ثابت
  { أرجع نسخو.منزل_int () < 0x00010001u ? Null (شي_Variation_Store) : هذا+varStore; }

  فراغ collect_variation_indices (ثابت hb_subset_plan_t* plan,
                                  hb_سيٽ_t& varidx_سيٽ /*ٻاهريون*/) ثابت
  {
    (هذا+hAxis).collect_variation_indices (plan, varidx_سيٽ);
    (هذا+vAxis).collect_variation_indices (plan, varidx_سيٽ);
  }

  منطقي subset_varstore (hb_subset_context_t *c,
                        بنياد_4 *ٻاهريون /*ٻاهريون*/) ثابت
  {
    TRACE_SUBSET (هذا);
    إذا (!c->serializer->allocate_سائيز<Offset32_منزل<شي_Variation_Store>> (Offset32_منزل<شي_Variation_Store>::static_سائيز))
        return_trace (خطأ);
    إذا (!c->plan->normalized_coords)
      return_trace (ٻاهريون->varStore.serialize_subset (c, varStore, هذا, c->plan->بنياد_varstore_inner_maps.as_array ()));

    إذا (c->plan->all_axes_pinned)
      return_trace (سچ);

    شي_variations_t شي_vars;
    إذا (!شي_vars.instantiate (هذا+varStore, c->plan, سچ, سچ,
                                c->plan->بنياد_varstore_inner_maps.as_array ()))
      return_trace (خطأ);

    إذا (!ٻاهريون->varStore.serialize_serialize (c->serializer,
                                            شي_vars.has_long_لفظ (),
                                            c->plan->axis_ٽيگ_فهرست,
                                            شي_vars.get_region_list (),
                                            شي_vars.get_vardata_encodings ()))
      return_trace (خطأ);

    ثابت hb_map_t &varidx_map = شي_vars.get_varidx_map ();
    /* base_variation_idx_map in the plan is old_varidx->(varidx, delta)
     * mapping, new varidx is generated for subsetting, we need to remap this
     * after instancing */
    لكل (خودڪار _ : c->plan->بنياد_variation_idx_map.iter_ref ())
    {
      uint32_t varidx = _.second.پهريون;
      uint32_t *new_varidx;
      إذا (varidx_map.has (varidx, &new_varidx))
        _.second.پهريون = *new_varidx;
      وإلا
        _.second.پهريون = HB_OT_LAYOUT_NO_VARIATIONS_اشاريو;
    }
    return_trace (سچ);
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    خودڪار *ٻاهريون = c->serializer->شروعات_embed (*هذا);
    إذا (unlikely (!ٻاهريون || !c->serializer->extend_min (ٻاهريون))) return_trace (خطأ);

    ٻاهريون->نسخو = نسخو;
    منطقي ret = خطأ;
    إذا (نسخو.منزل_int () >= 0x00010001u) ret |= subset_varstore (c, ٻاهريون);

    ret |= ٻاهريون->hAxis.serialize_subset (c, hAxis, هذا);
    ret |= ٻاهريون->vAxis.serialize_subset (c, vAxis, هذا);

    return_trace (ret);
  }

  منطقي get_baseline (hb_font_t      *font,
		     hb_ٽيگ_t        baseline_ٽيگ,
		     hb_direction_t  direction,
		     hb_ٽيگ_t        script_ٽيگ,
		     hb_ٽيگ_t        language_ٽيگ,
		     hb_position_t  *بنياد) ثابت
  {
    ثابت بنياد_Coord *بنياد_coord = خالي_اشارو;
    إذا (unlikely (!get_axis (direction).get_baseline (baseline_ٽيگ, script_ٽيگ, language_ٽيگ, &بنياد_coord) ||
		  !بنياد_coord || !بنياد_coord->has_معلومات ()))
      أرجع خطأ;

    إذا (likely (بنياد))
      *بنياد = بنياد_coord->get_coord (font, get_var_store (), direction);

    أرجع سچ;
  }

  منطقي get_min_max (hb_font_t      *font,
		    hb_direction_t  direction,
		    hb_ٽيگ_t        script_ٽيگ,
		    hb_ٽيگ_t        language_ٽيگ,
		    hb_ٽيگ_t        feature_ٽيگ,
		    hb_position_t  *min,
		    hb_position_t  *max) ثابت
  {
    ثابت بنياد_Coord *min_coord, *max_coord;
    إذا (!get_axis (direction).get_min_max (script_ٽيگ, language_ٽيگ, feature_ٽيگ,
					   &min_coord, &max_coord))
      أرجع خطأ;

    ثابت شي_Variation_Store &var_store = get_var_store ();
    إذا (likely (min && min_coord)) *min = min_coord->get_coord (font, var_store, direction);
    إذا (likely (max && max_coord)) *max = max_coord->get_coord (font, var_store, direction);
    أرجع سچ;
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (c->check_struct (هذا) &&
			  hb_barrier () &&
			  likely (نسخو.major == 1) &&
			  hAxis.sanitize (c, هذا) &&
			  vAxis.sanitize (c, هذا) &&
			  (نسخو.منزل_int () < 0x00010001u || varStore.sanitize (c, هذا))));
  }

  محمي:
  Fixed_نسخو<>نسخو;	/* Version of the BASE table */
  Offset16_منزل<Axis>hAxis;		/* Offset to horizontal Axis table, from beginning
				 * of BASE table (may be NULL) */
  Offset16_منزل<Axis>vAxis;		/* Offset to vertical Axis table, from beginning
				 * of BASE table (may be NULL) */
  Offset32_منزل<شي_Variation_Store>
		varStore;	/* Offset to the table of Item Variation
				 * Store--from beginning of BASE
				 * header (may be NULL).  Introduced
				 * in version 0x00010001. */
  عام:
  DEFINE_سائيز_MIN (8);
};


} /* namespace OT */


#جيڪڏهن_پورو /* HB_OT_LAYOUT_BASE_TABLE_HH */
