/*
 * Copyright © 2009  Red Hat, Inc.
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
 * Red Hat Author(s): Behdad Esfahbod
 * Google Author(s): Behdad Esfahbod
 */

#إذا !defined(HB_H_وچ) && !defined(HB_NO_SINGLE_سرخی_ERROR)
#غلطی "Include <hb.h> instead."
#جے_انت

#جے_بنا_تعریف HB_SHAPE_H
#تعریف_کرو HB_SHAPE_H

#ضمن "hb-common.h"
#ضمن "hb-buffer.h"
#ضمن "hb-font.h"

HB_BEGIN_DECLS


HB_EXTERN فراغ
hb_shape (hb_font_t           *font,
	  hb_بفر_t         *بفر,
	  ثابت hb_feature_t  *features,
	  بنانشانی صحيح         عدد_features);

HB_EXTERN hb_bool_t
hb_shape_بھریا (hb_font_t          *font,
	       hb_بفر_t        *بفر,
	       ثابت hb_feature_t *features,
	       بنانشانی صحيح        عدد_features,
	       ثابت حرف * ثابت *shaper_list);

#جے_تعریف_شدہ HB_EXPERIMENTAL_API
HB_EXTERN hb_bool_t
hb_shape_justify (hb_font_t          *font,
		  hb_بفر_t        *بفر,
		  ثابت hb_feature_t *features,
		  بنانشانی صحيح        عدد_features,
		  ثابت حرف * ثابت *shaper_list,
		  عائم               min_نشانہ_advance,
		  عائم               max_نشانہ_advance,
		  عائم              *advance, /* IN/OUT */
		  hb_ٹیگ_t           *var_ٹیگ, /*باہرلا*/
		  عائم              *var_قدر /*باہرلا*/);
#جے_انت

HB_EXTERN ثابت حرف **
hb_shape_list_shapers (فراغ);


HB_سرا_DECLS

#جے_انت /* HB_SHAPE_H */
