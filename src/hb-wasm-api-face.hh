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

#অন্তর্ভাব "hb-wasm-api.hh"

নামাকাশ hb {
নামাকাশ wasm {


HB_WASM_API (ptr_t(face_t), face_create) (HB_WASM_EXEC_ENV
					  ptr_d(blob_t, blob),
					  unsigned পূর্ণসংখ্যা সূচক_2)
{
  HB_PTR_PARAM (blob_t, blob);
  যদি (unlikely (!blob))
    ফেরত nullref;

  অক্ষর *তথ্য = HB_ARRAY_APP2NATIVE (অক্ষর, blob->তথ্য, blob->দৈর্ঘ্য);
  যদি (unlikely (!তথ্য))
    ফেরত nullref;

  hb_blob_t *hb_blob = hb_blob_create(
				      তথ্য,
				      blob->দৈর্ঘ্য,
				      HB_MEMORY_MODE_DUPLICATE,
				      ফাঁকা,
				      ফাঁকা);

  hb_face_t *face = hb_face_create(hb_blob, সূচক_2);
  hb_blob_destroy (hb_blob);

  HB_OBJ2REF (face);
  ফেরত faceref;
}

HB_WASM_API (bool_t, face_copy_table) (HB_WASM_EXEC_ENV
				       ptr_d(face_t, face),
				       tag_t table_tag,
				       ptr_d(blob_t, blob))
{
  HB_REF2OBJ (face);
  HB_PTR_PARAM (blob_t, blob);
  যদি (unlikely (!blob))
    ফেরত মিথ্যা;

  hb_blob_t *hb_blob = hb_face_reference_table (face, table_tag);

  unsigned দৈর্ঘ্য;
  ধ্রুবক অক্ষর *hb_তথ্য = hb_blob_get_তথ্য (hb_blob, &দৈর্ঘ্য);

  যদি (দৈর্ঘ্য <= blob->দৈর্ঘ্য)
  {
    অক্ষর *তথ্য = HB_ARRAY_APP2NATIVE (অক্ষর, blob->তথ্য, দৈর্ঘ্য);

    যদি (unlikely (!তথ্য))
    {
      blob->দৈর্ঘ্য = 0;
      ফেরত মিথ্যা;
    }

    hb_memcpy (তথ্য, hb_তথ্য, দৈর্ঘ্য);

    ফেরত সত্য;
  }

  module_free (blob->তথ্য);

  blob->দৈর্ঘ্য = দৈর্ঘ্য;
  blob->তথ্য = wasm_runtime_module_dup_তথ্য (module_inst, hb_তথ্য, দৈর্ঘ্য);

  hb_blob_destroy (hb_blob);

  যদি (blob->দৈর্ঘ্য && !blob->তথ্য)
  {
    blob->দৈর্ঘ্য = 0;
    ফেরত মিথ্যা;
  }

  ফেরত সত্য;
}

HB_WASM_API (unsigned, face_get_upem) (HB_WASM_EXEC_ENV
				       ptr_d(face_t, face))
{
  HB_REF2OBJ (face);

  ফেরত hb_face_get_upem (face);
}


}}

#endif /* HB_WASM_API_FACE_HH */
