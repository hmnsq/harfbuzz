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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_OT_VAR_MVAR_ਸਾਰਣੀ_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_VAR_MVAR_ਸਾਰਣੀ_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-var-common.hh"


ਨਾਮਾਕਾਸ਼ OT {


ਬਣਤਰ Variation_ਮੁੱਲ_ਰਿਕਾਰਡ
{
  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ));
  }

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c,
               ਅਚਲ hb_map_t& varidx_map) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->serializer->embed (*ਇਹ);
    ਜੇ (unlikely (!ਬਾਹਰਲਾ)) return_trace (ਝੂਠ);

    hb_codepoint_t *new_idx;
    return_trace (c->serializer->check_ਸੌਂਪੋ (ਬਾਹਰਲਾ->varIdx,
                                               (varidx_map.has (varIdx, &new_idx)) ? *new_idx : HB_OT_LAYOUT_NO_VARIATIONS_ਸੂਚਕਾਂਕ,
                                               HB_SERIALIZE_ERROR_INT_ਉਛਾਲ));
  }

  ਜਨਤਕ:
  ਟੈਗ		ਮੁੱਲ_ਟੈਗ;	/* Four-byte tag identifying a font-wide measure. */
  VarIdx	varIdx;		/* Outer/inner index into ItemVariationStore item. */

  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (8);
};


/*
 * MVAR -- Metrics Variations
 * https://docs.microsoft.com/en-us/typography/opentype/spec/mvar
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_ਟੈਗ_MVAR HB_ਟੈਗ('M','V','A','R')

ਬਣਤਰ MVAR
{
  ਸਥਿਰ constexpr hb_ਟੈਗ_t ਸਾਰਣੀ_ਟੈਗ = HB_OT_ਟੈਗ_MVAR;

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (ਸੰਸਕਰਣ.sanitize (c) &&
		  hb_barrier () &&
		  likely (ਸੰਸਕਰਣ.major == 1) &&
		  c->check_struct (ਇਹ) &&
		  hb_barrier () &&
		  ਮੁੱਲ_ਰਿਕਾਰਡ_ਮਾਪ >= Variation_ਮੁੱਲ_ਰਿਕਾਰਡ::static_ਮਾਪ &&
		  varStore.sanitize (c, ਇਹ) &&
		  c->check_range (ਮੁੱਲਾਂ_Z.arrayZ,
				  ਮੁੱਲ_ਰਿਕਾਰਡ_ਗਿਣਤੀ,
				  ਮੁੱਲ_ਰਿਕਾਰਡ_ਮਾਪ));
  }

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HB_NO_VAR
    return_trace (ਝੂਠ);
#ਜੇ_ਅੰਤ

    ਜੇ (c->plan->all_axes_pinned)
      return_trace (ਝੂਠ);

    MVAR *ਬਾਹਰਲਾ = c->serializer->ਸ਼ੁਰੂ_embed (*ਇਹ);
    ਜੇ (unlikely (!c->serializer->extend_min (ਬਾਹਰਲਾ))) return_trace (ਝੂਠ);
    ਬਾਹਰਲਾ->ਸੰਸਕਰਣ = ਸੰਸਕਰਣ;
    ਬਾਹਰਲਾ->reserved = reserved;
    ਬਾਹਰਲਾ->ਮੁੱਲ_ਰਿਕਾਰਡ_ਮਾਪ = ਮੁੱਲ_ਰਿਕਾਰਡ_ਮਾਪ;
    ਬਾਹਰਲਾ->ਮੁੱਲ_ਰਿਕਾਰਡ_ਗਿਣਤੀ = ਮੁੱਲ_ਰਿਕਾਰਡ_ਗਿਣਤੀ;

    ਵਸਤੂ_variations_t ਵਸਤੂ_vars;
    ਅਚਲ ਵਸਤੂ_Variation_Store& src_var_store = ਇਹ+varStore;

    ਜੇ (!ਵਸਤੂ_vars.instantiate (src_var_store, c->plan))
      return_trace (ਝੂਠ);

    /* serialize varstore */
    ਜੇ (!ਬਾਹਰਲਾ->varStore.serialize_serialize (c->serializer, ਵਸਤੂ_vars.has_long_ਸ਼ਬਦ (),
                                            c->plan->axis_ਟੈਗ_ਸੂਚੀ,
                                            ਵਸਤੂ_vars.get_region_list (),
                                            ਵਸਤੂ_vars.get_vardata_encodings ()))
      return_trace (ਝੂਠ);

    /* serialize value records array */
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਮੁੱਲ_rec_ਗਿਣਤੀ = ਮੁੱਲ_ਰਿਕਾਰਡ_ਗਿਣਤੀ;
    ਅਚਲ Variation_ਮੁੱਲ_ਰਿਕਾਰਡ *ਰਿਕਾਰਡ = reinterpret_cast<ਅਚਲ Variation_ਮੁੱਲ_ਰਿਕਾਰਡ*> (ਮੁੱਲਾਂ_Z.arrayZ);
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਮੁੱਲ_rec_ਗਿਣਤੀ; i++)
    {
      ਜੇ (!ਰਿਕਾਰਡ->subset (c, ਵਸਤੂ_vars.get_varidx_map ())) return_trace (ਝੂਠ);
      ਰਿਕਾਰਡ++;
    }
    return_trace (ਸੱਚ);
  }

  ਦਸ਼ਮਲਵ get_var (hb_ਟੈਗ_t ਟੈਗ_2,
		 ਅਚਲ ਪੂਰਨ *coords, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ coord_ਗਿਣਤੀ) ਅਚਲ
  {
    ਅਚਲ Variation_ਮੁੱਲ_ਰਿਕਾਰਡ *ਰਿਕਾਰਡ;
    ਰਿਕਾਰਡ = (Variation_ਮੁੱਲ_ਰਿਕਾਰਡ *) hb_bsearch (ਟੈਗ_2,
						  (ਅਚਲ Variation_ਮੁੱਲ_ਰਿਕਾਰਡ *)
						    (ਅਚਲ HBUINT8 *) ਮੁੱਲਾਂ_Z,
						  ਮੁੱਲ_ਰਿਕਾਰਡ_ਗਿਣਤੀ, ਮੁੱਲ_ਰਿਕਾਰਡ_ਮਾਪ,
						  ਟੈਗ_compare);
    ਜੇ (!ਰਿਕਾਰਡ)
      ਵਾਪਸ 0.;

    ਵਾਪਸ (ਇਹ+varStore).get_delta (ਰਿਕਾਰਡ->varIdx, coords, coord_ਗਿਣਤੀ);
  }

ਸੁਰੱਖਿਅਤ:
  ਸਥਿਰ ਪੂਰਨ ਟੈਗ_compare (ਅਚਲ ਖਾਲੀ *pa, ਅਚਲ ਖਾਲੀ *pb)
  {
    ਅਚਲ hb_ਟੈਗ_t *a = (ਅਚਲ hb_ਟੈਗ_t *) pa;
    ਅਚਲ ਟੈਗ *b = (ਅਚਲ ਟੈਗ *) pb;
    ਵਾਪਸ b->cmp (*a);
  }

  ਸੁਰੱਖਿਅਤ:
  Fixed_ਸੰਸਕਰਣ<>ਸੰਸਕਰਣ;	/* Version of the metrics variation table
				 * initially set to 0x00010000u */
  HBUINT16	reserved;	/* Not used; set to 0. */
  HBUINT16	ਮੁੱਲ_ਰਿਕਾਰਡ_ਮਾਪ;/* The size in bytes of each value record —
				 * must be greater than zero. */
  HBUINT16	ਮੁੱਲ_ਰਿਕਾਰਡ_ਗਿਣਤੀ;/* The number of value records — may be zero. */
  Offset16_ਮੰਜ਼ਿਲ<ਵਸਤੂ_Variation_Store>
		varStore;	/* Offset to item variation store table. */
  Unsized_Array_ਦਾ<HBUINT8>
		ਮੁੱਲਾਂ_Z;	/* Array of value records. The records must be
				 * in binary order of their valueTag field. */

  ਜਨਤਕ:
  DEFINE_ਮਾਪ_ARRAY (12, ਮੁੱਲਾਂ_Z);
};

} /* namespace OT */


#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_ਜੋੜੋ_MVAR_VAR(ਟੈਗ_2, ਖੇਤਰ) \
       c->serializer->check_ਸੌਂਪੋ (ਸਾਰਣੀ->ਖੇਤਰ, \
				    roundf (ਸਾਰਣੀ->ਖੇਤਰ + \
					    MVAR.get_var (ਟੈਗ_2, \
							  c->plan->normalized_coords.arrayZ, \
							  c->plan->normalized_coords.ਲੰਮਾਈ)), \
				    HB_SERIALIZE_ERROR_INT_ਉਛਾਲ)


#ਜੇ_ਅੰਤ /* HB_OT_VAR_MVAR_TABLE_HH */
