/*
 * Copyright (C) 2011  Google, Inc.
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

#ਜੇ !defined(HB_GOBJECT_H_ਵਿੱਚ) && !defined(HB_NO_SINGLE_ਸਿਰਲੇਖ_ERROR)
#ਗਲਤੀ "Include <hb-gobject.h> instead."
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_GOBJECT_STRUCTS_H
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_GOBJECT_STRUCTS_H

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.h"

#ਅਨ੍ਤਰ੍ਭਾਵ <glib-object.h>

HB_BEGIN_DECLS


/* Object types */

HB_EXTERN GType
hb_gobject_blob_get_type (ਖਾਲੀ);
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_GOBJECT_TYPE_BLOB (hb_gobject_blob_get_type ())

HB_EXTERN GType
hb_gobject_ਬਫਰ_get_type (ਖਾਲੀ);
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_GOBJECT_TYPE_ਬਫਰ (hb_gobject_ਬਫਰ_get_type ())

HB_EXTERN GType
hb_gobject_draw_funcs_get_type (ਖਾਲੀ);
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_GOBJECT_TYPE_DRAW_FUNCS (hb_gobject_draw_funcs_get_type ())

HB_EXTERN GType
hb_gobject_paint_funcs_get_type (ਖਾਲੀ);
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_GOBJECT_TYPE_PAINT_FUNCS (hb_gobject_paint_funcs_get_type ())

HB_EXTERN GType
hb_gobject_face_get_type (ਖਾਲੀ);
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_GOBJECT_TYPE_FACE (hb_gobject_face_get_type ())

HB_EXTERN GType
hb_gobject_font_get_type (ਖਾਲੀ);
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_GOBJECT_TYPE_FONT (hb_gobject_font_get_type ())

HB_EXTERN GType
hb_gobject_font_funcs_get_type (ਖਾਲੀ);
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_GOBJECT_TYPE_FONT_FUNCS (hb_gobject_font_funcs_get_type ())

HB_EXTERN GType
hb_gobject_ਸੈੱਟ_get_type (ਖਾਲੀ);
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_GOBJECT_TYPE_ਸੈੱਟ (hb_gobject_ਸੈੱਟ_get_type ())

HB_EXTERN GType
hb_gobject_map_get_type (ਖਾਲੀ);
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_GOBJECT_TYPE_MAP (hb_gobject_map_get_type ())

HB_EXTERN GType
hb_gobject_shape_plan_get_type (ਖਾਲੀ);
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_GOBJECT_TYPE_SHAPE_PLAN (hb_gobject_shape_plan_get_type ())

HB_EXTERN GType
hb_gobject_unicode_funcs_get_type (ਖਾਲੀ);
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_GOBJECT_TYPE_UNICODE_FUNCS (hb_gobject_unicode_funcs_get_type ())

/* Value types */

HB_EXTERN GType
hb_gobject_feature_get_type (ਖਾਲੀ);
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_GOBJECT_TYPE_FEATURE (hb_gobject_feature_get_type ())

HB_EXTERN GType
hb_gobject_glyph_info_get_type (ਖਾਲੀ);
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_GOBJECT_TYPE_GLYPH_INFO (hb_gobject_glyph_info_get_type ())

HB_EXTERN GType
hb_gobject_glyph_position_get_type (ਖਾਲੀ);
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_GOBJECT_TYPE_GLYPH_POSITION (hb_gobject_glyph_position_get_type ())

HB_EXTERN GType
hb_gobject_segment_properties_get_type (ਖਾਲੀ);
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_GOBJECT_TYPE_SEGMENT_PROPERTIES (hb_gobject_segment_properties_get_type ())

HB_EXTERN GType
hb_gobject_draw_state_get_type (ਖਾਲੀ);
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_GOBJECT_TYPE_DRAW_STATE (hb_gobject_draw_state_get_type ())

HB_EXTERN GType
hb_gobject_color_stop_get_type (ਖਾਲੀ);
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_GOBJECT_TYPE_COLOR_STOP (hb_gobject_color_stop_get_type ())

HB_EXTERN GType
hb_gobject_color_ਸਤਰ_get_type (ਖਾਲੀ);
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_GOBJECT_TYPE_COLOR_ਸਤਰ (hb_gobject_color_ਸਤਰ_get_type ())

HB_EXTERN GType
hb_gobject_user_ਅੰਕੜੇ_ਕੁੰਜੀ_get_type (ਖਾਲੀ);
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_GOBJECT_TYPE_USER_ਅੰਕੜੇ_ਕੁੰਜੀ (hb_gobject_user_ਅੰਕੜੇ_ਕੁੰਜੀ_get_type ())

HB_EXTERN GType
hb_gobject_ot_var_axis_info_get_type (ਖਾਲੀ);
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_GOBJECT_TYPE_OT_VAR_AXIS_INFO (hb_gobject_ot_var_axis_info_get_type ())

HB_EXTERN GType
hb_gobject_ot_math_glyph_variant_get_type (ਖਾਲੀ);
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_GOBJECT_TYPE_OT_MATH_GLYPH_VARIANT (hb_gobject_ot_math_glyph_variant_get_type ())

HB_EXTERN GType
hb_gobject_ot_math_glyph_part_get_type (ਖਾਲੀ);
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_GOBJECT_TYPE_OT_MATH_GLYPH_PART (hb_gobject_ot_math_glyph_part_get_type ())


HB_ਸਿਰਾ_DECLS

#ਜੇ_ਅੰਤ /* HB_GOBJECT_H */
