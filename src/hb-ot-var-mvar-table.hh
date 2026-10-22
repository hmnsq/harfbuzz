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

#నిర్వచితం_కాకపోతే HB_OT_VAR_MVAR_పట్టిక_HH
#నిర్వచించు HB_OT_VAR_MVAR_పట్టిక_HH

#అన్తర్భావ "hb-ot-var-common.hh"


నామాకాశ OT {


నిర్మాణం Variation_విలువ_రికార్డు
{
  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది));
  }

  తార్కిక subset (hb_subset_context_t *c,
               స్థిరరాశి hb_map_t& varidx_map) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    స్వయంచాలక *బహిర్గతం = c->serializer->embed (*ఇది);
    ఒకవేళ (unlikely (!బహిర్గతం)) return_trace (అబద్ధం);

    hb_codepoint_t *new_idx;
    return_trace (c->serializer->check_కేటాయించు (బహిర్గతం->varIdx,
                                               (varidx_map.has (varIdx, &new_idx)) ? *new_idx : HB_OT_LAYOUT_NO_VARIATIONS_సూచీ,
                                               HB_SERIALIZE_ERROR_INT_అధికప్రవాహం));
  }

  బహిరంగ:
  ట్యాగ్		విలువ_ట్యాగ్;	/* Four-byte tag identifying a font-wide measure. */
  VarIdx	varIdx;		/* Outer/inner index into ItemVariationStore item. */

  బహిరంగ:
  DEFINE_కొలత_STATIC (8);
};


/*
 * MVAR -- Metrics Variations
 * https://docs.microsoft.com/en-us/typography/opentype/spec/mvar
 */
#నిర్వచించు HB_OT_ట్యాగ్_MVAR HB_ట్యాగ్('M','V','A','R')

నిర్మాణం MVAR
{
  స్థిర constexpr hb_ట్యాగ్_t పట్టిక_ట్యాగ్ = HB_OT_ట్యాగ్_MVAR;

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (సంస్కరణ.sanitize (c) &&
		  hb_barrier () &&
		  likely (సంస్కరణ.major == 1) &&
		  c->check_struct (ఇది) &&
		  hb_barrier () &&
		  విలువ_రికార్డు_కొలత >= Variation_విలువ_రికార్డు::static_కొలత &&
		  varStore.sanitize (c, ఇది) &&
		  c->check_range (విలువలు_Z.arrayZ,
				  విలువ_రికార్డు_లెక్క,
				  విలువ_రికార్డు_కొలత));
  }

  తార్కిక subset (hb_subset_context_t *c) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
#నిర్వచితమైతే HB_NO_VAR
    return_trace (అబద్ధం);
#అయితే_ముగింపు

    ఒకవేళ (c->plan->all_axes_pinned)
      return_trace (అబద్ధం);

    MVAR *బహిర్గతం = c->serializer->ప్రారంభం_embed (*ఇది);
    ఒకవేళ (unlikely (!c->serializer->extend_min (బహిర్గతం))) return_trace (అబద్ధం);
    బహిర్గతం->సంస్కరణ = సంస్కరణ;
    బహిర్గతం->reserved = reserved;
    బహిర్గతం->విలువ_రికార్డు_కొలత = విలువ_రికార్డు_కొలత;
    బహిర్గతం->విలువ_రికార్డు_లెక్క = విలువ_రికార్డు_లెక్క;

    అంశం_variations_t అంశం_vars;
    స్థిరరాశి అంశం_Variation_Store& src_var_store = ఇది+varStore;

    ఒకవేళ (!అంశం_vars.instantiate (src_var_store, c->plan))
      return_trace (అబద్ధం);

    /* serialize varstore */
    ఒకవేళ (!బహిర్గతం->varStore.serialize_serialize (c->serializer, అంశం_vars.has_long_మాట (),
                                            c->plan->axis_ట్యాగులు,
                                            అంశం_vars.get_region_list (),
                                            అంశం_vars.get_vardata_encodings ()))
      return_trace (అబద్ధం);

    /* serialize value records array */
    గుర్తులేని విలువ_rec_లెక్క = విలువ_రికార్డు_లెక్క;
    స్థిరరాశి Variation_విలువ_రికార్డు *రికార్డు = reinterpret_cast<స్థిరరాశి Variation_విలువ_రికార్డు*> (విలువలు_Z.arrayZ);
    కొరకు (గుర్తులేని i = 0; i < విలువ_rec_లెక్క; i++)
    {
      ఒకవేళ (!రికార్డు->subset (c, అంశం_vars.get_varidx_map ())) return_trace (అబద్ధం);
      రికార్డు++;
    }
    return_trace (నిజం);
  }

  దశాంశం get_var (hb_ట్యాగ్_t ట్యాగ్_2,
		 స్థిరరాశి పూర్ణాంకం *coords, గుర్తులేని పూర్ణాంకం coord_లెక్క) స్థిరరాశి
  {
    స్థిరరాశి Variation_విలువ_రికార్డు *రికార్డు;
    రికార్డు = (Variation_విలువ_రికార్డు *) hb_bsearch (ట్యాగ్_2,
						  (స్థిరరాశి Variation_విలువ_రికార్డు *)
						    (స్థిరరాశి HBUINT8 *) విలువలు_Z,
						  విలువ_రికార్డు_లెక్క, విలువ_రికార్డు_కొలత,
						  ట్యాగ్_compare);
    ఒకవేళ (!రికార్డు)
      తిరిగి 0.;

    తిరిగి (ఇది+varStore).get_delta (రికార్డు->varIdx, coords, coord_లెక్క);
  }

రక్షిత:
  స్థిర పూర్ణాంకం ట్యాగ్_compare (స్థిరరాశి శూన్య *pa, స్థిరరాశి శూన్య *pb)
  {
    స్థిరరాశి hb_ట్యాగ్_t *a = (స్థిరరాశి hb_ట్యాగ్_t *) pa;
    స్థిరరాశి ట్యాగ్ *b = (స్థిరరాశి ట్యాగ్ *) pb;
    తిరిగి b->cmp (*a);
  }

  రక్షిత:
  Fixed_సంస్కరణ<>సంస్కరణ;	/* Version of the metrics variation table
				 * initially set to 0x00010000u */
  HBUINT16	reserved;	/* Not used; set to 0. */
  HBUINT16	విలువ_రికార్డు_కొలత;/* The size in bytes of each value record —
				 * must be greater than zero. */
  HBUINT16	విలువ_రికార్డు_లెక్క;/* The number of value records — may be zero. */
  Offset16_గమ్యం<అంశం_Variation_Store>
		varStore;	/* Offset to item variation store table. */
  Unsized_Array_యొక్క<HBUINT8>
		విలువలు_Z;	/* Array of value records. The records must be
				 * in binary order of their valueTag field. */

  బహిరంగ:
  DEFINE_కొలత_ARRAY (12, విలువలు_Z);
};

} /* namespace OT */


#నిర్వచించు HB_కూడు_MVAR_VAR(ట్యాగ్_2, క్షేత్రం) \
       c->serializer->check_కేటాయించు (పట్టిక->క్షేత్రం, \
				    roundf (పట్టిక->క్షేత్రం + \
					    MVAR.get_var (ట్యాగ్_2, \
							  c->plan->normalized_coords.arrayZ, \
							  c->plan->normalized_coords.నిడివి)), \
				    HB_SERIALIZE_ERROR_INT_అధికప్రవాహం)


#అయితే_ముగింపు /* HB_OT_VAR_MVAR_TABLE_HH */
