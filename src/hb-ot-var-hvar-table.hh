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

#ضمن "hb-ot-layout-common.hh"
#ضمن "hb-ot-var-common.hh"

نطاق OT {


بنية index_map_subset_plan_t
{
  تعداد index_map_index_t {
    ADV_INDEX,
    LSB_INDEX,	/* dual as TSB */
    RSB_INDEX,	/* dual as BSB */
    VORG_INDEX
  };

  فراغ init (ثابت DeltaSetIndexMap  *index_map,
	     hb_inc_bimap_t	     &outer_map,
	     hb_vector_t<hb_set_t *> &inner_sets,
	     ثابت hb_subset_plan_t  *plan,
	     منطقي bypass_empty = true)
  {
    map_كدي = 0;
    full_map_كدي = plan->new_to_old_gid_list.length ?
		     plan->new_to_old_gid_list.tail ().first + 1 : 0;
    /* Only the advance mapping may be dropped in favor of implicit
     * glyph-id indexing (for the others a null offset means no deltas),
     * and only when the source was map-less too, preserving its form. */
    can_drop_to_implicit = !bypass_empty && !index_map;
    max_inners.init ();
    output_map.init ();

    إذا (bypass_empty && (!index_map || !index_map->get_map_كدي ())) أرجع;

    unsigned صحيح	last_val = (unsigned صحيح)-1;
    hb_codepoint_t	last_gid = HB_CODEPOINT_INVALID;

    max_inners.resize (inner_sets.length);
    لكل (unsigned i = 0; i < inner_sets.length; i++) max_inners[i] = 0;

    /* Search backwards for a map value different from the last map value */
    auto &new_to_old_gid_list = plan->new_to_old_gid_list;
    unsigned كدي = new_to_old_gid_list.length;
    إذا (!index_map)
    {
      map_كدي = new_to_old_gid_list.tail ().first + 1;
    }
    وإلا
    {
      لكل (unsigned j = كدي; j; j--)
      {
        hb_codepoint_t gid = new_to_old_gid_list.arrayZ[j - 1].first;
        hb_codepoint_t old_gid = new_to_old_gid_list.arrayZ[j - 1].second;
  
        unsigned صحيح v = index_map->map (old_gid);
        إذا (last_gid == HB_CODEPOINT_INVALID)
        {
  	last_val = v;
  	last_gid = gid;
  	استمر;
        }
        إذا (v != last_val)
  	اكسر;
  
        last_gid = gid;
      }
  
      إذا (unlikely (last_gid == (hb_codepoint_t)-1)) أرجع;
      map_كدي = last_gid + 1;
    }

    لكل (auto _ : plan->new_to_old_gid_list)
    {
      hb_codepoint_t gid = _.first;
      إذا (gid >= map_كدي) اكسر;

      hb_codepoint_t old_gid = _.second;
      unsigned صحيح v = index_map ? index_map->map (old_gid): old_gid;
      unsigned صحيح outer = v >> 16;
      unsigned صحيح inner = v & 0xFFFF;
      outer_map.add (outer);
      إذا (inner > max_inners[outer]) max_inners[outer] = inner;
      إذا (outer >= inner_sets.length) أرجع;
      inner_sets[outer]->add (inner);
    }
  }

  فراغ fini ()
  {
    max_inners.fini ();
    output_map.fini ();
  }

  فراغ remap (ثابت DeltaSetIndexMap *شغرو_map,
	      ثابت hb_inc_bimap_t &outer_map,
	      ثابت hb_vector_t<hb_inc_bimap_t> &inner_maps,
	      ثابت hb_subset_plan_t *plan)
  {
    outer_bit_كدي = 1;
    inner_bit_كدي = 1;

    لكل (unsigned صحيح i = 0; i < max_inners.length; i++)
    {
      إذا (inner_maps[i].get_population () == 0) استمر;
      unsigned صحيح bit_كدي = (max_inners[i]==0)? 1: hb_bit_storage (inner_maps[i][max_inners[i]]);
      إذا (bit_كدي > inner_bit_كدي) inner_bit_كدي = bit_كدي;
    }

    إذا (unlikely (!output_map.resize (map_كدي))) أرجع;
    لكل (ثابت auto &_ : plan->new_to_old_gid_list)
    {
      hb_codepoint_t new_gid = _.first;
      hb_codepoint_t old_gid = _.second;

      إذا (unlikely (new_gid >= map_كدي)) اكسر;

      uint32_t v = شغرو_map? شغرو_map->map (old_gid) : old_gid;
      unsigned outer = v >> 16;
      unsigned new_outer = outer_map[outer];
      unsigned bit_كدي = (new_outer == 0) ? 1 : hb_bit_storage (new_outer);
      outer_bit_كدي = hb_max (bit_كدي, outer_bit_كدي);

      output_map.arrayZ[new_gid] = (new_outer << 16) | (inner_maps[outer][v & 0xFFFF]);
    }
  }

  منطقي remap_after_instantiation (ثابت hb_subset_plan_t *plan,
                                  ثابت hb_map_t& varidx_map)
  {
    /* recalculate bit_count after remapping */
    outer_bit_كدي = 1;
    inner_bit_كدي = 1;

    لكل (ثابت auto &_ : plan->new_to_old_gid_list)
    {
      hb_codepoint_t new_gid = _.first;
      إذا (unlikely (new_gid >= map_كدي)) اكسر;

      uint32_t v = output_map.arrayZ[new_gid];
      uint32_t *new_varidx;
      إذا (!varidx_map.has (v, &new_varidx))
        أرجع خطأ;

      output_map.arrayZ[new_gid] = *new_varidx;

      unsigned outer = (*new_varidx) >> 16;
      unsigned bit_كدي = (outer == 0) ? 1 : hb_bit_storage (outer);
      outer_bit_كدي = hb_max (bit_كدي, outer_bit_كدي);

      unsigned inner = (*new_varidx) & 0xFFFF;
      bit_كدي = (inner == 0) ? 1 : hb_bit_storage (inner);
      inner_bit_كدي = hb_max (bit_كدي, inner_bit_كدي);
    }
    أرجع true;
  }

  unsigned صحيح get_inner_bit_كدي () ثابت { أرجع inner_bit_كدي; }
  unsigned صحيح get_width ()           ثابت { أرجع ((outer_bit_كدي + inner_bit_كدي + 7) / 8); }
  unsigned صحيح get_map_كدي ()       ثابت { أرجع map_كدي; }

  size_t get_size () ثابت
  {
    إذا (!map_كدي) أرجع 0;
    أرجع hb_unsigned_mul_add_saturate (get_width (), map_كدي,
					 DeltaSetIndexMap::min_size);
  }

  منطقي is_identity () ثابت
  {
    إذا (!output_map) أرجع true;
    /* An advance map whose entries are the identity over every output
     * glyph is equivalent to implicit glyph-id indexing; drop it.  A
     * truncated map doesn't qualify: glyphs past its end share its last
     * value, not their own ids. */
    إذا (!can_drop_to_implicit || map_كدي != full_map_كدي) أرجع خطأ;
    لكل (unsigned i = 0; i < map_كدي; i++)
      إذا (output_map.arrayZ[i] != i) أرجع خطأ;
    أرجع true;
  }
  hb_array_t<ثابت uint32_t> get_output_map () ثابت { أرجع output_map.as_array (); }

  محمي:
  unsigned صحيح map_كدي;
  unsigned صحيح full_map_كدي;
  منطقي can_drop_to_implicit;
  hb_vector_t<unsigned صحيح> max_inners;
  unsigned صحيح outer_bit_كدي;
  unsigned صحيح inner_bit_كدي;
  hb_vector_t<uint32_t> output_map;
};

بنية hvarvvar_subset_plan_t
{
  hvarvvar_subset_plan_t() : inner_maps (), index_map_plans () {}
  ~hvarvvar_subset_plan_t() { fini (); }

  فراغ init (ثابت hb_array_t<ثابت DeltaSetIndexMap *> &index_maps,
	     ثابت ItemVariationStore &_var_store,
	     ثابت hb_subset_plan_t *plan)
  {
    index_map_plans.resize (index_maps.length);

    var_store = &_var_store;
    inner_sets.resize (var_store->get_sub_table_كدي ());
    لكل (unsigned صحيح i = 0; i < inner_sets.length; i++)
      inner_sets[i] = hb_set_create ();
    adv_set = hb_set_create ();

    inner_maps.resize (var_store->get_sub_table_كدي ());

    إذا (unlikely (!index_map_plans.length || !inner_sets.length || !inner_maps.length)) أرجع;

    منطقي retain_adv_map = خطأ;
    index_map_plans[0].init (index_maps[0], outer_map, inner_sets, plan, خطأ);
    إذا (!index_maps[0])
    {
      retain_adv_map = plan->flags & HB_SUBSET_FLAGS_RETAIN_GIDS;
      outer_map.add (0);
      لكل (hb_codepoint_t old_gid : plan->glyphset()->iter())
        inner_sets[0]->add (old_gid);
      hb_set_union (adv_set, inner_sets[0]);
    }

    لكل (unsigned صحيح i = 1; i < index_maps.length; i++)
      index_map_plans[i].init (index_maps[i], outer_map, inner_sets, plan);

    outer_map.sort ();

    إذا (retain_adv_map)
    {
      لكل (ثابت auto &_ : plan->new_to_old_gid_list)
      {
        hb_codepoint_t old_gid = _.second;
	inner_maps[0].add (old_gid);
      }
    }
    وإلا
    {
      inner_maps[0].add_set (adv_set);
      hb_set_subtract (inner_sets[0], adv_set);
      inner_maps[0].add_set (inner_sets[0]);
    }

    لكل (unsigned صحيح i = 1; i < inner_maps.length; i++)
      inner_maps[i].add_set (inner_sets[i]);

    لكل (unsigned صحيح i = 0; i < index_maps.length; i++)
      index_map_plans[i].remap (index_maps[i], outer_map, inner_maps, plan);
  }

  /* remap */
  منطقي remap_index_map_plans (ثابت hb_subset_plan_t *plan,
                              ثابت hb_map_t& varidx_map)
  {
    لكل (unsigned i = 0; i < index_map_plans.length; i++)
      إذا (!index_map_plans[i].remap_after_instantiation (plan, varidx_map))
        أرجع خطأ;
    أرجع true;
  }

  فراغ fini ()
  {
    لكل (unsigned صحيح i = 0; i < inner_sets.length; i++)
      hb_set_destroy (inner_sets[i]);
    hb_set_destroy (adv_set);
    inner_maps.fini ();
    index_map_plans.fini ();
  }

  hb_inc_bimap_t outer_map;
  hb_vector_t<hb_inc_bimap_t> inner_maps;
  hb_vector_t<index_map_subset_plan_t> index_map_plans;
  ثابت ItemVariationStore *var_store;

  محمي:
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

بنية HVARVVAR
{
  ساكن constexpr hb_tag_t HVARTag = HB_OT_TAG_HVAR;
  ساكن constexpr hb_tag_t VVARTag = HB_OT_TAG_VVAR;

  منطقي has_data () ثابت { أرجع version.major != 0; }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (version.sanitize (c) &&
		  hb_barrier () &&
		  likely (version.major == 1) &&
		  varStore.sanitize (c, هذا) &&
		  advMap.sanitize (c, هذا) &&
		  lsbMap.sanitize (c, هذا) &&
		  rsbMap.sanitize (c, هذا));
  }

  ثابت ItemVariationStore& get_var_store () ثابت
  { أرجع هذا+varStore; }

  فراغ listup_index_maps (hb_vector_t<ثابت DeltaSetIndexMap *> &index_maps) ثابت
  {
    إذا (advMap) index_maps.push (&(هذا+advMap));
    وإلا index_maps.push (nullptr);

    إذا (lsbMap) index_maps.push (&(هذا+lsbMap));
    وإلا index_maps.push (nullptr);

    إذا (rsbMap) index_maps.push (&(هذا+rsbMap));
    وإلا index_maps.push (nullptr);
  }

  منطقي serialize_index_maps (hb_serialize_context_t *c,
			     ثابت hb_array_t<index_map_subset_plan_t> &im_plans)
  {
    TRACE_SERIALIZE (هذا);
    إذا (im_plans[index_map_subset_plan_t::ADV_INDEX].is_identity ())
      advMap = 0;
    وإلا إذا (unlikely (!advMap.serialize_serialize (c, im_plans[index_map_subset_plan_t::ADV_INDEX])))
      return_trace (خطأ);
    إذا (im_plans[index_map_subset_plan_t::LSB_INDEX].is_identity ())
      lsbMap = 0;
    وإلا إذا (unlikely (!lsbMap.serialize_serialize (c, im_plans[index_map_subset_plan_t::LSB_INDEX])))
      return_trace (خطأ);
    إذا (im_plans[index_map_subset_plan_t::RSB_INDEX].is_identity ())
      rsbMap = 0;
    وإلا إذا (unlikely (!rsbMap.serialize_serialize (c, im_plans[index_map_subset_plan_t::RSB_INDEX])))
      return_trace (خطأ);

    return_trace (true);
  }

  template <typename T>
  منطقي _subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    إذا (c->plan->all_axes_pinned)
      return_trace (خطأ);

    hvarvvar_subset_plan_t	hvar_plan;
    hb_vector_t<ثابت DeltaSetIndexMap *>
				index_maps;

    ((T*)هذا)->listup_index_maps (index_maps);
    hvar_plan.init (index_maps.as_array (), هذا+varStore, c->plan);

    T *out = c->serializer->allocate_min<T> ();
    إذا (unlikely (!out)) return_trace (خطأ);

    out->version.major = 1;
    out->version.minor = 0;

    إذا (c->plan->normalized_coords)
    {
      item_variations_t item_vars;
      إذا (!item_vars.instantiate (هذا+varStore, c->plan,
                                  advMap == 0 ? خطأ : true,
                                  خطأ, /* use_no_variation_idx = false */
                                  hvar_plan.inner_maps.as_array ()))
        return_trace (خطأ);

      إذا (!out->varStore.serialize_serialize (c->serializer,
                                              item_vars.has_long_word (),
                                              c->plan->axis_tags,
                                              item_vars.get_region_list (),
                                              item_vars.get_vardata_encodings ()))
        return_trace (خطأ);

      /* if varstore is optimized, remap output_map */
      إذا (advMap)
      {
        إذا (!hvar_plan.remap_index_map_plans (c->plan, item_vars.get_varidx_map ()))
          return_trace (خطأ);
      }
    }
    وإلا
    {
      /* avar2 partial instancing: cull unreachable regions. */
      hb_set_t dead_regions;
      إذا (c->plan->has_avar2)
	hvar_plan.var_store->collect_dead_regions (c->plan->axes_old_index_tag_map,
						   c->plan->avar2_reachable_ranges,
						   dead_regions);

      إذا (unlikely (!out->varStore
		    .serialize_serialize (c->serializer,
					  hvar_plan.var_store,
					  hvar_plan.inner_maps.as_array (),
					  dead_regions.get_population () ? &dead_regions : nullptr)))
      return_trace (خطأ);
    }

    return_trace (out->T::serialize_index_maps (c->serializer,
						hvar_plan.index_map_plans.as_array ()));
  }

  HB_ALWAYS_INLINE
  عائم get_advance_delta_unscaled (hb_codepoint_t  glyph,
				    ثابت صحيح *coords, unsigned صحيح coord_كدي,
				    hb_scalar_cache_t *store_cache = nullptr) ثابت
  {
    /* Implicit mapping uses outer index zero and the glyph id as the
     * inner index. An oversized glyph id must not select another table. */
    إذا (!advMap && glyph > 0xFFFFu) أرجع 0.f;
    uint32_t varidx = (هذا+advMap).map (glyph);
    أرجع (هذا+varStore).get_delta (varidx,
				      coords, coord_كدي,
				      store_cache);
  }

  عام:
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

  عام:
  DEFINE_SIZE_STATIC (20);
};

بنية HVAR : HVARVVAR {
  ساكن constexpr hb_tag_t tableTag = HB_OT_TAG_HVAR;
  منطقي subset (hb_subset_context_t *c) ثابت { أرجع HVARVVAR::_subset<HVAR> (c); }
};
بنية VVAR : HVARVVAR {
  ساكن constexpr hb_tag_t tableTag = HB_OT_TAG_VVAR;

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (static_cast<ثابت HVARVVAR *> (هذا)->sanitize (c) &&
		  vorgMap.sanitize (c, هذا));
  }

  فراغ listup_index_maps (hb_vector_t<ثابت DeltaSetIndexMap *> &index_maps) ثابت
  {
    HVARVVAR::listup_index_maps (index_maps);
    إذا (vorgMap) index_maps.push (&(هذا+vorgMap));
    وإلا index_maps.push (nullptr);
  }

  منطقي serialize_index_maps (hb_serialize_context_t *c,
			     ثابت hb_array_t<index_map_subset_plan_t> &im_plans)
  {
    TRACE_SERIALIZE (هذا);
    إذا (unlikely (!HVARVVAR::serialize_index_maps (c, im_plans)))
      return_trace (خطأ);
    إذا (!im_plans[index_map_subset_plan_t::VORG_INDEX].get_map_كدي ())
      vorgMap = 0;
    وإلا إذا (unlikely (!vorgMap.serialize_serialize (c, im_plans[index_map_subset_plan_t::VORG_INDEX])))
      return_trace (خطأ);

    return_trace (true);
  }

  منطقي subset (hb_subset_context_t *c) ثابت { أرجع HVARVVAR::_subset<VVAR> (c); }

  HB_ALWAYS_INLINE
  عائم get_vorg_delta_unscaled (hb_codepoint_t glyph,
				 ثابت صحيح *coords, unsigned صحيح coord_كدي,
				 hb_scalar_cache_t *store_cache = nullptr) ثابت
  {
    إذا (!vorgMap) أرجع 0.f;
    uint32_t varidx = (هذا+vorgMap).map (glyph);
    أرجع (هذا+varStore).get_delta (varidx,
				      coords, coord_كدي,
				      store_cache);
  }

  محمي:
  Offset32To<DeltaSetIndexMap>
		vorgMap;	/* Offset to vertical-origin var-idx mapping. */

  عام:
  DEFINE_SIZE_STATIC (24);
};

} /* namespace OT */


#endif /* HB_OT_VAR_HVAR_TABLE_HH */
