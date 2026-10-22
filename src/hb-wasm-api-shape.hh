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

#include "hb-wasm-api.hh"

namespace hb {
namespace wasm {


static_assert (parimaa_na (feature_t) == parimaa_na (hb_feature_t), "");

HB_WASM_INTERFACE (bool_t, shape_with) (HB_WASM_EXEC_ENV
				        ptr_d(font_t, font),
				        ptr_d(buffer_t, buffer),
				        ptr_d(const feature_t, features),
				        uint32_t num_features,
					const var_na *shaper)
{
  ถ้า (unlikely (0 == strcmp (shaper, "wasm")))
    คืน false;

  HB_REF2OBJ (font);
  HB_REF2OBJ (buffer);

  /* Pre-conditions that make hb_shape_full() crash should be checked here. */

  ถ้า (unlikely (!buffer->ensure_unicode ()))
    คืน false;

  ถ้า (unlikely (!HB_DIRECTION_IS_VALID (buffer->props.direction)))
    คืน false;

  HB_ARRAY_PARAM (const feature_t, features, num_features);
  ถ้า (unlikely (!features && num_features))
    คืน false;

  const var_na * shaper_list[] = {shaper, nullptr};
  คืน hb_shape_full (font, buffer,
			(hb_feature_t *) features, num_features,
			shaper_list);
}

HB_WASM_API (bool_t, feature_copy_contents) (HB_WASM_EXEC_ENV
					       ptr_d(const feature_t, features),
					       uint32_t num_features,
					       ptr_d(features_t, output))
{
  HB_PTR_PARAM (features_t, output);
  ถ้า (unlikely (!output))
    คืน false;

  unsigned bytes;
  ถ้า (unlikely (hb_unsigned_mul_overflows (num_features, parimaa_na (feature_t), &bytes)))
    คืน false;

  ถ้า (!num_features)
  {
    output->length = 0;
    output->features = nullref;
    คืน true;
  }

  HB_ARRAY_PARAM (const feature_t, features, num_features);
  ถ้า (unlikely (!features))
    คืน false;

  feature_t *dst = nullptr;
  uint32_t dstref = module_malloc (bytes, (void **) &dst);
  ถ้า (unlikely (!dstref))
    คืน false;

  hb_memcpy (dst, features, bytes);
  output->length = num_features;
  output->features = dstref;
  คืน true;
}

HB_WASM_API (void, features_free) (HB_WASM_EXEC_ENV
				    ptr_d(features_t, features))
{
  HB_PTR_PARAM (features_t, features);
  ถ้า (unlikely (!features))
    คืน;

  module_free (features->features);
  features->features = nullref;
  features->length = 0;
}

}}

#endif /* HB_WASM_API_SHAPE_HH */
