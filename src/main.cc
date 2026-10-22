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

#اگر_تعریف_شدہ HAVE_CONFIG_H
#شامل "config.h"
#اگر_ختم

#شامل "hb.h"
#شامل "hb-ot.h"

#شامل <cassert>
#شامل <cstdlib>
#شامل <cstdio>
#شامل <cstring>

#اگر_تعریف_شدہ HB_NO_OPEN
#تعریف_کرو hb_blob_create_from_file_or_fail(x)  hb_blob_get_خالی_خانے ()
#اگر_ختم

#اگر !defined(HB_NO_COLOR) && !defined(HB_NO_DRAW)
#اگر_غیر_تعریف_شدہ HB_NO_SVG
ساکن خالی
svg_dump (hb_face_t *face, غیرعلامتی face_اشاریہ)
{
  غیرعلامتی glyph_گنتی = hb_face_get_glyph_گنتی (face);

  برائے (غیرعلامتی glyph_id = 0; glyph_id < glyph_گنتی; ++glyph_id)
  {
    hb_blob_t *blob = hb_ot_color_glyph_reference_svg (face, glyph_id);

    اگر (hb_blob_get_لمبائی (blob) == 0) جاری;

    غیرعلامتی لمبائی;
    ثابت حرف *ڈیٹا = hb_blob_get_ڈیٹا (blob, &لمبائی);

    حرف output_راستہ[255];
    snprintf (output_راستہ, حجم output_راستہ,
	     "out/svg-%u-%u.svg%s",
	     glyph_id,
	     face_اشاریہ,
	     // append "z" if the content is gzipped, https://stackoverflow.com/a/6059405
	     (لمبائی > 2 && (ڈیٹا[0] == '\x1F') && (ڈیٹا[1] == '\x8B')) ? "z" : "");

    FILE *f = fopen (output_راستہ, "wb");
    fwrite (ڈیٹا, 1, لمبائی, f);
    fclose (f);

    hb_blob_destroy (blob);
  }
}
#اگر_ختم

/* _png API is so easy to use unlike the below code, don't get confused */
ساکن خالی
png_dump (hb_face_t *face, غیرعلامتی face_اشاریہ)
{
  غیرعلامتی glyph_گنتی = hb_face_get_glyph_گنتی (face);
  hb_font_t *font = hb_font_create (face);

  /* scans the font for strikes */
  غیرعلامتی sample_glyph_id;
  /* we don't care about different strikes for different glyphs at this point */
  برائے (sample_glyph_id = 0; sample_glyph_id < glyph_گنتی; ++sample_glyph_id)
  {
    hb_blob_t *blob = hb_ot_color_glyph_reference_png (font, sample_glyph_id);
    غیرعلامتی blob_لمبائی = hb_blob_get_لمبائی (blob);
    hb_blob_destroy (blob);
    اگر (blob_لمبائی != 0)
      توڑو;
  }

  غیرعلامتی upem = hb_face_get_upem (face);
  غیرعلامتی blob_لمبائی = 0;
  غیرعلامتی strike = 0;
  برائے (غیرعلامتی ppem = 1; ppem < upem; ++ppem)
  {
    hb_font_سیٹ_ppem (font, ppem, ppem);
    hb_blob_t *blob = hb_ot_color_glyph_reference_png (font, sample_glyph_id);
    غیرعلامتی new_blob_لمبائی = hb_blob_get_لمبائی (blob);
    hb_blob_destroy (blob);
    اگر (new_blob_لمبائی != blob_لمبائی)
    {
      برائے (غیرعلامتی glyph_id = 0; glyph_id < glyph_گنتی; ++glyph_id)
      {
	hb_blob_t *blob = hb_ot_color_glyph_reference_png (font, glyph_id);

	اگر (hb_blob_get_لمبائی (blob) == 0) جاری;

	غیرعلامتی لمبائی;
	ثابت حرف *ڈیٹا = hb_blob_get_ڈیٹا (blob, &لمبائی);

	حرف output_راستہ[255];
	snprintf (output_راستہ, حجم output_راستہ, "out/png-%u-%u-%u.png", glyph_id, strike, face_اشاریہ);

	FILE *f = fopen (output_راستہ, "wb");
	fwrite (ڈیٹا, 1, لمبائی, f);
	fclose (f);

	hb_blob_destroy (blob);
      }

      strike++;
      blob_لمبائی = new_blob_لمبائی;
    }
  }

  hb_font_destroy (font);
}

ساخت draw_ڈیٹا_t
{
  FILE *f;
  hb_position_t ascender;
};

ساکن خالی
move_مقصد (hb_draw_funcs_t *, draw_ڈیٹا_t *draw_ڈیٹا,
	 hb_draw_state_t *,
	 اعشاری مقصد_x, اعشاری مقصد_y,
	 خالی *)
{
  fprintf (draw_ڈیٹا->f, "M%g,%g", مقصد_x, draw_ڈیٹا->ascender - مقصد_y);
}

ساکن خالی
سطر_مقصد (hb_draw_funcs_t *, draw_ڈیٹا_t *draw_ڈیٹا,
	 hb_draw_state_t *,
	 اعشاری مقصد_x, اعشاری مقصد_y,
	 خالی *)
{
  fprintf (draw_ڈیٹا->f, "L%g,%g", مقصد_x, draw_ڈیٹا->ascender - مقصد_y);
}

ساکن خالی
quadratic_مقصد (hb_draw_funcs_t *, draw_ڈیٹا_t *draw_ڈیٹا,
	      hb_draw_state_t *,
	      اعشاری control_x, اعشاری control_y,
	      اعشاری مقصد_x, اعشاری مقصد_y,
	      خالی *)
{
  fprintf (draw_ڈیٹا->f, "Q%g,%g %g,%g", control_x, draw_ڈیٹا->ascender - control_y,
					مقصد_x, draw_ڈیٹا->ascender - مقصد_y);
}

ساکن خالی
cubic_مقصد (hb_draw_funcs_t *, draw_ڈیٹا_t *draw_ڈیٹا,
	  hb_draw_state_t *,
	  اعشاری control1_x, اعشاری control1_y,
	  اعشاری control2_x, اعشاری control2_y,
	  اعشاری مقصد_x, اعشاری مقصد_y,
	  خالی *)
{
  fprintf (draw_ڈیٹا->f, "C%g,%g %g,%g %g,%g", control1_x, draw_ڈیٹا->ascender - control1_y,
					       control2_x, draw_ڈیٹا->ascender - control2_y,
					       مقصد_x, draw_ڈیٹا->ascender - مقصد_y);
}

ساکن خالی
close_راستہ (hb_draw_funcs_t *, draw_ڈیٹا_t *draw_ڈیٹا,
	    hb_draw_state_t *,
	    خالی *)
{
  fprintf (draw_ڈیٹا->f, "Z");
}

ساکن خالی
layered_glyph_dump (hb_font_t *font, hb_draw_funcs_t *funcs, غیرعلامتی face_اشاریہ)
{
  hb_face_t *face = hb_font_get_face (font);
  غیرعلامتی palette_گنتی = hb_ot_color_palette_get_گنتی (face);
  برائے (غیرعلامتی palette = 0; palette < palette_گنتی; ++palette)
  {
    غیرعلامتی عدد_colors = hb_ot_color_palette_get_colors (face, palette, 0, خالی_اشاریہ, خالی_اشاریہ);
    اگر (!عدد_colors) جاری;

    hb_color_t *colors = (hb_color_t*) صفر_حافظہ_دو (عدد_colors, حجم (hb_color_t));
    hb_ot_color_palette_get_colors (face, palette, 0, &عدد_colors, colors);
    اگر (!عدد_colors)
    {
      آزاد (colors);
      جاری;
    }

    غیرعلامتی عدد_glyphs = hb_face_get_glyph_گنتی (face);
    برائے (hb_codepoint_t gid = 0; gid < عدد_glyphs; ++gid)
    {
      غیرعلامتی عدد_layers = hb_ot_color_glyph_get_layers (face, gid, 0, خالی_اشاریہ, خالی_اشاریہ);
      اگر (!عدد_layers) جاری;

      hb_ot_color_layer_t *layers = (hb_ot_color_layer_t*) حافظہ_دو (عدد_layers * حجم (hb_ot_color_layer_t));

      hb_ot_color_glyph_get_layers (face, gid, 0, &عدد_layers, layers);
      اگر (عدد_layers)
      {
	hb_font_extents_t font_extents;
	hb_font_get_extents_for_direction (font, HB_DIRECTION_LTR, &font_extents);
	hb_glyph_extents_t extents = {0, 0, 0, 0};
	اگر (!hb_font_get_glyph_extents (font, gid, &extents))
	{
	  لکھو ("Skip gid: %u\n", gid);
	  جاری;
	}

	حرف output_راستہ[255];
	snprintf (output_راستہ, حجم output_راستہ, "out/colr-%u-%u-%u.svg", gid, palette, face_اشاریہ);
	FILE *f = fopen (output_راستہ, "wb");
	fprintf (f, "<svg xmlns=\"http://www.w3.org/2000/svg\""
		    " viewBox=\"%d %d %lld %lld\">\n",
		    extents.x_bearing, 0,
		    (طویل طویل) extents.x_bearing + extents.چوڑائی,
		    -(طویل طویل) extents.اونچائی);
	draw_ڈیٹا_t draw_ڈیٹا;
	draw_ڈیٹا.ascender = extents.y_bearing;
	draw_ڈیٹا.f = f;

	برائے (غیرعلامتی layer = 0; layer < عدد_layers; ++layer)
	{
	  hb_color_t color = 0x000000FF;
	  اگر (layers[layer].color_اشاریہ != 0xFFFF)
	    color = colors[layers[layer].color_اشاریہ];
	  fprintf (f, "<path fill=\"#%02X%02X%02X\" ",
		   hb_color_get_red (color), hb_color_get_green (color), hb_color_get_green (color));
	  اگر (hb_color_get_alpha (color) != 255)
	    fprintf (f, "fill-opacity=\"%.3f\"", (دوہرا) hb_color_get_alpha (color) / 255.);
	  fprintf (f, "d=\"");
	  hb_font_draw_glyph (font, layers[layer].glyph, funcs, &draw_ڈیٹا);
	  fprintf (f, "\"/>\n");
	}

	fprintf (f, "</svg>");
	fclose (f);
      }
      آزاد (layers);
    }

    آزاد (colors);
  }
}

ساکن خالی
dump_glyphs (hb_font_t *font, hb_draw_funcs_t *funcs, غیرعلامتی face_اشاریہ)
{
  غیرعلامتی عدد_glyphs = hb_face_get_glyph_گنتی (hb_font_get_face (font));
  برائے (غیرعلامتی gid = 0; gid < عدد_glyphs; ++gid)
  {
    hb_font_extents_t font_extents;
    hb_font_get_extents_for_direction (font, HB_DIRECTION_LTR, &font_extents);
    hb_glyph_extents_t extents = {0, 0, 0, 0};
    اگر (!hb_font_get_glyph_extents (font, gid, &extents))
    {
      لکھو ("Skip gid: %u\n", gid);
      جاری;
    }

    حرف output_راستہ[255];
    snprintf (output_راستہ, حجم output_راستہ, "out/%u-%u.svg", face_اشاریہ, gid);
    FILE *f = fopen (output_راستہ, "wb");
    fprintf (f, "<svg xmlns=\"http://www.w3.org/2000/svg\""
		" viewBox=\"%d %d %lld %lld\"><path d=\"",
		extents.x_bearing, 0,
		(طویل طویل) extents.x_bearing + extents.چوڑائی,
		(طویل طویل) font_extents.ascender - font_extents.descender);
    draw_ڈیٹا_t draw_ڈیٹا;
    draw_ڈیٹا.ascender = font_extents.ascender;
    draw_ڈیٹا.f = f;
    hb_font_draw_glyph (font, gid, funcs, &draw_ڈیٹا);
    fprintf (f, "\"/></svg>");
    fclose (f);
  }
}

ساکن خالی
dump_glyphs (hb_blob_t *blob, ثابت حرف *font_name)
{
  FILE *font_name_file = fopen ("out/.dumped_font_name", "r");
  اگر (font_name_file)
  {
    fprintf (stderr, "Purge or rename ./out folder if you like to run a glyph dump,\n"
		     "run it like `rm -rf out && mkdir out && src/main font-file.ttf`\n");
    واپس;
  }

  font_name_file = fopen ("out/.dumped_font_name", "w");
  اگر (!font_name_file)
  {
    fprintf (stderr, "./out is not accessible as a folder, create it please\n");
    واپس;
  }
  fwrite (font_name, 1, لڑی_لمبائی (font_name), font_name_file);
  fclose (font_name_file);

  hb_draw_funcs_t *funcs = hb_draw_funcs_create ();
  hb_draw_funcs_سیٹ_move_مقصد_func (funcs, (hb_draw_move_مقصد_func_t) move_مقصد, خالی_اشاریہ, خالی_اشاریہ);
  hb_draw_funcs_سیٹ_سطر_مقصد_func (funcs, (hb_draw_سطر_مقصد_func_t) سطر_مقصد, خالی_اشاریہ, خالی_اشاریہ);
  hb_draw_funcs_سیٹ_quadratic_مقصد_func (funcs, (hb_draw_quadratic_مقصد_func_t) quadratic_مقصد, خالی_اشاریہ, خالی_اشاریہ);
  hb_draw_funcs_سیٹ_cubic_مقصد_func (funcs, (hb_draw_cubic_مقصد_func_t) cubic_مقصد, خالی_اشاریہ, خالی_اشاریہ);
  hb_draw_funcs_سیٹ_close_راستہ_func (funcs, (hb_draw_close_راستہ_func_t) close_راستہ, خالی_اشاریہ, خالی_اشاریہ);

  غیرعلامتی عدد_faces = hb_face_گنتی (blob);
  برائے (غیرعلامتی face_اشاریہ = 0; face_اشاریہ < عدد_faces; ++face_اشاریہ)
  {
    hb_face_t *face = hb_face_create (blob, face_اشاریہ);
    hb_font_t *font = hb_font_create (face);

    اگر (hb_ot_color_has_png (face))
      لکھو ("Dumping png (CBDT/sbix)...\n");
    png_dump (face, face_اشاریہ);

#اگر_غیر_تعریف_شدہ HB_NO_SVG
    اگر (hb_ot_color_has_svg (face))
      لکھو ("Dumping svg (SVG )...\n");
    svg_dump (face, face_اشاریہ);
#اگر_ختم

    اگر (hb_ot_color_has_layers (face) && hb_ot_color_has_palettes (face))
      لکھو ("Dumping layered color glyphs (COLR/CPAL)...\n");
    layered_glyph_dump (font, funcs, face_اشاریہ);

    dump_glyphs (font, funcs, face_اشاریہ);

    hb_font_destroy (font);
    hb_face_destroy (face);
  }

  hb_draw_funcs_destroy (funcs);
}
#اگر_ختم

#اگر_غیر_تعریف_شدہ MAIN_CC_NO_نجی_API
/* Only this part of this mini app uses private API */
#شامل "hb-static.cc"
#شامل "hb-open-file.hh"
#شامل "hb-ot-layout-gdef-table.hh"
#شامل "hb-ot-layout-gsubgpos.hh"

استعمال نامخلا OT;

ساکن خالی
print_layout_info_using_نجی_api (hb_blob_t *blob)
{
  ثابت حرف *font_ڈیٹا = hb_blob_get_ڈیٹا (blob, خالی_اشاریہ);
  hb_blob_t *font_blob = hb_sanitize_context_t ().sanitize_blob<OpenTypeFontFile> (blob);
  ثابت OpenTypeFontFile* sanitized = font_blob->as<OpenTypeFontFile> ();
  اگر (!font_blob->ڈیٹا)
  {
    لکھو ("Sanitization of the file wasn't successful. Exit");
    باہر_نکلو (1);
  }
  ثابت OpenTypeFontFile& ot = *sanitized;

  انتخاب (ot.get_لیبل ())
  {
  حالت OpenTypeFontFile::True_Type_لیبل:
    لکھو ("OpenType font with TrueType outlines\n");
    توڑو;
  حالت OpenTypeFontFile::CFFTag:
    لکھو ("OpenType font with CFF (Type1) outlines\n");
    توڑو;
  حالت OpenTypeFontFile::TTCTag:
    لکھو ("TrueType Collection of OpenType fonts\n");
    توڑو;
  حالت OpenTypeFontFile::True_لیبل:
    لکھو ("Obsolete Apple TrueType font\n");
    توڑو;
  حالت OpenTypeFontFile::Typ1_لیبل:
    لکھو ("Obsolete Apple Type1 font in SFNT container\n");
    توڑو;
  حالت OpenTypeFontFile::DFont_لیبل:
    لکھو ("DFont Mac Resource Fork\n");
    توڑو;
  طےشدہ:
    لکھو ("Unknown font format\n");
    توڑو;
  }

  غیرعلامتی عدد_faces = hb_face_گنتی (blob);
  لکھو ("%u font(s) found in file\n", عدد_faces);
  برائے (غیرعلامتی n_font = 0; n_font < عدد_faces; ++n_font)
  {
    ثابت OpenTypeFontFace &font = ot.get_face (n_font);
    لکھو ("Font %u of %u:\n", n_font, عدد_faces);

    غیرعلامتی عدد_tables = font.get_جدول_گنتی ();
    لکھو ("  %u table(s) found in font\n", عدد_tables);
    برائے (غیرعلامتی n_جدول = 0; n_جدول < عدد_tables; ++n_جدول)
    {
      ثابت Open_Type_جدول &جدول = font.get_جدول (n_جدول);
      لکھو ("  Table %2u of %2u: %.4s (0x%08x+0x%08x)\n", n_جدول, عدد_tables,
	      (ثابت حرف *) جدول.لیبل_2,
	      (غیرعلامتی) جدول.انحراف,
	      (غیرعلامتی) جدول.لمبائی);

      انتخاب (جدول.لیبل_2)
      {

      حالت HB_OT_لیبل_GSUB:
      حالت HB_OT_لیبل_GPOS:
	{

	ثابت GSUBGPOS &g = *reinterpret_cast<ثابت GSUBGPOS *> (font_ڈیٹا + جدول.انحراف);

	غیرعلامتی عدد_scripts = g.get_script_گنتی ();
	لکھو ("    %u script(s) found in table\n", عدد_scripts);
	برائے (غیرعلامتی n_script = 0; n_script < عدد_scripts; ++n_script)
	{
	  ثابت Script &script = g.get_script (n_script);
	  لکھو ("    Script %2u of %2u: %.4s\n", n_script, عدد_scripts,
		  (ثابت حرف *) g.get_script_لیبل (n_script));

	  اگر (!script.has_default_lang_sys ())
	    لکھو ("      No default language system\n");
	  صحیح عدد_langsys = script.get_lang_sys_گنتی ();
	  لکھو ("      %d language system(s) found in script\n", عدد_langsys);
	  برائے (صحیح n_langsys = script.has_default_lang_sys () ? -1 : 0; n_langsys < عدد_langsys; ++n_langsys)
	  {
	    ثابت LangSys &langsys = n_langsys == -1
				   ? script.get_default_lang_sys ()
				   : script.get_lang_sys (n_langsys);
	    اگر (n_langsys == -1)
	      لکھو ("      Default Language System\n");
	    ورنہ
	      لکھو ("      Language System %2d of %2d: %.4s\n", n_langsys, عدد_langsys,
		      (ثابت حرف *) script.get_lang_sys_لیبل (n_langsys));
	    اگر (!langsys.has_required_feature ())
	      لکھو ("        No required feature\n");
	    ورنہ
	      لکھو ("        Required feature index: %u\n",
		      langsys.get_required_feature_اشاریہ ());

	    غیرعلامتی عدد_features = langsys.get_feature_گنتی ();
	    لکھو ("        %u feature(s) found in language system\n", عدد_features);
	    برائے (غیرعلامتی n_feature = 0; n_feature < عدد_features; ++n_feature)
	    {
	      لکھو ("        Feature index %2u of %2u: %u\n", n_feature, عدد_features,
		      langsys.get_feature_اشاریہ (n_feature));
	    }
	  }
	}

	غیرعلامتی عدد_features = g.get_feature_گنتی ();
	لکھو ("    %u feature(s) found in table\n", عدد_features);
	برائے (غیرعلامتی n_feature = 0; n_feature < عدد_features; ++n_feature)
	{
	  ثابت Feature &feature = g.get_feature (n_feature);
	  غیرعلامتی عدد_lookups = feature.get_lookup_گنتی ();
	  لکھو ("    Feature %2u of %2u: %c%c%c%c\n", n_feature, عدد_features,
		  HB_UNTAG (g.get_feature_لیبل (n_feature)));

	  لکھو ("        %u lookup(s) found in feature\n", عدد_lookups);
	  برائے (غیرعلامتی n_lookup = 0; n_lookup < عدد_lookups; ++n_lookup) {
	    لکھو ("        Lookup index %2u of %2u: %u\n", n_lookup, عدد_lookups,
		    feature.get_lookup_اشاریہ (n_lookup));
	  }
	}

	غیرعلامتی عدد_lookups = g.get_lookup_گنتی ();
	لکھو ("    %u lookup(s) found in table\n", عدد_lookups);
	برائے (غیرعلامتی n_lookup = 0; n_lookup < عدد_lookups; ++n_lookup)
	{
	  ثابت Lookup &lookup = g.get_lookup (n_lookup);
	  لکھو ("    Lookup %2u of %2u: type %u, props 0x%04X\n", n_lookup, عدد_lookups,
		  lookup.get_type (), lookup.get_props ());
	}

	}
	توڑو;

      حالت GDEF::جدول_لیبل:
	{

	ثابت GDEF &gdef = *reinterpret_cast<ثابت GDEF *> (font_ڈیٹا + جدول.انحراف);

	لکھو ("    Has %sglyph classes\n",
		  gdef.has_glyph_classes () ? "" : "no ");
	لکھو ("    Has %smark attachment types\n",
		  gdef.has_mark_attachment_types () ? "" : "no ");
	لکھو ("    Has %sattach list\n",
		  gdef.has_attach_list () ? "" : "no ");
	لکھو ("    Has %slig carets\n",
		  gdef.has_lig_carets () ? "" : "no ");
	لکھو ("    Has %smark glyph sets\n",
		  gdef.has_mark_glyph_sets () ? "" : "no ");
	توڑو;
	}
      }
    }
  }
}
/* end of private API use */
#اگر_ختم

صحیح
مین (صحیح argc, حرف **argv)
{
  اگر (argc != 2)
  {
    fprintf (stderr, "usage: %s font-file.ttf\n\n"
		     "This tools is unsupported and crashes on bad data.\nDon't use it.\n", argv[0]);
    باہر_نکلو (1);
  }

  hb_blob_t *blob = hb_blob_create_from_file_or_fail (argv[1]);
  أكد (blob);
  لکھو ("Opened font file %s: %u bytes long\n", argv[1], hb_blob_get_لمبائی (blob));
#اگر_غیر_تعریف_شدہ MAIN_CC_NO_نجی_API
  print_layout_info_using_نجی_api (blob);
#اگر_ختم
#اگر !defined(HB_NO_COLOR) && !defined(HB_NO_DRAW)
  dump_glyphs (blob, argv[1]);
#اگر_ختم
  hb_blob_destroy (blob);

  واپس 0;
}
