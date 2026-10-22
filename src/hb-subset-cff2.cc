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

#شامل "hb.hh"

#اگر_غیر_تعریف_شدہ HB_NO_SUBSET_CFF

#شامل "hb-open-type.hh"
#شامل "hb-ot-cff2-table.hh"
#شامل "hb-ot-var-common.hh"
#شامل "hb-set.h"
#شامل "hb-subset-plan.hh"
#شامل "hb-subset-cff-common.hh"
#شامل "hb-cff2-interp-cs.hh"
#شامل "hb-subset-cff2-to-cff1.hh"

استعمال نامخلا CFF;

نامخلا CFF {

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
ساخت cff2_instancing_plan_t
{
  ساخت vardata_transform_t
  {
    /* new_deltas[j] = Σ_i matrix[j][i] * old_deltas[i] */
    hb_سمتیہ_t<hb_سمتیہ_t<اعشاری>> میٹرکس;
    /* Default-value fold-in scalars, one per old region: the old store's
     * region scalars at the new default location. */
    hb_سمتیہ_t<اعشاری> gains;
    /* Surviving regions, parallel to matrix rows. */
    hb_سمتیہ_t<hb_hashmap_t<hb_لیبل_t, Triple>> regions;
    /* Index into the new global region list, per new region (finalize). */
    hb_سمتیہ_t<غیرعلامتی> region_indices;
    غیرعلامتی new_major = 0;
    منطقی alive_ = جھوٹ;
    منطقی alive () ثابت { واپس alive_; }

    /* One value's delta for new region j, from its old per-region deltas. */
    دوہرا transform_delta (غیرعلامتی j, hb_array_t<ثابت نمبر_t> deltas) ثابت
    {
      ثابت hb_سمتیہ_t<اعشاری> &ستون = میٹرکس.arrayZ[j];
      دوہرا d = 0.;
      غیرعلامتی گنتی = hb_min (ستون.لمبائی, deltas.لمبائی);
      برائے (غیرعلامتی c = 0; c < گنتی; c++)
	d += (دوہرا) ستون.arrayZ[c] * deltas.arrayZ[c].مقصد_real ();
      واپس d;
    }
  };

  hb_سمتیہ_t<vardata_transform_t> transforms;  /* per old VarData (vsindex) */
  hb_سمتیہ_t<hb_hashmap_t<hb_لیبل_t, Triple>> new_regions;  /* global region list */
  hb_سیٹ_t مستعمل_majors;
  غیرعلامتی new_major_گنتی = 0;

  منطقی has_variations () ثابت { واپس new_major_گنتی; }

  ثابت vardata_transform_t *get_transform (غیرعلامتی major) ثابت
  { واپس major < transforms.لمبائی ? &transforms.arrayZ[major] : خالی_اشاریہ; }

  /* Record that a retained blend references this VarData. */
  خالی note_use (غیرعلامتی major) { مستعمل_majors.جوڑو (major); }

  /* The new vsindex a private dict's (or charstring's) old ivs maps to;
   * dead VarDatas fall back to 0, their blends having become static. */
  غیرعلامتی remap_ivs (غیرعلامتی old_ivs) ثابت
  {
    ثابت vardata_transform_t *t = get_transform (old_ivs);
    واپس (t && t->alive ()) ? t->new_major : 0;
  }

  دوہرا fold_deltas (غیرعلامتی old_ivs, hb_array_t<ثابت نمبر_t> deltas) ثابت
  {
    ثابت vardata_transform_t *t = get_transform (old_ivs);
    اگر (unlikely (!t || t->gains.لمبائی != deltas.لمبائی)) واپس 0.;
    دوہرا v = 0.;
    برائے (غیرعلامتی i = 0; i < deltas.لمبائی; i++)
      v += (دوہرا) t->gains.arrayZ[i] * deltas.arrayZ[i].مقصد_real ();
    واپس v;
  }

  منطقی create (ثابت CFF2_شے_Variation_Store *cff2VarStore,
	       ثابت hb_subset_plan_t *plan)
  {
    ثابت OT::شے_Variation_Store &varStore = cff2VarStore->varStore;
    غیرعلامتی major_گنتی = varStore.get_sub_جدول_گنتی ();
    اگر (!transforms.resize (major_گنتی)) واپس جھوٹ;

    اگر (!varStore.get_region_list ().get_var_regions (plan->axes_old_اشاریہ_لیبل_map, orig_regions))
      واپس جھوٹ;

    /* Instance each VarData with identity delta rows; the surviving tuples'
     * delta columns are the old-region → new-region transform. */
    OT::optimize_scratch_t scratch;
    برائے (غیرعلامتی m = 0; m < major_گنتی; m++)
    {
      ثابت OT::Var_ڈیٹا &var_ڈیٹا = varStore.get_sub_جدول (m);
      غیرعلامتی k = var_ڈیٹا.get_region_اشاریہ_گنتی ();
      vardata_transform_t &t = transforms.arrayZ[m];

      اگر (!t.gains.resize_exact (k)) واپس جھوٹ;
      varStore.get_region_scalars (m, plan->normalized_coords.arrayZ,
				   plan->normalized_coords.لمبائی,
				   t.gains.arrayZ, k);

      OT::tuple_variations_t tuples;
      برائے (غیرعلامتی r = 0; r < k; r++)
      {
	OT::tuple_delta_t tuple;
	اگر (!tuple.deltas_x.resize (k) ||
	    !tuple.indices.resize (k))
	  واپس جھوٹ;
	برائے (غیرعلامتی i = 0; i < k; i++)
	  tuple.indices.arrayZ[i] = سچ;
	tuple.deltas_x.arrayZ[r] = 1.f;
	غیرعلامتی region_اشاریہ = var_ڈیٹا.get_region_اشاریہ (r);
	اگر (unlikely (region_اشاریہ >= orig_regions.لمبائی)) واپس جھوٹ;
	tuple.axis_tuples = orig_regions.arrayZ[region_اشاریہ];
	اگر (unlikely (tuple.axis_tuples.اندر_error ())) واپس جھوٹ;
	tuples.tuple_vars.push (std::move (tuple));
      }
      اگر (unlikely (tuples.tuple_vars.اندر_error ())) واپس جھوٹ;

      اگر (!tuples.instantiate (plan->axes_location, plan->axes_triple_distances, scratch))
	واپس جھوٹ;

      برائے (خودکار &tuple : tuples.tuple_vars)
      {
	اگر (unlikely (tuple.deltas_x.لمبائی != k)) واپس جھوٹ;
	hb_سمتیہ_t<اعشاری> ستون;
	اگر (unlikely (!ستون.resize_exact (k))) واپس جھوٹ;
	برائے (غیرعلامتی i = 0; i < k; i++)
	  ستون.arrayZ[i] = tuple.deltas_x.arrayZ[i];
	t.میٹرکس.push (std::move (ستون));
	t.regions.push (std::move (tuple.axis_tuples));
      }
      اگر (unlikely (t.میٹرکس.اندر_error () || t.regions.اندر_error ())) واپس جھوٹ;
    }

    واپس سچ;
  }

  /* Decide which VarDatas survive, number them, and build the new global
   * region list: surviving original regions first, in their original order,
   * then solver-split new regions in order of appearance (fontTools'
   * rebuildRegions + prune_regions). */
  منطقی finalize ()
  {
    برائے (غیرعلامتی m = 0; m < transforms.لمبائی; m++)
    {
      vardata_transform_t &t = transforms.arrayZ[m];
      t.alive_ = t.میٹرکس.لمبائی && مستعمل_majors.has (m);
      اگر (t.alive_)
	t.new_major = new_major_گنتی++;
    }

    /* Keys hash by content, so pointers into different owners compare fine. */
    hb_hashmap_t<ثابت hb_hashmap_t<hb_لیبل_t, Triple>*, غیرعلامتی> region_idx_map;
    hb_سمتیہ_t<ثابت hb_hashmap_t<hb_لیبل_t, Triple>*> region_ptrs;
    برائے (ثابت vardata_transform_t &t : transforms)
    {
      اگر (!t.alive ()) جاری;
      برائے (ثابت خودکار &region : t.regions)
	اگر (!region_idx_map.has (&region))
	  اگر (unlikely (!region_idx_map.سیٹ (&region, (غیرعلامتی) -1)))
	    واپس جھوٹ;
    }
    برائے (ثابت خودکار &r : orig_regions)
    {
      غیرعلامتی *idx;
      اگر (region_idx_map.has (&r, &idx) && *idx == (غیرعلامتی) -1)
      {
	اگر (unlikely (!region_idx_map.سیٹ (&r, region_ptrs.لمبائی))) واپس جھوٹ;
	region_ptrs.push (&r);
      }
    }
    برائے (ثابت vardata_transform_t &t : transforms)
    {
      اگر (!t.alive ()) جاری;
      برائے (ثابت خودکار &region : t.regions)
      {
	غیرعلامتی *idx;
	اگر (region_idx_map.has (&region, &idx) && *idx == (غیرعلامتی) -1)
	{
	  اگر (unlikely (!region_idx_map.سیٹ (&region, region_ptrs.لمبائی))) واپس جھوٹ;
	  region_ptrs.push (&region);
	}
      }
    }
    اگر (unlikely (region_ptrs.اندر_error ())) واپس جھوٹ;

    برائے (vardata_transform_t &t : transforms)
    {
      اگر (!t.alive ()) جاری;
      برائے (ثابت خودکار &region : t.regions)
      {
	غیرعلامتی *idx;
	اگر (unlikely (!region_idx_map.has (&region, &idx))) واپس جھوٹ;
	t.region_indices.push (*idx);
      }
      اگر (unlikely (t.region_indices.اندر_error ())) واپس جھوٹ;
    }

    اگر (!new_regions.resize (region_ptrs.لمبائی)) واپس جھوٹ;
    برائے (غیرعلامتی i = 0; i < region_ptrs.لمبائی; i++)
    {
      new_regions.arrayZ[i] = *region_ptrs.arrayZ[i];
      اگر (unlikely (new_regions.arrayZ[i].اندر_error ())) واپس جھوٹ;
    }

    واپس سچ;
  }

  /* Serialize the instanced store: a CFF2ItemVariationStore whose VarDatas
   * carry no items, just region indexes (blend deltas live in charstrings).
   * Laid out manually since all sizes are known up front. */
  منطقی serialize_var_store (hb_serialize_context_t *c,
			    ثابت hb_سمتیہ_t<hb_لیبل_t> &axis_لیبل_فہرست) ثابت
  {
    غیرعلامتی ڈیٹا_گنتی = new_major_گنتی;
    غیرعلامتی سرخی_سائز = 2 + 4 + 2 + 4 * ڈیٹا_گنتی;

    uint64_t انحراف = سرخی_سائز;
    hb_سمتیہ_t<غیرعلامتی> ڈیٹا_offsets;
    برائے (ثابت vardata_transform_t &t : transforms)
    {
      اگر (!t.alive ()) جاری;
      ڈیٹا_offsets.push ((غیرعلامتی) انحراف);
      انحراف += 6 + 2 * (uint64_t) t.region_indices.لمبائی;
    }
    اگر (unlikely (ڈیٹا_offsets.اندر_error () || ڈیٹا_offsets.لمبائی != ڈیٹا_گنتی))
      واپس جھوٹ;
    غیرعلامتی regions_انحراف = (غیرعلامتی) انحراف;
    uint64_t کل = انحراف + 4 + (uint64_t) new_regions.لمبائی * axis_لیبل_فہرست.لمبائی * 6;
    اگر (unlikely (کل > 0xFFFFFFFFu)) واپس جھوٹ;

    HBUINT16 *سائز_میدان = c->allocate_سائز<HBUINT16> (HBUINT16::static_سائز);
    اگر (unlikely (!سائز_میدان)) واپس جھوٹ;
    /* OFF 2026 5.3.3.11.2 permits larger stores with length=65535. */
    *سائز_میدان = hb_min (کل, (uint64_t) 0xFFFFu);

    حرف *سرخی = c->allocate_سائز<حرف> (سرخی_سائز);
    اگر (unlikely (!سرخی)) واپس جھوٹ;
    * (HBUINT16 *) (سرخی + 0) = 1;			/* format */
    * (HBUINT32 *) (سرخی + 2) = regions_انحراف;
    * (HBUINT16 *) (سرخی + 6) = ڈیٹا_گنتی;
    برائے (غیرعلامتی i = 0; i < ڈیٹا_گنتی; i++)
      * (HBUINT32 *) (سرخی + 8 + 4 * i) = ڈیٹا_offsets.arrayZ[i];

    برائے (ثابت vardata_transform_t &t : transforms)
    {
      اگر (!t.alive ()) جاری;
      غیرعلامتی k = t.region_indices.لمبائی;
      HBUINT16 *v = (HBUINT16 *) c->allocate_سائز<حرف> (6 + 2 * k);
      اگر (unlikely (!v)) واپس جھوٹ;
      v[0] = 0;						/* itemCount */
      v[1] = 0;						/* wordSizeCount */
      v[2] = k;						/* regionIndices.len */
      برائے (غیرعلامتی j = 0; j < k; j++)
	v[3 + j] = t.region_indices.arrayZ[j];
    }

    hb_سمتیہ_t<ثابت hb_hashmap_t<hb_لیبل_t, Triple>*> region_ptrs;
    اگر (unlikely (!region_ptrs.alloc_exact (new_regions.لمبائی))) واپس جھوٹ;
    برائے (ثابت خودکار &r : new_regions)
      region_ptrs.push (&r);
    خودکار *region_list = c->آغاز_embed<OT::VarRegionList> ();
    واپس region_list->serialize (c, axis_لیبل_فہرست, region_ptrs);
  }

  نجی:
  hb_سمتیہ_t<hb_hashmap_t<hb_لیبل_t, Triple>> orig_regions;
};

/* Usage-collection pass for partial instancing: record which VarDatas the
 * retained charstrings' and private dicts' blends reference.  Mirrors the
 * emission pass: blends consumed by dropped hint ops don't count. */
ساخت cff2_usage_param_t
{
  خالی init () {}

  cff2_instancing_plan_t *instancing = خالی_اشاریہ;
  منطقی drop_hints = جھوٹ;
  غیرعلامتی ivs = 0;	/* dict-side current vsindex */
};

ساخت cff2_cs_opset_usage_t : cff2_cs_opset_t<cff2_cs_opset_usage_t, cff2_usage_param_t, blend_arg_t>
{
  ساکن خالی flush_دلائل_and_op (op_رمز_t op, cff2_cs_interp_env_t<blend_arg_t> &env, cff2_usage_param_t& param)
  {
    منطقی counts = سچ;
    انتخاب (op)
    {
      حالت Op_رمز_return:
      حالت Op_رمز_endchar:
	counts = جھوٹ;
	توڑو;

      حالت Op_رمز_hstem:
      حالت Op_رمز_hstemhm:
      حالت Op_رمز_vstem:
      حالت Op_رمز_vstemhm:
      حالت Op_رمز_hintmask:
      حالت Op_رمز_cntrmask:
	اگر (param.drop_hints)
	  counts = جھوٹ;
	توڑو;

      طےشدہ:
	توڑو;
    }

    اگر (counts)
      برائے (غیرعلامتی صحیح i = 0; i < env.arg_ڈھیر.get_گنتی (); i++)
	اگر (env.arg_ڈھیر[i].blending ())
	{
	  param.instancing->note_use (env.get_ivs ());
	  توڑو;
	}

    env.clear_دلائل ();
  }
};

ساخت cff2_نجی_dict_usage_opset_t : dict_opset_t
{
  ساکن خالی process_op (op_رمز_t op, cff2_priv_dict_interp_env_t& env, cff2_usage_param_t& param)
  {
    انتخاب (op) {
      حالت Op_رمز_vsindexdict:
	env.process_vsindex ();
	param.ivs = env.get_ivs ();
	env.clear_دلائل ();
	واپس;
      حالت Op_رمز_blenddict:
	param.instancing->note_use (param.ivs);
	env.clear_دلائل ();
	واپس;
      طےشدہ:
	dict_opset_t::process_op (op, env);
	اگر (!env.arg_ڈھیر.is_empty ()) واپس;
	env.clear_دلائل ();
	واپس;
    }
  }
};

} /* namespace CFF */

ساخت cff2_sub_جدول_info_t : cff_sub_جدول_info_t
{
  cff2_sub_جدول_info_t ()
    : cff_sub_جدول_info_t (),
      var_store_link (0)
  {}

  objidx_t  var_store_link;
};

ساخت cff2_چوٹی_dict_op_serializer_t : cff_چوٹی_dict_op_serializer_t<>
{
  منطقی serialize (hb_serialize_context_t *c,
		  ثابت op_str_t &opstr,
		  ثابت cff2_sub_جدول_info_t &info) ثابت
  {
    TRACE_SERIALIZE (یہ);

    انتخاب (opstr.op)
    {
      حالت Op_رمز_vstore:
        اگر (info.var_store_link)
	  return_trace (FontDict::serialize_link4_op(c, opstr.op, info.var_store_link));
	ورنہ
	  return_trace (سچ);

      طےشدہ:
	return_trace (cff_چوٹی_dict_op_serializer_t<>::serialize (c, opstr, info));
    }
  }
};

ساخت cff2_cs_opset_flatten_t : cff2_cs_opset_t<cff2_cs_opset_flatten_t, flatten_param_t, blend_arg_t>
{
  ساکن خالی flush_دلائل_and_op (op_رمز_t op, cff2_cs_interp_env_t<blend_arg_t> &env, flatten_param_t& param)
  {
    /* Optionally capture command for specialization (before flushing, to preserve args) */
    اگر (param.commands)
    {
      منطقی skip_command = جھوٹ;

      انتخاب (op)
      {
	حالت Op_رمز_return:
	حالت Op_رمز_endchar:
	  skip_command = سچ;
	  توڑو;

	حالت Op_رمز_hstem:
	حالت Op_رمز_hstemhm:
	حالت Op_رمز_vstem:
	حالت Op_رمز_vstemhm:
	حالت Op_رمز_hintmask:
	حالت Op_رمز_cntrmask:
	  اگر (param.drop_hints)
	    skip_command = سچ;
	  توڑو;

	طےشدہ:
	  توڑو;
      }

      اگر (!skip_command)
      {
	cs_command_t cmd (op);
	/* Capture resolved blend values */
	برائے (غیرعلامتی صحیح i = 0; i < env.arg_ڈھیر.get_گنتی ();)
	{
	  ثابت blend_arg_t &arg = env.arg_ڈھیر[i];
	  اگر (arg.blending ())
	  {
	    /* For blend args, capture only the resolved default value */
	    cmd.دلائل.push (arg);
	    /* Skip over the multiple blend values */
	    i += arg.عدد_اقدار_2;
	  }
	  ورنہ
	  {
	    cmd.دلائل.push (arg);
	    i++;
	  }
	}
	param.commands->push (cmd);
      }
    }

    انتخاب (op)
    {
      حالت Op_رمز_return:
      حالت Op_رمز_endchar:
	/* dummy opcodes in CFF2. ignore */
	اگر (op == Op_رمز_endchar && param.instancer)
	  emit_vsindex_سابقہ (env, param);
	توڑو;

      حالت Op_رمز_hstem:
      حالت Op_رمز_hstemhm:
      حالت Op_رمز_vstem:
      حالت Op_رمز_vstemhm:
      حالت Op_رمز_hintmask:
      حالت Op_رمز_cntrmask:
	اگر (param.drop_hints)
	{
	  env.clear_دلائل ();
	  واپس;
	}
	HB_FALLTHROUGH;

      طےشدہ:
	SUPER::flush_دلائل_and_op (op, env, param);
	توڑو;
    }
  }

  ساکن خالی flush_دلائل (cff2_cs_interp_env_t<blend_arg_t> &env, flatten_param_t& param)
  {
    برائے (غیرعلامتی صحیح i = 0; i < env.arg_ڈھیر.get_گنتی ();)
    {
      ثابت blend_arg_t &arg = env.arg_ڈھیر[i];
      اگر (arg.blending ())
      {
	اگر (unlikely (!((arg.عدد_اقدار_2 > 0) && (env.arg_ڈھیر.get_گنتی () >= arg.عدد_اقدار_2))))
	{
	  env.سیٹ_error ();
	  واپس;
	}
	اگر (param.instancer)
	  rewrite_blends (arg, i, env, param);
	ورنہ
	  flatten_blends (arg, i, env, param);
	i += arg.عدد_اقدار_2;
      }
      ورنہ
      {
	str_encoder_t  encoder (param.flatStr);
	encoder.encode_عدد_cs (arg);
	i++;
      }
    }
    SUPER::flush_دلائل (env, param);
  }

  ساکن خالی flatten_blends (ثابت blend_arg_t &arg, غیرعلامتی صحیح i, cff2_cs_interp_env_t<blend_arg_t> &env, flatten_param_t& param)
  {
    /* flatten the default values */
    str_encoder_t  encoder (param.flatStr);
    برائے (غیرعلامتی صحیح j = 0; j < arg.عدد_اقدار_2; j++)
    {
      ثابت blend_arg_t &arg1 = env.arg_ڈھیر[i + j];
      اگر (unlikely (!((arg1.blending () && (arg.عدد_اقدار_2 == arg1.عدد_اقدار_2) && (arg1.قدر_اشاریہ_2 == j) &&
	      (arg1.deltas.لمبائی == env.get_region_گنتی ())))))
      {
	env.سیٹ_error ();
	واپس;
      }
      encoder.encode_عدد_cs (arg1);
    }
    /* flatten deltas for each value */
    برائے (غیرعلامتی صحیح j = 0; j < arg.عدد_اقدار_2; j++)
    {
      ثابت blend_arg_t &arg1 = env.arg_ڈھیر[i + j];
      برائے (غیرعلامتی صحیح k = 0; k < arg1.deltas.لمبائی; k++)
	encoder.encode_عدد_cs (arg1.deltas[k]);
    }
    /* flatten the number of values followed by blend operator */
    encoder.encode_int (arg.عدد_اقدار_2);
    encoder.encode_op (Op_رمز_blendcs);
  }

  /* Partial instancing: rewrite one blend group against the instanced store.
   * Each value's default absorbs the folded (pinned-away) contribution, and
   * its deltas are mapped onto the new region list. */
  ساکن خالی rewrite_blends (ثابت blend_arg_t &arg, غیرعلامتی صحیح i, cff2_cs_interp_env_t<blend_arg_t> &env, flatten_param_t& param)
  {
    ثابت cff2_instancing_plan_t &instancer = *param.instancer;
    غیرعلامتی ivs = env.get_ivs ();
    ثابت cff2_instancing_plan_t::vardata_transform_t *t = instancer.get_transform (ivs);
    اگر (unlikely (!t))
    {
      env.سیٹ_error ();
      واپس;
    }

    غیرعلامتی n = arg.عدد_اقدار_2;
    برائے (غیرعلامتی صحیح j = 0; j < n; j++)
    {
      ثابت blend_arg_t &arg1 = env.arg_ڈھیر[i + j];
      اگر (unlikely (!(arg1.blending () && (arg1.عدد_اقدار_2 == n) && (arg1.قدر_اشاریہ_2 == j) &&
		      (arg1.deltas.لمبائی == env.get_region_گنتی ()) &&
		      (arg1.deltas.لمبائی == t->gains.لمبائی))))
      {
	env.سیٹ_error ();
	واپس;
      }
    }

    str_encoder_t encoder (param.flatStr);
    غیرعلامتی k_new = t->alive () ? t->میٹرکس.لمبائی : 0;

    اگر (!k_new)
    {
      /* All of this VarData's regions died; blends dissolve into statics. */
      برائے (غیرعلامتی صحیح j = 0; j < n; j++)
      {
	ثابت blend_arg_t &arg1 = env.arg_ڈھیر[i + j];
	نمبر_t v;
	v.سیٹ_real (arg1.مقصد_real () + round (instancer.fold_deltas (ivs, arg1.deltas.as_array ())));
	encoder.encode_عدد_cs (v);
      }
      واپس;
    }

    /* Emit in chunks so the output charstring stays within the argument
     * stack limit: values already emitted for this op, plus this chunk's
     * defaults, deltas and count, must not exceed 513. */
    غیرعلامتی مکمل = 0;
    جب (مکمل < n)
    {
      غیرعلامتی avail = 512 > (i + مکمل) ? 512 - (i + مکمل) : 0;
      غیرعلامتی chunk = hb_min (avail / (k_new + 1), n - مکمل);
      اگر (unlikely (!chunk))
      {
	env.سیٹ_error ();
	واپس;
      }

      برائے (غیرعلامتی صحیح j = مکمل; j < مکمل + chunk; j++)
      {
	ثابت blend_arg_t &arg1 = env.arg_ڈھیر[i + j];
	نمبر_t v;
	v.سیٹ_real (arg1.مقصد_real () + round (instancer.fold_deltas (ivs, arg1.deltas.as_array ())));
	encoder.encode_عدد_cs (v);
      }
      برائے (غیرعلامتی صحیح j = مکمل; j < مکمل + chunk; j++)
      {
	ثابت blend_arg_t &arg1 = env.arg_ڈھیر[i + j];
	برائے (غیرعلامتی r = 0; r < k_new; r++)
	{
	  نمبر_t v;
	  v.سیٹ_int (hb_clamp_مقصد<صحیح> (round (t->transform_delta (r, arg1.deltas.as_array ()))));
	  encoder.encode_عدد_cs (v);
	}
      }
      encoder.encode_int (chunk);
      encoder.encode_op (Op_رمز_blendcs);
      مکمل += chunk;
    }
    param.emitted_blend = سچ;
  }

  /* Partial instancing: prepend a vsindex op when the charstring's
   * (remapped) ivs differs from its FD's (remapped) private-dict ivs. */
  ساکن خالی emit_vsindex_سابقہ (cff2_cs_interp_env_t<blend_arg_t> &env, flatten_param_t& param)
  {
    اگر (!param.emitted_blend) واپس;
    ثابت cff2_instancing_plan_t &instancer = *param.instancer;
    غیرعلامتی new_ivs = instancer.remap_ivs (env.get_ivs ());
    غیرعلامتی fd_ivs = instancer.remap_ivs (env.get_orig_ivs ());
    اگر (new_ivs == fd_ivs) واپس;

    str_buff_t سابقہ;
    str_encoder_t enc (سابقہ);
    enc.encode_int ((صحیح) new_ivs);
    enc.encode_op (Op_رمز_vsindexcs);

    str_buff_t &buf = param.flatStr;
    غیرعلامتی old_len = buf.لمبائی;
    اگر (unlikely (سابقہ.اندر_error () || !buf.resize (old_len + سابقہ.لمبائی)))
    {
      env.سیٹ_error ();
      واپس;
    }
    memmove (buf.arrayZ + سابقہ.لمبائی, buf.arrayZ, old_len);
    hb_memcpy (buf.arrayZ, سابقہ.arrayZ, سابقہ.لمبائی);
  }

  ساکن خالی flush_op (op_رمز_t op, cff2_cs_interp_env_t<blend_arg_t> &env, flatten_param_t& param)
  {
    انتخاب (op)
    {
      حالت Op_رمز_return:
      حالت Op_رمز_endchar:
	واپس;
      طےشدہ:
	str_encoder_t  encoder (param.flatStr);
	encoder.encode_op (op);
    }
  }

  ساکن خالی flush_hintmask (op_رمز_t op, cff2_cs_interp_env_t<blend_arg_t> &env, flatten_param_t& param)
  {
    SUPER::flush_hintmask (op, env, param);
    /* Preserve hintmask payload in captured commands for specializer re-encoding. */
    اگر (param.commands && !param.drop_hints && param.commands->لمبائی > 0)
    {
      خودکار &cmd = param.commands->دم ();
      اگر (cmd.op == op)
      {
        cmd.نقاب_بائٹیں.resize (env.hintmask_سائز);
        اگر (unlikely (cmd.نقاب_بائٹیں.اندر_error ()))
        {
          env.سیٹ_error ();
          واپس;
        }
        برائے (غیرعلامتی صحیح i = 0; i < env.hintmask_سائز; i++)
          cmd.نقاب_بائٹیں[i] = env.str_ref[i];
      }
    }
    اگر (!param.drop_hints)
    {
      str_encoder_t  encoder (param.flatStr);
      برائے (غیرعلامتی صحیح i = 0; i < env.hintmask_سائز; i++)
	encoder.encode_byte (env.str_ref[i]);
    }
  }

  نجی:
  قسمتعریف cff2_cs_opset_t<cff2_cs_opset_flatten_t, flatten_param_t, blend_arg_t> SUPER;
  قسمتعریف cs_opset_t<blend_arg_t, cff2_cs_opset_flatten_t, cff2_cs_opset_flatten_t, cff2_cs_interp_env_t<blend_arg_t>, flatten_param_t> CSOPSET;
};

ساخت cff2_cs_opset_subr_subset_t : cff2_cs_opset_t<cff2_cs_opset_subr_subset_t, subr_subset_param_t, blend_arg_t>
{
  ساکن خالی process_op (op_رمز_t op, cff2_cs_interp_env_t<blend_arg_t> &env, subr_subset_param_t& param)
  {
    انتخاب (op) {

      حالت Op_رمز_return:
	param.current_parsed_str->flush_coalesced (env.str_ref);
	param.current_parsed_str->سیٹ_parsed ();
	env.return_from_subr ();
	param.سیٹ_current_str (env, جھوٹ);
	توڑو;

      حالت Op_رمز_endchar:
	param.current_parsed_str->flush_coalesced (env.str_ref);
	param.current_parsed_str->سیٹ_parsed ();
	SUPER::process_op (op, env, param);
	توڑو;

      حالت Op_رمز_callsubr:
	process_call_subr (op, CSType_LocalSubr, env, param, env.localSubrs, param.local_احاطہ_2);
	توڑو;

      حالت Op_رمز_callgsubr:
	process_call_subr (op, CSType_GlobalSubr, env, param, env.globalSubrs, param.global_احاطہ);
	توڑو;

      طےشدہ:
	SUPER::process_op (op, env, param);
	param.current_parsed_str->جوڑو_op (op, env.str_ref);
	توڑو;
    }
  }

  محفوظ:
  ساکن خالی process_call_subr (op_رمز_t op, cs_type_t type,
				 cff2_cs_interp_env_t<blend_arg_t> &env, subr_subset_param_t& param,
				 cff2_biased_subrs_t& subrs, hb_سیٹ_t *احاطہ)
  {
    byte_str_ref_t    str_ref = env.str_ref;
    env.call_subr (subrs, type);
    param.current_parsed_str->جوڑو_call_op (op, str_ref, env.context.subr_عدد_2);
    احاطہ->جوڑو (env.context.subr_عدد_2);
    param.سیٹ_current_str (env, سچ);
  }

  نجی:
  قسمتعریف cff2_cs_opset_t<cff2_cs_opset_subr_subset_t, subr_subset_param_t, blend_arg_t> SUPER;
};

ساخت cff2_subr_subsetter_t : subr_subsetter_t<cff2_subr_subsetter_t, CFF2Subrs, ثابت OT::cff2::accelerator_subset_t, cff2_cs_interp_env_t<blend_arg_t>, cff2_cs_opset_subr_subset_t>
{
  cff2_subr_subsetter_t (ثابت OT::cff2::accelerator_subset_t &acc_, ثابت hb_subset_plan_t *plan_)
    : subr_subsetter_t (acc_, plan_) {}

  ساکن خالی complete_parsed_str (cff2_cs_interp_env_t<blend_arg_t> &env, subr_subset_param_t& param, parsed_cs_str_t &charstring)
  {
    /* vsindex is inserted at the beginning of the charstring as necessary */
    اگر (env.نظر_آیا_vsindex ())
    {
      نمبر_t  ivs;
      ivs.سیٹ_int ((صحیح)env.get_ivs ());
      charstring.سیٹ_سابقہ (ivs, Op_رمز_vsindexcs);
    }
  }
};

ساخت cff2_نجی_blend_encoder_param_t
{
  cff2_نجی_blend_encoder_param_t (hb_serialize_context_t *c,
				      ثابت CFF2_شے_Variation_Store *varStore,
				      hb_array_t<صحیح> normalized_coords) :
    c (c), varStore (varStore), normalized_coords (normalized_coords) {}

  خالی init () {}

  خالی process_blend ()
  {
    اگر (!نظر_آیا_blend)
    {
      region_گنتی_2 = varStore->varStore.get_region_اشاریہ_گنتی (ivs);
      اگر (unlikely (!scalars.resize_exact (region_گنتی_2)))
      {
	اگر (c) c->err (HB_SERIALIZE_ERROR_OTHER);
	نظر_آیا_blend = سچ;
	واپس;
      }
      varStore->varStore.get_region_scalars (ivs, normalized_coords.arrayZ, normalized_coords.لمبائی,
					     &scalars[0], region_گنتی_2);
      نظر_آیا_blend = سچ;
    }
  }

  دوہرا blend_deltas (hb_array_t<ثابت نمبر_t> deltas) ثابت
  {
    دوہرا v = 0;
    اگر (likely (scalars.لمبائی == deltas.لمبائی))
    {
      غیرعلامتی گنتی = scalars.لمبائی;
      برائے (غیرعلامتی i = 0; i < گنتی; i++)
	v += (دوہرا) scalars.arrayZ[i] * deltas.arrayZ[i].مقصد_real ();
    }
    واپس v;
  }


  hb_serialize_context_t *c = خالی_اشاریہ;
  منطقی نظر_آیا_blend = جھوٹ;
  غیرعلامتی ivs = 0;
  غیرعلامتی region_گنتی_2 = 0;
  hb_سمتیہ_t<اعشاری> scalars;
  ثابت	 CFF2_شے_Variation_Store *varStore = خالی_اشاریہ;
  hb_array_t<صحیح> normalized_coords;

  /* CFF2 partial instancing: when set, blends are rewritten against the
   * instanced variation store instead of being flattened. */
  ثابت cff2_instancing_plan_t *instancer = خالی_اشاریہ;
  /* Error-compensated fold rounding state, per dict operator. */
  دوہرا fold_exact = 0.;
  دوہرا fold_emitted = 0.;
};

ساخت cff2_نجی_dict_blend_opset_t : dict_opset_t
{
  ساکن خالی process_arg_blend (cff2_نجی_blend_encoder_param_t& param,
				 نمبر_t &arg,
				 ثابت hb_array_t<ثابت نمبر_t> blends,
				 غیرعلامتی n, غیرعلامتی i)
  {
    arg.سیٹ_int (round (arg.مقصد_real () + param.blend_deltas (blends)));
  }

  /* The fold added to a value, error-compensated across the operator: most
   * blended private-dict values (BlueValues, StemSnap*, ...) are
   * delta-encoded, so independently rounded folds would accumulate in the
   * decoded absolutes; keeping the emitted cumulative fold equal to the
   * rounded exact cumulative fold bounds every absolute within ±0.5. */
  ساکن دوہرا fold (cff2_نجی_blend_encoder_param_t& param,
		      ثابت hb_array_t<ثابت نمبر_t> blends)
  {
    دوہرا f = param.blend_deltas (blends);
    دوہرا emit = round (param.fold_exact + f) - param.fold_emitted;
    param.fold_exact += f;
    param.fold_emitted += emit;
    واپس emit;
  }

  /* Partial instancing: emit the args preceding this blend group, then the
   * group itself rewritten against the instanced store, and clear the
   * stack.  The op's remaining args are flushed at process_op time. */
  ساکن خالی rewrite_blends (cff2_priv_dict_interp_env_t& env,
			      cff2_نجی_blend_encoder_param_t& param,
			      غیرعلامتی آغاز, غیرعلامتی n, غیرعلامتی k)
  {
    ثابت cff2_instancing_plan_t &instancer = *param.instancer;
    ثابت cff2_instancing_plan_t::vardata_transform_t *t = instancer.get_transform (param.ivs);
    اگر (unlikely (!t || t->gains.لمبائی != k))
    {
      env.سیٹ_error ();
      واپس;
    }

    str_buff_t str;
    str_encoder_t encoder (str);

    برائے (غیرعلامتی idx = 0; idx < آغاز; idx++)
      encoder.encode_عدد_tp (env.arg_ڈھیر[idx]);

    غیرعلامتی k_new = t->alive () ? t->میٹرکس.لمبائی : 0;
    اگر (!k_new)
    {
      /* This VarData died (or is unused by any retained charstring, which
       * can happen for FDs kept only for retain-gids holes); blends
       * dissolve into statics. */
      برائے (غیرعلامتی j = 0; j < n; j++)
      {
	ثابت hb_array_t<ثابت نمبر_t> blends = env.arg_ڈھیر.sub_array (آغاز + n + (j * k), k);
	نمبر_t v;
	v.سیٹ_real (env.arg_ڈھیر[آغاز + j].مقصد_real () + fold (param, blends));
	encoder.encode_عدد_tp (v);
      }
    }
    ورنہ
    {
      غیرعلامتی مکمل = 0;
      جب (مکمل < n)
      {
	غیرعلامتی avail = 512 > (آغاز + مکمل) ? 512 - (آغاز + مکمل) : 0;
	غیرعلامتی chunk = hb_min (avail / (k_new + 1), n - مکمل);
	اگر (unlikely (!chunk))
	{
	  env.سیٹ_error ();
	  واپس;
	}

	برائے (غیرعلامتی j = مکمل; j < مکمل + chunk; j++)
	{
	  ثابت hb_array_t<ثابت نمبر_t> blends = env.arg_ڈھیر.sub_array (آغاز + n + (j * k), k);
	  نمبر_t v;
	  v.سیٹ_real (env.arg_ڈھیر[آغاز + j].مقصد_real () + fold (param, blends));
	  encoder.encode_عدد_tp (v);
	}
	برائے (غیرعلامتی j = مکمل; j < مکمل + chunk; j++)
	{
	  ثابت hb_array_t<ثابت نمبر_t> blends = env.arg_ڈھیر.sub_array (آغاز + n + (j * k), k);
	  برائے (غیرعلامتی r = 0; r < k_new; r++)
	  {
	    نمبر_t v;
	    v.سیٹ_int (hb_clamp_مقصد<صحیح> (round (t->transform_delta (r, blends))));
	    encoder.encode_عدد_tp (v);
	  }
	}
	encoder.encode_int (chunk);
	encoder.encode_op (Op_رمز_blenddict);
	مکمل += chunk;
      }
    }

    خودکار بائٹیں = str.as_بائٹیں ();
    param.c->embed (&بائٹیں, بائٹیں.لمبائی);

    env.clear_دلائل ();
  }

  ساکن خالی process_blend (cff2_priv_dict_interp_env_t& env, cff2_نجی_blend_encoder_param_t& param)
  {
    غیرعلامتی صحیح n, k;

    param.process_blend ();
    k = param.region_گنتی_2;
    n = env.arg_ڈھیر.نکالو_uint ();

    غیرعلامتی گنتی = env.arg_ڈھیر.get_گنتی ();
    غیرعلامتی صحیح کل;
    اگر (unlikely (hb_unsigned_mul_overflows (k + 1, n, &کل) || کل > گنتی))
    {
      env.سیٹ_error ();
      واپس;
    }

    /* copy the blend values into blend array of the default values */
    غیرعلامتی صحیح آغاز = گنتی - کل;
    /* let an obvious error case fail, but note CFF2 spec doesn't forbid n==0 */
    اگر (unlikely (آغاز > گنتی))
    {
      env.سیٹ_error ();
      واپس;
    }

    اگر (param.instancer && k)
    {
      rewrite_blends (env, param, آغاز, n, k);
      واپس;
    }

    برائے (غیرعلامتی صحیح i = 0; i < n; i++)
    {
      ثابت hb_array_t<ثابت نمبر_t> blends = env.arg_ڈھیر.sub_array (آغاز + n + (i * k), k);
      process_arg_blend (param, env.arg_ڈھیر[آغاز + i], blends, n, i);
    }

    /* pop off blend values leaving default values now adorned with blend values */
    env.arg_ڈھیر.نکالو (k * n);
  }

  ساکن خالی process_op (op_رمز_t op, cff2_priv_dict_interp_env_t& env, cff2_نجی_blend_encoder_param_t& param)
  {
    انتخاب (op) {
      حالت Op_رمز_Std_HW:
      حالت Op_رمز_Std_VW:
      حالت Op_رمز_Blue_Scale:
      حالت Op_رمز_Blue_سرکاؤ:
      حالت Op_رمز_Blue_Fuzz:
      حالت Op_رمز_Expansion_ضربی_عدد:
      حالت Op_رمز_Language_گروہ:
      حالت Op_رمز_Blue_اقدار:
      حالت Op_رمز_Other_Blues:
      حالت Op_رمز_Family_Blues:
      حالت Op_رمز_Family_Other_Blues:
      حالت Op_رمز_Stem_Snap_H:
      حالت Op_رمز_Stem_Snap_V:
	توڑو;
      حالت Op_رمز_vsindexdict:
	env.process_vsindex ();
	param.ivs = env.get_ivs ();
	اگر (param.instancer)
	{
	  /* Re-emit with the remapped vsindex; 0 is the dict default. */
	  غیرعلامتی new_ivs = param.instancer->remap_ivs (param.ivs);
	  اگر (new_ivs)
	  {
	    str_buff_t str;
	    str_encoder_t encoder (str);
	    encoder.encode_int ((صحیح) new_ivs);
	    encoder.encode_op (Op_رمز_vsindexdict);
	    خودکار بائٹیں = str.as_بائٹیں ();
	    param.c->embed (&بائٹیں, بائٹیں.لمبائی);
	  }
	}
	env.clear_دلائل ();
	واپس;
      حالت Op_رمز_blenddict:
	process_blend (env, param);
	واپس;

      طےشدہ:
	dict_opset_t::process_op (op, env);
	اگر (!env.arg_ڈھیر.is_empty ()) واپس;
	توڑو;
    }

    اگر (unlikely (env.اندر_error ())) واپس;

    // Write args then op

    str_buff_t str;
    str_encoder_t encoder (str);

    غیرعلامتی گنتی = env.arg_ڈھیر.get_گنتی ();
    برائے (غیرعلامتی i = 0; i < گنتی; i++)
      encoder.encode_عدد_tp (env.arg_ڈھیر[i]);

    encoder.encode_op (op);

    خودکار بائٹیں = str.as_بائٹیں ();
    param.c->embed (&بائٹیں, بائٹیں.لمبائی);

    env.clear_دلائل ();
    param.fold_exact = param.fold_emitted = 0.;
  }
};

ساخت cff2_نجی_dict_op_serializer_t : op_serializer_t
{
  cff2_نجی_dict_op_serializer_t (منطقی desubroutinize_, منطقی drop_hints_, منطقی pinned_,
				     ثابت CFF::CFF2_شے_Variation_Store* varStore_,
				     hb_array_t<صحیح> normalized_coords_,
				     ثابت cff2_instancing_plan_t *instancer_ = خالی_اشاریہ)
    : desubroutinize (desubroutinize_), drop_hints (drop_hints_), pinned (pinned_),
      param (خالی_اشاریہ, varStore_, normalized_coords_)
  { param.instancer = instancer_; }

  منطقی serialize (hb_serialize_context_t *c,
		  ثابت op_str_t &opstr,
		  objidx_t subrs_link) ثابت
  {
    TRACE_SERIALIZE (یہ);

    اگر (drop_hints && dict_opset_t::is_hint_op (opstr.op))
      return_trace (سچ);

    اگر (opstr.op == Op_رمز_Subrs)
    {
      اگر (desubroutinize || !subrs_link)
	return_trace (سچ);
      ورنہ
	return_trace (FontDict::serialize_link2_op (c, opstr.op, subrs_link));
    }

    اگر (pinned || param.instancer)
    {
      /* Reinterpret opstr and process blends.  The param persists across
       * this dict's opstrs, so that an ivs set by a vsindexdict opstr is
       * still in effect when a later opstr's blends are processed. */
      cff2_priv_dict_interp_env_t env {hb_ubytes_t (opstr.ptr, opstr.لمبائی)};
      param.c = c;
      dict_interpreter_t<cff2_نجی_dict_blend_opset_t, cff2_نجی_blend_encoder_param_t, cff2_priv_dict_interp_env_t> interp (env);
      return_trace (interp.interpret (param));
    }

    return_trace (copy_opstr (c, opstr));
  }

  محفوظ:
  ثابت منطقی desubroutinize;
  ثابت منطقی drop_hints;
  ثابت منطقی pinned;
  تغیرپذیر cff2_نجی_blend_encoder_param_t param;
};



نامخلا OT {
ساخت cff2_subset_plan
{
  منطقی create (ثابت OT::cff2::accelerator_subset_t &acc,
	      hb_subset_plan_t *plan)
  {
    /* make sure notdef is first */
    hb_codepoint_t old_glyph;
    اگر (!plan->old_gid_for_new_gid (0, &old_glyph) || (old_glyph != 0)) واپس جھوٹ;

    عدد_glyphs = plan->عدد_output_glyphs ();
    orig_fdcount = acc.fdArray->گنتی;

    drop_hints = plan->flags & HB_SUBSET_FLAGS_NO_HINTING;
    منطقی instancing_requested = (منطقی) plan->normalized_coords;
    pinned = instancing_requested && plan->all_axes_pinned;
    normalized_coords = plan->normalized_coords;
    سر_maxp_info = plan->سر_maxp_info;
    hmtx_map = &plan->hmtx_map;
    desubroutinize = plan->flags & HB_SUBSET_FLAGS_DESUBROUTINIZE ||
		     instancing_requested; // For instancing we need this path

    /* Partial instancing: some axes pinned or limited, others kept. */
    partial_instancing = instancing_requested && !pinned &&
			 acc.varStore != &Null (CFF2_شے_Variation_Store);
    اگر (partial_instancing &&
	unlikely (!instancing.create (acc.varStore, plan)))
      واپس جھوٹ;
    axis_لیبل_فہرست = &plan->axis_لیبل_فہرست;

    /* Enable command capture for CFF2→CFF1 conversion (for specialization) */
    capture_commands = pinned;

 #اگر_تعریف_شدہ HB_EXPERIMENTAL_API
    min_charstrings_off_سائز = (plan->flags & HB_SUBSET_FLAGS_IFTB_REQUIREMENTS) ? 4 : 0;
 #ورنہ
    min_charstrings_off_سائز = 0;
 #اگر_ختم

    اگر (desubroutinize)
    {
      اگر (partial_instancing)
      {
	/* Pass 1: collect which VarDatas the retained blends reference, so
	 * unused ones get pruned and the survivors renumbered. */
	hb_سیٹ_t مستعمل_fds;
	برائے (غیرعلامتی صحیح i = 0; i < عدد_glyphs; i++)
	{
	  hb_codepoint_t glyph;
	  اگر (!plan->old_gid_for_new_gid (i, &glyph))
	    جاری;
	  ثابت hb_ubytes_t str = (*acc.char_لڑیاں)[glyph];
	  غیرعلامتی صحیح fd = CFF::subset_get_fd (acc, glyph);
	  اگر (unlikely (fd >= acc.fd_گنتی))
	    واپس جھوٹ;
	  مستعمل_fds.جوڑو (fd);

	  cff2_cs_interp_env_t<blend_arg_t> env (str, acc, fd);
	  cs_interpreter_t<cff2_cs_interp_env_t<blend_arg_t>, cff2_cs_opset_usage_t, cff2_usage_param_t> interp (env);
	  cff2_usage_param_t param;
	  param.instancing = &instancing;
	  param.drop_hints = drop_hints;
	  اگر (unlikely (!interp.interpret (param)))
	    واپس جھوٹ;
	}
	برائے (غیرعلامتی fd : مستعمل_fds)
	{
	  cff2_usage_param_t param;
	  param.instancing = &instancing;
	  غیرعلامتی گنتی = acc.نجی_Dicts[fd].get_گنتی ();
	  برائے (غیرعلامتی j = 0; j < گنتی; j++)
	  {
	    ثابت op_str_t &opstr = acc.نجی_Dicts[fd][j];
	    اگر (drop_hints && dict_opset_t::is_hint_op (opstr.op))
	      جاری;
	    cff2_priv_dict_interp_env_t env {hb_ubytes_t (opstr.ptr, opstr.لمبائی)};
	    dict_interpreter_t<cff2_نجی_dict_usage_opset_t, cff2_usage_param_t, cff2_priv_dict_interp_env_t> interp (env);
	    اگر (unlikely (!interp.interpret (param)))
	      واپس جھوٹ;
	  }
	}
	اگر (unlikely (!instancing.finalize ()))
	  واپس جھوٹ;

	/* Pass 2: like subr_flattener_t, but blends stay symbolic (no coords
	 * fed to the env) and get rewritten against the instanced store. */
	اگر (!subset_charstrings.resize_exact (عدد_glyphs))
	  واپس جھوٹ;
	برائے (غیرعلامتی صحیح i = 0; i < عدد_glyphs; i++)
	{
	  hb_codepoint_t glyph;
	  اگر (!plan->old_gid_for_new_gid (i, &glyph))
	    جاری;
	  ثابت hb_ubytes_t str = (*acc.char_لڑیاں)[glyph];
	  غیرعلامتی صحیح fd = CFF::subset_get_fd (acc, glyph);
	  اگر (unlikely (fd >= acc.fd_گنتی))
	    واپس جھوٹ;

	  cff2_cs_interp_env_t<blend_arg_t> env (str, acc, fd);
	  cs_interpreter_t<cff2_cs_interp_env_t<blend_arg_t>, cff2_cs_opset_flatten_t, flatten_param_t> interp (env);
	  flatten_param_t param = {
	    subset_charstrings.arrayZ[i],
	    (منطقی) (plan->flags & HB_SUBSET_FLAGS_NO_HINTING),
	    plan
	  };
	  param.instancer = &instancing;
	  اگر (unlikely (!interp.interpret (param)))
	    واپس جھوٹ;
	}
      }
      ورنہ
      {
      /* Flatten global & local subrs */
      subr_flattener_t<ثابت OT::cff2::accelerator_subset_t, cff2_cs_interp_env_t<blend_arg_t>, cff2_cs_opset_flatten_t>
		    flattener(acc, plan);

      /* Enable command capture if requested (for specialization) */
      اگر (capture_commands)
      {
	اگر (!charstring_commands.resize_exact (عدد_glyphs))
	  واپس جھوٹ;

	اگر (!flattener.flatten (subset_charstrings, &charstring_commands))
	  واپس جھوٹ;
      }
      ورنہ
      {
	اگر (!flattener.flatten (subset_charstrings))
	  واپس جھوٹ;
      }
      }
    }
    ورنہ
    {
      cff2_subr_subsetter_t	subr_subsetter (acc, plan);

      /* Subset subrs: collect used subroutines, leaving all unused ones behind */
      اگر (!subr_subsetter.subset ())
	واپس جھوٹ;

      /* encode charstrings, global subrs, local subrs with new subroutine numbers */
      اگر (!subr_subsetter.encode_charstrings (subset_charstrings, !pinned))
	واپس جھوٹ;

      اگر (!subr_subsetter.encode_globalsubrs (subset_globalsubrs))
	واپس جھوٹ;

      /* local subrs */
      اگر (!subset_localsubrs.resize (orig_fdcount))
	واپس جھوٹ;
      برائے (غیرعلامتی صحیح fd = 0; fd < orig_fdcount; fd++)
      {
	subset_localsubrs[fd].init ();
	اگر (!subr_subsetter.encode_localsubrs (fd, subset_localsubrs[fd]))
	  واپس جھوٹ;
      }
    }

    /* FDSelect */
    اگر (acc.fdSelect != &Null (CFF2FDSelect))
    {
      اگر (unlikely (!hb_plan_subset_cff_fdselect (plan,
						  orig_fdcount,
						  *(ثابت FDSelect *)acc.fdSelect,
						  acc.عدد_glyphs,
						  subset_fdcount,
						  subset_fdselect_سائز,
						  subset_fdselect_format,
						  subset_fdselect_ranges,
						  fdmap)))
	واپس جھوٹ;
    }
    ورنہ
      fdmap.شناختی_عنصر (1);

    واپس سچ;
  }

  cff2_sub_جدول_info_t info;

  غیرعلامتی صحیح    عدد_glyphs;
  غیرعلامتی صحیح    orig_fdcount = 0;
  غیرعلامتی صحیح    subset_fdcount = 1;
  غیرعلامتی صحیح    subset_fdselect_سائز = 0;
  غیرعلامتی صحیح    subset_fdselect_format = 0;
  منطقی            pinned = جھوٹ;
  hb_سمتیہ_t<رمز_pair_t>   subset_fdselect_ranges;

  hb_inc_bimap_t   fdmap;

  str_buff_vec_t	    subset_charstrings;
  str_buff_vec_t	    subset_globalsubrs;
  hb_سمتیہ_t<str_buff_vec_t> subset_localsubrs;

  منطقی	    drop_hints = جھوٹ;
  منطقی	    desubroutinize = جھوٹ;

  منطقی	    partial_instancing = جھوٹ;
  cff2_instancing_plan_t instancing;
  ثابت hb_سمتیہ_t<hb_لیبل_t> *axis_لیبل_فہرست = خالی_اشاریہ;

  غیرعلامتی  min_charstrings_off_سائز = 0;

  hb_array_t<صحیح> normalized_coords; // For instantiation
  سر_maxp_info_t سر_maxp_info;  // For FontBBox
  ثابت hb_hashmap_t<hb_codepoint_t, hb_pair_t<غیرعلامتی, صحیح>> *hmtx_map; // For widths

  // Width optimization results (for CFF1 conversion)
  غیرعلامتی default_چوڑائی = 0;
  غیرعلامتی nominal_چوڑائی = 0;

  // Command capture for specialization (CFF2→CFF1 conversion)
  منطقی capture_commands = جھوٹ;
  hb_سمتیہ_t<hb_سمتیہ_t<cs_command_t>> charstring_commands;
};
} // namespace OT

/*
 * CFF2 to CFF1 Converter Implementation
 */

#شامل "hb-cff-width-optimizer.hh"
#شامل "hb-cff-specializer.hh"

/* Serialize charstrings using CFF1 format with widths */
ساکن منطقی
_serialize_cff1_charstrings (hb_serialize_context_t *c,
                             OT::cff2_subset_plan &plan,
                             غیرعلامتی default_چوڑائی,
                             غیرعلامتی nominal_چوڑائی)
{
  c->push ();

  // CFF1 requires:
  // 1. Width at the beginning (if != defaultWidthX)
  // 2. endchar at the end
  str_buff_vec_t cff1_charstrings;
  اگر (unlikely (!cff1_charstrings.resize (plan.subset_charstrings.لمبائی)))
  {
    c->نکالو_discard ();
    واپس جھوٹ;
  }

  برائے (غیرعلامتی i = 0; i < plan.subset_charstrings.لمبائی; i++)
  {
    // Get width for this glyph from hmtx_map
    غیرعلامتی چوڑائی = 0;
    اگر (plan.hmtx_map->has (i))
      چوڑائی = plan.hmtx_map->get (i).پہلا;

    // Encode width if different from default
    str_encoder_t encoder (cff1_charstrings[i]);
    اگر (چوڑائی != default_چوڑائی)
    {
      صحیح delta = (صحیح) چوڑائی - (صحیح) nominal_چوڑائی;
      encoder.encode_int (delta);
    }

    // Use specialized commands if available, otherwise use binary
    اگر (plan.capture_commands && i < plan.charstring_commands.لمبائی &&
        plan.charstring_commands[i].لمبائی > 0)
    {
      // Specialize and encode commands
      خودکار &commands = plan.charstring_commands[i];
      CFF::specialize_commands (commands, 48);  /* maxstack=48 for CFF1 */
      اگر (unlikely (!CFF::encode_commands (commands, cff1_charstrings[i])))
      {
        c->نکالو_discard ();
        واپس جھوٹ;
      }
    }
    ورنہ
    {
      // Use binary CharString
      ثابت str_buff_t &cs = plan.subset_charstrings[i];
      برائے (غیرعلامتی j = 0; j < cs.لمبائی; j++)
        cff1_charstrings[i].push (cs[j]);
    }

    // Check if it already ends with endchar (0x0e) or return (0x0b)
    اگر (cff1_charstrings[i].لمبائی == 0 ||
        (cff1_charstrings[i].دم () != 0x0e && cff1_charstrings[i].دم () != 0x0b))
    {
      // Append endchar operator
      اگر (unlikely (!cff1_charstrings[i].ڈالو_or_fail (0x0e)))
      {
        c->نکالو_discard ();
        واپس جھوٹ;
      }
    }
  }

  غیرعلامتی ڈیٹا_سائز = 0;
  غیرعلامتی کل_سائز = CFF1_Char_لڑیاں::کل_سائز (cff1_charstrings, &ڈیٹا_سائز, plan.min_charstrings_off_سائز);
  اگر (unlikely (!c->آغاز_zerocopy (کل_سائز)))
  {
    c->نکالو_discard ();
    واپس جھوٹ;
  }

  خودکار *cs = c->آغاز_embed<CFF1_Char_لڑیاں> ();
  اگر (unlikely (!cs->serialize (c, cff1_charstrings, &ڈیٹا_سائز, plan.min_charstrings_off_سائز)))
  {
    c->نکالو_discard ();
    واپس جھوٹ;
  }

  plan.info.char_لڑیاں_link = c->نکالو_pack (جھوٹ);
  واپس سچ;
}

/* Serialize CID Charset (format 2 range: gid 0-N -> cid 0-N) */
ساکن منطقی
_serialize_cff1_charset (hb_serialize_context_t *c,
                         غیرعلامتی صحیح عدد_glyphs,
                         objidx_t &charset_link)
{
  // For CID fonts, create a simple identity charset
  // Format 2: one range covering all glyphs (except .notdef)
  c->push ();

  خودکار *charset = c->آغاز_embed<Charset> ();
  اگر (unlikely (!charset))
  {
    c->نکالو_discard ();
    واپس جھوٹ;
  }

  // Create a single range for CID 1 to num_glyphs-1
  hb_سمتیہ_t<رمز_pair_t> ranges;
  اگر (عدد_glyphs > 1)
  {
    رمز_pair_t range;
    range.رمز = 1;  // first CID
    range.glyph = عدد_glyphs - 2;  // nLeft (covers glyphs 1 to num_glyphs-1)
    ranges.push (range);
  }

  اگر (unlikely (!charset->serialize (c, 2, عدد_glyphs, ranges)))
  {
    c->نکالو_discard ();
    واپس جھوٹ;
  }

  charset_link = c->نکالو_pack ();
  واپس سچ;
}

/* CFF2 to CFF1 serialization */
نامخلا CFF {

منطقی
serialize_cff2_مقصد_cff1 (hb_serialize_context_t *c,
                        OT::cff2_subset_plan &plan,
                        ثابت cff2_چوٹی_dict_اقدار_t &cff2_چوٹی_Dict,
                        ثابت OT::cff2::accelerator_subset_t &acc)
{
  TRACE_SERIALIZE (یہ);

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
    hb_سمتیہ_t<غیرعلامتی> widths;
    widths.alloc (plan.عدد_glyphs);

    برائے (غیرعلامتی gid = 0; gid < plan.عدد_glyphs; gid++)
    {
      غیرعلامتی چوڑائی = 0;
      اگر (plan.hmtx_map->has (gid))
        چوڑائی = plan.hmtx_map->get (gid).پہلا;
      widths.push (چوڑائی);
    }

    // Optimize defaultWidthX and nominalWidthX
    CFF::optimize_widths (widths, plan.default_چوڑائی, plan.nominal_چوڑائی);
  }

  // 1. CharStrings (with widths prepended)
  اگر (!_serialize_cff1_charstrings (c, plan, plan.default_چوڑائی, plan.nominal_چوڑائی))
    return_trace (جھوٹ);

  // 2. Private DICs & Local Subrs (same as CFF2)
  hb_سمتیہ_t<جدول_info_t> نجی_dict_infos;
  اگر (unlikely (!نجی_dict_infos.resize (plan.subset_fdcount)))
    return_trace (جھوٹ);

  برائے (صحیح i = (صحیح)acc.نجی_Dicts.لمبائی; --i >= 0;)
  {
    اگر (plan.fdmap.has (i))
    {
      objidx_t subrs_link = 0;

      اگر (plan.subset_localsubrs[i].لمبائی > 0)
      {
        خودکار *dest = c->push<CFF1Subrs> ();
        اگر (likely (dest->serialize (c, plan.subset_localsubrs[i])))
          subrs_link = c->نکالو_pack (جھوٹ);
        ورنہ
        {
          c->نکالو_discard ();
          return_trace (جھوٹ);
        }
      }

      خودکار *pd = c->push<نجی_Dict> ();
      // Use the CFF2 Private DICT serializer which instantiates blends when pinned=true
      cff2_نجی_dict_op_serializer_t privSzr (plan.desubroutinize, plan.drop_hints, plan.pinned,
                                                  acc.varStore, plan.normalized_coords);
      اگر (likely (pd->serialize (c, acc.نجی_Dicts[i], privSzr, subrs_link)))
      {
        // Add defaultWidthX and nominalWidthX for CFF1
        str_buff_t چوڑائی_ops;
        str_encoder_t encoder (چوڑائی_ops);
        encoder.encode_int (plan.default_چوڑائی);
        encoder.encode_op (Op_رمز_default_چوڑائی_X);
        encoder.encode_int (plan.nominal_چوڑائی);
        encoder.encode_op (Op_رمز_nominal_چوڑائی_X);

        اگر (!encoder.اندر_error () && c->embed (چوڑائی_ops.as_بائٹیں ().arrayZ, چوڑائی_ops.لمبائی))
        {
          غیرعلامتی fd = plan.fdmap[i];
          نجی_dict_infos[fd].سائز = c->لمبائی ();
          نجی_dict_infos[fd].link = c->نکالو_pack ();
        }
        ورنہ
        {
          c->نکالو_discard ();
          return_trace (جھوٹ);
        }
      }
      ورنہ
      {
        c->نکالو_discard ();
        return_trace (جھوٹ);
      }
    }
  }

  // 3. FDArray - serialize CFF2 font dicts as CFF1
  {
    خودکار *fda = c->push<FDArray<HBUINT16>> ();
    cff_font_dict_op_serializer_t fontSzr;
    خودکار it =
    + hb_zip (+ hb_iter (acc.fontDicts)
              | hb_filter ([&] (ثابت cff2_font_dict_اقدار_t &_)
                { واپس plan.fdmap.has (&_ - &acc.fontDicts[0]); }),
              hb_iter (نجی_dict_infos))
    ;
    // Explicitly specify template parameters: DICTVAL, INFO
    منطقی success = fda->serialize<cff2_font_dict_اقدار_t, جدول_info_t> (c, it, fontSzr);
    اگر (success)
      plan.info.fd_array_link = c->نکالو_pack (جھوٹ);
    ورنہ
    {
      c->نکالو_discard ();
      return_trace (جھوٹ);
    }
  }

  // 4. FDSelect (required in CFF1 CID-keyed fonts)
  // CFF1 requires FDSelect for all CID-keyed fonts, even with just one FD
  // CFF2 makes it optional when there's only one FD
  اگر (acc.fdSelect != &Null (CFF2FDSelect))
  {
    c->push ();
    اگر (likely (hb_serialize_cff_fdselect (c, plan.عدد_glyphs,
                                          *(ثابت FDSelect *)acc.fdSelect,
                                          plan.orig_fdcount,
                                          plan.subset_fdselect_format,
                                          plan.subset_fdselect_سائز,
                                          plan.subset_fdselect_ranges)))
      plan.info.fd_select.link = c->نکالو_pack ();
    ورنہ
    {
      c->نکالو_discard ();
      return_trace (جھوٹ);
    }
  }
  ورنہ
  {
    // Create a range-based FDSelect3 mapping all glyphs to FD 0
    // Format: format(1) + nRanges(2) + range(3) + sentinel(2) = 8 bytes
    c->push ();

    // Format byte
    HBUINT8 format;
    format = 3;
    اگر (unlikely (!c->embed (format)))
    {
      c->نکالو_discard ();
      return_trace (جھوٹ);
    }

    // nRanges
    HBUINT16 nRanges;
    nRanges = 1;
    اگر (unlikely (!c->embed (nRanges)))
    {
      c->نکالو_discard ();
      return_trace (جھوٹ);
    }

    // Single range: {first: 0, fd: 0}
    FDSelect3_Range range;
    range.پہلا = 0;
    range.fd = 0;
    اگر (unlikely (!c->embed (range)))
    {
      c->نکالو_discard ();
      return_trace (جھوٹ);
    }

    // Sentinel (number of glyphs)
    HBUINT16 sentinel;
    sentinel = plan.عدد_glyphs;
    اگر (unlikely (!c->embed (sentinel)))
    {
      c->نکالو_discard ();
      return_trace (جھوٹ);
    }

    plan.info.fd_select.link = c->نکالو_pack ();
  }

  // 5. Charset (CID charset for identity mapping)
  objidx_t charset_link;
  اگر (!_serialize_cff1_charset (c, plan.عدد_glyphs, charset_link))
    return_trace (جھوٹ);

  // 6. Global Subrs
  {
    خودکار *dest = c->push<CFF1Subrs> ();
    اگر (likely (dest->serialize (c, plan.subset_globalsubrs)))
      c->نکالو_pack (جھوٹ);
    ورنہ
    {
      c->نکالو_discard ();
      return_trace (جھوٹ);
    }
  }

  // 7. String INDEX - Add "Adobe" and "Identity" for ROS operator
  {
    ثابت حرف *adobe_str = "Adobe";
    ثابت حرف *شناختی_عنصر_str = "Identity";
    غیرعلامتی adobe_len = 5;  // strlen("Adobe")
    غیرعلامتی شناختی_عنصر_len = 8;  // strlen("Identity")

    // Build strings array
    hb_سمتیہ_t<hb_ubytes_t> لڑیاں;
    لڑیاں.alloc (2);
    لڑیاں.push (hb_ubytes_t ((ثابت غیرعلامتی حرف *) adobe_str, adobe_len));
    لڑیاں.push (hb_ubytes_t ((ثابت غیرعلامتی حرف *) شناختی_عنصر_str, شناختی_عنصر_len));

    // Serialize as CFF INDEX
    خودکار *dest = c->push<CFF1_اشاریہ> ();
    اگر (likely (dest->serialize (c, لڑیاں)))
      c->نکالو_pack (جھوٹ);
    ورنہ
    {
      c->نکالو_discard ();
      return_trace (جھوٹ);
    }
  }

  // 8. CFF Header
  OT::cff1 *cff = c->allocate_min<OT::cff1> ();
  اگر (unlikely (!cff)) return_trace (جھوٹ);

  /*سرخی*/
  cff->نسخہ.major = 0x01;
  cff->نسخہ.minor = 0x00;
  cff->name_اشاریہ = cff->min_سائز;
  cff->off_سائز = 4; /* unused? */

  // 9. Name INDEX (single entry)
  {
    غیرعلامتی name_len = لڑی_لمبائی (CFF1_DEFAULT_FONT_NAME);

    CFF1_اشاریہ *idx = c->آغاز_embed<CFF1_اشاریہ> ();
    اگر (unlikely (!idx)) return_trace (جھوٹ);

    اگر (unlikely (!idx->serialize_سرخی (c, hb_iter (&name_len, 1), name_len)))
      return_trace (جھوٹ);

    اگر (unlikely (!c->embed (CFF1_DEFAULT_FONT_NAME, name_len)))
      return_trace (جھوٹ);
  }

  // 10. Top DICT INDEX
  {
    // Serialize the Top DICT data first
    c->push<چوٹی_Dict> ();
    cff1_from_cff2_چوٹی_dict_op_serializer_t چوٹی_Szr;

    // Serialize ROS first
    اگر (unlikely (!چوٹی_Szr.serialize_ros (c)))
    {
      c->نکالو_discard ();
      return_trace (جھوٹ);
    }

    // Serialize FontBBox from head table
    {
      str_buff_t bbox_buff;
      str_encoder_t encoder (bbox_buff);

      encoder.encode_int (plan.سر_maxp_info.xMin);
      encoder.encode_int (plan.سر_maxp_info.yMin);
      encoder.encode_int (plan.سر_maxp_info.xMax);
      encoder.encode_int (plan.سر_maxp_info.yMax);
      encoder.encode_op (Op_رمز_Font_BBox);

      اگر (encoder.اندر_error () || !c->embed (bbox_buff.as_بائٹیں ().arrayZ, bbox_buff.لمبائی))
      {
        c->نکالو_discard ();
        return_trace (جھوٹ);
      }
    }

    // Serialize charset operator
    اگر (charset_link && unlikely (!FontDict::serialize_link4_op (c, Op_رمز_charset, charset_link, whence_t::Absolute)))
    {
      c->نکالو_discard ();
      return_trace (جھوٹ);
    }

    // Serialize FDSelect operator (required for CID-keyed CFF1 fonts)
    اگر (plan.info.fd_select.link && unlikely (!FontDict::serialize_link4_op (c, Op_رمز_FDSelect, plan.info.fd_select.link, whence_t::Absolute)))
    {
      c->نکالو_discard ();
      return_trace (جھوٹ);
    }

    // Serialize FDArray operator (required for CID-keyed CFF1 fonts)
    اگر (plan.info.fd_array_link && unlikely (!FontDict::serialize_link4_op (c, Op_رمز_FDArray, plan.info.fd_array_link, whence_t::Absolute)))
    {
      c->نکالو_discard ();
      return_trace (جھوٹ);
    }

    // Serialize other operators from CFF2 TopDict
    برائے (ثابت خودکار &opstr : cff2_چوٹی_Dict.values)
    {
      اگر (unlikely (!چوٹی_Szr.serialize (c, opstr, plan.info)))
      {
        c->نکالو_discard ();
        return_trace (جھوٹ);
      }
    }

    غیرعلامتی چوٹی_سائز = c->لمبائی ();
    c->نکالو_pack (جھوٹ);

    // Serialize INDEX header
    خودکار *dest = c->آغاز_embed<CFF1_اشاریہ> ();
    اگر (unlikely (!dest->serialize_سرخی (c, hb_iter (&چوٹی_سائز, 1), چوٹی_سائز)))
      return_trace (جھوٹ);
  }

  return_trace (سچ);
}

} /* namespace CFF */

ساکن منطقی _serialize_cff2_charstrings (hb_serialize_context_t *c,
			     cff2_subset_plan &plan,
			     ثابت OT::cff2::accelerator_subset_t  &acc)
{
  c->push ();

  غیرعلامتی ڈیٹا_سائز = 0;
  غیرعلامتی کل_سائز = CFF2_Char_لڑیاں::کل_سائز (plan.subset_charstrings, &ڈیٹا_سائز, plan.min_charstrings_off_سائز);
  اگر (unlikely (!c->آغاز_zerocopy (کل_سائز)))
    واپس جھوٹ;

  خودکار *cs = c->آغاز_embed<CFF2_Char_لڑیاں> ();
  اگر (unlikely (!cs->serialize (c, plan.subset_charstrings, &ڈیٹا_سائز, plan.min_charstrings_off_سائز)))
  {
    c->نکالو_discard ();
    واپس جھوٹ;
  }

  plan.info.char_لڑیاں_link = c->نکالو_pack (جھوٹ);
  واپس سچ;
}

منطقی
OT::cff2::accelerator_subset_t::serialize (hb_serialize_context_t *c,
					   ساخت cff2_subset_plan &plan,
					   hb_array_t<صحیح> normalized_coords) ثابت
{
  /* push charstrings onto the object stack first which will ensure it packs as the last
     object in the table. Keeping the chastrings last satisfies the requirements for patching
     via IFTB. If this ordering needs to be changed in the future, charstrings should be left
     at the end whenever HB_SUBSET_FLAGS_ITFB_REQUIREMENTS is enabled. */
  اگر (!_serialize_cff2_charstrings(c, plan, *یہ))
    واپس جھوٹ;

  /* private dicts & local subrs */
  hb_سمتیہ_t<جدول_info_t>  نجی_dict_infos;
  اگر (unlikely (!نجی_dict_infos.resize (plan.subset_fdcount))) واپس جھوٹ;

  برائے (صحیح i = (صحیح)نجی_Dicts.لمبائی; --i >= 0 ;)
  {
    اگر (plan.fdmap.has (i))
    {
      objidx_t	subrs_link = 0;

      اگر (plan.subset_localsubrs[i].لمبائی > 0)
      {
	خودکار *dest = c->push <CFF2Subrs> ();
	اگر (likely (dest->serialize (c, plan.subset_localsubrs[i])))
	  subrs_link = c->نکالو_pack (جھوٹ);
	ورنہ
	{
	  c->نکالو_discard ();
	  واپس جھوٹ;
	}
      }
      خودکار *pd = c->push<نجی_Dict> ();
      cff2_نجی_dict_op_serializer_t privSzr (plan.desubroutinize, plan.drop_hints, plan.pinned,
						 varStore, normalized_coords,
						 plan.partial_instancing ? &plan.instancing : خالی_اشاریہ);
      اگر (likely (pd->serialize (c, نجی_Dicts[i], privSzr, subrs_link)))
      {
	غیرعلامتی fd = plan.fdmap[i];
	نجی_dict_infos[fd].سائز = c->لمبائی ();
	نجی_dict_infos[fd].link = c->نکالو_pack ();
      }
      ورنہ
      {
	c->نکالو_discard ();
	واپس جھوٹ;
      }
    }
  }

  /* FDSelect */
  اگر (fdSelect != &Null (CFF2FDSelect))
  {
    c->push ();
    اگر (likely (hb_serialize_cff_fdselect (c, plan.عدد_glyphs, *(ثابت FDSelect *)fdSelect,
					   plan.orig_fdcount,
					   plan.subset_fdselect_format, plan.subset_fdselect_سائز,
					   plan.subset_fdselect_ranges)))
      plan.info.fd_select.link = c->نکالو_pack ();
    ورنہ
    {
      c->نکالو_discard ();
      واپس جھوٹ;
    }
  }

  /* FDArray (FD Index) */
  {
    خودکار *fda = c->push<CFF2FDArray> ();
    cff_font_dict_op_serializer_t fontSzr;
    خودکار it =
    + hb_zip (+ hb_iter (fontDicts)
	      | hb_filter ([&] (ثابت cff2_font_dict_اقدار_t &_)
		{ واپس plan.fdmap.has (&_ - &fontDicts[0]); }),
	      hb_iter (نجی_dict_infos))
    ;
    اگر (unlikely (!fda->serialize (c, it, fontSzr)))
    {
      c->نکالو_discard ();
      واپس جھوٹ;
    }
    plan.info.fd_array_link = c->نکالو_pack (جھوٹ);
  }

  /* variation store */
  اگر (varStore != &Null (CFF2_شے_Variation_Store) &&
      !plan.pinned)
  {
    اگر (plan.partial_instancing)
    {
      اگر (plan.instancing.has_variations ())
      {
	c->push ();
	اگر (unlikely (!plan.instancing.serialize_var_store (c, *plan.axis_لیبل_فہرست)))
	{
	  c->نکالو_discard ();
	  واپس جھوٹ;
	}
	plan.info.var_store_link = c->نکالو_pack (جھوٹ);
      }
      /* else: the store emptied; the vstore op gets dropped along with it. */
    }
    ورنہ
    {
      خودکار *dest = c->push<CFF2_شے_Variation_Store> ();
      اگر (unlikely (!dest->serialize (c, varStore)))
      {
	c->نکالو_discard ();
	واپس جھوٹ;
      }
      plan.info.var_store_link = c->نکالو_pack (جھوٹ);
    }
  }

  OT::cff2 *cff2 = c->allocate_min<OT::cff2> ();
  اگر (unlikely (!cff2)) واپس جھوٹ;

  /*سرخی*/
  cff2->نسخہ.major = 0x02;
  cff2->نسخہ.minor = 0x00;
  cff2->چوٹی_Dict_2 = OT::cff2::static_سائز;

  /* top dict */
  {
    چوٹی_Dict &dict = cff2 + cff2->چوٹی_Dict_2;
    cff2_چوٹی_dict_op_serializer_t چوٹی_Szr;
    اگر (unlikely (!dict.serialize (c, چوٹی_Dict_2, چوٹی_Szr, plan.info))) واپس جھوٹ;
    cff2->چوٹی_Dict_سائز = c->سر_2 - (ثابت حرف *)&dict;
  }

  /* global subrs */
  {
    خودکار *dest = c->آغاز_embed <CFF2Subrs> ();
    واپس dest->serialize (c, plan.subset_globalsubrs);
  }
}

منطقی
OT::cff2::accelerator_subset_t::subset (hb_subset_context_t *c) ثابت
{
  cff2_subset_plan cff2_plan;

  اگر (unlikely (!cff2_plan.create (*یہ, c->plan))) واپس جھوٹ;

  // If instantiating (pinned) and downgrade flag is set, convert to CFF1
  اگر (cff2_plan.pinned && (c->plan->flags & HB_SUBSET_FLAGS_DOWNGRADE_CFF2))
  {
    // Serialize CFF1 to the subsetter's serializer
    // If we run out of room, returning true will cause subsetter to retry with larger buffer
    منطقی نتیجہ = CFF::serialize_cff2_مقصد_cff1 (c->serializer, cff2_plan, چوٹی_Dict_2, *یہ);

    اگر (c->serializer->ran_برآمد_کا_گنجائش ())
      واپس سچ; // Subsetter will retry with larger buffer

    اگر (نتیجہ && !c->serializer->اندر_error ())
    {
      // Success - end serialization to resolve links
      c->serializer->انتہا_serialize ();

      // Copy the serialized CFF1 data and add as CFF table
      hb_blob_t *cff_blob = c->serializer->copy_blob ();
      اگر (cff_blob)
      {
        c->plan->جوڑو_جدول (HB_لیبل('C','F','F',' '), cff_blob);
        hb_blob_destroy (cff_blob);

        // Return false to signal CFF2 table is not needed
        واپس جھوٹ;
      }
    }

    // Conversion failed - don't fall back, fail hard for debugging
    واپس جھوٹ;
  }

  واپس serialize (c->serializer, cff2_plan,
		    c->plan->normalized_coords.as_array ());
}

#اگر_ختم
