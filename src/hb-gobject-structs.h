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

#अगर !defined(HB_GOBJECT_H_IN) && !defined(HB_NO_SINGLE_HEADER_ERROR)
#त्रुटि "Include <hb-gobject.h> instead."
#पूर्ण_अगर

#अगर_अघोषित HB_GOBJECT_STRUCTS_H
#घोषणा HB_GOBJECT_STRUCTS_H

#समावेश "hb.h"

#समावेश <glib-object.h>

HB_BEGIN_DECLS


/* Object types */

HB_EXTERN GType
hb_gobject_blob_get_type (व्योम);
#घोषणा HB_GOBJECT_TYPE_BLOB (hb_gobject_blob_get_type ())

HB_EXTERN GType
hb_gobject_बफर_get_type (व्योम);
#घोषणा HB_GOBJECT_TYPE_बफर (hb_gobject_बफर_get_type ())

HB_EXTERN GType
hb_gobject_draw_funcs_get_type (व्योम);
#घोषणा HB_GOBJECT_TYPE_DRAW_FUNCS (hb_gobject_draw_funcs_get_type ())

HB_EXTERN GType
hb_gobject_paint_funcs_get_type (व्योम);
#घोषणा HB_GOBJECT_TYPE_PAINT_FUNCS (hb_gobject_paint_funcs_get_type ())

HB_EXTERN GType
hb_gobject_face_get_type (व्योम);
#घोषणा HB_GOBJECT_TYPE_FACE (hb_gobject_face_get_type ())

HB_EXTERN GType
hb_gobject_font_get_type (व्योम);
#घोषणा HB_GOBJECT_TYPE_FONT (hb_gobject_font_get_type ())

HB_EXTERN GType
hb_gobject_font_funcs_get_type (व्योम);
#घोषणा HB_GOBJECT_TYPE_FONT_FUNCS (hb_gobject_font_funcs_get_type ())

HB_EXTERN GType
hb_gobject_set_get_type (व्योम);
#घोषणा HB_GOBJECT_TYPE_SET (hb_gobject_set_get_type ())

HB_EXTERN GType
hb_gobject_map_get_type (व्योम);
#घोषणा HB_GOBJECT_TYPE_MAP (hb_gobject_map_get_type ())

HB_EXTERN GType
hb_gobject_shape_plan_get_type (व्योम);
#घोषणा HB_GOBJECT_TYPE_SHAPE_PLAN (hb_gobject_shape_plan_get_type ())

HB_EXTERN GType
hb_gobject_unicode_funcs_get_type (व्योम);
#घोषणा HB_GOBJECT_TYPE_UNICODE_FUNCS (hb_gobject_unicode_funcs_get_type ())

/* Value types */

HB_EXTERN GType
hb_gobject_feature_get_type (व्योम);
#घोषणा HB_GOBJECT_TYPE_FEATURE (hb_gobject_feature_get_type ())

HB_EXTERN GType
hb_gobject_glyph_info_get_type (व्योम);
#घोषणा HB_GOBJECT_TYPE_GLYPH_INFO (hb_gobject_glyph_info_get_type ())

HB_EXTERN GType
hb_gobject_glyph_position_get_type (व्योम);
#घोषणा HB_GOBJECT_TYPE_GLYPH_POSITION (hb_gobject_glyph_position_get_type ())

HB_EXTERN GType
hb_gobject_segment_properties_get_type (व्योम);
#घोषणा HB_GOBJECT_TYPE_SEGMENT_PROPERTIES (hb_gobject_segment_properties_get_type ())

HB_EXTERN GType
hb_gobject_draw_state_get_type (व्योम);
#घोषणा HB_GOBJECT_TYPE_DRAW_STATE (hb_gobject_draw_state_get_type ())

HB_EXTERN GType
hb_gobject_color_stop_get_type (व्योम);
#घोषणा HB_GOBJECT_TYPE_COLOR_STOP (hb_gobject_color_stop_get_type ())

HB_EXTERN GType
hb_gobject_color_पंक्ति_get_type (व्योम);
#घोषणा HB_GOBJECT_TYPE_COLOR_पंक्ति (hb_gobject_color_पंक्ति_get_type ())

HB_EXTERN GType
hb_gobject_user_तथ्य_कुंजी_get_type (व्योम);
#घोषणा HB_GOBJECT_TYPE_USER_तथ्य_कुंजी (hb_gobject_user_तथ्य_कुंजी_get_type ())

HB_EXTERN GType
hb_gobject_ot_var_axis_info_get_type (व्योम);
#घोषणा HB_GOBJECT_TYPE_OT_VAR_AXIS_INFO (hb_gobject_ot_var_axis_info_get_type ())

HB_EXTERN GType
hb_gobject_ot_math_glyph_variant_get_type (व्योम);
#घोषणा HB_GOBJECT_TYPE_OT_MATH_GLYPH_VARIANT (hb_gobject_ot_math_glyph_variant_get_type ())

HB_EXTERN GType
hb_gobject_ot_math_glyph_part_get_type (व्योम);
#घोषणा HB_GOBJECT_TYPE_OT_MATH_GLYPH_PART (hb_gobject_ot_math_glyph_part_get_type ())


HB_END_DECLS

#पूर्ण_अगर /* HB_GOBJECT_H */
