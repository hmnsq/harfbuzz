/*
 * Copyright © 2007,2008,2009  Red Hat, Inc.
 * Copyright © 2018,2019,2020  Ebrahim Byagowi
 * Copyright © 2018  Khaled Hosny
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
 *
 * Red Hat Author(s): Behdad Esfahbod
 */

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_CONFIG_H
#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot.h"

#ਅਨ੍ਤਰ੍ਭਾਵ <cassert>
#ਅਨ੍ਤਰ੍ਭਾਵ <cstdlib>
#ਅਨ੍ਤਰ੍ਭਾਵ <cstdio>
#ਅਨ੍ਤਰ੍ਭਾਵ <cstring>

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HB_NO_OPEN
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ hb_blob_create_from_file_or_fail(x)  hb_blob_get_ਖਾਲੀ_ਥਾਵਾਂ ()
#ਜੇ_ਅੰਤ

#ਜੇ !defined(HB_NO_COLOR) && !defined(HB_NO_DRAW)
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_SVG
ਸਥਿਰ ਖਾਲੀ
svg_dump (hb_face_t *face, ਬਿਨਾਨਿਸ਼ਾਨੀ face_ਸੂਚਕਾਂਕ)
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ glyph_ਗਿਣਤੀ = hb_face_get_glyph_ਗਿਣਤੀ (face);

  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ glyph_id = 0; glyph_id < glyph_ਗਿਣਤੀ; ++glyph_id)
  {
    hb_blob_t *blob = hb_ot_color_glyph_reference_svg (face, glyph_id);

    ਜੇ (hb_blob_get_ਲੰਮਾਈ (blob) == 0) ਜਾਰੀ;

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾਈ;
    ਅਚਲ ਅੱਖਰ *ਅੰਕੜੇ = hb_blob_get_ਅੰਕੜੇ (blob, &ਲੰਮਾਈ);

    ਅੱਖਰ output_ਰਸਤਾ[255];
    snprintf (output_ਰਸਤਾ, ਆਕਾਰ output_ਰਸਤਾ,
	     "out/svg-%u-%u.svg%s",
	     glyph_id,
	     face_ਸੂਚਕਾਂਕ,
	     // append "z" if the content is gzipped, https://stackoverflow.com/a/6059405
	     (ਲੰਮਾਈ > 2 && (ਅੰਕੜੇ[0] == '\x1F') && (ਅੰਕੜੇ[1] == '\x8B')) ? "z" : "");

    FILE *f = fopen (output_ਰਸਤਾ, "wb");
    fwrite (ਅੰਕੜੇ, 1, ਲੰਮਾਈ, f);
    fclose (f);

    hb_blob_destroy (blob);
  }
}
#ਜੇ_ਅੰਤ

/* _png API is so easy to use unlike the below code, don't get confused */
ਸਥਿਰ ਖਾਲੀ
png_dump (hb_face_t *face, ਬਿਨਾਨਿਸ਼ਾਨੀ face_ਸੂਚਕਾਂਕ)
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ glyph_ਗਿਣਤੀ = hb_face_get_glyph_ਗਿਣਤੀ (face);
  hb_font_t *font = hb_font_create (face);

  /* scans the font for strikes */
  ਬਿਨਾਨਿਸ਼ਾਨੀ sample_glyph_id;
  /* we don't care about different strikes for different glyphs at this point */
  ਲਈ (sample_glyph_id = 0; sample_glyph_id < glyph_ਗਿਣਤੀ; ++sample_glyph_id)
  {
    hb_blob_t *blob = hb_ot_color_glyph_reference_png (font, sample_glyph_id);
    ਬਿਨਾਨਿਸ਼ਾਨੀ blob_ਲੰਮਾਈ = hb_blob_get_ਲੰਮਾਈ (blob);
    hb_blob_destroy (blob);
    ਜੇ (blob_ਲੰਮਾਈ != 0)
      ਰੋਕੋ;
  }

  ਬਿਨਾਨਿਸ਼ਾਨੀ upem = hb_face_get_upem (face);
  ਬਿਨਾਨਿਸ਼ਾਨੀ blob_ਲੰਮਾਈ = 0;
  ਬਿਨਾਨਿਸ਼ਾਨੀ strike = 0;
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ppem = 1; ppem < upem; ++ppem)
  {
    hb_font_ਸੈੱਟ_ppem (font, ppem, ppem);
    hb_blob_t *blob = hb_ot_color_glyph_reference_png (font, sample_glyph_id);
    ਬਿਨਾਨਿਸ਼ਾਨੀ new_blob_ਲੰਮਾਈ = hb_blob_get_ਲੰਮਾਈ (blob);
    hb_blob_destroy (blob);
    ਜੇ (new_blob_ਲੰਮਾਈ != blob_ਲੰਮਾਈ)
    {
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ glyph_id = 0; glyph_id < glyph_ਗਿਣਤੀ; ++glyph_id)
      {
	hb_blob_t *blob = hb_ot_color_glyph_reference_png (font, glyph_id);

	ਜੇ (hb_blob_get_ਲੰਮਾਈ (blob) == 0) ਜਾਰੀ;

	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾਈ;
	ਅਚਲ ਅੱਖਰ *ਅੰਕੜੇ = hb_blob_get_ਅੰਕੜੇ (blob, &ਲੰਮਾਈ);

	ਅੱਖਰ output_ਰਸਤਾ[255];
	snprintf (output_ਰਸਤਾ, ਆਕਾਰ output_ਰਸਤਾ, "out/png-%u-%u-%u.png", glyph_id, strike, face_ਸੂਚਕਾਂਕ);

	FILE *f = fopen (output_ਰਸਤਾ, "wb");
	fwrite (ਅੰਕੜੇ, 1, ਲੰਮਾਈ, f);
	fclose (f);

	hb_blob_destroy (blob);
      }

      strike++;
      blob_ਲੰਮਾਈ = new_blob_ਲੰਮਾਈ;
    }
  }

  hb_font_destroy (font);
}

ਬਣਤਰ draw_ਅੰਕੜੇ_t
{
  FILE *f;
  hb_position_t ascender;
};

ਸਥਿਰ ਖਾਲੀ
move_ਮੰਜ਼ਿਲ (hb_draw_funcs_t *, draw_ਅੰਕੜੇ_t *draw_ਅੰਕੜੇ,
	 hb_draw_state_t *,
	 ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_x, ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_y,
	 ਖਾਲੀ *)
{
  fprintf (draw_ਅੰਕੜੇ->f, "M%g,%g", ਮੰਜ਼ਿਲ_x, draw_ਅੰਕੜੇ->ascender - ਮੰਜ਼ਿਲ_y);
}

ਸਥਿਰ ਖਾਲੀ
ਸਤਰ_ਮੰਜ਼ਿਲ (hb_draw_funcs_t *, draw_ਅੰਕੜੇ_t *draw_ਅੰਕੜੇ,
	 hb_draw_state_t *,
	 ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_x, ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_y,
	 ਖਾਲੀ *)
{
  fprintf (draw_ਅੰਕੜੇ->f, "L%g,%g", ਮੰਜ਼ਿਲ_x, draw_ਅੰਕੜੇ->ascender - ਮੰਜ਼ਿਲ_y);
}

ਸਥਿਰ ਖਾਲੀ
quadratic_ਮੰਜ਼ਿਲ (hb_draw_funcs_t *, draw_ਅੰਕੜੇ_t *draw_ਅੰਕੜੇ,
	      hb_draw_state_t *,
	      ਦਸ਼ਮਲਵ control_x, ਦਸ਼ਮਲਵ control_y,
	      ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_x, ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_y,
	      ਖਾਲੀ *)
{
  fprintf (draw_ਅੰਕੜੇ->f, "Q%g,%g %g,%g", control_x, draw_ਅੰਕੜੇ->ascender - control_y,
					ਮੰਜ਼ਿਲ_x, draw_ਅੰਕੜੇ->ascender - ਮੰਜ਼ਿਲ_y);
}

ਸਥਿਰ ਖਾਲੀ
cubic_ਮੰਜ਼ਿਲ (hb_draw_funcs_t *, draw_ਅੰਕੜੇ_t *draw_ਅੰਕੜੇ,
	  hb_draw_state_t *,
	  ਦਸ਼ਮਲਵ control1_x, ਦਸ਼ਮਲਵ control1_y,
	  ਦਸ਼ਮਲਵ control2_x, ਦਸ਼ਮਲਵ control2_y,
	  ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_x, ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_y,
	  ਖਾਲੀ *)
{
  fprintf (draw_ਅੰਕੜੇ->f, "C%g,%g %g,%g %g,%g", control1_x, draw_ਅੰਕੜੇ->ascender - control1_y,
					       control2_x, draw_ਅੰਕੜੇ->ascender - control2_y,
					       ਮੰਜ਼ਿਲ_x, draw_ਅੰਕੜੇ->ascender - ਮੰਜ਼ਿਲ_y);
}

ਸਥਿਰ ਖਾਲੀ
close_ਰਸਤਾ (hb_draw_funcs_t *, draw_ਅੰਕੜੇ_t *draw_ਅੰਕੜੇ,
	    hb_draw_state_t *,
	    ਖਾਲੀ *)
{
  fprintf (draw_ਅੰਕੜੇ->f, "Z");
}

ਸਥਿਰ ਖਾਲੀ
layered_glyph_dump (hb_font_t *font, hb_draw_funcs_t *funcs, ਬਿਨਾਨਿਸ਼ਾਨੀ face_ਸੂਚਕਾਂਕ)
{
  hb_face_t *face = hb_font_get_face (font);
  ਬਿਨਾਨਿਸ਼ਾਨੀ palette_ਗਿਣਤੀ = hb_ot_color_palette_get_ਗਿਣਤੀ (face);
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ palette = 0; palette < palette_ਗਿਣਤੀ; ++palette)
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_colors = hb_ot_color_palette_get_colors (face, palette, 0, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    ਜੇ (!ਅੰਕ_colors) ਜਾਰੀ;

    hb_color_t *colors = (hb_color_t*) ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ (ਅੰਕ_colors, ਆਕਾਰ (hb_color_t));
    hb_ot_color_palette_get_colors (face, palette, 0, &ਅੰਕ_colors, colors);
    ਜੇ (!ਅੰਕ_colors)
    {
      ਮੁਕਤ (colors);
      ਜਾਰੀ;
    }

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_glyphs = hb_face_get_glyph_ਗਿਣਤੀ (face);
    ਲਈ (hb_codepoint_t gid = 0; gid < ਅੰਕ_glyphs; ++gid)
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_layers = hb_ot_color_glyph_get_layers (face, gid, 0, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
      ਜੇ (!ਅੰਕ_layers) ਜਾਰੀ;

      hb_ot_color_layer_t *layers = (hb_ot_color_layer_t*) ਸਮ੍ਰਿਤੀ_ਦਿਓ (ਅੰਕ_layers * ਆਕਾਰ (hb_ot_color_layer_t));

      hb_ot_color_glyph_get_layers (face, gid, 0, &ਅੰਕ_layers, layers);
      ਜੇ (ਅੰਕ_layers)
      {
	hb_font_extents_t font_extents;
	hb_font_get_extents_for_direction (font, HB_DIRECTION_LTR, &font_extents);
	hb_glyph_extents_t extents = {0, 0, 0, 0};
	ਜੇ (!hb_font_get_glyph_extents (font, gid, &extents))
	{
	  ਲਿਖੋ ("Skip gid: %u\n", gid);
	  ਜਾਰੀ;
	}

	ਅੱਖਰ output_ਰਸਤਾ[255];
	snprintf (output_ਰਸਤਾ, ਆਕਾਰ output_ਰਸਤਾ, "out/colr-%u-%u-%u.svg", gid, palette, face_ਸੂਚਕਾਂਕ);
	FILE *f = fopen (output_ਰਸਤਾ, "wb");
	fprintf (f, "<svg xmlns=\"http://www.w3.org/2000/svg\""
		    " viewBox=\"%d %d %lld %lld\">\n",
		    extents.x_bearing, 0,
		    (ਲੰਮਾ ਲੰਮਾ) extents.x_bearing + extents.ਚੌੜਾਈ,
		    -(ਲੰਮਾ ਲੰਮਾ) extents.ਉਚਾਈ);
	draw_ਅੰਕੜੇ_t draw_ਅੰਕੜੇ;
	draw_ਅੰਕੜੇ.ascender = extents.y_bearing;
	draw_ਅੰਕੜੇ.f = f;

	ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ layer = 0; layer < ਅੰਕ_layers; ++layer)
	{
	  hb_color_t color = 0x000000FF;
	  ਜੇ (layers[layer].color_ਸੂਚਕਾਂਕ != 0xFFFF)
	    color = colors[layers[layer].color_ਸੂਚਕਾਂਕ];
	  fprintf (f, "<path fill=\"#%02X%02X%02X\" ",
		   hb_color_get_red (color), hb_color_get_green (color), hb_color_get_green (color));
	  ਜੇ (hb_color_get_alpha (color) != 255)
	    fprintf (f, "fill-opacity=\"%.3f\"", (ਦੁੱਗਣਾ) hb_color_get_alpha (color) / 255.);
	  fprintf (f, "d=\"");
	  hb_font_draw_glyph (font, layers[layer].glyph, funcs, &draw_ਅੰਕੜੇ);
	  fprintf (f, "\"/>\n");
	}

	fprintf (f, "</svg>");
	fclose (f);
      }
      ਮੁਕਤ (layers);
    }

    ਮੁਕਤ (colors);
  }
}

ਸਥਿਰ ਖਾਲੀ
dump_glyphs (hb_font_t *font, hb_draw_funcs_t *funcs, ਬਿਨਾਨਿਸ਼ਾਨੀ face_ਸੂਚਕਾਂਕ)
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_glyphs = hb_face_get_glyph_ਗਿਣਤੀ (hb_font_get_face (font));
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ gid = 0; gid < ਅੰਕ_glyphs; ++gid)
  {
    hb_font_extents_t font_extents;
    hb_font_get_extents_for_direction (font, HB_DIRECTION_LTR, &font_extents);
    hb_glyph_extents_t extents = {0, 0, 0, 0};
    ਜੇ (!hb_font_get_glyph_extents (font, gid, &extents))
    {
      ਲਿਖੋ ("Skip gid: %u\n", gid);
      ਜਾਰੀ;
    }

    ਅੱਖਰ output_ਰਸਤਾ[255];
    snprintf (output_ਰਸਤਾ, ਆਕਾਰ output_ਰਸਤਾ, "out/%u-%u.svg", face_ਸੂਚਕਾਂਕ, gid);
    FILE *f = fopen (output_ਰਸਤਾ, "wb");
    fprintf (f, "<svg xmlns=\"http://www.w3.org/2000/svg\""
		" viewBox=\"%d %d %lld %lld\"><path d=\"",
		extents.x_bearing, 0,
		(ਲੰਮਾ ਲੰਮਾ) extents.x_bearing + extents.ਚੌੜਾਈ,
		(ਲੰਮਾ ਲੰਮਾ) font_extents.ascender - font_extents.descender);
    draw_ਅੰਕੜੇ_t draw_ਅੰਕੜੇ;
    draw_ਅੰਕੜੇ.ascender = font_extents.ascender;
    draw_ਅੰਕੜੇ.f = f;
    hb_font_draw_glyph (font, gid, funcs, &draw_ਅੰਕੜੇ);
    fprintf (f, "\"/></svg>");
    fclose (f);
  }
}

ਸਥਿਰ ਖਾਲੀ
dump_glyphs (hb_blob_t *blob, ਅਚਲ ਅੱਖਰ *font_name)
{
  FILE *font_name_file = fopen ("out/.dumped_font_name", "r");
  ਜੇ (font_name_file)
  {
    fprintf (stderr, "Purge or rename ./out folder if you like to run a glyph dump,\n"
		     "run it like `rm -rf out && mkdir out && src/main font-file.ttf`\n");
    ਵਾਪਸ;
  }

  font_name_file = fopen ("out/.dumped_font_name", "w");
  ਜੇ (!font_name_file)
  {
    fprintf (stderr, "./out is not accessible as a folder, create it please\n");
    ਵਾਪਸ;
  }
  fwrite (font_name, 1, ਲੜੀ_ਲੰਬਾਈ (font_name), font_name_file);
  fclose (font_name_file);

  hb_draw_funcs_t *funcs = hb_draw_funcs_create ();
  hb_draw_funcs_ਸੈੱਟ_move_ਮੰਜ਼ਿਲ_func (funcs, (hb_draw_move_ਮੰਜ਼ਿਲ_func_t) move_ਮੰਜ਼ਿਲ, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
  hb_draw_funcs_ਸੈੱਟ_ਸਤਰ_ਮੰਜ਼ਿਲ_func (funcs, (hb_draw_ਸਤਰ_ਮੰਜ਼ਿਲ_func_t) ਸਤਰ_ਮੰਜ਼ਿਲ, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
  hb_draw_funcs_ਸੈੱਟ_quadratic_ਮੰਜ਼ਿਲ_func (funcs, (hb_draw_quadratic_ਮੰਜ਼ਿਲ_func_t) quadratic_ਮੰਜ਼ਿਲ, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
  hb_draw_funcs_ਸੈੱਟ_cubic_ਮੰਜ਼ਿਲ_func (funcs, (hb_draw_cubic_ਮੰਜ਼ਿਲ_func_t) cubic_ਮੰਜ਼ਿਲ, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
  hb_draw_funcs_ਸੈੱਟ_close_ਰਸਤਾ_func (funcs, (hb_draw_close_ਰਸਤਾ_func_t) close_ਰਸਤਾ, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_faces = hb_face_ਗਿਣਤੀ (blob);
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ face_ਸੂਚਕਾਂਕ = 0; face_ਸੂਚਕਾਂਕ < ਅੰਕ_faces; ++face_ਸੂਚਕਾਂਕ)
  {
    hb_face_t *face = hb_face_create (blob, face_ਸੂਚਕਾਂਕ);
    hb_font_t *font = hb_font_create (face);

    ਜੇ (hb_ot_color_has_png (face))
      ਲਿਖੋ ("Dumping png (CBDT/sbix)...\n");
    png_dump (face, face_ਸੂਚਕਾਂਕ);

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_SVG
    ਜੇ (hb_ot_color_has_svg (face))
      ਲਿਖੋ ("Dumping svg (SVG )...\n");
    svg_dump (face, face_ਸੂਚਕਾਂਕ);
#ਜੇ_ਅੰਤ

    ਜੇ (hb_ot_color_has_layers (face) && hb_ot_color_has_palettes (face))
      ਲਿਖੋ ("Dumping layered color glyphs (COLR/CPAL)...\n");
    layered_glyph_dump (font, funcs, face_ਸੂਚਕਾਂਕ);

    dump_glyphs (font, funcs, face_ਸੂਚਕਾਂਕ);

    hb_font_destroy (font);
    hb_face_destroy (face);
  }

  hb_draw_funcs_destroy (funcs);
}
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ MAIN_CC_NO_ਨਿੱਜੀ_API
/* Only this part of this mini app uses private API */
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-static.cc"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-open-file.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-layout-gdef-table.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-layout-gsubgpos.hh"

ਵਰਤੋਂ ਨਾਮਾਕਾਸ਼ OT;

ਸਥਿਰ ਖਾਲੀ
print_layout_info_using_ਨਿੱਜੀ_api (hb_blob_t *blob)
{
  ਅਚਲ ਅੱਖਰ *font_ਅੰਕੜੇ = hb_blob_get_ਅੰਕੜੇ (blob, ਖਾਲੀ_ਸੂਚਕ);
  hb_blob_t *font_blob = hb_sanitize_context_t ().sanitize_blob<OpenTypeFontFile> (blob);
  ਅਚਲ OpenTypeFontFile* sanitized = font_blob->as<OpenTypeFontFile> ();
  ਜੇ (!font_blob->ਅੰਕੜੇ)
  {
    ਲਿਖੋ ("Sanitization of the file wasn't successful. Exit");
    ਬਾਹਰ_ਨਿਕਲੋ (1);
  }
  ਅਚਲ OpenTypeFontFile& ot = *sanitized;

  ਚੋਣ (ot.get_ਟੈਗ ())
  {
  ਹਾਲਤ OpenTypeFontFile::True_Type_ਟੈਗ:
    ਲਿਖੋ ("OpenType font with TrueType outlines\n");
    ਰੋਕੋ;
  ਹਾਲਤ OpenTypeFontFile::CFFTag:
    ਲਿਖੋ ("OpenType font with CFF (Type1) outlines\n");
    ਰੋਕੋ;
  ਹਾਲਤ OpenTypeFontFile::TTCTag:
    ਲਿਖੋ ("TrueType Collection of OpenType fonts\n");
    ਰੋਕੋ;
  ਹਾਲਤ OpenTypeFontFile::True_ਟੈਗ:
    ਲਿਖੋ ("Obsolete Apple TrueType font\n");
    ਰੋਕੋ;
  ਹਾਲਤ OpenTypeFontFile::Typ1_ਟੈਗ:
    ਲਿਖੋ ("Obsolete Apple Type1 font in SFNT container\n");
    ਰੋਕੋ;
  ਹਾਲਤ OpenTypeFontFile::DFont_ਟੈਗ:
    ਲਿਖੋ ("DFont Mac Resource Fork\n");
    ਰੋਕੋ;
  ਮੂਲ:
    ਲਿਖੋ ("Unknown font format\n");
    ਰੋਕੋ;
  }

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_faces = hb_face_ਗਿਣਤੀ (blob);
  ਲਿਖੋ ("%u font(s) found in file\n", ਅੰਕ_faces);
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ n_font = 0; n_font < ਅੰਕ_faces; ++n_font)
  {
    ਅਚਲ OpenTypeFontFace &font = ot.get_face (n_font);
    ਲਿਖੋ ("Font %u of %u:\n", n_font, ਅੰਕ_faces);

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_tables = font.get_ਸਾਰਣੀ_ਗਿਣਤੀ ();
    ਲਿਖੋ ("  %u table(s) found in font\n", ਅੰਕ_tables);
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ n_ਸਾਰਣੀ = 0; n_ਸਾਰਣੀ < ਅੰਕ_tables; ++n_ਸਾਰਣੀ)
    {
      ਅਚਲ Open_Type_ਸਾਰਣੀ &ਸਾਰਣੀ = font.get_ਸਾਰਣੀ (n_ਸਾਰਣੀ);
      ਲਿਖੋ ("  Table %2u of %2u: %.4s (0x%08x+0x%08x)\n", n_ਸਾਰਣੀ, ਅੰਕ_tables,
	      (ਅਚਲ ਅੱਖਰ *) ਸਾਰਣੀ.ਟੈਗ_2,
	      (ਬਿਨਾਨਿਸ਼ਾਨੀ) ਸਾਰਣੀ.ਵਿੱਥ,
	      (ਬਿਨਾਨਿਸ਼ਾਨੀ) ਸਾਰਣੀ.ਲੰਮਾਈ);

      ਚੋਣ (ਸਾਰਣੀ.ਟੈਗ_2)
      {

      ਹਾਲਤ HB_OT_ਟੈਗ_GSUB:
      ਹਾਲਤ HB_OT_ਟੈਗ_GPOS:
	{

	ਅਚਲ GSUBGPOS &g = *reinterpret_cast<ਅਚਲ GSUBGPOS *> (font_ਅੰਕੜੇ + ਸਾਰਣੀ.ਵਿੱਥ);

	ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_scripts = g.get_script_ਗਿਣਤੀ ();
	ਲਿਖੋ ("    %u script(s) found in table\n", ਅੰਕ_scripts);
	ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ n_script = 0; n_script < ਅੰਕ_scripts; ++n_script)
	{
	  ਅਚਲ Script &script = g.get_script (n_script);
	  ਲਿਖੋ ("    Script %2u of %2u: %.4s\n", n_script, ਅੰਕ_scripts,
		  (ਅਚਲ ਅੱਖਰ *) g.get_script_ਟੈਗ (n_script));

	  ਜੇ (!script.has_default_lang_sys ())
	    ਲਿਖੋ ("      No default language system\n");
	  ਪੂਰਨ ਅੰਕ_langsys = script.get_lang_sys_ਗਿਣਤੀ ();
	  ਲਿਖੋ ("      %d language system(s) found in script\n", ਅੰਕ_langsys);
	  ਲਈ (ਪੂਰਨ n_langsys = script.has_default_lang_sys () ? -1 : 0; n_langsys < ਅੰਕ_langsys; ++n_langsys)
	  {
	    ਅਚਲ LangSys &langsys = n_langsys == -1
				   ? script.get_default_lang_sys ()
				   : script.get_lang_sys (n_langsys);
	    ਜੇ (n_langsys == -1)
	      ਲਿਖੋ ("      Default Language System\n");
	    ਨਹੀਂਤਾਂ
	      ਲਿਖੋ ("      Language System %2d of %2d: %.4s\n", n_langsys, ਅੰਕ_langsys,
		      (ਅਚਲ ਅੱਖਰ *) script.get_lang_sys_ਟੈਗ (n_langsys));
	    ਜੇ (!langsys.has_required_feature ())
	      ਲਿਖੋ ("        No required feature\n");
	    ਨਹੀਂਤਾਂ
	      ਲਿਖੋ ("        Required feature index: %u\n",
		      langsys.get_required_feature_ਸੂਚਕਾਂਕ ());

	    ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_features = langsys.get_feature_ਗਿਣਤੀ ();
	    ਲਿਖੋ ("        %u feature(s) found in language system\n", ਅੰਕ_features);
	    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ n_feature = 0; n_feature < ਅੰਕ_features; ++n_feature)
	    {
	      ਲਿਖੋ ("        Feature index %2u of %2u: %u\n", n_feature, ਅੰਕ_features,
		      langsys.get_feature_ਸੂਚਕਾਂਕ (n_feature));
	    }
	  }
	}

	ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_features = g.get_feature_ਗਿਣਤੀ ();
	ਲਿਖੋ ("    %u feature(s) found in table\n", ਅੰਕ_features);
	ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ n_feature = 0; n_feature < ਅੰਕ_features; ++n_feature)
	{
	  ਅਚਲ Feature &feature = g.get_feature (n_feature);
	  ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_lookups = feature.get_lookup_ਗਿਣਤੀ ();
	  ਲਿਖੋ ("    Feature %2u of %2u: %c%c%c%c\n", n_feature, ਅੰਕ_features,
		  HB_UNTAG (g.get_feature_ਟੈਗ (n_feature)));

	  ਲਿਖੋ ("        %u lookup(s) found in feature\n", ਅੰਕ_lookups);
	  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ n_lookup = 0; n_lookup < ਅੰਕ_lookups; ++n_lookup) {
	    ਲਿਖੋ ("        Lookup index %2u of %2u: %u\n", n_lookup, ਅੰਕ_lookups,
		    feature.get_lookup_ਸੂਚਕਾਂਕ (n_lookup));
	  }
	}

	ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_lookups = g.get_lookup_ਗਿਣਤੀ ();
	ਲਿਖੋ ("    %u lookup(s) found in table\n", ਅੰਕ_lookups);
	ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ n_lookup = 0; n_lookup < ਅੰਕ_lookups; ++n_lookup)
	{
	  ਅਚਲ Lookup &lookup = g.get_lookup (n_lookup);
	  ਲਿਖੋ ("    Lookup %2u of %2u: type %u, props 0x%04X\n", n_lookup, ਅੰਕ_lookups,
		  lookup.get_type (), lookup.get_props ());
	}

	}
	ਰੋਕੋ;

      ਹਾਲਤ GDEF::ਸਾਰਣੀ_ਟੈਗ:
	{

	ਅਚਲ GDEF &gdef = *reinterpret_cast<ਅਚਲ GDEF *> (font_ਅੰਕੜੇ + ਸਾਰਣੀ.ਵਿੱਥ);

	ਲਿਖੋ ("    Has %sglyph classes\n",
		  gdef.has_glyph_classes () ? "" : "no ");
	ਲਿਖੋ ("    Has %smark attachment types\n",
		  gdef.has_mark_attachment_types () ? "" : "no ");
	ਲਿਖੋ ("    Has %sattach list\n",
		  gdef.has_attach_list () ? "" : "no ");
	ਲਿਖੋ ("    Has %slig carets\n",
		  gdef.has_lig_carets () ? "" : "no ");
	ਲਿਖੋ ("    Has %smark glyph sets\n",
		  gdef.has_mark_glyph_sets () ? "" : "no ");
	ਰੋਕੋ;
	}
      }
    }
  }
}
/* end of private API use */
#ਜੇ_ਅੰਤ

ਪੂਰਨ
ਮੁੱਖ (ਪੂਰਨ argc, ਅੱਖਰ **argv)
{
  ਜੇ (argc != 2)
  {
    fprintf (stderr, "usage: %s font-file.ttf\n\n"
		     "This tools is unsupported and crashes on bad data.\nDon't use it.\n", argv[0]);
    ਬਾਹਰ_ਨਿਕਲੋ (1);
  }

  hb_blob_t *blob = hb_blob_create_from_file_or_fail (argv[1]);
  ਪ੍ਰਤਿਜ੍ਞਾ (blob);
  ਲਿਖੋ ("Opened font file %s: %u bytes long\n", argv[1], hb_blob_get_ਲੰਮਾਈ (blob));
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ MAIN_CC_NO_ਨਿੱਜੀ_API
  print_layout_info_using_ਨਿੱਜੀ_api (blob);
#ਜੇ_ਅੰਤ
#ਜੇ !defined(HB_NO_COLOR) && !defined(HB_NO_DRAW)
  dump_glyphs (blob, argv[1]);
#ਜੇ_ਅੰਤ
  hb_blob_destroy (blob);

  ਵਾਪਸ 0;
}
