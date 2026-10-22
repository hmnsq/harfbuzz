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

#اگر_غیر_تعریف_شدہ HB_WASM_API_SHAPE_HH
#تعریف_کرو HB_WASM_API_SHAPE_HH

#شامل "hb-wasm-api.hh"

نامخلا hb {
نامخلا wasm {


static_assert (حجم (feature_t) == حجم (hb_feature_t), "");

HB_WASM_INTERFACE (bool_t, shape_with) (HB_WASM_EXEC_ENV
				        ptr_d(font_t, font),
				        ptr_d(بفر_t, بفر),
				        ptr_d(ثابت feature_t, features),
				        uint32_t عدد_features,
					ثابت حرف *shaper)
{
  اگر (unlikely (0 == strcmp (shaper, "wasm")))
    واپس جھوٹ;

  HB_REF2OBJ (font);
  HB_REF2OBJ (بفر);

  /* Pre-conditions that make hb_shape_full() crash should be checked here. */

  اگر (unlikely (!بفر->ensure_unicode ()))
    واپس جھوٹ;

  اگر (unlikely (!HB_DIRECTION_IS_VALID (بفر->props.direction)))
    واپس جھوٹ;

  HB_ARRAY_PARAM (ثابت feature_t, features, عدد_features);
  اگر (unlikely (!features && عدد_features))
    واپس جھوٹ;

  ثابت حرف * shaper_list[] = {shaper, خالی_اشاریہ};
  واپس hb_shape_بھرا (font, بفر,
			(hb_feature_t *) features, عدد_features,
			shaper_list);
}

HB_WASM_API (bool_t, feature_copy_contents) (HB_WASM_EXEC_ENV
					       ptr_d(ثابت feature_t, features),
					       uint32_t عدد_features,
					       ptr_d(features_t, output))
{
  HB_PTR_PARAM (features_t, output);
  اگر (unlikely (!output))
    واپس جھوٹ;

  غیرعلامتی بائٹیں;
  اگر (unlikely (hb_unsigned_mul_overflows (عدد_features, حجم (feature_t), &بائٹیں)))
    واپس جھوٹ;

  اگر (!عدد_features)
  {
    output->لمبائی = 0;
    output->features = nullref;
    واپس سچ;
  }

  HB_ARRAY_PARAM (ثابت feature_t, features, عدد_features);
  اگر (unlikely (!features))
    واپس جھوٹ;

  feature_t *dst = خالی_اشاریہ;
  uint32_t dstref = module_malloc (بائٹیں, (خالی **) &dst);
  اگر (unlikely (!dstref))
    واپس جھوٹ;

  hb_memcpy (dst, features, بائٹیں);
  output->لمبائی = عدد_features;
  output->features = dstref;
  واپس سچ;
}

HB_WASM_API (خالی, features_free) (HB_WASM_EXEC_ENV
				    ptr_d(features_t, features))
{
  HB_PTR_PARAM (features_t, features);
  اگر (unlikely (!features))
    واپس;

  module_free (features->features);
  features->features = nullref;
  features->لمبائی = 0;
}

}}

#اگر_ختم /* HB_WASM_API_SHAPE_HH */
