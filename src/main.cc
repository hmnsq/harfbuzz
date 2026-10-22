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

#నిర్వచితమైతే HAVE_CONFIG_H
#అన్తర్భావ "config.h"
#అయితే_ముగింపు

#అన్తర్భావ "hb.h"
#అన్తర్భావ "hb-ot.h"

#అన్తర్భావ <cassert>
#అన్తర్భావ <cstdlib>
#అన్తర్భావ <cstdio>
#అన్తర్భావ <cstring>

#నిర్వచితమైతే HB_NO_OPEN
#నిర్వచించు hb_blob_create_from_file_or_fail(x)  hb_blob_get_ఖాళీలు ()
#అయితే_ముగింపు

#ఒకవేళ !defined(HB_NO_COLOR) && !defined(HB_NO_DRAW)
#నిర్వచితం_కాకపోతే HB_NO_SVG
స్థిర శూన్య
svg_dump (hb_face_t *face, గుర్తులేని face_సూచీ)
{
  గుర్తులేని glyph_లెక్క = hb_face_get_glyph_లెక్క (face);

  కొరకు (గుర్తులేని glyph_id = 0; glyph_id < glyph_లెక్క; ++glyph_id)
  {
    hb_blob_t *blob = hb_ot_color_glyph_reference_svg (face, glyph_id);

    ఒకవేళ (hb_blob_get_నిడివి (blob) == 0) కొనసాగు;

    గుర్తులేని నిడివి;
    స్థిరరాశి అక్షరం *దత్తాంశం = hb_blob_get_దత్తాంశం (blob, &నిడివి);

    అక్షరం output_మార్గం[255];
    snprintf (output_మార్గం, పరిమాణం output_మార్గం,
	     "out/svg-%u-%u.svg%s",
	     glyph_id,
	     face_సూచీ,
	     // append "z" if the content is gzipped, https://stackoverflow.com/a/6059405
	     (నిడివి > 2 && (దత్తాంశం[0] == '\x1F') && (దత్తాంశం[1] == '\x8B')) ? "z" : "");

    FILE *f = fopen (output_మార్గం, "wb");
    fwrite (దత్తాంశం, 1, నిడివి, f);
    fclose (f);

    hb_blob_destroy (blob);
  }
}
#అయితే_ముగింపు

/* _png API is so easy to use unlike the below code, don't get confused */
స్థిర శూన్య
png_dump (hb_face_t *face, గుర్తులేని face_సూచీ)
{
  గుర్తులేని glyph_లెక్క = hb_face_get_glyph_లెక్క (face);
  hb_font_t *font = hb_font_create (face);

  /* scans the font for strikes */
  గుర్తులేని sample_glyph_id;
  /* we don't care about different strikes for different glyphs at this point */
  కొరకు (sample_glyph_id = 0; sample_glyph_id < glyph_లెక్క; ++sample_glyph_id)
  {
    hb_blob_t *blob = hb_ot_color_glyph_reference_png (font, sample_glyph_id);
    గుర్తులేని blob_నిడివి = hb_blob_get_నిడివి (blob);
    hb_blob_destroy (blob);
    ఒకవేళ (blob_నిడివి != 0)
      ఆపు;
  }

  గుర్తులేని upem = hb_face_get_upem (face);
  గుర్తులేని blob_నిడివి = 0;
  గుర్తులేని strike = 0;
  కొరకు (గుర్తులేని ppem = 1; ppem < upem; ++ppem)
  {
    hb_font_సమితి_ppem (font, ppem, ppem);
    hb_blob_t *blob = hb_ot_color_glyph_reference_png (font, sample_glyph_id);
    గుర్తులేని new_blob_నిడివి = hb_blob_get_నిడివి (blob);
    hb_blob_destroy (blob);
    ఒకవేళ (new_blob_నిడివి != blob_నిడివి)
    {
      కొరకు (గుర్తులేని glyph_id = 0; glyph_id < glyph_లెక్క; ++glyph_id)
      {
	hb_blob_t *blob = hb_ot_color_glyph_reference_png (font, glyph_id);

	ఒకవేళ (hb_blob_get_నిడివి (blob) == 0) కొనసాగు;

	గుర్తులేని నిడివి;
	స్థిరరాశి అక్షరం *దత్తాంశం = hb_blob_get_దత్తాంశం (blob, &నిడివి);

	అక్షరం output_మార్గం[255];
	snprintf (output_మార్గం, పరిమాణం output_మార్గం, "out/png-%u-%u-%u.png", glyph_id, strike, face_సూచీ);

	FILE *f = fopen (output_మార్గం, "wb");
	fwrite (దత్తాంశం, 1, నిడివి, f);
	fclose (f);

	hb_blob_destroy (blob);
      }

      strike++;
      blob_నిడివి = new_blob_నిడివి;
    }
  }

  hb_font_destroy (font);
}

నిర్మాణం draw_దత్తాంశం_t
{
  FILE *f;
  hb_position_t ascender;
};

స్థిర శూన్య
move_గమ్యం (hb_draw_funcs_t *, draw_దత్తాంశం_t *draw_దత్తాంశం,
	 hb_draw_state_t *,
	 దశాంశం గమ్యం_x, దశాంశం గమ్యం_y,
	 శూన్య *)
{
  fprintf (draw_దత్తాంశం->f, "M%g,%g", గమ్యం_x, draw_దత్తాంశం->ascender - గమ్యం_y);
}

స్థిర శూన్య
పంక్తి_గమ్యం (hb_draw_funcs_t *, draw_దత్తాంశం_t *draw_దత్తాంశం,
	 hb_draw_state_t *,
	 దశాంశం గమ్యం_x, దశాంశం గమ్యం_y,
	 శూన్య *)
{
  fprintf (draw_దత్తాంశం->f, "L%g,%g", గమ్యం_x, draw_దత్తాంశం->ascender - గమ్యం_y);
}

స్థిర శూన్య
quadratic_గమ్యం (hb_draw_funcs_t *, draw_దత్తాంశం_t *draw_దత్తాంశం,
	      hb_draw_state_t *,
	      దశాంశం control_x, దశాంశం control_y,
	      దశాంశం గమ్యం_x, దశాంశం గమ్యం_y,
	      శూన్య *)
{
  fprintf (draw_దత్తాంశం->f, "Q%g,%g %g,%g", control_x, draw_దత్తాంశం->ascender - control_y,
					గమ్యం_x, draw_దత్తాంశం->ascender - గమ్యం_y);
}

స్థిర శూన్య
cubic_గమ్యం (hb_draw_funcs_t *, draw_దత్తాంశం_t *draw_దత్తాంశం,
	  hb_draw_state_t *,
	  దశాంశం control1_x, దశాంశం control1_y,
	  దశాంశం control2_x, దశాంశం control2_y,
	  దశాంశం గమ్యం_x, దశాంశం గమ్యం_y,
	  శూన్య *)
{
  fprintf (draw_దత్తాంశం->f, "C%g,%g %g,%g %g,%g", control1_x, draw_దత్తాంశం->ascender - control1_y,
					       control2_x, draw_దత్తాంశం->ascender - control2_y,
					       గమ్యం_x, draw_దత్తాంశం->ascender - గమ్యం_y);
}

స్థిర శూన్య
close_మార్గం (hb_draw_funcs_t *, draw_దత్తాంశం_t *draw_దత్తాంశం,
	    hb_draw_state_t *,
	    శూన్య *)
{
  fprintf (draw_దత్తాంశం->f, "Z");
}

స్థిర శూన్య
layered_glyph_dump (hb_font_t *font, hb_draw_funcs_t *funcs, గుర్తులేని face_సూచీ)
{
  hb_face_t *face = hb_font_get_face (font);
  గుర్తులేని palette_లెక్క = hb_ot_color_palette_get_లెక్క (face);
  కొరకు (గుర్తులేని palette = 0; palette < palette_లెక్క; ++palette)
  {
    గుర్తులేని అంకె_colors = hb_ot_color_palette_get_colors (face, palette, 0, శూన్యసూచిక, శూన్యసూచిక);
    ఒకవేళ (!అంకె_colors) కొనసాగు;

    hb_color_t *colors = (hb_color_t*) సున్నా_జ్ఞాపకం_ఇవ్వు (అంకె_colors, పరిమాణం (hb_color_t));
    hb_ot_color_palette_get_colors (face, palette, 0, &అంకె_colors, colors);
    ఒకవేళ (!అంకె_colors)
    {
      విడుదల (colors);
      కొనసాగు;
    }

    గుర్తులేని అంకె_glyphs = hb_face_get_glyph_లెక్క (face);
    కొరకు (hb_codepoint_t gid = 0; gid < అంకె_glyphs; ++gid)
    {
      గుర్తులేని అంకె_layers = hb_ot_color_glyph_get_layers (face, gid, 0, శూన్యసూచిక, శూన్యసూచిక);
      ఒకవేళ (!అంకె_layers) కొనసాగు;

      hb_ot_color_layer_t *layers = (hb_ot_color_layer_t*) జ్ఞాపకం_ఇవ్వు (అంకె_layers * పరిమాణం (hb_ot_color_layer_t));

      hb_ot_color_glyph_get_layers (face, gid, 0, &అంకె_layers, layers);
      ఒకవేళ (అంకె_layers)
      {
	hb_font_extents_t font_extents;
	hb_font_get_extents_for_direction (font, HB_DIRECTION_LTR, &font_extents);
	hb_glyph_extents_t extents = {0, 0, 0, 0};
	ఒకవేళ (!hb_font_get_glyph_extents (font, gid, &extents))
	{
	  రాయి ("Skip gid: %u\n", gid);
	  కొనసాగు;
	}

	అక్షరం output_మార్గం[255];
	snprintf (output_మార్గం, పరిమాణం output_మార్గం, "out/colr-%u-%u-%u.svg", gid, palette, face_సూచీ);
	FILE *f = fopen (output_మార్గం, "wb");
	fprintf (f, "<svg xmlns=\"http://www.w3.org/2000/svg\""
		    " viewBox=\"%d %d %lld %lld\">\n",
		    extents.x_bearing, 0,
		    (దీర్ఘ దీర్ఘ) extents.x_bearing + extents.వెడల్పు,
		    -(దీర్ఘ దీర్ఘ) extents.ఎత్తు);
	draw_దత్తాంశం_t draw_దత్తాంశం;
	draw_దత్తాంశం.ascender = extents.y_bearing;
	draw_దత్తాంశం.f = f;

	కొరకు (గుర్తులేని layer = 0; layer < అంకె_layers; ++layer)
	{
	  hb_color_t color = 0x000000FF;
	  ఒకవేళ (layers[layer].color_సూచీ != 0xFFFF)
	    color = colors[layers[layer].color_సూచీ];
	  fprintf (f, "<path fill=\"#%02X%02X%02X\" ",
		   hb_color_get_red (color), hb_color_get_green (color), hb_color_get_green (color));
	  ఒకవేళ (hb_color_get_alpha (color) != 255)
	    fprintf (f, "fill-opacity=\"%.3f\"", (ద్విగుణ) hb_color_get_alpha (color) / 255.);
	  fprintf (f, "d=\"");
	  hb_font_draw_glyph (font, layers[layer].glyph, funcs, &draw_దత్తాంశం);
	  fprintf (f, "\"/>\n");
	}

	fprintf (f, "</svg>");
	fclose (f);
      }
      విడుదల (layers);
    }

    విడుదల (colors);
  }
}

స్థిర శూన్య
dump_glyphs (hb_font_t *font, hb_draw_funcs_t *funcs, గుర్తులేని face_సూచీ)
{
  గుర్తులేని అంకె_glyphs = hb_face_get_glyph_లెక్క (hb_font_get_face (font));
  కొరకు (గుర్తులేని gid = 0; gid < అంకె_glyphs; ++gid)
  {
    hb_font_extents_t font_extents;
    hb_font_get_extents_for_direction (font, HB_DIRECTION_LTR, &font_extents);
    hb_glyph_extents_t extents = {0, 0, 0, 0};
    ఒకవేళ (!hb_font_get_glyph_extents (font, gid, &extents))
    {
      రాయి ("Skip gid: %u\n", gid);
      కొనసాగు;
    }

    అక్షరం output_మార్గం[255];
    snprintf (output_మార్గం, పరిమాణం output_మార్గం, "out/%u-%u.svg", face_సూచీ, gid);
    FILE *f = fopen (output_మార్గం, "wb");
    fprintf (f, "<svg xmlns=\"http://www.w3.org/2000/svg\""
		" viewBox=\"%d %d %lld %lld\"><path d=\"",
		extents.x_bearing, 0,
		(దీర్ఘ దీర్ఘ) extents.x_bearing + extents.వెడల్పు,
		(దీర్ఘ దీర్ఘ) font_extents.ascender - font_extents.descender);
    draw_దత్తాంశం_t draw_దత్తాంశం;
    draw_దత్తాంశం.ascender = font_extents.ascender;
    draw_దత్తాంశం.f = f;
    hb_font_draw_glyph (font, gid, funcs, &draw_దత్తాంశం);
    fprintf (f, "\"/></svg>");
    fclose (f);
  }
}

స్థిర శూన్య
dump_glyphs (hb_blob_t *blob, స్థిరరాశి అక్షరం *font_name)
{
  FILE *font_name_file = fopen ("out/.dumped_font_name", "r");
  ఒకవేళ (font_name_file)
  {
    fprintf (stderr, "Purge or rename ./out folder if you like to run a glyph dump,\n"
		     "run it like `rm -rf out && mkdir out && src/main font-file.ttf`\n");
    తిరిగి;
  }

  font_name_file = fopen ("out/.dumped_font_name", "w");
  ఒకవేళ (!font_name_file)
  {
    fprintf (stderr, "./out is not accessible as a folder, create it please\n");
    తిరిగి;
  }
  fwrite (font_name, 1, అక్షరశ్రేణి_పొడవు (font_name), font_name_file);
  fclose (font_name_file);

  hb_draw_funcs_t *funcs = hb_draw_funcs_create ();
  hb_draw_funcs_సమితి_move_గమ్యం_func (funcs, (hb_draw_move_గమ్యం_func_t) move_గమ్యం, శూన్యసూచిక, శూన్యసూచిక);
  hb_draw_funcs_సమితి_పంక్తి_గమ్యం_func (funcs, (hb_draw_పంక్తి_గమ్యం_func_t) పంక్తి_గమ్యం, శూన్యసూచిక, శూన్యసూచిక);
  hb_draw_funcs_సమితి_quadratic_గమ్యం_func (funcs, (hb_draw_quadratic_గమ్యం_func_t) quadratic_గమ్యం, శూన్యసూచిక, శూన్యసూచిక);
  hb_draw_funcs_సమితి_cubic_గమ్యం_func (funcs, (hb_draw_cubic_గమ్యం_func_t) cubic_గమ్యం, శూన్యసూచిక, శూన్యసూచిక);
  hb_draw_funcs_సమితి_close_మార్గం_func (funcs, (hb_draw_close_మార్గం_func_t) close_మార్గం, శూన్యసూచిక, శూన్యసూచిక);

  గుర్తులేని అంకె_faces = hb_face_లెక్క (blob);
  కొరకు (గుర్తులేని face_సూచీ = 0; face_సూచీ < అంకె_faces; ++face_సూచీ)
  {
    hb_face_t *face = hb_face_create (blob, face_సూచీ);
    hb_font_t *font = hb_font_create (face);

    ఒకవేళ (hb_ot_color_has_png (face))
      రాయి ("Dumping png (CBDT/sbix)...\n");
    png_dump (face, face_సూచీ);

#నిర్వచితం_కాకపోతే HB_NO_SVG
    ఒకవేళ (hb_ot_color_has_svg (face))
      రాయి ("Dumping svg (SVG )...\n");
    svg_dump (face, face_సూచీ);
#అయితే_ముగింపు

    ఒకవేళ (hb_ot_color_has_layers (face) && hb_ot_color_has_palettes (face))
      రాయి ("Dumping layered color glyphs (COLR/CPAL)...\n");
    layered_glyph_dump (font, funcs, face_సూచీ);

    dump_glyphs (font, funcs, face_సూచీ);

    hb_font_destroy (font);
    hb_face_destroy (face);
  }

  hb_draw_funcs_destroy (funcs);
}
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే MAIN_CC_NO_వ్యక్తిగత_API
/* Only this part of this mini app uses private API */
#అన్తర్భావ "hb-static.cc"
#అన్తర్భావ "hb-open-file.hh"
#అన్తర్భావ "hb-ot-layout-gdef-table.hh"
#అన్తర్భావ "hb-ot-layout-gsubgpos.hh"

ఉపయోగించి నామాకాశ OT;

స్థిర శూన్య
print_layout_info_using_వ్యక్తిగత_api (hb_blob_t *blob)
{
  స్థిరరాశి అక్షరం *font_దత్తాంశం = hb_blob_get_దత్తాంశం (blob, శూన్యసూచిక);
  hb_blob_t *font_blob = hb_sanitize_context_t ().sanitize_blob<OpenTypeFontFile> (blob);
  స్థిరరాశి OpenTypeFontFile* sanitized = font_blob->as<OpenTypeFontFile> ();
  ఒకవేళ (!font_blob->దత్తాంశం)
  {
    రాయి ("Sanitization of the file wasn't successful. Exit");
    నిష్క్రమించు (1);
  }
  స్థిరరాశి OpenTypeFontFile& ot = *sanitized;

  ఎంపిక (ot.get_ట్యాగ్ ())
  {
  స్థితి OpenTypeFontFile::True_Type_ట్యాగ్:
    రాయి ("OpenType font with TrueType outlines\n");
    ఆపు;
  స్థితి OpenTypeFontFile::CFFTag:
    రాయి ("OpenType font with CFF (Type1) outlines\n");
    ఆపు;
  స్థితి OpenTypeFontFile::TTCTag:
    రాయి ("TrueType Collection of OpenType fonts\n");
    ఆపు;
  స్థితి OpenTypeFontFile::True_ట్యాగ్:
    రాయి ("Obsolete Apple TrueType font\n");
    ఆపు;
  స్థితి OpenTypeFontFile::Typ1_ట్యాగ్:
    రాయి ("Obsolete Apple Type1 font in SFNT container\n");
    ఆపు;
  స్థితి OpenTypeFontFile::DFont_ట్యాగ్:
    రాయి ("DFont Mac Resource Fork\n");
    ఆపు;
  అప్రమేయ:
    రాయి ("Unknown font format\n");
    ఆపు;
  }

  గుర్తులేని అంకె_faces = hb_face_లెక్క (blob);
  రాయి ("%u font(s) found in file\n", అంకె_faces);
  కొరకు (గుర్తులేని n_font = 0; n_font < అంకె_faces; ++n_font)
  {
    స్థిరరాశి OpenTypeFontFace &font = ot.get_face (n_font);
    రాయి ("Font %u of %u:\n", n_font, అంకె_faces);

    గుర్తులేని అంకె_tables = font.get_పట్టిక_లెక్క ();
    రాయి ("  %u table(s) found in font\n", అంకె_tables);
    కొరకు (గుర్తులేని n_పట్టిక = 0; n_పట్టిక < అంకె_tables; ++n_పట్టిక)
    {
      స్థిరరాశి Open_Type_పట్టిక &పట్టిక = font.get_పట్టిక (n_పట్టిక);
      రాయి ("  Table %2u of %2u: %.4s (0x%08x+0x%08x)\n", n_పట్టిక, అంకె_tables,
	      (స్థిరరాశి అక్షరం *) పట్టిక.ట్యాగ్_2,
	      (గుర్తులేని) పట్టిక.స్థానభ్రంశం,
	      (గుర్తులేని) పట్టిక.నిడివి);

      ఎంపిక (పట్టిక.ట్యాగ్_2)
      {

      స్థితి HB_OT_ట్యాగ్_GSUB:
      స్థితి HB_OT_ట్యాగ్_GPOS:
	{

	స్థిరరాశి GSUBGPOS &g = *reinterpret_cast<స్థిరరాశి GSUBGPOS *> (font_దత్తాంశం + పట్టిక.స్థానభ్రంశం);

	గుర్తులేని అంకె_scripts = g.get_script_లెక్క ();
	రాయి ("    %u script(s) found in table\n", అంకె_scripts);
	కొరకు (గుర్తులేని n_script = 0; n_script < అంకె_scripts; ++n_script)
	{
	  స్థిరరాశి Script &script = g.get_script (n_script);
	  రాయి ("    Script %2u of %2u: %.4s\n", n_script, అంకె_scripts,
		  (స్థిరరాశి అక్షరం *) g.get_script_ట్యాగ్ (n_script));

	  ఒకవేళ (!script.has_default_lang_sys ())
	    రాయి ("      No default language system\n");
	  పూర్ణాంకం అంకె_langsys = script.get_lang_sys_లెక్క ();
	  రాయి ("      %d language system(s) found in script\n", అంకె_langsys);
	  కొరకు (పూర్ణాంకం n_langsys = script.has_default_lang_sys () ? -1 : 0; n_langsys < అంకె_langsys; ++n_langsys)
	  {
	    స్థిరరాశి LangSys &langsys = n_langsys == -1
				   ? script.get_default_lang_sys ()
				   : script.get_lang_sys (n_langsys);
	    ఒకవేళ (n_langsys == -1)
	      రాయి ("      Default Language System\n");
	    లేకపోతే
	      రాయి ("      Language System %2d of %2d: %.4s\n", n_langsys, అంకె_langsys,
		      (స్థిరరాశి అక్షరం *) script.get_lang_sys_ట్యాగ్ (n_langsys));
	    ఒకవేళ (!langsys.has_required_feature ())
	      రాయి ("        No required feature\n");
	    లేకపోతే
	      రాయి ("        Required feature index: %u\n",
		      langsys.get_required_feature_సూచీ ());

	    గుర్తులేని అంకె_features = langsys.get_feature_లెక్క ();
	    రాయి ("        %u feature(s) found in language system\n", అంకె_features);
	    కొరకు (గుర్తులేని n_feature = 0; n_feature < అంకె_features; ++n_feature)
	    {
	      రాయి ("        Feature index %2u of %2u: %u\n", n_feature, అంకె_features,
		      langsys.get_feature_సూచీ (n_feature));
	    }
	  }
	}

	గుర్తులేని అంకె_features = g.get_feature_లెక్క ();
	రాయి ("    %u feature(s) found in table\n", అంకె_features);
	కొరకు (గుర్తులేని n_feature = 0; n_feature < అంకె_features; ++n_feature)
	{
	  స్థిరరాశి Feature &feature = g.get_feature (n_feature);
	  గుర్తులేని అంకె_lookups = feature.get_lookup_లెక్క ();
	  రాయి ("    Feature %2u of %2u: %c%c%c%c\n", n_feature, అంకె_features,
		  HB_UNTAG (g.get_feature_ట్యాగ్ (n_feature)));

	  రాయి ("        %u lookup(s) found in feature\n", అంకె_lookups);
	  కొరకు (గుర్తులేని n_lookup = 0; n_lookup < అంకె_lookups; ++n_lookup) {
	    రాయి ("        Lookup index %2u of %2u: %u\n", n_lookup, అంకె_lookups,
		    feature.get_lookup_సూచీ (n_lookup));
	  }
	}

	గుర్తులేని అంకె_lookups = g.get_lookup_లెక్క ();
	రాయి ("    %u lookup(s) found in table\n", అంకె_lookups);
	కొరకు (గుర్తులేని n_lookup = 0; n_lookup < అంకె_lookups; ++n_lookup)
	{
	  స్థిరరాశి Lookup &lookup = g.get_lookup (n_lookup);
	  రాయి ("    Lookup %2u of %2u: type %u, props 0x%04X\n", n_lookup, అంకె_lookups,
		  lookup.get_type (), lookup.get_props ());
	}

	}
	ఆపు;

      స్థితి GDEF::పట్టిక_ట్యాగ్:
	{

	స్థిరరాశి GDEF &gdef = *reinterpret_cast<స్థిరరాశి GDEF *> (font_దత్తాంశం + పట్టిక.స్థానభ్రంశం);

	రాయి ("    Has %sglyph classes\n",
		  gdef.has_glyph_classes () ? "" : "no ");
	రాయి ("    Has %smark attachment types\n",
		  gdef.has_mark_attachment_types () ? "" : "no ");
	రాయి ("    Has %sattach list\n",
		  gdef.has_attach_list () ? "" : "no ");
	రాయి ("    Has %slig carets\n",
		  gdef.has_lig_carets () ? "" : "no ");
	రాయి ("    Has %smark glyph sets\n",
		  gdef.has_mark_glyph_sets () ? "" : "no ");
	ఆపు;
	}
      }
    }
  }
}
/* end of private API use */
#అయితే_ముగింపు

పూర్ణాంకం
ప్రధాన (పూర్ణాంకం argc, అక్షరం **argv)
{
  ఒకవేళ (argc != 2)
  {
    fprintf (stderr, "usage: %s font-file.ttf\n\n"
		     "This tools is unsupported and crashes on bad data.\nDon't use it.\n", argv[0]);
    నిష్క్రమించు (1);
  }

  hb_blob_t *blob = hb_blob_create_from_file_or_fail (argv[1]);
  ప్రతిజ్ఞా (blob);
  రాయి ("Opened font file %s: %u bytes long\n", argv[1], hb_blob_get_నిడివి (blob));
#నిర్వచితం_కాకపోతే MAIN_CC_NO_వ్యక్తిగత_API
  print_layout_info_using_వ్యక్తిగత_api (blob);
#అయితే_ముగింపు
#ఒకవేళ !defined(HB_NO_COLOR) && !defined(HB_NO_DRAW)
  dump_glyphs (blob, argv[1]);
#అయితే_ముగింపు
  hb_blob_destroy (blob);

  తిరిగి 0;
}
