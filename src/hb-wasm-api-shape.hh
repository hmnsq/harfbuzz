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

#ಅನ್ತರ್ಭಾವ "hb-wasm-api.hh"

ನಾಮಾಕಾಶ hb {
ನಾಮಾಕಾಶ wasm {


static_assert (ಪರಿಮಾಣ (feature_t) == ಪರಿಮಾಣ (hb_feature_t), "");

HB_WASM_INTERFACE (bool_t, shape_with) (HB_WASM_EXEC_ENV
				        ptr_d(font_t, font),
				        ptr_d(buffer_t, buffer),
				        ptr_d(ಧ್ರುವ feature_t, features),
				        uint32_t ಅಂಕೆ_features,
					ಧ್ರುವ ವರ್ಣ *shaper)
{
  ಯದಿ (unlikely (0 == strcmp (shaper, "wasm")))
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

  HB_REF2OBJ (font);
  HB_REF2OBJ (buffer);

  /* Pre-conditions that make hb_shape_full() crash should be checked here. */

  ಯದಿ (unlikely (!buffer->ensure_unicode ()))
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

  ಯದಿ (unlikely (!HB_DIRECTION_IS_VALID (buffer->props.direction)))
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

  HB_ARRAY_PARAM (ಧ್ರುವ feature_t, features, ಅಂಕೆ_features);
  ಯದಿ (unlikely (!features && ಅಂಕೆ_features))
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

  ಧ್ರುವ ವರ್ಣ * shaper_list[] = {shaper, nullptr};
  ಪ್ರತ್ಯಾಗಮ hb_shape_full (font, buffer,
			(hb_feature_t *) features, ಅಂಕೆ_features,
			shaper_list);
}

HB_WASM_API (bool_t, feature_copy_contents) (HB_WASM_EXEC_ENV
					       ptr_d(ಧ್ರುವ feature_t, features),
					       uint32_t ಅಂಕೆ_features,
					       ptr_d(features_t, output))
{
  HB_PTR_PARAM (features_t, output);
  ಯದಿ (unlikely (!output))
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

  unsigned bytes;
  ಯದಿ (unlikely (hb_unsigned_mul_overflows (ಅಂಕೆ_features, ಪರಿಮಾಣ (feature_t), &bytes)))
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

  ಯದಿ (!ಅಂಕೆ_features)
  {
    output->length = 0;
    output->features = nullref;
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }

  HB_ARRAY_PARAM (ಧ್ರುವ feature_t, features, ಅಂಕೆ_features);
  ಯದಿ (unlikely (!features))
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

  feature_t *dst = nullptr;
  uint32_t dstref = module_malloc (bytes, (ಶೂನ್ಯ **) &dst);
  ಯದಿ (unlikely (!dstref))
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

  hb_memcpy (dst, features, bytes);
  output->length = ಅಂಕೆ_features;
  output->features = dstref;
  ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
}

HB_WASM_API (ಶೂನ್ಯ, features_free) (HB_WASM_EXEC_ENV
				    ptr_d(features_t, features))
{
  HB_PTR_PARAM (features_t, features);
  ಯದಿ (unlikely (!features))
    ಪ್ರತ್ಯಾಗಮ;

  module_free (features->features);
  features->features = nullref;
  features->length = 0;
}

}}

#endif /* HB_WASM_API_SHAPE_HH */
