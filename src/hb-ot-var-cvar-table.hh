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
  { प्रत्यागम &tupleVariationData; }

  तार्किक decompile_tuple_variations (unsigned axis_मोजणी,
                                   unsigned point_मोजणी,
                                   hb_blob_t *blob,
                                   तार्किक is_gvar,
                                   ध्रुव hb_map_t *axes_old_index_tag_map,
                                   TupleVariationData<>::tuple_variations_t& tuple_variations /* OUT */) ध्रुव
  {
    hb_vector_t<unsigned> shared_indices;
    TupleVariationData<>::tuple_iterator_t iterator;
    hb_bytes_t var_data_bytes = blob->as_bytes ().sub_array (4);
    यदि (!TupleVariationData<>::get_tuple_iterator (var_data_bytes, axis_मोजणी, एतत्,
                                                 shared_indices, &iterator))
      प्रत्यागम असत्य;

    प्रत्यागम tupleVariationData.decompile_tuple_variations (point_मोजणी, is_gvar, iterator,
                                                          axes_old_index_tag_map,
                                                          shared_indices,
                                                          hb_array<ध्रुव F2DOT14> (),
                                                          tuple_variations);
  }

  स्थिर तार्किक calculate_cvt_deltas (unsigned axis_मोजणी,
                                    hb_array_t<पूर्णाङ्क> coords,
                                    unsigned अंक_cvt_item,
                                    ध्रुव TupleVariationData<> *tuple_var_data,
                                    ध्रुव शून्य *base,
                                    unsigned var_data_length,
                                    hb_vector_t<भिन्न>& cvt_deltas /* OUT */)
  {
    यदि (!coords) प्रत्यागम सत्य;
    hb_vector_t<unsigned> shared_indices;
    TupleVariationData<>::tuple_iterator_t iterator;
    hb_bytes_t var_data_bytes = hb_bytes_t (reinterpret_cast<ध्रुव वर्ण*> (tuple_var_data), var_data_length);
    यदि (!TupleVariationData<>::get_tuple_iterator (var_data_bytes, axis_मोजणी, base,
                                                 shared_indices, &iterator))
      प्रत्यागम सत्य; /* isn't applied at all */

    hb_array_t<ध्रुव F2DOT14> shared_tuples = hb_array<F2DOT14> ();
    hb_vector_t<unsigned> private_indices;
    hb_vector_t<पूर्णाङ्क> unpacked_deltas;

    कुरु
    {
      भिन्न scalar = iterator.current_tuple->calculate_scalar (coords, axis_मोजणी, shared_tuples);
      यदि (scalar == 0.f) अनुवर्त;
      ध्रुव HBUINT8 *p = iterator.get_serialized_data ();
      unsigned पूर्णाङ्क length = iterator.current_tuple->get_data_size ();
      यदि (unlikely (!iterator.var_data_bytes.check_range (p, length)))
        प्रत्यागम असत्य;

      ध्रुव HBUINT8 *end = p + length;

      तार्किक has_private_points = iterator.current_tuple->has_private_points ();
      यदि (has_private_points &&
          !TupleVariationData<>::decompile_points (p, private_indices, end))
        प्रत्यागम असत्य;
      ध्रुव hb_vector_t<unsigned पूर्णाङ्क> &indices = has_private_points ? private_indices : shared_indices;

      तार्किक apply_to_all = (indices.length == 0);
      unsigned अंक_deltas = apply_to_all ? अंक_cvt_item : indices.length;
      यदि (unlikely (!unpacked_deltas.resize_dirty  (अंक_deltas))) प्रत्यागम असत्य;
      यदि (unlikely (!TupleVariationData<>::decompile_deltas (p, unpacked_deltas, end))) प्रत्यागम असत्य;

      प्रति (unsigned पूर्णाङ्क i = 0; i < अंक_deltas; i++)
      {
        unsigned पूर्णाङ्क idx = apply_to_all ? i : indices[i];
        यदि (unlikely (idx >= अंक_cvt_item)) अनुवर्त;
        यदि (scalar != 1.0f) cvt_deltas[idx] += unpacked_deltas[i] * scalar ;
        अन्यथा cvt_deltas[idx] += unpacked_deltas[i];
      }
    } यावत् (iterator.move_to_next ());

    प्रत्यागम सत्य;
  }
  
  तार्किक serialize (hb_serialize_context_t *c,
                  TupleVariationData<>::tuple_variations_t& tuple_variations) ध्रुव
  {
    TRACE_SERIALIZE (एतत्);
    यदि (!tuple_variations) return_trace (असत्य);
    यदि (unlikely (!c->embed (version))) return_trace (असत्य);

    return_trace (tupleVariationData.serialize (c, असत्य, tuple_variations));
  }

  तार्किक subset (hb_subset_context_t *c) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    यदि (c->plan->all_axes_pinned)
      return_trace (असत्य);

    OT::TupleVariationData<>::tuple_variations_t tuple_variations;
    unsigned axis_मोजणी = c->plan->axes_old_index_tag_map.get_population ();

    ध्रुव hb_tag_t cvt = HB_TAG('c','v','t',' ');
    hb_blob_t *cvt_blob = hb_face_reference_table (c->plan->source, cvt);
    unsigned point_मोजणी = hb_blob_get_length (cvt_blob) / FWORD::static_size;
    hb_blob_destroy (cvt_blob);

    यदि (!decompile_tuple_variations (axis_मोजणी, point_मोजणी,
                                     c->source_blob, असत्य,
                                     &(c->plan->axes_old_index_tag_map),
                                     tuple_variations))
      return_trace (असत्य);

    optimize_scratch_t scratch;
    यदि (!tuple_variations.instantiate (c->plan->axes_location, c->plan->axes_triple_distances, scratch))
      return_trace (असत्य);

    यदि (!tuple_variations.compile_bytes (c->plan->axes_index_map, c->plan->axes_old_index_tag_map,
                                         असत्य /* do not use shared points */))
      return_trace (असत्य);

    return_trace (serialize (c->serializer, tuple_variations));
  }

  स्थिर तार्किक add_cvt_and_apply_deltas (hb_subset_plan_t *plan,
                                        ध्रुव TupleVariationData<> *tuple_var_data,
                                        ध्रुव शून्य *base)
  {
    ध्रुव hb_tag_t cvt = HB_TAG('c','v','t',' ');
    hb_blob_t *cvt_blob = hb_face_reference_table (plan->source, cvt);
    hb_blob_t *cvt_मूळसंख्या_blob = hb_blob_copy_writable_or_fail (cvt_blob);
    hb_blob_destroy (cvt_blob);
  
    यदि (unlikely (!cvt_मूळसंख्या_blob))
      प्रत्यागम असत्य;
 
    unsigned cvt_blob_length = hb_blob_get_length (cvt_मूळसंख्या_blob);
    unsigned अंक_cvt_item = cvt_blob_length / FWORD::static_size;

    hb_vector_t<भिन्न> cvt_deltas;
    यदि (unlikely (!cvt_deltas.resize (अंक_cvt_item)))
    {
      hb_blob_destroy (cvt_मूळसंख्या_blob);
      प्रत्यागम असत्य;
    }

    /* Bound the tuple variation data by the actual cvar table length rather
     * than trusting tupleVarCount to walk the headers; an inflated count would
     * otherwise run get_size() off the end of the table. */
    hb_blob_t *cvar_blob = hb_face_reference_table (plan->source, cvar::tableTag);
    unsigned cvar_length = hb_blob_get_length (cvar_blob);
    hb_blob_destroy (cvar_blob);
    unsigned var_data_offset = (unsigned) ((ध्रुव वर्ण *) tuple_var_data - (ध्रुव वर्ण *) base);
    unsigned var_data_length = var_data_offset < cvar_length ? cvar_length - var_data_offset : 0;

    यदि (!calculate_cvt_deltas (plan->normalized_coords.length, plan->normalized_coords.as_array (),
                               अंक_cvt_item, tuple_var_data, base, var_data_length, cvt_deltas))
    {
      hb_blob_destroy (cvt_मूळसंख्या_blob);
      प्रत्यागम असत्य;
    }

    FWORD *cvt_मूळसंख्या = (FWORD *) hb_blob_get_data_writable (cvt_मूळसंख्या_blob, nullptr);
    प्रति (unsigned i = 0; i < अंक_cvt_item; i++)
      cvt_मूळसंख्या[i] += (पूर्णाङ्क) roundf (cvt_deltas[i]);
    
    तार्किक success = plan->add_table (cvt, cvt_मूळसंख्या_blob);
    hb_blob_destroy (cvt_मूळसंख्या_blob);
    प्रत्यागम success;
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
