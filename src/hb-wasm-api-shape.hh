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
  གལ་ཏེ (unlikely (0 == strcmp (shaper, "wasm")))
    ཕྱིར false;

  HB_REF2OBJ (font);
  HB_REF2OBJ (buffer);

  /* Pre-conditions that make hb_shape_full() crash should be checked here. */

  གལ་ཏེ (unlikely (!buffer->ensure_unicode ()))
    ཕྱིར false;

  གལ་ཏེ (unlikely (!HB_DIRECTION_IS_VALID (buffer->props.direction)))
    ཕྱིར false;

  HB_ARRAY_PARAM (const feature_t, features, num_features);
  གལ་ཏེ (unlikely (!features && num_features))
    ཕྱིར false;

  const var_na * shaper_list[] = {shaper, nullptr};
  ཕྱིར hb_shape_full (font, buffer,
			(hb_feature_t *) features, num_features,
			shaper_list);
}

HB_WASM_API (bool_t, feature_copy_contents) (HB_WASM_EXEC_ENV
					       ptr_d(const feature_t, features),
					       uint32_t num_features,
					       ptr_d(features_t, output))
{
  HB_PTR_PARAM (features_t, output);
  གལ་ཏེ (unlikely (!output))
    ཕྱིར false;

  unsigned bytes;
  གལ་ཏེ (unlikely (hb_unsigned_mul_overflows (num_features, parimaa_na (feature_t), &bytes)))
    ཕྱིར false;

  གལ་ཏེ (!num_features)
  {
    output->length = 0;
    output->features = nullref;
    ཕྱིར true;
  }

  HB_ARRAY_PARAM (const feature_t, features, num_features);
  གལ་ཏེ (unlikely (!features))
    ཕྱིར false;

  feature_t *dst = nullptr;
  uint32_t dstref = module_malloc (bytes, (void **) &dst);
  གལ་ཏེ (unlikely (!dstref))
    ཕྱིར false;

  hb_memcpy (dst, features, bytes);
  output->length = num_features;
  output->features = dstref;
  ཕྱིར true;
}

HB_WASM_API (void, features_free) (HB_WASM_EXEC_ENV
				    ptr_d(features_t, features))
{
  HB_PTR_PARAM (features_t, features);
  གལ་ཏེ (unlikely (!features))
    ཕྱིར;

  module_free (features->features);
  features->features = nullref;
  features->length = 0;
}

}}

#endif /* HB_WASM_API_SHAPE_HH */
