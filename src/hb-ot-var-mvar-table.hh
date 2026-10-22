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

#شامل "hb-ot-var-common.hh"


فضاینام OT {


ساختار Variation_مقدار_Record
{
  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این));
  }

  منطقی subset (hb_subset_context_t *c,
               ثابت hb_map_t& varidx_map) ثابت
  {
    TRACE_SUBSET (این);
    auto *out = c->serializer->embed (*این);
    اگر (unlikely (!out)) return_trace (نادرست);

    hb_codepoint_t *new_idx;
    return_trace (c->serializer->check_assign (out->varIdx,
                                               (varidx_map.has (varIdx, &new_idx)) ? *new_idx : HB_OT_LAYOUT_NO_VARIATIONS_INDEX,
                                               HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  عمومی:
  Tag		مقدار_Tag;	/* Four-byte tag identifying a font-wide measure. */
  VarIdx	varIdx;		/* Outer/inner index into ItemVariationStore item. */

  عمومی:
  DEFINE_SIZE_STATIC (8);
};


/*
 * MVAR -- Metrics Variations
 * https://docs.microsoft.com/en-us/typography/opentype/spec/mvar
 */
#define HB_OT_TAG_MVAR HB_TAG('M','V','A','R')

ساختار MVAR
{
  ایستا constexpr hb_tag_t tableTag = HB_OT_TAG_MVAR;

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (version.sanitize (c) &&
		  hb_barrier () &&
		  likely (version.major == 1) &&
		  c->check_struct (این) &&
		  hb_barrier () &&
		  مقدار_Record_Size >= Variation_مقدار_Record::static_size &&
		  varStore.sanitize (c, این) &&
		  c->check_range (مقادیر_Z.arrayZ,
				  مقدار_Record_شمار,
				  مقدار_Record_Size));
  }

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (این);
#ifdef HB_NO_VAR
    return_trace (نادرست);
#endif

    اگر (c->plan->all_axes_pinned)
      return_trace (نادرست);

    MVAR *out = c->serializer->start_embed (*این);
    اگر (unlikely (!c->serializer->extend_min (out))) return_trace (نادرست);
    out->version = version;
    out->reserved = reserved;
    out->مقدار_Record_Size = مقدار_Record_Size;
    out->مقدار_Record_شمار = مقدار_Record_شمار;

    item_variations_t item_vars;
    ثابت ItemVariationStore& src_var_store = این+varStore;

    اگر (!item_vars.instantiate (src_var_store, c->plan))
      return_trace (نادرست);

    /* serialize varstore */
    اگر (!out->varStore.serialize_serialize (c->serializer, item_vars.has_long_word (),
                                            c->plan->axis_tags,
                                            item_vars.get_region_list (),
                                            item_vars.get_vardata_encodings ()))
      return_trace (نادرست);

    /* serialize value records array */
    unsigned مقدار_rec_شمار = مقدار_Record_شمار;
    ثابت Variation_مقدار_Record *record = reinterpret_cast<ثابت Variation_مقدار_Record*> (مقادیر_Z.arrayZ);
    برای (unsigned i = 0; i < مقدار_rec_شمار; i++)
    {
      اگر (!record->subset (c, item_vars.get_varidx_map ())) return_trace (نادرست);
      record++;
    }
    return_trace (درست);
  }

  اعشاری get_var (hb_tag_t tag,
		 ثابت صحیح *coords, unsigned صحیح coord_شمار) ثابت
  {
    ثابت Variation_مقدار_Record *record;
    record = (Variation_مقدار_Record *) hb_bsearch (tag,
						  (ثابت Variation_مقدار_Record *)
						    (ثابت HBUINT8 *) مقادیر_Z,
						  مقدار_Record_شمار, مقدار_Record_Size,
						  tag_compare);
    اگر (!record)
      بازگشت 0.;

    بازگشت (این+varStore).get_delta (record->varIdx, coords, coord_شمار);
  }

محافظتشده:
  ایستا صحیح tag_compare (ثابت تهی *pa, ثابت تهی *pb)
  {
    ثابت hb_tag_t *a = (ثابت hb_tag_t *) pa;
    ثابت Tag *b = (ثابت Tag *) pb;
    بازگشت b->cmp (*a);
  }

  محافظتشده:
  FixedVersion<>version;	/* Version of the metrics variation table
				 * initially set to 0x00010000u */
  HBUINT16	reserved;	/* Not used; set to 0. */
  HBUINT16	مقدار_Record_Size;/* The size in bytes of each value record —
				 * must be greater than zero. */
  HBUINT16	مقدار_Record_شمار;/* The number of value records — may be zero. */
  Offset16To<ItemVariationStore>
		varStore;	/* Offset to item variation store table. */
  UnsizedArrayOf<HBUINT8>
		مقادیر_Z;	/* Array of value records. The records must be
				 * in binary order of their valueTag field. */

  عمومی:
  DEFINE_SIZE_ARRAY (12, مقادیر_Z);
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
