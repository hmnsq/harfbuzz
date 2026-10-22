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

#অন্তর্ভাব "hb-wasm-api.hh"


#ifdef HB_DEBUG_WASM
নামাকাশ hb { নামাকাশ wasm {

স্থির শূন্য debugprint (HB_WASM_EXEC_ENV অক্ষর *str)
{ DEBUG_বার্তা (WASM, exec_env, "%s", str); }
স্থির শূন্য debugprint1 (HB_WASM_EXEC_ENV অক্ষর *str, int32_t i1)
{ DEBUG_বার্তা (WASM, exec_env, "%s: %d", str, i1); }
স্থির শূন্য debugprint2 (HB_WASM_EXEC_ENV অক্ষর *str, int32_t i1, int32_t i2)
{ DEBUG_বার্তা (WASM, exec_env, "%s: %d, %d", str, i1, i2); }
স্থির শূন্য debugprint3 (HB_WASM_EXEC_ENV অক্ষর *str, int32_t i1, int32_t i2, int32_t i3)
{ DEBUG_বার্তা (WASM, exec_env, "%s: %d, %d, %d", str, i1, i2, i3); }
স্থির শূন্য debugprint4 (HB_WASM_EXEC_ENV অক্ষর *str, int32_t i1, int32_t i2, int32_t i3, int32_t i4)
{ DEBUG_বার্তা (WASM, exec_env, "%s: %d, %d, %d, %d", str, i1, i2, i3, i4); }

}}
#endif

#define NATIVE_প্রতীক(signature, name) {#name, (শূন্য *) hb::wasm::name, signature, ফাঁকা}
/* Note: the array must be static defined since runtime will keep it after registration.
 * Also not const, because it modifies it (sorts it).
 * https://github.com/bytecodealliance/wasm-micro-runtime/blob/main/doc/export_native_api.md
 *
 * TODO Allocate this lazily in _hb_wasm_init(). */
স্থির Native_প্রতীক _hb_wasm_native_symbols[] =
{
  /* common */
  NATIVE_প্রতীক ("(i)i",	script_get_horizontal_direction),

  /* blob */
  NATIVE_প্রতীক ("(i)",		blob_free),

  /*বাফার*/
  NATIVE_প্রতীক ("(i)",		বাফার_contents_free),
  NATIVE_প্রতীক ("(ii)i",	বাফার_contents_realloc),
  NATIVE_প্রতীক ("(ii)i",	বাফার_copy_contents),
  NATIVE_প্রতীক ("(ii)i",	বাফার_set_contents),
  NATIVE_প্রতীক ("(i)i",	বাফার_get_direction),
  NATIVE_প্রতীক ("(i)i",	বাফার_get_script),
  NATIVE_প্রতীক ("(i)",		বাফার_reverse),
  NATIVE_প্রতীক ("(i)",		বাফার_reverse_clusters),

  /* face */
  NATIVE_প্রতীক ("(ii)i",	face_create),
  NATIVE_প্রতীক ("(iii)i",	face_copy_table),
  NATIVE_প্রতীক ("(i)i",	face_get_upem),

  /* font */
  NATIVE_প্রতীক ("(i)i",	font_create),
  NATIVE_প্রতীক ("(i)i",	font_get_face),
  NATIVE_প্রতীক ("(iii)",	font_get_scale),
  NATIVE_প্রতীক ("(iii)i",	font_get_glyph),
  NATIVE_প্রতীক ("(ii)i",	font_get_glyph_h_advance),
  NATIVE_প্রতীক ("(ii)i",	font_get_glyph_v_advance),
  NATIVE_প্রতীক ("(iii)i",	font_get_glyph_extents),
  NATIVE_প্রতীক ("(iiii)",	font_glyph_to_string),
  NATIVE_প্রতীক ("(iii)i",	font_copy_glyph_outline),
  NATIVE_প্রতীক ("(ii)i",	font_copy_coords),
  NATIVE_প্রতীক ("(ii)i",	font_set_coords),

  /* outline */
  NATIVE_প্রতীক ("(i)",		glyph_outline_free),

  /* shape */
  NATIVE_প্রতীক ("(iiii$)i",	shape_with),
  NATIVE_প্রতীক ("(iii)i",	feature_copy_contents),
  NATIVE_প্রতীক ("(i)",		features_free),

  /* debug */
#ifdef HB_DEBUG_WASM
  NATIVE_প্রতীক ("($)",		debugprint),
  NATIVE_প্রতীক ("($i)",	debugprint1),
  NATIVE_প্রতীক ("($ii)",	debugprint2),
  NATIVE_প্রতীক ("($iii)",	debugprint3),
  NATIVE_প্রতীক ("($iiii)",	debugprint4),
#endif

};
#undef NATIVE_প্রতীক


#endif /* HB_WASM_API_LIST_HH */
