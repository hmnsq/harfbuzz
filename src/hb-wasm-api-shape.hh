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


static_assert (आकार (feature_t) == आकार (hb_feature_t), "");

HB_WASM_INTERFACE (bool_t, shape_with) (HB_WASM_EXEC_ENV
				        ptr_d(font_t, font),
				        ptr_d(buffer_t, buffer),
				        ptr_d(स्थिरांक feature_t, features),
				        uint32_t अंक_features,
					स्थिरांक अक्षर *shaper)
{
  जर (unlikely (0 == म_भेद (shaper, "wasm")))
    परत असत्य;

  HB_REF2OBJ (font);
  HB_REF2OBJ (buffer);

  /* Pre-conditions that make hb_shape_full() crash should be checked here. */

  जर (unlikely (!buffer->ensure_unicode ()))
    परत असत्य;

  जर (unlikely (!HB_DIRECTION_IS_VALID (buffer->props.direction)))
    परत असत्य;

  HB_ARRAY_PARAM (स्थिरांक feature_t, features, अंक_features);
  जर (unlikely (!features && अंक_features))
    परत असत्य;

  स्थिरांक अक्षर * shaper_list[] = {shaper, nullptr};
  परत hb_shape_full (font, buffer,
			(hb_feature_t *) features, अंक_features,
			shaper_list);
}

HB_WASM_API (bool_t, feature_copy_contents) (HB_WASM_EXEC_ENV
					       ptr_d(स्थिरांक feature_t, features),
					       uint32_t अंक_features,
					       ptr_d(features_t, output))
{
  HB_PTR_PARAM (features_t, output);
  जर (unlikely (!output))
    परत असत्य;

  अचिन्हित bytes;
  जर (unlikely (hb_unsigned_mul_overflows (अंक_features, आकार (feature_t), &bytes)))
    परत असत्य;

  जर (!अंक_features)
  {
    output->length = 0;
    output->features = nullref;
    परत सत्य;
  }

  HB_ARRAY_PARAM (स्थिरांक feature_t, features, अंक_features);
  जर (unlikely (!features))
    परत असत्य;

  feature_t *dst = nullptr;
  uint32_t dstref = module_malloc (bytes, (शून्य **) &dst);
  जर (unlikely (!dstref))
    परत असत्य;

  hb_memcpy (dst, features, bytes);
  output->length = अंक_features;
  output->features = dstref;
  परत सत्य;
}

HB_WASM_API (शून्य, features_free) (HB_WASM_EXEC_ENV
				    ptr_d(features_t, features))
{
  HB_PTR_PARAM (features_t, features);
  जर (unlikely (!features))
    परत;

  module_free (features->features);
  features->features = nullref;
  features->length = 0;
}

}}

#endif /* HB_WASM_API_SHAPE_HH */
