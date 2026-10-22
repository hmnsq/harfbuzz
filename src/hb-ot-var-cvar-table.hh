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

#अन्तर्भाव "hb-ot-var-common.hh"
#अन्तर्भाव "hb-ot-var-fvar-table.hh"


नामाकाश OT {
/*
 * cvar -- control value table (CVT) Variations
 * https://docs.microsoft.com/en-us/typography/opentype/spec/cvar
 */
#define HB_OT_TAG_cvar HB_TAG('c','v','a','r')

संरचना cvar
{
  स्थिर constexpr hb_tag_t tableTag = HB_OT_TAG_cvar;

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) &&
		  hb_barrier () &&
		  likely (version.major == 1) &&
		  tupleVariationData.sanitize (c));
  }

  ध्रुव TupleVariationData<>* get_tuple_var_data (शून्य) ध्रुव
  { पटिनिवत्त &tupleVariationData; }

  तार्किक decompile_tuple_variations (unsigned axis_गणनम्,
                                   unsigned point_गणनम्,
                                   hb_blob_t *blob,
                                   तार्किक is_gvar,
                                   ध्रुव hb_map_t *axes_old_index_tag_map,
                                   TupleVariationData<>::tuple_variations_t& tuple_variations /* OUT */) ध्रुव
  {
    hb_vector_t<unsigned> shared_indices;
    TupleVariationData<>::tuple_iterator_t iterator;
    hb_bytes_t var_data_bytes = blob->as_bytes ().sub_array (4);
    सचे (!TupleVariationData<>::get_tuple_iterator (var_data_bytes, axis_गणनम्, एतत्,
                                                 shared_indices, &iterator))
      पटिनिवत्त मिच्छा;

    पटिनिवत्त tupleVariationData.decompile_tuple_variations (point_गणनम्, is_gvar, iterator,
                                                          axes_old_index_tag_map,
                                                          shared_indices,
                                                          hb_array<ध्रुव F2DOT14> (),
                                                          tuple_variations);
  }

  स्थिर तार्किक calculate_cvt_deltas (unsigned axis_गणनम्,
                                    hb_array_t<पूर्णाङ्क> coords,
                                    unsigned अङ्कः_cvt_item,
                                    ध्रुव TupleVariationData<> *tuple_var_data,
                                    ध्रुव शून्य *base,
                                    unsigned var_data_length,
                                    hb_vector_t<भिन्न>& cvt_deltas /* OUT */)
  {
    सचे (!coords) पटिनिवत्त सच्च;
    hb_vector_t<unsigned> shared_indices;
    TupleVariationData<>::tuple_iterator_t iterator;
    hb_bytes_t var_data_bytes = hb_bytes_t (reinterpret_cast<ध्रुव वर्ण*> (tuple_var_data), var_data_length);
    सचे (!TupleVariationData<>::get_tuple_iterator (var_data_bytes, axis_गणनम्, base,
                                                 shared_indices, &iterator))
      पटिनिवत्त सच्च; /* isn't applied at all */

    hb_array_t<ध्रुव F2DOT14> shared_tuples = hb_array<F2DOT14> ();
    hb_vector_t<unsigned> private_indices;
    hb_vector_t<पूर्णाङ्क> unpacked_deltas;

    करोहि
    {
      भिन्न scalar = iterator.current_tuple->calculate_scalar (coords, axis_गणनम्, shared_tuples);
      सचे (scalar == 0.f) अनुवर्त;
      ध्रुव HBUINT8 *p = iterator.get_serialized_data ();
      unsigned पूर्णाङ्क length = iterator.current_tuple->get_data_size ();
      सचे (unlikely (!iterator.var_data_bytes.check_range (p, length)))
        पटिनिवत्त मिच्छा;

      ध्रुव HBUINT8 *end = p + length;

      तार्किक has_private_points = iterator.current_tuple->has_private_points ();
      सचे (has_private_points &&
          !TupleVariationData<>::decompile_points (p, private_indices, end))
        पटिनिवत्त मिच्छा;
      ध्रुव hb_vector_t<unsigned पूर्णाङ्क> &indices = has_private_points ? private_indices : shared_indices;

      तार्किक apply_to_all = (indices.length == 0);
      unsigned अङ्कः_deltas = apply_to_all ? अङ्कः_cvt_item : indices.length;
      सचे (unlikely (!unpacked_deltas.resize_dirty  (अङ्कः_deltas))) पटिनिवत्त मिच्छा;
      सचे (unlikely (!TupleVariationData<>::decompile_deltas (p, unpacked_deltas, end))) पटिनिवत्त मिच्छा;

      अत्थाय (unsigned पूर्णाङ्क i = 0; i < अङ्कः_deltas; i++)
      {
        unsigned पूर्णाङ्क idx = apply_to_all ? i : indices[i];
        सचे (unlikely (idx >= अङ्कः_cvt_item)) अनुवर्त;
        सचे (scalar != 1.0f) cvt_deltas[idx] += unpacked_deltas[i] * scalar ;
        अञ्ञथा cvt_deltas[idx] += unpacked_deltas[i];
      }
    } याव (iterator.move_to_next ());

    पटिनिवत्त सच्च;
  }
  
  तार्किक serialize (hb_serialize_context_t *c,
                  TupleVariationData<>::tuple_variations_t& tuple_variations) ध्रुव
  {
    TRACE_SERIALIZE (एतत्);
    सचे (!tuple_variations) return_trace (मिच्छा);
    सचे (unlikely (!c->embed (version))) return_trace (मिच्छा);

    return_trace (tupleVariationData.serialize (c, मिच्छा, tuple_variations));
  }

  तार्किक subset (hb_subset_context_t *c) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    सचे (c->plan->all_axes_pinned)
      return_trace (मिच्छा);

    OT::TupleVariationData<>::tuple_variations_t tuple_variations;
    unsigned axis_गणनम् = c->plan->axes_old_index_tag_map.get_population ();

    ध्रुव hb_tag_t cvt = HB_TAG('c','v','t',' ');
    hb_blob_t *cvt_blob = hb_face_reference_table (c->plan->source, cvt);
    unsigned point_गणनम् = hb_blob_get_length (cvt_blob) / FWORD::static_size;
    hb_blob_destroy (cvt_blob);

    सचे (!decompile_tuple_variations (axis_गणनम्, point_गणनम्,
                                     c->source_blob, मिच्छा,
                                     &(c->plan->axes_old_index_tag_map),
                                     tuple_variations))
      return_trace (मिच्छा);

    optimize_scratch_t scratch;
    सचे (!tuple_variations.instantiate (c->plan->axes_location, c->plan->axes_triple_distances, scratch))
      return_trace (मिच्छा);

    सचे (!tuple_variations.compile_bytes (c->plan->axes_index_map, c->plan->axes_old_index_tag_map,
                                         मिच्छा /* do not use shared points */))
      return_trace (मिच्छा);

    return_trace (serialize (c->serializer, tuple_variations));
  }

  स्थिर तार्किक add_cvt_and_apply_deltas (hb_subset_plan_t *plan,
                                        ध्रुव TupleVariationData<> *tuple_var_data,
                                        ध्रुव शून्य *base)
  {
    ध्रुव hb_tag_t cvt = HB_TAG('c','v','t',' ');
    hb_blob_t *cvt_blob = hb_face_reference_table (plan->source, cvt);
    hb_blob_t *cvt_अविभाज्यः_blob = hb_blob_copy_writable_or_fail (cvt_blob);
    hb_blob_destroy (cvt_blob);
  
    सचे (unlikely (!cvt_अविभाज्यः_blob))
      पटिनिवत्त मिच्छा;
 
    unsigned cvt_blob_length = hb_blob_get_length (cvt_अविभाज्यः_blob);
    unsigned अङ्कः_cvt_item = cvt_blob_length / FWORD::static_size;

    hb_vector_t<भिन्न> cvt_deltas;
    सचे (unlikely (!cvt_deltas.resize (अङ्कः_cvt_item)))
    {
      hb_blob_destroy (cvt_अविभाज्यः_blob);
      पटिनिवत्त मिच्छा;
    }

    /* Bound the tuple variation data by the actual cvar table length rather
     * than trusting tupleVarCount to walk the headers; an inflated count would
     * otherwise run get_size() off the end of the table. */
    hb_blob_t *cvar_blob = hb_face_reference_table (plan->source, cvar::tableTag);
    unsigned cvar_length = hb_blob_get_length (cvar_blob);
    hb_blob_destroy (cvar_blob);
    unsigned var_data_offset = (unsigned) ((ध्रुव वर्ण *) tuple_var_data - (ध्रुव वर्ण *) base);
    unsigned var_data_length = var_data_offset < cvar_length ? cvar_length - var_data_offset : 0;

    सचे (!calculate_cvt_deltas (plan->normalized_coords.length, plan->normalized_coords.as_array (),
                               अङ्कः_cvt_item, tuple_var_data, base, var_data_length, cvt_deltas))
    {
      hb_blob_destroy (cvt_अविभाज्यः_blob);
      पटिनिवत्त मिच्छा;
    }

    FWORD *cvt_अविभाज्यः = (FWORD *) hb_blob_get_data_writable (cvt_अविभाज्यः_blob, nullptr);
    अत्थाय (unsigned i = 0; i < अङ्कः_cvt_item; i++)
      cvt_अविभाज्यः[i] += (पूर्णाङ्क) roundf (cvt_deltas[i]);
    
    तार्किक success = plan->add_table (cvt, cvt_अविभाज्यः_blob);
    hb_blob_destroy (cvt_अविभाज्यः_blob);
    पटिनिवत्त success;
  }

  रक्षित:
  FixedVersion<>version;		/* Version of the CVT variation table
					 * initially set to 0x00010000u */
  TupleVariationData<> tupleVariationData; /* TupleVariationDate for cvar table */
  सार्वजनिक:
  DEFINE_SIZE_MIN (8);
};

} /* namespace OT */


#endif /* HB_OT_VAR_CVAR_TABLE_HH */
