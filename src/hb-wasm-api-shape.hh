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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_WASM_API_SHAPE_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_WASM_API_SHAPE_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-wasm-api.hh"

ਨਾਮਾਕਾਸ਼ hb {
ਨਾਮਾਕਾਸ਼ wasm {


static_assert (ਆਕਾਰ (feature_t) == ਆਕਾਰ (hb_feature_t), "");

HB_WASM_INTERFACE (bool_t, shape_with) (HB_WASM_EXEC_ENV
				        ptr_d(font_t, font),
				        ptr_d(ਬਫਰ_t, ਬਫਰ),
				        ptr_d(ਅਚਲ feature_t, features),
				        uint32_t ਅੰਕ_features,
					ਅਚਲ ਅੱਖਰ *shaper)
{
  ਜੇ (unlikely (0 == strcmp (shaper, "wasm")))
    ਵਾਪਸ ਝੂਠ;

  HB_REF2OBJ (font);
  HB_REF2OBJ (ਬਫਰ);

  /* Pre-conditions that make hb_shape_full() crash should be checked here. */

  ਜੇ (unlikely (!ਬਫਰ->ensure_unicode ()))
    ਵਾਪਸ ਝੂਠ;

  ਜੇ (unlikely (!HB_DIRECTION_IS_VALID (ਬਫਰ->props.direction)))
    ਵਾਪਸ ਝੂਠ;

  HB_ARRAY_PARAM (ਅਚਲ feature_t, features, ਅੰਕ_features);
  ਜੇ (unlikely (!features && ਅੰਕ_features))
    ਵਾਪਸ ਝੂਠ;

  ਅਚਲ ਅੱਖਰ * shaper_list[] = {shaper, ਖਾਲੀ_ਸੂਚਕ};
  ਵਾਪਸ hb_shape_ਭਰਿਆ (font, ਬਫਰ,
			(hb_feature_t *) features, ਅੰਕ_features,
			shaper_list);
}

HB_WASM_API (bool_t, feature_copy_contents) (HB_WASM_EXEC_ENV
					       ptr_d(ਅਚਲ feature_t, features),
					       uint32_t ਅੰਕ_features,
					       ptr_d(features_t, output))
{
  HB_PTR_PARAM (features_t, output);
  ਜੇ (unlikely (!output))
    ਵਾਪਸ ਝੂਠ;

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਬਾਈਟਾਂ;
  ਜੇ (unlikely (hb_unsigned_mul_overflows (ਅੰਕ_features, ਆਕਾਰ (feature_t), &ਬਾਈਟਾਂ)))
    ਵਾਪਸ ਝੂਠ;

  ਜੇ (!ਅੰਕ_features)
  {
    output->ਲੰਮਾਈ = 0;
    output->features = nullref;
    ਵਾਪਸ ਸੱਚ;
  }

  HB_ARRAY_PARAM (ਅਚਲ feature_t, features, ਅੰਕ_features);
  ਜੇ (unlikely (!features))
    ਵਾਪਸ ਝੂਠ;

  feature_t *dst = ਖਾਲੀ_ਸੂਚਕ;
  uint32_t dstref = module_malloc (ਬਾਈਟਾਂ, (ਖਾਲੀ **) &dst);
  ਜੇ (unlikely (!dstref))
    ਵਾਪਸ ਝੂਠ;

  hb_memcpy (dst, features, ਬਾਈਟਾਂ);
  output->ਲੰਮਾਈ = ਅੰਕ_features;
  output->features = dstref;
  ਵਾਪਸ ਸੱਚ;
}

HB_WASM_API (ਖਾਲੀ, features_free) (HB_WASM_EXEC_ENV
				    ptr_d(features_t, features))
{
  HB_PTR_PARAM (features_t, features);
  ਜੇ (unlikely (!features))
    ਵਾਪਸ;

  module_free (features->features);
  features->features = nullref;
  features->ਲੰਮਾਈ = 0;
}

}}

#ਜੇ_ਅੰਤ /* HB_WASM_API_SHAPE_HH */
