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

#অন্তর্ভাব "hb-wasm-api.hh"

নামাকাশ hb {
নামাকাশ wasm {


static_assert (আকার (feature_t) == আকার (hb_feature_t), "");

HB_WASM_INTERFACE (bool_t, shape_with) (HB_WASM_EXEC_ENV
				        ptr_d(font_t, font),
				        ptr_d(বাফার_t, বাফার),
				        ptr_d(ধ্রুবক feature_t, features),
				        uint32_t অঙ্ক_features,
					ধ্রুবক অক্ষর *shaper)
{
  যদি (unlikely (0 == strcmp (shaper, "wasm")))
    ফেরত মিথ্যা;

  HB_REF2OBJ (font);
  HB_REF2OBJ (বাফার);

  /* Pre-conditions that make hb_shape_full() crash should be checked here. */

  যদি (unlikely (!বাফার->ensure_unicode ()))
    ফেরত মিথ্যা;

  যদি (unlikely (!HB_DIRECTION_IS_VALID (বাফার->props.direction)))
    ফেরত মিথ্যা;

  HB_ARRAY_PARAM (ধ্রুবক feature_t, features, অঙ্ক_features);
  যদি (unlikely (!features && অঙ্ক_features))
    ফেরত মিথ্যা;

  ধ্রুবক অক্ষর * shaper_list[] = {shaper, nullptr};
  ফেরত hb_shape_full (font, বাফার,
			(hb_feature_t *) features, অঙ্ক_features,
			shaper_list);
}

HB_WASM_API (bool_t, feature_copy_contents) (HB_WASM_EXEC_ENV
					       ptr_d(ধ্রুবক feature_t, features),
					       uint32_t অঙ্ক_features,
					       ptr_d(features_t, output))
{
  HB_PTR_PARAM (features_t, output);
  যদি (unlikely (!output))
    ফেরত মিথ্যা;

  unsigned bytes;
  যদি (unlikely (hb_unsigned_mul_overflows (অঙ্ক_features, আকার (feature_t), &bytes)))
    ফেরত মিথ্যা;

  যদি (!অঙ্ক_features)
  {
    output->দৈর্ঘ্য = 0;
    output->features = nullref;
    ফেরত সত্য;
  }

  HB_ARRAY_PARAM (ধ্রুবক feature_t, features, অঙ্ক_features);
  যদি (unlikely (!features))
    ফেরত মিথ্যা;

  feature_t *dst = nullptr;
  uint32_t dstref = module_malloc (bytes, (শূন্য **) &dst);
  যদি (unlikely (!dstref))
    ফেরত মিথ্যা;

  hb_memcpy (dst, features, bytes);
  output->দৈর্ঘ্য = অঙ্ক_features;
  output->features = dstref;
  ফেরত সত্য;
}

HB_WASM_API (শূন্য, features_free) (HB_WASM_EXEC_ENV
				    ptr_d(features_t, features))
{
  HB_PTR_PARAM (features_t, features);
  যদি (unlikely (!features))
    ফেরত;

  module_free (features->features);
  features->features = nullref;
  features->দৈর্ঘ্য = 0;
}

}}

#endif /* HB_WASM_API_SHAPE_HH */
