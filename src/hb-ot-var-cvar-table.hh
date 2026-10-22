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

#ଅନ୍ତର୍ଭାଵ "hb-ot-var-common.hh"
#ଅନ୍ତର୍ଭାଵ "hb-ot-var-fvar-table.hh"


ନାମାକାଶ OT {
/*
 * cvar -- control value table (CVT) Variations
 * https://docs.microsoft.com/en-us/typography/opentype/spec/cvar
 */
#define HB_OT_TAG_cvar HB_TAG('c','v','a','r')

ସଂରଚନା cvar
{
  ସ୍ଥିର constexpr hb_tag_t tableTag = HB_OT_TAG_cvar;

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି) &&
		  hb_barrier () &&
		  likely (version.major == 1) &&
		  tupleVariationData.sanitize (c));
  }

  ଅଚଳ TupleVariationData<>* get_tuple_var_data (ଶୂନ୍ୟ) ଅଚଳ
  { ଫେରନ୍ତୁ &tupleVariationData; }

  ତାର୍କିକ decompile_tuple_variations (unsigned axis_ଗଣନା,
                                   unsigned point_ଗଣନା,
                                   hb_blob_t *blob,
                                   ତାର୍କିକ is_gvar,
                                   ଅଚଳ hb_map_t *axes_old_index_tag_map,
                                   TupleVariationData<>::tuple_variations_t& tuple_variations /* OUT */) ଅଚଳ
  {
    hb_vector_t<unsigned> shared_indices;
    TupleVariationData<>::tuple_iterator_t iterator;
    hb_bytes_t var_data_bytes = blob->as_bytes ().sub_array (4);
    ଯଦି (!TupleVariationData<>::get_tuple_iterator (var_data_bytes, axis_ଗଣନା, ଏହି,
                                                 shared_indices, &iterator))
      ଫେରନ୍ତୁ ମିଥ୍ୟା;

    ଫେରନ୍ତୁ tupleVariationData.decompile_tuple_variations (point_ଗଣନା, is_gvar, iterator,
                                                          axes_old_index_tag_map,
                                                          shared_indices,
                                                          hb_array<ଅଚଳ F2DOT14> (),
                                                          tuple_variations);
  }

  ସ୍ଥିର ତାର୍କିକ calculate_cvt_deltas (unsigned axis_ଗଣନା,
                                    hb_array_t<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> coords,
                                    unsigned ଅଙ୍କ_cvt_item,
                                    ଅଚଳ TupleVariationData<> *tuple_var_data,
                                    ଅଚଳ ଶୂନ୍ୟ *base,
                                    unsigned var_data_length,
                                    hb_vector_t<ଦଶମିକ>& cvt_deltas /* OUT */)
  {
    ଯଦି (!coords) ଫେରନ୍ତୁ ସତ୍ୟ;
    hb_vector_t<unsigned> shared_indices;
    TupleVariationData<>::tuple_iterator_t iterator;
    hb_bytes_t var_data_bytes = hb_bytes_t (reinterpret_cast<ଅଚଳ ଅକ୍ଷର*> (tuple_var_data), var_data_length);
    ଯଦି (!TupleVariationData<>::get_tuple_iterator (var_data_bytes, axis_ଗଣନା, base,
                                                 shared_indices, &iterator))
      ଫେରନ୍ତୁ ସତ୍ୟ; /* isn't applied at all */

    hb_array_t<ଅଚଳ F2DOT14> shared_tuples = hb_array<F2DOT14> ();
    hb_vector_t<unsigned> private_indices;
    hb_vector_t<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> unpacked_deltas;

    କର
    {
      ଦଶମିକ scalar = iterator.current_tuple->calculate_scalar (coords, axis_ଗଣନା, shared_tuples);
      ଯଦି (scalar == 0.f) ଜାରି;
      ଅଚଳ HBUINT8 *p = iterator.get_serialized_data ();
      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା length = iterator.current_tuple->get_data_size ();
      ଯଦି (unlikely (!iterator.var_data_bytes.check_range (p, length)))
        ଫେରନ୍ତୁ ମିଥ୍ୟା;

      ଅଚଳ HBUINT8 *end = p + length;

      ତାର୍କିକ has_private_points = iterator.current_tuple->has_private_points ();
      ଯଦି (has_private_points &&
          !TupleVariationData<>::decompile_points (p, private_indices, end))
        ଫେରନ୍ତୁ ମିଥ୍ୟା;
      ଅଚଳ hb_vector_t<unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା> &indices = has_private_points ? private_indices : shared_indices;

      ତାର୍କିକ apply_to_all = (indices.length == 0);
      unsigned ଅଙ୍କ_deltas = apply_to_all ? ଅଙ୍କ_cvt_item : indices.length;
      ଯଦି (unlikely (!unpacked_deltas.resize_dirty  (ଅଙ୍କ_deltas))) ଫେରନ୍ତୁ ମିଥ୍ୟା;
      ଯଦି (unlikely (!TupleVariationData<>::decompile_deltas (p, unpacked_deltas, end))) ଫେରନ୍ତୁ ମିଥ୍ୟା;

      ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଅଙ୍କ_deltas; i++)
      {
        unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା idx = apply_to_all ? i : indices[i];
        ଯଦି (unlikely (idx >= ଅଙ୍କ_cvt_item)) ଜାରି;
        ଯଦି (scalar != 1.0f) cvt_deltas[idx] += unpacked_deltas[i] * scalar ;
        ଅନ୍ୟଥା cvt_deltas[idx] += unpacked_deltas[i];
      }
    } ଯେତେବେଳେ (iterator.move_to_next ());

    ଫେରନ୍ତୁ ସତ୍ୟ;
  }
  
  ତାର୍କିକ serialize (hb_serialize_context_t *c,
                  TupleVariationData<>::tuple_variations_t& tuple_variations) ଅଚଳ
  {
    TRACE_SERIALIZE (ଏହି);
    ଯଦି (!tuple_variations) return_trace (ମିଥ୍ୟା);
    ଯଦି (unlikely (!c->embed (version))) return_trace (ମିଥ୍ୟା);

    return_trace (tupleVariationData.serialize (c, ମିଥ୍ୟା, tuple_variations));
  }

  ତାର୍କିକ subset (hb_subset_context_t *c) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    ଯଦି (c->plan->all_axes_pinned)
      return_trace (ମିଥ୍ୟା);

    OT::TupleVariationData<>::tuple_variations_t tuple_variations;
    unsigned axis_ଗଣନା = c->plan->axes_old_index_tag_map.get_population ();

    ଅଚଳ hb_tag_t cvt = HB_TAG('c','v','t',' ');
    hb_blob_t *cvt_blob = hb_face_reference_table (c->plan->source, cvt);
    unsigned point_ଗଣନା = hb_blob_get_length (cvt_blob) / FWORD::static_size;
    hb_blob_destroy (cvt_blob);

    ଯଦି (!decompile_tuple_variations (axis_ଗଣନା, point_ଗଣନା,
                                     c->source_blob, ମିଥ୍ୟା,
                                     &(c->plan->axes_old_index_tag_map),
                                     tuple_variations))
      return_trace (ମିଥ୍ୟା);

    optimize_scratch_t scratch;
    ଯଦି (!tuple_variations.instantiate (c->plan->axes_location, c->plan->axes_triple_distances, scratch))
      return_trace (ମିଥ୍ୟା);

    ଯଦି (!tuple_variations.compile_bytes (c->plan->axes_index_map, c->plan->axes_old_index_tag_map,
                                         ମିଥ୍ୟା /* do not use shared points */))
      return_trace (ମିଥ୍ୟା);

    return_trace (serialize (c->serializer, tuple_variations));
  }

  ସ୍ଥିର ତାର୍କିକ add_cvt_and_apply_deltas (hb_subset_plan_t *plan,
                                        ଅଚଳ TupleVariationData<> *tuple_var_data,
                                        ଅଚଳ ଶୂନ୍ୟ *base)
  {
    ଅଚଳ hb_tag_t cvt = HB_TAG('c','v','t',' ');
    hb_blob_t *cvt_blob = hb_face_reference_table (plan->source, cvt);
    hb_blob_t *cvt_ମୌଳିକ_blob = hb_blob_copy_writable_or_fail (cvt_blob);
    hb_blob_destroy (cvt_blob);
  
    ଯଦି (unlikely (!cvt_ମୌଳିକ_blob))
      ଫେରନ୍ତୁ ମିଥ୍ୟା;
 
    unsigned cvt_blob_length = hb_blob_get_length (cvt_ମୌଳିକ_blob);
    unsigned ଅଙ୍କ_cvt_item = cvt_blob_length / FWORD::static_size;

    hb_vector_t<ଦଶମିକ> cvt_deltas;
    ଯଦି (unlikely (!cvt_deltas.resize (ଅଙ୍କ_cvt_item)))
    {
      hb_blob_destroy (cvt_ମୌଳିକ_blob);
      ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }

    /* Bound the tuple variation data by the actual cvar table length rather
     * than trusting tupleVarCount to walk the headers; an inflated count would
     * otherwise run get_size() off the end of the table. */
    hb_blob_t *cvar_blob = hb_face_reference_table (plan->source, cvar::tableTag);
    unsigned cvar_length = hb_blob_get_length (cvar_blob);
    hb_blob_destroy (cvar_blob);
    unsigned var_data_offset = (unsigned) ((ଅଚଳ ଅକ୍ଷର *) tuple_var_data - (ଅଚଳ ଅକ୍ଷର *) base);
    unsigned var_data_length = var_data_offset < cvar_length ? cvar_length - var_data_offset : 0;

    ଯଦି (!calculate_cvt_deltas (plan->normalized_coords.length, plan->normalized_coords.as_array (),
                               ଅଙ୍କ_cvt_item, tuple_var_data, base, var_data_length, cvt_deltas))
    {
      hb_blob_destroy (cvt_ମୌଳିକ_blob);
      ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }

    FWORD *cvt_ମୌଳିକ = (FWORD *) hb_blob_get_data_writable (cvt_ମୌଳିକ_blob, nullptr);
    ପାଇଁ (unsigned i = 0; i < ଅଙ୍କ_cvt_item; i++)
      cvt_ମୌଳିକ[i] += (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) roundf (cvt_deltas[i]);
    
    ତାର୍କିକ success = plan->add_table (cvt, cvt_ମୌଳିକ_blob);
    hb_blob_destroy (cvt_ମୌଳିକ_blob);
    ଫେରନ୍ତୁ success;
  }

  ସୁରକ୍ଷିତ:
  FixedVersion<>version;		/* Version of the CVT variation table
					 * initially set to 0x00010000u */
  TupleVariationData<> tupleVariationData; /* TupleVariationDate for cvar table */
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_MIN (8);
};

} /* namespace OT */


#endif /* HB_OT_VAR_CVAR_TABLE_HH */
