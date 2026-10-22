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

#அந்தர்பாவ "hb-ot-var-common.hh"


நாமாகாஶ OT {


கட்டமைப்பு Variation_மதிப்பு_Record
{
  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது));
  }

  தருக்க subset (hb_subset_context_t *c,
               மாறிலி hb_map_t& varidx_map) மாறிலி
  {
    TRACE_SUBSET (இது);
    auto *out = c->serializer->embed (*இது);
    எனில் (unlikely (!out)) return_trace (பொய்);

    hb_codepoint_t *new_idx;
    return_trace (c->serializer->check_assign (out->varIdx,
                                               (varidx_map.has (varIdx, &new_idx)) ? *new_idx : HB_OT_LAYOUT_NO_VARIATIONS_INDEX,
                                               HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  பொது:
  Tag		மதிப்பு_Tag;	/* Four-byte tag identifying a font-wide measure. */
  VarIdx	varIdx;		/* Outer/inner index into ItemVariationStore item. */

  பொது:
  DEFINE_SIZE_STATIC (8);
};


/*
 * MVAR -- Metrics Variations
 * https://docs.microsoft.com/en-us/typography/opentype/spec/mvar
 */
#define HB_OT_TAG_MVAR HB_TAG('M','V','A','R')

கட்டமைப்பு MVAR
{
  நிலையான constexpr hb_tag_t tableTag = HB_OT_TAG_MVAR;

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (version.sanitize (c) &&
		  hb_barrier () &&
		  likely (version.major == 1) &&
		  c->check_struct (இது) &&
		  hb_barrier () &&
		  மதிப்பு_Record_Size >= Variation_மதிப்பு_Record::static_size &&
		  varStore.sanitize (c, இது) &&
		  c->check_range (மதிப்புகள்_Z.arrayZ,
				  மதிப்பு_Record_மொத்தம்,
				  மதிப்பு_Record_Size));
  }

  தருக்க subset (hb_subset_context_t *c) மாறிலி
  {
    TRACE_SUBSET (இது);
#ifdef HB_NO_VAR
    return_trace (பொய்);
#endif

    எனில் (c->plan->all_axes_pinned)
      return_trace (பொய்);

    MVAR *out = c->serializer->start_embed (*இது);
    எனில் (unlikely (!c->serializer->extend_min (out))) return_trace (பொய்);
    out->version = version;
    out->reserved = reserved;
    out->மதிப்பு_Record_Size = மதிப்பு_Record_Size;
    out->மதிப்பு_Record_மொத்தம் = மதிப்பு_Record_மொத்தம்;

    item_variations_t item_vars;
    மாறிலி ItemVariationStore& src_var_store = இது+varStore;

    எனில் (!item_vars.instantiate (src_var_store, c->plan))
      return_trace (பொய்);

    /* serialize varstore */
    எனில் (!out->varStore.serialize_serialize (c->serializer, item_vars.has_long_word (),
                                            c->plan->axis_tags,
                                            item_vars.get_region_list (),
                                            item_vars.get_vardata_encodings ()))
      return_trace (பொய்);

    /* serialize value records array */
    unsigned மதிப்பு_rec_மொத்தம் = மதிப்பு_Record_மொத்தம்;
    மாறிலி Variation_மதிப்பு_Record *record = reinterpret_cast<மாறிலி Variation_மதிப்பு_Record*> (மதிப்புகள்_Z.arrayZ);
    க்கு (unsigned i = 0; i < மதிப்பு_rec_மொத்தம்; i++)
    {
      எனில் (!record->subset (c, item_vars.get_varidx_map ())) return_trace (பொய்);
      record++;
    }
    return_trace (உண்மை);
  }

  தசம get_var (hb_tag_t tag,
		 மாறிலி முழு *coords, unsigned முழு coord_மொத்தம்) மாறிலி
  {
    மாறிலி Variation_மதிப்பு_Record *record;
    record = (Variation_மதிப்பு_Record *) hb_bsearch (tag,
						  (மாறிலி Variation_மதிப்பு_Record *)
						    (மாறிலி HBUINT8 *) மதிப்புகள்_Z,
						  மதிப்பு_Record_மொத்தம், மதிப்பு_Record_Size,
						  tag_compare);
    எனில் (!record)
      திரும்பு 0.;

    திரும்பு (இது+varStore).get_delta (record->varIdx, coords, coord_மொத்தம்);
  }

பாதுகாக்கப்பட்ட:
  நிலையான முழு tag_compare (மாறிலி வெறுமை *pa, மாறிலி வெறுமை *pb)
  {
    மாறிலி hb_tag_t *a = (மாறிலி hb_tag_t *) pa;
    மாறிலி Tag *b = (மாறிலி Tag *) pb;
    திரும்பு b->cmp (*a);
  }

  பாதுகாக்கப்பட்ட:
  FixedVersion<>version;	/* Version of the metrics variation table
				 * initially set to 0x00010000u */
  HBUINT16	reserved;	/* Not used; set to 0. */
  HBUINT16	மதிப்பு_Record_Size;/* The size in bytes of each value record —
				 * must be greater than zero. */
  HBUINT16	மதிப்பு_Record_மொத்தம்;/* The number of value records — may be zero. */
  Offset16To<ItemVariationStore>
		varStore;	/* Offset to item variation store table. */
  UnsizedArrayOf<HBUINT8>
		மதிப்புகள்_Z;	/* Array of value records. The records must be
				 * in binary order of their valueTag field. */

  பொது:
  DEFINE_SIZE_ARRAY (12, மதிப்புகள்_Z);
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
