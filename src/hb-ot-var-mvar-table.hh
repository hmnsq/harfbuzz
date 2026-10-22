/*
 * Copyright © 2017  Google, Inc.
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
 * Google Author(s): Behdad Esfahbod
 */

#اگر_تعریف_نشده HB_OT_VAR_MVAR_جدول_HH
#تعریف_کن HB_OT_VAR_MVAR_جدول_HH

#ضمن "hb-ot-var-common.hh"


نطاق OT {


بنية Variation_مقدار_رکورد
{
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  منطقي subset (hb_subset_context_t *c,
               ثابت hb_map_t& varidx_map) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *بیرون = c->serializer->embed (*هذا);
    إذا (unlikely (!بیرون)) return_trace (خطأ);

    hb_codepoint_t *new_idx;
    return_trace (c->serializer->check_تخصیص_بده (بیرون->varIdx,
                                               (varidx_map.has (varIdx, &new_idx)) ? *new_idx : HB_OT_LAYOUT_NO_VARIATIONS_شاخص,
                                               HB_SERIALIZE_ERROR_INT_سرریز));
  }

  عام:
  برچسب		مقدار_برچسب;	/* Four-byte tag identifying a font-wide measure. */
  VarIdx	varIdx;		/* Outer/inner index into ItemVariationStore item. */

  عام:
  DEFINE_اندازه_STATIC (8);
};


/*
 * MVAR -- Metrics Variations
 * https://docs.microsoft.com/en-us/typography/opentype/spec/mvar
 */
#تعریف_کن HB_OT_برچسب_MVAR HB_برچسب('M','V','A','R')

بنية MVAR
{
  ساكن constexpr hb_برچسب_t جدول_برچسب = HB_OT_برچسب_MVAR;

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (نسخه.sanitize (c) &&
		  hb_barrier () &&
		  likely (نسخه.major == 1) &&
		  c->check_struct (هذا) &&
		  hb_barrier () &&
		  مقدار_رکورد_اندازه >= Variation_مقدار_رکورد::static_اندازه &&
		  varStore.sanitize (c, هذا) &&
		  c->check_range (مقادیر_Z.arrayZ,
				  مقدار_رکورد_شمار,
				  مقدار_رکورد_اندازه));
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
#اگر_تعریف_شده HB_NO_VAR
    return_trace (خطأ);
#پایان_اگر

    إذا (c->plan->all_axes_pinned)
      return_trace (خطأ);

    MVAR *بیرون = c->serializer->شروع_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (بیرون))) return_trace (خطأ);
    بیرون->نسخه = نسخه;
    بیرون->reserved = reserved;
    بیرون->مقدار_رکورد_اندازه = مقدار_رکورد_اندازه;
    بیرون->مقدار_رکورد_شمار = مقدار_رکورد_شمار;

    قلم_variations_t قلم_vars;
    ثابت قلم_Variation_Store& src_var_store = هذا+varStore;

    إذا (!قلم_vars.instantiate (src_var_store, c->plan))
      return_trace (خطأ);

    /* serialize varstore */
    إذا (!بیرون->varStore.serialize_serialize (c->serializer, قلم_vars.has_long_کلمه (),
                                            c->plan->axis_برچسبها,
                                            قلم_vars.get_region_list (),
                                            قلم_vars.get_vardata_encodings ()))
      return_trace (خطأ);

    /* serialize value records array */
    بیعلامت مقدار_rec_شمار = مقدار_رکورد_شمار;
    ثابت Variation_مقدار_رکورد *رکورد = reinterpret_cast<ثابت Variation_مقدار_رکورد*> (مقادیر_Z.arrayZ);
    لكل (بیعلامت i = 0; i < مقدار_rec_شمار; i++)
    {
      إذا (!رکورد->subset (c, قلم_vars.get_varidx_map ())) return_trace (خطأ);
      رکورد++;
    }
    return_trace (درست);
  }

  عائم get_var (hb_برچسب_t برچسب_2,
		 ثابت صحيح *coords, بیعلامت صحيح coord_شمار) ثابت
  {
    ثابت Variation_مقدار_رکورد *رکورد;
    رکورد = (Variation_مقدار_رکورد *) hb_bsearch (برچسب_2,
						  (ثابت Variation_مقدار_رکورد *)
						    (ثابت HBUINT8 *) مقادیر_Z,
						  مقدار_رکورد_شمار, مقدار_رکورد_اندازه,
						  برچسب_compare);
    إذا (!رکورد)
      أرجع 0.;

    أرجع (هذا+varStore).get_delta (رکورد->varIdx, coords, coord_شمار);
  }

محمي:
  ساكن صحيح برچسب_compare (ثابت فراغ *pa, ثابت فراغ *pb)
  {
    ثابت hb_برچسب_t *a = (ثابت hb_برچسب_t *) pa;
    ثابت برچسب *b = (ثابت برچسب *) pb;
    أرجع b->cmp (*a);
  }

  محمي:
  Fixed_نسخه<>نسخه;	/* Version of the metrics variation table
				 * initially set to 0x00010000u */
  HBUINT16	reserved;	/* Not used; set to 0. */
  HBUINT16	مقدار_رکورد_اندازه;/* The size in bytes of each value record —
				 * must be greater than zero. */
  HBUINT16	مقدار_رکورد_شمار;/* The number of value records — may be zero. */
  Offset16_مقصد<قلم_Variation_Store>
		varStore;	/* Offset to item variation store table. */
  Unsized_Array_از<HBUINT8>
		مقادیر_Z;	/* Array of value records. The records must be
				 * in binary order of their valueTag field. */

  عام:
  DEFINE_اندازه_ARRAY (12, مقادیر_Z);
};

} /* namespace OT */


#تعریف_کن HB_جمع_کن_MVAR_VAR(برچسب_2, فیلد) \
       c->serializer->check_تخصیص_بده (جدول->فیلد, \
				    roundf (جدول->فیلد + \
					    MVAR.get_var (برچسب_2, \
							  c->plan->normalized_coords.arrayZ, \
							  c->plan->normalized_coords.درازی)), \
				    HB_SERIALIZE_ERROR_INT_سرریز)


#پایان_اگر /* HB_OT_VAR_MVAR_TABLE_HH */
