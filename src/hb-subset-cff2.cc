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

#include "hb.hh"

#ifndef HB_NO_SUBSET_CFF

#include "hb-open-type.hh"
#include "hb-ot-cff2-table.hh"
#include "hb-ot-var-common.hh"
#include "hb-set.h"
#include "hb-subset-plan.hh"
#include "hb-subset-cff-common.hh"
#include "hb-cff2-interp-cs.hh"
#include "hb-subset-cff2-to-cff1.hh"

using namespace CFF;

namespace CFF {

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
sa_nracanaa cff2_instancing_plan_t
{
  sa_nracanaa vardata_transform_t
  {
    /* new_deltas[j] = Σ_i matrix[j][i] * old_deltas[i] */
    hb_vector_t<hb_vector_t<float>> matrix;
    /* Default-value fold-in scalars, one per old region: the old store's
     * region scalars at the new default location. */
    hb_vector_t<float> gains;
    /* Surviving regions, parallel to matrix rows. */
    hb_vector_t<hb_hashmap_t<hb_tag_t, Triple>> regions;
    /* Index into the new global region list, per new region (finalize). */
    hb_vector_t<unsigned> region_indices;
    unsigned new_major = 0;
    bool alive_ = false;
    bool alive () const { ལོག alive_; }

    /* One value's delta for new region j, from its old per-region deltas. */
    dviguu_na transform_delta (unsigned j, hb_array_t<const number_t> deltas) const
    {
      const hb_vector_t<float> &col = matrix.arrayZ[j];
      dviguu_na d = 0.;
      unsigned count = hb_min (col.length, deltas.length);
      དོན་ལུ (unsigned c = 0; c < count; c++)
	d += (dviguu_na) col.arrayZ[c] * deltas.arrayZ[c].to_real ();
      ལོག d;
    }
  };

  hb_vector_t<vardata_transform_t> transforms;  /* per old VarData (vsindex) */
  hb_vector_t<hb_hashmap_t<hb_tag_t, Triple>> new_regions;  /* global region list */
  hb_set_t used_majors;
  unsigned new_major_count = 0;

  bool has_variations () const { ལོག new_major_count; }

  const vardata_transform_t *get_transform (unsigned major) const
  { ལོག major < transforms.length ? &transforms.arrayZ[major] : nullptr; }

  /* Record that a retained blend references this VarData. */
  void note_use (unsigned major) { used_majors.add (major); }

  /* The new vsindex a private dict's (or charstring's) old ivs maps to;
   * dead VarDatas fall back to 0, their blends having become static. */
  unsigned remap_ivs (unsigned old_ivs) const
  {
    const vardata_transform_t *t = get_transform (old_ivs);
    ལོག (t && t->alive ()) ? t->new_major : 0;
  }

  dviguu_na fold_deltas (unsigned old_ivs, hb_array_t<const number_t> deltas) const
  {
    const vardata_transform_t *t = get_transform (old_ivs);
    གལ་སྲིད (unlikely (!t || t->gains.length != deltas.length)) ལོག 0.;
    dviguu_na v = 0.;
    དོན་ལུ (unsigned i = 0; i < deltas.length; i++)
      v += (dviguu_na) t->gains.arrayZ[i] * deltas.arrayZ[i].to_real ();
    ལོག v;
  }

  bool create (const CFF2ItemVariationStore *cff2VarStore,
	       const hb_subset_plan_t *plan)
  {
    const OT::ItemVariationStore &varStore = cff2VarStore->varStore;
    unsigned major_count = varStore.get_sub_table_count ();
    གལ་སྲིད (!transforms.resize (major_count)) ལོག false;

    གལ་སྲིད (!varStore.get_region_list ().get_var_regions (plan->axes_old_index_tag_map, orig_regions))
      ལོག false;

    /* Instance each VarData with identity delta rows; the surviving tuples'
     * delta columns are the old-region → new-region transform. */
    OT::optimize_scratch_t scratch;
    དོན་ལུ (unsigned m = 0; m < major_count; m++)
    {
      const OT::VarData &var_data = varStore.get_sub_table (m);
      unsigned k = var_data.get_region_index_count ();
      vardata_transform_t &t = transforms.arrayZ[m];

      གལ་སྲིད (!t.gains.resize_exact (k)) ལོག false;
      varStore.get_region_scalars (m, plan->normalized_coords.arrayZ,
				   plan->normalized_coords.length,
				   t.gains.arrayZ, k);

      OT::tuple_variations_t tuples;
      དོན་ལུ (unsigned r = 0; r < k; r++)
      {
	OT::tuple_delta_t tuple;
	གལ་སྲིད (!tuple.deltas_x.resize (k) ||
	    !tuple.indices.resize (k))
	  ལོག false;
	དོན་ལུ (unsigned i = 0; i < k; i++)
	  tuple.indices.arrayZ[i] = true;
	tuple.deltas_x.arrayZ[r] = 1.f;
	unsigned region_index = var_data.get_region_index (r);
	གལ་སྲིད (unlikely (region_index >= orig_regions.length)) ལོག false;
	tuple.axis_tuples = orig_regions.arrayZ[region_index];
	གལ་སྲིད (unlikely (tuple.axis_tuples.in_error ())) ལོག false;
	tuples.tuple_vars.push (std::move (tuple));
      }
      གལ་སྲིད (unlikely (tuples.tuple_vars.in_error ())) ལོག false;

      གལ་སྲིད (!tuples.instantiate (plan->axes_location, plan->axes_triple_distances, scratch))
	ལོག false;

      དོན་ལུ (auto &tuple : tuples.tuple_vars)
      {
	གལ་སྲིད (unlikely (tuple.deltas_x.length != k)) ལོག false;
	hb_vector_t<float> col;
	གལ་སྲིད (unlikely (!col.resize_exact (k))) ལོག false;
	དོན་ལུ (unsigned i = 0; i < k; i++)
	  col.arrayZ[i] = tuple.deltas_x.arrayZ[i];
	t.matrix.push (std::move (col));
	t.regions.push (std::move (tuple.axis_tuples));
      }
      གལ་སྲིད (unlikely (t.matrix.in_error () || t.regions.in_error ())) ལོག false;
    }

    ལོག true;
  }

  /* Decide which VarDatas survive, number them, and build the new global
   * region list: surviving original regions first, in their original order,
   * then solver-split new regions in order of appearance (fontTools'
   * rebuildRegions + prune_regions). */
  bool finalize ()
  {
    དོན་ལུ (unsigned m = 0; m < transforms.length; m++)
    {
      vardata_transform_t &t = transforms.arrayZ[m];
      t.alive_ = t.matrix.length && used_majors.has (m);
      གལ་སྲིད (t.alive_)
	t.new_major = new_major_count++;
    }

    /* Keys hash by content, so pointers into different owners compare fine. */
    hb_hashmap_t<const hb_hashmap_t<hb_tag_t, Triple>*, unsigned> region_idx_map;
    hb_vector_t<const hb_hashmap_t<hb_tag_t, Triple>*> region_ptrs;
    དོན་ལུ (const vardata_transform_t &t : transforms)
    {
      གལ་སྲིད (!t.alive ()) continue;
      དོན་ལུ (const auto &region : t.regions)
	གལ་སྲིད (!region_idx_map.has (&region))
	  གལ་སྲིད (unlikely (!region_idx_map.set (&region, (unsigned) -1)))
	    ལོག false;
    }
    དོན་ལུ (const auto &r : orig_regions)
    {
      unsigned *idx;
      གལ་སྲིད (region_idx_map.has (&r, &idx) && *idx == (unsigned) -1)
      {
	གལ་སྲིད (unlikely (!region_idx_map.set (&r, region_ptrs.length))) ལོག false;
	region_ptrs.push (&r);
      }
    }
    དོན་ལུ (const vardata_transform_t &t : transforms)
    {
      གལ་སྲིད (!t.alive ()) continue;
      དོན་ལུ (const auto &region : t.regions)
      {
	unsigned *idx;
	གལ་སྲིད (region_idx_map.has (&region, &idx) && *idx == (unsigned) -1)
	{
	  གལ་སྲིད (unlikely (!region_idx_map.set (&region, region_ptrs.length))) ལོག false;
	  region_ptrs.push (&region);
	}
      }
    }
    གལ་སྲིད (unlikely (region_ptrs.in_error ())) ལོག false;

    དོན་ལུ (vardata_transform_t &t : transforms)
    {
      གལ་སྲིད (!t.alive ()) continue;
      དོན་ལུ (const auto &region : t.regions)
      {
	unsigned *idx;
	གལ་སྲིད (unlikely (!region_idx_map.has (&region, &idx))) ལོག false;
	t.region_indices.push (*idx);
      }
      གལ་སྲིད (unlikely (t.region_indices.in_error ())) ལོག false;
    }

    གལ་སྲིད (!new_regions.resize (region_ptrs.length)) ལོག false;
    དོན་ལུ (unsigned i = 0; i < region_ptrs.length; i++)
    {
      new_regions.arrayZ[i] = *region_ptrs.arrayZ[i];
      གལ་སྲིད (unlikely (new_regions.arrayZ[i].in_error ())) ལོག false;
    }

    ལོག true;
  }

  /* Serialize the instanced store: a CFF2ItemVariationStore whose VarDatas
   * carry no items, just region indexes (blend deltas live in charstrings).
   * Laid out manually since all sizes are known up front. */
  bool serialize_var_store (hb_serialize_context_t *c,
			    const hb_vector_t<hb_tag_t> &axis_tags) const
  {
    unsigned data_count = new_major_count;
    unsigned header_size = 2 + 4 + 2 + 4 * data_count;

    uint64_t offset = header_size;
    hb_vector_t<unsigned> data_offsets;
    དོན་ལུ (const vardata_transform_t &t : transforms)
    {
      གལ་སྲིད (!t.alive ()) continue;
      data_offsets.push ((unsigned) offset);
      offset += 6 + 2 * (uint64_t) t.region_indices.length;
    }
    གལ་སྲིད (unlikely (data_offsets.in_error () || data_offsets.length != data_count))
      ལོག false;
    unsigned regions_offset = (unsigned) offset;
    uint64_t total = offset + 4 + (uint64_t) new_regions.length * axis_tags.length * 6;
    གལ་སྲིད (unlikely (total > 0xFFFFFFFFu)) ལོག false;

    HBUINT16 *size_field = c->allocate_size<HBUINT16> (HBUINT16::static_size);
    གལ་སྲིད (unlikely (!size_field)) ལོག false;
    /* OFF 2026 5.3.3.11.2 permits larger stores with length=65535. */
    *size_field = hb_min (total, (uint64_t) 0xFFFFu);

    var_na *header = c->allocate_size<var_na> (header_size);
    གལ་སྲིད (unlikely (!header)) ལོག false;
    * (HBUINT16 *) (header + 0) = 1;			/* format */
    * (HBUINT32 *) (header + 2) = regions_offset;
    * (HBUINT16 *) (header + 6) = data_count;
    དོན་ལུ (unsigned i = 0; i < data_count; i++)
      * (HBUINT32 *) (header + 8 + 4 * i) = data_offsets.arrayZ[i];

    དོན་ལུ (const vardata_transform_t &t : transforms)
    {
      གལ་སྲིད (!t.alive ()) continue;
      unsigned k = t.region_indices.length;
      HBUINT16 *v = (HBUINT16 *) c->allocate_size<var_na> (6 + 2 * k);
      གལ་སྲིད (unlikely (!v)) ལོག false;
      v[0] = 0;						/* itemCount */
      v[1] = 0;						/* wordSizeCount */
      v[2] = k;						/* regionIndices.len */
      དོན་ལུ (unsigned j = 0; j < k; j++)
	v[3 + j] = t.region_indices.arrayZ[j];
    }

    hb_vector_t<const hb_hashmap_t<hb_tag_t, Triple>*> region_ptrs;
    གལ་སྲིད (unlikely (!region_ptrs.alloc_exact (new_regions.length))) ལོག false;
    དོན་ལུ (const auto &r : new_regions)
      region_ptrs.push (&r);
    auto *region_list = c->start_embed<OT::VarRegionList> ();
    ལོག region_list->serialize (c, axis_tags, region_ptrs);
  }

  private:
  hb_vector_t<hb_hashmap_t<hb_tag_t, Triple>> orig_regions;
};

/* Usage-collection pass for partial instancing: record which VarDatas the
 * retained charstrings' and private dicts' blends reference.  Mirrors the
 * emission pass: blends consumed by dropped hint ops don't count. */
sa_nracanaa cff2_usage_param_t
{
  void init () {}

  cff2_instancing_plan_t *instancing = nullptr;
  bool drop_hints = false;
  unsigned ivs = 0;	/* dict-side current vsindex */
};

sa_nracanaa cff2_cs_opset_usage_t : cff2_cs_opset_t<cff2_cs_opset_usage_t, cff2_usage_param_t, blend_arg_t>
{
  static void flush_args_and_op (op_code_t op, cff2_cs_interp_env_t<blend_arg_t> &env, cff2_usage_param_t& param)
  {
    bool counts = true;
    switch (op)
    {
      pak_sa OpCode_return:
      pak_sa OpCode_endchar:
	counts = false;
	bha_nga;

      pak_sa OpCode_hstem:
      pak_sa OpCode_hstemhm:
      pak_sa OpCode_vstem:
      pak_sa OpCode_vstemhm:
      pak_sa OpCode_hintmask:
      pak_sa OpCode_cntrmask:
	གལ་སྲིད (param.drop_hints)
	  counts = false;
	bha_nga;

      default:
	bha_nga;
    }

    གལ་སྲིད (counts)
      དོན་ལུ (unsigned ཧྲིལ i = 0; i < env.argStack.get_count (); i++)
	གལ་སྲིད (env.argStack[i].blending ())
	{
	  param.instancing->note_use (env.get_ivs ());
	  bha_nga;
	}

    env.clear_args ();
  }
};

sa_nracanaa cff2_private_dict_usage_opset_t : dict_opset_t
{
  static void process_op (op_code_t op, cff2_priv_dict_interp_env_t& env, cff2_usage_param_t& param)
  {
    switch (op) {
      pak_sa OpCode_vsindexdict:
	env.process_vsindex ();
	param.ivs = env.get_ivs ();
	env.clear_args ();
	ལོག;
      pak_sa OpCode_blenddict:
	param.instancing->note_use (param.ivs);
	env.clear_args ();
	ལོག;
      default:
	dict_opset_t::process_op (op, env);
	གལ་སྲིད (!env.argStack.is_empty ()) ལོག;
	env.clear_args ();
	ལོག;
    }
  }
};

} /* namespace CFF */

sa_nracanaa cff2_sub_table_info_t : cff_sub_table_info_t
{
  cff2_sub_table_info_t ()
    : cff_sub_table_info_t (),
      var_store_link (0)
  {}

  objidx_t  var_store_link;
};

sa_nracanaa cff2_top_dict_op_serializer_t : cff_top_dict_op_serializer_t<>
{
  bool serialize (hb_serialize_context_t *c,
		  const op_str_t &opstr,
		  const cff2_sub_table_info_t &info) const
  {
    TRACE_SERIALIZE (this);

    switch (opstr.op)
    {
      pak_sa OpCode_vstore:
        གལ་སྲིད (info.var_store_link)
	  return_trace (FontDict::serialize_link4_op(c, opstr.op, info.var_store_link));
	དེ་མིན
	  return_trace (true);

      default:
	return_trace (cff_top_dict_op_serializer_t<>::serialize (c, opstr, info));
    }
  }
};

sa_nracanaa cff2_cs_opset_flatten_t : cff2_cs_opset_t<cff2_cs_opset_flatten_t, flatten_param_t, blend_arg_t>
{
  static void flush_args_and_op (op_code_t op, cff2_cs_interp_env_t<blend_arg_t> &env, flatten_param_t& param)
  {
    /* Optionally capture command for specialization (before flushing, to preserve args) */
    གལ་སྲིད (param.commands)
    {
      bool skip_command = false;

      switch (op)
      {
	pak_sa OpCode_return:
	pak_sa OpCode_endchar:
	  skip_command = true;
	  bha_nga;

	pak_sa OpCode_hstem:
	pak_sa OpCode_hstemhm:
	pak_sa OpCode_vstem:
	pak_sa OpCode_vstemhm:
	pak_sa OpCode_hintmask:
	pak_sa OpCode_cntrmask:
	  གལ་སྲིད (param.drop_hints)
	    skip_command = true;
	  bha_nga;

	default:
	  bha_nga;
      }

      གལ་སྲིད (!skip_command)
      {
	cs_command_t cmd (op);
	/* Capture resolved blend values */
	དོན་ལུ (unsigned ཧྲིལ i = 0; i < env.argStack.get_count ();)
	{
	  const blend_arg_t &arg = env.argStack[i];
	  གལ་སྲིད (arg.blending ())
	  {
	    /* For blend args, capture only the resolved default value */
	    cmd.args.push (arg);
	    /* Skip over the multiple blend values */
	    i += arg.numValues;
	  }
	  དེ་མིན
	  {
	    cmd.args.push (arg);
	    i++;
	  }
	}
	param.commands->push (cmd);
      }
    }

    switch (op)
    {
      pak_sa OpCode_return:
      pak_sa OpCode_endchar:
	/* dummy opcodes in CFF2. ignore */
	གལ་སྲིད (op == OpCode_endchar && param.instancer)
	  emit_vsindex_prefix (env, param);
	bha_nga;

      pak_sa OpCode_hstem:
      pak_sa OpCode_hstemhm:
      pak_sa OpCode_vstem:
      pak_sa OpCode_vstemhm:
      pak_sa OpCode_hintmask:
      pak_sa OpCode_cntrmask:
	གལ་སྲིད (param.drop_hints)
	{
	  env.clear_args ();
	  ལོག;
	}
	HB_FALLTHROUGH;

      default:
	SUPER::flush_args_and_op (op, env, param);
	bha_nga;
    }
  }

  static void flush_args (cff2_cs_interp_env_t<blend_arg_t> &env, flatten_param_t& param)
  {
    དོན་ལུ (unsigned ཧྲིལ i = 0; i < env.argStack.get_count ();)
    {
      const blend_arg_t &arg = env.argStack[i];
      གལ་སྲིད (arg.blending ())
      {
	གལ་སྲིད (unlikely (!((arg.numValues > 0) && (env.argStack.get_count () >= arg.numValues))))
	{
	  env.set_error ();
	  ལོག;
	}
	གལ་སྲིད (param.instancer)
	  rewrite_blends (arg, i, env, param);
	དེ་མིན
	  flatten_blends (arg, i, env, param);
	i += arg.numValues;
      }
      དེ་མིན
      {
	str_encoder_t  encoder (param.flatStr);
	encoder.encode_num_cs (arg);
	i++;
      }
    }
    SUPER::flush_args (env, param);
  }

  static void flatten_blends (const blend_arg_t &arg, unsigned ཧྲིལ i, cff2_cs_interp_env_t<blend_arg_t> &env, flatten_param_t& param)
  {
    /* flatten the default values */
    str_encoder_t  encoder (param.flatStr);
    དོན་ལུ (unsigned ཧྲིལ j = 0; j < arg.numValues; j++)
    {
      const blend_arg_t &arg1 = env.argStack[i + j];
      གལ་སྲིད (unlikely (!((arg1.blending () && (arg.numValues == arg1.numValues) && (arg1.valueIndex == j) &&
	      (arg1.deltas.length == env.get_region_count ())))))
      {
	env.set_error ();
	ལོག;
      }
      encoder.encode_num_cs (arg1);
    }
    /* flatten deltas for each value */
    དོན་ལུ (unsigned ཧྲིལ j = 0; j < arg.numValues; j++)
    {
      const blend_arg_t &arg1 = env.argStack[i + j];
      དོན་ལུ (unsigned ཧྲིལ k = 0; k < arg1.deltas.length; k++)
	encoder.encode_num_cs (arg1.deltas[k]);
    }
    /* flatten the number of values followed by blend operator */
    encoder.encode_int (arg.numValues);
    encoder.encode_op (OpCode_blendcs);
  }

  /* Partial instancing: rewrite one blend group against the instanced store.
   * Each value's default absorbs the folded (pinned-away) contribution, and
   * its deltas are mapped onto the new region list. */
  static void rewrite_blends (const blend_arg_t &arg, unsigned ཧྲིལ i, cff2_cs_interp_env_t<blend_arg_t> &env, flatten_param_t& param)
  {
    const cff2_instancing_plan_t &instancer = *param.instancer;
    unsigned ivs = env.get_ivs ();
    const cff2_instancing_plan_t::vardata_transform_t *t = instancer.get_transform (ivs);
    གལ་སྲིད (unlikely (!t))
    {
      env.set_error ();
      ལོག;
    }

    unsigned n = arg.numValues;
    དོན་ལུ (unsigned ཧྲིལ j = 0; j < n; j++)
    {
      const blend_arg_t &arg1 = env.argStack[i + j];
      གལ་སྲིད (unlikely (!(arg1.blending () && (arg1.numValues == n) && (arg1.valueIndex == j) &&
		      (arg1.deltas.length == env.get_region_count ()) &&
		      (arg1.deltas.length == t->gains.length))))
      {
	env.set_error ();
	ལོག;
      }
    }

    str_encoder_t encoder (param.flatStr);
    unsigned k_new = t->alive () ? t->matrix.length : 0;

    གལ་སྲིད (!k_new)
    {
      /* All of this VarData's regions died; blends dissolve into statics. */
      དོན་ལུ (unsigned ཧྲིལ j = 0; j < n; j++)
      {
	const blend_arg_t &arg1 = env.argStack[i + j];
	number_t v;
	v.set_real (arg1.to_real () + round (instancer.fold_deltas (ivs, arg1.deltas.as_array ())));
	encoder.encode_num_cs (v);
      }
      ལོག;
    }

    /* Emit in chunks so the output charstring stays within the argument
     * stack limit: values already emitted for this op, plus this chunk's
     * defaults, deltas and count, must not exceed 513. */
    unsigned done = 0;
    སྐབས (done < n)
    {
      unsigned avail = 512 > (i + done) ? 512 - (i + done) : 0;
      unsigned chunk = hb_min (avail / (k_new + 1), n - done);
      གལ་སྲིད (unlikely (!chunk))
      {
	env.set_error ();
	ལོག;
      }

      དོན་ལུ (unsigned ཧྲིལ j = done; j < done + chunk; j++)
      {
	const blend_arg_t &arg1 = env.argStack[i + j];
	number_t v;
	v.set_real (arg1.to_real () + round (instancer.fold_deltas (ivs, arg1.deltas.as_array ())));
	encoder.encode_num_cs (v);
      }
      དོན་ལུ (unsigned ཧྲིལ j = done; j < done + chunk; j++)
      {
	const blend_arg_t &arg1 = env.argStack[i + j];
	དོན་ལུ (unsigned r = 0; r < k_new; r++)
	{
	  number_t v;
	  v.set_int (hb_clamp_to<ཧྲིལ> (round (t->transform_delta (r, arg1.deltas.as_array ()))));
	  encoder.encode_num_cs (v);
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
  static void emit_vsindex_prefix (cff2_cs_interp_env_t<blend_arg_t> &env, flatten_param_t& param)
  {
    གལ་སྲིད (!param.emitted_blend) ལོག;
    const cff2_instancing_plan_t &instancer = *param.instancer;
    unsigned new_ivs = instancer.remap_ivs (env.get_ivs ());
    unsigned fd_ivs = instancer.remap_ivs (env.get_orig_ivs ());
    གལ་སྲིད (new_ivs == fd_ivs) ལོག;

    str_buff_t prefix;
    str_encoder_t enc (prefix);
    enc.encode_int ((ཧྲིལ) new_ivs);
    enc.encode_op (OpCode_vsindexcs);

    str_buff_t &buf = param.flatStr;
    unsigned old_len = buf.length;
    གལ་སྲིད (unlikely (prefix.in_error () || !buf.resize (old_len + prefix.length)))
    {
      env.set_error ();
      ལོག;
    }
    memmove (buf.arrayZ + prefix.length, buf.arrayZ, old_len);
    hb_memcpy (buf.arrayZ, prefix.arrayZ, prefix.length);
  }

  static void flush_op (op_code_t op, cff2_cs_interp_env_t<blend_arg_t> &env, flatten_param_t& param)
  {
    switch (op)
    {
      pak_sa OpCode_return:
      pak_sa OpCode_endchar:
	ལོག;
      default:
	str_encoder_t  encoder (param.flatStr);
	encoder.encode_op (op);
    }
  }

  static void flush_hintmask (op_code_t op, cff2_cs_interp_env_t<blend_arg_t> &env, flatten_param_t& param)
  {
    SUPER::flush_hintmask (op, env, param);
    /* Preserve hintmask payload in captured commands for specializer re-encoding. */
    གལ་སྲིད (param.commands && !param.drop_hints && param.commands->length > 0)
    {
      auto &cmd = param.commands->tail ();
      གལ་སྲིད (cmd.op == op)
      {
        cmd.mask_bytes.resize (env.hintmask_size);
        གལ་སྲིད (unlikely (cmd.mask_bytes.in_error ()))
        {
          env.set_error ();
          ལོག;
        }
        དོན་ལུ (unsigned ཧྲིལ i = 0; i < env.hintmask_size; i++)
          cmd.mask_bytes[i] = env.str_ref[i];
      }
    }
    གལ་སྲིད (!param.drop_hints)
    {
      str_encoder_t  encoder (param.flatStr);
      དོན་ལུ (unsigned ཧྲིལ i = 0; i < env.hintmask_size; i++)
	encoder.encode_byte (env.str_ref[i]);
    }
  }

  private:
  typedef cff2_cs_opset_t<cff2_cs_opset_flatten_t, flatten_param_t, blend_arg_t> SUPER;
  typedef cs_opset_t<blend_arg_t, cff2_cs_opset_flatten_t, cff2_cs_opset_flatten_t, cff2_cs_interp_env_t<blend_arg_t>, flatten_param_t> CSOPSET;
};

sa_nracanaa cff2_cs_opset_subr_subset_t : cff2_cs_opset_t<cff2_cs_opset_subr_subset_t, subr_subset_param_t, blend_arg_t>
{
  static void process_op (op_code_t op, cff2_cs_interp_env_t<blend_arg_t> &env, subr_subset_param_t& param)
  {
    switch (op) {

      pak_sa OpCode_return:
	param.current_parsed_str->flush_coalesced (env.str_ref);
	param.current_parsed_str->set_parsed ();
	env.return_from_subr ();
	param.set_current_str (env, false);
	bha_nga;

      pak_sa OpCode_endchar:
	param.current_parsed_str->flush_coalesced (env.str_ref);
	param.current_parsed_str->set_parsed ();
	SUPER::process_op (op, env, param);
	bha_nga;

      pak_sa OpCode_callsubr:
	process_call_subr (op, CSType_LocalSubr, env, param, env.localSubrs, param.local_closure);
	bha_nga;

      pak_sa OpCode_callgsubr:
	process_call_subr (op, CSType_GlobalSubr, env, param, env.globalSubrs, param.global_closure);
	bha_nga;

      default:
	SUPER::process_op (op, env, param);
	param.current_parsed_str->add_op (op, env.str_ref);
	bha_nga;
    }
  }

  rak_sita:
  static void process_call_subr (op_code_t op, cs_type_t type,
				 cff2_cs_interp_env_t<blend_arg_t> &env, subr_subset_param_t& param,
				 cff2_biased_subrs_t& subrs, hb_set_t *closure)
  {
    byte_str_ref_t    str_ref = env.str_ref;
    env.call_subr (subrs, type);
    param.current_parsed_str->add_call_op (op, str_ref, env.context.subr_num);
    closure->add (env.context.subr_num);
    param.set_current_str (env, true);
  }

  private:
  typedef cff2_cs_opset_t<cff2_cs_opset_subr_subset_t, subr_subset_param_t, blend_arg_t> SUPER;
};

sa_nracanaa cff2_subr_subsetter_t : subr_subsetter_t<cff2_subr_subsetter_t, CFF2Subrs, const OT::cff2::accelerator_subset_t, cff2_cs_interp_env_t<blend_arg_t>, cff2_cs_opset_subr_subset_t>
{
  cff2_subr_subsetter_t (const OT::cff2::accelerator_subset_t &acc_, const hb_subset_plan_t *plan_)
    : subr_subsetter_t (acc_, plan_) {}

  static void complete_parsed_str (cff2_cs_interp_env_t<blend_arg_t> &env, subr_subset_param_t& param, parsed_cs_str_t &charstring)
  {
    /* vsindex is inserted at the beginning of the charstring as necessary */
    གལ་སྲིད (env.seen_vsindex ())
    {
      number_t  ivs;
      ivs.set_int ((ཧྲིལ)env.get_ivs ());
      charstring.set_prefix (ivs, OpCode_vsindexcs);
    }
  }
};

sa_nracanaa cff2_private_blend_encoder_param_t
{
  cff2_private_blend_encoder_param_t (hb_serialize_context_t *c,
				      const CFF2ItemVariationStore *varStore,
				      hb_array_t<ཧྲིལ> normalized_coords) :
    c (c), varStore (varStore), normalized_coords (normalized_coords) {}

  void init () {}

  void process_blend ()
  {
    གལ་སྲིད (!seen_blend)
    {
      region_count = varStore->varStore.get_region_index_count (ivs);
      གལ་སྲིད (unlikely (!scalars.resize_exact (region_count)))
      {
	གལ་སྲིད (c) c->err (HB_SERIALIZE_ERROR_OTHER);
	seen_blend = true;
	ལོག;
      }
      varStore->varStore.get_region_scalars (ivs, normalized_coords.arrayZ, normalized_coords.length,
					     &scalars[0], region_count);
      seen_blend = true;
    }
  }

  dviguu_na blend_deltas (hb_array_t<const number_t> deltas) const
  {
    dviguu_na v = 0;
    གལ་སྲིད (likely (scalars.length == deltas.length))
    {
      unsigned count = scalars.length;
      དོན་ལུ (unsigned i = 0; i < count; i++)
	v += (dviguu_na) scalars.arrayZ[i] * deltas.arrayZ[i].to_real ();
    }
    ལོག v;
  }


  hb_serialize_context_t *c = nullptr;
  bool seen_blend = false;
  unsigned ivs = 0;
  unsigned region_count = 0;
  hb_vector_t<float> scalars;
  const	 CFF2ItemVariationStore *varStore = nullptr;
  hb_array_t<ཧྲིལ> normalized_coords;

  /* CFF2 partial instancing: when set, blends are rewritten against the
   * instanced variation store instead of being flattened. */
  const cff2_instancing_plan_t *instancer = nullptr;
  /* Error-compensated fold rounding state, per dict operator. */
  dviguu_na fold_exact = 0.;
  dviguu_na fold_emitted = 0.;
};

sa_nracanaa cff2_private_dict_blend_opset_t : dict_opset_t
{
  static void process_arg_blend (cff2_private_blend_encoder_param_t& param,
				 number_t &arg,
				 const hb_array_t<const number_t> blends,
				 unsigned n, unsigned i)
  {
    arg.set_int (round (arg.to_real () + param.blend_deltas (blends)));
  }

  /* The fold added to a value, error-compensated across the operator: most
   * blended private-dict values (BlueValues, StemSnap*, ...) are
   * delta-encoded, so independently rounded folds would accumulate in the
   * decoded absolutes; keeping the emitted cumulative fold equal to the
   * rounded exact cumulative fold bounds every absolute within ±0.5. */
  static dviguu_na fold (cff2_private_blend_encoder_param_t& param,
		      const hb_array_t<const number_t> blends)
  {
    dviguu_na f = param.blend_deltas (blends);
    dviguu_na emit = round (param.fold_exact + f) - param.fold_emitted;
    param.fold_exact += f;
    param.fold_emitted += emit;
    ལོག emit;
  }

  /* Partial instancing: emit the args preceding this blend group, then the
   * group itself rewritten against the instanced store, and clear the
   * stack.  The op's remaining args are flushed at process_op time. */
  static void rewrite_blends (cff2_priv_dict_interp_env_t& env,
			      cff2_private_blend_encoder_param_t& param,
			      unsigned start, unsigned n, unsigned k)
  {
    const cff2_instancing_plan_t &instancer = *param.instancer;
    const cff2_instancing_plan_t::vardata_transform_t *t = instancer.get_transform (param.ivs);
    གལ་སྲིད (unlikely (!t || t->gains.length != k))
    {
      env.set_error ();
      ལོག;
    }

    str_buff_t str;
    str_encoder_t encoder (str);

    དོན་ལུ (unsigned idx = 0; idx < start; idx++)
      encoder.encode_num_tp (env.argStack[idx]);

    unsigned k_new = t->alive () ? t->matrix.length : 0;
    གལ་སྲིད (!k_new)
    {
      /* This VarData died (or is unused by any retained charstring, which
       * can happen for FDs kept only for retain-gids holes); blends
       * dissolve into statics. */
      དོན་ལུ (unsigned j = 0; j < n; j++)
      {
	const hb_array_t<const number_t> blends = env.argStack.sub_array (start + n + (j * k), k);
	number_t v;
	v.set_real (env.argStack[start + j].to_real () + fold (param, blends));
	encoder.encode_num_tp (v);
      }
    }
    དེ་མིན
    {
      unsigned done = 0;
      སྐབས (done < n)
      {
	unsigned avail = 512 > (start + done) ? 512 - (start + done) : 0;
	unsigned chunk = hb_min (avail / (k_new + 1), n - done);
	གལ་སྲིད (unlikely (!chunk))
	{
	  env.set_error ();
	  ལོག;
	}

	དོན་ལུ (unsigned j = done; j < done + chunk; j++)
	{
	  const hb_array_t<const number_t> blends = env.argStack.sub_array (start + n + (j * k), k);
	  number_t v;
	  v.set_real (env.argStack[start + j].to_real () + fold (param, blends));
	  encoder.encode_num_tp (v);
	}
	དོན་ལུ (unsigned j = done; j < done + chunk; j++)
	{
	  const hb_array_t<const number_t> blends = env.argStack.sub_array (start + n + (j * k), k);
	  དོན་ལུ (unsigned r = 0; r < k_new; r++)
	  {
	    number_t v;
	    v.set_int (hb_clamp_to<ཧྲིལ> (round (t->transform_delta (r, blends))));
	    encoder.encode_num_tp (v);
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

  static void process_blend (cff2_priv_dict_interp_env_t& env, cff2_private_blend_encoder_param_t& param)
  {
    unsigned ཧྲིལ n, k;

    param.process_blend ();
    k = param.region_count;
    n = env.argStack.pop_uint ();

    unsigned count = env.argStack.get_count ();
    unsigned ཧྲིལ total;
    གལ་སྲིད (unlikely (hb_unsigned_mul_overflows (k + 1, n, &total) || total > count))
    {
      env.set_error ();
      ལོག;
    }

    /* copy the blend values into blend array of the default values */
    unsigned ཧྲིལ start = count - total;
    /* let an obvious error case fail, but note CFF2 spec doesn't forbid n==0 */
    གལ་སྲིད (unlikely (start > count))
    {
      env.set_error ();
      ལོག;
    }

    གལ་སྲིད (param.instancer && k)
    {
      rewrite_blends (env, param, start, n, k);
      ལོག;
    }

    དོན་ལུ (unsigned ཧྲིལ i = 0; i < n; i++)
    {
      const hb_array_t<const number_t> blends = env.argStack.sub_array (start + n + (i * k), k);
      process_arg_blend (param, env.argStack[start + i], blends, n, i);
    }

    /* pop off blend values leaving default values now adorned with blend values */
    env.argStack.pop (k * n);
  }

  static void process_op (op_code_t op, cff2_priv_dict_interp_env_t& env, cff2_private_blend_encoder_param_t& param)
  {
    switch (op) {
      pak_sa OpCode_StdHW:
      pak_sa OpCode_StdVW:
      pak_sa OpCode_BlueScale:
      pak_sa OpCode_BlueShift:
      pak_sa OpCode_BlueFuzz:
      pak_sa OpCode_ExpansionFactor:
      pak_sa OpCode_LanguageGroup:
      pak_sa OpCode_BlueValues:
      pak_sa OpCode_OtherBlues:
      pak_sa OpCode_FamilyBlues:
      pak_sa OpCode_FamilyOtherBlues:
      pak_sa OpCode_StemSnapH:
      pak_sa OpCode_StemSnapV:
	bha_nga;
      pak_sa OpCode_vsindexdict:
	env.process_vsindex ();
	param.ivs = env.get_ivs ();
	གལ་སྲིད (param.instancer)
	{
	  /* Re-emit with the remapped vsindex; 0 is the dict default. */
	  unsigned new_ivs = param.instancer->remap_ivs (param.ivs);
	  གལ་སྲིད (new_ivs)
	  {
	    str_buff_t str;
	    str_encoder_t encoder (str);
	    encoder.encode_int ((ཧྲིལ) new_ivs);
	    encoder.encode_op (OpCode_vsindexdict);
	    auto bytes = str.as_bytes ();
	    param.c->embed (&bytes, bytes.length);
	  }
	}
	env.clear_args ();
	ལོག;
      pak_sa OpCode_blenddict:
	process_blend (env, param);
	ལོག;

      default:
	dict_opset_t::process_op (op, env);
	གལ་སྲིད (!env.argStack.is_empty ()) ལོག;
	bha_nga;
    }

    གལ་སྲིད (unlikely (env.in_error ())) ལོག;

    // Write args then op

    str_buff_t str;
    str_encoder_t encoder (str);

    unsigned count = env.argStack.get_count ();
    དོན་ལུ (unsigned i = 0; i < count; i++)
      encoder.encode_num_tp (env.argStack[i]);

    encoder.encode_op (op);

    auto bytes = str.as_bytes ();
    param.c->embed (&bytes, bytes.length);

    env.clear_args ();
    param.fold_exact = param.fold_emitted = 0.;
  }
};

sa_nracanaa cff2_private_dict_op_serializer_t : op_serializer_t
{
  cff2_private_dict_op_serializer_t (bool desubroutinize_, bool drop_hints_, bool pinned_,
				     const CFF::CFF2ItemVariationStore* varStore_,
				     hb_array_t<ཧྲིལ> normalized_coords_,
				     const cff2_instancing_plan_t *instancer_ = nullptr)
    : desubroutinize (desubroutinize_), drop_hints (drop_hints_), pinned (pinned_),
      param (nullptr, varStore_, normalized_coords_)
  { param.instancer = instancer_; }

  bool serialize (hb_serialize_context_t *c,
		  const op_str_t &opstr,
		  objidx_t subrs_link) const
  {
    TRACE_SERIALIZE (this);

    གལ་སྲིད (drop_hints && dict_opset_t::is_hint_op (opstr.op))
      return_trace (true);

    གལ་སྲིད (opstr.op == OpCode_Subrs)
    {
      གལ་སྲིད (desubroutinize || !subrs_link)
	return_trace (true);
      དེ་མིན
	return_trace (FontDict::serialize_link2_op (c, opstr.op, subrs_link));
    }

    གལ་སྲིད (pinned || param.instancer)
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

  rak_sita:
  const bool desubroutinize;
  const bool drop_hints;
  const bool pinned;
  mutable cff2_private_blend_encoder_param_t param;
};



namespace OT {
sa_nracanaa cff2_subset_plan
{
  bool create (const OT::cff2::accelerator_subset_t &acc,
	      hb_subset_plan_t *plan)
  {
    /* make sure notdef is first */
    hb_codepoint_t old_glyph;
    གལ་སྲིད (!plan->old_gid_for_new_gid (0, &old_glyph) || (old_glyph != 0)) ལོག false;

    num_glyphs = plan->num_output_glyphs ();
    orig_fdcount = acc.fdArray->count;

    drop_hints = plan->flags & HB_SUBSET_FLAGS_NO_HINTING;
    bool instancing_requested = (bool) plan->normalized_coords;
    pinned = instancing_requested && plan->all_axes_pinned;
    normalized_coords = plan->normalized_coords;
    head_maxp_info = plan->head_maxp_info;
    hmtx_map = &plan->hmtx_map;
    desubroutinize = plan->flags & HB_SUBSET_FLAGS_DESUBROUTINIZE ||
		     instancing_requested; // For instancing we need this path

    /* Partial instancing: some axes pinned or limited, others kept. */
    partial_instancing = instancing_requested && !pinned &&
			 acc.varStore != &Null (CFF2ItemVariationStore);
    གལ་སྲིད (partial_instancing &&
	unlikely (!instancing.create (acc.varStore, plan)))
      ལོག false;
    axis_tags = &plan->axis_tags;

    /* Enable command capture for CFF2→CFF1 conversion (for specialization) */
    capture_commands = pinned;

 #ifdef HB_EXPERIMENTAL_API
    min_charstrings_off_size = (plan->flags & HB_SUBSET_FLAGS_IFTB_REQUIREMENTS) ? 4 : 0;
 #དེ་མིན
    min_charstrings_off_size = 0;
 #endif

    གལ་སྲིད (desubroutinize)
    {
      གལ་སྲིད (partial_instancing)
      {
	/* Pass 1: collect which VarDatas the retained blends reference, so
	 * unused ones get pruned and the survivors renumbered. */
	hb_set_t used_fds;
	དོན་ལུ (unsigned ཧྲིལ i = 0; i < num_glyphs; i++)
	{
	  hb_codepoint_t glyph;
	  གལ་སྲིད (!plan->old_gid_for_new_gid (i, &glyph))
	    continue;
	  const hb_ubytes_t str = (*acc.charStrings)[glyph];
	  unsigned ཧྲིལ fd = CFF::subset_get_fd (acc, glyph);
	  གལ་སྲིད (unlikely (fd >= acc.fdCount))
	    ལོག false;
	  used_fds.add (fd);

	  cff2_cs_interp_env_t<blend_arg_t> env (str, acc, fd);
	  cs_interpreter_t<cff2_cs_interp_env_t<blend_arg_t>, cff2_cs_opset_usage_t, cff2_usage_param_t> interp (env);
	  cff2_usage_param_t param;
	  param.instancing = &instancing;
	  param.drop_hints = drop_hints;
	  གལ་སྲིད (unlikely (!interp.interpret (param)))
	    ལོག false;
	}
	དོན་ལུ (unsigned fd : used_fds)
	{
	  cff2_usage_param_t param;
	  param.instancing = &instancing;
	  unsigned count = acc.privateDicts[fd].get_count ();
	  དོན་ལུ (unsigned j = 0; j < count; j++)
	  {
	    const op_str_t &opstr = acc.privateDicts[fd][j];
	    གལ་སྲིད (drop_hints && dict_opset_t::is_hint_op (opstr.op))
	      continue;
	    cff2_priv_dict_interp_env_t env {hb_ubytes_t (opstr.ptr, opstr.length)};
	    dict_interpreter_t<cff2_private_dict_usage_opset_t, cff2_usage_param_t, cff2_priv_dict_interp_env_t> interp (env);
	    གལ་སྲིད (unlikely (!interp.interpret (param)))
	      ལོག false;
	  }
	}
	གལ་སྲིད (unlikely (!instancing.finalize ()))
	  ལོག false;

	/* Pass 2: like subr_flattener_t, but blends stay symbolic (no coords
	 * fed to the env) and get rewritten against the instanced store. */
	གལ་སྲིད (!subset_charstrings.resize_exact (num_glyphs))
	  ལོག false;
	དོན་ལུ (unsigned ཧྲིལ i = 0; i < num_glyphs; i++)
	{
	  hb_codepoint_t glyph;
	  གལ་སྲིད (!plan->old_gid_for_new_gid (i, &glyph))
	    continue;
	  const hb_ubytes_t str = (*acc.charStrings)[glyph];
	  unsigned ཧྲིལ fd = CFF::subset_get_fd (acc, glyph);
	  གལ་སྲིད (unlikely (fd >= acc.fdCount))
	    ལོག false;

	  cff2_cs_interp_env_t<blend_arg_t> env (str, acc, fd);
	  cs_interpreter_t<cff2_cs_interp_env_t<blend_arg_t>, cff2_cs_opset_flatten_t, flatten_param_t> interp (env);
	  flatten_param_t param = {
	    subset_charstrings.arrayZ[i],
	    (bool) (plan->flags & HB_SUBSET_FLAGS_NO_HINTING),
	    plan
	  };
	  param.instancer = &instancing;
	  གལ་སྲིད (unlikely (!interp.interpret (param)))
	    ལོག false;
	}
      }
      དེ་མིན
      {
      /* Flatten global & local subrs */
      subr_flattener_t<const OT::cff2::accelerator_subset_t, cff2_cs_interp_env_t<blend_arg_t>, cff2_cs_opset_flatten_t>
		    flattener(acc, plan);

      /* Enable command capture if requested (for specialization) */
      གལ་སྲིད (capture_commands)
      {
	གལ་སྲིད (!charstring_commands.resize_exact (num_glyphs))
	  ལོག false;

	གལ་སྲིད (!flattener.flatten (subset_charstrings, &charstring_commands))
	  ལོག false;
      }
      དེ་མིན
      {
	གལ་སྲིད (!flattener.flatten (subset_charstrings))
	  ལོག false;
      }
      }
    }
    དེ་མིན
    {
      cff2_subr_subsetter_t	subr_subsetter (acc, plan);

      /* Subset subrs: collect used subroutines, leaving all unused ones behind */
      གལ་སྲིད (!subr_subsetter.subset ())
	ལོག false;

      /* encode charstrings, global subrs, local subrs with new subroutine numbers */
      གལ་སྲིད (!subr_subsetter.encode_charstrings (subset_charstrings, !pinned))
	ལོག false;

      གལ་སྲིད (!subr_subsetter.encode_globalsubrs (subset_globalsubrs))
	ལོག false;

      /* local subrs */
      གལ་སྲིད (!subset_localsubrs.resize (orig_fdcount))
	ལོག false;
      དོན་ལུ (unsigned ཧྲིལ fd = 0; fd < orig_fdcount; fd++)
      {
	subset_localsubrs[fd].init ();
	གལ་སྲིད (!subr_subsetter.encode_localsubrs (fd, subset_localsubrs[fd]))
	  ལོག false;
      }
    }

    /* FDSelect */
    གལ་སྲིད (acc.fdSelect != &Null (CFF2FDSelect))
    {
      གལ་སྲིད (unlikely (!hb_plan_subset_cff_fdselect (plan,
						  orig_fdcount,
						  *(const FDSelect *)acc.fdSelect,
						  acc.num_glyphs,
						  subset_fdcount,
						  subset_fdselect_size,
						  subset_fdselect_format,
						  subset_fdselect_ranges,
						  fdmap)))
	ལོག false;
    }
    དེ་མིན
      fdmap.identity (1);

    ལོག true;
  }

  cff2_sub_table_info_t info;

  unsigned ཧྲིལ    num_glyphs;
  unsigned ཧྲིལ    orig_fdcount = 0;
  unsigned ཧྲིལ    subset_fdcount = 1;
  unsigned ཧྲིལ    subset_fdselect_size = 0;
  unsigned ཧྲིལ    subset_fdselect_format = 0;
  bool            pinned = false;
  hb_vector_t<code_pair_t>   subset_fdselect_ranges;

  hb_inc_bimap_t   fdmap;

  str_buff_vec_t	    subset_charstrings;
  str_buff_vec_t	    subset_globalsubrs;
  hb_vector_t<str_buff_vec_t> subset_localsubrs;

  bool	    drop_hints = false;
  bool	    desubroutinize = false;

  bool	    partial_instancing = false;
  cff2_instancing_plan_t instancing;
  const hb_vector_t<hb_tag_t> *axis_tags = nullptr;

  unsigned  min_charstrings_off_size = 0;

  hb_array_t<ཧྲིལ> normalized_coords; // For instantiation
  head_maxp_info_t head_maxp_info;  // For FontBBox
  const hb_hashmap_t<hb_codepoint_t, hb_pair_t<unsigned, ཧྲིལ>> *hmtx_map; // For widths

  // Width optimization results (for CFF1 conversion)
  unsigned default_width = 0;
  unsigned nominal_width = 0;

  // Command capture for specialization (CFF2→CFF1 conversion)
  bool capture_commands = false;
  hb_vector_t<hb_vector_t<cs_command_t>> charstring_commands;
};
} // namespace OT

/*
 * CFF2 to CFF1 Converter Implementation
 */

#include "hb-cff-width-optimizer.hh"
#include "hb-cff-specializer.hh"

/* Serialize charstrings using CFF1 format with widths */
static bool
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
  གལ་སྲིད (unlikely (!cff1_charstrings.resize (plan.subset_charstrings.length)))
  {
    c->pop_discard ();
    ལོག false;
  }

  དོན་ལུ (unsigned i = 0; i < plan.subset_charstrings.length; i++)
  {
    // Get width for this glyph from hmtx_map
    unsigned width = 0;
    གལ་སྲིད (plan.hmtx_map->has (i))
      width = plan.hmtx_map->get (i).first;

    // Encode width if different from default
    str_encoder_t encoder (cff1_charstrings[i]);
    གལ་སྲིད (width != default_width)
    {
      ཧྲིལ delta = (ཧྲིལ) width - (ཧྲིལ) nominal_width;
      encoder.encode_int (delta);
    }

    // Use specialized commands if available, otherwise use binary
    གལ་སྲིད (plan.capture_commands && i < plan.charstring_commands.length &&
        plan.charstring_commands[i].length > 0)
    {
      // Specialize and encode commands
      auto &commands = plan.charstring_commands[i];
      CFF::specialize_commands (commands, 48);  /* maxstack=48 for CFF1 */
      གལ་སྲིད (unlikely (!CFF::encode_commands (commands, cff1_charstrings[i])))
      {
        c->pop_discard ();
        ལོག false;
      }
    }
    དེ་མིན
    {
      // Use binary CharString
      const str_buff_t &cs = plan.subset_charstrings[i];
      དོན་ལུ (unsigned j = 0; j < cs.length; j++)
        cff1_charstrings[i].push (cs[j]);
    }

    // Check if it already ends with endchar (0x0e) or return (0x0b)
    གལ་སྲིད (cff1_charstrings[i].length == 0 ||
        (cff1_charstrings[i].tail () != 0x0e && cff1_charstrings[i].tail () != 0x0b))
    {
      // Append endchar operator
      གལ་སྲིད (unlikely (!cff1_charstrings[i].push_or_fail (0x0e)))
      {
        c->pop_discard ();
        ལོག false;
      }
    }
  }

  unsigned data_size = 0;
  unsigned total_size = CFF1CharStrings::total_size (cff1_charstrings, &data_size, plan.min_charstrings_off_size);
  གལ་སྲིད (unlikely (!c->start_zerocopy (total_size)))
  {
    c->pop_discard ();
    ལོག false;
  }

  auto *cs = c->start_embed<CFF1CharStrings> ();
  གལ་སྲིད (unlikely (!cs->serialize (c, cff1_charstrings, &data_size, plan.min_charstrings_off_size)))
  {
    c->pop_discard ();
    ལོག false;
  }

  plan.info.char_strings_link = c->pop_pack (false);
  ལོག true;
}

/* Serialize CID Charset (format 2 range: gid 0-N -> cid 0-N) */
static bool
_serialize_cff1_charset (hb_serialize_context_t *c,
                         unsigned ཧྲིལ num_glyphs,
                         objidx_t &charset_link)
{
  // For CID fonts, create a simple identity charset
  // Format 2: one range covering all glyphs (except .notdef)
  c->push ();

  auto *charset = c->start_embed<Charset> ();
  གལ་སྲིད (unlikely (!charset))
  {
    c->pop_discard ();
    ལོག false;
  }

  // Create a single range for CID 1 to num_glyphs-1
  hb_vector_t<code_pair_t> ranges;
  གལ་སྲིད (num_glyphs > 1)
  {
    code_pair_t range;
    range.code = 1;  // first CID
    range.glyph = num_glyphs - 2;  // nLeft (covers glyphs 1 to num_glyphs-1)
    ranges.push (range);
  }

  གལ་སྲིད (unlikely (!charset->serialize (c, 2, num_glyphs, ranges)))
  {
    c->pop_discard ();
    ལོག false;
  }

  charset_link = c->pop_pack ();
  ལོག true;
}

/* CFF2 to CFF1 serialization */
namespace CFF {

bool
serialize_cff2_to_cff1 (hb_serialize_context_t *c,
                        OT::cff2_subset_plan &plan,
                        const cff2_top_dict_values_t &cff2_topDict,
                        const OT::cff2::accelerator_subset_t &acc)
{
  TRACE_SERIALIZE (this);

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
    widths.alloc (plan.num_glyphs);

    དོན་ལུ (unsigned gid = 0; gid < plan.num_glyphs; gid++)
    {
      unsigned width = 0;
      གལ་སྲིད (plan.hmtx_map->has (gid))
        width = plan.hmtx_map->get (gid).first;
      widths.push (width);
    }

    // Optimize defaultWidthX and nominalWidthX
    CFF::optimize_widths (widths, plan.default_width, plan.nominal_width);
  }

  // 1. CharStrings (with widths prepended)
  གལ་སྲིད (!_serialize_cff1_charstrings (c, plan, plan.default_width, plan.nominal_width))
    return_trace (false);

  // 2. Private DICs & Local Subrs (same as CFF2)
  hb_vector_t<table_info_t> private_dict_infos;
  གལ་སྲིད (unlikely (!private_dict_infos.resize (plan.subset_fdcount)))
    return_trace (false);

  དོན་ལུ (ཧྲིལ i = (ཧྲིལ)acc.privateDicts.length; --i >= 0;)
  {
    གལ་སྲིད (plan.fdmap.has (i))
    {
      objidx_t subrs_link = 0;

      གལ་སྲིད (plan.subset_localsubrs[i].length > 0)
      {
        auto *dest = c->push<CFF1Subrs> ();
        གལ་སྲིད (likely (dest->serialize (c, plan.subset_localsubrs[i])))
          subrs_link = c->pop_pack (false);
        དེ་མིན
        {
          c->pop_discard ();
          return_trace (false);
        }
      }

      auto *pd = c->push<PrivateDict> ();
      // Use the CFF2 Private DICT serializer which instantiates blends when pinned=true
      cff2_private_dict_op_serializer_t privSzr (plan.desubroutinize, plan.drop_hints, plan.pinned,
                                                  acc.varStore, plan.normalized_coords);
      གལ་སྲིད (likely (pd->serialize (c, acc.privateDicts[i], privSzr, subrs_link)))
      {
        // Add defaultWidthX and nominalWidthX for CFF1
        str_buff_t width_ops;
        str_encoder_t encoder (width_ops);
        encoder.encode_int (plan.default_width);
        encoder.encode_op (OpCode_defaultWidthX);
        encoder.encode_int (plan.nominal_width);
        encoder.encode_op (OpCode_nominalWidthX);

        གལ་སྲིད (!encoder.in_error () && c->embed (width_ops.as_bytes ().arrayZ, width_ops.length))
        {
          unsigned fd = plan.fdmap[i];
          private_dict_infos[fd].size = c->length ();
          private_dict_infos[fd].link = c->pop_pack ();
        }
        དེ་མིན
        {
          c->pop_discard ();
          return_trace (false);
        }
      }
      དེ་མིན
      {
        c->pop_discard ();
        return_trace (false);
      }
    }
  }

  // 3. FDArray - serialize CFF2 font dicts as CFF1
  {
    auto *fda = c->push<FDArray<HBUINT16>> ();
    cff_font_dict_op_serializer_t fontSzr;
    auto it =
    + hb_zip (+ hb_iter (acc.fontDicts)
              | hb_filter ([&] (const cff2_font_dict_values_t &_)
                { ལོག plan.fdmap.has (&_ - &acc.fontDicts[0]); }),
              hb_iter (private_dict_infos))
    ;
    // Explicitly specify template parameters: DICTVAL, INFO
    bool success = fda->serialize<cff2_font_dict_values_t, table_info_t> (c, it, fontSzr);
    གལ་སྲིད (success)
      plan.info.fd_array_link = c->pop_pack (false);
    དེ་མིན
    {
      c->pop_discard ();
      return_trace (false);
    }
  }

  // 4. FDSelect (required in CFF1 CID-keyed fonts)
  // CFF1 requires FDSelect for all CID-keyed fonts, even with just one FD
  // CFF2 makes it optional when there's only one FD
  གལ་སྲིད (acc.fdSelect != &Null (CFF2FDSelect))
  {
    c->push ();
    གལ་སྲིད (likely (hb_serialize_cff_fdselect (c, plan.num_glyphs,
                                          *(const FDSelect *)acc.fdSelect,
                                          plan.orig_fdcount,
                                          plan.subset_fdselect_format,
                                          plan.subset_fdselect_size,
                                          plan.subset_fdselect_ranges)))
      plan.info.fd_select.link = c->pop_pack ();
    དེ་མིན
    {
      c->pop_discard ();
      return_trace (false);
    }
  }
  དེ་མིན
  {
    // Create a range-based FDSelect3 mapping all glyphs to FD 0
    // Format: format(1) + nRanges(2) + range(3) + sentinel(2) = 8 bytes
    c->push ();

    // Format byte
    HBUINT8 format;
    format = 3;
    གལ་སྲིད (unlikely (!c->embed (format)))
    {
      c->pop_discard ();
      return_trace (false);
    }

    // nRanges
    HBUINT16 nRanges;
    nRanges = 1;
    གལ་སྲིད (unlikely (!c->embed (nRanges)))
    {
      c->pop_discard ();
      return_trace (false);
    }

    // Single range: {first: 0, fd: 0}
    FDSelect3_Range range;
    range.first = 0;
    range.fd = 0;
    གལ་སྲིད (unlikely (!c->embed (range)))
    {
      c->pop_discard ();
      return_trace (false);
    }

    // Sentinel (number of glyphs)
    HBUINT16 sentinel;
    sentinel = plan.num_glyphs;
    གལ་སྲིད (unlikely (!c->embed (sentinel)))
    {
      c->pop_discard ();
      return_trace (false);
    }

    plan.info.fd_select.link = c->pop_pack ();
  }

  // 5. Charset (CID charset for identity mapping)
  objidx_t charset_link;
  གལ་སྲིད (!_serialize_cff1_charset (c, plan.num_glyphs, charset_link))
    return_trace (false);

  // 6. Global Subrs
  {
    auto *dest = c->push<CFF1Subrs> ();
    གལ་སྲིད (likely (dest->serialize (c, plan.subset_globalsubrs)))
      c->pop_pack (false);
    དེ་མིན
    {
      c->pop_discard ();
      return_trace (false);
    }
  }

  // 7. String INDEX - Add "Adobe" and "Identity" for ROS operator
  {
    const var_na *adobe_str = "Adobe";
    const var_na *identity_str = "Identity";
    unsigned adobe_len = 5;  // strlen("Adobe")
    unsigned identity_len = 8;  // strlen("Identity")

    // Build strings array
    hb_vector_t<hb_ubytes_t> strings;
    strings.alloc (2);
    strings.push (hb_ubytes_t ((const unsigned var_na *) adobe_str, adobe_len));
    strings.push (hb_ubytes_t ((const unsigned var_na *) identity_str, identity_len));

    // Serialize as CFF INDEX
    auto *dest = c->push<CFF1Index> ();
    གལ་སྲིད (likely (dest->serialize (c, strings)))
      c->pop_pack (false);
    དེ་མིན
    {
      c->pop_discard ();
      return_trace (false);
    }
  }

  // 8. CFF Header
  OT::cff1 *cff = c->allocate_min<OT::cff1> ();
  གལ་སྲིད (unlikely (!cff)) return_trace (false);

  /* header */
  cff->version.major = 0x01;
  cff->version.minor = 0x00;
  cff->nameIndex = cff->min_size;
  cff->offSize = 4; /* unused? */

  // 9. Name INDEX (single entry)
  {
    unsigned name_len = strlen (CFF1_DEFAULT_FONT_NAME);

    CFF1Index *idx = c->start_embed<CFF1Index> ();
    གལ་སྲིད (unlikely (!idx)) return_trace (false);

    གལ་སྲིད (unlikely (!idx->serialize_header (c, hb_iter (&name_len, 1), name_len)))
      return_trace (false);

    གལ་སྲིད (unlikely (!c->embed (CFF1_DEFAULT_FONT_NAME, name_len)))
      return_trace (false);
  }

  // 10. Top DICT INDEX
  {
    // Serialize the Top DICT data first
    c->push<TopDict> ();
    cff1_from_cff2_top_dict_op_serializer_t topSzr;

    // Serialize ROS first
    གལ་སྲིད (unlikely (!topSzr.serialize_ros (c)))
    {
      c->pop_discard ();
      return_trace (false);
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

      གལ་སྲིད (encoder.in_error () || !c->embed (bbox_buff.as_bytes ().arrayZ, bbox_buff.length))
      {
        c->pop_discard ();
        return_trace (false);
      }
    }

    // Serialize charset operator
    གལ་སྲིད (charset_link && unlikely (!FontDict::serialize_link4_op (c, OpCode_charset, charset_link, whence_t::Absolute)))
    {
      c->pop_discard ();
      return_trace (false);
    }

    // Serialize FDSelect operator (required for CID-keyed CFF1 fonts)
    གལ་སྲིད (plan.info.fd_select.link && unlikely (!FontDict::serialize_link4_op (c, OpCode_FDSelect, plan.info.fd_select.link, whence_t::Absolute)))
    {
      c->pop_discard ();
      return_trace (false);
    }

    // Serialize FDArray operator (required for CID-keyed CFF1 fonts)
    གལ་སྲིད (plan.info.fd_array_link && unlikely (!FontDict::serialize_link4_op (c, OpCode_FDArray, plan.info.fd_array_link, whence_t::Absolute)))
    {
      c->pop_discard ();
      return_trace (false);
    }

    // Serialize other operators from CFF2 TopDict
    དོན་ལུ (const auto &opstr : cff2_topDict.values)
    {
      གལ་སྲིད (unlikely (!topSzr.serialize (c, opstr, plan.info)))
      {
        c->pop_discard ();
        return_trace (false);
      }
    }

    unsigned top_size = c->length ();
    c->pop_pack (false);

    // Serialize INDEX header
    auto *dest = c->start_embed<CFF1Index> ();
    གལ་སྲིད (unlikely (!dest->serialize_header (c, hb_iter (&top_size, 1), top_size)))
      return_trace (false);
  }

  return_trace (true);
}

} /* namespace CFF */

static bool _serialize_cff2_charstrings (hb_serialize_context_t *c,
			     cff2_subset_plan &plan,
			     const OT::cff2::accelerator_subset_t  &acc)
{
  c->push ();

  unsigned data_size = 0;
  unsigned total_size = CFF2CharStrings::total_size (plan.subset_charstrings, &data_size, plan.min_charstrings_off_size);
  གལ་སྲིད (unlikely (!c->start_zerocopy (total_size)))
    ལོག false;

  auto *cs = c->start_embed<CFF2CharStrings> ();
  གལ་སྲིད (unlikely (!cs->serialize (c, plan.subset_charstrings, &data_size, plan.min_charstrings_off_size)))
  {
    c->pop_discard ();
    ལོག false;
  }

  plan.info.char_strings_link = c->pop_pack (false);
  ལོག true;
}

bool
OT::cff2::accelerator_subset_t::serialize (hb_serialize_context_t *c,
					   sa_nracanaa cff2_subset_plan &plan,
					   hb_array_t<ཧྲིལ> normalized_coords) const
{
  /* push charstrings onto the object stack first which will ensure it packs as the last
     object in the table. Keeping the chastrings last satisfies the requirements for patching
     via IFTB. If this ordering needs to be changed in the future, charstrings should be left
     at the end whenever HB_SUBSET_FLAGS_ITFB_REQUIREMENTS is enabled. */
  གལ་སྲིད (!_serialize_cff2_charstrings(c, plan, *this))
    ལོག false;

  /* private dicts & local subrs */
  hb_vector_t<table_info_t>  private_dict_infos;
  གལ་སྲིད (unlikely (!private_dict_infos.resize (plan.subset_fdcount))) ལོག false;

  དོན་ལུ (ཧྲིལ i = (ཧྲིལ)privateDicts.length; --i >= 0 ;)
  {
    གལ་སྲིད (plan.fdmap.has (i))
    {
      objidx_t	subrs_link = 0;

      གལ་སྲིད (plan.subset_localsubrs[i].length > 0)
      {
	auto *dest = c->push <CFF2Subrs> ();
	གལ་སྲིད (likely (dest->serialize (c, plan.subset_localsubrs[i])))
	  subrs_link = c->pop_pack (false);
	དེ་མིན
	{
	  c->pop_discard ();
	  ལོག false;
	}
      }
      auto *pd = c->push<PrivateDict> ();
      cff2_private_dict_op_serializer_t privSzr (plan.desubroutinize, plan.drop_hints, plan.pinned,
						 varStore, normalized_coords,
						 plan.partial_instancing ? &plan.instancing : nullptr);
      གལ་སྲིད (likely (pd->serialize (c, privateDicts[i], privSzr, subrs_link)))
      {
	unsigned fd = plan.fdmap[i];
	private_dict_infos[fd].size = c->length ();
	private_dict_infos[fd].link = c->pop_pack ();
      }
      དེ་མིན
      {
	c->pop_discard ();
	ལོག false;
      }
    }
  }

  /* FDSelect */
  གལ་སྲིད (fdSelect != &Null (CFF2FDSelect))
  {
    c->push ();
    གལ་སྲིད (likely (hb_serialize_cff_fdselect (c, plan.num_glyphs, *(const FDSelect *)fdSelect,
					   plan.orig_fdcount,
					   plan.subset_fdselect_format, plan.subset_fdselect_size,
					   plan.subset_fdselect_ranges)))
      plan.info.fd_select.link = c->pop_pack ();
    དེ་མིན
    {
      c->pop_discard ();
      ལོག false;
    }
  }

  /* FDArray (FD Index) */
  {
    auto *fda = c->push<CFF2FDArray> ();
    cff_font_dict_op_serializer_t fontSzr;
    auto it =
    + hb_zip (+ hb_iter (fontDicts)
	      | hb_filter ([&] (const cff2_font_dict_values_t &_)
		{ ལོག plan.fdmap.has (&_ - &fontDicts[0]); }),
	      hb_iter (private_dict_infos))
    ;
    གལ་སྲིད (unlikely (!fda->serialize (c, it, fontSzr)))
    {
      c->pop_discard ();
      ལོག false;
    }
    plan.info.fd_array_link = c->pop_pack (false);
  }

  /* variation store */
  གལ་སྲིད (varStore != &Null (CFF2ItemVariationStore) &&
      !plan.pinned)
  {
    གལ་སྲིད (plan.partial_instancing)
    {
      གལ་སྲིད (plan.instancing.has_variations ())
      {
	c->push ();
	གལ་སྲིད (unlikely (!plan.instancing.serialize_var_store (c, *plan.axis_tags)))
	{
	  c->pop_discard ();
	  ལོག false;
	}
	plan.info.var_store_link = c->pop_pack (false);
      }
      /* else: the store emptied; the vstore op gets dropped along with it. */
    }
    དེ་མིན
    {
      auto *dest = c->push<CFF2ItemVariationStore> ();
      གལ་སྲིད (unlikely (!dest->serialize (c, varStore)))
      {
	c->pop_discard ();
	ལོག false;
      }
      plan.info.var_store_link = c->pop_pack (false);
    }
  }

  OT::cff2 *cff2 = c->allocate_min<OT::cff2> ();
  གལ་སྲིད (unlikely (!cff2)) ལོག false;

  /* header */
  cff2->version.major = 0x02;
  cff2->version.minor = 0x00;
  cff2->topDict = OT::cff2::static_size;

  /* top dict */
  {
    TopDict &dict = cff2 + cff2->topDict;
    cff2_top_dict_op_serializer_t topSzr;
    གལ་སྲིད (unlikely (!dict.serialize (c, topDict, topSzr, plan.info))) ལོག false;
    cff2->topDictSize = c->head - (const var_na *)&dict;
  }

  /* global subrs */
  {
    auto *dest = c->start_embed <CFF2Subrs> ();
    ལོག dest->serialize (c, plan.subset_globalsubrs);
  }
}

bool
OT::cff2::accelerator_subset_t::subset (hb_subset_context_t *c) const
{
  cff2_subset_plan cff2_plan;

  གལ་སྲིད (unlikely (!cff2_plan.create (*this, c->plan))) ལོག false;

  // If instantiating (pinned) and downgrade flag is set, convert to CFF1
  གལ་སྲིད (cff2_plan.pinned && (c->plan->flags & HB_SUBSET_FLAGS_DOWNGRADE_CFF2))
  {
    // Serialize CFF1 to the subsetter's serializer
    // If we run out of room, returning true will cause subsetter to retry with larger buffer
    bool result = CFF::serialize_cff2_to_cff1 (c->serializer, cff2_plan, topDict, *this);

    གལ་སྲིད (c->serializer->ran_out_of_room ())
      ལོག true; // Subsetter will retry with larger buffer

    གལ་སྲིད (result && !c->serializer->in_error ())
    {
      // Success - end serialization to resolve links
      c->serializer->end_serialize ();

      // Copy the serialized CFF1 data and add as CFF table
      hb_blob_t *cff_blob = c->serializer->copy_blob ();
      གལ་སྲིད (cff_blob)
      {
        c->plan->add_table (HB_TAG('C','F','F',' '), cff_blob);
        hb_blob_destroy (cff_blob);

        // Return false to signal CFF2 table is not needed
        ལོག false;
      }
    }

    // Conversion failed - don't fall back, fail hard for debugging
    ལོག false;
  }

  ལོག serialize (c->serializer, cff2_plan,
		    c->plan->normalized_coords.as_array ());
}

#endif
