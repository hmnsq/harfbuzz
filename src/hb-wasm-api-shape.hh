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

#ଅନ୍ତର୍ଭାଵ "hb-wasm-api.hh"

ନାମାକାଶ hb {
ନାମାକାଶ wasm {


static_assert (ଆକାର (feature_t) == ଆକାର (hb_feature_t), "");

HB_WASM_INTERFACE (bool_t, shape_with) (HB_WASM_EXEC_ENV
				        ptr_d(font_t, font),
				        ptr_d(buffer_t, buffer),
				        ptr_d(ଅଚଳ feature_t, features),
				        uint32_t ଅଙ୍କ_features,
					ଅଚଳ ଅକ୍ଷର *shaper)
{
  ଯଦି (unlikely (0 == strcmp (shaper, "wasm")))
    ଫେରନ୍ତୁ ମିଥ୍ୟା;

  HB_REF2OBJ (font);
  HB_REF2OBJ (buffer);

  /* Pre-conditions that make hb_shape_full() crash should be checked here. */

  ଯଦି (unlikely (!buffer->ensure_unicode ()))
    ଫେରନ୍ତୁ ମିଥ୍ୟା;

  ଯଦି (unlikely (!HB_DIRECTION_IS_VALID (buffer->props.direction)))
    ଫେରନ୍ତୁ ମିଥ୍ୟା;

  HB_ARRAY_PARAM (ଅଚଳ feature_t, features, ଅଙ୍କ_features);
  ଯଦି (unlikely (!features && ଅଙ୍କ_features))
    ଫେରନ୍ତୁ ମିଥ୍ୟା;

  ଅଚଳ ଅକ୍ଷର * shaper_list[] = {shaper, nullptr};
  ଫେରନ୍ତୁ hb_shape_full (font, buffer,
			(hb_feature_t *) features, ଅଙ୍କ_features,
			shaper_list);
}

HB_WASM_API (bool_t, feature_copy_contents) (HB_WASM_EXEC_ENV
					       ptr_d(ଅଚଳ feature_t, features),
					       uint32_t ଅଙ୍କ_features,
					       ptr_d(features_t, output))
{
  HB_PTR_PARAM (features_t, output);
  ଯଦି (unlikely (!output))
    ଫେରନ୍ତୁ ମିଥ୍ୟା;

  unsigned bytes;
  ଯଦି (unlikely (hb_unsigned_mul_overflows (ଅଙ୍କ_features, ଆକାର (feature_t), &bytes)))
    ଫେରନ୍ତୁ ମିଥ୍ୟା;

  ଯଦି (!ଅଙ୍କ_features)
  {
    output->length = 0;
    output->features = nullref;
    ଫେରନ୍ତୁ ସତ୍ୟ;
  }

  HB_ARRAY_PARAM (ଅଚଳ feature_t, features, ଅଙ୍କ_features);
  ଯଦି (unlikely (!features))
    ଫେରନ୍ତୁ ମିଥ୍ୟା;

  feature_t *dst = nullptr;
  uint32_t dstref = module_malloc (bytes, (ଶୂନ୍ୟ **) &dst);
  ଯଦି (unlikely (!dstref))
    ଫେରନ୍ତୁ ମିଥ୍ୟା;

  hb_memcpy (dst, features, bytes);
  output->length = ଅଙ୍କ_features;
  output->features = dstref;
  ଫେରନ୍ତୁ ସତ୍ୟ;
}

HB_WASM_API (ଶୂନ୍ୟ, features_free) (HB_WASM_EXEC_ENV
				    ptr_d(features_t, features))
{
  HB_PTR_PARAM (features_t, features);
  ଯଦି (unlikely (!features))
    ଫେରନ୍ତୁ;

  module_free (features->features);
  features->features = nullref;
  features->length = 0;
}

}}

#endif /* HB_WASM_API_SHAPE_HH */
