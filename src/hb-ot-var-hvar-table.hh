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


কাঠামো সূচক_map_subset_plan_t
{
  গণনা সূচক_map_সূচক_t {
    ADV_সূচক,
    LSB_সূচক,	/* dual as TSB */
    RSB_সূচক,	/* dual as BSB */
    VORG_সূচক
  };

  শূন্য init (ধ্রুবক Delta_Set_সূচক_Map  *সূচক_map,
	     hb_inc_bimap_t	     &outer_map,
	     hb_vector_t<hb_set_t *> &inner_sets,
	     ধ্রুবক hb_subset_plan_t  *plan,
	     যৌক্তিক bypass_empty = সত্য)
  {
    map_গুনতি = 0;
    full_map_গুনতি = plan->new_to_old_gid_list.দৈর্ঘ্য ?
		     plan->new_to_old_gid_list.লেজ ().first + 1 : 0;
    /* Only the advance mapping may be dropped in favor of implicit
     * glyph-id indexing (for the others a null offset means no deltas),
     * and only when the source was map-less too, preserving its form. */
    can_drop_to_implicit = !bypass_empty && !সূচক_map;
    max_inners.init ();
    output_map.init ();

    যদি (bypass_empty && (!সূচক_map || !সূচক_map->get_map_গুনতি ())) ফেরত;

    unsigned পূর্ণসংখ্যা	last_val = (unsigned পূর্ণসংখ্যা)-1;
    hb_codepoint_t	last_gid = HB_CODEPOINT_INVALID;

    max_inners.resize (inner_sets.দৈর্ঘ্য);
    জন্য (unsigned i = 0; i < inner_sets.দৈর্ঘ্য; i++) max_inners[i] = 0;

    /* Search backwards for a map value different from the last map value */
    auto &new_to_old_gid_list = plan->new_to_old_gid_list;
    unsigned গুনতি = new_to_old_gid_list.দৈর্ঘ্য;
    যদি (!সূচক_map)
    {
      map_গুনতি = new_to_old_gid_list.লেজ ().first + 1;
    }
    অন্যথা
    {
      জন্য (unsigned j = গুনতি; j; j--)
      {
        hb_codepoint_t gid = new_to_old_gid_list.arrayZ[j - 1].first;
        hb_codepoint_t old_gid = new_to_old_gid_list.arrayZ[j - 1].second;
  
        unsigned পূর্ণসংখ্যা v = সূচক_map->map (old_gid);
        যদি (last_gid == HB_CODEPOINT_INVALID)
        {
  	last_val = v;
  	last_gid = gid;
  	চালু;
        }
        যদি (v != last_val)
  	বিরতি;
  
        last_gid = gid;
      }
  
      যদি (unlikely (last_gid == (hb_codepoint_t)-1)) ফেরত;
      map_গুনতি = last_gid + 1;
    }

    জন্য (auto _ : plan->new_to_old_gid_list)
    {
      hb_codepoint_t gid = _.first;
      যদি (gid >= map_গুনতি) বিরতি;

      hb_codepoint_t old_gid = _.second;
      unsigned পূর্ণসংখ্যা v = সূচক_map ? সূচক_map->map (old_gid): old_gid;
      unsigned পূর্ণসংখ্যা outer = v >> 16;
      unsigned পূর্ণসংখ্যা inner = v & 0xFFFF;
      outer_map.add (outer);
      যদি (inner > max_inners[outer]) max_inners[outer] = inner;
      যদি (outer >= inner_sets.দৈর্ঘ্য) ফেরত;
      inner_sets[outer]->add (inner);
    }
  }

  শূন্য fini ()
  {
    max_inners.fini ();
    output_map.fini ();
  }

  শূন্য remap (ধ্রুবক Delta_Set_সূচক_Map *প্রবেশ_map,
	      ধ্রুবক hb_inc_bimap_t &outer_map,
	      ধ্রুবক hb_vector_t<hb_inc_bimap_t> &inner_maps,
	      ধ্রুবক hb_subset_plan_t *plan)
  {
    outer_bit_গুনতি = 1;
    inner_bit_গুনতি = 1;

    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < max_inners.দৈর্ঘ্য; i++)
    {
      যদি (inner_maps[i].get_population () == 0) চালু;
      unsigned পূর্ণসংখ্যা bit_গুনতি = (max_inners[i]==0)? 1: hb_bit_storage (inner_maps[i][max_inners[i]]);
      যদি (bit_গুনতি > inner_bit_গুনতি) inner_bit_গুনতি = bit_গুনতি;
    }

    যদি (unlikely (!output_map.resize (map_গুনতি))) ফেরত;
    জন্য (ধ্রুবক auto &_ : plan->new_to_old_gid_list)
    {
      hb_codepoint_t new_gid = _.first;
      hb_codepoint_t old_gid = _.second;

      যদি (unlikely (new_gid >= map_গুনতি)) বিরতি;

      uint32_t v = প্রবেশ_map? প্রবেশ_map->map (old_gid) : old_gid;
      unsigned outer = v >> 16;
      unsigned new_outer = outer_map[outer];
      unsigned bit_গুনতি = (new_outer == 0) ? 1 : hb_bit_storage (new_outer);
      outer_bit_গুনতি = hb_max (bit_গুনতি, outer_bit_গুনতি);

      output_map.arrayZ[new_gid] = (new_outer << 16) | (inner_maps[outer][v & 0xFFFF]);
    }
  }

  যৌক্তিক remap_after_instantiation (ধ্রুবক hb_subset_plan_t *plan,
                                  ধ্রুবক hb_map_t& varidx_map)
  {
    /* recalculate bit_count after remapping */
    outer_bit_গুনতি = 1;
    inner_bit_গুনতি = 1;

    জন্য (ধ্রুবক auto &_ : plan->new_to_old_gid_list)
    {
      hb_codepoint_t new_gid = _.first;
      যদি (unlikely (new_gid >= map_গুনতি)) বিরতি;

      uint32_t v = output_map.arrayZ[new_gid];
      uint32_t *new_varidx;
      যদি (!varidx_map.has (v, &new_varidx))
        ফেরত মিথ্যা;

      output_map.arrayZ[new_gid] = *new_varidx;

      unsigned outer = (*new_varidx) >> 16;
      unsigned bit_গুনতি = (outer == 0) ? 1 : hb_bit_storage (outer);
      outer_bit_গুনতি = hb_max (bit_গুনতি, outer_bit_গুনতি);

      unsigned inner = (*new_varidx) & 0xFFFF;
      bit_গুনতি = (inner == 0) ? 1 : hb_bit_storage (inner);
      inner_bit_গুনতি = hb_max (bit_গুনতি, inner_bit_গুনতি);
    }
    ফেরত সত্য;
  }

  unsigned পূর্ণসংখ্যা get_inner_bit_গুনতি () ধ্রুবক { ফেরত inner_bit_গুনতি; }
  unsigned পূর্ণসংখ্যা get_width ()           ধ্রুবক { ফেরত ((outer_bit_গুনতি + inner_bit_গুনতি + 7) / 8); }
  unsigned পূর্ণসংখ্যা get_map_গুনতি ()       ধ্রুবক { ফেরত map_গুনতি; }

  size_t get_আকার () ধ্রুবক
  {
    যদি (!map_গুনতি) ফেরত 0;
    ফেরত hb_unsigned_mul_add_saturate (get_width (), map_গুনতি,
					 Delta_Set_সূচক_Map::min_আকার);
  }

  যৌক্তিক is_identity () ধ্রুবক
  {
    যদি (!output_map) ফেরত সত্য;
    /* An advance map whose entries are the identity over every output
     * glyph is equivalent to implicit glyph-id indexing; drop it.  A
     * truncated map doesn't qualify: glyphs past its end share its last
     * value, not their own ids. */
    যদি (!can_drop_to_implicit || map_গুনতি != full_map_গুনতি) ফেরত মিথ্যা;
    জন্য (unsigned i = 0; i < map_গুনতি; i++)
      যদি (output_map.arrayZ[i] != i) ফেরত মিথ্যা;
    ফেরত সত্য;
  }
  hb_array_t<ধ্রুবক uint32_t> get_output_map () ধ্রুবক { ফেরত output_map.as_array (); }

  সুরক্ষিত:
  unsigned পূর্ণসংখ্যা map_গুনতি;
  unsigned পূর্ণসংখ্যা full_map_গুনতি;
  যৌক্তিক can_drop_to_implicit;
  hb_vector_t<unsigned পূর্ণসংখ্যা> max_inners;
  unsigned পূর্ণসংখ্যা outer_bit_গুনতি;
  unsigned পূর্ণসংখ্যা inner_bit_গুনতি;
  hb_vector_t<uint32_t> output_map;
};

কাঠামো hvarvvar_subset_plan_t
{
  hvarvvar_subset_plan_t() : inner_maps (), সূচক_map_plans () {}
  ~hvarvvar_subset_plan_t() { fini (); }

  শূন্য init (ধ্রুবক hb_array_t<ধ্রুবক Delta_Set_সূচক_Map *> &সূচক_maps,
	     ধ্রুবক ItemVariationStore &_var_store,
	     ধ্রুবক hb_subset_plan_t *plan)
  {
    সূচক_map_plans.resize (সূচক_maps.দৈর্ঘ্য);

    var_store = &_var_store;
    inner_sets.resize (var_store->get_sub_table_গুনতি ());
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < inner_sets.দৈর্ঘ্য; i++)
      inner_sets[i] = hb_set_create ();
    adv_set = hb_set_create ();

    inner_maps.resize (var_store->get_sub_table_গুনতি ());

    যদি (unlikely (!সূচক_map_plans.দৈর্ঘ্য || !inner_sets.দৈর্ঘ্য || !inner_maps.দৈর্ঘ্য)) ফেরত;

    যৌক্তিক retain_adv_map = মিথ্যা;
    সূচক_map_plans[0].init (সূচক_maps[0], outer_map, inner_sets, plan, মিথ্যা);
    যদি (!সূচক_maps[0])
    {
      retain_adv_map = plan->flags & HB_SUBSET_FLAGS_RETAIN_GIDS;
      outer_map.add (0);
      জন্য (hb_codepoint_t old_gid : plan->glyphset()->iter())
        inner_sets[0]->add (old_gid);
      hb_set_union (adv_set, inner_sets[0]);
    }

    জন্য (unsigned পূর্ণসংখ্যা i = 1; i < সূচক_maps.দৈর্ঘ্য; i++)
      সূচক_map_plans[i].init (সূচক_maps[i], outer_map, inner_sets, plan);

    outer_map.sort ();

    যদি (retain_adv_map)
    {
      জন্য (ধ্রুবক auto &_ : plan->new_to_old_gid_list)
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

    জন্য (unsigned পূর্ণসংখ্যা i = 1; i < inner_maps.দৈর্ঘ্য; i++)
      inner_maps[i].add_set (inner_sets[i]);

    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < সূচক_maps.দৈর্ঘ্য; i++)
      সূচক_map_plans[i].remap (সূচক_maps[i], outer_map, inner_maps, plan);
  }

  /* remap */
  যৌক্তিক remap_সূচক_map_plans (ধ্রুবক hb_subset_plan_t *plan,
                              ধ্রুবক hb_map_t& varidx_map)
  {
    জন্য (unsigned i = 0; i < সূচক_map_plans.দৈর্ঘ্য; i++)
      যদি (!সূচক_map_plans[i].remap_after_instantiation (plan, varidx_map))
        ফেরত মিথ্যা;
    ফেরত সত্য;
  }

  শূন্য fini ()
  {
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < inner_sets.দৈর্ঘ্য; i++)
      hb_set_destroy (inner_sets[i]);
    hb_set_destroy (adv_set);
    inner_maps.fini ();
    সূচক_map_plans.fini ();
  }

  hb_inc_bimap_t outer_map;
  hb_vector_t<hb_inc_bimap_t> inner_maps;
  hb_vector_t<সূচক_map_subset_plan_t> সূচক_map_plans;
  ধ্রুবক ItemVariationStore *var_store;

  সুরক্ষিত:
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

কাঠামো HVARVVAR
{
  স্থির constexpr hb_tag_t HVARTag = HB_OT_TAG_HVAR;
  স্থির constexpr hb_tag_t VVARTag = HB_OT_TAG_VVAR;

  যৌক্তিক has_তথ্য () ধ্রুবক { ফেরত version.major != 0; }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (version.sanitize (c) &&
		  hb_barrier () &&
		  likely (version.major == 1) &&
		  varStore.sanitize (c, এই) &&
		  advMap.sanitize (c, এই) &&
		  lsbMap.sanitize (c, এই) &&
		  rsbMap.sanitize (c, এই));
  }

  ধ্রুবক ItemVariationStore& get_var_store () ধ্রুবক
  { ফেরত এই+varStore; }

  শূন্য listup_সূচক_maps (hb_vector_t<ধ্রুবক Delta_Set_সূচক_Map *> &সূচক_maps) ধ্রুবক
  {
    যদি (advMap) সূচক_maps.push (&(এই+advMap));
    অন্যথা সূচক_maps.push (nullptr);

    যদি (lsbMap) সূচক_maps.push (&(এই+lsbMap));
    অন্যথা সূচক_maps.push (nullptr);

    যদি (rsbMap) সূচক_maps.push (&(এই+rsbMap));
    অন্যথা সূচক_maps.push (nullptr);
  }

  যৌক্তিক serialize_সূচক_maps (hb_serialize_context_t *c,
			     ধ্রুবক hb_array_t<সূচক_map_subset_plan_t> &im_plans)
  {
    TRACE_SERIALIZE (এই);
    যদি (im_plans[সূচক_map_subset_plan_t::ADV_সূচক].is_identity ())
      advMap = 0;
    অন্যথা যদি (unlikely (!advMap.serialize_serialize (c, im_plans[সূচক_map_subset_plan_t::ADV_সূচক])))
      return_trace (মিথ্যা);
    যদি (im_plans[সূচক_map_subset_plan_t::LSB_সূচক].is_identity ())
      lsbMap = 0;
    অন্যথা যদি (unlikely (!lsbMap.serialize_serialize (c, im_plans[সূচক_map_subset_plan_t::LSB_সূচক])))
      return_trace (মিথ্যা);
    যদি (im_plans[সূচক_map_subset_plan_t::RSB_সূচক].is_identity ())
      rsbMap = 0;
    অন্যথা যদি (unlikely (!rsbMap.serialize_serialize (c, im_plans[সূচক_map_subset_plan_t::RSB_সূচক])))
      return_trace (মিথ্যা);

    return_trace (সত্য);
  }

  template <typename T>
  যৌক্তিক _subset (hb_subset_context_t *c) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    যদি (c->plan->all_axes_pinned)
      return_trace (মিথ্যা);

    hvarvvar_subset_plan_t	hvar_plan;
    hb_vector_t<ধ্রুবক Delta_Set_সূচক_Map *>
				সূচক_maps;

    ((T*)এই)->listup_সূচক_maps (সূচক_maps);
    hvar_plan.init (সূচক_maps.as_array (), এই+varStore, c->plan);

    T *out = c->serializer->allocate_min<T> ();
    যদি (unlikely (!out)) return_trace (মিথ্যা);

    out->version.major = 1;
    out->version.minor = 0;

    যদি (c->plan->normalized_coords)
    {
      item_variations_t item_vars;
      যদি (!item_vars.instantiate (এই+varStore, c->plan,
                                  advMap == 0 ? মিথ্যা : সত্য,
                                  মিথ্যা, /* use_no_variation_idx = false */
                                  hvar_plan.inner_maps.as_array ()))
        return_trace (মিথ্যা);

      যদি (!out->varStore.serialize_serialize (c->serializer,
                                              item_vars.has_long_word (),
                                              c->plan->axis_tags,
                                              item_vars.get_region_list (),
                                              item_vars.get_vardata_encodings ()))
        return_trace (মিথ্যা);

      /* if varstore is optimized, remap output_map */
      যদি (advMap)
      {
        যদি (!hvar_plan.remap_সূচক_map_plans (c->plan, item_vars.get_varidx_map ()))
          return_trace (মিথ্যা);
      }
    }
    অন্যথা
    {
      /* avar2 partial instancing: cull unreachable regions. */
      hb_set_t dead_regions;
      যদি (c->plan->has_avar2)
	hvar_plan.var_store->collect_dead_regions (c->plan->axes_old_সূচক_tag_map,
						   c->plan->avar2_reachable_ranges,
						   dead_regions);

      যদি (unlikely (!out->varStore
		    .serialize_serialize (c->serializer,
					  hvar_plan.var_store,
					  hvar_plan.inner_maps.as_array (),
					  dead_regions.get_population () ? &dead_regions : nullptr)))
      return_trace (মিথ্যা);
    }

    return_trace (out->T::serialize_সূচক_maps (c->serializer,
						hvar_plan.সূচক_map_plans.as_array ()));
  }

  HB_ALWAYS_INLINE
  দশমিক get_advance_delta_unscaled (hb_codepoint_t  glyph,
				    ধ্রুবক পূর্ণসংখ্যা *coords, unsigned পূর্ণসংখ্যা coord_গুনতি,
				    hb_scalar_cache_t *store_cache = nullptr) ধ্রুবক
  {
    /* Implicit mapping uses outer index zero and the glyph id as the
     * inner index. An oversized glyph id must not select another table. */
    যদি (!advMap && glyph > 0xFFFFu) ফেরত 0.f;
    uint32_t varidx = (এই+advMap).map (glyph);
    ফেরত (এই+varStore).get_delta (varidx,
				      coords, coord_গুনতি,
				      store_cache);
  }

  সর্বজনীন:
  FixedVersion<>version;	/* Version of the metrics variation table
				 * initially set to 0x00010000u */
  Offset32To<ItemVariationStore>
		varStore;	/* Offset to item variation store table. */
  Offset32To<Delta_Set_সূচক_Map>
		advMap;		/* Offset to advance var-idx mapping. */
  Offset32To<Delta_Set_সূচক_Map>
		lsbMap;		/* Offset to lsb/tsb var-idx mapping. */
  Offset32To<Delta_Set_সূচক_Map>
		rsbMap;		/* Offset to rsb/bsb var-idx mapping. */

  সর্বজনীন:
  DEFINE_আকার_STATIC (20);
};

কাঠামো HVAR : HVARVVAR {
  স্থির constexpr hb_tag_t tableTag = HB_OT_TAG_HVAR;
  যৌক্তিক subset (hb_subset_context_t *c) ধ্রুবক { ফেরত HVARVVAR::_subset<HVAR> (c); }
};
কাঠামো VVAR : HVARVVAR {
  স্থির constexpr hb_tag_t tableTag = HB_OT_TAG_VVAR;

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (static_cast<ধ্রুবক HVARVVAR *> (এই)->sanitize (c) &&
		  vorgMap.sanitize (c, এই));
  }

  শূন্য listup_সূচক_maps (hb_vector_t<ধ্রুবক Delta_Set_সূচক_Map *> &সূচক_maps) ধ্রুবক
  {
    HVARVVAR::listup_সূচক_maps (সূচক_maps);
    যদি (vorgMap) সূচক_maps.push (&(এই+vorgMap));
    অন্যথা সূচক_maps.push (nullptr);
  }

  যৌক্তিক serialize_সূচক_maps (hb_serialize_context_t *c,
			     ধ্রুবক hb_array_t<সূচক_map_subset_plan_t> &im_plans)
  {
    TRACE_SERIALIZE (এই);
    যদি (unlikely (!HVARVVAR::serialize_সূচক_maps (c, im_plans)))
      return_trace (মিথ্যা);
    যদি (!im_plans[সূচক_map_subset_plan_t::VORG_সূচক].get_map_গুনতি ())
      vorgMap = 0;
    অন্যথা যদি (unlikely (!vorgMap.serialize_serialize (c, im_plans[সূচক_map_subset_plan_t::VORG_সূচক])))
      return_trace (মিথ্যা);

    return_trace (সত্য);
  }

  যৌক্তিক subset (hb_subset_context_t *c) ধ্রুবক { ফেরত HVARVVAR::_subset<VVAR> (c); }

  HB_ALWAYS_INLINE
  দশমিক get_vorg_delta_unscaled (hb_codepoint_t glyph,
				 ধ্রুবক পূর্ণসংখ্যা *coords, unsigned পূর্ণসংখ্যা coord_গুনতি,
				 hb_scalar_cache_t *store_cache = nullptr) ধ্রুবক
  {
    যদি (!vorgMap) ফেরত 0.f;
    uint32_t varidx = (এই+vorgMap).map (glyph);
    ফেরত (এই+varStore).get_delta (varidx,
				      coords, coord_গুনতি,
				      store_cache);
  }

  সুরক্ষিত:
  Offset32To<Delta_Set_সূচক_Map>
		vorgMap;	/* Offset to vertical-origin var-idx mapping. */

  সর্বজনীন:
  DEFINE_আকার_STATIC (24);
};

} /* namespace OT */


#endif /* HB_OT_VAR_HVAR_TABLE_HH */
