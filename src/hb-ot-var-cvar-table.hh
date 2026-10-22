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

#अगर_अघोषित HB_OT_VAR_CVAR_TABLE_HH
#घोषणा HB_OT_VAR_CVAR_TABLE_HH

#समावेश "hb-ot-var-common.hh"
#समावेश "hb-ot-var-fvar-table.hh"


namespace OT {
/*
 * cvar -- control value table (CVT) Variations
 * https://docs.microsoft.com/en-us/typography/opentype/spec/cvar
 */
#घोषणा HB_OT_TAG_cvar HB_TAG('c','v','a','r')

काष्ठा cvar
{
  अटल constexpr hb_tag_t tableTag = HB_OT_TAG_cvar;

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह) &&
		  hb_barrier () &&
		  likely (version.major == 1) &&
		  tuple_Variation_तथ्य.sanitize (c));
  }

  स्थिर Tuple_Variation_तथ्य<>* get_tuple_var_तथ्य (व्योम) स्थिर
  { वापस &tuple_Variation_तथ्य; }

  bool decompile_tuple_variations (अचिन्हित axis_गिनती,
                                   अचिन्हित point_गिनती,
                                   hb_blob_t *blob,
                                   bool is_gvar,
                                   स्थिर hb_map_t *axes_old_सूचकांक_tag_map,
                                   Tuple_Variation_तथ्य<>::tuple_variations_t& tuple_variations /* OUT */) स्थिर
  {
    hb_vector_t<अचिन्हित> shared_indices;
    Tuple_Variation_तथ्य<>::tuple_iterator_t iterator;
    hb_bytes_t var_तथ्य_bytes = blob->as_bytes ().sub_array (4);
    अगर (!Tuple_Variation_तथ्य<>::get_tuple_iterator (var_तथ्य_bytes, axis_गिनती, यह,
                                                 shared_indices, &iterator))
      वापस false;

    वापस tuple_Variation_तथ्य.decompile_tuple_variations (point_गिनती, is_gvar, iterator,
                                                          axes_old_सूचकांक_tag_map,
                                                          shared_indices,
                                                          hb_array<स्थिर F2DOT14> (),
                                                          tuple_variations);
  }

  अटल bool calculate_cvt_deltas (अचिन्हित axis_गिनती,
                                    hb_array_t<पूर्णांक> coords,
                                    अचिन्हित अंक_cvt_item,
                                    स्थिर Tuple_Variation_तथ्य<> *tuple_var_तथ्य,
                                    स्थिर व्योम *base,
                                    अचिन्हित var_तथ्य_लंबाई,
                                    hb_vector_t<भग्न>& cvt_deltas /* OUT */)
  {
    अगर (!coords) वापस true;
    hb_vector_t<अचिन्हित> shared_indices;
    Tuple_Variation_तथ्य<>::tuple_iterator_t iterator;
    hb_bytes_t var_तथ्य_bytes = hb_bytes_t (reinterpret_cast<स्थिर अक्षर*> (tuple_var_तथ्य), var_तथ्य_लंबाई);
    अगर (!Tuple_Variation_तथ्य<>::get_tuple_iterator (var_तथ्य_bytes, axis_गिनती, base,
                                                 shared_indices, &iterator))
      वापस true; /* isn't applied at all */

    hb_array_t<स्थिर F2DOT14> shared_tuples = hb_array<F2DOT14> ();
    hb_vector_t<अचिन्हित> private_indices;
    hb_vector_t<पूर्णांक> unpacked_deltas;

    करो
    {
      भग्न scalar = iterator.current_tuple->calculate_scalar (coords, axis_गिनती, shared_tuples);
      अगर (scalar == 0.f) जारी;
      स्थिर HBUINT8 *p = iterator.get_serialized_तथ्य ();
      अचिन्हित पूर्णांक लंबाई = iterator.current_tuple->get_तथ्य_आकार ();
      अगर (unlikely (!iterator.var_तथ्य_bytes.check_range (p, लंबाई)))
        वापस false;

      स्थिर HBUINT8 *end = p + लंबाई;

      bool has_private_points = iterator.current_tuple->has_private_points ();
      अगर (has_private_points &&
          !Tuple_Variation_तथ्य<>::decompile_points (p, private_indices, end))
        वापस false;
      स्थिर hb_vector_t<अचिन्हित पूर्णांक> &indices = has_private_points ? private_indices : shared_indices;

      bool apply_to_all = (indices.लंबाई == 0);
      अचिन्हित अंक_deltas = apply_to_all ? अंक_cvt_item : indices.लंबाई;
      अगर (unlikely (!unpacked_deltas.resize_dirty  (अंक_deltas))) वापस false;
      अगर (unlikely (!Tuple_Variation_तथ्य<>::decompile_deltas (p, unpacked_deltas, end))) वापस false;

      क्रम (अचिन्हित पूर्णांक i = 0; i < अंक_deltas; i++)
      {
        अचिन्हित पूर्णांक idx = apply_to_all ? i : indices[i];
        अगर (unlikely (idx >= अंक_cvt_item)) जारी;
        अगर (scalar != 1.0f) cvt_deltas[idx] += unpacked_deltas[i] * scalar ;
        अन्यथा cvt_deltas[idx] += unpacked_deltas[i];
      }
    } जबतक (iterator.move_to_अग्र ());

    वापस true;
  }
  
  bool serialize (hb_serialize_context_t *c,
                  Tuple_Variation_तथ्य<>::tuple_variations_t& tuple_variations) स्थिर
  {
    TRACE_SERIALIZE (यह);
    अगर (!tuple_variations) return_trace (false);
    अगर (unlikely (!c->embed (version))) return_trace (false);

    return_trace (tuple_Variation_तथ्य.serialize (c, false, tuple_variations));
  }

  bool subset (hb_subset_context_t *c) स्थिर
  {
    TRACE_SUBSET (यह);
    अगर (c->plan->all_axes_pinned)
      return_trace (false);

    OT::Tuple_Variation_तथ्य<>::tuple_variations_t tuple_variations;
    अचिन्हित axis_गिनती = c->plan->axes_old_सूचकांक_tag_map.get_population ();

    स्थिर hb_tag_t cvt = HB_TAG('c','v','t',' ');
    hb_blob_t *cvt_blob = hb_face_reference_table (c->plan->source, cvt);
    अचिन्हित point_गिनती = hb_blob_get_लंबाई (cvt_blob) / FWORD::static_आकार;
    hb_blob_destroy (cvt_blob);

    अगर (!decompile_tuple_variations (axis_गिनती, point_गिनती,
                                     c->source_blob, false,
                                     &(c->plan->axes_old_सूचकांक_tag_map),
                                     tuple_variations))
      return_trace (false);

    optimize_scratch_t scratch;
    अगर (!tuple_variations.instantiate (c->plan->axes_location, c->plan->axes_triple_distances, scratch))
      return_trace (false);

    अगर (!tuple_variations.compile_bytes (c->plan->axes_सूचकांक_map, c->plan->axes_old_सूचकांक_tag_map,
                                         false /* do not use shared points */))
      return_trace (false);

    return_trace (serialize (c->serializer, tuple_variations));
  }

  अटल bool add_cvt_and_apply_deltas (hb_subset_plan_t *plan,
                                        स्थिर Tuple_Variation_तथ्य<> *tuple_var_तथ्य,
                                        स्थिर व्योम *base)
  {
    स्थिर hb_tag_t cvt = HB_TAG('c','v','t',' ');
    hb_blob_t *cvt_blob = hb_face_reference_table (plan->source, cvt);
    hb_blob_t *cvt_अभाज्य_blob = hb_blob_copy_writable_or_fail (cvt_blob);
    hb_blob_destroy (cvt_blob);
  
    अगर (unlikely (!cvt_अभाज्य_blob))
      वापस false;
 
    अचिन्हित cvt_blob_लंबाई = hb_blob_get_लंबाई (cvt_अभाज्य_blob);
    अचिन्हित अंक_cvt_item = cvt_blob_लंबाई / FWORD::static_आकार;

    hb_vector_t<भग्न> cvt_deltas;
    अगर (unlikely (!cvt_deltas.resize (अंक_cvt_item)))
    {
      hb_blob_destroy (cvt_अभाज्य_blob);
      वापस false;
    }

    /* Bound the tuple variation data by the actual cvar table length rather
     * than trusting tupleVarCount to walk the headers; an inflated count would
     * otherwise run get_size() off the end of the table. */
    hb_blob_t *cvar_blob = hb_face_reference_table (plan->source, cvar::tableTag);
    अचिन्हित cvar_लंबाई = hb_blob_get_लंबाई (cvar_blob);
    hb_blob_destroy (cvar_blob);
    अचिन्हित var_तथ्य_offset = (अचिन्हित) ((स्थिर अक्षर *) tuple_var_तथ्य - (स्थिर अक्षर *) base);
    अचिन्हित var_तथ्य_लंबाई = var_तथ्य_offset < cvar_लंबाई ? cvar_लंबाई - var_तथ्य_offset : 0;

    अगर (!calculate_cvt_deltas (plan->normalized_coords.लंबाई, plan->normalized_coords.as_array (),
                               अंक_cvt_item, tuple_var_तथ्य, base, var_तथ्य_लंबाई, cvt_deltas))
    {
      hb_blob_destroy (cvt_अभाज्य_blob);
      वापस false;
    }

    FWORD *cvt_अभाज्य = (FWORD *) hb_blob_get_तथ्य_writable (cvt_अभाज्य_blob, nullptr);
    क्रम (अचिन्हित i = 0; i < अंक_cvt_item; i++)
      cvt_अभाज्य[i] += (पूर्णांक) roundf (cvt_deltas[i]);
    
    bool success = plan->add_table (cvt, cvt_अभाज्य_blob);
    hb_blob_destroy (cvt_अभाज्य_blob);
    वापस success;
  }

  रक्षित:
  FixedVersion<>version;		/* Version of the CVT variation table
					 * initially set to 0x00010000u */
  Tuple_Variation_तथ्य<> tuple_Variation_तथ्य; /* TupleVariationDate for cvar table */
  खुला:
  DEFINE_आकार_MIN (8);
};

} /* namespace OT */


#पूर्ण_अगर /* HB_OT_VAR_CVAR_TABLE_HH */
