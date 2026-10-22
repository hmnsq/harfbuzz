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

#جيڪڏهن_وصف_نه_ٿيل HB_WASM_API_LIST_HH
#وصف_ڪريو HB_WASM_API_LIST_HH

#ضمن "hb-wasm-api.hh"


#جيڪڏهن_وصف_ٿيل HB_DEBUG_WASM
نطاق hb { نطاق wasm {

ساكن فراغ debugprint (HB_WASM_EXEC_ENV حرف *str)
{ DEBUG_نياپو (WASM, exec_env, "%s", str); }
ساكن فراغ debugprint1 (HB_WASM_EXEC_ENV حرف *str, int32_t i1)
{ DEBUG_نياپو (WASM, exec_env, "%s: %d", str, i1); }
ساكن فراغ debugprint2 (HB_WASM_EXEC_ENV حرف *str, int32_t i1, int32_t i2)
{ DEBUG_نياپو (WASM, exec_env, "%s: %d, %d", str, i1, i2); }
ساكن فراغ debugprint3 (HB_WASM_EXEC_ENV حرف *str, int32_t i1, int32_t i2, int32_t i3)
{ DEBUG_نياپو (WASM, exec_env, "%s: %d, %d, %d", str, i1, i2, i3); }
ساكن فراغ debugprint4 (HB_WASM_EXEC_ENV حرف *str, int32_t i1, int32_t i2, int32_t i3, int32_t i4)
{ DEBUG_نياپو (WASM, exec_env, "%s: %d, %d, %d, %d", str, i1, i2, i3, i4); }

}}
#جيڪڏهن_پورو

#وصف_ڪريو NATIVE_نشان(signature, name) {#name, (فراغ *) hb::wasm::name, signature, عدم}
/* Note: the array must be static defined since runtime will keep it after registration.
 * Also not const, because it modifies it (sorts it).
 * https://github.com/bytecodealliance/wasm-micro-runtime/blob/main/doc/export_native_api.md
 *
 * TODO Allocate this lazily in _hb_wasm_init(). */
ساكن Native_نشان _hb_wasm_native_symbols[] =
{
  /* common */
  NATIVE_نشان ("(i)i",	script_get_horizontal_direction),

  /* blob */
  NATIVE_نشان ("(i)",		blob_free),

  /*بفر*/
  NATIVE_نشان ("(i)",		بفر_contents_free),
  NATIVE_نشان ("(ii)i",	بفر_contents_realloc),
  NATIVE_نشان ("(ii)i",	بفر_copy_contents),
  NATIVE_نشان ("(ii)i",	بفر_سيٽ_contents),
  NATIVE_نشان ("(i)i",	بفر_get_direction),
  NATIVE_نشان ("(i)i",	بفر_get_script),
  NATIVE_نشان ("(i)",		بفر_ابتو),
  NATIVE_نشان ("(i)",		بفر_ابتو_clusters),

  /* face */
  NATIVE_نشان ("(ii)i",	face_create),
  NATIVE_نشان ("(iii)i",	face_copy_جدول),
  NATIVE_نشان ("(i)i",	face_get_upem),

  /* font */
  NATIVE_نشان ("(i)i",	font_create),
  NATIVE_نشان ("(i)i",	font_get_face),
  NATIVE_نشان ("(iii)",	font_get_scale),
  NATIVE_نشان ("(iii)i",	font_get_glyph),
  NATIVE_نشان ("(ii)i",	font_get_glyph_h_advance),
  NATIVE_نشان ("(ii)i",	font_get_glyph_v_advance),
  NATIVE_نشان ("(iii)i",	font_get_glyph_extents),
  NATIVE_نشان ("(iiii)",	font_glyph_منزل_string),
  NATIVE_نشان ("(iii)i",	font_copy_glyph_outline),
  NATIVE_نشان ("(ii)i",	font_copy_coords),
  NATIVE_نشان ("(ii)i",	font_سيٽ_coords),

  /* outline */
  NATIVE_نشان ("(i)",		glyph_outline_free),

  /* shape */
  NATIVE_نشان ("(iiii$)i",	shape_with),
  NATIVE_نشان ("(iii)i",	feature_copy_contents),
  NATIVE_نشان ("(i)",		features_free),

  /* debug */
#جيڪڏهن_وصف_ٿيل HB_DEBUG_WASM
  NATIVE_نشان ("($)",		debugprint),
  NATIVE_نشان ("($i)",	debugprint1),
  NATIVE_نشان ("($ii)",	debugprint2),
  NATIVE_نشان ("($iii)",	debugprint3),
  NATIVE_نشان ("($iiii)",	debugprint4),
#جيڪڏهن_پورو

};
#وصف_رد NATIVE_نشان


#جيڪڏهن_پورو /* HB_WASM_API_LIST_HH */
