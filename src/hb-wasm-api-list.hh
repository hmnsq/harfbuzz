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

#నిర్వచితం_కాకపోతే HB_WASM_API_LIST_HH
#నిర్వచించు HB_WASM_API_LIST_HH

#అన్తర్భావ "hb-wasm-api.hh"


#నిర్వచితమైతే HB_DEBUG_WASM
నామాకాశ hb { నామాకాశ wasm {

స్థిర శూన్య debugprint (HB_WASM_EXEC_ENV అక్షరం *str)
{ DEBUG_సందేశం (WASM, exec_env, "%s", str); }
స్థిర శూన్య debugprint1 (HB_WASM_EXEC_ENV అక్షరం *str, int32_t i1)
{ DEBUG_సందేశం (WASM, exec_env, "%s: %d", str, i1); }
స్థిర శూన్య debugprint2 (HB_WASM_EXEC_ENV అక్షరం *str, int32_t i1, int32_t i2)
{ DEBUG_సందేశం (WASM, exec_env, "%s: %d, %d", str, i1, i2); }
స్థిర శూన్య debugprint3 (HB_WASM_EXEC_ENV అక్షరం *str, int32_t i1, int32_t i2, int32_t i3)
{ DEBUG_సందేశం (WASM, exec_env, "%s: %d, %d, %d", str, i1, i2, i3); }
స్థిర శూన్య debugprint4 (HB_WASM_EXEC_ENV అక్షరం *str, int32_t i1, int32_t i2, int32_t i3, int32_t i4)
{ DEBUG_సందేశం (WASM, exec_env, "%s: %d, %d, %d, %d", str, i1, i2, i3, i4); }

}}
#అయితే_ముగింపు

#నిర్వచించు NATIVE_చిహ్నం(signature, name) {#name, (శూన్య *) hb::wasm::name, signature, ఖాళీ}
/* Note: the array must be static defined since runtime will keep it after registration.
 * Also not const, because it modifies it (sorts it).
 * https://github.com/bytecodealliance/wasm-micro-runtime/blob/main/doc/export_native_api.md
 *
 * TODO Allocate this lazily in _hb_wasm_init(). */
స్థిర Native_చిహ్నం _hb_wasm_native_symbols[] =
{
  /* common */
  NATIVE_చిహ్నం ("(i)i",	script_get_horizontal_direction),

  /* blob */
  NATIVE_చిహ్నం ("(i)",		blob_free),

  /*బఫరు*/
  NATIVE_చిహ్నం ("(i)",		బఫరు_contents_free),
  NATIVE_చిహ్నం ("(ii)i",	బఫరు_contents_realloc),
  NATIVE_చిహ్నం ("(ii)i",	బఫరు_copy_contents),
  NATIVE_చిహ్నం ("(ii)i",	బఫరు_సమితి_contents),
  NATIVE_చిహ్నం ("(i)i",	బఫరు_get_direction),
  NATIVE_చిహ్నం ("(i)i",	బఫరు_get_script),
  NATIVE_చిహ్నం ("(i)",		బఫరు_తిరగేయి),
  NATIVE_చిహ్నం ("(i)",		బఫరు_తిరగేయి_clusters),

  /* face */
  NATIVE_చిహ్నం ("(ii)i",	face_create),
  NATIVE_చిహ్నం ("(iii)i",	face_copy_పట్టిక),
  NATIVE_చిహ్నం ("(i)i",	face_get_upem),

  /* font */
  NATIVE_చిహ్నం ("(i)i",	font_create),
  NATIVE_చిహ్నం ("(i)i",	font_get_face),
  NATIVE_చిహ్నం ("(iii)",	font_get_scale),
  NATIVE_చిహ్నం ("(iii)i",	font_get_glyph),
  NATIVE_చిహ్నం ("(ii)i",	font_get_glyph_h_advance),
  NATIVE_చిహ్నం ("(ii)i",	font_get_glyph_v_advance),
  NATIVE_చిహ్నం ("(iii)i",	font_get_glyph_extents),
  NATIVE_చిహ్నం ("(iiii)",	font_glyph_గమ్యం_string),
  NATIVE_చిహ్నం ("(iii)i",	font_copy_glyph_outline),
  NATIVE_చిహ్నం ("(ii)i",	font_copy_coords),
  NATIVE_చిహ్నం ("(ii)i",	font_సమితి_coords),

  /* outline */
  NATIVE_చిహ్నం ("(i)",		glyph_outline_free),

  /* shape */
  NATIVE_చిహ్నం ("(iiii$)i",	shape_with),
  NATIVE_చిహ్నం ("(iii)i",	feature_copy_contents),
  NATIVE_చిహ్నం ("(i)",		features_free),

  /* debug */
#నిర్వచితమైతే HB_DEBUG_WASM
  NATIVE_చిహ్నం ("($)",		debugprint),
  NATIVE_చిహ్నం ("($i)",	debugprint1),
  NATIVE_చిహ్నం ("($ii)",	debugprint2),
  NATIVE_చిహ్నం ("($iii)",	debugprint3),
  NATIVE_చిహ్నం ("($iiii)",	debugprint4),
#అయితే_ముగింపు

};
#నిర్వచనం_రద్దు NATIVE_చిహ్నం


#అయితే_ముగింపు /* HB_WASM_API_LIST_HH */
