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

#جيڪڏهن_وصف_نه_ٿيل HB_WASM_API_H
#وصف_ڪريو HB_WASM_API_H

/*
#include "hb.h"

HB_BEGIN_DECLS
HB_END_DECLS
*/

#ضمن <stdint.h>


#جيڪڏهن_وصف_نه_ٿيل HB_WASM_BEGIN_DECLS
# ifdef __cplusplus
#  define HB_WASM_BEGIN_DECLS	خارجي "C" {
#  define HB_WASM_پڇاڙي_DECLS	}
# وإلا /* !__cplusplus */
#  define HB_WASM_BEGIN_DECLS
#  define HB_WASM_پڇاڙي_DECLS
# endif /* !__cplusplus */
#جيڪڏهن_پورو


HB_WASM_BEGIN_DECLS

#جيڪڏهن_وصف_نه_ٿيل HB_WASM_API
#وصف_ڪريو HB_WASM_API(ret_t, name) ret_t name
#جيڪڏهن_پورو
#جيڪڏهن_وصف_نه_ٿيل HB_WASM_API_COMPOUND /* compound return type */
#وصف_ڪريو HB_WASM_API_COMPOUND(ret_t, name) HB_WASM_API(ret_t, name)
#جيڪڏهن_پورو
#جيڪڏهن_وصف_نه_ٿيل HB_WASM_INTERFACE
#وصف_ڪريو HB_WASM_INTERFACE(ret_t, name) ret_t name
#جيڪڏهن_پورو
#جيڪڏهن_وصف_نه_ٿيل HB_WASM_EXEC_ENV
#وصف_ڪريو HB_WASM_EXEC_ENV
#جيڪڏهن_پورو
#جيڪڏهن_وصف_نه_ٿيل HB_WASM_EXEC_ENV_COMPOUND
#وصف_ڪريو HB_WASM_EXEC_ENV_COMPOUND HB_WASM_EXEC_ENV
#جيڪڏهن_پورو


#جيڪڏهن_وصف_نه_ٿيل bool_t
#وصف_ڪريو bool_t uint32_t
#جيڪڏهن_پورو
#جيڪڏهن_وصف_نه_ٿيل ptr_t
#وصف_ڪريو ptr_t(type_t) type_t *
#جيڪڏهن_پورو
#جيڪڏهن_وصف_نه_ٿيل ptr_d
#وصف_ڪريو ptr_d(type_t, name) type_t *name
#جيڪڏهن_پورو

تعريفنوع uint32_t codepoint_t;
تعريفنوع int32_t position_t;
تعريفنوع uint32_t نقاب_t;
تعريفنوع uint32_t ٽيگ_t;
#وصف_ڪريو ٽيگ_3(c1,c2,c3,c4) ((ٽيگ_t)((((uint32_t)(c1)&0xFF)<<24)|(((uint32_t)(c2)&0xFF)<<16)|(((uint32_t)(c3)&0xFF)<<8)|((uint32_t)(c4)&0xFF)))

تعريفنوع تعداد {
  DIRECTION_INVALID = 0,
  DIRECTION_LTR = 4,
  DIRECTION_RTL,
  DIRECTION_TTB,
  DIRECTION_BTT
} direction_t;
#وصف_ڪريو DIRECTION_IS_VALID(dir)		((((بنانشان صحيح) (dir)) & ~3U) == 4)
#وصف_ڪريو DIRECTION_IS_HORIZONTAL(dir)	((((بنانشان صحيح) (dir)) & ~1U) == 4)
#وصف_ڪريو DIRECTION_IS_VERTICAL(dir)	((((بنانشان صحيح) (dir)) & ~1U) == 6)
#وصف_ڪريو DIRECTION_IS_FORWARD(dir)	((((بنانشان صحيح) (dir)) & ~2U) == 4)
#وصف_ڪريو DIRECTION_IS_BACKWARD(dir)	((((بنانشان صحيح) (dir)) & ~2U) == 5)
#وصف_ڪريو DIRECTION_ابتو(dir)		((direction_t) (((بنانشان صحيح) (dir)) ^ 1))

تعريفنوع ٽيگ_t script_t; /* ISO 15924 representation of Unicode scripts. */


/* common */

HB_WASM_API (direction_t, script_get_horizontal_direction) (HB_WASM_EXEC_ENV
							    script_t script);


/* blob */

تعريفنوع بنية
{
  uint32_t ڊيگهائي;
  ptr_t(حرف) معلومات;
} blob_t;
#وصف_ڪريو BLOB_INIT {0, 0}

HB_WASM_API (فراغ, blob_free) (HB_WASM_EXEC_ENV
			       ptr_d(blob_t, blob));

/*بفر*/

تعريفنوع بنية
{
  uint32_t codepoint;
  uint32_t نقاب;
  uint32_t cluster;
  uint32_t var1;
  uint32_t var2;
} glyph_info_t;

تعريفنوع بنية
{
  position_t x_advance;
  position_t y_advance;
  position_t x_وٿي_2;
  position_t y_وٿي_2;
  uint32_t var;
} glyph_position_t;

تعريفنوع بنية
{
  uint32_t ڊيگهائي;
  ptr_t(glyph_info_t) info;
  ptr_t(glyph_position_t) pos;
} بفر_contents_t;
#وصف_ڪريو بفر_CONTENTS_INIT {0, 0, 0}

HB_WASM_API (bool_t, بفر_contents_realloc) (HB_WASM_EXEC_ENV
					       ptr_d(بفر_contents_t, contents),
					       uint32_t سائيز);

HB_WASM_API (فراغ, بفر_contents_free) (HB_WASM_EXEC_ENV
					  ptr_d(بفر_contents_t, contents));

تعريفنوع بنية بفر_t بفر_t;

HB_WASM_API (bool_t, بفر_copy_contents) (HB_WASM_EXEC_ENV
					    ptr_d(بفر_t, بفر),
					    ptr_d(بفر_contents_t, contents));

HB_WASM_API (bool_t, بفر_سيٽ_contents) (HB_WASM_EXEC_ENV
					   ptr_d(بفر_t, بفر),
					   ptr_d(ثابت بفر_contents_t, contents));

HB_WASM_API (direction_t, بفر_get_direction) (HB_WASM_EXEC_ENV
						 ptr_d(بفر_t, بفر));

HB_WASM_API (script_t, بفر_get_script) (HB_WASM_EXEC_ENV
					   ptr_d(بفر_t, بفر));

HB_WASM_API (فراغ, بفر_ابتو) (HB_WASM_EXEC_ENV
				    ptr_d(بفر_t, بفر));

HB_WASM_API (فراغ, بفر_ابتو_clusters) (HB_WASM_EXEC_ENV
					     ptr_d(بفر_t, بفر));

/* face */

تعريفنوع بنية face_t face_t;

HB_WASM_API (ptr_t(face_t), face_create) (HB_WASM_EXEC_ENV
					  ptr_d(blob_t, blob),
					  بنانشان صحيح);

HB_WASM_API (bool_t, face_copy_جدول) (HB_WASM_EXEC_ENV
				       ptr_d(face_t, face),
				       ٽيگ_t جدول_ٽيگ_2,
				       ptr_d(blob_t, blob));

HB_WASM_API (بنانشان, face_get_upem) (HB_WASM_EXEC_ENV
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
  position_t ويڪر;
  position_t اوچائي;
} glyph_extents_t;

HB_WASM_API (bool_t, font_get_glyph_extents) (HB_WASM_EXEC_ENV
					      ptr_d(font_t, font),
					      codepoint_t glyph,
					      ptr_d(glyph_extents_t, extents));

HB_WASM_API (فراغ, font_glyph_منزل_string) (HB_WASM_EXEC_ENV
					  ptr_d(font_t, font),
					  codepoint_t glyph,
					  ptr_d(حرف, s), uint32_t سائيز);


تعريفنوع بنية
{
  بنانشان صحيح ڊيگهائي;
  ptr_t(صحيح) coords;
} coords_t;

HB_WASM_API (bool_t, font_copy_coords) (HB_WASM_EXEC_ENV
					  ptr_d(font_t, font),
					  ptr_d(coords_t, coords));

HB_WASM_API (bool_t, font_سيٽ_coords) (HB_WASM_EXEC_ENV
					  ptr_d(font_t, font),
					  ptr_d(coords_t, coords));

/* outline */

تعداد glyph_outline_نقطو_type_t
{
  MOVE_منزل,
  سٽ_منزل_2,
  QUADRATIC_منزل,
  CUBIC_منزل,
};

تعريفنوع بنية
{
  عائم x;
  عائم y;
  uint32_t type;
} glyph_outline_نقطو_t;

تعريفنوع بنية
{
  uint32_t n_points;
  ptr_t(glyph_outline_نقطو_t) points;
  uint32_t n_contours;
  ptr_t(uint32_t) contours;
} glyph_outline_t;
#وصف_ڪريو GLYPH_OUTLINE_INIT {0, 0, 0, 0}

HB_WASM_API (فراغ, glyph_outline_free) (HB_WASM_EXEC_ENV
					ptr_d(glyph_outline_t, outline));

HB_WASM_API (bool_t, font_copy_glyph_outline) (HB_WASM_EXEC_ENV
					       ptr_d(font_t, font),
					       codepoint_t glyph,
					       ptr_d(glyph_outline_t, outline));


/* shape */

تعريفنوع بنية
{
  ٽيگ_t    ٽيگ_2;
  uint32_t قدر_2;
  uint32_t شروعات;
  uint32_t پڇاڙي;
} feature_t;
#وصف_ڪريو FEATURE_GLOBAL_شروعات	0
#وصف_ڪريو FEATURE_GLOBAL_پڇاڙي	((uint32_t) -1)

تعريفنوع بنية
{
  uint32_t ڊيگهائي;
  ptr_t(feature_t) features;
} features_t;
#وصف_ڪريو FEATURES_INIT {0, 0}

HB_WASM_API (bool_t, feature_copy_contents) (HB_WASM_EXEC_ENV
					       ptr_d(ثابت feature_t, features),
					       uint32_t انگ_features,
					       ptr_d(features_t, output));

HB_WASM_API (فراغ, features_free) (HB_WASM_EXEC_ENV
				     ptr_d(features_t, features));

HB_WASM_API (bool_t, shape_with) (HB_WASM_EXEC_ENV
				  ptr_d(font_t, font),
				  ptr_d(بفر_t, بفر),
				  ptr_d(ثابت feature_t, features),
				  uint32_t انگ_features,
				  ثابت حرف *shaper);

/* Implement these in your shaper. */

HB_WASM_INTERFACE (ptr_t(فراغ), shape_plan_create) (ptr_d(face_t, face));

HB_WASM_INTERFACE (bool_t, shape) (ptr_d(فراغ, shape_plan),
				   ptr_d(font_t, font),
				   ptr_d(بفر_t, بفر),
				   ptr_d(ثابت feature_t, features),
				   uint32_t انگ_features);

HB_WASM_INTERFACE (فراغ, shape_plan_destroy) (ptr_d(فراغ, shape_plan));


HB_WASM_پڇاڙي_DECLS

#جيڪڏهن_پورو /* HB_WASM_API_H */
