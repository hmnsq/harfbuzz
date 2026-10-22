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

#جے_تعریف_شدہ HAVE_CONFIG_H
#ضمن "config.h"
#جے_انت

#ضمن "hb.h"
#ضمن "hb-ot.h"

#ضمن <cassert>
#ضمن <cstdlib>
#ضمن <cstdio>
#ضمن <cstring>

#جے_تعریف_شدہ HB_NO_OPEN
#تعریف_کرو hb_blob_create_from_file_or_fail(x)  hb_blob_get_خالی_تھاواں ()
#جے_انت

#إذا !defined(HB_NO_COLOR) && !defined(HB_NO_DRAW)
#جے_بنا_تعریف HB_NO_SVG
ساكن فراغ
svg_dump (hb_face_t *face, بنانشانی face_اشاریہ)
{
  بنانشانی glyph_گنتی = hb_face_get_glyph_گنتی (face);

  لكل (بنانشانی glyph_id = 0; glyph_id < glyph_گنتی; ++glyph_id)
  {
    hb_blob_t *blob = hb_ot_color_glyph_reference_svg (face, glyph_id);

    إذا (hb_blob_get_لمائی (blob) == 0) استمر;

    بنانشانی لمائی;
    ثابت حرف *انکڑے = hb_blob_get_انکڑے (blob, &لمائی);

    حرف output_رستہ[255];
    snprintf (output_رستہ, حجم output_رستہ,
	     "out/svg-%u-%u.svg%s",
	     glyph_id,
	     face_اشاریہ,
	     // append "z" if the content is gzipped, https://stackoverflow.com/a/6059405
	     (لمائی > 2 && (انکڑے[0] == '\x1F') && (انکڑے[1] == '\x8B')) ? "z" : "");

    FILE *f = fopen (output_رستہ, "wb");
    fwrite (انکڑے, 1, لمائی, f);
    fclose (f);

    hb_blob_destroy (blob);
  }
}
#جے_انت

/* _png API is so easy to use unlike the below code, don't get confused */
ساكن فراغ
png_dump (hb_face_t *face, بنانشانی face_اشاریہ)
{
  بنانشانی glyph_گنتی = hb_face_get_glyph_گنتی (face);
  hb_font_t *font = hb_font_create (face);

  /* scans the font for strikes */
  بنانشانی sample_glyph_id;
  /* we don't care about different strikes for different glyphs at this point */
  لكل (sample_glyph_id = 0; sample_glyph_id < glyph_گنتی; ++sample_glyph_id)
  {
    hb_blob_t *blob = hb_ot_color_glyph_reference_png (font, sample_glyph_id);
    بنانشانی blob_لمائی = hb_blob_get_لمائی (blob);
    hb_blob_destroy (blob);
    إذا (blob_لمائی != 0)
      اكسر;
  }

  بنانشانی upem = hb_face_get_upem (face);
  بنانشانی blob_لمائی = 0;
  بنانشانی strike = 0;
  لكل (بنانشانی ppem = 1; ppem < upem; ++ppem)
  {
    hb_font_سیٹ_ppem (font, ppem, ppem);
    hb_blob_t *blob = hb_ot_color_glyph_reference_png (font, sample_glyph_id);
    بنانشانی new_blob_لمائی = hb_blob_get_لمائی (blob);
    hb_blob_destroy (blob);
    إذا (new_blob_لمائی != blob_لمائی)
    {
      لكل (بنانشانی glyph_id = 0; glyph_id < glyph_گنتی; ++glyph_id)
      {
	hb_blob_t *blob = hb_ot_color_glyph_reference_png (font, glyph_id);

	إذا (hb_blob_get_لمائی (blob) == 0) استمر;

	بنانشانی لمائی;
	ثابت حرف *انکڑے = hb_blob_get_انکڑے (blob, &لمائی);

	حرف output_رستہ[255];
	snprintf (output_رستہ, حجم output_رستہ, "out/png-%u-%u-%u.png", glyph_id, strike, face_اشاریہ);

	FILE *f = fopen (output_رستہ, "wb");
	fwrite (انکڑے, 1, لمائی, f);
	fclose (f);

	hb_blob_destroy (blob);
      }

      strike++;
      blob_لمائی = new_blob_لمائی;
    }
  }

  hb_font_destroy (font);
}

بنية draw_انکڑے_t
{
  FILE *f;
  hb_position_t ascender;
};

ساكن فراغ
move_منزل (hb_draw_funcs_t *, draw_انکڑے_t *draw_انکڑے,
	 hb_draw_state_t *,
	 عائم منزل_x, عائم منزل_y,
	 فراغ *)
{
  fprintf (draw_انکڑے->f, "M%g,%g", منزل_x, draw_انکڑے->ascender - منزل_y);
}

ساكن فراغ
سطر_منزل (hb_draw_funcs_t *, draw_انکڑے_t *draw_انکڑے,
	 hb_draw_state_t *,
	 عائم منزل_x, عائم منزل_y,
	 فراغ *)
{
  fprintf (draw_انکڑے->f, "L%g,%g", منزل_x, draw_انکڑے->ascender - منزل_y);
}

ساكن فراغ
quadratic_منزل (hb_draw_funcs_t *, draw_انکڑے_t *draw_انکڑے,
	      hb_draw_state_t *,
	      عائم control_x, عائم control_y,
	      عائم منزل_x, عائم منزل_y,
	      فراغ *)
{
  fprintf (draw_انکڑے->f, "Q%g,%g %g,%g", control_x, draw_انکڑے->ascender - control_y,
					منزل_x, draw_انکڑے->ascender - منزل_y);
}

ساكن فراغ
cubic_منزل (hb_draw_funcs_t *, draw_انکڑے_t *draw_انکڑے,
	  hb_draw_state_t *,
	  عائم control1_x, عائم control1_y,
	  عائم control2_x, عائم control2_y,
	  عائم منزل_x, عائم منزل_y,
	  فراغ *)
{
  fprintf (draw_انکڑے->f, "C%g,%g %g,%g %g,%g", control1_x, draw_انکڑے->ascender - control1_y,
					       control2_x, draw_انکڑے->ascender - control2_y,
					       منزل_x, draw_انکڑے->ascender - منزل_y);
}

ساكن فراغ
close_رستہ (hb_draw_funcs_t *, draw_انکڑے_t *draw_انکڑے,
	    hb_draw_state_t *,
	    فراغ *)
{
  fprintf (draw_انکڑے->f, "Z");
}

ساكن فراغ
layered_glyph_dump (hb_font_t *font, hb_draw_funcs_t *funcs, بنانشانی face_اشاریہ)
{
  hb_face_t *face = hb_font_get_face (font);
  بنانشانی palette_گنتی = hb_ot_color_palette_get_گنتی (face);
  لكل (بنانشانی palette = 0; palette < palette_گنتی; ++palette)
  {
    بنانشانی عدد_colors = hb_ot_color_palette_get_colors (face, palette, 0, خالی_سوچک, خالی_سوچک);
    إذا (!عدد_colors) استمر;

    hb_color_t *colors = (hb_color_t*) صفر_حافظہ_دیو (عدد_colors, حجم (hb_color_t));
    hb_ot_color_palette_get_colors (face, palette, 0, &عدد_colors, colors);
    إذا (!عدد_colors)
    {
      حرر (colors);
      استمر;
    }

    بنانشانی عدد_glyphs = hb_face_get_glyph_گنتی (face);
    لكل (hb_codepoint_t gid = 0; gid < عدد_glyphs; ++gid)
    {
      بنانشانی عدد_layers = hb_ot_color_glyph_get_layers (face, gid, 0, خالی_سوچک, خالی_سوچک);
      إذا (!عدد_layers) استمر;

      hb_ot_color_layer_t *layers = (hb_ot_color_layer_t*) حافظہ_دیو (عدد_layers * حجم (hb_ot_color_layer_t));

      hb_ot_color_glyph_get_layers (face, gid, 0, &عدد_layers, layers);
      إذا (عدد_layers)
      {
	hb_font_extents_t font_extents;
	hb_font_get_extents_for_direction (font, HB_DIRECTION_LTR, &font_extents);
	hb_glyph_extents_t extents = {0, 0, 0, 0};
	إذا (!hb_font_get_glyph_extents (font, gid, &extents))
	{
	  لکھو ("Skip gid: %u\n", gid);
	  استمر;
	}

	حرف output_رستہ[255];
	snprintf (output_رستہ, حجم output_رستہ, "out/colr-%u-%u-%u.svg", gid, palette, face_اشاریہ);
	FILE *f = fopen (output_رستہ, "wb");
	fprintf (f, "<svg xmlns=\"http://www.w3.org/2000/svg\""
		    " viewBox=\"%d %d %lld %lld\">\n",
		    extents.x_bearing, 0,
		    (لما لما) extents.x_bearing + extents.چوڑائی,
		    -(لما لما) extents.اچائی);
	draw_انکڑے_t draw_انکڑے;
	draw_انکڑے.ascender = extents.y_bearing;
	draw_انکڑے.f = f;

	لكل (بنانشانی layer = 0; layer < عدد_layers; ++layer)
	{
	  hb_color_t color = 0x000000FF;
	  إذا (layers[layer].color_اشاریہ != 0xFFFF)
	    color = colors[layers[layer].color_اشاریہ];
	  fprintf (f, "<path fill=\"#%02X%02X%02X\" ",
		   hb_color_get_red (color), hb_color_get_green (color), hb_color_get_green (color));
	  إذا (hb_color_get_alpha (color) != 255)
	    fprintf (f, "fill-opacity=\"%.3f\"", (مزدوج) hb_color_get_alpha (color) / 255.);
	  fprintf (f, "d=\"");
	  hb_font_draw_glyph (font, layers[layer].glyph, funcs, &draw_انکڑے);
	  fprintf (f, "\"/>\n");
	}

	fprintf (f, "</svg>");
	fclose (f);
      }
      حرر (layers);
    }

    حرر (colors);
  }
}

ساكن فراغ
dump_glyphs (hb_font_t *font, hb_draw_funcs_t *funcs, بنانشانی face_اشاریہ)
{
  بنانشانی عدد_glyphs = hb_face_get_glyph_گنتی (hb_font_get_face (font));
  لكل (بنانشانی gid = 0; gid < عدد_glyphs; ++gid)
  {
    hb_font_extents_t font_extents;
    hb_font_get_extents_for_direction (font, HB_DIRECTION_LTR, &font_extents);
    hb_glyph_extents_t extents = {0, 0, 0, 0};
    إذا (!hb_font_get_glyph_extents (font, gid, &extents))
    {
      لکھو ("Skip gid: %u\n", gid);
      استمر;
    }

    حرف output_رستہ[255];
    snprintf (output_رستہ, حجم output_رستہ, "out/%u-%u.svg", face_اشاریہ, gid);
    FILE *f = fopen (output_رستہ, "wb");
    fprintf (f, "<svg xmlns=\"http://www.w3.org/2000/svg\""
		" viewBox=\"%d %d %lld %lld\"><path d=\"",
		extents.x_bearing, 0,
		(لما لما) extents.x_bearing + extents.چوڑائی,
		(لما لما) font_extents.ascender - font_extents.descender);
    draw_انکڑے_t draw_انکڑے;
    draw_انکڑے.ascender = font_extents.ascender;
    draw_انکڑے.f = f;
    hb_font_draw_glyph (font, gid, funcs, &draw_انکڑے);
    fprintf (f, "\"/></svg>");
    fclose (f);
  }
}

ساكن فراغ
dump_glyphs (hb_blob_t *blob, ثابت حرف *font_name)
{
  FILE *font_name_file = fopen ("out/.dumped_font_name", "r");
  إذا (font_name_file)
  {
    fprintf (stderr, "Purge or rename ./out folder if you like to run a glyph dump,\n"
		     "run it like `rm -rf out && mkdir out && src/main font-file.ttf`\n");
    أرجع;
  }

  font_name_file = fopen ("out/.dumped_font_name", "w");
  إذا (!font_name_file)
  {
    fprintf (stderr, "./out is not accessible as a folder, create it please\n");
    أرجع;
  }
  fwrite (font_name, 1, لڑی_لمبائی (font_name), font_name_file);
  fclose (font_name_file);

  hb_draw_funcs_t *funcs = hb_draw_funcs_create ();
  hb_draw_funcs_سیٹ_move_منزل_func (funcs, (hb_draw_move_منزل_func_t) move_منزل, خالی_سوچک, خالی_سوچک);
  hb_draw_funcs_سیٹ_سطر_منزل_func (funcs, (hb_draw_سطر_منزل_func_t) سطر_منزل, خالی_سوچک, خالی_سوچک);
  hb_draw_funcs_سیٹ_quadratic_منزل_func (funcs, (hb_draw_quadratic_منزل_func_t) quadratic_منزل, خالی_سوچک, خالی_سوچک);
  hb_draw_funcs_سیٹ_cubic_منزل_func (funcs, (hb_draw_cubic_منزل_func_t) cubic_منزل, خالی_سوچک, خالی_سوچک);
  hb_draw_funcs_سیٹ_close_رستہ_func (funcs, (hb_draw_close_رستہ_func_t) close_رستہ, خالی_سوچک, خالی_سوچک);

  بنانشانی عدد_faces = hb_face_گنتی (blob);
  لكل (بنانشانی face_اشاریہ = 0; face_اشاریہ < عدد_faces; ++face_اشاریہ)
  {
    hb_face_t *face = hb_face_create (blob, face_اشاریہ);
    hb_font_t *font = hb_font_create (face);

    إذا (hb_ot_color_has_png (face))
      لکھو ("Dumping png (CBDT/sbix)...\n");
    png_dump (face, face_اشاریہ);

#جے_بنا_تعریف HB_NO_SVG
    إذا (hb_ot_color_has_svg (face))
      لکھو ("Dumping svg (SVG )...\n");
    svg_dump (face, face_اشاریہ);
#جے_انت

    إذا (hb_ot_color_has_layers (face) && hb_ot_color_has_palettes (face))
      لکھو ("Dumping layered color glyphs (COLR/CPAL)...\n");
    layered_glyph_dump (font, funcs, face_اشاریہ);

    dump_glyphs (font, funcs, face_اشاریہ);

    hb_font_destroy (font);
    hb_face_destroy (face);
  }

  hb_draw_funcs_destroy (funcs);
}
#جے_انت

#جے_بنا_تعریف MAIN_CC_NO_نجی_API
/* Only this part of this mini app uses private API */
#ضمن "hb-static.cc"
#ضمن "hb-open-file.hh"
#ضمن "hb-ot-layout-gdef-table.hh"
#ضمن "hb-ot-layout-gsubgpos.hh"

ورتوں نطاق OT;

ساكن فراغ
print_layout_info_using_نجی_api (hb_blob_t *blob)
{
  ثابت حرف *font_انکڑے = hb_blob_get_انکڑے (blob, خالی_سوچک);
  hb_blob_t *font_blob = hb_sanitize_context_t ().sanitize_blob<OpenTypeFontFile> (blob);
  ثابت OpenTypeFontFile* sanitized = font_blob->as<OpenTypeFontFile> ();
  إذا (!font_blob->انکڑے)
  {
    لکھو ("Sanitization of the file wasn't successful. Exit");
    باہر_نکلو (1);
  }
  ثابت OpenTypeFontFile& ot = *sanitized;

  بدل (ot.get_ٹیگ ())
  {
  حالة OpenTypeFontFile::True_Type_ٹیگ:
    لکھو ("OpenType font with TrueType outlines\n");
    اكسر;
  حالة OpenTypeFontFile::CFFTag:
    لکھو ("OpenType font with CFF (Type1) outlines\n");
    اكسر;
  حالة OpenTypeFontFile::TTCTag:
    لکھو ("TrueType Collection of OpenType fonts\n");
    اكسر;
  حالة OpenTypeFontFile::True_ٹیگ:
    لکھو ("Obsolete Apple TrueType font\n");
    اكسر;
  حالة OpenTypeFontFile::Typ1_ٹیگ:
    لکھو ("Obsolete Apple Type1 font in SFNT container\n");
    اكسر;
  حالة OpenTypeFontFile::DFont_ٹیگ:
    لکھو ("DFont Mac Resource Fork\n");
    اكسر;
  افتراضي:
    لکھو ("Unknown font format\n");
    اكسر;
  }

  بنانشانی عدد_faces = hb_face_گنتی (blob);
  لکھو ("%u font(s) found in file\n", عدد_faces);
  لكل (بنانشانی n_font = 0; n_font < عدد_faces; ++n_font)
  {
    ثابت OpenTypeFontFace &font = ot.get_face (n_font);
    لکھو ("Font %u of %u:\n", n_font, عدد_faces);

    بنانشانی عدد_tables = font.get_جدول_گنتی ();
    لکھو ("  %u table(s) found in font\n", عدد_tables);
    لكل (بنانشانی n_جدول = 0; n_جدول < عدد_tables; ++n_جدول)
    {
      ثابت Open_Type_جدول &جدول = font.get_جدول (n_جدول);
      لکھو ("  Table %2u of %2u: %.4s (0x%08x+0x%08x)\n", n_جدول, عدد_tables,
	      (ثابت حرف *) جدول.ٹیگ_2,
	      (بنانشانی) جدول.وتھ,
	      (بنانشانی) جدول.لمائی);

      بدل (جدول.ٹیگ_2)
      {

      حالة HB_OT_ٹیگ_GSUB:
      حالة HB_OT_ٹیگ_GPOS:
	{

	ثابت GSUBGPOS &g = *reinterpret_cast<ثابت GSUBGPOS *> (font_انکڑے + جدول.وتھ);

	بنانشانی عدد_scripts = g.get_script_گنتی ();
	لکھو ("    %u script(s) found in table\n", عدد_scripts);
	لكل (بنانشانی n_script = 0; n_script < عدد_scripts; ++n_script)
	{
	  ثابت Script &script = g.get_script (n_script);
	  لکھو ("    Script %2u of %2u: %.4s\n", n_script, عدد_scripts,
		  (ثابت حرف *) g.get_script_ٹیگ (n_script));

	  إذا (!script.has_default_lang_sys ())
	    لکھو ("      No default language system\n");
	  صحيح عدد_langsys = script.get_lang_sys_گنتی ();
	  لکھو ("      %d language system(s) found in script\n", عدد_langsys);
	  لكل (صحيح n_langsys = script.has_default_lang_sys () ? -1 : 0; n_langsys < عدد_langsys; ++n_langsys)
	  {
	    ثابت LangSys &langsys = n_langsys == -1
				   ? script.get_default_lang_sys ()
				   : script.get_lang_sys (n_langsys);
	    إذا (n_langsys == -1)
	      لکھو ("      Default Language System\n");
	    وإلا
	      لکھو ("      Language System %2d of %2d: %.4s\n", n_langsys, عدد_langsys,
		      (ثابت حرف *) script.get_lang_sys_ٹیگ (n_langsys));
	    إذا (!langsys.has_required_feature ())
	      لکھو ("        No required feature\n");
	    وإلا
	      لکھو ("        Required feature index: %u\n",
		      langsys.get_required_feature_اشاریہ ());

	    بنانشانی عدد_features = langsys.get_feature_گنتی ();
	    لکھو ("        %u feature(s) found in language system\n", عدد_features);
	    لكل (بنانشانی n_feature = 0; n_feature < عدد_features; ++n_feature)
	    {
	      لکھو ("        Feature index %2u of %2u: %u\n", n_feature, عدد_features,
		      langsys.get_feature_اشاریہ (n_feature));
	    }
	  }
	}

	بنانشانی عدد_features = g.get_feature_گنتی ();
	لکھو ("    %u feature(s) found in table\n", عدد_features);
	لكل (بنانشانی n_feature = 0; n_feature < عدد_features; ++n_feature)
	{
	  ثابت Feature &feature = g.get_feature (n_feature);
	  بنانشانی عدد_lookups = feature.get_lookup_گنتی ();
	  لکھو ("    Feature %2u of %2u: %c%c%c%c\n", n_feature, عدد_features,
		  HB_UNTAG (g.get_feature_ٹیگ (n_feature)));

	  لکھو ("        %u lookup(s) found in feature\n", عدد_lookups);
	  لكل (بنانشانی n_lookup = 0; n_lookup < عدد_lookups; ++n_lookup) {
	    لکھو ("        Lookup index %2u of %2u: %u\n", n_lookup, عدد_lookups,
		    feature.get_lookup_اشاریہ (n_lookup));
	  }
	}

	بنانشانی عدد_lookups = g.get_lookup_گنتی ();
	لکھو ("    %u lookup(s) found in table\n", عدد_lookups);
	لكل (بنانشانی n_lookup = 0; n_lookup < عدد_lookups; ++n_lookup)
	{
	  ثابت Lookup &lookup = g.get_lookup (n_lookup);
	  لکھو ("    Lookup %2u of %2u: type %u, props 0x%04X\n", n_lookup, عدد_lookups,
		  lookup.get_type (), lookup.get_props ());
	}

	}
	اكسر;

      حالة GDEF::جدول_ٹیگ:
	{

	ثابت GDEF &gdef = *reinterpret_cast<ثابت GDEF *> (font_انکڑے + جدول.وتھ);

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
	اكسر;
	}
      }
    }
  }
}
/* end of private API use */
#جے_انت

صحيح
مکھ (صحيح argc, حرف **argv)
{
  إذا (argc != 2)
  {
    fprintf (stderr, "usage: %s font-file.ttf\n\n"
		     "This tools is unsupported and crashes on bad data.\nDon't use it.\n", argv[0]);
    باہر_نکلو (1);
  }

  hb_blob_t *blob = hb_blob_create_from_file_or_fail (argv[1]);
  أكد (blob);
  لکھو ("Opened font file %s: %u bytes long\n", argv[1], hb_blob_get_لمائی (blob));
#جے_بنا_تعریف MAIN_CC_NO_نجی_API
  print_layout_info_using_نجی_api (blob);
#جے_انت
#إذا !defined(HB_NO_COLOR) && !defined(HB_NO_DRAW)
  dump_glyphs (blob, argv[1]);
#جے_انت
  hb_blob_destroy (blob);

  أرجع 0;
}
