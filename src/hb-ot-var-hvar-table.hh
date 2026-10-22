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

#جيڪڏهن_وصف_نه_ٿيل HB_OT_VAR_HVAR_جدول_HH
#وصف_ڪريو HB_OT_VAR_HVAR_جدول_HH

#ضمن "hb-ot-layout-common.hh"
#ضمن "hb-ot-var-common.hh"

نطاق OT {


بنية اشاريو_map_subset_plan_t
{
  تعداد اشاريو_map_اشاريو_t {
    ADV_اشاريو,
    LSB_اشاريو,	/* dual as TSB */
    RSB_اشاريو,	/* dual as BSB */
    VORG_اشاريو
  };

  فراغ init (ثابت Delta_سيٽ_اشاريو_Map  *اشاريو_map,
	     hb_inc_bimap_t	     &outer_map,
	     hb_ويڪٽر_t<hb_سيٽ_t *> &inner_sets,
	     ثابت hb_subset_plan_t  *plan,
	     منطقي bypass_خالي_جايون = سچ)
  {
    map_ڳڻپ = 0;
    ڀريل_map_ڳڻپ = plan->new_منزل_old_gid_list.ڊيگهائي ?
		     plan->new_منزل_old_gid_list.پڇ ().پهريون + 1 : 0;
    /* Only the advance mapping may be dropped in favor of implicit
     * glyph-id indexing (for the others a null offset means no deltas),
     * and only when the source was map-less too, preserving its form. */
    can_drop_منزل_implicit = !bypass_خالي_جايون && !اشاريو_map;
    max_inners.init ();
    output_map.init ();

    إذا (bypass_خالي_جايون && (!اشاريو_map || !اشاريو_map->get_map_ڳڻپ ())) أرجع;

    بنانشان صحيح	آخري_val = (بنانشان صحيح)-1;
    hb_codepoint_t	آخري_gid = HB_CODEPOINT_INVALID;

    max_inners.resize (inner_sets.ڊيگهائي);
    لكل (بنانشان i = 0; i < inner_sets.ڊيگهائي; i++) max_inners[i] = 0;

    /* Search backwards for a map value different from the last map value */
    خودڪار &new_منزل_old_gid_list = plan->new_منزل_old_gid_list;
    بنانشان ڳڻپ = new_منزل_old_gid_list.ڊيگهائي;
    إذا (!اشاريو_map)
    {
      map_ڳڻپ = new_منزل_old_gid_list.پڇ ().پهريون + 1;
    }
    وإلا
    {
      لكل (بنانشان j = ڳڻپ; j; j--)
      {
        hb_codepoint_t gid = new_منزل_old_gid_list.arrayZ[j - 1].پهريون;
        hb_codepoint_t old_gid = new_منزل_old_gid_list.arrayZ[j - 1].second;
  
        بنانشان صحيح v = اشاريو_map->map (old_gid);
        إذا (آخري_gid == HB_CODEPOINT_INVALID)
        {
  	آخري_val = v;
  	آخري_gid = gid;
  	استمر;
        }
        إذا (v != آخري_val)
  	اكسر;
  
        آخري_gid = gid;
      }
  
      إذا (unlikely (آخري_gid == (hb_codepoint_t)-1)) أرجع;
      map_ڳڻپ = آخري_gid + 1;
    }

    لكل (خودڪار _ : plan->new_منزل_old_gid_list)
    {
      hb_codepoint_t gid = _.پهريون;
      إذا (gid >= map_ڳڻپ) اكسر;

      hb_codepoint_t old_gid = _.second;
      بنانشان صحيح v = اشاريو_map ? اشاريو_map->map (old_gid): old_gid;
      بنانشان صحيح outer = v >> 16;
      بنانشان صحيح inner = v & 0xFFFF;
      outer_map.جوڙيو (outer);
      إذا (inner > max_inners[outer]) max_inners[outer] = inner;
      إذا (outer >= inner_sets.ڊيگهائي) أرجع;
      inner_sets[outer]->جوڙيو (inner);
    }
  }

  فراغ fini ()
  {
    max_inners.fini ();
    output_map.fini ();
  }

  فراغ remap (ثابت Delta_سيٽ_اشاريو_Map *داخلا_map,
	      ثابت hb_inc_bimap_t &outer_map,
	      ثابت hb_ويڪٽر_t<hb_inc_bimap_t> &inner_maps,
	      ثابت hb_subset_plan_t *plan)
  {
    outer_ٻٽو_انگ_ڳڻپ = 1;
    inner_ٻٽو_انگ_ڳڻپ = 1;

    لكل (بنانشان صحيح i = 0; i < max_inners.ڊيگهائي; i++)
    {
      إذا (inner_maps[i].get_population () == 0) استمر;
      بنانشان صحيح ٻٽو_انگ_ڳڻپ = (max_inners[i]==0)? 1: hb_ٻٽو_انگ_storage (inner_maps[i][max_inners[i]]);
      إذا (ٻٽو_انگ_ڳڻپ > inner_ٻٽو_انگ_ڳڻپ) inner_ٻٽو_انگ_ڳڻپ = ٻٽو_انگ_ڳڻپ;
    }

    إذا (unlikely (!output_map.resize (map_ڳڻپ))) أرجع;
    لكل (ثابت خودڪار &_ : plan->new_منزل_old_gid_list)
    {
      hb_codepoint_t new_gid = _.پهريون;
      hb_codepoint_t old_gid = _.second;

      إذا (unlikely (new_gid >= map_ڳڻپ)) اكسر;

      uint32_t v = داخلا_map? داخلا_map->map (old_gid) : old_gid;
      بنانشان outer = v >> 16;
      بنانشان new_outer = outer_map[outer];
      بنانشان ٻٽو_انگ_ڳڻپ = (new_outer == 0) ? 1 : hb_ٻٽو_انگ_storage (new_outer);
      outer_ٻٽو_انگ_ڳڻپ = hb_max (ٻٽو_انگ_ڳڻپ, outer_ٻٽو_انگ_ڳڻپ);

      output_map.arrayZ[new_gid] = (new_outer << 16) | (inner_maps[outer][v & 0xFFFF]);
    }
  }

  منطقي remap_after_instantiation (ثابت hb_subset_plan_t *plan,
                                  ثابت hb_map_t& varidx_map)
  {
    /* recalculate bit_count after remapping */
    outer_ٻٽو_انگ_ڳڻپ = 1;
    inner_ٻٽو_انگ_ڳڻپ = 1;

    لكل (ثابت خودڪار &_ : plan->new_منزل_old_gid_list)
    {
      hb_codepoint_t new_gid = _.پهريون;
      إذا (unlikely (new_gid >= map_ڳڻپ)) اكسر;

      uint32_t v = output_map.arrayZ[new_gid];
      uint32_t *new_varidx;
      إذا (!varidx_map.has (v, &new_varidx))
        أرجع خطأ;

      output_map.arrayZ[new_gid] = *new_varidx;

      بنانشان outer = (*new_varidx) >> 16;
      بنانشان ٻٽو_انگ_ڳڻپ = (outer == 0) ? 1 : hb_ٻٽو_انگ_storage (outer);
      outer_ٻٽو_انگ_ڳڻپ = hb_max (ٻٽو_انگ_ڳڻپ, outer_ٻٽو_انگ_ڳڻپ);

      بنانشان inner = (*new_varidx) & 0xFFFF;
      ٻٽو_انگ_ڳڻپ = (inner == 0) ? 1 : hb_ٻٽو_انگ_storage (inner);
      inner_ٻٽو_انگ_ڳڻپ = hb_max (ٻٽو_انگ_ڳڻپ, inner_ٻٽو_انگ_ڳڻپ);
    }
    أرجع سچ;
  }

  بنانشان صحيح get_inner_ٻٽو_انگ_ڳڻپ () ثابت { أرجع inner_ٻٽو_انگ_ڳڻپ; }
  بنانشان صحيح get_ويڪر ()           ثابت { أرجع ((outer_ٻٽو_انگ_ڳڻپ + inner_ٻٽو_انگ_ڳڻپ + 7) / 8); }
  بنانشان صحيح get_map_ڳڻپ ()       ثابت { أرجع map_ڳڻپ; }

  size_t get_سائيز () ثابت
  {
    إذا (!map_ڳڻپ) أرجع 0;
    أرجع hb_unsigned_mul_جوڙيو_saturate (get_ويڪر (), map_ڳڻپ,
					 Delta_سيٽ_اشاريو_Map::min_سائيز);
  }

  منطقي is_سڃاڻپ_عنصر () ثابت
  {
    إذا (!output_map) أرجع سچ;
    /* An advance map whose entries are the identity over every output
     * glyph is equivalent to implicit glyph-id indexing; drop it.  A
     * truncated map doesn't qualify: glyphs past its end share its last
     * value, not their own ids. */
    إذا (!can_drop_منزل_implicit || map_ڳڻپ != ڀريل_map_ڳڻپ) أرجع خطأ;
    لكل (بنانشان i = 0; i < map_ڳڻپ; i++)
      إذا (output_map.arrayZ[i] != i) أرجع خطأ;
    أرجع سچ;
  }
  hb_array_t<ثابت uint32_t> get_output_map () ثابت { أرجع output_map.as_array (); }

  محمي:
  بنانشان صحيح map_ڳڻپ;
  بنانشان صحيح ڀريل_map_ڳڻپ;
  منطقي can_drop_منزل_implicit;
  hb_ويڪٽر_t<بنانشان صحيح> max_inners;
  بنانشان صحيح outer_ٻٽو_انگ_ڳڻپ;
  بنانشان صحيح inner_ٻٽو_انگ_ڳڻپ;
  hb_ويڪٽر_t<uint32_t> output_map;
};

بنية hvarvvar_subset_plan_t
{
  hvarvvar_subset_plan_t() : inner_maps (), اشاريو_map_plans () {}
  ~hvarvvar_subset_plan_t() { fini (); }

  فراغ init (ثابت hb_array_t<ثابت Delta_سيٽ_اشاريو_Map *> &اشاريو_maps,
	     ثابت شي_Variation_Store &_var_store,
	     ثابت hb_subset_plan_t *plan)
  {
    اشاريو_map_plans.resize (اشاريو_maps.ڊيگهائي);

    var_store = &_var_store;
    inner_sets.resize (var_store->get_sub_جدول_ڳڻپ ());
    لكل (بنانشان صحيح i = 0; i < inner_sets.ڊيگهائي; i++)
      inner_sets[i] = hb_سيٽ_create ();
    adv_سيٽ = hb_سيٽ_create ();

    inner_maps.resize (var_store->get_sub_جدول_ڳڻپ ());

    إذا (unlikely (!اشاريو_map_plans.ڊيگهائي || !inner_sets.ڊيگهائي || !inner_maps.ڊيگهائي)) أرجع;

    منطقي retain_adv_map = خطأ;
    اشاريو_map_plans[0].init (اشاريو_maps[0], outer_map, inner_sets, plan, خطأ);
    إذا (!اشاريو_maps[0])
    {
      retain_adv_map = plan->flags & HB_SUBSET_FLAGS_RETAIN_GIDS;
      outer_map.جوڙيو (0);
      لكل (hb_codepoint_t old_gid : plan->glyphset()->iter())
        inner_sets[0]->جوڙيو (old_gid);
      hb_سيٽ_union (adv_سيٽ, inner_sets[0]);
    }

    لكل (بنانشان صحيح i = 1; i < اشاريو_maps.ڊيگهائي; i++)
      اشاريو_map_plans[i].init (اشاريو_maps[i], outer_map, inner_sets, plan);

    outer_map.sort ();

    إذا (retain_adv_map)
    {
      لكل (ثابت خودڪار &_ : plan->new_منزل_old_gid_list)
      {
        hb_codepoint_t old_gid = _.second;
	inner_maps[0].جوڙيو (old_gid);
      }
    }
    وإلا
    {
      inner_maps[0].جوڙيو_سيٽ (adv_سيٽ);
      hb_سيٽ_ڪٽيو (inner_sets[0], adv_سيٽ);
      inner_maps[0].جوڙيو_سيٽ (inner_sets[0]);
    }

    لكل (بنانشان صحيح i = 1; i < inner_maps.ڊيگهائي; i++)
      inner_maps[i].جوڙيو_سيٽ (inner_sets[i]);

    لكل (بنانشان صحيح i = 0; i < اشاريو_maps.ڊيگهائي; i++)
      اشاريو_map_plans[i].remap (اشاريو_maps[i], outer_map, inner_maps, plan);
  }

  /* remap */
  منطقي remap_اشاريو_map_plans (ثابت hb_subset_plan_t *plan,
                              ثابت hb_map_t& varidx_map)
  {
    لكل (بنانشان i = 0; i < اشاريو_map_plans.ڊيگهائي; i++)
      إذا (!اشاريو_map_plans[i].remap_after_instantiation (plan, varidx_map))
        أرجع خطأ;
    أرجع سچ;
  }

  فراغ fini ()
  {
    لكل (بنانشان صحيح i = 0; i < inner_sets.ڊيگهائي; i++)
      hb_سيٽ_destroy (inner_sets[i]);
    hb_سيٽ_destroy (adv_سيٽ);
    inner_maps.fini ();
    اشاريو_map_plans.fini ();
  }

  hb_inc_bimap_t outer_map;
  hb_ويڪٽر_t<hb_inc_bimap_t> inner_maps;
  hb_ويڪٽر_t<اشاريو_map_subset_plan_t> اشاريو_map_plans;
  ثابت شي_Variation_Store *var_store;

  محمي:
  hb_ويڪٽر_t<hb_سيٽ_t *> inner_sets;
  hb_سيٽ_t *adv_سيٽ;
};

/*
 * HVAR -- Horizontal Metrics Variations
 * https://docs.microsoft.com/en-us/typography/opentype/spec/hvar
 * VVAR -- Vertical Metrics Variations
 * https://docs.microsoft.com/en-us/typography/opentype/spec/vvar
 */
#وصف_ڪريو HB_OT_ٽيگ_HVAR HB_ٽيگ('H','V','A','R')
#وصف_ڪريو HB_OT_ٽيگ_VVAR HB_ٽيگ('V','V','A','R')

بنية HVARVVAR
{
  ساكن constexpr hb_ٽيگ_t HVARTag = HB_OT_ٽيگ_HVAR;
  ساكن constexpr hb_ٽيگ_t VVARTag = HB_OT_ٽيگ_VVAR;

  منطقي has_معلومات () ثابت { أرجع نسخو.major != 0; }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (نسخو.sanitize (c) &&
		  hb_barrier () &&
		  likely (نسخو.major == 1) &&
		  varStore.sanitize (c, هذا) &&
		  advMap.sanitize (c, هذا) &&
		  lsbMap.sanitize (c, هذا) &&
		  rsbMap.sanitize (c, هذا));
  }

  ثابت شي_Variation_Store& get_var_store () ثابت
  { أرجع هذا+varStore; }

  فراغ listup_اشاريو_maps (hb_ويڪٽر_t<ثابت Delta_سيٽ_اشاريو_Map *> &اشاريو_maps) ثابت
  {
    إذا (advMap) اشاريو_maps.push (&(هذا+advMap));
    وإلا اشاريو_maps.push (خالي_اشارو);

    إذا (lsbMap) اشاريو_maps.push (&(هذا+lsbMap));
    وإلا اشاريو_maps.push (خالي_اشارو);

    إذا (rsbMap) اشاريو_maps.push (&(هذا+rsbMap));
    وإلا اشاريو_maps.push (خالي_اشارو);
  }

  منطقي serialize_اشاريو_maps (hb_serialize_context_t *c,
			     ثابت hb_array_t<اشاريو_map_subset_plan_t> &im_plans)
  {
    TRACE_SERIALIZE (هذا);
    إذا (im_plans[اشاريو_map_subset_plan_t::ADV_اشاريو].is_سڃاڻپ_عنصر ())
      advMap = 0;
    وإلا إذا (unlikely (!advMap.serialize_serialize (c, im_plans[اشاريو_map_subset_plan_t::ADV_اشاريو])))
      return_trace (خطأ);
    إذا (im_plans[اشاريو_map_subset_plan_t::LSB_اشاريو].is_سڃاڻپ_عنصر ())
      lsbMap = 0;
    وإلا إذا (unlikely (!lsbMap.serialize_serialize (c, im_plans[اشاريو_map_subset_plan_t::LSB_اشاريو])))
      return_trace (خطأ);
    إذا (im_plans[اشاريو_map_subset_plan_t::RSB_اشاريو].is_سڃاڻپ_عنصر ())
      rsbMap = 0;
    وإلا إذا (unlikely (!rsbMap.serialize_serialize (c, im_plans[اشاريو_map_subset_plan_t::RSB_اشاريو])))
      return_trace (خطأ);

    return_trace (سچ);
  }

  سانچو <قسم_نالو T>
  منطقي _subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    إذا (c->plan->all_axes_pinned)
      return_trace (خطأ);

    hvarvvar_subset_plan_t	hvar_plan;
    hb_ويڪٽر_t<ثابت Delta_سيٽ_اشاريو_Map *>
				اشاريو_maps;

    ((T*)هذا)->listup_اشاريو_maps (اشاريو_maps);
    hvar_plan.init (اشاريو_maps.as_array (), هذا+varStore, c->plan);

    T *ٻاهريون = c->serializer->allocate_min<T> ();
    إذا (unlikely (!ٻاهريون)) return_trace (خطأ);

    ٻاهريون->نسخو.major = 1;
    ٻاهريون->نسخو.minor = 0;

    إذا (c->plan->normalized_coords)
    {
      شي_variations_t شي_vars;
      إذا (!شي_vars.instantiate (هذا+varStore, c->plan,
                                  advMap == 0 ? خطأ : سچ,
                                  خطأ, /* use_no_variation_idx = false */
                                  hvar_plan.inner_maps.as_array ()))
        return_trace (خطأ);

      إذا (!ٻاهريون->varStore.serialize_serialize (c->serializer,
                                              شي_vars.has_long_لفظ (),
                                              c->plan->axis_ٽيگ_فهرست,
                                              شي_vars.get_region_list (),
                                              شي_vars.get_vardata_encodings ()))
        return_trace (خطأ);

      /* if varstore is optimized, remap output_map */
      إذا (advMap)
      {
        إذا (!hvar_plan.remap_اشاريو_map_plans (c->plan, شي_vars.get_varidx_map ()))
          return_trace (خطأ);
      }
    }
    وإلا
    {
      /* avar2 partial instancing: cull unreachable regions. */
      hb_سيٽ_t dead_regions;
      إذا (c->plan->has_avar2)
	hvar_plan.var_store->collect_dead_regions (c->plan->axes_old_اشاريو_ٽيگ_map,
						   c->plan->avar2_reachable_ranges,
						   dead_regions);

      إذا (unlikely (!ٻاهريون->varStore
		    .serialize_serialize (c->serializer,
					  hvar_plan.var_store,
					  hvar_plan.inner_maps.as_array (),
					  dead_regions.get_population () ? &dead_regions : خالي_اشارو)))
      return_trace (خطأ);
    }

    return_trace (ٻاهريون->T::serialize_اشاريو_maps (c->serializer,
						hvar_plan.اشاريو_map_plans.as_array ()));
  }

  HB_ALWAYS_INLINE
  عائم get_advance_delta_unscaled (hb_codepoint_t  glyph,
				    ثابت صحيح *coords, بنانشان صحيح coord_ڳڻپ,
				    hb_scalar_cache_t *store_cache = خالي_اشارو) ثابت
  {
    /* Implicit mapping uses outer index zero and the glyph id as the
     * inner index. An oversized glyph id must not select another table. */
    إذا (!advMap && glyph > 0xFFFFu) أرجع 0.f;
    uint32_t varidx = (هذا+advMap).map (glyph);
    أرجع (هذا+varStore).get_delta (varidx,
				      coords, coord_ڳڻپ,
				      store_cache);
  }

  عام:
  Fixed_نسخو<>نسخو;	/* Version of the metrics variation table
				 * initially set to 0x00010000u */
  Offset32_منزل<شي_Variation_Store>
		varStore;	/* Offset to item variation store table. */
  Offset32_منزل<Delta_سيٽ_اشاريو_Map>
		advMap;		/* Offset to advance var-idx mapping. */
  Offset32_منزل<Delta_سيٽ_اشاريو_Map>
		lsbMap;		/* Offset to lsb/tsb var-idx mapping. */
  Offset32_منزل<Delta_سيٽ_اشاريو_Map>
		rsbMap;		/* Offset to rsb/bsb var-idx mapping. */

  عام:
  DEFINE_سائيز_STATIC (20);
};

بنية HVAR : HVARVVAR {
  ساكن constexpr hb_ٽيگ_t جدول_ٽيگ = HB_OT_ٽيگ_HVAR;
  منطقي subset (hb_subset_context_t *c) ثابت { أرجع HVARVVAR::_subset<HVAR> (c); }
};
بنية VVAR : HVARVVAR {
  ساكن constexpr hb_ٽيگ_t جدول_ٽيگ = HB_OT_ٽيگ_VVAR;

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (static_cast<ثابت HVARVVAR *> (هذا)->sanitize (c) &&
		  vorgMap.sanitize (c, هذا));
  }

  فراغ listup_اشاريو_maps (hb_ويڪٽر_t<ثابت Delta_سيٽ_اشاريو_Map *> &اشاريو_maps) ثابت
  {
    HVARVVAR::listup_اشاريو_maps (اشاريو_maps);
    إذا (vorgMap) اشاريو_maps.push (&(هذا+vorgMap));
    وإلا اشاريو_maps.push (خالي_اشارو);
  }

  منطقي serialize_اشاريو_maps (hb_serialize_context_t *c,
			     ثابت hb_array_t<اشاريو_map_subset_plan_t> &im_plans)
  {
    TRACE_SERIALIZE (هذا);
    إذا (unlikely (!HVARVVAR::serialize_اشاريو_maps (c, im_plans)))
      return_trace (خطأ);
    إذا (!im_plans[اشاريو_map_subset_plan_t::VORG_اشاريو].get_map_ڳڻپ ())
      vorgMap = 0;
    وإلا إذا (unlikely (!vorgMap.serialize_serialize (c, im_plans[اشاريو_map_subset_plan_t::VORG_اشاريو])))
      return_trace (خطأ);

    return_trace (سچ);
  }

  منطقي subset (hb_subset_context_t *c) ثابت { أرجع HVARVVAR::_subset<VVAR> (c); }

  HB_ALWAYS_INLINE
  عائم get_vorg_delta_unscaled (hb_codepoint_t glyph,
				 ثابت صحيح *coords, بنانشان صحيح coord_ڳڻپ,
				 hb_scalar_cache_t *store_cache = خالي_اشارو) ثابت
  {
    إذا (!vorgMap) أرجع 0.f;
    uint32_t varidx = (هذا+vorgMap).map (glyph);
    أرجع (هذا+varStore).get_delta (varidx,
				      coords, coord_ڳڻپ,
				      store_cache);
  }

  محمي:
  Offset32_منزل<Delta_سيٽ_اشاريو_Map>
		vorgMap;	/* Offset to vertical-origin var-idx mapping. */

  عام:
  DEFINE_سائيز_STATIC (24);
};

} /* namespace OT */


#جيڪڏهن_پورو /* HB_OT_VAR_HVAR_TABLE_HH */
