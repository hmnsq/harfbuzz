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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_WASM_API_BLOB_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_WASM_API_BLOB_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-wasm-api.hh"

ਨਾਮਾਕਾਸ਼ hb {
ਨਾਮਾਕਾਸ਼ wasm {


HB_WASM_API (ਖਾਲੀ, blob_free) (HB_WASM_EXEC_ENV
			       ptr_d(blob_t, blob))
{
  HB_PTR_PARAM (blob_t, blob);
  ਜੇ (unlikely (!blob))
    ਵਾਪਸ;

  module_free (blob->ਅੰਕੜੇ);

  blob->ਅੰਕੜੇ = nullref;
  blob->ਲੰਮਾਈ = 0;
}


}}

#ਜੇ_ਅੰਤ /* HB_WASM_API_BLOB_HH */
