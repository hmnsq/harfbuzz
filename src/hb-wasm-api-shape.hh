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

#अन्तर्भाव "hb-wasm-api.hh"

नामाकाश hb {
नामाकाश wasm {


static_assert (परिमाण (feature_t) == परिमाण (hb_feature_t), "");

HB_WASM_INTERFACE (bool_t, shape_with) (HB_WASM_EXEC_ENV
				        ptr_d(font_t, font),
				        ptr_d(buffer_t, buffer),
				        ptr_d(ध्रुव feature_t, features),
				        uint32_t अङ्कः_features,
					ध्रुव वर्ण *shaper)
{
  जइ (unlikely (0 == strcmp (shaper, "wasm")))
    प्रत्यागम मिच्छ;

  HB_REF2OBJ (font);
  HB_REF2OBJ (buffer);

  /* Pre-conditions that make hb_shape_full() crash should be checked here. */

  जइ (unlikely (!buffer->ensure_unicode ()))
    प्रत्यागम मिच्छ;

  जइ (unlikely (!HB_DIRECTION_IS_VALID (buffer->props.direction)))
    प्रत्यागम मिच्छ;

  HB_ARRAY_PARAM (ध्रुव feature_t, features, अङ्कः_features);
  जइ (unlikely (!features && अङ्कः_features))
    प्रत्यागम मिच्छ;

  ध्रुव वर्ण * shaper_list[] = {shaper, nullptr};
  प्रत्यागम hb_shape_full (font, buffer,
			(hb_feature_t *) features, अङ्कः_features,
			shaper_list);
}

HB_WASM_API (bool_t, feature_copy_contents) (HB_WASM_EXEC_ENV
					       ptr_d(ध्रुव feature_t, features),
					       uint32_t अङ्कः_features,
					       ptr_d(features_t, output))
{
  HB_PTR_PARAM (features_t, output);
  जइ (unlikely (!output))
    प्रत्यागम मिच्छ;

  unsigned bytes;
  जइ (unlikely (hb_unsigned_mul_overflows (अङ्कः_features, परिमाण (feature_t), &bytes)))
    प्रत्यागम मिच्छ;

  जइ (!अङ्कः_features)
  {
    output->length = 0;
    output->features = nullref;
    प्रत्यागम सच्च;
  }

  HB_ARRAY_PARAM (ध्रुव feature_t, features, अङ्कः_features);
  जइ (unlikely (!features))
    प्रत्यागम मिच्छ;

  feature_t *dst = nullptr;
  uint32_t dstref = module_malloc (bytes, (शून्य **) &dst);
  जइ (unlikely (!dstref))
    प्रत्यागम मिच्छ;

  hb_memcpy (dst, features, bytes);
  output->length = अङ्कः_features;
  output->features = dstref;
  प्रत्यागम सच्च;
}

HB_WASM_API (शून्य, features_free) (HB_WASM_EXEC_ENV
				    ptr_d(features_t, features))
{
  HB_PTR_PARAM (features_t, features);
  जइ (unlikely (!features))
    प्रत्यागम;

  module_free (features->features);
  features->features = nullref;
  features->length = 0;
}

}}

#endif /* HB_WASM_API_SHAPE_HH */
