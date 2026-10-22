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

#که_نه_تعریف_شوی HB_OT_VAR_MVAR_جدول_HH
#تعریف_کړه HB_OT_VAR_MVAR_جدول_HH

#ضمن "hb-ot-var-common.hh"


نطاق OT {


بنية Variation_ارزښت_ریکارډ
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
    خپلکاری *بهر = c->serializer->embed (*هذا);
    إذا (unlikely (!بهر)) return_trace (خطأ);

    hb_codepoint_t *new_idx;
    return_trace (c->serializer->check_ټاکل (بهر->varIdx,
                                               (varidx_map.has (varIdx, &new_idx)) ? *new_idx : HB_OT_LAYOUT_NO_VARIATIONS_شاخص,
                                               HB_SERIALIZE_ERROR_INT_ډکېدنه));
  }

  عام:
  نښان		ارزښت_نښان;	/* Four-byte tag identifying a font-wide measure. */
  VarIdx	varIdx;		/* Outer/inner index into ItemVariationStore item. */

  عام:
  DEFINE_اندازه_STATIC (8);
};


/*
 * MVAR -- Metrics Variations
 * https://docs.microsoft.com/en-us/typography/opentype/spec/mvar
 */
#تعریف_کړه HB_OT_نښان_MVAR HB_نښان('M','V','A','R')

بنية MVAR
{
  ساكن constexpr hb_نښان_t جدول_نښان = HB_OT_نښان_MVAR;

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (نسخه.sanitize (c) &&
		  hb_barrier () &&
		  likely (نسخه.major == 1) &&
		  c->check_struct (هذا) &&
		  hb_barrier () &&
		  ارزښت_ریکارډ_اندازه >= Variation_ارزښت_ریکارډ::static_اندازه &&
		  varStore.sanitize (c, هذا) &&
		  c->check_range (ارزښتونه_Z.arrayZ,
				  ارزښت_ریکارډ_شمېر,
				  ارزښت_ریکارډ_اندازه));
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
#که_تعریف_شوی HB_NO_VAR
    return_trace (خطأ);
#د_که_پای

    إذا (c->plan->all_axes_pinned)
      return_trace (خطأ);

    MVAR *بهر = c->serializer->پیل_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (بهر))) return_trace (خطأ);
    بهر->نسخه = نسخه;
    بهر->reserved = reserved;
    بهر->ارزښت_ریکارډ_اندازه = ارزښت_ریکارډ_اندازه;
    بهر->ارزښت_ریکارډ_شمېر = ارزښت_ریکارډ_شمېر;

    توکی_variations_t توکی_vars;
    ثابت توکی_Variation_Store& src_var_store = هذا+varStore;

    إذا (!توکی_vars.instantiate (src_var_store, c->plan))
      return_trace (خطأ);

    /* serialize varstore */
    إذا (!بهر->varStore.serialize_serialize (c->serializer, توکی_vars.has_long_کلمه (),
                                            c->plan->axis_نښانونه,
                                            توکی_vars.get_region_list (),
                                            توکی_vars.get_vardata_encodings ()))
      return_trace (خطأ);

    /* serialize value records array */
    بینښه ارزښت_rec_شمېر = ارزښت_ریکارډ_شمېر;
    ثابت Variation_ارزښت_ریکارډ *ریکارډ = reinterpret_cast<ثابت Variation_ارزښت_ریکارډ*> (ارزښتونه_Z.arrayZ);
    لكل (بینښه i = 0; i < ارزښت_rec_شمېر; i++)
    {
      إذا (!ریکارډ->subset (c, توکی_vars.get_varidx_map ())) return_trace (خطأ);
      ریکارډ++;
    }
    return_trace (ریښتیا);
  }

  عائم get_var (hb_نښان_t نښان_2,
		 ثابت صحيح *coords, بینښه صحيح coord_شمېر) ثابت
  {
    ثابت Variation_ارزښت_ریکارډ *ریکارډ;
    ریکارډ = (Variation_ارزښت_ریکارډ *) hb_bsearch (نښان_2,
						  (ثابت Variation_ارزښت_ریکارډ *)
						    (ثابت HBUINT8 *) ارزښتونه_Z,
						  ارزښت_ریکارډ_شمېر, ارزښت_ریکارډ_اندازه,
						  نښان_compare);
    إذا (!ریکارډ)
      أرجع 0.;

    أرجع (هذا+varStore).get_delta (ریکارډ->varIdx, coords, coord_شمېر);
  }

محمي:
  ساكن صحيح نښان_compare (ثابت فراغ *pa, ثابت فراغ *pb)
  {
    ثابت hb_نښان_t *a = (ثابت hb_نښان_t *) pa;
    ثابت نښان *b = (ثابت نښان *) pb;
    أرجع b->cmp (*a);
  }

  محمي:
  Fixed_نسخه<>نسخه;	/* Version of the metrics variation table
				 * initially set to 0x00010000u */
  HBUINT16	reserved;	/* Not used; set to 0. */
  HBUINT16	ارزښت_ریکارډ_اندازه;/* The size in bytes of each value record —
				 * must be greater than zero. */
  HBUINT16	ارزښت_ریکارډ_شمېر;/* The number of value records — may be zero. */
  Offset16_منزل<توکی_Variation_Store>
		varStore;	/* Offset to item variation store table. */
  Unsized_Array_د<HBUINT8>
		ارزښتونه_Z;	/* Array of value records. The records must be
				 * in binary order of their valueTag field. */

  عام:
  DEFINE_اندازه_ARRAY (12, ارزښتونه_Z);
};

} /* namespace OT */


#تعریف_کړه HB_جمع_کړه_MVAR_VAR(نښان_2, ډګر) \
       c->serializer->check_ټاکل (جدول->ډګر, \
				    roundf (جدول->ډګر + \
					    MVAR.get_var (نښان_2, \
							  c->plan->normalized_coords.arrayZ, \
							  c->plan->normalized_coords.اوږدتیا)), \
				    HB_SERIALIZE_ERROR_INT_ډکېدنه)


#د_که_پای /* HB_OT_VAR_MVAR_TABLE_HH */
