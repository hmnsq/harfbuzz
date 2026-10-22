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

#که_نه_تعریف_شوی HB_OT_VAR_CVAR_جدول_HH
#تعریف_کړه HB_OT_VAR_CVAR_جدول_HH

#ضمن "hb-ot-var-common.hh"
#ضمن "hb-ot-var-fvar-table.hh"


نطاق OT {
/*
 * cvar -- control value table (CVT) Variations
 * https://docs.microsoft.com/en-us/typography/opentype/spec/cvar
 */
#تعریف_کړه HB_OT_نښان_cvar HB_نښان('c','v','a','r')

بنية cvar
{
  ساكن constexpr hb_نښان_t جدول_نښان = HB_OT_نښان_cvar;

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  likely (نسخه.major == 1) &&
		  tuple_Variation_معلومات.sanitize (c));
  }

  ثابت Tuple_Variation_معلومات<>* get_tuple_var_معلومات (فراغ) ثابت
  { أرجع &tuple_Variation_معلومات; }

  منطقي decompile_tuple_variations (بینښه axis_شمېر,
                                   بینښه ټکی_شمېر,
                                   hb_blob_t *blob,
                                   منطقي is_gvar,
                                   ثابت hb_map_t *axes_old_شاخص_نښان_map,
                                   Tuple_Variation_معلومات<>::tuple_variations_t& tuple_variations /*بهر*/) ثابت
  {
    hb_ویکټور_t<بینښه> shared_indices;
    Tuple_Variation_معلومات<>::tuple_iterator_t iterator;
    hb_بایټونه_t var_معلومات_بایټونه = blob->as_بایټونه ().sub_array (4);
    إذا (!Tuple_Variation_معلومات<>::get_tuple_iterator (var_معلومات_بایټونه, axis_شمېر, هذا,
                                                 shared_indices, &iterator))
      أرجع خطأ;

    أرجع tuple_Variation_معلومات.decompile_tuple_variations (ټکی_شمېر, is_gvar, iterator,
                                                          axes_old_شاخص_نښان_map,
                                                          shared_indices,
                                                          hb_array<ثابت F2DOT14> (),
                                                          tuple_variations);
  }

  ساكن منطقي calculate_cvt_deltas (بینښه axis_شمېر,
                                    hb_array_t<صحيح> coords,
                                    بینښه عدد_cvt_توکی,
                                    ثابت Tuple_Variation_معلومات<> *tuple_var_معلومات,
                                    ثابت فراغ *بنسټ,
                                    بینښه var_معلومات_اوږدتیا,
                                    hb_ویکټور_t<عائم>& cvt_deltas /*بهر*/)
  {
    إذا (!coords) أرجع ریښتیا;
    hb_ویکټور_t<بینښه> shared_indices;
    Tuple_Variation_معلومات<>::tuple_iterator_t iterator;
    hb_بایټونه_t var_معلومات_بایټونه = hb_بایټونه_t (reinterpret_cast<ثابت حرف*> (tuple_var_معلومات), var_معلومات_اوږدتیا);
    إذا (!Tuple_Variation_معلومات<>::get_tuple_iterator (var_معلومات_بایټونه, axis_شمېر, بنسټ,
                                                 shared_indices, &iterator))
      أرجع ریښتیا; /* isn't applied at all */

    hb_array_t<ثابت F2DOT14> shared_tuples = hb_array<F2DOT14> ();
    hb_ویکټور_t<بینښه> شخصي_indices;
    hb_ویکټور_t<صحيح> unpacked_deltas;

    افعل
    {
      عائم scalar = iterator.current_tuple->calculate_scalar (coords, axis_شمېر, shared_tuples);
      إذا (scalar == 0.f) استمر;
      ثابت HBUINT8 *p = iterator.get_serialized_معلومات ();
      بینښه صحيح اوږدتیا = iterator.current_tuple->get_معلومات_اندازه ();
      إذا (unlikely (!iterator.var_معلومات_بایټونه.check_range (p, اوږدتیا)))
        أرجع خطأ;

      ثابت HBUINT8 *پای_ټکی = p + اوږدتیا;

      منطقي has_شخصي_points = iterator.current_tuple->has_شخصي_points ();
      إذا (has_شخصي_points &&
          !Tuple_Variation_معلومات<>::decompile_points (p, شخصي_indices, پای_ټکی))
        أرجع خطأ;
      ثابت hb_ویکټور_t<بینښه صحيح> &indices = has_شخصي_points ? شخصي_indices : shared_indices;

      منطقي apply_منزل_all = (indices.اوږدتیا == 0);
      بینښه عدد_deltas = apply_منزل_all ? عدد_cvt_توکی : indices.اوږدتیا;
      إذا (unlikely (!unpacked_deltas.resize_dirty  (عدد_deltas))) أرجع خطأ;
      إذا (unlikely (!Tuple_Variation_معلومات<>::decompile_deltas (p, unpacked_deltas, پای_ټکی))) أرجع خطأ;

      لكل (بینښه صحيح i = 0; i < عدد_deltas; i++)
      {
        بینښه صحيح idx = apply_منزل_all ? i : indices[i];
        إذا (unlikely (idx >= عدد_cvt_توکی)) استمر;
        إذا (scalar != 1.0f) cvt_deltas[idx] += unpacked_deltas[i] * scalar ;
        وإلا cvt_deltas[idx] += unpacked_deltas[i];
      }
    } طالما (iterator.move_منزل_بل ());

    أرجع ریښتیا;
  }
  
  منطقي serialize (hb_serialize_context_t *c,
                  Tuple_Variation_معلومات<>::tuple_variations_t& tuple_variations) ثابت
  {
    TRACE_SERIALIZE (هذا);
    إذا (!tuple_variations) return_trace (خطأ);
    إذا (unlikely (!c->embed (نسخه))) return_trace (خطأ);

    return_trace (tuple_Variation_معلومات.serialize (c, خطأ, tuple_variations));
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    إذا (c->plan->all_axes_pinned)
      return_trace (خطأ);

    OT::Tuple_Variation_معلومات<>::tuple_variations_t tuple_variations;
    بینښه axis_شمېر = c->plan->axes_old_شاخص_نښان_map.get_population ();

    ثابت hb_نښان_t cvt = HB_نښان('c','v','t',' ');
    hb_blob_t *cvt_blob = hb_face_reference_جدول (c->plan->سرچینه, cvt);
    بینښه ټکی_شمېر = hb_blob_get_اوږدتیا (cvt_blob) / FWORD::static_اندازه;
    hb_blob_destroy (cvt_blob);

    إذا (!decompile_tuple_variations (axis_شمېر, ټکی_شمېر,
                                     c->سرچینه_blob, خطأ,
                                     &(c->plan->axes_old_شاخص_نښان_map),
                                     tuple_variations))
      return_trace (خطأ);

    optimize_scratch_t scratch;
    إذا (!tuple_variations.instantiate (c->plan->axes_location, c->plan->axes_triple_distances, scratch))
      return_trace (خطأ);

    إذا (!tuple_variations.compile_بایټونه (c->plan->axes_شاخص_map, c->plan->axes_old_شاخص_نښان_map,
                                         خطأ /* do not use shared points */))
      return_trace (خطأ);

    return_trace (serialize (c->serializer, tuple_variations));
  }

  ساكن منطقي جمع_کړه_cvt_and_apply_deltas (hb_subset_plan_t *plan,
                                        ثابت Tuple_Variation_معلومات<> *tuple_var_معلومات,
                                        ثابت فراغ *بنسټ)
  {
    ثابت hb_نښان_t cvt = HB_نښان('c','v','t',' ');
    hb_blob_t *cvt_blob = hb_face_reference_جدول (plan->سرچینه, cvt);
    hb_blob_t *cvt_اولي_blob = hb_blob_copy_writable_or_fail (cvt_blob);
    hb_blob_destroy (cvt_blob);
  
    إذا (unlikely (!cvt_اولي_blob))
      أرجع خطأ;
 
    بینښه cvt_blob_اوږدتیا = hb_blob_get_اوږدتیا (cvt_اولي_blob);
    بینښه عدد_cvt_توکی = cvt_blob_اوږدتیا / FWORD::static_اندازه;

    hb_ویکټور_t<عائم> cvt_deltas;
    إذا (unlikely (!cvt_deltas.resize (عدد_cvt_توکی)))
    {
      hb_blob_destroy (cvt_اولي_blob);
      أرجع خطأ;
    }

    /* Bound the tuple variation data by the actual cvar table length rather
     * than trusting tupleVarCount to walk the headers; an inflated count would
     * otherwise run get_size() off the end of the table. */
    hb_blob_t *cvar_blob = hb_face_reference_جدول (plan->سرچینه, cvar::جدول_نښان);
    بینښه cvar_اوږدتیا = hb_blob_get_اوږدتیا (cvar_blob);
    hb_blob_destroy (cvar_blob);
    بینښه var_معلومات_دننه_واټن = (بینښه) ((ثابت حرف *) tuple_var_معلومات - (ثابت حرف *) بنسټ);
    بینښه var_معلومات_اوږدتیا = var_معلومات_دننه_واټن < cvar_اوږدتیا ? cvar_اوږدتیا - var_معلومات_دننه_واټن : 0;

    إذا (!calculate_cvt_deltas (plan->normalized_coords.اوږدتیا, plan->normalized_coords.as_array (),
                               عدد_cvt_توکی, tuple_var_معلومات, بنسټ, var_معلومات_اوږدتیا, cvt_deltas))
    {
      hb_blob_destroy (cvt_اولي_blob);
      أرجع خطأ;
    }

    FWORD *cvt_اولي = (FWORD *) hb_blob_get_معلومات_writable (cvt_اولي_blob, تش_اشاره);
    لكل (بینښه i = 0; i < عدد_cvt_توکی; i++)
      cvt_اولي[i] += (صحيح) roundf (cvt_deltas[i]);
    
    منطقي success = plan->جمع_کړه_جدول (cvt, cvt_اولي_blob);
    hb_blob_destroy (cvt_اولي_blob);
    أرجع success;
  }

  محمي:
  Fixed_نسخه<>نسخه;		/* Version of the CVT variation table
					 * initially set to 0x00010000u */
  Tuple_Variation_معلومات<> tuple_Variation_معلومات; /* TupleVariationDate for cvar table */
  عام:
  DEFINE_اندازه_MIN (8);
};

} /* namespace OT */


#د_که_پای /* HB_OT_VAR_CVAR_TABLE_HH */
