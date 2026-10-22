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


সংরচনা Variation_মান_Record
{
  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্));
  }

  তার্কিক subset (hb_subset_context_t *c,
               ধ্রুব hb_map_t& varidx_map) ধ্রুব
  {
    TRACE_SUBSET (এতত্);
    auto *out = c->serializer->embed (*এতত্);
    যদি (unlikely (!out)) return_trace (অসত্য);

    hb_codepoint_t *new_idx;
    return_trace (c->serializer->check_assign (out->varIdx,
                                               (varidx_map.has (varIdx, &new_idx)) ? *new_idx : HB_OT_LAYOUT_NO_VARIATIONS_INDEX,
                                               HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  সার্বজনিক:
  Tag		মান_Tag;	/* Four-byte tag identifying a font-wide measure. */
  VarIdx	varIdx;		/* Outer/inner index into ItemVariationStore item. */

  সার্বজনিক:
  DEFINE_SIZE_STATIC (8);
};


/*
 * MVAR -- Metrics Variations
 * https://docs.microsoft.com/en-us/typography/opentype/spec/mvar
 */
#define HB_OT_TAG_MVAR HB_TAG('M','V','A','R')

সংরচনা MVAR
{
  স্থির constexpr hb_tag_t tableTag = HB_OT_TAG_MVAR;

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    return_trace (version.sanitize (c) &&
		  hb_barrier () &&
		  likely (version.major == 1) &&
		  c->check_struct (এতত্) &&
		  hb_barrier () &&
		  মান_Record_Size >= Variation_মান_Record::static_size &&
		  varStore.sanitize (c, এতত্) &&
		  c->check_range (মানসমূহ_Z.arrayZ,
				  মান_Record_গুনতি,
				  মান_Record_Size));
  }

  তার্কিক subset (hb_subset_context_t *c) ধ্রুব
  {
    TRACE_SUBSET (এতত্);
#ifdef HB_NO_VAR
    return_trace (অসত্য);
#endif

    যদি (c->plan->all_axes_pinned)
      return_trace (অসত্য);

    MVAR *out = c->serializer->start_embed (*এতত্);
    যদি (unlikely (!c->serializer->extend_min (out))) return_trace (অসত্য);
    out->version = version;
    out->reserved = reserved;
    out->মান_Record_Size = মান_Record_Size;
    out->মান_Record_গুনতি = মান_Record_গুনতি;

    item_variations_t item_vars;
    ধ্রুব ItemVariationStore& src_var_store = এতত্+varStore;

    যদি (!item_vars.instantiate (src_var_store, c->plan))
      return_trace (অসত্য);

    /* serialize varstore */
    যদি (!out->varStore.serialize_serialize (c->serializer, item_vars.has_long_word (),
                                            c->plan->axis_tags,
                                            item_vars.get_region_list (),
                                            item_vars.get_vardata_encodings ()))
      return_trace (অসত্য);

    /* serialize value records array */
    unsigned মান_rec_গুনতি = মান_Record_গুনতি;
    ধ্রুব Variation_মান_Record *record = reinterpret_cast<ধ্রুব Variation_মান_Record*> (মানসমূহ_Z.arrayZ);
    প্রতি (unsigned i = 0; i < মান_rec_গুনতি; i++)
    {
      যদি (!record->subset (c, item_vars.get_varidx_map ())) return_trace (অসত্য);
      record++;
    }
    return_trace (সত্য);
  }

  ভিন্ন get_var (hb_tag_t tag,
		 ধ্রুব পূর্ণাঙ্ক *coords, unsigned পূর্ণাঙ্ক coord_গুনতি) ধ্রুব
  {
    ধ্রুব Variation_মান_Record *record;
    record = (Variation_মান_Record *) hb_bsearch (tag,
						  (ধ্রুব Variation_মান_Record *)
						    (ধ্রুব HBUINT8 *) মানসমূহ_Z,
						  মান_Record_গুনতি, মান_Record_Size,
						  tag_compare);
    যদি (!record)
      প্রত্যাগম 0.;

    প্রত্যাগম (এতত্+varStore).get_delta (record->varIdx, coords, coord_গুনতি);
  }

রক্ষিত:
  স্থির পূর্ণাঙ্ক tag_compare (ধ্রুব শূন্য *pa, ধ্রুব শূন্য *pb)
  {
    ধ্রুব hb_tag_t *a = (ধ্রুব hb_tag_t *) pa;
    ধ্রুব Tag *b = (ধ্রুব Tag *) pb;
    প্রত্যাগম b->cmp (*a);
  }

  রক্ষিত:
  FixedVersion<>version;	/* Version of the metrics variation table
				 * initially set to 0x00010000u */
  HBUINT16	reserved;	/* Not used; set to 0. */
  HBUINT16	মান_Record_Size;/* The size in bytes of each value record —
				 * must be greater than zero. */
  HBUINT16	মান_Record_গুনতি;/* The number of value records — may be zero. */
  Offset16To<ItemVariationStore>
		varStore;	/* Offset to item variation store table. */
  UnsizedArrayOf<HBUINT8>
		মানসমূহ_Z;	/* Array of value records. The records must be
				 * in binary order of their valueTag field. */

  সার্বজনিক:
  DEFINE_SIZE_ARRAY (12, মানসমূহ_Z);
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
