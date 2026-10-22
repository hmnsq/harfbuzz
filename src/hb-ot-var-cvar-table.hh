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

  तार्किक sanitize (hb_sanitize_context_t *c) अचर
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) &&
		  hb_barrier () &&
		  likely (version.major == 1) &&
		  tupleVariationData.sanitize (c));
  }

  अचर TupleVariationData<>* get_tuple_var_data (शून्य) अचर
  { फर्काऊ &tupleVariationData; }

  तार्किक decompile_tuple_variations (अचिन्हित axis_गन्ती,
                                   अचिन्हित point_गन्ती,
                                   hb_blob_t *blob,
                                   तार्किक is_gvar,
                                   अचर hb_map_t *axes_old_index_tag_map,
                                   TupleVariationData<>::tuple_variations_t& tuple_variations /* OUT */) अचर
  {
    hb_vector_t<अचिन्हित> shared_indices;
    TupleVariationData<>::tuple_iterator_t iterator;
    hb_bytes_t var_data_bytes = blob->as_bytes ().sub_array (4);
    यदि (!TupleVariationData<>::get_tuple_iterator (var_data_bytes, axis_गन्ती, एतत्,
                                                 shared_indices, &iterator))
      फर्काऊ असत्य;

    फर्काऊ tupleVariationData.decompile_tuple_variations (point_गन्ती, is_gvar, iterator,
                                                          axes_old_index_tag_map,
                                                          shared_indices,
                                                          hb_array<अचर F2DOT14> (),
                                                          tuple_variations);
  }

  स्थिर तार्किक calculate_cvt_deltas (अचिन्हित axis_गन्ती,
                                    hb_array_t<पूर्णाङ्क> coords,
                                    अचिन्हित अङ्क_cvt_item,
                                    अचर TupleVariationData<> *tuple_var_data,
                                    अचर शून्य *base,
                                    अचिन्हित var_data_length,
                                    hb_vector_t<भिन्न>& cvt_deltas /* OUT */)
  {
    यदि (!coords) फर्काऊ सत्य;
    hb_vector_t<अचिन्हित> shared_indices;
    TupleVariationData<>::tuple_iterator_t iterator;
    hb_bytes_t var_data_bytes = hb_bytes_t (reinterpret_cast<अचर वर्ण*> (tuple_var_data), var_data_length);
    यदि (!TupleVariationData<>::get_tuple_iterator (var_data_bytes, axis_गन्ती, base,
                                                 shared_indices, &iterator))
      फर्काऊ सत्य; /* isn't applied at all */

    hb_array_t<अचर F2DOT14> shared_tuples = hb_array<F2DOT14> ();
    hb_vector_t<अचिन्हित> private_indices;
    hb_vector_t<पूर्णाङ्क> unpacked_deltas;

    गर
    {
      भिन्न scalar = iterator.current_tuple->calculate_scalar (coords, axis_गन्ती, shared_tuples);
      यदि (scalar == 0.f) जारी;
      अचर HBUINT8 *p = iterator.get_serialized_data ();
      अचिन्हित पूर्णाङ्क length = iterator.current_tuple->get_data_size ();
      यदि (unlikely (!iterator.var_data_bytes.check_range (p, length)))
        फर्काऊ असत्य;

      अचर HBUINT8 *end = p + length;

      तार्किक has_private_points = iterator.current_tuple->has_private_points ();
      यदि (has_private_points &&
          !TupleVariationData<>::decompile_points (p, private_indices, end))
        फर्काऊ असत्य;
      अचर hb_vector_t<अचिन्हित पूर्णाङ्क> &indices = has_private_points ? private_indices : shared_indices;

      तार्किक apply_to_all = (indices.length == 0);
      अचिन्हित अङ्क_deltas = apply_to_all ? अङ्क_cvt_item : indices.length;
      यदि (unlikely (!unpacked_deltas.resize_dirty  (अङ्क_deltas))) फर्काऊ असत्य;
      यदि (unlikely (!TupleVariationData<>::decompile_deltas (p, unpacked_deltas, end))) फर्काऊ असत्य;

      लागि (अचिन्हित पूर्णाङ्क i = 0; i < अङ्क_deltas; i++)
      {
        अचिन्हित पूर्णाङ्क idx = apply_to_all ? i : indices[i];
        यदि (unlikely (idx >= अङ्क_cvt_item)) जारी;
        यदि (scalar != 1.0f) cvt_deltas[idx] += unpacked_deltas[i] * scalar ;
        अन्यथा cvt_deltas[idx] += unpacked_deltas[i];
      }
    } जबसम्म (iterator.move_to_next ());

    फर्काऊ सत्य;
  }
  
  तार्किक serialize (hb_serialize_context_t *c,
                  TupleVariationData<>::tuple_variations_t& tuple_variations) अचर
  {
    TRACE_SERIALIZE (एतत्);
    यदि (!tuple_variations) return_trace (असत्य);
    यदि (unlikely (!c->embed (version))) return_trace (असत्य);

    return_trace (tupleVariationData.serialize (c, असत्य, tuple_variations));
  }

  तार्किक subset (hb_subset_context_t *c) अचर
  {
    TRACE_SUBSET (एतत्);
    यदि (c->plan->all_axes_pinned)
      return_trace (असत्य);

    OT::TupleVariationData<>::tuple_variations_t tuple_variations;
    अचिन्हित axis_गन्ती = c->plan->axes_old_index_tag_map.get_population ();

    अचर hb_tag_t cvt = HB_TAG('c','v','t',' ');
    hb_blob_t *cvt_blob = hb_face_reference_table (c->plan->source, cvt);
    अचिन्हित point_गन्ती = hb_blob_get_length (cvt_blob) / FWORD::static_size;
    hb_blob_destroy (cvt_blob);

    यदि (!decompile_tuple_variations (axis_गन्ती, point_गन्ती,
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
                                        अचर TupleVariationData<> *tuple_var_data,
                                        अचर शून्य *base)
  {
    अचर hb_tag_t cvt = HB_TAG('c','v','t',' ');
    hb_blob_t *cvt_blob = hb_face_reference_table (plan->source, cvt);
    hb_blob_t *cvt_अविभाज्य_blob = hb_blob_copy_writable_or_fail (cvt_blob);
    hb_blob_destroy (cvt_blob);
  
    यदि (unlikely (!cvt_अविभाज्य_blob))
      फर्काऊ असत्य;
 
    अचिन्हित cvt_blob_length = hb_blob_get_length (cvt_अविभाज्य_blob);
    अचिन्हित अङ्क_cvt_item = cvt_blob_length / FWORD::static_size;

    hb_vector_t<भिन्न> cvt_deltas;
    यदि (unlikely (!cvt_deltas.resize (अङ्क_cvt_item)))
    {
      hb_blob_destroy (cvt_अविभाज्य_blob);
      फर्काऊ असत्य;
    }

    /* Bound the tuple variation data by the actual cvar table length rather
     * than trusting tupleVarCount to walk the headers; an inflated count would
     * otherwise run get_size() off the end of the table. */
    hb_blob_t *cvar_blob = hb_face_reference_table (plan->source, cvar::tableTag);
    अचिन्हित cvar_length = hb_blob_get_length (cvar_blob);
    hb_blob_destroy (cvar_blob);
    अचिन्हित var_data_offset = (अचिन्हित) ((अचर वर्ण *) tuple_var_data - (अचर वर्ण *) base);
    अचिन्हित var_data_length = var_data_offset < cvar_length ? cvar_length - var_data_offset : 0;

    यदि (!calculate_cvt_deltas (plan->normalized_coords.length, plan->normalized_coords.as_array (),
                               अङ्क_cvt_item, tuple_var_data, base, var_data_length, cvt_deltas))
    {
      hb_blob_destroy (cvt_अविभाज्य_blob);
      फर्काऊ असत्य;
    }

    FWORD *cvt_अविभाज्य = (FWORD *) hb_blob_get_data_writable (cvt_अविभाज्य_blob, nullptr);
    लागि (अचिन्हित i = 0; i < अङ्क_cvt_item; i++)
      cvt_अविभाज्य[i] += (पूर्णाङ्क) roundf (cvt_deltas[i]);
    
    तार्किक success = plan->add_table (cvt, cvt_अविभाज्य_blob);
    hb_blob_destroy (cvt_अविभाज्य_blob);
    फर्काऊ success;
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
