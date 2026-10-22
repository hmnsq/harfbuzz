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

#إذا !defined(HB_OT_H_دننه) && !defined(HB_NO_SINGLE_سرلیک_ERROR)
#تېروتنه "Include <hb-ot.h> instead."
#د_که_پای

#که_نه_تعریف_شوی HB_OT_SHAPE_H
#تعریف_کړه HB_OT_SHAPE_H

#ضمن "hb.h"

HB_BEGIN_DECLS

/* TODO port to shape-plan / set. */
HB_EXTERN فراغ
hb_ot_shape_glyphs_تړون (hb_font_t          *font,
			    hb_بفر_t        *بفر,
			    ثابت hb_feature_t *features,
			    بینښه صحيح        عدد_features,
			    hb_سېټ_t           *glyphs);

HB_EXTERN فراغ
hb_ot_shape_plan_collect_lookups (hb_shape_plan_t *shape_plan,
				  hb_نښان_t         جدول_نښان_2,
				  hb_سېټ_t        *lookup_indexes /*بهر*/);

HB_EXTERN بینښه صحيح
hb_ot_shape_plan_get_feature_نښانونه (hb_shape_plan_t *shape_plan,
				   بینښه صحيح     پیل_دننه_واټن,
				   بینښه صحيح    *نښان_شمېر, /* IN/OUT */
				   hb_نښان_t        *نښانونه /*بهر*/);

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
#تعریف_کړه HB_OT_SHAPE_بفر_FORMAT_SERIAL 1

HB_EXTERN بینښه صحيح
hb_ot_shape_get_بفر_format_serial (فراغ);

HB_پای_ټکی_DECLS

#د_که_پای /* HB_OT_SHAPE_H */
