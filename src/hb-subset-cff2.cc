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

#அந்தர்பாவ "hb.hh"

#ifndef HB_NO_SUBSET_CFF

#அந்தர்பாவ "hb-open-type.hh"
#அந்தர்பாவ "hb-ot-cff2-table.hh"
#அந்தர்பாவ "hb-ot-var-common.hh"
#அந்தர்பாவ "hb-set.h"
#அந்தர்பாவ "hb-subset-plan.hh"
#அந்தர்பாவ "hb-subset-cff-common.hh"
#அந்தர்பாவ "hb-cff2-interp-cs.hh"
#அந்தர்பாவ "hb-subset-cff2-to-cff1.hh"

using நாமாகாஶ CFF;

நாமாகாஶ CFF {

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
கட்டமைப்பு cff2_instancing_plan_t
{
  கட்டமைப்பு vardata_transform_t
  {
    /* new_deltas[j] = Σ_i matrix[j][i] * old_deltas[i] */
    hb_vector_t<hb_vector_t<தசம>> matrix;
    /* Default-value fold-in scalars, one per old region: the old store's
     * region scalars at the new default location. */
    hb_vector_t<தசம> gains;
    /* Surviving regions, parallel to matrix rows. */
    hb_vector_t<hb_hashmap_t<hb_tag_t, Triple>> regions;
    /* Index into the new global region list, per new region (finalize). */
    hb_vector_t<unsigned> region_indices;
    unsigned new_major = 0;
    தருக்க alive_ = பொய்;
    தருக்க alive () மாறிலி { திரும்பு alive_; }

    /* One value's delta for new region j, from its old per-region deltas. */
    இரட்டை transform_delta (unsigned j, hb_array_t<மாறிலி எண்_t> deltas) மாறிலி
    {
      மாறிலி hb_vector_t<தசம> &col = matrix.arrayZ[j];
      இரட்டை d = 0.;
      unsigned மொத்தம் = hb_min (col.length, deltas.length);
      க்கு (unsigned c = 0; c < மொத்தம்; c++)
	d += (இரட்டை) col.arrayZ[c] * deltas.arrayZ[c].to_real ();
      திரும்பு d;
    }
  };

  hb_vector_t<vardata_transform_t> transforms;  /* per old VarData (vsindex) */
  hb_vector_t<hb_hashmap_t<hb_tag_t, Triple>> new_regions;  /* global region list */
  hb_set_t used_majors;
  unsigned new_major_மொத்தம் = 0;

  தருக்க has_variations () மாறிலி { திரும்பு new_major_மொத்தம்; }

  மாறிலி vardata_transform_t *get_transform (unsigned major) மாறிலி
  { திரும்பு major < transforms.length ? &transforms.arrayZ[major] : nullptr; }

  /* Record that a retained blend references this VarData. */
  வெறுமை note_use (unsigned major) { used_majors.add (major); }

  /* The new vsindex a private dict's (or charstring's) old ivs maps to;
   * dead VarDatas fall back to 0, their blends having become static. */
  unsigned remap_ivs (unsigned old_ivs) மாறிலி
  {
    மாறிலி vardata_transform_t *t = get_transform (old_ivs);
    திரும்பு (t && t->alive ()) ? t->new_major : 0;
  }

  இரட்டை fold_deltas (unsigned old_ivs, hb_array_t<மாறிலி எண்_t> deltas) மாறிலி
  {
    மாறிலி vardata_transform_t *t = get_transform (old_ivs);
    எனில் (unlikely (!t || t->gains.length != deltas.length)) திரும்பு 0.;
    இரட்டை v = 0.;
    க்கு (unsigned i = 0; i < deltas.length; i++)
      v += (இரட்டை) t->gains.arrayZ[i] * deltas.arrayZ[i].to_real ();
    திரும்பு v;
  }

  தருக்க create (மாறிலி CFF2ItemVariationStore *cff2VarStore,
	       மாறிலி hb_subset_plan_t *plan)
  {
    மாறிலி OT::ItemVariationStore &varStore = cff2VarStore->varStore;
    unsigned major_மொத்தம் = varStore.get_sub_table_மொத்தம் ();
    எனில் (!transforms.resize (major_மொத்தம்)) திரும்பு பொய்;

    எனில் (!varStore.get_region_list ().get_var_regions (plan->axes_old_index_tag_map, orig_regions))
      திரும்பு பொய்;

    /* Instance each VarData with identity delta rows; the surviving tuples'
     * delta columns are the old-region → new-region transform. */
    OT::optimize_scratch_t scratch;
    க்கு (unsigned m = 0; m < major_மொத்தம்; m++)
    {
      மாறிலி OT::VarData &var_data = varStore.get_sub_table (m);
      unsigned k = var_data.get_region_index_மொத்தம் ();
      vardata_transform_t &t = transforms.arrayZ[m];

      எனில் (!t.gains.resize_exact (k)) திரும்பு பொய்;
      varStore.get_region_scalars (m, plan->normalized_coords.arrayZ,
				   plan->normalized_coords.length,
				   t.gains.arrayZ, k);

      OT::tuple_variations_t tuples;
      க்கு (unsigned r = 0; r < k; r++)
      {
	OT::tuple_delta_t tuple;
	எனில் (!tuple.deltas_x.resize (k) ||
	    !tuple.indices.resize (k))
	  திரும்பு பொய்;
	க்கு (unsigned i = 0; i < k; i++)
	  tuple.indices.arrayZ[i] = உண்மை;
	tuple.deltas_x.arrayZ[r] = 1.f;
	unsigned region_index = var_data.get_region_index (r);
	எனில் (unlikely (region_index >= orig_regions.length)) திரும்பு பொய்;
	tuple.axis_tuples = orig_regions.arrayZ[region_index];
	எனில் (unlikely (tuple.axis_tuples.in_error ())) திரும்பு பொய்;
	tuples.tuple_vars.push (std::move (tuple));
      }
      எனில் (unlikely (tuples.tuple_vars.in_error ())) திரும்பு பொய்;

      எனில் (!tuples.instantiate (plan->axes_location, plan->axes_triple_distances, scratch))
	திரும்பு பொய்;

      க்கு (auto &tuple : tuples.tuple_vars)
      {
	எனில் (unlikely (tuple.deltas_x.length != k)) திரும்பு பொய்;
	hb_vector_t<தசம> col;
	எனில் (unlikely (!col.resize_exact (k))) திரும்பு பொய்;
	க்கு (unsigned i = 0; i < k; i++)
	  col.arrayZ[i] = tuple.deltas_x.arrayZ[i];
	t.matrix.push (std::move (col));
	t.regions.push (std::move (tuple.axis_tuples));
      }
      எனில் (unlikely (t.matrix.in_error () || t.regions.in_error ())) திரும்பு பொய்;
    }

    திரும்பு உண்மை;
  }

  /* Decide which VarDatas survive, number them, and build the new global
   * region list: surviving original regions first, in their original order,
   * then solver-split new regions in order of appearance (fontTools'
   * rebuildRegions + prune_regions). */
  தருக்க finalize ()
  {
    க்கு (unsigned m = 0; m < transforms.length; m++)
    {
      vardata_transform_t &t = transforms.arrayZ[m];
      t.alive_ = t.matrix.length && used_majors.has (m);
      எனில் (t.alive_)
	t.new_major = new_major_மொத்தம்++;
    }

    /* Keys hash by content, so pointers into different owners compare fine. */
    hb_hashmap_t<மாறிலி hb_hashmap_t<hb_tag_t, Triple>*, unsigned> region_idx_map;
    hb_vector_t<மாறிலி hb_hashmap_t<hb_tag_t, Triple>*> region_ptrs;
    க்கு (மாறிலி vardata_transform_t &t : transforms)
    {
      எனில் (!t.alive ()) தொடர்;
      க்கு (மாறிலி auto &region : t.regions)
	எனில் (!region_idx_map.has (&region))
	  எனில் (unlikely (!region_idx_map.set (&region, (unsigned) -1)))
	    திரும்பு பொய்;
    }
    க்கு (மாறிலி auto &r : orig_regions)
    {
      unsigned *idx;
      எனில் (region_idx_map.has (&r, &idx) && *idx == (unsigned) -1)
      {
	எனில் (unlikely (!region_idx_map.set (&r, region_ptrs.length))) திரும்பு பொய்;
	region_ptrs.push (&r);
      }
    }
    க்கு (மாறிலி vardata_transform_t &t : transforms)
    {
      எனில் (!t.alive ()) தொடர்;
      க்கு (மாறிலி auto &region : t.regions)
      {
	unsigned *idx;
	எனில் (region_idx_map.has (&region, &idx) && *idx == (unsigned) -1)
	{
	  எனில் (unlikely (!region_idx_map.set (&region, region_ptrs.length))) திரும்பு பொய்;
	  region_ptrs.push (&region);
	}
      }
    }
    எனில் (unlikely (region_ptrs.in_error ())) திரும்பு பொய்;

    க்கு (vardata_transform_t &t : transforms)
    {
      எனில் (!t.alive ()) தொடர்;
      க்கு (மாறிலி auto &region : t.regions)
      {
	unsigned *idx;
	எனில் (unlikely (!region_idx_map.has (&region, &idx))) திரும்பு பொய்;
	t.region_indices.push (*idx);
      }
      எனில் (unlikely (t.region_indices.in_error ())) திரும்பு பொய்;
    }

    எனில் (!new_regions.resize (region_ptrs.length)) திரும்பு பொய்;
    க்கு (unsigned i = 0; i < region_ptrs.length; i++)
    {
      new_regions.arrayZ[i] = *region_ptrs.arrayZ[i];
      எனில் (unlikely (new_regions.arrayZ[i].in_error ())) திரும்பு பொய்;
    }

    திரும்பு உண்மை;
  }

  /* Serialize the instanced store: a CFF2ItemVariationStore whose VarDatas
   * carry no items, just region indexes (blend deltas live in charstrings).
   * Laid out manually since all sizes are known up front. */
  தருக்க serialize_var_store (hb_serialize_context_t *c,
			    மாறிலி hb_vector_t<hb_tag_t> &axis_tags) மாறிலி
  {
    unsigned data_மொத்தம் = new_major_மொத்தம்;
    unsigned header_size = 2 + 4 + 2 + 4 * data_மொத்தம்;

    uint64_t offset = header_size;
    hb_vector_t<unsigned> data_offsets;
    க்கு (மாறிலி vardata_transform_t &t : transforms)
    {
      எனில் (!t.alive ()) தொடர்;
      data_offsets.push ((unsigned) offset);
      offset += 6 + 2 * (uint64_t) t.region_indices.length;
    }
    எனில் (unlikely (data_offsets.in_error () || data_offsets.length != data_மொத்தம்))
      திரும்பு பொய்;
    unsigned regions_offset = (unsigned) offset;
    uint64_t total = offset + 4 + (uint64_t) new_regions.length * axis_tags.length * 6;
    எனில் (unlikely (total > 0xFFFFFFFFu)) திரும்பு பொய்;

    HBUINT16 *size_field = c->allocate_size<HBUINT16> (HBUINT16::static_size);
    எனில் (unlikely (!size_field)) திரும்பு பொய்;
    /* OFF 2026 5.3.3.11.2 permits larger stores with length=65535. */
    *size_field = hb_min (total, (uint64_t) 0xFFFFu);

    எழுத்து *header = c->allocate_size<எழுத்து> (header_size);
    எனில் (unlikely (!header)) திரும்பு பொய்;
    * (HBUINT16 *) (header + 0) = 1;			/* format */
    * (HBUINT32 *) (header + 2) = regions_offset;
    * (HBUINT16 *) (header + 6) = data_மொத்தம்;
    க்கு (unsigned i = 0; i < data_மொத்தம்; i++)
      * (HBUINT32 *) (header + 8 + 4 * i) = data_offsets.arrayZ[i];

    க்கு (மாறிலி vardata_transform_t &t : transforms)
    {
      எனில் (!t.alive ()) தொடர்;
      unsigned k = t.region_indices.length;
      HBUINT16 *v = (HBUINT16 *) c->allocate_size<எழுத்து> (6 + 2 * k);
      எனில் (unlikely (!v)) திரும்பு பொய்;
      v[0] = 0;						/* itemCount */
      v[1] = 0;						/* wordSizeCount */
      v[2] = k;						/* regionIndices.len */
      க்கு (unsigned j = 0; j < k; j++)
	v[3 + j] = t.region_indices.arrayZ[j];
    }

    hb_vector_t<மாறிலி hb_hashmap_t<hb_tag_t, Triple>*> region_ptrs;
    எனில் (unlikely (!region_ptrs.alloc_exact (new_regions.length))) திரும்பு பொய்;
    க்கு (மாறிலி auto &r : new_regions)
      region_ptrs.push (&r);
    auto *region_list = c->start_embed<OT::VarRegionList> ();
    திரும்பு region_list->serialize (c, axis_tags, region_ptrs);
  }

  தனிப்பட்ட:
  hb_vector_t<hb_hashmap_t<hb_tag_t, Triple>> orig_regions;
};

/* Usage-collection pass for partial instancing: record which VarDatas the
 * retained charstrings' and private dicts' blends reference.  Mirrors the
 * emission pass: blends consumed by dropped hint ops don't count. */
கட்டமைப்பு cff2_usage_param_t
{
  வெறுமை init () {}

  cff2_instancing_plan_t *instancing = nullptr;
  தருக்க drop_hints = பொய்;
  unsigned ivs = 0;	/* dict-side current vsindex */
};

கட்டமைப்பு cff2_cs_opset_usage_t : cff2_cs_opset_t<cff2_cs_opset_usage_t, cff2_usage_param_t, blend_arg_t>
{
  நிலையான வெறுமை flush_args_and_op (op_code_t op, cff2_cs_interp_env_t<blend_arg_t> &env, cff2_usage_param_t& param)
  {
    தருக்க counts = உண்மை;
    தேர்வு (op)
    {
      நிலை OpCode_return:
      நிலை OpCode_endchar:
	counts = பொய்;
	நிறுத்து;

      நிலை OpCode_hstem:
      நிலை OpCode_hstemhm:
      நிலை OpCode_vstem:
      நிலை OpCode_vstemhm:
      நிலை OpCode_hintmask:
      நிலை OpCode_cntrmask:
	எனில் (param.drop_hints)
	  counts = பொய்;
	நிறுத்து;

      இயல்பு:
	நிறுத்து;
    }

    எனில் (counts)
      க்கு (unsigned முழு i = 0; i < env.arg_அடுக்கு.get_மொத்தம் (); i++)
	எனில் (env.arg_அடுக்கு[i].blending ())
	{
	  param.instancing->note_use (env.get_ivs ());
	  நிறுத்து;
	}

    env.clear_args ();
  }
};

கட்டமைப்பு cff2_private_dict_usage_opset_t : dict_opset_t
{
  நிலையான வெறுமை process_op (op_code_t op, cff2_priv_dict_interp_env_t& env, cff2_usage_param_t& param)
  {
    தேர்வு (op) {
      நிலை OpCode_vsindexdict:
	env.process_vsindex ();
	param.ivs = env.get_ivs ();
	env.clear_args ();
	திரும்பு;
      நிலை OpCode_blenddict:
	param.instancing->note_use (param.ivs);
	env.clear_args ();
	திரும்பு;
      இயல்பு:
	dict_opset_t::process_op (op, env);
	எனில் (!env.arg_அடுக்கு.is_empty ()) திரும்பு;
	env.clear_args ();
	திரும்பு;
    }
  }
};

} /* namespace CFF */

கட்டமைப்பு cff2_sub_table_info_t : cff_sub_table_info_t
{
  cff2_sub_table_info_t ()
    : cff_sub_table_info_t (),
      var_store_link (0)
  {}

  objidx_t  var_store_link;
};

கட்டமைப்பு cff2_உச்சி_dict_op_serializer_t : cff_உச்சி_dict_op_serializer_t<>
{
  தருக்க serialize (hb_serialize_context_t *c,
		  மாறிலி op_str_t &opstr,
		  மாறிலி cff2_sub_table_info_t &info) மாறிலி
  {
    TRACE_SERIALIZE (இது);

    தேர்வு (opstr.op)
    {
      நிலை OpCode_vstore:
        எனில் (info.var_store_link)
	  return_trace (FontDict::serialize_link4_op(c, opstr.op, info.var_store_link));
	இல்லையெனில்
	  return_trace (உண்மை);

      இயல்பு:
	return_trace (cff_உச்சி_dict_op_serializer_t<>::serialize (c, opstr, info));
    }
  }
};

கட்டமைப்பு cff2_cs_opset_flatten_t : cff2_cs_opset_t<cff2_cs_opset_flatten_t, flatten_param_t, blend_arg_t>
{
  நிலையான வெறுமை flush_args_and_op (op_code_t op, cff2_cs_interp_env_t<blend_arg_t> &env, flatten_param_t& param)
  {
    /* Optionally capture command for specialization (before flushing, to preserve args) */
    எனில் (param.commands)
    {
      தருக்க skip_command = பொய்;

      தேர்வு (op)
      {
	நிலை OpCode_return:
	நிலை OpCode_endchar:
	  skip_command = உண்மை;
	  நிறுத்து;

	நிலை OpCode_hstem:
	நிலை OpCode_hstemhm:
	நிலை OpCode_vstem:
	நிலை OpCode_vstemhm:
	நிலை OpCode_hintmask:
	நிலை OpCode_cntrmask:
	  எனில் (param.drop_hints)
	    skip_command = உண்மை;
	  நிறுத்து;

	இயல்பு:
	  நிறுத்து;
      }

      எனில் (!skip_command)
      {
	cs_command_t cmd (op);
	/* Capture resolved blend values */
	க்கு (unsigned முழு i = 0; i < env.arg_அடுக்கு.get_மொத்தம் ();)
	{
	  மாறிலி blend_arg_t &arg = env.arg_அடுக்கு[i];
	  எனில் (arg.blending ())
	  {
	    /* For blend args, capture only the resolved default value */
	    cmd.args.push (arg);
	    /* Skip over the multiple blend values */
	    i += arg.இலக்கம்_மதிப்புகள்_2;
	  }
	  இல்லையெனில்
	  {
	    cmd.args.push (arg);
	    i++;
	  }
	}
	param.commands->push (cmd);
      }
    }

    தேர்வு (op)
    {
      நிலை OpCode_return:
      நிலை OpCode_endchar:
	/* dummy opcodes in CFF2. ignore */
	எனில் (op == OpCode_endchar && param.instancer)
	  emit_vsindex_prefix (env, param);
	நிறுத்து;

      நிலை OpCode_hstem:
      நிலை OpCode_hstemhm:
      நிலை OpCode_vstem:
      நிலை OpCode_vstemhm:
      நிலை OpCode_hintmask:
      நிலை OpCode_cntrmask:
	எனில் (param.drop_hints)
	{
	  env.clear_args ();
	  திரும்பு;
	}
	HB_FALLTHROUGH;

      இயல்பு:
	SUPER::flush_args_and_op (op, env, param);
	நிறுத்து;
    }
  }

  நிலையான வெறுமை flush_args (cff2_cs_interp_env_t<blend_arg_t> &env, flatten_param_t& param)
  {
    க்கு (unsigned முழு i = 0; i < env.arg_அடுக்கு.get_மொத்தம் ();)
    {
      மாறிலி blend_arg_t &arg = env.arg_அடுக்கு[i];
      எனில் (arg.blending ())
      {
	எனில் (unlikely (!((arg.இலக்கம்_மதிப்புகள்_2 > 0) && (env.arg_அடுக்கு.get_மொத்தம் () >= arg.இலக்கம்_மதிப்புகள்_2))))
	{
	  env.set_error ();
	  திரும்பு;
	}
	எனில் (param.instancer)
	  rewrite_blends (arg, i, env, param);
	இல்லையெனில்
	  flatten_blends (arg, i, env, param);
	i += arg.இலக்கம்_மதிப்புகள்_2;
      }
      இல்லையெனில்
      {
	str_encoder_t  encoder (param.flatStr);
	encoder.encode_இலக்கம்_cs (arg);
	i++;
      }
    }
    SUPER::flush_args (env, param);
  }

  நிலையான வெறுமை flatten_blends (மாறிலி blend_arg_t &arg, unsigned முழு i, cff2_cs_interp_env_t<blend_arg_t> &env, flatten_param_t& param)
  {
    /* flatten the default values */
    str_encoder_t  encoder (param.flatStr);
    க்கு (unsigned முழு j = 0; j < arg.இலக்கம்_மதிப்புகள்_2; j++)
    {
      மாறிலி blend_arg_t &arg1 = env.arg_அடுக்கு[i + j];
      எனில் (unlikely (!((arg1.blending () && (arg.இலக்கம்_மதிப்புகள்_2 == arg1.இலக்கம்_மதிப்புகள்_2) && (arg1.மதிப்பு_Index_2 == j) &&
	      (arg1.deltas.length == env.get_region_மொத்தம் ())))))
      {
	env.set_error ();
	திரும்பு;
      }
      encoder.encode_இலக்கம்_cs (arg1);
    }
    /* flatten deltas for each value */
    க்கு (unsigned முழு j = 0; j < arg.இலக்கம்_மதிப்புகள்_2; j++)
    {
      மாறிலி blend_arg_t &arg1 = env.arg_அடுக்கு[i + j];
      க்கு (unsigned முழு k = 0; k < arg1.deltas.length; k++)
	encoder.encode_இலக்கம்_cs (arg1.deltas[k]);
    }
    /* flatten the number of values followed by blend operator */
    encoder.encode_int (arg.இலக்கம்_மதிப்புகள்_2);
    encoder.encode_op (OpCode_blendcs);
  }

  /* Partial instancing: rewrite one blend group against the instanced store.
   * Each value's default absorbs the folded (pinned-away) contribution, and
   * its deltas are mapped onto the new region list. */
  நிலையான வெறுமை rewrite_blends (மாறிலி blend_arg_t &arg, unsigned முழு i, cff2_cs_interp_env_t<blend_arg_t> &env, flatten_param_t& param)
  {
    மாறிலி cff2_instancing_plan_t &instancer = *param.instancer;
    unsigned ivs = env.get_ivs ();
    மாறிலி cff2_instancing_plan_t::vardata_transform_t *t = instancer.get_transform (ivs);
    எனில் (unlikely (!t))
    {
      env.set_error ();
      திரும்பு;
    }

    unsigned n = arg.இலக்கம்_மதிப்புகள்_2;
    க்கு (unsigned முழு j = 0; j < n; j++)
    {
      மாறிலி blend_arg_t &arg1 = env.arg_அடுக்கு[i + j];
      எனில் (unlikely (!(arg1.blending () && (arg1.இலக்கம்_மதிப்புகள்_2 == n) && (arg1.மதிப்பு_Index_2 == j) &&
		      (arg1.deltas.length == env.get_region_மொத்தம் ()) &&
		      (arg1.deltas.length == t->gains.length))))
      {
	env.set_error ();
	திரும்பு;
      }
    }

    str_encoder_t encoder (param.flatStr);
    unsigned k_new = t->alive () ? t->matrix.length : 0;

    எனில் (!k_new)
    {
      /* All of this VarData's regions died; blends dissolve into statics. */
      க்கு (unsigned முழு j = 0; j < n; j++)
      {
	மாறிலி blend_arg_t &arg1 = env.arg_அடுக்கு[i + j];
	எண்_t v;
	v.set_real (arg1.to_real () + round (instancer.fold_deltas (ivs, arg1.deltas.as_array ())));
	encoder.encode_இலக்கம்_cs (v);
      }
      திரும்பு;
    }

    /* Emit in chunks so the output charstring stays within the argument
     * stack limit: values already emitted for this op, plus this chunk's
     * defaults, deltas and count, must not exceed 513. */
    unsigned done = 0;
    போது (done < n)
    {
      unsigned avail = 512 > (i + done) ? 512 - (i + done) : 0;
      unsigned chunk = hb_min (avail / (k_new + 1), n - done);
      எனில் (unlikely (!chunk))
      {
	env.set_error ();
	திரும்பு;
      }

      க்கு (unsigned முழு j = done; j < done + chunk; j++)
      {
	மாறிலி blend_arg_t &arg1 = env.arg_அடுக்கு[i + j];
	எண்_t v;
	v.set_real (arg1.to_real () + round (instancer.fold_deltas (ivs, arg1.deltas.as_array ())));
	encoder.encode_இலக்கம்_cs (v);
      }
      க்கு (unsigned முழு j = done; j < done + chunk; j++)
      {
	மாறிலி blend_arg_t &arg1 = env.arg_அடுக்கு[i + j];
	க்கு (unsigned r = 0; r < k_new; r++)
	{
	  எண்_t v;
	  v.set_int (hb_clamp_to<முழு> (round (t->transform_delta (r, arg1.deltas.as_array ()))));
	  encoder.encode_இலக்கம்_cs (v);
	}
      }
      encoder.encode_int (chunk);
      encoder.encode_op (OpCode_blendcs);
      done += chunk;
    }
    param.emitted_blend = உண்மை;
  }

  /* Partial instancing: prepend a vsindex op when the charstring's
   * (remapped) ivs differs from its FD's (remapped) private-dict ivs. */
  நிலையான வெறுமை emit_vsindex_prefix (cff2_cs_interp_env_t<blend_arg_t> &env, flatten_param_t& param)
  {
    எனில் (!param.emitted_blend) திரும்பு;
    மாறிலி cff2_instancing_plan_t &instancer = *param.instancer;
    unsigned new_ivs = instancer.remap_ivs (env.get_ivs ());
    unsigned fd_ivs = instancer.remap_ivs (env.get_orig_ivs ());
    எனில் (new_ivs == fd_ivs) திரும்பு;

    str_buff_t prefix;
    str_encoder_t enc (prefix);
    enc.encode_int ((முழு) new_ivs);
    enc.encode_op (OpCode_vsindexcs);

    str_buff_t &buf = param.flatStr;
    unsigned old_len = buf.length;
    எனில் (unlikely (prefix.in_error () || !buf.resize (old_len + prefix.length)))
    {
      env.set_error ();
      திரும்பு;
    }
    memmove (buf.arrayZ + prefix.length, buf.arrayZ, old_len);
    hb_memcpy (buf.arrayZ, prefix.arrayZ, prefix.length);
  }

  நிலையான வெறுமை flush_op (op_code_t op, cff2_cs_interp_env_t<blend_arg_t> &env, flatten_param_t& param)
  {
    தேர்வு (op)
    {
      நிலை OpCode_return:
      நிலை OpCode_endchar:
	திரும்பு;
      இயல்பு:
	str_encoder_t  encoder (param.flatStr);
	encoder.encode_op (op);
    }
  }

  நிலையான வெறுமை flush_hintmask (op_code_t op, cff2_cs_interp_env_t<blend_arg_t> &env, flatten_param_t& param)
  {
    SUPER::flush_hintmask (op, env, param);
    /* Preserve hintmask payload in captured commands for specializer re-encoding. */
    எனில் (param.commands && !param.drop_hints && param.commands->length > 0)
    {
      auto &cmd = param.commands->tail ();
      எனில் (cmd.op == op)
      {
        cmd.mask_bytes.resize (env.hintmask_size);
        எனில் (unlikely (cmd.mask_bytes.in_error ()))
        {
          env.set_error ();
          திரும்பு;
        }
        க்கு (unsigned முழு i = 0; i < env.hintmask_size; i++)
          cmd.mask_bytes[i] = env.str_ref[i];
      }
    }
    எனில் (!param.drop_hints)
    {
      str_encoder_t  encoder (param.flatStr);
      க்கு (unsigned முழு i = 0; i < env.hintmask_size; i++)
	encoder.encode_byte (env.str_ref[i]);
    }
  }

  தனிப்பட்ட:
  வகைவரை cff2_cs_opset_t<cff2_cs_opset_flatten_t, flatten_param_t, blend_arg_t> SUPER;
  வகைவரை cs_opset_t<blend_arg_t, cff2_cs_opset_flatten_t, cff2_cs_opset_flatten_t, cff2_cs_interp_env_t<blend_arg_t>, flatten_param_t> CSOPSET;
};

கட்டமைப்பு cff2_cs_opset_subr_subset_t : cff2_cs_opset_t<cff2_cs_opset_subr_subset_t, subr_subset_param_t, blend_arg_t>
{
  நிலையான வெறுமை process_op (op_code_t op, cff2_cs_interp_env_t<blend_arg_t> &env, subr_subset_param_t& param)
  {
    தேர்வு (op) {

      நிலை OpCode_return:
	param.current_parsed_str->flush_coalesced (env.str_ref);
	param.current_parsed_str->set_parsed ();
	env.return_from_subr ();
	param.set_current_str (env, பொய்);
	நிறுத்து;

      நிலை OpCode_endchar:
	param.current_parsed_str->flush_coalesced (env.str_ref);
	param.current_parsed_str->set_parsed ();
	SUPER::process_op (op, env, param);
	நிறுத்து;

      நிலை OpCode_callsubr:
	process_call_subr (op, CSType_LocalSubr, env, param, env.localSubrs, param.local_closure);
	நிறுத்து;

      நிலை OpCode_callgsubr:
	process_call_subr (op, CSType_GlobalSubr, env, param, env.globalSubrs, param.global_closure);
	நிறுத்து;

      இயல்பு:
	SUPER::process_op (op, env, param);
	param.current_parsed_str->add_op (op, env.str_ref);
	நிறுத்து;
    }
  }

  பாதுகாக்கப்பட்ட:
  நிலையான வெறுமை process_call_subr (op_code_t op, cs_type_t type,
				 cff2_cs_interp_env_t<blend_arg_t> &env, subr_subset_param_t& param,
				 cff2_biased_subrs_t& subrs, hb_set_t *closure)
  {
    byte_str_ref_t    str_ref = env.str_ref;
    env.call_subr (subrs, type);
    param.current_parsed_str->add_call_op (op, str_ref, env.context.subr_இலக்கம்_2);
    closure->add (env.context.subr_இலக்கம்_2);
    param.set_current_str (env, உண்மை);
  }

  தனிப்பட்ட:
  வகைவரை cff2_cs_opset_t<cff2_cs_opset_subr_subset_t, subr_subset_param_t, blend_arg_t> SUPER;
};

கட்டமைப்பு cff2_subr_subsetter_t : subr_subsetter_t<cff2_subr_subsetter_t, CFF2Subrs, மாறிலி OT::cff2::accelerator_subset_t, cff2_cs_interp_env_t<blend_arg_t>, cff2_cs_opset_subr_subset_t>
{
  cff2_subr_subsetter_t (மாறிலி OT::cff2::accelerator_subset_t &acc_, மாறிலி hb_subset_plan_t *plan_)
    : subr_subsetter_t (acc_, plan_) {}

  நிலையான வெறுமை complete_parsed_str (cff2_cs_interp_env_t<blend_arg_t> &env, subr_subset_param_t& param, parsed_cs_str_t &charstring)
  {
    /* vsindex is inserted at the beginning of the charstring as necessary */
    எனில் (env.seen_vsindex ())
    {
      எண்_t  ivs;
      ivs.set_int ((முழு)env.get_ivs ());
      charstring.set_prefix (ivs, OpCode_vsindexcs);
    }
  }
};

கட்டமைப்பு cff2_private_blend_encoder_param_t
{
  cff2_private_blend_encoder_param_t (hb_serialize_context_t *c,
				      மாறிலி CFF2ItemVariationStore *varStore,
				      hb_array_t<முழு> normalized_coords) :
    c (c), varStore (varStore), normalized_coords (normalized_coords) {}

  வெறுமை init () {}

  வெறுமை process_blend ()
  {
    எனில் (!seen_blend)
    {
      region_மொத்தம்_2 = varStore->varStore.get_region_index_மொத்தம் (ivs);
      எனில் (unlikely (!scalars.resize_exact (region_மொத்தம்_2)))
      {
	எனில் (c) c->err (HB_SERIALIZE_ERROR_OTHER);
	seen_blend = உண்மை;
	திரும்பு;
      }
      varStore->varStore.get_region_scalars (ivs, normalized_coords.arrayZ, normalized_coords.length,
					     &scalars[0], region_மொத்தம்_2);
      seen_blend = உண்மை;
    }
  }

  இரட்டை blend_deltas (hb_array_t<மாறிலி எண்_t> deltas) மாறிலி
  {
    இரட்டை v = 0;
    எனில் (likely (scalars.length == deltas.length))
    {
      unsigned மொத்தம் = scalars.length;
      க்கு (unsigned i = 0; i < மொத்தம்; i++)
	v += (இரட்டை) scalars.arrayZ[i] * deltas.arrayZ[i].to_real ();
    }
    திரும்பு v;
  }


  hb_serialize_context_t *c = nullptr;
  தருக்க seen_blend = பொய்;
  unsigned ivs = 0;
  unsigned region_மொத்தம்_2 = 0;
  hb_vector_t<தசம> scalars;
  மாறிலி	 CFF2ItemVariationStore *varStore = nullptr;
  hb_array_t<முழு> normalized_coords;

  /* CFF2 partial instancing: when set, blends are rewritten against the
   * instanced variation store instead of being flattened. */
  மாறிலி cff2_instancing_plan_t *instancer = nullptr;
  /* Error-compensated fold rounding state, per dict operator. */
  இரட்டை fold_exact = 0.;
  இரட்டை fold_emitted = 0.;
};

கட்டமைப்பு cff2_private_dict_blend_opset_t : dict_opset_t
{
  நிலையான வெறுமை process_arg_blend (cff2_private_blend_encoder_param_t& param,
				 எண்_t &arg,
				 மாறிலி hb_array_t<மாறிலி எண்_t> blends,
				 unsigned n, unsigned i)
  {
    arg.set_int (round (arg.to_real () + param.blend_deltas (blends)));
  }

  /* The fold added to a value, error-compensated across the operator: most
   * blended private-dict values (BlueValues, StemSnap*, ...) are
   * delta-encoded, so independently rounded folds would accumulate in the
   * decoded absolutes; keeping the emitted cumulative fold equal to the
   * rounded exact cumulative fold bounds every absolute within ±0.5. */
  நிலையான இரட்டை fold (cff2_private_blend_encoder_param_t& param,
		      மாறிலி hb_array_t<மாறிலி எண்_t> blends)
  {
    இரட்டை f = param.blend_deltas (blends);
    இரட்டை emit = round (param.fold_exact + f) - param.fold_emitted;
    param.fold_exact += f;
    param.fold_emitted += emit;
    திரும்பு emit;
  }

  /* Partial instancing: emit the args preceding this blend group, then the
   * group itself rewritten against the instanced store, and clear the
   * stack.  The op's remaining args are flushed at process_op time. */
  நிலையான வெறுமை rewrite_blends (cff2_priv_dict_interp_env_t& env,
			      cff2_private_blend_encoder_param_t& param,
			      unsigned start, unsigned n, unsigned k)
  {
    மாறிலி cff2_instancing_plan_t &instancer = *param.instancer;
    மாறிலி cff2_instancing_plan_t::vardata_transform_t *t = instancer.get_transform (param.ivs);
    எனில் (unlikely (!t || t->gains.length != k))
    {
      env.set_error ();
      திரும்பு;
    }

    str_buff_t str;
    str_encoder_t encoder (str);

    க்கு (unsigned idx = 0; idx < start; idx++)
      encoder.encode_இலக்கம்_tp (env.arg_அடுக்கு[idx]);

    unsigned k_new = t->alive () ? t->matrix.length : 0;
    எனில் (!k_new)
    {
      /* This VarData died (or is unused by any retained charstring, which
       * can happen for FDs kept only for retain-gids holes); blends
       * dissolve into statics. */
      க்கு (unsigned j = 0; j < n; j++)
      {
	மாறிலி hb_array_t<மாறிலி எண்_t> blends = env.arg_அடுக்கு.sub_array (start + n + (j * k), k);
	எண்_t v;
	v.set_real (env.arg_அடுக்கு[start + j].to_real () + fold (param, blends));
	encoder.encode_இலக்கம்_tp (v);
      }
    }
    இல்லையெனில்
    {
      unsigned done = 0;
      போது (done < n)
      {
	unsigned avail = 512 > (start + done) ? 512 - (start + done) : 0;
	unsigned chunk = hb_min (avail / (k_new + 1), n - done);
	எனில் (unlikely (!chunk))
	{
	  env.set_error ();
	  திரும்பு;
	}

	க்கு (unsigned j = done; j < done + chunk; j++)
	{
	  மாறிலி hb_array_t<மாறிலி எண்_t> blends = env.arg_அடுக்கு.sub_array (start + n + (j * k), k);
	  எண்_t v;
	  v.set_real (env.arg_அடுக்கு[start + j].to_real () + fold (param, blends));
	  encoder.encode_இலக்கம்_tp (v);
	}
	க்கு (unsigned j = done; j < done + chunk; j++)
	{
	  மாறிலி hb_array_t<மாறிலி எண்_t> blends = env.arg_அடுக்கு.sub_array (start + n + (j * k), k);
	  க்கு (unsigned r = 0; r < k_new; r++)
	  {
	    எண்_t v;
	    v.set_int (hb_clamp_to<முழு> (round (t->transform_delta (r, blends))));
	    encoder.encode_இலக்கம்_tp (v);
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

  நிலையான வெறுமை process_blend (cff2_priv_dict_interp_env_t& env, cff2_private_blend_encoder_param_t& param)
  {
    unsigned முழு n, k;

    param.process_blend ();
    k = param.region_மொத்தம்_2;
    n = env.arg_அடுக்கு.எடு_uint ();

    unsigned மொத்தம் = env.arg_அடுக்கு.get_மொத்தம் ();
    unsigned முழு total;
    எனில் (unlikely (hb_unsigned_mul_overflows (k + 1, n, &total) || total > மொத்தம்))
    {
      env.set_error ();
      திரும்பு;
    }

    /* copy the blend values into blend array of the default values */
    unsigned முழு start = மொத்தம் - total;
    /* let an obvious error case fail, but note CFF2 spec doesn't forbid n==0 */
    எனில் (unlikely (start > மொத்தம்))
    {
      env.set_error ();
      திரும்பு;
    }

    எனில் (param.instancer && k)
    {
      rewrite_blends (env, param, start, n, k);
      திரும்பு;
    }

    க்கு (unsigned முழு i = 0; i < n; i++)
    {
      மாறிலி hb_array_t<மாறிலி எண்_t> blends = env.arg_அடுக்கு.sub_array (start + n + (i * k), k);
      process_arg_blend (param, env.arg_அடுக்கு[start + i], blends, n, i);
    }

    /* pop off blend values leaving default values now adorned with blend values */
    env.arg_அடுக்கு.எடு (k * n);
  }

  நிலையான வெறுமை process_op (op_code_t op, cff2_priv_dict_interp_env_t& env, cff2_private_blend_encoder_param_t& param)
  {
    தேர்வு (op) {
      நிலை OpCode_StdHW:
      நிலை OpCode_StdVW:
      நிலை OpCode_BlueScale:
      நிலை OpCode_BlueShift:
      நிலை OpCode_BlueFuzz:
      நிலை OpCode_ExpansionFactor:
      நிலை OpCode_LanguageGroup:
      நிலை Op_Code_Blue_மதிப்புகள்:
      நிலை OpCode_OtherBlues:
      நிலை OpCode_FamilyBlues:
      நிலை OpCode_FamilyOtherBlues:
      நிலை OpCode_StemSnapH:
      நிலை OpCode_StemSnapV:
	நிறுத்து;
      நிலை OpCode_vsindexdict:
	env.process_vsindex ();
	param.ivs = env.get_ivs ();
	எனில் (param.instancer)
	{
	  /* Re-emit with the remapped vsindex; 0 is the dict default. */
	  unsigned new_ivs = param.instancer->remap_ivs (param.ivs);
	  எனில் (new_ivs)
	  {
	    str_buff_t str;
	    str_encoder_t encoder (str);
	    encoder.encode_int ((முழு) new_ivs);
	    encoder.encode_op (OpCode_vsindexdict);
	    auto bytes = str.as_bytes ();
	    param.c->embed (&bytes, bytes.length);
	  }
	}
	env.clear_args ();
	திரும்பு;
      நிலை OpCode_blenddict:
	process_blend (env, param);
	திரும்பு;

      இயல்பு:
	dict_opset_t::process_op (op, env);
	எனில் (!env.arg_அடுக்கு.is_empty ()) திரும்பு;
	நிறுத்து;
    }

    எனில் (unlikely (env.in_error ())) திரும்பு;

    // Write args then op

    str_buff_t str;
    str_encoder_t encoder (str);

    unsigned மொத்தம் = env.arg_அடுக்கு.get_மொத்தம் ();
    க்கு (unsigned i = 0; i < மொத்தம்; i++)
      encoder.encode_இலக்கம்_tp (env.arg_அடுக்கு[i]);

    encoder.encode_op (op);

    auto bytes = str.as_bytes ();
    param.c->embed (&bytes, bytes.length);

    env.clear_args ();
    param.fold_exact = param.fold_emitted = 0.;
  }
};

கட்டமைப்பு cff2_private_dict_op_serializer_t : op_serializer_t
{
  cff2_private_dict_op_serializer_t (தருக்க desubroutinize_, தருக்க drop_hints_, தருக்க pinned_,
				     மாறிலி CFF::CFF2ItemVariationStore* varStore_,
				     hb_array_t<முழு> normalized_coords_,
				     மாறிலி cff2_instancing_plan_t *instancer_ = nullptr)
    : desubroutinize (desubroutinize_), drop_hints (drop_hints_), pinned (pinned_),
      param (nullptr, varStore_, normalized_coords_)
  { param.instancer = instancer_; }

  தருக்க serialize (hb_serialize_context_t *c,
		  மாறிலி op_str_t &opstr,
		  objidx_t subrs_link) மாறிலி
  {
    TRACE_SERIALIZE (இது);

    எனில் (drop_hints && dict_opset_t::is_hint_op (opstr.op))
      return_trace (உண்மை);

    எனில் (opstr.op == OpCode_Subrs)
    {
      எனில் (desubroutinize || !subrs_link)
	return_trace (உண்மை);
      இல்லையெனில்
	return_trace (FontDict::serialize_link2_op (c, opstr.op, subrs_link));
    }

    எனில் (pinned || param.instancer)
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

  பாதுகாக்கப்பட்ட:
  மாறிலி தருக்க desubroutinize;
  மாறிலி தருக்க drop_hints;
  மாறிலி தருக்க pinned;
  mutable cff2_private_blend_encoder_param_t param;
};



நாமாகாஶ OT {
கட்டமைப்பு cff2_subset_plan
{
  தருக்க create (மாறிலி OT::cff2::accelerator_subset_t &acc,
	      hb_subset_plan_t *plan)
  {
    /* make sure notdef is first */
    hb_codepoint_t old_glyph;
    எனில் (!plan->old_gid_for_new_gid (0, &old_glyph) || (old_glyph != 0)) திரும்பு பொய்;

    இலக்கம்_glyphs = plan->இலக்கம்_output_glyphs ();
    orig_fdcount = acc.fdArray->மொத்தம்;

    drop_hints = plan->flags & HB_SUBSET_FLAGS_NO_HINTING;
    தருக்க instancing_requested = (தருக்க) plan->normalized_coords;
    pinned = instancing_requested && plan->all_axes_pinned;
    normalized_coords = plan->normalized_coords;
    head_maxp_info = plan->head_maxp_info;
    hmtx_map = &plan->hmtx_map;
    desubroutinize = plan->flags & HB_SUBSET_FLAGS_DESUBROUTINIZE ||
		     instancing_requested; // For instancing we need this path

    /* Partial instancing: some axes pinned or limited, others kept. */
    partial_instancing = instancing_requested && !pinned &&
			 acc.varStore != &Null (CFF2ItemVariationStore);
    எனில் (partial_instancing &&
	unlikely (!instancing.create (acc.varStore, plan)))
      திரும்பு பொய்;
    axis_tags = &plan->axis_tags;

    /* Enable command capture for CFF2→CFF1 conversion (for specialization) */
    capture_commands = pinned;

 #ifdef HB_EXPERIMENTAL_API
    min_charstrings_off_size = (plan->flags & HB_SUBSET_FLAGS_IFTB_REQUIREMENTS) ? 4 : 0;
 #இல்லையெனில்
    min_charstrings_off_size = 0;
 #endif

    எனில் (desubroutinize)
    {
      எனில் (partial_instancing)
      {
	/* Pass 1: collect which VarDatas the retained blends reference, so
	 * unused ones get pruned and the survivors renumbered. */
	hb_set_t used_fds;
	க்கு (unsigned முழு i = 0; i < இலக்கம்_glyphs; i++)
	{
	  hb_codepoint_t glyph;
	  எனில் (!plan->old_gid_for_new_gid (i, &glyph))
	    தொடர்;
	  மாறிலி hb_ubytes_t str = (*acc.charStrings)[glyph];
	  unsigned முழு fd = CFF::subset_get_fd (acc, glyph);
	  எனில் (unlikely (fd >= acc.fd_மொத்தம்))
	    திரும்பு பொய்;
	  used_fds.add (fd);

	  cff2_cs_interp_env_t<blend_arg_t> env (str, acc, fd);
	  cs_interpreter_t<cff2_cs_interp_env_t<blend_arg_t>, cff2_cs_opset_usage_t, cff2_usage_param_t> interp (env);
	  cff2_usage_param_t param;
	  param.instancing = &instancing;
	  param.drop_hints = drop_hints;
	  எனில் (unlikely (!interp.interpret (param)))
	    திரும்பு பொய்;
	}
	க்கு (unsigned fd : used_fds)
	{
	  cff2_usage_param_t param;
	  param.instancing = &instancing;
	  unsigned மொத்தம் = acc.privateDicts[fd].get_மொத்தம் ();
	  க்கு (unsigned j = 0; j < மொத்தம்; j++)
	  {
	    மாறிலி op_str_t &opstr = acc.privateDicts[fd][j];
	    எனில் (drop_hints && dict_opset_t::is_hint_op (opstr.op))
	      தொடர்;
	    cff2_priv_dict_interp_env_t env {hb_ubytes_t (opstr.ptr, opstr.length)};
	    dict_interpreter_t<cff2_private_dict_usage_opset_t, cff2_usage_param_t, cff2_priv_dict_interp_env_t> interp (env);
	    எனில் (unlikely (!interp.interpret (param)))
	      திரும்பு பொய்;
	  }
	}
	எனில் (unlikely (!instancing.finalize ()))
	  திரும்பு பொய்;

	/* Pass 2: like subr_flattener_t, but blends stay symbolic (no coords
	 * fed to the env) and get rewritten against the instanced store. */
	எனில் (!subset_charstrings.resize_exact (இலக்கம்_glyphs))
	  திரும்பு பொய்;
	க்கு (unsigned முழு i = 0; i < இலக்கம்_glyphs; i++)
	{
	  hb_codepoint_t glyph;
	  எனில் (!plan->old_gid_for_new_gid (i, &glyph))
	    தொடர்;
	  மாறிலி hb_ubytes_t str = (*acc.charStrings)[glyph];
	  unsigned முழு fd = CFF::subset_get_fd (acc, glyph);
	  எனில் (unlikely (fd >= acc.fd_மொத்தம்))
	    திரும்பு பொய்;

	  cff2_cs_interp_env_t<blend_arg_t> env (str, acc, fd);
	  cs_interpreter_t<cff2_cs_interp_env_t<blend_arg_t>, cff2_cs_opset_flatten_t, flatten_param_t> interp (env);
	  flatten_param_t param = {
	    subset_charstrings.arrayZ[i],
	    (தருக்க) (plan->flags & HB_SUBSET_FLAGS_NO_HINTING),
	    plan
	  };
	  param.instancer = &instancing;
	  எனில் (unlikely (!interp.interpret (param)))
	    திரும்பு பொய்;
	}
      }
      இல்லையெனில்
      {
      /* Flatten global & local subrs */
      subr_flattener_t<மாறிலி OT::cff2::accelerator_subset_t, cff2_cs_interp_env_t<blend_arg_t>, cff2_cs_opset_flatten_t>
		    flattener(acc, plan);

      /* Enable command capture if requested (for specialization) */
      எனில் (capture_commands)
      {
	எனில் (!charstring_commands.resize_exact (இலக்கம்_glyphs))
	  திரும்பு பொய்;

	எனில் (!flattener.flatten (subset_charstrings, &charstring_commands))
	  திரும்பு பொய்;
      }
      இல்லையெனில்
      {
	எனில் (!flattener.flatten (subset_charstrings))
	  திரும்பு பொய்;
      }
      }
    }
    இல்லையெனில்
    {
      cff2_subr_subsetter_t	subr_subsetter (acc, plan);

      /* Subset subrs: collect used subroutines, leaving all unused ones behind */
      எனில் (!subr_subsetter.subset ())
	திரும்பு பொய்;

      /* encode charstrings, global subrs, local subrs with new subroutine numbers */
      எனில் (!subr_subsetter.encode_charstrings (subset_charstrings, !pinned))
	திரும்பு பொய்;

      எனில் (!subr_subsetter.encode_globalsubrs (subset_globalsubrs))
	திரும்பு பொய்;

      /* local subrs */
      எனில் (!subset_localsubrs.resize (orig_fdcount))
	திரும்பு பொய்;
      க்கு (unsigned முழு fd = 0; fd < orig_fdcount; fd++)
      {
	subset_localsubrs[fd].init ();
	எனில் (!subr_subsetter.encode_localsubrs (fd, subset_localsubrs[fd]))
	  திரும்பு பொய்;
      }
    }

    /* FDSelect */
    எனில் (acc.fdSelect != &Null (CFF2FDSelect))
    {
      எனில் (unlikely (!hb_plan_subset_cff_fdselect (plan,
						  orig_fdcount,
						  *(மாறிலி FDSelect *)acc.fdSelect,
						  acc.இலக்கம்_glyphs,
						  subset_fdcount,
						  subset_fdselect_size,
						  subset_fdselect_format,
						  subset_fdselect_ranges,
						  fdmap)))
	திரும்பு பொய்;
    }
    இல்லையெனில்
      fdmap.identity (1);

    திரும்பு உண்மை;
  }

  cff2_sub_table_info_t info;

  unsigned முழு    இலக்கம்_glyphs;
  unsigned முழு    orig_fdcount = 0;
  unsigned முழு    subset_fdcount = 1;
  unsigned முழு    subset_fdselect_size = 0;
  unsigned முழு    subset_fdselect_format = 0;
  தருக்க            pinned = பொய்;
  hb_vector_t<code_pair_t>   subset_fdselect_ranges;

  hb_inc_bimap_t   fdmap;

  str_buff_vec_t	    subset_charstrings;
  str_buff_vec_t	    subset_globalsubrs;
  hb_vector_t<str_buff_vec_t> subset_localsubrs;

  தருக்க	    drop_hints = பொய்;
  தருக்க	    desubroutinize = பொய்;

  தருக்க	    partial_instancing = பொய்;
  cff2_instancing_plan_t instancing;
  மாறிலி hb_vector_t<hb_tag_t> *axis_tags = nullptr;

  unsigned  min_charstrings_off_size = 0;

  hb_array_t<முழு> normalized_coords; // For instantiation
  head_maxp_info_t head_maxp_info;  // For FontBBox
  மாறிலி hb_hashmap_t<hb_codepoint_t, hb_pair_t<unsigned, முழு>> *hmtx_map; // For widths

  // Width optimization results (for CFF1 conversion)
  unsigned default_width = 0;
  unsigned nominal_width = 0;

  // Command capture for specialization (CFF2→CFF1 conversion)
  தருக்க capture_commands = பொய்;
  hb_vector_t<hb_vector_t<cs_command_t>> charstring_commands;
};
} // namespace OT

/*
 * CFF2 to CFF1 Converter Implementation
 */

#அந்தர்பாவ "hb-cff-width-optimizer.hh"
#அந்தர்பாவ "hb-cff-specializer.hh"

/* Serialize charstrings using CFF1 format with widths */
நிலையான தருக்க
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
  எனில் (unlikely (!cff1_charstrings.resize (plan.subset_charstrings.length)))
  {
    c->எடு_discard ();
    திரும்பு பொய்;
  }

  க்கு (unsigned i = 0; i < plan.subset_charstrings.length; i++)
  {
    // Get width for this glyph from hmtx_map
    unsigned width = 0;
    எனில் (plan.hmtx_map->has (i))
      width = plan.hmtx_map->get (i).first;

    // Encode width if different from default
    str_encoder_t encoder (cff1_charstrings[i]);
    எனில் (width != default_width)
    {
      முழு delta = (முழு) width - (முழு) nominal_width;
      encoder.encode_int (delta);
    }

    // Use specialized commands if available, otherwise use binary
    எனில் (plan.capture_commands && i < plan.charstring_commands.length &&
        plan.charstring_commands[i].length > 0)
    {
      // Specialize and encode commands
      auto &commands = plan.charstring_commands[i];
      CFF::specialize_commands (commands, 48);  /* maxstack=48 for CFF1 */
      எனில் (unlikely (!CFF::encode_commands (commands, cff1_charstrings[i])))
      {
        c->எடு_discard ();
        திரும்பு பொய்;
      }
    }
    இல்லையெனில்
    {
      // Use binary CharString
      மாறிலி str_buff_t &cs = plan.subset_charstrings[i];
      க்கு (unsigned j = 0; j < cs.length; j++)
        cff1_charstrings[i].push (cs[j]);
    }

    // Check if it already ends with endchar (0x0e) or return (0x0b)
    எனில் (cff1_charstrings[i].length == 0 ||
        (cff1_charstrings[i].tail () != 0x0e && cff1_charstrings[i].tail () != 0x0b))
    {
      // Append endchar operator
      எனில் (unlikely (!cff1_charstrings[i].தள்ளு_or_fail (0x0e)))
      {
        c->எடு_discard ();
        திரும்பு பொய்;
      }
    }
  }

  unsigned data_size = 0;
  unsigned total_size = CFF1CharStrings::total_size (cff1_charstrings, &data_size, plan.min_charstrings_off_size);
  எனில் (unlikely (!c->start_zerocopy (total_size)))
  {
    c->எடு_discard ();
    திரும்பு பொய்;
  }

  auto *cs = c->start_embed<CFF1CharStrings> ();
  எனில் (unlikely (!cs->serialize (c, cff1_charstrings, &data_size, plan.min_charstrings_off_size)))
  {
    c->எடு_discard ();
    திரும்பு பொய்;
  }

  plan.info.char_strings_link = c->எடு_pack (பொய்);
  திரும்பு உண்மை;
}

/* Serialize CID Charset (format 2 range: gid 0-N -> cid 0-N) */
நிலையான தருக்க
_serialize_cff1_charset (hb_serialize_context_t *c,
                         unsigned முழு இலக்கம்_glyphs,
                         objidx_t &charset_link)
{
  // For CID fonts, create a simple identity charset
  // Format 2: one range covering all glyphs (except .notdef)
  c->push ();

  auto *charset = c->start_embed<Charset> ();
  எனில் (unlikely (!charset))
  {
    c->எடு_discard ();
    திரும்பு பொய்;
  }

  // Create a single range for CID 1 to num_glyphs-1
  hb_vector_t<code_pair_t> ranges;
  எனில் (இலக்கம்_glyphs > 1)
  {
    code_pair_t range;
    range.code = 1;  // first CID
    range.glyph = இலக்கம்_glyphs - 2;  // nLeft (covers glyphs 1 to num_glyphs-1)
    ranges.push (range);
  }

  எனில் (unlikely (!charset->serialize (c, 2, இலக்கம்_glyphs, ranges)))
  {
    c->எடு_discard ();
    திரும்பு பொய்;
  }

  charset_link = c->எடு_pack ();
  திரும்பு உண்மை;
}

/* CFF2 to CFF1 serialization */
நாமாகாஶ CFF {

தருக்க
serialize_cff2_to_cff1 (hb_serialize_context_t *c,
                        OT::cff2_subset_plan &plan,
                        மாறிலி cff2_உச்சி_dict_மதிப்புகள்_t &cff2_உச்சி_Dict,
                        மாறிலி OT::cff2::accelerator_subset_t &acc)
{
  TRACE_SERIALIZE (இது);

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
    widths.alloc (plan.இலக்கம்_glyphs);

    க்கு (unsigned gid = 0; gid < plan.இலக்கம்_glyphs; gid++)
    {
      unsigned width = 0;
      எனில் (plan.hmtx_map->has (gid))
        width = plan.hmtx_map->get (gid).first;
      widths.push (width);
    }

    // Optimize defaultWidthX and nominalWidthX
    CFF::optimize_widths (widths, plan.default_width, plan.nominal_width);
  }

  // 1. CharStrings (with widths prepended)
  எனில் (!_serialize_cff1_charstrings (c, plan, plan.default_width, plan.nominal_width))
    return_trace (பொய்);

  // 2. Private DICs & Local Subrs (same as CFF2)
  hb_vector_t<table_info_t> private_dict_infos;
  எனில் (unlikely (!private_dict_infos.resize (plan.subset_fdcount)))
    return_trace (பொய்);

  க்கு (முழு i = (முழு)acc.privateDicts.length; --i >= 0;)
  {
    எனில் (plan.fdmap.has (i))
    {
      objidx_t subrs_link = 0;

      எனில் (plan.subset_localsubrs[i].length > 0)
      {
        auto *dest = c->push<CFF1Subrs> ();
        எனில் (likely (dest->serialize (c, plan.subset_localsubrs[i])))
          subrs_link = c->எடு_pack (பொய்);
        இல்லையெனில்
        {
          c->எடு_discard ();
          return_trace (பொய்);
        }
      }

      auto *pd = c->push<PrivateDict> ();
      // Use the CFF2 Private DICT serializer which instantiates blends when pinned=true
      cff2_private_dict_op_serializer_t privSzr (plan.desubroutinize, plan.drop_hints, plan.pinned,
                                                  acc.varStore, plan.normalized_coords);
      எனில் (likely (pd->serialize (c, acc.privateDicts[i], privSzr, subrs_link)))
      {
        // Add defaultWidthX and nominalWidthX for CFF1
        str_buff_t width_ops;
        str_encoder_t encoder (width_ops);
        encoder.encode_int (plan.default_width);
        encoder.encode_op (OpCode_defaultWidthX);
        encoder.encode_int (plan.nominal_width);
        encoder.encode_op (OpCode_nominalWidthX);

        எனில் (!encoder.in_error () && c->embed (width_ops.as_bytes ().arrayZ, width_ops.length))
        {
          unsigned fd = plan.fdmap[i];
          private_dict_infos[fd].size = c->length ();
          private_dict_infos[fd].link = c->எடு_pack ();
        }
        இல்லையெனில்
        {
          c->எடு_discard ();
          return_trace (பொய்);
        }
      }
      இல்லையெனில்
      {
        c->எடு_discard ();
        return_trace (பொய்);
      }
    }
  }

  // 3. FDArray - serialize CFF2 font dicts as CFF1
  {
    auto *fda = c->push<FDArray<HBUINT16>> ();
    cff_font_dict_op_serializer_t fontSzr;
    auto it =
    + hb_zip (+ hb_iter (acc.fontDicts)
              | hb_filter ([&] (மாறிலி cff2_font_dict_மதிப்புகள்_t &_)
                { திரும்பு plan.fdmap.has (&_ - &acc.fontDicts[0]); }),
              hb_iter (private_dict_infos))
    ;
    // Explicitly specify template parameters: DICTVAL, INFO
    தருக்க success = fda->serialize<cff2_font_dict_மதிப்புகள்_t, table_info_t> (c, it, fontSzr);
    எனில் (success)
      plan.info.fd_array_link = c->எடு_pack (பொய்);
    இல்லையெனில்
    {
      c->எடு_discard ();
      return_trace (பொய்);
    }
  }

  // 4. FDSelect (required in CFF1 CID-keyed fonts)
  // CFF1 requires FDSelect for all CID-keyed fonts, even with just one FD
  // CFF2 makes it optional when there's only one FD
  எனில் (acc.fdSelect != &Null (CFF2FDSelect))
  {
    c->push ();
    எனில் (likely (hb_serialize_cff_fdselect (c, plan.இலக்கம்_glyphs,
                                          *(மாறிலி FDSelect *)acc.fdSelect,
                                          plan.orig_fdcount,
                                          plan.subset_fdselect_format,
                                          plan.subset_fdselect_size,
                                          plan.subset_fdselect_ranges)))
      plan.info.fd_select.link = c->எடு_pack ();
    இல்லையெனில்
    {
      c->எடு_discard ();
      return_trace (பொய்);
    }
  }
  இல்லையெனில்
  {
    // Create a range-based FDSelect3 mapping all glyphs to FD 0
    // Format: format(1) + nRanges(2) + range(3) + sentinel(2) = 8 bytes
    c->push ();

    // Format byte
    HBUINT8 format;
    format = 3;
    எனில் (unlikely (!c->embed (format)))
    {
      c->எடு_discard ();
      return_trace (பொய்);
    }

    // nRanges
    HBUINT16 nRanges;
    nRanges = 1;
    எனில் (unlikely (!c->embed (nRanges)))
    {
      c->எடு_discard ();
      return_trace (பொய்);
    }

    // Single range: {first: 0, fd: 0}
    FDSelect3_Range range;
    range.first = 0;
    range.fd = 0;
    எனில் (unlikely (!c->embed (range)))
    {
      c->எடு_discard ();
      return_trace (பொய்);
    }

    // Sentinel (number of glyphs)
    HBUINT16 sentinel;
    sentinel = plan.இலக்கம்_glyphs;
    எனில் (unlikely (!c->embed (sentinel)))
    {
      c->எடு_discard ();
      return_trace (பொய்);
    }

    plan.info.fd_select.link = c->எடு_pack ();
  }

  // 5. Charset (CID charset for identity mapping)
  objidx_t charset_link;
  எனில் (!_serialize_cff1_charset (c, plan.இலக்கம்_glyphs, charset_link))
    return_trace (பொய்);

  // 6. Global Subrs
  {
    auto *dest = c->push<CFF1Subrs> ();
    எனில் (likely (dest->serialize (c, plan.subset_globalsubrs)))
      c->எடு_pack (பொய்);
    இல்லையெனில்
    {
      c->எடு_discard ();
      return_trace (பொய்);
    }
  }

  // 7. String INDEX - Add "Adobe" and "Identity" for ROS operator
  {
    மாறிலி எழுத்து *adobe_str = "Adobe";
    மாறிலி எழுத்து *identity_str = "Identity";
    unsigned adobe_len = 5;  // strlen("Adobe")
    unsigned identity_len = 8;  // strlen("Identity")

    // Build strings array
    hb_vector_t<hb_ubytes_t> strings;
    strings.alloc (2);
    strings.push (hb_ubytes_t ((மாறிலி unsigned எழுத்து *) adobe_str, adobe_len));
    strings.push (hb_ubytes_t ((மாறிலி unsigned எழுத்து *) identity_str, identity_len));

    // Serialize as CFF INDEX
    auto *dest = c->push<CFF1Index> ();
    எனில் (likely (dest->serialize (c, strings)))
      c->எடு_pack (பொய்);
    இல்லையெனில்
    {
      c->எடு_discard ();
      return_trace (பொய்);
    }
  }

  // 8. CFF Header
  OT::cff1 *cff = c->allocate_min<OT::cff1> ();
  எனில் (unlikely (!cff)) return_trace (பொய்);

  /* header */
  cff->version.major = 0x01;
  cff->version.minor = 0x00;
  cff->nameIndex = cff->min_size;
  cff->offSize = 4; /* unused? */

  // 9. Name INDEX (single entry)
  {
    unsigned name_len = strlen (CFF1_DEFAULT_FONT_NAME);

    CFF1Index *idx = c->start_embed<CFF1Index> ();
    எனில் (unlikely (!idx)) return_trace (பொய்);

    எனில் (unlikely (!idx->serialize_header (c, hb_iter (&name_len, 1), name_len)))
      return_trace (பொய்);

    எனில் (unlikely (!c->embed (CFF1_DEFAULT_FONT_NAME, name_len)))
      return_trace (பொய்);
  }

  // 10. Top DICT INDEX
  {
    // Serialize the Top DICT data first
    c->push<உச்சி_Dict> ();
    cff1_from_cff2_உச்சி_dict_op_serializer_t உச்சி_Szr;

    // Serialize ROS first
    எனில் (unlikely (!உச்சி_Szr.serialize_ros (c)))
    {
      c->எடு_discard ();
      return_trace (பொய்);
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

      எனில் (encoder.in_error () || !c->embed (bbox_buff.as_bytes ().arrayZ, bbox_buff.length))
      {
        c->எடு_discard ();
        return_trace (பொய்);
      }
    }

    // Serialize charset operator
    எனில் (charset_link && unlikely (!FontDict::serialize_link4_op (c, OpCode_charset, charset_link, whence_t::Absolute)))
    {
      c->எடு_discard ();
      return_trace (பொய்);
    }

    // Serialize FDSelect operator (required for CID-keyed CFF1 fonts)
    எனில் (plan.info.fd_select.link && unlikely (!FontDict::serialize_link4_op (c, OpCode_FDSelect, plan.info.fd_select.link, whence_t::Absolute)))
    {
      c->எடு_discard ();
      return_trace (பொய்);
    }

    // Serialize FDArray operator (required for CID-keyed CFF1 fonts)
    எனில் (plan.info.fd_array_link && unlikely (!FontDict::serialize_link4_op (c, OpCode_FDArray, plan.info.fd_array_link, whence_t::Absolute)))
    {
      c->எடு_discard ();
      return_trace (பொய்);
    }

    // Serialize other operators from CFF2 TopDict
    க்கு (மாறிலி auto &opstr : cff2_உச்சி_Dict.values)
    {
      எனில் (unlikely (!உச்சி_Szr.serialize (c, opstr, plan.info)))
      {
        c->எடு_discard ();
        return_trace (பொய்);
      }
    }

    unsigned உச்சி_size = c->length ();
    c->எடு_pack (பொய்);

    // Serialize INDEX header
    auto *dest = c->start_embed<CFF1Index> ();
    எனில் (unlikely (!dest->serialize_header (c, hb_iter (&உச்சி_size, 1), உச்சி_size)))
      return_trace (பொய்);
  }

  return_trace (உண்மை);
}

} /* namespace CFF */

நிலையான தருக்க _serialize_cff2_charstrings (hb_serialize_context_t *c,
			     cff2_subset_plan &plan,
			     மாறிலி OT::cff2::accelerator_subset_t  &acc)
{
  c->push ();

  unsigned data_size = 0;
  unsigned total_size = CFF2CharStrings::total_size (plan.subset_charstrings, &data_size, plan.min_charstrings_off_size);
  எனில் (unlikely (!c->start_zerocopy (total_size)))
    திரும்பு பொய்;

  auto *cs = c->start_embed<CFF2CharStrings> ();
  எனில் (unlikely (!cs->serialize (c, plan.subset_charstrings, &data_size, plan.min_charstrings_off_size)))
  {
    c->எடு_discard ();
    திரும்பு பொய்;
  }

  plan.info.char_strings_link = c->எடு_pack (பொய்);
  திரும்பு உண்மை;
}

தருக்க
OT::cff2::accelerator_subset_t::serialize (hb_serialize_context_t *c,
					   கட்டமைப்பு cff2_subset_plan &plan,
					   hb_array_t<முழு> normalized_coords) மாறிலி
{
  /* push charstrings onto the object stack first which will ensure it packs as the last
     object in the table. Keeping the chastrings last satisfies the requirements for patching
     via IFTB. If this ordering needs to be changed in the future, charstrings should be left
     at the end whenever HB_SUBSET_FLAGS_ITFB_REQUIREMENTS is enabled. */
  எனில் (!_serialize_cff2_charstrings(c, plan, *இது))
    திரும்பு பொய்;

  /* private dicts & local subrs */
  hb_vector_t<table_info_t>  private_dict_infos;
  எனில் (unlikely (!private_dict_infos.resize (plan.subset_fdcount))) திரும்பு பொய்;

  க்கு (முழு i = (முழு)privateDicts.length; --i >= 0 ;)
  {
    எனில் (plan.fdmap.has (i))
    {
      objidx_t	subrs_link = 0;

      எனில் (plan.subset_localsubrs[i].length > 0)
      {
	auto *dest = c->push <CFF2Subrs> ();
	எனில் (likely (dest->serialize (c, plan.subset_localsubrs[i])))
	  subrs_link = c->எடு_pack (பொய்);
	இல்லையெனில்
	{
	  c->எடு_discard ();
	  திரும்பு பொய்;
	}
      }
      auto *pd = c->push<PrivateDict> ();
      cff2_private_dict_op_serializer_t privSzr (plan.desubroutinize, plan.drop_hints, plan.pinned,
						 varStore, normalized_coords,
						 plan.partial_instancing ? &plan.instancing : nullptr);
      எனில் (likely (pd->serialize (c, privateDicts[i], privSzr, subrs_link)))
      {
	unsigned fd = plan.fdmap[i];
	private_dict_infos[fd].size = c->length ();
	private_dict_infos[fd].link = c->எடு_pack ();
      }
      இல்லையெனில்
      {
	c->எடு_discard ();
	திரும்பு பொய்;
      }
    }
  }

  /* FDSelect */
  எனில் (fdSelect != &Null (CFF2FDSelect))
  {
    c->push ();
    எனில் (likely (hb_serialize_cff_fdselect (c, plan.இலக்கம்_glyphs, *(மாறிலி FDSelect *)fdSelect,
					   plan.orig_fdcount,
					   plan.subset_fdselect_format, plan.subset_fdselect_size,
					   plan.subset_fdselect_ranges)))
      plan.info.fd_select.link = c->எடு_pack ();
    இல்லையெனில்
    {
      c->எடு_discard ();
      திரும்பு பொய்;
    }
  }

  /* FDArray (FD Index) */
  {
    auto *fda = c->push<CFF2FDArray> ();
    cff_font_dict_op_serializer_t fontSzr;
    auto it =
    + hb_zip (+ hb_iter (fontDicts)
	      | hb_filter ([&] (மாறிலி cff2_font_dict_மதிப்புகள்_t &_)
		{ திரும்பு plan.fdmap.has (&_ - &fontDicts[0]); }),
	      hb_iter (private_dict_infos))
    ;
    எனில் (unlikely (!fda->serialize (c, it, fontSzr)))
    {
      c->எடு_discard ();
      திரும்பு பொய்;
    }
    plan.info.fd_array_link = c->எடு_pack (பொய்);
  }

  /* variation store */
  எனில் (varStore != &Null (CFF2ItemVariationStore) &&
      !plan.pinned)
  {
    எனில் (plan.partial_instancing)
    {
      எனில் (plan.instancing.has_variations ())
      {
	c->push ();
	எனில் (unlikely (!plan.instancing.serialize_var_store (c, *plan.axis_tags)))
	{
	  c->எடு_discard ();
	  திரும்பு பொய்;
	}
	plan.info.var_store_link = c->எடு_pack (பொய்);
      }
      /* else: the store emptied; the vstore op gets dropped along with it. */
    }
    இல்லையெனில்
    {
      auto *dest = c->push<CFF2ItemVariationStore> ();
      எனில் (unlikely (!dest->serialize (c, varStore)))
      {
	c->எடு_discard ();
	திரும்பு பொய்;
      }
      plan.info.var_store_link = c->எடு_pack (பொய்);
    }
  }

  OT::cff2 *cff2 = c->allocate_min<OT::cff2> ();
  எனில் (unlikely (!cff2)) திரும்பு பொய்;

  /* header */
  cff2->version.major = 0x02;
  cff2->version.minor = 0x00;
  cff2->உச்சி_Dict_2 = OT::cff2::static_size;

  /* top dict */
  {
    உச்சி_Dict &dict = cff2 + cff2->உச்சி_Dict_2;
    cff2_உச்சி_dict_op_serializer_t உச்சி_Szr;
    எனில் (unlikely (!dict.serialize (c, உச்சி_Dict_2, உச்சி_Szr, plan.info))) திரும்பு பொய்;
    cff2->உச்சி_Dict_Size = c->head - (மாறிலி எழுத்து *)&dict;
  }

  /* global subrs */
  {
    auto *dest = c->start_embed <CFF2Subrs> ();
    திரும்பு dest->serialize (c, plan.subset_globalsubrs);
  }
}

தருக்க
OT::cff2::accelerator_subset_t::subset (hb_subset_context_t *c) மாறிலி
{
  cff2_subset_plan cff2_plan;

  எனில் (unlikely (!cff2_plan.create (*இது, c->plan))) திரும்பு பொய்;

  // If instantiating (pinned) and downgrade flag is set, convert to CFF1
  எனில் (cff2_plan.pinned && (c->plan->flags & HB_SUBSET_FLAGS_DOWNGRADE_CFF2))
  {
    // Serialize CFF1 to the subsetter's serializer
    // If we run out of room, returning true will cause subsetter to retry with larger buffer
    தருக்க result = CFF::serialize_cff2_to_cff1 (c->serializer, cff2_plan, உச்சி_Dict_2, *இது);

    எனில் (c->serializer->ran_out_of_room ())
      திரும்பு உண்மை; // Subsetter will retry with larger buffer

    எனில் (result && !c->serializer->in_error ())
    {
      // Success - end serialization to resolve links
      c->serializer->end_serialize ();

      // Copy the serialized CFF1 data and add as CFF table
      hb_blob_t *cff_blob = c->serializer->copy_blob ();
      எனில் (cff_blob)
      {
        c->plan->add_table (HB_TAG('C','F','F',' '), cff_blob);
        hb_blob_destroy (cff_blob);

        // Return false to signal CFF2 table is not needed
        திரும்பு பொய்;
      }
    }

    // Conversion failed - don't fall back, fail hard for debugging
    திரும்பு பொய்;
  }

  திரும்பு serialize (c->serializer, cff2_plan,
		    c->plan->normalized_coords.as_array ());
}

#endif
