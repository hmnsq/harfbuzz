/*
 * Copyright © 2017  Google, Inc.
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

#अगर_अघोषित HB_OT_VAR_HVAR_TABLE_HH
#घोषणा HB_OT_VAR_HVAR_TABLE_HH

#समावेश "hb-ot-layout-common.hh"
#समावेश "hb-ot-var-common.hh"

namespace OT {


काष्ठा सूचकांक_map_subset_plan_t
{
  क्रमागत सूचकांक_map_सूचकांक_t {
    ADV_सूचकांक,
    LSB_सूचकांक,	/* dual as TSB */
    RSB_सूचकांक,	/* dual as BSB */
    VORG_सूचकांक
  };

  व्योम init (स्थिर Delta_Set_सूचकांक_Map  *सूचकांक_map,
	     hb_inc_bimap_t	     &outer_map,
	     hb_vector_t<hb_set_t *> &inner_sets,
	     स्थिर hb_subset_plan_t  *plan,
	     bool bypass_empty = true)
  {
    map_गिनती = 0;
    full_map_गिनती = plan->new_to_old_gid_list.लंबाई ?
		     plan->new_to_old_gid_list.पुच्छ ().first + 1 : 0;
    /* Only the advance mapping may be dropped in favor of implicit
     * glyph-id indexing (for the others a null offset means no deltas),
     * and only when the source was map-less too, preserving its form. */
    can_drop_to_implicit = !bypass_empty && !सूचकांक_map;
    max_inners.init ();
    output_map.init ();

    अगर (bypass_empty && (!सूचकांक_map || !सूचकांक_map->get_map_गिनती ())) वापस;

    अचिन्हित पूर्णांक	last_val = (अचिन्हित पूर्णांक)-1;
    hb_codepoint_t	last_gid = HB_CODEPOINT_INVALID;

    max_inners.resize (inner_sets.लंबाई);
    क्रम (अचिन्हित i = 0; i < inner_sets.लंबाई; i++) max_inners[i] = 0;

    /* Search backwards for a map value different from the last map value */
    स्वतः &new_to_old_gid_list = plan->new_to_old_gid_list;
    अचिन्हित गिनती = new_to_old_gid_list.लंबाई;
    अगर (!सूचकांक_map)
    {
      map_गिनती = new_to_old_gid_list.पुच्छ ().first + 1;
    }
    अन्यथा
    {
      क्रम (अचिन्हित j = गिनती; j; j--)
      {
        hb_codepoint_t gid = new_to_old_gid_list.arrayZ[j - 1].first;
        hb_codepoint_t old_gid = new_to_old_gid_list.arrayZ[j - 1].second;
  
        अचिन्हित पूर्णांक v = सूचकांक_map->map (old_gid);
        अगर (last_gid == HB_CODEPOINT_INVALID)
        {
  	last_val = v;
  	last_gid = gid;
  	जारी;
        }
        अगर (v != last_val)
  	अवरोध;
  
        last_gid = gid;
      }
  
      अगर (unlikely (last_gid == (hb_codepoint_t)-1)) वापस;
      map_गिनती = last_gid + 1;
    }

    क्रम (स्वतः _ : plan->new_to_old_gid_list)
    {
      hb_codepoint_t gid = _.first;
      अगर (gid >= map_गिनती) अवरोध;

      hb_codepoint_t old_gid = _.second;
      अचिन्हित पूर्णांक v = सूचकांक_map ? सूचकांक_map->map (old_gid): old_gid;
      अचिन्हित पूर्णांक outer = v >> 16;
      अचिन्हित पूर्णांक inner = v & 0xFFFF;
      outer_map.add (outer);
      अगर (inner > max_inners[outer]) max_inners[outer] = inner;
      अगर (outer >= inner_sets.लंबाई) वापस;
      inner_sets[outer]->add (inner);
    }
  }

  व्योम fini ()
  {
    max_inners.fini ();
    output_map.fini ();
  }

  व्योम remap (स्थिर Delta_Set_सूचकांक_Map *निवेश_map,
	      स्थिर hb_inc_bimap_t &outer_map,
	      स्थिर hb_vector_t<hb_inc_bimap_t> &inner_maps,
	      स्थिर hb_subset_plan_t *plan)
  {
    outer_bit_गिनती = 1;
    inner_bit_गिनती = 1;

    क्रम (अचिन्हित पूर्णांक i = 0; i < max_inners.लंबाई; i++)
    {
      अगर (inner_maps[i].get_population () == 0) जारी;
      अचिन्हित पूर्णांक bit_गिनती = (max_inners[i]==0)? 1: hb_bit_storage (inner_maps[i][max_inners[i]]);
      अगर (bit_गिनती > inner_bit_गिनती) inner_bit_गिनती = bit_गिनती;
    }

    अगर (unlikely (!output_map.resize (map_गिनती))) वापस;
    क्रम (स्थिर स्वतः &_ : plan->new_to_old_gid_list)
    {
      hb_codepoint_t new_gid = _.first;
      hb_codepoint_t old_gid = _.second;

      अगर (unlikely (new_gid >= map_गिनती)) अवरोध;

      uint32_t v = निवेश_map? निवेश_map->map (old_gid) : old_gid;
      अचिन्हित outer = v >> 16;
      अचिन्हित new_outer = outer_map[outer];
      अचिन्हित bit_गिनती = (new_outer == 0) ? 1 : hb_bit_storage (new_outer);
      outer_bit_गिनती = hb_max (bit_गिनती, outer_bit_गिनती);

      output_map.arrayZ[new_gid] = (new_outer << 16) | (inner_maps[outer][v & 0xFFFF]);
    }
  }

  bool remap_after_instantiation (स्थिर hb_subset_plan_t *plan,
                                  स्थिर hb_map_t& varidx_map)
  {
    /* recalculate bit_count after remapping */
    outer_bit_गिनती = 1;
    inner_bit_गिनती = 1;

    क्रम (स्थिर स्वतः &_ : plan->new_to_old_gid_list)
    {
      hb_codepoint_t new_gid = _.first;
      अगर (unlikely (new_gid >= map_गिनती)) अवरोध;

      uint32_t v = output_map.arrayZ[new_gid];
      uint32_t *new_varidx;
      अगर (!varidx_map.has (v, &new_varidx))
        वापस false;

      output_map.arrayZ[new_gid] = *new_varidx;

      अचिन्हित outer = (*new_varidx) >> 16;
      अचिन्हित bit_गिनती = (outer == 0) ? 1 : hb_bit_storage (outer);
      outer_bit_गिनती = hb_max (bit_गिनती, outer_bit_गिनती);

      अचिन्हित inner = (*new_varidx) & 0xFFFF;
      bit_गिनती = (inner == 0) ? 1 : hb_bit_storage (inner);
      inner_bit_गिनती = hb_max (bit_गिनती, inner_bit_गिनती);
    }
    वापस true;
  }

  अचिन्हित पूर्णांक get_inner_bit_गिनती () स्थिर { वापस inner_bit_गिनती; }
  अचिन्हित पूर्णांक get_width ()           स्थिर { वापस ((outer_bit_गिनती + inner_bit_गिनती + 7) / 8); }
  अचिन्हित पूर्णांक get_map_गिनती ()       स्थिर { वापस map_गिनती; }

  माप_प्रकार get_आकार () स्थिर
  {
    अगर (!map_गिनती) वापस 0;
    वापस hb_unsigned_mul_add_saturate (get_width (), map_गिनती,
					 Delta_Set_सूचकांक_Map::min_आकार);
  }

  bool is_identity () स्थिर
  {
    अगर (!output_map) वापस true;
    /* An advance map whose entries are the identity over every output
     * glyph is equivalent to implicit glyph-id indexing; drop it.  A
     * truncated map doesn't qualify: glyphs past its end share its last
     * value, not their own ids. */
    अगर (!can_drop_to_implicit || map_गिनती != full_map_गिनती) वापस false;
    क्रम (अचिन्हित i = 0; i < map_गिनती; i++)
      अगर (output_map.arrayZ[i] != i) वापस false;
    वापस true;
  }
  hb_array_t<स्थिर uint32_t> get_output_map () स्थिर { वापस output_map.as_array (); }

  रक्षित:
  अचिन्हित पूर्णांक map_गिनती;
  अचिन्हित पूर्णांक full_map_गिनती;
  bool can_drop_to_implicit;
  hb_vector_t<अचिन्हित पूर्णांक> max_inners;
  अचिन्हित पूर्णांक outer_bit_गिनती;
  अचिन्हित पूर्णांक inner_bit_गिनती;
  hb_vector_t<uint32_t> output_map;
};

काष्ठा hvarvvar_subset_plan_t
{
  hvarvvar_subset_plan_t() : inner_maps (), सूचकांक_map_plans () {}
  ~hvarvvar_subset_plan_t() { fini (); }

  व्योम init (स्थिर hb_array_t<स्थिर Delta_Set_सूचकांक_Map *> &सूचकांक_maps,
	     स्थिर ItemVariationStore &_var_store,
	     स्थिर hb_subset_plan_t *plan)
  {
    सूचकांक_map_plans.resize (सूचकांक_maps.लंबाई);

    var_store = &_var_store;
    inner_sets.resize (var_store->get_sub_table_गिनती ());
    क्रम (अचिन्हित पूर्णांक i = 0; i < inner_sets.लंबाई; i++)
      inner_sets[i] = hb_set_create ();
    adv_set = hb_set_create ();

    inner_maps.resize (var_store->get_sub_table_गिनती ());

    अगर (unlikely (!सूचकांक_map_plans.लंबाई || !inner_sets.लंबाई || !inner_maps.लंबाई)) वापस;

    bool retain_adv_map = false;
    सूचकांक_map_plans[0].init (सूचकांक_maps[0], outer_map, inner_sets, plan, false);
    अगर (!सूचकांक_maps[0])
    {
      retain_adv_map = plan->flags & HB_SUBSET_FLAGS_RETAIN_GIDS;
      outer_map.add (0);
      क्रम (hb_codepoint_t old_gid : plan->glyphset()->iter())
        inner_sets[0]->add (old_gid);
      hb_set_union (adv_set, inner_sets[0]);
    }

    क्रम (अचिन्हित पूर्णांक i = 1; i < सूचकांक_maps.लंबाई; i++)
      सूचकांक_map_plans[i].init (सूचकांक_maps[i], outer_map, inner_sets, plan);

    outer_map.sort ();

    अगर (retain_adv_map)
    {
      क्रम (स्थिर स्वतः &_ : plan->new_to_old_gid_list)
      {
        hb_codepoint_t old_gid = _.second;
	inner_maps[0].add (old_gid);
      }
    }
    अन्यथा
    {
      inner_maps[0].add_set (adv_set);
      hb_set_subtract (inner_sets[0], adv_set);
      inner_maps[0].add_set (inner_sets[0]);
    }

    क्रम (अचिन्हित पूर्णांक i = 1; i < inner_maps.लंबाई; i++)
      inner_maps[i].add_set (inner_sets[i]);

    क्रम (अचिन्हित पूर्णांक i = 0; i < सूचकांक_maps.लंबाई; i++)
      सूचकांक_map_plans[i].remap (सूचकांक_maps[i], outer_map, inner_maps, plan);
  }

  /* remap */
  bool remap_सूचकांक_map_plans (स्थिर hb_subset_plan_t *plan,
                              स्थिर hb_map_t& varidx_map)
  {
    क्रम (अचिन्हित i = 0; i < सूचकांक_map_plans.लंबाई; i++)
      अगर (!सूचकांक_map_plans[i].remap_after_instantiation (plan, varidx_map))
        वापस false;
    वापस true;
  }

  व्योम fini ()
  {
    क्रम (अचिन्हित पूर्णांक i = 0; i < inner_sets.लंबाई; i++)
      hb_set_destroy (inner_sets[i]);
    hb_set_destroy (adv_set);
    inner_maps.fini ();
    सूचकांक_map_plans.fini ();
  }

  hb_inc_bimap_t outer_map;
  hb_vector_t<hb_inc_bimap_t> inner_maps;
  hb_vector_t<सूचकांक_map_subset_plan_t> सूचकांक_map_plans;
  स्थिर ItemVariationStore *var_store;

  रक्षित:
  hb_vector_t<hb_set_t *> inner_sets;
  hb_set_t *adv_set;
};

/*
 * HVAR -- Horizontal Metrics Variations
 * https://docs.microsoft.com/en-us/typography/opentype/spec/hvar
 * VVAR -- Vertical Metrics Variations
 * https://docs.microsoft.com/en-us/typography/opentype/spec/vvar
 */
#घोषणा HB_OT_TAG_HVAR HB_TAG('H','V','A','R')
#घोषणा HB_OT_TAG_VVAR HB_TAG('V','V','A','R')

काष्ठा HVARVVAR
{
  अटल constexpr hb_tag_t HVARTag = HB_OT_TAG_HVAR;
  अटल constexpr hb_tag_t VVARTag = HB_OT_TAG_VVAR;

  bool has_तथ्य () स्थिर { वापस version.major != 0; }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (version.sanitize (c) &&
		  hb_barrier () &&
		  likely (version.major == 1) &&
		  varStore.sanitize (c, यह) &&
		  advMap.sanitize (c, यह) &&
		  lsbMap.sanitize (c, यह) &&
		  rsbMap.sanitize (c, यह));
  }

  स्थिर ItemVariationStore& get_var_store () स्थिर
  { वापस यह+varStore; }

  व्योम listup_सूचकांक_maps (hb_vector_t<स्थिर Delta_Set_सूचकांक_Map *> &सूचकांक_maps) स्थिर
  {
    अगर (advMap) सूचकांक_maps.push (&(यह+advMap));
    अन्यथा सूचकांक_maps.push (nullptr);

    अगर (lsbMap) सूचकांक_maps.push (&(यह+lsbMap));
    अन्यथा सूचकांक_maps.push (nullptr);

    अगर (rsbMap) सूचकांक_maps.push (&(यह+rsbMap));
    अन्यथा सूचकांक_maps.push (nullptr);
  }

  bool serialize_सूचकांक_maps (hb_serialize_context_t *c,
			     स्थिर hb_array_t<सूचकांक_map_subset_plan_t> &im_plans)
  {
    TRACE_SERIALIZE (यह);
    अगर (im_plans[सूचकांक_map_subset_plan_t::ADV_सूचकांक].is_identity ())
      advMap = 0;
    अन्यथा अगर (unlikely (!advMap.serialize_serialize (c, im_plans[सूचकांक_map_subset_plan_t::ADV_सूचकांक])))
      return_trace (false);
    अगर (im_plans[सूचकांक_map_subset_plan_t::LSB_सूचकांक].is_identity ())
      lsbMap = 0;
    अन्यथा अगर (unlikely (!lsbMap.serialize_serialize (c, im_plans[सूचकांक_map_subset_plan_t::LSB_सूचकांक])))
      return_trace (false);
    अगर (im_plans[सूचकांक_map_subset_plan_t::RSB_सूचकांक].is_identity ())
      rsbMap = 0;
    अन्यथा अगर (unlikely (!rsbMap.serialize_serialize (c, im_plans[सूचकांक_map_subset_plan_t::RSB_सूचकांक])))
      return_trace (false);

    return_trace (true);
  }

  ढाँचा <typename T>
  bool _subset (hb_subset_context_t *c) स्थिर
  {
    TRACE_SUBSET (यह);
    अगर (c->plan->all_axes_pinned)
      return_trace (false);

    hvarvvar_subset_plan_t	hvar_plan;
    hb_vector_t<स्थिर Delta_Set_सूचकांक_Map *>
				सूचकांक_maps;

    ((T*)यह)->listup_सूचकांक_maps (सूचकांक_maps);
    hvar_plan.init (सूचकांक_maps.as_array (), यह+varStore, c->plan);

    T *out = c->serializer->allocate_min<T> ();
    अगर (unlikely (!out)) return_trace (false);

    out->version.major = 1;
    out->version.minor = 0;

    अगर (c->plan->normalized_coords)
    {
      item_variations_t item_vars;
      अगर (!item_vars.instantiate (यह+varStore, c->plan,
                                  advMap == 0 ? false : true,
                                  false, /* use_no_variation_idx = false */
                                  hvar_plan.inner_maps.as_array ()))
        return_trace (false);

      अगर (!out->varStore.serialize_serialize (c->serializer,
                                              item_vars.has_long_word (),
                                              c->plan->axis_tags,
                                              item_vars.get_region_list (),
                                              item_vars.get_vardata_encodings ()))
        return_trace (false);

      /* if varstore is optimized, remap output_map */
      अगर (advMap)
      {
        अगर (!hvar_plan.remap_सूचकांक_map_plans (c->plan, item_vars.get_varidx_map ()))
          return_trace (false);
      }
    }
    अन्यथा
    {
      /* avar2 partial instancing: cull unreachable regions. */
      hb_set_t dead_regions;
      अगर (c->plan->has_avar2)
	hvar_plan.var_store->collect_dead_regions (c->plan->axes_old_सूचकांक_tag_map,
						   c->plan->avar2_reachable_ranges,
						   dead_regions);

      अगर (unlikely (!out->varStore
		    .serialize_serialize (c->serializer,
					  hvar_plan.var_store,
					  hvar_plan.inner_maps.as_array (),
					  dead_regions.get_population () ? &dead_regions : nullptr)))
      return_trace (false);
    }

    return_trace (out->T::serialize_सूचकांक_maps (c->serializer,
						hvar_plan.सूचकांक_map_plans.as_array ()));
  }

  HB_ALWAYS_INLINE
  भग्न get_advance_delta_unscaled (hb_codepoint_t  glyph,
				    स्थिर पूर्णांक *coords, अचिन्हित पूर्णांक coord_गिनती,
				    hb_scalar_cache_t *store_cache = nullptr) स्थिर
  {
    /* Implicit mapping uses outer index zero and the glyph id as the
     * inner index. An oversized glyph id must not select another table. */
    अगर (!advMap && glyph > 0xFFFFu) वापस 0.f;
    uint32_t varidx = (यह+advMap).map (glyph);
    वापस (यह+varStore).get_delta (varidx,
				      coords, coord_गिनती,
				      store_cache);
  }

  खुला:
  FixedVersion<>version;	/* Version of the metrics variation table
				 * initially set to 0x00010000u */
  Offset32To<ItemVariationStore>
		varStore;	/* Offset to item variation store table. */
  Offset32To<Delta_Set_सूचकांक_Map>
		advMap;		/* Offset to advance var-idx mapping. */
  Offset32To<Delta_Set_सूचकांक_Map>
		lsbMap;		/* Offset to lsb/tsb var-idx mapping. */
  Offset32To<Delta_Set_सूचकांक_Map>
		rsbMap;		/* Offset to rsb/bsb var-idx mapping. */

  खुला:
  DEFINE_आकार_STATIC (20);
};

काष्ठा HVAR : HVARVVAR {
  अटल constexpr hb_tag_t tableTag = HB_OT_TAG_HVAR;
  bool subset (hb_subset_context_t *c) स्थिर { वापस HVARVVAR::_subset<HVAR> (c); }
};
काष्ठा VVAR : HVARVVAR {
  अटल constexpr hb_tag_t tableTag = HB_OT_TAG_VVAR;

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (static_cast<स्थिर HVARVVAR *> (यह)->sanitize (c) &&
		  vorgMap.sanitize (c, यह));
  }

  व्योम listup_सूचकांक_maps (hb_vector_t<स्थिर Delta_Set_सूचकांक_Map *> &सूचकांक_maps) स्थिर
  {
    HVARVVAR::listup_सूचकांक_maps (सूचकांक_maps);
    अगर (vorgMap) सूचकांक_maps.push (&(यह+vorgMap));
    अन्यथा सूचकांक_maps.push (nullptr);
  }

  bool serialize_सूचकांक_maps (hb_serialize_context_t *c,
			     स्थिर hb_array_t<सूचकांक_map_subset_plan_t> &im_plans)
  {
    TRACE_SERIALIZE (यह);
    अगर (unlikely (!HVARVVAR::serialize_सूचकांक_maps (c, im_plans)))
      return_trace (false);
    अगर (!im_plans[सूचकांक_map_subset_plan_t::VORG_सूचकांक].get_map_गिनती ())
      vorgMap = 0;
    अन्यथा अगर (unlikely (!vorgMap.serialize_serialize (c, im_plans[सूचकांक_map_subset_plan_t::VORG_सूचकांक])))
      return_trace (false);

    return_trace (true);
  }

  bool subset (hb_subset_context_t *c) स्थिर { वापस HVARVVAR::_subset<VVAR> (c); }

  HB_ALWAYS_INLINE
  भग्न get_vorg_delta_unscaled (hb_codepoint_t glyph,
				 स्थिर पूर्णांक *coords, अचिन्हित पूर्णांक coord_गिनती,
				 hb_scalar_cache_t *store_cache = nullptr) स्थिर
  {
    अगर (!vorgMap) वापस 0.f;
    uint32_t varidx = (यह+vorgMap).map (glyph);
    वापस (यह+varStore).get_delta (varidx,
				      coords, coord_गिनती,
				      store_cache);
  }

  रक्षित:
  Offset32To<Delta_Set_सूचकांक_Map>
		vorgMap;	/* Offset to vertical-origin var-idx mapping. */

  खुला:
  DEFINE_आकार_STATIC (24);
};

} /* namespace OT */


#पूर्ण_अगर /* HB_OT_VAR_HVAR_TABLE_HH */
