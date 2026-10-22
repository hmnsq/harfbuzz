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

#اگر !defined(HB_OT_H_اندر) && !defined(HB_NO_SINGLE_سرخی_ERROR)
#غلطی "Include <hb-ot.h> instead."
#اگر_ختم

#اگر_غیر_تعریف_شدہ HB_OT_SHAPE_H
#تعریف_کرو HB_OT_SHAPE_H

#شامل "hb.h"

HB_BEGIN_DECLS

/* TODO port to shape-plan / set. */
HB_EXTERN خالی
hb_ot_shape_glyphs_احاطہ (hb_font_t          *font,
			    hb_بفر_t        *بفر,
			    ثابت hb_feature_t *features,
			    غیرعلامتی صحیح        عدد_features,
			    hb_سیٹ_t           *glyphs);

HB_EXTERN خالی
hb_ot_shape_plan_collect_lookups (hb_shape_plan_t *shape_plan,
				  hb_لیبل_t         جدول_لیبل_2,
				  hb_سیٹ_t        *lookup_indexes /*برآمد*/);

HB_EXTERN غیرعلامتی صحیح
hb_ot_shape_plan_get_feature_لیبل_فہرست (hb_shape_plan_t *shape_plan,
				   غیرعلامتی صحیح     آغاز_انحراف,
				   غیرعلامتی صحیح    *لیبل_گنتی, /* IN/OUT */
				   hb_لیبل_t        *لیبل_فہرست /*برآمد*/);

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
#تعریف_کرو HB_OT_SHAPE_بفر_FORMAT_SERIAL 1

HB_EXTERN غیرعلامتی صحیح
hb_ot_shape_get_بفر_format_serial (خالی);

HB_انتہا_DECLS

#اگر_ختم /* HB_OT_SHAPE_H */
