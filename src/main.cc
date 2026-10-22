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

#جيڪڏهن_وصف_ٿيل HAVE_CONFIG_H
#ضمن "config.h"
#جيڪڏهن_پورو

#ضمن "hb.h"
#ضمن "hb-ot.h"

#ضمن <cassert>
#ضمن <cstdlib>
#ضمن <cstdio>
#ضمن <cstring>

#جيڪڏهن_وصف_ٿيل HB_NO_OPEN
#وصف_ڪريو hb_blob_create_from_file_or_fail(x)  hb_blob_get_خالي_جايون ()
#جيڪڏهن_پورو

#إذا !defined(HB_NO_COLOR) && !defined(HB_NO_DRAW)
#جيڪڏهن_وصف_نه_ٿيل HB_NO_SVG
ساكن فراغ
svg_dump (hb_face_t *face, بنانشان face_اشاريو)
{
  بنانشان glyph_ڳڻپ = hb_face_get_glyph_ڳڻپ (face);

  لكل (بنانشان glyph_id = 0; glyph_id < glyph_ڳڻپ; ++glyph_id)
  {
    hb_blob_t *blob = hb_ot_color_glyph_reference_svg (face, glyph_id);

    إذا (hb_blob_get_ڊيگهائي (blob) == 0) استمر;

    بنانشان ڊيگهائي;
    ثابت حرف *معلومات = hb_blob_get_معلومات (blob, &ڊيگهائي);

    حرف output_رستو[255];
    snprintf (output_رستو, حجم output_رستو,
	     "out/svg-%u-%u.svg%s",
	     glyph_id,
	     face_اشاريو,
	     // append "z" if the content is gzipped, https://stackoverflow.com/a/6059405
	     (ڊيگهائي > 2 && (معلومات[0] == '\x1F') && (معلومات[1] == '\x8B')) ? "z" : "");

    FILE *f = fopen (output_رستو, "wb");
    fwrite (معلومات, 1, ڊيگهائي, f);
    fclose (f);

    hb_blob_destroy (blob);
  }
}
#جيڪڏهن_پورو

/* _png API is so easy to use unlike the below code, don't get confused */
ساكن فراغ
png_dump (hb_face_t *face, بنانشان face_اشاريو)
{
  بنانشان glyph_ڳڻپ = hb_face_get_glyph_ڳڻپ (face);
  hb_font_t *font = hb_font_create (face);

  /* scans the font for strikes */
  بنانشان sample_glyph_id;
  /* we don't care about different strikes for different glyphs at this point */
  لكل (sample_glyph_id = 0; sample_glyph_id < glyph_ڳڻپ; ++sample_glyph_id)
  {
    hb_blob_t *blob = hb_ot_color_glyph_reference_png (font, sample_glyph_id);
    بنانشان blob_ڊيگهائي = hb_blob_get_ڊيگهائي (blob);
    hb_blob_destroy (blob);
    إذا (blob_ڊيگهائي != 0)
      اكسر;
  }

  بنانشان upem = hb_face_get_upem (face);
  بنانشان blob_ڊيگهائي = 0;
  بنانشان strike = 0;
  لكل (بنانشان ppem = 1; ppem < upem; ++ppem)
  {
    hb_font_سيٽ_ppem (font, ppem, ppem);
    hb_blob_t *blob = hb_ot_color_glyph_reference_png (font, sample_glyph_id);
    بنانشان new_blob_ڊيگهائي = hb_blob_get_ڊيگهائي (blob);
    hb_blob_destroy (blob);
    إذا (new_blob_ڊيگهائي != blob_ڊيگهائي)
    {
      لكل (بنانشان glyph_id = 0; glyph_id < glyph_ڳڻپ; ++glyph_id)
      {
	hb_blob_t *blob = hb_ot_color_glyph_reference_png (font, glyph_id);

	إذا (hb_blob_get_ڊيگهائي (blob) == 0) استمر;

	بنانشان ڊيگهائي;
	ثابت حرف *معلومات = hb_blob_get_معلومات (blob, &ڊيگهائي);

	حرف output_رستو[255];
	snprintf (output_رستو, حجم output_رستو, "out/png-%u-%u-%u.png", glyph_id, strike, face_اشاريو);

	FILE *f = fopen (output_رستو, "wb");
	fwrite (معلومات, 1, ڊيگهائي, f);
	fclose (f);

	hb_blob_destroy (blob);
      }

      strike++;
      blob_ڊيگهائي = new_blob_ڊيگهائي;
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
سٽ_منزل (hb_draw_funcs_t *, draw_معلومات_t *draw_معلومات,
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
close_رستو (hb_draw_funcs_t *, draw_معلومات_t *draw_معلومات,
	    hb_draw_state_t *,
	    فراغ *)
{
  fprintf (draw_معلومات->f, "Z");
}

ساكن فراغ
layered_glyph_dump (hb_font_t *font, hb_draw_funcs_t *funcs, بنانشان face_اشاريو)
{
  hb_face_t *face = hb_font_get_face (font);
  بنانشان palette_ڳڻپ = hb_ot_color_palette_get_ڳڻپ (face);
  لكل (بنانشان palette = 0; palette < palette_ڳڻپ; ++palette)
  {
    بنانشان انگ_colors = hb_ot_color_palette_get_colors (face, palette, 0, خالي_اشارو, خالي_اشارو);
    إذا (!انگ_colors) استمر;

    hb_color_t *colors = (hb_color_t*) صفر_يادگيري_ڏيو (انگ_colors, حجم (hb_color_t));
    hb_ot_color_palette_get_colors (face, palette, 0, &انگ_colors, colors);
    إذا (!انگ_colors)
    {
      حرر (colors);
      استمر;
    }

    بنانشان انگ_glyphs = hb_face_get_glyph_ڳڻپ (face);
    لكل (hb_codepoint_t gid = 0; gid < انگ_glyphs; ++gid)
    {
      بنانشان انگ_layers = hb_ot_color_glyph_get_layers (face, gid, 0, خالي_اشارو, خالي_اشارو);
      إذا (!انگ_layers) استمر;

      hb_ot_color_layer_t *layers = (hb_ot_color_layer_t*) يادگيري_ڏيو (انگ_layers * حجم (hb_ot_color_layer_t));

      hb_ot_color_glyph_get_layers (face, gid, 0, &انگ_layers, layers);
      إذا (انگ_layers)
      {
	hb_font_extents_t font_extents;
	hb_font_get_extents_for_direction (font, HB_DIRECTION_LTR, &font_extents);
	hb_glyph_extents_t extents = {0, 0, 0, 0};
	إذا (!hb_font_get_glyph_extents (font, gid, &extents))
	{
	  لکو ("Skip gid: %u\n", gid);
	  استمر;
	}

	حرف output_رستو[255];
	snprintf (output_رستو, حجم output_رستو, "out/colr-%u-%u-%u.svg", gid, palette, face_اشاريو);
	FILE *f = fopen (output_رستو, "wb");
	fprintf (f, "<svg xmlns=\"http://www.w3.org/2000/svg\""
		    " viewBox=\"%d %d %lld %lld\">\n",
		    extents.x_bearing, 0,
		    (ڊگهو ڊگهو) extents.x_bearing + extents.ويڪر,
		    -(ڊگهو ڊگهو) extents.اوچائي);
	draw_معلومات_t draw_معلومات;
	draw_معلومات.ascender = extents.y_bearing;
	draw_معلومات.f = f;

	لكل (بنانشان layer = 0; layer < انگ_layers; ++layer)
	{
	  hb_color_t color = 0x000000FF;
	  إذا (layers[layer].color_اشاريو != 0xFFFF)
	    color = colors[layers[layer].color_اشاريو];
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
dump_glyphs (hb_font_t *font, hb_draw_funcs_t *funcs, بنانشان face_اشاريو)
{
  بنانشان انگ_glyphs = hb_face_get_glyph_ڳڻپ (hb_font_get_face (font));
  لكل (بنانشان gid = 0; gid < انگ_glyphs; ++gid)
  {
    hb_font_extents_t font_extents;
    hb_font_get_extents_for_direction (font, HB_DIRECTION_LTR, &font_extents);
    hb_glyph_extents_t extents = {0, 0, 0, 0};
    إذا (!hb_font_get_glyph_extents (font, gid, &extents))
    {
      لکو ("Skip gid: %u\n", gid);
      استمر;
    }

    حرف output_رستو[255];
    snprintf (output_رستو, حجم output_رستو, "out/%u-%u.svg", face_اشاريو, gid);
    FILE *f = fopen (output_رستو, "wb");
    fprintf (f, "<svg xmlns=\"http://www.w3.org/2000/svg\""
		" viewBox=\"%d %d %lld %lld\"><path d=\"",
		extents.x_bearing, 0,
		(ڊگهو ڊگهو) extents.x_bearing + extents.ويڪر,
		(ڊگهو ڊگهو) font_extents.ascender - font_extents.descender);
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
  fwrite (font_name, 1, لڙي_ڊيگهه (font_name), font_name_file);
  fclose (font_name_file);

  hb_draw_funcs_t *funcs = hb_draw_funcs_create ();
  hb_draw_funcs_سيٽ_move_منزل_func (funcs, (hb_draw_move_منزل_func_t) move_منزل, خالي_اشارو, خالي_اشارو);
  hb_draw_funcs_سيٽ_سٽ_منزل_func (funcs, (hb_draw_سٽ_منزل_func_t) سٽ_منزل, خالي_اشارو, خالي_اشارو);
  hb_draw_funcs_سيٽ_quadratic_منزل_func (funcs, (hb_draw_quadratic_منزل_func_t) quadratic_منزل, خالي_اشارو, خالي_اشارو);
  hb_draw_funcs_سيٽ_cubic_منزل_func (funcs, (hb_draw_cubic_منزل_func_t) cubic_منزل, خالي_اشارو, خالي_اشارو);
  hb_draw_funcs_سيٽ_close_رستو_func (funcs, (hb_draw_close_رستو_func_t) close_رستو, خالي_اشارو, خالي_اشارو);

  بنانشان انگ_faces = hb_face_ڳڻپ (blob);
  لكل (بنانشان face_اشاريو = 0; face_اشاريو < انگ_faces; ++face_اشاريو)
  {
    hb_face_t *face = hb_face_create (blob, face_اشاريو);
    hb_font_t *font = hb_font_create (face);

    إذا (hb_ot_color_has_png (face))
      لکو ("Dumping png (CBDT/sbix)...\n");
    png_dump (face, face_اشاريو);

#جيڪڏهن_وصف_نه_ٿيل HB_NO_SVG
    إذا (hb_ot_color_has_svg (face))
      لکو ("Dumping svg (SVG )...\n");
    svg_dump (face, face_اشاريو);
#جيڪڏهن_پورو

    إذا (hb_ot_color_has_layers (face) && hb_ot_color_has_palettes (face))
      لکو ("Dumping layered color glyphs (COLR/CPAL)...\n");
    layered_glyph_dump (font, funcs, face_اشاريو);

    dump_glyphs (font, funcs, face_اشاريو);

    hb_font_destroy (font);
    hb_face_destroy (face);
  }

  hb_draw_funcs_destroy (funcs);
}
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل MAIN_CC_NO_ذاتي_API
/* Only this part of this mini app uses private API */
#ضمن "hb-static.cc"
#ضمن "hb-open-file.hh"
#ضمن "hb-ot-layout-gdef-table.hh"
#ضمن "hb-ot-layout-gsubgpos.hh"

استعمال نطاق OT;

ساكن فراغ
print_layout_info_using_ذاتي_api (hb_blob_t *blob)
{
  ثابت حرف *font_معلومات = hb_blob_get_معلومات (blob, خالي_اشارو);
  hb_blob_t *font_blob = hb_sanitize_context_t ().sanitize_blob<OpenTypeFontFile> (blob);
  ثابت OpenTypeFontFile* sanitized = font_blob->as<OpenTypeFontFile> ();
  إذا (!font_blob->معلومات)
  {
    لکو ("Sanitization of the file wasn't successful. Exit");
    ٻاهر_نڪرو (1);
  }
  ثابت OpenTypeFontFile& ot = *sanitized;

  بدل (ot.get_ٽيگ ())
  {
  حالة OpenTypeFontFile::True_Type_ٽيگ:
    لکو ("OpenType font with TrueType outlines\n");
    اكسر;
  حالة OpenTypeFontFile::CFFTag:
    لکو ("OpenType font with CFF (Type1) outlines\n");
    اكسر;
  حالة OpenTypeFontFile::TTCTag:
    لکو ("TrueType Collection of OpenType fonts\n");
    اكسر;
  حالة OpenTypeFontFile::True_ٽيگ:
    لکو ("Obsolete Apple TrueType font\n");
    اكسر;
  حالة OpenTypeFontFile::Typ1_ٽيگ:
    لکو ("Obsolete Apple Type1 font in SFNT container\n");
    اكسر;
  حالة OpenTypeFontFile::DFont_ٽيگ:
    لکو ("DFont Mac Resource Fork\n");
    اكسر;
  افتراضي:
    لکو ("Unknown font format\n");
    اكسر;
  }

  بنانشان انگ_faces = hb_face_ڳڻپ (blob);
  لکو ("%u font(s) found in file\n", انگ_faces);
  لكل (بنانشان n_font = 0; n_font < انگ_faces; ++n_font)
  {
    ثابت OpenTypeFontFace &font = ot.get_face (n_font);
    لکو ("Font %u of %u:\n", n_font, انگ_faces);

    بنانشان انگ_tables = font.get_جدول_ڳڻپ ();
    لکو ("  %u table(s) found in font\n", انگ_tables);
    لكل (بنانشان n_جدول = 0; n_جدول < انگ_tables; ++n_جدول)
    {
      ثابت Open_Type_جدول &جدول = font.get_جدول (n_جدول);
      لکو ("  Table %2u of %2u: %.4s (0x%08x+0x%08x)\n", n_جدول, انگ_tables,
	      (ثابت حرف *) جدول.ٽيگ_2,
	      (بنانشان) جدول.وٿي,
	      (بنانشان) جدول.ڊيگهائي);

      بدل (جدول.ٽيگ_2)
      {

      حالة HB_OT_ٽيگ_GSUB:
      حالة HB_OT_ٽيگ_GPOS:
	{

	ثابت GSUBGPOS &g = *reinterpret_cast<ثابت GSUBGPOS *> (font_معلومات + جدول.وٿي);

	بنانشان انگ_scripts = g.get_script_ڳڻپ ();
	لکو ("    %u script(s) found in table\n", انگ_scripts);
	لكل (بنانشان n_script = 0; n_script < انگ_scripts; ++n_script)
	{
	  ثابت Script &script = g.get_script (n_script);
	  لکو ("    Script %2u of %2u: %.4s\n", n_script, انگ_scripts,
		  (ثابت حرف *) g.get_script_ٽيگ (n_script));

	  إذا (!script.has_default_lang_sys ())
	    لکو ("      No default language system\n");
	  صحيح انگ_langsys = script.get_lang_sys_ڳڻپ ();
	  لکو ("      %d language system(s) found in script\n", انگ_langsys);
	  لكل (صحيح n_langsys = script.has_default_lang_sys () ? -1 : 0; n_langsys < انگ_langsys; ++n_langsys)
	  {
	    ثابت LangSys &langsys = n_langsys == -1
				   ? script.get_default_lang_sys ()
				   : script.get_lang_sys (n_langsys);
	    إذا (n_langsys == -1)
	      لکو ("      Default Language System\n");
	    وإلا
	      لکو ("      Language System %2d of %2d: %.4s\n", n_langsys, انگ_langsys,
		      (ثابت حرف *) script.get_lang_sys_ٽيگ (n_langsys));
	    إذا (!langsys.has_required_feature ())
	      لکو ("        No required feature\n");
	    وإلا
	      لکو ("        Required feature index: %u\n",
		      langsys.get_required_feature_اشاريو ());

	    بنانشان انگ_features = langsys.get_feature_ڳڻپ ();
	    لکو ("        %u feature(s) found in language system\n", انگ_features);
	    لكل (بنانشان n_feature = 0; n_feature < انگ_features; ++n_feature)
	    {
	      لکو ("        Feature index %2u of %2u: %u\n", n_feature, انگ_features,
		      langsys.get_feature_اشاريو (n_feature));
	    }
	  }
	}

	بنانشان انگ_features = g.get_feature_ڳڻپ ();
	لکو ("    %u feature(s) found in table\n", انگ_features);
	لكل (بنانشان n_feature = 0; n_feature < انگ_features; ++n_feature)
	{
	  ثابت Feature &feature = g.get_feature (n_feature);
	  بنانشان انگ_lookups = feature.get_lookup_ڳڻپ ();
	  لکو ("    Feature %2u of %2u: %c%c%c%c\n", n_feature, انگ_features,
		  HB_UNTAG (g.get_feature_ٽيگ (n_feature)));

	  لکو ("        %u lookup(s) found in feature\n", انگ_lookups);
	  لكل (بنانشان n_lookup = 0; n_lookup < انگ_lookups; ++n_lookup) {
	    لکو ("        Lookup index %2u of %2u: %u\n", n_lookup, انگ_lookups,
		    feature.get_lookup_اشاريو (n_lookup));
	  }
	}

	بنانشان انگ_lookups = g.get_lookup_ڳڻپ ();
	لکو ("    %u lookup(s) found in table\n", انگ_lookups);
	لكل (بنانشان n_lookup = 0; n_lookup < انگ_lookups; ++n_lookup)
	{
	  ثابت Lookup &lookup = g.get_lookup (n_lookup);
	  لکو ("    Lookup %2u of %2u: type %u, props 0x%04X\n", n_lookup, انگ_lookups,
		  lookup.get_type (), lookup.get_props ());
	}

	}
	اكسر;

      حالة GDEF::جدول_ٽيگ:
	{

	ثابت GDEF &gdef = *reinterpret_cast<ثابت GDEF *> (font_معلومات + جدول.وٿي);

	لکو ("    Has %sglyph classes\n",
		  gdef.has_glyph_classes () ? "" : "no ");
	لکو ("    Has %smark attachment types\n",
		  gdef.has_mark_attachment_types () ? "" : "no ");
	لکو ("    Has %sattach list\n",
		  gdef.has_attach_list () ? "" : "no ");
	لکو ("    Has %slig carets\n",
		  gdef.has_lig_carets () ? "" : "no ");
	لکو ("    Has %smark glyph sets\n",
		  gdef.has_mark_glyph_sets () ? "" : "no ");
	اكسر;
	}
      }
    }
  }
}
/* end of private API use */
#جيڪڏهن_پورو

صحيح
مکيه (صحيح argc, حرف **argv)
{
  إذا (argc != 2)
  {
    fprintf (stderr, "usage: %s font-file.ttf\n\n"
		     "This tools is unsupported and crashes on bad data.\nDon't use it.\n", argv[0]);
    ٻاهر_نڪرو (1);
  }

  hb_blob_t *blob = hb_blob_create_from_file_or_fail (argv[1]);
  أكد (blob);
  لکو ("Opened font file %s: %u bytes long\n", argv[1], hb_blob_get_ڊيگهائي (blob));
#جيڪڏهن_وصف_نه_ٿيل MAIN_CC_NO_ذاتي_API
  print_layout_info_using_ذاتي_api (blob);
#جيڪڏهن_پورو
#إذا !defined(HB_NO_COLOR) && !defined(HB_NO_DRAW)
  dump_glyphs (blob, argv[1]);
#جيڪڏهن_پورو
  hb_blob_destroy (blob);

  أرجع 0;
}
