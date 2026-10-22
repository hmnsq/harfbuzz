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

#ਜੇ !defined(HB_H_ਵਿੱਚ) && !defined(HB_NO_SINGLE_ਸਿਰਲੇਖ_ERROR)
#ਗਲਤੀ "Include <hb.h> instead."
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_SHAPE_H
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_SHAPE_H

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-common.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-buffer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-font.h"

HB_BEGIN_DECLS


HB_EXTERN ਖਾਲੀ
hb_shape (hb_font_t           *font,
	  hb_ਬਫਰ_t         *ਬਫਰ,
	  ਅਚਲ hb_feature_t  *features,
	  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ         ਅੰਕ_features);

HB_EXTERN hb_bool_t
hb_shape_ਭਰਿਆ (hb_font_t          *font,
	       hb_ਬਫਰ_t        *ਬਫਰ,
	       ਅਚਲ hb_feature_t *features,
	       ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ        ਅੰਕ_features,
	       ਅਚਲ ਅੱਖਰ * ਅਚਲ *shaper_list);

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HB_EXPERIMENTAL_API
HB_EXTERN hb_bool_t
hb_shape_justify (hb_font_t          *font,
		  hb_ਬਫਰ_t        *ਬਫਰ,
		  ਅਚਲ hb_feature_t *features,
		  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ        ਅੰਕ_features,
		  ਅਚਲ ਅੱਖਰ * ਅਚਲ *shaper_list,
		  ਦਸ਼ਮਲਵ               min_ਨਿਸ਼ਾਨਾ_advance,
		  ਦਸ਼ਮਲਵ               max_ਨਿਸ਼ਾਨਾ_advance,
		  ਦਸ਼ਮਲਵ              *advance, /* IN/OUT */
		  hb_ਟੈਗ_t           *var_ਟੈਗ, /*ਬਾਹਰਲਾ*/
		  ਦਸ਼ਮਲਵ              *var_ਮੁੱਲ /*ਬਾਹਰਲਾ*/);
#ਜੇ_ਅੰਤ

HB_EXTERN ਅਚਲ ਅੱਖਰ **
hb_shape_list_shapers (ਖਾਲੀ);


HB_ਸਿਰਾ_DECLS

#ਜੇ_ਅੰਤ /* HB_SHAPE_H */
