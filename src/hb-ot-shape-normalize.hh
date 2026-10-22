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

#جے_بنا_تعریف HB_OT_SHAPE_NORMALIZE_HH
#تعریف_کرو HB_OT_SHAPE_NORMALIZE_HH

#ضمن "hb.hh"
#ضمن "hb-unicode.hh"


/* buffer var allocations, used during the normalization process */
#تعریف_کرو normalizer_glyph_اشاریہ()	var1.u32

بنية hb_ot_shape_plan_t;

تعداد hb_ot_shape_normalization_منوال_t {
  HB_OT_SHAPE_NORMALIZATION_منوال_NONE,
  HB_OT_SHAPE_NORMALIZATION_منوال_DECOMPOSED,
  HB_OT_SHAPE_NORMALIZATION_منوال_COMPOSED_DIACRITICS, /* Never composes base-to-base */
  HB_OT_SHAPE_NORMALIZATION_منوال_COMPOSED_DIACRITICS_NO_SHORT_CIRCUIT, /* Always fully decomposes and then recompose back */

  HB_OT_SHAPE_NORMALIZATION_منوال_AUTO, /* See hb-ot-shape-normalize.cc for logic. */
  HB_OT_SHAPE_NORMALIZATION_منوال_DEFAULT = HB_OT_SHAPE_NORMALIZATION_منوال_AUTO
};

HB_اندرونی فراغ _hb_ot_shape_normalize (ثابت hb_ot_shape_plan_t *shaper,
					 hb_بفر_t *بفر,
					 hb_font_t *font);


بنية hb_ot_shape_normalize_context_t
{
  ساكن منطقي
  decompose_unicode (ثابت hb_ot_shape_normalize_context_t *c,
		     hb_codepoint_t  ab,
		     hb_codepoint_t *a,
		     hb_codepoint_t *b)
  {
    أرجع (منطقي) c->unicode->decompose (ab, a, b);
  }

  ساكن منطقي
  compose_unicode (ثابت hb_ot_shape_normalize_context_t *c,
		   hb_codepoint_t  a,
		   hb_codepoint_t  b,
		   hb_codepoint_t *ab)
  {
    أرجع (منطقي) c->unicode->compose (a, b, ab);
  }

  فراغ
  override_decompose_and_compose (منطقي (*decompose) (ثابت hb_ot_shape_normalize_context_t *c,
						      hb_codepoint_t  ab,
						      hb_codepoint_t *a,
						      hb_codepoint_t *b),
				  منطقي (*compose) (ثابت hb_ot_shape_normalize_context_t *c,
						   hb_codepoint_t  a,
						   hb_codepoint_t  b,
						   hb_codepoint_t *ab))
  {
    هذا->decompose = decompose ? decompose : decompose_unicode;
    هذا->compose = compose ? compose : compose_unicode;
  }

  ثابت hb_ot_shape_plan_t *plan;
  hb_بفر_t *بفر;
  hb_font_t *font;
  hb_unicode_funcs_t *unicode;
  منطقي (*decompose) (ثابت hb_ot_shape_normalize_context_t *c,
		     hb_codepoint_t  ab,
		     hb_codepoint_t *a,
		     hb_codepoint_t *b);
  منطقي (*compose) (ثابت hb_ot_shape_normalize_context_t *c,
		   hb_codepoint_t  a,
		   hb_codepoint_t  b,
		   hb_codepoint_t *ab);
};


#جے_انت /* HB_OT_SHAPE_NORMALIZE_HH */
