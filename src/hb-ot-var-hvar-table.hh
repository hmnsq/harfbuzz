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

#اگر_تعریف_نشده HB_OT_VAR_HVAR_جدول_HH
#تعریف_کن HB_OT_VAR_HVAR_جدول_HH

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

  فراغ init (ثابت Delta_مجموعه_شاخص_Map  *شاخص_map,
	     hb_inc_bimap_t	     &outer_map,
	     hb_وکتور_t<hb_مجموعه_t *> &inner_sets,
	     ثابت hb_subset_plan_t  *plan,
	     منطقي bypass_خالی = درست)
  {
    map_شمار = 0;
    پر_map_شمار = plan->new_مقصد_old_gid_list.درازی ?
		     plan->new_مقصد_old_gid_list.دم ().اولی + 1 : 0;
    /* Only the advance mapping may be dropped in favor of implicit
     * glyph-id indexing (for the others a null offset means no deltas),
     * and only when the source was map-less too, preserving its form. */
    can_drop_مقصد_implicit = !bypass_خالی && !شاخص_map;
    max_inners.init ();
    output_map.init ();

    إذا (bypass_خالی && (!شاخص_map || !شاخص_map->get_map_شمار ())) أرجع;

    بیعلامت صحيح	آخرین_val = (بیعلامت صحيح)-1;
    hb_codepoint_t	آخرین_gid = HB_CODEPOINT_INVALID;

    max_inners.resize (inner_sets.درازی);
    لكل (بیعلامت i = 0; i < inner_sets.درازی; i++) max_inners[i] = 0;

    /* Search backwards for a map value different from the last map value */
    خودکار &new_مقصد_old_gid_list = plan->new_مقصد_old_gid_list;
    بیعلامت شمار = new_مقصد_old_gid_list.درازی;
    إذا (!شاخص_map)
    {
      map_شمار = new_مقصد_old_gid_list.دم ().اولی + 1;
    }
    وإلا
    {
      لكل (بیعلامت j = شمار; j; j--)
      {
        hb_codepoint_t gid = new_مقصد_old_gid_list.arrayZ[j - 1].اولی;
        hb_codepoint_t old_gid = new_مقصد_old_gid_list.arrayZ[j - 1].second;
  
        بیعلامت صحيح v = شاخص_map->map (old_gid);
        إذا (آخرین_gid == HB_CODEPOINT_INVALID)
        {
  	آخرین_val = v;
  	آخرین_gid = gid;
  	استمر;
        }
        إذا (v != آخرین_val)
  	اكسر;
  
        آخرین_gid = gid;
      }
  
      إذا (unlikely (آخرین_gid == (hb_codepoint_t)-1)) أرجع;
      map_شمار = آخرین_gid + 1;
    }

    لكل (خودکار _ : plan->new_مقصد_old_gid_list)
    {
      hb_codepoint_t gid = _.اولی;
      إذا (gid >= map_شمار) اكسر;

      hb_codepoint_t old_gid = _.second;
      بیعلامت صحيح v = شاخص_map ? شاخص_map->map (old_gid): old_gid;
      بیعلامت صحيح outer = v >> 16;
      بیعلامت صحيح inner = v & 0xFFFF;
      outer_map.جمع_کن (outer);
      إذا (inner > max_inners[outer]) max_inners[outer] = inner;
      إذا (outer >= inner_sets.درازی) أرجع;
      inner_sets[outer]->جمع_کن (inner);
    }
  }

  فراغ fini ()
  {
    max_inners.fini ();
    output_map.fini ();
  }

  فراغ remap (ثابت Delta_مجموعه_شاخص_Map *درون_داد_map,
	      ثابت hb_inc_bimap_t &outer_map,
	      ثابت hb_وکتور_t<hb_inc_bimap_t> &inner_maps,
	      ثابت hb_subset_plan_t *plan)
  {
    outer_بیت_شمار = 1;
    inner_بیت_شمار = 1;

    لكل (بیعلامت صحيح i = 0; i < max_inners.درازی; i++)
    {
      إذا (inner_maps[i].get_population () == 0) استمر;
      بیعلامت صحيح بیت_شمار = (max_inners[i]==0)? 1: hb_بیت_storage (inner_maps[i][max_inners[i]]);
      إذا (بیت_شمار > inner_بیت_شمار) inner_بیت_شمار = بیت_شمار;
    }

    إذا (unlikely (!output_map.resize (map_شمار))) أرجع;
    لكل (ثابت خودکار &_ : plan->new_مقصد_old_gid_list)
    {
      hb_codepoint_t new_gid = _.اولی;
      hb_codepoint_t old_gid = _.second;

      إذا (unlikely (new_gid >= map_شمار)) اكسر;

      uint32_t v = درون_داد_map? درون_داد_map->map (old_gid) : old_gid;
      بیعلامت outer = v >> 16;
      بیعلامت new_outer = outer_map[outer];
      بیعلامت بیت_شمار = (new_outer == 0) ? 1 : hb_بیت_storage (new_outer);
      outer_بیت_شمار = hb_max (بیت_شمار, outer_بیت_شمار);

      output_map.arrayZ[new_gid] = (new_outer << 16) | (inner_maps[outer][v & 0xFFFF]);
    }
  }

  منطقي remap_after_instantiation (ثابت hb_subset_plan_t *plan,
                                  ثابت hb_map_t& varidx_map)
  {
    /* recalculate bit_count after remapping */
    outer_بیت_شمار = 1;
    inner_بیت_شمار = 1;

    لكل (ثابت خودکار &_ : plan->new_مقصد_old_gid_list)
    {
      hb_codepoint_t new_gid = _.اولی;
      إذا (unlikely (new_gid >= map_شمار)) اكسر;

      uint32_t v = output_map.arrayZ[new_gid];
      uint32_t *new_varidx;
      إذا (!varidx_map.has (v, &new_varidx))
        أرجع خطأ;

      output_map.arrayZ[new_gid] = *new_varidx;

      بیعلامت outer = (*new_varidx) >> 16;
      بیعلامت بیت_شمار = (outer == 0) ? 1 : hb_بیت_storage (outer);
      outer_بیت_شمار = hb_max (بیت_شمار, outer_بیت_شمار);

      بیعلامت inner = (*new_varidx) & 0xFFFF;
      بیت_شمار = (inner == 0) ? 1 : hb_بیت_storage (inner);
      inner_بیت_شمار = hb_max (بیت_شمار, inner_بیت_شمار);
    }
    أرجع درست;
  }

  بیعلامت صحيح get_inner_بیت_شمار () ثابت { أرجع inner_بیت_شمار; }
  بیعلامت صحيح get_عرض ()           ثابت { أرجع ((outer_بیت_شمار + inner_بیت_شمار + 7) / 8); }
  بیعلامت صحيح get_map_شمار ()       ثابت { أرجع map_شمار; }

  size_t get_اندازه () ثابت
  {
    إذا (!map_شمار) أرجع 0;
    أرجع hb_unsigned_mul_جمع_کن_saturate (get_عرض (), map_شمار,
					 Delta_مجموعه_شاخص_Map::min_اندازه);
  }

  منطقي is_عنصر_خنثی () ثابت
  {
    إذا (!output_map) أرجع درست;
    /* An advance map whose entries are the identity over every output
     * glyph is equivalent to implicit glyph-id indexing; drop it.  A
     * truncated map doesn't qualify: glyphs past its end share its last
     * value, not their own ids. */
    إذا (!can_drop_مقصد_implicit || map_شمار != پر_map_شمار) أرجع خطأ;
    لكل (بیعلامت i = 0; i < map_شمار; i++)
      إذا (output_map.arrayZ[i] != i) أرجع خطأ;
    أرجع درست;
  }
  hb_array_t<ثابت uint32_t> get_output_map () ثابت { أرجع output_map.as_array (); }

  محمي:
  بیعلامت صحيح map_شمار;
  بیعلامت صحيح پر_map_شمار;
  منطقي can_drop_مقصد_implicit;
  hb_وکتور_t<بیعلامت صحيح> max_inners;
  بیعلامت صحيح outer_بیت_شمار;
  بیعلامت صحيح inner_بیت_شمار;
  hb_وکتور_t<uint32_t> output_map;
};

بنية hvarvvar_subset_plan_t
{
  hvarvvar_subset_plan_t() : inner_maps (), شاخص_map_plans () {}
  ~hvarvvar_subset_plan_t() { fini (); }

  فراغ init (ثابت hb_array_t<ثابت Delta_مجموعه_شاخص_Map *> &شاخص_maps,
	     ثابت قلم_Variation_Store &_var_store,
	     ثابت hb_subset_plan_t *plan)
  {
    شاخص_map_plans.resize (شاخص_maps.درازی);

    var_store = &_var_store;
    inner_sets.resize (var_store->get_sub_جدول_شمار ());
    لكل (بیعلامت صحيح i = 0; i < inner_sets.درازی; i++)
      inner_sets[i] = hb_مجموعه_create ();
    adv_مجموعه = hb_مجموعه_create ();

    inner_maps.resize (var_store->get_sub_جدول_شمار ());

    إذا (unlikely (!شاخص_map_plans.درازی || !inner_sets.درازی || !inner_maps.درازی)) أرجع;

    منطقي retain_adv_map = خطأ;
    شاخص_map_plans[0].init (شاخص_maps[0], outer_map, inner_sets, plan, خطأ);
    إذا (!شاخص_maps[0])
    {
      retain_adv_map = plan->flags & HB_SUBSET_FLAGS_RETAIN_GIDS;
      outer_map.جمع_کن (0);
      لكل (hb_codepoint_t old_gid : plan->glyphset()->iter())
        inner_sets[0]->جمع_کن (old_gid);
      hb_مجموعه_union (adv_مجموعه, inner_sets[0]);
    }

    لكل (بیعلامت صحيح i = 1; i < شاخص_maps.درازی; i++)
      شاخص_map_plans[i].init (شاخص_maps[i], outer_map, inner_sets, plan);

    outer_map.sort ();

    إذا (retain_adv_map)
    {
      لكل (ثابت خودکار &_ : plan->new_مقصد_old_gid_list)
      {
        hb_codepoint_t old_gid = _.second;
	inner_maps[0].جمع_کن (old_gid);
      }
    }
    وإلا
    {
      inner_maps[0].جمع_کن_مجموعه (adv_مجموعه);
      hb_مجموعه_تفریق (inner_sets[0], adv_مجموعه);
      inner_maps[0].جمع_کن_مجموعه (inner_sets[0]);
    }

    لكل (بیعلامت صحيح i = 1; i < inner_maps.درازی; i++)
      inner_maps[i].جمع_کن_مجموعه (inner_sets[i]);

    لكل (بیعلامت صحيح i = 0; i < شاخص_maps.درازی; i++)
      شاخص_map_plans[i].remap (شاخص_maps[i], outer_map, inner_maps, plan);
  }

  /* remap */
  منطقي remap_شاخص_map_plans (ثابت hb_subset_plan_t *plan,
                              ثابت hb_map_t& varidx_map)
  {
    لكل (بیعلامت i = 0; i < شاخص_map_plans.درازی; i++)
      إذا (!شاخص_map_plans[i].remap_after_instantiation (plan, varidx_map))
        أرجع خطأ;
    أرجع درست;
  }

  فراغ fini ()
  {
    لكل (بیعلامت صحيح i = 0; i < inner_sets.درازی; i++)
      hb_مجموعه_destroy (inner_sets[i]);
    hb_مجموعه_destroy (adv_مجموعه);
    inner_maps.fini ();
    شاخص_map_plans.fini ();
  }

  hb_inc_bimap_t outer_map;
  hb_وکتور_t<hb_inc_bimap_t> inner_maps;
  hb_وکتور_t<شاخص_map_subset_plan_t> شاخص_map_plans;
  ثابت قلم_Variation_Store *var_store;

  محمي:
  hb_وکتور_t<hb_مجموعه_t *> inner_sets;
  hb_مجموعه_t *adv_مجموعه;
};

/*
 * HVAR -- Horizontal Metrics Variations
 * https://docs.microsoft.com/en-us/typography/opentype/spec/hvar
 * VVAR -- Vertical Metrics Variations
 * https://docs.microsoft.com/en-us/typography/opentype/spec/vvar
 */
#تعریف_کن HB_OT_برچسب_HVAR HB_برچسب('H','V','A','R')
#تعریف_کن HB_OT_برچسب_VVAR HB_برچسب('V','V','A','R')

بنية HVARVVAR
{
  ساكن constexpr hb_برچسب_t HVARTag = HB_OT_برچسب_HVAR;
  ساكن constexpr hb_برچسب_t VVARTag = HB_OT_برچسب_VVAR;

  منطقي has_داده () ثابت { أرجع نسخه.major != 0; }

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

  ثابت قلم_Variation_Store& get_var_store () ثابت
  { أرجع هذا+varStore; }

  فراغ listup_شاخص_maps (hb_وکتور_t<ثابت Delta_مجموعه_شاخص_Map *> &شاخص_maps) ثابت
  {
    إذا (advMap) شاخص_maps.push (&(هذا+advMap));
    وإلا شاخص_maps.push (اشاره_تهی);

    إذا (lsbMap) شاخص_maps.push (&(هذا+lsbMap));
    وإلا شاخص_maps.push (اشاره_تهی);

    إذا (rsbMap) شاخص_maps.push (&(هذا+rsbMap));
    وإلا شاخص_maps.push (اشاره_تهی);
  }

  منطقي serialize_شاخص_maps (hb_serialize_context_t *c,
			     ثابت hb_array_t<شاخص_map_subset_plan_t> &im_plans)
  {
    TRACE_SERIALIZE (هذا);
    إذا (im_plans[شاخص_map_subset_plan_t::ADV_شاخص].is_عنصر_خنثی ())
      advMap = 0;
    وإلا إذا (unlikely (!advMap.serialize_serialize (c, im_plans[شاخص_map_subset_plan_t::ADV_شاخص])))
      return_trace (خطأ);
    إذا (im_plans[شاخص_map_subset_plan_t::LSB_شاخص].is_عنصر_خنثی ())
      lsbMap = 0;
    وإلا إذا (unlikely (!lsbMap.serialize_serialize (c, im_plans[شاخص_map_subset_plan_t::LSB_شاخص])))
      return_trace (خطأ);
    إذا (im_plans[شاخص_map_subset_plan_t::RSB_شاخص].is_عنصر_خنثی ())
      rsbMap = 0;
    وإلا إذا (unlikely (!rsbMap.serialize_serialize (c, im_plans[شاخص_map_subset_plan_t::RSB_شاخص])))
      return_trace (خطأ);

    return_trace (درست);
  }

  قالب <نام_نوع T>
  منطقي _subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    إذا (c->plan->all_axes_pinned)
      return_trace (خطأ);

    hvarvvar_subset_plan_t	hvar_plan;
    hb_وکتور_t<ثابت Delta_مجموعه_شاخص_Map *>
				شاخص_maps;

    ((T*)هذا)->listup_شاخص_maps (شاخص_maps);
    hvar_plan.init (شاخص_maps.as_array (), هذا+varStore, c->plan);

    T *بیرون = c->serializer->allocate_min<T> ();
    إذا (unlikely (!بیرون)) return_trace (خطأ);

    بیرون->نسخه.major = 1;
    بیرون->نسخه.minor = 0;

    إذا (c->plan->normalized_coords)
    {
      قلم_variations_t قلم_vars;
      إذا (!قلم_vars.instantiate (هذا+varStore, c->plan,
                                  advMap == 0 ? خطأ : درست,
                                  خطأ, /* use_no_variation_idx = false */
                                  hvar_plan.inner_maps.as_array ()))
        return_trace (خطأ);

      إذا (!بیرون->varStore.serialize_serialize (c->serializer,
                                              قلم_vars.has_long_کلمه (),
                                              c->plan->axis_برچسبها,
                                              قلم_vars.get_region_list (),
                                              قلم_vars.get_vardata_encodings ()))
        return_trace (خطأ);

      /* if varstore is optimized, remap output_map */
      إذا (advMap)
      {
        إذا (!hvar_plan.remap_شاخص_map_plans (c->plan, قلم_vars.get_varidx_map ()))
          return_trace (خطأ);
      }
    }
    وإلا
    {
      /* avar2 partial instancing: cull unreachable regions. */
      hb_مجموعه_t dead_regions;
      إذا (c->plan->has_avar2)
	hvar_plan.var_store->collect_dead_regions (c->plan->axes_old_شاخص_برچسب_map,
						   c->plan->avar2_reachable_ranges,
						   dead_regions);

      إذا (unlikely (!بیرون->varStore
		    .serialize_serialize (c->serializer,
					  hvar_plan.var_store,
					  hvar_plan.inner_maps.as_array (),
					  dead_regions.get_population () ? &dead_regions : اشاره_تهی)))
      return_trace (خطأ);
    }

    return_trace (بیرون->T::serialize_شاخص_maps (c->serializer,
						hvar_plan.شاخص_map_plans.as_array ()));
  }

  HB_ALWAYS_INLINE
  عائم get_advance_delta_unscaled (hb_codepoint_t  glyph,
				    ثابت صحيح *coords, بیعلامت صحيح coord_شمار,
				    hb_scalar_cache_t *store_cache = اشاره_تهی) ثابت
  {
    /* Implicit mapping uses outer index zero and the glyph id as the
     * inner index. An oversized glyph id must not select another table. */
    إذا (!advMap && glyph > 0xFFFFu) أرجع 0.f;
    uint32_t varidx = (هذا+advMap).map (glyph);
    أرجع (هذا+varStore).get_delta (varidx,
				      coords, coord_شمار,
				      store_cache);
  }

  عام:
  Fixed_نسخه<>نسخه;	/* Version of the metrics variation table
				 * initially set to 0x00010000u */
  Offset32_مقصد<قلم_Variation_Store>
		varStore;	/* Offset to item variation store table. */
  Offset32_مقصد<Delta_مجموعه_شاخص_Map>
		advMap;		/* Offset to advance var-idx mapping. */
  Offset32_مقصد<Delta_مجموعه_شاخص_Map>
		lsbMap;		/* Offset to lsb/tsb var-idx mapping. */
  Offset32_مقصد<Delta_مجموعه_شاخص_Map>
		rsbMap;		/* Offset to rsb/bsb var-idx mapping. */

  عام:
  DEFINE_اندازه_STATIC (20);
};

بنية HVAR : HVARVVAR {
  ساكن constexpr hb_برچسب_t جدول_برچسب = HB_OT_برچسب_HVAR;
  منطقي subset (hb_subset_context_t *c) ثابت { أرجع HVARVVAR::_subset<HVAR> (c); }
};
بنية VVAR : HVARVVAR {
  ساكن constexpr hb_برچسب_t جدول_برچسب = HB_OT_برچسب_VVAR;

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (static_cast<ثابت HVARVVAR *> (هذا)->sanitize (c) &&
		  vorgMap.sanitize (c, هذا));
  }

  فراغ listup_شاخص_maps (hb_وکتور_t<ثابت Delta_مجموعه_شاخص_Map *> &شاخص_maps) ثابت
  {
    HVARVVAR::listup_شاخص_maps (شاخص_maps);
    إذا (vorgMap) شاخص_maps.push (&(هذا+vorgMap));
    وإلا شاخص_maps.push (اشاره_تهی);
  }

  منطقي serialize_شاخص_maps (hb_serialize_context_t *c,
			     ثابت hb_array_t<شاخص_map_subset_plan_t> &im_plans)
  {
    TRACE_SERIALIZE (هذا);
    إذا (unlikely (!HVARVVAR::serialize_شاخص_maps (c, im_plans)))
      return_trace (خطأ);
    إذا (!im_plans[شاخص_map_subset_plan_t::VORG_شاخص].get_map_شمار ())
      vorgMap = 0;
    وإلا إذا (unlikely (!vorgMap.serialize_serialize (c, im_plans[شاخص_map_subset_plan_t::VORG_شاخص])))
      return_trace (خطأ);

    return_trace (درست);
  }

  منطقي subset (hb_subset_context_t *c) ثابت { أرجع HVARVVAR::_subset<VVAR> (c); }

  HB_ALWAYS_INLINE
  عائم get_vorg_delta_unscaled (hb_codepoint_t glyph,
				 ثابت صحيح *coords, بیعلامت صحيح coord_شمار,
				 hb_scalar_cache_t *store_cache = اشاره_تهی) ثابت
  {
    إذا (!vorgMap) أرجع 0.f;
    uint32_t varidx = (هذا+vorgMap).map (glyph);
    أرجع (هذا+varStore).get_delta (varidx,
				      coords, coord_شمار,
				      store_cache);
  }

  محمي:
  Offset32_مقصد<Delta_مجموعه_شاخص_Map>
		vorgMap;	/* Offset to vertical-origin var-idx mapping. */

  عام:
  DEFINE_اندازه_STATIC (24);
};

} /* namespace OT */


#پایان_اگر /* HB_OT_VAR_HVAR_TABLE_HH */
