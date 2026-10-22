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

#که_نه_تعریف_شوی HB_WASM_API_FACE_HH
#تعریف_کړه HB_WASM_API_FACE_HH

#ضمن "hb-wasm-api.hh"

نطاق hb {
نطاق wasm {


HB_WASM_API (ptr_t(face_t), face_create) (HB_WASM_EXEC_ENV
					  ptr_d(blob_t, blob),
					  بینښه صحيح شاخص_2)
{
  HB_PTR_PARAM (blob_t, blob);
  إذا (unlikely (!blob))
    أرجع nullref;

  حرف *معلومات = HB_ARRAY_APP2NATIVE (حرف, blob->معلومات, blob->اوږدتیا);
  إذا (unlikely (!معلومات))
    أرجع nullref;

  hb_blob_t *hb_blob = hb_blob_create(
				      معلومات,
				      blob->اوږدتیا,
				      HB_حافظه_ډېرپېښ_DUPLICATE,
				      عدم,
				      عدم);

  hb_face_t *face = hb_face_create(hb_blob, شاخص_2);
  hb_blob_destroy (hb_blob);

  HB_OBJ2REF (face);
  أرجع faceref;
}

HB_WASM_API (bool_t, face_copy_جدول) (HB_WASM_EXEC_ENV
				       ptr_d(face_t, face),
				       نښان_t جدول_نښان_2,
				       ptr_d(blob_t, blob))
{
  HB_REF2OBJ (face);
  HB_PTR_PARAM (blob_t, blob);
  إذا (unlikely (!blob))
    أرجع خطأ;

  hb_blob_t *hb_blob = hb_face_reference_جدول (face, جدول_نښان_2);

  بینښه اوږدتیا;
  ثابت حرف *hb_معلومات = hb_blob_get_معلومات (hb_blob, &اوږدتیا);

  إذا (اوږدتیا <= blob->اوږدتیا)
  {
    حرف *معلومات = HB_ARRAY_APP2NATIVE (حرف, blob->معلومات, اوږدتیا);

    إذا (unlikely (!معلومات))
    {
      blob->اوږدتیا = 0;
      أرجع خطأ;
    }

    hb_memcpy (معلومات, hb_معلومات, اوږدتیا);

    أرجع ریښتیا;
  }

  module_free (blob->معلومات);

  blob->اوږدتیا = اوږدتیا;
  blob->معلومات = wasm_runtime_module_dup_معلومات (module_inst, hb_معلومات, اوږدتیا);

  hb_blob_destroy (hb_blob);

  إذا (blob->اوږدتیا && !blob->معلومات)
  {
    blob->اوږدتیا = 0;
    أرجع خطأ;
  }

  أرجع ریښتیا;
}

HB_WASM_API (بینښه, face_get_upem) (HB_WASM_EXEC_ENV
				       ptr_d(face_t, face))
{
  HB_REF2OBJ (face);

  أرجع hb_face_get_upem (face);
}


}}

#د_که_پای /* HB_WASM_API_FACE_HH */
