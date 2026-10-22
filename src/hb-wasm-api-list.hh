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

#اگر_تعریف_نشده HB_WASM_API_LIST_HH
#تعریف_کن HB_WASM_API_LIST_HH

#ضمن "hb-wasm-api.hh"


#اگر_تعریف_شده HB_DEBUG_WASM
نطاق hb { نطاق wasm {

ساكن فراغ debugprint (HB_WASM_EXEC_ENV حرف *str)
{ DEBUG_پیام (WASM, exec_env, "%s", str); }
ساكن فراغ debugprint1 (HB_WASM_EXEC_ENV حرف *str, int32_t i1)
{ DEBUG_پیام (WASM, exec_env, "%s: %d", str, i1); }
ساكن فراغ debugprint2 (HB_WASM_EXEC_ENV حرف *str, int32_t i1, int32_t i2)
{ DEBUG_پیام (WASM, exec_env, "%s: %d, %d", str, i1, i2); }
ساكن فراغ debugprint3 (HB_WASM_EXEC_ENV حرف *str, int32_t i1, int32_t i2, int32_t i3)
{ DEBUG_پیام (WASM, exec_env, "%s: %d, %d, %d", str, i1, i2, i3); }
ساكن فراغ debugprint4 (HB_WASM_EXEC_ENV حرف *str, int32_t i1, int32_t i2, int32_t i3, int32_t i4)
{ DEBUG_پیام (WASM, exec_env, "%s: %d, %d, %d, %d", str, i1, i2, i3, i4); }

}}
#پایان_اگر

#تعریف_کن NATIVE_نماد(signature, name) {#name, (فراغ *) hb::wasm::name, signature, عدم}
/* Note: the array must be static defined since runtime will keep it after registration.
 * Also not const, because it modifies it (sorts it).
 * https://github.com/bytecodealliance/wasm-micro-runtime/blob/main/doc/export_native_api.md
 *
 * TODO Allocate this lazily in _hb_wasm_init(). */
ساكن Native_نماد _hb_wasm_native_symbols[] =
{
  /* common */
  NATIVE_نماد ("(i)i",	script_get_horizontal_direction),

  /* blob */
  NATIVE_نماد ("(i)",		blob_free),

  /*میانگیر*/
  NATIVE_نماد ("(i)",		میانگیر_contents_free),
  NATIVE_نماد ("(ii)i",	میانگیر_contents_realloc),
  NATIVE_نماد ("(ii)i",	میانگیر_copy_contents),
  NATIVE_نماد ("(ii)i",	میانگیر_مجموعه_contents),
  NATIVE_نماد ("(i)i",	میانگیر_get_direction),
  NATIVE_نماد ("(i)i",	میانگیر_get_script),
  NATIVE_نماد ("(i)",		میانگیر_معکوس),
  NATIVE_نماد ("(i)",		میانگیر_معکوس_clusters),

  /* face */
  NATIVE_نماد ("(ii)i",	face_create),
  NATIVE_نماد ("(iii)i",	face_copy_جدول),
  NATIVE_نماد ("(i)i",	face_get_upem),

  /* font */
  NATIVE_نماد ("(i)i",	font_create),
  NATIVE_نماد ("(i)i",	font_get_face),
  NATIVE_نماد ("(iii)",	font_get_scale),
  NATIVE_نماد ("(iii)i",	font_get_glyph),
  NATIVE_نماد ("(ii)i",	font_get_glyph_h_advance),
  NATIVE_نماد ("(ii)i",	font_get_glyph_v_advance),
  NATIVE_نماد ("(iii)i",	font_get_glyph_extents),
  NATIVE_نماد ("(iiii)",	font_glyph_مقصد_string),
  NATIVE_نماد ("(iii)i",	font_copy_glyph_outline),
  NATIVE_نماد ("(ii)i",	font_copy_coords),
  NATIVE_نماد ("(ii)i",	font_مجموعه_coords),

  /* outline */
  NATIVE_نماد ("(i)",		glyph_outline_free),

  /* shape */
  NATIVE_نماد ("(iiii$)i",	shape_with),
  NATIVE_نماد ("(iii)i",	feature_copy_contents),
  NATIVE_نماد ("(i)",		features_free),

  /* debug */
#اگر_تعریف_شده HB_DEBUG_WASM
  NATIVE_نماد ("($)",		debugprint),
  NATIVE_نماد ("($i)",	debugprint1),
  NATIVE_نماد ("($ii)",	debugprint2),
  NATIVE_نماد ("($iii)",	debugprint3),
  NATIVE_نماد ("($iiii)",	debugprint4),
#پایان_اگر

};
#لغو_تعریف NATIVE_نماد


#پایان_اگر /* HB_WASM_API_LIST_HH */
