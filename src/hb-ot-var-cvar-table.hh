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

#ضمن "hb-ot-var-common.hh"
#ضمن "hb-ot-var-fvar-table.hh"


نطاق OT {
/*
 * cvar -- control value table (CVT) Variations
 * https://docs.microsoft.com/en-us/typography/opentype/spec/cvar
 */
#define HB_OT_TAG_cvar HB_TAG('c','v','a','r')

بنية cvar
{
  ساكن constexpr hb_tag_t tableTag = HB_OT_TAG_cvar;

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  likely (version.major == 1) &&
		  tupleVariationData.sanitize (c));
  }

  ثابت TupleVariationData<>* get_tuple_var_data (فراغ) ثابت
  { إرجاع &tupleVariationData; }

  منطقي decompile_tuple_variations (unsigned axis_تعداد,
                                   unsigned point_تعداد,
                                   hb_blob_t *blob,
                                   منطقي is_gvar,
                                   ثابت hb_map_t *axes_old_index_tag_map,
                                   TupleVariationData<>::tuple_variations_t& tuple_variations /* OUT */) ثابت
  {
    hb_vector_t<unsigned> shared_indices;
    TupleVariationData<>::tuple_iterator_t iterator;
    hb_bytes_t var_data_bytes = blob->as_bytes ().sub_array (4);
    إذا (!TupleVariationData<>::get_tuple_iterator (var_data_bytes, axis_تعداد, هذا,
                                                 shared_indices, &iterator))
      إرجاع خطأ;

    إرجاع tupleVariationData.decompile_tuple_variations (point_تعداد, is_gvar, iterator,
                                                          axes_old_index_tag_map,
                                                          shared_indices,
                                                          hb_array<ثابت F2DOT14> (),
                                                          tuple_variations);
  }

  ساكن منطقي calculate_cvt_deltas (unsigned axis_تعداد,
                                    hb_array_t<صحيح> coords,
                                    unsigned رقم_cvt_item,
                                    ثابت TupleVariationData<> *tuple_var_data,
                                    ثابت فراغ *base,
                                    unsigned var_data_length,
                                    hb_vector_t<عائم>& cvt_deltas /* OUT */)
  {
    إذا (!coords) إرجاع true;
    hb_vector_t<unsigned> shared_indices;
    TupleVariationData<>::tuple_iterator_t iterator;
    hb_bytes_t var_data_bytes = hb_bytes_t (reinterpret_cast<ثابت حرف*> (tuple_var_data), var_data_length);
    إذا (!TupleVariationData<>::get_tuple_iterator (var_data_bytes, axis_تعداد, base,
                                                 shared_indices, &iterator))
      إرجاع true; /* isn't applied at all */

    hb_array_t<ثابت F2DOT14> shared_tuples = hb_array<F2DOT14> ();
    hb_vector_t<unsigned> private_indices;
    hb_vector_t<صحيح> unpacked_deltas;

    افعل
    {
      عائم scalar = iterator.current_tuple->calculate_scalar (coords, axis_تعداد, shared_tuples);
      إذا (scalar == 0.f) استمر;
      ثابت HBUINT8 *p = iterator.get_serialized_data ();
      unsigned صحيح length = iterator.current_tuple->get_data_size ();
      إذا (unlikely (!iterator.var_data_bytes.check_range (p, length)))
        إرجاع خطأ;

      ثابت HBUINT8 *end = p + length;

      منطقي has_private_points = iterator.current_tuple->has_private_points ();
      إذا (has_private_points &&
          !TupleVariationData<>::decompile_points (p, private_indices, end))
        إرجاع خطأ;
      ثابت hb_vector_t<unsigned صحيح> &indices = has_private_points ? private_indices : shared_indices;

      منطقي apply_to_all = (indices.length == 0);
      unsigned رقم_deltas = apply_to_all ? رقم_cvt_item : indices.length;
      إذا (unlikely (!unpacked_deltas.resize_dirty  (رقم_deltas))) إرجاع خطأ;
      إذا (unlikely (!TupleVariationData<>::decompile_deltas (p, unpacked_deltas, end))) إرجاع خطأ;

      لكل (unsigned صحيح i = 0; i < رقم_deltas; i++)
      {
        unsigned صحيح idx = apply_to_all ? i : indices[i];
        إذا (unlikely (idx >= رقم_cvt_item)) استمر;
        إذا (scalar != 1.0f) cvt_deltas[idx] += unpacked_deltas[i] * scalar ;
        وإلا cvt_deltas[idx] += unpacked_deltas[i];
      }
    } بينما (iterator.move_to_next ());

    إرجاع true;
  }
  
  منطقي serialize (hb_serialize_context_t *c,
                  TupleVariationData<>::tuple_variations_t& tuple_variations) ثابت
  {
    TRACE_SERIALIZE (هذا);
    إذا (!tuple_variations) return_trace (خطأ);
    إذا (unlikely (!c->embed (version))) return_trace (خطأ);

    return_trace (tupleVariationData.serialize (c, خطأ, tuple_variations));
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    إذا (c->plan->all_axes_pinned)
      return_trace (خطأ);

    OT::TupleVariationData<>::tuple_variations_t tuple_variations;
    unsigned axis_تعداد = c->plan->axes_old_index_tag_map.get_population ();

    ثابت hb_tag_t cvt = HB_TAG('c','v','t',' ');
    hb_blob_t *cvt_blob = hb_face_reference_table (c->plan->source, cvt);
    unsigned point_تعداد = hb_blob_get_length (cvt_blob) / FWORD::static_size;
    hb_blob_destroy (cvt_blob);

    إذا (!decompile_tuple_variations (axis_تعداد, point_تعداد,
                                     c->source_blob, خطأ,
                                     &(c->plan->axes_old_index_tag_map),
                                     tuple_variations))
      return_trace (خطأ);

    optimize_scratch_t scratch;
    إذا (!tuple_variations.instantiate (c->plan->axes_location, c->plan->axes_triple_distances, scratch))
      return_trace (خطأ);

    إذا (!tuple_variations.compile_bytes (c->plan->axes_index_map, c->plan->axes_old_index_tag_map,
                                         خطأ /* do not use shared points */))
      return_trace (خطأ);

    return_trace (serialize (c->serializer, tuple_variations));
  }

  ساكن منطقي add_cvt_and_apply_deltas (hb_subset_plan_t *plan,
                                        ثابت TupleVariationData<> *tuple_var_data,
                                        ثابت فراغ *base)
  {
    ثابت hb_tag_t cvt = HB_TAG('c','v','t',' ');
    hb_blob_t *cvt_blob = hb_face_reference_table (plan->source, cvt);
    hb_blob_t *cvt_أولي_blob = hb_blob_copy_writable_or_fail (cvt_blob);
    hb_blob_destroy (cvt_blob);
  
    إذا (unlikely (!cvt_أولي_blob))
      إرجاع خطأ;
 
    unsigned cvt_blob_length = hb_blob_get_length (cvt_أولي_blob);
    unsigned رقم_cvt_item = cvt_blob_length / FWORD::static_size;

    hb_vector_t<عائم> cvt_deltas;
    إذا (unlikely (!cvt_deltas.resize (رقم_cvt_item)))
    {
      hb_blob_destroy (cvt_أولي_blob);
      إرجاع خطأ;
    }

    /* Bound the tuple variation data by the actual cvar table length rather
     * than trusting tupleVarCount to walk the headers; an inflated count would
     * otherwise run get_size() off the end of the table. */
    hb_blob_t *cvar_blob = hb_face_reference_table (plan->source, cvar::tableTag);
    unsigned cvar_length = hb_blob_get_length (cvar_blob);
    hb_blob_destroy (cvar_blob);
    unsigned var_data_offset = (unsigned) ((ثابت حرف *) tuple_var_data - (ثابت حرف *) base);
    unsigned var_data_length = var_data_offset < cvar_length ? cvar_length - var_data_offset : 0;

    إذا (!calculate_cvt_deltas (plan->normalized_coords.length, plan->normalized_coords.as_array (),
                               رقم_cvt_item, tuple_var_data, base, var_data_length, cvt_deltas))
    {
      hb_blob_destroy (cvt_أولي_blob);
      إرجاع خطأ;
    }

    FWORD *cvt_أولي = (FWORD *) hb_blob_get_data_writable (cvt_أولي_blob, nullptr);
    لكل (unsigned i = 0; i < رقم_cvt_item; i++)
      cvt_أولي[i] += (صحيح) roundf (cvt_deltas[i]);
    
    منطقي success = plan->add_table (cvt, cvt_أولي_blob);
    hb_blob_destroy (cvt_أولي_blob);
    إرجاع success;
  }

  محمي:
  FixedVersion<>version;		/* Version of the CVT variation table
					 * initially set to 0x00010000u */
  TupleVariationData<> tupleVariationData; /* TupleVariationDate for cvar table */
  عام:
  DEFINE_SIZE_MIN (8);
};

} /* namespace OT */


#endif /* HB_OT_VAR_CVAR_TABLE_HH */
