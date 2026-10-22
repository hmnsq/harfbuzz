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

#ଅନ୍ତର୍ଭାଵ "hb-open-type.hh"
#ଅନ୍ତର୍ଭାଵ "hb-ot-layout-common.hh"

ନାମାକାଶ OT {

/*
 * BASE -- Baseline
 * https://docs.microsoft.com/en-us/typography/opentype/spec/base
 */

ସଂରଚନା BaseCoordFormat1
{
  hb_position_t get_coord (hb_font_t *font, hb_direction_t direction) ଅଚଳ
  {
    ଫେରନ୍ତୁ HB_DIRECTION_IS_HORIZONTAL (direction) ? font->em_scale_y (coordinate) : font->em_scale_x (coordinate);
  }

  ତାର୍କିକ subset (hb_subset_context_t *c) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    return_trace ((ତାର୍କିକ) c->serializer->embed (*ଏହି));
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି));
  }

  ସୁରକ୍ଷିତ:
  HBUINT16	format;		/* Format identifier--format = 1 */
  FWORD		coordinate;	/* X or Y value, in design units */
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (4);
};

template <typename Types>
ସଂରଚନା BaseCoordFormat2_4
{
  hb_position_t get_coord (hb_font_t *font, hb_direction_t direction) ଅଚଳ
  {
    /* TODO */
    ଫେରନ୍ତୁ HB_DIRECTION_IS_HORIZONTAL (direction) ? font->em_scale_y (coordinate) : font->em_scale_x (coordinate);
  }

  ତାର୍କିକ subset (hb_subset_context_t *c) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    auto *out = c->serializer->embed (*ଏହି);
    ଯଦି (unlikely (!out)) return_trace (ମିଥ୍ୟା);

    hb_codepoint_t *new_gid;
    ଯଦି (!c->plan->glyph_map->has (referenceGlyph, &new_gid))
      return_trace (ମିଥ୍ୟା);

    return_trace (c->serializer->check_assign (out->referenceGlyph,
                                               *new_gid,
                                               HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି));
  }

  ସୁରକ୍ଷିତ:
  HBUINT16	format;		/* Format identifier--format = 2 or 4 */
  FWORD		coordinate;	/* X or Y value, in design units */
  typename Types::HBGlyphID
		referenceGlyph;	/* Glyph ID of control glyph */
  HBUINT16	coordPoint;	/* Index of contour point on the
				 * reference glyph */
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (6 + Types::HBGlyphID::static_size);
};

ସଂରଚନା BaseCoordFormat3
{
  hb_position_t get_coord (hb_font_t *font,
			   ଅଚଳ ItemVariationStore &var_store,
			   hb_direction_t direction) ଅଚଳ
  {
    ଅଚଳ Device &device = ଏହି+deviceTable;

    ଫେରନ୍ତୁ HB_DIRECTION_IS_HORIZONTAL (direction)
	 ? hb_saturate_add (font->em_scale_y (coordinate), device.get_y_delta (font, var_store))
	 : hb_saturate_add (font->em_scale_x (coordinate), device.get_x_delta (font, var_store));
  }

  ଶୂନ୍ୟ collect_variation_indices (hb_set_t& varidx_set /* OUT */) ଅଚଳ
  {
    unsigned varidx = (ଏହି+deviceTable).get_variation_index ();
    varidx_set.add (varidx);
  }

  ତାର୍କିକ subset (hb_subset_context_t *c) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    auto *out = c->serializer->embed (*ଏହି);
    ଯଦି (unlikely (!out)) return_trace (ମିଥ୍ୟା);

    ଯଦି (!c->plan->pinned_at_default)
    {
      unsigned var_idx = (ଏହି+deviceTable).get_variation_index ();
      ଯଦି (var_idx != VarIdx::NO_VARIATION)
      {
        hb_pair_t<unsigned, ପୂର୍ଣ୍ଣସଂଖ୍ୟା> *v;
        ଯଦି (!c->plan->base_variation_idx_map.has (var_idx, &v))
          return_trace (ମିଥ୍ୟା);
        
        ଯଦି (unlikely (!c->serializer->check_assign (out->coordinate, coordinate + hb_second (*v),
                                                    HB_SERIALIZE_ERROR_INT_OVERFLOW)))
          return_trace (ମିଥ୍ୟା);
      }
    }
    
    out->deviceTable.serialize_copy (c->serializer, deviceTable,
                                     ଏହି, 0,
                                     hb_serialize_context_t::Head,
                                     &c->plan->base_variation_idx_map);
    return_trace (ସତ୍ୟ);
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (likely (c->check_struct (ଏହି) &&
			  deviceTable.sanitize (c, ଏହି)));
  }

  ସୁରକ୍ଷିତ:
  HBUINT16	format;		/* Format identifier--format = 3 */
  FWORD		coordinate;	/* X or Y value, in design units */
  Offset16To<Device>
		deviceTable;	/* Offset to Device table for X or
				 * Y value, from beginning of
				 * BaseCoord table (may be NULL). */
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (6);
};

ସଂରଚନା BaseCoord
{
  ତାର୍କିକ has_data () ଅଚଳ { ଫେରନ୍ତୁ u.format.v; }

  hb_position_t get_coord (hb_font_t            *font,
			   ଅଚଳ ItemVariationStore &var_store,
			   hb_direction_t        direction) ଅଚଳ
  {
    ବାଛ (u.format.v) {
    ସ୍ଥିତି 1: hb_barrier (); ଫେରନ୍ତୁ u.format1.get_coord (font, direction);
    ସ୍ଥିତି 2: hb_barrier (); ଫେରନ୍ତୁ u.format2.get_coord (font, direction);
    ସ୍ଥିତି 3: hb_barrier (); ଫେରନ୍ତୁ u.format3.get_coord (font, var_store, direction);
#ifndef HB_NO_BEYOND_64K
    ସ୍ଥିତି 4: hb_barrier (); ଫେରନ୍ତୁ u.format4.get_coord (font, direction);
#endif
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:ଫେରନ୍ତୁ 0;
    }
  }

  ଶୂନ୍ୟ collect_variation_indices (hb_set_t& varidx_set /* OUT */) ଅଚଳ
  {
    ବାଛ (u.format.v) {
    ସ୍ଥିତି 3: hb_barrier (); u.format3.collect_variation_indices (varidx_set); ଫେରନ୍ତୁ;
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:ଫେରନ୍ତୁ;
    }
  }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ଅଚଳ
  {
    ଯଦି (unlikely (!c->may_dispatch (ଏହି, &u.format.v))) ଫେରନ୍ତୁ c->no_dispatch_return_ମୂଲ୍ୟ ();
    TRACE_DISPATCH (ଏହି, u.format.v);
    ବାଛ (u.format.v) {
    ସ୍ଥିତି 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    ସ୍ଥିତି 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    ସ୍ଥିତି 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
#ifndef HB_NO_BEYOND_64K
    ସ୍ଥିତି 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
#endif
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:return_trace (c->default_return_ମୂଲ୍ୟ ());
    }
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    ଯଦି (unlikely (!u.format.v.sanitize (c))) return_trace (ମିଥ୍ୟା);
    hb_barrier ();
    ବାଛ (u.format.v) {
    ସ୍ଥିତି 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    ସ୍ଥିତି 2: hb_barrier (); return_trace (u.format2.sanitize (c));
    ସ୍ଥିତି 3: hb_barrier (); return_trace (u.format3.sanitize (c));
#ifndef HB_NO_BEYOND_64K
    ସ୍ଥିତି 4: hb_barrier (); return_trace (u.format4.sanitize (c));
#endif
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:return_trace (ମିଥ୍ୟା);
    }
  }

  ସୁରକ୍ଷିତ:
  ସଂଘ {
  ସଂରଚନା { HBUINT16 v; }	format;
  BaseCoordFormat1	format1;
  BaseCoordFormat2_4<Layout::SmallTypes>
			format2;
  BaseCoordFormat3	format3;
#ifndef HB_NO_BEYOND_64K
  BaseCoordFormat2_4<Layout::MediumTypes>
			format4;
#endif
  } u;
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_UNION (2, format.v);
};

ସଂରଚନା FeatMinMaxRecord
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmp (hb_tag_t key) ଅଚଳ { ଫେରନ୍ତୁ tag.cmp (key); }

  ତାର୍କିକ has_data () ଅଚଳ { ଫେରନ୍ତୁ tag; }

  hb_tag_t get_feature_tag () ଅଚଳ { ଫେରନ୍ତୁ tag; }

  ଶୂନ୍ୟ get_min_max (ଅଚଳ BaseCoord **min, ଅଚଳ BaseCoord **max) ଅଚଳ
  {
    ଯଦି (likely (min)) *min = &(ଏହି+minCoord);
    ଯଦି (likely (max)) *max = &(ଏହି+maxCoord);
  }

  ଶୂନ୍ୟ collect_variation_indices (ଅଚଳ hb_subset_plan_t* plan,
                                  ଅଚଳ ଶୂନ୍ୟ *base,
                                  hb_set_t& varidx_set /* OUT */) ଅଚଳ
  {
    ଯଦି (!plan->layout_features.has (tag))
      ଫେରନ୍ତୁ;

    (base+minCoord).collect_variation_indices (varidx_set);
    (base+maxCoord).collect_variation_indices (varidx_set);
  }

  ତାର୍କିକ subset (hb_subset_context_t *c,
               ଅଚଳ ଶୂନ୍ୟ *base) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    auto *out = c->serializer->embed (*ଏହି);
    ଯଦି (unlikely (!out)) return_trace (ମିଥ୍ୟା);

    ତାର୍କିକ ret = out->minCoord.serialize_subset (c, minCoord, base);
    ret |= out->maxCoord.serialize_subset (c, maxCoord, base);
    return_trace (ret);
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c, ଅଚଳ ଶୂନ୍ୟ *base) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (likely (c->check_struct (ଏହି) &&
			  minCoord.sanitize (c, base) &&
			  maxCoord.sanitize (c, base)));
  }

  ସୁରକ୍ଷିତ:
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
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (8);
};

ସଂରଚନା MinMax
{
  ଶୂନ୍ୟ get_min_max (hb_tag_t          feature_tag,
		    ଅଚଳ BaseCoord **min,
		    ଅଚଳ BaseCoord **max) ଅଚଳ
  {
    ଅଚଳ FeatMinMaxRecord &minMaxCoord = featMinMaxRecords.bsearch (feature_tag);
    ଯଦି (minMaxCoord.has_data ())
      minMaxCoord.get_min_max (min, max);
    ଅନ୍ୟଥା
    {
      ଯଦି (likely (min)) *min = &(ଏହି+minCoord);
      ଯଦି (likely (max)) *max = &(ଏହି+maxCoord);
    }
  }

  ଶୂନ୍ୟ collect_variation_indices (ଅଚଳ hb_subset_plan_t* plan,
                                  hb_set_t& varidx_set /* OUT */) ଅଚଳ
  {
    (ଏହି+minCoord).collect_variation_indices (varidx_set);
    (ଏହି+maxCoord).collect_variation_indices (varidx_set);
    ପାଇଁ (ଅଚଳ FeatMinMaxRecord& record : featMinMaxRecords)
      record.collect_variation_indices (plan, ଏହି, varidx_set);
  }

  ତାର୍କିକ subset (hb_subset_context_t *c) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    auto *out = c->serializer->start_embed (*ଏହି);
    ଯଦି (unlikely (!out || !c->serializer->extend_min (out))) return_trace (ମିଥ୍ୟା);

    ତାର୍କିକ ret = out->minCoord.serialize_subset (c, minCoord, ଏହି);
    ret |= out->maxCoord.serialize_subset (c, maxCoord, ଏହି);

    unsigned len = 0;
    ପାଇଁ (ଅଚଳ FeatMinMaxRecord& _ : featMinMaxRecords)
    {
      hb_tag_t feature_tag = _.get_feature_tag ();
      ଯଦି (!c->plan->layout_features.has (feature_tag))
        ଜାରି;

      auto snap = c->serializer->snapshot ();
      ଯଦି (!_.subset (c, ଏହି)) c->serializer->revert (snap);
      ଅନ୍ୟଥା len++;
    }

    ଯଦି (len > 0) ret = ସତ୍ୟ;
    ଯଦି (!ret) return_trace (ମିଥ୍ୟା);
    return_trace (c->serializer->check_assign (out->featMinMaxRecords.len, len,
                                               HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (likely (c->check_struct (ଏହି) &&
			  minCoord.sanitize (c, ଏହି) &&
			  maxCoord.sanitize (c, ଏହି) &&
			  featMinMaxRecords.sanitize (c, ଏହି)));
  }

  ସୁରକ୍ଷିତ:
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
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_ARRAY (6, featMinMaxRecords);
};

ସଂରଚନା Base_ମୂଲ୍ୟଗୁଡ଼ିକ
{
  ଅଚଳ BaseCoord &get_base_coord (ପୂର୍ଣ୍ଣସଂଖ୍ୟା baseline_tag_index) ଅଚଳ
  {
    ଯଦି (baseline_tag_index == -1) baseline_tag_index = defaultIndex;
    ଫେରନ୍ତୁ ଏହି+baseCoords[baseline_tag_index];
  }

  ଶୂନ୍ୟ collect_variation_indices (hb_set_t& varidx_set /* OUT */) ଅଚଳ
  {
    ପାଇଁ (ଅଚଳ auto& _ : baseCoords)
      (ଏହି+_).collect_variation_indices (varidx_set);
  }

  ତାର୍କିକ subset (hb_subset_context_t *c) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    auto *out = c->serializer->start_embed (*ଏହି);
    ଯଦି (unlikely (!out || !c->serializer->extend_min (out))) return_trace (ମିଥ୍ୟା);
    out->defaultIndex = defaultIndex;

    ତାର୍କିକ ret = ମିଥ୍ୟା;
    ପାଇଁ (ଅଚଳ auto& _ : baseCoords)
    {
      auto *o = out->baseCoords.serialize_append (c->serializer);
      ଯଦି (unlikely (!o)) return_trace (ମିଥ୍ୟା);
       ret |= o->serialize_subset (c, _, ଏହି);
    }

    return_trace (ret);
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (likely (c->check_struct (ଏହି) &&
			  baseCoords.sanitize (c, ଏହି)));
  }

  ସୁରକ୍ଷିତ:
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
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_ARRAY (4, baseCoords);
};

ସଂରଚନା BaseLangSysRecord
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmp (hb_tag_t key) ଅଚଳ { ଫେରନ୍ତୁ baseLangSysTag.cmp (key); }

  ତାର୍କିକ has_data () ଅଚଳ { ଫେରନ୍ତୁ baseLangSysTag; }

  ଅଚଳ MinMax &get_min_max (ଅଚଳ ଶୂନ୍ୟ* base) ଅଚଳ { ଫେରନ୍ତୁ base+minMax; }

  ଶୂନ୍ୟ collect_variation_indices (ଅଚଳ ଶୂନ୍ୟ* base,
                                  ଅଚଳ hb_subset_plan_t* plan,
                                  hb_set_t& varidx_set /* OUT */) ଅଚଳ
  { (base+minMax).collect_variation_indices (plan, varidx_set); }

  ତାର୍କିକ subset (hb_subset_context_t *c,
               ଅଚଳ ଶୂନ୍ୟ *base) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    auto *out = c->serializer->embed (*ଏହି);
    ଯଦି (unlikely (!out)) return_trace (ମିଥ୍ୟା);

    return_trace (out->minMax.serialize_subset (c, minMax, base));
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c, ଅଚଳ ଶୂନ୍ୟ *base) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (likely (c->check_struct (ଏହି) &&
			  minMax.sanitize (c, base)));
  }

  ସୁରକ୍ଷିତ:
  Tag		baseLangSysTag;	/* 4-byte language system identification tag */
  Offset16To<MinMax>
		minMax;		/* Offset to MinMax table, from beginning
				 * of BaseScript table */
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (6);
};

ସଂରଚନା BaseScript
{
  ଅଚଳ MinMax &get_min_max (hb_tag_t language_tag) ଅଚଳ
  {
    ଅଚଳ BaseLangSysRecord& record = baseLangSysRecords.bsearch (language_tag);
    ଫେରନ୍ତୁ record.has_data () ? record.get_min_max (ଏହି) : ଏହି+defaultMinMax;
  }

  ଅଚଳ BaseCoord &get_base_coord (ପୂର୍ଣ୍ଣସଂଖ୍ୟା baseline_tag_index) ଅଚଳ
  { ଫେରନ୍ତୁ (ଏହି+base_ମୂଲ୍ୟଗୁଡ଼ିକ).get_base_coord (baseline_tag_index); }

  ତାର୍କିକ has_ମୂଲ୍ୟଗୁଡ଼ିକ () ଅଚଳ { ଫେରନ୍ତୁ base_ମୂଲ୍ୟଗୁଡ଼ିକ; }
  ତାର୍କିକ has_min_max () ଅଚଳ { ଫେରନ୍ତୁ defaultMinMax || baseLangSysRecords; }

  ଶୂନ୍ୟ collect_variation_indices (ଅଚଳ hb_subset_plan_t* plan,
                                  hb_set_t& varidx_set /* OUT */) ଅଚଳ
  {
    (ଏହି+base_ମୂଲ୍ୟଗୁଡ଼ିକ).collect_variation_indices (varidx_set);
    (ଏହି+defaultMinMax).collect_variation_indices (plan, varidx_set);
    
    ପାଇଁ (ଅଚଳ BaseLangSysRecord& _ : baseLangSysRecords)
      _.collect_variation_indices (ଏହି, plan, varidx_set);
  }

  ତାର୍କିକ subset (hb_subset_context_t *c) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    auto *out = c->serializer->start_embed (*ଏହି);
    ଯଦି (unlikely (!out || !c->serializer->extend_min (out))) return_trace (ମିଥ୍ୟା);

    ତାର୍କିକ ret = out->base_ମୂଲ୍ୟଗୁଡ଼ିକ.serialize_subset (c, base_ମୂଲ୍ୟଗୁଡ଼ିକ, ଏହି);
    ret |= out->defaultMinMax.serialize_subset (c, defaultMinMax, ଏହି);

    unsigned len = 0;
    ପାଇଁ (ଅଚଳ auto& _ : baseLangSysRecords)
    {
      auto snap = c->serializer->snapshot ();
      ଯଦି (_.subset (c, ଏହି)) {
        ret = ସତ୍ୟ;
        len++;
      }
      ଅନ୍ୟଥା c->serializer->revert (snap);
    }

    ଯଦି (!ret) return_trace (ମିଥ୍ୟା);
    return_trace (c->serializer->check_assign (out->baseLangSysRecords.len, len,
                                               HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (likely (c->check_struct (ଏହି) &&
			  base_ମୂଲ୍ୟଗୁଡ଼ିକ.sanitize (c, ଏହି) &&
			  defaultMinMax.sanitize (c, ଏହି) &&
			  baseLangSysRecords.sanitize (c, ଏହି)));
  }

  ସୁରକ୍ଷିତ:
  Offset16To<Base_ମୂଲ୍ୟଗୁଡ଼ିକ>
		base_ମୂଲ୍ୟଗୁଡ଼ିକ;	/* Offset to BaseValues table, from beginning
				 * of BaseScript table (may be NULL) */
  Offset16To<MinMax>
		defaultMinMax;	/* Offset to MinMax table, from beginning of
				 * BaseScript table (may be NULL) */
  SortedArray16Of<BaseLangSysRecord>
		baseLangSysRecords;
				/* Number of BaseLangSysRecords
				 * defined — may be zero (0) */

  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_ARRAY (6, baseLangSysRecords);
};

ସଂରଚନା BaseScriptList;
ସଂରଚନା BaseScriptRecord
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmp (hb_tag_t key) ଅଚଳ { ଫେରନ୍ତୁ baseScriptTag.cmp (key); }

  ତାର୍କିକ has_data () ଅଚଳ { ଫେରନ୍ତୁ baseScriptTag; }

  hb_tag_t get_script_tag () ଅଚଳ { ଫେରନ୍ତୁ baseScriptTag; }

  ଅଚଳ BaseScript &get_base_script (ଅଚଳ BaseScriptList *list) ଅଚଳ
  { ଫେରନ୍ତୁ list+baseScript; }

  ଶୂନ୍ୟ collect_variation_indices (ଅଚଳ hb_subset_plan_t* plan,
                                  ଅଚଳ ଶୂନ୍ୟ* list,
                                  hb_set_t& varidx_set /* OUT */) ଅଚଳ
  {
    ଯଦି (!plan->layout_scripts.has (baseScriptTag))
      ଫେରନ୍ତୁ;

    (list+baseScript).collect_variation_indices (plan, varidx_set);
  }

  ତାର୍କିକ subset (hb_subset_context_t *c,
               ଅଚଳ ଶୂନ୍ୟ *base) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    auto *out = c->serializer->embed (*ଏହି);
    ଯଦି (unlikely (!out)) return_trace (ମିଥ୍ୟା);

    return_trace (out->baseScript.serialize_subset (c, baseScript, base));
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c, ଅଚଳ ଶୂନ୍ୟ *base) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (likely (c->check_struct (ଏହି) &&
			  baseScript.sanitize (c, base)));
  }

  ସୁରକ୍ଷିତ:
  Tag		baseScriptTag;	/* 4-byte script identification tag */
  Offset16To<BaseScript>
		baseScript;	/* Offset to BaseScript table, from beginning
				 * of BaseScriptList */

  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (6);
};

ସଂରଚନା BaseScriptList
{
  ଅଚଳ BaseScript &get_base_script (hb_tag_t script) ଅଚଳ
  {
    ଅଚଳ BaseScriptRecord *record = &baseScriptRecords.bsearch (script);
    ଯଦି (!record->has_data ()) record = &baseScriptRecords.bsearch (HB_TAG ('D','F','L','T'));
    ଫେରନ୍ତୁ record->has_data () ? record->get_base_script (ଏହି) : Null (BaseScript);
  }

  ଶୂନ୍ୟ collect_variation_indices (ଅଚଳ hb_subset_plan_t* plan,
                                  hb_set_t& varidx_set /* OUT */) ଅଚଳ
  {
    ପାଇଁ (ଅଚଳ BaseScriptRecord& _ : baseScriptRecords)
      _.collect_variation_indices (plan, ଏହି, varidx_set);
  }

  ତାର୍କିକ subset (hb_subset_context_t *c) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    auto *out = c->serializer->start_embed (*ଏହି);
    ଯଦି (unlikely (!out || !c->serializer->extend_min (out))) return_trace (ମିଥ୍ୟା);

    unsigned len = 0;
    ପାଇଁ (ଅଚଳ BaseScriptRecord& _ : baseScriptRecords)
    {
      hb_tag_t script_tag = _.get_script_tag ();
      ଯଦି (!c->plan->layout_scripts.has (script_tag))
        ଜାରି;

      auto snap = c->serializer->snapshot ();
      ଯଦି (!_.subset (c, ଏହି)) c->serializer->revert (snap);
      ଅନ୍ୟଥା len++;
    }

    ଯଦି (!len) return_trace (ମିଥ୍ୟା);
    return_trace (c->serializer->check_assign (out->baseScriptRecords.len, len,
                                               HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି) &&
		  baseScriptRecords.sanitize (c, ଏହି));
  }

  ସୁରକ୍ଷିତ:
  SortedArray16Of<BaseScriptRecord>
			baseScriptRecords;

  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_ARRAY (2, baseScriptRecords);
};

ସଂରଚନା Axis
{
  ତାର୍କିକ get_baseline (hb_tag_t          baseline_tag,
		     hb_tag_t          script_tag,
		     hb_tag_t          language_tag,
		     ଅଚଳ BaseCoord **coord) ଅଚଳ
  {
    ଅଚଳ BaseScript &base_script = (ଏହି+baseScriptList).get_base_script (script_tag);
    ଯଦି (!base_script.has_ମୂଲ୍ୟଗୁଡ଼ିକ ())
    {
      *coord = nullptr;
      ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }

    ଯଦି (likely (coord))
    {
      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା tag_index = 0;
      ଯଦି (!(ଏହି+baseTagList).bfind (baseline_tag, &tag_index))
      {
        *coord = nullptr;
        ଫେରନ୍ତୁ ମିଥ୍ୟା;
      }
      *coord = &base_script.get_base_coord (tag_index);
    }

    ଫେରନ୍ତୁ ସତ୍ୟ;
  }

  ତାର୍କିକ get_min_max (hb_tag_t          script_tag,
		    hb_tag_t          language_tag,
		    hb_tag_t          feature_tag,
		    ଅଚଳ BaseCoord **min_coord,
		    ଅଚଳ BaseCoord **max_coord) ଅଚଳ
  {
    ଅଚଳ BaseScript &base_script = (ଏହି+baseScriptList).get_base_script (script_tag);
    ଯଦି (!base_script.has_min_max ())
    {
      *min_coord = *max_coord = nullptr;
      ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }

    base_script.get_min_max (language_tag).get_min_max (feature_tag, min_coord, max_coord);

    ଫେରନ୍ତୁ ସତ୍ୟ;
  }

  ଶୂନ୍ୟ collect_variation_indices (ଅଚଳ hb_subset_plan_t* plan,
                                  hb_set_t& varidx_set /* OUT */) ଅଚଳ
  { (ଏହି+baseScriptList).collect_variation_indices (plan, varidx_set); }

  ତାର୍କିକ subset (hb_subset_context_t *c) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    auto *out = c->serializer->embed (*ଏହି);
    ଯଦି (unlikely (!out)) return_trace (ମିଥ୍ୟା);

    ତାର୍କିକ ret = out->baseTagList.serialize_copy (c->serializer, baseTagList, ଏହି);
    ret |= out->baseScriptList.serialize_subset (c, baseScriptList, ଏହି);
    return_trace (ret);
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (likely (c->check_struct (ଏହି) &&
			  baseTagList.sanitize (c, ଏହି) &&
			  baseScriptList.sanitize (c, ଏହି)));
  }

  ସୁରକ୍ଷିତ:
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

  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (4);
};

ସଂରଚନା BASE
{
  ସ୍ଥିର constexpr hb_tag_t tableTag = HB_OT_TAG_BASE;

  ଅଚଳ Axis &get_axis (hb_direction_t direction) ଅଚଳ
  { ଫେରନ୍ତୁ HB_DIRECTION_IS_VERTICAL (direction) ? ଏହି+vAxis : ଏହି+hAxis; }

  ତାର୍କିକ has_var_store () ଅଚଳ
  { ଫେରନ୍ତୁ version.to_int () >= 0x00010001u && varStore != 0; }

  ଅଚଳ ItemVariationStore &get_var_store () ଅଚଳ
  { ଫେରନ୍ତୁ version.to_int () < 0x00010001u ? Null (ItemVariationStore) : ଏହି+varStore; }

  ଶୂନ୍ୟ collect_variation_indices (ଅଚଳ hb_subset_plan_t* plan,
                                  hb_set_t& varidx_set /* OUT */) ଅଚଳ
  {
    (ଏହି+hAxis).collect_variation_indices (plan, varidx_set);
    (ଏହି+vAxis).collect_variation_indices (plan, varidx_set);
  }

  ତାର୍କିକ subset_varstore (hb_subset_context_t *c,
                        BASE *out /* OUT */) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    ଯଦି (!c->serializer->allocate_size<Offset32To<ItemVariationStore>> (Offset32To<ItemVariationStore>::static_size))
        return_trace (ମିଥ୍ୟା);
    ଯଦି (!c->plan->normalized_coords)
      return_trace (out->varStore.serialize_subset (c, varStore, ଏହି, c->plan->base_varstore_inner_maps.as_array ()));

    ଯଦି (c->plan->all_axes_pinned)
      return_trace (ସତ୍ୟ);

    item_variations_t item_vars;
    ଯଦି (!item_vars.instantiate (ଏହି+varStore, c->plan, ସତ୍ୟ, ସତ୍ୟ,
                                c->plan->base_varstore_inner_maps.as_array ()))
      return_trace (ମିଥ୍ୟା);

    ଯଦି (!out->varStore.serialize_serialize (c->serializer,
                                            item_vars.has_long_word (),
                                            c->plan->axis_tags,
                                            item_vars.get_region_list (),
                                            item_vars.get_vardata_encodings ()))
      return_trace (ମିଥ୍ୟା);

    ଅଚଳ hb_map_t &varidx_map = item_vars.get_varidx_map ();
    /* base_variation_idx_map in the plan is old_varidx->(varidx, delta)
     * mapping, new varidx is generated for subsetting, we need to remap this
     * after instancing */
    ପାଇଁ (auto _ : c->plan->base_variation_idx_map.iter_ref ())
    {
      uint32_t varidx = _.second.first;
      uint32_t *new_varidx;
      ଯଦି (varidx_map.has (varidx, &new_varidx))
        _.second.first = *new_varidx;
      ଅନ୍ୟଥା
        _.second.first = HB_OT_LAYOUT_NO_VARIATIONS_INDEX;
    }
    return_trace (ସତ୍ୟ);
  }

  ତାର୍କିକ subset (hb_subset_context_t *c) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    auto *out = c->serializer->start_embed (*ଏହି);
    ଯଦି (unlikely (!out || !c->serializer->extend_min (out))) return_trace (ମିଥ୍ୟା);

    out->version = version;
    ତାର୍କିକ ret = ମିଥ୍ୟା;
    ଯଦି (version.to_int () >= 0x00010001u) ret |= subset_varstore (c, out);

    ret |= out->hAxis.serialize_subset (c, hAxis, ଏହି);
    ret |= out->vAxis.serialize_subset (c, vAxis, ଏହି);

    return_trace (ret);
  }

  ତାର୍କିକ get_baseline (hb_font_t      *font,
		     hb_tag_t        baseline_tag,
		     hb_direction_t  direction,
		     hb_tag_t        script_tag,
		     hb_tag_t        language_tag,
		     hb_position_t  *base) ଅଚଳ
  {
    ଅଚଳ BaseCoord *base_coord = nullptr;
    ଯଦି (unlikely (!get_axis (direction).get_baseline (baseline_tag, script_tag, language_tag, &base_coord) ||
		  !base_coord || !base_coord->has_data ()))
      ଫେରନ୍ତୁ ମିଥ୍ୟା;

    ଯଦି (likely (base))
      *base = base_coord->get_coord (font, get_var_store (), direction);

    ଫେରନ୍ତୁ ସତ୍ୟ;
  }

  ତାର୍କିକ get_min_max (hb_font_t      *font,
		    hb_direction_t  direction,
		    hb_tag_t        script_tag,
		    hb_tag_t        language_tag,
		    hb_tag_t        feature_tag,
		    hb_position_t  *min,
		    hb_position_t  *max) ଅଚଳ
  {
    ଅଚଳ BaseCoord *min_coord, *max_coord;
    ଯଦି (!get_axis (direction).get_min_max (script_tag, language_tag, feature_tag,
					   &min_coord, &max_coord))
      ଫେରନ୍ତୁ ମିଥ୍ୟା;

    ଅଚଳ ItemVariationStore &var_store = get_var_store ();
    ଯଦି (likely (min && min_coord)) *min = min_coord->get_coord (font, var_store, direction);
    ଯଦି (likely (max && max_coord)) *max = max_coord->get_coord (font, var_store, direction);
    ଫେରନ୍ତୁ ସତ୍ୟ;
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (likely (c->check_struct (ଏହି) &&
			  hb_barrier () &&
			  likely (version.major == 1) &&
			  hAxis.sanitize (c, ଏହି) &&
			  vAxis.sanitize (c, ଏହି) &&
			  (version.to_int () < 0x00010001u || varStore.sanitize (c, ଏହି))));
  }

  ସୁରକ୍ଷିତ:
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
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_MIN (8);
};


} /* namespace OT */


#endif /* HB_OT_LAYOUT_BASE_TABLE_HH */
