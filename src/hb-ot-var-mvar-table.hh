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

#ଅନ୍ତର୍ଭାଵ "hb-ot-var-common.hh"


ନାମାକାଶ OT {


ସଂରଚନା Variation_ମୂଲ୍ୟ_Record
{
  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି));
  }

  ତାର୍କିକ subset (hb_subset_context_t *c,
               ଅଚଳ hb_map_t& varidx_map) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    auto *out = c->serializer->embed (*ଏହି);
    ଯଦି (unlikely (!out)) return_trace (ମିଥ୍ୟା);

    hb_codepoint_t *new_idx;
    return_trace (c->serializer->check_assign (out->varIdx,
                                               (varidx_map.has (varIdx, &new_idx)) ? *new_idx : HB_OT_LAYOUT_NO_VARIATIONS_INDEX,
                                               HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  ସର୍ବସାଧାରଣ:
  Tag		ମୂଲ୍ୟ_Tag;	/* Four-byte tag identifying a font-wide measure. */
  VarIdx	varIdx;		/* Outer/inner index into ItemVariationStore item. */

  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (8);
};


/*
 * MVAR -- Metrics Variations
 * https://docs.microsoft.com/en-us/typography/opentype/spec/mvar
 */
#define HB_OT_TAG_MVAR HB_TAG('M','V','A','R')

ସଂରଚନା MVAR
{
  ସ୍ଥିର constexpr hb_tag_t tableTag = HB_OT_TAG_MVAR;

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (version.sanitize (c) &&
		  hb_barrier () &&
		  likely (version.major == 1) &&
		  c->check_struct (ଏହି) &&
		  hb_barrier () &&
		  ମୂଲ୍ୟ_Record_Size >= Variation_ମୂଲ୍ୟ_Record::static_size &&
		  varStore.sanitize (c, ଏହି) &&
		  c->check_range (ମୂଲ୍ୟଗୁଡ଼ିକ_Z.arrayZ,
				  ମୂଲ୍ୟ_Record_ଗଣନା,
				  ମୂଲ୍ୟ_Record_Size));
  }

  ତାର୍କିକ subset (hb_subset_context_t *c) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
#ifdef HB_NO_VAR
    return_trace (ମିଥ୍ୟା);
#endif

    ଯଦି (c->plan->all_axes_pinned)
      return_trace (ମିଥ୍ୟା);

    MVAR *out = c->serializer->start_embed (*ଏହି);
    ଯଦି (unlikely (!c->serializer->extend_min (out))) return_trace (ମିଥ୍ୟା);
    out->version = version;
    out->reserved = reserved;
    out->ମୂଲ୍ୟ_Record_Size = ମୂଲ୍ୟ_Record_Size;
    out->ମୂଲ୍ୟ_Record_ଗଣନା = ମୂଲ୍ୟ_Record_ଗଣନା;

    item_variations_t item_vars;
    ଅଚଳ ItemVariationStore& src_var_store = ଏହି+varStore;

    ଯଦି (!item_vars.instantiate (src_var_store, c->plan))
      return_trace (ମିଥ୍ୟା);

    /* serialize varstore */
    ଯଦି (!out->varStore.serialize_serialize (c->serializer, item_vars.has_long_word (),
                                            c->plan->axis_tags,
                                            item_vars.get_region_list (),
                                            item_vars.get_vardata_encodings ()))
      return_trace (ମିଥ୍ୟା);

    /* serialize value records array */
    unsigned ମୂଲ୍ୟ_rec_ଗଣନା = ମୂଲ୍ୟ_Record_ଗଣନା;
    ଅଚଳ Variation_ମୂଲ୍ୟ_Record *record = reinterpret_cast<ଅଚଳ Variation_ମୂଲ୍ୟ_Record*> (ମୂଲ୍ୟଗୁଡ଼ିକ_Z.arrayZ);
    ପାଇଁ (unsigned i = 0; i < ମୂଲ୍ୟ_rec_ଗଣନା; i++)
    {
      ଯଦି (!record->subset (c, item_vars.get_varidx_map ())) return_trace (ମିଥ୍ୟା);
      record++;
    }
    return_trace (ସତ୍ୟ);
  }

  ଦଶମିକ get_var (hb_tag_t tag,
		 ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା *coords, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା coord_ଗଣନା) ଅଚଳ
  {
    ଅଚଳ Variation_ମୂଲ୍ୟ_Record *record;
    record = (Variation_ମୂଲ୍ୟ_Record *) hb_bsearch (tag,
						  (ଅଚଳ Variation_ମୂଲ୍ୟ_Record *)
						    (ଅଚଳ HBUINT8 *) ମୂଲ୍ୟଗୁଡ଼ିକ_Z,
						  ମୂଲ୍ୟ_Record_ଗଣନା, ମୂଲ୍ୟ_Record_Size,
						  tag_compare);
    ଯଦି (!record)
      ଫେରନ୍ତୁ 0.;

    ଫେରନ୍ତୁ (ଏହି+varStore).get_delta (record->varIdx, coords, coord_ଗଣନା);
  }

ସୁରକ୍ଷିତ:
  ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା tag_compare (ଅଚଳ ଶୂନ୍ୟ *pa, ଅଚଳ ଶୂନ୍ୟ *pb)
  {
    ଅଚଳ hb_tag_t *a = (ଅଚଳ hb_tag_t *) pa;
    ଅଚଳ Tag *b = (ଅଚଳ Tag *) pb;
    ଫେରନ୍ତୁ b->cmp (*a);
  }

  ସୁରକ୍ଷିତ:
  FixedVersion<>version;	/* Version of the metrics variation table
				 * initially set to 0x00010000u */
  HBUINT16	reserved;	/* Not used; set to 0. */
  HBUINT16	ମୂଲ୍ୟ_Record_Size;/* The size in bytes of each value record —
				 * must be greater than zero. */
  HBUINT16	ମୂଲ୍ୟ_Record_ଗଣନା;/* The number of value records — may be zero. */
  Offset16To<ItemVariationStore>
		varStore;	/* Offset to item variation store table. */
  UnsizedArrayOf<HBUINT8>
		ମୂଲ୍ୟଗୁଡ଼ିକ_Z;	/* Array of value records. The records must be
				 * in binary order of their valueTag field. */

  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_ARRAY (12, ମୂଲ୍ୟଗୁଡ଼ିକ_Z);
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
