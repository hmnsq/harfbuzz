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

#അന്തര്ഭാവ "hb.hh"

#ifndef HB_NO_SUBSET_CFF

#അന്തര്ഭാവ "hb-open-type.hh"
#അന്തര്ഭാവ "hb-ot-cff2-table.hh"
#അന്തര്ഭാവ "hb-ot-var-common.hh"
#അന്തര്ഭാവ "hb-set.h"
#അന്തര്ഭാവ "hb-subset-plan.hh"
#അന്തര്ഭാവ "hb-subset-cff-common.hh"
#അന്തര്ഭാവ "hb-cff2-interp-cs.hh"
#അന്തര്ഭാവ "hb-subset-cff2-to-cff1.hh"

using നാമാകാശ CFF;

നാമാകാശ CFF {

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
ഘടന cff2_instancing_plan_t
{
  ഘടന vardata_transform_t
  {
    /* new_deltas[j] = Σ_i matrix[j][i] * old_deltas[i] */
    hb_vector_t<hb_vector_t<ദശാംശം>> matrix;
    /* Default-value fold-in scalars, one per old region: the old store's
     * region scalars at the new default location. */
    hb_vector_t<ദശാംശം> gains;
    /* Surviving regions, parallel to matrix rows. */
    hb_vector_t<hb_hashmap_t<hb_tag_t, Triple>> regions;
    /* Index into the new global region list, per new region (finalize). */
    hb_vector_t<unsigned> region_indices;
    unsigned new_major = 0;
    തർക്കം alive_ = അസത്യം;
    തർക്കം alive () സ്ഥിരം { തിരികെ alive_; }

    /* One value's delta for new region j, from its old per-region deltas. */
    ഇരട്ട transform_delta (unsigned j, hb_array_t<സ്ഥിരം സംഖ്യ_t> deltas) സ്ഥിരം
    {
      സ്ഥിരം hb_vector_t<ദശാംശം> &col = matrix.arrayZ[j];
      ഇരട്ട d = 0.;
      unsigned എണ്ണം = hb_min (col.length, deltas.length);
      വേണ്ടി (unsigned c = 0; c < എണ്ണം; c++)
	d += (ഇരട്ട) col.arrayZ[c] * deltas.arrayZ[c].to_real ();
      തിരികെ d;
    }
  };

  hb_vector_t<vardata_transform_t> transforms;  /* per old VarData (vsindex) */
  hb_vector_t<hb_hashmap_t<hb_tag_t, Triple>> new_regions;  /* global region list */
  hb_set_t used_majors;
  unsigned new_major_എണ്ണം = 0;

  തർക്കം has_variations () സ്ഥിരം { തിരികെ new_major_എണ്ണം; }

  സ്ഥിരം vardata_transform_t *get_transform (unsigned major) സ്ഥിരം
  { തിരികെ major < transforms.length ? &transforms.arrayZ[major] : nullptr; }

  /* Record that a retained blend references this VarData. */
  ശൂന്യം note_use (unsigned major) { used_majors.add (major); }

  /* The new vsindex a private dict's (or charstring's) old ivs maps to;
   * dead VarDatas fall back to 0, their blends having become static. */
  unsigned remap_ivs (unsigned old_ivs) സ്ഥിരം
  {
    സ്ഥിരം vardata_transform_t *t = get_transform (old_ivs);
    തിരികെ (t && t->alive ()) ? t->new_major : 0;
  }

  ഇരട്ട fold_deltas (unsigned old_ivs, hb_array_t<സ്ഥിരം സംഖ്യ_t> deltas) സ്ഥിരം
  {
    സ്ഥിരം vardata_transform_t *t = get_transform (old_ivs);
    എങ്കിൽ (unlikely (!t || t->gains.length != deltas.length)) തിരികെ 0.;
    ഇരട്ട v = 0.;
    വേണ്ടി (unsigned i = 0; i < deltas.length; i++)
      v += (ഇരട്ട) t->gains.arrayZ[i] * deltas.arrayZ[i].to_real ();
    തിരികെ v;
  }

  തർക്കം create (സ്ഥിരം CFF2ItemVariationStore *cff2VarStore,
	       സ്ഥിരം hb_subset_plan_t *plan)
  {
    സ്ഥിരം OT::ItemVariationStore &varStore = cff2VarStore->varStore;
    unsigned major_എണ്ണം = varStore.get_sub_table_എണ്ണം ();
    എങ്കിൽ (!transforms.resize (major_എണ്ണം)) തിരികെ അസത്യം;

    എങ്കിൽ (!varStore.get_region_list ().get_var_regions (plan->axes_old_index_tag_map, orig_regions))
      തിരികെ അസത്യം;

    /* Instance each VarData with identity delta rows; the surviving tuples'
     * delta columns are the old-region → new-region transform. */
    OT::optimize_scratch_t scratch;
    വേണ്ടി (unsigned m = 0; m < major_എണ്ണം; m++)
    {
      സ്ഥിരം OT::VarData &var_data = varStore.get_sub_table (m);
      unsigned k = var_data.get_region_index_എണ്ണം ();
      vardata_transform_t &t = transforms.arrayZ[m];

      എങ്കിൽ (!t.gains.resize_exact (k)) തിരികെ അസത്യം;
      varStore.get_region_scalars (m, plan->normalized_coords.arrayZ,
				   plan->normalized_coords.length,
				   t.gains.arrayZ, k);

      OT::tuple_variations_t tuples;
      വേണ്ടി (unsigned r = 0; r < k; r++)
      {
	OT::tuple_delta_t tuple;
	എങ്കിൽ (!tuple.deltas_x.resize (k) ||
	    !tuple.indices.resize (k))
	  തിരികെ അസത്യം;
	വേണ്ടി (unsigned i = 0; i < k; i++)
	  tuple.indices.arrayZ[i] = സത്യം;
	tuple.deltas_x.arrayZ[r] = 1.f;
	unsigned region_index = var_data.get_region_index (r);
	എങ്കിൽ (unlikely (region_index >= orig_regions.length)) തിരികെ അസത്യം;
	tuple.axis_tuples = orig_regions.arrayZ[region_index];
	എങ്കിൽ (unlikely (tuple.axis_tuples.in_error ())) തിരികെ അസത്യം;
	tuples.tuple_vars.push (std::move (tuple));
      }
      എങ്കിൽ (unlikely (tuples.tuple_vars.in_error ())) തിരികെ അസത്യം;

      എങ്കിൽ (!tuples.instantiate (plan->axes_location, plan->axes_triple_distances, scratch))
	തിരികെ അസത്യം;

      വേണ്ടി (auto &tuple : tuples.tuple_vars)
      {
	എങ്കിൽ (unlikely (tuple.deltas_x.length != k)) തിരികെ അസത്യം;
	hb_vector_t<ദശാംശം> col;
	എങ്കിൽ (unlikely (!col.resize_exact (k))) തിരികെ അസത്യം;
	വേണ്ടി (unsigned i = 0; i < k; i++)
	  col.arrayZ[i] = tuple.deltas_x.arrayZ[i];
	t.matrix.push (std::move (col));
	t.regions.push (std::move (tuple.axis_tuples));
      }
      എങ്കിൽ (unlikely (t.matrix.in_error () || t.regions.in_error ())) തിരികെ അസത്യം;
    }

    തിരികെ സത്യം;
  }

  /* Decide which VarDatas survive, number them, and build the new global
   * region list: surviving original regions first, in their original order,
   * then solver-split new regions in order of appearance (fontTools'
   * rebuildRegions + prune_regions). */
  തർക്കം finalize ()
  {
    വേണ്ടി (unsigned m = 0; m < transforms.length; m++)
    {
      vardata_transform_t &t = transforms.arrayZ[m];
      t.alive_ = t.matrix.length && used_majors.has (m);
      എങ്കിൽ (t.alive_)
	t.new_major = new_major_എണ്ണം++;
    }

    /* Keys hash by content, so pointers into different owners compare fine. */
    hb_hashmap_t<സ്ഥിരം hb_hashmap_t<hb_tag_t, Triple>*, unsigned> region_idx_map;
    hb_vector_t<സ്ഥിരം hb_hashmap_t<hb_tag_t, Triple>*> region_ptrs;
    വേണ്ടി (സ്ഥിരം vardata_transform_t &t : transforms)
    {
      എങ്കിൽ (!t.alive ()) തുടരുക;
      വേണ്ടി (സ്ഥിരം auto &region : t.regions)
	എങ്കിൽ (!region_idx_map.has (&region))
	  എങ്കിൽ (unlikely (!region_idx_map.set (&region, (unsigned) -1)))
	    തിരികെ അസത്യം;
    }
    വേണ്ടി (സ്ഥിരം auto &r : orig_regions)
    {
      unsigned *idx;
      എങ്കിൽ (region_idx_map.has (&r, &idx) && *idx == (unsigned) -1)
      {
	എങ്കിൽ (unlikely (!region_idx_map.set (&r, region_ptrs.length))) തിരികെ അസത്യം;
	region_ptrs.push (&r);
      }
    }
    വേണ്ടി (സ്ഥിരം vardata_transform_t &t : transforms)
    {
      എങ്കിൽ (!t.alive ()) തുടരുക;
      വേണ്ടി (സ്ഥിരം auto &region : t.regions)
      {
	unsigned *idx;
	എങ്കിൽ (region_idx_map.has (&region, &idx) && *idx == (unsigned) -1)
	{
	  എങ്കിൽ (unlikely (!region_idx_map.set (&region, region_ptrs.length))) തിരികെ അസത്യം;
	  region_ptrs.push (&region);
	}
      }
    }
    എങ്കിൽ (unlikely (region_ptrs.in_error ())) തിരികെ അസത്യം;

    വേണ്ടി (vardata_transform_t &t : transforms)
    {
      എങ്കിൽ (!t.alive ()) തുടരുക;
      വേണ്ടി (സ്ഥിരം auto &region : t.regions)
      {
	unsigned *idx;
	എങ്കിൽ (unlikely (!region_idx_map.has (&region, &idx))) തിരികെ അസത്യം;
	t.region_indices.push (*idx);
      }
      എങ്കിൽ (unlikely (t.region_indices.in_error ())) തിരികെ അസത്യം;
    }

    എങ്കിൽ (!new_regions.resize (region_ptrs.length)) തിരികെ അസത്യം;
    വേണ്ടി (unsigned i = 0; i < region_ptrs.length; i++)
    {
      new_regions.arrayZ[i] = *region_ptrs.arrayZ[i];
      എങ്കിൽ (unlikely (new_regions.arrayZ[i].in_error ())) തിരികെ അസത്യം;
    }

    തിരികെ സത്യം;
  }

  /* Serialize the instanced store: a CFF2ItemVariationStore whose VarDatas
   * carry no items, just region indexes (blend deltas live in charstrings).
   * Laid out manually since all sizes are known up front. */
  തർക്കം serialize_var_store (hb_serialize_context_t *c,
			    സ്ഥിരം hb_vector_t<hb_tag_t> &axis_tags) സ്ഥിരം
  {
    unsigned data_എണ്ണം = new_major_എണ്ണം;
    unsigned header_size = 2 + 4 + 2 + 4 * data_എണ്ണം;

    uint64_t offset = header_size;
    hb_vector_t<unsigned> data_offsets;
    വേണ്ടി (സ്ഥിരം vardata_transform_t &t : transforms)
    {
      എങ്കിൽ (!t.alive ()) തുടരുക;
      data_offsets.push ((unsigned) offset);
      offset += 6 + 2 * (uint64_t) t.region_indices.length;
    }
    എങ്കിൽ (unlikely (data_offsets.in_error () || data_offsets.length != data_എണ്ണം))
      തിരികെ അസത്യം;
    unsigned regions_offset = (unsigned) offset;
    uint64_t total = offset + 4 + (uint64_t) new_regions.length * axis_tags.length * 6;
    എങ്കിൽ (unlikely (total > 0xFFFFFFFFu)) തിരികെ അസത്യം;

    HBUINT16 *size_field = c->allocate_size<HBUINT16> (HBUINT16::static_size);
    എങ്കിൽ (unlikely (!size_field)) തിരികെ അസത്യം;
    /* OFF 2026 5.3.3.11.2 permits larger stores with length=65535. */
    *size_field = hb_min (total, (uint64_t) 0xFFFFu);

    അക്ഷരം *header = c->allocate_size<അക്ഷരം> (header_size);
    എങ്കിൽ (unlikely (!header)) തിരികെ അസത്യം;
    * (HBUINT16 *) (header + 0) = 1;			/* format */
    * (HBUINT32 *) (header + 2) = regions_offset;
    * (HBUINT16 *) (header + 6) = data_എണ്ണം;
    വേണ്ടി (unsigned i = 0; i < data_എണ്ണം; i++)
      * (HBUINT32 *) (header + 8 + 4 * i) = data_offsets.arrayZ[i];

    വേണ്ടി (സ്ഥിരം vardata_transform_t &t : transforms)
    {
      എങ്കിൽ (!t.alive ()) തുടരുക;
      unsigned k = t.region_indices.length;
      HBUINT16 *v = (HBUINT16 *) c->allocate_size<അക്ഷരം> (6 + 2 * k);
      എങ്കിൽ (unlikely (!v)) തിരികെ അസത്യം;
      v[0] = 0;						/* itemCount */
      v[1] = 0;						/* wordSizeCount */
      v[2] = k;						/* regionIndices.len */
      വേണ്ടി (unsigned j = 0; j < k; j++)
	v[3 + j] = t.region_indices.arrayZ[j];
    }

    hb_vector_t<സ്ഥിരം hb_hashmap_t<hb_tag_t, Triple>*> region_ptrs;
    എങ്കിൽ (unlikely (!region_ptrs.alloc_exact (new_regions.length))) തിരികെ അസത്യം;
    വേണ്ടി (സ്ഥിരം auto &r : new_regions)
      region_ptrs.push (&r);
    auto *region_list = c->start_embed<OT::VarRegionList> ();
    തിരികെ region_list->serialize (c, axis_tags, region_ptrs);
  }

  സ്വകാര്യ:
  hb_vector_t<hb_hashmap_t<hb_tag_t, Triple>> orig_regions;
};

/* Usage-collection pass for partial instancing: record which VarDatas the
 * retained charstrings' and private dicts' blends reference.  Mirrors the
 * emission pass: blends consumed by dropped hint ops don't count. */
ഘടന cff2_usage_param_t
{
  ശൂന്യം init () {}

  cff2_instancing_plan_t *instancing = nullptr;
  തർക്കം drop_hints = അസത്യം;
  unsigned ivs = 0;	/* dict-side current vsindex */
};

ഘടന cff2_cs_opset_usage_t : cff2_cs_opset_t<cff2_cs_opset_usage_t, cff2_usage_param_t, blend_arg_t>
{
  സ്ഥിതം ശൂന്യം flush_args_and_op (op_code_t op, cff2_cs_interp_env_t<blend_arg_t> &env, cff2_usage_param_t& param)
  {
    തർക്കം counts = സത്യം;
    തിരഞ്ഞെടുപ്പ് (op)
    {
      അവസ്ഥ OpCode_return:
      അവസ്ഥ OpCode_endchar:
	counts = അസത്യം;
	നിർത്തുക;

      അവസ്ഥ OpCode_hstem:
      അവസ്ഥ OpCode_hstemhm:
      അവസ്ഥ OpCode_vstem:
      അവസ്ഥ OpCode_vstemhm:
      അവസ്ഥ OpCode_hintmask:
      അവസ്ഥ OpCode_cntrmask:
	എങ്കിൽ (param.drop_hints)
	  counts = അസത്യം;
	നിർത്തുക;

      സ്ഥിരസ്ഥിതി:
	നിർത്തുക;
    }

    എങ്കിൽ (counts)
      വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < env.arg_അട്ടി.get_എണ്ണം (); i++)
	എങ്കിൽ (env.arg_അട്ടി[i].blending ())
	{
	  param.instancing->note_use (env.get_ivs ());
	  നിർത്തുക;
	}

    env.clear_args ();
  }
};

ഘടന cff2_private_dict_usage_opset_t : dict_opset_t
{
  സ്ഥിതം ശൂന്യം process_op (op_code_t op, cff2_priv_dict_interp_env_t& env, cff2_usage_param_t& param)
  {
    തിരഞ്ഞെടുപ്പ് (op) {
      അവസ്ഥ OpCode_vsindexdict:
	env.process_vsindex ();
	param.ivs = env.get_ivs ();
	env.clear_args ();
	തിരികെ;
      അവസ്ഥ OpCode_blenddict:
	param.instancing->note_use (param.ivs);
	env.clear_args ();
	തിരികെ;
      സ്ഥിരസ്ഥിതി:
	dict_opset_t::process_op (op, env);
	എങ്കിൽ (!env.arg_അട്ടി.is_empty ()) തിരികെ;
	env.clear_args ();
	തിരികെ;
    }
  }
};

} /* namespace CFF */

ഘടന cff2_sub_table_info_t : cff_sub_table_info_t
{
  cff2_sub_table_info_t ()
    : cff_sub_table_info_t (),
      var_store_link (0)
  {}

  objidx_t  var_store_link;
};

ഘടന cff2_മുകൾ_dict_op_serializer_t : cff_മുകൾ_dict_op_serializer_t<>
{
  തർക്കം serialize (hb_serialize_context_t *c,
		  സ്ഥിരം op_str_t &opstr,
		  സ്ഥിരം cff2_sub_table_info_t &info) സ്ഥിരം
  {
    TRACE_SERIALIZE (ഇത്);

    തിരഞ്ഞെടുപ്പ് (opstr.op)
    {
      അവസ്ഥ OpCode_vstore:
        എങ്കിൽ (info.var_store_link)
	  return_trace (FontDict::serialize_link4_op(c, opstr.op, info.var_store_link));
	അല്ലെങ്കിൽ
	  return_trace (സത്യം);

      സ്ഥിരസ്ഥിതി:
	return_trace (cff_മുകൾ_dict_op_serializer_t<>::serialize (c, opstr, info));
    }
  }
};

ഘടന cff2_cs_opset_flatten_t : cff2_cs_opset_t<cff2_cs_opset_flatten_t, flatten_param_t, blend_arg_t>
{
  സ്ഥിതം ശൂന്യം flush_args_and_op (op_code_t op, cff2_cs_interp_env_t<blend_arg_t> &env, flatten_param_t& param)
  {
    /* Optionally capture command for specialization (before flushing, to preserve args) */
    എങ്കിൽ (param.commands)
    {
      തർക്കം skip_command = അസത്യം;

      തിരഞ്ഞെടുപ്പ് (op)
      {
	അവസ്ഥ OpCode_return:
	അവസ്ഥ OpCode_endchar:
	  skip_command = സത്യം;
	  നിർത്തുക;

	അവസ്ഥ OpCode_hstem:
	അവസ്ഥ OpCode_hstemhm:
	അവസ്ഥ OpCode_vstem:
	അവസ്ഥ OpCode_vstemhm:
	അവസ്ഥ OpCode_hintmask:
	അവസ്ഥ OpCode_cntrmask:
	  എങ്കിൽ (param.drop_hints)
	    skip_command = സത്യം;
	  നിർത്തുക;

	സ്ഥിരസ്ഥിതി:
	  നിർത്തുക;
      }

      എങ്കിൽ (!skip_command)
      {
	cs_command_t cmd (op);
	/* Capture resolved blend values */
	വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < env.arg_അട്ടി.get_എണ്ണം ();)
	{
	  സ്ഥിരം blend_arg_t &arg = env.arg_അട്ടി[i];
	  എങ്കിൽ (arg.blending ())
	  {
	    /* For blend args, capture only the resolved default value */
	    cmd.args.push (arg);
	    /* Skip over the multiple blend values */
	    i += arg.അക്കം_മൂല്യങ്ങൾ_2;
	  }
	  അല്ലെങ്കിൽ
	  {
	    cmd.args.push (arg);
	    i++;
	  }
	}
	param.commands->push (cmd);
      }
    }

    തിരഞ്ഞെടുപ്പ് (op)
    {
      അവസ്ഥ OpCode_return:
      അവസ്ഥ OpCode_endchar:
	/* dummy opcodes in CFF2. ignore */
	എങ്കിൽ (op == OpCode_endchar && param.instancer)
	  emit_vsindex_prefix (env, param);
	നിർത്തുക;

      അവസ്ഥ OpCode_hstem:
      അവസ്ഥ OpCode_hstemhm:
      അവസ്ഥ OpCode_vstem:
      അവസ്ഥ OpCode_vstemhm:
      അവസ്ഥ OpCode_hintmask:
      അവസ്ഥ OpCode_cntrmask:
	എങ്കിൽ (param.drop_hints)
	{
	  env.clear_args ();
	  തിരികെ;
	}
	HB_FALLTHROUGH;

      സ്ഥിരസ്ഥിതി:
	SUPER::flush_args_and_op (op, env, param);
	നിർത്തുക;
    }
  }

  സ്ഥിതം ശൂന്യം flush_args (cff2_cs_interp_env_t<blend_arg_t> &env, flatten_param_t& param)
  {
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < env.arg_അട്ടി.get_എണ്ണം ();)
    {
      സ്ഥിരം blend_arg_t &arg = env.arg_അട്ടി[i];
      എങ്കിൽ (arg.blending ())
      {
	എങ്കിൽ (unlikely (!((arg.അക്കം_മൂല്യങ്ങൾ_2 > 0) && (env.arg_അട്ടി.get_എണ്ണം () >= arg.അക്കം_മൂല്യങ്ങൾ_2))))
	{
	  env.set_error ();
	  തിരികെ;
	}
	എങ്കിൽ (param.instancer)
	  rewrite_blends (arg, i, env, param);
	അല്ലെങ്കിൽ
	  flatten_blends (arg, i, env, param);
	i += arg.അക്കം_മൂല്യങ്ങൾ_2;
      }
      അല്ലെങ്കിൽ
      {
	str_encoder_t  encoder (param.flatStr);
	encoder.encode_അക്കം_cs (arg);
	i++;
      }
    }
    SUPER::flush_args (env, param);
  }

  സ്ഥിതം ശൂന്യം flatten_blends (സ്ഥിരം blend_arg_t &arg, unsigned പൂർണ്ണസംഖ്യ i, cff2_cs_interp_env_t<blend_arg_t> &env, flatten_param_t& param)
  {
    /* flatten the default values */
    str_encoder_t  encoder (param.flatStr);
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ j = 0; j < arg.അക്കം_മൂല്യങ്ങൾ_2; j++)
    {
      സ്ഥിരം blend_arg_t &arg1 = env.arg_അട്ടി[i + j];
      എങ്കിൽ (unlikely (!((arg1.blending () && (arg.അക്കം_മൂല്യങ്ങൾ_2 == arg1.അക്കം_മൂല്യങ്ങൾ_2) && (arg1.മൂല്യം_Index_2 == j) &&
	      (arg1.deltas.length == env.get_region_എണ്ണം ())))))
      {
	env.set_error ();
	തിരികെ;
      }
      encoder.encode_അക്കം_cs (arg1);
    }
    /* flatten deltas for each value */
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ j = 0; j < arg.അക്കം_മൂല്യങ്ങൾ_2; j++)
    {
      സ്ഥിരം blend_arg_t &arg1 = env.arg_അട്ടി[i + j];
      വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ k = 0; k < arg1.deltas.length; k++)
	encoder.encode_അക്കം_cs (arg1.deltas[k]);
    }
    /* flatten the number of values followed by blend operator */
    encoder.encode_int (arg.അക്കം_മൂല്യങ്ങൾ_2);
    encoder.encode_op (OpCode_blendcs);
  }

  /* Partial instancing: rewrite one blend group against the instanced store.
   * Each value's default absorbs the folded (pinned-away) contribution, and
   * its deltas are mapped onto the new region list. */
  സ്ഥിതം ശൂന്യം rewrite_blends (സ്ഥിരം blend_arg_t &arg, unsigned പൂർണ്ണസംഖ്യ i, cff2_cs_interp_env_t<blend_arg_t> &env, flatten_param_t& param)
  {
    സ്ഥിരം cff2_instancing_plan_t &instancer = *param.instancer;
    unsigned ivs = env.get_ivs ();
    സ്ഥിരം cff2_instancing_plan_t::vardata_transform_t *t = instancer.get_transform (ivs);
    എങ്കിൽ (unlikely (!t))
    {
      env.set_error ();
      തിരികെ;
    }

    unsigned n = arg.അക്കം_മൂല്യങ്ങൾ_2;
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ j = 0; j < n; j++)
    {
      സ്ഥിരം blend_arg_t &arg1 = env.arg_അട്ടി[i + j];
      എങ്കിൽ (unlikely (!(arg1.blending () && (arg1.അക്കം_മൂല്യങ്ങൾ_2 == n) && (arg1.മൂല്യം_Index_2 == j) &&
		      (arg1.deltas.length == env.get_region_എണ്ണം ()) &&
		      (arg1.deltas.length == t->gains.length))))
      {
	env.set_error ();
	തിരികെ;
      }
    }

    str_encoder_t encoder (param.flatStr);
    unsigned k_new = t->alive () ? t->matrix.length : 0;

    എങ്കിൽ (!k_new)
    {
      /* All of this VarData's regions died; blends dissolve into statics. */
      വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ j = 0; j < n; j++)
      {
	സ്ഥിരം blend_arg_t &arg1 = env.arg_അട്ടി[i + j];
	സംഖ്യ_t v;
	v.set_real (arg1.to_real () + round (instancer.fold_deltas (ivs, arg1.deltas.as_array ())));
	encoder.encode_അക്കം_cs (v);
      }
      തിരികെ;
    }

    /* Emit in chunks so the output charstring stays within the argument
     * stack limit: values already emitted for this op, plus this chunk's
     * defaults, deltas and count, must not exceed 513. */
    unsigned done = 0;
    ആകുമ്പോൾ (done < n)
    {
      unsigned avail = 512 > (i + done) ? 512 - (i + done) : 0;
      unsigned chunk = hb_min (avail / (k_new + 1), n - done);
      എങ്കിൽ (unlikely (!chunk))
      {
	env.set_error ();
	തിരികെ;
      }

      വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ j = done; j < done + chunk; j++)
      {
	സ്ഥിരം blend_arg_t &arg1 = env.arg_അട്ടി[i + j];
	സംഖ്യ_t v;
	v.set_real (arg1.to_real () + round (instancer.fold_deltas (ivs, arg1.deltas.as_array ())));
	encoder.encode_അക്കം_cs (v);
      }
      വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ j = done; j < done + chunk; j++)
      {
	സ്ഥിരം blend_arg_t &arg1 = env.arg_അട്ടി[i + j];
	വേണ്ടി (unsigned r = 0; r < k_new; r++)
	{
	  സംഖ്യ_t v;
	  v.set_int (hb_clamp_to<പൂർണ്ണസംഖ്യ> (round (t->transform_delta (r, arg1.deltas.as_array ()))));
	  encoder.encode_അക്കം_cs (v);
	}
      }
      encoder.encode_int (chunk);
      encoder.encode_op (OpCode_blendcs);
      done += chunk;
    }
    param.emitted_blend = സത്യം;
  }

  /* Partial instancing: prepend a vsindex op when the charstring's
   * (remapped) ivs differs from its FD's (remapped) private-dict ivs. */
  സ്ഥിതം ശൂന്യം emit_vsindex_prefix (cff2_cs_interp_env_t<blend_arg_t> &env, flatten_param_t& param)
  {
    എങ്കിൽ (!param.emitted_blend) തിരികെ;
    സ്ഥിരം cff2_instancing_plan_t &instancer = *param.instancer;
    unsigned new_ivs = instancer.remap_ivs (env.get_ivs ());
    unsigned fd_ivs = instancer.remap_ivs (env.get_orig_ivs ());
    എങ്കിൽ (new_ivs == fd_ivs) തിരികെ;

    str_buff_t prefix;
    str_encoder_t enc (prefix);
    enc.encode_int ((പൂർണ്ണസംഖ്യ) new_ivs);
    enc.encode_op (OpCode_vsindexcs);

    str_buff_t &buf = param.flatStr;
    unsigned old_len = buf.length;
    എങ്കിൽ (unlikely (prefix.in_error () || !buf.resize (old_len + prefix.length)))
    {
      env.set_error ();
      തിരികെ;
    }
    memmove (buf.arrayZ + prefix.length, buf.arrayZ, old_len);
    hb_memcpy (buf.arrayZ, prefix.arrayZ, prefix.length);
  }

  സ്ഥിതം ശൂന്യം flush_op (op_code_t op, cff2_cs_interp_env_t<blend_arg_t> &env, flatten_param_t& param)
  {
    തിരഞ്ഞെടുപ്പ് (op)
    {
      അവസ്ഥ OpCode_return:
      അവസ്ഥ OpCode_endchar:
	തിരികെ;
      സ്ഥിരസ്ഥിതി:
	str_encoder_t  encoder (param.flatStr);
	encoder.encode_op (op);
    }
  }

  സ്ഥിതം ശൂന്യം flush_hintmask (op_code_t op, cff2_cs_interp_env_t<blend_arg_t> &env, flatten_param_t& param)
  {
    SUPER::flush_hintmask (op, env, param);
    /* Preserve hintmask payload in captured commands for specializer re-encoding. */
    എങ്കിൽ (param.commands && !param.drop_hints && param.commands->length > 0)
    {
      auto &cmd = param.commands->tail ();
      എങ്കിൽ (cmd.op == op)
      {
        cmd.mask_bytes.resize (env.hintmask_size);
        എങ്കിൽ (unlikely (cmd.mask_bytes.in_error ()))
        {
          env.set_error ();
          തിരികെ;
        }
        വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < env.hintmask_size; i++)
          cmd.mask_bytes[i] = env.str_ref[i];
      }
    }
    എങ്കിൽ (!param.drop_hints)
    {
      str_encoder_t  encoder (param.flatStr);
      വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < env.hintmask_size; i++)
	encoder.encode_byte (env.str_ref[i]);
    }
  }

  സ്വകാര്യ:
  തരനിർവചനം cff2_cs_opset_t<cff2_cs_opset_flatten_t, flatten_param_t, blend_arg_t> SUPER;
  തരനിർവചനം cs_opset_t<blend_arg_t, cff2_cs_opset_flatten_t, cff2_cs_opset_flatten_t, cff2_cs_interp_env_t<blend_arg_t>, flatten_param_t> CSOPSET;
};

ഘടന cff2_cs_opset_subr_subset_t : cff2_cs_opset_t<cff2_cs_opset_subr_subset_t, subr_subset_param_t, blend_arg_t>
{
  സ്ഥിതം ശൂന്യം process_op (op_code_t op, cff2_cs_interp_env_t<blend_arg_t> &env, subr_subset_param_t& param)
  {
    തിരഞ്ഞെടുപ്പ് (op) {

      അവസ്ഥ OpCode_return:
	param.current_parsed_str->flush_coalesced (env.str_ref);
	param.current_parsed_str->set_parsed ();
	env.return_from_subr ();
	param.set_current_str (env, അസത്യം);
	നിർത്തുക;

      അവസ്ഥ OpCode_endchar:
	param.current_parsed_str->flush_coalesced (env.str_ref);
	param.current_parsed_str->set_parsed ();
	SUPER::process_op (op, env, param);
	നിർത്തുക;

      അവസ്ഥ OpCode_callsubr:
	process_call_subr (op, CSType_LocalSubr, env, param, env.localSubrs, param.local_closure);
	നിർത്തുക;

      അവസ്ഥ OpCode_callgsubr:
	process_call_subr (op, CSType_GlobalSubr, env, param, env.globalSubrs, param.global_closure);
	നിർത്തുക;

      സ്ഥിരസ്ഥിതി:
	SUPER::process_op (op, env, param);
	param.current_parsed_str->add_op (op, env.str_ref);
	നിർത്തുക;
    }
  }

  സംരക്ഷിത:
  സ്ഥിതം ശൂന്യം process_call_subr (op_code_t op, cs_type_t type,
				 cff2_cs_interp_env_t<blend_arg_t> &env, subr_subset_param_t& param,
				 cff2_biased_subrs_t& subrs, hb_set_t *closure)
  {
    byte_str_ref_t    str_ref = env.str_ref;
    env.call_subr (subrs, type);
    param.current_parsed_str->add_call_op (op, str_ref, env.context.subr_അക്കം_2);
    closure->add (env.context.subr_അക്കം_2);
    param.set_current_str (env, സത്യം);
  }

  സ്വകാര്യ:
  തരനിർവചനം cff2_cs_opset_t<cff2_cs_opset_subr_subset_t, subr_subset_param_t, blend_arg_t> SUPER;
};

ഘടന cff2_subr_subsetter_t : subr_subsetter_t<cff2_subr_subsetter_t, CFF2Subrs, സ്ഥിരം OT::cff2::accelerator_subset_t, cff2_cs_interp_env_t<blend_arg_t>, cff2_cs_opset_subr_subset_t>
{
  cff2_subr_subsetter_t (സ്ഥിരം OT::cff2::accelerator_subset_t &acc_, സ്ഥിരം hb_subset_plan_t *plan_)
    : subr_subsetter_t (acc_, plan_) {}

  സ്ഥിതം ശൂന്യം complete_parsed_str (cff2_cs_interp_env_t<blend_arg_t> &env, subr_subset_param_t& param, parsed_cs_str_t &charstring)
  {
    /* vsindex is inserted at the beginning of the charstring as necessary */
    എങ്കിൽ (env.seen_vsindex ())
    {
      സംഖ്യ_t  ivs;
      ivs.set_int ((പൂർണ്ണസംഖ്യ)env.get_ivs ());
      charstring.set_prefix (ivs, OpCode_vsindexcs);
    }
  }
};

ഘടന cff2_private_blend_encoder_param_t
{
  cff2_private_blend_encoder_param_t (hb_serialize_context_t *c,
				      സ്ഥിരം CFF2ItemVariationStore *varStore,
				      hb_array_t<പൂർണ്ണസംഖ്യ> normalized_coords) :
    c (c), varStore (varStore), normalized_coords (normalized_coords) {}

  ശൂന്യം init () {}

  ശൂന്യം process_blend ()
  {
    എങ്കിൽ (!seen_blend)
    {
      region_എണ്ണം_2 = varStore->varStore.get_region_index_എണ്ണം (ivs);
      എങ്കിൽ (unlikely (!scalars.resize_exact (region_എണ്ണം_2)))
      {
	എങ്കിൽ (c) c->err (HB_SERIALIZE_ERROR_OTHER);
	seen_blend = സത്യം;
	തിരികെ;
      }
      varStore->varStore.get_region_scalars (ivs, normalized_coords.arrayZ, normalized_coords.length,
					     &scalars[0], region_എണ്ണം_2);
      seen_blend = സത്യം;
    }
  }

  ഇരട്ട blend_deltas (hb_array_t<സ്ഥിരം സംഖ്യ_t> deltas) സ്ഥിരം
  {
    ഇരട്ട v = 0;
    എങ്കിൽ (likely (scalars.length == deltas.length))
    {
      unsigned എണ്ണം = scalars.length;
      വേണ്ടി (unsigned i = 0; i < എണ്ണം; i++)
	v += (ഇരട്ട) scalars.arrayZ[i] * deltas.arrayZ[i].to_real ();
    }
    തിരികെ v;
  }


  hb_serialize_context_t *c = nullptr;
  തർക്കം seen_blend = അസത്യം;
  unsigned ivs = 0;
  unsigned region_എണ്ണം_2 = 0;
  hb_vector_t<ദശാംശം> scalars;
  സ്ഥിരം	 CFF2ItemVariationStore *varStore = nullptr;
  hb_array_t<പൂർണ്ണസംഖ്യ> normalized_coords;

  /* CFF2 partial instancing: when set, blends are rewritten against the
   * instanced variation store instead of being flattened. */
  സ്ഥിരം cff2_instancing_plan_t *instancer = nullptr;
  /* Error-compensated fold rounding state, per dict operator. */
  ഇരട്ട fold_exact = 0.;
  ഇരട്ട fold_emitted = 0.;
};

ഘടന cff2_private_dict_blend_opset_t : dict_opset_t
{
  സ്ഥിതം ശൂന്യം process_arg_blend (cff2_private_blend_encoder_param_t& param,
				 സംഖ്യ_t &arg,
				 സ്ഥിരം hb_array_t<സ്ഥിരം സംഖ്യ_t> blends,
				 unsigned n, unsigned i)
  {
    arg.set_int (round (arg.to_real () + param.blend_deltas (blends)));
  }

  /* The fold added to a value, error-compensated across the operator: most
   * blended private-dict values (BlueValues, StemSnap*, ...) are
   * delta-encoded, so independently rounded folds would accumulate in the
   * decoded absolutes; keeping the emitted cumulative fold equal to the
   * rounded exact cumulative fold bounds every absolute within ±0.5. */
  സ്ഥിതം ഇരട്ട fold (cff2_private_blend_encoder_param_t& param,
		      സ്ഥിരം hb_array_t<സ്ഥിരം സംഖ്യ_t> blends)
  {
    ഇരട്ട f = param.blend_deltas (blends);
    ഇരട്ട emit = round (param.fold_exact + f) - param.fold_emitted;
    param.fold_exact += f;
    param.fold_emitted += emit;
    തിരികെ emit;
  }

  /* Partial instancing: emit the args preceding this blend group, then the
   * group itself rewritten against the instanced store, and clear the
   * stack.  The op's remaining args are flushed at process_op time. */
  സ്ഥിതം ശൂന്യം rewrite_blends (cff2_priv_dict_interp_env_t& env,
			      cff2_private_blend_encoder_param_t& param,
			      unsigned start, unsigned n, unsigned k)
  {
    സ്ഥിരം cff2_instancing_plan_t &instancer = *param.instancer;
    സ്ഥിരം cff2_instancing_plan_t::vardata_transform_t *t = instancer.get_transform (param.ivs);
    എങ്കിൽ (unlikely (!t || t->gains.length != k))
    {
      env.set_error ();
      തിരികെ;
    }

    str_buff_t str;
    str_encoder_t encoder (str);

    വേണ്ടി (unsigned idx = 0; idx < start; idx++)
      encoder.encode_അക്കം_tp (env.arg_അട്ടി[idx]);

    unsigned k_new = t->alive () ? t->matrix.length : 0;
    എങ്കിൽ (!k_new)
    {
      /* This VarData died (or is unused by any retained charstring, which
       * can happen for FDs kept only for retain-gids holes); blends
       * dissolve into statics. */
      വേണ്ടി (unsigned j = 0; j < n; j++)
      {
	സ്ഥിരം hb_array_t<സ്ഥിരം സംഖ്യ_t> blends = env.arg_അട്ടി.sub_array (start + n + (j * k), k);
	സംഖ്യ_t v;
	v.set_real (env.arg_അട്ടി[start + j].to_real () + fold (param, blends));
	encoder.encode_അക്കം_tp (v);
      }
    }
    അല്ലെങ്കിൽ
    {
      unsigned done = 0;
      ആകുമ്പോൾ (done < n)
      {
	unsigned avail = 512 > (start + done) ? 512 - (start + done) : 0;
	unsigned chunk = hb_min (avail / (k_new + 1), n - done);
	എങ്കിൽ (unlikely (!chunk))
	{
	  env.set_error ();
	  തിരികെ;
	}

	വേണ്ടി (unsigned j = done; j < done + chunk; j++)
	{
	  സ്ഥിരം hb_array_t<സ്ഥിരം സംഖ്യ_t> blends = env.arg_അട്ടി.sub_array (start + n + (j * k), k);
	  സംഖ്യ_t v;
	  v.set_real (env.arg_അട്ടി[start + j].to_real () + fold (param, blends));
	  encoder.encode_അക്കം_tp (v);
	}
	വേണ്ടി (unsigned j = done; j < done + chunk; j++)
	{
	  സ്ഥിരം hb_array_t<സ്ഥിരം സംഖ്യ_t> blends = env.arg_അട്ടി.sub_array (start + n + (j * k), k);
	  വേണ്ടി (unsigned r = 0; r < k_new; r++)
	  {
	    സംഖ്യ_t v;
	    v.set_int (hb_clamp_to<പൂർണ്ണസംഖ്യ> (round (t->transform_delta (r, blends))));
	    encoder.encode_അക്കം_tp (v);
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

  സ്ഥിതം ശൂന്യം process_blend (cff2_priv_dict_interp_env_t& env, cff2_private_blend_encoder_param_t& param)
  {
    unsigned പൂർണ്ണസംഖ്യ n, k;

    param.process_blend ();
    k = param.region_എണ്ണം_2;
    n = env.arg_അട്ടി.എടുക്കുക_uint ();

    unsigned എണ്ണം = env.arg_അട്ടി.get_എണ്ണം ();
    unsigned പൂർണ്ണസംഖ്യ total;
    എങ്കിൽ (unlikely (hb_unsigned_mul_overflows (k + 1, n, &total) || total > എണ്ണം))
    {
      env.set_error ();
      തിരികെ;
    }

    /* copy the blend values into blend array of the default values */
    unsigned പൂർണ്ണസംഖ്യ start = എണ്ണം - total;
    /* let an obvious error case fail, but note CFF2 spec doesn't forbid n==0 */
    എങ്കിൽ (unlikely (start > എണ്ണം))
    {
      env.set_error ();
      തിരികെ;
    }

    എങ്കിൽ (param.instancer && k)
    {
      rewrite_blends (env, param, start, n, k);
      തിരികെ;
    }

    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < n; i++)
    {
      സ്ഥിരം hb_array_t<സ്ഥിരം സംഖ്യ_t> blends = env.arg_അട്ടി.sub_array (start + n + (i * k), k);
      process_arg_blend (param, env.arg_അട്ടി[start + i], blends, n, i);
    }

    /* pop off blend values leaving default values now adorned with blend values */
    env.arg_അട്ടി.എടുക്കുക (k * n);
  }

  സ്ഥിതം ശൂന്യം process_op (op_code_t op, cff2_priv_dict_interp_env_t& env, cff2_private_blend_encoder_param_t& param)
  {
    തിരഞ്ഞെടുപ്പ് (op) {
      അവസ്ഥ OpCode_StdHW:
      അവസ്ഥ OpCode_StdVW:
      അവസ്ഥ OpCode_BlueScale:
      അവസ്ഥ OpCode_BlueShift:
      അവസ്ഥ OpCode_BlueFuzz:
      അവസ്ഥ OpCode_ExpansionFactor:
      അവസ്ഥ OpCode_LanguageGroup:
      അവസ്ഥ Op_Code_Blue_മൂല്യങ്ങൾ:
      അവസ്ഥ OpCode_OtherBlues:
      അവസ്ഥ OpCode_FamilyBlues:
      അവസ്ഥ OpCode_FamilyOtherBlues:
      അവസ്ഥ OpCode_StemSnapH:
      അവസ്ഥ OpCode_StemSnapV:
	നിർത്തുക;
      അവസ്ഥ OpCode_vsindexdict:
	env.process_vsindex ();
	param.ivs = env.get_ivs ();
	എങ്കിൽ (param.instancer)
	{
	  /* Re-emit with the remapped vsindex; 0 is the dict default. */
	  unsigned new_ivs = param.instancer->remap_ivs (param.ivs);
	  എങ്കിൽ (new_ivs)
	  {
	    str_buff_t str;
	    str_encoder_t encoder (str);
	    encoder.encode_int ((പൂർണ്ണസംഖ്യ) new_ivs);
	    encoder.encode_op (OpCode_vsindexdict);
	    auto bytes = str.as_bytes ();
	    param.c->embed (&bytes, bytes.length);
	  }
	}
	env.clear_args ();
	തിരികെ;
      അവസ്ഥ OpCode_blenddict:
	process_blend (env, param);
	തിരികെ;

      സ്ഥിരസ്ഥിതി:
	dict_opset_t::process_op (op, env);
	എങ്കിൽ (!env.arg_അട്ടി.is_empty ()) തിരികെ;
	നിർത്തുക;
    }

    എങ്കിൽ (unlikely (env.in_error ())) തിരികെ;

    // Write args then op

    str_buff_t str;
    str_encoder_t encoder (str);

    unsigned എണ്ണം = env.arg_അട്ടി.get_എണ്ണം ();
    വേണ്ടി (unsigned i = 0; i < എണ്ണം; i++)
      encoder.encode_അക്കം_tp (env.arg_അട്ടി[i]);

    encoder.encode_op (op);

    auto bytes = str.as_bytes ();
    param.c->embed (&bytes, bytes.length);

    env.clear_args ();
    param.fold_exact = param.fold_emitted = 0.;
  }
};

ഘടന cff2_private_dict_op_serializer_t : op_serializer_t
{
  cff2_private_dict_op_serializer_t (തർക്കം desubroutinize_, തർക്കം drop_hints_, തർക്കം pinned_,
				     സ്ഥിരം CFF::CFF2ItemVariationStore* varStore_,
				     hb_array_t<പൂർണ്ണസംഖ്യ> normalized_coords_,
				     സ്ഥിരം cff2_instancing_plan_t *instancer_ = nullptr)
    : desubroutinize (desubroutinize_), drop_hints (drop_hints_), pinned (pinned_),
      param (nullptr, varStore_, normalized_coords_)
  { param.instancer = instancer_; }

  തർക്കം serialize (hb_serialize_context_t *c,
		  സ്ഥിരം op_str_t &opstr,
		  objidx_t subrs_link) സ്ഥിരം
  {
    TRACE_SERIALIZE (ഇത്);

    എങ്കിൽ (drop_hints && dict_opset_t::is_hint_op (opstr.op))
      return_trace (സത്യം);

    എങ്കിൽ (opstr.op == OpCode_Subrs)
    {
      എങ്കിൽ (desubroutinize || !subrs_link)
	return_trace (സത്യം);
      അല്ലെങ്കിൽ
	return_trace (FontDict::serialize_link2_op (c, opstr.op, subrs_link));
    }

    എങ്കിൽ (pinned || param.instancer)
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

  സംരക്ഷിത:
  സ്ഥിരം തർക്കം desubroutinize;
  സ്ഥിരം തർക്കം drop_hints;
  സ്ഥിരം തർക്കം pinned;
  mutable cff2_private_blend_encoder_param_t param;
};



നാമാകാശ OT {
ഘടന cff2_subset_plan
{
  തർക്കം create (സ്ഥിരം OT::cff2::accelerator_subset_t &acc,
	      hb_subset_plan_t *plan)
  {
    /* make sure notdef is first */
    hb_codepoint_t old_glyph;
    എങ്കിൽ (!plan->old_gid_for_new_gid (0, &old_glyph) || (old_glyph != 0)) തിരികെ അസത്യം;

    അക്കം_glyphs = plan->അക്കം_output_glyphs ();
    orig_fdcount = acc.fdArray->എണ്ണം;

    drop_hints = plan->flags & HB_SUBSET_FLAGS_NO_HINTING;
    തർക്കം instancing_requested = (തർക്കം) plan->normalized_coords;
    pinned = instancing_requested && plan->all_axes_pinned;
    normalized_coords = plan->normalized_coords;
    head_maxp_info = plan->head_maxp_info;
    hmtx_map = &plan->hmtx_map;
    desubroutinize = plan->flags & HB_SUBSET_FLAGS_DESUBROUTINIZE ||
		     instancing_requested; // For instancing we need this path

    /* Partial instancing: some axes pinned or limited, others kept. */
    partial_instancing = instancing_requested && !pinned &&
			 acc.varStore != &Null (CFF2ItemVariationStore);
    എങ്കിൽ (partial_instancing &&
	unlikely (!instancing.create (acc.varStore, plan)))
      തിരികെ അസത്യം;
    axis_tags = &plan->axis_tags;

    /* Enable command capture for CFF2→CFF1 conversion (for specialization) */
    capture_commands = pinned;

 #ifdef HB_EXPERIMENTAL_API
    min_charstrings_off_size = (plan->flags & HB_SUBSET_FLAGS_IFTB_REQUIREMENTS) ? 4 : 0;
 #അല്ലെങ്കിൽ
    min_charstrings_off_size = 0;
 #endif

    എങ്കിൽ (desubroutinize)
    {
      എങ്കിൽ (partial_instancing)
      {
	/* Pass 1: collect which VarDatas the retained blends reference, so
	 * unused ones get pruned and the survivors renumbered. */
	hb_set_t used_fds;
	വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < അക്കം_glyphs; i++)
	{
	  hb_codepoint_t glyph;
	  എങ്കിൽ (!plan->old_gid_for_new_gid (i, &glyph))
	    തുടരുക;
	  സ്ഥിരം hb_ubytes_t str = (*acc.charStrings)[glyph];
	  unsigned പൂർണ്ണസംഖ്യ fd = CFF::subset_get_fd (acc, glyph);
	  എങ്കിൽ (unlikely (fd >= acc.fd_എണ്ണം))
	    തിരികെ അസത്യം;
	  used_fds.add (fd);

	  cff2_cs_interp_env_t<blend_arg_t> env (str, acc, fd);
	  cs_interpreter_t<cff2_cs_interp_env_t<blend_arg_t>, cff2_cs_opset_usage_t, cff2_usage_param_t> interp (env);
	  cff2_usage_param_t param;
	  param.instancing = &instancing;
	  param.drop_hints = drop_hints;
	  എങ്കിൽ (unlikely (!interp.interpret (param)))
	    തിരികെ അസത്യം;
	}
	വേണ്ടി (unsigned fd : used_fds)
	{
	  cff2_usage_param_t param;
	  param.instancing = &instancing;
	  unsigned എണ്ണം = acc.privateDicts[fd].get_എണ്ണം ();
	  വേണ്ടി (unsigned j = 0; j < എണ്ണം; j++)
	  {
	    സ്ഥിരം op_str_t &opstr = acc.privateDicts[fd][j];
	    എങ്കിൽ (drop_hints && dict_opset_t::is_hint_op (opstr.op))
	      തുടരുക;
	    cff2_priv_dict_interp_env_t env {hb_ubytes_t (opstr.ptr, opstr.length)};
	    dict_interpreter_t<cff2_private_dict_usage_opset_t, cff2_usage_param_t, cff2_priv_dict_interp_env_t> interp (env);
	    എങ്കിൽ (unlikely (!interp.interpret (param)))
	      തിരികെ അസത്യം;
	  }
	}
	എങ്കിൽ (unlikely (!instancing.finalize ()))
	  തിരികെ അസത്യം;

	/* Pass 2: like subr_flattener_t, but blends stay symbolic (no coords
	 * fed to the env) and get rewritten against the instanced store. */
	എങ്കിൽ (!subset_charstrings.resize_exact (അക്കം_glyphs))
	  തിരികെ അസത്യം;
	വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < അക്കം_glyphs; i++)
	{
	  hb_codepoint_t glyph;
	  എങ്കിൽ (!plan->old_gid_for_new_gid (i, &glyph))
	    തുടരുക;
	  സ്ഥിരം hb_ubytes_t str = (*acc.charStrings)[glyph];
	  unsigned പൂർണ്ണസംഖ്യ fd = CFF::subset_get_fd (acc, glyph);
	  എങ്കിൽ (unlikely (fd >= acc.fd_എണ്ണം))
	    തിരികെ അസത്യം;

	  cff2_cs_interp_env_t<blend_arg_t> env (str, acc, fd);
	  cs_interpreter_t<cff2_cs_interp_env_t<blend_arg_t>, cff2_cs_opset_flatten_t, flatten_param_t> interp (env);
	  flatten_param_t param = {
	    subset_charstrings.arrayZ[i],
	    (തർക്കം) (plan->flags & HB_SUBSET_FLAGS_NO_HINTING),
	    plan
	  };
	  param.instancer = &instancing;
	  എങ്കിൽ (unlikely (!interp.interpret (param)))
	    തിരികെ അസത്യം;
	}
      }
      അല്ലെങ്കിൽ
      {
      /* Flatten global & local subrs */
      subr_flattener_t<സ്ഥിരം OT::cff2::accelerator_subset_t, cff2_cs_interp_env_t<blend_arg_t>, cff2_cs_opset_flatten_t>
		    flattener(acc, plan);

      /* Enable command capture if requested (for specialization) */
      എങ്കിൽ (capture_commands)
      {
	എങ്കിൽ (!charstring_commands.resize_exact (അക്കം_glyphs))
	  തിരികെ അസത്യം;

	എങ്കിൽ (!flattener.flatten (subset_charstrings, &charstring_commands))
	  തിരികെ അസത്യം;
      }
      അല്ലെങ്കിൽ
      {
	എങ്കിൽ (!flattener.flatten (subset_charstrings))
	  തിരികെ അസത്യം;
      }
      }
    }
    അല്ലെങ്കിൽ
    {
      cff2_subr_subsetter_t	subr_subsetter (acc, plan);

      /* Subset subrs: collect used subroutines, leaving all unused ones behind */
      എങ്കിൽ (!subr_subsetter.subset ())
	തിരികെ അസത്യം;

      /* encode charstrings, global subrs, local subrs with new subroutine numbers */
      എങ്കിൽ (!subr_subsetter.encode_charstrings (subset_charstrings, !pinned))
	തിരികെ അസത്യം;

      എങ്കിൽ (!subr_subsetter.encode_globalsubrs (subset_globalsubrs))
	തിരികെ അസത്യം;

      /* local subrs */
      എങ്കിൽ (!subset_localsubrs.resize (orig_fdcount))
	തിരികെ അസത്യം;
      വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ fd = 0; fd < orig_fdcount; fd++)
      {
	subset_localsubrs[fd].init ();
	എങ്കിൽ (!subr_subsetter.encode_localsubrs (fd, subset_localsubrs[fd]))
	  തിരികെ അസത്യം;
      }
    }

    /* FDSelect */
    എങ്കിൽ (acc.fdSelect != &Null (CFF2FDSelect))
    {
      എങ്കിൽ (unlikely (!hb_plan_subset_cff_fdselect (plan,
						  orig_fdcount,
						  *(സ്ഥിരം FDSelect *)acc.fdSelect,
						  acc.അക്കം_glyphs,
						  subset_fdcount,
						  subset_fdselect_size,
						  subset_fdselect_format,
						  subset_fdselect_ranges,
						  fdmap)))
	തിരികെ അസത്യം;
    }
    അല്ലെങ്കിൽ
      fdmap.identity (1);

    തിരികെ സത്യം;
  }

  cff2_sub_table_info_t info;

  unsigned പൂർണ്ണസംഖ്യ    അക്കം_glyphs;
  unsigned പൂർണ്ണസംഖ്യ    orig_fdcount = 0;
  unsigned പൂർണ്ണസംഖ്യ    subset_fdcount = 1;
  unsigned പൂർണ്ണസംഖ്യ    subset_fdselect_size = 0;
  unsigned പൂർണ്ണസംഖ്യ    subset_fdselect_format = 0;
  തർക്കം            pinned = അസത്യം;
  hb_vector_t<code_pair_t>   subset_fdselect_ranges;

  hb_inc_bimap_t   fdmap;

  str_buff_vec_t	    subset_charstrings;
  str_buff_vec_t	    subset_globalsubrs;
  hb_vector_t<str_buff_vec_t> subset_localsubrs;

  തർക്കം	    drop_hints = അസത്യം;
  തർക്കം	    desubroutinize = അസത്യം;

  തർക്കം	    partial_instancing = അസത്യം;
  cff2_instancing_plan_t instancing;
  സ്ഥിരം hb_vector_t<hb_tag_t> *axis_tags = nullptr;

  unsigned  min_charstrings_off_size = 0;

  hb_array_t<പൂർണ്ണസംഖ്യ> normalized_coords; // For instantiation
  head_maxp_info_t head_maxp_info;  // For FontBBox
  സ്ഥിരം hb_hashmap_t<hb_codepoint_t, hb_pair_t<unsigned, പൂർണ്ണസംഖ്യ>> *hmtx_map; // For widths

  // Width optimization results (for CFF1 conversion)
  unsigned default_width = 0;
  unsigned nominal_width = 0;

  // Command capture for specialization (CFF2→CFF1 conversion)
  തർക്കം capture_commands = അസത്യം;
  hb_vector_t<hb_vector_t<cs_command_t>> charstring_commands;
};
} // namespace OT

/*
 * CFF2 to CFF1 Converter Implementation
 */

#അന്തര്ഭാവ "hb-cff-width-optimizer.hh"
#അന്തര്ഭാവ "hb-cff-specializer.hh"

/* Serialize charstrings using CFF1 format with widths */
സ്ഥിതം തർക്കം
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
  എങ്കിൽ (unlikely (!cff1_charstrings.resize (plan.subset_charstrings.length)))
  {
    c->എടുക്കുക_discard ();
    തിരികെ അസത്യം;
  }

  വേണ്ടി (unsigned i = 0; i < plan.subset_charstrings.length; i++)
  {
    // Get width for this glyph from hmtx_map
    unsigned width = 0;
    എങ്കിൽ (plan.hmtx_map->has (i))
      width = plan.hmtx_map->get (i).first;

    // Encode width if different from default
    str_encoder_t encoder (cff1_charstrings[i]);
    എങ്കിൽ (width != default_width)
    {
      പൂർണ്ണസംഖ്യ delta = (പൂർണ്ണസംഖ്യ) width - (പൂർണ്ണസംഖ്യ) nominal_width;
      encoder.encode_int (delta);
    }

    // Use specialized commands if available, otherwise use binary
    എങ്കിൽ (plan.capture_commands && i < plan.charstring_commands.length &&
        plan.charstring_commands[i].length > 0)
    {
      // Specialize and encode commands
      auto &commands = plan.charstring_commands[i];
      CFF::specialize_commands (commands, 48);  /* maxstack=48 for CFF1 */
      എങ്കിൽ (unlikely (!CFF::encode_commands (commands, cff1_charstrings[i])))
      {
        c->എടുക്കുക_discard ();
        തിരികെ അസത്യം;
      }
    }
    അല്ലെങ്കിൽ
    {
      // Use binary CharString
      സ്ഥിരം str_buff_t &cs = plan.subset_charstrings[i];
      വേണ്ടി (unsigned j = 0; j < cs.length; j++)
        cff1_charstrings[i].push (cs[j]);
    }

    // Check if it already ends with endchar (0x0e) or return (0x0b)
    എങ്കിൽ (cff1_charstrings[i].length == 0 ||
        (cff1_charstrings[i].tail () != 0x0e && cff1_charstrings[i].tail () != 0x0b))
    {
      // Append endchar operator
      എങ്കിൽ (unlikely (!cff1_charstrings[i].തള്ളുക_or_fail (0x0e)))
      {
        c->എടുക്കുക_discard ();
        തിരികെ അസത്യം;
      }
    }
  }

  unsigned data_size = 0;
  unsigned total_size = CFF1CharStrings::total_size (cff1_charstrings, &data_size, plan.min_charstrings_off_size);
  എങ്കിൽ (unlikely (!c->start_zerocopy (total_size)))
  {
    c->എടുക്കുക_discard ();
    തിരികെ അസത്യം;
  }

  auto *cs = c->start_embed<CFF1CharStrings> ();
  എങ്കിൽ (unlikely (!cs->serialize (c, cff1_charstrings, &data_size, plan.min_charstrings_off_size)))
  {
    c->എടുക്കുക_discard ();
    തിരികെ അസത്യം;
  }

  plan.info.char_strings_link = c->എടുക്കുക_pack (അസത്യം);
  തിരികെ സത്യം;
}

/* Serialize CID Charset (format 2 range: gid 0-N -> cid 0-N) */
സ്ഥിതം തർക്കം
_serialize_cff1_charset (hb_serialize_context_t *c,
                         unsigned പൂർണ്ണസംഖ്യ അക്കം_glyphs,
                         objidx_t &charset_link)
{
  // For CID fonts, create a simple identity charset
  // Format 2: one range covering all glyphs (except .notdef)
  c->push ();

  auto *charset = c->start_embed<Charset> ();
  എങ്കിൽ (unlikely (!charset))
  {
    c->എടുക്കുക_discard ();
    തിരികെ അസത്യം;
  }

  // Create a single range for CID 1 to num_glyphs-1
  hb_vector_t<code_pair_t> ranges;
  എങ്കിൽ (അക്കം_glyphs > 1)
  {
    code_pair_t range;
    range.code = 1;  // first CID
    range.glyph = അക്കം_glyphs - 2;  // nLeft (covers glyphs 1 to num_glyphs-1)
    ranges.push (range);
  }

  എങ്കിൽ (unlikely (!charset->serialize (c, 2, അക്കം_glyphs, ranges)))
  {
    c->എടുക്കുക_discard ();
    തിരികെ അസത്യം;
  }

  charset_link = c->എടുക്കുക_pack ();
  തിരികെ സത്യം;
}

/* CFF2 to CFF1 serialization */
നാമാകാശ CFF {

തർക്കം
serialize_cff2_to_cff1 (hb_serialize_context_t *c,
                        OT::cff2_subset_plan &plan,
                        സ്ഥിരം cff2_മുകൾ_dict_മൂല്യങ്ങൾ_t &cff2_മുകൾ_Dict,
                        സ്ഥിരം OT::cff2::accelerator_subset_t &acc)
{
  TRACE_SERIALIZE (ഇത്);

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
    widths.alloc (plan.അക്കം_glyphs);

    വേണ്ടി (unsigned gid = 0; gid < plan.അക്കം_glyphs; gid++)
    {
      unsigned width = 0;
      എങ്കിൽ (plan.hmtx_map->has (gid))
        width = plan.hmtx_map->get (gid).first;
      widths.push (width);
    }

    // Optimize defaultWidthX and nominalWidthX
    CFF::optimize_widths (widths, plan.default_width, plan.nominal_width);
  }

  // 1. CharStrings (with widths prepended)
  എങ്കിൽ (!_serialize_cff1_charstrings (c, plan, plan.default_width, plan.nominal_width))
    return_trace (അസത്യം);

  // 2. Private DICs & Local Subrs (same as CFF2)
  hb_vector_t<table_info_t> private_dict_infos;
  എങ്കിൽ (unlikely (!private_dict_infos.resize (plan.subset_fdcount)))
    return_trace (അസത്യം);

  വേണ്ടി (പൂർണ്ണസംഖ്യ i = (പൂർണ്ണസംഖ്യ)acc.privateDicts.length; --i >= 0;)
  {
    എങ്കിൽ (plan.fdmap.has (i))
    {
      objidx_t subrs_link = 0;

      എങ്കിൽ (plan.subset_localsubrs[i].length > 0)
      {
        auto *dest = c->push<CFF1Subrs> ();
        എങ്കിൽ (likely (dest->serialize (c, plan.subset_localsubrs[i])))
          subrs_link = c->എടുക്കുക_pack (അസത്യം);
        അല്ലെങ്കിൽ
        {
          c->എടുക്കുക_discard ();
          return_trace (അസത്യം);
        }
      }

      auto *pd = c->push<PrivateDict> ();
      // Use the CFF2 Private DICT serializer which instantiates blends when pinned=true
      cff2_private_dict_op_serializer_t privSzr (plan.desubroutinize, plan.drop_hints, plan.pinned,
                                                  acc.varStore, plan.normalized_coords);
      എങ്കിൽ (likely (pd->serialize (c, acc.privateDicts[i], privSzr, subrs_link)))
      {
        // Add defaultWidthX and nominalWidthX for CFF1
        str_buff_t width_ops;
        str_encoder_t encoder (width_ops);
        encoder.encode_int (plan.default_width);
        encoder.encode_op (OpCode_defaultWidthX);
        encoder.encode_int (plan.nominal_width);
        encoder.encode_op (OpCode_nominalWidthX);

        എങ്കിൽ (!encoder.in_error () && c->embed (width_ops.as_bytes ().arrayZ, width_ops.length))
        {
          unsigned fd = plan.fdmap[i];
          private_dict_infos[fd].size = c->length ();
          private_dict_infos[fd].link = c->എടുക്കുക_pack ();
        }
        അല്ലെങ്കിൽ
        {
          c->എടുക്കുക_discard ();
          return_trace (അസത്യം);
        }
      }
      അല്ലെങ്കിൽ
      {
        c->എടുക്കുക_discard ();
        return_trace (അസത്യം);
      }
    }
  }

  // 3. FDArray - serialize CFF2 font dicts as CFF1
  {
    auto *fda = c->push<FDArray<HBUINT16>> ();
    cff_font_dict_op_serializer_t fontSzr;
    auto it =
    + hb_zip (+ hb_iter (acc.fontDicts)
              | hb_filter ([&] (സ്ഥിരം cff2_font_dict_മൂല്യങ്ങൾ_t &_)
                { തിരികെ plan.fdmap.has (&_ - &acc.fontDicts[0]); }),
              hb_iter (private_dict_infos))
    ;
    // Explicitly specify template parameters: DICTVAL, INFO
    തർക്കം success = fda->serialize<cff2_font_dict_മൂല്യങ്ങൾ_t, table_info_t> (c, it, fontSzr);
    എങ്കിൽ (success)
      plan.info.fd_array_link = c->എടുക്കുക_pack (അസത്യം);
    അല്ലെങ്കിൽ
    {
      c->എടുക്കുക_discard ();
      return_trace (അസത്യം);
    }
  }

  // 4. FDSelect (required in CFF1 CID-keyed fonts)
  // CFF1 requires FDSelect for all CID-keyed fonts, even with just one FD
  // CFF2 makes it optional when there's only one FD
  എങ്കിൽ (acc.fdSelect != &Null (CFF2FDSelect))
  {
    c->push ();
    എങ്കിൽ (likely (hb_serialize_cff_fdselect (c, plan.അക്കം_glyphs,
                                          *(സ്ഥിരം FDSelect *)acc.fdSelect,
                                          plan.orig_fdcount,
                                          plan.subset_fdselect_format,
                                          plan.subset_fdselect_size,
                                          plan.subset_fdselect_ranges)))
      plan.info.fd_select.link = c->എടുക്കുക_pack ();
    അല്ലെങ്കിൽ
    {
      c->എടുക്കുക_discard ();
      return_trace (അസത്യം);
    }
  }
  അല്ലെങ്കിൽ
  {
    // Create a range-based FDSelect3 mapping all glyphs to FD 0
    // Format: format(1) + nRanges(2) + range(3) + sentinel(2) = 8 bytes
    c->push ();

    // Format byte
    HBUINT8 format;
    format = 3;
    എങ്കിൽ (unlikely (!c->embed (format)))
    {
      c->എടുക്കുക_discard ();
      return_trace (അസത്യം);
    }

    // nRanges
    HBUINT16 nRanges;
    nRanges = 1;
    എങ്കിൽ (unlikely (!c->embed (nRanges)))
    {
      c->എടുക്കുക_discard ();
      return_trace (അസത്യം);
    }

    // Single range: {first: 0, fd: 0}
    FDSelect3_Range range;
    range.first = 0;
    range.fd = 0;
    എങ്കിൽ (unlikely (!c->embed (range)))
    {
      c->എടുക്കുക_discard ();
      return_trace (അസത്യം);
    }

    // Sentinel (number of glyphs)
    HBUINT16 sentinel;
    sentinel = plan.അക്കം_glyphs;
    എങ്കിൽ (unlikely (!c->embed (sentinel)))
    {
      c->എടുക്കുക_discard ();
      return_trace (അസത്യം);
    }

    plan.info.fd_select.link = c->എടുക്കുക_pack ();
  }

  // 5. Charset (CID charset for identity mapping)
  objidx_t charset_link;
  എങ്കിൽ (!_serialize_cff1_charset (c, plan.അക്കം_glyphs, charset_link))
    return_trace (അസത്യം);

  // 6. Global Subrs
  {
    auto *dest = c->push<CFF1Subrs> ();
    എങ്കിൽ (likely (dest->serialize (c, plan.subset_globalsubrs)))
      c->എടുക്കുക_pack (അസത്യം);
    അല്ലെങ്കിൽ
    {
      c->എടുക്കുക_discard ();
      return_trace (അസത്യം);
    }
  }

  // 7. String INDEX - Add "Adobe" and "Identity" for ROS operator
  {
    സ്ഥിരം അക്ഷരം *adobe_str = "Adobe";
    സ്ഥിരം അക്ഷരം *identity_str = "Identity";
    unsigned adobe_len = 5;  // strlen("Adobe")
    unsigned identity_len = 8;  // strlen("Identity")

    // Build strings array
    hb_vector_t<hb_ubytes_t> strings;
    strings.alloc (2);
    strings.push (hb_ubytes_t ((സ്ഥിരം unsigned അക്ഷരം *) adobe_str, adobe_len));
    strings.push (hb_ubytes_t ((സ്ഥിരം unsigned അക്ഷരം *) identity_str, identity_len));

    // Serialize as CFF INDEX
    auto *dest = c->push<CFF1Index> ();
    എങ്കിൽ (likely (dest->serialize (c, strings)))
      c->എടുക്കുക_pack (അസത്യം);
    അല്ലെങ്കിൽ
    {
      c->എടുക്കുക_discard ();
      return_trace (അസത്യം);
    }
  }

  // 8. CFF Header
  OT::cff1 *cff = c->allocate_min<OT::cff1> ();
  എങ്കിൽ (unlikely (!cff)) return_trace (അസത്യം);

  /* header */
  cff->version.major = 0x01;
  cff->version.minor = 0x00;
  cff->nameIndex = cff->min_size;
  cff->offSize = 4; /* unused? */

  // 9. Name INDEX (single entry)
  {
    unsigned name_len = strlen (CFF1_DEFAULT_FONT_NAME);

    CFF1Index *idx = c->start_embed<CFF1Index> ();
    എങ്കിൽ (unlikely (!idx)) return_trace (അസത്യം);

    എങ്കിൽ (unlikely (!idx->serialize_header (c, hb_iter (&name_len, 1), name_len)))
      return_trace (അസത്യം);

    എങ്കിൽ (unlikely (!c->embed (CFF1_DEFAULT_FONT_NAME, name_len)))
      return_trace (അസത്യം);
  }

  // 10. Top DICT INDEX
  {
    // Serialize the Top DICT data first
    c->push<മുകൾ_Dict> ();
    cff1_from_cff2_മുകൾ_dict_op_serializer_t മുകൾ_Szr;

    // Serialize ROS first
    എങ്കിൽ (unlikely (!മുകൾ_Szr.serialize_ros (c)))
    {
      c->എടുക്കുക_discard ();
      return_trace (അസത്യം);
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

      എങ്കിൽ (encoder.in_error () || !c->embed (bbox_buff.as_bytes ().arrayZ, bbox_buff.length))
      {
        c->എടുക്കുക_discard ();
        return_trace (അസത്യം);
      }
    }

    // Serialize charset operator
    എങ്കിൽ (charset_link && unlikely (!FontDict::serialize_link4_op (c, OpCode_charset, charset_link, whence_t::Absolute)))
    {
      c->എടുക്കുക_discard ();
      return_trace (അസത്യം);
    }

    // Serialize FDSelect operator (required for CID-keyed CFF1 fonts)
    എങ്കിൽ (plan.info.fd_select.link && unlikely (!FontDict::serialize_link4_op (c, OpCode_FDSelect, plan.info.fd_select.link, whence_t::Absolute)))
    {
      c->എടുക്കുക_discard ();
      return_trace (അസത്യം);
    }

    // Serialize FDArray operator (required for CID-keyed CFF1 fonts)
    എങ്കിൽ (plan.info.fd_array_link && unlikely (!FontDict::serialize_link4_op (c, OpCode_FDArray, plan.info.fd_array_link, whence_t::Absolute)))
    {
      c->എടുക്കുക_discard ();
      return_trace (അസത്യം);
    }

    // Serialize other operators from CFF2 TopDict
    വേണ്ടി (സ്ഥിരം auto &opstr : cff2_മുകൾ_Dict.values)
    {
      എങ്കിൽ (unlikely (!മുകൾ_Szr.serialize (c, opstr, plan.info)))
      {
        c->എടുക്കുക_discard ();
        return_trace (അസത്യം);
      }
    }

    unsigned മുകൾ_size = c->length ();
    c->എടുക്കുക_pack (അസത്യം);

    // Serialize INDEX header
    auto *dest = c->start_embed<CFF1Index> ();
    എങ്കിൽ (unlikely (!dest->serialize_header (c, hb_iter (&മുകൾ_size, 1), മുകൾ_size)))
      return_trace (അസത്യം);
  }

  return_trace (സത്യം);
}

} /* namespace CFF */

സ്ഥിതം തർക്കം _serialize_cff2_charstrings (hb_serialize_context_t *c,
			     cff2_subset_plan &plan,
			     സ്ഥിരം OT::cff2::accelerator_subset_t  &acc)
{
  c->push ();

  unsigned data_size = 0;
  unsigned total_size = CFF2CharStrings::total_size (plan.subset_charstrings, &data_size, plan.min_charstrings_off_size);
  എങ്കിൽ (unlikely (!c->start_zerocopy (total_size)))
    തിരികെ അസത്യം;

  auto *cs = c->start_embed<CFF2CharStrings> ();
  എങ്കിൽ (unlikely (!cs->serialize (c, plan.subset_charstrings, &data_size, plan.min_charstrings_off_size)))
  {
    c->എടുക്കുക_discard ();
    തിരികെ അസത്യം;
  }

  plan.info.char_strings_link = c->എടുക്കുക_pack (അസത്യം);
  തിരികെ സത്യം;
}

തർക്കം
OT::cff2::accelerator_subset_t::serialize (hb_serialize_context_t *c,
					   ഘടന cff2_subset_plan &plan,
					   hb_array_t<പൂർണ്ണസംഖ്യ> normalized_coords) സ്ഥിരം
{
  /* push charstrings onto the object stack first which will ensure it packs as the last
     object in the table. Keeping the chastrings last satisfies the requirements for patching
     via IFTB. If this ordering needs to be changed in the future, charstrings should be left
     at the end whenever HB_SUBSET_FLAGS_ITFB_REQUIREMENTS is enabled. */
  എങ്കിൽ (!_serialize_cff2_charstrings(c, plan, *ഇത്))
    തിരികെ അസത്യം;

  /* private dicts & local subrs */
  hb_vector_t<table_info_t>  private_dict_infos;
  എങ്കിൽ (unlikely (!private_dict_infos.resize (plan.subset_fdcount))) തിരികെ അസത്യം;

  വേണ്ടി (പൂർണ്ണസംഖ്യ i = (പൂർണ്ണസംഖ്യ)privateDicts.length; --i >= 0 ;)
  {
    എങ്കിൽ (plan.fdmap.has (i))
    {
      objidx_t	subrs_link = 0;

      എങ്കിൽ (plan.subset_localsubrs[i].length > 0)
      {
	auto *dest = c->push <CFF2Subrs> ();
	എങ്കിൽ (likely (dest->serialize (c, plan.subset_localsubrs[i])))
	  subrs_link = c->എടുക്കുക_pack (അസത്യം);
	അല്ലെങ്കിൽ
	{
	  c->എടുക്കുക_discard ();
	  തിരികെ അസത്യം;
	}
      }
      auto *pd = c->push<PrivateDict> ();
      cff2_private_dict_op_serializer_t privSzr (plan.desubroutinize, plan.drop_hints, plan.pinned,
						 varStore, normalized_coords,
						 plan.partial_instancing ? &plan.instancing : nullptr);
      എങ്കിൽ (likely (pd->serialize (c, privateDicts[i], privSzr, subrs_link)))
      {
	unsigned fd = plan.fdmap[i];
	private_dict_infos[fd].size = c->length ();
	private_dict_infos[fd].link = c->എടുക്കുക_pack ();
      }
      അല്ലെങ്കിൽ
      {
	c->എടുക്കുക_discard ();
	തിരികെ അസത്യം;
      }
    }
  }

  /* FDSelect */
  എങ്കിൽ (fdSelect != &Null (CFF2FDSelect))
  {
    c->push ();
    എങ്കിൽ (likely (hb_serialize_cff_fdselect (c, plan.അക്കം_glyphs, *(സ്ഥിരം FDSelect *)fdSelect,
					   plan.orig_fdcount,
					   plan.subset_fdselect_format, plan.subset_fdselect_size,
					   plan.subset_fdselect_ranges)))
      plan.info.fd_select.link = c->എടുക്കുക_pack ();
    അല്ലെങ്കിൽ
    {
      c->എടുക്കുക_discard ();
      തിരികെ അസത്യം;
    }
  }

  /* FDArray (FD Index) */
  {
    auto *fda = c->push<CFF2FDArray> ();
    cff_font_dict_op_serializer_t fontSzr;
    auto it =
    + hb_zip (+ hb_iter (fontDicts)
	      | hb_filter ([&] (സ്ഥിരം cff2_font_dict_മൂല്യങ്ങൾ_t &_)
		{ തിരികെ plan.fdmap.has (&_ - &fontDicts[0]); }),
	      hb_iter (private_dict_infos))
    ;
    എങ്കിൽ (unlikely (!fda->serialize (c, it, fontSzr)))
    {
      c->എടുക്കുക_discard ();
      തിരികെ അസത്യം;
    }
    plan.info.fd_array_link = c->എടുക്കുക_pack (അസത്യം);
  }

  /* variation store */
  എങ്കിൽ (varStore != &Null (CFF2ItemVariationStore) &&
      !plan.pinned)
  {
    എങ്കിൽ (plan.partial_instancing)
    {
      എങ്കിൽ (plan.instancing.has_variations ())
      {
	c->push ();
	എങ്കിൽ (unlikely (!plan.instancing.serialize_var_store (c, *plan.axis_tags)))
	{
	  c->എടുക്കുക_discard ();
	  തിരികെ അസത്യം;
	}
	plan.info.var_store_link = c->എടുക്കുക_pack (അസത്യം);
      }
      /* else: the store emptied; the vstore op gets dropped along with it. */
    }
    അല്ലെങ്കിൽ
    {
      auto *dest = c->push<CFF2ItemVariationStore> ();
      എങ്കിൽ (unlikely (!dest->serialize (c, varStore)))
      {
	c->എടുക്കുക_discard ();
	തിരികെ അസത്യം;
      }
      plan.info.var_store_link = c->എടുക്കുക_pack (അസത്യം);
    }
  }

  OT::cff2 *cff2 = c->allocate_min<OT::cff2> ();
  എങ്കിൽ (unlikely (!cff2)) തിരികെ അസത്യം;

  /* header */
  cff2->version.major = 0x02;
  cff2->version.minor = 0x00;
  cff2->മുകൾ_Dict_2 = OT::cff2::static_size;

  /* top dict */
  {
    മുകൾ_Dict &dict = cff2 + cff2->മുകൾ_Dict_2;
    cff2_മുകൾ_dict_op_serializer_t മുകൾ_Szr;
    എങ്കിൽ (unlikely (!dict.serialize (c, മുകൾ_Dict_2, മുകൾ_Szr, plan.info))) തിരികെ അസത്യം;
    cff2->മുകൾ_Dict_Size = c->head - (സ്ഥിരം അക്ഷരം *)&dict;
  }

  /* global subrs */
  {
    auto *dest = c->start_embed <CFF2Subrs> ();
    തിരികെ dest->serialize (c, plan.subset_globalsubrs);
  }
}

തർക്കം
OT::cff2::accelerator_subset_t::subset (hb_subset_context_t *c) സ്ഥിരം
{
  cff2_subset_plan cff2_plan;

  എങ്കിൽ (unlikely (!cff2_plan.create (*ഇത്, c->plan))) തിരികെ അസത്യം;

  // If instantiating (pinned) and downgrade flag is set, convert to CFF1
  എങ്കിൽ (cff2_plan.pinned && (c->plan->flags & HB_SUBSET_FLAGS_DOWNGRADE_CFF2))
  {
    // Serialize CFF1 to the subsetter's serializer
    // If we run out of room, returning true will cause subsetter to retry with larger buffer
    തർക്കം result = CFF::serialize_cff2_to_cff1 (c->serializer, cff2_plan, മുകൾ_Dict_2, *ഇത്);

    എങ്കിൽ (c->serializer->ran_out_of_room ())
      തിരികെ സത്യം; // Subsetter will retry with larger buffer

    എങ്കിൽ (result && !c->serializer->in_error ())
    {
      // Success - end serialization to resolve links
      c->serializer->end_serialize ();

      // Copy the serialized CFF1 data and add as CFF table
      hb_blob_t *cff_blob = c->serializer->copy_blob ();
      എങ്കിൽ (cff_blob)
      {
        c->plan->add_table (HB_TAG('C','F','F',' '), cff_blob);
        hb_blob_destroy (cff_blob);

        // Return false to signal CFF2 table is not needed
        തിരികെ അസത്യം;
      }
    }

    // Conversion failed - don't fall back, fail hard for debugging
    തിരികെ അസത്യം;
  }

  തിരികെ serialize (c->serializer, cff2_plan,
		    c->plan->normalized_coords.as_array ());
}

#endif
