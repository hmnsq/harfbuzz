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

#अगर_अघोषित HB_WASM_API_FACE_HH
#घोषणा HB_WASM_API_FACE_HH

#समावेश "hb-wasm-api.hh"

namespace hb {
namespace wasm {


HB_WASM_API (ptr_t(face_t), face_create) (HB_WASM_EXEC_ENV
					  ptr_d(blob_t, blob),
					  अचिन्हित पूर्णांक सूचकांक_2)
{
  HB_PTR_PARAM (blob_t, blob);
  अगर (unlikely (!blob))
    वापस nullref;

  अक्षर *तथ्य = HB_ARRAY_APP2NATIVE (अक्षर, blob->तथ्य, blob->लंबाई);
  अगर (unlikely (!तथ्य))
    वापस nullref;

  hb_blob_t *hb_blob = hb_blob_create(
				      तथ्य,
				      blob->लंबाई,
				      HB_MEMORY_MODE_DUPLICATE,
				      शून्य,
				      शून्य);

  hb_face_t *face = hb_face_create(hb_blob, सूचकांक_2);
  hb_blob_destroy (hb_blob);

  HB_OBJ2REF (face);
  वापस faceref;
}

HB_WASM_API (bool_t, face_copy_table) (HB_WASM_EXEC_ENV
				       ptr_d(face_t, face),
				       tag_t table_tag,
				       ptr_d(blob_t, blob))
{
  HB_REF2OBJ (face);
  HB_PTR_PARAM (blob_t, blob);
  अगर (unlikely (!blob))
    वापस false;

  hb_blob_t *hb_blob = hb_face_reference_table (face, table_tag);

  अचिन्हित लंबाई;
  स्थिर अक्षर *hb_तथ्य = hb_blob_get_तथ्य (hb_blob, &लंबाई);

  अगर (लंबाई <= blob->लंबाई)
  {
    अक्षर *तथ्य = HB_ARRAY_APP2NATIVE (अक्षर, blob->तथ्य, लंबाई);

    अगर (unlikely (!तथ्य))
    {
      blob->लंबाई = 0;
      वापस false;
    }

    hb_memcpy (तथ्य, hb_तथ्य, लंबाई);

    वापस true;
  }

  module_free (blob->तथ्य);

  blob->लंबाई = लंबाई;
  blob->तथ्य = wasm_runtime_module_dup_तथ्य (module_inst, hb_तथ्य, लंबाई);

  hb_blob_destroy (hb_blob);

  अगर (blob->लंबाई && !blob->तथ्य)
  {
    blob->लंबाई = 0;
    वापस false;
  }

  वापस true;
}

HB_WASM_API (अचिन्हित, face_get_upem) (HB_WASM_EXEC_ENV
				       ptr_d(face_t, face))
{
  HB_REF2OBJ (face);

  वापस hb_face_get_upem (face);
}


}}

#पूर्ण_अगर /* HB_WASM_API_FACE_HH */
