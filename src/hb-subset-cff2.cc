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

#અન્તર્ભાવ "hb.hh"

#ifndef HB_NO_SUBSET_CFF

#અન્તર્ભાવ "hb-open-type.hh"
#અન્તર્ભાવ "hb-ot-cff2-table.hh"
#અન્તર્ભાવ "hb-ot-var-common.hh"
#અન્તર્ભાવ "hb-set.h"
#અન્તર્ભાવ "hb-subset-plan.hh"
#અન્તર્ભાવ "hb-subset-cff-common.hh"
#અન્તર્ભાવ "hb-cff2-interp-cs.hh"
#અન્તર્ભાવ "hb-subset-cff2-to-cff1.hh"

using નામાકાશ CFF;

નામાકાશ CFF {

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
સંરચના cff2_instancing_plan_t
{
  સંરચના vardata_transform_t
  {
    /* new_deltas[j] = Σ_i matrix[j][i] * old_deltas[i] */
    hb_vector_t<hb_vector_t<દશાંશ>> matrix;
    /* Default-value fold-in scalars, one per old region: the old store's
     * region scalars at the new default location. */
    hb_vector_t<દશાંશ> gains;
    /* Surviving regions, parallel to matrix rows. */
    hb_vector_t<hb_hashmap_t<hb_tag_t, Triple>> regions;
    /* Index into the new global region list, per new region (finalize). */
    hb_vector_t<unsigned> region_indices;
    unsigned new_major = 0;
    તાર્કિક alive_ = ખોટું;
    તાર્કિક alive () અચલ { પાછા alive_; }

    /* One value's delta for new region j, from its old per-region deltas. */
    દ્વિગુણ transform_delta (unsigned j, hb_array_t<અચલ સંખ્યા_t> deltas) અચલ
    {
      અચલ hb_vector_t<દશાંશ> &col = matrix.arrayZ[j];
      દ્વિગુણ d = 0.;
      unsigned ગણતરી = hb_min (col.length, deltas.length);
      માટે (unsigned c = 0; c < ગણતરી; c++)
	d += (દ્વિગુણ) col.arrayZ[c] * deltas.arrayZ[c].to_real ();
      પાછા d;
    }
  };

  hb_vector_t<vardata_transform_t> transforms;  /* per old VarData (vsindex) */
  hb_vector_t<hb_hashmap_t<hb_tag_t, Triple>> new_regions;  /* global region list */
  hb_set_t used_majors;
  unsigned new_major_ગણતરી = 0;

  તાર્કિક has_variations () અચલ { પાછા new_major_ગણતરી; }

  અચલ vardata_transform_t *get_transform (unsigned major) અચલ
  { પાછા major < transforms.length ? &transforms.arrayZ[major] : nullptr; }

  /* Record that a retained blend references this VarData. */
  શૂન્ય note_use (unsigned major) { used_majors.add (major); }

  /* The new vsindex a private dict's (or charstring's) old ivs maps to;
   * dead VarDatas fall back to 0, their blends having become static. */
  unsigned remap_ivs (unsigned old_ivs) અચલ
  {
    અચલ vardata_transform_t *t = get_transform (old_ivs);
    પાછા (t && t->alive ()) ? t->new_major : 0;
  }

  દ્વિગુણ fold_deltas (unsigned old_ivs, hb_array_t<અચલ સંખ્યા_t> deltas) અચલ
  {
    અચલ vardata_transform_t *t = get_transform (old_ivs);
    જો (unlikely (!t || t->gains.length != deltas.length)) પાછા 0.;
    દ્વિગુણ v = 0.;
    માટે (unsigned i = 0; i < deltas.length; i++)
      v += (દ્વિગુણ) t->gains.arrayZ[i] * deltas.arrayZ[i].to_real ();
    પાછા v;
  }

  તાર્કિક create (અચલ CFF2ItemVariationStore *cff2VarStore,
	       અચલ hb_subset_plan_t *plan)
  {
    અચલ OT::ItemVariationStore &varStore = cff2VarStore->varStore;
    unsigned major_ગણતરી = varStore.get_sub_table_ગણતરી ();
    જો (!transforms.resize (major_ગણતરી)) પાછા ખોટું;

    જો (!varStore.get_region_list ().get_var_regions (plan->axes_old_index_tag_map, orig_regions))
      પાછા ખોટું;

    /* Instance each VarData with identity delta rows; the surviving tuples'
     * delta columns are the old-region → new-region transform. */
    OT::optimize_scratch_t scratch;
    માટે (unsigned m = 0; m < major_ગણતરી; m++)
    {
      અચલ OT::VarData &var_data = varStore.get_sub_table (m);
      unsigned k = var_data.get_region_index_ગણતરી ();
      vardata_transform_t &t = transforms.arrayZ[m];

      જો (!t.gains.resize_exact (k)) પાછા ખોટું;
      varStore.get_region_scalars (m, plan->normalized_coords.arrayZ,
				   plan->normalized_coords.length,
				   t.gains.arrayZ, k);

      OT::tuple_variations_t tuples;
      માટે (unsigned r = 0; r < k; r++)
      {
	OT::tuple_delta_t tuple;
	જો (!tuple.deltas_x.resize (k) ||
	    !tuple.indices.resize (k))
	  પાછા ખોટું;
	માટે (unsigned i = 0; i < k; i++)
	  tuple.indices.arrayZ[i] = સાચું;
	tuple.deltas_x.arrayZ[r] = 1.f;
	unsigned region_index = var_data.get_region_index (r);
	જો (unlikely (region_index >= orig_regions.length)) પાછા ખોટું;
	tuple.axis_tuples = orig_regions.arrayZ[region_index];
	જો (unlikely (tuple.axis_tuples.in_error ())) પાછા ખોટું;
	tuples.tuple_vars.push (std::move (tuple));
      }
      જો (unlikely (tuples.tuple_vars.in_error ())) પાછા ખોટું;

      જો (!tuples.instantiate (plan->axes_location, plan->axes_triple_distances, scratch))
	પાછા ખોટું;

      માટે (auto &tuple : tuples.tuple_vars)
      {
	જો (unlikely (tuple.deltas_x.length != k)) પાછા ખોટું;
	hb_vector_t<દશાંશ> col;
	જો (unlikely (!col.resize_exact (k))) પાછા ખોટું;
	માટે (unsigned i = 0; i < k; i++)
	  col.arrayZ[i] = tuple.deltas_x.arrayZ[i];
	t.matrix.push (std::move (col));
	t.regions.push (std::move (tuple.axis_tuples));
      }
      જો (unlikely (t.matrix.in_error () || t.regions.in_error ())) પાછા ખોટું;
    }

    પાછા સાચું;
  }

  /* Decide which VarDatas survive, number them, and build the new global
   * region list: surviving original regions first, in their original order,
   * then solver-split new regions in order of appearance (fontTools'
   * rebuildRegions + prune_regions). */
  તાર્કિક finalize ()
  {
    માટે (unsigned m = 0; m < transforms.length; m++)
    {
      vardata_transform_t &t = transforms.arrayZ[m];
      t.alive_ = t.matrix.length && used_majors.has (m);
      જો (t.alive_)
	t.new_major = new_major_ગણતરી++;
    }

    /* Keys hash by content, so pointers into different owners compare fine. */
    hb_hashmap_t<અચલ hb_hashmap_t<hb_tag_t, Triple>*, unsigned> region_idx_map;
    hb_vector_t<અચલ hb_hashmap_t<hb_tag_t, Triple>*> region_ptrs;
    માટે (અચલ vardata_transform_t &t : transforms)
    {
      જો (!t.alive ()) ચાલુ;
      માટે (અચલ auto &region : t.regions)
	જો (!region_idx_map.has (&region))
	  જો (unlikely (!region_idx_map.set (&region, (unsigned) -1)))
	    પાછા ખોટું;
    }
    માટે (અચલ auto &r : orig_regions)
    {
      unsigned *idx;
      જો (region_idx_map.has (&r, &idx) && *idx == (unsigned) -1)
      {
	જો (unlikely (!region_idx_map.set (&r, region_ptrs.length))) પાછા ખોટું;
	region_ptrs.push (&r);
      }
    }
    માટે (અચલ vardata_transform_t &t : transforms)
    {
      જો (!t.alive ()) ચાલુ;
      માટે (અચલ auto &region : t.regions)
      {
	unsigned *idx;
	જો (region_idx_map.has (&region, &idx) && *idx == (unsigned) -1)
	{
	  જો (unlikely (!region_idx_map.set (&region, region_ptrs.length))) પાછા ખોટું;
	  region_ptrs.push (&region);
	}
      }
    }
    જો (unlikely (region_ptrs.in_error ())) પાછા ખોટું;

    માટે (vardata_transform_t &t : transforms)
    {
      જો (!t.alive ()) ચાલુ;
      માટે (અચલ auto &region : t.regions)
      {
	unsigned *idx;
	જો (unlikely (!region_idx_map.has (&region, &idx))) પાછા ખોટું;
	t.region_indices.push (*idx);
      }
      જો (unlikely (t.region_indices.in_error ())) પાછા ખોટું;
    }

    જો (!new_regions.resize (region_ptrs.length)) પાછા ખોટું;
    માટે (unsigned i = 0; i < region_ptrs.length; i++)
    {
      new_regions.arrayZ[i] = *region_ptrs.arrayZ[i];
      જો (unlikely (new_regions.arrayZ[i].in_error ())) પાછા ખોટું;
    }

    પાછા સાચું;
  }

  /* Serialize the instanced store: a CFF2ItemVariationStore whose VarDatas
   * carry no items, just region indexes (blend deltas live in charstrings).
   * Laid out manually since all sizes are known up front. */
  તાર્કિક serialize_var_store (hb_serialize_context_t *c,
			    અચલ hb_vector_t<hb_tag_t> &axis_tags) અચલ
  {
    unsigned data_ગણતરી = new_major_ગણતરી;
    unsigned header_size = 2 + 4 + 2 + 4 * data_ગણતરી;

    uint64_t offset = header_size;
    hb_vector_t<unsigned> data_offsets;
    માટે (અચલ vardata_transform_t &t : transforms)
    {
      જો (!t.alive ()) ચાલુ;
      data_offsets.push ((unsigned) offset);
      offset += 6 + 2 * (uint64_t) t.region_indices.length;
    }
    જો (unlikely (data_offsets.in_error () || data_offsets.length != data_ગણતરી))
      પાછા ખોટું;
    unsigned regions_offset = (unsigned) offset;
    uint64_t total = offset + 4 + (uint64_t) new_regions.length * axis_tags.length * 6;
    જો (unlikely (total > 0xFFFFFFFFu)) પાછા ખોટું;

    HBUINT16 *size_field = c->allocate_size<HBUINT16> (HBUINT16::static_size);
    જો (unlikely (!size_field)) પાછા ખોટું;
    /* OFF 2026 5.3.3.11.2 permits larger stores with length=65535. */
    *size_field = hb_min (total, (uint64_t) 0xFFFFu);

    અક્ષર *header = c->allocate_size<અક્ષર> (header_size);
    જો (unlikely (!header)) પાછા ખોટું;
    * (HBUINT16 *) (header + 0) = 1;			/* format */
    * (HBUINT32 *) (header + 2) = regions_offset;
    * (HBUINT16 *) (header + 6) = data_ગણતરી;
    માટે (unsigned i = 0; i < data_ગણતરી; i++)
      * (HBUINT32 *) (header + 8 + 4 * i) = data_offsets.arrayZ[i];

    માટે (અચલ vardata_transform_t &t : transforms)
    {
      જો (!t.alive ()) ચાલુ;
      unsigned k = t.region_indices.length;
      HBUINT16 *v = (HBUINT16 *) c->allocate_size<અક્ષર> (6 + 2 * k);
      જો (unlikely (!v)) પાછા ખોટું;
      v[0] = 0;						/* itemCount */
      v[1] = 0;						/* wordSizeCount */
      v[2] = k;						/* regionIndices.len */
      માટે (unsigned j = 0; j < k; j++)
	v[3 + j] = t.region_indices.arrayZ[j];
    }

    hb_vector_t<અચલ hb_hashmap_t<hb_tag_t, Triple>*> region_ptrs;
    જો (unlikely (!region_ptrs.alloc_exact (new_regions.length))) પાછા ખોટું;
    માટે (અચલ auto &r : new_regions)
      region_ptrs.push (&r);
    auto *region_list = c->start_embed<OT::VarRegionList> ();
    પાછા region_list->serialize (c, axis_tags, region_ptrs);
  }

  ખાનગી:
  hb_vector_t<hb_hashmap_t<hb_tag_t, Triple>> orig_regions;
};

/* Usage-collection pass for partial instancing: record which VarDatas the
 * retained charstrings' and private dicts' blends reference.  Mirrors the
 * emission pass: blends consumed by dropped hint ops don't count. */
સંરચના cff2_usage_param_t
{
  શૂન્ય init () {}

  cff2_instancing_plan_t *instancing = nullptr;
  તાર્કિક drop_hints = ખોટું;
  unsigned ivs = 0;	/* dict-side current vsindex */
};

સંરચના cff2_cs_opset_usage_t : cff2_cs_opset_t<cff2_cs_opset_usage_t, cff2_usage_param_t, blend_arg_t>
{
  સ્થિર શૂન્ય flush_args_and_op (op_code_t op, cff2_cs_interp_env_t<blend_arg_t> &env, cff2_usage_param_t& param)
  {
    તાર્કિક counts = સાચું;
    પસંદગી (op)
    {
      સ્થિતિ OpCode_return:
      સ્થિતિ OpCode_endchar:
	counts = ખોટું;
	તોડો;

      સ્થિતિ OpCode_hstem:
      સ્થિતિ OpCode_hstemhm:
      સ્થિતિ OpCode_vstem:
      સ્થિતિ OpCode_vstemhm:
      સ્થિતિ OpCode_hintmask:
      સ્થિતિ OpCode_cntrmask:
	જો (param.drop_hints)
	  counts = ખોટું;
	તોડો;

      મૂળભૂત:
	તોડો;
    }

    જો (counts)
      માટે (unsigned પૂર્ણાંક i = 0; i < env.arg_ઢગલો.get_ગણતરી (); i++)
	જો (env.arg_ઢગલો[i].blending ())
	{
	  param.instancing->note_use (env.get_ivs ());
	  તોડો;
	}

    env.clear_args ();
  }
};

સંરચના cff2_private_dict_usage_opset_t : dict_opset_t
{
  સ્થિર શૂન્ય process_op (op_code_t op, cff2_priv_dict_interp_env_t& env, cff2_usage_param_t& param)
  {
    પસંદગી (op) {
      સ્થિતિ OpCode_vsindexdict:
	env.process_vsindex ();
	param.ivs = env.get_ivs ();
	env.clear_args ();
	પાછા;
      સ્થિતિ OpCode_blenddict:
	param.instancing->note_use (param.ivs);
	env.clear_args ();
	પાછા;
      મૂળભૂત:
	dict_opset_t::process_op (op, env);
	જો (!env.arg_ઢગલો.is_empty ()) પાછા;
	env.clear_args ();
	પાછા;
    }
  }
};

} /* namespace CFF */

સંરચના cff2_sub_table_info_t : cff_sub_table_info_t
{
  cff2_sub_table_info_t ()
    : cff_sub_table_info_t (),
      var_store_link (0)
  {}

  objidx_t  var_store_link;
};

સંરચના cff2_ટોચ_dict_op_serializer_t : cff_ટોચ_dict_op_serializer_t<>
{
  તાર્કિક serialize (hb_serialize_context_t *c,
		  અચલ op_str_t &opstr,
		  અચલ cff2_sub_table_info_t &info) અચલ
  {
    TRACE_SERIALIZE (આ);

    પસંદગી (opstr.op)
    {
      સ્થિતિ OpCode_vstore:
        જો (info.var_store_link)
	  return_trace (FontDict::serialize_link4_op(c, opstr.op, info.var_store_link));
	નહીંતર
	  return_trace (સાચું);

      મૂળભૂત:
	return_trace (cff_ટોચ_dict_op_serializer_t<>::serialize (c, opstr, info));
    }
  }
};

સંરચના cff2_cs_opset_flatten_t : cff2_cs_opset_t<cff2_cs_opset_flatten_t, flatten_param_t, blend_arg_t>
{
  સ્થિર શૂન્ય flush_args_and_op (op_code_t op, cff2_cs_interp_env_t<blend_arg_t> &env, flatten_param_t& param)
  {
    /* Optionally capture command for specialization (before flushing, to preserve args) */
    જો (param.commands)
    {
      તાર્કિક skip_command = ખોટું;

      પસંદગી (op)
      {
	સ્થિતિ OpCode_return:
	સ્થિતિ OpCode_endchar:
	  skip_command = સાચું;
	  તોડો;

	સ્થિતિ OpCode_hstem:
	સ્થિતિ OpCode_hstemhm:
	સ્થિતિ OpCode_vstem:
	સ્થિતિ OpCode_vstemhm:
	સ્થિતિ OpCode_hintmask:
	સ્થિતિ OpCode_cntrmask:
	  જો (param.drop_hints)
	    skip_command = સાચું;
	  તોડો;

	મૂળભૂત:
	  તોડો;
      }

      જો (!skip_command)
      {
	cs_command_t cmd (op);
	/* Capture resolved blend values */
	માટે (unsigned પૂર્ણાંક i = 0; i < env.arg_ઢગલો.get_ગણતરી ();)
	{
	  અચલ blend_arg_t &arg = env.arg_ઢગલો[i];
	  જો (arg.blending ())
	  {
	    /* For blend args, capture only the resolved default value */
	    cmd.args.push (arg);
	    /* Skip over the multiple blend values */
	    i += arg.અંક_કિંમતો_2;
	  }
	  નહીંતર
	  {
	    cmd.args.push (arg);
	    i++;
	  }
	}
	param.commands->push (cmd);
      }
    }

    પસંદગી (op)
    {
      સ્થિતિ OpCode_return:
      સ્થિતિ OpCode_endchar:
	/* dummy opcodes in CFF2. ignore */
	જો (op == OpCode_endchar && param.instancer)
	  emit_vsindex_prefix (env, param);
	તોડો;

      સ્થિતિ OpCode_hstem:
      સ્થિતિ OpCode_hstemhm:
      સ્થિતિ OpCode_vstem:
      સ્થિતિ OpCode_vstemhm:
      સ્થિતિ OpCode_hintmask:
      સ્થિતિ OpCode_cntrmask:
	જો (param.drop_hints)
	{
	  env.clear_args ();
	  પાછા;
	}
	HB_FALLTHROUGH;

      મૂળભૂત:
	SUPER::flush_args_and_op (op, env, param);
	તોડો;
    }
  }

  સ્થિર શૂન્ય flush_args (cff2_cs_interp_env_t<blend_arg_t> &env, flatten_param_t& param)
  {
    માટે (unsigned પૂર્ણાંક i = 0; i < env.arg_ઢગલો.get_ગણતરી ();)
    {
      અચલ blend_arg_t &arg = env.arg_ઢગલો[i];
      જો (arg.blending ())
      {
	જો (unlikely (!((arg.અંક_કિંમતો_2 > 0) && (env.arg_ઢગલો.get_ગણતરી () >= arg.અંક_કિંમતો_2))))
	{
	  env.set_error ();
	  પાછા;
	}
	જો (param.instancer)
	  rewrite_blends (arg, i, env, param);
	નહીંતર
	  flatten_blends (arg, i, env, param);
	i += arg.અંક_કિંમતો_2;
      }
      નહીંતર
      {
	str_encoder_t  encoder (param.flatStr);
	encoder.encode_અંક_cs (arg);
	i++;
      }
    }
    SUPER::flush_args (env, param);
  }

  સ્થિર શૂન્ય flatten_blends (અચલ blend_arg_t &arg, unsigned પૂર્ણાંક i, cff2_cs_interp_env_t<blend_arg_t> &env, flatten_param_t& param)
  {
    /* flatten the default values */
    str_encoder_t  encoder (param.flatStr);
    માટે (unsigned પૂર્ણાંક j = 0; j < arg.અંક_કિંમતો_2; j++)
    {
      અચલ blend_arg_t &arg1 = env.arg_ઢગલો[i + j];
      જો (unlikely (!((arg1.blending () && (arg.અંક_કિંમતો_2 == arg1.અંક_કિંમતો_2) && (arg1.કિંમત_Index_2 == j) &&
	      (arg1.deltas.length == env.get_region_ગણતરી ())))))
      {
	env.set_error ();
	પાછા;
      }
      encoder.encode_અંક_cs (arg1);
    }
    /* flatten deltas for each value */
    માટે (unsigned પૂર્ણાંક j = 0; j < arg.અંક_કિંમતો_2; j++)
    {
      અચલ blend_arg_t &arg1 = env.arg_ઢગલો[i + j];
      માટે (unsigned પૂર્ણાંક k = 0; k < arg1.deltas.length; k++)
	encoder.encode_અંક_cs (arg1.deltas[k]);
    }
    /* flatten the number of values followed by blend operator */
    encoder.encode_int (arg.અંક_કિંમતો_2);
    encoder.encode_op (OpCode_blendcs);
  }

  /* Partial instancing: rewrite one blend group against the instanced store.
   * Each value's default absorbs the folded (pinned-away) contribution, and
   * its deltas are mapped onto the new region list. */
  સ્થિર શૂન્ય rewrite_blends (અચલ blend_arg_t &arg, unsigned પૂર્ણાંક i, cff2_cs_interp_env_t<blend_arg_t> &env, flatten_param_t& param)
  {
    અચલ cff2_instancing_plan_t &instancer = *param.instancer;
    unsigned ivs = env.get_ivs ();
    અચલ cff2_instancing_plan_t::vardata_transform_t *t = instancer.get_transform (ivs);
    જો (unlikely (!t))
    {
      env.set_error ();
      પાછા;
    }

    unsigned n = arg.અંક_કિંમતો_2;
    માટે (unsigned પૂર્ણાંક j = 0; j < n; j++)
    {
      અચલ blend_arg_t &arg1 = env.arg_ઢગલો[i + j];
      જો (unlikely (!(arg1.blending () && (arg1.અંક_કિંમતો_2 == n) && (arg1.કિંમત_Index_2 == j) &&
		      (arg1.deltas.length == env.get_region_ગણતરી ()) &&
		      (arg1.deltas.length == t->gains.length))))
      {
	env.set_error ();
	પાછા;
      }
    }

    str_encoder_t encoder (param.flatStr);
    unsigned k_new = t->alive () ? t->matrix.length : 0;

    જો (!k_new)
    {
      /* All of this VarData's regions died; blends dissolve into statics. */
      માટે (unsigned પૂર્ણાંક j = 0; j < n; j++)
      {
	અચલ blend_arg_t &arg1 = env.arg_ઢગલો[i + j];
	સંખ્યા_t v;
	v.set_real (arg1.to_real () + round (instancer.fold_deltas (ivs, arg1.deltas.as_array ())));
	encoder.encode_અંક_cs (v);
      }
      પાછા;
    }

    /* Emit in chunks so the output charstring stays within the argument
     * stack limit: values already emitted for this op, plus this chunk's
     * defaults, deltas and count, must not exceed 513. */
    unsigned done = 0;
    જ્યારે (done < n)
    {
      unsigned avail = 512 > (i + done) ? 512 - (i + done) : 0;
      unsigned chunk = hb_min (avail / (k_new + 1), n - done);
      જો (unlikely (!chunk))
      {
	env.set_error ();
	પાછા;
      }

      માટે (unsigned પૂર્ણાંક j = done; j < done + chunk; j++)
      {
	અચલ blend_arg_t &arg1 = env.arg_ઢગલો[i + j];
	સંખ્યા_t v;
	v.set_real (arg1.to_real () + round (instancer.fold_deltas (ivs, arg1.deltas.as_array ())));
	encoder.encode_અંક_cs (v);
      }
      માટે (unsigned પૂર્ણાંક j = done; j < done + chunk; j++)
      {
	અચલ blend_arg_t &arg1 = env.arg_ઢગલો[i + j];
	માટે (unsigned r = 0; r < k_new; r++)
	{
	  સંખ્યા_t v;
	  v.set_int (hb_clamp_to<પૂર્ણાંક> (round (t->transform_delta (r, arg1.deltas.as_array ()))));
	  encoder.encode_અંક_cs (v);
	}
      }
      encoder.encode_int (chunk);
      encoder.encode_op (OpCode_blendcs);
      done += chunk;
    }
    param.emitted_blend = સાચું;
  }

  /* Partial instancing: prepend a vsindex op when the charstring's
   * (remapped) ivs differs from its FD's (remapped) private-dict ivs. */
  સ્થિર શૂન્ય emit_vsindex_prefix (cff2_cs_interp_env_t<blend_arg_t> &env, flatten_param_t& param)
  {
    જો (!param.emitted_blend) પાછા;
    અચલ cff2_instancing_plan_t &instancer = *param.instancer;
    unsigned new_ivs = instancer.remap_ivs (env.get_ivs ());
    unsigned fd_ivs = instancer.remap_ivs (env.get_orig_ivs ());
    જો (new_ivs == fd_ivs) પાછા;

    str_buff_t prefix;
    str_encoder_t enc (prefix);
    enc.encode_int ((પૂર્ણાંક) new_ivs);
    enc.encode_op (OpCode_vsindexcs);

    str_buff_t &buf = param.flatStr;
    unsigned old_len = buf.length;
    જો (unlikely (prefix.in_error () || !buf.resize (old_len + prefix.length)))
    {
      env.set_error ();
      પાછા;
    }
    memmove (buf.arrayZ + prefix.length, buf.arrayZ, old_len);
    hb_memcpy (buf.arrayZ, prefix.arrayZ, prefix.length);
  }

  સ્થિર શૂન્ય flush_op (op_code_t op, cff2_cs_interp_env_t<blend_arg_t> &env, flatten_param_t& param)
  {
    પસંદગી (op)
    {
      સ્થિતિ OpCode_return:
      સ્થિતિ OpCode_endchar:
	પાછા;
      મૂળભૂત:
	str_encoder_t  encoder (param.flatStr);
	encoder.encode_op (op);
    }
  }

  સ્થિર શૂન્ય flush_hintmask (op_code_t op, cff2_cs_interp_env_t<blend_arg_t> &env, flatten_param_t& param)
  {
    SUPER::flush_hintmask (op, env, param);
    /* Preserve hintmask payload in captured commands for specializer re-encoding. */
    જો (param.commands && !param.drop_hints && param.commands->length > 0)
    {
      auto &cmd = param.commands->tail ();
      જો (cmd.op == op)
      {
        cmd.mask_bytes.resize (env.hintmask_size);
        જો (unlikely (cmd.mask_bytes.in_error ()))
        {
          env.set_error ();
          પાછા;
        }
        માટે (unsigned પૂર્ણાંક i = 0; i < env.hintmask_size; i++)
          cmd.mask_bytes[i] = env.str_ref[i];
      }
    }
    જો (!param.drop_hints)
    {
      str_encoder_t  encoder (param.flatStr);
      માટે (unsigned પૂર્ણાંક i = 0; i < env.hintmask_size; i++)
	encoder.encode_byte (env.str_ref[i]);
    }
  }

  ખાનગી:
  પ્રકારવ્યાખ્યા cff2_cs_opset_t<cff2_cs_opset_flatten_t, flatten_param_t, blend_arg_t> SUPER;
  પ્રકારવ્યાખ્યા cs_opset_t<blend_arg_t, cff2_cs_opset_flatten_t, cff2_cs_opset_flatten_t, cff2_cs_interp_env_t<blend_arg_t>, flatten_param_t> CSOPSET;
};

સંરચના cff2_cs_opset_subr_subset_t : cff2_cs_opset_t<cff2_cs_opset_subr_subset_t, subr_subset_param_t, blend_arg_t>
{
  સ્થિર શૂન્ય process_op (op_code_t op, cff2_cs_interp_env_t<blend_arg_t> &env, subr_subset_param_t& param)
  {
    પસંદગી (op) {

      સ્થિતિ OpCode_return:
	param.current_parsed_str->flush_coalesced (env.str_ref);
	param.current_parsed_str->set_parsed ();
	env.return_from_subr ();
	param.set_current_str (env, ખોટું);
	તોડો;

      સ્થિતિ OpCode_endchar:
	param.current_parsed_str->flush_coalesced (env.str_ref);
	param.current_parsed_str->set_parsed ();
	SUPER::process_op (op, env, param);
	તોડો;

      સ્થિતિ OpCode_callsubr:
	process_call_subr (op, CSType_LocalSubr, env, param, env.localSubrs, param.local_closure);
	તોડો;

      સ્થિતિ OpCode_callgsubr:
	process_call_subr (op, CSType_GlobalSubr, env, param, env.globalSubrs, param.global_closure);
	તોડો;

      મૂળભૂત:
	SUPER::process_op (op, env, param);
	param.current_parsed_str->add_op (op, env.str_ref);
	તોડો;
    }
  }

  સુરક્ષિત:
  સ્થિર શૂન્ય process_call_subr (op_code_t op, cs_type_t type,
				 cff2_cs_interp_env_t<blend_arg_t> &env, subr_subset_param_t& param,
				 cff2_biased_subrs_t& subrs, hb_set_t *closure)
  {
    byte_str_ref_t    str_ref = env.str_ref;
    env.call_subr (subrs, type);
    param.current_parsed_str->add_call_op (op, str_ref, env.context.subr_અંક_2);
    closure->add (env.context.subr_અંક_2);
    param.set_current_str (env, સાચું);
  }

  ખાનગી:
  પ્રકારવ્યાખ્યા cff2_cs_opset_t<cff2_cs_opset_subr_subset_t, subr_subset_param_t, blend_arg_t> SUPER;
};

સંરચના cff2_subr_subsetter_t : subr_subsetter_t<cff2_subr_subsetter_t, CFF2Subrs, અચલ OT::cff2::accelerator_subset_t, cff2_cs_interp_env_t<blend_arg_t>, cff2_cs_opset_subr_subset_t>
{
  cff2_subr_subsetter_t (અચલ OT::cff2::accelerator_subset_t &acc_, અચલ hb_subset_plan_t *plan_)
    : subr_subsetter_t (acc_, plan_) {}

  સ્થિર શૂન્ય complete_parsed_str (cff2_cs_interp_env_t<blend_arg_t> &env, subr_subset_param_t& param, parsed_cs_str_t &charstring)
  {
    /* vsindex is inserted at the beginning of the charstring as necessary */
    જો (env.seen_vsindex ())
    {
      સંખ્યા_t  ivs;
      ivs.set_int ((પૂર્ણાંક)env.get_ivs ());
      charstring.set_prefix (ivs, OpCode_vsindexcs);
    }
  }
};

સંરચના cff2_private_blend_encoder_param_t
{
  cff2_private_blend_encoder_param_t (hb_serialize_context_t *c,
				      અચલ CFF2ItemVariationStore *varStore,
				      hb_array_t<પૂર્ણાંક> normalized_coords) :
    c (c), varStore (varStore), normalized_coords (normalized_coords) {}

  શૂન્ય init () {}

  શૂન્ય process_blend ()
  {
    જો (!seen_blend)
    {
      region_ગણતરી_2 = varStore->varStore.get_region_index_ગણતરી (ivs);
      જો (unlikely (!scalars.resize_exact (region_ગણતરી_2)))
      {
	જો (c) c->err (HB_SERIALIZE_ERROR_OTHER);
	seen_blend = સાચું;
	પાછા;
      }
      varStore->varStore.get_region_scalars (ivs, normalized_coords.arrayZ, normalized_coords.length,
					     &scalars[0], region_ગણતરી_2);
      seen_blend = સાચું;
    }
  }

  દ્વિગુણ blend_deltas (hb_array_t<અચલ સંખ્યા_t> deltas) અચલ
  {
    દ્વિગુણ v = 0;
    જો (likely (scalars.length == deltas.length))
    {
      unsigned ગણતરી = scalars.length;
      માટે (unsigned i = 0; i < ગણતરી; i++)
	v += (દ્વિગુણ) scalars.arrayZ[i] * deltas.arrayZ[i].to_real ();
    }
    પાછા v;
  }


  hb_serialize_context_t *c = nullptr;
  તાર્કિક seen_blend = ખોટું;
  unsigned ivs = 0;
  unsigned region_ગણતરી_2 = 0;
  hb_vector_t<દશાંશ> scalars;
  અચલ	 CFF2ItemVariationStore *varStore = nullptr;
  hb_array_t<પૂર્ણાંક> normalized_coords;

  /* CFF2 partial instancing: when set, blends are rewritten against the
   * instanced variation store instead of being flattened. */
  અચલ cff2_instancing_plan_t *instancer = nullptr;
  /* Error-compensated fold rounding state, per dict operator. */
  દ્વિગુણ fold_exact = 0.;
  દ્વિગુણ fold_emitted = 0.;
};

સંરચના cff2_private_dict_blend_opset_t : dict_opset_t
{
  સ્થિર શૂન્ય process_arg_blend (cff2_private_blend_encoder_param_t& param,
				 સંખ્યા_t &arg,
				 અચલ hb_array_t<અચલ સંખ્યા_t> blends,
				 unsigned n, unsigned i)
  {
    arg.set_int (round (arg.to_real () + param.blend_deltas (blends)));
  }

  /* The fold added to a value, error-compensated across the operator: most
   * blended private-dict values (BlueValues, StemSnap*, ...) are
   * delta-encoded, so independently rounded folds would accumulate in the
   * decoded absolutes; keeping the emitted cumulative fold equal to the
   * rounded exact cumulative fold bounds every absolute within ±0.5. */
  સ્થિર દ્વિગુણ fold (cff2_private_blend_encoder_param_t& param,
		      અચલ hb_array_t<અચલ સંખ્યા_t> blends)
  {
    દ્વિગુણ f = param.blend_deltas (blends);
    દ્વિગુણ emit = round (param.fold_exact + f) - param.fold_emitted;
    param.fold_exact += f;
    param.fold_emitted += emit;
    પાછા emit;
  }

  /* Partial instancing: emit the args preceding this blend group, then the
   * group itself rewritten against the instanced store, and clear the
   * stack.  The op's remaining args are flushed at process_op time. */
  સ્થિર શૂન્ય rewrite_blends (cff2_priv_dict_interp_env_t& env,
			      cff2_private_blend_encoder_param_t& param,
			      unsigned start, unsigned n, unsigned k)
  {
    અચલ cff2_instancing_plan_t &instancer = *param.instancer;
    અચલ cff2_instancing_plan_t::vardata_transform_t *t = instancer.get_transform (param.ivs);
    જો (unlikely (!t || t->gains.length != k))
    {
      env.set_error ();
      પાછા;
    }

    str_buff_t str;
    str_encoder_t encoder (str);

    માટે (unsigned idx = 0; idx < start; idx++)
      encoder.encode_અંક_tp (env.arg_ઢગલો[idx]);

    unsigned k_new = t->alive () ? t->matrix.length : 0;
    જો (!k_new)
    {
      /* This VarData died (or is unused by any retained charstring, which
       * can happen for FDs kept only for retain-gids holes); blends
       * dissolve into statics. */
      માટે (unsigned j = 0; j < n; j++)
      {
	અચલ hb_array_t<અચલ સંખ્યા_t> blends = env.arg_ઢગલો.sub_array (start + n + (j * k), k);
	સંખ્યા_t v;
	v.set_real (env.arg_ઢગલો[start + j].to_real () + fold (param, blends));
	encoder.encode_અંક_tp (v);
      }
    }
    નહીંતર
    {
      unsigned done = 0;
      જ્યારે (done < n)
      {
	unsigned avail = 512 > (start + done) ? 512 - (start + done) : 0;
	unsigned chunk = hb_min (avail / (k_new + 1), n - done);
	જો (unlikely (!chunk))
	{
	  env.set_error ();
	  પાછા;
	}

	માટે (unsigned j = done; j < done + chunk; j++)
	{
	  અચલ hb_array_t<અચલ સંખ્યા_t> blends = env.arg_ઢગલો.sub_array (start + n + (j * k), k);
	  સંખ્યા_t v;
	  v.set_real (env.arg_ઢગલો[start + j].to_real () + fold (param, blends));
	  encoder.encode_અંક_tp (v);
	}
	માટે (unsigned j = done; j < done + chunk; j++)
	{
	  અચલ hb_array_t<અચલ સંખ્યા_t> blends = env.arg_ઢગલો.sub_array (start + n + (j * k), k);
	  માટે (unsigned r = 0; r < k_new; r++)
	  {
	    સંખ્યા_t v;
	    v.set_int (hb_clamp_to<પૂર્ણાંક> (round (t->transform_delta (r, blends))));
	    encoder.encode_અંક_tp (v);
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

  સ્થિર શૂન્ય process_blend (cff2_priv_dict_interp_env_t& env, cff2_private_blend_encoder_param_t& param)
  {
    unsigned પૂર્ણાંક n, k;

    param.process_blend ();
    k = param.region_ગણતરી_2;
    n = env.arg_ઢગલો.કાઢો_uint ();

    unsigned ગણતરી = env.arg_ઢગલો.get_ગણતરી ();
    unsigned પૂર્ણાંક total;
    જો (unlikely (hb_unsigned_mul_overflows (k + 1, n, &total) || total > ગણતરી))
    {
      env.set_error ();
      પાછા;
    }

    /* copy the blend values into blend array of the default values */
    unsigned પૂર્ણાંક start = ગણતરી - total;
    /* let an obvious error case fail, but note CFF2 spec doesn't forbid n==0 */
    જો (unlikely (start > ગણતરી))
    {
      env.set_error ();
      પાછા;
    }

    જો (param.instancer && k)
    {
      rewrite_blends (env, param, start, n, k);
      પાછા;
    }

    માટે (unsigned પૂર્ણાંક i = 0; i < n; i++)
    {
      અચલ hb_array_t<અચલ સંખ્યા_t> blends = env.arg_ઢગલો.sub_array (start + n + (i * k), k);
      process_arg_blend (param, env.arg_ઢગલો[start + i], blends, n, i);
    }

    /* pop off blend values leaving default values now adorned with blend values */
    env.arg_ઢગલો.કાઢો (k * n);
  }

  સ્થિર શૂન્ય process_op (op_code_t op, cff2_priv_dict_interp_env_t& env, cff2_private_blend_encoder_param_t& param)
  {
    પસંદગી (op) {
      સ્થિતિ OpCode_StdHW:
      સ્થિતિ OpCode_StdVW:
      સ્થિતિ OpCode_BlueScale:
      સ્થિતિ OpCode_BlueShift:
      સ્થિતિ OpCode_BlueFuzz:
      સ્થિતિ OpCode_ExpansionFactor:
      સ્થિતિ OpCode_LanguageGroup:
      સ્થિતિ Op_Code_Blue_કિંમતો:
      સ્થિતિ OpCode_OtherBlues:
      સ્થિતિ OpCode_FamilyBlues:
      સ્થિતિ OpCode_FamilyOtherBlues:
      સ્થિતિ OpCode_StemSnapH:
      સ્થિતિ OpCode_StemSnapV:
	તોડો;
      સ્થિતિ OpCode_vsindexdict:
	env.process_vsindex ();
	param.ivs = env.get_ivs ();
	જો (param.instancer)
	{
	  /* Re-emit with the remapped vsindex; 0 is the dict default. */
	  unsigned new_ivs = param.instancer->remap_ivs (param.ivs);
	  જો (new_ivs)
	  {
	    str_buff_t str;
	    str_encoder_t encoder (str);
	    encoder.encode_int ((પૂર્ણાંક) new_ivs);
	    encoder.encode_op (OpCode_vsindexdict);
	    auto bytes = str.as_bytes ();
	    param.c->embed (&bytes, bytes.length);
	  }
	}
	env.clear_args ();
	પાછા;
      સ્થિતિ OpCode_blenddict:
	process_blend (env, param);
	પાછા;

      મૂળભૂત:
	dict_opset_t::process_op (op, env);
	જો (!env.arg_ઢગલો.is_empty ()) પાછા;
	તોડો;
    }

    જો (unlikely (env.in_error ())) પાછા;

    // Write args then op

    str_buff_t str;
    str_encoder_t encoder (str);

    unsigned ગણતરી = env.arg_ઢગલો.get_ગણતરી ();
    માટે (unsigned i = 0; i < ગણતરી; i++)
      encoder.encode_અંક_tp (env.arg_ઢગલો[i]);

    encoder.encode_op (op);

    auto bytes = str.as_bytes ();
    param.c->embed (&bytes, bytes.length);

    env.clear_args ();
    param.fold_exact = param.fold_emitted = 0.;
  }
};

સંરચના cff2_private_dict_op_serializer_t : op_serializer_t
{
  cff2_private_dict_op_serializer_t (તાર્કિક desubroutinize_, તાર્કિક drop_hints_, તાર્કિક pinned_,
				     અચલ CFF::CFF2ItemVariationStore* varStore_,
				     hb_array_t<પૂર્ણાંક> normalized_coords_,
				     અચલ cff2_instancing_plan_t *instancer_ = nullptr)
    : desubroutinize (desubroutinize_), drop_hints (drop_hints_), pinned (pinned_),
      param (nullptr, varStore_, normalized_coords_)
  { param.instancer = instancer_; }

  તાર્કિક serialize (hb_serialize_context_t *c,
		  અચલ op_str_t &opstr,
		  objidx_t subrs_link) અચલ
  {
    TRACE_SERIALIZE (આ);

    જો (drop_hints && dict_opset_t::is_hint_op (opstr.op))
      return_trace (સાચું);

    જો (opstr.op == OpCode_Subrs)
    {
      જો (desubroutinize || !subrs_link)
	return_trace (સાચું);
      નહીંતર
	return_trace (FontDict::serialize_link2_op (c, opstr.op, subrs_link));
    }

    જો (pinned || param.instancer)
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

  સુરક્ષિત:
  અચલ તાર્કિક desubroutinize;
  અચલ તાર્કિક drop_hints;
  અચલ તાર્કિક pinned;
  mutable cff2_private_blend_encoder_param_t param;
};



નામાકાશ OT {
સંરચના cff2_subset_plan
{
  તાર્કિક create (અચલ OT::cff2::accelerator_subset_t &acc,
	      hb_subset_plan_t *plan)
  {
    /* make sure notdef is first */
    hb_codepoint_t old_glyph;
    જો (!plan->old_gid_for_new_gid (0, &old_glyph) || (old_glyph != 0)) પાછા ખોટું;

    અંક_glyphs = plan->અંક_output_glyphs ();
    orig_fdcount = acc.fdArray->ગણતરી;

    drop_hints = plan->flags & HB_SUBSET_FLAGS_NO_HINTING;
    તાર્કિક instancing_requested = (તાર્કિક) plan->normalized_coords;
    pinned = instancing_requested && plan->all_axes_pinned;
    normalized_coords = plan->normalized_coords;
    head_maxp_info = plan->head_maxp_info;
    hmtx_map = &plan->hmtx_map;
    desubroutinize = plan->flags & HB_SUBSET_FLAGS_DESUBROUTINIZE ||
		     instancing_requested; // For instancing we need this path

    /* Partial instancing: some axes pinned or limited, others kept. */
    partial_instancing = instancing_requested && !pinned &&
			 acc.varStore != &Null (CFF2ItemVariationStore);
    જો (partial_instancing &&
	unlikely (!instancing.create (acc.varStore, plan)))
      પાછા ખોટું;
    axis_tags = &plan->axis_tags;

    /* Enable command capture for CFF2→CFF1 conversion (for specialization) */
    capture_commands = pinned;

 #ifdef HB_EXPERIMENTAL_API
    min_charstrings_off_size = (plan->flags & HB_SUBSET_FLAGS_IFTB_REQUIREMENTS) ? 4 : 0;
 #નહીંતર
    min_charstrings_off_size = 0;
 #endif

    જો (desubroutinize)
    {
      જો (partial_instancing)
      {
	/* Pass 1: collect which VarDatas the retained blends reference, so
	 * unused ones get pruned and the survivors renumbered. */
	hb_set_t used_fds;
	માટે (unsigned પૂર્ણાંક i = 0; i < અંક_glyphs; i++)
	{
	  hb_codepoint_t glyph;
	  જો (!plan->old_gid_for_new_gid (i, &glyph))
	    ચાલુ;
	  અચલ hb_ubytes_t str = (*acc.charStrings)[glyph];
	  unsigned પૂર્ણાંક fd = CFF::subset_get_fd (acc, glyph);
	  જો (unlikely (fd >= acc.fd_ગણતરી))
	    પાછા ખોટું;
	  used_fds.add (fd);

	  cff2_cs_interp_env_t<blend_arg_t> env (str, acc, fd);
	  cs_interpreter_t<cff2_cs_interp_env_t<blend_arg_t>, cff2_cs_opset_usage_t, cff2_usage_param_t> interp (env);
	  cff2_usage_param_t param;
	  param.instancing = &instancing;
	  param.drop_hints = drop_hints;
	  જો (unlikely (!interp.interpret (param)))
	    પાછા ખોટું;
	}
	માટે (unsigned fd : used_fds)
	{
	  cff2_usage_param_t param;
	  param.instancing = &instancing;
	  unsigned ગણતરી = acc.privateDicts[fd].get_ગણતરી ();
	  માટે (unsigned j = 0; j < ગણતરી; j++)
	  {
	    અચલ op_str_t &opstr = acc.privateDicts[fd][j];
	    જો (drop_hints && dict_opset_t::is_hint_op (opstr.op))
	      ચાલુ;
	    cff2_priv_dict_interp_env_t env {hb_ubytes_t (opstr.ptr, opstr.length)};
	    dict_interpreter_t<cff2_private_dict_usage_opset_t, cff2_usage_param_t, cff2_priv_dict_interp_env_t> interp (env);
	    જો (unlikely (!interp.interpret (param)))
	      પાછા ખોટું;
	  }
	}
	જો (unlikely (!instancing.finalize ()))
	  પાછા ખોટું;

	/* Pass 2: like subr_flattener_t, but blends stay symbolic (no coords
	 * fed to the env) and get rewritten against the instanced store. */
	જો (!subset_charstrings.resize_exact (અંક_glyphs))
	  પાછા ખોટું;
	માટે (unsigned પૂર્ણાંક i = 0; i < અંક_glyphs; i++)
	{
	  hb_codepoint_t glyph;
	  જો (!plan->old_gid_for_new_gid (i, &glyph))
	    ચાલુ;
	  અચલ hb_ubytes_t str = (*acc.charStrings)[glyph];
	  unsigned પૂર્ણાંક fd = CFF::subset_get_fd (acc, glyph);
	  જો (unlikely (fd >= acc.fd_ગણતરી))
	    પાછા ખોટું;

	  cff2_cs_interp_env_t<blend_arg_t> env (str, acc, fd);
	  cs_interpreter_t<cff2_cs_interp_env_t<blend_arg_t>, cff2_cs_opset_flatten_t, flatten_param_t> interp (env);
	  flatten_param_t param = {
	    subset_charstrings.arrayZ[i],
	    (તાર્કિક) (plan->flags & HB_SUBSET_FLAGS_NO_HINTING),
	    plan
	  };
	  param.instancer = &instancing;
	  જો (unlikely (!interp.interpret (param)))
	    પાછા ખોટું;
	}
      }
      નહીંતર
      {
      /* Flatten global & local subrs */
      subr_flattener_t<અચલ OT::cff2::accelerator_subset_t, cff2_cs_interp_env_t<blend_arg_t>, cff2_cs_opset_flatten_t>
		    flattener(acc, plan);

      /* Enable command capture if requested (for specialization) */
      જો (capture_commands)
      {
	જો (!charstring_commands.resize_exact (અંક_glyphs))
	  પાછા ખોટું;

	જો (!flattener.flatten (subset_charstrings, &charstring_commands))
	  પાછા ખોટું;
      }
      નહીંતર
      {
	જો (!flattener.flatten (subset_charstrings))
	  પાછા ખોટું;
      }
      }
    }
    નહીંતર
    {
      cff2_subr_subsetter_t	subr_subsetter (acc, plan);

      /* Subset subrs: collect used subroutines, leaving all unused ones behind */
      જો (!subr_subsetter.subset ())
	પાછા ખોટું;

      /* encode charstrings, global subrs, local subrs with new subroutine numbers */
      જો (!subr_subsetter.encode_charstrings (subset_charstrings, !pinned))
	પાછા ખોટું;

      જો (!subr_subsetter.encode_globalsubrs (subset_globalsubrs))
	પાછા ખોટું;

      /* local subrs */
      જો (!subset_localsubrs.resize (orig_fdcount))
	પાછા ખોટું;
      માટે (unsigned પૂર્ણાંક fd = 0; fd < orig_fdcount; fd++)
      {
	subset_localsubrs[fd].init ();
	જો (!subr_subsetter.encode_localsubrs (fd, subset_localsubrs[fd]))
	  પાછા ખોટું;
      }
    }

    /* FDSelect */
    જો (acc.fdSelect != &Null (CFF2FDSelect))
    {
      જો (unlikely (!hb_plan_subset_cff_fdselect (plan,
						  orig_fdcount,
						  *(અચલ FDSelect *)acc.fdSelect,
						  acc.અંક_glyphs,
						  subset_fdcount,
						  subset_fdselect_size,
						  subset_fdselect_format,
						  subset_fdselect_ranges,
						  fdmap)))
	પાછા ખોટું;
    }
    નહીંતર
      fdmap.identity (1);

    પાછા સાચું;
  }

  cff2_sub_table_info_t info;

  unsigned પૂર્ણાંક    અંક_glyphs;
  unsigned પૂર્ણાંક    orig_fdcount = 0;
  unsigned પૂર્ણાંક    subset_fdcount = 1;
  unsigned પૂર્ણાંક    subset_fdselect_size = 0;
  unsigned પૂર્ણાંક    subset_fdselect_format = 0;
  તાર્કિક            pinned = ખોટું;
  hb_vector_t<code_pair_t>   subset_fdselect_ranges;

  hb_inc_bimap_t   fdmap;

  str_buff_vec_t	    subset_charstrings;
  str_buff_vec_t	    subset_globalsubrs;
  hb_vector_t<str_buff_vec_t> subset_localsubrs;

  તાર્કિક	    drop_hints = ખોટું;
  તાર્કિક	    desubroutinize = ખોટું;

  તાર્કિક	    partial_instancing = ખોટું;
  cff2_instancing_plan_t instancing;
  અચલ hb_vector_t<hb_tag_t> *axis_tags = nullptr;

  unsigned  min_charstrings_off_size = 0;

  hb_array_t<પૂર્ણાંક> normalized_coords; // For instantiation
  head_maxp_info_t head_maxp_info;  // For FontBBox
  અચલ hb_hashmap_t<hb_codepoint_t, hb_pair_t<unsigned, પૂર્ણાંક>> *hmtx_map; // For widths

  // Width optimization results (for CFF1 conversion)
  unsigned default_width = 0;
  unsigned nominal_width = 0;

  // Command capture for specialization (CFF2→CFF1 conversion)
  તાર્કિક capture_commands = ખોટું;
  hb_vector_t<hb_vector_t<cs_command_t>> charstring_commands;
};
} // namespace OT

/*
 * CFF2 to CFF1 Converter Implementation
 */

#અન્તર્ભાવ "hb-cff-width-optimizer.hh"
#અન્તર્ભાવ "hb-cff-specializer.hh"

/* Serialize charstrings using CFF1 format with widths */
સ્થિર તાર્કિક
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
  જો (unlikely (!cff1_charstrings.resize (plan.subset_charstrings.length)))
  {
    c->કાઢો_discard ();
    પાછા ખોટું;
  }

  માટે (unsigned i = 0; i < plan.subset_charstrings.length; i++)
  {
    // Get width for this glyph from hmtx_map
    unsigned width = 0;
    જો (plan.hmtx_map->has (i))
      width = plan.hmtx_map->get (i).first;

    // Encode width if different from default
    str_encoder_t encoder (cff1_charstrings[i]);
    જો (width != default_width)
    {
      પૂર્ણાંક delta = (પૂર્ણાંક) width - (પૂર્ણાંક) nominal_width;
      encoder.encode_int (delta);
    }

    // Use specialized commands if available, otherwise use binary
    જો (plan.capture_commands && i < plan.charstring_commands.length &&
        plan.charstring_commands[i].length > 0)
    {
      // Specialize and encode commands
      auto &commands = plan.charstring_commands[i];
      CFF::specialize_commands (commands, 48);  /* maxstack=48 for CFF1 */
      જો (unlikely (!CFF::encode_commands (commands, cff1_charstrings[i])))
      {
        c->કાઢો_discard ();
        પાછા ખોટું;
      }
    }
    નહીંતર
    {
      // Use binary CharString
      અચલ str_buff_t &cs = plan.subset_charstrings[i];
      માટે (unsigned j = 0; j < cs.length; j++)
        cff1_charstrings[i].push (cs[j]);
    }

    // Check if it already ends with endchar (0x0e) or return (0x0b)
    જો (cff1_charstrings[i].length == 0 ||
        (cff1_charstrings[i].tail () != 0x0e && cff1_charstrings[i].tail () != 0x0b))
    {
      // Append endchar operator
      જો (unlikely (!cff1_charstrings[i].ધકેલો_or_fail (0x0e)))
      {
        c->કાઢો_discard ();
        પાછા ખોટું;
      }
    }
  }

  unsigned data_size = 0;
  unsigned total_size = CFF1CharStrings::total_size (cff1_charstrings, &data_size, plan.min_charstrings_off_size);
  જો (unlikely (!c->start_zerocopy (total_size)))
  {
    c->કાઢો_discard ();
    પાછા ખોટું;
  }

  auto *cs = c->start_embed<CFF1CharStrings> ();
  જો (unlikely (!cs->serialize (c, cff1_charstrings, &data_size, plan.min_charstrings_off_size)))
  {
    c->કાઢો_discard ();
    પાછા ખોટું;
  }

  plan.info.char_strings_link = c->કાઢો_pack (ખોટું);
  પાછા સાચું;
}

/* Serialize CID Charset (format 2 range: gid 0-N -> cid 0-N) */
સ્થિર તાર્કિક
_serialize_cff1_charset (hb_serialize_context_t *c,
                         unsigned પૂર્ણાંક અંક_glyphs,
                         objidx_t &charset_link)
{
  // For CID fonts, create a simple identity charset
  // Format 2: one range covering all glyphs (except .notdef)
  c->push ();

  auto *charset = c->start_embed<Charset> ();
  જો (unlikely (!charset))
  {
    c->કાઢો_discard ();
    પાછા ખોટું;
  }

  // Create a single range for CID 1 to num_glyphs-1
  hb_vector_t<code_pair_t> ranges;
  જો (અંક_glyphs > 1)
  {
    code_pair_t range;
    range.code = 1;  // first CID
    range.glyph = અંક_glyphs - 2;  // nLeft (covers glyphs 1 to num_glyphs-1)
    ranges.push (range);
  }

  જો (unlikely (!charset->serialize (c, 2, અંક_glyphs, ranges)))
  {
    c->કાઢો_discard ();
    પાછા ખોટું;
  }

  charset_link = c->કાઢો_pack ();
  પાછા સાચું;
}

/* CFF2 to CFF1 serialization */
નામાકાશ CFF {

તાર્કિક
serialize_cff2_to_cff1 (hb_serialize_context_t *c,
                        OT::cff2_subset_plan &plan,
                        અચલ cff2_ટોચ_dict_કિંમતો_t &cff2_ટોચ_Dict,
                        અચલ OT::cff2::accelerator_subset_t &acc)
{
  TRACE_SERIALIZE (આ);

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
    widths.alloc (plan.અંક_glyphs);

    માટે (unsigned gid = 0; gid < plan.અંક_glyphs; gid++)
    {
      unsigned width = 0;
      જો (plan.hmtx_map->has (gid))
        width = plan.hmtx_map->get (gid).first;
      widths.push (width);
    }

    // Optimize defaultWidthX and nominalWidthX
    CFF::optimize_widths (widths, plan.default_width, plan.nominal_width);
  }

  // 1. CharStrings (with widths prepended)
  જો (!_serialize_cff1_charstrings (c, plan, plan.default_width, plan.nominal_width))
    return_trace (ખોટું);

  // 2. Private DICs & Local Subrs (same as CFF2)
  hb_vector_t<table_info_t> private_dict_infos;
  જો (unlikely (!private_dict_infos.resize (plan.subset_fdcount)))
    return_trace (ખોટું);

  માટે (પૂર્ણાંક i = (પૂર્ણાંક)acc.privateDicts.length; --i >= 0;)
  {
    જો (plan.fdmap.has (i))
    {
      objidx_t subrs_link = 0;

      જો (plan.subset_localsubrs[i].length > 0)
      {
        auto *dest = c->push<CFF1Subrs> ();
        જો (likely (dest->serialize (c, plan.subset_localsubrs[i])))
          subrs_link = c->કાઢો_pack (ખોટું);
        નહીંતર
        {
          c->કાઢો_discard ();
          return_trace (ખોટું);
        }
      }

      auto *pd = c->push<PrivateDict> ();
      // Use the CFF2 Private DICT serializer which instantiates blends when pinned=true
      cff2_private_dict_op_serializer_t privSzr (plan.desubroutinize, plan.drop_hints, plan.pinned,
                                                  acc.varStore, plan.normalized_coords);
      જો (likely (pd->serialize (c, acc.privateDicts[i], privSzr, subrs_link)))
      {
        // Add defaultWidthX and nominalWidthX for CFF1
        str_buff_t width_ops;
        str_encoder_t encoder (width_ops);
        encoder.encode_int (plan.default_width);
        encoder.encode_op (OpCode_defaultWidthX);
        encoder.encode_int (plan.nominal_width);
        encoder.encode_op (OpCode_nominalWidthX);

        જો (!encoder.in_error () && c->embed (width_ops.as_bytes ().arrayZ, width_ops.length))
        {
          unsigned fd = plan.fdmap[i];
          private_dict_infos[fd].size = c->length ();
          private_dict_infos[fd].link = c->કાઢો_pack ();
        }
        નહીંતર
        {
          c->કાઢો_discard ();
          return_trace (ખોટું);
        }
      }
      નહીંતર
      {
        c->કાઢો_discard ();
        return_trace (ખોટું);
      }
    }
  }

  // 3. FDArray - serialize CFF2 font dicts as CFF1
  {
    auto *fda = c->push<FDArray<HBUINT16>> ();
    cff_font_dict_op_serializer_t fontSzr;
    auto it =
    + hb_zip (+ hb_iter (acc.fontDicts)
              | hb_filter ([&] (અચલ cff2_font_dict_કિંમતો_t &_)
                { પાછા plan.fdmap.has (&_ - &acc.fontDicts[0]); }),
              hb_iter (private_dict_infos))
    ;
    // Explicitly specify template parameters: DICTVAL, INFO
    તાર્કિક success = fda->serialize<cff2_font_dict_કિંમતો_t, table_info_t> (c, it, fontSzr);
    જો (success)
      plan.info.fd_array_link = c->કાઢો_pack (ખોટું);
    નહીંતર
    {
      c->કાઢો_discard ();
      return_trace (ખોટું);
    }
  }

  // 4. FDSelect (required in CFF1 CID-keyed fonts)
  // CFF1 requires FDSelect for all CID-keyed fonts, even with just one FD
  // CFF2 makes it optional when there's only one FD
  જો (acc.fdSelect != &Null (CFF2FDSelect))
  {
    c->push ();
    જો (likely (hb_serialize_cff_fdselect (c, plan.અંક_glyphs,
                                          *(અચલ FDSelect *)acc.fdSelect,
                                          plan.orig_fdcount,
                                          plan.subset_fdselect_format,
                                          plan.subset_fdselect_size,
                                          plan.subset_fdselect_ranges)))
      plan.info.fd_select.link = c->કાઢો_pack ();
    નહીંતર
    {
      c->કાઢો_discard ();
      return_trace (ખોટું);
    }
  }
  નહીંતર
  {
    // Create a range-based FDSelect3 mapping all glyphs to FD 0
    // Format: format(1) + nRanges(2) + range(3) + sentinel(2) = 8 bytes
    c->push ();

    // Format byte
    HBUINT8 format;
    format = 3;
    જો (unlikely (!c->embed (format)))
    {
      c->કાઢો_discard ();
      return_trace (ખોટું);
    }

    // nRanges
    HBUINT16 nRanges;
    nRanges = 1;
    જો (unlikely (!c->embed (nRanges)))
    {
      c->કાઢો_discard ();
      return_trace (ખોટું);
    }

    // Single range: {first: 0, fd: 0}
    FDSelect3_Range range;
    range.first = 0;
    range.fd = 0;
    જો (unlikely (!c->embed (range)))
    {
      c->કાઢો_discard ();
      return_trace (ખોટું);
    }

    // Sentinel (number of glyphs)
    HBUINT16 sentinel;
    sentinel = plan.અંક_glyphs;
    જો (unlikely (!c->embed (sentinel)))
    {
      c->કાઢો_discard ();
      return_trace (ખોટું);
    }

    plan.info.fd_select.link = c->કાઢો_pack ();
  }

  // 5. Charset (CID charset for identity mapping)
  objidx_t charset_link;
  જો (!_serialize_cff1_charset (c, plan.અંક_glyphs, charset_link))
    return_trace (ખોટું);

  // 6. Global Subrs
  {
    auto *dest = c->push<CFF1Subrs> ();
    જો (likely (dest->serialize (c, plan.subset_globalsubrs)))
      c->કાઢો_pack (ખોટું);
    નહીંતર
    {
      c->કાઢો_discard ();
      return_trace (ખોટું);
    }
  }

  // 7. String INDEX - Add "Adobe" and "Identity" for ROS operator
  {
    અચલ અક્ષર *adobe_str = "Adobe";
    અચલ અક્ષર *identity_str = "Identity";
    unsigned adobe_len = 5;  // strlen("Adobe")
    unsigned identity_len = 8;  // strlen("Identity")

    // Build strings array
    hb_vector_t<hb_ubytes_t> strings;
    strings.alloc (2);
    strings.push (hb_ubytes_t ((અચલ unsigned અક્ષર *) adobe_str, adobe_len));
    strings.push (hb_ubytes_t ((અચલ unsigned અક્ષર *) identity_str, identity_len));

    // Serialize as CFF INDEX
    auto *dest = c->push<CFF1Index> ();
    જો (likely (dest->serialize (c, strings)))
      c->કાઢો_pack (ખોટું);
    નહીંતર
    {
      c->કાઢો_discard ();
      return_trace (ખોટું);
    }
  }

  // 8. CFF Header
  OT::cff1 *cff = c->allocate_min<OT::cff1> ();
  જો (unlikely (!cff)) return_trace (ખોટું);

  /* header */
  cff->version.major = 0x01;
  cff->version.minor = 0x00;
  cff->nameIndex = cff->min_size;
  cff->offSize = 4; /* unused? */

  // 9. Name INDEX (single entry)
  {
    unsigned name_len = strlen (CFF1_DEFAULT_FONT_NAME);

    CFF1Index *idx = c->start_embed<CFF1Index> ();
    જો (unlikely (!idx)) return_trace (ખોટું);

    જો (unlikely (!idx->serialize_header (c, hb_iter (&name_len, 1), name_len)))
      return_trace (ખોટું);

    જો (unlikely (!c->embed (CFF1_DEFAULT_FONT_NAME, name_len)))
      return_trace (ખોટું);
  }

  // 10. Top DICT INDEX
  {
    // Serialize the Top DICT data first
    c->push<ટોચ_Dict> ();
    cff1_from_cff2_ટોચ_dict_op_serializer_t ટોચ_Szr;

    // Serialize ROS first
    જો (unlikely (!ટોચ_Szr.serialize_ros (c)))
    {
      c->કાઢો_discard ();
      return_trace (ખોટું);
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

      જો (encoder.in_error () || !c->embed (bbox_buff.as_bytes ().arrayZ, bbox_buff.length))
      {
        c->કાઢો_discard ();
        return_trace (ખોટું);
      }
    }

    // Serialize charset operator
    જો (charset_link && unlikely (!FontDict::serialize_link4_op (c, OpCode_charset, charset_link, whence_t::Absolute)))
    {
      c->કાઢો_discard ();
      return_trace (ખોટું);
    }

    // Serialize FDSelect operator (required for CID-keyed CFF1 fonts)
    જો (plan.info.fd_select.link && unlikely (!FontDict::serialize_link4_op (c, OpCode_FDSelect, plan.info.fd_select.link, whence_t::Absolute)))
    {
      c->કાઢો_discard ();
      return_trace (ખોટું);
    }

    // Serialize FDArray operator (required for CID-keyed CFF1 fonts)
    જો (plan.info.fd_array_link && unlikely (!FontDict::serialize_link4_op (c, OpCode_FDArray, plan.info.fd_array_link, whence_t::Absolute)))
    {
      c->કાઢો_discard ();
      return_trace (ખોટું);
    }

    // Serialize other operators from CFF2 TopDict
    માટે (અચલ auto &opstr : cff2_ટોચ_Dict.values)
    {
      જો (unlikely (!ટોચ_Szr.serialize (c, opstr, plan.info)))
      {
        c->કાઢો_discard ();
        return_trace (ખોટું);
      }
    }

    unsigned ટોચ_size = c->length ();
    c->કાઢો_pack (ખોટું);

    // Serialize INDEX header
    auto *dest = c->start_embed<CFF1Index> ();
    જો (unlikely (!dest->serialize_header (c, hb_iter (&ટોચ_size, 1), ટોચ_size)))
      return_trace (ખોટું);
  }

  return_trace (સાચું);
}

} /* namespace CFF */

સ્થિર તાર્કિક _serialize_cff2_charstrings (hb_serialize_context_t *c,
			     cff2_subset_plan &plan,
			     અચલ OT::cff2::accelerator_subset_t  &acc)
{
  c->push ();

  unsigned data_size = 0;
  unsigned total_size = CFF2CharStrings::total_size (plan.subset_charstrings, &data_size, plan.min_charstrings_off_size);
  જો (unlikely (!c->start_zerocopy (total_size)))
    પાછા ખોટું;

  auto *cs = c->start_embed<CFF2CharStrings> ();
  જો (unlikely (!cs->serialize (c, plan.subset_charstrings, &data_size, plan.min_charstrings_off_size)))
  {
    c->કાઢો_discard ();
    પાછા ખોટું;
  }

  plan.info.char_strings_link = c->કાઢો_pack (ખોટું);
  પાછા સાચું;
}

તાર્કિક
OT::cff2::accelerator_subset_t::serialize (hb_serialize_context_t *c,
					   સંરચના cff2_subset_plan &plan,
					   hb_array_t<પૂર્ણાંક> normalized_coords) અચલ
{
  /* push charstrings onto the object stack first which will ensure it packs as the last
     object in the table. Keeping the chastrings last satisfies the requirements for patching
     via IFTB. If this ordering needs to be changed in the future, charstrings should be left
     at the end whenever HB_SUBSET_FLAGS_ITFB_REQUIREMENTS is enabled. */
  જો (!_serialize_cff2_charstrings(c, plan, *આ))
    પાછા ખોટું;

  /* private dicts & local subrs */
  hb_vector_t<table_info_t>  private_dict_infos;
  જો (unlikely (!private_dict_infos.resize (plan.subset_fdcount))) પાછા ખોટું;

  માટે (પૂર્ણાંક i = (પૂર્ણાંક)privateDicts.length; --i >= 0 ;)
  {
    જો (plan.fdmap.has (i))
    {
      objidx_t	subrs_link = 0;

      જો (plan.subset_localsubrs[i].length > 0)
      {
	auto *dest = c->push <CFF2Subrs> ();
	જો (likely (dest->serialize (c, plan.subset_localsubrs[i])))
	  subrs_link = c->કાઢો_pack (ખોટું);
	નહીંતર
	{
	  c->કાઢો_discard ();
	  પાછા ખોટું;
	}
      }
      auto *pd = c->push<PrivateDict> ();
      cff2_private_dict_op_serializer_t privSzr (plan.desubroutinize, plan.drop_hints, plan.pinned,
						 varStore, normalized_coords,
						 plan.partial_instancing ? &plan.instancing : nullptr);
      જો (likely (pd->serialize (c, privateDicts[i], privSzr, subrs_link)))
      {
	unsigned fd = plan.fdmap[i];
	private_dict_infos[fd].size = c->length ();
	private_dict_infos[fd].link = c->કાઢો_pack ();
      }
      નહીંતર
      {
	c->કાઢો_discard ();
	પાછા ખોટું;
      }
    }
  }

  /* FDSelect */
  જો (fdSelect != &Null (CFF2FDSelect))
  {
    c->push ();
    જો (likely (hb_serialize_cff_fdselect (c, plan.અંક_glyphs, *(અચલ FDSelect *)fdSelect,
					   plan.orig_fdcount,
					   plan.subset_fdselect_format, plan.subset_fdselect_size,
					   plan.subset_fdselect_ranges)))
      plan.info.fd_select.link = c->કાઢો_pack ();
    નહીંતર
    {
      c->કાઢો_discard ();
      પાછા ખોટું;
    }
  }

  /* FDArray (FD Index) */
  {
    auto *fda = c->push<CFF2FDArray> ();
    cff_font_dict_op_serializer_t fontSzr;
    auto it =
    + hb_zip (+ hb_iter (fontDicts)
	      | hb_filter ([&] (અચલ cff2_font_dict_કિંમતો_t &_)
		{ પાછા plan.fdmap.has (&_ - &fontDicts[0]); }),
	      hb_iter (private_dict_infos))
    ;
    જો (unlikely (!fda->serialize (c, it, fontSzr)))
    {
      c->કાઢો_discard ();
      પાછા ખોટું;
    }
    plan.info.fd_array_link = c->કાઢો_pack (ખોટું);
  }

  /* variation store */
  જો (varStore != &Null (CFF2ItemVariationStore) &&
      !plan.pinned)
  {
    જો (plan.partial_instancing)
    {
      જો (plan.instancing.has_variations ())
      {
	c->push ();
	જો (unlikely (!plan.instancing.serialize_var_store (c, *plan.axis_tags)))
	{
	  c->કાઢો_discard ();
	  પાછા ખોટું;
	}
	plan.info.var_store_link = c->કાઢો_pack (ખોટું);
      }
      /* else: the store emptied; the vstore op gets dropped along with it. */
    }
    નહીંતર
    {
      auto *dest = c->push<CFF2ItemVariationStore> ();
      જો (unlikely (!dest->serialize (c, varStore)))
      {
	c->કાઢો_discard ();
	પાછા ખોટું;
      }
      plan.info.var_store_link = c->કાઢો_pack (ખોટું);
    }
  }

  OT::cff2 *cff2 = c->allocate_min<OT::cff2> ();
  જો (unlikely (!cff2)) પાછા ખોટું;

  /* header */
  cff2->version.major = 0x02;
  cff2->version.minor = 0x00;
  cff2->ટોચ_Dict_2 = OT::cff2::static_size;

  /* top dict */
  {
    ટોચ_Dict &dict = cff2 + cff2->ટોચ_Dict_2;
    cff2_ટોચ_dict_op_serializer_t ટોચ_Szr;
    જો (unlikely (!dict.serialize (c, ટોચ_Dict_2, ટોચ_Szr, plan.info))) પાછા ખોટું;
    cff2->ટોચ_Dict_Size = c->head - (અચલ અક્ષર *)&dict;
  }

  /* global subrs */
  {
    auto *dest = c->start_embed <CFF2Subrs> ();
    પાછા dest->serialize (c, plan.subset_globalsubrs);
  }
}

તાર્કિક
OT::cff2::accelerator_subset_t::subset (hb_subset_context_t *c) અચલ
{
  cff2_subset_plan cff2_plan;

  જો (unlikely (!cff2_plan.create (*આ, c->plan))) પાછા ખોટું;

  // If instantiating (pinned) and downgrade flag is set, convert to CFF1
  જો (cff2_plan.pinned && (c->plan->flags & HB_SUBSET_FLAGS_DOWNGRADE_CFF2))
  {
    // Serialize CFF1 to the subsetter's serializer
    // If we run out of room, returning true will cause subsetter to retry with larger buffer
    તાર્કિક result = CFF::serialize_cff2_to_cff1 (c->serializer, cff2_plan, ટોચ_Dict_2, *આ);

    જો (c->serializer->ran_out_of_room ())
      પાછા સાચું; // Subsetter will retry with larger buffer

    જો (result && !c->serializer->in_error ())
    {
      // Success - end serialization to resolve links
      c->serializer->end_serialize ();

      // Copy the serialized CFF1 data and add as CFF table
      hb_blob_t *cff_blob = c->serializer->copy_blob ();
      જો (cff_blob)
      {
        c->plan->add_table (HB_TAG('C','F','F',' '), cff_blob);
        hb_blob_destroy (cff_blob);

        // Return false to signal CFF2 table is not needed
        પાછા ખોટું;
      }
    }

    // Conversion failed - don't fall back, fail hard for debugging
    પાછા ખોટું;
  }

  પાછા serialize (c->serializer, cff2_plan,
		    c->plan->normalized_coords.as_array ());
}

#endif
