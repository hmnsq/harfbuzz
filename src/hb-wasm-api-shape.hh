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

#నిర్వచితం_కాకపోతే HB_WASM_API_SHAPE_HH
#నిర్వచించు HB_WASM_API_SHAPE_HH

#అన్తర్భావ "hb-wasm-api.hh"

నామాకాశ hb {
నామాకాశ wasm {


static_assert (పరిమాణం (feature_t) == పరిమాణం (hb_feature_t), "");

HB_WASM_INTERFACE (bool_t, shape_with) (HB_WASM_EXEC_ENV
				        ptr_d(font_t, font),
				        ptr_d(బఫరు_t, బఫరు),
				        ptr_d(స్థిరరాశి feature_t, features),
				        uint32_t అంకె_features,
					స్థిరరాశి అక్షరం *shaper)
{
  ఒకవేళ (unlikely (0 == strcmp (shaper, "wasm")))
    తిరిగి అబద్ధం;

  HB_REF2OBJ (font);
  HB_REF2OBJ (బఫరు);

  /* Pre-conditions that make hb_shape_full() crash should be checked here. */

  ఒకవేళ (unlikely (!బఫరు->ensure_unicode ()))
    తిరిగి అబద్ధం;

  ఒకవేళ (unlikely (!HB_DIRECTION_IS_VALID (బఫరు->props.direction)))
    తిరిగి అబద్ధం;

  HB_ARRAY_PARAM (స్థిరరాశి feature_t, features, అంకె_features);
  ఒకవేళ (unlikely (!features && అంకె_features))
    తిరిగి అబద్ధం;

  స్థిరరాశి అక్షరం * shaper_list[] = {shaper, శూన్యసూచిక};
  తిరిగి hb_shape_నిండిన (font, బఫరు,
			(hb_feature_t *) features, అంకె_features,
			shaper_list);
}

HB_WASM_API (bool_t, feature_copy_contents) (HB_WASM_EXEC_ENV
					       ptr_d(స్థిరరాశి feature_t, features),
					       uint32_t అంకె_features,
					       ptr_d(features_t, output))
{
  HB_PTR_PARAM (features_t, output);
  ఒకవేళ (unlikely (!output))
    తిరిగి అబద్ధం;

  గుర్తులేని బైట్లు;
  ఒకవేళ (unlikely (hb_unsigned_mul_overflows (అంకె_features, పరిమాణం (feature_t), &బైట్లు)))
    తిరిగి అబద్ధం;

  ఒకవేళ (!అంకె_features)
  {
    output->నిడివి = 0;
    output->features = nullref;
    తిరిగి నిజం;
  }

  HB_ARRAY_PARAM (స్థిరరాశి feature_t, features, అంకె_features);
  ఒకవేళ (unlikely (!features))
    తిరిగి అబద్ధం;

  feature_t *dst = శూన్యసూచిక;
  uint32_t dstref = module_malloc (బైట్లు, (శూన్య **) &dst);
  ఒకవేళ (unlikely (!dstref))
    తిరిగి అబద్ధం;

  hb_memcpy (dst, features, బైట్లు);
  output->నిడివి = అంకె_features;
  output->features = dstref;
  తిరిగి నిజం;
}

HB_WASM_API (శూన్య, features_free) (HB_WASM_EXEC_ENV
				    ptr_d(features_t, features))
{
  HB_PTR_PARAM (features_t, features);
  ఒకవేళ (unlikely (!features))
    తిరిగి;

  module_free (features->features);
  features->features = nullref;
  features->నిడివి = 0;
}

}}

#అయితే_ముగింపు /* HB_WASM_API_SHAPE_HH */
