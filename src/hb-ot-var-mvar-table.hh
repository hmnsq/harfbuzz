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

#અન્તર્ભાવ "hb-ot-var-common.hh"


નામાકાશ OT {


સંરચના Variation_કિંમત_Record
{
  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ));
  }

  તાર્કિક subset (hb_subset_context_t *c,
               અચલ hb_map_t& varidx_map) અચલ
  {
    TRACE_SUBSET (આ);
    auto *out = c->serializer->embed (*આ);
    જો (unlikely (!out)) return_trace (ખોટું);

    hb_codepoint_t *new_idx;
    return_trace (c->serializer->check_assign (out->varIdx,
                                               (varidx_map.has (varIdx, &new_idx)) ? *new_idx : HB_OT_LAYOUT_NO_VARIATIONS_INDEX,
                                               HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  જાહેર:
  Tag		કિંમત_Tag;	/* Four-byte tag identifying a font-wide measure. */
  VarIdx	varIdx;		/* Outer/inner index into ItemVariationStore item. */

  જાહેર:
  DEFINE_SIZE_STATIC (8);
};


/*
 * MVAR -- Metrics Variations
 * https://docs.microsoft.com/en-us/typography/opentype/spec/mvar
 */
#define HB_OT_TAG_MVAR HB_TAG('M','V','A','R')

સંરચના MVAR
{
  સ્થિર constexpr hb_tag_t tableTag = HB_OT_TAG_MVAR;

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (version.sanitize (c) &&
		  hb_barrier () &&
		  likely (version.major == 1) &&
		  c->check_struct (આ) &&
		  hb_barrier () &&
		  કિંમત_Record_Size >= Variation_કિંમત_Record::static_size &&
		  varStore.sanitize (c, આ) &&
		  c->check_range (કિંમતો_Z.arrayZ,
				  કિંમત_Record_ગણતરી,
				  કિંમત_Record_Size));
  }

  તાર્કિક subset (hb_subset_context_t *c) અચલ
  {
    TRACE_SUBSET (આ);
#ifdef HB_NO_VAR
    return_trace (ખોટું);
#endif

    જો (c->plan->all_axes_pinned)
      return_trace (ખોટું);

    MVAR *out = c->serializer->start_embed (*આ);
    જો (unlikely (!c->serializer->extend_min (out))) return_trace (ખોટું);
    out->version = version;
    out->reserved = reserved;
    out->કિંમત_Record_Size = કિંમત_Record_Size;
    out->કિંમત_Record_ગણતરી = કિંમત_Record_ગણતરી;

    item_variations_t item_vars;
    અચલ ItemVariationStore& src_var_store = આ+varStore;

    જો (!item_vars.instantiate (src_var_store, c->plan))
      return_trace (ખોટું);

    /* serialize varstore */
    જો (!out->varStore.serialize_serialize (c->serializer, item_vars.has_long_word (),
                                            c->plan->axis_tags,
                                            item_vars.get_region_list (),
                                            item_vars.get_vardata_encodings ()))
      return_trace (ખોટું);

    /* serialize value records array */
    unsigned કિંમત_rec_ગણતરી = કિંમત_Record_ગણતરી;
    અચલ Variation_કિંમત_Record *record = reinterpret_cast<અચલ Variation_કિંમત_Record*> (કિંમતો_Z.arrayZ);
    માટે (unsigned i = 0; i < કિંમત_rec_ગણતરી; i++)
    {
      જો (!record->subset (c, item_vars.get_varidx_map ())) return_trace (ખોટું);
      record++;
    }
    return_trace (સાચું);
  }

  દશાંશ get_var (hb_tag_t tag,
		 અચલ પૂર્ણાંક *coords, unsigned પૂર્ણાંક coord_ગણતરી) અચલ
  {
    અચલ Variation_કિંમત_Record *record;
    record = (Variation_કિંમત_Record *) hb_bsearch (tag,
						  (અચલ Variation_કિંમત_Record *)
						    (અચલ HBUINT8 *) કિંમતો_Z,
						  કિંમત_Record_ગણતરી, કિંમત_Record_Size,
						  tag_compare);
    જો (!record)
      પાછા 0.;

    પાછા (આ+varStore).get_delta (record->varIdx, coords, coord_ગણતરી);
  }

સુરક્ષિત:
  સ્થિર પૂર્ણાંક tag_compare (અચલ શૂન્ય *pa, અચલ શૂન્ય *pb)
  {
    અચલ hb_tag_t *a = (અચલ hb_tag_t *) pa;
    અચલ Tag *b = (અચલ Tag *) pb;
    પાછા b->cmp (*a);
  }

  સુરક્ષિત:
  FixedVersion<>version;	/* Version of the metrics variation table
				 * initially set to 0x00010000u */
  HBUINT16	reserved;	/* Not used; set to 0. */
  HBUINT16	કિંમત_Record_Size;/* The size in bytes of each value record —
				 * must be greater than zero. */
  HBUINT16	કિંમત_Record_ગણતરી;/* The number of value records — may be zero. */
  Offset16To<ItemVariationStore>
		varStore;	/* Offset to item variation store table. */
  UnsizedArrayOf<HBUINT8>
		કિંમતો_Z;	/* Array of value records. The records must be
				 * in binary order of their valueTag field. */

  જાહેર:
  DEFINE_SIZE_ARRAY (12, કિંમતો_Z);
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
