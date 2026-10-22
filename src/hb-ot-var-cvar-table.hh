/*
 * Copyright © 2023  Google, Inc.
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
 */

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_OT_VAR_CVAR_ਸਾਰਣੀ_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_VAR_CVAR_ਸਾਰਣੀ_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-var-common.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-var-fvar-table.hh"


ਨਾਮਾਕਾਸ਼ OT {
/*
 * cvar -- control value table (CVT) Variations
 * https://docs.microsoft.com/en-us/typography/opentype/spec/cvar
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_ਟੈਗ_cvar HB_ਟੈਗ('c','v','a','r')

ਬਣਤਰ cvar
{
  ਸਥਿਰ constexpr hb_ਟੈਗ_t ਸਾਰਣੀ_ਟੈਗ = HB_OT_ਟੈਗ_cvar;

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ) &&
		  hb_barrier () &&
		  likely (ਸੰਸਕਰਣ.major == 1) &&
		  tuple_Variation_ਅੰਕੜੇ.sanitize (c));
  }

  ਅਚਲ Tuple_Variation_ਅੰਕੜੇ<>* get_tuple_var_ਅੰਕੜੇ (ਖਾਲੀ) ਅਚਲ
  { ਵਾਪਸ &tuple_Variation_ਅੰਕੜੇ; }

  ਤਾਰਕਿਕ decompile_tuple_variations (ਬਿਨਾਨਿਸ਼ਾਨੀ axis_ਗਿਣਤੀ,
                                   ਬਿਨਾਨਿਸ਼ਾਨੀ ਬਿੰਦੂ_ਗਿਣਤੀ,
                                   hb_blob_t *blob,
                                   ਤਾਰਕਿਕ is_gvar,
                                   ਅਚਲ hb_map_t *axes_old_ਸੂਚਕਾਂਕ_ਟੈਗ_map,
                                   Tuple_Variation_ਅੰਕੜੇ<>::tuple_variations_t& tuple_variations /*ਬਾਹਰਲਾ*/) ਅਚਲ
  {
    hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> shared_indices;
    Tuple_Variation_ਅੰਕੜੇ<>::tuple_iterator_t iterator;
    hb_ਬਾਈਟਾਂ_t var_ਅੰਕੜੇ_ਬਾਈਟਾਂ = blob->as_ਬਾਈਟਾਂ ().sub_array (4);
    ਜੇ (!Tuple_Variation_ਅੰਕੜੇ<>::get_tuple_iterator (var_ਅੰਕੜੇ_ਬਾਈਟਾਂ, axis_ਗਿਣਤੀ, ਇਹ,
                                                 shared_indices, &iterator))
      ਵਾਪਸ ਝੂਠ;

    ਵਾਪਸ tuple_Variation_ਅੰਕੜੇ.decompile_tuple_variations (ਬਿੰਦੂ_ਗਿਣਤੀ, is_gvar, iterator,
                                                          axes_old_ਸੂਚਕਾਂਕ_ਟੈਗ_map,
                                                          shared_indices,
                                                          hb_array<ਅਚਲ F2DOT14> (),
                                                          tuple_variations);
  }

  ਸਥਿਰ ਤਾਰਕਿਕ calculate_cvt_deltas (ਬਿਨਾਨਿਸ਼ਾਨੀ axis_ਗਿਣਤੀ,
                                    hb_array_t<ਪੂਰਨ> coords,
                                    ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_cvt_ਵਸਤੂ,
                                    ਅਚਲ Tuple_Variation_ਅੰਕੜੇ<> *tuple_var_ਅੰਕੜੇ,
                                    ਅਚਲ ਖਾਲੀ *ਅਧਾਰ,
                                    ਬਿਨਾਨਿਸ਼ਾਨੀ var_ਅੰਕੜੇ_ਲੰਮਾਈ,
                                    hb_ਸਦਿਸ਼_t<ਦਸ਼ਮਲਵ>& cvt_deltas /*ਬਾਹਰਲਾ*/)
  {
    ਜੇ (!coords) ਵਾਪਸ ਸੱਚ;
    hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> shared_indices;
    Tuple_Variation_ਅੰਕੜੇ<>::tuple_iterator_t iterator;
    hb_ਬਾਈਟਾਂ_t var_ਅੰਕੜੇ_ਬਾਈਟਾਂ = hb_ਬਾਈਟਾਂ_t (reinterpret_cast<ਅਚਲ ਅੱਖਰ*> (tuple_var_ਅੰਕੜੇ), var_ਅੰਕੜੇ_ਲੰਮਾਈ);
    ਜੇ (!Tuple_Variation_ਅੰਕੜੇ<>::get_tuple_iterator (var_ਅੰਕੜੇ_ਬਾਈਟਾਂ, axis_ਗਿਣਤੀ, ਅਧਾਰ,
                                                 shared_indices, &iterator))
      ਵਾਪਸ ਸੱਚ; /* isn't applied at all */

    hb_array_t<ਅਚਲ F2DOT14> shared_tuples = hb_array<F2DOT14> ();
    hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> ਨਿੱਜੀ_indices;
    hb_ਸਦਿਸ਼_t<ਪੂਰਨ> unpacked_deltas;

    ਕਰੋ
    {
      ਦਸ਼ਮਲਵ scalar = iterator.current_tuple->calculate_scalar (coords, axis_ਗਿਣਤੀ, shared_tuples);
      ਜੇ (scalar == 0.f) ਜਾਰੀ;
      ਅਚਲ HBUINT8 *p = iterator.get_serialized_ਅੰਕੜੇ ();
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਲੰਮਾਈ = iterator.current_tuple->get_ਅੰਕੜੇ_ਮਾਪ ();
      ਜੇ (unlikely (!iterator.var_ਅੰਕੜੇ_ਬਾਈਟਾਂ.check_range (p, ਲੰਮਾਈ)))
        ਵਾਪਸ ਝੂਠ;

      ਅਚਲ HBUINT8 *ਸਿਰਾ = p + ਲੰਮਾਈ;

      ਤਾਰਕਿਕ has_ਨਿੱਜੀ_points = iterator.current_tuple->has_ਨਿੱਜੀ_points ();
      ਜੇ (has_ਨਿੱਜੀ_points &&
          !Tuple_Variation_ਅੰਕੜੇ<>::decompile_points (p, ਨਿੱਜੀ_indices, ਸਿਰਾ))
        ਵਾਪਸ ਝੂਠ;
      ਅਚਲ hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ> &indices = has_ਨਿੱਜੀ_points ? ਨਿੱਜੀ_indices : shared_indices;

      ਤਾਰਕਿਕ apply_ਮੰਜ਼ਿਲ_all = (indices.ਲੰਮਾਈ == 0);
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_deltas = apply_ਮੰਜ਼ਿਲ_all ? ਅੰਕ_cvt_ਵਸਤੂ : indices.ਲੰਮਾਈ;
      ਜੇ (unlikely (!unpacked_deltas.resize_dirty  (ਅੰਕ_deltas))) ਵਾਪਸ ਝੂਠ;
      ਜੇ (unlikely (!Tuple_Variation_ਅੰਕੜੇ<>::decompile_deltas (p, unpacked_deltas, ਸਿਰਾ))) ਵਾਪਸ ਝੂਠ;

      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਅੰਕ_deltas; i++)
      {
        ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ idx = apply_ਮੰਜ਼ਿਲ_all ? i : indices[i];
        ਜੇ (unlikely (idx >= ਅੰਕ_cvt_ਵਸਤੂ)) ਜਾਰੀ;
        ਜੇ (scalar != 1.0f) cvt_deltas[idx] += unpacked_deltas[i] * scalar ;
        ਨਹੀਂਤਾਂ cvt_deltas[idx] += unpacked_deltas[i];
      }
    } ਜਦੋਂ (iterator.move_ਮੰਜ਼ਿਲ_ਅਗਲਾ ());

    ਵਾਪਸ ਸੱਚ;
  }
  
  ਤਾਰਕਿਕ serialize (hb_serialize_context_t *c,
                  Tuple_Variation_ਅੰਕੜੇ<>::tuple_variations_t& tuple_variations) ਅਚਲ
  {
    TRACE_SERIALIZE (ਇਹ);
    ਜੇ (!tuple_variations) return_trace (ਝੂਠ);
    ਜੇ (unlikely (!c->embed (ਸੰਸਕਰਣ))) return_trace (ਝੂਠ);

    return_trace (tuple_Variation_ਅੰਕੜੇ.serialize (c, ਝੂਠ, tuple_variations));
  }

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਜੇ (c->plan->all_axes_pinned)
      return_trace (ਝੂਠ);

    OT::Tuple_Variation_ਅੰਕੜੇ<>::tuple_variations_t tuple_variations;
    ਬਿਨਾਨਿਸ਼ਾਨੀ axis_ਗਿਣਤੀ = c->plan->axes_old_ਸੂਚਕਾਂਕ_ਟੈਗ_map.get_population ();

    ਅਚਲ hb_ਟੈਗ_t cvt = HB_ਟੈਗ('c','v','t',' ');
    hb_blob_t *cvt_blob = hb_face_reference_ਸਾਰਣੀ (c->plan->ਸਰੋਤ, cvt);
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਬਿੰਦੂ_ਗਿਣਤੀ = hb_blob_get_ਲੰਮਾਈ (cvt_blob) / FWORD::static_ਮਾਪ;
    hb_blob_destroy (cvt_blob);

    ਜੇ (!decompile_tuple_variations (axis_ਗਿਣਤੀ, ਬਿੰਦੂ_ਗਿਣਤੀ,
                                     c->ਸਰੋਤ_blob, ਝੂਠ,
                                     &(c->plan->axes_old_ਸੂਚਕਾਂਕ_ਟੈਗ_map),
                                     tuple_variations))
      return_trace (ਝੂਠ);

    optimize_scratch_t scratch;
    ਜੇ (!tuple_variations.instantiate (c->plan->axes_location, c->plan->axes_triple_distances, scratch))
      return_trace (ਝੂਠ);

    ਜੇ (!tuple_variations.compile_ਬਾਈਟਾਂ (c->plan->axes_ਸੂਚਕਾਂਕ_map, c->plan->axes_old_ਸੂਚਕਾਂਕ_ਟੈਗ_map,
                                         ਝੂਠ /* do not use shared points */))
      return_trace (ਝੂਠ);

    return_trace (serialize (c->serializer, tuple_variations));
  }

  ਸਥਿਰ ਤਾਰਕਿਕ ਜੋੜੋ_cvt_and_apply_deltas (hb_subset_plan_t *plan,
                                        ਅਚਲ Tuple_Variation_ਅੰਕੜੇ<> *tuple_var_ਅੰਕੜੇ,
                                        ਅਚਲ ਖਾਲੀ *ਅਧਾਰ)
  {
    ਅਚਲ hb_ਟੈਗ_t cvt = HB_ਟੈਗ('c','v','t',' ');
    hb_blob_t *cvt_blob = hb_face_reference_ਸਾਰਣੀ (plan->ਸਰੋਤ, cvt);
    hb_blob_t *cvt_ਅਭਾਜ_blob = hb_blob_copy_writable_or_fail (cvt_blob);
    hb_blob_destroy (cvt_blob);
  
    ਜੇ (unlikely (!cvt_ਅਭਾਜ_blob))
      ਵਾਪਸ ਝੂਠ;
 
    ਬਿਨਾਨਿਸ਼ਾਨੀ cvt_blob_ਲੰਮਾਈ = hb_blob_get_ਲੰਮਾਈ (cvt_ਅਭਾਜ_blob);
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_cvt_ਵਸਤੂ = cvt_blob_ਲੰਮਾਈ / FWORD::static_ਮਾਪ;

    hb_ਸਦਿਸ਼_t<ਦਸ਼ਮਲਵ> cvt_deltas;
    ਜੇ (unlikely (!cvt_deltas.resize (ਅੰਕ_cvt_ਵਸਤੂ)))
    {
      hb_blob_destroy (cvt_ਅਭਾਜ_blob);
      ਵਾਪਸ ਝੂਠ;
    }

    /* Bound the tuple variation data by the actual cvar table length rather
     * than trusting tupleVarCount to walk the headers; an inflated count would
     * otherwise run get_size() off the end of the table. */
    hb_blob_t *cvar_blob = hb_face_reference_ਸਾਰਣੀ (plan->ਸਰੋਤ, cvar::ਸਾਰਣੀ_ਟੈਗ);
    ਬਿਨਾਨਿਸ਼ਾਨੀ cvar_ਲੰਮਾਈ = hb_blob_get_ਲੰਮਾਈ (cvar_blob);
    hb_blob_destroy (cvar_blob);
    ਬਿਨਾਨਿਸ਼ਾਨੀ var_ਅੰਕੜੇ_ਵਿੱਥ = (ਬਿਨਾਨਿਸ਼ਾਨੀ) ((ਅਚਲ ਅੱਖਰ *) tuple_var_ਅੰਕੜੇ - (ਅਚਲ ਅੱਖਰ *) ਅਧਾਰ);
    ਬਿਨਾਨਿਸ਼ਾਨੀ var_ਅੰਕੜੇ_ਲੰਮਾਈ = var_ਅੰਕੜੇ_ਵਿੱਥ < cvar_ਲੰਮਾਈ ? cvar_ਲੰਮਾਈ - var_ਅੰਕੜੇ_ਵਿੱਥ : 0;

    ਜੇ (!calculate_cvt_deltas (plan->normalized_coords.ਲੰਮਾਈ, plan->normalized_coords.as_array (),
                               ਅੰਕ_cvt_ਵਸਤੂ, tuple_var_ਅੰਕੜੇ, ਅਧਾਰ, var_ਅੰਕੜੇ_ਲੰਮਾਈ, cvt_deltas))
    {
      hb_blob_destroy (cvt_ਅਭਾਜ_blob);
      ਵਾਪਸ ਝੂਠ;
    }

    FWORD *cvt_ਅਭਾਜ = (FWORD *) hb_blob_get_ਅੰਕੜੇ_writable (cvt_ਅਭਾਜ_blob, ਖਾਲੀ_ਸੂਚਕ);
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਅੰਕ_cvt_ਵਸਤੂ; i++)
      cvt_ਅਭਾਜ[i] += (ਪੂਰਨ) roundf (cvt_deltas[i]);
    
    ਤਾਰਕਿਕ success = plan->ਜੋੜੋ_ਸਾਰਣੀ (cvt, cvt_ਅਭਾਜ_blob);
    hb_blob_destroy (cvt_ਅਭਾਜ_blob);
    ਵਾਪਸ success;
  }

  ਸੁਰੱਖਿਅਤ:
  Fixed_ਸੰਸਕਰਣ<>ਸੰਸਕਰਣ;		/* Version of the CVT variation table
					 * initially set to 0x00010000u */
  Tuple_Variation_ਅੰਕੜੇ<> tuple_Variation_ਅੰਕੜੇ; /* TupleVariationDate for cvar table */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_MIN (8);
};

} /* namespace OT */


#ਜੇ_ਅੰਤ /* HB_OT_VAR_CVAR_TABLE_HH */
