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

#جيڪڏهن_وصف_نه_ٿيل HB_OT_VAR_CVAR_جدول_HH
#وصف_ڪريو HB_OT_VAR_CVAR_جدول_HH

#ضمن "hb-ot-var-common.hh"
#ضمن "hb-ot-var-fvar-table.hh"


نطاق OT {
/*
 * cvar -- control value table (CVT) Variations
 * https://docs.microsoft.com/en-us/typography/opentype/spec/cvar
 */
#وصف_ڪريو HB_OT_ٽيگ_cvar HB_ٽيگ('c','v','a','r')

بنية cvar
{
  ساكن constexpr hb_ٽيگ_t جدول_ٽيگ = HB_OT_ٽيگ_cvar;

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  likely (نسخو.major == 1) &&
		  tuple_Variation_معلومات.sanitize (c));
  }

  ثابت Tuple_Variation_معلومات<>* get_tuple_var_معلومات (فراغ) ثابت
  { أرجع &tuple_Variation_معلومات; }

  منطقي decompile_tuple_variations (بنانشان axis_ڳڻپ,
                                   بنانشان نقطو_ڳڻپ,
                                   hb_blob_t *blob,
                                   منطقي is_gvar,
                                   ثابت hb_map_t *axes_old_اشاريو_ٽيگ_map,
                                   Tuple_Variation_معلومات<>::tuple_variations_t& tuple_variations /*ٻاهريون*/) ثابت
  {
    hb_ويڪٽر_t<بنانشان> shared_indices;
    Tuple_Variation_معلومات<>::tuple_iterator_t iterator;
    hb_بائيٽون_t var_معلومات_بائيٽون = blob->as_بائيٽون ().sub_array (4);
    إذا (!Tuple_Variation_معلومات<>::get_tuple_iterator (var_معلومات_بائيٽون, axis_ڳڻپ, هذا,
                                                 shared_indices, &iterator))
      أرجع خطأ;

    أرجع tuple_Variation_معلومات.decompile_tuple_variations (نقطو_ڳڻپ, is_gvar, iterator,
                                                          axes_old_اشاريو_ٽيگ_map,
                                                          shared_indices,
                                                          hb_array<ثابت F2DOT14> (),
                                                          tuple_variations);
  }

  ساكن منطقي calculate_cvt_deltas (بنانشان axis_ڳڻپ,
                                    hb_array_t<صحيح> coords,
                                    بنانشان انگ_cvt_شي,
                                    ثابت Tuple_Variation_معلومات<> *tuple_var_معلومات,
                                    ثابت فراغ *بنياد,
                                    بنانشان var_معلومات_ڊيگهائي,
                                    hb_ويڪٽر_t<عائم>& cvt_deltas /*ٻاهريون*/)
  {
    إذا (!coords) أرجع سچ;
    hb_ويڪٽر_t<بنانشان> shared_indices;
    Tuple_Variation_معلومات<>::tuple_iterator_t iterator;
    hb_بائيٽون_t var_معلومات_بائيٽون = hb_بائيٽون_t (reinterpret_cast<ثابت حرف*> (tuple_var_معلومات), var_معلومات_ڊيگهائي);
    إذا (!Tuple_Variation_معلومات<>::get_tuple_iterator (var_معلومات_بائيٽون, axis_ڳڻپ, بنياد,
                                                 shared_indices, &iterator))
      أرجع سچ; /* isn't applied at all */

    hb_array_t<ثابت F2DOT14> shared_tuples = hb_array<F2DOT14> ();
    hb_ويڪٽر_t<بنانشان> ذاتي_indices;
    hb_ويڪٽر_t<صحيح> unpacked_deltas;

    افعل
    {
      عائم scalar = iterator.current_tuple->calculate_scalar (coords, axis_ڳڻپ, shared_tuples);
      إذا (scalar == 0.f) استمر;
      ثابت HBUINT8 *p = iterator.get_serialized_معلومات ();
      بنانشان صحيح ڊيگهائي = iterator.current_tuple->get_معلومات_سائيز ();
      إذا (unlikely (!iterator.var_معلومات_بائيٽون.check_range (p, ڊيگهائي)))
        أرجع خطأ;

      ثابت HBUINT8 *پڇاڙي = p + ڊيگهائي;

      منطقي has_ذاتي_points = iterator.current_tuple->has_ذاتي_points ();
      إذا (has_ذاتي_points &&
          !Tuple_Variation_معلومات<>::decompile_points (p, ذاتي_indices, پڇاڙي))
        أرجع خطأ;
      ثابت hb_ويڪٽر_t<بنانشان صحيح> &indices = has_ذاتي_points ? ذاتي_indices : shared_indices;

      منطقي apply_منزل_all = (indices.ڊيگهائي == 0);
      بنانشان انگ_deltas = apply_منزل_all ? انگ_cvt_شي : indices.ڊيگهائي;
      إذا (unlikely (!unpacked_deltas.resize_dirty  (انگ_deltas))) أرجع خطأ;
      إذا (unlikely (!Tuple_Variation_معلومات<>::decompile_deltas (p, unpacked_deltas, پڇاڙي))) أرجع خطأ;

      لكل (بنانشان صحيح i = 0; i < انگ_deltas; i++)
      {
        بنانشان صحيح idx = apply_منزل_all ? i : indices[i];
        إذا (unlikely (idx >= انگ_cvt_شي)) استمر;
        إذا (scalar != 1.0f) cvt_deltas[idx] += unpacked_deltas[i] * scalar ;
        وإلا cvt_deltas[idx] += unpacked_deltas[i];
      }
    } طالما (iterator.move_منزل_ايندڙ ());

    أرجع سچ;
  }
  
  منطقي serialize (hb_serialize_context_t *c,
                  Tuple_Variation_معلومات<>::tuple_variations_t& tuple_variations) ثابت
  {
    TRACE_SERIALIZE (هذا);
    إذا (!tuple_variations) return_trace (خطأ);
    إذا (unlikely (!c->embed (نسخو))) return_trace (خطأ);

    return_trace (tuple_Variation_معلومات.serialize (c, خطأ, tuple_variations));
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    إذا (c->plan->all_axes_pinned)
      return_trace (خطأ);

    OT::Tuple_Variation_معلومات<>::tuple_variations_t tuple_variations;
    بنانشان axis_ڳڻپ = c->plan->axes_old_اشاريو_ٽيگ_map.get_population ();

    ثابت hb_ٽيگ_t cvt = HB_ٽيگ('c','v','t',' ');
    hb_blob_t *cvt_blob = hb_face_reference_جدول (c->plan->ذريعو, cvt);
    بنانشان نقطو_ڳڻپ = hb_blob_get_ڊيگهائي (cvt_blob) / FWORD::static_سائيز;
    hb_blob_destroy (cvt_blob);

    إذا (!decompile_tuple_variations (axis_ڳڻپ, نقطو_ڳڻپ,
                                     c->ذريعو_blob, خطأ,
                                     &(c->plan->axes_old_اشاريو_ٽيگ_map),
                                     tuple_variations))
      return_trace (خطأ);

    optimize_scratch_t scratch;
    إذا (!tuple_variations.instantiate (c->plan->axes_location, c->plan->axes_triple_distances, scratch))
      return_trace (خطأ);

    إذا (!tuple_variations.compile_بائيٽون (c->plan->axes_اشاريو_map, c->plan->axes_old_اشاريو_ٽيگ_map,
                                         خطأ /* do not use shared points */))
      return_trace (خطأ);

    return_trace (serialize (c->serializer, tuple_variations));
  }

  ساكن منطقي جوڙيو_cvt_and_apply_deltas (hb_subset_plan_t *plan,
                                        ثابت Tuple_Variation_معلومات<> *tuple_var_معلومات,
                                        ثابت فراغ *بنياد)
  {
    ثابت hb_ٽيگ_t cvt = HB_ٽيگ('c','v','t',' ');
    hb_blob_t *cvt_blob = hb_face_reference_جدول (plan->ذريعو, cvt);
    hb_blob_t *cvt_مفرد_blob = hb_blob_copy_writable_or_fail (cvt_blob);
    hb_blob_destroy (cvt_blob);
  
    إذا (unlikely (!cvt_مفرد_blob))
      أرجع خطأ;
 
    بنانشان cvt_blob_ڊيگهائي = hb_blob_get_ڊيگهائي (cvt_مفرد_blob);
    بنانشان انگ_cvt_شي = cvt_blob_ڊيگهائي / FWORD::static_سائيز;

    hb_ويڪٽر_t<عائم> cvt_deltas;
    إذا (unlikely (!cvt_deltas.resize (انگ_cvt_شي)))
    {
      hb_blob_destroy (cvt_مفرد_blob);
      أرجع خطأ;
    }

    /* Bound the tuple variation data by the actual cvar table length rather
     * than trusting tupleVarCount to walk the headers; an inflated count would
     * otherwise run get_size() off the end of the table. */
    hb_blob_t *cvar_blob = hb_face_reference_جدول (plan->ذريعو, cvar::جدول_ٽيگ);
    بنانشان cvar_ڊيگهائي = hb_blob_get_ڊيگهائي (cvar_blob);
    hb_blob_destroy (cvar_blob);
    بنانشان var_معلومات_وٿي = (بنانشان) ((ثابت حرف *) tuple_var_معلومات - (ثابت حرف *) بنياد);
    بنانشان var_معلومات_ڊيگهائي = var_معلومات_وٿي < cvar_ڊيگهائي ? cvar_ڊيگهائي - var_معلومات_وٿي : 0;

    إذا (!calculate_cvt_deltas (plan->normalized_coords.ڊيگهائي, plan->normalized_coords.as_array (),
                               انگ_cvt_شي, tuple_var_معلومات, بنياد, var_معلومات_ڊيگهائي, cvt_deltas))
    {
      hb_blob_destroy (cvt_مفرد_blob);
      أرجع خطأ;
    }

    FWORD *cvt_مفرد = (FWORD *) hb_blob_get_معلومات_writable (cvt_مفرد_blob, خالي_اشارو);
    لكل (بنانشان i = 0; i < انگ_cvt_شي; i++)
      cvt_مفرد[i] += (صحيح) roundf (cvt_deltas[i]);
    
    منطقي success = plan->جوڙيو_جدول (cvt, cvt_مفرد_blob);
    hb_blob_destroy (cvt_مفرد_blob);
    أرجع success;
  }

  محمي:
  Fixed_نسخو<>نسخو;		/* Version of the CVT variation table
					 * initially set to 0x00010000u */
  Tuple_Variation_معلومات<> tuple_Variation_معلومات; /* TupleVariationDate for cvar table */
  عام:
  DEFINE_سائيز_MIN (8);
};

} /* namespace OT */


#جيڪڏهن_پورو /* HB_OT_VAR_CVAR_TABLE_HH */
