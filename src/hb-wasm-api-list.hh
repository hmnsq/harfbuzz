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

#اگر_غیر_تعریف_شدہ HB_WASM_API_LIST_HH
#تعریف_کرو HB_WASM_API_LIST_HH

#شامل "hb-wasm-api.hh"


#اگر_تعریف_شدہ HB_DEBUG_WASM
نامخلا hb { نامخلا wasm {

ساکن خالی debugprint (HB_WASM_EXEC_ENV حرف *str)
{ DEBUG_پیغام (WASM, exec_env, "%s", str); }
ساکن خالی debugprint1 (HB_WASM_EXEC_ENV حرف *str, int32_t i1)
{ DEBUG_پیغام (WASM, exec_env, "%s: %d", str, i1); }
ساکن خالی debugprint2 (HB_WASM_EXEC_ENV حرف *str, int32_t i1, int32_t i2)
{ DEBUG_پیغام (WASM, exec_env, "%s: %d, %d", str, i1, i2); }
ساکن خالی debugprint3 (HB_WASM_EXEC_ENV حرف *str, int32_t i1, int32_t i2, int32_t i3)
{ DEBUG_پیغام (WASM, exec_env, "%s: %d, %d, %d", str, i1, i2, i3); }
ساکن خالی debugprint4 (HB_WASM_EXEC_ENV حرف *str, int32_t i1, int32_t i2, int32_t i3, int32_t i4)
{ DEBUG_پیغام (WASM, exec_env, "%s: %d, %d, %d, %d", str, i1, i2, i3, i4); }

}}
#اگر_ختم

#تعریف_کرو NATIVE_علامت(signature, name) {#name, (خالی *) hb::wasm::name, signature, تہی}
/* Note: the array must be static defined since runtime will keep it after registration.
 * Also not const, because it modifies it (sorts it).
 * https://github.com/bytecodealliance/wasm-micro-runtime/blob/main/doc/export_native_api.md
 *
 * TODO Allocate this lazily in _hb_wasm_init(). */
ساکن Native_علامت _hb_wasm_native_symbols[] =
{
  /* common */
  NATIVE_علامت ("(i)i",	script_get_horizontal_direction),

  /* blob */
  NATIVE_علامت ("(i)",		blob_free),

  /*بفر*/
  NATIVE_علامت ("(i)",		بفر_contents_free),
  NATIVE_علامت ("(ii)i",	بفر_contents_realloc),
  NATIVE_علامت ("(ii)i",	بفر_copy_contents),
  NATIVE_علامت ("(ii)i",	بفر_سیٹ_contents),
  NATIVE_علامت ("(i)i",	بفر_get_direction),
  NATIVE_علامت ("(i)i",	بفر_get_script),
  NATIVE_علامت ("(i)",		بفر_الٹ),
  NATIVE_علامت ("(i)",		بفر_الٹ_clusters),

  /* face */
  NATIVE_علامت ("(ii)i",	face_create),
  NATIVE_علامت ("(iii)i",	face_copy_جدول),
  NATIVE_علامت ("(i)i",	face_get_upem),

  /* font */
  NATIVE_علامت ("(i)i",	font_create),
  NATIVE_علامت ("(i)i",	font_get_face),
  NATIVE_علامت ("(iii)",	font_get_scale),
  NATIVE_علامت ("(iii)i",	font_get_glyph),
  NATIVE_علامت ("(ii)i",	font_get_glyph_h_advance),
  NATIVE_علامت ("(ii)i",	font_get_glyph_v_advance),
  NATIVE_علامت ("(iii)i",	font_get_glyph_extents),
  NATIVE_علامت ("(iiii)",	font_glyph_مقصد_string),
  NATIVE_علامت ("(iii)i",	font_copy_glyph_outline),
  NATIVE_علامت ("(ii)i",	font_copy_coords),
  NATIVE_علامت ("(ii)i",	font_سیٹ_coords),

  /* outline */
  NATIVE_علامت ("(i)",		glyph_outline_free),

  /* shape */
  NATIVE_علامت ("(iiii$)i",	shape_with),
  NATIVE_علامت ("(iii)i",	feature_copy_contents),
  NATIVE_علامت ("(i)",		features_free),

  /* debug */
#اگر_تعریف_شدہ HB_DEBUG_WASM
  NATIVE_علامت ("($)",		debugprint),
  NATIVE_علامت ("($i)",	debugprint1),
  NATIVE_علامت ("($ii)",	debugprint2),
  NATIVE_علامت ("($iii)",	debugprint3),
  NATIVE_علامت ("($iiii)",	debugprint4),
#اگر_ختم

};
#تعریف_منسوخ NATIVE_علامت


#اگر_ختم /* HB_WASM_API_LIST_HH */
