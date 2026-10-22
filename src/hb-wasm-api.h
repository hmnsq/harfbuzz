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

#ifndef HB_WASM_API_H
#define HB_WASM_API_H

/*
#include "hb.h"

HB_BEGIN_DECLS
HB_END_DECLS
*/

#ಅನ್ತರ್ಭಾವ <stdint.h>


#ifndef HB_WASM_BEGIN_DECLS
# ifdef __cplusplus
#  define HB_WASM_BEGIN_DECLS	ಬಾಹ್ಯ "C" {
#  define HB_WASM_END_DECLS	}
# ಅನ್ಯಥಾ /* !__cplusplus */
#  define HB_WASM_BEGIN_DECLS
#  define HB_WASM_END_DECLS
# endif /* !__cplusplus */
#endif


HB_WASM_BEGIN_DECLS

#ifndef HB_WASM_API
#define HB_WASM_API(ret_t, name) ret_t name
#endif
#ifndef HB_WASM_API_COMPOUND /* compound return type */
#define HB_WASM_API_COMPOUND(ret_t, name) HB_WASM_API(ret_t, name)
#endif
#ifndef HB_WASM_INTERFACE
#define HB_WASM_INTERFACE(ret_t, name) ret_t name
#endif
#ifndef HB_WASM_EXEC_ENV
#define HB_WASM_EXEC_ENV
#endif
#ifndef HB_WASM_EXEC_ENV_COMPOUND
#define HB_WASM_EXEC_ENV_COMPOUND HB_WASM_EXEC_ENV
#endif


#ifndef bool_t
#define bool_t uint32_t
#endif
#ifndef ptr_t
#define ptr_t(type_t) type_t *
#endif
#ifndef ptr_d
#define ptr_d(type_t, name) type_t *name
#endif

ಪ್ರಕಾರನಿರ್ದೇಶ uint32_t codepoint_t;
ಪ್ರಕಾರನಿರ್ದೇಶ int32_t position_t;
ಪ್ರಕಾರನಿರ್ದೇಶ uint32_t mask_t;
ಪ್ರಕಾರನಿರ್ದೇಶ uint32_t tag_t;
#define TAG(c1,c2,c3,c4) ((tag_t)((((uint32_t)(c1)&0xFF)<<24)|(((uint32_t)(c2)&0xFF)<<16)|(((uint32_t)(c3)&0xFF)<<8)|((uint32_t)(c4)&0xFF)))

ಪ್ರಕಾರನಿರ್ದೇಶ ಗಣನಾ {
  DIRECTION_INVALID = 0,
  DIRECTION_LTR = 4,
  DIRECTION_RTL,
  DIRECTION_TTB,
  DIRECTION_BTT
} direction_t;
#define DIRECTION_IS_VALID(dir)		((((unsigned ಪೂರ್ಣಾಙ್ಕ) (dir)) & ~3U) == 4)
#define DIRECTION_IS_HORIZONTAL(dir)	((((unsigned ಪೂರ್ಣಾಙ್ಕ) (dir)) & ~1U) == 4)
#define DIRECTION_IS_VERTICAL(dir)	((((unsigned ಪೂರ್ಣಾಙ್ಕ) (dir)) & ~1U) == 6)
#define DIRECTION_IS_FORWARD(dir)	((((unsigned ಪೂರ್ಣಾಙ್ಕ) (dir)) & ~2U) == 4)
#define DIRECTION_IS_BACKWARD(dir)	((((unsigned ಪೂರ್ಣಾಙ್ಕ) (dir)) & ~2U) == 5)
#define DIRECTION_REVERSE(dir)		((direction_t) (((unsigned ಪೂರ್ಣಾಙ್ಕ) (dir)) ^ 1))

ಪ್ರಕಾರನಿರ್ದೇಶ tag_t script_t; /* ISO 15924 representation of Unicode scripts. */


/* common */

HB_WASM_API (direction_t, script_get_horizontal_direction) (HB_WASM_EXEC_ENV
							    script_t script);


/* blob */

ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ
{
  uint32_t length;
  ptr_t(ವರ್ಣ) data;
} blob_t;
#define BLOB_INIT {0, 0}

HB_WASM_API (ಶೂನ್ಯ, blob_free) (HB_WASM_EXEC_ENV
			       ptr_d(blob_t, blob));

/* buffer */

ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ
{
  uint32_t codepoint;
  uint32_t mask;
  uint32_t cluster;
  uint32_t var1;
  uint32_t var2;
} glyph_info_t;

ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ
{
  position_t x_advance;
  position_t y_advance;
  position_t x_offset;
  position_t y_offset;
  uint32_t var;
} glyph_position_t;

ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ
{
  uint32_t length;
  ptr_t(glyph_info_t) info;
  ptr_t(glyph_position_t) pos;
} buffer_contents_t;
#define BUFFER_CONTENTS_INIT {0, 0, 0}

HB_WASM_API (bool_t, buffer_contents_realloc) (HB_WASM_EXEC_ENV
					       ptr_d(buffer_contents_t, contents),
					       uint32_t size);

HB_WASM_API (ಶೂನ್ಯ, buffer_contents_free) (HB_WASM_EXEC_ENV
					  ptr_d(buffer_contents_t, contents));

ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ buffer_t buffer_t;

HB_WASM_API (bool_t, buffer_copy_contents) (HB_WASM_EXEC_ENV
					    ptr_d(buffer_t, buffer),
					    ptr_d(buffer_contents_t, contents));

HB_WASM_API (bool_t, buffer_set_contents) (HB_WASM_EXEC_ENV
					   ptr_d(buffer_t, buffer),
					   ptr_d(ಧ್ರುವ buffer_contents_t, contents));

HB_WASM_API (direction_t, buffer_get_direction) (HB_WASM_EXEC_ENV
						 ptr_d(buffer_t, buffer));

HB_WASM_API (script_t, buffer_get_script) (HB_WASM_EXEC_ENV
					   ptr_d(buffer_t, buffer));

HB_WASM_API (ಶೂನ್ಯ, buffer_reverse) (HB_WASM_EXEC_ENV
				    ptr_d(buffer_t, buffer));

HB_WASM_API (ಶೂನ್ಯ, buffer_reverse_clusters) (HB_WASM_EXEC_ENV
					     ptr_d(buffer_t, buffer));

/* face */

ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ face_t face_t;

HB_WASM_API (ptr_t(face_t), face_create) (HB_WASM_EXEC_ENV
					  ptr_d(blob_t, blob),
					  unsigned ಪೂರ್ಣಾಙ್ಕ);

HB_WASM_API (bool_t, face_copy_table) (HB_WASM_EXEC_ENV
				       ptr_d(face_t, face),
				       tag_t table_tag,
				       ptr_d(blob_t, blob));

HB_WASM_API (unsigned, face_get_upem) (HB_WASM_EXEC_ENV
				       ptr_d(face_t, face));

/* font */

ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ font_t font_t;

HB_WASM_API (ptr_t(font_t), font_create) (HB_WASM_EXEC_ENV
					  ptr_d(face_t, face));

HB_WASM_API (ptr_t(face_t), font_get_face) (HB_WASM_EXEC_ENV
					    ptr_d(font_t, font));

HB_WASM_API (ಶೂನ್ಯ, font_get_scale) (HB_WASM_EXEC_ENV
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

ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ
{
  position_t x_bearing;
  position_t y_bearing;
  position_t width;
  position_t height;
} glyph_extents_t;

HB_WASM_API (bool_t, font_get_glyph_extents) (HB_WASM_EXEC_ENV
					      ptr_d(font_t, font),
					      codepoint_t glyph,
					      ptr_d(glyph_extents_t, extents));

HB_WASM_API (ಶೂನ್ಯ, font_glyph_to_string) (HB_WASM_EXEC_ENV
					  ptr_d(font_t, font),
					  codepoint_t glyph,
					  ptr_d(ವರ್ಣ, s), uint32_t size);


ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ
{
  unsigned ಪೂರ್ಣಾಙ್ಕ length;
  ptr_t(ಪೂರ್ಣಾಙ್ಕ) coords;
} coords_t;

HB_WASM_API (bool_t, font_copy_coords) (HB_WASM_EXEC_ENV
					  ptr_d(font_t, font),
					  ptr_d(coords_t, coords));

HB_WASM_API (bool_t, font_set_coords) (HB_WASM_EXEC_ENV
					  ptr_d(font_t, font),
					  ptr_d(coords_t, coords));

/* outline */

ಗಣನಾ glyph_outline_point_type_t
{
  MOVE_TO,
  ಸಾಲು_TO,
  QUADRATIC_TO,
  CUBIC_TO,
};

ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ
{
  ಭಿನ್ನ x;
  ಭಿನ್ನ y;
  uint32_t type;
} glyph_outline_point_t;

ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ
{
  uint32_t n_points;
  ptr_t(glyph_outline_point_t) points;
  uint32_t n_contours;
  ptr_t(uint32_t) contours;
} glyph_outline_t;
#define GLYPH_OUTLINE_INIT {0, 0, 0, 0}

HB_WASM_API (ಶೂನ್ಯ, glyph_outline_free) (HB_WASM_EXEC_ENV
					ptr_d(glyph_outline_t, outline));

HB_WASM_API (bool_t, font_copy_glyph_outline) (HB_WASM_EXEC_ENV
					       ptr_d(font_t, font),
					       codepoint_t glyph,
					       ptr_d(glyph_outline_t, outline));


/* shape */

ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ
{
  tag_t    tag;
  uint32_t ಮೌಲ್ಯ_2;
  uint32_t start;
  uint32_t end;
} feature_t;
#define FEATURE_GLOBAL_START	0
#define FEATURE_GLOBAL_END	((uint32_t) -1)

ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ
{
  uint32_t length;
  ptr_t(feature_t) features;
} features_t;
#define FEATURES_INIT {0, 0}

HB_WASM_API (bool_t, feature_copy_contents) (HB_WASM_EXEC_ENV
					       ptr_d(ಧ್ರುವ feature_t, features),
					       uint32_t ಅಂಕೆ_features,
					       ptr_d(features_t, output));

HB_WASM_API (ಶೂನ್ಯ, features_free) (HB_WASM_EXEC_ENV
				     ptr_d(features_t, features));

HB_WASM_API (bool_t, shape_with) (HB_WASM_EXEC_ENV
				  ptr_d(font_t, font),
				  ptr_d(buffer_t, buffer),
				  ptr_d(ಧ್ರುವ feature_t, features),
				  uint32_t ಅಂಕೆ_features,
				  ಧ್ರುವ ವರ್ಣ *shaper);

/* Implement these in your shaper. */

HB_WASM_INTERFACE (ptr_t(ಶೂನ್ಯ), shape_plan_create) (ptr_d(face_t, face));

HB_WASM_INTERFACE (bool_t, shape) (ptr_d(ಶೂನ್ಯ, shape_plan),
				   ptr_d(font_t, font),
				   ptr_d(buffer_t, buffer),
				   ptr_d(ಧ್ರುವ feature_t, features),
				   uint32_t ಅಂಕೆ_features);

HB_WASM_INTERFACE (ಶೂನ್ಯ, shape_plan_destroy) (ptr_d(ಶೂನ್ಯ, shape_plan));


HB_WASM_END_DECLS

#endif /* HB_WASM_API_H */
