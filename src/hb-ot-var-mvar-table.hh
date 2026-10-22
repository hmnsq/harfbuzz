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

#جے_بنا_تعریف HB_OT_VAR_MVAR_جدول_HH
#تعریف_کرو HB_OT_VAR_MVAR_جدول_HH

#ضمن "hb-ot-var-common.hh"


نطاق OT {


بنية Variation_قدر_ریکارڈ
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
    خودکار *باہرلا = c->serializer->embed (*هذا);
    إذا (unlikely (!باہرلا)) return_trace (خطأ);

    hb_codepoint_t *new_idx;
    return_trace (c->serializer->check_سونپو (باہرلا->varIdx,
                                               (varidx_map.has (varIdx, &new_idx)) ? *new_idx : HB_OT_LAYOUT_NO_VARIATIONS_اشاریہ,
                                               HB_SERIALIZE_ERROR_INT_حد_توں_ودھ));
  }

  عام:
  ٹیگ		قدر_ٹیگ;	/* Four-byte tag identifying a font-wide measure. */
  VarIdx	varIdx;		/* Outer/inner index into ItemVariationStore item. */

  عام:
  DEFINE_ناپ_STATIC (8);
};


/*
 * MVAR -- Metrics Variations
 * https://docs.microsoft.com/en-us/typography/opentype/spec/mvar
 */
#تعریف_کرو HB_OT_ٹیگ_MVAR HB_ٹیگ('M','V','A','R')

بنية MVAR
{
  ساكن constexpr hb_ٹیگ_t جدول_ٹیگ = HB_OT_ٹیگ_MVAR;

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (نسخہ.sanitize (c) &&
		  hb_barrier () &&
		  likely (نسخہ.major == 1) &&
		  c->check_struct (هذا) &&
		  hb_barrier () &&
		  قدر_ریکارڈ_ناپ >= Variation_قدر_ریکارڈ::static_ناپ &&
		  varStore.sanitize (c, هذا) &&
		  c->check_range (اقدار_Z.arrayZ,
				  قدر_ریکارڈ_گنتی,
				  قدر_ریکارڈ_ناپ));
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
#جے_تعریف_شدہ HB_NO_VAR
    return_trace (خطأ);
#جے_انت

    إذا (c->plan->all_axes_pinned)
      return_trace (خطأ);

    MVAR *باہرلا = c->serializer->شروع_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (باہرلا))) return_trace (خطأ);
    باہرلا->نسخہ = نسخہ;
    باہرلا->reserved = reserved;
    باہرلا->قدر_ریکارڈ_ناپ = قدر_ریکارڈ_ناپ;
    باہرلا->قدر_ریکارڈ_گنتی = قدر_ریکارڈ_گنتی;

    شے_variations_t شے_vars;
    ثابت شے_Variation_Store& src_var_store = هذا+varStore;

    إذا (!شے_vars.instantiate (src_var_store, c->plan))
      return_trace (خطأ);

    /* serialize varstore */
    إذا (!باہرلا->varStore.serialize_serialize (c->serializer, شے_vars.has_long_لفظ (),
                                            c->plan->axis_ٹیگ_سوچی,
                                            شے_vars.get_region_list (),
                                            شے_vars.get_vardata_encodings ()))
      return_trace (خطأ);

    /* serialize value records array */
    بنانشانی قدر_rec_گنتی = قدر_ریکارڈ_گنتی;
    ثابت Variation_قدر_ریکارڈ *ریکارڈ = reinterpret_cast<ثابت Variation_قدر_ریکارڈ*> (اقدار_Z.arrayZ);
    لكل (بنانشانی i = 0; i < قدر_rec_گنتی; i++)
    {
      إذا (!ریکارڈ->subset (c, شے_vars.get_varidx_map ())) return_trace (خطأ);
      ریکارڈ++;
    }
    return_trace (سچ);
  }

  عائم get_var (hb_ٹیگ_t ٹیگ_2,
		 ثابت صحيح *coords, بنانشانی صحيح coord_گنتی) ثابت
  {
    ثابت Variation_قدر_ریکارڈ *ریکارڈ;
    ریکارڈ = (Variation_قدر_ریکارڈ *) hb_bsearch (ٹیگ_2,
						  (ثابت Variation_قدر_ریکارڈ *)
						    (ثابت HBUINT8 *) اقدار_Z,
						  قدر_ریکارڈ_گنتی, قدر_ریکارڈ_ناپ,
						  ٹیگ_compare);
    إذا (!ریکارڈ)
      أرجع 0.;

    أرجع (هذا+varStore).get_delta (ریکارڈ->varIdx, coords, coord_گنتی);
  }

محمي:
  ساكن صحيح ٹیگ_compare (ثابت فراغ *pa, ثابت فراغ *pb)
  {
    ثابت hb_ٹیگ_t *a = (ثابت hb_ٹیگ_t *) pa;
    ثابت ٹیگ *b = (ثابت ٹیگ *) pb;
    أرجع b->cmp (*a);
  }

  محمي:
  Fixed_نسخہ<>نسخہ;	/* Version of the metrics variation table
				 * initially set to 0x00010000u */
  HBUINT16	reserved;	/* Not used; set to 0. */
  HBUINT16	قدر_ریکارڈ_ناپ;/* The size in bytes of each value record —
				 * must be greater than zero. */
  HBUINT16	قدر_ریکارڈ_گنتی;/* The number of value records — may be zero. */
  Offset16_منزل<شے_Variation_Store>
		varStore;	/* Offset to item variation store table. */
  Unsized_Array_دا<HBUINT8>
		اقدار_Z;	/* Array of value records. The records must be
				 * in binary order of their valueTag field. */

  عام:
  DEFINE_ناپ_ARRAY (12, اقدار_Z);
};

} /* namespace OT */


#تعریف_کرو HB_جوڑو_MVAR_VAR(ٹیگ_2, میدان) \
       c->serializer->check_سونپو (جدول->میدان, \
				    roundf (جدول->میدان + \
					    MVAR.get_var (ٹیگ_2, \
							  c->plan->normalized_coords.arrayZ, \
							  c->plan->normalized_coords.لمائی)), \
				    HB_SERIALIZE_ERROR_INT_حد_توں_ودھ)


#جے_انت /* HB_OT_VAR_MVAR_TABLE_HH */
