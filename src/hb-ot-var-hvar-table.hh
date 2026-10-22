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

  शून्य init (अचर DeltaSetIndexMap  *index_map,
	     hb_inc_bimap_t	     &outer_map,
	     hb_vector_t<hb_set_t *> &inner_sets,
	     अचर hb_subset_plan_t  *plan,
	     तार्किक bypass_empty = सत्य)
  {
    map_गन्ती = 0;
    full_map_गन्ती = plan->new_to_old_gid_list.length ?
		     plan->new_to_old_gid_list.tail ().first + 1 : 0;
    /* Only the advance mapping may be dropped in favor of implicit
     * glyph-id indexing (for the others a null offset means no deltas),
     * and only when the source was map-less too, preserving its form. */
    can_drop_to_implicit = !bypass_empty && !index_map;
    max_inners.init ();
    output_map.init ();

    यदि (bypass_empty && (!index_map || !index_map->get_map_गन्ती ())) फर्काऊ;

    अचिन्हित पूर्णाङ्क	last_val = (अचिन्हित पूर्णाङ्क)-1;
    hb_codepoint_t	last_gid = HB_CODEPOINT_INVALID;

    max_inners.resize (inner_sets.length);
    लागि (अचिन्हित i = 0; i < inner_sets.length; i++) max_inners[i] = 0;

    /* Search backwards for a map value different from the last map value */
    स्वतः &new_to_old_gid_list = plan->new_to_old_gid_list;
    अचिन्हित गन्ती = new_to_old_gid_list.length;
    यदि (!index_map)
    {
      map_गन्ती = new_to_old_gid_list.tail ().first + 1;
    }
    अन्यथा
    {
      लागि (अचिन्हित j = गन्ती; j; j--)
      {
        hb_codepoint_t gid = new_to_old_gid_list.arrayZ[j - 1].first;
        hb_codepoint_t old_gid = new_to_old_gid_list.arrayZ[j - 1].second;
  
        अचिन्हित पूर्णाङ्क v = index_map->map (old_gid);
        यदि (last_gid == HB_CODEPOINT_INVALID)
        {
  	last_val = v;
  	last_gid = gid;
  	जारी;
        }
        यदि (v != last_val)
  	रोक;
  
        last_gid = gid;
      }
  
      यदि (unlikely (last_gid == (hb_codepoint_t)-1)) फर्काऊ;
      map_गन्ती = last_gid + 1;
    }

    लागि (स्वतः _ : plan->new_to_old_gid_list)
    {
      hb_codepoint_t gid = _.first;
      यदि (gid >= map_गन्ती) रोक;

      hb_codepoint_t old_gid = _.second;
      अचिन्हित पूर्णाङ्क v = index_map ? index_map->map (old_gid): old_gid;
      अचिन्हित पूर्णाङ्क outer = v >> 16;
      अचिन्हित पूर्णाङ्क inner = v & 0xFFFF;
      outer_map.add (outer);
      यदि (inner > max_inners[outer]) max_inners[outer] = inner;
      यदि (outer >= inner_sets.length) फर्काऊ;
      inner_sets[outer]->add (inner);
    }
  }

  शून्य fini ()
  {
    max_inners.fini ();
    output_map.fini ();
  }

  शून्य remap (अचर DeltaSetIndexMap *आगत_map,
	      अचर hb_inc_bimap_t &outer_map,
	      अचर hb_vector_t<hb_inc_bimap_t> &inner_maps,
	      अचर hb_subset_plan_t *plan)
  {
    outer_bit_गन्ती = 1;
    inner_bit_गन्ती = 1;

    लागि (अचिन्हित पूर्णाङ्क i = 0; i < max_inners.length; i++)
    {
      यदि (inner_maps[i].get_population () == 0) जारी;
      अचिन्हित पूर्णाङ्क bit_गन्ती = (max_inners[i]==0)? 1: hb_bit_storage (inner_maps[i][max_inners[i]]);
      यदि (bit_गन्ती > inner_bit_गन्ती) inner_bit_गन्ती = bit_गन्ती;
    }

    यदि (unlikely (!output_map.resize (map_गन्ती))) फर्काऊ;
    लागि (अचर स्वतः &_ : plan->new_to_old_gid_list)
    {
      hb_codepoint_t new_gid = _.first;
      hb_codepoint_t old_gid = _.second;

      यदि (unlikely (new_gid >= map_गन्ती)) रोक;

      uint32_t v = आगत_map? आगत_map->map (old_gid) : old_gid;
      अचिन्हित outer = v >> 16;
      अचिन्हित new_outer = outer_map[outer];
      अचिन्हित bit_गन्ती = (new_outer == 0) ? 1 : hb_bit_storage (new_outer);
      outer_bit_गन्ती = hb_max (bit_गन्ती, outer_bit_गन्ती);

      output_map.arrayZ[new_gid] = (new_outer << 16) | (inner_maps[outer][v & 0xFFFF]);
    }
  }

  तार्किक remap_after_instantiation (अचर hb_subset_plan_t *plan,
                                  अचर hb_map_t& varidx_map)
  {
    /* recalculate bit_count after remapping */
    outer_bit_गन्ती = 1;
    inner_bit_गन्ती = 1;

    लागि (अचर स्वतः &_ : plan->new_to_old_gid_list)
    {
      hb_codepoint_t new_gid = _.first;
      यदि (unlikely (new_gid >= map_गन्ती)) रोक;

      uint32_t v = output_map.arrayZ[new_gid];
      uint32_t *new_varidx;
      यदि (!varidx_map.has (v, &new_varidx))
        फर्काऊ असत्य;

      output_map.arrayZ[new_gid] = *new_varidx;

      अचिन्हित outer = (*new_varidx) >> 16;
      अचिन्हित bit_गन्ती = (outer == 0) ? 1 : hb_bit_storage (outer);
      outer_bit_गन्ती = hb_max (bit_गन्ती, outer_bit_गन्ती);

      अचिन्हित inner = (*new_varidx) & 0xFFFF;
      bit_गन्ती = (inner == 0) ? 1 : hb_bit_storage (inner);
      inner_bit_गन्ती = hb_max (bit_गन्ती, inner_bit_गन्ती);
    }
    फर्काऊ सत्य;
  }

  अचिन्हित पूर्णाङ्क get_inner_bit_गन्ती () अचर { फर्काऊ inner_bit_गन्ती; }
  अचिन्हित पूर्णाङ्क get_width ()           अचर { फर्काऊ ((outer_bit_गन्ती + inner_bit_गन्ती + 7) / 8); }
  अचिन्हित पूर्णाङ्क get_map_गन्ती ()       अचर { फर्काऊ map_गन्ती; }

  माप_प्रकार get_size () अचर
  {
    यदि (!map_गन्ती) फर्काऊ 0;
    फर्काऊ hb_unsigned_mul_add_saturate (get_width (), map_गन्ती,
					 DeltaSetIndexMap::min_size);
  }

  तार्किक is_identity () अचर
  {
    यदि (!output_map) फर्काऊ सत्य;
    /* An advance map whose entries are the identity over every output
     * glyph is equivalent to implicit glyph-id indexing; drop it.  A
     * truncated map doesn't qualify: glyphs past its end share its last
     * value, not their own ids. */
    यदि (!can_drop_to_implicit || map_गन्ती != full_map_गन्ती) फर्काऊ असत्य;
    लागि (अचिन्हित i = 0; i < map_गन्ती; i++)
      यदि (output_map.arrayZ[i] != i) फर्काऊ असत्य;
    फर्काऊ सत्य;
  }
  hb_array_t<अचर uint32_t> get_output_map () अचर { फर्काऊ output_map.as_array (); }

  रक्षित:
  अचिन्हित पूर्णाङ्क map_गन्ती;
  अचिन्हित पूर्णाङ्क full_map_गन्ती;
  तार्किक can_drop_to_implicit;
  hb_vector_t<अचिन्हित पूर्णाङ्क> max_inners;
  अचिन्हित पूर्णाङ्क outer_bit_गन्ती;
  अचिन्हित पूर्णाङ्क inner_bit_गन्ती;
  hb_vector_t<uint32_t> output_map;
};

संरचना hvarvvar_subset_plan_t
{
  hvarvvar_subset_plan_t() : inner_maps (), index_map_plans () {}
  ~hvarvvar_subset_plan_t() { fini (); }

  शून्य init (अचर hb_array_t<अचर DeltaSetIndexMap *> &index_maps,
	     अचर ItemVariationStore &_var_store,
	     अचर hb_subset_plan_t *plan)
  {
    index_map_plans.resize (index_maps.length);

    var_store = &_var_store;
    inner_sets.resize (var_store->get_sub_table_गन्ती ());
    लागि (अचिन्हित पूर्णाङ्क i = 0; i < inner_sets.length; i++)
      inner_sets[i] = hb_set_create ();
    adv_set = hb_set_create ();

    inner_maps.resize (var_store->get_sub_table_गन्ती ());

    यदि (unlikely (!index_map_plans.length || !inner_sets.length || !inner_maps.length)) फर्काऊ;

    तार्किक retain_adv_map = असत्य;
    index_map_plans[0].init (index_maps[0], outer_map, inner_sets, plan, असत्य);
    यदि (!index_maps[0])
    {
      retain_adv_map = plan->flags & HB_SUBSET_FLAGS_RETAIN_GIDS;
      outer_map.add (0);
      लागि (hb_codepoint_t old_gid : plan->glyphset()->iter())
        inner_sets[0]->add (old_gid);
      hb_set_union (adv_set, inner_sets[0]);
    }

    लागि (अचिन्हित पूर्णाङ्क i = 1; i < index_maps.length; i++)
      index_map_plans[i].init (index_maps[i], outer_map, inner_sets, plan);

    outer_map.sort ();

    यदि (retain_adv_map)
    {
      लागि (अचर स्वतः &_ : plan->new_to_old_gid_list)
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

    लागि (अचिन्हित पूर्णाङ्क i = 1; i < inner_maps.length; i++)
      inner_maps[i].add_set (inner_sets[i]);

    लागि (अचिन्हित पूर्णाङ्क i = 0; i < index_maps.length; i++)
      index_map_plans[i].remap (index_maps[i], outer_map, inner_maps, plan);
  }

  /* remap */
  तार्किक remap_index_map_plans (अचर hb_subset_plan_t *plan,
                              अचर hb_map_t& varidx_map)
  {
    लागि (अचिन्हित i = 0; i < index_map_plans.length; i++)
      यदि (!index_map_plans[i].remap_after_instantiation (plan, varidx_map))
        फर्काऊ असत्य;
    फर्काऊ सत्य;
  }

  शून्य fini ()
  {
    लागि (अचिन्हित पूर्णाङ्क i = 0; i < inner_sets.length; i++)
      hb_set_destroy (inner_sets[i]);
    hb_set_destroy (adv_set);
    inner_maps.fini ();
    index_map_plans.fini ();
  }

  hb_inc_bimap_t outer_map;
  hb_vector_t<hb_inc_bimap_t> inner_maps;
  hb_vector_t<index_map_subset_plan_t> index_map_plans;
  अचर ItemVariationStore *var_store;

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
#define HB_OT_TAG_HVAR HB_TAG('H','V','A','R')
#define HB_OT_TAG_VVAR HB_TAG('V','V','A','R')

संरचना HVARVVAR
{
  स्थिर constexpr hb_tag_t HVARTag = HB_OT_TAG_HVAR;
  स्थिर constexpr hb_tag_t VVARTag = HB_OT_TAG_VVAR;

  तार्किक has_data () अचर { फर्काऊ version.major != 0; }

  तार्किक sanitize (hb_sanitize_context_t *c) अचर
  {
    TRACE_SANITIZE (एतत्);
    return_trace (version.sanitize (c) &&
		  hb_barrier () &&
		  likely (version.major == 1) &&
		  varStore.sanitize (c, एतत्) &&
		  advMap.sanitize (c, एतत्) &&
		  lsbMap.sanitize (c, एतत्) &&
		  rsbMap.sanitize (c, एतत्));
  }

  अचर ItemVariationStore& get_var_store () अचर
  { फर्काऊ एतत्+varStore; }

  शून्य listup_index_maps (hb_vector_t<अचर DeltaSetIndexMap *> &index_maps) अचर
  {
    यदि (advMap) index_maps.push (&(एतत्+advMap));
    अन्यथा index_maps.push (nullptr);

    यदि (lsbMap) index_maps.push (&(एतत्+lsbMap));
    अन्यथा index_maps.push (nullptr);

    यदि (rsbMap) index_maps.push (&(एतत्+rsbMap));
    अन्यथा index_maps.push (nullptr);
  }

  तार्किक serialize_index_maps (hb_serialize_context_t *c,
			     अचर hb_array_t<index_map_subset_plan_t> &im_plans)
  {
    TRACE_SERIALIZE (एतत्);
    यदि (im_plans[index_map_subset_plan_t::ADV_INDEX].is_identity ())
      advMap = 0;
    अन्यथा यदि (unlikely (!advMap.serialize_serialize (c, im_plans[index_map_subset_plan_t::ADV_INDEX])))
      return_trace (असत्य);
    यदि (im_plans[index_map_subset_plan_t::LSB_INDEX].is_identity ())
      lsbMap = 0;
    अन्यथा यदि (unlikely (!lsbMap.serialize_serialize (c, im_plans[index_map_subset_plan_t::LSB_INDEX])))
      return_trace (असत्य);
    यदि (im_plans[index_map_subset_plan_t::RSB_INDEX].is_identity ())
      rsbMap = 0;
    अन्यथा यदि (unlikely (!rsbMap.serialize_serialize (c, im_plans[index_map_subset_plan_t::RSB_INDEX])))
      return_trace (असत्य);

    return_trace (सत्य);
  }

  ढाँचा <typename T>
  तार्किक _subset (hb_subset_context_t *c) अचर
  {
    TRACE_SUBSET (एतत्);
    यदि (c->plan->all_axes_pinned)
      return_trace (असत्य);

    hvarvvar_subset_plan_t	hvar_plan;
    hb_vector_t<अचर DeltaSetIndexMap *>
				index_maps;

    ((T*)एतत्)->listup_index_maps (index_maps);
    hvar_plan.init (index_maps.as_array (), एतत्+varStore, c->plan);

    T *out = c->serializer->allocate_min<T> ();
    यदि (unlikely (!out)) return_trace (असत्य);

    out->version.major = 1;
    out->version.minor = 0;

    यदि (c->plan->normalized_coords)
    {
      item_variations_t item_vars;
      यदि (!item_vars.instantiate (एतत्+varStore, c->plan,
                                  advMap == 0 ? असत्य : सत्य,
                                  असत्य, /* use_no_variation_idx = false */
                                  hvar_plan.inner_maps.as_array ()))
        return_trace (असत्य);

      यदि (!out->varStore.serialize_serialize (c->serializer,
                                              item_vars.has_long_word (),
                                              c->plan->axis_tags,
                                              item_vars.get_region_list (),
                                              item_vars.get_vardata_encodings ()))
        return_trace (असत्य);

      /* if varstore is optimized, remap output_map */
      यदि (advMap)
      {
        यदि (!hvar_plan.remap_index_map_plans (c->plan, item_vars.get_varidx_map ()))
          return_trace (असत्य);
      }
    }
    अन्यथा
    {
      /* avar2 partial instancing: cull unreachable regions. */
      hb_set_t dead_regions;
      यदि (c->plan->has_avar2)
	hvar_plan.var_store->collect_dead_regions (c->plan->axes_old_index_tag_map,
						   c->plan->avar2_reachable_ranges,
						   dead_regions);

      यदि (unlikely (!out->varStore
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
  भिन्न get_advance_delta_unscaled (hb_codepoint_t  glyph,
				    अचर पूर्णाङ्क *coords, अचिन्हित पूर्णाङ्क coord_गन्ती,
				    hb_scalar_cache_t *store_cache = nullptr) अचर
  {
    /* Implicit mapping uses outer index zero and the glyph id as the
     * inner index. An oversized glyph id must not select another table. */
    यदि (!advMap && glyph > 0xFFFFu) फर्काऊ 0.f;
    uint32_t varidx = (एतत्+advMap).map (glyph);
    फर्काऊ (एतत्+varStore).get_delta (varidx,
				      coords, coord_गन्ती,
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
  तार्किक subset (hb_subset_context_t *c) अचर { फर्काऊ HVARVVAR::_subset<HVAR> (c); }
};
संरचना VVAR : HVARVVAR {
  स्थिर constexpr hb_tag_t tableTag = HB_OT_TAG_VVAR;

  तार्किक sanitize (hb_sanitize_context_t *c) अचर
  {
    TRACE_SANITIZE (एतत्);
    return_trace (static_cast<अचर HVARVVAR *> (एतत्)->sanitize (c) &&
		  vorgMap.sanitize (c, एतत्));
  }

  शून्य listup_index_maps (hb_vector_t<अचर DeltaSetIndexMap *> &index_maps) अचर
  {
    HVARVVAR::listup_index_maps (index_maps);
    यदि (vorgMap) index_maps.push (&(एतत्+vorgMap));
    अन्यथा index_maps.push (nullptr);
  }

  तार्किक serialize_index_maps (hb_serialize_context_t *c,
			     अचर hb_array_t<index_map_subset_plan_t> &im_plans)
  {
    TRACE_SERIALIZE (एतत्);
    यदि (unlikely (!HVARVVAR::serialize_index_maps (c, im_plans)))
      return_trace (असत्य);
    यदि (!im_plans[index_map_subset_plan_t::VORG_INDEX].get_map_गन्ती ())
      vorgMap = 0;
    अन्यथा यदि (unlikely (!vorgMap.serialize_serialize (c, im_plans[index_map_subset_plan_t::VORG_INDEX])))
      return_trace (असत्य);

    return_trace (सत्य);
  }

  तार्किक subset (hb_subset_context_t *c) अचर { फर्काऊ HVARVVAR::_subset<VVAR> (c); }

  HB_ALWAYS_INLINE
  भिन्न get_vorg_delta_unscaled (hb_codepoint_t glyph,
				 अचर पूर्णाङ्क *coords, अचिन्हित पूर्णाङ्क coord_गन्ती,
				 hb_scalar_cache_t *store_cache = nullptr) अचर
  {
    यदि (!vorgMap) फर्काऊ 0.f;
    uint32_t varidx = (एतत्+vorgMap).map (glyph);
    फर्काऊ (एतत्+varStore).get_delta (varidx,
				      coords, coord_गन्ती,
				      store_cache);
  }

  रक्षित:
  Offset32To<DeltaSetIndexMap>
		vorgMap;	/* Offset to vertical-origin var-idx mapping. */

  सार्वजनिक:
  DEFINE_SIZE_STATIC (24);
};

} /* namespace OT */


#endif /* HB_OT_VAR_HVAR_TABLE_HH */
