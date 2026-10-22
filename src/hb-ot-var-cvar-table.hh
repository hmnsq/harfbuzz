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

#অন্তর্ভাব "hb-ot-var-common.hh"
#অন্তর্ভাব "hb-ot-var-fvar-table.hh"


নামাকাশ OT {
/*
 * cvar -- control value table (CVT) Variations
 * https://docs.microsoft.com/en-us/typography/opentype/spec/cvar
 */
#define HB_OT_TAG_cvar HB_TAG('c','v','a','r')

সংরচনা cvar
{
  স্থির constexpr hb_tag_t tableTag = HB_OT_TAG_cvar;

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্) &&
		  hb_barrier () &&
		  likely (version.major == 1) &&
		  tupleVariationData.sanitize (c));
  }

  ধ্রুব TupleVariationData<>* get_tuple_var_data (শূন্য) ধ্রুব
  { প্রত্যাগম &tupleVariationData; }

  তার্কিক decompile_tuple_variations (unsigned axis_গুনতি,
                                   unsigned point_গুনতি,
                                   hb_blob_t *blob,
                                   তার্কিক is_gvar,
                                   ধ্রুব hb_map_t *axes_old_index_tag_map,
                                   TupleVariationData<>::tuple_variations_t& tuple_variations /* OUT */) ধ্রুব
  {
    hb_vector_t<unsigned> shared_indices;
    TupleVariationData<>::tuple_iterator_t iterator;
    hb_bytes_t var_data_bytes = blob->as_bytes ().sub_array (4);
    যদি (!TupleVariationData<>::get_tuple_iterator (var_data_bytes, axis_গুনতি, এতত্,
                                                 shared_indices, &iterator))
      প্রত্যাগম অসত্য;

    প্রত্যাগম tupleVariationData.decompile_tuple_variations (point_গুনতি, is_gvar, iterator,
                                                          axes_old_index_tag_map,
                                                          shared_indices,
                                                          hb_array<ধ্রুব F2DOT14> (),
                                                          tuple_variations);
  }

  স্থির তার্কিক calculate_cvt_deltas (unsigned axis_গুনতি,
                                    hb_array_t<পূর্ণাঙ্ক> coords,
                                    unsigned অঙ্ক_cvt_item,
                                    ধ্রুব TupleVariationData<> *tuple_var_data,
                                    ধ্রুব শূন্য *base,
                                    unsigned var_data_length,
                                    hb_vector_t<ভিন্ন>& cvt_deltas /* OUT */)
  {
    যদি (!coords) প্রত্যাগম সত্য;
    hb_vector_t<unsigned> shared_indices;
    TupleVariationData<>::tuple_iterator_t iterator;
    hb_bytes_t var_data_bytes = hb_bytes_t (reinterpret_cast<ধ্রুব বর্ণ*> (tuple_var_data), var_data_length);
    যদি (!TupleVariationData<>::get_tuple_iterator (var_data_bytes, axis_গুনতি, base,
                                                 shared_indices, &iterator))
      প্রত্যাগম সত্য; /* isn't applied at all */

    hb_array_t<ধ্রুব F2DOT14> shared_tuples = hb_array<F2DOT14> ();
    hb_vector_t<unsigned> private_indices;
    hb_vector_t<পূর্ণাঙ্ক> unpacked_deltas;

    কুরু
    {
      ভিন্ন scalar = iterator.current_tuple->calculate_scalar (coords, axis_গুনতি, shared_tuples);
      যদি (scalar == 0.f) অনুবর্ত;
      ধ্রুব HBUINT8 *p = iterator.get_serialized_data ();
      unsigned পূর্ণাঙ্ক length = iterator.current_tuple->get_data_size ();
      যদি (unlikely (!iterator.var_data_bytes.check_range (p, length)))
        প্রত্যাগম অসত্য;

      ধ্রুব HBUINT8 *end = p + length;

      তার্কিক has_private_points = iterator.current_tuple->has_private_points ();
      যদি (has_private_points &&
          !TupleVariationData<>::decompile_points (p, private_indices, end))
        প্রত্যাগম অসত্য;
      ধ্রুব hb_vector_t<unsigned পূর্ণাঙ্ক> &indices = has_private_points ? private_indices : shared_indices;

      তার্কিক apply_to_all = (indices.length == 0);
      unsigned অঙ্ক_deltas = apply_to_all ? অঙ্ক_cvt_item : indices.length;
      যদি (unlikely (!unpacked_deltas.resize_dirty  (অঙ্ক_deltas))) প্রত্যাগম অসত্য;
      যদি (unlikely (!TupleVariationData<>::decompile_deltas (p, unpacked_deltas, end))) প্রত্যাগম অসত্য;

      প্রতি (unsigned পূর্ণাঙ্ক i = 0; i < অঙ্ক_deltas; i++)
      {
        unsigned পূর্ণাঙ্ক idx = apply_to_all ? i : indices[i];
        যদি (unlikely (idx >= অঙ্ক_cvt_item)) অনুবর্ত;
        যদি (scalar != 1.0f) cvt_deltas[idx] += unpacked_deltas[i] * scalar ;
        অন্যথা cvt_deltas[idx] += unpacked_deltas[i];
      }
    } যাবত্ (iterator.move_to_next ());

    প্রত্যাগম সত্য;
  }
  
  তার্কিক serialize (hb_serialize_context_t *c,
                  TupleVariationData<>::tuple_variations_t& tuple_variations) ধ্রুব
  {
    TRACE_SERIALIZE (এতত্);
    যদি (!tuple_variations) return_trace (অসত্য);
    যদি (unlikely (!c->embed (version))) return_trace (অসত্য);

    return_trace (tupleVariationData.serialize (c, অসত্য, tuple_variations));
  }

  তার্কিক subset (hb_subset_context_t *c) ধ্রুব
  {
    TRACE_SUBSET (এতত্);
    যদি (c->plan->all_axes_pinned)
      return_trace (অসত্য);

    OT::TupleVariationData<>::tuple_variations_t tuple_variations;
    unsigned axis_গুনতি = c->plan->axes_old_index_tag_map.get_population ();

    ধ্রুব hb_tag_t cvt = HB_TAG('c','v','t',' ');
    hb_blob_t *cvt_blob = hb_face_reference_table (c->plan->source, cvt);
    unsigned point_গুনতি = hb_blob_get_length (cvt_blob) / FWORD::static_size;
    hb_blob_destroy (cvt_blob);

    যদি (!decompile_tuple_variations (axis_গুনতি, point_গুনতি,
                                     c->source_blob, অসত্য,
                                     &(c->plan->axes_old_index_tag_map),
                                     tuple_variations))
      return_trace (অসত্য);

    optimize_scratch_t scratch;
    যদি (!tuple_variations.instantiate (c->plan->axes_location, c->plan->axes_triple_distances, scratch))
      return_trace (অসত্য);

    যদি (!tuple_variations.compile_bytes (c->plan->axes_index_map, c->plan->axes_old_index_tag_map,
                                         অসত্য /* do not use shared points */))
      return_trace (অসত্য);

    return_trace (serialize (c->serializer, tuple_variations));
  }

  স্থির তার্কিক add_cvt_and_apply_deltas (hb_subset_plan_t *plan,
                                        ধ্রুব TupleVariationData<> *tuple_var_data,
                                        ধ্রুব শূন্য *base)
  {
    ধ্রুব hb_tag_t cvt = HB_TAG('c','v','t',' ');
    hb_blob_t *cvt_blob = hb_face_reference_table (plan->source, cvt);
    hb_blob_t *cvt_মৌলিক_blob = hb_blob_copy_writable_or_fail (cvt_blob);
    hb_blob_destroy (cvt_blob);
  
    যদি (unlikely (!cvt_মৌলিক_blob))
      প্রত্যাগম অসত্য;
 
    unsigned cvt_blob_length = hb_blob_get_length (cvt_মৌলিক_blob);
    unsigned অঙ্ক_cvt_item = cvt_blob_length / FWORD::static_size;

    hb_vector_t<ভিন্ন> cvt_deltas;
    যদি (unlikely (!cvt_deltas.resize (অঙ্ক_cvt_item)))
    {
      hb_blob_destroy (cvt_মৌলিক_blob);
      প্রত্যাগম অসত্য;
    }

    /* Bound the tuple variation data by the actual cvar table length rather
     * than trusting tupleVarCount to walk the headers; an inflated count would
     * otherwise run get_size() off the end of the table. */
    hb_blob_t *cvar_blob = hb_face_reference_table (plan->source, cvar::tableTag);
    unsigned cvar_length = hb_blob_get_length (cvar_blob);
    hb_blob_destroy (cvar_blob);
    unsigned var_data_offset = (unsigned) ((ধ্রুব বর্ণ *) tuple_var_data - (ধ্রুব বর্ণ *) base);
    unsigned var_data_length = var_data_offset < cvar_length ? cvar_length - var_data_offset : 0;

    যদি (!calculate_cvt_deltas (plan->normalized_coords.length, plan->normalized_coords.as_array (),
                               অঙ্ক_cvt_item, tuple_var_data, base, var_data_length, cvt_deltas))
    {
      hb_blob_destroy (cvt_মৌলিক_blob);
      প্রত্যাগম অসত্য;
    }

    FWORD *cvt_মৌলিক = (FWORD *) hb_blob_get_data_writable (cvt_মৌলিক_blob, nullptr);
    প্রতি (unsigned i = 0; i < অঙ্ক_cvt_item; i++)
      cvt_মৌলিক[i] += (পূর্ণাঙ্ক) roundf (cvt_deltas[i]);
    
    তার্কিক success = plan->add_table (cvt, cvt_মৌলিক_blob);
    hb_blob_destroy (cvt_মৌলিক_blob);
    প্রত্যাগম success;
  }

  রক্ষিত:
  FixedVersion<>version;		/* Version of the CVT variation table
					 * initially set to 0x00010000u */
  TupleVariationData<> tupleVariationData; /* TupleVariationDate for cvar table */
  সার্বজনিক:
  DEFINE_SIZE_MIN (8);
};

} /* namespace OT */


#endif /* HB_OT_VAR_CVAR_TABLE_HH */
