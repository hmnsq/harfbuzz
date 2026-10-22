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

#ifndef HB_OT_VAR_HVAR_TABLE_HH
#define HB_OT_VAR_HVAR_TABLE_HH

#अन्तर्भाव "hb-ot-layout-common.hh"
#अन्तर्भाव "hb-ot-var-common.hh"

नामाकाश OT {


संरचना index_map_subset_plan_t
{
  गणना index_map_index_t {
    ADV_INDEX,
    LSB_INDEX,	/* dual as TSB */
    RSB_INDEX,	/* dual as BSB */
    VORG_INDEX
  };

  शून्य init (स्थिरांक DeltaSetIndexMap  *index_map,
	     hb_inc_bimap_t	     &outer_map,
	     hb_vector_t<hb_set_t *> &inner_sets,
	     स्थिरांक hb_subset_plan_t  *plan,
	     तार्किक bypass_empty = सत्य)
  {
    map_मोजणी = 0;
    full_map_मोजणी = plan->new_to_old_gid_list.length ?
		     plan->new_to_old_gid_list.tail ().first + 1 : 0;
    /* Only the advance mapping may be dropped in favor of implicit
     * glyph-id indexing (for the others a null offset means no deltas),
     * and only when the source was map-less too, preserving its form. */
    can_drop_to_implicit = !bypass_empty && !index_map;
    max_inners.init ();
    output_map.init ();

    जर (bypass_empty && (!index_map || !index_map->get_map_मोजणी ())) परत;

    अचिन्हित पूर्णांक	last_val = (अचिन्हित पूर्णांक)-1;
    hb_codepoint_t	last_gid = HB_CODEPOINT_INVALID;

    max_inners.resize (inner_sets.length);
    साठी (अचिन्हित i = 0; i < inner_sets.length; i++) max_inners[i] = 0;

    /* Search backwards for a map value different from the last map value */
    स्वतः &new_to_old_gid_list = plan->new_to_old_gid_list;
    अचिन्हित मोजणी = new_to_old_gid_list.length;
    जर (!index_map)
    {
      map_मोजणी = new_to_old_gid_list.tail ().first + 1;
    }
    अन्यथा
    {
      साठी (अचिन्हित j = मोजणी; j; j--)
      {
        hb_codepoint_t gid = new_to_old_gid_list.arrayZ[j - 1].first;
        hb_codepoint_t old_gid = new_to_old_gid_list.arrayZ[j - 1].second;
  
        अचिन्हित पूर्णांक v = index_map->map (old_gid);
        जर (last_gid == HB_CODEPOINT_INVALID)
        {
  	last_val = v;
  	last_gid = gid;
  	सुरूठेवा;
        }
        जर (v != last_val)
  	थांबा;
  
        last_gid = gid;
      }
  
      जर (unlikely (last_gid == (hb_codepoint_t)-1)) परत;
      map_मोजणी = last_gid + 1;
    }

    साठी (स्वतः _ : plan->new_to_old_gid_list)
    {
      hb_codepoint_t gid = _.first;
      जर (gid >= map_मोजणी) थांबा;

      hb_codepoint_t old_gid = _.second;
      अचिन्हित पूर्णांक v = index_map ? index_map->map (old_gid): old_gid;
      अचिन्हित पूर्णांक outer = v >> 16;
      अचिन्हित पूर्णांक inner = v & 0xFFFF;
      outer_map.add (outer);
      जर (inner > max_inners[outer]) max_inners[outer] = inner;
      जर (outer >= inner_sets.length) परत;
      inner_sets[outer]->add (inner);
    }
  }

  शून्य fini ()
  {
    max_inners.fini ();
    output_map.fini ();
  }

  शून्य remap (स्थिरांक DeltaSetIndexMap *आदान_map,
	      स्थिरांक hb_inc_bimap_t &outer_map,
	      स्थिरांक hb_vector_t<hb_inc_bimap_t> &inner_maps,
	      स्थिरांक hb_subset_plan_t *plan)
  {
    outer_bit_मोजणी = 1;
    inner_bit_मोजणी = 1;

    साठी (अचिन्हित पूर्णांक i = 0; i < max_inners.length; i++)
    {
      जर (inner_maps[i].get_population () == 0) सुरूठेवा;
      अचिन्हित पूर्णांक bit_मोजणी = (max_inners[i]==0)? 1: hb_bit_storage (inner_maps[i][max_inners[i]]);
      जर (bit_मोजणी > inner_bit_मोजणी) inner_bit_मोजणी = bit_मोजणी;
    }

    जर (unlikely (!output_map.resize (map_मोजणी))) परत;
    साठी (स्थिरांक स्वतः &_ : plan->new_to_old_gid_list)
    {
      hb_codepoint_t new_gid = _.first;
      hb_codepoint_t old_gid = _.second;

      जर (unlikely (new_gid >= map_मोजणी)) थांबा;

      uint32_t v = आदान_map? आदान_map->map (old_gid) : old_gid;
      अचिन्हित outer = v >> 16;
      अचिन्हित new_outer = outer_map[outer];
      अचिन्हित bit_मोजणी = (new_outer == 0) ? 1 : hb_bit_storage (new_outer);
      outer_bit_मोजणी = hb_max (bit_मोजणी, outer_bit_मोजणी);

      output_map.arrayZ[new_gid] = (new_outer << 16) | (inner_maps[outer][v & 0xFFFF]);
    }
  }

  तार्किक remap_after_instantiation (स्थिरांक hb_subset_plan_t *plan,
                                  स्थिरांक hb_map_t& varidx_map)
  {
    /* recalculate bit_count after remapping */
    outer_bit_मोजणी = 1;
    inner_bit_मोजणी = 1;

    साठी (स्थिरांक स्वतः &_ : plan->new_to_old_gid_list)
    {
      hb_codepoint_t new_gid = _.first;
      जर (unlikely (new_gid >= map_मोजणी)) थांबा;

      uint32_t v = output_map.arrayZ[new_gid];
      uint32_t *new_varidx;
      जर (!varidx_map.has (v, &new_varidx))
        परत असत्य;

      output_map.arrayZ[new_gid] = *new_varidx;

      अचिन्हित outer = (*new_varidx) >> 16;
      अचिन्हित bit_मोजणी = (outer == 0) ? 1 : hb_bit_storage (outer);
      outer_bit_मोजणी = hb_max (bit_मोजणी, outer_bit_मोजणी);

      अचिन्हित inner = (*new_varidx) & 0xFFFF;
      bit_मोजणी = (inner == 0) ? 1 : hb_bit_storage (inner);
      inner_bit_मोजणी = hb_max (bit_मोजणी, inner_bit_मोजणी);
    }
    परत सत्य;
  }

  अचिन्हित पूर्णांक get_inner_bit_मोजणी () स्थिरांक { परत inner_bit_मोजणी; }
  अचिन्हित पूर्णांक get_width ()           स्थिरांक { परत ((outer_bit_मोजणी + inner_bit_मोजणी + 7) / 8); }
  अचिन्हित पूर्णांक get_map_मोजणी ()       स्थिरांक { परत map_मोजणी; }

  माप_प्रकार get_size () स्थिरांक
  {
    जर (!map_मोजणी) परत 0;
    परत hb_unsigned_mul_add_saturate (get_width (), map_मोजणी,
					 DeltaSetIndexMap::min_size);
  }

  तार्किक is_identity () स्थिरांक
  {
    जर (!output_map) परत सत्य;
    /* An advance map whose entries are the identity over every output
     * glyph is equivalent to implicit glyph-id indexing; drop it.  A
     * truncated map doesn't qualify: glyphs past its end share its last
     * value, not their own ids. */
    जर (!can_drop_to_implicit || map_मोजणी != full_map_मोजणी) परत असत्य;
    साठी (अचिन्हित i = 0; i < map_मोजणी; i++)
      जर (output_map.arrayZ[i] != i) परत असत्य;
    परत सत्य;
  }
  hb_array_t<स्थिरांक uint32_t> get_output_map () स्थिरांक { परत output_map.as_array (); }

  संरक्षित:
  अचिन्हित पूर्णांक map_मोजणी;
  अचिन्हित पूर्णांक full_map_मोजणी;
  तार्किक can_drop_to_implicit;
  hb_vector_t<अचिन्हित पूर्णांक> max_inners;
  अचिन्हित पूर्णांक outer_bit_मोजणी;
  अचिन्हित पूर्णांक inner_bit_मोजणी;
  hb_vector_t<uint32_t> output_map;
};

संरचना hvarvvar_subset_plan_t
{
  hvarvvar_subset_plan_t() : inner_maps (), index_map_plans () {}
  ~hvarvvar_subset_plan_t() { fini (); }

  शून्य init (स्थिरांक hb_array_t<स्थिरांक DeltaSetIndexMap *> &index_maps,
	     स्थिरांक ItemVariationStore &_var_store,
	     स्थिरांक hb_subset_plan_t *plan)
  {
    index_map_plans.resize (index_maps.length);

    var_store = &_var_store;
    inner_sets.resize (var_store->get_sub_table_मोजणी ());
    साठी (अचिन्हित पूर्णांक i = 0; i < inner_sets.length; i++)
      inner_sets[i] = hb_set_create ();
    adv_set = hb_set_create ();

    inner_maps.resize (var_store->get_sub_table_मोजणी ());

    जर (unlikely (!index_map_plans.length || !inner_sets.length || !inner_maps.length)) परत;

    तार्किक retain_adv_map = असत्य;
    index_map_plans[0].init (index_maps[0], outer_map, inner_sets, plan, असत्य);
    जर (!index_maps[0])
    {
      retain_adv_map = plan->flags & HB_SUBSET_FLAGS_RETAIN_GIDS;
      outer_map.add (0);
      साठी (hb_codepoint_t old_gid : plan->glyphset()->iter())
        inner_sets[0]->add (old_gid);
      hb_set_union (adv_set, inner_sets[0]);
    }

    साठी (अचिन्हित पूर्णांक i = 1; i < index_maps.length; i++)
      index_map_plans[i].init (index_maps[i], outer_map, inner_sets, plan);

    outer_map.sort ();

    जर (retain_adv_map)
    {
      साठी (स्थिरांक स्वतः &_ : plan->new_to_old_gid_list)
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

    साठी (अचिन्हित पूर्णांक i = 1; i < inner_maps.length; i++)
      inner_maps[i].add_set (inner_sets[i]);

    साठी (अचिन्हित पूर्णांक i = 0; i < index_maps.length; i++)
      index_map_plans[i].remap (index_maps[i], outer_map, inner_maps, plan);
  }

  /* remap */
  तार्किक remap_index_map_plans (स्थिरांक hb_subset_plan_t *plan,
                              स्थिरांक hb_map_t& varidx_map)
  {
    साठी (अचिन्हित i = 0; i < index_map_plans.length; i++)
      जर (!index_map_plans[i].remap_after_instantiation (plan, varidx_map))
        परत असत्य;
    परत सत्य;
  }

  शून्य fini ()
  {
    साठी (अचिन्हित पूर्णांक i = 0; i < inner_sets.length; i++)
      hb_set_destroy (inner_sets[i]);
    hb_set_destroy (adv_set);
    inner_maps.fini ();
    index_map_plans.fini ();
  }

  hb_inc_bimap_t outer_map;
  hb_vector_t<hb_inc_bimap_t> inner_maps;
  hb_vector_t<index_map_subset_plan_t> index_map_plans;
  स्थिरांक ItemVariationStore *var_store;

  संरक्षित:
  hb_vector_t<hb_set_t *> inner_sets;
  hb_set_t *adv_set;
};

/*
 * HVAR -- Horizontal Metrics Variations
 * https://docs.microsoft.com/en-us/typography/opentype/spec/hvar
 * VVAR -- Vertical Metrics Variations
 * https://docs.microsoft.com/en-us/typography/opentype/spec/vvar
 */
#define HB_OT_TAG_HVAR HB_TAG('H','V','A','R')
#define HB_OT_TAG_VVAR HB_TAG('V','V','A','R')

संरचना HVARVVAR
{
  स्थिर constexpr hb_tag_t HVARTag = HB_OT_TAG_HVAR;
  स्थिर constexpr hb_tag_t VVARTag = HB_OT_TAG_VVAR;

  तार्किक has_data () स्थिरांक { परत version.major != 0; }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (version.sanitize (c) &&
		  hb_barrier () &&
		  likely (version.major == 1) &&
		  varStore.sanitize (c, हे) &&
		  advMap.sanitize (c, हे) &&
		  lsbMap.sanitize (c, हे) &&
		  rsbMap.sanitize (c, हे));
  }

  स्थिरांक ItemVariationStore& get_var_store () स्थिरांक
  { परत हे+varStore; }

  शून्य listup_index_maps (hb_vector_t<स्थिरांक DeltaSetIndexMap *> &index_maps) स्थिरांक
  {
    जर (advMap) index_maps.push (&(हे+advMap));
    अन्यथा index_maps.push (nullptr);

    जर (lsbMap) index_maps.push (&(हे+lsbMap));
    अन्यथा index_maps.push (nullptr);

    जर (rsbMap) index_maps.push (&(हे+rsbMap));
    अन्यथा index_maps.push (nullptr);
  }

  तार्किक serialize_index_maps (hb_serialize_context_t *c,
			     स्थिरांक hb_array_t<index_map_subset_plan_t> &im_plans)
  {
    TRACE_SERIALIZE (हे);
    जर (im_plans[index_map_subset_plan_t::ADV_INDEX].is_identity ())
      advMap = 0;
    अन्यथा जर (unlikely (!advMap.serialize_serialize (c, im_plans[index_map_subset_plan_t::ADV_INDEX])))
      return_trace (असत्य);
    जर (im_plans[index_map_subset_plan_t::LSB_INDEX].is_identity ())
      lsbMap = 0;
    अन्यथा जर (unlikely (!lsbMap.serialize_serialize (c, im_plans[index_map_subset_plan_t::LSB_INDEX])))
      return_trace (असत्य);
    जर (im_plans[index_map_subset_plan_t::RSB_INDEX].is_identity ())
      rsbMap = 0;
    अन्यथा जर (unlikely (!rsbMap.serialize_serialize (c, im_plans[index_map_subset_plan_t::RSB_INDEX])))
      return_trace (असत्य);

    return_trace (सत्य);
  }

  ढाँचा <typename T>
  तार्किक _subset (hb_subset_context_t *c) स्थिरांक
  {
    TRACE_SUBSET (हे);
    जर (c->plan->all_axes_pinned)
      return_trace (असत्य);

    hvarvvar_subset_plan_t	hvar_plan;
    hb_vector_t<स्थिरांक DeltaSetIndexMap *>
				index_maps;

    ((T*)हे)->listup_index_maps (index_maps);
    hvar_plan.init (index_maps.as_array (), हे+varStore, c->plan);

    T *out = c->serializer->allocate_min<T> ();
    जर (unlikely (!out)) return_trace (असत्य);

    out->version.major = 1;
    out->version.minor = 0;

    जर (c->plan->normalized_coords)
    {
      item_variations_t item_vars;
      जर (!item_vars.instantiate (हे+varStore, c->plan,
                                  advMap == 0 ? असत्य : सत्य,
                                  असत्य, /* use_no_variation_idx = false */
                                  hvar_plan.inner_maps.as_array ()))
        return_trace (असत्य);

      जर (!out->varStore.serialize_serialize (c->serializer,
                                              item_vars.has_long_word (),
                                              c->plan->axis_tags,
                                              item_vars.get_region_list (),
                                              item_vars.get_vardata_encodings ()))
        return_trace (असत्य);

      /* if varstore is optimized, remap output_map */
      जर (advMap)
      {
        जर (!hvar_plan.remap_index_map_plans (c->plan, item_vars.get_varidx_map ()))
          return_trace (असत्य);
      }
    }
    अन्यथा
    {
      /* avar2 partial instancing: cull unreachable regions. */
      hb_set_t dead_regions;
      जर (c->plan->has_avar2)
	hvar_plan.var_store->collect_dead_regions (c->plan->axes_old_index_tag_map,
						   c->plan->avar2_reachable_ranges,
						   dead_regions);

      जर (unlikely (!out->varStore
		    .serialize_serialize (c->serializer,
					  hvar_plan.var_store,
					  hvar_plan.inner_maps.as_array (),
					  dead_regions.get_population () ? &dead_regions : nullptr)))
      return_trace (असत्य);
    }

    return_trace (out->T::serialize_index_maps (c->serializer,
						hvar_plan.index_map_plans.as_array ()));
  }

  HB_ALWAYS_INLINE
  दशांश get_advance_delta_unscaled (hb_codepoint_t  glyph,
				    स्थिरांक पूर्णांक *coords, अचिन्हित पूर्णांक coord_मोजणी,
				    hb_scalar_cache_t *store_cache = nullptr) स्थिरांक
  {
    /* Implicit mapping uses outer index zero and the glyph id as the
     * inner index. An oversized glyph id must not select another table. */
    जर (!advMap && glyph > 0xFFFFu) परत 0.f;
    uint32_t varidx = (हे+advMap).map (glyph);
    परत (हे+varStore).get_delta (varidx,
				      coords, coord_मोजणी,
				      store_cache);
  }

  सार्वजनिक:
  FixedVersion<>version;	/* Version of the metrics variation table
				 * initially set to 0x00010000u */
  Offset32To<ItemVariationStore>
		varStore;	/* Offset to item variation store table. */
  Offset32To<DeltaSetIndexMap>
		advMap;		/* Offset to advance var-idx mapping. */
  Offset32To<DeltaSetIndexMap>
		lsbMap;		/* Offset to lsb/tsb var-idx mapping. */
  Offset32To<DeltaSetIndexMap>
		rsbMap;		/* Offset to rsb/bsb var-idx mapping. */

  सार्वजनिक:
  DEFINE_SIZE_STATIC (20);
};

संरचना HVAR : HVARVVAR {
  स्थिर constexpr hb_tag_t tableTag = HB_OT_TAG_HVAR;
  तार्किक subset (hb_subset_context_t *c) स्थिरांक { परत HVARVVAR::_subset<HVAR> (c); }
};
संरचना VVAR : HVARVVAR {
  स्थिर constexpr hb_tag_t tableTag = HB_OT_TAG_VVAR;

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (static_cast<स्थिरांक HVARVVAR *> (हे)->sanitize (c) &&
		  vorgMap.sanitize (c, हे));
  }

  शून्य listup_index_maps (hb_vector_t<स्थिरांक DeltaSetIndexMap *> &index_maps) स्थिरांक
  {
    HVARVVAR::listup_index_maps (index_maps);
    जर (vorgMap) index_maps.push (&(हे+vorgMap));
    अन्यथा index_maps.push (nullptr);
  }

  तार्किक serialize_index_maps (hb_serialize_context_t *c,
			     स्थिरांक hb_array_t<index_map_subset_plan_t> &im_plans)
  {
    TRACE_SERIALIZE (हे);
    जर (unlikely (!HVARVVAR::serialize_index_maps (c, im_plans)))
      return_trace (असत्य);
    जर (!im_plans[index_map_subset_plan_t::VORG_INDEX].get_map_मोजणी ())
      vorgMap = 0;
    अन्यथा जर (unlikely (!vorgMap.serialize_serialize (c, im_plans[index_map_subset_plan_t::VORG_INDEX])))
      return_trace (असत्य);

    return_trace (सत्य);
  }

  तार्किक subset (hb_subset_context_t *c) स्थिरांक { परत HVARVVAR::_subset<VVAR> (c); }

  HB_ALWAYS_INLINE
  दशांश get_vorg_delta_unscaled (hb_codepoint_t glyph,
				 स्थिरांक पूर्णांक *coords, अचिन्हित पूर्णांक coord_मोजणी,
				 hb_scalar_cache_t *store_cache = nullptr) स्थिरांक
  {
    जर (!vorgMap) परत 0.f;
    uint32_t varidx = (हे+vorgMap).map (glyph);
    परत (हे+varStore).get_delta (varidx,
				      coords, coord_मोजणी,
				      store_cache);
  }

  संरक्षित:
  Offset32To<DeltaSetIndexMap>
		vorgMap;	/* Offset to vertical-origin var-idx mapping. */

  सार्वजनिक:
  DEFINE_SIZE_STATIC (24);
};

} /* namespace OT */


#endif /* HB_OT_VAR_HVAR_TABLE_HH */
