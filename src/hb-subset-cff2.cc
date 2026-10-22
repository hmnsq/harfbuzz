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

#ಅನ್ತರ್ಭಾವ "hb.hh"

#ifndef HB_NO_SUBSET_CFF

#ಅನ್ತರ್ಭಾವ "hb-open-type.hh"
#ಅನ್ತರ್ಭಾವ "hb-ot-cff2-table.hh"
#ಅನ್ತರ್ಭಾವ "hb-ot-var-common.hh"
#ಅನ್ತರ್ಭಾವ "hb-set.h"
#ಅನ್ತರ್ಭಾವ "hb-subset-plan.hh"
#ಅನ್ತರ್ಭಾವ "hb-subset-cff-common.hh"
#ಅನ್ತರ್ಭಾವ "hb-cff2-interp-cs.hh"
#ಅನ್ತರ್ಭಾವ "hb-subset-cff2-to-cff1.hh"

using ನಾಮಾಕಾಶ CFF;

ನಾಮಾಕಾಶ CFF {

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
ಸಂರಚನಾ cff2_instancing_plan_t
{
  ಸಂರಚನಾ vardata_transform_t
  {
    /* new_deltas[j] = Σ_i matrix[j][i] * old_deltas[i] */
    hb_vector_t<hb_vector_t<ಭಿನ್ನ>> matrix;
    /* Default-value fold-in scalars, one per old region: the old store's
     * region scalars at the new default location. */
    hb_vector_t<ಭಿನ್ನ> gains;
    /* Surviving regions, parallel to matrix rows. */
    hb_vector_t<hb_hashmap_t<hb_tag_t, Triple>> regions;
    /* Index into the new global region list, per new region (finalize). */
    hb_vector_t<unsigned> region_indices;
    unsigned new_major = 0;
    ತಾರ್ಕಿಕ alive_ = ಅಸತ್ಯ;
    ತಾರ್ಕಿಕ alive () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ alive_; }

    /* One value's delta for new region j, from its old per-region deltas. */
    ದ್ವಿಗುಣ transform_delta (unsigned j, hb_array_t<ಧ್ರುವ ಸಂಖ್ಯೆ_t> deltas) ಧ್ರುವ
    {
      ಧ್ರುವ hb_vector_t<ಭಿನ್ನ> &col = matrix.arrayZ[j];
      ದ್ವಿಗುಣ d = 0.;
      unsigned ಎಣಿಕೆ = hb_min (col.length, deltas.length);
      ಪ್ರತಿ (unsigned c = 0; c < ಎಣಿಕೆ; c++)
	d += (ದ್ವಿಗುಣ) col.arrayZ[c] * deltas.arrayZ[c].to_real ();
      ಪ್ರತ್ಯಾಗಮ d;
    }
  };

  hb_vector_t<vardata_transform_t> transforms;  /* per old VarData (vsindex) */
  hb_vector_t<hb_hashmap_t<hb_tag_t, Triple>> new_regions;  /* global region list */
  hb_set_t used_majors;
  unsigned new_major_ಎಣಿಕೆ = 0;

  ತಾರ್ಕಿಕ has_variations () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ new_major_ಎಣಿಕೆ; }

  ಧ್ರುವ vardata_transform_t *get_transform (unsigned major) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ major < transforms.length ? &transforms.arrayZ[major] : nullptr; }

  /* Record that a retained blend references this VarData. */
  ಶೂನ್ಯ note_use (unsigned major) { used_majors.add (major); }

  /* The new vsindex a private dict's (or charstring's) old ivs maps to;
   * dead VarDatas fall back to 0, their blends having become static. */
  unsigned remap_ivs (unsigned old_ivs) ಧ್ರುವ
  {
    ಧ್ರುವ vardata_transform_t *t = get_transform (old_ivs);
    ಪ್ರತ್ಯಾಗಮ (t && t->alive ()) ? t->new_major : 0;
  }

  ದ್ವಿಗುಣ fold_deltas (unsigned old_ivs, hb_array_t<ಧ್ರುವ ಸಂಖ್ಯೆ_t> deltas) ಧ್ರುವ
  {
    ಧ್ರುವ vardata_transform_t *t = get_transform (old_ivs);
    ಯದಿ (unlikely (!t || t->gains.length != deltas.length)) ಪ್ರತ್ಯಾಗಮ 0.;
    ದ್ವಿಗುಣ v = 0.;
    ಪ್ರತಿ (unsigned i = 0; i < deltas.length; i++)
      v += (ದ್ವಿಗುಣ) t->gains.arrayZ[i] * deltas.arrayZ[i].to_real ();
    ಪ್ರತ್ಯಾಗಮ v;
  }

  ತಾರ್ಕಿಕ create (ಧ್ರುವ CFF2ItemVariationStore *cff2VarStore,
	       ಧ್ರುವ hb_subset_plan_t *plan)
  {
    ಧ್ರುವ OT::ItemVariationStore &varStore = cff2VarStore->varStore;
    unsigned major_ಎಣಿಕೆ = varStore.get_sub_table_ಎಣಿಕೆ ();
    ಯದಿ (!transforms.resize (major_ಎಣಿಕೆ)) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    ಯದಿ (!varStore.get_region_list ().get_var_regions (plan->axes_old_index_tag_map, orig_regions))
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    /* Instance each VarData with identity delta rows; the surviving tuples'
     * delta columns are the old-region → new-region transform. */
    OT::optimize_scratch_t scratch;
    ಪ್ರತಿ (unsigned m = 0; m < major_ಎಣಿಕೆ; m++)
    {
      ಧ್ರುವ OT::VarData &var_data = varStore.get_sub_table (m);
      unsigned k = var_data.get_region_index_ಎಣಿಕೆ ();
      vardata_transform_t &t = transforms.arrayZ[m];

      ಯದಿ (!t.gains.resize_exact (k)) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
      varStore.get_region_scalars (m, plan->normalized_coords.arrayZ,
				   plan->normalized_coords.length,
				   t.gains.arrayZ, k);

      OT::tuple_variations_t tuples;
      ಪ್ರತಿ (unsigned r = 0; r < k; r++)
      {
	OT::tuple_delta_t tuple;
	ಯದಿ (!tuple.deltas_x.resize (k) ||
	    !tuple.indices.resize (k))
	  ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
	ಪ್ರತಿ (unsigned i = 0; i < k; i++)
	  tuple.indices.arrayZ[i] = ಸತ್ಯ;
	tuple.deltas_x.arrayZ[r] = 1.f;
	unsigned region_index = var_data.get_region_index (r);
	ಯದಿ (unlikely (region_index >= orig_regions.length)) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
	tuple.axis_tuples = orig_regions.arrayZ[region_index];
	ಯದಿ (unlikely (tuple.axis_tuples.in_error ())) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
	tuples.tuple_vars.push (std::move (tuple));
      }
      ಯದಿ (unlikely (tuples.tuple_vars.in_error ())) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

      ಯದಿ (!tuples.instantiate (plan->axes_location, plan->axes_triple_distances, scratch))
	ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

      ಪ್ರತಿ (auto &tuple : tuples.tuple_vars)
      {
	ಯದಿ (unlikely (tuple.deltas_x.length != k)) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
	hb_vector_t<ಭಿನ್ನ> col;
	ಯದಿ (unlikely (!col.resize_exact (k))) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
	ಪ್ರತಿ (unsigned i = 0; i < k; i++)
	  col.arrayZ[i] = tuple.deltas_x.arrayZ[i];
	t.matrix.push (std::move (col));
	t.regions.push (std::move (tuple.axis_tuples));
      }
      ಯದಿ (unlikely (t.matrix.in_error () || t.regions.in_error ())) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }

    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }

  /* Decide which VarDatas survive, number them, and build the new global
   * region list: surviving original regions first, in their original order,
   * then solver-split new regions in order of appearance (fontTools'
   * rebuildRegions + prune_regions). */
  ತಾರ್ಕಿಕ finalize ()
  {
    ಪ್ರತಿ (unsigned m = 0; m < transforms.length; m++)
    {
      vardata_transform_t &t = transforms.arrayZ[m];
      t.alive_ = t.matrix.length && used_majors.has (m);
      ಯದಿ (t.alive_)
	t.new_major = new_major_ಎಣಿಕೆ++;
    }

    /* Keys hash by content, so pointers into different owners compare fine. */
    hb_hashmap_t<ಧ್ರುವ hb_hashmap_t<hb_tag_t, Triple>*, unsigned> region_idx_map;
    hb_vector_t<ಧ್ರುವ hb_hashmap_t<hb_tag_t, Triple>*> region_ptrs;
    ಪ್ರತಿ (ಧ್ರುವ vardata_transform_t &t : transforms)
    {
      ಯದಿ (!t.alive ()) ಅನುವರ್ತ;
      ಪ್ರತಿ (ಧ್ರುವ auto &region : t.regions)
	ಯದಿ (!region_idx_map.has (&region))
	  ಯದಿ (unlikely (!region_idx_map.set (&region, (unsigned) -1)))
	    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }
    ಪ್ರತಿ (ಧ್ರುವ auto &r : orig_regions)
    {
      unsigned *idx;
      ಯದಿ (region_idx_map.has (&r, &idx) && *idx == (unsigned) -1)
      {
	ಯದಿ (unlikely (!region_idx_map.set (&r, region_ptrs.length))) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
	region_ptrs.push (&r);
      }
    }
    ಪ್ರತಿ (ಧ್ರುವ vardata_transform_t &t : transforms)
    {
      ಯದಿ (!t.alive ()) ಅನುವರ್ತ;
      ಪ್ರತಿ (ಧ್ರುವ auto &region : t.regions)
      {
	unsigned *idx;
	ಯದಿ (region_idx_map.has (&region, &idx) && *idx == (unsigned) -1)
	{
	  ಯದಿ (unlikely (!region_idx_map.set (&region, region_ptrs.length))) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
	  region_ptrs.push (&region);
	}
      }
    }
    ಯದಿ (unlikely (region_ptrs.in_error ())) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    ಪ್ರತಿ (vardata_transform_t &t : transforms)
    {
      ಯದಿ (!t.alive ()) ಅನುವರ್ತ;
      ಪ್ರತಿ (ಧ್ರುವ auto &region : t.regions)
      {
	unsigned *idx;
	ಯದಿ (unlikely (!region_idx_map.has (&region, &idx))) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
	t.region_indices.push (*idx);
      }
      ಯದಿ (unlikely (t.region_indices.in_error ())) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }

    ಯದಿ (!new_regions.resize (region_ptrs.length)) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    ಪ್ರತಿ (unsigned i = 0; i < region_ptrs.length; i++)
    {
      new_regions.arrayZ[i] = *region_ptrs.arrayZ[i];
      ಯದಿ (unlikely (new_regions.arrayZ[i].in_error ())) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }

    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }

  /* Serialize the instanced store: a CFF2ItemVariationStore whose VarDatas
   * carry no items, just region indexes (blend deltas live in charstrings).
   * Laid out manually since all sizes are known up front. */
  ತಾರ್ಕಿಕ serialize_var_store (hb_serialize_context_t *c,
			    ಧ್ರುವ hb_vector_t<hb_tag_t> &axis_tags) ಧ್ರುವ
  {
    unsigned data_ಎಣಿಕೆ = new_major_ಎಣಿಕೆ;
    unsigned header_size = 2 + 4 + 2 + 4 * data_ಎಣಿಕೆ;

    uint64_t offset = header_size;
    hb_vector_t<unsigned> data_offsets;
    ಪ್ರತಿ (ಧ್ರುವ vardata_transform_t &t : transforms)
    {
      ಯದಿ (!t.alive ()) ಅನುವರ್ತ;
      data_offsets.push ((unsigned) offset);
      offset += 6 + 2 * (uint64_t) t.region_indices.length;
    }
    ಯದಿ (unlikely (data_offsets.in_error () || data_offsets.length != data_ಎಣಿಕೆ))
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    unsigned regions_offset = (unsigned) offset;
    uint64_t total = offset + 4 + (uint64_t) new_regions.length * axis_tags.length * 6;
    ಯದಿ (unlikely (total > 0xFFFFFFFFu)) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    HBUINT16 *size_field = c->allocate_size<HBUINT16> (HBUINT16::static_size);
    ಯದಿ (unlikely (!size_field)) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    /* OFF 2026 5.3.3.11.2 permits larger stores with length=65535. */
    *size_field = hb_min (total, (uint64_t) 0xFFFFu);

    ವರ್ಣ *header = c->allocate_size<ವರ್ಣ> (header_size);
    ಯದಿ (unlikely (!header)) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    * (HBUINT16 *) (header + 0) = 1;			/* format */
    * (HBUINT32 *) (header + 2) = regions_offset;
    * (HBUINT16 *) (header + 6) = data_ಎಣಿಕೆ;
    ಪ್ರತಿ (unsigned i = 0; i < data_ಎಣಿಕೆ; i++)
      * (HBUINT32 *) (header + 8 + 4 * i) = data_offsets.arrayZ[i];

    ಪ್ರತಿ (ಧ್ರುವ vardata_transform_t &t : transforms)
    {
      ಯದಿ (!t.alive ()) ಅನುವರ್ತ;
      unsigned k = t.region_indices.length;
      HBUINT16 *v = (HBUINT16 *) c->allocate_size<ವರ್ಣ> (6 + 2 * k);
      ಯದಿ (unlikely (!v)) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
      v[0] = 0;						/* itemCount */
      v[1] = 0;						/* wordSizeCount */
      v[2] = k;						/* regionIndices.len */
      ಪ್ರತಿ (unsigned j = 0; j < k; j++)
	v[3 + j] = t.region_indices.arrayZ[j];
    }

    hb_vector_t<ಧ್ರುವ hb_hashmap_t<hb_tag_t, Triple>*> region_ptrs;
    ಯದಿ (unlikely (!region_ptrs.alloc_exact (new_regions.length))) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    ಪ್ರತಿ (ಧ್ರುವ auto &r : new_regions)
      region_ptrs.push (&r);
    auto *region_list = c->start_embed<OT::VarRegionList> ();
    ಪ್ರತ್ಯಾಗಮ region_list->serialize (c, axis_tags, region_ptrs);
  }

  ಗುಪ್ತ:
  hb_vector_t<hb_hashmap_t<hb_tag_t, Triple>> orig_regions;
};

/* Usage-collection pass for partial instancing: record which VarDatas the
 * retained charstrings' and private dicts' blends reference.  Mirrors the
 * emission pass: blends consumed by dropped hint ops don't count. */
ಸಂರಚನಾ cff2_usage_param_t
{
  ಶೂನ್ಯ init () {}

  cff2_instancing_plan_t *instancing = nullptr;
  ತಾರ್ಕಿಕ drop_hints = ಅಸತ್ಯ;
  unsigned ivs = 0;	/* dict-side current vsindex */
};

ಸಂರಚನಾ cff2_cs_opset_usage_t : cff2_cs_opset_t<cff2_cs_opset_usage_t, cff2_usage_param_t, blend_arg_t>
{
  ಸ್ಥಿರ ಶೂನ್ಯ flush_args_and_op (op_code_t op, cff2_cs_interp_env_t<blend_arg_t> &env, cff2_usage_param_t& param)
  {
    ತಾರ್ಕಿಕ counts = ಸತ್ಯ;
    ವಿಕಲ್ಪ (op)
    {
      ಪಕ್ಷ OpCode_return:
      ಪಕ್ಷ OpCode_endchar:
	counts = ಅಸತ್ಯ;
	ಭಙ್ಗ;

      ಪಕ್ಷ OpCode_hstem:
      ಪಕ್ಷ OpCode_hstemhm:
      ಪಕ್ಷ OpCode_vstem:
      ಪಕ್ಷ OpCode_vstemhm:
      ಪಕ್ಷ OpCode_hintmask:
      ಪಕ್ಷ OpCode_cntrmask:
	ಯದಿ (param.drop_hints)
	  counts = ಅಸತ್ಯ;
	ಭಙ್ಗ;

      ಸ್ವಭಾವ:
	ಭಙ್ಗ;
    }

    ಯದಿ (counts)
      ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < env.arg_ರಾಶಿ.get_ಎಣಿಕೆ (); i++)
	ಯದಿ (env.arg_ರಾಶಿ[i].blending ())
	{
	  param.instancing->note_use (env.get_ivs ());
	  ಭಙ್ಗ;
	}

    env.clear_args ();
  }
};

ಸಂರಚನಾ cff2_private_dict_usage_opset_t : dict_opset_t
{
  ಸ್ಥಿರ ಶೂನ್ಯ process_op (op_code_t op, cff2_priv_dict_interp_env_t& env, cff2_usage_param_t& param)
  {
    ವಿಕಲ್ಪ (op) {
      ಪಕ್ಷ OpCode_vsindexdict:
	env.process_vsindex ();
	param.ivs = env.get_ivs ();
	env.clear_args ();
	ಪ್ರತ್ಯಾಗಮ;
      ಪಕ್ಷ OpCode_blenddict:
	param.instancing->note_use (param.ivs);
	env.clear_args ();
	ಪ್ರತ್ಯಾಗಮ;
      ಸ್ವಭಾವ:
	dict_opset_t::process_op (op, env);
	ಯದಿ (!env.arg_ರಾಶಿ.is_empty ()) ಪ್ರತ್ಯಾಗಮ;
	env.clear_args ();
	ಪ್ರತ್ಯಾಗಮ;
    }
  }
};

} /* namespace CFF */

ಸಂರಚನಾ cff2_sub_table_info_t : cff_sub_table_info_t
{
  cff2_sub_table_info_t ()
    : cff_sub_table_info_t (),
      var_store_link (0)
  {}

  objidx_t  var_store_link;
};

ಸಂರಚನಾ cff2_ತುದಿ_dict_op_serializer_t : cff_ತುದಿ_dict_op_serializer_t<>
{
  ತಾರ್ಕಿಕ serialize (hb_serialize_context_t *c,
		  ಧ್ರುವ op_str_t &opstr,
		  ಧ್ರುವ cff2_sub_table_info_t &info) ಧ್ರುವ
  {
    TRACE_SERIALIZE (ಏತತ್);

    ವಿಕಲ್ಪ (opstr.op)
    {
      ಪಕ್ಷ OpCode_vstore:
        ಯದಿ (info.var_store_link)
	  return_trace (FontDict::serialize_link4_op(c, opstr.op, info.var_store_link));
	ಅನ್ಯಥಾ
	  return_trace (ಸತ್ಯ);

      ಸ್ವಭಾವ:
	return_trace (cff_ತುದಿ_dict_op_serializer_t<>::serialize (c, opstr, info));
    }
  }
};

ಸಂರಚನಾ cff2_cs_opset_flatten_t : cff2_cs_opset_t<cff2_cs_opset_flatten_t, flatten_param_t, blend_arg_t>
{
  ಸ್ಥಿರ ಶೂನ್ಯ flush_args_and_op (op_code_t op, cff2_cs_interp_env_t<blend_arg_t> &env, flatten_param_t& param)
  {
    /* Optionally capture command for specialization (before flushing, to preserve args) */
    ಯದಿ (param.commands)
    {
      ತಾರ್ಕಿಕ skip_command = ಅಸತ್ಯ;

      ವಿಕಲ್ಪ (op)
      {
	ಪಕ್ಷ OpCode_return:
	ಪಕ್ಷ OpCode_endchar:
	  skip_command = ಸತ್ಯ;
	  ಭಙ್ಗ;

	ಪಕ್ಷ OpCode_hstem:
	ಪಕ್ಷ OpCode_hstemhm:
	ಪಕ್ಷ OpCode_vstem:
	ಪಕ್ಷ OpCode_vstemhm:
	ಪಕ್ಷ OpCode_hintmask:
	ಪಕ್ಷ OpCode_cntrmask:
	  ಯದಿ (param.drop_hints)
	    skip_command = ಸತ್ಯ;
	  ಭಙ್ಗ;

	ಸ್ವಭಾವ:
	  ಭಙ್ಗ;
      }

      ಯದಿ (!skip_command)
      {
	cs_command_t cmd (op);
	/* Capture resolved blend values */
	ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < env.arg_ರಾಶಿ.get_ಎಣಿಕೆ ();)
	{
	  ಧ್ರುವ blend_arg_t &arg = env.arg_ರಾಶಿ[i];
	  ಯದಿ (arg.blending ())
	  {
	    /* For blend args, capture only the resolved default value */
	    cmd.args.push (arg);
	    /* Skip over the multiple blend values */
	    i += arg.ಅಂಕೆ_ಮೌಲ್ಯಗಳು_2;
	  }
	  ಅನ್ಯಥಾ
	  {
	    cmd.args.push (arg);
	    i++;
	  }
	}
	param.commands->push (cmd);
      }
    }

    ವಿಕಲ್ಪ (op)
    {
      ಪಕ್ಷ OpCode_return:
      ಪಕ್ಷ OpCode_endchar:
	/* dummy opcodes in CFF2. ignore */
	ಯದಿ (op == OpCode_endchar && param.instancer)
	  emit_vsindex_prefix (env, param);
	ಭಙ್ಗ;

      ಪಕ್ಷ OpCode_hstem:
      ಪಕ್ಷ OpCode_hstemhm:
      ಪಕ್ಷ OpCode_vstem:
      ಪಕ್ಷ OpCode_vstemhm:
      ಪಕ್ಷ OpCode_hintmask:
      ಪಕ್ಷ OpCode_cntrmask:
	ಯದಿ (param.drop_hints)
	{
	  env.clear_args ();
	  ಪ್ರತ್ಯಾಗಮ;
	}
	HB_FALLTHROUGH;

      ಸ್ವಭಾವ:
	SUPER::flush_args_and_op (op, env, param);
	ಭಙ್ಗ;
    }
  }

  ಸ್ಥಿರ ಶೂನ್ಯ flush_args (cff2_cs_interp_env_t<blend_arg_t> &env, flatten_param_t& param)
  {
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < env.arg_ರಾಶಿ.get_ಎಣಿಕೆ ();)
    {
      ಧ್ರುವ blend_arg_t &arg = env.arg_ರಾಶಿ[i];
      ಯದಿ (arg.blending ())
      {
	ಯದಿ (unlikely (!((arg.ಅಂಕೆ_ಮೌಲ್ಯಗಳು_2 > 0) && (env.arg_ರಾಶಿ.get_ಎಣಿಕೆ () >= arg.ಅಂಕೆ_ಮೌಲ್ಯಗಳು_2))))
	{
	  env.set_error ();
	  ಪ್ರತ್ಯಾಗಮ;
	}
	ಯದಿ (param.instancer)
	  rewrite_blends (arg, i, env, param);
	ಅನ್ಯಥಾ
	  flatten_blends (arg, i, env, param);
	i += arg.ಅಂಕೆ_ಮೌಲ್ಯಗಳು_2;
      }
      ಅನ್ಯಥಾ
      {
	str_encoder_t  encoder (param.flatStr);
	encoder.encode_ಅಂಕೆ_cs (arg);
	i++;
      }
    }
    SUPER::flush_args (env, param);
  }

  ಸ್ಥಿರ ಶೂನ್ಯ flatten_blends (ಧ್ರುವ blend_arg_t &arg, unsigned ಪೂರ್ಣಾಙ್ಕ i, cff2_cs_interp_env_t<blend_arg_t> &env, flatten_param_t& param)
  {
    /* flatten the default values */
    str_encoder_t  encoder (param.flatStr);
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ j = 0; j < arg.ಅಂಕೆ_ಮೌಲ್ಯಗಳು_2; j++)
    {
      ಧ್ರುವ blend_arg_t &arg1 = env.arg_ರಾಶಿ[i + j];
      ಯದಿ (unlikely (!((arg1.blending () && (arg.ಅಂಕೆ_ಮೌಲ್ಯಗಳು_2 == arg1.ಅಂಕೆ_ಮೌಲ್ಯಗಳು_2) && (arg1.ಮೌಲ್ಯ_Index_2 == j) &&
	      (arg1.deltas.length == env.get_region_ಎಣಿಕೆ ())))))
      {
	env.set_error ();
	ಪ್ರತ್ಯಾಗಮ;
      }
      encoder.encode_ಅಂಕೆ_cs (arg1);
    }
    /* flatten deltas for each value */
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ j = 0; j < arg.ಅಂಕೆ_ಮೌಲ್ಯಗಳು_2; j++)
    {
      ಧ್ರುವ blend_arg_t &arg1 = env.arg_ರಾಶಿ[i + j];
      ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ k = 0; k < arg1.deltas.length; k++)
	encoder.encode_ಅಂಕೆ_cs (arg1.deltas[k]);
    }
    /* flatten the number of values followed by blend operator */
    encoder.encode_int (arg.ಅಂಕೆ_ಮೌಲ್ಯಗಳು_2);
    encoder.encode_op (OpCode_blendcs);
  }

  /* Partial instancing: rewrite one blend group against the instanced store.
   * Each value's default absorbs the folded (pinned-away) contribution, and
   * its deltas are mapped onto the new region list. */
  ಸ್ಥಿರ ಶೂನ್ಯ rewrite_blends (ಧ್ರುವ blend_arg_t &arg, unsigned ಪೂರ್ಣಾಙ್ಕ i, cff2_cs_interp_env_t<blend_arg_t> &env, flatten_param_t& param)
  {
    ಧ್ರುವ cff2_instancing_plan_t &instancer = *param.instancer;
    unsigned ivs = env.get_ivs ();
    ಧ್ರುವ cff2_instancing_plan_t::vardata_transform_t *t = instancer.get_transform (ivs);
    ಯದಿ (unlikely (!t))
    {
      env.set_error ();
      ಪ್ರತ್ಯಾಗಮ;
    }

    unsigned n = arg.ಅಂಕೆ_ಮೌಲ್ಯಗಳು_2;
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ j = 0; j < n; j++)
    {
      ಧ್ರುವ blend_arg_t &arg1 = env.arg_ರಾಶಿ[i + j];
      ಯದಿ (unlikely (!(arg1.blending () && (arg1.ಅಂಕೆ_ಮೌಲ್ಯಗಳು_2 == n) && (arg1.ಮೌಲ್ಯ_Index_2 == j) &&
		      (arg1.deltas.length == env.get_region_ಎಣಿಕೆ ()) &&
		      (arg1.deltas.length == t->gains.length))))
      {
	env.set_error ();
	ಪ್ರತ್ಯಾಗಮ;
      }
    }

    str_encoder_t encoder (param.flatStr);
    unsigned k_new = t->alive () ? t->matrix.length : 0;

    ಯದಿ (!k_new)
    {
      /* All of this VarData's regions died; blends dissolve into statics. */
      ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ j = 0; j < n; j++)
      {
	ಧ್ರುವ blend_arg_t &arg1 = env.arg_ರಾಶಿ[i + j];
	ಸಂಖ್ಯೆ_t v;
	v.set_real (arg1.to_real () + round (instancer.fold_deltas (ivs, arg1.deltas.as_array ())));
	encoder.encode_ಅಂಕೆ_cs (v);
      }
      ಪ್ರತ್ಯಾಗಮ;
    }

    /* Emit in chunks so the output charstring stays within the argument
     * stack limit: values already emitted for this op, plus this chunk's
     * defaults, deltas and count, must not exceed 513. */
    unsigned done = 0;
    ಯಾವತ್ (done < n)
    {
      unsigned avail = 512 > (i + done) ? 512 - (i + done) : 0;
      unsigned chunk = hb_min (avail / (k_new + 1), n - done);
      ಯದಿ (unlikely (!chunk))
      {
	env.set_error ();
	ಪ್ರತ್ಯಾಗಮ;
      }

      ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ j = done; j < done + chunk; j++)
      {
	ಧ್ರುವ blend_arg_t &arg1 = env.arg_ರಾಶಿ[i + j];
	ಸಂಖ್ಯೆ_t v;
	v.set_real (arg1.to_real () + round (instancer.fold_deltas (ivs, arg1.deltas.as_array ())));
	encoder.encode_ಅಂಕೆ_cs (v);
      }
      ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ j = done; j < done + chunk; j++)
      {
	ಧ್ರುವ blend_arg_t &arg1 = env.arg_ರಾಶಿ[i + j];
	ಪ್ರತಿ (unsigned r = 0; r < k_new; r++)
	{
	  ಸಂಖ್ಯೆ_t v;
	  v.set_int (hb_clamp_to<ಪೂರ್ಣಾಙ್ಕ> (round (t->transform_delta (r, arg1.deltas.as_array ()))));
	  encoder.encode_ಅಂಕೆ_cs (v);
	}
      }
      encoder.encode_int (chunk);
      encoder.encode_op (OpCode_blendcs);
      done += chunk;
    }
    param.emitted_blend = ಸತ್ಯ;
  }

  /* Partial instancing: prepend a vsindex op when the charstring's
   * (remapped) ivs differs from its FD's (remapped) private-dict ivs. */
  ಸ್ಥಿರ ಶೂನ್ಯ emit_vsindex_prefix (cff2_cs_interp_env_t<blend_arg_t> &env, flatten_param_t& param)
  {
    ಯದಿ (!param.emitted_blend) ಪ್ರತ್ಯಾಗಮ;
    ಧ್ರುವ cff2_instancing_plan_t &instancer = *param.instancer;
    unsigned new_ivs = instancer.remap_ivs (env.get_ivs ());
    unsigned fd_ivs = instancer.remap_ivs (env.get_orig_ivs ());
    ಯದಿ (new_ivs == fd_ivs) ಪ್ರತ್ಯಾಗಮ;

    str_buff_t prefix;
    str_encoder_t enc (prefix);
    enc.encode_int ((ಪೂರ್ಣಾಙ್ಕ) new_ivs);
    enc.encode_op (OpCode_vsindexcs);

    str_buff_t &buf = param.flatStr;
    unsigned old_len = buf.length;
    ಯದಿ (unlikely (prefix.in_error () || !buf.resize (old_len + prefix.length)))
    {
      env.set_error ();
      ಪ್ರತ್ಯಾಗಮ;
    }
    memmove (buf.arrayZ + prefix.length, buf.arrayZ, old_len);
    hb_memcpy (buf.arrayZ, prefix.arrayZ, prefix.length);
  }

  ಸ್ಥಿರ ಶೂನ್ಯ flush_op (op_code_t op, cff2_cs_interp_env_t<blend_arg_t> &env, flatten_param_t& param)
  {
    ವಿಕಲ್ಪ (op)
    {
      ಪಕ್ಷ OpCode_return:
      ಪಕ್ಷ OpCode_endchar:
	ಪ್ರತ್ಯಾಗಮ;
      ಸ್ವಭಾವ:
	str_encoder_t  encoder (param.flatStr);
	encoder.encode_op (op);
    }
  }

  ಸ್ಥಿರ ಶೂನ್ಯ flush_hintmask (op_code_t op, cff2_cs_interp_env_t<blend_arg_t> &env, flatten_param_t& param)
  {
    SUPER::flush_hintmask (op, env, param);
    /* Preserve hintmask payload in captured commands for specializer re-encoding. */
    ಯದಿ (param.commands && !param.drop_hints && param.commands->length > 0)
    {
      auto &cmd = param.commands->tail ();
      ಯದಿ (cmd.op == op)
      {
        cmd.mask_bytes.resize (env.hintmask_size);
        ಯದಿ (unlikely (cmd.mask_bytes.in_error ()))
        {
          env.set_error ();
          ಪ್ರತ್ಯಾಗಮ;
        }
        ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < env.hintmask_size; i++)
          cmd.mask_bytes[i] = env.str_ref[i];
      }
    }
    ಯದಿ (!param.drop_hints)
    {
      str_encoder_t  encoder (param.flatStr);
      ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < env.hintmask_size; i++)
	encoder.encode_byte (env.str_ref[i]);
    }
  }

  ಗುಪ್ತ:
  ಪ್ರಕಾರನಿರ್ದೇಶ cff2_cs_opset_t<cff2_cs_opset_flatten_t, flatten_param_t, blend_arg_t> SUPER;
  ಪ್ರಕಾರನಿರ್ದೇಶ cs_opset_t<blend_arg_t, cff2_cs_opset_flatten_t, cff2_cs_opset_flatten_t, cff2_cs_interp_env_t<blend_arg_t>, flatten_param_t> CSOPSET;
};

ಸಂರಚನಾ cff2_cs_opset_subr_subset_t : cff2_cs_opset_t<cff2_cs_opset_subr_subset_t, subr_subset_param_t, blend_arg_t>
{
  ಸ್ಥಿರ ಶೂನ್ಯ process_op (op_code_t op, cff2_cs_interp_env_t<blend_arg_t> &env, subr_subset_param_t& param)
  {
    ವಿಕಲ್ಪ (op) {

      ಪಕ್ಷ OpCode_return:
	param.current_parsed_str->flush_coalesced (env.str_ref);
	param.current_parsed_str->set_parsed ();
	env.return_from_subr ();
	param.set_current_str (env, ಅಸತ್ಯ);
	ಭಙ್ಗ;

      ಪಕ್ಷ OpCode_endchar:
	param.current_parsed_str->flush_coalesced (env.str_ref);
	param.current_parsed_str->set_parsed ();
	SUPER::process_op (op, env, param);
	ಭಙ್ಗ;

      ಪಕ್ಷ OpCode_callsubr:
	process_call_subr (op, CSType_LocalSubr, env, param, env.localSubrs, param.local_closure);
	ಭಙ್ಗ;

      ಪಕ್ಷ OpCode_callgsubr:
	process_call_subr (op, CSType_GlobalSubr, env, param, env.globalSubrs, param.global_closure);
	ಭಙ್ಗ;

      ಸ್ವಭಾವ:
	SUPER::process_op (op, env, param);
	param.current_parsed_str->add_op (op, env.str_ref);
	ಭಙ್ಗ;
    }
  }

  ರಕ್ಷಿತ:
  ಸ್ಥಿರ ಶೂನ್ಯ process_call_subr (op_code_t op, cs_type_t type,
				 cff2_cs_interp_env_t<blend_arg_t> &env, subr_subset_param_t& param,
				 cff2_biased_subrs_t& subrs, hb_set_t *closure)
  {
    byte_str_ref_t    str_ref = env.str_ref;
    env.call_subr (subrs, type);
    param.current_parsed_str->add_call_op (op, str_ref, env.context.subr_ಅಂಕೆ_2);
    closure->add (env.context.subr_ಅಂಕೆ_2);
    param.set_current_str (env, ಸತ್ಯ);
  }

  ಗುಪ್ತ:
  ಪ್ರಕಾರನಿರ್ದೇಶ cff2_cs_opset_t<cff2_cs_opset_subr_subset_t, subr_subset_param_t, blend_arg_t> SUPER;
};

ಸಂರಚನಾ cff2_subr_subsetter_t : subr_subsetter_t<cff2_subr_subsetter_t, CFF2Subrs, ಧ್ರುವ OT::cff2::accelerator_subset_t, cff2_cs_interp_env_t<blend_arg_t>, cff2_cs_opset_subr_subset_t>
{
  cff2_subr_subsetter_t (ಧ್ರುವ OT::cff2::accelerator_subset_t &acc_, ಧ್ರುವ hb_subset_plan_t *plan_)
    : subr_subsetter_t (acc_, plan_) {}

  ಸ್ಥಿರ ಶೂನ್ಯ complete_parsed_str (cff2_cs_interp_env_t<blend_arg_t> &env, subr_subset_param_t& param, parsed_cs_str_t &charstring)
  {
    /* vsindex is inserted at the beginning of the charstring as necessary */
    ಯದಿ (env.seen_vsindex ())
    {
      ಸಂಖ್ಯೆ_t  ivs;
      ivs.set_int ((ಪೂರ್ಣಾಙ್ಕ)env.get_ivs ());
      charstring.set_prefix (ivs, OpCode_vsindexcs);
    }
  }
};

ಸಂರಚನಾ cff2_private_blend_encoder_param_t
{
  cff2_private_blend_encoder_param_t (hb_serialize_context_t *c,
				      ಧ್ರುವ CFF2ItemVariationStore *varStore,
				      hb_array_t<ಪೂರ್ಣಾಙ್ಕ> normalized_coords) :
    c (c), varStore (varStore), normalized_coords (normalized_coords) {}

  ಶೂನ್ಯ init () {}

  ಶೂನ್ಯ process_blend ()
  {
    ಯದಿ (!seen_blend)
    {
      region_ಎಣಿಕೆ_2 = varStore->varStore.get_region_index_ಎಣಿಕೆ (ivs);
      ಯದಿ (unlikely (!scalars.resize_exact (region_ಎಣಿಕೆ_2)))
      {
	ಯದಿ (c) c->err (HB_SERIALIZE_ERROR_OTHER);
	seen_blend = ಸತ್ಯ;
	ಪ್ರತ್ಯಾಗಮ;
      }
      varStore->varStore.get_region_scalars (ivs, normalized_coords.arrayZ, normalized_coords.length,
					     &scalars[0], region_ಎಣಿಕೆ_2);
      seen_blend = ಸತ್ಯ;
    }
  }

  ದ್ವಿಗುಣ blend_deltas (hb_array_t<ಧ್ರುವ ಸಂಖ್ಯೆ_t> deltas) ಧ್ರುವ
  {
    ದ್ವಿಗುಣ v = 0;
    ಯದಿ (likely (scalars.length == deltas.length))
    {
      unsigned ಎಣಿಕೆ = scalars.length;
      ಪ್ರತಿ (unsigned i = 0; i < ಎಣಿಕೆ; i++)
	v += (ದ್ವಿಗುಣ) scalars.arrayZ[i] * deltas.arrayZ[i].to_real ();
    }
    ಪ್ರತ್ಯಾಗಮ v;
  }


  hb_serialize_context_t *c = nullptr;
  ತಾರ್ಕಿಕ seen_blend = ಅಸತ್ಯ;
  unsigned ivs = 0;
  unsigned region_ಎಣಿಕೆ_2 = 0;
  hb_vector_t<ಭಿನ್ನ> scalars;
  ಧ್ರುವ	 CFF2ItemVariationStore *varStore = nullptr;
  hb_array_t<ಪೂರ್ಣಾಙ್ಕ> normalized_coords;

  /* CFF2 partial instancing: when set, blends are rewritten against the
   * instanced variation store instead of being flattened. */
  ಧ್ರುವ cff2_instancing_plan_t *instancer = nullptr;
  /* Error-compensated fold rounding state, per dict operator. */
  ದ್ವಿಗುಣ fold_exact = 0.;
  ದ್ವಿಗುಣ fold_emitted = 0.;
};

ಸಂರಚನಾ cff2_private_dict_blend_opset_t : dict_opset_t
{
  ಸ್ಥಿರ ಶೂನ್ಯ process_arg_blend (cff2_private_blend_encoder_param_t& param,
				 ಸಂಖ್ಯೆ_t &arg,
				 ಧ್ರುವ hb_array_t<ಧ್ರುವ ಸಂಖ್ಯೆ_t> blends,
				 unsigned n, unsigned i)
  {
    arg.set_int (round (arg.to_real () + param.blend_deltas (blends)));
  }

  /* The fold added to a value, error-compensated across the operator: most
   * blended private-dict values (BlueValues, StemSnap*, ...) are
   * delta-encoded, so independently rounded folds would accumulate in the
   * decoded absolutes; keeping the emitted cumulative fold equal to the
   * rounded exact cumulative fold bounds every absolute within ±0.5. */
  ಸ್ಥಿರ ದ್ವಿಗುಣ fold (cff2_private_blend_encoder_param_t& param,
		      ಧ್ರುವ hb_array_t<ಧ್ರುವ ಸಂಖ್ಯೆ_t> blends)
  {
    ದ್ವಿಗುಣ f = param.blend_deltas (blends);
    ದ್ವಿಗುಣ emit = round (param.fold_exact + f) - param.fold_emitted;
    param.fold_exact += f;
    param.fold_emitted += emit;
    ಪ್ರತ್ಯಾಗಮ emit;
  }

  /* Partial instancing: emit the args preceding this blend group, then the
   * group itself rewritten against the instanced store, and clear the
   * stack.  The op's remaining args are flushed at process_op time. */
  ಸ್ಥಿರ ಶೂನ್ಯ rewrite_blends (cff2_priv_dict_interp_env_t& env,
			      cff2_private_blend_encoder_param_t& param,
			      unsigned start, unsigned n, unsigned k)
  {
    ಧ್ರುವ cff2_instancing_plan_t &instancer = *param.instancer;
    ಧ್ರುವ cff2_instancing_plan_t::vardata_transform_t *t = instancer.get_transform (param.ivs);
    ಯದಿ (unlikely (!t || t->gains.length != k))
    {
      env.set_error ();
      ಪ್ರತ್ಯಾಗಮ;
    }

    str_buff_t str;
    str_encoder_t encoder (str);

    ಪ್ರತಿ (unsigned idx = 0; idx < start; idx++)
      encoder.encode_ಅಂಕೆ_tp (env.arg_ರಾಶಿ[idx]);

    unsigned k_new = t->alive () ? t->matrix.length : 0;
    ಯದಿ (!k_new)
    {
      /* This VarData died (or is unused by any retained charstring, which
       * can happen for FDs kept only for retain-gids holes); blends
       * dissolve into statics. */
      ಪ್ರತಿ (unsigned j = 0; j < n; j++)
      {
	ಧ್ರುವ hb_array_t<ಧ್ರುವ ಸಂಖ್ಯೆ_t> blends = env.arg_ರಾಶಿ.sub_array (start + n + (j * k), k);
	ಸಂಖ್ಯೆ_t v;
	v.set_real (env.arg_ರಾಶಿ[start + j].to_real () + fold (param, blends));
	encoder.encode_ಅಂಕೆ_tp (v);
      }
    }
    ಅನ್ಯಥಾ
    {
      unsigned done = 0;
      ಯಾವತ್ (done < n)
      {
	unsigned avail = 512 > (start + done) ? 512 - (start + done) : 0;
	unsigned chunk = hb_min (avail / (k_new + 1), n - done);
	ಯದಿ (unlikely (!chunk))
	{
	  env.set_error ();
	  ಪ್ರತ್ಯಾಗಮ;
	}

	ಪ್ರತಿ (unsigned j = done; j < done + chunk; j++)
	{
	  ಧ್ರುವ hb_array_t<ಧ್ರುವ ಸಂಖ್ಯೆ_t> blends = env.arg_ರಾಶಿ.sub_array (start + n + (j * k), k);
	  ಸಂಖ್ಯೆ_t v;
	  v.set_real (env.arg_ರಾಶಿ[start + j].to_real () + fold (param, blends));
	  encoder.encode_ಅಂಕೆ_tp (v);
	}
	ಪ್ರತಿ (unsigned j = done; j < done + chunk; j++)
	{
	  ಧ್ರುವ hb_array_t<ಧ್ರುವ ಸಂಖ್ಯೆ_t> blends = env.arg_ರಾಶಿ.sub_array (start + n + (j * k), k);
	  ಪ್ರತಿ (unsigned r = 0; r < k_new; r++)
	  {
	    ಸಂಖ್ಯೆ_t v;
	    v.set_int (hb_clamp_to<ಪೂರ್ಣಾಙ್ಕ> (round (t->transform_delta (r, blends))));
	    encoder.encode_ಅಂಕೆ_tp (v);
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

  ಸ್ಥಿರ ಶೂನ್ಯ process_blend (cff2_priv_dict_interp_env_t& env, cff2_private_blend_encoder_param_t& param)
  {
    unsigned ಪೂರ್ಣಾಙ್ಕ n, k;

    param.process_blend ();
    k = param.region_ಎಣಿಕೆ_2;
    n = env.arg_ರಾಶಿ.ತೆಗೆ_uint ();

    unsigned ಎಣಿಕೆ = env.arg_ರಾಶಿ.get_ಎಣಿಕೆ ();
    unsigned ಪೂರ್ಣಾಙ್ಕ total;
    ಯದಿ (unlikely (hb_unsigned_mul_overflows (k + 1, n, &total) || total > ಎಣಿಕೆ))
    {
      env.set_error ();
      ಪ್ರತ್ಯಾಗಮ;
    }

    /* copy the blend values into blend array of the default values */
    unsigned ಪೂರ್ಣಾಙ್ಕ start = ಎಣಿಕೆ - total;
    /* let an obvious error case fail, but note CFF2 spec doesn't forbid n==0 */
    ಯದಿ (unlikely (start > ಎಣಿಕೆ))
    {
      env.set_error ();
      ಪ್ರತ್ಯಾಗಮ;
    }

    ಯದಿ (param.instancer && k)
    {
      rewrite_blends (env, param, start, n, k);
      ಪ್ರತ್ಯಾಗಮ;
    }

    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < n; i++)
    {
      ಧ್ರುವ hb_array_t<ಧ್ರುವ ಸಂಖ್ಯೆ_t> blends = env.arg_ರಾಶಿ.sub_array (start + n + (i * k), k);
      process_arg_blend (param, env.arg_ರಾಶಿ[start + i], blends, n, i);
    }

    /* pop off blend values leaving default values now adorned with blend values */
    env.arg_ರಾಶಿ.ತೆಗೆ (k * n);
  }

  ಸ್ಥಿರ ಶೂನ್ಯ process_op (op_code_t op, cff2_priv_dict_interp_env_t& env, cff2_private_blend_encoder_param_t& param)
  {
    ವಿಕಲ್ಪ (op) {
      ಪಕ್ಷ OpCode_StdHW:
      ಪಕ್ಷ OpCode_StdVW:
      ಪಕ್ಷ OpCode_BlueScale:
      ಪಕ್ಷ OpCode_BlueShift:
      ಪಕ್ಷ OpCode_BlueFuzz:
      ಪಕ್ಷ OpCode_ExpansionFactor:
      ಪಕ್ಷ OpCode_LanguageGroup:
      ಪಕ್ಷ Op_Code_Blue_ಮೌಲ್ಯಗಳು:
      ಪಕ್ಷ OpCode_OtherBlues:
      ಪಕ್ಷ OpCode_FamilyBlues:
      ಪಕ್ಷ OpCode_FamilyOtherBlues:
      ಪಕ್ಷ OpCode_StemSnapH:
      ಪಕ್ಷ OpCode_StemSnapV:
	ಭಙ್ಗ;
      ಪಕ್ಷ OpCode_vsindexdict:
	env.process_vsindex ();
	param.ivs = env.get_ivs ();
	ಯದಿ (param.instancer)
	{
	  /* Re-emit with the remapped vsindex; 0 is the dict default. */
	  unsigned new_ivs = param.instancer->remap_ivs (param.ivs);
	  ಯದಿ (new_ivs)
	  {
	    str_buff_t str;
	    str_encoder_t encoder (str);
	    encoder.encode_int ((ಪೂರ್ಣಾಙ್ಕ) new_ivs);
	    encoder.encode_op (OpCode_vsindexdict);
	    auto bytes = str.as_bytes ();
	    param.c->embed (&bytes, bytes.length);
	  }
	}
	env.clear_args ();
	ಪ್ರತ್ಯಾಗಮ;
      ಪಕ್ಷ OpCode_blenddict:
	process_blend (env, param);
	ಪ್ರತ್ಯಾಗಮ;

      ಸ್ವಭಾವ:
	dict_opset_t::process_op (op, env);
	ಯದಿ (!env.arg_ರಾಶಿ.is_empty ()) ಪ್ರತ್ಯಾಗಮ;
	ಭಙ್ಗ;
    }

    ಯದಿ (unlikely (env.in_error ())) ಪ್ರತ್ಯಾಗಮ;

    // Write args then op

    str_buff_t str;
    str_encoder_t encoder (str);

    unsigned ಎಣಿಕೆ = env.arg_ರಾಶಿ.get_ಎಣಿಕೆ ();
    ಪ್ರತಿ (unsigned i = 0; i < ಎಣಿಕೆ; i++)
      encoder.encode_ಅಂಕೆ_tp (env.arg_ರಾಶಿ[i]);

    encoder.encode_op (op);

    auto bytes = str.as_bytes ();
    param.c->embed (&bytes, bytes.length);

    env.clear_args ();
    param.fold_exact = param.fold_emitted = 0.;
  }
};

ಸಂರಚನಾ cff2_private_dict_op_serializer_t : op_serializer_t
{
  cff2_private_dict_op_serializer_t (ತಾರ್ಕಿಕ desubroutinize_, ತಾರ್ಕಿಕ drop_hints_, ತಾರ್ಕಿಕ pinned_,
				     ಧ್ರುವ CFF::CFF2ItemVariationStore* varStore_,
				     hb_array_t<ಪೂರ್ಣಾಙ್ಕ> normalized_coords_,
				     ಧ್ರುವ cff2_instancing_plan_t *instancer_ = nullptr)
    : desubroutinize (desubroutinize_), drop_hints (drop_hints_), pinned (pinned_),
      param (nullptr, varStore_, normalized_coords_)
  { param.instancer = instancer_; }

  ತಾರ್ಕಿಕ serialize (hb_serialize_context_t *c,
		  ಧ್ರುವ op_str_t &opstr,
		  objidx_t subrs_link) ಧ್ರುವ
  {
    TRACE_SERIALIZE (ಏತತ್);

    ಯದಿ (drop_hints && dict_opset_t::is_hint_op (opstr.op))
      return_trace (ಸತ್ಯ);

    ಯದಿ (opstr.op == OpCode_Subrs)
    {
      ಯದಿ (desubroutinize || !subrs_link)
	return_trace (ಸತ್ಯ);
      ಅನ್ಯಥಾ
	return_trace (FontDict::serialize_link2_op (c, opstr.op, subrs_link));
    }

    ಯದಿ (pinned || param.instancer)
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

  ರಕ್ಷಿತ:
  ಧ್ರುವ ತಾರ್ಕಿಕ desubroutinize;
  ಧ್ರುವ ತಾರ್ಕಿಕ drop_hints;
  ಧ್ರುವ ತಾರ್ಕಿಕ pinned;
  mutable cff2_private_blend_encoder_param_t param;
};



ನಾಮಾಕಾಶ OT {
ಸಂರಚನಾ cff2_subset_plan
{
  ತಾರ್ಕಿಕ create (ಧ್ರುವ OT::cff2::accelerator_subset_t &acc,
	      hb_subset_plan_t *plan)
  {
    /* make sure notdef is first */
    hb_codepoint_t old_glyph;
    ಯದಿ (!plan->old_gid_for_new_gid (0, &old_glyph) || (old_glyph != 0)) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    ಅಂಕೆ_glyphs = plan->ಅಂಕೆ_output_glyphs ();
    orig_fdcount = acc.fdArray->ಎಣಿಕೆ;

    drop_hints = plan->flags & HB_SUBSET_FLAGS_NO_HINTING;
    ತಾರ್ಕಿಕ instancing_requested = (ತಾರ್ಕಿಕ) plan->normalized_coords;
    pinned = instancing_requested && plan->all_axes_pinned;
    normalized_coords = plan->normalized_coords;
    head_maxp_info = plan->head_maxp_info;
    hmtx_map = &plan->hmtx_map;
    desubroutinize = plan->flags & HB_SUBSET_FLAGS_DESUBROUTINIZE ||
		     instancing_requested; // For instancing we need this path

    /* Partial instancing: some axes pinned or limited, others kept. */
    partial_instancing = instancing_requested && !pinned &&
			 acc.varStore != &Null (CFF2ItemVariationStore);
    ಯದಿ (partial_instancing &&
	unlikely (!instancing.create (acc.varStore, plan)))
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    axis_tags = &plan->axis_tags;

    /* Enable command capture for CFF2→CFF1 conversion (for specialization) */
    capture_commands = pinned;

 #ifdef HB_EXPERIMENTAL_API
    min_charstrings_off_size = (plan->flags & HB_SUBSET_FLAGS_IFTB_REQUIREMENTS) ? 4 : 0;
 #ಅನ್ಯಥಾ
    min_charstrings_off_size = 0;
 #endif

    ಯದಿ (desubroutinize)
    {
      ಯದಿ (partial_instancing)
      {
	/* Pass 1: collect which VarDatas the retained blends reference, so
	 * unused ones get pruned and the survivors renumbered. */
	hb_set_t used_fds;
	ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಅಂಕೆ_glyphs; i++)
	{
	  hb_codepoint_t glyph;
	  ಯದಿ (!plan->old_gid_for_new_gid (i, &glyph))
	    ಅನುವರ್ತ;
	  ಧ್ರುವ hb_ubytes_t str = (*acc.charStrings)[glyph];
	  unsigned ಪೂರ್ಣಾಙ್ಕ fd = CFF::subset_get_fd (acc, glyph);
	  ಯದಿ (unlikely (fd >= acc.fd_ಎಣಿಕೆ))
	    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
	  used_fds.add (fd);

	  cff2_cs_interp_env_t<blend_arg_t> env (str, acc, fd);
	  cs_interpreter_t<cff2_cs_interp_env_t<blend_arg_t>, cff2_cs_opset_usage_t, cff2_usage_param_t> interp (env);
	  cff2_usage_param_t param;
	  param.instancing = &instancing;
	  param.drop_hints = drop_hints;
	  ಯದಿ (unlikely (!interp.interpret (param)))
	    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
	}
	ಪ್ರತಿ (unsigned fd : used_fds)
	{
	  cff2_usage_param_t param;
	  param.instancing = &instancing;
	  unsigned ಎಣಿಕೆ = acc.privateDicts[fd].get_ಎಣಿಕೆ ();
	  ಪ್ರತಿ (unsigned j = 0; j < ಎಣಿಕೆ; j++)
	  {
	    ಧ್ರುವ op_str_t &opstr = acc.privateDicts[fd][j];
	    ಯದಿ (drop_hints && dict_opset_t::is_hint_op (opstr.op))
	      ಅನುವರ್ತ;
	    cff2_priv_dict_interp_env_t env {hb_ubytes_t (opstr.ptr, opstr.length)};
	    dict_interpreter_t<cff2_private_dict_usage_opset_t, cff2_usage_param_t, cff2_priv_dict_interp_env_t> interp (env);
	    ಯದಿ (unlikely (!interp.interpret (param)))
	      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
	  }
	}
	ಯದಿ (unlikely (!instancing.finalize ()))
	  ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

	/* Pass 2: like subr_flattener_t, but blends stay symbolic (no coords
	 * fed to the env) and get rewritten against the instanced store. */
	ಯದಿ (!subset_charstrings.resize_exact (ಅಂಕೆ_glyphs))
	  ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
	ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಅಂಕೆ_glyphs; i++)
	{
	  hb_codepoint_t glyph;
	  ಯದಿ (!plan->old_gid_for_new_gid (i, &glyph))
	    ಅನುವರ್ತ;
	  ಧ್ರುವ hb_ubytes_t str = (*acc.charStrings)[glyph];
	  unsigned ಪೂರ್ಣಾಙ್ಕ fd = CFF::subset_get_fd (acc, glyph);
	  ಯದಿ (unlikely (fd >= acc.fd_ಎಣಿಕೆ))
	    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

	  cff2_cs_interp_env_t<blend_arg_t> env (str, acc, fd);
	  cs_interpreter_t<cff2_cs_interp_env_t<blend_arg_t>, cff2_cs_opset_flatten_t, flatten_param_t> interp (env);
	  flatten_param_t param = {
	    subset_charstrings.arrayZ[i],
	    (ತಾರ್ಕಿಕ) (plan->flags & HB_SUBSET_FLAGS_NO_HINTING),
	    plan
	  };
	  param.instancer = &instancing;
	  ಯದಿ (unlikely (!interp.interpret (param)))
	    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
	}
      }
      ಅನ್ಯಥಾ
      {
      /* Flatten global & local subrs */
      subr_flattener_t<ಧ್ರುವ OT::cff2::accelerator_subset_t, cff2_cs_interp_env_t<blend_arg_t>, cff2_cs_opset_flatten_t>
		    flattener(acc, plan);

      /* Enable command capture if requested (for specialization) */
      ಯದಿ (capture_commands)
      {
	ಯದಿ (!charstring_commands.resize_exact (ಅಂಕೆ_glyphs))
	  ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

	ಯದಿ (!flattener.flatten (subset_charstrings, &charstring_commands))
	  ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
      }
      ಅನ್ಯಥಾ
      {
	ಯದಿ (!flattener.flatten (subset_charstrings))
	  ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
      }
      }
    }
    ಅನ್ಯಥಾ
    {
      cff2_subr_subsetter_t	subr_subsetter (acc, plan);

      /* Subset subrs: collect used subroutines, leaving all unused ones behind */
      ಯದಿ (!subr_subsetter.subset ())
	ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

      /* encode charstrings, global subrs, local subrs with new subroutine numbers */
      ಯದಿ (!subr_subsetter.encode_charstrings (subset_charstrings, !pinned))
	ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

      ಯದಿ (!subr_subsetter.encode_globalsubrs (subset_globalsubrs))
	ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

      /* local subrs */
      ಯದಿ (!subset_localsubrs.resize (orig_fdcount))
	ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
      ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ fd = 0; fd < orig_fdcount; fd++)
      {
	subset_localsubrs[fd].init ();
	ಯದಿ (!subr_subsetter.encode_localsubrs (fd, subset_localsubrs[fd]))
	  ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
      }
    }

    /* FDSelect */
    ಯದಿ (acc.fdSelect != &Null (CFF2FDSelect))
    {
      ಯದಿ (unlikely (!hb_plan_subset_cff_fdselect (plan,
						  orig_fdcount,
						  *(ಧ್ರುವ FDSelect *)acc.fdSelect,
						  acc.ಅಂಕೆ_glyphs,
						  subset_fdcount,
						  subset_fdselect_size,
						  subset_fdselect_format,
						  subset_fdselect_ranges,
						  fdmap)))
	ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }
    ಅನ್ಯಥಾ
      fdmap.identity (1);

    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }

  cff2_sub_table_info_t info;

  unsigned ಪೂರ್ಣಾಙ್ಕ    ಅಂಕೆ_glyphs;
  unsigned ಪೂರ್ಣಾಙ್ಕ    orig_fdcount = 0;
  unsigned ಪೂರ್ಣಾಙ್ಕ    subset_fdcount = 1;
  unsigned ಪೂರ್ಣಾಙ್ಕ    subset_fdselect_size = 0;
  unsigned ಪೂರ್ಣಾಙ್ಕ    subset_fdselect_format = 0;
  ತಾರ್ಕಿಕ            pinned = ಅಸತ್ಯ;
  hb_vector_t<code_pair_t>   subset_fdselect_ranges;

  hb_inc_bimap_t   fdmap;

  str_buff_vec_t	    subset_charstrings;
  str_buff_vec_t	    subset_globalsubrs;
  hb_vector_t<str_buff_vec_t> subset_localsubrs;

  ತಾರ್ಕಿಕ	    drop_hints = ಅಸತ್ಯ;
  ತಾರ್ಕಿಕ	    desubroutinize = ಅಸತ್ಯ;

  ತಾರ್ಕಿಕ	    partial_instancing = ಅಸತ್ಯ;
  cff2_instancing_plan_t instancing;
  ಧ್ರುವ hb_vector_t<hb_tag_t> *axis_tags = nullptr;

  unsigned  min_charstrings_off_size = 0;

  hb_array_t<ಪೂರ್ಣಾಙ್ಕ> normalized_coords; // For instantiation
  head_maxp_info_t head_maxp_info;  // For FontBBox
  ಧ್ರುವ hb_hashmap_t<hb_codepoint_t, hb_pair_t<unsigned, ಪೂರ್ಣಾಙ್ಕ>> *hmtx_map; // For widths

  // Width optimization results (for CFF1 conversion)
  unsigned default_width = 0;
  unsigned nominal_width = 0;

  // Command capture for specialization (CFF2→CFF1 conversion)
  ತಾರ್ಕಿಕ capture_commands = ಅಸತ್ಯ;
  hb_vector_t<hb_vector_t<cs_command_t>> charstring_commands;
};
} // namespace OT

/*
 * CFF2 to CFF1 Converter Implementation
 */

#ಅನ್ತರ್ಭಾವ "hb-cff-width-optimizer.hh"
#ಅನ್ತರ್ಭಾವ "hb-cff-specializer.hh"

/* Serialize charstrings using CFF1 format with widths */
ಸ್ಥಿರ ತಾರ್ಕಿಕ
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
  ಯದಿ (unlikely (!cff1_charstrings.resize (plan.subset_charstrings.length)))
  {
    c->ತೆಗೆ_discard ();
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
  }

  ಪ್ರತಿ (unsigned i = 0; i < plan.subset_charstrings.length; i++)
  {
    // Get width for this glyph from hmtx_map
    unsigned width = 0;
    ಯದಿ (plan.hmtx_map->has (i))
      width = plan.hmtx_map->get (i).first;

    // Encode width if different from default
    str_encoder_t encoder (cff1_charstrings[i]);
    ಯದಿ (width != default_width)
    {
      ಪೂರ್ಣಾಙ್ಕ delta = (ಪೂರ್ಣಾಙ್ಕ) width - (ಪೂರ್ಣಾಙ್ಕ) nominal_width;
      encoder.encode_int (delta);
    }

    // Use specialized commands if available, otherwise use binary
    ಯದಿ (plan.capture_commands && i < plan.charstring_commands.length &&
        plan.charstring_commands[i].length > 0)
    {
      // Specialize and encode commands
      auto &commands = plan.charstring_commands[i];
      CFF::specialize_commands (commands, 48);  /* maxstack=48 for CFF1 */
      ಯದಿ (unlikely (!CFF::encode_commands (commands, cff1_charstrings[i])))
      {
        c->ತೆಗೆ_discard ();
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
      }
    }
    ಅನ್ಯಥಾ
    {
      // Use binary CharString
      ಧ್ರುವ str_buff_t &cs = plan.subset_charstrings[i];
      ಪ್ರತಿ (unsigned j = 0; j < cs.length; j++)
        cff1_charstrings[i].push (cs[j]);
    }

    // Check if it already ends with endchar (0x0e) or return (0x0b)
    ಯದಿ (cff1_charstrings[i].length == 0 ||
        (cff1_charstrings[i].tail () != 0x0e && cff1_charstrings[i].tail () != 0x0b))
    {
      // Append endchar operator
      ಯದಿ (unlikely (!cff1_charstrings[i].ತಳ್ಳು_or_fail (0x0e)))
      {
        c->ತೆಗೆ_discard ();
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
      }
    }
  }

  unsigned data_size = 0;
  unsigned total_size = CFF1CharStrings::total_size (cff1_charstrings, &data_size, plan.min_charstrings_off_size);
  ಯದಿ (unlikely (!c->start_zerocopy (total_size)))
  {
    c->ತೆಗೆ_discard ();
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
  }

  auto *cs = c->start_embed<CFF1CharStrings> ();
  ಯದಿ (unlikely (!cs->serialize (c, cff1_charstrings, &data_size, plan.min_charstrings_off_size)))
  {
    c->ತೆಗೆ_discard ();
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
  }

  plan.info.char_strings_link = c->ತೆಗೆ_pack (ಅಸತ್ಯ);
  ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
}

/* Serialize CID Charset (format 2 range: gid 0-N -> cid 0-N) */
ಸ್ಥಿರ ತಾರ್ಕಿಕ
_serialize_cff1_charset (hb_serialize_context_t *c,
                         unsigned ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_glyphs,
                         objidx_t &charset_link)
{
  // For CID fonts, create a simple identity charset
  // Format 2: one range covering all glyphs (except .notdef)
  c->push ();

  auto *charset = c->start_embed<Charset> ();
  ಯದಿ (unlikely (!charset))
  {
    c->ತೆಗೆ_discard ();
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
  }

  // Create a single range for CID 1 to num_glyphs-1
  hb_vector_t<code_pair_t> ranges;
  ಯದಿ (ಅಂಕೆ_glyphs > 1)
  {
    code_pair_t range;
    range.code = 1;  // first CID
    range.glyph = ಅಂಕೆ_glyphs - 2;  // nLeft (covers glyphs 1 to num_glyphs-1)
    ranges.push (range);
  }

  ಯದಿ (unlikely (!charset->serialize (c, 2, ಅಂಕೆ_glyphs, ranges)))
  {
    c->ತೆಗೆ_discard ();
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
  }

  charset_link = c->ತೆಗೆ_pack ();
  ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
}

/* CFF2 to CFF1 serialization */
ನಾಮಾಕಾಶ CFF {

ತಾರ್ಕಿಕ
serialize_cff2_to_cff1 (hb_serialize_context_t *c,
                        OT::cff2_subset_plan &plan,
                        ಧ್ರುವ cff2_ತುದಿ_dict_ಮೌಲ್ಯಗಳು_t &cff2_ತುದಿ_Dict,
                        ಧ್ರುವ OT::cff2::accelerator_subset_t &acc)
{
  TRACE_SERIALIZE (ಏತತ್);

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
    widths.alloc (plan.ಅಂಕೆ_glyphs);

    ಪ್ರತಿ (unsigned gid = 0; gid < plan.ಅಂಕೆ_glyphs; gid++)
    {
      unsigned width = 0;
      ಯದಿ (plan.hmtx_map->has (gid))
        width = plan.hmtx_map->get (gid).first;
      widths.push (width);
    }

    // Optimize defaultWidthX and nominalWidthX
    CFF::optimize_widths (widths, plan.default_width, plan.nominal_width);
  }

  // 1. CharStrings (with widths prepended)
  ಯದಿ (!_serialize_cff1_charstrings (c, plan, plan.default_width, plan.nominal_width))
    return_trace (ಅಸತ್ಯ);

  // 2. Private DICs & Local Subrs (same as CFF2)
  hb_vector_t<table_info_t> private_dict_infos;
  ಯದಿ (unlikely (!private_dict_infos.resize (plan.subset_fdcount)))
    return_trace (ಅಸತ್ಯ);

  ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = (ಪೂರ್ಣಾಙ್ಕ)acc.privateDicts.length; --i >= 0;)
  {
    ಯದಿ (plan.fdmap.has (i))
    {
      objidx_t subrs_link = 0;

      ಯದಿ (plan.subset_localsubrs[i].length > 0)
      {
        auto *dest = c->push<CFF1Subrs> ();
        ಯದಿ (likely (dest->serialize (c, plan.subset_localsubrs[i])))
          subrs_link = c->ತೆಗೆ_pack (ಅಸತ್ಯ);
        ಅನ್ಯಥಾ
        {
          c->ತೆಗೆ_discard ();
          return_trace (ಅಸತ್ಯ);
        }
      }

      auto *pd = c->push<PrivateDict> ();
      // Use the CFF2 Private DICT serializer which instantiates blends when pinned=true
      cff2_private_dict_op_serializer_t privSzr (plan.desubroutinize, plan.drop_hints, plan.pinned,
                                                  acc.varStore, plan.normalized_coords);
      ಯದಿ (likely (pd->serialize (c, acc.privateDicts[i], privSzr, subrs_link)))
      {
        // Add defaultWidthX and nominalWidthX for CFF1
        str_buff_t width_ops;
        str_encoder_t encoder (width_ops);
        encoder.encode_int (plan.default_width);
        encoder.encode_op (OpCode_defaultWidthX);
        encoder.encode_int (plan.nominal_width);
        encoder.encode_op (OpCode_nominalWidthX);

        ಯದಿ (!encoder.in_error () && c->embed (width_ops.as_bytes ().arrayZ, width_ops.length))
        {
          unsigned fd = plan.fdmap[i];
          private_dict_infos[fd].size = c->length ();
          private_dict_infos[fd].link = c->ತೆಗೆ_pack ();
        }
        ಅನ್ಯಥಾ
        {
          c->ತೆಗೆ_discard ();
          return_trace (ಅಸತ್ಯ);
        }
      }
      ಅನ್ಯಥಾ
      {
        c->ತೆಗೆ_discard ();
        return_trace (ಅಸತ್ಯ);
      }
    }
  }

  // 3. FDArray - serialize CFF2 font dicts as CFF1
  {
    auto *fda = c->push<FDArray<HBUINT16>> ();
    cff_font_dict_op_serializer_t fontSzr;
    auto it =
    + hb_zip (+ hb_iter (acc.fontDicts)
              | hb_filter ([&] (ಧ್ರುವ cff2_font_dict_ಮೌಲ್ಯಗಳು_t &_)
                { ಪ್ರತ್ಯಾಗಮ plan.fdmap.has (&_ - &acc.fontDicts[0]); }),
              hb_iter (private_dict_infos))
    ;
    // Explicitly specify template parameters: DICTVAL, INFO
    ತಾರ್ಕಿಕ success = fda->serialize<cff2_font_dict_ಮೌಲ್ಯಗಳು_t, table_info_t> (c, it, fontSzr);
    ಯದಿ (success)
      plan.info.fd_array_link = c->ತೆಗೆ_pack (ಅಸತ್ಯ);
    ಅನ್ಯಥಾ
    {
      c->ತೆಗೆ_discard ();
      return_trace (ಅಸತ್ಯ);
    }
  }

  // 4. FDSelect (required in CFF1 CID-keyed fonts)
  // CFF1 requires FDSelect for all CID-keyed fonts, even with just one FD
  // CFF2 makes it optional when there's only one FD
  ಯದಿ (acc.fdSelect != &Null (CFF2FDSelect))
  {
    c->push ();
    ಯದಿ (likely (hb_serialize_cff_fdselect (c, plan.ಅಂಕೆ_glyphs,
                                          *(ಧ್ರುವ FDSelect *)acc.fdSelect,
                                          plan.orig_fdcount,
                                          plan.subset_fdselect_format,
                                          plan.subset_fdselect_size,
                                          plan.subset_fdselect_ranges)))
      plan.info.fd_select.link = c->ತೆಗೆ_pack ();
    ಅನ್ಯಥಾ
    {
      c->ತೆಗೆ_discard ();
      return_trace (ಅಸತ್ಯ);
    }
  }
  ಅನ್ಯಥಾ
  {
    // Create a range-based FDSelect3 mapping all glyphs to FD 0
    // Format: format(1) + nRanges(2) + range(3) + sentinel(2) = 8 bytes
    c->push ();

    // Format byte
    HBUINT8 format;
    format = 3;
    ಯದಿ (unlikely (!c->embed (format)))
    {
      c->ತೆಗೆ_discard ();
      return_trace (ಅಸತ್ಯ);
    }

    // nRanges
    HBUINT16 nRanges;
    nRanges = 1;
    ಯದಿ (unlikely (!c->embed (nRanges)))
    {
      c->ತೆಗೆ_discard ();
      return_trace (ಅಸತ್ಯ);
    }

    // Single range: {first: 0, fd: 0}
    FDSelect3_Range range;
    range.first = 0;
    range.fd = 0;
    ಯದಿ (unlikely (!c->embed (range)))
    {
      c->ತೆಗೆ_discard ();
      return_trace (ಅಸತ್ಯ);
    }

    // Sentinel (number of glyphs)
    HBUINT16 sentinel;
    sentinel = plan.ಅಂಕೆ_glyphs;
    ಯದಿ (unlikely (!c->embed (sentinel)))
    {
      c->ತೆಗೆ_discard ();
      return_trace (ಅಸತ್ಯ);
    }

    plan.info.fd_select.link = c->ತೆಗೆ_pack ();
  }

  // 5. Charset (CID charset for identity mapping)
  objidx_t charset_link;
  ಯದಿ (!_serialize_cff1_charset (c, plan.ಅಂಕೆ_glyphs, charset_link))
    return_trace (ಅಸತ್ಯ);

  // 6. Global Subrs
  {
    auto *dest = c->push<CFF1Subrs> ();
    ಯದಿ (likely (dest->serialize (c, plan.subset_globalsubrs)))
      c->ತೆಗೆ_pack (ಅಸತ್ಯ);
    ಅನ್ಯಥಾ
    {
      c->ತೆಗೆ_discard ();
      return_trace (ಅಸತ್ಯ);
    }
  }

  // 7. String INDEX - Add "Adobe" and "Identity" for ROS operator
  {
    ಧ್ರುವ ವರ್ಣ *adobe_str = "Adobe";
    ಧ್ರುವ ವರ್ಣ *identity_str = "Identity";
    unsigned adobe_len = 5;  // strlen("Adobe")
    unsigned identity_len = 8;  // strlen("Identity")

    // Build strings array
    hb_vector_t<hb_ubytes_t> strings;
    strings.alloc (2);
    strings.push (hb_ubytes_t ((ಧ್ರುವ unsigned ವರ್ಣ *) adobe_str, adobe_len));
    strings.push (hb_ubytes_t ((ಧ್ರುವ unsigned ವರ್ಣ *) identity_str, identity_len));

    // Serialize as CFF INDEX
    auto *dest = c->push<CFF1Index> ();
    ಯದಿ (likely (dest->serialize (c, strings)))
      c->ತೆಗೆ_pack (ಅಸತ್ಯ);
    ಅನ್ಯಥಾ
    {
      c->ತೆಗೆ_discard ();
      return_trace (ಅಸತ್ಯ);
    }
  }

  // 8. CFF Header
  OT::cff1 *cff = c->allocate_min<OT::cff1> ();
  ಯದಿ (unlikely (!cff)) return_trace (ಅಸತ್ಯ);

  /* header */
  cff->version.major = 0x01;
  cff->version.minor = 0x00;
  cff->nameIndex = cff->min_size;
  cff->offSize = 4; /* unused? */

  // 9. Name INDEX (single entry)
  {
    unsigned name_len = strlen (CFF1_DEFAULT_FONT_NAME);

    CFF1Index *idx = c->start_embed<CFF1Index> ();
    ಯದಿ (unlikely (!idx)) return_trace (ಅಸತ್ಯ);

    ಯದಿ (unlikely (!idx->serialize_header (c, hb_iter (&name_len, 1), name_len)))
      return_trace (ಅಸತ್ಯ);

    ಯದಿ (unlikely (!c->embed (CFF1_DEFAULT_FONT_NAME, name_len)))
      return_trace (ಅಸತ್ಯ);
  }

  // 10. Top DICT INDEX
  {
    // Serialize the Top DICT data first
    c->push<ತುದಿ_Dict> ();
    cff1_from_cff2_ತುದಿ_dict_op_serializer_t ತುದಿ_Szr;

    // Serialize ROS first
    ಯದಿ (unlikely (!ತುದಿ_Szr.serialize_ros (c)))
    {
      c->ತೆಗೆ_discard ();
      return_trace (ಅಸತ್ಯ);
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

      ಯದಿ (encoder.in_error () || !c->embed (bbox_buff.as_bytes ().arrayZ, bbox_buff.length))
      {
        c->ತೆಗೆ_discard ();
        return_trace (ಅಸತ್ಯ);
      }
    }

    // Serialize charset operator
    ಯದಿ (charset_link && unlikely (!FontDict::serialize_link4_op (c, OpCode_charset, charset_link, whence_t::Absolute)))
    {
      c->ತೆಗೆ_discard ();
      return_trace (ಅಸತ್ಯ);
    }

    // Serialize FDSelect operator (required for CID-keyed CFF1 fonts)
    ಯದಿ (plan.info.fd_select.link && unlikely (!FontDict::serialize_link4_op (c, OpCode_FDSelect, plan.info.fd_select.link, whence_t::Absolute)))
    {
      c->ತೆಗೆ_discard ();
      return_trace (ಅಸತ್ಯ);
    }

    // Serialize FDArray operator (required for CID-keyed CFF1 fonts)
    ಯದಿ (plan.info.fd_array_link && unlikely (!FontDict::serialize_link4_op (c, OpCode_FDArray, plan.info.fd_array_link, whence_t::Absolute)))
    {
      c->ತೆಗೆ_discard ();
      return_trace (ಅಸತ್ಯ);
    }

    // Serialize other operators from CFF2 TopDict
    ಪ್ರತಿ (ಧ್ರುವ auto &opstr : cff2_ತುದಿ_Dict.values)
    {
      ಯದಿ (unlikely (!ತುದಿ_Szr.serialize (c, opstr, plan.info)))
      {
        c->ತೆಗೆ_discard ();
        return_trace (ಅಸತ್ಯ);
      }
    }

    unsigned ತುದಿ_size = c->length ();
    c->ತೆಗೆ_pack (ಅಸತ್ಯ);

    // Serialize INDEX header
    auto *dest = c->start_embed<CFF1Index> ();
    ಯದಿ (unlikely (!dest->serialize_header (c, hb_iter (&ತುದಿ_size, 1), ತುದಿ_size)))
      return_trace (ಅಸತ್ಯ);
  }

  return_trace (ಸತ್ಯ);
}

} /* namespace CFF */

ಸ್ಥಿರ ತಾರ್ಕಿಕ _serialize_cff2_charstrings (hb_serialize_context_t *c,
			     cff2_subset_plan &plan,
			     ಧ್ರುವ OT::cff2::accelerator_subset_t  &acc)
{
  c->push ();

  unsigned data_size = 0;
  unsigned total_size = CFF2CharStrings::total_size (plan.subset_charstrings, &data_size, plan.min_charstrings_off_size);
  ಯದಿ (unlikely (!c->start_zerocopy (total_size)))
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

  auto *cs = c->start_embed<CFF2CharStrings> ();
  ಯದಿ (unlikely (!cs->serialize (c, plan.subset_charstrings, &data_size, plan.min_charstrings_off_size)))
  {
    c->ತೆಗೆ_discard ();
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
  }

  plan.info.char_strings_link = c->ತೆಗೆ_pack (ಅಸತ್ಯ);
  ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
}

ತಾರ್ಕಿಕ
OT::cff2::accelerator_subset_t::serialize (hb_serialize_context_t *c,
					   ಸಂರಚನಾ cff2_subset_plan &plan,
					   hb_array_t<ಪೂರ್ಣಾಙ್ಕ> normalized_coords) ಧ್ರುವ
{
  /* push charstrings onto the object stack first which will ensure it packs as the last
     object in the table. Keeping the chastrings last satisfies the requirements for patching
     via IFTB. If this ordering needs to be changed in the future, charstrings should be left
     at the end whenever HB_SUBSET_FLAGS_ITFB_REQUIREMENTS is enabled. */
  ಯದಿ (!_serialize_cff2_charstrings(c, plan, *ಏತತ್))
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

  /* private dicts & local subrs */
  hb_vector_t<table_info_t>  private_dict_infos;
  ಯದಿ (unlikely (!private_dict_infos.resize (plan.subset_fdcount))) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

  ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = (ಪೂರ್ಣಾಙ್ಕ)privateDicts.length; --i >= 0 ;)
  {
    ಯದಿ (plan.fdmap.has (i))
    {
      objidx_t	subrs_link = 0;

      ಯದಿ (plan.subset_localsubrs[i].length > 0)
      {
	auto *dest = c->push <CFF2Subrs> ();
	ಯದಿ (likely (dest->serialize (c, plan.subset_localsubrs[i])))
	  subrs_link = c->ತೆಗೆ_pack (ಅಸತ್ಯ);
	ಅನ್ಯಥಾ
	{
	  c->ತೆಗೆ_discard ();
	  ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
	}
      }
      auto *pd = c->push<PrivateDict> ();
      cff2_private_dict_op_serializer_t privSzr (plan.desubroutinize, plan.drop_hints, plan.pinned,
						 varStore, normalized_coords,
						 plan.partial_instancing ? &plan.instancing : nullptr);
      ಯದಿ (likely (pd->serialize (c, privateDicts[i], privSzr, subrs_link)))
      {
	unsigned fd = plan.fdmap[i];
	private_dict_infos[fd].size = c->length ();
	private_dict_infos[fd].link = c->ತೆಗೆ_pack ();
      }
      ಅನ್ಯಥಾ
      {
	c->ತೆಗೆ_discard ();
	ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
      }
    }
  }

  /* FDSelect */
  ಯದಿ (fdSelect != &Null (CFF2FDSelect))
  {
    c->push ();
    ಯದಿ (likely (hb_serialize_cff_fdselect (c, plan.ಅಂಕೆ_glyphs, *(ಧ್ರುವ FDSelect *)fdSelect,
					   plan.orig_fdcount,
					   plan.subset_fdselect_format, plan.subset_fdselect_size,
					   plan.subset_fdselect_ranges)))
      plan.info.fd_select.link = c->ತೆಗೆ_pack ();
    ಅನ್ಯಥಾ
    {
      c->ತೆಗೆ_discard ();
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }
  }

  /* FDArray (FD Index) */
  {
    auto *fda = c->push<CFF2FDArray> ();
    cff_font_dict_op_serializer_t fontSzr;
    auto it =
    + hb_zip (+ hb_iter (fontDicts)
	      | hb_filter ([&] (ಧ್ರುವ cff2_font_dict_ಮೌಲ್ಯಗಳು_t &_)
		{ ಪ್ರತ್ಯಾಗಮ plan.fdmap.has (&_ - &fontDicts[0]); }),
	      hb_iter (private_dict_infos))
    ;
    ಯದಿ (unlikely (!fda->serialize (c, it, fontSzr)))
    {
      c->ತೆಗೆ_discard ();
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }
    plan.info.fd_array_link = c->ತೆಗೆ_pack (ಅಸತ್ಯ);
  }

  /* variation store */
  ಯದಿ (varStore != &Null (CFF2ItemVariationStore) &&
      !plan.pinned)
  {
    ಯದಿ (plan.partial_instancing)
    {
      ಯದಿ (plan.instancing.has_variations ())
      {
	c->push ();
	ಯದಿ (unlikely (!plan.instancing.serialize_var_store (c, *plan.axis_tags)))
	{
	  c->ತೆಗೆ_discard ();
	  ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
	}
	plan.info.var_store_link = c->ತೆಗೆ_pack (ಅಸತ್ಯ);
      }
      /* else: the store emptied; the vstore op gets dropped along with it. */
    }
    ಅನ್ಯಥಾ
    {
      auto *dest = c->push<CFF2ItemVariationStore> ();
      ಯದಿ (unlikely (!dest->serialize (c, varStore)))
      {
	c->ತೆಗೆ_discard ();
	ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
      }
      plan.info.var_store_link = c->ತೆಗೆ_pack (ಅಸತ್ಯ);
    }
  }

  OT::cff2 *cff2 = c->allocate_min<OT::cff2> ();
  ಯದಿ (unlikely (!cff2)) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

  /* header */
  cff2->version.major = 0x02;
  cff2->version.minor = 0x00;
  cff2->ತುದಿ_Dict_2 = OT::cff2::static_size;

  /* top dict */
  {
    ತುದಿ_Dict &dict = cff2 + cff2->ತುದಿ_Dict_2;
    cff2_ತುದಿ_dict_op_serializer_t ತುದಿ_Szr;
    ಯದಿ (unlikely (!dict.serialize (c, ತುದಿ_Dict_2, ತುದಿ_Szr, plan.info))) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    cff2->ತುದಿ_Dict_Size = c->head - (ಧ್ರುವ ವರ್ಣ *)&dict;
  }

  /* global subrs */
  {
    auto *dest = c->start_embed <CFF2Subrs> ();
    ಪ್ರತ್ಯಾಗಮ dest->serialize (c, plan.subset_globalsubrs);
  }
}

ತಾರ್ಕಿಕ
OT::cff2::accelerator_subset_t::subset (hb_subset_context_t *c) ಧ್ರುವ
{
  cff2_subset_plan cff2_plan;

  ಯದಿ (unlikely (!cff2_plan.create (*ಏತತ್, c->plan))) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

  // If instantiating (pinned) and downgrade flag is set, convert to CFF1
  ಯದಿ (cff2_plan.pinned && (c->plan->flags & HB_SUBSET_FLAGS_DOWNGRADE_CFF2))
  {
    // Serialize CFF1 to the subsetter's serializer
    // If we run out of room, returning true will cause subsetter to retry with larger buffer
    ತಾರ್ಕಿಕ result = CFF::serialize_cff2_to_cff1 (c->serializer, cff2_plan, ತುದಿ_Dict_2, *ಏತತ್);

    ಯದಿ (c->serializer->ran_out_of_room ())
      ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ; // Subsetter will retry with larger buffer

    ಯದಿ (result && !c->serializer->in_error ())
    {
      // Success - end serialization to resolve links
      c->serializer->end_serialize ();

      // Copy the serialized CFF1 data and add as CFF table
      hb_blob_t *cff_blob = c->serializer->copy_blob ();
      ಯದಿ (cff_blob)
      {
        c->plan->add_table (HB_TAG('C','F','F',' '), cff_blob);
        hb_blob_destroy (cff_blob);

        // Return false to signal CFF2 table is not needed
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
      }
    }

    // Conversion failed - don't fall back, fail hard for debugging
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
  }

  ಪ್ರತ್ಯಾಗಮ serialize (c->serializer, cff2_plan,
		    c->plan->normalized_coords.as_array ());
}

#endif
