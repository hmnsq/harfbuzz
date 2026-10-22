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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_WASM_API_FONT_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_WASM_API_FONT_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-wasm-api.hh"

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-outline.hh"

ਨਾਮਾਕਾਸ਼ hb {
ਨਾਮਾਕਾਸ਼ wasm {


HB_WASM_API (ptr_t(font_t), font_create) (HB_WASM_EXEC_ENV
					  ptr_d(face_t, face))
{
  HB_REF2OBJ (face);

  hb_font_t *font = hb_font_create (face);

  HB_OBJ2REF (font);
  ਵਾਪਸ fontref;
}

HB_WASM_API (ptr_t(face_t), font_get_face) (HB_WASM_EXEC_ENV
					    ptr_d(font_t, font))
{
  HB_REF2OBJ (font);

  hb_face_t *face = hb_font_get_face (font);

  HB_OBJ2REF (face);
  ਵਾਪਸ faceref;
}

HB_WASM_API (ਖਾਲੀ, font_get_scale) (HB_WASM_EXEC_ENV
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
  ਵਾਪਸ glyph;
}

HB_WASM_API (position_t, font_get_glyph_h_advance) (HB_WASM_EXEC_ENV
						    ptr_d(font_t, font),
						    codepoint_t glyph)
{
  HB_REF2OBJ (font);
  ਵਾਪਸ hb_font_get_glyph_h_advance (font, glyph);
}

HB_WASM_API (position_t, font_get_glyph_v_advance) (HB_WASM_EXEC_ENV
						    ptr_d(font_t, font),
						    codepoint_t glyph)
{
  HB_REF2OBJ (font);
  ਵਾਪਸ hb_font_get_glyph_v_advance (font, glyph);
}

static_assert (ਆਕਾਰ (glyph_extents_t) == ਆਕਾਰ (hb_glyph_extents_t), "");

HB_WASM_API (bool_t, font_get_glyph_extents) (HB_WASM_EXEC_ENV
					      ptr_d(font_t, font),
					      codepoint_t glyph,
					      ptr_d(glyph_extents_t, extents))
{
  HB_REF2OBJ (font);
  HB_PTR_PARAM (glyph_extents_t, extents);
  ਜੇ (unlikely (!extents))
    ਵਾਪਸ ਝੂਠ;

  ਵਾਪਸ hb_font_get_glyph_extents (font, glyph,
				    (hb_glyph_extents_t *) extents);
}

HB_WASM_API (ਖਾਲੀ, font_glyph_ਮੰਜ਼ਿਲ_string) (HB_WASM_EXEC_ENV
					  ptr_d(font_t, font),
					  codepoint_t glyph,
					  ptr_d(ਅੱਖਰ, s), uint32_t ਮਾਪ)
{
  HB_REF2OBJ (font);
  HB_ARRAY_PARAM (ਅੱਖਰ, s, ਮਾਪ);
  ਜੇ (unlikely (!s))
    ਵਾਪਸ;

  hb_font_glyph_ਮੰਜ਼ਿਲ_string (font, glyph, s, ਮਾਪ);
}

static_assert (ਆਕਾਰ (glyph_outline_ਬਿੰਦੂ_t) == ਆਕਾਰ (hb_outline_ਬਿੰਦੂ_t), "");
static_assert (ਆਕਾਰ (uint32_t) == ਆਕਾਰ (hb_outline_t::contours[0]), "");

HB_WASM_API (bool_t, font_copy_glyph_outline) (HB_WASM_EXEC_ENV
					       ptr_d(font_t, font),
					       codepoint_t glyph,
					       ptr_d(glyph_outline_t, outline))
{
  HB_REF2OBJ (font);
  HB_PTR_PARAM (glyph_outline_t, outline);
  ਜੇ (unlikely (!outline))
    ਵਾਪਸ ਝੂਠ;

  hb_outline_t hb_outline;
  ਸਵੈਚਾਲਿਤ *funcs = hb_outline_recording_pen_get_funcs ();

  hb_font_draw_glyph (font, glyph, funcs, &hb_outline);

  ਜੇ (unlikely (hb_outline.points.ਵਿੱਚ_error () ||
		hb_outline.contours.ਵਿੱਚ_error ()))
  {
    outline->n_points = outline->n_contours = 0;
    ਵਾਪਸ ਝੂਠ;
  }

  // TODO Check two buffers separately
  ਜੇ (hb_outline.points.ਲੰਮਾਈ <= outline->n_points &&
      hb_outline.contours.ਲੰਮਾਈ <= outline->n_contours)
  {
    glyph_outline_ਬਿੰਦੂ_t *points = HB_ARRAY_APP2NATIVE (glyph_outline_ਬਿੰਦੂ_t, outline->points, hb_outline.points.ਲੰਮਾਈ);
    uint32_t *contours = HB_ARRAY_APP2NATIVE (uint32_t, outline->contours, hb_outline.contours.ਲੰਮਾਈ);

    ਜੇ (unlikely (!points || !contours))
    {
      outline->n_points = outline->n_contours = 0;
      ਵਾਪਸ ਝੂਠ;
    }

    hb_memcpy (points, hb_outline.points.arrayZ, hb_outline.points.get_ਮਾਪ ());
    hb_memcpy (contours, hb_outline.contours.arrayZ, hb_outline.contours.get_ਮਾਪ ());

    ਵਾਪਸ ਸੱਚ;
  }

  outline->n_points = hb_outline.points.ਲੰਮਾਈ;
  outline->points = wasm_runtime_module_dup_ਅੰਕੜੇ (module_inst,
						  (ਅਚਲ ਅੱਖਰ *) hb_outline.points.arrayZ,
						  hb_outline.points.get_ਮਾਪ ());
  outline->n_contours = hb_outline.contours.ਲੰਮਾਈ;
  outline->contours = wasm_runtime_module_dup_ਅੰਕੜੇ (module_inst,
						    (ਅਚਲ ਅੱਖਰ *) hb_outline.contours.arrayZ,
						    hb_outline.contours.get_ਮਾਪ ());

  ਜੇ ((outline->n_points && !outline->points) ||
      (outline->n_contours && !outline->contours))
  {
    outline->n_points = outline->n_contours = 0;
    ਵਾਪਸ ਝੂਠ;
  }

  ਵਾਪਸ ਸੱਚ;
}

HB_WASM_API (ਖਾਲੀ, glyph_outline_free) (HB_WASM_EXEC_ENV
					ptr_d(glyph_outline_t, outline))
{
  HB_PTR_PARAM (glyph_outline_t, outline);
  ਜੇ (unlikely (!outline))
    ਵਾਪਸ;

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
  ਜੇ (unlikely (!coords))
    ਵਾਪਸ ਝੂਠ;

  ਬਿਨਾਨਿਸ਼ਾਨੀ our_ਲੰਮਾਈ;
  ਅਚਲ ਪੂਰਨ* our_coords = hb_font_get_var_coords_normalized(font, &our_ਲੰਮਾਈ);

  ਜੇ (our_ਲੰਮਾਈ <= coords->ਲੰਮਾਈ) {
    ਪੂਰਨ *their_coords = HB_ARRAY_APP2NATIVE (ਪੂਰਨ, coords->coords, our_ਲੰਮਾਈ);
    ਜੇ (unlikely(!their_coords)) {
    	coords->ਲੰਮਾਈ = 0;
    	ਵਾਪਸ ਝੂਠ;
    }
		ਬਿਨਾਨਿਸ਼ਾਨੀ ਬਾਈਟਾਂ = our_ਲੰਮਾਈ * ਆਕਾਰ (ਪੂਰਨ);
    hb_memcpy (their_coords, our_coords, ਬਾਈਟਾਂ);

    ਵਾਪਸ ਸੱਚ;
  }

  module_free (coords->coords);
  coords->ਲੰਮਾਈ = our_ਲੰਮਾਈ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਬਾਈਟਾਂ = our_ਲੰਮਾਈ * ਆਕਾਰ (ਪੂਰਨ);
  coords->coords = wasm_runtime_module_dup_ਅੰਕੜੇ (module_inst, (ਅਚਲ ਅੱਖਰ *) our_coords, ਬਾਈਟਾਂ);
	ਜੇ (our_ਲੰਮਾਈ && !coords->coords)
	  {
    coords->ਲੰਮਾਈ = 0;
    ਵਾਪਸ ਝੂਠ;
  }

  ਵਾਪਸ ਸੱਚ;
}

HB_WASM_API (bool_t, font_ਸੈੱਟ_coords) (HB_WASM_EXEC_ENV
					  ptr_d(font_t, font),
					  ptr_d(coords_t, coords))
{
  HB_REF2OBJ (font);
  HB_PTR_PARAM (coords_t, coords);
  ਜੇ (unlikely (!coords))
    ਵਾਪਸ ਝੂਠ;

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾਈ = coords->ਲੰਮਾਈ;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਬਾਈਟਾਂ;
  ਜੇ (unlikely (hb_unsigned_mul_overflows (ਲੰਮਾਈ, ਆਕਾਰ (ਪੂਰਨ), &ਬਾਈਟਾਂ)))
    ਵਾਪਸ ਝੂਠ;

  ਅਚਲ ਪੂਰਨ *our_coords = (ਅਚਲ ਪੂਰਨ *) (validate_app_addr (coords->coords, ਬਾਈਟਾਂ) ? addr_app_ਮੰਜ਼ਿਲ_native (coords->coords) : ਖਾਲੀ_ਸੂਚਕ);
  ਜੇ (unlikely (!our_coords))
    ਵਾਪਸ ਝੂਠ;

  hb_font_ਸੈੱਟ_var_coords_normalized(font, our_coords, ਲੰਮਾਈ);
  ਵਾਪਸ ਸੱਚ;
}


}}

#ਜੇ_ਅੰਤ /* HB_WASM_API_FONT_HH */
