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

#که_نه_تعریف_شوی HB_WASM_API_FONT_HH
#تعریف_کړه HB_WASM_API_FONT_HH

#ضمن "hb-wasm-api.hh"

#ضمن "hb-outline.hh"

نطاق hb {
نطاق wasm {


HB_WASM_API (ptr_t(font_t), font_create) (HB_WASM_EXEC_ENV
					  ptr_d(face_t, face))
{
  HB_REF2OBJ (face);

  hb_font_t *font = hb_font_create (face);

  HB_OBJ2REF (font);
  أرجع fontref;
}

HB_WASM_API (ptr_t(face_t), font_get_face) (HB_WASM_EXEC_ENV
					    ptr_d(font_t, font))
{
  HB_REF2OBJ (font);

  hb_face_t *face = hb_font_get_face (font);

  HB_OBJ2REF (face);
  أرجع faceref;
}

HB_WASM_API (فراغ, font_get_scale) (HB_WASM_EXEC_ENV
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
  أرجع glyph;
}

HB_WASM_API (position_t, font_get_glyph_h_advance) (HB_WASM_EXEC_ENV
						    ptr_d(font_t, font),
						    codepoint_t glyph)
{
  HB_REF2OBJ (font);
  أرجع hb_font_get_glyph_h_advance (font, glyph);
}

HB_WASM_API (position_t, font_get_glyph_v_advance) (HB_WASM_EXEC_ENV
						    ptr_d(font_t, font),
						    codepoint_t glyph)
{
  HB_REF2OBJ (font);
  أرجع hb_font_get_glyph_v_advance (font, glyph);
}

static_assert (حجم (glyph_extents_t) == حجم (hb_glyph_extents_t), "");

HB_WASM_API (bool_t, font_get_glyph_extents) (HB_WASM_EXEC_ENV
					      ptr_d(font_t, font),
					      codepoint_t glyph,
					      ptr_d(glyph_extents_t, extents))
{
  HB_REF2OBJ (font);
  HB_PTR_PARAM (glyph_extents_t, extents);
  إذا (unlikely (!extents))
    أرجع خطأ;

  أرجع hb_font_get_glyph_extents (font, glyph,
				    (hb_glyph_extents_t *) extents);
}

HB_WASM_API (فراغ, font_glyph_منزل_string) (HB_WASM_EXEC_ENV
					  ptr_d(font_t, font),
					  codepoint_t glyph,
					  ptr_d(حرف, s), uint32_t اندازه)
{
  HB_REF2OBJ (font);
  HB_ARRAY_PARAM (حرف, s, اندازه);
  إذا (unlikely (!s))
    أرجع;

  hb_font_glyph_منزل_string (font, glyph, s, اندازه);
}

static_assert (حجم (glyph_outline_ټکی_t) == حجم (hb_outline_ټکی_t), "");
static_assert (حجم (uint32_t) == حجم (hb_outline_t::contours[0]), "");

HB_WASM_API (bool_t, font_copy_glyph_outline) (HB_WASM_EXEC_ENV
					       ptr_d(font_t, font),
					       codepoint_t glyph,
					       ptr_d(glyph_outline_t, outline))
{
  HB_REF2OBJ (font);
  HB_PTR_PARAM (glyph_outline_t, outline);
  إذا (unlikely (!outline))
    أرجع خطأ;

  hb_outline_t hb_outline;
  خپلکاری *funcs = hb_outline_recording_pen_get_funcs ();

  hb_font_draw_glyph (font, glyph, funcs, &hb_outline);

  إذا (unlikely (hb_outline.points.دننه_error () ||
		hb_outline.contours.دننه_error ()))
  {
    outline->n_points = outline->n_contours = 0;
    أرجع خطأ;
  }

  // TODO Check two buffers separately
  إذا (hb_outline.points.اوږدتیا <= outline->n_points &&
      hb_outline.contours.اوږدتیا <= outline->n_contours)
  {
    glyph_outline_ټکی_t *points = HB_ARRAY_APP2NATIVE (glyph_outline_ټکی_t, outline->points, hb_outline.points.اوږدتیا);
    uint32_t *contours = HB_ARRAY_APP2NATIVE (uint32_t, outline->contours, hb_outline.contours.اوږدتیا);

    إذا (unlikely (!points || !contours))
    {
      outline->n_points = outline->n_contours = 0;
      أرجع خطأ;
    }

    hb_memcpy (points, hb_outline.points.arrayZ, hb_outline.points.get_اندازه ());
    hb_memcpy (contours, hb_outline.contours.arrayZ, hb_outline.contours.get_اندازه ());

    أرجع ریښتیا;
  }

  outline->n_points = hb_outline.points.اوږدتیا;
  outline->points = wasm_runtime_module_dup_معلومات (module_inst,
						  (ثابت حرف *) hb_outline.points.arrayZ,
						  hb_outline.points.get_اندازه ());
  outline->n_contours = hb_outline.contours.اوږدتیا;
  outline->contours = wasm_runtime_module_dup_معلومات (module_inst,
						    (ثابت حرف *) hb_outline.contours.arrayZ,
						    hb_outline.contours.get_اندازه ());

  إذا ((outline->n_points && !outline->points) ||
      (outline->n_contours && !outline->contours))
  {
    outline->n_points = outline->n_contours = 0;
    أرجع خطأ;
  }

  أرجع ریښتیا;
}

HB_WASM_API (فراغ, glyph_outline_free) (HB_WASM_EXEC_ENV
					ptr_d(glyph_outline_t, outline))
{
  HB_PTR_PARAM (glyph_outline_t, outline);
  إذا (unlikely (!outline))
    أرجع;

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
  إذا (unlikely (!coords))
    أرجع خطأ;

  بینښه our_اوږدتیا;
  ثابت صحيح* our_coords = hb_font_get_var_coords_normalized(font, &our_اوږدتیا);

  إذا (our_اوږدتیا <= coords->اوږدتیا) {
    صحيح *their_coords = HB_ARRAY_APP2NATIVE (صحيح, coords->coords, our_اوږدتیا);
    إذا (unlikely(!their_coords)) {
    	coords->اوږدتیا = 0;
    	أرجع خطأ;
    }
		بینښه بایټونه = our_اوږدتیا * حجم (صحيح);
    hb_memcpy (their_coords, our_coords, بایټونه);

    أرجع ریښتیا;
  }

  module_free (coords->coords);
  coords->اوږدتیا = our_اوږدتیا;
	بینښه بایټونه = our_اوږدتیا * حجم (صحيح);
  coords->coords = wasm_runtime_module_dup_معلومات (module_inst, (ثابت حرف *) our_coords, بایټونه);
	إذا (our_اوږدتیا && !coords->coords)
	  {
    coords->اوږدتیا = 0;
    أرجع خطأ;
  }

  أرجع ریښتیا;
}

HB_WASM_API (bool_t, font_سېټ_coords) (HB_WASM_EXEC_ENV
					  ptr_d(font_t, font),
					  ptr_d(coords_t, coords))
{
  HB_REF2OBJ (font);
  HB_PTR_PARAM (coords_t, coords);
  إذا (unlikely (!coords))
    أرجع خطأ;

  بینښه اوږدتیا = coords->اوږدتیا;
  بینښه بایټونه;
  إذا (unlikely (hb_unsigned_mul_overflows (اوږدتیا, حجم (صحيح), &بایټونه)))
    أرجع خطأ;

  ثابت صحيح *our_coords = (ثابت صحيح *) (validate_app_addr (coords->coords, بایټونه) ? addr_app_منزل_native (coords->coords) : تش_اشاره);
  إذا (unlikely (!our_coords))
    أرجع خطأ;

  hb_font_سېټ_var_coords_normalized(font, our_coords, اوږدتیا);
  أرجع ریښتیا;
}


}}

#د_که_پای /* HB_WASM_API_FONT_HH */
