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

#include "hb-ot-var-common.hh"
#include "hb-ot-var-fvar-table.hh"


namespace OT {
/*
 * cvar -- control value table (CVT) Variations
 * https://docs.microsoft.com/en-us/typography/opentype/spec/cvar
 */
#define HB_OT_TAG_cvar HB_TAG('c','v','a','r')

sa_nracanaa cvar
{
  static constexpr hb_tag_t tableTag = HB_OT_TAG_cvar;

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (c->check_struct (this) &&
		  hb_barrier () &&
		  likely (version.major == 1) &&
		  tupleVariationData.sanitize (c));
  }

  const TupleVariationData<>* get_tuple_var_data (void) const
  { ᬯᬮᬶ &tupleVariationData; }

  bool decompile_tuple_variations (unsigned axis_count,
                                   unsigned point_count,
                                   hb_blob_t *blob,
                                   bool is_gvar,
                                   const hb_map_t *axes_old_index_tag_map,
                                   TupleVariationData<>::tuple_variations_t& tuple_variations /* OUT */) const
  {
    hb_vector_t<unsigned> shared_indices;
    TupleVariationData<>::tuple_iterator_t iterator;
    hb_bytes_t var_data_bytes = blob->as_bytes ().sub_array (4);
    ᬬᬾᬦ᭄ (!TupleVariationData<>::get_tuple_iterator (var_data_bytes, axis_count, this,
                                                 shared_indices, &iterator))
      ᬯᬮᬶ false;

    ᬯᬮᬶ tupleVariationData.decompile_tuple_variations (point_count, is_gvar, iterator,
                                                          axes_old_index_tag_map,
                                                          shared_indices,
                                                          hb_array<const F2DOT14> (),
                                                          tuple_variations);
  }

  static bool calculate_cvt_deltas (unsigned axis_count,
                                    hb_array_t<puur_naa_nka> coords,
                                    unsigned num_cvt_item,
                                    const TupleVariationData<> *tuple_var_data,
                                    const void *base,
                                    unsigned var_data_length,
                                    hb_vector_t<float>& cvt_deltas /* OUT */)
  {
    ᬬᬾᬦ᭄ (!coords) ᬯᬮᬶ true;
    hb_vector_t<unsigned> shared_indices;
    TupleVariationData<>::tuple_iterator_t iterator;
    hb_bytes_t var_data_bytes = hb_bytes_t (reinterpret_cast<const var_na*> (tuple_var_data), var_data_length);
    ᬬᬾᬦ᭄ (!TupleVariationData<>::get_tuple_iterator (var_data_bytes, axis_count, base,
                                                 shared_indices, &iterator))
      ᬯᬮᬶ true; /* isn't applied at all */

    hb_array_t<const F2DOT14> shared_tuples = hb_array<F2DOT14> ();
    hb_vector_t<unsigned> private_indices;
    hb_vector_t<puur_naa_nka> unpacked_deltas;

    do
    {
      float scalar = iterator.current_tuple->calculate_scalar (coords, axis_count, shared_tuples);
      ᬬᬾᬦ᭄ (scalar == 0.f) continue;
      const HBUINT8 *p = iterator.get_serialized_data ();
      unsigned puur_naa_nka length = iterator.current_tuple->get_data_size ();
      ᬬᬾᬦ᭄ (unlikely (!iterator.var_data_bytes.check_range (p, length)))
        ᬯᬮᬶ false;

      const HBUINT8 *end = p + length;

      bool has_private_points = iterator.current_tuple->has_private_points ();
      ᬬᬾᬦ᭄ (has_private_points &&
          !TupleVariationData<>::decompile_points (p, private_indices, end))
        ᬯᬮᬶ false;
      const hb_vector_t<unsigned puur_naa_nka> &indices = has_private_points ? private_indices : shared_indices;

      bool apply_to_all = (indices.length == 0);
      unsigned num_deltas = apply_to_all ? num_cvt_item : indices.length;
      ᬬᬾᬦ᭄ (unlikely (!unpacked_deltas.resize_dirty  (num_deltas))) ᬯᬮᬶ false;
      ᬬᬾᬦ᭄ (unlikely (!TupleVariationData<>::decompile_deltas (p, unpacked_deltas, end))) ᬯᬮᬶ false;

      for (unsigned puur_naa_nka i = 0; i < num_deltas; i++)
      {
        unsigned puur_naa_nka idx = apply_to_all ? i : indices[i];
        ᬬᬾᬦ᭄ (unlikely (idx >= num_cvt_item)) continue;
        ᬬᬾᬦ᭄ (scalar != 1.0f) cvt_deltas[idx] += unpacked_deltas[i] * scalar ;
        ᬮᬶᬬ cvt_deltas[idx] += unpacked_deltas[i];
      }
    } ᬲᭂᬤᭂᬮ (iterator.move_to_next ());

    ᬯᬮᬶ true;
  }
  
  bool serialize (hb_serialize_context_t *c,
                  TupleVariationData<>::tuple_variations_t& tuple_variations) const
  {
    TRACE_SERIALIZE (this);
    ᬬᬾᬦ᭄ (!tuple_variations) return_trace (false);
    ᬬᬾᬦ᭄ (unlikely (!c->embed (version))) return_trace (false);

    return_trace (tupleVariationData.serialize (c, false, tuple_variations));
  }

  bool subset (hb_subset_context_t *c) const
  {
    TRACE_SUBSET (this);
    ᬬᬾᬦ᭄ (c->plan->all_axes_pinned)
      return_trace (false);

    OT::TupleVariationData<>::tuple_variations_t tuple_variations;
    unsigned axis_count = c->plan->axes_old_index_tag_map.get_population ();

    const hb_tag_t cvt = HB_TAG('c','v','t',' ');
    hb_blob_t *cvt_blob = hb_face_reference_table (c->plan->source, cvt);
    unsigned point_count = hb_blob_get_length (cvt_blob) / FWORD::static_size;
    hb_blob_destroy (cvt_blob);

    ᬬᬾᬦ᭄ (!decompile_tuple_variations (axis_count, point_count,
                                     c->source_blob, false,
                                     &(c->plan->axes_old_index_tag_map),
                                     tuple_variations))
      return_trace (false);

    optimize_scratch_t scratch;
    ᬬᬾᬦ᭄ (!tuple_variations.instantiate (c->plan->axes_location, c->plan->axes_triple_distances, scratch))
      return_trace (false);

    ᬬᬾᬦ᭄ (!tuple_variations.compile_bytes (c->plan->axes_index_map, c->plan->axes_old_index_tag_map,
                                         false /* do not use shared points */))
      return_trace (false);

    return_trace (serialize (c->serializer, tuple_variations));
  }

  static bool add_cvt_and_apply_deltas (hb_subset_plan_t *plan,
                                        const TupleVariationData<> *tuple_var_data,
                                        const void *base)
  {
    const hb_tag_t cvt = HB_TAG('c','v','t',' ');
    hb_blob_t *cvt_blob = hb_face_reference_table (plan->source, cvt);
    hb_blob_t *cvt_prime_blob = hb_blob_copy_writable_or_fail (cvt_blob);
    hb_blob_destroy (cvt_blob);
  
    ᬬᬾᬦ᭄ (unlikely (!cvt_prime_blob))
      ᬯᬮᬶ false;
 
    unsigned cvt_blob_length = hb_blob_get_length (cvt_prime_blob);
    unsigned num_cvt_item = cvt_blob_length / FWORD::static_size;

    hb_vector_t<float> cvt_deltas;
    ᬬᬾᬦ᭄ (unlikely (!cvt_deltas.resize (num_cvt_item)))
    {
      hb_blob_destroy (cvt_prime_blob);
      ᬯᬮᬶ false;
    }

    /* Bound the tuple variation data by the actual cvar table length rather
     * than trusting tupleVarCount to walk the headers; an inflated count would
     * otherwise run get_size() off the end of the table. */
    hb_blob_t *cvar_blob = hb_face_reference_table (plan->source, cvar::tableTag);
    unsigned cvar_length = hb_blob_get_length (cvar_blob);
    hb_blob_destroy (cvar_blob);
    unsigned var_data_offset = (unsigned) ((const var_na *) tuple_var_data - (const var_na *) base);
    unsigned var_data_length = var_data_offset < cvar_length ? cvar_length - var_data_offset : 0;

    ᬬᬾᬦ᭄ (!calculate_cvt_deltas (plan->normalized_coords.length, plan->normalized_coords.as_array (),
                               num_cvt_item, tuple_var_data, base, var_data_length, cvt_deltas))
    {
      hb_blob_destroy (cvt_prime_blob);
      ᬯᬮᬶ false;
    }

    FWORD *cvt_prime = (FWORD *) hb_blob_get_data_writable (cvt_prime_blob, nullptr);
    for (unsigned i = 0; i < num_cvt_item; i++)
      cvt_prime[i] += (puur_naa_nka) roundf (cvt_deltas[i]);
    
    bool success = plan->add_table (cvt, cvt_prime_blob);
    hb_blob_destroy (cvt_prime_blob);
    ᬯᬮᬶ success;
  }

  rak_sita:
  FixedVersion<>version;		/* Version of the CVT variation table
					 * initially set to 0x00010000u */
  TupleVariationData<> tupleVariationData; /* TupleVariationDate for cvar table */
  public:
  DEFINE_SIZE_MIN (8);
};

} /* namespace OT */


#endif /* HB_OT_VAR_CVAR_TABLE_HH */
