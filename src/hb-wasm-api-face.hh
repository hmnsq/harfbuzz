/*
 * Copyright © 2023  Behdad Esfahbod
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

#నిర్వచితం_కాకపోతే HB_WASM_API_FACE_HH
#నిర్వచించు HB_WASM_API_FACE_HH

#అన్తర్భావ "hb-wasm-api.hh"

నామాకాశ hb {
నామాకాశ wasm {


HB_WASM_API (ptr_t(face_t), face_create) (HB_WASM_EXEC_ENV
					  ptr_d(blob_t, blob),
					  గుర్తులేని పూర్ణాంకం సూచీ_2)
{
  HB_PTR_PARAM (blob_t, blob);
  ఒకవేళ (unlikely (!blob))
    తిరిగి nullref;

  అక్షరం *దత్తాంశం = HB_ARRAY_APP2NATIVE (అక్షరం, blob->దత్తాంశం, blob->నిడివి);
  ఒకవేళ (unlikely (!దత్తాంశం))
    తిరిగి nullref;

  hb_blob_t *hb_blob = hb_blob_create(
				      దత్తాంశం,
				      blob->నిడివి,
				      HB_జ్ఞాపకం_బహుళకం_DUPLICATE,
				      ఖాళీ,
				      ఖాళీ);

  hb_face_t *face = hb_face_create(hb_blob, సూచీ_2);
  hb_blob_destroy (hb_blob);

  HB_OBJ2REF (face);
  తిరిగి faceref;
}

HB_WASM_API (bool_t, face_copy_పట్టిక) (HB_WASM_EXEC_ENV
				       ptr_d(face_t, face),
				       ట్యాగ్_t పట్టిక_ట్యాగ్_2,
				       ptr_d(blob_t, blob))
{
  HB_REF2OBJ (face);
  HB_PTR_PARAM (blob_t, blob);
  ఒకవేళ (unlikely (!blob))
    తిరిగి అబద్ధం;

  hb_blob_t *hb_blob = hb_face_reference_పట్టిక (face, పట్టిక_ట్యాగ్_2);

  గుర్తులేని నిడివి;
  స్థిరరాశి అక్షరం *hb_దత్తాంశం = hb_blob_get_దత్తాంశం (hb_blob, &నిడివి);

  ఒకవేళ (నిడివి <= blob->నిడివి)
  {
    అక్షరం *దత్తాంశం = HB_ARRAY_APP2NATIVE (అక్షరం, blob->దత్తాంశం, నిడివి);

    ఒకవేళ (unlikely (!దత్తాంశం))
    {
      blob->నిడివి = 0;
      తిరిగి అబద్ధం;
    }

    hb_memcpy (దత్తాంశం, hb_దత్తాంశం, నిడివి);

    తిరిగి నిజం;
  }

  module_free (blob->దత్తాంశం);

  blob->నిడివి = నిడివి;
  blob->దత్తాంశం = wasm_runtime_module_dup_దత్తాంశం (module_inst, hb_దత్తాంశం, నిడివి);

  hb_blob_destroy (hb_blob);

  ఒకవేళ (blob->నిడివి && !blob->దత్తాంశం)
  {
    blob->నిడివి = 0;
    తిరిగి అబద్ధం;
  }

  తిరిగి నిజం;
}

HB_WASM_API (గుర్తులేని, face_get_upem) (HB_WASM_EXEC_ENV
				       ptr_d(face_t, face))
{
  HB_REF2OBJ (face);

  తిరిగి hb_face_get_upem (face);
}


}}

#అయితే_ముగింపు /* HB_WASM_API_FACE_HH */
