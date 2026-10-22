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

#ضمن "hb.hh"

#جے_بنا_تعریف HB_NO_SUBSET_CFF

#ضمن "hb-open-type.hh"
#ضمن "hb-ot-cff2-table.hh"
#ضمن "hb-ot-var-common.hh"
#ضمن "hb-set.h"
#ضمن "hb-subset-plan.hh"
#ضمن "hb-subset-cff-common.hh"
#ضمن "hb-cff2-interp-cs.hh"
#ضمن "hb-subset-cff2-to-cff1.hh"

ورتوں نطاق CFF;

نطاق CFF {

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
بنية cff2_instancing_plan_t
{
  بنية vardata_transform_t
  {
    /* new_deltas[j] = Σ_i matrix[j][i] * old_deltas[i] */
    hb_سمتیہ_t<hb_سمتیہ_t<عائم>> میٹرکس;
    /* Default-value fold-in scalars, one per old region: the old store's
     * region scalars at the new default location. */
    hb_سمتیہ_t<عائم> gains;
    /* Surviving regions, parallel to matrix rows. */
    hb_سمتیہ_t<hb_hashmap_t<hb_ٹیگ_t, Triple>> regions;
    /* Index into the new global region list, per new region (finalize). */
    hb_سمتیہ_t<بنانشانی> region_indices;
    بنانشانی new_major = 0;
    منطقي alive_ = خطأ;
    منطقي alive () ثابت { أرجع alive_; }

    /* One value's delta for new region j, from its old per-region deltas. */
    مزدوج transform_delta (بنانشانی j, hb_array_t<ثابت نمبر_t> deltas) ثابت
    {
      ثابت hb_سمتیہ_t<عائم> &خانہ = میٹرکس.arrayZ[j];
      مزدوج d = 0.;
      بنانشانی گنتی = hb_min (خانہ.لمائی, deltas.لمائی);
      لكل (بنانشانی c = 0; c < گنتی; c++)
	d += (مزدوج) خانہ.arrayZ[c] * deltas.arrayZ[c].منزل_real ();
      أرجع d;
    }
  };

  hb_سمتیہ_t<vardata_transform_t> transforms;  /* per old VarData (vsindex) */
  hb_سمتیہ_t<hb_hashmap_t<hb_ٹیگ_t, Triple>> new_regions;  /* global region list */
  hb_سیٹ_t ورتیا_majors;
  بنانشانی new_major_گنتی = 0;

  منطقي has_variations () ثابت { أرجع new_major_گنتی; }

  ثابت vardata_transform_t *get_transform (بنانشانی major) ثابت
  { أرجع major < transforms.لمائی ? &transforms.arrayZ[major] : خالی_سوچک; }

  /* Record that a retained blend references this VarData. */
  فراغ note_use (بنانشانی major) { ورتیا_majors.جوڑو (major); }

  /* The new vsindex a private dict's (or charstring's) old ivs maps to;
   * dead VarDatas fall back to 0, their blends having become static. */
  بنانشانی remap_ivs (بنانشانی old_ivs) ثابت
  {
    ثابت vardata_transform_t *t = get_transform (old_ivs);
    أرجع (t && t->alive ()) ? t->new_major : 0;
  }

  مزدوج fold_deltas (بنانشانی old_ivs, hb_array_t<ثابت نمبر_t> deltas) ثابت
  {
    ثابت vardata_transform_t *t = get_transform (old_ivs);
    إذا (unlikely (!t || t->gains.لمائی != deltas.لمائی)) أرجع 0.;
    مزدوج v = 0.;
    لكل (بنانشانی i = 0; i < deltas.لمائی; i++)
      v += (مزدوج) t->gains.arrayZ[i] * deltas.arrayZ[i].منزل_real ();
    أرجع v;
  }

  منطقي create (ثابت CFF2_شے_Variation_Store *cff2VarStore,
	       ثابت hb_subset_plan_t *plan)
  {
    ثابت OT::شے_Variation_Store &varStore = cff2VarStore->varStore;
    بنانشانی major_گنتی = varStore.get_sub_جدول_گنتی ();
    إذا (!transforms.resize (major_گنتی)) أرجع خطأ;

    إذا (!varStore.get_region_list ().get_var_regions (plan->axes_old_اشاریہ_ٹیگ_map, orig_regions))
      أرجع خطأ;

    /* Instance each VarData with identity delta rows; the surviving tuples'
     * delta columns are the old-region → new-region transform. */
    OT::optimize_scratch_t scratch;
    لكل (بنانشانی m = 0; m < major_گنتی; m++)
    {
      ثابت OT::Var_انکڑے &var_انکڑے = varStore.get_sub_جدول (m);
      بنانشانی k = var_انکڑے.get_region_اشاریہ_گنتی ();
      vardata_transform_t &t = transforms.arrayZ[m];

      إذا (!t.gains.resize_exact (k)) أرجع خطأ;
      varStore.get_region_scalars (m, plan->normalized_coords.arrayZ,
				   plan->normalized_coords.لمائی,
				   t.gains.arrayZ, k);

      OT::tuple_variations_t tuples;
      لكل (بنانشانی r = 0; r < k; r++)
      {
	OT::tuple_delta_t tuple;
	إذا (!tuple.deltas_x.resize (k) ||
	    !tuple.indices.resize (k))
	  أرجع خطأ;
	لكل (بنانشانی i = 0; i < k; i++)
	  tuple.indices.arrayZ[i] = سچ;
	tuple.deltas_x.arrayZ[r] = 1.f;
	بنانشانی region_اشاریہ = var_انکڑے.get_region_اشاریہ (r);
	إذا (unlikely (region_اشاریہ >= orig_regions.لمائی)) أرجع خطأ;
	tuple.axis_tuples = orig_regions.arrayZ[region_اشاریہ];
	إذا (unlikely (tuple.axis_tuples.وچ_error ())) أرجع خطأ;
	tuples.tuple_vars.push (std::move (tuple));
      }
      إذا (unlikely (tuples.tuple_vars.وچ_error ())) أرجع خطأ;

      إذا (!tuples.instantiate (plan->axes_location, plan->axes_triple_distances, scratch))
	أرجع خطأ;

      لكل (خودکار &tuple : tuples.tuple_vars)
      {
	إذا (unlikely (tuple.deltas_x.لمائی != k)) أرجع خطأ;
	hb_سمتیہ_t<عائم> خانہ;
	إذا (unlikely (!خانہ.resize_exact (k))) أرجع خطأ;
	لكل (بنانشانی i = 0; i < k; i++)
	  خانہ.arrayZ[i] = tuple.deltas_x.arrayZ[i];
	t.میٹرکس.push (std::move (خانہ));
	t.regions.push (std::move (tuple.axis_tuples));
      }
      إذا (unlikely (t.میٹرکس.وچ_error () || t.regions.وچ_error ())) أرجع خطأ;
    }

    أرجع سچ;
  }

  /* Decide which VarDatas survive, number them, and build the new global
   * region list: surviving original regions first, in their original order,
   * then solver-split new regions in order of appearance (fontTools'
   * rebuildRegions + prune_regions). */
  منطقي finalize ()
  {
    لكل (بنانشانی m = 0; m < transforms.لمائی; m++)
    {
      vardata_transform_t &t = transforms.arrayZ[m];
      t.alive_ = t.میٹرکس.لمائی && ورتیا_majors.has (m);
      إذا (t.alive_)
	t.new_major = new_major_گنتی++;
    }

    /* Keys hash by content, so pointers into different owners compare fine. */
    hb_hashmap_t<ثابت hb_hashmap_t<hb_ٹیگ_t, Triple>*, بنانشانی> region_idx_map;
    hb_سمتیہ_t<ثابت hb_hashmap_t<hb_ٹیگ_t, Triple>*> region_ptrs;
    لكل (ثابت vardata_transform_t &t : transforms)
    {
      إذا (!t.alive ()) استمر;
      لكل (ثابت خودکار &region : t.regions)
	إذا (!region_idx_map.has (&region))
	  إذا (unlikely (!region_idx_map.سیٹ (&region, (بنانشانی) -1)))
	    أرجع خطأ;
    }
    لكل (ثابت خودکار &r : orig_regions)
    {
      بنانشانی *idx;
      إذا (region_idx_map.has (&r, &idx) && *idx == (بنانشانی) -1)
      {
	إذا (unlikely (!region_idx_map.سیٹ (&r, region_ptrs.لمائی))) أرجع خطأ;
	region_ptrs.push (&r);
      }
    }
    لكل (ثابت vardata_transform_t &t : transforms)
    {
      إذا (!t.alive ()) استمر;
      لكل (ثابت خودکار &region : t.regions)
      {
	بنانشانی *idx;
	إذا (region_idx_map.has (&region, &idx) && *idx == (بنانشانی) -1)
	{
	  إذا (unlikely (!region_idx_map.سیٹ (&region, region_ptrs.لمائی))) أرجع خطأ;
	  region_ptrs.push (&region);
	}
      }
    }
    إذا (unlikely (region_ptrs.وچ_error ())) أرجع خطأ;

    لكل (vardata_transform_t &t : transforms)
    {
      إذا (!t.alive ()) استمر;
      لكل (ثابت خودکار &region : t.regions)
      {
	بنانشانی *idx;
	إذا (unlikely (!region_idx_map.has (&region, &idx))) أرجع خطأ;
	t.region_indices.push (*idx);
      }
      إذا (unlikely (t.region_indices.وچ_error ())) أرجع خطأ;
    }

    إذا (!new_regions.resize (region_ptrs.لمائی)) أرجع خطأ;
    لكل (بنانشانی i = 0; i < region_ptrs.لمائی; i++)
    {
      new_regions.arrayZ[i] = *region_ptrs.arrayZ[i];
      إذا (unlikely (new_regions.arrayZ[i].وچ_error ())) أرجع خطأ;
    }

    أرجع سچ;
  }

  /* Serialize the instanced store: a CFF2ItemVariationStore whose VarDatas
   * carry no items, just region indexes (blend deltas live in charstrings).
   * Laid out manually since all sizes are known up front. */
  منطقي serialize_var_store (hb_serialize_context_t *c,
			    ثابت hb_سمتیہ_t<hb_ٹیگ_t> &axis_ٹیگ_سوچی) ثابت
  {
    بنانشانی انکڑے_گنتی = new_major_گنتی;
    بنانشانی سرخی_ناپ = 2 + 4 + 2 + 4 * انکڑے_گنتی;

    uint64_t وتھ = سرخی_ناپ;
    hb_سمتیہ_t<بنانشانی> انکڑے_offsets;
    لكل (ثابت vardata_transform_t &t : transforms)
    {
      إذا (!t.alive ()) استمر;
      انکڑے_offsets.push ((بنانشانی) وتھ);
      وتھ += 6 + 2 * (uint64_t) t.region_indices.لمائی;
    }
    إذا (unlikely (انکڑے_offsets.وچ_error () || انکڑے_offsets.لمائی != انکڑے_گنتی))
      أرجع خطأ;
    بنانشانی regions_وتھ = (بنانشانی) وتھ;
    uint64_t کل = وتھ + 4 + (uint64_t) new_regions.لمائی * axis_ٹیگ_سوچی.لمائی * 6;
    إذا (unlikely (کل > 0xFFFFFFFFu)) أرجع خطأ;

    HBUINT16 *ناپ_میدان = c->allocate_ناپ<HBUINT16> (HBUINT16::static_ناپ);
    إذا (unlikely (!ناپ_میدان)) أرجع خطأ;
    /* OFF 2026 5.3.3.11.2 permits larger stores with length=65535. */
    *ناپ_میدان = hb_min (کل, (uint64_t) 0xFFFFu);

    حرف *سرخی = c->allocate_ناپ<حرف> (سرخی_ناپ);
    إذا (unlikely (!سرخی)) أرجع خطأ;
    * (HBUINT16 *) (سرخی + 0) = 1;			/* format */
    * (HBUINT32 *) (سرخی + 2) = regions_وتھ;
    * (HBUINT16 *) (سرخی + 6) = انکڑے_گنتی;
    لكل (بنانشانی i = 0; i < انکڑے_گنتی; i++)
      * (HBUINT32 *) (سرخی + 8 + 4 * i) = انکڑے_offsets.arrayZ[i];

    لكل (ثابت vardata_transform_t &t : transforms)
    {
      إذا (!t.alive ()) استمر;
      بنانشانی k = t.region_indices.لمائی;
      HBUINT16 *v = (HBUINT16 *) c->allocate_ناپ<حرف> (6 + 2 * k);
      إذا (unlikely (!v)) أرجع خطأ;
      v[0] = 0;						/* itemCount */
      v[1] = 0;						/* wordSizeCount */
      v[2] = k;						/* regionIndices.len */
      لكل (بنانشانی j = 0; j < k; j++)
	v[3 + j] = t.region_indices.arrayZ[j];
    }

    hb_سمتیہ_t<ثابت hb_hashmap_t<hb_ٹیگ_t, Triple>*> region_ptrs;
    إذا (unlikely (!region_ptrs.alloc_exact (new_regions.لمائی))) أرجع خطأ;
    لكل (ثابت خودکار &r : new_regions)
      region_ptrs.push (&r);
    خودکار *region_list = c->شروع_embed<OT::VarRegionList> ();
    أرجع region_list->serialize (c, axis_ٹیگ_سوچی, region_ptrs);
  }

  خاص:
  hb_سمتیہ_t<hb_hashmap_t<hb_ٹیگ_t, Triple>> orig_regions;
};

/* Usage-collection pass for partial instancing: record which VarDatas the
 * retained charstrings' and private dicts' blends reference.  Mirrors the
 * emission pass: blends consumed by dropped hint ops don't count. */
بنية cff2_usage_param_t
{
  فراغ init () {}

  cff2_instancing_plan_t *instancing = خالی_سوچک;
  منطقي drop_hints = خطأ;
  بنانشانی ivs = 0;	/* dict-side current vsindex */
};

بنية cff2_cs_opset_usage_t : cff2_cs_opset_t<cff2_cs_opset_usage_t, cff2_usage_param_t, blend_arg_t>
{
  ساكن فراغ flush_دلیلاں_and_op (op_کوڈ_t op, cff2_cs_interp_env_t<blend_arg_t> &env, cff2_usage_param_t& param)
  {
    منطقي counts = سچ;
    بدل (op)
    {
      حالة Op_کوڈ_return:
      حالة Op_کوڈ_endchar:
	counts = خطأ;
	اكسر;

      حالة Op_کوڈ_hstem:
      حالة Op_کوڈ_hstemhm:
      حالة Op_کوڈ_vstem:
      حالة Op_کوڈ_vstemhm:
      حالة Op_کوڈ_hintmask:
      حالة Op_کوڈ_cntrmask:
	إذا (param.drop_hints)
	  counts = خطأ;
	اكسر;

      افتراضي:
	اكسر;
    }

    إذا (counts)
      لكل (بنانشانی صحيح i = 0; i < env.arg_ڈھیر.get_گنتی (); i++)
	إذا (env.arg_ڈھیر[i].blending ())
	{
	  param.instancing->note_use (env.get_ivs ());
	  اكسر;
	}

    env.clear_دلیلاں ();
  }
};

بنية cff2_نجی_dict_usage_opset_t : dict_opset_t
{
  ساكن فراغ process_op (op_کوڈ_t op, cff2_priv_dict_interp_env_t& env, cff2_usage_param_t& param)
  {
    بدل (op) {
      حالة Op_کوڈ_vsindexdict:
	env.process_vsindex ();
	param.ivs = env.get_ivs ();
	env.clear_دلیلاں ();
	أرجع;
      حالة Op_کوڈ_blenddict:
	param.instancing->note_use (param.ivs);
	env.clear_دلیلاں ();
	أرجع;
      افتراضي:
	dict_opset_t::process_op (op, env);
	إذا (!env.arg_ڈھیر.is_empty ()) أرجع;
	env.clear_دلیلاں ();
	أرجع;
    }
  }
};

} /* namespace CFF */

بنية cff2_sub_جدول_info_t : cff_sub_جدول_info_t
{
  cff2_sub_جدول_info_t ()
    : cff_sub_جدول_info_t (),
      var_store_link (0)
  {}

  objidx_t  var_store_link;
};

بنية cff2_چوٹی_dict_op_serializer_t : cff_چوٹی_dict_op_serializer_t<>
{
  منطقي serialize (hb_serialize_context_t *c,
		  ثابت op_str_t &opstr,
		  ثابت cff2_sub_جدول_info_t &info) ثابت
  {
    TRACE_SERIALIZE (هذا);

    بدل (opstr.op)
    {
      حالة Op_کوڈ_vstore:
        إذا (info.var_store_link)
	  return_trace (FontDict::serialize_link4_op(c, opstr.op, info.var_store_link));
	وإلا
	  return_trace (سچ);

      افتراضي:
	return_trace (cff_چوٹی_dict_op_serializer_t<>::serialize (c, opstr, info));
    }
  }
};

بنية cff2_cs_opset_flatten_t : cff2_cs_opset_t<cff2_cs_opset_flatten_t, flatten_param_t, blend_arg_t>
{
  ساكن فراغ flush_دلیلاں_and_op (op_کوڈ_t op, cff2_cs_interp_env_t<blend_arg_t> &env, flatten_param_t& param)
  {
    /* Optionally capture command for specialization (before flushing, to preserve args) */
    إذا (param.commands)
    {
      منطقي skip_command = خطأ;

      بدل (op)
      {
	حالة Op_کوڈ_return:
	حالة Op_کوڈ_endchar:
	  skip_command = سچ;
	  اكسر;

	حالة Op_کوڈ_hstem:
	حالة Op_کوڈ_hstemhm:
	حالة Op_کوڈ_vstem:
	حالة Op_کوڈ_vstemhm:
	حالة Op_کوڈ_hintmask:
	حالة Op_کوڈ_cntrmask:
	  إذا (param.drop_hints)
	    skip_command = سچ;
	  اكسر;

	افتراضي:
	  اكسر;
      }

      إذا (!skip_command)
      {
	cs_command_t cmd (op);
	/* Capture resolved blend values */
	لكل (بنانشانی صحيح i = 0; i < env.arg_ڈھیر.get_گنتی ();)
	{
	  ثابت blend_arg_t &arg = env.arg_ڈھیر[i];
	  إذا (arg.blending ())
	  {
	    /* For blend args, capture only the resolved default value */
	    cmd.دلیلاں.push (arg);
	    /* Skip over the multiple blend values */
	    i += arg.عدد_اقدار_2;
	  }
	  وإلا
	  {
	    cmd.دلیلاں.push (arg);
	    i++;
	  }
	}
	param.commands->push (cmd);
      }
    }

    بدل (op)
    {
      حالة Op_کوڈ_return:
      حالة Op_کوڈ_endchar:
	/* dummy opcodes in CFF2. ignore */
	إذا (op == Op_کوڈ_endchar && param.instancer)
	  emit_vsindex_اگیتر (env, param);
	اكسر;

      حالة Op_کوڈ_hstem:
      حالة Op_کوڈ_hstemhm:
      حالة Op_کوڈ_vstem:
      حالة Op_کوڈ_vstemhm:
      حالة Op_کوڈ_hintmask:
      حالة Op_کوڈ_cntrmask:
	إذا (param.drop_hints)
	{
	  env.clear_دلیلاں ();
	  أرجع;
	}
	HB_FALLTHROUGH;

      افتراضي:
	SUPER::flush_دلیلاں_and_op (op, env, param);
	اكسر;
    }
  }

  ساكن فراغ flush_دلیلاں (cff2_cs_interp_env_t<blend_arg_t> &env, flatten_param_t& param)
  {
    لكل (بنانشانی صحيح i = 0; i < env.arg_ڈھیر.get_گنتی ();)
    {
      ثابت blend_arg_t &arg = env.arg_ڈھیر[i];
      إذا (arg.blending ())
      {
	إذا (unlikely (!((arg.عدد_اقدار_2 > 0) && (env.arg_ڈھیر.get_گنتی () >= arg.عدد_اقدار_2))))
	{
	  env.سیٹ_error ();
	  أرجع;
	}
	إذا (param.instancer)
	  rewrite_blends (arg, i, env, param);
	وإلا
	  flatten_blends (arg, i, env, param);
	i += arg.عدد_اقدار_2;
      }
      وإلا
      {
	str_encoder_t  encoder (param.flatStr);
	encoder.encode_عدد_cs (arg);
	i++;
      }
    }
    SUPER::flush_دلیلاں (env, param);
  }

  ساكن فراغ flatten_blends (ثابت blend_arg_t &arg, بنانشانی صحيح i, cff2_cs_interp_env_t<blend_arg_t> &env, flatten_param_t& param)
  {
    /* flatten the default values */
    str_encoder_t  encoder (param.flatStr);
    لكل (بنانشانی صحيح j = 0; j < arg.عدد_اقدار_2; j++)
    {
      ثابت blend_arg_t &arg1 = env.arg_ڈھیر[i + j];
      إذا (unlikely (!((arg1.blending () && (arg.عدد_اقدار_2 == arg1.عدد_اقدار_2) && (arg1.قدر_اشاریہ_2 == j) &&
	      (arg1.deltas.لمائی == env.get_region_گنتی ())))))
      {
	env.سیٹ_error ();
	أرجع;
      }
      encoder.encode_عدد_cs (arg1);
    }
    /* flatten deltas for each value */
    لكل (بنانشانی صحيح j = 0; j < arg.عدد_اقدار_2; j++)
    {
      ثابت blend_arg_t &arg1 = env.arg_ڈھیر[i + j];
      لكل (بنانشانی صحيح k = 0; k < arg1.deltas.لمائی; k++)
	encoder.encode_عدد_cs (arg1.deltas[k]);
    }
    /* flatten the number of values followed by blend operator */
    encoder.encode_int (arg.عدد_اقدار_2);
    encoder.encode_op (Op_کوڈ_blendcs);
  }

  /* Partial instancing: rewrite one blend group against the instanced store.
   * Each value's default absorbs the folded (pinned-away) contribution, and
   * its deltas are mapped onto the new region list. */
  ساكن فراغ rewrite_blends (ثابت blend_arg_t &arg, بنانشانی صحيح i, cff2_cs_interp_env_t<blend_arg_t> &env, flatten_param_t& param)
  {
    ثابت cff2_instancing_plan_t &instancer = *param.instancer;
    بنانشانی ivs = env.get_ivs ();
    ثابت cff2_instancing_plan_t::vardata_transform_t *t = instancer.get_transform (ivs);
    إذا (unlikely (!t))
    {
      env.سیٹ_error ();
      أرجع;
    }

    بنانشانی n = arg.عدد_اقدار_2;
    لكل (بنانشانی صحيح j = 0; j < n; j++)
    {
      ثابت blend_arg_t &arg1 = env.arg_ڈھیر[i + j];
      إذا (unlikely (!(arg1.blending () && (arg1.عدد_اقدار_2 == n) && (arg1.قدر_اشاریہ_2 == j) &&
		      (arg1.deltas.لمائی == env.get_region_گنتی ()) &&
		      (arg1.deltas.لمائی == t->gains.لمائی))))
      {
	env.سیٹ_error ();
	أرجع;
      }
    }

    str_encoder_t encoder (param.flatStr);
    بنانشانی k_new = t->alive () ? t->میٹرکس.لمائی : 0;

    إذا (!k_new)
    {
      /* All of this VarData's regions died; blends dissolve into statics. */
      لكل (بنانشانی صحيح j = 0; j < n; j++)
      {
	ثابت blend_arg_t &arg1 = env.arg_ڈھیر[i + j];
	نمبر_t v;
	v.سیٹ_real (arg1.منزل_real () + round (instancer.fold_deltas (ivs, arg1.deltas.as_array ())));
	encoder.encode_عدد_cs (v);
      }
      أرجع;
    }

    /* Emit in chunks so the output charstring stays within the argument
     * stack limit: values already emitted for this op, plus this chunk's
     * defaults, deltas and count, must not exceed 513. */
    بنانشانی پورا = 0;
    طالما (پورا < n)
    {
      بنانشانی avail = 512 > (i + پورا) ? 512 - (i + پورا) : 0;
      بنانشانی chunk = hb_min (avail / (k_new + 1), n - پورا);
      إذا (unlikely (!chunk))
      {
	env.سیٹ_error ();
	أرجع;
      }

      لكل (بنانشانی صحيح j = پورا; j < پورا + chunk; j++)
      {
	ثابت blend_arg_t &arg1 = env.arg_ڈھیر[i + j];
	نمبر_t v;
	v.سیٹ_real (arg1.منزل_real () + round (instancer.fold_deltas (ivs, arg1.deltas.as_array ())));
	encoder.encode_عدد_cs (v);
      }
      لكل (بنانشانی صحيح j = پورا; j < پورا + chunk; j++)
      {
	ثابت blend_arg_t &arg1 = env.arg_ڈھیر[i + j];
	لكل (بنانشانی r = 0; r < k_new; r++)
	{
	  نمبر_t v;
	  v.سیٹ_int (hb_clamp_منزل<صحيح> (round (t->transform_delta (r, arg1.deltas.as_array ()))));
	  encoder.encode_عدد_cs (v);
	}
      }
      encoder.encode_int (chunk);
      encoder.encode_op (Op_کوڈ_blendcs);
      پورا += chunk;
    }
    param.emitted_blend = سچ;
  }

  /* Partial instancing: prepend a vsindex op when the charstring's
   * (remapped) ivs differs from its FD's (remapped) private-dict ivs. */
  ساكن فراغ emit_vsindex_اگیتر (cff2_cs_interp_env_t<blend_arg_t> &env, flatten_param_t& param)
  {
    إذا (!param.emitted_blend) أرجع;
    ثابت cff2_instancing_plan_t &instancer = *param.instancer;
    بنانشانی new_ivs = instancer.remap_ivs (env.get_ivs ());
    بنانشانی fd_ivs = instancer.remap_ivs (env.get_orig_ivs ());
    إذا (new_ivs == fd_ivs) أرجع;

    str_buff_t اگیتر;
    str_encoder_t enc (اگیتر);
    enc.encode_int ((صحيح) new_ivs);
    enc.encode_op (Op_کوڈ_vsindexcs);

    str_buff_t &buf = param.flatStr;
    بنانشانی old_len = buf.لمائی;
    إذا (unlikely (اگیتر.وچ_error () || !buf.resize (old_len + اگیتر.لمائی)))
    {
      env.سیٹ_error ();
      أرجع;
    }
    memmove (buf.arrayZ + اگیتر.لمائی, buf.arrayZ, old_len);
    hb_memcpy (buf.arrayZ, اگیتر.arrayZ, اگیتر.لمائی);
  }

  ساكن فراغ flush_op (op_کوڈ_t op, cff2_cs_interp_env_t<blend_arg_t> &env, flatten_param_t& param)
  {
    بدل (op)
    {
      حالة Op_کوڈ_return:
      حالة Op_کوڈ_endchar:
	أرجع;
      افتراضي:
	str_encoder_t  encoder (param.flatStr);
	encoder.encode_op (op);
    }
  }

  ساكن فراغ flush_hintmask (op_کوڈ_t op, cff2_cs_interp_env_t<blend_arg_t> &env, flatten_param_t& param)
  {
    SUPER::flush_hintmask (op, env, param);
    /* Preserve hintmask payload in captured commands for specializer re-encoding. */
    إذا (param.commands && !param.drop_hints && param.commands->لمائی > 0)
    {
      خودکار &cmd = param.commands->پوچھ ();
      إذا (cmd.op == op)
      {
        cmd.نقاب_بائٹاں.resize (env.hintmask_ناپ);
        إذا (unlikely (cmd.نقاب_بائٹاں.وچ_error ()))
        {
          env.سیٹ_error ();
          أرجع;
        }
        لكل (بنانشانی صحيح i = 0; i < env.hintmask_ناپ; i++)
          cmd.نقاب_بائٹاں[i] = env.str_ref[i];
      }
    }
    إذا (!param.drop_hints)
    {
      str_encoder_t  encoder (param.flatStr);
      لكل (بنانشانی صحيح i = 0; i < env.hintmask_ناپ; i++)
	encoder.encode_byte (env.str_ref[i]);
    }
  }

  خاص:
  تعريفنوع cff2_cs_opset_t<cff2_cs_opset_flatten_t, flatten_param_t, blend_arg_t> SUPER;
  تعريفنوع cs_opset_t<blend_arg_t, cff2_cs_opset_flatten_t, cff2_cs_opset_flatten_t, cff2_cs_interp_env_t<blend_arg_t>, flatten_param_t> CSOPSET;
};

بنية cff2_cs_opset_subr_subset_t : cff2_cs_opset_t<cff2_cs_opset_subr_subset_t, subr_subset_param_t, blend_arg_t>
{
  ساكن فراغ process_op (op_کوڈ_t op, cff2_cs_interp_env_t<blend_arg_t> &env, subr_subset_param_t& param)
  {
    بدل (op) {

      حالة Op_کوڈ_return:
	param.current_parsed_str->flush_coalesced (env.str_ref);
	param.current_parsed_str->سیٹ_parsed ();
	env.return_from_subr ();
	param.سیٹ_current_str (env, خطأ);
	اكسر;

      حالة Op_کوڈ_endchar:
	param.current_parsed_str->flush_coalesced (env.str_ref);
	param.current_parsed_str->سیٹ_parsed ();
	SUPER::process_op (op, env, param);
	اكسر;

      حالة Op_کوڈ_callsubr:
	process_call_subr (op, CSType_LocalSubr, env, param, env.localSubrs, param.local_گھیرا_2);
	اكسر;

      حالة Op_کوڈ_callgsubr:
	process_call_subr (op, CSType_GlobalSubr, env, param, env.globalSubrs, param.global_گھیرا);
	اكسر;

      افتراضي:
	SUPER::process_op (op, env, param);
	param.current_parsed_str->جوڑو_op (op, env.str_ref);
	اكسر;
    }
  }

  محمي:
  ساكن فراغ process_call_subr (op_کوڈ_t op, cs_type_t type,
				 cff2_cs_interp_env_t<blend_arg_t> &env, subr_subset_param_t& param,
				 cff2_biased_subrs_t& subrs, hb_سیٹ_t *گھیرا)
  {
    byte_str_ref_t    str_ref = env.str_ref;
    env.call_subr (subrs, type);
    param.current_parsed_str->جوڑو_call_op (op, str_ref, env.context.subr_عدد_2);
    گھیرا->جوڑو (env.context.subr_عدد_2);
    param.سیٹ_current_str (env, سچ);
  }

  خاص:
  تعريفنوع cff2_cs_opset_t<cff2_cs_opset_subr_subset_t, subr_subset_param_t, blend_arg_t> SUPER;
};

بنية cff2_subr_subsetter_t : subr_subsetter_t<cff2_subr_subsetter_t, CFF2Subrs, ثابت OT::cff2::accelerator_subset_t, cff2_cs_interp_env_t<blend_arg_t>, cff2_cs_opset_subr_subset_t>
{
  cff2_subr_subsetter_t (ثابت OT::cff2::accelerator_subset_t &acc_, ثابت hb_subset_plan_t *plan_)
    : subr_subsetter_t (acc_, plan_) {}

  ساكن فراغ complete_parsed_str (cff2_cs_interp_env_t<blend_arg_t> &env, subr_subset_param_t& param, parsed_cs_str_t &charstring)
  {
    /* vsindex is inserted at the beginning of the charstring as necessary */
    إذا (env.تکیا_vsindex ())
    {
      نمبر_t  ivs;
      ivs.سیٹ_int ((صحيح)env.get_ivs ());
      charstring.سیٹ_اگیتر (ivs, Op_کوڈ_vsindexcs);
    }
  }
};

بنية cff2_نجی_blend_encoder_param_t
{
  cff2_نجی_blend_encoder_param_t (hb_serialize_context_t *c,
				      ثابت CFF2_شے_Variation_Store *varStore,
				      hb_array_t<صحيح> normalized_coords) :
    c (c), varStore (varStore), normalized_coords (normalized_coords) {}

  فراغ init () {}

  فراغ process_blend ()
  {
    إذا (!تکیا_blend)
    {
      region_گنتی_2 = varStore->varStore.get_region_اشاریہ_گنتی (ivs);
      إذا (unlikely (!scalars.resize_exact (region_گنتی_2)))
      {
	إذا (c) c->err (HB_SERIALIZE_ERROR_OTHER);
	تکیا_blend = سچ;
	أرجع;
      }
      varStore->varStore.get_region_scalars (ivs, normalized_coords.arrayZ, normalized_coords.لمائی,
					     &scalars[0], region_گنتی_2);
      تکیا_blend = سچ;
    }
  }

  مزدوج blend_deltas (hb_array_t<ثابت نمبر_t> deltas) ثابت
  {
    مزدوج v = 0;
    إذا (likely (scalars.لمائی == deltas.لمائی))
    {
      بنانشانی گنتی = scalars.لمائی;
      لكل (بنانشانی i = 0; i < گنتی; i++)
	v += (مزدوج) scalars.arrayZ[i] * deltas.arrayZ[i].منزل_real ();
    }
    أرجع v;
  }


  hb_serialize_context_t *c = خالی_سوچک;
  منطقي تکیا_blend = خطأ;
  بنانشانی ivs = 0;
  بنانشانی region_گنتی_2 = 0;
  hb_سمتیہ_t<عائم> scalars;
  ثابت	 CFF2_شے_Variation_Store *varStore = خالی_سوچک;
  hb_array_t<صحيح> normalized_coords;

  /* CFF2 partial instancing: when set, blends are rewritten against the
   * instanced variation store instead of being flattened. */
  ثابت cff2_instancing_plan_t *instancer = خالی_سوچک;
  /* Error-compensated fold rounding state, per dict operator. */
  مزدوج fold_exact = 0.;
  مزدوج fold_emitted = 0.;
};

بنية cff2_نجی_dict_blend_opset_t : dict_opset_t
{
  ساكن فراغ process_arg_blend (cff2_نجی_blend_encoder_param_t& param,
				 نمبر_t &arg,
				 ثابت hb_array_t<ثابت نمبر_t> blends,
				 بنانشانی n, بنانشانی i)
  {
    arg.سیٹ_int (round (arg.منزل_real () + param.blend_deltas (blends)));
  }

  /* The fold added to a value, error-compensated across the operator: most
   * blended private-dict values (BlueValues, StemSnap*, ...) are
   * delta-encoded, so independently rounded folds would accumulate in the
   * decoded absolutes; keeping the emitted cumulative fold equal to the
   * rounded exact cumulative fold bounds every absolute within ±0.5. */
  ساكن مزدوج fold (cff2_نجی_blend_encoder_param_t& param,
		      ثابت hb_array_t<ثابت نمبر_t> blends)
  {
    مزدوج f = param.blend_deltas (blends);
    مزدوج emit = round (param.fold_exact + f) - param.fold_emitted;
    param.fold_exact += f;
    param.fold_emitted += emit;
    أرجع emit;
  }

  /* Partial instancing: emit the args preceding this blend group, then the
   * group itself rewritten against the instanced store, and clear the
   * stack.  The op's remaining args are flushed at process_op time. */
  ساكن فراغ rewrite_blends (cff2_priv_dict_interp_env_t& env,
			      cff2_نجی_blend_encoder_param_t& param,
			      بنانشانی شروع, بنانشانی n, بنانشانی k)
  {
    ثابت cff2_instancing_plan_t &instancer = *param.instancer;
    ثابت cff2_instancing_plan_t::vardata_transform_t *t = instancer.get_transform (param.ivs);
    إذا (unlikely (!t || t->gains.لمائی != k))
    {
      env.سیٹ_error ();
      أرجع;
    }

    str_buff_t str;
    str_encoder_t encoder (str);

    لكل (بنانشانی idx = 0; idx < شروع; idx++)
      encoder.encode_عدد_tp (env.arg_ڈھیر[idx]);

    بنانشانی k_new = t->alive () ? t->میٹرکس.لمائی : 0;
    إذا (!k_new)
    {
      /* This VarData died (or is unused by any retained charstring, which
       * can happen for FDs kept only for retain-gids holes); blends
       * dissolve into statics. */
      لكل (بنانشانی j = 0; j < n; j++)
      {
	ثابت hb_array_t<ثابت نمبر_t> blends = env.arg_ڈھیر.sub_array (شروع + n + (j * k), k);
	نمبر_t v;
	v.سیٹ_real (env.arg_ڈھیر[شروع + j].منزل_real () + fold (param, blends));
	encoder.encode_عدد_tp (v);
      }
    }
    وإلا
    {
      بنانشانی پورا = 0;
      طالما (پورا < n)
      {
	بنانشانی avail = 512 > (شروع + پورا) ? 512 - (شروع + پورا) : 0;
	بنانشانی chunk = hb_min (avail / (k_new + 1), n - پورا);
	إذا (unlikely (!chunk))
	{
	  env.سیٹ_error ();
	  أرجع;
	}

	لكل (بنانشانی j = پورا; j < پورا + chunk; j++)
	{
	  ثابت hb_array_t<ثابت نمبر_t> blends = env.arg_ڈھیر.sub_array (شروع + n + (j * k), k);
	  نمبر_t v;
	  v.سیٹ_real (env.arg_ڈھیر[شروع + j].منزل_real () + fold (param, blends));
	  encoder.encode_عدد_tp (v);
	}
	لكل (بنانشانی j = پورا; j < پورا + chunk; j++)
	{
	  ثابت hb_array_t<ثابت نمبر_t> blends = env.arg_ڈھیر.sub_array (شروع + n + (j * k), k);
	  لكل (بنانشانی r = 0; r < k_new; r++)
	  {
	    نمبر_t v;
	    v.سیٹ_int (hb_clamp_منزل<صحيح> (round (t->transform_delta (r, blends))));
	    encoder.encode_عدد_tp (v);
	  }
	}
	encoder.encode_int (chunk);
	encoder.encode_op (Op_کوڈ_blenddict);
	پورا += chunk;
      }
    }

    خودکار بائٹاں = str.as_بائٹاں ();
    param.c->embed (&بائٹاں, بائٹاں.لمائی);

    env.clear_دلیلاں ();
  }

  ساكن فراغ process_blend (cff2_priv_dict_interp_env_t& env, cff2_نجی_blend_encoder_param_t& param)
  {
    بنانشانی صحيح n, k;

    param.process_blend ();
    k = param.region_گنتی_2;
    n = env.arg_ڈھیر.نکالو_uint ();

    بنانشانی گنتی = env.arg_ڈھیر.get_گنتی ();
    بنانشانی صحيح کل;
    إذا (unlikely (hb_unsigned_mul_overflows (k + 1, n, &کل) || کل > گنتی))
    {
      env.سیٹ_error ();
      أرجع;
    }

    /* copy the blend values into blend array of the default values */
    بنانشانی صحيح شروع = گنتی - کل;
    /* let an obvious error case fail, but note CFF2 spec doesn't forbid n==0 */
    إذا (unlikely (شروع > گنتی))
    {
      env.سیٹ_error ();
      أرجع;
    }

    إذا (param.instancer && k)
    {
      rewrite_blends (env, param, شروع, n, k);
      أرجع;
    }

    لكل (بنانشانی صحيح i = 0; i < n; i++)
    {
      ثابت hb_array_t<ثابت نمبر_t> blends = env.arg_ڈھیر.sub_array (شروع + n + (i * k), k);
      process_arg_blend (param, env.arg_ڈھیر[شروع + i], blends, n, i);
    }

    /* pop off blend values leaving default values now adorned with blend values */
    env.arg_ڈھیر.نکالو (k * n);
  }

  ساكن فراغ process_op (op_کوڈ_t op, cff2_priv_dict_interp_env_t& env, cff2_نجی_blend_encoder_param_t& param)
  {
    بدل (op) {
      حالة Op_کوڈ_Std_HW:
      حالة Op_کوڈ_Std_VW:
      حالة Op_کوڈ_Blue_Scale:
      حالة Op_کوڈ_Blue_کھسکاؤ:
      حالة Op_کوڈ_Blue_Fuzz:
      حالة Op_کوڈ_Expansion_ضارب:
      حالة Op_کوڈ_Language_گروہ:
      حالة Op_کوڈ_Blue_اقدار:
      حالة Op_کوڈ_Other_Blues:
      حالة Op_کوڈ_Family_Blues:
      حالة Op_کوڈ_Family_Other_Blues:
      حالة Op_کوڈ_Stem_Snap_H:
      حالة Op_کوڈ_Stem_Snap_V:
	اكسر;
      حالة Op_کوڈ_vsindexdict:
	env.process_vsindex ();
	param.ivs = env.get_ivs ();
	إذا (param.instancer)
	{
	  /* Re-emit with the remapped vsindex; 0 is the dict default. */
	  بنانشانی new_ivs = param.instancer->remap_ivs (param.ivs);
	  إذا (new_ivs)
	  {
	    str_buff_t str;
	    str_encoder_t encoder (str);
	    encoder.encode_int ((صحيح) new_ivs);
	    encoder.encode_op (Op_کوڈ_vsindexdict);
	    خودکار بائٹاں = str.as_بائٹاں ();
	    param.c->embed (&بائٹاں, بائٹاں.لمائی);
	  }
	}
	env.clear_دلیلاں ();
	أرجع;
      حالة Op_کوڈ_blenddict:
	process_blend (env, param);
	أرجع;

      افتراضي:
	dict_opset_t::process_op (op, env);
	إذا (!env.arg_ڈھیر.is_empty ()) أرجع;
	اكسر;
    }

    إذا (unlikely (env.وچ_error ())) أرجع;

    // Write args then op

    str_buff_t str;
    str_encoder_t encoder (str);

    بنانشانی گنتی = env.arg_ڈھیر.get_گنتی ();
    لكل (بنانشانی i = 0; i < گنتی; i++)
      encoder.encode_عدد_tp (env.arg_ڈھیر[i]);

    encoder.encode_op (op);

    خودکار بائٹاں = str.as_بائٹاں ();
    param.c->embed (&بائٹاں, بائٹاں.لمائی);

    env.clear_دلیلاں ();
    param.fold_exact = param.fold_emitted = 0.;
  }
};

بنية cff2_نجی_dict_op_serializer_t : op_serializer_t
{
  cff2_نجی_dict_op_serializer_t (منطقي desubroutinize_, منطقي drop_hints_, منطقي pinned_,
				     ثابت CFF::CFF2_شے_Variation_Store* varStore_,
				     hb_array_t<صحيح> normalized_coords_,
				     ثابت cff2_instancing_plan_t *instancer_ = خالی_سوچک)
    : desubroutinize (desubroutinize_), drop_hints (drop_hints_), pinned (pinned_),
      param (خالی_سوچک, varStore_, normalized_coords_)
  { param.instancer = instancer_; }

  منطقي serialize (hb_serialize_context_t *c,
		  ثابت op_str_t &opstr,
		  objidx_t subrs_link) ثابت
  {
    TRACE_SERIALIZE (هذا);

    إذا (drop_hints && dict_opset_t::is_hint_op (opstr.op))
      return_trace (سچ);

    إذا (opstr.op == Op_کوڈ_Subrs)
    {
      إذا (desubroutinize || !subrs_link)
	return_trace (سچ);
      وإلا
	return_trace (FontDict::serialize_link2_op (c, opstr.op, subrs_link));
    }

    إذا (pinned || param.instancer)
    {
      /* Reinterpret opstr and process blends.  The param persists across
       * this dict's opstrs, so that an ivs set by a vsindexdict opstr is
       * still in effect when a later opstr's blends are processed. */
      cff2_priv_dict_interp_env_t env {hb_ubytes_t (opstr.ptr, opstr.لمائی)};
      param.c = c;
      dict_interpreter_t<cff2_نجی_dict_blend_opset_t, cff2_نجی_blend_encoder_param_t, cff2_priv_dict_interp_env_t> interp (env);
      return_trace (interp.interpret (param));
    }

    return_trace (copy_opstr (c, opstr));
  }

  محمي:
  ثابت منطقي desubroutinize;
  ثابت منطقي drop_hints;
  ثابت منطقي pinned;
  بدلنجوگ cff2_نجی_blend_encoder_param_t param;
};



نطاق OT {
بنية cff2_subset_plan
{
  منطقي create (ثابت OT::cff2::accelerator_subset_t &acc,
	      hb_subset_plan_t *plan)
  {
    /* make sure notdef is first */
    hb_codepoint_t old_glyph;
    إذا (!plan->old_gid_for_new_gid (0, &old_glyph) || (old_glyph != 0)) أرجع خطأ;

    عدد_glyphs = plan->عدد_output_glyphs ();
    orig_fdcount = acc.fdArray->گنتی;

    drop_hints = plan->flags & HB_SUBSET_FLAGS_NO_HINTING;
    منطقي instancing_requested = (منطقي) plan->normalized_coords;
    pinned = instancing_requested && plan->all_axes_pinned;
    normalized_coords = plan->normalized_coords;
    سر_maxp_info = plan->سر_maxp_info;
    hmtx_map = &plan->hmtx_map;
    desubroutinize = plan->flags & HB_SUBSET_FLAGS_DESUBROUTINIZE ||
		     instancing_requested; // For instancing we need this path

    /* Partial instancing: some axes pinned or limited, others kept. */
    partial_instancing = instancing_requested && !pinned &&
			 acc.varStore != &Null (CFF2_شے_Variation_Store);
    إذا (partial_instancing &&
	unlikely (!instancing.create (acc.varStore, plan)))
      أرجع خطأ;
    axis_ٹیگ_سوچی = &plan->axis_ٹیگ_سوچی;

    /* Enable command capture for CFF2→CFF1 conversion (for specialization) */
    capture_commands = pinned;

 #جے_تعریف_شدہ HB_EXPERIMENTAL_API
    min_charstrings_off_ناپ = (plan->flags & HB_SUBSET_FLAGS_IFTB_REQUIREMENTS) ? 4 : 0;
 #وإلا
    min_charstrings_off_ناپ = 0;
 #جے_انت

    إذا (desubroutinize)
    {
      إذا (partial_instancing)
      {
	/* Pass 1: collect which VarDatas the retained blends reference, so
	 * unused ones get pruned and the survivors renumbered. */
	hb_سیٹ_t ورتیا_fds;
	لكل (بنانشانی صحيح i = 0; i < عدد_glyphs; i++)
	{
	  hb_codepoint_t glyph;
	  إذا (!plan->old_gid_for_new_gid (i, &glyph))
	    استمر;
	  ثابت hb_ubytes_t str = (*acc.char_لڑیاں)[glyph];
	  بنانشانی صحيح fd = CFF::subset_get_fd (acc, glyph);
	  إذا (unlikely (fd >= acc.fd_گنتی))
	    أرجع خطأ;
	  ورتیا_fds.جوڑو (fd);

	  cff2_cs_interp_env_t<blend_arg_t> env (str, acc, fd);
	  cs_interpreter_t<cff2_cs_interp_env_t<blend_arg_t>, cff2_cs_opset_usage_t, cff2_usage_param_t> interp (env);
	  cff2_usage_param_t param;
	  param.instancing = &instancing;
	  param.drop_hints = drop_hints;
	  إذا (unlikely (!interp.interpret (param)))
	    أرجع خطأ;
	}
	لكل (بنانشانی fd : ورتیا_fds)
	{
	  cff2_usage_param_t param;
	  param.instancing = &instancing;
	  بنانشانی گنتی = acc.نجی_Dicts[fd].get_گنتی ();
	  لكل (بنانشانی j = 0; j < گنتی; j++)
	  {
	    ثابت op_str_t &opstr = acc.نجی_Dicts[fd][j];
	    إذا (drop_hints && dict_opset_t::is_hint_op (opstr.op))
	      استمر;
	    cff2_priv_dict_interp_env_t env {hb_ubytes_t (opstr.ptr, opstr.لمائی)};
	    dict_interpreter_t<cff2_نجی_dict_usage_opset_t, cff2_usage_param_t, cff2_priv_dict_interp_env_t> interp (env);
	    إذا (unlikely (!interp.interpret (param)))
	      أرجع خطأ;
	  }
	}
	إذا (unlikely (!instancing.finalize ()))
	  أرجع خطأ;

	/* Pass 2: like subr_flattener_t, but blends stay symbolic (no coords
	 * fed to the env) and get rewritten against the instanced store. */
	إذا (!subset_charstrings.resize_exact (عدد_glyphs))
	  أرجع خطأ;
	لكل (بنانشانی صحيح i = 0; i < عدد_glyphs; i++)
	{
	  hb_codepoint_t glyph;
	  إذا (!plan->old_gid_for_new_gid (i, &glyph))
	    استمر;
	  ثابت hb_ubytes_t str = (*acc.char_لڑیاں)[glyph];
	  بنانشانی صحيح fd = CFF::subset_get_fd (acc, glyph);
	  إذا (unlikely (fd >= acc.fd_گنتی))
	    أرجع خطأ;

	  cff2_cs_interp_env_t<blend_arg_t> env (str, acc, fd);
	  cs_interpreter_t<cff2_cs_interp_env_t<blend_arg_t>, cff2_cs_opset_flatten_t, flatten_param_t> interp (env);
	  flatten_param_t param = {
	    subset_charstrings.arrayZ[i],
	    (منطقي) (plan->flags & HB_SUBSET_FLAGS_NO_HINTING),
	    plan
	  };
	  param.instancer = &instancing;
	  إذا (unlikely (!interp.interpret (param)))
	    أرجع خطأ;
	}
      }
      وإلا
      {
      /* Flatten global & local subrs */
      subr_flattener_t<ثابت OT::cff2::accelerator_subset_t, cff2_cs_interp_env_t<blend_arg_t>, cff2_cs_opset_flatten_t>
		    flattener(acc, plan);

      /* Enable command capture if requested (for specialization) */
      إذا (capture_commands)
      {
	إذا (!charstring_commands.resize_exact (عدد_glyphs))
	  أرجع خطأ;

	إذا (!flattener.flatten (subset_charstrings, &charstring_commands))
	  أرجع خطأ;
      }
      وإلا
      {
	إذا (!flattener.flatten (subset_charstrings))
	  أرجع خطأ;
      }
      }
    }
    وإلا
    {
      cff2_subr_subsetter_t	subr_subsetter (acc, plan);

      /* Subset subrs: collect used subroutines, leaving all unused ones behind */
      إذا (!subr_subsetter.subset ())
	أرجع خطأ;

      /* encode charstrings, global subrs, local subrs with new subroutine numbers */
      إذا (!subr_subsetter.encode_charstrings (subset_charstrings, !pinned))
	أرجع خطأ;

      إذا (!subr_subsetter.encode_globalsubrs (subset_globalsubrs))
	أرجع خطأ;

      /* local subrs */
      إذا (!subset_localsubrs.resize (orig_fdcount))
	أرجع خطأ;
      لكل (بنانشانی صحيح fd = 0; fd < orig_fdcount; fd++)
      {
	subset_localsubrs[fd].init ();
	إذا (!subr_subsetter.encode_localsubrs (fd, subset_localsubrs[fd]))
	  أرجع خطأ;
      }
    }

    /* FDSelect */
    إذا (acc.fdSelect != &Null (CFF2FDSelect))
    {
      إذا (unlikely (!hb_plan_subset_cff_fdselect (plan,
						  orig_fdcount,
						  *(ثابت FDSelect *)acc.fdSelect,
						  acc.عدد_glyphs,
						  subset_fdcount,
						  subset_fdselect_ناپ,
						  subset_fdselect_format,
						  subset_fdselect_ranges,
						  fdmap)))
	أرجع خطأ;
    }
    وإلا
      fdmap.پچھان_تت (1);

    أرجع سچ;
  }

  cff2_sub_جدول_info_t info;

  بنانشانی صحيح    عدد_glyphs;
  بنانشانی صحيح    orig_fdcount = 0;
  بنانشانی صحيح    subset_fdcount = 1;
  بنانشانی صحيح    subset_fdselect_ناپ = 0;
  بنانشانی صحيح    subset_fdselect_format = 0;
  منطقي            pinned = خطأ;
  hb_سمتیہ_t<کوڈ_pair_t>   subset_fdselect_ranges;

  hb_inc_bimap_t   fdmap;

  str_buff_vec_t	    subset_charstrings;
  str_buff_vec_t	    subset_globalsubrs;
  hb_سمتیہ_t<str_buff_vec_t> subset_localsubrs;

  منطقي	    drop_hints = خطأ;
  منطقي	    desubroutinize = خطأ;

  منطقي	    partial_instancing = خطأ;
  cff2_instancing_plan_t instancing;
  ثابت hb_سمتیہ_t<hb_ٹیگ_t> *axis_ٹیگ_سوچی = خالی_سوچک;

  بنانشانی  min_charstrings_off_ناپ = 0;

  hb_array_t<صحيح> normalized_coords; // For instantiation
  سر_maxp_info_t سر_maxp_info;  // For FontBBox
  ثابت hb_hashmap_t<hb_codepoint_t, hb_pair_t<بنانشانی, صحيح>> *hmtx_map; // For widths

  // Width optimization results (for CFF1 conversion)
  بنانشانی default_چوڑائی = 0;
  بنانشانی nominal_چوڑائی = 0;

  // Command capture for specialization (CFF2→CFF1 conversion)
  منطقي capture_commands = خطأ;
  hb_سمتیہ_t<hb_سمتیہ_t<cs_command_t>> charstring_commands;
};
} // namespace OT

/*
 * CFF2 to CFF1 Converter Implementation
 */

#ضمن "hb-cff-width-optimizer.hh"
#ضمن "hb-cff-specializer.hh"

/* Serialize charstrings using CFF1 format with widths */
ساكن منطقي
_serialize_cff1_charstrings (hb_serialize_context_t *c,
                             OT::cff2_subset_plan &plan,
                             بنانشانی default_چوڑائی,
                             بنانشانی nominal_چوڑائی)
{
  c->push ();

  // CFF1 requires:
  // 1. Width at the beginning (if != defaultWidthX)
  // 2. endchar at the end
  str_buff_vec_t cff1_charstrings;
  إذا (unlikely (!cff1_charstrings.resize (plan.subset_charstrings.لمائی)))
  {
    c->نکالو_discard ();
    أرجع خطأ;
  }

  لكل (بنانشانی i = 0; i < plan.subset_charstrings.لمائی; i++)
  {
    // Get width for this glyph from hmtx_map
    بنانشانی چوڑائی = 0;
    إذا (plan.hmtx_map->has (i))
      چوڑائی = plan.hmtx_map->get (i).پہلا;

    // Encode width if different from default
    str_encoder_t encoder (cff1_charstrings[i]);
    إذا (چوڑائی != default_چوڑائی)
    {
      صحيح delta = (صحيح) چوڑائی - (صحيح) nominal_چوڑائی;
      encoder.encode_int (delta);
    }

    // Use specialized commands if available, otherwise use binary
    إذا (plan.capture_commands && i < plan.charstring_commands.لمائی &&
        plan.charstring_commands[i].لمائی > 0)
    {
      // Specialize and encode commands
      خودکار &commands = plan.charstring_commands[i];
      CFF::specialize_commands (commands, 48);  /* maxstack=48 for CFF1 */
      إذا (unlikely (!CFF::encode_commands (commands, cff1_charstrings[i])))
      {
        c->نکالو_discard ();
        أرجع خطأ;
      }
    }
    وإلا
    {
      // Use binary CharString
      ثابت str_buff_t &cs = plan.subset_charstrings[i];
      لكل (بنانشانی j = 0; j < cs.لمائی; j++)
        cff1_charstrings[i].push (cs[j]);
    }

    // Check if it already ends with endchar (0x0e) or return (0x0b)
    إذا (cff1_charstrings[i].لمائی == 0 ||
        (cff1_charstrings[i].پوچھ () != 0x0e && cff1_charstrings[i].پوچھ () != 0x0b))
    {
      // Append endchar operator
      إذا (unlikely (!cff1_charstrings[i].ڈالو_or_fail (0x0e)))
      {
        c->نکالو_discard ();
        أرجع خطأ;
      }
    }
  }

  بنانشانی انکڑے_ناپ = 0;
  بنانشانی کل_ناپ = CFF1_Char_لڑیاں::کل_ناپ (cff1_charstrings, &انکڑے_ناپ, plan.min_charstrings_off_ناپ);
  إذا (unlikely (!c->شروع_zerocopy (کل_ناپ)))
  {
    c->نکالو_discard ();
    أرجع خطأ;
  }

  خودکار *cs = c->شروع_embed<CFF1_Char_لڑیاں> ();
  إذا (unlikely (!cs->serialize (c, cff1_charstrings, &انکڑے_ناپ, plan.min_charstrings_off_ناپ)))
  {
    c->نکالو_discard ();
    أرجع خطأ;
  }

  plan.info.char_لڑیاں_link = c->نکالو_pack (خطأ);
  أرجع سچ;
}

/* Serialize CID Charset (format 2 range: gid 0-N -> cid 0-N) */
ساكن منطقي
_serialize_cff1_charset (hb_serialize_context_t *c,
                         بنانشانی صحيح عدد_glyphs,
                         objidx_t &charset_link)
{
  // For CID fonts, create a simple identity charset
  // Format 2: one range covering all glyphs (except .notdef)
  c->push ();

  خودکار *charset = c->شروع_embed<Charset> ();
  إذا (unlikely (!charset))
  {
    c->نکالو_discard ();
    أرجع خطأ;
  }

  // Create a single range for CID 1 to num_glyphs-1
  hb_سمتیہ_t<کوڈ_pair_t> ranges;
  إذا (عدد_glyphs > 1)
  {
    کوڈ_pair_t range;
    range.کوڈ = 1;  // first CID
    range.glyph = عدد_glyphs - 2;  // nLeft (covers glyphs 1 to num_glyphs-1)
    ranges.push (range);
  }

  إذا (unlikely (!charset->serialize (c, 2, عدد_glyphs, ranges)))
  {
    c->نکالو_discard ();
    أرجع خطأ;
  }

  charset_link = c->نکالو_pack ();
  أرجع سچ;
}

/* CFF2 to CFF1 serialization */
نطاق CFF {

منطقي
serialize_cff2_منزل_cff1 (hb_serialize_context_t *c,
                        OT::cff2_subset_plan &plan,
                        ثابت cff2_چوٹی_dict_اقدار_t &cff2_چوٹی_Dict,
                        ثابت OT::cff2::accelerator_subset_t &acc)
{
  TRACE_SERIALIZE (هذا);

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
    hb_سمتیہ_t<بنانشانی> widths;
    widths.alloc (plan.عدد_glyphs);

    لكل (بنانشانی gid = 0; gid < plan.عدد_glyphs; gid++)
    {
      بنانشانی چوڑائی = 0;
      إذا (plan.hmtx_map->has (gid))
        چوڑائی = plan.hmtx_map->get (gid).پہلا;
      widths.push (چوڑائی);
    }

    // Optimize defaultWidthX and nominalWidthX
    CFF::optimize_widths (widths, plan.default_چوڑائی, plan.nominal_چوڑائی);
  }

  // 1. CharStrings (with widths prepended)
  إذا (!_serialize_cff1_charstrings (c, plan, plan.default_چوڑائی, plan.nominal_چوڑائی))
    return_trace (خطأ);

  // 2. Private DICs & Local Subrs (same as CFF2)
  hb_سمتیہ_t<جدول_info_t> نجی_dict_infos;
  إذا (unlikely (!نجی_dict_infos.resize (plan.subset_fdcount)))
    return_trace (خطأ);

  لكل (صحيح i = (صحيح)acc.نجی_Dicts.لمائی; --i >= 0;)
  {
    إذا (plan.fdmap.has (i))
    {
      objidx_t subrs_link = 0;

      إذا (plan.subset_localsubrs[i].لمائی > 0)
      {
        خودکار *dest = c->push<CFF1Subrs> ();
        إذا (likely (dest->serialize (c, plan.subset_localsubrs[i])))
          subrs_link = c->نکالو_pack (خطأ);
        وإلا
        {
          c->نکالو_discard ();
          return_trace (خطأ);
        }
      }

      خودکار *pd = c->push<نجی_Dict> ();
      // Use the CFF2 Private DICT serializer which instantiates blends when pinned=true
      cff2_نجی_dict_op_serializer_t privSzr (plan.desubroutinize, plan.drop_hints, plan.pinned,
                                                  acc.varStore, plan.normalized_coords);
      إذا (likely (pd->serialize (c, acc.نجی_Dicts[i], privSzr, subrs_link)))
      {
        // Add defaultWidthX and nominalWidthX for CFF1
        str_buff_t چوڑائی_ops;
        str_encoder_t encoder (چوڑائی_ops);
        encoder.encode_int (plan.default_چوڑائی);
        encoder.encode_op (Op_کوڈ_default_چوڑائی_X);
        encoder.encode_int (plan.nominal_چوڑائی);
        encoder.encode_op (Op_کوڈ_nominal_چوڑائی_X);

        إذا (!encoder.وچ_error () && c->embed (چوڑائی_ops.as_بائٹاں ().arrayZ, چوڑائی_ops.لمائی))
        {
          بنانشانی fd = plan.fdmap[i];
          نجی_dict_infos[fd].ناپ = c->لمائی ();
          نجی_dict_infos[fd].link = c->نکالو_pack ();
        }
        وإلا
        {
          c->نکالو_discard ();
          return_trace (خطأ);
        }
      }
      وإلا
      {
        c->نکالو_discard ();
        return_trace (خطأ);
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
                { أرجع plan.fdmap.has (&_ - &acc.fontDicts[0]); }),
              hb_iter (نجی_dict_infos))
    ;
    // Explicitly specify template parameters: DICTVAL, INFO
    منطقي success = fda->serialize<cff2_font_dict_اقدار_t, جدول_info_t> (c, it, fontSzr);
    إذا (success)
      plan.info.fd_array_link = c->نکالو_pack (خطأ);
    وإلا
    {
      c->نکالو_discard ();
      return_trace (خطأ);
    }
  }

  // 4. FDSelect (required in CFF1 CID-keyed fonts)
  // CFF1 requires FDSelect for all CID-keyed fonts, even with just one FD
  // CFF2 makes it optional when there's only one FD
  إذا (acc.fdSelect != &Null (CFF2FDSelect))
  {
    c->push ();
    إذا (likely (hb_serialize_cff_fdselect (c, plan.عدد_glyphs,
                                          *(ثابت FDSelect *)acc.fdSelect,
                                          plan.orig_fdcount,
                                          plan.subset_fdselect_format,
                                          plan.subset_fdselect_ناپ,
                                          plan.subset_fdselect_ranges)))
      plan.info.fd_select.link = c->نکالو_pack ();
    وإلا
    {
      c->نکالو_discard ();
      return_trace (خطأ);
    }
  }
  وإلا
  {
    // Create a range-based FDSelect3 mapping all glyphs to FD 0
    // Format: format(1) + nRanges(2) + range(3) + sentinel(2) = 8 bytes
    c->push ();

    // Format byte
    HBUINT8 format;
    format = 3;
    إذا (unlikely (!c->embed (format)))
    {
      c->نکالو_discard ();
      return_trace (خطأ);
    }

    // nRanges
    HBUINT16 nRanges;
    nRanges = 1;
    إذا (unlikely (!c->embed (nRanges)))
    {
      c->نکالو_discard ();
      return_trace (خطأ);
    }

    // Single range: {first: 0, fd: 0}
    FDSelect3_Range range;
    range.پہلا = 0;
    range.fd = 0;
    إذا (unlikely (!c->embed (range)))
    {
      c->نکالو_discard ();
      return_trace (خطأ);
    }

    // Sentinel (number of glyphs)
    HBUINT16 sentinel;
    sentinel = plan.عدد_glyphs;
    إذا (unlikely (!c->embed (sentinel)))
    {
      c->نکالو_discard ();
      return_trace (خطأ);
    }

    plan.info.fd_select.link = c->نکالو_pack ();
  }

  // 5. Charset (CID charset for identity mapping)
  objidx_t charset_link;
  إذا (!_serialize_cff1_charset (c, plan.عدد_glyphs, charset_link))
    return_trace (خطأ);

  // 6. Global Subrs
  {
    خودکار *dest = c->push<CFF1Subrs> ();
    إذا (likely (dest->serialize (c, plan.subset_globalsubrs)))
      c->نکالو_pack (خطأ);
    وإلا
    {
      c->نکالو_discard ();
      return_trace (خطأ);
    }
  }

  // 7. String INDEX - Add "Adobe" and "Identity" for ROS operator
  {
    ثابت حرف *adobe_str = "Adobe";
    ثابت حرف *پچھان_تت_str = "Identity";
    بنانشانی adobe_len = 5;  // strlen("Adobe")
    بنانشانی پچھان_تت_len = 8;  // strlen("Identity")

    // Build strings array
    hb_سمتیہ_t<hb_ubytes_t> لڑیاں;
    لڑیاں.alloc (2);
    لڑیاں.push (hb_ubytes_t ((ثابت بنانشانی حرف *) adobe_str, adobe_len));
    لڑیاں.push (hb_ubytes_t ((ثابت بنانشانی حرف *) پچھان_تت_str, پچھان_تت_len));

    // Serialize as CFF INDEX
    خودکار *dest = c->push<CFF1_اشاریہ> ();
    إذا (likely (dest->serialize (c, لڑیاں)))
      c->نکالو_pack (خطأ);
    وإلا
    {
      c->نکالو_discard ();
      return_trace (خطأ);
    }
  }

  // 8. CFF Header
  OT::cff1 *cff = c->allocate_min<OT::cff1> ();
  إذا (unlikely (!cff)) return_trace (خطأ);

  /*سرخی*/
  cff->نسخہ.major = 0x01;
  cff->نسخہ.minor = 0x00;
  cff->name_اشاریہ = cff->min_ناپ;
  cff->off_ناپ = 4; /* unused? */

  // 9. Name INDEX (single entry)
  {
    بنانشانی name_len = لڑی_لمبائی (CFF1_DEFAULT_FONT_NAME);

    CFF1_اشاریہ *idx = c->شروع_embed<CFF1_اشاریہ> ();
    إذا (unlikely (!idx)) return_trace (خطأ);

    إذا (unlikely (!idx->serialize_سرخی (c, hb_iter (&name_len, 1), name_len)))
      return_trace (خطأ);

    إذا (unlikely (!c->embed (CFF1_DEFAULT_FONT_NAME, name_len)))
      return_trace (خطأ);
  }

  // 10. Top DICT INDEX
  {
    // Serialize the Top DICT data first
    c->push<چوٹی_Dict> ();
    cff1_from_cff2_چوٹی_dict_op_serializer_t چوٹی_Szr;

    // Serialize ROS first
    إذا (unlikely (!چوٹی_Szr.serialize_ros (c)))
    {
      c->نکالو_discard ();
      return_trace (خطأ);
    }

    // Serialize FontBBox from head table
    {
      str_buff_t bbox_buff;
      str_encoder_t encoder (bbox_buff);

      encoder.encode_int (plan.سر_maxp_info.xMin);
      encoder.encode_int (plan.سر_maxp_info.yMin);
      encoder.encode_int (plan.سر_maxp_info.xMax);
      encoder.encode_int (plan.سر_maxp_info.yMax);
      encoder.encode_op (Op_کوڈ_Font_BBox);

      إذا (encoder.وچ_error () || !c->embed (bbox_buff.as_بائٹاں ().arrayZ, bbox_buff.لمائی))
      {
        c->نکالو_discard ();
        return_trace (خطأ);
      }
    }

    // Serialize charset operator
    إذا (charset_link && unlikely (!FontDict::serialize_link4_op (c, Op_کوڈ_charset, charset_link, whence_t::Absolute)))
    {
      c->نکالو_discard ();
      return_trace (خطأ);
    }

    // Serialize FDSelect operator (required for CID-keyed CFF1 fonts)
    إذا (plan.info.fd_select.link && unlikely (!FontDict::serialize_link4_op (c, Op_کوڈ_FDSelect, plan.info.fd_select.link, whence_t::Absolute)))
    {
      c->نکالو_discard ();
      return_trace (خطأ);
    }

    // Serialize FDArray operator (required for CID-keyed CFF1 fonts)
    إذا (plan.info.fd_array_link && unlikely (!FontDict::serialize_link4_op (c, Op_کوڈ_FDArray, plan.info.fd_array_link, whence_t::Absolute)))
    {
      c->نکالو_discard ();
      return_trace (خطأ);
    }

    // Serialize other operators from CFF2 TopDict
    لكل (ثابت خودکار &opstr : cff2_چوٹی_Dict.values)
    {
      إذا (unlikely (!چوٹی_Szr.serialize (c, opstr, plan.info)))
      {
        c->نکالو_discard ();
        return_trace (خطأ);
      }
    }

    بنانشانی چوٹی_ناپ = c->لمائی ();
    c->نکالو_pack (خطأ);

    // Serialize INDEX header
    خودکار *dest = c->شروع_embed<CFF1_اشاریہ> ();
    إذا (unlikely (!dest->serialize_سرخی (c, hb_iter (&چوٹی_ناپ, 1), چوٹی_ناپ)))
      return_trace (خطأ);
  }

  return_trace (سچ);
}

} /* namespace CFF */

ساكن منطقي _serialize_cff2_charstrings (hb_serialize_context_t *c,
			     cff2_subset_plan &plan,
			     ثابت OT::cff2::accelerator_subset_t  &acc)
{
  c->push ();

  بنانشانی انکڑے_ناپ = 0;
  بنانشانی کل_ناپ = CFF2_Char_لڑیاں::کل_ناپ (plan.subset_charstrings, &انکڑے_ناپ, plan.min_charstrings_off_ناپ);
  إذا (unlikely (!c->شروع_zerocopy (کل_ناپ)))
    أرجع خطأ;

  خودکار *cs = c->شروع_embed<CFF2_Char_لڑیاں> ();
  إذا (unlikely (!cs->serialize (c, plan.subset_charstrings, &انکڑے_ناپ, plan.min_charstrings_off_ناپ)))
  {
    c->نکالو_discard ();
    أرجع خطأ;
  }

  plan.info.char_لڑیاں_link = c->نکالو_pack (خطأ);
  أرجع سچ;
}

منطقي
OT::cff2::accelerator_subset_t::serialize (hb_serialize_context_t *c,
					   بنية cff2_subset_plan &plan,
					   hb_array_t<صحيح> normalized_coords) ثابت
{
  /* push charstrings onto the object stack first which will ensure it packs as the last
     object in the table. Keeping the chastrings last satisfies the requirements for patching
     via IFTB. If this ordering needs to be changed in the future, charstrings should be left
     at the end whenever HB_SUBSET_FLAGS_ITFB_REQUIREMENTS is enabled. */
  إذا (!_serialize_cff2_charstrings(c, plan, *هذا))
    أرجع خطأ;

  /* private dicts & local subrs */
  hb_سمتیہ_t<جدول_info_t>  نجی_dict_infos;
  إذا (unlikely (!نجی_dict_infos.resize (plan.subset_fdcount))) أرجع خطأ;

  لكل (صحيح i = (صحيح)نجی_Dicts.لمائی; --i >= 0 ;)
  {
    إذا (plan.fdmap.has (i))
    {
      objidx_t	subrs_link = 0;

      إذا (plan.subset_localsubrs[i].لمائی > 0)
      {
	خودکار *dest = c->push <CFF2Subrs> ();
	إذا (likely (dest->serialize (c, plan.subset_localsubrs[i])))
	  subrs_link = c->نکالو_pack (خطأ);
	وإلا
	{
	  c->نکالو_discard ();
	  أرجع خطأ;
	}
      }
      خودکار *pd = c->push<نجی_Dict> ();
      cff2_نجی_dict_op_serializer_t privSzr (plan.desubroutinize, plan.drop_hints, plan.pinned,
						 varStore, normalized_coords,
						 plan.partial_instancing ? &plan.instancing : خالی_سوچک);
      إذا (likely (pd->serialize (c, نجی_Dicts[i], privSzr, subrs_link)))
      {
	بنانشانی fd = plan.fdmap[i];
	نجی_dict_infos[fd].ناپ = c->لمائی ();
	نجی_dict_infos[fd].link = c->نکالو_pack ();
      }
      وإلا
      {
	c->نکالو_discard ();
	أرجع خطأ;
      }
    }
  }

  /* FDSelect */
  إذا (fdSelect != &Null (CFF2FDSelect))
  {
    c->push ();
    إذا (likely (hb_serialize_cff_fdselect (c, plan.عدد_glyphs, *(ثابت FDSelect *)fdSelect,
					   plan.orig_fdcount,
					   plan.subset_fdselect_format, plan.subset_fdselect_ناپ,
					   plan.subset_fdselect_ranges)))
      plan.info.fd_select.link = c->نکالو_pack ();
    وإلا
    {
      c->نکالو_discard ();
      أرجع خطأ;
    }
  }

  /* FDArray (FD Index) */
  {
    خودکار *fda = c->push<CFF2FDArray> ();
    cff_font_dict_op_serializer_t fontSzr;
    خودکار it =
    + hb_zip (+ hb_iter (fontDicts)
	      | hb_filter ([&] (ثابت cff2_font_dict_اقدار_t &_)
		{ أرجع plan.fdmap.has (&_ - &fontDicts[0]); }),
	      hb_iter (نجی_dict_infos))
    ;
    إذا (unlikely (!fda->serialize (c, it, fontSzr)))
    {
      c->نکالو_discard ();
      أرجع خطأ;
    }
    plan.info.fd_array_link = c->نکالو_pack (خطأ);
  }

  /* variation store */
  إذا (varStore != &Null (CFF2_شے_Variation_Store) &&
      !plan.pinned)
  {
    إذا (plan.partial_instancing)
    {
      إذا (plan.instancing.has_variations ())
      {
	c->push ();
	إذا (unlikely (!plan.instancing.serialize_var_store (c, *plan.axis_ٹیگ_سوچی)))
	{
	  c->نکالو_discard ();
	  أرجع خطأ;
	}
	plan.info.var_store_link = c->نکالو_pack (خطأ);
      }
      /* else: the store emptied; the vstore op gets dropped along with it. */
    }
    وإلا
    {
      خودکار *dest = c->push<CFF2_شے_Variation_Store> ();
      إذا (unlikely (!dest->serialize (c, varStore)))
      {
	c->نکالو_discard ();
	أرجع خطأ;
      }
      plan.info.var_store_link = c->نکالو_pack (خطأ);
    }
  }

  OT::cff2 *cff2 = c->allocate_min<OT::cff2> ();
  إذا (unlikely (!cff2)) أرجع خطأ;

  /*سرخی*/
  cff2->نسخہ.major = 0x02;
  cff2->نسخہ.minor = 0x00;
  cff2->چوٹی_Dict_2 = OT::cff2::static_ناپ;

  /* top dict */
  {
    چوٹی_Dict &dict = cff2 + cff2->چوٹی_Dict_2;
    cff2_چوٹی_dict_op_serializer_t چوٹی_Szr;
    إذا (unlikely (!dict.serialize (c, چوٹی_Dict_2, چوٹی_Szr, plan.info))) أرجع خطأ;
    cff2->چوٹی_Dict_ناپ = c->سر_2 - (ثابت حرف *)&dict;
  }

  /* global subrs */
  {
    خودکار *dest = c->شروع_embed <CFF2Subrs> ();
    أرجع dest->serialize (c, plan.subset_globalsubrs);
  }
}

منطقي
OT::cff2::accelerator_subset_t::subset (hb_subset_context_t *c) ثابت
{
  cff2_subset_plan cff2_plan;

  إذا (unlikely (!cff2_plan.create (*هذا, c->plan))) أرجع خطأ;

  // If instantiating (pinned) and downgrade flag is set, convert to CFF1
  إذا (cff2_plan.pinned && (c->plan->flags & HB_SUBSET_FLAGS_DOWNGRADE_CFF2))
  {
    // Serialize CFF1 to the subsetter's serializer
    // If we run out of room, returning true will cause subsetter to retry with larger buffer
    منطقي نتیجہ = CFF::serialize_cff2_منزل_cff1 (c->serializer, cff2_plan, چوٹی_Dict_2, *هذا);

    إذا (c->serializer->ran_باہرلا_دا_گنجائش ())
      أرجع سچ; // Subsetter will retry with larger buffer

    إذا (نتیجہ && !c->serializer->وچ_error ())
    {
      // Success - end serialization to resolve links
      c->serializer->سرا_serialize ();

      // Copy the serialized CFF1 data and add as CFF table
      hb_blob_t *cff_blob = c->serializer->copy_blob ();
      إذا (cff_blob)
      {
        c->plan->جوڑو_جدول (HB_ٹیگ('C','F','F',' '), cff_blob);
        hb_blob_destroy (cff_blob);

        // Return false to signal CFF2 table is not needed
        أرجع خطأ;
      }
    }

    // Conversion failed - don't fall back, fail hard for debugging
    أرجع خطأ;
  }

  أرجع serialize (c->serializer, cff2_plan,
		    c->plan->normalized_coords.as_array ());
}

#جے_انت
