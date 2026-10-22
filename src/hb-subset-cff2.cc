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

#অন্তর্ভাব "hb.hh"

#ifndef HB_NO_SUBSET_CFF

#অন্তর্ভাব "hb-open-type.hh"
#অন্তর্ভাব "hb-ot-cff2-table.hh"
#অন্তর্ভাব "hb-ot-var-common.hh"
#অন্তর্ভাব "hb-set.h"
#অন্তর্ভাব "hb-subset-plan.hh"
#অন্তর্ভাব "hb-subset-cff-common.hh"
#অন্তর্ভাব "hb-cff2-interp-cs.hh"
#অন্তর্ভাব "hb-subset-cff2-to-cff1.hh"

using নামাকাশ CFF;

নামাকাশ CFF {

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
কাঠামো cff2_instancing_plan_t
{
  কাঠামো vardata_transform_t
  {
    /* new_deltas[j] = Σ_i matrix[j][i] * old_deltas[i] */
    hb_vector_t<hb_vector_t<দশমিক>> matrix;
    /* Default-value fold-in scalars, one per old region: the old store's
     * region scalars at the new default location. */
    hb_vector_t<দশমিক> gains;
    /* Surviving regions, parallel to matrix rows. */
    hb_vector_t<hb_hashmap_t<hb_tag_t, Triple>> regions;
    /* Index into the new global region list, per new region (finalize). */
    hb_vector_t<unsigned> region_indices;
    unsigned new_major = 0;
    যৌক্তিক alive_ = মিথ্যা;
    যৌক্তিক alive () ধ্রুবক { ফেরত alive_; }

    /* One value's delta for new region j, from its old per-region deltas. */
    দ্বিগুণ transform_delta (unsigned j, hb_array_t<ধ্রুবক সংখ্যা_t> deltas) ধ্রুবক
    {
      ধ্রুবক hb_vector_t<দশমিক> &col = matrix.arrayZ[j];
      দ্বিগুণ d = 0.;
      unsigned গুনতি = hb_min (col.দৈর্ঘ্য, deltas.দৈর্ঘ্য);
      জন্য (unsigned c = 0; c < গুনতি; c++)
	d += (দ্বিগুণ) col.arrayZ[c] * deltas.arrayZ[c].to_real ();
      ফেরত d;
    }
  };

  hb_vector_t<vardata_transform_t> transforms;  /* per old VarData (vsindex) */
  hb_vector_t<hb_hashmap_t<hb_tag_t, Triple>> new_regions;  /* global region list */
  hb_set_t used_majors;
  unsigned new_major_গুনতি = 0;

  যৌক্তিক has_variations () ধ্রুবক { ফেরত new_major_গুনতি; }

  ধ্রুবক vardata_transform_t *get_transform (unsigned major) ধ্রুবক
  { ফেরত major < transforms.দৈর্ঘ্য ? &transforms.arrayZ[major] : nullptr; }

  /* Record that a retained blend references this VarData. */
  শূন্য note_use (unsigned major) { used_majors.add (major); }

  /* The new vsindex a private dict's (or charstring's) old ivs maps to;
   * dead VarDatas fall back to 0, their blends having become static. */
  unsigned remap_ivs (unsigned old_ivs) ধ্রুবক
  {
    ধ্রুবক vardata_transform_t *t = get_transform (old_ivs);
    ফেরত (t && t->alive ()) ? t->new_major : 0;
  }

  দ্বিগুণ fold_deltas (unsigned old_ivs, hb_array_t<ধ্রুবক সংখ্যা_t> deltas) ধ্রুবক
  {
    ধ্রুবক vardata_transform_t *t = get_transform (old_ivs);
    যদি (unlikely (!t || t->gains.দৈর্ঘ্য != deltas.দৈর্ঘ্য)) ফেরত 0.;
    দ্বিগুণ v = 0.;
    জন্য (unsigned i = 0; i < deltas.দৈর্ঘ্য; i++)
      v += (দ্বিগুণ) t->gains.arrayZ[i] * deltas.arrayZ[i].to_real ();
    ফেরত v;
  }

  যৌক্তিক create (ধ্রুবক CFF2ItemVariationStore *cff2VarStore,
	       ধ্রুবক hb_subset_plan_t *plan)
  {
    ধ্রুবক OT::ItemVariationStore &varStore = cff2VarStore->varStore;
    unsigned major_গুনতি = varStore.get_sub_table_গুনতি ();
    যদি (!transforms.resize (major_গুনতি)) ফেরত মিথ্যা;

    যদি (!varStore.get_region_list ().get_var_regions (plan->axes_old_সূচক_tag_map, orig_regions))
      ফেরত মিথ্যা;

    /* Instance each VarData with identity delta rows; the surviving tuples'
     * delta columns are the old-region → new-region transform. */
    OT::optimize_scratch_t scratch;
    জন্য (unsigned m = 0; m < major_গুনতি; m++)
    {
      ধ্রুবক OT::Var_তথ্য &var_তথ্য = varStore.get_sub_table (m);
      unsigned k = var_তথ্য.get_region_সূচক_গুনতি ();
      vardata_transform_t &t = transforms.arrayZ[m];

      যদি (!t.gains.resize_exact (k)) ফেরত মিথ্যা;
      varStore.get_region_scalars (m, plan->normalized_coords.arrayZ,
				   plan->normalized_coords.দৈর্ঘ্য,
				   t.gains.arrayZ, k);

      OT::tuple_variations_t tuples;
      জন্য (unsigned r = 0; r < k; r++)
      {
	OT::tuple_delta_t tuple;
	যদি (!tuple.deltas_x.resize (k) ||
	    !tuple.indices.resize (k))
	  ফেরত মিথ্যা;
	জন্য (unsigned i = 0; i < k; i++)
	  tuple.indices.arrayZ[i] = সত্য;
	tuple.deltas_x.arrayZ[r] = 1.f;
	unsigned region_সূচক = var_তথ্য.get_region_সূচক (r);
	যদি (unlikely (region_সূচক >= orig_regions.দৈর্ঘ্য)) ফেরত মিথ্যা;
	tuple.axis_tuples = orig_regions.arrayZ[region_সূচক];
	যদি (unlikely (tuple.axis_tuples.in_error ())) ফেরত মিথ্যা;
	tuples.tuple_vars.push (std::move (tuple));
      }
      যদি (unlikely (tuples.tuple_vars.in_error ())) ফেরত মিথ্যা;

      যদি (!tuples.instantiate (plan->axes_location, plan->axes_triple_distances, scratch))
	ফেরত মিথ্যা;

      জন্য (auto &tuple : tuples.tuple_vars)
      {
	যদি (unlikely (tuple.deltas_x.দৈর্ঘ্য != k)) ফেরত মিথ্যা;
	hb_vector_t<দশমিক> col;
	যদি (unlikely (!col.resize_exact (k))) ফেরত মিথ্যা;
	জন্য (unsigned i = 0; i < k; i++)
	  col.arrayZ[i] = tuple.deltas_x.arrayZ[i];
	t.matrix.push (std::move (col));
	t.regions.push (std::move (tuple.axis_tuples));
      }
      যদি (unlikely (t.matrix.in_error () || t.regions.in_error ())) ফেরত মিথ্যা;
    }

    ফেরত সত্য;
  }

  /* Decide which VarDatas survive, number them, and build the new global
   * region list: surviving original regions first, in their original order,
   * then solver-split new regions in order of appearance (fontTools'
   * rebuildRegions + prune_regions). */
  যৌক্তিক finalize ()
  {
    জন্য (unsigned m = 0; m < transforms.দৈর্ঘ্য; m++)
    {
      vardata_transform_t &t = transforms.arrayZ[m];
      t.alive_ = t.matrix.দৈর্ঘ্য && used_majors.has (m);
      যদি (t.alive_)
	t.new_major = new_major_গুনতি++;
    }

    /* Keys hash by content, so pointers into different owners compare fine. */
    hb_hashmap_t<ধ্রুবক hb_hashmap_t<hb_tag_t, Triple>*, unsigned> region_idx_map;
    hb_vector_t<ধ্রুবক hb_hashmap_t<hb_tag_t, Triple>*> region_ptrs;
    জন্য (ধ্রুবক vardata_transform_t &t : transforms)
    {
      যদি (!t.alive ()) চালু;
      জন্য (ধ্রুবক auto &region : t.regions)
	যদি (!region_idx_map.has (&region))
	  যদি (unlikely (!region_idx_map.set (&region, (unsigned) -1)))
	    ফেরত মিথ্যা;
    }
    জন্য (ধ্রুবক auto &r : orig_regions)
    {
      unsigned *idx;
      যদি (region_idx_map.has (&r, &idx) && *idx == (unsigned) -1)
      {
	যদি (unlikely (!region_idx_map.set (&r, region_ptrs.দৈর্ঘ্য))) ফেরত মিথ্যা;
	region_ptrs.push (&r);
      }
    }
    জন্য (ধ্রুবক vardata_transform_t &t : transforms)
    {
      যদি (!t.alive ()) চালু;
      জন্য (ধ্রুবক auto &region : t.regions)
      {
	unsigned *idx;
	যদি (region_idx_map.has (&region, &idx) && *idx == (unsigned) -1)
	{
	  যদি (unlikely (!region_idx_map.set (&region, region_ptrs.দৈর্ঘ্য))) ফেরত মিথ্যা;
	  region_ptrs.push (&region);
	}
      }
    }
    যদি (unlikely (region_ptrs.in_error ())) ফেরত মিথ্যা;

    জন্য (vardata_transform_t &t : transforms)
    {
      যদি (!t.alive ()) চালু;
      জন্য (ধ্রুবক auto &region : t.regions)
      {
	unsigned *idx;
	যদি (unlikely (!region_idx_map.has (&region, &idx))) ফেরত মিথ্যা;
	t.region_indices.push (*idx);
      }
      যদি (unlikely (t.region_indices.in_error ())) ফেরত মিথ্যা;
    }

    যদি (!new_regions.resize (region_ptrs.দৈর্ঘ্য)) ফেরত মিথ্যা;
    জন্য (unsigned i = 0; i < region_ptrs.দৈর্ঘ্য; i++)
    {
      new_regions.arrayZ[i] = *region_ptrs.arrayZ[i];
      যদি (unlikely (new_regions.arrayZ[i].in_error ())) ফেরত মিথ্যা;
    }

    ফেরত সত্য;
  }

  /* Serialize the instanced store: a CFF2ItemVariationStore whose VarDatas
   * carry no items, just region indexes (blend deltas live in charstrings).
   * Laid out manually since all sizes are known up front. */
  যৌক্তিক serialize_var_store (hb_serialize_context_t *c,
			    ধ্রুবক hb_vector_t<hb_tag_t> &axis_tags) ধ্রুবক
  {
    unsigned তথ্য_গুনতি = new_major_গুনতি;
    unsigned header_আকার = 2 + 4 + 2 + 4 * তথ্য_গুনতি;

    uint64_t offset = header_আকার;
    hb_vector_t<unsigned> তথ্য_offsets;
    জন্য (ধ্রুবক vardata_transform_t &t : transforms)
    {
      যদি (!t.alive ()) চালু;
      তথ্য_offsets.push ((unsigned) offset);
      offset += 6 + 2 * (uint64_t) t.region_indices.দৈর্ঘ্য;
    }
    যদি (unlikely (তথ্য_offsets.in_error () || তথ্য_offsets.দৈর্ঘ্য != তথ্য_গুনতি))
      ফেরত মিথ্যা;
    unsigned regions_offset = (unsigned) offset;
    uint64_t যোগ = offset + 4 + (uint64_t) new_regions.দৈর্ঘ্য * axis_tags.দৈর্ঘ্য * 6;
    যদি (unlikely (যোগ > 0xFFFFFFFFu)) ফেরত মিথ্যা;

    HBUINT16 *আকার_field = c->allocate_আকার<HBUINT16> (HBUINT16::static_আকার);
    যদি (unlikely (!আকার_field)) ফেরত মিথ্যা;
    /* OFF 2026 5.3.3.11.2 permits larger stores with length=65535. */
    *আকার_field = hb_min (যোগ, (uint64_t) 0xFFFFu);

    অক্ষর *header = c->allocate_আকার<অক্ষর> (header_আকার);
    যদি (unlikely (!header)) ফেরত মিথ্যা;
    * (HBUINT16 *) (header + 0) = 1;			/* format */
    * (HBUINT32 *) (header + 2) = regions_offset;
    * (HBUINT16 *) (header + 6) = তথ্য_গুনতি;
    জন্য (unsigned i = 0; i < তথ্য_গুনতি; i++)
      * (HBUINT32 *) (header + 8 + 4 * i) = তথ্য_offsets.arrayZ[i];

    জন্য (ধ্রুবক vardata_transform_t &t : transforms)
    {
      যদি (!t.alive ()) চালু;
      unsigned k = t.region_indices.দৈর্ঘ্য;
      HBUINT16 *v = (HBUINT16 *) c->allocate_আকার<অক্ষর> (6 + 2 * k);
      যদি (unlikely (!v)) ফেরত মিথ্যা;
      v[0] = 0;						/* itemCount */
      v[1] = 0;						/* wordSizeCount */
      v[2] = k;						/* regionIndices.len */
      জন্য (unsigned j = 0; j < k; j++)
	v[3 + j] = t.region_indices.arrayZ[j];
    }

    hb_vector_t<ধ্রুবক hb_hashmap_t<hb_tag_t, Triple>*> region_ptrs;
    যদি (unlikely (!region_ptrs.alloc_exact (new_regions.দৈর্ঘ্য))) ফেরত মিথ্যা;
    জন্য (ধ্রুবক auto &r : new_regions)
      region_ptrs.push (&r);
    auto *region_list = c->start_embed<OT::VarRegionList> ();
    ফেরত region_list->serialize (c, axis_tags, region_ptrs);
  }

  ব্যক্তিগত:
  hb_vector_t<hb_hashmap_t<hb_tag_t, Triple>> orig_regions;
};

/* Usage-collection pass for partial instancing: record which VarDatas the
 * retained charstrings' and private dicts' blends reference.  Mirrors the
 * emission pass: blends consumed by dropped hint ops don't count. */
কাঠামো cff2_usage_param_t
{
  শূন্য init () {}

  cff2_instancing_plan_t *instancing = nullptr;
  যৌক্তিক drop_hints = মিথ্যা;
  unsigned ivs = 0;	/* dict-side current vsindex */
};

কাঠামো cff2_cs_opset_usage_t : cff2_cs_opset_t<cff2_cs_opset_usage_t, cff2_usage_param_t, blend_arg_t>
{
  স্থির শূন্য flush_args_and_op (op_code_t op, cff2_cs_interp_env_t<blend_arg_t> &env, cff2_usage_param_t& param)
  {
    যৌক্তিক counts = সত্য;
    নির্বাচন (op)
    {
      ক্ষেত্র OpCode_return:
      ক্ষেত্র OpCode_endchar:
	counts = মিথ্যা;
	বিরতি;

      ক্ষেত্র OpCode_hstem:
      ক্ষেত্র OpCode_hstemhm:
      ক্ষেত্র OpCode_vstem:
      ক্ষেত্র OpCode_vstemhm:
      ক্ষেত্র OpCode_hintmask:
      ক্ষেত্র OpCode_cntrmask:
	যদি (param.drop_hints)
	  counts = মিথ্যা;
	বিরতি;

      পূর্বনির্ধারিত:
	বিরতি;
    }

    যদি (counts)
      জন্য (unsigned পূর্ণসংখ্যা i = 0; i < env.arg_স্তূপ.get_গুনতি (); i++)
	যদি (env.arg_স্তূপ[i].blending ())
	{
	  param.instancing->note_use (env.get_ivs ());
	  বিরতি;
	}

    env.clear_args ();
  }
};

কাঠামো cff2_private_dict_usage_opset_t : dict_opset_t
{
  স্থির শূন্য process_op (op_code_t op, cff2_priv_dict_interp_env_t& env, cff2_usage_param_t& param)
  {
    নির্বাচন (op) {
      ক্ষেত্র OpCode_vsindexdict:
	env.process_vsindex ();
	param.ivs = env.get_ivs ();
	env.clear_args ();
	ফেরত;
      ক্ষেত্র OpCode_blenddict:
	param.instancing->note_use (param.ivs);
	env.clear_args ();
	ফেরত;
      পূর্বনির্ধারিত:
	dict_opset_t::process_op (op, env);
	যদি (!env.arg_স্তূপ.is_empty ()) ফেরত;
	env.clear_args ();
	ফেরত;
    }
  }
};

} /* namespace CFF */

কাঠামো cff2_sub_table_info_t : cff_sub_table_info_t
{
  cff2_sub_table_info_t ()
    : cff_sub_table_info_t (),
      var_store_link (0)
  {}

  objidx_t  var_store_link;
};

কাঠামো cff2_শীর্ষ_dict_op_serializer_t : cff_শীর্ষ_dict_op_serializer_t<>
{
  যৌক্তিক serialize (hb_serialize_context_t *c,
		  ধ্রুবক op_str_t &opstr,
		  ধ্রুবক cff2_sub_table_info_t &info) ধ্রুবক
  {
    TRACE_SERIALIZE (এই);

    নির্বাচন (opstr.op)
    {
      ক্ষেত্র OpCode_vstore:
        যদি (info.var_store_link)
	  return_trace (FontDict::serialize_link4_op(c, opstr.op, info.var_store_link));
	অন্যথা
	  return_trace (সত্য);

      পূর্বনির্ধারিত:
	return_trace (cff_শীর্ষ_dict_op_serializer_t<>::serialize (c, opstr, info));
    }
  }
};

কাঠামো cff2_cs_opset_flatten_t : cff2_cs_opset_t<cff2_cs_opset_flatten_t, flatten_param_t, blend_arg_t>
{
  স্থির শূন্য flush_args_and_op (op_code_t op, cff2_cs_interp_env_t<blend_arg_t> &env, flatten_param_t& param)
  {
    /* Optionally capture command for specialization (before flushing, to preserve args) */
    যদি (param.commands)
    {
      যৌক্তিক skip_command = মিথ্যা;

      নির্বাচন (op)
      {
	ক্ষেত্র OpCode_return:
	ক্ষেত্র OpCode_endchar:
	  skip_command = সত্য;
	  বিরতি;

	ক্ষেত্র OpCode_hstem:
	ক্ষেত্র OpCode_hstemhm:
	ক্ষেত্র OpCode_vstem:
	ক্ষেত্র OpCode_vstemhm:
	ক্ষেত্র OpCode_hintmask:
	ক্ষেত্র OpCode_cntrmask:
	  যদি (param.drop_hints)
	    skip_command = সত্য;
	  বিরতি;

	পূর্বনির্ধারিত:
	  বিরতি;
      }

      যদি (!skip_command)
      {
	cs_command_t cmd (op);
	/* Capture resolved blend values */
	জন্য (unsigned পূর্ণসংখ্যা i = 0; i < env.arg_স্তূপ.get_গুনতি ();)
	{
	  ধ্রুবক blend_arg_t &arg = env.arg_স্তূপ[i];
	  যদি (arg.blending ())
	  {
	    /* For blend args, capture only the resolved default value */
	    cmd.args.push (arg);
	    /* Skip over the multiple blend values */
	    i += arg.অঙ্ক_মানসমূহ_2;
	  }
	  অন্যথা
	  {
	    cmd.args.push (arg);
	    i++;
	  }
	}
	param.commands->push (cmd);
      }
    }

    নির্বাচন (op)
    {
      ক্ষেত্র OpCode_return:
      ক্ষেত্র OpCode_endchar:
	/* dummy opcodes in CFF2. ignore */
	যদি (op == OpCode_endchar && param.instancer)
	  emit_vsindex_prefix (env, param);
	বিরতি;

      ক্ষেত্র OpCode_hstem:
      ক্ষেত্র OpCode_hstemhm:
      ক্ষেত্র OpCode_vstem:
      ক্ষেত্র OpCode_vstemhm:
      ক্ষেত্র OpCode_hintmask:
      ক্ষেত্র OpCode_cntrmask:
	যদি (param.drop_hints)
	{
	  env.clear_args ();
	  ফেরত;
	}
	HB_FALLTHROUGH;

      পূর্বনির্ধারিত:
	SUPER::flush_args_and_op (op, env, param);
	বিরতি;
    }
  }

  স্থির শূন্য flush_args (cff2_cs_interp_env_t<blend_arg_t> &env, flatten_param_t& param)
  {
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < env.arg_স্তূপ.get_গুনতি ();)
    {
      ধ্রুবক blend_arg_t &arg = env.arg_স্তূপ[i];
      যদি (arg.blending ())
      {
	যদি (unlikely (!((arg.অঙ্ক_মানসমূহ_2 > 0) && (env.arg_স্তূপ.get_গুনতি () >= arg.অঙ্ক_মানসমূহ_2))))
	{
	  env.set_error ();
	  ফেরত;
	}
	যদি (param.instancer)
	  rewrite_blends (arg, i, env, param);
	অন্যথা
	  flatten_blends (arg, i, env, param);
	i += arg.অঙ্ক_মানসমূহ_2;
      }
      অন্যথা
      {
	str_encoder_t  encoder (param.flatStr);
	encoder.encode_অঙ্ক_cs (arg);
	i++;
      }
    }
    SUPER::flush_args (env, param);
  }

  স্থির শূন্য flatten_blends (ধ্রুবক blend_arg_t &arg, unsigned পূর্ণসংখ্যা i, cff2_cs_interp_env_t<blend_arg_t> &env, flatten_param_t& param)
  {
    /* flatten the default values */
    str_encoder_t  encoder (param.flatStr);
    জন্য (unsigned পূর্ণসংখ্যা j = 0; j < arg.অঙ্ক_মানসমূহ_2; j++)
    {
      ধ্রুবক blend_arg_t &arg1 = env.arg_স্তূপ[i + j];
      যদি (unlikely (!((arg1.blending () && (arg.অঙ্ক_মানসমূহ_2 == arg1.অঙ্ক_মানসমূহ_2) && (arg1.মান_সূচক_2 == j) &&
	      (arg1.deltas.দৈর্ঘ্য == env.get_region_গুনতি ())))))
      {
	env.set_error ();
	ফেরত;
      }
      encoder.encode_অঙ্ক_cs (arg1);
    }
    /* flatten deltas for each value */
    জন্য (unsigned পূর্ণসংখ্যা j = 0; j < arg.অঙ্ক_মানসমূহ_2; j++)
    {
      ধ্রুবক blend_arg_t &arg1 = env.arg_স্তূপ[i + j];
      জন্য (unsigned পূর্ণসংখ্যা k = 0; k < arg1.deltas.দৈর্ঘ্য; k++)
	encoder.encode_অঙ্ক_cs (arg1.deltas[k]);
    }
    /* flatten the number of values followed by blend operator */
    encoder.encode_int (arg.অঙ্ক_মানসমূহ_2);
    encoder.encode_op (OpCode_blendcs);
  }

  /* Partial instancing: rewrite one blend group against the instanced store.
   * Each value's default absorbs the folded (pinned-away) contribution, and
   * its deltas are mapped onto the new region list. */
  স্থির শূন্য rewrite_blends (ধ্রুবক blend_arg_t &arg, unsigned পূর্ণসংখ্যা i, cff2_cs_interp_env_t<blend_arg_t> &env, flatten_param_t& param)
  {
    ধ্রুবক cff2_instancing_plan_t &instancer = *param.instancer;
    unsigned ivs = env.get_ivs ();
    ধ্রুবক cff2_instancing_plan_t::vardata_transform_t *t = instancer.get_transform (ivs);
    যদি (unlikely (!t))
    {
      env.set_error ();
      ফেরত;
    }

    unsigned n = arg.অঙ্ক_মানসমূহ_2;
    জন্য (unsigned পূর্ণসংখ্যা j = 0; j < n; j++)
    {
      ধ্রুবক blend_arg_t &arg1 = env.arg_স্তূপ[i + j];
      যদি (unlikely (!(arg1.blending () && (arg1.অঙ্ক_মানসমূহ_2 == n) && (arg1.মান_সূচক_2 == j) &&
		      (arg1.deltas.দৈর্ঘ্য == env.get_region_গুনতি ()) &&
		      (arg1.deltas.দৈর্ঘ্য == t->gains.দৈর্ঘ্য))))
      {
	env.set_error ();
	ফেরত;
      }
    }

    str_encoder_t encoder (param.flatStr);
    unsigned k_new = t->alive () ? t->matrix.দৈর্ঘ্য : 0;

    যদি (!k_new)
    {
      /* All of this VarData's regions died; blends dissolve into statics. */
      জন্য (unsigned পূর্ণসংখ্যা j = 0; j < n; j++)
      {
	ধ্রুবক blend_arg_t &arg1 = env.arg_স্তূপ[i + j];
	সংখ্যা_t v;
	v.set_real (arg1.to_real () + round (instancer.fold_deltas (ivs, arg1.deltas.as_array ())));
	encoder.encode_অঙ্ক_cs (v);
      }
      ফেরত;
    }

    /* Emit in chunks so the output charstring stays within the argument
     * stack limit: values already emitted for this op, plus this chunk's
     * defaults, deltas and count, must not exceed 513. */
    unsigned done = 0;
    যখন (done < n)
    {
      unsigned avail = 512 > (i + done) ? 512 - (i + done) : 0;
      unsigned chunk = hb_min (avail / (k_new + 1), n - done);
      যদি (unlikely (!chunk))
      {
	env.set_error ();
	ফেরত;
      }

      জন্য (unsigned পূর্ণসংখ্যা j = done; j < done + chunk; j++)
      {
	ধ্রুবক blend_arg_t &arg1 = env.arg_স্তূপ[i + j];
	সংখ্যা_t v;
	v.set_real (arg1.to_real () + round (instancer.fold_deltas (ivs, arg1.deltas.as_array ())));
	encoder.encode_অঙ্ক_cs (v);
      }
      জন্য (unsigned পূর্ণসংখ্যা j = done; j < done + chunk; j++)
      {
	ধ্রুবক blend_arg_t &arg1 = env.arg_স্তূপ[i + j];
	জন্য (unsigned r = 0; r < k_new; r++)
	{
	  সংখ্যা_t v;
	  v.set_int (hb_clamp_to<পূর্ণসংখ্যা> (round (t->transform_delta (r, arg1.deltas.as_array ()))));
	  encoder.encode_অঙ্ক_cs (v);
	}
      }
      encoder.encode_int (chunk);
      encoder.encode_op (OpCode_blendcs);
      done += chunk;
    }
    param.emitted_blend = সত্য;
  }

  /* Partial instancing: prepend a vsindex op when the charstring's
   * (remapped) ivs differs from its FD's (remapped) private-dict ivs. */
  স্থির শূন্য emit_vsindex_prefix (cff2_cs_interp_env_t<blend_arg_t> &env, flatten_param_t& param)
  {
    যদি (!param.emitted_blend) ফেরত;
    ধ্রুবক cff2_instancing_plan_t &instancer = *param.instancer;
    unsigned new_ivs = instancer.remap_ivs (env.get_ivs ());
    unsigned fd_ivs = instancer.remap_ivs (env.get_orig_ivs ());
    যদি (new_ivs == fd_ivs) ফেরত;

    str_buff_t prefix;
    str_encoder_t enc (prefix);
    enc.encode_int ((পূর্ণসংখ্যা) new_ivs);
    enc.encode_op (OpCode_vsindexcs);

    str_buff_t &buf = param.flatStr;
    unsigned old_len = buf.দৈর্ঘ্য;
    যদি (unlikely (prefix.in_error () || !buf.resize (old_len + prefix.দৈর্ঘ্য)))
    {
      env.set_error ();
      ফেরত;
    }
    memmove (buf.arrayZ + prefix.দৈর্ঘ্য, buf.arrayZ, old_len);
    hb_memcpy (buf.arrayZ, prefix.arrayZ, prefix.দৈর্ঘ্য);
  }

  স্থির শূন্য flush_op (op_code_t op, cff2_cs_interp_env_t<blend_arg_t> &env, flatten_param_t& param)
  {
    নির্বাচন (op)
    {
      ক্ষেত্র OpCode_return:
      ক্ষেত্র OpCode_endchar:
	ফেরত;
      পূর্বনির্ধারিত:
	str_encoder_t  encoder (param.flatStr);
	encoder.encode_op (op);
    }
  }

  স্থির শূন্য flush_hintmask (op_code_t op, cff2_cs_interp_env_t<blend_arg_t> &env, flatten_param_t& param)
  {
    SUPER::flush_hintmask (op, env, param);
    /* Preserve hintmask payload in captured commands for specializer re-encoding. */
    যদি (param.commands && !param.drop_hints && param.commands->দৈর্ঘ্য > 0)
    {
      auto &cmd = param.commands->লেজ ();
      যদি (cmd.op == op)
      {
        cmd.mask_bytes.resize (env.hintmask_আকার);
        যদি (unlikely (cmd.mask_bytes.in_error ()))
        {
          env.set_error ();
          ফেরত;
        }
        জন্য (unsigned পূর্ণসংখ্যা i = 0; i < env.hintmask_আকার; i++)
          cmd.mask_bytes[i] = env.str_ref[i];
      }
    }
    যদি (!param.drop_hints)
    {
      str_encoder_t  encoder (param.flatStr);
      জন্য (unsigned পূর্ণসংখ্যা i = 0; i < env.hintmask_আকার; i++)
	encoder.encode_byte (env.str_ref[i]);
    }
  }

  ব্যক্তিগত:
  প্রকারলেখ cff2_cs_opset_t<cff2_cs_opset_flatten_t, flatten_param_t, blend_arg_t> SUPER;
  প্রকারলেখ cs_opset_t<blend_arg_t, cff2_cs_opset_flatten_t, cff2_cs_opset_flatten_t, cff2_cs_interp_env_t<blend_arg_t>, flatten_param_t> CSOPSET;
};

কাঠামো cff2_cs_opset_subr_subset_t : cff2_cs_opset_t<cff2_cs_opset_subr_subset_t, subr_subset_param_t, blend_arg_t>
{
  স্থির শূন্য process_op (op_code_t op, cff2_cs_interp_env_t<blend_arg_t> &env, subr_subset_param_t& param)
  {
    নির্বাচন (op) {

      ক্ষেত্র OpCode_return:
	param.current_parsed_str->flush_coalesced (env.str_ref);
	param.current_parsed_str->set_parsed ();
	env.return_from_subr ();
	param.set_current_str (env, মিথ্যা);
	বিরতি;

      ক্ষেত্র OpCode_endchar:
	param.current_parsed_str->flush_coalesced (env.str_ref);
	param.current_parsed_str->set_parsed ();
	SUPER::process_op (op, env, param);
	বিরতি;

      ক্ষেত্র OpCode_callsubr:
	process_call_subr (op, CSType_LocalSubr, env, param, env.localSubrs, param.local_closure);
	বিরতি;

      ক্ষেত্র OpCode_callgsubr:
	process_call_subr (op, CSType_GlobalSubr, env, param, env.globalSubrs, param.global_closure);
	বিরতি;

      পূর্বনির্ধারিত:
	SUPER::process_op (op, env, param);
	param.current_parsed_str->add_op (op, env.str_ref);
	বিরতি;
    }
  }

  সুরক্ষিত:
  স্থির শূন্য process_call_subr (op_code_t op, cs_type_t type,
				 cff2_cs_interp_env_t<blend_arg_t> &env, subr_subset_param_t& param,
				 cff2_biased_subrs_t& subrs, hb_set_t *closure)
  {
    byte_str_ref_t    str_ref = env.str_ref;
    env.call_subr (subrs, type);
    param.current_parsed_str->add_call_op (op, str_ref, env.context.subr_অঙ্ক_2);
    closure->add (env.context.subr_অঙ্ক_2);
    param.set_current_str (env, সত্য);
  }

  ব্যক্তিগত:
  প্রকারলেখ cff2_cs_opset_t<cff2_cs_opset_subr_subset_t, subr_subset_param_t, blend_arg_t> SUPER;
};

কাঠামো cff2_subr_subsetter_t : subr_subsetter_t<cff2_subr_subsetter_t, CFF2Subrs, ধ্রুবক OT::cff2::accelerator_subset_t, cff2_cs_interp_env_t<blend_arg_t>, cff2_cs_opset_subr_subset_t>
{
  cff2_subr_subsetter_t (ধ্রুবক OT::cff2::accelerator_subset_t &acc_, ধ্রুবক hb_subset_plan_t *plan_)
    : subr_subsetter_t (acc_, plan_) {}

  স্থির শূন্য complete_parsed_str (cff2_cs_interp_env_t<blend_arg_t> &env, subr_subset_param_t& param, parsed_cs_str_t &charstring)
  {
    /* vsindex is inserted at the beginning of the charstring as necessary */
    যদি (env.seen_vsindex ())
    {
      সংখ্যা_t  ivs;
      ivs.set_int ((পূর্ণসংখ্যা)env.get_ivs ());
      charstring.set_prefix (ivs, OpCode_vsindexcs);
    }
  }
};

কাঠামো cff2_private_blend_encoder_param_t
{
  cff2_private_blend_encoder_param_t (hb_serialize_context_t *c,
				      ধ্রুবক CFF2ItemVariationStore *varStore,
				      hb_array_t<পূর্ণসংখ্যা> normalized_coords) :
    c (c), varStore (varStore), normalized_coords (normalized_coords) {}

  শূন্য init () {}

  শূন্য process_blend ()
  {
    যদি (!seen_blend)
    {
      region_গুনতি_2 = varStore->varStore.get_region_সূচক_গুনতি (ivs);
      যদি (unlikely (!scalars.resize_exact (region_গুনতি_2)))
      {
	যদি (c) c->err (HB_SERIALIZE_ERROR_OTHER);
	seen_blend = সত্য;
	ফেরত;
      }
      varStore->varStore.get_region_scalars (ivs, normalized_coords.arrayZ, normalized_coords.দৈর্ঘ্য,
					     &scalars[0], region_গুনতি_2);
      seen_blend = সত্য;
    }
  }

  দ্বিগুণ blend_deltas (hb_array_t<ধ্রুবক সংখ্যা_t> deltas) ধ্রুবক
  {
    দ্বিগুণ v = 0;
    যদি (likely (scalars.দৈর্ঘ্য == deltas.দৈর্ঘ্য))
    {
      unsigned গুনতি = scalars.দৈর্ঘ্য;
      জন্য (unsigned i = 0; i < গুনতি; i++)
	v += (দ্বিগুণ) scalars.arrayZ[i] * deltas.arrayZ[i].to_real ();
    }
    ফেরত v;
  }


  hb_serialize_context_t *c = nullptr;
  যৌক্তিক seen_blend = মিথ্যা;
  unsigned ivs = 0;
  unsigned region_গুনতি_2 = 0;
  hb_vector_t<দশমিক> scalars;
  ধ্রুবক	 CFF2ItemVariationStore *varStore = nullptr;
  hb_array_t<পূর্ণসংখ্যা> normalized_coords;

  /* CFF2 partial instancing: when set, blends are rewritten against the
   * instanced variation store instead of being flattened. */
  ধ্রুবক cff2_instancing_plan_t *instancer = nullptr;
  /* Error-compensated fold rounding state, per dict operator. */
  দ্বিগুণ fold_exact = 0.;
  দ্বিগুণ fold_emitted = 0.;
};

কাঠামো cff2_private_dict_blend_opset_t : dict_opset_t
{
  স্থির শূন্য process_arg_blend (cff2_private_blend_encoder_param_t& param,
				 সংখ্যা_t &arg,
				 ধ্রুবক hb_array_t<ধ্রুবক সংখ্যা_t> blends,
				 unsigned n, unsigned i)
  {
    arg.set_int (round (arg.to_real () + param.blend_deltas (blends)));
  }

  /* The fold added to a value, error-compensated across the operator: most
   * blended private-dict values (BlueValues, StemSnap*, ...) are
   * delta-encoded, so independently rounded folds would accumulate in the
   * decoded absolutes; keeping the emitted cumulative fold equal to the
   * rounded exact cumulative fold bounds every absolute within ±0.5. */
  স্থির দ্বিগুণ fold (cff2_private_blend_encoder_param_t& param,
		      ধ্রুবক hb_array_t<ধ্রুবক সংখ্যা_t> blends)
  {
    দ্বিগুণ f = param.blend_deltas (blends);
    দ্বিগুণ emit = round (param.fold_exact + f) - param.fold_emitted;
    param.fold_exact += f;
    param.fold_emitted += emit;
    ফেরত emit;
  }

  /* Partial instancing: emit the args preceding this blend group, then the
   * group itself rewritten against the instanced store, and clear the
   * stack.  The op's remaining args are flushed at process_op time. */
  স্থির শূন্য rewrite_blends (cff2_priv_dict_interp_env_t& env,
			      cff2_private_blend_encoder_param_t& param,
			      unsigned start, unsigned n, unsigned k)
  {
    ধ্রুবক cff2_instancing_plan_t &instancer = *param.instancer;
    ধ্রুবক cff2_instancing_plan_t::vardata_transform_t *t = instancer.get_transform (param.ivs);
    যদি (unlikely (!t || t->gains.দৈর্ঘ্য != k))
    {
      env.set_error ();
      ফেরত;
    }

    str_buff_t str;
    str_encoder_t encoder (str);

    জন্য (unsigned idx = 0; idx < start; idx++)
      encoder.encode_অঙ্ক_tp (env.arg_স্তূপ[idx]);

    unsigned k_new = t->alive () ? t->matrix.দৈর্ঘ্য : 0;
    যদি (!k_new)
    {
      /* This VarData died (or is unused by any retained charstring, which
       * can happen for FDs kept only for retain-gids holes); blends
       * dissolve into statics. */
      জন্য (unsigned j = 0; j < n; j++)
      {
	ধ্রুবক hb_array_t<ধ্রুবক সংখ্যা_t> blends = env.arg_স্তূপ.sub_array (start + n + (j * k), k);
	সংখ্যা_t v;
	v.set_real (env.arg_স্তূপ[start + j].to_real () + fold (param, blends));
	encoder.encode_অঙ্ক_tp (v);
      }
    }
    অন্যথা
    {
      unsigned done = 0;
      যখন (done < n)
      {
	unsigned avail = 512 > (start + done) ? 512 - (start + done) : 0;
	unsigned chunk = hb_min (avail / (k_new + 1), n - done);
	যদি (unlikely (!chunk))
	{
	  env.set_error ();
	  ফেরত;
	}

	জন্য (unsigned j = done; j < done + chunk; j++)
	{
	  ধ্রুবক hb_array_t<ধ্রুবক সংখ্যা_t> blends = env.arg_স্তূপ.sub_array (start + n + (j * k), k);
	  সংখ্যা_t v;
	  v.set_real (env.arg_স্তূপ[start + j].to_real () + fold (param, blends));
	  encoder.encode_অঙ্ক_tp (v);
	}
	জন্য (unsigned j = done; j < done + chunk; j++)
	{
	  ধ্রুবক hb_array_t<ধ্রুবক সংখ্যা_t> blends = env.arg_স্তূপ.sub_array (start + n + (j * k), k);
	  জন্য (unsigned r = 0; r < k_new; r++)
	  {
	    সংখ্যা_t v;
	    v.set_int (hb_clamp_to<পূর্ণসংখ্যা> (round (t->transform_delta (r, blends))));
	    encoder.encode_অঙ্ক_tp (v);
	  }
	}
	encoder.encode_int (chunk);
	encoder.encode_op (OpCode_blenddict);
	done += chunk;
      }
    }

    auto bytes = str.as_bytes ();
    param.c->embed (&bytes, bytes.দৈর্ঘ্য);

    env.clear_args ();
  }

  স্থির শূন্য process_blend (cff2_priv_dict_interp_env_t& env, cff2_private_blend_encoder_param_t& param)
  {
    unsigned পূর্ণসংখ্যা n, k;

    param.process_blend ();
    k = param.region_গুনতি_2;
    n = env.arg_স্তূপ.বের_করো_uint ();

    unsigned গুনতি = env.arg_স্তূপ.get_গুনতি ();
    unsigned পূর্ণসংখ্যা যোগ;
    যদি (unlikely (hb_unsigned_mul_overflows (k + 1, n, &যোগ) || যোগ > গুনতি))
    {
      env.set_error ();
      ফেরত;
    }

    /* copy the blend values into blend array of the default values */
    unsigned পূর্ণসংখ্যা start = গুনতি - যোগ;
    /* let an obvious error case fail, but note CFF2 spec doesn't forbid n==0 */
    যদি (unlikely (start > গুনতি))
    {
      env.set_error ();
      ফেরত;
    }

    যদি (param.instancer && k)
    {
      rewrite_blends (env, param, start, n, k);
      ফেরত;
    }

    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < n; i++)
    {
      ধ্রুবক hb_array_t<ধ্রুবক সংখ্যা_t> blends = env.arg_স্তূপ.sub_array (start + n + (i * k), k);
      process_arg_blend (param, env.arg_স্তূপ[start + i], blends, n, i);
    }

    /* pop off blend values leaving default values now adorned with blend values */
    env.arg_স্তূপ.বের_করো (k * n);
  }

  স্থির শূন্য process_op (op_code_t op, cff2_priv_dict_interp_env_t& env, cff2_private_blend_encoder_param_t& param)
  {
    নির্বাচন (op) {
      ক্ষেত্র OpCode_StdHW:
      ক্ষেত্র OpCode_StdVW:
      ক্ষেত্র OpCode_BlueScale:
      ক্ষেত্র OpCode_BlueShift:
      ক্ষেত্র OpCode_BlueFuzz:
      ক্ষেত্র OpCode_ExpansionFactor:
      ক্ষেত্র OpCode_LanguageGroup:
      ক্ষেত্র Op_Code_Blue_মানসমূহ:
      ক্ষেত্র OpCode_OtherBlues:
      ক্ষেত্র OpCode_FamilyBlues:
      ক্ষেত্র OpCode_FamilyOtherBlues:
      ক্ষেত্র OpCode_StemSnapH:
      ক্ষেত্র OpCode_StemSnapV:
	বিরতি;
      ক্ষেত্র OpCode_vsindexdict:
	env.process_vsindex ();
	param.ivs = env.get_ivs ();
	যদি (param.instancer)
	{
	  /* Re-emit with the remapped vsindex; 0 is the dict default. */
	  unsigned new_ivs = param.instancer->remap_ivs (param.ivs);
	  যদি (new_ivs)
	  {
	    str_buff_t str;
	    str_encoder_t encoder (str);
	    encoder.encode_int ((পূর্ণসংখ্যা) new_ivs);
	    encoder.encode_op (OpCode_vsindexdict);
	    auto bytes = str.as_bytes ();
	    param.c->embed (&bytes, bytes.দৈর্ঘ্য);
	  }
	}
	env.clear_args ();
	ফেরত;
      ক্ষেত্র OpCode_blenddict:
	process_blend (env, param);
	ফেরত;

      পূর্বনির্ধারিত:
	dict_opset_t::process_op (op, env);
	যদি (!env.arg_স্তূপ.is_empty ()) ফেরত;
	বিরতি;
    }

    যদি (unlikely (env.in_error ())) ফেরত;

    // Write args then op

    str_buff_t str;
    str_encoder_t encoder (str);

    unsigned গুনতি = env.arg_স্তূপ.get_গুনতি ();
    জন্য (unsigned i = 0; i < গুনতি; i++)
      encoder.encode_অঙ্ক_tp (env.arg_স্তূপ[i]);

    encoder.encode_op (op);

    auto bytes = str.as_bytes ();
    param.c->embed (&bytes, bytes.দৈর্ঘ্য);

    env.clear_args ();
    param.fold_exact = param.fold_emitted = 0.;
  }
};

কাঠামো cff2_private_dict_op_serializer_t : op_serializer_t
{
  cff2_private_dict_op_serializer_t (যৌক্তিক desubroutinize_, যৌক্তিক drop_hints_, যৌক্তিক pinned_,
				     ধ্রুবক CFF::CFF2ItemVariationStore* varStore_,
				     hb_array_t<পূর্ণসংখ্যা> normalized_coords_,
				     ধ্রুবক cff2_instancing_plan_t *instancer_ = nullptr)
    : desubroutinize (desubroutinize_), drop_hints (drop_hints_), pinned (pinned_),
      param (nullptr, varStore_, normalized_coords_)
  { param.instancer = instancer_; }

  যৌক্তিক serialize (hb_serialize_context_t *c,
		  ধ্রুবক op_str_t &opstr,
		  objidx_t subrs_link) ধ্রুবক
  {
    TRACE_SERIALIZE (এই);

    যদি (drop_hints && dict_opset_t::is_hint_op (opstr.op))
      return_trace (সত্য);

    যদি (opstr.op == OpCode_Subrs)
    {
      যদি (desubroutinize || !subrs_link)
	return_trace (সত্য);
      অন্যথা
	return_trace (FontDict::serialize_link2_op (c, opstr.op, subrs_link));
    }

    যদি (pinned || param.instancer)
    {
      /* Reinterpret opstr and process blends.  The param persists across
       * this dict's opstrs, so that an ivs set by a vsindexdict opstr is
       * still in effect when a later opstr's blends are processed. */
      cff2_priv_dict_interp_env_t env {hb_ubytes_t (opstr.ptr, opstr.দৈর্ঘ্য)};
      param.c = c;
      dict_interpreter_t<cff2_private_dict_blend_opset_t, cff2_private_blend_encoder_param_t, cff2_priv_dict_interp_env_t> interp (env);
      return_trace (interp.interpret (param));
    }

    return_trace (copy_opstr (c, opstr));
  }

  সুরক্ষিত:
  ধ্রুবক যৌক্তিক desubroutinize;
  ধ্রুবক যৌক্তিক drop_hints;
  ধ্রুবক যৌক্তিক pinned;
  mutable cff2_private_blend_encoder_param_t param;
};



নামাকাশ OT {
কাঠামো cff2_subset_plan
{
  যৌক্তিক create (ধ্রুবক OT::cff2::accelerator_subset_t &acc,
	      hb_subset_plan_t *plan)
  {
    /* make sure notdef is first */
    hb_codepoint_t old_glyph;
    যদি (!plan->old_gid_for_new_gid (0, &old_glyph) || (old_glyph != 0)) ফেরত মিথ্যা;

    অঙ্ক_glyphs = plan->অঙ্ক_output_glyphs ();
    orig_fdcount = acc.fdArray->গুনতি;

    drop_hints = plan->flags & HB_SUBSET_FLAGS_NO_HINTING;
    যৌক্তিক instancing_requested = (যৌক্তিক) plan->normalized_coords;
    pinned = instancing_requested && plan->all_axes_pinned;
    normalized_coords = plan->normalized_coords;
    শীর্ষ_maxp_info = plan->শীর্ষ_maxp_info;
    hmtx_map = &plan->hmtx_map;
    desubroutinize = plan->flags & HB_SUBSET_FLAGS_DESUBROUTINIZE ||
		     instancing_requested; // For instancing we need this path

    /* Partial instancing: some axes pinned or limited, others kept. */
    partial_instancing = instancing_requested && !pinned &&
			 acc.varStore != &Null (CFF2ItemVariationStore);
    যদি (partial_instancing &&
	unlikely (!instancing.create (acc.varStore, plan)))
      ফেরত মিথ্যা;
    axis_tags = &plan->axis_tags;

    /* Enable command capture for CFF2→CFF1 conversion (for specialization) */
    capture_commands = pinned;

 #ifdef HB_EXPERIMENTAL_API
    min_charstrings_off_আকার = (plan->flags & HB_SUBSET_FLAGS_IFTB_REQUIREMENTS) ? 4 : 0;
 #অন্যথা
    min_charstrings_off_আকার = 0;
 #endif

    যদি (desubroutinize)
    {
      যদি (partial_instancing)
      {
	/* Pass 1: collect which VarDatas the retained blends reference, so
	 * unused ones get pruned and the survivors renumbered. */
	hb_set_t used_fds;
	জন্য (unsigned পূর্ণসংখ্যা i = 0; i < অঙ্ক_glyphs; i++)
	{
	  hb_codepoint_t glyph;
	  যদি (!plan->old_gid_for_new_gid (i, &glyph))
	    চালু;
	  ধ্রুবক hb_ubytes_t str = (*acc.charStrings)[glyph];
	  unsigned পূর্ণসংখ্যা fd = CFF::subset_get_fd (acc, glyph);
	  যদি (unlikely (fd >= acc.fd_গুনতি))
	    ফেরত মিথ্যা;
	  used_fds.add (fd);

	  cff2_cs_interp_env_t<blend_arg_t> env (str, acc, fd);
	  cs_interpreter_t<cff2_cs_interp_env_t<blend_arg_t>, cff2_cs_opset_usage_t, cff2_usage_param_t> interp (env);
	  cff2_usage_param_t param;
	  param.instancing = &instancing;
	  param.drop_hints = drop_hints;
	  যদি (unlikely (!interp.interpret (param)))
	    ফেরত মিথ্যা;
	}
	জন্য (unsigned fd : used_fds)
	{
	  cff2_usage_param_t param;
	  param.instancing = &instancing;
	  unsigned গুনতি = acc.privateDicts[fd].get_গুনতি ();
	  জন্য (unsigned j = 0; j < গুনতি; j++)
	  {
	    ধ্রুবক op_str_t &opstr = acc.privateDicts[fd][j];
	    যদি (drop_hints && dict_opset_t::is_hint_op (opstr.op))
	      চালু;
	    cff2_priv_dict_interp_env_t env {hb_ubytes_t (opstr.ptr, opstr.দৈর্ঘ্য)};
	    dict_interpreter_t<cff2_private_dict_usage_opset_t, cff2_usage_param_t, cff2_priv_dict_interp_env_t> interp (env);
	    যদি (unlikely (!interp.interpret (param)))
	      ফেরত মিথ্যা;
	  }
	}
	যদি (unlikely (!instancing.finalize ()))
	  ফেরত মিথ্যা;

	/* Pass 2: like subr_flattener_t, but blends stay symbolic (no coords
	 * fed to the env) and get rewritten against the instanced store. */
	যদি (!subset_charstrings.resize_exact (অঙ্ক_glyphs))
	  ফেরত মিথ্যা;
	জন্য (unsigned পূর্ণসংখ্যা i = 0; i < অঙ্ক_glyphs; i++)
	{
	  hb_codepoint_t glyph;
	  যদি (!plan->old_gid_for_new_gid (i, &glyph))
	    চালু;
	  ধ্রুবক hb_ubytes_t str = (*acc.charStrings)[glyph];
	  unsigned পূর্ণসংখ্যা fd = CFF::subset_get_fd (acc, glyph);
	  যদি (unlikely (fd >= acc.fd_গুনতি))
	    ফেরত মিথ্যা;

	  cff2_cs_interp_env_t<blend_arg_t> env (str, acc, fd);
	  cs_interpreter_t<cff2_cs_interp_env_t<blend_arg_t>, cff2_cs_opset_flatten_t, flatten_param_t> interp (env);
	  flatten_param_t param = {
	    subset_charstrings.arrayZ[i],
	    (যৌক্তিক) (plan->flags & HB_SUBSET_FLAGS_NO_HINTING),
	    plan
	  };
	  param.instancer = &instancing;
	  যদি (unlikely (!interp.interpret (param)))
	    ফেরত মিথ্যা;
	}
      }
      অন্যথা
      {
      /* Flatten global & local subrs */
      subr_flattener_t<ধ্রুবক OT::cff2::accelerator_subset_t, cff2_cs_interp_env_t<blend_arg_t>, cff2_cs_opset_flatten_t>
		    flattener(acc, plan);

      /* Enable command capture if requested (for specialization) */
      যদি (capture_commands)
      {
	যদি (!charstring_commands.resize_exact (অঙ্ক_glyphs))
	  ফেরত মিথ্যা;

	যদি (!flattener.flatten (subset_charstrings, &charstring_commands))
	  ফেরত মিথ্যা;
      }
      অন্যথা
      {
	যদি (!flattener.flatten (subset_charstrings))
	  ফেরত মিথ্যা;
      }
      }
    }
    অন্যথা
    {
      cff2_subr_subsetter_t	subr_subsetter (acc, plan);

      /* Subset subrs: collect used subroutines, leaving all unused ones behind */
      যদি (!subr_subsetter.subset ())
	ফেরত মিথ্যা;

      /* encode charstrings, global subrs, local subrs with new subroutine numbers */
      যদি (!subr_subsetter.encode_charstrings (subset_charstrings, !pinned))
	ফেরত মিথ্যা;

      যদি (!subr_subsetter.encode_globalsubrs (subset_globalsubrs))
	ফেরত মিথ্যা;

      /* local subrs */
      যদি (!subset_localsubrs.resize (orig_fdcount))
	ফেরত মিথ্যা;
      জন্য (unsigned পূর্ণসংখ্যা fd = 0; fd < orig_fdcount; fd++)
      {
	subset_localsubrs[fd].init ();
	যদি (!subr_subsetter.encode_localsubrs (fd, subset_localsubrs[fd]))
	  ফেরত মিথ্যা;
      }
    }

    /* FDSelect */
    যদি (acc.fdSelect != &Null (CFF2FDSelect))
    {
      যদি (unlikely (!hb_plan_subset_cff_fdselect (plan,
						  orig_fdcount,
						  *(ধ্রুবক FDSelect *)acc.fdSelect,
						  acc.অঙ্ক_glyphs,
						  subset_fdcount,
						  subset_fdselect_আকার,
						  subset_fdselect_format,
						  subset_fdselect_ranges,
						  fdmap)))
	ফেরত মিথ্যা;
    }
    অন্যথা
      fdmap.identity (1);

    ফেরত সত্য;
  }

  cff2_sub_table_info_t info;

  unsigned পূর্ণসংখ্যা    অঙ্ক_glyphs;
  unsigned পূর্ণসংখ্যা    orig_fdcount = 0;
  unsigned পূর্ণসংখ্যা    subset_fdcount = 1;
  unsigned পূর্ণসংখ্যা    subset_fdselect_আকার = 0;
  unsigned পূর্ণসংখ্যা    subset_fdselect_format = 0;
  যৌক্তিক            pinned = মিথ্যা;
  hb_vector_t<code_pair_t>   subset_fdselect_ranges;

  hb_inc_bimap_t   fdmap;

  str_buff_vec_t	    subset_charstrings;
  str_buff_vec_t	    subset_globalsubrs;
  hb_vector_t<str_buff_vec_t> subset_localsubrs;

  যৌক্তিক	    drop_hints = মিথ্যা;
  যৌক্তিক	    desubroutinize = মিথ্যা;

  যৌক্তিক	    partial_instancing = মিথ্যা;
  cff2_instancing_plan_t instancing;
  ধ্রুবক hb_vector_t<hb_tag_t> *axis_tags = nullptr;

  unsigned  min_charstrings_off_আকার = 0;

  hb_array_t<পূর্ণসংখ্যা> normalized_coords; // For instantiation
  শীর্ষ_maxp_info_t শীর্ষ_maxp_info;  // For FontBBox
  ধ্রুবক hb_hashmap_t<hb_codepoint_t, hb_pair_t<unsigned, পূর্ণসংখ্যা>> *hmtx_map; // For widths

  // Width optimization results (for CFF1 conversion)
  unsigned default_width = 0;
  unsigned nominal_width = 0;

  // Command capture for specialization (CFF2→CFF1 conversion)
  যৌক্তিক capture_commands = মিথ্যা;
  hb_vector_t<hb_vector_t<cs_command_t>> charstring_commands;
};
} // namespace OT

/*
 * CFF2 to CFF1 Converter Implementation
 */

#অন্তর্ভাব "hb-cff-width-optimizer.hh"
#অন্তর্ভাব "hb-cff-specializer.hh"

/* Serialize charstrings using CFF1 format with widths */
স্থির যৌক্তিক
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
  যদি (unlikely (!cff1_charstrings.resize (plan.subset_charstrings.দৈর্ঘ্য)))
  {
    c->বের_করো_discard ();
    ফেরত মিথ্যা;
  }

  জন্য (unsigned i = 0; i < plan.subset_charstrings.দৈর্ঘ্য; i++)
  {
    // Get width for this glyph from hmtx_map
    unsigned width = 0;
    যদি (plan.hmtx_map->has (i))
      width = plan.hmtx_map->get (i).first;

    // Encode width if different from default
    str_encoder_t encoder (cff1_charstrings[i]);
    যদি (width != default_width)
    {
      পূর্ণসংখ্যা delta = (পূর্ণসংখ্যা) width - (পূর্ণসংখ্যা) nominal_width;
      encoder.encode_int (delta);
    }

    // Use specialized commands if available, otherwise use binary
    যদি (plan.capture_commands && i < plan.charstring_commands.দৈর্ঘ্য &&
        plan.charstring_commands[i].দৈর্ঘ্য > 0)
    {
      // Specialize and encode commands
      auto &commands = plan.charstring_commands[i];
      CFF::specialize_commands (commands, 48);  /* maxstack=48 for CFF1 */
      যদি (unlikely (!CFF::encode_commands (commands, cff1_charstrings[i])))
      {
        c->বের_করো_discard ();
        ফেরত মিথ্যা;
      }
    }
    অন্যথা
    {
      // Use binary CharString
      ধ্রুবক str_buff_t &cs = plan.subset_charstrings[i];
      জন্য (unsigned j = 0; j < cs.দৈর্ঘ্য; j++)
        cff1_charstrings[i].push (cs[j]);
    }

    // Check if it already ends with endchar (0x0e) or return (0x0b)
    যদি (cff1_charstrings[i].দৈর্ঘ্য == 0 ||
        (cff1_charstrings[i].লেজ () != 0x0e && cff1_charstrings[i].লেজ () != 0x0b))
    {
      // Append endchar operator
      যদি (unlikely (!cff1_charstrings[i].ঠেলো_or_fail (0x0e)))
      {
        c->বের_করো_discard ();
        ফেরত মিথ্যা;
      }
    }
  }

  unsigned তথ্য_আকার = 0;
  unsigned যোগ_আকার = CFF1CharStrings::যোগ_আকার (cff1_charstrings, &তথ্য_আকার, plan.min_charstrings_off_আকার);
  যদি (unlikely (!c->start_zerocopy (যোগ_আকার)))
  {
    c->বের_করো_discard ();
    ফেরত মিথ্যা;
  }

  auto *cs = c->start_embed<CFF1CharStrings> ();
  যদি (unlikely (!cs->serialize (c, cff1_charstrings, &তথ্য_আকার, plan.min_charstrings_off_আকার)))
  {
    c->বের_করো_discard ();
    ফেরত মিথ্যা;
  }

  plan.info.char_strings_link = c->বের_করো_pack (মিথ্যা);
  ফেরত সত্য;
}

/* Serialize CID Charset (format 2 range: gid 0-N -> cid 0-N) */
স্থির যৌক্তিক
_serialize_cff1_charset (hb_serialize_context_t *c,
                         unsigned পূর্ণসংখ্যা অঙ্ক_glyphs,
                         objidx_t &charset_link)
{
  // For CID fonts, create a simple identity charset
  // Format 2: one range covering all glyphs (except .notdef)
  c->push ();

  auto *charset = c->start_embed<Charset> ();
  যদি (unlikely (!charset))
  {
    c->বের_করো_discard ();
    ফেরত মিথ্যা;
  }

  // Create a single range for CID 1 to num_glyphs-1
  hb_vector_t<code_pair_t> ranges;
  যদি (অঙ্ক_glyphs > 1)
  {
    code_pair_t range;
    range.code = 1;  // first CID
    range.glyph = অঙ্ক_glyphs - 2;  // nLeft (covers glyphs 1 to num_glyphs-1)
    ranges.push (range);
  }

  যদি (unlikely (!charset->serialize (c, 2, অঙ্ক_glyphs, ranges)))
  {
    c->বের_করো_discard ();
    ফেরত মিথ্যা;
  }

  charset_link = c->বের_করো_pack ();
  ফেরত সত্য;
}

/* CFF2 to CFF1 serialization */
নামাকাশ CFF {

যৌক্তিক
serialize_cff2_to_cff1 (hb_serialize_context_t *c,
                        OT::cff2_subset_plan &plan,
                        ধ্রুবক cff2_শীর্ষ_dict_মানসমূহ_t &cff2_শীর্ষ_Dict,
                        ধ্রুবক OT::cff2::accelerator_subset_t &acc)
{
  TRACE_SERIALIZE (এই);

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
    widths.alloc (plan.অঙ্ক_glyphs);

    জন্য (unsigned gid = 0; gid < plan.অঙ্ক_glyphs; gid++)
    {
      unsigned width = 0;
      যদি (plan.hmtx_map->has (gid))
        width = plan.hmtx_map->get (gid).first;
      widths.push (width);
    }

    // Optimize defaultWidthX and nominalWidthX
    CFF::optimize_widths (widths, plan.default_width, plan.nominal_width);
  }

  // 1. CharStrings (with widths prepended)
  যদি (!_serialize_cff1_charstrings (c, plan, plan.default_width, plan.nominal_width))
    return_trace (মিথ্যা);

  // 2. Private DICs & Local Subrs (same as CFF2)
  hb_vector_t<table_info_t> private_dict_infos;
  যদি (unlikely (!private_dict_infos.resize (plan.subset_fdcount)))
    return_trace (মিথ্যা);

  জন্য (পূর্ণসংখ্যা i = (পূর্ণসংখ্যা)acc.privateDicts.দৈর্ঘ্য; --i >= 0;)
  {
    যদি (plan.fdmap.has (i))
    {
      objidx_t subrs_link = 0;

      যদি (plan.subset_localsubrs[i].দৈর্ঘ্য > 0)
      {
        auto *dest = c->push<CFF1Subrs> ();
        যদি (likely (dest->serialize (c, plan.subset_localsubrs[i])))
          subrs_link = c->বের_করো_pack (মিথ্যা);
        অন্যথা
        {
          c->বের_করো_discard ();
          return_trace (মিথ্যা);
        }
      }

      auto *pd = c->push<PrivateDict> ();
      // Use the CFF2 Private DICT serializer which instantiates blends when pinned=true
      cff2_private_dict_op_serializer_t privSzr (plan.desubroutinize, plan.drop_hints, plan.pinned,
                                                  acc.varStore, plan.normalized_coords);
      যদি (likely (pd->serialize (c, acc.privateDicts[i], privSzr, subrs_link)))
      {
        // Add defaultWidthX and nominalWidthX for CFF1
        str_buff_t width_ops;
        str_encoder_t encoder (width_ops);
        encoder.encode_int (plan.default_width);
        encoder.encode_op (OpCode_defaultWidthX);
        encoder.encode_int (plan.nominal_width);
        encoder.encode_op (OpCode_nominalWidthX);

        যদি (!encoder.in_error () && c->embed (width_ops.as_bytes ().arrayZ, width_ops.দৈর্ঘ্য))
        {
          unsigned fd = plan.fdmap[i];
          private_dict_infos[fd].আকার_2 = c->দৈর্ঘ্য ();
          private_dict_infos[fd].link = c->বের_করো_pack ();
        }
        অন্যথা
        {
          c->বের_করো_discard ();
          return_trace (মিথ্যা);
        }
      }
      অন্যথা
      {
        c->বের_করো_discard ();
        return_trace (মিথ্যা);
      }
    }
  }

  // 3. FDArray - serialize CFF2 font dicts as CFF1
  {
    auto *fda = c->push<FDArray<HBUINT16>> ();
    cff_font_dict_op_serializer_t fontSzr;
    auto it =
    + hb_zip (+ hb_iter (acc.fontDicts)
              | hb_filter ([&] (ধ্রুবক cff2_font_dict_মানসমূহ_t &_)
                { ফেরত plan.fdmap.has (&_ - &acc.fontDicts[0]); }),
              hb_iter (private_dict_infos))
    ;
    // Explicitly specify template parameters: DICTVAL, INFO
    যৌক্তিক success = fda->serialize<cff2_font_dict_মানসমূহ_t, table_info_t> (c, it, fontSzr);
    যদি (success)
      plan.info.fd_array_link = c->বের_করো_pack (মিথ্যা);
    অন্যথা
    {
      c->বের_করো_discard ();
      return_trace (মিথ্যা);
    }
  }

  // 4. FDSelect (required in CFF1 CID-keyed fonts)
  // CFF1 requires FDSelect for all CID-keyed fonts, even with just one FD
  // CFF2 makes it optional when there's only one FD
  যদি (acc.fdSelect != &Null (CFF2FDSelect))
  {
    c->push ();
    যদি (likely (hb_serialize_cff_fdselect (c, plan.অঙ্ক_glyphs,
                                          *(ধ্রুবক FDSelect *)acc.fdSelect,
                                          plan.orig_fdcount,
                                          plan.subset_fdselect_format,
                                          plan.subset_fdselect_আকার,
                                          plan.subset_fdselect_ranges)))
      plan.info.fd_select.link = c->বের_করো_pack ();
    অন্যথা
    {
      c->বের_করো_discard ();
      return_trace (মিথ্যা);
    }
  }
  অন্যথা
  {
    // Create a range-based FDSelect3 mapping all glyphs to FD 0
    // Format: format(1) + nRanges(2) + range(3) + sentinel(2) = 8 bytes
    c->push ();

    // Format byte
    HBUINT8 format;
    format = 3;
    যদি (unlikely (!c->embed (format)))
    {
      c->বের_করো_discard ();
      return_trace (মিথ্যা);
    }

    // nRanges
    HBUINT16 nRanges;
    nRanges = 1;
    যদি (unlikely (!c->embed (nRanges)))
    {
      c->বের_করো_discard ();
      return_trace (মিথ্যা);
    }

    // Single range: {first: 0, fd: 0}
    FDSelect3_Range range;
    range.first = 0;
    range.fd = 0;
    যদি (unlikely (!c->embed (range)))
    {
      c->বের_করো_discard ();
      return_trace (মিথ্যা);
    }

    // Sentinel (number of glyphs)
    HBUINT16 sentinel;
    sentinel = plan.অঙ্ক_glyphs;
    যদি (unlikely (!c->embed (sentinel)))
    {
      c->বের_করো_discard ();
      return_trace (মিথ্যা);
    }

    plan.info.fd_select.link = c->বের_করো_pack ();
  }

  // 5. Charset (CID charset for identity mapping)
  objidx_t charset_link;
  যদি (!_serialize_cff1_charset (c, plan.অঙ্ক_glyphs, charset_link))
    return_trace (মিথ্যা);

  // 6. Global Subrs
  {
    auto *dest = c->push<CFF1Subrs> ();
    যদি (likely (dest->serialize (c, plan.subset_globalsubrs)))
      c->বের_করো_pack (মিথ্যা);
    অন্যথা
    {
      c->বের_করো_discard ();
      return_trace (মিথ্যা);
    }
  }

  // 7. String INDEX - Add "Adobe" and "Identity" for ROS operator
  {
    ধ্রুবক অক্ষর *adobe_str = "Adobe";
    ধ্রুবক অক্ষর *identity_str = "Identity";
    unsigned adobe_len = 5;  // strlen("Adobe")
    unsigned identity_len = 8;  // strlen("Identity")

    // Build strings array
    hb_vector_t<hb_ubytes_t> strings;
    strings.alloc (2);
    strings.push (hb_ubytes_t ((ধ্রুবক unsigned অক্ষর *) adobe_str, adobe_len));
    strings.push (hb_ubytes_t ((ধ্রুবক unsigned অক্ষর *) identity_str, identity_len));

    // Serialize as CFF INDEX
    auto *dest = c->push<CFF1_সূচক> ();
    যদি (likely (dest->serialize (c, strings)))
      c->বের_করো_pack (মিথ্যা);
    অন্যথা
    {
      c->বের_করো_discard ();
      return_trace (মিথ্যা);
    }
  }

  // 8. CFF Header
  OT::cff1 *cff = c->allocate_min<OT::cff1> ();
  যদি (unlikely (!cff)) return_trace (মিথ্যা);

  /* header */
  cff->version.major = 0x01;
  cff->version.minor = 0x00;
  cff->name_সূচক = cff->min_আকার;
  cff->off_আকার = 4; /* unused? */

  // 9. Name INDEX (single entry)
  {
    unsigned name_len = strlen (CFF1_DEFAULT_FONT_NAME);

    CFF1_সূচক *idx = c->start_embed<CFF1_সূচক> ();
    যদি (unlikely (!idx)) return_trace (মিথ্যা);

    যদি (unlikely (!idx->serialize_header (c, hb_iter (&name_len, 1), name_len)))
      return_trace (মিথ্যা);

    যদি (unlikely (!c->embed (CFF1_DEFAULT_FONT_NAME, name_len)))
      return_trace (মিথ্যা);
  }

  // 10. Top DICT INDEX
  {
    // Serialize the Top DICT data first
    c->push<শীর্ষ_Dict> ();
    cff1_from_cff2_শীর্ষ_dict_op_serializer_t শীর্ষ_Szr;

    // Serialize ROS first
    যদি (unlikely (!শীর্ষ_Szr.serialize_ros (c)))
    {
      c->বের_করো_discard ();
      return_trace (মিথ্যা);
    }

    // Serialize FontBBox from head table
    {
      str_buff_t bbox_buff;
      str_encoder_t encoder (bbox_buff);

      encoder.encode_int (plan.শীর্ষ_maxp_info.xMin);
      encoder.encode_int (plan.শীর্ষ_maxp_info.yMin);
      encoder.encode_int (plan.শীর্ষ_maxp_info.xMax);
      encoder.encode_int (plan.শীর্ষ_maxp_info.yMax);
      encoder.encode_op (OpCode_FontBBox);

      যদি (encoder.in_error () || !c->embed (bbox_buff.as_bytes ().arrayZ, bbox_buff.দৈর্ঘ্য))
      {
        c->বের_করো_discard ();
        return_trace (মিথ্যা);
      }
    }

    // Serialize charset operator
    যদি (charset_link && unlikely (!FontDict::serialize_link4_op (c, OpCode_charset, charset_link, whence_t::Absolute)))
    {
      c->বের_করো_discard ();
      return_trace (মিথ্যা);
    }

    // Serialize FDSelect operator (required for CID-keyed CFF1 fonts)
    যদি (plan.info.fd_select.link && unlikely (!FontDict::serialize_link4_op (c, OpCode_FDSelect, plan.info.fd_select.link, whence_t::Absolute)))
    {
      c->বের_করো_discard ();
      return_trace (মিথ্যা);
    }

    // Serialize FDArray operator (required for CID-keyed CFF1 fonts)
    যদি (plan.info.fd_array_link && unlikely (!FontDict::serialize_link4_op (c, OpCode_FDArray, plan.info.fd_array_link, whence_t::Absolute)))
    {
      c->বের_করো_discard ();
      return_trace (মিথ্যা);
    }

    // Serialize other operators from CFF2 TopDict
    জন্য (ধ্রুবক auto &opstr : cff2_শীর্ষ_Dict.values)
    {
      যদি (unlikely (!শীর্ষ_Szr.serialize (c, opstr, plan.info)))
      {
        c->বের_করো_discard ();
        return_trace (মিথ্যা);
      }
    }

    unsigned শীর্ষ_আকার = c->দৈর্ঘ্য ();
    c->বের_করো_pack (মিথ্যা);

    // Serialize INDEX header
    auto *dest = c->start_embed<CFF1_সূচক> ();
    যদি (unlikely (!dest->serialize_header (c, hb_iter (&শীর্ষ_আকার, 1), শীর্ষ_আকার)))
      return_trace (মিথ্যা);
  }

  return_trace (সত্য);
}

} /* namespace CFF */

স্থির যৌক্তিক _serialize_cff2_charstrings (hb_serialize_context_t *c,
			     cff2_subset_plan &plan,
			     ধ্রুবক OT::cff2::accelerator_subset_t  &acc)
{
  c->push ();

  unsigned তথ্য_আকার = 0;
  unsigned যোগ_আকার = CFF2CharStrings::যোগ_আকার (plan.subset_charstrings, &তথ্য_আকার, plan.min_charstrings_off_আকার);
  যদি (unlikely (!c->start_zerocopy (যোগ_আকার)))
    ফেরত মিথ্যা;

  auto *cs = c->start_embed<CFF2CharStrings> ();
  যদি (unlikely (!cs->serialize (c, plan.subset_charstrings, &তথ্য_আকার, plan.min_charstrings_off_আকার)))
  {
    c->বের_করো_discard ();
    ফেরত মিথ্যা;
  }

  plan.info.char_strings_link = c->বের_করো_pack (মিথ্যা);
  ফেরত সত্য;
}

যৌক্তিক
OT::cff2::accelerator_subset_t::serialize (hb_serialize_context_t *c,
					   কাঠামো cff2_subset_plan &plan,
					   hb_array_t<পূর্ণসংখ্যা> normalized_coords) ধ্রুবক
{
  /* push charstrings onto the object stack first which will ensure it packs as the last
     object in the table. Keeping the chastrings last satisfies the requirements for patching
     via IFTB. If this ordering needs to be changed in the future, charstrings should be left
     at the end whenever HB_SUBSET_FLAGS_ITFB_REQUIREMENTS is enabled. */
  যদি (!_serialize_cff2_charstrings(c, plan, *এই))
    ফেরত মিথ্যা;

  /* private dicts & local subrs */
  hb_vector_t<table_info_t>  private_dict_infos;
  যদি (unlikely (!private_dict_infos.resize (plan.subset_fdcount))) ফেরত মিথ্যা;

  জন্য (পূর্ণসংখ্যা i = (পূর্ণসংখ্যা)privateDicts.দৈর্ঘ্য; --i >= 0 ;)
  {
    যদি (plan.fdmap.has (i))
    {
      objidx_t	subrs_link = 0;

      যদি (plan.subset_localsubrs[i].দৈর্ঘ্য > 0)
      {
	auto *dest = c->push <CFF2Subrs> ();
	যদি (likely (dest->serialize (c, plan.subset_localsubrs[i])))
	  subrs_link = c->বের_করো_pack (মিথ্যা);
	অন্যথা
	{
	  c->বের_করো_discard ();
	  ফেরত মিথ্যা;
	}
      }
      auto *pd = c->push<PrivateDict> ();
      cff2_private_dict_op_serializer_t privSzr (plan.desubroutinize, plan.drop_hints, plan.pinned,
						 varStore, normalized_coords,
						 plan.partial_instancing ? &plan.instancing : nullptr);
      যদি (likely (pd->serialize (c, privateDicts[i], privSzr, subrs_link)))
      {
	unsigned fd = plan.fdmap[i];
	private_dict_infos[fd].আকার_2 = c->দৈর্ঘ্য ();
	private_dict_infos[fd].link = c->বের_করো_pack ();
      }
      অন্যথা
      {
	c->বের_করো_discard ();
	ফেরত মিথ্যা;
      }
    }
  }

  /* FDSelect */
  যদি (fdSelect != &Null (CFF2FDSelect))
  {
    c->push ();
    যদি (likely (hb_serialize_cff_fdselect (c, plan.অঙ্ক_glyphs, *(ধ্রুবক FDSelect *)fdSelect,
					   plan.orig_fdcount,
					   plan.subset_fdselect_format, plan.subset_fdselect_আকার,
					   plan.subset_fdselect_ranges)))
      plan.info.fd_select.link = c->বের_করো_pack ();
    অন্যথা
    {
      c->বের_করো_discard ();
      ফেরত মিথ্যা;
    }
  }

  /* FDArray (FD Index) */
  {
    auto *fda = c->push<CFF2FDArray> ();
    cff_font_dict_op_serializer_t fontSzr;
    auto it =
    + hb_zip (+ hb_iter (fontDicts)
	      | hb_filter ([&] (ধ্রুবক cff2_font_dict_মানসমূহ_t &_)
		{ ফেরত plan.fdmap.has (&_ - &fontDicts[0]); }),
	      hb_iter (private_dict_infos))
    ;
    যদি (unlikely (!fda->serialize (c, it, fontSzr)))
    {
      c->বের_করো_discard ();
      ফেরত মিথ্যা;
    }
    plan.info.fd_array_link = c->বের_করো_pack (মিথ্যা);
  }

  /* variation store */
  যদি (varStore != &Null (CFF2ItemVariationStore) &&
      !plan.pinned)
  {
    যদি (plan.partial_instancing)
    {
      যদি (plan.instancing.has_variations ())
      {
	c->push ();
	যদি (unlikely (!plan.instancing.serialize_var_store (c, *plan.axis_tags)))
	{
	  c->বের_করো_discard ();
	  ফেরত মিথ্যা;
	}
	plan.info.var_store_link = c->বের_করো_pack (মিথ্যা);
      }
      /* else: the store emptied; the vstore op gets dropped along with it. */
    }
    অন্যথা
    {
      auto *dest = c->push<CFF2ItemVariationStore> ();
      যদি (unlikely (!dest->serialize (c, varStore)))
      {
	c->বের_করো_discard ();
	ফেরত মিথ্যা;
      }
      plan.info.var_store_link = c->বের_করো_pack (মিথ্যা);
    }
  }

  OT::cff2 *cff2 = c->allocate_min<OT::cff2> ();
  যদি (unlikely (!cff2)) ফেরত মিথ্যা;

  /* header */
  cff2->version.major = 0x02;
  cff2->version.minor = 0x00;
  cff2->শীর্ষ_Dict_2 = OT::cff2::static_আকার;

  /* top dict */
  {
    শীর্ষ_Dict &dict = cff2 + cff2->শীর্ষ_Dict_2;
    cff2_শীর্ষ_dict_op_serializer_t শীর্ষ_Szr;
    যদি (unlikely (!dict.serialize (c, শীর্ষ_Dict_2, শীর্ষ_Szr, plan.info))) ফেরত মিথ্যা;
    cff2->শীর্ষ_Dict_আকার = c->শীর্ষ_2 - (ধ্রুবক অক্ষর *)&dict;
  }

  /* global subrs */
  {
    auto *dest = c->start_embed <CFF2Subrs> ();
    ফেরত dest->serialize (c, plan.subset_globalsubrs);
  }
}

যৌক্তিক
OT::cff2::accelerator_subset_t::subset (hb_subset_context_t *c) ধ্রুবক
{
  cff2_subset_plan cff2_plan;

  যদি (unlikely (!cff2_plan.create (*এই, c->plan))) ফেরত মিথ্যা;

  // If instantiating (pinned) and downgrade flag is set, convert to CFF1
  যদি (cff2_plan.pinned && (c->plan->flags & HB_SUBSET_FLAGS_DOWNGRADE_CFF2))
  {
    // Serialize CFF1 to the subsetter's serializer
    // If we run out of room, returning true will cause subsetter to retry with larger buffer
    যৌক্তিক ফলাফল = CFF::serialize_cff2_to_cff1 (c->serializer, cff2_plan, শীর্ষ_Dict_2, *এই);

    যদি (c->serializer->ran_out_of_room ())
      ফেরত সত্য; // Subsetter will retry with larger buffer

    যদি (ফলাফল && !c->serializer->in_error ())
    {
      // Success - end serialization to resolve links
      c->serializer->end_serialize ();

      // Copy the serialized CFF1 data and add as CFF table
      hb_blob_t *cff_blob = c->serializer->copy_blob ();
      যদি (cff_blob)
      {
        c->plan->add_table (HB_TAG('C','F','F',' '), cff_blob);
        hb_blob_destroy (cff_blob);

        // Return false to signal CFF2 table is not needed
        ফেরত মিথ্যা;
      }
    }

    // Conversion failed - don't fall back, fail hard for debugging
    ফেরত মিথ্যা;
  }

  ফেরত serialize (c->serializer, cff2_plan,
		    c->plan->normalized_coords.as_array ());
}

#endif
