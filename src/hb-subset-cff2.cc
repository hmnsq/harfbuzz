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

#ଅନ୍ତର୍ଭାଵ "hb.hh"

#ifndef HB_NO_SUBSET_CFF

#ଅନ୍ତର୍ଭାଵ "hb-open-type.hh"
#ଅନ୍ତର୍ଭାଵ "hb-ot-cff2-table.hh"
#ଅନ୍ତର୍ଭାଵ "hb-ot-var-common.hh"
#ଅନ୍ତର୍ଭାଵ "hb-set.h"
#ଅନ୍ତର୍ଭାଵ "hb-subset-plan.hh"
#ଅନ୍ତର୍ଭାଵ "hb-subset-cff-common.hh"
#ଅନ୍ତର୍ଭାଵ "hb-cff2-interp-cs.hh"
#ଅନ୍ତର୍ଭାଵ "hb-subset-cff2-to-cff1.hh"

using ନାମାକାଶ CFF;

ନାମାକାଶ CFF {

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
ସଂରଚନା cff2_instancing_plan_t
{
  ସଂରଚନା vardata_transform_t
  {
    /* new_deltas[j] = Σ_i matrix[j][i] * old_deltas[i] */
    hb_vector_t<hb_vector_t<ଦଶମିକ>> matrix;
    /* Default-value fold-in scalars, one per old region: the old store's
     * region scalars at the new default location. */
    hb_vector_t<ଦଶମିକ> gains;
    /* Surviving regions, parallel to matrix rows. */
    hb_vector_t<hb_hashmap_t<hb_tag_t, Triple>> regions;
    /* Index into the new global region list, per new region (finalize). */
    hb_vector_t<unsigned> region_indices;
    unsigned new_major = 0;
    ତାର୍କିକ alive_ = ମିଥ୍ୟା;
    ତାର୍କିକ alive () ଅଚଳ { ଫେରନ୍ତୁ alive_; }

    /* One value's delta for new region j, from its old per-region deltas. */
    ଦ୍ୱିଗୁଣ transform_delta (unsigned j, hb_array_t<ଅଚଳ ସଂଖ୍ୟା_t> deltas) ଅଚଳ
    {
      ଅଚଳ hb_vector_t<ଦଶମିକ> &col = matrix.arrayZ[j];
      ଦ୍ୱିଗୁଣ d = 0.;
      unsigned ଗଣନା_2 = hb_min (col.length, deltas.length);
      ପାଇଁ (unsigned c = 0; c < ଗଣନା_2; c++)
	d += (ଦ୍ୱିଗୁଣ) col.arrayZ[c] * deltas.arrayZ[c].to_real ();
      ଫେରନ୍ତୁ d;
    }
  };

  hb_vector_t<vardata_transform_t> transforms;  /* per old VarData (vsindex) */
  hb_vector_t<hb_hashmap_t<hb_tag_t, Triple>> new_regions;  /* global region list */
  hb_set_t used_majors;
  unsigned new_major_ଗଣନା = 0;

  ତାର୍କିକ has_variations () ଅଚଳ { ଫେରନ୍ତୁ new_major_ଗଣନା; }

  ଅଚଳ vardata_transform_t *get_transform (unsigned major) ଅଚଳ
  { ଫେରନ୍ତୁ major < transforms.length ? &transforms.arrayZ[major] : nullptr; }

  /* Record that a retained blend references this VarData. */
  ଶୂନ୍ୟ note_use (unsigned major) { used_majors.add (major); }

  /* The new vsindex a private dict's (or charstring's) old ivs maps to;
   * dead VarDatas fall back to 0, their blends having become static. */
  unsigned remap_ivs (unsigned old_ivs) ଅଚଳ
  {
    ଅଚଳ vardata_transform_t *t = get_transform (old_ivs);
    ଫେରନ୍ତୁ (t && t->alive ()) ? t->new_major : 0;
  }

  ଦ୍ୱିଗୁଣ fold_deltas (unsigned old_ivs, hb_array_t<ଅଚଳ ସଂଖ୍ୟା_t> deltas) ଅଚଳ
  {
    ଅଚଳ vardata_transform_t *t = get_transform (old_ivs);
    ଯଦି (unlikely (!t || t->gains.length != deltas.length)) ଫେରନ୍ତୁ 0.;
    ଦ୍ୱିଗୁଣ v = 0.;
    ପାଇଁ (unsigned i = 0; i < deltas.length; i++)
      v += (ଦ୍ୱିଗୁଣ) t->gains.arrayZ[i] * deltas.arrayZ[i].to_real ();
    ଫେରନ୍ତୁ v;
  }

  ତାର୍କିକ create (ଅଚଳ CFF2ItemVariationStore *cff2VarStore,
	       ଅଚଳ hb_subset_plan_t *plan)
  {
    ଅଚଳ OT::ItemVariationStore &varStore = cff2VarStore->varStore;
    unsigned major_ଗଣନା = varStore.get_sub_table_ଗଣନା ();
    ଯଦି (!transforms.resize (major_ଗଣନା)) ଫେରନ୍ତୁ ମିଥ୍ୟା;

    ଯଦି (!varStore.get_region_list ().get_var_regions (plan->axes_old_index_tag_map, orig_regions))
      ଫେରନ୍ତୁ ମିଥ୍ୟା;

    /* Instance each VarData with identity delta rows; the surviving tuples'
     * delta columns are the old-region → new-region transform. */
    OT::optimize_scratch_t scratch;
    ପାଇଁ (unsigned m = 0; m < major_ଗଣନା; m++)
    {
      ଅଚଳ OT::VarData &var_data = varStore.get_sub_table (m);
      unsigned k = var_data.get_region_index_ଗଣନା ();
      vardata_transform_t &t = transforms.arrayZ[m];

      ଯଦି (!t.gains.resize_exact (k)) ଫେରନ୍ତୁ ମିଥ୍ୟା;
      varStore.get_region_scalars (m, plan->normalized_coords.arrayZ,
				   plan->normalized_coords.length,
				   t.gains.arrayZ, k);

      OT::tuple_variations_t tuples;
      ପାଇଁ (unsigned r = 0; r < k; r++)
      {
	OT::tuple_delta_t tuple;
	ଯଦି (!tuple.deltas_x.resize (k) ||
	    !tuple.indices.resize (k))
	  ଫେରନ୍ତୁ ମିଥ୍ୟା;
	ପାଇଁ (unsigned i = 0; i < k; i++)
	  tuple.indices.arrayZ[i] = ସତ୍ୟ;
	tuple.deltas_x.arrayZ[r] = 1.f;
	unsigned region_index = var_data.get_region_index (r);
	ଯଦି (unlikely (region_index >= orig_regions.length)) ଫେରନ୍ତୁ ମିଥ୍ୟା;
	tuple.axis_tuples = orig_regions.arrayZ[region_index];
	ଯଦି (unlikely (tuple.axis_tuples.in_error ())) ଫେରନ୍ତୁ ମିଥ୍ୟା;
	tuples.tuple_vars.push (std::move (tuple));
      }
      ଯଦି (unlikely (tuples.tuple_vars.in_error ())) ଫେରନ୍ତୁ ମିଥ୍ୟା;

      ଯଦି (!tuples.instantiate (plan->axes_location, plan->axes_triple_distances, scratch))
	ଫେରନ୍ତୁ ମିଥ୍ୟା;

      ପାଇଁ (auto &tuple : tuples.tuple_vars)
      {
	ଯଦି (unlikely (tuple.deltas_x.length != k)) ଫେରନ୍ତୁ ମିଥ୍ୟା;
	hb_vector_t<ଦଶମିକ> col;
	ଯଦି (unlikely (!col.resize_exact (k))) ଫେରନ୍ତୁ ମିଥ୍ୟା;
	ପାଇଁ (unsigned i = 0; i < k; i++)
	  col.arrayZ[i] = tuple.deltas_x.arrayZ[i];
	t.matrix.push (std::move (col));
	t.regions.push (std::move (tuple.axis_tuples));
      }
      ଯଦି (unlikely (t.matrix.in_error () || t.regions.in_error ())) ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }

    ଫେରନ୍ତୁ ସତ୍ୟ;
  }

  /* Decide which VarDatas survive, number them, and build the new global
   * region list: surviving original regions first, in their original order,
   * then solver-split new regions in order of appearance (fontTools'
   * rebuildRegions + prune_regions). */
  ତାର୍କିକ finalize ()
  {
    ପାଇଁ (unsigned m = 0; m < transforms.length; m++)
    {
      vardata_transform_t &t = transforms.arrayZ[m];
      t.alive_ = t.matrix.length && used_majors.has (m);
      ଯଦି (t.alive_)
	t.new_major = new_major_ଗଣନା++;
    }

    /* Keys hash by content, so pointers into different owners compare fine. */
    hb_hashmap_t<ଅଚଳ hb_hashmap_t<hb_tag_t, Triple>*, unsigned> region_idx_map;
    hb_vector_t<ଅଚଳ hb_hashmap_t<hb_tag_t, Triple>*> region_ptrs;
    ପାଇଁ (ଅଚଳ vardata_transform_t &t : transforms)
    {
      ଯଦି (!t.alive ()) ଜାରି;
      ପାଇଁ (ଅଚଳ auto &region : t.regions)
	ଯଦି (!region_idx_map.has (&region))
	  ଯଦି (unlikely (!region_idx_map.set (&region, (unsigned) -1)))
	    ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }
    ପାଇଁ (ଅଚଳ auto &r : orig_regions)
    {
      unsigned *idx;
      ଯଦି (region_idx_map.has (&r, &idx) && *idx == (unsigned) -1)
      {
	ଯଦି (unlikely (!region_idx_map.set (&r, region_ptrs.length))) ଫେରନ୍ତୁ ମିଥ୍ୟା;
	region_ptrs.push (&r);
      }
    }
    ପାଇଁ (ଅଚଳ vardata_transform_t &t : transforms)
    {
      ଯଦି (!t.alive ()) ଜାରି;
      ପାଇଁ (ଅଚଳ auto &region : t.regions)
      {
	unsigned *idx;
	ଯଦି (region_idx_map.has (&region, &idx) && *idx == (unsigned) -1)
	{
	  ଯଦି (unlikely (!region_idx_map.set (&region, region_ptrs.length))) ଫେରନ୍ତୁ ମିଥ୍ୟା;
	  region_ptrs.push (&region);
	}
      }
    }
    ଯଦି (unlikely (region_ptrs.in_error ())) ଫେରନ୍ତୁ ମିଥ୍ୟା;

    ପାଇଁ (vardata_transform_t &t : transforms)
    {
      ଯଦି (!t.alive ()) ଜାରି;
      ପାଇଁ (ଅଚଳ auto &region : t.regions)
      {
	unsigned *idx;
	ଯଦି (unlikely (!region_idx_map.has (&region, &idx))) ଫେରନ୍ତୁ ମିଥ୍ୟା;
	t.region_indices.push (*idx);
      }
      ଯଦି (unlikely (t.region_indices.in_error ())) ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }

    ଯଦି (!new_regions.resize (region_ptrs.length)) ଫେରନ୍ତୁ ମିଥ୍ୟା;
    ପାଇଁ (unsigned i = 0; i < region_ptrs.length; i++)
    {
      new_regions.arrayZ[i] = *region_ptrs.arrayZ[i];
      ଯଦି (unlikely (new_regions.arrayZ[i].in_error ())) ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }

    ଫେରନ୍ତୁ ସତ୍ୟ;
  }

  /* Serialize the instanced store: a CFF2ItemVariationStore whose VarDatas
   * carry no items, just region indexes (blend deltas live in charstrings).
   * Laid out manually since all sizes are known up front. */
  ତାର୍କିକ serialize_var_store (hb_serialize_context_t *c,
			    ଅଚଳ hb_vector_t<hb_tag_t> &axis_tags) ଅଚଳ
  {
    unsigned data_ଗଣନା = new_major_ଗଣନା;
    unsigned header_size = 2 + 4 + 2 + 4 * data_ଗଣନା;

    uint64_t offset = header_size;
    hb_vector_t<unsigned> data_offsets;
    ପାଇଁ (ଅଚଳ vardata_transform_t &t : transforms)
    {
      ଯଦି (!t.alive ()) ଜାରି;
      data_offsets.push ((unsigned) offset);
      offset += 6 + 2 * (uint64_t) t.region_indices.length;
    }
    ଯଦି (unlikely (data_offsets.in_error () || data_offsets.length != data_ଗଣନା))
      ଫେରନ୍ତୁ ମିଥ୍ୟା;
    unsigned regions_offset = (unsigned) offset;
    uint64_t total = offset + 4 + (uint64_t) new_regions.length * axis_tags.length * 6;
    ଯଦି (unlikely (total > 0xFFFFFFFFu)) ଫେରନ୍ତୁ ମିଥ୍ୟା;

    HBUINT16 *size_field = c->allocate_size<HBUINT16> (HBUINT16::static_size);
    ଯଦି (unlikely (!size_field)) ଫେରନ୍ତୁ ମିଥ୍ୟା;
    /* OFF 2026 5.3.3.11.2 permits larger stores with length=65535. */
    *size_field = hb_min (total, (uint64_t) 0xFFFFu);

    ଅକ୍ଷର *header = c->allocate_size<ଅକ୍ଷର> (header_size);
    ଯଦି (unlikely (!header)) ଫେରନ୍ତୁ ମିଥ୍ୟା;
    * (HBUINT16 *) (header + 0) = 1;			/* format */
    * (HBUINT32 *) (header + 2) = regions_offset;
    * (HBUINT16 *) (header + 6) = data_ଗଣନା;
    ପାଇଁ (unsigned i = 0; i < data_ଗଣନା; i++)
      * (HBUINT32 *) (header + 8 + 4 * i) = data_offsets.arrayZ[i];

    ପାଇଁ (ଅଚଳ vardata_transform_t &t : transforms)
    {
      ଯଦି (!t.alive ()) ଜାରି;
      unsigned k = t.region_indices.length;
      HBUINT16 *v = (HBUINT16 *) c->allocate_size<ଅକ୍ଷର> (6 + 2 * k);
      ଯଦି (unlikely (!v)) ଫେରନ୍ତୁ ମିଥ୍ୟା;
      v[0] = 0;						/* itemCount */
      v[1] = 0;						/* wordSizeCount */
      v[2] = k;						/* regionIndices.len */
      ପାଇଁ (unsigned j = 0; j < k; j++)
	v[3 + j] = t.region_indices.arrayZ[j];
    }

    hb_vector_t<ଅଚଳ hb_hashmap_t<hb_tag_t, Triple>*> region_ptrs;
    ଯଦି (unlikely (!region_ptrs.alloc_exact (new_regions.length))) ଫେରନ୍ତୁ ମିଥ୍ୟା;
    ପାଇଁ (ଅଚଳ auto &r : new_regions)
      region_ptrs.push (&r);
    auto *region_list = c->start_embed<OT::VarRegionList> ();
    ଫେରନ୍ତୁ region_list->serialize (c, axis_tags, region_ptrs);
  }

  ବ୍ୟକ୍ତିଗତ:
  hb_vector_t<hb_hashmap_t<hb_tag_t, Triple>> orig_regions;
};

/* Usage-collection pass for partial instancing: record which VarDatas the
 * retained charstrings' and private dicts' blends reference.  Mirrors the
 * emission pass: blends consumed by dropped hint ops don't count. */
ସଂରଚନା cff2_usage_param_t
{
  ଶୂନ୍ୟ init () {}

  cff2_instancing_plan_t *instancing = nullptr;
  ତାର୍କିକ drop_hints = ମିଥ୍ୟା;
  unsigned ivs = 0;	/* dict-side current vsindex */
};

ସଂରଚନା cff2_cs_opset_usage_t : cff2_cs_opset_t<cff2_cs_opset_usage_t, cff2_usage_param_t, blend_arg_t>
{
  ସ୍ଥିର ଶୂନ୍ୟ flush_args_and_op (op_code_t op, cff2_cs_interp_env_t<blend_arg_t> &env, cff2_usage_param_t& param)
  {
    ତାର୍କିକ counts = ସତ୍ୟ;
    ବାଛ (op)
    {
      ସ୍ଥିତି OpCode_return:
      ସ୍ଥିତି OpCode_endchar:
	counts = ମିଥ୍ୟା;
	ଭାଙ୍ଗ;

      ସ୍ଥିତି OpCode_hstem:
      ସ୍ଥିତି OpCode_hstemhm:
      ସ୍ଥିତି OpCode_vstem:
      ସ୍ଥିତି OpCode_vstemhm:
      ସ୍ଥିତି OpCode_hintmask:
      ସ୍ଥିତି OpCode_cntrmask:
	ଯଦି (param.drop_hints)
	  counts = ମିଥ୍ୟା;
	ଭାଙ୍ଗ;

      ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
	ଭାଙ୍ଗ;
    }

    ଯଦି (counts)
      ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < env.arg_ଗଦା.get_ଗଣନା (); i++)
	ଯଦି (env.arg_ଗଦା[i].blending ())
	{
	  param.instancing->note_use (env.get_ivs ());
	  ଭାଙ୍ଗ;
	}

    env.clear_args ();
  }
};

ସଂରଚନା cff2_private_dict_usage_opset_t : dict_opset_t
{
  ସ୍ଥିର ଶୂନ୍ୟ process_op (op_code_t op, cff2_priv_dict_interp_env_t& env, cff2_usage_param_t& param)
  {
    ବାଛ (op) {
      ସ୍ଥିତି OpCode_vsindexdict:
	env.process_vsindex ();
	param.ivs = env.get_ivs ();
	env.clear_args ();
	ଫେରନ୍ତୁ;
      ସ୍ଥିତି OpCode_blenddict:
	param.instancing->note_use (param.ivs);
	env.clear_args ();
	ଫେରନ୍ତୁ;
      ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
	dict_opset_t::process_op (op, env);
	ଯଦି (!env.arg_ଗଦା.is_empty ()) ଫେରନ୍ତୁ;
	env.clear_args ();
	ଫେରନ୍ତୁ;
    }
  }
};

} /* namespace CFF */

ସଂରଚନା cff2_sub_table_info_t : cff_sub_table_info_t
{
  cff2_sub_table_info_t ()
    : cff_sub_table_info_t (),
      var_store_link (0)
  {}

  objidx_t  var_store_link;
};

ସଂରଚନା cff2_ଶୀର୍ଷ_dict_op_serializer_t : cff_ଶୀର୍ଷ_dict_op_serializer_t<>
{
  ତାର୍କିକ serialize (hb_serialize_context_t *c,
		  ଅଚଳ op_str_t &opstr,
		  ଅଚଳ cff2_sub_table_info_t &info) ଅଚଳ
  {
    TRACE_SERIALIZE (ଏହି);

    ବାଛ (opstr.op)
    {
      ସ୍ଥିତି OpCode_vstore:
        ଯଦି (info.var_store_link)
	  return_trace (FontDict::serialize_link4_op(c, opstr.op, info.var_store_link));
	ଅନ୍ୟଥା
	  return_trace (ସତ୍ୟ);

      ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
	return_trace (cff_ଶୀର୍ଷ_dict_op_serializer_t<>::serialize (c, opstr, info));
    }
  }
};

ସଂରଚନା cff2_cs_opset_flatten_t : cff2_cs_opset_t<cff2_cs_opset_flatten_t, flatten_param_t, blend_arg_t>
{
  ସ୍ଥିର ଶୂନ୍ୟ flush_args_and_op (op_code_t op, cff2_cs_interp_env_t<blend_arg_t> &env, flatten_param_t& param)
  {
    /* Optionally capture command for specialization (before flushing, to preserve args) */
    ଯଦି (param.commands)
    {
      ତାର୍କିକ skip_command = ମିଥ୍ୟା;

      ବାଛ (op)
      {
	ସ୍ଥିତି OpCode_return:
	ସ୍ଥିତି OpCode_endchar:
	  skip_command = ସତ୍ୟ;
	  ଭାଙ୍ଗ;

	ସ୍ଥିତି OpCode_hstem:
	ସ୍ଥିତି OpCode_hstemhm:
	ସ୍ଥିତି OpCode_vstem:
	ସ୍ଥିତି OpCode_vstemhm:
	ସ୍ଥିତି OpCode_hintmask:
	ସ୍ଥିତି OpCode_cntrmask:
	  ଯଦି (param.drop_hints)
	    skip_command = ସତ୍ୟ;
	  ଭାଙ୍ଗ;

	ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
	  ଭାଙ୍ଗ;
      }

      ଯଦି (!skip_command)
      {
	cs_command_t cmd (op);
	/* Capture resolved blend values */
	ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < env.arg_ଗଦା.get_ଗଣନା ();)
	{
	  ଅଚଳ blend_arg_t &arg = env.arg_ଗଦା[i];
	  ଯଦି (arg.blending ())
	  {
	    /* For blend args, capture only the resolved default value */
	    cmd.args.push (arg);
	    /* Skip over the multiple blend values */
	    i += arg.ଅଙ୍କ_ମୂଲ୍ୟଗୁଡ଼ିକ_2;
	  }
	  ଅନ୍ୟଥା
	  {
	    cmd.args.push (arg);
	    i++;
	  }
	}
	param.commands->push (cmd);
      }
    }

    ବାଛ (op)
    {
      ସ୍ଥିତି OpCode_return:
      ସ୍ଥିତି OpCode_endchar:
	/* dummy opcodes in CFF2. ignore */
	ଯଦି (op == OpCode_endchar && param.instancer)
	  emit_vsindex_prefix (env, param);
	ଭାଙ୍ଗ;

      ସ୍ଥିତି OpCode_hstem:
      ସ୍ଥିତି OpCode_hstemhm:
      ସ୍ଥିତି OpCode_vstem:
      ସ୍ଥିତି OpCode_vstemhm:
      ସ୍ଥିତି OpCode_hintmask:
      ସ୍ଥିତି OpCode_cntrmask:
	ଯଦି (param.drop_hints)
	{
	  env.clear_args ();
	  ଫେରନ୍ତୁ;
	}
	HB_FALLTHROUGH;

      ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
	SUPER::flush_args_and_op (op, env, param);
	ଭାଙ୍ଗ;
    }
  }

  ସ୍ଥିର ଶୂନ୍ୟ flush_args (cff2_cs_interp_env_t<blend_arg_t> &env, flatten_param_t& param)
  {
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < env.arg_ଗଦା.get_ଗଣନା ();)
    {
      ଅଚଳ blend_arg_t &arg = env.arg_ଗଦା[i];
      ଯଦି (arg.blending ())
      {
	ଯଦି (unlikely (!((arg.ଅଙ୍କ_ମୂଲ୍ୟଗୁଡ଼ିକ_2 > 0) && (env.arg_ଗଦା.get_ଗଣନା () >= arg.ଅଙ୍କ_ମୂଲ୍ୟଗୁଡ଼ିକ_2))))
	{
	  env.set_error ();
	  ଫେରନ୍ତୁ;
	}
	ଯଦି (param.instancer)
	  rewrite_blends (arg, i, env, param);
	ଅନ୍ୟଥା
	  flatten_blends (arg, i, env, param);
	i += arg.ଅଙ୍କ_ମୂଲ୍ୟଗୁଡ଼ିକ_2;
      }
      ଅନ୍ୟଥା
      {
	str_encoder_t  encoder (param.flatStr);
	encoder.encode_ଅଙ୍କ_cs (arg);
	i++;
      }
    }
    SUPER::flush_args (env, param);
  }

  ସ୍ଥିର ଶୂନ୍ୟ flatten_blends (ଅଚଳ blend_arg_t &arg, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, cff2_cs_interp_env_t<blend_arg_t> &env, flatten_param_t& param)
  {
    /* flatten the default values */
    str_encoder_t  encoder (param.flatStr);
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0; j < arg.ଅଙ୍କ_ମୂଲ୍ୟଗୁଡ଼ିକ_2; j++)
    {
      ଅଚଳ blend_arg_t &arg1 = env.arg_ଗଦା[i + j];
      ଯଦି (unlikely (!((arg1.blending () && (arg.ଅଙ୍କ_ମୂଲ୍ୟଗୁଡ଼ିକ_2 == arg1.ଅଙ୍କ_ମୂଲ୍ୟଗୁଡ଼ିକ_2) && (arg1.ମୂଲ୍ୟ_Index_2 == j) &&
	      (arg1.deltas.length == env.get_region_ଗଣନା ())))))
      {
	env.set_error ();
	ଫେରନ୍ତୁ;
      }
      encoder.encode_ଅଙ୍କ_cs (arg1);
    }
    /* flatten deltas for each value */
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0; j < arg.ଅଙ୍କ_ମୂଲ୍ୟଗୁଡ଼ିକ_2; j++)
    {
      ଅଚଳ blend_arg_t &arg1 = env.arg_ଗଦା[i + j];
      ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = 0; k < arg1.deltas.length; k++)
	encoder.encode_ଅଙ୍କ_cs (arg1.deltas[k]);
    }
    /* flatten the number of values followed by blend operator */
    encoder.encode_int (arg.ଅଙ୍କ_ମୂଲ୍ୟଗୁଡ଼ିକ_2);
    encoder.encode_op (OpCode_blendcs);
  }

  /* Partial instancing: rewrite one blend group against the instanced store.
   * Each value's default absorbs the folded (pinned-away) contribution, and
   * its deltas are mapped onto the new region list. */
  ସ୍ଥିର ଶୂନ୍ୟ rewrite_blends (ଅଚଳ blend_arg_t &arg, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, cff2_cs_interp_env_t<blend_arg_t> &env, flatten_param_t& param)
  {
    ଅଚଳ cff2_instancing_plan_t &instancer = *param.instancer;
    unsigned ivs = env.get_ivs ();
    ଅଚଳ cff2_instancing_plan_t::vardata_transform_t *t = instancer.get_transform (ivs);
    ଯଦି (unlikely (!t))
    {
      env.set_error ();
      ଫେରନ୍ତୁ;
    }

    unsigned n = arg.ଅଙ୍କ_ମୂଲ୍ୟଗୁଡ଼ିକ_2;
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0; j < n; j++)
    {
      ଅଚଳ blend_arg_t &arg1 = env.arg_ଗଦା[i + j];
      ଯଦି (unlikely (!(arg1.blending () && (arg1.ଅଙ୍କ_ମୂଲ୍ୟଗୁଡ଼ିକ_2 == n) && (arg1.ମୂଲ୍ୟ_Index_2 == j) &&
		      (arg1.deltas.length == env.get_region_ଗଣନା ()) &&
		      (arg1.deltas.length == t->gains.length))))
      {
	env.set_error ();
	ଫେରନ୍ତୁ;
      }
    }

    str_encoder_t encoder (param.flatStr);
    unsigned k_new = t->alive () ? t->matrix.length : 0;

    ଯଦି (!k_new)
    {
      /* All of this VarData's regions died; blends dissolve into statics. */
      ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0; j < n; j++)
      {
	ଅଚଳ blend_arg_t &arg1 = env.arg_ଗଦା[i + j];
	ସଂଖ୍ୟା_t v;
	v.set_real (arg1.to_real () + round (instancer.fold_deltas (ivs, arg1.deltas.as_array ())));
	encoder.encode_ଅଙ୍କ_cs (v);
      }
      ଫେରନ୍ତୁ;
    }

    /* Emit in chunks so the output charstring stays within the argument
     * stack limit: values already emitted for this op, plus this chunk's
     * defaults, deltas and count, must not exceed 513. */
    unsigned done = 0;
    ଯେତେବେଳେ (done < n)
    {
      unsigned avail = 512 > (i + done) ? 512 - (i + done) : 0;
      unsigned chunk = hb_min (avail / (k_new + 1), n - done);
      ଯଦି (unlikely (!chunk))
      {
	env.set_error ();
	ଫେରନ୍ତୁ;
      }

      ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = done; j < done + chunk; j++)
      {
	ଅଚଳ blend_arg_t &arg1 = env.arg_ଗଦା[i + j];
	ସଂଖ୍ୟା_t v;
	v.set_real (arg1.to_real () + round (instancer.fold_deltas (ivs, arg1.deltas.as_array ())));
	encoder.encode_ଅଙ୍କ_cs (v);
      }
      ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = done; j < done + chunk; j++)
      {
	ଅଚଳ blend_arg_t &arg1 = env.arg_ଗଦା[i + j];
	ପାଇଁ (unsigned r = 0; r < k_new; r++)
	{
	  ସଂଖ୍ୟା_t v;
	  v.set_int (hb_clamp_to<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> (round (t->transform_delta (r, arg1.deltas.as_array ()))));
	  encoder.encode_ଅଙ୍କ_cs (v);
	}
      }
      encoder.encode_int (chunk);
      encoder.encode_op (OpCode_blendcs);
      done += chunk;
    }
    param.emitted_blend = ସତ୍ୟ;
  }

  /* Partial instancing: prepend a vsindex op when the charstring's
   * (remapped) ivs differs from its FD's (remapped) private-dict ivs. */
  ସ୍ଥିର ଶୂନ୍ୟ emit_vsindex_prefix (cff2_cs_interp_env_t<blend_arg_t> &env, flatten_param_t& param)
  {
    ଯଦି (!param.emitted_blend) ଫେରନ୍ତୁ;
    ଅଚଳ cff2_instancing_plan_t &instancer = *param.instancer;
    unsigned new_ivs = instancer.remap_ivs (env.get_ivs ());
    unsigned fd_ivs = instancer.remap_ivs (env.get_orig_ivs ());
    ଯଦି (new_ivs == fd_ivs) ଫେରନ୍ତୁ;

    str_buff_t prefix;
    str_encoder_t enc (prefix);
    enc.encode_int ((ପୂର୍ଣ୍ଣସଂଖ୍ୟା) new_ivs);
    enc.encode_op (OpCode_vsindexcs);

    str_buff_t &buf = param.flatStr;
    unsigned old_len = buf.length;
    ଯଦି (unlikely (prefix.in_error () || !buf.resize (old_len + prefix.length)))
    {
      env.set_error ();
      ଫେରନ୍ତୁ;
    }
    memmove (buf.arrayZ + prefix.length, buf.arrayZ, old_len);
    hb_memcpy (buf.arrayZ, prefix.arrayZ, prefix.length);
  }

  ସ୍ଥିର ଶୂନ୍ୟ flush_op (op_code_t op, cff2_cs_interp_env_t<blend_arg_t> &env, flatten_param_t& param)
  {
    ବାଛ (op)
    {
      ସ୍ଥିତି OpCode_return:
      ସ୍ଥିତି OpCode_endchar:
	ଫେରନ୍ତୁ;
      ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
	str_encoder_t  encoder (param.flatStr);
	encoder.encode_op (op);
    }
  }

  ସ୍ଥିର ଶୂନ୍ୟ flush_hintmask (op_code_t op, cff2_cs_interp_env_t<blend_arg_t> &env, flatten_param_t& param)
  {
    SUPER::flush_hintmask (op, env, param);
    /* Preserve hintmask payload in captured commands for specializer re-encoding. */
    ଯଦି (param.commands && !param.drop_hints && param.commands->length > 0)
    {
      auto &cmd = param.commands->tail ();
      ଯଦି (cmd.op == op)
      {
        cmd.mask_bytes.resize (env.hintmask_size);
        ଯଦି (unlikely (cmd.mask_bytes.in_error ()))
        {
          env.set_error ();
          ଫେରନ୍ତୁ;
        }
        ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < env.hintmask_size; i++)
          cmd.mask_bytes[i] = env.str_ref[i];
      }
    }
    ଯଦି (!param.drop_hints)
    {
      str_encoder_t  encoder (param.flatStr);
      ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < env.hintmask_size; i++)
	encoder.encode_byte (env.str_ref[i]);
    }
  }

  ବ୍ୟକ୍ତିଗତ:
  ପ୍ରକାରବ୍ୟାଖ୍ୟା cff2_cs_opset_t<cff2_cs_opset_flatten_t, flatten_param_t, blend_arg_t> SUPER;
  ପ୍ରକାରବ୍ୟାଖ୍ୟା cs_opset_t<blend_arg_t, cff2_cs_opset_flatten_t, cff2_cs_opset_flatten_t, cff2_cs_interp_env_t<blend_arg_t>, flatten_param_t> CSOPSET;
};

ସଂରଚନା cff2_cs_opset_subr_subset_t : cff2_cs_opset_t<cff2_cs_opset_subr_subset_t, subr_subset_param_t, blend_arg_t>
{
  ସ୍ଥିର ଶୂନ୍ୟ process_op (op_code_t op, cff2_cs_interp_env_t<blend_arg_t> &env, subr_subset_param_t& param)
  {
    ବାଛ (op) {

      ସ୍ଥିତି OpCode_return:
	param.current_parsed_str->flush_coalesced (env.str_ref);
	param.current_parsed_str->set_parsed ();
	env.return_from_subr ();
	param.set_current_str (env, ମିଥ୍ୟା);
	ଭାଙ୍ଗ;

      ସ୍ଥିତି OpCode_endchar:
	param.current_parsed_str->flush_coalesced (env.str_ref);
	param.current_parsed_str->set_parsed ();
	SUPER::process_op (op, env, param);
	ଭାଙ୍ଗ;

      ସ୍ଥିତି OpCode_callsubr:
	process_call_subr (op, CSType_LocalSubr, env, param, env.localSubrs, param.local_closure);
	ଭାଙ୍ଗ;

      ସ୍ଥିତି OpCode_callgsubr:
	process_call_subr (op, CSType_GlobalSubr, env, param, env.globalSubrs, param.global_closure);
	ଭାଙ୍ଗ;

      ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
	SUPER::process_op (op, env, param);
	param.current_parsed_str->add_op (op, env.str_ref);
	ଭାଙ୍ଗ;
    }
  }

  ସୁରକ୍ଷିତ:
  ସ୍ଥିର ଶୂନ୍ୟ process_call_subr (op_code_t op, cs_type_t type,
				 cff2_cs_interp_env_t<blend_arg_t> &env, subr_subset_param_t& param,
				 cff2_biased_subrs_t& subrs, hb_set_t *closure)
  {
    byte_str_ref_t    str_ref = env.str_ref;
    env.call_subr (subrs, type);
    param.current_parsed_str->add_call_op (op, str_ref, env.context.subr_ଅଙ୍କ_2);
    closure->add (env.context.subr_ଅଙ୍କ_2);
    param.set_current_str (env, ସତ୍ୟ);
  }

  ବ୍ୟକ୍ତିଗତ:
  ପ୍ରକାରବ୍ୟାଖ୍ୟା cff2_cs_opset_t<cff2_cs_opset_subr_subset_t, subr_subset_param_t, blend_arg_t> SUPER;
};

ସଂରଚନା cff2_subr_subsetter_t : subr_subsetter_t<cff2_subr_subsetter_t, CFF2Subrs, ଅଚଳ OT::cff2::accelerator_subset_t, cff2_cs_interp_env_t<blend_arg_t>, cff2_cs_opset_subr_subset_t>
{
  cff2_subr_subsetter_t (ଅଚଳ OT::cff2::accelerator_subset_t &acc_, ଅଚଳ hb_subset_plan_t *plan_)
    : subr_subsetter_t (acc_, plan_) {}

  ସ୍ଥିର ଶୂନ୍ୟ complete_parsed_str (cff2_cs_interp_env_t<blend_arg_t> &env, subr_subset_param_t& param, parsed_cs_str_t &charstring)
  {
    /* vsindex is inserted at the beginning of the charstring as necessary */
    ଯଦି (env.seen_vsindex ())
    {
      ସଂଖ୍ୟା_t  ivs;
      ivs.set_int ((ପୂର୍ଣ୍ଣସଂଖ୍ୟା)env.get_ivs ());
      charstring.set_prefix (ivs, OpCode_vsindexcs);
    }
  }
};

ସଂରଚନା cff2_private_blend_encoder_param_t
{
  cff2_private_blend_encoder_param_t (hb_serialize_context_t *c,
				      ଅଚଳ CFF2ItemVariationStore *varStore,
				      hb_array_t<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> normalized_coords) :
    c (c), varStore (varStore), normalized_coords (normalized_coords) {}

  ଶୂନ୍ୟ init () {}

  ଶୂନ୍ୟ process_blend ()
  {
    ଯଦି (!seen_blend)
    {
      region_ଗଣନା_2 = varStore->varStore.get_region_index_ଗଣନା (ivs);
      ଯଦି (unlikely (!scalars.resize_exact (region_ଗଣନା_2)))
      {
	ଯଦି (c) c->err (HB_SERIALIZE_ERROR_OTHER);
	seen_blend = ସତ୍ୟ;
	ଫେରନ୍ତୁ;
      }
      varStore->varStore.get_region_scalars (ivs, normalized_coords.arrayZ, normalized_coords.length,
					     &scalars[0], region_ଗଣନା_2);
      seen_blend = ସତ୍ୟ;
    }
  }

  ଦ୍ୱିଗୁଣ blend_deltas (hb_array_t<ଅଚଳ ସଂଖ୍ୟା_t> deltas) ଅଚଳ
  {
    ଦ୍ୱିଗୁଣ v = 0;
    ଯଦି (likely (scalars.length == deltas.length))
    {
      unsigned ଗଣନା_2 = scalars.length;
      ପାଇଁ (unsigned i = 0; i < ଗଣନା_2; i++)
	v += (ଦ୍ୱିଗୁଣ) scalars.arrayZ[i] * deltas.arrayZ[i].to_real ();
    }
    ଫେରନ୍ତୁ v;
  }


  hb_serialize_context_t *c = nullptr;
  ତାର୍କିକ seen_blend = ମିଥ୍ୟା;
  unsigned ivs = 0;
  unsigned region_ଗଣନା_2 = 0;
  hb_vector_t<ଦଶମିକ> scalars;
  ଅଚଳ	 CFF2ItemVariationStore *varStore = nullptr;
  hb_array_t<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> normalized_coords;

  /* CFF2 partial instancing: when set, blends are rewritten against the
   * instanced variation store instead of being flattened. */
  ଅଚଳ cff2_instancing_plan_t *instancer = nullptr;
  /* Error-compensated fold rounding state, per dict operator. */
  ଦ୍ୱିଗୁଣ fold_exact = 0.;
  ଦ୍ୱିଗୁଣ fold_emitted = 0.;
};

ସଂରଚନା cff2_private_dict_blend_opset_t : dict_opset_t
{
  ସ୍ଥିର ଶୂନ୍ୟ process_arg_blend (cff2_private_blend_encoder_param_t& param,
				 ସଂଖ୍ୟା_t &arg,
				 ଅଚଳ hb_array_t<ଅଚଳ ସଂଖ୍ୟା_t> blends,
				 unsigned n, unsigned i)
  {
    arg.set_int (round (arg.to_real () + param.blend_deltas (blends)));
  }

  /* The fold added to a value, error-compensated across the operator: most
   * blended private-dict values (BlueValues, StemSnap*, ...) are
   * delta-encoded, so independently rounded folds would accumulate in the
   * decoded absolutes; keeping the emitted cumulative fold equal to the
   * rounded exact cumulative fold bounds every absolute within ±0.5. */
  ସ୍ଥିର ଦ୍ୱିଗୁଣ fold (cff2_private_blend_encoder_param_t& param,
		      ଅଚଳ hb_array_t<ଅଚଳ ସଂଖ୍ୟା_t> blends)
  {
    ଦ୍ୱିଗୁଣ f = param.blend_deltas (blends);
    ଦ୍ୱିଗୁଣ emit = round (param.fold_exact + f) - param.fold_emitted;
    param.fold_exact += f;
    param.fold_emitted += emit;
    ଫେରନ୍ତୁ emit;
  }

  /* Partial instancing: emit the args preceding this blend group, then the
   * group itself rewritten against the instanced store, and clear the
   * stack.  The op's remaining args are flushed at process_op time. */
  ସ୍ଥିର ଶୂନ୍ୟ rewrite_blends (cff2_priv_dict_interp_env_t& env,
			      cff2_private_blend_encoder_param_t& param,
			      unsigned start, unsigned n, unsigned k)
  {
    ଅଚଳ cff2_instancing_plan_t &instancer = *param.instancer;
    ଅଚଳ cff2_instancing_plan_t::vardata_transform_t *t = instancer.get_transform (param.ivs);
    ଯଦି (unlikely (!t || t->gains.length != k))
    {
      env.set_error ();
      ଫେରନ୍ତୁ;
    }

    str_buff_t str;
    str_encoder_t encoder (str);

    ପାଇଁ (unsigned idx = 0; idx < start; idx++)
      encoder.encode_ଅଙ୍କ_tp (env.arg_ଗଦା[idx]);

    unsigned k_new = t->alive () ? t->matrix.length : 0;
    ଯଦି (!k_new)
    {
      /* This VarData died (or is unused by any retained charstring, which
       * can happen for FDs kept only for retain-gids holes); blends
       * dissolve into statics. */
      ପାଇଁ (unsigned j = 0; j < n; j++)
      {
	ଅଚଳ hb_array_t<ଅଚଳ ସଂଖ୍ୟା_t> blends = env.arg_ଗଦା.sub_array (start + n + (j * k), k);
	ସଂଖ୍ୟା_t v;
	v.set_real (env.arg_ଗଦା[start + j].to_real () + fold (param, blends));
	encoder.encode_ଅଙ୍କ_tp (v);
      }
    }
    ଅନ୍ୟଥା
    {
      unsigned done = 0;
      ଯେତେବେଳେ (done < n)
      {
	unsigned avail = 512 > (start + done) ? 512 - (start + done) : 0;
	unsigned chunk = hb_min (avail / (k_new + 1), n - done);
	ଯଦି (unlikely (!chunk))
	{
	  env.set_error ();
	  ଫେରନ୍ତୁ;
	}

	ପାଇଁ (unsigned j = done; j < done + chunk; j++)
	{
	  ଅଚଳ hb_array_t<ଅଚଳ ସଂଖ୍ୟା_t> blends = env.arg_ଗଦା.sub_array (start + n + (j * k), k);
	  ସଂଖ୍ୟା_t v;
	  v.set_real (env.arg_ଗଦା[start + j].to_real () + fold (param, blends));
	  encoder.encode_ଅଙ୍କ_tp (v);
	}
	ପାଇଁ (unsigned j = done; j < done + chunk; j++)
	{
	  ଅଚଳ hb_array_t<ଅଚଳ ସଂଖ୍ୟା_t> blends = env.arg_ଗଦା.sub_array (start + n + (j * k), k);
	  ପାଇଁ (unsigned r = 0; r < k_new; r++)
	  {
	    ସଂଖ୍ୟା_t v;
	    v.set_int (hb_clamp_to<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> (round (t->transform_delta (r, blends))));
	    encoder.encode_ଅଙ୍କ_tp (v);
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

  ସ୍ଥିର ଶୂନ୍ୟ process_blend (cff2_priv_dict_interp_env_t& env, cff2_private_blend_encoder_param_t& param)
  {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, k;

    param.process_blend ();
    k = param.region_ଗଣନା_2;
    n = env.arg_ଗଦା.କାଢ଼_uint ();

    unsigned ଗଣନା_2 = env.arg_ଗଦା.get_ଗଣନା ();
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା total;
    ଯଦି (unlikely (hb_unsigned_mul_overflows (k + 1, n, &total) || total > ଗଣନା_2))
    {
      env.set_error ();
      ଫେରନ୍ତୁ;
    }

    /* copy the blend values into blend array of the default values */
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା start = ଗଣନା_2 - total;
    /* let an obvious error case fail, but note CFF2 spec doesn't forbid n==0 */
    ଯଦି (unlikely (start > ଗଣନା_2))
    {
      env.set_error ();
      ଫେରନ୍ତୁ;
    }

    ଯଦି (param.instancer && k)
    {
      rewrite_blends (env, param, start, n, k);
      ଫେରନ୍ତୁ;
    }

    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < n; i++)
    {
      ଅଚଳ hb_array_t<ଅଚଳ ସଂଖ୍ୟା_t> blends = env.arg_ଗଦା.sub_array (start + n + (i * k), k);
      process_arg_blend (param, env.arg_ଗଦା[start + i], blends, n, i);
    }

    /* pop off blend values leaving default values now adorned with blend values */
    env.arg_ଗଦା.କାଢ଼ (k * n);
  }

  ସ୍ଥିର ଶୂନ୍ୟ process_op (op_code_t op, cff2_priv_dict_interp_env_t& env, cff2_private_blend_encoder_param_t& param)
  {
    ବାଛ (op) {
      ସ୍ଥିତି OpCode_StdHW:
      ସ୍ଥିତି OpCode_StdVW:
      ସ୍ଥିତି OpCode_BlueScale:
      ସ୍ଥିତି OpCode_BlueShift:
      ସ୍ଥିତି OpCode_BlueFuzz:
      ସ୍ଥିତି OpCode_ExpansionFactor:
      ସ୍ଥିତି OpCode_LanguageGroup:
      ସ୍ଥିତି Op_Code_Blue_ମୂଲ୍ୟଗୁଡ଼ିକ:
      ସ୍ଥିତି OpCode_OtherBlues:
      ସ୍ଥିତି OpCode_FamilyBlues:
      ସ୍ଥିତି OpCode_FamilyOtherBlues:
      ସ୍ଥିତି OpCode_StemSnapH:
      ସ୍ଥିତି OpCode_StemSnapV:
	ଭାଙ୍ଗ;
      ସ୍ଥିତି OpCode_vsindexdict:
	env.process_vsindex ();
	param.ivs = env.get_ivs ();
	ଯଦି (param.instancer)
	{
	  /* Re-emit with the remapped vsindex; 0 is the dict default. */
	  unsigned new_ivs = param.instancer->remap_ivs (param.ivs);
	  ଯଦି (new_ivs)
	  {
	    str_buff_t str;
	    str_encoder_t encoder (str);
	    encoder.encode_int ((ପୂର୍ଣ୍ଣସଂଖ୍ୟା) new_ivs);
	    encoder.encode_op (OpCode_vsindexdict);
	    auto bytes = str.as_bytes ();
	    param.c->embed (&bytes, bytes.length);
	  }
	}
	env.clear_args ();
	ଫେରନ୍ତୁ;
      ସ୍ଥିତି OpCode_blenddict:
	process_blend (env, param);
	ଫେରନ୍ତୁ;

      ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
	dict_opset_t::process_op (op, env);
	ଯଦି (!env.arg_ଗଦା.is_empty ()) ଫେରନ୍ତୁ;
	ଭାଙ୍ଗ;
    }

    ଯଦି (unlikely (env.in_error ())) ଫେରନ୍ତୁ;

    // Write args then op

    str_buff_t str;
    str_encoder_t encoder (str);

    unsigned ଗଣନା_2 = env.arg_ଗଦା.get_ଗଣନା ();
    ପାଇଁ (unsigned i = 0; i < ଗଣନା_2; i++)
      encoder.encode_ଅଙ୍କ_tp (env.arg_ଗଦା[i]);

    encoder.encode_op (op);

    auto bytes = str.as_bytes ();
    param.c->embed (&bytes, bytes.length);

    env.clear_args ();
    param.fold_exact = param.fold_emitted = 0.;
  }
};

ସଂରଚନା cff2_private_dict_op_serializer_t : op_serializer_t
{
  cff2_private_dict_op_serializer_t (ତାର୍କିକ desubroutinize_, ତାର୍କିକ drop_hints_, ତାର୍କିକ pinned_,
				     ଅଚଳ CFF::CFF2ItemVariationStore* varStore_,
				     hb_array_t<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> normalized_coords_,
				     ଅଚଳ cff2_instancing_plan_t *instancer_ = nullptr)
    : desubroutinize (desubroutinize_), drop_hints (drop_hints_), pinned (pinned_),
      param (nullptr, varStore_, normalized_coords_)
  { param.instancer = instancer_; }

  ତାର୍କିକ serialize (hb_serialize_context_t *c,
		  ଅଚଳ op_str_t &opstr,
		  objidx_t subrs_link) ଅଚଳ
  {
    TRACE_SERIALIZE (ଏହି);

    ଯଦି (drop_hints && dict_opset_t::is_hint_op (opstr.op))
      return_trace (ସତ୍ୟ);

    ଯଦି (opstr.op == OpCode_Subrs)
    {
      ଯଦି (desubroutinize || !subrs_link)
	return_trace (ସତ୍ୟ);
      ଅନ୍ୟଥା
	return_trace (FontDict::serialize_link2_op (c, opstr.op, subrs_link));
    }

    ଯଦି (pinned || param.instancer)
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

  ସୁରକ୍ଷିତ:
  ଅଚଳ ତାର୍କିକ desubroutinize;
  ଅଚଳ ତାର୍କିକ drop_hints;
  ଅଚଳ ତାର୍କିକ pinned;
  mutable cff2_private_blend_encoder_param_t param;
};



ନାମାକାଶ OT {
ସଂରଚନା cff2_subset_plan
{
  ତାର୍କିକ create (ଅଚଳ OT::cff2::accelerator_subset_t &acc,
	      hb_subset_plan_t *plan)
  {
    /* make sure notdef is first */
    hb_codepoint_t old_glyph;
    ଯଦି (!plan->old_gid_for_new_gid (0, &old_glyph) || (old_glyph != 0)) ଫେରନ୍ତୁ ମିଥ୍ୟା;

    ଅଙ୍କ_glyphs = plan->ଅଙ୍କ_output_glyphs ();
    orig_fdcount = acc.fdArray->ଗଣନା_2;

    drop_hints = plan->flags & HB_SUBSET_FLAGS_NO_HINTING;
    ତାର୍କିକ instancing_requested = (ତାର୍କିକ) plan->normalized_coords;
    pinned = instancing_requested && plan->all_axes_pinned;
    normalized_coords = plan->normalized_coords;
    head_maxp_info = plan->head_maxp_info;
    hmtx_map = &plan->hmtx_map;
    desubroutinize = plan->flags & HB_SUBSET_FLAGS_DESUBROUTINIZE ||
		     instancing_requested; // For instancing we need this path

    /* Partial instancing: some axes pinned or limited, others kept. */
    partial_instancing = instancing_requested && !pinned &&
			 acc.varStore != &Null (CFF2ItemVariationStore);
    ଯଦି (partial_instancing &&
	unlikely (!instancing.create (acc.varStore, plan)))
      ଫେରନ୍ତୁ ମିଥ୍ୟା;
    axis_tags = &plan->axis_tags;

    /* Enable command capture for CFF2→CFF1 conversion (for specialization) */
    capture_commands = pinned;

 #ifdef HB_EXPERIMENTAL_API
    min_charstrings_off_size = (plan->flags & HB_SUBSET_FLAGS_IFTB_REQUIREMENTS) ? 4 : 0;
 #ଅନ୍ୟଥା
    min_charstrings_off_size = 0;
 #endif

    ଯଦି (desubroutinize)
    {
      ଯଦି (partial_instancing)
      {
	/* Pass 1: collect which VarDatas the retained blends reference, so
	 * unused ones get pruned and the survivors renumbered. */
	hb_set_t used_fds;
	ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଅଙ୍କ_glyphs; i++)
	{
	  hb_codepoint_t glyph;
	  ଯଦି (!plan->old_gid_for_new_gid (i, &glyph))
	    ଜାରି;
	  ଅଚଳ hb_ubytes_t str = (*acc.charStrings)[glyph];
	  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd = CFF::subset_get_fd (acc, glyph);
	  ଯଦି (unlikely (fd >= acc.fd_ଗଣନା))
	    ଫେରନ୍ତୁ ମିଥ୍ୟା;
	  used_fds.add (fd);

	  cff2_cs_interp_env_t<blend_arg_t> env (str, acc, fd);
	  cs_interpreter_t<cff2_cs_interp_env_t<blend_arg_t>, cff2_cs_opset_usage_t, cff2_usage_param_t> interp (env);
	  cff2_usage_param_t param;
	  param.instancing = &instancing;
	  param.drop_hints = drop_hints;
	  ଯଦି (unlikely (!interp.interpret (param)))
	    ଫେରନ୍ତୁ ମିଥ୍ୟା;
	}
	ପାଇଁ (unsigned fd : used_fds)
	{
	  cff2_usage_param_t param;
	  param.instancing = &instancing;
	  unsigned ଗଣନା_2 = acc.privateDicts[fd].get_ଗଣନା ();
	  ପାଇଁ (unsigned j = 0; j < ଗଣନା_2; j++)
	  {
	    ଅଚଳ op_str_t &opstr = acc.privateDicts[fd][j];
	    ଯଦି (drop_hints && dict_opset_t::is_hint_op (opstr.op))
	      ଜାରି;
	    cff2_priv_dict_interp_env_t env {hb_ubytes_t (opstr.ptr, opstr.length)};
	    dict_interpreter_t<cff2_private_dict_usage_opset_t, cff2_usage_param_t, cff2_priv_dict_interp_env_t> interp (env);
	    ଯଦି (unlikely (!interp.interpret (param)))
	      ଫେରନ୍ତୁ ମିଥ୍ୟା;
	  }
	}
	ଯଦି (unlikely (!instancing.finalize ()))
	  ଫେରନ୍ତୁ ମିଥ୍ୟା;

	/* Pass 2: like subr_flattener_t, but blends stay symbolic (no coords
	 * fed to the env) and get rewritten against the instanced store. */
	ଯଦି (!subset_charstrings.resize_exact (ଅଙ୍କ_glyphs))
	  ଫେରନ୍ତୁ ମିଥ୍ୟା;
	ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଅଙ୍କ_glyphs; i++)
	{
	  hb_codepoint_t glyph;
	  ଯଦି (!plan->old_gid_for_new_gid (i, &glyph))
	    ଜାରି;
	  ଅଚଳ hb_ubytes_t str = (*acc.charStrings)[glyph];
	  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd = CFF::subset_get_fd (acc, glyph);
	  ଯଦି (unlikely (fd >= acc.fd_ଗଣନା))
	    ଫେରନ୍ତୁ ମିଥ୍ୟା;

	  cff2_cs_interp_env_t<blend_arg_t> env (str, acc, fd);
	  cs_interpreter_t<cff2_cs_interp_env_t<blend_arg_t>, cff2_cs_opset_flatten_t, flatten_param_t> interp (env);
	  flatten_param_t param = {
	    subset_charstrings.arrayZ[i],
	    (ତାର୍କିକ) (plan->flags & HB_SUBSET_FLAGS_NO_HINTING),
	    plan
	  };
	  param.instancer = &instancing;
	  ଯଦି (unlikely (!interp.interpret (param)))
	    ଫେରନ୍ତୁ ମିଥ୍ୟା;
	}
      }
      ଅନ୍ୟଥା
      {
      /* Flatten global & local subrs */
      subr_flattener_t<ଅଚଳ OT::cff2::accelerator_subset_t, cff2_cs_interp_env_t<blend_arg_t>, cff2_cs_opset_flatten_t>
		    flattener(acc, plan);

      /* Enable command capture if requested (for specialization) */
      ଯଦି (capture_commands)
      {
	ଯଦି (!charstring_commands.resize_exact (ଅଙ୍କ_glyphs))
	  ଫେରନ୍ତୁ ମିଥ୍ୟା;

	ଯଦି (!flattener.flatten (subset_charstrings, &charstring_commands))
	  ଫେରନ୍ତୁ ମିଥ୍ୟା;
      }
      ଅନ୍ୟଥା
      {
	ଯଦି (!flattener.flatten (subset_charstrings))
	  ଫେରନ୍ତୁ ମିଥ୍ୟା;
      }
      }
    }
    ଅନ୍ୟଥା
    {
      cff2_subr_subsetter_t	subr_subsetter (acc, plan);

      /* Subset subrs: collect used subroutines, leaving all unused ones behind */
      ଯଦି (!subr_subsetter.subset ())
	ଫେରନ୍ତୁ ମିଥ୍ୟା;

      /* encode charstrings, global subrs, local subrs with new subroutine numbers */
      ଯଦି (!subr_subsetter.encode_charstrings (subset_charstrings, !pinned))
	ଫେରନ୍ତୁ ମିଥ୍ୟା;

      ଯଦି (!subr_subsetter.encode_globalsubrs (subset_globalsubrs))
	ଫେରନ୍ତୁ ମିଥ୍ୟା;

      /* local subrs */
      ଯଦି (!subset_localsubrs.resize (orig_fdcount))
	ଫେରନ୍ତୁ ମିଥ୍ୟା;
      ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd = 0; fd < orig_fdcount; fd++)
      {
	subset_localsubrs[fd].init ();
	ଯଦି (!subr_subsetter.encode_localsubrs (fd, subset_localsubrs[fd]))
	  ଫେରନ୍ତୁ ମିଥ୍ୟା;
      }
    }

    /* FDSelect */
    ଯଦି (acc.fdSelect != &Null (CFF2FDSelect))
    {
      ଯଦି (unlikely (!hb_plan_subset_cff_fdselect (plan,
						  orig_fdcount,
						  *(ଅଚଳ FDSelect *)acc.fdSelect,
						  acc.ଅଙ୍କ_glyphs,
						  subset_fdcount,
						  subset_fdselect_size,
						  subset_fdselect_format,
						  subset_fdselect_ranges,
						  fdmap)))
	ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }
    ଅନ୍ୟଥା
      fdmap.identity (1);

    ଫେରନ୍ତୁ ସତ୍ୟ;
  }

  cff2_sub_table_info_t info;

  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା    ଅଙ୍କ_glyphs;
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା    orig_fdcount = 0;
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା    subset_fdcount = 1;
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା    subset_fdselect_size = 0;
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା    subset_fdselect_format = 0;
  ତାର୍କିକ            pinned = ମିଥ୍ୟା;
  hb_vector_t<code_pair_t>   subset_fdselect_ranges;

  hb_inc_bimap_t   fdmap;

  str_buff_vec_t	    subset_charstrings;
  str_buff_vec_t	    subset_globalsubrs;
  hb_vector_t<str_buff_vec_t> subset_localsubrs;

  ତାର୍କିକ	    drop_hints = ମିଥ୍ୟା;
  ତାର୍କିକ	    desubroutinize = ମିଥ୍ୟା;

  ତାର୍କିକ	    partial_instancing = ମିଥ୍ୟା;
  cff2_instancing_plan_t instancing;
  ଅଚଳ hb_vector_t<hb_tag_t> *axis_tags = nullptr;

  unsigned  min_charstrings_off_size = 0;

  hb_array_t<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> normalized_coords; // For instantiation
  head_maxp_info_t head_maxp_info;  // For FontBBox
  ଅଚଳ hb_hashmap_t<hb_codepoint_t, hb_pair_t<unsigned, ପୂର୍ଣ୍ଣସଂଖ୍ୟା>> *hmtx_map; // For widths

  // Width optimization results (for CFF1 conversion)
  unsigned default_width = 0;
  unsigned nominal_width = 0;

  // Command capture for specialization (CFF2→CFF1 conversion)
  ତାର୍କିକ capture_commands = ମିଥ୍ୟା;
  hb_vector_t<hb_vector_t<cs_command_t>> charstring_commands;
};
} // namespace OT

/*
 * CFF2 to CFF1 Converter Implementation
 */

#ଅନ୍ତର୍ଭାଵ "hb-cff-width-optimizer.hh"
#ଅନ୍ତର୍ଭାଵ "hb-cff-specializer.hh"

/* Serialize charstrings using CFF1 format with widths */
ସ୍ଥିର ତାର୍କିକ
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
  ଯଦି (unlikely (!cff1_charstrings.resize (plan.subset_charstrings.length)))
  {
    c->କାଢ଼_discard ();
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
  }

  ପାଇଁ (unsigned i = 0; i < plan.subset_charstrings.length; i++)
  {
    // Get width for this glyph from hmtx_map
    unsigned width = 0;
    ଯଦି (plan.hmtx_map->has (i))
      width = plan.hmtx_map->get (i).first;

    // Encode width if different from default
    str_encoder_t encoder (cff1_charstrings[i]);
    ଯଦି (width != default_width)
    {
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା delta = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) width - (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) nominal_width;
      encoder.encode_int (delta);
    }

    // Use specialized commands if available, otherwise use binary
    ଯଦି (plan.capture_commands && i < plan.charstring_commands.length &&
        plan.charstring_commands[i].length > 0)
    {
      // Specialize and encode commands
      auto &commands = plan.charstring_commands[i];
      CFF::specialize_commands (commands, 48);  /* maxstack=48 for CFF1 */
      ଯଦି (unlikely (!CFF::encode_commands (commands, cff1_charstrings[i])))
      {
        c->କାଢ଼_discard ();
        ଫେରନ୍ତୁ ମିଥ୍ୟା;
      }
    }
    ଅନ୍ୟଥା
    {
      // Use binary CharString
      ଅଚଳ str_buff_t &cs = plan.subset_charstrings[i];
      ପାଇଁ (unsigned j = 0; j < cs.length; j++)
        cff1_charstrings[i].push (cs[j]);
    }

    // Check if it already ends with endchar (0x0e) or return (0x0b)
    ଯଦି (cff1_charstrings[i].length == 0 ||
        (cff1_charstrings[i].tail () != 0x0e && cff1_charstrings[i].tail () != 0x0b))
    {
      // Append endchar operator
      ଯଦି (unlikely (!cff1_charstrings[i].ଠେଲ_or_fail (0x0e)))
      {
        c->କାଢ଼_discard ();
        ଫେରନ୍ତୁ ମିଥ୍ୟା;
      }
    }
  }

  unsigned data_size = 0;
  unsigned total_size = CFF1CharStrings::total_size (cff1_charstrings, &data_size, plan.min_charstrings_off_size);
  ଯଦି (unlikely (!c->start_zerocopy (total_size)))
  {
    c->କାଢ଼_discard ();
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
  }

  auto *cs = c->start_embed<CFF1CharStrings> ();
  ଯଦି (unlikely (!cs->serialize (c, cff1_charstrings, &data_size, plan.min_charstrings_off_size)))
  {
    c->କାଢ଼_discard ();
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
  }

  plan.info.char_strings_link = c->କାଢ଼_pack (ମିଥ୍ୟା);
  ଫେରନ୍ତୁ ସତ୍ୟ;
}

/* Serialize CID Charset (format 2 range: gid 0-N -> cid 0-N) */
ସ୍ଥିର ତାର୍କିକ
_serialize_cff1_charset (hb_serialize_context_t *c,
                         unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_glyphs,
                         objidx_t &charset_link)
{
  // For CID fonts, create a simple identity charset
  // Format 2: one range covering all glyphs (except .notdef)
  c->push ();

  auto *charset = c->start_embed<Charset> ();
  ଯଦି (unlikely (!charset))
  {
    c->କାଢ଼_discard ();
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
  }

  // Create a single range for CID 1 to num_glyphs-1
  hb_vector_t<code_pair_t> ranges;
  ଯଦି (ଅଙ୍କ_glyphs > 1)
  {
    code_pair_t range;
    range.code = 1;  // first CID
    range.glyph = ଅଙ୍କ_glyphs - 2;  // nLeft (covers glyphs 1 to num_glyphs-1)
    ranges.push (range);
  }

  ଯଦି (unlikely (!charset->serialize (c, 2, ଅଙ୍କ_glyphs, ranges)))
  {
    c->କାଢ଼_discard ();
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
  }

  charset_link = c->କାଢ଼_pack ();
  ଫେରନ୍ତୁ ସତ୍ୟ;
}

/* CFF2 to CFF1 serialization */
ନାମାକାଶ CFF {

ତାର୍କିକ
serialize_cff2_to_cff1 (hb_serialize_context_t *c,
                        OT::cff2_subset_plan &plan,
                        ଅଚଳ cff2_ଶୀର୍ଷ_dict_ମୂଲ୍ୟଗୁଡ଼ିକ_t &cff2_ଶୀର୍ଷ_Dict,
                        ଅଚଳ OT::cff2::accelerator_subset_t &acc)
{
  TRACE_SERIALIZE (ଏହି);

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
    widths.alloc (plan.ଅଙ୍କ_glyphs);

    ପାଇଁ (unsigned gid = 0; gid < plan.ଅଙ୍କ_glyphs; gid++)
    {
      unsigned width = 0;
      ଯଦି (plan.hmtx_map->has (gid))
        width = plan.hmtx_map->get (gid).first;
      widths.push (width);
    }

    // Optimize defaultWidthX and nominalWidthX
    CFF::optimize_widths (widths, plan.default_width, plan.nominal_width);
  }

  // 1. CharStrings (with widths prepended)
  ଯଦି (!_serialize_cff1_charstrings (c, plan, plan.default_width, plan.nominal_width))
    return_trace (ମିଥ୍ୟା);

  // 2. Private DICs & Local Subrs (same as CFF2)
  hb_vector_t<table_info_t> private_dict_infos;
  ଯଦି (unlikely (!private_dict_infos.resize (plan.subset_fdcount)))
    return_trace (ମିଥ୍ୟା);

  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)acc.privateDicts.length; --i >= 0;)
  {
    ଯଦି (plan.fdmap.has (i))
    {
      objidx_t subrs_link = 0;

      ଯଦି (plan.subset_localsubrs[i].length > 0)
      {
        auto *dest = c->push<CFF1Subrs> ();
        ଯଦି (likely (dest->serialize (c, plan.subset_localsubrs[i])))
          subrs_link = c->କାଢ଼_pack (ମିଥ୍ୟା);
        ଅନ୍ୟଥା
        {
          c->କାଢ଼_discard ();
          return_trace (ମିଥ୍ୟା);
        }
      }

      auto *pd = c->push<PrivateDict> ();
      // Use the CFF2 Private DICT serializer which instantiates blends when pinned=true
      cff2_private_dict_op_serializer_t privSzr (plan.desubroutinize, plan.drop_hints, plan.pinned,
                                                  acc.varStore, plan.normalized_coords);
      ଯଦି (likely (pd->serialize (c, acc.privateDicts[i], privSzr, subrs_link)))
      {
        // Add defaultWidthX and nominalWidthX for CFF1
        str_buff_t width_ops;
        str_encoder_t encoder (width_ops);
        encoder.encode_int (plan.default_width);
        encoder.encode_op (OpCode_defaultWidthX);
        encoder.encode_int (plan.nominal_width);
        encoder.encode_op (OpCode_nominalWidthX);

        ଯଦି (!encoder.in_error () && c->embed (width_ops.as_bytes ().arrayZ, width_ops.length))
        {
          unsigned fd = plan.fdmap[i];
          private_dict_infos[fd].size = c->length ();
          private_dict_infos[fd].link = c->କାଢ଼_pack ();
        }
        ଅନ୍ୟଥା
        {
          c->କାଢ଼_discard ();
          return_trace (ମିଥ୍ୟା);
        }
      }
      ଅନ୍ୟଥା
      {
        c->କାଢ଼_discard ();
        return_trace (ମିଥ୍ୟା);
      }
    }
  }

  // 3. FDArray - serialize CFF2 font dicts as CFF1
  {
    auto *fda = c->push<FDArray<HBUINT16>> ();
    cff_font_dict_op_serializer_t fontSzr;
    auto it =
    + hb_zip (+ hb_iter (acc.fontDicts)
              | hb_filter ([&] (ଅଚଳ cff2_font_dict_ମୂଲ୍ୟଗୁଡ଼ିକ_t &_)
                { ଫେରନ୍ତୁ plan.fdmap.has (&_ - &acc.fontDicts[0]); }),
              hb_iter (private_dict_infos))
    ;
    // Explicitly specify template parameters: DICTVAL, INFO
    ତାର୍କିକ success = fda->serialize<cff2_font_dict_ମୂଲ୍ୟଗୁଡ଼ିକ_t, table_info_t> (c, it, fontSzr);
    ଯଦି (success)
      plan.info.fd_array_link = c->କାଢ଼_pack (ମିଥ୍ୟା);
    ଅନ୍ୟଥା
    {
      c->କାଢ଼_discard ();
      return_trace (ମିଥ୍ୟା);
    }
  }

  // 4. FDSelect (required in CFF1 CID-keyed fonts)
  // CFF1 requires FDSelect for all CID-keyed fonts, even with just one FD
  // CFF2 makes it optional when there's only one FD
  ଯଦି (acc.fdSelect != &Null (CFF2FDSelect))
  {
    c->push ();
    ଯଦି (likely (hb_serialize_cff_fdselect (c, plan.ଅଙ୍କ_glyphs,
                                          *(ଅଚଳ FDSelect *)acc.fdSelect,
                                          plan.orig_fdcount,
                                          plan.subset_fdselect_format,
                                          plan.subset_fdselect_size,
                                          plan.subset_fdselect_ranges)))
      plan.info.fd_select.link = c->କାଢ଼_pack ();
    ଅନ୍ୟଥା
    {
      c->କାଢ଼_discard ();
      return_trace (ମିଥ୍ୟା);
    }
  }
  ଅନ୍ୟଥା
  {
    // Create a range-based FDSelect3 mapping all glyphs to FD 0
    // Format: format(1) + nRanges(2) + range(3) + sentinel(2) = 8 bytes
    c->push ();

    // Format byte
    HBUINT8 format;
    format = 3;
    ଯଦି (unlikely (!c->embed (format)))
    {
      c->କାଢ଼_discard ();
      return_trace (ମିଥ୍ୟା);
    }

    // nRanges
    HBUINT16 nRanges;
    nRanges = 1;
    ଯଦି (unlikely (!c->embed (nRanges)))
    {
      c->କାଢ଼_discard ();
      return_trace (ମିଥ୍ୟା);
    }

    // Single range: {first: 0, fd: 0}
    FDSelect3_Range range;
    range.first = 0;
    range.fd = 0;
    ଯଦି (unlikely (!c->embed (range)))
    {
      c->କାଢ଼_discard ();
      return_trace (ମିଥ୍ୟା);
    }

    // Sentinel (number of glyphs)
    HBUINT16 sentinel;
    sentinel = plan.ଅଙ୍କ_glyphs;
    ଯଦି (unlikely (!c->embed (sentinel)))
    {
      c->କାଢ଼_discard ();
      return_trace (ମିଥ୍ୟା);
    }

    plan.info.fd_select.link = c->କାଢ଼_pack ();
  }

  // 5. Charset (CID charset for identity mapping)
  objidx_t charset_link;
  ଯଦି (!_serialize_cff1_charset (c, plan.ଅଙ୍କ_glyphs, charset_link))
    return_trace (ମିଥ୍ୟା);

  // 6. Global Subrs
  {
    auto *dest = c->push<CFF1Subrs> ();
    ଯଦି (likely (dest->serialize (c, plan.subset_globalsubrs)))
      c->କାଢ଼_pack (ମିଥ୍ୟା);
    ଅନ୍ୟଥା
    {
      c->କାଢ଼_discard ();
      return_trace (ମିଥ୍ୟା);
    }
  }

  // 7. String INDEX - Add "Adobe" and "Identity" for ROS operator
  {
    ଅଚଳ ଅକ୍ଷର *adobe_str = "Adobe";
    ଅଚଳ ଅକ୍ଷର *identity_str = "Identity";
    unsigned adobe_len = 5;  // strlen("Adobe")
    unsigned identity_len = 8;  // strlen("Identity")

    // Build strings array
    hb_vector_t<hb_ubytes_t> strings;
    strings.alloc (2);
    strings.push (hb_ubytes_t ((ଅଚଳ unsigned ଅକ୍ଷର *) adobe_str, adobe_len));
    strings.push (hb_ubytes_t ((ଅଚଳ unsigned ଅକ୍ଷର *) identity_str, identity_len));

    // Serialize as CFF INDEX
    auto *dest = c->push<CFF1Index> ();
    ଯଦି (likely (dest->serialize (c, strings)))
      c->କାଢ଼_pack (ମିଥ୍ୟା);
    ଅନ୍ୟଥା
    {
      c->କାଢ଼_discard ();
      return_trace (ମିଥ୍ୟା);
    }
  }

  // 8. CFF Header
  OT::cff1 *cff = c->allocate_min<OT::cff1> ();
  ଯଦି (unlikely (!cff)) return_trace (ମିଥ୍ୟା);

  /* header */
  cff->version.major = 0x01;
  cff->version.minor = 0x00;
  cff->nameIndex = cff->min_size;
  cff->offSize = 4; /* unused? */

  // 9. Name INDEX (single entry)
  {
    unsigned name_len = strlen (CFF1_DEFAULT_FONT_NAME);

    CFF1Index *idx = c->start_embed<CFF1Index> ();
    ଯଦି (unlikely (!idx)) return_trace (ମିଥ୍ୟା);

    ଯଦି (unlikely (!idx->serialize_header (c, hb_iter (&name_len, 1), name_len)))
      return_trace (ମିଥ୍ୟା);

    ଯଦି (unlikely (!c->embed (CFF1_DEFAULT_FONT_NAME, name_len)))
      return_trace (ମିଥ୍ୟା);
  }

  // 10. Top DICT INDEX
  {
    // Serialize the Top DICT data first
    c->push<ଶୀର୍ଷ_Dict> ();
    cff1_from_cff2_ଶୀର୍ଷ_dict_op_serializer_t ଶୀର୍ଷ_Szr;

    // Serialize ROS first
    ଯଦି (unlikely (!ଶୀର୍ଷ_Szr.serialize_ros (c)))
    {
      c->କାଢ଼_discard ();
      return_trace (ମିଥ୍ୟା);
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

      ଯଦି (encoder.in_error () || !c->embed (bbox_buff.as_bytes ().arrayZ, bbox_buff.length))
      {
        c->କାଢ଼_discard ();
        return_trace (ମିଥ୍ୟା);
      }
    }

    // Serialize charset operator
    ଯଦି (charset_link && unlikely (!FontDict::serialize_link4_op (c, OpCode_charset, charset_link, whence_t::Absolute)))
    {
      c->କାଢ଼_discard ();
      return_trace (ମିଥ୍ୟା);
    }

    // Serialize FDSelect operator (required for CID-keyed CFF1 fonts)
    ଯଦି (plan.info.fd_select.link && unlikely (!FontDict::serialize_link4_op (c, OpCode_FDSelect, plan.info.fd_select.link, whence_t::Absolute)))
    {
      c->କାଢ଼_discard ();
      return_trace (ମିଥ୍ୟା);
    }

    // Serialize FDArray operator (required for CID-keyed CFF1 fonts)
    ଯଦି (plan.info.fd_array_link && unlikely (!FontDict::serialize_link4_op (c, OpCode_FDArray, plan.info.fd_array_link, whence_t::Absolute)))
    {
      c->କାଢ଼_discard ();
      return_trace (ମିଥ୍ୟା);
    }

    // Serialize other operators from CFF2 TopDict
    ପାଇଁ (ଅଚଳ auto &opstr : cff2_ଶୀର୍ଷ_Dict.values)
    {
      ଯଦି (unlikely (!ଶୀର୍ଷ_Szr.serialize (c, opstr, plan.info)))
      {
        c->କାଢ଼_discard ();
        return_trace (ମିଥ୍ୟା);
      }
    }

    unsigned ଶୀର୍ଷ_size = c->length ();
    c->କାଢ଼_pack (ମିଥ୍ୟା);

    // Serialize INDEX header
    auto *dest = c->start_embed<CFF1Index> ();
    ଯଦି (unlikely (!dest->serialize_header (c, hb_iter (&ଶୀର୍ଷ_size, 1), ଶୀର୍ଷ_size)))
      return_trace (ମିଥ୍ୟା);
  }

  return_trace (ସତ୍ୟ);
}

} /* namespace CFF */

ସ୍ଥିର ତାର୍କିକ _serialize_cff2_charstrings (hb_serialize_context_t *c,
			     cff2_subset_plan &plan,
			     ଅଚଳ OT::cff2::accelerator_subset_t  &acc)
{
  c->push ();

  unsigned data_size = 0;
  unsigned total_size = CFF2CharStrings::total_size (plan.subset_charstrings, &data_size, plan.min_charstrings_off_size);
  ଯଦି (unlikely (!c->start_zerocopy (total_size)))
    ଫେରନ୍ତୁ ମିଥ୍ୟା;

  auto *cs = c->start_embed<CFF2CharStrings> ();
  ଯଦି (unlikely (!cs->serialize (c, plan.subset_charstrings, &data_size, plan.min_charstrings_off_size)))
  {
    c->କାଢ଼_discard ();
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
  }

  plan.info.char_strings_link = c->କାଢ଼_pack (ମିଥ୍ୟା);
  ଫେରନ୍ତୁ ସତ୍ୟ;
}

ତାର୍କିକ
OT::cff2::accelerator_subset_t::serialize (hb_serialize_context_t *c,
					   ସଂରଚନା cff2_subset_plan &plan,
					   hb_array_t<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> normalized_coords) ଅଚଳ
{
  /* push charstrings onto the object stack first which will ensure it packs as the last
     object in the table. Keeping the chastrings last satisfies the requirements for patching
     via IFTB. If this ordering needs to be changed in the future, charstrings should be left
     at the end whenever HB_SUBSET_FLAGS_ITFB_REQUIREMENTS is enabled. */
  ଯଦି (!_serialize_cff2_charstrings(c, plan, *ଏହି))
    ଫେରନ୍ତୁ ମିଥ୍ୟା;

  /* private dicts & local subrs */
  hb_vector_t<table_info_t>  private_dict_infos;
  ଯଦି (unlikely (!private_dict_infos.resize (plan.subset_fdcount))) ଫେରନ୍ତୁ ମିଥ୍ୟା;

  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)privateDicts.length; --i >= 0 ;)
  {
    ଯଦି (plan.fdmap.has (i))
    {
      objidx_t	subrs_link = 0;

      ଯଦି (plan.subset_localsubrs[i].length > 0)
      {
	auto *dest = c->push <CFF2Subrs> ();
	ଯଦି (likely (dest->serialize (c, plan.subset_localsubrs[i])))
	  subrs_link = c->କାଢ଼_pack (ମିଥ୍ୟା);
	ଅନ୍ୟଥା
	{
	  c->କାଢ଼_discard ();
	  ଫେରନ୍ତୁ ମିଥ୍ୟା;
	}
      }
      auto *pd = c->push<PrivateDict> ();
      cff2_private_dict_op_serializer_t privSzr (plan.desubroutinize, plan.drop_hints, plan.pinned,
						 varStore, normalized_coords,
						 plan.partial_instancing ? &plan.instancing : nullptr);
      ଯଦି (likely (pd->serialize (c, privateDicts[i], privSzr, subrs_link)))
      {
	unsigned fd = plan.fdmap[i];
	private_dict_infos[fd].size = c->length ();
	private_dict_infos[fd].link = c->କାଢ଼_pack ();
      }
      ଅନ୍ୟଥା
      {
	c->କାଢ଼_discard ();
	ଫେରନ୍ତୁ ମିଥ୍ୟା;
      }
    }
  }

  /* FDSelect */
  ଯଦି (fdSelect != &Null (CFF2FDSelect))
  {
    c->push ();
    ଯଦି (likely (hb_serialize_cff_fdselect (c, plan.ଅଙ୍କ_glyphs, *(ଅଚଳ FDSelect *)fdSelect,
					   plan.orig_fdcount,
					   plan.subset_fdselect_format, plan.subset_fdselect_size,
					   plan.subset_fdselect_ranges)))
      plan.info.fd_select.link = c->କାଢ଼_pack ();
    ଅନ୍ୟଥା
    {
      c->କାଢ଼_discard ();
      ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }
  }

  /* FDArray (FD Index) */
  {
    auto *fda = c->push<CFF2FDArray> ();
    cff_font_dict_op_serializer_t fontSzr;
    auto it =
    + hb_zip (+ hb_iter (fontDicts)
	      | hb_filter ([&] (ଅଚଳ cff2_font_dict_ମୂଲ୍ୟଗୁଡ଼ିକ_t &_)
		{ ଫେରନ୍ତୁ plan.fdmap.has (&_ - &fontDicts[0]); }),
	      hb_iter (private_dict_infos))
    ;
    ଯଦି (unlikely (!fda->serialize (c, it, fontSzr)))
    {
      c->କାଢ଼_discard ();
      ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }
    plan.info.fd_array_link = c->କାଢ଼_pack (ମିଥ୍ୟା);
  }

  /* variation store */
  ଯଦି (varStore != &Null (CFF2ItemVariationStore) &&
      !plan.pinned)
  {
    ଯଦି (plan.partial_instancing)
    {
      ଯଦି (plan.instancing.has_variations ())
      {
	c->push ();
	ଯଦି (unlikely (!plan.instancing.serialize_var_store (c, *plan.axis_tags)))
	{
	  c->କାଢ଼_discard ();
	  ଫେରନ୍ତୁ ମିଥ୍ୟା;
	}
	plan.info.var_store_link = c->କାଢ଼_pack (ମିଥ୍ୟା);
      }
      /* else: the store emptied; the vstore op gets dropped along with it. */
    }
    ଅନ୍ୟଥା
    {
      auto *dest = c->push<CFF2ItemVariationStore> ();
      ଯଦି (unlikely (!dest->serialize (c, varStore)))
      {
	c->କାଢ଼_discard ();
	ଫେରନ୍ତୁ ମିଥ୍ୟା;
      }
      plan.info.var_store_link = c->କାଢ଼_pack (ମିଥ୍ୟା);
    }
  }

  OT::cff2 *cff2 = c->allocate_min<OT::cff2> ();
  ଯଦି (unlikely (!cff2)) ଫେରନ୍ତୁ ମିଥ୍ୟା;

  /* header */
  cff2->version.major = 0x02;
  cff2->version.minor = 0x00;
  cff2->ଶୀର୍ଷ_Dict_2 = OT::cff2::static_size;

  /* top dict */
  {
    ଶୀର୍ଷ_Dict &dict = cff2 + cff2->ଶୀର୍ଷ_Dict_2;
    cff2_ଶୀର୍ଷ_dict_op_serializer_t ଶୀର୍ଷ_Szr;
    ଯଦି (unlikely (!dict.serialize (c, ଶୀର୍ଷ_Dict_2, ଶୀର୍ଷ_Szr, plan.info))) ଫେରନ୍ତୁ ମିଥ୍ୟା;
    cff2->ଶୀର୍ଷ_Dict_Size = c->head - (ଅଚଳ ଅକ୍ଷର *)&dict;
  }

  /* global subrs */
  {
    auto *dest = c->start_embed <CFF2Subrs> ();
    ଫେରନ୍ତୁ dest->serialize (c, plan.subset_globalsubrs);
  }
}

ତାର୍କିକ
OT::cff2::accelerator_subset_t::subset (hb_subset_context_t *c) ଅଚଳ
{
  cff2_subset_plan cff2_plan;

  ଯଦି (unlikely (!cff2_plan.create (*ଏହି, c->plan))) ଫେରନ୍ତୁ ମିଥ୍ୟା;

  // If instantiating (pinned) and downgrade flag is set, convert to CFF1
  ଯଦି (cff2_plan.pinned && (c->plan->flags & HB_SUBSET_FLAGS_DOWNGRADE_CFF2))
  {
    // Serialize CFF1 to the subsetter's serializer
    // If we run out of room, returning true will cause subsetter to retry with larger buffer
    ତାର୍କିକ result = CFF::serialize_cff2_to_cff1 (c->serializer, cff2_plan, ଶୀର୍ଷ_Dict_2, *ଏହି);

    ଯଦି (c->serializer->ran_out_of_room ())
      ଫେରନ୍ତୁ ସତ୍ୟ; // Subsetter will retry with larger buffer

    ଯଦି (result && !c->serializer->in_error ())
    {
      // Success - end serialization to resolve links
      c->serializer->end_serialize ();

      // Copy the serialized CFF1 data and add as CFF table
      hb_blob_t *cff_blob = c->serializer->copy_blob ();
      ଯଦି (cff_blob)
      {
        c->plan->add_table (HB_TAG('C','F','F',' '), cff_blob);
        hb_blob_destroy (cff_blob);

        // Return false to signal CFF2 table is not needed
        ଫେରନ୍ତୁ ମିଥ୍ୟା;
      }
    }

    // Conversion failed - don't fall back, fail hard for debugging
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
  }

  ଫେରନ୍ତୁ serialize (c->serializer, cff2_plan,
		    c->plan->normalized_coords.as_array ());
}

#endif
