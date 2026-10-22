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

#অন্তর্ভাৱ "hb-wasm-api.hh"

নামাকাশ hb {
নামাকাশ wasm {


HB_WASM_API (ptr_t(face_t), face_create) (HB_WASM_EXEC_ENV
					  ptr_d(blob_t, blob),
					  unsigned পূর্ণাঙ্ক index)
{
  HB_PTR_PARAM (blob_t, blob);
  যদি (unlikely (!blob))
    ঘূৰাই nullref;

  ৱর্ণ *data = HB_ARRAY_APP2NATIVE (ৱর্ণ, blob->data, blob->length);
  যদি (unlikely (!data))
    ঘূৰাই nullref;

  hb_blob_t *hb_blob = hb_blob_create(
				      data,
				      blob->length,
				      HB_MEMORY_MODE_DUPLICATE,
				      রিক্ত,
				      রিক্ত);

  hb_face_t *face = hb_face_create(hb_blob, index);
  hb_blob_destroy (hb_blob);

  HB_OBJ2REF (face);
  ঘূৰাই faceref;
}

HB_WASM_API (bool_t, face_copy_table) (HB_WASM_EXEC_ENV
				       ptr_d(face_t, face),
				       tag_t table_tag,
				       ptr_d(blob_t, blob))
{
  HB_REF2OBJ (face);
  HB_PTR_PARAM (blob_t, blob);
  যদি (unlikely (!blob))
    ঘূৰাই মিছা;

  hb_blob_t *hb_blob = hb_face_reference_table (face, table_tag);

  unsigned length;
  ধ্রুৱ ৱর্ণ *hb_data = hb_blob_get_data (hb_blob, &length);

  যদি (length <= blob->length)
  {
    ৱর্ণ *data = HB_ARRAY_APP2NATIVE (ৱর্ণ, blob->data, length);

    যদি (unlikely (!data))
    {
      blob->length = 0;
      ঘূৰাই মিছা;
    }

    hb_memcpy (data, hb_data, length);

    ঘূৰাই সঁচা;
  }

  module_free (blob->data);

  blob->length = length;
  blob->data = wasm_runtime_module_dup_data (module_inst, hb_data, length);

  hb_blob_destroy (hb_blob);

  যদি (blob->length && !blob->data)
  {
    blob->length = 0;
    ঘূৰাই মিছা;
  }

  ঘূৰাই সঁচা;
}

HB_WASM_API (unsigned, face_get_upem) (HB_WASM_EXEC_ENV
				       ptr_d(face_t, face))
{
  HB_REF2OBJ (face);

  ঘূৰাই hb_face_get_upem (face);
}


}}

#endif /* HB_WASM_API_FACE_HH */
