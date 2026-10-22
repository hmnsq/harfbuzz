/*
 * Copyright © 2026  Behdad Esfahbod
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
 */

#అన్తర్భావ "hb.hh"
#అన్తర్భావ "hb-ot-layout-jstf-table.hh"

స్థిర hb_face_t *
create_face (స్థిరరాశి అక్షరం *దత్తాంశం, గుర్తులేని నిడివి)
{
  hb_face_t *face = hb_face_builder_create ();
  hb_blob_t *blob = hb_blob_create (దత్తాంశం, నిడివి, HB_జ్ఞాపకం_బహుళకం_READONLY,
				    శూన్యసూచిక, శూన్యసూచిక);
  hb_always_assert (hb_face_builder_కూడు_పట్టిక (face, HB_OT_ట్యాగ్_JSTF, blob));
  hb_blob_destroy (blob);
  తిరిగి face;
}

పూర్ణాంకం
ప్రధాన (పూర్ణాంకం argc, అక్షరం **argv)
{
  స్థిర స్థిరరాశి అక్షరం దత్తాంశం[] = {
    0, 1, 0, 1,				/* version 1.1 */
    0, 0,				/* JstfScriptRecord count */
    0, 1,				/* JstfScriptRecord2 count */
    'a', 'r', 'a', 'b', 0, 0, 0, 16,	/* JstfScriptRecord2 */
    0, 0, 0, 10,			/* ExtenderGlyph2 offset */
    0, 0, 0, 0,				/* default JstfLangSys offset */
    0, 0,				/* JstfLangSysRecord count */
    0, 1, 1, 0, 1,			/* ExtenderGlyph2 */
  };

  hb_face_t *face = create_face (దత్తాంశం, పరిమాణం (దత్తాంశం));
  hb_blob_ptr_t<OT::JSTF> jstf =
      hb_sanitize_context_t ().reference_పట్టిక<OT::JSTF> (face);
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
  hb_always_assert (jstf.get_నిడివి ());
  hb_always_assert (jstf->get_script_లెక్క () == 1);
  hb_always_assert (jstf->get_script_ట్యాగ్ (0) == HB_ట్యాగ్ ('a','r','a','b'));
  గుర్తులేని సూచీ_2;
  hb_always_assert (jstf->కనుగొను_script_సూచీ (HB_ట్యాగ్ ('a','r','a','b'), &సూచీ_2));
  hb_always_assert (సూచీ_2 == 0);
  hb_always_assert (!jstf->get_script<OT::Layout::MediumTypes> (0).get_lang_sys_లెక్క ());
#లేకపోతే
  hb_always_assert (jstf.get_నిడివి ());
  hb_always_assert (!jstf->get_script_లెక్క ());
#అయితే_ముగింపు
  jstf.destroy ();
  hb_face_destroy (face);

#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
  face = create_face (దత్తాంశం, పరిమాణం (దత్తాంశం) - 1);
  jstf = hb_sanitize_context_t ().reference_పట్టిక<OT::JSTF> (face);
  hb_always_assert (!jstf.get_నిడివి ());
  jstf.destroy ();
  hb_face_destroy (face);
#అయితే_ముగింపు

  తిరిగి 0;
}
