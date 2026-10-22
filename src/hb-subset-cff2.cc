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

#अन्तर्भाव "hb.hh"

#ifndef HB_NO_SUBSET_CFF

#अन्तर्भाव "hb-open-type.hh"
#अन्तर्भाव "hb-ot-cff2-table.hh"
#अन्तर्भाव "hb-ot-var-common.hh"
#अन्तर्भाव "hb-set.h"
#अन्तर्भाव "hb-subset-plan.hh"
#अन्तर्भाव "hb-subset-cff-common.hh"
#अन्तर्भाव "hb-cff2-interp-cs.hh"
#अन्तर्भाव "hb-subset-cff2-to-cff1.hh"

using नामाकाश CFF;

नामाकाश CFF {

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
संरचना cff2_instancing_plan_t
{
  संरचना vardata_transform_t
  {
    /* new_deltas[j] = Σ_i matrix[j][i] * old_deltas[i] */
    hb_vector_t<hb_vector_t<दशांश>> matrix;
    /* Default-value fold-in scalars, one per old region: the old store's
     * region scalars at the new default location. */
    hb_vector_t<दशांश> gains;
    /* Surviving regions, parallel to matrix rows. */
    hb_vector_t<hb_hashmap_t<hb_tag_t, Triple>> regions;
    /* Index into the new global region list, per new region (finalize). */
    hb_vector_t<अचिन्हित> region_indices;
    अचिन्हित new_major = 0;
    तार्किक alive_ = असत्य;
    तार्किक alive () स्थिरांक { परत alive_; }

    /* One value's delta for new region j, from its old per-region deltas. */
    द्विगुण transform_delta (अचिन्हित j, hb_array_t<स्थिरांक संख्या_t> deltas) स्थिरांक
    {
      स्थिरांक hb_vector_t<दशांश> &col = matrix.arrayZ[j];
      द्विगुण d = 0.;
      अचिन्हित मोजणी = hb_min (col.length, deltas.length);
      साठी (अचिन्हित c = 0; c < मोजणी; c++)
	d += (द्विगुण) col.arrayZ[c] * deltas.arrayZ[c].to_real ();
      परत d;
    }
  };

  hb_vector_t<vardata_transform_t> transforms;  /* per old VarData (vsindex) */
  hb_vector_t<hb_hashmap_t<hb_tag_t, Triple>> new_regions;  /* global region list */
  hb_set_t used_majors;
  अचिन्हित new_major_मोजणी = 0;

  तार्किक has_variations () स्थिरांक { परत new_major_मोजणी; }

  स्थिरांक vardata_transform_t *get_transform (अचिन्हित major) स्थिरांक
  { परत major < transforms.length ? &transforms.arrayZ[major] : nullptr; }

  /* Record that a retained blend references this VarData. */
  शून्य note_use (अचिन्हित major) { used_majors.add (major); }

  /* The new vsindex a private dict's (or charstring's) old ivs maps to;
   * dead VarDatas fall back to 0, their blends having become static. */
  अचिन्हित remap_ivs (अचिन्हित old_ivs) स्थिरांक
  {
    स्थिरांक vardata_transform_t *t = get_transform (old_ivs);
    परत (t && t->alive ()) ? t->new_major : 0;
  }

  द्विगुण fold_deltas (अचिन्हित old_ivs, hb_array_t<स्थिरांक संख्या_t> deltas) स्थिरांक
  {
    स्थिरांक vardata_transform_t *t = get_transform (old_ivs);
    जर (unlikely (!t || t->gains.length != deltas.length)) परत 0.;
    द्विगुण v = 0.;
    साठी (अचिन्हित i = 0; i < deltas.length; i++)
      v += (द्विगुण) t->gains.arrayZ[i] * deltas.arrayZ[i].to_real ();
    परत v;
  }

  तार्किक create (स्थिरांक CFF2ItemVariationStore *cff2VarStore,
	       स्थिरांक hb_subset_plan_t *plan)
  {
    स्थिरांक OT::ItemVariationStore &varStore = cff2VarStore->varStore;
    अचिन्हित major_मोजणी = varStore.get_sub_table_मोजणी ();
    जर (!transforms.resize (major_मोजणी)) परत असत्य;

    जर (!varStore.get_region_list ().get_var_regions (plan->axes_old_index_tag_map, orig_regions))
      परत असत्य;

    /* Instance each VarData with identity delta rows; the surviving tuples'
     * delta columns are the old-region → new-region transform. */
    OT::optimize_scratch_t scratch;
    साठी (अचिन्हित m = 0; m < major_मोजणी; m++)
    {
      स्थिरांक OT::VarData &var_data = varStore.get_sub_table (m);
      अचिन्हित k = var_data.get_region_index_मोजणी ();
      vardata_transform_t &t = transforms.arrayZ[m];

      जर (!t.gains.resize_exact (k)) परत असत्य;
      varStore.get_region_scalars (m, plan->normalized_coords.arrayZ,
				   plan->normalized_coords.length,
				   t.gains.arrayZ, k);

      OT::tuple_variations_t tuples;
      साठी (अचिन्हित r = 0; r < k; r++)
      {
	OT::tuple_delta_t tuple;
	जर (!tuple.deltas_x.resize (k) ||
	    !tuple.indices.resize (k))
	  परत असत्य;
	साठी (अचिन्हित i = 0; i < k; i++)
	  tuple.indices.arrayZ[i] = सत्य;
	tuple.deltas_x.arrayZ[r] = 1.f;
	अचिन्हित region_index = var_data.get_region_index (r);
	जर (unlikely (region_index >= orig_regions.length)) परत असत्य;
	tuple.axis_tuples = orig_regions.arrayZ[region_index];
	जर (unlikely (tuple.axis_tuples.in_error ())) परत असत्य;
	tuples.tuple_vars.push (std::move (tuple));
      }
      जर (unlikely (tuples.tuple_vars.in_error ())) परत असत्य;

      जर (!tuples.instantiate (plan->axes_location, plan->axes_triple_distances, scratch))
	परत असत्य;

      साठी (स्वतः &tuple : tuples.tuple_vars)
      {
	जर (unlikely (tuple.deltas_x.length != k)) परत असत्य;
	hb_vector_t<दशांश> col;
	जर (unlikely (!col.resize_exact (k))) परत असत्य;
	साठी (अचिन्हित i = 0; i < k; i++)
	  col.arrayZ[i] = tuple.deltas_x.arrayZ[i];
	t.matrix.push (std::move (col));
	t.regions.push (std::move (tuple.axis_tuples));
      }
      जर (unlikely (t.matrix.in_error () || t.regions.in_error ())) परत असत्य;
    }

    परत सत्य;
  }

  /* Decide which VarDatas survive, number them, and build the new global
   * region list: surviving original regions first, in their original order,
   * then solver-split new regions in order of appearance (fontTools'
   * rebuildRegions + prune_regions). */
  तार्किक finalize ()
  {
    साठी (अचिन्हित m = 0; m < transforms.length; m++)
    {
      vardata_transform_t &t = transforms.arrayZ[m];
      t.alive_ = t.matrix.length && used_majors.has (m);
      जर (t.alive_)
	t.new_major = new_major_मोजणी++;
    }

    /* Keys hash by content, so pointers into different owners compare fine. */
    hb_hashmap_t<स्थिरांक hb_hashmap_t<hb_tag_t, Triple>*, अचिन्हित> region_idx_map;
    hb_vector_t<स्थिरांक hb_hashmap_t<hb_tag_t, Triple>*> region_ptrs;
    साठी (स्थिरांक vardata_transform_t &t : transforms)
    {
      जर (!t.alive ()) सुरूठेवा;
      साठी (स्थिरांक स्वतः &region : t.regions)
	जर (!region_idx_map.has (&region))
	  जर (unlikely (!region_idx_map.set (&region, (अचिन्हित) -1)))
	    परत असत्य;
    }
    साठी (स्थिरांक स्वतः &r : orig_regions)
    {
      अचिन्हित *idx;
      जर (region_idx_map.has (&r, &idx) && *idx == (अचिन्हित) -1)
      {
	जर (unlikely (!region_idx_map.set (&r, region_ptrs.length))) परत असत्य;
	region_ptrs.push (&r);
      }
    }
    साठी (स्थिरांक vardata_transform_t &t : transforms)
    {
      जर (!t.alive ()) सुरूठेवा;
      साठी (स्थिरांक स्वतः &region : t.regions)
      {
	अचिन्हित *idx;
	जर (region_idx_map.has (&region, &idx) && *idx == (अचिन्हित) -1)
	{
	  जर (unlikely (!region_idx_map.set (&region, region_ptrs.length))) परत असत्य;
	  region_ptrs.push (&region);
	}
      }
    }
    जर (unlikely (region_ptrs.in_error ())) परत असत्य;

    साठी (vardata_transform_t &t : transforms)
    {
      जर (!t.alive ()) सुरूठेवा;
      साठी (स्थिरांक स्वतः &region : t.regions)
      {
	अचिन्हित *idx;
	जर (unlikely (!region_idx_map.has (&region, &idx))) परत असत्य;
	t.region_indices.push (*idx);
      }
      जर (unlikely (t.region_indices.in_error ())) परत असत्य;
    }

    जर (!new_regions.resize (region_ptrs.length)) परत असत्य;
    साठी (अचिन्हित i = 0; i < region_ptrs.length; i++)
    {
      new_regions.arrayZ[i] = *region_ptrs.arrayZ[i];
      जर (unlikely (new_regions.arrayZ[i].in_error ())) परत असत्य;
    }

    परत सत्य;
  }

  /* Serialize the instanced store: a CFF2ItemVariationStore whose VarDatas
   * carry no items, just region indexes (blend deltas live in charstrings).
   * Laid out manually since all sizes are known up front. */
  तार्किक serialize_var_store (hb_serialize_context_t *c,
			    स्थिरांक hb_vector_t<hb_tag_t> &axis_tags) स्थिरांक
  {
    अचिन्हित data_मोजणी = new_major_मोजणी;
    अचिन्हित header_size = 2 + 4 + 2 + 4 * data_मोजणी;

    uint64_t offset = header_size;
    hb_vector_t<अचिन्हित> data_offsets;
    साठी (स्थिरांक vardata_transform_t &t : transforms)
    {
      जर (!t.alive ()) सुरूठेवा;
      data_offsets.push ((अचिन्हित) offset);
      offset += 6 + 2 * (uint64_t) t.region_indices.length;
    }
    जर (unlikely (data_offsets.in_error () || data_offsets.length != data_मोजणी))
      परत असत्य;
    अचिन्हित regions_offset = (अचिन्हित) offset;
    uint64_t total = offset + 4 + (uint64_t) new_regions.length * axis_tags.length * 6;
    जर (unlikely (total > 0xFFFFFFFFu)) परत असत्य;

    HBUINT16 *size_field = c->allocate_size<HBUINT16> (HBUINT16::static_size);
    जर (unlikely (!size_field)) परत असत्य;
    /* OFF 2026 5.3.3.11.2 permits larger stores with length=65535. */
    *size_field = hb_min (total, (uint64_t) 0xFFFFu);

    अक्षर *header = c->allocate_size<अक्षर> (header_size);
    जर (unlikely (!header)) परत असत्य;
    * (HBUINT16 *) (header + 0) = 1;			/* format */
    * (HBUINT32 *) (header + 2) = regions_offset;
    * (HBUINT16 *) (header + 6) = data_मोजणी;
    साठी (अचिन्हित i = 0; i < data_मोजणी; i++)
      * (HBUINT32 *) (header + 8 + 4 * i) = data_offsets.arrayZ[i];

    साठी (स्थिरांक vardata_transform_t &t : transforms)
    {
      जर (!t.alive ()) सुरूठेवा;
      अचिन्हित k = t.region_indices.length;
      HBUINT16 *v = (HBUINT16 *) c->allocate_size<अक्षर> (6 + 2 * k);
      जर (unlikely (!v)) परत असत्य;
      v[0] = 0;						/* itemCount */
      v[1] = 0;						/* wordSizeCount */
      v[2] = k;						/* regionIndices.len */
      साठी (अचिन्हित j = 0; j < k; j++)
	v[3 + j] = t.region_indices.arrayZ[j];
    }

    hb_vector_t<स्थिरांक hb_hashmap_t<hb_tag_t, Triple>*> region_ptrs;
    जर (unlikely (!region_ptrs.alloc_exact (new_regions.length))) परत असत्य;
    साठी (स्थिरांक स्वतः &r : new_regions)
      region_ptrs.push (&r);
    स्वतः *region_list = c->start_embed<OT::VarRegionList> ();
    परत region_list->serialize (c, axis_tags, region_ptrs);
  }

  खाजगी:
  hb_vector_t<hb_hashmap_t<hb_tag_t, Triple>> orig_regions;
};

/* Usage-collection pass for partial instancing: record which VarDatas the
 * retained charstrings' and private dicts' blends reference.  Mirrors the
 * emission pass: blends consumed by dropped hint ops don't count. */
संरचना cff2_usage_param_t
{
  शून्य init () {}

  cff2_instancing_plan_t *instancing = nullptr;
  तार्किक drop_hints = असत्य;
  अचिन्हित ivs = 0;	/* dict-side current vsindex */
};

संरचना cff2_cs_opset_usage_t : cff2_cs_opset_t<cff2_cs_opset_usage_t, cff2_usage_param_t, blend_arg_t>
{
  स्थिर शून्य flush_args_and_op (op_code_t op, cff2_cs_interp_env_t<blend_arg_t> &env, cff2_usage_param_t& param)
  {
    तार्किक counts = सत्य;
    निवड (op)
    {
      स्थिती OpCode_return:
      स्थिती OpCode_endchar:
	counts = असत्य;
	थांबा;

      स्थिती OpCode_hstem:
      स्थिती OpCode_hstemhm:
      स्थिती OpCode_vstem:
      स्थिती OpCode_vstemhm:
      स्थिती OpCode_hintmask:
      स्थिती OpCode_cntrmask:
	जर (param.drop_hints)
	  counts = असत्य;
	थांबा;

      पूर्वनिर्धारित:
	थांबा;
    }

    जर (counts)
      साठी (अचिन्हित पूर्णांक i = 0; i < env.arg_चळत.get_मोजणी (); i++)
	जर (env.arg_चळत[i].blending ())
	{
	  param.instancing->note_use (env.get_ivs ());
	  थांबा;
	}

    env.clear_args ();
  }
};

संरचना cff2_private_dict_usage_opset_t : dict_opset_t
{
  स्थिर शून्य process_op (op_code_t op, cff2_priv_dict_interp_env_t& env, cff2_usage_param_t& param)
  {
    निवड (op) {
      स्थिती OpCode_vsindexdict:
	env.process_vsindex ();
	param.ivs = env.get_ivs ();
	env.clear_args ();
	परत;
      स्थिती OpCode_blenddict:
	param.instancing->note_use (param.ivs);
	env.clear_args ();
	परत;
      पूर्वनिर्धारित:
	dict_opset_t::process_op (op, env);
	जर (!env.arg_चळत.is_empty ()) परत;
	env.clear_args ();
	परत;
    }
  }
};

} /* namespace CFF */

संरचना cff2_sub_table_info_t : cff_sub_table_info_t
{
  cff2_sub_table_info_t ()
    : cff_sub_table_info_t (),
      var_store_link (0)
  {}

  objidx_t  var_store_link;
};

संरचना cff2_शीर्ष_dict_op_serializer_t : cff_शीर्ष_dict_op_serializer_t<>
{
  तार्किक serialize (hb_serialize_context_t *c,
		  स्थिरांक op_str_t &opstr,
		  स्थिरांक cff2_sub_table_info_t &info) स्थिरांक
  {
    TRACE_SERIALIZE (हे);

    निवड (opstr.op)
    {
      स्थिती OpCode_vstore:
        जर (info.var_store_link)
	  return_trace (FontDict::serialize_link4_op(c, opstr.op, info.var_store_link));
	अन्यथा
	  return_trace (सत्य);

      पूर्वनिर्धारित:
	return_trace (cff_शीर्ष_dict_op_serializer_t<>::serialize (c, opstr, info));
    }
  }
};

संरचना cff2_cs_opset_flatten_t : cff2_cs_opset_t<cff2_cs_opset_flatten_t, flatten_param_t, blend_arg_t>
{
  स्थिर शून्य flush_args_and_op (op_code_t op, cff2_cs_interp_env_t<blend_arg_t> &env, flatten_param_t& param)
  {
    /* Optionally capture command for specialization (before flushing, to preserve args) */
    जर (param.commands)
    {
      तार्किक skip_command = असत्य;

      निवड (op)
      {
	स्थिती OpCode_return:
	स्थिती OpCode_endchar:
	  skip_command = सत्य;
	  थांबा;

	स्थिती OpCode_hstem:
	स्थिती OpCode_hstemhm:
	स्थिती OpCode_vstem:
	स्थिती OpCode_vstemhm:
	स्थिती OpCode_hintmask:
	स्थिती OpCode_cntrmask:
	  जर (param.drop_hints)
	    skip_command = सत्य;
	  थांबा;

	पूर्वनिर्धारित:
	  थांबा;
      }

      जर (!skip_command)
      {
	cs_command_t cmd (op);
	/* Capture resolved blend values */
	साठी (अचिन्हित पूर्णांक i = 0; i < env.arg_चळत.get_मोजणी ();)
	{
	  स्थिरांक blend_arg_t &arg = env.arg_चळत[i];
	  जर (arg.blending ())
	  {
	    /* For blend args, capture only the resolved default value */
	    cmd.args.push (arg);
	    /* Skip over the multiple blend values */
	    i += arg.अंक_मूल्ये_2;
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

    निवड (op)
    {
      स्थिती OpCode_return:
      स्थिती OpCode_endchar:
	/* dummy opcodes in CFF2. ignore */
	जर (op == OpCode_endchar && param.instancer)
	  emit_vsindex_prefix (env, param);
	थांबा;

      स्थिती OpCode_hstem:
      स्थिती OpCode_hstemhm:
      स्थिती OpCode_vstem:
      स्थिती OpCode_vstemhm:
      स्थिती OpCode_hintmask:
      स्थिती OpCode_cntrmask:
	जर (param.drop_hints)
	{
	  env.clear_args ();
	  परत;
	}
	HB_FALLTHROUGH;

      पूर्वनिर्धारित:
	SUPER::flush_args_and_op (op, env, param);
	थांबा;
    }
  }

  स्थिर शून्य flush_args (cff2_cs_interp_env_t<blend_arg_t> &env, flatten_param_t& param)
  {
    साठी (अचिन्हित पूर्णांक i = 0; i < env.arg_चळत.get_मोजणी ();)
    {
      स्थिरांक blend_arg_t &arg = env.arg_चळत[i];
      जर (arg.blending ())
      {
	जर (unlikely (!((arg.अंक_मूल्ये_2 > 0) && (env.arg_चळत.get_मोजणी () >= arg.अंक_मूल्ये_2))))
	{
	  env.set_error ();
	  परत;
	}
	जर (param.instancer)
	  rewrite_blends (arg, i, env, param);
	अन्यथा
	  flatten_blends (arg, i, env, param);
	i += arg.अंक_मूल्ये_2;
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

  स्थिर शून्य flatten_blends (स्थिरांक blend_arg_t &arg, अचिन्हित पूर्णांक i, cff2_cs_interp_env_t<blend_arg_t> &env, flatten_param_t& param)
  {
    /* flatten the default values */
    str_encoder_t  encoder (param.flatStr);
    साठी (अचिन्हित पूर्णांक j = 0; j < arg.अंक_मूल्ये_2; j++)
    {
      स्थिरांक blend_arg_t &arg1 = env.arg_चळत[i + j];
      जर (unlikely (!((arg1.blending () && (arg.अंक_मूल्ये_2 == arg1.अंक_मूल्ये_2) && (arg1.मूल्य_Index_2 == j) &&
	      (arg1.deltas.length == env.get_region_मोजणी ())))))
      {
	env.set_error ();
	परत;
      }
      encoder.encode_अंक_cs (arg1);
    }
    /* flatten deltas for each value */
    साठी (अचिन्हित पूर्णांक j = 0; j < arg.अंक_मूल्ये_2; j++)
    {
      स्थिरांक blend_arg_t &arg1 = env.arg_चळत[i + j];
      साठी (अचिन्हित पूर्णांक k = 0; k < arg1.deltas.length; k++)
	encoder.encode_अंक_cs (arg1.deltas[k]);
    }
    /* flatten the number of values followed by blend operator */
    encoder.encode_int (arg.अंक_मूल्ये_2);
    encoder.encode_op (OpCode_blendcs);
  }

  /* Partial instancing: rewrite one blend group against the instanced store.
   * Each value's default absorbs the folded (pinned-away) contribution, and
   * its deltas are mapped onto the new region list. */
  स्थिर शून्य rewrite_blends (स्थिरांक blend_arg_t &arg, अचिन्हित पूर्णांक i, cff2_cs_interp_env_t<blend_arg_t> &env, flatten_param_t& param)
  {
    स्थिरांक cff2_instancing_plan_t &instancer = *param.instancer;
    अचिन्हित ivs = env.get_ivs ();
    स्थिरांक cff2_instancing_plan_t::vardata_transform_t *t = instancer.get_transform (ivs);
    जर (unlikely (!t))
    {
      env.set_error ();
      परत;
    }

    अचिन्हित n = arg.अंक_मूल्ये_2;
    साठी (अचिन्हित पूर्णांक j = 0; j < n; j++)
    {
      स्थिरांक blend_arg_t &arg1 = env.arg_चळत[i + j];
      जर (unlikely (!(arg1.blending () && (arg1.अंक_मूल्ये_2 == n) && (arg1.मूल्य_Index_2 == j) &&
		      (arg1.deltas.length == env.get_region_मोजणी ()) &&
		      (arg1.deltas.length == t->gains.length))))
      {
	env.set_error ();
	परत;
      }
    }

    str_encoder_t encoder (param.flatStr);
    अचिन्हित k_new = t->alive () ? t->matrix.length : 0;

    जर (!k_new)
    {
      /* All of this VarData's regions died; blends dissolve into statics. */
      साठी (अचिन्हित पूर्णांक j = 0; j < n; j++)
      {
	स्थिरांक blend_arg_t &arg1 = env.arg_चळत[i + j];
	संख्या_t v;
	v.set_real (arg1.to_real () + round (instancer.fold_deltas (ivs, arg1.deltas.as_array ())));
	encoder.encode_अंक_cs (v);
      }
      परत;
    }

    /* Emit in chunks so the output charstring stays within the argument
     * stack limit: values already emitted for this op, plus this chunk's
     * defaults, deltas and count, must not exceed 513. */
    अचिन्हित done = 0;
    पर्यंत (done < n)
    {
      अचिन्हित avail = 512 > (i + done) ? 512 - (i + done) : 0;
      अचिन्हित chunk = hb_min (avail / (k_new + 1), n - done);
      जर (unlikely (!chunk))
      {
	env.set_error ();
	परत;
      }

      साठी (अचिन्हित पूर्णांक j = done; j < done + chunk; j++)
      {
	स्थिरांक blend_arg_t &arg1 = env.arg_चळत[i + j];
	संख्या_t v;
	v.set_real (arg1.to_real () + round (instancer.fold_deltas (ivs, arg1.deltas.as_array ())));
	encoder.encode_अंक_cs (v);
      }
      साठी (अचिन्हित पूर्णांक j = done; j < done + chunk; j++)
      {
	स्थिरांक blend_arg_t &arg1 = env.arg_चळत[i + j];
	साठी (अचिन्हित r = 0; r < k_new; r++)
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
    param.emitted_blend = सत्य;
  }

  /* Partial instancing: prepend a vsindex op when the charstring's
   * (remapped) ivs differs from its FD's (remapped) private-dict ivs. */
  स्थिर शून्य emit_vsindex_prefix (cff2_cs_interp_env_t<blend_arg_t> &env, flatten_param_t& param)
  {
    जर (!param.emitted_blend) परत;
    स्थिरांक cff2_instancing_plan_t &instancer = *param.instancer;
    अचिन्हित new_ivs = instancer.remap_ivs (env.get_ivs ());
    अचिन्हित fd_ivs = instancer.remap_ivs (env.get_orig_ivs ());
    जर (new_ivs == fd_ivs) परत;

    str_buff_t prefix;
    str_encoder_t enc (prefix);
    enc.encode_int ((पूर्णांक) new_ivs);
    enc.encode_op (OpCode_vsindexcs);

    str_buff_t &buf = param.flatStr;
    अचिन्हित old_len = buf.length;
    जर (unlikely (prefix.in_error () || !buf.resize (old_len + prefix.length)))
    {
      env.set_error ();
      परत;
    }
    स_हटाओ (buf.arrayZ + prefix.length, buf.arrayZ, old_len);
    hb_memcpy (buf.arrayZ, prefix.arrayZ, prefix.length);
  }

  स्थिर शून्य flush_op (op_code_t op, cff2_cs_interp_env_t<blend_arg_t> &env, flatten_param_t& param)
  {
    निवड (op)
    {
      स्थिती OpCode_return:
      स्थिती OpCode_endchar:
	परत;
      पूर्वनिर्धारित:
	str_encoder_t  encoder (param.flatStr);
	encoder.encode_op (op);
    }
  }

  स्थिर शून्य flush_hintmask (op_code_t op, cff2_cs_interp_env_t<blend_arg_t> &env, flatten_param_t& param)
  {
    SUPER::flush_hintmask (op, env, param);
    /* Preserve hintmask payload in captured commands for specializer re-encoding. */
    जर (param.commands && !param.drop_hints && param.commands->length > 0)
    {
      स्वतः &cmd = param.commands->tail ();
      जर (cmd.op == op)
      {
        cmd.mask_bytes.resize (env.hintmask_size);
        जर (unlikely (cmd.mask_bytes.in_error ()))
        {
          env.set_error ();
          परत;
        }
        साठी (अचिन्हित पूर्णांक i = 0; i < env.hintmask_size; i++)
          cmd.mask_bytes[i] = env.str_ref[i];
      }
    }
    जर (!param.drop_hints)
    {
      str_encoder_t  encoder (param.flatStr);
      साठी (अचिन्हित पूर्णांक i = 0; i < env.hintmask_size; i++)
	encoder.encode_byte (env.str_ref[i]);
    }
  }

  खाजगी:
  प्रकारव्याख्या cff2_cs_opset_t<cff2_cs_opset_flatten_t, flatten_param_t, blend_arg_t> SUPER;
  प्रकारव्याख्या cs_opset_t<blend_arg_t, cff2_cs_opset_flatten_t, cff2_cs_opset_flatten_t, cff2_cs_interp_env_t<blend_arg_t>, flatten_param_t> CSOPSET;
};

संरचना cff2_cs_opset_subr_subset_t : cff2_cs_opset_t<cff2_cs_opset_subr_subset_t, subr_subset_param_t, blend_arg_t>
{
  स्थिर शून्य process_op (op_code_t op, cff2_cs_interp_env_t<blend_arg_t> &env, subr_subset_param_t& param)
  {
    निवड (op) {

      स्थिती OpCode_return:
	param.current_parsed_str->flush_coalesced (env.str_ref);
	param.current_parsed_str->set_parsed ();
	env.return_from_subr ();
	param.set_current_str (env, असत्य);
	थांबा;

      स्थिती OpCode_endchar:
	param.current_parsed_str->flush_coalesced (env.str_ref);
	param.current_parsed_str->set_parsed ();
	SUPER::process_op (op, env, param);
	थांबा;

      स्थिती OpCode_callsubr:
	process_call_subr (op, CSType_LocalSubr, env, param, env.localSubrs, param.local_closure);
	थांबा;

      स्थिती OpCode_callgsubr:
	process_call_subr (op, CSType_GlobalSubr, env, param, env.globalSubrs, param.global_closure);
	थांबा;

      पूर्वनिर्धारित:
	SUPER::process_op (op, env, param);
	param.current_parsed_str->add_op (op, env.str_ref);
	थांबा;
    }
  }

  संरक्षित:
  स्थिर शून्य process_call_subr (op_code_t op, cs_type_t type,
				 cff2_cs_interp_env_t<blend_arg_t> &env, subr_subset_param_t& param,
				 cff2_biased_subrs_t& subrs, hb_set_t *closure)
  {
    byte_str_ref_t    str_ref = env.str_ref;
    env.call_subr (subrs, type);
    param.current_parsed_str->add_call_op (op, str_ref, env.context.subr_अंक_2);
    closure->add (env.context.subr_अंक_2);
    param.set_current_str (env, सत्य);
  }

  खाजगी:
  प्रकारव्याख्या cff2_cs_opset_t<cff2_cs_opset_subr_subset_t, subr_subset_param_t, blend_arg_t> SUPER;
};

संरचना cff2_subr_subsetter_t : subr_subsetter_t<cff2_subr_subsetter_t, CFF2Subrs, स्थिरांक OT::cff2::accelerator_subset_t, cff2_cs_interp_env_t<blend_arg_t>, cff2_cs_opset_subr_subset_t>
{
  cff2_subr_subsetter_t (स्थिरांक OT::cff2::accelerator_subset_t &acc_, स्थिरांक hb_subset_plan_t *plan_)
    : subr_subsetter_t (acc_, plan_) {}

  स्थिर शून्य complete_parsed_str (cff2_cs_interp_env_t<blend_arg_t> &env, subr_subset_param_t& param, parsed_cs_str_t &charstring)
  {
    /* vsindex is inserted at the beginning of the charstring as necessary */
    जर (env.seen_vsindex ())
    {
      संख्या_t  ivs;
      ivs.set_int ((पूर्णांक)env.get_ivs ());
      charstring.set_prefix (ivs, OpCode_vsindexcs);
    }
  }
};

संरचना cff2_private_blend_encoder_param_t
{
  cff2_private_blend_encoder_param_t (hb_serialize_context_t *c,
				      स्थिरांक CFF2ItemVariationStore *varStore,
				      hb_array_t<पूर्णांक> normalized_coords) :
    c (c), varStore (varStore), normalized_coords (normalized_coords) {}

  शून्य init () {}

  शून्य process_blend ()
  {
    जर (!seen_blend)
    {
      region_मोजणी_2 = varStore->varStore.get_region_index_मोजणी (ivs);
      जर (unlikely (!scalars.resize_exact (region_मोजणी_2)))
      {
	जर (c) c->err (HB_SERIALIZE_ERROR_OTHER);
	seen_blend = सत्य;
	परत;
      }
      varStore->varStore.get_region_scalars (ivs, normalized_coords.arrayZ, normalized_coords.length,
					     &scalars[0], region_मोजणी_2);
      seen_blend = सत्य;
    }
  }

  द्विगुण blend_deltas (hb_array_t<स्थिरांक संख्या_t> deltas) स्थिरांक
  {
    द्विगुण v = 0;
    जर (likely (scalars.length == deltas.length))
    {
      अचिन्हित मोजणी = scalars.length;
      साठी (अचिन्हित i = 0; i < मोजणी; i++)
	v += (द्विगुण) scalars.arrayZ[i] * deltas.arrayZ[i].to_real ();
    }
    परत v;
  }


  hb_serialize_context_t *c = nullptr;
  तार्किक seen_blend = असत्य;
  अचिन्हित ivs = 0;
  अचिन्हित region_मोजणी_2 = 0;
  hb_vector_t<दशांश> scalars;
  स्थिरांक	 CFF2ItemVariationStore *varStore = nullptr;
  hb_array_t<पूर्णांक> normalized_coords;

  /* CFF2 partial instancing: when set, blends are rewritten against the
   * instanced variation store instead of being flattened. */
  स्थिरांक cff2_instancing_plan_t *instancer = nullptr;
  /* Error-compensated fold rounding state, per dict operator. */
  द्विगुण fold_exact = 0.;
  द्विगुण fold_emitted = 0.;
};

संरचना cff2_private_dict_blend_opset_t : dict_opset_t
{
  स्थिर शून्य process_arg_blend (cff2_private_blend_encoder_param_t& param,
				 संख्या_t &arg,
				 स्थिरांक hb_array_t<स्थिरांक संख्या_t> blends,
				 अचिन्हित n, अचिन्हित i)
  {
    arg.set_int (round (arg.to_real () + param.blend_deltas (blends)));
  }

  /* The fold added to a value, error-compensated across the operator: most
   * blended private-dict values (BlueValues, StemSnap*, ...) are
   * delta-encoded, so independently rounded folds would accumulate in the
   * decoded absolutes; keeping the emitted cumulative fold equal to the
   * rounded exact cumulative fold bounds every absolute within ±0.5. */
  स्थिर द्विगुण fold (cff2_private_blend_encoder_param_t& param,
		      स्थिरांक hb_array_t<स्थिरांक संख्या_t> blends)
  {
    द्विगुण f = param.blend_deltas (blends);
    द्विगुण emit = round (param.fold_exact + f) - param.fold_emitted;
    param.fold_exact += f;
    param.fold_emitted += emit;
    परत emit;
  }

  /* Partial instancing: emit the args preceding this blend group, then the
   * group itself rewritten against the instanced store, and clear the
   * stack.  The op's remaining args are flushed at process_op time. */
  स्थिर शून्य rewrite_blends (cff2_priv_dict_interp_env_t& env,
			      cff2_private_blend_encoder_param_t& param,
			      अचिन्हित start, अचिन्हित n, अचिन्हित k)
  {
    स्थिरांक cff2_instancing_plan_t &instancer = *param.instancer;
    स्थिरांक cff2_instancing_plan_t::vardata_transform_t *t = instancer.get_transform (param.ivs);
    जर (unlikely (!t || t->gains.length != k))
    {
      env.set_error ();
      परत;
    }

    str_buff_t str;
    str_encoder_t encoder (str);

    साठी (अचिन्हित idx = 0; idx < start; idx++)
      encoder.encode_अंक_tp (env.arg_चळत[idx]);

    अचिन्हित k_new = t->alive () ? t->matrix.length : 0;
    जर (!k_new)
    {
      /* This VarData died (or is unused by any retained charstring, which
       * can happen for FDs kept only for retain-gids holes); blends
       * dissolve into statics. */
      साठी (अचिन्हित j = 0; j < n; j++)
      {
	स्थिरांक hb_array_t<स्थिरांक संख्या_t> blends = env.arg_चळत.sub_array (start + n + (j * k), k);
	संख्या_t v;
	v.set_real (env.arg_चळत[start + j].to_real () + fold (param, blends));
	encoder.encode_अंक_tp (v);
      }
    }
    अन्यथा
    {
      अचिन्हित done = 0;
      पर्यंत (done < n)
      {
	अचिन्हित avail = 512 > (start + done) ? 512 - (start + done) : 0;
	अचिन्हित chunk = hb_min (avail / (k_new + 1), n - done);
	जर (unlikely (!chunk))
	{
	  env.set_error ();
	  परत;
	}

	साठी (अचिन्हित j = done; j < done + chunk; j++)
	{
	  स्थिरांक hb_array_t<स्थिरांक संख्या_t> blends = env.arg_चळत.sub_array (start + n + (j * k), k);
	  संख्या_t v;
	  v.set_real (env.arg_चळत[start + j].to_real () + fold (param, blends));
	  encoder.encode_अंक_tp (v);
	}
	साठी (अचिन्हित j = done; j < done + chunk; j++)
	{
	  स्थिरांक hb_array_t<स्थिरांक संख्या_t> blends = env.arg_चळत.sub_array (start + n + (j * k), k);
	  साठी (अचिन्हित r = 0; r < k_new; r++)
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
    param.c->embed (&bytes, bytes.length);

    env.clear_args ();
  }

  स्थिर शून्य process_blend (cff2_priv_dict_interp_env_t& env, cff2_private_blend_encoder_param_t& param)
  {
    अचिन्हित पूर्णांक n, k;

    param.process_blend ();
    k = param.region_मोजणी_2;
    n = env.arg_चळत.काढा_uint ();

    अचिन्हित मोजणी = env.arg_चळत.get_मोजणी ();
    अचिन्हित पूर्णांक total;
    जर (unlikely (hb_unsigned_mul_overflows (k + 1, n, &total) || total > मोजणी))
    {
      env.set_error ();
      परत;
    }

    /* copy the blend values into blend array of the default values */
    अचिन्हित पूर्णांक start = मोजणी - total;
    /* let an obvious error case fail, but note CFF2 spec doesn't forbid n==0 */
    जर (unlikely (start > मोजणी))
    {
      env.set_error ();
      परत;
    }

    जर (param.instancer && k)
    {
      rewrite_blends (env, param, start, n, k);
      परत;
    }

    साठी (अचिन्हित पूर्णांक i = 0; i < n; i++)
    {
      स्थिरांक hb_array_t<स्थिरांक संख्या_t> blends = env.arg_चळत.sub_array (start + n + (i * k), k);
      process_arg_blend (param, env.arg_चळत[start + i], blends, n, i);
    }

    /* pop off blend values leaving default values now adorned with blend values */
    env.arg_चळत.काढा (k * n);
  }

  स्थिर शून्य process_op (op_code_t op, cff2_priv_dict_interp_env_t& env, cff2_private_blend_encoder_param_t& param)
  {
    निवड (op) {
      स्थिती OpCode_StdHW:
      स्थिती OpCode_StdVW:
      स्थिती OpCode_BlueScale:
      स्थिती OpCode_BlueShift:
      स्थिती OpCode_BlueFuzz:
      स्थिती OpCode_ExpansionFactor:
      स्थिती OpCode_LanguageGroup:
      स्थिती Op_Code_Blue_मूल्ये:
      स्थिती OpCode_OtherBlues:
      स्थिती OpCode_FamilyBlues:
      स्थिती OpCode_FamilyOtherBlues:
      स्थिती OpCode_StemSnapH:
      स्थिती OpCode_StemSnapV:
	थांबा;
      स्थिती OpCode_vsindexdict:
	env.process_vsindex ();
	param.ivs = env.get_ivs ();
	जर (param.instancer)
	{
	  /* Re-emit with the remapped vsindex; 0 is the dict default. */
	  अचिन्हित new_ivs = param.instancer->remap_ivs (param.ivs);
	  जर (new_ivs)
	  {
	    str_buff_t str;
	    str_encoder_t encoder (str);
	    encoder.encode_int ((पूर्णांक) new_ivs);
	    encoder.encode_op (OpCode_vsindexdict);
	    स्वतः bytes = str.as_bytes ();
	    param.c->embed (&bytes, bytes.length);
	  }
	}
	env.clear_args ();
	परत;
      स्थिती OpCode_blenddict:
	process_blend (env, param);
	परत;

      पूर्वनिर्धारित:
	dict_opset_t::process_op (op, env);
	जर (!env.arg_चळत.is_empty ()) परत;
	थांबा;
    }

    जर (unlikely (env.in_error ())) परत;

    // Write args then op

    str_buff_t str;
    str_encoder_t encoder (str);

    अचिन्हित मोजणी = env.arg_चळत.get_मोजणी ();
    साठी (अचिन्हित i = 0; i < मोजणी; i++)
      encoder.encode_अंक_tp (env.arg_चळत[i]);

    encoder.encode_op (op);

    स्वतः bytes = str.as_bytes ();
    param.c->embed (&bytes, bytes.length);

    env.clear_args ();
    param.fold_exact = param.fold_emitted = 0.;
  }
};

संरचना cff2_private_dict_op_serializer_t : op_serializer_t
{
  cff2_private_dict_op_serializer_t (तार्किक desubroutinize_, तार्किक drop_hints_, तार्किक pinned_,
				     स्थिरांक CFF::CFF2ItemVariationStore* varStore_,
				     hb_array_t<पूर्णांक> normalized_coords_,
				     स्थिरांक cff2_instancing_plan_t *instancer_ = nullptr)
    : desubroutinize (desubroutinize_), drop_hints (drop_hints_), pinned (pinned_),
      param (nullptr, varStore_, normalized_coords_)
  { param.instancer = instancer_; }

  तार्किक serialize (hb_serialize_context_t *c,
		  स्थिरांक op_str_t &opstr,
		  objidx_t subrs_link) स्थिरांक
  {
    TRACE_SERIALIZE (हे);

    जर (drop_hints && dict_opset_t::is_hint_op (opstr.op))
      return_trace (सत्य);

    जर (opstr.op == OpCode_Subrs)
    {
      जर (desubroutinize || !subrs_link)
	return_trace (सत्य);
      अन्यथा
	return_trace (FontDict::serialize_link2_op (c, opstr.op, subrs_link));
    }

    जर (pinned || param.instancer)
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

  संरक्षित:
  स्थिरांक तार्किक desubroutinize;
  स्थिरांक तार्किक drop_hints;
  स्थिरांक तार्किक pinned;
  mutable cff2_private_blend_encoder_param_t param;
};



नामाकाश OT {
संरचना cff2_subset_plan
{
  तार्किक create (स्थिरांक OT::cff2::accelerator_subset_t &acc,
	      hb_subset_plan_t *plan)
  {
    /* make sure notdef is first */
    hb_codepoint_t old_glyph;
    जर (!plan->old_gid_for_new_gid (0, &old_glyph) || (old_glyph != 0)) परत असत्य;

    अंक_glyphs = plan->अंक_output_glyphs ();
    orig_fdcount = acc.fdArray->मोजणी;

    drop_hints = plan->flags & HB_SUBSET_FLAGS_NO_HINTING;
    तार्किक instancing_requested = (तार्किक) plan->normalized_coords;
    pinned = instancing_requested && plan->all_axes_pinned;
    normalized_coords = plan->normalized_coords;
    head_maxp_info = plan->head_maxp_info;
    hmtx_map = &plan->hmtx_map;
    desubroutinize = plan->flags & HB_SUBSET_FLAGS_DESUBROUTINIZE ||
		     instancing_requested; // For instancing we need this path

    /* Partial instancing: some axes pinned or limited, others kept. */
    partial_instancing = instancing_requested && !pinned &&
			 acc.varStore != &Null (CFF2ItemVariationStore);
    जर (partial_instancing &&
	unlikely (!instancing.create (acc.varStore, plan)))
      परत असत्य;
    axis_tags = &plan->axis_tags;

    /* Enable command capture for CFF2→CFF1 conversion (for specialization) */
    capture_commands = pinned;

 #ifdef HB_EXPERIMENTAL_API
    min_charstrings_off_size = (plan->flags & HB_SUBSET_FLAGS_IFTB_REQUIREMENTS) ? 4 : 0;
 #अन्यथा
    min_charstrings_off_size = 0;
 #endif

    जर (desubroutinize)
    {
      जर (partial_instancing)
      {
	/* Pass 1: collect which VarDatas the retained blends reference, so
	 * unused ones get pruned and the survivors renumbered. */
	hb_set_t used_fds;
	साठी (अचिन्हित पूर्णांक i = 0; i < अंक_glyphs; i++)
	{
	  hb_codepoint_t glyph;
	  जर (!plan->old_gid_for_new_gid (i, &glyph))
	    सुरूठेवा;
	  स्थिरांक hb_ubytes_t str = (*acc.charStrings)[glyph];
	  अचिन्हित पूर्णांक fd = CFF::subset_get_fd (acc, glyph);
	  जर (unlikely (fd >= acc.fd_मोजणी))
	    परत असत्य;
	  used_fds.add (fd);

	  cff2_cs_interp_env_t<blend_arg_t> env (str, acc, fd);
	  cs_interpreter_t<cff2_cs_interp_env_t<blend_arg_t>, cff2_cs_opset_usage_t, cff2_usage_param_t> interp (env);
	  cff2_usage_param_t param;
	  param.instancing = &instancing;
	  param.drop_hints = drop_hints;
	  जर (unlikely (!interp.interpret (param)))
	    परत असत्य;
	}
	साठी (अचिन्हित fd : used_fds)
	{
	  cff2_usage_param_t param;
	  param.instancing = &instancing;
	  अचिन्हित मोजणी = acc.privateDicts[fd].get_मोजणी ();
	  साठी (अचिन्हित j = 0; j < मोजणी; j++)
	  {
	    स्थिरांक op_str_t &opstr = acc.privateDicts[fd][j];
	    जर (drop_hints && dict_opset_t::is_hint_op (opstr.op))
	      सुरूठेवा;
	    cff2_priv_dict_interp_env_t env {hb_ubytes_t (opstr.ptr, opstr.length)};
	    dict_interpreter_t<cff2_private_dict_usage_opset_t, cff2_usage_param_t, cff2_priv_dict_interp_env_t> interp (env);
	    जर (unlikely (!interp.interpret (param)))
	      परत असत्य;
	  }
	}
	जर (unlikely (!instancing.finalize ()))
	  परत असत्य;

	/* Pass 2: like subr_flattener_t, but blends stay symbolic (no coords
	 * fed to the env) and get rewritten against the instanced store. */
	जर (!subset_charstrings.resize_exact (अंक_glyphs))
	  परत असत्य;
	साठी (अचिन्हित पूर्णांक i = 0; i < अंक_glyphs; i++)
	{
	  hb_codepoint_t glyph;
	  जर (!plan->old_gid_for_new_gid (i, &glyph))
	    सुरूठेवा;
	  स्थिरांक hb_ubytes_t str = (*acc.charStrings)[glyph];
	  अचिन्हित पूर्णांक fd = CFF::subset_get_fd (acc, glyph);
	  जर (unlikely (fd >= acc.fd_मोजणी))
	    परत असत्य;

	  cff2_cs_interp_env_t<blend_arg_t> env (str, acc, fd);
	  cs_interpreter_t<cff2_cs_interp_env_t<blend_arg_t>, cff2_cs_opset_flatten_t, flatten_param_t> interp (env);
	  flatten_param_t param = {
	    subset_charstrings.arrayZ[i],
	    (तार्किक) (plan->flags & HB_SUBSET_FLAGS_NO_HINTING),
	    plan
	  };
	  param.instancer = &instancing;
	  जर (unlikely (!interp.interpret (param)))
	    परत असत्य;
	}
      }
      अन्यथा
      {
      /* Flatten global & local subrs */
      subr_flattener_t<स्थिरांक OT::cff2::accelerator_subset_t, cff2_cs_interp_env_t<blend_arg_t>, cff2_cs_opset_flatten_t>
		    flattener(acc, plan);

      /* Enable command capture if requested (for specialization) */
      जर (capture_commands)
      {
	जर (!charstring_commands.resize_exact (अंक_glyphs))
	  परत असत्य;

	जर (!flattener.flatten (subset_charstrings, &charstring_commands))
	  परत असत्य;
      }
      अन्यथा
      {
	जर (!flattener.flatten (subset_charstrings))
	  परत असत्य;
      }
      }
    }
    अन्यथा
    {
      cff2_subr_subsetter_t	subr_subsetter (acc, plan);

      /* Subset subrs: collect used subroutines, leaving all unused ones behind */
      जर (!subr_subsetter.subset ())
	परत असत्य;

      /* encode charstrings, global subrs, local subrs with new subroutine numbers */
      जर (!subr_subsetter.encode_charstrings (subset_charstrings, !pinned))
	परत असत्य;

      जर (!subr_subsetter.encode_globalsubrs (subset_globalsubrs))
	परत असत्य;

      /* local subrs */
      जर (!subset_localsubrs.resize (orig_fdcount))
	परत असत्य;
      साठी (अचिन्हित पूर्णांक fd = 0; fd < orig_fdcount; fd++)
      {
	subset_localsubrs[fd].init ();
	जर (!subr_subsetter.encode_localsubrs (fd, subset_localsubrs[fd]))
	  परत असत्य;
      }
    }

    /* FDSelect */
    जर (acc.fdSelect != &Null (CFF2FDSelect))
    {
      जर (unlikely (!hb_plan_subset_cff_fdselect (plan,
						  orig_fdcount,
						  *(स्थिरांक FDSelect *)acc.fdSelect,
						  acc.अंक_glyphs,
						  subset_fdcount,
						  subset_fdselect_size,
						  subset_fdselect_format,
						  subset_fdselect_ranges,
						  fdmap)))
	परत असत्य;
    }
    अन्यथा
      fdmap.identity (1);

    परत सत्य;
  }

  cff2_sub_table_info_t info;

  अचिन्हित पूर्णांक    अंक_glyphs;
  अचिन्हित पूर्णांक    orig_fdcount = 0;
  अचिन्हित पूर्णांक    subset_fdcount = 1;
  अचिन्हित पूर्णांक    subset_fdselect_size = 0;
  अचिन्हित पूर्णांक    subset_fdselect_format = 0;
  तार्किक            pinned = असत्य;
  hb_vector_t<code_pair_t>   subset_fdselect_ranges;

  hb_inc_bimap_t   fdmap;

  str_buff_vec_t	    subset_charstrings;
  str_buff_vec_t	    subset_globalsubrs;
  hb_vector_t<str_buff_vec_t> subset_localsubrs;

  तार्किक	    drop_hints = असत्य;
  तार्किक	    desubroutinize = असत्य;

  तार्किक	    partial_instancing = असत्य;
  cff2_instancing_plan_t instancing;
  स्थिरांक hb_vector_t<hb_tag_t> *axis_tags = nullptr;

  अचिन्हित  min_charstrings_off_size = 0;

  hb_array_t<पूर्णांक> normalized_coords; // For instantiation
  head_maxp_info_t head_maxp_info;  // For FontBBox
  स्थिरांक hb_hashmap_t<hb_codepoint_t, hb_pair_t<अचिन्हित, पूर्णांक>> *hmtx_map; // For widths

  // Width optimization results (for CFF1 conversion)
  अचिन्हित default_width = 0;
  अचिन्हित nominal_width = 0;

  // Command capture for specialization (CFF2→CFF1 conversion)
  तार्किक capture_commands = असत्य;
  hb_vector_t<hb_vector_t<cs_command_t>> charstring_commands;
};
} // namespace OT

/*
 * CFF2 to CFF1 Converter Implementation
 */

#अन्तर्भाव "hb-cff-width-optimizer.hh"
#अन्तर्भाव "hb-cff-specializer.hh"

/* Serialize charstrings using CFF1 format with widths */
स्थिर तार्किक
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
  जर (unlikely (!cff1_charstrings.resize (plan.subset_charstrings.length)))
  {
    c->काढा_discard ();
    परत असत्य;
  }

  साठी (अचिन्हित i = 0; i < plan.subset_charstrings.length; i++)
  {
    // Get width for this glyph from hmtx_map
    अचिन्हित width = 0;
    जर (plan.hmtx_map->has (i))
      width = plan.hmtx_map->get (i).first;

    // Encode width if different from default
    str_encoder_t encoder (cff1_charstrings[i]);
    जर (width != default_width)
    {
      पूर्णांक delta = (पूर्णांक) width - (पूर्णांक) nominal_width;
      encoder.encode_int (delta);
    }

    // Use specialized commands if available, otherwise use binary
    जर (plan.capture_commands && i < plan.charstring_commands.length &&
        plan.charstring_commands[i].length > 0)
    {
      // Specialize and encode commands
      स्वतः &commands = plan.charstring_commands[i];
      CFF::specialize_commands (commands, 48);  /* maxstack=48 for CFF1 */
      जर (unlikely (!CFF::encode_commands (commands, cff1_charstrings[i])))
      {
        c->काढा_discard ();
        परत असत्य;
      }
    }
    अन्यथा
    {
      // Use binary CharString
      स्थिरांक str_buff_t &cs = plan.subset_charstrings[i];
      साठी (अचिन्हित j = 0; j < cs.length; j++)
        cff1_charstrings[i].push (cs[j]);
    }

    // Check if it already ends with endchar (0x0e) or return (0x0b)
    जर (cff1_charstrings[i].length == 0 ||
        (cff1_charstrings[i].tail () != 0x0e && cff1_charstrings[i].tail () != 0x0b))
    {
      // Append endchar operator
      जर (unlikely (!cff1_charstrings[i].ढकला_or_fail (0x0e)))
      {
        c->काढा_discard ();
        परत असत्य;
      }
    }
  }

  अचिन्हित data_size = 0;
  अचिन्हित total_size = CFF1CharStrings::total_size (cff1_charstrings, &data_size, plan.min_charstrings_off_size);
  जर (unlikely (!c->start_zerocopy (total_size)))
  {
    c->काढा_discard ();
    परत असत्य;
  }

  स्वतः *cs = c->start_embed<CFF1CharStrings> ();
  जर (unlikely (!cs->serialize (c, cff1_charstrings, &data_size, plan.min_charstrings_off_size)))
  {
    c->काढा_discard ();
    परत असत्य;
  }

  plan.info.char_strings_link = c->काढा_pack (असत्य);
  परत सत्य;
}

/* Serialize CID Charset (format 2 range: gid 0-N -> cid 0-N) */
स्थिर तार्किक
_serialize_cff1_charset (hb_serialize_context_t *c,
                         अचिन्हित पूर्णांक अंक_glyphs,
                         objidx_t &charset_link)
{
  // For CID fonts, create a simple identity charset
  // Format 2: one range covering all glyphs (except .notdef)
  c->push ();

  स्वतः *charset = c->start_embed<Charset> ();
  जर (unlikely (!charset))
  {
    c->काढा_discard ();
    परत असत्य;
  }

  // Create a single range for CID 1 to num_glyphs-1
  hb_vector_t<code_pair_t> ranges;
  जर (अंक_glyphs > 1)
  {
    code_pair_t range;
    range.code = 1;  // first CID
    range.glyph = अंक_glyphs - 2;  // nLeft (covers glyphs 1 to num_glyphs-1)
    ranges.push (range);
  }

  जर (unlikely (!charset->serialize (c, 2, अंक_glyphs, ranges)))
  {
    c->काढा_discard ();
    परत असत्य;
  }

  charset_link = c->काढा_pack ();
  परत सत्य;
}

/* CFF2 to CFF1 serialization */
नामाकाश CFF {

तार्किक
serialize_cff2_to_cff1 (hb_serialize_context_t *c,
                        OT::cff2_subset_plan &plan,
                        स्थिरांक cff2_शीर्ष_dict_मूल्ये_t &cff2_शीर्ष_Dict,
                        स्थिरांक OT::cff2::accelerator_subset_t &acc)
{
  TRACE_SERIALIZE (हे);

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

    साठी (अचिन्हित gid = 0; gid < plan.अंक_glyphs; gid++)
    {
      अचिन्हित width = 0;
      जर (plan.hmtx_map->has (gid))
        width = plan.hmtx_map->get (gid).first;
      widths.push (width);
    }

    // Optimize defaultWidthX and nominalWidthX
    CFF::optimize_widths (widths, plan.default_width, plan.nominal_width);
  }

  // 1. CharStrings (with widths prepended)
  जर (!_serialize_cff1_charstrings (c, plan, plan.default_width, plan.nominal_width))
    return_trace (असत्य);

  // 2. Private DICs & Local Subrs (same as CFF2)
  hb_vector_t<table_info_t> private_dict_infos;
  जर (unlikely (!private_dict_infos.resize (plan.subset_fdcount)))
    return_trace (असत्य);

  साठी (पूर्णांक i = (पूर्णांक)acc.privateDicts.length; --i >= 0;)
  {
    जर (plan.fdmap.has (i))
    {
      objidx_t subrs_link = 0;

      जर (plan.subset_localsubrs[i].length > 0)
      {
        स्वतः *dest = c->push<CFF1Subrs> ();
        जर (likely (dest->serialize (c, plan.subset_localsubrs[i])))
          subrs_link = c->काढा_pack (असत्य);
        अन्यथा
        {
          c->काढा_discard ();
          return_trace (असत्य);
        }
      }

      स्वतः *pd = c->push<PrivateDict> ();
      // Use the CFF2 Private DICT serializer which instantiates blends when pinned=true
      cff2_private_dict_op_serializer_t privSzr (plan.desubroutinize, plan.drop_hints, plan.pinned,
                                                  acc.varStore, plan.normalized_coords);
      जर (likely (pd->serialize (c, acc.privateDicts[i], privSzr, subrs_link)))
      {
        // Add defaultWidthX and nominalWidthX for CFF1
        str_buff_t width_ops;
        str_encoder_t encoder (width_ops);
        encoder.encode_int (plan.default_width);
        encoder.encode_op (OpCode_defaultWidthX);
        encoder.encode_int (plan.nominal_width);
        encoder.encode_op (OpCode_nominalWidthX);

        जर (!encoder.in_error () && c->embed (width_ops.as_bytes ().arrayZ, width_ops.length))
        {
          अचिन्हित fd = plan.fdmap[i];
          private_dict_infos[fd].size = c->length ();
          private_dict_infos[fd].link = c->काढा_pack ();
        }
        अन्यथा
        {
          c->काढा_discard ();
          return_trace (असत्य);
        }
      }
      अन्यथा
      {
        c->काढा_discard ();
        return_trace (असत्य);
      }
    }
  }

  // 3. FDArray - serialize CFF2 font dicts as CFF1
  {
    स्वतः *fda = c->push<FDArray<HBUINT16>> ();
    cff_font_dict_op_serializer_t fontSzr;
    स्वतः it =
    + hb_zip (+ hb_iter (acc.fontDicts)
              | hb_filter ([&] (स्थिरांक cff2_font_dict_मूल्ये_t &_)
                { परत plan.fdmap.has (&_ - &acc.fontDicts[0]); }),
              hb_iter (private_dict_infos))
    ;
    // Explicitly specify template parameters: DICTVAL, INFO
    तार्किक success = fda->serialize<cff2_font_dict_मूल्ये_t, table_info_t> (c, it, fontSzr);
    जर (success)
      plan.info.fd_array_link = c->काढा_pack (असत्य);
    अन्यथा
    {
      c->काढा_discard ();
      return_trace (असत्य);
    }
  }

  // 4. FDSelect (required in CFF1 CID-keyed fonts)
  // CFF1 requires FDSelect for all CID-keyed fonts, even with just one FD
  // CFF2 makes it optional when there's only one FD
  जर (acc.fdSelect != &Null (CFF2FDSelect))
  {
    c->push ();
    जर (likely (hb_serialize_cff_fdselect (c, plan.अंक_glyphs,
                                          *(स्थिरांक FDSelect *)acc.fdSelect,
                                          plan.orig_fdcount,
                                          plan.subset_fdselect_format,
                                          plan.subset_fdselect_size,
                                          plan.subset_fdselect_ranges)))
      plan.info.fd_select.link = c->काढा_pack ();
    अन्यथा
    {
      c->काढा_discard ();
      return_trace (असत्य);
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
    जर (unlikely (!c->embed (format)))
    {
      c->काढा_discard ();
      return_trace (असत्य);
    }

    // nRanges
    HBUINT16 nRanges;
    nRanges = 1;
    जर (unlikely (!c->embed (nRanges)))
    {
      c->काढा_discard ();
      return_trace (असत्य);
    }

    // Single range: {first: 0, fd: 0}
    FDSelect3_Range range;
    range.first = 0;
    range.fd = 0;
    जर (unlikely (!c->embed (range)))
    {
      c->काढा_discard ();
      return_trace (असत्य);
    }

    // Sentinel (number of glyphs)
    HBUINT16 sentinel;
    sentinel = plan.अंक_glyphs;
    जर (unlikely (!c->embed (sentinel)))
    {
      c->काढा_discard ();
      return_trace (असत्य);
    }

    plan.info.fd_select.link = c->काढा_pack ();
  }

  // 5. Charset (CID charset for identity mapping)
  objidx_t charset_link;
  जर (!_serialize_cff1_charset (c, plan.अंक_glyphs, charset_link))
    return_trace (असत्य);

  // 6. Global Subrs
  {
    स्वतः *dest = c->push<CFF1Subrs> ();
    जर (likely (dest->serialize (c, plan.subset_globalsubrs)))
      c->काढा_pack (असत्य);
    अन्यथा
    {
      c->काढा_discard ();
      return_trace (असत्य);
    }
  }

  // 7. String INDEX - Add "Adobe" and "Identity" for ROS operator
  {
    स्थिरांक अक्षर *adobe_str = "Adobe";
    स्थिरांक अक्षर *identity_str = "Identity";
    अचिन्हित adobe_len = 5;  // strlen("Adobe")
    अचिन्हित identity_len = 8;  // strlen("Identity")

    // Build strings array
    hb_vector_t<hb_ubytes_t> strings;
    strings.alloc (2);
    strings.push (hb_ubytes_t ((स्थिरांक अचिन्हित अक्षर *) adobe_str, adobe_len));
    strings.push (hb_ubytes_t ((स्थिरांक अचिन्हित अक्षर *) identity_str, identity_len));

    // Serialize as CFF INDEX
    स्वतः *dest = c->push<CFF1Index> ();
    जर (likely (dest->serialize (c, strings)))
      c->काढा_pack (असत्य);
    अन्यथा
    {
      c->काढा_discard ();
      return_trace (असत्य);
    }
  }

  // 8. CFF Header
  OT::cff1 *cff = c->allocate_min<OT::cff1> ();
  जर (unlikely (!cff)) return_trace (असत्य);

  /* header */
  cff->version.major = 0x01;
  cff->version.minor = 0x00;
  cff->nameIndex = cff->min_size;
  cff->offSize = 4; /* unused? */

  // 9. Name INDEX (single entry)
  {
    अचिन्हित name_len = म_माप (CFF1_DEFAULT_FONT_NAME);

    CFF1Index *idx = c->start_embed<CFF1Index> ();
    जर (unlikely (!idx)) return_trace (असत्य);

    जर (unlikely (!idx->serialize_header (c, hb_iter (&name_len, 1), name_len)))
      return_trace (असत्य);

    जर (unlikely (!c->embed (CFF1_DEFAULT_FONT_NAME, name_len)))
      return_trace (असत्य);
  }

  // 10. Top DICT INDEX
  {
    // Serialize the Top DICT data first
    c->push<शीर्ष_Dict> ();
    cff1_from_cff2_शीर्ष_dict_op_serializer_t शीर्ष_Szr;

    // Serialize ROS first
    जर (unlikely (!शीर्ष_Szr.serialize_ros (c)))
    {
      c->काढा_discard ();
      return_trace (असत्य);
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

      जर (encoder.in_error () || !c->embed (bbox_buff.as_bytes ().arrayZ, bbox_buff.length))
      {
        c->काढा_discard ();
        return_trace (असत्य);
      }
    }

    // Serialize charset operator
    जर (charset_link && unlikely (!FontDict::serialize_link4_op (c, OpCode_charset, charset_link, whence_t::Absolute)))
    {
      c->काढा_discard ();
      return_trace (असत्य);
    }

    // Serialize FDSelect operator (required for CID-keyed CFF1 fonts)
    जर (plan.info.fd_select.link && unlikely (!FontDict::serialize_link4_op (c, OpCode_FDSelect, plan.info.fd_select.link, whence_t::Absolute)))
    {
      c->काढा_discard ();
      return_trace (असत्य);
    }

    // Serialize FDArray operator (required for CID-keyed CFF1 fonts)
    जर (plan.info.fd_array_link && unlikely (!FontDict::serialize_link4_op (c, OpCode_FDArray, plan.info.fd_array_link, whence_t::Absolute)))
    {
      c->काढा_discard ();
      return_trace (असत्य);
    }

    // Serialize other operators from CFF2 TopDict
    साठी (स्थिरांक स्वतः &opstr : cff2_शीर्ष_Dict.values)
    {
      जर (unlikely (!शीर्ष_Szr.serialize (c, opstr, plan.info)))
      {
        c->काढा_discard ();
        return_trace (असत्य);
      }
    }

    अचिन्हित शीर्ष_size = c->length ();
    c->काढा_pack (असत्य);

    // Serialize INDEX header
    स्वतः *dest = c->start_embed<CFF1Index> ();
    जर (unlikely (!dest->serialize_header (c, hb_iter (&शीर्ष_size, 1), शीर्ष_size)))
      return_trace (असत्य);
  }

  return_trace (सत्य);
}

} /* namespace CFF */

स्थिर तार्किक _serialize_cff2_charstrings (hb_serialize_context_t *c,
			     cff2_subset_plan &plan,
			     स्थिरांक OT::cff2::accelerator_subset_t  &acc)
{
  c->push ();

  अचिन्हित data_size = 0;
  अचिन्हित total_size = CFF2CharStrings::total_size (plan.subset_charstrings, &data_size, plan.min_charstrings_off_size);
  जर (unlikely (!c->start_zerocopy (total_size)))
    परत असत्य;

  स्वतः *cs = c->start_embed<CFF2CharStrings> ();
  जर (unlikely (!cs->serialize (c, plan.subset_charstrings, &data_size, plan.min_charstrings_off_size)))
  {
    c->काढा_discard ();
    परत असत्य;
  }

  plan.info.char_strings_link = c->काढा_pack (असत्य);
  परत सत्य;
}

तार्किक
OT::cff2::accelerator_subset_t::serialize (hb_serialize_context_t *c,
					   संरचना cff2_subset_plan &plan,
					   hb_array_t<पूर्णांक> normalized_coords) स्थिरांक
{
  /* push charstrings onto the object stack first which will ensure it packs as the last
     object in the table. Keeping the chastrings last satisfies the requirements for patching
     via IFTB. If this ordering needs to be changed in the future, charstrings should be left
     at the end whenever HB_SUBSET_FLAGS_ITFB_REQUIREMENTS is enabled. */
  जर (!_serialize_cff2_charstrings(c, plan, *हे))
    परत असत्य;

  /* private dicts & local subrs */
  hb_vector_t<table_info_t>  private_dict_infos;
  जर (unlikely (!private_dict_infos.resize (plan.subset_fdcount))) परत असत्य;

  साठी (पूर्णांक i = (पूर्णांक)privateDicts.length; --i >= 0 ;)
  {
    जर (plan.fdmap.has (i))
    {
      objidx_t	subrs_link = 0;

      जर (plan.subset_localsubrs[i].length > 0)
      {
	स्वतः *dest = c->push <CFF2Subrs> ();
	जर (likely (dest->serialize (c, plan.subset_localsubrs[i])))
	  subrs_link = c->काढा_pack (असत्य);
	अन्यथा
	{
	  c->काढा_discard ();
	  परत असत्य;
	}
      }
      स्वतः *pd = c->push<PrivateDict> ();
      cff2_private_dict_op_serializer_t privSzr (plan.desubroutinize, plan.drop_hints, plan.pinned,
						 varStore, normalized_coords,
						 plan.partial_instancing ? &plan.instancing : nullptr);
      जर (likely (pd->serialize (c, privateDicts[i], privSzr, subrs_link)))
      {
	अचिन्हित fd = plan.fdmap[i];
	private_dict_infos[fd].size = c->length ();
	private_dict_infos[fd].link = c->काढा_pack ();
      }
      अन्यथा
      {
	c->काढा_discard ();
	परत असत्य;
      }
    }
  }

  /* FDSelect */
  जर (fdSelect != &Null (CFF2FDSelect))
  {
    c->push ();
    जर (likely (hb_serialize_cff_fdselect (c, plan.अंक_glyphs, *(स्थिरांक FDSelect *)fdSelect,
					   plan.orig_fdcount,
					   plan.subset_fdselect_format, plan.subset_fdselect_size,
					   plan.subset_fdselect_ranges)))
      plan.info.fd_select.link = c->काढा_pack ();
    अन्यथा
    {
      c->काढा_discard ();
      परत असत्य;
    }
  }

  /* FDArray (FD Index) */
  {
    स्वतः *fda = c->push<CFF2FDArray> ();
    cff_font_dict_op_serializer_t fontSzr;
    स्वतः it =
    + hb_zip (+ hb_iter (fontDicts)
	      | hb_filter ([&] (स्थिरांक cff2_font_dict_मूल्ये_t &_)
		{ परत plan.fdmap.has (&_ - &fontDicts[0]); }),
	      hb_iter (private_dict_infos))
    ;
    जर (unlikely (!fda->serialize (c, it, fontSzr)))
    {
      c->काढा_discard ();
      परत असत्य;
    }
    plan.info.fd_array_link = c->काढा_pack (असत्य);
  }

  /* variation store */
  जर (varStore != &Null (CFF2ItemVariationStore) &&
      !plan.pinned)
  {
    जर (plan.partial_instancing)
    {
      जर (plan.instancing.has_variations ())
      {
	c->push ();
	जर (unlikely (!plan.instancing.serialize_var_store (c, *plan.axis_tags)))
	{
	  c->काढा_discard ();
	  परत असत्य;
	}
	plan.info.var_store_link = c->काढा_pack (असत्य);
      }
      /* else: the store emptied; the vstore op gets dropped along with it. */
    }
    अन्यथा
    {
      स्वतः *dest = c->push<CFF2ItemVariationStore> ();
      जर (unlikely (!dest->serialize (c, varStore)))
      {
	c->काढा_discard ();
	परत असत्य;
      }
      plan.info.var_store_link = c->काढा_pack (असत्य);
    }
  }

  OT::cff2 *cff2 = c->allocate_min<OT::cff2> ();
  जर (unlikely (!cff2)) परत असत्य;

  /* header */
  cff2->version.major = 0x02;
  cff2->version.minor = 0x00;
  cff2->शीर्ष_Dict_2 = OT::cff2::static_size;

  /* top dict */
  {
    शीर्ष_Dict &dict = cff2 + cff2->शीर्ष_Dict_2;
    cff2_शीर्ष_dict_op_serializer_t शीर्ष_Szr;
    जर (unlikely (!dict.serialize (c, शीर्ष_Dict_2, शीर्ष_Szr, plan.info))) परत असत्य;
    cff2->शीर्ष_Dict_Size = c->head - (स्थिरांक अक्षर *)&dict;
  }

  /* global subrs */
  {
    स्वतः *dest = c->start_embed <CFF2Subrs> ();
    परत dest->serialize (c, plan.subset_globalsubrs);
  }
}

तार्किक
OT::cff2::accelerator_subset_t::subset (hb_subset_context_t *c) स्थिरांक
{
  cff2_subset_plan cff2_plan;

  जर (unlikely (!cff2_plan.create (*हे, c->plan))) परत असत्य;

  // If instantiating (pinned) and downgrade flag is set, convert to CFF1
  जर (cff2_plan.pinned && (c->plan->flags & HB_SUBSET_FLAGS_DOWNGRADE_CFF2))
  {
    // Serialize CFF1 to the subsetter's serializer
    // If we run out of room, returning true will cause subsetter to retry with larger buffer
    तार्किक result = CFF::serialize_cff2_to_cff1 (c->serializer, cff2_plan, शीर्ष_Dict_2, *हे);

    जर (c->serializer->ran_out_of_room ())
      परत सत्य; // Subsetter will retry with larger buffer

    जर (result && !c->serializer->in_error ())
    {
      // Success - end serialization to resolve links
      c->serializer->end_serialize ();

      // Copy the serialized CFF1 data and add as CFF table
      hb_blob_t *cff_blob = c->serializer->copy_blob ();
      जर (cff_blob)
      {
        c->plan->add_table (HB_TAG('C','F','F',' '), cff_blob);
        hb_blob_destroy (cff_blob);

        // Return false to signal CFF2 table is not needed
        परत असत्य;
      }
    }

    // Conversion failed - don't fall back, fail hard for debugging
    परत असत्य;
  }

  परत serialize (c->serializer, cff2_plan,
		    c->plan->normalized_coords.as_array ());
}

#endif
