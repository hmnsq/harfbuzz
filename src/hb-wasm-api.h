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

#که_نه_تعریف_شوی HB_WASM_API_H
#تعریف_کړه HB_WASM_API_H

/*
#include "hb.h"

HB_BEGIN_DECLS
HB_END_DECLS
*/

#ضمن <stdint.h>


#که_نه_تعریف_شوی HB_WASM_BEGIN_DECLS
# ifdef __cplusplus
#  define HB_WASM_BEGIN_DECLS	خارجي "C" {
#  define HB_WASM_پای_ټکی_DECLS	}
# وإلا /* !__cplusplus */
#  define HB_WASM_BEGIN_DECLS
#  define HB_WASM_پای_ټکی_DECLS
# endif /* !__cplusplus */
#د_که_پای


HB_WASM_BEGIN_DECLS

#که_نه_تعریف_شوی HB_WASM_API
#تعریف_کړه HB_WASM_API(ret_t, name) ret_t name
#د_که_پای
#که_نه_تعریف_شوی HB_WASM_API_COMPOUND /* compound return type */
#تعریف_کړه HB_WASM_API_COMPOUND(ret_t, name) HB_WASM_API(ret_t, name)
#د_که_پای
#که_نه_تعریف_شوی HB_WASM_INTERFACE
#تعریف_کړه HB_WASM_INTERFACE(ret_t, name) ret_t name
#د_که_پای
#که_نه_تعریف_شوی HB_WASM_EXEC_ENV
#تعریف_کړه HB_WASM_EXEC_ENV
#د_که_پای
#که_نه_تعریف_شوی HB_WASM_EXEC_ENV_COMPOUND
#تعریف_کړه HB_WASM_EXEC_ENV_COMPOUND HB_WASM_EXEC_ENV
#د_که_پای


#که_نه_تعریف_شوی bool_t
#تعریف_کړه bool_t uint32_t
#د_که_پای
#که_نه_تعریف_شوی ptr_t
#تعریف_کړه ptr_t(type_t) type_t *
#د_که_پای
#که_نه_تعریف_شوی ptr_d
#تعریف_کړه ptr_d(type_t, name) type_t *name
#د_که_پای

تعريفنوع uint32_t codepoint_t;
تعريفنوع int32_t position_t;
تعريفنوع uint32_t ماسک_t;
تعريفنوع uint32_t نښان_t;
#تعریف_کړه نښان_3(c1,c2,c3,c4) ((نښان_t)((((uint32_t)(c1)&0xFF)<<24)|(((uint32_t)(c2)&0xFF)<<16)|(((uint32_t)(c3)&0xFF)<<8)|((uint32_t)(c4)&0xFF)))

تعريفنوع تعداد {
  DIRECTION_INVALID = 0,
  DIRECTION_LTR = 4,
  DIRECTION_RTL,
  DIRECTION_TTB,
  DIRECTION_BTT
} direction_t;
#تعریف_کړه DIRECTION_IS_VALID(dir)		((((بینښه صحيح) (dir)) & ~3U) == 4)
#تعریف_کړه DIRECTION_IS_HORIZONTAL(dir)	((((بینښه صحيح) (dir)) & ~1U) == 4)
#تعریف_کړه DIRECTION_IS_VERTICAL(dir)	((((بینښه صحيح) (dir)) & ~1U) == 6)
#تعریف_کړه DIRECTION_IS_FORWARD(dir)	((((بینښه صحيح) (dir)) & ~2U) == 4)
#تعریف_کړه DIRECTION_IS_BACKWARD(dir)	((((بینښه صحيح) (dir)) & ~2U) == 5)
#تعریف_کړه DIRECTION_سرچپه(dir)		((direction_t) (((بینښه صحيح) (dir)) ^ 1))

تعريفنوع نښان_t script_t; /* ISO 15924 representation of Unicode scripts. */


/* common */

HB_WASM_API (direction_t, script_get_horizontal_direction) (HB_WASM_EXEC_ENV
							    script_t script);


/* blob */

تعريفنوع بنية
{
  uint32_t اوږدتیا;
  ptr_t(حرف) معلومات;
} blob_t;
#تعریف_کړه BLOB_INIT {0, 0}

HB_WASM_API (فراغ, blob_free) (HB_WASM_EXEC_ENV
			       ptr_d(blob_t, blob));

/*بفر*/

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
  position_t x_دننه_واټن_2;
  position_t y_دننه_واټن_2;
  uint32_t var;
} glyph_position_t;

تعريفنوع بنية
{
  uint32_t اوږدتیا;
  ptr_t(glyph_info_t) info;
  ptr_t(glyph_position_t) pos;
} بفر_contents_t;
#تعریف_کړه بفر_CONTENTS_INIT {0, 0, 0}

HB_WASM_API (bool_t, بفر_contents_realloc) (HB_WASM_EXEC_ENV
					       ptr_d(بفر_contents_t, contents),
					       uint32_t اندازه);

HB_WASM_API (فراغ, بفر_contents_free) (HB_WASM_EXEC_ENV
					  ptr_d(بفر_contents_t, contents));

تعريفنوع بنية بفر_t بفر_t;

HB_WASM_API (bool_t, بفر_copy_contents) (HB_WASM_EXEC_ENV
					    ptr_d(بفر_t, بفر),
					    ptr_d(بفر_contents_t, contents));

HB_WASM_API (bool_t, بفر_سېټ_contents) (HB_WASM_EXEC_ENV
					   ptr_d(بفر_t, بفر),
					   ptr_d(ثابت بفر_contents_t, contents));

HB_WASM_API (direction_t, بفر_get_direction) (HB_WASM_EXEC_ENV
						 ptr_d(بفر_t, بفر));

HB_WASM_API (script_t, بفر_get_script) (HB_WASM_EXEC_ENV
					   ptr_d(بفر_t, بفر));

HB_WASM_API (فراغ, بفر_سرچپه) (HB_WASM_EXEC_ENV
				    ptr_d(بفر_t, بفر));

HB_WASM_API (فراغ, بفر_سرچپه_clusters) (HB_WASM_EXEC_ENV
					     ptr_d(بفر_t, بفر));

/* face */

تعريفنوع بنية face_t face_t;

HB_WASM_API (ptr_t(face_t), face_create) (HB_WASM_EXEC_ENV
					  ptr_d(blob_t, blob),
					  بینښه صحيح);

HB_WASM_API (bool_t, face_copy_جدول) (HB_WASM_EXEC_ENV
				       ptr_d(face_t, face),
				       نښان_t جدول_نښان_2,
				       ptr_d(blob_t, blob));

HB_WASM_API (بینښه, face_get_upem) (HB_WASM_EXEC_ENV
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
  position_t پلنوالی;
  position_t لوړوالی;
} glyph_extents_t;

HB_WASM_API (bool_t, font_get_glyph_extents) (HB_WASM_EXEC_ENV
					      ptr_d(font_t, font),
					      codepoint_t glyph,
					      ptr_d(glyph_extents_t, extents));

HB_WASM_API (فراغ, font_glyph_منزل_string) (HB_WASM_EXEC_ENV
					  ptr_d(font_t, font),
					  codepoint_t glyph,
					  ptr_d(حرف, s), uint32_t اندازه);


تعريفنوع بنية
{
  بینښه صحيح اوږدتیا;
  ptr_t(صحيح) coords;
} coords_t;

HB_WASM_API (bool_t, font_copy_coords) (HB_WASM_EXEC_ENV
					  ptr_d(font_t, font),
					  ptr_d(coords_t, coords));

HB_WASM_API (bool_t, font_سېټ_coords) (HB_WASM_EXEC_ENV
					  ptr_d(font_t, font),
					  ptr_d(coords_t, coords));

/* outline */

تعداد glyph_outline_ټکی_type_t
{
  MOVE_منزل,
  کرښه_منزل_2,
  QUADRATIC_منزل,
  CUBIC_منزل,
};

تعريفنوع بنية
{
  عائم x;
  عائم y;
  uint32_t type;
} glyph_outline_ټکی_t;

تعريفنوع بنية
{
  uint32_t n_points;
  ptr_t(glyph_outline_ټکی_t) points;
  uint32_t n_contours;
  ptr_t(uint32_t) contours;
} glyph_outline_t;
#تعریف_کړه GLYPH_OUTLINE_INIT {0, 0, 0, 0}

HB_WASM_API (فراغ, glyph_outline_free) (HB_WASM_EXEC_ENV
					ptr_d(glyph_outline_t, outline));

HB_WASM_API (bool_t, font_copy_glyph_outline) (HB_WASM_EXEC_ENV
					       ptr_d(font_t, font),
					       codepoint_t glyph,
					       ptr_d(glyph_outline_t, outline));


/* shape */

تعريفنوع بنية
{
  نښان_t    نښان_2;
  uint32_t ارزښت_2;
  uint32_t پیل;
  uint32_t پای_ټکی;
} feature_t;
#تعریف_کړه FEATURE_GLOBAL_پیل	0
#تعریف_کړه FEATURE_GLOBAL_پای_ټکی	((uint32_t) -1)

تعريفنوع بنية
{
  uint32_t اوږدتیا;
  ptr_t(feature_t) features;
} features_t;
#تعریف_کړه FEATURES_INIT {0, 0}

HB_WASM_API (bool_t, feature_copy_contents) (HB_WASM_EXEC_ENV
					       ptr_d(ثابت feature_t, features),
					       uint32_t عدد_features,
					       ptr_d(features_t, output));

HB_WASM_API (فراغ, features_free) (HB_WASM_EXEC_ENV
				     ptr_d(features_t, features));

HB_WASM_API (bool_t, shape_with) (HB_WASM_EXEC_ENV
				  ptr_d(font_t, font),
				  ptr_d(بفر_t, بفر),
				  ptr_d(ثابت feature_t, features),
				  uint32_t عدد_features,
				  ثابت حرف *shaper);

/* Implement these in your shaper. */

HB_WASM_INTERFACE (ptr_t(فراغ), shape_plan_create) (ptr_d(face_t, face));

HB_WASM_INTERFACE (bool_t, shape) (ptr_d(فراغ, shape_plan),
				   ptr_d(font_t, font),
				   ptr_d(بفر_t, بفر),
				   ptr_d(ثابت feature_t, features),
				   uint32_t عدد_features);

HB_WASM_INTERFACE (فراغ, shape_plan_destroy) (ptr_d(فراغ, shape_plan));


HB_WASM_پای_ټکی_DECLS

#د_که_پای /* HB_WASM_API_H */
