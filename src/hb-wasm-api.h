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

#నిర్వచితం_కాకపోతే HB_WASM_API_H
#నిర్వచించు HB_WASM_API_H

/*
#include "hb.h"

HB_BEGIN_DECLS
HB_END_DECLS
*/

#అన్తర్భావ <stdint.h>


#నిర్వచితం_కాకపోతే HB_WASM_BEGIN_DECLS
# ifdef __cplusplus
#  define HB_WASM_BEGIN_DECLS	బాహ్య "C" {
#  define HB_WASM_చివర_DECLS	}
# లేకపోతే /* !__cplusplus */
#  define HB_WASM_BEGIN_DECLS
#  define HB_WASM_చివర_DECLS
# endif /* !__cplusplus */
#అయితే_ముగింపు


HB_WASM_BEGIN_DECLS

#నిర్వచితం_కాకపోతే HB_WASM_API
#నిర్వచించు HB_WASM_API(ret_t, name) ret_t name
#అయితే_ముగింపు
#నిర్వచితం_కాకపోతే HB_WASM_API_COMPOUND /* compound return type */
#నిర్వచించు HB_WASM_API_COMPOUND(ret_t, name) HB_WASM_API(ret_t, name)
#అయితే_ముగింపు
#నిర్వచితం_కాకపోతే HB_WASM_INTERFACE
#నిర్వచించు HB_WASM_INTERFACE(ret_t, name) ret_t name
#అయితే_ముగింపు
#నిర్వచితం_కాకపోతే HB_WASM_EXEC_ENV
#నిర్వచించు HB_WASM_EXEC_ENV
#అయితే_ముగింపు
#నిర్వచితం_కాకపోతే HB_WASM_EXEC_ENV_COMPOUND
#నిర్వచించు HB_WASM_EXEC_ENV_COMPOUND HB_WASM_EXEC_ENV
#అయితే_ముగింపు


#నిర్వచితం_కాకపోతే bool_t
#నిర్వచించు bool_t uint32_t
#అయితే_ముగింపు
#నిర్వచితం_కాకపోతే ptr_t
#నిర్వచించు ptr_t(type_t) type_t *
#అయితే_ముగింపు
#నిర్వచితం_కాకపోతే ptr_d
#నిర్వచించు ptr_d(type_t, name) type_t *name
#అయితే_ముగింపు

రకనిర్వచనం uint32_t codepoint_t;
రకనిర్వచనం int32_t position_t;
రకనిర్వచనం uint32_t ముసుగు_t;
రకనిర్వచనం uint32_t ట్యాగ్_t;
#నిర్వచించు ట్యాగ్_3(c1,c2,c3,c4) ((ట్యాగ్_t)((((uint32_t)(c1)&0xFF)<<24)|(((uint32_t)(c2)&0xFF)<<16)|(((uint32_t)(c3)&0xFF)<<8)|((uint32_t)(c4)&0xFF)))

రకనిర్వచనం గణన {
  DIRECTION_INVALID = 0,
  DIRECTION_LTR = 4,
  DIRECTION_RTL,
  DIRECTION_TTB,
  DIRECTION_BTT
} direction_t;
#నిర్వచించు DIRECTION_IS_VALID(dir)		((((గుర్తులేని పూర్ణాంకం) (dir)) & ~3U) == 4)
#నిర్వచించు DIRECTION_IS_HORIZONTAL(dir)	((((గుర్తులేని పూర్ణాంకం) (dir)) & ~1U) == 4)
#నిర్వచించు DIRECTION_IS_VERTICAL(dir)	((((గుర్తులేని పూర్ణాంకం) (dir)) & ~1U) == 6)
#నిర్వచించు DIRECTION_IS_FORWARD(dir)	((((గుర్తులేని పూర్ణాంకం) (dir)) & ~2U) == 4)
#నిర్వచించు DIRECTION_IS_BACKWARD(dir)	((((గుర్తులేని పూర్ణాంకం) (dir)) & ~2U) == 5)
#నిర్వచించు DIRECTION_తిరగేయి(dir)		((direction_t) (((గుర్తులేని పూర్ణాంకం) (dir)) ^ 1))

రకనిర్వచనం ట్యాగ్_t script_t; /* ISO 15924 representation of Unicode scripts. */


/* common */

HB_WASM_API (direction_t, script_get_horizontal_direction) (HB_WASM_EXEC_ENV
							    script_t script);


/* blob */

రకనిర్వచనం నిర్మాణం
{
  uint32_t నిడివి;
  ptr_t(అక్షరం) దత్తాంశం;
} blob_t;
#నిర్వచించు BLOB_INIT {0, 0}

HB_WASM_API (శూన్య, blob_free) (HB_WASM_EXEC_ENV
			       ptr_d(blob_t, blob));

/*బఫరు*/

రకనిర్వచనం నిర్మాణం
{
  uint32_t codepoint;
  uint32_t ముసుగు;
  uint32_t cluster;
  uint32_t var1;
  uint32_t var2;
} glyph_info_t;

రకనిర్వచనం నిర్మాణం
{
  position_t x_advance;
  position_t y_advance;
  position_t x_స్థానభ్రంశం_2;
  position_t y_స్థానభ్రంశం_2;
  uint32_t var;
} glyph_position_t;

రకనిర్వచనం నిర్మాణం
{
  uint32_t నిడివి;
  ptr_t(glyph_info_t) info;
  ptr_t(glyph_position_t) pos;
} బఫరు_contents_t;
#నిర్వచించు బఫరు_CONTENTS_INIT {0, 0, 0}

HB_WASM_API (bool_t, బఫరు_contents_realloc) (HB_WASM_EXEC_ENV
					       ptr_d(బఫరు_contents_t, contents),
					       uint32_t కొలత);

HB_WASM_API (శూన్య, బఫరు_contents_free) (HB_WASM_EXEC_ENV
					  ptr_d(బఫరు_contents_t, contents));

రకనిర్వచనం నిర్మాణం బఫరు_t బఫరు_t;

HB_WASM_API (bool_t, బఫరు_copy_contents) (HB_WASM_EXEC_ENV
					    ptr_d(బఫరు_t, బఫరు),
					    ptr_d(బఫరు_contents_t, contents));

HB_WASM_API (bool_t, బఫరు_సమితి_contents) (HB_WASM_EXEC_ENV
					   ptr_d(బఫరు_t, బఫరు),
					   ptr_d(స్థిరరాశి బఫరు_contents_t, contents));

HB_WASM_API (direction_t, బఫరు_get_direction) (HB_WASM_EXEC_ENV
						 ptr_d(బఫరు_t, బఫరు));

HB_WASM_API (script_t, బఫరు_get_script) (HB_WASM_EXEC_ENV
					   ptr_d(బఫరు_t, బఫరు));

HB_WASM_API (శూన్య, బఫరు_తిరగేయి) (HB_WASM_EXEC_ENV
				    ptr_d(బఫరు_t, బఫరు));

HB_WASM_API (శూన్య, బఫరు_తిరగేయి_clusters) (HB_WASM_EXEC_ENV
					     ptr_d(బఫరు_t, బఫరు));

/* face */

రకనిర్వచనం నిర్మాణం face_t face_t;

HB_WASM_API (ptr_t(face_t), face_create) (HB_WASM_EXEC_ENV
					  ptr_d(blob_t, blob),
					  గుర్తులేని పూర్ణాంకం);

HB_WASM_API (bool_t, face_copy_పట్టిక) (HB_WASM_EXEC_ENV
				       ptr_d(face_t, face),
				       ట్యాగ్_t పట్టిక_ట్యాగ్_2,
				       ptr_d(blob_t, blob));

HB_WASM_API (గుర్తులేని, face_get_upem) (HB_WASM_EXEC_ENV
				       ptr_d(face_t, face));

/* font */

రకనిర్వచనం నిర్మాణం font_t font_t;

HB_WASM_API (ptr_t(font_t), font_create) (HB_WASM_EXEC_ENV
					  ptr_d(face_t, face));

HB_WASM_API (ptr_t(face_t), font_get_face) (HB_WASM_EXEC_ENV
					    ptr_d(font_t, font));

HB_WASM_API (శూన్య, font_get_scale) (HB_WASM_EXEC_ENV
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

రకనిర్వచనం నిర్మాణం
{
  position_t x_bearing;
  position_t y_bearing;
  position_t వెడల్పు;
  position_t ఎత్తు;
} glyph_extents_t;

HB_WASM_API (bool_t, font_get_glyph_extents) (HB_WASM_EXEC_ENV
					      ptr_d(font_t, font),
					      codepoint_t glyph,
					      ptr_d(glyph_extents_t, extents));

HB_WASM_API (శూన్య, font_glyph_గమ్యం_string) (HB_WASM_EXEC_ENV
					  ptr_d(font_t, font),
					  codepoint_t glyph,
					  ptr_d(అక్షరం, s), uint32_t కొలత);


రకనిర్వచనం నిర్మాణం
{
  గుర్తులేని పూర్ణాంకం నిడివి;
  ptr_t(పూర్ణాంకం) coords;
} coords_t;

HB_WASM_API (bool_t, font_copy_coords) (HB_WASM_EXEC_ENV
					  ptr_d(font_t, font),
					  ptr_d(coords_t, coords));

HB_WASM_API (bool_t, font_సమితి_coords) (HB_WASM_EXEC_ENV
					  ptr_d(font_t, font),
					  ptr_d(coords_t, coords));

/* outline */

గణన glyph_outline_బిందువు_type_t
{
  MOVE_గమ్యం,
  పంక్తి_గమ్యం_2,
  QUADRATIC_గమ్యం,
  CUBIC_గమ్యం,
};

రకనిర్వచనం నిర్మాణం
{
  దశాంశం x;
  దశాంశం y;
  uint32_t type;
} glyph_outline_బిందువు_t;

రకనిర్వచనం నిర్మాణం
{
  uint32_t n_points;
  ptr_t(glyph_outline_బిందువు_t) points;
  uint32_t n_contours;
  ptr_t(uint32_t) contours;
} glyph_outline_t;
#నిర్వచించు GLYPH_OUTLINE_INIT {0, 0, 0, 0}

HB_WASM_API (శూన్య, glyph_outline_free) (HB_WASM_EXEC_ENV
					ptr_d(glyph_outline_t, outline));

HB_WASM_API (bool_t, font_copy_glyph_outline) (HB_WASM_EXEC_ENV
					       ptr_d(font_t, font),
					       codepoint_t glyph,
					       ptr_d(glyph_outline_t, outline));


/* shape */

రకనిర్వచనం నిర్మాణం
{
  ట్యాగ్_t    ట్యాగ్_2;
  uint32_t విలువ_2;
  uint32_t ప్రారంభం;
  uint32_t చివర;
} feature_t;
#నిర్వచించు FEATURE_GLOBAL_ప్రారంభం	0
#నిర్వచించు FEATURE_GLOBAL_చివర	((uint32_t) -1)

రకనిర్వచనం నిర్మాణం
{
  uint32_t నిడివి;
  ptr_t(feature_t) features;
} features_t;
#నిర్వచించు FEATURES_INIT {0, 0}

HB_WASM_API (bool_t, feature_copy_contents) (HB_WASM_EXEC_ENV
					       ptr_d(స్థిరరాశి feature_t, features),
					       uint32_t అంకె_features,
					       ptr_d(features_t, output));

HB_WASM_API (శూన్య, features_free) (HB_WASM_EXEC_ENV
				     ptr_d(features_t, features));

HB_WASM_API (bool_t, shape_with) (HB_WASM_EXEC_ENV
				  ptr_d(font_t, font),
				  ptr_d(బఫరు_t, బఫరు),
				  ptr_d(స్థిరరాశి feature_t, features),
				  uint32_t అంకె_features,
				  స్థిరరాశి అక్షరం *shaper);

/* Implement these in your shaper. */

HB_WASM_INTERFACE (ptr_t(శూన్య), shape_plan_create) (ptr_d(face_t, face));

HB_WASM_INTERFACE (bool_t, shape) (ptr_d(శూన్య, shape_plan),
				   ptr_d(font_t, font),
				   ptr_d(బఫరు_t, బఫరు),
				   ptr_d(స్థిరరాశి feature_t, features),
				   uint32_t అంకె_features);

HB_WASM_INTERFACE (శూన్య, shape_plan_destroy) (ptr_d(శూన్య, shape_plan));


HB_WASM_చివర_DECLS

#అయితే_ముగింపు /* HB_WASM_API_H */
