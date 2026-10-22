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

#اگر_غیر_تعریف_شدہ HB_OT_VAR_CVAR_جدول_HH
#تعریف_کرو HB_OT_VAR_CVAR_جدول_HH

#شامل "hb-ot-var-common.hh"
#شامل "hb-ot-var-fvar-table.hh"


نامخلا OT {
/*
 * cvar -- control value table (CVT) Variations
 * https://docs.microsoft.com/en-us/typography/opentype/spec/cvar
 */
#تعریف_کرو HB_OT_لیبل_cvar HB_لیبل('c','v','a','r')

ساخت cvar
{
  ساکن constexpr hb_لیبل_t جدول_لیبل = HB_OT_لیبل_cvar;

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ) &&
		  hb_barrier () &&
		  likely (نسخہ.major == 1) &&
		  tuple_Variation_ڈیٹا.sanitize (c));
  }

  ثابت Tuple_Variation_ڈیٹا<>* get_tuple_var_ڈیٹا (خالی) ثابت
  { واپس &tuple_Variation_ڈیٹا; }

  منطقی decompile_tuple_variations (غیرعلامتی axis_گنتی,
                                   غیرعلامتی نقطہ_گنتی,
                                   hb_blob_t *blob,
                                   منطقی is_gvar,
                                   ثابت hb_map_t *axes_old_اشاریہ_لیبل_map,
                                   Tuple_Variation_ڈیٹا<>::tuple_variations_t& tuple_variations /*برآمد*/) ثابت
  {
    hb_سمتیہ_t<غیرعلامتی> shared_indices;
    Tuple_Variation_ڈیٹا<>::tuple_iterator_t iterator;
    hb_بائٹیں_t var_ڈیٹا_بائٹیں = blob->as_بائٹیں ().sub_array (4);
    اگر (!Tuple_Variation_ڈیٹا<>::get_tuple_iterator (var_ڈیٹا_بائٹیں, axis_گنتی, یہ,
                                                 shared_indices, &iterator))
      واپس جھوٹ;

    واپس tuple_Variation_ڈیٹا.decompile_tuple_variations (نقطہ_گنتی, is_gvar, iterator,
                                                          axes_old_اشاریہ_لیبل_map,
                                                          shared_indices,
                                                          hb_array<ثابت F2DOT14> (),
                                                          tuple_variations);
  }

  ساکن منطقی calculate_cvt_deltas (غیرعلامتی axis_گنتی,
                                    hb_array_t<صحیح> coords,
                                    غیرعلامتی عدد_cvt_شے,
                                    ثابت Tuple_Variation_ڈیٹا<> *tuple_var_ڈیٹا,
                                    ثابت خالی *بنیاد,
                                    غیرعلامتی var_ڈیٹا_لمبائی,
                                    hb_سمتیہ_t<اعشاری>& cvt_deltas /*برآمد*/)
  {
    اگر (!coords) واپس سچ;
    hb_سمتیہ_t<غیرعلامتی> shared_indices;
    Tuple_Variation_ڈیٹا<>::tuple_iterator_t iterator;
    hb_بائٹیں_t var_ڈیٹا_بائٹیں = hb_بائٹیں_t (reinterpret_cast<ثابت حرف*> (tuple_var_ڈیٹا), var_ڈیٹا_لمبائی);
    اگر (!Tuple_Variation_ڈیٹا<>::get_tuple_iterator (var_ڈیٹا_بائٹیں, axis_گنتی, بنیاد,
                                                 shared_indices, &iterator))
      واپس سچ; /* isn't applied at all */

    hb_array_t<ثابت F2DOT14> shared_tuples = hb_array<F2DOT14> ();
    hb_سمتیہ_t<غیرعلامتی> نجی_indices;
    hb_سمتیہ_t<صحیح> unpacked_deltas;

    کرو
    {
      اعشاری scalar = iterator.current_tuple->calculate_scalar (coords, axis_گنتی, shared_tuples);
      اگر (scalar == 0.f) جاری;
      ثابت HBUINT8 *p = iterator.get_serialized_ڈیٹا ();
      غیرعلامتی صحیح لمبائی = iterator.current_tuple->get_ڈیٹا_سائز ();
      اگر (unlikely (!iterator.var_ڈیٹا_بائٹیں.check_range (p, لمبائی)))
        واپس جھوٹ;

      ثابت HBUINT8 *انتہا = p + لمبائی;

      منطقی has_نجی_points = iterator.current_tuple->has_نجی_points ();
      اگر (has_نجی_points &&
          !Tuple_Variation_ڈیٹا<>::decompile_points (p, نجی_indices, انتہا))
        واپس جھوٹ;
      ثابت hb_سمتیہ_t<غیرعلامتی صحیح> &indices = has_نجی_points ? نجی_indices : shared_indices;

      منطقی apply_مقصد_all = (indices.لمبائی == 0);
      غیرعلامتی عدد_deltas = apply_مقصد_all ? عدد_cvt_شے : indices.لمبائی;
      اگر (unlikely (!unpacked_deltas.resize_dirty  (عدد_deltas))) واپس جھوٹ;
      اگر (unlikely (!Tuple_Variation_ڈیٹا<>::decompile_deltas (p, unpacked_deltas, انتہا))) واپس جھوٹ;

      برائے (غیرعلامتی صحیح i = 0; i < عدد_deltas; i++)
      {
        غیرعلامتی صحیح idx = apply_مقصد_all ? i : indices[i];
        اگر (unlikely (idx >= عدد_cvt_شے)) جاری;
        اگر (scalar != 1.0f) cvt_deltas[idx] += unpacked_deltas[i] * scalar ;
        ورنہ cvt_deltas[idx] += unpacked_deltas[i];
      }
    } جب (iterator.move_مقصد_اگلا ());

    واپس سچ;
  }
  
  منطقی serialize (hb_serialize_context_t *c,
                  Tuple_Variation_ڈیٹا<>::tuple_variations_t& tuple_variations) ثابت
  {
    TRACE_SERIALIZE (یہ);
    اگر (!tuple_variations) return_trace (جھوٹ);
    اگر (unlikely (!c->embed (نسخہ))) return_trace (جھوٹ);

    return_trace (tuple_Variation_ڈیٹا.serialize (c, جھوٹ, tuple_variations));
  }

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (یہ);
    اگر (c->plan->all_axes_pinned)
      return_trace (جھوٹ);

    OT::Tuple_Variation_ڈیٹا<>::tuple_variations_t tuple_variations;
    غیرعلامتی axis_گنتی = c->plan->axes_old_اشاریہ_لیبل_map.get_population ();

    ثابت hb_لیبل_t cvt = HB_لیبل('c','v','t',' ');
    hb_blob_t *cvt_blob = hb_face_reference_جدول (c->plan->ماخذ, cvt);
    غیرعلامتی نقطہ_گنتی = hb_blob_get_لمبائی (cvt_blob) / FWORD::static_سائز;
    hb_blob_destroy (cvt_blob);

    اگر (!decompile_tuple_variations (axis_گنتی, نقطہ_گنتی,
                                     c->ماخذ_blob, جھوٹ,
                                     &(c->plan->axes_old_اشاریہ_لیبل_map),
                                     tuple_variations))
      return_trace (جھوٹ);

    optimize_scratch_t scratch;
    اگر (!tuple_variations.instantiate (c->plan->axes_location, c->plan->axes_triple_distances, scratch))
      return_trace (جھوٹ);

    اگر (!tuple_variations.compile_بائٹیں (c->plan->axes_اشاریہ_map, c->plan->axes_old_اشاریہ_لیبل_map,
                                         جھوٹ /* do not use shared points */))
      return_trace (جھوٹ);

    return_trace (serialize (c->serializer, tuple_variations));
  }

  ساکن منطقی جوڑو_cvt_and_apply_deltas (hb_subset_plan_t *plan,
                                        ثابت Tuple_Variation_ڈیٹا<> *tuple_var_ڈیٹا,
                                        ثابت خالی *بنیاد)
  {
    ثابت hb_لیبل_t cvt = HB_لیبل('c','v','t',' ');
    hb_blob_t *cvt_blob = hb_face_reference_جدول (plan->ماخذ, cvt);
    hb_blob_t *cvt_مفرد_blob = hb_blob_copy_writable_or_fail (cvt_blob);
    hb_blob_destroy (cvt_blob);
  
    اگر (unlikely (!cvt_مفرد_blob))
      واپس جھوٹ;
 
    غیرعلامتی cvt_blob_لمبائی = hb_blob_get_لمبائی (cvt_مفرد_blob);
    غیرعلامتی عدد_cvt_شے = cvt_blob_لمبائی / FWORD::static_سائز;

    hb_سمتیہ_t<اعشاری> cvt_deltas;
    اگر (unlikely (!cvt_deltas.resize (عدد_cvt_شے)))
    {
      hb_blob_destroy (cvt_مفرد_blob);
      واپس جھوٹ;
    }

    /* Bound the tuple variation data by the actual cvar table length rather
     * than trusting tupleVarCount to walk the headers; an inflated count would
     * otherwise run get_size() off the end of the table. */
    hb_blob_t *cvar_blob = hb_face_reference_جدول (plan->ماخذ, cvar::جدول_لیبل);
    غیرعلامتی cvar_لمبائی = hb_blob_get_لمبائی (cvar_blob);
    hb_blob_destroy (cvar_blob);
    غیرعلامتی var_ڈیٹا_انحراف = (غیرعلامتی) ((ثابت حرف *) tuple_var_ڈیٹا - (ثابت حرف *) بنیاد);
    غیرعلامتی var_ڈیٹا_لمبائی = var_ڈیٹا_انحراف < cvar_لمبائی ? cvar_لمبائی - var_ڈیٹا_انحراف : 0;

    اگر (!calculate_cvt_deltas (plan->normalized_coords.لمبائی, plan->normalized_coords.as_array (),
                               عدد_cvt_شے, tuple_var_ڈیٹا, بنیاد, var_ڈیٹا_لمبائی, cvt_deltas))
    {
      hb_blob_destroy (cvt_مفرد_blob);
      واپس جھوٹ;
    }

    FWORD *cvt_مفرد = (FWORD *) hb_blob_get_ڈیٹا_writable (cvt_مفرد_blob, خالی_اشاریہ);
    برائے (غیرعلامتی i = 0; i < عدد_cvt_شے; i++)
      cvt_مفرد[i] += (صحیح) roundf (cvt_deltas[i]);
    
    منطقی success = plan->جوڑو_جدول (cvt, cvt_مفرد_blob);
    hb_blob_destroy (cvt_مفرد_blob);
    واپس success;
  }

  محفوظ:
  Fixed_نسخہ<>نسخہ;		/* Version of the CVT variation table
					 * initially set to 0x00010000u */
  Tuple_Variation_ڈیٹا<> tuple_Variation_ڈیٹا; /* TupleVariationDate for cvar table */
  عوامی:
  DEFINE_سائز_MIN (8);
};

} /* namespace OT */


#اگر_ختم /* HB_OT_VAR_CVAR_TABLE_HH */
