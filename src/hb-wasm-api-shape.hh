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

#अगर_अघोषित HB_WASM_API_SHAPE_HH
#घोषणा HB_WASM_API_SHAPE_HH

#समावेश "hb-wasm-api.hh"

namespace hb {
namespace wasm {


static_assert (माप (feature_t) == माप (hb_feature_t), "");

HB_WASM_INTERFACE (bool_t, shape_with) (HB_WASM_EXEC_ENV
				        ptr_d(font_t, font),
				        ptr_d(बफर_t, बफर),
				        ptr_d(स्थिर feature_t, features),
				        uint32_t अंक_features,
					स्थिर अक्षर *shaper)
{
  अगर (unlikely (0 == म_भेद (shaper, "wasm")))
    वापस false;

  HB_REF2OBJ (font);
  HB_REF2OBJ (बफर);

  /* Pre-conditions that make hb_shape_full() crash should be checked here. */

  अगर (unlikely (!बफर->ensure_unicode ()))
    वापस false;

  अगर (unlikely (!HB_DIRECTION_IS_VALID (बफर->props.direction)))
    वापस false;

  HB_ARRAY_PARAM (स्थिर feature_t, features, अंक_features);
  अगर (unlikely (!features && अंक_features))
    वापस false;

  स्थिर अक्षर * shaper_list[] = {shaper, nullptr};
  वापस hb_shape_full (font, बफर,
			(hb_feature_t *) features, अंक_features,
			shaper_list);
}

HB_WASM_API (bool_t, feature_copy_contents) (HB_WASM_EXEC_ENV
					       ptr_d(स्थिर feature_t, features),
					       uint32_t अंक_features,
					       ptr_d(features_t, output))
{
  HB_PTR_PARAM (features_t, output);
  अगर (unlikely (!output))
    वापस false;

  अचिन्हित bytes;
  अगर (unlikely (hb_unsigned_mul_overflows (अंक_features, माप (feature_t), &bytes)))
    वापस false;

  अगर (!अंक_features)
  {
    output->लंबाई = 0;
    output->features = nullref;
    वापस true;
  }

  HB_ARRAY_PARAM (स्थिर feature_t, features, अंक_features);
  अगर (unlikely (!features))
    वापस false;

  feature_t *dst = nullptr;
  uint32_t dstref = module_malloc (bytes, (व्योम **) &dst);
  अगर (unlikely (!dstref))
    वापस false;

  hb_memcpy (dst, features, bytes);
  output->लंबाई = अंक_features;
  output->features = dstref;
  वापस true;
}

HB_WASM_API (व्योम, features_free) (HB_WASM_EXEC_ENV
				    ptr_d(features_t, features))
{
  HB_PTR_PARAM (features_t, features);
  अगर (unlikely (!features))
    वापस;

  module_free (features->features);
  features->features = nullref;
  features->लंबाई = 0;
}

}}

#पूर्ण_अगर /* HB_WASM_API_SHAPE_HH */
