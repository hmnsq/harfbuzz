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

#ifndef HB_NO_SUBSET_CFF

#شامل "hb-open-type.hh"
#شامل "hb-ot-cff2-table.hh"
#شامل "hb-ot-var-common.hh"
#شامل "hb-set.h"
#شامل "hb-subset-plan.hh"
#شامل "hb-subset-cff-common.hh"
#شامل "hb-cff2-interp-cs.hh"
#شامل "hb-subset-cff2-to-cff1.hh"

using فضاینام CFF;

فضاینام CFF {

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
ساختار cff2_instancing_plan_t
{
  ساختار vardata_transform_t
  {
    /* new_deltas[j] = Σ_i matrix[j][i] * old_deltas[i] */
    hb_vector_t<hb_vector_t<اعشاری>> matrix;
    /* Default-value fold-in scalars, one per old region: the old store's
     * region scalars at the new default location. */
    hb_vector_t<اعشاری> gains;
    /* Surviving regions, parallel to matrix rows. */
    hb_vector_t<hb_hashmap_t<hb_tag_t, Triple>> regions;
    /* Index into the new global region list, per new region (finalize). */
    hb_vector_t<unsigned> region_indices;
    unsigned new_major = 0;
    منطقی alive_ = نادرست;
    منطقی alive () ثابت { بازگشت alive_; }

    /* One value's delta for new region j, from its old per-region deltas. */
    دوگانه transform_delta (unsigned j, hb_array_t<ثابت عدد_t> deltas) ثابت
    {
      ثابت hb_vector_t<اعشاری> &col = matrix.arrayZ[j];
      دوگانه d = 0.;
      unsigned شمار = hb_min (col.length, deltas.length);
      برای (unsigned c = 0; c < شمار; c++)
	d += (دوگانه) col.arrayZ[c] * deltas.arrayZ[c].to_real ();
      بازگشت d;
    }
  };

  hb_vector_t<vardata_transform_t> transforms;  /* per old VarData (vsindex) */
  hb_vector_t<hb_hashmap_t<hb_tag_t, Triple>> new_regions;  /* global region list */
  hb_set_t used_majors;
  unsigned new_major_شمار = 0;

  منطقی has_variations () ثابت { بازگشت new_major_شمار; }

  ثابت vardata_transform_t *get_transform (unsigned major) ثابت
  { بازگشت major < transforms.length ? &transforms.arrayZ[major] : nullptr; }

  /* Record that a retained blend references this VarData. */
  تهی note_use (unsigned major) { used_majors.add (major); }

  /* The new vsindex a private dict's (or charstring's) old ivs maps to;
   * dead VarDatas fall back to 0, their blends having become static. */
  unsigned remap_ivs (unsigned old_ivs) ثابت
  {
    ثابت vardata_transform_t *t = get_transform (old_ivs);
    بازگشت (t && t->alive ()) ? t->new_major : 0;
  }

  دوگانه fold_deltas (unsigned old_ivs, hb_array_t<ثابت عدد_t> deltas) ثابت
  {
    ثابت vardata_transform_t *t = get_transform (old_ivs);
    اگر (unlikely (!t || t->gains.length != deltas.length)) بازگشت 0.;
    دوگانه v = 0.;
    برای (unsigned i = 0; i < deltas.length; i++)
      v += (دوگانه) t->gains.arrayZ[i] * deltas.arrayZ[i].to_real ();
    بازگشت v;
  }

  منطقی create (ثابت CFF2ItemVariationStore *cff2VarStore,
	       ثابت hb_subset_plan_t *plan)
  {
    ثابت OT::ItemVariationStore &varStore = cff2VarStore->varStore;
    unsigned major_شمار = varStore.get_sub_table_شمار ();
    اگر (!transforms.resize (major_شمار)) بازگشت نادرست;

    اگر (!varStore.get_region_list ().get_var_regions (plan->axes_old_index_tag_map, orig_regions))
      بازگشت نادرست;

    /* Instance each VarData with identity delta rows; the surviving tuples'
     * delta columns are the old-region → new-region transform. */
    OT::optimize_scratch_t scratch;
    برای (unsigned m = 0; m < major_شمار; m++)
    {
      ثابت OT::VarData &var_data = varStore.get_sub_table (m);
      unsigned k = var_data.get_region_index_شمار ();
      vardata_transform_t &t = transforms.arrayZ[m];

      اگر (!t.gains.resize_exact (k)) بازگشت نادرست;
      varStore.get_region_scalars (m, plan->normalized_coords.arrayZ,
				   plan->normalized_coords.length,
				   t.gains.arrayZ, k);

      OT::tuple_variations_t tuples;
      برای (unsigned r = 0; r < k; r++)
      {
	OT::tuple_delta_t tuple;
	اگر (!tuple.deltas_x.resize (k) ||
	    !tuple.indices.resize (k))
	  بازگشت نادرست;
	برای (unsigned i = 0; i < k; i++)
	  tuple.indices.arrayZ[i] = درست;
	tuple.deltas_x.arrayZ[r] = 1.f;
	unsigned region_index = var_data.get_region_index (r);
	اگر (unlikely (region_index >= orig_regions.length)) بازگشت نادرست;
	tuple.axis_tuples = orig_regions.arrayZ[region_index];
	اگر (unlikely (tuple.axis_tuples.in_error ())) بازگشت نادرست;
	tuples.tuple_vars.push (std::move (tuple));
      }
      اگر (unlikely (tuples.tuple_vars.in_error ())) بازگشت نادرست;

      اگر (!tuples.instantiate (plan->axes_location, plan->axes_triple_distances, scratch))
	بازگشت نادرست;

      برای (auto &tuple : tuples.tuple_vars)
      {
	اگر (unlikely (tuple.deltas_x.length != k)) بازگشت نادرست;
	hb_vector_t<اعشاری> col;
	اگر (unlikely (!col.resize_exact (k))) بازگشت نادرست;
	برای (unsigned i = 0; i < k; i++)
	  col.arrayZ[i] = tuple.deltas_x.arrayZ[i];
	t.matrix.push (std::move (col));
	t.regions.push (std::move (tuple.axis_tuples));
      }
      اگر (unlikely (t.matrix.in_error () || t.regions.in_error ())) بازگشت نادرست;
    }

    بازگشت درست;
  }

  /* Decide which VarDatas survive, number them, and build the new global
   * region list: surviving original regions first, in their original order,
   * then solver-split new regions in order of appearance (fontTools'
   * rebuildRegions + prune_regions). */
  منطقی finalize ()
  {
    برای (unsigned m = 0; m < transforms.length; m++)
    {
      vardata_transform_t &t = transforms.arrayZ[m];
      t.alive_ = t.matrix.length && used_majors.has (m);
      اگر (t.alive_)
	t.new_major = new_major_شمار++;
    }

    /* Keys hash by content, so pointers into different owners compare fine. */
    hb_hashmap_t<ثابت hb_hashmap_t<hb_tag_t, Triple>*, unsigned> region_idx_map;
    hb_vector_t<ثابت hb_hashmap_t<hb_tag_t, Triple>*> region_ptrs;
    برای (ثابت vardata_transform_t &t : transforms)
    {
      اگر (!t.alive ()) ادامه;
      برای (ثابت auto &region : t.regions)
	اگر (!region_idx_map.has (&region))
	  اگر (unlikely (!region_idx_map.set (&region, (unsigned) -1)))
	    بازگشت نادرست;
    }
    برای (ثابت auto &r : orig_regions)
    {
      unsigned *idx;
      اگر (region_idx_map.has (&r, &idx) && *idx == (unsigned) -1)
      {
	اگر (unlikely (!region_idx_map.set (&r, region_ptrs.length))) بازگشت نادرست;
	region_ptrs.push (&r);
      }
    }
    برای (ثابت vardata_transform_t &t : transforms)
    {
      اگر (!t.alive ()) ادامه;
      برای (ثابت auto &region : t.regions)
      {
	unsigned *idx;
	اگر (region_idx_map.has (&region, &idx) && *idx == (unsigned) -1)
	{
	  اگر (unlikely (!region_idx_map.set (&region, region_ptrs.length))) بازگشت نادرست;
	  region_ptrs.push (&region);
	}
      }
    }
    اگر (unlikely (region_ptrs.in_error ())) بازگشت نادرست;

    برای (vardata_transform_t &t : transforms)
    {
      اگر (!t.alive ()) ادامه;
      برای (ثابت auto &region : t.regions)
      {
	unsigned *idx;
	اگر (unlikely (!region_idx_map.has (&region, &idx))) بازگشت نادرست;
	t.region_indices.push (*idx);
      }
      اگر (unlikely (t.region_indices.in_error ())) بازگشت نادرست;
    }

    اگر (!new_regions.resize (region_ptrs.length)) بازگشت نادرست;
    برای (unsigned i = 0; i < region_ptrs.length; i++)
    {
      new_regions.arrayZ[i] = *region_ptrs.arrayZ[i];
      اگر (unlikely (new_regions.arrayZ[i].in_error ())) بازگشت نادرست;
    }

    بازگشت درست;
  }

  /* Serialize the instanced store: a CFF2ItemVariationStore whose VarDatas
   * carry no items, just region indexes (blend deltas live in charstrings).
   * Laid out manually since all sizes are known up front. */
  منطقی serialize_var_store (hb_serialize_context_t *c,
			    ثابت hb_vector_t<hb_tag_t> &axis_tags) ثابت
  {
    unsigned data_شمار = new_major_شمار;
    unsigned header_size = 2 + 4 + 2 + 4 * data_شمار;

    uint64_t offset = header_size;
    hb_vector_t<unsigned> data_offsets;
    برای (ثابت vardata_transform_t &t : transforms)
    {
      اگر (!t.alive ()) ادامه;
      data_offsets.push ((unsigned) offset);
      offset += 6 + 2 * (uint64_t) t.region_indices.length;
    }
    اگر (unlikely (data_offsets.in_error () || data_offsets.length != data_شمار))
      بازگشت نادرست;
    unsigned regions_offset = (unsigned) offset;
    uint64_t total = offset + 4 + (uint64_t) new_regions.length * axis_tags.length * 6;
    اگر (unlikely (total > 0xFFFFFFFFu)) بازگشت نادرست;

    HBUINT16 *size_field = c->allocate_size<HBUINT16> (HBUINT16::static_size);
    اگر (unlikely (!size_field)) بازگشت نادرست;
    /* OFF 2026 5.3.3.11.2 permits larger stores with length=65535. */
    *size_field = hb_min (total, (uint64_t) 0xFFFFu);

    نویسه *header = c->allocate_size<نویسه> (header_size);
    اگر (unlikely (!header)) بازگشت نادرست;
    * (HBUINT16 *) (header + 0) = 1;			/* format */
    * (HBUINT32 *) (header + 2) = regions_offset;
    * (HBUINT16 *) (header + 6) = data_شمار;
    برای (unsigned i = 0; i < data_شمار; i++)
      * (HBUINT32 *) (header + 8 + 4 * i) = data_offsets.arrayZ[i];

    برای (ثابت vardata_transform_t &t : transforms)
    {
      اگر (!t.alive ()) ادامه;
      unsigned k = t.region_indices.length;
      HBUINT16 *v = (HBUINT16 *) c->allocate_size<نویسه> (6 + 2 * k);
      اگر (unlikely (!v)) بازگشت نادرست;
      v[0] = 0;						/* itemCount */
      v[1] = 0;						/* wordSizeCount */
      v[2] = k;						/* regionIndices.len */
      برای (unsigned j = 0; j < k; j++)
	v[3 + j] = t.region_indices.arrayZ[j];
    }

    hb_vector_t<ثابت hb_hashmap_t<hb_tag_t, Triple>*> region_ptrs;
    اگر (unlikely (!region_ptrs.alloc_exact (new_regions.length))) بازگشت نادرست;
    برای (ثابت auto &r : new_regions)
      region_ptrs.push (&r);
    auto *region_list = c->start_embed<OT::VarRegionList> ();
    بازگشت region_list->serialize (c, axis_tags, region_ptrs);
  }

  خصوصی:
  hb_vector_t<hb_hashmap_t<hb_tag_t, Triple>> orig_regions;
};

/* Usage-collection pass for partial instancing: record which VarDatas the
 * retained charstrings' and private dicts' blends reference.  Mirrors the
 * emission pass: blends consumed by dropped hint ops don't count. */
ساختار cff2_usage_param_t
{
  تهی init () {}

  cff2_instancing_plan_t *instancing = nullptr;
  منطقی drop_hints = نادرست;
  unsigned ivs = 0;	/* dict-side current vsindex */
};

ساختار cff2_cs_opset_usage_t : cff2_cs_opset_t<cff2_cs_opset_usage_t, cff2_usage_param_t, blend_arg_t>
{
  ایستا تهی flush_args_and_op (op_code_t op, cff2_cs_interp_env_t<blend_arg_t> &env, cff2_usage_param_t& param)
  {
    منطقی counts = درست;
    انتخاب (op)
    {
      حالت OpCode_return:
      حالت OpCode_endchar:
	counts = نادرست;
	بشکن;

      حالت OpCode_hstem:
      حالت OpCode_hstemhm:
      حالت OpCode_vstem:
      حالت OpCode_vstemhm:
      حالت OpCode_hintmask:
      حالت OpCode_cntrmask:
	اگر (param.drop_hints)
	  counts = نادرست;
	بشکن;

      پیشفرض:
	بشکن;
    }

    اگر (counts)
      برای (unsigned صحیح i = 0; i < env.arg_پشته.get_شمار (); i++)
	اگر (env.arg_پشته[i].blending ())
	{
	  param.instancing->note_use (env.get_ivs ());
	  بشکن;
	}

    env.clear_args ();
  }
};

ساختار cff2_private_dict_usage_opset_t : dict_opset_t
{
  ایستا تهی process_op (op_code_t op, cff2_priv_dict_interp_env_t& env, cff2_usage_param_t& param)
  {
    انتخاب (op) {
      حالت OpCode_vsindexdict:
	env.process_vsindex ();
	param.ivs = env.get_ivs ();
	env.clear_args ();
	بازگشت;
      حالت OpCode_blenddict:
	param.instancing->note_use (param.ivs);
	env.clear_args ();
	بازگشت;
      پیشفرض:
	dict_opset_t::process_op (op, env);
	اگر (!env.arg_پشته.is_empty ()) بازگشت;
	env.clear_args ();
	بازگشت;
    }
  }
};

} /* namespace CFF */

ساختار cff2_sub_table_info_t : cff_sub_table_info_t
{
  cff2_sub_table_info_t ()
    : cff_sub_table_info_t (),
      var_store_link (0)
  {}

  objidx_t  var_store_link;
};

ساختار cff2_بالاترین_dict_op_serializer_t : cff_بالاترین_dict_op_serializer_t<>
{
  منطقی serialize (hb_serialize_context_t *c,
		  ثابت op_str_t &opstr,
		  ثابت cff2_sub_table_info_t &info) ثابت
  {
    TRACE_SERIALIZE (این);

    انتخاب (opstr.op)
    {
      حالت OpCode_vstore:
        اگر (info.var_store_link)
	  return_trace (FontDict::serialize_link4_op(c, opstr.op, info.var_store_link));
	وگرنه
	  return_trace (درست);

      پیشفرض:
	return_trace (cff_بالاترین_dict_op_serializer_t<>::serialize (c, opstr, info));
    }
  }
};

ساختار cff2_cs_opset_flatten_t : cff2_cs_opset_t<cff2_cs_opset_flatten_t, flatten_param_t, blend_arg_t>
{
  ایستا تهی flush_args_and_op (op_code_t op, cff2_cs_interp_env_t<blend_arg_t> &env, flatten_param_t& param)
  {
    /* Optionally capture command for specialization (before flushing, to preserve args) */
    اگر (param.commands)
    {
      منطقی skip_command = نادرست;

      انتخاب (op)
      {
	حالت OpCode_return:
	حالت OpCode_endchar:
	  skip_command = درست;
	  بشکن;

	حالت OpCode_hstem:
	حالت OpCode_hstemhm:
	حالت OpCode_vstem:
	حالت OpCode_vstemhm:
	حالت OpCode_hintmask:
	حالت OpCode_cntrmask:
	  اگر (param.drop_hints)
	    skip_command = درست;
	  بشکن;

	پیشفرض:
	  بشکن;
      }

      اگر (!skip_command)
      {
	cs_command_t cmd (op);
	/* Capture resolved blend values */
	برای (unsigned صحیح i = 0; i < env.arg_پشته.get_شمار ();)
	{
	  ثابت blend_arg_t &arg = env.arg_پشته[i];
	  اگر (arg.blending ())
	  {
	    /* For blend args, capture only the resolved default value */
	    cmd.args.push (arg);
	    /* Skip over the multiple blend values */
	    i += arg.رقم_مقادیر_2;
	  }
	  وگرنه
	  {
	    cmd.args.push (arg);
	    i++;
	  }
	}
	param.commands->push (cmd);
      }
    }

    انتخاب (op)
    {
      حالت OpCode_return:
      حالت OpCode_endchar:
	/* dummy opcodes in CFF2. ignore */
	اگر (op == OpCode_endchar && param.instancer)
	  emit_vsindex_prefix (env, param);
	بشکن;

      حالت OpCode_hstem:
      حالت OpCode_hstemhm:
      حالت OpCode_vstem:
      حالت OpCode_vstemhm:
      حالت OpCode_hintmask:
      حالت OpCode_cntrmask:
	اگر (param.drop_hints)
	{
	  env.clear_args ();
	  بازگشت;
	}
	HB_FALLTHROUGH;

      پیشفرض:
	SUPER::flush_args_and_op (op, env, param);
	بشکن;
    }
  }

  ایستا تهی flush_args (cff2_cs_interp_env_t<blend_arg_t> &env, flatten_param_t& param)
  {
    برای (unsigned صحیح i = 0; i < env.arg_پشته.get_شمار ();)
    {
      ثابت blend_arg_t &arg = env.arg_پشته[i];
      اگر (arg.blending ())
      {
	اگر (unlikely (!((arg.رقم_مقادیر_2 > 0) && (env.arg_پشته.get_شمار () >= arg.رقم_مقادیر_2))))
	{
	  env.set_error ();
	  بازگشت;
	}
	اگر (param.instancer)
	  rewrite_blends (arg, i, env, param);
	وگرنه
	  flatten_blends (arg, i, env, param);
	i += arg.رقم_مقادیر_2;
      }
      وگرنه
      {
	str_encoder_t  encoder (param.flatStr);
	encoder.encode_رقم_cs (arg);
	i++;
      }
    }
    SUPER::flush_args (env, param);
  }

  ایستا تهی flatten_blends (ثابت blend_arg_t &arg, unsigned صحیح i, cff2_cs_interp_env_t<blend_arg_t> &env, flatten_param_t& param)
  {
    /* flatten the default values */
    str_encoder_t  encoder (param.flatStr);
    برای (unsigned صحیح j = 0; j < arg.رقم_مقادیر_2; j++)
    {
      ثابت blend_arg_t &arg1 = env.arg_پشته[i + j];
      اگر (unlikely (!((arg1.blending () && (arg.رقم_مقادیر_2 == arg1.رقم_مقادیر_2) && (arg1.مقدار_Index_2 == j) &&
	      (arg1.deltas.length == env.get_region_شمار ())))))
      {
	env.set_error ();
	بازگشت;
      }
      encoder.encode_رقم_cs (arg1);
    }
    /* flatten deltas for each value */
    برای (unsigned صحیح j = 0; j < arg.رقم_مقادیر_2; j++)
    {
      ثابت blend_arg_t &arg1 = env.arg_پشته[i + j];
      برای (unsigned صحیح k = 0; k < arg1.deltas.length; k++)
	encoder.encode_رقم_cs (arg1.deltas[k]);
    }
    /* flatten the number of values followed by blend operator */
    encoder.encode_int (arg.رقم_مقادیر_2);
    encoder.encode_op (OpCode_blendcs);
  }

  /* Partial instancing: rewrite one blend group against the instanced store.
   * Each value's default absorbs the folded (pinned-away) contribution, and
   * its deltas are mapped onto the new region list. */
  ایستا تهی rewrite_blends (ثابت blend_arg_t &arg, unsigned صحیح i, cff2_cs_interp_env_t<blend_arg_t> &env, flatten_param_t& param)
  {
    ثابت cff2_instancing_plan_t &instancer = *param.instancer;
    unsigned ivs = env.get_ivs ();
    ثابت cff2_instancing_plan_t::vardata_transform_t *t = instancer.get_transform (ivs);
    اگر (unlikely (!t))
    {
      env.set_error ();
      بازگشت;
    }

    unsigned n = arg.رقم_مقادیر_2;
    برای (unsigned صحیح j = 0; j < n; j++)
    {
      ثابت blend_arg_t &arg1 = env.arg_پشته[i + j];
      اگر (unlikely (!(arg1.blending () && (arg1.رقم_مقادیر_2 == n) && (arg1.مقدار_Index_2 == j) &&
		      (arg1.deltas.length == env.get_region_شمار ()) &&
		      (arg1.deltas.length == t->gains.length))))
      {
	env.set_error ();
	بازگشت;
      }
    }

    str_encoder_t encoder (param.flatStr);
    unsigned k_new = t->alive () ? t->matrix.length : 0;

    اگر (!k_new)
    {
      /* All of this VarData's regions died; blends dissolve into statics. */
      برای (unsigned صحیح j = 0; j < n; j++)
      {
	ثابت blend_arg_t &arg1 = env.arg_پشته[i + j];
	عدد_t v;
	v.set_real (arg1.to_real () + round (instancer.fold_deltas (ivs, arg1.deltas.as_array ())));
	encoder.encode_رقم_cs (v);
      }
      بازگشت;
    }

    /* Emit in chunks so the output charstring stays within the argument
     * stack limit: values already emitted for this op, plus this chunk's
     * defaults, deltas and count, must not exceed 513. */
    unsigned done = 0;
    وقتی (done < n)
    {
      unsigned avail = 512 > (i + done) ? 512 - (i + done) : 0;
      unsigned chunk = hb_min (avail / (k_new + 1), n - done);
      اگر (unlikely (!chunk))
      {
	env.set_error ();
	بازگشت;
      }

      برای (unsigned صحیح j = done; j < done + chunk; j++)
      {
	ثابت blend_arg_t &arg1 = env.arg_پشته[i + j];
	عدد_t v;
	v.set_real (arg1.to_real () + round (instancer.fold_deltas (ivs, arg1.deltas.as_array ())));
	encoder.encode_رقم_cs (v);
      }
      برای (unsigned صحیح j = done; j < done + chunk; j++)
      {
	ثابت blend_arg_t &arg1 = env.arg_پشته[i + j];
	برای (unsigned r = 0; r < k_new; r++)
	{
	  عدد_t v;
	  v.set_int (hb_clamp_to<صحیح> (round (t->transform_delta (r, arg1.deltas.as_array ()))));
	  encoder.encode_رقم_cs (v);
	}
      }
      encoder.encode_int (chunk);
      encoder.encode_op (OpCode_blendcs);
      done += chunk;
    }
    param.emitted_blend = درست;
  }

  /* Partial instancing: prepend a vsindex op when the charstring's
   * (remapped) ivs differs from its FD's (remapped) private-dict ivs. */
  ایستا تهی emit_vsindex_prefix (cff2_cs_interp_env_t<blend_arg_t> &env, flatten_param_t& param)
  {
    اگر (!param.emitted_blend) بازگشت;
    ثابت cff2_instancing_plan_t &instancer = *param.instancer;
    unsigned new_ivs = instancer.remap_ivs (env.get_ivs ());
    unsigned fd_ivs = instancer.remap_ivs (env.get_orig_ivs ());
    اگر (new_ivs == fd_ivs) بازگشت;

    str_buff_t prefix;
    str_encoder_t enc (prefix);
    enc.encode_int ((صحیح) new_ivs);
    enc.encode_op (OpCode_vsindexcs);

    str_buff_t &buf = param.flatStr;
    unsigned old_len = buf.length;
    اگر (unlikely (prefix.in_error () || !buf.resize (old_len + prefix.length)))
    {
      env.set_error ();
      بازگشت;
    }
    memmove (buf.arrayZ + prefix.length, buf.arrayZ, old_len);
    hb_memcpy (buf.arrayZ, prefix.arrayZ, prefix.length);
  }

  ایستا تهی flush_op (op_code_t op, cff2_cs_interp_env_t<blend_arg_t> &env, flatten_param_t& param)
  {
    انتخاب (op)
    {
      حالت OpCode_return:
      حالت OpCode_endchar:
	بازگشت;
      پیشفرض:
	str_encoder_t  encoder (param.flatStr);
	encoder.encode_op (op);
    }
  }

  ایستا تهی flush_hintmask (op_code_t op, cff2_cs_interp_env_t<blend_arg_t> &env, flatten_param_t& param)
  {
    SUPER::flush_hintmask (op, env, param);
    /* Preserve hintmask payload in captured commands for specializer re-encoding. */
    اگر (param.commands && !param.drop_hints && param.commands->length > 0)
    {
      auto &cmd = param.commands->tail ();
      اگر (cmd.op == op)
      {
        cmd.mask_bytes.resize (env.hintmask_size);
        اگر (unlikely (cmd.mask_bytes.in_error ()))
        {
          env.set_error ();
          بازگشت;
        }
        برای (unsigned صحیح i = 0; i < env.hintmask_size; i++)
          cmd.mask_bytes[i] = env.str_ref[i];
      }
    }
    اگر (!param.drop_hints)
    {
      str_encoder_t  encoder (param.flatStr);
      برای (unsigned صحیح i = 0; i < env.hintmask_size; i++)
	encoder.encode_byte (env.str_ref[i]);
    }
  }

  خصوصی:
  تعریفنوع cff2_cs_opset_t<cff2_cs_opset_flatten_t, flatten_param_t, blend_arg_t> SUPER;
  تعریفنوع cs_opset_t<blend_arg_t, cff2_cs_opset_flatten_t, cff2_cs_opset_flatten_t, cff2_cs_interp_env_t<blend_arg_t>, flatten_param_t> CSOPSET;
};

ساختار cff2_cs_opset_subr_subset_t : cff2_cs_opset_t<cff2_cs_opset_subr_subset_t, subr_subset_param_t, blend_arg_t>
{
  ایستا تهی process_op (op_code_t op, cff2_cs_interp_env_t<blend_arg_t> &env, subr_subset_param_t& param)
  {
    انتخاب (op) {

      حالت OpCode_return:
	param.current_parsed_str->flush_coalesced (env.str_ref);
	param.current_parsed_str->set_parsed ();
	env.return_from_subr ();
	param.set_current_str (env, نادرست);
	بشکن;

      حالت OpCode_endchar:
	param.current_parsed_str->flush_coalesced (env.str_ref);
	param.current_parsed_str->set_parsed ();
	SUPER::process_op (op, env, param);
	بشکن;

      حالت OpCode_callsubr:
	process_call_subr (op, CSType_LocalSubr, env, param, env.localSubrs, param.local_closure);
	بشکن;

      حالت OpCode_callgsubr:
	process_call_subr (op, CSType_GlobalSubr, env, param, env.globalSubrs, param.global_closure);
	بشکن;

      پیشفرض:
	SUPER::process_op (op, env, param);
	param.current_parsed_str->add_op (op, env.str_ref);
	بشکن;
    }
  }

  محافظتشده:
  ایستا تهی process_call_subr (op_code_t op, cs_type_t type,
				 cff2_cs_interp_env_t<blend_arg_t> &env, subr_subset_param_t& param,
				 cff2_biased_subrs_t& subrs, hb_set_t *closure)
  {
    byte_str_ref_t    str_ref = env.str_ref;
    env.call_subr (subrs, type);
    param.current_parsed_str->add_call_op (op, str_ref, env.context.subr_رقم_2);
    closure->add (env.context.subr_رقم_2);
    param.set_current_str (env, درست);
  }

  خصوصی:
  تعریفنوع cff2_cs_opset_t<cff2_cs_opset_subr_subset_t, subr_subset_param_t, blend_arg_t> SUPER;
};

ساختار cff2_subr_subsetter_t : subr_subsetter_t<cff2_subr_subsetter_t, CFF2Subrs, ثابت OT::cff2::accelerator_subset_t, cff2_cs_interp_env_t<blend_arg_t>, cff2_cs_opset_subr_subset_t>
{
  cff2_subr_subsetter_t (ثابت OT::cff2::accelerator_subset_t &acc_, ثابت hb_subset_plan_t *plan_)
    : subr_subsetter_t (acc_, plan_) {}

  ایستا تهی complete_parsed_str (cff2_cs_interp_env_t<blend_arg_t> &env, subr_subset_param_t& param, parsed_cs_str_t &charstring)
  {
    /* vsindex is inserted at the beginning of the charstring as necessary */
    اگر (env.seen_vsindex ())
    {
      عدد_t  ivs;
      ivs.set_int ((صحیح)env.get_ivs ());
      charstring.set_prefix (ivs, OpCode_vsindexcs);
    }
  }
};

ساختار cff2_private_blend_encoder_param_t
{
  cff2_private_blend_encoder_param_t (hb_serialize_context_t *c,
				      ثابت CFF2ItemVariationStore *varStore,
				      hb_array_t<صحیح> normalized_coords) :
    c (c), varStore (varStore), normalized_coords (normalized_coords) {}

  تهی init () {}

  تهی process_blend ()
  {
    اگر (!seen_blend)
    {
      region_شمار_2 = varStore->varStore.get_region_index_شمار (ivs);
      اگر (unlikely (!scalars.resize_exact (region_شمار_2)))
      {
	اگر (c) c->err (HB_SERIALIZE_ERROR_OTHER);
	seen_blend = درست;
	بازگشت;
      }
      varStore->varStore.get_region_scalars (ivs, normalized_coords.arrayZ, normalized_coords.length,
					     &scalars[0], region_شمار_2);
      seen_blend = درست;
    }
  }

  دوگانه blend_deltas (hb_array_t<ثابت عدد_t> deltas) ثابت
  {
    دوگانه v = 0;
    اگر (likely (scalars.length == deltas.length))
    {
      unsigned شمار = scalars.length;
      برای (unsigned i = 0; i < شمار; i++)
	v += (دوگانه) scalars.arrayZ[i] * deltas.arrayZ[i].to_real ();
    }
    بازگشت v;
  }


  hb_serialize_context_t *c = nullptr;
  منطقی seen_blend = نادرست;
  unsigned ivs = 0;
  unsigned region_شمار_2 = 0;
  hb_vector_t<اعشاری> scalars;
  ثابت	 CFF2ItemVariationStore *varStore = nullptr;
  hb_array_t<صحیح> normalized_coords;

  /* CFF2 partial instancing: when set, blends are rewritten against the
   * instanced variation store instead of being flattened. */
  ثابت cff2_instancing_plan_t *instancer = nullptr;
  /* Error-compensated fold rounding state, per dict operator. */
  دوگانه fold_exact = 0.;
  دوگانه fold_emitted = 0.;
};

ساختار cff2_private_dict_blend_opset_t : dict_opset_t
{
  ایستا تهی process_arg_blend (cff2_private_blend_encoder_param_t& param,
				 عدد_t &arg,
				 ثابت hb_array_t<ثابت عدد_t> blends,
				 unsigned n, unsigned i)
  {
    arg.set_int (round (arg.to_real () + param.blend_deltas (blends)));
  }

  /* The fold added to a value, error-compensated across the operator: most
   * blended private-dict values (BlueValues, StemSnap*, ...) are
   * delta-encoded, so independently rounded folds would accumulate in the
   * decoded absolutes; keeping the emitted cumulative fold equal to the
   * rounded exact cumulative fold bounds every absolute within ±0.5. */
  ایستا دوگانه fold (cff2_private_blend_encoder_param_t& param,
		      ثابت hb_array_t<ثابت عدد_t> blends)
  {
    دوگانه f = param.blend_deltas (blends);
    دوگانه emit = round (param.fold_exact + f) - param.fold_emitted;
    param.fold_exact += f;
    param.fold_emitted += emit;
    بازگشت emit;
  }

  /* Partial instancing: emit the args preceding this blend group, then the
   * group itself rewritten against the instanced store, and clear the
   * stack.  The op's remaining args are flushed at process_op time. */
  ایستا تهی rewrite_blends (cff2_priv_dict_interp_env_t& env,
			      cff2_private_blend_encoder_param_t& param,
			      unsigned start, unsigned n, unsigned k)
  {
    ثابت cff2_instancing_plan_t &instancer = *param.instancer;
    ثابت cff2_instancing_plan_t::vardata_transform_t *t = instancer.get_transform (param.ivs);
    اگر (unlikely (!t || t->gains.length != k))
    {
      env.set_error ();
      بازگشت;
    }

    str_buff_t str;
    str_encoder_t encoder (str);

    برای (unsigned idx = 0; idx < start; idx++)
      encoder.encode_رقم_tp (env.arg_پشته[idx]);

    unsigned k_new = t->alive () ? t->matrix.length : 0;
    اگر (!k_new)
    {
      /* This VarData died (or is unused by any retained charstring, which
       * can happen for FDs kept only for retain-gids holes); blends
       * dissolve into statics. */
      برای (unsigned j = 0; j < n; j++)
      {
	ثابت hb_array_t<ثابت عدد_t> blends = env.arg_پشته.sub_array (start + n + (j * k), k);
	عدد_t v;
	v.set_real (env.arg_پشته[start + j].to_real () + fold (param, blends));
	encoder.encode_رقم_tp (v);
      }
    }
    وگرنه
    {
      unsigned done = 0;
      وقتی (done < n)
      {
	unsigned avail = 512 > (start + done) ? 512 - (start + done) : 0;
	unsigned chunk = hb_min (avail / (k_new + 1), n - done);
	اگر (unlikely (!chunk))
	{
	  env.set_error ();
	  بازگشت;
	}

	برای (unsigned j = done; j < done + chunk; j++)
	{
	  ثابت hb_array_t<ثابت عدد_t> blends = env.arg_پشته.sub_array (start + n + (j * k), k);
	  عدد_t v;
	  v.set_real (env.arg_پشته[start + j].to_real () + fold (param, blends));
	  encoder.encode_رقم_tp (v);
	}
	برای (unsigned j = done; j < done + chunk; j++)
	{
	  ثابت hb_array_t<ثابت عدد_t> blends = env.arg_پشته.sub_array (start + n + (j * k), k);
	  برای (unsigned r = 0; r < k_new; r++)
	  {
	    عدد_t v;
	    v.set_int (hb_clamp_to<صحیح> (round (t->transform_delta (r, blends))));
	    encoder.encode_رقم_tp (v);
	  }
	}
	encoder.encode_int (chunk);
	encoder.encode_op (OpCode_blenddict);
	done += chunk;
      }
    }

    auto bytes = str.as_bytes ();
    param.c->embed (&bytes, bytes.length);

    env.clear_args ();
  }

  ایستا تهی process_blend (cff2_priv_dict_interp_env_t& env, cff2_private_blend_encoder_param_t& param)
  {
    unsigned صحیح n, k;

    param.process_blend ();
    k = param.region_شمار_2;
    n = env.arg_پشته.برداشت_uint ();

    unsigned شمار = env.arg_پشته.get_شمار ();
    unsigned صحیح total;
    اگر (unlikely (hb_unsigned_mul_overflows (k + 1, n, &total) || total > شمار))
    {
      env.set_error ();
      بازگشت;
    }

    /* copy the blend values into blend array of the default values */
    unsigned صحیح start = شمار - total;
    /* let an obvious error case fail, but note CFF2 spec doesn't forbid n==0 */
    اگر (unlikely (start > شمار))
    {
      env.set_error ();
      بازگشت;
    }

    اگر (param.instancer && k)
    {
      rewrite_blends (env, param, start, n, k);
      بازگشت;
    }

    برای (unsigned صحیح i = 0; i < n; i++)
    {
      ثابت hb_array_t<ثابت عدد_t> blends = env.arg_پشته.sub_array (start + n + (i * k), k);
      process_arg_blend (param, env.arg_پشته[start + i], blends, n, i);
    }

    /* pop off blend values leaving default values now adorned with blend values */
    env.arg_پشته.برداشت (k * n);
  }

  ایستا تهی process_op (op_code_t op, cff2_priv_dict_interp_env_t& env, cff2_private_blend_encoder_param_t& param)
  {
    انتخاب (op) {
      حالت OpCode_StdHW:
      حالت OpCode_StdVW:
      حالت OpCode_BlueScale:
      حالت OpCode_BlueShift:
      حالت OpCode_BlueFuzz:
      حالت OpCode_ExpansionFactor:
      حالت OpCode_LanguageGroup:
      حالت Op_Code_Blue_مقادیر:
      حالت OpCode_OtherBlues:
      حالت OpCode_FamilyBlues:
      حالت OpCode_FamilyOtherBlues:
      حالت OpCode_StemSnapH:
      حالت OpCode_StemSnapV:
	بشکن;
      حالت OpCode_vsindexdict:
	env.process_vsindex ();
	param.ivs = env.get_ivs ();
	اگر (param.instancer)
	{
	  /* Re-emit with the remapped vsindex; 0 is the dict default. */
	  unsigned new_ivs = param.instancer->remap_ivs (param.ivs);
	  اگر (new_ivs)
	  {
	    str_buff_t str;
	    str_encoder_t encoder (str);
	    encoder.encode_int ((صحیح) new_ivs);
	    encoder.encode_op (OpCode_vsindexdict);
	    auto bytes = str.as_bytes ();
	    param.c->embed (&bytes, bytes.length);
	  }
	}
	env.clear_args ();
	بازگشت;
      حالت OpCode_blenddict:
	process_blend (env, param);
	بازگشت;

      پیشفرض:
	dict_opset_t::process_op (op, env);
	اگر (!env.arg_پشته.is_empty ()) بازگشت;
	بشکن;
    }

    اگر (unlikely (env.in_error ())) بازگشت;

    // Write args then op

    str_buff_t str;
    str_encoder_t encoder (str);

    unsigned شمار = env.arg_پشته.get_شمار ();
    برای (unsigned i = 0; i < شمار; i++)
      encoder.encode_رقم_tp (env.arg_پشته[i]);

    encoder.encode_op (op);

    auto bytes = str.as_bytes ();
    param.c->embed (&bytes, bytes.length);

    env.clear_args ();
    param.fold_exact = param.fold_emitted = 0.;
  }
};

ساختار cff2_private_dict_op_serializer_t : op_serializer_t
{
  cff2_private_dict_op_serializer_t (منطقی desubroutinize_, منطقی drop_hints_, منطقی pinned_,
				     ثابت CFF::CFF2ItemVariationStore* varStore_,
				     hb_array_t<صحیح> normalized_coords_,
				     ثابت cff2_instancing_plan_t *instancer_ = nullptr)
    : desubroutinize (desubroutinize_), drop_hints (drop_hints_), pinned (pinned_),
      param (nullptr, varStore_, normalized_coords_)
  { param.instancer = instancer_; }

  منطقی serialize (hb_serialize_context_t *c,
		  ثابت op_str_t &opstr,
		  objidx_t subrs_link) ثابت
  {
    TRACE_SERIALIZE (این);

    اگر (drop_hints && dict_opset_t::is_hint_op (opstr.op))
      return_trace (درست);

    اگر (opstr.op == OpCode_Subrs)
    {
      اگر (desubroutinize || !subrs_link)
	return_trace (درست);
      وگرنه
	return_trace (FontDict::serialize_link2_op (c, opstr.op, subrs_link));
    }

    اگر (pinned || param.instancer)
    {
      /* Reinterpret opstr and process blends.  The param persists across
       * this dict's opstrs, so that an ivs set by a vsindexdict opstr is
       * still in effect when a later opstr's blends are processed. */
      cff2_priv_dict_interp_env_t env {hb_ubytes_t (opstr.ptr, opstr.length)};
      param.c = c;
      dict_interpreter_t<cff2_private_dict_blend_opset_t, cff2_private_blend_encoder_param_t, cff2_priv_dict_interp_env_t> interp (env);
      return_trace (interp.interpret (param));
    }

    return_trace (copy_opstr (c, opstr));
  }

  محافظتشده:
  ثابت منطقی desubroutinize;
  ثابت منطقی drop_hints;
  ثابت منطقی pinned;
  mutable cff2_private_blend_encoder_param_t param;
};



فضاینام OT {
ساختار cff2_subset_plan
{
  منطقی create (ثابت OT::cff2::accelerator_subset_t &acc,
	      hb_subset_plan_t *plan)
  {
    /* make sure notdef is first */
    hb_codepoint_t old_glyph;
    اگر (!plan->old_gid_for_new_gid (0, &old_glyph) || (old_glyph != 0)) بازگشت نادرست;

    رقم_glyphs = plan->رقم_output_glyphs ();
    orig_fdcount = acc.fdArray->شمار;

    drop_hints = plan->flags & HB_SUBSET_FLAGS_NO_HINTING;
    منطقی instancing_requested = (منطقی) plan->normalized_coords;
    pinned = instancing_requested && plan->all_axes_pinned;
    normalized_coords = plan->normalized_coords;
    head_maxp_info = plan->head_maxp_info;
    hmtx_map = &plan->hmtx_map;
    desubroutinize = plan->flags & HB_SUBSET_FLAGS_DESUBROUTINIZE ||
		     instancing_requested; // For instancing we need this path

    /* Partial instancing: some axes pinned or limited, others kept. */
    partial_instancing = instancing_requested && !pinned &&
			 acc.varStore != &Null (CFF2ItemVariationStore);
    اگر (partial_instancing &&
	unlikely (!instancing.create (acc.varStore, plan)))
      بازگشت نادرست;
    axis_tags = &plan->axis_tags;

    /* Enable command capture for CFF2→CFF1 conversion (for specialization) */
    capture_commands = pinned;

 #ifdef HB_EXPERIMENTAL_API
    min_charstrings_off_size = (plan->flags & HB_SUBSET_FLAGS_IFTB_REQUIREMENTS) ? 4 : 0;
 #وگرنه
    min_charstrings_off_size = 0;
 #endif

    اگر (desubroutinize)
    {
      اگر (partial_instancing)
      {
	/* Pass 1: collect which VarDatas the retained blends reference, so
	 * unused ones get pruned and the survivors renumbered. */
	hb_set_t used_fds;
	برای (unsigned صحیح i = 0; i < رقم_glyphs; i++)
	{
	  hb_codepoint_t glyph;
	  اگر (!plan->old_gid_for_new_gid (i, &glyph))
	    ادامه;
	  ثابت hb_ubytes_t str = (*acc.charStrings)[glyph];
	  unsigned صحیح fd = CFF::subset_get_fd (acc, glyph);
	  اگر (unlikely (fd >= acc.fd_شمار))
	    بازگشت نادرست;
	  used_fds.add (fd);

	  cff2_cs_interp_env_t<blend_arg_t> env (str, acc, fd);
	  cs_interpreter_t<cff2_cs_interp_env_t<blend_arg_t>, cff2_cs_opset_usage_t, cff2_usage_param_t> interp (env);
	  cff2_usage_param_t param;
	  param.instancing = &instancing;
	  param.drop_hints = drop_hints;
	  اگر (unlikely (!interp.interpret (param)))
	    بازگشت نادرست;
	}
	برای (unsigned fd : used_fds)
	{
	  cff2_usage_param_t param;
	  param.instancing = &instancing;
	  unsigned شمار = acc.privateDicts[fd].get_شمار ();
	  برای (unsigned j = 0; j < شمار; j++)
	  {
	    ثابت op_str_t &opstr = acc.privateDicts[fd][j];
	    اگر (drop_hints && dict_opset_t::is_hint_op (opstr.op))
	      ادامه;
	    cff2_priv_dict_interp_env_t env {hb_ubytes_t (opstr.ptr, opstr.length)};
	    dict_interpreter_t<cff2_private_dict_usage_opset_t, cff2_usage_param_t, cff2_priv_dict_interp_env_t> interp (env);
	    اگر (unlikely (!interp.interpret (param)))
	      بازگشت نادرست;
	  }
	}
	اگر (unlikely (!instancing.finalize ()))
	  بازگشت نادرست;

	/* Pass 2: like subr_flattener_t, but blends stay symbolic (no coords
	 * fed to the env) and get rewritten against the instanced store. */
	اگر (!subset_charstrings.resize_exact (رقم_glyphs))
	  بازگشت نادرست;
	برای (unsigned صحیح i = 0; i < رقم_glyphs; i++)
	{
	  hb_codepoint_t glyph;
	  اگر (!plan->old_gid_for_new_gid (i, &glyph))
	    ادامه;
	  ثابت hb_ubytes_t str = (*acc.charStrings)[glyph];
	  unsigned صحیح fd = CFF::subset_get_fd (acc, glyph);
	  اگر (unlikely (fd >= acc.fd_شمار))
	    بازگشت نادرست;

	  cff2_cs_interp_env_t<blend_arg_t> env (str, acc, fd);
	  cs_interpreter_t<cff2_cs_interp_env_t<blend_arg_t>, cff2_cs_opset_flatten_t, flatten_param_t> interp (env);
	  flatten_param_t param = {
	    subset_charstrings.arrayZ[i],
	    (منطقی) (plan->flags & HB_SUBSET_FLAGS_NO_HINTING),
	    plan
	  };
	  param.instancer = &instancing;
	  اگر (unlikely (!interp.interpret (param)))
	    بازگشت نادرست;
	}
      }
      وگرنه
      {
      /* Flatten global & local subrs */
      subr_flattener_t<ثابت OT::cff2::accelerator_subset_t, cff2_cs_interp_env_t<blend_arg_t>, cff2_cs_opset_flatten_t>
		    flattener(acc, plan);

      /* Enable command capture if requested (for specialization) */
      اگر (capture_commands)
      {
	اگر (!charstring_commands.resize_exact (رقم_glyphs))
	  بازگشت نادرست;

	اگر (!flattener.flatten (subset_charstrings, &charstring_commands))
	  بازگشت نادرست;
      }
      وگرنه
      {
	اگر (!flattener.flatten (subset_charstrings))
	  بازگشت نادرست;
      }
      }
    }
    وگرنه
    {
      cff2_subr_subsetter_t	subr_subsetter (acc, plan);

      /* Subset subrs: collect used subroutines, leaving all unused ones behind */
      اگر (!subr_subsetter.subset ())
	بازگشت نادرست;

      /* encode charstrings, global subrs, local subrs with new subroutine numbers */
      اگر (!subr_subsetter.encode_charstrings (subset_charstrings, !pinned))
	بازگشت نادرست;

      اگر (!subr_subsetter.encode_globalsubrs (subset_globalsubrs))
	بازگشت نادرست;

      /* local subrs */
      اگر (!subset_localsubrs.resize (orig_fdcount))
	بازگشت نادرست;
      برای (unsigned صحیح fd = 0; fd < orig_fdcount; fd++)
      {
	subset_localsubrs[fd].init ();
	اگر (!subr_subsetter.encode_localsubrs (fd, subset_localsubrs[fd]))
	  بازگشت نادرست;
      }
    }

    /* FDSelect */
    اگر (acc.fdSelect != &Null (CFF2FDSelect))
    {
      اگر (unlikely (!hb_plan_subset_cff_fdselect (plan,
						  orig_fdcount,
						  *(ثابت FDSelect *)acc.fdSelect,
						  acc.رقم_glyphs,
						  subset_fdcount,
						  subset_fdselect_size,
						  subset_fdselect_format,
						  subset_fdselect_ranges,
						  fdmap)))
	بازگشت نادرست;
    }
    وگرنه
      fdmap.identity (1);

    بازگشت درست;
  }

  cff2_sub_table_info_t info;

  unsigned صحیح    رقم_glyphs;
  unsigned صحیح    orig_fdcount = 0;
  unsigned صحیح    subset_fdcount = 1;
  unsigned صحیح    subset_fdselect_size = 0;
  unsigned صحیح    subset_fdselect_format = 0;
  منطقی            pinned = نادرست;
  hb_vector_t<code_pair_t>   subset_fdselect_ranges;

  hb_inc_bimap_t   fdmap;

  str_buff_vec_t	    subset_charstrings;
  str_buff_vec_t	    subset_globalsubrs;
  hb_vector_t<str_buff_vec_t> subset_localsubrs;

  منطقی	    drop_hints = نادرست;
  منطقی	    desubroutinize = نادرست;

  منطقی	    partial_instancing = نادرست;
  cff2_instancing_plan_t instancing;
  ثابت hb_vector_t<hb_tag_t> *axis_tags = nullptr;

  unsigned  min_charstrings_off_size = 0;

  hb_array_t<صحیح> normalized_coords; // For instantiation
  head_maxp_info_t head_maxp_info;  // For FontBBox
  ثابت hb_hashmap_t<hb_codepoint_t, hb_pair_t<unsigned, صحیح>> *hmtx_map; // For widths

  // Width optimization results (for CFF1 conversion)
  unsigned default_width = 0;
  unsigned nominal_width = 0;

  // Command capture for specialization (CFF2→CFF1 conversion)
  منطقی capture_commands = نادرست;
  hb_vector_t<hb_vector_t<cs_command_t>> charstring_commands;
};
} // namespace OT

/*
 * CFF2 to CFF1 Converter Implementation
 */

#شامل "hb-cff-width-optimizer.hh"
#شامل "hb-cff-specializer.hh"

/* Serialize charstrings using CFF1 format with widths */
ایستا منطقی
_serialize_cff1_charstrings (hb_serialize_context_t *c,
                             OT::cff2_subset_plan &plan,
                             unsigned default_width,
                             unsigned nominal_width)
{
  c->push ();

  // CFF1 requires:
  // 1. Width at the beginning (if != defaultWidthX)
  // 2. endchar at the end
  str_buff_vec_t cff1_charstrings;
  اگر (unlikely (!cff1_charstrings.resize (plan.subset_charstrings.length)))
  {
    c->برداشت_discard ();
    بازگشت نادرست;
  }

  برای (unsigned i = 0; i < plan.subset_charstrings.length; i++)
  {
    // Get width for this glyph from hmtx_map
    unsigned width = 0;
    اگر (plan.hmtx_map->has (i))
      width = plan.hmtx_map->get (i).first;

    // Encode width if different from default
    str_encoder_t encoder (cff1_charstrings[i]);
    اگر (width != default_width)
    {
      صحیح delta = (صحیح) width - (صحیح) nominal_width;
      encoder.encode_int (delta);
    }

    // Use specialized commands if available, otherwise use binary
    اگر (plan.capture_commands && i < plan.charstring_commands.length &&
        plan.charstring_commands[i].length > 0)
    {
      // Specialize and encode commands
      auto &commands = plan.charstring_commands[i];
      CFF::specialize_commands (commands, 48);  /* maxstack=48 for CFF1 */
      اگر (unlikely (!CFF::encode_commands (commands, cff1_charstrings[i])))
      {
        c->برداشت_discard ();
        بازگشت نادرست;
      }
    }
    وگرنه
    {
      // Use binary CharString
      ثابت str_buff_t &cs = plan.subset_charstrings[i];
      برای (unsigned j = 0; j < cs.length; j++)
        cff1_charstrings[i].push (cs[j]);
    }

    // Check if it already ends with endchar (0x0e) or return (0x0b)
    اگر (cff1_charstrings[i].length == 0 ||
        (cff1_charstrings[i].tail () != 0x0e && cff1_charstrings[i].tail () != 0x0b))
    {
      // Append endchar operator
      اگر (unlikely (!cff1_charstrings[i].افزودن_or_fail (0x0e)))
      {
        c->برداشت_discard ();
        بازگشت نادرست;
      }
    }
  }

  unsigned data_size = 0;
  unsigned total_size = CFF1CharStrings::total_size (cff1_charstrings, &data_size, plan.min_charstrings_off_size);
  اگر (unlikely (!c->start_zerocopy (total_size)))
  {
    c->برداشت_discard ();
    بازگشت نادرست;
  }

  auto *cs = c->start_embed<CFF1CharStrings> ();
  اگر (unlikely (!cs->serialize (c, cff1_charstrings, &data_size, plan.min_charstrings_off_size)))
  {
    c->برداشت_discard ();
    بازگشت نادرست;
  }

  plan.info.char_strings_link = c->برداشت_pack (نادرست);
  بازگشت درست;
}

/* Serialize CID Charset (format 2 range: gid 0-N -> cid 0-N) */
ایستا منطقی
_serialize_cff1_charset (hb_serialize_context_t *c,
                         unsigned صحیح رقم_glyphs,
                         objidx_t &charset_link)
{
  // For CID fonts, create a simple identity charset
  // Format 2: one range covering all glyphs (except .notdef)
  c->push ();

  auto *charset = c->start_embed<Charset> ();
  اگر (unlikely (!charset))
  {
    c->برداشت_discard ();
    بازگشت نادرست;
  }

  // Create a single range for CID 1 to num_glyphs-1
  hb_vector_t<code_pair_t> ranges;
  اگر (رقم_glyphs > 1)
  {
    code_pair_t range;
    range.code = 1;  // first CID
    range.glyph = رقم_glyphs - 2;  // nLeft (covers glyphs 1 to num_glyphs-1)
    ranges.push (range);
  }

  اگر (unlikely (!charset->serialize (c, 2, رقم_glyphs, ranges)))
  {
    c->برداشت_discard ();
    بازگشت نادرست;
  }

  charset_link = c->برداشت_pack ();
  بازگشت درست;
}

/* CFF2 to CFF1 serialization */
فضاینام CFF {

منطقی
serialize_cff2_to_cff1 (hb_serialize_context_t *c,
                        OT::cff2_subset_plan &plan,
                        ثابت cff2_بالاترین_dict_مقادیر_t &cff2_بالاترین_Dict,
                        ثابت OT::cff2::accelerator_subset_t &acc)
{
  TRACE_SERIALIZE (این);

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
    hb_vector_t<unsigned> widths;
    widths.alloc (plan.رقم_glyphs);

    برای (unsigned gid = 0; gid < plan.رقم_glyphs; gid++)
    {
      unsigned width = 0;
      اگر (plan.hmtx_map->has (gid))
        width = plan.hmtx_map->get (gid).first;
      widths.push (width);
    }

    // Optimize defaultWidthX and nominalWidthX
    CFF::optimize_widths (widths, plan.default_width, plan.nominal_width);
  }

  // 1. CharStrings (with widths prepended)
  اگر (!_serialize_cff1_charstrings (c, plan, plan.default_width, plan.nominal_width))
    return_trace (نادرست);

  // 2. Private DICs & Local Subrs (same as CFF2)
  hb_vector_t<table_info_t> private_dict_infos;
  اگر (unlikely (!private_dict_infos.resize (plan.subset_fdcount)))
    return_trace (نادرست);

  برای (صحیح i = (صحیح)acc.privateDicts.length; --i >= 0;)
  {
    اگر (plan.fdmap.has (i))
    {
      objidx_t subrs_link = 0;

      اگر (plan.subset_localsubrs[i].length > 0)
      {
        auto *dest = c->push<CFF1Subrs> ();
        اگر (likely (dest->serialize (c, plan.subset_localsubrs[i])))
          subrs_link = c->برداشت_pack (نادرست);
        وگرنه
        {
          c->برداشت_discard ();
          return_trace (نادرست);
        }
      }

      auto *pd = c->push<PrivateDict> ();
      // Use the CFF2 Private DICT serializer which instantiates blends when pinned=true
      cff2_private_dict_op_serializer_t privSzr (plan.desubroutinize, plan.drop_hints, plan.pinned,
                                                  acc.varStore, plan.normalized_coords);
      اگر (likely (pd->serialize (c, acc.privateDicts[i], privSzr, subrs_link)))
      {
        // Add defaultWidthX and nominalWidthX for CFF1
        str_buff_t width_ops;
        str_encoder_t encoder (width_ops);
        encoder.encode_int (plan.default_width);
        encoder.encode_op (OpCode_defaultWidthX);
        encoder.encode_int (plan.nominal_width);
        encoder.encode_op (OpCode_nominalWidthX);

        اگر (!encoder.in_error () && c->embed (width_ops.as_bytes ().arrayZ, width_ops.length))
        {
          unsigned fd = plan.fdmap[i];
          private_dict_infos[fd].size = c->length ();
          private_dict_infos[fd].link = c->برداشت_pack ();
        }
        وگرنه
        {
          c->برداشت_discard ();
          return_trace (نادرست);
        }
      }
      وگرنه
      {
        c->برداشت_discard ();
        return_trace (نادرست);
      }
    }
  }

  // 3. FDArray - serialize CFF2 font dicts as CFF1
  {
    auto *fda = c->push<FDArray<HBUINT16>> ();
    cff_font_dict_op_serializer_t fontSzr;
    auto it =
    + hb_zip (+ hb_iter (acc.fontDicts)
              | hb_filter ([&] (ثابت cff2_font_dict_مقادیر_t &_)
                { بازگشت plan.fdmap.has (&_ - &acc.fontDicts[0]); }),
              hb_iter (private_dict_infos))
    ;
    // Explicitly specify template parameters: DICTVAL, INFO
    منطقی success = fda->serialize<cff2_font_dict_مقادیر_t, table_info_t> (c, it, fontSzr);
    اگر (success)
      plan.info.fd_array_link = c->برداشت_pack (نادرست);
    وگرنه
    {
      c->برداشت_discard ();
      return_trace (نادرست);
    }
  }

  // 4. FDSelect (required in CFF1 CID-keyed fonts)
  // CFF1 requires FDSelect for all CID-keyed fonts, even with just one FD
  // CFF2 makes it optional when there's only one FD
  اگر (acc.fdSelect != &Null (CFF2FDSelect))
  {
    c->push ();
    اگر (likely (hb_serialize_cff_fdselect (c, plan.رقم_glyphs,
                                          *(ثابت FDSelect *)acc.fdSelect,
                                          plan.orig_fdcount,
                                          plan.subset_fdselect_format,
                                          plan.subset_fdselect_size,
                                          plan.subset_fdselect_ranges)))
      plan.info.fd_select.link = c->برداشت_pack ();
    وگرنه
    {
      c->برداشت_discard ();
      return_trace (نادرست);
    }
  }
  وگرنه
  {
    // Create a range-based FDSelect3 mapping all glyphs to FD 0
    // Format: format(1) + nRanges(2) + range(3) + sentinel(2) = 8 bytes
    c->push ();

    // Format byte
    HBUINT8 format;
    format = 3;
    اگر (unlikely (!c->embed (format)))
    {
      c->برداشت_discard ();
      return_trace (نادرست);
    }

    // nRanges
    HBUINT16 nRanges;
    nRanges = 1;
    اگر (unlikely (!c->embed (nRanges)))
    {
      c->برداشت_discard ();
      return_trace (نادرست);
    }

    // Single range: {first: 0, fd: 0}
    FDSelect3_Range range;
    range.first = 0;
    range.fd = 0;
    اگر (unlikely (!c->embed (range)))
    {
      c->برداشت_discard ();
      return_trace (نادرست);
    }

    // Sentinel (number of glyphs)
    HBUINT16 sentinel;
    sentinel = plan.رقم_glyphs;
    اگر (unlikely (!c->embed (sentinel)))
    {
      c->برداشت_discard ();
      return_trace (نادرست);
    }

    plan.info.fd_select.link = c->برداشت_pack ();
  }

  // 5. Charset (CID charset for identity mapping)
  objidx_t charset_link;
  اگر (!_serialize_cff1_charset (c, plan.رقم_glyphs, charset_link))
    return_trace (نادرست);

  // 6. Global Subrs
  {
    auto *dest = c->push<CFF1Subrs> ();
    اگر (likely (dest->serialize (c, plan.subset_globalsubrs)))
      c->برداشت_pack (نادرست);
    وگرنه
    {
      c->برداشت_discard ();
      return_trace (نادرست);
    }
  }

  // 7. String INDEX - Add "Adobe" and "Identity" for ROS operator
  {
    ثابت نویسه *adobe_str = "Adobe";
    ثابت نویسه *identity_str = "Identity";
    unsigned adobe_len = 5;  // strlen("Adobe")
    unsigned identity_len = 8;  // strlen("Identity")

    // Build strings array
    hb_vector_t<hb_ubytes_t> strings;
    strings.alloc (2);
    strings.push (hb_ubytes_t ((ثابت unsigned نویسه *) adobe_str, adobe_len));
    strings.push (hb_ubytes_t ((ثابت unsigned نویسه *) identity_str, identity_len));

    // Serialize as CFF INDEX
    auto *dest = c->push<CFF1Index> ();
    اگر (likely (dest->serialize (c, strings)))
      c->برداشت_pack (نادرست);
    وگرنه
    {
      c->برداشت_discard ();
      return_trace (نادرست);
    }
  }

  // 8. CFF Header
  OT::cff1 *cff = c->allocate_min<OT::cff1> ();
  اگر (unlikely (!cff)) return_trace (نادرست);

  /* header */
  cff->version.major = 0x01;
  cff->version.minor = 0x00;
  cff->nameIndex = cff->min_size;
  cff->offSize = 4; /* unused? */

  // 9. Name INDEX (single entry)
  {
    unsigned name_len = strlen (CFF1_DEFAULT_FONT_NAME);

    CFF1Index *idx = c->start_embed<CFF1Index> ();
    اگر (unlikely (!idx)) return_trace (نادرست);

    اگر (unlikely (!idx->serialize_header (c, hb_iter (&name_len, 1), name_len)))
      return_trace (نادرست);

    اگر (unlikely (!c->embed (CFF1_DEFAULT_FONT_NAME, name_len)))
      return_trace (نادرست);
  }

  // 10. Top DICT INDEX
  {
    // Serialize the Top DICT data first
    c->push<بالاترین_Dict> ();
    cff1_from_cff2_بالاترین_dict_op_serializer_t بالاترین_Szr;

    // Serialize ROS first
    اگر (unlikely (!بالاترین_Szr.serialize_ros (c)))
    {
      c->برداشت_discard ();
      return_trace (نادرست);
    }

    // Serialize FontBBox from head table
    {
      str_buff_t bbox_buff;
      str_encoder_t encoder (bbox_buff);

      encoder.encode_int (plan.head_maxp_info.xMin);
      encoder.encode_int (plan.head_maxp_info.yMin);
      encoder.encode_int (plan.head_maxp_info.xMax);
      encoder.encode_int (plan.head_maxp_info.yMax);
      encoder.encode_op (OpCode_FontBBox);

      اگر (encoder.in_error () || !c->embed (bbox_buff.as_bytes ().arrayZ, bbox_buff.length))
      {
        c->برداشت_discard ();
        return_trace (نادرست);
      }
    }

    // Serialize charset operator
    اگر (charset_link && unlikely (!FontDict::serialize_link4_op (c, OpCode_charset, charset_link, whence_t::Absolute)))
    {
      c->برداشت_discard ();
      return_trace (نادرست);
    }

    // Serialize FDSelect operator (required for CID-keyed CFF1 fonts)
    اگر (plan.info.fd_select.link && unlikely (!FontDict::serialize_link4_op (c, OpCode_FDSelect, plan.info.fd_select.link, whence_t::Absolute)))
    {
      c->برداشت_discard ();
      return_trace (نادرست);
    }

    // Serialize FDArray operator (required for CID-keyed CFF1 fonts)
    اگر (plan.info.fd_array_link && unlikely (!FontDict::serialize_link4_op (c, OpCode_FDArray, plan.info.fd_array_link, whence_t::Absolute)))
    {
      c->برداشت_discard ();
      return_trace (نادرست);
    }

    // Serialize other operators from CFF2 TopDict
    برای (ثابت auto &opstr : cff2_بالاترین_Dict.values)
    {
      اگر (unlikely (!بالاترین_Szr.serialize (c, opstr, plan.info)))
      {
        c->برداشت_discard ();
        return_trace (نادرست);
      }
    }

    unsigned بالاترین_size = c->length ();
    c->برداشت_pack (نادرست);

    // Serialize INDEX header
    auto *dest = c->start_embed<CFF1Index> ();
    اگر (unlikely (!dest->serialize_header (c, hb_iter (&بالاترین_size, 1), بالاترین_size)))
      return_trace (نادرست);
  }

  return_trace (درست);
}

} /* namespace CFF */

ایستا منطقی _serialize_cff2_charstrings (hb_serialize_context_t *c,
			     cff2_subset_plan &plan,
			     ثابت OT::cff2::accelerator_subset_t  &acc)
{
  c->push ();

  unsigned data_size = 0;
  unsigned total_size = CFF2CharStrings::total_size (plan.subset_charstrings, &data_size, plan.min_charstrings_off_size);
  اگر (unlikely (!c->start_zerocopy (total_size)))
    بازگشت نادرست;

  auto *cs = c->start_embed<CFF2CharStrings> ();
  اگر (unlikely (!cs->serialize (c, plan.subset_charstrings, &data_size, plan.min_charstrings_off_size)))
  {
    c->برداشت_discard ();
    بازگشت نادرست;
  }

  plan.info.char_strings_link = c->برداشت_pack (نادرست);
  بازگشت درست;
}

منطقی
OT::cff2::accelerator_subset_t::serialize (hb_serialize_context_t *c,
					   ساختار cff2_subset_plan &plan,
					   hb_array_t<صحیح> normalized_coords) ثابت
{
  /* push charstrings onto the object stack first which will ensure it packs as the last
     object in the table. Keeping the chastrings last satisfies the requirements for patching
     via IFTB. If this ordering needs to be changed in the future, charstrings should be left
     at the end whenever HB_SUBSET_FLAGS_ITFB_REQUIREMENTS is enabled. */
  اگر (!_serialize_cff2_charstrings(c, plan, *این))
    بازگشت نادرست;

  /* private dicts & local subrs */
  hb_vector_t<table_info_t>  private_dict_infos;
  اگر (unlikely (!private_dict_infos.resize (plan.subset_fdcount))) بازگشت نادرست;

  برای (صحیح i = (صحیح)privateDicts.length; --i >= 0 ;)
  {
    اگر (plan.fdmap.has (i))
    {
      objidx_t	subrs_link = 0;

      اگر (plan.subset_localsubrs[i].length > 0)
      {
	auto *dest = c->push <CFF2Subrs> ();
	اگر (likely (dest->serialize (c, plan.subset_localsubrs[i])))
	  subrs_link = c->برداشت_pack (نادرست);
	وگرنه
	{
	  c->برداشت_discard ();
	  بازگشت نادرست;
	}
      }
      auto *pd = c->push<PrivateDict> ();
      cff2_private_dict_op_serializer_t privSzr (plan.desubroutinize, plan.drop_hints, plan.pinned,
						 varStore, normalized_coords,
						 plan.partial_instancing ? &plan.instancing : nullptr);
      اگر (likely (pd->serialize (c, privateDicts[i], privSzr, subrs_link)))
      {
	unsigned fd = plan.fdmap[i];
	private_dict_infos[fd].size = c->length ();
	private_dict_infos[fd].link = c->برداشت_pack ();
      }
      وگرنه
      {
	c->برداشت_discard ();
	بازگشت نادرست;
      }
    }
  }

  /* FDSelect */
  اگر (fdSelect != &Null (CFF2FDSelect))
  {
    c->push ();
    اگر (likely (hb_serialize_cff_fdselect (c, plan.رقم_glyphs, *(ثابت FDSelect *)fdSelect,
					   plan.orig_fdcount,
					   plan.subset_fdselect_format, plan.subset_fdselect_size,
					   plan.subset_fdselect_ranges)))
      plan.info.fd_select.link = c->برداشت_pack ();
    وگرنه
    {
      c->برداشت_discard ();
      بازگشت نادرست;
    }
  }

  /* FDArray (FD Index) */
  {
    auto *fda = c->push<CFF2FDArray> ();
    cff_font_dict_op_serializer_t fontSzr;
    auto it =
    + hb_zip (+ hb_iter (fontDicts)
	      | hb_filter ([&] (ثابت cff2_font_dict_مقادیر_t &_)
		{ بازگشت plan.fdmap.has (&_ - &fontDicts[0]); }),
	      hb_iter (private_dict_infos))
    ;
    اگر (unlikely (!fda->serialize (c, it, fontSzr)))
    {
      c->برداشت_discard ();
      بازگشت نادرست;
    }
    plan.info.fd_array_link = c->برداشت_pack (نادرست);
  }

  /* variation store */
  اگر (varStore != &Null (CFF2ItemVariationStore) &&
      !plan.pinned)
  {
    اگر (plan.partial_instancing)
    {
      اگر (plan.instancing.has_variations ())
      {
	c->push ();
	اگر (unlikely (!plan.instancing.serialize_var_store (c, *plan.axis_tags)))
	{
	  c->برداشت_discard ();
	  بازگشت نادرست;
	}
	plan.info.var_store_link = c->برداشت_pack (نادرست);
      }
      /* else: the store emptied; the vstore op gets dropped along with it. */
    }
    وگرنه
    {
      auto *dest = c->push<CFF2ItemVariationStore> ();
      اگر (unlikely (!dest->serialize (c, varStore)))
      {
	c->برداشت_discard ();
	بازگشت نادرست;
      }
      plan.info.var_store_link = c->برداشت_pack (نادرست);
    }
  }

  OT::cff2 *cff2 = c->allocate_min<OT::cff2> ();
  اگر (unlikely (!cff2)) بازگشت نادرست;

  /* header */
  cff2->version.major = 0x02;
  cff2->version.minor = 0x00;
  cff2->بالاترین_Dict_2 = OT::cff2::static_size;

  /* top dict */
  {
    بالاترین_Dict &dict = cff2 + cff2->بالاترین_Dict_2;
    cff2_بالاترین_dict_op_serializer_t بالاترین_Szr;
    اگر (unlikely (!dict.serialize (c, بالاترین_Dict_2, بالاترین_Szr, plan.info))) بازگشت نادرست;
    cff2->بالاترین_Dict_Size = c->head - (ثابت نویسه *)&dict;
  }

  /* global subrs */
  {
    auto *dest = c->start_embed <CFF2Subrs> ();
    بازگشت dest->serialize (c, plan.subset_globalsubrs);
  }
}

منطقی
OT::cff2::accelerator_subset_t::subset (hb_subset_context_t *c) ثابت
{
  cff2_subset_plan cff2_plan;

  اگر (unlikely (!cff2_plan.create (*این, c->plan))) بازگشت نادرست;

  // If instantiating (pinned) and downgrade flag is set, convert to CFF1
  اگر (cff2_plan.pinned && (c->plan->flags & HB_SUBSET_FLAGS_DOWNGRADE_CFF2))
  {
    // Serialize CFF1 to the subsetter's serializer
    // If we run out of room, returning true will cause subsetter to retry with larger buffer
    منطقی result = CFF::serialize_cff2_to_cff1 (c->serializer, cff2_plan, بالاترین_Dict_2, *این);

    اگر (c->serializer->ran_out_of_room ())
      بازگشت درست; // Subsetter will retry with larger buffer

    اگر (result && !c->serializer->in_error ())
    {
      // Success - end serialization to resolve links
      c->serializer->end_serialize ();

      // Copy the serialized CFF1 data and add as CFF table
      hb_blob_t *cff_blob = c->serializer->copy_blob ();
      اگر (cff_blob)
      {
        c->plan->add_table (HB_TAG('C','F','F',' '), cff_blob);
        hb_blob_destroy (cff_blob);

        // Return false to signal CFF2 table is not needed
        بازگشت نادرست;
      }
    }

    // Conversion failed - don't fall back, fail hard for debugging
    بازگشت نادرست;
  }

  بازگشت serialize (c->serializer, cff2_plan,
		    c->plan->normalized_coords.as_array ());
}

#endif
