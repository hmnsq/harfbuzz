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

#جيڪڏهن_وصف_نه_ٿيل HB_OT_VAR_MVAR_جدول_HH
#وصف_ڪريو HB_OT_VAR_MVAR_جدول_HH

#ضمن "hb-ot-var-common.hh"


نطاق OT {


بنية Variation_قدر_رڪارڊ
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
    خودڪار *ٻاهريون = c->serializer->embed (*هذا);
    إذا (unlikely (!ٻاهريون)) return_trace (خطأ);

    hb_codepoint_t *new_idx;
    return_trace (c->serializer->check_سونپيو (ٻاهريون->varIdx,
                                               (varidx_map.has (varIdx, &new_idx)) ? *new_idx : HB_OT_LAYOUT_NO_VARIATIONS_اشاريو,
                                               HB_SERIALIZE_ERROR_INT_حد_کان_مٿي));
  }

  عام:
  ٽيگ		قدر_ٽيگ;	/* Four-byte tag identifying a font-wide measure. */
  VarIdx	varIdx;		/* Outer/inner index into ItemVariationStore item. */

  عام:
  DEFINE_سائيز_STATIC (8);
};


/*
 * MVAR -- Metrics Variations
 * https://docs.microsoft.com/en-us/typography/opentype/spec/mvar
 */
#وصف_ڪريو HB_OT_ٽيگ_MVAR HB_ٽيگ('M','V','A','R')

بنية MVAR
{
  ساكن constexpr hb_ٽيگ_t جدول_ٽيگ = HB_OT_ٽيگ_MVAR;

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (نسخو.sanitize (c) &&
		  hb_barrier () &&
		  likely (نسخو.major == 1) &&
		  c->check_struct (هذا) &&
		  hb_barrier () &&
		  قدر_رڪارڊ_سائيز >= Variation_قدر_رڪارڊ::static_سائيز &&
		  varStore.sanitize (c, هذا) &&
		  c->check_range (قدرون_Z.arrayZ,
				  قدر_رڪارڊ_ڳڻپ,
				  قدر_رڪارڊ_سائيز));
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
#جيڪڏهن_وصف_ٿيل HB_NO_VAR
    return_trace (خطأ);
#جيڪڏهن_پورو

    إذا (c->plan->all_axes_pinned)
      return_trace (خطأ);

    MVAR *ٻاهريون = c->serializer->شروعات_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (ٻاهريون))) return_trace (خطأ);
    ٻاهريون->نسخو = نسخو;
    ٻاهريون->reserved = reserved;
    ٻاهريون->قدر_رڪارڊ_سائيز = قدر_رڪارڊ_سائيز;
    ٻاهريون->قدر_رڪارڊ_ڳڻپ = قدر_رڪارڊ_ڳڻپ;

    شي_variations_t شي_vars;
    ثابت شي_Variation_Store& src_var_store = هذا+varStore;

    إذا (!شي_vars.instantiate (src_var_store, c->plan))
      return_trace (خطأ);

    /* serialize varstore */
    إذا (!ٻاهريون->varStore.serialize_serialize (c->serializer, شي_vars.has_long_لفظ (),
                                            c->plan->axis_ٽيگ_فهرست,
                                            شي_vars.get_region_list (),
                                            شي_vars.get_vardata_encodings ()))
      return_trace (خطأ);

    /* serialize value records array */
    بنانشان قدر_rec_ڳڻپ = قدر_رڪارڊ_ڳڻپ;
    ثابت Variation_قدر_رڪارڊ *رڪارڊ = reinterpret_cast<ثابت Variation_قدر_رڪارڊ*> (قدرون_Z.arrayZ);
    لكل (بنانشان i = 0; i < قدر_rec_ڳڻپ; i++)
    {
      إذا (!رڪارڊ->subset (c, شي_vars.get_varidx_map ())) return_trace (خطأ);
      رڪارڊ++;
    }
    return_trace (سچ);
  }

  عائم get_var (hb_ٽيگ_t ٽيگ_2,
		 ثابت صحيح *coords, بنانشان صحيح coord_ڳڻپ) ثابت
  {
    ثابت Variation_قدر_رڪارڊ *رڪارڊ;
    رڪارڊ = (Variation_قدر_رڪارڊ *) hb_bsearch (ٽيگ_2,
						  (ثابت Variation_قدر_رڪارڊ *)
						    (ثابت HBUINT8 *) قدرون_Z,
						  قدر_رڪارڊ_ڳڻپ, قدر_رڪارڊ_سائيز,
						  ٽيگ_compare);
    إذا (!رڪارڊ)
      أرجع 0.;

    أرجع (هذا+varStore).get_delta (رڪارڊ->varIdx, coords, coord_ڳڻپ);
  }

محمي:
  ساكن صحيح ٽيگ_compare (ثابت فراغ *pa, ثابت فراغ *pb)
  {
    ثابت hb_ٽيگ_t *a = (ثابت hb_ٽيگ_t *) pa;
    ثابت ٽيگ *b = (ثابت ٽيگ *) pb;
    أرجع b->cmp (*a);
  }

  محمي:
  Fixed_نسخو<>نسخو;	/* Version of the metrics variation table
				 * initially set to 0x00010000u */
  HBUINT16	reserved;	/* Not used; set to 0. */
  HBUINT16	قدر_رڪارڊ_سائيز;/* The size in bytes of each value record —
				 * must be greater than zero. */
  HBUINT16	قدر_رڪارڊ_ڳڻپ;/* The number of value records — may be zero. */
  Offset16_منزل<شي_Variation_Store>
		varStore;	/* Offset to item variation store table. */
  Unsized_Array_جو<HBUINT8>
		قدرون_Z;	/* Array of value records. The records must be
				 * in binary order of their valueTag field. */

  عام:
  DEFINE_سائيز_ARRAY (12, قدرون_Z);
};

} /* namespace OT */


#وصف_ڪريو HB_جوڙيو_MVAR_VAR(ٽيگ_2, ميدان) \
       c->serializer->check_سونپيو (جدول->ميدان, \
				    roundf (جدول->ميدان + \
					    MVAR.get_var (ٽيگ_2, \
							  c->plan->normalized_coords.arrayZ, \
							  c->plan->normalized_coords.ڊيگهائي)), \
				    HB_SERIALIZE_ERROR_INT_حد_کان_مٿي)


#جيڪڏهن_پورو /* HB_OT_VAR_MVAR_TABLE_HH */
