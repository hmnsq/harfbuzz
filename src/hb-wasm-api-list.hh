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

#include "hb-wasm-api.hh"


#ifdef HB_DEBUG_WASM
namespace hb { namespace wasm {

static void debugprint (HB_WASM_EXEC_ENV char *str)
{ DEBUG_मेनसाहे (WASM, exec_env, "%s", str); }
static void debugprint1 (HB_WASM_EXEC_ENV char *str, int32_t i1)
{ DEBUG_मेनसाहे (WASM, exec_env, "%s: %d", str, i1); }
static void debugprint2 (HB_WASM_EXEC_ENV char *str, int32_t i1, int32_t i2)
{ DEBUG_मेनसाहे (WASM, exec_env, "%s: %d, %d", str, i1, i2); }
static void debugprint3 (HB_WASM_EXEC_ENV char *str, int32_t i1, int32_t i2, int32_t i3)
{ DEBUG_मेनसाहे (WASM, exec_env, "%s: %d, %d, %d", str, i1, i2, i3); }
static void debugprint4 (HB_WASM_EXEC_ENV char *str, int32_t i1, int32_t i2, int32_t i3, int32_t i4)
{ DEBUG_मेनसाहे (WASM, exec_env, "%s: %d, %d, %d, %d", str, i1, i2, i3, i4); }

}}
#endif

#define NATIVE_सिम्बोलो(signature, name) {#name, (void *) hb::wasm::name, signature, NULL}
/* Note: the array must be static defined since runtime will keep it after registration.
 * Also not const, because it modifies it (sorts it).
 * https://github.com/bytecodealliance/wasm-micro-runtime/blob/main/doc/export_native_api.md
 *
 * TODO Allocate this lazily in _hb_wasm_init(). */
static Native_सिम्बोलो _hb_wasm_native_symbols[] =
{
  /* common */
  NATIVE_सिम्बोलो ("(i)i",	script_get_horizontal_direction),

  /* blob */
  NATIVE_सिम्बोलो ("(i)",		blob_free),

  /* buffer */
  NATIVE_सिम्बोलो ("(i)",		buffer_contents_free),
  NATIVE_सिम्बोलो ("(ii)i",	buffer_contents_realloc),
  NATIVE_सिम्बोलो ("(ii)i",	buffer_copy_contents),
  NATIVE_सिम्बोलो ("(ii)i",	buffer_set_contents),
  NATIVE_सिम्बोलो ("(i)i",	buffer_get_direction),
  NATIVE_सिम्बोलो ("(i)i",	buffer_get_script),
  NATIVE_सिम्बोलो ("(i)",		buffer_reverse),
  NATIVE_सिम्बोलो ("(i)",		buffer_reverse_clusters),

  /* face */
  NATIVE_सिम्बोलो ("(ii)i",	face_create),
  NATIVE_सिम्बोलो ("(iii)i",	face_copy_table),
  NATIVE_सिम्बोलो ("(i)i",	face_get_upem),

  /* font */
  NATIVE_सिम्बोलो ("(i)i",	font_create),
  NATIVE_सिम्बोलो ("(i)i",	font_get_face),
  NATIVE_सिम्बोलो ("(iii)",	font_get_scale),
  NATIVE_सिम्बोलो ("(iii)i",	font_get_glyph),
  NATIVE_सिम्बोलो ("(ii)i",	font_get_glyph_h_advance),
  NATIVE_सिम्बोलो ("(ii)i",	font_get_glyph_v_advance),
  NATIVE_सिम्बोलो ("(iii)i",	font_get_glyph_extents),
  NATIVE_सिम्बोलो ("(iiii)",	font_glyph_to_string),
  NATIVE_सिम्बोलो ("(iii)i",	font_copy_glyph_outline),
  NATIVE_सिम्बोलो ("(ii)i",	font_copy_coords),
  NATIVE_सिम्बोलो ("(ii)i",	font_set_coords),

  /* outline */
  NATIVE_सिम्बोलो ("(i)",		glyph_outline_free),

  /* shape */
  NATIVE_सिम्बोलो ("(iiii$)i",	shape_with),
  NATIVE_सिम्बोलो ("(iii)i",	feature_copy_contents),
  NATIVE_सिम्बोलो ("(i)",		features_free),

  /* debug */
#ifdef HB_DEBUG_WASM
  NATIVE_सिम्बोलो ("($)",		debugprint),
  NATIVE_सिम्बोलो ("($i)",	debugprint1),
  NATIVE_सिम्बोलो ("($ii)",	debugprint2),
  NATIVE_सिम्बोलो ("($iii)",	debugprint3),
  NATIVE_सिम्बोलो ("($iiii)",	debugprint4),
#endif

};
#undef NATIVE_सिम्बोलो


#endif /* HB_WASM_API_LIST_HH */
