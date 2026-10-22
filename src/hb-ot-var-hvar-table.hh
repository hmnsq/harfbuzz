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

#నిర్వచితం_కాకపోతే HB_OT_VAR_HVAR_పట్టిక_HH
#నిర్వచించు HB_OT_VAR_HVAR_పట్టిక_HH

#అన్తర్భావ "hb-ot-layout-common.hh"
#అన్తర్భావ "hb-ot-var-common.hh"

నామాకాశ OT {


నిర్మాణం సూచీ_map_subset_plan_t
{
  గణన సూచీ_map_సూచీ_t {
    ADV_సూచీ,
    LSB_సూచీ,	/* dual as TSB */
    RSB_సూచీ,	/* dual as BSB */
    VORG_సూచీ
  };

  శూన్య init (స్థిరరాశి Delta_సమితి_సూచీ_Map  *సూచీ_map,
	     hb_inc_bimap_t	     &outer_map,
	     hb_సదిశ_t<hb_సమితి_t *> &inner_sets,
	     స్థిరరాశి hb_subset_plan_t  *plan,
	     తార్కిక bypass_ఖాళీలు = నిజం)
  {
    map_లెక్క = 0;
    నిండిన_map_లెక్క = plan->new_గమ్యం_old_gid_list.నిడివి ?
		     plan->new_గమ్యం_old_gid_list.తోక ().మొదటి + 1 : 0;
    /* Only the advance mapping may be dropped in favor of implicit
     * glyph-id indexing (for the others a null offset means no deltas),
     * and only when the source was map-less too, preserving its form. */
    can_drop_గమ్యం_implicit = !bypass_ఖాళీలు && !సూచీ_map;
    max_inners.init ();
    output_map.init ();

    ఒకవేళ (bypass_ఖాళీలు && (!సూచీ_map || !సూచీ_map->get_map_లెక్క ())) తిరిగి;

    గుర్తులేని పూర్ణాంకం	చివరి_val = (గుర్తులేని పూర్ణాంకం)-1;
    hb_codepoint_t	చివరి_gid = HB_CODEPOINT_INVALID;

    max_inners.resize (inner_sets.నిడివి);
    కొరకు (గుర్తులేని i = 0; i < inner_sets.నిడివి; i++) max_inners[i] = 0;

    /* Search backwards for a map value different from the last map value */
    స్వయంచాలక &new_గమ్యం_old_gid_list = plan->new_గమ్యం_old_gid_list;
    గుర్తులేని లెక్క = new_గమ్యం_old_gid_list.నిడివి;
    ఒకవేళ (!సూచీ_map)
    {
      map_లెక్క = new_గమ్యం_old_gid_list.తోక ().మొదటి + 1;
    }
    లేకపోతే
    {
      కొరకు (గుర్తులేని j = లెక్క; j; j--)
      {
        hb_codepoint_t gid = new_గమ్యం_old_gid_list.arrayZ[j - 1].మొదటి;
        hb_codepoint_t old_gid = new_గమ్యం_old_gid_list.arrayZ[j - 1].second;
  
        గుర్తులేని పూర్ణాంకం v = సూచీ_map->map (old_gid);
        ఒకవేళ (చివరి_gid == HB_CODEPOINT_INVALID)
        {
  	చివరి_val = v;
  	చివరి_gid = gid;
  	కొనసాగు;
        }
        ఒకవేళ (v != చివరి_val)
  	ఆపు;
  
        చివరి_gid = gid;
      }
  
      ఒకవేళ (unlikely (చివరి_gid == (hb_codepoint_t)-1)) తిరిగి;
      map_లెక్క = చివరి_gid + 1;
    }

    కొరకు (స్వయంచాలక _ : plan->new_గమ్యం_old_gid_list)
    {
      hb_codepoint_t gid = _.మొదటి;
      ఒకవేళ (gid >= map_లెక్క) ఆపు;

      hb_codepoint_t old_gid = _.second;
      గుర్తులేని పూర్ణాంకం v = సూచీ_map ? సూచీ_map->map (old_gid): old_gid;
      గుర్తులేని పూర్ణాంకం outer = v >> 16;
      గుర్తులేని పూర్ణాంకం inner = v & 0xFFFF;
      outer_map.కూడు (outer);
      ఒకవేళ (inner > max_inners[outer]) max_inners[outer] = inner;
      ఒకవేళ (outer >= inner_sets.నిడివి) తిరిగి;
      inner_sets[outer]->కూడు (inner);
    }
  }

  శూన్య fini ()
  {
    max_inners.fini ();
    output_map.fini ();
  }

  శూన్య remap (స్థిరరాశి Delta_సమితి_సూచీ_Map *ఆదానం_map,
	      స్థిరరాశి hb_inc_bimap_t &outer_map,
	      స్థిరరాశి hb_సదిశ_t<hb_inc_bimap_t> &inner_maps,
	      స్థిరరాశి hb_subset_plan_t *plan)
  {
    outer_బిట్టు_లెక్క = 1;
    inner_బిట్టు_లెక్క = 1;

    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < max_inners.నిడివి; i++)
    {
      ఒకవేళ (inner_maps[i].get_population () == 0) కొనసాగు;
      గుర్తులేని పూర్ణాంకం బిట్టు_లెక్క = (max_inners[i]==0)? 1: hb_బిట్టు_storage (inner_maps[i][max_inners[i]]);
      ఒకవేళ (బిట్టు_లెక్క > inner_బిట్టు_లెక్క) inner_బిట్టు_లెక్క = బిట్టు_లెక్క;
    }

    ఒకవేళ (unlikely (!output_map.resize (map_లెక్క))) తిరిగి;
    కొరకు (స్థిరరాశి స్వయంచాలక &_ : plan->new_గమ్యం_old_gid_list)
    {
      hb_codepoint_t new_gid = _.మొదటి;
      hb_codepoint_t old_gid = _.second;

      ఒకవేళ (unlikely (new_gid >= map_లెక్క)) ఆపు;

      uint32_t v = ఆదానం_map? ఆదానం_map->map (old_gid) : old_gid;
      గుర్తులేని outer = v >> 16;
      గుర్తులేని new_outer = outer_map[outer];
      గుర్తులేని బిట్టు_లెక్క = (new_outer == 0) ? 1 : hb_బిట్టు_storage (new_outer);
      outer_బిట్టు_లెక్క = hb_max (బిట్టు_లెక్క, outer_బిట్టు_లెక్క);

      output_map.arrayZ[new_gid] = (new_outer << 16) | (inner_maps[outer][v & 0xFFFF]);
    }
  }

  తార్కిక remap_after_instantiation (స్థిరరాశి hb_subset_plan_t *plan,
                                  స్థిరరాశి hb_map_t& varidx_map)
  {
    /* recalculate bit_count after remapping */
    outer_బిట్టు_లెక్క = 1;
    inner_బిట్టు_లెక్క = 1;

    కొరకు (స్థిరరాశి స్వయంచాలక &_ : plan->new_గమ్యం_old_gid_list)
    {
      hb_codepoint_t new_gid = _.మొదటి;
      ఒకవేళ (unlikely (new_gid >= map_లెక్క)) ఆపు;

      uint32_t v = output_map.arrayZ[new_gid];
      uint32_t *new_varidx;
      ఒకవేళ (!varidx_map.has (v, &new_varidx))
        తిరిగి అబద్ధం;

      output_map.arrayZ[new_gid] = *new_varidx;

      గుర్తులేని outer = (*new_varidx) >> 16;
      గుర్తులేని బిట్టు_లెక్క = (outer == 0) ? 1 : hb_బిట్టు_storage (outer);
      outer_బిట్టు_లెక్క = hb_max (బిట్టు_లెక్క, outer_బిట్టు_లెక్క);

      గుర్తులేని inner = (*new_varidx) & 0xFFFF;
      బిట్టు_లెక్క = (inner == 0) ? 1 : hb_బిట్టు_storage (inner);
      inner_బిట్టు_లెక్క = hb_max (బిట్టు_లెక్క, inner_బిట్టు_లెక్క);
    }
    తిరిగి నిజం;
  }

  గుర్తులేని పూర్ణాంకం get_inner_బిట్టు_లెక్క () స్థిరరాశి { తిరిగి inner_బిట్టు_లెక్క; }
  గుర్తులేని పూర్ణాంకం get_వెడల్పు ()           స్థిరరాశి { తిరిగి ((outer_బిట్టు_లెక్క + inner_బిట్టు_లెక్క + 7) / 8); }
  గుర్తులేని పూర్ణాంకం get_map_లెక్క ()       స్థిరరాశి { తిరిగి map_లెక్క; }

  size_t get_కొలత () స్థిరరాశి
  {
    ఒకవేళ (!map_లెక్క) తిరిగి 0;
    తిరిగి hb_unsigned_mul_కూడు_saturate (get_వెడల్పు (), map_లెక్క,
					 Delta_సమితి_సూచీ_Map::min_కొలత);
  }

  తార్కిక is_తటస్థం () స్థిరరాశి
  {
    ఒకవేళ (!output_map) తిరిగి నిజం;
    /* An advance map whose entries are the identity over every output
     * glyph is equivalent to implicit glyph-id indexing; drop it.  A
     * truncated map doesn't qualify: glyphs past its end share its last
     * value, not their own ids. */
    ఒకవేళ (!can_drop_గమ్యం_implicit || map_లెక్క != నిండిన_map_లెక్క) తిరిగి అబద్ధం;
    కొరకు (గుర్తులేని i = 0; i < map_లెక్క; i++)
      ఒకవేళ (output_map.arrayZ[i] != i) తిరిగి అబద్ధం;
    తిరిగి నిజం;
  }
  hb_array_t<స్థిరరాశి uint32_t> get_output_map () స్థిరరాశి { తిరిగి output_map.as_array (); }

  రక్షిత:
  గుర్తులేని పూర్ణాంకం map_లెక్క;
  గుర్తులేని పూర్ణాంకం నిండిన_map_లెక్క;
  తార్కిక can_drop_గమ్యం_implicit;
  hb_సదిశ_t<గుర్తులేని పూర్ణాంకం> max_inners;
  గుర్తులేని పూర్ణాంకం outer_బిట్టు_లెక్క;
  గుర్తులేని పూర్ణాంకం inner_బిట్టు_లెక్క;
  hb_సదిశ_t<uint32_t> output_map;
};

నిర్మాణం hvarvvar_subset_plan_t
{
  hvarvvar_subset_plan_t() : inner_maps (), సూచీ_map_plans () {}
  ~hvarvvar_subset_plan_t() { fini (); }

  శూన్య init (స్థిరరాశి hb_array_t<స్థిరరాశి Delta_సమితి_సూచీ_Map *> &సూచీ_maps,
	     స్థిరరాశి అంశం_Variation_Store &_var_store,
	     స్థిరరాశి hb_subset_plan_t *plan)
  {
    సూచీ_map_plans.resize (సూచీ_maps.నిడివి);

    var_store = &_var_store;
    inner_sets.resize (var_store->get_sub_పట్టిక_లెక్క ());
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < inner_sets.నిడివి; i++)
      inner_sets[i] = hb_సమితి_create ();
    adv_సమితి = hb_సమితి_create ();

    inner_maps.resize (var_store->get_sub_పట్టిక_లెక్క ());

    ఒకవేళ (unlikely (!సూచీ_map_plans.నిడివి || !inner_sets.నిడివి || !inner_maps.నిడివి)) తిరిగి;

    తార్కిక retain_adv_map = అబద్ధం;
    సూచీ_map_plans[0].init (సూచీ_maps[0], outer_map, inner_sets, plan, అబద్ధం);
    ఒకవేళ (!సూచీ_maps[0])
    {
      retain_adv_map = plan->flags & HB_SUBSET_FLAGS_RETAIN_GIDS;
      outer_map.కూడు (0);
      కొరకు (hb_codepoint_t old_gid : plan->glyphset()->iter())
        inner_sets[0]->కూడు (old_gid);
      hb_సమితి_union (adv_సమితి, inner_sets[0]);
    }

    కొరకు (గుర్తులేని పూర్ణాంకం i = 1; i < సూచీ_maps.నిడివి; i++)
      సూచీ_map_plans[i].init (సూచీ_maps[i], outer_map, inner_sets, plan);

    outer_map.sort ();

    ఒకవేళ (retain_adv_map)
    {
      కొరకు (స్థిరరాశి స్వయంచాలక &_ : plan->new_గమ్యం_old_gid_list)
      {
        hb_codepoint_t old_gid = _.second;
	inner_maps[0].కూడు (old_gid);
      }
    }
    లేకపోతే
    {
      inner_maps[0].కూడు_సమితి (adv_సమితి);
      hb_సమితి_వ్యవకలనం (inner_sets[0], adv_సమితి);
      inner_maps[0].కూడు_సమితి (inner_sets[0]);
    }

    కొరకు (గుర్తులేని పూర్ణాంకం i = 1; i < inner_maps.నిడివి; i++)
      inner_maps[i].కూడు_సమితి (inner_sets[i]);

    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < సూచీ_maps.నిడివి; i++)
      సూచీ_map_plans[i].remap (సూచీ_maps[i], outer_map, inner_maps, plan);
  }

  /* remap */
  తార్కిక remap_సూచీ_map_plans (స్థిరరాశి hb_subset_plan_t *plan,
                              స్థిరరాశి hb_map_t& varidx_map)
  {
    కొరకు (గుర్తులేని i = 0; i < సూచీ_map_plans.నిడివి; i++)
      ఒకవేళ (!సూచీ_map_plans[i].remap_after_instantiation (plan, varidx_map))
        తిరిగి అబద్ధం;
    తిరిగి నిజం;
  }

  శూన్య fini ()
  {
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < inner_sets.నిడివి; i++)
      hb_సమితి_destroy (inner_sets[i]);
    hb_సమితి_destroy (adv_సమితి);
    inner_maps.fini ();
    సూచీ_map_plans.fini ();
  }

  hb_inc_bimap_t outer_map;
  hb_సదిశ_t<hb_inc_bimap_t> inner_maps;
  hb_సదిశ_t<సూచీ_map_subset_plan_t> సూచీ_map_plans;
  స్థిరరాశి అంశం_Variation_Store *var_store;

  రక్షిత:
  hb_సదిశ_t<hb_సమితి_t *> inner_sets;
  hb_సమితి_t *adv_సమితి;
};

/*
 * HVAR -- Horizontal Metrics Variations
 * https://docs.microsoft.com/en-us/typography/opentype/spec/hvar
 * VVAR -- Vertical Metrics Variations
 * https://docs.microsoft.com/en-us/typography/opentype/spec/vvar
 */
#నిర్వచించు HB_OT_ట్యాగ్_HVAR HB_ట్యాగ్('H','V','A','R')
#నిర్వచించు HB_OT_ట్యాగ్_VVAR HB_ట్యాగ్('V','V','A','R')

నిర్మాణం HVARVVAR
{
  స్థిర constexpr hb_ట్యాగ్_t HVARTag = HB_OT_ట్యాగ్_HVAR;
  స్థిర constexpr hb_ట్యాగ్_t VVARTag = HB_OT_ట్యాగ్_VVAR;

  తార్కిక has_దత్తాంశం () స్థిరరాశి { తిరిగి సంస్కరణ.major != 0; }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (సంస్కరణ.sanitize (c) &&
		  hb_barrier () &&
		  likely (సంస్కరణ.major == 1) &&
		  varStore.sanitize (c, ఇది) &&
		  advMap.sanitize (c, ఇది) &&
		  lsbMap.sanitize (c, ఇది) &&
		  rsbMap.sanitize (c, ఇది));
  }

  స్థిరరాశి అంశం_Variation_Store& get_var_store () స్థిరరాశి
  { తిరిగి ఇది+varStore; }

  శూన్య listup_సూచీ_maps (hb_సదిశ_t<స్థిరరాశి Delta_సమితి_సూచీ_Map *> &సూచీ_maps) స్థిరరాశి
  {
    ఒకవేళ (advMap) సూచీ_maps.push (&(ఇది+advMap));
    లేకపోతే సూచీ_maps.push (శూన్యసూచిక);

    ఒకవేళ (lsbMap) సూచీ_maps.push (&(ఇది+lsbMap));
    లేకపోతే సూచీ_maps.push (శూన్యసూచిక);

    ఒకవేళ (rsbMap) సూచీ_maps.push (&(ఇది+rsbMap));
    లేకపోతే సూచీ_maps.push (శూన్యసూచిక);
  }

  తార్కిక serialize_సూచీ_maps (hb_serialize_context_t *c,
			     స్థిరరాశి hb_array_t<సూచీ_map_subset_plan_t> &im_plans)
  {
    TRACE_SERIALIZE (ఇది);
    ఒకవేళ (im_plans[సూచీ_map_subset_plan_t::ADV_సూచీ].is_తటస్థం ())
      advMap = 0;
    లేకపోతే ఒకవేళ (unlikely (!advMap.serialize_serialize (c, im_plans[సూచీ_map_subset_plan_t::ADV_సూచీ])))
      return_trace (అబద్ధం);
    ఒకవేళ (im_plans[సూచీ_map_subset_plan_t::LSB_సూచీ].is_తటస్థం ())
      lsbMap = 0;
    లేకపోతే ఒకవేళ (unlikely (!lsbMap.serialize_serialize (c, im_plans[సూచీ_map_subset_plan_t::LSB_సూచీ])))
      return_trace (అబద్ధం);
    ఒకవేళ (im_plans[సూచీ_map_subset_plan_t::RSB_సూచీ].is_తటస్థం ())
      rsbMap = 0;
    లేకపోతే ఒకవేళ (unlikely (!rsbMap.serialize_serialize (c, im_plans[సూచీ_map_subset_plan_t::RSB_సూచీ])))
      return_trace (అబద్ధం);

    return_trace (నిజం);
  }

  మూస <రకం_పేరు T>
  తార్కిక _subset (hb_subset_context_t *c) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    ఒకవేళ (c->plan->all_axes_pinned)
      return_trace (అబద్ధం);

    hvarvvar_subset_plan_t	hvar_plan;
    hb_సదిశ_t<స్థిరరాశి Delta_సమితి_సూచీ_Map *>
				సూచీ_maps;

    ((T*)ఇది)->listup_సూచీ_maps (సూచీ_maps);
    hvar_plan.init (సూచీ_maps.as_array (), ఇది+varStore, c->plan);

    T *బహిర్గతం = c->serializer->allocate_min<T> ();
    ఒకవేళ (unlikely (!బహిర్గతం)) return_trace (అబద్ధం);

    బహిర్గతం->సంస్కరణ.major = 1;
    బహిర్గతం->సంస్కరణ.minor = 0;

    ఒకవేళ (c->plan->normalized_coords)
    {
      అంశం_variations_t అంశం_vars;
      ఒకవేళ (!అంశం_vars.instantiate (ఇది+varStore, c->plan,
                                  advMap == 0 ? అబద్ధం : నిజం,
                                  అబద్ధం, /* use_no_variation_idx = false */
                                  hvar_plan.inner_maps.as_array ()))
        return_trace (అబద్ధం);

      ఒకవేళ (!బహిర్గతం->varStore.serialize_serialize (c->serializer,
                                              అంశం_vars.has_long_మాట (),
                                              c->plan->axis_ట్యాగులు,
                                              అంశం_vars.get_region_list (),
                                              అంశం_vars.get_vardata_encodings ()))
        return_trace (అబద్ధం);

      /* if varstore is optimized, remap output_map */
      ఒకవేళ (advMap)
      {
        ఒకవేళ (!hvar_plan.remap_సూచీ_map_plans (c->plan, అంశం_vars.get_varidx_map ()))
          return_trace (అబద్ధం);
      }
    }
    లేకపోతే
    {
      /* avar2 partial instancing: cull unreachable regions. */
      hb_సమితి_t dead_regions;
      ఒకవేళ (c->plan->has_avar2)
	hvar_plan.var_store->collect_dead_regions (c->plan->axes_old_సూచీ_ట్యాగ్_map,
						   c->plan->avar2_reachable_ranges,
						   dead_regions);

      ఒకవేళ (unlikely (!బహిర్గతం->varStore
		    .serialize_serialize (c->serializer,
					  hvar_plan.var_store,
					  hvar_plan.inner_maps.as_array (),
					  dead_regions.get_population () ? &dead_regions : శూన్యసూచిక)))
      return_trace (అబద్ధం);
    }

    return_trace (బహిర్గతం->T::serialize_సూచీ_maps (c->serializer,
						hvar_plan.సూచీ_map_plans.as_array ()));
  }

  HB_ALWAYS_INLINE
  దశాంశం get_advance_delta_unscaled (hb_codepoint_t  glyph,
				    స్థిరరాశి పూర్ణాంకం *coords, గుర్తులేని పూర్ణాంకం coord_లెక్క,
				    hb_scalar_cache_t *store_cache = శూన్యసూచిక) స్థిరరాశి
  {
    /* Implicit mapping uses outer index zero and the glyph id as the
     * inner index. An oversized glyph id must not select another table. */
    ఒకవేళ (!advMap && glyph > 0xFFFFu) తిరిగి 0.f;
    uint32_t varidx = (ఇది+advMap).map (glyph);
    తిరిగి (ఇది+varStore).get_delta (varidx,
				      coords, coord_లెక్క,
				      store_cache);
  }

  బహిరంగ:
  Fixed_సంస్కరణ<>సంస్కరణ;	/* Version of the metrics variation table
				 * initially set to 0x00010000u */
  Offset32_గమ్యం<అంశం_Variation_Store>
		varStore;	/* Offset to item variation store table. */
  Offset32_గమ్యం<Delta_సమితి_సూచీ_Map>
		advMap;		/* Offset to advance var-idx mapping. */
  Offset32_గమ్యం<Delta_సమితి_సూచీ_Map>
		lsbMap;		/* Offset to lsb/tsb var-idx mapping. */
  Offset32_గమ్యం<Delta_సమితి_సూచీ_Map>
		rsbMap;		/* Offset to rsb/bsb var-idx mapping. */

  బహిరంగ:
  DEFINE_కొలత_STATIC (20);
};

నిర్మాణం HVAR : HVARVVAR {
  స్థిర constexpr hb_ట్యాగ్_t పట్టిక_ట్యాగ్ = HB_OT_ట్యాగ్_HVAR;
  తార్కిక subset (hb_subset_context_t *c) స్థిరరాశి { తిరిగి HVARVVAR::_subset<HVAR> (c); }
};
నిర్మాణం VVAR : HVARVVAR {
  స్థిర constexpr hb_ట్యాగ్_t పట్టిక_ట్యాగ్ = HB_OT_ట్యాగ్_VVAR;

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (static_cast<స్థిరరాశి HVARVVAR *> (ఇది)->sanitize (c) &&
		  vorgMap.sanitize (c, ఇది));
  }

  శూన్య listup_సూచీ_maps (hb_సదిశ_t<స్థిరరాశి Delta_సమితి_సూచీ_Map *> &సూచీ_maps) స్థిరరాశి
  {
    HVARVVAR::listup_సూచీ_maps (సూచీ_maps);
    ఒకవేళ (vorgMap) సూచీ_maps.push (&(ఇది+vorgMap));
    లేకపోతే సూచీ_maps.push (శూన్యసూచిక);
  }

  తార్కిక serialize_సూచీ_maps (hb_serialize_context_t *c,
			     స్థిరరాశి hb_array_t<సూచీ_map_subset_plan_t> &im_plans)
  {
    TRACE_SERIALIZE (ఇది);
    ఒకవేళ (unlikely (!HVARVVAR::serialize_సూచీ_maps (c, im_plans)))
      return_trace (అబద్ధం);
    ఒకవేళ (!im_plans[సూచీ_map_subset_plan_t::VORG_సూచీ].get_map_లెక్క ())
      vorgMap = 0;
    లేకపోతే ఒకవేళ (unlikely (!vorgMap.serialize_serialize (c, im_plans[సూచీ_map_subset_plan_t::VORG_సూచీ])))
      return_trace (అబద్ధం);

    return_trace (నిజం);
  }

  తార్కిక subset (hb_subset_context_t *c) స్థిరరాశి { తిరిగి HVARVVAR::_subset<VVAR> (c); }

  HB_ALWAYS_INLINE
  దశాంశం get_vorg_delta_unscaled (hb_codepoint_t glyph,
				 స్థిరరాశి పూర్ణాంకం *coords, గుర్తులేని పూర్ణాంకం coord_లెక్క,
				 hb_scalar_cache_t *store_cache = శూన్యసూచిక) స్థిరరాశి
  {
    ఒకవేళ (!vorgMap) తిరిగి 0.f;
    uint32_t varidx = (ఇది+vorgMap).map (glyph);
    తిరిగి (ఇది+varStore).get_delta (varidx,
				      coords, coord_లెక్క,
				      store_cache);
  }

  రక్షిత:
  Offset32_గమ్యం<Delta_సమితి_సూచీ_Map>
		vorgMap;	/* Offset to vertical-origin var-idx mapping. */

  బహిరంగ:
  DEFINE_కొలత_STATIC (24);
};

} /* namespace OT */


#అయితే_ముగింపు /* HB_OT_VAR_HVAR_TABLE_HH */
