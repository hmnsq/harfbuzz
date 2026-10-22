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

#ifndef HB_WASM_API_LIST_HH
#define HB_WASM_API_LIST_HH

#ಅನ್ತರ್ಭಾವ "hb-wasm-api.hh"


#ifdef HB_DEBUG_WASM
ನಾಮಾಕಾಶ hb { ನಾಮಾಕಾಶ wasm {

ಸ್ಥಿರ ಶೂನ್ಯ debugprint (HB_WASM_EXEC_ENV ವರ್ಣ *str)
{ DEBUG_ಸಂದೇಶ (WASM, exec_env, "%s", str); }
ಸ್ಥಿರ ಶೂನ್ಯ debugprint1 (HB_WASM_EXEC_ENV ವರ್ಣ *str, int32_t i1)
{ DEBUG_ಸಂದೇಶ (WASM, exec_env, "%s: %d", str, i1); }
ಸ್ಥಿರ ಶೂನ್ಯ debugprint2 (HB_WASM_EXEC_ENV ವರ್ಣ *str, int32_t i1, int32_t i2)
{ DEBUG_ಸಂದೇಶ (WASM, exec_env, "%s: %d, %d", str, i1, i2); }
ಸ್ಥಿರ ಶೂನ್ಯ debugprint3 (HB_WASM_EXEC_ENV ವರ್ಣ *str, int32_t i1, int32_t i2, int32_t i3)
{ DEBUG_ಸಂದೇಶ (WASM, exec_env, "%s: %d, %d, %d", str, i1, i2, i3); }
ಸ್ಥಿರ ಶೂನ್ಯ debugprint4 (HB_WASM_EXEC_ENV ವರ್ಣ *str, int32_t i1, int32_t i2, int32_t i3, int32_t i4)
{ DEBUG_ಸಂದೇಶ (WASM, exec_env, "%s: %d, %d, %d, %d", str, i1, i2, i3, i4); }

}}
#endif

#define NATIVE_ಚಿಹ್ನೆ(signature, name) {#name, (ಶೂನ್ಯ *) hb::wasm::name, signature, ರಿಕ್ತ}
/* Note: the array must be static defined since runtime will keep it after registration.
 * Also not const, because it modifies it (sorts it).
 * https://github.com/bytecodealliance/wasm-micro-runtime/blob/main/doc/export_native_api.md
 *
 * TODO Allocate this lazily in _hb_wasm_init(). */
ಸ್ಥಿರ Native_ಚಿಹ್ನೆ _hb_wasm_native_symbols[] =
{
  /* common */
  NATIVE_ಚಿಹ್ನೆ ("(i)i",	script_get_horizontal_direction),

  /* blob */
  NATIVE_ಚಿಹ್ನೆ ("(i)",		blob_free),

  /* buffer */
  NATIVE_ಚಿಹ್ನೆ ("(i)",		buffer_contents_free),
  NATIVE_ಚಿಹ್ನೆ ("(ii)i",	buffer_contents_realloc),
  NATIVE_ಚಿಹ್ನೆ ("(ii)i",	buffer_copy_contents),
  NATIVE_ಚಿಹ್ನೆ ("(ii)i",	buffer_set_contents),
  NATIVE_ಚಿಹ್ನೆ ("(i)i",	buffer_get_direction),
  NATIVE_ಚಿಹ್ನೆ ("(i)i",	buffer_get_script),
  NATIVE_ಚಿಹ್ನೆ ("(i)",		buffer_reverse),
  NATIVE_ಚಿಹ್ನೆ ("(i)",		buffer_reverse_clusters),

  /* face */
  NATIVE_ಚಿಹ್ನೆ ("(ii)i",	face_create),
  NATIVE_ಚಿಹ್ನೆ ("(iii)i",	face_copy_table),
  NATIVE_ಚಿಹ್ನೆ ("(i)i",	face_get_upem),

  /* font */
  NATIVE_ಚಿಹ್ನೆ ("(i)i",	font_create),
  NATIVE_ಚಿಹ್ನೆ ("(i)i",	font_get_face),
  NATIVE_ಚಿಹ್ನೆ ("(iii)",	font_get_scale),
  NATIVE_ಚಿಹ್ನೆ ("(iii)i",	font_get_glyph),
  NATIVE_ಚಿಹ್ನೆ ("(ii)i",	font_get_glyph_h_advance),
  NATIVE_ಚಿಹ್ನೆ ("(ii)i",	font_get_glyph_v_advance),
  NATIVE_ಚಿಹ್ನೆ ("(iii)i",	font_get_glyph_extents),
  NATIVE_ಚಿಹ್ನೆ ("(iiii)",	font_glyph_to_string),
  NATIVE_ಚಿಹ್ನೆ ("(iii)i",	font_copy_glyph_outline),
  NATIVE_ಚಿಹ್ನೆ ("(ii)i",	font_copy_coords),
  NATIVE_ಚಿಹ್ನೆ ("(ii)i",	font_set_coords),

  /* outline */
  NATIVE_ಚಿಹ್ನೆ ("(i)",		glyph_outline_free),

  /* shape */
  NATIVE_ಚಿಹ್ನೆ ("(iiii$)i",	shape_with),
  NATIVE_ಚಿಹ್ನೆ ("(iii)i",	feature_copy_contents),
  NATIVE_ಚಿಹ್ನೆ ("(i)",		features_free),

  /* debug */
#ifdef HB_DEBUG_WASM
  NATIVE_ಚಿಹ್ನೆ ("($)",		debugprint),
  NATIVE_ಚಿಹ್ನೆ ("($i)",	debugprint1),
  NATIVE_ಚಿಹ್ನೆ ("($ii)",	debugprint2),
  NATIVE_ಚಿಹ್ನೆ ("($iii)",	debugprint3),
  NATIVE_ಚಿಹ್ನೆ ("($iiii)",	debugprint4),
#endif

};
#undef NATIVE_ಚಿಹ್ನೆ


#endif /* HB_WASM_API_LIST_HH */
