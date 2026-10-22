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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_WASM_API_LIST_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_WASM_API_LIST_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-wasm-api.hh"


#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HB_DEBUG_WASM
ਨਾਮਾਕਾਸ਼ hb { ਨਾਮਾਕਾਸ਼ wasm {

ਸਥਿਰ ਖਾਲੀ debugprint (HB_WASM_EXEC_ENV ਅੱਖਰ *str)
{ DEBUG_ਸੁਨੇਹਾ (WASM, exec_env, "%s", str); }
ਸਥਿਰ ਖਾਲੀ debugprint1 (HB_WASM_EXEC_ENV ਅੱਖਰ *str, int32_t i1)
{ DEBUG_ਸੁਨੇਹਾ (WASM, exec_env, "%s: %d", str, i1); }
ਸਥਿਰ ਖਾਲੀ debugprint2 (HB_WASM_EXEC_ENV ਅੱਖਰ *str, int32_t i1, int32_t i2)
{ DEBUG_ਸੁਨੇਹਾ (WASM, exec_env, "%s: %d, %d", str, i1, i2); }
ਸਥਿਰ ਖਾਲੀ debugprint3 (HB_WASM_EXEC_ENV ਅੱਖਰ *str, int32_t i1, int32_t i2, int32_t i3)
{ DEBUG_ਸੁਨੇਹਾ (WASM, exec_env, "%s: %d, %d, %d", str, i1, i2, i3); }
ਸਥਿਰ ਖਾਲੀ debugprint4 (HB_WASM_EXEC_ENV ਅੱਖਰ *str, int32_t i1, int32_t i2, int32_t i3, int32_t i4)
{ DEBUG_ਸੁਨੇਹਾ (WASM, exec_env, "%s: %d, %d, %d, %d", str, i1, i2, i3, i4); }

}}
#ਜੇ_ਅੰਤ

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NATIVE_ਚਿੰਨ੍ਹ(signature, name) {#name, (ਖਾਲੀ *) hb::wasm::name, signature, ਨਿਰਾ}
/* Note: the array must be static defined since runtime will keep it after registration.
 * Also not const, because it modifies it (sorts it).
 * https://github.com/bytecodealliance/wasm-micro-runtime/blob/main/doc/export_native_api.md
 *
 * TODO Allocate this lazily in _hb_wasm_init(). */
ਸਥਿਰ Native_ਚਿੰਨ੍ਹ _hb_wasm_native_symbols[] =
{
  /* common */
  NATIVE_ਚਿੰਨ੍ਹ ("(i)i",	script_get_horizontal_direction),

  /* blob */
  NATIVE_ਚਿੰਨ੍ਹ ("(i)",		blob_free),

  /*ਬਫਰ*/
  NATIVE_ਚਿੰਨ੍ਹ ("(i)",		ਬਫਰ_contents_free),
  NATIVE_ਚਿੰਨ੍ਹ ("(ii)i",	ਬਫਰ_contents_realloc),
  NATIVE_ਚਿੰਨ੍ਹ ("(ii)i",	ਬਫਰ_copy_contents),
  NATIVE_ਚਿੰਨ੍ਹ ("(ii)i",	ਬਫਰ_ਸੈੱਟ_contents),
  NATIVE_ਚਿੰਨ੍ਹ ("(i)i",	ਬਫਰ_get_direction),
  NATIVE_ਚਿੰਨ੍ਹ ("(i)i",	ਬਫਰ_get_script),
  NATIVE_ਚਿੰਨ੍ਹ ("(i)",		ਬਫਰ_ਉਲਟ),
  NATIVE_ਚਿੰਨ੍ਹ ("(i)",		ਬਫਰ_ਉਲਟ_clusters),

  /* face */
  NATIVE_ਚਿੰਨ੍ਹ ("(ii)i",	face_create),
  NATIVE_ਚਿੰਨ੍ਹ ("(iii)i",	face_copy_ਸਾਰਣੀ),
  NATIVE_ਚਿੰਨ੍ਹ ("(i)i",	face_get_upem),

  /* font */
  NATIVE_ਚਿੰਨ੍ਹ ("(i)i",	font_create),
  NATIVE_ਚਿੰਨ੍ਹ ("(i)i",	font_get_face),
  NATIVE_ਚਿੰਨ੍ਹ ("(iii)",	font_get_scale),
  NATIVE_ਚਿੰਨ੍ਹ ("(iii)i",	font_get_glyph),
  NATIVE_ਚਿੰਨ੍ਹ ("(ii)i",	font_get_glyph_h_advance),
  NATIVE_ਚਿੰਨ੍ਹ ("(ii)i",	font_get_glyph_v_advance),
  NATIVE_ਚਿੰਨ੍ਹ ("(iii)i",	font_get_glyph_extents),
  NATIVE_ਚਿੰਨ੍ਹ ("(iiii)",	font_glyph_ਮੰਜ਼ਿਲ_string),
  NATIVE_ਚਿੰਨ੍ਹ ("(iii)i",	font_copy_glyph_outline),
  NATIVE_ਚਿੰਨ੍ਹ ("(ii)i",	font_copy_coords),
  NATIVE_ਚਿੰਨ੍ਹ ("(ii)i",	font_ਸੈੱਟ_coords),

  /* outline */
  NATIVE_ਚਿੰਨ੍ਹ ("(i)",		glyph_outline_free),

  /* shape */
  NATIVE_ਚਿੰਨ੍ਹ ("(iiii$)i",	shape_with),
  NATIVE_ਚਿੰਨ੍ਹ ("(iii)i",	feature_copy_contents),
  NATIVE_ਚਿੰਨ੍ਹ ("(i)",		features_free),

  /* debug */
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HB_DEBUG_WASM
  NATIVE_ਚਿੰਨ੍ਹ ("($)",		debugprint),
  NATIVE_ਚਿੰਨ੍ਹ ("($i)",	debugprint1),
  NATIVE_ਚਿੰਨ੍ਹ ("($ii)",	debugprint2),
  NATIVE_ਚਿੰਨ੍ਹ ("($iii)",	debugprint3),
  NATIVE_ਚਿੰਨ੍ਹ ("($iiii)",	debugprint4),
#ਜੇ_ਅੰਤ

};
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ NATIVE_ਚਿੰਨ੍ਹ


#ਜੇ_ਅੰਤ /* HB_WASM_API_LIST_HH */
