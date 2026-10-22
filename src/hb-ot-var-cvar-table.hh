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

#અન્તર્ભાવ "hb-ot-var-common.hh"
#અન્તર્ભાવ "hb-ot-var-fvar-table.hh"


નામાકાશ OT {
/*
 * cvar -- control value table (CVT) Variations
 * https://docs.microsoft.com/en-us/typography/opentype/spec/cvar
 */
#define HB_OT_TAG_cvar HB_TAG('c','v','a','r')

સંરચના cvar
{
  સ્થિર constexpr hb_tag_t tableTag = HB_OT_TAG_cvar;

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ) &&
		  hb_barrier () &&
		  likely (version.major == 1) &&
		  tupleVariationData.sanitize (c));
  }

  અચલ TupleVariationData<>* get_tuple_var_data (શૂન્ય) અચલ
  { પાછા &tupleVariationData; }

  તાર્કિક decompile_tuple_variations (unsigned axis_ગણતરી,
                                   unsigned point_ગણતરી,
                                   hb_blob_t *blob,
                                   તાર્કિક is_gvar,
                                   અચલ hb_map_t *axes_old_index_tag_map,
                                   TupleVariationData<>::tuple_variations_t& tuple_variations /* OUT */) અચલ
  {
    hb_vector_t<unsigned> shared_indices;
    TupleVariationData<>::tuple_iterator_t iterator;
    hb_bytes_t var_data_bytes = blob->as_bytes ().sub_array (4);
    જો (!TupleVariationData<>::get_tuple_iterator (var_data_bytes, axis_ગણતરી, આ,
                                                 shared_indices, &iterator))
      પાછા ખોટું;

    પાછા tupleVariationData.decompile_tuple_variations (point_ગણતરી, is_gvar, iterator,
                                                          axes_old_index_tag_map,
                                                          shared_indices,
                                                          hb_array<અચલ F2DOT14> (),
                                                          tuple_variations);
  }

  સ્થિર તાર્કિક calculate_cvt_deltas (unsigned axis_ગણતરી,
                                    hb_array_t<પૂર્ણાંક> coords,
                                    unsigned અંક_cvt_item,
                                    અચલ TupleVariationData<> *tuple_var_data,
                                    અચલ શૂન્ય *base,
                                    unsigned var_data_length,
                                    hb_vector_t<દશાંશ>& cvt_deltas /* OUT */)
  {
    જો (!coords) પાછા સાચું;
    hb_vector_t<unsigned> shared_indices;
    TupleVariationData<>::tuple_iterator_t iterator;
    hb_bytes_t var_data_bytes = hb_bytes_t (reinterpret_cast<અચલ અક્ષર*> (tuple_var_data), var_data_length);
    જો (!TupleVariationData<>::get_tuple_iterator (var_data_bytes, axis_ગણતરી, base,
                                                 shared_indices, &iterator))
      પાછા સાચું; /* isn't applied at all */

    hb_array_t<અચલ F2DOT14> shared_tuples = hb_array<F2DOT14> ();
    hb_vector_t<unsigned> private_indices;
    hb_vector_t<પૂર્ણાંક> unpacked_deltas;

    કરો
    {
      દશાંશ scalar = iterator.current_tuple->calculate_scalar (coords, axis_ગણતરી, shared_tuples);
      જો (scalar == 0.f) ચાલુ;
      અચલ HBUINT8 *p = iterator.get_serialized_data ();
      unsigned પૂર્ણાંક length = iterator.current_tuple->get_data_size ();
      જો (unlikely (!iterator.var_data_bytes.check_range (p, length)))
        પાછા ખોટું;

      અચલ HBUINT8 *end = p + length;

      તાર્કિક has_private_points = iterator.current_tuple->has_private_points ();
      જો (has_private_points &&
          !TupleVariationData<>::decompile_points (p, private_indices, end))
        પાછા ખોટું;
      અચલ hb_vector_t<unsigned પૂર્ણાંક> &indices = has_private_points ? private_indices : shared_indices;

      તાર્કિક apply_to_all = (indices.length == 0);
      unsigned અંક_deltas = apply_to_all ? અંક_cvt_item : indices.length;
      જો (unlikely (!unpacked_deltas.resize_dirty  (અંક_deltas))) પાછા ખોટું;
      જો (unlikely (!TupleVariationData<>::decompile_deltas (p, unpacked_deltas, end))) પાછા ખોટું;

      માટે (unsigned પૂર્ણાંક i = 0; i < અંક_deltas; i++)
      {
        unsigned પૂર્ણાંક idx = apply_to_all ? i : indices[i];
        જો (unlikely (idx >= અંક_cvt_item)) ચાલુ;
        જો (scalar != 1.0f) cvt_deltas[idx] += unpacked_deltas[i] * scalar ;
        નહીંતર cvt_deltas[idx] += unpacked_deltas[i];
      }
    } જ્યારે (iterator.move_to_next ());

    પાછા સાચું;
  }
  
  તાર્કિક serialize (hb_serialize_context_t *c,
                  TupleVariationData<>::tuple_variations_t& tuple_variations) અચલ
  {
    TRACE_SERIALIZE (આ);
    જો (!tuple_variations) return_trace (ખોટું);
    જો (unlikely (!c->embed (version))) return_trace (ખોટું);

    return_trace (tupleVariationData.serialize (c, ખોટું, tuple_variations));
  }

  તાર્કિક subset (hb_subset_context_t *c) અચલ
  {
    TRACE_SUBSET (આ);
    જો (c->plan->all_axes_pinned)
      return_trace (ખોટું);

    OT::TupleVariationData<>::tuple_variations_t tuple_variations;
    unsigned axis_ગણતરી = c->plan->axes_old_index_tag_map.get_population ();

    અચલ hb_tag_t cvt = HB_TAG('c','v','t',' ');
    hb_blob_t *cvt_blob = hb_face_reference_table (c->plan->source, cvt);
    unsigned point_ગણતરી = hb_blob_get_length (cvt_blob) / FWORD::static_size;
    hb_blob_destroy (cvt_blob);

    જો (!decompile_tuple_variations (axis_ગણતરી, point_ગણતરી,
                                     c->source_blob, ખોટું,
                                     &(c->plan->axes_old_index_tag_map),
                                     tuple_variations))
      return_trace (ખોટું);

    optimize_scratch_t scratch;
    જો (!tuple_variations.instantiate (c->plan->axes_location, c->plan->axes_triple_distances, scratch))
      return_trace (ખોટું);

    જો (!tuple_variations.compile_bytes (c->plan->axes_index_map, c->plan->axes_old_index_tag_map,
                                         ખોટું /* do not use shared points */))
      return_trace (ખોટું);

    return_trace (serialize (c->serializer, tuple_variations));
  }

  સ્થિર તાર્કિક add_cvt_and_apply_deltas (hb_subset_plan_t *plan,
                                        અચલ TupleVariationData<> *tuple_var_data,
                                        અચલ શૂન્ય *base)
  {
    અચલ hb_tag_t cvt = HB_TAG('c','v','t',' ');
    hb_blob_t *cvt_blob = hb_face_reference_table (plan->source, cvt);
    hb_blob_t *cvt_અવિભાજ્ય_blob = hb_blob_copy_writable_or_fail (cvt_blob);
    hb_blob_destroy (cvt_blob);
  
    જો (unlikely (!cvt_અવિભાજ્ય_blob))
      પાછા ખોટું;
 
    unsigned cvt_blob_length = hb_blob_get_length (cvt_અવિભાજ્ય_blob);
    unsigned અંક_cvt_item = cvt_blob_length / FWORD::static_size;

    hb_vector_t<દશાંશ> cvt_deltas;
    જો (unlikely (!cvt_deltas.resize (અંક_cvt_item)))
    {
      hb_blob_destroy (cvt_અવિભાજ્ય_blob);
      પાછા ખોટું;
    }

    /* Bound the tuple variation data by the actual cvar table length rather
     * than trusting tupleVarCount to walk the headers; an inflated count would
     * otherwise run get_size() off the end of the table. */
    hb_blob_t *cvar_blob = hb_face_reference_table (plan->source, cvar::tableTag);
    unsigned cvar_length = hb_blob_get_length (cvar_blob);
    hb_blob_destroy (cvar_blob);
    unsigned var_data_offset = (unsigned) ((અચલ અક્ષર *) tuple_var_data - (અચલ અક્ષર *) base);
    unsigned var_data_length = var_data_offset < cvar_length ? cvar_length - var_data_offset : 0;

    જો (!calculate_cvt_deltas (plan->normalized_coords.length, plan->normalized_coords.as_array (),
                               અંક_cvt_item, tuple_var_data, base, var_data_length, cvt_deltas))
    {
      hb_blob_destroy (cvt_અવિભાજ્ય_blob);
      પાછા ખોટું;
    }

    FWORD *cvt_અવિભાજ્ય = (FWORD *) hb_blob_get_data_writable (cvt_અવિભાજ્ય_blob, nullptr);
    માટે (unsigned i = 0; i < અંક_cvt_item; i++)
      cvt_અવિભાજ્ય[i] += (પૂર્ણાંક) roundf (cvt_deltas[i]);
    
    તાર્કિક success = plan->add_table (cvt, cvt_અવિભાજ્ય_blob);
    hb_blob_destroy (cvt_અવિભાજ્ય_blob);
    પાછા success;
  }

  સુરક્ષિત:
  FixedVersion<>version;		/* Version of the CVT variation table
					 * initially set to 0x00010000u */
  TupleVariationData<> tupleVariationData; /* TupleVariationDate for cvar table */
  જાહેર:
  DEFINE_SIZE_MIN (8);
};

} /* namespace OT */


#endif /* HB_OT_VAR_CVAR_TABLE_HH */
