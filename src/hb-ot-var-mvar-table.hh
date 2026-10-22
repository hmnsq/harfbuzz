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

#ضمن "hb-ot-var-common.hh"


نطاق OT {


بنية Variation_قيمة_Record
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
    auto *out = c->serializer->embed (*هذا);
    إذا (unlikely (!out)) return_trace (خطأ);

    hb_codepoint_t *new_idx;
    return_trace (c->serializer->check_assign (out->varIdx,
                                               (varidx_map.has (varIdx, &new_idx)) ? *new_idx : HB_OT_LAYOUT_NO_VARIATIONS_INDEX,
                                               HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  عام:
  Tag		قيمة_Tag;	/* Four-byte tag identifying a font-wide measure. */
  VarIdx	varIdx;		/* Outer/inner index into ItemVariationStore item. */

  عام:
  DEFINE_SIZE_STATIC (8);
};


/*
 * MVAR -- Metrics Variations
 * https://docs.microsoft.com/en-us/typography/opentype/spec/mvar
 */
#define HB_OT_TAG_MVAR HB_TAG('M','V','A','R')

بنية MVAR
{
  ساكن constexpr hb_tag_t tableTag = HB_OT_TAG_MVAR;

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (version.sanitize (c) &&
		  hb_barrier () &&
		  likely (version.major == 1) &&
		  c->check_struct (هذا) &&
		  hb_barrier () &&
		  قيمة_Record_Size >= Variation_قيمة_Record::static_size &&
		  varStore.sanitize (c, هذا) &&
		  c->check_range (قيم_Z.arrayZ,
				  قيمة_Record_عدد,
				  قيمة_Record_Size));
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
#ifdef HB_NO_VAR
    return_trace (خطأ);
#endif

    إذا (c->plan->all_axes_pinned)
      return_trace (خطأ);

    MVAR *out = c->serializer->start_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (out))) return_trace (خطأ);
    out->version = version;
    out->reserved = reserved;
    out->قيمة_Record_Size = قيمة_Record_Size;
    out->قيمة_Record_عدد = قيمة_Record_عدد;

    item_variations_t item_vars;
    ثابت ItemVariationStore& src_var_store = هذا+varStore;

    إذا (!item_vars.instantiate (src_var_store, c->plan))
      return_trace (خطأ);

    /* serialize varstore */
    إذا (!out->varStore.serialize_serialize (c->serializer, item_vars.has_long_word (),
                                            c->plan->axis_tags,
                                            item_vars.get_region_list (),
                                            item_vars.get_vardata_encodings ()))
      return_trace (خطأ);

    /* serialize value records array */
    unsigned قيمة_rec_عدد = قيمة_Record_عدد;
    ثابت Variation_قيمة_Record *record = reinterpret_cast<ثابت Variation_قيمة_Record*> (قيم_Z.arrayZ);
    لكل (unsigned i = 0; i < قيمة_rec_عدد; i++)
    {
      إذا (!record->subset (c, item_vars.get_varidx_map ())) return_trace (خطأ);
      record++;
    }
    return_trace (true);
  }

  عائم get_var (hb_tag_t tag,
		 ثابت صحيح *coords, unsigned صحيح coord_عدد) ثابت
  {
    ثابت Variation_قيمة_Record *record;
    record = (Variation_قيمة_Record *) hb_bsearch (tag,
						  (ثابت Variation_قيمة_Record *)
						    (ثابت HBUINT8 *) قيم_Z,
						  قيمة_Record_عدد, قيمة_Record_Size,
						  tag_compare);
    إذا (!record)
      أرجع 0.;

    أرجع (هذا+varStore).get_delta (record->varIdx, coords, coord_عدد);
  }

محمي:
  ساكن صحيح tag_compare (ثابت فراغ *pa, ثابت فراغ *pb)
  {
    ثابت hb_tag_t *a = (ثابت hb_tag_t *) pa;
    ثابت Tag *b = (ثابت Tag *) pb;
    أرجع b->cmp (*a);
  }

  محمي:
  FixedVersion<>version;	/* Version of the metrics variation table
				 * initially set to 0x00010000u */
  HBUINT16	reserved;	/* Not used; set to 0. */
  HBUINT16	قيمة_Record_Size;/* The size in bytes of each value record —
				 * must be greater than zero. */
  HBUINT16	قيمة_Record_عدد;/* The number of value records — may be zero. */
  Offset16To<ItemVariationStore>
		varStore;	/* Offset to item variation store table. */
  UnsizedArrayOf<HBUINT8>
		قيم_Z;	/* Array of value records. The records must be
				 * in binary order of their valueTag field. */

  عام:
  DEFINE_SIZE_ARRAY (12, قيم_Z);
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
