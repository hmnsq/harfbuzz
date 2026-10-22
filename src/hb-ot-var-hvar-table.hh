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

#অন্তর্ভাব "hb-ot-layout-common.hh"
#অন্তর্ভাব "hb-ot-var-common.hh"

নামাকাশ OT {


সংরচনা index_map_subset_plan_t
{
  গণনা index_map_index_t {
    ADV_INDEX,
    LSB_INDEX,	/* dual as TSB */
    RSB_INDEX,	/* dual as BSB */
    VORG_INDEX
  };

  শূন্য init (ধ্রুব DeltaSetIndexMap  *index_map,
	     hb_inc_bimap_t	     &outer_map,
	     hb_vector_t<hb_set_t *> &inner_sets,
	     ধ্রুব hb_subset_plan_t  *plan,
	     তার্কিক bypass_empty = সত্য)
  {
    map_গুনতি = 0;
    full_map_গুনতি = plan->new_to_old_gid_list.length ?
		     plan->new_to_old_gid_list.tail ().first + 1 : 0;
    /* Only the advance mapping may be dropped in favor of implicit
     * glyph-id indexing (for the others a null offset means no deltas),
     * and only when the source was map-less too, preserving its form. */
    can_drop_to_implicit = !bypass_empty && !index_map;
    max_inners.init ();
    output_map.init ();

    যদি (bypass_empty && (!index_map || !index_map->get_map_গুনতি ())) প্রত্যাগম;

    unsigned পূর্ণাঙ্ক	last_val = (unsigned পূর্ণাঙ্ক)-1;
    hb_codepoint_t	last_gid = HB_CODEPOINT_INVALID;

    max_inners.resize (inner_sets.length);
    প্রতি (unsigned i = 0; i < inner_sets.length; i++) max_inners[i] = 0;

    /* Search backwards for a map value different from the last map value */
    auto &new_to_old_gid_list = plan->new_to_old_gid_list;
    unsigned গুনতি = new_to_old_gid_list.length;
    যদি (!index_map)
    {
      map_গুনতি = new_to_old_gid_list.tail ().first + 1;
    }
    অন্যথা
    {
      প্রতি (unsigned j = গুনতি; j; j--)
      {
        hb_codepoint_t gid = new_to_old_gid_list.arrayZ[j - 1].first;
        hb_codepoint_t old_gid = new_to_old_gid_list.arrayZ[j - 1].second;
  
        unsigned পূর্ণাঙ্ক v = index_map->map (old_gid);
        যদি (last_gid == HB_CODEPOINT_INVALID)
        {
  	last_val = v;
  	last_gid = gid;
  	অনুবর্ত;
        }
        যদি (v != last_val)
  	ভঙ্গ;
  
        last_gid = gid;
      }
  
      যদি (unlikely (last_gid == (hb_codepoint_t)-1)) প্রত্যাগম;
      map_গুনতি = last_gid + 1;
    }

    প্রতি (auto _ : plan->new_to_old_gid_list)
    {
      hb_codepoint_t gid = _.first;
      যদি (gid >= map_গুনতি) ভঙ্গ;

      hb_codepoint_t old_gid = _.second;
      unsigned পূর্ণাঙ্ক v = index_map ? index_map->map (old_gid): old_gid;
      unsigned পূর্ণাঙ্ক outer = v >> 16;
      unsigned পূর্ণাঙ্ক inner = v & 0xFFFF;
      outer_map.add (outer);
      যদি (inner > max_inners[outer]) max_inners[outer] = inner;
      যদি (outer >= inner_sets.length) প্রত্যাগম;
      inner_sets[outer]->add (inner);
    }
  }

  শূন্য fini ()
  {
    max_inners.fini ();
    output_map.fini ();
  }

  শূন্য remap (ধ্রুব DeltaSetIndexMap *প্রবেশ_map,
	      ধ্রুব hb_inc_bimap_t &outer_map,
	      ধ্রুব hb_vector_t<hb_inc_bimap_t> &inner_maps,
	      ধ্রুব hb_subset_plan_t *plan)
  {
    outer_bit_গুনতি = 1;
    inner_bit_গুনতি = 1;

    প্রতি (unsigned পূর্ণাঙ্ক i = 0; i < max_inners.length; i++)
    {
      যদি (inner_maps[i].get_population () == 0) অনুবর্ত;
      unsigned পূর্ণাঙ্ক bit_গুনতি = (max_inners[i]==0)? 1: hb_bit_storage (inner_maps[i][max_inners[i]]);
      যদি (bit_গুনতি > inner_bit_গুনতি) inner_bit_গুনতি = bit_গুনতি;
    }

    যদি (unlikely (!output_map.resize (map_গুনতি))) প্রত্যাগম;
    প্রতি (ধ্রুব auto &_ : plan->new_to_old_gid_list)
    {
      hb_codepoint_t new_gid = _.first;
      hb_codepoint_t old_gid = _.second;

      যদি (unlikely (new_gid >= map_গুনতি)) ভঙ্গ;

      uint32_t v = প্রবেশ_map? প্রবেশ_map->map (old_gid) : old_gid;
      unsigned outer = v >> 16;
      unsigned new_outer = outer_map[outer];
      unsigned bit_গুনতি = (new_outer == 0) ? 1 : hb_bit_storage (new_outer);
      outer_bit_গুনতি = hb_max (bit_গুনতি, outer_bit_গুনতি);

      output_map.arrayZ[new_gid] = (new_outer << 16) | (inner_maps[outer][v & 0xFFFF]);
    }
  }

  তার্কিক remap_after_instantiation (ধ্রুব hb_subset_plan_t *plan,
                                  ধ্রুব hb_map_t& varidx_map)
  {
    /* recalculate bit_count after remapping */
    outer_bit_গুনতি = 1;
    inner_bit_গুনতি = 1;

    প্রতি (ধ্রুব auto &_ : plan->new_to_old_gid_list)
    {
      hb_codepoint_t new_gid = _.first;
      যদি (unlikely (new_gid >= map_গুনতি)) ভঙ্গ;

      uint32_t v = output_map.arrayZ[new_gid];
      uint32_t *new_varidx;
      যদি (!varidx_map.has (v, &new_varidx))
        প্রত্যাগম অসত্য;

      output_map.arrayZ[new_gid] = *new_varidx;

      unsigned outer = (*new_varidx) >> 16;
      unsigned bit_গুনতি = (outer == 0) ? 1 : hb_bit_storage (outer);
      outer_bit_গুনতি = hb_max (bit_গুনতি, outer_bit_গুনতি);

      unsigned inner = (*new_varidx) & 0xFFFF;
      bit_গুনতি = (inner == 0) ? 1 : hb_bit_storage (inner);
      inner_bit_গুনতি = hb_max (bit_গুনতি, inner_bit_গুনতি);
    }
    প্রত্যাগম সত্য;
  }

  unsigned পূর্ণাঙ্ক get_inner_bit_গুনতি () ধ্রুব { প্রত্যাগম inner_bit_গুনতি; }
  unsigned পূর্ণাঙ্ক get_width ()           ধ্রুব { প্রত্যাগম ((outer_bit_গুনতি + inner_bit_গুনতি + 7) / 8); }
  unsigned পূর্ণাঙ্ক get_map_গুনতি ()       ধ্রুব { প্রত্যাগম map_গুনতি; }

  size_t get_size () ধ্রুব
  {
    যদি (!map_গুনতি) প্রত্যাগম 0;
    প্রত্যাগম hb_unsigned_mul_add_saturate (get_width (), map_গুনতি,
					 DeltaSetIndexMap::min_size);
  }

  তার্কিক is_identity () ধ্রুব
  {
    যদি (!output_map) প্রত্যাগম সত্য;
    /* An advance map whose entries are the identity over every output
     * glyph is equivalent to implicit glyph-id indexing; drop it.  A
     * truncated map doesn't qualify: glyphs past its end share its last
     * value, not their own ids. */
    যদি (!can_drop_to_implicit || map_গুনতি != full_map_গুনতি) প্রত্যাগম অসত্য;
    প্রতি (unsigned i = 0; i < map_গুনতি; i++)
      যদি (output_map.arrayZ[i] != i) প্রত্যাগম অসত্য;
    প্রত্যাগম সত্য;
  }
  hb_array_t<ধ্রুব uint32_t> get_output_map () ধ্রুব { প্রত্যাগম output_map.as_array (); }

  রক্ষিত:
  unsigned পূর্ণাঙ্ক map_গুনতি;
  unsigned পূর্ণাঙ্ক full_map_গুনতি;
  তার্কিক can_drop_to_implicit;
  hb_vector_t<unsigned পূর্ণাঙ্ক> max_inners;
  unsigned পূর্ণাঙ্ক outer_bit_গুনতি;
  unsigned পূর্ণাঙ্ক inner_bit_গুনতি;
  hb_vector_t<uint32_t> output_map;
};

সংরচনা hvarvvar_subset_plan_t
{
  hvarvvar_subset_plan_t() : inner_maps (), index_map_plans () {}
  ~hvarvvar_subset_plan_t() { fini (); }

  শূন্য init (ধ্রুব hb_array_t<ধ্রুব DeltaSetIndexMap *> &index_maps,
	     ধ্রুব ItemVariationStore &_var_store,
	     ধ্রুব hb_subset_plan_t *plan)
  {
    index_map_plans.resize (index_maps.length);

    var_store = &_var_store;
    inner_sets.resize (var_store->get_sub_table_গুনতি ());
    প্রতি (unsigned পূর্ণাঙ্ক i = 0; i < inner_sets.length; i++)
      inner_sets[i] = hb_set_create ();
    adv_set = hb_set_create ();

    inner_maps.resize (var_store->get_sub_table_গুনতি ());

    যদি (unlikely (!index_map_plans.length || !inner_sets.length || !inner_maps.length)) প্রত্যাগম;

    তার্কিক retain_adv_map = অসত্য;
    index_map_plans[0].init (index_maps[0], outer_map, inner_sets, plan, অসত্য);
    যদি (!index_maps[0])
    {
      retain_adv_map = plan->flags & HB_SUBSET_FLAGS_RETAIN_GIDS;
      outer_map.add (0);
      প্রতি (hb_codepoint_t old_gid : plan->glyphset()->iter())
        inner_sets[0]->add (old_gid);
      hb_set_union (adv_set, inner_sets[0]);
    }

    প্রতি (unsigned পূর্ণাঙ্ক i = 1; i < index_maps.length; i++)
      index_map_plans[i].init (index_maps[i], outer_map, inner_sets, plan);

    outer_map.sort ();

    যদি (retain_adv_map)
    {
      প্রতি (ধ্রুব auto &_ : plan->new_to_old_gid_list)
      {
        hb_codepoint_t old_gid = _.second;
	inner_maps[0].add (old_gid);
      }
    }
    অন্যথা
    {
      inner_maps[0].add_set (adv_set);
      hb_set_subtract (inner_sets[0], adv_set);
      inner_maps[0].add_set (inner_sets[0]);
    }

    প্রতি (unsigned পূর্ণাঙ্ক i = 1; i < inner_maps.length; i++)
      inner_maps[i].add_set (inner_sets[i]);

    প্রতি (unsigned পূর্ণাঙ্ক i = 0; i < index_maps.length; i++)
      index_map_plans[i].remap (index_maps[i], outer_map, inner_maps, plan);
  }

  /* remap */
  তার্কিক remap_index_map_plans (ধ্রুব hb_subset_plan_t *plan,
                              ধ্রুব hb_map_t& varidx_map)
  {
    প্রতি (unsigned i = 0; i < index_map_plans.length; i++)
      যদি (!index_map_plans[i].remap_after_instantiation (plan, varidx_map))
        প্রত্যাগম অসত্য;
    প্রত্যাগম সত্য;
  }

  শূন্য fini ()
  {
    প্রতি (unsigned পূর্ণাঙ্ক i = 0; i < inner_sets.length; i++)
      hb_set_destroy (inner_sets[i]);
    hb_set_destroy (adv_set);
    inner_maps.fini ();
    index_map_plans.fini ();
  }

  hb_inc_bimap_t outer_map;
  hb_vector_t<hb_inc_bimap_t> inner_maps;
  hb_vector_t<index_map_subset_plan_t> index_map_plans;
  ধ্রুব ItemVariationStore *var_store;

  রক্ষিত:
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

সংরচনা HVARVVAR
{
  স্থির constexpr hb_tag_t HVARTag = HB_OT_TAG_HVAR;
  স্থির constexpr hb_tag_t VVARTag = HB_OT_TAG_VVAR;

  তার্কিক has_data () ধ্রুব { প্রত্যাগম version.major != 0; }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    return_trace (version.sanitize (c) &&
		  hb_barrier () &&
		  likely (version.major == 1) &&
		  varStore.sanitize (c, এতত্) &&
		  advMap.sanitize (c, এতত্) &&
		  lsbMap.sanitize (c, এতত্) &&
		  rsbMap.sanitize (c, এতত্));
  }

  ধ্রুব ItemVariationStore& get_var_store () ধ্রুব
  { প্রত্যাগম এতত্+varStore; }

  শূন্য listup_index_maps (hb_vector_t<ধ্রুব DeltaSetIndexMap *> &index_maps) ধ্রুব
  {
    যদি (advMap) index_maps.push (&(এতত্+advMap));
    অন্যথা index_maps.push (nullptr);

    যদি (lsbMap) index_maps.push (&(এতত্+lsbMap));
    অন্যথা index_maps.push (nullptr);

    যদি (rsbMap) index_maps.push (&(এতত্+rsbMap));
    অন্যথা index_maps.push (nullptr);
  }

  তার্কিক serialize_index_maps (hb_serialize_context_t *c,
			     ধ্রুব hb_array_t<index_map_subset_plan_t> &im_plans)
  {
    TRACE_SERIALIZE (এতত্);
    যদি (im_plans[index_map_subset_plan_t::ADV_INDEX].is_identity ())
      advMap = 0;
    অন্যথা যদি (unlikely (!advMap.serialize_serialize (c, im_plans[index_map_subset_plan_t::ADV_INDEX])))
      return_trace (অসত্য);
    যদি (im_plans[index_map_subset_plan_t::LSB_INDEX].is_identity ())
      lsbMap = 0;
    অন্যথা যদি (unlikely (!lsbMap.serialize_serialize (c, im_plans[index_map_subset_plan_t::LSB_INDEX])))
      return_trace (অসত্য);
    যদি (im_plans[index_map_subset_plan_t::RSB_INDEX].is_identity ())
      rsbMap = 0;
    অন্যথা যদি (unlikely (!rsbMap.serialize_serialize (c, im_plans[index_map_subset_plan_t::RSB_INDEX])))
      return_trace (অসত্য);

    return_trace (সত্য);
  }

  template <typename T>
  তার্কিক _subset (hb_subset_context_t *c) ধ্রুব
  {
    TRACE_SUBSET (এতত্);
    যদি (c->plan->all_axes_pinned)
      return_trace (অসত্য);

    hvarvvar_subset_plan_t	hvar_plan;
    hb_vector_t<ধ্রুব DeltaSetIndexMap *>
				index_maps;

    ((T*)এতত্)->listup_index_maps (index_maps);
    hvar_plan.init (index_maps.as_array (), এতত্+varStore, c->plan);

    T *out = c->serializer->allocate_min<T> ();
    যদি (unlikely (!out)) return_trace (অসত্য);

    out->version.major = 1;
    out->version.minor = 0;

    যদি (c->plan->normalized_coords)
    {
      item_variations_t item_vars;
      যদি (!item_vars.instantiate (এতত্+varStore, c->plan,
                                  advMap == 0 ? অসত্য : সত্য,
                                  অসত্য, /* use_no_variation_idx = false */
                                  hvar_plan.inner_maps.as_array ()))
        return_trace (অসত্য);

      যদি (!out->varStore.serialize_serialize (c->serializer,
                                              item_vars.has_long_word (),
                                              c->plan->axis_tags,
                                              item_vars.get_region_list (),
                                              item_vars.get_vardata_encodings ()))
        return_trace (অসত্য);

      /* if varstore is optimized, remap output_map */
      যদি (advMap)
      {
        যদি (!hvar_plan.remap_index_map_plans (c->plan, item_vars.get_varidx_map ()))
          return_trace (অসত্য);
      }
    }
    অন্যথা
    {
      /* avar2 partial instancing: cull unreachable regions. */
      hb_set_t dead_regions;
      যদি (c->plan->has_avar2)
	hvar_plan.var_store->collect_dead_regions (c->plan->axes_old_index_tag_map,
						   c->plan->avar2_reachable_ranges,
						   dead_regions);

      যদি (unlikely (!out->varStore
		    .serialize_serialize (c->serializer,
					  hvar_plan.var_store,
					  hvar_plan.inner_maps.as_array (),
					  dead_regions.get_population () ? &dead_regions : nullptr)))
      return_trace (অসত্য);
    }

    return_trace (out->T::serialize_index_maps (c->serializer,
						hvar_plan.index_map_plans.as_array ()));
  }

  HB_ALWAYS_INLINE
  ভিন্ন get_advance_delta_unscaled (hb_codepoint_t  glyph,
				    ধ্রুব পূর্ণাঙ্ক *coords, unsigned পূর্ণাঙ্ক coord_গুনতি,
				    hb_scalar_cache_t *store_cache = nullptr) ধ্রুব
  {
    /* Implicit mapping uses outer index zero and the glyph id as the
     * inner index. An oversized glyph id must not select another table. */
    যদি (!advMap && glyph > 0xFFFFu) প্রত্যাগম 0.f;
    uint32_t varidx = (এতত্+advMap).map (glyph);
    প্রত্যাগম (এতত্+varStore).get_delta (varidx,
				      coords, coord_গুনতি,
				      store_cache);
  }

  সার্বজনিক:
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

  সার্বজনিক:
  DEFINE_SIZE_STATIC (20);
};

সংরচনা HVAR : HVARVVAR {
  স্থির constexpr hb_tag_t tableTag = HB_OT_TAG_HVAR;
  তার্কিক subset (hb_subset_context_t *c) ধ্রুব { প্রত্যাগম HVARVVAR::_subset<HVAR> (c); }
};
সংরচনা VVAR : HVARVVAR {
  স্থির constexpr hb_tag_t tableTag = HB_OT_TAG_VVAR;

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    return_trace (static_cast<ধ্রুব HVARVVAR *> (এতত্)->sanitize (c) &&
		  vorgMap.sanitize (c, এতত্));
  }

  শূন্য listup_index_maps (hb_vector_t<ধ্রুব DeltaSetIndexMap *> &index_maps) ধ্রুব
  {
    HVARVVAR::listup_index_maps (index_maps);
    যদি (vorgMap) index_maps.push (&(এতত্+vorgMap));
    অন্যথা index_maps.push (nullptr);
  }

  তার্কিক serialize_index_maps (hb_serialize_context_t *c,
			     ধ্রুব hb_array_t<index_map_subset_plan_t> &im_plans)
  {
    TRACE_SERIALIZE (এতত্);
    যদি (unlikely (!HVARVVAR::serialize_index_maps (c, im_plans)))
      return_trace (অসত্য);
    যদি (!im_plans[index_map_subset_plan_t::VORG_INDEX].get_map_গুনতি ())
      vorgMap = 0;
    অন্যথা যদি (unlikely (!vorgMap.serialize_serialize (c, im_plans[index_map_subset_plan_t::VORG_INDEX])))
      return_trace (অসত্য);

    return_trace (সত্য);
  }

  তার্কিক subset (hb_subset_context_t *c) ধ্রুব { প্রত্যাগম HVARVVAR::_subset<VVAR> (c); }

  HB_ALWAYS_INLINE
  ভিন্ন get_vorg_delta_unscaled (hb_codepoint_t glyph,
				 ধ্রুব পূর্ণাঙ্ক *coords, unsigned পূর্ণাঙ্ক coord_গুনতি,
				 hb_scalar_cache_t *store_cache = nullptr) ধ্রুব
  {
    যদি (!vorgMap) প্রত্যাগম 0.f;
    uint32_t varidx = (এতত্+vorgMap).map (glyph);
    প্রত্যাগম (এতত্+varStore).get_delta (varidx,
				      coords, coord_গুনতি,
				      store_cache);
  }

  রক্ষিত:
  Offset32To<DeltaSetIndexMap>
		vorgMap;	/* Offset to vertical-origin var-idx mapping. */

  সার্বজনিক:
  DEFINE_SIZE_STATIC (24);
};

} /* namespace OT */


#endif /* HB_OT_VAR_HVAR_TABLE_HH */
