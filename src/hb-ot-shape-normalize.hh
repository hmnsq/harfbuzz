/*
 * Copyright © 2012  Google, Inc.
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
 *
 * Google Author(s): Behdad Esfahbod
 */

#ifndef HB_OT_SHAPE_NORMALIZE_HH
#define HB_OT_SHAPE_NORMALIZE_HH

#ଅନ୍ତର୍ଭାଵ "hb.hh"
#ଅନ୍ତର୍ଭାଵ "hb-unicode.hh"


/* buffer var allocations, used during the normalization process */
#define normalizer_glyph_index()	var1.u32

ସଂରଚନା hb_ot_shape_plan_t;

ଗଣନା hb_ot_shape_normalization_mode_t {
  HB_OT_SHAPE_NORMALIZATION_MODE_NONE,
  HB_OT_SHAPE_NORMALIZATION_MODE_DECOMPOSED,
  HB_OT_SHAPE_NORMALIZATION_MODE_COMPOSED_DIACRITICS, /* Never composes base-to-base */
  HB_OT_SHAPE_NORMALIZATION_MODE_COMPOSED_DIACRITICS_NO_SHORT_CIRCUIT, /* Always fully decomposes and then recompose back */

  HB_OT_SHAPE_NORMALIZATION_MODE_AUTO, /* See hb-ot-shape-normalize.cc for logic. */
  HB_OT_SHAPE_NORMALIZATION_MODE_DEFAULT = HB_OT_SHAPE_NORMALIZATION_MODE_AUTO
};

HB_INTERNAL ଶୂନ୍ୟ _hb_ot_shape_normalize (ଅଚଳ hb_ot_shape_plan_t *shaper,
					 hb_buffer_t *buffer,
					 hb_font_t *font);


ସଂରଚନା hb_ot_shape_normalize_context_t
{
  ସ୍ଥିର ତାର୍କିକ
  decompose_unicode (ଅଚଳ hb_ot_shape_normalize_context_t *c,
		     hb_codepoint_t  ab,
		     hb_codepoint_t *a,
		     hb_codepoint_t *b)
  {
    ଫେରନ୍ତୁ (ତାର୍କିକ) c->unicode->decompose (ab, a, b);
  }

  ସ୍ଥିର ତାର୍କିକ
  compose_unicode (ଅଚଳ hb_ot_shape_normalize_context_t *c,
		   hb_codepoint_t  a,
		   hb_codepoint_t  b,
		   hb_codepoint_t *ab)
  {
    ଫେରନ୍ତୁ (ତାର୍କିକ) c->unicode->compose (a, b, ab);
  }

  ଶୂନ୍ୟ
  override_decompose_and_compose (ତାର୍କିକ (*decompose) (ଅଚଳ hb_ot_shape_normalize_context_t *c,
						      hb_codepoint_t  ab,
						      hb_codepoint_t *a,
						      hb_codepoint_t *b),
				  ତାର୍କିକ (*compose) (ଅଚଳ hb_ot_shape_normalize_context_t *c,
						   hb_codepoint_t  a,
						   hb_codepoint_t  b,
						   hb_codepoint_t *ab))
  {
    ଏହି->decompose = decompose ? decompose : decompose_unicode;
    ଏହି->compose = compose ? compose : compose_unicode;
  }

  ଅଚଳ hb_ot_shape_plan_t *plan;
  hb_buffer_t *buffer;
  hb_font_t *font;
  hb_unicode_funcs_t *unicode;
  ତାର୍କିକ (*decompose) (ଅଚଳ hb_ot_shape_normalize_context_t *c,
		     hb_codepoint_t  ab,
		     hb_codepoint_t *a,
		     hb_codepoint_t *b);
  ତାର୍କିକ (*compose) (ଅଚଳ hb_ot_shape_normalize_context_t *c,
		   hb_codepoint_t  a,
		   hb_codepoint_t  b,
		   hb_codepoint_t *ab);
};


#endif /* HB_OT_SHAPE_NORMALIZE_HH */
