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

#अन्तर्भाव "hb-open-type.hh"
#अन्तर्भाव "hb-ot-layout-common.hh"

नामाकाश OT {

/*
 * BASE -- Baseline
 * https://docs.microsoft.com/en-us/typography/opentype/spec/base
 */

संरचना BaseCoordFormat1
{
  hb_position_t get_coord (hb_font_t *font, hb_direction_t direction) स्थिरांक
  {
    परत HB_DIRECTION_IS_HORIZONTAL (direction) ? font->em_scale_y (coordinate) : font->em_scale_x (coordinate);
  }

  तार्किक subset (hb_subset_context_t *c) स्थिरांक
  {
    TRACE_SUBSET (हे);
    return_trace ((तार्किक) c->serializer->embed (*हे));
  }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे));
  }

  संरक्षित:
  HBUINT16	format;		/* Format identifier--format = 1 */
  FWORD		coordinate;	/* X or Y value, in design units */
  सार्वजनिक:
  DEFINE_SIZE_STATIC (4);
};

ढाँचा <typename Types>
संरचना BaseCoordFormat2_4
{
  hb_position_t get_coord (hb_font_t *font, hb_direction_t direction) स्थिरांक
  {
    /* TODO */
    परत HB_DIRECTION_IS_HORIZONTAL (direction) ? font->em_scale_y (coordinate) : font->em_scale_x (coordinate);
  }

  तार्किक subset (hb_subset_context_t *c) स्थिरांक
  {
    TRACE_SUBSET (हे);
    स्वतः *out = c->serializer->embed (*हे);
    जर (unlikely (!out)) return_trace (असत्य);

    hb_codepoint_t *new_gid;
    जर (!c->plan->glyph_map->has (referenceGlyph, &new_gid))
      return_trace (असत्य);

    return_trace (c->serializer->check_assign (out->referenceGlyph,
                                               *new_gid,
                                               HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे));
  }

  संरक्षित:
  HBUINT16	format;		/* Format identifier--format = 2 or 4 */
  FWORD		coordinate;	/* X or Y value, in design units */
  typename Types::HBGlyphID
		referenceGlyph;	/* Glyph ID of control glyph */
  HBUINT16	coordPoint;	/* Index of contour point on the
				 * reference glyph */
  सार्वजनिक:
  DEFINE_SIZE_STATIC (6 + Types::HBGlyphID::static_size);
};

संरचना BaseCoordFormat3
{
  hb_position_t get_coord (hb_font_t *font,
			   स्थिरांक ItemVariationStore &var_store,
			   hb_direction_t direction) स्थिरांक
  {
    स्थिरांक Device &device = हे+deviceTable;

    परत HB_DIRECTION_IS_HORIZONTAL (direction)
	 ? hb_saturate_add (font->em_scale_y (coordinate), device.get_y_delta (font, var_store))
	 : hb_saturate_add (font->em_scale_x (coordinate), device.get_x_delta (font, var_store));
  }

  शून्य collect_variation_indices (hb_set_t& varidx_set /* OUT */) स्थिरांक
  {
    अचिन्हित varidx = (हे+deviceTable).get_variation_index ();
    varidx_set.add (varidx);
  }

  तार्किक subset (hb_subset_context_t *c) स्थिरांक
  {
    TRACE_SUBSET (हे);
    स्वतः *out = c->serializer->embed (*हे);
    जर (unlikely (!out)) return_trace (असत्य);

    जर (!c->plan->pinned_at_default)
    {
      अचिन्हित var_idx = (हे+deviceTable).get_variation_index ();
      जर (var_idx != VarIdx::NO_VARIATION)
      {
        hb_pair_t<अचिन्हित, पूर्णांक> *v;
        जर (!c->plan->base_variation_idx_map.has (var_idx, &v))
          return_trace (असत्य);
        
        जर (unlikely (!c->serializer->check_assign (out->coordinate, coordinate + hb_second (*v),
                                                    HB_SERIALIZE_ERROR_INT_OVERFLOW)))
          return_trace (असत्य);
      }
    }
    
    out->deviceTable.serialize_copy (c->serializer, deviceTable,
                                     हे, 0,
                                     hb_serialize_context_t::Head,
                                     &c->plan->base_variation_idx_map);
    return_trace (सत्य);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (likely (c->check_struct (हे) &&
			  deviceTable.sanitize (c, हे)));
  }

  संरक्षित:
  HBUINT16	format;		/* Format identifier--format = 3 */
  FWORD		coordinate;	/* X or Y value, in design units */
  Offset16To<Device>
		deviceTable;	/* Offset to Device table for X or
				 * Y value, from beginning of
				 * BaseCoord table (may be NULL). */
  सार्वजनिक:
  DEFINE_SIZE_STATIC (6);
};

संरचना BaseCoord
{
  तार्किक has_data () स्थिरांक { परत u.format.v; }

  hb_position_t get_coord (hb_font_t            *font,
			   स्थिरांक ItemVariationStore &var_store,
			   hb_direction_t        direction) स्थिरांक
  {
    निवड (u.format.v) {
    स्थिती 1: hb_barrier (); परत u.format1.get_coord (font, direction);
    स्थिती 2: hb_barrier (); परत u.format2.get_coord (font, direction);
    स्थिती 3: hb_barrier (); परत u.format3.get_coord (font, var_store, direction);
#ifndef HB_NO_BEYOND_64K
    स्थिती 4: hb_barrier (); परत u.format4.get_coord (font, direction);
#endif
    पूर्वनिर्धारित:परत 0;
    }
  }

  शून्य collect_variation_indices (hb_set_t& varidx_set /* OUT */) स्थिरांक
  {
    निवड (u.format.v) {
    स्थिती 3: hb_barrier (); u.format3.collect_variation_indices (varidx_set); परत;
    पूर्वनिर्धारित:परत;
    }
  }

  ढाँचा <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) स्थिरांक
  {
    जर (unlikely (!c->may_dispatch (हे, &u.format.v))) परत c->no_dispatch_return_मूल्य ();
    TRACE_DISPATCH (हे, u.format.v);
    निवड (u.format.v) {
    स्थिती 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    स्थिती 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    स्थिती 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
#ifndef HB_NO_BEYOND_64K
    स्थिती 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
#endif
    पूर्वनिर्धारित:return_trace (c->default_return_मूल्य ());
    }
  }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    जर (unlikely (!u.format.v.sanitize (c))) return_trace (असत्य);
    hb_barrier ();
    निवड (u.format.v) {
    स्थिती 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    स्थिती 2: hb_barrier (); return_trace (u.format2.sanitize (c));
    स्थिती 3: hb_barrier (); return_trace (u.format3.sanitize (c));
#ifndef HB_NO_BEYOND_64K
    स्थिती 4: hb_barrier (); return_trace (u.format4.sanitize (c));
#endif
    पूर्वनिर्धारित:return_trace (असत्य);
    }
  }

  संरक्षित:
  संघ {
  संरचना { HBUINT16 v; }	format;
  BaseCoordFormat1	format1;
  BaseCoordFormat2_4<Layout::SmallTypes>
			format2;
  BaseCoordFormat3	format3;
#ifndef HB_NO_BEYOND_64K
  BaseCoordFormat2_4<Layout::MediumTypes>
			format4;
#endif
  } u;
  सार्वजनिक:
  DEFINE_SIZE_UNION (2, format.v);
};

संरचना FeatMinMaxRecord
{
  पूर्णांक cmp (hb_tag_t key) स्थिरांक { परत tag.cmp (key); }

  तार्किक has_data () स्थिरांक { परत tag; }

  hb_tag_t get_feature_tag () स्थिरांक { परत tag; }

  शून्य get_min_max (स्थिरांक BaseCoord **min, स्थिरांक BaseCoord **max) स्थिरांक
  {
    जर (likely (min)) *min = &(हे+minCoord);
    जर (likely (max)) *max = &(हे+maxCoord);
  }

  शून्य collect_variation_indices (स्थिरांक hb_subset_plan_t* plan,
                                  स्थिरांक शून्य *base,
                                  hb_set_t& varidx_set /* OUT */) स्थिरांक
  {
    जर (!plan->layout_features.has (tag))
      परत;

    (base+minCoord).collect_variation_indices (varidx_set);
    (base+maxCoord).collect_variation_indices (varidx_set);
  }

  तार्किक subset (hb_subset_context_t *c,
               स्थिरांक शून्य *base) स्थिरांक
  {
    TRACE_SUBSET (हे);
    स्वतः *out = c->serializer->embed (*हे);
    जर (unlikely (!out)) return_trace (असत्य);

    तार्किक ret = out->minCoord.serialize_subset (c, minCoord, base);
    ret |= out->maxCoord.serialize_subset (c, maxCoord, base);
    return_trace (ret);
  }

  तार्किक sanitize (hb_sanitize_context_t *c, स्थिरांक शून्य *base) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (likely (c->check_struct (हे) &&
			  minCoord.sanitize (c, base) &&
			  maxCoord.sanitize (c, base)));
  }

  संरक्षित:
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
  सार्वजनिक:
  DEFINE_SIZE_STATIC (8);
};

संरचना MinMax
{
  शून्य get_min_max (hb_tag_t          feature_tag,
		    स्थिरांक BaseCoord **min,
		    स्थिरांक BaseCoord **max) स्थिरांक
  {
    स्थिरांक FeatMinMaxRecord &minMaxCoord = featMinMaxRecords.द्वा_खोज (feature_tag);
    जर (minMaxCoord.has_data ())
      minMaxCoord.get_min_max (min, max);
    अन्यथा
    {
      जर (likely (min)) *min = &(हे+minCoord);
      जर (likely (max)) *max = &(हे+maxCoord);
    }
  }

  शून्य collect_variation_indices (स्थिरांक hb_subset_plan_t* plan,
                                  hb_set_t& varidx_set /* OUT */) स्थिरांक
  {
    (हे+minCoord).collect_variation_indices (varidx_set);
    (हे+maxCoord).collect_variation_indices (varidx_set);
    साठी (स्थिरांक FeatMinMaxRecord& record : featMinMaxRecords)
      record.collect_variation_indices (plan, हे, varidx_set);
  }

  तार्किक subset (hb_subset_context_t *c) स्थिरांक
  {
    TRACE_SUBSET (हे);
    स्वतः *out = c->serializer->start_embed (*हे);
    जर (unlikely (!out || !c->serializer->extend_min (out))) return_trace (असत्य);

    तार्किक ret = out->minCoord.serialize_subset (c, minCoord, हे);
    ret |= out->maxCoord.serialize_subset (c, maxCoord, हे);

    अचिन्हित len = 0;
    साठी (स्थिरांक FeatMinMaxRecord& _ : featMinMaxRecords)
    {
      hb_tag_t feature_tag = _.get_feature_tag ();
      जर (!c->plan->layout_features.has (feature_tag))
        सुरूठेवा;

      स्वतः snap = c->serializer->snapshot ();
      जर (!_.subset (c, हे)) c->serializer->revert (snap);
      अन्यथा len++;
    }

    जर (len > 0) ret = सत्य;
    जर (!ret) return_trace (असत्य);
    return_trace (c->serializer->check_assign (out->featMinMaxRecords.len, len,
                                               HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (likely (c->check_struct (हे) &&
			  minCoord.sanitize (c, हे) &&
			  maxCoord.sanitize (c, हे) &&
			  featMinMaxRecords.sanitize (c, हे)));
  }

  संरक्षित:
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
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (6, featMinMaxRecords);
};

संरचना Base_मूल्ये
{
  स्थिरांक BaseCoord &get_base_coord (पूर्णांक baseline_tag_index) स्थिरांक
  {
    जर (baseline_tag_index == -1) baseline_tag_index = defaultIndex;
    परत हे+baseCoords[baseline_tag_index];
  }

  शून्य collect_variation_indices (hb_set_t& varidx_set /* OUT */) स्थिरांक
  {
    साठी (स्थिरांक स्वतः& _ : baseCoords)
      (हे+_).collect_variation_indices (varidx_set);
  }

  तार्किक subset (hb_subset_context_t *c) स्थिरांक
  {
    TRACE_SUBSET (हे);
    स्वतः *out = c->serializer->start_embed (*हे);
    जर (unlikely (!out || !c->serializer->extend_min (out))) return_trace (असत्य);
    out->defaultIndex = defaultIndex;

    तार्किक ret = असत्य;
    साठी (स्थिरांक स्वतः& _ : baseCoords)
    {
      स्वतः *o = out->baseCoords.serialize_append (c->serializer);
      जर (unlikely (!o)) return_trace (असत्य);
       ret |= o->serialize_subset (c, _, हे);
    }

    return_trace (ret);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (likely (c->check_struct (हे) &&
			  baseCoords.sanitize (c, हे)));
  }

  संरक्षित:
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
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (4, baseCoords);
};

संरचना BaseLangSysRecord
{
  पूर्णांक cmp (hb_tag_t key) स्थिरांक { परत baseLangSysTag.cmp (key); }

  तार्किक has_data () स्थिरांक { परत baseLangSysTag; }

  स्थिरांक MinMax &get_min_max (स्थिरांक शून्य* base) स्थिरांक { परत base+minMax; }

  शून्य collect_variation_indices (स्थिरांक शून्य* base,
                                  स्थिरांक hb_subset_plan_t* plan,
                                  hb_set_t& varidx_set /* OUT */) स्थिरांक
  { (base+minMax).collect_variation_indices (plan, varidx_set); }

  तार्किक subset (hb_subset_context_t *c,
               स्थिरांक शून्य *base) स्थिरांक
  {
    TRACE_SUBSET (हे);
    स्वतः *out = c->serializer->embed (*हे);
    जर (unlikely (!out)) return_trace (असत्य);

    return_trace (out->minMax.serialize_subset (c, minMax, base));
  }

  तार्किक sanitize (hb_sanitize_context_t *c, स्थिरांक शून्य *base) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (likely (c->check_struct (हे) &&
			  minMax.sanitize (c, base)));
  }

  संरक्षित:
  Tag		baseLangSysTag;	/* 4-byte language system identification tag */
  Offset16To<MinMax>
		minMax;		/* Offset to MinMax table, from beginning
				 * of BaseScript table */
  सार्वजनिक:
  DEFINE_SIZE_STATIC (6);
};

संरचना BaseScript
{
  स्थिरांक MinMax &get_min_max (hb_tag_t language_tag) स्थिरांक
  {
    स्थिरांक BaseLangSysRecord& record = baseLangSysRecords.द्वा_खोज (language_tag);
    परत record.has_data () ? record.get_min_max (हे) : हे+defaultMinMax;
  }

  स्थिरांक BaseCoord &get_base_coord (पूर्णांक baseline_tag_index) स्थिरांक
  { परत (हे+base_मूल्ये).get_base_coord (baseline_tag_index); }

  तार्किक has_मूल्ये () स्थिरांक { परत base_मूल्ये; }
  तार्किक has_min_max () स्थिरांक { परत defaultMinMax || baseLangSysRecords; }

  शून्य collect_variation_indices (स्थिरांक hb_subset_plan_t* plan,
                                  hb_set_t& varidx_set /* OUT */) स्थिरांक
  {
    (हे+base_मूल्ये).collect_variation_indices (varidx_set);
    (हे+defaultMinMax).collect_variation_indices (plan, varidx_set);
    
    साठी (स्थिरांक BaseLangSysRecord& _ : baseLangSysRecords)
      _.collect_variation_indices (हे, plan, varidx_set);
  }

  तार्किक subset (hb_subset_context_t *c) स्थिरांक
  {
    TRACE_SUBSET (हे);
    स्वतः *out = c->serializer->start_embed (*हे);
    जर (unlikely (!out || !c->serializer->extend_min (out))) return_trace (असत्य);

    तार्किक ret = out->base_मूल्ये.serialize_subset (c, base_मूल्ये, हे);
    ret |= out->defaultMinMax.serialize_subset (c, defaultMinMax, हे);

    अचिन्हित len = 0;
    साठी (स्थिरांक स्वतः& _ : baseLangSysRecords)
    {
      स्वतः snap = c->serializer->snapshot ();
      जर (_.subset (c, हे)) {
        ret = सत्य;
        len++;
      }
      अन्यथा c->serializer->revert (snap);
    }

    जर (!ret) return_trace (असत्य);
    return_trace (c->serializer->check_assign (out->baseLangSysRecords.len, len,
                                               HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (likely (c->check_struct (हे) &&
			  base_मूल्ये.sanitize (c, हे) &&
			  defaultMinMax.sanitize (c, हे) &&
			  baseLangSysRecords.sanitize (c, हे)));
  }

  संरक्षित:
  Offset16To<Base_मूल्ये>
		base_मूल्ये;	/* Offset to BaseValues table, from beginning
				 * of BaseScript table (may be NULL) */
  Offset16To<MinMax>
		defaultMinMax;	/* Offset to MinMax table, from beginning of
				 * BaseScript table (may be NULL) */
  SortedArray16Of<BaseLangSysRecord>
		baseLangSysRecords;
				/* Number of BaseLangSysRecords
				 * defined — may be zero (0) */

  सार्वजनिक:
  DEFINE_SIZE_ARRAY (6, baseLangSysRecords);
};

संरचना BaseScriptList;
संरचना BaseScriptRecord
{
  पूर्णांक cmp (hb_tag_t key) स्थिरांक { परत baseScriptTag.cmp (key); }

  तार्किक has_data () स्थिरांक { परत baseScriptTag; }

  hb_tag_t get_script_tag () स्थिरांक { परत baseScriptTag; }

  स्थिरांक BaseScript &get_base_script (स्थिरांक BaseScriptList *list) स्थिरांक
  { परत list+baseScript; }

  शून्य collect_variation_indices (स्थिरांक hb_subset_plan_t* plan,
                                  स्थिरांक शून्य* list,
                                  hb_set_t& varidx_set /* OUT */) स्थिरांक
  {
    जर (!plan->layout_scripts.has (baseScriptTag))
      परत;

    (list+baseScript).collect_variation_indices (plan, varidx_set);
  }

  तार्किक subset (hb_subset_context_t *c,
               स्थिरांक शून्य *base) स्थिरांक
  {
    TRACE_SUBSET (हे);
    स्वतः *out = c->serializer->embed (*हे);
    जर (unlikely (!out)) return_trace (असत्य);

    return_trace (out->baseScript.serialize_subset (c, baseScript, base));
  }

  तार्किक sanitize (hb_sanitize_context_t *c, स्थिरांक शून्य *base) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (likely (c->check_struct (हे) &&
			  baseScript.sanitize (c, base)));
  }

  संरक्षित:
  Tag		baseScriptTag;	/* 4-byte script identification tag */
  Offset16To<BaseScript>
		baseScript;	/* Offset to BaseScript table, from beginning
				 * of BaseScriptList */

  सार्वजनिक:
  DEFINE_SIZE_STATIC (6);
};

संरचना BaseScriptList
{
  स्थिरांक BaseScript &get_base_script (hb_tag_t script) स्थिरांक
  {
    स्थिरांक BaseScriptRecord *record = &baseScriptRecords.द्वा_खोज (script);
    जर (!record->has_data ()) record = &baseScriptRecords.द्वा_खोज (HB_TAG ('D','F','L','T'));
    परत record->has_data () ? record->get_base_script (हे) : Null (BaseScript);
  }

  शून्य collect_variation_indices (स्थिरांक hb_subset_plan_t* plan,
                                  hb_set_t& varidx_set /* OUT */) स्थिरांक
  {
    साठी (स्थिरांक BaseScriptRecord& _ : baseScriptRecords)
      _.collect_variation_indices (plan, हे, varidx_set);
  }

  तार्किक subset (hb_subset_context_t *c) स्थिरांक
  {
    TRACE_SUBSET (हे);
    स्वतः *out = c->serializer->start_embed (*हे);
    जर (unlikely (!out || !c->serializer->extend_min (out))) return_trace (असत्य);

    अचिन्हित len = 0;
    साठी (स्थिरांक BaseScriptRecord& _ : baseScriptRecords)
    {
      hb_tag_t script_tag = _.get_script_tag ();
      जर (!c->plan->layout_scripts.has (script_tag))
        सुरूठेवा;

      स्वतः snap = c->serializer->snapshot ();
      जर (!_.subset (c, हे)) c->serializer->revert (snap);
      अन्यथा len++;
    }

    जर (!len) return_trace (असत्य);
    return_trace (c->serializer->check_assign (out->baseScriptRecords.len, len,
                                               HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे) &&
		  baseScriptRecords.sanitize (c, हे));
  }

  संरक्षित:
  SortedArray16Of<BaseScriptRecord>
			baseScriptRecords;

  सार्वजनिक:
  DEFINE_SIZE_ARRAY (2, baseScriptRecords);
};

संरचना Axis
{
  तार्किक get_baseline (hb_tag_t          baseline_tag,
		     hb_tag_t          script_tag,
		     hb_tag_t          language_tag,
		     स्थिरांक BaseCoord **coord) स्थिरांक
  {
    स्थिरांक BaseScript &base_script = (हे+baseScriptList).get_base_script (script_tag);
    जर (!base_script.has_मूल्ये ())
    {
      *coord = nullptr;
      परत असत्य;
    }

    जर (likely (coord))
    {
      अचिन्हित पूर्णांक tag_index = 0;
      जर (!(हे+baseTagList).bfind (baseline_tag, &tag_index))
      {
        *coord = nullptr;
        परत असत्य;
      }
      *coord = &base_script.get_base_coord (tag_index);
    }

    परत सत्य;
  }

  तार्किक get_min_max (hb_tag_t          script_tag,
		    hb_tag_t          language_tag,
		    hb_tag_t          feature_tag,
		    स्थिरांक BaseCoord **min_coord,
		    स्थिरांक BaseCoord **max_coord) स्थिरांक
  {
    स्थिरांक BaseScript &base_script = (हे+baseScriptList).get_base_script (script_tag);
    जर (!base_script.has_min_max ())
    {
      *min_coord = *max_coord = nullptr;
      परत असत्य;
    }

    base_script.get_min_max (language_tag).get_min_max (feature_tag, min_coord, max_coord);

    परत सत्य;
  }

  शून्य collect_variation_indices (स्थिरांक hb_subset_plan_t* plan,
                                  hb_set_t& varidx_set /* OUT */) स्थिरांक
  { (हे+baseScriptList).collect_variation_indices (plan, varidx_set); }

  तार्किक subset (hb_subset_context_t *c) स्थिरांक
  {
    TRACE_SUBSET (हे);
    स्वतः *out = c->serializer->embed (*हे);
    जर (unlikely (!out)) return_trace (असत्य);

    तार्किक ret = out->baseTagList.serialize_copy (c->serializer, baseTagList, हे);
    ret |= out->baseScriptList.serialize_subset (c, baseScriptList, हे);
    return_trace (ret);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (likely (c->check_struct (हे) &&
			  baseTagList.sanitize (c, हे) &&
			  baseScriptList.sanitize (c, हे)));
  }

  संरक्षित:
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

  सार्वजनिक:
  DEFINE_SIZE_STATIC (4);
};

संरचना BASE
{
  स्थिर constexpr hb_tag_t tableTag = HB_OT_TAG_BASE;

  स्थिरांक Axis &get_axis (hb_direction_t direction) स्थिरांक
  { परत HB_DIRECTION_IS_VERTICAL (direction) ? हे+vAxis : हे+hAxis; }

  तार्किक has_var_store () स्थिरांक
  { परत version.to_int () >= 0x00010001u && varStore != 0; }

  स्थिरांक ItemVariationStore &get_var_store () स्थिरांक
  { परत version.to_int () < 0x00010001u ? Null (ItemVariationStore) : हे+varStore; }

  शून्य collect_variation_indices (स्थिरांक hb_subset_plan_t* plan,
                                  hb_set_t& varidx_set /* OUT */) स्थिरांक
  {
    (हे+hAxis).collect_variation_indices (plan, varidx_set);
    (हे+vAxis).collect_variation_indices (plan, varidx_set);
  }

  तार्किक subset_varstore (hb_subset_context_t *c,
                        BASE *out /* OUT */) स्थिरांक
  {
    TRACE_SUBSET (हे);
    जर (!c->serializer->allocate_size<Offset32To<ItemVariationStore>> (Offset32To<ItemVariationStore>::static_size))
        return_trace (असत्य);
    जर (!c->plan->normalized_coords)
      return_trace (out->varStore.serialize_subset (c, varStore, हे, c->plan->base_varstore_inner_maps.as_array ()));

    जर (c->plan->all_axes_pinned)
      return_trace (सत्य);

    item_variations_t item_vars;
    जर (!item_vars.instantiate (हे+varStore, c->plan, सत्य, सत्य,
                                c->plan->base_varstore_inner_maps.as_array ()))
      return_trace (असत्य);

    जर (!out->varStore.serialize_serialize (c->serializer,
                                            item_vars.has_long_word (),
                                            c->plan->axis_tags,
                                            item_vars.get_region_list (),
                                            item_vars.get_vardata_encodings ()))
      return_trace (असत्य);

    स्थिरांक hb_map_t &varidx_map = item_vars.get_varidx_map ();
    /* base_variation_idx_map in the plan is old_varidx->(varidx, delta)
     * mapping, new varidx is generated for subsetting, we need to remap this
     * after instancing */
    साठी (स्वतः _ : c->plan->base_variation_idx_map.iter_ref ())
    {
      uint32_t varidx = _.second.first;
      uint32_t *new_varidx;
      जर (varidx_map.has (varidx, &new_varidx))
        _.second.first = *new_varidx;
      अन्यथा
        _.second.first = HB_OT_LAYOUT_NO_VARIATIONS_INDEX;
    }
    return_trace (सत्य);
  }

  तार्किक subset (hb_subset_context_t *c) स्थिरांक
  {
    TRACE_SUBSET (हे);
    स्वतः *out = c->serializer->start_embed (*हे);
    जर (unlikely (!out || !c->serializer->extend_min (out))) return_trace (असत्य);

    out->version = version;
    तार्किक ret = असत्य;
    जर (version.to_int () >= 0x00010001u) ret |= subset_varstore (c, out);

    ret |= out->hAxis.serialize_subset (c, hAxis, हे);
    ret |= out->vAxis.serialize_subset (c, vAxis, हे);

    return_trace (ret);
  }

  तार्किक get_baseline (hb_font_t      *font,
		     hb_tag_t        baseline_tag,
		     hb_direction_t  direction,
		     hb_tag_t        script_tag,
		     hb_tag_t        language_tag,
		     hb_position_t  *base) स्थिरांक
  {
    स्थिरांक BaseCoord *base_coord = nullptr;
    जर (unlikely (!get_axis (direction).get_baseline (baseline_tag, script_tag, language_tag, &base_coord) ||
		  !base_coord || !base_coord->has_data ()))
      परत असत्य;

    जर (likely (base))
      *base = base_coord->get_coord (font, get_var_store (), direction);

    परत सत्य;
  }

  तार्किक get_min_max (hb_font_t      *font,
		    hb_direction_t  direction,
		    hb_tag_t        script_tag,
		    hb_tag_t        language_tag,
		    hb_tag_t        feature_tag,
		    hb_position_t  *min,
		    hb_position_t  *max) स्थिरांक
  {
    स्थिरांक BaseCoord *min_coord, *max_coord;
    जर (!get_axis (direction).get_min_max (script_tag, language_tag, feature_tag,
					   &min_coord, &max_coord))
      परत असत्य;

    स्थिरांक ItemVariationStore &var_store = get_var_store ();
    जर (likely (min && min_coord)) *min = min_coord->get_coord (font, var_store, direction);
    जर (likely (max && max_coord)) *max = max_coord->get_coord (font, var_store, direction);
    परत सत्य;
  }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (likely (c->check_struct (हे) &&
			  hb_barrier () &&
			  likely (version.major == 1) &&
			  hAxis.sanitize (c, हे) &&
			  vAxis.sanitize (c, हे) &&
			  (version.to_int () < 0x00010001u || varStore.sanitize (c, हे))));
  }

  संरक्षित:
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
  सार्वजनिक:
  DEFINE_SIZE_MIN (8);
};


} /* namespace OT */


#endif /* HB_OT_LAYOUT_BASE_TABLE_HH */
