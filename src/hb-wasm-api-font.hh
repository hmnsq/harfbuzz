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

#अगर_अघोषित HB_WASM_API_FONT_HH
#घोषणा HB_WASM_API_FONT_HH

#समावेश "hb-wasm-api.hh"

#समावेश "hb-outline.hh"

namespace hb {
namespace wasm {


HB_WASM_API (ptr_t(font_t), font_create) (HB_WASM_EXEC_ENV
					  ptr_d(face_t, face))
{
  HB_REF2OBJ (face);

  hb_font_t *font = hb_font_create (face);

  HB_OBJ2REF (font);
  वापस fontref;
}

HB_WASM_API (ptr_t(face_t), font_get_face) (HB_WASM_EXEC_ENV
					    ptr_d(font_t, font))
{
  HB_REF2OBJ (font);

  hb_face_t *face = hb_font_get_face (font);

  HB_OBJ2REF (face);
  वापस faceref;
}

HB_WASM_API (व्योम, font_get_scale) (HB_WASM_EXEC_ENV
				    ptr_d(font_t, font),
				    ptr_d(int32_t, x_scale),
				    ptr_d(int32_t, y_scale))
{
  HB_REF2OBJ (font);

  HB_PTR_PARAM(int32_t, x_scale);
  HB_PTR_PARAM(int32_t, y_scale);

  hb_font_get_scale (font, x_scale, y_scale);
}

HB_WASM_API (codepoint_t, font_get_glyph) (HB_WASM_EXEC_ENV
					      ptr_d(font_t, font),
					      codepoint_t unicode,
					      codepoint_t variation_selector)
{
  HB_REF2OBJ (font);
  codepoint_t glyph;

  hb_font_get_glyph (font, unicode, variation_selector, &glyph);
  वापस glyph;
}

HB_WASM_API (position_t, font_get_glyph_h_advance) (HB_WASM_EXEC_ENV
						    ptr_d(font_t, font),
						    codepoint_t glyph)
{
  HB_REF2OBJ (font);
  वापस hb_font_get_glyph_h_advance (font, glyph);
}

HB_WASM_API (position_t, font_get_glyph_v_advance) (HB_WASM_EXEC_ENV
						    ptr_d(font_t, font),
						    codepoint_t glyph)
{
  HB_REF2OBJ (font);
  वापस hb_font_get_glyph_v_advance (font, glyph);
}

static_assert (माप (glyph_extents_t) == माप (hb_glyph_extents_t), "");

HB_WASM_API (bool_t, font_get_glyph_extents) (HB_WASM_EXEC_ENV
					      ptr_d(font_t, font),
					      codepoint_t glyph,
					      ptr_d(glyph_extents_t, extents))
{
  HB_REF2OBJ (font);
  HB_PTR_PARAM (glyph_extents_t, extents);
  अगर (unlikely (!extents))
    वापस false;

  वापस hb_font_get_glyph_extents (font, glyph,
				    (hb_glyph_extents_t *) extents);
}

HB_WASM_API (व्योम, font_glyph_to_string) (HB_WASM_EXEC_ENV
					  ptr_d(font_t, font),
					  codepoint_t glyph,
					  ptr_d(अक्षर, s), uint32_t आकार)
{
  HB_REF2OBJ (font);
  HB_ARRAY_PARAM (अक्षर, s, आकार);
  अगर (unlikely (!s))
    वापस;

  hb_font_glyph_to_string (font, glyph, s, आकार);
}

static_assert (माप (glyph_outline_point_t) == माप (hb_outline_point_t), "");
static_assert (माप (uint32_t) == माप (hb_outline_t::contours[0]), "");

HB_WASM_API (bool_t, font_copy_glyph_outline) (HB_WASM_EXEC_ENV
					       ptr_d(font_t, font),
					       codepoint_t glyph,
					       ptr_d(glyph_outline_t, outline))
{
  HB_REF2OBJ (font);
  HB_PTR_PARAM (glyph_outline_t, outline);
  अगर (unlikely (!outline))
    वापस false;

  hb_outline_t hb_outline;
  स्वतः *funcs = hb_outline_recording_pen_get_funcs ();

  hb_font_draw_glyph (font, glyph, funcs, &hb_outline);

  अगर (unlikely (hb_outline.points.in_error () ||
		hb_outline.contours.in_error ()))
  {
    outline->n_points = outline->n_contours = 0;
    वापस false;
  }

  // TODO Check two buffers separately
  अगर (hb_outline.points.लंबाई <= outline->n_points &&
      hb_outline.contours.लंबाई <= outline->n_contours)
  {
    glyph_outline_point_t *points = HB_ARRAY_APP2NATIVE (glyph_outline_point_t, outline->points, hb_outline.points.लंबाई);
    uint32_t *contours = HB_ARRAY_APP2NATIVE (uint32_t, outline->contours, hb_outline.contours.लंबाई);

    अगर (unlikely (!points || !contours))
    {
      outline->n_points = outline->n_contours = 0;
      वापस false;
    }

    hb_memcpy (points, hb_outline.points.arrayZ, hb_outline.points.get_आकार ());
    hb_memcpy (contours, hb_outline.contours.arrayZ, hb_outline.contours.get_आकार ());

    वापस true;
  }

  outline->n_points = hb_outline.points.लंबाई;
  outline->points = wasm_runtime_module_dup_तथ्य (module_inst,
						  (स्थिर अक्षर *) hb_outline.points.arrayZ,
						  hb_outline.points.get_आकार ());
  outline->n_contours = hb_outline.contours.लंबाई;
  outline->contours = wasm_runtime_module_dup_तथ्य (module_inst,
						    (स्थिर अक्षर *) hb_outline.contours.arrayZ,
						    hb_outline.contours.get_आकार ());

  अगर ((outline->n_points && !outline->points) ||
      (outline->n_contours && !outline->contours))
  {
    outline->n_points = outline->n_contours = 0;
    वापस false;
  }

  वापस true;
}

HB_WASM_API (व्योम, glyph_outline_free) (HB_WASM_EXEC_ENV
					ptr_d(glyph_outline_t, outline))
{
  HB_PTR_PARAM (glyph_outline_t, outline);
  अगर (unlikely (!outline))
    वापस;

  module_free (outline->points);
  module_free (outline->contours);

  outline->n_points = 0;
  outline->points = nullref;
  outline->n_contours = 0;
  outline->contours = nullref;
}

HB_WASM_API (bool_t, font_copy_coords) (HB_WASM_EXEC_ENV
					  ptr_d(font_t, font),
					  ptr_d(coords_t, coords))
{
  HB_REF2OBJ (font);
  HB_PTR_PARAM (coords_t, coords);
  अगर (unlikely (!coords))
    वापस false;

  अचिन्हित our_लंबाई;
  स्थिर पूर्णांक* our_coords = hb_font_get_var_coords_normalized(font, &our_लंबाई);

  अगर (our_लंबाई <= coords->लंबाई) {
    पूर्णांक *their_coords = HB_ARRAY_APP2NATIVE (पूर्णांक, coords->coords, our_लंबाई);
    अगर (unlikely(!their_coords)) {
    	coords->लंबाई = 0;
    	वापस false;
    }
		अचिन्हित bytes = our_लंबाई * माप (पूर्णांक);
    hb_memcpy (their_coords, our_coords, bytes);

    वापस true;
  }

  module_free (coords->coords);
  coords->लंबाई = our_लंबाई;
	अचिन्हित bytes = our_लंबाई * माप (पूर्णांक);
  coords->coords = wasm_runtime_module_dup_तथ्य (module_inst, (स्थिर अक्षर *) our_coords, bytes);
	अगर (our_लंबाई && !coords->coords)
	  {
    coords->लंबाई = 0;
    वापस false;
  }

  वापस true;
}

HB_WASM_API (bool_t, font_set_coords) (HB_WASM_EXEC_ENV
					  ptr_d(font_t, font),
					  ptr_d(coords_t, coords))
{
  HB_REF2OBJ (font);
  HB_PTR_PARAM (coords_t, coords);
  अगर (unlikely (!coords))
    वापस false;

  अचिन्हित लंबाई = coords->लंबाई;
  अचिन्हित bytes;
  अगर (unlikely (hb_unsigned_mul_overflows (लंबाई, माप (पूर्णांक), &bytes)))
    वापस false;

  स्थिर पूर्णांक *our_coords = (स्थिर पूर्णांक *) (validate_app_addr (coords->coords, bytes) ? addr_app_to_native (coords->coords) : nullptr);
  अगर (unlikely (!our_coords))
    वापस false;

  hb_font_set_var_coords_normalized(font, our_coords, लंबाई);
  वापस true;
}


}}

#पूर्ण_अगर /* HB_WASM_API_FONT_HH */
