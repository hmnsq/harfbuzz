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
    hb_vector_t<hb_vector_t<भिन्न>> matrix;
    /* Default-value fold-in scalars, one per old region: the old store's
     * region scalars at the new default location. */
    hb_vector_t<भिन्न> gains;
    /* Surviving regions, parallel to matrix rows. */
    hb_vector_t<hb_hashmap_t<hb_tag_t, Triple>> regions;
    /* Index into the new global region list, per new region (finalize). */
    hb_vector_t<unsigned> region_indices;
    unsigned new_major = 0;
    तार्किक alive_ = मिच्छ;
    तार्किक alive () ध्रुव { प्रत्यागम alive_; }

    /* One value's delta for new region j, from its old per-region deltas. */
    द्विगुण transform_delta (unsigned j, hb_array_t<ध्रुव सङ्ख्या_t> deltas) ध्रुव
    {
      ध्रुव hb_vector_t<भिन्न> &col = matrix.arrayZ[j];
      द्विगुण d = 0.;
      unsigned गणनम् = hb_min (col.length, deltas.length);
      प्रति (unsigned c = 0; c < गणनम्; c++)
	d += (द्विगुण) col.arrayZ[c] * deltas.arrayZ[c].to_real ();
      प्रत्यागम d;
    }
  };

  hb_vector_t<vardata_transform_t> transforms;  /* per old VarData (vsindex) */
  hb_vector_t<hb_hashmap_t<hb_tag_t, Triple>> new_regions;  /* global region list */
  hb_set_t used_majors;
  unsigned new_major_गणनम् = 0;

  तार्किक has_variations () ध्रुव { प्रत्यागम new_major_गणनम्; }

  ध्रुव vardata_transform_t *get_transform (unsigned major) ध्रुव
  { प्रत्यागम major < transforms.length ? &transforms.arrayZ[major] : nullptr; }

  /* Record that a retained blend references this VarData. */
  शून्य note_use (unsigned major) { used_majors.add (major); }

  /* The new vsindex a private dict's (or charstring's) old ivs maps to;
   * dead VarDatas fall back to 0, their blends having become static. */
  unsigned remap_ivs (unsigned old_ivs) ध्रुव
  {
    ध्रुव vardata_transform_t *t = get_transform (old_ivs);
    प्रत्यागम (t && t->alive ()) ? t->new_major : 0;
  }

  द्विगुण fold_deltas (unsigned old_ivs, hb_array_t<ध्रुव सङ्ख्या_t> deltas) ध्रुव
  {
    ध्रुव vardata_transform_t *t = get_transform (old_ivs);
    जइ (unlikely (!t || t->gains.length != deltas.length)) प्रत्यागम 0.;
    द्विगुण v = 0.;
    प्रति (unsigned i = 0; i < deltas.length; i++)
      v += (द्विगुण) t->gains.arrayZ[i] * deltas.arrayZ[i].to_real ();
    प्रत्यागम v;
  }

  तार्किक create (ध्रुव CFF2ItemVariationStore *cff2VarStore,
	       ध्रुव hb_subset_plan_t *plan)
  {
    ध्रुव OT::ItemVariationStore &varStore = cff2VarStore->varStore;
    unsigned major_गणनम् = varStore.get_sub_table_गणनम् ();
    जइ (!transforms.resize (major_गणनम्)) प्रत्यागम मिच्छ;

    जइ (!varStore.get_region_list ().get_var_regions (plan->axes_old_index_tag_map, orig_regions))
      प्रत्यागम मिच्छ;

    /* Instance each VarData with identity delta rows; the surviving tuples'
     * delta columns are the old-region → new-region transform. */
    OT::optimize_scratch_t scratch;
    प्रति (unsigned m = 0; m < major_गणनम्; m++)
    {
      ध्रुव OT::VarData &var_data = varStore.get_sub_table (m);
      unsigned k = var_data.get_region_index_गणनम् ();
      vardata_transform_t &t = transforms.arrayZ[m];

      जइ (!t.gains.resize_exact (k)) प्रत्यागम मिच्छ;
      varStore.get_region_scalars (m, plan->normalized_coords.arrayZ,
				   plan->normalized_coords.length,
				   t.gains.arrayZ, k);

      OT::tuple_variations_t tuples;
      प्रति (unsigned r = 0; r < k; r++)
      {
	OT::tuple_delta_t tuple;
	जइ (!tuple.deltas_x.resize (k) ||
	    !tuple.indices.resize (k))
	  प्रत्यागम मिच्छ;
	प्रति (unsigned i = 0; i < k; i++)
	  tuple.indices.arrayZ[i] = सच्च;
	tuple.deltas_x.arrayZ[r] = 1.f;
	unsigned region_index = var_data.get_region_index (r);
	जइ (unlikely (region_index >= orig_regions.length)) प्रत्यागम मिच्छ;
	tuple.axis_tuples = orig_regions.arrayZ[region_index];
	जइ (unlikely (tuple.axis_tuples.in_error ())) प्रत्यागम मिच्छ;
	tuples.tuple_vars.push (std::move (tuple));
      }
      जइ (unlikely (tuples.tuple_vars.in_error ())) प्रत्यागम मिच्छ;

      जइ (!tuples.instantiate (plan->axes_location, plan->axes_triple_distances, scratch))
	प्रत्यागम मिच्छ;

      प्रति (auto &tuple : tuples.tuple_vars)
      {
	जइ (unlikely (tuple.deltas_x.length != k)) प्रत्यागम मिच्छ;
	hb_vector_t<भिन्न> col;
	जइ (unlikely (!col.resize_exact (k))) प्रत्यागम मिच्छ;
	प्रति (unsigned i = 0; i < k; i++)
	  col.arrayZ[i] = tuple.deltas_x.arrayZ[i];
	t.matrix.push (std::move (col));
	t.regions.push (std::move (tuple.axis_tuples));
      }
      जइ (unlikely (t.matrix.in_error () || t.regions.in_error ())) प्रत्यागम मिच्छ;
    }

    प्रत्यागम सच्च;
  }

  /* Decide which VarDatas survive, number them, and build the new global
   * region list: surviving original regions first, in their original order,
   * then solver-split new regions in order of appearance (fontTools'
   * rebuildRegions + prune_regions). */
  तार्किक finalize ()
  {
    प्रति (unsigned m = 0; m < transforms.length; m++)
    {
      vardata_transform_t &t = transforms.arrayZ[m];
      t.alive_ = t.matrix.length && used_majors.has (m);
      जइ (t.alive_)
	t.new_major = new_major_गणनम्++;
    }

    /* Keys hash by content, so pointers into different owners compare fine. */
    hb_hashmap_t<ध्रुव hb_hashmap_t<hb_tag_t, Triple>*, unsigned> region_idx_map;
    hb_vector_t<ध्रुव hb_hashmap_t<hb_tag_t, Triple>*> region_ptrs;
    प्रति (ध्रुव vardata_transform_t &t : transforms)
    {
      जइ (!t.alive ()) अनुवर्त;
      प्रति (ध्रुव auto &region : t.regions)
	जइ (!region_idx_map.has (&region))
	  जइ (unlikely (!region_idx_map.set (&region, (unsigned) -1)))
	    प्रत्यागम मिच्छ;
    }
    प्रति (ध्रुव auto &r : orig_regions)
    {
      unsigned *idx;
      जइ (region_idx_map.has (&r, &idx) && *idx == (unsigned) -1)
      {
	जइ (unlikely (!region_idx_map.set (&r, region_ptrs.length))) प्रत्यागम मिच्छ;
	region_ptrs.push (&r);
      }
    }
    प्रति (ध्रुव vardata_transform_t &t : transforms)
    {
      जइ (!t.alive ()) अनुवर्त;
      प्रति (ध्रुव auto &region : t.regions)
      {
	unsigned *idx;
	जइ (region_idx_map.has (&region, &idx) && *idx == (unsigned) -1)
	{
	  जइ (unlikely (!region_idx_map.set (&region, region_ptrs.length))) प्रत्यागम मिच्छ;
	  region_ptrs.push (&region);
	}
      }
    }
    जइ (unlikely (region_ptrs.in_error ())) प्रत्यागम मिच्छ;

    प्रति (vardata_transform_t &t : transforms)
    {
      जइ (!t.alive ()) अनुवर्त;
      प्रति (ध्रुव auto &region : t.regions)
      {
	unsigned *idx;
	जइ (unlikely (!region_idx_map.has (&region, &idx))) प्रत्यागम मिच्छ;
	t.region_indices.push (*idx);
      }
      जइ (unlikely (t.region_indices.in_error ())) प्रत्यागम मिच्छ;
    }

    जइ (!new_regions.resize (region_ptrs.length)) प्रत्यागम मिच्छ;
    प्रति (unsigned i = 0; i < region_ptrs.length; i++)
    {
      new_regions.arrayZ[i] = *region_ptrs.arrayZ[i];
      जइ (unlikely (new_regions.arrayZ[i].in_error ())) प्रत्यागम मिच्छ;
    }

    प्रत्यागम सच्च;
  }

  /* Serialize the instanced store: a CFF2ItemVariationStore whose VarDatas
   * carry no items, just region indexes (blend deltas live in charstrings).
   * Laid out manually since all sizes are known up front. */
  तार्किक serialize_var_store (hb_serialize_context_t *c,
			    ध्रुव hb_vector_t<hb_tag_t> &axis_tags) ध्रुव
  {
    unsigned data_गणनम् = new_major_गणनम्;
    unsigned header_size = 2 + 4 + 2 + 4 * data_गणनम्;

    uint64_t offset = header_size;
    hb_vector_t<unsigned> data_offsets;
    प्रति (ध्रुव vardata_transform_t &t : transforms)
    {
      जइ (!t.alive ()) अनुवर्त;
      data_offsets.push ((unsigned) offset);
      offset += 6 + 2 * (uint64_t) t.region_indices.length;
    }
    जइ (unlikely (data_offsets.in_error () || data_offsets.length != data_गणनम्))
      प्रत्यागम मिच्छ;
    unsigned regions_offset = (unsigned) offset;
    uint64_t total = offset + 4 + (uint64_t) new_regions.length * axis_tags.length * 6;
    जइ (unlikely (total > 0xFFFFFFFFu)) प्रत्यागम मिच्छ;

    HBUINT16 *size_field = c->allocate_size<HBUINT16> (HBUINT16::static_size);
    जइ (unlikely (!size_field)) प्रत्यागम मिच्छ;
    /* OFF 2026 5.3.3.11.2 permits larger stores with length=65535. */
    *size_field = hb_min (total, (uint64_t) 0xFFFFu);

    वर्ण *header = c->allocate_size<वर्ण> (header_size);
    जइ (unlikely (!header)) प्रत्यागम मिच्छ;
    * (HBUINT16 *) (header + 0) = 1;			/* format */
    * (HBUINT32 *) (header + 2) = regions_offset;
    * (HBUINT16 *) (header + 6) = data_गणनम्;
    प्रति (unsigned i = 0; i < data_गणनम्; i++)
      * (HBUINT32 *) (header + 8 + 4 * i) = data_offsets.arrayZ[i];

    प्रति (ध्रुव vardata_transform_t &t : transforms)
    {
      जइ (!t.alive ()) अनुवर्त;
      unsigned k = t.region_indices.length;
      HBUINT16 *v = (HBUINT16 *) c->allocate_size<वर्ण> (6 + 2 * k);
      जइ (unlikely (!v)) प्रत्यागम मिच्छ;
      v[0] = 0;						/* itemCount */
      v[1] = 0;						/* wordSizeCount */
      v[2] = k;						/* regionIndices.len */
      प्रति (unsigned j = 0; j < k; j++)
	v[3 + j] = t.region_indices.arrayZ[j];
    }

    hb_vector_t<ध्रुव hb_hashmap_t<hb_tag_t, Triple>*> region_ptrs;
    जइ (unlikely (!region_ptrs.alloc_exact (new_regions.length))) प्रत्यागम मिच्छ;
    प्रति (ध्रुव auto &r : new_regions)
      region_ptrs.push (&r);
    auto *region_list = c->start_embed<OT::VarRegionList> ();
    प्रत्यागम region_list->serialize (c, axis_tags, region_ptrs);
  }

  गुप्त:
  hb_vector_t<hb_hashmap_t<hb_tag_t, Triple>> orig_regions;
};

/* Usage-collection pass for partial instancing: record which VarDatas the
 * retained charstrings' and private dicts' blends reference.  Mirrors the
 * emission pass: blends consumed by dropped hint ops don't count. */
संरचना cff2_usage_param_t
{
  शून्य init () {}

  cff2_instancing_plan_t *instancing = nullptr;
  तार्किक drop_hints = मिच्छ;
  unsigned ivs = 0;	/* dict-side current vsindex */
};

संरचना cff2_cs_opset_usage_t : cff2_cs_opset_t<cff2_cs_opset_usage_t, cff2_usage_param_t, blend_arg_t>
{
  स्थिर शून्य flush_args_and_op (op_code_t op, cff2_cs_interp_env_t<blend_arg_t> &env, cff2_usage_param_t& param)
  {
    तार्किक counts = सच्च;
    विकल्प (op)
    {
      पक्ष OpCode_return:
      पक्ष OpCode_endchar:
	counts = मिच्छ;
	भङ्ग;

      पक्ष OpCode_hstem:
      पक्ष OpCode_hstemhm:
      पक्ष OpCode_vstem:
      पक्ष OpCode_vstemhm:
      पक्ष OpCode_hintmask:
      पक्ष OpCode_cntrmask:
	जइ (param.drop_hints)
	  counts = मिच्छ;
	भङ्ग;

      स्वभाव:
	भङ्ग;
    }

    जइ (counts)
      प्रति (unsigned पूर्णाङ्क i = 0; i < env.arg_राशिः.get_गणनम् (); i++)
	जइ (env.arg_राशिः[i].blending ())
	{
	  param.instancing->note_use (env.get_ivs ());
	  भङ्ग;
	}

    env.clear_args ();
  }
};

संरचना cff2_private_dict_usage_opset_t : dict_opset_t
{
  स्थिर शून्य process_op (op_code_t op, cff2_priv_dict_interp_env_t& env, cff2_usage_param_t& param)
  {
    विकल्प (op) {
      पक्ष OpCode_vsindexdict:
	env.process_vsindex ();
	param.ivs = env.get_ivs ();
	env.clear_args ();
	प्रत्यागम;
      पक्ष OpCode_blenddict:
	param.instancing->note_use (param.ivs);
	env.clear_args ();
	प्रत्यागम;
      स्वभाव:
	dict_opset_t::process_op (op, env);
	जइ (!env.arg_राशिः.is_empty ()) प्रत्यागम;
	env.clear_args ();
	प्रत्यागम;
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

संरचना cff2_शिखरम्_dict_op_serializer_t : cff_शिखरम्_dict_op_serializer_t<>
{
  तार्किक serialize (hb_serialize_context_t *c,
		  ध्रुव op_str_t &opstr,
		  ध्रुव cff2_sub_table_info_t &info) ध्रुव
  {
    TRACE_SERIALIZE (एतत्);

    विकल्प (opstr.op)
    {
      पक्ष OpCode_vstore:
        जइ (info.var_store_link)
	  return_trace (FontDict::serialize_link4_op(c, opstr.op, info.var_store_link));
	अण्णहा
	  return_trace (सच्च);

      स्वभाव:
	return_trace (cff_शिखरम्_dict_op_serializer_t<>::serialize (c, opstr, info));
    }
  }
};

संरचना cff2_cs_opset_flatten_t : cff2_cs_opset_t<cff2_cs_opset_flatten_t, flatten_param_t, blend_arg_t>
{
  स्थिर शून्य flush_args_and_op (op_code_t op, cff2_cs_interp_env_t<blend_arg_t> &env, flatten_param_t& param)
  {
    /* Optionally capture command for specialization (before flushing, to preserve args) */
    जइ (param.commands)
    {
      तार्किक skip_command = मिच्छ;

      विकल्प (op)
      {
	पक्ष OpCode_return:
	पक्ष OpCode_endchar:
	  skip_command = सच्च;
	  भङ्ग;

	पक्ष OpCode_hstem:
	पक्ष OpCode_hstemhm:
	पक्ष OpCode_vstem:
	पक्ष OpCode_vstemhm:
	पक्ष OpCode_hintmask:
	पक्ष OpCode_cntrmask:
	  जइ (param.drop_hints)
	    skip_command = सच्च;
	  भङ्ग;

	स्वभाव:
	  भङ्ग;
      }

      जइ (!skip_command)
      {
	cs_command_t cmd (op);
	/* Capture resolved blend values */
	प्रति (unsigned पूर्णाङ्क i = 0; i < env.arg_राशिः.get_गणनम् ();)
	{
	  ध्रुव blend_arg_t &arg = env.arg_राशिः[i];
	  जइ (arg.blending ())
	  {
	    /* For blend args, capture only the resolved default value */
	    cmd.args.push (arg);
	    /* Skip over the multiple blend values */
	    i += arg.अङ्कः_मानानि_2;
	  }
	  अण्णहा
	  {
	    cmd.args.push (arg);
	    i++;
	  }
	}
	param.commands->push (cmd);
      }
    }

    विकल्प (op)
    {
      पक्ष OpCode_return:
      पक्ष OpCode_endchar:
	/* dummy opcodes in CFF2. ignore */
	जइ (op == OpCode_endchar && param.instancer)
	  emit_vsindex_prefix (env, param);
	भङ्ग;

      पक्ष OpCode_hstem:
      पक्ष OpCode_hstemhm:
      पक्ष OpCode_vstem:
      पक्ष OpCode_vstemhm:
      पक्ष OpCode_hintmask:
      पक्ष OpCode_cntrmask:
	जइ (param.drop_hints)
	{
	  env.clear_args ();
	  प्रत्यागम;
	}
	HB_FALLTHROUGH;

      स्वभाव:
	SUPER::flush_args_and_op (op, env, param);
	भङ्ग;
    }
  }

  स्थिर शून्य flush_args (cff2_cs_interp_env_t<blend_arg_t> &env, flatten_param_t& param)
  {
    प्रति (unsigned पूर्णाङ्क i = 0; i < env.arg_राशिः.get_गणनम् ();)
    {
      ध्रुव blend_arg_t &arg = env.arg_राशिः[i];
      जइ (arg.blending ())
      {
	जइ (unlikely (!((arg.अङ्कः_मानानि_2 > 0) && (env.arg_राशिः.get_गणनम् () >= arg.अङ्कः_मानानि_2))))
	{
	  env.set_error ();
	  प्रत्यागम;
	}
	जइ (param.instancer)
	  rewrite_blends (arg, i, env, param);
	अण्णहा
	  flatten_blends (arg, i, env, param);
	i += arg.अङ्कः_मानानि_2;
      }
      अण्णहा
      {
	str_encoder_t  encoder (param.flatStr);
	encoder.encode_अङ्कः_cs (arg);
	i++;
      }
    }
    SUPER::flush_args (env, param);
  }

  स्थिर शून्य flatten_blends (ध्रुव blend_arg_t &arg, unsigned पूर्णाङ्क i, cff2_cs_interp_env_t<blend_arg_t> &env, flatten_param_t& param)
  {
    /* flatten the default values */
    str_encoder_t  encoder (param.flatStr);
    प्रति (unsigned पूर्णाङ्क j = 0; j < arg.अङ्कः_मानानि_2; j++)
    {
      ध्रुव blend_arg_t &arg1 = env.arg_राशिः[i + j];
      जइ (unlikely (!((arg1.blending () && (arg.अङ्कः_मानानि_2 == arg1.अङ्कः_मानानि_2) && (arg1.मानम्_Index_2 == j) &&
	      (arg1.deltas.length == env.get_region_गणनम् ())))))
      {
	env.set_error ();
	प्रत्यागम;
      }
      encoder.encode_अङ्कः_cs (arg1);
    }
    /* flatten deltas for each value */
    प्रति (unsigned पूर्णाङ्क j = 0; j < arg.अङ्कः_मानानि_2; j++)
    {
      ध्रुव blend_arg_t &arg1 = env.arg_राशिः[i + j];
      प्रति (unsigned पूर्णाङ्क k = 0; k < arg1.deltas.length; k++)
	encoder.encode_अङ्कः_cs (arg1.deltas[k]);
    }
    /* flatten the number of values followed by blend operator */
    encoder.encode_int (arg.अङ्कः_मानानि_2);
    encoder.encode_op (OpCode_blendcs);
  }

  /* Partial instancing: rewrite one blend group against the instanced store.
   * Each value's default absorbs the folded (pinned-away) contribution, and
   * its deltas are mapped onto the new region list. */
  स्थिर शून्य rewrite_blends (ध्रुव blend_arg_t &arg, unsigned पूर्णाङ्क i, cff2_cs_interp_env_t<blend_arg_t> &env, flatten_param_t& param)
  {
    ध्रुव cff2_instancing_plan_t &instancer = *param.instancer;
    unsigned ivs = env.get_ivs ();
    ध्रुव cff2_instancing_plan_t::vardata_transform_t *t = instancer.get_transform (ivs);
    जइ (unlikely (!t))
    {
      env.set_error ();
      प्रत्यागम;
    }

    unsigned n = arg.अङ्कः_मानानि_2;
    प्रति (unsigned पूर्णाङ्क j = 0; j < n; j++)
    {
      ध्रुव blend_arg_t &arg1 = env.arg_राशिः[i + j];
      जइ (unlikely (!(arg1.blending () && (arg1.अङ्कः_मानानि_2 == n) && (arg1.मानम्_Index_2 == j) &&
		      (arg1.deltas.length == env.get_region_गणनम् ()) &&
		      (arg1.deltas.length == t->gains.length))))
      {
	env.set_error ();
	प्रत्यागम;
      }
    }

    str_encoder_t encoder (param.flatStr);
    unsigned k_new = t->alive () ? t->matrix.length : 0;

    जइ (!k_new)
    {
      /* All of this VarData's regions died; blends dissolve into statics. */
      प्रति (unsigned पूर्णाङ्क j = 0; j < n; j++)
      {
	ध्रुव blend_arg_t &arg1 = env.arg_राशिः[i + j];
	सङ्ख्या_t v;
	v.set_real (arg1.to_real () + round (instancer.fold_deltas (ivs, arg1.deltas.as_array ())));
	encoder.encode_अङ्कः_cs (v);
      }
      प्रत्यागम;
    }

    /* Emit in chunks so the output charstring stays within the argument
     * stack limit: values already emitted for this op, plus this chunk's
     * defaults, deltas and count, must not exceed 513. */
    unsigned done = 0;
    जाव (done < n)
    {
      unsigned avail = 512 > (i + done) ? 512 - (i + done) : 0;
      unsigned chunk = hb_min (avail / (k_new + 1), n - done);
      जइ (unlikely (!chunk))
      {
	env.set_error ();
	प्रत्यागम;
      }

      प्रति (unsigned पूर्णाङ्क j = done; j < done + chunk; j++)
      {
	ध्रुव blend_arg_t &arg1 = env.arg_राशिः[i + j];
	सङ्ख्या_t v;
	v.set_real (arg1.to_real () + round (instancer.fold_deltas (ivs, arg1.deltas.as_array ())));
	encoder.encode_अङ्कः_cs (v);
      }
      प्रति (unsigned पूर्णाङ्क j = done; j < done + chunk; j++)
      {
	ध्रुव blend_arg_t &arg1 = env.arg_राशिः[i + j];
	प्रति (unsigned r = 0; r < k_new; r++)
	{
	  सङ्ख्या_t v;
	  v.set_int (hb_clamp_to<पूर्णाङ्क> (round (t->transform_delta (r, arg1.deltas.as_array ()))));
	  encoder.encode_अङ्कः_cs (v);
	}
      }
      encoder.encode_int (chunk);
      encoder.encode_op (OpCode_blendcs);
      done += chunk;
    }
    param.emitted_blend = सच्च;
  }

  /* Partial instancing: prepend a vsindex op when the charstring's
   * (remapped) ivs differs from its FD's (remapped) private-dict ivs. */
  स्थिर शून्य emit_vsindex_prefix (cff2_cs_interp_env_t<blend_arg_t> &env, flatten_param_t& param)
  {
    जइ (!param.emitted_blend) प्रत्यागम;
    ध्रुव cff2_instancing_plan_t &instancer = *param.instancer;
    unsigned new_ivs = instancer.remap_ivs (env.get_ivs ());
    unsigned fd_ivs = instancer.remap_ivs (env.get_orig_ivs ());
    जइ (new_ivs == fd_ivs) प्रत्यागम;

    str_buff_t prefix;
    str_encoder_t enc (prefix);
    enc.encode_int ((पूर्णाङ्क) new_ivs);
    enc.encode_op (OpCode_vsindexcs);

    str_buff_t &buf = param.flatStr;
    unsigned old_len = buf.length;
    जइ (unlikely (prefix.in_error () || !buf.resize (old_len + prefix.length)))
    {
      env.set_error ();
      प्रत्यागम;
    }
    memmove (buf.arrayZ + prefix.length, buf.arrayZ, old_len);
    hb_memcpy (buf.arrayZ, prefix.arrayZ, prefix.length);
  }

  स्थिर शून्य flush_op (op_code_t op, cff2_cs_interp_env_t<blend_arg_t> &env, flatten_param_t& param)
  {
    विकल्प (op)
    {
      पक्ष OpCode_return:
      पक्ष OpCode_endchar:
	प्रत्यागम;
      स्वभाव:
	str_encoder_t  encoder (param.flatStr);
	encoder.encode_op (op);
    }
  }

  स्थिर शून्य flush_hintmask (op_code_t op, cff2_cs_interp_env_t<blend_arg_t> &env, flatten_param_t& param)
  {
    SUPER::flush_hintmask (op, env, param);
    /* Preserve hintmask payload in captured commands for specializer re-encoding. */
    जइ (param.commands && !param.drop_hints && param.commands->length > 0)
    {
      auto &cmd = param.commands->tail ();
      जइ (cmd.op == op)
      {
        cmd.mask_bytes.resize (env.hintmask_size);
        जइ (unlikely (cmd.mask_bytes.in_error ()))
        {
          env.set_error ();
          प्रत्यागम;
        }
        प्रति (unsigned पूर्णाङ्क i = 0; i < env.hintmask_size; i++)
          cmd.mask_bytes[i] = env.str_ref[i];
      }
    }
    जइ (!param.drop_hints)
    {
      str_encoder_t  encoder (param.flatStr);
      प्रति (unsigned पूर्णाङ्क i = 0; i < env.hintmask_size; i++)
	encoder.encode_byte (env.str_ref[i]);
    }
  }

  गुप्त:
  प्रकारनिर्देश cff2_cs_opset_t<cff2_cs_opset_flatten_t, flatten_param_t, blend_arg_t> SUPER;
  प्रकारनिर्देश cs_opset_t<blend_arg_t, cff2_cs_opset_flatten_t, cff2_cs_opset_flatten_t, cff2_cs_interp_env_t<blend_arg_t>, flatten_param_t> CSOPSET;
};

संरचना cff2_cs_opset_subr_subset_t : cff2_cs_opset_t<cff2_cs_opset_subr_subset_t, subr_subset_param_t, blend_arg_t>
{
  स्थिर शून्य process_op (op_code_t op, cff2_cs_interp_env_t<blend_arg_t> &env, subr_subset_param_t& param)
  {
    विकल्प (op) {

      पक्ष OpCode_return:
	param.current_parsed_str->flush_coalesced (env.str_ref);
	param.current_parsed_str->set_parsed ();
	env.return_from_subr ();
	param.set_current_str (env, मिच्छ);
	भङ्ग;

      पक्ष OpCode_endchar:
	param.current_parsed_str->flush_coalesced (env.str_ref);
	param.current_parsed_str->set_parsed ();
	SUPER::process_op (op, env, param);
	भङ्ग;

      पक्ष OpCode_callsubr:
	process_call_subr (op, CSType_LocalSubr, env, param, env.localSubrs, param.local_closure);
	भङ्ग;

      पक्ष OpCode_callgsubr:
	process_call_subr (op, CSType_GlobalSubr, env, param, env.globalSubrs, param.global_closure);
	भङ्ग;

      स्वभाव:
	SUPER::process_op (op, env, param);
	param.current_parsed_str->add_op (op, env.str_ref);
	भङ्ग;
    }
  }

  रक्षित:
  स्थिर शून्य process_call_subr (op_code_t op, cs_type_t type,
				 cff2_cs_interp_env_t<blend_arg_t> &env, subr_subset_param_t& param,
				 cff2_biased_subrs_t& subrs, hb_set_t *closure)
  {
    byte_str_ref_t    str_ref = env.str_ref;
    env.call_subr (subrs, type);
    param.current_parsed_str->add_call_op (op, str_ref, env.context.subr_अङ्कः_2);
    closure->add (env.context.subr_अङ्कः_2);
    param.set_current_str (env, सच्च);
  }

  गुप्त:
  प्रकारनिर्देश cff2_cs_opset_t<cff2_cs_opset_subr_subset_t, subr_subset_param_t, blend_arg_t> SUPER;
};

संरचना cff2_subr_subsetter_t : subr_subsetter_t<cff2_subr_subsetter_t, CFF2Subrs, ध्रुव OT::cff2::accelerator_subset_t, cff2_cs_interp_env_t<blend_arg_t>, cff2_cs_opset_subr_subset_t>
{
  cff2_subr_subsetter_t (ध्रुव OT::cff2::accelerator_subset_t &acc_, ध्रुव hb_subset_plan_t *plan_)
    : subr_subsetter_t (acc_, plan_) {}

  स्थिर शून्य complete_parsed_str (cff2_cs_interp_env_t<blend_arg_t> &env, subr_subset_param_t& param, parsed_cs_str_t &charstring)
  {
    /* vsindex is inserted at the beginning of the charstring as necessary */
    जइ (env.seen_vsindex ())
    {
      सङ्ख्या_t  ivs;
      ivs.set_int ((पूर्णाङ्क)env.get_ivs ());
      charstring.set_prefix (ivs, OpCode_vsindexcs);
    }
  }
};

संरचना cff2_private_blend_encoder_param_t
{
  cff2_private_blend_encoder_param_t (hb_serialize_context_t *c,
				      ध्रुव CFF2ItemVariationStore *varStore,
				      hb_array_t<पूर्णाङ्क> normalized_coords) :
    c (c), varStore (varStore), normalized_coords (normalized_coords) {}

  शून्य init () {}

  शून्य process_blend ()
  {
    जइ (!seen_blend)
    {
      region_गणनम्_2 = varStore->varStore.get_region_index_गणनम् (ivs);
      जइ (unlikely (!scalars.resize_exact (region_गणनम्_2)))
      {
	जइ (c) c->err (HB_SERIALIZE_ERROR_OTHER);
	seen_blend = सच्च;
	प्रत्यागम;
      }
      varStore->varStore.get_region_scalars (ivs, normalized_coords.arrayZ, normalized_coords.length,
					     &scalars[0], region_गणनम्_2);
      seen_blend = सच्च;
    }
  }

  द्विगुण blend_deltas (hb_array_t<ध्रुव सङ्ख्या_t> deltas) ध्रुव
  {
    द्विगुण v = 0;
    जइ (likely (scalars.length == deltas.length))
    {
      unsigned गणनम् = scalars.length;
      प्रति (unsigned i = 0; i < गणनम्; i++)
	v += (द्विगुण) scalars.arrayZ[i] * deltas.arrayZ[i].to_real ();
    }
    प्रत्यागम v;
  }


  hb_serialize_context_t *c = nullptr;
  तार्किक seen_blend = मिच्छ;
  unsigned ivs = 0;
  unsigned region_गणनम्_2 = 0;
  hb_vector_t<भिन्न> scalars;
  ध्रुव	 CFF2ItemVariationStore *varStore = nullptr;
  hb_array_t<पूर्णाङ्क> normalized_coords;

  /* CFF2 partial instancing: when set, blends are rewritten against the
   * instanced variation store instead of being flattened. */
  ध्रुव cff2_instancing_plan_t *instancer = nullptr;
  /* Error-compensated fold rounding state, per dict operator. */
  द्विगुण fold_exact = 0.;
  द्विगुण fold_emitted = 0.;
};

संरचना cff2_private_dict_blend_opset_t : dict_opset_t
{
  स्थिर शून्य process_arg_blend (cff2_private_blend_encoder_param_t& param,
				 सङ्ख्या_t &arg,
				 ध्रुव hb_array_t<ध्रुव सङ्ख्या_t> blends,
				 unsigned n, unsigned i)
  {
    arg.set_int (round (arg.to_real () + param.blend_deltas (blends)));
  }

  /* The fold added to a value, error-compensated across the operator: most
   * blended private-dict values (BlueValues, StemSnap*, ...) are
   * delta-encoded, so independently rounded folds would accumulate in the
   * decoded absolutes; keeping the emitted cumulative fold equal to the
   * rounded exact cumulative fold bounds every absolute within ±0.5. */
  स्थिर द्विगुण fold (cff2_private_blend_encoder_param_t& param,
		      ध्रुव hb_array_t<ध्रुव सङ्ख्या_t> blends)
  {
    द्विगुण f = param.blend_deltas (blends);
    द्विगुण emit = round (param.fold_exact + f) - param.fold_emitted;
    param.fold_exact += f;
    param.fold_emitted += emit;
    प्रत्यागम emit;
  }

  /* Partial instancing: emit the args preceding this blend group, then the
   * group itself rewritten against the instanced store, and clear the
   * stack.  The op's remaining args are flushed at process_op time. */
  स्थिर शून्य rewrite_blends (cff2_priv_dict_interp_env_t& env,
			      cff2_private_blend_encoder_param_t& param,
			      unsigned start, unsigned n, unsigned k)
  {
    ध्रुव cff2_instancing_plan_t &instancer = *param.instancer;
    ध्रुव cff2_instancing_plan_t::vardata_transform_t *t = instancer.get_transform (param.ivs);
    जइ (unlikely (!t || t->gains.length != k))
    {
      env.set_error ();
      प्रत्यागम;
    }

    str_buff_t str;
    str_encoder_t encoder (str);

    प्रति (unsigned idx = 0; idx < start; idx++)
      encoder.encode_अङ्कः_tp (env.arg_राशिः[idx]);

    unsigned k_new = t->alive () ? t->matrix.length : 0;
    जइ (!k_new)
    {
      /* This VarData died (or is unused by any retained charstring, which
       * can happen for FDs kept only for retain-gids holes); blends
       * dissolve into statics. */
      प्रति (unsigned j = 0; j < n; j++)
      {
	ध्रुव hb_array_t<ध्रुव सङ्ख्या_t> blends = env.arg_राशिः.sub_array (start + n + (j * k), k);
	सङ्ख्या_t v;
	v.set_real (env.arg_राशिः[start + j].to_real () + fold (param, blends));
	encoder.encode_अङ्कः_tp (v);
      }
    }
    अण्णहा
    {
      unsigned done = 0;
      जाव (done < n)
      {
	unsigned avail = 512 > (start + done) ? 512 - (start + done) : 0;
	unsigned chunk = hb_min (avail / (k_new + 1), n - done);
	जइ (unlikely (!chunk))
	{
	  env.set_error ();
	  प्रत्यागम;
	}

	प्रति (unsigned j = done; j < done + chunk; j++)
	{
	  ध्रुव hb_array_t<ध्रुव सङ्ख्या_t> blends = env.arg_राशिः.sub_array (start + n + (j * k), k);
	  सङ्ख्या_t v;
	  v.set_real (env.arg_राशिः[start + j].to_real () + fold (param, blends));
	  encoder.encode_अङ्कः_tp (v);
	}
	प्रति (unsigned j = done; j < done + chunk; j++)
	{
	  ध्रुव hb_array_t<ध्रुव सङ्ख्या_t> blends = env.arg_राशिः.sub_array (start + n + (j * k), k);
	  प्रति (unsigned r = 0; r < k_new; r++)
	  {
	    सङ्ख्या_t v;
	    v.set_int (hb_clamp_to<पूर्णाङ्क> (round (t->transform_delta (r, blends))));
	    encoder.encode_अङ्कः_tp (v);
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

  स्थिर शून्य process_blend (cff2_priv_dict_interp_env_t& env, cff2_private_blend_encoder_param_t& param)
  {
    unsigned पूर्णाङ्क n, k;

    param.process_blend ();
    k = param.region_गणनम्_2;
    n = env.arg_राशिः.निष्कासय_uint ();

    unsigned गणनम् = env.arg_राशिः.get_गणनम् ();
    unsigned पूर्णाङ्क total;
    जइ (unlikely (hb_unsigned_mul_overflows (k + 1, n, &total) || total > गणनम्))
    {
      env.set_error ();
      प्रत्यागम;
    }

    /* copy the blend values into blend array of the default values */
    unsigned पूर्णाङ्क start = गणनम् - total;
    /* let an obvious error case fail, but note CFF2 spec doesn't forbid n==0 */
    जइ (unlikely (start > गणनम्))
    {
      env.set_error ();
      प्रत्यागम;
    }

    जइ (param.instancer && k)
    {
      rewrite_blends (env, param, start, n, k);
      प्रत्यागम;
    }

    प्रति (unsigned पूर्णाङ्क i = 0; i < n; i++)
    {
      ध्रुव hb_array_t<ध्रुव सङ्ख्या_t> blends = env.arg_राशिः.sub_array (start + n + (i * k), k);
      process_arg_blend (param, env.arg_राशिः[start + i], blends, n, i);
    }

    /* pop off blend values leaving default values now adorned with blend values */
    env.arg_राशिः.निष्कासय (k * n);
  }

  स्थिर शून्य process_op (op_code_t op, cff2_priv_dict_interp_env_t& env, cff2_private_blend_encoder_param_t& param)
  {
    विकल्प (op) {
      पक्ष OpCode_StdHW:
      पक्ष OpCode_StdVW:
      पक्ष OpCode_BlueScale:
      पक्ष OpCode_BlueShift:
      पक्ष OpCode_BlueFuzz:
      पक्ष OpCode_ExpansionFactor:
      पक्ष OpCode_LanguageGroup:
      पक्ष Op_Code_Blue_मानानि:
      पक्ष OpCode_OtherBlues:
      पक्ष OpCode_FamilyBlues:
      पक्ष OpCode_FamilyOtherBlues:
      पक्ष OpCode_StemSnapH:
      पक्ष OpCode_StemSnapV:
	भङ्ग;
      पक्ष OpCode_vsindexdict:
	env.process_vsindex ();
	param.ivs = env.get_ivs ();
	जइ (param.instancer)
	{
	  /* Re-emit with the remapped vsindex; 0 is the dict default. */
	  unsigned new_ivs = param.instancer->remap_ivs (param.ivs);
	  जइ (new_ivs)
	  {
	    str_buff_t str;
	    str_encoder_t encoder (str);
	    encoder.encode_int ((पूर्णाङ्क) new_ivs);
	    encoder.encode_op (OpCode_vsindexdict);
	    auto bytes = str.as_bytes ();
	    param.c->embed (&bytes, bytes.length);
	  }
	}
	env.clear_args ();
	प्रत्यागम;
      पक्ष OpCode_blenddict:
	process_blend (env, param);
	प्रत्यागम;

      स्वभाव:
	dict_opset_t::process_op (op, env);
	जइ (!env.arg_राशिः.is_empty ()) प्रत्यागम;
	भङ्ग;
    }

    जइ (unlikely (env.in_error ())) प्रत्यागम;

    // Write args then op

    str_buff_t str;
    str_encoder_t encoder (str);

    unsigned गणनम् = env.arg_राशिः.get_गणनम् ();
    प्रति (unsigned i = 0; i < गणनम्; i++)
      encoder.encode_अङ्कः_tp (env.arg_राशिः[i]);

    encoder.encode_op (op);

    auto bytes = str.as_bytes ();
    param.c->embed (&bytes, bytes.length);

    env.clear_args ();
    param.fold_exact = param.fold_emitted = 0.;
  }
};

संरचना cff2_private_dict_op_serializer_t : op_serializer_t
{
  cff2_private_dict_op_serializer_t (तार्किक desubroutinize_, तार्किक drop_hints_, तार्किक pinned_,
				     ध्रुव CFF::CFF2ItemVariationStore* varStore_,
				     hb_array_t<पूर्णाङ्क> normalized_coords_,
				     ध्रुव cff2_instancing_plan_t *instancer_ = nullptr)
    : desubroutinize (desubroutinize_), drop_hints (drop_hints_), pinned (pinned_),
      param (nullptr, varStore_, normalized_coords_)
  { param.instancer = instancer_; }

  तार्किक serialize (hb_serialize_context_t *c,
		  ध्रुव op_str_t &opstr,
		  objidx_t subrs_link) ध्रुव
  {
    TRACE_SERIALIZE (एतत्);

    जइ (drop_hints && dict_opset_t::is_hint_op (opstr.op))
      return_trace (सच्च);

    जइ (opstr.op == OpCode_Subrs)
    {
      जइ (desubroutinize || !subrs_link)
	return_trace (सच्च);
      अण्णहा
	return_trace (FontDict::serialize_link2_op (c, opstr.op, subrs_link));
    }

    जइ (pinned || param.instancer)
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

  रक्षित:
  ध्रुव तार्किक desubroutinize;
  ध्रुव तार्किक drop_hints;
  ध्रुव तार्किक pinned;
  mutable cff2_private_blend_encoder_param_t param;
};



नामाकाश OT {
संरचना cff2_subset_plan
{
  तार्किक create (ध्रुव OT::cff2::accelerator_subset_t &acc,
	      hb_subset_plan_t *plan)
  {
    /* make sure notdef is first */
    hb_codepoint_t old_glyph;
    जइ (!plan->old_gid_for_new_gid (0, &old_glyph) || (old_glyph != 0)) प्रत्यागम मिच्छ;

    अङ्कः_glyphs = plan->अङ्कः_output_glyphs ();
    orig_fdcount = acc.fdArray->गणनम्;

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
    जइ (partial_instancing &&
	unlikely (!instancing.create (acc.varStore, plan)))
      प्रत्यागम मिच्छ;
    axis_tags = &plan->axis_tags;

    /* Enable command capture for CFF2→CFF1 conversion (for specialization) */
    capture_commands = pinned;

 #ifdef HB_EXPERIMENTAL_API
    min_charstrings_off_size = (plan->flags & HB_SUBSET_FLAGS_IFTB_REQUIREMENTS) ? 4 : 0;
 #अण्णहा
    min_charstrings_off_size = 0;
 #endif

    जइ (desubroutinize)
    {
      जइ (partial_instancing)
      {
	/* Pass 1: collect which VarDatas the retained blends reference, so
	 * unused ones get pruned and the survivors renumbered. */
	hb_set_t used_fds;
	प्रति (unsigned पूर्णाङ्क i = 0; i < अङ्कः_glyphs; i++)
	{
	  hb_codepoint_t glyph;
	  जइ (!plan->old_gid_for_new_gid (i, &glyph))
	    अनुवर्त;
	  ध्रुव hb_ubytes_t str = (*acc.charStrings)[glyph];
	  unsigned पूर्णाङ्क fd = CFF::subset_get_fd (acc, glyph);
	  जइ (unlikely (fd >= acc.fd_गणनम्))
	    प्रत्यागम मिच्छ;
	  used_fds.add (fd);

	  cff2_cs_interp_env_t<blend_arg_t> env (str, acc, fd);
	  cs_interpreter_t<cff2_cs_interp_env_t<blend_arg_t>, cff2_cs_opset_usage_t, cff2_usage_param_t> interp (env);
	  cff2_usage_param_t param;
	  param.instancing = &instancing;
	  param.drop_hints = drop_hints;
	  जइ (unlikely (!interp.interpret (param)))
	    प्रत्यागम मिच्छ;
	}
	प्रति (unsigned fd : used_fds)
	{
	  cff2_usage_param_t param;
	  param.instancing = &instancing;
	  unsigned गणनम् = acc.privateDicts[fd].get_गणनम् ();
	  प्रति (unsigned j = 0; j < गणनम्; j++)
	  {
	    ध्रुव op_str_t &opstr = acc.privateDicts[fd][j];
	    जइ (drop_hints && dict_opset_t::is_hint_op (opstr.op))
	      अनुवर्त;
	    cff2_priv_dict_interp_env_t env {hb_ubytes_t (opstr.ptr, opstr.length)};
	    dict_interpreter_t<cff2_private_dict_usage_opset_t, cff2_usage_param_t, cff2_priv_dict_interp_env_t> interp (env);
	    जइ (unlikely (!interp.interpret (param)))
	      प्रत्यागम मिच्छ;
	  }
	}
	जइ (unlikely (!instancing.finalize ()))
	  प्रत्यागम मिच्छ;

	/* Pass 2: like subr_flattener_t, but blends stay symbolic (no coords
	 * fed to the env) and get rewritten against the instanced store. */
	जइ (!subset_charstrings.resize_exact (अङ्कः_glyphs))
	  प्रत्यागम मिच्छ;
	प्रति (unsigned पूर्णाङ्क i = 0; i < अङ्कः_glyphs; i++)
	{
	  hb_codepoint_t glyph;
	  जइ (!plan->old_gid_for_new_gid (i, &glyph))
	    अनुवर्त;
	  ध्रुव hb_ubytes_t str = (*acc.charStrings)[glyph];
	  unsigned पूर्णाङ्क fd = CFF::subset_get_fd (acc, glyph);
	  जइ (unlikely (fd >= acc.fd_गणनम्))
	    प्रत्यागम मिच्छ;

	  cff2_cs_interp_env_t<blend_arg_t> env (str, acc, fd);
	  cs_interpreter_t<cff2_cs_interp_env_t<blend_arg_t>, cff2_cs_opset_flatten_t, flatten_param_t> interp (env);
	  flatten_param_t param = {
	    subset_charstrings.arrayZ[i],
	    (तार्किक) (plan->flags & HB_SUBSET_FLAGS_NO_HINTING),
	    plan
	  };
	  param.instancer = &instancing;
	  जइ (unlikely (!interp.interpret (param)))
	    प्रत्यागम मिच्छ;
	}
      }
      अण्णहा
      {
      /* Flatten global & local subrs */
      subr_flattener_t<ध्रुव OT::cff2::accelerator_subset_t, cff2_cs_interp_env_t<blend_arg_t>, cff2_cs_opset_flatten_t>
		    flattener(acc, plan);

      /* Enable command capture if requested (for specialization) */
      जइ (capture_commands)
      {
	जइ (!charstring_commands.resize_exact (अङ्कः_glyphs))
	  प्रत्यागम मिच्छ;

	जइ (!flattener.flatten (subset_charstrings, &charstring_commands))
	  प्रत्यागम मिच्छ;
      }
      अण्णहा
      {
	जइ (!flattener.flatten (subset_charstrings))
	  प्रत्यागम मिच्छ;
      }
      }
    }
    अण्णहा
    {
      cff2_subr_subsetter_t	subr_subsetter (acc, plan);

      /* Subset subrs: collect used subroutines, leaving all unused ones behind */
      जइ (!subr_subsetter.subset ())
	प्रत्यागम मिच्छ;

      /* encode charstrings, global subrs, local subrs with new subroutine numbers */
      जइ (!subr_subsetter.encode_charstrings (subset_charstrings, !pinned))
	प्रत्यागम मिच्छ;

      जइ (!subr_subsetter.encode_globalsubrs (subset_globalsubrs))
	प्रत्यागम मिच्छ;

      /* local subrs */
      जइ (!subset_localsubrs.resize (orig_fdcount))
	प्रत्यागम मिच्छ;
      प्रति (unsigned पूर्णाङ्क fd = 0; fd < orig_fdcount; fd++)
      {
	subset_localsubrs[fd].init ();
	जइ (!subr_subsetter.encode_localsubrs (fd, subset_localsubrs[fd]))
	  प्रत्यागम मिच्छ;
      }
    }

    /* FDSelect */
    जइ (acc.fdSelect != &Null (CFF2FDSelect))
    {
      जइ (unlikely (!hb_plan_subset_cff_fdselect (plan,
						  orig_fdcount,
						  *(ध्रुव FDSelect *)acc.fdSelect,
						  acc.अङ्कः_glyphs,
						  subset_fdcount,
						  subset_fdselect_size,
						  subset_fdselect_format,
						  subset_fdselect_ranges,
						  fdmap)))
	प्रत्यागम मिच्छ;
    }
    अण्णहा
      fdmap.identity (1);

    प्रत्यागम सच्च;
  }

  cff2_sub_table_info_t info;

  unsigned पूर्णाङ्क    अङ्कः_glyphs;
  unsigned पूर्णाङ्क    orig_fdcount = 0;
  unsigned पूर्णाङ्क    subset_fdcount = 1;
  unsigned पूर्णाङ्क    subset_fdselect_size = 0;
  unsigned पूर्णाङ्क    subset_fdselect_format = 0;
  तार्किक            pinned = मिच्छ;
  hb_vector_t<code_pair_t>   subset_fdselect_ranges;

  hb_inc_bimap_t   fdmap;

  str_buff_vec_t	    subset_charstrings;
  str_buff_vec_t	    subset_globalsubrs;
  hb_vector_t<str_buff_vec_t> subset_localsubrs;

  तार्किक	    drop_hints = मिच्छ;
  तार्किक	    desubroutinize = मिच्छ;

  तार्किक	    partial_instancing = मिच्छ;
  cff2_instancing_plan_t instancing;
  ध्रुव hb_vector_t<hb_tag_t> *axis_tags = nullptr;

  unsigned  min_charstrings_off_size = 0;

  hb_array_t<पूर्णाङ्क> normalized_coords; // For instantiation
  head_maxp_info_t head_maxp_info;  // For FontBBox
  ध्रुव hb_hashmap_t<hb_codepoint_t, hb_pair_t<unsigned, पूर्णाङ्क>> *hmtx_map; // For widths

  // Width optimization results (for CFF1 conversion)
  unsigned default_width = 0;
  unsigned nominal_width = 0;

  // Command capture for specialization (CFF2→CFF1 conversion)
  तार्किक capture_commands = मिच्छ;
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
                             unsigned default_width,
                             unsigned nominal_width)
{
  c->push ();

  // CFF1 requires:
  // 1. Width at the beginning (if != defaultWidthX)
  // 2. endchar at the end
  str_buff_vec_t cff1_charstrings;
  जइ (unlikely (!cff1_charstrings.resize (plan.subset_charstrings.length)))
  {
    c->निष्कासय_discard ();
    प्रत्यागम मिच्छ;
  }

  प्रति (unsigned i = 0; i < plan.subset_charstrings.length; i++)
  {
    // Get width for this glyph from hmtx_map
    unsigned width = 0;
    जइ (plan.hmtx_map->has (i))
      width = plan.hmtx_map->get (i).first;

    // Encode width if different from default
    str_encoder_t encoder (cff1_charstrings[i]);
    जइ (width != default_width)
    {
      पूर्णाङ्क delta = (पूर्णाङ्क) width - (पूर्णाङ्क) nominal_width;
      encoder.encode_int (delta);
    }

    // Use specialized commands if available, otherwise use binary
    जइ (plan.capture_commands && i < plan.charstring_commands.length &&
        plan.charstring_commands[i].length > 0)
    {
      // Specialize and encode commands
      auto &commands = plan.charstring_commands[i];
      CFF::specialize_commands (commands, 48);  /* maxstack=48 for CFF1 */
      जइ (unlikely (!CFF::encode_commands (commands, cff1_charstrings[i])))
      {
        c->निष्कासय_discard ();
        प्रत्यागम मिच्छ;
      }
    }
    अण्णहा
    {
      // Use binary CharString
      ध्रुव str_buff_t &cs = plan.subset_charstrings[i];
      प्रति (unsigned j = 0; j < cs.length; j++)
        cff1_charstrings[i].push (cs[j]);
    }

    // Check if it already ends with endchar (0x0e) or return (0x0b)
    जइ (cff1_charstrings[i].length == 0 ||
        (cff1_charstrings[i].tail () != 0x0e && cff1_charstrings[i].tail () != 0x0b))
    {
      // Append endchar operator
      जइ (unlikely (!cff1_charstrings[i].प्रक्षिप_or_fail (0x0e)))
      {
        c->निष्कासय_discard ();
        प्रत्यागम मिच्छ;
      }
    }
  }

  unsigned data_size = 0;
  unsigned total_size = CFF1CharStrings::total_size (cff1_charstrings, &data_size, plan.min_charstrings_off_size);
  जइ (unlikely (!c->start_zerocopy (total_size)))
  {
    c->निष्कासय_discard ();
    प्रत्यागम मिच्छ;
  }

  auto *cs = c->start_embed<CFF1CharStrings> ();
  जइ (unlikely (!cs->serialize (c, cff1_charstrings, &data_size, plan.min_charstrings_off_size)))
  {
    c->निष्कासय_discard ();
    प्रत्यागम मिच्छ;
  }

  plan.info.char_strings_link = c->निष्कासय_pack (मिच्छ);
  प्रत्यागम सच्च;
}

/* Serialize CID Charset (format 2 range: gid 0-N -> cid 0-N) */
स्थिर तार्किक
_serialize_cff1_charset (hb_serialize_context_t *c,
                         unsigned पूर्णाङ्क अङ्कः_glyphs,
                         objidx_t &charset_link)
{
  // For CID fonts, create a simple identity charset
  // Format 2: one range covering all glyphs (except .notdef)
  c->push ();

  auto *charset = c->start_embed<Charset> ();
  जइ (unlikely (!charset))
  {
    c->निष्कासय_discard ();
    प्रत्यागम मिच्छ;
  }

  // Create a single range for CID 1 to num_glyphs-1
  hb_vector_t<code_pair_t> ranges;
  जइ (अङ्कः_glyphs > 1)
  {
    code_pair_t range;
    range.code = 1;  // first CID
    range.glyph = अङ्कः_glyphs - 2;  // nLeft (covers glyphs 1 to num_glyphs-1)
    ranges.push (range);
  }

  जइ (unlikely (!charset->serialize (c, 2, अङ्कः_glyphs, ranges)))
  {
    c->निष्कासय_discard ();
    प्रत्यागम मिच्छ;
  }

  charset_link = c->निष्कासय_pack ();
  प्रत्यागम सच्च;
}

/* CFF2 to CFF1 serialization */
नामाकाश CFF {

तार्किक
serialize_cff2_to_cff1 (hb_serialize_context_t *c,
                        OT::cff2_subset_plan &plan,
                        ध्रुव cff2_शिखरम्_dict_मानानि_t &cff2_शिखरम्_Dict,
                        ध्रुव OT::cff2::accelerator_subset_t &acc)
{
  TRACE_SERIALIZE (एतत्);

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
    widths.alloc (plan.अङ्कः_glyphs);

    प्रति (unsigned gid = 0; gid < plan.अङ्कः_glyphs; gid++)
    {
      unsigned width = 0;
      जइ (plan.hmtx_map->has (gid))
        width = plan.hmtx_map->get (gid).first;
      widths.push (width);
    }

    // Optimize defaultWidthX and nominalWidthX
    CFF::optimize_widths (widths, plan.default_width, plan.nominal_width);
  }

  // 1. CharStrings (with widths prepended)
  जइ (!_serialize_cff1_charstrings (c, plan, plan.default_width, plan.nominal_width))
    return_trace (मिच्छ);

  // 2. Private DICs & Local Subrs (same as CFF2)
  hb_vector_t<table_info_t> private_dict_infos;
  जइ (unlikely (!private_dict_infos.resize (plan.subset_fdcount)))
    return_trace (मिच्छ);

  प्रति (पूर्णाङ्क i = (पूर्णाङ्क)acc.privateDicts.length; --i >= 0;)
  {
    जइ (plan.fdmap.has (i))
    {
      objidx_t subrs_link = 0;

      जइ (plan.subset_localsubrs[i].length > 0)
      {
        auto *dest = c->push<CFF1Subrs> ();
        जइ (likely (dest->serialize (c, plan.subset_localsubrs[i])))
          subrs_link = c->निष्कासय_pack (मिच्छ);
        अण्णहा
        {
          c->निष्कासय_discard ();
          return_trace (मिच्छ);
        }
      }

      auto *pd = c->push<PrivateDict> ();
      // Use the CFF2 Private DICT serializer which instantiates blends when pinned=true
      cff2_private_dict_op_serializer_t privSzr (plan.desubroutinize, plan.drop_hints, plan.pinned,
                                                  acc.varStore, plan.normalized_coords);
      जइ (likely (pd->serialize (c, acc.privateDicts[i], privSzr, subrs_link)))
      {
        // Add defaultWidthX and nominalWidthX for CFF1
        str_buff_t width_ops;
        str_encoder_t encoder (width_ops);
        encoder.encode_int (plan.default_width);
        encoder.encode_op (OpCode_defaultWidthX);
        encoder.encode_int (plan.nominal_width);
        encoder.encode_op (OpCode_nominalWidthX);

        जइ (!encoder.in_error () && c->embed (width_ops.as_bytes ().arrayZ, width_ops.length))
        {
          unsigned fd = plan.fdmap[i];
          private_dict_infos[fd].size = c->length ();
          private_dict_infos[fd].link = c->निष्कासय_pack ();
        }
        अण्णहा
        {
          c->निष्कासय_discard ();
          return_trace (मिच्छ);
        }
      }
      अण्णहा
      {
        c->निष्कासय_discard ();
        return_trace (मिच्छ);
      }
    }
  }

  // 3. FDArray - serialize CFF2 font dicts as CFF1
  {
    auto *fda = c->push<FDArray<HBUINT16>> ();
    cff_font_dict_op_serializer_t fontSzr;
    auto it =
    + hb_zip (+ hb_iter (acc.fontDicts)
              | hb_filter ([&] (ध्रुव cff2_font_dict_मानानि_t &_)
                { प्रत्यागम plan.fdmap.has (&_ - &acc.fontDicts[0]); }),
              hb_iter (private_dict_infos))
    ;
    // Explicitly specify template parameters: DICTVAL, INFO
    तार्किक success = fda->serialize<cff2_font_dict_मानानि_t, table_info_t> (c, it, fontSzr);
    जइ (success)
      plan.info.fd_array_link = c->निष्कासय_pack (मिच्छ);
    अण्णहा
    {
      c->निष्कासय_discard ();
      return_trace (मिच्छ);
    }
  }

  // 4. FDSelect (required in CFF1 CID-keyed fonts)
  // CFF1 requires FDSelect for all CID-keyed fonts, even with just one FD
  // CFF2 makes it optional when there's only one FD
  जइ (acc.fdSelect != &Null (CFF2FDSelect))
  {
    c->push ();
    जइ (likely (hb_serialize_cff_fdselect (c, plan.अङ्कः_glyphs,
                                          *(ध्रुव FDSelect *)acc.fdSelect,
                                          plan.orig_fdcount,
                                          plan.subset_fdselect_format,
                                          plan.subset_fdselect_size,
                                          plan.subset_fdselect_ranges)))
      plan.info.fd_select.link = c->निष्कासय_pack ();
    अण्णहा
    {
      c->निष्कासय_discard ();
      return_trace (मिच्छ);
    }
  }
  अण्णहा
  {
    // Create a range-based FDSelect3 mapping all glyphs to FD 0
    // Format: format(1) + nRanges(2) + range(3) + sentinel(2) = 8 bytes
    c->push ();

    // Format byte
    HBUINT8 format;
    format = 3;
    जइ (unlikely (!c->embed (format)))
    {
      c->निष्कासय_discard ();
      return_trace (मिच्छ);
    }

    // nRanges
    HBUINT16 nRanges;
    nRanges = 1;
    जइ (unlikely (!c->embed (nRanges)))
    {
      c->निष्कासय_discard ();
      return_trace (मिच्छ);
    }

    // Single range: {first: 0, fd: 0}
    FDSelect3_Range range;
    range.first = 0;
    range.fd = 0;
    जइ (unlikely (!c->embed (range)))
    {
      c->निष्कासय_discard ();
      return_trace (मिच्छ);
    }

    // Sentinel (number of glyphs)
    HBUINT16 sentinel;
    sentinel = plan.अङ्कः_glyphs;
    जइ (unlikely (!c->embed (sentinel)))
    {
      c->निष्कासय_discard ();
      return_trace (मिच्छ);
    }

    plan.info.fd_select.link = c->निष्कासय_pack ();
  }

  // 5. Charset (CID charset for identity mapping)
  objidx_t charset_link;
  जइ (!_serialize_cff1_charset (c, plan.अङ्कः_glyphs, charset_link))
    return_trace (मिच्छ);

  // 6. Global Subrs
  {
    auto *dest = c->push<CFF1Subrs> ();
    जइ (likely (dest->serialize (c, plan.subset_globalsubrs)))
      c->निष्कासय_pack (मिच्छ);
    अण्णहा
    {
      c->निष्कासय_discard ();
      return_trace (मिच्छ);
    }
  }

  // 7. String INDEX - Add "Adobe" and "Identity" for ROS operator
  {
    ध्रुव वर्ण *adobe_str = "Adobe";
    ध्रुव वर्ण *identity_str = "Identity";
    unsigned adobe_len = 5;  // strlen("Adobe")
    unsigned identity_len = 8;  // strlen("Identity")

    // Build strings array
    hb_vector_t<hb_ubytes_t> strings;
    strings.alloc (2);
    strings.push (hb_ubytes_t ((ध्रुव unsigned वर्ण *) adobe_str, adobe_len));
    strings.push (hb_ubytes_t ((ध्रुव unsigned वर्ण *) identity_str, identity_len));

    // Serialize as CFF INDEX
    auto *dest = c->push<CFF1Index> ();
    जइ (likely (dest->serialize (c, strings)))
      c->निष्कासय_pack (मिच्छ);
    अण्णहा
    {
      c->निष्कासय_discard ();
      return_trace (मिच्छ);
    }
  }

  // 8. CFF Header
  OT::cff1 *cff = c->allocate_min<OT::cff1> ();
  जइ (unlikely (!cff)) return_trace (मिच्छ);

  /* header */
  cff->version.major = 0x01;
  cff->version.minor = 0x00;
  cff->nameIndex = cff->min_size;
  cff->offSize = 4; /* unused? */

  // 9. Name INDEX (single entry)
  {
    unsigned name_len = strlen (CFF1_DEFAULT_FONT_NAME);

    CFF1Index *idx = c->start_embed<CFF1Index> ();
    जइ (unlikely (!idx)) return_trace (मिच्छ);

    जइ (unlikely (!idx->serialize_header (c, hb_iter (&name_len, 1), name_len)))
      return_trace (मिच्छ);

    जइ (unlikely (!c->embed (CFF1_DEFAULT_FONT_NAME, name_len)))
      return_trace (मिच्छ);
  }

  // 10. Top DICT INDEX
  {
    // Serialize the Top DICT data first
    c->push<शिखरम्_Dict> ();
    cff1_from_cff2_शिखरम्_dict_op_serializer_t शिखरम्_Szr;

    // Serialize ROS first
    जइ (unlikely (!शिखरम्_Szr.serialize_ros (c)))
    {
      c->निष्कासय_discard ();
      return_trace (मिच्छ);
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

      जइ (encoder.in_error () || !c->embed (bbox_buff.as_bytes ().arrayZ, bbox_buff.length))
      {
        c->निष्कासय_discard ();
        return_trace (मिच्छ);
      }
    }

    // Serialize charset operator
    जइ (charset_link && unlikely (!FontDict::serialize_link4_op (c, OpCode_charset, charset_link, whence_t::Absolute)))
    {
      c->निष्कासय_discard ();
      return_trace (मिच्छ);
    }

    // Serialize FDSelect operator (required for CID-keyed CFF1 fonts)
    जइ (plan.info.fd_select.link && unlikely (!FontDict::serialize_link4_op (c, OpCode_FDSelect, plan.info.fd_select.link, whence_t::Absolute)))
    {
      c->निष्कासय_discard ();
      return_trace (मिच्छ);
    }

    // Serialize FDArray operator (required for CID-keyed CFF1 fonts)
    जइ (plan.info.fd_array_link && unlikely (!FontDict::serialize_link4_op (c, OpCode_FDArray, plan.info.fd_array_link, whence_t::Absolute)))
    {
      c->निष्कासय_discard ();
      return_trace (मिच्छ);
    }

    // Serialize other operators from CFF2 TopDict
    प्रति (ध्रुव auto &opstr : cff2_शिखरम्_Dict.values)
    {
      जइ (unlikely (!शिखरम्_Szr.serialize (c, opstr, plan.info)))
      {
        c->निष्कासय_discard ();
        return_trace (मिच्छ);
      }
    }

    unsigned शिखरम्_size = c->length ();
    c->निष्कासय_pack (मिच्छ);

    // Serialize INDEX header
    auto *dest = c->start_embed<CFF1Index> ();
    जइ (unlikely (!dest->serialize_header (c, hb_iter (&शिखरम्_size, 1), शिखरम्_size)))
      return_trace (मिच्छ);
  }

  return_trace (सच्च);
}

} /* namespace CFF */

स्थिर तार्किक _serialize_cff2_charstrings (hb_serialize_context_t *c,
			     cff2_subset_plan &plan,
			     ध्रुव OT::cff2::accelerator_subset_t  &acc)
{
  c->push ();

  unsigned data_size = 0;
  unsigned total_size = CFF2CharStrings::total_size (plan.subset_charstrings, &data_size, plan.min_charstrings_off_size);
  जइ (unlikely (!c->start_zerocopy (total_size)))
    प्रत्यागम मिच्छ;

  auto *cs = c->start_embed<CFF2CharStrings> ();
  जइ (unlikely (!cs->serialize (c, plan.subset_charstrings, &data_size, plan.min_charstrings_off_size)))
  {
    c->निष्कासय_discard ();
    प्रत्यागम मिच्छ;
  }

  plan.info.char_strings_link = c->निष्कासय_pack (मिच्छ);
  प्रत्यागम सच्च;
}

तार्किक
OT::cff2::accelerator_subset_t::serialize (hb_serialize_context_t *c,
					   संरचना cff2_subset_plan &plan,
					   hb_array_t<पूर्णाङ्क> normalized_coords) ध्रुव
{
  /* push charstrings onto the object stack first which will ensure it packs as the last
     object in the table. Keeping the chastrings last satisfies the requirements for patching
     via IFTB. If this ordering needs to be changed in the future, charstrings should be left
     at the end whenever HB_SUBSET_FLAGS_ITFB_REQUIREMENTS is enabled. */
  जइ (!_serialize_cff2_charstrings(c, plan, *एतत्))
    प्रत्यागम मिच्छ;

  /* private dicts & local subrs */
  hb_vector_t<table_info_t>  private_dict_infos;
  जइ (unlikely (!private_dict_infos.resize (plan.subset_fdcount))) प्रत्यागम मिच्छ;

  प्रति (पूर्णाङ्क i = (पूर्णाङ्क)privateDicts.length; --i >= 0 ;)
  {
    जइ (plan.fdmap.has (i))
    {
      objidx_t	subrs_link = 0;

      जइ (plan.subset_localsubrs[i].length > 0)
      {
	auto *dest = c->push <CFF2Subrs> ();
	जइ (likely (dest->serialize (c, plan.subset_localsubrs[i])))
	  subrs_link = c->निष्कासय_pack (मिच्छ);
	अण्णहा
	{
	  c->निष्कासय_discard ();
	  प्रत्यागम मिच्छ;
	}
      }
      auto *pd = c->push<PrivateDict> ();
      cff2_private_dict_op_serializer_t privSzr (plan.desubroutinize, plan.drop_hints, plan.pinned,
						 varStore, normalized_coords,
						 plan.partial_instancing ? &plan.instancing : nullptr);
      जइ (likely (pd->serialize (c, privateDicts[i], privSzr, subrs_link)))
      {
	unsigned fd = plan.fdmap[i];
	private_dict_infos[fd].size = c->length ();
	private_dict_infos[fd].link = c->निष्कासय_pack ();
      }
      अण्णहा
      {
	c->निष्कासय_discard ();
	प्रत्यागम मिच्छ;
      }
    }
  }

  /* FDSelect */
  जइ (fdSelect != &Null (CFF2FDSelect))
  {
    c->push ();
    जइ (likely (hb_serialize_cff_fdselect (c, plan.अङ्कः_glyphs, *(ध्रुव FDSelect *)fdSelect,
					   plan.orig_fdcount,
					   plan.subset_fdselect_format, plan.subset_fdselect_size,
					   plan.subset_fdselect_ranges)))
      plan.info.fd_select.link = c->निष्कासय_pack ();
    अण्णहा
    {
      c->निष्कासय_discard ();
      प्रत्यागम मिच्छ;
    }
  }

  /* FDArray (FD Index) */
  {
    auto *fda = c->push<CFF2FDArray> ();
    cff_font_dict_op_serializer_t fontSzr;
    auto it =
    + hb_zip (+ hb_iter (fontDicts)
	      | hb_filter ([&] (ध्रुव cff2_font_dict_मानानि_t &_)
		{ प्रत्यागम plan.fdmap.has (&_ - &fontDicts[0]); }),
	      hb_iter (private_dict_infos))
    ;
    जइ (unlikely (!fda->serialize (c, it, fontSzr)))
    {
      c->निष्कासय_discard ();
      प्रत्यागम मिच्छ;
    }
    plan.info.fd_array_link = c->निष्कासय_pack (मिच्छ);
  }

  /* variation store */
  जइ (varStore != &Null (CFF2ItemVariationStore) &&
      !plan.pinned)
  {
    जइ (plan.partial_instancing)
    {
      जइ (plan.instancing.has_variations ())
      {
	c->push ();
	जइ (unlikely (!plan.instancing.serialize_var_store (c, *plan.axis_tags)))
	{
	  c->निष्कासय_discard ();
	  प्रत्यागम मिच्छ;
	}
	plan.info.var_store_link = c->निष्कासय_pack (मिच्छ);
      }
      /* else: the store emptied; the vstore op gets dropped along with it. */
    }
    अण्णहा
    {
      auto *dest = c->push<CFF2ItemVariationStore> ();
      जइ (unlikely (!dest->serialize (c, varStore)))
      {
	c->निष्कासय_discard ();
	प्रत्यागम मिच्छ;
      }
      plan.info.var_store_link = c->निष्कासय_pack (मिच्छ);
    }
  }

  OT::cff2 *cff2 = c->allocate_min<OT::cff2> ();
  जइ (unlikely (!cff2)) प्रत्यागम मिच्छ;

  /* header */
  cff2->version.major = 0x02;
  cff2->version.minor = 0x00;
  cff2->शिखरम्_Dict_2 = OT::cff2::static_size;

  /* top dict */
  {
    शिखरम्_Dict &dict = cff2 + cff2->शिखरम्_Dict_2;
    cff2_शिखरम्_dict_op_serializer_t शिखरम्_Szr;
    जइ (unlikely (!dict.serialize (c, शिखरम्_Dict_2, शिखरम्_Szr, plan.info))) प्रत्यागम मिच्छ;
    cff2->शिखरम्_Dict_Size = c->head - (ध्रुव वर्ण *)&dict;
  }

  /* global subrs */
  {
    auto *dest = c->start_embed <CFF2Subrs> ();
    प्रत्यागम dest->serialize (c, plan.subset_globalsubrs);
  }
}

तार्किक
OT::cff2::accelerator_subset_t::subset (hb_subset_context_t *c) ध्रुव
{
  cff2_subset_plan cff2_plan;

  जइ (unlikely (!cff2_plan.create (*एतत्, c->plan))) प्रत्यागम मिच्छ;

  // If instantiating (pinned) and downgrade flag is set, convert to CFF1
  जइ (cff2_plan.pinned && (c->plan->flags & HB_SUBSET_FLAGS_DOWNGRADE_CFF2))
  {
    // Serialize CFF1 to the subsetter's serializer
    // If we run out of room, returning true will cause subsetter to retry with larger buffer
    तार्किक result = CFF::serialize_cff2_to_cff1 (c->serializer, cff2_plan, शिखरम्_Dict_2, *एतत्);

    जइ (c->serializer->ran_out_of_room ())
      प्रत्यागम सच्च; // Subsetter will retry with larger buffer

    जइ (result && !c->serializer->in_error ())
    {
      // Success - end serialization to resolve links
      c->serializer->end_serialize ();

      // Copy the serialized CFF1 data and add as CFF table
      hb_blob_t *cff_blob = c->serializer->copy_blob ();
      जइ (cff_blob)
      {
        c->plan->add_table (HB_TAG('C','F','F',' '), cff_blob);
        hb_blob_destroy (cff_blob);

        // Return false to signal CFF2 table is not needed
        प्रत्यागम मिच्छ;
      }
    }

    // Conversion failed - don't fall back, fail hard for debugging
    प्रत्यागम मिच्छ;
  }

  प्रत्यागम serialize (c->serializer, cff2_plan,
		    c->plan->normalized_coords.as_array ());
}

#endif
