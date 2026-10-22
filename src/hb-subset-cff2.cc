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

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_SUBSET_CFF

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-open-type.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-cff2-table.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-var-common.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-set.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-subset-plan.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-subset-cff-common.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-cff2-interp-cs.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-subset-cff2-to-cff1.hh"

ਵਰਤੋਂ ਨਾਮਾਕਾਸ਼ CFF;

ਨਾਮਾਕਾਸ਼ CFF {

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
ਬਣਤਰ cff2_instancing_plan_t
{
  ਬਣਤਰ vardata_transform_t
  {
    /* new_deltas[j] = Σ_i matrix[j][i] * old_deltas[i] */
    hb_ਸਦਿਸ਼_t<hb_ਸਦਿਸ਼_t<ਦਸ਼ਮਲਵ>> ਮੈਟ੍ਰਿਕਸ;
    /* Default-value fold-in scalars, one per old region: the old store's
     * region scalars at the new default location. */
    hb_ਸਦਿਸ਼_t<ਦਸ਼ਮਲਵ> gains;
    /* Surviving regions, parallel to matrix rows. */
    hb_ਸਦਿਸ਼_t<hb_hashmap_t<hb_ਟੈਗ_t, Triple>> regions;
    /* Index into the new global region list, per new region (finalize). */
    hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> region_indices;
    ਬਿਨਾਨਿਸ਼ਾਨੀ new_major = 0;
    ਤਾਰਕਿਕ alive_ = ਝੂਠ;
    ਤਾਰਕਿਕ alive () ਅਚਲ { ਵਾਪਸ alive_; }

    /* One value's delta for new region j, from its old per-region deltas. */
    ਦੁੱਗਣਾ transform_delta (ਬਿਨਾਨਿਸ਼ਾਨੀ j, hb_array_t<ਅਚਲ ਸੰਖਿਆ_t> deltas) ਅਚਲ
    {
      ਅਚਲ hb_ਸਦਿਸ਼_t<ਦਸ਼ਮਲਵ> &ਖਾਨਾ = ਮੈਟ੍ਰਿਕਸ.arrayZ[j];
      ਦੁੱਗਣਾ d = 0.;
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2 = hb_min (ਖਾਨਾ.ਲੰਮਾਈ, deltas.ਲੰਮਾਈ);
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ c = 0; c < ਗਿਣਤੀ_2; c++)
	d += (ਦੁੱਗਣਾ) ਖਾਨਾ.arrayZ[c] * deltas.arrayZ[c].ਮੰਜ਼ਿਲ_real ();
      ਵਾਪਸ d;
    }
  };

  hb_ਸਦਿਸ਼_t<vardata_transform_t> transforms;  /* per old VarData (vsindex) */
  hb_ਸਦਿਸ਼_t<hb_hashmap_t<hb_ਟੈਗ_t, Triple>> new_regions;  /* global region list */
  hb_ਸੈੱਟ_t ਵਰਤਿਆ_majors;
  ਬਿਨਾਨਿਸ਼ਾਨੀ new_major_ਗਿਣਤੀ = 0;

  ਤਾਰਕਿਕ has_variations () ਅਚਲ { ਵਾਪਸ new_major_ਗਿਣਤੀ; }

  ਅਚਲ vardata_transform_t *get_transform (ਬਿਨਾਨਿਸ਼ਾਨੀ major) ਅਚਲ
  { ਵਾਪਸ major < transforms.ਲੰਮਾਈ ? &transforms.arrayZ[major] : ਖਾਲੀ_ਸੂਚਕ; }

  /* Record that a retained blend references this VarData. */
  ਖਾਲੀ note_use (ਬਿਨਾਨਿਸ਼ਾਨੀ major) { ਵਰਤਿਆ_majors.ਜੋੜੋ (major); }

  /* The new vsindex a private dict's (or charstring's) old ivs maps to;
   * dead VarDatas fall back to 0, their blends having become static. */
  ਬਿਨਾਨਿਸ਼ਾਨੀ remap_ivs (ਬਿਨਾਨਿਸ਼ਾਨੀ old_ivs) ਅਚਲ
  {
    ਅਚਲ vardata_transform_t *t = get_transform (old_ivs);
    ਵਾਪਸ (t && t->alive ()) ? t->new_major : 0;
  }

  ਦੁੱਗਣਾ fold_deltas (ਬਿਨਾਨਿਸ਼ਾਨੀ old_ivs, hb_array_t<ਅਚਲ ਸੰਖਿਆ_t> deltas) ਅਚਲ
  {
    ਅਚਲ vardata_transform_t *t = get_transform (old_ivs);
    ਜੇ (unlikely (!t || t->gains.ਲੰਮਾਈ != deltas.ਲੰਮਾਈ)) ਵਾਪਸ 0.;
    ਦੁੱਗਣਾ v = 0.;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < deltas.ਲੰਮਾਈ; i++)
      v += (ਦੁੱਗਣਾ) t->gains.arrayZ[i] * deltas.arrayZ[i].ਮੰਜ਼ਿਲ_real ();
    ਵਾਪਸ v;
  }

  ਤਾਰਕਿਕ create (ਅਚਲ CFF2_ਵਸਤੂ_Variation_Store *cff2VarStore,
	       ਅਚਲ hb_subset_plan_t *plan)
  {
    ਅਚਲ OT::ਵਸਤੂ_Variation_Store &varStore = cff2VarStore->varStore;
    ਬਿਨਾਨਿਸ਼ਾਨੀ major_ਗਿਣਤੀ = varStore.get_sub_ਸਾਰਣੀ_ਗਿਣਤੀ ();
    ਜੇ (!transforms.resize (major_ਗਿਣਤੀ)) ਵਾਪਸ ਝੂਠ;

    ਜੇ (!varStore.get_region_list ().get_var_regions (plan->axes_old_ਸੂਚਕਾਂਕ_ਟੈਗ_map, orig_regions))
      ਵਾਪਸ ਝੂਠ;

    /* Instance each VarData with identity delta rows; the surviving tuples'
     * delta columns are the old-region → new-region transform. */
    OT::optimize_scratch_t scratch;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ m = 0; m < major_ਗਿਣਤੀ; m++)
    {
      ਅਚਲ OT::Var_ਅੰਕੜੇ &var_ਅੰਕੜੇ = varStore.get_sub_ਸਾਰਣੀ (m);
      ਬਿਨਾਨਿਸ਼ਾਨੀ k = var_ਅੰਕੜੇ.get_region_ਸੂਚਕਾਂਕ_ਗਿਣਤੀ ();
      vardata_transform_t &t = transforms.arrayZ[m];

      ਜੇ (!t.gains.resize_exact (k)) ਵਾਪਸ ਝੂਠ;
      varStore.get_region_scalars (m, plan->normalized_coords.arrayZ,
				   plan->normalized_coords.ਲੰਮਾਈ,
				   t.gains.arrayZ, k);

      OT::tuple_variations_t tuples;
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ r = 0; r < k; r++)
      {
	OT::tuple_delta_t tuple;
	ਜੇ (!tuple.deltas_x.resize (k) ||
	    !tuple.indices.resize (k))
	  ਵਾਪਸ ਝੂਠ;
	ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < k; i++)
	  tuple.indices.arrayZ[i] = ਸੱਚ;
	tuple.deltas_x.arrayZ[r] = 1.f;
	ਬਿਨਾਨਿਸ਼ਾਨੀ region_ਸੂਚਕਾਂਕ = var_ਅੰਕੜੇ.get_region_ਸੂਚਕਾਂਕ (r);
	ਜੇ (unlikely (region_ਸੂਚਕਾਂਕ >= orig_regions.ਲੰਮਾਈ)) ਵਾਪਸ ਝੂਠ;
	tuple.axis_tuples = orig_regions.arrayZ[region_ਸੂਚਕਾਂਕ];
	ਜੇ (unlikely (tuple.axis_tuples.ਵਿੱਚ_error ())) ਵਾਪਸ ਝੂਠ;
	tuples.tuple_vars.push (std::move (tuple));
      }
      ਜੇ (unlikely (tuples.tuple_vars.ਵਿੱਚ_error ())) ਵਾਪਸ ਝੂਠ;

      ਜੇ (!tuples.instantiate (plan->axes_location, plan->axes_triple_distances, scratch))
	ਵਾਪਸ ਝੂਠ;

      ਲਈ (ਸਵੈਚਾਲਿਤ &tuple : tuples.tuple_vars)
      {
	ਜੇ (unlikely (tuple.deltas_x.ਲੰਮਾਈ != k)) ਵਾਪਸ ਝੂਠ;
	hb_ਸਦਿਸ਼_t<ਦਸ਼ਮਲਵ> ਖਾਨਾ;
	ਜੇ (unlikely (!ਖਾਨਾ.resize_exact (k))) ਵਾਪਸ ਝੂਠ;
	ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < k; i++)
	  ਖਾਨਾ.arrayZ[i] = tuple.deltas_x.arrayZ[i];
	t.ਮੈਟ੍ਰਿਕਸ.push (std::move (ਖਾਨਾ));
	t.regions.push (std::move (tuple.axis_tuples));
      }
      ਜੇ (unlikely (t.ਮੈਟ੍ਰਿਕਸ.ਵਿੱਚ_error () || t.regions.ਵਿੱਚ_error ())) ਵਾਪਸ ਝੂਠ;
    }

    ਵਾਪਸ ਸੱਚ;
  }

  /* Decide which VarDatas survive, number them, and build the new global
   * region list: surviving original regions first, in their original order,
   * then solver-split new regions in order of appearance (fontTools'
   * rebuildRegions + prune_regions). */
  ਤਾਰਕਿਕ finalize ()
  {
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ m = 0; m < transforms.ਲੰਮਾਈ; m++)
    {
      vardata_transform_t &t = transforms.arrayZ[m];
      t.alive_ = t.ਮੈਟ੍ਰਿਕਸ.ਲੰਮਾਈ && ਵਰਤਿਆ_majors.has (m);
      ਜੇ (t.alive_)
	t.new_major = new_major_ਗਿਣਤੀ++;
    }

    /* Keys hash by content, so pointers into different owners compare fine. */
    hb_hashmap_t<ਅਚਲ hb_hashmap_t<hb_ਟੈਗ_t, Triple>*, ਬਿਨਾਨਿਸ਼ਾਨੀ> region_idx_map;
    hb_ਸਦਿਸ਼_t<ਅਚਲ hb_hashmap_t<hb_ਟੈਗ_t, Triple>*> region_ptrs;
    ਲਈ (ਅਚਲ vardata_transform_t &t : transforms)
    {
      ਜੇ (!t.alive ()) ਜਾਰੀ;
      ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ &region : t.regions)
	ਜੇ (!region_idx_map.has (&region))
	  ਜੇ (unlikely (!region_idx_map.ਸੈੱਟ (&region, (ਬਿਨਾਨਿਸ਼ਾਨੀ) -1)))
	    ਵਾਪਸ ਝੂਠ;
    }
    ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ &r : orig_regions)
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ *idx;
      ਜੇ (region_idx_map.has (&r, &idx) && *idx == (ਬਿਨਾਨਿਸ਼ਾਨੀ) -1)
      {
	ਜੇ (unlikely (!region_idx_map.ਸੈੱਟ (&r, region_ptrs.ਲੰਮਾਈ))) ਵਾਪਸ ਝੂਠ;
	region_ptrs.push (&r);
      }
    }
    ਲਈ (ਅਚਲ vardata_transform_t &t : transforms)
    {
      ਜੇ (!t.alive ()) ਜਾਰੀ;
      ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ &region : t.regions)
      {
	ਬਿਨਾਨਿਸ਼ਾਨੀ *idx;
	ਜੇ (region_idx_map.has (&region, &idx) && *idx == (ਬਿਨਾਨਿਸ਼ਾਨੀ) -1)
	{
	  ਜੇ (unlikely (!region_idx_map.ਸੈੱਟ (&region, region_ptrs.ਲੰਮਾਈ))) ਵਾਪਸ ਝੂਠ;
	  region_ptrs.push (&region);
	}
      }
    }
    ਜੇ (unlikely (region_ptrs.ਵਿੱਚ_error ())) ਵਾਪਸ ਝੂਠ;

    ਲਈ (vardata_transform_t &t : transforms)
    {
      ਜੇ (!t.alive ()) ਜਾਰੀ;
      ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ &region : t.regions)
      {
	ਬਿਨਾਨਿਸ਼ਾਨੀ *idx;
	ਜੇ (unlikely (!region_idx_map.has (&region, &idx))) ਵਾਪਸ ਝੂਠ;
	t.region_indices.push (*idx);
      }
      ਜੇ (unlikely (t.region_indices.ਵਿੱਚ_error ())) ਵਾਪਸ ਝੂਠ;
    }

    ਜੇ (!new_regions.resize (region_ptrs.ਲੰਮਾਈ)) ਵਾਪਸ ਝੂਠ;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < region_ptrs.ਲੰਮਾਈ; i++)
    {
      new_regions.arrayZ[i] = *region_ptrs.arrayZ[i];
      ਜੇ (unlikely (new_regions.arrayZ[i].ਵਿੱਚ_error ())) ਵਾਪਸ ਝੂਠ;
    }

    ਵਾਪਸ ਸੱਚ;
  }

  /* Serialize the instanced store: a CFF2ItemVariationStore whose VarDatas
   * carry no items, just region indexes (blend deltas live in charstrings).
   * Laid out manually since all sizes are known up front. */
  ਤਾਰਕਿਕ serialize_var_store (hb_serialize_context_t *c,
			    ਅਚਲ hb_ਸਦਿਸ਼_t<hb_ਟੈਗ_t> &axis_ਟੈਗ_ਸੂਚੀ) ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕੜੇ_ਗਿਣਤੀ = new_major_ਗਿਣਤੀ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਸਿਰਲੇਖ_ਮਾਪ = 2 + 4 + 2 + 4 * ਅੰਕੜੇ_ਗਿਣਤੀ;

    uint64_t ਵਿੱਥ = ਸਿਰਲੇਖ_ਮਾਪ;
    hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> ਅੰਕੜੇ_offsets;
    ਲਈ (ਅਚਲ vardata_transform_t &t : transforms)
    {
      ਜੇ (!t.alive ()) ਜਾਰੀ;
      ਅੰਕੜੇ_offsets.push ((ਬਿਨਾਨਿਸ਼ਾਨੀ) ਵਿੱਥ);
      ਵਿੱਥ += 6 + 2 * (uint64_t) t.region_indices.ਲੰਮਾਈ;
    }
    ਜੇ (unlikely (ਅੰਕੜੇ_offsets.ਵਿੱਚ_error () || ਅੰਕੜੇ_offsets.ਲੰਮਾਈ != ਅੰਕੜੇ_ਗਿਣਤੀ))
      ਵਾਪਸ ਝੂਠ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ regions_ਵਿੱਥ = (ਬਿਨਾਨਿਸ਼ਾਨੀ) ਵਿੱਥ;
    uint64_t ਕੁੱਲ = ਵਿੱਥ + 4 + (uint64_t) new_regions.ਲੰਮਾਈ * axis_ਟੈਗ_ਸੂਚੀ.ਲੰਮਾਈ * 6;
    ਜੇ (unlikely (ਕੁੱਲ > 0xFFFFFFFFu)) ਵਾਪਸ ਝੂਠ;

    HBUINT16 *ਮਾਪ_ਖੇਤਰ = c->allocate_ਮਾਪ<HBUINT16> (HBUINT16::static_ਮਾਪ);
    ਜੇ (unlikely (!ਮਾਪ_ਖੇਤਰ)) ਵਾਪਸ ਝੂਠ;
    /* OFF 2026 5.3.3.11.2 permits larger stores with length=65535. */
    *ਮਾਪ_ਖੇਤਰ = hb_min (ਕੁੱਲ, (uint64_t) 0xFFFFu);

    ਅੱਖਰ *ਸਿਰਲੇਖ = c->allocate_ਮਾਪ<ਅੱਖਰ> (ਸਿਰਲੇਖ_ਮਾਪ);
    ਜੇ (unlikely (!ਸਿਰਲੇਖ)) ਵਾਪਸ ਝੂਠ;
    * (HBUINT16 *) (ਸਿਰਲੇਖ + 0) = 1;			/* format */
    * (HBUINT32 *) (ਸਿਰਲੇਖ + 2) = regions_ਵਿੱਥ;
    * (HBUINT16 *) (ਸਿਰਲੇਖ + 6) = ਅੰਕੜੇ_ਗਿਣਤੀ;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਅੰਕੜੇ_ਗਿਣਤੀ; i++)
      * (HBUINT32 *) (ਸਿਰਲੇਖ + 8 + 4 * i) = ਅੰਕੜੇ_offsets.arrayZ[i];

    ਲਈ (ਅਚਲ vardata_transform_t &t : transforms)
    {
      ਜੇ (!t.alive ()) ਜਾਰੀ;
      ਬਿਨਾਨਿਸ਼ਾਨੀ k = t.region_indices.ਲੰਮਾਈ;
      HBUINT16 *v = (HBUINT16 *) c->allocate_ਮਾਪ<ਅੱਖਰ> (6 + 2 * k);
      ਜੇ (unlikely (!v)) ਵਾਪਸ ਝੂਠ;
      v[0] = 0;						/* itemCount */
      v[1] = 0;						/* wordSizeCount */
      v[2] = k;						/* regionIndices.len */
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ j = 0; j < k; j++)
	v[3 + j] = t.region_indices.arrayZ[j];
    }

    hb_ਸਦਿਸ਼_t<ਅਚਲ hb_hashmap_t<hb_ਟੈਗ_t, Triple>*> region_ptrs;
    ਜੇ (unlikely (!region_ptrs.alloc_exact (new_regions.ਲੰਮਾਈ))) ਵਾਪਸ ਝੂਠ;
    ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ &r : new_regions)
      region_ptrs.push (&r);
    ਸਵੈਚਾਲਿਤ *region_list = c->ਸ਼ੁਰੂ_embed<OT::VarRegionList> ();
    ਵਾਪਸ region_list->serialize (c, axis_ਟੈਗ_ਸੂਚੀ, region_ptrs);
  }

  ਨਿਜੀ:
  hb_ਸਦਿਸ਼_t<hb_hashmap_t<hb_ਟੈਗ_t, Triple>> orig_regions;
};

/* Usage-collection pass for partial instancing: record which VarDatas the
 * retained charstrings' and private dicts' blends reference.  Mirrors the
 * emission pass: blends consumed by dropped hint ops don't count. */
ਬਣਤਰ cff2_usage_param_t
{
  ਖਾਲੀ init () {}

  cff2_instancing_plan_t *instancing = ਖਾਲੀ_ਸੂਚਕ;
  ਤਾਰਕਿਕ drop_hints = ਝੂਠ;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ivs = 0;	/* dict-side current vsindex */
};

ਬਣਤਰ cff2_cs_opset_usage_t : cff2_cs_opset_t<cff2_cs_opset_usage_t, cff2_usage_param_t, blend_arg_t>
{
  ਸਥਿਰ ਖਾਲੀ flush_ਦਲੀਲਾਂ_and_op (op_ਕੋਡ_t op, cff2_cs_interp_env_t<blend_arg_t> &env, cff2_usage_param_t& param)
  {
    ਤਾਰਕਿਕ counts = ਸੱਚ;
    ਚੋਣ (op)
    {
      ਹਾਲਤ Op_ਕੋਡ_return:
      ਹਾਲਤ Op_ਕੋਡ_endchar:
	counts = ਝੂਠ;
	ਰੋਕੋ;

      ਹਾਲਤ Op_ਕੋਡ_hstem:
      ਹਾਲਤ Op_ਕੋਡ_hstemhm:
      ਹਾਲਤ Op_ਕੋਡ_vstem:
      ਹਾਲਤ Op_ਕੋਡ_vstemhm:
      ਹਾਲਤ Op_ਕੋਡ_hintmask:
      ਹਾਲਤ Op_ਕੋਡ_cntrmask:
	ਜੇ (param.drop_hints)
	  counts = ਝੂਠ;
	ਰੋਕੋ;

      ਮੂਲ:
	ਰੋਕੋ;
    }

    ਜੇ (counts)
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < env.arg_ਢੇਰ.get_ਗਿਣਤੀ (); i++)
	ਜੇ (env.arg_ਢੇਰ[i].blending ())
	{
	  param.instancing->note_use (env.get_ivs ());
	  ਰੋਕੋ;
	}

    env.clear_ਦਲੀਲਾਂ ();
  }
};

ਬਣਤਰ cff2_ਨਿੱਜੀ_dict_usage_opset_t : dict_opset_t
{
  ਸਥਿਰ ਖਾਲੀ process_op (op_ਕੋਡ_t op, cff2_priv_dict_interp_env_t& env, cff2_usage_param_t& param)
  {
    ਚੋਣ (op) {
      ਹਾਲਤ Op_ਕੋਡ_vsindexdict:
	env.process_vsindex ();
	param.ivs = env.get_ivs ();
	env.clear_ਦਲੀਲਾਂ ();
	ਵਾਪਸ;
      ਹਾਲਤ Op_ਕੋਡ_blenddict:
	param.instancing->note_use (param.ivs);
	env.clear_ਦਲੀਲਾਂ ();
	ਵਾਪਸ;
      ਮੂਲ:
	dict_opset_t::process_op (op, env);
	ਜੇ (!env.arg_ਢੇਰ.is_empty ()) ਵਾਪਸ;
	env.clear_ਦਲੀਲਾਂ ();
	ਵਾਪਸ;
    }
  }
};

} /* namespace CFF */

ਬਣਤਰ cff2_sub_ਸਾਰਣੀ_info_t : cff_sub_ਸਾਰਣੀ_info_t
{
  cff2_sub_ਸਾਰਣੀ_info_t ()
    : cff_sub_ਸਾਰਣੀ_info_t (),
      var_store_link (0)
  {}

  objidx_t  var_store_link;
};

ਬਣਤਰ cff2_ਸਿਖਰ_dict_op_serializer_t : cff_ਸਿਖਰ_dict_op_serializer_t<>
{
  ਤਾਰਕਿਕ serialize (hb_serialize_context_t *c,
		  ਅਚਲ op_str_t &opstr,
		  ਅਚਲ cff2_sub_ਸਾਰਣੀ_info_t &info) ਅਚਲ
  {
    TRACE_SERIALIZE (ਇਹ);

    ਚੋਣ (opstr.op)
    {
      ਹਾਲਤ Op_ਕੋਡ_vstore:
        ਜੇ (info.var_store_link)
	  return_trace (FontDict::serialize_link4_op(c, opstr.op, info.var_store_link));
	ਨਹੀਂਤਾਂ
	  return_trace (ਸੱਚ);

      ਮੂਲ:
	return_trace (cff_ਸਿਖਰ_dict_op_serializer_t<>::serialize (c, opstr, info));
    }
  }
};

ਬਣਤਰ cff2_cs_opset_flatten_t : cff2_cs_opset_t<cff2_cs_opset_flatten_t, flatten_param_t, blend_arg_t>
{
  ਸਥਿਰ ਖਾਲੀ flush_ਦਲੀਲਾਂ_and_op (op_ਕੋਡ_t op, cff2_cs_interp_env_t<blend_arg_t> &env, flatten_param_t& param)
  {
    /* Optionally capture command for specialization (before flushing, to preserve args) */
    ਜੇ (param.commands)
    {
      ਤਾਰਕਿਕ skip_command = ਝੂਠ;

      ਚੋਣ (op)
      {
	ਹਾਲਤ Op_ਕੋਡ_return:
	ਹਾਲਤ Op_ਕੋਡ_endchar:
	  skip_command = ਸੱਚ;
	  ਰੋਕੋ;

	ਹਾਲਤ Op_ਕੋਡ_hstem:
	ਹਾਲਤ Op_ਕੋਡ_hstemhm:
	ਹਾਲਤ Op_ਕੋਡ_vstem:
	ਹਾਲਤ Op_ਕੋਡ_vstemhm:
	ਹਾਲਤ Op_ਕੋਡ_hintmask:
	ਹਾਲਤ Op_ਕੋਡ_cntrmask:
	  ਜੇ (param.drop_hints)
	    skip_command = ਸੱਚ;
	  ਰੋਕੋ;

	ਮੂਲ:
	  ਰੋਕੋ;
      }

      ਜੇ (!skip_command)
      {
	cs_command_t cmd (op);
	/* Capture resolved blend values */
	ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < env.arg_ਢੇਰ.get_ਗਿਣਤੀ ();)
	{
	  ਅਚਲ blend_arg_t &arg = env.arg_ਢੇਰ[i];
	  ਜੇ (arg.blending ())
	  {
	    /* For blend args, capture only the resolved default value */
	    cmd.ਦਲੀਲਾਂ.push (arg);
	    /* Skip over the multiple blend values */
	    i += arg.ਅੰਕ_ਮੁੱਲਾਂ_2;
	  }
	  ਨਹੀਂਤਾਂ
	  {
	    cmd.ਦਲੀਲਾਂ.push (arg);
	    i++;
	  }
	}
	param.commands->push (cmd);
      }
    }

    ਚੋਣ (op)
    {
      ਹਾਲਤ Op_ਕੋਡ_return:
      ਹਾਲਤ Op_ਕੋਡ_endchar:
	/* dummy opcodes in CFF2. ignore */
	ਜੇ (op == Op_ਕੋਡ_endchar && param.instancer)
	  emit_vsindex_ਅਗੇਤਰ (env, param);
	ਰੋਕੋ;

      ਹਾਲਤ Op_ਕੋਡ_hstem:
      ਹਾਲਤ Op_ਕੋਡ_hstemhm:
      ਹਾਲਤ Op_ਕੋਡ_vstem:
      ਹਾਲਤ Op_ਕੋਡ_vstemhm:
      ਹਾਲਤ Op_ਕੋਡ_hintmask:
      ਹਾਲਤ Op_ਕੋਡ_cntrmask:
	ਜੇ (param.drop_hints)
	{
	  env.clear_ਦਲੀਲਾਂ ();
	  ਵਾਪਸ;
	}
	HB_FALLTHROUGH;

      ਮੂਲ:
	SUPER::flush_ਦਲੀਲਾਂ_and_op (op, env, param);
	ਰੋਕੋ;
    }
  }

  ਸਥਿਰ ਖਾਲੀ flush_ਦਲੀਲਾਂ (cff2_cs_interp_env_t<blend_arg_t> &env, flatten_param_t& param)
  {
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < env.arg_ਢੇਰ.get_ਗਿਣਤੀ ();)
    {
      ਅਚਲ blend_arg_t &arg = env.arg_ਢੇਰ[i];
      ਜੇ (arg.blending ())
      {
	ਜੇ (unlikely (!((arg.ਅੰਕ_ਮੁੱਲਾਂ_2 > 0) && (env.arg_ਢੇਰ.get_ਗਿਣਤੀ () >= arg.ਅੰਕ_ਮੁੱਲਾਂ_2))))
	{
	  env.ਸੈੱਟ_error ();
	  ਵਾਪਸ;
	}
	ਜੇ (param.instancer)
	  rewrite_blends (arg, i, env, param);
	ਨਹੀਂਤਾਂ
	  flatten_blends (arg, i, env, param);
	i += arg.ਅੰਕ_ਮੁੱਲਾਂ_2;
      }
      ਨਹੀਂਤਾਂ
      {
	str_encoder_t  encoder (param.flatStr);
	encoder.encode_ਅੰਕ_cs (arg);
	i++;
      }
    }
    SUPER::flush_ਦਲੀਲਾਂ (env, param);
  }

  ਸਥਿਰ ਖਾਲੀ flatten_blends (ਅਚਲ blend_arg_t &arg, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i, cff2_cs_interp_env_t<blend_arg_t> &env, flatten_param_t& param)
  {
    /* flatten the default values */
    str_encoder_t  encoder (param.flatStr);
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ j = 0; j < arg.ਅੰਕ_ਮੁੱਲਾਂ_2; j++)
    {
      ਅਚਲ blend_arg_t &arg1 = env.arg_ਢੇਰ[i + j];
      ਜੇ (unlikely (!((arg1.blending () && (arg.ਅੰਕ_ਮੁੱਲਾਂ_2 == arg1.ਅੰਕ_ਮੁੱਲਾਂ_2) && (arg1.ਮੁੱਲ_ਸੂਚਕਾਂਕ_2 == j) &&
	      (arg1.deltas.ਲੰਮਾਈ == env.get_region_ਗਿਣਤੀ ())))))
      {
	env.ਸੈੱਟ_error ();
	ਵਾਪਸ;
      }
      encoder.encode_ਅੰਕ_cs (arg1);
    }
    /* flatten deltas for each value */
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ j = 0; j < arg.ਅੰਕ_ਮੁੱਲਾਂ_2; j++)
    {
      ਅਚਲ blend_arg_t &arg1 = env.arg_ਢੇਰ[i + j];
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ k = 0; k < arg1.deltas.ਲੰਮਾਈ; k++)
	encoder.encode_ਅੰਕ_cs (arg1.deltas[k]);
    }
    /* flatten the number of values followed by blend operator */
    encoder.encode_int (arg.ਅੰਕ_ਮੁੱਲਾਂ_2);
    encoder.encode_op (Op_ਕੋਡ_blendcs);
  }

  /* Partial instancing: rewrite one blend group against the instanced store.
   * Each value's default absorbs the folded (pinned-away) contribution, and
   * its deltas are mapped onto the new region list. */
  ਸਥਿਰ ਖਾਲੀ rewrite_blends (ਅਚਲ blend_arg_t &arg, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i, cff2_cs_interp_env_t<blend_arg_t> &env, flatten_param_t& param)
  {
    ਅਚਲ cff2_instancing_plan_t &instancer = *param.instancer;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ivs = env.get_ivs ();
    ਅਚਲ cff2_instancing_plan_t::vardata_transform_t *t = instancer.get_transform (ivs);
    ਜੇ (unlikely (!t))
    {
      env.ਸੈੱਟ_error ();
      ਵਾਪਸ;
    }

    ਬਿਨਾਨਿਸ਼ਾਨੀ n = arg.ਅੰਕ_ਮੁੱਲਾਂ_2;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ j = 0; j < n; j++)
    {
      ਅਚਲ blend_arg_t &arg1 = env.arg_ਢੇਰ[i + j];
      ਜੇ (unlikely (!(arg1.blending () && (arg1.ਅੰਕ_ਮੁੱਲਾਂ_2 == n) && (arg1.ਮੁੱਲ_ਸੂਚਕਾਂਕ_2 == j) &&
		      (arg1.deltas.ਲੰਮਾਈ == env.get_region_ਗਿਣਤੀ ()) &&
		      (arg1.deltas.ਲੰਮਾਈ == t->gains.ਲੰਮਾਈ))))
      {
	env.ਸੈੱਟ_error ();
	ਵਾਪਸ;
      }
    }

    str_encoder_t encoder (param.flatStr);
    ਬਿਨਾਨਿਸ਼ਾਨੀ k_new = t->alive () ? t->ਮੈਟ੍ਰਿਕਸ.ਲੰਮਾਈ : 0;

    ਜੇ (!k_new)
    {
      /* All of this VarData's regions died; blends dissolve into statics. */
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ j = 0; j < n; j++)
      {
	ਅਚਲ blend_arg_t &arg1 = env.arg_ਢੇਰ[i + j];
	ਸੰਖਿਆ_t v;
	v.ਸੈੱਟ_real (arg1.ਮੰਜ਼ਿਲ_real () + round (instancer.fold_deltas (ivs, arg1.deltas.as_array ())));
	encoder.encode_ਅੰਕ_cs (v);
      }
      ਵਾਪਸ;
    }

    /* Emit in chunks so the output charstring stays within the argument
     * stack limit: values already emitted for this op, plus this chunk's
     * defaults, deltas and count, must not exceed 513. */
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਾ = 0;
    ਜਦੋਂ (ਪੂਰਾ < n)
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ avail = 512 > (i + ਪੂਰਾ) ? 512 - (i + ਪੂਰਾ) : 0;
      ਬਿਨਾਨਿਸ਼ਾਨੀ chunk = hb_min (avail / (k_new + 1), n - ਪੂਰਾ);
      ਜੇ (unlikely (!chunk))
      {
	env.ਸੈੱਟ_error ();
	ਵਾਪਸ;
      }

      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ j = ਪੂਰਾ; j < ਪੂਰਾ + chunk; j++)
      {
	ਅਚਲ blend_arg_t &arg1 = env.arg_ਢੇਰ[i + j];
	ਸੰਖਿਆ_t v;
	v.ਸੈੱਟ_real (arg1.ਮੰਜ਼ਿਲ_real () + round (instancer.fold_deltas (ivs, arg1.deltas.as_array ())));
	encoder.encode_ਅੰਕ_cs (v);
      }
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ j = ਪੂਰਾ; j < ਪੂਰਾ + chunk; j++)
      {
	ਅਚਲ blend_arg_t &arg1 = env.arg_ਢੇਰ[i + j];
	ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ r = 0; r < k_new; r++)
	{
	  ਸੰਖਿਆ_t v;
	  v.ਸੈੱਟ_int (hb_clamp_ਮੰਜ਼ਿਲ<ਪੂਰਨ> (round (t->transform_delta (r, arg1.deltas.as_array ()))));
	  encoder.encode_ਅੰਕ_cs (v);
	}
      }
      encoder.encode_int (chunk);
      encoder.encode_op (Op_ਕੋਡ_blendcs);
      ਪੂਰਾ += chunk;
    }
    param.emitted_blend = ਸੱਚ;
  }

  /* Partial instancing: prepend a vsindex op when the charstring's
   * (remapped) ivs differs from its FD's (remapped) private-dict ivs. */
  ਸਥਿਰ ਖਾਲੀ emit_vsindex_ਅਗੇਤਰ (cff2_cs_interp_env_t<blend_arg_t> &env, flatten_param_t& param)
  {
    ਜੇ (!param.emitted_blend) ਵਾਪਸ;
    ਅਚਲ cff2_instancing_plan_t &instancer = *param.instancer;
    ਬਿਨਾਨਿਸ਼ਾਨੀ new_ivs = instancer.remap_ivs (env.get_ivs ());
    ਬਿਨਾਨਿਸ਼ਾਨੀ fd_ivs = instancer.remap_ivs (env.get_orig_ivs ());
    ਜੇ (new_ivs == fd_ivs) ਵਾਪਸ;

    str_buff_t ਅਗੇਤਰ;
    str_encoder_t enc (ਅਗੇਤਰ);
    enc.encode_int ((ਪੂਰਨ) new_ivs);
    enc.encode_op (Op_ਕੋਡ_vsindexcs);

    str_buff_t &buf = param.flatStr;
    ਬਿਨਾਨਿਸ਼ਾਨੀ old_len = buf.ਲੰਮਾਈ;
    ਜੇ (unlikely (ਅਗੇਤਰ.ਵਿੱਚ_error () || !buf.resize (old_len + ਅਗੇਤਰ.ਲੰਮਾਈ)))
    {
      env.ਸੈੱਟ_error ();
      ਵਾਪਸ;
    }
    memmove (buf.arrayZ + ਅਗੇਤਰ.ਲੰਮਾਈ, buf.arrayZ, old_len);
    hb_memcpy (buf.arrayZ, ਅਗੇਤਰ.arrayZ, ਅਗੇਤਰ.ਲੰਮਾਈ);
  }

  ਸਥਿਰ ਖਾਲੀ flush_op (op_ਕੋਡ_t op, cff2_cs_interp_env_t<blend_arg_t> &env, flatten_param_t& param)
  {
    ਚੋਣ (op)
    {
      ਹਾਲਤ Op_ਕੋਡ_return:
      ਹਾਲਤ Op_ਕੋਡ_endchar:
	ਵਾਪਸ;
      ਮੂਲ:
	str_encoder_t  encoder (param.flatStr);
	encoder.encode_op (op);
    }
  }

  ਸਥਿਰ ਖਾਲੀ flush_hintmask (op_ਕੋਡ_t op, cff2_cs_interp_env_t<blend_arg_t> &env, flatten_param_t& param)
  {
    SUPER::flush_hintmask (op, env, param);
    /* Preserve hintmask payload in captured commands for specializer re-encoding. */
    ਜੇ (param.commands && !param.drop_hints && param.commands->ਲੰਮਾਈ > 0)
    {
      ਸਵੈਚਾਲਿਤ &cmd = param.commands->ਪੂਛ ();
      ਜੇ (cmd.op == op)
      {
        cmd.ਨਕਾਬ_ਬਾਈਟਾਂ.resize (env.hintmask_ਮਾਪ);
        ਜੇ (unlikely (cmd.ਨਕਾਬ_ਬਾਈਟਾਂ.ਵਿੱਚ_error ()))
        {
          env.ਸੈੱਟ_error ();
          ਵਾਪਸ;
        }
        ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < env.hintmask_ਮਾਪ; i++)
          cmd.ਨਕਾਬ_ਬਾਈਟਾਂ[i] = env.str_ref[i];
      }
    }
    ਜੇ (!param.drop_hints)
    {
      str_encoder_t  encoder (param.flatStr);
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < env.hintmask_ਮਾਪ; i++)
	encoder.encode_byte (env.str_ref[i]);
    }
  }

  ਨਿਜੀ:
  ਕਿਸਮਪਰਿਭਾਸ਼ਾ cff2_cs_opset_t<cff2_cs_opset_flatten_t, flatten_param_t, blend_arg_t> SUPER;
  ਕਿਸਮਪਰਿਭਾਸ਼ਾ cs_opset_t<blend_arg_t, cff2_cs_opset_flatten_t, cff2_cs_opset_flatten_t, cff2_cs_interp_env_t<blend_arg_t>, flatten_param_t> CSOPSET;
};

ਬਣਤਰ cff2_cs_opset_subr_subset_t : cff2_cs_opset_t<cff2_cs_opset_subr_subset_t, subr_subset_param_t, blend_arg_t>
{
  ਸਥਿਰ ਖਾਲੀ process_op (op_ਕੋਡ_t op, cff2_cs_interp_env_t<blend_arg_t> &env, subr_subset_param_t& param)
  {
    ਚੋਣ (op) {

      ਹਾਲਤ Op_ਕੋਡ_return:
	param.current_parsed_str->flush_coalesced (env.str_ref);
	param.current_parsed_str->ਸੈੱਟ_parsed ();
	env.return_from_subr ();
	param.ਸੈੱਟ_current_str (env, ਝੂਠ);
	ਰੋਕੋ;

      ਹਾਲਤ Op_ਕੋਡ_endchar:
	param.current_parsed_str->flush_coalesced (env.str_ref);
	param.current_parsed_str->ਸੈੱਟ_parsed ();
	SUPER::process_op (op, env, param);
	ਰੋਕੋ;

      ਹਾਲਤ Op_ਕੋਡ_callsubr:
	process_call_subr (op, CSType_LocalSubr, env, param, env.localSubrs, param.local_ਘੇਰਾ_2);
	ਰੋਕੋ;

      ਹਾਲਤ Op_ਕੋਡ_callgsubr:
	process_call_subr (op, CSType_GlobalSubr, env, param, env.globalSubrs, param.global_ਘੇਰਾ);
	ਰੋਕੋ;

      ਮੂਲ:
	SUPER::process_op (op, env, param);
	param.current_parsed_str->ਜੋੜੋ_op (op, env.str_ref);
	ਰੋਕੋ;
    }
  }

  ਸੁਰੱਖਿਅਤ:
  ਸਥਿਰ ਖਾਲੀ process_call_subr (op_ਕੋਡ_t op, cs_type_t type,
				 cff2_cs_interp_env_t<blend_arg_t> &env, subr_subset_param_t& param,
				 cff2_biased_subrs_t& subrs, hb_ਸੈੱਟ_t *ਘੇਰਾ)
  {
    byte_str_ref_t    str_ref = env.str_ref;
    env.call_subr (subrs, type);
    param.current_parsed_str->ਜੋੜੋ_call_op (op, str_ref, env.context.subr_ਅੰਕ_2);
    ਘੇਰਾ->ਜੋੜੋ (env.context.subr_ਅੰਕ_2);
    param.ਸੈੱਟ_current_str (env, ਸੱਚ);
  }

  ਨਿਜੀ:
  ਕਿਸਮਪਰਿਭਾਸ਼ਾ cff2_cs_opset_t<cff2_cs_opset_subr_subset_t, subr_subset_param_t, blend_arg_t> SUPER;
};

ਬਣਤਰ cff2_subr_subsetter_t : subr_subsetter_t<cff2_subr_subsetter_t, CFF2Subrs, ਅਚਲ OT::cff2::accelerator_subset_t, cff2_cs_interp_env_t<blend_arg_t>, cff2_cs_opset_subr_subset_t>
{
  cff2_subr_subsetter_t (ਅਚਲ OT::cff2::accelerator_subset_t &acc_, ਅਚਲ hb_subset_plan_t *plan_)
    : subr_subsetter_t (acc_, plan_) {}

  ਸਥਿਰ ਖਾਲੀ complete_parsed_str (cff2_cs_interp_env_t<blend_arg_t> &env, subr_subset_param_t& param, parsed_cs_str_t &charstring)
  {
    /* vsindex is inserted at the beginning of the charstring as necessary */
    ਜੇ (env.ਵੇਖਿਆ_vsindex ())
    {
      ਸੰਖਿਆ_t  ivs;
      ivs.ਸੈੱਟ_int ((ਪੂਰਨ)env.get_ivs ());
      charstring.ਸੈੱਟ_ਅਗੇਤਰ (ivs, Op_ਕੋਡ_vsindexcs);
    }
  }
};

ਬਣਤਰ cff2_ਨਿੱਜੀ_blend_encoder_param_t
{
  cff2_ਨਿੱਜੀ_blend_encoder_param_t (hb_serialize_context_t *c,
				      ਅਚਲ CFF2_ਵਸਤੂ_Variation_Store *varStore,
				      hb_array_t<ਪੂਰਨ> normalized_coords) :
    c (c), varStore (varStore), normalized_coords (normalized_coords) {}

  ਖਾਲੀ init () {}

  ਖਾਲੀ process_blend ()
  {
    ਜੇ (!ਵੇਖਿਆ_blend)
    {
      region_ਗਿਣਤੀ_2 = varStore->varStore.get_region_ਸੂਚਕਾਂਕ_ਗਿਣਤੀ (ivs);
      ਜੇ (unlikely (!scalars.resize_exact (region_ਗਿਣਤੀ_2)))
      {
	ਜੇ (c) c->err (HB_SERIALIZE_ERROR_OTHER);
	ਵੇਖਿਆ_blend = ਸੱਚ;
	ਵਾਪਸ;
      }
      varStore->varStore.get_region_scalars (ivs, normalized_coords.arrayZ, normalized_coords.ਲੰਮਾਈ,
					     &scalars[0], region_ਗਿਣਤੀ_2);
      ਵੇਖਿਆ_blend = ਸੱਚ;
    }
  }

  ਦੁੱਗਣਾ blend_deltas (hb_array_t<ਅਚਲ ਸੰਖਿਆ_t> deltas) ਅਚਲ
  {
    ਦੁੱਗਣਾ v = 0;
    ਜੇ (likely (scalars.ਲੰਮਾਈ == deltas.ਲੰਮਾਈ))
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2 = scalars.ਲੰਮਾਈ;
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਗਿਣਤੀ_2; i++)
	v += (ਦੁੱਗਣਾ) scalars.arrayZ[i] * deltas.arrayZ[i].ਮੰਜ਼ਿਲ_real ();
    }
    ਵਾਪਸ v;
  }


  hb_serialize_context_t *c = ਖਾਲੀ_ਸੂਚਕ;
  ਤਾਰਕਿਕ ਵੇਖਿਆ_blend = ਝੂਠ;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ivs = 0;
  ਬਿਨਾਨਿਸ਼ਾਨੀ region_ਗਿਣਤੀ_2 = 0;
  hb_ਸਦਿਸ਼_t<ਦਸ਼ਮਲਵ> scalars;
  ਅਚਲ	 CFF2_ਵਸਤੂ_Variation_Store *varStore = ਖਾਲੀ_ਸੂਚਕ;
  hb_array_t<ਪੂਰਨ> normalized_coords;

  /* CFF2 partial instancing: when set, blends are rewritten against the
   * instanced variation store instead of being flattened. */
  ਅਚਲ cff2_instancing_plan_t *instancer = ਖਾਲੀ_ਸੂਚਕ;
  /* Error-compensated fold rounding state, per dict operator. */
  ਦੁੱਗਣਾ fold_exact = 0.;
  ਦੁੱਗਣਾ fold_emitted = 0.;
};

ਬਣਤਰ cff2_ਨਿੱਜੀ_dict_blend_opset_t : dict_opset_t
{
  ਸਥਿਰ ਖਾਲੀ process_arg_blend (cff2_ਨਿੱਜੀ_blend_encoder_param_t& param,
				 ਸੰਖਿਆ_t &arg,
				 ਅਚਲ hb_array_t<ਅਚਲ ਸੰਖਿਆ_t> blends,
				 ਬਿਨਾਨਿਸ਼ਾਨੀ n, ਬਿਨਾਨਿਸ਼ਾਨੀ i)
  {
    arg.ਸੈੱਟ_int (round (arg.ਮੰਜ਼ਿਲ_real () + param.blend_deltas (blends)));
  }

  /* The fold added to a value, error-compensated across the operator: most
   * blended private-dict values (BlueValues, StemSnap*, ...) are
   * delta-encoded, so independently rounded folds would accumulate in the
   * decoded absolutes; keeping the emitted cumulative fold equal to the
   * rounded exact cumulative fold bounds every absolute within ±0.5. */
  ਸਥਿਰ ਦੁੱਗਣਾ fold (cff2_ਨਿੱਜੀ_blend_encoder_param_t& param,
		      ਅਚਲ hb_array_t<ਅਚਲ ਸੰਖਿਆ_t> blends)
  {
    ਦੁੱਗਣਾ f = param.blend_deltas (blends);
    ਦੁੱਗਣਾ emit = round (param.fold_exact + f) - param.fold_emitted;
    param.fold_exact += f;
    param.fold_emitted += emit;
    ਵਾਪਸ emit;
  }

  /* Partial instancing: emit the args preceding this blend group, then the
   * group itself rewritten against the instanced store, and clear the
   * stack.  The op's remaining args are flushed at process_op time. */
  ਸਥਿਰ ਖਾਲੀ rewrite_blends (cff2_priv_dict_interp_env_t& env,
			      cff2_ਨਿੱਜੀ_blend_encoder_param_t& param,
			      ਬਿਨਾਨਿਸ਼ਾਨੀ ਸ਼ੁਰੂ, ਬਿਨਾਨਿਸ਼ਾਨੀ n, ਬਿਨਾਨਿਸ਼ਾਨੀ k)
  {
    ਅਚਲ cff2_instancing_plan_t &instancer = *param.instancer;
    ਅਚਲ cff2_instancing_plan_t::vardata_transform_t *t = instancer.get_transform (param.ivs);
    ਜੇ (unlikely (!t || t->gains.ਲੰਮਾਈ != k))
    {
      env.ਸੈੱਟ_error ();
      ਵਾਪਸ;
    }

    str_buff_t str;
    str_encoder_t encoder (str);

    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ idx = 0; idx < ਸ਼ੁਰੂ; idx++)
      encoder.encode_ਅੰਕ_tp (env.arg_ਢੇਰ[idx]);

    ਬਿਨਾਨਿਸ਼ਾਨੀ k_new = t->alive () ? t->ਮੈਟ੍ਰਿਕਸ.ਲੰਮਾਈ : 0;
    ਜੇ (!k_new)
    {
      /* This VarData died (or is unused by any retained charstring, which
       * can happen for FDs kept only for retain-gids holes); blends
       * dissolve into statics. */
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ j = 0; j < n; j++)
      {
	ਅਚਲ hb_array_t<ਅਚਲ ਸੰਖਿਆ_t> blends = env.arg_ਢੇਰ.sub_array (ਸ਼ੁਰੂ + n + (j * k), k);
	ਸੰਖਿਆ_t v;
	v.ਸੈੱਟ_real (env.arg_ਢੇਰ[ਸ਼ੁਰੂ + j].ਮੰਜ਼ਿਲ_real () + fold (param, blends));
	encoder.encode_ਅੰਕ_tp (v);
      }
    }
    ਨਹੀਂਤਾਂ
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਾ = 0;
      ਜਦੋਂ (ਪੂਰਾ < n)
      {
	ਬਿਨਾਨਿਸ਼ਾਨੀ avail = 512 > (ਸ਼ੁਰੂ + ਪੂਰਾ) ? 512 - (ਸ਼ੁਰੂ + ਪੂਰਾ) : 0;
	ਬਿਨਾਨਿਸ਼ਾਨੀ chunk = hb_min (avail / (k_new + 1), n - ਪੂਰਾ);
	ਜੇ (unlikely (!chunk))
	{
	  env.ਸੈੱਟ_error ();
	  ਵਾਪਸ;
	}

	ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ j = ਪੂਰਾ; j < ਪੂਰਾ + chunk; j++)
	{
	  ਅਚਲ hb_array_t<ਅਚਲ ਸੰਖਿਆ_t> blends = env.arg_ਢੇਰ.sub_array (ਸ਼ੁਰੂ + n + (j * k), k);
	  ਸੰਖਿਆ_t v;
	  v.ਸੈੱਟ_real (env.arg_ਢੇਰ[ਸ਼ੁਰੂ + j].ਮੰਜ਼ਿਲ_real () + fold (param, blends));
	  encoder.encode_ਅੰਕ_tp (v);
	}
	ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ j = ਪੂਰਾ; j < ਪੂਰਾ + chunk; j++)
	{
	  ਅਚਲ hb_array_t<ਅਚਲ ਸੰਖਿਆ_t> blends = env.arg_ਢੇਰ.sub_array (ਸ਼ੁਰੂ + n + (j * k), k);
	  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ r = 0; r < k_new; r++)
	  {
	    ਸੰਖਿਆ_t v;
	    v.ਸੈੱਟ_int (hb_clamp_ਮੰਜ਼ਿਲ<ਪੂਰਨ> (round (t->transform_delta (r, blends))));
	    encoder.encode_ਅੰਕ_tp (v);
	  }
	}
	encoder.encode_int (chunk);
	encoder.encode_op (Op_ਕੋਡ_blenddict);
	ਪੂਰਾ += chunk;
      }
    }

    ਸਵੈਚਾਲਿਤ ਬਾਈਟਾਂ = str.as_ਬਾਈਟਾਂ ();
    param.c->embed (&ਬਾਈਟਾਂ, ਬਾਈਟਾਂ.ਲੰਮਾਈ);

    env.clear_ਦਲੀਲਾਂ ();
  }

  ਸਥਿਰ ਖਾਲੀ process_blend (cff2_priv_dict_interp_env_t& env, cff2_ਨਿੱਜੀ_blend_encoder_param_t& param)
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ n, k;

    param.process_blend ();
    k = param.region_ਗਿਣਤੀ_2;
    n = env.arg_ਢੇਰ.ਕੱਢੋ_uint ();

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2 = env.arg_ਢੇਰ.get_ਗਿਣਤੀ ();
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਕੁੱਲ;
    ਜੇ (unlikely (hb_unsigned_mul_overflows (k + 1, n, &ਕੁੱਲ) || ਕੁੱਲ > ਗਿਣਤੀ_2))
    {
      env.ਸੈੱਟ_error ();
      ਵਾਪਸ;
    }

    /* copy the blend values into blend array of the default values */
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸ਼ੁਰੂ = ਗਿਣਤੀ_2 - ਕੁੱਲ;
    /* let an obvious error case fail, but note CFF2 spec doesn't forbid n==0 */
    ਜੇ (unlikely (ਸ਼ੁਰੂ > ਗਿਣਤੀ_2))
    {
      env.ਸੈੱਟ_error ();
      ਵਾਪਸ;
    }

    ਜੇ (param.instancer && k)
    {
      rewrite_blends (env, param, ਸ਼ੁਰੂ, n, k);
      ਵਾਪਸ;
    }

    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < n; i++)
    {
      ਅਚਲ hb_array_t<ਅਚਲ ਸੰਖਿਆ_t> blends = env.arg_ਢੇਰ.sub_array (ਸ਼ੁਰੂ + n + (i * k), k);
      process_arg_blend (param, env.arg_ਢੇਰ[ਸ਼ੁਰੂ + i], blends, n, i);
    }

    /* pop off blend values leaving default values now adorned with blend values */
    env.arg_ਢੇਰ.ਕੱਢੋ (k * n);
  }

  ਸਥਿਰ ਖਾਲੀ process_op (op_ਕੋਡ_t op, cff2_priv_dict_interp_env_t& env, cff2_ਨਿੱਜੀ_blend_encoder_param_t& param)
  {
    ਚੋਣ (op) {
      ਹਾਲਤ Op_ਕੋਡ_Std_HW:
      ਹਾਲਤ Op_ਕੋਡ_Std_VW:
      ਹਾਲਤ Op_ਕੋਡ_Blue_Scale:
      ਹਾਲਤ Op_ਕੋਡ_Blue_ਖਿਸਕਾਓ:
      ਹਾਲਤ Op_ਕੋਡ_Blue_Fuzz:
      ਹਾਲਤ Op_ਕੋਡ_Expansion_ਗੁਣਕ:
      ਹਾਲਤ Op_ਕੋਡ_Language_ਸਮੂਹ:
      ਹਾਲਤ Op_ਕੋਡ_Blue_ਮੁੱਲਾਂ:
      ਹਾਲਤ Op_ਕੋਡ_Other_Blues:
      ਹਾਲਤ Op_ਕੋਡ_Family_Blues:
      ਹਾਲਤ Op_ਕੋਡ_Family_Other_Blues:
      ਹਾਲਤ Op_ਕੋਡ_Stem_Snap_H:
      ਹਾਲਤ Op_ਕੋਡ_Stem_Snap_V:
	ਰੋਕੋ;
      ਹਾਲਤ Op_ਕੋਡ_vsindexdict:
	env.process_vsindex ();
	param.ivs = env.get_ivs ();
	ਜੇ (param.instancer)
	{
	  /* Re-emit with the remapped vsindex; 0 is the dict default. */
	  ਬਿਨਾਨਿਸ਼ਾਨੀ new_ivs = param.instancer->remap_ivs (param.ivs);
	  ਜੇ (new_ivs)
	  {
	    str_buff_t str;
	    str_encoder_t encoder (str);
	    encoder.encode_int ((ਪੂਰਨ) new_ivs);
	    encoder.encode_op (Op_ਕੋਡ_vsindexdict);
	    ਸਵੈਚਾਲਿਤ ਬਾਈਟਾਂ = str.as_ਬਾਈਟਾਂ ();
	    param.c->embed (&ਬਾਈਟਾਂ, ਬਾਈਟਾਂ.ਲੰਮਾਈ);
	  }
	}
	env.clear_ਦਲੀਲਾਂ ();
	ਵਾਪਸ;
      ਹਾਲਤ Op_ਕੋਡ_blenddict:
	process_blend (env, param);
	ਵਾਪਸ;

      ਮੂਲ:
	dict_opset_t::process_op (op, env);
	ਜੇ (!env.arg_ਢੇਰ.is_empty ()) ਵਾਪਸ;
	ਰੋਕੋ;
    }

    ਜੇ (unlikely (env.ਵਿੱਚ_error ())) ਵਾਪਸ;

    // Write args then op

    str_buff_t str;
    str_encoder_t encoder (str);

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2 = env.arg_ਢੇਰ.get_ਗਿਣਤੀ ();
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਗਿਣਤੀ_2; i++)
      encoder.encode_ਅੰਕ_tp (env.arg_ਢੇਰ[i]);

    encoder.encode_op (op);

    ਸਵੈਚਾਲਿਤ ਬਾਈਟਾਂ = str.as_ਬਾਈਟਾਂ ();
    param.c->embed (&ਬਾਈਟਾਂ, ਬਾਈਟਾਂ.ਲੰਮਾਈ);

    env.clear_ਦਲੀਲਾਂ ();
    param.fold_exact = param.fold_emitted = 0.;
  }
};

ਬਣਤਰ cff2_ਨਿੱਜੀ_dict_op_serializer_t : op_serializer_t
{
  cff2_ਨਿੱਜੀ_dict_op_serializer_t (ਤਾਰਕਿਕ desubroutinize_, ਤਾਰਕਿਕ drop_hints_, ਤਾਰਕਿਕ pinned_,
				     ਅਚਲ CFF::CFF2_ਵਸਤੂ_Variation_Store* varStore_,
				     hb_array_t<ਪੂਰਨ> normalized_coords_,
				     ਅਚਲ cff2_instancing_plan_t *instancer_ = ਖਾਲੀ_ਸੂਚਕ)
    : desubroutinize (desubroutinize_), drop_hints (drop_hints_), pinned (pinned_),
      param (ਖਾਲੀ_ਸੂਚਕ, varStore_, normalized_coords_)
  { param.instancer = instancer_; }

  ਤਾਰਕਿਕ serialize (hb_serialize_context_t *c,
		  ਅਚਲ op_str_t &opstr,
		  objidx_t subrs_link) ਅਚਲ
  {
    TRACE_SERIALIZE (ਇਹ);

    ਜੇ (drop_hints && dict_opset_t::is_hint_op (opstr.op))
      return_trace (ਸੱਚ);

    ਜੇ (opstr.op == Op_ਕੋਡ_Subrs)
    {
      ਜੇ (desubroutinize || !subrs_link)
	return_trace (ਸੱਚ);
      ਨਹੀਂਤਾਂ
	return_trace (FontDict::serialize_link2_op (c, opstr.op, subrs_link));
    }

    ਜੇ (pinned || param.instancer)
    {
      /* Reinterpret opstr and process blends.  The param persists across
       * this dict's opstrs, so that an ivs set by a vsindexdict opstr is
       * still in effect when a later opstr's blends are processed. */
      cff2_priv_dict_interp_env_t env {hb_ubytes_t (opstr.ptr, opstr.ਲੰਮਾਈ)};
      param.c = c;
      dict_interpreter_t<cff2_ਨਿੱਜੀ_dict_blend_opset_t, cff2_ਨਿੱਜੀ_blend_encoder_param_t, cff2_priv_dict_interp_env_t> interp (env);
      return_trace (interp.interpret (param));
    }

    return_trace (copy_opstr (c, opstr));
  }

  ਸੁਰੱਖਿਅਤ:
  ਅਚਲ ਤਾਰਕਿਕ desubroutinize;
  ਅਚਲ ਤਾਰਕਿਕ drop_hints;
  ਅਚਲ ਤਾਰਕਿਕ pinned;
  ਬਦਲਣਯੋਗ cff2_ਨਿੱਜੀ_blend_encoder_param_t param;
};



ਨਾਮਾਕਾਸ਼ OT {
ਬਣਤਰ cff2_subset_plan
{
  ਤਾਰਕਿਕ create (ਅਚਲ OT::cff2::accelerator_subset_t &acc,
	      hb_subset_plan_t *plan)
  {
    /* make sure notdef is first */
    hb_codepoint_t old_glyph;
    ਜੇ (!plan->old_gid_for_new_gid (0, &old_glyph) || (old_glyph != 0)) ਵਾਪਸ ਝੂਠ;

    ਅੰਕ_glyphs = plan->ਅੰਕ_output_glyphs ();
    orig_fdcount = acc.fdArray->ਗਿਣਤੀ_2;

    drop_hints = plan->flags & HB_SUBSET_FLAGS_NO_HINTING;
    ਤਾਰਕਿਕ instancing_requested = (ਤਾਰਕਿਕ) plan->normalized_coords;
    pinned = instancing_requested && plan->all_axes_pinned;
    normalized_coords = plan->normalized_coords;
    ਸਿਰ_maxp_info = plan->ਸਿਰ_maxp_info;
    hmtx_map = &plan->hmtx_map;
    desubroutinize = plan->flags & HB_SUBSET_FLAGS_DESUBROUTINIZE ||
		     instancing_requested; // For instancing we need this path

    /* Partial instancing: some axes pinned or limited, others kept. */
    partial_instancing = instancing_requested && !pinned &&
			 acc.varStore != &Null (CFF2_ਵਸਤੂ_Variation_Store);
    ਜੇ (partial_instancing &&
	unlikely (!instancing.create (acc.varStore, plan)))
      ਵਾਪਸ ਝੂਠ;
    axis_ਟੈਗ_ਸੂਚੀ = &plan->axis_ਟੈਗ_ਸੂਚੀ;

    /* Enable command capture for CFF2→CFF1 conversion (for specialization) */
    capture_commands = pinned;

 #ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HB_EXPERIMENTAL_API
    min_charstrings_off_ਮਾਪ = (plan->flags & HB_SUBSET_FLAGS_IFTB_REQUIREMENTS) ? 4 : 0;
 #ਨਹੀਂਤਾਂ
    min_charstrings_off_ਮਾਪ = 0;
 #ਜੇ_ਅੰਤ

    ਜੇ (desubroutinize)
    {
      ਜੇ (partial_instancing)
      {
	/* Pass 1: collect which VarDatas the retained blends reference, so
	 * unused ones get pruned and the survivors renumbered. */
	hb_ਸੈੱਟ_t ਵਰਤਿਆ_fds;
	ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਅੰਕ_glyphs; i++)
	{
	  hb_codepoint_t glyph;
	  ਜੇ (!plan->old_gid_for_new_gid (i, &glyph))
	    ਜਾਰੀ;
	  ਅਚਲ hb_ubytes_t str = (*acc.char_ਲੜੀਆਂ)[glyph];
	  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ fd = CFF::subset_get_fd (acc, glyph);
	  ਜੇ (unlikely (fd >= acc.fd_ਗਿਣਤੀ))
	    ਵਾਪਸ ਝੂਠ;
	  ਵਰਤਿਆ_fds.ਜੋੜੋ (fd);

	  cff2_cs_interp_env_t<blend_arg_t> env (str, acc, fd);
	  cs_interpreter_t<cff2_cs_interp_env_t<blend_arg_t>, cff2_cs_opset_usage_t, cff2_usage_param_t> interp (env);
	  cff2_usage_param_t param;
	  param.instancing = &instancing;
	  param.drop_hints = drop_hints;
	  ਜੇ (unlikely (!interp.interpret (param)))
	    ਵਾਪਸ ਝੂਠ;
	}
	ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ fd : ਵਰਤਿਆ_fds)
	{
	  cff2_usage_param_t param;
	  param.instancing = &instancing;
	  ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2 = acc.ਨਿੱਜੀ_Dicts[fd].get_ਗਿਣਤੀ ();
	  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ j = 0; j < ਗਿਣਤੀ_2; j++)
	  {
	    ਅਚਲ op_str_t &opstr = acc.ਨਿੱਜੀ_Dicts[fd][j];
	    ਜੇ (drop_hints && dict_opset_t::is_hint_op (opstr.op))
	      ਜਾਰੀ;
	    cff2_priv_dict_interp_env_t env {hb_ubytes_t (opstr.ptr, opstr.ਲੰਮਾਈ)};
	    dict_interpreter_t<cff2_ਨਿੱਜੀ_dict_usage_opset_t, cff2_usage_param_t, cff2_priv_dict_interp_env_t> interp (env);
	    ਜੇ (unlikely (!interp.interpret (param)))
	      ਵਾਪਸ ਝੂਠ;
	  }
	}
	ਜੇ (unlikely (!instancing.finalize ()))
	  ਵਾਪਸ ਝੂਠ;

	/* Pass 2: like subr_flattener_t, but blends stay symbolic (no coords
	 * fed to the env) and get rewritten against the instanced store. */
	ਜੇ (!subset_charstrings.resize_exact (ਅੰਕ_glyphs))
	  ਵਾਪਸ ਝੂਠ;
	ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਅੰਕ_glyphs; i++)
	{
	  hb_codepoint_t glyph;
	  ਜੇ (!plan->old_gid_for_new_gid (i, &glyph))
	    ਜਾਰੀ;
	  ਅਚਲ hb_ubytes_t str = (*acc.char_ਲੜੀਆਂ)[glyph];
	  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ fd = CFF::subset_get_fd (acc, glyph);
	  ਜੇ (unlikely (fd >= acc.fd_ਗਿਣਤੀ))
	    ਵਾਪਸ ਝੂਠ;

	  cff2_cs_interp_env_t<blend_arg_t> env (str, acc, fd);
	  cs_interpreter_t<cff2_cs_interp_env_t<blend_arg_t>, cff2_cs_opset_flatten_t, flatten_param_t> interp (env);
	  flatten_param_t param = {
	    subset_charstrings.arrayZ[i],
	    (ਤਾਰਕਿਕ) (plan->flags & HB_SUBSET_FLAGS_NO_HINTING),
	    plan
	  };
	  param.instancer = &instancing;
	  ਜੇ (unlikely (!interp.interpret (param)))
	    ਵਾਪਸ ਝੂਠ;
	}
      }
      ਨਹੀਂਤਾਂ
      {
      /* Flatten global & local subrs */
      subr_flattener_t<ਅਚਲ OT::cff2::accelerator_subset_t, cff2_cs_interp_env_t<blend_arg_t>, cff2_cs_opset_flatten_t>
		    flattener(acc, plan);

      /* Enable command capture if requested (for specialization) */
      ਜੇ (capture_commands)
      {
	ਜੇ (!charstring_commands.resize_exact (ਅੰਕ_glyphs))
	  ਵਾਪਸ ਝੂਠ;

	ਜੇ (!flattener.flatten (subset_charstrings, &charstring_commands))
	  ਵਾਪਸ ਝੂਠ;
      }
      ਨਹੀਂਤਾਂ
      {
	ਜੇ (!flattener.flatten (subset_charstrings))
	  ਵਾਪਸ ਝੂਠ;
      }
      }
    }
    ਨਹੀਂਤਾਂ
    {
      cff2_subr_subsetter_t	subr_subsetter (acc, plan);

      /* Subset subrs: collect used subroutines, leaving all unused ones behind */
      ਜੇ (!subr_subsetter.subset ())
	ਵਾਪਸ ਝੂਠ;

      /* encode charstrings, global subrs, local subrs with new subroutine numbers */
      ਜੇ (!subr_subsetter.encode_charstrings (subset_charstrings, !pinned))
	ਵਾਪਸ ਝੂਠ;

      ਜੇ (!subr_subsetter.encode_globalsubrs (subset_globalsubrs))
	ਵਾਪਸ ਝੂਠ;

      /* local subrs */
      ਜੇ (!subset_localsubrs.resize (orig_fdcount))
	ਵਾਪਸ ਝੂਠ;
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ fd = 0; fd < orig_fdcount; fd++)
      {
	subset_localsubrs[fd].init ();
	ਜੇ (!subr_subsetter.encode_localsubrs (fd, subset_localsubrs[fd]))
	  ਵਾਪਸ ਝੂਠ;
      }
    }

    /* FDSelect */
    ਜੇ (acc.fdSelect != &Null (CFF2FDSelect))
    {
      ਜੇ (unlikely (!hb_plan_subset_cff_fdselect (plan,
						  orig_fdcount,
						  *(ਅਚਲ FDSelect *)acc.fdSelect,
						  acc.ਅੰਕ_glyphs,
						  subset_fdcount,
						  subset_fdselect_ਮਾਪ,
						  subset_fdselect_format,
						  subset_fdselect_ranges,
						  fdmap)))
	ਵਾਪਸ ਝੂਠ;
    }
    ਨਹੀਂਤਾਂ
      fdmap.ਪਛਾਣ_ਤੱਤ (1);

    ਵਾਪਸ ਸੱਚ;
  }

  cff2_sub_ਸਾਰਣੀ_info_t info;

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ    ਅੰਕ_glyphs;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ    orig_fdcount = 0;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ    subset_fdcount = 1;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ    subset_fdselect_ਮਾਪ = 0;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ    subset_fdselect_format = 0;
  ਤਾਰਕਿਕ            pinned = ਝੂਠ;
  hb_ਸਦਿਸ਼_t<ਕੋਡ_pair_t>   subset_fdselect_ranges;

  hb_inc_bimap_t   fdmap;

  str_buff_vec_t	    subset_charstrings;
  str_buff_vec_t	    subset_globalsubrs;
  hb_ਸਦਿਸ਼_t<str_buff_vec_t> subset_localsubrs;

  ਤਾਰਕਿਕ	    drop_hints = ਝੂਠ;
  ਤਾਰਕਿਕ	    desubroutinize = ਝੂਠ;

  ਤਾਰਕਿਕ	    partial_instancing = ਝੂਠ;
  cff2_instancing_plan_t instancing;
  ਅਚਲ hb_ਸਦਿਸ਼_t<hb_ਟੈਗ_t> *axis_ਟੈਗ_ਸੂਚੀ = ਖਾਲੀ_ਸੂਚਕ;

  ਬਿਨਾਨਿਸ਼ਾਨੀ  min_charstrings_off_ਮਾਪ = 0;

  hb_array_t<ਪੂਰਨ> normalized_coords; // For instantiation
  ਸਿਰ_maxp_info_t ਸਿਰ_maxp_info;  // For FontBBox
  ਅਚਲ hb_hashmap_t<hb_codepoint_t, hb_pair_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਪੂਰਨ>> *hmtx_map; // For widths

  // Width optimization results (for CFF1 conversion)
  ਬਿਨਾਨਿਸ਼ਾਨੀ default_ਚੌੜਾਈ = 0;
  ਬਿਨਾਨਿਸ਼ਾਨੀ nominal_ਚੌੜਾਈ = 0;

  // Command capture for specialization (CFF2→CFF1 conversion)
  ਤਾਰਕਿਕ capture_commands = ਝੂਠ;
  hb_ਸਦਿਸ਼_t<hb_ਸਦਿਸ਼_t<cs_command_t>> charstring_commands;
};
} // namespace OT

/*
 * CFF2 to CFF1 Converter Implementation
 */

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-cff-width-optimizer.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-cff-specializer.hh"

/* Serialize charstrings using CFF1 format with widths */
ਸਥਿਰ ਤਾਰਕਿਕ
_serialize_cff1_charstrings (hb_serialize_context_t *c,
                             OT::cff2_subset_plan &plan,
                             ਬਿਨਾਨਿਸ਼ਾਨੀ default_ਚੌੜਾਈ,
                             ਬਿਨਾਨਿਸ਼ਾਨੀ nominal_ਚੌੜਾਈ)
{
  c->push ();

  // CFF1 requires:
  // 1. Width at the beginning (if != defaultWidthX)
  // 2. endchar at the end
  str_buff_vec_t cff1_charstrings;
  ਜੇ (unlikely (!cff1_charstrings.resize (plan.subset_charstrings.ਲੰਮਾਈ)))
  {
    c->ਕੱਢੋ_discard ();
    ਵਾਪਸ ਝੂਠ;
  }

  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < plan.subset_charstrings.ਲੰਮਾਈ; i++)
  {
    // Get width for this glyph from hmtx_map
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਚੌੜਾਈ = 0;
    ਜੇ (plan.hmtx_map->has (i))
      ਚੌੜਾਈ = plan.hmtx_map->get (i).ਪਹਿਲਾ;

    // Encode width if different from default
    str_encoder_t encoder (cff1_charstrings[i]);
    ਜੇ (ਚੌੜਾਈ != default_ਚੌੜਾਈ)
    {
      ਪੂਰਨ delta = (ਪੂਰਨ) ਚੌੜਾਈ - (ਪੂਰਨ) nominal_ਚੌੜਾਈ;
      encoder.encode_int (delta);
    }

    // Use specialized commands if available, otherwise use binary
    ਜੇ (plan.capture_commands && i < plan.charstring_commands.ਲੰਮਾਈ &&
        plan.charstring_commands[i].ਲੰਮਾਈ > 0)
    {
      // Specialize and encode commands
      ਸਵੈਚਾਲਿਤ &commands = plan.charstring_commands[i];
      CFF::specialize_commands (commands, 48);  /* maxstack=48 for CFF1 */
      ਜੇ (unlikely (!CFF::encode_commands (commands, cff1_charstrings[i])))
      {
        c->ਕੱਢੋ_discard ();
        ਵਾਪਸ ਝੂਠ;
      }
    }
    ਨਹੀਂਤਾਂ
    {
      // Use binary CharString
      ਅਚਲ str_buff_t &cs = plan.subset_charstrings[i];
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ j = 0; j < cs.ਲੰਮਾਈ; j++)
        cff1_charstrings[i].push (cs[j]);
    }

    // Check if it already ends with endchar (0x0e) or return (0x0b)
    ਜੇ (cff1_charstrings[i].ਲੰਮਾਈ == 0 ||
        (cff1_charstrings[i].ਪੂਛ () != 0x0e && cff1_charstrings[i].ਪੂਛ () != 0x0b))
    {
      // Append endchar operator
      ਜੇ (unlikely (!cff1_charstrings[i].ਧੱਕੋ_or_fail (0x0e)))
      {
        c->ਕੱਢੋ_discard ();
        ਵਾਪਸ ਝੂਠ;
      }
    }
  }

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕੜੇ_ਮਾਪ = 0;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਕੁੱਲ_ਮਾਪ = CFF1_Char_ਲੜੀਆਂ::ਕੁੱਲ_ਮਾਪ (cff1_charstrings, &ਅੰਕੜੇ_ਮਾਪ, plan.min_charstrings_off_ਮਾਪ);
  ਜੇ (unlikely (!c->ਸ਼ੁਰੂ_zerocopy (ਕੁੱਲ_ਮਾਪ)))
  {
    c->ਕੱਢੋ_discard ();
    ਵਾਪਸ ਝੂਠ;
  }

  ਸਵੈਚਾਲਿਤ *cs = c->ਸ਼ੁਰੂ_embed<CFF1_Char_ਲੜੀਆਂ> ();
  ਜੇ (unlikely (!cs->serialize (c, cff1_charstrings, &ਅੰਕੜੇ_ਮਾਪ, plan.min_charstrings_off_ਮਾਪ)))
  {
    c->ਕੱਢੋ_discard ();
    ਵਾਪਸ ਝੂਠ;
  }

  plan.info.char_ਲੜੀਆਂ_link = c->ਕੱਢੋ_pack (ਝੂਠ);
  ਵਾਪਸ ਸੱਚ;
}

/* Serialize CID Charset (format 2 range: gid 0-N -> cid 0-N) */
ਸਥਿਰ ਤਾਰਕਿਕ
_serialize_cff1_charset (hb_serialize_context_t *c,
                         ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ_glyphs,
                         objidx_t &charset_link)
{
  // For CID fonts, create a simple identity charset
  // Format 2: one range covering all glyphs (except .notdef)
  c->push ();

  ਸਵੈਚਾਲਿਤ *charset = c->ਸ਼ੁਰੂ_embed<Charset> ();
  ਜੇ (unlikely (!charset))
  {
    c->ਕੱਢੋ_discard ();
    ਵਾਪਸ ਝੂਠ;
  }

  // Create a single range for CID 1 to num_glyphs-1
  hb_ਸਦਿਸ਼_t<ਕੋਡ_pair_t> ranges;
  ਜੇ (ਅੰਕ_glyphs > 1)
  {
    ਕੋਡ_pair_t range;
    range.ਕੋਡ = 1;  // first CID
    range.glyph = ਅੰਕ_glyphs - 2;  // nLeft (covers glyphs 1 to num_glyphs-1)
    ranges.push (range);
  }

  ਜੇ (unlikely (!charset->serialize (c, 2, ਅੰਕ_glyphs, ranges)))
  {
    c->ਕੱਢੋ_discard ();
    ਵਾਪਸ ਝੂਠ;
  }

  charset_link = c->ਕੱਢੋ_pack ();
  ਵਾਪਸ ਸੱਚ;
}

/* CFF2 to CFF1 serialization */
ਨਾਮਾਕਾਸ਼ CFF {

ਤਾਰਕਿਕ
serialize_cff2_ਮੰਜ਼ਿਲ_cff1 (hb_serialize_context_t *c,
                        OT::cff2_subset_plan &plan,
                        ਅਚਲ cff2_ਸਿਖਰ_dict_ਮੁੱਲਾਂ_t &cff2_ਸਿਖਰ_Dict,
                        ਅਚਲ OT::cff2::accelerator_subset_t &acc)
{
  TRACE_SERIALIZE (ਇਹ);

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
    hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> widths;
    widths.alloc (plan.ਅੰਕ_glyphs);

    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ gid = 0; gid < plan.ਅੰਕ_glyphs; gid++)
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਚੌੜਾਈ = 0;
      ਜੇ (plan.hmtx_map->has (gid))
        ਚੌੜਾਈ = plan.hmtx_map->get (gid).ਪਹਿਲਾ;
      widths.push (ਚੌੜਾਈ);
    }

    // Optimize defaultWidthX and nominalWidthX
    CFF::optimize_widths (widths, plan.default_ਚੌੜਾਈ, plan.nominal_ਚੌੜਾਈ);
  }

  // 1. CharStrings (with widths prepended)
  ਜੇ (!_serialize_cff1_charstrings (c, plan, plan.default_ਚੌੜਾਈ, plan.nominal_ਚੌੜਾਈ))
    return_trace (ਝੂਠ);

  // 2. Private DICs & Local Subrs (same as CFF2)
  hb_ਸਦਿਸ਼_t<ਸਾਰਣੀ_info_t> ਨਿੱਜੀ_dict_infos;
  ਜੇ (unlikely (!ਨਿੱਜੀ_dict_infos.resize (plan.subset_fdcount)))
    return_trace (ਝੂਠ);

  ਲਈ (ਪੂਰਨ i = (ਪੂਰਨ)acc.ਨਿੱਜੀ_Dicts.ਲੰਮਾਈ; --i >= 0;)
  {
    ਜੇ (plan.fdmap.has (i))
    {
      objidx_t subrs_link = 0;

      ਜੇ (plan.subset_localsubrs[i].ਲੰਮਾਈ > 0)
      {
        ਸਵੈਚਾਲਿਤ *dest = c->push<CFF1Subrs> ();
        ਜੇ (likely (dest->serialize (c, plan.subset_localsubrs[i])))
          subrs_link = c->ਕੱਢੋ_pack (ਝੂਠ);
        ਨਹੀਂਤਾਂ
        {
          c->ਕੱਢੋ_discard ();
          return_trace (ਝੂਠ);
        }
      }

      ਸਵੈਚਾਲਿਤ *pd = c->push<ਨਿੱਜੀ_Dict> ();
      // Use the CFF2 Private DICT serializer which instantiates blends when pinned=true
      cff2_ਨਿੱਜੀ_dict_op_serializer_t privSzr (plan.desubroutinize, plan.drop_hints, plan.pinned,
                                                  acc.varStore, plan.normalized_coords);
      ਜੇ (likely (pd->serialize (c, acc.ਨਿੱਜੀ_Dicts[i], privSzr, subrs_link)))
      {
        // Add defaultWidthX and nominalWidthX for CFF1
        str_buff_t ਚੌੜਾਈ_ops;
        str_encoder_t encoder (ਚੌੜਾਈ_ops);
        encoder.encode_int (plan.default_ਚੌੜਾਈ);
        encoder.encode_op (Op_ਕੋਡ_default_ਚੌੜਾਈ_X);
        encoder.encode_int (plan.nominal_ਚੌੜਾਈ);
        encoder.encode_op (Op_ਕੋਡ_nominal_ਚੌੜਾਈ_X);

        ਜੇ (!encoder.ਵਿੱਚ_error () && c->embed (ਚੌੜਾਈ_ops.as_ਬਾਈਟਾਂ ().arrayZ, ਚੌੜਾਈ_ops.ਲੰਮਾਈ))
        {
          ਬਿਨਾਨਿਸ਼ਾਨੀ fd = plan.fdmap[i];
          ਨਿੱਜੀ_dict_infos[fd].ਮਾਪ = c->ਲੰਮਾਈ ();
          ਨਿੱਜੀ_dict_infos[fd].link = c->ਕੱਢੋ_pack ();
        }
        ਨਹੀਂਤਾਂ
        {
          c->ਕੱਢੋ_discard ();
          return_trace (ਝੂਠ);
        }
      }
      ਨਹੀਂਤਾਂ
      {
        c->ਕੱਢੋ_discard ();
        return_trace (ਝੂਠ);
      }
    }
  }

  // 3. FDArray - serialize CFF2 font dicts as CFF1
  {
    ਸਵੈਚਾਲਿਤ *fda = c->push<FDArray<HBUINT16>> ();
    cff_font_dict_op_serializer_t fontSzr;
    ਸਵੈਚਾਲਿਤ it =
    + hb_zip (+ hb_iter (acc.fontDicts)
              | hb_filter ([&] (ਅਚਲ cff2_font_dict_ਮੁੱਲਾਂ_t &_)
                { ਵਾਪਸ plan.fdmap.has (&_ - &acc.fontDicts[0]); }),
              hb_iter (ਨਿੱਜੀ_dict_infos))
    ;
    // Explicitly specify template parameters: DICTVAL, INFO
    ਤਾਰਕਿਕ success = fda->serialize<cff2_font_dict_ਮੁੱਲਾਂ_t, ਸਾਰਣੀ_info_t> (c, it, fontSzr);
    ਜੇ (success)
      plan.info.fd_array_link = c->ਕੱਢੋ_pack (ਝੂਠ);
    ਨਹੀਂਤਾਂ
    {
      c->ਕੱਢੋ_discard ();
      return_trace (ਝੂਠ);
    }
  }

  // 4. FDSelect (required in CFF1 CID-keyed fonts)
  // CFF1 requires FDSelect for all CID-keyed fonts, even with just one FD
  // CFF2 makes it optional when there's only one FD
  ਜੇ (acc.fdSelect != &Null (CFF2FDSelect))
  {
    c->push ();
    ਜੇ (likely (hb_serialize_cff_fdselect (c, plan.ਅੰਕ_glyphs,
                                          *(ਅਚਲ FDSelect *)acc.fdSelect,
                                          plan.orig_fdcount,
                                          plan.subset_fdselect_format,
                                          plan.subset_fdselect_ਮਾਪ,
                                          plan.subset_fdselect_ranges)))
      plan.info.fd_select.link = c->ਕੱਢੋ_pack ();
    ਨਹੀਂਤਾਂ
    {
      c->ਕੱਢੋ_discard ();
      return_trace (ਝੂਠ);
    }
  }
  ਨਹੀਂਤਾਂ
  {
    // Create a range-based FDSelect3 mapping all glyphs to FD 0
    // Format: format(1) + nRanges(2) + range(3) + sentinel(2) = 8 bytes
    c->push ();

    // Format byte
    HBUINT8 format;
    format = 3;
    ਜੇ (unlikely (!c->embed (format)))
    {
      c->ਕੱਢੋ_discard ();
      return_trace (ਝੂਠ);
    }

    // nRanges
    HBUINT16 nRanges;
    nRanges = 1;
    ਜੇ (unlikely (!c->embed (nRanges)))
    {
      c->ਕੱਢੋ_discard ();
      return_trace (ਝੂਠ);
    }

    // Single range: {first: 0, fd: 0}
    FDSelect3_Range range;
    range.ਪਹਿਲਾ = 0;
    range.fd = 0;
    ਜੇ (unlikely (!c->embed (range)))
    {
      c->ਕੱਢੋ_discard ();
      return_trace (ਝੂਠ);
    }

    // Sentinel (number of glyphs)
    HBUINT16 sentinel;
    sentinel = plan.ਅੰਕ_glyphs;
    ਜੇ (unlikely (!c->embed (sentinel)))
    {
      c->ਕੱਢੋ_discard ();
      return_trace (ਝੂਠ);
    }

    plan.info.fd_select.link = c->ਕੱਢੋ_pack ();
  }

  // 5. Charset (CID charset for identity mapping)
  objidx_t charset_link;
  ਜੇ (!_serialize_cff1_charset (c, plan.ਅੰਕ_glyphs, charset_link))
    return_trace (ਝੂਠ);

  // 6. Global Subrs
  {
    ਸਵੈਚਾਲਿਤ *dest = c->push<CFF1Subrs> ();
    ਜੇ (likely (dest->serialize (c, plan.subset_globalsubrs)))
      c->ਕੱਢੋ_pack (ਝੂਠ);
    ਨਹੀਂਤਾਂ
    {
      c->ਕੱਢੋ_discard ();
      return_trace (ਝੂਠ);
    }
  }

  // 7. String INDEX - Add "Adobe" and "Identity" for ROS operator
  {
    ਅਚਲ ਅੱਖਰ *adobe_str = "Adobe";
    ਅਚਲ ਅੱਖਰ *ਪਛਾਣ_ਤੱਤ_str = "Identity";
    ਬਿਨਾਨਿਸ਼ਾਨੀ adobe_len = 5;  // strlen("Adobe")
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪਛਾਣ_ਤੱਤ_len = 8;  // strlen("Identity")

    // Build strings array
    hb_ਸਦਿਸ਼_t<hb_ubytes_t> ਲੜੀਆਂ;
    ਲੜੀਆਂ.alloc (2);
    ਲੜੀਆਂ.push (hb_ubytes_t ((ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *) adobe_str, adobe_len));
    ਲੜੀਆਂ.push (hb_ubytes_t ((ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *) ਪਛਾਣ_ਤੱਤ_str, ਪਛਾਣ_ਤੱਤ_len));

    // Serialize as CFF INDEX
    ਸਵੈਚਾਲਿਤ *dest = c->push<CFF1_ਸੂਚਕਾਂਕ> ();
    ਜੇ (likely (dest->serialize (c, ਲੜੀਆਂ)))
      c->ਕੱਢੋ_pack (ਝੂਠ);
    ਨਹੀਂਤਾਂ
    {
      c->ਕੱਢੋ_discard ();
      return_trace (ਝੂਠ);
    }
  }

  // 8. CFF Header
  OT::cff1 *cff = c->allocate_min<OT::cff1> ();
  ਜੇ (unlikely (!cff)) return_trace (ਝੂਠ);

  /*ਸਿਰਲੇਖ*/
  cff->ਸੰਸਕਰਣ.major = 0x01;
  cff->ਸੰਸਕਰਣ.minor = 0x00;
  cff->name_ਸੂਚਕਾਂਕ = cff->min_ਮਾਪ;
  cff->off_ਮਾਪ = 4; /* unused? */

  // 9. Name INDEX (single entry)
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ name_len = ਲੜੀ_ਲੰਬਾਈ (CFF1_DEFAULT_FONT_NAME);

    CFF1_ਸੂਚਕਾਂਕ *idx = c->ਸ਼ੁਰੂ_embed<CFF1_ਸੂਚਕਾਂਕ> ();
    ਜੇ (unlikely (!idx)) return_trace (ਝੂਠ);

    ਜੇ (unlikely (!idx->serialize_ਸਿਰਲੇਖ (c, hb_iter (&name_len, 1), name_len)))
      return_trace (ਝੂਠ);

    ਜੇ (unlikely (!c->embed (CFF1_DEFAULT_FONT_NAME, name_len)))
      return_trace (ਝੂਠ);
  }

  // 10. Top DICT INDEX
  {
    // Serialize the Top DICT data first
    c->push<ਸਿਖਰ_Dict> ();
    cff1_from_cff2_ਸਿਖਰ_dict_op_serializer_t ਸਿਖਰ_Szr;

    // Serialize ROS first
    ਜੇ (unlikely (!ਸਿਖਰ_Szr.serialize_ros (c)))
    {
      c->ਕੱਢੋ_discard ();
      return_trace (ਝੂਠ);
    }

    // Serialize FontBBox from head table
    {
      str_buff_t bbox_buff;
      str_encoder_t encoder (bbox_buff);

      encoder.encode_int (plan.ਸਿਰ_maxp_info.xMin);
      encoder.encode_int (plan.ਸਿਰ_maxp_info.yMin);
      encoder.encode_int (plan.ਸਿਰ_maxp_info.xMax);
      encoder.encode_int (plan.ਸਿਰ_maxp_info.yMax);
      encoder.encode_op (Op_ਕੋਡ_Font_BBox);

      ਜੇ (encoder.ਵਿੱਚ_error () || !c->embed (bbox_buff.as_ਬਾਈਟਾਂ ().arrayZ, bbox_buff.ਲੰਮਾਈ))
      {
        c->ਕੱਢੋ_discard ();
        return_trace (ਝੂਠ);
      }
    }

    // Serialize charset operator
    ਜੇ (charset_link && unlikely (!FontDict::serialize_link4_op (c, Op_ਕੋਡ_charset, charset_link, whence_t::Absolute)))
    {
      c->ਕੱਢੋ_discard ();
      return_trace (ਝੂਠ);
    }

    // Serialize FDSelect operator (required for CID-keyed CFF1 fonts)
    ਜੇ (plan.info.fd_select.link && unlikely (!FontDict::serialize_link4_op (c, Op_ਕੋਡ_FDSelect, plan.info.fd_select.link, whence_t::Absolute)))
    {
      c->ਕੱਢੋ_discard ();
      return_trace (ਝੂਠ);
    }

    // Serialize FDArray operator (required for CID-keyed CFF1 fonts)
    ਜੇ (plan.info.fd_array_link && unlikely (!FontDict::serialize_link4_op (c, Op_ਕੋਡ_FDArray, plan.info.fd_array_link, whence_t::Absolute)))
    {
      c->ਕੱਢੋ_discard ();
      return_trace (ਝੂਠ);
    }

    // Serialize other operators from CFF2 TopDict
    ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ &opstr : cff2_ਸਿਖਰ_Dict.values)
    {
      ਜੇ (unlikely (!ਸਿਖਰ_Szr.serialize (c, opstr, plan.info)))
      {
        c->ਕੱਢੋ_discard ();
        return_trace (ਝੂਠ);
      }
    }

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਸਿਖਰ_ਮਾਪ = c->ਲੰਮਾਈ ();
    c->ਕੱਢੋ_pack (ਝੂਠ);

    // Serialize INDEX header
    ਸਵੈਚਾਲਿਤ *dest = c->ਸ਼ੁਰੂ_embed<CFF1_ਸੂਚਕਾਂਕ> ();
    ਜੇ (unlikely (!dest->serialize_ਸਿਰਲੇਖ (c, hb_iter (&ਸਿਖਰ_ਮਾਪ, 1), ਸਿਖਰ_ਮਾਪ)))
      return_trace (ਝੂਠ);
  }

  return_trace (ਸੱਚ);
}

} /* namespace CFF */

ਸਥਿਰ ਤਾਰਕਿਕ _serialize_cff2_charstrings (hb_serialize_context_t *c,
			     cff2_subset_plan &plan,
			     ਅਚਲ OT::cff2::accelerator_subset_t  &acc)
{
  c->push ();

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕੜੇ_ਮਾਪ = 0;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਕੁੱਲ_ਮਾਪ = CFF2_Char_ਲੜੀਆਂ::ਕੁੱਲ_ਮਾਪ (plan.subset_charstrings, &ਅੰਕੜੇ_ਮਾਪ, plan.min_charstrings_off_ਮਾਪ);
  ਜੇ (unlikely (!c->ਸ਼ੁਰੂ_zerocopy (ਕੁੱਲ_ਮਾਪ)))
    ਵਾਪਸ ਝੂਠ;

  ਸਵੈਚਾਲਿਤ *cs = c->ਸ਼ੁਰੂ_embed<CFF2_Char_ਲੜੀਆਂ> ();
  ਜੇ (unlikely (!cs->serialize (c, plan.subset_charstrings, &ਅੰਕੜੇ_ਮਾਪ, plan.min_charstrings_off_ਮਾਪ)))
  {
    c->ਕੱਢੋ_discard ();
    ਵਾਪਸ ਝੂਠ;
  }

  plan.info.char_ਲੜੀਆਂ_link = c->ਕੱਢੋ_pack (ਝੂਠ);
  ਵਾਪਸ ਸੱਚ;
}

ਤਾਰਕਿਕ
OT::cff2::accelerator_subset_t::serialize (hb_serialize_context_t *c,
					   ਬਣਤਰ cff2_subset_plan &plan,
					   hb_array_t<ਪੂਰਨ> normalized_coords) ਅਚਲ
{
  /* push charstrings onto the object stack first which will ensure it packs as the last
     object in the table. Keeping the chastrings last satisfies the requirements for patching
     via IFTB. If this ordering needs to be changed in the future, charstrings should be left
     at the end whenever HB_SUBSET_FLAGS_ITFB_REQUIREMENTS is enabled. */
  ਜੇ (!_serialize_cff2_charstrings(c, plan, *ਇਹ))
    ਵਾਪਸ ਝੂਠ;

  /* private dicts & local subrs */
  hb_ਸਦਿਸ਼_t<ਸਾਰਣੀ_info_t>  ਨਿੱਜੀ_dict_infos;
  ਜੇ (unlikely (!ਨਿੱਜੀ_dict_infos.resize (plan.subset_fdcount))) ਵਾਪਸ ਝੂਠ;

  ਲਈ (ਪੂਰਨ i = (ਪੂਰਨ)ਨਿੱਜੀ_Dicts.ਲੰਮਾਈ; --i >= 0 ;)
  {
    ਜੇ (plan.fdmap.has (i))
    {
      objidx_t	subrs_link = 0;

      ਜੇ (plan.subset_localsubrs[i].ਲੰਮਾਈ > 0)
      {
	ਸਵੈਚਾਲਿਤ *dest = c->push <CFF2Subrs> ();
	ਜੇ (likely (dest->serialize (c, plan.subset_localsubrs[i])))
	  subrs_link = c->ਕੱਢੋ_pack (ਝੂਠ);
	ਨਹੀਂਤਾਂ
	{
	  c->ਕੱਢੋ_discard ();
	  ਵਾਪਸ ਝੂਠ;
	}
      }
      ਸਵੈਚਾਲਿਤ *pd = c->push<ਨਿੱਜੀ_Dict> ();
      cff2_ਨਿੱਜੀ_dict_op_serializer_t privSzr (plan.desubroutinize, plan.drop_hints, plan.pinned,
						 varStore, normalized_coords,
						 plan.partial_instancing ? &plan.instancing : ਖਾਲੀ_ਸੂਚਕ);
      ਜੇ (likely (pd->serialize (c, ਨਿੱਜੀ_Dicts[i], privSzr, subrs_link)))
      {
	ਬਿਨਾਨਿਸ਼ਾਨੀ fd = plan.fdmap[i];
	ਨਿੱਜੀ_dict_infos[fd].ਮਾਪ = c->ਲੰਮਾਈ ();
	ਨਿੱਜੀ_dict_infos[fd].link = c->ਕੱਢੋ_pack ();
      }
      ਨਹੀਂਤਾਂ
      {
	c->ਕੱਢੋ_discard ();
	ਵਾਪਸ ਝੂਠ;
      }
    }
  }

  /* FDSelect */
  ਜੇ (fdSelect != &Null (CFF2FDSelect))
  {
    c->push ();
    ਜੇ (likely (hb_serialize_cff_fdselect (c, plan.ਅੰਕ_glyphs, *(ਅਚਲ FDSelect *)fdSelect,
					   plan.orig_fdcount,
					   plan.subset_fdselect_format, plan.subset_fdselect_ਮਾਪ,
					   plan.subset_fdselect_ranges)))
      plan.info.fd_select.link = c->ਕੱਢੋ_pack ();
    ਨਹੀਂਤਾਂ
    {
      c->ਕੱਢੋ_discard ();
      ਵਾਪਸ ਝੂਠ;
    }
  }

  /* FDArray (FD Index) */
  {
    ਸਵੈਚਾਲਿਤ *fda = c->push<CFF2FDArray> ();
    cff_font_dict_op_serializer_t fontSzr;
    ਸਵੈਚਾਲਿਤ it =
    + hb_zip (+ hb_iter (fontDicts)
	      | hb_filter ([&] (ਅਚਲ cff2_font_dict_ਮੁੱਲਾਂ_t &_)
		{ ਵਾਪਸ plan.fdmap.has (&_ - &fontDicts[0]); }),
	      hb_iter (ਨਿੱਜੀ_dict_infos))
    ;
    ਜੇ (unlikely (!fda->serialize (c, it, fontSzr)))
    {
      c->ਕੱਢੋ_discard ();
      ਵਾਪਸ ਝੂਠ;
    }
    plan.info.fd_array_link = c->ਕੱਢੋ_pack (ਝੂਠ);
  }

  /* variation store */
  ਜੇ (varStore != &Null (CFF2_ਵਸਤੂ_Variation_Store) &&
      !plan.pinned)
  {
    ਜੇ (plan.partial_instancing)
    {
      ਜੇ (plan.instancing.has_variations ())
      {
	c->push ();
	ਜੇ (unlikely (!plan.instancing.serialize_var_store (c, *plan.axis_ਟੈਗ_ਸੂਚੀ)))
	{
	  c->ਕੱਢੋ_discard ();
	  ਵਾਪਸ ਝੂਠ;
	}
	plan.info.var_store_link = c->ਕੱਢੋ_pack (ਝੂਠ);
      }
      /* else: the store emptied; the vstore op gets dropped along with it. */
    }
    ਨਹੀਂਤਾਂ
    {
      ਸਵੈਚਾਲਿਤ *dest = c->push<CFF2_ਵਸਤੂ_Variation_Store> ();
      ਜੇ (unlikely (!dest->serialize (c, varStore)))
      {
	c->ਕੱਢੋ_discard ();
	ਵਾਪਸ ਝੂਠ;
      }
      plan.info.var_store_link = c->ਕੱਢੋ_pack (ਝੂਠ);
    }
  }

  OT::cff2 *cff2 = c->allocate_min<OT::cff2> ();
  ਜੇ (unlikely (!cff2)) ਵਾਪਸ ਝੂਠ;

  /*ਸਿਰਲੇਖ*/
  cff2->ਸੰਸਕਰਣ.major = 0x02;
  cff2->ਸੰਸਕਰਣ.minor = 0x00;
  cff2->ਸਿਖਰ_Dict_2 = OT::cff2::static_ਮਾਪ;

  /* top dict */
  {
    ਸਿਖਰ_Dict &dict = cff2 + cff2->ਸਿਖਰ_Dict_2;
    cff2_ਸਿਖਰ_dict_op_serializer_t ਸਿਖਰ_Szr;
    ਜੇ (unlikely (!dict.serialize (c, ਸਿਖਰ_Dict_2, ਸਿਖਰ_Szr, plan.info))) ਵਾਪਸ ਝੂਠ;
    cff2->ਸਿਖਰ_Dict_ਮਾਪ = c->ਸਿਰ_2 - (ਅਚਲ ਅੱਖਰ *)&dict;
  }

  /* global subrs */
  {
    ਸਵੈਚਾਲਿਤ *dest = c->ਸ਼ੁਰੂ_embed <CFF2Subrs> ();
    ਵਾਪਸ dest->serialize (c, plan.subset_globalsubrs);
  }
}

ਤਾਰਕਿਕ
OT::cff2::accelerator_subset_t::subset (hb_subset_context_t *c) ਅਚਲ
{
  cff2_subset_plan cff2_plan;

  ਜੇ (unlikely (!cff2_plan.create (*ਇਹ, c->plan))) ਵਾਪਸ ਝੂਠ;

  // If instantiating (pinned) and downgrade flag is set, convert to CFF1
  ਜੇ (cff2_plan.pinned && (c->plan->flags & HB_SUBSET_FLAGS_DOWNGRADE_CFF2))
  {
    // Serialize CFF1 to the subsetter's serializer
    // If we run out of room, returning true will cause subsetter to retry with larger buffer
    ਤਾਰਕਿਕ ਨਤੀਜਾ = CFF::serialize_cff2_ਮੰਜ਼ਿਲ_cff1 (c->serializer, cff2_plan, ਸਿਖਰ_Dict_2, *ਇਹ);

    ਜੇ (c->serializer->ran_ਬਾਹਰਲਾ_ਦਾ_ਗੁੰਜਾਇਸ਼ ())
      ਵਾਪਸ ਸੱਚ; // Subsetter will retry with larger buffer

    ਜੇ (ਨਤੀਜਾ && !c->serializer->ਵਿੱਚ_error ())
    {
      // Success - end serialization to resolve links
      c->serializer->ਸਿਰਾ_serialize ();

      // Copy the serialized CFF1 data and add as CFF table
      hb_blob_t *cff_blob = c->serializer->copy_blob ();
      ਜੇ (cff_blob)
      {
        c->plan->ਜੋੜੋ_ਸਾਰਣੀ (HB_ਟੈਗ('C','F','F',' '), cff_blob);
        hb_blob_destroy (cff_blob);

        // Return false to signal CFF2 table is not needed
        ਵਾਪਸ ਝੂਠ;
      }
    }

    // Conversion failed - don't fall back, fail hard for debugging
    ਵਾਪਸ ਝੂਠ;
  }

  ਵਾਪਸ serialize (c->serializer, cff2_plan,
		    c->plan->normalized_coords.as_array ());
}

#ਜੇ_ਅੰਤ
