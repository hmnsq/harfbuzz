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

#അന്തര്ഭാവ "hb-ot-var-common.hh"


നാമാകാശ OT {


ഘടന Variation_മൂല്യം_Record
{
  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_struct (ഇത്));
  }

  തർക്കം subset (hb_subset_context_t *c,
               സ്ഥിരം hb_map_t& varidx_map) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);
    auto *out = c->serializer->embed (*ഇത്);
    എങ്കിൽ (unlikely (!out)) return_trace (അസത്യം);

    hb_codepoint_t *new_idx;
    return_trace (c->serializer->check_assign (out->varIdx,
                                               (varidx_map.has (varIdx, &new_idx)) ? *new_idx : HB_OT_LAYOUT_NO_VARIATIONS_INDEX,
                                               HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  പൊതു:
  Tag		മൂല്യം_Tag;	/* Four-byte tag identifying a font-wide measure. */
  VarIdx	varIdx;		/* Outer/inner index into ItemVariationStore item. */

  പൊതു:
  DEFINE_SIZE_STATIC (8);
};


/*
 * MVAR -- Metrics Variations
 * https://docs.microsoft.com/en-us/typography/opentype/spec/mvar
 */
#define HB_OT_TAG_MVAR HB_TAG('M','V','A','R')

ഘടന MVAR
{
  സ്ഥിതം constexpr hb_tag_t tableTag = HB_OT_TAG_MVAR;

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (version.sanitize (c) &&
		  hb_barrier () &&
		  likely (version.major == 1) &&
		  c->check_struct (ഇത്) &&
		  hb_barrier () &&
		  മൂല്യം_Record_Size >= Variation_മൂല്യം_Record::static_size &&
		  varStore.sanitize (c, ഇത്) &&
		  c->check_range (മൂല്യങ്ങൾ_Z.arrayZ,
				  മൂല്യം_Record_എണ്ണം,
				  മൂല്യം_Record_Size));
  }

  തർക്കം subset (hb_subset_context_t *c) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);
#ifdef HB_NO_VAR
    return_trace (അസത്യം);
#endif

    എങ്കിൽ (c->plan->all_axes_pinned)
      return_trace (അസത്യം);

    MVAR *out = c->serializer->start_embed (*ഇത്);
    എങ്കിൽ (unlikely (!c->serializer->extend_min (out))) return_trace (അസത്യം);
    out->version = version;
    out->reserved = reserved;
    out->മൂല്യം_Record_Size = മൂല്യം_Record_Size;
    out->മൂല്യം_Record_എണ്ണം = മൂല്യം_Record_എണ്ണം;

    item_variations_t item_vars;
    സ്ഥിരം ItemVariationStore& src_var_store = ഇത്+varStore;

    എങ്കിൽ (!item_vars.instantiate (src_var_store, c->plan))
      return_trace (അസത്യം);

    /* serialize varstore */
    എങ്കിൽ (!out->varStore.serialize_serialize (c->serializer, item_vars.has_long_word (),
                                            c->plan->axis_tags,
                                            item_vars.get_region_list (),
                                            item_vars.get_vardata_encodings ()))
      return_trace (അസത്യം);

    /* serialize value records array */
    unsigned മൂല്യം_rec_എണ്ണം = മൂല്യം_Record_എണ്ണം;
    സ്ഥിരം Variation_മൂല്യം_Record *record = reinterpret_cast<സ്ഥിരം Variation_മൂല്യം_Record*> (മൂല്യങ്ങൾ_Z.arrayZ);
    വേണ്ടി (unsigned i = 0; i < മൂല്യം_rec_എണ്ണം; i++)
    {
      എങ്കിൽ (!record->subset (c, item_vars.get_varidx_map ())) return_trace (അസത്യം);
      record++;
    }
    return_trace (സത്യം);
  }

  ദശാംശം get_var (hb_tag_t tag,
		 സ്ഥിരം പൂർണ്ണസംഖ്യ *coords, unsigned പൂർണ്ണസംഖ്യ coord_എണ്ണം) സ്ഥിരം
  {
    സ്ഥിരം Variation_മൂല്യം_Record *record;
    record = (Variation_മൂല്യം_Record *) hb_bsearch (tag,
						  (സ്ഥിരം Variation_മൂല്യം_Record *)
						    (സ്ഥിരം HBUINT8 *) മൂല്യങ്ങൾ_Z,
						  മൂല്യം_Record_എണ്ണം, മൂല്യം_Record_Size,
						  tag_compare);
    എങ്കിൽ (!record)
      തിരികെ 0.;

    തിരികെ (ഇത്+varStore).get_delta (record->varIdx, coords, coord_എണ്ണം);
  }

സംരക്ഷിത:
  സ്ഥിതം പൂർണ്ണസംഖ്യ tag_compare (സ്ഥിരം ശൂന്യം *pa, സ്ഥിരം ശൂന്യം *pb)
  {
    സ്ഥിരം hb_tag_t *a = (സ്ഥിരം hb_tag_t *) pa;
    സ്ഥിരം Tag *b = (സ്ഥിരം Tag *) pb;
    തിരികെ b->cmp (*a);
  }

  സംരക്ഷിത:
  FixedVersion<>version;	/* Version of the metrics variation table
				 * initially set to 0x00010000u */
  HBUINT16	reserved;	/* Not used; set to 0. */
  HBUINT16	മൂല്യം_Record_Size;/* The size in bytes of each value record —
				 * must be greater than zero. */
  HBUINT16	മൂല്യം_Record_എണ്ണം;/* The number of value records — may be zero. */
  Offset16To<ItemVariationStore>
		varStore;	/* Offset to item variation store table. */
  UnsizedArrayOf<HBUINT8>
		മൂല്യങ്ങൾ_Z;	/* Array of value records. The records must be
				 * in binary order of their valueTag field. */

  പൊതു:
  DEFINE_SIZE_ARRAY (12, മൂല്യങ്ങൾ_Z);
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
