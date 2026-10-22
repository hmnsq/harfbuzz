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

#അന്തര്ഭാവ "hb-open-type.hh"
#അന്തര്ഭാവ "hb-ot-layout-common.hh"

നാമാകാശ OT {

/*
 * BASE -- Baseline
 * https://docs.microsoft.com/en-us/typography/opentype/spec/base
 */

ഘടന BaseCoordFormat1
{
  hb_position_t get_coord (hb_font_t *font, hb_direction_t direction) സ്ഥിരം
  {
    തിരികെ HB_DIRECTION_IS_HORIZONTAL (direction) ? font->em_scale_y (coordinate) : font->em_scale_x (coordinate);
  }

  തർക്കം subset (hb_subset_context_t *c) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);
    return_trace ((തർക്കം) c->serializer->embed (*ഇത്));
  }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_struct (ഇത്));
  }

  സംരക്ഷിത:
  HBUINT16	format;		/* Format identifier--format = 1 */
  FWORD		coordinate;	/* X or Y value, in design units */
  പൊതു:
  DEFINE_SIZE_STATIC (4);
};

template <typename Types>
ഘടന BaseCoordFormat2_4
{
  hb_position_t get_coord (hb_font_t *font, hb_direction_t direction) സ്ഥിരം
  {
    /* TODO */
    തിരികെ HB_DIRECTION_IS_HORIZONTAL (direction) ? font->em_scale_y (coordinate) : font->em_scale_x (coordinate);
  }

  തർക്കം subset (hb_subset_context_t *c) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);
    auto *out = c->serializer->embed (*ഇത്);
    എങ്കിൽ (unlikely (!out)) return_trace (അസത്യം);

    hb_codepoint_t *new_gid;
    എങ്കിൽ (!c->plan->glyph_map->has (referenceGlyph, &new_gid))
      return_trace (അസത്യം);

    return_trace (c->serializer->check_assign (out->referenceGlyph,
                                               *new_gid,
                                               HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_struct (ഇത്));
  }

  സംരക്ഷിത:
  HBUINT16	format;		/* Format identifier--format = 2 or 4 */
  FWORD		coordinate;	/* X or Y value, in design units */
  typename Types::HBGlyphID
		referenceGlyph;	/* Glyph ID of control glyph */
  HBUINT16	coordPoint;	/* Index of contour point on the
				 * reference glyph */
  പൊതു:
  DEFINE_SIZE_STATIC (6 + Types::HBGlyphID::static_size);
};

ഘടന BaseCoordFormat3
{
  hb_position_t get_coord (hb_font_t *font,
			   സ്ഥിരം ItemVariationStore &var_store,
			   hb_direction_t direction) സ്ഥിരം
  {
    സ്ഥിരം Device &device = ഇത്+deviceTable;

    തിരികെ HB_DIRECTION_IS_HORIZONTAL (direction)
	 ? hb_saturate_add (font->em_scale_y (coordinate), device.get_y_delta (font, var_store))
	 : hb_saturate_add (font->em_scale_x (coordinate), device.get_x_delta (font, var_store));
  }

  ശൂന്യം collect_variation_indices (hb_set_t& varidx_set /* OUT */) സ്ഥിരം
  {
    unsigned varidx = (ഇത്+deviceTable).get_variation_index ();
    varidx_set.add (varidx);
  }

  തർക്കം subset (hb_subset_context_t *c) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);
    auto *out = c->serializer->embed (*ഇത്);
    എങ്കിൽ (unlikely (!out)) return_trace (അസത്യം);

    എങ്കിൽ (!c->plan->pinned_at_default)
    {
      unsigned var_idx = (ഇത്+deviceTable).get_variation_index ();
      എങ്കിൽ (var_idx != VarIdx::NO_VARIATION)
      {
        hb_pair_t<unsigned, പൂർണ്ണസംഖ്യ> *v;
        എങ്കിൽ (!c->plan->base_variation_idx_map.has (var_idx, &v))
          return_trace (അസത്യം);
        
        എങ്കിൽ (unlikely (!c->serializer->check_assign (out->coordinate, coordinate + hb_second (*v),
                                                    HB_SERIALIZE_ERROR_INT_OVERFLOW)))
          return_trace (അസത്യം);
      }
    }
    
    out->deviceTable.serialize_copy (c->serializer, deviceTable,
                                     ഇത്, 0,
                                     hb_serialize_context_t::Head,
                                     &c->plan->base_variation_idx_map);
    return_trace (സത്യം);
  }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (likely (c->check_struct (ഇത്) &&
			  deviceTable.sanitize (c, ഇത്)));
  }

  സംരക്ഷിത:
  HBUINT16	format;		/* Format identifier--format = 3 */
  FWORD		coordinate;	/* X or Y value, in design units */
  Offset16To<Device>
		deviceTable;	/* Offset to Device table for X or
				 * Y value, from beginning of
				 * BaseCoord table (may be NULL). */
  പൊതു:
  DEFINE_SIZE_STATIC (6);
};

ഘടന BaseCoord
{
  തർക്കം has_data () സ്ഥിരം { തിരികെ u.format.v; }

  hb_position_t get_coord (hb_font_t            *font,
			   സ്ഥിരം ItemVariationStore &var_store,
			   hb_direction_t        direction) സ്ഥിരം
  {
    തിരഞ്ഞെടുപ്പ് (u.format.v) {
    അവസ്ഥ 1: hb_barrier (); തിരികെ u.format1.get_coord (font, direction);
    അവസ്ഥ 2: hb_barrier (); തിരികെ u.format2.get_coord (font, direction);
    അവസ്ഥ 3: hb_barrier (); തിരികെ u.format3.get_coord (font, var_store, direction);
#ifndef HB_NO_BEYOND_64K
    അവസ്ഥ 4: hb_barrier (); തിരികെ u.format4.get_coord (font, direction);
#endif
    സ്ഥിരസ്ഥിതി:തിരികെ 0;
    }
  }

  ശൂന്യം collect_variation_indices (hb_set_t& varidx_set /* OUT */) സ്ഥിരം
  {
    തിരഞ്ഞെടുപ്പ് (u.format.v) {
    അവസ്ഥ 3: hb_barrier (); u.format3.collect_variation_indices (varidx_set); തിരികെ;
    സ്ഥിരസ്ഥിതി:തിരികെ;
    }
  }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) സ്ഥിരം
  {
    എങ്കിൽ (unlikely (!c->may_dispatch (ഇത്, &u.format.v))) തിരികെ c->no_dispatch_return_മൂല്യം ();
    TRACE_DISPATCH (ഇത്, u.format.v);
    തിരഞ്ഞെടുപ്പ് (u.format.v) {
    അവസ്ഥ 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    അവസ്ഥ 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    അവസ്ഥ 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
#ifndef HB_NO_BEYOND_64K
    അവസ്ഥ 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
#endif
    സ്ഥിരസ്ഥിതി:return_trace (c->default_return_മൂല്യം ());
    }
  }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    എങ്കിൽ (unlikely (!u.format.v.sanitize (c))) return_trace (അസത്യം);
    hb_barrier ();
    തിരഞ്ഞെടുപ്പ് (u.format.v) {
    അവസ്ഥ 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    അവസ്ഥ 2: hb_barrier (); return_trace (u.format2.sanitize (c));
    അവസ്ഥ 3: hb_barrier (); return_trace (u.format3.sanitize (c));
#ifndef HB_NO_BEYOND_64K
    അവസ്ഥ 4: hb_barrier (); return_trace (u.format4.sanitize (c));
#endif
    സ്ഥിരസ്ഥിതി:return_trace (അസത്യം);
    }
  }

  സംരക്ഷിത:
  സംഘം {
  ഘടന { HBUINT16 v; }	format;
  BaseCoordFormat1	format1;
  BaseCoordFormat2_4<Layout::SmallTypes>
			format2;
  BaseCoordFormat3	format3;
#ifndef HB_NO_BEYOND_64K
  BaseCoordFormat2_4<Layout::MediumTypes>
			format4;
#endif
  } u;
  പൊതു:
  DEFINE_SIZE_UNION (2, format.v);
};

ഘടന FeatMinMaxRecord
{
  പൂർണ്ണസംഖ്യ cmp (hb_tag_t key) സ്ഥിരം { തിരികെ tag.cmp (key); }

  തർക്കം has_data () സ്ഥിരം { തിരികെ tag; }

  hb_tag_t get_feature_tag () സ്ഥിരം { തിരികെ tag; }

  ശൂന്യം get_min_max (സ്ഥിരം BaseCoord **min, സ്ഥിരം BaseCoord **max) സ്ഥിരം
  {
    എങ്കിൽ (likely (min)) *min = &(ഇത്+minCoord);
    എങ്കിൽ (likely (max)) *max = &(ഇത്+maxCoord);
  }

  ശൂന്യം collect_variation_indices (സ്ഥിരം hb_subset_plan_t* plan,
                                  സ്ഥിരം ശൂന്യം *base,
                                  hb_set_t& varidx_set /* OUT */) സ്ഥിരം
  {
    എങ്കിൽ (!plan->layout_features.has (tag))
      തിരികെ;

    (base+minCoord).collect_variation_indices (varidx_set);
    (base+maxCoord).collect_variation_indices (varidx_set);
  }

  തർക്കം subset (hb_subset_context_t *c,
               സ്ഥിരം ശൂന്യം *base) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);
    auto *out = c->serializer->embed (*ഇത്);
    എങ്കിൽ (unlikely (!out)) return_trace (അസത്യം);

    തർക്കം ret = out->minCoord.serialize_subset (c, minCoord, base);
    ret |= out->maxCoord.serialize_subset (c, maxCoord, base);
    return_trace (ret);
  }

  തർക്കം sanitize (hb_sanitize_context_t *c, സ്ഥിരം ശൂന്യം *base) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (likely (c->check_struct (ഇത്) &&
			  minCoord.sanitize (c, base) &&
			  maxCoord.sanitize (c, base)));
  }

  സംരക്ഷിത:
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
  പൊതു:
  DEFINE_SIZE_STATIC (8);
};

ഘടന MinMax
{
  ശൂന്യം get_min_max (hb_tag_t          feature_tag,
		    സ്ഥിരം BaseCoord **min,
		    സ്ഥിരം BaseCoord **max) സ്ഥിരം
  {
    സ്ഥിരം FeatMinMaxRecord &minMaxCoord = featMinMaxRecords.bsearch (feature_tag);
    എങ്കിൽ (minMaxCoord.has_data ())
      minMaxCoord.get_min_max (min, max);
    അല്ലെങ്കിൽ
    {
      എങ്കിൽ (likely (min)) *min = &(ഇത്+minCoord);
      എങ്കിൽ (likely (max)) *max = &(ഇത്+maxCoord);
    }
  }

  ശൂന്യം collect_variation_indices (സ്ഥിരം hb_subset_plan_t* plan,
                                  hb_set_t& varidx_set /* OUT */) സ്ഥിരം
  {
    (ഇത്+minCoord).collect_variation_indices (varidx_set);
    (ഇത്+maxCoord).collect_variation_indices (varidx_set);
    വേണ്ടി (സ്ഥിരം FeatMinMaxRecord& record : featMinMaxRecords)
      record.collect_variation_indices (plan, ഇത്, varidx_set);
  }

  തർക്കം subset (hb_subset_context_t *c) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);
    auto *out = c->serializer->start_embed (*ഇത്);
    എങ്കിൽ (unlikely (!out || !c->serializer->extend_min (out))) return_trace (അസത്യം);

    തർക്കം ret = out->minCoord.serialize_subset (c, minCoord, ഇത്);
    ret |= out->maxCoord.serialize_subset (c, maxCoord, ഇത്);

    unsigned len = 0;
    വേണ്ടി (സ്ഥിരം FeatMinMaxRecord& _ : featMinMaxRecords)
    {
      hb_tag_t feature_tag = _.get_feature_tag ();
      എങ്കിൽ (!c->plan->layout_features.has (feature_tag))
        തുടരുക;

      auto snap = c->serializer->snapshot ();
      എങ്കിൽ (!_.subset (c, ഇത്)) c->serializer->revert (snap);
      അല്ലെങ്കിൽ len++;
    }

    എങ്കിൽ (len > 0) ret = സത്യം;
    എങ്കിൽ (!ret) return_trace (അസത്യം);
    return_trace (c->serializer->check_assign (out->featMinMaxRecords.len, len,
                                               HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (likely (c->check_struct (ഇത്) &&
			  minCoord.sanitize (c, ഇത്) &&
			  maxCoord.sanitize (c, ഇത്) &&
			  featMinMaxRecords.sanitize (c, ഇത്)));
  }

  സംരക്ഷിത:
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
  പൊതു:
  DEFINE_SIZE_ARRAY (6, featMinMaxRecords);
};

ഘടന Base_മൂല്യങ്ങൾ
{
  സ്ഥിരം BaseCoord &get_base_coord (പൂർണ്ണസംഖ്യ baseline_tag_index) സ്ഥിരം
  {
    എങ്കിൽ (baseline_tag_index == -1) baseline_tag_index = defaultIndex;
    തിരികെ ഇത്+baseCoords[baseline_tag_index];
  }

  ശൂന്യം collect_variation_indices (hb_set_t& varidx_set /* OUT */) സ്ഥിരം
  {
    വേണ്ടി (സ്ഥിരം auto& _ : baseCoords)
      (ഇത്+_).collect_variation_indices (varidx_set);
  }

  തർക്കം subset (hb_subset_context_t *c) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);
    auto *out = c->serializer->start_embed (*ഇത്);
    എങ്കിൽ (unlikely (!out || !c->serializer->extend_min (out))) return_trace (അസത്യം);
    out->defaultIndex = defaultIndex;

    തർക്കം ret = അസത്യം;
    വേണ്ടി (സ്ഥിരം auto& _ : baseCoords)
    {
      auto *o = out->baseCoords.serialize_append (c->serializer);
      എങ്കിൽ (unlikely (!o)) return_trace (അസത്യം);
       ret |= o->serialize_subset (c, _, ഇത്);
    }

    return_trace (ret);
  }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (likely (c->check_struct (ഇത്) &&
			  baseCoords.sanitize (c, ഇത്)));
  }

  സംരക്ഷിത:
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
  പൊതു:
  DEFINE_SIZE_ARRAY (4, baseCoords);
};

ഘടന BaseLangSysRecord
{
  പൂർണ്ണസംഖ്യ cmp (hb_tag_t key) സ്ഥിരം { തിരികെ baseLangSysTag.cmp (key); }

  തർക്കം has_data () സ്ഥിരം { തിരികെ baseLangSysTag; }

  സ്ഥിരം MinMax &get_min_max (സ്ഥിരം ശൂന്യം* base) സ്ഥിരം { തിരികെ base+minMax; }

  ശൂന്യം collect_variation_indices (സ്ഥിരം ശൂന്യം* base,
                                  സ്ഥിരം hb_subset_plan_t* plan,
                                  hb_set_t& varidx_set /* OUT */) സ്ഥിരം
  { (base+minMax).collect_variation_indices (plan, varidx_set); }

  തർക്കം subset (hb_subset_context_t *c,
               സ്ഥിരം ശൂന്യം *base) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);
    auto *out = c->serializer->embed (*ഇത്);
    എങ്കിൽ (unlikely (!out)) return_trace (അസത്യം);

    return_trace (out->minMax.serialize_subset (c, minMax, base));
  }

  തർക്കം sanitize (hb_sanitize_context_t *c, സ്ഥിരം ശൂന്യം *base) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (likely (c->check_struct (ഇത്) &&
			  minMax.sanitize (c, base)));
  }

  സംരക്ഷിത:
  Tag		baseLangSysTag;	/* 4-byte language system identification tag */
  Offset16To<MinMax>
		minMax;		/* Offset to MinMax table, from beginning
				 * of BaseScript table */
  പൊതു:
  DEFINE_SIZE_STATIC (6);
};

ഘടന BaseScript
{
  സ്ഥിരം MinMax &get_min_max (hb_tag_t language_tag) സ്ഥിരം
  {
    സ്ഥിരം BaseLangSysRecord& record = baseLangSysRecords.bsearch (language_tag);
    തിരികെ record.has_data () ? record.get_min_max (ഇത്) : ഇത്+defaultMinMax;
  }

  സ്ഥിരം BaseCoord &get_base_coord (പൂർണ്ണസംഖ്യ baseline_tag_index) സ്ഥിരം
  { തിരികെ (ഇത്+base_മൂല്യങ്ങൾ).get_base_coord (baseline_tag_index); }

  തർക്കം has_മൂല്യങ്ങൾ () സ്ഥിരം { തിരികെ base_മൂല്യങ്ങൾ; }
  തർക്കം has_min_max () സ്ഥിരം { തിരികെ defaultMinMax || baseLangSysRecords; }

  ശൂന്യം collect_variation_indices (സ്ഥിരം hb_subset_plan_t* plan,
                                  hb_set_t& varidx_set /* OUT */) സ്ഥിരം
  {
    (ഇത്+base_മൂല്യങ്ങൾ).collect_variation_indices (varidx_set);
    (ഇത്+defaultMinMax).collect_variation_indices (plan, varidx_set);
    
    വേണ്ടി (സ്ഥിരം BaseLangSysRecord& _ : baseLangSysRecords)
      _.collect_variation_indices (ഇത്, plan, varidx_set);
  }

  തർക്കം subset (hb_subset_context_t *c) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);
    auto *out = c->serializer->start_embed (*ഇത്);
    എങ്കിൽ (unlikely (!out || !c->serializer->extend_min (out))) return_trace (അസത്യം);

    തർക്കം ret = out->base_മൂല്യങ്ങൾ.serialize_subset (c, base_മൂല്യങ്ങൾ, ഇത്);
    ret |= out->defaultMinMax.serialize_subset (c, defaultMinMax, ഇത്);

    unsigned len = 0;
    വേണ്ടി (സ്ഥിരം auto& _ : baseLangSysRecords)
    {
      auto snap = c->serializer->snapshot ();
      എങ്കിൽ (_.subset (c, ഇത്)) {
        ret = സത്യം;
        len++;
      }
      അല്ലെങ്കിൽ c->serializer->revert (snap);
    }

    എങ്കിൽ (!ret) return_trace (അസത്യം);
    return_trace (c->serializer->check_assign (out->baseLangSysRecords.len, len,
                                               HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (likely (c->check_struct (ഇത്) &&
			  base_മൂല്യങ്ങൾ.sanitize (c, ഇത്) &&
			  defaultMinMax.sanitize (c, ഇത്) &&
			  baseLangSysRecords.sanitize (c, ഇത്)));
  }

  സംരക്ഷിത:
  Offset16To<Base_മൂല്യങ്ങൾ>
		base_മൂല്യങ്ങൾ;	/* Offset to BaseValues table, from beginning
				 * of BaseScript table (may be NULL) */
  Offset16To<MinMax>
		defaultMinMax;	/* Offset to MinMax table, from beginning of
				 * BaseScript table (may be NULL) */
  SortedArray16Of<BaseLangSysRecord>
		baseLangSysRecords;
				/* Number of BaseLangSysRecords
				 * defined — may be zero (0) */

  പൊതു:
  DEFINE_SIZE_ARRAY (6, baseLangSysRecords);
};

ഘടന BaseScriptList;
ഘടന BaseScriptRecord
{
  പൂർണ്ണസംഖ്യ cmp (hb_tag_t key) സ്ഥിരം { തിരികെ baseScriptTag.cmp (key); }

  തർക്കം has_data () സ്ഥിരം { തിരികെ baseScriptTag; }

  hb_tag_t get_script_tag () സ്ഥിരം { തിരികെ baseScriptTag; }

  സ്ഥിരം BaseScript &get_base_script (സ്ഥിരം BaseScriptList *list) സ്ഥിരം
  { തിരികെ list+baseScript; }

  ശൂന്യം collect_variation_indices (സ്ഥിരം hb_subset_plan_t* plan,
                                  സ്ഥിരം ശൂന്യം* list,
                                  hb_set_t& varidx_set /* OUT */) സ്ഥിരം
  {
    എങ്കിൽ (!plan->layout_scripts.has (baseScriptTag))
      തിരികെ;

    (list+baseScript).collect_variation_indices (plan, varidx_set);
  }

  തർക്കം subset (hb_subset_context_t *c,
               സ്ഥിരം ശൂന്യം *base) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);
    auto *out = c->serializer->embed (*ഇത്);
    എങ്കിൽ (unlikely (!out)) return_trace (അസത്യം);

    return_trace (out->baseScript.serialize_subset (c, baseScript, base));
  }

  തർക്കം sanitize (hb_sanitize_context_t *c, സ്ഥിരം ശൂന്യം *base) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (likely (c->check_struct (ഇത്) &&
			  baseScript.sanitize (c, base)));
  }

  സംരക്ഷിത:
  Tag		baseScriptTag;	/* 4-byte script identification tag */
  Offset16To<BaseScript>
		baseScript;	/* Offset to BaseScript table, from beginning
				 * of BaseScriptList */

  പൊതു:
  DEFINE_SIZE_STATIC (6);
};

ഘടന BaseScriptList
{
  സ്ഥിരം BaseScript &get_base_script (hb_tag_t script) സ്ഥിരം
  {
    സ്ഥിരം BaseScriptRecord *record = &baseScriptRecords.bsearch (script);
    എങ്കിൽ (!record->has_data ()) record = &baseScriptRecords.bsearch (HB_TAG ('D','F','L','T'));
    തിരികെ record->has_data () ? record->get_base_script (ഇത്) : Null (BaseScript);
  }

  ശൂന്യം collect_variation_indices (സ്ഥിരം hb_subset_plan_t* plan,
                                  hb_set_t& varidx_set /* OUT */) സ്ഥിരം
  {
    വേണ്ടി (സ്ഥിരം BaseScriptRecord& _ : baseScriptRecords)
      _.collect_variation_indices (plan, ഇത്, varidx_set);
  }

  തർക്കം subset (hb_subset_context_t *c) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);
    auto *out = c->serializer->start_embed (*ഇത്);
    എങ്കിൽ (unlikely (!out || !c->serializer->extend_min (out))) return_trace (അസത്യം);

    unsigned len = 0;
    വേണ്ടി (സ്ഥിരം BaseScriptRecord& _ : baseScriptRecords)
    {
      hb_tag_t script_tag = _.get_script_tag ();
      എങ്കിൽ (!c->plan->layout_scripts.has (script_tag))
        തുടരുക;

      auto snap = c->serializer->snapshot ();
      എങ്കിൽ (!_.subset (c, ഇത്)) c->serializer->revert (snap);
      അല്ലെങ്കിൽ len++;
    }

    എങ്കിൽ (!len) return_trace (അസത്യം);
    return_trace (c->serializer->check_assign (out->baseScriptRecords.len, len,
                                               HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_struct (ഇത്) &&
		  baseScriptRecords.sanitize (c, ഇത്));
  }

  സംരക്ഷിത:
  SortedArray16Of<BaseScriptRecord>
			baseScriptRecords;

  പൊതു:
  DEFINE_SIZE_ARRAY (2, baseScriptRecords);
};

ഘടന Axis
{
  തർക്കം get_baseline (hb_tag_t          baseline_tag,
		     hb_tag_t          script_tag,
		     hb_tag_t          language_tag,
		     സ്ഥിരം BaseCoord **coord) സ്ഥിരം
  {
    സ്ഥിരം BaseScript &base_script = (ഇത്+baseScriptList).get_base_script (script_tag);
    എങ്കിൽ (!base_script.has_മൂല്യങ്ങൾ ())
    {
      *coord = nullptr;
      തിരികെ അസത്യം;
    }

    എങ്കിൽ (likely (coord))
    {
      unsigned പൂർണ്ണസംഖ്യ tag_index = 0;
      എങ്കിൽ (!(ഇത്+baseTagList).bfind (baseline_tag, &tag_index))
      {
        *coord = nullptr;
        തിരികെ അസത്യം;
      }
      *coord = &base_script.get_base_coord (tag_index);
    }

    തിരികെ സത്യം;
  }

  തർക്കം get_min_max (hb_tag_t          script_tag,
		    hb_tag_t          language_tag,
		    hb_tag_t          feature_tag,
		    സ്ഥിരം BaseCoord **min_coord,
		    സ്ഥിരം BaseCoord **max_coord) സ്ഥിരം
  {
    സ്ഥിരം BaseScript &base_script = (ഇത്+baseScriptList).get_base_script (script_tag);
    എങ്കിൽ (!base_script.has_min_max ())
    {
      *min_coord = *max_coord = nullptr;
      തിരികെ അസത്യം;
    }

    base_script.get_min_max (language_tag).get_min_max (feature_tag, min_coord, max_coord);

    തിരികെ സത്യം;
  }

  ശൂന്യം collect_variation_indices (സ്ഥിരം hb_subset_plan_t* plan,
                                  hb_set_t& varidx_set /* OUT */) സ്ഥിരം
  { (ഇത്+baseScriptList).collect_variation_indices (plan, varidx_set); }

  തർക്കം subset (hb_subset_context_t *c) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);
    auto *out = c->serializer->embed (*ഇത്);
    എങ്കിൽ (unlikely (!out)) return_trace (അസത്യം);

    തർക്കം ret = out->baseTagList.serialize_copy (c->serializer, baseTagList, ഇത്);
    ret |= out->baseScriptList.serialize_subset (c, baseScriptList, ഇത്);
    return_trace (ret);
  }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (likely (c->check_struct (ഇത്) &&
			  baseTagList.sanitize (c, ഇത്) &&
			  baseScriptList.sanitize (c, ഇത്)));
  }

  സംരക്ഷിത:
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

  പൊതു:
  DEFINE_SIZE_STATIC (4);
};

ഘടന BASE
{
  സ്ഥിതം constexpr hb_tag_t tableTag = HB_OT_TAG_BASE;

  സ്ഥിരം Axis &get_axis (hb_direction_t direction) സ്ഥിരം
  { തിരികെ HB_DIRECTION_IS_VERTICAL (direction) ? ഇത്+vAxis : ഇത്+hAxis; }

  തർക്കം has_var_store () സ്ഥിരം
  { തിരികെ version.to_int () >= 0x00010001u && varStore != 0; }

  സ്ഥിരം ItemVariationStore &get_var_store () സ്ഥിരം
  { തിരികെ version.to_int () < 0x00010001u ? Null (ItemVariationStore) : ഇത്+varStore; }

  ശൂന്യം collect_variation_indices (സ്ഥിരം hb_subset_plan_t* plan,
                                  hb_set_t& varidx_set /* OUT */) സ്ഥിരം
  {
    (ഇത്+hAxis).collect_variation_indices (plan, varidx_set);
    (ഇത്+vAxis).collect_variation_indices (plan, varidx_set);
  }

  തർക്കം subset_varstore (hb_subset_context_t *c,
                        BASE *out /* OUT */) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);
    എങ്കിൽ (!c->serializer->allocate_size<Offset32To<ItemVariationStore>> (Offset32To<ItemVariationStore>::static_size))
        return_trace (അസത്യം);
    എങ്കിൽ (!c->plan->normalized_coords)
      return_trace (out->varStore.serialize_subset (c, varStore, ഇത്, c->plan->base_varstore_inner_maps.as_array ()));

    എങ്കിൽ (c->plan->all_axes_pinned)
      return_trace (സത്യം);

    item_variations_t item_vars;
    എങ്കിൽ (!item_vars.instantiate (ഇത്+varStore, c->plan, സത്യം, സത്യം,
                                c->plan->base_varstore_inner_maps.as_array ()))
      return_trace (അസത്യം);

    എങ്കിൽ (!out->varStore.serialize_serialize (c->serializer,
                                            item_vars.has_long_word (),
                                            c->plan->axis_tags,
                                            item_vars.get_region_list (),
                                            item_vars.get_vardata_encodings ()))
      return_trace (അസത്യം);

    സ്ഥിരം hb_map_t &varidx_map = item_vars.get_varidx_map ();
    /* base_variation_idx_map in the plan is old_varidx->(varidx, delta)
     * mapping, new varidx is generated for subsetting, we need to remap this
     * after instancing */
    വേണ്ടി (auto _ : c->plan->base_variation_idx_map.iter_ref ())
    {
      uint32_t varidx = _.second.first;
      uint32_t *new_varidx;
      എങ്കിൽ (varidx_map.has (varidx, &new_varidx))
        _.second.first = *new_varidx;
      അല്ലെങ്കിൽ
        _.second.first = HB_OT_LAYOUT_NO_VARIATIONS_INDEX;
    }
    return_trace (സത്യം);
  }

  തർക്കം subset (hb_subset_context_t *c) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);
    auto *out = c->serializer->start_embed (*ഇത്);
    എങ്കിൽ (unlikely (!out || !c->serializer->extend_min (out))) return_trace (അസത്യം);

    out->version = version;
    തർക്കം ret = അസത്യം;
    എങ്കിൽ (version.to_int () >= 0x00010001u) ret |= subset_varstore (c, out);

    ret |= out->hAxis.serialize_subset (c, hAxis, ഇത്);
    ret |= out->vAxis.serialize_subset (c, vAxis, ഇത്);

    return_trace (ret);
  }

  തർക്കം get_baseline (hb_font_t      *font,
		     hb_tag_t        baseline_tag,
		     hb_direction_t  direction,
		     hb_tag_t        script_tag,
		     hb_tag_t        language_tag,
		     hb_position_t  *base) സ്ഥിരം
  {
    സ്ഥിരം BaseCoord *base_coord = nullptr;
    എങ്കിൽ (unlikely (!get_axis (direction).get_baseline (baseline_tag, script_tag, language_tag, &base_coord) ||
		  !base_coord || !base_coord->has_data ()))
      തിരികെ അസത്യം;

    എങ്കിൽ (likely (base))
      *base = base_coord->get_coord (font, get_var_store (), direction);

    തിരികെ സത്യം;
  }

  തർക്കം get_min_max (hb_font_t      *font,
		    hb_direction_t  direction,
		    hb_tag_t        script_tag,
		    hb_tag_t        language_tag,
		    hb_tag_t        feature_tag,
		    hb_position_t  *min,
		    hb_position_t  *max) സ്ഥിരം
  {
    സ്ഥിരം BaseCoord *min_coord, *max_coord;
    എങ്കിൽ (!get_axis (direction).get_min_max (script_tag, language_tag, feature_tag,
					   &min_coord, &max_coord))
      തിരികെ അസത്യം;

    സ്ഥിരം ItemVariationStore &var_store = get_var_store ();
    എങ്കിൽ (likely (min && min_coord)) *min = min_coord->get_coord (font, var_store, direction);
    എങ്കിൽ (likely (max && max_coord)) *max = max_coord->get_coord (font, var_store, direction);
    തിരികെ സത്യം;
  }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (likely (c->check_struct (ഇത്) &&
			  hb_barrier () &&
			  likely (version.major == 1) &&
			  hAxis.sanitize (c, ഇത്) &&
			  vAxis.sanitize (c, ഇത്) &&
			  (version.to_int () < 0x00010001u || varStore.sanitize (c, ഇത്))));
  }

  സംരക്ഷിത:
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
  പൊതു:
  DEFINE_SIZE_MIN (8);
};


} /* namespace OT */


#endif /* HB_OT_LAYOUT_BASE_TABLE_HH */
