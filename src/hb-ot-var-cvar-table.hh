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

#ifndef HB_OT_VAR_CVAR_TABLE_HH
#define HB_OT_VAR_CVAR_TABLE_HH

#ಅನ್ತರ್ಭಾವ "hb-ot-var-common.hh"
#ಅನ್ತರ್ಭಾವ "hb-ot-var-fvar-table.hh"


ನಾಮಾಕಾಶ OT {
/*
 * cvar -- control value table (CVT) Variations
 * https://docs.microsoft.com/en-us/typography/opentype/spec/cvar
 */
#define HB_OT_TAG_cvar HB_TAG('c','v','a','r')

ಸಂರಚನಾ cvar
{
  ಸ್ಥಿರ constexpr hb_tag_t tableTag = HB_OT_TAG_cvar;

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್) &&
		  hb_barrier () &&
		  likely (version.major == 1) &&
		  tupleVariationData.sanitize (c));
  }

  ಧ್ರುವ TupleVariationData<>* get_tuple_var_data (ಶೂನ್ಯ) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ &tupleVariationData; }

  ತಾರ್ಕಿಕ decompile_tuple_variations (unsigned axis_ಎಣಿಕೆ,
                                   unsigned point_ಎಣಿಕೆ,
                                   hb_blob_t *blob,
                                   ತಾರ್ಕಿಕ is_gvar,
                                   ಧ್ರುವ hb_map_t *axes_old_index_tag_map,
                                   TupleVariationData<>::tuple_variations_t& tuple_variations /* OUT */) ಧ್ರುವ
  {
    hb_vector_t<unsigned> shared_indices;
    TupleVariationData<>::tuple_iterator_t iterator;
    hb_bytes_t var_data_bytes = blob->as_bytes ().sub_array (4);
    ಯದಿ (!TupleVariationData<>::get_tuple_iterator (var_data_bytes, axis_ಎಣಿಕೆ, ಏತತ್,
                                                 shared_indices, &iterator))
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    ಪ್ರತ್ಯಾಗಮ tupleVariationData.decompile_tuple_variations (point_ಎಣಿಕೆ, is_gvar, iterator,
                                                          axes_old_index_tag_map,
                                                          shared_indices,
                                                          hb_array<ಧ್ರುವ F2DOT14> (),
                                                          tuple_variations);
  }

  ಸ್ಥಿರ ತಾರ್ಕಿಕ calculate_cvt_deltas (unsigned axis_ಎಣಿಕೆ,
                                    hb_array_t<ಪೂರ್ಣಾಙ್ಕ> coords,
                                    unsigned ಅಂಕೆ_cvt_item,
                                    ಧ್ರುವ TupleVariationData<> *tuple_var_data,
                                    ಧ್ರುವ ಶೂನ್ಯ *base,
                                    unsigned var_data_length,
                                    hb_vector_t<ಭಿನ್ನ>& cvt_deltas /* OUT */)
  {
    ಯದಿ (!coords) ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    hb_vector_t<unsigned> shared_indices;
    TupleVariationData<>::tuple_iterator_t iterator;
    hb_bytes_t var_data_bytes = hb_bytes_t (reinterpret_cast<ಧ್ರುವ ವರ್ಣ*> (tuple_var_data), var_data_length);
    ಯದಿ (!TupleVariationData<>::get_tuple_iterator (var_data_bytes, axis_ಎಣಿಕೆ, base,
                                                 shared_indices, &iterator))
      ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ; /* isn't applied at all */

    hb_array_t<ಧ್ರುವ F2DOT14> shared_tuples = hb_array<F2DOT14> ();
    hb_vector_t<unsigned> private_indices;
    hb_vector_t<ಪೂರ್ಣಾಙ್ಕ> unpacked_deltas;

    ಕುರು
    {
      ಭಿನ್ನ scalar = iterator.current_tuple->calculate_scalar (coords, axis_ಎಣಿಕೆ, shared_tuples);
      ಯದಿ (scalar == 0.f) ಅನುವರ್ತ;
      ಧ್ರುವ HBUINT8 *p = iterator.get_serialized_data ();
      unsigned ಪೂರ್ಣಾಙ್ಕ length = iterator.current_tuple->get_data_size ();
      ಯದಿ (unlikely (!iterator.var_data_bytes.check_range (p, length)))
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

      ಧ್ರುವ HBUINT8 *end = p + length;

      ತಾರ್ಕಿಕ has_private_points = iterator.current_tuple->has_private_points ();
      ಯದಿ (has_private_points &&
          !TupleVariationData<>::decompile_points (p, private_indices, end))
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
      ಧ್ರುವ hb_vector_t<unsigned ಪೂರ್ಣಾಙ್ಕ> &indices = has_private_points ? private_indices : shared_indices;

      ತಾರ್ಕಿಕ apply_to_all = (indices.length == 0);
      unsigned ಅಂಕೆ_deltas = apply_to_all ? ಅಂಕೆ_cvt_item : indices.length;
      ಯದಿ (unlikely (!unpacked_deltas.resize_dirty  (ಅಂಕೆ_deltas))) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
      ಯದಿ (unlikely (!TupleVariationData<>::decompile_deltas (p, unpacked_deltas, end))) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

      ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಅಂಕೆ_deltas; i++)
      {
        unsigned ಪೂರ್ಣಾಙ್ಕ idx = apply_to_all ? i : indices[i];
        ಯದಿ (unlikely (idx >= ಅಂಕೆ_cvt_item)) ಅನುವರ್ತ;
        ಯದಿ (scalar != 1.0f) cvt_deltas[idx] += unpacked_deltas[i] * scalar ;
        ಅನ್ಯಥಾ cvt_deltas[idx] += unpacked_deltas[i];
      }
    } ಯಾವತ್ (iterator.move_to_next ());

    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }
  
  ತಾರ್ಕಿಕ serialize (hb_serialize_context_t *c,
                  TupleVariationData<>::tuple_variations_t& tuple_variations) ಧ್ರುವ
  {
    TRACE_SERIALIZE (ಏತತ್);
    ಯದಿ (!tuple_variations) return_trace (ಅಸತ್ಯ);
    ಯದಿ (unlikely (!c->embed (version))) return_trace (ಅಸತ್ಯ);

    return_trace (tupleVariationData.serialize (c, ಅಸತ್ಯ, tuple_variations));
  }

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    ಯದಿ (c->plan->all_axes_pinned)
      return_trace (ಅಸತ್ಯ);

    OT::TupleVariationData<>::tuple_variations_t tuple_variations;
    unsigned axis_ಎಣಿಕೆ = c->plan->axes_old_index_tag_map.get_population ();

    ಧ್ರುವ hb_tag_t cvt = HB_TAG('c','v','t',' ');
    hb_blob_t *cvt_blob = hb_face_reference_table (c->plan->source, cvt);
    unsigned point_ಎಣಿಕೆ = hb_blob_get_length (cvt_blob) / FWORD::static_size;
    hb_blob_destroy (cvt_blob);

    ಯದಿ (!decompile_tuple_variations (axis_ಎಣಿಕೆ, point_ಎಣಿಕೆ,
                                     c->source_blob, ಅಸತ್ಯ,
                                     &(c->plan->axes_old_index_tag_map),
                                     tuple_variations))
      return_trace (ಅಸತ್ಯ);

    optimize_scratch_t scratch;
    ಯದಿ (!tuple_variations.instantiate (c->plan->axes_location, c->plan->axes_triple_distances, scratch))
      return_trace (ಅಸತ್ಯ);

    ಯದಿ (!tuple_variations.compile_bytes (c->plan->axes_index_map, c->plan->axes_old_index_tag_map,
                                         ಅಸತ್ಯ /* do not use shared points */))
      return_trace (ಅಸತ್ಯ);

    return_trace (serialize (c->serializer, tuple_variations));
  }

  ಸ್ಥಿರ ತಾರ್ಕಿಕ add_cvt_and_apply_deltas (hb_subset_plan_t *plan,
                                        ಧ್ರುವ TupleVariationData<> *tuple_var_data,
                                        ಧ್ರುವ ಶೂನ್ಯ *base)
  {
    ಧ್ರುವ hb_tag_t cvt = HB_TAG('c','v','t',' ');
    hb_blob_t *cvt_blob = hb_face_reference_table (plan->source, cvt);
    hb_blob_t *cvt_ಅವಿಭಾಜ್ಯ_blob = hb_blob_copy_writable_or_fail (cvt_blob);
    hb_blob_destroy (cvt_blob);
  
    ಯದಿ (unlikely (!cvt_ಅವಿಭಾಜ್ಯ_blob))
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
 
    unsigned cvt_blob_length = hb_blob_get_length (cvt_ಅವಿಭಾಜ್ಯ_blob);
    unsigned ಅಂಕೆ_cvt_item = cvt_blob_length / FWORD::static_size;

    hb_vector_t<ಭಿನ್ನ> cvt_deltas;
    ಯದಿ (unlikely (!cvt_deltas.resize (ಅಂಕೆ_cvt_item)))
    {
      hb_blob_destroy (cvt_ಅವಿಭಾಜ್ಯ_blob);
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }

    /* Bound the tuple variation data by the actual cvar table length rather
     * than trusting tupleVarCount to walk the headers; an inflated count would
     * otherwise run get_size() off the end of the table. */
    hb_blob_t *cvar_blob = hb_face_reference_table (plan->source, cvar::tableTag);
    unsigned cvar_length = hb_blob_get_length (cvar_blob);
    hb_blob_destroy (cvar_blob);
    unsigned var_data_offset = (unsigned) ((ಧ್ರುವ ವರ್ಣ *) tuple_var_data - (ಧ್ರುವ ವರ್ಣ *) base);
    unsigned var_data_length = var_data_offset < cvar_length ? cvar_length - var_data_offset : 0;

    ಯದಿ (!calculate_cvt_deltas (plan->normalized_coords.length, plan->normalized_coords.as_array (),
                               ಅಂಕೆ_cvt_item, tuple_var_data, base, var_data_length, cvt_deltas))
    {
      hb_blob_destroy (cvt_ಅವಿಭಾಜ್ಯ_blob);
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }

    FWORD *cvt_ಅವಿಭಾಜ್ಯ = (FWORD *) hb_blob_get_data_writable (cvt_ಅವಿಭಾಜ್ಯ_blob, nullptr);
    ಪ್ರತಿ (unsigned i = 0; i < ಅಂಕೆ_cvt_item; i++)
      cvt_ಅವಿಭಾಜ್ಯ[i] += (ಪೂರ್ಣಾಙ್ಕ) roundf (cvt_deltas[i]);
    
    ತಾರ್ಕಿಕ success = plan->add_table (cvt, cvt_ಅವಿಭಾಜ್ಯ_blob);
    hb_blob_destroy (cvt_ಅವಿಭಾಜ್ಯ_blob);
    ಪ್ರತ್ಯಾಗಮ success;
  }

  ರಕ್ಷಿತ:
  FixedVersion<>version;		/* Version of the CVT variation table
					 * initially set to 0x00010000u */
  TupleVariationData<> tupleVariationData; /* TupleVariationDate for cvar table */
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_MIN (8);
};

} /* namespace OT */


#endif /* HB_OT_VAR_CVAR_TABLE_HH */
