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

#ಅನ್ತರ್ಭಾವ "hb.hh"
#ಅನ್ತರ್ಭಾವ "hb-unicode.hh"


/* buffer var allocations, used during the normalization process */
#define normalizer_glyph_index()	var1.u32

ಸಂರಚನಾ hb_ot_shape_plan_t;

ಗಣನಾ hb_ot_shape_normalization_mode_t {
  HB_OT_SHAPE_NORMALIZATION_MODE_NONE,
  HB_OT_SHAPE_NORMALIZATION_MODE_DECOMPOSED,
  HB_OT_SHAPE_NORMALIZATION_MODE_COMPOSED_DIACRITICS, /* Never composes base-to-base */
  HB_OT_SHAPE_NORMALIZATION_MODE_COMPOSED_DIACRITICS_NO_SHORT_CIRCUIT, /* Always fully decomposes and then recompose back */

  HB_OT_SHAPE_NORMALIZATION_MODE_AUTO, /* See hb-ot-shape-normalize.cc for logic. */
  HB_OT_SHAPE_NORMALIZATION_MODE_DEFAULT = HB_OT_SHAPE_NORMALIZATION_MODE_AUTO
};

HB_INTERNAL ಶೂನ್ಯ _hb_ot_shape_normalize (ಧ್ರುವ hb_ot_shape_plan_t *shaper,
					 hb_buffer_t *buffer,
					 hb_font_t *font);


ಸಂರಚನಾ hb_ot_shape_normalize_context_t
{
  ಸ್ಥಿರ ತಾರ್ಕಿಕ
  decompose_unicode (ಧ್ರುವ hb_ot_shape_normalize_context_t *c,
		     hb_codepoint_t  ab,
		     hb_codepoint_t *a,
		     hb_codepoint_t *b)
  {
    ಪ್ರತ್ಯಾಗಮ (ತಾರ್ಕಿಕ) c->unicode->decompose (ab, a, b);
  }

  ಸ್ಥಿರ ತಾರ್ಕಿಕ
  compose_unicode (ಧ್ರುವ hb_ot_shape_normalize_context_t *c,
		   hb_codepoint_t  a,
		   hb_codepoint_t  b,
		   hb_codepoint_t *ab)
  {
    ಪ್ರತ್ಯಾಗಮ (ತಾರ್ಕಿಕ) c->unicode->compose (a, b, ab);
  }

  ಶೂನ್ಯ
  override_decompose_and_compose (ತಾರ್ಕಿಕ (*decompose) (ಧ್ರುವ hb_ot_shape_normalize_context_t *c,
						      hb_codepoint_t  ab,
						      hb_codepoint_t *a,
						      hb_codepoint_t *b),
				  ತಾರ್ಕಿಕ (*compose) (ಧ್ರುವ hb_ot_shape_normalize_context_t *c,
						   hb_codepoint_t  a,
						   hb_codepoint_t  b,
						   hb_codepoint_t *ab))
  {
    ಏತತ್->decompose = decompose ? decompose : decompose_unicode;
    ಏತತ್->compose = compose ? compose : compose_unicode;
  }

  ಧ್ರುವ hb_ot_shape_plan_t *plan;
  hb_buffer_t *buffer;
  hb_font_t *font;
  hb_unicode_funcs_t *unicode;
  ತಾರ್ಕಿಕ (*decompose) (ಧ್ರುವ hb_ot_shape_normalize_context_t *c,
		     hb_codepoint_t  ab,
		     hb_codepoint_t *a,
		     hb_codepoint_t *b);
  ತಾರ್ಕಿಕ (*compose) (ಧ್ರುವ hb_ot_shape_normalize_context_t *c,
		   hb_codepoint_t  a,
		   hb_codepoint_t  b,
		   hb_codepoint_t *ab);
};


#endif /* HB_OT_SHAPE_NORMALIZE_HH */
