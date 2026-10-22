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

#அந்தர்பாவ "hb-ot-var-common.hh"
#அந்தர்பாவ "hb-ot-var-fvar-table.hh"


நாமாகாஶ OT {
/*
 * cvar -- control value table (CVT) Variations
 * https://docs.microsoft.com/en-us/typography/opentype/spec/cvar
 */
#define HB_OT_TAG_cvar HB_TAG('c','v','a','r')

கட்டமைப்பு cvar
{
  நிலையான constexpr hb_tag_t tableTag = HB_OT_TAG_cvar;

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது) &&
		  hb_barrier () &&
		  likely (version.major == 1) &&
		  tupleVariationData.sanitize (c));
  }

  மாறிலி TupleVariationData<>* get_tuple_var_data (வெறுமை) மாறிலி
  { திரும்பு &tupleVariationData; }

  தருக்க decompile_tuple_variations (unsigned axis_மொத்தம்,
                                   unsigned point_மொத்தம்,
                                   hb_blob_t *blob,
                                   தருக்க is_gvar,
                                   மாறிலி hb_map_t *axes_old_index_tag_map,
                                   TupleVariationData<>::tuple_variations_t& tuple_variations /* OUT */) மாறிலி
  {
    hb_vector_t<unsigned> shared_indices;
    TupleVariationData<>::tuple_iterator_t iterator;
    hb_bytes_t var_data_bytes = blob->as_bytes ().sub_array (4);
    எனில் (!TupleVariationData<>::get_tuple_iterator (var_data_bytes, axis_மொத்தம், இது,
                                                 shared_indices, &iterator))
      திரும்பு பொய்;

    திரும்பு tupleVariationData.decompile_tuple_variations (point_மொத்தம், is_gvar, iterator,
                                                          axes_old_index_tag_map,
                                                          shared_indices,
                                                          hb_array<மாறிலி F2DOT14> (),
                                                          tuple_variations);
  }

  நிலையான தருக்க calculate_cvt_deltas (unsigned axis_மொத்தம்,
                                    hb_array_t<முழு> coords,
                                    unsigned இலக்கம்_cvt_item,
                                    மாறிலி TupleVariationData<> *tuple_var_data,
                                    மாறிலி வெறுமை *base,
                                    unsigned var_data_length,
                                    hb_vector_t<தசம>& cvt_deltas /* OUT */)
  {
    எனில் (!coords) திரும்பு உண்மை;
    hb_vector_t<unsigned> shared_indices;
    TupleVariationData<>::tuple_iterator_t iterator;
    hb_bytes_t var_data_bytes = hb_bytes_t (reinterpret_cast<மாறிலி எழுத்து*> (tuple_var_data), var_data_length);
    எனில் (!TupleVariationData<>::get_tuple_iterator (var_data_bytes, axis_மொத்தம், base,
                                                 shared_indices, &iterator))
      திரும்பு உண்மை; /* isn't applied at all */

    hb_array_t<மாறிலி F2DOT14> shared_tuples = hb_array<F2DOT14> ();
    hb_vector_t<unsigned> private_indices;
    hb_vector_t<முழு> unpacked_deltas;

    செய்
    {
      தசம scalar = iterator.current_tuple->calculate_scalar (coords, axis_மொத்தம், shared_tuples);
      எனில் (scalar == 0.f) தொடர்;
      மாறிலி HBUINT8 *p = iterator.get_serialized_data ();
      unsigned முழு length = iterator.current_tuple->get_data_size ();
      எனில் (unlikely (!iterator.var_data_bytes.check_range (p, length)))
        திரும்பு பொய்;

      மாறிலி HBUINT8 *end = p + length;

      தருக்க has_private_points = iterator.current_tuple->has_private_points ();
      எனில் (has_private_points &&
          !TupleVariationData<>::decompile_points (p, private_indices, end))
        திரும்பு பொய்;
      மாறிலி hb_vector_t<unsigned முழு> &indices = has_private_points ? private_indices : shared_indices;

      தருக்க apply_to_all = (indices.length == 0);
      unsigned இலக்கம்_deltas = apply_to_all ? இலக்கம்_cvt_item : indices.length;
      எனில் (unlikely (!unpacked_deltas.resize_dirty  (இலக்கம்_deltas))) திரும்பு பொய்;
      எனில் (unlikely (!TupleVariationData<>::decompile_deltas (p, unpacked_deltas, end))) திரும்பு பொய்;

      க்கு (unsigned முழு i = 0; i < இலக்கம்_deltas; i++)
      {
        unsigned முழு idx = apply_to_all ? i : indices[i];
        எனில் (unlikely (idx >= இலக்கம்_cvt_item)) தொடர்;
        எனில் (scalar != 1.0f) cvt_deltas[idx] += unpacked_deltas[i] * scalar ;
        இல்லையெனில் cvt_deltas[idx] += unpacked_deltas[i];
      }
    } போது (iterator.move_to_next ());

    திரும்பு உண்மை;
  }
  
  தருக்க serialize (hb_serialize_context_t *c,
                  TupleVariationData<>::tuple_variations_t& tuple_variations) மாறிலி
  {
    TRACE_SERIALIZE (இது);
    எனில் (!tuple_variations) return_trace (பொய்);
    எனில் (unlikely (!c->embed (version))) return_trace (பொய்);

    return_trace (tupleVariationData.serialize (c, பொய், tuple_variations));
  }

  தருக்க subset (hb_subset_context_t *c) மாறிலி
  {
    TRACE_SUBSET (இது);
    எனில் (c->plan->all_axes_pinned)
      return_trace (பொய்);

    OT::TupleVariationData<>::tuple_variations_t tuple_variations;
    unsigned axis_மொத்தம் = c->plan->axes_old_index_tag_map.get_population ();

    மாறிலி hb_tag_t cvt = HB_TAG('c','v','t',' ');
    hb_blob_t *cvt_blob = hb_face_reference_table (c->plan->source, cvt);
    unsigned point_மொத்தம் = hb_blob_get_length (cvt_blob) / FWORD::static_size;
    hb_blob_destroy (cvt_blob);

    எனில் (!decompile_tuple_variations (axis_மொத்தம், point_மொத்தம்,
                                     c->source_blob, பொய்,
                                     &(c->plan->axes_old_index_tag_map),
                                     tuple_variations))
      return_trace (பொய்);

    optimize_scratch_t scratch;
    எனில் (!tuple_variations.instantiate (c->plan->axes_location, c->plan->axes_triple_distances, scratch))
      return_trace (பொய்);

    எனில் (!tuple_variations.compile_bytes (c->plan->axes_index_map, c->plan->axes_old_index_tag_map,
                                         பொய் /* do not use shared points */))
      return_trace (பொய்);

    return_trace (serialize (c->serializer, tuple_variations));
  }

  நிலையான தருக்க add_cvt_and_apply_deltas (hb_subset_plan_t *plan,
                                        மாறிலி TupleVariationData<> *tuple_var_data,
                                        மாறிலி வெறுமை *base)
  {
    மாறிலி hb_tag_t cvt = HB_TAG('c','v','t',' ');
    hb_blob_t *cvt_blob = hb_face_reference_table (plan->source, cvt);
    hb_blob_t *cvt_பகாஎண்_blob = hb_blob_copy_writable_or_fail (cvt_blob);
    hb_blob_destroy (cvt_blob);
  
    எனில் (unlikely (!cvt_பகாஎண்_blob))
      திரும்பு பொய்;
 
    unsigned cvt_blob_length = hb_blob_get_length (cvt_பகாஎண்_blob);
    unsigned இலக்கம்_cvt_item = cvt_blob_length / FWORD::static_size;

    hb_vector_t<தசம> cvt_deltas;
    எனில் (unlikely (!cvt_deltas.resize (இலக்கம்_cvt_item)))
    {
      hb_blob_destroy (cvt_பகாஎண்_blob);
      திரும்பு பொய்;
    }

    /* Bound the tuple variation data by the actual cvar table length rather
     * than trusting tupleVarCount to walk the headers; an inflated count would
     * otherwise run get_size() off the end of the table. */
    hb_blob_t *cvar_blob = hb_face_reference_table (plan->source, cvar::tableTag);
    unsigned cvar_length = hb_blob_get_length (cvar_blob);
    hb_blob_destroy (cvar_blob);
    unsigned var_data_offset = (unsigned) ((மாறிலி எழுத்து *) tuple_var_data - (மாறிலி எழுத்து *) base);
    unsigned var_data_length = var_data_offset < cvar_length ? cvar_length - var_data_offset : 0;

    எனில் (!calculate_cvt_deltas (plan->normalized_coords.length, plan->normalized_coords.as_array (),
                               இலக்கம்_cvt_item, tuple_var_data, base, var_data_length, cvt_deltas))
    {
      hb_blob_destroy (cvt_பகாஎண்_blob);
      திரும்பு பொய்;
    }

    FWORD *cvt_பகாஎண் = (FWORD *) hb_blob_get_data_writable (cvt_பகாஎண்_blob, nullptr);
    க்கு (unsigned i = 0; i < இலக்கம்_cvt_item; i++)
      cvt_பகாஎண்[i] += (முழு) roundf (cvt_deltas[i]);
    
    தருக்க success = plan->add_table (cvt, cvt_பகாஎண்_blob);
    hb_blob_destroy (cvt_பகாஎண்_blob);
    திரும்பு success;
  }

  பாதுகாக்கப்பட்ட:
  FixedVersion<>version;		/* Version of the CVT variation table
					 * initially set to 0x00010000u */
  TupleVariationData<> tupleVariationData; /* TupleVariationDate for cvar table */
  பொது:
  DEFINE_SIZE_MIN (8);
};

} /* namespace OT */


#endif /* HB_OT_VAR_CVAR_TABLE_HH */
