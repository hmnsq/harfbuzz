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

#અન્તર્ભાવ "hb-open-type.hh"
#અન્તર્ભાવ "hb-ot-layout-common.hh"

નામાકાશ OT {

/*
 * BASE -- Baseline
 * https://docs.microsoft.com/en-us/typography/opentype/spec/base
 */

સંરચના BaseCoordFormat1
{
  hb_position_t get_coord (hb_font_t *font, hb_direction_t direction) અચલ
  {
    પાછા HB_DIRECTION_IS_HORIZONTAL (direction) ? font->em_scale_y (coordinate) : font->em_scale_x (coordinate);
  }

  તાર્કિક subset (hb_subset_context_t *c) અચલ
  {
    TRACE_SUBSET (આ);
    return_trace ((તાર્કિક) c->serializer->embed (*આ));
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ));
  }

  સુરક્ષિત:
  HBUINT16	format;		/* Format identifier--format = 1 */
  FWORD		coordinate;	/* X or Y value, in design units */
  જાહેર:
  DEFINE_SIZE_STATIC (4);
};

template <typename Types>
સંરચના BaseCoordFormat2_4
{
  hb_position_t get_coord (hb_font_t *font, hb_direction_t direction) અચલ
  {
    /* TODO */
    પાછા HB_DIRECTION_IS_HORIZONTAL (direction) ? font->em_scale_y (coordinate) : font->em_scale_x (coordinate);
  }

  તાર્કિક subset (hb_subset_context_t *c) અચલ
  {
    TRACE_SUBSET (આ);
    auto *out = c->serializer->embed (*આ);
    જો (unlikely (!out)) return_trace (ખોટું);

    hb_codepoint_t *new_gid;
    જો (!c->plan->glyph_map->has (referenceGlyph, &new_gid))
      return_trace (ખોટું);

    return_trace (c->serializer->check_assign (out->referenceGlyph,
                                               *new_gid,
                                               HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ));
  }

  સુરક્ષિત:
  HBUINT16	format;		/* Format identifier--format = 2 or 4 */
  FWORD		coordinate;	/* X or Y value, in design units */
  typename Types::HBGlyphID
		referenceGlyph;	/* Glyph ID of control glyph */
  HBUINT16	coordPoint;	/* Index of contour point on the
				 * reference glyph */
  જાહેર:
  DEFINE_SIZE_STATIC (6 + Types::HBGlyphID::static_size);
};

સંરચના BaseCoordFormat3
{
  hb_position_t get_coord (hb_font_t *font,
			   અચલ ItemVariationStore &var_store,
			   hb_direction_t direction) અચલ
  {
    અચલ Device &device = આ+deviceTable;

    પાછા HB_DIRECTION_IS_HORIZONTAL (direction)
	 ? hb_saturate_add (font->em_scale_y (coordinate), device.get_y_delta (font, var_store))
	 : hb_saturate_add (font->em_scale_x (coordinate), device.get_x_delta (font, var_store));
  }

  શૂન્ય collect_variation_indices (hb_set_t& varidx_set /* OUT */) અચલ
  {
    unsigned varidx = (આ+deviceTable).get_variation_index ();
    varidx_set.add (varidx);
  }

  તાર્કિક subset (hb_subset_context_t *c) અચલ
  {
    TRACE_SUBSET (આ);
    auto *out = c->serializer->embed (*આ);
    જો (unlikely (!out)) return_trace (ખોટું);

    જો (!c->plan->pinned_at_default)
    {
      unsigned var_idx = (આ+deviceTable).get_variation_index ();
      જો (var_idx != VarIdx::NO_VARIATION)
      {
        hb_pair_t<unsigned, પૂર્ણાંક> *v;
        જો (!c->plan->base_variation_idx_map.has (var_idx, &v))
          return_trace (ખોટું);
        
        જો (unlikely (!c->serializer->check_assign (out->coordinate, coordinate + hb_second (*v),
                                                    HB_SERIALIZE_ERROR_INT_OVERFLOW)))
          return_trace (ખોટું);
      }
    }
    
    out->deviceTable.serialize_copy (c->serializer, deviceTable,
                                     આ, 0,
                                     hb_serialize_context_t::Head,
                                     &c->plan->base_variation_idx_map);
    return_trace (સાચું);
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (likely (c->check_struct (આ) &&
			  deviceTable.sanitize (c, આ)));
  }

  સુરક્ષિત:
  HBUINT16	format;		/* Format identifier--format = 3 */
  FWORD		coordinate;	/* X or Y value, in design units */
  Offset16To<Device>
		deviceTable;	/* Offset to Device table for X or
				 * Y value, from beginning of
				 * BaseCoord table (may be NULL). */
  જાહેર:
  DEFINE_SIZE_STATIC (6);
};

સંરચના BaseCoord
{
  તાર્કિક has_data () અચલ { પાછા u.format.v; }

  hb_position_t get_coord (hb_font_t            *font,
			   અચલ ItemVariationStore &var_store,
			   hb_direction_t        direction) અચલ
  {
    પસંદગી (u.format.v) {
    સ્થિતિ 1: hb_barrier (); પાછા u.format1.get_coord (font, direction);
    સ્થિતિ 2: hb_barrier (); પાછા u.format2.get_coord (font, direction);
    સ્થિતિ 3: hb_barrier (); પાછા u.format3.get_coord (font, var_store, direction);
#ifndef HB_NO_BEYOND_64K
    સ્થિતિ 4: hb_barrier (); પાછા u.format4.get_coord (font, direction);
#endif
    મૂળભૂત:પાછા 0;
    }
  }

  શૂન્ય collect_variation_indices (hb_set_t& varidx_set /* OUT */) અચલ
  {
    પસંદગી (u.format.v) {
    સ્થિતિ 3: hb_barrier (); u.format3.collect_variation_indices (varidx_set); પાછા;
    મૂળભૂત:પાછા;
    }
  }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) અચલ
  {
    જો (unlikely (!c->may_dispatch (આ, &u.format.v))) પાછા c->no_dispatch_return_કિંમત ();
    TRACE_DISPATCH (આ, u.format.v);
    પસંદગી (u.format.v) {
    સ્થિતિ 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    સ્થિતિ 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    સ્થિતિ 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
#ifndef HB_NO_BEYOND_64K
    સ્થિતિ 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
#endif
    મૂળભૂત:return_trace (c->default_return_કિંમત ());
    }
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    જો (unlikely (!u.format.v.sanitize (c))) return_trace (ખોટું);
    hb_barrier ();
    પસંદગી (u.format.v) {
    સ્થિતિ 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    સ્થિતિ 2: hb_barrier (); return_trace (u.format2.sanitize (c));
    સ્થિતિ 3: hb_barrier (); return_trace (u.format3.sanitize (c));
#ifndef HB_NO_BEYOND_64K
    સ્થિતિ 4: hb_barrier (); return_trace (u.format4.sanitize (c));
#endif
    મૂળભૂત:return_trace (ખોટું);
    }
  }

  સુરક્ષિત:
  સંઘ {
  સંરચના { HBUINT16 v; }	format;
  BaseCoordFormat1	format1;
  BaseCoordFormat2_4<Layout::SmallTypes>
			format2;
  BaseCoordFormat3	format3;
#ifndef HB_NO_BEYOND_64K
  BaseCoordFormat2_4<Layout::MediumTypes>
			format4;
#endif
  } u;
  જાહેર:
  DEFINE_SIZE_UNION (2, format.v);
};

સંરચના FeatMinMaxRecord
{
  પૂર્ણાંક cmp (hb_tag_t key) અચલ { પાછા tag.cmp (key); }

  તાર્કિક has_data () અચલ { પાછા tag; }

  hb_tag_t get_feature_tag () અચલ { પાછા tag; }

  શૂન્ય get_min_max (અચલ BaseCoord **min, અચલ BaseCoord **max) અચલ
  {
    જો (likely (min)) *min = &(આ+minCoord);
    જો (likely (max)) *max = &(આ+maxCoord);
  }

  શૂન્ય collect_variation_indices (અચલ hb_subset_plan_t* plan,
                                  અચલ શૂન્ય *base,
                                  hb_set_t& varidx_set /* OUT */) અચલ
  {
    જો (!plan->layout_features.has (tag))
      પાછા;

    (base+minCoord).collect_variation_indices (varidx_set);
    (base+maxCoord).collect_variation_indices (varidx_set);
  }

  તાર્કિક subset (hb_subset_context_t *c,
               અચલ શૂન્ય *base) અચલ
  {
    TRACE_SUBSET (આ);
    auto *out = c->serializer->embed (*આ);
    જો (unlikely (!out)) return_trace (ખોટું);

    તાર્કિક ret = out->minCoord.serialize_subset (c, minCoord, base);
    ret |= out->maxCoord.serialize_subset (c, maxCoord, base);
    return_trace (ret);
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c, અચલ શૂન્ય *base) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (likely (c->check_struct (આ) &&
			  minCoord.sanitize (c, base) &&
			  maxCoord.sanitize (c, base)));
  }

  સુરક્ષિત:
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
  જાહેર:
  DEFINE_SIZE_STATIC (8);
};

સંરચના MinMax
{
  શૂન્ય get_min_max (hb_tag_t          feature_tag,
		    અચલ BaseCoord **min,
		    અચલ BaseCoord **max) અચલ
  {
    અચલ FeatMinMaxRecord &minMaxCoord = featMinMaxRecords.bsearch (feature_tag);
    જો (minMaxCoord.has_data ())
      minMaxCoord.get_min_max (min, max);
    નહીંતર
    {
      જો (likely (min)) *min = &(આ+minCoord);
      જો (likely (max)) *max = &(આ+maxCoord);
    }
  }

  શૂન્ય collect_variation_indices (અચલ hb_subset_plan_t* plan,
                                  hb_set_t& varidx_set /* OUT */) અચલ
  {
    (આ+minCoord).collect_variation_indices (varidx_set);
    (આ+maxCoord).collect_variation_indices (varidx_set);
    માટે (અચલ FeatMinMaxRecord& record : featMinMaxRecords)
      record.collect_variation_indices (plan, આ, varidx_set);
  }

  તાર્કિક subset (hb_subset_context_t *c) અચલ
  {
    TRACE_SUBSET (આ);
    auto *out = c->serializer->start_embed (*આ);
    જો (unlikely (!out || !c->serializer->extend_min (out))) return_trace (ખોટું);

    તાર્કિક ret = out->minCoord.serialize_subset (c, minCoord, આ);
    ret |= out->maxCoord.serialize_subset (c, maxCoord, આ);

    unsigned len = 0;
    માટે (અચલ FeatMinMaxRecord& _ : featMinMaxRecords)
    {
      hb_tag_t feature_tag = _.get_feature_tag ();
      જો (!c->plan->layout_features.has (feature_tag))
        ચાલુ;

      auto snap = c->serializer->snapshot ();
      જો (!_.subset (c, આ)) c->serializer->revert (snap);
      નહીંતર len++;
    }

    જો (len > 0) ret = સાચું;
    જો (!ret) return_trace (ખોટું);
    return_trace (c->serializer->check_assign (out->featMinMaxRecords.len, len,
                                               HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (likely (c->check_struct (આ) &&
			  minCoord.sanitize (c, આ) &&
			  maxCoord.sanitize (c, આ) &&
			  featMinMaxRecords.sanitize (c, આ)));
  }

  સુરક્ષિત:
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
  જાહેર:
  DEFINE_SIZE_ARRAY (6, featMinMaxRecords);
};

સંરચના Base_કિંમતો
{
  અચલ BaseCoord &get_base_coord (પૂર્ણાંક baseline_tag_index) અચલ
  {
    જો (baseline_tag_index == -1) baseline_tag_index = defaultIndex;
    પાછા આ+baseCoords[baseline_tag_index];
  }

  શૂન્ય collect_variation_indices (hb_set_t& varidx_set /* OUT */) અચલ
  {
    માટે (અચલ auto& _ : baseCoords)
      (આ+_).collect_variation_indices (varidx_set);
  }

  તાર્કિક subset (hb_subset_context_t *c) અચલ
  {
    TRACE_SUBSET (આ);
    auto *out = c->serializer->start_embed (*આ);
    જો (unlikely (!out || !c->serializer->extend_min (out))) return_trace (ખોટું);
    out->defaultIndex = defaultIndex;

    તાર્કિક ret = ખોટું;
    માટે (અચલ auto& _ : baseCoords)
    {
      auto *o = out->baseCoords.serialize_append (c->serializer);
      જો (unlikely (!o)) return_trace (ખોટું);
       ret |= o->serialize_subset (c, _, આ);
    }

    return_trace (ret);
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (likely (c->check_struct (આ) &&
			  baseCoords.sanitize (c, આ)));
  }

  સુરક્ષિત:
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
  જાહેર:
  DEFINE_SIZE_ARRAY (4, baseCoords);
};

સંરચના BaseLangSysRecord
{
  પૂર્ણાંક cmp (hb_tag_t key) અચલ { પાછા baseLangSysTag.cmp (key); }

  તાર્કિક has_data () અચલ { પાછા baseLangSysTag; }

  અચલ MinMax &get_min_max (અચલ શૂન્ય* base) અચલ { પાછા base+minMax; }

  શૂન્ય collect_variation_indices (અચલ શૂન્ય* base,
                                  અચલ hb_subset_plan_t* plan,
                                  hb_set_t& varidx_set /* OUT */) અચલ
  { (base+minMax).collect_variation_indices (plan, varidx_set); }

  તાર્કિક subset (hb_subset_context_t *c,
               અચલ શૂન્ય *base) અચલ
  {
    TRACE_SUBSET (આ);
    auto *out = c->serializer->embed (*આ);
    જો (unlikely (!out)) return_trace (ખોટું);

    return_trace (out->minMax.serialize_subset (c, minMax, base));
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c, અચલ શૂન્ય *base) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (likely (c->check_struct (આ) &&
			  minMax.sanitize (c, base)));
  }

  સુરક્ષિત:
  Tag		baseLangSysTag;	/* 4-byte language system identification tag */
  Offset16To<MinMax>
		minMax;		/* Offset to MinMax table, from beginning
				 * of BaseScript table */
  જાહેર:
  DEFINE_SIZE_STATIC (6);
};

સંરચના BaseScript
{
  અચલ MinMax &get_min_max (hb_tag_t language_tag) અચલ
  {
    અચલ BaseLangSysRecord& record = baseLangSysRecords.bsearch (language_tag);
    પાછા record.has_data () ? record.get_min_max (આ) : આ+defaultMinMax;
  }

  અચલ BaseCoord &get_base_coord (પૂર્ણાંક baseline_tag_index) અચલ
  { પાછા (આ+base_કિંમતો).get_base_coord (baseline_tag_index); }

  તાર્કિક has_કિંમતો () અચલ { પાછા base_કિંમતો; }
  તાર્કિક has_min_max () અચલ { પાછા defaultMinMax || baseLangSysRecords; }

  શૂન્ય collect_variation_indices (અચલ hb_subset_plan_t* plan,
                                  hb_set_t& varidx_set /* OUT */) અચલ
  {
    (આ+base_કિંમતો).collect_variation_indices (varidx_set);
    (આ+defaultMinMax).collect_variation_indices (plan, varidx_set);
    
    માટે (અચલ BaseLangSysRecord& _ : baseLangSysRecords)
      _.collect_variation_indices (આ, plan, varidx_set);
  }

  તાર્કિક subset (hb_subset_context_t *c) અચલ
  {
    TRACE_SUBSET (આ);
    auto *out = c->serializer->start_embed (*આ);
    જો (unlikely (!out || !c->serializer->extend_min (out))) return_trace (ખોટું);

    તાર્કિક ret = out->base_કિંમતો.serialize_subset (c, base_કિંમતો, આ);
    ret |= out->defaultMinMax.serialize_subset (c, defaultMinMax, આ);

    unsigned len = 0;
    માટે (અચલ auto& _ : baseLangSysRecords)
    {
      auto snap = c->serializer->snapshot ();
      જો (_.subset (c, આ)) {
        ret = સાચું;
        len++;
      }
      નહીંતર c->serializer->revert (snap);
    }

    જો (!ret) return_trace (ખોટું);
    return_trace (c->serializer->check_assign (out->baseLangSysRecords.len, len,
                                               HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (likely (c->check_struct (આ) &&
			  base_કિંમતો.sanitize (c, આ) &&
			  defaultMinMax.sanitize (c, આ) &&
			  baseLangSysRecords.sanitize (c, આ)));
  }

  સુરક્ષિત:
  Offset16To<Base_કિંમતો>
		base_કિંમતો;	/* Offset to BaseValues table, from beginning
				 * of BaseScript table (may be NULL) */
  Offset16To<MinMax>
		defaultMinMax;	/* Offset to MinMax table, from beginning of
				 * BaseScript table (may be NULL) */
  SortedArray16Of<BaseLangSysRecord>
		baseLangSysRecords;
				/* Number of BaseLangSysRecords
				 * defined — may be zero (0) */

  જાહેર:
  DEFINE_SIZE_ARRAY (6, baseLangSysRecords);
};

સંરચના BaseScriptList;
સંરચના BaseScriptRecord
{
  પૂર્ણાંક cmp (hb_tag_t key) અચલ { પાછા baseScriptTag.cmp (key); }

  તાર્કિક has_data () અચલ { પાછા baseScriptTag; }

  hb_tag_t get_script_tag () અચલ { પાછા baseScriptTag; }

  અચલ BaseScript &get_base_script (અચલ BaseScriptList *list) અચલ
  { પાછા list+baseScript; }

  શૂન્ય collect_variation_indices (અચલ hb_subset_plan_t* plan,
                                  અચલ શૂન્ય* list,
                                  hb_set_t& varidx_set /* OUT */) અચલ
  {
    જો (!plan->layout_scripts.has (baseScriptTag))
      પાછા;

    (list+baseScript).collect_variation_indices (plan, varidx_set);
  }

  તાર્કિક subset (hb_subset_context_t *c,
               અચલ શૂન્ય *base) અચલ
  {
    TRACE_SUBSET (આ);
    auto *out = c->serializer->embed (*આ);
    જો (unlikely (!out)) return_trace (ખોટું);

    return_trace (out->baseScript.serialize_subset (c, baseScript, base));
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c, અચલ શૂન્ય *base) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (likely (c->check_struct (આ) &&
			  baseScript.sanitize (c, base)));
  }

  સુરક્ષિત:
  Tag		baseScriptTag;	/* 4-byte script identification tag */
  Offset16To<BaseScript>
		baseScript;	/* Offset to BaseScript table, from beginning
				 * of BaseScriptList */

  જાહેર:
  DEFINE_SIZE_STATIC (6);
};

સંરચના BaseScriptList
{
  અચલ BaseScript &get_base_script (hb_tag_t script) અચલ
  {
    અચલ BaseScriptRecord *record = &baseScriptRecords.bsearch (script);
    જો (!record->has_data ()) record = &baseScriptRecords.bsearch (HB_TAG ('D','F','L','T'));
    પાછા record->has_data () ? record->get_base_script (આ) : Null (BaseScript);
  }

  શૂન્ય collect_variation_indices (અચલ hb_subset_plan_t* plan,
                                  hb_set_t& varidx_set /* OUT */) અચલ
  {
    માટે (અચલ BaseScriptRecord& _ : baseScriptRecords)
      _.collect_variation_indices (plan, આ, varidx_set);
  }

  તાર્કિક subset (hb_subset_context_t *c) અચલ
  {
    TRACE_SUBSET (આ);
    auto *out = c->serializer->start_embed (*આ);
    જો (unlikely (!out || !c->serializer->extend_min (out))) return_trace (ખોટું);

    unsigned len = 0;
    માટે (અચલ BaseScriptRecord& _ : baseScriptRecords)
    {
      hb_tag_t script_tag = _.get_script_tag ();
      જો (!c->plan->layout_scripts.has (script_tag))
        ચાલુ;

      auto snap = c->serializer->snapshot ();
      જો (!_.subset (c, આ)) c->serializer->revert (snap);
      નહીંતર len++;
    }

    જો (!len) return_trace (ખોટું);
    return_trace (c->serializer->check_assign (out->baseScriptRecords.len, len,
                                               HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ) &&
		  baseScriptRecords.sanitize (c, આ));
  }

  સુરક્ષિત:
  SortedArray16Of<BaseScriptRecord>
			baseScriptRecords;

  જાહેર:
  DEFINE_SIZE_ARRAY (2, baseScriptRecords);
};

સંરચના Axis
{
  તાર્કિક get_baseline (hb_tag_t          baseline_tag,
		     hb_tag_t          script_tag,
		     hb_tag_t          language_tag,
		     અચલ BaseCoord **coord) અચલ
  {
    અચલ BaseScript &base_script = (આ+baseScriptList).get_base_script (script_tag);
    જો (!base_script.has_કિંમતો ())
    {
      *coord = nullptr;
      પાછા ખોટું;
    }

    જો (likely (coord))
    {
      unsigned પૂર્ણાંક tag_index = 0;
      જો (!(આ+baseTagList).bfind (baseline_tag, &tag_index))
      {
        *coord = nullptr;
        પાછા ખોટું;
      }
      *coord = &base_script.get_base_coord (tag_index);
    }

    પાછા સાચું;
  }

  તાર્કિક get_min_max (hb_tag_t          script_tag,
		    hb_tag_t          language_tag,
		    hb_tag_t          feature_tag,
		    અચલ BaseCoord **min_coord,
		    અચલ BaseCoord **max_coord) અચલ
  {
    અચલ BaseScript &base_script = (આ+baseScriptList).get_base_script (script_tag);
    જો (!base_script.has_min_max ())
    {
      *min_coord = *max_coord = nullptr;
      પાછા ખોટું;
    }

    base_script.get_min_max (language_tag).get_min_max (feature_tag, min_coord, max_coord);

    પાછા સાચું;
  }

  શૂન્ય collect_variation_indices (અચલ hb_subset_plan_t* plan,
                                  hb_set_t& varidx_set /* OUT */) અચલ
  { (આ+baseScriptList).collect_variation_indices (plan, varidx_set); }

  તાર્કિક subset (hb_subset_context_t *c) અચલ
  {
    TRACE_SUBSET (આ);
    auto *out = c->serializer->embed (*આ);
    જો (unlikely (!out)) return_trace (ખોટું);

    તાર્કિક ret = out->baseTagList.serialize_copy (c->serializer, baseTagList, આ);
    ret |= out->baseScriptList.serialize_subset (c, baseScriptList, આ);
    return_trace (ret);
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (likely (c->check_struct (આ) &&
			  baseTagList.sanitize (c, આ) &&
			  baseScriptList.sanitize (c, આ)));
  }

  સુરક્ષિત:
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

  જાહેર:
  DEFINE_SIZE_STATIC (4);
};

સંરચના BASE
{
  સ્થિર constexpr hb_tag_t tableTag = HB_OT_TAG_BASE;

  અચલ Axis &get_axis (hb_direction_t direction) અચલ
  { પાછા HB_DIRECTION_IS_VERTICAL (direction) ? આ+vAxis : આ+hAxis; }

  તાર્કિક has_var_store () અચલ
  { પાછા version.to_int () >= 0x00010001u && varStore != 0; }

  અચલ ItemVariationStore &get_var_store () અચલ
  { પાછા version.to_int () < 0x00010001u ? Null (ItemVariationStore) : આ+varStore; }

  શૂન્ય collect_variation_indices (અચલ hb_subset_plan_t* plan,
                                  hb_set_t& varidx_set /* OUT */) અચલ
  {
    (આ+hAxis).collect_variation_indices (plan, varidx_set);
    (આ+vAxis).collect_variation_indices (plan, varidx_set);
  }

  તાર્કિક subset_varstore (hb_subset_context_t *c,
                        BASE *out /* OUT */) અચલ
  {
    TRACE_SUBSET (આ);
    જો (!c->serializer->allocate_size<Offset32To<ItemVariationStore>> (Offset32To<ItemVariationStore>::static_size))
        return_trace (ખોટું);
    જો (!c->plan->normalized_coords)
      return_trace (out->varStore.serialize_subset (c, varStore, આ, c->plan->base_varstore_inner_maps.as_array ()));

    જો (c->plan->all_axes_pinned)
      return_trace (સાચું);

    item_variations_t item_vars;
    જો (!item_vars.instantiate (આ+varStore, c->plan, સાચું, સાચું,
                                c->plan->base_varstore_inner_maps.as_array ()))
      return_trace (ખોટું);

    જો (!out->varStore.serialize_serialize (c->serializer,
                                            item_vars.has_long_word (),
                                            c->plan->axis_tags,
                                            item_vars.get_region_list (),
                                            item_vars.get_vardata_encodings ()))
      return_trace (ખોટું);

    અચલ hb_map_t &varidx_map = item_vars.get_varidx_map ();
    /* base_variation_idx_map in the plan is old_varidx->(varidx, delta)
     * mapping, new varidx is generated for subsetting, we need to remap this
     * after instancing */
    માટે (auto _ : c->plan->base_variation_idx_map.iter_ref ())
    {
      uint32_t varidx = _.second.first;
      uint32_t *new_varidx;
      જો (varidx_map.has (varidx, &new_varidx))
        _.second.first = *new_varidx;
      નહીંતર
        _.second.first = HB_OT_LAYOUT_NO_VARIATIONS_INDEX;
    }
    return_trace (સાચું);
  }

  તાર્કિક subset (hb_subset_context_t *c) અચલ
  {
    TRACE_SUBSET (આ);
    auto *out = c->serializer->start_embed (*આ);
    જો (unlikely (!out || !c->serializer->extend_min (out))) return_trace (ખોટું);

    out->version = version;
    તાર્કિક ret = ખોટું;
    જો (version.to_int () >= 0x00010001u) ret |= subset_varstore (c, out);

    ret |= out->hAxis.serialize_subset (c, hAxis, આ);
    ret |= out->vAxis.serialize_subset (c, vAxis, આ);

    return_trace (ret);
  }

  તાર્કિક get_baseline (hb_font_t      *font,
		     hb_tag_t        baseline_tag,
		     hb_direction_t  direction,
		     hb_tag_t        script_tag,
		     hb_tag_t        language_tag,
		     hb_position_t  *base) અચલ
  {
    અચલ BaseCoord *base_coord = nullptr;
    જો (unlikely (!get_axis (direction).get_baseline (baseline_tag, script_tag, language_tag, &base_coord) ||
		  !base_coord || !base_coord->has_data ()))
      પાછા ખોટું;

    જો (likely (base))
      *base = base_coord->get_coord (font, get_var_store (), direction);

    પાછા સાચું;
  }

  તાર્કિક get_min_max (hb_font_t      *font,
		    hb_direction_t  direction,
		    hb_tag_t        script_tag,
		    hb_tag_t        language_tag,
		    hb_tag_t        feature_tag,
		    hb_position_t  *min,
		    hb_position_t  *max) અચલ
  {
    અચલ BaseCoord *min_coord, *max_coord;
    જો (!get_axis (direction).get_min_max (script_tag, language_tag, feature_tag,
					   &min_coord, &max_coord))
      પાછા ખોટું;

    અચલ ItemVariationStore &var_store = get_var_store ();
    જો (likely (min && min_coord)) *min = min_coord->get_coord (font, var_store, direction);
    જો (likely (max && max_coord)) *max = max_coord->get_coord (font, var_store, direction);
    પાછા સાચું;
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (likely (c->check_struct (આ) &&
			  hb_barrier () &&
			  likely (version.major == 1) &&
			  hAxis.sanitize (c, આ) &&
			  vAxis.sanitize (c, આ) &&
			  (version.to_int () < 0x00010001u || varStore.sanitize (c, આ))));
  }

  સુરક્ષિત:
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
  જાહેર:
  DEFINE_SIZE_MIN (8);
};


} /* namespace OT */


#endif /* HB_OT_LAYOUT_BASE_TABLE_HH */
