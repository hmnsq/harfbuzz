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

#که_نه_تعریف_شوی HB_WASM_API_LIST_HH
#تعریف_کړه HB_WASM_API_LIST_HH

#ضمن "hb-wasm-api.hh"


#که_تعریف_شوی HB_DEBUG_WASM
نطاق hb { نطاق wasm {

ساكن فراغ debugprint (HB_WASM_EXEC_ENV حرف *str)
{ DEBUG_پیغام (WASM, exec_env, "%s", str); }
ساكن فراغ debugprint1 (HB_WASM_EXEC_ENV حرف *str, int32_t i1)
{ DEBUG_پیغام (WASM, exec_env, "%s: %d", str, i1); }
ساكن فراغ debugprint2 (HB_WASM_EXEC_ENV حرف *str, int32_t i1, int32_t i2)
{ DEBUG_پیغام (WASM, exec_env, "%s: %d, %d", str, i1, i2); }
ساكن فراغ debugprint3 (HB_WASM_EXEC_ENV حرف *str, int32_t i1, int32_t i2, int32_t i3)
{ DEBUG_پیغام (WASM, exec_env, "%s: %d, %d, %d", str, i1, i2, i3); }
ساكن فراغ debugprint4 (HB_WASM_EXEC_ENV حرف *str, int32_t i1, int32_t i2, int32_t i3, int32_t i4)
{ DEBUG_پیغام (WASM, exec_env, "%s: %d, %d, %d, %d", str, i1, i2, i3, i4); }

}}
#د_که_پای

#تعریف_کړه NATIVE_نښه(signature, name) {#name, (فراغ *) hb::wasm::name, signature, عدم}
/* Note: the array must be static defined since runtime will keep it after registration.
 * Also not const, because it modifies it (sorts it).
 * https://github.com/bytecodealliance/wasm-micro-runtime/blob/main/doc/export_native_api.md
 *
 * TODO Allocate this lazily in _hb_wasm_init(). */
ساكن Native_نښه _hb_wasm_native_symbols[] =
{
  /* common */
  NATIVE_نښه ("(i)i",	script_get_horizontal_direction),

  /* blob */
  NATIVE_نښه ("(i)",		blob_free),

  /*بفر*/
  NATIVE_نښه ("(i)",		بفر_contents_free),
  NATIVE_نښه ("(ii)i",	بفر_contents_realloc),
  NATIVE_نښه ("(ii)i",	بفر_copy_contents),
  NATIVE_نښه ("(ii)i",	بفر_سېټ_contents),
  NATIVE_نښه ("(i)i",	بفر_get_direction),
  NATIVE_نښه ("(i)i",	بفر_get_script),
  NATIVE_نښه ("(i)",		بفر_سرچپه),
  NATIVE_نښه ("(i)",		بفر_سرچپه_clusters),

  /* face */
  NATIVE_نښه ("(ii)i",	face_create),
  NATIVE_نښه ("(iii)i",	face_copy_جدول),
  NATIVE_نښه ("(i)i",	face_get_upem),

  /* font */
  NATIVE_نښه ("(i)i",	font_create),
  NATIVE_نښه ("(i)i",	font_get_face),
  NATIVE_نښه ("(iii)",	font_get_scale),
  NATIVE_نښه ("(iii)i",	font_get_glyph),
  NATIVE_نښه ("(ii)i",	font_get_glyph_h_advance),
  NATIVE_نښه ("(ii)i",	font_get_glyph_v_advance),
  NATIVE_نښه ("(iii)i",	font_get_glyph_extents),
  NATIVE_نښه ("(iiii)",	font_glyph_منزل_string),
  NATIVE_نښه ("(iii)i",	font_copy_glyph_outline),
  NATIVE_نښه ("(ii)i",	font_copy_coords),
  NATIVE_نښه ("(ii)i",	font_سېټ_coords),

  /* outline */
  NATIVE_نښه ("(i)",		glyph_outline_free),

  /* shape */
  NATIVE_نښه ("(iiii$)i",	shape_with),
  NATIVE_نښه ("(iii)i",	feature_copy_contents),
  NATIVE_نښه ("(i)",		features_free),

  /* debug */
#که_تعریف_شوی HB_DEBUG_WASM
  NATIVE_نښه ("($)",		debugprint),
  NATIVE_نښه ("($i)",	debugprint1),
  NATIVE_نښه ("($ii)",	debugprint2),
  NATIVE_نښه ("($iii)",	debugprint3),
  NATIVE_نښه ("($iiii)",	debugprint4),
#د_که_پای

};
#تعریف_لغوه NATIVE_نښه


#د_که_پای /* HB_WASM_API_LIST_HH */
