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

#اگر_تعریف_نشده HB_WASM_API_H
#تعریف_کن HB_WASM_API_H

/*
#include "hb.h"

HB_BEGIN_DECLS
HB_END_DECLS
*/

#ضمن <stdint.h>


#اگر_تعریف_نشده HB_WASM_BEGIN_DECLS
# ifdef __cplusplus
#  define HB_WASM_BEGIN_DECLS	خارجي "C" {
#  define HB_WASM_انتها_DECLS	}
# وإلا /* !__cplusplus */
#  define HB_WASM_BEGIN_DECLS
#  define HB_WASM_انتها_DECLS
# endif /* !__cplusplus */
#پایان_اگر


HB_WASM_BEGIN_DECLS

#اگر_تعریف_نشده HB_WASM_API
#تعریف_کن HB_WASM_API(ret_t, name) ret_t name
#پایان_اگر
#اگر_تعریف_نشده HB_WASM_API_COMPOUND /* compound return type */
#تعریف_کن HB_WASM_API_COMPOUND(ret_t, name) HB_WASM_API(ret_t, name)
#پایان_اگر
#اگر_تعریف_نشده HB_WASM_INTERFACE
#تعریف_کن HB_WASM_INTERFACE(ret_t, name) ret_t name
#پایان_اگر
#اگر_تعریف_نشده HB_WASM_EXEC_ENV
#تعریف_کن HB_WASM_EXEC_ENV
#پایان_اگر
#اگر_تعریف_نشده HB_WASM_EXEC_ENV_COMPOUND
#تعریف_کن HB_WASM_EXEC_ENV_COMPOUND HB_WASM_EXEC_ENV
#پایان_اگر


#اگر_تعریف_نشده bool_t
#تعریف_کن bool_t uint32_t
#پایان_اگر
#اگر_تعریف_نشده ptr_t
#تعریف_کن ptr_t(type_t) type_t *
#پایان_اگر
#اگر_تعریف_نشده ptr_d
#تعریف_کن ptr_d(type_t, name) type_t *name
#پایان_اگر

تعريفنوع uint32_t codepoint_t;
تعريفنوع int32_t position_t;
تعريفنوع uint32_t ماسک_t;
تعريفنوع uint32_t برچسب_t;
#تعریف_کن برچسب_3(c1,c2,c3,c4) ((برچسب_t)((((uint32_t)(c1)&0xFF)<<24)|(((uint32_t)(c2)&0xFF)<<16)|(((uint32_t)(c3)&0xFF)<<8)|((uint32_t)(c4)&0xFF)))

تعريفنوع تعداد {
  DIRECTION_INVALID = 0,
  DIRECTION_LTR = 4,
  DIRECTION_RTL,
  DIRECTION_TTB,
  DIRECTION_BTT
} direction_t;
#تعریف_کن DIRECTION_IS_VALID(dir)		((((بیعلامت صحيح) (dir)) & ~3U) == 4)
#تعریف_کن DIRECTION_IS_HORIZONTAL(dir)	((((بیعلامت صحيح) (dir)) & ~1U) == 4)
#تعریف_کن DIRECTION_IS_VERTICAL(dir)	((((بیعلامت صحيح) (dir)) & ~1U) == 6)
#تعریف_کن DIRECTION_IS_FORWARD(dir)	((((بیعلامت صحيح) (dir)) & ~2U) == 4)
#تعریف_کن DIRECTION_IS_BACKWARD(dir)	((((بیعلامت صحيح) (dir)) & ~2U) == 5)
#تعریف_کن DIRECTION_معکوس(dir)		((direction_t) (((بیعلامت صحيح) (dir)) ^ 1))

تعريفنوع برچسب_t script_t; /* ISO 15924 representation of Unicode scripts. */


/* common */

HB_WASM_API (direction_t, script_get_horizontal_direction) (HB_WASM_EXEC_ENV
							    script_t script);


/* blob */

تعريفنوع بنية
{
  uint32_t درازی;
  ptr_t(حرف) داده;
} blob_t;
#تعریف_کن BLOB_INIT {0, 0}

HB_WASM_API (فراغ, blob_free) (HB_WASM_EXEC_ENV
			       ptr_d(blob_t, blob));

/*میانگیر*/

تعريفنوع بنية
{
  uint32_t codepoint;
  uint32_t ماسک;
  uint32_t cluster;
  uint32_t var1;
  uint32_t var2;
} glyph_info_t;

تعريفنوع بنية
{
  position_t x_advance;
  position_t y_advance;
  position_t x_جابه_جایی_2;
  position_t y_جابه_جایی_2;
  uint32_t var;
} glyph_position_t;

تعريفنوع بنية
{
  uint32_t درازی;
  ptr_t(glyph_info_t) info;
  ptr_t(glyph_position_t) pos;
} میانگیر_contents_t;
#تعریف_کن میانگیر_CONTENTS_INIT {0, 0, 0}

HB_WASM_API (bool_t, میانگیر_contents_realloc) (HB_WASM_EXEC_ENV
					       ptr_d(میانگیر_contents_t, contents),
					       uint32_t اندازه);

HB_WASM_API (فراغ, میانگیر_contents_free) (HB_WASM_EXEC_ENV
					  ptr_d(میانگیر_contents_t, contents));

تعريفنوع بنية میانگیر_t میانگیر_t;

HB_WASM_API (bool_t, میانگیر_copy_contents) (HB_WASM_EXEC_ENV
					    ptr_d(میانگیر_t, میانگیر),
					    ptr_d(میانگیر_contents_t, contents));

HB_WASM_API (bool_t, میانگیر_مجموعه_contents) (HB_WASM_EXEC_ENV
					   ptr_d(میانگیر_t, میانگیر),
					   ptr_d(ثابت میانگیر_contents_t, contents));

HB_WASM_API (direction_t, میانگیر_get_direction) (HB_WASM_EXEC_ENV
						 ptr_d(میانگیر_t, میانگیر));

HB_WASM_API (script_t, میانگیر_get_script) (HB_WASM_EXEC_ENV
					   ptr_d(میانگیر_t, میانگیر));

HB_WASM_API (فراغ, میانگیر_معکوس) (HB_WASM_EXEC_ENV
				    ptr_d(میانگیر_t, میانگیر));

HB_WASM_API (فراغ, میانگیر_معکوس_clusters) (HB_WASM_EXEC_ENV
					     ptr_d(میانگیر_t, میانگیر));

/* face */

تعريفنوع بنية face_t face_t;

HB_WASM_API (ptr_t(face_t), face_create) (HB_WASM_EXEC_ENV
					  ptr_d(blob_t, blob),
					  بیعلامت صحيح);

HB_WASM_API (bool_t, face_copy_جدول) (HB_WASM_EXEC_ENV
				       ptr_d(face_t, face),
				       برچسب_t جدول_برچسب_2,
				       ptr_d(blob_t, blob));

HB_WASM_API (بیعلامت, face_get_upem) (HB_WASM_EXEC_ENV
				       ptr_d(face_t, face));

/* font */

تعريفنوع بنية font_t font_t;

HB_WASM_API (ptr_t(font_t), font_create) (HB_WASM_EXEC_ENV
					  ptr_d(face_t, face));

HB_WASM_API (ptr_t(face_t), font_get_face) (HB_WASM_EXEC_ENV
					    ptr_d(font_t, font));

HB_WASM_API (فراغ, font_get_scale) (HB_WASM_EXEC_ENV
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

تعريفنوع بنية
{
  position_t x_bearing;
  position_t y_bearing;
  position_t عرض;
  position_t ارتفاع;
} glyph_extents_t;

HB_WASM_API (bool_t, font_get_glyph_extents) (HB_WASM_EXEC_ENV
					      ptr_d(font_t, font),
					      codepoint_t glyph,
					      ptr_d(glyph_extents_t, extents));

HB_WASM_API (فراغ, font_glyph_مقصد_string) (HB_WASM_EXEC_ENV
					  ptr_d(font_t, font),
					  codepoint_t glyph,
					  ptr_d(حرف, s), uint32_t اندازه);


تعريفنوع بنية
{
  بیعلامت صحيح درازی;
  ptr_t(صحيح) coords;
} coords_t;

HB_WASM_API (bool_t, font_copy_coords) (HB_WASM_EXEC_ENV
					  ptr_d(font_t, font),
					  ptr_d(coords_t, coords));

HB_WASM_API (bool_t, font_مجموعه_coords) (HB_WASM_EXEC_ENV
					  ptr_d(font_t, font),
					  ptr_d(coords_t, coords));

/* outline */

تعداد glyph_outline_نقطه_type_t
{
  MOVE_مقصد,
  سطر_مقصد_2,
  QUADRATIC_مقصد,
  CUBIC_مقصد,
};

تعريفنوع بنية
{
  عائم x;
  عائم y;
  uint32_t type;
} glyph_outline_نقطه_t;

تعريفنوع بنية
{
  uint32_t n_points;
  ptr_t(glyph_outline_نقطه_t) points;
  uint32_t n_contours;
  ptr_t(uint32_t) contours;
} glyph_outline_t;
#تعریف_کن GLYPH_OUTLINE_INIT {0, 0, 0, 0}

HB_WASM_API (فراغ, glyph_outline_free) (HB_WASM_EXEC_ENV
					ptr_d(glyph_outline_t, outline));

HB_WASM_API (bool_t, font_copy_glyph_outline) (HB_WASM_EXEC_ENV
					       ptr_d(font_t, font),
					       codepoint_t glyph,
					       ptr_d(glyph_outline_t, outline));


/* shape */

تعريفنوع بنية
{
  برچسب_t    برچسب_2;
  uint32_t مقدار_2;
  uint32_t شروع;
  uint32_t انتها;
} feature_t;
#تعریف_کن FEATURE_GLOBAL_شروع	0
#تعریف_کن FEATURE_GLOBAL_انتها	((uint32_t) -1)

تعريفنوع بنية
{
  uint32_t درازی;
  ptr_t(feature_t) features;
} features_t;
#تعریف_کن FEATURES_INIT {0, 0}

HB_WASM_API (bool_t, feature_copy_contents) (HB_WASM_EXEC_ENV
					       ptr_d(ثابت feature_t, features),
					       uint32_t رقم_features,
					       ptr_d(features_t, output));

HB_WASM_API (فراغ, features_free) (HB_WASM_EXEC_ENV
				     ptr_d(features_t, features));

HB_WASM_API (bool_t, shape_with) (HB_WASM_EXEC_ENV
				  ptr_d(font_t, font),
				  ptr_d(میانگیر_t, میانگیر),
				  ptr_d(ثابت feature_t, features),
				  uint32_t رقم_features,
				  ثابت حرف *shaper);

/* Implement these in your shaper. */

HB_WASM_INTERFACE (ptr_t(فراغ), shape_plan_create) (ptr_d(face_t, face));

HB_WASM_INTERFACE (bool_t, shape) (ptr_d(فراغ, shape_plan),
				   ptr_d(font_t, font),
				   ptr_d(میانگیر_t, میانگیر),
				   ptr_d(ثابت feature_t, features),
				   uint32_t رقم_features);

HB_WASM_INTERFACE (فراغ, shape_plan_destroy) (ptr_d(فراغ, shape_plan));


HB_WASM_انتها_DECLS

#پایان_اگر /* HB_WASM_API_H */
