/*
 * Copyright © 2018 Adobe Inc.
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
 * Adobe Author(s): Michiharu Ariza
 */

#అన్తర్భావ "hb.hh"

#నిర్వచితం_కాకపోతే HB_NO_SUBSET_CFF

#అన్తర్భావ "hb-open-type.hh"
#అన్తర్భావ "hb-ot-cff2-table.hh"
#అన్తర్భావ "hb-ot-var-common.hh"
#అన్తర్భావ "hb-set.h"
#అన్తర్భావ "hb-subset-plan.hh"
#అన్తర్భావ "hb-subset-cff-common.hh"
#అన్తర్భావ "hb-cff2-interp-cs.hh"
#అన్తర్భావ "hb-subset-cff2-to-cff1.hh"

ఉపయోగించి నామాకాశ CFF;

నామాకాశ CFF {

/*
 * Partial instancing (some axes pinned or limited, others kept).
 *
 * This is a port of fontTools' instantiateCFF2 (fonttools/fonttools#3506),
 * with one simplification: the variation-store instancing solver acts on
 * regions independently of the per-blend delta values, so instead of lifting
 * every blend's deltas into the store, instancing it, and reading the rows
 * back, we instance each VarData once with identity delta rows.  The
 * instantiated tuples then give, per VarData, a fixed linear map from old
 * per-region deltas to new per-region deltas.  The contribution that folds
 * into each blend's default value is the old store's region scalars
 * evaluated at the new default location (plan->normalized_coords); the
 * surviving rebased regions all evaluate to zero there.
 *
 * Use: create (), then note_use () for every blend the retained charstrings
 * and private dicts hold, then finalize (); only VarDatas that are both
 * still variable and actually used survive into the new store.
 */
నిర్మాణం cff2_instancing_plan_t
{
  నిర్మాణం vardata_transform_t
  {
    /* new_deltas[j] = Σ_i matrix[j][i] * old_deltas[i] */
    hb_సదిశ_t<hb_సదిశ_t<దశాంశం>> మాతృక;
    /* Default-value fold-in scalars, one per old region: the old store's
     * region scalars at the new default location. */
    hb_సదిశ_t<దశాంశం> gains;
    /* Surviving regions, parallel to matrix rows. */
    hb_సదిశ_t<hb_hashmap_t<hb_ట్యాగ్_t, Triple>> regions;
    /* Index into the new global region list, per new region (finalize). */
    hb_సదిశ_t<గుర్తులేని> region_indices;
    గుర్తులేని new_major = 0;
    తార్కిక alive_ = అబద్ధం;
    తార్కిక alive () స్థిరరాశి { తిరిగి alive_; }

    /* One value's delta for new region j, from its old per-region deltas. */
    ద్విగుణ transform_delta (గుర్తులేని j, hb_array_t<స్థిరరాశి సంఖ్య_t> deltas) స్థిరరాశి
    {
      స్థిరరాశి hb_సదిశ_t<దశాంశం> &నిలువు = మాతృక.arrayZ[j];
      ద్విగుణ d = 0.;
      గుర్తులేని లెక్క = hb_min (నిలువు.నిడివి, deltas.నిడివి);
      కొరకు (గుర్తులేని c = 0; c < లెక్క; c++)
	d += (ద్విగుణ) నిలువు.arrayZ[c] * deltas.arrayZ[c].గమ్యం_real ();
      తిరిగి d;
    }
  };

  hb_సదిశ_t<vardata_transform_t> transforms;  /* per old VarData (vsindex) */
  hb_సదిశ_t<hb_hashmap_t<hb_ట్యాగ్_t, Triple>> new_regions;  /* global region list */
  hb_సమితి_t వాడిన_majors;
  గుర్తులేని new_major_లెక్క = 0;

  తార్కిక has_variations () స్థిరరాశి { తిరిగి new_major_లెక్క; }

  స్థిరరాశి vardata_transform_t *get_transform (గుర్తులేని major) స్థిరరాశి
  { తిరిగి major < transforms.నిడివి ? &transforms.arrayZ[major] : శూన్యసూచిక; }

  /* Record that a retained blend references this VarData. */
  శూన్య note_use (గుర్తులేని major) { వాడిన_majors.కూడు (major); }

  /* The new vsindex a private dict's (or charstring's) old ivs maps to;
   * dead VarDatas fall back to 0, their blends having become static. */
  గుర్తులేని remap_ivs (గుర్తులేని old_ivs) స్థిరరాశి
  {
    స్థిరరాశి vardata_transform_t *t = get_transform (old_ivs);
    తిరిగి (t && t->alive ()) ? t->new_major : 0;
  }

  ద్విగుణ fold_deltas (గుర్తులేని old_ivs, hb_array_t<స్థిరరాశి సంఖ్య_t> deltas) స్థిరరాశి
  {
    స్థిరరాశి vardata_transform_t *t = get_transform (old_ivs);
    ఒకవేళ (unlikely (!t || t->gains.నిడివి != deltas.నిడివి)) తిరిగి 0.;
    ద్విగుణ v = 0.;
    కొరకు (గుర్తులేని i = 0; i < deltas.నిడివి; i++)
      v += (ద్విగుణ) t->gains.arrayZ[i] * deltas.arrayZ[i].గమ్యం_real ();
    తిరిగి v;
  }

  తార్కిక create (స్థిరరాశి CFF2_అంశం_Variation_Store *cff2VarStore,
	       స్థిరరాశి hb_subset_plan_t *plan)
  {
    స్థిరరాశి OT::అంశం_Variation_Store &varStore = cff2VarStore->varStore;
    గుర్తులేని major_లెక్క = varStore.get_sub_పట్టిక_లెక్క ();
    ఒకవేళ (!transforms.resize (major_లెక్క)) తిరిగి అబద్ధం;

    ఒకవేళ (!varStore.get_region_list ().get_var_regions (plan->axes_old_సూచీ_ట్యాగ్_map, orig_regions))
      తిరిగి అబద్ధం;

    /* Instance each VarData with identity delta rows; the surviving tuples'
     * delta columns are the old-region → new-region transform. */
    OT::optimize_scratch_t scratch;
    కొరకు (గుర్తులేని m = 0; m < major_లెక్క; m++)
    {
      స్థిరరాశి OT::Var_దత్తాంశం &var_దత్తాంశం = varStore.get_sub_పట్టిక (m);
      గుర్తులేని k = var_దత్తాంశం.get_region_సూచీ_లెక్క ();
      vardata_transform_t &t = transforms.arrayZ[m];

      ఒకవేళ (!t.gains.resize_exact (k)) తిరిగి అబద్ధం;
      varStore.get_region_scalars (m, plan->normalized_coords.arrayZ,
				   plan->normalized_coords.నిడివి,
				   t.gains.arrayZ, k);

      OT::tuple_variations_t tuples;
      కొరకు (గుర్తులేని r = 0; r < k; r++)
      {
	OT::tuple_delta_t tuple;
	ఒకవేళ (!tuple.deltas_x.resize (k) ||
	    !tuple.indices.resize (k))
	  తిరిగి అబద్ధం;
	కొరకు (గుర్తులేని i = 0; i < k; i++)
	  tuple.indices.arrayZ[i] = నిజం;
	tuple.deltas_x.arrayZ[r] = 1.f;
	గుర్తులేని region_సూచీ = var_దత్తాంశం.get_region_సూచీ (r);
	ఒకవేళ (unlikely (region_సూచీ >= orig_regions.నిడివి)) తిరిగి అబద్ధం;
	tuple.axis_tuples = orig_regions.arrayZ[region_సూచీ];
	ఒకవేళ (unlikely (tuple.axis_tuples.లోపల_error ())) తిరిగి అబద్ధం;
	tuples.tuple_vars.push (std::move (tuple));
      }
      ఒకవేళ (unlikely (tuples.tuple_vars.లోపల_error ())) తిరిగి అబద్ధం;

      ఒకవేళ (!tuples.instantiate (plan->axes_location, plan->axes_triple_distances, scratch))
	తిరిగి అబద్ధం;

      కొరకు (స్వయంచాలక &tuple : tuples.tuple_vars)
      {
	ఒకవేళ (unlikely (tuple.deltas_x.నిడివి != k)) తిరిగి అబద్ధం;
	hb_సదిశ_t<దశాంశం> నిలువు;
	ఒకవేళ (unlikely (!నిలువు.resize_exact (k))) తిరిగి అబద్ధం;
	కొరకు (గుర్తులేని i = 0; i < k; i++)
	  నిలువు.arrayZ[i] = tuple.deltas_x.arrayZ[i];
	t.మాతృక.push (std::move (నిలువు));
	t.regions.push (std::move (tuple.axis_tuples));
      }
      ఒకవేళ (unlikely (t.మాతృక.లోపల_error () || t.regions.లోపల_error ())) తిరిగి అబద్ధం;
    }

    తిరిగి నిజం;
  }

  /* Decide which VarDatas survive, number them, and build the new global
   * region list: surviving original regions first, in their original order,
   * then solver-split new regions in order of appearance (fontTools'
   * rebuildRegions + prune_regions). */
  తార్కిక finalize ()
  {
    కొరకు (గుర్తులేని m = 0; m < transforms.నిడివి; m++)
    {
      vardata_transform_t &t = transforms.arrayZ[m];
      t.alive_ = t.మాతృక.నిడివి && వాడిన_majors.has (m);
      ఒకవేళ (t.alive_)
	t.new_major = new_major_లెక్క++;
    }

    /* Keys hash by content, so pointers into different owners compare fine. */
    hb_hashmap_t<స్థిరరాశి hb_hashmap_t<hb_ట్యాగ్_t, Triple>*, గుర్తులేని> region_idx_map;
    hb_సదిశ_t<స్థిరరాశి hb_hashmap_t<hb_ట్యాగ్_t, Triple>*> region_ptrs;
    కొరకు (స్థిరరాశి vardata_transform_t &t : transforms)
    {
      ఒకవేళ (!t.alive ()) కొనసాగు;
      కొరకు (స్థిరరాశి స్వయంచాలక &region : t.regions)
	ఒకవేళ (!region_idx_map.has (&region))
	  ఒకవేళ (unlikely (!region_idx_map.సమితి (&region, (గుర్తులేని) -1)))
	    తిరిగి అబద్ధం;
    }
    కొరకు (స్థిరరాశి స్వయంచాలక &r : orig_regions)
    {
      గుర్తులేని *idx;
      ఒకవేళ (region_idx_map.has (&r, &idx) && *idx == (గుర్తులేని) -1)
      {
	ఒకవేళ (unlikely (!region_idx_map.సమితి (&r, region_ptrs.నిడివి))) తిరిగి అబద్ధం;
	region_ptrs.push (&r);
      }
    }
    కొరకు (స్థిరరాశి vardata_transform_t &t : transforms)
    {
      ఒకవేళ (!t.alive ()) కొనసాగు;
      కొరకు (స్థిరరాశి స్వయంచాలక &region : t.regions)
      {
	గుర్తులేని *idx;
	ఒకవేళ (region_idx_map.has (&region, &idx) && *idx == (గుర్తులేని) -1)
	{
	  ఒకవేళ (unlikely (!region_idx_map.సమితి (&region, region_ptrs.నిడివి))) తిరిగి అబద్ధం;
	  region_ptrs.push (&region);
	}
      }
    }
    ఒకవేళ (unlikely (region_ptrs.లోపల_error ())) తిరిగి అబద్ధం;

    కొరకు (vardata_transform_t &t : transforms)
    {
      ఒకవేళ (!t.alive ()) కొనసాగు;
      కొరకు (స్థిరరాశి స్వయంచాలక &region : t.regions)
      {
	గుర్తులేని *idx;
	ఒకవేళ (unlikely (!region_idx_map.has (&region, &idx))) తిరిగి అబద్ధం;
	t.region_indices.push (*idx);
      }
      ఒకవేళ (unlikely (t.region_indices.లోపల_error ())) తిరిగి అబద్ధం;
    }

    ఒకవేళ (!new_regions.resize (region_ptrs.నిడివి)) తిరిగి అబద్ధం;
    కొరకు (గుర్తులేని i = 0; i < region_ptrs.నిడివి; i++)
    {
      new_regions.arrayZ[i] = *region_ptrs.arrayZ[i];
      ఒకవేళ (unlikely (new_regions.arrayZ[i].లోపల_error ())) తిరిగి అబద్ధం;
    }

    తిరిగి నిజం;
  }

  /* Serialize the instanced store: a CFF2ItemVariationStore whose VarDatas
   * carry no items, just region indexes (blend deltas live in charstrings).
   * Laid out manually since all sizes are known up front. */
  తార్కిక serialize_var_store (hb_serialize_context_t *c,
			    స్థిరరాశి hb_సదిశ_t<hb_ట్యాగ్_t> &axis_ట్యాగులు) స్థిరరాశి
  {
    గుర్తులేని దత్తాంశం_లెక్క = new_major_లెక్క;
    గుర్తులేని శీర్షిక_కొలత = 2 + 4 + 2 + 4 * దత్తాంశం_లెక్క;

    uint64_t స్థానభ్రంశం = శీర్షిక_కొలత;
    hb_సదిశ_t<గుర్తులేని> దత్తాంశం_offsets;
    కొరకు (స్థిరరాశి vardata_transform_t &t : transforms)
    {
      ఒకవేళ (!t.alive ()) కొనసాగు;
      దత్తాంశం_offsets.push ((గుర్తులేని) స్థానభ్రంశం);
      స్థానభ్రంశం += 6 + 2 * (uint64_t) t.region_indices.నిడివి;
    }
    ఒకవేళ (unlikely (దత్తాంశం_offsets.లోపల_error () || దత్తాంశం_offsets.నిడివి != దత్తాంశం_లెక్క))
      తిరిగి అబద్ధం;
    గుర్తులేని regions_స్థానభ్రంశం = (గుర్తులేని) స్థానభ్రంశం;
    uint64_t మొత్తం = స్థానభ్రంశం + 4 + (uint64_t) new_regions.నిడివి * axis_ట్యాగులు.నిడివి * 6;
    ఒకవేళ (unlikely (మొత్తం > 0xFFFFFFFFu)) తిరిగి అబద్ధం;

    HBUINT16 *కొలత_క్షేత్రం = c->allocate_కొలత<HBUINT16> (HBUINT16::static_కొలత);
    ఒకవేళ (unlikely (!కొలత_క్షేత్రం)) తిరిగి అబద్ధం;
    /* OFF 2026 5.3.3.11.2 permits larger stores with length=65535. */
    *కొలత_క్షేత్రం = hb_min (మొత్తం, (uint64_t) 0xFFFFu);

    అక్షరం *శీర్షిక = c->allocate_కొలత<అక్షరం> (శీర్షిక_కొలత);
    ఒకవేళ (unlikely (!శీర్షిక)) తిరిగి అబద్ధం;
    * (HBUINT16 *) (శీర్షిక + 0) = 1;			/* format */
    * (HBUINT32 *) (శీర్షిక + 2) = regions_స్థానభ్రంశం;
    * (HBUINT16 *) (శీర్షిక + 6) = దత్తాంశం_లెక్క;
    కొరకు (గుర్తులేని i = 0; i < దత్తాంశం_లెక్క; i++)
      * (HBUINT32 *) (శీర్షిక + 8 + 4 * i) = దత్తాంశం_offsets.arrayZ[i];

    కొరకు (స్థిరరాశి vardata_transform_t &t : transforms)
    {
      ఒకవేళ (!t.alive ()) కొనసాగు;
      గుర్తులేని k = t.region_indices.నిడివి;
      HBUINT16 *v = (HBUINT16 *) c->allocate_కొలత<అక్షరం> (6 + 2 * k);
      ఒకవేళ (unlikely (!v)) తిరిగి అబద్ధం;
      v[0] = 0;						/* itemCount */
      v[1] = 0;						/* wordSizeCount */
      v[2] = k;						/* regionIndices.len */
      కొరకు (గుర్తులేని j = 0; j < k; j++)
	v[3 + j] = t.region_indices.arrayZ[j];
    }

    hb_సదిశ_t<స్థిరరాశి hb_hashmap_t<hb_ట్యాగ్_t, Triple>*> region_ptrs;
    ఒకవేళ (unlikely (!region_ptrs.alloc_exact (new_regions.నిడివి))) తిరిగి అబద్ధం;
    కొరకు (స్థిరరాశి స్వయంచాలక &r : new_regions)
      region_ptrs.push (&r);
    స్వయంచాలక *region_list = c->ప్రారంభం_embed<OT::VarRegionList> ();
    తిరిగి region_list->serialize (c, axis_ట్యాగులు, region_ptrs);
  }

  అంతరంగ:
  hb_సదిశ_t<hb_hashmap_t<hb_ట్యాగ్_t, Triple>> orig_regions;
};

/* Usage-collection pass for partial instancing: record which VarDatas the
 * retained charstrings' and private dicts' blends reference.  Mirrors the
 * emission pass: blends consumed by dropped hint ops don't count. */
నిర్మాణం cff2_usage_param_t
{
  శూన్య init () {}

  cff2_instancing_plan_t *instancing = శూన్యసూచిక;
  తార్కిక drop_hints = అబద్ధం;
  గుర్తులేని ivs = 0;	/* dict-side current vsindex */
};

నిర్మాణం cff2_cs_opset_usage_t : cff2_cs_opset_t<cff2_cs_opset_usage_t, cff2_usage_param_t, blend_arg_t>
{
  స్థిర శూన్య flush_వాదనలు_and_op (op_సంకేతం_t op, cff2_cs_interp_env_t<blend_arg_t> &env, cff2_usage_param_t& param)
  {
    తార్కిక counts = నిజం;
    ఎంపిక (op)
    {
      స్థితి Op_సంకేతం_return:
      స్థితి Op_సంకేతం_endchar:
	counts = అబద్ధం;
	ఆపు;

      స్థితి Op_సంకేతం_hstem:
      స్థితి Op_సంకేతం_hstemhm:
      స్థితి Op_సంకేతం_vstem:
      స్థితి Op_సంకేతం_vstemhm:
      స్థితి Op_సంకేతం_hintmask:
      స్థితి Op_సంకేతం_cntrmask:
	ఒకవేళ (param.drop_hints)
	  counts = అబద్ధం;
	ఆపు;

      అప్రమేయ:
	ఆపు;
    }

    ఒకవేళ (counts)
      కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < env.arg_దొంతర.get_లెక్క (); i++)
	ఒకవేళ (env.arg_దొంతర[i].blending ())
	{
	  param.instancing->note_use (env.get_ivs ());
	  ఆపు;
	}

    env.clear_వాదనలు ();
  }
};

నిర్మాణం cff2_వ్యక్తిగత_dict_usage_opset_t : dict_opset_t
{
  స్థిర శూన్య process_op (op_సంకేతం_t op, cff2_priv_dict_interp_env_t& env, cff2_usage_param_t& param)
  {
    ఎంపిక (op) {
      స్థితి Op_సంకేతం_vsindexdict:
	env.process_vsindex ();
	param.ivs = env.get_ivs ();
	env.clear_వాదనలు ();
	తిరిగి;
      స్థితి Op_సంకేతం_blenddict:
	param.instancing->note_use (param.ivs);
	env.clear_వాదనలు ();
	తిరిగి;
      అప్రమేయ:
	dict_opset_t::process_op (op, env);
	ఒకవేళ (!env.arg_దొంతర.is_empty ()) తిరిగి;
	env.clear_వాదనలు ();
	తిరిగి;
    }
  }
};

} /* namespace CFF */

నిర్మాణం cff2_sub_పట్టిక_info_t : cff_sub_పట్టిక_info_t
{
  cff2_sub_పట్టిక_info_t ()
    : cff_sub_పట్టిక_info_t (),
      var_store_link (0)
  {}

  objidx_t  var_store_link;
};

నిర్మాణం cff2_శిఖరం_dict_op_serializer_t : cff_శిఖరం_dict_op_serializer_t<>
{
  తార్కిక serialize (hb_serialize_context_t *c,
		  స్థిరరాశి op_str_t &opstr,
		  స్థిరరాశి cff2_sub_పట్టిక_info_t &info) స్థిరరాశి
  {
    TRACE_SERIALIZE (ఇది);

    ఎంపిక (opstr.op)
    {
      స్థితి Op_సంకేతం_vstore:
        ఒకవేళ (info.var_store_link)
	  return_trace (FontDict::serialize_link4_op(c, opstr.op, info.var_store_link));
	లేకపోతే
	  return_trace (నిజం);

      అప్రమేయ:
	return_trace (cff_శిఖరం_dict_op_serializer_t<>::serialize (c, opstr, info));
    }
  }
};

నిర్మాణం cff2_cs_opset_flatten_t : cff2_cs_opset_t<cff2_cs_opset_flatten_t, flatten_param_t, blend_arg_t>
{
  స్థిర శూన్య flush_వాదనలు_and_op (op_సంకేతం_t op, cff2_cs_interp_env_t<blend_arg_t> &env, flatten_param_t& param)
  {
    /* Optionally capture command for specialization (before flushing, to preserve args) */
    ఒకవేళ (param.commands)
    {
      తార్కిక skip_command = అబద్ధం;

      ఎంపిక (op)
      {
	స్థితి Op_సంకేతం_return:
	స్థితి Op_సంకేతం_endchar:
	  skip_command = నిజం;
	  ఆపు;

	స్థితి Op_సంకేతం_hstem:
	స్థితి Op_సంకేతం_hstemhm:
	స్థితి Op_సంకేతం_vstem:
	స్థితి Op_సంకేతం_vstemhm:
	స్థితి Op_సంకేతం_hintmask:
	స్థితి Op_సంకేతం_cntrmask:
	  ఒకవేళ (param.drop_hints)
	    skip_command = నిజం;
	  ఆపు;

	అప్రమేయ:
	  ఆపు;
      }

      ఒకవేళ (!skip_command)
      {
	cs_command_t cmd (op);
	/* Capture resolved blend values */
	కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < env.arg_దొంతర.get_లెక్క ();)
	{
	  స్థిరరాశి blend_arg_t &arg = env.arg_దొంతర[i];
	  ఒకవేళ (arg.blending ())
	  {
	    /* For blend args, capture only the resolved default value */
	    cmd.వాదనలు.push (arg);
	    /* Skip over the multiple blend values */
	    i += arg.అంకె_విలువలు_2;
	  }
	  లేకపోతే
	  {
	    cmd.వాదనలు.push (arg);
	    i++;
	  }
	}
	param.commands->push (cmd);
      }
    }

    ఎంపిక (op)
    {
      స్థితి Op_సంకేతం_return:
      స్థితి Op_సంకేతం_endchar:
	/* dummy opcodes in CFF2. ignore */
	ఒకవేళ (op == Op_సంకేతం_endchar && param.instancer)
	  emit_vsindex_ఉపసర్గ (env, param);
	ఆపు;

      స్థితి Op_సంకేతం_hstem:
      స్థితి Op_సంకేతం_hstemhm:
      స్థితి Op_సంకేతం_vstem:
      స్థితి Op_సంకేతం_vstemhm:
      స్థితి Op_సంకేతం_hintmask:
      స్థితి Op_సంకేతం_cntrmask:
	ఒకవేళ (param.drop_hints)
	{
	  env.clear_వాదనలు ();
	  తిరిగి;
	}
	HB_FALLTHROUGH;

      అప్రమేయ:
	SUPER::flush_వాదనలు_and_op (op, env, param);
	ఆపు;
    }
  }

  స్థిర శూన్య flush_వాదనలు (cff2_cs_interp_env_t<blend_arg_t> &env, flatten_param_t& param)
  {
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < env.arg_దొంతర.get_లెక్క ();)
    {
      స్థిరరాశి blend_arg_t &arg = env.arg_దొంతర[i];
      ఒకవేళ (arg.blending ())
      {
	ఒకవేళ (unlikely (!((arg.అంకె_విలువలు_2 > 0) && (env.arg_దొంతర.get_లెక్క () >= arg.అంకె_విలువలు_2))))
	{
	  env.సమితి_error ();
	  తిరిగి;
	}
	ఒకవేళ (param.instancer)
	  rewrite_blends (arg, i, env, param);
	లేకపోతే
	  flatten_blends (arg, i, env, param);
	i += arg.అంకె_విలువలు_2;
      }
      లేకపోతే
      {
	str_encoder_t  encoder (param.flatStr);
	encoder.encode_అంకె_cs (arg);
	i++;
      }
    }
    SUPER::flush_వాదనలు (env, param);
  }

  స్థిర శూన్య flatten_blends (స్థిరరాశి blend_arg_t &arg, గుర్తులేని పూర్ణాంకం i, cff2_cs_interp_env_t<blend_arg_t> &env, flatten_param_t& param)
  {
    /* flatten the default values */
    str_encoder_t  encoder (param.flatStr);
    కొరకు (గుర్తులేని పూర్ణాంకం j = 0; j < arg.అంకె_విలువలు_2; j++)
    {
      స్థిరరాశి blend_arg_t &arg1 = env.arg_దొంతర[i + j];
      ఒకవేళ (unlikely (!((arg1.blending () && (arg.అంకె_విలువలు_2 == arg1.అంకె_విలువలు_2) && (arg1.విలువ_సూచీ_2 == j) &&
	      (arg1.deltas.నిడివి == env.get_region_లెక్క ())))))
      {
	env.సమితి_error ();
	తిరిగి;
      }
      encoder.encode_అంకె_cs (arg1);
    }
    /* flatten deltas for each value */
    కొరకు (గుర్తులేని పూర్ణాంకం j = 0; j < arg.అంకె_విలువలు_2; j++)
    {
      స్థిరరాశి blend_arg_t &arg1 = env.arg_దొంతర[i + j];
      కొరకు (గుర్తులేని పూర్ణాంకం k = 0; k < arg1.deltas.నిడివి; k++)
	encoder.encode_అంకె_cs (arg1.deltas[k]);
    }
    /* flatten the number of values followed by blend operator */
    encoder.encode_int (arg.అంకె_విలువలు_2);
    encoder.encode_op (Op_సంకేతం_blendcs);
  }

  /* Partial instancing: rewrite one blend group against the instanced store.
   * Each value's default absorbs the folded (pinned-away) contribution, and
   * its deltas are mapped onto the new region list. */
  స్థిర శూన్య rewrite_blends (స్థిరరాశి blend_arg_t &arg, గుర్తులేని పూర్ణాంకం i, cff2_cs_interp_env_t<blend_arg_t> &env, flatten_param_t& param)
  {
    స్థిరరాశి cff2_instancing_plan_t &instancer = *param.instancer;
    గుర్తులేని ivs = env.get_ivs ();
    స్థిరరాశి cff2_instancing_plan_t::vardata_transform_t *t = instancer.get_transform (ivs);
    ఒకవేళ (unlikely (!t))
    {
      env.సమితి_error ();
      తిరిగి;
    }

    గుర్తులేని n = arg.అంకె_విలువలు_2;
    కొరకు (గుర్తులేని పూర్ణాంకం j = 0; j < n; j++)
    {
      స్థిరరాశి blend_arg_t &arg1 = env.arg_దొంతర[i + j];
      ఒకవేళ (unlikely (!(arg1.blending () && (arg1.అంకె_విలువలు_2 == n) && (arg1.విలువ_సూచీ_2 == j) &&
		      (arg1.deltas.నిడివి == env.get_region_లెక్క ()) &&
		      (arg1.deltas.నిడివి == t->gains.నిడివి))))
      {
	env.సమితి_error ();
	తిరిగి;
      }
    }

    str_encoder_t encoder (param.flatStr);
    గుర్తులేని k_new = t->alive () ? t->మాతృక.నిడివి : 0;

    ఒకవేళ (!k_new)
    {
      /* All of this VarData's regions died; blends dissolve into statics. */
      కొరకు (గుర్తులేని పూర్ణాంకం j = 0; j < n; j++)
      {
	స్థిరరాశి blend_arg_t &arg1 = env.arg_దొంతర[i + j];
	సంఖ్య_t v;
	v.సమితి_real (arg1.గమ్యం_real () + round (instancer.fold_deltas (ivs, arg1.deltas.as_array ())));
	encoder.encode_అంకె_cs (v);
      }
      తిరిగి;
    }

    /* Emit in chunks so the output charstring stays within the argument
     * stack limit: values already emitted for this op, plus this chunk's
     * defaults, deltas and count, must not exceed 513. */
    గుర్తులేని పూర్తి = 0;
    అయితే (పూర్తి < n)
    {
      గుర్తులేని avail = 512 > (i + పూర్తి) ? 512 - (i + పూర్తి) : 0;
      గుర్తులేని chunk = hb_min (avail / (k_new + 1), n - పూర్తి);
      ఒకవేళ (unlikely (!chunk))
      {
	env.సమితి_error ();
	తిరిగి;
      }

      కొరకు (గుర్తులేని పూర్ణాంకం j = పూర్తి; j < పూర్తి + chunk; j++)
      {
	స్థిరరాశి blend_arg_t &arg1 = env.arg_దొంతర[i + j];
	సంఖ్య_t v;
	v.సమితి_real (arg1.గమ్యం_real () + round (instancer.fold_deltas (ivs, arg1.deltas.as_array ())));
	encoder.encode_అంకె_cs (v);
      }
      కొరకు (గుర్తులేని పూర్ణాంకం j = పూర్తి; j < పూర్తి + chunk; j++)
      {
	స్థిరరాశి blend_arg_t &arg1 = env.arg_దొంతర[i + j];
	కొరకు (గుర్తులేని r = 0; r < k_new; r++)
	{
	  సంఖ్య_t v;
	  v.సమితి_int (hb_clamp_గమ్యం<పూర్ణాంకం> (round (t->transform_delta (r, arg1.deltas.as_array ()))));
	  encoder.encode_అంకె_cs (v);
	}
      }
      encoder.encode_int (chunk);
      encoder.encode_op (Op_సంకేతం_blendcs);
      పూర్తి += chunk;
    }
    param.emitted_blend = నిజం;
  }

  /* Partial instancing: prepend a vsindex op when the charstring's
   * (remapped) ivs differs from its FD's (remapped) private-dict ivs. */
  స్థిర శూన్య emit_vsindex_ఉపసర్గ (cff2_cs_interp_env_t<blend_arg_t> &env, flatten_param_t& param)
  {
    ఒకవేళ (!param.emitted_blend) తిరిగి;
    స్థిరరాశి cff2_instancing_plan_t &instancer = *param.instancer;
    గుర్తులేని new_ivs = instancer.remap_ivs (env.get_ivs ());
    గుర్తులేని fd_ivs = instancer.remap_ivs (env.get_orig_ivs ());
    ఒకవేళ (new_ivs == fd_ivs) తిరిగి;

    str_buff_t ఉపసర్గ;
    str_encoder_t enc (ఉపసర్గ);
    enc.encode_int ((పూర్ణాంకం) new_ivs);
    enc.encode_op (Op_సంకేతం_vsindexcs);

    str_buff_t &buf = param.flatStr;
    గుర్తులేని old_len = buf.నిడివి;
    ఒకవేళ (unlikely (ఉపసర్గ.లోపల_error () || !buf.resize (old_len + ఉపసర్గ.నిడివి)))
    {
      env.సమితి_error ();
      తిరిగి;
    }
    memmove (buf.arrayZ + ఉపసర్గ.నిడివి, buf.arrayZ, old_len);
    hb_memcpy (buf.arrayZ, ఉపసర్గ.arrayZ, ఉపసర్గ.నిడివి);
  }

  స్థిర శూన్య flush_op (op_సంకేతం_t op, cff2_cs_interp_env_t<blend_arg_t> &env, flatten_param_t& param)
  {
    ఎంపిక (op)
    {
      స్థితి Op_సంకేతం_return:
      స్థితి Op_సంకేతం_endchar:
	తిరిగి;
      అప్రమేయ:
	str_encoder_t  encoder (param.flatStr);
	encoder.encode_op (op);
    }
  }

  స్థిర శూన్య flush_hintmask (op_సంకేతం_t op, cff2_cs_interp_env_t<blend_arg_t> &env, flatten_param_t& param)
  {
    SUPER::flush_hintmask (op, env, param);
    /* Preserve hintmask payload in captured commands for specializer re-encoding. */
    ఒకవేళ (param.commands && !param.drop_hints && param.commands->నిడివి > 0)
    {
      స్వయంచాలక &cmd = param.commands->తోక ();
      ఒకవేళ (cmd.op == op)
      {
        cmd.ముసుగు_బైట్లు.resize (env.hintmask_కొలత);
        ఒకవేళ (unlikely (cmd.ముసుగు_బైట్లు.లోపల_error ()))
        {
          env.సమితి_error ();
          తిరిగి;
        }
        కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < env.hintmask_కొలత; i++)
          cmd.ముసుగు_బైట్లు[i] = env.str_ref[i];
      }
    }
    ఒకవేళ (!param.drop_hints)
    {
      str_encoder_t  encoder (param.flatStr);
      కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < env.hintmask_కొలత; i++)
	encoder.encode_byte (env.str_ref[i]);
    }
  }

  అంతరంగ:
  రకనిర్వచనం cff2_cs_opset_t<cff2_cs_opset_flatten_t, flatten_param_t, blend_arg_t> SUPER;
  రకనిర్వచనం cs_opset_t<blend_arg_t, cff2_cs_opset_flatten_t, cff2_cs_opset_flatten_t, cff2_cs_interp_env_t<blend_arg_t>, flatten_param_t> CSOPSET;
};

నిర్మాణం cff2_cs_opset_subr_subset_t : cff2_cs_opset_t<cff2_cs_opset_subr_subset_t, subr_subset_param_t, blend_arg_t>
{
  స్థిర శూన్య process_op (op_సంకేతం_t op, cff2_cs_interp_env_t<blend_arg_t> &env, subr_subset_param_t& param)
  {
    ఎంపిక (op) {

      స్థితి Op_సంకేతం_return:
	param.current_parsed_str->flush_coalesced (env.str_ref);
	param.current_parsed_str->సమితి_parsed ();
	env.return_from_subr ();
	param.సమితి_current_str (env, అబద్ధం);
	ఆపు;

      స్థితి Op_సంకేతం_endchar:
	param.current_parsed_str->flush_coalesced (env.str_ref);
	param.current_parsed_str->సమితి_parsed ();
	SUPER::process_op (op, env, param);
	ఆపు;

      స్థితి Op_సంకేతం_callsubr:
	process_call_subr (op, CSType_LocalSubr, env, param, env.localSubrs, param.local_సంవృతం_2);
	ఆపు;

      స్థితి Op_సంకేతం_callgsubr:
	process_call_subr (op, CSType_GlobalSubr, env, param, env.globalSubrs, param.global_సంవృతం);
	ఆపు;

      అప్రమేయ:
	SUPER::process_op (op, env, param);
	param.current_parsed_str->కూడు_op (op, env.str_ref);
	ఆపు;
    }
  }

  రక్షిత:
  స్థిర శూన్య process_call_subr (op_సంకేతం_t op, cs_type_t type,
				 cff2_cs_interp_env_t<blend_arg_t> &env, subr_subset_param_t& param,
				 cff2_biased_subrs_t& subrs, hb_సమితి_t *సంవృతం)
  {
    byte_str_ref_t    str_ref = env.str_ref;
    env.call_subr (subrs, type);
    param.current_parsed_str->కూడు_call_op (op, str_ref, env.context.subr_అంకె_2);
    సంవృతం->కూడు (env.context.subr_అంకె_2);
    param.సమితి_current_str (env, నిజం);
  }

  అంతరంగ:
  రకనిర్వచనం cff2_cs_opset_t<cff2_cs_opset_subr_subset_t, subr_subset_param_t, blend_arg_t> SUPER;
};

నిర్మాణం cff2_subr_subsetter_t : subr_subsetter_t<cff2_subr_subsetter_t, CFF2Subrs, స్థిరరాశి OT::cff2::accelerator_subset_t, cff2_cs_interp_env_t<blend_arg_t>, cff2_cs_opset_subr_subset_t>
{
  cff2_subr_subsetter_t (స్థిరరాశి OT::cff2::accelerator_subset_t &acc_, స్థిరరాశి hb_subset_plan_t *plan_)
    : subr_subsetter_t (acc_, plan_) {}

  స్థిర శూన్య complete_parsed_str (cff2_cs_interp_env_t<blend_arg_t> &env, subr_subset_param_t& param, parsed_cs_str_t &charstring)
  {
    /* vsindex is inserted at the beginning of the charstring as necessary */
    ఒకవేళ (env.చూసిన_vsindex ())
    {
      సంఖ్య_t  ivs;
      ivs.సమితి_int ((పూర్ణాంకం)env.get_ivs ());
      charstring.సమితి_ఉపసర్గ (ivs, Op_సంకేతం_vsindexcs);
    }
  }
};

నిర్మాణం cff2_వ్యక్తిగత_blend_encoder_param_t
{
  cff2_వ్యక్తిగత_blend_encoder_param_t (hb_serialize_context_t *c,
				      స్థిరరాశి CFF2_అంశం_Variation_Store *varStore,
				      hb_array_t<పూర్ణాంకం> normalized_coords) :
    c (c), varStore (varStore), normalized_coords (normalized_coords) {}

  శూన్య init () {}

  శూన్య process_blend ()
  {
    ఒకవేళ (!చూసిన_blend)
    {
      region_లెక్క_2 = varStore->varStore.get_region_సూచీ_లెక్క (ivs);
      ఒకవేళ (unlikely (!scalars.resize_exact (region_లెక్క_2)))
      {
	ఒకవేళ (c) c->err (HB_SERIALIZE_ERROR_OTHER);
	చూసిన_blend = నిజం;
	తిరిగి;
      }
      varStore->varStore.get_region_scalars (ivs, normalized_coords.arrayZ, normalized_coords.నిడివి,
					     &scalars[0], region_లెక్క_2);
      చూసిన_blend = నిజం;
    }
  }

  ద్విగుణ blend_deltas (hb_array_t<స్థిరరాశి సంఖ్య_t> deltas) స్థిరరాశి
  {
    ద్విగుణ v = 0;
    ఒకవేళ (likely (scalars.నిడివి == deltas.నిడివి))
    {
      గుర్తులేని లెక్క = scalars.నిడివి;
      కొరకు (గుర్తులేని i = 0; i < లెక్క; i++)
	v += (ద్విగుణ) scalars.arrayZ[i] * deltas.arrayZ[i].గమ్యం_real ();
    }
    తిరిగి v;
  }


  hb_serialize_context_t *c = శూన్యసూచిక;
  తార్కిక చూసిన_blend = అబద్ధం;
  గుర్తులేని ivs = 0;
  గుర్తులేని region_లెక్క_2 = 0;
  hb_సదిశ_t<దశాంశం> scalars;
  స్థిరరాశి	 CFF2_అంశం_Variation_Store *varStore = శూన్యసూచిక;
  hb_array_t<పూర్ణాంకం> normalized_coords;

  /* CFF2 partial instancing: when set, blends are rewritten against the
   * instanced variation store instead of being flattened. */
  స్థిరరాశి cff2_instancing_plan_t *instancer = శూన్యసూచిక;
  /* Error-compensated fold rounding state, per dict operator. */
  ద్విగుణ fold_exact = 0.;
  ద్విగుణ fold_emitted = 0.;
};

నిర్మాణం cff2_వ్యక్తిగత_dict_blend_opset_t : dict_opset_t
{
  స్థిర శూన్య process_arg_blend (cff2_వ్యక్తిగత_blend_encoder_param_t& param,
				 సంఖ్య_t &arg,
				 స్థిరరాశి hb_array_t<స్థిరరాశి సంఖ్య_t> blends,
				 గుర్తులేని n, గుర్తులేని i)
  {
    arg.సమితి_int (round (arg.గమ్యం_real () + param.blend_deltas (blends)));
  }

  /* The fold added to a value, error-compensated across the operator: most
   * blended private-dict values (BlueValues, StemSnap*, ...) are
   * delta-encoded, so independently rounded folds would accumulate in the
   * decoded absolutes; keeping the emitted cumulative fold equal to the
   * rounded exact cumulative fold bounds every absolute within ±0.5. */
  స్థిర ద్విగుణ fold (cff2_వ్యక్తిగత_blend_encoder_param_t& param,
		      స్థిరరాశి hb_array_t<స్థిరరాశి సంఖ్య_t> blends)
  {
    ద్విగుణ f = param.blend_deltas (blends);
    ద్విగుణ emit = round (param.fold_exact + f) - param.fold_emitted;
    param.fold_exact += f;
    param.fold_emitted += emit;
    తిరిగి emit;
  }

  /* Partial instancing: emit the args preceding this blend group, then the
   * group itself rewritten against the instanced store, and clear the
   * stack.  The op's remaining args are flushed at process_op time. */
  స్థిర శూన్య rewrite_blends (cff2_priv_dict_interp_env_t& env,
			      cff2_వ్యక్తిగత_blend_encoder_param_t& param,
			      గుర్తులేని ప్రారంభం, గుర్తులేని n, గుర్తులేని k)
  {
    స్థిరరాశి cff2_instancing_plan_t &instancer = *param.instancer;
    స్థిరరాశి cff2_instancing_plan_t::vardata_transform_t *t = instancer.get_transform (param.ivs);
    ఒకవేళ (unlikely (!t || t->gains.నిడివి != k))
    {
      env.సమితి_error ();
      తిరిగి;
    }

    str_buff_t str;
    str_encoder_t encoder (str);

    కొరకు (గుర్తులేని idx = 0; idx < ప్రారంభం; idx++)
      encoder.encode_అంకె_tp (env.arg_దొంతర[idx]);

    గుర్తులేని k_new = t->alive () ? t->మాతృక.నిడివి : 0;
    ఒకవేళ (!k_new)
    {
      /* This VarData died (or is unused by any retained charstring, which
       * can happen for FDs kept only for retain-gids holes); blends
       * dissolve into statics. */
      కొరకు (గుర్తులేని j = 0; j < n; j++)
      {
	స్థిరరాశి hb_array_t<స్థిరరాశి సంఖ్య_t> blends = env.arg_దొంతర.sub_array (ప్రారంభం + n + (j * k), k);
	సంఖ్య_t v;
	v.సమితి_real (env.arg_దొంతర[ప్రారంభం + j].గమ్యం_real () + fold (param, blends));
	encoder.encode_అంకె_tp (v);
      }
    }
    లేకపోతే
    {
      గుర్తులేని పూర్తి = 0;
      అయితే (పూర్తి < n)
      {
	గుర్తులేని avail = 512 > (ప్రారంభం + పూర్తి) ? 512 - (ప్రారంభం + పూర్తి) : 0;
	గుర్తులేని chunk = hb_min (avail / (k_new + 1), n - పూర్తి);
	ఒకవేళ (unlikely (!chunk))
	{
	  env.సమితి_error ();
	  తిరిగి;
	}

	కొరకు (గుర్తులేని j = పూర్తి; j < పూర్తి + chunk; j++)
	{
	  స్థిరరాశి hb_array_t<స్థిరరాశి సంఖ్య_t> blends = env.arg_దొంతర.sub_array (ప్రారంభం + n + (j * k), k);
	  సంఖ్య_t v;
	  v.సమితి_real (env.arg_దొంతర[ప్రారంభం + j].గమ్యం_real () + fold (param, blends));
	  encoder.encode_అంకె_tp (v);
	}
	కొరకు (గుర్తులేని j = పూర్తి; j < పూర్తి + chunk; j++)
	{
	  స్థిరరాశి hb_array_t<స్థిరరాశి సంఖ్య_t> blends = env.arg_దొంతర.sub_array (ప్రారంభం + n + (j * k), k);
	  కొరకు (గుర్తులేని r = 0; r < k_new; r++)
	  {
	    సంఖ్య_t v;
	    v.సమితి_int (hb_clamp_గమ్యం<పూర్ణాంకం> (round (t->transform_delta (r, blends))));
	    encoder.encode_అంకె_tp (v);
	  }
	}
	encoder.encode_int (chunk);
	encoder.encode_op (Op_సంకేతం_blenddict);
	పూర్తి += chunk;
      }
    }

    స్వయంచాలక బైట్లు = str.as_బైట్లు ();
    param.c->embed (&బైట్లు, బైట్లు.నిడివి);

    env.clear_వాదనలు ();
  }

  స్థిర శూన్య process_blend (cff2_priv_dict_interp_env_t& env, cff2_వ్యక్తిగత_blend_encoder_param_t& param)
  {
    గుర్తులేని పూర్ణాంకం n, k;

    param.process_blend ();
    k = param.region_లెక్క_2;
    n = env.arg_దొంతర.తీయి_uint ();

    గుర్తులేని లెక్క = env.arg_దొంతర.get_లెక్క ();
    గుర్తులేని పూర్ణాంకం మొత్తం;
    ఒకవేళ (unlikely (hb_unsigned_mul_overflows (k + 1, n, &మొత్తం) || మొత్తం > లెక్క))
    {
      env.సమితి_error ();
      తిరిగి;
    }

    /* copy the blend values into blend array of the default values */
    గుర్తులేని పూర్ణాంకం ప్రారంభం = లెక్క - మొత్తం;
    /* let an obvious error case fail, but note CFF2 spec doesn't forbid n==0 */
    ఒకవేళ (unlikely (ప్రారంభం > లెక్క))
    {
      env.సమితి_error ();
      తిరిగి;
    }

    ఒకవేళ (param.instancer && k)
    {
      rewrite_blends (env, param, ప్రారంభం, n, k);
      తిరిగి;
    }

    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < n; i++)
    {
      స్థిరరాశి hb_array_t<స్థిరరాశి సంఖ్య_t> blends = env.arg_దొంతర.sub_array (ప్రారంభం + n + (i * k), k);
      process_arg_blend (param, env.arg_దొంతర[ప్రారంభం + i], blends, n, i);
    }

    /* pop off blend values leaving default values now adorned with blend values */
    env.arg_దొంతర.తీయి (k * n);
  }

  స్థిర శూన్య process_op (op_సంకేతం_t op, cff2_priv_dict_interp_env_t& env, cff2_వ్యక్తిగత_blend_encoder_param_t& param)
  {
    ఎంపిక (op) {
      స్థితి Op_సంకేతం_Std_HW:
      స్థితి Op_సంకేతం_Std_VW:
      స్థితి Op_సంకేతం_Blue_Scale:
      స్థితి Op_సంకేతం_Blue_జరుపు:
      స్థితి Op_సంకేతం_Blue_Fuzz:
      స్థితి Op_సంకేతం_Expansion_గుణకం:
      స్థితి Op_సంకేతం_Language_సమూహం:
      స్థితి Op_సంకేతం_Blue_విలువలు:
      స్థితి Op_సంకేతం_Other_Blues:
      స్థితి Op_సంకేతం_Family_Blues:
      స్థితి Op_సంకేతం_Family_Other_Blues:
      స్థితి Op_సంకేతం_Stem_Snap_H:
      స్థితి Op_సంకేతం_Stem_Snap_V:
	ఆపు;
      స్థితి Op_సంకేతం_vsindexdict:
	env.process_vsindex ();
	param.ivs = env.get_ivs ();
	ఒకవేళ (param.instancer)
	{
	  /* Re-emit with the remapped vsindex; 0 is the dict default. */
	  గుర్తులేని new_ivs = param.instancer->remap_ivs (param.ivs);
	  ఒకవేళ (new_ivs)
	  {
	    str_buff_t str;
	    str_encoder_t encoder (str);
	    encoder.encode_int ((పూర్ణాంకం) new_ivs);
	    encoder.encode_op (Op_సంకేతం_vsindexdict);
	    స్వయంచాలక బైట్లు = str.as_బైట్లు ();
	    param.c->embed (&బైట్లు, బైట్లు.నిడివి);
	  }
	}
	env.clear_వాదనలు ();
	తిరిగి;
      స్థితి Op_సంకేతం_blenddict:
	process_blend (env, param);
	తిరిగి;

      అప్రమేయ:
	dict_opset_t::process_op (op, env);
	ఒకవేళ (!env.arg_దొంతర.is_empty ()) తిరిగి;
	ఆపు;
    }

    ఒకవేళ (unlikely (env.లోపల_error ())) తిరిగి;

    // Write args then op

    str_buff_t str;
    str_encoder_t encoder (str);

    గుర్తులేని లెక్క = env.arg_దొంతర.get_లెక్క ();
    కొరకు (గుర్తులేని i = 0; i < లెక్క; i++)
      encoder.encode_అంకె_tp (env.arg_దొంతర[i]);

    encoder.encode_op (op);

    స్వయంచాలక బైట్లు = str.as_బైట్లు ();
    param.c->embed (&బైట్లు, బైట్లు.నిడివి);

    env.clear_వాదనలు ();
    param.fold_exact = param.fold_emitted = 0.;
  }
};

నిర్మాణం cff2_వ్యక్తిగత_dict_op_serializer_t : op_serializer_t
{
  cff2_వ్యక్తిగత_dict_op_serializer_t (తార్కిక desubroutinize_, తార్కిక drop_hints_, తార్కిక pinned_,
				     స్థిరరాశి CFF::CFF2_అంశం_Variation_Store* varStore_,
				     hb_array_t<పూర్ణాంకం> normalized_coords_,
				     స్థిరరాశి cff2_instancing_plan_t *instancer_ = శూన్యసూచిక)
    : desubroutinize (desubroutinize_), drop_hints (drop_hints_), pinned (pinned_),
      param (శూన్యసూచిక, varStore_, normalized_coords_)
  { param.instancer = instancer_; }

  తార్కిక serialize (hb_serialize_context_t *c,
		  స్థిరరాశి op_str_t &opstr,
		  objidx_t subrs_link) స్థిరరాశి
  {
    TRACE_SERIALIZE (ఇది);

    ఒకవేళ (drop_hints && dict_opset_t::is_hint_op (opstr.op))
      return_trace (నిజం);

    ఒకవేళ (opstr.op == Op_సంకేతం_Subrs)
    {
      ఒకవేళ (desubroutinize || !subrs_link)
	return_trace (నిజం);
      లేకపోతే
	return_trace (FontDict::serialize_link2_op (c, opstr.op, subrs_link));
    }

    ఒకవేళ (pinned || param.instancer)
    {
      /* Reinterpret opstr and process blends.  The param persists across
       * this dict's opstrs, so that an ivs set by a vsindexdict opstr is
       * still in effect when a later opstr's blends are processed. */
      cff2_priv_dict_interp_env_t env {hb_ubytes_t (opstr.ptr, opstr.నిడివి)};
      param.c = c;
      dict_interpreter_t<cff2_వ్యక్తిగత_dict_blend_opset_t, cff2_వ్యక్తిగత_blend_encoder_param_t, cff2_priv_dict_interp_env_t> interp (env);
      return_trace (interp.interpret (param));
    }

    return_trace (copy_opstr (c, opstr));
  }

  రక్షిత:
  స్థిరరాశి తార్కిక desubroutinize;
  స్థిరరాశి తార్కిక drop_hints;
  స్థిరరాశి తార్కిక pinned;
  మార్చదగిన cff2_వ్యక్తిగత_blend_encoder_param_t param;
};



నామాకాశ OT {
నిర్మాణం cff2_subset_plan
{
  తార్కిక create (స్థిరరాశి OT::cff2::accelerator_subset_t &acc,
	      hb_subset_plan_t *plan)
  {
    /* make sure notdef is first */
    hb_codepoint_t old_glyph;
    ఒకవేళ (!plan->old_gid_for_new_gid (0, &old_glyph) || (old_glyph != 0)) తిరిగి అబద్ధం;

    అంకె_glyphs = plan->అంకె_output_glyphs ();
    orig_fdcount = acc.fdArray->లెక్క;

    drop_hints = plan->flags & HB_SUBSET_FLAGS_NO_HINTING;
    తార్కిక instancing_requested = (తార్కిక) plan->normalized_coords;
    pinned = instancing_requested && plan->all_axes_pinned;
    normalized_coords = plan->normalized_coords;
    తల_maxp_info = plan->తల_maxp_info;
    hmtx_map = &plan->hmtx_map;
    desubroutinize = plan->flags & HB_SUBSET_FLAGS_DESUBROUTINIZE ||
		     instancing_requested; // For instancing we need this path

    /* Partial instancing: some axes pinned or limited, others kept. */
    partial_instancing = instancing_requested && !pinned &&
			 acc.varStore != &Null (CFF2_అంశం_Variation_Store);
    ఒకవేళ (partial_instancing &&
	unlikely (!instancing.create (acc.varStore, plan)))
      తిరిగి అబద్ధం;
    axis_ట్యాగులు = &plan->axis_ట్యాగులు;

    /* Enable command capture for CFF2→CFF1 conversion (for specialization) */
    capture_commands = pinned;

 #నిర్వచితమైతే HB_EXPERIMENTAL_API
    min_charstrings_off_కొలత = (plan->flags & HB_SUBSET_FLAGS_IFTB_REQUIREMENTS) ? 4 : 0;
 #లేకపోతే
    min_charstrings_off_కొలత = 0;
 #అయితే_ముగింపు

    ఒకవేళ (desubroutinize)
    {
      ఒకవేళ (partial_instancing)
      {
	/* Pass 1: collect which VarDatas the retained blends reference, so
	 * unused ones get pruned and the survivors renumbered. */
	hb_సమితి_t వాడిన_fds;
	కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < అంకె_glyphs; i++)
	{
	  hb_codepoint_t glyph;
	  ఒకవేళ (!plan->old_gid_for_new_gid (i, &glyph))
	    కొనసాగు;
	  స్థిరరాశి hb_ubytes_t str = (*acc.char_అక్షరశ్రేణులు)[glyph];
	  గుర్తులేని పూర్ణాంకం fd = CFF::subset_get_fd (acc, glyph);
	  ఒకవేళ (unlikely (fd >= acc.fd_లెక్క))
	    తిరిగి అబద్ధం;
	  వాడిన_fds.కూడు (fd);

	  cff2_cs_interp_env_t<blend_arg_t> env (str, acc, fd);
	  cs_interpreter_t<cff2_cs_interp_env_t<blend_arg_t>, cff2_cs_opset_usage_t, cff2_usage_param_t> interp (env);
	  cff2_usage_param_t param;
	  param.instancing = &instancing;
	  param.drop_hints = drop_hints;
	  ఒకవేళ (unlikely (!interp.interpret (param)))
	    తిరిగి అబద్ధం;
	}
	కొరకు (గుర్తులేని fd : వాడిన_fds)
	{
	  cff2_usage_param_t param;
	  param.instancing = &instancing;
	  గుర్తులేని లెక్క = acc.వ్యక్తిగత_Dicts[fd].get_లెక్క ();
	  కొరకు (గుర్తులేని j = 0; j < లెక్క; j++)
	  {
	    స్థిరరాశి op_str_t &opstr = acc.వ్యక్తిగత_Dicts[fd][j];
	    ఒకవేళ (drop_hints && dict_opset_t::is_hint_op (opstr.op))
	      కొనసాగు;
	    cff2_priv_dict_interp_env_t env {hb_ubytes_t (opstr.ptr, opstr.నిడివి)};
	    dict_interpreter_t<cff2_వ్యక్తిగత_dict_usage_opset_t, cff2_usage_param_t, cff2_priv_dict_interp_env_t> interp (env);
	    ఒకవేళ (unlikely (!interp.interpret (param)))
	      తిరిగి అబద్ధం;
	  }
	}
	ఒకవేళ (unlikely (!instancing.finalize ()))
	  తిరిగి అబద్ధం;

	/* Pass 2: like subr_flattener_t, but blends stay symbolic (no coords
	 * fed to the env) and get rewritten against the instanced store. */
	ఒకవేళ (!subset_charstrings.resize_exact (అంకె_glyphs))
	  తిరిగి అబద్ధం;
	కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < అంకె_glyphs; i++)
	{
	  hb_codepoint_t glyph;
	  ఒకవేళ (!plan->old_gid_for_new_gid (i, &glyph))
	    కొనసాగు;
	  స్థిరరాశి hb_ubytes_t str = (*acc.char_అక్షరశ్రేణులు)[glyph];
	  గుర్తులేని పూర్ణాంకం fd = CFF::subset_get_fd (acc, glyph);
	  ఒకవేళ (unlikely (fd >= acc.fd_లెక్క))
	    తిరిగి అబద్ధం;

	  cff2_cs_interp_env_t<blend_arg_t> env (str, acc, fd);
	  cs_interpreter_t<cff2_cs_interp_env_t<blend_arg_t>, cff2_cs_opset_flatten_t, flatten_param_t> interp (env);
	  flatten_param_t param = {
	    subset_charstrings.arrayZ[i],
	    (తార్కిక) (plan->flags & HB_SUBSET_FLAGS_NO_HINTING),
	    plan
	  };
	  param.instancer = &instancing;
	  ఒకవేళ (unlikely (!interp.interpret (param)))
	    తిరిగి అబద్ధం;
	}
      }
      లేకపోతే
      {
      /* Flatten global & local subrs */
      subr_flattener_t<స్థిరరాశి OT::cff2::accelerator_subset_t, cff2_cs_interp_env_t<blend_arg_t>, cff2_cs_opset_flatten_t>
		    flattener(acc, plan);

      /* Enable command capture if requested (for specialization) */
      ఒకవేళ (capture_commands)
      {
	ఒకవేళ (!charstring_commands.resize_exact (అంకె_glyphs))
	  తిరిగి అబద్ధం;

	ఒకవేళ (!flattener.flatten (subset_charstrings, &charstring_commands))
	  తిరిగి అబద్ధం;
      }
      లేకపోతే
      {
	ఒకవేళ (!flattener.flatten (subset_charstrings))
	  తిరిగి అబద్ధం;
      }
      }
    }
    లేకపోతే
    {
      cff2_subr_subsetter_t	subr_subsetter (acc, plan);

      /* Subset subrs: collect used subroutines, leaving all unused ones behind */
      ఒకవేళ (!subr_subsetter.subset ())
	తిరిగి అబద్ధం;

      /* encode charstrings, global subrs, local subrs with new subroutine numbers */
      ఒకవేళ (!subr_subsetter.encode_charstrings (subset_charstrings, !pinned))
	తిరిగి అబద్ధం;

      ఒకవేళ (!subr_subsetter.encode_globalsubrs (subset_globalsubrs))
	తిరిగి అబద్ధం;

      /* local subrs */
      ఒకవేళ (!subset_localsubrs.resize (orig_fdcount))
	తిరిగి అబద్ధం;
      కొరకు (గుర్తులేని పూర్ణాంకం fd = 0; fd < orig_fdcount; fd++)
      {
	subset_localsubrs[fd].init ();
	ఒకవేళ (!subr_subsetter.encode_localsubrs (fd, subset_localsubrs[fd]))
	  తిరిగి అబద్ధం;
      }
    }

    /* FDSelect */
    ఒకవేళ (acc.fdSelect != &Null (CFF2FDSelect))
    {
      ఒకవేళ (unlikely (!hb_plan_subset_cff_fdselect (plan,
						  orig_fdcount,
						  *(స్థిరరాశి FDSelect *)acc.fdSelect,
						  acc.అంకె_glyphs,
						  subset_fdcount,
						  subset_fdselect_కొలత,
						  subset_fdselect_format,
						  subset_fdselect_ranges,
						  fdmap)))
	తిరిగి అబద్ధం;
    }
    లేకపోతే
      fdmap.తటస్థం (1);

    తిరిగి నిజం;
  }

  cff2_sub_పట్టిక_info_t info;

  గుర్తులేని పూర్ణాంకం    అంకె_glyphs;
  గుర్తులేని పూర్ణాంకం    orig_fdcount = 0;
  గుర్తులేని పూర్ణాంకం    subset_fdcount = 1;
  గుర్తులేని పూర్ణాంకం    subset_fdselect_కొలత = 0;
  గుర్తులేని పూర్ణాంకం    subset_fdselect_format = 0;
  తార్కిక            pinned = అబద్ధం;
  hb_సదిశ_t<సంకేతం_pair_t>   subset_fdselect_ranges;

  hb_inc_bimap_t   fdmap;

  str_buff_vec_t	    subset_charstrings;
  str_buff_vec_t	    subset_globalsubrs;
  hb_సదిశ_t<str_buff_vec_t> subset_localsubrs;

  తార్కిక	    drop_hints = అబద్ధం;
  తార్కిక	    desubroutinize = అబద్ధం;

  తార్కిక	    partial_instancing = అబద్ధం;
  cff2_instancing_plan_t instancing;
  స్థిరరాశి hb_సదిశ_t<hb_ట్యాగ్_t> *axis_ట్యాగులు = శూన్యసూచిక;

  గుర్తులేని  min_charstrings_off_కొలత = 0;

  hb_array_t<పూర్ణాంకం> normalized_coords; // For instantiation
  తల_maxp_info_t తల_maxp_info;  // For FontBBox
  స్థిరరాశి hb_hashmap_t<hb_codepoint_t, hb_pair_t<గుర్తులేని, పూర్ణాంకం>> *hmtx_map; // For widths

  // Width optimization results (for CFF1 conversion)
  గుర్తులేని default_వెడల్పు = 0;
  గుర్తులేని nominal_వెడల్పు = 0;

  // Command capture for specialization (CFF2→CFF1 conversion)
  తార్కిక capture_commands = అబద్ధం;
  hb_సదిశ_t<hb_సదిశ_t<cs_command_t>> charstring_commands;
};
} // namespace OT

/*
 * CFF2 to CFF1 Converter Implementation
 */

#అన్తర్భావ "hb-cff-width-optimizer.hh"
#అన్తర్భావ "hb-cff-specializer.hh"

/* Serialize charstrings using CFF1 format with widths */
స్థిర తార్కిక
_serialize_cff1_charstrings (hb_serialize_context_t *c,
                             OT::cff2_subset_plan &plan,
                             గుర్తులేని default_వెడల్పు,
                             గుర్తులేని nominal_వెడల్పు)
{
  c->push ();

  // CFF1 requires:
  // 1. Width at the beginning (if != defaultWidthX)
  // 2. endchar at the end
  str_buff_vec_t cff1_charstrings;
  ఒకవేళ (unlikely (!cff1_charstrings.resize (plan.subset_charstrings.నిడివి)))
  {
    c->తీయి_discard ();
    తిరిగి అబద్ధం;
  }

  కొరకు (గుర్తులేని i = 0; i < plan.subset_charstrings.నిడివి; i++)
  {
    // Get width for this glyph from hmtx_map
    గుర్తులేని వెడల్పు = 0;
    ఒకవేళ (plan.hmtx_map->has (i))
      వెడల్పు = plan.hmtx_map->get (i).మొదటి;

    // Encode width if different from default
    str_encoder_t encoder (cff1_charstrings[i]);
    ఒకవేళ (వెడల్పు != default_వెడల్పు)
    {
      పూర్ణాంకం delta = (పూర్ణాంకం) వెడల్పు - (పూర్ణాంకం) nominal_వెడల్పు;
      encoder.encode_int (delta);
    }

    // Use specialized commands if available, otherwise use binary
    ఒకవేళ (plan.capture_commands && i < plan.charstring_commands.నిడివి &&
        plan.charstring_commands[i].నిడివి > 0)
    {
      // Specialize and encode commands
      స్వయంచాలక &commands = plan.charstring_commands[i];
      CFF::specialize_commands (commands, 48);  /* maxstack=48 for CFF1 */
      ఒకవేళ (unlikely (!CFF::encode_commands (commands, cff1_charstrings[i])))
      {
        c->తీయి_discard ();
        తిరిగి అబద్ధం;
      }
    }
    లేకపోతే
    {
      // Use binary CharString
      స్థిరరాశి str_buff_t &cs = plan.subset_charstrings[i];
      కొరకు (గుర్తులేని j = 0; j < cs.నిడివి; j++)
        cff1_charstrings[i].push (cs[j]);
    }

    // Check if it already ends with endchar (0x0e) or return (0x0b)
    ఒకవేళ (cff1_charstrings[i].నిడివి == 0 ||
        (cff1_charstrings[i].తోక () != 0x0e && cff1_charstrings[i].తోక () != 0x0b))
    {
      // Append endchar operator
      ఒకవేళ (unlikely (!cff1_charstrings[i].నెట్టు_or_fail (0x0e)))
      {
        c->తీయి_discard ();
        తిరిగి అబద్ధం;
      }
    }
  }

  గుర్తులేని దత్తాంశం_కొలత = 0;
  గుర్తులేని మొత్తం_కొలత = CFF1_Char_అక్షరశ్రేణులు::మొత్తం_కొలత (cff1_charstrings, &దత్తాంశం_కొలత, plan.min_charstrings_off_కొలత);
  ఒకవేళ (unlikely (!c->ప్రారంభం_zerocopy (మొత్తం_కొలత)))
  {
    c->తీయి_discard ();
    తిరిగి అబద్ధం;
  }

  స్వయంచాలక *cs = c->ప్రారంభం_embed<CFF1_Char_అక్షరశ్రేణులు> ();
  ఒకవేళ (unlikely (!cs->serialize (c, cff1_charstrings, &దత్తాంశం_కొలత, plan.min_charstrings_off_కొలత)))
  {
    c->తీయి_discard ();
    తిరిగి అబద్ధం;
  }

  plan.info.char_అక్షరశ్రేణులు_link = c->తీయి_pack (అబద్ధం);
  తిరిగి నిజం;
}

/* Serialize CID Charset (format 2 range: gid 0-N -> cid 0-N) */
స్థిర తార్కిక
_serialize_cff1_charset (hb_serialize_context_t *c,
                         గుర్తులేని పూర్ణాంకం అంకె_glyphs,
                         objidx_t &charset_link)
{
  // For CID fonts, create a simple identity charset
  // Format 2: one range covering all glyphs (except .notdef)
  c->push ();

  స్వయంచాలక *charset = c->ప్రారంభం_embed<Charset> ();
  ఒకవేళ (unlikely (!charset))
  {
    c->తీయి_discard ();
    తిరిగి అబద్ధం;
  }

  // Create a single range for CID 1 to num_glyphs-1
  hb_సదిశ_t<సంకేతం_pair_t> ranges;
  ఒకవేళ (అంకె_glyphs > 1)
  {
    సంకేతం_pair_t range;
    range.సంకేతం_2 = 1;  // first CID
    range.glyph = అంకె_glyphs - 2;  // nLeft (covers glyphs 1 to num_glyphs-1)
    ranges.push (range);
  }

  ఒకవేళ (unlikely (!charset->serialize (c, 2, అంకె_glyphs, ranges)))
  {
    c->తీయి_discard ();
    తిరిగి అబద్ధం;
  }

  charset_link = c->తీయి_pack ();
  తిరిగి నిజం;
}

/* CFF2 to CFF1 serialization */
నామాకాశ CFF {

తార్కిక
serialize_cff2_గమ్యం_cff1 (hb_serialize_context_t *c,
                        OT::cff2_subset_plan &plan,
                        స్థిరరాశి cff2_శిఖరం_dict_విలువలు_t &cff2_శిఖరం_Dict,
                        స్థిరరాశి OT::cff2::accelerator_subset_t &acc)
{
  TRACE_SERIALIZE (ఇది);

  /*
   * CFF1 Serialization Order (reverse, as HarfBuzz packs from end):
   * 1. CharStrings
   * 2. Private DICs & Local Subrs
   * 3. FDArray
   * 4. FDSelect
   * 5. Charset
   * 6. Global Subrs
   * 7. String INDEX
   * 8. Top DICT INDEX
   * 9. Name INDEX
   * 10. Header
   */

  // 0. Optimize width encoding (for all FDs)
  {
    // Collect widths from hmtx_map
    hb_సదిశ_t<గుర్తులేని> widths;
    widths.alloc (plan.అంకె_glyphs);

    కొరకు (గుర్తులేని gid = 0; gid < plan.అంకె_glyphs; gid++)
    {
      గుర్తులేని వెడల్పు = 0;
      ఒకవేళ (plan.hmtx_map->has (gid))
        వెడల్పు = plan.hmtx_map->get (gid).మొదటి;
      widths.push (వెడల్పు);
    }

    // Optimize defaultWidthX and nominalWidthX
    CFF::optimize_widths (widths, plan.default_వెడల్పు, plan.nominal_వెడల్పు);
  }

  // 1. CharStrings (with widths prepended)
  ఒకవేళ (!_serialize_cff1_charstrings (c, plan, plan.default_వెడల్పు, plan.nominal_వెడల్పు))
    return_trace (అబద్ధం);

  // 2. Private DICs & Local Subrs (same as CFF2)
  hb_సదిశ_t<పట్టిక_info_t> వ్యక్తిగత_dict_infos;
  ఒకవేళ (unlikely (!వ్యక్తిగత_dict_infos.resize (plan.subset_fdcount)))
    return_trace (అబద్ధం);

  కొరకు (పూర్ణాంకం i = (పూర్ణాంకం)acc.వ్యక్తిగత_Dicts.నిడివి; --i >= 0;)
  {
    ఒకవేళ (plan.fdmap.has (i))
    {
      objidx_t subrs_link = 0;

      ఒకవేళ (plan.subset_localsubrs[i].నిడివి > 0)
      {
        స్వయంచాలక *dest = c->push<CFF1Subrs> ();
        ఒకవేళ (likely (dest->serialize (c, plan.subset_localsubrs[i])))
          subrs_link = c->తీయి_pack (అబద్ధం);
        లేకపోతే
        {
          c->తీయి_discard ();
          return_trace (అబద్ధం);
        }
      }

      స్వయంచాలక *pd = c->push<వ్యక్తిగత_Dict> ();
      // Use the CFF2 Private DICT serializer which instantiates blends when pinned=true
      cff2_వ్యక్తిగత_dict_op_serializer_t privSzr (plan.desubroutinize, plan.drop_hints, plan.pinned,
                                                  acc.varStore, plan.normalized_coords);
      ఒకవేళ (likely (pd->serialize (c, acc.వ్యక్తిగత_Dicts[i], privSzr, subrs_link)))
      {
        // Add defaultWidthX and nominalWidthX for CFF1
        str_buff_t వెడల్పు_ops;
        str_encoder_t encoder (వెడల్పు_ops);
        encoder.encode_int (plan.default_వెడల్పు);
        encoder.encode_op (Op_సంకేతం_default_వెడల్పు_X);
        encoder.encode_int (plan.nominal_వెడల్పు);
        encoder.encode_op (Op_సంకేతం_nominal_వెడల్పు_X);

        ఒకవేళ (!encoder.లోపల_error () && c->embed (వెడల్పు_ops.as_బైట్లు ().arrayZ, వెడల్పు_ops.నిడివి))
        {
          గుర్తులేని fd = plan.fdmap[i];
          వ్యక్తిగత_dict_infos[fd].కొలత = c->నిడివి ();
          వ్యక్తిగత_dict_infos[fd].link = c->తీయి_pack ();
        }
        లేకపోతే
        {
          c->తీయి_discard ();
          return_trace (అబద్ధం);
        }
      }
      లేకపోతే
      {
        c->తీయి_discard ();
        return_trace (అబద్ధం);
      }
    }
  }

  // 3. FDArray - serialize CFF2 font dicts as CFF1
  {
    స్వయంచాలక *fda = c->push<FDArray<HBUINT16>> ();
    cff_font_dict_op_serializer_t fontSzr;
    స్వయంచాలక it =
    + hb_zip (+ hb_iter (acc.fontDicts)
              | hb_filter ([&] (స్థిరరాశి cff2_font_dict_విలువలు_t &_)
                { తిరిగి plan.fdmap.has (&_ - &acc.fontDicts[0]); }),
              hb_iter (వ్యక్తిగత_dict_infos))
    ;
    // Explicitly specify template parameters: DICTVAL, INFO
    తార్కిక success = fda->serialize<cff2_font_dict_విలువలు_t, పట్టిక_info_t> (c, it, fontSzr);
    ఒకవేళ (success)
      plan.info.fd_array_link = c->తీయి_pack (అబద్ధం);
    లేకపోతే
    {
      c->తీయి_discard ();
      return_trace (అబద్ధం);
    }
  }

  // 4. FDSelect (required in CFF1 CID-keyed fonts)
  // CFF1 requires FDSelect for all CID-keyed fonts, even with just one FD
  // CFF2 makes it optional when there's only one FD
  ఒకవేళ (acc.fdSelect != &Null (CFF2FDSelect))
  {
    c->push ();
    ఒకవేళ (likely (hb_serialize_cff_fdselect (c, plan.అంకె_glyphs,
                                          *(స్థిరరాశి FDSelect *)acc.fdSelect,
                                          plan.orig_fdcount,
                                          plan.subset_fdselect_format,
                                          plan.subset_fdselect_కొలత,
                                          plan.subset_fdselect_ranges)))
      plan.info.fd_select.link = c->తీయి_pack ();
    లేకపోతే
    {
      c->తీయి_discard ();
      return_trace (అబద్ధం);
    }
  }
  లేకపోతే
  {
    // Create a range-based FDSelect3 mapping all glyphs to FD 0
    // Format: format(1) + nRanges(2) + range(3) + sentinel(2) = 8 bytes
    c->push ();

    // Format byte
    HBUINT8 format;
    format = 3;
    ఒకవేళ (unlikely (!c->embed (format)))
    {
      c->తీయి_discard ();
      return_trace (అబద్ధం);
    }

    // nRanges
    HBUINT16 nRanges;
    nRanges = 1;
    ఒకవేళ (unlikely (!c->embed (nRanges)))
    {
      c->తీయి_discard ();
      return_trace (అబద్ధం);
    }

    // Single range: {first: 0, fd: 0}
    FDSelect3_Range range;
    range.మొదటి = 0;
    range.fd = 0;
    ఒకవేళ (unlikely (!c->embed (range)))
    {
      c->తీయి_discard ();
      return_trace (అబద్ధం);
    }

    // Sentinel (number of glyphs)
    HBUINT16 sentinel;
    sentinel = plan.అంకె_glyphs;
    ఒకవేళ (unlikely (!c->embed (sentinel)))
    {
      c->తీయి_discard ();
      return_trace (అబద్ధం);
    }

    plan.info.fd_select.link = c->తీయి_pack ();
  }

  // 5. Charset (CID charset for identity mapping)
  objidx_t charset_link;
  ఒకవేళ (!_serialize_cff1_charset (c, plan.అంకె_glyphs, charset_link))
    return_trace (అబద్ధం);

  // 6. Global Subrs
  {
    స్వయంచాలక *dest = c->push<CFF1Subrs> ();
    ఒకవేళ (likely (dest->serialize (c, plan.subset_globalsubrs)))
      c->తీయి_pack (అబద్ధం);
    లేకపోతే
    {
      c->తీయి_discard ();
      return_trace (అబద్ధం);
    }
  }

  // 7. String INDEX - Add "Adobe" and "Identity" for ROS operator
  {
    స్థిరరాశి అక్షరం *adobe_str = "Adobe";
    స్థిరరాశి అక్షరం *తటస్థం_str = "Identity";
    గుర్తులేని adobe_len = 5;  // strlen("Adobe")
    గుర్తులేని తటస్థం_len = 8;  // strlen("Identity")

    // Build strings array
    hb_సదిశ_t<hb_ubytes_t> అక్షరశ్రేణులు;
    అక్షరశ్రేణులు.alloc (2);
    అక్షరశ్రేణులు.push (hb_ubytes_t ((స్థిరరాశి గుర్తులేని అక్షరం *) adobe_str, adobe_len));
    అక్షరశ్రేణులు.push (hb_ubytes_t ((స్థిరరాశి గుర్తులేని అక్షరం *) తటస్థం_str, తటస్థం_len));

    // Serialize as CFF INDEX
    స్వయంచాలక *dest = c->push<CFF1_సూచీ> ();
    ఒకవేళ (likely (dest->serialize (c, అక్షరశ్రేణులు)))
      c->తీయి_pack (అబద్ధం);
    లేకపోతే
    {
      c->తీయి_discard ();
      return_trace (అబద్ధం);
    }
  }

  // 8. CFF Header
  OT::cff1 *cff = c->allocate_min<OT::cff1> ();
  ఒకవేళ (unlikely (!cff)) return_trace (అబద్ధం);

  /*శీర్షిక*/
  cff->సంస్కరణ.major = 0x01;
  cff->సంస్కరణ.minor = 0x00;
  cff->name_సూచీ = cff->min_కొలత;
  cff->off_కొలత = 4; /* unused? */

  // 9. Name INDEX (single entry)
  {
    గుర్తులేని name_len = అక్షరశ్రేణి_పొడవు (CFF1_DEFAULT_FONT_NAME);

    CFF1_సూచీ *idx = c->ప్రారంభం_embed<CFF1_సూచీ> ();
    ఒకవేళ (unlikely (!idx)) return_trace (అబద్ధం);

    ఒకవేళ (unlikely (!idx->serialize_శీర్షిక (c, hb_iter (&name_len, 1), name_len)))
      return_trace (అబద్ధం);

    ఒకవేళ (unlikely (!c->embed (CFF1_DEFAULT_FONT_NAME, name_len)))
      return_trace (అబద్ధం);
  }

  // 10. Top DICT INDEX
  {
    // Serialize the Top DICT data first
    c->push<శిఖరం_Dict> ();
    cff1_from_cff2_శిఖరం_dict_op_serializer_t శిఖరం_Szr;

    // Serialize ROS first
    ఒకవేళ (unlikely (!శిఖరం_Szr.serialize_ros (c)))
    {
      c->తీయి_discard ();
      return_trace (అబద్ధం);
    }

    // Serialize FontBBox from head table
    {
      str_buff_t bbox_buff;
      str_encoder_t encoder (bbox_buff);

      encoder.encode_int (plan.తల_maxp_info.xMin);
      encoder.encode_int (plan.తల_maxp_info.yMin);
      encoder.encode_int (plan.తల_maxp_info.xMax);
      encoder.encode_int (plan.తల_maxp_info.yMax);
      encoder.encode_op (Op_సంకేతం_Font_BBox);

      ఒకవేళ (encoder.లోపల_error () || !c->embed (bbox_buff.as_బైట్లు ().arrayZ, bbox_buff.నిడివి))
      {
        c->తీయి_discard ();
        return_trace (అబద్ధం);
      }
    }

    // Serialize charset operator
    ఒకవేళ (charset_link && unlikely (!FontDict::serialize_link4_op (c, Op_సంకేతం_charset, charset_link, whence_t::Absolute)))
    {
      c->తీయి_discard ();
      return_trace (అబద్ధం);
    }

    // Serialize FDSelect operator (required for CID-keyed CFF1 fonts)
    ఒకవేళ (plan.info.fd_select.link && unlikely (!FontDict::serialize_link4_op (c, Op_సంకేతం_FDSelect, plan.info.fd_select.link, whence_t::Absolute)))
    {
      c->తీయి_discard ();
      return_trace (అబద్ధం);
    }

    // Serialize FDArray operator (required for CID-keyed CFF1 fonts)
    ఒకవేళ (plan.info.fd_array_link && unlikely (!FontDict::serialize_link4_op (c, Op_సంకేతం_FDArray, plan.info.fd_array_link, whence_t::Absolute)))
    {
      c->తీయి_discard ();
      return_trace (అబద్ధం);
    }

    // Serialize other operators from CFF2 TopDict
    కొరకు (స్థిరరాశి స్వయంచాలక &opstr : cff2_శిఖరం_Dict.values)
    {
      ఒకవేళ (unlikely (!శిఖరం_Szr.serialize (c, opstr, plan.info)))
      {
        c->తీయి_discard ();
        return_trace (అబద్ధం);
      }
    }

    గుర్తులేని శిఖరం_కొలత = c->నిడివి ();
    c->తీయి_pack (అబద్ధం);

    // Serialize INDEX header
    స్వయంచాలక *dest = c->ప్రారంభం_embed<CFF1_సూచీ> ();
    ఒకవేళ (unlikely (!dest->serialize_శీర్షిక (c, hb_iter (&శిఖరం_కొలత, 1), శిఖరం_కొలత)))
      return_trace (అబద్ధం);
  }

  return_trace (నిజం);
}

} /* namespace CFF */

స్థిర తార్కిక _serialize_cff2_charstrings (hb_serialize_context_t *c,
			     cff2_subset_plan &plan,
			     స్థిరరాశి OT::cff2::accelerator_subset_t  &acc)
{
  c->push ();

  గుర్తులేని దత్తాంశం_కొలత = 0;
  గుర్తులేని మొత్తం_కొలత = CFF2_Char_అక్షరశ్రేణులు::మొత్తం_కొలత (plan.subset_charstrings, &దత్తాంశం_కొలత, plan.min_charstrings_off_కొలత);
  ఒకవేళ (unlikely (!c->ప్రారంభం_zerocopy (మొత్తం_కొలత)))
    తిరిగి అబద్ధం;

  స్వయంచాలక *cs = c->ప్రారంభం_embed<CFF2_Char_అక్షరశ్రేణులు> ();
  ఒకవేళ (unlikely (!cs->serialize (c, plan.subset_charstrings, &దత్తాంశం_కొలత, plan.min_charstrings_off_కొలత)))
  {
    c->తీయి_discard ();
    తిరిగి అబద్ధం;
  }

  plan.info.char_అక్షరశ్రేణులు_link = c->తీయి_pack (అబద్ధం);
  తిరిగి నిజం;
}

తార్కిక
OT::cff2::accelerator_subset_t::serialize (hb_serialize_context_t *c,
					   నిర్మాణం cff2_subset_plan &plan,
					   hb_array_t<పూర్ణాంకం> normalized_coords) స్థిరరాశి
{
  /* push charstrings onto the object stack first which will ensure it packs as the last
     object in the table. Keeping the chastrings last satisfies the requirements for patching
     via IFTB. If this ordering needs to be changed in the future, charstrings should be left
     at the end whenever HB_SUBSET_FLAGS_ITFB_REQUIREMENTS is enabled. */
  ఒకవేళ (!_serialize_cff2_charstrings(c, plan, *ఇది))
    తిరిగి అబద్ధం;

  /* private dicts & local subrs */
  hb_సదిశ_t<పట్టిక_info_t>  వ్యక్తిగత_dict_infos;
  ఒకవేళ (unlikely (!వ్యక్తిగత_dict_infos.resize (plan.subset_fdcount))) తిరిగి అబద్ధం;

  కొరకు (పూర్ణాంకం i = (పూర్ణాంకం)వ్యక్తిగత_Dicts.నిడివి; --i >= 0 ;)
  {
    ఒకవేళ (plan.fdmap.has (i))
    {
      objidx_t	subrs_link = 0;

      ఒకవేళ (plan.subset_localsubrs[i].నిడివి > 0)
      {
	స్వయంచాలక *dest = c->push <CFF2Subrs> ();
	ఒకవేళ (likely (dest->serialize (c, plan.subset_localsubrs[i])))
	  subrs_link = c->తీయి_pack (అబద్ధం);
	లేకపోతే
	{
	  c->తీయి_discard ();
	  తిరిగి అబద్ధం;
	}
      }
      స్వయంచాలక *pd = c->push<వ్యక్తిగత_Dict> ();
      cff2_వ్యక్తిగత_dict_op_serializer_t privSzr (plan.desubroutinize, plan.drop_hints, plan.pinned,
						 varStore, normalized_coords,
						 plan.partial_instancing ? &plan.instancing : శూన్యసూచిక);
      ఒకవేళ (likely (pd->serialize (c, వ్యక్తిగత_Dicts[i], privSzr, subrs_link)))
      {
	గుర్తులేని fd = plan.fdmap[i];
	వ్యక్తిగత_dict_infos[fd].కొలత = c->నిడివి ();
	వ్యక్తిగత_dict_infos[fd].link = c->తీయి_pack ();
      }
      లేకపోతే
      {
	c->తీయి_discard ();
	తిరిగి అబద్ధం;
      }
    }
  }

  /* FDSelect */
  ఒకవేళ (fdSelect != &Null (CFF2FDSelect))
  {
    c->push ();
    ఒకవేళ (likely (hb_serialize_cff_fdselect (c, plan.అంకె_glyphs, *(స్థిరరాశి FDSelect *)fdSelect,
					   plan.orig_fdcount,
					   plan.subset_fdselect_format, plan.subset_fdselect_కొలత,
					   plan.subset_fdselect_ranges)))
      plan.info.fd_select.link = c->తీయి_pack ();
    లేకపోతే
    {
      c->తీయి_discard ();
      తిరిగి అబద్ధం;
    }
  }

  /* FDArray (FD Index) */
  {
    స్వయంచాలక *fda = c->push<CFF2FDArray> ();
    cff_font_dict_op_serializer_t fontSzr;
    స్వయంచాలక it =
    + hb_zip (+ hb_iter (fontDicts)
	      | hb_filter ([&] (స్థిరరాశి cff2_font_dict_విలువలు_t &_)
		{ తిరిగి plan.fdmap.has (&_ - &fontDicts[0]); }),
	      hb_iter (వ్యక్తిగత_dict_infos))
    ;
    ఒకవేళ (unlikely (!fda->serialize (c, it, fontSzr)))
    {
      c->తీయి_discard ();
      తిరిగి అబద్ధం;
    }
    plan.info.fd_array_link = c->తీయి_pack (అబద్ధం);
  }

  /* variation store */
  ఒకవేళ (varStore != &Null (CFF2_అంశం_Variation_Store) &&
      !plan.pinned)
  {
    ఒకవేళ (plan.partial_instancing)
    {
      ఒకవేళ (plan.instancing.has_variations ())
      {
	c->push ();
	ఒకవేళ (unlikely (!plan.instancing.serialize_var_store (c, *plan.axis_ట్యాగులు)))
	{
	  c->తీయి_discard ();
	  తిరిగి అబద్ధం;
	}
	plan.info.var_store_link = c->తీయి_pack (అబద్ధం);
      }
      /* else: the store emptied; the vstore op gets dropped along with it. */
    }
    లేకపోతే
    {
      స్వయంచాలక *dest = c->push<CFF2_అంశం_Variation_Store> ();
      ఒకవేళ (unlikely (!dest->serialize (c, varStore)))
      {
	c->తీయి_discard ();
	తిరిగి అబద్ధం;
      }
      plan.info.var_store_link = c->తీయి_pack (అబద్ధం);
    }
  }

  OT::cff2 *cff2 = c->allocate_min<OT::cff2> ();
  ఒకవేళ (unlikely (!cff2)) తిరిగి అబద్ధం;

  /*శీర్షిక*/
  cff2->సంస్కరణ.major = 0x02;
  cff2->సంస్కరణ.minor = 0x00;
  cff2->శిఖరం_Dict_2 = OT::cff2::static_కొలత;

  /* top dict */
  {
    శిఖరం_Dict &dict = cff2 + cff2->శిఖరం_Dict_2;
    cff2_శిఖరం_dict_op_serializer_t శిఖరం_Szr;
    ఒకవేళ (unlikely (!dict.serialize (c, శిఖరం_Dict_2, శిఖరం_Szr, plan.info))) తిరిగి అబద్ధం;
    cff2->శిఖరం_Dict_కొలత = c->తల_2 - (స్థిరరాశి అక్షరం *)&dict;
  }

  /* global subrs */
  {
    స్వయంచాలక *dest = c->ప్రారంభం_embed <CFF2Subrs> ();
    తిరిగి dest->serialize (c, plan.subset_globalsubrs);
  }
}

తార్కిక
OT::cff2::accelerator_subset_t::subset (hb_subset_context_t *c) స్థిరరాశి
{
  cff2_subset_plan cff2_plan;

  ఒకవేళ (unlikely (!cff2_plan.create (*ఇది, c->plan))) తిరిగి అబద్ధం;

  // If instantiating (pinned) and downgrade flag is set, convert to CFF1
  ఒకవేళ (cff2_plan.pinned && (c->plan->flags & HB_SUBSET_FLAGS_DOWNGRADE_CFF2))
  {
    // Serialize CFF1 to the subsetter's serializer
    // If we run out of room, returning true will cause subsetter to retry with larger buffer
    తార్కిక ఫలితం = CFF::serialize_cff2_గమ్యం_cff1 (c->serializer, cff2_plan, శిఖరం_Dict_2, *ఇది);

    ఒకవేళ (c->serializer->ran_బహిర్గతం_యొక్క_స్థలం ())
      తిరిగి నిజం; // Subsetter will retry with larger buffer

    ఒకవేళ (ఫలితం && !c->serializer->లోపల_error ())
    {
      // Success - end serialization to resolve links
      c->serializer->చివర_serialize ();

      // Copy the serialized CFF1 data and add as CFF table
      hb_blob_t *cff_blob = c->serializer->copy_blob ();
      ఒకవేళ (cff_blob)
      {
        c->plan->కూడు_పట్టిక (HB_ట్యాగ్('C','F','F',' '), cff_blob);
        hb_blob_destroy (cff_blob);

        // Return false to signal CFF2 table is not needed
        తిరిగి అబద్ధం;
      }
    }

    // Conversion failed - don't fall back, fail hard for debugging
    తిరిగి అబద్ధం;
  }

  తిరిగి serialize (c->serializer, cff2_plan,
		    c->plan->normalized_coords.as_array ());
}

#అయితే_ముగింపు
