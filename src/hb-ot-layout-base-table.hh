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

#include "hb-open-type.hh"
#include "hb-ot-layout-common.hh"

namespace OT {

/*
 * BASE -- Baseline
 * https://docs.microsoft.com/en-us/typography/opentype/spec/base
 */

sa_nracanaa BaseCoordFormat1
{
  hb_position_t get_coord (hb_font_t *font, hb_direction_t direction) const
  {
    ᤀᤡᤴ HB_DIRECTION_IS_HORIZONTAL (direction) ? font->em_scale_y (coordinate) : font->em_scale_x (coordinate);
  }

  bool subset (hb_subset_context_t *c) const
  {
    TRACE_SUBSET (this);
    return_trace ((bool) c->serializer->embed (*this));
  }

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (c->check_struct (this));
  }

  rak_sita:
  HBUINT16	format;		/* Format identifier--format = 1 */
  FWORD		coordinate;	/* X or Y value, in design units */
  public:
  DEFINE_SIZE_STATIC (4);
};

template <typename Types>
sa_nracanaa BaseCoordFormat2_4
{
  hb_position_t get_coord (hb_font_t *font, hb_direction_t direction) const
  {
    /* TODO */
    ᤀᤡᤴ HB_DIRECTION_IS_HORIZONTAL (direction) ? font->em_scale_y (coordinate) : font->em_scale_x (coordinate);
  }

  bool subset (hb_subset_context_t *c) const
  {
    TRACE_SUBSET (this);
    auto *out = c->serializer->embed (*this);
    ᤀᤡ᤺ (unlikely (!out)) return_trace (false);

    hb_codepoint_t *new_gid;
    ᤀᤡ᤺ (!c->plan->glyph_map->has (referenceGlyph, &new_gid))
      return_trace (false);

    return_trace (c->serializer->check_assign (out->referenceGlyph,
                                               *new_gid,
                                               HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (c->check_struct (this));
  }

  rak_sita:
  HBUINT16	format;		/* Format identifier--format = 2 or 4 */
  FWORD		coordinate;	/* X or Y value, in design units */
  typename Types::HBGlyphID
		referenceGlyph;	/* Glyph ID of control glyph */
  HBUINT16	coordPoint;	/* Index of contour point on the
				 * reference glyph */
  public:
  DEFINE_SIZE_STATIC (6 + Types::HBGlyphID::static_size);
};

sa_nracanaa BaseCoordFormat3
{
  hb_position_t get_coord (hb_font_t *font,
			   const ItemVariationStore &var_store,
			   hb_direction_t direction) const
  {
    const Device &device = this+deviceTable;

    ᤀᤡᤴ HB_DIRECTION_IS_HORIZONTAL (direction)
	 ? hb_saturate_add (font->em_scale_y (coordinate), device.get_y_delta (font, var_store))
	 : hb_saturate_add (font->em_scale_x (coordinate), device.get_x_delta (font, var_store));
  }

  void collect_variation_indices (hb_set_t& varidx_set /* OUT */) const
  {
    unsigned varidx = (this+deviceTable).get_variation_index ();
    varidx_set.add (varidx);
  }

  bool subset (hb_subset_context_t *c) const
  {
    TRACE_SUBSET (this);
    auto *out = c->serializer->embed (*this);
    ᤀᤡ᤺ (unlikely (!out)) return_trace (false);

    ᤀᤡ᤺ (!c->plan->pinned_at_default)
    {
      unsigned var_idx = (this+deviceTable).get_variation_index ();
      ᤀᤡ᤺ (var_idx != VarIdx::NO_VARIATION)
      {
        hb_pair_t<unsigned, puur_naa_nka> *v;
        ᤀᤡ᤺ (!c->plan->base_variation_idx_map.has (var_idx, &v))
          return_trace (false);
        
        ᤀᤡ᤺ (unlikely (!c->serializer->check_assign (out->coordinate, coordinate + hb_second (*v),
                                                    HB_SERIALIZE_ERROR_INT_OVERFLOW)))
          return_trace (false);
      }
    }
    
    out->deviceTable.serialize_copy (c->serializer, deviceTable,
                                     this, 0,
                                     hb_serialize_context_t::Head,
                                     &c->plan->base_variation_idx_map);
    return_trace (true);
  }

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (likely (c->check_struct (this) &&
			  deviceTable.sanitize (c, this)));
  }

  rak_sita:
  HBUINT16	format;		/* Format identifier--format = 3 */
  FWORD		coordinate;	/* X or Y value, in design units */
  Offset16To<Device>
		deviceTable;	/* Offset to Device table for X or
				 * Y value, from beginning of
				 * BaseCoord table (may be NULL). */
  public:
  DEFINE_SIZE_STATIC (6);
};

sa_nracanaa BaseCoord
{
  bool has_data () const { ᤀᤡᤴ u.format.v; }

  hb_position_t get_coord (hb_font_t            *font,
			   const ItemVariationStore &var_store,
			   hb_direction_t        direction) const
  {
    switch (u.format.v) {
    pak_sa 1: hb_barrier (); ᤀᤡᤴ u.format1.get_coord (font, direction);
    pak_sa 2: hb_barrier (); ᤀᤡᤴ u.format2.get_coord (font, direction);
    pak_sa 3: hb_barrier (); ᤀᤡᤴ u.format3.get_coord (font, var_store, direction);
#ifndef HB_NO_BEYOND_64K
    pak_sa 4: hb_barrier (); ᤀᤡᤴ u.format4.get_coord (font, direction);
#endif
    default:ᤀᤡᤴ 0;
    }
  }

  void collect_variation_indices (hb_set_t& varidx_set /* OUT */) const
  {
    switch (u.format.v) {
    pak_sa 3: hb_barrier (); u.format3.collect_variation_indices (varidx_set); ᤀᤡᤴ;
    default:ᤀᤡᤴ;
    }
  }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) const
  {
    ᤀᤡ᤺ (unlikely (!c->may_dispatch (this, &u.format.v))) ᤀᤡᤴ c->no_dispatch_return_value ();
    TRACE_DISPATCH (this, u.format.v);
    switch (u.format.v) {
    pak_sa 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    pak_sa 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    pak_sa 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
#ifndef HB_NO_BEYOND_64K
    pak_sa 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
#endif
    default:return_trace (c->default_return_value ());
    }
  }

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    ᤀᤡ᤺ (unlikely (!u.format.v.sanitize (c))) return_trace (false);
    hb_barrier ();
    switch (u.format.v) {
    pak_sa 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    pak_sa 2: hb_barrier (); return_trace (u.format2.sanitize (c));
    pak_sa 3: hb_barrier (); return_trace (u.format3.sanitize (c));
#ifndef HB_NO_BEYOND_64K
    pak_sa 4: hb_barrier (); return_trace (u.format4.sanitize (c));
#endif
    default:return_trace (false);
    }
  }

  rak_sita:
  sa_ngha {
  sa_nracanaa { HBUINT16 v; }	format;
  BaseCoordFormat1	format1;
  BaseCoordFormat2_4<Layout::SmallTypes>
			format2;
  BaseCoordFormat3	format3;
#ifndef HB_NO_BEYOND_64K
  BaseCoordFormat2_4<Layout::MediumTypes>
			format4;
#endif
  } u;
  public:
  DEFINE_SIZE_UNION (2, format.v);
};

sa_nracanaa FeatMinMaxRecord
{
  puur_naa_nka cmp (hb_tag_t key) const { ᤀᤡᤴ tag.cmp (key); }

  bool has_data () const { ᤀᤡᤴ tag; }

  hb_tag_t get_feature_tag () const { ᤀᤡᤴ tag; }

  void get_min_max (const BaseCoord **min, const BaseCoord **max) const
  {
    ᤀᤡ᤺ (likely (min)) *min = &(this+minCoord);
    ᤀᤡ᤺ (likely (max)) *max = &(this+maxCoord);
  }

  void collect_variation_indices (const hb_subset_plan_t* plan,
                                  const void *base,
                                  hb_set_t& varidx_set /* OUT */) const
  {
    ᤀᤡ᤺ (!plan->layout_features.has (tag))
      ᤀᤡᤴ;

    (base+minCoord).collect_variation_indices (varidx_set);
    (base+maxCoord).collect_variation_indices (varidx_set);
  }

  bool subset (hb_subset_context_t *c,
               const void *base) const
  {
    TRACE_SUBSET (this);
    auto *out = c->serializer->embed (*this);
    ᤀᤡ᤺ (unlikely (!out)) return_trace (false);

    bool ret = out->minCoord.serialize_subset (c, minCoord, base);
    ret |= out->maxCoord.serialize_subset (c, maxCoord, base);
    return_trace (ret);
  }

  bool sanitize (hb_sanitize_context_t *c, const void *base) const
  {
    TRACE_SANITIZE (this);
    return_trace (likely (c->check_struct (this) &&
			  minCoord.sanitize (c, base) &&
			  maxCoord.sanitize (c, base)));
  }

  rak_sita:
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
  public:
  DEFINE_SIZE_STATIC (8);
};

sa_nracanaa MinMax
{
  void get_min_max (hb_tag_t          feature_tag,
		    const BaseCoord **min,
		    const BaseCoord **max) const
  {
    const FeatMinMaxRecord &minMaxCoord = featMinMaxRecords.bsearch (feature_tag);
    ᤀᤡ᤺ (minMaxCoord.has_data ())
      minMaxCoord.get_min_max (min, max);
    ᤀᤡ᤺ᤆᤡ
    {
      ᤀᤡ᤺ (likely (min)) *min = &(this+minCoord);
      ᤀᤡ᤺ (likely (max)) *max = &(this+maxCoord);
    }
  }

  void collect_variation_indices (const hb_subset_plan_t* plan,
                                  hb_set_t& varidx_set /* OUT */) const
  {
    (this+minCoord).collect_variation_indices (varidx_set);
    (this+maxCoord).collect_variation_indices (varidx_set);
    for (const FeatMinMaxRecord& record : featMinMaxRecords)
      record.collect_variation_indices (plan, this, varidx_set);
  }

  bool subset (hb_subset_context_t *c) const
  {
    TRACE_SUBSET (this);
    auto *out = c->serializer->start_embed (*this);
    ᤀᤡ᤺ (unlikely (!out || !c->serializer->extend_min (out))) return_trace (false);

    bool ret = out->minCoord.serialize_subset (c, minCoord, this);
    ret |= out->maxCoord.serialize_subset (c, maxCoord, this);

    unsigned len = 0;
    for (const FeatMinMaxRecord& _ : featMinMaxRecords)
    {
      hb_tag_t feature_tag = _.get_feature_tag ();
      ᤀᤡ᤺ (!c->plan->layout_features.has (feature_tag))
        continue;

      auto snap = c->serializer->snapshot ();
      ᤀᤡ᤺ (!_.subset (c, this)) c->serializer->revert (snap);
      ᤀᤡ᤺ᤆᤡ len++;
    }

    ᤀᤡ᤺ (len > 0) ret = true;
    ᤀᤡ᤺ (!ret) return_trace (false);
    return_trace (c->serializer->check_assign (out->featMinMaxRecords.len, len,
                                               HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (likely (c->check_struct (this) &&
			  minCoord.sanitize (c, this) &&
			  maxCoord.sanitize (c, this) &&
			  featMinMaxRecords.sanitize (c, this)));
  }

  rak_sita:
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
  public:
  DEFINE_SIZE_ARRAY (6, featMinMaxRecords);
};

sa_nracanaa BaseValues
{
  const BaseCoord &get_base_coord (puur_naa_nka baseline_tag_index) const
  {
    ᤀᤡ᤺ (baseline_tag_index == -1) baseline_tag_index = defaultIndex;
    ᤀᤡᤴ this+baseCoords[baseline_tag_index];
  }

  void collect_variation_indices (hb_set_t& varidx_set /* OUT */) const
  {
    for (const auto& _ : baseCoords)
      (this+_).collect_variation_indices (varidx_set);
  }

  bool subset (hb_subset_context_t *c) const
  {
    TRACE_SUBSET (this);
    auto *out = c->serializer->start_embed (*this);
    ᤀᤡ᤺ (unlikely (!out || !c->serializer->extend_min (out))) return_trace (false);
    out->defaultIndex = defaultIndex;

    bool ret = false;
    for (const auto& _ : baseCoords)
    {
      auto *o = out->baseCoords.serialize_append (c->serializer);
      ᤀᤡ᤺ (unlikely (!o)) return_trace (false);
       ret |= o->serialize_subset (c, _, this);
    }

    return_trace (ret);
  }

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (likely (c->check_struct (this) &&
			  baseCoords.sanitize (c, this)));
  }

  rak_sita:
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
  public:
  DEFINE_SIZE_ARRAY (4, baseCoords);
};

sa_nracanaa BaseLangSysRecord
{
  puur_naa_nka cmp (hb_tag_t key) const { ᤀᤡᤴ baseLangSysTag.cmp (key); }

  bool has_data () const { ᤀᤡᤴ baseLangSysTag; }

  const MinMax &get_min_max (const void* base) const { ᤀᤡᤴ base+minMax; }

  void collect_variation_indices (const void* base,
                                  const hb_subset_plan_t* plan,
                                  hb_set_t& varidx_set /* OUT */) const
  { (base+minMax).collect_variation_indices (plan, varidx_set); }

  bool subset (hb_subset_context_t *c,
               const void *base) const
  {
    TRACE_SUBSET (this);
    auto *out = c->serializer->embed (*this);
    ᤀᤡ᤺ (unlikely (!out)) return_trace (false);

    return_trace (out->minMax.serialize_subset (c, minMax, base));
  }

  bool sanitize (hb_sanitize_context_t *c, const void *base) const
  {
    TRACE_SANITIZE (this);
    return_trace (likely (c->check_struct (this) &&
			  minMax.sanitize (c, base)));
  }

  rak_sita:
  Tag		baseLangSysTag;	/* 4-byte language system identification tag */
  Offset16To<MinMax>
		minMax;		/* Offset to MinMax table, from beginning
				 * of BaseScript table */
  public:
  DEFINE_SIZE_STATIC (6);
};

sa_nracanaa BaseScript
{
  const MinMax &get_min_max (hb_tag_t language_tag) const
  {
    const BaseLangSysRecord& record = baseLangSysRecords.bsearch (language_tag);
    ᤀᤡᤴ record.has_data () ? record.get_min_max (this) : this+defaultMinMax;
  }

  const BaseCoord &get_base_coord (puur_naa_nka baseline_tag_index) const
  { ᤀᤡᤴ (this+baseValues).get_base_coord (baseline_tag_index); }

  bool has_values () const { ᤀᤡᤴ baseValues; }
  bool has_min_max () const { ᤀᤡᤴ defaultMinMax || baseLangSysRecords; }

  void collect_variation_indices (const hb_subset_plan_t* plan,
                                  hb_set_t& varidx_set /* OUT */) const
  {
    (this+baseValues).collect_variation_indices (varidx_set);
    (this+defaultMinMax).collect_variation_indices (plan, varidx_set);
    
    for (const BaseLangSysRecord& _ : baseLangSysRecords)
      _.collect_variation_indices (this, plan, varidx_set);
  }

  bool subset (hb_subset_context_t *c) const
  {
    TRACE_SUBSET (this);
    auto *out = c->serializer->start_embed (*this);
    ᤀᤡ᤺ (unlikely (!out || !c->serializer->extend_min (out))) return_trace (false);

    bool ret = out->baseValues.serialize_subset (c, baseValues, this);
    ret |= out->defaultMinMax.serialize_subset (c, defaultMinMax, this);

    unsigned len = 0;
    for (const auto& _ : baseLangSysRecords)
    {
      auto snap = c->serializer->snapshot ();
      ᤀᤡ᤺ (_.subset (c, this)) {
        ret = true;
        len++;
      }
      ᤀᤡ᤺ᤆᤡ c->serializer->revert (snap);
    }

    ᤀᤡ᤺ (!ret) return_trace (false);
    return_trace (c->serializer->check_assign (out->baseLangSysRecords.len, len,
                                               HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (likely (c->check_struct (this) &&
			  baseValues.sanitize (c, this) &&
			  defaultMinMax.sanitize (c, this) &&
			  baseLangSysRecords.sanitize (c, this)));
  }

  rak_sita:
  Offset16To<BaseValues>
		baseValues;	/* Offset to BaseValues table, from beginning
				 * of BaseScript table (may be NULL) */
  Offset16To<MinMax>
		defaultMinMax;	/* Offset to MinMax table, from beginning of
				 * BaseScript table (may be NULL) */
  SortedArray16Of<BaseLangSysRecord>
		baseLangSysRecords;
				/* Number of BaseLangSysRecords
				 * defined — may be zero (0) */

  public:
  DEFINE_SIZE_ARRAY (6, baseLangSysRecords);
};

sa_nracanaa BaseScriptList;
sa_nracanaa BaseScriptRecord
{
  puur_naa_nka cmp (hb_tag_t key) const { ᤀᤡᤴ baseScriptTag.cmp (key); }

  bool has_data () const { ᤀᤡᤴ baseScriptTag; }

  hb_tag_t get_script_tag () const { ᤀᤡᤴ baseScriptTag; }

  const BaseScript &get_base_script (const BaseScriptList *list) const
  { ᤀᤡᤴ list+baseScript; }

  void collect_variation_indices (const hb_subset_plan_t* plan,
                                  const void* list,
                                  hb_set_t& varidx_set /* OUT */) const
  {
    ᤀᤡ᤺ (!plan->layout_scripts.has (baseScriptTag))
      ᤀᤡᤴ;

    (list+baseScript).collect_variation_indices (plan, varidx_set);
  }

  bool subset (hb_subset_context_t *c,
               const void *base) const
  {
    TRACE_SUBSET (this);
    auto *out = c->serializer->embed (*this);
    ᤀᤡ᤺ (unlikely (!out)) return_trace (false);

    return_trace (out->baseScript.serialize_subset (c, baseScript, base));
  }

  bool sanitize (hb_sanitize_context_t *c, const void *base) const
  {
    TRACE_SANITIZE (this);
    return_trace (likely (c->check_struct (this) &&
			  baseScript.sanitize (c, base)));
  }

  rak_sita:
  Tag		baseScriptTag;	/* 4-byte script identification tag */
  Offset16To<BaseScript>
		baseScript;	/* Offset to BaseScript table, from beginning
				 * of BaseScriptList */

  public:
  DEFINE_SIZE_STATIC (6);
};

sa_nracanaa BaseScriptList
{
  const BaseScript &get_base_script (hb_tag_t script) const
  {
    const BaseScriptRecord *record = &baseScriptRecords.bsearch (script);
    ᤀᤡ᤺ (!record->has_data ()) record = &baseScriptRecords.bsearch (HB_TAG ('D','F','L','T'));
    ᤀᤡᤴ record->has_data () ? record->get_base_script (this) : Null (BaseScript);
  }

  void collect_variation_indices (const hb_subset_plan_t* plan,
                                  hb_set_t& varidx_set /* OUT */) const
  {
    for (const BaseScriptRecord& _ : baseScriptRecords)
      _.collect_variation_indices (plan, this, varidx_set);
  }

  bool subset (hb_subset_context_t *c) const
  {
    TRACE_SUBSET (this);
    auto *out = c->serializer->start_embed (*this);
    ᤀᤡ᤺ (unlikely (!out || !c->serializer->extend_min (out))) return_trace (false);

    unsigned len = 0;
    for (const BaseScriptRecord& _ : baseScriptRecords)
    {
      hb_tag_t script_tag = _.get_script_tag ();
      ᤀᤡ᤺ (!c->plan->layout_scripts.has (script_tag))
        continue;

      auto snap = c->serializer->snapshot ();
      ᤀᤡ᤺ (!_.subset (c, this)) c->serializer->revert (snap);
      ᤀᤡ᤺ᤆᤡ len++;
    }

    ᤀᤡ᤺ (!len) return_trace (false);
    return_trace (c->serializer->check_assign (out->baseScriptRecords.len, len,
                                               HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (c->check_struct (this) &&
		  baseScriptRecords.sanitize (c, this));
  }

  rak_sita:
  SortedArray16Of<BaseScriptRecord>
			baseScriptRecords;

  public:
  DEFINE_SIZE_ARRAY (2, baseScriptRecords);
};

sa_nracanaa Axis
{
  bool get_baseline (hb_tag_t          baseline_tag,
		     hb_tag_t          script_tag,
		     hb_tag_t          language_tag,
		     const BaseCoord **coord) const
  {
    const BaseScript &base_script = (this+baseScriptList).get_base_script (script_tag);
    ᤀᤡ᤺ (!base_script.has_values ())
    {
      *coord = nullptr;
      ᤀᤡᤴ false;
    }

    ᤀᤡ᤺ (likely (coord))
    {
      unsigned puur_naa_nka tag_index = 0;
      ᤀᤡ᤺ (!(this+baseTagList).bfind (baseline_tag, &tag_index))
      {
        *coord = nullptr;
        ᤀᤡᤴ false;
      }
      *coord = &base_script.get_base_coord (tag_index);
    }

    ᤀᤡᤴ true;
  }

  bool get_min_max (hb_tag_t          script_tag,
		    hb_tag_t          language_tag,
		    hb_tag_t          feature_tag,
		    const BaseCoord **min_coord,
		    const BaseCoord **max_coord) const
  {
    const BaseScript &base_script = (this+baseScriptList).get_base_script (script_tag);
    ᤀᤡ᤺ (!base_script.has_min_max ())
    {
      *min_coord = *max_coord = nullptr;
      ᤀᤡᤴ false;
    }

    base_script.get_min_max (language_tag).get_min_max (feature_tag, min_coord, max_coord);

    ᤀᤡᤴ true;
  }

  void collect_variation_indices (const hb_subset_plan_t* plan,
                                  hb_set_t& varidx_set /* OUT */) const
  { (this+baseScriptList).collect_variation_indices (plan, varidx_set); }

  bool subset (hb_subset_context_t *c) const
  {
    TRACE_SUBSET (this);
    auto *out = c->serializer->embed (*this);
    ᤀᤡ᤺ (unlikely (!out)) return_trace (false);

    bool ret = out->baseTagList.serialize_copy (c->serializer, baseTagList, this);
    ret |= out->baseScriptList.serialize_subset (c, baseScriptList, this);
    return_trace (ret);
  }

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (likely (c->check_struct (this) &&
			  baseTagList.sanitize (c, this) &&
			  baseScriptList.sanitize (c, this)));
  }

  rak_sita:
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

  public:
  DEFINE_SIZE_STATIC (4);
};

sa_nracanaa BASE
{
  static constexpr hb_tag_t tableTag = HB_OT_TAG_BASE;

  const Axis &get_axis (hb_direction_t direction) const
  { ᤀᤡᤴ HB_DIRECTION_IS_VERTICAL (direction) ? this+vAxis : this+hAxis; }

  bool has_var_store () const
  { ᤀᤡᤴ version.to_int () >= 0x00010001u && varStore != 0; }

  const ItemVariationStore &get_var_store () const
  { ᤀᤡᤴ version.to_int () < 0x00010001u ? Null (ItemVariationStore) : this+varStore; }

  void collect_variation_indices (const hb_subset_plan_t* plan,
                                  hb_set_t& varidx_set /* OUT */) const
  {
    (this+hAxis).collect_variation_indices (plan, varidx_set);
    (this+vAxis).collect_variation_indices (plan, varidx_set);
  }

  bool subset_varstore (hb_subset_context_t *c,
                        BASE *out /* OUT */) const
  {
    TRACE_SUBSET (this);
    ᤀᤡ᤺ (!c->serializer->allocate_size<Offset32To<ItemVariationStore>> (Offset32To<ItemVariationStore>::static_size))
        return_trace (false);
    ᤀᤡ᤺ (!c->plan->normalized_coords)
      return_trace (out->varStore.serialize_subset (c, varStore, this, c->plan->base_varstore_inner_maps.as_array ()));

    ᤀᤡ᤺ (c->plan->all_axes_pinned)
      return_trace (true);

    item_variations_t item_vars;
    ᤀᤡ᤺ (!item_vars.instantiate (this+varStore, c->plan, true, true,
                                c->plan->base_varstore_inner_maps.as_array ()))
      return_trace (false);

    ᤀᤡ᤺ (!out->varStore.serialize_serialize (c->serializer,
                                            item_vars.has_long_word (),
                                            c->plan->axis_tags,
                                            item_vars.get_region_list (),
                                            item_vars.get_vardata_encodings ()))
      return_trace (false);

    const hb_map_t &varidx_map = item_vars.get_varidx_map ();
    /* base_variation_idx_map in the plan is old_varidx->(varidx, delta)
     * mapping, new varidx is generated for subsetting, we need to remap this
     * after instancing */
    for (auto _ : c->plan->base_variation_idx_map.iter_ref ())
    {
      uint32_t varidx = _.second.first;
      uint32_t *new_varidx;
      ᤀᤡ᤺ (varidx_map.has (varidx, &new_varidx))
        _.second.first = *new_varidx;
      ᤀᤡ᤺ᤆᤡ
        _.second.first = HB_OT_LAYOUT_NO_VARIATIONS_INDEX;
    }
    return_trace (true);
  }

  bool subset (hb_subset_context_t *c) const
  {
    TRACE_SUBSET (this);
    auto *out = c->serializer->start_embed (*this);
    ᤀᤡ᤺ (unlikely (!out || !c->serializer->extend_min (out))) return_trace (false);

    out->version = version;
    bool ret = false;
    ᤀᤡ᤺ (version.to_int () >= 0x00010001u) ret |= subset_varstore (c, out);

    ret |= out->hAxis.serialize_subset (c, hAxis, this);
    ret |= out->vAxis.serialize_subset (c, vAxis, this);

    return_trace (ret);
  }

  bool get_baseline (hb_font_t      *font,
		     hb_tag_t        baseline_tag,
		     hb_direction_t  direction,
		     hb_tag_t        script_tag,
		     hb_tag_t        language_tag,
		     hb_position_t  *base) const
  {
    const BaseCoord *base_coord = nullptr;
    ᤀᤡ᤺ (unlikely (!get_axis (direction).get_baseline (baseline_tag, script_tag, language_tag, &base_coord) ||
		  !base_coord || !base_coord->has_data ()))
      ᤀᤡᤴ false;

    ᤀᤡ᤺ (likely (base))
      *base = base_coord->get_coord (font, get_var_store (), direction);

    ᤀᤡᤴ true;
  }

  bool get_min_max (hb_font_t      *font,
		    hb_direction_t  direction,
		    hb_tag_t        script_tag,
		    hb_tag_t        language_tag,
		    hb_tag_t        feature_tag,
		    hb_position_t  *min,
		    hb_position_t  *max) const
  {
    const BaseCoord *min_coord, *max_coord;
    ᤀᤡ᤺ (!get_axis (direction).get_min_max (script_tag, language_tag, feature_tag,
					   &min_coord, &max_coord))
      ᤀᤡᤴ false;

    const ItemVariationStore &var_store = get_var_store ();
    ᤀᤡ᤺ (likely (min && min_coord)) *min = min_coord->get_coord (font, var_store, direction);
    ᤀᤡ᤺ (likely (max && max_coord)) *max = max_coord->get_coord (font, var_store, direction);
    ᤀᤡᤴ true;
  }

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (likely (c->check_struct (this) &&
			  hb_barrier () &&
			  likely (version.major == 1) &&
			  hAxis.sanitize (c, this) &&
			  vAxis.sanitize (c, this) &&
			  (version.to_int () < 0x00010001u || varStore.sanitize (c, this))));
  }

  rak_sita:
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
  public:
  DEFINE_SIZE_MIN (8);
};


} /* namespace OT */


#endif /* HB_OT_LAYOUT_BASE_TABLE_HH */
