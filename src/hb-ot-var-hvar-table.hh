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

#ಅನ್ತರ್ಭಾವ "hb-ot-layout-common.hh"
#ಅನ್ತರ್ಭಾವ "hb-ot-var-common.hh"

ನಾಮಾಕಾಶ OT {


ಸಂರಚನಾ index_map_subset_plan_t
{
  ಗಣನಾ index_map_index_t {
    ADV_INDEX,
    LSB_INDEX,	/* dual as TSB */
    RSB_INDEX,	/* dual as BSB */
    VORG_INDEX
  };

  ಶೂನ್ಯ init (ಧ್ರುವ DeltaSetIndexMap  *index_map,
	     hb_inc_bimap_t	     &outer_map,
	     hb_vector_t<hb_set_t *> &inner_sets,
	     ಧ್ರುವ hb_subset_plan_t  *plan,
	     ತಾರ್ಕಿಕ bypass_empty = ಸತ್ಯ)
  {
    map_ಎಣಿಕೆ = 0;
    full_map_ಎಣಿಕೆ = plan->new_to_old_gid_list.length ?
		     plan->new_to_old_gid_list.tail ().first + 1 : 0;
    /* Only the advance mapping may be dropped in favor of implicit
     * glyph-id indexing (for the others a null offset means no deltas),
     * and only when the source was map-less too, preserving its form. */
    can_drop_to_implicit = !bypass_empty && !index_map;
    max_inners.init ();
    output_map.init ();

    ಯದಿ (bypass_empty && (!index_map || !index_map->get_map_ಎಣಿಕೆ ())) ಪ್ರತ್ಯಾಗಮ;

    unsigned ಪೂರ್ಣಾಙ್ಕ	last_val = (unsigned ಪೂರ್ಣಾಙ್ಕ)-1;
    hb_codepoint_t	last_gid = HB_CODEPOINT_INVALID;

    max_inners.resize (inner_sets.length);
    ಪ್ರತಿ (unsigned i = 0; i < inner_sets.length; i++) max_inners[i] = 0;

    /* Search backwards for a map value different from the last map value */
    auto &new_to_old_gid_list = plan->new_to_old_gid_list;
    unsigned ಎಣಿಕೆ = new_to_old_gid_list.length;
    ಯದಿ (!index_map)
    {
      map_ಎಣಿಕೆ = new_to_old_gid_list.tail ().first + 1;
    }
    ಅನ್ಯಥಾ
    {
      ಪ್ರತಿ (unsigned j = ಎಣಿಕೆ; j; j--)
      {
        hb_codepoint_t gid = new_to_old_gid_list.arrayZ[j - 1].first;
        hb_codepoint_t old_gid = new_to_old_gid_list.arrayZ[j - 1].second;
  
        unsigned ಪೂರ್ಣಾಙ್ಕ v = index_map->map (old_gid);
        ಯದಿ (last_gid == HB_CODEPOINT_INVALID)
        {
  	last_val = v;
  	last_gid = gid;
  	ಅನುವರ್ತ;
        }
        ಯದಿ (v != last_val)
  	ಭಙ್ಗ;
  
        last_gid = gid;
      }
  
      ಯದಿ (unlikely (last_gid == (hb_codepoint_t)-1)) ಪ್ರತ್ಯಾಗಮ;
      map_ಎಣಿಕೆ = last_gid + 1;
    }

    ಪ್ರತಿ (auto _ : plan->new_to_old_gid_list)
    {
      hb_codepoint_t gid = _.first;
      ಯದಿ (gid >= map_ಎಣಿಕೆ) ಭಙ್ಗ;

      hb_codepoint_t old_gid = _.second;
      unsigned ಪೂರ್ಣಾಙ್ಕ v = index_map ? index_map->map (old_gid): old_gid;
      unsigned ಪೂರ್ಣಾಙ್ಕ outer = v >> 16;
      unsigned ಪೂರ್ಣಾಙ್ಕ inner = v & 0xFFFF;
      outer_map.add (outer);
      ಯದಿ (inner > max_inners[outer]) max_inners[outer] = inner;
      ಯದಿ (outer >= inner_sets.length) ಪ್ರತ್ಯಾಗಮ;
      inner_sets[outer]->add (inner);
    }
  }

  ಶೂನ್ಯ fini ()
  {
    max_inners.fini ();
    output_map.fini ();
  }

  ಶೂನ್ಯ remap (ಧ್ರುವ DeltaSetIndexMap *ಒಳಹರಿವು_map,
	      ಧ್ರುವ hb_inc_bimap_t &outer_map,
	      ಧ್ರುವ hb_vector_t<hb_inc_bimap_t> &inner_maps,
	      ಧ್ರುವ hb_subset_plan_t *plan)
  {
    outer_bit_ಎಣಿಕೆ = 1;
    inner_bit_ಎಣಿಕೆ = 1;

    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < max_inners.length; i++)
    {
      ಯದಿ (inner_maps[i].get_population () == 0) ಅನುವರ್ತ;
      unsigned ಪೂರ್ಣಾಙ್ಕ bit_ಎಣಿಕೆ = (max_inners[i]==0)? 1: hb_bit_storage (inner_maps[i][max_inners[i]]);
      ಯದಿ (bit_ಎಣಿಕೆ > inner_bit_ಎಣಿಕೆ) inner_bit_ಎಣಿಕೆ = bit_ಎಣಿಕೆ;
    }

    ಯದಿ (unlikely (!output_map.resize (map_ಎಣಿಕೆ))) ಪ್ರತ್ಯಾಗಮ;
    ಪ್ರತಿ (ಧ್ರುವ auto &_ : plan->new_to_old_gid_list)
    {
      hb_codepoint_t new_gid = _.first;
      hb_codepoint_t old_gid = _.second;

      ಯದಿ (unlikely (new_gid >= map_ಎಣಿಕೆ)) ಭಙ್ಗ;

      uint32_t v = ಒಳಹರಿವು_map? ಒಳಹರಿವು_map->map (old_gid) : old_gid;
      unsigned outer = v >> 16;
      unsigned new_outer = outer_map[outer];
      unsigned bit_ಎಣಿಕೆ = (new_outer == 0) ? 1 : hb_bit_storage (new_outer);
      outer_bit_ಎಣಿಕೆ = hb_max (bit_ಎಣಿಕೆ, outer_bit_ಎಣಿಕೆ);

      output_map.arrayZ[new_gid] = (new_outer << 16) | (inner_maps[outer][v & 0xFFFF]);
    }
  }

  ತಾರ್ಕಿಕ remap_after_instantiation (ಧ್ರುವ hb_subset_plan_t *plan,
                                  ಧ್ರುವ hb_map_t& varidx_map)
  {
    /* recalculate bit_count after remapping */
    outer_bit_ಎಣಿಕೆ = 1;
    inner_bit_ಎಣಿಕೆ = 1;

    ಪ್ರತಿ (ಧ್ರುವ auto &_ : plan->new_to_old_gid_list)
    {
      hb_codepoint_t new_gid = _.first;
      ಯದಿ (unlikely (new_gid >= map_ಎಣಿಕೆ)) ಭಙ್ಗ;

      uint32_t v = output_map.arrayZ[new_gid];
      uint32_t *new_varidx;
      ಯದಿ (!varidx_map.has (v, &new_varidx))
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

      output_map.arrayZ[new_gid] = *new_varidx;

      unsigned outer = (*new_varidx) >> 16;
      unsigned bit_ಎಣಿಕೆ = (outer == 0) ? 1 : hb_bit_storage (outer);
      outer_bit_ಎಣಿಕೆ = hb_max (bit_ಎಣಿಕೆ, outer_bit_ಎಣಿಕೆ);

      unsigned inner = (*new_varidx) & 0xFFFF;
      bit_ಎಣಿಕೆ = (inner == 0) ? 1 : hb_bit_storage (inner);
      inner_bit_ಎಣಿಕೆ = hb_max (bit_ಎಣಿಕೆ, inner_bit_ಎಣಿಕೆ);
    }
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }

  unsigned ಪೂರ್ಣಾಙ್ಕ get_inner_bit_ಎಣಿಕೆ () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ inner_bit_ಎಣಿಕೆ; }
  unsigned ಪೂರ್ಣಾಙ್ಕ get_width ()           ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ ((outer_bit_ಎಣಿಕೆ + inner_bit_ಎಣಿಕೆ + 7) / 8); }
  unsigned ಪೂರ್ಣಾಙ್ಕ get_map_ಎಣಿಕೆ ()       ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ map_ಎಣಿಕೆ; }

  size_t get_size () ಧ್ರುವ
  {
    ಯದಿ (!map_ಎಣಿಕೆ) ಪ್ರತ್ಯಾಗಮ 0;
    ಪ್ರತ್ಯಾಗಮ hb_unsigned_mul_add_saturate (get_width (), map_ಎಣಿಕೆ,
					 DeltaSetIndexMap::min_size);
  }

  ತಾರ್ಕಿಕ is_identity () ಧ್ರುವ
  {
    ಯದಿ (!output_map) ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    /* An advance map whose entries are the identity over every output
     * glyph is equivalent to implicit glyph-id indexing; drop it.  A
     * truncated map doesn't qualify: glyphs past its end share its last
     * value, not their own ids. */
    ಯದಿ (!can_drop_to_implicit || map_ಎಣಿಕೆ != full_map_ಎಣಿಕೆ) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    ಪ್ರತಿ (unsigned i = 0; i < map_ಎಣಿಕೆ; i++)
      ಯದಿ (output_map.arrayZ[i] != i) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }
  hb_array_t<ಧ್ರುವ uint32_t> get_output_map () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ output_map.as_array (); }

  ರಕ್ಷಿತ:
  unsigned ಪೂರ್ಣಾಙ್ಕ map_ಎಣಿಕೆ;
  unsigned ಪೂರ್ಣಾಙ್ಕ full_map_ಎಣಿಕೆ;
  ತಾರ್ಕಿಕ can_drop_to_implicit;
  hb_vector_t<unsigned ಪೂರ್ಣಾಙ್ಕ> max_inners;
  unsigned ಪೂರ್ಣಾಙ್ಕ outer_bit_ಎಣಿಕೆ;
  unsigned ಪೂರ್ಣಾಙ್ಕ inner_bit_ಎಣಿಕೆ;
  hb_vector_t<uint32_t> output_map;
};

ಸಂರಚನಾ hvarvvar_subset_plan_t
{
  hvarvvar_subset_plan_t() : inner_maps (), index_map_plans () {}
  ~hvarvvar_subset_plan_t() { fini (); }

  ಶೂನ್ಯ init (ಧ್ರುವ hb_array_t<ಧ್ರುವ DeltaSetIndexMap *> &index_maps,
	     ಧ್ರುವ ItemVariationStore &_var_store,
	     ಧ್ರುವ hb_subset_plan_t *plan)
  {
    index_map_plans.resize (index_maps.length);

    var_store = &_var_store;
    inner_sets.resize (var_store->get_sub_table_ಎಣಿಕೆ ());
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < inner_sets.length; i++)
      inner_sets[i] = hb_set_create ();
    adv_set = hb_set_create ();

    inner_maps.resize (var_store->get_sub_table_ಎಣಿಕೆ ());

    ಯದಿ (unlikely (!index_map_plans.length || !inner_sets.length || !inner_maps.length)) ಪ್ರತ್ಯಾಗಮ;

    ತಾರ್ಕಿಕ retain_adv_map = ಅಸತ್ಯ;
    index_map_plans[0].init (index_maps[0], outer_map, inner_sets, plan, ಅಸತ್ಯ);
    ಯದಿ (!index_maps[0])
    {
      retain_adv_map = plan->flags & HB_SUBSET_FLAGS_RETAIN_GIDS;
      outer_map.add (0);
      ಪ್ರತಿ (hb_codepoint_t old_gid : plan->glyphset()->iter())
        inner_sets[0]->add (old_gid);
      hb_set_union (adv_set, inner_sets[0]);
    }

    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 1; i < index_maps.length; i++)
      index_map_plans[i].init (index_maps[i], outer_map, inner_sets, plan);

    outer_map.sort ();

    ಯದಿ (retain_adv_map)
    {
      ಪ್ರತಿ (ಧ್ರುವ auto &_ : plan->new_to_old_gid_list)
      {
        hb_codepoint_t old_gid = _.second;
	inner_maps[0].add (old_gid);
      }
    }
    ಅನ್ಯಥಾ
    {
      inner_maps[0].add_set (adv_set);
      hb_set_subtract (inner_sets[0], adv_set);
      inner_maps[0].add_set (inner_sets[0]);
    }

    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 1; i < inner_maps.length; i++)
      inner_maps[i].add_set (inner_sets[i]);

    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < index_maps.length; i++)
      index_map_plans[i].remap (index_maps[i], outer_map, inner_maps, plan);
  }

  /* remap */
  ತಾರ್ಕಿಕ remap_index_map_plans (ಧ್ರುವ hb_subset_plan_t *plan,
                              ಧ್ರುವ hb_map_t& varidx_map)
  {
    ಪ್ರತಿ (unsigned i = 0; i < index_map_plans.length; i++)
      ಯದಿ (!index_map_plans[i].remap_after_instantiation (plan, varidx_map))
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }

  ಶೂನ್ಯ fini ()
  {
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < inner_sets.length; i++)
      hb_set_destroy (inner_sets[i]);
    hb_set_destroy (adv_set);
    inner_maps.fini ();
    index_map_plans.fini ();
  }

  hb_inc_bimap_t outer_map;
  hb_vector_t<hb_inc_bimap_t> inner_maps;
  hb_vector_t<index_map_subset_plan_t> index_map_plans;
  ಧ್ರುವ ItemVariationStore *var_store;

  ರಕ್ಷಿತ:
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

ಸಂರಚನಾ HVARVVAR
{
  ಸ್ಥಿರ constexpr hb_tag_t HVARTag = HB_OT_TAG_HVAR;
  ಸ್ಥಿರ constexpr hb_tag_t VVARTag = HB_OT_TAG_VVAR;

  ತಾರ್ಕಿಕ has_data () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ version.major != 0; }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (version.sanitize (c) &&
		  hb_barrier () &&
		  likely (version.major == 1) &&
		  varStore.sanitize (c, ಏತತ್) &&
		  advMap.sanitize (c, ಏತತ್) &&
		  lsbMap.sanitize (c, ಏತತ್) &&
		  rsbMap.sanitize (c, ಏತತ್));
  }

  ಧ್ರುವ ItemVariationStore& get_var_store () ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ ಏತತ್+varStore; }

  ಶೂನ್ಯ listup_index_maps (hb_vector_t<ಧ್ರುವ DeltaSetIndexMap *> &index_maps) ಧ್ರುವ
  {
    ಯದಿ (advMap) index_maps.push (&(ಏತತ್+advMap));
    ಅನ್ಯಥಾ index_maps.push (nullptr);

    ಯದಿ (lsbMap) index_maps.push (&(ಏತತ್+lsbMap));
    ಅನ್ಯಥಾ index_maps.push (nullptr);

    ಯದಿ (rsbMap) index_maps.push (&(ಏತತ್+rsbMap));
    ಅನ್ಯಥಾ index_maps.push (nullptr);
  }

  ತಾರ್ಕಿಕ serialize_index_maps (hb_serialize_context_t *c,
			     ಧ್ರುವ hb_array_t<index_map_subset_plan_t> &im_plans)
  {
    TRACE_SERIALIZE (ಏತತ್);
    ಯದಿ (im_plans[index_map_subset_plan_t::ADV_INDEX].is_identity ())
      advMap = 0;
    ಅನ್ಯಥಾ ಯದಿ (unlikely (!advMap.serialize_serialize (c, im_plans[index_map_subset_plan_t::ADV_INDEX])))
      return_trace (ಅಸತ್ಯ);
    ಯದಿ (im_plans[index_map_subset_plan_t::LSB_INDEX].is_identity ())
      lsbMap = 0;
    ಅನ್ಯಥಾ ಯದಿ (unlikely (!lsbMap.serialize_serialize (c, im_plans[index_map_subset_plan_t::LSB_INDEX])))
      return_trace (ಅಸತ್ಯ);
    ಯದಿ (im_plans[index_map_subset_plan_t::RSB_INDEX].is_identity ())
      rsbMap = 0;
    ಅನ್ಯಥಾ ಯದಿ (unlikely (!rsbMap.serialize_serialize (c, im_plans[index_map_subset_plan_t::RSB_INDEX])))
      return_trace (ಅಸತ್ಯ);

    return_trace (ಸತ್ಯ);
  }

  template <typename T>
  ತಾರ್ಕಿಕ _subset (hb_subset_context_t *c) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    ಯದಿ (c->plan->all_axes_pinned)
      return_trace (ಅಸತ್ಯ);

    hvarvvar_subset_plan_t	hvar_plan;
    hb_vector_t<ಧ್ರುವ DeltaSetIndexMap *>
				index_maps;

    ((T*)ಏತತ್)->listup_index_maps (index_maps);
    hvar_plan.init (index_maps.as_array (), ಏತತ್+varStore, c->plan);

    T *out = c->serializer->allocate_min<T> ();
    ಯದಿ (unlikely (!out)) return_trace (ಅಸತ್ಯ);

    out->version.major = 1;
    out->version.minor = 0;

    ಯದಿ (c->plan->normalized_coords)
    {
      item_variations_t item_vars;
      ಯದಿ (!item_vars.instantiate (ಏತತ್+varStore, c->plan,
                                  advMap == 0 ? ಅಸತ್ಯ : ಸತ್ಯ,
                                  ಅಸತ್ಯ, /* use_no_variation_idx = false */
                                  hvar_plan.inner_maps.as_array ()))
        return_trace (ಅಸತ್ಯ);

      ಯದಿ (!out->varStore.serialize_serialize (c->serializer,
                                              item_vars.has_long_word (),
                                              c->plan->axis_tags,
                                              item_vars.get_region_list (),
                                              item_vars.get_vardata_encodings ()))
        return_trace (ಅಸತ್ಯ);

      /* if varstore is optimized, remap output_map */
      ಯದಿ (advMap)
      {
        ಯದಿ (!hvar_plan.remap_index_map_plans (c->plan, item_vars.get_varidx_map ()))
          return_trace (ಅಸತ್ಯ);
      }
    }
    ಅನ್ಯಥಾ
    {
      /* avar2 partial instancing: cull unreachable regions. */
      hb_set_t dead_regions;
      ಯದಿ (c->plan->has_avar2)
	hvar_plan.var_store->collect_dead_regions (c->plan->axes_old_index_tag_map,
						   c->plan->avar2_reachable_ranges,
						   dead_regions);

      ಯದಿ (unlikely (!out->varStore
		    .serialize_serialize (c->serializer,
					  hvar_plan.var_store,
					  hvar_plan.inner_maps.as_array (),
					  dead_regions.get_population () ? &dead_regions : nullptr)))
      return_trace (ಅಸತ್ಯ);
    }

    return_trace (out->T::serialize_index_maps (c->serializer,
						hvar_plan.index_map_plans.as_array ()));
  }

  HB_ALWAYS_INLINE
  ಭಿನ್ನ get_advance_delta_unscaled (hb_codepoint_t  glyph,
				    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ *coords, unsigned ಪೂರ್ಣಾಙ್ಕ coord_ಎಣಿಕೆ,
				    hb_scalar_cache_t *store_cache = nullptr) ಧ್ರುವ
  {
    /* Implicit mapping uses outer index zero and the glyph id as the
     * inner index. An oversized glyph id must not select another table. */
    ಯದಿ (!advMap && glyph > 0xFFFFu) ಪ್ರತ್ಯಾಗಮ 0.f;
    uint32_t varidx = (ಏತತ್+advMap).map (glyph);
    ಪ್ರತ್ಯಾಗಮ (ಏತತ್+varStore).get_delta (varidx,
				      coords, coord_ಎಣಿಕೆ,
				      store_cache);
  }

  ಸಾರ್ವಜನಿಕ:
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

  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (20);
};

ಸಂರಚನಾ HVAR : HVARVVAR {
  ಸ್ಥಿರ constexpr hb_tag_t tableTag = HB_OT_TAG_HVAR;
  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ HVARVVAR::_subset<HVAR> (c); }
};
ಸಂರಚನಾ VVAR : HVARVVAR {
  ಸ್ಥಿರ constexpr hb_tag_t tableTag = HB_OT_TAG_VVAR;

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (static_cast<ಧ್ರುವ HVARVVAR *> (ಏತತ್)->sanitize (c) &&
		  vorgMap.sanitize (c, ಏತತ್));
  }

  ಶೂನ್ಯ listup_index_maps (hb_vector_t<ಧ್ರುವ DeltaSetIndexMap *> &index_maps) ಧ್ರುವ
  {
    HVARVVAR::listup_index_maps (index_maps);
    ಯದಿ (vorgMap) index_maps.push (&(ಏತತ್+vorgMap));
    ಅನ್ಯಥಾ index_maps.push (nullptr);
  }

  ತಾರ್ಕಿಕ serialize_index_maps (hb_serialize_context_t *c,
			     ಧ್ರುವ hb_array_t<index_map_subset_plan_t> &im_plans)
  {
    TRACE_SERIALIZE (ಏತತ್);
    ಯದಿ (unlikely (!HVARVVAR::serialize_index_maps (c, im_plans)))
      return_trace (ಅಸತ್ಯ);
    ಯದಿ (!im_plans[index_map_subset_plan_t::VORG_INDEX].get_map_ಎಣಿಕೆ ())
      vorgMap = 0;
    ಅನ್ಯಥಾ ಯದಿ (unlikely (!vorgMap.serialize_serialize (c, im_plans[index_map_subset_plan_t::VORG_INDEX])))
      return_trace (ಅಸತ್ಯ);

    return_trace (ಸತ್ಯ);
  }

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ HVARVVAR::_subset<VVAR> (c); }

  HB_ALWAYS_INLINE
  ಭಿನ್ನ get_vorg_delta_unscaled (hb_codepoint_t glyph,
				 ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ *coords, unsigned ಪೂರ್ಣಾಙ್ಕ coord_ಎಣಿಕೆ,
				 hb_scalar_cache_t *store_cache = nullptr) ಧ್ರುವ
  {
    ಯದಿ (!vorgMap) ಪ್ರತ್ಯಾಗಮ 0.f;
    uint32_t varidx = (ಏತತ್+vorgMap).map (glyph);
    ಪ್ರತ್ಯಾಗಮ (ಏತತ್+varStore).get_delta (varidx,
				      coords, coord_ಎಣಿಕೆ,
				      store_cache);
  }

  ರಕ್ಷಿತ:
  Offset32To<DeltaSetIndexMap>
		vorgMap;	/* Offset to vertical-origin var-idx mapping. */

  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (24);
};

} /* namespace OT */


#endif /* HB_OT_VAR_HVAR_TABLE_HH */
