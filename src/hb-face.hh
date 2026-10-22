/*
 * Copyright © 2009  Red Hat, Inc.
 * Copyright © 2011  Google, Inc.
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
 * Google Author(s): Behdad Esfahbod
 */

#నిర్వచితం_కాకపోతే HB_FACE_HH
#నిర్వచించు HB_FACE_HH

#అన్తర్భావ "hb.hh"

#అన్తర్భావ "hb-shaper.hh"
#అన్తర్భావ "hb-shape-plan.hh"
#అన్తర్భావ "hb-ot-face.hh"


/*
 * hb_face_t
 */

#నిర్వచించు HB_SHAPER_IMPLEMENT(shaper) HB_SHAPER_దత్తాంశం_INSTANTIATE_SHAPERS(shaper, face);
#అన్తర్భావ "hb-shaper-list.hh"
#నిర్వచనం_రద్దు HB_SHAPER_IMPLEMENT

నిర్మాణం hb_face_t
{
  hb_object_శీర్షిక_t శీర్షిక;

  గుర్తులేని పూర్ణాంకం సూచీ_2;			/* Face index in a collection, zero-based. */
  మార్చదగిన hb_atomic_t<గుర్తులేని> upem;	/* Units-per-EM. */
  మార్చదగిన hb_atomic_t<గుర్తులేని> అంకె_glyphs;/* Number of glyphs. */

  hb_reference_పట్టిక_func_t  reference_పట్టిక_func;
  శూన్య                      *user_దత్తాంశం;
  hb_destroy_func_t          destroy;

  hb_get_పట్టిక_ట్యాగులు_func_t   get_పట్టిక_ట్యాగులు_func;
  శూన్య                      *get_పట్టిక_ట్యాగులు_user_దత్తాంశం;
  hb_destroy_func_t          get_పట్టిక_ట్యాగులు_destroy;

  hb_shaper_object_dataset_t<hb_face_t> దత్తాంశం;/* Various shaper data. */
  hb_ot_face_t పట్టిక;			/* All the face's tables. */

  /* Cache */
  నిర్మాణం plan_కణుపు_t
  {
    hb_shape_plan_t *shape_plan;
    plan_కణుపు_t *next;
  };
#నిర్వచితం_కాకపోతే HB_NO_SHAPER
  hb_atomic_t<plan_కణుపు_t *> shape_plans;
#అయితే_ముగింపు

  hb_blob_t *reference_పట్టిక (hb_ట్యాగ్_t ట్యాగ్_2) స్థిరరాశి
  {
    hb_blob_t *blob;

    ఒకవేళ (unlikely (!reference_పట్టిక_func))
      తిరిగి hb_blob_get_ఖాళీలు ();

    blob = reference_పట్టిక_func (/*Oh, well.*/const_cast<hb_face_t *> (ఇది), ట్యాగ్_2, user_దత్తాంశం);
    ఒకవేళ (unlikely (!blob))
      తిరిగి hb_blob_get_ఖాళీలు ();

    తిరిగి blob;
  }

  గుర్తులేని పూర్ణాంకం get_upem () స్థిరరాశి
  {
    గుర్తులేని పూర్ణాంకం ret = upem;
    ఒకవేళ (unlikely (!ret))
    {
      తిరిగి ఎక్కించు_upem ();
    }
    తిరిగి ret;
  }

  గుర్తులేని పూర్ణాంకం get_అంకె_glyphs () స్థిరరాశి
  {
    గుర్తులేని పూర్ణాంకం ret = అంకె_glyphs;
    ఒకవేళ (unlikely (ret == UINT_MAX))
      తిరిగి ఎక్కించు_అంకె_glyphs ();
    తిరిగి ret;
  }

  అంతరంగ:
  HB_అంతర్గత గుర్తులేని పూర్ణాంకం ఎక్కించు_upem () స్థిరరాశి;
  HB_అంతర్గత గుర్తులేని పూర్ణాంకం ఎక్కించు_అంకె_glyphs () స్థిరరాశి;
};
DECLARE_NULL_INSTANCE (hb_face_t);


#అయితే_ముగింపు /* HB_FACE_HH */
