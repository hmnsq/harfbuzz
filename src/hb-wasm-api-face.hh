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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_WASM_API_FACE_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_WASM_API_FACE_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-wasm-api.hh"

ਨਾਮਾਕਾਸ਼ hb {
ਨਾਮਾਕਾਸ਼ wasm {


HB_WASM_API (ptr_t(face_t), face_create) (HB_WASM_EXEC_ENV
					  ptr_d(blob_t, blob),
					  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸੂਚਕਾਂਕ_2)
{
  HB_PTR_PARAM (blob_t, blob);
  ਜੇ (unlikely (!blob))
    ਵਾਪਸ nullref;

  ਅੱਖਰ *ਅੰਕੜੇ = HB_ARRAY_APP2NATIVE (ਅੱਖਰ, blob->ਅੰਕੜੇ, blob->ਲੰਮਾਈ);
  ਜੇ (unlikely (!ਅੰਕੜੇ))
    ਵਾਪਸ nullref;

  hb_blob_t *hb_blob = hb_blob_create(
				      ਅੰਕੜੇ,
				      blob->ਲੰਮਾਈ,
				      HB_ਸਮ੍ਰਿਤੀ_ਬਹੁਲਕ_DUPLICATE,
				      ਨਿਰਾ,
				      ਨਿਰਾ);

  hb_face_t *face = hb_face_create(hb_blob, ਸੂਚਕਾਂਕ_2);
  hb_blob_destroy (hb_blob);

  HB_OBJ2REF (face);
  ਵਾਪਸ faceref;
}

HB_WASM_API (bool_t, face_copy_ਸਾਰਣੀ) (HB_WASM_EXEC_ENV
				       ptr_d(face_t, face),
				       ਟੈਗ_t ਸਾਰਣੀ_ਟੈਗ_2,
				       ptr_d(blob_t, blob))
{
  HB_REF2OBJ (face);
  HB_PTR_PARAM (blob_t, blob);
  ਜੇ (unlikely (!blob))
    ਵਾਪਸ ਝੂਠ;

  hb_blob_t *hb_blob = hb_face_reference_ਸਾਰਣੀ (face, ਸਾਰਣੀ_ਟੈਗ_2);

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾਈ;
  ਅਚਲ ਅੱਖਰ *hb_ਅੰਕੜੇ = hb_blob_get_ਅੰਕੜੇ (hb_blob, &ਲੰਮਾਈ);

  ਜੇ (ਲੰਮਾਈ <= blob->ਲੰਮਾਈ)
  {
    ਅੱਖਰ *ਅੰਕੜੇ = HB_ARRAY_APP2NATIVE (ਅੱਖਰ, blob->ਅੰਕੜੇ, ਲੰਮਾਈ);

    ਜੇ (unlikely (!ਅੰਕੜੇ))
    {
      blob->ਲੰਮਾਈ = 0;
      ਵਾਪਸ ਝੂਠ;
    }

    hb_memcpy (ਅੰਕੜੇ, hb_ਅੰਕੜੇ, ਲੰਮਾਈ);

    ਵਾਪਸ ਸੱਚ;
  }

  module_free (blob->ਅੰਕੜੇ);

  blob->ਲੰਮਾਈ = ਲੰਮਾਈ;
  blob->ਅੰਕੜੇ = wasm_runtime_module_dup_ਅੰਕੜੇ (module_inst, hb_ਅੰਕੜੇ, ਲੰਮਾਈ);

  hb_blob_destroy (hb_blob);

  ਜੇ (blob->ਲੰਮਾਈ && !blob->ਅੰਕੜੇ)
  {
    blob->ਲੰਮਾਈ = 0;
    ਵਾਪਸ ਝੂਠ;
  }

  ਵਾਪਸ ਸੱਚ;
}

HB_WASM_API (ਬਿਨਾਨਿਸ਼ਾਨੀ, face_get_upem) (HB_WASM_EXEC_ENV
				       ptr_d(face_t, face))
{
  HB_REF2OBJ (face);

  ਵਾਪਸ hb_face_get_upem (face);
}


}}

#ਜੇ_ਅੰਤ /* HB_WASM_API_FACE_HH */
