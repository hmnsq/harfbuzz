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

#اگر_غیر_تعریف_شدہ HB_OT_VAR_HVAR_جدول_HH
#تعریف_کرو HB_OT_VAR_HVAR_جدول_HH

#شامل "hb-ot-layout-common.hh"
#شامل "hb-ot-var-common.hh"

نامخلا OT {


ساخت اشاریہ_map_subset_plan_t
{
  شمار اشاریہ_map_اشاریہ_t {
    ADV_اشاریہ,
    LSB_اشاریہ,	/* dual as TSB */
    RSB_اشاریہ,	/* dual as BSB */
    VORG_اشاریہ
  };

  خالی init (ثابت Delta_سیٹ_اشاریہ_Map  *اشاریہ_map,
	     hb_inc_bimap_t	     &outer_map,
	     hb_سمتیہ_t<hb_سیٹ_t *> &inner_sets,
	     ثابت hb_subset_plan_t  *plan,
	     منطقی bypass_خالی_خانے = سچ)
  {
    map_گنتی = 0;
    بھرا_map_گنتی = plan->new_مقصد_old_gid_list.لمبائی ?
		     plan->new_مقصد_old_gid_list.دم ().پہلا + 1 : 0;
    /* Only the advance mapping may be dropped in favor of implicit
     * glyph-id indexing (for the others a null offset means no deltas),
     * and only when the source was map-less too, preserving its form. */
    can_drop_مقصد_implicit = !bypass_خالی_خانے && !اشاریہ_map;
    max_inners.init ();
    output_map.init ();

    اگر (bypass_خالی_خانے && (!اشاریہ_map || !اشاریہ_map->get_map_گنتی ())) واپس;

    غیرعلامتی صحیح	آخری_val = (غیرعلامتی صحیح)-1;
    hb_codepoint_t	آخری_gid = HB_CODEPOINT_INVALID;

    max_inners.resize (inner_sets.لمبائی);
    برائے (غیرعلامتی i = 0; i < inner_sets.لمبائی; i++) max_inners[i] = 0;

    /* Search backwards for a map value different from the last map value */
    خودکار &new_مقصد_old_gid_list = plan->new_مقصد_old_gid_list;
    غیرعلامتی گنتی = new_مقصد_old_gid_list.لمبائی;
    اگر (!اشاریہ_map)
    {
      map_گنتی = new_مقصد_old_gid_list.دم ().پہلا + 1;
    }
    ورنہ
    {
      برائے (غیرعلامتی j = گنتی; j; j--)
      {
        hb_codepoint_t gid = new_مقصد_old_gid_list.arrayZ[j - 1].پہلا;
        hb_codepoint_t old_gid = new_مقصد_old_gid_list.arrayZ[j - 1].second;
  
        غیرعلامتی صحیح v = اشاریہ_map->map (old_gid);
        اگر (آخری_gid == HB_CODEPOINT_INVALID)
        {
  	آخری_val = v;
  	آخری_gid = gid;
  	جاری;
        }
        اگر (v != آخری_val)
  	توڑو;
  
        آخری_gid = gid;
      }
  
      اگر (unlikely (آخری_gid == (hb_codepoint_t)-1)) واپس;
      map_گنتی = آخری_gid + 1;
    }

    برائے (خودکار _ : plan->new_مقصد_old_gid_list)
    {
      hb_codepoint_t gid = _.پہلا;
      اگر (gid >= map_گنتی) توڑو;

      hb_codepoint_t old_gid = _.second;
      غیرعلامتی صحیح v = اشاریہ_map ? اشاریہ_map->map (old_gid): old_gid;
      غیرعلامتی صحیح outer = v >> 16;
      غیرعلامتی صحیح inner = v & 0xFFFF;
      outer_map.جوڑو (outer);
      اگر (inner > max_inners[outer]) max_inners[outer] = inner;
      اگر (outer >= inner_sets.لمبائی) واپس;
      inner_sets[outer]->جوڑو (inner);
    }
  }

  خالی fini ()
  {
    max_inners.fini ();
    output_map.fini ();
  }

  خالی remap (ثابت Delta_سیٹ_اشاریہ_Map *داخلہ_map,
	      ثابت hb_inc_bimap_t &outer_map,
	      ثابت hb_سمتیہ_t<hb_inc_bimap_t> &inner_maps,
	      ثابت hb_subset_plan_t *plan)
  {
    outer_ثنائی_ہندسہ_گنتی = 1;
    inner_ثنائی_ہندسہ_گنتی = 1;

    برائے (غیرعلامتی صحیح i = 0; i < max_inners.لمبائی; i++)
    {
      اگر (inner_maps[i].get_population () == 0) جاری;
      غیرعلامتی صحیح ثنائی_ہندسہ_گنتی = (max_inners[i]==0)? 1: hb_ثنائی_ہندسہ_storage (inner_maps[i][max_inners[i]]);
      اگر (ثنائی_ہندسہ_گنتی > inner_ثنائی_ہندسہ_گنتی) inner_ثنائی_ہندسہ_گنتی = ثنائی_ہندسہ_گنتی;
    }

    اگر (unlikely (!output_map.resize (map_گنتی))) واپس;
    برائے (ثابت خودکار &_ : plan->new_مقصد_old_gid_list)
    {
      hb_codepoint_t new_gid = _.پہلا;
      hb_codepoint_t old_gid = _.second;

      اگر (unlikely (new_gid >= map_گنتی)) توڑو;

      uint32_t v = داخلہ_map? داخلہ_map->map (old_gid) : old_gid;
      غیرعلامتی outer = v >> 16;
      غیرعلامتی new_outer = outer_map[outer];
      غیرعلامتی ثنائی_ہندسہ_گنتی = (new_outer == 0) ? 1 : hb_ثنائی_ہندسہ_storage (new_outer);
      outer_ثنائی_ہندسہ_گنتی = hb_max (ثنائی_ہندسہ_گنتی, outer_ثنائی_ہندسہ_گنتی);

      output_map.arrayZ[new_gid] = (new_outer << 16) | (inner_maps[outer][v & 0xFFFF]);
    }
  }

  منطقی remap_after_instantiation (ثابت hb_subset_plan_t *plan,
                                  ثابت hb_map_t& varidx_map)
  {
    /* recalculate bit_count after remapping */
    outer_ثنائی_ہندسہ_گنتی = 1;
    inner_ثنائی_ہندسہ_گنتی = 1;

    برائے (ثابت خودکار &_ : plan->new_مقصد_old_gid_list)
    {
      hb_codepoint_t new_gid = _.پہلا;
      اگر (unlikely (new_gid >= map_گنتی)) توڑو;

      uint32_t v = output_map.arrayZ[new_gid];
      uint32_t *new_varidx;
      اگر (!varidx_map.has (v, &new_varidx))
        واپس جھوٹ;

      output_map.arrayZ[new_gid] = *new_varidx;

      غیرعلامتی outer = (*new_varidx) >> 16;
      غیرعلامتی ثنائی_ہندسہ_گنتی = (outer == 0) ? 1 : hb_ثنائی_ہندسہ_storage (outer);
      outer_ثنائی_ہندسہ_گنتی = hb_max (ثنائی_ہندسہ_گنتی, outer_ثنائی_ہندسہ_گنتی);

      غیرعلامتی inner = (*new_varidx) & 0xFFFF;
      ثنائی_ہندسہ_گنتی = (inner == 0) ? 1 : hb_ثنائی_ہندسہ_storage (inner);
      inner_ثنائی_ہندسہ_گنتی = hb_max (ثنائی_ہندسہ_گنتی, inner_ثنائی_ہندسہ_گنتی);
    }
    واپس سچ;
  }

  غیرعلامتی صحیح get_inner_ثنائی_ہندسہ_گنتی () ثابت { واپس inner_ثنائی_ہندسہ_گنتی; }
  غیرعلامتی صحیح get_چوڑائی ()           ثابت { واپس ((outer_ثنائی_ہندسہ_گنتی + inner_ثنائی_ہندسہ_گنتی + 7) / 8); }
  غیرعلامتی صحیح get_map_گنتی ()       ثابت { واپس map_گنتی; }

  size_t get_سائز () ثابت
  {
    اگر (!map_گنتی) واپس 0;
    واپس hb_unsigned_mul_جوڑو_saturate (get_چوڑائی (), map_گنتی,
					 Delta_سیٹ_اشاریہ_Map::min_سائز);
  }

  منطقی is_شناختی_عنصر () ثابت
  {
    اگر (!output_map) واپس سچ;
    /* An advance map whose entries are the identity over every output
     * glyph is equivalent to implicit glyph-id indexing; drop it.  A
     * truncated map doesn't qualify: glyphs past its end share its last
     * value, not their own ids. */
    اگر (!can_drop_مقصد_implicit || map_گنتی != بھرا_map_گنتی) واپس جھوٹ;
    برائے (غیرعلامتی i = 0; i < map_گنتی; i++)
      اگر (output_map.arrayZ[i] != i) واپس جھوٹ;
    واپس سچ;
  }
  hb_array_t<ثابت uint32_t> get_output_map () ثابت { واپس output_map.as_array (); }

  محفوظ:
  غیرعلامتی صحیح map_گنتی;
  غیرعلامتی صحیح بھرا_map_گنتی;
  منطقی can_drop_مقصد_implicit;
  hb_سمتیہ_t<غیرعلامتی صحیح> max_inners;
  غیرعلامتی صحیح outer_ثنائی_ہندسہ_گنتی;
  غیرعلامتی صحیح inner_ثنائی_ہندسہ_گنتی;
  hb_سمتیہ_t<uint32_t> output_map;
};

ساخت hvarvvar_subset_plan_t
{
  hvarvvar_subset_plan_t() : inner_maps (), اشاریہ_map_plans () {}
  ~hvarvvar_subset_plan_t() { fini (); }

  خالی init (ثابت hb_array_t<ثابت Delta_سیٹ_اشاریہ_Map *> &اشاریہ_maps,
	     ثابت شے_Variation_Store &_var_store,
	     ثابت hb_subset_plan_t *plan)
  {
    اشاریہ_map_plans.resize (اشاریہ_maps.لمبائی);

    var_store = &_var_store;
    inner_sets.resize (var_store->get_sub_جدول_گنتی ());
    برائے (غیرعلامتی صحیح i = 0; i < inner_sets.لمبائی; i++)
      inner_sets[i] = hb_سیٹ_create ();
    adv_سیٹ = hb_سیٹ_create ();

    inner_maps.resize (var_store->get_sub_جدول_گنتی ());

    اگر (unlikely (!اشاریہ_map_plans.لمبائی || !inner_sets.لمبائی || !inner_maps.لمبائی)) واپس;

    منطقی retain_adv_map = جھوٹ;
    اشاریہ_map_plans[0].init (اشاریہ_maps[0], outer_map, inner_sets, plan, جھوٹ);
    اگر (!اشاریہ_maps[0])
    {
      retain_adv_map = plan->flags & HB_SUBSET_FLAGS_RETAIN_GIDS;
      outer_map.جوڑو (0);
      برائے (hb_codepoint_t old_gid : plan->glyphset()->iter())
        inner_sets[0]->جوڑو (old_gid);
      hb_سیٹ_union (adv_سیٹ, inner_sets[0]);
    }

    برائے (غیرعلامتی صحیح i = 1; i < اشاریہ_maps.لمبائی; i++)
      اشاریہ_map_plans[i].init (اشاریہ_maps[i], outer_map, inner_sets, plan);

    outer_map.sort ();

    اگر (retain_adv_map)
    {
      برائے (ثابت خودکار &_ : plan->new_مقصد_old_gid_list)
      {
        hb_codepoint_t old_gid = _.second;
	inner_maps[0].جوڑو (old_gid);
      }
    }
    ورنہ
    {
      inner_maps[0].جوڑو_سیٹ (adv_سیٹ);
      hb_سیٹ_تفریق (inner_sets[0], adv_سیٹ);
      inner_maps[0].جوڑو_سیٹ (inner_sets[0]);
    }

    برائے (غیرعلامتی صحیح i = 1; i < inner_maps.لمبائی; i++)
      inner_maps[i].جوڑو_سیٹ (inner_sets[i]);

    برائے (غیرعلامتی صحیح i = 0; i < اشاریہ_maps.لمبائی; i++)
      اشاریہ_map_plans[i].remap (اشاریہ_maps[i], outer_map, inner_maps, plan);
  }

  /* remap */
  منطقی remap_اشاریہ_map_plans (ثابت hb_subset_plan_t *plan,
                              ثابت hb_map_t& varidx_map)
  {
    برائے (غیرعلامتی i = 0; i < اشاریہ_map_plans.لمبائی; i++)
      اگر (!اشاریہ_map_plans[i].remap_after_instantiation (plan, varidx_map))
        واپس جھوٹ;
    واپس سچ;
  }

  خالی fini ()
  {
    برائے (غیرعلامتی صحیح i = 0; i < inner_sets.لمبائی; i++)
      hb_سیٹ_destroy (inner_sets[i]);
    hb_سیٹ_destroy (adv_سیٹ);
    inner_maps.fini ();
    اشاریہ_map_plans.fini ();
  }

  hb_inc_bimap_t outer_map;
  hb_سمتیہ_t<hb_inc_bimap_t> inner_maps;
  hb_سمتیہ_t<اشاریہ_map_subset_plan_t> اشاریہ_map_plans;
  ثابت شے_Variation_Store *var_store;

  محفوظ:
  hb_سمتیہ_t<hb_سیٹ_t *> inner_sets;
  hb_سیٹ_t *adv_سیٹ;
};

/*
 * HVAR -- Horizontal Metrics Variations
 * https://docs.microsoft.com/en-us/typography/opentype/spec/hvar
 * VVAR -- Vertical Metrics Variations
 * https://docs.microsoft.com/en-us/typography/opentype/spec/vvar
 */
#تعریف_کرو HB_OT_لیبل_HVAR HB_لیبل('H','V','A','R')
#تعریف_کرو HB_OT_لیبل_VVAR HB_لیبل('V','V','A','R')

ساخت HVARVVAR
{
  ساکن constexpr hb_لیبل_t HVARTag = HB_OT_لیبل_HVAR;
  ساکن constexpr hb_لیبل_t VVARTag = HB_OT_لیبل_VVAR;

  منطقی has_ڈیٹا () ثابت { واپس نسخہ.major != 0; }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (نسخہ.sanitize (c) &&
		  hb_barrier () &&
		  likely (نسخہ.major == 1) &&
		  varStore.sanitize (c, یہ) &&
		  advMap.sanitize (c, یہ) &&
		  lsbMap.sanitize (c, یہ) &&
		  rsbMap.sanitize (c, یہ));
  }

  ثابت شے_Variation_Store& get_var_store () ثابت
  { واپس یہ+varStore; }

  خالی listup_اشاریہ_maps (hb_سمتیہ_t<ثابت Delta_سیٹ_اشاریہ_Map *> &اشاریہ_maps) ثابت
  {
    اگر (advMap) اشاریہ_maps.push (&(یہ+advMap));
    ورنہ اشاریہ_maps.push (خالی_اشاریہ);

    اگر (lsbMap) اشاریہ_maps.push (&(یہ+lsbMap));
    ورنہ اشاریہ_maps.push (خالی_اشاریہ);

    اگر (rsbMap) اشاریہ_maps.push (&(یہ+rsbMap));
    ورنہ اشاریہ_maps.push (خالی_اشاریہ);
  }

  منطقی serialize_اشاریہ_maps (hb_serialize_context_t *c,
			     ثابت hb_array_t<اشاریہ_map_subset_plan_t> &im_plans)
  {
    TRACE_SERIALIZE (یہ);
    اگر (im_plans[اشاریہ_map_subset_plan_t::ADV_اشاریہ].is_شناختی_عنصر ())
      advMap = 0;
    ورنہ اگر (unlikely (!advMap.serialize_serialize (c, im_plans[اشاریہ_map_subset_plan_t::ADV_اشاریہ])))
      return_trace (جھوٹ);
    اگر (im_plans[اشاریہ_map_subset_plan_t::LSB_اشاریہ].is_شناختی_عنصر ())
      lsbMap = 0;
    ورنہ اگر (unlikely (!lsbMap.serialize_serialize (c, im_plans[اشاریہ_map_subset_plan_t::LSB_اشاریہ])))
      return_trace (جھوٹ);
    اگر (im_plans[اشاریہ_map_subset_plan_t::RSB_اشاریہ].is_شناختی_عنصر ())
      rsbMap = 0;
    ورنہ اگر (unlikely (!rsbMap.serialize_serialize (c, im_plans[اشاریہ_map_subset_plan_t::RSB_اشاریہ])))
      return_trace (جھوٹ);

    return_trace (سچ);
  }

  سانچہ <قسم_نام T>
  منطقی _subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (یہ);
    اگر (c->plan->all_axes_pinned)
      return_trace (جھوٹ);

    hvarvvar_subset_plan_t	hvar_plan;
    hb_سمتیہ_t<ثابت Delta_سیٹ_اشاریہ_Map *>
				اشاریہ_maps;

    ((T*)یہ)->listup_اشاریہ_maps (اشاریہ_maps);
    hvar_plan.init (اشاریہ_maps.as_array (), یہ+varStore, c->plan);

    T *برآمد = c->serializer->allocate_min<T> ();
    اگر (unlikely (!برآمد)) return_trace (جھوٹ);

    برآمد->نسخہ.major = 1;
    برآمد->نسخہ.minor = 0;

    اگر (c->plan->normalized_coords)
    {
      شے_variations_t شے_vars;
      اگر (!شے_vars.instantiate (یہ+varStore, c->plan,
                                  advMap == 0 ? جھوٹ : سچ,
                                  جھوٹ, /* use_no_variation_idx = false */
                                  hvar_plan.inner_maps.as_array ()))
        return_trace (جھوٹ);

      اگر (!برآمد->varStore.serialize_serialize (c->serializer,
                                              شے_vars.has_long_لفظ (),
                                              c->plan->axis_لیبل_فہرست,
                                              شے_vars.get_region_list (),
                                              شے_vars.get_vardata_encodings ()))
        return_trace (جھوٹ);

      /* if varstore is optimized, remap output_map */
      اگر (advMap)
      {
        اگر (!hvar_plan.remap_اشاریہ_map_plans (c->plan, شے_vars.get_varidx_map ()))
          return_trace (جھوٹ);
      }
    }
    ورنہ
    {
      /* avar2 partial instancing: cull unreachable regions. */
      hb_سیٹ_t dead_regions;
      اگر (c->plan->has_avar2)
	hvar_plan.var_store->collect_dead_regions (c->plan->axes_old_اشاریہ_لیبل_map,
						   c->plan->avar2_reachable_ranges,
						   dead_regions);

      اگر (unlikely (!برآمد->varStore
		    .serialize_serialize (c->serializer,
					  hvar_plan.var_store,
					  hvar_plan.inner_maps.as_array (),
					  dead_regions.get_population () ? &dead_regions : خالی_اشاریہ)))
      return_trace (جھوٹ);
    }

    return_trace (برآمد->T::serialize_اشاریہ_maps (c->serializer,
						hvar_plan.اشاریہ_map_plans.as_array ()));
  }

  HB_ALWAYS_INLINE
  اعشاری get_advance_delta_unscaled (hb_codepoint_t  glyph,
				    ثابت صحیح *coords, غیرعلامتی صحیح coord_گنتی,
				    hb_scalar_cache_t *store_cache = خالی_اشاریہ) ثابت
  {
    /* Implicit mapping uses outer index zero and the glyph id as the
     * inner index. An oversized glyph id must not select another table. */
    اگر (!advMap && glyph > 0xFFFFu) واپس 0.f;
    uint32_t varidx = (یہ+advMap).map (glyph);
    واپس (یہ+varStore).get_delta (varidx,
				      coords, coord_گنتی,
				      store_cache);
  }

  عوامی:
  Fixed_نسخہ<>نسخہ;	/* Version of the metrics variation table
				 * initially set to 0x00010000u */
  Offset32_مقصد<شے_Variation_Store>
		varStore;	/* Offset to item variation store table. */
  Offset32_مقصد<Delta_سیٹ_اشاریہ_Map>
		advMap;		/* Offset to advance var-idx mapping. */
  Offset32_مقصد<Delta_سیٹ_اشاریہ_Map>
		lsbMap;		/* Offset to lsb/tsb var-idx mapping. */
  Offset32_مقصد<Delta_سیٹ_اشاریہ_Map>
		rsbMap;		/* Offset to rsb/bsb var-idx mapping. */

  عوامی:
  DEFINE_سائز_STATIC (20);
};

ساخت HVAR : HVARVVAR {
  ساکن constexpr hb_لیبل_t جدول_لیبل = HB_OT_لیبل_HVAR;
  منطقی subset (hb_subset_context_t *c) ثابت { واپس HVARVVAR::_subset<HVAR> (c); }
};
ساخت VVAR : HVARVVAR {
  ساکن constexpr hb_لیبل_t جدول_لیبل = HB_OT_لیبل_VVAR;

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (static_cast<ثابت HVARVVAR *> (یہ)->sanitize (c) &&
		  vorgMap.sanitize (c, یہ));
  }

  خالی listup_اشاریہ_maps (hb_سمتیہ_t<ثابت Delta_سیٹ_اشاریہ_Map *> &اشاریہ_maps) ثابت
  {
    HVARVVAR::listup_اشاریہ_maps (اشاریہ_maps);
    اگر (vorgMap) اشاریہ_maps.push (&(یہ+vorgMap));
    ورنہ اشاریہ_maps.push (خالی_اشاریہ);
  }

  منطقی serialize_اشاریہ_maps (hb_serialize_context_t *c,
			     ثابت hb_array_t<اشاریہ_map_subset_plan_t> &im_plans)
  {
    TRACE_SERIALIZE (یہ);
    اگر (unlikely (!HVARVVAR::serialize_اشاریہ_maps (c, im_plans)))
      return_trace (جھوٹ);
    اگر (!im_plans[اشاریہ_map_subset_plan_t::VORG_اشاریہ].get_map_گنتی ())
      vorgMap = 0;
    ورنہ اگر (unlikely (!vorgMap.serialize_serialize (c, im_plans[اشاریہ_map_subset_plan_t::VORG_اشاریہ])))
      return_trace (جھوٹ);

    return_trace (سچ);
  }

  منطقی subset (hb_subset_context_t *c) ثابت { واپس HVARVVAR::_subset<VVAR> (c); }

  HB_ALWAYS_INLINE
  اعشاری get_vorg_delta_unscaled (hb_codepoint_t glyph,
				 ثابت صحیح *coords, غیرعلامتی صحیح coord_گنتی,
				 hb_scalar_cache_t *store_cache = خالی_اشاریہ) ثابت
  {
    اگر (!vorgMap) واپس 0.f;
    uint32_t varidx = (یہ+vorgMap).map (glyph);
    واپس (یہ+varStore).get_delta (varidx,
				      coords, coord_گنتی,
				      store_cache);
  }

  محفوظ:
  Offset32_مقصد<Delta_سیٹ_اشاریہ_Map>
		vorgMap;	/* Offset to vertical-origin var-idx mapping. */

  عوامی:
  DEFINE_سائز_STATIC (24);
};

} /* namespace OT */


#اگر_ختم /* HB_OT_VAR_HVAR_TABLE_HH */
