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

#अगर_अघोषित HB_WASM_API_H
#घोषणा HB_WASM_API_H

/*
#include "hb.h"

HB_BEGIN_DECLS
HB_END_DECLS
*/

#समावेश <stdint.h>


#अगर_अघोषित HB_WASM_BEGIN_DECLS
# ifdef __cplusplus
#  define HB_WASM_BEGIN_DECLS	बाह्य "C" {
#  define HB_WASM_END_DECLS	}
# अन्यथा /* !__cplusplus */
#  define HB_WASM_BEGIN_DECLS
#  define HB_WASM_END_DECLS
# endif /* !__cplusplus */
#पूर्ण_अगर


HB_WASM_BEGIN_DECLS

#अगर_अघोषित HB_WASM_API
#घोषणा HB_WASM_API(ret_t, name) ret_t name
#पूर्ण_अगर
#अगर_अघोषित HB_WASM_API_COMPOUND /* compound return type */
#घोषणा HB_WASM_API_COMPOUND(ret_t, name) HB_WASM_API(ret_t, name)
#पूर्ण_अगर
#अगर_अघोषित HB_WASM_INTERFACE
#घोषणा HB_WASM_INTERFACE(ret_t, name) ret_t name
#पूर्ण_अगर
#अगर_अघोषित HB_WASM_EXEC_ENV
#घोषणा HB_WASM_EXEC_ENV
#पूर्ण_अगर
#अगर_अघोषित HB_WASM_EXEC_ENV_COMPOUND
#घोषणा HB_WASM_EXEC_ENV_COMPOUND HB_WASM_EXEC_ENV
#पूर्ण_अगर


#अगर_अघोषित bool_t
#घोषणा bool_t uint32_t
#पूर्ण_अगर
#अगर_अघोषित ptr_t
#घोषणा ptr_t(type_t) type_t *
#पूर्ण_अगर
#अगर_अघोषित ptr_d
#घोषणा ptr_d(type_t, name) type_t *name
#पूर्ण_अगर

प्रकार uint32_t codepoint_t;
प्रकार int32_t position_t;
प्रकार uint32_t mask_t;
प्रकार uint32_t tag_t;
#घोषणा TAG(c1,c2,c3,c4) ((tag_t)((((uint32_t)(c1)&0xFF)<<24)|(((uint32_t)(c2)&0xFF)<<16)|(((uint32_t)(c3)&0xFF)<<8)|((uint32_t)(c4)&0xFF)))

प्रकार क्रमागत {
  DIRECTION_INVALID = 0,
  DIRECTION_LTR = 4,
  DIRECTION_RTL,
  DIRECTION_TTB,
  DIRECTION_BTT
} direction_t;
#घोषणा DIRECTION_IS_VALID(dir)		((((अचिन्हित पूर्णांक) (dir)) & ~3U) == 4)
#घोषणा DIRECTION_IS_HORIZONTAL(dir)	((((अचिन्हित पूर्णांक) (dir)) & ~1U) == 4)
#घोषणा DIRECTION_IS_VERTICAL(dir)	((((अचिन्हित पूर्णांक) (dir)) & ~1U) == 6)
#घोषणा DIRECTION_IS_FORWARD(dir)	((((अचिन्हित पूर्णांक) (dir)) & ~2U) == 4)
#घोषणा DIRECTION_IS_BACKWARD(dir)	((((अचिन्हित पूर्णांक) (dir)) & ~2U) == 5)
#घोषणा DIRECTION_REVERSE(dir)		((direction_t) (((अचिन्हित पूर्णांक) (dir)) ^ 1))

प्रकार tag_t script_t; /* ISO 15924 representation of Unicode scripts. */


/* common */

HB_WASM_API (direction_t, script_get_horizontal_direction) (HB_WASM_EXEC_ENV
							    script_t script);


/* blob */

प्रकार काष्ठा
{
  uint32_t लंबाई;
  ptr_t(अक्षर) तथ्य;
} blob_t;
#घोषणा BLOB_INIT {0, 0}

HB_WASM_API (व्योम, blob_free) (HB_WASM_EXEC_ENV
			       ptr_d(blob_t, blob));

/*बफर*/

प्रकार काष्ठा
{
  uint32_t codepoint;
  uint32_t mask;
  uint32_t cluster;
  uint32_t var1;
  uint32_t var2;
} glyph_info_t;

प्रकार काष्ठा
{
  position_t x_advance;
  position_t y_advance;
  position_t x_offset;
  position_t y_offset;
  uint32_t var;
} glyph_position_t;

प्रकार काष्ठा
{
  uint32_t लंबाई;
  ptr_t(glyph_info_t) info;
  ptr_t(glyph_position_t) pos;
} बफर_contents_t;
#घोषणा बफर_CONTENTS_INIT {0, 0, 0}

HB_WASM_API (bool_t, बफर_contents_realloc) (HB_WASM_EXEC_ENV
					       ptr_d(बफर_contents_t, contents),
					       uint32_t आकार);

HB_WASM_API (व्योम, बफर_contents_free) (HB_WASM_EXEC_ENV
					  ptr_d(बफर_contents_t, contents));

प्रकार काष्ठा बफर_t बफर_t;

HB_WASM_API (bool_t, बफर_copy_contents) (HB_WASM_EXEC_ENV
					    ptr_d(बफर_t, बफर),
					    ptr_d(बफर_contents_t, contents));

HB_WASM_API (bool_t, बफर_set_contents) (HB_WASM_EXEC_ENV
					   ptr_d(बफर_t, बफर),
					   ptr_d(स्थिर बफर_contents_t, contents));

HB_WASM_API (direction_t, बफर_get_direction) (HB_WASM_EXEC_ENV
						 ptr_d(बफर_t, बफर));

HB_WASM_API (script_t, बफर_get_script) (HB_WASM_EXEC_ENV
					   ptr_d(बफर_t, बफर));

HB_WASM_API (व्योम, बफर_reverse) (HB_WASM_EXEC_ENV
				    ptr_d(बफर_t, बफर));

HB_WASM_API (व्योम, बफर_reverse_clusters) (HB_WASM_EXEC_ENV
					     ptr_d(बफर_t, बफर));

/* face */

प्रकार काष्ठा face_t face_t;

HB_WASM_API (ptr_t(face_t), face_create) (HB_WASM_EXEC_ENV
					  ptr_d(blob_t, blob),
					  अचिन्हित पूर्णांक);

HB_WASM_API (bool_t, face_copy_table) (HB_WASM_EXEC_ENV
				       ptr_d(face_t, face),
				       tag_t table_tag,
				       ptr_d(blob_t, blob));

HB_WASM_API (अचिन्हित, face_get_upem) (HB_WASM_EXEC_ENV
				       ptr_d(face_t, face));

/* font */

प्रकार काष्ठा font_t font_t;

HB_WASM_API (ptr_t(font_t), font_create) (HB_WASM_EXEC_ENV
					  ptr_d(face_t, face));

HB_WASM_API (ptr_t(face_t), font_get_face) (HB_WASM_EXEC_ENV
					    ptr_d(font_t, font));

HB_WASM_API (व्योम, font_get_scale) (HB_WASM_EXEC_ENV
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

प्रकार काष्ठा
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

HB_WASM_API (व्योम, font_glyph_to_string) (HB_WASM_EXEC_ENV
					  ptr_d(font_t, font),
					  codepoint_t glyph,
					  ptr_d(अक्षर, s), uint32_t आकार);


प्रकार काष्ठा
{
  अचिन्हित पूर्णांक लंबाई;
  ptr_t(पूर्णांक) coords;
} coords_t;

HB_WASM_API (bool_t, font_copy_coords) (HB_WASM_EXEC_ENV
					  ptr_d(font_t, font),
					  ptr_d(coords_t, coords));

HB_WASM_API (bool_t, font_set_coords) (HB_WASM_EXEC_ENV
					  ptr_d(font_t, font),
					  ptr_d(coords_t, coords));

/* outline */

क्रमागत glyph_outline_point_type_t
{
  MOVE_TO,
  पंक्ति_TO,
  QUADRATIC_TO,
  CUBIC_TO,
};

प्रकार काष्ठा
{
  भग्न x;
  भग्न y;
  uint32_t type;
} glyph_outline_point_t;

प्रकार काष्ठा
{
  uint32_t n_points;
  ptr_t(glyph_outline_point_t) points;
  uint32_t n_contours;
  ptr_t(uint32_t) contours;
} glyph_outline_t;
#घोषणा GLYPH_OUTLINE_INIT {0, 0, 0, 0}

HB_WASM_API (व्योम, glyph_outline_free) (HB_WASM_EXEC_ENV
					ptr_d(glyph_outline_t, outline));

HB_WASM_API (bool_t, font_copy_glyph_outline) (HB_WASM_EXEC_ENV
					       ptr_d(font_t, font),
					       codepoint_t glyph,
					       ptr_d(glyph_outline_t, outline));


/* shape */

प्रकार काष्ठा
{
  tag_t    tag;
  uint32_t मूल्य_2;
  uint32_t start;
  uint32_t end;
} feature_t;
#घोषणा FEATURE_GLOBAL_START	0
#घोषणा FEATURE_GLOBAL_END	((uint32_t) -1)

प्रकार काष्ठा
{
  uint32_t लंबाई;
  ptr_t(feature_t) features;
} features_t;
#घोषणा FEATURES_INIT {0, 0}

HB_WASM_API (bool_t, feature_copy_contents) (HB_WASM_EXEC_ENV
					       ptr_d(स्थिर feature_t, features),
					       uint32_t अंक_features,
					       ptr_d(features_t, output));

HB_WASM_API (व्योम, features_free) (HB_WASM_EXEC_ENV
				     ptr_d(features_t, features));

HB_WASM_API (bool_t, shape_with) (HB_WASM_EXEC_ENV
				  ptr_d(font_t, font),
				  ptr_d(बफर_t, बफर),
				  ptr_d(स्थिर feature_t, features),
				  uint32_t अंक_features,
				  स्थिर अक्षर *shaper);

/* Implement these in your shaper. */

HB_WASM_INTERFACE (ptr_t(व्योम), shape_plan_create) (ptr_d(face_t, face));

HB_WASM_INTERFACE (bool_t, shape) (ptr_d(व्योम, shape_plan),
				   ptr_d(font_t, font),
				   ptr_d(बफर_t, बफर),
				   ptr_d(स्थिर feature_t, features),
				   uint32_t अंक_features);

HB_WASM_INTERFACE (व्योम, shape_plan_destroy) (ptr_d(व्योम, shape_plan));


HB_WASM_END_DECLS

#पूर्ण_अगर /* HB_WASM_API_H */
