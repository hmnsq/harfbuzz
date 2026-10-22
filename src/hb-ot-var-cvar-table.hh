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

#اگر_تعریف_نشده HB_OT_VAR_CVAR_جدول_HH
#تعریف_کن HB_OT_VAR_CVAR_جدول_HH

#ضمن "hb-ot-var-common.hh"
#ضمن "hb-ot-var-fvar-table.hh"


نطاق OT {
/*
 * cvar -- control value table (CVT) Variations
 * https://docs.microsoft.com/en-us/typography/opentype/spec/cvar
 */
#تعریف_کن HB_OT_برچسب_cvar HB_برچسب('c','v','a','r')

بنية cvar
{
  ساكن constexpr hb_برچسب_t جدول_برچسب = HB_OT_برچسب_cvar;

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  likely (نسخه.major == 1) &&
		  tuple_Variation_داده.sanitize (c));
  }

  ثابت Tuple_Variation_داده<>* get_tuple_var_داده (فراغ) ثابت
  { أرجع &tuple_Variation_داده; }

  منطقي decompile_tuple_variations (بیعلامت axis_شمار,
                                   بیعلامت نقطه_شمار,
                                   hb_blob_t *blob,
                                   منطقي is_gvar,
                                   ثابت hb_map_t *axes_old_شاخص_برچسب_map,
                                   Tuple_Variation_داده<>::tuple_variations_t& tuple_variations /*بیرون*/) ثابت
  {
    hb_وکتور_t<بیعلامت> shared_indices;
    Tuple_Variation_داده<>::tuple_iterator_t iterator;
    hb_بایتها_t var_داده_بایتها = blob->as_بایتها ().sub_array (4);
    إذا (!Tuple_Variation_داده<>::get_tuple_iterator (var_داده_بایتها, axis_شمار, هذا,
                                                 shared_indices, &iterator))
      أرجع خطأ;

    أرجع tuple_Variation_داده.decompile_tuple_variations (نقطه_شمار, is_gvar, iterator,
                                                          axes_old_شاخص_برچسب_map,
                                                          shared_indices,
                                                          hb_array<ثابت F2DOT14> (),
                                                          tuple_variations);
  }

  ساكن منطقي calculate_cvt_deltas (بیعلامت axis_شمار,
                                    hb_array_t<صحيح> coords,
                                    بیعلامت رقم_cvt_قلم,
                                    ثابت Tuple_Variation_داده<> *tuple_var_داده,
                                    ثابت فراغ *پایه,
                                    بیعلامت var_داده_درازی,
                                    hb_وکتور_t<عائم>& cvt_deltas /*بیرون*/)
  {
    إذا (!coords) أرجع درست;
    hb_وکتور_t<بیعلامت> shared_indices;
    Tuple_Variation_داده<>::tuple_iterator_t iterator;
    hb_بایتها_t var_داده_بایتها = hb_بایتها_t (reinterpret_cast<ثابت حرف*> (tuple_var_داده), var_داده_درازی);
    إذا (!Tuple_Variation_داده<>::get_tuple_iterator (var_داده_بایتها, axis_شمار, پایه,
                                                 shared_indices, &iterator))
      أرجع درست; /* isn't applied at all */

    hb_array_t<ثابت F2DOT14> shared_tuples = hb_array<F2DOT14> ();
    hb_وکتور_t<بیعلامت> خصوصی_indices;
    hb_وکتور_t<صحيح> unpacked_deltas;

    افعل
    {
      عائم scalar = iterator.current_tuple->calculate_scalar (coords, axis_شمار, shared_tuples);
      إذا (scalar == 0.f) استمر;
      ثابت HBUINT8 *p = iterator.get_serialized_داده ();
      بیعلامت صحيح درازی = iterator.current_tuple->get_داده_اندازه ();
      إذا (unlikely (!iterator.var_داده_بایتها.check_range (p, درازی)))
        أرجع خطأ;

      ثابت HBUINT8 *انتها = p + درازی;

      منطقي has_خصوصی_points = iterator.current_tuple->has_خصوصی_points ();
      إذا (has_خصوصی_points &&
          !Tuple_Variation_داده<>::decompile_points (p, خصوصی_indices, انتها))
        أرجع خطأ;
      ثابت hb_وکتور_t<بیعلامت صحيح> &indices = has_خصوصی_points ? خصوصی_indices : shared_indices;

      منطقي apply_مقصد_all = (indices.درازی == 0);
      بیعلامت رقم_deltas = apply_مقصد_all ? رقم_cvt_قلم : indices.درازی;
      إذا (unlikely (!unpacked_deltas.resize_dirty  (رقم_deltas))) أرجع خطأ;
      إذا (unlikely (!Tuple_Variation_داده<>::decompile_deltas (p, unpacked_deltas, انتها))) أرجع خطأ;

      لكل (بیعلامت صحيح i = 0; i < رقم_deltas; i++)
      {
        بیعلامت صحيح idx = apply_مقصد_all ? i : indices[i];
        إذا (unlikely (idx >= رقم_cvt_قلم)) استمر;
        إذا (scalar != 1.0f) cvt_deltas[idx] += unpacked_deltas[i] * scalar ;
        وإلا cvt_deltas[idx] += unpacked_deltas[i];
      }
    } طالما (iterator.move_مقصد_بعدی ());

    أرجع درست;
  }
  
  منطقي serialize (hb_serialize_context_t *c,
                  Tuple_Variation_داده<>::tuple_variations_t& tuple_variations) ثابت
  {
    TRACE_SERIALIZE (هذا);
    إذا (!tuple_variations) return_trace (خطأ);
    إذا (unlikely (!c->embed (نسخه))) return_trace (خطأ);

    return_trace (tuple_Variation_داده.serialize (c, خطأ, tuple_variations));
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    إذا (c->plan->all_axes_pinned)
      return_trace (خطأ);

    OT::Tuple_Variation_داده<>::tuple_variations_t tuple_variations;
    بیعلامت axis_شمار = c->plan->axes_old_شاخص_برچسب_map.get_population ();

    ثابت hb_برچسب_t cvt = HB_برچسب('c','v','t',' ');
    hb_blob_t *cvt_blob = hb_face_reference_جدول (c->plan->منبع, cvt);
    بیعلامت نقطه_شمار = hb_blob_get_درازی (cvt_blob) / FWORD::static_اندازه;
    hb_blob_destroy (cvt_blob);

    إذا (!decompile_tuple_variations (axis_شمار, نقطه_شمار,
                                     c->منبع_blob, خطأ,
                                     &(c->plan->axes_old_شاخص_برچسب_map),
                                     tuple_variations))
      return_trace (خطأ);

    optimize_scratch_t scratch;
    إذا (!tuple_variations.instantiate (c->plan->axes_location, c->plan->axes_triple_distances, scratch))
      return_trace (خطأ);

    إذا (!tuple_variations.compile_بایتها (c->plan->axes_شاخص_map, c->plan->axes_old_شاخص_برچسب_map,
                                         خطأ /* do not use shared points */))
      return_trace (خطأ);

    return_trace (serialize (c->serializer, tuple_variations));
  }

  ساكن منطقي جمع_کن_cvt_and_apply_deltas (hb_subset_plan_t *plan,
                                        ثابت Tuple_Variation_داده<> *tuple_var_داده,
                                        ثابت فراغ *پایه)
  {
    ثابت hb_برچسب_t cvt = HB_برچسب('c','v','t',' ');
    hb_blob_t *cvt_blob = hb_face_reference_جدول (plan->منبع, cvt);
    hb_blob_t *cvt_اول_blob = hb_blob_copy_writable_or_fail (cvt_blob);
    hb_blob_destroy (cvt_blob);
  
    إذا (unlikely (!cvt_اول_blob))
      أرجع خطأ;
 
    بیعلامت cvt_blob_درازی = hb_blob_get_درازی (cvt_اول_blob);
    بیعلامت رقم_cvt_قلم = cvt_blob_درازی / FWORD::static_اندازه;

    hb_وکتور_t<عائم> cvt_deltas;
    إذا (unlikely (!cvt_deltas.resize (رقم_cvt_قلم)))
    {
      hb_blob_destroy (cvt_اول_blob);
      أرجع خطأ;
    }

    /* Bound the tuple variation data by the actual cvar table length rather
     * than trusting tupleVarCount to walk the headers; an inflated count would
     * otherwise run get_size() off the end of the table. */
    hb_blob_t *cvar_blob = hb_face_reference_جدول (plan->منبع, cvar::جدول_برچسب);
    بیعلامت cvar_درازی = hb_blob_get_درازی (cvar_blob);
    hb_blob_destroy (cvar_blob);
    بیعلامت var_داده_جابه_جایی = (بیعلامت) ((ثابت حرف *) tuple_var_داده - (ثابت حرف *) پایه);
    بیعلامت var_داده_درازی = var_داده_جابه_جایی < cvar_درازی ? cvar_درازی - var_داده_جابه_جایی : 0;

    إذا (!calculate_cvt_deltas (plan->normalized_coords.درازی, plan->normalized_coords.as_array (),
                               رقم_cvt_قلم, tuple_var_داده, پایه, var_داده_درازی, cvt_deltas))
    {
      hb_blob_destroy (cvt_اول_blob);
      أرجع خطأ;
    }

    FWORD *cvt_اول = (FWORD *) hb_blob_get_داده_writable (cvt_اول_blob, اشاره_تهی);
    لكل (بیعلامت i = 0; i < رقم_cvt_قلم; i++)
      cvt_اول[i] += (صحيح) roundf (cvt_deltas[i]);
    
    منطقي success = plan->جمع_کن_جدول (cvt, cvt_اول_blob);
    hb_blob_destroy (cvt_اول_blob);
    أرجع success;
  }

  محمي:
  Fixed_نسخه<>نسخه;		/* Version of the CVT variation table
					 * initially set to 0x00010000u */
  Tuple_Variation_داده<> tuple_Variation_داده; /* TupleVariationDate for cvar table */
  عام:
  DEFINE_اندازه_MIN (8);
};

} /* namespace OT */


#پایان_اگر /* HB_OT_VAR_CVAR_TABLE_HH */
