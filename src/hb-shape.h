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

#إذا !defined(HB_H_اندريون) && !defined(HB_NO_SINGLE_سرو_ERROR)
#غلطي "Include <hb.h> instead."
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل HB_SHAPE_H
#وصف_ڪريو HB_SHAPE_H

#ضمن "hb-common.h"
#ضمن "hb-buffer.h"
#ضمن "hb-font.h"

HB_BEGIN_DECLS


HB_EXTERN فراغ
hb_shape (hb_font_t           *font,
	  hb_بفر_t         *بفر,
	  ثابت hb_feature_t  *features,
	  بنانشان صحيح         انگ_features);

HB_EXTERN hb_bool_t
hb_shape_ڀريل (hb_font_t          *font,
	       hb_بفر_t        *بفر,
	       ثابت hb_feature_t *features,
	       بنانشان صحيح        انگ_features,
	       ثابت حرف * ثابت *shaper_list);

#جيڪڏهن_وصف_ٿيل HB_EXPERIMENTAL_API
HB_EXTERN hb_bool_t
hb_shape_justify (hb_font_t          *font,
		  hb_بفر_t        *بفر,
		  ثابت hb_feature_t *features,
		  بنانشان صحيح        انگ_features,
		  ثابت حرف * ثابت *shaper_list,
		  عائم               min_هدف_advance,
		  عائم               max_هدف_advance,
		  عائم              *advance, /* IN/OUT */
		  hb_ٽيگ_t           *var_ٽيگ, /*ٻاهريون*/
		  عائم              *var_قدر /*ٻاهريون*/);
#جيڪڏهن_پورو

HB_EXTERN ثابت حرف **
hb_shape_list_shapers (فراغ);


HB_پڇاڙي_DECLS

#جيڪڏهن_پورو /* HB_SHAPE_H */
