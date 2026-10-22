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

#اگر_غیر_تعریف_شدہ HB_OT_VAR_MVAR_جدول_HH
#تعریف_کرو HB_OT_VAR_MVAR_جدول_HH

#شامل "hb-ot-var-common.hh"


نامخلا OT {


ساخت Variation_قدر_ریکارڈ
{
  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ));
  }

  منطقی subset (hb_subset_context_t *c,
               ثابت hb_map_t& varidx_map) ثابت
  {
    TRACE_SUBSET (یہ);
    خودکار *برآمد = c->serializer->embed (*یہ);
    اگر (unlikely (!برآمد)) return_trace (جھوٹ);

    hb_codepoint_t *new_idx;
    return_trace (c->serializer->check_تفویض (برآمد->varIdx,
                                               (varidx_map.has (varIdx, &new_idx)) ? *new_idx : HB_OT_LAYOUT_NO_VARIATIONS_اشاریہ,
                                               HB_SERIALIZE_ERROR_INT_حد_سے_تجاوز));
  }

  عوامی:
  لیبل		قدر_لیبل;	/* Four-byte tag identifying a font-wide measure. */
  VarIdx	varIdx;		/* Outer/inner index into ItemVariationStore item. */

  عوامی:
  DEFINE_سائز_STATIC (8);
};


/*
 * MVAR -- Metrics Variations
 * https://docs.microsoft.com/en-us/typography/opentype/spec/mvar
 */
#تعریف_کرو HB_OT_لیبل_MVAR HB_لیبل('M','V','A','R')

ساخت MVAR
{
  ساکن constexpr hb_لیبل_t جدول_لیبل = HB_OT_لیبل_MVAR;

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (نسخہ.sanitize (c) &&
		  hb_barrier () &&
		  likely (نسخہ.major == 1) &&
		  c->check_struct (یہ) &&
		  hb_barrier () &&
		  قدر_ریکارڈ_سائز >= Variation_قدر_ریکارڈ::static_سائز &&
		  varStore.sanitize (c, یہ) &&
		  c->check_range (اقدار_Z.arrayZ,
				  قدر_ریکارڈ_گنتی,
				  قدر_ریکارڈ_سائز));
  }

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (یہ);
#اگر_تعریف_شدہ HB_NO_VAR
    return_trace (جھوٹ);
#اگر_ختم

    اگر (c->plan->all_axes_pinned)
      return_trace (جھوٹ);

    MVAR *برآمد = c->serializer->آغاز_embed (*یہ);
    اگر (unlikely (!c->serializer->extend_min (برآمد))) return_trace (جھوٹ);
    برآمد->نسخہ = نسخہ;
    برآمد->reserved = reserved;
    برآمد->قدر_ریکارڈ_سائز = قدر_ریکارڈ_سائز;
    برآمد->قدر_ریکارڈ_گنتی = قدر_ریکارڈ_گنتی;

    شے_variations_t شے_vars;
    ثابت شے_Variation_Store& src_var_store = یہ+varStore;

    اگر (!شے_vars.instantiate (src_var_store, c->plan))
      return_trace (جھوٹ);

    /* serialize varstore */
    اگر (!برآمد->varStore.serialize_serialize (c->serializer, شے_vars.has_long_لفظ (),
                                            c->plan->axis_لیبل_فہرست,
                                            شے_vars.get_region_list (),
                                            شے_vars.get_vardata_encodings ()))
      return_trace (جھوٹ);

    /* serialize value records array */
    غیرعلامتی قدر_rec_گنتی = قدر_ریکارڈ_گنتی;
    ثابت Variation_قدر_ریکارڈ *ریکارڈ = reinterpret_cast<ثابت Variation_قدر_ریکارڈ*> (اقدار_Z.arrayZ);
    برائے (غیرعلامتی i = 0; i < قدر_rec_گنتی; i++)
    {
      اگر (!ریکارڈ->subset (c, شے_vars.get_varidx_map ())) return_trace (جھوٹ);
      ریکارڈ++;
    }
    return_trace (سچ);
  }

  اعشاری get_var (hb_لیبل_t لیبل_2,
		 ثابت صحیح *coords, غیرعلامتی صحیح coord_گنتی) ثابت
  {
    ثابت Variation_قدر_ریکارڈ *ریکارڈ;
    ریکارڈ = (Variation_قدر_ریکارڈ *) hb_bsearch (لیبل_2,
						  (ثابت Variation_قدر_ریکارڈ *)
						    (ثابت HBUINT8 *) اقدار_Z,
						  قدر_ریکارڈ_گنتی, قدر_ریکارڈ_سائز,
						  لیبل_compare);
    اگر (!ریکارڈ)
      واپس 0.;

    واپس (یہ+varStore).get_delta (ریکارڈ->varIdx, coords, coord_گنتی);
  }

محفوظ:
  ساکن صحیح لیبل_compare (ثابت خالی *pa, ثابت خالی *pb)
  {
    ثابت hb_لیبل_t *a = (ثابت hb_لیبل_t *) pa;
    ثابت لیبل *b = (ثابت لیبل *) pb;
    واپس b->cmp (*a);
  }

  محفوظ:
  Fixed_نسخہ<>نسخہ;	/* Version of the metrics variation table
				 * initially set to 0x00010000u */
  HBUINT16	reserved;	/* Not used; set to 0. */
  HBUINT16	قدر_ریکارڈ_سائز;/* The size in bytes of each value record —
				 * must be greater than zero. */
  HBUINT16	قدر_ریکارڈ_گنتی;/* The number of value records — may be zero. */
  Offset16_مقصد<شے_Variation_Store>
		varStore;	/* Offset to item variation store table. */
  Unsized_Array_کا<HBUINT8>
		اقدار_Z;	/* Array of value records. The records must be
				 * in binary order of their valueTag field. */

  عوامی:
  DEFINE_سائز_ARRAY (12, اقدار_Z);
};

} /* namespace OT */


#تعریف_کرو HB_جوڑو_MVAR_VAR(لیبل_2, میدان) \
       c->serializer->check_تفویض (جدول->میدان, \
				    roundf (جدول->میدان + \
					    MVAR.get_var (لیبل_2, \
							  c->plan->normalized_coords.arrayZ, \
							  c->plan->normalized_coords.لمبائی)), \
				    HB_SERIALIZE_ERROR_INT_حد_سے_تجاوز)


#اگر_ختم /* HB_OT_VAR_MVAR_TABLE_HH */
