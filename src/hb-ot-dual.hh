/*
 * Copyright © 2007,2008,2009  Red Hat, Inc.
 * Copyright © 2010,2012  Google, Inc.
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
 * Red Hat Author(s): Behdad Esfahbod
 * Google Author(s): Behdad Esfahbod, Garret Rieger
 */

#నిర్వచితం_కాకపోతే HB_OT_DUAL_HH
#నిర్వచించు HB_OT_DUAL_HH

#అన్తర్భావ "hb-open-type.hh"


నామాకాశ OT {
నామాకాశ Layout {

నిర్మాణం SmallTypes {
  స్థిర constexpr గుర్తులేని కొలత = 2;
  ఉపయోగించి large_int = uint32_t;
  ఉపయోగించి HBUINT = HBUINT16;
  ఉపయోగించి HBLUINT = HBUINT16;
  ఉపయోగించి HBGlyphID = HBGlyphID16;
  ఉపయోగించి స్థానభ్రంశం_2 = Offset16;
  ఉపయోగించి LOffset = Offset16;
  మూస <రకం_పేరు Type, రకం_పేరు పునాది_Type=శూన్య, తార్కిక has_null=నిజం>
  ఉపయోగించి స్థానభ్రంశం_గమ్యం = OT::Offset16_గమ్యం<Type, పునాది_Type, has_null>;
  మూస <రకం_పేరు Type, రకం_పేరు పునాది_Type=శూన్య, తార్కిక has_null=నిజం>
  ఉపయోగించి LOffset_గమ్యం = OT::Offset16_గమ్యం<Type, పునాది_Type, has_null>;
  మూస <రకం_పేరు Type>
  ఉపయోగించి Array_యొక్క = OT::Array16_యొక్క<Type>;
  మూస <రకం_పేరు Type>
  ఉపయోగించి Sorted_Array_యొక్క = OT::Sorted_Array16_యొక్క<Type>;
};

నిర్మాణం MediumTypes {
  స్థిర constexpr గుర్తులేని కొలత = 3;
  ఉపయోగించి large_int = uint64_t;
  ఉపయోగించి HBUINT = HBUINT24;
  ఉపయోగించి HBLUINT = HBUINT32;
  ఉపయోగించి HBGlyphID = HBGlyphID24;
  ఉపయోగించి స్థానభ్రంశం_2 = Offset24;
  /* Long offsets for large tables. */
  ఉపయోగించి LOffset = Offset32;
  మూస <రకం_పేరు Type, రకం_పేరు పునాది_Type=శూన్య, తార్కిక has_null=నిజం>
  ఉపయోగించి స్థానభ్రంశం_గమ్యం = OT::Offset24_గమ్యం<Type, పునాది_Type, has_null>;
  మూస <రకం_పేరు Type, రకం_పేరు పునాది_Type=శూన్య, తార్కిక has_null=నిజం>
  ఉపయోగించి LOffset_గమ్యం = OT::Offset32_గమ్యం<Type, పునాది_Type, has_null>;
  మూస <రకం_పేరు Type>
  ఉపయోగించి Array_యొక్క = OT::Array24_యొక్క<Type>;
  మూస <రకం_పేరు Type>
  ఉపయోగించి Sorted_Array_యొక్క = OT::Sorted_Array24_యొక్క<Type>;
};

}
}


మూస <రకం_పేరు క్రింది_2, రకం_పేరు పై_2>
నిర్మాణం hb_dual_accelerator_t
{
  hb_dual_accelerator_t (hb_face_t *face) :
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
    పై (face),
#అయితే_ముగింపు
    క్రింది (
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
	   పై.has_దత్తాంశం () ? hb_face_get_ఖాళీలు () :
#అయితే_ముగింపు
	   face)
  {}

  తార్కిక has_దత్తాంశం () స్థిరరాశి
  {
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
    ఒకవేళ (పై.has_దత్తాంశం ()) తిరిగి నిజం;
#అయితే_ముగింపు
    తిరిగి క్రింది.has_దత్తాంశం ();
  }

  తార్కిక has_పై_దత్తాంశం () స్థిరరాశి
  {
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
    తిరిగి పై.has_దత్తాంశం ();
#లేకపోతే
    తిరిగి అబద్ధం;
#అయితే_ముగింపు
  }

#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
  పై_2 పై;
#అయితే_ముగింపు
  క్రింది_2 క్రింది;
};

మూస <రకం_పేరు పట్టిక_3>
నిర్మాణం hb_పట్టిక_accelerator_t
{
  hb_పట్టిక_accelerator_t (hb_face_t *face) :
    పట్టిక (hb_sanitize_context_t ().reference_పట్టిక<పట్టిక_3> (face))
  {}
  ~hb_పట్టిక_accelerator_t () { పట్టిక.destroy (); }

  తార్కిక has_దత్తాంశం () స్థిరరాశి { తిరిగి పట్టిక->has_దత్తాంశం (); }

  hb_blob_ptr_t<పట్టిక_3> పట్టిక;
};

మూస <రకం_పేరు క్రింది_2, రకం_పేరు పై_2>
నిర్మాణం hb_dual_పట్టిక_t : hb_dual_accelerator_t<hb_పట్టిక_accelerator_t<క్రింది_2>,
					       hb_పట్టిక_accelerator_t<పై_2>>
{
  hb_dual_పట్టిక_t (hb_face_t *face) :
    hb_dual_accelerator_t<hb_పట్టిక_accelerator_t<క్రింది_2>,
			  hb_పట్టిక_accelerator_t<పై_2>> (face) {}
};

#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
#నిర్వచించు HB_DUAL_GET(accel, సమాసం) ((accel).పై.has_దత్తాంశం () ? (accel).పై.సమాసం : (accel).క్రింది.సమాసం)
#లేకపోతే
#నిర్వచించు HB_DUAL_GET(accel, సమాసం) ((accel).క్రింది.సమాసం)
#అయితే_ముగింపు


#అయితే_ముగింపు  /* HB_OT_DUAL_HH */
