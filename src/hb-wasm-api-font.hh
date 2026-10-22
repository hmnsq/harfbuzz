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

#నిర్వచితం_కాకపోతే HB_WASM_API_FONT_HH
#నిర్వచించు HB_WASM_API_FONT_HH

#అన్తర్భావ "hb-wasm-api.hh"

#అన్తర్భావ "hb-outline.hh"

నామాకాశ hb {
నామాకాశ wasm {


HB_WASM_API (ptr_t(font_t), font_create) (HB_WASM_EXEC_ENV
					  ptr_d(face_t, face))
{
  HB_REF2OBJ (face);

  hb_font_t *font = hb_font_create (face);

  HB_OBJ2REF (font);
  తిరిగి fontref;
}

HB_WASM_API (ptr_t(face_t), font_get_face) (HB_WASM_EXEC_ENV
					    ptr_d(font_t, font))
{
  HB_REF2OBJ (font);

  hb_face_t *face = hb_font_get_face (font);

  HB_OBJ2REF (face);
  తిరిగి faceref;
}

HB_WASM_API (శూన్య, font_get_scale) (HB_WASM_EXEC_ENV
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
  తిరిగి glyph;
}

HB_WASM_API (position_t, font_get_glyph_h_advance) (HB_WASM_EXEC_ENV
						    ptr_d(font_t, font),
						    codepoint_t glyph)
{
  HB_REF2OBJ (font);
  తిరిగి hb_font_get_glyph_h_advance (font, glyph);
}

HB_WASM_API (position_t, font_get_glyph_v_advance) (HB_WASM_EXEC_ENV
						    ptr_d(font_t, font),
						    codepoint_t glyph)
{
  HB_REF2OBJ (font);
  తిరిగి hb_font_get_glyph_v_advance (font, glyph);
}

static_assert (పరిమాణం (glyph_extents_t) == పరిమాణం (hb_glyph_extents_t), "");

HB_WASM_API (bool_t, font_get_glyph_extents) (HB_WASM_EXEC_ENV
					      ptr_d(font_t, font),
					      codepoint_t glyph,
					      ptr_d(glyph_extents_t, extents))
{
  HB_REF2OBJ (font);
  HB_PTR_PARAM (glyph_extents_t, extents);
  ఒకవేళ (unlikely (!extents))
    తిరిగి అబద్ధం;

  తిరిగి hb_font_get_glyph_extents (font, glyph,
				    (hb_glyph_extents_t *) extents);
}

HB_WASM_API (శూన్య, font_glyph_గమ్యం_string) (HB_WASM_EXEC_ENV
					  ptr_d(font_t, font),
					  codepoint_t glyph,
					  ptr_d(అక్షరం, s), uint32_t కొలత)
{
  HB_REF2OBJ (font);
  HB_ARRAY_PARAM (అక్షరం, s, కొలత);
  ఒకవేళ (unlikely (!s))
    తిరిగి;

  hb_font_glyph_గమ్యం_string (font, glyph, s, కొలత);
}

static_assert (పరిమాణం (glyph_outline_బిందువు_t) == పరిమాణం (hb_outline_బిందువు_t), "");
static_assert (పరిమాణం (uint32_t) == పరిమాణం (hb_outline_t::contours[0]), "");

HB_WASM_API (bool_t, font_copy_glyph_outline) (HB_WASM_EXEC_ENV
					       ptr_d(font_t, font),
					       codepoint_t glyph,
					       ptr_d(glyph_outline_t, outline))
{
  HB_REF2OBJ (font);
  HB_PTR_PARAM (glyph_outline_t, outline);
  ఒకవేళ (unlikely (!outline))
    తిరిగి అబద్ధం;

  hb_outline_t hb_outline;
  స్వయంచాలక *funcs = hb_outline_recording_pen_get_funcs ();

  hb_font_draw_glyph (font, glyph, funcs, &hb_outline);

  ఒకవేళ (unlikely (hb_outline.points.లోపల_error () ||
		hb_outline.contours.లోపల_error ()))
  {
    outline->n_points = outline->n_contours = 0;
    తిరిగి అబద్ధం;
  }

  // TODO Check two buffers separately
  ఒకవేళ (hb_outline.points.నిడివి <= outline->n_points &&
      hb_outline.contours.నిడివి <= outline->n_contours)
  {
    glyph_outline_బిందువు_t *points = HB_ARRAY_APP2NATIVE (glyph_outline_బిందువు_t, outline->points, hb_outline.points.నిడివి);
    uint32_t *contours = HB_ARRAY_APP2NATIVE (uint32_t, outline->contours, hb_outline.contours.నిడివి);

    ఒకవేళ (unlikely (!points || !contours))
    {
      outline->n_points = outline->n_contours = 0;
      తిరిగి అబద్ధం;
    }

    hb_memcpy (points, hb_outline.points.arrayZ, hb_outline.points.get_కొలత ());
    hb_memcpy (contours, hb_outline.contours.arrayZ, hb_outline.contours.get_కొలత ());

    తిరిగి నిజం;
  }

  outline->n_points = hb_outline.points.నిడివి;
  outline->points = wasm_runtime_module_dup_దత్తాంశం (module_inst,
						  (స్థిరరాశి అక్షరం *) hb_outline.points.arrayZ,
						  hb_outline.points.get_కొలత ());
  outline->n_contours = hb_outline.contours.నిడివి;
  outline->contours = wasm_runtime_module_dup_దత్తాంశం (module_inst,
						    (స్థిరరాశి అక్షరం *) hb_outline.contours.arrayZ,
						    hb_outline.contours.get_కొలత ());

  ఒకవేళ ((outline->n_points && !outline->points) ||
      (outline->n_contours && !outline->contours))
  {
    outline->n_points = outline->n_contours = 0;
    తిరిగి అబద్ధం;
  }

  తిరిగి నిజం;
}

HB_WASM_API (శూన్య, glyph_outline_free) (HB_WASM_EXEC_ENV
					ptr_d(glyph_outline_t, outline))
{
  HB_PTR_PARAM (glyph_outline_t, outline);
  ఒకవేళ (unlikely (!outline))
    తిరిగి;

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
  ఒకవేళ (unlikely (!coords))
    తిరిగి అబద్ధం;

  గుర్తులేని our_నిడివి;
  స్థిరరాశి పూర్ణాంకం* our_coords = hb_font_get_var_coords_normalized(font, &our_నిడివి);

  ఒకవేళ (our_నిడివి <= coords->నిడివి) {
    పూర్ణాంకం *their_coords = HB_ARRAY_APP2NATIVE (పూర్ణాంకం, coords->coords, our_నిడివి);
    ఒకవేళ (unlikely(!their_coords)) {
    	coords->నిడివి = 0;
    	తిరిగి అబద్ధం;
    }
		గుర్తులేని బైట్లు = our_నిడివి * పరిమాణం (పూర్ణాంకం);
    hb_memcpy (their_coords, our_coords, బైట్లు);

    తిరిగి నిజం;
  }

  module_free (coords->coords);
  coords->నిడివి = our_నిడివి;
	గుర్తులేని బైట్లు = our_నిడివి * పరిమాణం (పూర్ణాంకం);
  coords->coords = wasm_runtime_module_dup_దత్తాంశం (module_inst, (స్థిరరాశి అక్షరం *) our_coords, బైట్లు);
	ఒకవేళ (our_నిడివి && !coords->coords)
	  {
    coords->నిడివి = 0;
    తిరిగి అబద్ధం;
  }

  తిరిగి నిజం;
}

HB_WASM_API (bool_t, font_సమితి_coords) (HB_WASM_EXEC_ENV
					  ptr_d(font_t, font),
					  ptr_d(coords_t, coords))
{
  HB_REF2OBJ (font);
  HB_PTR_PARAM (coords_t, coords);
  ఒకవేళ (unlikely (!coords))
    తిరిగి అబద్ధం;

  గుర్తులేని నిడివి = coords->నిడివి;
  గుర్తులేని బైట్లు;
  ఒకవేళ (unlikely (hb_unsigned_mul_overflows (నిడివి, పరిమాణం (పూర్ణాంకం), &బైట్లు)))
    తిరిగి అబద్ధం;

  స్థిరరాశి పూర్ణాంకం *our_coords = (స్థిరరాశి పూర్ణాంకం *) (validate_app_addr (coords->coords, బైట్లు) ? addr_app_గమ్యం_native (coords->coords) : శూన్యసూచిక);
  ఒకవేళ (unlikely (!our_coords))
    తిరిగి అబద్ధం;

  hb_font_సమితి_var_coords_normalized(font, our_coords, నిడివి);
  తిరిగి నిజం;
}


}}

#అయితే_ముగింపు /* HB_WASM_API_FONT_HH */
