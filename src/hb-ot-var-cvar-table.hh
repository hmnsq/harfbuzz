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

কাঠামো cvar
{
  স্থির constexpr hb_tag_t tableTag = HB_OT_TAG_cvar;

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই) &&
		  hb_barrier () &&
		  likely (version.major == 1) &&
		  tuple_Variation_তথ্য.sanitize (c));
  }

  ধ্রুবক Tuple_Variation_তথ্য<>* get_tuple_var_তথ্য (শূন্য) ধ্রুবক
  { ফেরত &tuple_Variation_তথ্য; }

  যৌক্তিক decompile_tuple_variations (unsigned axis_গুনতি,
                                   unsigned point_গুনতি,
                                   hb_blob_t *blob,
                                   যৌক্তিক is_gvar,
                                   ধ্রুবক hb_map_t *axes_old_সূচক_tag_map,
                                   Tuple_Variation_তথ্য<>::tuple_variations_t& tuple_variations /* OUT */) ধ্রুবক
  {
    hb_vector_t<unsigned> shared_indices;
    Tuple_Variation_তথ্য<>::tuple_iterator_t iterator;
    hb_bytes_t var_তথ্য_bytes = blob->as_bytes ().sub_array (4);
    যদি (!Tuple_Variation_তথ্য<>::get_tuple_iterator (var_তথ্য_bytes, axis_গুনতি, এই,
                                                 shared_indices, &iterator))
      ফেরত মিথ্যা;

    ফেরত tuple_Variation_তথ্য.decompile_tuple_variations (point_গুনতি, is_gvar, iterator,
                                                          axes_old_সূচক_tag_map,
                                                          shared_indices,
                                                          hb_array<ধ্রুবক F2DOT14> (),
                                                          tuple_variations);
  }

  স্থির যৌক্তিক calculate_cvt_deltas (unsigned axis_গুনতি,
                                    hb_array_t<পূর্ণসংখ্যা> coords,
                                    unsigned অঙ্ক_cvt_item,
                                    ধ্রুবক Tuple_Variation_তথ্য<> *tuple_var_তথ্য,
                                    ধ্রুবক শূন্য *base,
                                    unsigned var_তথ্য_দৈর্ঘ্য,
                                    hb_vector_t<দশমিক>& cvt_deltas /* OUT */)
  {
    যদি (!coords) ফেরত সত্য;
    hb_vector_t<unsigned> shared_indices;
    Tuple_Variation_তথ্য<>::tuple_iterator_t iterator;
    hb_bytes_t var_তথ্য_bytes = hb_bytes_t (reinterpret_cast<ধ্রুবক অক্ষর*> (tuple_var_তথ্য), var_তথ্য_দৈর্ঘ্য);
    যদি (!Tuple_Variation_তথ্য<>::get_tuple_iterator (var_তথ্য_bytes, axis_গুনতি, base,
                                                 shared_indices, &iterator))
      ফেরত সত্য; /* isn't applied at all */

    hb_array_t<ধ্রুবক F2DOT14> shared_tuples = hb_array<F2DOT14> ();
    hb_vector_t<unsigned> private_indices;
    hb_vector_t<পূর্ণসংখ্যা> unpacked_deltas;

    করো
    {
      দশমিক scalar = iterator.current_tuple->calculate_scalar (coords, axis_গুনতি, shared_tuples);
      যদি (scalar == 0.f) চালু;
      ধ্রুবক HBUINT8 *p = iterator.get_serialized_তথ্য ();
      unsigned পূর্ণসংখ্যা দৈর্ঘ্য = iterator.current_tuple->get_তথ্য_আকার ();
      যদি (unlikely (!iterator.var_তথ্য_bytes.check_range (p, দৈর্ঘ্য)))
        ফেরত মিথ্যা;

      ধ্রুবক HBUINT8 *end = p + দৈর্ঘ্য;

      যৌক্তিক has_private_points = iterator.current_tuple->has_private_points ();
      যদি (has_private_points &&
          !Tuple_Variation_তথ্য<>::decompile_points (p, private_indices, end))
        ফেরত মিথ্যা;
      ধ্রুবক hb_vector_t<unsigned পূর্ণসংখ্যা> &indices = has_private_points ? private_indices : shared_indices;

      যৌক্তিক apply_to_all = (indices.দৈর্ঘ্য == 0);
      unsigned অঙ্ক_deltas = apply_to_all ? অঙ্ক_cvt_item : indices.দৈর্ঘ্য;
      যদি (unlikely (!unpacked_deltas.resize_dirty  (অঙ্ক_deltas))) ফেরত মিথ্যা;
      যদি (unlikely (!Tuple_Variation_তথ্য<>::decompile_deltas (p, unpacked_deltas, end))) ফেরত মিথ্যা;

      জন্য (unsigned পূর্ণসংখ্যা i = 0; i < অঙ্ক_deltas; i++)
      {
        unsigned পূর্ণসংখ্যা idx = apply_to_all ? i : indices[i];
        যদি (unlikely (idx >= অঙ্ক_cvt_item)) চালু;
        যদি (scalar != 1.0f) cvt_deltas[idx] += unpacked_deltas[i] * scalar ;
        অন্যথা cvt_deltas[idx] += unpacked_deltas[i];
      }
    } যখন (iterator.move_to_পরবর্তী ());

    ফেরত সত্য;
  }
  
  যৌক্তিক serialize (hb_serialize_context_t *c,
                  Tuple_Variation_তথ্য<>::tuple_variations_t& tuple_variations) ধ্রুবক
  {
    TRACE_SERIALIZE (এই);
    যদি (!tuple_variations) return_trace (মিথ্যা);
    যদি (unlikely (!c->embed (version))) return_trace (মিথ্যা);

    return_trace (tuple_Variation_তথ্য.serialize (c, মিথ্যা, tuple_variations));
  }

  যৌক্তিক subset (hb_subset_context_t *c) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    যদি (c->plan->all_axes_pinned)
      return_trace (মিথ্যা);

    OT::Tuple_Variation_তথ্য<>::tuple_variations_t tuple_variations;
    unsigned axis_গুনতি = c->plan->axes_old_সূচক_tag_map.get_population ();

    ধ্রুবক hb_tag_t cvt = HB_TAG('c','v','t',' ');
    hb_blob_t *cvt_blob = hb_face_reference_table (c->plan->source, cvt);
    unsigned point_গুনতি = hb_blob_get_দৈর্ঘ্য (cvt_blob) / FWORD::static_আকার;
    hb_blob_destroy (cvt_blob);

    যদি (!decompile_tuple_variations (axis_গুনতি, point_গুনতি,
                                     c->source_blob, মিথ্যা,
                                     &(c->plan->axes_old_সূচক_tag_map),
                                     tuple_variations))
      return_trace (মিথ্যা);

    optimize_scratch_t scratch;
    যদি (!tuple_variations.instantiate (c->plan->axes_location, c->plan->axes_triple_distances, scratch))
      return_trace (মিথ্যা);

    যদি (!tuple_variations.compile_bytes (c->plan->axes_সূচক_map, c->plan->axes_old_সূচক_tag_map,
                                         মিথ্যা /* do not use shared points */))
      return_trace (মিথ্যা);

    return_trace (serialize (c->serializer, tuple_variations));
  }

  স্থির যৌক্তিক add_cvt_and_apply_deltas (hb_subset_plan_t *plan,
                                        ধ্রুবক Tuple_Variation_তথ্য<> *tuple_var_তথ্য,
                                        ধ্রুবক শূন্য *base)
  {
    ধ্রুবক hb_tag_t cvt = HB_TAG('c','v','t',' ');
    hb_blob_t *cvt_blob = hb_face_reference_table (plan->source, cvt);
    hb_blob_t *cvt_মৌলিক_blob = hb_blob_copy_writable_or_fail (cvt_blob);
    hb_blob_destroy (cvt_blob);
  
    যদি (unlikely (!cvt_মৌলিক_blob))
      ফেরত মিথ্যা;
 
    unsigned cvt_blob_দৈর্ঘ্য = hb_blob_get_দৈর্ঘ্য (cvt_মৌলিক_blob);
    unsigned অঙ্ক_cvt_item = cvt_blob_দৈর্ঘ্য / FWORD::static_আকার;

    hb_vector_t<দশমিক> cvt_deltas;
    যদি (unlikely (!cvt_deltas.resize (অঙ্ক_cvt_item)))
    {
      hb_blob_destroy (cvt_মৌলিক_blob);
      ফেরত মিথ্যা;
    }

    /* Bound the tuple variation data by the actual cvar table length rather
     * than trusting tupleVarCount to walk the headers; an inflated count would
     * otherwise run get_size() off the end of the table. */
    hb_blob_t *cvar_blob = hb_face_reference_table (plan->source, cvar::tableTag);
    unsigned cvar_দৈর্ঘ্য = hb_blob_get_দৈর্ঘ্য (cvar_blob);
    hb_blob_destroy (cvar_blob);
    unsigned var_তথ্য_offset = (unsigned) ((ধ্রুবক অক্ষর *) tuple_var_তথ্য - (ধ্রুবক অক্ষর *) base);
    unsigned var_তথ্য_দৈর্ঘ্য = var_তথ্য_offset < cvar_দৈর্ঘ্য ? cvar_দৈর্ঘ্য - var_তথ্য_offset : 0;

    যদি (!calculate_cvt_deltas (plan->normalized_coords.দৈর্ঘ্য, plan->normalized_coords.as_array (),
                               অঙ্ক_cvt_item, tuple_var_তথ্য, base, var_তথ্য_দৈর্ঘ্য, cvt_deltas))
    {
      hb_blob_destroy (cvt_মৌলিক_blob);
      ফেরত মিথ্যা;
    }

    FWORD *cvt_মৌলিক = (FWORD *) hb_blob_get_তথ্য_writable (cvt_মৌলিক_blob, nullptr);
    জন্য (unsigned i = 0; i < অঙ্ক_cvt_item; i++)
      cvt_মৌলিক[i] += (পূর্ণসংখ্যা) roundf (cvt_deltas[i]);
    
    যৌক্তিক success = plan->add_table (cvt, cvt_মৌলিক_blob);
    hb_blob_destroy (cvt_মৌলিক_blob);
    ফেরত success;
  }

  সুরক্ষিত:
  FixedVersion<>version;		/* Version of the CVT variation table
					 * initially set to 0x00010000u */
  Tuple_Variation_তথ্য<> tuple_Variation_তথ্য; /* TupleVariationDate for cvar table */
  সর্বজনীন:
  DEFINE_আকার_MIN (8);
};

} /* namespace OT */


#endif /* HB_OT_VAR_CVAR_TABLE_HH */
