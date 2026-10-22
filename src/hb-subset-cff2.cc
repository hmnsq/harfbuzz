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

#اگر_تعریف_نشده HB_NO_SUBSET_CFF

#ضمن "hb-open-type.hh"
#ضمن "hb-ot-cff2-table.hh"
#ضمن "hb-ot-var-common.hh"
#ضمن "hb-set.h"
#ضمن "hb-subset-plan.hh"
#ضمن "hb-subset-cff-common.hh"
#ضمن "hb-cff2-interp-cs.hh"
#ضمن "hb-subset-cff2-to-cff1.hh"

استفاده نطاق CFF;

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
    hb_وکتور_t<hb_وکتور_t<عائم>> ماتریس;
    /* Default-value fold-in scalars, one per old region: the old store's
     * region scalars at the new default location. */
    hb_وکتور_t<عائم> gains;
    /* Surviving regions, parallel to matrix rows. */
    hb_وکتور_t<hb_hashmap_t<hb_برچسب_t, Triple>> regions;
    /* Index into the new global region list, per new region (finalize). */
    hb_وکتور_t<بیعلامت> region_indices;
    بیعلامت new_major = 0;
    منطقي alive_ = خطأ;
    منطقي alive () ثابت { أرجع alive_; }

    /* One value's delta for new region j, from its old per-region deltas. */
    مزدوج transform_delta (بیعلامت j, hb_array_t<ثابت عدد_t> deltas) ثابت
    {
      ثابت hb_وکتور_t<عائم> &ستون = ماتریس.arrayZ[j];
      مزدوج d = 0.;
      بیعلامت شمار = hb_min (ستون.درازی, deltas.درازی);
      لكل (بیعلامت c = 0; c < شمار; c++)
	d += (مزدوج) ستون.arrayZ[c] * deltas.arrayZ[c].مقصد_real ();
      أرجع d;
    }
  };

  hb_وکتور_t<vardata_transform_t> transforms;  /* per old VarData (vsindex) */
  hb_وکتور_t<hb_hashmap_t<hb_برچسب_t, Triple>> new_regions;  /* global region list */
  hb_مجموعه_t استفاده_شده_majors;
  بیعلامت new_major_شمار = 0;

  منطقي has_variations () ثابت { أرجع new_major_شمار; }

  ثابت vardata_transform_t *get_transform (بیعلامت major) ثابت
  { أرجع major < transforms.درازی ? &transforms.arrayZ[major] : اشاره_تهی; }

  /* Record that a retained blend references this VarData. */
  فراغ note_use (بیعلامت major) { استفاده_شده_majors.جمع_کن (major); }

  /* The new vsindex a private dict's (or charstring's) old ivs maps to;
   * dead VarDatas fall back to 0, their blends having become static. */
  بیعلامت remap_ivs (بیعلامت old_ivs) ثابت
  {
    ثابت vardata_transform_t *t = get_transform (old_ivs);
    أرجع (t && t->alive ()) ? t->new_major : 0;
  }

  مزدوج fold_deltas (بیعلامت old_ivs, hb_array_t<ثابت عدد_t> deltas) ثابت
  {
    ثابت vardata_transform_t *t = get_transform (old_ivs);
    إذا (unlikely (!t || t->gains.درازی != deltas.درازی)) أرجع 0.;
    مزدوج v = 0.;
    لكل (بیعلامت i = 0; i < deltas.درازی; i++)
      v += (مزدوج) t->gains.arrayZ[i] * deltas.arrayZ[i].مقصد_real ();
    أرجع v;
  }

  منطقي create (ثابت CFF2_قلم_Variation_Store *cff2VarStore,
	       ثابت hb_subset_plan_t *plan)
  {
    ثابت OT::قلم_Variation_Store &varStore = cff2VarStore->varStore;
    بیعلامت major_شمار = varStore.get_sub_جدول_شمار ();
    إذا (!transforms.resize (major_شمار)) أرجع خطأ;

    إذا (!varStore.get_region_list ().get_var_regions (plan->axes_old_شاخص_برچسب_map, orig_regions))
      أرجع خطأ;

    /* Instance each VarData with identity delta rows; the surviving tuples'
     * delta columns are the old-region → new-region transform. */
    OT::optimize_scratch_t scratch;
    لكل (بیعلامت m = 0; m < major_شمار; m++)
    {
      ثابت OT::Var_داده &var_داده = varStore.get_sub_جدول (m);
      بیعلامت k = var_داده.get_region_شاخص_شمار ();
      vardata_transform_t &t = transforms.arrayZ[m];

      إذا (!t.gains.resize_exact (k)) أرجع خطأ;
      varStore.get_region_scalars (m, plan->normalized_coords.arrayZ,
				   plan->normalized_coords.درازی,
				   t.gains.arrayZ, k);

      OT::tuple_variations_t tuples;
      لكل (بیعلامت r = 0; r < k; r++)
      {
	OT::tuple_delta_t tuple;
	إذا (!tuple.deltas_x.resize (k) ||
	    !tuple.indices.resize (k))
	  أرجع خطأ;
	لكل (بیعلامت i = 0; i < k; i++)
	  tuple.indices.arrayZ[i] = درست;
	tuple.deltas_x.arrayZ[r] = 1.f;
	بیعلامت region_شاخص = var_داده.get_region_شاخص (r);
	إذا (unlikely (region_شاخص >= orig_regions.درازی)) أرجع خطأ;
	tuple.axis_tuples = orig_regions.arrayZ[region_شاخص];
	إذا (unlikely (tuple.axis_tuples.درون_error ())) أرجع خطأ;
	tuples.tuple_vars.push (std::move (tuple));
      }
      إذا (unlikely (tuples.tuple_vars.درون_error ())) أرجع خطأ;

      إذا (!tuples.instantiate (plan->axes_location, plan->axes_triple_distances, scratch))
	أرجع خطأ;

      لكل (خودکار &tuple : tuples.tuple_vars)
      {
	إذا (unlikely (tuple.deltas_x.درازی != k)) أرجع خطأ;
	hb_وکتور_t<عائم> ستون;
	إذا (unlikely (!ستون.resize_exact (k))) أرجع خطأ;
	لكل (بیعلامت i = 0; i < k; i++)
	  ستون.arrayZ[i] = tuple.deltas_x.arrayZ[i];
	t.ماتریس.push (std::move (ستون));
	t.regions.push (std::move (tuple.axis_tuples));
      }
      إذا (unlikely (t.ماتریس.درون_error () || t.regions.درون_error ())) أرجع خطأ;
    }

    أرجع درست;
  }

  /* Decide which VarDatas survive, number them, and build the new global
   * region list: surviving original regions first, in their original order,
   * then solver-split new regions in order of appearance (fontTools'
   * rebuildRegions + prune_regions). */
  منطقي finalize ()
  {
    لكل (بیعلامت m = 0; m < transforms.درازی; m++)
    {
      vardata_transform_t &t = transforms.arrayZ[m];
      t.alive_ = t.ماتریس.درازی && استفاده_شده_majors.has (m);
      إذا (t.alive_)
	t.new_major = new_major_شمار++;
    }

    /* Keys hash by content, so pointers into different owners compare fine. */
    hb_hashmap_t<ثابت hb_hashmap_t<hb_برچسب_t, Triple>*, بیعلامت> region_idx_map;
    hb_وکتور_t<ثابت hb_hashmap_t<hb_برچسب_t, Triple>*> region_ptrs;
    لكل (ثابت vardata_transform_t &t : transforms)
    {
      إذا (!t.alive ()) استمر;
      لكل (ثابت خودکار &region : t.regions)
	إذا (!region_idx_map.has (&region))
	  إذا (unlikely (!region_idx_map.مجموعه (&region, (بیعلامت) -1)))
	    أرجع خطأ;
    }
    لكل (ثابت خودکار &r : orig_regions)
    {
      بیعلامت *idx;
      إذا (region_idx_map.has (&r, &idx) && *idx == (بیعلامت) -1)
      {
	إذا (unlikely (!region_idx_map.مجموعه (&r, region_ptrs.درازی))) أرجع خطأ;
	region_ptrs.push (&r);
      }
    }
    لكل (ثابت vardata_transform_t &t : transforms)
    {
      إذا (!t.alive ()) استمر;
      لكل (ثابت خودکار &region : t.regions)
      {
	بیعلامت *idx;
	إذا (region_idx_map.has (&region, &idx) && *idx == (بیعلامت) -1)
	{
	  إذا (unlikely (!region_idx_map.مجموعه (&region, region_ptrs.درازی))) أرجع خطأ;
	  region_ptrs.push (&region);
	}
      }
    }
    إذا (unlikely (region_ptrs.درون_error ())) أرجع خطأ;

    لكل (vardata_transform_t &t : transforms)
    {
      إذا (!t.alive ()) استمر;
      لكل (ثابت خودکار &region : t.regions)
      {
	بیعلامت *idx;
	إذا (unlikely (!region_idx_map.has (&region, &idx))) أرجع خطأ;
	t.region_indices.push (*idx);
      }
      إذا (unlikely (t.region_indices.درون_error ())) أرجع خطأ;
    }

    إذا (!new_regions.resize (region_ptrs.درازی)) أرجع خطأ;
    لكل (بیعلامت i = 0; i < region_ptrs.درازی; i++)
    {
      new_regions.arrayZ[i] = *region_ptrs.arrayZ[i];
      إذا (unlikely (new_regions.arrayZ[i].درون_error ())) أرجع خطأ;
    }

    أرجع درست;
  }

  /* Serialize the instanced store: a CFF2ItemVariationStore whose VarDatas
   * carry no items, just region indexes (blend deltas live in charstrings).
   * Laid out manually since all sizes are known up front. */
  منطقي serialize_var_store (hb_serialize_context_t *c,
			    ثابت hb_وکتور_t<hb_برچسب_t> &axis_برچسبها) ثابت
  {
    بیعلامت داده_شمار = new_major_شمار;
    بیعلامت سرآیند_اندازه = 2 + 4 + 2 + 4 * داده_شمار;

    uint64_t جابه_جایی = سرآیند_اندازه;
    hb_وکتور_t<بیعلامت> داده_offsets;
    لكل (ثابت vardata_transform_t &t : transforms)
    {
      إذا (!t.alive ()) استمر;
      داده_offsets.push ((بیعلامت) جابه_جایی);
      جابه_جایی += 6 + 2 * (uint64_t) t.region_indices.درازی;
    }
    إذا (unlikely (داده_offsets.درون_error () || داده_offsets.درازی != داده_شمار))
      أرجع خطأ;
    بیعلامت regions_جابه_جایی = (بیعلامت) جابه_جایی;
    uint64_t مجموع = جابه_جایی + 4 + (uint64_t) new_regions.درازی * axis_برچسبها.درازی * 6;
    إذا (unlikely (مجموع > 0xFFFFFFFFu)) أرجع خطأ;

    HBUINT16 *اندازه_فیلد = c->allocate_اندازه<HBUINT16> (HBUINT16::static_اندازه);
    إذا (unlikely (!اندازه_فیلد)) أرجع خطأ;
    /* OFF 2026 5.3.3.11.2 permits larger stores with length=65535. */
    *اندازه_فیلد = hb_min (مجموع, (uint64_t) 0xFFFFu);

    حرف *سرآیند = c->allocate_اندازه<حرف> (سرآیند_اندازه);
    إذا (unlikely (!سرآیند)) أرجع خطأ;
    * (HBUINT16 *) (سرآیند + 0) = 1;			/* format */
    * (HBUINT32 *) (سرآیند + 2) = regions_جابه_جایی;
    * (HBUINT16 *) (سرآیند + 6) = داده_شمار;
    لكل (بیعلامت i = 0; i < داده_شمار; i++)
      * (HBUINT32 *) (سرآیند + 8 + 4 * i) = داده_offsets.arrayZ[i];

    لكل (ثابت vardata_transform_t &t : transforms)
    {
      إذا (!t.alive ()) استمر;
      بیعلامت k = t.region_indices.درازی;
      HBUINT16 *v = (HBUINT16 *) c->allocate_اندازه<حرف> (6 + 2 * k);
      إذا (unlikely (!v)) أرجع خطأ;
      v[0] = 0;						/* itemCount */
      v[1] = 0;						/* wordSizeCount */
      v[2] = k;						/* regionIndices.len */
      لكل (بیعلامت j = 0; j < k; j++)
	v[3 + j] = t.region_indices.arrayZ[j];
    }

    hb_وکتور_t<ثابت hb_hashmap_t<hb_برچسب_t, Triple>*> region_ptrs;
    إذا (unlikely (!region_ptrs.alloc_exact (new_regions.درازی))) أرجع خطأ;
    لكل (ثابت خودکار &r : new_regions)
      region_ptrs.push (&r);
    خودکار *region_list = c->شروع_embed<OT::VarRegionList> ();
    أرجع region_list->serialize (c, axis_برچسبها, region_ptrs);
  }

  خاص:
  hb_وکتور_t<hb_hashmap_t<hb_برچسب_t, Triple>> orig_regions;
};

/* Usage-collection pass for partial instancing: record which VarDatas the
 * retained charstrings' and private dicts' blends reference.  Mirrors the
 * emission pass: blends consumed by dropped hint ops don't count. */
بنية cff2_usage_param_t
{
  فراغ init () {}

  cff2_instancing_plan_t *instancing = اشاره_تهی;
  منطقي drop_hints = خطأ;
  بیعلامت ivs = 0;	/* dict-side current vsindex */
};

بنية cff2_cs_opset_usage_t : cff2_cs_opset_t<cff2_cs_opset_usage_t, cff2_usage_param_t, blend_arg_t>
{
  ساكن فراغ flush_آرگومانها_and_op (op_کد_t op, cff2_cs_interp_env_t<blend_arg_t> &env, cff2_usage_param_t& param)
  {
    منطقي counts = درست;
    بدل (op)
    {
      حالة Op_کد_return:
      حالة Op_کد_endchar:
	counts = خطأ;
	اكسر;

      حالة Op_کد_hstem:
      حالة Op_کد_hstemhm:
      حالة Op_کد_vstem:
      حالة Op_کد_vstemhm:
      حالة Op_کد_hintmask:
      حالة Op_کد_cntrmask:
	إذا (param.drop_hints)
	  counts = خطأ;
	اكسر;

      افتراضي:
	اكسر;
    }

    إذا (counts)
      لكل (بیعلامت صحيح i = 0; i < env.arg_پشته.get_شمار (); i++)
	إذا (env.arg_پشته[i].blending ())
	{
	  param.instancing->note_use (env.get_ivs ());
	  اكسر;
	}

    env.clear_آرگومانها ();
  }
};

بنية cff2_خصوصی_dict_usage_opset_t : dict_opset_t
{
  ساكن فراغ process_op (op_کد_t op, cff2_priv_dict_interp_env_t& env, cff2_usage_param_t& param)
  {
    بدل (op) {
      حالة Op_کد_vsindexdict:
	env.process_vsindex ();
	param.ivs = env.get_ivs ();
	env.clear_آرگومانها ();
	أرجع;
      حالة Op_کد_blenddict:
	param.instancing->note_use (param.ivs);
	env.clear_آرگومانها ();
	أرجع;
      افتراضي:
	dict_opset_t::process_op (op, env);
	إذا (!env.arg_پشته.is_empty ()) أرجع;
	env.clear_آرگومانها ();
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

بنية cff2_بالاترین_dict_op_serializer_t : cff_بالاترین_dict_op_serializer_t<>
{
  منطقي serialize (hb_serialize_context_t *c,
		  ثابت op_str_t &opstr,
		  ثابت cff2_sub_جدول_info_t &info) ثابت
  {
    TRACE_SERIALIZE (هذا);

    بدل (opstr.op)
    {
      حالة Op_کد_vstore:
        إذا (info.var_store_link)
	  return_trace (FontDict::serialize_link4_op(c, opstr.op, info.var_store_link));
	وإلا
	  return_trace (درست);

      افتراضي:
	return_trace (cff_بالاترین_dict_op_serializer_t<>::serialize (c, opstr, info));
    }
  }
};

بنية cff2_cs_opset_flatten_t : cff2_cs_opset_t<cff2_cs_opset_flatten_t, flatten_param_t, blend_arg_t>
{
  ساكن فراغ flush_آرگومانها_and_op (op_کد_t op, cff2_cs_interp_env_t<blend_arg_t> &env, flatten_param_t& param)
  {
    /* Optionally capture command for specialization (before flushing, to preserve args) */
    إذا (param.commands)
    {
      منطقي skip_command = خطأ;

      بدل (op)
      {
	حالة Op_کد_return:
	حالة Op_کد_endchar:
	  skip_command = درست;
	  اكسر;

	حالة Op_کد_hstem:
	حالة Op_کد_hstemhm:
	حالة Op_کد_vstem:
	حالة Op_کد_vstemhm:
	حالة Op_کد_hintmask:
	حالة Op_کد_cntrmask:
	  إذا (param.drop_hints)
	    skip_command = درست;
	  اكسر;

	افتراضي:
	  اكسر;
      }

      إذا (!skip_command)
      {
	cs_command_t cmd (op);
	/* Capture resolved blend values */
	لكل (بیعلامت صحيح i = 0; i < env.arg_پشته.get_شمار ();)
	{
	  ثابت blend_arg_t &arg = env.arg_پشته[i];
	  إذا (arg.blending ())
	  {
	    /* For blend args, capture only the resolved default value */
	    cmd.آرگومانها.push (arg);
	    /* Skip over the multiple blend values */
	    i += arg.رقم_مقادیر_2;
	  }
	  وإلا
	  {
	    cmd.آرگومانها.push (arg);
	    i++;
	  }
	}
	param.commands->push (cmd);
      }
    }

    بدل (op)
    {
      حالة Op_کد_return:
      حالة Op_کد_endchar:
	/* dummy opcodes in CFF2. ignore */
	إذا (op == Op_کد_endchar && param.instancer)
	  emit_vsindex_پیشوند (env, param);
	اكسر;

      حالة Op_کد_hstem:
      حالة Op_کد_hstemhm:
      حالة Op_کد_vstem:
      حالة Op_کد_vstemhm:
      حالة Op_کد_hintmask:
      حالة Op_کد_cntrmask:
	إذا (param.drop_hints)
	{
	  env.clear_آرگومانها ();
	  أرجع;
	}
	HB_FALLTHROUGH;

      افتراضي:
	SUPER::flush_آرگومانها_and_op (op, env, param);
	اكسر;
    }
  }

  ساكن فراغ flush_آرگومانها (cff2_cs_interp_env_t<blend_arg_t> &env, flatten_param_t& param)
  {
    لكل (بیعلامت صحيح i = 0; i < env.arg_پشته.get_شمار ();)
    {
      ثابت blend_arg_t &arg = env.arg_پشته[i];
      إذا (arg.blending ())
      {
	إذا (unlikely (!((arg.رقم_مقادیر_2 > 0) && (env.arg_پشته.get_شمار () >= arg.رقم_مقادیر_2))))
	{
	  env.مجموعه_error ();
	  أرجع;
	}
	إذا (param.instancer)
	  rewrite_blends (arg, i, env, param);
	وإلا
	  flatten_blends (arg, i, env, param);
	i += arg.رقم_مقادیر_2;
      }
      وإلا
      {
	str_encoder_t  encoder (param.flatStr);
	encoder.encode_رقم_cs (arg);
	i++;
      }
    }
    SUPER::flush_آرگومانها (env, param);
  }

  ساكن فراغ flatten_blends (ثابت blend_arg_t &arg, بیعلامت صحيح i, cff2_cs_interp_env_t<blend_arg_t> &env, flatten_param_t& param)
  {
    /* flatten the default values */
    str_encoder_t  encoder (param.flatStr);
    لكل (بیعلامت صحيح j = 0; j < arg.رقم_مقادیر_2; j++)
    {
      ثابت blend_arg_t &arg1 = env.arg_پشته[i + j];
      إذا (unlikely (!((arg1.blending () && (arg.رقم_مقادیر_2 == arg1.رقم_مقادیر_2) && (arg1.مقدار_شاخص_2 == j) &&
	      (arg1.deltas.درازی == env.get_region_شمار ())))))
      {
	env.مجموعه_error ();
	أرجع;
      }
      encoder.encode_رقم_cs (arg1);
    }
    /* flatten deltas for each value */
    لكل (بیعلامت صحيح j = 0; j < arg.رقم_مقادیر_2; j++)
    {
      ثابت blend_arg_t &arg1 = env.arg_پشته[i + j];
      لكل (بیعلامت صحيح k = 0; k < arg1.deltas.درازی; k++)
	encoder.encode_رقم_cs (arg1.deltas[k]);
    }
    /* flatten the number of values followed by blend operator */
    encoder.encode_int (arg.رقم_مقادیر_2);
    encoder.encode_op (Op_کد_blendcs);
  }

  /* Partial instancing: rewrite one blend group against the instanced store.
   * Each value's default absorbs the folded (pinned-away) contribution, and
   * its deltas are mapped onto the new region list. */
  ساكن فراغ rewrite_blends (ثابت blend_arg_t &arg, بیعلامت صحيح i, cff2_cs_interp_env_t<blend_arg_t> &env, flatten_param_t& param)
  {
    ثابت cff2_instancing_plan_t &instancer = *param.instancer;
    بیعلامت ivs = env.get_ivs ();
    ثابت cff2_instancing_plan_t::vardata_transform_t *t = instancer.get_transform (ivs);
    إذا (unlikely (!t))
    {
      env.مجموعه_error ();
      أرجع;
    }

    بیعلامت n = arg.رقم_مقادیر_2;
    لكل (بیعلامت صحيح j = 0; j < n; j++)
    {
      ثابت blend_arg_t &arg1 = env.arg_پشته[i + j];
      إذا (unlikely (!(arg1.blending () && (arg1.رقم_مقادیر_2 == n) && (arg1.مقدار_شاخص_2 == j) &&
		      (arg1.deltas.درازی == env.get_region_شمار ()) &&
		      (arg1.deltas.درازی == t->gains.درازی))))
      {
	env.مجموعه_error ();
	أرجع;
      }
    }

    str_encoder_t encoder (param.flatStr);
    بیعلامت k_new = t->alive () ? t->ماتریس.درازی : 0;

    إذا (!k_new)
    {
      /* All of this VarData's regions died; blends dissolve into statics. */
      لكل (بیعلامت صحيح j = 0; j < n; j++)
      {
	ثابت blend_arg_t &arg1 = env.arg_پشته[i + j];
	عدد_t v;
	v.مجموعه_real (arg1.مقصد_real () + round (instancer.fold_deltas (ivs, arg1.deltas.as_array ())));
	encoder.encode_رقم_cs (v);
      }
      أرجع;
    }

    /* Emit in chunks so the output charstring stays within the argument
     * stack limit: values already emitted for this op, plus this chunk's
     * defaults, deltas and count, must not exceed 513. */
    بیعلامت انجامشده = 0;
    طالما (انجامشده < n)
    {
      بیعلامت avail = 512 > (i + انجامشده) ? 512 - (i + انجامشده) : 0;
      بیعلامت chunk = hb_min (avail / (k_new + 1), n - انجامشده);
      إذا (unlikely (!chunk))
      {
	env.مجموعه_error ();
	أرجع;
      }

      لكل (بیعلامت صحيح j = انجامشده; j < انجامشده + chunk; j++)
      {
	ثابت blend_arg_t &arg1 = env.arg_پشته[i + j];
	عدد_t v;
	v.مجموعه_real (arg1.مقصد_real () + round (instancer.fold_deltas (ivs, arg1.deltas.as_array ())));
	encoder.encode_رقم_cs (v);
      }
      لكل (بیعلامت صحيح j = انجامشده; j < انجامشده + chunk; j++)
      {
	ثابت blend_arg_t &arg1 = env.arg_پشته[i + j];
	لكل (بیعلامت r = 0; r < k_new; r++)
	{
	  عدد_t v;
	  v.مجموعه_int (hb_clamp_مقصد<صحيح> (round (t->transform_delta (r, arg1.deltas.as_array ()))));
	  encoder.encode_رقم_cs (v);
	}
      }
      encoder.encode_int (chunk);
      encoder.encode_op (Op_کد_blendcs);
      انجامشده += chunk;
    }
    param.emitted_blend = درست;
  }

  /* Partial instancing: prepend a vsindex op when the charstring's
   * (remapped) ivs differs from its FD's (remapped) private-dict ivs. */
  ساكن فراغ emit_vsindex_پیشوند (cff2_cs_interp_env_t<blend_arg_t> &env, flatten_param_t& param)
  {
    إذا (!param.emitted_blend) أرجع;
    ثابت cff2_instancing_plan_t &instancer = *param.instancer;
    بیعلامت new_ivs = instancer.remap_ivs (env.get_ivs ());
    بیعلامت fd_ivs = instancer.remap_ivs (env.get_orig_ivs ());
    إذا (new_ivs == fd_ivs) أرجع;

    str_buff_t پیشوند;
    str_encoder_t enc (پیشوند);
    enc.encode_int ((صحيح) new_ivs);
    enc.encode_op (Op_کد_vsindexcs);

    str_buff_t &buf = param.flatStr;
    بیعلامت old_len = buf.درازی;
    إذا (unlikely (پیشوند.درون_error () || !buf.resize (old_len + پیشوند.درازی)))
    {
      env.مجموعه_error ();
      أرجع;
    }
    memmove (buf.arrayZ + پیشوند.درازی, buf.arrayZ, old_len);
    hb_memcpy (buf.arrayZ, پیشوند.arrayZ, پیشوند.درازی);
  }

  ساكن فراغ flush_op (op_کد_t op, cff2_cs_interp_env_t<blend_arg_t> &env, flatten_param_t& param)
  {
    بدل (op)
    {
      حالة Op_کد_return:
      حالة Op_کد_endchar:
	أرجع;
      افتراضي:
	str_encoder_t  encoder (param.flatStr);
	encoder.encode_op (op);
    }
  }

  ساكن فراغ flush_hintmask (op_کد_t op, cff2_cs_interp_env_t<blend_arg_t> &env, flatten_param_t& param)
  {
    SUPER::flush_hintmask (op, env, param);
    /* Preserve hintmask payload in captured commands for specializer re-encoding. */
    إذا (param.commands && !param.drop_hints && param.commands->درازی > 0)
    {
      خودکار &cmd = param.commands->دم ();
      إذا (cmd.op == op)
      {
        cmd.ماسک_بایتها.resize (env.hintmask_اندازه);
        إذا (unlikely (cmd.ماسک_بایتها.درون_error ()))
        {
          env.مجموعه_error ();
          أرجع;
        }
        لكل (بیعلامت صحيح i = 0; i < env.hintmask_اندازه; i++)
          cmd.ماسک_بایتها[i] = env.str_ref[i];
      }
    }
    إذا (!param.drop_hints)
    {
      str_encoder_t  encoder (param.flatStr);
      لكل (بیعلامت صحيح i = 0; i < env.hintmask_اندازه; i++)
	encoder.encode_byte (env.str_ref[i]);
    }
  }

  خاص:
  تعريفنوع cff2_cs_opset_t<cff2_cs_opset_flatten_t, flatten_param_t, blend_arg_t> SUPER;
  تعريفنوع cs_opset_t<blend_arg_t, cff2_cs_opset_flatten_t, cff2_cs_opset_flatten_t, cff2_cs_interp_env_t<blend_arg_t>, flatten_param_t> CSOPSET;
};

بنية cff2_cs_opset_subr_subset_t : cff2_cs_opset_t<cff2_cs_opset_subr_subset_t, subr_subset_param_t, blend_arg_t>
{
  ساكن فراغ process_op (op_کد_t op, cff2_cs_interp_env_t<blend_arg_t> &env, subr_subset_param_t& param)
  {
    بدل (op) {

      حالة Op_کد_return:
	param.current_parsed_str->flush_coalesced (env.str_ref);
	param.current_parsed_str->مجموعه_parsed ();
	env.return_from_subr ();
	param.مجموعه_current_str (env, خطأ);
	اكسر;

      حالة Op_کد_endchar:
	param.current_parsed_str->flush_coalesced (env.str_ref);
	param.current_parsed_str->مجموعه_parsed ();
	SUPER::process_op (op, env, param);
	اكسر;

      حالة Op_کد_callsubr:
	process_call_subr (op, CSType_LocalSubr, env, param, env.localSubrs, param.local_بستگی_2);
	اكسر;

      حالة Op_کد_callgsubr:
	process_call_subr (op, CSType_GlobalSubr, env, param, env.globalSubrs, param.global_بستگی);
	اكسر;

      افتراضي:
	SUPER::process_op (op, env, param);
	param.current_parsed_str->جمع_کن_op (op, env.str_ref);
	اكسر;
    }
  }

  محمي:
  ساكن فراغ process_call_subr (op_کد_t op, cs_type_t type,
				 cff2_cs_interp_env_t<blend_arg_t> &env, subr_subset_param_t& param,
				 cff2_biased_subrs_t& subrs, hb_مجموعه_t *بستگی)
  {
    byte_str_ref_t    str_ref = env.str_ref;
    env.call_subr (subrs, type);
    param.current_parsed_str->جمع_کن_call_op (op, str_ref, env.context.subr_رقم_2);
    بستگی->جمع_کن (env.context.subr_رقم_2);
    param.مجموعه_current_str (env, درست);
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
    إذا (env.دیده_vsindex ())
    {
      عدد_t  ivs;
      ivs.مجموعه_int ((صحيح)env.get_ivs ());
      charstring.مجموعه_پیشوند (ivs, Op_کد_vsindexcs);
    }
  }
};

بنية cff2_خصوصی_blend_encoder_param_t
{
  cff2_خصوصی_blend_encoder_param_t (hb_serialize_context_t *c,
				      ثابت CFF2_قلم_Variation_Store *varStore,
				      hb_array_t<صحيح> normalized_coords) :
    c (c), varStore (varStore), normalized_coords (normalized_coords) {}

  فراغ init () {}

  فراغ process_blend ()
  {
    إذا (!دیده_blend)
    {
      region_شمار_2 = varStore->varStore.get_region_شاخص_شمار (ivs);
      إذا (unlikely (!scalars.resize_exact (region_شمار_2)))
      {
	إذا (c) c->err (HB_SERIALIZE_ERROR_OTHER);
	دیده_blend = درست;
	أرجع;
      }
      varStore->varStore.get_region_scalars (ivs, normalized_coords.arrayZ, normalized_coords.درازی,
					     &scalars[0], region_شمار_2);
      دیده_blend = درست;
    }
  }

  مزدوج blend_deltas (hb_array_t<ثابت عدد_t> deltas) ثابت
  {
    مزدوج v = 0;
    إذا (likely (scalars.درازی == deltas.درازی))
    {
      بیعلامت شمار = scalars.درازی;
      لكل (بیعلامت i = 0; i < شمار; i++)
	v += (مزدوج) scalars.arrayZ[i] * deltas.arrayZ[i].مقصد_real ();
    }
    أرجع v;
  }


  hb_serialize_context_t *c = اشاره_تهی;
  منطقي دیده_blend = خطأ;
  بیعلامت ivs = 0;
  بیعلامت region_شمار_2 = 0;
  hb_وکتور_t<عائم> scalars;
  ثابت	 CFF2_قلم_Variation_Store *varStore = اشاره_تهی;
  hb_array_t<صحيح> normalized_coords;

  /* CFF2 partial instancing: when set, blends are rewritten against the
   * instanced variation store instead of being flattened. */
  ثابت cff2_instancing_plan_t *instancer = اشاره_تهی;
  /* Error-compensated fold rounding state, per dict operator. */
  مزدوج fold_exact = 0.;
  مزدوج fold_emitted = 0.;
};

بنية cff2_خصوصی_dict_blend_opset_t : dict_opset_t
{
  ساكن فراغ process_arg_blend (cff2_خصوصی_blend_encoder_param_t& param,
				 عدد_t &arg,
				 ثابت hb_array_t<ثابت عدد_t> blends,
				 بیعلامت n, بیعلامت i)
  {
    arg.مجموعه_int (round (arg.مقصد_real () + param.blend_deltas (blends)));
  }

  /* The fold added to a value, error-compensated across the operator: most
   * blended private-dict values (BlueValues, StemSnap*, ...) are
   * delta-encoded, so independently rounded folds would accumulate in the
   * decoded absolutes; keeping the emitted cumulative fold equal to the
   * rounded exact cumulative fold bounds every absolute within ±0.5. */
  ساكن مزدوج fold (cff2_خصوصی_blend_encoder_param_t& param,
		      ثابت hb_array_t<ثابت عدد_t> blends)
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
			      cff2_خصوصی_blend_encoder_param_t& param,
			      بیعلامت شروع, بیعلامت n, بیعلامت k)
  {
    ثابت cff2_instancing_plan_t &instancer = *param.instancer;
    ثابت cff2_instancing_plan_t::vardata_transform_t *t = instancer.get_transform (param.ivs);
    إذا (unlikely (!t || t->gains.درازی != k))
    {
      env.مجموعه_error ();
      أرجع;
    }

    str_buff_t str;
    str_encoder_t encoder (str);

    لكل (بیعلامت idx = 0; idx < شروع; idx++)
      encoder.encode_رقم_tp (env.arg_پشته[idx]);

    بیعلامت k_new = t->alive () ? t->ماتریس.درازی : 0;
    إذا (!k_new)
    {
      /* This VarData died (or is unused by any retained charstring, which
       * can happen for FDs kept only for retain-gids holes); blends
       * dissolve into statics. */
      لكل (بیعلامت j = 0; j < n; j++)
      {
	ثابت hb_array_t<ثابت عدد_t> blends = env.arg_پشته.sub_array (شروع + n + (j * k), k);
	عدد_t v;
	v.مجموعه_real (env.arg_پشته[شروع + j].مقصد_real () + fold (param, blends));
	encoder.encode_رقم_tp (v);
      }
    }
    وإلا
    {
      بیعلامت انجامشده = 0;
      طالما (انجامشده < n)
      {
	بیعلامت avail = 512 > (شروع + انجامشده) ? 512 - (شروع + انجامشده) : 0;
	بیعلامت chunk = hb_min (avail / (k_new + 1), n - انجامشده);
	إذا (unlikely (!chunk))
	{
	  env.مجموعه_error ();
	  أرجع;
	}

	لكل (بیعلامت j = انجامشده; j < انجامشده + chunk; j++)
	{
	  ثابت hb_array_t<ثابت عدد_t> blends = env.arg_پشته.sub_array (شروع + n + (j * k), k);
	  عدد_t v;
	  v.مجموعه_real (env.arg_پشته[شروع + j].مقصد_real () + fold (param, blends));
	  encoder.encode_رقم_tp (v);
	}
	لكل (بیعلامت j = انجامشده; j < انجامشده + chunk; j++)
	{
	  ثابت hb_array_t<ثابت عدد_t> blends = env.arg_پشته.sub_array (شروع + n + (j * k), k);
	  لكل (بیعلامت r = 0; r < k_new; r++)
	  {
	    عدد_t v;
	    v.مجموعه_int (hb_clamp_مقصد<صحيح> (round (t->transform_delta (r, blends))));
	    encoder.encode_رقم_tp (v);
	  }
	}
	encoder.encode_int (chunk);
	encoder.encode_op (Op_کد_blenddict);
	انجامشده += chunk;
      }
    }

    خودکار بایتها = str.as_بایتها ();
    param.c->embed (&بایتها, بایتها.درازی);

    env.clear_آرگومانها ();
  }

  ساكن فراغ process_blend (cff2_priv_dict_interp_env_t& env, cff2_خصوصی_blend_encoder_param_t& param)
  {
    بیعلامت صحيح n, k;

    param.process_blend ();
    k = param.region_شمار_2;
    n = env.arg_پشته.برداشت_uint ();

    بیعلامت شمار = env.arg_پشته.get_شمار ();
    بیعلامت صحيح مجموع;
    إذا (unlikely (hb_unsigned_mul_overflows (k + 1, n, &مجموع) || مجموع > شمار))
    {
      env.مجموعه_error ();
      أرجع;
    }

    /* copy the blend values into blend array of the default values */
    بیعلامت صحيح شروع = شمار - مجموع;
    /* let an obvious error case fail, but note CFF2 spec doesn't forbid n==0 */
    إذا (unlikely (شروع > شمار))
    {
      env.مجموعه_error ();
      أرجع;
    }

    إذا (param.instancer && k)
    {
      rewrite_blends (env, param, شروع, n, k);
      أرجع;
    }

    لكل (بیعلامت صحيح i = 0; i < n; i++)
    {
      ثابت hb_array_t<ثابت عدد_t> blends = env.arg_پشته.sub_array (شروع + n + (i * k), k);
      process_arg_blend (param, env.arg_پشته[شروع + i], blends, n, i);
    }

    /* pop off blend values leaving default values now adorned with blend values */
    env.arg_پشته.برداشت (k * n);
  }

  ساكن فراغ process_op (op_کد_t op, cff2_priv_dict_interp_env_t& env, cff2_خصوصی_blend_encoder_param_t& param)
  {
    بدل (op) {
      حالة Op_کد_Std_HW:
      حالة Op_کد_Std_VW:
      حالة Op_کد_Blue_Scale:
      حالة Op_کد_Blue_لغزش:
      حالة Op_کد_Blue_Fuzz:
      حالة Op_کد_Expansion_ضریب:
      حالة Op_کد_Language_گروه:
      حالة Op_کد_Blue_مقادیر:
      حالة Op_کد_Other_Blues:
      حالة Op_کد_Family_Blues:
      حالة Op_کد_Family_Other_Blues:
      حالة Op_کد_Stem_Snap_H:
      حالة Op_کد_Stem_Snap_V:
	اكسر;
      حالة Op_کد_vsindexdict:
	env.process_vsindex ();
	param.ivs = env.get_ivs ();
	إذا (param.instancer)
	{
	  /* Re-emit with the remapped vsindex; 0 is the dict default. */
	  بیعلامت new_ivs = param.instancer->remap_ivs (param.ivs);
	  إذا (new_ivs)
	  {
	    str_buff_t str;
	    str_encoder_t encoder (str);
	    encoder.encode_int ((صحيح) new_ivs);
	    encoder.encode_op (Op_کد_vsindexdict);
	    خودکار بایتها = str.as_بایتها ();
	    param.c->embed (&بایتها, بایتها.درازی);
	  }
	}
	env.clear_آرگومانها ();
	أرجع;
      حالة Op_کد_blenddict:
	process_blend (env, param);
	أرجع;

      افتراضي:
	dict_opset_t::process_op (op, env);
	إذا (!env.arg_پشته.is_empty ()) أرجع;
	اكسر;
    }

    إذا (unlikely (env.درون_error ())) أرجع;

    // Write args then op

    str_buff_t str;
    str_encoder_t encoder (str);

    بیعلامت شمار = env.arg_پشته.get_شمار ();
    لكل (بیعلامت i = 0; i < شمار; i++)
      encoder.encode_رقم_tp (env.arg_پشته[i]);

    encoder.encode_op (op);

    خودکار بایتها = str.as_بایتها ();
    param.c->embed (&بایتها, بایتها.درازی);

    env.clear_آرگومانها ();
    param.fold_exact = param.fold_emitted = 0.;
  }
};

بنية cff2_خصوصی_dict_op_serializer_t : op_serializer_t
{
  cff2_خصوصی_dict_op_serializer_t (منطقي desubroutinize_, منطقي drop_hints_, منطقي pinned_,
				     ثابت CFF::CFF2_قلم_Variation_Store* varStore_,
				     hb_array_t<صحيح> normalized_coords_,
				     ثابت cff2_instancing_plan_t *instancer_ = اشاره_تهی)
    : desubroutinize (desubroutinize_), drop_hints (drop_hints_), pinned (pinned_),
      param (اشاره_تهی, varStore_, normalized_coords_)
  { param.instancer = instancer_; }

  منطقي serialize (hb_serialize_context_t *c,
		  ثابت op_str_t &opstr,
		  objidx_t subrs_link) ثابت
  {
    TRACE_SERIALIZE (هذا);

    إذا (drop_hints && dict_opset_t::is_hint_op (opstr.op))
      return_trace (درست);

    إذا (opstr.op == Op_کد_Subrs)
    {
      إذا (desubroutinize || !subrs_link)
	return_trace (درست);
      وإلا
	return_trace (FontDict::serialize_link2_op (c, opstr.op, subrs_link));
    }

    إذا (pinned || param.instancer)
    {
      /* Reinterpret opstr and process blends.  The param persists across
       * this dict's opstrs, so that an ivs set by a vsindexdict opstr is
       * still in effect when a later opstr's blends are processed. */
      cff2_priv_dict_interp_env_t env {hb_ubytes_t (opstr.ptr, opstr.درازی)};
      param.c = c;
      dict_interpreter_t<cff2_خصوصی_dict_blend_opset_t, cff2_خصوصی_blend_encoder_param_t, cff2_priv_dict_interp_env_t> interp (env);
      return_trace (interp.interpret (param));
    }

    return_trace (copy_opstr (c, opstr));
  }

  محمي:
  ثابت منطقي desubroutinize;
  ثابت منطقي drop_hints;
  ثابت منطقي pinned;
  تغییرپذیر cff2_خصوصی_blend_encoder_param_t param;
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

    رقم_glyphs = plan->رقم_output_glyphs ();
    orig_fdcount = acc.fdArray->شمار;

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
			 acc.varStore != &Null (CFF2_قلم_Variation_Store);
    إذا (partial_instancing &&
	unlikely (!instancing.create (acc.varStore, plan)))
      أرجع خطأ;
    axis_برچسبها = &plan->axis_برچسبها;

    /* Enable command capture for CFF2→CFF1 conversion (for specialization) */
    capture_commands = pinned;

 #اگر_تعریف_شده HB_EXPERIMENTAL_API
    min_charstrings_off_اندازه = (plan->flags & HB_SUBSET_FLAGS_IFTB_REQUIREMENTS) ? 4 : 0;
 #وإلا
    min_charstrings_off_اندازه = 0;
 #پایان_اگر

    إذا (desubroutinize)
    {
      إذا (partial_instancing)
      {
	/* Pass 1: collect which VarDatas the retained blends reference, so
	 * unused ones get pruned and the survivors renumbered. */
	hb_مجموعه_t استفاده_شده_fds;
	لكل (بیعلامت صحيح i = 0; i < رقم_glyphs; i++)
	{
	  hb_codepoint_t glyph;
	  إذا (!plan->old_gid_for_new_gid (i, &glyph))
	    استمر;
	  ثابت hb_ubytes_t str = (*acc.char_رشتهها)[glyph];
	  بیعلامت صحيح fd = CFF::subset_get_fd (acc, glyph);
	  إذا (unlikely (fd >= acc.fd_شمار))
	    أرجع خطأ;
	  استفاده_شده_fds.جمع_کن (fd);

	  cff2_cs_interp_env_t<blend_arg_t> env (str, acc, fd);
	  cs_interpreter_t<cff2_cs_interp_env_t<blend_arg_t>, cff2_cs_opset_usage_t, cff2_usage_param_t> interp (env);
	  cff2_usage_param_t param;
	  param.instancing = &instancing;
	  param.drop_hints = drop_hints;
	  إذا (unlikely (!interp.interpret (param)))
	    أرجع خطأ;
	}
	لكل (بیعلامت fd : استفاده_شده_fds)
	{
	  cff2_usage_param_t param;
	  param.instancing = &instancing;
	  بیعلامت شمار = acc.خصوصی_Dicts[fd].get_شمار ();
	  لكل (بیعلامت j = 0; j < شمار; j++)
	  {
	    ثابت op_str_t &opstr = acc.خصوصی_Dicts[fd][j];
	    إذا (drop_hints && dict_opset_t::is_hint_op (opstr.op))
	      استمر;
	    cff2_priv_dict_interp_env_t env {hb_ubytes_t (opstr.ptr, opstr.درازی)};
	    dict_interpreter_t<cff2_خصوصی_dict_usage_opset_t, cff2_usage_param_t, cff2_priv_dict_interp_env_t> interp (env);
	    إذا (unlikely (!interp.interpret (param)))
	      أرجع خطأ;
	  }
	}
	إذا (unlikely (!instancing.finalize ()))
	  أرجع خطأ;

	/* Pass 2: like subr_flattener_t, but blends stay symbolic (no coords
	 * fed to the env) and get rewritten against the instanced store. */
	إذا (!subset_charstrings.resize_exact (رقم_glyphs))
	  أرجع خطأ;
	لكل (بیعلامت صحيح i = 0; i < رقم_glyphs; i++)
	{
	  hb_codepoint_t glyph;
	  إذا (!plan->old_gid_for_new_gid (i, &glyph))
	    استمر;
	  ثابت hb_ubytes_t str = (*acc.char_رشتهها)[glyph];
	  بیعلامت صحيح fd = CFF::subset_get_fd (acc, glyph);
	  إذا (unlikely (fd >= acc.fd_شمار))
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
	إذا (!charstring_commands.resize_exact (رقم_glyphs))
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
      لكل (بیعلامت صحيح fd = 0; fd < orig_fdcount; fd++)
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
						  acc.رقم_glyphs,
						  subset_fdcount,
						  subset_fdselect_اندازه,
						  subset_fdselect_format,
						  subset_fdselect_ranges,
						  fdmap)))
	أرجع خطأ;
    }
    وإلا
      fdmap.عنصر_خنثی (1);

    أرجع درست;
  }

  cff2_sub_جدول_info_t info;

  بیعلامت صحيح    رقم_glyphs;
  بیعلامت صحيح    orig_fdcount = 0;
  بیعلامت صحيح    subset_fdcount = 1;
  بیعلامت صحيح    subset_fdselect_اندازه = 0;
  بیعلامت صحيح    subset_fdselect_format = 0;
  منطقي            pinned = خطأ;
  hb_وکتور_t<کد_pair_t>   subset_fdselect_ranges;

  hb_inc_bimap_t   fdmap;

  str_buff_vec_t	    subset_charstrings;
  str_buff_vec_t	    subset_globalsubrs;
  hb_وکتور_t<str_buff_vec_t> subset_localsubrs;

  منطقي	    drop_hints = خطأ;
  منطقي	    desubroutinize = خطأ;

  منطقي	    partial_instancing = خطأ;
  cff2_instancing_plan_t instancing;
  ثابت hb_وکتور_t<hb_برچسب_t> *axis_برچسبها = اشاره_تهی;

  بیعلامت  min_charstrings_off_اندازه = 0;

  hb_array_t<صحيح> normalized_coords; // For instantiation
  سر_maxp_info_t سر_maxp_info;  // For FontBBox
  ثابت hb_hashmap_t<hb_codepoint_t, hb_pair_t<بیعلامت, صحيح>> *hmtx_map; // For widths

  // Width optimization results (for CFF1 conversion)
  بیعلامت default_عرض = 0;
  بیعلامت nominal_عرض = 0;

  // Command capture for specialization (CFF2→CFF1 conversion)
  منطقي capture_commands = خطأ;
  hb_وکتور_t<hb_وکتور_t<cs_command_t>> charstring_commands;
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
                             بیعلامت default_عرض,
                             بیعلامت nominal_عرض)
{
  c->push ();

  // CFF1 requires:
  // 1. Width at the beginning (if != defaultWidthX)
  // 2. endchar at the end
  str_buff_vec_t cff1_charstrings;
  إذا (unlikely (!cff1_charstrings.resize (plan.subset_charstrings.درازی)))
  {
    c->برداشت_discard ();
    أرجع خطأ;
  }

  لكل (بیعلامت i = 0; i < plan.subset_charstrings.درازی; i++)
  {
    // Get width for this glyph from hmtx_map
    بیعلامت عرض = 0;
    إذا (plan.hmtx_map->has (i))
      عرض = plan.hmtx_map->get (i).اولی;

    // Encode width if different from default
    str_encoder_t encoder (cff1_charstrings[i]);
    إذا (عرض != default_عرض)
    {
      صحيح delta = (صحيح) عرض - (صحيح) nominal_عرض;
      encoder.encode_int (delta);
    }

    // Use specialized commands if available, otherwise use binary
    إذا (plan.capture_commands && i < plan.charstring_commands.درازی &&
        plan.charstring_commands[i].درازی > 0)
    {
      // Specialize and encode commands
      خودکار &commands = plan.charstring_commands[i];
      CFF::specialize_commands (commands, 48);  /* maxstack=48 for CFF1 */
      إذا (unlikely (!CFF::encode_commands (commands, cff1_charstrings[i])))
      {
        c->برداشت_discard ();
        أرجع خطأ;
      }
    }
    وإلا
    {
      // Use binary CharString
      ثابت str_buff_t &cs = plan.subset_charstrings[i];
      لكل (بیعلامت j = 0; j < cs.درازی; j++)
        cff1_charstrings[i].push (cs[j]);
    }

    // Check if it already ends with endchar (0x0e) or return (0x0b)
    إذا (cff1_charstrings[i].درازی == 0 ||
        (cff1_charstrings[i].دم () != 0x0e && cff1_charstrings[i].دم () != 0x0b))
    {
      // Append endchar operator
      إذا (unlikely (!cff1_charstrings[i].افزودن_or_fail (0x0e)))
      {
        c->برداشت_discard ();
        أرجع خطأ;
      }
    }
  }

  بیعلامت داده_اندازه = 0;
  بیعلامت مجموع_اندازه = CFF1_Char_رشتهها::مجموع_اندازه (cff1_charstrings, &داده_اندازه, plan.min_charstrings_off_اندازه);
  إذا (unlikely (!c->شروع_zerocopy (مجموع_اندازه)))
  {
    c->برداشت_discard ();
    أرجع خطأ;
  }

  خودکار *cs = c->شروع_embed<CFF1_Char_رشتهها> ();
  إذا (unlikely (!cs->serialize (c, cff1_charstrings, &داده_اندازه, plan.min_charstrings_off_اندازه)))
  {
    c->برداشت_discard ();
    أرجع خطأ;
  }

  plan.info.char_رشتهها_link = c->برداشت_pack (خطأ);
  أرجع درست;
}

/* Serialize CID Charset (format 2 range: gid 0-N -> cid 0-N) */
ساكن منطقي
_serialize_cff1_charset (hb_serialize_context_t *c,
                         بیعلامت صحيح رقم_glyphs,
                         objidx_t &charset_link)
{
  // For CID fonts, create a simple identity charset
  // Format 2: one range covering all glyphs (except .notdef)
  c->push ();

  خودکار *charset = c->شروع_embed<Charset> ();
  إذا (unlikely (!charset))
  {
    c->برداشت_discard ();
    أرجع خطأ;
  }

  // Create a single range for CID 1 to num_glyphs-1
  hb_وکتور_t<کد_pair_t> ranges;
  إذا (رقم_glyphs > 1)
  {
    کد_pair_t range;
    range.کد = 1;  // first CID
    range.glyph = رقم_glyphs - 2;  // nLeft (covers glyphs 1 to num_glyphs-1)
    ranges.push (range);
  }

  إذا (unlikely (!charset->serialize (c, 2, رقم_glyphs, ranges)))
  {
    c->برداشت_discard ();
    أرجع خطأ;
  }

  charset_link = c->برداشت_pack ();
  أرجع درست;
}

/* CFF2 to CFF1 serialization */
نطاق CFF {

منطقي
serialize_cff2_مقصد_cff1 (hb_serialize_context_t *c,
                        OT::cff2_subset_plan &plan,
                        ثابت cff2_بالاترین_dict_مقادیر_t &cff2_بالاترین_Dict,
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
    hb_وکتور_t<بیعلامت> widths;
    widths.alloc (plan.رقم_glyphs);

    لكل (بیعلامت gid = 0; gid < plan.رقم_glyphs; gid++)
    {
      بیعلامت عرض = 0;
      إذا (plan.hmtx_map->has (gid))
        عرض = plan.hmtx_map->get (gid).اولی;
      widths.push (عرض);
    }

    // Optimize defaultWidthX and nominalWidthX
    CFF::optimize_widths (widths, plan.default_عرض, plan.nominal_عرض);
  }

  // 1. CharStrings (with widths prepended)
  إذا (!_serialize_cff1_charstrings (c, plan, plan.default_عرض, plan.nominal_عرض))
    return_trace (خطأ);

  // 2. Private DICs & Local Subrs (same as CFF2)
  hb_وکتور_t<جدول_info_t> خصوصی_dict_infos;
  إذا (unlikely (!خصوصی_dict_infos.resize (plan.subset_fdcount)))
    return_trace (خطأ);

  لكل (صحيح i = (صحيح)acc.خصوصی_Dicts.درازی; --i >= 0;)
  {
    إذا (plan.fdmap.has (i))
    {
      objidx_t subrs_link = 0;

      إذا (plan.subset_localsubrs[i].درازی > 0)
      {
        خودکار *dest = c->push<CFF1Subrs> ();
        إذا (likely (dest->serialize (c, plan.subset_localsubrs[i])))
          subrs_link = c->برداشت_pack (خطأ);
        وإلا
        {
          c->برداشت_discard ();
          return_trace (خطأ);
        }
      }

      خودکار *pd = c->push<خصوصی_Dict> ();
      // Use the CFF2 Private DICT serializer which instantiates blends when pinned=true
      cff2_خصوصی_dict_op_serializer_t privSzr (plan.desubroutinize, plan.drop_hints, plan.pinned,
                                                  acc.varStore, plan.normalized_coords);
      إذا (likely (pd->serialize (c, acc.خصوصی_Dicts[i], privSzr, subrs_link)))
      {
        // Add defaultWidthX and nominalWidthX for CFF1
        str_buff_t عرض_ops;
        str_encoder_t encoder (عرض_ops);
        encoder.encode_int (plan.default_عرض);
        encoder.encode_op (Op_کد_default_عرض_X);
        encoder.encode_int (plan.nominal_عرض);
        encoder.encode_op (Op_کد_nominal_عرض_X);

        إذا (!encoder.درون_error () && c->embed (عرض_ops.as_بایتها ().arrayZ, عرض_ops.درازی))
        {
          بیعلامت fd = plan.fdmap[i];
          خصوصی_dict_infos[fd].اندازه = c->درازی ();
          خصوصی_dict_infos[fd].link = c->برداشت_pack ();
        }
        وإلا
        {
          c->برداشت_discard ();
          return_trace (خطأ);
        }
      }
      وإلا
      {
        c->برداشت_discard ();
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
              | hb_filter ([&] (ثابت cff2_font_dict_مقادیر_t &_)
                { أرجع plan.fdmap.has (&_ - &acc.fontDicts[0]); }),
              hb_iter (خصوصی_dict_infos))
    ;
    // Explicitly specify template parameters: DICTVAL, INFO
    منطقي success = fda->serialize<cff2_font_dict_مقادیر_t, جدول_info_t> (c, it, fontSzr);
    إذا (success)
      plan.info.fd_array_link = c->برداشت_pack (خطأ);
    وإلا
    {
      c->برداشت_discard ();
      return_trace (خطأ);
    }
  }

  // 4. FDSelect (required in CFF1 CID-keyed fonts)
  // CFF1 requires FDSelect for all CID-keyed fonts, even with just one FD
  // CFF2 makes it optional when there's only one FD
  إذا (acc.fdSelect != &Null (CFF2FDSelect))
  {
    c->push ();
    إذا (likely (hb_serialize_cff_fdselect (c, plan.رقم_glyphs,
                                          *(ثابت FDSelect *)acc.fdSelect,
                                          plan.orig_fdcount,
                                          plan.subset_fdselect_format,
                                          plan.subset_fdselect_اندازه,
                                          plan.subset_fdselect_ranges)))
      plan.info.fd_select.link = c->برداشت_pack ();
    وإلا
    {
      c->برداشت_discard ();
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
      c->برداشت_discard ();
      return_trace (خطأ);
    }

    // nRanges
    HBUINT16 nRanges;
    nRanges = 1;
    إذا (unlikely (!c->embed (nRanges)))
    {
      c->برداشت_discard ();
      return_trace (خطأ);
    }

    // Single range: {first: 0, fd: 0}
    FDSelect3_Range range;
    range.اولی = 0;
    range.fd = 0;
    إذا (unlikely (!c->embed (range)))
    {
      c->برداشت_discard ();
      return_trace (خطأ);
    }

    // Sentinel (number of glyphs)
    HBUINT16 sentinel;
    sentinel = plan.رقم_glyphs;
    إذا (unlikely (!c->embed (sentinel)))
    {
      c->برداشت_discard ();
      return_trace (خطأ);
    }

    plan.info.fd_select.link = c->برداشت_pack ();
  }

  // 5. Charset (CID charset for identity mapping)
  objidx_t charset_link;
  إذا (!_serialize_cff1_charset (c, plan.رقم_glyphs, charset_link))
    return_trace (خطأ);

  // 6. Global Subrs
  {
    خودکار *dest = c->push<CFF1Subrs> ();
    إذا (likely (dest->serialize (c, plan.subset_globalsubrs)))
      c->برداشت_pack (خطأ);
    وإلا
    {
      c->برداشت_discard ();
      return_trace (خطأ);
    }
  }

  // 7. String INDEX - Add "Adobe" and "Identity" for ROS operator
  {
    ثابت حرف *adobe_str = "Adobe";
    ثابت حرف *عنصر_خنثی_str = "Identity";
    بیعلامت adobe_len = 5;  // strlen("Adobe")
    بیعلامت عنصر_خنثی_len = 8;  // strlen("Identity")

    // Build strings array
    hb_وکتور_t<hb_ubytes_t> رشتهها;
    رشتهها.alloc (2);
    رشتهها.push (hb_ubytes_t ((ثابت بیعلامت حرف *) adobe_str, adobe_len));
    رشتهها.push (hb_ubytes_t ((ثابت بیعلامت حرف *) عنصر_خنثی_str, عنصر_خنثی_len));

    // Serialize as CFF INDEX
    خودکار *dest = c->push<CFF1_شاخص> ();
    إذا (likely (dest->serialize (c, رشتهها)))
      c->برداشت_pack (خطأ);
    وإلا
    {
      c->برداشت_discard ();
      return_trace (خطأ);
    }
  }

  // 8. CFF Header
  OT::cff1 *cff = c->allocate_min<OT::cff1> ();
  إذا (unlikely (!cff)) return_trace (خطأ);

  /*سرآیند*/
  cff->نسخه.major = 0x01;
  cff->نسخه.minor = 0x00;
  cff->name_شاخص = cff->min_اندازه;
  cff->off_اندازه = 4; /* unused? */

  // 9. Name INDEX (single entry)
  {
    بیعلامت name_len = طول_رشته (CFF1_DEFAULT_FONT_NAME);

    CFF1_شاخص *idx = c->شروع_embed<CFF1_شاخص> ();
    إذا (unlikely (!idx)) return_trace (خطأ);

    إذا (unlikely (!idx->serialize_سرآیند (c, hb_iter (&name_len, 1), name_len)))
      return_trace (خطأ);

    إذا (unlikely (!c->embed (CFF1_DEFAULT_FONT_NAME, name_len)))
      return_trace (خطأ);
  }

  // 10. Top DICT INDEX
  {
    // Serialize the Top DICT data first
    c->push<بالاترین_Dict> ();
    cff1_from_cff2_بالاترین_dict_op_serializer_t بالاترین_Szr;

    // Serialize ROS first
    إذا (unlikely (!بالاترین_Szr.serialize_ros (c)))
    {
      c->برداشت_discard ();
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
      encoder.encode_op (Op_کد_Font_BBox);

      إذا (encoder.درون_error () || !c->embed (bbox_buff.as_بایتها ().arrayZ, bbox_buff.درازی))
      {
        c->برداشت_discard ();
        return_trace (خطأ);
      }
    }

    // Serialize charset operator
    إذا (charset_link && unlikely (!FontDict::serialize_link4_op (c, Op_کد_charset, charset_link, whence_t::Absolute)))
    {
      c->برداشت_discard ();
      return_trace (خطأ);
    }

    // Serialize FDSelect operator (required for CID-keyed CFF1 fonts)
    إذا (plan.info.fd_select.link && unlikely (!FontDict::serialize_link4_op (c, Op_کد_FDSelect, plan.info.fd_select.link, whence_t::Absolute)))
    {
      c->برداشت_discard ();
      return_trace (خطأ);
    }

    // Serialize FDArray operator (required for CID-keyed CFF1 fonts)
    إذا (plan.info.fd_array_link && unlikely (!FontDict::serialize_link4_op (c, Op_کد_FDArray, plan.info.fd_array_link, whence_t::Absolute)))
    {
      c->برداشت_discard ();
      return_trace (خطأ);
    }

    // Serialize other operators from CFF2 TopDict
    لكل (ثابت خودکار &opstr : cff2_بالاترین_Dict.values)
    {
      إذا (unlikely (!بالاترین_Szr.serialize (c, opstr, plan.info)))
      {
        c->برداشت_discard ();
        return_trace (خطأ);
      }
    }

    بیعلامت بالاترین_اندازه = c->درازی ();
    c->برداشت_pack (خطأ);

    // Serialize INDEX header
    خودکار *dest = c->شروع_embed<CFF1_شاخص> ();
    إذا (unlikely (!dest->serialize_سرآیند (c, hb_iter (&بالاترین_اندازه, 1), بالاترین_اندازه)))
      return_trace (خطأ);
  }

  return_trace (درست);
}

} /* namespace CFF */

ساكن منطقي _serialize_cff2_charstrings (hb_serialize_context_t *c,
			     cff2_subset_plan &plan,
			     ثابت OT::cff2::accelerator_subset_t  &acc)
{
  c->push ();

  بیعلامت داده_اندازه = 0;
  بیعلامت مجموع_اندازه = CFF2_Char_رشتهها::مجموع_اندازه (plan.subset_charstrings, &داده_اندازه, plan.min_charstrings_off_اندازه);
  إذا (unlikely (!c->شروع_zerocopy (مجموع_اندازه)))
    أرجع خطأ;

  خودکار *cs = c->شروع_embed<CFF2_Char_رشتهها> ();
  إذا (unlikely (!cs->serialize (c, plan.subset_charstrings, &داده_اندازه, plan.min_charstrings_off_اندازه)))
  {
    c->برداشت_discard ();
    أرجع خطأ;
  }

  plan.info.char_رشتهها_link = c->برداشت_pack (خطأ);
  أرجع درست;
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
  hb_وکتور_t<جدول_info_t>  خصوصی_dict_infos;
  إذا (unlikely (!خصوصی_dict_infos.resize (plan.subset_fdcount))) أرجع خطأ;

  لكل (صحيح i = (صحيح)خصوصی_Dicts.درازی; --i >= 0 ;)
  {
    إذا (plan.fdmap.has (i))
    {
      objidx_t	subrs_link = 0;

      إذا (plan.subset_localsubrs[i].درازی > 0)
      {
	خودکار *dest = c->push <CFF2Subrs> ();
	إذا (likely (dest->serialize (c, plan.subset_localsubrs[i])))
	  subrs_link = c->برداشت_pack (خطأ);
	وإلا
	{
	  c->برداشت_discard ();
	  أرجع خطأ;
	}
      }
      خودکار *pd = c->push<خصوصی_Dict> ();
      cff2_خصوصی_dict_op_serializer_t privSzr (plan.desubroutinize, plan.drop_hints, plan.pinned,
						 varStore, normalized_coords,
						 plan.partial_instancing ? &plan.instancing : اشاره_تهی);
      إذا (likely (pd->serialize (c, خصوصی_Dicts[i], privSzr, subrs_link)))
      {
	بیعلامت fd = plan.fdmap[i];
	خصوصی_dict_infos[fd].اندازه = c->درازی ();
	خصوصی_dict_infos[fd].link = c->برداشت_pack ();
      }
      وإلا
      {
	c->برداشت_discard ();
	أرجع خطأ;
      }
    }
  }

  /* FDSelect */
  إذا (fdSelect != &Null (CFF2FDSelect))
  {
    c->push ();
    إذا (likely (hb_serialize_cff_fdselect (c, plan.رقم_glyphs, *(ثابت FDSelect *)fdSelect,
					   plan.orig_fdcount,
					   plan.subset_fdselect_format, plan.subset_fdselect_اندازه,
					   plan.subset_fdselect_ranges)))
      plan.info.fd_select.link = c->برداشت_pack ();
    وإلا
    {
      c->برداشت_discard ();
      أرجع خطأ;
    }
  }

  /* FDArray (FD Index) */
  {
    خودکار *fda = c->push<CFF2FDArray> ();
    cff_font_dict_op_serializer_t fontSzr;
    خودکار it =
    + hb_zip (+ hb_iter (fontDicts)
	      | hb_filter ([&] (ثابت cff2_font_dict_مقادیر_t &_)
		{ أرجع plan.fdmap.has (&_ - &fontDicts[0]); }),
	      hb_iter (خصوصی_dict_infos))
    ;
    إذا (unlikely (!fda->serialize (c, it, fontSzr)))
    {
      c->برداشت_discard ();
      أرجع خطأ;
    }
    plan.info.fd_array_link = c->برداشت_pack (خطأ);
  }

  /* variation store */
  إذا (varStore != &Null (CFF2_قلم_Variation_Store) &&
      !plan.pinned)
  {
    إذا (plan.partial_instancing)
    {
      إذا (plan.instancing.has_variations ())
      {
	c->push ();
	إذا (unlikely (!plan.instancing.serialize_var_store (c, *plan.axis_برچسبها)))
	{
	  c->برداشت_discard ();
	  أرجع خطأ;
	}
	plan.info.var_store_link = c->برداشت_pack (خطأ);
      }
      /* else: the store emptied; the vstore op gets dropped along with it. */
    }
    وإلا
    {
      خودکار *dest = c->push<CFF2_قلم_Variation_Store> ();
      إذا (unlikely (!dest->serialize (c, varStore)))
      {
	c->برداشت_discard ();
	أرجع خطأ;
      }
      plan.info.var_store_link = c->برداشت_pack (خطأ);
    }
  }

  OT::cff2 *cff2 = c->allocate_min<OT::cff2> ();
  إذا (unlikely (!cff2)) أرجع خطأ;

  /*سرآیند*/
  cff2->نسخه.major = 0x02;
  cff2->نسخه.minor = 0x00;
  cff2->بالاترین_Dict_2 = OT::cff2::static_اندازه;

  /* top dict */
  {
    بالاترین_Dict &dict = cff2 + cff2->بالاترین_Dict_2;
    cff2_بالاترین_dict_op_serializer_t بالاترین_Szr;
    إذا (unlikely (!dict.serialize (c, بالاترین_Dict_2, بالاترین_Szr, plan.info))) أرجع خطأ;
    cff2->بالاترین_Dict_اندازه = c->سر_2 - (ثابت حرف *)&dict;
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
    منطقي نتیجه = CFF::serialize_cff2_مقصد_cff1 (c->serializer, cff2_plan, بالاترین_Dict_2, *هذا);

    إذا (c->serializer->ran_بیرون_از_جا ())
      أرجع درست; // Subsetter will retry with larger buffer

    إذا (نتیجه && !c->serializer->درون_error ())
    {
      // Success - end serialization to resolve links
      c->serializer->انتها_serialize ();

      // Copy the serialized CFF1 data and add as CFF table
      hb_blob_t *cff_blob = c->serializer->copy_blob ();
      إذا (cff_blob)
      {
        c->plan->جمع_کن_جدول (HB_برچسب('C','F','F',' '), cff_blob);
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

#پایان_اگر
