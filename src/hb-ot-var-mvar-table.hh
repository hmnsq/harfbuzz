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

#ifndef HB_OT_VAR_MVAR_TABLE_HH
#define HB_OT_VAR_MVAR_TABLE_HH

#ಅನ್ತರ್ಭಾವ "hb-ot-var-common.hh"


ನಾಮಾಕಾಶ OT {


ಸಂರಚನಾ Variation_ಮೌಲ್ಯ_Record
{
  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್));
  }

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c,
               ಧ್ರುವ hb_map_t& varidx_map) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    auto *out = c->serializer->embed (*ಏತತ್);
    ಯದಿ (unlikely (!out)) return_trace (ಅಸತ್ಯ);

    hb_codepoint_t *new_idx;
    return_trace (c->serializer->check_assign (out->varIdx,
                                               (varidx_map.has (varIdx, &new_idx)) ? *new_idx : HB_OT_LAYOUT_NO_VARIATIONS_INDEX,
                                               HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  ಸಾರ್ವಜನಿಕ:
  Tag		ಮೌಲ್ಯ_Tag;	/* Four-byte tag identifying a font-wide measure. */
  VarIdx	varIdx;		/* Outer/inner index into ItemVariationStore item. */

  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (8);
};


/*
 * MVAR -- Metrics Variations
 * https://docs.microsoft.com/en-us/typography/opentype/spec/mvar
 */
#define HB_OT_TAG_MVAR HB_TAG('M','V','A','R')

ಸಂರಚನಾ MVAR
{
  ಸ್ಥಿರ constexpr hb_tag_t tableTag = HB_OT_TAG_MVAR;

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (version.sanitize (c) &&
		  hb_barrier () &&
		  likely (version.major == 1) &&
		  c->check_struct (ಏತತ್) &&
		  hb_barrier () &&
		  ಮೌಲ್ಯ_Record_Size >= Variation_ಮೌಲ್ಯ_Record::static_size &&
		  varStore.sanitize (c, ಏತತ್) &&
		  c->check_range (ಮೌಲ್ಯಗಳು_Z.arrayZ,
				  ಮೌಲ್ಯ_Record_ಎಣಿಕೆ,
				  ಮೌಲ್ಯ_Record_Size));
  }

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
#ifdef HB_NO_VAR
    return_trace (ಅಸತ್ಯ);
#endif

    ಯದಿ (c->plan->all_axes_pinned)
      return_trace (ಅಸತ್ಯ);

    MVAR *out = c->serializer->start_embed (*ಏತತ್);
    ಯದಿ (unlikely (!c->serializer->extend_min (out))) return_trace (ಅಸತ್ಯ);
    out->version = version;
    out->reserved = reserved;
    out->ಮೌಲ್ಯ_Record_Size = ಮೌಲ್ಯ_Record_Size;
    out->ಮೌಲ್ಯ_Record_ಎಣಿಕೆ = ಮೌಲ್ಯ_Record_ಎಣಿಕೆ;

    item_variations_t item_vars;
    ಧ್ರುವ ItemVariationStore& src_var_store = ಏತತ್+varStore;

    ಯದಿ (!item_vars.instantiate (src_var_store, c->plan))
      return_trace (ಅಸತ್ಯ);

    /* serialize varstore */
    ಯದಿ (!out->varStore.serialize_serialize (c->serializer, item_vars.has_long_word (),
                                            c->plan->axis_tags,
                                            item_vars.get_region_list (),
                                            item_vars.get_vardata_encodings ()))
      return_trace (ಅಸತ್ಯ);

    /* serialize value records array */
    unsigned ಮೌಲ್ಯ_rec_ಎಣಿಕೆ = ಮೌಲ್ಯ_Record_ಎಣಿಕೆ;
    ಧ್ರುವ Variation_ಮೌಲ್ಯ_Record *record = reinterpret_cast<ಧ್ರುವ Variation_ಮೌಲ್ಯ_Record*> (ಮೌಲ್ಯಗಳು_Z.arrayZ);
    ಪ್ರತಿ (unsigned i = 0; i < ಮೌಲ್ಯ_rec_ಎಣಿಕೆ; i++)
    {
      ಯದಿ (!record->subset (c, item_vars.get_varidx_map ())) return_trace (ಅಸತ್ಯ);
      record++;
    }
    return_trace (ಸತ್ಯ);
  }

  ಭಿನ್ನ get_var (hb_tag_t tag,
		 ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ *coords, unsigned ಪೂರ್ಣಾಙ್ಕ coord_ಎಣಿಕೆ) ಧ್ರುವ
  {
    ಧ್ರುವ Variation_ಮೌಲ್ಯ_Record *record;
    record = (Variation_ಮೌಲ್ಯ_Record *) hb_bsearch (tag,
						  (ಧ್ರುವ Variation_ಮೌಲ್ಯ_Record *)
						    (ಧ್ರುವ HBUINT8 *) ಮೌಲ್ಯಗಳು_Z,
						  ಮೌಲ್ಯ_Record_ಎಣಿಕೆ, ಮೌಲ್ಯ_Record_Size,
						  tag_compare);
    ಯದಿ (!record)
      ಪ್ರತ್ಯಾಗಮ 0.;

    ಪ್ರತ್ಯಾಗಮ (ಏತತ್+varStore).get_delta (record->varIdx, coords, coord_ಎಣಿಕೆ);
  }

ರಕ್ಷಿತ:
  ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ tag_compare (ಧ್ರುವ ಶೂನ್ಯ *pa, ಧ್ರುವ ಶೂನ್ಯ *pb)
  {
    ಧ್ರುವ hb_tag_t *a = (ಧ್ರುವ hb_tag_t *) pa;
    ಧ್ರುವ Tag *b = (ಧ್ರುವ Tag *) pb;
    ಪ್ರತ್ಯಾಗಮ b->cmp (*a);
  }

  ರಕ್ಷಿತ:
  FixedVersion<>version;	/* Version of the metrics variation table
				 * initially set to 0x00010000u */
  HBUINT16	reserved;	/* Not used; set to 0. */
  HBUINT16	ಮೌಲ್ಯ_Record_Size;/* The size in bytes of each value record —
				 * must be greater than zero. */
  HBUINT16	ಮೌಲ್ಯ_Record_ಎಣಿಕೆ;/* The number of value records — may be zero. */
  Offset16To<ItemVariationStore>
		varStore;	/* Offset to item variation store table. */
  UnsizedArrayOf<HBUINT8>
		ಮೌಲ್ಯಗಳು_Z;	/* Array of value records. The records must be
				 * in binary order of their valueTag field. */

  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_ARRAY (12, ಮೌಲ್ಯಗಳು_Z);
};

} /* namespace OT */


#define HB_ADD_MVAR_VAR(tag, field) \
       c->serializer->check_assign (table->field, \
				    roundf (table->field + \
					    MVAR.get_var (tag, \
							  c->plan->normalized_coords.arrayZ, \
							  c->plan->normalized_coords.length)), \
				    HB_SERIALIZE_ERROR_INT_OVERFLOW)


#endif /* HB_OT_VAR_MVAR_TABLE_HH */
