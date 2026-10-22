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

#அந்தர்பாவ "hb-ot-layout-common.hh"
#அந்தர்பாவ "hb-ot-var-common.hh"

நாமாகாஶ OT {


கட்டமைப்பு index_map_subset_plan_t
{
  எண்ணிக்கை index_map_index_t {
    ADV_INDEX,
    LSB_INDEX,	/* dual as TSB */
    RSB_INDEX,	/* dual as BSB */
    VORG_INDEX
  };

  வெறுமை init (மாறிலி DeltaSetIndexMap  *index_map,
	     hb_inc_bimap_t	     &outer_map,
	     hb_vector_t<hb_set_t *> &inner_sets,
	     மாறிலி hb_subset_plan_t  *plan,
	     தருக்க bypass_empty = உண்மை)
  {
    map_மொத்தம் = 0;
    full_map_மொத்தம் = plan->new_to_old_gid_list.length ?
		     plan->new_to_old_gid_list.tail ().first + 1 : 0;
    /* Only the advance mapping may be dropped in favor of implicit
     * glyph-id indexing (for the others a null offset means no deltas),
     * and only when the source was map-less too, preserving its form. */
    can_drop_to_implicit = !bypass_empty && !index_map;
    max_inners.init ();
    output_map.init ();

    எனில் (bypass_empty && (!index_map || !index_map->get_map_மொத்தம் ())) திரும்பு;

    unsigned முழு	last_val = (unsigned முழு)-1;
    hb_codepoint_t	last_gid = HB_CODEPOINT_INVALID;

    max_inners.resize (inner_sets.length);
    க்கு (unsigned i = 0; i < inner_sets.length; i++) max_inners[i] = 0;

    /* Search backwards for a map value different from the last map value */
    auto &new_to_old_gid_list = plan->new_to_old_gid_list;
    unsigned மொத்தம் = new_to_old_gid_list.length;
    எனில் (!index_map)
    {
      map_மொத்தம் = new_to_old_gid_list.tail ().first + 1;
    }
    இல்லையெனில்
    {
      க்கு (unsigned j = மொத்தம்; j; j--)
      {
        hb_codepoint_t gid = new_to_old_gid_list.arrayZ[j - 1].first;
        hb_codepoint_t old_gid = new_to_old_gid_list.arrayZ[j - 1].second;
  
        unsigned முழு v = index_map->map (old_gid);
        எனில் (last_gid == HB_CODEPOINT_INVALID)
        {
  	last_val = v;
  	last_gid = gid;
  	தொடர்;
        }
        எனில் (v != last_val)
  	நிறுத்து;
  
        last_gid = gid;
      }
  
      எனில் (unlikely (last_gid == (hb_codepoint_t)-1)) திரும்பு;
      map_மொத்தம் = last_gid + 1;
    }

    க்கு (auto _ : plan->new_to_old_gid_list)
    {
      hb_codepoint_t gid = _.first;
      எனில் (gid >= map_மொத்தம்) நிறுத்து;

      hb_codepoint_t old_gid = _.second;
      unsigned முழு v = index_map ? index_map->map (old_gid): old_gid;
      unsigned முழு outer = v >> 16;
      unsigned முழு inner = v & 0xFFFF;
      outer_map.add (outer);
      எனில் (inner > max_inners[outer]) max_inners[outer] = inner;
      எனில் (outer >= inner_sets.length) திரும்பு;
      inner_sets[outer]->add (inner);
    }
  }

  வெறுமை fini ()
  {
    max_inners.fini ();
    output_map.fini ();
  }

  வெறுமை remap (மாறிலி DeltaSetIndexMap *உள்ளீடு_map,
	      மாறிலி hb_inc_bimap_t &outer_map,
	      மாறிலி hb_vector_t<hb_inc_bimap_t> &inner_maps,
	      மாறிலி hb_subset_plan_t *plan)
  {
    outer_bit_மொத்தம் = 1;
    inner_bit_மொத்தம் = 1;

    க்கு (unsigned முழு i = 0; i < max_inners.length; i++)
    {
      எனில் (inner_maps[i].get_population () == 0) தொடர்;
      unsigned முழு bit_மொத்தம் = (max_inners[i]==0)? 1: hb_bit_storage (inner_maps[i][max_inners[i]]);
      எனில் (bit_மொத்தம் > inner_bit_மொத்தம்) inner_bit_மொத்தம் = bit_மொத்தம்;
    }

    எனில் (unlikely (!output_map.resize (map_மொத்தம்))) திரும்பு;
    க்கு (மாறிலி auto &_ : plan->new_to_old_gid_list)
    {
      hb_codepoint_t new_gid = _.first;
      hb_codepoint_t old_gid = _.second;

      எனில் (unlikely (new_gid >= map_மொத்தம்)) நிறுத்து;

      uint32_t v = உள்ளீடு_map? உள்ளீடு_map->map (old_gid) : old_gid;
      unsigned outer = v >> 16;
      unsigned new_outer = outer_map[outer];
      unsigned bit_மொத்தம் = (new_outer == 0) ? 1 : hb_bit_storage (new_outer);
      outer_bit_மொத்தம் = hb_max (bit_மொத்தம், outer_bit_மொத்தம்);

      output_map.arrayZ[new_gid] = (new_outer << 16) | (inner_maps[outer][v & 0xFFFF]);
    }
  }

  தருக்க remap_after_instantiation (மாறிலி hb_subset_plan_t *plan,
                                  மாறிலி hb_map_t& varidx_map)
  {
    /* recalculate bit_count after remapping */
    outer_bit_மொத்தம் = 1;
    inner_bit_மொத்தம் = 1;

    க்கு (மாறிலி auto &_ : plan->new_to_old_gid_list)
    {
      hb_codepoint_t new_gid = _.first;
      எனில் (unlikely (new_gid >= map_மொத்தம்)) நிறுத்து;

      uint32_t v = output_map.arrayZ[new_gid];
      uint32_t *new_varidx;
      எனில் (!varidx_map.has (v, &new_varidx))
        திரும்பு பொய்;

      output_map.arrayZ[new_gid] = *new_varidx;

      unsigned outer = (*new_varidx) >> 16;
      unsigned bit_மொத்தம் = (outer == 0) ? 1 : hb_bit_storage (outer);
      outer_bit_மொத்தம் = hb_max (bit_மொத்தம், outer_bit_மொத்தம்);

      unsigned inner = (*new_varidx) & 0xFFFF;
      bit_மொத்தம் = (inner == 0) ? 1 : hb_bit_storage (inner);
      inner_bit_மொத்தம் = hb_max (bit_மொத்தம், inner_bit_மொத்தம்);
    }
    திரும்பு உண்மை;
  }

  unsigned முழு get_inner_bit_மொத்தம் () மாறிலி { திரும்பு inner_bit_மொத்தம்; }
  unsigned முழு get_width ()           மாறிலி { திரும்பு ((outer_bit_மொத்தம் + inner_bit_மொத்தம் + 7) / 8); }
  unsigned முழு get_map_மொத்தம் ()       மாறிலி { திரும்பு map_மொத்தம்; }

  size_t get_size () மாறிலி
  {
    எனில் (!map_மொத்தம்) திரும்பு 0;
    திரும்பு hb_unsigned_mul_add_saturate (get_width (), map_மொத்தம்,
					 DeltaSetIndexMap::min_size);
  }

  தருக்க is_identity () மாறிலி
  {
    எனில் (!output_map) திரும்பு உண்மை;
    /* An advance map whose entries are the identity over every output
     * glyph is equivalent to implicit glyph-id indexing; drop it.  A
     * truncated map doesn't qualify: glyphs past its end share its last
     * value, not their own ids. */
    எனில் (!can_drop_to_implicit || map_மொத்தம் != full_map_மொத்தம்) திரும்பு பொய்;
    க்கு (unsigned i = 0; i < map_மொத்தம்; i++)
      எனில் (output_map.arrayZ[i] != i) திரும்பு பொய்;
    திரும்பு உண்மை;
  }
  hb_array_t<மாறிலி uint32_t> get_output_map () மாறிலி { திரும்பு output_map.as_array (); }

  பாதுகாக்கப்பட்ட:
  unsigned முழு map_மொத்தம்;
  unsigned முழு full_map_மொத்தம்;
  தருக்க can_drop_to_implicit;
  hb_vector_t<unsigned முழு> max_inners;
  unsigned முழு outer_bit_மொத்தம்;
  unsigned முழு inner_bit_மொத்தம்;
  hb_vector_t<uint32_t> output_map;
};

கட்டமைப்பு hvarvvar_subset_plan_t
{
  hvarvvar_subset_plan_t() : inner_maps (), index_map_plans () {}
  ~hvarvvar_subset_plan_t() { fini (); }

  வெறுமை init (மாறிலி hb_array_t<மாறிலி DeltaSetIndexMap *> &index_maps,
	     மாறிலி ItemVariationStore &_var_store,
	     மாறிலி hb_subset_plan_t *plan)
  {
    index_map_plans.resize (index_maps.length);

    var_store = &_var_store;
    inner_sets.resize (var_store->get_sub_table_மொத்தம் ());
    க்கு (unsigned முழு i = 0; i < inner_sets.length; i++)
      inner_sets[i] = hb_set_create ();
    adv_set = hb_set_create ();

    inner_maps.resize (var_store->get_sub_table_மொத்தம் ());

    எனில் (unlikely (!index_map_plans.length || !inner_sets.length || !inner_maps.length)) திரும்பு;

    தருக்க retain_adv_map = பொய்;
    index_map_plans[0].init (index_maps[0], outer_map, inner_sets, plan, பொய்);
    எனில் (!index_maps[0])
    {
      retain_adv_map = plan->flags & HB_SUBSET_FLAGS_RETAIN_GIDS;
      outer_map.add (0);
      க்கு (hb_codepoint_t old_gid : plan->glyphset()->iter())
        inner_sets[0]->add (old_gid);
      hb_set_union (adv_set, inner_sets[0]);
    }

    க்கு (unsigned முழு i = 1; i < index_maps.length; i++)
      index_map_plans[i].init (index_maps[i], outer_map, inner_sets, plan);

    outer_map.sort ();

    எனில் (retain_adv_map)
    {
      க்கு (மாறிலி auto &_ : plan->new_to_old_gid_list)
      {
        hb_codepoint_t old_gid = _.second;
	inner_maps[0].add (old_gid);
      }
    }
    இல்லையெனில்
    {
      inner_maps[0].add_set (adv_set);
      hb_set_subtract (inner_sets[0], adv_set);
      inner_maps[0].add_set (inner_sets[0]);
    }

    க்கு (unsigned முழு i = 1; i < inner_maps.length; i++)
      inner_maps[i].add_set (inner_sets[i]);

    க்கு (unsigned முழு i = 0; i < index_maps.length; i++)
      index_map_plans[i].remap (index_maps[i], outer_map, inner_maps, plan);
  }

  /* remap */
  தருக்க remap_index_map_plans (மாறிலி hb_subset_plan_t *plan,
                              மாறிலி hb_map_t& varidx_map)
  {
    க்கு (unsigned i = 0; i < index_map_plans.length; i++)
      எனில் (!index_map_plans[i].remap_after_instantiation (plan, varidx_map))
        திரும்பு பொய்;
    திரும்பு உண்மை;
  }

  வெறுமை fini ()
  {
    க்கு (unsigned முழு i = 0; i < inner_sets.length; i++)
      hb_set_destroy (inner_sets[i]);
    hb_set_destroy (adv_set);
    inner_maps.fini ();
    index_map_plans.fini ();
  }

  hb_inc_bimap_t outer_map;
  hb_vector_t<hb_inc_bimap_t> inner_maps;
  hb_vector_t<index_map_subset_plan_t> index_map_plans;
  மாறிலி ItemVariationStore *var_store;

  பாதுகாக்கப்பட்ட:
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

கட்டமைப்பு HVARVVAR
{
  நிலையான constexpr hb_tag_t HVARTag = HB_OT_TAG_HVAR;
  நிலையான constexpr hb_tag_t VVARTag = HB_OT_TAG_VVAR;

  தருக்க has_data () மாறிலி { திரும்பு version.major != 0; }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (version.sanitize (c) &&
		  hb_barrier () &&
		  likely (version.major == 1) &&
		  varStore.sanitize (c, இது) &&
		  advMap.sanitize (c, இது) &&
		  lsbMap.sanitize (c, இது) &&
		  rsbMap.sanitize (c, இது));
  }

  மாறிலி ItemVariationStore& get_var_store () மாறிலி
  { திரும்பு இது+varStore; }

  வெறுமை listup_index_maps (hb_vector_t<மாறிலி DeltaSetIndexMap *> &index_maps) மாறிலி
  {
    எனில் (advMap) index_maps.push (&(இது+advMap));
    இல்லையெனில் index_maps.push (nullptr);

    எனில் (lsbMap) index_maps.push (&(இது+lsbMap));
    இல்லையெனில் index_maps.push (nullptr);

    எனில் (rsbMap) index_maps.push (&(இது+rsbMap));
    இல்லையெனில் index_maps.push (nullptr);
  }

  தருக்க serialize_index_maps (hb_serialize_context_t *c,
			     மாறிலி hb_array_t<index_map_subset_plan_t> &im_plans)
  {
    TRACE_SERIALIZE (இது);
    எனில் (im_plans[index_map_subset_plan_t::ADV_INDEX].is_identity ())
      advMap = 0;
    இல்லையெனில் எனில் (unlikely (!advMap.serialize_serialize (c, im_plans[index_map_subset_plan_t::ADV_INDEX])))
      return_trace (பொய்);
    எனில் (im_plans[index_map_subset_plan_t::LSB_INDEX].is_identity ())
      lsbMap = 0;
    இல்லையெனில் எனில் (unlikely (!lsbMap.serialize_serialize (c, im_plans[index_map_subset_plan_t::LSB_INDEX])))
      return_trace (பொய்);
    எனில் (im_plans[index_map_subset_plan_t::RSB_INDEX].is_identity ())
      rsbMap = 0;
    இல்லையெனில் எனில் (unlikely (!rsbMap.serialize_serialize (c, im_plans[index_map_subset_plan_t::RSB_INDEX])))
      return_trace (பொய்);

    return_trace (உண்மை);
  }

  template <typename T>
  தருக்க _subset (hb_subset_context_t *c) மாறிலி
  {
    TRACE_SUBSET (இது);
    எனில் (c->plan->all_axes_pinned)
      return_trace (பொய்);

    hvarvvar_subset_plan_t	hvar_plan;
    hb_vector_t<மாறிலி DeltaSetIndexMap *>
				index_maps;

    ((T*)இது)->listup_index_maps (index_maps);
    hvar_plan.init (index_maps.as_array (), இது+varStore, c->plan);

    T *out = c->serializer->allocate_min<T> ();
    எனில் (unlikely (!out)) return_trace (பொய்);

    out->version.major = 1;
    out->version.minor = 0;

    எனில் (c->plan->normalized_coords)
    {
      item_variations_t item_vars;
      எனில் (!item_vars.instantiate (இது+varStore, c->plan,
                                  advMap == 0 ? பொய் : உண்மை,
                                  பொய், /* use_no_variation_idx = false */
                                  hvar_plan.inner_maps.as_array ()))
        return_trace (பொய்);

      எனில் (!out->varStore.serialize_serialize (c->serializer,
                                              item_vars.has_long_word (),
                                              c->plan->axis_tags,
                                              item_vars.get_region_list (),
                                              item_vars.get_vardata_encodings ()))
        return_trace (பொய்);

      /* if varstore is optimized, remap output_map */
      எனில் (advMap)
      {
        எனில் (!hvar_plan.remap_index_map_plans (c->plan, item_vars.get_varidx_map ()))
          return_trace (பொய்);
      }
    }
    இல்லையெனில்
    {
      /* avar2 partial instancing: cull unreachable regions. */
      hb_set_t dead_regions;
      எனில் (c->plan->has_avar2)
	hvar_plan.var_store->collect_dead_regions (c->plan->axes_old_index_tag_map,
						   c->plan->avar2_reachable_ranges,
						   dead_regions);

      எனில் (unlikely (!out->varStore
		    .serialize_serialize (c->serializer,
					  hvar_plan.var_store,
					  hvar_plan.inner_maps.as_array (),
					  dead_regions.get_population () ? &dead_regions : nullptr)))
      return_trace (பொய்);
    }

    return_trace (out->T::serialize_index_maps (c->serializer,
						hvar_plan.index_map_plans.as_array ()));
  }

  HB_ALWAYS_INLINE
  தசம get_advance_delta_unscaled (hb_codepoint_t  glyph,
				    மாறிலி முழு *coords, unsigned முழு coord_மொத்தம்,
				    hb_scalar_cache_t *store_cache = nullptr) மாறிலி
  {
    /* Implicit mapping uses outer index zero and the glyph id as the
     * inner index. An oversized glyph id must not select another table. */
    எனில் (!advMap && glyph > 0xFFFFu) திரும்பு 0.f;
    uint32_t varidx = (இது+advMap).map (glyph);
    திரும்பு (இது+varStore).get_delta (varidx,
				      coords, coord_மொத்தம்,
				      store_cache);
  }

  பொது:
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

  பொது:
  DEFINE_SIZE_STATIC (20);
};

கட்டமைப்பு HVAR : HVARVVAR {
  நிலையான constexpr hb_tag_t tableTag = HB_OT_TAG_HVAR;
  தருக்க subset (hb_subset_context_t *c) மாறிலி { திரும்பு HVARVVAR::_subset<HVAR> (c); }
};
கட்டமைப்பு VVAR : HVARVVAR {
  நிலையான constexpr hb_tag_t tableTag = HB_OT_TAG_VVAR;

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (static_cast<மாறிலி HVARVVAR *> (இது)->sanitize (c) &&
		  vorgMap.sanitize (c, இது));
  }

  வெறுமை listup_index_maps (hb_vector_t<மாறிலி DeltaSetIndexMap *> &index_maps) மாறிலி
  {
    HVARVVAR::listup_index_maps (index_maps);
    எனில் (vorgMap) index_maps.push (&(இது+vorgMap));
    இல்லையெனில் index_maps.push (nullptr);
  }

  தருக்க serialize_index_maps (hb_serialize_context_t *c,
			     மாறிலி hb_array_t<index_map_subset_plan_t> &im_plans)
  {
    TRACE_SERIALIZE (இது);
    எனில் (unlikely (!HVARVVAR::serialize_index_maps (c, im_plans)))
      return_trace (பொய்);
    எனில் (!im_plans[index_map_subset_plan_t::VORG_INDEX].get_map_மொத்தம் ())
      vorgMap = 0;
    இல்லையெனில் எனில் (unlikely (!vorgMap.serialize_serialize (c, im_plans[index_map_subset_plan_t::VORG_INDEX])))
      return_trace (பொய்);

    return_trace (உண்மை);
  }

  தருக்க subset (hb_subset_context_t *c) மாறிலி { திரும்பு HVARVVAR::_subset<VVAR> (c); }

  HB_ALWAYS_INLINE
  தசம get_vorg_delta_unscaled (hb_codepoint_t glyph,
				 மாறிலி முழு *coords, unsigned முழு coord_மொத்தம்,
				 hb_scalar_cache_t *store_cache = nullptr) மாறிலி
  {
    எனில் (!vorgMap) திரும்பு 0.f;
    uint32_t varidx = (இது+vorgMap).map (glyph);
    திரும்பு (இது+varStore).get_delta (varidx,
				      coords, coord_மொத்தம்,
				      store_cache);
  }

  பாதுகாக்கப்பட்ட:
  Offset32To<DeltaSetIndexMap>
		vorgMap;	/* Offset to vertical-origin var-idx mapping. */

  பொது:
  DEFINE_SIZE_STATIC (24);
};

} /* namespace OT */


#endif /* HB_OT_VAR_HVAR_TABLE_HH */
