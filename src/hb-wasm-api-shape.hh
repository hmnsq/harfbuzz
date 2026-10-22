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

#شامل "hb-wasm-api.hh"

فضاینام hb {
فضاینام wasm {


static_assert (اندازه (feature_t) == اندازه (hb_feature_t), "");

HB_WASM_INTERFACE (bool_t, shape_with) (HB_WASM_EXEC_ENV
				        ptr_d(font_t, font),
				        ptr_d(buffer_t, buffer),
				        ptr_d(ثابت feature_t, features),
				        uint32_t رقم_features,
					ثابت نویسه *shaper)
{
  اگر (unlikely (0 == strcmp (shaper, "wasm")))
    بازگشت نادرست;

  HB_REF2OBJ (font);
  HB_REF2OBJ (buffer);

  /* Pre-conditions that make hb_shape_full() crash should be checked here. */

  اگر (unlikely (!buffer->ensure_unicode ()))
    بازگشت نادرست;

  اگر (unlikely (!HB_DIRECTION_IS_VALID (buffer->props.direction)))
    بازگشت نادرست;

  HB_ARRAY_PARAM (ثابت feature_t, features, رقم_features);
  اگر (unlikely (!features && رقم_features))
    بازگشت نادرست;

  ثابت نویسه * shaper_list[] = {shaper, nullptr};
  بازگشت hb_shape_full (font, buffer,
			(hb_feature_t *) features, رقم_features,
			shaper_list);
}

HB_WASM_API (bool_t, feature_copy_contents) (HB_WASM_EXEC_ENV
					       ptr_d(ثابت feature_t, features),
					       uint32_t رقم_features,
					       ptr_d(features_t, output))
{
  HB_PTR_PARAM (features_t, output);
  اگر (unlikely (!output))
    بازگشت نادرست;

  unsigned bytes;
  اگر (unlikely (hb_unsigned_mul_overflows (رقم_features, اندازه (feature_t), &bytes)))
    بازگشت نادرست;

  اگر (!رقم_features)
  {
    output->length = 0;
    output->features = nullref;
    بازگشت درست;
  }

  HB_ARRAY_PARAM (ثابت feature_t, features, رقم_features);
  اگر (unlikely (!features))
    بازگشت نادرست;

  feature_t *dst = nullptr;
  uint32_t dstref = module_malloc (bytes, (تهی **) &dst);
  اگر (unlikely (!dstref))
    بازگشت نادرست;

  hb_memcpy (dst, features, bytes);
  output->length = رقم_features;
  output->features = dstref;
  بازگشت درست;
}

HB_WASM_API (تهی, features_free) (HB_WASM_EXEC_ENV
				    ptr_d(features_t, features))
{
  HB_PTR_PARAM (features_t, features);
  اگر (unlikely (!features))
    بازگشت;

  module_free (features->features);
  features->features = nullref;
  features->length = 0;
}

}}

#endif /* HB_WASM_API_SHAPE_HH */
