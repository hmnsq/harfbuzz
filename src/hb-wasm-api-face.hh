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

#ifndef HB_WASM_API_FACE_HH
#define HB_WASM_API_FACE_HH

#अन्तर्भाव "hb-wasm-api.hh"

नामाकाश hb {
नामाकाश wasm {


HB_WASM_API (ptr_t(face_t), face_create) (HB_WASM_EXEC_ENV
					  ptr_d(blob_t, blob),
					  अचिन्हित पूर्णांक index)
{
  HB_PTR_PARAM (blob_t, blob);
  जर (unlikely (!blob))
    परत nullref;

  अक्षर *data = HB_ARRAY_APP2NATIVE (अक्षर, blob->data, blob->length);
  जर (unlikely (!data))
    परत nullref;

  hb_blob_t *hb_blob = hb_blob_create(
				      data,
				      blob->length,
				      HB_MEMORY_MODE_DUPLICATE,
				      रिक्त,
				      रिक्त);

  hb_face_t *face = hb_face_create(hb_blob, index);
  hb_blob_destroy (hb_blob);

  HB_OBJ2REF (face);
  परत faceref;
}

HB_WASM_API (bool_t, face_copy_table) (HB_WASM_EXEC_ENV
				       ptr_d(face_t, face),
				       tag_t table_tag,
				       ptr_d(blob_t, blob))
{
  HB_REF2OBJ (face);
  HB_PTR_PARAM (blob_t, blob);
  जर (unlikely (!blob))
    परत असत्य;

  hb_blob_t *hb_blob = hb_face_reference_table (face, table_tag);

  अचिन्हित length;
  स्थिरांक अक्षर *hb_data = hb_blob_get_data (hb_blob, &length);

  जर (length <= blob->length)
  {
    अक्षर *data = HB_ARRAY_APP2NATIVE (अक्षर, blob->data, length);

    जर (unlikely (!data))
    {
      blob->length = 0;
      परत असत्य;
    }

    hb_memcpy (data, hb_data, length);

    परत सत्य;
  }

  module_free (blob->data);

  blob->length = length;
  blob->data = wasm_runtime_module_dup_data (module_inst, hb_data, length);

  hb_blob_destroy (hb_blob);

  जर (blob->length && !blob->data)
  {
    blob->length = 0;
    परत असत्य;
  }

  परत सत्य;
}

HB_WASM_API (अचिन्हित, face_get_upem) (HB_WASM_EXEC_ENV
				       ptr_d(face_t, face))
{
  HB_REF2OBJ (face);

  परत hb_face_get_upem (face);
}


}}

#endif /* HB_WASM_API_FACE_HH */
