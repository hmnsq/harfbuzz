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

#నిర్వచితం_కాకపోతే HB_OT_SHAPE_NORMALIZE_HH
#నిర్వచించు HB_OT_SHAPE_NORMALIZE_HH

#అన్తర్భావ "hb.hh"
#అన్తర్భావ "hb-unicode.hh"


/* buffer var allocations, used during the normalization process */
#నిర్వచించు normalizer_glyph_సూచీ()	var1.u32

నిర్మాణం hb_ot_shape_plan_t;

గణన hb_ot_shape_normalization_బహుళకం_t {
  HB_OT_SHAPE_NORMALIZATION_బహుళకం_NONE,
  HB_OT_SHAPE_NORMALIZATION_బహుళకం_DECOMPOSED,
  HB_OT_SHAPE_NORMALIZATION_బహుళకం_COMPOSED_DIACRITICS, /* Never composes base-to-base */
  HB_OT_SHAPE_NORMALIZATION_బహుళకం_COMPOSED_DIACRITICS_NO_SHORT_CIRCUIT, /* Always fully decomposes and then recompose back */

  HB_OT_SHAPE_NORMALIZATION_బహుళకం_AUTO, /* See hb-ot-shape-normalize.cc for logic. */
  HB_OT_SHAPE_NORMALIZATION_బహుళకం_DEFAULT = HB_OT_SHAPE_NORMALIZATION_బహుళకం_AUTO
};

HB_అంతర్గత శూన్య _hb_ot_shape_normalize (స్థిరరాశి hb_ot_shape_plan_t *shaper,
					 hb_బఫరు_t *బఫరు,
					 hb_font_t *font);


నిర్మాణం hb_ot_shape_normalize_context_t
{
  స్థిర తార్కిక
  decompose_unicode (స్థిరరాశి hb_ot_shape_normalize_context_t *c,
		     hb_codepoint_t  ab,
		     hb_codepoint_t *a,
		     hb_codepoint_t *b)
  {
    తిరిగి (తార్కిక) c->unicode->decompose (ab, a, b);
  }

  స్థిర తార్కిక
  compose_unicode (స్థిరరాశి hb_ot_shape_normalize_context_t *c,
		   hb_codepoint_t  a,
		   hb_codepoint_t  b,
		   hb_codepoint_t *ab)
  {
    తిరిగి (తార్కిక) c->unicode->compose (a, b, ab);
  }

  శూన్య
  override_decompose_and_compose (తార్కిక (*decompose) (స్థిరరాశి hb_ot_shape_normalize_context_t *c,
						      hb_codepoint_t  ab,
						      hb_codepoint_t *a,
						      hb_codepoint_t *b),
				  తార్కిక (*compose) (స్థిరరాశి hb_ot_shape_normalize_context_t *c,
						   hb_codepoint_t  a,
						   hb_codepoint_t  b,
						   hb_codepoint_t *ab))
  {
    ఇది->decompose = decompose ? decompose : decompose_unicode;
    ఇది->compose = compose ? compose : compose_unicode;
  }

  స్థిరరాశి hb_ot_shape_plan_t *plan;
  hb_బఫరు_t *బఫరు;
  hb_font_t *font;
  hb_unicode_funcs_t *unicode;
  తార్కిక (*decompose) (స్థిరరాశి hb_ot_shape_normalize_context_t *c,
		     hb_codepoint_t  ab,
		     hb_codepoint_t *a,
		     hb_codepoint_t *b);
  తార్కిక (*compose) (స్థిరరాశి hb_ot_shape_normalize_context_t *c,
		   hb_codepoint_t  a,
		   hb_codepoint_t  b,
		   hb_codepoint_t *ab);
};


#అయితే_ముగింపు /* HB_OT_SHAPE_NORMALIZE_HH */
