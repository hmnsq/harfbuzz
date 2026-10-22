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

#إذا !defined(HB_OT_H_اندريون) && !defined(HB_NO_SINGLE_سرو_ERROR)
#غلطي "Include <hb-ot.h> instead."
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل HB_OT_SHAPE_H
#وصف_ڪريو HB_OT_SHAPE_H

#ضمن "hb.h"

HB_BEGIN_DECLS

/* TODO port to shape-plan / set. */
HB_EXTERN فراغ
hb_ot_shape_glyphs_گهيرو (hb_font_t          *font,
			    hb_بفر_t        *بفر,
			    ثابت hb_feature_t *features,
			    بنانشان صحيح        انگ_features,
			    hb_سيٽ_t           *glyphs);

HB_EXTERN فراغ
hb_ot_shape_plan_collect_lookups (hb_shape_plan_t *shape_plan,
				  hb_ٽيگ_t         جدول_ٽيگ_2,
				  hb_سيٽ_t        *lookup_indexes /*ٻاهريون*/);

HB_EXTERN بنانشان صحيح
hb_ot_shape_plan_get_feature_ٽيگ_فهرست (hb_shape_plan_t *shape_plan,
				   بنانشان صحيح     شروعات_وٿي,
				   بنانشان صحيح    *ٽيگ_ڳڻپ, /* IN/OUT */
				   hb_ٽيگ_t        *ٽيگ_فهرست /*ٻاهريون*/);

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
#وصف_ڪريو HB_OT_SHAPE_بفر_FORMAT_SERIAL 1

HB_EXTERN بنانشان صحيح
hb_ot_shape_get_بفر_format_serial (فراغ);

HB_پڇاڙي_DECLS

#جيڪڏهن_پورو /* HB_OT_SHAPE_H */
