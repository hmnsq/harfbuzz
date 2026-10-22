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

#अगर_अघोषित HB_OT_LAYOUT_BASE_TABLE_HH
#घोषणा HB_OT_LAYOUT_BASE_TABLE_HH

#समावेश "hb-open-type.hh"
#समावेश "hb-ot-layout-common.hh"

namespace OT {

/*
 * BASE -- Baseline
 * https://docs.microsoft.com/en-us/typography/opentype/spec/base
 */

काष्ठा BaseCoordFormat1
{
  hb_position_t get_coord (hb_font_t *font, hb_direction_t direction) स्थिर
  {
    वापस HB_DIRECTION_IS_HORIZONTAL (direction) ? font->em_scale_y (coordinate) : font->em_scale_x (coordinate);
  }

  bool subset (hb_subset_context_t *c) स्थिर
  {
    TRACE_SUBSET (यह);
    return_trace ((bool) c->serializer->embed (*यह));
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह));
  }

  रक्षित:
  HBUINT16	format;		/* Format identifier--format = 1 */
  FWORD		coordinate;	/* X or Y value, in design units */
  खुला:
  DEFINE_आकार_STATIC (4);
};

ढाँचा <typename Types>
काष्ठा BaseCoordFormat2_4
{
  hb_position_t get_coord (hb_font_t *font, hb_direction_t direction) स्थिर
  {
    /* TODO */
    वापस HB_DIRECTION_IS_HORIZONTAL (direction) ? font->em_scale_y (coordinate) : font->em_scale_x (coordinate);
  }

  bool subset (hb_subset_context_t *c) स्थिर
  {
    TRACE_SUBSET (यह);
    स्वतः *out = c->serializer->embed (*यह);
    अगर (unlikely (!out)) return_trace (false);

    hb_codepoint_t *new_gid;
    अगर (!c->plan->glyph_map->has (referenceGlyph, &new_gid))
      return_trace (false);

    return_trace (c->serializer->check_assign (out->referenceGlyph,
                                               *new_gid,
                                               HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह));
  }

  रक्षित:
  HBUINT16	format;		/* Format identifier--format = 2 or 4 */
  FWORD		coordinate;	/* X or Y value, in design units */
  typename Types::HBGlyphID
		referenceGlyph;	/* Glyph ID of control glyph */
  HBUINT16	coordPoint;	/* Index of contour point on the
				 * reference glyph */
  खुला:
  DEFINE_आकार_STATIC (6 + Types::HBGlyphID::static_आकार);
};

काष्ठा BaseCoordFormat3
{
  hb_position_t get_coord (hb_font_t *font,
			   स्थिर ItemVariationStore &var_store,
			   hb_direction_t direction) स्थिर
  {
    स्थिर Device &device = यह+deviceTable;

    वापस HB_DIRECTION_IS_HORIZONTAL (direction)
	 ? hb_saturate_add (font->em_scale_y (coordinate), device.get_y_delta (font, var_store))
	 : hb_saturate_add (font->em_scale_x (coordinate), device.get_x_delta (font, var_store));
  }

  व्योम collect_variation_indices (hb_set_t& varidx_set /* OUT */) स्थिर
  {
    अचिन्हित varidx = (यह+deviceTable).get_variation_सूचकांक ();
    varidx_set.add (varidx);
  }

  bool subset (hb_subset_context_t *c) स्थिर
  {
    TRACE_SUBSET (यह);
    स्वतः *out = c->serializer->embed (*यह);
    अगर (unlikely (!out)) return_trace (false);

    अगर (!c->plan->pinned_at_default)
    {
      अचिन्हित var_idx = (यह+deviceTable).get_variation_सूचकांक ();
      अगर (var_idx != VarIdx::NO_VARIATION)
      {
        hb_pair_t<अचिन्हित, पूर्णांक> *v;
        अगर (!c->plan->base_variation_idx_map.has (var_idx, &v))
          return_trace (false);
        
        अगर (unlikely (!c->serializer->check_assign (out->coordinate, coordinate + hb_second (*v),
                                                    HB_SERIALIZE_ERROR_INT_OVERFLOW)))
          return_trace (false);
      }
    }
    
    out->deviceTable.serialize_copy (c->serializer, deviceTable,
                                     यह, 0,
                                     hb_serialize_context_t::शीर्ष,
                                     &c->plan->base_variation_idx_map);
    return_trace (true);
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (likely (c->check_struct (यह) &&
			  deviceTable.sanitize (c, यह)));
  }

  रक्षित:
  HBUINT16	format;		/* Format identifier--format = 3 */
  FWORD		coordinate;	/* X or Y value, in design units */
  Offset16To<Device>
		deviceTable;	/* Offset to Device table for X or
				 * Y value, from beginning of
				 * BaseCoord table (may be NULL). */
  खुला:
  DEFINE_आकार_STATIC (6);
};

काष्ठा BaseCoord
{
  bool has_तथ्य () स्थिर { वापस u.format.v; }

  hb_position_t get_coord (hb_font_t            *font,
			   स्थिर ItemVariationStore &var_store,
			   hb_direction_t        direction) स्थिर
  {
    चयन (u.format.v) {
    हाल 1: hb_barrier (); वापस u.format1.get_coord (font, direction);
    हाल 2: hb_barrier (); वापस u.format2.get_coord (font, direction);
    हाल 3: hb_barrier (); वापस u.format3.get_coord (font, var_store, direction);
#अगर_अघोषित HB_NO_BEYOND_64K
    हाल 4: hb_barrier (); वापस u.format4.get_coord (font, direction);
#पूर्ण_अगर
    शेष:वापस 0;
    }
  }

  व्योम collect_variation_indices (hb_set_t& varidx_set /* OUT */) स्थिर
  {
    चयन (u.format.v) {
    हाल 3: hb_barrier (); u.format3.collect_variation_indices (varidx_set); वापस;
    शेष:वापस;
    }
  }

  ढाँचा <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) स्थिर
  {
    अगर (unlikely (!c->may_dispatch (यह, &u.format.v))) वापस c->no_dispatch_return_मूल्य ();
    TRACE_DISPATCH (यह, u.format.v);
    चयन (u.format.v) {
    हाल 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    हाल 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    हाल 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
#अगर_अघोषित HB_NO_BEYOND_64K
    हाल 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
#पूर्ण_अगर
    शेष:return_trace (c->default_return_मूल्य ());
    }
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    अगर (unlikely (!u.format.v.sanitize (c))) return_trace (false);
    hb_barrier ();
    चयन (u.format.v) {
    हाल 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    हाल 2: hb_barrier (); return_trace (u.format2.sanitize (c));
    हाल 3: hb_barrier (); return_trace (u.format3.sanitize (c));
#अगर_अघोषित HB_NO_BEYOND_64K
    हाल 4: hb_barrier (); return_trace (u.format4.sanitize (c));
#पूर्ण_अगर
    शेष:return_trace (false);
    }
  }

  रक्षित:
  जोड़ {
  काष्ठा { HBUINT16 v; }	format;
  BaseCoordFormat1	format1;
  BaseCoordFormat2_4<Layout::SmallTypes>
			format2;
  BaseCoordFormat3	format3;
#अगर_अघोषित HB_NO_BEYOND_64K
  BaseCoordFormat2_4<Layout::MediumTypes>
			format4;
#पूर्ण_अगर
  } u;
  खुला:
  DEFINE_आकार_UNION (2, format.v);
};

काष्ठा FeatMinMaxRecord
{
  पूर्णांक cmp (hb_tag_t key) स्थिर { वापस tag.cmp (key); }

  bool has_तथ्य () स्थिर { वापस tag; }

  hb_tag_t get_feature_tag () स्थिर { वापस tag; }

  व्योम get_min_max (स्थिर BaseCoord **min, स्थिर BaseCoord **max) स्थिर
  {
    अगर (likely (min)) *min = &(यह+minCoord);
    अगर (likely (max)) *max = &(यह+maxCoord);
  }

  व्योम collect_variation_indices (स्थिर hb_subset_plan_t* plan,
                                  स्थिर व्योम *base,
                                  hb_set_t& varidx_set /* OUT */) स्थिर
  {
    अगर (!plan->layout_features.has (tag))
      वापस;

    (base+minCoord).collect_variation_indices (varidx_set);
    (base+maxCoord).collect_variation_indices (varidx_set);
  }

  bool subset (hb_subset_context_t *c,
               स्थिर व्योम *base) स्थिर
  {
    TRACE_SUBSET (यह);
    स्वतः *out = c->serializer->embed (*यह);
    अगर (unlikely (!out)) return_trace (false);

    bool ret = out->minCoord.serialize_subset (c, minCoord, base);
    ret |= out->maxCoord.serialize_subset (c, maxCoord, base);
    return_trace (ret);
  }

  bool sanitize (hb_sanitize_context_t *c, स्थिर व्योम *base) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (likely (c->check_struct (यह) &&
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
  खुला:
  DEFINE_आकार_STATIC (8);
};

काष्ठा MinMax
{
  व्योम get_min_max (hb_tag_t          feature_tag,
		    स्थिर BaseCoord **min,
		    स्थिर BaseCoord **max) स्थिर
  {
    स्थिर FeatMinMaxRecord &minMaxCoord = featMinMaxRecords.द्वा_खोज (feature_tag);
    अगर (minMaxCoord.has_तथ्य ())
      minMaxCoord.get_min_max (min, max);
    अन्यथा
    {
      अगर (likely (min)) *min = &(यह+minCoord);
      अगर (likely (max)) *max = &(यह+maxCoord);
    }
  }

  व्योम collect_variation_indices (स्थिर hb_subset_plan_t* plan,
                                  hb_set_t& varidx_set /* OUT */) स्थिर
  {
    (यह+minCoord).collect_variation_indices (varidx_set);
    (यह+maxCoord).collect_variation_indices (varidx_set);
    क्रम (स्थिर FeatMinMaxRecord& record : featMinMaxRecords)
      record.collect_variation_indices (plan, यह, varidx_set);
  }

  bool subset (hb_subset_context_t *c) स्थिर
  {
    TRACE_SUBSET (यह);
    स्वतः *out = c->serializer->start_embed (*यह);
    अगर (unlikely (!out || !c->serializer->extend_min (out))) return_trace (false);

    bool ret = out->minCoord.serialize_subset (c, minCoord, यह);
    ret |= out->maxCoord.serialize_subset (c, maxCoord, यह);

    अचिन्हित len = 0;
    क्रम (स्थिर FeatMinMaxRecord& _ : featMinMaxRecords)
    {
      hb_tag_t feature_tag = _.get_feature_tag ();
      अगर (!c->plan->layout_features.has (feature_tag))
        जारी;

      स्वतः snap = c->serializer->snapshot ();
      अगर (!_.subset (c, यह)) c->serializer->revert (snap);
      अन्यथा len++;
    }

    अगर (len > 0) ret = true;
    अगर (!ret) return_trace (false);
    return_trace (c->serializer->check_assign (out->featMinMaxRecords.len, len,
                                               HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (likely (c->check_struct (यह) &&
			  minCoord.sanitize (c, यह) &&
			  maxCoord.sanitize (c, यह) &&
			  featMinMaxRecords.sanitize (c, यह)));
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
  खुला:
  DEFINE_आकार_ARRAY (6, featMinMaxRecords);
};

काष्ठा Base_मूल्यावली
{
  स्थिर BaseCoord &get_base_coord (पूर्णांक baseline_tag_सूचकांक) स्थिर
  {
    अगर (baseline_tag_सूचकांक == -1) baseline_tag_सूचकांक = default_सूचकांक_2;
    वापस यह+baseCoords[baseline_tag_सूचकांक];
  }

  व्योम collect_variation_indices (hb_set_t& varidx_set /* OUT */) स्थिर
  {
    क्रम (स्थिर स्वतः& _ : baseCoords)
      (यह+_).collect_variation_indices (varidx_set);
  }

  bool subset (hb_subset_context_t *c) स्थिर
  {
    TRACE_SUBSET (यह);
    स्वतः *out = c->serializer->start_embed (*यह);
    अगर (unlikely (!out || !c->serializer->extend_min (out))) return_trace (false);
    out->default_सूचकांक_2 = default_सूचकांक_2;

    bool ret = false;
    क्रम (स्थिर स्वतः& _ : baseCoords)
    {
      स्वतः *o = out->baseCoords.serialize_append (c->serializer);
      अगर (unlikely (!o)) return_trace (false);
       ret |= o->serialize_subset (c, _, यह);
    }

    return_trace (ret);
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (likely (c->check_struct (यह) &&
			  baseCoords.sanitize (c, यह)));
  }

  रक्षित:
  सूचकांक		default_सूचकांक_2;	/* Index number of default baseline for this
				 * script — equals index position of baseline tag
				 * in baselineTags array of the BaseTagList */
  Array16OfOffset16To<BaseCoord>
		baseCoords;	/* Number of BaseCoord tables defined — should equal
				 * baseTagCount in the BaseTagList
				 *
				 * Array of offsets to BaseCoord tables, from beginning of
				 * BaseValues table — order matches baselineTags array in
				 * the BaseTagList */
  खुला:
  DEFINE_आकार_ARRAY (4, baseCoords);
};

काष्ठा BaseLangSysRecord
{
  पूर्णांक cmp (hb_tag_t key) स्थिर { वापस baseLangSysTag.cmp (key); }

  bool has_तथ्य () स्थिर { वापस baseLangSysTag; }

  स्थिर MinMax &get_min_max (स्थिर व्योम* base) स्थिर { वापस base+minMax; }

  व्योम collect_variation_indices (स्थिर व्योम* base,
                                  स्थिर hb_subset_plan_t* plan,
                                  hb_set_t& varidx_set /* OUT */) स्थिर
  { (base+minMax).collect_variation_indices (plan, varidx_set); }

  bool subset (hb_subset_context_t *c,
               स्थिर व्योम *base) स्थिर
  {
    TRACE_SUBSET (यह);
    स्वतः *out = c->serializer->embed (*यह);
    अगर (unlikely (!out)) return_trace (false);

    return_trace (out->minMax.serialize_subset (c, minMax, base));
  }

  bool sanitize (hb_sanitize_context_t *c, स्थिर व्योम *base) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (likely (c->check_struct (यह) &&
			  minMax.sanitize (c, base)));
  }

  रक्षित:
  Tag		baseLangSysTag;	/* 4-byte language system identification tag */
  Offset16To<MinMax>
		minMax;		/* Offset to MinMax table, from beginning
				 * of BaseScript table */
  खुला:
  DEFINE_आकार_STATIC (6);
};

काष्ठा BaseScript
{
  स्थिर MinMax &get_min_max (hb_tag_t language_tag) स्थिर
  {
    स्थिर BaseLangSysRecord& record = baseLangSysRecords.द्वा_खोज (language_tag);
    वापस record.has_तथ्य () ? record.get_min_max (यह) : यह+defaultMinMax;
  }

  स्थिर BaseCoord &get_base_coord (पूर्णांक baseline_tag_सूचकांक) स्थिर
  { वापस (यह+base_मूल्यावली).get_base_coord (baseline_tag_सूचकांक); }

  bool has_मूल्यावली () स्थिर { वापस base_मूल्यावली; }
  bool has_min_max () स्थिर { वापस defaultMinMax || baseLangSysRecords; }

  व्योम collect_variation_indices (स्थिर hb_subset_plan_t* plan,
                                  hb_set_t& varidx_set /* OUT */) स्थिर
  {
    (यह+base_मूल्यावली).collect_variation_indices (varidx_set);
    (यह+defaultMinMax).collect_variation_indices (plan, varidx_set);
    
    क्रम (स्थिर BaseLangSysRecord& _ : baseLangSysRecords)
      _.collect_variation_indices (यह, plan, varidx_set);
  }

  bool subset (hb_subset_context_t *c) स्थिर
  {
    TRACE_SUBSET (यह);
    स्वतः *out = c->serializer->start_embed (*यह);
    अगर (unlikely (!out || !c->serializer->extend_min (out))) return_trace (false);

    bool ret = out->base_मूल्यावली.serialize_subset (c, base_मूल्यावली, यह);
    ret |= out->defaultMinMax.serialize_subset (c, defaultMinMax, यह);

    अचिन्हित len = 0;
    क्रम (स्थिर स्वतः& _ : baseLangSysRecords)
    {
      स्वतः snap = c->serializer->snapshot ();
      अगर (_.subset (c, यह)) {
        ret = true;
        len++;
      }
      अन्यथा c->serializer->revert (snap);
    }

    अगर (!ret) return_trace (false);
    return_trace (c->serializer->check_assign (out->baseLangSysRecords.len, len,
                                               HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (likely (c->check_struct (यह) &&
			  base_मूल्यावली.sanitize (c, यह) &&
			  defaultMinMax.sanitize (c, यह) &&
			  baseLangSysRecords.sanitize (c, यह)));
  }

  रक्षित:
  Offset16To<Base_मूल्यावली>
		base_मूल्यावली;	/* Offset to BaseValues table, from beginning
				 * of BaseScript table (may be NULL) */
  Offset16To<MinMax>
		defaultMinMax;	/* Offset to MinMax table, from beginning of
				 * BaseScript table (may be NULL) */
  SortedArray16Of<BaseLangSysRecord>
		baseLangSysRecords;
				/* Number of BaseLangSysRecords
				 * defined — may be zero (0) */

  खुला:
  DEFINE_आकार_ARRAY (6, baseLangSysRecords);
};

काष्ठा BaseScriptList;
काष्ठा BaseScriptRecord
{
  पूर्णांक cmp (hb_tag_t key) स्थिर { वापस baseScriptTag.cmp (key); }

  bool has_तथ्य () स्थिर { वापस baseScriptTag; }

  hb_tag_t get_script_tag () स्थिर { वापस baseScriptTag; }

  स्थिर BaseScript &get_base_script (स्थिर BaseScriptList *list) स्थिर
  { वापस list+baseScript; }

  व्योम collect_variation_indices (स्थिर hb_subset_plan_t* plan,
                                  स्थिर व्योम* list,
                                  hb_set_t& varidx_set /* OUT */) स्थिर
  {
    अगर (!plan->layout_scripts.has (baseScriptTag))
      वापस;

    (list+baseScript).collect_variation_indices (plan, varidx_set);
  }

  bool subset (hb_subset_context_t *c,
               स्थिर व्योम *base) स्थिर
  {
    TRACE_SUBSET (यह);
    स्वतः *out = c->serializer->embed (*यह);
    अगर (unlikely (!out)) return_trace (false);

    return_trace (out->baseScript.serialize_subset (c, baseScript, base));
  }

  bool sanitize (hb_sanitize_context_t *c, स्थिर व्योम *base) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (likely (c->check_struct (यह) &&
			  baseScript.sanitize (c, base)));
  }

  रक्षित:
  Tag		baseScriptTag;	/* 4-byte script identification tag */
  Offset16To<BaseScript>
		baseScript;	/* Offset to BaseScript table, from beginning
				 * of BaseScriptList */

  खुला:
  DEFINE_आकार_STATIC (6);
};

काष्ठा BaseScriptList
{
  स्थिर BaseScript &get_base_script (hb_tag_t script) स्थिर
  {
    स्थिर BaseScriptRecord *record = &baseScriptRecords.द्वा_खोज (script);
    अगर (!record->has_तथ्य ()) record = &baseScriptRecords.द्वा_खोज (HB_TAG ('D','F','L','T'));
    वापस record->has_तथ्य () ? record->get_base_script (यह) : Null (BaseScript);
  }

  व्योम collect_variation_indices (स्थिर hb_subset_plan_t* plan,
                                  hb_set_t& varidx_set /* OUT */) स्थिर
  {
    क्रम (स्थिर BaseScriptRecord& _ : baseScriptRecords)
      _.collect_variation_indices (plan, यह, varidx_set);
  }

  bool subset (hb_subset_context_t *c) स्थिर
  {
    TRACE_SUBSET (यह);
    स्वतः *out = c->serializer->start_embed (*यह);
    अगर (unlikely (!out || !c->serializer->extend_min (out))) return_trace (false);

    अचिन्हित len = 0;
    क्रम (स्थिर BaseScriptRecord& _ : baseScriptRecords)
    {
      hb_tag_t script_tag = _.get_script_tag ();
      अगर (!c->plan->layout_scripts.has (script_tag))
        जारी;

      स्वतः snap = c->serializer->snapshot ();
      अगर (!_.subset (c, यह)) c->serializer->revert (snap);
      अन्यथा len++;
    }

    अगर (!len) return_trace (false);
    return_trace (c->serializer->check_assign (out->baseScriptRecords.len, len,
                                               HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह) &&
		  baseScriptRecords.sanitize (c, यह));
  }

  रक्षित:
  SortedArray16Of<BaseScriptRecord>
			baseScriptRecords;

  खुला:
  DEFINE_आकार_ARRAY (2, baseScriptRecords);
};

काष्ठा Axis
{
  bool get_baseline (hb_tag_t          baseline_tag,
		     hb_tag_t          script_tag,
		     hb_tag_t          language_tag,
		     स्थिर BaseCoord **coord) स्थिर
  {
    स्थिर BaseScript &base_script = (यह+baseScriptList).get_base_script (script_tag);
    अगर (!base_script.has_मूल्यावली ())
    {
      *coord = nullptr;
      वापस false;
    }

    अगर (likely (coord))
    {
      अचिन्हित पूर्णांक tag_सूचकांक = 0;
      अगर (!(यह+baseTagList).bfind (baseline_tag, &tag_सूचकांक))
      {
        *coord = nullptr;
        वापस false;
      }
      *coord = &base_script.get_base_coord (tag_सूचकांक);
    }

    वापस true;
  }

  bool get_min_max (hb_tag_t          script_tag,
		    hb_tag_t          language_tag,
		    hb_tag_t          feature_tag,
		    स्थिर BaseCoord **min_coord,
		    स्थिर BaseCoord **max_coord) स्थिर
  {
    स्थिर BaseScript &base_script = (यह+baseScriptList).get_base_script (script_tag);
    अगर (!base_script.has_min_max ())
    {
      *min_coord = *max_coord = nullptr;
      वापस false;
    }

    base_script.get_min_max (language_tag).get_min_max (feature_tag, min_coord, max_coord);

    वापस true;
  }

  व्योम collect_variation_indices (स्थिर hb_subset_plan_t* plan,
                                  hb_set_t& varidx_set /* OUT */) स्थिर
  { (यह+baseScriptList).collect_variation_indices (plan, varidx_set); }

  bool subset (hb_subset_context_t *c) स्थिर
  {
    TRACE_SUBSET (यह);
    स्वतः *out = c->serializer->embed (*यह);
    अगर (unlikely (!out)) return_trace (false);

    bool ret = out->baseTagList.serialize_copy (c->serializer, baseTagList, यह);
    ret |= out->baseScriptList.serialize_subset (c, baseScriptList, यह);
    return_trace (ret);
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (likely (c->check_struct (यह) &&
			  baseTagList.sanitize (c, यह) &&
			  baseScriptList.sanitize (c, यह)));
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

  खुला:
  DEFINE_आकार_STATIC (4);
};

काष्ठा BASE
{
  अटल constexpr hb_tag_t tableTag = HB_OT_TAG_BASE;

  स्थिर Axis &get_axis (hb_direction_t direction) स्थिर
  { वापस HB_DIRECTION_IS_VERTICAL (direction) ? यह+vAxis : यह+hAxis; }

  bool has_var_store () स्थिर
  { वापस version.to_int () >= 0x00010001u && varStore != 0; }

  स्थिर ItemVariationStore &get_var_store () स्थिर
  { वापस version.to_int () < 0x00010001u ? Null (ItemVariationStore) : यह+varStore; }

  व्योम collect_variation_indices (स्थिर hb_subset_plan_t* plan,
                                  hb_set_t& varidx_set /* OUT */) स्थिर
  {
    (यह+hAxis).collect_variation_indices (plan, varidx_set);
    (यह+vAxis).collect_variation_indices (plan, varidx_set);
  }

  bool subset_varstore (hb_subset_context_t *c,
                        BASE *out /* OUT */) स्थिर
  {
    TRACE_SUBSET (यह);
    अगर (!c->serializer->allocate_आकार<Offset32To<ItemVariationStore>> (Offset32To<ItemVariationStore>::static_आकार))
        return_trace (false);
    अगर (!c->plan->normalized_coords)
      return_trace (out->varStore.serialize_subset (c, varStore, यह, c->plan->base_varstore_inner_maps.as_array ()));

    अगर (c->plan->all_axes_pinned)
      return_trace (true);

    item_variations_t item_vars;
    अगर (!item_vars.instantiate (यह+varStore, c->plan, true, true,
                                c->plan->base_varstore_inner_maps.as_array ()))
      return_trace (false);

    अगर (!out->varStore.serialize_serialize (c->serializer,
                                            item_vars.has_long_word (),
                                            c->plan->axis_tags,
                                            item_vars.get_region_list (),
                                            item_vars.get_vardata_encodings ()))
      return_trace (false);

    स्थिर hb_map_t &varidx_map = item_vars.get_varidx_map ();
    /* base_variation_idx_map in the plan is old_varidx->(varidx, delta)
     * mapping, new varidx is generated for subsetting, we need to remap this
     * after instancing */
    क्रम (स्वतः _ : c->plan->base_variation_idx_map.iter_ref ())
    {
      uint32_t varidx = _.second.first;
      uint32_t *new_varidx;
      अगर (varidx_map.has (varidx, &new_varidx))
        _.second.first = *new_varidx;
      अन्यथा
        _.second.first = HB_OT_LAYOUT_NO_VARIATIONS_सूचकांक;
    }
    return_trace (true);
  }

  bool subset (hb_subset_context_t *c) स्थिर
  {
    TRACE_SUBSET (यह);
    स्वतः *out = c->serializer->start_embed (*यह);
    अगर (unlikely (!out || !c->serializer->extend_min (out))) return_trace (false);

    out->version = version;
    bool ret = false;
    अगर (version.to_int () >= 0x00010001u) ret |= subset_varstore (c, out);

    ret |= out->hAxis.serialize_subset (c, hAxis, यह);
    ret |= out->vAxis.serialize_subset (c, vAxis, यह);

    return_trace (ret);
  }

  bool get_baseline (hb_font_t      *font,
		     hb_tag_t        baseline_tag,
		     hb_direction_t  direction,
		     hb_tag_t        script_tag,
		     hb_tag_t        language_tag,
		     hb_position_t  *base) स्थिर
  {
    स्थिर BaseCoord *base_coord = nullptr;
    अगर (unlikely (!get_axis (direction).get_baseline (baseline_tag, script_tag, language_tag, &base_coord) ||
		  !base_coord || !base_coord->has_तथ्य ()))
      वापस false;

    अगर (likely (base))
      *base = base_coord->get_coord (font, get_var_store (), direction);

    वापस true;
  }

  bool get_min_max (hb_font_t      *font,
		    hb_direction_t  direction,
		    hb_tag_t        script_tag,
		    hb_tag_t        language_tag,
		    hb_tag_t        feature_tag,
		    hb_position_t  *min,
		    hb_position_t  *max) स्थिर
  {
    स्थिर BaseCoord *min_coord, *max_coord;
    अगर (!get_axis (direction).get_min_max (script_tag, language_tag, feature_tag,
					   &min_coord, &max_coord))
      वापस false;

    स्थिर ItemVariationStore &var_store = get_var_store ();
    अगर (likely (min && min_coord)) *min = min_coord->get_coord (font, var_store, direction);
    अगर (likely (max && max_coord)) *max = max_coord->get_coord (font, var_store, direction);
    वापस true;
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (likely (c->check_struct (यह) &&
			  hb_barrier () &&
			  likely (version.major == 1) &&
			  hAxis.sanitize (c, यह) &&
			  vAxis.sanitize (c, यह) &&
			  (version.to_int () < 0x00010001u || varStore.sanitize (c, यह))));
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
  खुला:
  DEFINE_आकार_MIN (8);
};


} /* namespace OT */


#पूर्ण_अगर /* HB_OT_LAYOUT_BASE_TABLE_HH */
