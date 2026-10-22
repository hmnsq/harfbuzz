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
					  unsigned पूर्णाङ्क index)
{
  HB_PTR_PARAM (blob_t, blob);
  सचे (unlikely (!blob))
    पटिनिवत्त nullref;

  वर्ण *data = HB_ARRAY_APP2NATIVE (वर्ण, blob->data, blob->length);
  सचे (unlikely (!data))
    पटिनिवत्त nullref;

  hb_blob_t *hb_blob = hb_blob_create(
				      data,
				      blob->length,
				      HB_MEMORY_MODE_DUPLICATE,
				      रिक्त,
				      रिक्त);

  hb_face_t *face = hb_face_create(hb_blob, index);
  hb_blob_destroy (hb_blob);

  HB_OBJ2REF (face);
  पटिनिवत्त faceref;
}

HB_WASM_API (bool_t, face_copy_table) (HB_WASM_EXEC_ENV
				       ptr_d(face_t, face),
				       tag_t table_tag,
				       ptr_d(blob_t, blob))
{
  HB_REF2OBJ (face);
  HB_PTR_PARAM (blob_t, blob);
  सचे (unlikely (!blob))
    पटिनिवत्त मिच्छा;

  hb_blob_t *hb_blob = hb_face_reference_table (face, table_tag);

  unsigned length;
  ध्रुव वर्ण *hb_data = hb_blob_get_data (hb_blob, &length);

  सचे (length <= blob->length)
  {
    वर्ण *data = HB_ARRAY_APP2NATIVE (वर्ण, blob->data, length);

    सचे (unlikely (!data))
    {
      blob->length = 0;
      पटिनिवत्त मिच्छा;
    }

    hb_memcpy (data, hb_data, length);

    पटिनिवत्त सच्च;
  }

  module_free (blob->data);

  blob->length = length;
  blob->data = wasm_runtime_module_dup_data (module_inst, hb_data, length);

  hb_blob_destroy (hb_blob);

  सचे (blob->length && !blob->data)
  {
    blob->length = 0;
    पटिनिवत्त मिच्छा;
  }

  पटिनिवत्त सच्च;
}

HB_WASM_API (unsigned, face_get_upem) (HB_WASM_EXEC_ENV
				       ptr_d(face_t, face))
{
  HB_REF2OBJ (face);

  पटिनिवत्त hb_face_get_upem (face);
}


}}

#endif /* HB_WASM_API_FACE_HH */
