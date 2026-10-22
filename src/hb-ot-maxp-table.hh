/*
 * Copyright © 2011,2012  Google, Inc.
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

#నిర్వచితం_కాకపోతే HB_OT_MAXP_పట్టిక_HH
#నిర్వచించు HB_OT_MAXP_పట్టిక_HH

#అన్తర్భావ "hb-open-type.hh"
#అన్తర్భావ "hb-ot-dual.hh"

నామాకాశ OT {


/*
 * maxp -- Maximum Profile
 * https://docs.microsoft.com/en-us/typography/opentype/spec/maxp
 */

#నిర్వచించు HB_OT_ట్యాగ్_maxp HB_ట్యాగ్('m','a','x','p')
#నిర్వచించు HB_OT_ట్యాగ్_MAXP HB_ట్యాగ్('M','A','X','P')

నిర్మాణం maxp_V1_తోక
{
  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది));
  }

  HBUINT16 maxPoints;		  /* Maximum points in a non-composite glyph. */
  HBUINT16 maxContours;		  /* Maximum contours in a non-composite glyph. */
  HBUINT16 max_సంయుక్త_Points;	  /* Maximum points in a composite glyph. */
  HBUINT16 max_సంయుక్త_Contours;  /* Maximum contours in a composite glyph. */
  HBUINT16 maxZones;		  /* 1 if instructions do not use the twilight zone (Z0),
				   * or 2 if instructions do use Z0; should be set to 2 in
				   * most cases. */
  HBUINT16 maxTwilightPoints;	  /* Maximum points used in Z0. */
  HBUINT16 maxStorage;		  /* Number of Storage Area locations. */
  HBUINT16 maxFunctionDefs;	  /* Number of FDEFs, equal to the highest function number + 1. */
  HBUINT16 maxInstructionDefs;	  /* Number of IDEFs. */
  HBUINT16 max_దొంతర_Elements;	  /* Maximum stack depth. (This includes Font and CVT
				   * Programs, as well as the instructions for each glyph.) */
  HBUINT16 max_కొలత_యొక్క_Instructions; /* Maximum byte count for glyph instructions. */
  HBUINT16 maxComponentElements;  /* Maximum number of components referenced at
				   * "top level" for any composite glyph. */
  HBUINT16 max_Component_లోతు;	  /* Maximum levels of recursion; 1 for simple components. */
 బహిరంగ:
  DEFINE_కొలత_STATIC (26);
};


మూస <రకం_పేరు Types>
నిర్మాణం maxp_MAXP
{
  గుర్తులేని పూర్ణాంకం get_అంకె_glyphs () స్థిరరాశి { తిరిగి అంకె_Glyphs; }

  శూన్య సమితి_అంకె_glyphs (గుర్తులేని పూర్ణాంకం లెక్క)
  {
    అంకె_Glyphs = లెక్క;
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    ఒకవేళ (unlikely (!c->check_struct (ఇది)))
      return_trace (అబద్ధం);
    hb_barrier ();
    ఒకవేళ (సంస్కరణ.major == 1)
    {
      స్థిరరాశి maxp_V1_తోక &v1 = StructAfter<maxp_V1_తోక> (*ఇది);
      return_trace (v1.sanitize (c));
    }
    return_trace (likely (సంస్కరణ.major == 0 && సంస్కరణ.minor == 0x5000u));
  }

  తార్కిక subset (hb_subset_context_t *c) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    maxp_MAXP *maxp_ప్రధానసంఖ్య = c->serializer->embed (ఇది);
    ఒకవేళ (unlikely (!maxp_ప్రధానసంఖ్య)) return_trace (అబద్ధం);

    maxp_ప్రధానసంఖ్య->అంకె_Glyphs = hb_min (c->plan->అంకె_output_glyphs (),
				    (1u << (8 * Types::కొలత)) - 1);
    ఒకవేళ (maxp_ప్రధానసంఖ్య->సంస్కరణ.major == 1)
    {
      hb_barrier ();
      స్థిరరాశి maxp_V1_తోక *src_v1 = &StructAfter<maxp_V1_తోక> (*ఇది);
      maxp_V1_తోక *dest_v1 = c->serializer->embed<maxp_V1_తోక> (src_v1);
      ఒకవేళ (unlikely (!dest_v1)) return_trace (అబద్ధం);

      ఒకవేళ (c->plan->flags & HB_SUBSET_FLAGS_NO_HINTING)
	drop_hint_fields (dest_v1);

      ఒకవేళ (c->plan->normalized_coords)
        instancing_update_fields (c->plan->తల_maxp_info, dest_v1);
    }

    return_trace (నిజం);
  }

  శూన్య instancing_update_fields (తల_maxp_info_t& maxp_info, maxp_V1_తోక* dest_v1) స్థిరరాశి
  {
    dest_v1->maxPoints = maxp_info.maxPoints;
    dest_v1->maxContours = maxp_info.maxContours;
    dest_v1->max_సంయుక్త_Points = maxp_info.max_సంయుక్త_Points;
    dest_v1->max_సంయుక్త_Contours = maxp_info.max_సంయుక్త_Contours;
    dest_v1->maxComponentElements = maxp_info.maxComponentElements;
    dest_v1->max_Component_లోతు = maxp_info.max_Component_లోతు;
  }

  స్థిర శూన్య drop_hint_fields (maxp_V1_తోక* dest_v1)
  {
    dest_v1->maxZones = 1;
    dest_v1->maxTwilightPoints = 0;
    dest_v1->maxStorage = 0;
    dest_v1->maxFunctionDefs = 0;
    dest_v1->maxInstructionDefs = 0;
    dest_v1->max_దొంతర_Elements = 0;
    dest_v1->max_కొలత_యొక్క_Instructions = 0;
  }

  రక్షిత:
  Fixed_సంస్కరణ<>సంస్కరణ;/* Version of the maxp table (0.5 or 1.0),
			 * 0x00005000u or 0x00010000u. */
  రకం_పేరు Types::HBUINT
		అంకె_Glyphs;
			/* The number of glyphs in the font. */
/*maxpV1Tail	v1Tail[HB_VAR_ARRAY]; */
  బహిరంగ:
  DEFINE_కొలత_STATIC (4 + Types::HBUINT::static_కొలత);
};

నిర్మాణం maxp : maxp_MAXP<Layout::SmallTypes>
{
  స్థిర constexpr hb_ట్యాగ్_t పట్టిక_ట్యాగ్ = HB_OT_ట్యాగ్_maxp;
};

నిర్మాణం MAXP : maxp_MAXP<Layout::MediumTypes>
{
  స్థిర constexpr hb_ట్యాగ్_t పట్టిక_ట్యాగ్ = HB_OT_ట్యాగ్_MAXP;
};

నిర్మాణం maxp_accelerator_t
{
  maxp_accelerator_t ()
  {
    blob = hb_blob_get_ఖాళీలు ();
    అంకె_Glyphs = 0;
  }

  maxp_accelerator_t (hb_face_t *face) : maxp_accelerator_t ()
  {
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
    ఒకవేళ (init_పట్టిక<MAXP> (face))
      తిరిగి;
#అయితే_ముగింపు

    init_పట్టిక<maxp> (face);
  }

  ~maxp_accelerator_t () { hb_blob_destroy (blob); }

  గుర్తులేని పూర్ణాంకం get_అంకె_glyphs () స్థిరరాశి { తిరిగి అంకె_Glyphs; }
  hb_blob_t *get_blob () స్థిరరాశి { తిరిగి blob; }

  అంతరంగ:
  మూస <రకం_పేరు పట్టిక_3>
  తార్కిక init_పట్టిక (hb_face_t *face)
  {
    hb_sanitize_context_t c;
    c.సమితి_అంకె_glyphs (0);
    hb_blob_ptr_t<పట్టిక_3> పట్టిక = c.reference_పట్టిక<పట్టిక_3> (face);
    తార్కిక ret = పట్టిక.get_నిడివి ();
    ఒకవేళ (ret)
    {
      blob = hb_blob_reference (పట్టిక.get_blob ());
      అంకె_Glyphs = పట్టిక->get_అంకె_glyphs ();
    }
    పట్టిక.destroy ();
    తిరిగి ret;
  }

  hb_blob_t *blob;
  గుర్తులేని పూర్ణాంకం అంకె_Glyphs;
};


} /* namespace OT */


#అయితే_ముగింపు /* HB_OT_MAXP_TABLE_HH */
