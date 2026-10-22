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

#اگر_تعریف_شده HAVE_CONFIG_H
#ضمن "config.h"
#پایان_اگر

#ضمن "hb.h"
#ضمن "hb-ot.h"

#ضمن <cassert>
#ضمن <cstdlib>
#ضمن <cstdio>
#ضمن <cstring>

#اگر_تعریف_شده HB_NO_OPEN
#تعریف_کن hb_blob_create_from_file_or_fail(x)  hb_blob_get_خالی ()
#پایان_اگر

#إذا !defined(HB_NO_COLOR) && !defined(HB_NO_DRAW)
#اگر_تعریف_نشده HB_NO_SVG
ساكن فراغ
svg_dump (hb_face_t *face, بیعلامت face_شاخص)
{
  بیعلامت glyph_شمار = hb_face_get_glyph_شمار (face);

  لكل (بیعلامت glyph_id = 0; glyph_id < glyph_شمار; ++glyph_id)
  {
    hb_blob_t *blob = hb_ot_color_glyph_reference_svg (face, glyph_id);

    إذا (hb_blob_get_درازی (blob) == 0) استمر;

    بیعلامت درازی;
    ثابت حرف *داده = hb_blob_get_داده (blob, &درازی);

    حرف output_مسیر[255];
    snprintf (output_مسیر, حجم output_مسیر,
	     "out/svg-%u-%u.svg%s",
	     glyph_id,
	     face_شاخص,
	     // append "z" if the content is gzipped, https://stackoverflow.com/a/6059405
	     (درازی > 2 && (داده[0] == '\x1F') && (داده[1] == '\x8B')) ? "z" : "");

    FILE *f = fopen (output_مسیر, "wb");
    fwrite (داده, 1, درازی, f);
    fclose (f);

    hb_blob_destroy (blob);
  }
}
#پایان_اگر

/* _png API is so easy to use unlike the below code, don't get confused */
ساكن فراغ
png_dump (hb_face_t *face, بیعلامت face_شاخص)
{
  بیعلامت glyph_شمار = hb_face_get_glyph_شمار (face);
  hb_font_t *font = hb_font_create (face);

  /* scans the font for strikes */
  بیعلامت sample_glyph_id;
  /* we don't care about different strikes for different glyphs at this point */
  لكل (sample_glyph_id = 0; sample_glyph_id < glyph_شمار; ++sample_glyph_id)
  {
    hb_blob_t *blob = hb_ot_color_glyph_reference_png (font, sample_glyph_id);
    بیعلامت blob_درازی = hb_blob_get_درازی (blob);
    hb_blob_destroy (blob);
    إذا (blob_درازی != 0)
      اكسر;
  }

  بیعلامت upem = hb_face_get_upem (face);
  بیعلامت blob_درازی = 0;
  بیعلامت strike = 0;
  لكل (بیعلامت ppem = 1; ppem < upem; ++ppem)
  {
    hb_font_مجموعه_ppem (font, ppem, ppem);
    hb_blob_t *blob = hb_ot_color_glyph_reference_png (font, sample_glyph_id);
    بیعلامت new_blob_درازی = hb_blob_get_درازی (blob);
    hb_blob_destroy (blob);
    إذا (new_blob_درازی != blob_درازی)
    {
      لكل (بیعلامت glyph_id = 0; glyph_id < glyph_شمار; ++glyph_id)
      {
	hb_blob_t *blob = hb_ot_color_glyph_reference_png (font, glyph_id);

	إذا (hb_blob_get_درازی (blob) == 0) استمر;

	بیعلامت درازی;
	ثابت حرف *داده = hb_blob_get_داده (blob, &درازی);

	حرف output_مسیر[255];
	snprintf (output_مسیر, حجم output_مسیر, "out/png-%u-%u-%u.png", glyph_id, strike, face_شاخص);

	FILE *f = fopen (output_مسیر, "wb");
	fwrite (داده, 1, درازی, f);
	fclose (f);

	hb_blob_destroy (blob);
      }

      strike++;
      blob_درازی = new_blob_درازی;
    }
  }

  hb_font_destroy (font);
}

بنية draw_داده_t
{
  FILE *f;
  hb_position_t ascender;
};

ساكن فراغ
move_مقصد (hb_draw_funcs_t *, draw_داده_t *draw_داده,
	 hb_draw_state_t *,
	 عائم مقصد_x, عائم مقصد_y,
	 فراغ *)
{
  fprintf (draw_داده->f, "M%g,%g", مقصد_x, draw_داده->ascender - مقصد_y);
}

ساكن فراغ
سطر_مقصد (hb_draw_funcs_t *, draw_داده_t *draw_داده,
	 hb_draw_state_t *,
	 عائم مقصد_x, عائم مقصد_y,
	 فراغ *)
{
  fprintf (draw_داده->f, "L%g,%g", مقصد_x, draw_داده->ascender - مقصد_y);
}

ساكن فراغ
quadratic_مقصد (hb_draw_funcs_t *, draw_داده_t *draw_داده,
	      hb_draw_state_t *,
	      عائم control_x, عائم control_y,
	      عائم مقصد_x, عائم مقصد_y,
	      فراغ *)
{
  fprintf (draw_داده->f, "Q%g,%g %g,%g", control_x, draw_داده->ascender - control_y,
					مقصد_x, draw_داده->ascender - مقصد_y);
}

ساكن فراغ
cubic_مقصد (hb_draw_funcs_t *, draw_داده_t *draw_داده,
	  hb_draw_state_t *,
	  عائم control1_x, عائم control1_y,
	  عائم control2_x, عائم control2_y,
	  عائم مقصد_x, عائم مقصد_y,
	  فراغ *)
{
  fprintf (draw_داده->f, "C%g,%g %g,%g %g,%g", control1_x, draw_داده->ascender - control1_y,
					       control2_x, draw_داده->ascender - control2_y,
					       مقصد_x, draw_داده->ascender - مقصد_y);
}

ساكن فراغ
close_مسیر (hb_draw_funcs_t *, draw_داده_t *draw_داده,
	    hb_draw_state_t *,
	    فراغ *)
{
  fprintf (draw_داده->f, "Z");
}

ساكن فراغ
layered_glyph_dump (hb_font_t *font, hb_draw_funcs_t *funcs, بیعلامت face_شاخص)
{
  hb_face_t *face = hb_font_get_face (font);
  بیعلامت palette_شمار = hb_ot_color_palette_get_شمار (face);
  لكل (بیعلامت palette = 0; palette < palette_شمار; ++palette)
  {
    بیعلامت رقم_colors = hb_ot_color_palette_get_colors (face, palette, 0, اشاره_تهی, اشاره_تهی);
    إذا (!رقم_colors) استمر;

    hb_color_t *colors = (hb_color_t*) حافظه_صفر_بده (رقم_colors, حجم (hb_color_t));
    hb_ot_color_palette_get_colors (face, palette, 0, &رقم_colors, colors);
    إذا (!رقم_colors)
    {
      حرر (colors);
      استمر;
    }

    بیعلامت رقم_glyphs = hb_face_get_glyph_شمار (face);
    لكل (hb_codepoint_t gid = 0; gid < رقم_glyphs; ++gid)
    {
      بیعلامت رقم_layers = hb_ot_color_glyph_get_layers (face, gid, 0, اشاره_تهی, اشاره_تهی);
      إذا (!رقم_layers) استمر;

      hb_ot_color_layer_t *layers = (hb_ot_color_layer_t*) حافظه_بده (رقم_layers * حجم (hb_ot_color_layer_t));

      hb_ot_color_glyph_get_layers (face, gid, 0, &رقم_layers, layers);
      إذا (رقم_layers)
      {
	hb_font_extents_t font_extents;
	hb_font_get_extents_for_direction (font, HB_DIRECTION_LTR, &font_extents);
	hb_glyph_extents_t extents = {0, 0, 0, 0};
	إذا (!hb_font_get_glyph_extents (font, gid, &extents))
	{
	  چاپ_کن ("Skip gid: %u\n", gid);
	  استمر;
	}

	حرف output_مسیر[255];
	snprintf (output_مسیر, حجم output_مسیر, "out/colr-%u-%u-%u.svg", gid, palette, face_شاخص);
	FILE *f = fopen (output_مسیر, "wb");
	fprintf (f, "<svg xmlns=\"http://www.w3.org/2000/svg\""
		    " viewBox=\"%d %d %lld %lld\">\n",
		    extents.x_bearing, 0,
		    (دراز دراز) extents.x_bearing + extents.عرض,
		    -(دراز دراز) extents.ارتفاع);
	draw_داده_t draw_داده;
	draw_داده.ascender = extents.y_bearing;
	draw_داده.f = f;

	لكل (بیعلامت layer = 0; layer < رقم_layers; ++layer)
	{
	  hb_color_t color = 0x000000FF;
	  إذا (layers[layer].color_شاخص != 0xFFFF)
	    color = colors[layers[layer].color_شاخص];
	  fprintf (f, "<path fill=\"#%02X%02X%02X\" ",
		   hb_color_get_red (color), hb_color_get_green (color), hb_color_get_green (color));
	  إذا (hb_color_get_alpha (color) != 255)
	    fprintf (f, "fill-opacity=\"%.3f\"", (مزدوج) hb_color_get_alpha (color) / 255.);
	  fprintf (f, "d=\"");
	  hb_font_draw_glyph (font, layers[layer].glyph, funcs, &draw_داده);
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
dump_glyphs (hb_font_t *font, hb_draw_funcs_t *funcs, بیعلامت face_شاخص)
{
  بیعلامت رقم_glyphs = hb_face_get_glyph_شمار (hb_font_get_face (font));
  لكل (بیعلامت gid = 0; gid < رقم_glyphs; ++gid)
  {
    hb_font_extents_t font_extents;
    hb_font_get_extents_for_direction (font, HB_DIRECTION_LTR, &font_extents);
    hb_glyph_extents_t extents = {0, 0, 0, 0};
    إذا (!hb_font_get_glyph_extents (font, gid, &extents))
    {
      چاپ_کن ("Skip gid: %u\n", gid);
      استمر;
    }

    حرف output_مسیر[255];
    snprintf (output_مسیر, حجم output_مسیر, "out/%u-%u.svg", face_شاخص, gid);
    FILE *f = fopen (output_مسیر, "wb");
    fprintf (f, "<svg xmlns=\"http://www.w3.org/2000/svg\""
		" viewBox=\"%d %d %lld %lld\"><path d=\"",
		extents.x_bearing, 0,
		(دراز دراز) extents.x_bearing + extents.عرض,
		(دراز دراز) font_extents.ascender - font_extents.descender);
    draw_داده_t draw_داده;
    draw_داده.ascender = font_extents.ascender;
    draw_داده.f = f;
    hb_font_draw_glyph (font, gid, funcs, &draw_داده);
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
  fwrite (font_name, 1, طول_رشته (font_name), font_name_file);
  fclose (font_name_file);

  hb_draw_funcs_t *funcs = hb_draw_funcs_create ();
  hb_draw_funcs_مجموعه_move_مقصد_func (funcs, (hb_draw_move_مقصد_func_t) move_مقصد, اشاره_تهی, اشاره_تهی);
  hb_draw_funcs_مجموعه_سطر_مقصد_func (funcs, (hb_draw_سطر_مقصد_func_t) سطر_مقصد, اشاره_تهی, اشاره_تهی);
  hb_draw_funcs_مجموعه_quadratic_مقصد_func (funcs, (hb_draw_quadratic_مقصد_func_t) quadratic_مقصد, اشاره_تهی, اشاره_تهی);
  hb_draw_funcs_مجموعه_cubic_مقصد_func (funcs, (hb_draw_cubic_مقصد_func_t) cubic_مقصد, اشاره_تهی, اشاره_تهی);
  hb_draw_funcs_مجموعه_close_مسیر_func (funcs, (hb_draw_close_مسیر_func_t) close_مسیر, اشاره_تهی, اشاره_تهی);

  بیعلامت رقم_faces = hb_face_شمار (blob);
  لكل (بیعلامت face_شاخص = 0; face_شاخص < رقم_faces; ++face_شاخص)
  {
    hb_face_t *face = hb_face_create (blob, face_شاخص);
    hb_font_t *font = hb_font_create (face);

    إذا (hb_ot_color_has_png (face))
      چاپ_کن ("Dumping png (CBDT/sbix)...\n");
    png_dump (face, face_شاخص);

#اگر_تعریف_نشده HB_NO_SVG
    إذا (hb_ot_color_has_svg (face))
      چاپ_کن ("Dumping svg (SVG )...\n");
    svg_dump (face, face_شاخص);
#پایان_اگر

    إذا (hb_ot_color_has_layers (face) && hb_ot_color_has_palettes (face))
      چاپ_کن ("Dumping layered color glyphs (COLR/CPAL)...\n");
    layered_glyph_dump (font, funcs, face_شاخص);

    dump_glyphs (font, funcs, face_شاخص);

    hb_font_destroy (font);
    hb_face_destroy (face);
  }

  hb_draw_funcs_destroy (funcs);
}
#پایان_اگر

#اگر_تعریف_نشده MAIN_CC_NO_خصوصی_API
/* Only this part of this mini app uses private API */
#ضمن "hb-static.cc"
#ضمن "hb-open-file.hh"
#ضمن "hb-ot-layout-gdef-table.hh"
#ضمن "hb-ot-layout-gsubgpos.hh"

استفاده نطاق OT;

ساكن فراغ
print_layout_info_using_خصوصی_api (hb_blob_t *blob)
{
  ثابت حرف *font_داده = hb_blob_get_داده (blob, اشاره_تهی);
  hb_blob_t *font_blob = hb_sanitize_context_t ().sanitize_blob<OpenTypeFontFile> (blob);
  ثابت OpenTypeFontFile* sanitized = font_blob->as<OpenTypeFontFile> ();
  إذا (!font_blob->داده)
  {
    چاپ_کن ("Sanitization of the file wasn't successful. Exit");
    خروج (1);
  }
  ثابت OpenTypeFontFile& ot = *sanitized;

  بدل (ot.get_برچسب ())
  {
  حالة OpenTypeFontFile::True_Type_برچسب:
    چاپ_کن ("OpenType font with TrueType outlines\n");
    اكسر;
  حالة OpenTypeFontFile::CFFTag:
    چاپ_کن ("OpenType font with CFF (Type1) outlines\n");
    اكسر;
  حالة OpenTypeFontFile::TTCTag:
    چاپ_کن ("TrueType Collection of OpenType fonts\n");
    اكسر;
  حالة OpenTypeFontFile::True_برچسب:
    چاپ_کن ("Obsolete Apple TrueType font\n");
    اكسر;
  حالة OpenTypeFontFile::Typ1_برچسب:
    چاپ_کن ("Obsolete Apple Type1 font in SFNT container\n");
    اكسر;
  حالة OpenTypeFontFile::DFont_برچسب:
    چاپ_کن ("DFont Mac Resource Fork\n");
    اكسر;
  افتراضي:
    چاپ_کن ("Unknown font format\n");
    اكسر;
  }

  بیعلامت رقم_faces = hb_face_شمار (blob);
  چاپ_کن ("%u font(s) found in file\n", رقم_faces);
  لكل (بیعلامت n_font = 0; n_font < رقم_faces; ++n_font)
  {
    ثابت OpenTypeFontFace &font = ot.get_face (n_font);
    چاپ_کن ("Font %u of %u:\n", n_font, رقم_faces);

    بیعلامت رقم_tables = font.get_جدول_شمار ();
    چاپ_کن ("  %u table(s) found in font\n", رقم_tables);
    لكل (بیعلامت n_جدول = 0; n_جدول < رقم_tables; ++n_جدول)
    {
      ثابت Open_Type_جدول &جدول = font.get_جدول (n_جدول);
      چاپ_کن ("  Table %2u of %2u: %.4s (0x%08x+0x%08x)\n", n_جدول, رقم_tables,
	      (ثابت حرف *) جدول.برچسب_2,
	      (بیعلامت) جدول.جابه_جایی,
	      (بیعلامت) جدول.درازی);

      بدل (جدول.برچسب_2)
      {

      حالة HB_OT_برچسب_GSUB:
      حالة HB_OT_برچسب_GPOS:
	{

	ثابت GSUBGPOS &g = *reinterpret_cast<ثابت GSUBGPOS *> (font_داده + جدول.جابه_جایی);

	بیعلامت رقم_scripts = g.get_script_شمار ();
	چاپ_کن ("    %u script(s) found in table\n", رقم_scripts);
	لكل (بیعلامت n_script = 0; n_script < رقم_scripts; ++n_script)
	{
	  ثابت Script &script = g.get_script (n_script);
	  چاپ_کن ("    Script %2u of %2u: %.4s\n", n_script, رقم_scripts,
		  (ثابت حرف *) g.get_script_برچسب (n_script));

	  إذا (!script.has_default_lang_sys ())
	    چاپ_کن ("      No default language system\n");
	  صحيح رقم_langsys = script.get_lang_sys_شمار ();
	  چاپ_کن ("      %d language system(s) found in script\n", رقم_langsys);
	  لكل (صحيح n_langsys = script.has_default_lang_sys () ? -1 : 0; n_langsys < رقم_langsys; ++n_langsys)
	  {
	    ثابت LangSys &langsys = n_langsys == -1
				   ? script.get_default_lang_sys ()
				   : script.get_lang_sys (n_langsys);
	    إذا (n_langsys == -1)
	      چاپ_کن ("      Default Language System\n");
	    وإلا
	      چاپ_کن ("      Language System %2d of %2d: %.4s\n", n_langsys, رقم_langsys,
		      (ثابت حرف *) script.get_lang_sys_برچسب (n_langsys));
	    إذا (!langsys.has_required_feature ())
	      چاپ_کن ("        No required feature\n");
	    وإلا
	      چاپ_کن ("        Required feature index: %u\n",
		      langsys.get_required_feature_شاخص ());

	    بیعلامت رقم_features = langsys.get_feature_شمار ();
	    چاپ_کن ("        %u feature(s) found in language system\n", رقم_features);
	    لكل (بیعلامت n_feature = 0; n_feature < رقم_features; ++n_feature)
	    {
	      چاپ_کن ("        Feature index %2u of %2u: %u\n", n_feature, رقم_features,
		      langsys.get_feature_شاخص (n_feature));
	    }
	  }
	}

	بیعلامت رقم_features = g.get_feature_شمار ();
	چاپ_کن ("    %u feature(s) found in table\n", رقم_features);
	لكل (بیعلامت n_feature = 0; n_feature < رقم_features; ++n_feature)
	{
	  ثابت Feature &feature = g.get_feature (n_feature);
	  بیعلامت رقم_lookups = feature.get_lookup_شمار ();
	  چاپ_کن ("    Feature %2u of %2u: %c%c%c%c\n", n_feature, رقم_features,
		  HB_UNTAG (g.get_feature_برچسب (n_feature)));

	  چاپ_کن ("        %u lookup(s) found in feature\n", رقم_lookups);
	  لكل (بیعلامت n_lookup = 0; n_lookup < رقم_lookups; ++n_lookup) {
	    چاپ_کن ("        Lookup index %2u of %2u: %u\n", n_lookup, رقم_lookups,
		    feature.get_lookup_شاخص (n_lookup));
	  }
	}

	بیعلامت رقم_lookups = g.get_lookup_شمار ();
	چاپ_کن ("    %u lookup(s) found in table\n", رقم_lookups);
	لكل (بیعلامت n_lookup = 0; n_lookup < رقم_lookups; ++n_lookup)
	{
	  ثابت Lookup &lookup = g.get_lookup (n_lookup);
	  چاپ_کن ("    Lookup %2u of %2u: type %u, props 0x%04X\n", n_lookup, رقم_lookups,
		  lookup.get_type (), lookup.get_props ());
	}

	}
	اكسر;

      حالة GDEF::جدول_برچسب:
	{

	ثابت GDEF &gdef = *reinterpret_cast<ثابت GDEF *> (font_داده + جدول.جابه_جایی);

	چاپ_کن ("    Has %sglyph classes\n",
		  gdef.has_glyph_classes () ? "" : "no ");
	چاپ_کن ("    Has %smark attachment types\n",
		  gdef.has_mark_attachment_types () ? "" : "no ");
	چاپ_کن ("    Has %sattach list\n",
		  gdef.has_attach_list () ? "" : "no ");
	چاپ_کن ("    Has %slig carets\n",
		  gdef.has_lig_carets () ? "" : "no ");
	چاپ_کن ("    Has %smark glyph sets\n",
		  gdef.has_mark_glyph_sets () ? "" : "no ");
	اكسر;
	}
      }
    }
  }
}
/* end of private API use */
#پایان_اگر

صحيح
اصلی (صحيح argc, حرف **argv)
{
  إذا (argc != 2)
  {
    fprintf (stderr, "usage: %s font-file.ttf\n\n"
		     "This tools is unsupported and crashes on bad data.\nDon't use it.\n", argv[0]);
    خروج (1);
  }

  hb_blob_t *blob = hb_blob_create_from_file_or_fail (argv[1]);
  أكد (blob);
  چاپ_کن ("Opened font file %s: %u bytes long\n", argv[1], hb_blob_get_درازی (blob));
#اگر_تعریف_نشده MAIN_CC_NO_خصوصی_API
  print_layout_info_using_خصوصی_api (blob);
#پایان_اگر
#إذا !defined(HB_NO_COLOR) && !defined(HB_NO_DRAW)
  dump_glyphs (blob, argv[1]);
#پایان_اگر
  hb_blob_destroy (blob);

  أرجع 0;
}
