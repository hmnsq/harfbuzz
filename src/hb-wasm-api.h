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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_WASM_API_H
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_WASM_API_H

/*
#include "hb.h"

HB_BEGIN_DECLS
HB_END_DECLS
*/

#ਅਨ੍ਤਰ੍ਭਾਵ <stdint.h>


#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_WASM_BEGIN_DECLS
# ifdef __cplusplus
#  define HB_WASM_BEGIN_DECLS	ਬਾਹਰੀ "C" {
#  define HB_WASM_ਸਿਰਾ_DECLS	}
# ਨਹੀਂਤਾਂ /* !__cplusplus */
#  define HB_WASM_BEGIN_DECLS
#  define HB_WASM_ਸਿਰਾ_DECLS
# endif /* !__cplusplus */
#ਜੇ_ਅੰਤ


HB_WASM_BEGIN_DECLS

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_WASM_API
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_WASM_API(ret_t, name) ret_t name
#ਜੇ_ਅੰਤ
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_WASM_API_COMPOUND /* compound return type */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_WASM_API_COMPOUND(ret_t, name) HB_WASM_API(ret_t, name)
#ਜੇ_ਅੰਤ
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_WASM_INTERFACE
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_WASM_INTERFACE(ret_t, name) ret_t name
#ਜੇ_ਅੰਤ
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_WASM_EXEC_ENV
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_WASM_EXEC_ENV
#ਜੇ_ਅੰਤ
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_WASM_EXEC_ENV_COMPOUND
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_WASM_EXEC_ENV_COMPOUND HB_WASM_EXEC_ENV
#ਜੇ_ਅੰਤ


#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ bool_t
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ bool_t uint32_t
#ਜੇ_ਅੰਤ
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ ptr_t
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ptr_t(type_t) type_t *
#ਜੇ_ਅੰਤ
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ ptr_d
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ptr_d(type_t, name) type_t *name
#ਜੇ_ਅੰਤ

ਕਿਸਮਪਰਿਭਾਸ਼ਾ uint32_t codepoint_t;
ਕਿਸਮਪਰਿਭਾਸ਼ਾ int32_t position_t;
ਕਿਸਮਪਰਿਭਾਸ਼ਾ uint32_t ਨਕਾਬ_t;
ਕਿਸਮਪਰਿਭਾਸ਼ਾ uint32_t ਟੈਗ_t;
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਟੈਗ_3(c1,c2,c3,c4) ((ਟੈਗ_t)((((uint32_t)(c1)&0xFF)<<24)|(((uint32_t)(c2)&0xFF)<<16)|(((uint32_t)(c3)&0xFF)<<8)|((uint32_t)(c4)&0xFF)))

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਗਿਣਤੀ {
  DIRECTION_INVALID = 0,
  DIRECTION_LTR = 4,
  DIRECTION_RTL,
  DIRECTION_TTB,
  DIRECTION_BTT
} direction_t;
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DIRECTION_IS_VALID(dir)		((((ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) (dir)) & ~3U) == 4)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DIRECTION_IS_HORIZONTAL(dir)	((((ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) (dir)) & ~1U) == 4)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DIRECTION_IS_VERTICAL(dir)	((((ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) (dir)) & ~1U) == 6)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DIRECTION_IS_FORWARD(dir)	((((ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) (dir)) & ~2U) == 4)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DIRECTION_IS_BACKWARD(dir)	((((ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) (dir)) & ~2U) == 5)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DIRECTION_ਉਲਟ(dir)		((direction_t) (((ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) (dir)) ^ 1))

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਟੈਗ_t script_t; /* ISO 15924 representation of Unicode scripts. */


/* common */

HB_WASM_API (direction_t, script_get_horizontal_direction) (HB_WASM_EXEC_ENV
							    script_t script);


/* blob */

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ
{
  uint32_t ਲੰਮਾਈ;
  ptr_t(ਅੱਖਰ) ਅੰਕੜੇ;
} blob_t;
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ BLOB_INIT {0, 0}

HB_WASM_API (ਖਾਲੀ, blob_free) (HB_WASM_EXEC_ENV
			       ptr_d(blob_t, blob));

/*ਬਫਰ*/

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ
{
  uint32_t codepoint;
  uint32_t ਨਕਾਬ;
  uint32_t cluster;
  uint32_t var1;
  uint32_t var2;
} glyph_info_t;

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ
{
  position_t x_advance;
  position_t y_advance;
  position_t x_ਵਿੱਥ_2;
  position_t y_ਵਿੱਥ_2;
  uint32_t var;
} glyph_position_t;

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ
{
  uint32_t ਲੰਮਾਈ;
  ptr_t(glyph_info_t) info;
  ptr_t(glyph_position_t) pos;
} ਬਫਰ_contents_t;
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਬਫਰ_CONTENTS_INIT {0, 0, 0}

HB_WASM_API (bool_t, ਬਫਰ_contents_realloc) (HB_WASM_EXEC_ENV
					       ptr_d(ਬਫਰ_contents_t, contents),
					       uint32_t ਮਾਪ);

HB_WASM_API (ਖਾਲੀ, ਬਫਰ_contents_free) (HB_WASM_EXEC_ENV
					  ptr_d(ਬਫਰ_contents_t, contents));

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ ਬਫਰ_t ਬਫਰ_t;

HB_WASM_API (bool_t, ਬਫਰ_copy_contents) (HB_WASM_EXEC_ENV
					    ptr_d(ਬਫਰ_t, ਬਫਰ),
					    ptr_d(ਬਫਰ_contents_t, contents));

HB_WASM_API (bool_t, ਬਫਰ_ਸੈੱਟ_contents) (HB_WASM_EXEC_ENV
					   ptr_d(ਬਫਰ_t, ਬਫਰ),
					   ptr_d(ਅਚਲ ਬਫਰ_contents_t, contents));

HB_WASM_API (direction_t, ਬਫਰ_get_direction) (HB_WASM_EXEC_ENV
						 ptr_d(ਬਫਰ_t, ਬਫਰ));

HB_WASM_API (script_t, ਬਫਰ_get_script) (HB_WASM_EXEC_ENV
					   ptr_d(ਬਫਰ_t, ਬਫਰ));

HB_WASM_API (ਖਾਲੀ, ਬਫਰ_ਉਲਟ) (HB_WASM_EXEC_ENV
				    ptr_d(ਬਫਰ_t, ਬਫਰ));

HB_WASM_API (ਖਾਲੀ, ਬਫਰ_ਉਲਟ_clusters) (HB_WASM_EXEC_ENV
					     ptr_d(ਬਫਰ_t, ਬਫਰ));

/* face */

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ face_t face_t;

HB_WASM_API (ptr_t(face_t), face_create) (HB_WASM_EXEC_ENV
					  ptr_d(blob_t, blob),
					  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ);

HB_WASM_API (bool_t, face_copy_ਸਾਰਣੀ) (HB_WASM_EXEC_ENV
				       ptr_d(face_t, face),
				       ਟੈਗ_t ਸਾਰਣੀ_ਟੈਗ_2,
				       ptr_d(blob_t, blob));

HB_WASM_API (ਬਿਨਾਨਿਸ਼ਾਨੀ, face_get_upem) (HB_WASM_EXEC_ENV
				       ptr_d(face_t, face));

/* font */

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ font_t font_t;

HB_WASM_API (ptr_t(font_t), font_create) (HB_WASM_EXEC_ENV
					  ptr_d(face_t, face));

HB_WASM_API (ptr_t(face_t), font_get_face) (HB_WASM_EXEC_ENV
					    ptr_d(font_t, font));

HB_WASM_API (ਖਾਲੀ, font_get_scale) (HB_WASM_EXEC_ENV
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

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ
{
  position_t x_bearing;
  position_t y_bearing;
  position_t ਚੌੜਾਈ;
  position_t ਉਚਾਈ;
} glyph_extents_t;

HB_WASM_API (bool_t, font_get_glyph_extents) (HB_WASM_EXEC_ENV
					      ptr_d(font_t, font),
					      codepoint_t glyph,
					      ptr_d(glyph_extents_t, extents));

HB_WASM_API (ਖਾਲੀ, font_glyph_ਮੰਜ਼ਿਲ_string) (HB_WASM_EXEC_ENV
					  ptr_d(font_t, font),
					  codepoint_t glyph,
					  ptr_d(ਅੱਖਰ, s), uint32_t ਮਾਪ);


ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਲੰਮਾਈ;
  ptr_t(ਪੂਰਨ) coords;
} coords_t;

HB_WASM_API (bool_t, font_copy_coords) (HB_WASM_EXEC_ENV
					  ptr_d(font_t, font),
					  ptr_d(coords_t, coords));

HB_WASM_API (bool_t, font_ਸੈੱਟ_coords) (HB_WASM_EXEC_ENV
					  ptr_d(font_t, font),
					  ptr_d(coords_t, coords));

/* outline */

ਗਿਣਤੀ glyph_outline_ਬਿੰਦੂ_type_t
{
  MOVE_ਮੰਜ਼ਿਲ,
  ਸਤਰ_ਮੰਜ਼ਿਲ_2,
  QUADRATIC_ਮੰਜ਼ਿਲ,
  CUBIC_ਮੰਜ਼ਿਲ,
};

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ
{
  ਦਸ਼ਮਲਵ x;
  ਦਸ਼ਮਲਵ y;
  uint32_t type;
} glyph_outline_ਬਿੰਦੂ_t;

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ
{
  uint32_t n_points;
  ptr_t(glyph_outline_ਬਿੰਦੂ_t) points;
  uint32_t n_contours;
  ptr_t(uint32_t) contours;
} glyph_outline_t;
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ GLYPH_OUTLINE_INIT {0, 0, 0, 0}

HB_WASM_API (ਖਾਲੀ, glyph_outline_free) (HB_WASM_EXEC_ENV
					ptr_d(glyph_outline_t, outline));

HB_WASM_API (bool_t, font_copy_glyph_outline) (HB_WASM_EXEC_ENV
					       ptr_d(font_t, font),
					       codepoint_t glyph,
					       ptr_d(glyph_outline_t, outline));


/* shape */

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ
{
  ਟੈਗ_t    ਟੈਗ_2;
  uint32_t ਮੁੱਲ_2;
  uint32_t ਸ਼ੁਰੂ;
  uint32_t ਸਿਰਾ;
} feature_t;
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ FEATURE_GLOBAL_ਸ਼ੁਰੂ	0
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ FEATURE_GLOBAL_ਸਿਰਾ	((uint32_t) -1)

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ
{
  uint32_t ਲੰਮਾਈ;
  ptr_t(feature_t) features;
} features_t;
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ FEATURES_INIT {0, 0}

HB_WASM_API (bool_t, feature_copy_contents) (HB_WASM_EXEC_ENV
					       ptr_d(ਅਚਲ feature_t, features),
					       uint32_t ਅੰਕ_features,
					       ptr_d(features_t, output));

HB_WASM_API (ਖਾਲੀ, features_free) (HB_WASM_EXEC_ENV
				     ptr_d(features_t, features));

HB_WASM_API (bool_t, shape_with) (HB_WASM_EXEC_ENV
				  ptr_d(font_t, font),
				  ptr_d(ਬਫਰ_t, ਬਫਰ),
				  ptr_d(ਅਚਲ feature_t, features),
				  uint32_t ਅੰਕ_features,
				  ਅਚਲ ਅੱਖਰ *shaper);

/* Implement these in your shaper. */

HB_WASM_INTERFACE (ptr_t(ਖਾਲੀ), shape_plan_create) (ptr_d(face_t, face));

HB_WASM_INTERFACE (bool_t, shape) (ptr_d(ਖਾਲੀ, shape_plan),
				   ptr_d(font_t, font),
				   ptr_d(ਬਫਰ_t, ਬਫਰ),
				   ptr_d(ਅਚਲ feature_t, features),
				   uint32_t ਅੰਕ_features);

HB_WASM_INTERFACE (ਖਾਲੀ, shape_plan_destroy) (ptr_d(ਖਾਲੀ, shape_plan));


HB_WASM_ਸਿਰਾ_DECLS

#ਜੇ_ਅੰਤ /* HB_WASM_API_H */
