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

#اگر_غیر_تعریف_شدہ HB_WASM_API_FACE_HH
#تعریف_کرو HB_WASM_API_FACE_HH

#شامل "hb-wasm-api.hh"

نامخلا hb {
نامخلا wasm {


HB_WASM_API (ptr_t(face_t), face_create) (HB_WASM_EXEC_ENV
					  ptr_d(blob_t, blob),
					  غیرعلامتی صحیح اشاریہ_2)
{
  HB_PTR_PARAM (blob_t, blob);
  اگر (unlikely (!blob))
    واپس nullref;

  حرف *ڈیٹا = HB_ARRAY_APP2NATIVE (حرف, blob->ڈیٹا, blob->لمبائی);
  اگر (unlikely (!ڈیٹا))
    واپس nullref;

  hb_blob_t *hb_blob = hb_blob_create(
				      ڈیٹا,
				      blob->لمبائی,
				      HB_حافظہ_منوال_DUPLICATE,
				      تہی,
				      تہی);

  hb_face_t *face = hb_face_create(hb_blob, اشاریہ_2);
  hb_blob_destroy (hb_blob);

  HB_OBJ2REF (face);
  واپس faceref;
}

HB_WASM_API (bool_t, face_copy_جدول) (HB_WASM_EXEC_ENV
				       ptr_d(face_t, face),
				       لیبل_t جدول_لیبل_2,
				       ptr_d(blob_t, blob))
{
  HB_REF2OBJ (face);
  HB_PTR_PARAM (blob_t, blob);
  اگر (unlikely (!blob))
    واپس جھوٹ;

  hb_blob_t *hb_blob = hb_face_reference_جدول (face, جدول_لیبل_2);

  غیرعلامتی لمبائی;
  ثابت حرف *hb_ڈیٹا = hb_blob_get_ڈیٹا (hb_blob, &لمبائی);

  اگر (لمبائی <= blob->لمبائی)
  {
    حرف *ڈیٹا = HB_ARRAY_APP2NATIVE (حرف, blob->ڈیٹا, لمبائی);

    اگر (unlikely (!ڈیٹا))
    {
      blob->لمبائی = 0;
      واپس جھوٹ;
    }

    hb_memcpy (ڈیٹا, hb_ڈیٹا, لمبائی);

    واپس سچ;
  }

  module_free (blob->ڈیٹا);

  blob->لمبائی = لمبائی;
  blob->ڈیٹا = wasm_runtime_module_dup_ڈیٹا (module_inst, hb_ڈیٹا, لمبائی);

  hb_blob_destroy (hb_blob);

  اگر (blob->لمبائی && !blob->ڈیٹا)
  {
    blob->لمبائی = 0;
    واپس جھوٹ;
  }

  واپس سچ;
}

HB_WASM_API (غیرعلامتی, face_get_upem) (HB_WASM_EXEC_ENV
				       ptr_d(face_t, face))
{
  HB_REF2OBJ (face);

  واپس hb_face_get_upem (face);
}


}}

#اگر_ختم /* HB_WASM_API_FACE_HH */
