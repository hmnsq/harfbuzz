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

#അന്തര്ഭാവ "hb-ot-layout-common.hh"
#അന്തര്ഭാവ "hb-ot-var-common.hh"

നാമാകാശ OT {


ഘടന index_map_subset_plan_t
{
  എണ്ണൽ index_map_index_t {
    ADV_INDEX,
    LSB_INDEX,	/* dual as TSB */
    RSB_INDEX,	/* dual as BSB */
    VORG_INDEX
  };

  ശൂന്യം init (സ്ഥിരം DeltaSetIndexMap  *index_map,
	     hb_inc_bimap_t	     &outer_map,
	     hb_vector_t<hb_set_t *> &inner_sets,
	     സ്ഥിരം hb_subset_plan_t  *plan,
	     തർക്കം bypass_empty = സത്യം)
  {
    map_എണ്ണം = 0;
    full_map_എണ്ണം = plan->new_to_old_gid_list.length ?
		     plan->new_to_old_gid_list.tail ().first + 1 : 0;
    /* Only the advance mapping may be dropped in favor of implicit
     * glyph-id indexing (for the others a null offset means no deltas),
     * and only when the source was map-less too, preserving its form. */
    can_drop_to_implicit = !bypass_empty && !index_map;
    max_inners.init ();
    output_map.init ();

    എങ്കിൽ (bypass_empty && (!index_map || !index_map->get_map_എണ്ണം ())) തിരികെ;

    unsigned പൂർണ്ണസംഖ്യ	last_val = (unsigned പൂർണ്ണസംഖ്യ)-1;
    hb_codepoint_t	last_gid = HB_CODEPOINT_INVALID;

    max_inners.resize (inner_sets.length);
    വേണ്ടി (unsigned i = 0; i < inner_sets.length; i++) max_inners[i] = 0;

    /* Search backwards for a map value different from the last map value */
    auto &new_to_old_gid_list = plan->new_to_old_gid_list;
    unsigned എണ്ണം = new_to_old_gid_list.length;
    എങ്കിൽ (!index_map)
    {
      map_എണ്ണം = new_to_old_gid_list.tail ().first + 1;
    }
    അല്ലെങ്കിൽ
    {
      വേണ്ടി (unsigned j = എണ്ണം; j; j--)
      {
        hb_codepoint_t gid = new_to_old_gid_list.arrayZ[j - 1].first;
        hb_codepoint_t old_gid = new_to_old_gid_list.arrayZ[j - 1].second;
  
        unsigned പൂർണ്ണസംഖ്യ v = index_map->map (old_gid);
        എങ്കിൽ (last_gid == HB_CODEPOINT_INVALID)
        {
  	last_val = v;
  	last_gid = gid;
  	തുടരുക;
        }
        എങ്കിൽ (v != last_val)
  	നിർത്തുക;
  
        last_gid = gid;
      }
  
      എങ്കിൽ (unlikely (last_gid == (hb_codepoint_t)-1)) തിരികെ;
      map_എണ്ണം = last_gid + 1;
    }

    വേണ്ടി (auto _ : plan->new_to_old_gid_list)
    {
      hb_codepoint_t gid = _.first;
      എങ്കിൽ (gid >= map_എണ്ണം) നിർത്തുക;

      hb_codepoint_t old_gid = _.second;
      unsigned പൂർണ്ണസംഖ്യ v = index_map ? index_map->map (old_gid): old_gid;
      unsigned പൂർണ്ണസംഖ്യ outer = v >> 16;
      unsigned പൂർണ്ണസംഖ്യ inner = v & 0xFFFF;
      outer_map.add (outer);
      എങ്കിൽ (inner > max_inners[outer]) max_inners[outer] = inner;
      എങ്കിൽ (outer >= inner_sets.length) തിരികെ;
      inner_sets[outer]->add (inner);
    }
  }

  ശൂന്യം fini ()
  {
    max_inners.fini ();
    output_map.fini ();
  }

  ശൂന്യം remap (സ്ഥിരം DeltaSetIndexMap *നിവേശം_map,
	      സ്ഥിരം hb_inc_bimap_t &outer_map,
	      സ്ഥിരം hb_vector_t<hb_inc_bimap_t> &inner_maps,
	      സ്ഥിരം hb_subset_plan_t *plan)
  {
    outer_bit_എണ്ണം = 1;
    inner_bit_എണ്ണം = 1;

    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < max_inners.length; i++)
    {
      എങ്കിൽ (inner_maps[i].get_population () == 0) തുടരുക;
      unsigned പൂർണ്ണസംഖ്യ bit_എണ്ണം = (max_inners[i]==0)? 1: hb_bit_storage (inner_maps[i][max_inners[i]]);
      എങ്കിൽ (bit_എണ്ണം > inner_bit_എണ്ണം) inner_bit_എണ്ണം = bit_എണ്ണം;
    }

    എങ്കിൽ (unlikely (!output_map.resize (map_എണ്ണം))) തിരികെ;
    വേണ്ടി (സ്ഥിരം auto &_ : plan->new_to_old_gid_list)
    {
      hb_codepoint_t new_gid = _.first;
      hb_codepoint_t old_gid = _.second;

      എങ്കിൽ (unlikely (new_gid >= map_എണ്ണം)) നിർത്തുക;

      uint32_t v = നിവേശം_map? നിവേശം_map->map (old_gid) : old_gid;
      unsigned outer = v >> 16;
      unsigned new_outer = outer_map[outer];
      unsigned bit_എണ്ണം = (new_outer == 0) ? 1 : hb_bit_storage (new_outer);
      outer_bit_എണ്ണം = hb_max (bit_എണ്ണം, outer_bit_എണ്ണം);

      output_map.arrayZ[new_gid] = (new_outer << 16) | (inner_maps[outer][v & 0xFFFF]);
    }
  }

  തർക്കം remap_after_instantiation (സ്ഥിരം hb_subset_plan_t *plan,
                                  സ്ഥിരം hb_map_t& varidx_map)
  {
    /* recalculate bit_count after remapping */
    outer_bit_എണ്ണം = 1;
    inner_bit_എണ്ണം = 1;

    വേണ്ടി (സ്ഥിരം auto &_ : plan->new_to_old_gid_list)
    {
      hb_codepoint_t new_gid = _.first;
      എങ്കിൽ (unlikely (new_gid >= map_എണ്ണം)) നിർത്തുക;

      uint32_t v = output_map.arrayZ[new_gid];
      uint32_t *new_varidx;
      എങ്കിൽ (!varidx_map.has (v, &new_varidx))
        തിരികെ അസത്യം;

      output_map.arrayZ[new_gid] = *new_varidx;

      unsigned outer = (*new_varidx) >> 16;
      unsigned bit_എണ്ണം = (outer == 0) ? 1 : hb_bit_storage (outer);
      outer_bit_എണ്ണം = hb_max (bit_എണ്ണം, outer_bit_എണ്ണം);

      unsigned inner = (*new_varidx) & 0xFFFF;
      bit_എണ്ണം = (inner == 0) ? 1 : hb_bit_storage (inner);
      inner_bit_എണ്ണം = hb_max (bit_എണ്ണം, inner_bit_എണ്ണം);
    }
    തിരികെ സത്യം;
  }

  unsigned പൂർണ്ണസംഖ്യ get_inner_bit_എണ്ണം () സ്ഥിരം { തിരികെ inner_bit_എണ്ണം; }
  unsigned പൂർണ്ണസംഖ്യ get_width ()           സ്ഥിരം { തിരികെ ((outer_bit_എണ്ണം + inner_bit_എണ്ണം + 7) / 8); }
  unsigned പൂർണ്ണസംഖ്യ get_map_എണ്ണം ()       സ്ഥിരം { തിരികെ map_എണ്ണം; }

  size_t get_size () സ്ഥിരം
  {
    എങ്കിൽ (!map_എണ്ണം) തിരികെ 0;
    തിരികെ hb_unsigned_mul_add_saturate (get_width (), map_എണ്ണം,
					 DeltaSetIndexMap::min_size);
  }

  തർക്കം is_identity () സ്ഥിരം
  {
    എങ്കിൽ (!output_map) തിരികെ സത്യം;
    /* An advance map whose entries are the identity over every output
     * glyph is equivalent to implicit glyph-id indexing; drop it.  A
     * truncated map doesn't qualify: glyphs past its end share its last
     * value, not their own ids. */
    എങ്കിൽ (!can_drop_to_implicit || map_എണ്ണം != full_map_എണ്ണം) തിരികെ അസത്യം;
    വേണ്ടി (unsigned i = 0; i < map_എണ്ണം; i++)
      എങ്കിൽ (output_map.arrayZ[i] != i) തിരികെ അസത്യം;
    തിരികെ സത്യം;
  }
  hb_array_t<സ്ഥിരം uint32_t> get_output_map () സ്ഥിരം { തിരികെ output_map.as_array (); }

  സംരക്ഷിത:
  unsigned പൂർണ്ണസംഖ്യ map_എണ്ണം;
  unsigned പൂർണ്ണസംഖ്യ full_map_എണ്ണം;
  തർക്കം can_drop_to_implicit;
  hb_vector_t<unsigned പൂർണ്ണസംഖ്യ> max_inners;
  unsigned പൂർണ്ണസംഖ്യ outer_bit_എണ്ണം;
  unsigned പൂർണ്ണസംഖ്യ inner_bit_എണ്ണം;
  hb_vector_t<uint32_t> output_map;
};

ഘടന hvarvvar_subset_plan_t
{
  hvarvvar_subset_plan_t() : inner_maps (), index_map_plans () {}
  ~hvarvvar_subset_plan_t() { fini (); }

  ശൂന്യം init (സ്ഥിരം hb_array_t<സ്ഥിരം DeltaSetIndexMap *> &index_maps,
	     സ്ഥിരം ItemVariationStore &_var_store,
	     സ്ഥിരം hb_subset_plan_t *plan)
  {
    index_map_plans.resize (index_maps.length);

    var_store = &_var_store;
    inner_sets.resize (var_store->get_sub_table_എണ്ണം ());
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < inner_sets.length; i++)
      inner_sets[i] = hb_set_create ();
    adv_set = hb_set_create ();

    inner_maps.resize (var_store->get_sub_table_എണ്ണം ());

    എങ്കിൽ (unlikely (!index_map_plans.length || !inner_sets.length || !inner_maps.length)) തിരികെ;

    തർക്കം retain_adv_map = അസത്യം;
    index_map_plans[0].init (index_maps[0], outer_map, inner_sets, plan, അസത്യം);
    എങ്കിൽ (!index_maps[0])
    {
      retain_adv_map = plan->flags & HB_SUBSET_FLAGS_RETAIN_GIDS;
      outer_map.add (0);
      വേണ്ടി (hb_codepoint_t old_gid : plan->glyphset()->iter())
        inner_sets[0]->add (old_gid);
      hb_set_union (adv_set, inner_sets[0]);
    }

    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 1; i < index_maps.length; i++)
      index_map_plans[i].init (index_maps[i], outer_map, inner_sets, plan);

    outer_map.sort ();

    എങ്കിൽ (retain_adv_map)
    {
      വേണ്ടി (സ്ഥിരം auto &_ : plan->new_to_old_gid_list)
      {
        hb_codepoint_t old_gid = _.second;
	inner_maps[0].add (old_gid);
      }
    }
    അല്ലെങ്കിൽ
    {
      inner_maps[0].add_set (adv_set);
      hb_set_subtract (inner_sets[0], adv_set);
      inner_maps[0].add_set (inner_sets[0]);
    }

    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 1; i < inner_maps.length; i++)
      inner_maps[i].add_set (inner_sets[i]);

    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < index_maps.length; i++)
      index_map_plans[i].remap (index_maps[i], outer_map, inner_maps, plan);
  }

  /* remap */
  തർക്കം remap_index_map_plans (സ്ഥിരം hb_subset_plan_t *plan,
                              സ്ഥിരം hb_map_t& varidx_map)
  {
    വേണ്ടി (unsigned i = 0; i < index_map_plans.length; i++)
      എങ്കിൽ (!index_map_plans[i].remap_after_instantiation (plan, varidx_map))
        തിരികെ അസത്യം;
    തിരികെ സത്യം;
  }

  ശൂന്യം fini ()
  {
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < inner_sets.length; i++)
      hb_set_destroy (inner_sets[i]);
    hb_set_destroy (adv_set);
    inner_maps.fini ();
    index_map_plans.fini ();
  }

  hb_inc_bimap_t outer_map;
  hb_vector_t<hb_inc_bimap_t> inner_maps;
  hb_vector_t<index_map_subset_plan_t> index_map_plans;
  സ്ഥിരം ItemVariationStore *var_store;

  സംരക്ഷിത:
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

ഘടന HVARVVAR
{
  സ്ഥിതം constexpr hb_tag_t HVARTag = HB_OT_TAG_HVAR;
  സ്ഥിതം constexpr hb_tag_t VVARTag = HB_OT_TAG_VVAR;

  തർക്കം has_data () സ്ഥിരം { തിരികെ version.major != 0; }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (version.sanitize (c) &&
		  hb_barrier () &&
		  likely (version.major == 1) &&
		  varStore.sanitize (c, ഇത്) &&
		  advMap.sanitize (c, ഇത്) &&
		  lsbMap.sanitize (c, ഇത്) &&
		  rsbMap.sanitize (c, ഇത്));
  }

  സ്ഥിരം ItemVariationStore& get_var_store () സ്ഥിരം
  { തിരികെ ഇത്+varStore; }

  ശൂന്യം listup_index_maps (hb_vector_t<സ്ഥിരം DeltaSetIndexMap *> &index_maps) സ്ഥിരം
  {
    എങ്കിൽ (advMap) index_maps.push (&(ഇത്+advMap));
    അല്ലെങ്കിൽ index_maps.push (nullptr);

    എങ്കിൽ (lsbMap) index_maps.push (&(ഇത്+lsbMap));
    അല്ലെങ്കിൽ index_maps.push (nullptr);

    എങ്കിൽ (rsbMap) index_maps.push (&(ഇത്+rsbMap));
    അല്ലെങ്കിൽ index_maps.push (nullptr);
  }

  തർക്കം serialize_index_maps (hb_serialize_context_t *c,
			     സ്ഥിരം hb_array_t<index_map_subset_plan_t> &im_plans)
  {
    TRACE_SERIALIZE (ഇത്);
    എങ്കിൽ (im_plans[index_map_subset_plan_t::ADV_INDEX].is_identity ())
      advMap = 0;
    അല്ലെങ്കിൽ എങ്കിൽ (unlikely (!advMap.serialize_serialize (c, im_plans[index_map_subset_plan_t::ADV_INDEX])))
      return_trace (അസത്യം);
    എങ്കിൽ (im_plans[index_map_subset_plan_t::LSB_INDEX].is_identity ())
      lsbMap = 0;
    അല്ലെങ്കിൽ എങ്കിൽ (unlikely (!lsbMap.serialize_serialize (c, im_plans[index_map_subset_plan_t::LSB_INDEX])))
      return_trace (അസത്യം);
    എങ്കിൽ (im_plans[index_map_subset_plan_t::RSB_INDEX].is_identity ())
      rsbMap = 0;
    അല്ലെങ്കിൽ എങ്കിൽ (unlikely (!rsbMap.serialize_serialize (c, im_plans[index_map_subset_plan_t::RSB_INDEX])))
      return_trace (അസത്യം);

    return_trace (സത്യം);
  }

  template <typename T>
  തർക്കം _subset (hb_subset_context_t *c) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);
    എങ്കിൽ (c->plan->all_axes_pinned)
      return_trace (അസത്യം);

    hvarvvar_subset_plan_t	hvar_plan;
    hb_vector_t<സ്ഥിരം DeltaSetIndexMap *>
				index_maps;

    ((T*)ഇത്)->listup_index_maps (index_maps);
    hvar_plan.init (index_maps.as_array (), ഇത്+varStore, c->plan);

    T *out = c->serializer->allocate_min<T> ();
    എങ്കിൽ (unlikely (!out)) return_trace (അസത്യം);

    out->version.major = 1;
    out->version.minor = 0;

    എങ്കിൽ (c->plan->normalized_coords)
    {
      item_variations_t item_vars;
      എങ്കിൽ (!item_vars.instantiate (ഇത്+varStore, c->plan,
                                  advMap == 0 ? അസത്യം : സത്യം,
                                  അസത്യം, /* use_no_variation_idx = false */
                                  hvar_plan.inner_maps.as_array ()))
        return_trace (അസത്യം);

      എങ്കിൽ (!out->varStore.serialize_serialize (c->serializer,
                                              item_vars.has_long_word (),
                                              c->plan->axis_tags,
                                              item_vars.get_region_list (),
                                              item_vars.get_vardata_encodings ()))
        return_trace (അസത്യം);

      /* if varstore is optimized, remap output_map */
      എങ്കിൽ (advMap)
      {
        എങ്കിൽ (!hvar_plan.remap_index_map_plans (c->plan, item_vars.get_varidx_map ()))
          return_trace (അസത്യം);
      }
    }
    അല്ലെങ്കിൽ
    {
      /* avar2 partial instancing: cull unreachable regions. */
      hb_set_t dead_regions;
      എങ്കിൽ (c->plan->has_avar2)
	hvar_plan.var_store->collect_dead_regions (c->plan->axes_old_index_tag_map,
						   c->plan->avar2_reachable_ranges,
						   dead_regions);

      എങ്കിൽ (unlikely (!out->varStore
		    .serialize_serialize (c->serializer,
					  hvar_plan.var_store,
					  hvar_plan.inner_maps.as_array (),
					  dead_regions.get_population () ? &dead_regions : nullptr)))
      return_trace (അസത്യം);
    }

    return_trace (out->T::serialize_index_maps (c->serializer,
						hvar_plan.index_map_plans.as_array ()));
  }

  HB_ALWAYS_INLINE
  ദശാംശം get_advance_delta_unscaled (hb_codepoint_t  glyph,
				    സ്ഥിരം പൂർണ്ണസംഖ്യ *coords, unsigned പൂർണ്ണസംഖ്യ coord_എണ്ണം,
				    hb_scalar_cache_t *store_cache = nullptr) സ്ഥിരം
  {
    /* Implicit mapping uses outer index zero and the glyph id as the
     * inner index. An oversized glyph id must not select another table. */
    എങ്കിൽ (!advMap && glyph > 0xFFFFu) തിരികെ 0.f;
    uint32_t varidx = (ഇത്+advMap).map (glyph);
    തിരികെ (ഇത്+varStore).get_delta (varidx,
				      coords, coord_എണ്ണം,
				      store_cache);
  }

  പൊതു:
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

  പൊതു:
  DEFINE_SIZE_STATIC (20);
};

ഘടന HVAR : HVARVVAR {
  സ്ഥിതം constexpr hb_tag_t tableTag = HB_OT_TAG_HVAR;
  തർക്കം subset (hb_subset_context_t *c) സ്ഥിരം { തിരികെ HVARVVAR::_subset<HVAR> (c); }
};
ഘടന VVAR : HVARVVAR {
  സ്ഥിതം constexpr hb_tag_t tableTag = HB_OT_TAG_VVAR;

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (static_cast<സ്ഥിരം HVARVVAR *> (ഇത്)->sanitize (c) &&
		  vorgMap.sanitize (c, ഇത്));
  }

  ശൂന്യം listup_index_maps (hb_vector_t<സ്ഥിരം DeltaSetIndexMap *> &index_maps) സ്ഥിരം
  {
    HVARVVAR::listup_index_maps (index_maps);
    എങ്കിൽ (vorgMap) index_maps.push (&(ഇത്+vorgMap));
    അല്ലെങ്കിൽ index_maps.push (nullptr);
  }

  തർക്കം serialize_index_maps (hb_serialize_context_t *c,
			     സ്ഥിരം hb_array_t<index_map_subset_plan_t> &im_plans)
  {
    TRACE_SERIALIZE (ഇത്);
    എങ്കിൽ (unlikely (!HVARVVAR::serialize_index_maps (c, im_plans)))
      return_trace (അസത്യം);
    എങ്കിൽ (!im_plans[index_map_subset_plan_t::VORG_INDEX].get_map_എണ്ണം ())
      vorgMap = 0;
    അല്ലെങ്കിൽ എങ്കിൽ (unlikely (!vorgMap.serialize_serialize (c, im_plans[index_map_subset_plan_t::VORG_INDEX])))
      return_trace (അസത്യം);

    return_trace (സത്യം);
  }

  തർക്കം subset (hb_subset_context_t *c) സ്ഥിരം { തിരികെ HVARVVAR::_subset<VVAR> (c); }

  HB_ALWAYS_INLINE
  ദശാംശം get_vorg_delta_unscaled (hb_codepoint_t glyph,
				 സ്ഥിരം പൂർണ്ണസംഖ്യ *coords, unsigned പൂർണ്ണസംഖ്യ coord_എണ്ണം,
				 hb_scalar_cache_t *store_cache = nullptr) സ്ഥിരം
  {
    എങ്കിൽ (!vorgMap) തിരികെ 0.f;
    uint32_t varidx = (ഇത്+vorgMap).map (glyph);
    തിരികെ (ഇത്+varStore).get_delta (varidx,
				      coords, coord_എണ്ണം,
				      store_cache);
  }

  സംരക്ഷിത:
  Offset32To<DeltaSetIndexMap>
		vorgMap;	/* Offset to vertical-origin var-idx mapping. */

  പൊതു:
  DEFINE_SIZE_STATIC (24);
};

} /* namespace OT */


#endif /* HB_OT_VAR_HVAR_TABLE_HH */
