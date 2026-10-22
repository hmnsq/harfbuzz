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

#شامل "hb-ot-layout-common.hh"
#شامل "hb-ot-var-common.hh"

فضاینام OT {


ساختار index_map_subset_plan_t
{
  شمارش index_map_index_t {
    ADV_INDEX,
    LSB_INDEX,	/* dual as TSB */
    RSB_INDEX,	/* dual as BSB */
    VORG_INDEX
  };

  تهی init (ثابت DeltaSetIndexMap  *index_map,
	     hb_inc_bimap_t	     &outer_map,
	     hb_vector_t<hb_set_t *> &inner_sets,
	     ثابت hb_subset_plan_t  *plan,
	     منطقی bypass_empty = درست)
  {
    map_شمار = 0;
    full_map_شمار = plan->new_to_old_gid_list.length ?
		     plan->new_to_old_gid_list.tail ().first + 1 : 0;
    /* Only the advance mapping may be dropped in favor of implicit
     * glyph-id indexing (for the others a null offset means no deltas),
     * and only when the source was map-less too, preserving its form. */
    can_drop_to_implicit = !bypass_empty && !index_map;
    max_inners.init ();
    output_map.init ();

    اگر (bypass_empty && (!index_map || !index_map->get_map_شمار ())) بازگشت;

    unsigned صحیح	last_val = (unsigned صحیح)-1;
    hb_codepoint_t	last_gid = HB_CODEPOINT_INVALID;

    max_inners.resize (inner_sets.length);
    برای (unsigned i = 0; i < inner_sets.length; i++) max_inners[i] = 0;

    /* Search backwards for a map value different from the last map value */
    auto &new_to_old_gid_list = plan->new_to_old_gid_list;
    unsigned شمار = new_to_old_gid_list.length;
    اگر (!index_map)
    {
      map_شمار = new_to_old_gid_list.tail ().first + 1;
    }
    وگرنه
    {
      برای (unsigned j = شمار; j; j--)
      {
        hb_codepoint_t gid = new_to_old_gid_list.arrayZ[j - 1].first;
        hb_codepoint_t old_gid = new_to_old_gid_list.arrayZ[j - 1].second;
  
        unsigned صحیح v = index_map->map (old_gid);
        اگر (last_gid == HB_CODEPOINT_INVALID)
        {
  	last_val = v;
  	last_gid = gid;
  	ادامه;
        }
        اگر (v != last_val)
  	بشکن;
  
        last_gid = gid;
      }
  
      اگر (unlikely (last_gid == (hb_codepoint_t)-1)) بازگشت;
      map_شمار = last_gid + 1;
    }

    برای (auto _ : plan->new_to_old_gid_list)
    {
      hb_codepoint_t gid = _.first;
      اگر (gid >= map_شمار) بشکن;

      hb_codepoint_t old_gid = _.second;
      unsigned صحیح v = index_map ? index_map->map (old_gid): old_gid;
      unsigned صحیح outer = v >> 16;
      unsigned صحیح inner = v & 0xFFFF;
      outer_map.add (outer);
      اگر (inner > max_inners[outer]) max_inners[outer] = inner;
      اگر (outer >= inner_sets.length) بازگشت;
      inner_sets[outer]->add (inner);
    }
  }

  تهی fini ()
  {
    max_inners.fini ();
    output_map.fini ();
  }

  تهی remap (ثابت DeltaSetIndexMap *درون_داد_map,
	      ثابت hb_inc_bimap_t &outer_map,
	      ثابت hb_vector_t<hb_inc_bimap_t> &inner_maps,
	      ثابت hb_subset_plan_t *plan)
  {
    outer_bit_شمار = 1;
    inner_bit_شمار = 1;

    برای (unsigned صحیح i = 0; i < max_inners.length; i++)
    {
      اگر (inner_maps[i].get_population () == 0) ادامه;
      unsigned صحیح bit_شمار = (max_inners[i]==0)? 1: hb_bit_storage (inner_maps[i][max_inners[i]]);
      اگر (bit_شمار > inner_bit_شمار) inner_bit_شمار = bit_شمار;
    }

    اگر (unlikely (!output_map.resize (map_شمار))) بازگشت;
    برای (ثابت auto &_ : plan->new_to_old_gid_list)
    {
      hb_codepoint_t new_gid = _.first;
      hb_codepoint_t old_gid = _.second;

      اگر (unlikely (new_gid >= map_شمار)) بشکن;

      uint32_t v = درون_داد_map? درون_داد_map->map (old_gid) : old_gid;
      unsigned outer = v >> 16;
      unsigned new_outer = outer_map[outer];
      unsigned bit_شمار = (new_outer == 0) ? 1 : hb_bit_storage (new_outer);
      outer_bit_شمار = hb_max (bit_شمار, outer_bit_شمار);

      output_map.arrayZ[new_gid] = (new_outer << 16) | (inner_maps[outer][v & 0xFFFF]);
    }
  }

  منطقی remap_after_instantiation (ثابت hb_subset_plan_t *plan,
                                  ثابت hb_map_t& varidx_map)
  {
    /* recalculate bit_count after remapping */
    outer_bit_شمار = 1;
    inner_bit_شمار = 1;

    برای (ثابت auto &_ : plan->new_to_old_gid_list)
    {
      hb_codepoint_t new_gid = _.first;
      اگر (unlikely (new_gid >= map_شمار)) بشکن;

      uint32_t v = output_map.arrayZ[new_gid];
      uint32_t *new_varidx;
      اگر (!varidx_map.has (v, &new_varidx))
        بازگشت نادرست;

      output_map.arrayZ[new_gid] = *new_varidx;

      unsigned outer = (*new_varidx) >> 16;
      unsigned bit_شمار = (outer == 0) ? 1 : hb_bit_storage (outer);
      outer_bit_شمار = hb_max (bit_شمار, outer_bit_شمار);

      unsigned inner = (*new_varidx) & 0xFFFF;
      bit_شمار = (inner == 0) ? 1 : hb_bit_storage (inner);
      inner_bit_شمار = hb_max (bit_شمار, inner_bit_شمار);
    }
    بازگشت درست;
  }

  unsigned صحیح get_inner_bit_شمار () ثابت { بازگشت inner_bit_شمار; }
  unsigned صحیح get_width ()           ثابت { بازگشت ((outer_bit_شمار + inner_bit_شمار + 7) / 8); }
  unsigned صحیح get_map_شمار ()       ثابت { بازگشت map_شمار; }

  size_t get_size () ثابت
  {
    اگر (!map_شمار) بازگشت 0;
    بازگشت hb_unsigned_mul_add_saturate (get_width (), map_شمار,
					 DeltaSetIndexMap::min_size);
  }

  منطقی is_identity () ثابت
  {
    اگر (!output_map) بازگشت درست;
    /* An advance map whose entries are the identity over every output
     * glyph is equivalent to implicit glyph-id indexing; drop it.  A
     * truncated map doesn't qualify: glyphs past its end share its last
     * value, not their own ids. */
    اگر (!can_drop_to_implicit || map_شمار != full_map_شمار) بازگشت نادرست;
    برای (unsigned i = 0; i < map_شمار; i++)
      اگر (output_map.arrayZ[i] != i) بازگشت نادرست;
    بازگشت درست;
  }
  hb_array_t<ثابت uint32_t> get_output_map () ثابت { بازگشت output_map.as_array (); }

  محافظتشده:
  unsigned صحیح map_شمار;
  unsigned صحیح full_map_شمار;
  منطقی can_drop_to_implicit;
  hb_vector_t<unsigned صحیح> max_inners;
  unsigned صحیح outer_bit_شمار;
  unsigned صحیح inner_bit_شمار;
  hb_vector_t<uint32_t> output_map;
};

ساختار hvarvvar_subset_plan_t
{
  hvarvvar_subset_plan_t() : inner_maps (), index_map_plans () {}
  ~hvarvvar_subset_plan_t() { fini (); }

  تهی init (ثابت hb_array_t<ثابت DeltaSetIndexMap *> &index_maps,
	     ثابت ItemVariationStore &_var_store,
	     ثابت hb_subset_plan_t *plan)
  {
    index_map_plans.resize (index_maps.length);

    var_store = &_var_store;
    inner_sets.resize (var_store->get_sub_table_شمار ());
    برای (unsigned صحیح i = 0; i < inner_sets.length; i++)
      inner_sets[i] = hb_set_create ();
    adv_set = hb_set_create ();

    inner_maps.resize (var_store->get_sub_table_شمار ());

    اگر (unlikely (!index_map_plans.length || !inner_sets.length || !inner_maps.length)) بازگشت;

    منطقی retain_adv_map = نادرست;
    index_map_plans[0].init (index_maps[0], outer_map, inner_sets, plan, نادرست);
    اگر (!index_maps[0])
    {
      retain_adv_map = plan->flags & HB_SUBSET_FLAGS_RETAIN_GIDS;
      outer_map.add (0);
      برای (hb_codepoint_t old_gid : plan->glyphset()->iter())
        inner_sets[0]->add (old_gid);
      hb_set_union (adv_set, inner_sets[0]);
    }

    برای (unsigned صحیح i = 1; i < index_maps.length; i++)
      index_map_plans[i].init (index_maps[i], outer_map, inner_sets, plan);

    outer_map.sort ();

    اگر (retain_adv_map)
    {
      برای (ثابت auto &_ : plan->new_to_old_gid_list)
      {
        hb_codepoint_t old_gid = _.second;
	inner_maps[0].add (old_gid);
      }
    }
    وگرنه
    {
      inner_maps[0].add_set (adv_set);
      hb_set_subtract (inner_sets[0], adv_set);
      inner_maps[0].add_set (inner_sets[0]);
    }

    برای (unsigned صحیح i = 1; i < inner_maps.length; i++)
      inner_maps[i].add_set (inner_sets[i]);

    برای (unsigned صحیح i = 0; i < index_maps.length; i++)
      index_map_plans[i].remap (index_maps[i], outer_map, inner_maps, plan);
  }

  /* remap */
  منطقی remap_index_map_plans (ثابت hb_subset_plan_t *plan,
                              ثابت hb_map_t& varidx_map)
  {
    برای (unsigned i = 0; i < index_map_plans.length; i++)
      اگر (!index_map_plans[i].remap_after_instantiation (plan, varidx_map))
        بازگشت نادرست;
    بازگشت درست;
  }

  تهی fini ()
  {
    برای (unsigned صحیح i = 0; i < inner_sets.length; i++)
      hb_set_destroy (inner_sets[i]);
    hb_set_destroy (adv_set);
    inner_maps.fini ();
    index_map_plans.fini ();
  }

  hb_inc_bimap_t outer_map;
  hb_vector_t<hb_inc_bimap_t> inner_maps;
  hb_vector_t<index_map_subset_plan_t> index_map_plans;
  ثابت ItemVariationStore *var_store;

  محافظتشده:
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

ساختار HVARVVAR
{
  ایستا constexpr hb_tag_t HVARTag = HB_OT_TAG_HVAR;
  ایستا constexpr hb_tag_t VVARTag = HB_OT_TAG_VVAR;

  منطقی has_data () ثابت { بازگشت version.major != 0; }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (version.sanitize (c) &&
		  hb_barrier () &&
		  likely (version.major == 1) &&
		  varStore.sanitize (c, این) &&
		  advMap.sanitize (c, این) &&
		  lsbMap.sanitize (c, این) &&
		  rsbMap.sanitize (c, این));
  }

  ثابت ItemVariationStore& get_var_store () ثابت
  { بازگشت این+varStore; }

  تهی listup_index_maps (hb_vector_t<ثابت DeltaSetIndexMap *> &index_maps) ثابت
  {
    اگر (advMap) index_maps.push (&(این+advMap));
    وگرنه index_maps.push (nullptr);

    اگر (lsbMap) index_maps.push (&(این+lsbMap));
    وگرنه index_maps.push (nullptr);

    اگر (rsbMap) index_maps.push (&(این+rsbMap));
    وگرنه index_maps.push (nullptr);
  }

  منطقی serialize_index_maps (hb_serialize_context_t *c,
			     ثابت hb_array_t<index_map_subset_plan_t> &im_plans)
  {
    TRACE_SERIALIZE (این);
    اگر (im_plans[index_map_subset_plan_t::ADV_INDEX].is_identity ())
      advMap = 0;
    وگرنه اگر (unlikely (!advMap.serialize_serialize (c, im_plans[index_map_subset_plan_t::ADV_INDEX])))
      return_trace (نادرست);
    اگر (im_plans[index_map_subset_plan_t::LSB_INDEX].is_identity ())
      lsbMap = 0;
    وگرنه اگر (unlikely (!lsbMap.serialize_serialize (c, im_plans[index_map_subset_plan_t::LSB_INDEX])))
      return_trace (نادرست);
    اگر (im_plans[index_map_subset_plan_t::RSB_INDEX].is_identity ())
      rsbMap = 0;
    وگرنه اگر (unlikely (!rsbMap.serialize_serialize (c, im_plans[index_map_subset_plan_t::RSB_INDEX])))
      return_trace (نادرست);

    return_trace (درست);
  }

  template <typename T>
  منطقی _subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (این);
    اگر (c->plan->all_axes_pinned)
      return_trace (نادرست);

    hvarvvar_subset_plan_t	hvar_plan;
    hb_vector_t<ثابت DeltaSetIndexMap *>
				index_maps;

    ((T*)این)->listup_index_maps (index_maps);
    hvar_plan.init (index_maps.as_array (), این+varStore, c->plan);

    T *out = c->serializer->allocate_min<T> ();
    اگر (unlikely (!out)) return_trace (نادرست);

    out->version.major = 1;
    out->version.minor = 0;

    اگر (c->plan->normalized_coords)
    {
      item_variations_t item_vars;
      اگر (!item_vars.instantiate (این+varStore, c->plan,
                                  advMap == 0 ? نادرست : درست,
                                  نادرست, /* use_no_variation_idx = false */
                                  hvar_plan.inner_maps.as_array ()))
        return_trace (نادرست);

      اگر (!out->varStore.serialize_serialize (c->serializer,
                                              item_vars.has_long_word (),
                                              c->plan->axis_tags,
                                              item_vars.get_region_list (),
                                              item_vars.get_vardata_encodings ()))
        return_trace (نادرست);

      /* if varstore is optimized, remap output_map */
      اگر (advMap)
      {
        اگر (!hvar_plan.remap_index_map_plans (c->plan, item_vars.get_varidx_map ()))
          return_trace (نادرست);
      }
    }
    وگرنه
    {
      /* avar2 partial instancing: cull unreachable regions. */
      hb_set_t dead_regions;
      اگر (c->plan->has_avar2)
	hvar_plan.var_store->collect_dead_regions (c->plan->axes_old_index_tag_map,
						   c->plan->avar2_reachable_ranges,
						   dead_regions);

      اگر (unlikely (!out->varStore
		    .serialize_serialize (c->serializer,
					  hvar_plan.var_store,
					  hvar_plan.inner_maps.as_array (),
					  dead_regions.get_population () ? &dead_regions : nullptr)))
      return_trace (نادرست);
    }

    return_trace (out->T::serialize_index_maps (c->serializer,
						hvar_plan.index_map_plans.as_array ()));
  }

  HB_ALWAYS_INLINE
  اعشاری get_advance_delta_unscaled (hb_codepoint_t  glyph,
				    ثابت صحیح *coords, unsigned صحیح coord_شمار,
				    hb_scalar_cache_t *store_cache = nullptr) ثابت
  {
    /* Implicit mapping uses outer index zero and the glyph id as the
     * inner index. An oversized glyph id must not select another table. */
    اگر (!advMap && glyph > 0xFFFFu) بازگشت 0.f;
    uint32_t varidx = (این+advMap).map (glyph);
    بازگشت (این+varStore).get_delta (varidx,
				      coords, coord_شمار,
				      store_cache);
  }

  عمومی:
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

  عمومی:
  DEFINE_SIZE_STATIC (20);
};

ساختار HVAR : HVARVVAR {
  ایستا constexpr hb_tag_t tableTag = HB_OT_TAG_HVAR;
  منطقی subset (hb_subset_context_t *c) ثابت { بازگشت HVARVVAR::_subset<HVAR> (c); }
};
ساختار VVAR : HVARVVAR {
  ایستا constexpr hb_tag_t tableTag = HB_OT_TAG_VVAR;

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (static_cast<ثابت HVARVVAR *> (این)->sanitize (c) &&
		  vorgMap.sanitize (c, این));
  }

  تهی listup_index_maps (hb_vector_t<ثابت DeltaSetIndexMap *> &index_maps) ثابت
  {
    HVARVVAR::listup_index_maps (index_maps);
    اگر (vorgMap) index_maps.push (&(این+vorgMap));
    وگرنه index_maps.push (nullptr);
  }

  منطقی serialize_index_maps (hb_serialize_context_t *c,
			     ثابت hb_array_t<index_map_subset_plan_t> &im_plans)
  {
    TRACE_SERIALIZE (این);
    اگر (unlikely (!HVARVVAR::serialize_index_maps (c, im_plans)))
      return_trace (نادرست);
    اگر (!im_plans[index_map_subset_plan_t::VORG_INDEX].get_map_شمار ())
      vorgMap = 0;
    وگرنه اگر (unlikely (!vorgMap.serialize_serialize (c, im_plans[index_map_subset_plan_t::VORG_INDEX])))
      return_trace (نادرست);

    return_trace (درست);
  }

  منطقی subset (hb_subset_context_t *c) ثابت { بازگشت HVARVVAR::_subset<VVAR> (c); }

  HB_ALWAYS_INLINE
  اعشاری get_vorg_delta_unscaled (hb_codepoint_t glyph,
				 ثابت صحیح *coords, unsigned صحیح coord_شمار,
				 hb_scalar_cache_t *store_cache = nullptr) ثابت
  {
    اگر (!vorgMap) بازگشت 0.f;
    uint32_t varidx = (این+vorgMap).map (glyph);
    بازگشت (این+varStore).get_delta (varidx,
				      coords, coord_شمار,
				      store_cache);
  }

  محافظتشده:
  Offset32To<DeltaSetIndexMap>
		vorgMap;	/* Offset to vertical-origin var-idx mapping. */

  عمومی:
  DEFINE_SIZE_STATIC (24);
};

} /* namespace OT */


#endif /* HB_OT_VAR_HVAR_TABLE_HH */
