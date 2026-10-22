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

#ifndef HB_NO_SUBSET_CFF

#ضمن "hb-open-type.hh"
#ضمن "hb-ot-cff2-table.hh"
#ضمن "hb-ot-var-common.hh"
#ضمن "hb-set.h"
#ضمن "hb-subset-plan.hh"
#ضمن "hb-subset-cff-common.hh"
#ضمن "hb-cff2-interp-cs.hh"
#ضمن "hb-subset-cff2-to-cff1.hh"

using نطاق CFF;

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
    hb_vector_t<hb_vector_t<عائم>> matrix;
    /* Default-value fold-in scalars, one per old region: the old store's
     * region scalars at the new default location. */
    hb_vector_t<عائم> gains;
    /* Surviving regions, parallel to matrix rows. */
    hb_vector_t<hb_hashmap_t<hb_tag_t, Triple>> regions;
    /* Index into the new global region list, per new region (finalize). */
    hb_vector_t<unsigned> region_indices;
    unsigned new_major = 0;
    منطقي alive_ = خطأ;
    منطقي alive () ثابت { أرجع alive_; }

    /* One value's delta for new region j, from its old per-region deltas. */
    مزدوج transform_delta (unsigned j, hb_array_t<ثابت ژمار_t> deltas) ثابت
    {
      ثابت hb_vector_t<عائم> &col = matrix.arrayZ[j];
      مزدوج d = 0.;
      unsigned ژماردن = hb_min (col.length, deltas.length);
      لكل (unsigned c = 0; c < ژماردن; c++)
	d += (مزدوج) col.arrayZ[c] * deltas.arrayZ[c].to_real ();
      أرجع d;
    }
  };

  hb_vector_t<vardata_transform_t> transforms;  /* per old VarData (vsindex) */
  hb_vector_t<hb_hashmap_t<hb_tag_t, Triple>> new_regions;  /* global region list */
  hb_set_t used_majors;
  unsigned new_major_ژماردن = 0;

  منطقي has_variations () ثابت { أرجع new_major_ژماردن; }

  ثابت vardata_transform_t *get_transform (unsigned major) ثابت
  { أرجع major < transforms.length ? &transforms.arrayZ[major] : nullptr; }

  /* Record that a retained blend references this VarData. */
  فراغ note_use (unsigned major) { used_majors.add (major); }

  /* The new vsindex a private dict's (or charstring's) old ivs maps to;
   * dead VarDatas fall back to 0, their blends having become static. */
  unsigned remap_ivs (unsigned old_ivs) ثابت
  {
    ثابت vardata_transform_t *t = get_transform (old_ivs);
    أرجع (t && t->alive ()) ? t->new_major : 0;
  }

  مزدوج fold_deltas (unsigned old_ivs, hb_array_t<ثابت ژمار_t> deltas) ثابت
  {
    ثابت vardata_transform_t *t = get_transform (old_ivs);
    إذا (unlikely (!t || t->gains.length != deltas.length)) أرجع 0.;
    مزدوج v = 0.;
    لكل (unsigned i = 0; i < deltas.length; i++)
      v += (مزدوج) t->gains.arrayZ[i] * deltas.arrayZ[i].to_real ();
    أرجع v;
  }

  منطقي create (ثابت CFF2ItemVariationStore *cff2VarStore,
	       ثابت hb_subset_plan_t *plan)
  {
    ثابت OT::ItemVariationStore &varStore = cff2VarStore->varStore;
    unsigned major_ژماردن = varStore.get_sub_table_ژماردن ();
    إذا (!transforms.resize (major_ژماردن)) أرجع خطأ;

    إذا (!varStore.get_region_list ().get_var_regions (plan->axes_old_index_tag_map, orig_regions))
      أرجع خطأ;

    /* Instance each VarData with identity delta rows; the surviving tuples'
     * delta columns are the old-region → new-region transform. */
    OT::optimize_scratch_t scratch;
    لكل (unsigned m = 0; m < major_ژماردن; m++)
    {
      ثابت OT::VarData &var_data = varStore.get_sub_table (m);
      unsigned k = var_data.get_region_index_ژماردن ();
      vardata_transform_t &t = transforms.arrayZ[m];

      إذا (!t.gains.resize_exact (k)) أرجع خطأ;
      varStore.get_region_scalars (m, plan->normalized_coords.arrayZ,
				   plan->normalized_coords.length,
				   t.gains.arrayZ, k);

      OT::tuple_variations_t tuples;
      لكل (unsigned r = 0; r < k; r++)
      {
	OT::tuple_delta_t tuple;
	إذا (!tuple.deltas_x.resize (k) ||
	    !tuple.indices.resize (k))
	  أرجع خطأ;
	لكل (unsigned i = 0; i < k; i++)
	  tuple.indices.arrayZ[i] = true;
	tuple.deltas_x.arrayZ[r] = 1.f;
	unsigned region_index = var_data.get_region_index (r);
	إذا (unlikely (region_index >= orig_regions.length)) أرجع خطأ;
	tuple.axis_tuples = orig_regions.arrayZ[region_index];
	إذا (unlikely (tuple.axis_tuples.in_error ())) أرجع خطأ;
	tuples.tuple_vars.push (std::move (tuple));
      }
      إذا (unlikely (tuples.tuple_vars.in_error ())) أرجع خطأ;

      إذا (!tuples.instantiate (plan->axes_location, plan->axes_triple_distances, scratch))
	أرجع خطأ;

      لكل (auto &tuple : tuples.tuple_vars)
      {
	إذا (unlikely (tuple.deltas_x.length != k)) أرجع خطأ;
	hb_vector_t<عائم> col;
	إذا (unlikely (!col.resize_exact (k))) أرجع خطأ;
	لكل (unsigned i = 0; i < k; i++)
	  col.arrayZ[i] = tuple.deltas_x.arrayZ[i];
	t.matrix.push (std::move (col));
	t.regions.push (std::move (tuple.axis_tuples));
      }
      إذا (unlikely (t.matrix.in_error () || t.regions.in_error ())) أرجع خطأ;
    }

    أرجع true;
  }

  /* Decide which VarDatas survive, number them, and build the new global
   * region list: surviving original regions first, in their original order,
   * then solver-split new regions in order of appearance (fontTools'
   * rebuildRegions + prune_regions). */
  منطقي finalize ()
  {
    لكل (unsigned m = 0; m < transforms.length; m++)
    {
      vardata_transform_t &t = transforms.arrayZ[m];
      t.alive_ = t.matrix.length && used_majors.has (m);
      إذا (t.alive_)
	t.new_major = new_major_ژماردن++;
    }

    /* Keys hash by content, so pointers into different owners compare fine. */
    hb_hashmap_t<ثابت hb_hashmap_t<hb_tag_t, Triple>*, unsigned> region_idx_map;
    hb_vector_t<ثابت hb_hashmap_t<hb_tag_t, Triple>*> region_ptrs;
    لكل (ثابت vardata_transform_t &t : transforms)
    {
      إذا (!t.alive ()) استمر;
      لكل (ثابت auto &region : t.regions)
	إذا (!region_idx_map.has (&region))
	  إذا (unlikely (!region_idx_map.set (&region, (unsigned) -1)))
	    أرجع خطأ;
    }
    لكل (ثابت auto &r : orig_regions)
    {
      unsigned *idx;
      إذا (region_idx_map.has (&r, &idx) && *idx == (unsigned) -1)
      {
	إذا (unlikely (!region_idx_map.set (&r, region_ptrs.length))) أرجع خطأ;
	region_ptrs.push (&r);
      }
    }
    لكل (ثابت vardata_transform_t &t : transforms)
    {
      إذا (!t.alive ()) استمر;
      لكل (ثابت auto &region : t.regions)
      {
	unsigned *idx;
	إذا (region_idx_map.has (&region, &idx) && *idx == (unsigned) -1)
	{
	  إذا (unlikely (!region_idx_map.set (&region, region_ptrs.length))) أرجع خطأ;
	  region_ptrs.push (&region);
	}
      }
    }
    إذا (unlikely (region_ptrs.in_error ())) أرجع خطأ;

    لكل (vardata_transform_t &t : transforms)
    {
      إذا (!t.alive ()) استمر;
      لكل (ثابت auto &region : t.regions)
      {
	unsigned *idx;
	إذا (unlikely (!region_idx_map.has (&region, &idx))) أرجع خطأ;
	t.region_indices.push (*idx);
      }
      إذا (unlikely (t.region_indices.in_error ())) أرجع خطأ;
    }

    إذا (!new_regions.resize (region_ptrs.length)) أرجع خطأ;
    لكل (unsigned i = 0; i < region_ptrs.length; i++)
    {
      new_regions.arrayZ[i] = *region_ptrs.arrayZ[i];
      إذا (unlikely (new_regions.arrayZ[i].in_error ())) أرجع خطأ;
    }

    أرجع true;
  }

  /* Serialize the instanced store: a CFF2ItemVariationStore whose VarDatas
   * carry no items, just region indexes (blend deltas live in charstrings).
   * Laid out manually since all sizes are known up front. */
  منطقي serialize_var_store (hb_serialize_context_t *c,
			    ثابت hb_vector_t<hb_tag_t> &axis_tags) ثابت
  {
    unsigned data_ژماردن = new_major_ژماردن;
    unsigned header_size = 2 + 4 + 2 + 4 * data_ژماردن;

    uint64_t offset = header_size;
    hb_vector_t<unsigned> data_offsets;
    لكل (ثابت vardata_transform_t &t : transforms)
    {
      إذا (!t.alive ()) استمر;
      data_offsets.push ((unsigned) offset);
      offset += 6 + 2 * (uint64_t) t.region_indices.length;
    }
    إذا (unlikely (data_offsets.in_error () || data_offsets.length != data_ژماردن))
      أرجع خطأ;
    unsigned regions_offset = (unsigned) offset;
    uint64_t total = offset + 4 + (uint64_t) new_regions.length * axis_tags.length * 6;
    إذا (unlikely (total > 0xFFFFFFFFu)) أرجع خطأ;

    HBUINT16 *size_field = c->allocate_size<HBUINT16> (HBUINT16::static_size);
    إذا (unlikely (!size_field)) أرجع خطأ;
    /* OFF 2026 5.3.3.11.2 permits larger stores with length=65535. */
    *size_field = hb_min (total, (uint64_t) 0xFFFFu);

    حرف *header = c->allocate_size<حرف> (header_size);
    إذا (unlikely (!header)) أرجع خطأ;
    * (HBUINT16 *) (header + 0) = 1;			/* format */
    * (HBUINT32 *) (header + 2) = regions_offset;
    * (HBUINT16 *) (header + 6) = data_ژماردن;
    لكل (unsigned i = 0; i < data_ژماردن; i++)
      * (HBUINT32 *) (header + 8 + 4 * i) = data_offsets.arrayZ[i];

    لكل (ثابت vardata_transform_t &t : transforms)
    {
      إذا (!t.alive ()) استمر;
      unsigned k = t.region_indices.length;
      HBUINT16 *v = (HBUINT16 *) c->allocate_size<حرف> (6 + 2 * k);
      إذا (unlikely (!v)) أرجع خطأ;
      v[0] = 0;						/* itemCount */
      v[1] = 0;						/* wordSizeCount */
      v[2] = k;						/* regionIndices.len */
      لكل (unsigned j = 0; j < k; j++)
	v[3 + j] = t.region_indices.arrayZ[j];
    }

    hb_vector_t<ثابت hb_hashmap_t<hb_tag_t, Triple>*> region_ptrs;
    إذا (unlikely (!region_ptrs.alloc_exact (new_regions.length))) أرجع خطأ;
    لكل (ثابت auto &r : new_regions)
      region_ptrs.push (&r);
    auto *region_list = c->start_embed<OT::VarRegionList> ();
    أرجع region_list->serialize (c, axis_tags, region_ptrs);
  }

  خاص:
  hb_vector_t<hb_hashmap_t<hb_tag_t, Triple>> orig_regions;
};

/* Usage-collection pass for partial instancing: record which VarDatas the
 * retained charstrings' and private dicts' blends reference.  Mirrors the
 * emission pass: blends consumed by dropped hint ops don't count. */
بنية cff2_usage_param_t
{
  فراغ init () {}

  cff2_instancing_plan_t *instancing = nullptr;
  منطقي drop_hints = خطأ;
  unsigned ivs = 0;	/* dict-side current vsindex */
};

بنية cff2_cs_opset_usage_t : cff2_cs_opset_t<cff2_cs_opset_usage_t, cff2_usage_param_t, blend_arg_t>
{
  ساكن فراغ flush_args_and_op (op_code_t op, cff2_cs_interp_env_t<blend_arg_t> &env, cff2_usage_param_t& param)
  {
    منطقي counts = true;
    بدل (op)
    {
      حالة OpCode_return:
      حالة OpCode_endchar:
	counts = خطأ;
	اكسر;

      حالة OpCode_hstem:
      حالة OpCode_hstemhm:
      حالة OpCode_vstem:
      حالة OpCode_vstemhm:
      حالة OpCode_hintmask:
      حالة OpCode_cntrmask:
	إذا (param.drop_hints)
	  counts = خطأ;
	اكسر;

      افتراضي:
	اكسر;
    }

    إذا (counts)
      لكل (unsigned صحيح i = 0; i < env.arg_کەڵەکە.get_ژماردن (); i++)
	إذا (env.arg_کەڵەکە[i].blending ())
	{
	  param.instancing->note_use (env.get_ivs ());
	  اكسر;
	}

    env.clear_args ();
  }
};

بنية cff2_private_dict_usage_opset_t : dict_opset_t
{
  ساكن فراغ process_op (op_code_t op, cff2_priv_dict_interp_env_t& env, cff2_usage_param_t& param)
  {
    بدل (op) {
      حالة OpCode_vsindexdict:
	env.process_vsindex ();
	param.ivs = env.get_ivs ();
	env.clear_args ();
	أرجع;
      حالة OpCode_blenddict:
	param.instancing->note_use (param.ivs);
	env.clear_args ();
	أرجع;
      افتراضي:
	dict_opset_t::process_op (op, env);
	إذا (!env.arg_کەڵەکە.is_empty ()) أرجع;
	env.clear_args ();
	أرجع;
    }
  }
};

} /* namespace CFF */

بنية cff2_sub_table_info_t : cff_sub_table_info_t
{
  cff2_sub_table_info_t ()
    : cff_sub_table_info_t (),
      var_store_link (0)
  {}

  objidx_t  var_store_link;
};

بنية cff2_لووتکە_dict_op_serializer_t : cff_لووتکە_dict_op_serializer_t<>
{
  منطقي serialize (hb_serialize_context_t *c,
		  ثابت op_str_t &opstr,
		  ثابت cff2_sub_table_info_t &info) ثابت
  {
    TRACE_SERIALIZE (هذا);

    بدل (opstr.op)
    {
      حالة OpCode_vstore:
        إذا (info.var_store_link)
	  return_trace (FontDict::serialize_link4_op(c, opstr.op, info.var_store_link));
	وإلا
	  return_trace (true);

      افتراضي:
	return_trace (cff_لووتکە_dict_op_serializer_t<>::serialize (c, opstr, info));
    }
  }
};

بنية cff2_cs_opset_flatten_t : cff2_cs_opset_t<cff2_cs_opset_flatten_t, flatten_param_t, blend_arg_t>
{
  ساكن فراغ flush_args_and_op (op_code_t op, cff2_cs_interp_env_t<blend_arg_t> &env, flatten_param_t& param)
  {
    /* Optionally capture command for specialization (before flushing, to preserve args) */
    إذا (param.commands)
    {
      منطقي skip_command = خطأ;

      بدل (op)
      {
	حالة OpCode_return:
	حالة OpCode_endchar:
	  skip_command = true;
	  اكسر;

	حالة OpCode_hstem:
	حالة OpCode_hstemhm:
	حالة OpCode_vstem:
	حالة OpCode_vstemhm:
	حالة OpCode_hintmask:
	حالة OpCode_cntrmask:
	  إذا (param.drop_hints)
	    skip_command = true;
	  اكسر;

	افتراضي:
	  اكسر;
      }

      إذا (!skip_command)
      {
	cs_command_t cmd (op);
	/* Capture resolved blend values */
	لكل (unsigned صحيح i = 0; i < env.arg_کەڵەکە.get_ژماردن ();)
	{
	  ثابت blend_arg_t &arg = env.arg_کەڵەکە[i];
	  إذا (arg.blending ())
	  {
	    /* For blend args, capture only the resolved default value */
	    cmd.args.push (arg);
	    /* Skip over the multiple blend values */
	    i += arg.ژمارە_بەهاکان_2;
	  }
	  وإلا
	  {
	    cmd.args.push (arg);
	    i++;
	  }
	}
	param.commands->push (cmd);
      }
    }

    بدل (op)
    {
      حالة OpCode_return:
      حالة OpCode_endchar:
	/* dummy opcodes in CFF2. ignore */
	إذا (op == OpCode_endchar && param.instancer)
	  emit_vsindex_prefix (env, param);
	اكسر;

      حالة OpCode_hstem:
      حالة OpCode_hstemhm:
      حالة OpCode_vstem:
      حالة OpCode_vstemhm:
      حالة OpCode_hintmask:
      حالة OpCode_cntrmask:
	إذا (param.drop_hints)
	{
	  env.clear_args ();
	  أرجع;
	}
	HB_FALLTHROUGH;

      افتراضي:
	SUPER::flush_args_and_op (op, env, param);
	اكسر;
    }
  }

  ساكن فراغ flush_args (cff2_cs_interp_env_t<blend_arg_t> &env, flatten_param_t& param)
  {
    لكل (unsigned صحيح i = 0; i < env.arg_کەڵەکە.get_ژماردن ();)
    {
      ثابت blend_arg_t &arg = env.arg_کەڵەکە[i];
      إذا (arg.blending ())
      {
	إذا (unlikely (!((arg.ژمارە_بەهاکان_2 > 0) && (env.arg_کەڵەکە.get_ژماردن () >= arg.ژمارە_بەهاکان_2))))
	{
	  env.set_error ();
	  أرجع;
	}
	إذا (param.instancer)
	  rewrite_blends (arg, i, env, param);
	وإلا
	  flatten_blends (arg, i, env, param);
	i += arg.ژمارە_بەهاکان_2;
      }
      وإلا
      {
	str_encoder_t  encoder (param.flatStr);
	encoder.encode_ژمارە_cs (arg);
	i++;
      }
    }
    SUPER::flush_args (env, param);
  }

  ساكن فراغ flatten_blends (ثابت blend_arg_t &arg, unsigned صحيح i, cff2_cs_interp_env_t<blend_arg_t> &env, flatten_param_t& param)
  {
    /* flatten the default values */
    str_encoder_t  encoder (param.flatStr);
    لكل (unsigned صحيح j = 0; j < arg.ژمارە_بەهاکان_2; j++)
    {
      ثابت blend_arg_t &arg1 = env.arg_کەڵەکە[i + j];
      إذا (unlikely (!((arg1.blending () && (arg.ژمارە_بەهاکان_2 == arg1.ژمارە_بەهاکان_2) && (arg1.بەها_Index_2 == j) &&
	      (arg1.deltas.length == env.get_region_ژماردن ())))))
      {
	env.set_error ();
	أرجع;
      }
      encoder.encode_ژمارە_cs (arg1);
    }
    /* flatten deltas for each value */
    لكل (unsigned صحيح j = 0; j < arg.ژمارە_بەهاکان_2; j++)
    {
      ثابت blend_arg_t &arg1 = env.arg_کەڵەکە[i + j];
      لكل (unsigned صحيح k = 0; k < arg1.deltas.length; k++)
	encoder.encode_ژمارە_cs (arg1.deltas[k]);
    }
    /* flatten the number of values followed by blend operator */
    encoder.encode_int (arg.ژمارە_بەهاکان_2);
    encoder.encode_op (OpCode_blendcs);
  }

  /* Partial instancing: rewrite one blend group against the instanced store.
   * Each value's default absorbs the folded (pinned-away) contribution, and
   * its deltas are mapped onto the new region list. */
  ساكن فراغ rewrite_blends (ثابت blend_arg_t &arg, unsigned صحيح i, cff2_cs_interp_env_t<blend_arg_t> &env, flatten_param_t& param)
  {
    ثابت cff2_instancing_plan_t &instancer = *param.instancer;
    unsigned ivs = env.get_ivs ();
    ثابت cff2_instancing_plan_t::vardata_transform_t *t = instancer.get_transform (ivs);
    إذا (unlikely (!t))
    {
      env.set_error ();
      أرجع;
    }

    unsigned n = arg.ژمارە_بەهاکان_2;
    لكل (unsigned صحيح j = 0; j < n; j++)
    {
      ثابت blend_arg_t &arg1 = env.arg_کەڵەکە[i + j];
      إذا (unlikely (!(arg1.blending () && (arg1.ژمارە_بەهاکان_2 == n) && (arg1.بەها_Index_2 == j) &&
		      (arg1.deltas.length == env.get_region_ژماردن ()) &&
		      (arg1.deltas.length == t->gains.length))))
      {
	env.set_error ();
	أرجع;
      }
    }

    str_encoder_t encoder (param.flatStr);
    unsigned k_new = t->alive () ? t->matrix.length : 0;

    إذا (!k_new)
    {
      /* All of this VarData's regions died; blends dissolve into statics. */
      لكل (unsigned صحيح j = 0; j < n; j++)
      {
	ثابت blend_arg_t &arg1 = env.arg_کەڵەکە[i + j];
	ژمار_t v;
	v.set_real (arg1.to_real () + round (instancer.fold_deltas (ivs, arg1.deltas.as_array ())));
	encoder.encode_ژمارە_cs (v);
      }
      أرجع;
    }

    /* Emit in chunks so the output charstring stays within the argument
     * stack limit: values already emitted for this op, plus this chunk's
     * defaults, deltas and count, must not exceed 513. */
    unsigned done = 0;
    طالما (done < n)
    {
      unsigned avail = 512 > (i + done) ? 512 - (i + done) : 0;
      unsigned chunk = hb_min (avail / (k_new + 1), n - done);
      إذا (unlikely (!chunk))
      {
	env.set_error ();
	أرجع;
      }

      لكل (unsigned صحيح j = done; j < done + chunk; j++)
      {
	ثابت blend_arg_t &arg1 = env.arg_کەڵەکە[i + j];
	ژمار_t v;
	v.set_real (arg1.to_real () + round (instancer.fold_deltas (ivs, arg1.deltas.as_array ())));
	encoder.encode_ژمارە_cs (v);
      }
      لكل (unsigned صحيح j = done; j < done + chunk; j++)
      {
	ثابت blend_arg_t &arg1 = env.arg_کەڵەکە[i + j];
	لكل (unsigned r = 0; r < k_new; r++)
	{
	  ژمار_t v;
	  v.set_int (hb_clamp_to<صحيح> (round (t->transform_delta (r, arg1.deltas.as_array ()))));
	  encoder.encode_ژمارە_cs (v);
	}
      }
      encoder.encode_int (chunk);
      encoder.encode_op (OpCode_blendcs);
      done += chunk;
    }
    param.emitted_blend = true;
  }

  /* Partial instancing: prepend a vsindex op when the charstring's
   * (remapped) ivs differs from its FD's (remapped) private-dict ivs. */
  ساكن فراغ emit_vsindex_prefix (cff2_cs_interp_env_t<blend_arg_t> &env, flatten_param_t& param)
  {
    إذا (!param.emitted_blend) أرجع;
    ثابت cff2_instancing_plan_t &instancer = *param.instancer;
    unsigned new_ivs = instancer.remap_ivs (env.get_ivs ());
    unsigned fd_ivs = instancer.remap_ivs (env.get_orig_ivs ());
    إذا (new_ivs == fd_ivs) أرجع;

    str_buff_t prefix;
    str_encoder_t enc (prefix);
    enc.encode_int ((صحيح) new_ivs);
    enc.encode_op (OpCode_vsindexcs);

    str_buff_t &buf = param.flatStr;
    unsigned old_len = buf.length;
    إذا (unlikely (prefix.in_error () || !buf.resize (old_len + prefix.length)))
    {
      env.set_error ();
      أرجع;
    }
    memmove (buf.arrayZ + prefix.length, buf.arrayZ, old_len);
    hb_memcpy (buf.arrayZ, prefix.arrayZ, prefix.length);
  }

  ساكن فراغ flush_op (op_code_t op, cff2_cs_interp_env_t<blend_arg_t> &env, flatten_param_t& param)
  {
    بدل (op)
    {
      حالة OpCode_return:
      حالة OpCode_endchar:
	أرجع;
      افتراضي:
	str_encoder_t  encoder (param.flatStr);
	encoder.encode_op (op);
    }
  }

  ساكن فراغ flush_hintmask (op_code_t op, cff2_cs_interp_env_t<blend_arg_t> &env, flatten_param_t& param)
  {
    SUPER::flush_hintmask (op, env, param);
    /* Preserve hintmask payload in captured commands for specializer re-encoding. */
    إذا (param.commands && !param.drop_hints && param.commands->length > 0)
    {
      auto &cmd = param.commands->tail ();
      إذا (cmd.op == op)
      {
        cmd.mask_bytes.resize (env.hintmask_size);
        إذا (unlikely (cmd.mask_bytes.in_error ()))
        {
          env.set_error ();
          أرجع;
        }
        لكل (unsigned صحيح i = 0; i < env.hintmask_size; i++)
          cmd.mask_bytes[i] = env.str_ref[i];
      }
    }
    إذا (!param.drop_hints)
    {
      str_encoder_t  encoder (param.flatStr);
      لكل (unsigned صحيح i = 0; i < env.hintmask_size; i++)
	encoder.encode_byte (env.str_ref[i]);
    }
  }

  خاص:
  تعريفنوع cff2_cs_opset_t<cff2_cs_opset_flatten_t, flatten_param_t, blend_arg_t> SUPER;
  تعريفنوع cs_opset_t<blend_arg_t, cff2_cs_opset_flatten_t, cff2_cs_opset_flatten_t, cff2_cs_interp_env_t<blend_arg_t>, flatten_param_t> CSOPSET;
};

بنية cff2_cs_opset_subr_subset_t : cff2_cs_opset_t<cff2_cs_opset_subr_subset_t, subr_subset_param_t, blend_arg_t>
{
  ساكن فراغ process_op (op_code_t op, cff2_cs_interp_env_t<blend_arg_t> &env, subr_subset_param_t& param)
  {
    بدل (op) {

      حالة OpCode_return:
	param.current_parsed_str->flush_coalesced (env.str_ref);
	param.current_parsed_str->set_parsed ();
	env.return_from_subr ();
	param.set_current_str (env, خطأ);
	اكسر;

      حالة OpCode_endchar:
	param.current_parsed_str->flush_coalesced (env.str_ref);
	param.current_parsed_str->set_parsed ();
	SUPER::process_op (op, env, param);
	اكسر;

      حالة OpCode_callsubr:
	process_call_subr (op, CSType_LocalSubr, env, param, env.localSubrs, param.local_closure);
	اكسر;

      حالة OpCode_callgsubr:
	process_call_subr (op, CSType_GlobalSubr, env, param, env.globalSubrs, param.global_closure);
	اكسر;

      افتراضي:
	SUPER::process_op (op, env, param);
	param.current_parsed_str->add_op (op, env.str_ref);
	اكسر;
    }
  }

  محمي:
  ساكن فراغ process_call_subr (op_code_t op, cs_type_t type,
				 cff2_cs_interp_env_t<blend_arg_t> &env, subr_subset_param_t& param,
				 cff2_biased_subrs_t& subrs, hb_set_t *closure)
  {
    byte_str_ref_t    str_ref = env.str_ref;
    env.call_subr (subrs, type);
    param.current_parsed_str->add_call_op (op, str_ref, env.context.subr_ژمارە_2);
    closure->add (env.context.subr_ژمارە_2);
    param.set_current_str (env, true);
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
    إذا (env.seen_vsindex ())
    {
      ژمار_t  ivs;
      ivs.set_int ((صحيح)env.get_ivs ());
      charstring.set_prefix (ivs, OpCode_vsindexcs);
    }
  }
};

بنية cff2_private_blend_encoder_param_t
{
  cff2_private_blend_encoder_param_t (hb_serialize_context_t *c,
				      ثابت CFF2ItemVariationStore *varStore,
				      hb_array_t<صحيح> normalized_coords) :
    c (c), varStore (varStore), normalized_coords (normalized_coords) {}

  فراغ init () {}

  فراغ process_blend ()
  {
    إذا (!seen_blend)
    {
      region_ژماردن_2 = varStore->varStore.get_region_index_ژماردن (ivs);
      إذا (unlikely (!scalars.resize_exact (region_ژماردن_2)))
      {
	إذا (c) c->err (HB_SERIALIZE_ERROR_OTHER);
	seen_blend = true;
	أرجع;
      }
      varStore->varStore.get_region_scalars (ivs, normalized_coords.arrayZ, normalized_coords.length,
					     &scalars[0], region_ژماردن_2);
      seen_blend = true;
    }
  }

  مزدوج blend_deltas (hb_array_t<ثابت ژمار_t> deltas) ثابت
  {
    مزدوج v = 0;
    إذا (likely (scalars.length == deltas.length))
    {
      unsigned ژماردن = scalars.length;
      لكل (unsigned i = 0; i < ژماردن; i++)
	v += (مزدوج) scalars.arrayZ[i] * deltas.arrayZ[i].to_real ();
    }
    أرجع v;
  }


  hb_serialize_context_t *c = nullptr;
  منطقي seen_blend = خطأ;
  unsigned ivs = 0;
  unsigned region_ژماردن_2 = 0;
  hb_vector_t<عائم> scalars;
  ثابت	 CFF2ItemVariationStore *varStore = nullptr;
  hb_array_t<صحيح> normalized_coords;

  /* CFF2 partial instancing: when set, blends are rewritten against the
   * instanced variation store instead of being flattened. */
  ثابت cff2_instancing_plan_t *instancer = nullptr;
  /* Error-compensated fold rounding state, per dict operator. */
  مزدوج fold_exact = 0.;
  مزدوج fold_emitted = 0.;
};

بنية cff2_private_dict_blend_opset_t : dict_opset_t
{
  ساكن فراغ process_arg_blend (cff2_private_blend_encoder_param_t& param,
				 ژمار_t &arg,
				 ثابت hb_array_t<ثابت ژمار_t> blends,
				 unsigned n, unsigned i)
  {
    arg.set_int (round (arg.to_real () + param.blend_deltas (blends)));
  }

  /* The fold added to a value, error-compensated across the operator: most
   * blended private-dict values (BlueValues, StemSnap*, ...) are
   * delta-encoded, so independently rounded folds would accumulate in the
   * decoded absolutes; keeping the emitted cumulative fold equal to the
   * rounded exact cumulative fold bounds every absolute within ±0.5. */
  ساكن مزدوج fold (cff2_private_blend_encoder_param_t& param,
		      ثابت hb_array_t<ثابت ژمار_t> blends)
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
			      cff2_private_blend_encoder_param_t& param,
			      unsigned start, unsigned n, unsigned k)
  {
    ثابت cff2_instancing_plan_t &instancer = *param.instancer;
    ثابت cff2_instancing_plan_t::vardata_transform_t *t = instancer.get_transform (param.ivs);
    إذا (unlikely (!t || t->gains.length != k))
    {
      env.set_error ();
      أرجع;
    }

    str_buff_t str;
    str_encoder_t encoder (str);

    لكل (unsigned idx = 0; idx < start; idx++)
      encoder.encode_ژمارە_tp (env.arg_کەڵەکە[idx]);

    unsigned k_new = t->alive () ? t->matrix.length : 0;
    إذا (!k_new)
    {
      /* This VarData died (or is unused by any retained charstring, which
       * can happen for FDs kept only for retain-gids holes); blends
       * dissolve into statics. */
      لكل (unsigned j = 0; j < n; j++)
      {
	ثابت hb_array_t<ثابت ژمار_t> blends = env.arg_کەڵەکە.sub_array (start + n + (j * k), k);
	ژمار_t v;
	v.set_real (env.arg_کەڵەکە[start + j].to_real () + fold (param, blends));
	encoder.encode_ژمارە_tp (v);
      }
    }
    وإلا
    {
      unsigned done = 0;
      طالما (done < n)
      {
	unsigned avail = 512 > (start + done) ? 512 - (start + done) : 0;
	unsigned chunk = hb_min (avail / (k_new + 1), n - done);
	إذا (unlikely (!chunk))
	{
	  env.set_error ();
	  أرجع;
	}

	لكل (unsigned j = done; j < done + chunk; j++)
	{
	  ثابت hb_array_t<ثابت ژمار_t> blends = env.arg_کەڵەکە.sub_array (start + n + (j * k), k);
	  ژمار_t v;
	  v.set_real (env.arg_کەڵەکە[start + j].to_real () + fold (param, blends));
	  encoder.encode_ژمارە_tp (v);
	}
	لكل (unsigned j = done; j < done + chunk; j++)
	{
	  ثابت hb_array_t<ثابت ژمار_t> blends = env.arg_کەڵەکە.sub_array (start + n + (j * k), k);
	  لكل (unsigned r = 0; r < k_new; r++)
	  {
	    ژمار_t v;
	    v.set_int (hb_clamp_to<صحيح> (round (t->transform_delta (r, blends))));
	    encoder.encode_ژمارە_tp (v);
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

  ساكن فراغ process_blend (cff2_priv_dict_interp_env_t& env, cff2_private_blend_encoder_param_t& param)
  {
    unsigned صحيح n, k;

    param.process_blend ();
    k = param.region_ژماردن_2;
    n = env.arg_کەڵەکە.دەرهێنان_uint ();

    unsigned ژماردن = env.arg_کەڵەکە.get_ژماردن ();
    unsigned صحيح total;
    إذا (unlikely (hb_unsigned_mul_overflows (k + 1, n, &total) || total > ژماردن))
    {
      env.set_error ();
      أرجع;
    }

    /* copy the blend values into blend array of the default values */
    unsigned صحيح start = ژماردن - total;
    /* let an obvious error case fail, but note CFF2 spec doesn't forbid n==0 */
    إذا (unlikely (start > ژماردن))
    {
      env.set_error ();
      أرجع;
    }

    إذا (param.instancer && k)
    {
      rewrite_blends (env, param, start, n, k);
      أرجع;
    }

    لكل (unsigned صحيح i = 0; i < n; i++)
    {
      ثابت hb_array_t<ثابت ژمار_t> blends = env.arg_کەڵەکە.sub_array (start + n + (i * k), k);
      process_arg_blend (param, env.arg_کەڵەکە[start + i], blends, n, i);
    }

    /* pop off blend values leaving default values now adorned with blend values */
    env.arg_کەڵەکە.دەرهێنان (k * n);
  }

  ساكن فراغ process_op (op_code_t op, cff2_priv_dict_interp_env_t& env, cff2_private_blend_encoder_param_t& param)
  {
    بدل (op) {
      حالة OpCode_StdHW:
      حالة OpCode_StdVW:
      حالة OpCode_BlueScale:
      حالة OpCode_BlueShift:
      حالة OpCode_BlueFuzz:
      حالة OpCode_ExpansionFactor:
      حالة OpCode_LanguageGroup:
      حالة Op_Code_Blue_بەهاکان:
      حالة OpCode_OtherBlues:
      حالة OpCode_FamilyBlues:
      حالة OpCode_FamilyOtherBlues:
      حالة OpCode_StemSnapH:
      حالة OpCode_StemSnapV:
	اكسر;
      حالة OpCode_vsindexdict:
	env.process_vsindex ();
	param.ivs = env.get_ivs ();
	إذا (param.instancer)
	{
	  /* Re-emit with the remapped vsindex; 0 is the dict default. */
	  unsigned new_ivs = param.instancer->remap_ivs (param.ivs);
	  إذا (new_ivs)
	  {
	    str_buff_t str;
	    str_encoder_t encoder (str);
	    encoder.encode_int ((صحيح) new_ivs);
	    encoder.encode_op (OpCode_vsindexdict);
	    auto bytes = str.as_bytes ();
	    param.c->embed (&bytes, bytes.length);
	  }
	}
	env.clear_args ();
	أرجع;
      حالة OpCode_blenddict:
	process_blend (env, param);
	أرجع;

      افتراضي:
	dict_opset_t::process_op (op, env);
	إذا (!env.arg_کەڵەکە.is_empty ()) أرجع;
	اكسر;
    }

    إذا (unlikely (env.in_error ())) أرجع;

    // Write args then op

    str_buff_t str;
    str_encoder_t encoder (str);

    unsigned ژماردن = env.arg_کەڵەکە.get_ژماردن ();
    لكل (unsigned i = 0; i < ژماردن; i++)
      encoder.encode_ژمارە_tp (env.arg_کەڵەکە[i]);

    encoder.encode_op (op);

    auto bytes = str.as_bytes ();
    param.c->embed (&bytes, bytes.length);

    env.clear_args ();
    param.fold_exact = param.fold_emitted = 0.;
  }
};

بنية cff2_private_dict_op_serializer_t : op_serializer_t
{
  cff2_private_dict_op_serializer_t (منطقي desubroutinize_, منطقي drop_hints_, منطقي pinned_,
				     ثابت CFF::CFF2ItemVariationStore* varStore_,
				     hb_array_t<صحيح> normalized_coords_,
				     ثابت cff2_instancing_plan_t *instancer_ = nullptr)
    : desubroutinize (desubroutinize_), drop_hints (drop_hints_), pinned (pinned_),
      param (nullptr, varStore_, normalized_coords_)
  { param.instancer = instancer_; }

  منطقي serialize (hb_serialize_context_t *c,
		  ثابت op_str_t &opstr,
		  objidx_t subrs_link) ثابت
  {
    TRACE_SERIALIZE (هذا);

    إذا (drop_hints && dict_opset_t::is_hint_op (opstr.op))
      return_trace (true);

    إذا (opstr.op == OpCode_Subrs)
    {
      إذا (desubroutinize || !subrs_link)
	return_trace (true);
      وإلا
	return_trace (FontDict::serialize_link2_op (c, opstr.op, subrs_link));
    }

    إذا (pinned || param.instancer)
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

  محمي:
  ثابت منطقي desubroutinize;
  ثابت منطقي drop_hints;
  ثابت منطقي pinned;
  mutable cff2_private_blend_encoder_param_t param;
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

    ژمارە_glyphs = plan->ژمارە_output_glyphs ();
    orig_fdcount = acc.fdArray->ژماردن;

    drop_hints = plan->flags & HB_SUBSET_FLAGS_NO_HINTING;
    منطقي instancing_requested = (منطقي) plan->normalized_coords;
    pinned = instancing_requested && plan->all_axes_pinned;
    normalized_coords = plan->normalized_coords;
    head_maxp_info = plan->head_maxp_info;
    hmtx_map = &plan->hmtx_map;
    desubroutinize = plan->flags & HB_SUBSET_FLAGS_DESUBROUTINIZE ||
		     instancing_requested; // For instancing we need this path

    /* Partial instancing: some axes pinned or limited, others kept. */
    partial_instancing = instancing_requested && !pinned &&
			 acc.varStore != &Null (CFF2ItemVariationStore);
    إذا (partial_instancing &&
	unlikely (!instancing.create (acc.varStore, plan)))
      أرجع خطأ;
    axis_tags = &plan->axis_tags;

    /* Enable command capture for CFF2→CFF1 conversion (for specialization) */
    capture_commands = pinned;

 #ifdef HB_EXPERIMENTAL_API
    min_charstrings_off_size = (plan->flags & HB_SUBSET_FLAGS_IFTB_REQUIREMENTS) ? 4 : 0;
 #وإلا
    min_charstrings_off_size = 0;
 #endif

    إذا (desubroutinize)
    {
      إذا (partial_instancing)
      {
	/* Pass 1: collect which VarDatas the retained blends reference, so
	 * unused ones get pruned and the survivors renumbered. */
	hb_set_t used_fds;
	لكل (unsigned صحيح i = 0; i < ژمارە_glyphs; i++)
	{
	  hb_codepoint_t glyph;
	  إذا (!plan->old_gid_for_new_gid (i, &glyph))
	    استمر;
	  ثابت hb_ubytes_t str = (*acc.charStrings)[glyph];
	  unsigned صحيح fd = CFF::subset_get_fd (acc, glyph);
	  إذا (unlikely (fd >= acc.fd_ژماردن))
	    أرجع خطأ;
	  used_fds.add (fd);

	  cff2_cs_interp_env_t<blend_arg_t> env (str, acc, fd);
	  cs_interpreter_t<cff2_cs_interp_env_t<blend_arg_t>, cff2_cs_opset_usage_t, cff2_usage_param_t> interp (env);
	  cff2_usage_param_t param;
	  param.instancing = &instancing;
	  param.drop_hints = drop_hints;
	  إذا (unlikely (!interp.interpret (param)))
	    أرجع خطأ;
	}
	لكل (unsigned fd : used_fds)
	{
	  cff2_usage_param_t param;
	  param.instancing = &instancing;
	  unsigned ژماردن = acc.privateDicts[fd].get_ژماردن ();
	  لكل (unsigned j = 0; j < ژماردن; j++)
	  {
	    ثابت op_str_t &opstr = acc.privateDicts[fd][j];
	    إذا (drop_hints && dict_opset_t::is_hint_op (opstr.op))
	      استمر;
	    cff2_priv_dict_interp_env_t env {hb_ubytes_t (opstr.ptr, opstr.length)};
	    dict_interpreter_t<cff2_private_dict_usage_opset_t, cff2_usage_param_t, cff2_priv_dict_interp_env_t> interp (env);
	    إذا (unlikely (!interp.interpret (param)))
	      أرجع خطأ;
	  }
	}
	إذا (unlikely (!instancing.finalize ()))
	  أرجع خطأ;

	/* Pass 2: like subr_flattener_t, but blends stay symbolic (no coords
	 * fed to the env) and get rewritten against the instanced store. */
	إذا (!subset_charstrings.resize_exact (ژمارە_glyphs))
	  أرجع خطأ;
	لكل (unsigned صحيح i = 0; i < ژمارە_glyphs; i++)
	{
	  hb_codepoint_t glyph;
	  إذا (!plan->old_gid_for_new_gid (i, &glyph))
	    استمر;
	  ثابت hb_ubytes_t str = (*acc.charStrings)[glyph];
	  unsigned صحيح fd = CFF::subset_get_fd (acc, glyph);
	  إذا (unlikely (fd >= acc.fd_ژماردن))
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
	إذا (!charstring_commands.resize_exact (ژمارە_glyphs))
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
      لكل (unsigned صحيح fd = 0; fd < orig_fdcount; fd++)
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
						  acc.ژمارە_glyphs,
						  subset_fdcount,
						  subset_fdselect_size,
						  subset_fdselect_format,
						  subset_fdselect_ranges,
						  fdmap)))
	أرجع خطأ;
    }
    وإلا
      fdmap.identity (1);

    أرجع true;
  }

  cff2_sub_table_info_t info;

  unsigned صحيح    ژمارە_glyphs;
  unsigned صحيح    orig_fdcount = 0;
  unsigned صحيح    subset_fdcount = 1;
  unsigned صحيح    subset_fdselect_size = 0;
  unsigned صحيح    subset_fdselect_format = 0;
  منطقي            pinned = خطأ;
  hb_vector_t<code_pair_t>   subset_fdselect_ranges;

  hb_inc_bimap_t   fdmap;

  str_buff_vec_t	    subset_charstrings;
  str_buff_vec_t	    subset_globalsubrs;
  hb_vector_t<str_buff_vec_t> subset_localsubrs;

  منطقي	    drop_hints = خطأ;
  منطقي	    desubroutinize = خطأ;

  منطقي	    partial_instancing = خطأ;
  cff2_instancing_plan_t instancing;
  ثابت hb_vector_t<hb_tag_t> *axis_tags = nullptr;

  unsigned  min_charstrings_off_size = 0;

  hb_array_t<صحيح> normalized_coords; // For instantiation
  head_maxp_info_t head_maxp_info;  // For FontBBox
  ثابت hb_hashmap_t<hb_codepoint_t, hb_pair_t<unsigned, صحيح>> *hmtx_map; // For widths

  // Width optimization results (for CFF1 conversion)
  unsigned default_width = 0;
  unsigned nominal_width = 0;

  // Command capture for specialization (CFF2→CFF1 conversion)
  منطقي capture_commands = خطأ;
  hb_vector_t<hb_vector_t<cs_command_t>> charstring_commands;
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
                             unsigned default_width,
                             unsigned nominal_width)
{
  c->push ();

  // CFF1 requires:
  // 1. Width at the beginning (if != defaultWidthX)
  // 2. endchar at the end
  str_buff_vec_t cff1_charstrings;
  إذا (unlikely (!cff1_charstrings.resize (plan.subset_charstrings.length)))
  {
    c->دەرهێنان_discard ();
    أرجع خطأ;
  }

  لكل (unsigned i = 0; i < plan.subset_charstrings.length; i++)
  {
    // Get width for this glyph from hmtx_map
    unsigned width = 0;
    إذا (plan.hmtx_map->has (i))
      width = plan.hmtx_map->get (i).first;

    // Encode width if different from default
    str_encoder_t encoder (cff1_charstrings[i]);
    إذا (width != default_width)
    {
      صحيح delta = (صحيح) width - (صحيح) nominal_width;
      encoder.encode_int (delta);
    }

    // Use specialized commands if available, otherwise use binary
    إذا (plan.capture_commands && i < plan.charstring_commands.length &&
        plan.charstring_commands[i].length > 0)
    {
      // Specialize and encode commands
      auto &commands = plan.charstring_commands[i];
      CFF::specialize_commands (commands, 48);  /* maxstack=48 for CFF1 */
      إذا (unlikely (!CFF::encode_commands (commands, cff1_charstrings[i])))
      {
        c->دەرهێنان_discard ();
        أرجع خطأ;
      }
    }
    وإلا
    {
      // Use binary CharString
      ثابت str_buff_t &cs = plan.subset_charstrings[i];
      لكل (unsigned j = 0; j < cs.length; j++)
        cff1_charstrings[i].push (cs[j]);
    }

    // Check if it already ends with endchar (0x0e) or return (0x0b)
    إذا (cff1_charstrings[i].length == 0 ||
        (cff1_charstrings[i].tail () != 0x0e && cff1_charstrings[i].tail () != 0x0b))
    {
      // Append endchar operator
      إذا (unlikely (!cff1_charstrings[i].پاڵنان_or_fail (0x0e)))
      {
        c->دەرهێنان_discard ();
        أرجع خطأ;
      }
    }
  }

  unsigned data_size = 0;
  unsigned total_size = CFF1CharStrings::total_size (cff1_charstrings, &data_size, plan.min_charstrings_off_size);
  إذا (unlikely (!c->start_zerocopy (total_size)))
  {
    c->دەرهێنان_discard ();
    أرجع خطأ;
  }

  auto *cs = c->start_embed<CFF1CharStrings> ();
  إذا (unlikely (!cs->serialize (c, cff1_charstrings, &data_size, plan.min_charstrings_off_size)))
  {
    c->دەرهێنان_discard ();
    أرجع خطأ;
  }

  plan.info.char_strings_link = c->دەرهێنان_pack (خطأ);
  أرجع true;
}

/* Serialize CID Charset (format 2 range: gid 0-N -> cid 0-N) */
ساكن منطقي
_serialize_cff1_charset (hb_serialize_context_t *c,
                         unsigned صحيح ژمارە_glyphs,
                         objidx_t &charset_link)
{
  // For CID fonts, create a simple identity charset
  // Format 2: one range covering all glyphs (except .notdef)
  c->push ();

  auto *charset = c->start_embed<Charset> ();
  إذا (unlikely (!charset))
  {
    c->دەرهێنان_discard ();
    أرجع خطأ;
  }

  // Create a single range for CID 1 to num_glyphs-1
  hb_vector_t<code_pair_t> ranges;
  إذا (ژمارە_glyphs > 1)
  {
    code_pair_t range;
    range.code = 1;  // first CID
    range.glyph = ژمارە_glyphs - 2;  // nLeft (covers glyphs 1 to num_glyphs-1)
    ranges.push (range);
  }

  إذا (unlikely (!charset->serialize (c, 2, ژمارە_glyphs, ranges)))
  {
    c->دەرهێنان_discard ();
    أرجع خطأ;
  }

  charset_link = c->دەرهێنان_pack ();
  أرجع true;
}

/* CFF2 to CFF1 serialization */
نطاق CFF {

منطقي
serialize_cff2_to_cff1 (hb_serialize_context_t *c,
                        OT::cff2_subset_plan &plan,
                        ثابت cff2_لووتکە_dict_بەهاکان_t &cff2_لووتکە_Dict,
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
    hb_vector_t<unsigned> widths;
    widths.alloc (plan.ژمارە_glyphs);

    لكل (unsigned gid = 0; gid < plan.ژمارە_glyphs; gid++)
    {
      unsigned width = 0;
      إذا (plan.hmtx_map->has (gid))
        width = plan.hmtx_map->get (gid).first;
      widths.push (width);
    }

    // Optimize defaultWidthX and nominalWidthX
    CFF::optimize_widths (widths, plan.default_width, plan.nominal_width);
  }

  // 1. CharStrings (with widths prepended)
  إذا (!_serialize_cff1_charstrings (c, plan, plan.default_width, plan.nominal_width))
    return_trace (خطأ);

  // 2. Private DICs & Local Subrs (same as CFF2)
  hb_vector_t<table_info_t> private_dict_infos;
  إذا (unlikely (!private_dict_infos.resize (plan.subset_fdcount)))
    return_trace (خطأ);

  لكل (صحيح i = (صحيح)acc.privateDicts.length; --i >= 0;)
  {
    إذا (plan.fdmap.has (i))
    {
      objidx_t subrs_link = 0;

      إذا (plan.subset_localsubrs[i].length > 0)
      {
        auto *dest = c->push<CFF1Subrs> ();
        إذا (likely (dest->serialize (c, plan.subset_localsubrs[i])))
          subrs_link = c->دەرهێنان_pack (خطأ);
        وإلا
        {
          c->دەرهێنان_discard ();
          return_trace (خطأ);
        }
      }

      auto *pd = c->push<PrivateDict> ();
      // Use the CFF2 Private DICT serializer which instantiates blends when pinned=true
      cff2_private_dict_op_serializer_t privSzr (plan.desubroutinize, plan.drop_hints, plan.pinned,
                                                  acc.varStore, plan.normalized_coords);
      إذا (likely (pd->serialize (c, acc.privateDicts[i], privSzr, subrs_link)))
      {
        // Add defaultWidthX and nominalWidthX for CFF1
        str_buff_t width_ops;
        str_encoder_t encoder (width_ops);
        encoder.encode_int (plan.default_width);
        encoder.encode_op (OpCode_defaultWidthX);
        encoder.encode_int (plan.nominal_width);
        encoder.encode_op (OpCode_nominalWidthX);

        إذا (!encoder.in_error () && c->embed (width_ops.as_bytes ().arrayZ, width_ops.length))
        {
          unsigned fd = plan.fdmap[i];
          private_dict_infos[fd].size = c->length ();
          private_dict_infos[fd].link = c->دەرهێنان_pack ();
        }
        وإلا
        {
          c->دەرهێنان_discard ();
          return_trace (خطأ);
        }
      }
      وإلا
      {
        c->دەرهێنان_discard ();
        return_trace (خطأ);
      }
    }
  }

  // 3. FDArray - serialize CFF2 font dicts as CFF1
  {
    auto *fda = c->push<FDArray<HBUINT16>> ();
    cff_font_dict_op_serializer_t fontSzr;
    auto it =
    + hb_zip (+ hb_iter (acc.fontDicts)
              | hb_filter ([&] (ثابت cff2_font_dict_بەهاکان_t &_)
                { أرجع plan.fdmap.has (&_ - &acc.fontDicts[0]); }),
              hb_iter (private_dict_infos))
    ;
    // Explicitly specify template parameters: DICTVAL, INFO
    منطقي success = fda->serialize<cff2_font_dict_بەهاکان_t, table_info_t> (c, it, fontSzr);
    إذا (success)
      plan.info.fd_array_link = c->دەرهێنان_pack (خطأ);
    وإلا
    {
      c->دەرهێنان_discard ();
      return_trace (خطأ);
    }
  }

  // 4. FDSelect (required in CFF1 CID-keyed fonts)
  // CFF1 requires FDSelect for all CID-keyed fonts, even with just one FD
  // CFF2 makes it optional when there's only one FD
  إذا (acc.fdSelect != &Null (CFF2FDSelect))
  {
    c->push ();
    إذا (likely (hb_serialize_cff_fdselect (c, plan.ژمارە_glyphs,
                                          *(ثابت FDSelect *)acc.fdSelect,
                                          plan.orig_fdcount,
                                          plan.subset_fdselect_format,
                                          plan.subset_fdselect_size,
                                          plan.subset_fdselect_ranges)))
      plan.info.fd_select.link = c->دەرهێنان_pack ();
    وإلا
    {
      c->دەرهێنان_discard ();
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
      c->دەرهێنان_discard ();
      return_trace (خطأ);
    }

    // nRanges
    HBUINT16 nRanges;
    nRanges = 1;
    إذا (unlikely (!c->embed (nRanges)))
    {
      c->دەرهێنان_discard ();
      return_trace (خطأ);
    }

    // Single range: {first: 0, fd: 0}
    FDSelect3_Range range;
    range.first = 0;
    range.fd = 0;
    إذا (unlikely (!c->embed (range)))
    {
      c->دەرهێنان_discard ();
      return_trace (خطأ);
    }

    // Sentinel (number of glyphs)
    HBUINT16 sentinel;
    sentinel = plan.ژمارە_glyphs;
    إذا (unlikely (!c->embed (sentinel)))
    {
      c->دەرهێنان_discard ();
      return_trace (خطأ);
    }

    plan.info.fd_select.link = c->دەرهێنان_pack ();
  }

  // 5. Charset (CID charset for identity mapping)
  objidx_t charset_link;
  إذا (!_serialize_cff1_charset (c, plan.ژمارە_glyphs, charset_link))
    return_trace (خطأ);

  // 6. Global Subrs
  {
    auto *dest = c->push<CFF1Subrs> ();
    إذا (likely (dest->serialize (c, plan.subset_globalsubrs)))
      c->دەرهێنان_pack (خطأ);
    وإلا
    {
      c->دەرهێنان_discard ();
      return_trace (خطأ);
    }
  }

  // 7. String INDEX - Add "Adobe" and "Identity" for ROS operator
  {
    ثابت حرف *adobe_str = "Adobe";
    ثابت حرف *identity_str = "Identity";
    unsigned adobe_len = 5;  // strlen("Adobe")
    unsigned identity_len = 8;  // strlen("Identity")

    // Build strings array
    hb_vector_t<hb_ubytes_t> strings;
    strings.alloc (2);
    strings.push (hb_ubytes_t ((ثابت unsigned حرف *) adobe_str, adobe_len));
    strings.push (hb_ubytes_t ((ثابت unsigned حرف *) identity_str, identity_len));

    // Serialize as CFF INDEX
    auto *dest = c->push<CFF1Index> ();
    إذا (likely (dest->serialize (c, strings)))
      c->دەرهێنان_pack (خطأ);
    وإلا
    {
      c->دەرهێنان_discard ();
      return_trace (خطأ);
    }
  }

  // 8. CFF Header
  OT::cff1 *cff = c->allocate_min<OT::cff1> ();
  إذا (unlikely (!cff)) return_trace (خطأ);

  /* header */
  cff->version.major = 0x01;
  cff->version.minor = 0x00;
  cff->nameIndex = cff->min_size;
  cff->offSize = 4; /* unused? */

  // 9. Name INDEX (single entry)
  {
    unsigned name_len = strlen (CFF1_DEFAULT_FONT_NAME);

    CFF1Index *idx = c->start_embed<CFF1Index> ();
    إذا (unlikely (!idx)) return_trace (خطأ);

    إذا (unlikely (!idx->serialize_header (c, hb_iter (&name_len, 1), name_len)))
      return_trace (خطأ);

    إذا (unlikely (!c->embed (CFF1_DEFAULT_FONT_NAME, name_len)))
      return_trace (خطأ);
  }

  // 10. Top DICT INDEX
  {
    // Serialize the Top DICT data first
    c->push<لووتکە_Dict> ();
    cff1_from_cff2_لووتکە_dict_op_serializer_t لووتکە_Szr;

    // Serialize ROS first
    إذا (unlikely (!لووتکە_Szr.serialize_ros (c)))
    {
      c->دەرهێنان_discard ();
      return_trace (خطأ);
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

      إذا (encoder.in_error () || !c->embed (bbox_buff.as_bytes ().arrayZ, bbox_buff.length))
      {
        c->دەرهێنان_discard ();
        return_trace (خطأ);
      }
    }

    // Serialize charset operator
    إذا (charset_link && unlikely (!FontDict::serialize_link4_op (c, OpCode_charset, charset_link, whence_t::Absolute)))
    {
      c->دەرهێنان_discard ();
      return_trace (خطأ);
    }

    // Serialize FDSelect operator (required for CID-keyed CFF1 fonts)
    إذا (plan.info.fd_select.link && unlikely (!FontDict::serialize_link4_op (c, OpCode_FDSelect, plan.info.fd_select.link, whence_t::Absolute)))
    {
      c->دەرهێنان_discard ();
      return_trace (خطأ);
    }

    // Serialize FDArray operator (required for CID-keyed CFF1 fonts)
    إذا (plan.info.fd_array_link && unlikely (!FontDict::serialize_link4_op (c, OpCode_FDArray, plan.info.fd_array_link, whence_t::Absolute)))
    {
      c->دەرهێنان_discard ();
      return_trace (خطأ);
    }

    // Serialize other operators from CFF2 TopDict
    لكل (ثابت auto &opstr : cff2_لووتکە_Dict.values)
    {
      إذا (unlikely (!لووتکە_Szr.serialize (c, opstr, plan.info)))
      {
        c->دەرهێنان_discard ();
        return_trace (خطأ);
      }
    }

    unsigned لووتکە_size = c->length ();
    c->دەرهێنان_pack (خطأ);

    // Serialize INDEX header
    auto *dest = c->start_embed<CFF1Index> ();
    إذا (unlikely (!dest->serialize_header (c, hb_iter (&لووتکە_size, 1), لووتکە_size)))
      return_trace (خطأ);
  }

  return_trace (true);
}

} /* namespace CFF */

ساكن منطقي _serialize_cff2_charstrings (hb_serialize_context_t *c,
			     cff2_subset_plan &plan,
			     ثابت OT::cff2::accelerator_subset_t  &acc)
{
  c->push ();

  unsigned data_size = 0;
  unsigned total_size = CFF2CharStrings::total_size (plan.subset_charstrings, &data_size, plan.min_charstrings_off_size);
  إذا (unlikely (!c->start_zerocopy (total_size)))
    أرجع خطأ;

  auto *cs = c->start_embed<CFF2CharStrings> ();
  إذا (unlikely (!cs->serialize (c, plan.subset_charstrings, &data_size, plan.min_charstrings_off_size)))
  {
    c->دەرهێنان_discard ();
    أرجع خطأ;
  }

  plan.info.char_strings_link = c->دەرهێنان_pack (خطأ);
  أرجع true;
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
  hb_vector_t<table_info_t>  private_dict_infos;
  إذا (unlikely (!private_dict_infos.resize (plan.subset_fdcount))) أرجع خطأ;

  لكل (صحيح i = (صحيح)privateDicts.length; --i >= 0 ;)
  {
    إذا (plan.fdmap.has (i))
    {
      objidx_t	subrs_link = 0;

      إذا (plan.subset_localsubrs[i].length > 0)
      {
	auto *dest = c->push <CFF2Subrs> ();
	إذا (likely (dest->serialize (c, plan.subset_localsubrs[i])))
	  subrs_link = c->دەرهێنان_pack (خطأ);
	وإلا
	{
	  c->دەرهێنان_discard ();
	  أرجع خطأ;
	}
      }
      auto *pd = c->push<PrivateDict> ();
      cff2_private_dict_op_serializer_t privSzr (plan.desubroutinize, plan.drop_hints, plan.pinned,
						 varStore, normalized_coords,
						 plan.partial_instancing ? &plan.instancing : nullptr);
      إذا (likely (pd->serialize (c, privateDicts[i], privSzr, subrs_link)))
      {
	unsigned fd = plan.fdmap[i];
	private_dict_infos[fd].size = c->length ();
	private_dict_infos[fd].link = c->دەرهێنان_pack ();
      }
      وإلا
      {
	c->دەرهێنان_discard ();
	أرجع خطأ;
      }
    }
  }

  /* FDSelect */
  إذا (fdSelect != &Null (CFF2FDSelect))
  {
    c->push ();
    إذا (likely (hb_serialize_cff_fdselect (c, plan.ژمارە_glyphs, *(ثابت FDSelect *)fdSelect,
					   plan.orig_fdcount,
					   plan.subset_fdselect_format, plan.subset_fdselect_size,
					   plan.subset_fdselect_ranges)))
      plan.info.fd_select.link = c->دەرهێنان_pack ();
    وإلا
    {
      c->دەرهێنان_discard ();
      أرجع خطأ;
    }
  }

  /* FDArray (FD Index) */
  {
    auto *fda = c->push<CFF2FDArray> ();
    cff_font_dict_op_serializer_t fontSzr;
    auto it =
    + hb_zip (+ hb_iter (fontDicts)
	      | hb_filter ([&] (ثابت cff2_font_dict_بەهاکان_t &_)
		{ أرجع plan.fdmap.has (&_ - &fontDicts[0]); }),
	      hb_iter (private_dict_infos))
    ;
    إذا (unlikely (!fda->serialize (c, it, fontSzr)))
    {
      c->دەرهێنان_discard ();
      أرجع خطأ;
    }
    plan.info.fd_array_link = c->دەرهێنان_pack (خطأ);
  }

  /* variation store */
  إذا (varStore != &Null (CFF2ItemVariationStore) &&
      !plan.pinned)
  {
    إذا (plan.partial_instancing)
    {
      إذا (plan.instancing.has_variations ())
      {
	c->push ();
	إذا (unlikely (!plan.instancing.serialize_var_store (c, *plan.axis_tags)))
	{
	  c->دەرهێنان_discard ();
	  أرجع خطأ;
	}
	plan.info.var_store_link = c->دەرهێنان_pack (خطأ);
      }
      /* else: the store emptied; the vstore op gets dropped along with it. */
    }
    وإلا
    {
      auto *dest = c->push<CFF2ItemVariationStore> ();
      إذا (unlikely (!dest->serialize (c, varStore)))
      {
	c->دەرهێنان_discard ();
	أرجع خطأ;
      }
      plan.info.var_store_link = c->دەرهێنان_pack (خطأ);
    }
  }

  OT::cff2 *cff2 = c->allocate_min<OT::cff2> ();
  إذا (unlikely (!cff2)) أرجع خطأ;

  /* header */
  cff2->version.major = 0x02;
  cff2->version.minor = 0x00;
  cff2->لووتکە_Dict_2 = OT::cff2::static_size;

  /* top dict */
  {
    لووتکە_Dict &dict = cff2 + cff2->لووتکە_Dict_2;
    cff2_لووتکە_dict_op_serializer_t لووتکە_Szr;
    إذا (unlikely (!dict.serialize (c, لووتکە_Dict_2, لووتکە_Szr, plan.info))) أرجع خطأ;
    cff2->لووتکە_Dict_Size = c->head - (ثابت حرف *)&dict;
  }

  /* global subrs */
  {
    auto *dest = c->start_embed <CFF2Subrs> ();
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
    منطقي result = CFF::serialize_cff2_to_cff1 (c->serializer, cff2_plan, لووتکە_Dict_2, *هذا);

    إذا (c->serializer->ran_out_of_room ())
      أرجع true; // Subsetter will retry with larger buffer

    إذا (result && !c->serializer->in_error ())
    {
      // Success - end serialization to resolve links
      c->serializer->end_serialize ();

      // Copy the serialized CFF1 data and add as CFF table
      hb_blob_t *cff_blob = c->serializer->copy_blob ();
      إذا (cff_blob)
      {
        c->plan->add_table (HB_TAG('C','F','F',' '), cff_blob);
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

#endif
