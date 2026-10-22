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

#অন্তর্ভাব "hb-open-type.hh"
#অন্তর্ভাব "hb-ot-layout-common.hh"

নামাকাশ OT {

/*
 * BASE -- Baseline
 * https://docs.microsoft.com/en-us/typography/opentype/spec/base
 */

কাঠামো BaseCoordFormat1
{
  hb_position_t get_coord (hb_font_t *font, hb_direction_t direction) ধ্রুবক
  {
    ফেরত HB_DIRECTION_IS_HORIZONTAL (direction) ? font->em_scale_y (coordinate) : font->em_scale_x (coordinate);
  }

  যৌক্তিক subset (hb_subset_context_t *c) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    return_trace ((যৌক্তিক) c->serializer->embed (*এই));
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই));
  }

  সুরক্ষিত:
  HBUINT16	format;		/* Format identifier--format = 1 */
  FWORD		coordinate;	/* X or Y value, in design units */
  সর্বজনীন:
  DEFINE_আকার_STATIC (4);
};

template <typename Types>
কাঠামো BaseCoordFormat2_4
{
  hb_position_t get_coord (hb_font_t *font, hb_direction_t direction) ধ্রুবক
  {
    /* TODO */
    ফেরত HB_DIRECTION_IS_HORIZONTAL (direction) ? font->em_scale_y (coordinate) : font->em_scale_x (coordinate);
  }

  যৌক্তিক subset (hb_subset_context_t *c) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    auto *out = c->serializer->embed (*এই);
    যদি (unlikely (!out)) return_trace (মিথ্যা);

    hb_codepoint_t *new_gid;
    যদি (!c->plan->glyph_map->has (referenceGlyph, &new_gid))
      return_trace (মিথ্যা);

    return_trace (c->serializer->check_assign (out->referenceGlyph,
                                               *new_gid,
                                               HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই));
  }

  সুরক্ষিত:
  HBUINT16	format;		/* Format identifier--format = 2 or 4 */
  FWORD		coordinate;	/* X or Y value, in design units */
  typename Types::HBGlyphID
		referenceGlyph;	/* Glyph ID of control glyph */
  HBUINT16	coordPoint;	/* Index of contour point on the
				 * reference glyph */
  সর্বজনীন:
  DEFINE_আকার_STATIC (6 + Types::HBGlyphID::static_আকার);
};

কাঠামো BaseCoordFormat3
{
  hb_position_t get_coord (hb_font_t *font,
			   ধ্রুবক ItemVariationStore &var_store,
			   hb_direction_t direction) ধ্রুবক
  {
    ধ্রুবক Device &device = এই+deviceTable;

    ফেরত HB_DIRECTION_IS_HORIZONTAL (direction)
	 ? hb_saturate_add (font->em_scale_y (coordinate), device.get_y_delta (font, var_store))
	 : hb_saturate_add (font->em_scale_x (coordinate), device.get_x_delta (font, var_store));
  }

  শূন্য collect_variation_indices (hb_set_t& varidx_set /* OUT */) ধ্রুবক
  {
    unsigned varidx = (এই+deviceTable).get_variation_সূচক ();
    varidx_set.add (varidx);
  }

  যৌক্তিক subset (hb_subset_context_t *c) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    auto *out = c->serializer->embed (*এই);
    যদি (unlikely (!out)) return_trace (মিথ্যা);

    যদি (!c->plan->pinned_at_default)
    {
      unsigned var_idx = (এই+deviceTable).get_variation_সূচক ();
      যদি (var_idx != VarIdx::NO_VARIATION)
      {
        hb_pair_t<unsigned, পূর্ণসংখ্যা> *v;
        যদি (!c->plan->base_variation_idx_map.has (var_idx, &v))
          return_trace (মিথ্যা);
        
        যদি (unlikely (!c->serializer->check_assign (out->coordinate, coordinate + hb_second (*v),
                                                    HB_SERIALIZE_ERROR_INT_OVERFLOW)))
          return_trace (মিথ্যা);
      }
    }
    
    out->deviceTable.serialize_copy (c->serializer, deviceTable,
                                     এই, 0,
                                     hb_serialize_context_t::শীর্ষ,
                                     &c->plan->base_variation_idx_map);
    return_trace (সত্য);
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (likely (c->check_struct (এই) &&
			  deviceTable.sanitize (c, এই)));
  }

  সুরক্ষিত:
  HBUINT16	format;		/* Format identifier--format = 3 */
  FWORD		coordinate;	/* X or Y value, in design units */
  Offset16To<Device>
		deviceTable;	/* Offset to Device table for X or
				 * Y value, from beginning of
				 * BaseCoord table (may be NULL). */
  সর্বজনীন:
  DEFINE_আকার_STATIC (6);
};

কাঠামো BaseCoord
{
  যৌক্তিক has_তথ্য () ধ্রুবক { ফেরত u.format.v; }

  hb_position_t get_coord (hb_font_t            *font,
			   ধ্রুবক ItemVariationStore &var_store,
			   hb_direction_t        direction) ধ্রুবক
  {
    নির্বাচন (u.format.v) {
    ক্ষেত্র 1: hb_barrier (); ফেরত u.format1.get_coord (font, direction);
    ক্ষেত্র 2: hb_barrier (); ফেরত u.format2.get_coord (font, direction);
    ক্ষেত্র 3: hb_barrier (); ফেরত u.format3.get_coord (font, var_store, direction);
#ifndef HB_NO_BEYOND_64K
    ক্ষেত্র 4: hb_barrier (); ফেরত u.format4.get_coord (font, direction);
#endif
    পূর্বনির্ধারিত:ফেরত 0;
    }
  }

  শূন্য collect_variation_indices (hb_set_t& varidx_set /* OUT */) ধ্রুবক
  {
    নির্বাচন (u.format.v) {
    ক্ষেত্র 3: hb_barrier (); u.format3.collect_variation_indices (varidx_set); ফেরত;
    পূর্বনির্ধারিত:ফেরত;
    }
  }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ধ্রুবক
  {
    যদি (unlikely (!c->may_dispatch (এই, &u.format.v))) ফেরত c->no_dispatch_return_মান ();
    TRACE_DISPATCH (এই, u.format.v);
    নির্বাচন (u.format.v) {
    ক্ষেত্র 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    ক্ষেত্র 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    ক্ষেত্র 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
#ifndef HB_NO_BEYOND_64K
    ক্ষেত্র 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
#endif
    পূর্বনির্ধারিত:return_trace (c->default_return_মান ());
    }
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    যদি (unlikely (!u.format.v.sanitize (c))) return_trace (মিথ্যা);
    hb_barrier ();
    নির্বাচন (u.format.v) {
    ক্ষেত্র 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    ক্ষেত্র 2: hb_barrier (); return_trace (u.format2.sanitize (c));
    ক্ষেত্র 3: hb_barrier (); return_trace (u.format3.sanitize (c));
#ifndef HB_NO_BEYOND_64K
    ক্ষেত্র 4: hb_barrier (); return_trace (u.format4.sanitize (c));
#endif
    পূর্বনির্ধারিত:return_trace (মিথ্যা);
    }
  }

  সুরক্ষিত:
  সংঘ {
  কাঠামো { HBUINT16 v; }	format;
  BaseCoordFormat1	format1;
  BaseCoordFormat2_4<Layout::SmallTypes>
			format2;
  BaseCoordFormat3	format3;
#ifndef HB_NO_BEYOND_64K
  BaseCoordFormat2_4<Layout::MediumTypes>
			format4;
#endif
  } u;
  সর্বজনীন:
  DEFINE_আকার_UNION (2, format.v);
};

কাঠামো FeatMinMaxRecord
{
  পূর্ণসংখ্যা cmp (hb_tag_t key) ধ্রুবক { ফেরত tag.cmp (key); }

  যৌক্তিক has_তথ্য () ধ্রুবক { ফেরত tag; }

  hb_tag_t get_feature_tag () ধ্রুবক { ফেরত tag; }

  শূন্য get_min_max (ধ্রুবক BaseCoord **min, ধ্রুবক BaseCoord **max) ধ্রুবক
  {
    যদি (likely (min)) *min = &(এই+minCoord);
    যদি (likely (max)) *max = &(এই+maxCoord);
  }

  শূন্য collect_variation_indices (ধ্রুবক hb_subset_plan_t* plan,
                                  ধ্রুবক শূন্য *base,
                                  hb_set_t& varidx_set /* OUT */) ধ্রুবক
  {
    যদি (!plan->layout_features.has (tag))
      ফেরত;

    (base+minCoord).collect_variation_indices (varidx_set);
    (base+maxCoord).collect_variation_indices (varidx_set);
  }

  যৌক্তিক subset (hb_subset_context_t *c,
               ধ্রুবক শূন্য *base) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    auto *out = c->serializer->embed (*এই);
    যদি (unlikely (!out)) return_trace (মিথ্যা);

    যৌক্তিক ret = out->minCoord.serialize_subset (c, minCoord, base);
    ret |= out->maxCoord.serialize_subset (c, maxCoord, base);
    return_trace (ret);
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c, ধ্রুবক শূন্য *base) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (likely (c->check_struct (এই) &&
			  minCoord.sanitize (c, base) &&
			  maxCoord.sanitize (c, base)));
  }

  সুরক্ষিত:
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
  সর্বজনীন:
  DEFINE_আকার_STATIC (8);
};

কাঠামো MinMax
{
  শূন্য get_min_max (hb_tag_t          feature_tag,
		    ধ্রুবক BaseCoord **min,
		    ধ্রুবক BaseCoord **max) ধ্রুবক
  {
    ধ্রুবক FeatMinMaxRecord &minMaxCoord = featMinMaxRecords.bsearch (feature_tag);
    যদি (minMaxCoord.has_তথ্য ())
      minMaxCoord.get_min_max (min, max);
    অন্যথা
    {
      যদি (likely (min)) *min = &(এই+minCoord);
      যদি (likely (max)) *max = &(এই+maxCoord);
    }
  }

  শূন্য collect_variation_indices (ধ্রুবক hb_subset_plan_t* plan,
                                  hb_set_t& varidx_set /* OUT */) ধ্রুবক
  {
    (এই+minCoord).collect_variation_indices (varidx_set);
    (এই+maxCoord).collect_variation_indices (varidx_set);
    জন্য (ধ্রুবক FeatMinMaxRecord& record : featMinMaxRecords)
      record.collect_variation_indices (plan, এই, varidx_set);
  }

  যৌক্তিক subset (hb_subset_context_t *c) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    auto *out = c->serializer->start_embed (*এই);
    যদি (unlikely (!out || !c->serializer->extend_min (out))) return_trace (মিথ্যা);

    যৌক্তিক ret = out->minCoord.serialize_subset (c, minCoord, এই);
    ret |= out->maxCoord.serialize_subset (c, maxCoord, এই);

    unsigned len = 0;
    জন্য (ধ্রুবক FeatMinMaxRecord& _ : featMinMaxRecords)
    {
      hb_tag_t feature_tag = _.get_feature_tag ();
      যদি (!c->plan->layout_features.has (feature_tag))
        চালু;

      auto snap = c->serializer->snapshot ();
      যদি (!_.subset (c, এই)) c->serializer->revert (snap);
      অন্যথা len++;
    }

    যদি (len > 0) ret = সত্য;
    যদি (!ret) return_trace (মিথ্যা);
    return_trace (c->serializer->check_assign (out->featMinMaxRecords.len, len,
                                               HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (likely (c->check_struct (এই) &&
			  minCoord.sanitize (c, এই) &&
			  maxCoord.sanitize (c, এই) &&
			  featMinMaxRecords.sanitize (c, এই)));
  }

  সুরক্ষিত:
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
  সর্বজনীন:
  DEFINE_আকার_ARRAY (6, featMinMaxRecords);
};

কাঠামো Base_মানসমূহ
{
  ধ্রুবক BaseCoord &get_base_coord (পূর্ণসংখ্যা baseline_tag_সূচক) ধ্রুবক
  {
    যদি (baseline_tag_সূচক == -1) baseline_tag_সূচক = default_সূচক_2;
    ফেরত এই+baseCoords[baseline_tag_সূচক];
  }

  শূন্য collect_variation_indices (hb_set_t& varidx_set /* OUT */) ধ্রুবক
  {
    জন্য (ধ্রুবক auto& _ : baseCoords)
      (এই+_).collect_variation_indices (varidx_set);
  }

  যৌক্তিক subset (hb_subset_context_t *c) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    auto *out = c->serializer->start_embed (*এই);
    যদি (unlikely (!out || !c->serializer->extend_min (out))) return_trace (মিথ্যা);
    out->default_সূচক_2 = default_সূচক_2;

    যৌক্তিক ret = মিথ্যা;
    জন্য (ধ্রুবক auto& _ : baseCoords)
    {
      auto *o = out->baseCoords.serialize_append (c->serializer);
      যদি (unlikely (!o)) return_trace (মিথ্যা);
       ret |= o->serialize_subset (c, _, এই);
    }

    return_trace (ret);
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (likely (c->check_struct (এই) &&
			  baseCoords.sanitize (c, এই)));
  }

  সুরক্ষিত:
  সূচক		default_সূচক_2;	/* Index number of default baseline for this
				 * script — equals index position of baseline tag
				 * in baselineTags array of the BaseTagList */
  Array16OfOffset16To<BaseCoord>
		baseCoords;	/* Number of BaseCoord tables defined — should equal
				 * baseTagCount in the BaseTagList
				 *
				 * Array of offsets to BaseCoord tables, from beginning of
				 * BaseValues table — order matches baselineTags array in
				 * the BaseTagList */
  সর্বজনীন:
  DEFINE_আকার_ARRAY (4, baseCoords);
};

কাঠামো BaseLangSysRecord
{
  পূর্ণসংখ্যা cmp (hb_tag_t key) ধ্রুবক { ফেরত baseLangSysTag.cmp (key); }

  যৌক্তিক has_তথ্য () ধ্রুবক { ফেরত baseLangSysTag; }

  ধ্রুবক MinMax &get_min_max (ধ্রুবক শূন্য* base) ধ্রুবক { ফেরত base+minMax; }

  শূন্য collect_variation_indices (ধ্রুবক শূন্য* base,
                                  ধ্রুবক hb_subset_plan_t* plan,
                                  hb_set_t& varidx_set /* OUT */) ধ্রুবক
  { (base+minMax).collect_variation_indices (plan, varidx_set); }

  যৌক্তিক subset (hb_subset_context_t *c,
               ধ্রুবক শূন্য *base) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    auto *out = c->serializer->embed (*এই);
    যদি (unlikely (!out)) return_trace (মিথ্যা);

    return_trace (out->minMax.serialize_subset (c, minMax, base));
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c, ধ্রুবক শূন্য *base) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (likely (c->check_struct (এই) &&
			  minMax.sanitize (c, base)));
  }

  সুরক্ষিত:
  Tag		baseLangSysTag;	/* 4-byte language system identification tag */
  Offset16To<MinMax>
		minMax;		/* Offset to MinMax table, from beginning
				 * of BaseScript table */
  সর্বজনীন:
  DEFINE_আকার_STATIC (6);
};

কাঠামো BaseScript
{
  ধ্রুবক MinMax &get_min_max (hb_tag_t language_tag) ধ্রুবক
  {
    ধ্রুবক BaseLangSysRecord& record = baseLangSysRecords.bsearch (language_tag);
    ফেরত record.has_তথ্য () ? record.get_min_max (এই) : এই+defaultMinMax;
  }

  ধ্রুবক BaseCoord &get_base_coord (পূর্ণসংখ্যা baseline_tag_সূচক) ধ্রুবক
  { ফেরত (এই+base_মানসমূহ).get_base_coord (baseline_tag_সূচক); }

  যৌক্তিক has_মানসমূহ () ধ্রুবক { ফেরত base_মানসমূহ; }
  যৌক্তিক has_min_max () ধ্রুবক { ফেরত defaultMinMax || baseLangSysRecords; }

  শূন্য collect_variation_indices (ধ্রুবক hb_subset_plan_t* plan,
                                  hb_set_t& varidx_set /* OUT */) ধ্রুবক
  {
    (এই+base_মানসমূহ).collect_variation_indices (varidx_set);
    (এই+defaultMinMax).collect_variation_indices (plan, varidx_set);
    
    জন্য (ধ্রুবক BaseLangSysRecord& _ : baseLangSysRecords)
      _.collect_variation_indices (এই, plan, varidx_set);
  }

  যৌক্তিক subset (hb_subset_context_t *c) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    auto *out = c->serializer->start_embed (*এই);
    যদি (unlikely (!out || !c->serializer->extend_min (out))) return_trace (মিথ্যা);

    যৌক্তিক ret = out->base_মানসমূহ.serialize_subset (c, base_মানসমূহ, এই);
    ret |= out->defaultMinMax.serialize_subset (c, defaultMinMax, এই);

    unsigned len = 0;
    জন্য (ধ্রুবক auto& _ : baseLangSysRecords)
    {
      auto snap = c->serializer->snapshot ();
      যদি (_.subset (c, এই)) {
        ret = সত্য;
        len++;
      }
      অন্যথা c->serializer->revert (snap);
    }

    যদি (!ret) return_trace (মিথ্যা);
    return_trace (c->serializer->check_assign (out->baseLangSysRecords.len, len,
                                               HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (likely (c->check_struct (এই) &&
			  base_মানসমূহ.sanitize (c, এই) &&
			  defaultMinMax.sanitize (c, এই) &&
			  baseLangSysRecords.sanitize (c, এই)));
  }

  সুরক্ষিত:
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

  সর্বজনীন:
  DEFINE_আকার_ARRAY (6, baseLangSysRecords);
};

কাঠামো BaseScriptList;
কাঠামো BaseScriptRecord
{
  পূর্ণসংখ্যা cmp (hb_tag_t key) ধ্রুবক { ফেরত baseScriptTag.cmp (key); }

  যৌক্তিক has_তথ্য () ধ্রুবক { ফেরত baseScriptTag; }

  hb_tag_t get_script_tag () ধ্রুবক { ফেরত baseScriptTag; }

  ধ্রুবক BaseScript &get_base_script (ধ্রুবক BaseScriptList *list) ধ্রুবক
  { ফেরত list+baseScript; }

  শূন্য collect_variation_indices (ধ্রুবক hb_subset_plan_t* plan,
                                  ধ্রুবক শূন্য* list,
                                  hb_set_t& varidx_set /* OUT */) ধ্রুবক
  {
    যদি (!plan->layout_scripts.has (baseScriptTag))
      ফেরত;

    (list+baseScript).collect_variation_indices (plan, varidx_set);
  }

  যৌক্তিক subset (hb_subset_context_t *c,
               ধ্রুবক শূন্য *base) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    auto *out = c->serializer->embed (*এই);
    যদি (unlikely (!out)) return_trace (মিথ্যা);

    return_trace (out->baseScript.serialize_subset (c, baseScript, base));
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c, ধ্রুবক শূন্য *base) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (likely (c->check_struct (এই) &&
			  baseScript.sanitize (c, base)));
  }

  সুরক্ষিত:
  Tag		baseScriptTag;	/* 4-byte script identification tag */
  Offset16To<BaseScript>
		baseScript;	/* Offset to BaseScript table, from beginning
				 * of BaseScriptList */

  সর্বজনীন:
  DEFINE_আকার_STATIC (6);
};

কাঠামো BaseScriptList
{
  ধ্রুবক BaseScript &get_base_script (hb_tag_t script) ধ্রুবক
  {
    ধ্রুবক BaseScriptRecord *record = &baseScriptRecords.bsearch (script);
    যদি (!record->has_তথ্য ()) record = &baseScriptRecords.bsearch (HB_TAG ('D','F','L','T'));
    ফেরত record->has_তথ্য () ? record->get_base_script (এই) : Null (BaseScript);
  }

  শূন্য collect_variation_indices (ধ্রুবক hb_subset_plan_t* plan,
                                  hb_set_t& varidx_set /* OUT */) ধ্রুবক
  {
    জন্য (ধ্রুবক BaseScriptRecord& _ : baseScriptRecords)
      _.collect_variation_indices (plan, এই, varidx_set);
  }

  যৌক্তিক subset (hb_subset_context_t *c) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    auto *out = c->serializer->start_embed (*এই);
    যদি (unlikely (!out || !c->serializer->extend_min (out))) return_trace (মিথ্যা);

    unsigned len = 0;
    জন্য (ধ্রুবক BaseScriptRecord& _ : baseScriptRecords)
    {
      hb_tag_t script_tag = _.get_script_tag ();
      যদি (!c->plan->layout_scripts.has (script_tag))
        চালু;

      auto snap = c->serializer->snapshot ();
      যদি (!_.subset (c, এই)) c->serializer->revert (snap);
      অন্যথা len++;
    }

    যদি (!len) return_trace (মিথ্যা);
    return_trace (c->serializer->check_assign (out->baseScriptRecords.len, len,
                                               HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই) &&
		  baseScriptRecords.sanitize (c, এই));
  }

  সুরক্ষিত:
  SortedArray16Of<BaseScriptRecord>
			baseScriptRecords;

  সর্বজনীন:
  DEFINE_আকার_ARRAY (2, baseScriptRecords);
};

কাঠামো Axis
{
  যৌক্তিক get_baseline (hb_tag_t          baseline_tag,
		     hb_tag_t          script_tag,
		     hb_tag_t          language_tag,
		     ধ্রুবক BaseCoord **coord) ধ্রুবক
  {
    ধ্রুবক BaseScript &base_script = (এই+baseScriptList).get_base_script (script_tag);
    যদি (!base_script.has_মানসমূহ ())
    {
      *coord = nullptr;
      ফেরত মিথ্যা;
    }

    যদি (likely (coord))
    {
      unsigned পূর্ণসংখ্যা tag_সূচক = 0;
      যদি (!(এই+baseTagList).bfind (baseline_tag, &tag_সূচক))
      {
        *coord = nullptr;
        ফেরত মিথ্যা;
      }
      *coord = &base_script.get_base_coord (tag_সূচক);
    }

    ফেরত সত্য;
  }

  যৌক্তিক get_min_max (hb_tag_t          script_tag,
		    hb_tag_t          language_tag,
		    hb_tag_t          feature_tag,
		    ধ্রুবক BaseCoord **min_coord,
		    ধ্রুবক BaseCoord **max_coord) ধ্রুবক
  {
    ধ্রুবক BaseScript &base_script = (এই+baseScriptList).get_base_script (script_tag);
    যদি (!base_script.has_min_max ())
    {
      *min_coord = *max_coord = nullptr;
      ফেরত মিথ্যা;
    }

    base_script.get_min_max (language_tag).get_min_max (feature_tag, min_coord, max_coord);

    ফেরত সত্য;
  }

  শূন্য collect_variation_indices (ধ্রুবক hb_subset_plan_t* plan,
                                  hb_set_t& varidx_set /* OUT */) ধ্রুবক
  { (এই+baseScriptList).collect_variation_indices (plan, varidx_set); }

  যৌক্তিক subset (hb_subset_context_t *c) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    auto *out = c->serializer->embed (*এই);
    যদি (unlikely (!out)) return_trace (মিথ্যা);

    যৌক্তিক ret = out->baseTagList.serialize_copy (c->serializer, baseTagList, এই);
    ret |= out->baseScriptList.serialize_subset (c, baseScriptList, এই);
    return_trace (ret);
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (likely (c->check_struct (এই) &&
			  baseTagList.sanitize (c, এই) &&
			  baseScriptList.sanitize (c, এই)));
  }

  সুরক্ষিত:
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

  সর্বজনীন:
  DEFINE_আকার_STATIC (4);
};

কাঠামো BASE
{
  স্থির constexpr hb_tag_t tableTag = HB_OT_TAG_BASE;

  ধ্রুবক Axis &get_axis (hb_direction_t direction) ধ্রুবক
  { ফেরত HB_DIRECTION_IS_VERTICAL (direction) ? এই+vAxis : এই+hAxis; }

  যৌক্তিক has_var_store () ধ্রুবক
  { ফেরত version.to_int () >= 0x00010001u && varStore != 0; }

  ধ্রুবক ItemVariationStore &get_var_store () ধ্রুবক
  { ফেরত version.to_int () < 0x00010001u ? Null (ItemVariationStore) : এই+varStore; }

  শূন্য collect_variation_indices (ধ্রুবক hb_subset_plan_t* plan,
                                  hb_set_t& varidx_set /* OUT */) ধ্রুবক
  {
    (এই+hAxis).collect_variation_indices (plan, varidx_set);
    (এই+vAxis).collect_variation_indices (plan, varidx_set);
  }

  যৌক্তিক subset_varstore (hb_subset_context_t *c,
                        BASE *out /* OUT */) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    যদি (!c->serializer->allocate_আকার<Offset32To<ItemVariationStore>> (Offset32To<ItemVariationStore>::static_আকার))
        return_trace (মিথ্যা);
    যদি (!c->plan->normalized_coords)
      return_trace (out->varStore.serialize_subset (c, varStore, এই, c->plan->base_varstore_inner_maps.as_array ()));

    যদি (c->plan->all_axes_pinned)
      return_trace (সত্য);

    item_variations_t item_vars;
    যদি (!item_vars.instantiate (এই+varStore, c->plan, সত্য, সত্য,
                                c->plan->base_varstore_inner_maps.as_array ()))
      return_trace (মিথ্যা);

    যদি (!out->varStore.serialize_serialize (c->serializer,
                                            item_vars.has_long_word (),
                                            c->plan->axis_tags,
                                            item_vars.get_region_list (),
                                            item_vars.get_vardata_encodings ()))
      return_trace (মিথ্যা);

    ধ্রুবক hb_map_t &varidx_map = item_vars.get_varidx_map ();
    /* base_variation_idx_map in the plan is old_varidx->(varidx, delta)
     * mapping, new varidx is generated for subsetting, we need to remap this
     * after instancing */
    জন্য (auto _ : c->plan->base_variation_idx_map.iter_ref ())
    {
      uint32_t varidx = _.second.first;
      uint32_t *new_varidx;
      যদি (varidx_map.has (varidx, &new_varidx))
        _.second.first = *new_varidx;
      অন্যথা
        _.second.first = HB_OT_LAYOUT_NO_VARIATIONS_সূচক;
    }
    return_trace (সত্য);
  }

  যৌক্তিক subset (hb_subset_context_t *c) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    auto *out = c->serializer->start_embed (*এই);
    যদি (unlikely (!out || !c->serializer->extend_min (out))) return_trace (মিথ্যা);

    out->version = version;
    যৌক্তিক ret = মিথ্যা;
    যদি (version.to_int () >= 0x00010001u) ret |= subset_varstore (c, out);

    ret |= out->hAxis.serialize_subset (c, hAxis, এই);
    ret |= out->vAxis.serialize_subset (c, vAxis, এই);

    return_trace (ret);
  }

  যৌক্তিক get_baseline (hb_font_t      *font,
		     hb_tag_t        baseline_tag,
		     hb_direction_t  direction,
		     hb_tag_t        script_tag,
		     hb_tag_t        language_tag,
		     hb_position_t  *base) ধ্রুবক
  {
    ধ্রুবক BaseCoord *base_coord = nullptr;
    যদি (unlikely (!get_axis (direction).get_baseline (baseline_tag, script_tag, language_tag, &base_coord) ||
		  !base_coord || !base_coord->has_তথ্য ()))
      ফেরত মিথ্যা;

    যদি (likely (base))
      *base = base_coord->get_coord (font, get_var_store (), direction);

    ফেরত সত্য;
  }

  যৌক্তিক get_min_max (hb_font_t      *font,
		    hb_direction_t  direction,
		    hb_tag_t        script_tag,
		    hb_tag_t        language_tag,
		    hb_tag_t        feature_tag,
		    hb_position_t  *min,
		    hb_position_t  *max) ধ্রুবক
  {
    ধ্রুবক BaseCoord *min_coord, *max_coord;
    যদি (!get_axis (direction).get_min_max (script_tag, language_tag, feature_tag,
					   &min_coord, &max_coord))
      ফেরত মিথ্যা;

    ধ্রুবক ItemVariationStore &var_store = get_var_store ();
    যদি (likely (min && min_coord)) *min = min_coord->get_coord (font, var_store, direction);
    যদি (likely (max && max_coord)) *max = max_coord->get_coord (font, var_store, direction);
    ফেরত সত্য;
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (likely (c->check_struct (এই) &&
			  hb_barrier () &&
			  likely (version.major == 1) &&
			  hAxis.sanitize (c, এই) &&
			  vAxis.sanitize (c, এই) &&
			  (version.to_int () < 0x00010001u || varStore.sanitize (c, এই))));
  }

  সুরক্ষিত:
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
  সর্বজনীন:
  DEFINE_আকার_MIN (8);
};


} /* namespace OT */


#endif /* HB_OT_LAYOUT_BASE_TABLE_HH */
