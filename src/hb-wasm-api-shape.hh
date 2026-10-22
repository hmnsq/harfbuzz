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

#ifndef HB_WASM_API_SHAPE_HH
#define HB_WASM_API_SHAPE_HH

#അന്തര്ഭാവ "hb-wasm-api.hh"

നാമാകാശ hb {
നാമാകാശ wasm {


static_assert (വലുപ്പം (feature_t) == വലുപ്പം (hb_feature_t), "");

HB_WASM_INTERFACE (bool_t, shape_with) (HB_WASM_EXEC_ENV
				        ptr_d(font_t, font),
				        ptr_d(buffer_t, buffer),
				        ptr_d(സ്ഥിരം feature_t, features),
				        uint32_t അക്കം_features,
					സ്ഥിരം അക്ഷരം *shaper)
{
  എങ്കിൽ (unlikely (0 == strcmp (shaper, "wasm")))
    തിരികെ അസത്യം;

  HB_REF2OBJ (font);
  HB_REF2OBJ (buffer);

  /* Pre-conditions that make hb_shape_full() crash should be checked here. */

  എങ്കിൽ (unlikely (!buffer->ensure_unicode ()))
    തിരികെ അസത്യം;

  എങ്കിൽ (unlikely (!HB_DIRECTION_IS_VALID (buffer->props.direction)))
    തിരികെ അസത്യം;

  HB_ARRAY_PARAM (സ്ഥിരം feature_t, features, അക്കം_features);
  എങ്കിൽ (unlikely (!features && അക്കം_features))
    തിരികെ അസത്യം;

  സ്ഥിരം അക്ഷരം * shaper_list[] = {shaper, nullptr};
  തിരികെ hb_shape_full (font, buffer,
			(hb_feature_t *) features, അക്കം_features,
			shaper_list);
}

HB_WASM_API (bool_t, feature_copy_contents) (HB_WASM_EXEC_ENV
					       ptr_d(സ്ഥിരം feature_t, features),
					       uint32_t അക്കം_features,
					       ptr_d(features_t, output))
{
  HB_PTR_PARAM (features_t, output);
  എങ്കിൽ (unlikely (!output))
    തിരികെ അസത്യം;

  unsigned bytes;
  എങ്കിൽ (unlikely (hb_unsigned_mul_overflows (അക്കം_features, വലുപ്പം (feature_t), &bytes)))
    തിരികെ അസത്യം;

  എങ്കിൽ (!അക്കം_features)
  {
    output->length = 0;
    output->features = nullref;
    തിരികെ സത്യം;
  }

  HB_ARRAY_PARAM (സ്ഥിരം feature_t, features, അക്കം_features);
  എങ്കിൽ (unlikely (!features))
    തിരികെ അസത്യം;

  feature_t *dst = nullptr;
  uint32_t dstref = module_malloc (bytes, (ശൂന്യം **) &dst);
  എങ്കിൽ (unlikely (!dstref))
    തിരികെ അസത്യം;

  hb_memcpy (dst, features, bytes);
  output->length = അക്കം_features;
  output->features = dstref;
  തിരികെ സത്യം;
}

HB_WASM_API (ശൂന്യം, features_free) (HB_WASM_EXEC_ENV
				    ptr_d(features_t, features))
{
  HB_PTR_PARAM (features_t, features);
  എങ്കിൽ (unlikely (!features))
    തിരികെ;

  module_free (features->features);
  features->features = nullref;
  features->length = 0;
}

}}

#endif /* HB_WASM_API_SHAPE_HH */
