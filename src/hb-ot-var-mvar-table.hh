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

#अन्तर्भाव "hb-ot-var-common.hh"


नामाकाश OT {


संरचना Variation_मूल्य_Record
{
  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्));
  }

  तार्किक subset (hb_subset_context_t *c,
               ध्रुव hb_map_t& varidx_map) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    auto *out = c->serializer->embed (*एतत्);
    यदि (unlikely (!out)) return_trace (असत्य);

    hb_codepoint_t *new_idx;
    return_trace (c->serializer->check_assign (out->varIdx,
                                               (varidx_map.has (varIdx, &new_idx)) ? *new_idx : HB_OT_LAYOUT_NO_VARIATIONS_INDEX,
                                               HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  सार्वजनिक:
  Tag		मूल्य_Tag;	/* Four-byte tag identifying a font-wide measure. */
  VarIdx	varIdx;		/* Outer/inner index into ItemVariationStore item. */

  सार्वजनिक:
  DEFINE_SIZE_STATIC (8);
};


/*
 * MVAR -- Metrics Variations
 * https://docs.microsoft.com/en-us/typography/opentype/spec/mvar
 */
#define HB_OT_TAG_MVAR HB_TAG('M','V','A','R')

संरचना MVAR
{
  स्थिर constexpr hb_tag_t tableTag = HB_OT_TAG_MVAR;

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (version.sanitize (c) &&
		  hb_barrier () &&
		  likely (version.major == 1) &&
		  c->check_struct (एतत्) &&
		  hb_barrier () &&
		  मूल्य_Record_Size >= Variation_मूल्य_Record::static_size &&
		  varStore.sanitize (c, एतत्) &&
		  c->check_range (मूल्यावली_Z.arrayZ,
				  मूल्य_Record_गिनती,
				  मूल्य_Record_Size));
  }

  तार्किक subset (hb_subset_context_t *c) ध्रुव
  {
    TRACE_SUBSET (एतत्);
#ifdef HB_NO_VAR
    return_trace (असत्य);
#endif

    यदि (c->plan->all_axes_pinned)
      return_trace (असत्य);

    MVAR *out = c->serializer->start_embed (*एतत्);
    यदि (unlikely (!c->serializer->extend_min (out))) return_trace (असत्य);
    out->version = version;
    out->reserved = reserved;
    out->मूल्य_Record_Size = मूल्य_Record_Size;
    out->मूल्य_Record_गिनती = मूल्य_Record_गिनती;

    item_variations_t item_vars;
    ध्रुव ItemVariationStore& src_var_store = एतत्+varStore;

    यदि (!item_vars.instantiate (src_var_store, c->plan))
      return_trace (असत्य);

    /* serialize varstore */
    यदि (!out->varStore.serialize_serialize (c->serializer, item_vars.has_long_word (),
                                            c->plan->axis_tags,
                                            item_vars.get_region_list (),
                                            item_vars.get_vardata_encodings ()))
      return_trace (असत्य);

    /* serialize value records array */
    unsigned मूल्य_rec_गिनती = मूल्य_Record_गिनती;
    ध्रुव Variation_मूल्य_Record *record = reinterpret_cast<ध्रुव Variation_मूल्य_Record*> (मूल्यावली_Z.arrayZ);
    प्रति (unsigned i = 0; i < मूल्य_rec_गिनती; i++)
    {
      यदि (!record->subset (c, item_vars.get_varidx_map ())) return_trace (असत्य);
      record++;
    }
    return_trace (सत्य);
  }

  भिन्न get_var (hb_tag_t tag,
		 ध्रुव पूर्णाङ्क *coords, unsigned पूर्णाङ्क coord_गिनती) ध्रुव
  {
    ध्रुव Variation_मूल्य_Record *record;
    record = (Variation_मूल्य_Record *) hb_bsearch (tag,
						  (ध्रुव Variation_मूल्य_Record *)
						    (ध्रुव HBUINT8 *) मूल्यावली_Z,
						  मूल्य_Record_गिनती, मूल्य_Record_Size,
						  tag_compare);
    यदि (!record)
      प्रत्यागम 0.;

    प्रत्यागम (एतत्+varStore).get_delta (record->varIdx, coords, coord_गिनती);
  }

रक्षित:
  स्थिर पूर्णाङ्क tag_compare (ध्रुव शून्य *pa, ध्रुव शून्य *pb)
  {
    ध्रुव hb_tag_t *a = (ध्रुव hb_tag_t *) pa;
    ध्रुव Tag *b = (ध्रुव Tag *) pb;
    प्रत्यागम b->cmp (*a);
  }

  रक्षित:
  FixedVersion<>version;	/* Version of the metrics variation table
				 * initially set to 0x00010000u */
  HBUINT16	reserved;	/* Not used; set to 0. */
  HBUINT16	मूल्य_Record_Size;/* The size in bytes of each value record —
				 * must be greater than zero. */
  HBUINT16	मूल्य_Record_गिनती;/* The number of value records — may be zero. */
  Offset16To<ItemVariationStore>
		varStore;	/* Offset to item variation store table. */
  UnsizedArrayOf<HBUINT8>
		मूल्यावली_Z;	/* Array of value records. The records must be
				 * in binary order of their valueTag field. */

  सार्वजनिक:
  DEFINE_SIZE_ARRAY (12, मूल्यावली_Z);
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
