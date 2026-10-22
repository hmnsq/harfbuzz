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

#جے_بنا_تعریف HB_OT_VAR_HVAR_جدول_HH
#تعریف_کرو HB_OT_VAR_HVAR_جدول_HH

#ضمن "hb-ot-layout-common.hh"
#ضمن "hb-ot-var-common.hh"

نطاق OT {


بنية اشاریہ_map_subset_plan_t
{
  تعداد اشاریہ_map_اشاریہ_t {
    ADV_اشاریہ,
    LSB_اشاریہ,	/* dual as TSB */
    RSB_اشاریہ,	/* dual as BSB */
    VORG_اشاریہ
  };

  فراغ init (ثابت Delta_سیٹ_اشاریہ_Map  *اشاریہ_map,
	     hb_inc_bimap_t	     &outer_map,
	     hb_سمتیہ_t<hb_سیٹ_t *> &inner_sets,
	     ثابت hb_subset_plan_t  *plan,
	     منطقي bypass_خالی_تھاواں = سچ)
  {
    map_گنتی = 0;
    بھریا_map_گنتی = plan->new_منزل_old_gid_list.لمائی ?
		     plan->new_منزل_old_gid_list.پوچھ ().پہلا + 1 : 0;
    /* Only the advance mapping may be dropped in favor of implicit
     * glyph-id indexing (for the others a null offset means no deltas),
     * and only when the source was map-less too, preserving its form. */
    can_drop_منزل_implicit = !bypass_خالی_تھاواں && !اشاریہ_map;
    max_inners.init ();
    output_map.init ();

    إذا (bypass_خالی_تھاواں && (!اشاریہ_map || !اشاریہ_map->get_map_گنتی ())) أرجع;

    بنانشانی صحيح	آخری_val = (بنانشانی صحيح)-1;
    hb_codepoint_t	آخری_gid = HB_CODEPOINT_INVALID;

    max_inners.resize (inner_sets.لمائی);
    لكل (بنانشانی i = 0; i < inner_sets.لمائی; i++) max_inners[i] = 0;

    /* Search backwards for a map value different from the last map value */
    خودکار &new_منزل_old_gid_list = plan->new_منزل_old_gid_list;
    بنانشانی گنتی = new_منزل_old_gid_list.لمائی;
    إذا (!اشاریہ_map)
    {
      map_گنتی = new_منزل_old_gid_list.پوچھ ().پہلا + 1;
    }
    وإلا
    {
      لكل (بنانشانی j = گنتی; j; j--)
      {
        hb_codepoint_t gid = new_منزل_old_gid_list.arrayZ[j - 1].پہلا;
        hb_codepoint_t old_gid = new_منزل_old_gid_list.arrayZ[j - 1].second;
  
        بنانشانی صحيح v = اشاریہ_map->map (old_gid);
        إذا (آخری_gid == HB_CODEPOINT_INVALID)
        {
  	آخری_val = v;
  	آخری_gid = gid;
  	استمر;
        }
        إذا (v != آخری_val)
  	اكسر;
  
        آخری_gid = gid;
      }
  
      إذا (unlikely (آخری_gid == (hb_codepoint_t)-1)) أرجع;
      map_گنتی = آخری_gid + 1;
    }

    لكل (خودکار _ : plan->new_منزل_old_gid_list)
    {
      hb_codepoint_t gid = _.پہلا;
      إذا (gid >= map_گنتی) اكسر;

      hb_codepoint_t old_gid = _.second;
      بنانشانی صحيح v = اشاریہ_map ? اشاریہ_map->map (old_gid): old_gid;
      بنانشانی صحيح outer = v >> 16;
      بنانشانی صحيح inner = v & 0xFFFF;
      outer_map.جوڑو (outer);
      إذا (inner > max_inners[outer]) max_inners[outer] = inner;
      إذا (outer >= inner_sets.لمائی) أرجع;
      inner_sets[outer]->جوڑو (inner);
    }
  }

  فراغ fini ()
  {
    max_inners.fini ();
    output_map.fini ();
  }

  فراغ remap (ثابت Delta_سیٹ_اشاریہ_Map *داخلہ_map,
	      ثابت hb_inc_bimap_t &outer_map,
	      ثابت hb_سمتیہ_t<hb_inc_bimap_t> &inner_maps,
	      ثابت hb_subset_plan_t *plan)
  {
    outer_ثنائی_ہندسہ_گنتی = 1;
    inner_ثنائی_ہندسہ_گنتی = 1;

    لكل (بنانشانی صحيح i = 0; i < max_inners.لمائی; i++)
    {
      إذا (inner_maps[i].get_population () == 0) استمر;
      بنانشانی صحيح ثنائی_ہندسہ_گنتی = (max_inners[i]==0)? 1: hb_ثنائی_ہندسہ_storage (inner_maps[i][max_inners[i]]);
      إذا (ثنائی_ہندسہ_گنتی > inner_ثنائی_ہندسہ_گنتی) inner_ثنائی_ہندسہ_گنتی = ثنائی_ہندسہ_گنتی;
    }

    إذا (unlikely (!output_map.resize (map_گنتی))) أرجع;
    لكل (ثابت خودکار &_ : plan->new_منزل_old_gid_list)
    {
      hb_codepoint_t new_gid = _.پہلا;
      hb_codepoint_t old_gid = _.second;

      إذا (unlikely (new_gid >= map_گنتی)) اكسر;

      uint32_t v = داخلہ_map? داخلہ_map->map (old_gid) : old_gid;
      بنانشانی outer = v >> 16;
      بنانشانی new_outer = outer_map[outer];
      بنانشانی ثنائی_ہندسہ_گنتی = (new_outer == 0) ? 1 : hb_ثنائی_ہندسہ_storage (new_outer);
      outer_ثنائی_ہندسہ_گنتی = hb_max (ثنائی_ہندسہ_گنتی, outer_ثنائی_ہندسہ_گنتی);

      output_map.arrayZ[new_gid] = (new_outer << 16) | (inner_maps[outer][v & 0xFFFF]);
    }
  }

  منطقي remap_after_instantiation (ثابت hb_subset_plan_t *plan,
                                  ثابت hb_map_t& varidx_map)
  {
    /* recalculate bit_count after remapping */
    outer_ثنائی_ہندسہ_گنتی = 1;
    inner_ثنائی_ہندسہ_گنتی = 1;

    لكل (ثابت خودکار &_ : plan->new_منزل_old_gid_list)
    {
      hb_codepoint_t new_gid = _.پہلا;
      إذا (unlikely (new_gid >= map_گنتی)) اكسر;

      uint32_t v = output_map.arrayZ[new_gid];
      uint32_t *new_varidx;
      إذا (!varidx_map.has (v, &new_varidx))
        أرجع خطأ;

      output_map.arrayZ[new_gid] = *new_varidx;

      بنانشانی outer = (*new_varidx) >> 16;
      بنانشانی ثنائی_ہندسہ_گنتی = (outer == 0) ? 1 : hb_ثنائی_ہندسہ_storage (outer);
      outer_ثنائی_ہندسہ_گنتی = hb_max (ثنائی_ہندسہ_گنتی, outer_ثنائی_ہندسہ_گنتی);

      بنانشانی inner = (*new_varidx) & 0xFFFF;
      ثنائی_ہندسہ_گنتی = (inner == 0) ? 1 : hb_ثنائی_ہندسہ_storage (inner);
      inner_ثنائی_ہندسہ_گنتی = hb_max (ثنائی_ہندسہ_گنتی, inner_ثنائی_ہندسہ_گنتی);
    }
    أرجع سچ;
  }

  بنانشانی صحيح get_inner_ثنائی_ہندسہ_گنتی () ثابت { أرجع inner_ثنائی_ہندسہ_گنتی; }
  بنانشانی صحيح get_چوڑائی ()           ثابت { أرجع ((outer_ثنائی_ہندسہ_گنتی + inner_ثنائی_ہندسہ_گنتی + 7) / 8); }
  بنانشانی صحيح get_map_گنتی ()       ثابت { أرجع map_گنتی; }

  size_t get_ناپ () ثابت
  {
    إذا (!map_گنتی) أرجع 0;
    أرجع hb_unsigned_mul_جوڑو_saturate (get_چوڑائی (), map_گنتی,
					 Delta_سیٹ_اشاریہ_Map::min_ناپ);
  }

  منطقي is_پچھان_تت () ثابت
  {
    إذا (!output_map) أرجع سچ;
    /* An advance map whose entries are the identity over every output
     * glyph is equivalent to implicit glyph-id indexing; drop it.  A
     * truncated map doesn't qualify: glyphs past its end share its last
     * value, not their own ids. */
    إذا (!can_drop_منزل_implicit || map_گنتی != بھریا_map_گنتی) أرجع خطأ;
    لكل (بنانشانی i = 0; i < map_گنتی; i++)
      إذا (output_map.arrayZ[i] != i) أرجع خطأ;
    أرجع سچ;
  }
  hb_array_t<ثابت uint32_t> get_output_map () ثابت { أرجع output_map.as_array (); }

  محمي:
  بنانشانی صحيح map_گنتی;
  بنانشانی صحيح بھریا_map_گنتی;
  منطقي can_drop_منزل_implicit;
  hb_سمتیہ_t<بنانشانی صحيح> max_inners;
  بنانشانی صحيح outer_ثنائی_ہندسہ_گنتی;
  بنانشانی صحيح inner_ثنائی_ہندسہ_گنتی;
  hb_سمتیہ_t<uint32_t> output_map;
};

بنية hvarvvar_subset_plan_t
{
  hvarvvar_subset_plan_t() : inner_maps (), اشاریہ_map_plans () {}
  ~hvarvvar_subset_plan_t() { fini (); }

  فراغ init (ثابت hb_array_t<ثابت Delta_سیٹ_اشاریہ_Map *> &اشاریہ_maps,
	     ثابت شے_Variation_Store &_var_store,
	     ثابت hb_subset_plan_t *plan)
  {
    اشاریہ_map_plans.resize (اشاریہ_maps.لمائی);

    var_store = &_var_store;
    inner_sets.resize (var_store->get_sub_جدول_گنتی ());
    لكل (بنانشانی صحيح i = 0; i < inner_sets.لمائی; i++)
      inner_sets[i] = hb_سیٹ_create ();
    adv_سیٹ = hb_سیٹ_create ();

    inner_maps.resize (var_store->get_sub_جدول_گنتی ());

    إذا (unlikely (!اشاریہ_map_plans.لمائی || !inner_sets.لمائی || !inner_maps.لمائی)) أرجع;

    منطقي retain_adv_map = خطأ;
    اشاریہ_map_plans[0].init (اشاریہ_maps[0], outer_map, inner_sets, plan, خطأ);
    إذا (!اشاریہ_maps[0])
    {
      retain_adv_map = plan->flags & HB_SUBSET_FLAGS_RETAIN_GIDS;
      outer_map.جوڑو (0);
      لكل (hb_codepoint_t old_gid : plan->glyphset()->iter())
        inner_sets[0]->جوڑو (old_gid);
      hb_سیٹ_union (adv_سیٹ, inner_sets[0]);
    }

    لكل (بنانشانی صحيح i = 1; i < اشاریہ_maps.لمائی; i++)
      اشاریہ_map_plans[i].init (اشاریہ_maps[i], outer_map, inner_sets, plan);

    outer_map.sort ();

    إذا (retain_adv_map)
    {
      لكل (ثابت خودکار &_ : plan->new_منزل_old_gid_list)
      {
        hb_codepoint_t old_gid = _.second;
	inner_maps[0].جوڑو (old_gid);
      }
    }
    وإلا
    {
      inner_maps[0].جوڑو_سیٹ (adv_سیٹ);
      hb_سیٹ_گھٹاؤ (inner_sets[0], adv_سیٹ);
      inner_maps[0].جوڑو_سیٹ (inner_sets[0]);
    }

    لكل (بنانشانی صحيح i = 1; i < inner_maps.لمائی; i++)
      inner_maps[i].جوڑو_سیٹ (inner_sets[i]);

    لكل (بنانشانی صحيح i = 0; i < اشاریہ_maps.لمائی; i++)
      اشاریہ_map_plans[i].remap (اشاریہ_maps[i], outer_map, inner_maps, plan);
  }

  /* remap */
  منطقي remap_اشاریہ_map_plans (ثابت hb_subset_plan_t *plan,
                              ثابت hb_map_t& varidx_map)
  {
    لكل (بنانشانی i = 0; i < اشاریہ_map_plans.لمائی; i++)
      إذا (!اشاریہ_map_plans[i].remap_after_instantiation (plan, varidx_map))
        أرجع خطأ;
    أرجع سچ;
  }

  فراغ fini ()
  {
    لكل (بنانشانی صحيح i = 0; i < inner_sets.لمائی; i++)
      hb_سیٹ_destroy (inner_sets[i]);
    hb_سیٹ_destroy (adv_سیٹ);
    inner_maps.fini ();
    اشاریہ_map_plans.fini ();
  }

  hb_inc_bimap_t outer_map;
  hb_سمتیہ_t<hb_inc_bimap_t> inner_maps;
  hb_سمتیہ_t<اشاریہ_map_subset_plan_t> اشاریہ_map_plans;
  ثابت شے_Variation_Store *var_store;

  محمي:
  hb_سمتیہ_t<hb_سیٹ_t *> inner_sets;
  hb_سیٹ_t *adv_سیٹ;
};

/*
 * HVAR -- Horizontal Metrics Variations
 * https://docs.microsoft.com/en-us/typography/opentype/spec/hvar
 * VVAR -- Vertical Metrics Variations
 * https://docs.microsoft.com/en-us/typography/opentype/spec/vvar
 */
#تعریف_کرو HB_OT_ٹیگ_HVAR HB_ٹیگ('H','V','A','R')
#تعریف_کرو HB_OT_ٹیگ_VVAR HB_ٹیگ('V','V','A','R')

بنية HVARVVAR
{
  ساكن constexpr hb_ٹیگ_t HVARTag = HB_OT_ٹیگ_HVAR;
  ساكن constexpr hb_ٹیگ_t VVARTag = HB_OT_ٹیگ_VVAR;

  منطقي has_انکڑے () ثابت { أرجع نسخہ.major != 0; }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (نسخہ.sanitize (c) &&
		  hb_barrier () &&
		  likely (نسخہ.major == 1) &&
		  varStore.sanitize (c, هذا) &&
		  advMap.sanitize (c, هذا) &&
		  lsbMap.sanitize (c, هذا) &&
		  rsbMap.sanitize (c, هذا));
  }

  ثابت شے_Variation_Store& get_var_store () ثابت
  { أرجع هذا+varStore; }

  فراغ listup_اشاریہ_maps (hb_سمتیہ_t<ثابت Delta_سیٹ_اشاریہ_Map *> &اشاریہ_maps) ثابت
  {
    إذا (advMap) اشاریہ_maps.push (&(هذا+advMap));
    وإلا اشاریہ_maps.push (خالی_سوچک);

    إذا (lsbMap) اشاریہ_maps.push (&(هذا+lsbMap));
    وإلا اشاریہ_maps.push (خالی_سوچک);

    إذا (rsbMap) اشاریہ_maps.push (&(هذا+rsbMap));
    وإلا اشاریہ_maps.push (خالی_سوچک);
  }

  منطقي serialize_اشاریہ_maps (hb_serialize_context_t *c,
			     ثابت hb_array_t<اشاریہ_map_subset_plan_t> &im_plans)
  {
    TRACE_SERIALIZE (هذا);
    إذا (im_plans[اشاریہ_map_subset_plan_t::ADV_اشاریہ].is_پچھان_تت ())
      advMap = 0;
    وإلا إذا (unlikely (!advMap.serialize_serialize (c, im_plans[اشاریہ_map_subset_plan_t::ADV_اشاریہ])))
      return_trace (خطأ);
    إذا (im_plans[اشاریہ_map_subset_plan_t::LSB_اشاریہ].is_پچھان_تت ())
      lsbMap = 0;
    وإلا إذا (unlikely (!lsbMap.serialize_serialize (c, im_plans[اشاریہ_map_subset_plan_t::LSB_اشاریہ])))
      return_trace (خطأ);
    إذا (im_plans[اشاریہ_map_subset_plan_t::RSB_اشاریہ].is_پچھان_تت ())
      rsbMap = 0;
    وإلا إذا (unlikely (!rsbMap.serialize_serialize (c, im_plans[اشاریہ_map_subset_plan_t::RSB_اشاریہ])))
      return_trace (خطأ);

    return_trace (سچ);
  }

  سانچا <قسم_ناں T>
  منطقي _subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    إذا (c->plan->all_axes_pinned)
      return_trace (خطأ);

    hvarvvar_subset_plan_t	hvar_plan;
    hb_سمتیہ_t<ثابت Delta_سیٹ_اشاریہ_Map *>
				اشاریہ_maps;

    ((T*)هذا)->listup_اشاریہ_maps (اشاریہ_maps);
    hvar_plan.init (اشاریہ_maps.as_array (), هذا+varStore, c->plan);

    T *باہرلا = c->serializer->allocate_min<T> ();
    إذا (unlikely (!باہرلا)) return_trace (خطأ);

    باہرلا->نسخہ.major = 1;
    باہرلا->نسخہ.minor = 0;

    إذا (c->plan->normalized_coords)
    {
      شے_variations_t شے_vars;
      إذا (!شے_vars.instantiate (هذا+varStore, c->plan,
                                  advMap == 0 ? خطأ : سچ,
                                  خطأ, /* use_no_variation_idx = false */
                                  hvar_plan.inner_maps.as_array ()))
        return_trace (خطأ);

      إذا (!باہرلا->varStore.serialize_serialize (c->serializer,
                                              شے_vars.has_long_لفظ (),
                                              c->plan->axis_ٹیگ_سوچی,
                                              شے_vars.get_region_list (),
                                              شے_vars.get_vardata_encodings ()))
        return_trace (خطأ);

      /* if varstore is optimized, remap output_map */
      إذا (advMap)
      {
        إذا (!hvar_plan.remap_اشاریہ_map_plans (c->plan, شے_vars.get_varidx_map ()))
          return_trace (خطأ);
      }
    }
    وإلا
    {
      /* avar2 partial instancing: cull unreachable regions. */
      hb_سیٹ_t dead_regions;
      إذا (c->plan->has_avar2)
	hvar_plan.var_store->collect_dead_regions (c->plan->axes_old_اشاریہ_ٹیگ_map,
						   c->plan->avar2_reachable_ranges,
						   dead_regions);

      إذا (unlikely (!باہرلا->varStore
		    .serialize_serialize (c->serializer,
					  hvar_plan.var_store,
					  hvar_plan.inner_maps.as_array (),
					  dead_regions.get_population () ? &dead_regions : خالی_سوچک)))
      return_trace (خطأ);
    }

    return_trace (باہرلا->T::serialize_اشاریہ_maps (c->serializer,
						hvar_plan.اشاریہ_map_plans.as_array ()));
  }

  HB_ALWAYS_INLINE
  عائم get_advance_delta_unscaled (hb_codepoint_t  glyph,
				    ثابت صحيح *coords, بنانشانی صحيح coord_گنتی,
				    hb_scalar_cache_t *store_cache = خالی_سوچک) ثابت
  {
    /* Implicit mapping uses outer index zero and the glyph id as the
     * inner index. An oversized glyph id must not select another table. */
    إذا (!advMap && glyph > 0xFFFFu) أرجع 0.f;
    uint32_t varidx = (هذا+advMap).map (glyph);
    أرجع (هذا+varStore).get_delta (varidx,
				      coords, coord_گنتی,
				      store_cache);
  }

  عام:
  Fixed_نسخہ<>نسخہ;	/* Version of the metrics variation table
				 * initially set to 0x00010000u */
  Offset32_منزل<شے_Variation_Store>
		varStore;	/* Offset to item variation store table. */
  Offset32_منزل<Delta_سیٹ_اشاریہ_Map>
		advMap;		/* Offset to advance var-idx mapping. */
  Offset32_منزل<Delta_سیٹ_اشاریہ_Map>
		lsbMap;		/* Offset to lsb/tsb var-idx mapping. */
  Offset32_منزل<Delta_سیٹ_اشاریہ_Map>
		rsbMap;		/* Offset to rsb/bsb var-idx mapping. */

  عام:
  DEFINE_ناپ_STATIC (20);
};

بنية HVAR : HVARVVAR {
  ساكن constexpr hb_ٹیگ_t جدول_ٹیگ = HB_OT_ٹیگ_HVAR;
  منطقي subset (hb_subset_context_t *c) ثابت { أرجع HVARVVAR::_subset<HVAR> (c); }
};
بنية VVAR : HVARVVAR {
  ساكن constexpr hb_ٹیگ_t جدول_ٹیگ = HB_OT_ٹیگ_VVAR;

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (static_cast<ثابت HVARVVAR *> (هذا)->sanitize (c) &&
		  vorgMap.sanitize (c, هذا));
  }

  فراغ listup_اشاریہ_maps (hb_سمتیہ_t<ثابت Delta_سیٹ_اشاریہ_Map *> &اشاریہ_maps) ثابت
  {
    HVARVVAR::listup_اشاریہ_maps (اشاریہ_maps);
    إذا (vorgMap) اشاریہ_maps.push (&(هذا+vorgMap));
    وإلا اشاریہ_maps.push (خالی_سوچک);
  }

  منطقي serialize_اشاریہ_maps (hb_serialize_context_t *c,
			     ثابت hb_array_t<اشاریہ_map_subset_plan_t> &im_plans)
  {
    TRACE_SERIALIZE (هذا);
    إذا (unlikely (!HVARVVAR::serialize_اشاریہ_maps (c, im_plans)))
      return_trace (خطأ);
    إذا (!im_plans[اشاریہ_map_subset_plan_t::VORG_اشاریہ].get_map_گنتی ())
      vorgMap = 0;
    وإلا إذا (unlikely (!vorgMap.serialize_serialize (c, im_plans[اشاریہ_map_subset_plan_t::VORG_اشاریہ])))
      return_trace (خطأ);

    return_trace (سچ);
  }

  منطقي subset (hb_subset_context_t *c) ثابت { أرجع HVARVVAR::_subset<VVAR> (c); }

  HB_ALWAYS_INLINE
  عائم get_vorg_delta_unscaled (hb_codepoint_t glyph,
				 ثابت صحيح *coords, بنانشانی صحيح coord_گنتی,
				 hb_scalar_cache_t *store_cache = خالی_سوچک) ثابت
  {
    إذا (!vorgMap) أرجع 0.f;
    uint32_t varidx = (هذا+vorgMap).map (glyph);
    أرجع (هذا+varStore).get_delta (varidx,
				      coords, coord_گنتی,
				      store_cache);
  }

  محمي:
  Offset32_منزل<Delta_سیٹ_اشاریہ_Map>
		vorgMap;	/* Offset to vertical-origin var-idx mapping. */

  عام:
  DEFINE_ناپ_STATIC (24);
};

} /* namespace OT */


#جے_انت /* HB_OT_VAR_HVAR_TABLE_HH */
