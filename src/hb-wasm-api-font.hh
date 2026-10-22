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

#ifndef HB_WASM_API_FONT_HH
#define HB_WASM_API_FONT_HH

#അന്തര്ഭാവ "hb-wasm-api.hh"

#അന്തര്ഭാവ "hb-outline.hh"

നാമാകാശ hb {
നാമാകാശ wasm {


HB_WASM_API (ptr_t(font_t), font_create) (HB_WASM_EXEC_ENV
					  ptr_d(face_t, face))
{
  HB_REF2OBJ (face);

  hb_font_t *font = hb_font_create (face);

  HB_OBJ2REF (font);
  തിരികെ fontref;
}

HB_WASM_API (ptr_t(face_t), font_get_face) (HB_WASM_EXEC_ENV
					    ptr_d(font_t, font))
{
  HB_REF2OBJ (font);

  hb_face_t *face = hb_font_get_face (font);

  HB_OBJ2REF (face);
  തിരികെ faceref;
}

HB_WASM_API (ശൂന്യം, font_get_scale) (HB_WASM_EXEC_ENV
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
  തിരികെ glyph;
}

HB_WASM_API (position_t, font_get_glyph_h_advance) (HB_WASM_EXEC_ENV
						    ptr_d(font_t, font),
						    codepoint_t glyph)
{
  HB_REF2OBJ (font);
  തിരികെ hb_font_get_glyph_h_advance (font, glyph);
}

HB_WASM_API (position_t, font_get_glyph_v_advance) (HB_WASM_EXEC_ENV
						    ptr_d(font_t, font),
						    codepoint_t glyph)
{
  HB_REF2OBJ (font);
  തിരികെ hb_font_get_glyph_v_advance (font, glyph);
}

static_assert (വലുപ്പം (glyph_extents_t) == വലുപ്പം (hb_glyph_extents_t), "");

HB_WASM_API (bool_t, font_get_glyph_extents) (HB_WASM_EXEC_ENV
					      ptr_d(font_t, font),
					      codepoint_t glyph,
					      ptr_d(glyph_extents_t, extents))
{
  HB_REF2OBJ (font);
  HB_PTR_PARAM (glyph_extents_t, extents);
  എങ്കിൽ (unlikely (!extents))
    തിരികെ അസത്യം;

  തിരികെ hb_font_get_glyph_extents (font, glyph,
				    (hb_glyph_extents_t *) extents);
}

HB_WASM_API (ശൂന്യം, font_glyph_to_string) (HB_WASM_EXEC_ENV
					  ptr_d(font_t, font),
					  codepoint_t glyph,
					  ptr_d(അക്ഷരം, s), uint32_t size)
{
  HB_REF2OBJ (font);
  HB_ARRAY_PARAM (അക്ഷരം, s, size);
  എങ്കിൽ (unlikely (!s))
    തിരികെ;

  hb_font_glyph_to_string (font, glyph, s, size);
}

static_assert (വലുപ്പം (glyph_outline_point_t) == വലുപ്പം (hb_outline_point_t), "");
static_assert (വലുപ്പം (uint32_t) == വലുപ്പം (hb_outline_t::contours[0]), "");

HB_WASM_API (bool_t, font_copy_glyph_outline) (HB_WASM_EXEC_ENV
					       ptr_d(font_t, font),
					       codepoint_t glyph,
					       ptr_d(glyph_outline_t, outline))
{
  HB_REF2OBJ (font);
  HB_PTR_PARAM (glyph_outline_t, outline);
  എങ്കിൽ (unlikely (!outline))
    തിരികെ അസത്യം;

  hb_outline_t hb_outline;
  auto *funcs = hb_outline_recording_pen_get_funcs ();

  hb_font_draw_glyph (font, glyph, funcs, &hb_outline);

  എങ്കിൽ (unlikely (hb_outline.points.in_error () ||
		hb_outline.contours.in_error ()))
  {
    outline->n_points = outline->n_contours = 0;
    തിരികെ അസത്യം;
  }

  // TODO Check two buffers separately
  എങ്കിൽ (hb_outline.points.length <= outline->n_points &&
      hb_outline.contours.length <= outline->n_contours)
  {
    glyph_outline_point_t *points = HB_ARRAY_APP2NATIVE (glyph_outline_point_t, outline->points, hb_outline.points.length);
    uint32_t *contours = HB_ARRAY_APP2NATIVE (uint32_t, outline->contours, hb_outline.contours.length);

    എങ്കിൽ (unlikely (!points || !contours))
    {
      outline->n_points = outline->n_contours = 0;
      തിരികെ അസത്യം;
    }

    hb_memcpy (points, hb_outline.points.arrayZ, hb_outline.points.get_size ());
    hb_memcpy (contours, hb_outline.contours.arrayZ, hb_outline.contours.get_size ());

    തിരികെ സത്യം;
  }

  outline->n_points = hb_outline.points.length;
  outline->points = wasm_runtime_module_dup_data (module_inst,
						  (സ്ഥിരം അക്ഷരം *) hb_outline.points.arrayZ,
						  hb_outline.points.get_size ());
  outline->n_contours = hb_outline.contours.length;
  outline->contours = wasm_runtime_module_dup_data (module_inst,
						    (സ്ഥിരം അക്ഷരം *) hb_outline.contours.arrayZ,
						    hb_outline.contours.get_size ());

  എങ്കിൽ ((outline->n_points && !outline->points) ||
      (outline->n_contours && !outline->contours))
  {
    outline->n_points = outline->n_contours = 0;
    തിരികെ അസത്യം;
  }

  തിരികെ സത്യം;
}

HB_WASM_API (ശൂന്യം, glyph_outline_free) (HB_WASM_EXEC_ENV
					ptr_d(glyph_outline_t, outline))
{
  HB_PTR_PARAM (glyph_outline_t, outline);
  എങ്കിൽ (unlikely (!outline))
    തിരികെ;

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
  എങ്കിൽ (unlikely (!coords))
    തിരികെ അസത്യം;

  unsigned our_length;
  സ്ഥിരം പൂർണ്ണസംഖ്യ* our_coords = hb_font_get_var_coords_normalized(font, &our_length);

  എങ്കിൽ (our_length <= coords->length) {
    പൂർണ്ണസംഖ്യ *their_coords = HB_ARRAY_APP2NATIVE (പൂർണ്ണസംഖ്യ, coords->coords, our_length);
    എങ്കിൽ (unlikely(!their_coords)) {
    	coords->length = 0;
    	തിരികെ അസത്യം;
    }
		unsigned bytes = our_length * വലുപ്പം (പൂർണ്ണസംഖ്യ);
    hb_memcpy (their_coords, our_coords, bytes);

    തിരികെ സത്യം;
  }

  module_free (coords->coords);
  coords->length = our_length;
	unsigned bytes = our_length * വലുപ്പം (പൂർണ്ണസംഖ്യ);
  coords->coords = wasm_runtime_module_dup_data (module_inst, (സ്ഥിരം അക്ഷരം *) our_coords, bytes);
	എങ്കിൽ (our_length && !coords->coords)
	  {
    coords->length = 0;
    തിരികെ അസത്യം;
  }

  തിരികെ സത്യം;
}

HB_WASM_API (bool_t, font_set_coords) (HB_WASM_EXEC_ENV
					  ptr_d(font_t, font),
					  ptr_d(coords_t, coords))
{
  HB_REF2OBJ (font);
  HB_PTR_PARAM (coords_t, coords);
  എങ്കിൽ (unlikely (!coords))
    തിരികെ അസത്യം;

  unsigned length = coords->length;
  unsigned bytes;
  എങ്കിൽ (unlikely (hb_unsigned_mul_overflows (length, വലുപ്പം (പൂർണ്ണസംഖ്യ), &bytes)))
    തിരികെ അസത്യം;

  സ്ഥിരം പൂർണ്ണസംഖ്യ *our_coords = (സ്ഥിരം പൂർണ്ണസംഖ്യ *) (validate_app_addr (coords->coords, bytes) ? addr_app_to_native (coords->coords) : nullptr);
  എങ്കിൽ (unlikely (!our_coords))
    തിരികെ അസത്യം;

  hb_font_set_var_coords_normalized(font, our_coords, length);
  തിരികെ സത്യം;
}


}}

#endif /* HB_WASM_API_FONT_HH */
