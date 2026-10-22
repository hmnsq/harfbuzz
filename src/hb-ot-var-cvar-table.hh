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

#അന്തര്ഭാവ "hb-ot-var-common.hh"
#അന്തര്ഭാവ "hb-ot-var-fvar-table.hh"


നാമാകാശ OT {
/*
 * cvar -- control value table (CVT) Variations
 * https://docs.microsoft.com/en-us/typography/opentype/spec/cvar
 */
#define HB_OT_TAG_cvar HB_TAG('c','v','a','r')

ഘടന cvar
{
  സ്ഥിതം constexpr hb_tag_t tableTag = HB_OT_TAG_cvar;

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_struct (ഇത്) &&
		  hb_barrier () &&
		  likely (version.major == 1) &&
		  tupleVariationData.sanitize (c));
  }

  സ്ഥിരം TupleVariationData<>* get_tuple_var_data (ശൂന്യം) സ്ഥിരം
  { തിരികെ &tupleVariationData; }

  തർക്കം decompile_tuple_variations (unsigned axis_എണ്ണം,
                                   unsigned point_എണ്ണം,
                                   hb_blob_t *blob,
                                   തർക്കം is_gvar,
                                   സ്ഥിരം hb_map_t *axes_old_index_tag_map,
                                   TupleVariationData<>::tuple_variations_t& tuple_variations /* OUT */) സ്ഥിരം
  {
    hb_vector_t<unsigned> shared_indices;
    TupleVariationData<>::tuple_iterator_t iterator;
    hb_bytes_t var_data_bytes = blob->as_bytes ().sub_array (4);
    എങ്കിൽ (!TupleVariationData<>::get_tuple_iterator (var_data_bytes, axis_എണ്ണം, ഇത്,
                                                 shared_indices, &iterator))
      തിരികെ അസത്യം;

    തിരികെ tupleVariationData.decompile_tuple_variations (point_എണ്ണം, is_gvar, iterator,
                                                          axes_old_index_tag_map,
                                                          shared_indices,
                                                          hb_array<സ്ഥിരം F2DOT14> (),
                                                          tuple_variations);
  }

  സ്ഥിതം തർക്കം calculate_cvt_deltas (unsigned axis_എണ്ണം,
                                    hb_array_t<പൂർണ്ണസംഖ്യ> coords,
                                    unsigned അക്കം_cvt_item,
                                    സ്ഥിരം TupleVariationData<> *tuple_var_data,
                                    സ്ഥിരം ശൂന്യം *base,
                                    unsigned var_data_length,
                                    hb_vector_t<ദശാംശം>& cvt_deltas /* OUT */)
  {
    എങ്കിൽ (!coords) തിരികെ സത്യം;
    hb_vector_t<unsigned> shared_indices;
    TupleVariationData<>::tuple_iterator_t iterator;
    hb_bytes_t var_data_bytes = hb_bytes_t (reinterpret_cast<സ്ഥിരം അക്ഷരം*> (tuple_var_data), var_data_length);
    എങ്കിൽ (!TupleVariationData<>::get_tuple_iterator (var_data_bytes, axis_എണ്ണം, base,
                                                 shared_indices, &iterator))
      തിരികെ സത്യം; /* isn't applied at all */

    hb_array_t<സ്ഥിരം F2DOT14> shared_tuples = hb_array<F2DOT14> ();
    hb_vector_t<unsigned> private_indices;
    hb_vector_t<പൂർണ്ണസംഖ്യ> unpacked_deltas;

    ചെയ്യുക
    {
      ദശാംശം scalar = iterator.current_tuple->calculate_scalar (coords, axis_എണ്ണം, shared_tuples);
      എങ്കിൽ (scalar == 0.f) തുടരുക;
      സ്ഥിരം HBUINT8 *p = iterator.get_serialized_data ();
      unsigned പൂർണ്ണസംഖ്യ length = iterator.current_tuple->get_data_size ();
      എങ്കിൽ (unlikely (!iterator.var_data_bytes.check_range (p, length)))
        തിരികെ അസത്യം;

      സ്ഥിരം HBUINT8 *end = p + length;

      തർക്കം has_private_points = iterator.current_tuple->has_private_points ();
      എങ്കിൽ (has_private_points &&
          !TupleVariationData<>::decompile_points (p, private_indices, end))
        തിരികെ അസത്യം;
      സ്ഥിരം hb_vector_t<unsigned പൂർണ്ണസംഖ്യ> &indices = has_private_points ? private_indices : shared_indices;

      തർക്കം apply_to_all = (indices.length == 0);
      unsigned അക്കം_deltas = apply_to_all ? അക്കം_cvt_item : indices.length;
      എങ്കിൽ (unlikely (!unpacked_deltas.resize_dirty  (അക്കം_deltas))) തിരികെ അസത്യം;
      എങ്കിൽ (unlikely (!TupleVariationData<>::decompile_deltas (p, unpacked_deltas, end))) തിരികെ അസത്യം;

      വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < അക്കം_deltas; i++)
      {
        unsigned പൂർണ്ണസംഖ്യ idx = apply_to_all ? i : indices[i];
        എങ്കിൽ (unlikely (idx >= അക്കം_cvt_item)) തുടരുക;
        എങ്കിൽ (scalar != 1.0f) cvt_deltas[idx] += unpacked_deltas[i] * scalar ;
        അല്ലെങ്കിൽ cvt_deltas[idx] += unpacked_deltas[i];
      }
    } ആകുമ്പോൾ (iterator.move_to_next ());

    തിരികെ സത്യം;
  }
  
  തർക്കം serialize (hb_serialize_context_t *c,
                  TupleVariationData<>::tuple_variations_t& tuple_variations) സ്ഥിരം
  {
    TRACE_SERIALIZE (ഇത്);
    എങ്കിൽ (!tuple_variations) return_trace (അസത്യം);
    എങ്കിൽ (unlikely (!c->embed (version))) return_trace (അസത്യം);

    return_trace (tupleVariationData.serialize (c, അസത്യം, tuple_variations));
  }

  തർക്കം subset (hb_subset_context_t *c) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);
    എങ്കിൽ (c->plan->all_axes_pinned)
      return_trace (അസത്യം);

    OT::TupleVariationData<>::tuple_variations_t tuple_variations;
    unsigned axis_എണ്ണം = c->plan->axes_old_index_tag_map.get_population ();

    സ്ഥിരം hb_tag_t cvt = HB_TAG('c','v','t',' ');
    hb_blob_t *cvt_blob = hb_face_reference_table (c->plan->source, cvt);
    unsigned point_എണ്ണം = hb_blob_get_length (cvt_blob) / FWORD::static_size;
    hb_blob_destroy (cvt_blob);

    എങ്കിൽ (!decompile_tuple_variations (axis_എണ്ണം, point_എണ്ണം,
                                     c->source_blob, അസത്യം,
                                     &(c->plan->axes_old_index_tag_map),
                                     tuple_variations))
      return_trace (അസത്യം);

    optimize_scratch_t scratch;
    എങ്കിൽ (!tuple_variations.instantiate (c->plan->axes_location, c->plan->axes_triple_distances, scratch))
      return_trace (അസത്യം);

    എങ്കിൽ (!tuple_variations.compile_bytes (c->plan->axes_index_map, c->plan->axes_old_index_tag_map,
                                         അസത്യം /* do not use shared points */))
      return_trace (അസത്യം);

    return_trace (serialize (c->serializer, tuple_variations));
  }

  സ്ഥിതം തർക്കം add_cvt_and_apply_deltas (hb_subset_plan_t *plan,
                                        സ്ഥിരം TupleVariationData<> *tuple_var_data,
                                        സ്ഥിരം ശൂന്യം *base)
  {
    സ്ഥിരം hb_tag_t cvt = HB_TAG('c','v','t',' ');
    hb_blob_t *cvt_blob = hb_face_reference_table (plan->source, cvt);
    hb_blob_t *cvt_അഭാജ്യം_blob = hb_blob_copy_writable_or_fail (cvt_blob);
    hb_blob_destroy (cvt_blob);
  
    എങ്കിൽ (unlikely (!cvt_അഭാജ്യം_blob))
      തിരികെ അസത്യം;
 
    unsigned cvt_blob_length = hb_blob_get_length (cvt_അഭാജ്യം_blob);
    unsigned അക്കം_cvt_item = cvt_blob_length / FWORD::static_size;

    hb_vector_t<ദശാംശം> cvt_deltas;
    എങ്കിൽ (unlikely (!cvt_deltas.resize (അക്കം_cvt_item)))
    {
      hb_blob_destroy (cvt_അഭാജ്യം_blob);
      തിരികെ അസത്യം;
    }

    /* Bound the tuple variation data by the actual cvar table length rather
     * than trusting tupleVarCount to walk the headers; an inflated count would
     * otherwise run get_size() off the end of the table. */
    hb_blob_t *cvar_blob = hb_face_reference_table (plan->source, cvar::tableTag);
    unsigned cvar_length = hb_blob_get_length (cvar_blob);
    hb_blob_destroy (cvar_blob);
    unsigned var_data_offset = (unsigned) ((സ്ഥിരം അക്ഷരം *) tuple_var_data - (സ്ഥിരം അക്ഷരം *) base);
    unsigned var_data_length = var_data_offset < cvar_length ? cvar_length - var_data_offset : 0;

    എങ്കിൽ (!calculate_cvt_deltas (plan->normalized_coords.length, plan->normalized_coords.as_array (),
                               അക്കം_cvt_item, tuple_var_data, base, var_data_length, cvt_deltas))
    {
      hb_blob_destroy (cvt_അഭാജ്യം_blob);
      തിരികെ അസത്യം;
    }

    FWORD *cvt_അഭാജ്യം = (FWORD *) hb_blob_get_data_writable (cvt_അഭാജ്യം_blob, nullptr);
    വേണ്ടി (unsigned i = 0; i < അക്കം_cvt_item; i++)
      cvt_അഭാജ്യം[i] += (പൂർണ്ണസംഖ്യ) roundf (cvt_deltas[i]);
    
    തർക്കം success = plan->add_table (cvt, cvt_അഭാജ്യം_blob);
    hb_blob_destroy (cvt_അഭാജ്യം_blob);
    തിരികെ success;
  }

  സംരക്ഷിത:
  FixedVersion<>version;		/* Version of the CVT variation table
					 * initially set to 0x00010000u */
  TupleVariationData<> tupleVariationData; /* TupleVariationDate for cvar table */
  പൊതു:
  DEFINE_SIZE_MIN (8);
};

} /* namespace OT */


#endif /* HB_OT_VAR_CVAR_TABLE_HH */
