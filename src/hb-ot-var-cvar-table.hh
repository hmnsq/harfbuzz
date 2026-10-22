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

#جے_بنا_تعریف HB_OT_VAR_CVAR_جدول_HH
#تعریف_کرو HB_OT_VAR_CVAR_جدول_HH

#ضمن "hb-ot-var-common.hh"
#ضمن "hb-ot-var-fvar-table.hh"


نطاق OT {
/*
 * cvar -- control value table (CVT) Variations
 * https://docs.microsoft.com/en-us/typography/opentype/spec/cvar
 */
#تعریف_کرو HB_OT_ٹیگ_cvar HB_ٹیگ('c','v','a','r')

بنية cvar
{
  ساكن constexpr hb_ٹیگ_t جدول_ٹیگ = HB_OT_ٹیگ_cvar;

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  likely (نسخہ.major == 1) &&
		  tuple_Variation_انکڑے.sanitize (c));
  }

  ثابت Tuple_Variation_انکڑے<>* get_tuple_var_انکڑے (فراغ) ثابت
  { أرجع &tuple_Variation_انکڑے; }

  منطقي decompile_tuple_variations (بنانشانی axis_گنتی,
                                   بنانشانی نقطہ_گنتی,
                                   hb_blob_t *blob,
                                   منطقي is_gvar,
                                   ثابت hb_map_t *axes_old_اشاریہ_ٹیگ_map,
                                   Tuple_Variation_انکڑے<>::tuple_variations_t& tuple_variations /*باہرلا*/) ثابت
  {
    hb_سمتیہ_t<بنانشانی> shared_indices;
    Tuple_Variation_انکڑے<>::tuple_iterator_t iterator;
    hb_بائٹاں_t var_انکڑے_بائٹاں = blob->as_بائٹاں ().sub_array (4);
    إذا (!Tuple_Variation_انکڑے<>::get_tuple_iterator (var_انکڑے_بائٹاں, axis_گنتی, هذا,
                                                 shared_indices, &iterator))
      أرجع خطأ;

    أرجع tuple_Variation_انکڑے.decompile_tuple_variations (نقطہ_گنتی, is_gvar, iterator,
                                                          axes_old_اشاریہ_ٹیگ_map,
                                                          shared_indices,
                                                          hb_array<ثابت F2DOT14> (),
                                                          tuple_variations);
  }

  ساكن منطقي calculate_cvt_deltas (بنانشانی axis_گنتی,
                                    hb_array_t<صحيح> coords,
                                    بنانشانی عدد_cvt_شے,
                                    ثابت Tuple_Variation_انکڑے<> *tuple_var_انکڑے,
                                    ثابت فراغ *ادھار,
                                    بنانشانی var_انکڑے_لمائی,
                                    hb_سمتیہ_t<عائم>& cvt_deltas /*باہرلا*/)
  {
    إذا (!coords) أرجع سچ;
    hb_سمتیہ_t<بنانشانی> shared_indices;
    Tuple_Variation_انکڑے<>::tuple_iterator_t iterator;
    hb_بائٹاں_t var_انکڑے_بائٹاں = hb_بائٹاں_t (reinterpret_cast<ثابت حرف*> (tuple_var_انکڑے), var_انکڑے_لمائی);
    إذا (!Tuple_Variation_انکڑے<>::get_tuple_iterator (var_انکڑے_بائٹاں, axis_گنتی, ادھار,
                                                 shared_indices, &iterator))
      أرجع سچ; /* isn't applied at all */

    hb_array_t<ثابت F2DOT14> shared_tuples = hb_array<F2DOT14> ();
    hb_سمتیہ_t<بنانشانی> نجی_indices;
    hb_سمتیہ_t<صحيح> unpacked_deltas;

    افعل
    {
      عائم scalar = iterator.current_tuple->calculate_scalar (coords, axis_گنتی, shared_tuples);
      إذا (scalar == 0.f) استمر;
      ثابت HBUINT8 *p = iterator.get_serialized_انکڑے ();
      بنانشانی صحيح لمائی = iterator.current_tuple->get_انکڑے_ناپ ();
      إذا (unlikely (!iterator.var_انکڑے_بائٹاں.check_range (p, لمائی)))
        أرجع خطأ;

      ثابت HBUINT8 *سرا = p + لمائی;

      منطقي has_نجی_points = iterator.current_tuple->has_نجی_points ();
      إذا (has_نجی_points &&
          !Tuple_Variation_انکڑے<>::decompile_points (p, نجی_indices, سرا))
        أرجع خطأ;
      ثابت hb_سمتیہ_t<بنانشانی صحيح> &indices = has_نجی_points ? نجی_indices : shared_indices;

      منطقي apply_منزل_all = (indices.لمائی == 0);
      بنانشانی عدد_deltas = apply_منزل_all ? عدد_cvt_شے : indices.لمائی;
      إذا (unlikely (!unpacked_deltas.resize_dirty  (عدد_deltas))) أرجع خطأ;
      إذا (unlikely (!Tuple_Variation_انکڑے<>::decompile_deltas (p, unpacked_deltas, سرا))) أرجع خطأ;

      لكل (بنانشانی صحيح i = 0; i < عدد_deltas; i++)
      {
        بنانشانی صحيح idx = apply_منزل_all ? i : indices[i];
        إذا (unlikely (idx >= عدد_cvt_شے)) استمر;
        إذا (scalar != 1.0f) cvt_deltas[idx] += unpacked_deltas[i] * scalar ;
        وإلا cvt_deltas[idx] += unpacked_deltas[i];
      }
    } طالما (iterator.move_منزل_اگلا ());

    أرجع سچ;
  }
  
  منطقي serialize (hb_serialize_context_t *c,
                  Tuple_Variation_انکڑے<>::tuple_variations_t& tuple_variations) ثابت
  {
    TRACE_SERIALIZE (هذا);
    إذا (!tuple_variations) return_trace (خطأ);
    إذا (unlikely (!c->embed (نسخہ))) return_trace (خطأ);

    return_trace (tuple_Variation_انکڑے.serialize (c, خطأ, tuple_variations));
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    إذا (c->plan->all_axes_pinned)
      return_trace (خطأ);

    OT::Tuple_Variation_انکڑے<>::tuple_variations_t tuple_variations;
    بنانشانی axis_گنتی = c->plan->axes_old_اشاریہ_ٹیگ_map.get_population ();

    ثابت hb_ٹیگ_t cvt = HB_ٹیگ('c','v','t',' ');
    hb_blob_t *cvt_blob = hb_face_reference_جدول (c->plan->سروت, cvt);
    بنانشانی نقطہ_گنتی = hb_blob_get_لمائی (cvt_blob) / FWORD::static_ناپ;
    hb_blob_destroy (cvt_blob);

    إذا (!decompile_tuple_variations (axis_گنتی, نقطہ_گنتی,
                                     c->سروت_blob, خطأ,
                                     &(c->plan->axes_old_اشاریہ_ٹیگ_map),
                                     tuple_variations))
      return_trace (خطأ);

    optimize_scratch_t scratch;
    إذا (!tuple_variations.instantiate (c->plan->axes_location, c->plan->axes_triple_distances, scratch))
      return_trace (خطأ);

    إذا (!tuple_variations.compile_بائٹاں (c->plan->axes_اشاریہ_map, c->plan->axes_old_اشاریہ_ٹیگ_map,
                                         خطأ /* do not use shared points */))
      return_trace (خطأ);

    return_trace (serialize (c->serializer, tuple_variations));
  }

  ساكن منطقي جوڑو_cvt_and_apply_deltas (hb_subset_plan_t *plan,
                                        ثابت Tuple_Variation_انکڑے<> *tuple_var_انکڑے,
                                        ثابت فراغ *ادھار)
  {
    ثابت hb_ٹیگ_t cvt = HB_ٹیگ('c','v','t',' ');
    hb_blob_t *cvt_blob = hb_face_reference_جدول (plan->سروت, cvt);
    hb_blob_t *cvt_مفرد_blob = hb_blob_copy_writable_or_fail (cvt_blob);
    hb_blob_destroy (cvt_blob);
  
    إذا (unlikely (!cvt_مفرد_blob))
      أرجع خطأ;
 
    بنانشانی cvt_blob_لمائی = hb_blob_get_لمائی (cvt_مفرد_blob);
    بنانشانی عدد_cvt_شے = cvt_blob_لمائی / FWORD::static_ناپ;

    hb_سمتیہ_t<عائم> cvt_deltas;
    إذا (unlikely (!cvt_deltas.resize (عدد_cvt_شے)))
    {
      hb_blob_destroy (cvt_مفرد_blob);
      أرجع خطأ;
    }

    /* Bound the tuple variation data by the actual cvar table length rather
     * than trusting tupleVarCount to walk the headers; an inflated count would
     * otherwise run get_size() off the end of the table. */
    hb_blob_t *cvar_blob = hb_face_reference_جدول (plan->سروت, cvar::جدول_ٹیگ);
    بنانشانی cvar_لمائی = hb_blob_get_لمائی (cvar_blob);
    hb_blob_destroy (cvar_blob);
    بنانشانی var_انکڑے_وتھ = (بنانشانی) ((ثابت حرف *) tuple_var_انکڑے - (ثابت حرف *) ادھار);
    بنانشانی var_انکڑے_لمائی = var_انکڑے_وتھ < cvar_لمائی ? cvar_لمائی - var_انکڑے_وتھ : 0;

    إذا (!calculate_cvt_deltas (plan->normalized_coords.لمائی, plan->normalized_coords.as_array (),
                               عدد_cvt_شے, tuple_var_انکڑے, ادھار, var_انکڑے_لمائی, cvt_deltas))
    {
      hb_blob_destroy (cvt_مفرد_blob);
      أرجع خطأ;
    }

    FWORD *cvt_مفرد = (FWORD *) hb_blob_get_انکڑے_writable (cvt_مفرد_blob, خالی_سوچک);
    لكل (بنانشانی i = 0; i < عدد_cvt_شے; i++)
      cvt_مفرد[i] += (صحيح) roundf (cvt_deltas[i]);
    
    منطقي success = plan->جوڑو_جدول (cvt, cvt_مفرد_blob);
    hb_blob_destroy (cvt_مفرد_blob);
    أرجع success;
  }

  محمي:
  Fixed_نسخہ<>نسخہ;		/* Version of the CVT variation table
					 * initially set to 0x00010000u */
  Tuple_Variation_انکڑے<> tuple_Variation_انکڑے; /* TupleVariationDate for cvar table */
  عام:
  DEFINE_ناپ_MIN (8);
};

} /* namespace OT */


#جے_انت /* HB_OT_VAR_CVAR_TABLE_HH */
