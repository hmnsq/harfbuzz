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

#অন্তর্ভাব "hb-ot-var-common.hh"


নামাকাশ OT {


কাঠামো Variation_মান_Record
{
  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই));
  }

  যৌক্তিক subset (hb_subset_context_t *c,
               ধ্রুবক hb_map_t& varidx_map) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    auto *out = c->serializer->embed (*এই);
    যদি (unlikely (!out)) return_trace (মিথ্যা);

    hb_codepoint_t *new_idx;
    return_trace (c->serializer->check_assign (out->varIdx,
                                               (varidx_map.has (varIdx, &new_idx)) ? *new_idx : HB_OT_LAYOUT_NO_VARIATIONS_সূচক,
                                               HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  সর্বজনীন:
  Tag		মান_Tag;	/* Four-byte tag identifying a font-wide measure. */
  VarIdx	varIdx;		/* Outer/inner index into ItemVariationStore item. */

  সর্বজনীন:
  DEFINE_আকার_STATIC (8);
};


/*
 * MVAR -- Metrics Variations
 * https://docs.microsoft.com/en-us/typography/opentype/spec/mvar
 */
#define HB_OT_TAG_MVAR HB_TAG('M','V','A','R')

কাঠামো MVAR
{
  স্থির constexpr hb_tag_t tableTag = HB_OT_TAG_MVAR;

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (version.sanitize (c) &&
		  hb_barrier () &&
		  likely (version.major == 1) &&
		  c->check_struct (এই) &&
		  hb_barrier () &&
		  মান_Record_আকার >= Variation_মান_Record::static_আকার &&
		  varStore.sanitize (c, এই) &&
		  c->check_range (মানসমূহ_Z.arrayZ,
				  মান_Record_গুনতি,
				  মান_Record_আকার));
  }

  যৌক্তিক subset (hb_subset_context_t *c) ধ্রুবক
  {
    TRACE_SUBSET (এই);
#ifdef HB_NO_VAR
    return_trace (মিথ্যা);
#endif

    যদি (c->plan->all_axes_pinned)
      return_trace (মিথ্যা);

    MVAR *out = c->serializer->start_embed (*এই);
    যদি (unlikely (!c->serializer->extend_min (out))) return_trace (মিথ্যা);
    out->version = version;
    out->reserved = reserved;
    out->মান_Record_আকার = মান_Record_আকার;
    out->মান_Record_গুনতি = মান_Record_গুনতি;

    item_variations_t item_vars;
    ধ্রুবক ItemVariationStore& src_var_store = এই+varStore;

    যদি (!item_vars.instantiate (src_var_store, c->plan))
      return_trace (মিথ্যা);

    /* serialize varstore */
    যদি (!out->varStore.serialize_serialize (c->serializer, item_vars.has_long_word (),
                                            c->plan->axis_tags,
                                            item_vars.get_region_list (),
                                            item_vars.get_vardata_encodings ()))
      return_trace (মিথ্যা);

    /* serialize value records array */
    unsigned মান_rec_গুনতি = মান_Record_গুনতি;
    ধ্রুবক Variation_মান_Record *record = reinterpret_cast<ধ্রুবক Variation_মান_Record*> (মানসমূহ_Z.arrayZ);
    জন্য (unsigned i = 0; i < মান_rec_গুনতি; i++)
    {
      যদি (!record->subset (c, item_vars.get_varidx_map ())) return_trace (মিথ্যা);
      record++;
    }
    return_trace (সত্য);
  }

  দশমিক get_var (hb_tag_t tag,
		 ধ্রুবক পূর্ণসংখ্যা *coords, unsigned পূর্ণসংখ্যা coord_গুনতি) ধ্রুবক
  {
    ধ্রুবক Variation_মান_Record *record;
    record = (Variation_মান_Record *) hb_bsearch (tag,
						  (ধ্রুবক Variation_মান_Record *)
						    (ধ্রুবক HBUINT8 *) মানসমূহ_Z,
						  মান_Record_গুনতি, মান_Record_আকার,
						  tag_compare);
    যদি (!record)
      ফেরত 0.;

    ফেরত (এই+varStore).get_delta (record->varIdx, coords, coord_গুনতি);
  }

সুরক্ষিত:
  স্থির পূর্ণসংখ্যা tag_compare (ধ্রুবক শূন্য *pa, ধ্রুবক শূন্য *pb)
  {
    ধ্রুবক hb_tag_t *a = (ধ্রুবক hb_tag_t *) pa;
    ধ্রুবক Tag *b = (ধ্রুবক Tag *) pb;
    ফেরত b->cmp (*a);
  }

  সুরক্ষিত:
  FixedVersion<>version;	/* Version of the metrics variation table
				 * initially set to 0x00010000u */
  HBUINT16	reserved;	/* Not used; set to 0. */
  HBUINT16	মান_Record_আকার;/* The size in bytes of each value record —
				 * must be greater than zero. */
  HBUINT16	মান_Record_গুনতি;/* The number of value records — may be zero. */
  Offset16To<ItemVariationStore>
		varStore;	/* Offset to item variation store table. */
  UnsizedArrayOf<HBUINT8>
		মানসমূহ_Z;	/* Array of value records. The records must be
				 * in binary order of their valueTag field. */

  সর্বজনীন:
  DEFINE_আকার_ARRAY (12, মানসমূহ_Z);
};

} /* namespace OT */


#define HB_ADD_MVAR_VAR(tag, field) \
       c->serializer->check_assign (table->field, \
				    roundf (table->field + \
					    MVAR.get_var (tag, \
							  c->plan->normalized_coords.arrayZ, \
							  c->plan->normalized_coords.দৈর্ঘ্য)), \
				    HB_SERIALIZE_ERROR_INT_OVERFLOW)


#endif /* HB_OT_VAR_MVAR_TABLE_HH */
