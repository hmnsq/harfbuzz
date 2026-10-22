/*
 * Copyright © 2023  Behdad Esfahbod
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

#اگر_غیر_تعریف_شدہ HB_WASM_API_H
#تعریف_کرو HB_WASM_API_H

/*
#include "hb.h"

HB_BEGIN_DECLS
HB_END_DECLS
*/

#شامل <stdint.h>


#اگر_غیر_تعریف_شدہ HB_WASM_BEGIN_DECLS
# ifdef __cplusplus
#  define HB_WASM_BEGIN_DECLS	خارجی "C" {
#  define HB_WASM_انتہا_DECLS	}
# ورنہ /* !__cplusplus */
#  define HB_WASM_BEGIN_DECLS
#  define HB_WASM_انتہا_DECLS
# endif /* !__cplusplus */
#اگر_ختم


HB_WASM_BEGIN_DECLS

#اگر_غیر_تعریف_شدہ HB_WASM_API
#تعریف_کرو HB_WASM_API(ret_t, name) ret_t name
#اگر_ختم
#اگر_غیر_تعریف_شدہ HB_WASM_API_COMPOUND /* compound return type */
#تعریف_کرو HB_WASM_API_COMPOUND(ret_t, name) HB_WASM_API(ret_t, name)
#اگر_ختم
#اگر_غیر_تعریف_شدہ HB_WASM_INTERFACE
#تعریف_کرو HB_WASM_INTERFACE(ret_t, name) ret_t name
#اگر_ختم
#اگر_غیر_تعریف_شدہ HB_WASM_EXEC_ENV
#تعریف_کرو HB_WASM_EXEC_ENV
#اگر_ختم
#اگر_غیر_تعریف_شدہ HB_WASM_EXEC_ENV_COMPOUND
#تعریف_کرو HB_WASM_EXEC_ENV_COMPOUND HB_WASM_EXEC_ENV
#اگر_ختم


#اگر_غیر_تعریف_شدہ bool_t
#تعریف_کرو bool_t uint32_t
#اگر_ختم
#اگر_غیر_تعریف_شدہ ptr_t
#تعریف_کرو ptr_t(type_t) type_t *
#اگر_ختم
#اگر_غیر_تعریف_شدہ ptr_d
#تعریف_کرو ptr_d(type_t, name) type_t *name
#اگر_ختم

قسمتعریف uint32_t codepoint_t;
قسمتعریف int32_t position_t;
قسمتعریف uint32_t نقاب_t;
قسمتعریف uint32_t لیبل_t;
#تعریف_کرو لیبل_3(c1,c2,c3,c4) ((لیبل_t)((((uint32_t)(c1)&0xFF)<<24)|(((uint32_t)(c2)&0xFF)<<16)|(((uint32_t)(c3)&0xFF)<<8)|((uint32_t)(c4)&0xFF)))

قسمتعریف شمار {
  DIRECTION_INVALID = 0,
  DIRECTION_LTR = 4,
  DIRECTION_RTL,
  DIRECTION_TTB,
  DIRECTION_BTT
} direction_t;
#تعریف_کرو DIRECTION_IS_VALID(dir)		((((غیرعلامتی صحیح) (dir)) & ~3U) == 4)
#تعریف_کرو DIRECTION_IS_HORIZONTAL(dir)	((((غیرعلامتی صحیح) (dir)) & ~1U) == 4)
#تعریف_کرو DIRECTION_IS_VERTICAL(dir)	((((غیرعلامتی صحیح) (dir)) & ~1U) == 6)
#تعریف_کرو DIRECTION_IS_FORWARD(dir)	((((غیرعلامتی صحیح) (dir)) & ~2U) == 4)
#تعریف_کرو DIRECTION_IS_BACKWARD(dir)	((((غیرعلامتی صحیح) (dir)) & ~2U) == 5)
#تعریف_کرو DIRECTION_الٹ(dir)		((direction_t) (((غیرعلامتی صحیح) (dir)) ^ 1))

قسمتعریف لیبل_t script_t; /* ISO 15924 representation of Unicode scripts. */


/* common */

HB_WASM_API (direction_t, script_get_horizontal_direction) (HB_WASM_EXEC_ENV
							    script_t script);


/* blob */

قسمتعریف ساخت
{
  uint32_t لمبائی;
  ptr_t(حرف) ڈیٹا;
} blob_t;
#تعریف_کرو BLOB_INIT {0, 0}

HB_WASM_API (خالی, blob_free) (HB_WASM_EXEC_ENV
			       ptr_d(blob_t, blob));

/*بفر*/

قسمتعریف ساخت
{
  uint32_t codepoint;
  uint32_t نقاب;
  uint32_t cluster;
  uint32_t var1;
  uint32_t var2;
} glyph_info_t;

قسمتعریف ساخت
{
  position_t x_advance;
  position_t y_advance;
  position_t x_انحراف_2;
  position_t y_انحراف_2;
  uint32_t var;
} glyph_position_t;

قسمتعریف ساخت
{
  uint32_t لمبائی;
  ptr_t(glyph_info_t) info;
  ptr_t(glyph_position_t) pos;
} بفر_contents_t;
#تعریف_کرو بفر_CONTENTS_INIT {0, 0, 0}

HB_WASM_API (bool_t, بفر_contents_realloc) (HB_WASM_EXEC_ENV
					       ptr_d(بفر_contents_t, contents),
					       uint32_t سائز);

HB_WASM_API (خالی, بفر_contents_free) (HB_WASM_EXEC_ENV
					  ptr_d(بفر_contents_t, contents));

قسمتعریف ساخت بفر_t بفر_t;

HB_WASM_API (bool_t, بفر_copy_contents) (HB_WASM_EXEC_ENV
					    ptr_d(بفر_t, بفر),
					    ptr_d(بفر_contents_t, contents));

HB_WASM_API (bool_t, بفر_سیٹ_contents) (HB_WASM_EXEC_ENV
					   ptr_d(بفر_t, بفر),
					   ptr_d(ثابت بفر_contents_t, contents));

HB_WASM_API (direction_t, بفر_get_direction) (HB_WASM_EXEC_ENV
						 ptr_d(بفر_t, بفر));

HB_WASM_API (script_t, بفر_get_script) (HB_WASM_EXEC_ENV
					   ptr_d(بفر_t, بفر));

HB_WASM_API (خالی, بفر_الٹ) (HB_WASM_EXEC_ENV
				    ptr_d(بفر_t, بفر));

HB_WASM_API (خالی, بفر_الٹ_clusters) (HB_WASM_EXEC_ENV
					     ptr_d(بفر_t, بفر));

/* face */

قسمتعریف ساخت face_t face_t;

HB_WASM_API (ptr_t(face_t), face_create) (HB_WASM_EXEC_ENV
					  ptr_d(blob_t, blob),
					  غیرعلامتی صحیح);

HB_WASM_API (bool_t, face_copy_جدول) (HB_WASM_EXEC_ENV
				       ptr_d(face_t, face),
				       لیبل_t جدول_لیبل_2,
				       ptr_d(blob_t, blob));

HB_WASM_API (غیرعلامتی, face_get_upem) (HB_WASM_EXEC_ENV
				       ptr_d(face_t, face));

/* font */

قسمتعریف ساخت font_t font_t;

HB_WASM_API (ptr_t(font_t), font_create) (HB_WASM_EXEC_ENV
					  ptr_d(face_t, face));

HB_WASM_API (ptr_t(face_t), font_get_face) (HB_WASM_EXEC_ENV
					    ptr_d(font_t, font));

HB_WASM_API (خالی, font_get_scale) (HB_WASM_EXEC_ENV
				    ptr_d(font_t, font),
				    ptr_d(int32_t, x_scale),
				    ptr_d(int32_t, y_scale));

HB_WASM_API (codepoint_t, font_get_glyph) (HB_WASM_EXEC_ENV
					      ptr_d(font_t, font),
					      codepoint_t unicode,
					      codepoint_t variation_selector);

HB_WASM_API (position_t, font_get_glyph_h_advance) (HB_WASM_EXEC_ENV
						    ptr_d(font_t, font),
						    codepoint_t glyph);

HB_WASM_API (position_t, font_get_glyph_v_advance) (HB_WASM_EXEC_ENV
						    ptr_d(font_t, font),
						    codepoint_t glyph);

قسمتعریف ساخت
{
  position_t x_bearing;
  position_t y_bearing;
  position_t چوڑائی;
  position_t اونچائی;
} glyph_extents_t;

HB_WASM_API (bool_t, font_get_glyph_extents) (HB_WASM_EXEC_ENV
					      ptr_d(font_t, font),
					      codepoint_t glyph,
					      ptr_d(glyph_extents_t, extents));

HB_WASM_API (خالی, font_glyph_مقصد_string) (HB_WASM_EXEC_ENV
					  ptr_d(font_t, font),
					  codepoint_t glyph,
					  ptr_d(حرف, s), uint32_t سائز);


قسمتعریف ساخت
{
  غیرعلامتی صحیح لمبائی;
  ptr_t(صحیح) coords;
} coords_t;

HB_WASM_API (bool_t, font_copy_coords) (HB_WASM_EXEC_ENV
					  ptr_d(font_t, font),
					  ptr_d(coords_t, coords));

HB_WASM_API (bool_t, font_سیٹ_coords) (HB_WASM_EXEC_ENV
					  ptr_d(font_t, font),
					  ptr_d(coords_t, coords));

/* outline */

شمار glyph_outline_نقطہ_type_t
{
  MOVE_مقصد,
  سطر_مقصد_2,
  QUADRATIC_مقصد,
  CUBIC_مقصد,
};

قسمتعریف ساخت
{
  اعشاری x;
  اعشاری y;
  uint32_t type;
} glyph_outline_نقطہ_t;

قسمتعریف ساخت
{
  uint32_t n_points;
  ptr_t(glyph_outline_نقطہ_t) points;
  uint32_t n_contours;
  ptr_t(uint32_t) contours;
} glyph_outline_t;
#تعریف_کرو GLYPH_OUTLINE_INIT {0, 0, 0, 0}

HB_WASM_API (خالی, glyph_outline_free) (HB_WASM_EXEC_ENV
					ptr_d(glyph_outline_t, outline));

HB_WASM_API (bool_t, font_copy_glyph_outline) (HB_WASM_EXEC_ENV
					       ptr_d(font_t, font),
					       codepoint_t glyph,
					       ptr_d(glyph_outline_t, outline));


/* shape */

قسمتعریف ساخت
{
  لیبل_t    لیبل_2;
  uint32_t قدر_2;
  uint32_t آغاز;
  uint32_t انتہا;
} feature_t;
#تعریف_کرو FEATURE_GLOBAL_آغاز	0
#تعریف_کرو FEATURE_GLOBAL_انتہا	((uint32_t) -1)

قسمتعریف ساخت
{
  uint32_t لمبائی;
  ptr_t(feature_t) features;
} features_t;
#تعریف_کرو FEATURES_INIT {0, 0}

HB_WASM_API (bool_t, feature_copy_contents) (HB_WASM_EXEC_ENV
					       ptr_d(ثابت feature_t, features),
					       uint32_t عدد_features,
					       ptr_d(features_t, output));

HB_WASM_API (خالی, features_free) (HB_WASM_EXEC_ENV
				     ptr_d(features_t, features));

HB_WASM_API (bool_t, shape_with) (HB_WASM_EXEC_ENV
				  ptr_d(font_t, font),
				  ptr_d(بفر_t, بفر),
				  ptr_d(ثابت feature_t, features),
				  uint32_t عدد_features,
				  ثابت حرف *shaper);

/* Implement these in your shaper. */

HB_WASM_INTERFACE (ptr_t(خالی), shape_plan_create) (ptr_d(face_t, face));

HB_WASM_INTERFACE (bool_t, shape) (ptr_d(خالی, shape_plan),
				   ptr_d(font_t, font),
				   ptr_d(بفر_t, بفر),
				   ptr_d(ثابت feature_t, features),
				   uint32_t عدد_features);

HB_WASM_INTERFACE (خالی, shape_plan_destroy) (ptr_d(خالی, shape_plan));


HB_WASM_انتہا_DECLS

#اگر_ختم /* HB_WASM_API_H */
