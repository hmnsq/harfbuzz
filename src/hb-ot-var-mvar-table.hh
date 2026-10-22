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

#अगर_अघोषित HB_OT_VAR_MVAR_TABLE_HH
#घोषणा HB_OT_VAR_MVAR_TABLE_HH

#समावेश "hb-ot-var-common.hh"


namespace OT {


काष्ठा Variation_मूल्य_Record
{
  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह));
  }

  bool subset (hb_subset_context_t *c,
               स्थिर hb_map_t& varidx_map) स्थिर
  {
    TRACE_SUBSET (यह);
    स्वतः *out = c->serializer->embed (*यह);
    अगर (unlikely (!out)) return_trace (false);

    hb_codepoint_t *new_idx;
    return_trace (c->serializer->check_assign (out->varIdx,
                                               (varidx_map.has (varIdx, &new_idx)) ? *new_idx : HB_OT_LAYOUT_NO_VARIATIONS_सूचकांक,
                                               HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  खुला:
  Tag		मूल्य_Tag;	/* Four-byte tag identifying a font-wide measure. */
  VarIdx	varIdx;		/* Outer/inner index into ItemVariationStore item. */

  खुला:
  DEFINE_आकार_STATIC (8);
};


/*
 * MVAR -- Metrics Variations
 * https://docs.microsoft.com/en-us/typography/opentype/spec/mvar
 */
#घोषणा HB_OT_TAG_MVAR HB_TAG('M','V','A','R')

काष्ठा MVAR
{
  अटल constexpr hb_tag_t tableTag = HB_OT_TAG_MVAR;

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (version.sanitize (c) &&
		  hb_barrier () &&
		  likely (version.major == 1) &&
		  c->check_struct (यह) &&
		  hb_barrier () &&
		  मूल्य_Record_आकार >= Variation_मूल्य_Record::static_आकार &&
		  varStore.sanitize (c, यह) &&
		  c->check_range (मूल्यावली_Z.arrayZ,
				  मूल्य_Record_गिनती,
				  मूल्य_Record_आकार));
  }

  bool subset (hb_subset_context_t *c) स्थिर
  {
    TRACE_SUBSET (यह);
#अगर_घोषित HB_NO_VAR
    return_trace (false);
#पूर्ण_अगर

    अगर (c->plan->all_axes_pinned)
      return_trace (false);

    MVAR *out = c->serializer->start_embed (*यह);
    अगर (unlikely (!c->serializer->extend_min (out))) return_trace (false);
    out->version = version;
    out->reserved = reserved;
    out->मूल्य_Record_आकार = मूल्य_Record_आकार;
    out->मूल्य_Record_गिनती = मूल्य_Record_गिनती;

    item_variations_t item_vars;
    स्थिर ItemVariationStore& src_var_store = यह+varStore;

    अगर (!item_vars.instantiate (src_var_store, c->plan))
      return_trace (false);

    /* serialize varstore */
    अगर (!out->varStore.serialize_serialize (c->serializer, item_vars.has_long_word (),
                                            c->plan->axis_tags,
                                            item_vars.get_region_list (),
                                            item_vars.get_vardata_encodings ()))
      return_trace (false);

    /* serialize value records array */
    अचिन्हित मूल्य_rec_गिनती = मूल्य_Record_गिनती;
    स्थिर Variation_मूल्य_Record *record = reinterpret_cast<स्थिर Variation_मूल्य_Record*> (मूल्यावली_Z.arrayZ);
    क्रम (अचिन्हित i = 0; i < मूल्य_rec_गिनती; i++)
    {
      अगर (!record->subset (c, item_vars.get_varidx_map ())) return_trace (false);
      record++;
    }
    return_trace (true);
  }

  भग्न get_var (hb_tag_t tag,
		 स्थिर पूर्णांक *coords, अचिन्हित पूर्णांक coord_गिनती) स्थिर
  {
    स्थिर Variation_मूल्य_Record *record;
    record = (Variation_मूल्य_Record *) hb_bsearch (tag,
						  (स्थिर Variation_मूल्य_Record *)
						    (स्थिर HBUINT8 *) मूल्यावली_Z,
						  मूल्य_Record_गिनती, मूल्य_Record_आकार,
						  tag_compare);
    अगर (!record)
      वापस 0.;

    वापस (यह+varStore).get_delta (record->varIdx, coords, coord_गिनती);
  }

रक्षित:
  अटल पूर्णांक tag_compare (स्थिर व्योम *pa, स्थिर व्योम *pb)
  {
    स्थिर hb_tag_t *a = (स्थिर hb_tag_t *) pa;
    स्थिर Tag *b = (स्थिर Tag *) pb;
    वापस b->cmp (*a);
  }

  रक्षित:
  FixedVersion<>version;	/* Version of the metrics variation table
				 * initially set to 0x00010000u */
  HBUINT16	reserved;	/* Not used; set to 0. */
  HBUINT16	मूल्य_Record_आकार;/* The size in bytes of each value record —
				 * must be greater than zero. */
  HBUINT16	मूल्य_Record_गिनती;/* The number of value records — may be zero. */
  Offset16To<ItemVariationStore>
		varStore;	/* Offset to item variation store table. */
  UnsizedArrayOf<HBUINT8>
		मूल्यावली_Z;	/* Array of value records. The records must be
				 * in binary order of their valueTag field. */

  खुला:
  DEFINE_आकार_ARRAY (12, मूल्यावली_Z);
};

} /* namespace OT */


#घोषणा HB_ADD_MVAR_VAR(tag, field) \
       c->serializer->check_assign (table->field, \
				    roundf (table->field + \
					    MVAR.get_var (tag, \
							  c->plan->normalized_coords.arrayZ, \
							  c->plan->normalized_coords.लंबाई)), \
				    HB_SERIALIZE_ERROR_INT_OVERFLOW)


#पूर्ण_अगर /* HB_OT_VAR_MVAR_TABLE_HH */
