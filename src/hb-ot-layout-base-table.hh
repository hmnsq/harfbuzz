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
  hb_position_t get_coord (hb_font_t *font, hb_direction_t direction) ध्रुव
  {
    प्रत्यागम HB_DIRECTION_IS_HORIZONTAL (direction) ? font->em_scale_y (coordinate) : font->em_scale_x (coordinate);
  }

  तार्किक subset (hb_subset_context_t *c) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    return_trace ((तार्किक) c->serializer->embed (*एतत्));
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्));
  }

  रक्षित:
  HBUINT16	format;		/* Format identifier--format = 1 */
  FWORD		coordinate;	/* X or Y value, in design units */
  सार्वजनिक:
  DEFINE_SIZE_STATIC (4);
};

template <typename Types>
संरचना BaseCoordFormat2_4
{
  hb_position_t get_coord (hb_font_t *font, hb_direction_t direction) ध्रुव
  {
    /* TODO */
    प्रत्यागम HB_DIRECTION_IS_HORIZONTAL (direction) ? font->em_scale_y (coordinate) : font->em_scale_x (coordinate);
  }

  तार्किक subset (hb_subset_context_t *c) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    auto *out = c->serializer->embed (*एतत्);
    यदि (unlikely (!out)) return_trace (असत्य);

    hb_codepoint_t *new_gid;
    यदि (!c->plan->glyph_map->has (referenceGlyph, &new_gid))
      return_trace (असत्य);

    return_trace (c->serializer->check_assign (out->referenceGlyph,
                                               *new_gid,
                                               HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्));
  }

  रक्षित:
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
			   ध्रुव ItemVariationStore &var_store,
			   hb_direction_t direction) ध्रुव
  {
    ध्रुव Device &device = एतत्+deviceTable;

    प्रत्यागम HB_DIRECTION_IS_HORIZONTAL (direction)
	 ? hb_saturate_add (font->em_scale_y (coordinate), device.get_y_delta (font, var_store))
	 : hb_saturate_add (font->em_scale_x (coordinate), device.get_x_delta (font, var_store));
  }

  शून्य collect_variation_indices (hb_set_t& varidx_set /* OUT */) ध्रुव
  {
    unsigned varidx = (एतत्+deviceTable).get_variation_index ();
    varidx_set.add (varidx);
  }

  तार्किक subset (hb_subset_context_t *c) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    auto *out = c->serializer->embed (*एतत्);
    यदि (unlikely (!out)) return_trace (असत्य);

    यदि (!c->plan->pinned_at_default)
    {
      unsigned var_idx = (एतत्+deviceTable).get_variation_index ();
      यदि (var_idx != VarIdx::NO_VARIATION)
      {
        hb_pair_t<unsigned, पूर्णाङ्क> *v;
        यदि (!c->plan->base_variation_idx_map.has (var_idx, &v))
          return_trace (असत्य);
        
        यदि (unlikely (!c->serializer->check_assign (out->coordinate, coordinate + hb_second (*v),
                                                    HB_SERIALIZE_ERROR_INT_OVERFLOW)))
          return_trace (असत्य);
      }
    }
    
    out->deviceTable.serialize_copy (c->serializer, deviceTable,
                                     एतत्, 0,
                                     hb_serialize_context_t::Head,
                                     &c->plan->base_variation_idx_map);
    return_trace (सत्य);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (likely (c->check_struct (एतत्) &&
			  deviceTable.sanitize (c, एतत्)));
  }

  रक्षित:
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
  तार्किक has_data () ध्रुव { प्रत्यागम u.format.v; }

  hb_position_t get_coord (hb_font_t            *font,
			   ध्रुव ItemVariationStore &var_store,
			   hb_direction_t        direction) ध्रुव
  {
    विकल्प (u.format.v) {
    पक्ष 1: hb_barrier (); प्रत्यागम u.format1.get_coord (font, direction);
    पक्ष 2: hb_barrier (); प्रत्यागम u.format2.get_coord (font, direction);
    पक्ष 3: hb_barrier (); प्रत्यागम u.format3.get_coord (font, var_store, direction);
#ifndef HB_NO_BEYOND_64K
    पक्ष 4: hb_barrier (); प्रत्यागम u.format4.get_coord (font, direction);
#endif
    स्वभाव:प्रत्यागम 0;
    }
  }

  शून्य collect_variation_indices (hb_set_t& varidx_set /* OUT */) ध्रुव
  {
    विकल्प (u.format.v) {
    पक्ष 3: hb_barrier (); u.format3.collect_variation_indices (varidx_set); प्रत्यागम;
    स्वभाव:प्रत्यागम;
    }
  }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ध्रुव
  {
    यदि (unlikely (!c->may_dispatch (एतत्, &u.format.v))) प्रत्यागम c->no_dispatch_return_मूल्य ();
    TRACE_DISPATCH (एतत्, u.format.v);
    विकल्प (u.format.v) {
    पक्ष 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    पक्ष 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    पक्ष 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
#ifndef HB_NO_BEYOND_64K
    पक्ष 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
#endif
    स्वभाव:return_trace (c->default_return_मूल्य ());
    }
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    यदि (unlikely (!u.format.v.sanitize (c))) return_trace (असत्य);
    hb_barrier ();
    विकल्प (u.format.v) {
    पक्ष 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    पक्ष 2: hb_barrier (); return_trace (u.format2.sanitize (c));
    पक्ष 3: hb_barrier (); return_trace (u.format3.sanitize (c));
#ifndef HB_NO_BEYOND_64K
    पक्ष 4: hb_barrier (); return_trace (u.format4.sanitize (c));
#endif
    स्वभाव:return_trace (असत्य);
    }
  }

  रक्षित:
  सङ्घ {
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
  पूर्णाङ्क cmp (hb_tag_t key) ध्रुव { प्रत्यागम tag.cmp (key); }

  तार्किक has_data () ध्रुव { प्रत्यागम tag; }

  hb_tag_t get_feature_tag () ध्रुव { प्रत्यागम tag; }

  शून्य get_min_max (ध्रुव BaseCoord **min, ध्रुव BaseCoord **max) ध्रुव
  {
    यदि (likely (min)) *min = &(एतत्+minCoord);
    यदि (likely (max)) *max = &(एतत्+maxCoord);
  }

  शून्य collect_variation_indices (ध्रुव hb_subset_plan_t* plan,
                                  ध्रुव शून्य *base,
                                  hb_set_t& varidx_set /* OUT */) ध्रुव
  {
    यदि (!plan->layout_features.has (tag))
      प्रत्यागम;

    (base+minCoord).collect_variation_indices (varidx_set);
    (base+maxCoord).collect_variation_indices (varidx_set);
  }

  तार्किक subset (hb_subset_context_t *c,
               ध्रुव शून्य *base) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    auto *out = c->serializer->embed (*एतत्);
    यदि (unlikely (!out)) return_trace (असत्य);

    तार्किक ret = out->minCoord.serialize_subset (c, minCoord, base);
    ret |= out->maxCoord.serialize_subset (c, maxCoord, base);
    return_trace (ret);
  }

  तार्किक sanitize (hb_sanitize_context_t *c, ध्रुव शून्य *base) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (likely (c->check_struct (एतत्) &&
			  minCoord.sanitize (c, base) &&
			  maxCoord.sanitize (c, base)));
  }

  रक्षित:
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
		    ध्रुव BaseCoord **min,
		    ध्रुव BaseCoord **max) ध्रुव
  {
    ध्रुव FeatMinMaxRecord &minMaxCoord = featMinMaxRecords.bsearch (feature_tag);
    यदि (minMaxCoord.has_data ())
      minMaxCoord.get_min_max (min, max);
    अन्यथा
    {
      यदि (likely (min)) *min = &(एतत्+minCoord);
      यदि (likely (max)) *max = &(एतत्+maxCoord);
    }
  }

  शून्य collect_variation_indices (ध्रुव hb_subset_plan_t* plan,
                                  hb_set_t& varidx_set /* OUT */) ध्रुव
  {
    (एतत्+minCoord).collect_variation_indices (varidx_set);
    (एतत्+maxCoord).collect_variation_indices (varidx_set);
    प्रति (ध्रुव FeatMinMaxRecord& record : featMinMaxRecords)
      record.collect_variation_indices (plan, एतत्, varidx_set);
  }

  तार्किक subset (hb_subset_context_t *c) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    auto *out = c->serializer->start_embed (*एतत्);
    यदि (unlikely (!out || !c->serializer->extend_min (out))) return_trace (असत्य);

    तार्किक ret = out->minCoord.serialize_subset (c, minCoord, एतत्);
    ret |= out->maxCoord.serialize_subset (c, maxCoord, एतत्);

    unsigned len = 0;
    प्रति (ध्रुव FeatMinMaxRecord& _ : featMinMaxRecords)
    {
      hb_tag_t feature_tag = _.get_feature_tag ();
      यदि (!c->plan->layout_features.has (feature_tag))
        अनुवर्त;

      auto snap = c->serializer->snapshot ();
      यदि (!_.subset (c, एतत्)) c->serializer->revert (snap);
      अन्यथा len++;
    }

    यदि (len > 0) ret = सत्य;
    यदि (!ret) return_trace (असत्य);
    return_trace (c->serializer->check_assign (out->featMinMaxRecords.len, len,
                                               HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (likely (c->check_struct (एतत्) &&
			  minCoord.sanitize (c, एतत्) &&
			  maxCoord.sanitize (c, एतत्) &&
			  featMinMaxRecords.sanitize (c, एतत्)));
  }

  रक्षित:
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
  ध्रुव BaseCoord &get_base_coord (पूर्णाङ्क baseline_tag_index) ध्रुव
  {
    यदि (baseline_tag_index == -1) baseline_tag_index = defaultIndex;
    प्रत्यागम एतत्+baseCoords[baseline_tag_index];
  }

  शून्य collect_variation_indices (hb_set_t& varidx_set /* OUT */) ध्रुव
  {
    प्रति (ध्रुव auto& _ : baseCoords)
      (एतत्+_).collect_variation_indices (varidx_set);
  }

  तार्किक subset (hb_subset_context_t *c) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    auto *out = c->serializer->start_embed (*एतत्);
    यदि (unlikely (!out || !c->serializer->extend_min (out))) return_trace (असत्य);
    out->defaultIndex = defaultIndex;

    तार्किक ret = असत्य;
    प्रति (ध्रुव auto& _ : baseCoords)
    {
      auto *o = out->baseCoords.serialize_append (c->serializer);
      यदि (unlikely (!o)) return_trace (असत्य);
       ret |= o->serialize_subset (c, _, एतत्);
    }

    return_trace (ret);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (likely (c->check_struct (एतत्) &&
			  baseCoords.sanitize (c, एतत्)));
  }

  रक्षित:
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
  पूर्णाङ्क cmp (hb_tag_t key) ध्रुव { प्रत्यागम baseLangSysTag.cmp (key); }

  तार्किक has_data () ध्रुव { प्रत्यागम baseLangSysTag; }

  ध्रुव MinMax &get_min_max (ध्रुव शून्य* base) ध्रुव { प्रत्यागम base+minMax; }

  शून्य collect_variation_indices (ध्रुव शून्य* base,
                                  ध्रुव hb_subset_plan_t* plan,
                                  hb_set_t& varidx_set /* OUT */) ध्रुव
  { (base+minMax).collect_variation_indices (plan, varidx_set); }

  तार्किक subset (hb_subset_context_t *c,
               ध्रुव शून्य *base) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    auto *out = c->serializer->embed (*एतत्);
    यदि (unlikely (!out)) return_trace (असत्य);

    return_trace (out->minMax.serialize_subset (c, minMax, base));
  }

  तार्किक sanitize (hb_sanitize_context_t *c, ध्रुव शून्य *base) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (likely (c->check_struct (एतत्) &&
			  minMax.sanitize (c, base)));
  }

  रक्षित:
  Tag		baseLangSysTag;	/* 4-byte language system identification tag */
  Offset16To<MinMax>
		minMax;		/* Offset to MinMax table, from beginning
				 * of BaseScript table */
  सार्वजनिक:
  DEFINE_SIZE_STATIC (6);
};

संरचना BaseScript
{
  ध्रुव MinMax &get_min_max (hb_tag_t language_tag) ध्रुव
  {
    ध्रुव BaseLangSysRecord& record = baseLangSysRecords.bsearch (language_tag);
    प्रत्यागम record.has_data () ? record.get_min_max (एतत्) : एतत्+defaultMinMax;
  }

  ध्रुव BaseCoord &get_base_coord (पूर्णाङ्क baseline_tag_index) ध्रुव
  { प्रत्यागम (एतत्+base_मूल्ये).get_base_coord (baseline_tag_index); }

  तार्किक has_मूल्ये () ध्रुव { प्रत्यागम base_मूल्ये; }
  तार्किक has_min_max () ध्रुव { प्रत्यागम defaultMinMax || baseLangSysRecords; }

  शून्य collect_variation_indices (ध्रुव hb_subset_plan_t* plan,
                                  hb_set_t& varidx_set /* OUT */) ध्रुव
  {
    (एतत्+base_मूल्ये).collect_variation_indices (varidx_set);
    (एतत्+defaultMinMax).collect_variation_indices (plan, varidx_set);
    
    प्रति (ध्रुव BaseLangSysRecord& _ : baseLangSysRecords)
      _.collect_variation_indices (एतत्, plan, varidx_set);
  }

  तार्किक subset (hb_subset_context_t *c) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    auto *out = c->serializer->start_embed (*एतत्);
    यदि (unlikely (!out || !c->serializer->extend_min (out))) return_trace (असत्य);

    तार्किक ret = out->base_मूल्ये.serialize_subset (c, base_मूल्ये, एतत्);
    ret |= out->defaultMinMax.serialize_subset (c, defaultMinMax, एतत्);

    unsigned len = 0;
    प्रति (ध्रुव auto& _ : baseLangSysRecords)
    {
      auto snap = c->serializer->snapshot ();
      यदि (_.subset (c, एतत्)) {
        ret = सत्य;
        len++;
      }
      अन्यथा c->serializer->revert (snap);
    }

    यदि (!ret) return_trace (असत्य);
    return_trace (c->serializer->check_assign (out->baseLangSysRecords.len, len,
                                               HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (likely (c->check_struct (एतत्) &&
			  base_मूल्ये.sanitize (c, एतत्) &&
			  defaultMinMax.sanitize (c, एतत्) &&
			  baseLangSysRecords.sanitize (c, एतत्)));
  }

  रक्षित:
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
  पूर्णाङ्क cmp (hb_tag_t key) ध्रुव { प्रत्यागम baseScriptTag.cmp (key); }

  तार्किक has_data () ध्रुव { प्रत्यागम baseScriptTag; }

  hb_tag_t get_script_tag () ध्रुव { प्रत्यागम baseScriptTag; }

  ध्रुव BaseScript &get_base_script (ध्रुव BaseScriptList *list) ध्रुव
  { प्रत्यागम list+baseScript; }

  शून्य collect_variation_indices (ध्रुव hb_subset_plan_t* plan,
                                  ध्रुव शून्य* list,
                                  hb_set_t& varidx_set /* OUT */) ध्रुव
  {
    यदि (!plan->layout_scripts.has (baseScriptTag))
      प्रत्यागम;

    (list+baseScript).collect_variation_indices (plan, varidx_set);
  }

  तार्किक subset (hb_subset_context_t *c,
               ध्रुव शून्य *base) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    auto *out = c->serializer->embed (*एतत्);
    यदि (unlikely (!out)) return_trace (असत्य);

    return_trace (out->baseScript.serialize_subset (c, baseScript, base));
  }

  तार्किक sanitize (hb_sanitize_context_t *c, ध्रुव शून्य *base) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (likely (c->check_struct (एतत्) &&
			  baseScript.sanitize (c, base)));
  }

  रक्षित:
  Tag		baseScriptTag;	/* 4-byte script identification tag */
  Offset16To<BaseScript>
		baseScript;	/* Offset to BaseScript table, from beginning
				 * of BaseScriptList */

  सार्वजनिक:
  DEFINE_SIZE_STATIC (6);
};

संरचना BaseScriptList
{
  ध्रुव BaseScript &get_base_script (hb_tag_t script) ध्रुव
  {
    ध्रुव BaseScriptRecord *record = &baseScriptRecords.bsearch (script);
    यदि (!record->has_data ()) record = &baseScriptRecords.bsearch (HB_TAG ('D','F','L','T'));
    प्रत्यागम record->has_data () ? record->get_base_script (एतत्) : Null (BaseScript);
  }

  शून्य collect_variation_indices (ध्रुव hb_subset_plan_t* plan,
                                  hb_set_t& varidx_set /* OUT */) ध्रुव
  {
    प्रति (ध्रुव BaseScriptRecord& _ : baseScriptRecords)
      _.collect_variation_indices (plan, एतत्, varidx_set);
  }

  तार्किक subset (hb_subset_context_t *c) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    auto *out = c->serializer->start_embed (*एतत्);
    यदि (unlikely (!out || !c->serializer->extend_min (out))) return_trace (असत्य);

    unsigned len = 0;
    प्रति (ध्रुव BaseScriptRecord& _ : baseScriptRecords)
    {
      hb_tag_t script_tag = _.get_script_tag ();
      यदि (!c->plan->layout_scripts.has (script_tag))
        अनुवर्त;

      auto snap = c->serializer->snapshot ();
      यदि (!_.subset (c, एतत्)) c->serializer->revert (snap);
      अन्यथा len++;
    }

    यदि (!len) return_trace (असत्य);
    return_trace (c->serializer->check_assign (out->baseScriptRecords.len, len,
                                               HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) &&
		  baseScriptRecords.sanitize (c, एतत्));
  }

  रक्षित:
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
		     ध्रुव BaseCoord **coord) ध्रुव
  {
    ध्रुव BaseScript &base_script = (एतत्+baseScriptList).get_base_script (script_tag);
    यदि (!base_script.has_मूल्ये ())
    {
      *coord = nullptr;
      प्रत्यागम असत्य;
    }

    यदि (likely (coord))
    {
      unsigned पूर्णाङ्क tag_index = 0;
      यदि (!(एतत्+baseTagList).bfind (baseline_tag, &tag_index))
      {
        *coord = nullptr;
        प्रत्यागम असत्य;
      }
      *coord = &base_script.get_base_coord (tag_index);
    }

    प्रत्यागम सत्य;
  }

  तार्किक get_min_max (hb_tag_t          script_tag,
		    hb_tag_t          language_tag,
		    hb_tag_t          feature_tag,
		    ध्रुव BaseCoord **min_coord,
		    ध्रुव BaseCoord **max_coord) ध्रुव
  {
    ध्रुव BaseScript &base_script = (एतत्+baseScriptList).get_base_script (script_tag);
    यदि (!base_script.has_min_max ())
    {
      *min_coord = *max_coord = nullptr;
      प्रत्यागम असत्य;
    }

    base_script.get_min_max (language_tag).get_min_max (feature_tag, min_coord, max_coord);

    प्रत्यागम सत्य;
  }

  शून्य collect_variation_indices (ध्रुव hb_subset_plan_t* plan,
                                  hb_set_t& varidx_set /* OUT */) ध्रुव
  { (एतत्+baseScriptList).collect_variation_indices (plan, varidx_set); }

  तार्किक subset (hb_subset_context_t *c) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    auto *out = c->serializer->embed (*एतत्);
    यदि (unlikely (!out)) return_trace (असत्य);

    तार्किक ret = out->baseTagList.serialize_copy (c->serializer, baseTagList, एतत्);
    ret |= out->baseScriptList.serialize_subset (c, baseScriptList, एतत्);
    return_trace (ret);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (likely (c->check_struct (एतत्) &&
			  baseTagList.sanitize (c, एतत्) &&
			  baseScriptList.sanitize (c, एतत्)));
  }

  रक्षित:
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

  ध्रुव Axis &get_axis (hb_direction_t direction) ध्रुव
  { प्रत्यागम HB_DIRECTION_IS_VERTICAL (direction) ? एतत्+vAxis : एतत्+hAxis; }

  तार्किक has_var_store () ध्रुव
  { प्रत्यागम version.to_int () >= 0x00010001u && varStore != 0; }

  ध्रुव ItemVariationStore &get_var_store () ध्रुव
  { प्रत्यागम version.to_int () < 0x00010001u ? Null (ItemVariationStore) : एतत्+varStore; }

  शून्य collect_variation_indices (ध्रुव hb_subset_plan_t* plan,
                                  hb_set_t& varidx_set /* OUT */) ध्रुव
  {
    (एतत्+hAxis).collect_variation_indices (plan, varidx_set);
    (एतत्+vAxis).collect_variation_indices (plan, varidx_set);
  }

  तार्किक subset_varstore (hb_subset_context_t *c,
                        BASE *out /* OUT */) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    यदि (!c->serializer->allocate_size<Offset32To<ItemVariationStore>> (Offset32To<ItemVariationStore>::static_size))
        return_trace (असत्य);
    यदि (!c->plan->normalized_coords)
      return_trace (out->varStore.serialize_subset (c, varStore, एतत्, c->plan->base_varstore_inner_maps.as_array ()));

    यदि (c->plan->all_axes_pinned)
      return_trace (सत्य);

    item_variations_t item_vars;
    यदि (!item_vars.instantiate (एतत्+varStore, c->plan, सत्य, सत्य,
                                c->plan->base_varstore_inner_maps.as_array ()))
      return_trace (असत्य);

    यदि (!out->varStore.serialize_serialize (c->serializer,
                                            item_vars.has_long_word (),
                                            c->plan->axis_tags,
                                            item_vars.get_region_list (),
                                            item_vars.get_vardata_encodings ()))
      return_trace (असत्य);

    ध्रुव hb_map_t &varidx_map = item_vars.get_varidx_map ();
    /* base_variation_idx_map in the plan is old_varidx->(varidx, delta)
     * mapping, new varidx is generated for subsetting, we need to remap this
     * after instancing */
    प्रति (auto _ : c->plan->base_variation_idx_map.iter_ref ())
    {
      uint32_t varidx = _.second.first;
      uint32_t *new_varidx;
      यदि (varidx_map.has (varidx, &new_varidx))
        _.second.first = *new_varidx;
      अन्यथा
        _.second.first = HB_OT_LAYOUT_NO_VARIATIONS_INDEX;
    }
    return_trace (सत्य);
  }

  तार्किक subset (hb_subset_context_t *c) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    auto *out = c->serializer->start_embed (*एतत्);
    यदि (unlikely (!out || !c->serializer->extend_min (out))) return_trace (असत्य);

    out->version = version;
    तार्किक ret = असत्य;
    यदि (version.to_int () >= 0x00010001u) ret |= subset_varstore (c, out);

    ret |= out->hAxis.serialize_subset (c, hAxis, एतत्);
    ret |= out->vAxis.serialize_subset (c, vAxis, एतत्);

    return_trace (ret);
  }

  तार्किक get_baseline (hb_font_t      *font,
		     hb_tag_t        baseline_tag,
		     hb_direction_t  direction,
		     hb_tag_t        script_tag,
		     hb_tag_t        language_tag,
		     hb_position_t  *base) ध्रुव
  {
    ध्रुव BaseCoord *base_coord = nullptr;
    यदि (unlikely (!get_axis (direction).get_baseline (baseline_tag, script_tag, language_tag, &base_coord) ||
		  !base_coord || !base_coord->has_data ()))
      प्रत्यागम असत्य;

    यदि (likely (base))
      *base = base_coord->get_coord (font, get_var_store (), direction);

    प्रत्यागम सत्य;
  }

  तार्किक get_min_max (hb_font_t      *font,
		    hb_direction_t  direction,
		    hb_tag_t        script_tag,
		    hb_tag_t        language_tag,
		    hb_tag_t        feature_tag,
		    hb_position_t  *min,
		    hb_position_t  *max) ध्रुव
  {
    ध्रुव BaseCoord *min_coord, *max_coord;
    यदि (!get_axis (direction).get_min_max (script_tag, language_tag, feature_tag,
					   &min_coord, &max_coord))
      प्रत्यागम असत्य;

    ध्रुव ItemVariationStore &var_store = get_var_store ();
    यदि (likely (min && min_coord)) *min = min_coord->get_coord (font, var_store, direction);
    यदि (likely (max && max_coord)) *max = max_coord->get_coord (font, var_store, direction);
    प्रत्यागम सत्य;
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (likely (c->check_struct (एतत्) &&
			  hb_barrier () &&
			  likely (version.major == 1) &&
			  hAxis.sanitize (c, एतत्) &&
			  vAxis.sanitize (c, एतत्) &&
			  (version.to_int () < 0x00010001u || varStore.sanitize (c, एतत्))));
  }

  रक्षित:
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
