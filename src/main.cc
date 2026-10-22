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

#که_تعریف_شوی HAVE_CONFIG_H
#ضمن "config.h"
#د_که_پای

#ضمن "hb.h"
#ضمن "hb-ot.h"

#ضمن <cassert>
#ضمن <cstdlib>
#ضمن <cstdio>
#ضمن <cstring>

#که_تعریف_شوی HB_NO_OPEN
#تعریف_کړه hb_blob_create_from_file_or_fail(x)  hb_blob_get_تشه ()
#د_که_پای

#إذا !defined(HB_NO_COLOR) && !defined(HB_NO_DRAW)
#که_نه_تعریف_شوی HB_NO_SVG
ساكن فراغ
svg_dump (hb_face_t *face, بینښه face_شاخص)
{
  بینښه glyph_شمېر = hb_face_get_glyph_شمېر (face);

  لكل (بینښه glyph_id = 0; glyph_id < glyph_شمېر; ++glyph_id)
  {
    hb_blob_t *blob = hb_ot_color_glyph_reference_svg (face, glyph_id);

    إذا (hb_blob_get_اوږدتیا (blob) == 0) استمر;

    بینښه اوږدتیا;
    ثابت حرف *معلومات = hb_blob_get_معلومات (blob, &اوږدتیا);

    حرف output_لار[255];
    snprintf (output_لار, حجم output_لار,
	     "out/svg-%u-%u.svg%s",
	     glyph_id,
	     face_شاخص,
	     // append "z" if the content is gzipped, https://stackoverflow.com/a/6059405
	     (اوږدتیا > 2 && (معلومات[0] == '\x1F') && (معلومات[1] == '\x8B')) ? "z" : "");

    FILE *f = fopen (output_لار, "wb");
    fwrite (معلومات, 1, اوږدتیا, f);
    fclose (f);

    hb_blob_destroy (blob);
  }
}
#د_که_پای

/* _png API is so easy to use unlike the below code, don't get confused */
ساكن فراغ
png_dump (hb_face_t *face, بینښه face_شاخص)
{
  بینښه glyph_شمېر = hb_face_get_glyph_شمېر (face);
  hb_font_t *font = hb_font_create (face);

  /* scans the font for strikes */
  بینښه sample_glyph_id;
  /* we don't care about different strikes for different glyphs at this point */
  لكل (sample_glyph_id = 0; sample_glyph_id < glyph_شمېر; ++sample_glyph_id)
  {
    hb_blob_t *blob = hb_ot_color_glyph_reference_png (font, sample_glyph_id);
    بینښه blob_اوږدتیا = hb_blob_get_اوږدتیا (blob);
    hb_blob_destroy (blob);
    إذا (blob_اوږدتیا != 0)
      اكسر;
  }

  بینښه upem = hb_face_get_upem (face);
  بینښه blob_اوږدتیا = 0;
  بینښه strike = 0;
  لكل (بینښه ppem = 1; ppem < upem; ++ppem)
  {
    hb_font_سېټ_ppem (font, ppem, ppem);
    hb_blob_t *blob = hb_ot_color_glyph_reference_png (font, sample_glyph_id);
    بینښه new_blob_اوږدتیا = hb_blob_get_اوږدتیا (blob);
    hb_blob_destroy (blob);
    إذا (new_blob_اوږدتیا != blob_اوږدتیا)
    {
      لكل (بینښه glyph_id = 0; glyph_id < glyph_شمېر; ++glyph_id)
      {
	hb_blob_t *blob = hb_ot_color_glyph_reference_png (font, glyph_id);

	إذا (hb_blob_get_اوږدتیا (blob) == 0) استمر;

	بینښه اوږدتیا;
	ثابت حرف *معلومات = hb_blob_get_معلومات (blob, &اوږدتیا);

	حرف output_لار[255];
	snprintf (output_لار, حجم output_لار, "out/png-%u-%u-%u.png", glyph_id, strike, face_شاخص);

	FILE *f = fopen (output_لار, "wb");
	fwrite (معلومات, 1, اوږدتیا, f);
	fclose (f);

	hb_blob_destroy (blob);
      }

      strike++;
      blob_اوږدتیا = new_blob_اوږدتیا;
    }
  }

  hb_font_destroy (font);
}

بنية draw_معلومات_t
{
  FILE *f;
  hb_position_t ascender;
};

ساكن فراغ
move_منزل (hb_draw_funcs_t *, draw_معلومات_t *draw_معلومات,
	 hb_draw_state_t *,
	 عائم منزل_x, عائم منزل_y,
	 فراغ *)
{
  fprintf (draw_معلومات->f, "M%g,%g", منزل_x, draw_معلومات->ascender - منزل_y);
}

ساكن فراغ
کرښه_منزل (hb_draw_funcs_t *, draw_معلومات_t *draw_معلومات,
	 hb_draw_state_t *,
	 عائم منزل_x, عائم منزل_y,
	 فراغ *)
{
  fprintf (draw_معلومات->f, "L%g,%g", منزل_x, draw_معلومات->ascender - منزل_y);
}

ساكن فراغ
quadratic_منزل (hb_draw_funcs_t *, draw_معلومات_t *draw_معلومات,
	      hb_draw_state_t *,
	      عائم control_x, عائم control_y,
	      عائم منزل_x, عائم منزل_y,
	      فراغ *)
{
  fprintf (draw_معلومات->f, "Q%g,%g %g,%g", control_x, draw_معلومات->ascender - control_y,
					منزل_x, draw_معلومات->ascender - منزل_y);
}

ساكن فراغ
cubic_منزل (hb_draw_funcs_t *, draw_معلومات_t *draw_معلومات,
	  hb_draw_state_t *,
	  عائم control1_x, عائم control1_y,
	  عائم control2_x, عائم control2_y,
	  عائم منزل_x, عائم منزل_y,
	  فراغ *)
{
  fprintf (draw_معلومات->f, "C%g,%g %g,%g %g,%g", control1_x, draw_معلومات->ascender - control1_y,
					       control2_x, draw_معلومات->ascender - control2_y,
					       منزل_x, draw_معلومات->ascender - منزل_y);
}

ساكن فراغ
close_لار (hb_draw_funcs_t *, draw_معلومات_t *draw_معلومات,
	    hb_draw_state_t *,
	    فراغ *)
{
  fprintf (draw_معلومات->f, "Z");
}

ساكن فراغ
layered_glyph_dump (hb_font_t *font, hb_draw_funcs_t *funcs, بینښه face_شاخص)
{
  hb_face_t *face = hb_font_get_face (font);
  بینښه palette_شمېر = hb_ot_color_palette_get_شمېر (face);
  لكل (بینښه palette = 0; palette < palette_شمېر; ++palette)
  {
    بینښه عدد_colors = hb_ot_color_palette_get_colors (face, palette, 0, تش_اشاره, تش_اشاره);
    إذا (!عدد_colors) استمر;

    hb_color_t *colors = (hb_color_t*) حافظه_صفر_ورکړه (عدد_colors, حجم (hb_color_t));
    hb_ot_color_palette_get_colors (face, palette, 0, &عدد_colors, colors);
    إذا (!عدد_colors)
    {
      حرر (colors);
      استمر;
    }

    بینښه عدد_glyphs = hb_face_get_glyph_شمېر (face);
    لكل (hb_codepoint_t gid = 0; gid < عدد_glyphs; ++gid)
    {
      بینښه عدد_layers = hb_ot_color_glyph_get_layers (face, gid, 0, تش_اشاره, تش_اشاره);
      إذا (!عدد_layers) استمر;

      hb_ot_color_layer_t *layers = (hb_ot_color_layer_t*) حافظه_ورکړه (عدد_layers * حجم (hb_ot_color_layer_t));

      hb_ot_color_glyph_get_layers (face, gid, 0, &عدد_layers, layers);
      إذا (عدد_layers)
      {
	hb_font_extents_t font_extents;
	hb_font_get_extents_for_direction (font, HB_DIRECTION_LTR, &font_extents);
	hb_glyph_extents_t extents = {0, 0, 0, 0};
	إذا (!hb_font_get_glyph_extents (font, gid, &extents))
	{
	  چاپ_کړه ("Skip gid: %u\n", gid);
	  استمر;
	}

	حرف output_لار[255];
	snprintf (output_لار, حجم output_لار, "out/colr-%u-%u-%u.svg", gid, palette, face_شاخص);
	FILE *f = fopen (output_لار, "wb");
	fprintf (f, "<svg xmlns=\"http://www.w3.org/2000/svg\""
		    " viewBox=\"%d %d %lld %lld\">\n",
		    extents.x_bearing, 0,
		    (اوږد اوږد) extents.x_bearing + extents.پلنوالی,
		    -(اوږد اوږد) extents.لوړوالی);
	draw_معلومات_t draw_معلومات;
	draw_معلومات.ascender = extents.y_bearing;
	draw_معلومات.f = f;

	لكل (بینښه layer = 0; layer < عدد_layers; ++layer)
	{
	  hb_color_t color = 0x000000FF;
	  إذا (layers[layer].color_شاخص != 0xFFFF)
	    color = colors[layers[layer].color_شاخص];
	  fprintf (f, "<path fill=\"#%02X%02X%02X\" ",
		   hb_color_get_red (color), hb_color_get_green (color), hb_color_get_green (color));
	  إذا (hb_color_get_alpha (color) != 255)
	    fprintf (f, "fill-opacity=\"%.3f\"", (مزدوج) hb_color_get_alpha (color) / 255.);
	  fprintf (f, "d=\"");
	  hb_font_draw_glyph (font, layers[layer].glyph, funcs, &draw_معلومات);
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
dump_glyphs (hb_font_t *font, hb_draw_funcs_t *funcs, بینښه face_شاخص)
{
  بینښه عدد_glyphs = hb_face_get_glyph_شمېر (hb_font_get_face (font));
  لكل (بینښه gid = 0; gid < عدد_glyphs; ++gid)
  {
    hb_font_extents_t font_extents;
    hb_font_get_extents_for_direction (font, HB_DIRECTION_LTR, &font_extents);
    hb_glyph_extents_t extents = {0, 0, 0, 0};
    إذا (!hb_font_get_glyph_extents (font, gid, &extents))
    {
      چاپ_کړه ("Skip gid: %u\n", gid);
      استمر;
    }

    حرف output_لار[255];
    snprintf (output_لار, حجم output_لار, "out/%u-%u.svg", face_شاخص, gid);
    FILE *f = fopen (output_لار, "wb");
    fprintf (f, "<svg xmlns=\"http://www.w3.org/2000/svg\""
		" viewBox=\"%d %d %lld %lld\"><path d=\"",
		extents.x_bearing, 0,
		(اوږد اوږد) extents.x_bearing + extents.پلنوالی,
		(اوږد اوږد) font_extents.ascender - font_extents.descender);
    draw_معلومات_t draw_معلومات;
    draw_معلومات.ascender = font_extents.ascender;
    draw_معلومات.f = f;
    hb_font_draw_glyph (font, gid, funcs, &draw_معلومات);
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
  fwrite (font_name, 1, مزي_اوږدوالی (font_name), font_name_file);
  fclose (font_name_file);

  hb_draw_funcs_t *funcs = hb_draw_funcs_create ();
  hb_draw_funcs_سېټ_move_منزل_func (funcs, (hb_draw_move_منزل_func_t) move_منزل, تش_اشاره, تش_اشاره);
  hb_draw_funcs_سېټ_کرښه_منزل_func (funcs, (hb_draw_کرښه_منزل_func_t) کرښه_منزل, تش_اشاره, تش_اشاره);
  hb_draw_funcs_سېټ_quadratic_منزل_func (funcs, (hb_draw_quadratic_منزل_func_t) quadratic_منزل, تش_اشاره, تش_اشاره);
  hb_draw_funcs_سېټ_cubic_منزل_func (funcs, (hb_draw_cubic_منزل_func_t) cubic_منزل, تش_اشاره, تش_اشاره);
  hb_draw_funcs_سېټ_close_لار_func (funcs, (hb_draw_close_لار_func_t) close_لار, تش_اشاره, تش_اشاره);

  بینښه عدد_faces = hb_face_شمېر (blob);
  لكل (بینښه face_شاخص = 0; face_شاخص < عدد_faces; ++face_شاخص)
  {
    hb_face_t *face = hb_face_create (blob, face_شاخص);
    hb_font_t *font = hb_font_create (face);

    إذا (hb_ot_color_has_png (face))
      چاپ_کړه ("Dumping png (CBDT/sbix)...\n");
    png_dump (face, face_شاخص);

#که_نه_تعریف_شوی HB_NO_SVG
    إذا (hb_ot_color_has_svg (face))
      چاپ_کړه ("Dumping svg (SVG )...\n");
    svg_dump (face, face_شاخص);
#د_که_پای

    إذا (hb_ot_color_has_layers (face) && hb_ot_color_has_palettes (face))
      چاپ_کړه ("Dumping layered color glyphs (COLR/CPAL)...\n");
    layered_glyph_dump (font, funcs, face_شاخص);

    dump_glyphs (font, funcs, face_شاخص);

    hb_font_destroy (font);
    hb_face_destroy (face);
  }

  hb_draw_funcs_destroy (funcs);
}
#د_که_پای

#که_نه_تعریف_شوی MAIN_CC_NO_شخصي_API
/* Only this part of this mini app uses private API */
#ضمن "hb-static.cc"
#ضمن "hb-open-file.hh"
#ضمن "hb-ot-layout-gdef-table.hh"
#ضمن "hb-ot-layout-gsubgpos.hh"

کارول نطاق OT;

ساكن فراغ
print_layout_info_using_شخصي_api (hb_blob_t *blob)
{
  ثابت حرف *font_معلومات = hb_blob_get_معلومات (blob, تش_اشاره);
  hb_blob_t *font_blob = hb_sanitize_context_t ().sanitize_blob<OpenTypeFontFile> (blob);
  ثابت OpenTypeFontFile* sanitized = font_blob->as<OpenTypeFontFile> ();
  إذا (!font_blob->معلومات)
  {
    چاپ_کړه ("Sanitization of the file wasn't successful. Exit");
    وتل (1);
  }
  ثابت OpenTypeFontFile& ot = *sanitized;

  بدل (ot.get_نښان ())
  {
  حالة OpenTypeFontFile::True_Type_نښان:
    چاپ_کړه ("OpenType font with TrueType outlines\n");
    اكسر;
  حالة OpenTypeFontFile::CFFTag:
    چاپ_کړه ("OpenType font with CFF (Type1) outlines\n");
    اكسر;
  حالة OpenTypeFontFile::TTCTag:
    چاپ_کړه ("TrueType Collection of OpenType fonts\n");
    اكسر;
  حالة OpenTypeFontFile::True_نښان:
    چاپ_کړه ("Obsolete Apple TrueType font\n");
    اكسر;
  حالة OpenTypeFontFile::Typ1_نښان:
    چاپ_کړه ("Obsolete Apple Type1 font in SFNT container\n");
    اكسر;
  حالة OpenTypeFontFile::DFont_نښان:
    چاپ_کړه ("DFont Mac Resource Fork\n");
    اكسر;
  افتراضي:
    چاپ_کړه ("Unknown font format\n");
    اكسر;
  }

  بینښه عدد_faces = hb_face_شمېر (blob);
  چاپ_کړه ("%u font(s) found in file\n", عدد_faces);
  لكل (بینښه n_font = 0; n_font < عدد_faces; ++n_font)
  {
    ثابت OpenTypeFontFace &font = ot.get_face (n_font);
    چاپ_کړه ("Font %u of %u:\n", n_font, عدد_faces);

    بینښه عدد_tables = font.get_جدول_شمېر ();
    چاپ_کړه ("  %u table(s) found in font\n", عدد_tables);
    لكل (بینښه n_جدول = 0; n_جدول < عدد_tables; ++n_جدول)
    {
      ثابت Open_Type_جدول &جدول = font.get_جدول (n_جدول);
      چاپ_کړه ("  Table %2u of %2u: %.4s (0x%08x+0x%08x)\n", n_جدول, عدد_tables,
	      (ثابت حرف *) جدول.نښان_2,
	      (بینښه) جدول.دننه_واټن,
	      (بینښه) جدول.اوږدتیا);

      بدل (جدول.نښان_2)
      {

      حالة HB_OT_نښان_GSUB:
      حالة HB_OT_نښان_GPOS:
	{

	ثابت GSUBGPOS &g = *reinterpret_cast<ثابت GSUBGPOS *> (font_معلومات + جدول.دننه_واټن);

	بینښه عدد_scripts = g.get_script_شمېر ();
	چاپ_کړه ("    %u script(s) found in table\n", عدد_scripts);
	لكل (بینښه n_script = 0; n_script < عدد_scripts; ++n_script)
	{
	  ثابت Script &script = g.get_script (n_script);
	  چاپ_کړه ("    Script %2u of %2u: %.4s\n", n_script, عدد_scripts,
		  (ثابت حرف *) g.get_script_نښان (n_script));

	  إذا (!script.has_default_lang_sys ())
	    چاپ_کړه ("      No default language system\n");
	  صحيح عدد_langsys = script.get_lang_sys_شمېر ();
	  چاپ_کړه ("      %d language system(s) found in script\n", عدد_langsys);
	  لكل (صحيح n_langsys = script.has_default_lang_sys () ? -1 : 0; n_langsys < عدد_langsys; ++n_langsys)
	  {
	    ثابت LangSys &langsys = n_langsys == -1
				   ? script.get_default_lang_sys ()
				   : script.get_lang_sys (n_langsys);
	    إذا (n_langsys == -1)
	      چاپ_کړه ("      Default Language System\n");
	    وإلا
	      چاپ_کړه ("      Language System %2d of %2d: %.4s\n", n_langsys, عدد_langsys,
		      (ثابت حرف *) script.get_lang_sys_نښان (n_langsys));
	    إذا (!langsys.has_required_feature ())
	      چاپ_کړه ("        No required feature\n");
	    وإلا
	      چاپ_کړه ("        Required feature index: %u\n",
		      langsys.get_required_feature_شاخص ());

	    بینښه عدد_features = langsys.get_feature_شمېر ();
	    چاپ_کړه ("        %u feature(s) found in language system\n", عدد_features);
	    لكل (بینښه n_feature = 0; n_feature < عدد_features; ++n_feature)
	    {
	      چاپ_کړه ("        Feature index %2u of %2u: %u\n", n_feature, عدد_features,
		      langsys.get_feature_شاخص (n_feature));
	    }
	  }
	}

	بینښه عدد_features = g.get_feature_شمېر ();
	چاپ_کړه ("    %u feature(s) found in table\n", عدد_features);
	لكل (بینښه n_feature = 0; n_feature < عدد_features; ++n_feature)
	{
	  ثابت Feature &feature = g.get_feature (n_feature);
	  بینښه عدد_lookups = feature.get_lookup_شمېر ();
	  چاپ_کړه ("    Feature %2u of %2u: %c%c%c%c\n", n_feature, عدد_features,
		  HB_UNTAG (g.get_feature_نښان (n_feature)));

	  چاپ_کړه ("        %u lookup(s) found in feature\n", عدد_lookups);
	  لكل (بینښه n_lookup = 0; n_lookup < عدد_lookups; ++n_lookup) {
	    چاپ_کړه ("        Lookup index %2u of %2u: %u\n", n_lookup, عدد_lookups,
		    feature.get_lookup_شاخص (n_lookup));
	  }
	}

	بینښه عدد_lookups = g.get_lookup_شمېر ();
	چاپ_کړه ("    %u lookup(s) found in table\n", عدد_lookups);
	لكل (بینښه n_lookup = 0; n_lookup < عدد_lookups; ++n_lookup)
	{
	  ثابت Lookup &lookup = g.get_lookup (n_lookup);
	  چاپ_کړه ("    Lookup %2u of %2u: type %u, props 0x%04X\n", n_lookup, عدد_lookups,
		  lookup.get_type (), lookup.get_props ());
	}

	}
	اكسر;

      حالة GDEF::جدول_نښان:
	{

	ثابت GDEF &gdef = *reinterpret_cast<ثابت GDEF *> (font_معلومات + جدول.دننه_واټن);

	چاپ_کړه ("    Has %sglyph classes\n",
		  gdef.has_glyph_classes () ? "" : "no ");
	چاپ_کړه ("    Has %smark attachment types\n",
		  gdef.has_mark_attachment_types () ? "" : "no ");
	چاپ_کړه ("    Has %sattach list\n",
		  gdef.has_attach_list () ? "" : "no ");
	چاپ_کړه ("    Has %slig carets\n",
		  gdef.has_lig_carets () ? "" : "no ");
	چاپ_کړه ("    Has %smark glyph sets\n",
		  gdef.has_mark_glyph_sets () ? "" : "no ");
	اكسر;
	}
      }
    }
  }
}
/* end of private API use */
#د_که_پای

صحيح
اصلي (صحيح argc, حرف **argv)
{
  إذا (argc != 2)
  {
    fprintf (stderr, "usage: %s font-file.ttf\n\n"
		     "This tools is unsupported and crashes on bad data.\nDon't use it.\n", argv[0]);
    وتل (1);
  }

  hb_blob_t *blob = hb_blob_create_from_file_or_fail (argv[1]);
  أكد (blob);
  چاپ_کړه ("Opened font file %s: %u bytes long\n", argv[1], hb_blob_get_اوږدتیا (blob));
#که_نه_تعریف_شوی MAIN_CC_NO_شخصي_API
  print_layout_info_using_شخصي_api (blob);
#د_که_پای
#إذا !defined(HB_NO_COLOR) && !defined(HB_NO_DRAW)
  dump_glyphs (blob, argv[1]);
#د_که_پای
  hb_blob_destroy (blob);

  أرجع 0;
}
