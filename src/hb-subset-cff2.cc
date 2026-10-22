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

#समावेश "hb.hh"

#अगर_अघोषित HB_NO_SUBSET_CFF

#समावेश "hb-open-type.hh"
#समावेश "hb-ot-cff2-table.hh"
#समावेश "hb-ot-var-common.hh"
#समावेश "hb-set.h"
#समावेश "hb-subset-plan.hh"
#समावेश "hb-subset-cff-common.hh"
#समावेश "hb-cff2-interp-cs.hh"
#समावेश "hb-subset-cff2-to-cff1.hh"

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
काष्ठा cff2_instancing_plan_t
{
  काष्ठा vardata_transform_t
  {
    /* new_deltas[j] = Σ_i matrix[j][i] * old_deltas[i] */
    hb_vector_t<hb_vector_t<भग्न>> matrix;
    /* Default-value fold-in scalars, one per old region: the old store's
     * region scalars at the new default location. */
    hb_vector_t<भग्न> gains;
    /* Surviving regions, parallel to matrix rows. */
    hb_vector_t<hb_hashmap_t<hb_tag_t, Triple>> regions;
    /* Index into the new global region list, per new region (finalize). */
    hb_vector_t<अचिन्हित> region_indices;
    अचिन्हित new_major = 0;
    bool alive_ = false;
    bool alive () स्थिर { वापस alive_; }

    /* One value's delta for new region j, from its old per-region deltas. */
    द्विगुन transform_delta (अचिन्हित j, hb_array_t<स्थिर संख्या_t> deltas) स्थिर
    {
      स्थिर hb_vector_t<भग्न> &col = matrix.arrayZ[j];
      द्विगुन d = 0.;
      अचिन्हित गिनती = hb_min (col.लंबाई, deltas.लंबाई);
      क्रम (अचिन्हित c = 0; c < गिनती; c++)
	d += (द्विगुन) col.arrayZ[c] * deltas.arrayZ[c].to_real ();
      वापस d;
    }
  };

  hb_vector_t<vardata_transform_t> transforms;  /* per old VarData (vsindex) */
  hb_vector_t<hb_hashmap_t<hb_tag_t, Triple>> new_regions;  /* global region list */
  hb_set_t used_majors;
  अचिन्हित new_major_गिनती = 0;

  bool has_variations () स्थिर { वापस new_major_गिनती; }

  स्थिर vardata_transform_t *get_transform (अचिन्हित major) स्थिर
  { वापस major < transforms.लंबाई ? &transforms.arrayZ[major] : nullptr; }

  /* Record that a retained blend references this VarData. */
  व्योम note_use (अचिन्हित major) { used_majors.add (major); }

  /* The new vsindex a private dict's (or charstring's) old ivs maps to;
   * dead VarDatas fall back to 0, their blends having become static. */
  अचिन्हित remap_ivs (अचिन्हित old_ivs) स्थिर
  {
    स्थिर vardata_transform_t *t = get_transform (old_ivs);
    वापस (t && t->alive ()) ? t->new_major : 0;
  }

  द्विगुन fold_deltas (अचिन्हित old_ivs, hb_array_t<स्थिर संख्या_t> deltas) स्थिर
  {
    स्थिर vardata_transform_t *t = get_transform (old_ivs);
    अगर (unlikely (!t || t->gains.लंबाई != deltas.लंबाई)) वापस 0.;
    द्विगुन v = 0.;
    क्रम (अचिन्हित i = 0; i < deltas.लंबाई; i++)
      v += (द्विगुन) t->gains.arrayZ[i] * deltas.arrayZ[i].to_real ();
    वापस v;
  }

  bool create (स्थिर CFF2ItemVariationStore *cff2VarStore,
	       स्थिर hb_subset_plan_t *plan)
  {
    स्थिर OT::ItemVariationStore &varStore = cff2VarStore->varStore;
    अचिन्हित major_गिनती = varStore.get_sub_table_गिनती ();
    अगर (!transforms.resize (major_गिनती)) वापस false;

    अगर (!varStore.get_region_list ().get_var_regions (plan->axes_old_सूचकांक_tag_map, orig_regions))
      वापस false;

    /* Instance each VarData with identity delta rows; the surviving tuples'
     * delta columns are the old-region → new-region transform. */
    OT::optimize_scratch_t scratch;
    क्रम (अचिन्हित m = 0; m < major_गिनती; m++)
    {
      स्थिर OT::Var_तथ्य &var_तथ्य = varStore.get_sub_table (m);
      अचिन्हित k = var_तथ्य.get_region_सूचकांक_गिनती ();
      vardata_transform_t &t = transforms.arrayZ[m];

      अगर (!t.gains.resize_exact (k)) वापस false;
      varStore.get_region_scalars (m, plan->normalized_coords.arrayZ,
				   plan->normalized_coords.लंबाई,
				   t.gains.arrayZ, k);

      OT::tuple_variations_t tuples;
      क्रम (अचिन्हित r = 0; r < k; r++)
      {
	OT::tuple_delta_t tuple;
	अगर (!tuple.deltas_x.resize (k) ||
	    !tuple.indices.resize (k))
	  वापस false;
	क्रम (अचिन्हित i = 0; i < k; i++)
	  tuple.indices.arrayZ[i] = true;
	tuple.deltas_x.arrayZ[r] = 1.f;
	अचिन्हित region_सूचकांक = var_तथ्य.get_region_सूचकांक (r);
	अगर (unlikely (region_सूचकांक >= orig_regions.लंबाई)) वापस false;
	tuple.axis_tuples = orig_regions.arrayZ[region_सूचकांक];
	अगर (unlikely (tuple.axis_tuples.in_error ())) वापस false;
	tuples.tuple_vars.push (std::move (tuple));
      }
      अगर (unlikely (tuples.tuple_vars.in_error ())) वापस false;

      अगर (!tuples.instantiate (plan->axes_location, plan->axes_triple_distances, scratch))
	वापस false;

      क्रम (स्वतः &tuple : tuples.tuple_vars)
      {
	अगर (unlikely (tuple.deltas_x.लंबाई != k)) वापस false;
	hb_vector_t<भग्न> col;
	अगर (unlikely (!col.resize_exact (k))) वापस false;
	क्रम (अचिन्हित i = 0; i < k; i++)
	  col.arrayZ[i] = tuple.deltas_x.arrayZ[i];
	t.matrix.push (std::move (col));
	t.regions.push (std::move (tuple.axis_tuples));
      }
      अगर (unlikely (t.matrix.in_error () || t.regions.in_error ())) वापस false;
    }

    वापस true;
  }

  /* Decide which VarDatas survive, number them, and build the new global
   * region list: surviving original regions first, in their original order,
   * then solver-split new regions in order of appearance (fontTools'
   * rebuildRegions + prune_regions). */
  bool finalize ()
  {
    क्रम (अचिन्हित m = 0; m < transforms.लंबाई; m++)
    {
      vardata_transform_t &t = transforms.arrayZ[m];
      t.alive_ = t.matrix.लंबाई && used_majors.has (m);
      अगर (t.alive_)
	t.new_major = new_major_गिनती++;
    }

    /* Keys hash by content, so pointers into different owners compare fine. */
    hb_hashmap_t<स्थिर hb_hashmap_t<hb_tag_t, Triple>*, अचिन्हित> region_idx_map;
    hb_vector_t<स्थिर hb_hashmap_t<hb_tag_t, Triple>*> region_ptrs;
    क्रम (स्थिर vardata_transform_t &t : transforms)
    {
      अगर (!t.alive ()) जारी;
      क्रम (स्थिर स्वतः &region : t.regions)
	अगर (!region_idx_map.has (&region))
	  अगर (unlikely (!region_idx_map.set (&region, (अचिन्हित) -1)))
	    वापस false;
    }
    क्रम (स्थिर स्वतः &r : orig_regions)
    {
      अचिन्हित *idx;
      अगर (region_idx_map.has (&r, &idx) && *idx == (अचिन्हित) -1)
      {
	अगर (unlikely (!region_idx_map.set (&r, region_ptrs.लंबाई))) वापस false;
	region_ptrs.push (&r);
      }
    }
    क्रम (स्थिर vardata_transform_t &t : transforms)
    {
      अगर (!t.alive ()) जारी;
      क्रम (स्थिर स्वतः &region : t.regions)
      {
	अचिन्हित *idx;
	अगर (region_idx_map.has (&region, &idx) && *idx == (अचिन्हित) -1)
	{
	  अगर (unlikely (!region_idx_map.set (&region, region_ptrs.लंबाई))) वापस false;
	  region_ptrs.push (&region);
	}
      }
    }
    अगर (unlikely (region_ptrs.in_error ())) वापस false;

    क्रम (vardata_transform_t &t : transforms)
    {
      अगर (!t.alive ()) जारी;
      क्रम (स्थिर स्वतः &region : t.regions)
      {
	अचिन्हित *idx;
	अगर (unlikely (!region_idx_map.has (&region, &idx))) वापस false;
	t.region_indices.push (*idx);
      }
      अगर (unlikely (t.region_indices.in_error ())) वापस false;
    }

    अगर (!new_regions.resize (region_ptrs.लंबाई)) वापस false;
    क्रम (अचिन्हित i = 0; i < region_ptrs.लंबाई; i++)
    {
      new_regions.arrayZ[i] = *region_ptrs.arrayZ[i];
      अगर (unlikely (new_regions.arrayZ[i].in_error ())) वापस false;
    }

    वापस true;
  }

  /* Serialize the instanced store: a CFF2ItemVariationStore whose VarDatas
   * carry no items, just region indexes (blend deltas live in charstrings).
   * Laid out manually since all sizes are known up front. */
  bool serialize_var_store (hb_serialize_context_t *c,
			    स्थिर hb_vector_t<hb_tag_t> &axis_tags) स्थिर
  {
    अचिन्हित तथ्य_गिनती = new_major_गिनती;
    अचिन्हित header_आकार = 2 + 4 + 2 + 4 * तथ्य_गिनती;

    uint64_t offset = header_आकार;
    hb_vector_t<अचिन्हित> तथ्य_offsets;
    क्रम (स्थिर vardata_transform_t &t : transforms)
    {
      अगर (!t.alive ()) जारी;
      तथ्य_offsets.push ((अचिन्हित) offset);
      offset += 6 + 2 * (uint64_t) t.region_indices.लंबाई;
    }
    अगर (unlikely (तथ्य_offsets.in_error () || तथ्य_offsets.लंबाई != तथ्य_गिनती))
      वापस false;
    अचिन्हित regions_offset = (अचिन्हित) offset;
    uint64_t योग = offset + 4 + (uint64_t) new_regions.लंबाई * axis_tags.लंबाई * 6;
    अगर (unlikely (योग > 0xFFFFFFFFu)) वापस false;

    HBUINT16 *आकार_field = c->allocate_आकार<HBUINT16> (HBUINT16::static_आकार);
    अगर (unlikely (!आकार_field)) वापस false;
    /* OFF 2026 5.3.3.11.2 permits larger stores with length=65535. */
    *आकार_field = hb_min (योग, (uint64_t) 0xFFFFu);

    अक्षर *header = c->allocate_आकार<अक्षर> (header_आकार);
    अगर (unlikely (!header)) वापस false;
    * (HBUINT16 *) (header + 0) = 1;			/* format */
    * (HBUINT32 *) (header + 2) = regions_offset;
    * (HBUINT16 *) (header + 6) = तथ्य_गिनती;
    क्रम (अचिन्हित i = 0; i < तथ्य_गिनती; i++)
      * (HBUINT32 *) (header + 8 + 4 * i) = तथ्य_offsets.arrayZ[i];

    क्रम (स्थिर vardata_transform_t &t : transforms)
    {
      अगर (!t.alive ()) जारी;
      अचिन्हित k = t.region_indices.लंबाई;
      HBUINT16 *v = (HBUINT16 *) c->allocate_आकार<अक्षर> (6 + 2 * k);
      अगर (unlikely (!v)) वापस false;
      v[0] = 0;						/* itemCount */
      v[1] = 0;						/* wordSizeCount */
      v[2] = k;						/* regionIndices.len */
      क्रम (अचिन्हित j = 0; j < k; j++)
	v[3 + j] = t.region_indices.arrayZ[j];
    }

    hb_vector_t<स्थिर hb_hashmap_t<hb_tag_t, Triple>*> region_ptrs;
    अगर (unlikely (!region_ptrs.alloc_exact (new_regions.लंबाई))) वापस false;
    क्रम (स्थिर स्वतः &r : new_regions)
      region_ptrs.push (&r);
    स्वतः *region_list = c->start_embed<OT::VarRegionList> ();
    वापस region_list->serialize (c, axis_tags, region_ptrs);
  }

  निजी:
  hb_vector_t<hb_hashmap_t<hb_tag_t, Triple>> orig_regions;
};

/* Usage-collection pass for partial instancing: record which VarDatas the
 * retained charstrings' and private dicts' blends reference.  Mirrors the
 * emission pass: blends consumed by dropped hint ops don't count. */
काष्ठा cff2_usage_param_t
{
  व्योम init () {}

  cff2_instancing_plan_t *instancing = nullptr;
  bool drop_hints = false;
  अचिन्हित ivs = 0;	/* dict-side current vsindex */
};

काष्ठा cff2_cs_opset_usage_t : cff2_cs_opset_t<cff2_cs_opset_usage_t, cff2_usage_param_t, blend_arg_t>
{
  अटल व्योम flush_args_and_op (op_code_t op, cff2_cs_interp_env_t<blend_arg_t> &env, cff2_usage_param_t& param)
  {
    bool counts = true;
    चयन (op)
    {
      हाल OpCode_return:
      हाल OpCode_endchar:
	counts = false;
	अवरोध;

      हाल OpCode_hstem:
      हाल OpCode_hstemhm:
      हाल OpCode_vstem:
      हाल OpCode_vstemhm:
      हाल OpCode_hintmask:
      हाल OpCode_cntrmask:
	अगर (param.drop_hints)
	  counts = false;
	अवरोध;

      शेष:
	अवरोध;
    }

    अगर (counts)
      क्रम (अचिन्हित पूर्णांक i = 0; i < env.arg_ढेर.get_गिनती (); i++)
	अगर (env.arg_ढेर[i].blending ())
	{
	  param.instancing->note_use (env.get_ivs ());
	  अवरोध;
	}

    env.clear_args ();
  }
};

काष्ठा cff2_private_dict_usage_opset_t : dict_opset_t
{
  अटल व्योम process_op (op_code_t op, cff2_priv_dict_interp_env_t& env, cff2_usage_param_t& param)
  {
    चयन (op) {
      हाल OpCode_vsindexdict:
	env.process_vsindex ();
	param.ivs = env.get_ivs ();
	env.clear_args ();
	वापस;
      हाल OpCode_blenddict:
	param.instancing->note_use (param.ivs);
	env.clear_args ();
	वापस;
      शेष:
	dict_opset_t::process_op (op, env);
	अगर (!env.arg_ढेर.is_empty ()) वापस;
	env.clear_args ();
	वापस;
    }
  }
};

} /* namespace CFF */

काष्ठा cff2_sub_table_info_t : cff_sub_table_info_t
{
  cff2_sub_table_info_t ()
    : cff_sub_table_info_t (),
      var_store_link (0)
  {}

  objidx_t  var_store_link;
};

काष्ठा cff2_शीर्ष_dict_op_serializer_t : cff_शीर्ष_dict_op_serializer_t<>
{
  bool serialize (hb_serialize_context_t *c,
		  स्थिर op_str_t &opstr,
		  स्थिर cff2_sub_table_info_t &info) स्थिर
  {
    TRACE_SERIALIZE (यह);

    चयन (opstr.op)
    {
      हाल OpCode_vstore:
        अगर (info.var_store_link)
	  return_trace (FontDict::serialize_link4_op(c, opstr.op, info.var_store_link));
	अन्यथा
	  return_trace (true);

      शेष:
	return_trace (cff_शीर्ष_dict_op_serializer_t<>::serialize (c, opstr, info));
    }
  }
};

काष्ठा cff2_cs_opset_flatten_t : cff2_cs_opset_t<cff2_cs_opset_flatten_t, flatten_param_t, blend_arg_t>
{
  अटल व्योम flush_args_and_op (op_code_t op, cff2_cs_interp_env_t<blend_arg_t> &env, flatten_param_t& param)
  {
    /* Optionally capture command for specialization (before flushing, to preserve args) */
    अगर (param.commands)
    {
      bool skip_command = false;

      चयन (op)
      {
	हाल OpCode_return:
	हाल OpCode_endchar:
	  skip_command = true;
	  अवरोध;

	हाल OpCode_hstem:
	हाल OpCode_hstemhm:
	हाल OpCode_vstem:
	हाल OpCode_vstemhm:
	हाल OpCode_hintmask:
	हाल OpCode_cntrmask:
	  अगर (param.drop_hints)
	    skip_command = true;
	  अवरोध;

	शेष:
	  अवरोध;
      }

      अगर (!skip_command)
      {
	cs_command_t cmd (op);
	/* Capture resolved blend values */
	क्रम (अचिन्हित पूर्णांक i = 0; i < env.arg_ढेर.get_गिनती ();)
	{
	  स्थिर blend_arg_t &arg = env.arg_ढेर[i];
	  अगर (arg.blending ())
	  {
	    /* For blend args, capture only the resolved default value */
	    cmd.args.push (arg);
	    /* Skip over the multiple blend values */
	    i += arg.अंक_मूल्यावली_2;
	  }
	  अन्यथा
	  {
	    cmd.args.push (arg);
	    i++;
	  }
	}
	param.commands->push (cmd);
      }
    }

    चयन (op)
    {
      हाल OpCode_return:
      हाल OpCode_endchar:
	/* dummy opcodes in CFF2. ignore */
	अगर (op == OpCode_endchar && param.instancer)
	  emit_vsindex_prefix (env, param);
	अवरोध;

      हाल OpCode_hstem:
      हाल OpCode_hstemhm:
      हाल OpCode_vstem:
      हाल OpCode_vstemhm:
      हाल OpCode_hintmask:
      हाल OpCode_cntrmask:
	अगर (param.drop_hints)
	{
	  env.clear_args ();
	  वापस;
	}
	HB_FALLTHROUGH;

      शेष:
	SUPER::flush_args_and_op (op, env, param);
	अवरोध;
    }
  }

  अटल व्योम flush_args (cff2_cs_interp_env_t<blend_arg_t> &env, flatten_param_t& param)
  {
    क्रम (अचिन्हित पूर्णांक i = 0; i < env.arg_ढेर.get_गिनती ();)
    {
      स्थिर blend_arg_t &arg = env.arg_ढेर[i];
      अगर (arg.blending ())
      {
	अगर (unlikely (!((arg.अंक_मूल्यावली_2 > 0) && (env.arg_ढेर.get_गिनती () >= arg.अंक_मूल्यावली_2))))
	{
	  env.set_error ();
	  वापस;
	}
	अगर (param.instancer)
	  rewrite_blends (arg, i, env, param);
	अन्यथा
	  flatten_blends (arg, i, env, param);
	i += arg.अंक_मूल्यावली_2;
      }
      अन्यथा
      {
	str_encoder_t  encoder (param.flatStr);
	encoder.encode_अंक_cs (arg);
	i++;
      }
    }
    SUPER::flush_args (env, param);
  }

  अटल व्योम flatten_blends (स्थिर blend_arg_t &arg, अचिन्हित पूर्णांक i, cff2_cs_interp_env_t<blend_arg_t> &env, flatten_param_t& param)
  {
    /* flatten the default values */
    str_encoder_t  encoder (param.flatStr);
    क्रम (अचिन्हित पूर्णांक j = 0; j < arg.अंक_मूल्यावली_2; j++)
    {
      स्थिर blend_arg_t &arg1 = env.arg_ढेर[i + j];
      अगर (unlikely (!((arg1.blending () && (arg.अंक_मूल्यावली_2 == arg1.अंक_मूल्यावली_2) && (arg1.मूल्य_सूचकांक_2 == j) &&
	      (arg1.deltas.लंबाई == env.get_region_गिनती ())))))
      {
	env.set_error ();
	वापस;
      }
      encoder.encode_अंक_cs (arg1);
    }
    /* flatten deltas for each value */
    क्रम (अचिन्हित पूर्णांक j = 0; j < arg.अंक_मूल्यावली_2; j++)
    {
      स्थिर blend_arg_t &arg1 = env.arg_ढेर[i + j];
      क्रम (अचिन्हित पूर्णांक k = 0; k < arg1.deltas.लंबाई; k++)
	encoder.encode_अंक_cs (arg1.deltas[k]);
    }
    /* flatten the number of values followed by blend operator */
    encoder.encode_int (arg.अंक_मूल्यावली_2);
    encoder.encode_op (OpCode_blendcs);
  }

  /* Partial instancing: rewrite one blend group against the instanced store.
   * Each value's default absorbs the folded (pinned-away) contribution, and
   * its deltas are mapped onto the new region list. */
  अटल व्योम rewrite_blends (स्थिर blend_arg_t &arg, अचिन्हित पूर्णांक i, cff2_cs_interp_env_t<blend_arg_t> &env, flatten_param_t& param)
  {
    स्थिर cff2_instancing_plan_t &instancer = *param.instancer;
    अचिन्हित ivs = env.get_ivs ();
    स्थिर cff2_instancing_plan_t::vardata_transform_t *t = instancer.get_transform (ivs);
    अगर (unlikely (!t))
    {
      env.set_error ();
      वापस;
    }

    अचिन्हित n = arg.अंक_मूल्यावली_2;
    क्रम (अचिन्हित पूर्णांक j = 0; j < n; j++)
    {
      स्थिर blend_arg_t &arg1 = env.arg_ढेर[i + j];
      अगर (unlikely (!(arg1.blending () && (arg1.अंक_मूल्यावली_2 == n) && (arg1.मूल्य_सूचकांक_2 == j) &&
		      (arg1.deltas.लंबाई == env.get_region_गिनती ()) &&
		      (arg1.deltas.लंबाई == t->gains.लंबाई))))
      {
	env.set_error ();
	वापस;
      }
    }

    str_encoder_t encoder (param.flatStr);
    अचिन्हित k_new = t->alive () ? t->matrix.लंबाई : 0;

    अगर (!k_new)
    {
      /* All of this VarData's regions died; blends dissolve into statics. */
      क्रम (अचिन्हित पूर्णांक j = 0; j < n; j++)
      {
	स्थिर blend_arg_t &arg1 = env.arg_ढेर[i + j];
	संख्या_t v;
	v.set_real (arg1.to_real () + round (instancer.fold_deltas (ivs, arg1.deltas.as_array ())));
	encoder.encode_अंक_cs (v);
      }
      वापस;
    }

    /* Emit in chunks so the output charstring stays within the argument
     * stack limit: values already emitted for this op, plus this chunk's
     * defaults, deltas and count, must not exceed 513. */
    अचिन्हित done = 0;
    जबतक (done < n)
    {
      अचिन्हित avail = 512 > (i + done) ? 512 - (i + done) : 0;
      अचिन्हित chunk = hb_min (avail / (k_new + 1), n - done);
      अगर (unlikely (!chunk))
      {
	env.set_error ();
	वापस;
      }

      क्रम (अचिन्हित पूर्णांक j = done; j < done + chunk; j++)
      {
	स्थिर blend_arg_t &arg1 = env.arg_ढेर[i + j];
	संख्या_t v;
	v.set_real (arg1.to_real () + round (instancer.fold_deltas (ivs, arg1.deltas.as_array ())));
	encoder.encode_अंक_cs (v);
      }
      क्रम (अचिन्हित पूर्णांक j = done; j < done + chunk; j++)
      {
	स्थिर blend_arg_t &arg1 = env.arg_ढेर[i + j];
	क्रम (अचिन्हित r = 0; r < k_new; r++)
	{
	  संख्या_t v;
	  v.set_int (hb_clamp_to<पूर्णांक> (round (t->transform_delta (r, arg1.deltas.as_array ()))));
	  encoder.encode_अंक_cs (v);
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
  अटल व्योम emit_vsindex_prefix (cff2_cs_interp_env_t<blend_arg_t> &env, flatten_param_t& param)
  {
    अगर (!param.emitted_blend) वापस;
    स्थिर cff2_instancing_plan_t &instancer = *param.instancer;
    अचिन्हित new_ivs = instancer.remap_ivs (env.get_ivs ());
    अचिन्हित fd_ivs = instancer.remap_ivs (env.get_orig_ivs ());
    अगर (new_ivs == fd_ivs) वापस;

    str_buff_t prefix;
    str_encoder_t enc (prefix);
    enc.encode_int ((पूर्णांक) new_ivs);
    enc.encode_op (OpCode_vsindexcs);

    str_buff_t &buf = param.flatStr;
    अचिन्हित old_len = buf.लंबाई;
    अगर (unlikely (prefix.in_error () || !buf.resize (old_len + prefix.लंबाई)))
    {
      env.set_error ();
      वापस;
    }
    स_हटाओ (buf.arrayZ + prefix.लंबाई, buf.arrayZ, old_len);
    hb_memcpy (buf.arrayZ, prefix.arrayZ, prefix.लंबाई);
  }

  अटल व्योम flush_op (op_code_t op, cff2_cs_interp_env_t<blend_arg_t> &env, flatten_param_t& param)
  {
    चयन (op)
    {
      हाल OpCode_return:
      हाल OpCode_endchar:
	वापस;
      शेष:
	str_encoder_t  encoder (param.flatStr);
	encoder.encode_op (op);
    }
  }

  अटल व्योम flush_hintmask (op_code_t op, cff2_cs_interp_env_t<blend_arg_t> &env, flatten_param_t& param)
  {
    SUPER::flush_hintmask (op, env, param);
    /* Preserve hintmask payload in captured commands for specializer re-encoding. */
    अगर (param.commands && !param.drop_hints && param.commands->लंबाई > 0)
    {
      स्वतः &cmd = param.commands->पुच्छ ();
      अगर (cmd.op == op)
      {
        cmd.mask_bytes.resize (env.hintmask_आकार);
        अगर (unlikely (cmd.mask_bytes.in_error ()))
        {
          env.set_error ();
          वापस;
        }
        क्रम (अचिन्हित पूर्णांक i = 0; i < env.hintmask_आकार; i++)
          cmd.mask_bytes[i] = env.str_ref[i];
      }
    }
    अगर (!param.drop_hints)
    {
      str_encoder_t  encoder (param.flatStr);
      क्रम (अचिन्हित पूर्णांक i = 0; i < env.hintmask_आकार; i++)
	encoder.encode_byte (env.str_ref[i]);
    }
  }

  निजी:
  प्रकार cff2_cs_opset_t<cff2_cs_opset_flatten_t, flatten_param_t, blend_arg_t> SUPER;
  प्रकार cs_opset_t<blend_arg_t, cff2_cs_opset_flatten_t, cff2_cs_opset_flatten_t, cff2_cs_interp_env_t<blend_arg_t>, flatten_param_t> CSOPSET;
};

काष्ठा cff2_cs_opset_subr_subset_t : cff2_cs_opset_t<cff2_cs_opset_subr_subset_t, subr_subset_param_t, blend_arg_t>
{
  अटल व्योम process_op (op_code_t op, cff2_cs_interp_env_t<blend_arg_t> &env, subr_subset_param_t& param)
  {
    चयन (op) {

      हाल OpCode_return:
	param.current_parsed_str->flush_coalesced (env.str_ref);
	param.current_parsed_str->set_parsed ();
	env.return_from_subr ();
	param.set_current_str (env, false);
	अवरोध;

      हाल OpCode_endchar:
	param.current_parsed_str->flush_coalesced (env.str_ref);
	param.current_parsed_str->set_parsed ();
	SUPER::process_op (op, env, param);
	अवरोध;

      हाल OpCode_callsubr:
	process_call_subr (op, CSType_LocalSubr, env, param, env.localSubrs, param.local_closure);
	अवरोध;

      हाल OpCode_callgsubr:
	process_call_subr (op, CSType_GlobalSubr, env, param, env.globalSubrs, param.global_closure);
	अवरोध;

      शेष:
	SUPER::process_op (op, env, param);
	param.current_parsed_str->add_op (op, env.str_ref);
	अवरोध;
    }
  }

  रक्षित:
  अटल व्योम process_call_subr (op_code_t op, cs_type_t type,
				 cff2_cs_interp_env_t<blend_arg_t> &env, subr_subset_param_t& param,
				 cff2_biased_subrs_t& subrs, hb_set_t *closure)
  {
    byte_str_ref_t    str_ref = env.str_ref;
    env.call_subr (subrs, type);
    param.current_parsed_str->add_call_op (op, str_ref, env.context.subr_अंक_2);
    closure->add (env.context.subr_अंक_2);
    param.set_current_str (env, true);
  }

  निजी:
  प्रकार cff2_cs_opset_t<cff2_cs_opset_subr_subset_t, subr_subset_param_t, blend_arg_t> SUPER;
};

काष्ठा cff2_subr_subsetter_t : subr_subsetter_t<cff2_subr_subsetter_t, CFF2Subrs, स्थिर OT::cff2::accelerator_subset_t, cff2_cs_interp_env_t<blend_arg_t>, cff2_cs_opset_subr_subset_t>
{
  cff2_subr_subsetter_t (स्थिर OT::cff2::accelerator_subset_t &acc_, स्थिर hb_subset_plan_t *plan_)
    : subr_subsetter_t (acc_, plan_) {}

  अटल व्योम complete_parsed_str (cff2_cs_interp_env_t<blend_arg_t> &env, subr_subset_param_t& param, parsed_cs_str_t &charstring)
  {
    /* vsindex is inserted at the beginning of the charstring as necessary */
    अगर (env.seen_vsindex ())
    {
      संख्या_t  ivs;
      ivs.set_int ((पूर्णांक)env.get_ivs ());
      charstring.set_prefix (ivs, OpCode_vsindexcs);
    }
  }
};

काष्ठा cff2_private_blend_encoder_param_t
{
  cff2_private_blend_encoder_param_t (hb_serialize_context_t *c,
				      स्थिर CFF2ItemVariationStore *varStore,
				      hb_array_t<पूर्णांक> normalized_coords) :
    c (c), varStore (varStore), normalized_coords (normalized_coords) {}

  व्योम init () {}

  व्योम process_blend ()
  {
    अगर (!seen_blend)
    {
      region_गिनती_2 = varStore->varStore.get_region_सूचकांक_गिनती (ivs);
      अगर (unlikely (!scalars.resize_exact (region_गिनती_2)))
      {
	अगर (c) c->err (HB_SERIALIZE_ERROR_OTHER);
	seen_blend = true;
	वापस;
      }
      varStore->varStore.get_region_scalars (ivs, normalized_coords.arrayZ, normalized_coords.लंबाई,
					     &scalars[0], region_गिनती_2);
      seen_blend = true;
    }
  }

  द्विगुन blend_deltas (hb_array_t<स्थिर संख्या_t> deltas) स्थिर
  {
    द्विगुन v = 0;
    अगर (likely (scalars.लंबाई == deltas.लंबाई))
    {
      अचिन्हित गिनती = scalars.लंबाई;
      क्रम (अचिन्हित i = 0; i < गिनती; i++)
	v += (द्विगुन) scalars.arrayZ[i] * deltas.arrayZ[i].to_real ();
    }
    वापस v;
  }


  hb_serialize_context_t *c = nullptr;
  bool seen_blend = false;
  अचिन्हित ivs = 0;
  अचिन्हित region_गिनती_2 = 0;
  hb_vector_t<भग्न> scalars;
  स्थिर	 CFF2ItemVariationStore *varStore = nullptr;
  hb_array_t<पूर्णांक> normalized_coords;

  /* CFF2 partial instancing: when set, blends are rewritten against the
   * instanced variation store instead of being flattened. */
  स्थिर cff2_instancing_plan_t *instancer = nullptr;
  /* Error-compensated fold rounding state, per dict operator. */
  द्विगुन fold_exact = 0.;
  द्विगुन fold_emitted = 0.;
};

काष्ठा cff2_private_dict_blend_opset_t : dict_opset_t
{
  अटल व्योम process_arg_blend (cff2_private_blend_encoder_param_t& param,
				 संख्या_t &arg,
				 स्थिर hb_array_t<स्थिर संख्या_t> blends,
				 अचिन्हित n, अचिन्हित i)
  {
    arg.set_int (round (arg.to_real () + param.blend_deltas (blends)));
  }

  /* The fold added to a value, error-compensated across the operator: most
   * blended private-dict values (BlueValues, StemSnap*, ...) are
   * delta-encoded, so independently rounded folds would accumulate in the
   * decoded absolutes; keeping the emitted cumulative fold equal to the
   * rounded exact cumulative fold bounds every absolute within ±0.5. */
  अटल द्विगुन fold (cff2_private_blend_encoder_param_t& param,
		      स्थिर hb_array_t<स्थिर संख्या_t> blends)
  {
    द्विगुन f = param.blend_deltas (blends);
    द्विगुन emit = round (param.fold_exact + f) - param.fold_emitted;
    param.fold_exact += f;
    param.fold_emitted += emit;
    वापस emit;
  }

  /* Partial instancing: emit the args preceding this blend group, then the
   * group itself rewritten against the instanced store, and clear the
   * stack.  The op's remaining args are flushed at process_op time. */
  अटल व्योम rewrite_blends (cff2_priv_dict_interp_env_t& env,
			      cff2_private_blend_encoder_param_t& param,
			      अचिन्हित start, अचिन्हित n, अचिन्हित k)
  {
    स्थिर cff2_instancing_plan_t &instancer = *param.instancer;
    स्थिर cff2_instancing_plan_t::vardata_transform_t *t = instancer.get_transform (param.ivs);
    अगर (unlikely (!t || t->gains.लंबाई != k))
    {
      env.set_error ();
      वापस;
    }

    str_buff_t str;
    str_encoder_t encoder (str);

    क्रम (अचिन्हित idx = 0; idx < start; idx++)
      encoder.encode_अंक_tp (env.arg_ढेर[idx]);

    अचिन्हित k_new = t->alive () ? t->matrix.लंबाई : 0;
    अगर (!k_new)
    {
      /* This VarData died (or is unused by any retained charstring, which
       * can happen for FDs kept only for retain-gids holes); blends
       * dissolve into statics. */
      क्रम (अचिन्हित j = 0; j < n; j++)
      {
	स्थिर hb_array_t<स्थिर संख्या_t> blends = env.arg_ढेर.sub_array (start + n + (j * k), k);
	संख्या_t v;
	v.set_real (env.arg_ढेर[start + j].to_real () + fold (param, blends));
	encoder.encode_अंक_tp (v);
      }
    }
    अन्यथा
    {
      अचिन्हित done = 0;
      जबतक (done < n)
      {
	अचिन्हित avail = 512 > (start + done) ? 512 - (start + done) : 0;
	अचिन्हित chunk = hb_min (avail / (k_new + 1), n - done);
	अगर (unlikely (!chunk))
	{
	  env.set_error ();
	  वापस;
	}

	क्रम (अचिन्हित j = done; j < done + chunk; j++)
	{
	  स्थिर hb_array_t<स्थिर संख्या_t> blends = env.arg_ढेर.sub_array (start + n + (j * k), k);
	  संख्या_t v;
	  v.set_real (env.arg_ढेर[start + j].to_real () + fold (param, blends));
	  encoder.encode_अंक_tp (v);
	}
	क्रम (अचिन्हित j = done; j < done + chunk; j++)
	{
	  स्थिर hb_array_t<स्थिर संख्या_t> blends = env.arg_ढेर.sub_array (start + n + (j * k), k);
	  क्रम (अचिन्हित r = 0; r < k_new; r++)
	  {
	    संख्या_t v;
	    v.set_int (hb_clamp_to<पूर्णांक> (round (t->transform_delta (r, blends))));
	    encoder.encode_अंक_tp (v);
	  }
	}
	encoder.encode_int (chunk);
	encoder.encode_op (OpCode_blenddict);
	done += chunk;
      }
    }

    स्वतः bytes = str.as_bytes ();
    param.c->embed (&bytes, bytes.लंबाई);

    env.clear_args ();
  }

  अटल व्योम process_blend (cff2_priv_dict_interp_env_t& env, cff2_private_blend_encoder_param_t& param)
  {
    अचिन्हित पूर्णांक n, k;

    param.process_blend ();
    k = param.region_गिनती_2;
    n = env.arg_ढेर.निकालो_uint ();

    अचिन्हित गिनती = env.arg_ढेर.get_गिनती ();
    अचिन्हित पूर्णांक योग;
    अगर (unlikely (hb_unsigned_mul_overflows (k + 1, n, &योग) || योग > गिनती))
    {
      env.set_error ();
      वापस;
    }

    /* copy the blend values into blend array of the default values */
    अचिन्हित पूर्णांक start = गिनती - योग;
    /* let an obvious error case fail, but note CFF2 spec doesn't forbid n==0 */
    अगर (unlikely (start > गिनती))
    {
      env.set_error ();
      वापस;
    }

    अगर (param.instancer && k)
    {
      rewrite_blends (env, param, start, n, k);
      वापस;
    }

    क्रम (अचिन्हित पूर्णांक i = 0; i < n; i++)
    {
      स्थिर hb_array_t<स्थिर संख्या_t> blends = env.arg_ढेर.sub_array (start + n + (i * k), k);
      process_arg_blend (param, env.arg_ढेर[start + i], blends, n, i);
    }

    /* pop off blend values leaving default values now adorned with blend values */
    env.arg_ढेर.निकालो (k * n);
  }

  अटल व्योम process_op (op_code_t op, cff2_priv_dict_interp_env_t& env, cff2_private_blend_encoder_param_t& param)
  {
    चयन (op) {
      हाल OpCode_StdHW:
      हाल OpCode_StdVW:
      हाल OpCode_BlueScale:
      हाल OpCode_BlueShift:
      हाल OpCode_BlueFuzz:
      हाल OpCode_ExpansionFactor:
      हाल OpCode_LanguageGroup:
      हाल Op_Code_Blue_मूल्यावली:
      हाल OpCode_OtherBlues:
      हाल OpCode_FamilyBlues:
      हाल OpCode_FamilyOtherBlues:
      हाल OpCode_StemSnapH:
      हाल OpCode_StemSnapV:
	अवरोध;
      हाल OpCode_vsindexdict:
	env.process_vsindex ();
	param.ivs = env.get_ivs ();
	अगर (param.instancer)
	{
	  /* Re-emit with the remapped vsindex; 0 is the dict default. */
	  अचिन्हित new_ivs = param.instancer->remap_ivs (param.ivs);
	  अगर (new_ivs)
	  {
	    str_buff_t str;
	    str_encoder_t encoder (str);
	    encoder.encode_int ((पूर्णांक) new_ivs);
	    encoder.encode_op (OpCode_vsindexdict);
	    स्वतः bytes = str.as_bytes ();
	    param.c->embed (&bytes, bytes.लंबाई);
	  }
	}
	env.clear_args ();
	वापस;
      हाल OpCode_blenddict:
	process_blend (env, param);
	वापस;

      शेष:
	dict_opset_t::process_op (op, env);
	अगर (!env.arg_ढेर.is_empty ()) वापस;
	अवरोध;
    }

    अगर (unlikely (env.in_error ())) वापस;

    // Write args then op

    str_buff_t str;
    str_encoder_t encoder (str);

    अचिन्हित गिनती = env.arg_ढेर.get_गिनती ();
    क्रम (अचिन्हित i = 0; i < गिनती; i++)
      encoder.encode_अंक_tp (env.arg_ढेर[i]);

    encoder.encode_op (op);

    स्वतः bytes = str.as_bytes ();
    param.c->embed (&bytes, bytes.लंबाई);

    env.clear_args ();
    param.fold_exact = param.fold_emitted = 0.;
  }
};

काष्ठा cff2_private_dict_op_serializer_t : op_serializer_t
{
  cff2_private_dict_op_serializer_t (bool desubroutinize_, bool drop_hints_, bool pinned_,
				     स्थिर CFF::CFF2ItemVariationStore* varStore_,
				     hb_array_t<पूर्णांक> normalized_coords_,
				     स्थिर cff2_instancing_plan_t *instancer_ = nullptr)
    : desubroutinize (desubroutinize_), drop_hints (drop_hints_), pinned (pinned_),
      param (nullptr, varStore_, normalized_coords_)
  { param.instancer = instancer_; }

  bool serialize (hb_serialize_context_t *c,
		  स्थिर op_str_t &opstr,
		  objidx_t subrs_link) स्थिर
  {
    TRACE_SERIALIZE (यह);

    अगर (drop_hints && dict_opset_t::is_hint_op (opstr.op))
      return_trace (true);

    अगर (opstr.op == OpCode_Subrs)
    {
      अगर (desubroutinize || !subrs_link)
	return_trace (true);
      अन्यथा
	return_trace (FontDict::serialize_link2_op (c, opstr.op, subrs_link));
    }

    अगर (pinned || param.instancer)
    {
      /* Reinterpret opstr and process blends.  The param persists across
       * this dict's opstrs, so that an ivs set by a vsindexdict opstr is
       * still in effect when a later opstr's blends are processed. */
      cff2_priv_dict_interp_env_t env {hb_ubytes_t (opstr.ptr, opstr.लंबाई)};
      param.c = c;
      dict_interpreter_t<cff2_private_dict_blend_opset_t, cff2_private_blend_encoder_param_t, cff2_priv_dict_interp_env_t> interp (env);
      return_trace (interp.interpret (param));
    }

    return_trace (copy_opstr (c, opstr));
  }

  रक्षित:
  स्थिर bool desubroutinize;
  स्थिर bool drop_hints;
  स्थिर bool pinned;
  mutable cff2_private_blend_encoder_param_t param;
};



namespace OT {
काष्ठा cff2_subset_plan
{
  bool create (स्थिर OT::cff2::accelerator_subset_t &acc,
	      hb_subset_plan_t *plan)
  {
    /* make sure notdef is first */
    hb_codepoint_t old_glyph;
    अगर (!plan->old_gid_for_new_gid (0, &old_glyph) || (old_glyph != 0)) वापस false;

    अंक_glyphs = plan->अंक_output_glyphs ();
    orig_fdcount = acc.fdArray->गिनती;

    drop_hints = plan->flags & HB_SUBSET_FLAGS_NO_HINTING;
    bool instancing_requested = (bool) plan->normalized_coords;
    pinned = instancing_requested && plan->all_axes_pinned;
    normalized_coords = plan->normalized_coords;
    शीर्ष_maxp_info = plan->शीर्ष_maxp_info;
    hmtx_map = &plan->hmtx_map;
    desubroutinize = plan->flags & HB_SUBSET_FLAGS_DESUBROUTINIZE ||
		     instancing_requested; // For instancing we need this path

    /* Partial instancing: some axes pinned or limited, others kept. */
    partial_instancing = instancing_requested && !pinned &&
			 acc.varStore != &Null (CFF2ItemVariationStore);
    अगर (partial_instancing &&
	unlikely (!instancing.create (acc.varStore, plan)))
      वापस false;
    axis_tags = &plan->axis_tags;

    /* Enable command capture for CFF2→CFF1 conversion (for specialization) */
    capture_commands = pinned;

 #अगर_घोषित HB_EXPERIMENTAL_API
    min_charstrings_off_आकार = (plan->flags & HB_SUBSET_FLAGS_IFTB_REQUIREMENTS) ? 4 : 0;
 #अन्यथा
    min_charstrings_off_आकार = 0;
 #पूर्ण_अगर

    अगर (desubroutinize)
    {
      अगर (partial_instancing)
      {
	/* Pass 1: collect which VarDatas the retained blends reference, so
	 * unused ones get pruned and the survivors renumbered. */
	hb_set_t used_fds;
	क्रम (अचिन्हित पूर्णांक i = 0; i < अंक_glyphs; i++)
	{
	  hb_codepoint_t glyph;
	  अगर (!plan->old_gid_for_new_gid (i, &glyph))
	    जारी;
	  स्थिर hb_ubytes_t str = (*acc.charStrings)[glyph];
	  अचिन्हित पूर्णांक fd = CFF::subset_get_fd (acc, glyph);
	  अगर (unlikely (fd >= acc.fd_गिनती))
	    वापस false;
	  used_fds.add (fd);

	  cff2_cs_interp_env_t<blend_arg_t> env (str, acc, fd);
	  cs_interpreter_t<cff2_cs_interp_env_t<blend_arg_t>, cff2_cs_opset_usage_t, cff2_usage_param_t> interp (env);
	  cff2_usage_param_t param;
	  param.instancing = &instancing;
	  param.drop_hints = drop_hints;
	  अगर (unlikely (!interp.interpret (param)))
	    वापस false;
	}
	क्रम (अचिन्हित fd : used_fds)
	{
	  cff2_usage_param_t param;
	  param.instancing = &instancing;
	  अचिन्हित गिनती = acc.privateDicts[fd].get_गिनती ();
	  क्रम (अचिन्हित j = 0; j < गिनती; j++)
	  {
	    स्थिर op_str_t &opstr = acc.privateDicts[fd][j];
	    अगर (drop_hints && dict_opset_t::is_hint_op (opstr.op))
	      जारी;
	    cff2_priv_dict_interp_env_t env {hb_ubytes_t (opstr.ptr, opstr.लंबाई)};
	    dict_interpreter_t<cff2_private_dict_usage_opset_t, cff2_usage_param_t, cff2_priv_dict_interp_env_t> interp (env);
	    अगर (unlikely (!interp.interpret (param)))
	      वापस false;
	  }
	}
	अगर (unlikely (!instancing.finalize ()))
	  वापस false;

	/* Pass 2: like subr_flattener_t, but blends stay symbolic (no coords
	 * fed to the env) and get rewritten against the instanced store. */
	अगर (!subset_charstrings.resize_exact (अंक_glyphs))
	  वापस false;
	क्रम (अचिन्हित पूर्णांक i = 0; i < अंक_glyphs; i++)
	{
	  hb_codepoint_t glyph;
	  अगर (!plan->old_gid_for_new_gid (i, &glyph))
	    जारी;
	  स्थिर hb_ubytes_t str = (*acc.charStrings)[glyph];
	  अचिन्हित पूर्णांक fd = CFF::subset_get_fd (acc, glyph);
	  अगर (unlikely (fd >= acc.fd_गिनती))
	    वापस false;

	  cff2_cs_interp_env_t<blend_arg_t> env (str, acc, fd);
	  cs_interpreter_t<cff2_cs_interp_env_t<blend_arg_t>, cff2_cs_opset_flatten_t, flatten_param_t> interp (env);
	  flatten_param_t param = {
	    subset_charstrings.arrayZ[i],
	    (bool) (plan->flags & HB_SUBSET_FLAGS_NO_HINTING),
	    plan
	  };
	  param.instancer = &instancing;
	  अगर (unlikely (!interp.interpret (param)))
	    वापस false;
	}
      }
      अन्यथा
      {
      /* Flatten global & local subrs */
      subr_flattener_t<स्थिर OT::cff2::accelerator_subset_t, cff2_cs_interp_env_t<blend_arg_t>, cff2_cs_opset_flatten_t>
		    flattener(acc, plan);

      /* Enable command capture if requested (for specialization) */
      अगर (capture_commands)
      {
	अगर (!charstring_commands.resize_exact (अंक_glyphs))
	  वापस false;

	अगर (!flattener.flatten (subset_charstrings, &charstring_commands))
	  वापस false;
      }
      अन्यथा
      {
	अगर (!flattener.flatten (subset_charstrings))
	  वापस false;
      }
      }
    }
    अन्यथा
    {
      cff2_subr_subsetter_t	subr_subsetter (acc, plan);

      /* Subset subrs: collect used subroutines, leaving all unused ones behind */
      अगर (!subr_subsetter.subset ())
	वापस false;

      /* encode charstrings, global subrs, local subrs with new subroutine numbers */
      अगर (!subr_subsetter.encode_charstrings (subset_charstrings, !pinned))
	वापस false;

      अगर (!subr_subsetter.encode_globalsubrs (subset_globalsubrs))
	वापस false;

      /* local subrs */
      अगर (!subset_localsubrs.resize (orig_fdcount))
	वापस false;
      क्रम (अचिन्हित पूर्णांक fd = 0; fd < orig_fdcount; fd++)
      {
	subset_localsubrs[fd].init ();
	अगर (!subr_subsetter.encode_localsubrs (fd, subset_localsubrs[fd]))
	  वापस false;
      }
    }

    /* FDSelect */
    अगर (acc.fdSelect != &Null (CFF2FDSelect))
    {
      अगर (unlikely (!hb_plan_subset_cff_fdselect (plan,
						  orig_fdcount,
						  *(स्थिर FDSelect *)acc.fdSelect,
						  acc.अंक_glyphs,
						  subset_fdcount,
						  subset_fdselect_आकार,
						  subset_fdselect_format,
						  subset_fdselect_ranges,
						  fdmap)))
	वापस false;
    }
    अन्यथा
      fdmap.identity (1);

    वापस true;
  }

  cff2_sub_table_info_t info;

  अचिन्हित पूर्णांक    अंक_glyphs;
  अचिन्हित पूर्णांक    orig_fdcount = 0;
  अचिन्हित पूर्णांक    subset_fdcount = 1;
  अचिन्हित पूर्णांक    subset_fdselect_आकार = 0;
  अचिन्हित पूर्णांक    subset_fdselect_format = 0;
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
  स्थिर hb_vector_t<hb_tag_t> *axis_tags = nullptr;

  अचिन्हित  min_charstrings_off_आकार = 0;

  hb_array_t<पूर्णांक> normalized_coords; // For instantiation
  शीर्ष_maxp_info_t शीर्ष_maxp_info;  // For FontBBox
  स्थिर hb_hashmap_t<hb_codepoint_t, hb_pair_t<अचिन्हित, पूर्णांक>> *hmtx_map; // For widths

  // Width optimization results (for CFF1 conversion)
  अचिन्हित default_width = 0;
  अचिन्हित nominal_width = 0;

  // Command capture for specialization (CFF2→CFF1 conversion)
  bool capture_commands = false;
  hb_vector_t<hb_vector_t<cs_command_t>> charstring_commands;
};
} // namespace OT

/*
 * CFF2 to CFF1 Converter Implementation
 */

#समावेश "hb-cff-width-optimizer.hh"
#समावेश "hb-cff-specializer.hh"

/* Serialize charstrings using CFF1 format with widths */
अटल bool
_serialize_cff1_charstrings (hb_serialize_context_t *c,
                             OT::cff2_subset_plan &plan,
                             अचिन्हित default_width,
                             अचिन्हित nominal_width)
{
  c->push ();

  // CFF1 requires:
  // 1. Width at the beginning (if != defaultWidthX)
  // 2. endchar at the end
  str_buff_vec_t cff1_charstrings;
  अगर (unlikely (!cff1_charstrings.resize (plan.subset_charstrings.लंबाई)))
  {
    c->निकालो_discard ();
    वापस false;
  }

  क्रम (अचिन्हित i = 0; i < plan.subset_charstrings.लंबाई; i++)
  {
    // Get width for this glyph from hmtx_map
    अचिन्हित width = 0;
    अगर (plan.hmtx_map->has (i))
      width = plan.hmtx_map->लो (i).first;

    // Encode width if different from default
    str_encoder_t encoder (cff1_charstrings[i]);
    अगर (width != default_width)
    {
      पूर्णांक delta = (पूर्णांक) width - (पूर्णांक) nominal_width;
      encoder.encode_int (delta);
    }

    // Use specialized commands if available, otherwise use binary
    अगर (plan.capture_commands && i < plan.charstring_commands.लंबाई &&
        plan.charstring_commands[i].लंबाई > 0)
    {
      // Specialize and encode commands
      स्वतः &commands = plan.charstring_commands[i];
      CFF::specialize_commands (commands, 48);  /* maxstack=48 for CFF1 */
      अगर (unlikely (!CFF::encode_commands (commands, cff1_charstrings[i])))
      {
        c->निकालो_discard ();
        वापस false;
      }
    }
    अन्यथा
    {
      // Use binary CharString
      स्थिर str_buff_t &cs = plan.subset_charstrings[i];
      क्रम (अचिन्हित j = 0; j < cs.लंबाई; j++)
        cff1_charstrings[i].push (cs[j]);
    }

    // Check if it already ends with endchar (0x0e) or return (0x0b)
    अगर (cff1_charstrings[i].लंबाई == 0 ||
        (cff1_charstrings[i].पुच्छ () != 0x0e && cff1_charstrings[i].पुच्छ () != 0x0b))
    {
      // Append endchar operator
      अगर (unlikely (!cff1_charstrings[i].धकेलो_or_fail (0x0e)))
      {
        c->निकालो_discard ();
        वापस false;
      }
    }
  }

  अचिन्हित तथ्य_आकार = 0;
  अचिन्हित योग_आकार = CFF1CharStrings::योग_आकार (cff1_charstrings, &तथ्य_आकार, plan.min_charstrings_off_आकार);
  अगर (unlikely (!c->start_zerocopy (योग_आकार)))
  {
    c->निकालो_discard ();
    वापस false;
  }

  स्वतः *cs = c->start_embed<CFF1CharStrings> ();
  अगर (unlikely (!cs->serialize (c, cff1_charstrings, &तथ्य_आकार, plan.min_charstrings_off_आकार)))
  {
    c->निकालो_discard ();
    वापस false;
  }

  plan.info.char_strings_link = c->निकालो_pack (false);
  वापस true;
}

/* Serialize CID Charset (format 2 range: gid 0-N -> cid 0-N) */
अटल bool
_serialize_cff1_charset (hb_serialize_context_t *c,
                         अचिन्हित पूर्णांक अंक_glyphs,
                         objidx_t &charset_link)
{
  // For CID fonts, create a simple identity charset
  // Format 2: one range covering all glyphs (except .notdef)
  c->push ();

  स्वतः *charset = c->start_embed<Charset> ();
  अगर (unlikely (!charset))
  {
    c->निकालो_discard ();
    वापस false;
  }

  // Create a single range for CID 1 to num_glyphs-1
  hb_vector_t<code_pair_t> ranges;
  अगर (अंक_glyphs > 1)
  {
    code_pair_t range;
    range.code = 1;  // first CID
    range.glyph = अंक_glyphs - 2;  // nLeft (covers glyphs 1 to num_glyphs-1)
    ranges.push (range);
  }

  अगर (unlikely (!charset->serialize (c, 2, अंक_glyphs, ranges)))
  {
    c->निकालो_discard ();
    वापस false;
  }

  charset_link = c->निकालो_pack ();
  वापस true;
}

/* CFF2 to CFF1 serialization */
namespace CFF {

bool
serialize_cff2_to_cff1 (hb_serialize_context_t *c,
                        OT::cff2_subset_plan &plan,
                        स्थिर cff2_शीर्ष_dict_मूल्यावली_t &cff2_शीर्ष_Dict,
                        स्थिर OT::cff2::accelerator_subset_t &acc)
{
  TRACE_SERIALIZE (यह);

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
    hb_vector_t<अचिन्हित> widths;
    widths.alloc (plan.अंक_glyphs);

    क्रम (अचिन्हित gid = 0; gid < plan.अंक_glyphs; gid++)
    {
      अचिन्हित width = 0;
      अगर (plan.hmtx_map->has (gid))
        width = plan.hmtx_map->लो (gid).first;
      widths.push (width);
    }

    // Optimize defaultWidthX and nominalWidthX
    CFF::optimize_widths (widths, plan.default_width, plan.nominal_width);
  }

  // 1. CharStrings (with widths prepended)
  अगर (!_serialize_cff1_charstrings (c, plan, plan.default_width, plan.nominal_width))
    return_trace (false);

  // 2. Private DICs & Local Subrs (same as CFF2)
  hb_vector_t<table_info_t> private_dict_infos;
  अगर (unlikely (!private_dict_infos.resize (plan.subset_fdcount)))
    return_trace (false);

  क्रम (पूर्णांक i = (पूर्णांक)acc.privateDicts.लंबाई; --i >= 0;)
  {
    अगर (plan.fdmap.has (i))
    {
      objidx_t subrs_link = 0;

      अगर (plan.subset_localsubrs[i].लंबाई > 0)
      {
        स्वतः *dest = c->push<CFF1Subrs> ();
        अगर (likely (dest->serialize (c, plan.subset_localsubrs[i])))
          subrs_link = c->निकालो_pack (false);
        अन्यथा
        {
          c->निकालो_discard ();
          return_trace (false);
        }
      }

      स्वतः *pd = c->push<PrivateDict> ();
      // Use the CFF2 Private DICT serializer which instantiates blends when pinned=true
      cff2_private_dict_op_serializer_t privSzr (plan.desubroutinize, plan.drop_hints, plan.pinned,
                                                  acc.varStore, plan.normalized_coords);
      अगर (likely (pd->serialize (c, acc.privateDicts[i], privSzr, subrs_link)))
      {
        // Add defaultWidthX and nominalWidthX for CFF1
        str_buff_t width_ops;
        str_encoder_t encoder (width_ops);
        encoder.encode_int (plan.default_width);
        encoder.encode_op (OpCode_defaultWidthX);
        encoder.encode_int (plan.nominal_width);
        encoder.encode_op (OpCode_nominalWidthX);

        अगर (!encoder.in_error () && c->embed (width_ops.as_bytes ().arrayZ, width_ops.लंबाई))
        {
          अचिन्हित fd = plan.fdmap[i];
          private_dict_infos[fd].आकार = c->लंबाई ();
          private_dict_infos[fd].link = c->निकालो_pack ();
        }
        अन्यथा
        {
          c->निकालो_discard ();
          return_trace (false);
        }
      }
      अन्यथा
      {
        c->निकालो_discard ();
        return_trace (false);
      }
    }
  }

  // 3. FDArray - serialize CFF2 font dicts as CFF1
  {
    स्वतः *fda = c->push<FDArray<HBUINT16>> ();
    cff_font_dict_op_serializer_t fontSzr;
    स्वतः it =
    + hb_zip (+ hb_iter (acc.fontDicts)
              | hb_filter ([&] (स्थिर cff2_font_dict_मूल्यावली_t &_)
                { वापस plan.fdmap.has (&_ - &acc.fontDicts[0]); }),
              hb_iter (private_dict_infos))
    ;
    // Explicitly specify template parameters: DICTVAL, INFO
    bool success = fda->serialize<cff2_font_dict_मूल्यावली_t, table_info_t> (c, it, fontSzr);
    अगर (success)
      plan.info.fd_array_link = c->निकालो_pack (false);
    अन्यथा
    {
      c->निकालो_discard ();
      return_trace (false);
    }
  }

  // 4. FDSelect (required in CFF1 CID-keyed fonts)
  // CFF1 requires FDSelect for all CID-keyed fonts, even with just one FD
  // CFF2 makes it optional when there's only one FD
  अगर (acc.fdSelect != &Null (CFF2FDSelect))
  {
    c->push ();
    अगर (likely (hb_serialize_cff_fdselect (c, plan.अंक_glyphs,
                                          *(स्थिर FDSelect *)acc.fdSelect,
                                          plan.orig_fdcount,
                                          plan.subset_fdselect_format,
                                          plan.subset_fdselect_आकार,
                                          plan.subset_fdselect_ranges)))
      plan.info.fd_select.link = c->निकालो_pack ();
    अन्यथा
    {
      c->निकालो_discard ();
      return_trace (false);
    }
  }
  अन्यथा
  {
    // Create a range-based FDSelect3 mapping all glyphs to FD 0
    // Format: format(1) + nRanges(2) + range(3) + sentinel(2) = 8 bytes
    c->push ();

    // Format byte
    HBUINT8 format;
    format = 3;
    अगर (unlikely (!c->embed (format)))
    {
      c->निकालो_discard ();
      return_trace (false);
    }

    // nRanges
    HBUINT16 nRanges;
    nRanges = 1;
    अगर (unlikely (!c->embed (nRanges)))
    {
      c->निकालो_discard ();
      return_trace (false);
    }

    // Single range: {first: 0, fd: 0}
    FDSelect3_Range range;
    range.first = 0;
    range.fd = 0;
    अगर (unlikely (!c->embed (range)))
    {
      c->निकालो_discard ();
      return_trace (false);
    }

    // Sentinel (number of glyphs)
    HBUINT16 sentinel;
    sentinel = plan.अंक_glyphs;
    अगर (unlikely (!c->embed (sentinel)))
    {
      c->निकालो_discard ();
      return_trace (false);
    }

    plan.info.fd_select.link = c->निकालो_pack ();
  }

  // 5. Charset (CID charset for identity mapping)
  objidx_t charset_link;
  अगर (!_serialize_cff1_charset (c, plan.अंक_glyphs, charset_link))
    return_trace (false);

  // 6. Global Subrs
  {
    स्वतः *dest = c->push<CFF1Subrs> ();
    अगर (likely (dest->serialize (c, plan.subset_globalsubrs)))
      c->निकालो_pack (false);
    अन्यथा
    {
      c->निकालो_discard ();
      return_trace (false);
    }
  }

  // 7. String INDEX - Add "Adobe" and "Identity" for ROS operator
  {
    स्थिर अक्षर *adobe_str = "Adobe";
    स्थिर अक्षर *identity_str = "Identity";
    अचिन्हित adobe_len = 5;  // strlen("Adobe")
    अचिन्हित identity_len = 8;  // strlen("Identity")

    // Build strings array
    hb_vector_t<hb_ubytes_t> strings;
    strings.alloc (2);
    strings.push (hb_ubytes_t ((स्थिर अचिन्हित अक्षर *) adobe_str, adobe_len));
    strings.push (hb_ubytes_t ((स्थिर अचिन्हित अक्षर *) identity_str, identity_len));

    // Serialize as CFF INDEX
    स्वतः *dest = c->push<CFF1_सूचकांक> ();
    अगर (likely (dest->serialize (c, strings)))
      c->निकालो_pack (false);
    अन्यथा
    {
      c->निकालो_discard ();
      return_trace (false);
    }
  }

  // 8. CFF Header
  OT::cff1 *cff = c->allocate_min<OT::cff1> ();
  अगर (unlikely (!cff)) return_trace (false);

  /* header */
  cff->version.major = 0x01;
  cff->version.minor = 0x00;
  cff->name_सूचकांक = cff->min_आकार;
  cff->off_आकार = 4; /* unused? */

  // 9. Name INDEX (single entry)
  {
    अचिन्हित name_len = म_माप (CFF1_DEFAULT_FONT_NAME);

    CFF1_सूचकांक *idx = c->start_embed<CFF1_सूचकांक> ();
    अगर (unlikely (!idx)) return_trace (false);

    अगर (unlikely (!idx->serialize_header (c, hb_iter (&name_len, 1), name_len)))
      return_trace (false);

    अगर (unlikely (!c->embed (CFF1_DEFAULT_FONT_NAME, name_len)))
      return_trace (false);
  }

  // 10. Top DICT INDEX
  {
    // Serialize the Top DICT data first
    c->push<शीर्ष_Dict> ();
    cff1_from_cff2_शीर्ष_dict_op_serializer_t शीर्ष_Szr;

    // Serialize ROS first
    अगर (unlikely (!शीर्ष_Szr.serialize_ros (c)))
    {
      c->निकालो_discard ();
      return_trace (false);
    }

    // Serialize FontBBox from head table
    {
      str_buff_t bbox_buff;
      str_encoder_t encoder (bbox_buff);

      encoder.encode_int (plan.शीर्ष_maxp_info.xMin);
      encoder.encode_int (plan.शीर्ष_maxp_info.yMin);
      encoder.encode_int (plan.शीर्ष_maxp_info.xMax);
      encoder.encode_int (plan.शीर्ष_maxp_info.yMax);
      encoder.encode_op (OpCode_FontBBox);

      अगर (encoder.in_error () || !c->embed (bbox_buff.as_bytes ().arrayZ, bbox_buff.लंबाई))
      {
        c->निकालो_discard ();
        return_trace (false);
      }
    }

    // Serialize charset operator
    अगर (charset_link && unlikely (!FontDict::serialize_link4_op (c, OpCode_charset, charset_link, whence_t::Absolute)))
    {
      c->निकालो_discard ();
      return_trace (false);
    }

    // Serialize FDSelect operator (required for CID-keyed CFF1 fonts)
    अगर (plan.info.fd_select.link && unlikely (!FontDict::serialize_link4_op (c, OpCode_FDSelect, plan.info.fd_select.link, whence_t::Absolute)))
    {
      c->निकालो_discard ();
      return_trace (false);
    }

    // Serialize FDArray operator (required for CID-keyed CFF1 fonts)
    अगर (plan.info.fd_array_link && unlikely (!FontDict::serialize_link4_op (c, OpCode_FDArray, plan.info.fd_array_link, whence_t::Absolute)))
    {
      c->निकालो_discard ();
      return_trace (false);
    }

    // Serialize other operators from CFF2 TopDict
    क्रम (स्थिर स्वतः &opstr : cff2_शीर्ष_Dict.values)
    {
      अगर (unlikely (!शीर्ष_Szr.serialize (c, opstr, plan.info)))
      {
        c->निकालो_discard ();
        return_trace (false);
      }
    }

    अचिन्हित शीर्ष_आकार = c->लंबाई ();
    c->निकालो_pack (false);

    // Serialize INDEX header
    स्वतः *dest = c->start_embed<CFF1_सूचकांक> ();
    अगर (unlikely (!dest->serialize_header (c, hb_iter (&शीर्ष_आकार, 1), शीर्ष_आकार)))
      return_trace (false);
  }

  return_trace (true);
}

} /* namespace CFF */

अटल bool _serialize_cff2_charstrings (hb_serialize_context_t *c,
			     cff2_subset_plan &plan,
			     स्थिर OT::cff2::accelerator_subset_t  &acc)
{
  c->push ();

  अचिन्हित तथ्य_आकार = 0;
  अचिन्हित योग_आकार = CFF2CharStrings::योग_आकार (plan.subset_charstrings, &तथ्य_आकार, plan.min_charstrings_off_आकार);
  अगर (unlikely (!c->start_zerocopy (योग_आकार)))
    वापस false;

  स्वतः *cs = c->start_embed<CFF2CharStrings> ();
  अगर (unlikely (!cs->serialize (c, plan.subset_charstrings, &तथ्य_आकार, plan.min_charstrings_off_आकार)))
  {
    c->निकालो_discard ();
    वापस false;
  }

  plan.info.char_strings_link = c->निकालो_pack (false);
  वापस true;
}

bool
OT::cff2::accelerator_subset_t::serialize (hb_serialize_context_t *c,
					   काष्ठा cff2_subset_plan &plan,
					   hb_array_t<पूर्णांक> normalized_coords) स्थिर
{
  /* push charstrings onto the object stack first which will ensure it packs as the last
     object in the table. Keeping the chastrings last satisfies the requirements for patching
     via IFTB. If this ordering needs to be changed in the future, charstrings should be left
     at the end whenever HB_SUBSET_FLAGS_ITFB_REQUIREMENTS is enabled. */
  अगर (!_serialize_cff2_charstrings(c, plan, *यह))
    वापस false;

  /* private dicts & local subrs */
  hb_vector_t<table_info_t>  private_dict_infos;
  अगर (unlikely (!private_dict_infos.resize (plan.subset_fdcount))) वापस false;

  क्रम (पूर्णांक i = (पूर्णांक)privateDicts.लंबाई; --i >= 0 ;)
  {
    अगर (plan.fdmap.has (i))
    {
      objidx_t	subrs_link = 0;

      अगर (plan.subset_localsubrs[i].लंबाई > 0)
      {
	स्वतः *dest = c->push <CFF2Subrs> ();
	अगर (likely (dest->serialize (c, plan.subset_localsubrs[i])))
	  subrs_link = c->निकालो_pack (false);
	अन्यथा
	{
	  c->निकालो_discard ();
	  वापस false;
	}
      }
      स्वतः *pd = c->push<PrivateDict> ();
      cff2_private_dict_op_serializer_t privSzr (plan.desubroutinize, plan.drop_hints, plan.pinned,
						 varStore, normalized_coords,
						 plan.partial_instancing ? &plan.instancing : nullptr);
      अगर (likely (pd->serialize (c, privateDicts[i], privSzr, subrs_link)))
      {
	अचिन्हित fd = plan.fdmap[i];
	private_dict_infos[fd].आकार = c->लंबाई ();
	private_dict_infos[fd].link = c->निकालो_pack ();
      }
      अन्यथा
      {
	c->निकालो_discard ();
	वापस false;
      }
    }
  }

  /* FDSelect */
  अगर (fdSelect != &Null (CFF2FDSelect))
  {
    c->push ();
    अगर (likely (hb_serialize_cff_fdselect (c, plan.अंक_glyphs, *(स्थिर FDSelect *)fdSelect,
					   plan.orig_fdcount,
					   plan.subset_fdselect_format, plan.subset_fdselect_आकार,
					   plan.subset_fdselect_ranges)))
      plan.info.fd_select.link = c->निकालो_pack ();
    अन्यथा
    {
      c->निकालो_discard ();
      वापस false;
    }
  }

  /* FDArray (FD Index) */
  {
    स्वतः *fda = c->push<CFF2FDArray> ();
    cff_font_dict_op_serializer_t fontSzr;
    स्वतः it =
    + hb_zip (+ hb_iter (fontDicts)
	      | hb_filter ([&] (स्थिर cff2_font_dict_मूल्यावली_t &_)
		{ वापस plan.fdmap.has (&_ - &fontDicts[0]); }),
	      hb_iter (private_dict_infos))
    ;
    अगर (unlikely (!fda->serialize (c, it, fontSzr)))
    {
      c->निकालो_discard ();
      वापस false;
    }
    plan.info.fd_array_link = c->निकालो_pack (false);
  }

  /* variation store */
  अगर (varStore != &Null (CFF2ItemVariationStore) &&
      !plan.pinned)
  {
    अगर (plan.partial_instancing)
    {
      अगर (plan.instancing.has_variations ())
      {
	c->push ();
	अगर (unlikely (!plan.instancing.serialize_var_store (c, *plan.axis_tags)))
	{
	  c->निकालो_discard ();
	  वापस false;
	}
	plan.info.var_store_link = c->निकालो_pack (false);
      }
      /* else: the store emptied; the vstore op gets dropped along with it. */
    }
    अन्यथा
    {
      स्वतः *dest = c->push<CFF2ItemVariationStore> ();
      अगर (unlikely (!dest->serialize (c, varStore)))
      {
	c->निकालो_discard ();
	वापस false;
      }
      plan.info.var_store_link = c->निकालो_pack (false);
    }
  }

  OT::cff2 *cff2 = c->allocate_min<OT::cff2> ();
  अगर (unlikely (!cff2)) वापस false;

  /* header */
  cff2->version.major = 0x02;
  cff2->version.minor = 0x00;
  cff2->शीर्ष_Dict_2 = OT::cff2::static_आकार;

  /* top dict */
  {
    शीर्ष_Dict &dict = cff2 + cff2->शीर्ष_Dict_2;
    cff2_शीर्ष_dict_op_serializer_t शीर्ष_Szr;
    अगर (unlikely (!dict.serialize (c, शीर्ष_Dict_2, शीर्ष_Szr, plan.info))) वापस false;
    cff2->शीर्ष_Dict_आकार = c->शीर्ष_2 - (स्थिर अक्षर *)&dict;
  }

  /* global subrs */
  {
    स्वतः *dest = c->start_embed <CFF2Subrs> ();
    वापस dest->serialize (c, plan.subset_globalsubrs);
  }
}

bool
OT::cff2::accelerator_subset_t::subset (hb_subset_context_t *c) स्थिर
{
  cff2_subset_plan cff2_plan;

  अगर (unlikely (!cff2_plan.create (*यह, c->plan))) वापस false;

  // If instantiating (pinned) and downgrade flag is set, convert to CFF1
  अगर (cff2_plan.pinned && (c->plan->flags & HB_SUBSET_FLAGS_DOWNGRADE_CFF2))
  {
    // Serialize CFF1 to the subsetter's serializer
    // If we run out of room, returning true will cause subsetter to retry with larger buffer
    bool परिणाम = CFF::serialize_cff2_to_cff1 (c->serializer, cff2_plan, शीर्ष_Dict_2, *यह);

    अगर (c->serializer->ran_out_of_room ())
      वापस true; // Subsetter will retry with larger buffer

    अगर (परिणाम && !c->serializer->in_error ())
    {
      // Success - end serialization to resolve links
      c->serializer->end_serialize ();

      // Copy the serialized CFF1 data and add as CFF table
      hb_blob_t *cff_blob = c->serializer->copy_blob ();
      अगर (cff_blob)
      {
        c->plan->add_table (HB_TAG('C','F','F',' '), cff_blob);
        hb_blob_destroy (cff_blob);

        // Return false to signal CFF2 table is not needed
        वापस false;
      }
    }

    // Conversion failed - don't fall back, fail hard for debugging
    वापस false;
  }

  वापस serialize (c->serializer, cff2_plan,
		    c->plan->normalized_coords.as_array ());
}

#पूर्ण_अगर
