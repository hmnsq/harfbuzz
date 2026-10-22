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

#اگر_غیر_تعریف_شدہ HB_WASM_API_FONT_HH
#تعریف_کرو HB_WASM_API_FONT_HH

#شامل "hb-wasm-api.hh"

#شامل "hb-outline.hh"

نامخلا hb {
نامخلا wasm {


HB_WASM_API (ptr_t(font_t), font_create) (HB_WASM_EXEC_ENV
					  ptr_d(face_t, face))
{
  HB_REF2OBJ (face);

  hb_font_t *font = hb_font_create (face);

  HB_OBJ2REF (font);
  واپس fontref;
}

HB_WASM_API (ptr_t(face_t), font_get_face) (HB_WASM_EXEC_ENV
					    ptr_d(font_t, font))
{
  HB_REF2OBJ (font);

  hb_face_t *face = hb_font_get_face (font);

  HB_OBJ2REF (face);
  واپس faceref;
}

HB_WASM_API (خالی, font_get_scale) (HB_WASM_EXEC_ENV
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
  واپس glyph;
}

HB_WASM_API (position_t, font_get_glyph_h_advance) (HB_WASM_EXEC_ENV
						    ptr_d(font_t, font),
						    codepoint_t glyph)
{
  HB_REF2OBJ (font);
  واپس hb_font_get_glyph_h_advance (font, glyph);
}

HB_WASM_API (position_t, font_get_glyph_v_advance) (HB_WASM_EXEC_ENV
						    ptr_d(font_t, font),
						    codepoint_t glyph)
{
  HB_REF2OBJ (font);
  واپس hb_font_get_glyph_v_advance (font, glyph);
}

static_assert (حجم (glyph_extents_t) == حجم (hb_glyph_extents_t), "");

HB_WASM_API (bool_t, font_get_glyph_extents) (HB_WASM_EXEC_ENV
					      ptr_d(font_t, font),
					      codepoint_t glyph,
					      ptr_d(glyph_extents_t, extents))
{
  HB_REF2OBJ (font);
  HB_PTR_PARAM (glyph_extents_t, extents);
  اگر (unlikely (!extents))
    واپس جھوٹ;

  واپس hb_font_get_glyph_extents (font, glyph,
				    (hb_glyph_extents_t *) extents);
}

HB_WASM_API (خالی, font_glyph_مقصد_string) (HB_WASM_EXEC_ENV
					  ptr_d(font_t, font),
					  codepoint_t glyph,
					  ptr_d(حرف, s), uint32_t سائز)
{
  HB_REF2OBJ (font);
  HB_ARRAY_PARAM (حرف, s, سائز);
  اگر (unlikely (!s))
    واپس;

  hb_font_glyph_مقصد_string (font, glyph, s, سائز);
}

static_assert (حجم (glyph_outline_نقطہ_t) == حجم (hb_outline_نقطہ_t), "");
static_assert (حجم (uint32_t) == حجم (hb_outline_t::contours[0]), "");

HB_WASM_API (bool_t, font_copy_glyph_outline) (HB_WASM_EXEC_ENV
					       ptr_d(font_t, font),
					       codepoint_t glyph,
					       ptr_d(glyph_outline_t, outline))
{
  HB_REF2OBJ (font);
  HB_PTR_PARAM (glyph_outline_t, outline);
  اگر (unlikely (!outline))
    واپس جھوٹ;

  hb_outline_t hb_outline;
  خودکار *funcs = hb_outline_recording_pen_get_funcs ();

  hb_font_draw_glyph (font, glyph, funcs, &hb_outline);

  اگر (unlikely (hb_outline.points.اندر_error () ||
		hb_outline.contours.اندر_error ()))
  {
    outline->n_points = outline->n_contours = 0;
    واپس جھوٹ;
  }

  // TODO Check two buffers separately
  اگر (hb_outline.points.لمبائی <= outline->n_points &&
      hb_outline.contours.لمبائی <= outline->n_contours)
  {
    glyph_outline_نقطہ_t *points = HB_ARRAY_APP2NATIVE (glyph_outline_نقطہ_t, outline->points, hb_outline.points.لمبائی);
    uint32_t *contours = HB_ARRAY_APP2NATIVE (uint32_t, outline->contours, hb_outline.contours.لمبائی);

    اگر (unlikely (!points || !contours))
    {
      outline->n_points = outline->n_contours = 0;
      واپس جھوٹ;
    }

    hb_memcpy (points, hb_outline.points.arrayZ, hb_outline.points.get_سائز ());
    hb_memcpy (contours, hb_outline.contours.arrayZ, hb_outline.contours.get_سائز ());

    واپس سچ;
  }

  outline->n_points = hb_outline.points.لمبائی;
  outline->points = wasm_runtime_module_dup_ڈیٹا (module_inst,
						  (ثابت حرف *) hb_outline.points.arrayZ,
						  hb_outline.points.get_سائز ());
  outline->n_contours = hb_outline.contours.لمبائی;
  outline->contours = wasm_runtime_module_dup_ڈیٹا (module_inst,
						    (ثابت حرف *) hb_outline.contours.arrayZ,
						    hb_outline.contours.get_سائز ());

  اگر ((outline->n_points && !outline->points) ||
      (outline->n_contours && !outline->contours))
  {
    outline->n_points = outline->n_contours = 0;
    واپس جھوٹ;
  }

  واپس سچ;
}

HB_WASM_API (خالی, glyph_outline_free) (HB_WASM_EXEC_ENV
					ptr_d(glyph_outline_t, outline))
{
  HB_PTR_PARAM (glyph_outline_t, outline);
  اگر (unlikely (!outline))
    واپس;

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
  اگر (unlikely (!coords))
    واپس جھوٹ;

  غیرعلامتی our_لمبائی;
  ثابت صحیح* our_coords = hb_font_get_var_coords_normalized(font, &our_لمبائی);

  اگر (our_لمبائی <= coords->لمبائی) {
    صحیح *their_coords = HB_ARRAY_APP2NATIVE (صحیح, coords->coords, our_لمبائی);
    اگر (unlikely(!their_coords)) {
    	coords->لمبائی = 0;
    	واپس جھوٹ;
    }
		غیرعلامتی بائٹیں = our_لمبائی * حجم (صحیح);
    hb_memcpy (their_coords, our_coords, بائٹیں);

    واپس سچ;
  }

  module_free (coords->coords);
  coords->لمبائی = our_لمبائی;
	غیرعلامتی بائٹیں = our_لمبائی * حجم (صحیح);
  coords->coords = wasm_runtime_module_dup_ڈیٹا (module_inst, (ثابت حرف *) our_coords, بائٹیں);
	اگر (our_لمبائی && !coords->coords)
	  {
    coords->لمبائی = 0;
    واپس جھوٹ;
  }

  واپس سچ;
}

HB_WASM_API (bool_t, font_سیٹ_coords) (HB_WASM_EXEC_ENV
					  ptr_d(font_t, font),
					  ptr_d(coords_t, coords))
{
  HB_REF2OBJ (font);
  HB_PTR_PARAM (coords_t, coords);
  اگر (unlikely (!coords))
    واپس جھوٹ;

  غیرعلامتی لمبائی = coords->لمبائی;
  غیرعلامتی بائٹیں;
  اگر (unlikely (hb_unsigned_mul_overflows (لمبائی, حجم (صحیح), &بائٹیں)))
    واپس جھوٹ;

  ثابت صحیح *our_coords = (ثابت صحیح *) (validate_app_addr (coords->coords, بائٹیں) ? addr_app_مقصد_native (coords->coords) : خالی_اشاریہ);
  اگر (unlikely (!our_coords))
    واپس جھوٹ;

  hb_font_سیٹ_var_coords_normalized(font, our_coords, لمبائی);
  واپس سچ;
}


}}

#اگر_ختم /* HB_WASM_API_FONT_HH */
