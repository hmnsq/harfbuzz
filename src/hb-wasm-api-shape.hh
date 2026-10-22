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

#அந்தர்பாவ "hb-wasm-api.hh"

நாமாகாஶ hb {
நாமாகாஶ wasm {


static_assert (அளவு (feature_t) == அளவு (hb_feature_t), "");

HB_WASM_INTERFACE (bool_t, shape_with) (HB_WASM_EXEC_ENV
				        ptr_d(font_t, font),
				        ptr_d(buffer_t, buffer),
				        ptr_d(மாறிலி feature_t, features),
				        uint32_t இலக்கம்_features,
					மாறிலி எழுத்து *shaper)
{
  எனில் (unlikely (0 == strcmp (shaper, "wasm")))
    திரும்பு பொய்;

  HB_REF2OBJ (font);
  HB_REF2OBJ (buffer);

  /* Pre-conditions that make hb_shape_full() crash should be checked here. */

  எனில் (unlikely (!buffer->ensure_unicode ()))
    திரும்பு பொய்;

  எனில் (unlikely (!HB_DIRECTION_IS_VALID (buffer->props.direction)))
    திரும்பு பொய்;

  HB_ARRAY_PARAM (மாறிலி feature_t, features, இலக்கம்_features);
  எனில் (unlikely (!features && இலக்கம்_features))
    திரும்பு பொய்;

  மாறிலி எழுத்து * shaper_list[] = {shaper, nullptr};
  திரும்பு hb_shape_full (font, buffer,
			(hb_feature_t *) features, இலக்கம்_features,
			shaper_list);
}

HB_WASM_API (bool_t, feature_copy_contents) (HB_WASM_EXEC_ENV
					       ptr_d(மாறிலி feature_t, features),
					       uint32_t இலக்கம்_features,
					       ptr_d(features_t, output))
{
  HB_PTR_PARAM (features_t, output);
  எனில் (unlikely (!output))
    திரும்பு பொய்;

  unsigned bytes;
  எனில் (unlikely (hb_unsigned_mul_overflows (இலக்கம்_features, அளவு (feature_t), &bytes)))
    திரும்பு பொய்;

  எனில் (!இலக்கம்_features)
  {
    output->length = 0;
    output->features = nullref;
    திரும்பு உண்மை;
  }

  HB_ARRAY_PARAM (மாறிலி feature_t, features, இலக்கம்_features);
  எனில் (unlikely (!features))
    திரும்பு பொய்;

  feature_t *dst = nullptr;
  uint32_t dstref = module_malloc (bytes, (வெறுமை **) &dst);
  எனில் (unlikely (!dstref))
    திரும்பு பொய்;

  hb_memcpy (dst, features, bytes);
  output->length = இலக்கம்_features;
  output->features = dstref;
  திரும்பு உண்மை;
}

HB_WASM_API (வெறுமை, features_free) (HB_WASM_EXEC_ENV
				    ptr_d(features_t, features))
{
  HB_PTR_PARAM (features_t, features);
  எனில் (unlikely (!features))
    திரும்பு;

  module_free (features->features);
  features->features = nullref;
  features->length = 0;
}

}}

#endif /* HB_WASM_API_SHAPE_HH */
