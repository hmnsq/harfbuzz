/*
 * Copyright © 2013  Red Hat, Inc.
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
 */

#ਜੇ !defined(HB_OT_H_ਵਿੱਚ) && !defined(HB_NO_SINGLE_ਸਿਰਲੇਖ_ERROR)
#ਗਲਤੀ "Include <hb-ot.h> instead."
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_OT_SHAPE_H
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_SHAPE_H

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.h"

HB_BEGIN_DECLS

/* TODO port to shape-plan / set. */
HB_EXTERN ਖਾਲੀ
hb_ot_shape_glyphs_ਘੇਰਾ (hb_font_t          *font,
			    hb_ਬਫਰ_t        *ਬਫਰ,
			    ਅਚਲ hb_feature_t *features,
			    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ        ਅੰਕ_features,
			    hb_ਸੈੱਟ_t           *glyphs);

HB_EXTERN ਖਾਲੀ
hb_ot_shape_plan_collect_lookups (hb_shape_plan_t *shape_plan,
				  hb_ਟੈਗ_t         ਸਾਰਣੀ_ਟੈਗ_2,
				  hb_ਸੈੱਟ_t        *lookup_indexes /*ਬਾਹਰਲਾ*/);

HB_EXTERN ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ
hb_ot_shape_plan_get_feature_ਟੈਗ_ਸੂਚੀ (hb_shape_plan_t *shape_plan,
				   ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ     ਸ਼ੁਰੂ_ਵਿੱਥ,
				   ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ    *ਟੈਗ_ਗਿਣਤੀ, /* IN/OUT */
				   hb_ਟੈਗ_t        *ਟੈਗ_ਸੂਚੀ /*ਬਾਹਰਲਾ*/);

/**
 * HB_OT_SHAPE_BUFFER_FORMAT_SERIAL:
 *
 * The serial number of the current internal buffer format.
 *
 * The serial number will increase when internal #hb_glyph_info_t and
 * #hb_glyph_position_t members change their format.
 *
 * Since: 13.2.0
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_SHAPE_ਬਫਰ_FORMAT_SERIAL 1

HB_EXTERN ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ
hb_ot_shape_get_ਬਫਰ_format_serial (ਖਾਲੀ);

HB_ਸਿਰਾ_DECLS

#ਜੇ_ਅੰਤ /* HB_OT_SHAPE_H */
