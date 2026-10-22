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

#جيڪڏهن_وصف_نه_ٿيل HB_NO_SUBSET_CFF

#ضمن "hb-open-type.hh"
#ضمن "hb-ot-cff2-table.hh"
#ضمن "hb-ot-var-common.hh"
#ضمن "hb-set.h"
#ضمن "hb-subset-plan.hh"
#ضمن "hb-subset-cff-common.hh"
#ضمن "hb-cff2-interp-cs.hh"
#ضمن "hb-subset-cff2-to-cff1.hh"

استعمال نطاق CFF;

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
    hb_ويڪٽر_t<hb_ويڪٽر_t<عائم>> ميٽرڪس;
    /* Default-value fold-in scalars, one per old region: the old store's
     * region scalars at the new default location. */
    hb_ويڪٽر_t<عائم> gains;
    /* Surviving regions, parallel to matrix rows. */
    hb_ويڪٽر_t<hb_hashmap_t<hb_ٽيگ_t, Triple>> regions;
    /* Index into the new global region list, per new region (finalize). */
    hb_ويڪٽر_t<بنانشان> region_indices;
    بنانشان new_major = 0;
    منطقي alive_ = خطأ;
    منطقي alive () ثابت { أرجع alive_; }

    /* One value's delta for new region j, from its old per-region deltas. */
    مزدوج transform_delta (بنانشان j, hb_array_t<ثابت عدد_t> deltas) ثابت
    {
      ثابت hb_ويڪٽر_t<عائم> &کانو = ميٽرڪس.arrayZ[j];
      مزدوج d = 0.;
      بنانشان ڳڻپ = hb_min (کانو.ڊيگهائي, deltas.ڊيگهائي);
      لكل (بنانشان c = 0; c < ڳڻپ; c++)
	d += (مزدوج) کانو.arrayZ[c] * deltas.arrayZ[c].منزل_real ();
      أرجع d;
    }
  };

  hb_ويڪٽر_t<vardata_transform_t> transforms;  /* per old VarData (vsindex) */
  hb_ويڪٽر_t<hb_hashmap_t<hb_ٽيگ_t, Triple>> new_regions;  /* global region list */
  hb_سيٽ_t استعمال_ٿيل_majors;
  بنانشان new_major_ڳڻپ = 0;

  منطقي has_variations () ثابت { أرجع new_major_ڳڻپ; }

  ثابت vardata_transform_t *get_transform (بنانشان major) ثابت
  { أرجع major < transforms.ڊيگهائي ? &transforms.arrayZ[major] : خالي_اشارو; }

  /* Record that a retained blend references this VarData. */
  فراغ note_use (بنانشان major) { استعمال_ٿيل_majors.جوڙيو (major); }

  /* The new vsindex a private dict's (or charstring's) old ivs maps to;
   * dead VarDatas fall back to 0, their blends having become static. */
  بنانشان remap_ivs (بنانشان old_ivs) ثابت
  {
    ثابت vardata_transform_t *t = get_transform (old_ivs);
    أرجع (t && t->alive ()) ? t->new_major : 0;
  }

  مزدوج fold_deltas (بنانشان old_ivs, hb_array_t<ثابت عدد_t> deltas) ثابت
  {
    ثابت vardata_transform_t *t = get_transform (old_ivs);
    إذا (unlikely (!t || t->gains.ڊيگهائي != deltas.ڊيگهائي)) أرجع 0.;
    مزدوج v = 0.;
    لكل (بنانشان i = 0; i < deltas.ڊيگهائي; i++)
      v += (مزدوج) t->gains.arrayZ[i] * deltas.arrayZ[i].منزل_real ();
    أرجع v;
  }

  منطقي create (ثابت CFF2_شي_Variation_Store *cff2VarStore,
	       ثابت hb_subset_plan_t *plan)
  {
    ثابت OT::شي_Variation_Store &varStore = cff2VarStore->varStore;
    بنانشان major_ڳڻپ = varStore.get_sub_جدول_ڳڻپ ();
    إذا (!transforms.resize (major_ڳڻپ)) أرجع خطأ;

    إذا (!varStore.get_region_list ().get_var_regions (plan->axes_old_اشاريو_ٽيگ_map, orig_regions))
      أرجع خطأ;

    /* Instance each VarData with identity delta rows; the surviving tuples'
     * delta columns are the old-region → new-region transform. */
    OT::optimize_scratch_t scratch;
    لكل (بنانشان m = 0; m < major_ڳڻپ; m++)
    {
      ثابت OT::Var_معلومات &var_معلومات = varStore.get_sub_جدول (m);
      بنانشان k = var_معلومات.get_region_اشاريو_ڳڻپ ();
      vardata_transform_t &t = transforms.arrayZ[m];

      إذا (!t.gains.resize_exact (k)) أرجع خطأ;
      varStore.get_region_scalars (m, plan->normalized_coords.arrayZ,
				   plan->normalized_coords.ڊيگهائي,
				   t.gains.arrayZ, k);

      OT::tuple_variations_t tuples;
      لكل (بنانشان r = 0; r < k; r++)
      {
	OT::tuple_delta_t tuple;
	إذا (!tuple.deltas_x.resize (k) ||
	    !tuple.indices.resize (k))
	  أرجع خطأ;
	لكل (بنانشان i = 0; i < k; i++)
	  tuple.indices.arrayZ[i] = سچ;
	tuple.deltas_x.arrayZ[r] = 1.f;
	بنانشان region_اشاريو = var_معلومات.get_region_اشاريو (r);
	إذا (unlikely (region_اشاريو >= orig_regions.ڊيگهائي)) أرجع خطأ;
	tuple.axis_tuples = orig_regions.arrayZ[region_اشاريو];
	إذا (unlikely (tuple.axis_tuples.اندريون_error ())) أرجع خطأ;
	tuples.tuple_vars.push (std::move (tuple));
      }
      إذا (unlikely (tuples.tuple_vars.اندريون_error ())) أرجع خطأ;

      إذا (!tuples.instantiate (plan->axes_location, plan->axes_triple_distances, scratch))
	أرجع خطأ;

      لكل (خودڪار &tuple : tuples.tuple_vars)
      {
	إذا (unlikely (tuple.deltas_x.ڊيگهائي != k)) أرجع خطأ;
	hb_ويڪٽر_t<عائم> کانو;
	إذا (unlikely (!کانو.resize_exact (k))) أرجع خطأ;
	لكل (بنانشان i = 0; i < k; i++)
	  کانو.arrayZ[i] = tuple.deltas_x.arrayZ[i];
	t.ميٽرڪس.push (std::move (کانو));
	t.regions.push (std::move (tuple.axis_tuples));
      }
      إذا (unlikely (t.ميٽرڪس.اندريون_error () || t.regions.اندريون_error ())) أرجع خطأ;
    }

    أرجع سچ;
  }

  /* Decide which VarDatas survive, number them, and build the new global
   * region list: surviving original regions first, in their original order,
   * then solver-split new regions in order of appearance (fontTools'
   * rebuildRegions + prune_regions). */
  منطقي finalize ()
  {
    لكل (بنانشان m = 0; m < transforms.ڊيگهائي; m++)
    {
      vardata_transform_t &t = transforms.arrayZ[m];
      t.alive_ = t.ميٽرڪس.ڊيگهائي && استعمال_ٿيل_majors.has (m);
      إذا (t.alive_)
	t.new_major = new_major_ڳڻپ++;
    }

    /* Keys hash by content, so pointers into different owners compare fine. */
    hb_hashmap_t<ثابت hb_hashmap_t<hb_ٽيگ_t, Triple>*, بنانشان> region_idx_map;
    hb_ويڪٽر_t<ثابت hb_hashmap_t<hb_ٽيگ_t, Triple>*> region_ptrs;
    لكل (ثابت vardata_transform_t &t : transforms)
    {
      إذا (!t.alive ()) استمر;
      لكل (ثابت خودڪار &region : t.regions)
	إذا (!region_idx_map.has (&region))
	  إذا (unlikely (!region_idx_map.سيٽ (&region, (بنانشان) -1)))
	    أرجع خطأ;
    }
    لكل (ثابت خودڪار &r : orig_regions)
    {
      بنانشان *idx;
      إذا (region_idx_map.has (&r, &idx) && *idx == (بنانشان) -1)
      {
	إذا (unlikely (!region_idx_map.سيٽ (&r, region_ptrs.ڊيگهائي))) أرجع خطأ;
	region_ptrs.push (&r);
      }
    }
    لكل (ثابت vardata_transform_t &t : transforms)
    {
      إذا (!t.alive ()) استمر;
      لكل (ثابت خودڪار &region : t.regions)
      {
	بنانشان *idx;
	إذا (region_idx_map.has (&region, &idx) && *idx == (بنانشان) -1)
	{
	  إذا (unlikely (!region_idx_map.سيٽ (&region, region_ptrs.ڊيگهائي))) أرجع خطأ;
	  region_ptrs.push (&region);
	}
      }
    }
    إذا (unlikely (region_ptrs.اندريون_error ())) أرجع خطأ;

    لكل (vardata_transform_t &t : transforms)
    {
      إذا (!t.alive ()) استمر;
      لكل (ثابت خودڪار &region : t.regions)
      {
	بنانشان *idx;
	إذا (unlikely (!region_idx_map.has (&region, &idx))) أرجع خطأ;
	t.region_indices.push (*idx);
      }
      إذا (unlikely (t.region_indices.اندريون_error ())) أرجع خطأ;
    }

    إذا (!new_regions.resize (region_ptrs.ڊيگهائي)) أرجع خطأ;
    لكل (بنانشان i = 0; i < region_ptrs.ڊيگهائي; i++)
    {
      new_regions.arrayZ[i] = *region_ptrs.arrayZ[i];
      إذا (unlikely (new_regions.arrayZ[i].اندريون_error ())) أرجع خطأ;
    }

    أرجع سچ;
  }

  /* Serialize the instanced store: a CFF2ItemVariationStore whose VarDatas
   * carry no items, just region indexes (blend deltas live in charstrings).
   * Laid out manually since all sizes are known up front. */
  منطقي serialize_var_store (hb_serialize_context_t *c,
			    ثابت hb_ويڪٽر_t<hb_ٽيگ_t> &axis_ٽيگ_فهرست) ثابت
  {
    بنانشان معلومات_ڳڻپ = new_major_ڳڻپ;
    بنانشان سرو_سائيز = 2 + 4 + 2 + 4 * معلومات_ڳڻپ;

    uint64_t وٿي = سرو_سائيز;
    hb_ويڪٽر_t<بنانشان> معلومات_offsets;
    لكل (ثابت vardata_transform_t &t : transforms)
    {
      إذا (!t.alive ()) استمر;
      معلومات_offsets.push ((بنانشان) وٿي);
      وٿي += 6 + 2 * (uint64_t) t.region_indices.ڊيگهائي;
    }
    إذا (unlikely (معلومات_offsets.اندريون_error () || معلومات_offsets.ڊيگهائي != معلومات_ڳڻپ))
      أرجع خطأ;
    بنانشان regions_وٿي = (بنانشان) وٿي;
    uint64_t ڪل = وٿي + 4 + (uint64_t) new_regions.ڊيگهائي * axis_ٽيگ_فهرست.ڊيگهائي * 6;
    إذا (unlikely (ڪل > 0xFFFFFFFFu)) أرجع خطأ;

    HBUINT16 *سائيز_ميدان = c->allocate_سائيز<HBUINT16> (HBUINT16::static_سائيز);
    إذا (unlikely (!سائيز_ميدان)) أرجع خطأ;
    /* OFF 2026 5.3.3.11.2 permits larger stores with length=65535. */
    *سائيز_ميدان = hb_min (ڪل, (uint64_t) 0xFFFFu);

    حرف *سرو = c->allocate_سائيز<حرف> (سرو_سائيز);
    إذا (unlikely (!سرو)) أرجع خطأ;
    * (HBUINT16 *) (سرو + 0) = 1;			/* format */
    * (HBUINT32 *) (سرو + 2) = regions_وٿي;
    * (HBUINT16 *) (سرو + 6) = معلومات_ڳڻپ;
    لكل (بنانشان i = 0; i < معلومات_ڳڻپ; i++)
      * (HBUINT32 *) (سرو + 8 + 4 * i) = معلومات_offsets.arrayZ[i];

    لكل (ثابت vardata_transform_t &t : transforms)
    {
      إذا (!t.alive ()) استمر;
      بنانشان k = t.region_indices.ڊيگهائي;
      HBUINT16 *v = (HBUINT16 *) c->allocate_سائيز<حرف> (6 + 2 * k);
      إذا (unlikely (!v)) أرجع خطأ;
      v[0] = 0;						/* itemCount */
      v[1] = 0;						/* wordSizeCount */
      v[2] = k;						/* regionIndices.len */
      لكل (بنانشان j = 0; j < k; j++)
	v[3 + j] = t.region_indices.arrayZ[j];
    }

    hb_ويڪٽر_t<ثابت hb_hashmap_t<hb_ٽيگ_t, Triple>*> region_ptrs;
    إذا (unlikely (!region_ptrs.alloc_exact (new_regions.ڊيگهائي))) أرجع خطأ;
    لكل (ثابت خودڪار &r : new_regions)
      region_ptrs.push (&r);
    خودڪار *region_list = c->شروعات_embed<OT::VarRegionList> ();
    أرجع region_list->serialize (c, axis_ٽيگ_فهرست, region_ptrs);
  }

  خاص:
  hb_ويڪٽر_t<hb_hashmap_t<hb_ٽيگ_t, Triple>> orig_regions;
};

/* Usage-collection pass for partial instancing: record which VarDatas the
 * retained charstrings' and private dicts' blends reference.  Mirrors the
 * emission pass: blends consumed by dropped hint ops don't count. */
بنية cff2_usage_param_t
{
  فراغ init () {}

  cff2_instancing_plan_t *instancing = خالي_اشارو;
  منطقي drop_hints = خطأ;
  بنانشان ivs = 0;	/* dict-side current vsindex */
};

بنية cff2_cs_opset_usage_t : cff2_cs_opset_t<cff2_cs_opset_usage_t, cff2_usage_param_t, blend_arg_t>
{
  ساكن فراغ flush_دليلون_and_op (op_ڪوڊ_t op, cff2_cs_interp_env_t<blend_arg_t> &env, cff2_usage_param_t& param)
  {
    منطقي counts = سچ;
    بدل (op)
    {
      حالة Op_ڪوڊ_return:
      حالة Op_ڪوڊ_endchar:
	counts = خطأ;
	اكسر;

      حالة Op_ڪوڊ_hstem:
      حالة Op_ڪوڊ_hstemhm:
      حالة Op_ڪوڊ_vstem:
      حالة Op_ڪوڊ_vstemhm:
      حالة Op_ڪوڊ_hintmask:
      حالة Op_ڪوڊ_cntrmask:
	إذا (param.drop_hints)
	  counts = خطأ;
	اكسر;

      افتراضي:
	اكسر;
    }

    إذا (counts)
      لكل (بنانشان صحيح i = 0; i < env.arg_ڍير.get_ڳڻپ (); i++)
	إذا (env.arg_ڍير[i].blending ())
	{
	  param.instancing->note_use (env.get_ivs ());
	  اكسر;
	}

    env.clear_دليلون ();
  }
};

بنية cff2_ذاتي_dict_usage_opset_t : dict_opset_t
{
  ساكن فراغ process_op (op_ڪوڊ_t op, cff2_priv_dict_interp_env_t& env, cff2_usage_param_t& param)
  {
    بدل (op) {
      حالة Op_ڪوڊ_vsindexdict:
	env.process_vsindex ();
	param.ivs = env.get_ivs ();
	env.clear_دليلون ();
	أرجع;
      حالة Op_ڪوڊ_blenddict:
	param.instancing->note_use (param.ivs);
	env.clear_دليلون ();
	أرجع;
      افتراضي:
	dict_opset_t::process_op (op, env);
	إذا (!env.arg_ڍير.is_empty ()) أرجع;
	env.clear_دليلون ();
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

بنية cff2_چوٽي_dict_op_serializer_t : cff_چوٽي_dict_op_serializer_t<>
{
  منطقي serialize (hb_serialize_context_t *c,
		  ثابت op_str_t &opstr,
		  ثابت cff2_sub_جدول_info_t &info) ثابت
  {
    TRACE_SERIALIZE (هذا);

    بدل (opstr.op)
    {
      حالة Op_ڪوڊ_vstore:
        إذا (info.var_store_link)
	  return_trace (FontDict::serialize_link4_op(c, opstr.op, info.var_store_link));
	وإلا
	  return_trace (سچ);

      افتراضي:
	return_trace (cff_چوٽي_dict_op_serializer_t<>::serialize (c, opstr, info));
    }
  }
};

بنية cff2_cs_opset_flatten_t : cff2_cs_opset_t<cff2_cs_opset_flatten_t, flatten_param_t, blend_arg_t>
{
  ساكن فراغ flush_دليلون_and_op (op_ڪوڊ_t op, cff2_cs_interp_env_t<blend_arg_t> &env, flatten_param_t& param)
  {
    /* Optionally capture command for specialization (before flushing, to preserve args) */
    إذا (param.commands)
    {
      منطقي skip_command = خطأ;

      بدل (op)
      {
	حالة Op_ڪوڊ_return:
	حالة Op_ڪوڊ_endchar:
	  skip_command = سچ;
	  اكسر;

	حالة Op_ڪوڊ_hstem:
	حالة Op_ڪوڊ_hstemhm:
	حالة Op_ڪوڊ_vstem:
	حالة Op_ڪوڊ_vstemhm:
	حالة Op_ڪوڊ_hintmask:
	حالة Op_ڪوڊ_cntrmask:
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
	لكل (بنانشان صحيح i = 0; i < env.arg_ڍير.get_ڳڻپ ();)
	{
	  ثابت blend_arg_t &arg = env.arg_ڍير[i];
	  إذا (arg.blending ())
	  {
	    /* For blend args, capture only the resolved default value */
	    cmd.دليلون.push (arg);
	    /* Skip over the multiple blend values */
	    i += arg.انگ_قدرون_2;
	  }
	  وإلا
	  {
	    cmd.دليلون.push (arg);
	    i++;
	  }
	}
	param.commands->push (cmd);
      }
    }

    بدل (op)
    {
      حالة Op_ڪوڊ_return:
      حالة Op_ڪوڊ_endchar:
	/* dummy opcodes in CFF2. ignore */
	إذا (op == Op_ڪوڊ_endchar && param.instancer)
	  emit_vsindex_اڳياڙي (env, param);
	اكسر;

      حالة Op_ڪوڊ_hstem:
      حالة Op_ڪوڊ_hstemhm:
      حالة Op_ڪوڊ_vstem:
      حالة Op_ڪوڊ_vstemhm:
      حالة Op_ڪوڊ_hintmask:
      حالة Op_ڪوڊ_cntrmask:
	إذا (param.drop_hints)
	{
	  env.clear_دليلون ();
	  أرجع;
	}
	HB_FALLTHROUGH;

      افتراضي:
	SUPER::flush_دليلون_and_op (op, env, param);
	اكسر;
    }
  }

  ساكن فراغ flush_دليلون (cff2_cs_interp_env_t<blend_arg_t> &env, flatten_param_t& param)
  {
    لكل (بنانشان صحيح i = 0; i < env.arg_ڍير.get_ڳڻپ ();)
    {
      ثابت blend_arg_t &arg = env.arg_ڍير[i];
      إذا (arg.blending ())
      {
	إذا (unlikely (!((arg.انگ_قدرون_2 > 0) && (env.arg_ڍير.get_ڳڻپ () >= arg.انگ_قدرون_2))))
	{
	  env.سيٽ_error ();
	  أرجع;
	}
	إذا (param.instancer)
	  rewrite_blends (arg, i, env, param);
	وإلا
	  flatten_blends (arg, i, env, param);
	i += arg.انگ_قدرون_2;
      }
      وإلا
      {
	str_encoder_t  encoder (param.flatStr);
	encoder.encode_انگ_cs (arg);
	i++;
      }
    }
    SUPER::flush_دليلون (env, param);
  }

  ساكن فراغ flatten_blends (ثابت blend_arg_t &arg, بنانشان صحيح i, cff2_cs_interp_env_t<blend_arg_t> &env, flatten_param_t& param)
  {
    /* flatten the default values */
    str_encoder_t  encoder (param.flatStr);
    لكل (بنانشان صحيح j = 0; j < arg.انگ_قدرون_2; j++)
    {
      ثابت blend_arg_t &arg1 = env.arg_ڍير[i + j];
      إذا (unlikely (!((arg1.blending () && (arg.انگ_قدرون_2 == arg1.انگ_قدرون_2) && (arg1.قدر_اشاريو_2 == j) &&
	      (arg1.deltas.ڊيگهائي == env.get_region_ڳڻپ ())))))
      {
	env.سيٽ_error ();
	أرجع;
      }
      encoder.encode_انگ_cs (arg1);
    }
    /* flatten deltas for each value */
    لكل (بنانشان صحيح j = 0; j < arg.انگ_قدرون_2; j++)
    {
      ثابت blend_arg_t &arg1 = env.arg_ڍير[i + j];
      لكل (بنانشان صحيح k = 0; k < arg1.deltas.ڊيگهائي; k++)
	encoder.encode_انگ_cs (arg1.deltas[k]);
    }
    /* flatten the number of values followed by blend operator */
    encoder.encode_int (arg.انگ_قدرون_2);
    encoder.encode_op (Op_ڪوڊ_blendcs);
  }

  /* Partial instancing: rewrite one blend group against the instanced store.
   * Each value's default absorbs the folded (pinned-away) contribution, and
   * its deltas are mapped onto the new region list. */
  ساكن فراغ rewrite_blends (ثابت blend_arg_t &arg, بنانشان صحيح i, cff2_cs_interp_env_t<blend_arg_t> &env, flatten_param_t& param)
  {
    ثابت cff2_instancing_plan_t &instancer = *param.instancer;
    بنانشان ivs = env.get_ivs ();
    ثابت cff2_instancing_plan_t::vardata_transform_t *t = instancer.get_transform (ivs);
    إذا (unlikely (!t))
    {
      env.سيٽ_error ();
      أرجع;
    }

    بنانشان n = arg.انگ_قدرون_2;
    لكل (بنانشان صحيح j = 0; j < n; j++)
    {
      ثابت blend_arg_t &arg1 = env.arg_ڍير[i + j];
      إذا (unlikely (!(arg1.blending () && (arg1.انگ_قدرون_2 == n) && (arg1.قدر_اشاريو_2 == j) &&
		      (arg1.deltas.ڊيگهائي == env.get_region_ڳڻپ ()) &&
		      (arg1.deltas.ڊيگهائي == t->gains.ڊيگهائي))))
      {
	env.سيٽ_error ();
	أرجع;
      }
    }

    str_encoder_t encoder (param.flatStr);
    بنانشان k_new = t->alive () ? t->ميٽرڪس.ڊيگهائي : 0;

    إذا (!k_new)
    {
      /* All of this VarData's regions died; blends dissolve into statics. */
      لكل (بنانشان صحيح j = 0; j < n; j++)
      {
	ثابت blend_arg_t &arg1 = env.arg_ڍير[i + j];
	عدد_t v;
	v.سيٽ_real (arg1.منزل_real () + round (instancer.fold_deltas (ivs, arg1.deltas.as_array ())));
	encoder.encode_انگ_cs (v);
      }
      أرجع;
    }

    /* Emit in chunks so the output charstring stays within the argument
     * stack limit: values already emitted for this op, plus this chunk's
     * defaults, deltas and count, must not exceed 513. */
    بنانشان پورو = 0;
    طالما (پورو < n)
    {
      بنانشان avail = 512 > (i + پورو) ? 512 - (i + پورو) : 0;
      بنانشان chunk = hb_min (avail / (k_new + 1), n - پورو);
      إذا (unlikely (!chunk))
      {
	env.سيٽ_error ();
	أرجع;
      }

      لكل (بنانشان صحيح j = پورو; j < پورو + chunk; j++)
      {
	ثابت blend_arg_t &arg1 = env.arg_ڍير[i + j];
	عدد_t v;
	v.سيٽ_real (arg1.منزل_real () + round (instancer.fold_deltas (ivs, arg1.deltas.as_array ())));
	encoder.encode_انگ_cs (v);
      }
      لكل (بنانشان صحيح j = پورو; j < پورو + chunk; j++)
      {
	ثابت blend_arg_t &arg1 = env.arg_ڍير[i + j];
	لكل (بنانشان r = 0; r < k_new; r++)
	{
	  عدد_t v;
	  v.سيٽ_int (hb_clamp_منزل<صحيح> (round (t->transform_delta (r, arg1.deltas.as_array ()))));
	  encoder.encode_انگ_cs (v);
	}
      }
      encoder.encode_int (chunk);
      encoder.encode_op (Op_ڪوڊ_blendcs);
      پورو += chunk;
    }
    param.emitted_blend = سچ;
  }

  /* Partial instancing: prepend a vsindex op when the charstring's
   * (remapped) ivs differs from its FD's (remapped) private-dict ivs. */
  ساكن فراغ emit_vsindex_اڳياڙي (cff2_cs_interp_env_t<blend_arg_t> &env, flatten_param_t& param)
  {
    إذا (!param.emitted_blend) أرجع;
    ثابت cff2_instancing_plan_t &instancer = *param.instancer;
    بنانشان new_ivs = instancer.remap_ivs (env.get_ivs ());
    بنانشان fd_ivs = instancer.remap_ivs (env.get_orig_ivs ());
    إذا (new_ivs == fd_ivs) أرجع;

    str_buff_t اڳياڙي;
    str_encoder_t enc (اڳياڙي);
    enc.encode_int ((صحيح) new_ivs);
    enc.encode_op (Op_ڪوڊ_vsindexcs);

    str_buff_t &buf = param.flatStr;
    بنانشان old_len = buf.ڊيگهائي;
    إذا (unlikely (اڳياڙي.اندريون_error () || !buf.resize (old_len + اڳياڙي.ڊيگهائي)))
    {
      env.سيٽ_error ();
      أرجع;
    }
    memmove (buf.arrayZ + اڳياڙي.ڊيگهائي, buf.arrayZ, old_len);
    hb_memcpy (buf.arrayZ, اڳياڙي.arrayZ, اڳياڙي.ڊيگهائي);
  }

  ساكن فراغ flush_op (op_ڪوڊ_t op, cff2_cs_interp_env_t<blend_arg_t> &env, flatten_param_t& param)
  {
    بدل (op)
    {
      حالة Op_ڪوڊ_return:
      حالة Op_ڪوڊ_endchar:
	أرجع;
      افتراضي:
	str_encoder_t  encoder (param.flatStr);
	encoder.encode_op (op);
    }
  }

  ساكن فراغ flush_hintmask (op_ڪوڊ_t op, cff2_cs_interp_env_t<blend_arg_t> &env, flatten_param_t& param)
  {
    SUPER::flush_hintmask (op, env, param);
    /* Preserve hintmask payload in captured commands for specializer re-encoding. */
    إذا (param.commands && !param.drop_hints && param.commands->ڊيگهائي > 0)
    {
      خودڪار &cmd = param.commands->پڇ ();
      إذا (cmd.op == op)
      {
        cmd.نقاب_بائيٽون.resize (env.hintmask_سائيز);
        إذا (unlikely (cmd.نقاب_بائيٽون.اندريون_error ()))
        {
          env.سيٽ_error ();
          أرجع;
        }
        لكل (بنانشان صحيح i = 0; i < env.hintmask_سائيز; i++)
          cmd.نقاب_بائيٽون[i] = env.str_ref[i];
      }
    }
    إذا (!param.drop_hints)
    {
      str_encoder_t  encoder (param.flatStr);
      لكل (بنانشان صحيح i = 0; i < env.hintmask_سائيز; i++)
	encoder.encode_byte (env.str_ref[i]);
    }
  }

  خاص:
  تعريفنوع cff2_cs_opset_t<cff2_cs_opset_flatten_t, flatten_param_t, blend_arg_t> SUPER;
  تعريفنوع cs_opset_t<blend_arg_t, cff2_cs_opset_flatten_t, cff2_cs_opset_flatten_t, cff2_cs_interp_env_t<blend_arg_t>, flatten_param_t> CSOPSET;
};

بنية cff2_cs_opset_subr_subset_t : cff2_cs_opset_t<cff2_cs_opset_subr_subset_t, subr_subset_param_t, blend_arg_t>
{
  ساكن فراغ process_op (op_ڪوڊ_t op, cff2_cs_interp_env_t<blend_arg_t> &env, subr_subset_param_t& param)
  {
    بدل (op) {

      حالة Op_ڪوڊ_return:
	param.current_parsed_str->flush_coalesced (env.str_ref);
	param.current_parsed_str->سيٽ_parsed ();
	env.return_from_subr ();
	param.سيٽ_current_str (env, خطأ);
	اكسر;

      حالة Op_ڪوڊ_endchar:
	param.current_parsed_str->flush_coalesced (env.str_ref);
	param.current_parsed_str->سيٽ_parsed ();
	SUPER::process_op (op, env, param);
	اكسر;

      حالة Op_ڪوڊ_callsubr:
	process_call_subr (op, CSType_LocalSubr, env, param, env.localSubrs, param.local_گهيرو_2);
	اكسر;

      حالة Op_ڪوڊ_callgsubr:
	process_call_subr (op, CSType_GlobalSubr, env, param, env.globalSubrs, param.global_گهيرو);
	اكسر;

      افتراضي:
	SUPER::process_op (op, env, param);
	param.current_parsed_str->جوڙيو_op (op, env.str_ref);
	اكسر;
    }
  }

  محمي:
  ساكن فراغ process_call_subr (op_ڪوڊ_t op, cs_type_t type,
				 cff2_cs_interp_env_t<blend_arg_t> &env, subr_subset_param_t& param,
				 cff2_biased_subrs_t& subrs, hb_سيٽ_t *گهيرو)
  {
    byte_str_ref_t    str_ref = env.str_ref;
    env.call_subr (subrs, type);
    param.current_parsed_str->جوڙيو_call_op (op, str_ref, env.context.subr_انگ_2);
    گهيرو->جوڙيو (env.context.subr_انگ_2);
    param.سيٽ_current_str (env, سچ);
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
    إذا (env.ڏٺل_vsindex ())
    {
      عدد_t  ivs;
      ivs.سيٽ_int ((صحيح)env.get_ivs ());
      charstring.سيٽ_اڳياڙي (ivs, Op_ڪوڊ_vsindexcs);
    }
  }
};

بنية cff2_ذاتي_blend_encoder_param_t
{
  cff2_ذاتي_blend_encoder_param_t (hb_serialize_context_t *c,
				      ثابت CFF2_شي_Variation_Store *varStore,
				      hb_array_t<صحيح> normalized_coords) :
    c (c), varStore (varStore), normalized_coords (normalized_coords) {}

  فراغ init () {}

  فراغ process_blend ()
  {
    إذا (!ڏٺل_blend)
    {
      region_ڳڻپ_2 = varStore->varStore.get_region_اشاريو_ڳڻپ (ivs);
      إذا (unlikely (!scalars.resize_exact (region_ڳڻپ_2)))
      {
	إذا (c) c->err (HB_SERIALIZE_ERROR_OTHER);
	ڏٺل_blend = سچ;
	أرجع;
      }
      varStore->varStore.get_region_scalars (ivs, normalized_coords.arrayZ, normalized_coords.ڊيگهائي,
					     &scalars[0], region_ڳڻپ_2);
      ڏٺل_blend = سچ;
    }
  }

  مزدوج blend_deltas (hb_array_t<ثابت عدد_t> deltas) ثابت
  {
    مزدوج v = 0;
    إذا (likely (scalars.ڊيگهائي == deltas.ڊيگهائي))
    {
      بنانشان ڳڻپ = scalars.ڊيگهائي;
      لكل (بنانشان i = 0; i < ڳڻپ; i++)
	v += (مزدوج) scalars.arrayZ[i] * deltas.arrayZ[i].منزل_real ();
    }
    أرجع v;
  }


  hb_serialize_context_t *c = خالي_اشارو;
  منطقي ڏٺل_blend = خطأ;
  بنانشان ivs = 0;
  بنانشان region_ڳڻپ_2 = 0;
  hb_ويڪٽر_t<عائم> scalars;
  ثابت	 CFF2_شي_Variation_Store *varStore = خالي_اشارو;
  hb_array_t<صحيح> normalized_coords;

  /* CFF2 partial instancing: when set, blends are rewritten against the
   * instanced variation store instead of being flattened. */
  ثابت cff2_instancing_plan_t *instancer = خالي_اشارو;
  /* Error-compensated fold rounding state, per dict operator. */
  مزدوج fold_exact = 0.;
  مزدوج fold_emitted = 0.;
};

بنية cff2_ذاتي_dict_blend_opset_t : dict_opset_t
{
  ساكن فراغ process_arg_blend (cff2_ذاتي_blend_encoder_param_t& param,
				 عدد_t &arg,
				 ثابت hb_array_t<ثابت عدد_t> blends,
				 بنانشان n, بنانشان i)
  {
    arg.سيٽ_int (round (arg.منزل_real () + param.blend_deltas (blends)));
  }

  /* The fold added to a value, error-compensated across the operator: most
   * blended private-dict values (BlueValues, StemSnap*, ...) are
   * delta-encoded, so independently rounded folds would accumulate in the
   * decoded absolutes; keeping the emitted cumulative fold equal to the
   * rounded exact cumulative fold bounds every absolute within ±0.5. */
  ساكن مزدوج fold (cff2_ذاتي_blend_encoder_param_t& param,
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
			      cff2_ذاتي_blend_encoder_param_t& param,
			      بنانشان شروعات, بنانشان n, بنانشان k)
  {
    ثابت cff2_instancing_plan_t &instancer = *param.instancer;
    ثابت cff2_instancing_plan_t::vardata_transform_t *t = instancer.get_transform (param.ivs);
    إذا (unlikely (!t || t->gains.ڊيگهائي != k))
    {
      env.سيٽ_error ();
      أرجع;
    }

    str_buff_t str;
    str_encoder_t encoder (str);

    لكل (بنانشان idx = 0; idx < شروعات; idx++)
      encoder.encode_انگ_tp (env.arg_ڍير[idx]);

    بنانشان k_new = t->alive () ? t->ميٽرڪس.ڊيگهائي : 0;
    إذا (!k_new)
    {
      /* This VarData died (or is unused by any retained charstring, which
       * can happen for FDs kept only for retain-gids holes); blends
       * dissolve into statics. */
      لكل (بنانشان j = 0; j < n; j++)
      {
	ثابت hb_array_t<ثابت عدد_t> blends = env.arg_ڍير.sub_array (شروعات + n + (j * k), k);
	عدد_t v;
	v.سيٽ_real (env.arg_ڍير[شروعات + j].منزل_real () + fold (param, blends));
	encoder.encode_انگ_tp (v);
      }
    }
    وإلا
    {
      بنانشان پورو = 0;
      طالما (پورو < n)
      {
	بنانشان avail = 512 > (شروعات + پورو) ? 512 - (شروعات + پورو) : 0;
	بنانشان chunk = hb_min (avail / (k_new + 1), n - پورو);
	إذا (unlikely (!chunk))
	{
	  env.سيٽ_error ();
	  أرجع;
	}

	لكل (بنانشان j = پورو; j < پورو + chunk; j++)
	{
	  ثابت hb_array_t<ثابت عدد_t> blends = env.arg_ڍير.sub_array (شروعات + n + (j * k), k);
	  عدد_t v;
	  v.سيٽ_real (env.arg_ڍير[شروعات + j].منزل_real () + fold (param, blends));
	  encoder.encode_انگ_tp (v);
	}
	لكل (بنانشان j = پورو; j < پورو + chunk; j++)
	{
	  ثابت hb_array_t<ثابت عدد_t> blends = env.arg_ڍير.sub_array (شروعات + n + (j * k), k);
	  لكل (بنانشان r = 0; r < k_new; r++)
	  {
	    عدد_t v;
	    v.سيٽ_int (hb_clamp_منزل<صحيح> (round (t->transform_delta (r, blends))));
	    encoder.encode_انگ_tp (v);
	  }
	}
	encoder.encode_int (chunk);
	encoder.encode_op (Op_ڪوڊ_blenddict);
	پورو += chunk;
      }
    }

    خودڪار بائيٽون = str.as_بائيٽون ();
    param.c->embed (&بائيٽون, بائيٽون.ڊيگهائي);

    env.clear_دليلون ();
  }

  ساكن فراغ process_blend (cff2_priv_dict_interp_env_t& env, cff2_ذاتي_blend_encoder_param_t& param)
  {
    بنانشان صحيح n, k;

    param.process_blend ();
    k = param.region_ڳڻپ_2;
    n = env.arg_ڍير.ڪڍو_uint ();

    بنانشان ڳڻپ = env.arg_ڍير.get_ڳڻپ ();
    بنانشان صحيح ڪل;
    إذا (unlikely (hb_unsigned_mul_overflows (k + 1, n, &ڪل) || ڪل > ڳڻپ))
    {
      env.سيٽ_error ();
      أرجع;
    }

    /* copy the blend values into blend array of the default values */
    بنانشان صحيح شروعات = ڳڻپ - ڪل;
    /* let an obvious error case fail, but note CFF2 spec doesn't forbid n==0 */
    إذا (unlikely (شروعات > ڳڻپ))
    {
      env.سيٽ_error ();
      أرجع;
    }

    إذا (param.instancer && k)
    {
      rewrite_blends (env, param, شروعات, n, k);
      أرجع;
    }

    لكل (بنانشان صحيح i = 0; i < n; i++)
    {
      ثابت hb_array_t<ثابت عدد_t> blends = env.arg_ڍير.sub_array (شروعات + n + (i * k), k);
      process_arg_blend (param, env.arg_ڍير[شروعات + i], blends, n, i);
    }

    /* pop off blend values leaving default values now adorned with blend values */
    env.arg_ڍير.ڪڍو (k * n);
  }

  ساكن فراغ process_op (op_ڪوڊ_t op, cff2_priv_dict_interp_env_t& env, cff2_ذاتي_blend_encoder_param_t& param)
  {
    بدل (op) {
      حالة Op_ڪوڊ_Std_HW:
      حالة Op_ڪوڊ_Std_VW:
      حالة Op_ڪوڊ_Blue_Scale:
      حالة Op_ڪوڊ_Blue_سرڪايو:
      حالة Op_ڪوڊ_Blue_Fuzz:
      حالة Op_ڪوڊ_Expansion_ضرب_ڪندڙ:
      حالة Op_ڪوڊ_Language_گروهه:
      حالة Op_ڪوڊ_Blue_قدرون:
      حالة Op_ڪوڊ_Other_Blues:
      حالة Op_ڪوڊ_Family_Blues:
      حالة Op_ڪوڊ_Family_Other_Blues:
      حالة Op_ڪوڊ_Stem_Snap_H:
      حالة Op_ڪوڊ_Stem_Snap_V:
	اكسر;
      حالة Op_ڪوڊ_vsindexdict:
	env.process_vsindex ();
	param.ivs = env.get_ivs ();
	إذا (param.instancer)
	{
	  /* Re-emit with the remapped vsindex; 0 is the dict default. */
	  بنانشان new_ivs = param.instancer->remap_ivs (param.ivs);
	  إذا (new_ivs)
	  {
	    str_buff_t str;
	    str_encoder_t encoder (str);
	    encoder.encode_int ((صحيح) new_ivs);
	    encoder.encode_op (Op_ڪوڊ_vsindexdict);
	    خودڪار بائيٽون = str.as_بائيٽون ();
	    param.c->embed (&بائيٽون, بائيٽون.ڊيگهائي);
	  }
	}
	env.clear_دليلون ();
	أرجع;
      حالة Op_ڪوڊ_blenddict:
	process_blend (env, param);
	أرجع;

      افتراضي:
	dict_opset_t::process_op (op, env);
	إذا (!env.arg_ڍير.is_empty ()) أرجع;
	اكسر;
    }

    إذا (unlikely (env.اندريون_error ())) أرجع;

    // Write args then op

    str_buff_t str;
    str_encoder_t encoder (str);

    بنانشان ڳڻپ = env.arg_ڍير.get_ڳڻپ ();
    لكل (بنانشان i = 0; i < ڳڻپ; i++)
      encoder.encode_انگ_tp (env.arg_ڍير[i]);

    encoder.encode_op (op);

    خودڪار بائيٽون = str.as_بائيٽون ();
    param.c->embed (&بائيٽون, بائيٽون.ڊيگهائي);

    env.clear_دليلون ();
    param.fold_exact = param.fold_emitted = 0.;
  }
};

بنية cff2_ذاتي_dict_op_serializer_t : op_serializer_t
{
  cff2_ذاتي_dict_op_serializer_t (منطقي desubroutinize_, منطقي drop_hints_, منطقي pinned_,
				     ثابت CFF::CFF2_شي_Variation_Store* varStore_,
				     hb_array_t<صحيح> normalized_coords_,
				     ثابت cff2_instancing_plan_t *instancer_ = خالي_اشارو)
    : desubroutinize (desubroutinize_), drop_hints (drop_hints_), pinned (pinned_),
      param (خالي_اشارو, varStore_, normalized_coords_)
  { param.instancer = instancer_; }

  منطقي serialize (hb_serialize_context_t *c,
		  ثابت op_str_t &opstr,
		  objidx_t subrs_link) ثابت
  {
    TRACE_SERIALIZE (هذا);

    إذا (drop_hints && dict_opset_t::is_hint_op (opstr.op))
      return_trace (سچ);

    إذا (opstr.op == Op_ڪوڊ_Subrs)
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
      cff2_priv_dict_interp_env_t env {hb_ubytes_t (opstr.ptr, opstr.ڊيگهائي)};
      param.c = c;
      dict_interpreter_t<cff2_ذاتي_dict_blend_opset_t, cff2_ذاتي_blend_encoder_param_t, cff2_priv_dict_interp_env_t> interp (env);
      return_trace (interp.interpret (param));
    }

    return_trace (copy_opstr (c, opstr));
  }

  محمي:
  ثابت منطقي desubroutinize;
  ثابت منطقي drop_hints;
  ثابت منطقي pinned;
  بدلجندڙ cff2_ذاتي_blend_encoder_param_t param;
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

    انگ_glyphs = plan->انگ_output_glyphs ();
    orig_fdcount = acc.fdArray->ڳڻپ;

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
			 acc.varStore != &Null (CFF2_شي_Variation_Store);
    إذا (partial_instancing &&
	unlikely (!instancing.create (acc.varStore, plan)))
      أرجع خطأ;
    axis_ٽيگ_فهرست = &plan->axis_ٽيگ_فهرست;

    /* Enable command capture for CFF2→CFF1 conversion (for specialization) */
    capture_commands = pinned;

 #جيڪڏهن_وصف_ٿيل HB_EXPERIMENTAL_API
    min_charstrings_off_سائيز = (plan->flags & HB_SUBSET_FLAGS_IFTB_REQUIREMENTS) ? 4 : 0;
 #وإلا
    min_charstrings_off_سائيز = 0;
 #جيڪڏهن_پورو

    إذا (desubroutinize)
    {
      إذا (partial_instancing)
      {
	/* Pass 1: collect which VarDatas the retained blends reference, so
	 * unused ones get pruned and the survivors renumbered. */
	hb_سيٽ_t استعمال_ٿيل_fds;
	لكل (بنانشان صحيح i = 0; i < انگ_glyphs; i++)
	{
	  hb_codepoint_t glyph;
	  إذا (!plan->old_gid_for_new_gid (i, &glyph))
	    استمر;
	  ثابت hb_ubytes_t str = (*acc.char_لڙيون)[glyph];
	  بنانشان صحيح fd = CFF::subset_get_fd (acc, glyph);
	  إذا (unlikely (fd >= acc.fd_ڳڻپ))
	    أرجع خطأ;
	  استعمال_ٿيل_fds.جوڙيو (fd);

	  cff2_cs_interp_env_t<blend_arg_t> env (str, acc, fd);
	  cs_interpreter_t<cff2_cs_interp_env_t<blend_arg_t>, cff2_cs_opset_usage_t, cff2_usage_param_t> interp (env);
	  cff2_usage_param_t param;
	  param.instancing = &instancing;
	  param.drop_hints = drop_hints;
	  إذا (unlikely (!interp.interpret (param)))
	    أرجع خطأ;
	}
	لكل (بنانشان fd : استعمال_ٿيل_fds)
	{
	  cff2_usage_param_t param;
	  param.instancing = &instancing;
	  بنانشان ڳڻپ = acc.ذاتي_Dicts[fd].get_ڳڻپ ();
	  لكل (بنانشان j = 0; j < ڳڻپ; j++)
	  {
	    ثابت op_str_t &opstr = acc.ذاتي_Dicts[fd][j];
	    إذا (drop_hints && dict_opset_t::is_hint_op (opstr.op))
	      استمر;
	    cff2_priv_dict_interp_env_t env {hb_ubytes_t (opstr.ptr, opstr.ڊيگهائي)};
	    dict_interpreter_t<cff2_ذاتي_dict_usage_opset_t, cff2_usage_param_t, cff2_priv_dict_interp_env_t> interp (env);
	    إذا (unlikely (!interp.interpret (param)))
	      أرجع خطأ;
	  }
	}
	إذا (unlikely (!instancing.finalize ()))
	  أرجع خطأ;

	/* Pass 2: like subr_flattener_t, but blends stay symbolic (no coords
	 * fed to the env) and get rewritten against the instanced store. */
	إذا (!subset_charstrings.resize_exact (انگ_glyphs))
	  أرجع خطأ;
	لكل (بنانشان صحيح i = 0; i < انگ_glyphs; i++)
	{
	  hb_codepoint_t glyph;
	  إذا (!plan->old_gid_for_new_gid (i, &glyph))
	    استمر;
	  ثابت hb_ubytes_t str = (*acc.char_لڙيون)[glyph];
	  بنانشان صحيح fd = CFF::subset_get_fd (acc, glyph);
	  إذا (unlikely (fd >= acc.fd_ڳڻپ))
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
	إذا (!charstring_commands.resize_exact (انگ_glyphs))
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
      لكل (بنانشان صحيح fd = 0; fd < orig_fdcount; fd++)
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
						  acc.انگ_glyphs,
						  subset_fdcount,
						  subset_fdselect_سائيز,
						  subset_fdselect_format,
						  subset_fdselect_ranges,
						  fdmap)))
	أرجع خطأ;
    }
    وإلا
      fdmap.سڃاڻپ_عنصر (1);

    أرجع سچ;
  }

  cff2_sub_جدول_info_t info;

  بنانشان صحيح    انگ_glyphs;
  بنانشان صحيح    orig_fdcount = 0;
  بنانشان صحيح    subset_fdcount = 1;
  بنانشان صحيح    subset_fdselect_سائيز = 0;
  بنانشان صحيح    subset_fdselect_format = 0;
  منطقي            pinned = خطأ;
  hb_ويڪٽر_t<ڪوڊ_pair_t>   subset_fdselect_ranges;

  hb_inc_bimap_t   fdmap;

  str_buff_vec_t	    subset_charstrings;
  str_buff_vec_t	    subset_globalsubrs;
  hb_ويڪٽر_t<str_buff_vec_t> subset_localsubrs;

  منطقي	    drop_hints = خطأ;
  منطقي	    desubroutinize = خطأ;

  منطقي	    partial_instancing = خطأ;
  cff2_instancing_plan_t instancing;
  ثابت hb_ويڪٽر_t<hb_ٽيگ_t> *axis_ٽيگ_فهرست = خالي_اشارو;

  بنانشان  min_charstrings_off_سائيز = 0;

  hb_array_t<صحيح> normalized_coords; // For instantiation
  سر_maxp_info_t سر_maxp_info;  // For FontBBox
  ثابت hb_hashmap_t<hb_codepoint_t, hb_pair_t<بنانشان, صحيح>> *hmtx_map; // For widths

  // Width optimization results (for CFF1 conversion)
  بنانشان default_ويڪر = 0;
  بنانشان nominal_ويڪر = 0;

  // Command capture for specialization (CFF2→CFF1 conversion)
  منطقي capture_commands = خطأ;
  hb_ويڪٽر_t<hb_ويڪٽر_t<cs_command_t>> charstring_commands;
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
                             بنانشان default_ويڪر,
                             بنانشان nominal_ويڪر)
{
  c->push ();

  // CFF1 requires:
  // 1. Width at the beginning (if != defaultWidthX)
  // 2. endchar at the end
  str_buff_vec_t cff1_charstrings;
  إذا (unlikely (!cff1_charstrings.resize (plan.subset_charstrings.ڊيگهائي)))
  {
    c->ڪڍو_discard ();
    أرجع خطأ;
  }

  لكل (بنانشان i = 0; i < plan.subset_charstrings.ڊيگهائي; i++)
  {
    // Get width for this glyph from hmtx_map
    بنانشان ويڪر = 0;
    إذا (plan.hmtx_map->has (i))
      ويڪر = plan.hmtx_map->get (i).پهريون;

    // Encode width if different from default
    str_encoder_t encoder (cff1_charstrings[i]);
    إذا (ويڪر != default_ويڪر)
    {
      صحيح delta = (صحيح) ويڪر - (صحيح) nominal_ويڪر;
      encoder.encode_int (delta);
    }

    // Use specialized commands if available, otherwise use binary
    إذا (plan.capture_commands && i < plan.charstring_commands.ڊيگهائي &&
        plan.charstring_commands[i].ڊيگهائي > 0)
    {
      // Specialize and encode commands
      خودڪار &commands = plan.charstring_commands[i];
      CFF::specialize_commands (commands, 48);  /* maxstack=48 for CFF1 */
      إذا (unlikely (!CFF::encode_commands (commands, cff1_charstrings[i])))
      {
        c->ڪڍو_discard ();
        أرجع خطأ;
      }
    }
    وإلا
    {
      // Use binary CharString
      ثابت str_buff_t &cs = plan.subset_charstrings[i];
      لكل (بنانشان j = 0; j < cs.ڊيگهائي; j++)
        cff1_charstrings[i].push (cs[j]);
    }

    // Check if it already ends with endchar (0x0e) or return (0x0b)
    إذا (cff1_charstrings[i].ڊيگهائي == 0 ||
        (cff1_charstrings[i].پڇ () != 0x0e && cff1_charstrings[i].پڇ () != 0x0b))
    {
      // Append endchar operator
      إذا (unlikely (!cff1_charstrings[i].ڌڪو_or_fail (0x0e)))
      {
        c->ڪڍو_discard ();
        أرجع خطأ;
      }
    }
  }

  بنانشان معلومات_سائيز = 0;
  بنانشان ڪل_سائيز = CFF1_Char_لڙيون::ڪل_سائيز (cff1_charstrings, &معلومات_سائيز, plan.min_charstrings_off_سائيز);
  إذا (unlikely (!c->شروعات_zerocopy (ڪل_سائيز)))
  {
    c->ڪڍو_discard ();
    أرجع خطأ;
  }

  خودڪار *cs = c->شروعات_embed<CFF1_Char_لڙيون> ();
  إذا (unlikely (!cs->serialize (c, cff1_charstrings, &معلومات_سائيز, plan.min_charstrings_off_سائيز)))
  {
    c->ڪڍو_discard ();
    أرجع خطأ;
  }

  plan.info.char_لڙيون_link = c->ڪڍو_pack (خطأ);
  أرجع سچ;
}

/* Serialize CID Charset (format 2 range: gid 0-N -> cid 0-N) */
ساكن منطقي
_serialize_cff1_charset (hb_serialize_context_t *c,
                         بنانشان صحيح انگ_glyphs,
                         objidx_t &charset_link)
{
  // For CID fonts, create a simple identity charset
  // Format 2: one range covering all glyphs (except .notdef)
  c->push ();

  خودڪار *charset = c->شروعات_embed<Charset> ();
  إذا (unlikely (!charset))
  {
    c->ڪڍو_discard ();
    أرجع خطأ;
  }

  // Create a single range for CID 1 to num_glyphs-1
  hb_ويڪٽر_t<ڪوڊ_pair_t> ranges;
  إذا (انگ_glyphs > 1)
  {
    ڪوڊ_pair_t range;
    range.ڪوڊ = 1;  // first CID
    range.glyph = انگ_glyphs - 2;  // nLeft (covers glyphs 1 to num_glyphs-1)
    ranges.push (range);
  }

  إذا (unlikely (!charset->serialize (c, 2, انگ_glyphs, ranges)))
  {
    c->ڪڍو_discard ();
    أرجع خطأ;
  }

  charset_link = c->ڪڍو_pack ();
  أرجع سچ;
}

/* CFF2 to CFF1 serialization */
نطاق CFF {

منطقي
serialize_cff2_منزل_cff1 (hb_serialize_context_t *c,
                        OT::cff2_subset_plan &plan,
                        ثابت cff2_چوٽي_dict_قدرون_t &cff2_چوٽي_Dict,
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
    hb_ويڪٽر_t<بنانشان> widths;
    widths.alloc (plan.انگ_glyphs);

    لكل (بنانشان gid = 0; gid < plan.انگ_glyphs; gid++)
    {
      بنانشان ويڪر = 0;
      إذا (plan.hmtx_map->has (gid))
        ويڪر = plan.hmtx_map->get (gid).پهريون;
      widths.push (ويڪر);
    }

    // Optimize defaultWidthX and nominalWidthX
    CFF::optimize_widths (widths, plan.default_ويڪر, plan.nominal_ويڪر);
  }

  // 1. CharStrings (with widths prepended)
  إذا (!_serialize_cff1_charstrings (c, plan, plan.default_ويڪر, plan.nominal_ويڪر))
    return_trace (خطأ);

  // 2. Private DICs & Local Subrs (same as CFF2)
  hb_ويڪٽر_t<جدول_info_t> ذاتي_dict_infos;
  إذا (unlikely (!ذاتي_dict_infos.resize (plan.subset_fdcount)))
    return_trace (خطأ);

  لكل (صحيح i = (صحيح)acc.ذاتي_Dicts.ڊيگهائي; --i >= 0;)
  {
    إذا (plan.fdmap.has (i))
    {
      objidx_t subrs_link = 0;

      إذا (plan.subset_localsubrs[i].ڊيگهائي > 0)
      {
        خودڪار *dest = c->push<CFF1Subrs> ();
        إذا (likely (dest->serialize (c, plan.subset_localsubrs[i])))
          subrs_link = c->ڪڍو_pack (خطأ);
        وإلا
        {
          c->ڪڍو_discard ();
          return_trace (خطأ);
        }
      }

      خودڪار *pd = c->push<ذاتي_Dict> ();
      // Use the CFF2 Private DICT serializer which instantiates blends when pinned=true
      cff2_ذاتي_dict_op_serializer_t privSzr (plan.desubroutinize, plan.drop_hints, plan.pinned,
                                                  acc.varStore, plan.normalized_coords);
      إذا (likely (pd->serialize (c, acc.ذاتي_Dicts[i], privSzr, subrs_link)))
      {
        // Add defaultWidthX and nominalWidthX for CFF1
        str_buff_t ويڪر_ops;
        str_encoder_t encoder (ويڪر_ops);
        encoder.encode_int (plan.default_ويڪر);
        encoder.encode_op (Op_ڪوڊ_default_ويڪر_X);
        encoder.encode_int (plan.nominal_ويڪر);
        encoder.encode_op (Op_ڪوڊ_nominal_ويڪر_X);

        إذا (!encoder.اندريون_error () && c->embed (ويڪر_ops.as_بائيٽون ().arrayZ, ويڪر_ops.ڊيگهائي))
        {
          بنانشان fd = plan.fdmap[i];
          ذاتي_dict_infos[fd].سائيز = c->ڊيگهائي ();
          ذاتي_dict_infos[fd].link = c->ڪڍو_pack ();
        }
        وإلا
        {
          c->ڪڍو_discard ();
          return_trace (خطأ);
        }
      }
      وإلا
      {
        c->ڪڍو_discard ();
        return_trace (خطأ);
      }
    }
  }

  // 3. FDArray - serialize CFF2 font dicts as CFF1
  {
    خودڪار *fda = c->push<FDArray<HBUINT16>> ();
    cff_font_dict_op_serializer_t fontSzr;
    خودڪار it =
    + hb_zip (+ hb_iter (acc.fontDicts)
              | hb_filter ([&] (ثابت cff2_font_dict_قدرون_t &_)
                { أرجع plan.fdmap.has (&_ - &acc.fontDicts[0]); }),
              hb_iter (ذاتي_dict_infos))
    ;
    // Explicitly specify template parameters: DICTVAL, INFO
    منطقي success = fda->serialize<cff2_font_dict_قدرون_t, جدول_info_t> (c, it, fontSzr);
    إذا (success)
      plan.info.fd_array_link = c->ڪڍو_pack (خطأ);
    وإلا
    {
      c->ڪڍو_discard ();
      return_trace (خطأ);
    }
  }

  // 4. FDSelect (required in CFF1 CID-keyed fonts)
  // CFF1 requires FDSelect for all CID-keyed fonts, even with just one FD
  // CFF2 makes it optional when there's only one FD
  إذا (acc.fdSelect != &Null (CFF2FDSelect))
  {
    c->push ();
    إذا (likely (hb_serialize_cff_fdselect (c, plan.انگ_glyphs,
                                          *(ثابت FDSelect *)acc.fdSelect,
                                          plan.orig_fdcount,
                                          plan.subset_fdselect_format,
                                          plan.subset_fdselect_سائيز,
                                          plan.subset_fdselect_ranges)))
      plan.info.fd_select.link = c->ڪڍو_pack ();
    وإلا
    {
      c->ڪڍو_discard ();
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
      c->ڪڍو_discard ();
      return_trace (خطأ);
    }

    // nRanges
    HBUINT16 nRanges;
    nRanges = 1;
    إذا (unlikely (!c->embed (nRanges)))
    {
      c->ڪڍو_discard ();
      return_trace (خطأ);
    }

    // Single range: {first: 0, fd: 0}
    FDSelect3_Range range;
    range.پهريون = 0;
    range.fd = 0;
    إذا (unlikely (!c->embed (range)))
    {
      c->ڪڍو_discard ();
      return_trace (خطأ);
    }

    // Sentinel (number of glyphs)
    HBUINT16 sentinel;
    sentinel = plan.انگ_glyphs;
    إذا (unlikely (!c->embed (sentinel)))
    {
      c->ڪڍو_discard ();
      return_trace (خطأ);
    }

    plan.info.fd_select.link = c->ڪڍو_pack ();
  }

  // 5. Charset (CID charset for identity mapping)
  objidx_t charset_link;
  إذا (!_serialize_cff1_charset (c, plan.انگ_glyphs, charset_link))
    return_trace (خطأ);

  // 6. Global Subrs
  {
    خودڪار *dest = c->push<CFF1Subrs> ();
    إذا (likely (dest->serialize (c, plan.subset_globalsubrs)))
      c->ڪڍو_pack (خطأ);
    وإلا
    {
      c->ڪڍو_discard ();
      return_trace (خطأ);
    }
  }

  // 7. String INDEX - Add "Adobe" and "Identity" for ROS operator
  {
    ثابت حرف *adobe_str = "Adobe";
    ثابت حرف *سڃاڻپ_عنصر_str = "Identity";
    بنانشان adobe_len = 5;  // strlen("Adobe")
    بنانشان سڃاڻپ_عنصر_len = 8;  // strlen("Identity")

    // Build strings array
    hb_ويڪٽر_t<hb_ubytes_t> لڙيون;
    لڙيون.alloc (2);
    لڙيون.push (hb_ubytes_t ((ثابت بنانشان حرف *) adobe_str, adobe_len));
    لڙيون.push (hb_ubytes_t ((ثابت بنانشان حرف *) سڃاڻپ_عنصر_str, سڃاڻپ_عنصر_len));

    // Serialize as CFF INDEX
    خودڪار *dest = c->push<CFF1_اشاريو> ();
    إذا (likely (dest->serialize (c, لڙيون)))
      c->ڪڍو_pack (خطأ);
    وإلا
    {
      c->ڪڍو_discard ();
      return_trace (خطأ);
    }
  }

  // 8. CFF Header
  OT::cff1 *cff = c->allocate_min<OT::cff1> ();
  إذا (unlikely (!cff)) return_trace (خطأ);

  /*سرو*/
  cff->نسخو.major = 0x01;
  cff->نسخو.minor = 0x00;
  cff->name_اشاريو = cff->min_سائيز;
  cff->off_سائيز = 4; /* unused? */

  // 9. Name INDEX (single entry)
  {
    بنانشان name_len = لڙي_ڊيگهه (CFF1_DEFAULT_FONT_NAME);

    CFF1_اشاريو *idx = c->شروعات_embed<CFF1_اشاريو> ();
    إذا (unlikely (!idx)) return_trace (خطأ);

    إذا (unlikely (!idx->serialize_سرو (c, hb_iter (&name_len, 1), name_len)))
      return_trace (خطأ);

    إذا (unlikely (!c->embed (CFF1_DEFAULT_FONT_NAME, name_len)))
      return_trace (خطأ);
  }

  // 10. Top DICT INDEX
  {
    // Serialize the Top DICT data first
    c->push<چوٽي_Dict> ();
    cff1_from_cff2_چوٽي_dict_op_serializer_t چوٽي_Szr;

    // Serialize ROS first
    إذا (unlikely (!چوٽي_Szr.serialize_ros (c)))
    {
      c->ڪڍو_discard ();
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
      encoder.encode_op (Op_ڪوڊ_Font_BBox);

      إذا (encoder.اندريون_error () || !c->embed (bbox_buff.as_بائيٽون ().arrayZ, bbox_buff.ڊيگهائي))
      {
        c->ڪڍو_discard ();
        return_trace (خطأ);
      }
    }

    // Serialize charset operator
    إذا (charset_link && unlikely (!FontDict::serialize_link4_op (c, Op_ڪوڊ_charset, charset_link, whence_t::Absolute)))
    {
      c->ڪڍو_discard ();
      return_trace (خطأ);
    }

    // Serialize FDSelect operator (required for CID-keyed CFF1 fonts)
    إذا (plan.info.fd_select.link && unlikely (!FontDict::serialize_link4_op (c, Op_ڪوڊ_FDSelect, plan.info.fd_select.link, whence_t::Absolute)))
    {
      c->ڪڍو_discard ();
      return_trace (خطأ);
    }

    // Serialize FDArray operator (required for CID-keyed CFF1 fonts)
    إذا (plan.info.fd_array_link && unlikely (!FontDict::serialize_link4_op (c, Op_ڪوڊ_FDArray, plan.info.fd_array_link, whence_t::Absolute)))
    {
      c->ڪڍو_discard ();
      return_trace (خطأ);
    }

    // Serialize other operators from CFF2 TopDict
    لكل (ثابت خودڪار &opstr : cff2_چوٽي_Dict.values)
    {
      إذا (unlikely (!چوٽي_Szr.serialize (c, opstr, plan.info)))
      {
        c->ڪڍو_discard ();
        return_trace (خطأ);
      }
    }

    بنانشان چوٽي_سائيز = c->ڊيگهائي ();
    c->ڪڍو_pack (خطأ);

    // Serialize INDEX header
    خودڪار *dest = c->شروعات_embed<CFF1_اشاريو> ();
    إذا (unlikely (!dest->serialize_سرو (c, hb_iter (&چوٽي_سائيز, 1), چوٽي_سائيز)))
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

  بنانشان معلومات_سائيز = 0;
  بنانشان ڪل_سائيز = CFF2_Char_لڙيون::ڪل_سائيز (plan.subset_charstrings, &معلومات_سائيز, plan.min_charstrings_off_سائيز);
  إذا (unlikely (!c->شروعات_zerocopy (ڪل_سائيز)))
    أرجع خطأ;

  خودڪار *cs = c->شروعات_embed<CFF2_Char_لڙيون> ();
  إذا (unlikely (!cs->serialize (c, plan.subset_charstrings, &معلومات_سائيز, plan.min_charstrings_off_سائيز)))
  {
    c->ڪڍو_discard ();
    أرجع خطأ;
  }

  plan.info.char_لڙيون_link = c->ڪڍو_pack (خطأ);
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
  hb_ويڪٽر_t<جدول_info_t>  ذاتي_dict_infos;
  إذا (unlikely (!ذاتي_dict_infos.resize (plan.subset_fdcount))) أرجع خطأ;

  لكل (صحيح i = (صحيح)ذاتي_Dicts.ڊيگهائي; --i >= 0 ;)
  {
    إذا (plan.fdmap.has (i))
    {
      objidx_t	subrs_link = 0;

      إذا (plan.subset_localsubrs[i].ڊيگهائي > 0)
      {
	خودڪار *dest = c->push <CFF2Subrs> ();
	إذا (likely (dest->serialize (c, plan.subset_localsubrs[i])))
	  subrs_link = c->ڪڍو_pack (خطأ);
	وإلا
	{
	  c->ڪڍو_discard ();
	  أرجع خطأ;
	}
      }
      خودڪار *pd = c->push<ذاتي_Dict> ();
      cff2_ذاتي_dict_op_serializer_t privSzr (plan.desubroutinize, plan.drop_hints, plan.pinned,
						 varStore, normalized_coords,
						 plan.partial_instancing ? &plan.instancing : خالي_اشارو);
      إذا (likely (pd->serialize (c, ذاتي_Dicts[i], privSzr, subrs_link)))
      {
	بنانشان fd = plan.fdmap[i];
	ذاتي_dict_infos[fd].سائيز = c->ڊيگهائي ();
	ذاتي_dict_infos[fd].link = c->ڪڍو_pack ();
      }
      وإلا
      {
	c->ڪڍو_discard ();
	أرجع خطأ;
      }
    }
  }

  /* FDSelect */
  إذا (fdSelect != &Null (CFF2FDSelect))
  {
    c->push ();
    إذا (likely (hb_serialize_cff_fdselect (c, plan.انگ_glyphs, *(ثابت FDSelect *)fdSelect,
					   plan.orig_fdcount,
					   plan.subset_fdselect_format, plan.subset_fdselect_سائيز,
					   plan.subset_fdselect_ranges)))
      plan.info.fd_select.link = c->ڪڍو_pack ();
    وإلا
    {
      c->ڪڍو_discard ();
      أرجع خطأ;
    }
  }

  /* FDArray (FD Index) */
  {
    خودڪار *fda = c->push<CFF2FDArray> ();
    cff_font_dict_op_serializer_t fontSzr;
    خودڪار it =
    + hb_zip (+ hb_iter (fontDicts)
	      | hb_filter ([&] (ثابت cff2_font_dict_قدرون_t &_)
		{ أرجع plan.fdmap.has (&_ - &fontDicts[0]); }),
	      hb_iter (ذاتي_dict_infos))
    ;
    إذا (unlikely (!fda->serialize (c, it, fontSzr)))
    {
      c->ڪڍو_discard ();
      أرجع خطأ;
    }
    plan.info.fd_array_link = c->ڪڍو_pack (خطأ);
  }

  /* variation store */
  إذا (varStore != &Null (CFF2_شي_Variation_Store) &&
      !plan.pinned)
  {
    إذا (plan.partial_instancing)
    {
      إذا (plan.instancing.has_variations ())
      {
	c->push ();
	إذا (unlikely (!plan.instancing.serialize_var_store (c, *plan.axis_ٽيگ_فهرست)))
	{
	  c->ڪڍو_discard ();
	  أرجع خطأ;
	}
	plan.info.var_store_link = c->ڪڍو_pack (خطأ);
      }
      /* else: the store emptied; the vstore op gets dropped along with it. */
    }
    وإلا
    {
      خودڪار *dest = c->push<CFF2_شي_Variation_Store> ();
      إذا (unlikely (!dest->serialize (c, varStore)))
      {
	c->ڪڍو_discard ();
	أرجع خطأ;
      }
      plan.info.var_store_link = c->ڪڍو_pack (خطأ);
    }
  }

  OT::cff2 *cff2 = c->allocate_min<OT::cff2> ();
  إذا (unlikely (!cff2)) أرجع خطأ;

  /*سرو*/
  cff2->نسخو.major = 0x02;
  cff2->نسخو.minor = 0x00;
  cff2->چوٽي_Dict_2 = OT::cff2::static_سائيز;

  /* top dict */
  {
    چوٽي_Dict &dict = cff2 + cff2->چوٽي_Dict_2;
    cff2_چوٽي_dict_op_serializer_t چوٽي_Szr;
    إذا (unlikely (!dict.serialize (c, چوٽي_Dict_2, چوٽي_Szr, plan.info))) أرجع خطأ;
    cff2->چوٽي_Dict_سائيز = c->سر_2 - (ثابت حرف *)&dict;
  }

  /* global subrs */
  {
    خودڪار *dest = c->شروعات_embed <CFF2Subrs> ();
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
    منطقي نتيجو = CFF::serialize_cff2_منزل_cff1 (c->serializer, cff2_plan, چوٽي_Dict_2, *هذا);

    إذا (c->serializer->ran_ٻاهريون_جو_گنجائش ())
      أرجع سچ; // Subsetter will retry with larger buffer

    إذا (نتيجو && !c->serializer->اندريون_error ())
    {
      // Success - end serialization to resolve links
      c->serializer->پڇاڙي_serialize ();

      // Copy the serialized CFF1 data and add as CFF table
      hb_blob_t *cff_blob = c->serializer->copy_blob ();
      إذا (cff_blob)
      {
        c->plan->جوڙيو_جدول (HB_ٽيگ('C','F','F',' '), cff_blob);
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

#جيڪڏهن_پورو
