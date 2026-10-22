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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_OT_SHAPE_NORMALIZE_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_SHAPE_NORMALIZE_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-unicode.hh"


/* buffer var allocations, used during the normalization process */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ normalizer_glyph_ਸੂਚਕਾਂਕ()	var1.u32

ਬਣਤਰ hb_ot_shape_plan_t;

ਗਿਣਤੀ hb_ot_shape_normalization_ਬਹੁਲਕ_t {
  HB_OT_SHAPE_NORMALIZATION_ਬਹੁਲਕ_NONE,
  HB_OT_SHAPE_NORMALIZATION_ਬਹੁਲਕ_DECOMPOSED,
  HB_OT_SHAPE_NORMALIZATION_ਬਹੁਲਕ_COMPOSED_DIACRITICS, /* Never composes base-to-base */
  HB_OT_SHAPE_NORMALIZATION_ਬਹੁਲਕ_COMPOSED_DIACRITICS_NO_SHORT_CIRCUIT, /* Always fully decomposes and then recompose back */

  HB_OT_SHAPE_NORMALIZATION_ਬਹੁਲਕ_AUTO, /* See hb-ot-shape-normalize.cc for logic. */
  HB_OT_SHAPE_NORMALIZATION_ਬਹੁਲਕ_DEFAULT = HB_OT_SHAPE_NORMALIZATION_ਬਹੁਲਕ_AUTO
};

HB_ਅੰਦਰੂਨੀ ਖਾਲੀ _hb_ot_shape_normalize (ਅਚਲ hb_ot_shape_plan_t *shaper,
					 hb_ਬਫਰ_t *ਬਫਰ,
					 hb_font_t *font);


ਬਣਤਰ hb_ot_shape_normalize_context_t
{
  ਸਥਿਰ ਤਾਰਕਿਕ
  decompose_unicode (ਅਚਲ hb_ot_shape_normalize_context_t *c,
		     hb_codepoint_t  ab,
		     hb_codepoint_t *a,
		     hb_codepoint_t *b)
  {
    ਵਾਪਸ (ਤਾਰਕਿਕ) c->unicode->decompose (ab, a, b);
  }

  ਸਥਿਰ ਤਾਰਕਿਕ
  compose_unicode (ਅਚਲ hb_ot_shape_normalize_context_t *c,
		   hb_codepoint_t  a,
		   hb_codepoint_t  b,
		   hb_codepoint_t *ab)
  {
    ਵਾਪਸ (ਤਾਰਕਿਕ) c->unicode->compose (a, b, ab);
  }

  ਖਾਲੀ
  override_decompose_and_compose (ਤਾਰਕਿਕ (*decompose) (ਅਚਲ hb_ot_shape_normalize_context_t *c,
						      hb_codepoint_t  ab,
						      hb_codepoint_t *a,
						      hb_codepoint_t *b),
				  ਤਾਰਕਿਕ (*compose) (ਅਚਲ hb_ot_shape_normalize_context_t *c,
						   hb_codepoint_t  a,
						   hb_codepoint_t  b,
						   hb_codepoint_t *ab))
  {
    ਇਹ->decompose = decompose ? decompose : decompose_unicode;
    ਇਹ->compose = compose ? compose : compose_unicode;
  }

  ਅਚਲ hb_ot_shape_plan_t *plan;
  hb_ਬਫਰ_t *ਬਫਰ;
  hb_font_t *font;
  hb_unicode_funcs_t *unicode;
  ਤਾਰਕਿਕ (*decompose) (ਅਚਲ hb_ot_shape_normalize_context_t *c,
		     hb_codepoint_t  ab,
		     hb_codepoint_t *a,
		     hb_codepoint_t *b);
  ਤਾਰਕਿਕ (*compose) (ਅਚਲ hb_ot_shape_normalize_context_t *c,
		   hb_codepoint_t  a,
		   hb_codepoint_t  b,
		   hb_codepoint_t *ab);
};


#ਜੇ_ਅੰਤ /* HB_OT_SHAPE_NORMALIZE_HH */
