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

#અન્તર્ભાવ "hb-ot-layout-common.hh"
#અન્તર્ભાવ "hb-ot-var-common.hh"

નામાકાશ OT {


સંરચના index_map_subset_plan_t
{
  ગણના index_map_index_t {
    ADV_INDEX,
    LSB_INDEX,	/* dual as TSB */
    RSB_INDEX,	/* dual as BSB */
    VORG_INDEX
  };

  શૂન્ય init (અચલ DeltaSetIndexMap  *index_map,
	     hb_inc_bimap_t	     &outer_map,
	     hb_vector_t<hb_set_t *> &inner_sets,
	     અચલ hb_subset_plan_t  *plan,
	     તાર્કિક bypass_empty = સાચું)
  {
    map_ગણતરી = 0;
    full_map_ગણતરી = plan->new_to_old_gid_list.length ?
		     plan->new_to_old_gid_list.tail ().first + 1 : 0;
    /* Only the advance mapping may be dropped in favor of implicit
     * glyph-id indexing (for the others a null offset means no deltas),
     * and only when the source was map-less too, preserving its form. */
    can_drop_to_implicit = !bypass_empty && !index_map;
    max_inners.init ();
    output_map.init ();

    જો (bypass_empty && (!index_map || !index_map->get_map_ગણતરી ())) પાછા;

    unsigned પૂર્ણાંક	last_val = (unsigned પૂર્ણાંક)-1;
    hb_codepoint_t	last_gid = HB_CODEPOINT_INVALID;

    max_inners.resize (inner_sets.length);
    માટે (unsigned i = 0; i < inner_sets.length; i++) max_inners[i] = 0;

    /* Search backwards for a map value different from the last map value */
    auto &new_to_old_gid_list = plan->new_to_old_gid_list;
    unsigned ગણતરી = new_to_old_gid_list.length;
    જો (!index_map)
    {
      map_ગણતરી = new_to_old_gid_list.tail ().first + 1;
    }
    નહીંતર
    {
      માટે (unsigned j = ગણતરી; j; j--)
      {
        hb_codepoint_t gid = new_to_old_gid_list.arrayZ[j - 1].first;
        hb_codepoint_t old_gid = new_to_old_gid_list.arrayZ[j - 1].second;
  
        unsigned પૂર્ણાંક v = index_map->map (old_gid);
        જો (last_gid == HB_CODEPOINT_INVALID)
        {
  	last_val = v;
  	last_gid = gid;
  	ચાલુ;
        }
        જો (v != last_val)
  	તોડો;
  
        last_gid = gid;
      }
  
      જો (unlikely (last_gid == (hb_codepoint_t)-1)) પાછા;
      map_ગણતરી = last_gid + 1;
    }

    માટે (auto _ : plan->new_to_old_gid_list)
    {
      hb_codepoint_t gid = _.first;
      જો (gid >= map_ગણતરી) તોડો;

      hb_codepoint_t old_gid = _.second;
      unsigned પૂર્ણાંક v = index_map ? index_map->map (old_gid): old_gid;
      unsigned પૂર્ણાંક outer = v >> 16;
      unsigned પૂર્ણાંક inner = v & 0xFFFF;
      outer_map.add (outer);
      જો (inner > max_inners[outer]) max_inners[outer] = inner;
      જો (outer >= inner_sets.length) પાછા;
      inner_sets[outer]->add (inner);
    }
  }

  શૂન્ય fini ()
  {
    max_inners.fini ();
    output_map.fini ();
  }

  શૂન્ય remap (અચલ DeltaSetIndexMap *આગત_map,
	      અચલ hb_inc_bimap_t &outer_map,
	      અચલ hb_vector_t<hb_inc_bimap_t> &inner_maps,
	      અચલ hb_subset_plan_t *plan)
  {
    outer_bit_ગણતરી = 1;
    inner_bit_ગણતરી = 1;

    માટે (unsigned પૂર્ણાંક i = 0; i < max_inners.length; i++)
    {
      જો (inner_maps[i].get_population () == 0) ચાલુ;
      unsigned પૂર્ણાંક bit_ગણતરી = (max_inners[i]==0)? 1: hb_bit_storage (inner_maps[i][max_inners[i]]);
      જો (bit_ગણતરી > inner_bit_ગણતરી) inner_bit_ગણતરી = bit_ગણતરી;
    }

    જો (unlikely (!output_map.resize (map_ગણતરી))) પાછા;
    માટે (અચલ auto &_ : plan->new_to_old_gid_list)
    {
      hb_codepoint_t new_gid = _.first;
      hb_codepoint_t old_gid = _.second;

      જો (unlikely (new_gid >= map_ગણતરી)) તોડો;

      uint32_t v = આગત_map? આગત_map->map (old_gid) : old_gid;
      unsigned outer = v >> 16;
      unsigned new_outer = outer_map[outer];
      unsigned bit_ગણતરી = (new_outer == 0) ? 1 : hb_bit_storage (new_outer);
      outer_bit_ગણતરી = hb_max (bit_ગણતરી, outer_bit_ગણતરી);

      output_map.arrayZ[new_gid] = (new_outer << 16) | (inner_maps[outer][v & 0xFFFF]);
    }
  }

  તાર્કિક remap_after_instantiation (અચલ hb_subset_plan_t *plan,
                                  અચલ hb_map_t& varidx_map)
  {
    /* recalculate bit_count after remapping */
    outer_bit_ગણતરી = 1;
    inner_bit_ગણતરી = 1;

    માટે (અચલ auto &_ : plan->new_to_old_gid_list)
    {
      hb_codepoint_t new_gid = _.first;
      જો (unlikely (new_gid >= map_ગણતરી)) તોડો;

      uint32_t v = output_map.arrayZ[new_gid];
      uint32_t *new_varidx;
      જો (!varidx_map.has (v, &new_varidx))
        પાછા ખોટું;

      output_map.arrayZ[new_gid] = *new_varidx;

      unsigned outer = (*new_varidx) >> 16;
      unsigned bit_ગણતરી = (outer == 0) ? 1 : hb_bit_storage (outer);
      outer_bit_ગણતરી = hb_max (bit_ગણતરી, outer_bit_ગણતરી);

      unsigned inner = (*new_varidx) & 0xFFFF;
      bit_ગણતરી = (inner == 0) ? 1 : hb_bit_storage (inner);
      inner_bit_ગણતરી = hb_max (bit_ગણતરી, inner_bit_ગણતરી);
    }
    પાછા સાચું;
  }

  unsigned પૂર્ણાંક get_inner_bit_ગણતરી () અચલ { પાછા inner_bit_ગણતરી; }
  unsigned પૂર્ણાંક get_width ()           અચલ { પાછા ((outer_bit_ગણતરી + inner_bit_ગણતરી + 7) / 8); }
  unsigned પૂર્ણાંક get_map_ગણતરી ()       અચલ { પાછા map_ગણતરી; }

  size_t get_size () અચલ
  {
    જો (!map_ગણતરી) પાછા 0;
    પાછા hb_unsigned_mul_add_saturate (get_width (), map_ગણતરી,
					 DeltaSetIndexMap::min_size);
  }

  તાર્કિક is_identity () અચલ
  {
    જો (!output_map) પાછા સાચું;
    /* An advance map whose entries are the identity over every output
     * glyph is equivalent to implicit glyph-id indexing; drop it.  A
     * truncated map doesn't qualify: glyphs past its end share its last
     * value, not their own ids. */
    જો (!can_drop_to_implicit || map_ગણતરી != full_map_ગણતરી) પાછા ખોટું;
    માટે (unsigned i = 0; i < map_ગણતરી; i++)
      જો (output_map.arrayZ[i] != i) પાછા ખોટું;
    પાછા સાચું;
  }
  hb_array_t<અચલ uint32_t> get_output_map () અચલ { પાછા output_map.as_array (); }

  સુરક્ષિત:
  unsigned પૂર્ણાંક map_ગણતરી;
  unsigned પૂર્ણાંક full_map_ગણતરી;
  તાર્કિક can_drop_to_implicit;
  hb_vector_t<unsigned પૂર્ણાંક> max_inners;
  unsigned પૂર્ણાંક outer_bit_ગણતરી;
  unsigned પૂર્ણાંક inner_bit_ગણતરી;
  hb_vector_t<uint32_t> output_map;
};

સંરચના hvarvvar_subset_plan_t
{
  hvarvvar_subset_plan_t() : inner_maps (), index_map_plans () {}
  ~hvarvvar_subset_plan_t() { fini (); }

  શૂન્ય init (અચલ hb_array_t<અચલ DeltaSetIndexMap *> &index_maps,
	     અચલ ItemVariationStore &_var_store,
	     અચલ hb_subset_plan_t *plan)
  {
    index_map_plans.resize (index_maps.length);

    var_store = &_var_store;
    inner_sets.resize (var_store->get_sub_table_ગણતરી ());
    માટે (unsigned પૂર્ણાંક i = 0; i < inner_sets.length; i++)
      inner_sets[i] = hb_set_create ();
    adv_set = hb_set_create ();

    inner_maps.resize (var_store->get_sub_table_ગણતરી ());

    જો (unlikely (!index_map_plans.length || !inner_sets.length || !inner_maps.length)) પાછા;

    તાર્કિક retain_adv_map = ખોટું;
    index_map_plans[0].init (index_maps[0], outer_map, inner_sets, plan, ખોટું);
    જો (!index_maps[0])
    {
      retain_adv_map = plan->flags & HB_SUBSET_FLAGS_RETAIN_GIDS;
      outer_map.add (0);
      માટે (hb_codepoint_t old_gid : plan->glyphset()->iter())
        inner_sets[0]->add (old_gid);
      hb_set_union (adv_set, inner_sets[0]);
    }

    માટે (unsigned પૂર્ણાંક i = 1; i < index_maps.length; i++)
      index_map_plans[i].init (index_maps[i], outer_map, inner_sets, plan);

    outer_map.sort ();

    જો (retain_adv_map)
    {
      માટે (અચલ auto &_ : plan->new_to_old_gid_list)
      {
        hb_codepoint_t old_gid = _.second;
	inner_maps[0].add (old_gid);
      }
    }
    નહીંતર
    {
      inner_maps[0].add_set (adv_set);
      hb_set_subtract (inner_sets[0], adv_set);
      inner_maps[0].add_set (inner_sets[0]);
    }

    માટે (unsigned પૂર્ણાંક i = 1; i < inner_maps.length; i++)
      inner_maps[i].add_set (inner_sets[i]);

    માટે (unsigned પૂર્ણાંક i = 0; i < index_maps.length; i++)
      index_map_plans[i].remap (index_maps[i], outer_map, inner_maps, plan);
  }

  /* remap */
  તાર્કિક remap_index_map_plans (અચલ hb_subset_plan_t *plan,
                              અચલ hb_map_t& varidx_map)
  {
    માટે (unsigned i = 0; i < index_map_plans.length; i++)
      જો (!index_map_plans[i].remap_after_instantiation (plan, varidx_map))
        પાછા ખોટું;
    પાછા સાચું;
  }

  શૂન્ય fini ()
  {
    માટે (unsigned પૂર્ણાંક i = 0; i < inner_sets.length; i++)
      hb_set_destroy (inner_sets[i]);
    hb_set_destroy (adv_set);
    inner_maps.fini ();
    index_map_plans.fini ();
  }

  hb_inc_bimap_t outer_map;
  hb_vector_t<hb_inc_bimap_t> inner_maps;
  hb_vector_t<index_map_subset_plan_t> index_map_plans;
  અચલ ItemVariationStore *var_store;

  સુરક્ષિત:
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

સંરચના HVARVVAR
{
  સ્થિર constexpr hb_tag_t HVARTag = HB_OT_TAG_HVAR;
  સ્થિર constexpr hb_tag_t VVARTag = HB_OT_TAG_VVAR;

  તાર્કિક has_data () અચલ { પાછા version.major != 0; }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (version.sanitize (c) &&
		  hb_barrier () &&
		  likely (version.major == 1) &&
		  varStore.sanitize (c, આ) &&
		  advMap.sanitize (c, આ) &&
		  lsbMap.sanitize (c, આ) &&
		  rsbMap.sanitize (c, આ));
  }

  અચલ ItemVariationStore& get_var_store () અચલ
  { પાછા આ+varStore; }

  શૂન્ય listup_index_maps (hb_vector_t<અચલ DeltaSetIndexMap *> &index_maps) અચલ
  {
    જો (advMap) index_maps.push (&(આ+advMap));
    નહીંતર index_maps.push (nullptr);

    જો (lsbMap) index_maps.push (&(આ+lsbMap));
    નહીંતર index_maps.push (nullptr);

    જો (rsbMap) index_maps.push (&(આ+rsbMap));
    નહીંતર index_maps.push (nullptr);
  }

  તાર્કિક serialize_index_maps (hb_serialize_context_t *c,
			     અચલ hb_array_t<index_map_subset_plan_t> &im_plans)
  {
    TRACE_SERIALIZE (આ);
    જો (im_plans[index_map_subset_plan_t::ADV_INDEX].is_identity ())
      advMap = 0;
    નહીંતર જો (unlikely (!advMap.serialize_serialize (c, im_plans[index_map_subset_plan_t::ADV_INDEX])))
      return_trace (ખોટું);
    જો (im_plans[index_map_subset_plan_t::LSB_INDEX].is_identity ())
      lsbMap = 0;
    નહીંતર જો (unlikely (!lsbMap.serialize_serialize (c, im_plans[index_map_subset_plan_t::LSB_INDEX])))
      return_trace (ખોટું);
    જો (im_plans[index_map_subset_plan_t::RSB_INDEX].is_identity ())
      rsbMap = 0;
    નહીંતર જો (unlikely (!rsbMap.serialize_serialize (c, im_plans[index_map_subset_plan_t::RSB_INDEX])))
      return_trace (ખોટું);

    return_trace (સાચું);
  }

  template <typename T>
  તાર્કિક _subset (hb_subset_context_t *c) અચલ
  {
    TRACE_SUBSET (આ);
    જો (c->plan->all_axes_pinned)
      return_trace (ખોટું);

    hvarvvar_subset_plan_t	hvar_plan;
    hb_vector_t<અચલ DeltaSetIndexMap *>
				index_maps;

    ((T*)આ)->listup_index_maps (index_maps);
    hvar_plan.init (index_maps.as_array (), આ+varStore, c->plan);

    T *out = c->serializer->allocate_min<T> ();
    જો (unlikely (!out)) return_trace (ખોટું);

    out->version.major = 1;
    out->version.minor = 0;

    જો (c->plan->normalized_coords)
    {
      item_variations_t item_vars;
      જો (!item_vars.instantiate (આ+varStore, c->plan,
                                  advMap == 0 ? ખોટું : સાચું,
                                  ખોટું, /* use_no_variation_idx = false */
                                  hvar_plan.inner_maps.as_array ()))
        return_trace (ખોટું);

      જો (!out->varStore.serialize_serialize (c->serializer,
                                              item_vars.has_long_word (),
                                              c->plan->axis_tags,
                                              item_vars.get_region_list (),
                                              item_vars.get_vardata_encodings ()))
        return_trace (ખોટું);

      /* if varstore is optimized, remap output_map */
      જો (advMap)
      {
        જો (!hvar_plan.remap_index_map_plans (c->plan, item_vars.get_varidx_map ()))
          return_trace (ખોટું);
      }
    }
    નહીંતર
    {
      /* avar2 partial instancing: cull unreachable regions. */
      hb_set_t dead_regions;
      જો (c->plan->has_avar2)
	hvar_plan.var_store->collect_dead_regions (c->plan->axes_old_index_tag_map,
						   c->plan->avar2_reachable_ranges,
						   dead_regions);

      જો (unlikely (!out->varStore
		    .serialize_serialize (c->serializer,
					  hvar_plan.var_store,
					  hvar_plan.inner_maps.as_array (),
					  dead_regions.get_population () ? &dead_regions : nullptr)))
      return_trace (ખોટું);
    }

    return_trace (out->T::serialize_index_maps (c->serializer,
						hvar_plan.index_map_plans.as_array ()));
  }

  HB_ALWAYS_INLINE
  દશાંશ get_advance_delta_unscaled (hb_codepoint_t  glyph,
				    અચલ પૂર્ણાંક *coords, unsigned પૂર્ણાંક coord_ગણતરી,
				    hb_scalar_cache_t *store_cache = nullptr) અચલ
  {
    /* Implicit mapping uses outer index zero and the glyph id as the
     * inner index. An oversized glyph id must not select another table. */
    જો (!advMap && glyph > 0xFFFFu) પાછા 0.f;
    uint32_t varidx = (આ+advMap).map (glyph);
    પાછા (આ+varStore).get_delta (varidx,
				      coords, coord_ગણતરી,
				      store_cache);
  }

  જાહેર:
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

  જાહેર:
  DEFINE_SIZE_STATIC (20);
};

સંરચના HVAR : HVARVVAR {
  સ્થિર constexpr hb_tag_t tableTag = HB_OT_TAG_HVAR;
  તાર્કિક subset (hb_subset_context_t *c) અચલ { પાછા HVARVVAR::_subset<HVAR> (c); }
};
સંરચના VVAR : HVARVVAR {
  સ્થિર constexpr hb_tag_t tableTag = HB_OT_TAG_VVAR;

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (static_cast<અચલ HVARVVAR *> (આ)->sanitize (c) &&
		  vorgMap.sanitize (c, આ));
  }

  શૂન્ય listup_index_maps (hb_vector_t<અચલ DeltaSetIndexMap *> &index_maps) અચલ
  {
    HVARVVAR::listup_index_maps (index_maps);
    જો (vorgMap) index_maps.push (&(આ+vorgMap));
    નહીંતર index_maps.push (nullptr);
  }

  તાર્કિક serialize_index_maps (hb_serialize_context_t *c,
			     અચલ hb_array_t<index_map_subset_plan_t> &im_plans)
  {
    TRACE_SERIALIZE (આ);
    જો (unlikely (!HVARVVAR::serialize_index_maps (c, im_plans)))
      return_trace (ખોટું);
    જો (!im_plans[index_map_subset_plan_t::VORG_INDEX].get_map_ગણતરી ())
      vorgMap = 0;
    નહીંતર જો (unlikely (!vorgMap.serialize_serialize (c, im_plans[index_map_subset_plan_t::VORG_INDEX])))
      return_trace (ખોટું);

    return_trace (સાચું);
  }

  તાર્કિક subset (hb_subset_context_t *c) અચલ { પાછા HVARVVAR::_subset<VVAR> (c); }

  HB_ALWAYS_INLINE
  દશાંશ get_vorg_delta_unscaled (hb_codepoint_t glyph,
				 અચલ પૂર્ણાંક *coords, unsigned પૂર્ણાંક coord_ગણતરી,
				 hb_scalar_cache_t *store_cache = nullptr) અચલ
  {
    જો (!vorgMap) પાછા 0.f;
    uint32_t varidx = (આ+vorgMap).map (glyph);
    પાછા (આ+varStore).get_delta (varidx,
				      coords, coord_ગણતરી,
				      store_cache);
  }

  સુરક્ષિત:
  Offset32To<DeltaSetIndexMap>
		vorgMap;	/* Offset to vertical-origin var-idx mapping. */

  જાહેર:
  DEFINE_SIZE_STATIC (24);
};

} /* namespace OT */


#endif /* HB_OT_VAR_HVAR_TABLE_HH */
