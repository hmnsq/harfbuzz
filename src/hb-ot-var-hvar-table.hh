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

#که_نه_تعریف_شوی HB_OT_VAR_HVAR_جدول_HH
#تعریف_کړه HB_OT_VAR_HVAR_جدول_HH

#ضمن "hb-ot-layout-common.hh"
#ضمن "hb-ot-var-common.hh"

نطاق OT {


بنية شاخص_map_subset_plan_t
{
  تعداد شاخص_map_شاخص_t {
    ADV_شاخص,
    LSB_شاخص,	/* dual as TSB */
    RSB_شاخص,	/* dual as BSB */
    VORG_شاخص
  };

  فراغ init (ثابت Delta_سېټ_شاخص_Map  *شاخص_map,
	     hb_inc_bimap_t	     &outer_map,
	     hb_ویکټور_t<hb_سېټ_t *> &inner_sets,
	     ثابت hb_subset_plan_t  *plan,
	     منطقي bypass_تشه = ریښتیا)
  {
    map_شمېر = 0;
    ډک_map_شمېر = plan->new_منزل_old_gid_list.اوږدتیا ?
		     plan->new_منزل_old_gid_list.لکۍ ().لومړی + 1 : 0;
    /* Only the advance mapping may be dropped in favor of implicit
     * glyph-id indexing (for the others a null offset means no deltas),
     * and only when the source was map-less too, preserving its form. */
    can_drop_منزل_implicit = !bypass_تشه && !شاخص_map;
    max_inners.init ();
    output_map.init ();

    إذا (bypass_تشه && (!شاخص_map || !شاخص_map->get_map_شمېر ())) أرجع;

    بینښه صحيح	وروستی_val = (بینښه صحيح)-1;
    hb_codepoint_t	وروستی_gid = HB_CODEPOINT_INVALID;

    max_inners.resize (inner_sets.اوږدتیا);
    لكل (بینښه i = 0; i < inner_sets.اوږدتیا; i++) max_inners[i] = 0;

    /* Search backwards for a map value different from the last map value */
    خپلکاری &new_منزل_old_gid_list = plan->new_منزل_old_gid_list;
    بینښه شمېر = new_منزل_old_gid_list.اوږدتیا;
    إذا (!شاخص_map)
    {
      map_شمېر = new_منزل_old_gid_list.لکۍ ().لومړی + 1;
    }
    وإلا
    {
      لكل (بینښه j = شمېر; j; j--)
      {
        hb_codepoint_t gid = new_منزل_old_gid_list.arrayZ[j - 1].لومړی;
        hb_codepoint_t old_gid = new_منزل_old_gid_list.arrayZ[j - 1].second;
  
        بینښه صحيح v = شاخص_map->map (old_gid);
        إذا (وروستی_gid == HB_CODEPOINT_INVALID)
        {
  	وروستی_val = v;
  	وروستی_gid = gid;
  	استمر;
        }
        إذا (v != وروستی_val)
  	اكسر;
  
        وروستی_gid = gid;
      }
  
      إذا (unlikely (وروستی_gid == (hb_codepoint_t)-1)) أرجع;
      map_شمېر = وروستی_gid + 1;
    }

    لكل (خپلکاری _ : plan->new_منزل_old_gid_list)
    {
      hb_codepoint_t gid = _.لومړی;
      إذا (gid >= map_شمېر) اكسر;

      hb_codepoint_t old_gid = _.second;
      بینښه صحيح v = شاخص_map ? شاخص_map->map (old_gid): old_gid;
      بینښه صحيح outer = v >> 16;
      بینښه صحيح inner = v & 0xFFFF;
      outer_map.جمع_کړه (outer);
      إذا (inner > max_inners[outer]) max_inners[outer] = inner;
      إذا (outer >= inner_sets.اوږدتیا) أرجع;
      inner_sets[outer]->جمع_کړه (inner);
    }
  }

  فراغ fini ()
  {
    max_inners.fini ();
    output_map.fini ();
  }

  فراغ remap (ثابت Delta_سېټ_شاخص_Map *ننوت_map,
	      ثابت hb_inc_bimap_t &outer_map,
	      ثابت hb_ویکټور_t<hb_inc_bimap_t> &inner_maps,
	      ثابت hb_subset_plan_t *plan)
  {
    outer_بټ_شمېر = 1;
    inner_بټ_شمېر = 1;

    لكل (بینښه صحيح i = 0; i < max_inners.اوږدتیا; i++)
    {
      إذا (inner_maps[i].get_population () == 0) استمر;
      بینښه صحيح بټ_شمېر = (max_inners[i]==0)? 1: hb_بټ_storage (inner_maps[i][max_inners[i]]);
      إذا (بټ_شمېر > inner_بټ_شمېر) inner_بټ_شمېر = بټ_شمېر;
    }

    إذا (unlikely (!output_map.resize (map_شمېر))) أرجع;
    لكل (ثابت خپلکاری &_ : plan->new_منزل_old_gid_list)
    {
      hb_codepoint_t new_gid = _.لومړی;
      hb_codepoint_t old_gid = _.second;

      إذا (unlikely (new_gid >= map_شمېر)) اكسر;

      uint32_t v = ننوت_map? ننوت_map->map (old_gid) : old_gid;
      بینښه outer = v >> 16;
      بینښه new_outer = outer_map[outer];
      بینښه بټ_شمېر = (new_outer == 0) ? 1 : hb_بټ_storage (new_outer);
      outer_بټ_شمېر = hb_max (بټ_شمېر, outer_بټ_شمېر);

      output_map.arrayZ[new_gid] = (new_outer << 16) | (inner_maps[outer][v & 0xFFFF]);
    }
  }

  منطقي remap_after_instantiation (ثابت hb_subset_plan_t *plan,
                                  ثابت hb_map_t& varidx_map)
  {
    /* recalculate bit_count after remapping */
    outer_بټ_شمېر = 1;
    inner_بټ_شمېر = 1;

    لكل (ثابت خپلکاری &_ : plan->new_منزل_old_gid_list)
    {
      hb_codepoint_t new_gid = _.لومړی;
      إذا (unlikely (new_gid >= map_شمېر)) اكسر;

      uint32_t v = output_map.arrayZ[new_gid];
      uint32_t *new_varidx;
      إذا (!varidx_map.has (v, &new_varidx))
        أرجع خطأ;

      output_map.arrayZ[new_gid] = *new_varidx;

      بینښه outer = (*new_varidx) >> 16;
      بینښه بټ_شمېر = (outer == 0) ? 1 : hb_بټ_storage (outer);
      outer_بټ_شمېر = hb_max (بټ_شمېر, outer_بټ_شمېر);

      بینښه inner = (*new_varidx) & 0xFFFF;
      بټ_شمېر = (inner == 0) ? 1 : hb_بټ_storage (inner);
      inner_بټ_شمېر = hb_max (بټ_شمېر, inner_بټ_شمېر);
    }
    أرجع ریښتیا;
  }

  بینښه صحيح get_inner_بټ_شمېر () ثابت { أرجع inner_بټ_شمېر; }
  بینښه صحيح get_پلنوالی ()           ثابت { أرجع ((outer_بټ_شمېر + inner_بټ_شمېر + 7) / 8); }
  بینښه صحيح get_map_شمېر ()       ثابت { أرجع map_شمېر; }

  size_t get_اندازه () ثابت
  {
    إذا (!map_شمېر) أرجع 0;
    أرجع hb_unsigned_mul_جمع_کړه_saturate (get_پلنوالی (), map_شمېر,
					 Delta_سېټ_شاخص_Map::min_اندازه);
  }

  منطقي is_بې_اغېزه_عنصر () ثابت
  {
    إذا (!output_map) أرجع ریښتیا;
    /* An advance map whose entries are the identity over every output
     * glyph is equivalent to implicit glyph-id indexing; drop it.  A
     * truncated map doesn't qualify: glyphs past its end share its last
     * value, not their own ids. */
    إذا (!can_drop_منزل_implicit || map_شمېر != ډک_map_شمېر) أرجع خطأ;
    لكل (بینښه i = 0; i < map_شمېر; i++)
      إذا (output_map.arrayZ[i] != i) أرجع خطأ;
    أرجع ریښتیا;
  }
  hb_array_t<ثابت uint32_t> get_output_map () ثابت { أرجع output_map.as_array (); }

  محمي:
  بینښه صحيح map_شمېر;
  بینښه صحيح ډک_map_شمېر;
  منطقي can_drop_منزل_implicit;
  hb_ویکټور_t<بینښه صحيح> max_inners;
  بینښه صحيح outer_بټ_شمېر;
  بینښه صحيح inner_بټ_شمېر;
  hb_ویکټور_t<uint32_t> output_map;
};

بنية hvarvvar_subset_plan_t
{
  hvarvvar_subset_plan_t() : inner_maps (), شاخص_map_plans () {}
  ~hvarvvar_subset_plan_t() { fini (); }

  فراغ init (ثابت hb_array_t<ثابت Delta_سېټ_شاخص_Map *> &شاخص_maps,
	     ثابت توکی_Variation_Store &_var_store,
	     ثابت hb_subset_plan_t *plan)
  {
    شاخص_map_plans.resize (شاخص_maps.اوږدتیا);

    var_store = &_var_store;
    inner_sets.resize (var_store->get_sub_جدول_شمېر ());
    لكل (بینښه صحيح i = 0; i < inner_sets.اوږدتیا; i++)
      inner_sets[i] = hb_سېټ_create ();
    adv_سېټ = hb_سېټ_create ();

    inner_maps.resize (var_store->get_sub_جدول_شمېر ());

    إذا (unlikely (!شاخص_map_plans.اوږدتیا || !inner_sets.اوږدتیا || !inner_maps.اوږدتیا)) أرجع;

    منطقي retain_adv_map = خطأ;
    شاخص_map_plans[0].init (شاخص_maps[0], outer_map, inner_sets, plan, خطأ);
    إذا (!شاخص_maps[0])
    {
      retain_adv_map = plan->flags & HB_SUBSET_FLAGS_RETAIN_GIDS;
      outer_map.جمع_کړه (0);
      لكل (hb_codepoint_t old_gid : plan->glyphset()->iter())
        inner_sets[0]->جمع_کړه (old_gid);
      hb_سېټ_union (adv_سېټ, inner_sets[0]);
    }

    لكل (بینښه صحيح i = 1; i < شاخص_maps.اوږدتیا; i++)
      شاخص_map_plans[i].init (شاخص_maps[i], outer_map, inner_sets, plan);

    outer_map.sort ();

    إذا (retain_adv_map)
    {
      لكل (ثابت خپلکاری &_ : plan->new_منزل_old_gid_list)
      {
        hb_codepoint_t old_gid = _.second;
	inner_maps[0].جمع_کړه (old_gid);
      }
    }
    وإلا
    {
      inner_maps[0].جمع_کړه_سېټ (adv_سېټ);
      hb_سېټ_کمول (inner_sets[0], adv_سېټ);
      inner_maps[0].جمع_کړه_سېټ (inner_sets[0]);
    }

    لكل (بینښه صحيح i = 1; i < inner_maps.اوږدتیا; i++)
      inner_maps[i].جمع_کړه_سېټ (inner_sets[i]);

    لكل (بینښه صحيح i = 0; i < شاخص_maps.اوږدتیا; i++)
      شاخص_map_plans[i].remap (شاخص_maps[i], outer_map, inner_maps, plan);
  }

  /* remap */
  منطقي remap_شاخص_map_plans (ثابت hb_subset_plan_t *plan,
                              ثابت hb_map_t& varidx_map)
  {
    لكل (بینښه i = 0; i < شاخص_map_plans.اوږدتیا; i++)
      إذا (!شاخص_map_plans[i].remap_after_instantiation (plan, varidx_map))
        أرجع خطأ;
    أرجع ریښتیا;
  }

  فراغ fini ()
  {
    لكل (بینښه صحيح i = 0; i < inner_sets.اوږدتیا; i++)
      hb_سېټ_destroy (inner_sets[i]);
    hb_سېټ_destroy (adv_سېټ);
    inner_maps.fini ();
    شاخص_map_plans.fini ();
  }

  hb_inc_bimap_t outer_map;
  hb_ویکټور_t<hb_inc_bimap_t> inner_maps;
  hb_ویکټور_t<شاخص_map_subset_plan_t> شاخص_map_plans;
  ثابت توکی_Variation_Store *var_store;

  محمي:
  hb_ویکټور_t<hb_سېټ_t *> inner_sets;
  hb_سېټ_t *adv_سېټ;
};

/*
 * HVAR -- Horizontal Metrics Variations
 * https://docs.microsoft.com/en-us/typography/opentype/spec/hvar
 * VVAR -- Vertical Metrics Variations
 * https://docs.microsoft.com/en-us/typography/opentype/spec/vvar
 */
#تعریف_کړه HB_OT_نښان_HVAR HB_نښان('H','V','A','R')
#تعریف_کړه HB_OT_نښان_VVAR HB_نښان('V','V','A','R')

بنية HVARVVAR
{
  ساكن constexpr hb_نښان_t HVARTag = HB_OT_نښان_HVAR;
  ساكن constexpr hb_نښان_t VVARTag = HB_OT_نښان_VVAR;

  منطقي has_معلومات () ثابت { أرجع نسخه.major != 0; }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (نسخه.sanitize (c) &&
		  hb_barrier () &&
		  likely (نسخه.major == 1) &&
		  varStore.sanitize (c, هذا) &&
		  advMap.sanitize (c, هذا) &&
		  lsbMap.sanitize (c, هذا) &&
		  rsbMap.sanitize (c, هذا));
  }

  ثابت توکی_Variation_Store& get_var_store () ثابت
  { أرجع هذا+varStore; }

  فراغ listup_شاخص_maps (hb_ویکټور_t<ثابت Delta_سېټ_شاخص_Map *> &شاخص_maps) ثابت
  {
    إذا (advMap) شاخص_maps.push (&(هذا+advMap));
    وإلا شاخص_maps.push (تش_اشاره);

    إذا (lsbMap) شاخص_maps.push (&(هذا+lsbMap));
    وإلا شاخص_maps.push (تش_اشاره);

    إذا (rsbMap) شاخص_maps.push (&(هذا+rsbMap));
    وإلا شاخص_maps.push (تش_اشاره);
  }

  منطقي serialize_شاخص_maps (hb_serialize_context_t *c,
			     ثابت hb_array_t<شاخص_map_subset_plan_t> &im_plans)
  {
    TRACE_SERIALIZE (هذا);
    إذا (im_plans[شاخص_map_subset_plan_t::ADV_شاخص].is_بې_اغېزه_عنصر ())
      advMap = 0;
    وإلا إذا (unlikely (!advMap.serialize_serialize (c, im_plans[شاخص_map_subset_plan_t::ADV_شاخص])))
      return_trace (خطأ);
    إذا (im_plans[شاخص_map_subset_plan_t::LSB_شاخص].is_بې_اغېزه_عنصر ())
      lsbMap = 0;
    وإلا إذا (unlikely (!lsbMap.serialize_serialize (c, im_plans[شاخص_map_subset_plan_t::LSB_شاخص])))
      return_trace (خطأ);
    إذا (im_plans[شاخص_map_subset_plan_t::RSB_شاخص].is_بې_اغېزه_عنصر ())
      rsbMap = 0;
    وإلا إذا (unlikely (!rsbMap.serialize_serialize (c, im_plans[شاخص_map_subset_plan_t::RSB_شاخص])))
      return_trace (خطأ);

    return_trace (ریښتیا);
  }

  کينډۍ <ډول_نوم T>
  منطقي _subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    إذا (c->plan->all_axes_pinned)
      return_trace (خطأ);

    hvarvvar_subset_plan_t	hvar_plan;
    hb_ویکټور_t<ثابت Delta_سېټ_شاخص_Map *>
				شاخص_maps;

    ((T*)هذا)->listup_شاخص_maps (شاخص_maps);
    hvar_plan.init (شاخص_maps.as_array (), هذا+varStore, c->plan);

    T *بهر = c->serializer->allocate_min<T> ();
    إذا (unlikely (!بهر)) return_trace (خطأ);

    بهر->نسخه.major = 1;
    بهر->نسخه.minor = 0;

    إذا (c->plan->normalized_coords)
    {
      توکی_variations_t توکی_vars;
      إذا (!توکی_vars.instantiate (هذا+varStore, c->plan,
                                  advMap == 0 ? خطأ : ریښتیا,
                                  خطأ, /* use_no_variation_idx = false */
                                  hvar_plan.inner_maps.as_array ()))
        return_trace (خطأ);

      إذا (!بهر->varStore.serialize_serialize (c->serializer,
                                              توکی_vars.has_long_کلمه (),
                                              c->plan->axis_نښانونه,
                                              توکی_vars.get_region_list (),
                                              توکی_vars.get_vardata_encodings ()))
        return_trace (خطأ);

      /* if varstore is optimized, remap output_map */
      إذا (advMap)
      {
        إذا (!hvar_plan.remap_شاخص_map_plans (c->plan, توکی_vars.get_varidx_map ()))
          return_trace (خطأ);
      }
    }
    وإلا
    {
      /* avar2 partial instancing: cull unreachable regions. */
      hb_سېټ_t dead_regions;
      إذا (c->plan->has_avar2)
	hvar_plan.var_store->collect_dead_regions (c->plan->axes_old_شاخص_نښان_map,
						   c->plan->avar2_reachable_ranges,
						   dead_regions);

      إذا (unlikely (!بهر->varStore
		    .serialize_serialize (c->serializer,
					  hvar_plan.var_store,
					  hvar_plan.inner_maps.as_array (),
					  dead_regions.get_population () ? &dead_regions : تش_اشاره)))
      return_trace (خطأ);
    }

    return_trace (بهر->T::serialize_شاخص_maps (c->serializer,
						hvar_plan.شاخص_map_plans.as_array ()));
  }

  HB_ALWAYS_INLINE
  عائم get_advance_delta_unscaled (hb_codepoint_t  glyph,
				    ثابت صحيح *coords, بینښه صحيح coord_شمېر,
				    hb_scalar_cache_t *store_cache = تش_اشاره) ثابت
  {
    /* Implicit mapping uses outer index zero and the glyph id as the
     * inner index. An oversized glyph id must not select another table. */
    إذا (!advMap && glyph > 0xFFFFu) أرجع 0.f;
    uint32_t varidx = (هذا+advMap).map (glyph);
    أرجع (هذا+varStore).get_delta (varidx,
				      coords, coord_شمېر,
				      store_cache);
  }

  عام:
  Fixed_نسخه<>نسخه;	/* Version of the metrics variation table
				 * initially set to 0x00010000u */
  Offset32_منزل<توکی_Variation_Store>
		varStore;	/* Offset to item variation store table. */
  Offset32_منزل<Delta_سېټ_شاخص_Map>
		advMap;		/* Offset to advance var-idx mapping. */
  Offset32_منزل<Delta_سېټ_شاخص_Map>
		lsbMap;		/* Offset to lsb/tsb var-idx mapping. */
  Offset32_منزل<Delta_سېټ_شاخص_Map>
		rsbMap;		/* Offset to rsb/bsb var-idx mapping. */

  عام:
  DEFINE_اندازه_STATIC (20);
};

بنية HVAR : HVARVVAR {
  ساكن constexpr hb_نښان_t جدول_نښان = HB_OT_نښان_HVAR;
  منطقي subset (hb_subset_context_t *c) ثابت { أرجع HVARVVAR::_subset<HVAR> (c); }
};
بنية VVAR : HVARVVAR {
  ساكن constexpr hb_نښان_t جدول_نښان = HB_OT_نښان_VVAR;

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (static_cast<ثابت HVARVVAR *> (هذا)->sanitize (c) &&
		  vorgMap.sanitize (c, هذا));
  }

  فراغ listup_شاخص_maps (hb_ویکټور_t<ثابت Delta_سېټ_شاخص_Map *> &شاخص_maps) ثابت
  {
    HVARVVAR::listup_شاخص_maps (شاخص_maps);
    إذا (vorgMap) شاخص_maps.push (&(هذا+vorgMap));
    وإلا شاخص_maps.push (تش_اشاره);
  }

  منطقي serialize_شاخص_maps (hb_serialize_context_t *c,
			     ثابت hb_array_t<شاخص_map_subset_plan_t> &im_plans)
  {
    TRACE_SERIALIZE (هذا);
    إذا (unlikely (!HVARVVAR::serialize_شاخص_maps (c, im_plans)))
      return_trace (خطأ);
    إذا (!im_plans[شاخص_map_subset_plan_t::VORG_شاخص].get_map_شمېر ())
      vorgMap = 0;
    وإلا إذا (unlikely (!vorgMap.serialize_serialize (c, im_plans[شاخص_map_subset_plan_t::VORG_شاخص])))
      return_trace (خطأ);

    return_trace (ریښتیا);
  }

  منطقي subset (hb_subset_context_t *c) ثابت { أرجع HVARVVAR::_subset<VVAR> (c); }

  HB_ALWAYS_INLINE
  عائم get_vorg_delta_unscaled (hb_codepoint_t glyph,
				 ثابت صحيح *coords, بینښه صحيح coord_شمېر,
				 hb_scalar_cache_t *store_cache = تش_اشاره) ثابت
  {
    إذا (!vorgMap) أرجع 0.f;
    uint32_t varidx = (هذا+vorgMap).map (glyph);
    أرجع (هذا+varStore).get_delta (varidx,
				      coords, coord_شمېر,
				      store_cache);
  }

  محمي:
  Offset32_منزل<Delta_سېټ_شاخص_Map>
		vorgMap;	/* Offset to vertical-origin var-idx mapping. */

  عام:
  DEFINE_اندازه_STATIC (24);
};

} /* namespace OT */


#د_که_پای /* HB_OT_VAR_HVAR_TABLE_HH */
