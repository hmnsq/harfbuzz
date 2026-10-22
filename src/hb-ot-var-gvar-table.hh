/*
 * Copyright © 2019  Adobe Inc.
 * Copyright © 2019  Ebrahim Byagowi
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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_OT_VAR_GVAR_ਸਾਰਣੀ_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_VAR_GVAR_ਸਾਰਣੀ_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-decycler.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-open-type.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-dual.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-var-common.hh"

/*
 * gvar -- Glyph Variation Table
 * https://docs.microsoft.com/en-us/typography/opentype/spec/gvar
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_ਟੈਗ_gvar HB_ਟੈਗ('g','v','a','r')
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_ਟੈਗ_GVAR HB_ਟੈਗ('G','V','A','R')

ਬਣਤਰ hb_glyf_scratch_t
{
  // glyf
  contour_ਬਿੰਦੂ_ਸਦਿਸ਼_t all_points;
  contour_ਬਿੰਦੂ_ਸਦਿਸ਼_t comp_points;
  hb_decycler_t decycler;

  // gvar
  contour_ਬਿੰਦੂ_ਸਦਿਸ਼_t orig_points;
  hb_ਸਦਿਸ਼_t<ਪੂਰਨ> x_deltas;
  hb_ਸਦਿਸ਼_t<ਪੂਰਨ> y_deltas;
  contour_ਬਿੰਦੂ_ਸਦਿਸ਼_t deltas;
  hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ> shared_indices;
  hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ> ਨਿੱਜੀ_indices;
};

ਨਾਮਾਕਾਸ਼ OT {

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ ਵਿੱਥ_Type>
ਬਣਤਰ glyph_variations_t
{
  // TODO: Move tuple_variations_t to outside of TupleVariationData
  ਵਰਤੋਂ tuple_variations_t = ਕਿਸਮ_ਨਾਂ Tuple_Variation_ਅੰਕੜੇ<ਵਿੱਥ_Type>::tuple_variations_t;
  ਵਰਤੋਂ Glyph_Variation_ਅੰਕੜੇ = Tuple_Variation_ਅੰਕੜੇ<ਵਿੱਥ_Type>;

  hb_ਸਦਿਸ਼_t<tuple_variations_t> glyph_variations;

  hb_ਸਦਿਸ਼_t<F2DOT14> compiled_shared_tuples;
  ਨਿਜੀ:
  ਬਿਨਾਨਿਸ਼ਾਨੀ shared_tuples_ਗਿਣਤੀ = 0;

  /* shared coords-> index map after instantiation */
  hb_hashmap_t<ਅਚਲ hb_ਸਦਿਸ਼_t<F2DOT14>*, ਬਿਨਾਨਿਸ਼ਾਨੀ> shared_tuples_idx_map;

  hb_alloc_pool_t pool;

  ਜਨਤਕ:
  ਬਿਨਾਨਿਸ਼ਾਨੀ compiled_shared_tuples_ਗਿਣਤੀ () ਅਚਲ
  { ਵਾਪਸ shared_tuples_ਗਿਣਤੀ; }

  ਬਿਨਾਨਿਸ਼ਾਨੀ compiled_byte_ਮਾਪ () ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ byte_ਮਾਪ = 0;
    ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ& _ : glyph_variations)
      byte_ਮਾਪ += _.get_compiled_byte_ਮਾਪ ();

    ਵਾਪਸ byte_ਮਾਪ;
  }

  ਤਾਰਕਿਕ create_from_glyphs_var_ਅੰਕੜੇ (ਬਿਨਾਨਿਸ਼ਾਨੀ axis_ਗਿਣਤੀ,
                                    ਅਚਲ hb_array_t<ਅਚਲ F2DOT14> shared_tuples,
                                    ਅਚਲ hb_subset_plan_t *plan,
                                    ਅਚਲ hb_hashmap_t<hb_codepoint_t, hb_ਬਾਈਟਾਂ_t>& new_gid_var_ਅੰਕੜੇ_map)
  {
    ਜੇ (unlikely (!glyph_variations.alloc_exact (plan->new_ਮੰਜ਼ਿਲ_old_gid_list.ਲੰਮਾਈ)))
      ਵਾਪਸ ਝੂਠ;

    ਸਵੈਚਾਲਿਤ it = hb_iter (plan->new_ਮੰਜ਼ਿਲ_old_gid_list);
    ਲਈ (ਸਵੈਚਾਲਿਤ &_ : it)
    {
      hb_codepoint_t new_gid = _.ਪਹਿਲਾ;
      contour_ਬਿੰਦੂ_ਸਦਿਸ਼_t *all_contour_points;
      ਜੇ (!new_gid_var_ਅੰਕੜੇ_map.has (new_gid) ||
          !plan->new_gid_contour_points_map.has (new_gid, &all_contour_points))
        ਵਾਪਸ ਝੂਠ;
      hb_ਬਾਈਟਾਂ_t var_ਅੰਕੜੇ = new_gid_var_ਅੰਕੜੇ_map.get (new_gid);

      ਅਚਲ Glyph_Variation_ਅੰਕੜੇ* p = reinterpret_cast<ਅਚਲ Glyph_Variation_ਅੰਕੜੇ*> (var_ਅੰਕੜੇ.arrayZ);
      ਕਿਸਮ_ਨਾਂ Glyph_Variation_ਅੰਕੜੇ::tuple_iterator_t iterator;
      tuple_variations_t tuple_vars;

      hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> shared_indices;

      /* in case variation data is empty, push an empty struct into the vector,
       * keep the vector in sync with the new_to_old_gid_list */
      ਜੇ (!var_ਅੰਕੜੇ || ! p->has_ਅੰਕੜੇ () || !all_contour_points->ਲੰਮਾਈ ||
          !Glyph_Variation_ਅੰਕੜੇ::get_tuple_iterator (var_ਅੰਕੜੇ, axis_ਗਿਣਤੀ,
                                                   var_ਅੰਕੜੇ.arrayZ,
                                                   shared_indices, &iterator))
      {
        glyph_variations.push (std::move (tuple_vars));
        ਜਾਰੀ;
      }

      ਤਾਰਕਿਕ is_ਸੰਯੁਕਤ_glyph = ਝੂਠ;
      is_ਸੰਯੁਕਤ_glyph = plan->ਸੰਯੁਕਤ_new_gids.has (new_gid);

      ਜੇ (!p->decompile_tuple_variations (all_contour_points->ਲੰਮਾਈ, ਸੱਚ /* is_gvar */,
                                          iterator, &(plan->axes_old_ਸੂਚਕਾਂਕ_ਟੈਗ_map),
                                          shared_indices, shared_tuples,
                                          tuple_vars, /*ਬਾਹਰਲਾ*/
					  &pool,
                                          is_ਸੰਯੁਕਤ_glyph))
        ਵਾਪਸ ਝੂਠ;
      glyph_variations.push (std::move (tuple_vars));
    }
    ਵਾਪਸ !glyph_variations.ਵਿੱਚ_error () && glyph_variations.ਲੰਮਾਈ == plan->new_ਮੰਜ਼ਿਲ_old_gid_list.ਲੰਮਾਈ;
  }

  ਤਾਰਕਿਕ instantiate (ਅਚਲ hb_subset_plan_t *plan)
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2 = plan->new_ਮੰਜ਼ਿਲ_old_gid_list.ਲੰਮਾਈ;
    ਤਾਰਕਿਕ iup_optimize = ਝੂਠ;
    optimize_scratch_t scratch;
    iup_optimize = plan->flags & HB_SUBSET_FLAGS_OPTIMIZE_IUP_DELTAS;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਗਿਣਤੀ_2; i++)
    {
      hb_codepoint_t new_gid = plan->new_ਮੰਜ਼ਿਲ_old_gid_list[i].ਪਹਿਲਾ;
      contour_ਬਿੰਦੂ_ਸਦਿਸ਼_t *all_points;
      ਜੇ (!plan->new_gid_contour_points_map.has (new_gid, &all_points))
        ਵਾਪਸ ਝੂਠ;
      /* IUP adds rebased constant deltas to the contour coordinates. Keep
       * the plan's original points intact across serialization retries. */
      contour_ਬਿੰਦੂ_ਸਦਿਸ਼_t points;
      ਜੇ (iup_optimize)
      {
        points = *all_points;
        ਜੇ (unlikely (points.ਵਿੱਚ_error ())) ਵਾਪਸ ਝੂਠ;
        all_points = &points;
      }
      /* avar2 partial instancing: cull unreachable tuples. */
      ਜੇ (plan->has_avar2 && plan->avar2_reachable_ranges.get_population ())
	glyph_variations[i].cull_unreachable (plan->avar2_reachable_ranges);
      ਜੇ (!glyph_variations[i].instantiate (plan->axes_location, plan->axes_triple_distances, scratch, &pool, all_points, iup_optimize))
        ਵਾਪਸ ਝੂਠ;
    }
    ਵਾਪਸ ਸੱਚ;
  }

  ਤਾਰਕਿਕ compile_ਬਾਈਟਾਂ (ਅਚਲ hb_map_t& axes_ਸੂਚਕਾਂਕ_map,
                      ਅਚਲ hb_map_t& axes_old_ਸੂਚਕਾਂਕ_ਟੈਗ_map)
  {
    ਜੇ (!compile_shared_tuples (axes_ਸੂਚਕਾਂਕ_map, axes_old_ਸੂਚਕਾਂਕ_ਟੈਗ_map))
      ਵਾਪਸ ਝੂਠ;
    ਲਈ (tuple_variations_t& vars: glyph_variations)
      ਜੇ (!vars.compile_ਬਾਈਟਾਂ (axes_ਸੂਚਕਾਂਕ_map, axes_old_ਸੂਚਕਾਂਕ_ਟੈਗ_map,
                               ਸੱਚ, /* use shared points*/
                               ਸੱਚ,
                               &shared_tuples_idx_map,
			       &pool))
        ਵਾਪਸ ਝੂਠ;

    ਵਾਪਸ ਸੱਚ;
  }

  ਤਾਰਕਿਕ compile_shared_tuples (ਅਚਲ hb_map_t& axes_ਸੂਚਕਾਂਕ_map,
                              ਅਚਲ hb_map_t& axes_old_ਸੂਚਕਾਂਕ_ਟੈਗ_map)
  {
    /* key is pointer to compiled_peak_coords inside each tuple, hashing
     * function will always deref pointers first */
    hb_hashmap_t<ਅਚਲ hb_ਸਦਿਸ਼_t<F2DOT14>*, ਬਿਨਾਨਿਸ਼ਾਨੀ> coords_ਗਿਣਤੀ_map;

    /* count the num of shared coords */
    ਲਈ (tuple_variations_t& vars: glyph_variations)
    {
      ਲਈ (tuple_delta_t& var : vars.tuple_vars)
      {
        ਜੇ (!var.compile_coords (axes_ਸੂਚਕਾਂਕ_map, axes_old_ਸੂਚਕਾਂਕ_ਟੈਗ_map, &pool))
          ਵਾਪਸ ਝੂਠ;
        ਬਿਨਾਨਿਸ਼ਾਨੀ *ਗਿਣਤੀ_2;
	ਬਿਨਾਨਿਸ਼ਾਨੀ hash = hb_hash (&var.compiled_peak_coords);
        ਜੇ (coords_ਗਿਣਤੀ_map.has_with_hash (&(var.compiled_peak_coords), hash, &ਗਿਣਤੀ_2))
	  (*ਗਿਣਤੀ_2)++;
        ਨਹੀਂਤਾਂ
          coords_ਗਿਣਤੀ_map.ਸੈੱਟ_with_hash (&(var.compiled_peak_coords), hash, 1);
      }
    }

    ਜੇ (!coords_ਗਿਣਤੀ_map || coords_ਗਿਣਤੀ_map.ਵਿੱਚ_error ())
      ਵਾਪਸ ਝੂਠ;

    /* add only those coords that are used more than once into the vector and sort */
    hb_ਸਦਿਸ਼_t<hb_pair_t<ਅਚਲ hb_ਸਦਿਸ਼_t<F2DOT14>*, ਬਿਨਾਨਿਸ਼ਾਨੀ>> shared_coords {
      + hb_iter (coords_ਗਿਣਤੀ_map)
      | hb_filter ([] (ਅਚਲ hb_pair_t<ਅਚਲ hb_ਸਦਿਸ਼_t<F2DOT14>*, ਬਿਨਾਨਿਸ਼ਾਨੀ>& p) { ਵਾਪਸ p.second > 1; })
    };
    ਜੇ (unlikely (shared_coords.ਵਿੱਚ_error ())) ਵਾਪਸ ਝੂਠ;

    /* no shared tuples: no coords are used more than once */
    ਜੇ (!shared_coords) ਵਾਪਸ ਸੱਚ;
    /* sorting based on the coords frequency first (high to low), then compare
     * the coords bytes */
    shared_coords.qsort (_cmp_coords);

    /* build shared_coords->idx map and shared tuples byte array */

    shared_tuples_ਗਿਣਤੀ = hb_min (0xFFFu + 1, shared_coords.ਲੰਮਾਈ);
    ਬਿਨਾਨਿਸ਼ਾਨੀ len = shared_tuples_ਗਿਣਤੀ * (shared_coords[0].ਪਹਿਲਾ->ਲੰਮਾਈ);
    ਜੇ (unlikely (!compiled_shared_tuples.alloc (len)))
      ਵਾਪਸ ਝੂਠ;

    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < shared_tuples_ਗਿਣਤੀ; i++)
    {
      shared_tuples_idx_map.ਸੈੱਟ (shared_coords[i].ਪਹਿਲਾ, i);
      /* add a concat() in hb_vector_t? */
      ਲਈ (ਸਵੈਚਾਲਿਤ c : shared_coords[i].ਪਹਿਲਾ->iter ())
        compiled_shared_tuples.push (c);
    }

    ਵਾਪਸ ਸੱਚ;
  }

  ਸਥਿਰ ਪੂਰਨ _cmp_coords (ਅਚਲ ਖਾਲੀ *pa, ਅਚਲ ਖਾਲੀ *pb)
  {
    ਅਚਲ hb_pair_t<hb_ਸਦਿਸ਼_t<F2DOT14> *, ਬਿਨਾਨਿਸ਼ਾਨੀ> *a = (ਅਚਲ hb_pair_t<hb_ਸਦਿਸ਼_t<F2DOT14> *, ਬਿਨਾਨਿਸ਼ਾਨੀ> *) pa;
    ਅਚਲ hb_pair_t<hb_ਸਦਿਸ਼_t<F2DOT14> *, ਬਿਨਾਨਿਸ਼ਾਨੀ> *b = (ਅਚਲ hb_pair_t<hb_ਸਦਿਸ਼_t<F2DOT14> *, ਬਿਨਾਨਿਸ਼ਾਨੀ> *) pb;

    ਜੇ (a->second != b->second)
      ਵਾਪਸ b->second - a->second; // high to low

    ਵਾਪਸ b->ਪਹਿਲਾ->as_array().cmp (a->ਪਹਿਲਾ->as_array ());
  }

  ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ Iterator,
           hb_requires (hb_is_iterator (Iterator))>
  ਤਾਰਕਿਕ serialize_glyph_var_ਅੰਕੜੇ (hb_serialize_context_t *c,
                                 Iterator it,
                                 ਤਾਰਕਿਕ long_ਵਿੱਥ,
                                 ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_glyphs,
                                 ਅੱਖਰ* glyph_var_ਅੰਕੜੇ_offsets /* OUT: glyph var data offsets array */) ਅਚਲ
  {
    TRACE_SERIALIZE (ਇਹ);

    ਜੇ (long_ਵਿੱਥ)
    {
      ((HBUINT32 *) glyph_var_ਅੰਕੜੇ_offsets)[0] = 0;
      glyph_var_ਅੰਕੜੇ_offsets += 4;
    }
    ਨਹੀਂਤਾਂ
    {
      ((HBUINT16 *) glyph_var_ਅੰਕੜੇ_offsets)[0] = 0;
      glyph_var_ਅੰਕੜੇ_offsets += 2;
    }
    ਬਿਨਾਨਿਸ਼ਾਨੀ glyph_ਵਿੱਥ = 0;
    hb_codepoint_t ਆਖਰੀ_gid = 0;
    ਬਿਨਾਨਿਸ਼ਾਨੀ idx = 0;

    Glyph_Variation_ਅੰਕੜੇ* cur_glyph = c->ਸ਼ੁਰੂ_embed<Glyph_Variation_ਅੰਕੜੇ> ();
    ਜੇ (!cur_glyph) return_trace (ਝੂਠ);
    ਲਈ (ਸਵੈਚਾਲਿਤ &_ : it)
    {
      hb_codepoint_t gid = _.ਪਹਿਲਾ;
      ਜੇ (long_ਵਿੱਥ)
        ਲਈ (; ਆਖਰੀ_gid < gid; ਆਖਰੀ_gid++)
          ((HBUINT32 *) glyph_var_ਅੰਕੜੇ_offsets)[ਆਖਰੀ_gid] = glyph_ਵਿੱਥ;
      ਨਹੀਂਤਾਂ
        ਲਈ (; ਆਖਰੀ_gid < gid; ਆਖਰੀ_gid++)
          ((HBUINT16 *) glyph_var_ਅੰਕੜੇ_offsets)[ਆਖਰੀ_gid] = glyph_ਵਿੱਥ / 2;

      ਜੇ (idx >= glyph_variations.ਲੰਮਾਈ) return_trace (ਝੂਠ);
      ਜੇ (!cur_glyph->serialize (c, ਸੱਚ, glyph_variations[idx])) return_trace (ਝੂਠ);
      Glyph_Variation_ਅੰਕੜੇ* ਅਗਲਾ_glyph = c->ਸ਼ੁਰੂ_embed<Glyph_Variation_ਅੰਕੜੇ> ();
      glyph_ਵਿੱਥ += (ਅੱਖਰ *) ਅਗਲਾ_glyph - (ਅੱਖਰ *) cur_glyph;

      ਜੇ (long_ਵਿੱਥ)
        ((HBUINT32 *) glyph_var_ਅੰਕੜੇ_offsets)[gid] = glyph_ਵਿੱਥ;
      ਨਹੀਂਤਾਂ
        ((HBUINT16 *) glyph_var_ਅੰਕੜੇ_offsets)[gid] = glyph_ਵਿੱਥ / 2;

      ਆਖਰੀ_gid++;
      idx++;
      cur_glyph = ਅਗਲਾ_glyph;
    }

    ਜੇ (long_ਵਿੱਥ)
      ਲਈ (; ਆਖਰੀ_gid < ਅੰਕ_glyphs; ਆਖਰੀ_gid++)
        ((HBUINT32 *) glyph_var_ਅੰਕੜੇ_offsets)[ਆਖਰੀ_gid] = glyph_ਵਿੱਥ;
    ਨਹੀਂਤਾਂ
      ਲਈ (; ਆਖਰੀ_gid < ਅੰਕ_glyphs; ਆਖਰੀ_gid++)
        ((HBUINT16 *) glyph_var_ਅੰਕੜੇ_offsets)[ਆਖਰੀ_gid] = glyph_ਵਿੱਥ / 2;
    return_trace (ਸੱਚ);
  }
};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ Types>
ਬਣਤਰ gvar_GVAR
{
  ਵਰਤੋਂ Glyph_ਗਿਣਤੀ_Type = ਕਿਸਮ_ਨਾਂ Types::HBUINT;

  ਵਰਤੋਂ Glyph_Variation_ਅੰਕੜੇ = Tuple_Variation_ਅੰਕੜੇ<Glyph_ਗਿਣਤੀ_Type>;
  ਸਥਿਰ constexpr ਬਿਨਾਨਿਸ਼ਾਨੀ glyph_ਗਿਣਤੀ_max ()
  { ਵਾਪਸ (1u << (8 * Glyph_ਗਿਣਤੀ_Type::static_ਮਾਪ)) - 1; }

  ਤਾਰਕਿਕ has_ਅੰਕੜੇ () ਅਚਲ { ਵਾਪਸ ਸੰਸਕਰਣ.ਮੰਜ਼ਿਲ_int () != 0; }

  ਤਾਰਕਿਕ sanitize_shallow (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ) &&
		  hb_barrier () &&
		  (ਸੰਸਕਰਣ.major == 1) &&
		  sharedTuples.sanitize (c, ਇਹ, axis_ਗਿਣਤੀ_2 * shared_Tuple_ਗਿਣਤੀ) &&
		  (is_long_ਵਿੱਥ () ?
		     c->check_array (get_long_ਵਿੱਥ_array (), c->get_ਅੰਕ_glyphs () + 1) :
		     c->check_array (get_short_ਵਿੱਥ_array (), c->get_ਅੰਕ_glyphs () + 1)));
  }

  /* GlyphVariationData not sanitized here; must be checked while accessing each glyph variation data */
  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  { ਵਾਪਸ sanitize_shallow (c); }

  ਤਾਰਕਿਕ decompile_glyph_variations (hb_subset_context_t *c,
                                   glyph_variations_t<Glyph_ਗਿਣਤੀ_Type>& glyph_vars /*ਬਾਹਰਲਾ*/) ਅਚਲ
  {
    hb_hashmap_t<hb_codepoint_t, hb_ਬਾਈਟਾਂ_t> new_gid_var_ਅੰਕੜੇ_map;
    ਸਵੈਚਾਲਿਤ it = hb_iter (c->plan->new_ਮੰਜ਼ਿਲ_old_gid_list);
    ਜੇ (it->ਪਹਿਲਾ == 0 && !(c->plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE))
    {
      new_gid_var_ਅੰਕੜੇ_map.ਸੈੱਟ (0, hb_ਬਾਈਟਾਂ_t ());
      it++;
    }

    ਲਈ (ਸਵੈਚਾਲਿਤ &_ : it)
    {
      hb_codepoint_t new_gid = _.ਪਹਿਲਾ;
      hb_codepoint_t old_gid = _.second;
      hb_ਬਾਈਟਾਂ_t var_ਅੰਕੜੇ_ਬਾਈਟਾਂ = get_glyph_var_ਅੰਕੜੇ_ਬਾਈਟਾਂ (c->ਸਰੋਤ_blob, glyph_ਗਿਣਤੀ_X, old_gid);
      new_gid_var_ਅੰਕੜੇ_map.ਸੈੱਟ (new_gid, var_ਅੰਕੜੇ_ਬਾਈਟਾਂ);
    }

    ਜੇ (new_gid_var_ਅੰਕੜੇ_map.ਵਿੱਚ_error ()) ਵਾਪਸ ਝੂਠ;

    hb_array_t<ਅਚਲ F2DOT14> shared_tuples = (ਇਹ+sharedTuples).as_array ((ਬਿਨਾਨਿਸ਼ਾਨੀ) shared_Tuple_ਗਿਣਤੀ * (ਬਿਨਾਨਿਸ਼ਾਨੀ) axis_ਗਿਣਤੀ_2);
    ਵਾਪਸ glyph_vars.create_from_glyphs_var_ਅੰਕੜੇ (axis_ਗਿਣਤੀ_2, shared_tuples, c->plan, new_gid_var_ਅੰਕੜੇ_map);
  }

  ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ Iterator,
           hb_requires (hb_is_iterator (Iterator))>
  ਤਾਰਕਿਕ serialize (hb_serialize_context_t *c,
                  ਅਚਲ glyph_variations_t<Glyph_ਗਿਣਤੀ_Type>& glyph_vars,
                  Iterator it,
                  ਬਿਨਾਨਿਸ਼ਾਨੀ axis_ਗਿਣਤੀ,
                  ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_glyphs,
                  ਤਾਰਕਿਕ force_long_offsets) ਅਚਲ
  {
    TRACE_SERIALIZE (ਇਹ);
    gvar_GVAR *ਬਾਹਰਲਾ = c->allocate_min<gvar_GVAR> ();
    ਜੇ (unlikely (!ਬਾਹਰਲਾ)) return_trace (ਝੂਠ);

    ਬਾਹਰਲਾ->ਸੰਸਕਰਣ.major = 1;
    ਬਾਹਰਲਾ->ਸੰਸਕਰਣ.minor = 0;
    ਬਾਹਰਲਾ->axis_ਗਿਣਤੀ_2 = axis_ਗਿਣਤੀ;
    ਬਾਹਰਲਾ->glyph_ਗਿਣਤੀ_X = hb_min (glyph_ਗਿਣਤੀ_max (), ਅੰਕ_glyphs);

    ਬਿਨਾਨਿਸ਼ਾਨੀ glyph_var_ਅੰਕੜੇ_ਮਾਪ = glyph_vars.compiled_byte_ਮਾਪ ();
    /* According to the spec: If the short format (Offset16) is used for offsets,
     * the value stored is the offset divided by 2, so the maximum data size should
     * be 2 * 0xFFFFu, which is 0x1FFFEu */
    ਤਾਰਕਿਕ long_ਵਿੱਥ = glyph_var_ਅੰਕੜੇ_ਮਾਪ > 0x1FFFEu || force_long_offsets;
    ਬਾਹਰਲਾ->flags = long_ਵਿੱਥ ? 1 : 0;

    ਬਿਨਾਨਿਸ਼ਾਨੀ glyph_var_ਅੰਕੜੇ_offsets_ਮਾਪ = (long_ਵਿੱਥ ? 4 : 2) * (ਅੰਕ_glyphs + 1);
    HBUINT8 *glyph_var_ਅੰਕੜੇ_offsets = c->allocate_ਮਾਪ<HBUINT8> (glyph_var_ਅੰਕੜੇ_offsets_ਮਾਪ, ਝੂਠ);
    ਜੇ (!glyph_var_ਅੰਕੜੇ_offsets) return_trace (ਝੂਠ);

    /* shared tuples */
    ਬਿਨਾਨਿਸ਼ਾਨੀ shared_tuple_ਗਿਣਤੀ = glyph_vars.compiled_shared_tuples_ਗਿਣਤੀ ();
    ਬਾਹਰਲਾ->shared_Tuple_ਗਿਣਤੀ = shared_tuple_ਗਿਣਤੀ;

    ਜੇ (!shared_tuple_ਗਿਣਤੀ)
      ਬਾਹਰਲਾ->sharedTuples = (ਅੱਖਰ *) glyph_var_ਅੰਕੜੇ_offsets + glyph_var_ਅੰਕੜੇ_offsets_ਮਾਪ - (ਅੱਖਰ *) ਬਾਹਰਲਾ;
    ਨਹੀਂਤਾਂ
    {
      hb_array_t<ਅਚਲ F2DOT14> shared_tuples = glyph_vars.compiled_shared_tuples.as_array ().copy (c);
      ਜੇ (!shared_tuples.arrayZ) return_trace (ਝੂਠ);
      ਬਾਹਰਲਾ->sharedTuples = (ਅਚਲ ਅੱਖਰ *) shared_tuples.arrayZ - (ਅੱਖਰ *) ਬਾਹਰਲਾ;
    }

    ਅੱਖਰ *glyph_var_ਅੰਕੜੇ = c->ਸ਼ੁਰੂ_embed<ਅੱਖਰ> ();
    ਜੇ (!glyph_var_ਅੰਕੜੇ) return_trace (ਝੂਠ);
    ਬਾਹਰਲਾ->ਅੰਕੜੇ_Z = glyph_var_ਅੰਕੜੇ - (ਅੱਖਰ *) ਬਾਹਰਲਾ;

    return_trace (glyph_vars.serialize_glyph_var_ਅੰਕੜੇ (c, it, long_ਵਿੱਥ, ਅੰਕ_glyphs,
                                                       (ਅੱਖਰ *) glyph_var_ਅੰਕੜੇ_offsets));
  }

  ਤਾਰਕਿਕ instantiate (hb_subset_context_t *c) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    glyph_variations_t<Glyph_ਗਿਣਤੀ_Type> glyph_vars;
    ਜੇ (!decompile_glyph_variations (c, glyph_vars))
      return_trace (ਝੂਠ);

    ਜੇ (!glyph_vars.instantiate (c->plan)) return_trace (ਝੂਠ);
    ਜੇ (!glyph_vars.compile_ਬਾਈਟਾਂ (c->plan->axes_ਸੂਚਕਾਂਕ_map, c->plan->axes_old_ਸੂਚਕਾਂਕ_ਟੈਗ_map))
      return_trace (ਝੂਠ);

    ਬਿਨਾਨਿਸ਼ਾਨੀ axis_ਗਿਣਤੀ = c->plan->axes_ਸੂਚਕਾਂਕ_map.get_population ();
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_glyphs = c->plan->ਅੰਕ_output_glyphs ();
    ਸਵੈਚਾਲਿਤ it = hb_iter (c->plan->new_ਮੰਜ਼ਿਲ_old_gid_list);

    ਤਾਰਕਿਕ force_long_offsets = ਝੂਠ;
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HB_EXPERIMENTAL_API
    force_long_offsets = c->plan->flags & HB_SUBSET_FLAGS_IFTB_REQUIREMENTS;
#ਜੇ_ਅੰਤ
    return_trace (serialize (c->serializer, glyph_vars, it, axis_ਗਿਣਤੀ, ਅੰਕ_glyphs, force_long_offsets));
  }

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਜੇ (c->plan->all_axes_pinned)
      return_trace (ਝੂਠ);

    ਜੇ (c->plan->normalized_coords)
      return_trace (instantiate (c));

    ਬਿਨਾਨਿਸ਼ਾਨੀ glyph_ਗਿਣਤੀ = ਸੰਸਕਰਣ.ਮੰਜ਼ਿਲ_int () ? c->plan->ਸਰੋਤ->get_ਅੰਕ_glyphs () : 0;

    gvar_GVAR *ਬਾਹਰਲਾ = c->serializer->allocate_min<gvar_GVAR> ();
    ਜੇ (unlikely (!ਬਾਹਰਲਾ)) return_trace (ਝੂਠ);

    ਬਾਹਰਲਾ->ਸੰਸਕਰਣ.major = 1;
    ਬਾਹਰਲਾ->ਸੰਸਕਰਣ.minor = 0;
    ਬਾਹਰਲਾ->axis_ਗਿਣਤੀ_2 = axis_ਗਿਣਤੀ_2;
    ਬਾਹਰਲਾ->shared_Tuple_ਗਿਣਤੀ = shared_Tuple_ਗਿਣਤੀ;

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ_glyphs = c->plan->ਅੰਕ_output_glyphs ();
    ਬਾਹਰਲਾ->glyph_ਗਿਣਤੀ_X = hb_min (glyph_ਗਿਣਤੀ_max (), ਅੰਕ_glyphs);

    /* avar2 partial instancing: cull tuple variations whose region falls
     * outside the reachable old-space final-coord ranges. Rewritten glyph
     * data is cached per new gid; untouched glyphs are copied verbatim. */
    hb_ਸਦਿਸ਼_t<hb_ਸਦਿਸ਼_t<ਅੱਖਰ>> culled_vars;
    hb_ਸੈੱਟ_t culled_gids;
    ਜੇ (c->plan->has_avar2 && c->plan->avar2_reachable_ranges.get_population ())
    {
      hb_array_t<ਅਚਲ F2DOT14> shared_tuples_array =
	  (ਇਹ+sharedTuples).as_array ((ਬਿਨਾਨਿਸ਼ਾਨੀ) shared_Tuple_ਗਿਣਤੀ * (ਬਿਨਾਨਿਸ਼ਾਨੀ) axis_ਗਿਣਤੀ_2);
      ਜੇ (unlikely (!culled_vars.resize (ਅੰਕ_glyphs))) return_trace (ਝੂਠ);
      ਸਵੈਚਾਲਿਤ cull_it = hb_iter (c->plan->new_ਮੰਜ਼ਿਲ_old_gid_list);
      ਜੇ (cull_it->ਪਹਿਲਾ == 0 && !(c->plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE))
	cull_it++;
      ਲਈ (ਸਵੈਚਾਲਿਤ &_ : cull_it)
      {
	hb_ਬਾਈਟਾਂ_t var_ਅੰਕੜੇ_ਬਾਈਟਾਂ = get_glyph_var_ਅੰਕੜੇ_ਬਾਈਟਾਂ (c->ਸਰੋਤ_blob, glyph_ਗਿਣਤੀ, _.second);
	ਜੇ (var_ਅੰਕੜੇ_ਬਾਈਟਾਂ.ਲੰਮਾਈ < Glyph_Variation_ਅੰਕੜੇ::min_ਮਾਪ) ਜਾਰੀ;
	ਤਾਰਕਿਕ ਬਦਲਿਆ = ਝੂਠ;
	ਜੇ (unlikely (!var_ਅੰਕੜੇ_ਬਾਈਟਾਂ.as<Glyph_Variation_ਅੰਕੜੇ> ()
		      ->cull_tuple_variations (var_ਅੰਕੜੇ_ਬਾਈਟਾਂ, axis_ਗਿਣਤੀ_2,
					       shared_tuples_array,
					       &c->plan->axes_old_ਸੂਚਕਾਂਕ_ਟੈਗ_map,
					       c->plan->avar2_reachable_ranges,
					       culled_vars[_.ਪਹਿਲਾ], &ਬਦਲਿਆ)))
	  return_trace (ਝੂਠ);
	ਜੇ (ਬਦਲਿਆ) culled_gids.ਜੋੜੋ (_.ਪਹਿਲਾ);
      }
      ਜੇ (unlikely (culled_gids.ਵਿੱਚ_error ())) return_trace (ਝੂਠ);
    }

    ਸਵੈਚਾਲਿਤ it = hb_iter (c->plan->new_ਮੰਜ਼ਿਲ_old_gid_list);
    ਜੇ (it->ਪਹਿਲਾ == 0 && !(c->plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE))
      it++;
    ਬਿਨਾਨਿਸ਼ਾਨੀ subset_ਅੰਕੜੇ_ਮਾਪ = 0;
    ਬਿਨਾਨਿਸ਼ਾਨੀ padding_ਮਾਪ = 0;
    ਲਈ (ਸਵੈਚਾਲਿਤ &_ : it)
    {
      hb_codepoint_t old_gid = _.second;
      ਬਿਨਾਨਿਸ਼ਾਨੀ glyph_ਅੰਕੜੇ_ਮਾਪ = culled_gids.has (_.ਪਹਿਲਾ)
				 ? culled_vars[_.ਪਹਿਲਾ].ਲੰਮਾਈ
				 : get_glyph_var_ਅੰਕੜੇ_ਬਾਈਟਾਂ (c->ਸਰੋਤ_blob, glyph_ਗਿਣਤੀ, old_gid).ਲੰਮਾਈ;
      ਜੇ (glyph_ਅੰਕੜੇ_ਮਾਪ % 2)
      {
        glyph_ਅੰਕੜੇ_ਮਾਪ++;
        padding_ਮਾਪ++;
      }

      ਜੇ (unlikely (hb_unsigned_ਜੋੜੋ_overflows (subset_ਅੰਕੜੇ_ਮਾਪ, glyph_ਅੰਕੜੇ_ਮਾਪ, &subset_ਅੰਕੜੇ_ਮਾਪ)))
	return_trace (ਝੂਠ);
    }

    /* According to the spec: If the short format (Offset16) is used for offsets,
     * the value stored is the offset divided by 2, so the maximum data size should
     * be 2 * 0xFFFFu, which is 0x1FFFEu */
    ਤਾਰਕਿਕ long_ਵਿੱਥ = subset_ਅੰਕੜੇ_ਮਾਪ > 0x1FFFEu;
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HB_EXPERIMENTAL_API
    long_ਵਿੱਥ = long_ਵਿੱਥ || (c->plan->flags & HB_SUBSET_FLAGS_IFTB_REQUIREMENTS);
#ਜੇ_ਅੰਤ
    ਬਾਹਰਲਾ->flags = long_ਵਿੱਥ ? 1 : 0;

    ਬਿਨਾਨਿਸ਼ਾਨੀ subset_offsets_ਮਾਪ = (long_ਵਿੱਥ ? 4 : 2) * (ਅੰਕ_glyphs + 1);
    HBUINT8 *subset_offsets = c->serializer->allocate_ਮਾਪ<HBUINT8> (subset_offsets_ਮਾਪ, ਝੂਠ);
    ਜੇ (!subset_offsets) return_trace (ਝੂਠ);

    /* shared tuples */
    ਜੇ (!shared_Tuple_ਗਿਣਤੀ || !sharedTuples)
      ਬਾਹਰਲਾ->sharedTuples = (ਅੱਖਰ *) subset_offsets + subset_offsets_ਮਾਪ - (ਅੱਖਰ *) ਬਾਹਰਲਾ;
    ਨਹੀਂਤਾਂ
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ shared_tuple_ਮਾਪ = F2DOT14::static_ਮਾਪ * axis_ਗਿਣਤੀ_2 * shared_Tuple_ਗਿਣਤੀ;
      F2DOT14 *tuples = c->serializer->allocate_ਮਾਪ<F2DOT14> (shared_tuple_ਮਾਪ);
      ਜੇ (!tuples) return_trace (ਝੂਠ);
      ਬਾਹਰਲਾ->sharedTuples = (ਅੱਖਰ *) tuples - (ਅੱਖਰ *) ਬਾਹਰਲਾ;
      hb_memcpy (tuples, ਇਹ+sharedTuples, shared_tuple_ਮਾਪ);
    }

    /* This ordering relative to the shared tuples array, which puts the glyphVariationData
       last in the table, is required when HB_SUBSET_FLAGS_IFTB_REQUIREMENTS is set */
    ਜੇ (long_ਵਿੱਥ)
      subset_ਅੰਕੜੇ_ਮਾਪ -= padding_ਮਾਪ;
    ਅੱਖਰ *subset_ਅੰਕੜੇ = c->serializer->allocate_ਮਾਪ<ਅੱਖਰ> (subset_ਅੰਕੜੇ_ਮਾਪ, ਝੂਠ);
    ਜੇ (!subset_ਅੰਕੜੇ) return_trace (ਝੂਠ);
    ਬਾਹਰਲਾ->ਅੰਕੜੇ_Z = subset_ਅੰਕੜੇ - (ਅੱਖਰ *) ਬਾਹਰਲਾ;


    ਜੇ (long_ਵਿੱਥ)
    {
      ((HBUINT32 *) subset_offsets)[0] = 0;
      subset_offsets += 4;
    }
    ਨਹੀਂਤਾਂ
    {
      ((HBUINT16 *) subset_offsets)[0] = 0;
      subset_offsets += 2;
    }
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ glyph_ਵਿੱਥ = 0;

    hb_codepoint_t ਆਖਰੀ = 0;
    it = hb_iter (c->plan->new_ਮੰਜ਼ਿਲ_old_gid_list);
    ਜੇ (it->ਪਹਿਲਾ == 0 && !(c->plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE))
      it++;
    ਲਈ (ਸਵੈਚਾਲਿਤ &_ : it)
    {
      hb_codepoint_t gid = _.ਪਹਿਲਾ;
      hb_codepoint_t old_gid = _.second;

      ਜੇ (long_ਵਿੱਥ)
	ਲਈ (; ਆਖਰੀ < gid; ਆਖਰੀ++)
	  ((HBUINT32 *) subset_offsets)[ਆਖਰੀ] = glyph_ਵਿੱਥ;
      ਨਹੀਂਤਾਂ
	ਲਈ (; ਆਖਰੀ < gid; ਆਖਰੀ++)
	  ((HBUINT16 *) subset_offsets)[ਆਖਰੀ] = glyph_ਵਿੱਥ / 2;

      hb_ਬਾਈਟਾਂ_t var_ਅੰਕੜੇ_ਬਾਈਟਾਂ = culled_gids.has (gid)
				  ? hb_ਬਾਈਟਾਂ_t (culled_vars[gid].arrayZ,
						culled_vars[gid].ਲੰਮਾਈ)
				  : get_glyph_var_ਅੰਕੜੇ_ਬਾਈਟਾਂ (c->ਸਰੋਤ_blob,
							      glyph_ਗਿਣਤੀ,
							      old_gid);

      hb_memcpy (subset_ਅੰਕੜੇ, var_ਅੰਕੜੇ_ਬਾਈਟਾਂ.arrayZ, var_ਅੰਕੜੇ_ਬਾਈਟਾਂ.ਲੰਮਾਈ);
      ਬਿਨਾਨਿਸ਼ਾਨੀ glyph_ਅੰਕੜੇ_ਮਾਪ = var_ਅੰਕੜੇ_ਬਾਈਟਾਂ.ਲੰਮਾਈ;
      subset_ਅੰਕੜੇ += glyph_ਅੰਕੜੇ_ਮਾਪ;
      glyph_ਵਿੱਥ += glyph_ਅੰਕੜੇ_ਮਾਪ;

      ਜੇ (!long_ਵਿੱਥ && (glyph_ਅੰਕੜੇ_ਮਾਪ % 2))
      {
        *subset_ਅੰਕੜੇ = 0;
        subset_ਅੰਕੜੇ++;
        glyph_ਵਿੱਥ++;
      }

      ਜੇ (long_ਵਿੱਥ)
	((HBUINT32 *) subset_offsets)[gid] = glyph_ਵਿੱਥ;
      ਨਹੀਂਤਾਂ
	((HBUINT16 *) subset_offsets)[gid] = glyph_ਵਿੱਥ / 2;

      ਆਖਰੀ++; // Skip over gid
    }

    ਜੇ (long_ਵਿੱਥ)
      ਲਈ (; ਆਖਰੀ < ਅੰਕ_glyphs; ਆਖਰੀ++)
	((HBUINT32 *) subset_offsets)[ਆਖਰੀ] = glyph_ਵਿੱਥ;
    ਨਹੀਂਤਾਂ
      ਲਈ (; ਆਖਰੀ < ਅੰਕ_glyphs; ਆਖਰੀ++)
	((HBUINT16 *) subset_offsets)[ਆਖਰੀ] = glyph_ਵਿੱਥ / 2;

    return_trace (ਸੱਚ);
  }

  ਸੁਰੱਖਿਅਤ:
  ਅਚਲ hb_ਬਾਈਟਾਂ_t get_glyph_var_ਅੰਕੜੇ_ਬਾਈਟਾਂ (hb_blob_t *blob,
					     ਬਿਨਾਨਿਸ਼ਾਨੀ glyph_ਗਿਣਤੀ,
					     hb_codepoint_t glyph) ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਸ਼ੁਰੂ_ਵਿੱਥ = get_ਵਿੱਥ (glyph_ਗਿਣਤੀ, glyph);
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਸਿਰਾ_ਵਿੱਥ = get_ਵਿੱਥ (glyph_ਗਿਣਤੀ, glyph+1);
    ਜੇ (unlikely (ਸਿਰਾ_ਵਿੱਥ < ਸ਼ੁਰੂ_ਵਿੱਥ)) ਵਾਪਸ hb_ਬਾਈਟਾਂ_t ();
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾਈ = ਸਿਰਾ_ਵਿੱਥ - ਸ਼ੁਰੂ_ਵਿੱਥ;
    hb_ਬਾਈਟਾਂ_t var_ਅੰਕੜੇ = blob->as_ਬਾਈਟਾਂ ().sub_array (((ਬਿਨਾਨਿਸ਼ਾਨੀ) ਅੰਕੜੇ_Z) + ਸ਼ੁਰੂ_ਵਿੱਥ, ਲੰਮਾਈ);
    ਵਾਪਸ likely (var_ਅੰਕੜੇ.ਲੰਮਾਈ >= Glyph_Variation_ਅੰਕੜੇ::min_ਮਾਪ) ? var_ਅੰਕੜੇ : hb_ਬਾਈਟਾਂ_t ();
  }

  ਤਾਰਕਿਕ is_long_ਵਿੱਥ () ਅਚਲ { ਵਾਪਸ flags & 1; }

  ਬਿਨਾਨਿਸ਼ਾਨੀ get_ਵਿੱਥ (ਬਿਨਾਨਿਸ਼ਾਨੀ glyph_ਗਿਣਤੀ, ਬਿਨਾਨਿਸ਼ਾਨੀ i) ਅਚਲ
  {
    ਜੇ (unlikely (i > glyph_ਗਿਣਤੀ)) ਵਾਪਸ 0;
    hb_barrier ();
    ਵਾਪਸ is_long_ਵਿੱਥ () ? get_long_ਵਿੱਥ_array ()[i] : get_short_ਵਿੱਥ_array ()[i] * 2;
  }

  ਅਚਲ HBUINT32 * get_long_ਵਿੱਥ_array () ਅਚਲ { ਵਾਪਸ (ਅਚਲ HBUINT32 *) &ਵਿੱਥ_Z; }
  ਅਚਲ HBUINT16 *get_short_ਵਿੱਥ_array () ਅਚਲ { ਵਾਪਸ (ਅਚਲ HBUINT16 *) &ਵਿੱਥ_Z; }

  ਜਨਤਕ:
  ਬਣਤਰ accelerator_t
  {

    hb_scalar_cache_t *create_cache () ਅਚਲ
    {
      ਵਾਪਸ hb_scalar_cache_t::create (hb_min ((ਬਿਨਾਨਿਸ਼ਾਨੀ) ਸਾਰਣੀ->shared_Tuple_ਗਿਣਤੀ,
						+Tuple_Variation_ਸਿਰਲੇਖ::max_shared_tuple_ਗਿਣਤੀ));
    }

    ਸਥਿਰ ਖਾਲੀ destroy_cache (hb_scalar_cache_t *cache)
    {
      hb_scalar_cache_t::destroy (cache);
    }

    ਤਾਰਕਿਕ has_ਅੰਕੜੇ () ਅਚਲ { ਵਾਪਸ ਸਾਰਣੀ->has_ਅੰਕੜੇ (); }

    accelerator_t (hb_face_t *face)
    {
      ਸਾਰਣੀ = hb_sanitize_context_t ().reference_ਸਾਰਣੀ<gvar_GVAR> (face, T::ਸਾਰਣੀ_ਟੈਗ);
      /* If sanitize failed, set glyphCount to 0. */
      glyph_ਗਿਣਤੀ_2 = ਸਾਰਣੀ->ਸੰਸਕਰਣ.ਮੰਜ਼ਿਲ_int () ? face->get_ਅੰਕ_glyphs () : 0;
    }
    ~accelerator_t () { ਸਾਰਣੀ.destroy (); }

    ਨਿਜੀ:

    ਸਥਿਰ ਦਸ਼ਮਲਵ infer_delta (ਅਚਲ hb_array_t<contour_ਬਿੰਦੂ_t> points,
			      ਅਚਲ hb_array_t<contour_ਬਿੰਦੂ_t> deltas,
			      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ target, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ prev, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ next,
			      ਦਸ਼ਮਲਵ contour_ਬਿੰਦੂ_t::*m)
    {
      ਦਸ਼ਮਲਵ ਨਿਸ਼ਾਨਾ_val = points.arrayZ[target].*m;
      ਦਸ਼ਮਲਵ prev_val = points.arrayZ[prev].*m;
      ਦਸ਼ਮਲਵ ਅਗਲਾ_val = points.arrayZ[next].*m;
      ਦਸ਼ਮਲਵ prev_delta =  deltas.arrayZ[prev].*m;
      ਦਸ਼ਮਲਵ ਅਗਲਾ_delta =  deltas.arrayZ[next].*m;

      ਜੇ (prev_val == ਅਗਲਾ_val)
	ਵਾਪਸ (prev_delta == ਅਗਲਾ_delta) ? prev_delta : 0.f;
      ਨਹੀਂਤਾਂ ਜੇ (ਨਿਸ਼ਾਨਾ_val <= hb_min (prev_val, ਅਗਲਾ_val))
	ਵਾਪਸ (prev_val < ਅਗਲਾ_val) ? prev_delta : ਅਗਲਾ_delta;
      ਨਹੀਂਤਾਂ ਜੇ (ਨਿਸ਼ਾਨਾ_val >= hb_max (prev_val, ਅਗਲਾ_val))
	ਵਾਪਸ (prev_val > ਅਗਲਾ_val) ? prev_delta : ਅਗਲਾ_delta;

      /* linear interpolation */
      ਦਸ਼ਮਲਵ r = (ਨਿਸ਼ਾਨਾ_val - prev_val) / (ਅਗਲਾ_val - prev_val);
      ਵਾਪਸ prev_delta + r * (ਅਗਲਾ_delta - prev_delta);
    }

    ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅਗਲਾ_ਸੂਚਕਾਂਕ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸ਼ੁਰੂ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸਿਰਾ)
    { ਵਾਪਸ (i >= ਸਿਰਾ) ? ਸ਼ੁਰੂ : (i + 1); }

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_OPTIMIZE_ਮਾਪ
    ਸਾਂਚਾ <ਤਾਰਕਿਕ is_x>
#ਜੇ_ਅੰਤ
    ਸਥਿਰ ਤਾਰਕਿਕ decompile_deltas_ਜੋੜੋ_ਮੰਜ਼ਿਲ_points (ਅਚਲ HBUINT8 *&p /* IN/OUT */,
						hb_array_t<contour_ਬਿੰਦੂ_t> points,
						ਦਸ਼ਮਲਵ scalar,
						ਅਚਲ HBUINT8 *ਸਿਰਾ,
						ਬਿਨਾਨਿਸ਼ਾਨੀ ਸ਼ੁਰੂ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HB_OPTIMIZE_ਮਾਪ
						, ਤਾਰਕਿਕ is_x
#ਜੇ_ਅੰਤ
						)
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0;
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2 = points.ਲੰਮਾਈ;
      ਜਦੋਂ (i < ਗਿਣਤੀ_2)
      {
	ਜੇ (unlikely (p + 1 > ਸਿਰਾ)) ਵਾਪਸ ਝੂਠ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ control = *p++;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੜੀਵਾਰ_ਗਿਣਤੀ = (control & Tuple_ਮੁੱਲਾਂ::ਮੁੱਲ_ਲੜੀਵਾਰ_ਗਿਣਤੀ_ਨਕਾਬ) + 1;
	ਬਿਨਾਨਿਸ਼ਾਨੀ stop = i + ਲੜੀਵਾਰ_ਗਿਣਤੀ;
	ਜੇ (unlikely (stop > ਗਿਣਤੀ_2)) ਵਾਪਸ ਝੂਠ;

	ਬਿਨਾਨਿਸ਼ਾਨੀ skip = i < ਸ਼ੁਰੂ ? hb_min (ਸ਼ੁਰੂ - i, ਲੜੀਵਾਰ_ਗਿਣਤੀ) : 0;
	i += skip;

	ਚੋਣ (control & Tuple_ਮੁੱਲਾਂ::ਮੁੱਲਾਂ_ਮਾਪ_ਨਕਾਬ)
	{
	  ਹਾਲਤ Tuple_ਮੁੱਲਾਂ::ਮੁੱਲਾਂ_ARE_ZEROS:
	    i = stop;
	    ਰੋਕੋ;
	  ਹਾਲਤ Tuple_ਮੁੱਲਾਂ::ਮੁੱਲਾਂ_ARE_ਸ਼ਬਦ:
	  {
	    ਜੇ (unlikely (p + ਲੜੀਵਾਰ_ਗਿਣਤੀ * HBINT16::static_ਮਾਪ > ਸਿਰਾ)) ਵਾਪਸ ਝੂਠ;
	    p += skip * HBINT16::static_ਮਾਪ;
	    ਅਚਲ ਸਵੈਚਾਲਿਤ *pp = (ਅਚਲ HBINT16 *) p;
	    ਲਈ (; i < stop; i++)
	    {
	      ਦਸ਼ਮਲਵ v = *pp++ * scalar;
	      ਜੇ (is_x) points.arrayZ[i].x += v;
	      ਨਹੀਂਤਾਂ points.arrayZ[i].y += v;
	    }
	    p = (ਅਚਲ HBUINT8 *) pp;
	  }
	  ਰੋਕੋ;
	  ਹਾਲਤ Tuple_ਮੁੱਲਾਂ::ਮੁੱਲਾਂ_ARE_LONGS:
	  {
	    ਜੇ (unlikely (p + ਲੜੀਵਾਰ_ਗਿਣਤੀ * HBINT32::static_ਮਾਪ > ਸਿਰਾ)) ਵਾਪਸ ਝੂਠ;
	    p += skip * HBINT32::static_ਮਾਪ;
	    ਅਚਲ ਸਵੈਚਾਲਿਤ *pp = (ਅਚਲ HBINT32 *) p;
	    ਲਈ (; i < stop; i++)
	    {
	      ਦਸ਼ਮਲਵ v = *pp++ * scalar;
	      ਜੇ (is_x) points.arrayZ[i].x += v;
	      ਨਹੀਂਤਾਂ points.arrayZ[i].y += v;
	    }
	    p = (ਅਚਲ HBUINT8 *) pp;
	  }
	  ਰੋਕੋ;
	  ਹਾਲਤ Tuple_ਮੁੱਲਾਂ::ਮੁੱਲਾਂ_ARE_ਬਾਈਟਾਂ:
	  {
	    ਜੇ (unlikely (p + ਲੜੀਵਾਰ_ਗਿਣਤੀ > ਸਿਰਾ)) ਵਾਪਸ ਝੂਠ;
	    p += skip * HBINT8::static_ਮਾਪ;
	    ਅਚਲ ਸਵੈਚਾਲਿਤ *pp = (ਅਚਲ HBINT8 *) p;
	    ਲਈ (; i < stop; i++)
	    {
	      ਦਸ਼ਮਲਵ v = *pp++ * scalar;
	      ਜੇ (is_x) points.arrayZ[i].x += v;
	      ਨਹੀਂਤਾਂ points.arrayZ[i].y += v;
	    }
	    p = (ਅਚਲ HBUINT8 *) pp;
	  }
	  ਰੋਕੋ;
	}
      }
      ਵਾਪਸ ਸੱਚ;
    }
    ਜਨਤਕ:
    ਤਾਰਕਿਕ apply_deltas_ਮੰਜ਼ਿਲ_points (hb_codepoint_t glyph,
				 hb_array_t<ਅਚਲ ਪੂਰਨ> coords,
				 ਅਚਲ hb_array_t<contour_ਬਿੰਦੂ_t> points,
				 hb_glyf_scratch_t &scratch,
				 hb_scalar_cache_t *gvar_cache = ਖਾਲੀ_ਸੂਚਕ,
				 ਤਾਰਕਿਕ phantom_only = ਝੂਠ,
				 int64_t *budget = ਖਾਲੀ_ਸੂਚਕ) ਅਚਲ
    {
      ਜੇ (unlikely (glyph >= glyph_ਗਿਣਤੀ_2)) ਵਾਪਸ ਸੱਚ;
      hb_scalar_cache_t *scalar_cache = gvar_cache ?
					gvar_cache :
					(hb_scalar_cache_t *) &Null(hb_scalar_cache_t);

      hb_ਬਾਈਟਾਂ_t var_ਅੰਕੜੇ_ਬਾਈਟਾਂ = ਸਾਰਣੀ->get_glyph_var_ਅੰਕੜੇ_ਬਾਈਟਾਂ (ਸਾਰਣੀ.get_blob (), glyph_ਗਿਣਤੀ_2, glyph);
      ਜੇ (!var_ਅੰਕੜੇ_ਬਾਈਟਾਂ.as<Glyph_Variation_ਅੰਕੜੇ> ()->has_ਅੰਕੜੇ ()) ਵਾਪਸ ਸੱਚ;

      ਸਵੈਚਾਲਿਤ &shared_indices = scratch.shared_indices;
      shared_indices.clear ();

      ਕਿਸਮ_ਨਾਂ Glyph_Variation_ਅੰਕੜੇ::tuple_iterator_t iterator;
      ਜੇ (!Glyph_Variation_ਅੰਕੜੇ::get_tuple_iterator (var_ਅੰਕੜੇ_ਬਾਈਟਾਂ, ਸਾਰਣੀ->axis_ਗਿਣਤੀ_2,
						   var_ਅੰਕੜੇ_ਬਾਈਟਾਂ.arrayZ,
						   shared_indices, &iterator))
	ਵਾਪਸ ਸੱਚ; /* so isn't applied at all */

      ਤਾਰਕਿਕ any_ਨਿੱਜੀ_points = ਝੂਠ;
      ਤਾਰਕਿਕ ਨਿੱਜੀ_points_checked = ਝੂਠ;

      /* Save original points for inferred delta calculation */
      ਸਵੈਚਾਲਿਤ &orig_points_vec = scratch.orig_points;
      orig_points_vec.clear (); // Populated lazily
      ਸਵੈਚਾਲਿਤ orig_points = orig_points_vec.as_array ();

      /* flag is used to indicate referenced point */
      ਸਵੈਚਾਲਿਤ &deltas_vec = scratch.deltas;
      deltas_vec.clear (); // Populated lazily
      ਸਵੈਚਾਲਿਤ deltas = deltas_vec.as_array ();

      ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_coords = ਸਾਰਣੀ->axis_ਗਿਣਤੀ_2;
      hb_array_t<ਅਚਲ F2DOT14> shared_tuples = (ਸਾਰਣੀ+ਸਾਰਣੀ->sharedTuples).as_array (ਸਾਰਣੀ->shared_Tuple_ਗਿਣਤੀ * ਅੰਕ_coords);

      ਸਵੈਚਾਲਿਤ &ਨਿੱਜੀ_indices = scratch.ਨਿੱਜੀ_indices;
      ਸਵੈਚਾਲਿਤ &x_deltas = scratch.x_deltas;
      ਸਵੈਚਾਲਿਤ &y_deltas = scratch.y_deltas;

      ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2 = points.ਲੰਮਾਈ;
      ਤਾਰਕਿਕ flush = ਝੂਠ;

      ਕਰੋ
      {
	ਦਸ਼ਮਲਵ scalar = iterator.current_tuple->calculate_scalar (coords, ਅੰਕ_coords, shared_tuples,
								 scalar_cache);

	ਜੇ (scalar == 0.f) ਜਾਰੀ;

	ਜੇ (budget &&
	    unlikely (!hb_budget_spend (*budget, HB_BUDGET_1,
					 phantom_only && ਗਿਣਤੀ_2 >= 4 ? 4 : ਗਿਣਤੀ_2)))
	  ਵਾਪਸ ਝੂਠ;

	ਜੇ (!ਨਿੱਜੀ_points_checked)
	{
	  ਸਵੈਚਾਲਿਤ ਜਾਂਚ = iterator;
	  ਕਰੋ
	  {
	    ਜੇ (ਜਾਂਚ.current_tuple->has_ਨਿੱਜੀ_points ())
	    {
	      any_ਨਿੱਜੀ_points = ਸੱਚ;
	      ਰੋਕੋ;
	    }
	  } ਜਦੋਂ (ਜਾਂਚ.move_ਮੰਜ਼ਿਲ_ਅਗਲਾ ());
	  ਨਿੱਜੀ_points_checked = ਸੱਚ;
	}
	ਅਚਲ HBUINT8 *p = iterator.get_serialized_ਅੰਕੜੇ ();
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਲੰਮਾਈ = iterator.current_tuple->get_ਅੰਕੜੇ_ਮਾਪ ();
	ਜੇ (unlikely (!iterator.var_ਅੰਕੜੇ_ਬਾਈਟਾਂ.check_range (p, ਲੰਮਾਈ)))
	  ਵਾਪਸ ਝੂਠ;

	ਜੇ (!deltas)
	{
	  ਜੇ (unlikely (!deltas_vec.resize_dirty  (ਗਿਣਤੀ_2))) ਵਾਪਸ ਝੂਠ;
	  deltas = deltas_vec.as_array ();
	  hb_memset (deltas.arrayZ + (phantom_only ? ਗਿਣਤੀ_2 - 4 : 0), 0,
		     (phantom_only ? 4 : ਗਿਣਤੀ_2) * ਆਕਾਰ (deltas[0]));
	}

	ਅਚਲ HBUINT8 *ਸਿਰਾ = p + ਲੰਮਾਈ;

	ਤਾਰਕਿਕ has_ਨਿੱਜੀ_points = iterator.current_tuple->has_ਨਿੱਜੀ_points ();
	ਜੇ (has_ਨਿੱਜੀ_points &&
	    !Glyph_Variation_ਅੰਕੜੇ::decompile_points (p, ਨਿੱਜੀ_indices, ਸਿਰਾ))
	  ਵਾਪਸ ਝੂਠ;
	ਅਚਲ hb_array_t<ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ> &indices = has_ਨਿੱਜੀ_points ? ਨਿੱਜੀ_indices : shared_indices;

	ਤਾਰਕਿਕ apply_ਮੰਜ਼ਿਲ_all = (indices.ਲੰਮਾਈ == 0);
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_deltas = apply_ਮੰਜ਼ਿਲ_all ? points.ਲੰਮਾਈ : indices.ਲੰਮਾਈ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਸ਼ੁਰੂ_deltas = (apply_ਮੰਜ਼ਿਲ_all && phantom_only && ਅੰਕ_deltas >= 4 ? ਅੰਕ_deltas - 4 : 0);

	ਜੇ (apply_ਮੰਜ਼ਿਲ_all && !any_ਨਿੱਜੀ_points)
	{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HB_OPTIMIZE_ਮਾਪ
	  ਜੇ (unlikely (!decompile_deltas_ਜੋੜੋ_ਮੰਜ਼ਿਲ_points (p, points, scalar, ਸਿਰਾ, ਸ਼ੁਰੂ_deltas, ਸੱਚ))) ਵਾਪਸ ਝੂਠ;
	  ਜੇ (unlikely (!decompile_deltas_ਜੋੜੋ_ਮੰਜ਼ਿਲ_points (p, points, scalar, ਸਿਰਾ, ਸ਼ੁਰੂ_deltas, ਝੂਠ))) ਵਾਪਸ ਝੂਠ;
#ਨਹੀਂਤਾਂ
	  ਜੇ (unlikely (!decompile_deltas_ਜੋੜੋ_ਮੰਜ਼ਿਲ_points<ਸੱਚ> (p, points, scalar, ਸਿਰਾ, ਸ਼ੁਰੂ_deltas))) ਵਾਪਸ ਝੂਠ;
	  ਜੇ (unlikely (!decompile_deltas_ਜੋੜੋ_ਮੰਜ਼ਿਲ_points<ਝੂਠ> (p, points, scalar, ਸਿਰਾ, ਸ਼ੁਰੂ_deltas))) ਵਾਪਸ ਝੂਠ;
#ਜੇ_ਅੰਤ
	  ਜਾਰੀ;
	}

	ਜੇ (unlikely (!x_deltas.resize_dirty  (ਅੰਕ_deltas))) ਵਾਪਸ ਝੂਠ;
	ਜੇ (unlikely (!Glyph_Variation_ਅੰਕੜੇ::decompile_deltas (p, x_deltas, ਸਿਰਾ, ਝੂਠ, ਸ਼ੁਰੂ_deltas))) ਵਾਪਸ ਝੂਠ;
	ਜੇ (unlikely (!y_deltas.resize_dirty  (ਅੰਕ_deltas))) ਵਾਪਸ ਝੂਠ;
	ਜੇ (unlikely (!Glyph_Variation_ਅੰਕੜੇ::decompile_deltas (p, y_deltas, ਸਿਰਾ, ਝੂਠ, ਸ਼ੁਰੂ_deltas))) ਵਾਪਸ ਝੂਠ;

	ਜੇ (!apply_ਮੰਜ਼ਿਲ_all)
	{
	  ਜੇ (!orig_points && !phantom_only)
	  {
	    orig_points_vec.extend (points);
	    ਜੇ (unlikely (orig_points_vec.ਵਿੱਚ_error ())) ਵਾਪਸ ਝੂਠ;
	    orig_points = orig_points_vec.as_array ();
	  }

	  ਜੇ (flush)
	  {
	    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = phantom_only ? ਗਿਣਤੀ_2 - 4 : 0; i < ਗਿਣਤੀ_2; i++)
	      points.arrayZ[i].translate (deltas.arrayZ[i]);
	  }
	  hb_memset (deltas.arrayZ + (phantom_only ? ਗਿਣਤੀ_2 - 4 : 0), 0,
		     (phantom_only ? 4 : ਗਿਣਤੀ_2) * ਆਕਾਰ (deltas[0]));
	}

	ਜੇ (HB_OPTIMIZE_ਮਾਪ_VAL)
	{
	  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਅੰਕ_deltas; i++)
	  {
	    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ pt_ਸੂਚਕਾਂਕ;
	    ਜੇ (apply_ਮੰਜ਼ਿਲ_all)
	      pt_ਸੂਚਕਾਂਕ = i;
	    ਨਹੀਂਤਾਂ
	    {
	      pt_ਸੂਚਕਾਂਕ = indices[i];
	      ਜੇ (unlikely (pt_ਸੂਚਕਾਂਕ >= deltas.ਲੰਮਾਈ)) ਜਾਰੀ;
	    }
	    ਜੇ (phantom_only && pt_ਸੂਚਕਾਂਕ < ਗਿਣਤੀ_2 - 4) ਜਾਰੀ;
	    ਸਵੈਚਾਲਿਤ &delta = deltas.arrayZ[pt_ਸੂਚਕਾਂਕ];
	    delta.flag = 1;	/* this point is referenced, i.e., explicit deltas specified */
	    delta.ਜੋੜੋ_delta (x_deltas.arrayZ[i] * scalar,
			     y_deltas.arrayZ[i] * scalar);
	  }
	}
	ਨਹੀਂਤਾਂ
	{
	  /* Ouch. Four cases... for optimization. */
	  ਜੇ (scalar != 1.0f)
	  {
	    ਜੇ (apply_ਮੰਜ਼ਿਲ_all)
	      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = phantom_only ? ਗਿਣਤੀ_2 - 4 : 0; i < ਗਿਣਤੀ_2; i++)
	      {
		ਸਵੈਚਾਲਿਤ &delta = deltas.arrayZ[i];
		delta.ਜੋੜੋ_delta (x_deltas.arrayZ[i] * scalar,
				 y_deltas.arrayZ[i] * scalar);
	      }
	    ਨਹੀਂਤਾਂ
	      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਅੰਕ_deltas; i++)
	      {
		ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ pt_ਸੂਚਕਾਂਕ = indices[i];
		ਜੇ (unlikely (pt_ਸੂਚਕਾਂਕ >= deltas.ਲੰਮਾਈ)) ਜਾਰੀ;
		ਜੇ (phantom_only && pt_ਸੂਚਕਾਂਕ < ਗਿਣਤੀ_2 - 4) ਜਾਰੀ;
		ਸਵੈਚਾਲਿਤ &delta = deltas.arrayZ[pt_ਸੂਚਕਾਂਕ];
		delta.flag = 1;	/* this point is referenced, i.e., explicit deltas specified */
		delta.ਜੋੜੋ_delta (x_deltas.arrayZ[i] * scalar,
				 y_deltas.arrayZ[i] * scalar);
	      }
	  }
	  ਨਹੀਂਤਾਂ
	  {
	    ਜੇ (apply_ਮੰਜ਼ਿਲ_all)
	      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = phantom_only ? ਗਿਣਤੀ_2 - 4 : 0; i < ਗਿਣਤੀ_2; i++)
	      {
		ਸਵੈਚਾਲਿਤ &delta = deltas.arrayZ[i];
		delta.ਜੋੜੋ_delta (x_deltas.arrayZ[i],
				 y_deltas.arrayZ[i]);
	      }
	    ਨਹੀਂਤਾਂ
	      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਅੰਕ_deltas; i++)
	      {
		ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ pt_ਸੂਚਕਾਂਕ = indices[i];
		ਜੇ (unlikely (pt_ਸੂਚਕਾਂਕ >= deltas.ਲੰਮਾਈ)) ਜਾਰੀ;
		ਜੇ (phantom_only && pt_ਸੂਚਕਾਂਕ < ਗਿਣਤੀ_2 - 4) ਜਾਰੀ;
		ਸਵੈਚਾਲਿਤ &delta = deltas.arrayZ[pt_ਸੂਚਕਾਂਕ];
		delta.flag = 1;	/* this point is referenced, i.e., explicit deltas specified */
		delta.ਜੋੜੋ_delta (x_deltas.arrayZ[i],
				 y_deltas.arrayZ[i]);
	      }
	  }
	}

	/* infer deltas for unreferenced points */
	ਜੇ (!apply_ਮੰਜ਼ਿਲ_all && !phantom_only)
	{
	  ਬਿਨਾਨਿਸ਼ਾਨੀ ਸ਼ੁਰੂ_ਬਿੰਦੂ_2 = 0;
	  ਬਿਨਾਨਿਸ਼ਾਨੀ ਸਿਰਾ_ਬਿੰਦੂ = 0;
	  ਜਦੋਂ (ਸੱਚ)
	  {
	    ਜਦੋਂ (ਸਿਰਾ_ਬਿੰਦੂ < ਗਿਣਤੀ_2 && !points.arrayZ[ਸਿਰਾ_ਬਿੰਦੂ].is_ਸਿਰਾ_ਬਿੰਦੂ)
	      ਸਿਰਾ_ਬਿੰਦੂ++;
	    ਜੇ (unlikely (ਸਿਰਾ_ਬਿੰਦੂ == ਗਿਣਤੀ_2)) ਰੋਕੋ;

	    /* Check the number of unreferenced points in a contour. If no unref points or no ref points, nothing to do. */
	    ਬਿਨਾਨਿਸ਼ਾਨੀ unref_ਗਿਣਤੀ = 0;
	    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = ਸ਼ੁਰੂ_ਬਿੰਦੂ_2; i < ਸਿਰਾ_ਬਿੰਦੂ + 1; i++)
	      unref_ਗਿਣਤੀ += deltas.arrayZ[i].flag;
	    unref_ਗਿਣਤੀ = (ਸਿਰਾ_ਬਿੰਦੂ - ਸ਼ੁਰੂ_ਬਿੰਦੂ_2 + 1) - unref_ਗਿਣਤੀ;

	    ਬਿਨਾਨਿਸ਼ਾਨੀ j = ਸ਼ੁਰੂ_ਬਿੰਦੂ_2;
	    ਜੇ (unref_ਗਿਣਤੀ == 0 || unref_ਗਿਣਤੀ > ਸਿਰਾ_ਬਿੰਦੂ - ਸ਼ੁਰੂ_ਬਿੰਦੂ_2)
	      ਜਾਓ no_ਹੋਰ_gaps;

	    ਲਈ (;;)
	    {
	      /* Locate the next gap of unreferenced points between two referenced points prev and next.
	       * Note that a gap may wrap around at left (start_point) and/or at right (end_point).
	       */
	      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ prev, next, i;
	      ਲਈ (;;)
	      {
		i = j;
		j = ਅਗਲਾ_ਸੂਚਕਾਂਕ (i, ਸ਼ੁਰੂ_ਬਿੰਦੂ_2, ਸਿਰਾ_ਬਿੰਦੂ);
		ਜੇ (deltas.arrayZ[i].flag && !deltas.arrayZ[j].flag) ਰੋਕੋ;
	      }
	      prev = j = i;
	      ਲਈ (;;)
	      {
		i = j;
		j = ਅਗਲਾ_ਸੂਚਕਾਂਕ (i, ਸ਼ੁਰੂ_ਬਿੰਦੂ_2, ਸਿਰਾ_ਬਿੰਦੂ);
		ਜੇ (!deltas.arrayZ[i].flag && deltas.arrayZ[j].flag) ਰੋਕੋ;
	      }
	      next = j;
	      /* Infer deltas for all unref points in the gap between prev and next */
	      i = prev;
	      ਲਈ (;;)
	      {
		i = ਅਗਲਾ_ਸੂਚਕਾਂਕ (i, ਸ਼ੁਰੂ_ਬਿੰਦੂ_2, ਸਿਰਾ_ਬਿੰਦੂ);
		ਜੇ (i == next) ਰੋਕੋ;
		deltas.arrayZ[i].x = infer_delta (orig_points, deltas, i, prev, next, &contour_ਬਿੰਦੂ_t::x);
		deltas.arrayZ[i].y = infer_delta (orig_points, deltas, i, prev, next, &contour_ਬਿੰਦੂ_t::y);
		ਜੇ (--unref_ਗਿਣਤੀ == 0) ਜਾਓ no_ਹੋਰ_gaps;
	      }
	    }
	  no_ਹੋਰ_gaps:
	    ਸ਼ੁਰੂ_ਬਿੰਦੂ_2 = ਸਿਰਾ_ਬਿੰਦੂ = ਸਿਰਾ_ਬਿੰਦੂ + 1;
	  }
	}

	flush = ਸੱਚ;

      } ਜਦੋਂ (iterator.move_ਮੰਜ਼ਿਲ_ਅਗਲਾ ());

      ਜੇ (flush)
      {
	ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = phantom_only ? ਗਿਣਤੀ_2 - 4 : 0; i < ਗਿਣਤੀ_2; i++)
	  points.arrayZ[i].translate (deltas.arrayZ[i]);
      }

      ਵਾਪਸ ਸੱਚ;
    }

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_axis_ਗਿਣਤੀ () ਅਚਲ { ਵਾਪਸ ਸਾਰਣੀ->axis_ਗਿਣਤੀ_2; }

    ਨਿਜੀ:
    hb_blob_ptr_t<gvar_GVAR> ਸਾਰਣੀ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ glyph_ਗਿਣਤੀ_2;
  };

  ਸੁਰੱਖਿਅਤ:
  Fixed_ਸੰਸਕਰਣ<>ਸੰਸਕਰਣ;	/* Version number of the glyph variations table
				 * Set to 0x00010000u. */
  HBUINT16	axis_ਗਿਣਤੀ_2;	/* The number of variation axes for this font. This must be
				 * the same number as axisCount in the 'fvar' table. */
  HBUINT16	shared_Tuple_ਗਿਣਤੀ;
				/* The number of shared tuple records. Shared tuple records
				 * can be referenced within glyph variation data tables for
				 * multiple glyphs, as opposed to other tuple records stored
				 * directly within a glyph variation data table. */
  NNOffset32_ਮੰਜ਼ਿਲ<Unsized_Array_ਦਾ<F2DOT14>>
		sharedTuples;	/* Offset from the start of this table to the shared tuple records.
				 * Array of tuple records shared across all glyph variation data tables. */
  Glyph_ਗਿਣਤੀ_Type
		glyph_ਗਿਣਤੀ_X;	/* The number of glyphs in this font. This must match the number of
				 * glyphs stored elsewhere in the font. */
  HBUINT16	flags;		/* Bit-field that gives the format of the offset array that follows.
				 * If bit 0 is clear, the offsets are uint16; if bit 0 is set, the
				 * offsets are uint32. */
  Offset32_ਮੰਜ਼ਿਲ<Glyph_Variation_ਅੰਕੜੇ>
		ਅੰਕੜੇ_Z;		/* Offset from the start of this table to the array of
				 * GlyphVariationData tables. */
  Unsized_Array_ਦਾ<HBUINT8>
		ਵਿੱਥ_Z;	/* Offsets from the start of the GlyphVariationData array
				 * to each GlyphVariationData table. */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_ARRAY (18 + Glyph_ਗਿਣਤੀ_Type::static_ਮਾਪ, ਵਿੱਥ_Z);
};

ਬਣਤਰ gvar : gvar_GVAR<gvar, SmallTypes>
{
  ਸਥਿਰ constexpr hb_ਟੈਗ_t ਸਾਰਣੀ_ਟੈਗ = HB_OT_ਟੈਗ_gvar;
};

ਬਣਤਰ GVAR : gvar_GVAR<GVAR, MediumTypes>
{
  ਸਥਿਰ constexpr hb_ਟੈਗ_t ਸਾਰਣੀ_ਟੈਗ = HB_OT_ਟੈਗ_GVAR;
};

ਬਣਤਰ gvar_accelerator_t : hb_dual_accelerator_t<gvar::accelerator_t,
						  GVAR::accelerator_t>
{
  gvar_accelerator_t (hb_face_t *face) :
    hb_dual_accelerator_t<gvar::accelerator_t, GVAR::accelerator_t> (face) {}

  hb_scalar_cache_t *create_cache () ਅਚਲ
  {
    ਜੇ (has_ਅੰਕੜੇ ()) ਵਾਪਸ HB_DUAL_GET (*ਇਹ, create_cache ());
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
  }

  ਸਥਿਰ ਖਾਲੀ destroy_cache (hb_scalar_cache_t *cache)
  {
    hb_scalar_cache_t::destroy (cache);
  }

  ਤਾਰਕਿਕ apply_deltas_ਮੰਜ਼ਿਲ_points (hb_codepoint_t glyph,
			       hb_array_t<ਅਚਲ ਪੂਰਨ> coords,
			       ਅਚਲ hb_array_t<contour_ਬਿੰਦੂ_t> points,
			       hb_glyf_scratch_t &scratch,
			       hb_scalar_cache_t *gvar_cache = ਖਾਲੀ_ਸੂਚਕ,
			       ਤਾਰਕਿਕ phantom_only = ਝੂਠ,
			       int64_t *budget = ਖਾਲੀ_ਸੂਚਕ) ਅਚਲ
  {
    ਵਾਪਸ HB_DUAL_GET (*ਇਹ, apply_deltas_ਮੰਜ਼ਿਲ_points (glyph, coords, points,
						       scratch, gvar_cache,
						       phantom_only, budget));
  }

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_axis_ਗਿਣਤੀ () ਅਚਲ
  {
    ਜੇ (has_ਅੰਕੜੇ ()) ਵਾਪਸ HB_DUAL_GET (*ਇਹ, get_axis_ਗਿਣਤੀ ());
    ਵਾਪਸ 0;
  }
};

} /* namespace OT */

#ਜੇ_ਅੰਤ /* HB_OT_VAR_GVAR_TABLE_HH */
