/*
 * Copyright © 2021  Behdad Esfahbod.
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

#अगर_अघोषित HB_OT_SHAPER_SYLLABIC_HH
#घोषणा HB_OT_SHAPER_SYLLABIC_HH

#समावेश "hb.hh"

#समावेश "hb-ot-shaper.hh"


HB_INTERNAL bool
hb_syllabic_जोड़ो_dotted_circles (hb_font_t *font,
				   hb_बफर_t *बफर,
				   अचिन्हित पूर्णांक broken_syllable_type,
				   अचिन्हित पूर्णांक dottedcircle_category,
				   पूर्णांक repha_category = -1,
				   पूर्णांक dottedcircle_position = -1);

HB_INTERNAL bool
hb_syllabic_clear_var (स्थिर hb_ot_shape_plan_t *plan,
		       hb_font_t *font,
		       hb_बफर_t *बफर);


#पूर्ण_अगर /* HB_OT_SHAPER_SYLLABIC_HH */
