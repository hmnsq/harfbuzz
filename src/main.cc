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

#ifdef HAVE_CONFIG_H
#شامل "config.h"
#endif

#شامل "hb.h"
#شامل "hb-ot.h"

#شامل <cassert>
#شامل <cstdlib>
#شامل <cstdio>
#شامل <cstring>

#ifdef HB_NO_OPEN
#define hb_blob_create_from_file_or_fail(x)  hb_blob_get_empty ()
#endif

#اگر !defined(HB_NO_COLOR) && !defined(HB_NO_DRAW)
#ifndef HB_NO_SVG
ایستا تهی
svg_dump (hb_face_t *face, unsigned face_index)
{
  unsigned glyph_شمار = hb_face_get_glyph_شمار (face);

  برای (unsigned glyph_id = 0; glyph_id < glyph_شمار; ++glyph_id)
  {
    hb_blob_t *blob = hb_ot_color_glyph_reference_svg (face, glyph_id);

    اگر (hb_blob_get_length (blob) == 0) ادامه;

    unsigned length;
    ثابت نویسه *data = hb_blob_get_data (blob, &length);

    نویسه output_path[255];
    snprintf (output_path, اندازه output_path,
	     "out/svg-%u-%u.svg%s",
	     glyph_id,
	     face_index,
	     // append "z" if the content is gzipped, https://stackoverflow.com/a/6059405
	     (length > 2 && (data[0] == '\x1F') && (data[1] == '\x8B')) ? "z" : "");

    FILE *f = fopen (output_path, "wb");
    fwrite (data, 1, length, f);
    fclose (f);

    hb_blob_destroy (blob);
  }
}
#endif

/* _png API is so easy to use unlike the below code, don't get confused */
ایستا تهی
png_dump (hb_face_t *face, unsigned face_index)
{
  unsigned glyph_شمار = hb_face_get_glyph_شمار (face);
  hb_font_t *font = hb_font_create (face);

  /* scans the font for strikes */
  unsigned sample_glyph_id;
  /* we don't care about different strikes for different glyphs at this point */
  برای (sample_glyph_id = 0; sample_glyph_id < glyph_شمار; ++sample_glyph_id)
  {
    hb_blob_t *blob = hb_ot_color_glyph_reference_png (font, sample_glyph_id);
    unsigned blob_length = hb_blob_get_length (blob);
    hb_blob_destroy (blob);
    اگر (blob_length != 0)
      بشکن;
  }

  unsigned upem = hb_face_get_upem (face);
  unsigned blob_length = 0;
  unsigned strike = 0;
  برای (unsigned ppem = 1; ppem < upem; ++ppem)
  {
    hb_font_set_ppem (font, ppem, ppem);
    hb_blob_t *blob = hb_ot_color_glyph_reference_png (font, sample_glyph_id);
    unsigned new_blob_length = hb_blob_get_length (blob);
    hb_blob_destroy (blob);
    اگر (new_blob_length != blob_length)
    {
      برای (unsigned glyph_id = 0; glyph_id < glyph_شمار; ++glyph_id)
      {
	hb_blob_t *blob = hb_ot_color_glyph_reference_png (font, glyph_id);

	اگر (hb_blob_get_length (blob) == 0) ادامه;

	unsigned length;
	ثابت نویسه *data = hb_blob_get_data (blob, &length);

	نویسه output_path[255];
	snprintf (output_path, اندازه output_path, "out/png-%u-%u-%u.png", glyph_id, strike, face_index);

	FILE *f = fopen (output_path, "wb");
	fwrite (data, 1, length, f);
	fclose (f);

	hb_blob_destroy (blob);
      }

      strike++;
      blob_length = new_blob_length;
    }
  }

  hb_font_destroy (font);
}

ساختار draw_data_t
{
  FILE *f;
  hb_position_t ascender;
};

ایستا تهی
move_to (hb_draw_funcs_t *, draw_data_t *draw_data,
	 hb_draw_state_t *,
	 اعشاری to_x, اعشاری to_y,
	 تهی *)
{
  fprintf (draw_data->f, "M%g,%g", to_x, draw_data->ascender - to_y);
}

ایستا تهی
سطر_to (hb_draw_funcs_t *, draw_data_t *draw_data,
	 hb_draw_state_t *,
	 اعشاری to_x, اعشاری to_y,
	 تهی *)
{
  fprintf (draw_data->f, "L%g,%g", to_x, draw_data->ascender - to_y);
}

ایستا تهی
quadratic_to (hb_draw_funcs_t *, draw_data_t *draw_data,
	      hb_draw_state_t *,
	      اعشاری control_x, اعشاری control_y,
	      اعشاری to_x, اعشاری to_y,
	      تهی *)
{
  fprintf (draw_data->f, "Q%g,%g %g,%g", control_x, draw_data->ascender - control_y,
					to_x, draw_data->ascender - to_y);
}

ایستا تهی
cubic_to (hb_draw_funcs_t *, draw_data_t *draw_data,
	  hb_draw_state_t *,
	  اعشاری control1_x, اعشاری control1_y,
	  اعشاری control2_x, اعشاری control2_y,
	  اعشاری to_x, اعشاری to_y,
	  تهی *)
{
  fprintf (draw_data->f, "C%g,%g %g,%g %g,%g", control1_x, draw_data->ascender - control1_y,
					       control2_x, draw_data->ascender - control2_y,
					       to_x, draw_data->ascender - to_y);
}

ایستا تهی
close_path (hb_draw_funcs_t *, draw_data_t *draw_data,
	    hb_draw_state_t *,
	    تهی *)
{
  fprintf (draw_data->f, "Z");
}

ایستا تهی
layered_glyph_dump (hb_font_t *font, hb_draw_funcs_t *funcs, unsigned face_index)
{
  hb_face_t *face = hb_font_get_face (font);
  unsigned palette_شمار = hb_ot_color_palette_get_شمار (face);
  برای (unsigned palette = 0; palette < palette_شمار; ++palette)
  {
    unsigned رقم_colors = hb_ot_color_palette_get_colors (face, palette, 0, nullptr, nullptr);
    اگر (!رقم_colors) ادامه;

    hb_color_t *colors = (hb_color_t*) calloc (رقم_colors, اندازه (hb_color_t));
    hb_ot_color_palette_get_colors (face, palette, 0, &رقم_colors, colors);
    اگر (!رقم_colors)
    {
      آزاد (colors);
      ادامه;
    }

    unsigned رقم_glyphs = hb_face_get_glyph_شمار (face);
    برای (hb_codepoint_t gid = 0; gid < رقم_glyphs; ++gid)
    {
      unsigned رقم_layers = hb_ot_color_glyph_get_layers (face, gid, 0, nullptr, nullptr);
      اگر (!رقم_layers) ادامه;

      hb_ot_color_layer_t *layers = (hb_ot_color_layer_t*) malloc (رقم_layers * اندازه (hb_ot_color_layer_t));

      hb_ot_color_glyph_get_layers (face, gid, 0, &رقم_layers, layers);
      اگر (رقم_layers)
      {
	hb_font_extents_t font_extents;
	hb_font_get_extents_for_direction (font, HB_DIRECTION_LTR, &font_extents);
	hb_glyph_extents_t extents = {0, 0, 0, 0};
	اگر (!hb_font_get_glyph_extents (font, gid, &extents))
	{
	  printf ("Skip gid: %u\n", gid);
	  ادامه;
	}

	نویسه output_path[255];
	snprintf (output_path, اندازه output_path, "out/colr-%u-%u-%u.svg", gid, palette, face_index);
	FILE *f = fopen (output_path, "wb");
	fprintf (f, "<svg xmlns=\"http://www.w3.org/2000/svg\""
		    " viewBox=\"%d %d %lld %lld\">\n",
		    extents.x_bearing, 0,
		    (long long) extents.x_bearing + extents.width,
		    -(long long) extents.height);
	draw_data_t draw_data;
	draw_data.ascender = extents.y_bearing;
	draw_data.f = f;

	برای (unsigned layer = 0; layer < رقم_layers; ++layer)
	{
	  hb_color_t color = 0x000000FF;
	  اگر (layers[layer].color_index != 0xFFFF)
	    color = colors[layers[layer].color_index];
	  fprintf (f, "<path fill=\"#%02X%02X%02X\" ",
		   hb_color_get_red (color), hb_color_get_green (color), hb_color_get_green (color));
	  اگر (hb_color_get_alpha (color) != 255)
	    fprintf (f, "fill-opacity=\"%.3f\"", (دوگانه) hb_color_get_alpha (color) / 255.);
	  fprintf (f, "d=\"");
	  hb_font_draw_glyph (font, layers[layer].glyph, funcs, &draw_data);
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

ایستا تهی
dump_glyphs (hb_font_t *font, hb_draw_funcs_t *funcs, unsigned face_index)
{
  unsigned رقم_glyphs = hb_face_get_glyph_شمار (hb_font_get_face (font));
  برای (unsigned gid = 0; gid < رقم_glyphs; ++gid)
  {
    hb_font_extents_t font_extents;
    hb_font_get_extents_for_direction (font, HB_DIRECTION_LTR, &font_extents);
    hb_glyph_extents_t extents = {0, 0, 0, 0};
    اگر (!hb_font_get_glyph_extents (font, gid, &extents))
    {
      printf ("Skip gid: %u\n", gid);
      ادامه;
    }

    نویسه output_path[255];
    snprintf (output_path, اندازه output_path, "out/%u-%u.svg", face_index, gid);
    FILE *f = fopen (output_path, "wb");
    fprintf (f, "<svg xmlns=\"http://www.w3.org/2000/svg\""
		" viewBox=\"%d %d %lld %lld\"><path d=\"",
		extents.x_bearing, 0,
		(long long) extents.x_bearing + extents.width,
		(long long) font_extents.ascender - font_extents.descender);
    draw_data_t draw_data;
    draw_data.ascender = font_extents.ascender;
    draw_data.f = f;
    hb_font_draw_glyph (font, gid, funcs, &draw_data);
    fprintf (f, "\"/></svg>");
    fclose (f);
  }
}

ایستا تهی
dump_glyphs (hb_blob_t *blob, ثابت نویسه *font_name)
{
  FILE *font_name_file = fopen ("out/.dumped_font_name", "r");
  اگر (font_name_file)
  {
    fprintf (stderr, "Purge or rename ./out folder if you like to run a glyph dump,\n"
		     "run it like `rm -rf out && mkdir out && src/main font-file.ttf`\n");
    بازگشت;
  }

  font_name_file = fopen ("out/.dumped_font_name", "w");
  اگر (!font_name_file)
  {
    fprintf (stderr, "./out is not accessible as a folder, create it please\n");
    بازگشت;
  }
  fwrite (font_name, 1, strlen (font_name), font_name_file);
  fclose (font_name_file);

  hb_draw_funcs_t *funcs = hb_draw_funcs_create ();
  hb_draw_funcs_set_move_to_func (funcs, (hb_draw_move_to_func_t) move_to, nullptr, nullptr);
  hb_draw_funcs_set_سطر_to_func (funcs, (hb_draw_سطر_to_func_t) سطر_to, nullptr, nullptr);
  hb_draw_funcs_set_quadratic_to_func (funcs, (hb_draw_quadratic_to_func_t) quadratic_to, nullptr, nullptr);
  hb_draw_funcs_set_cubic_to_func (funcs, (hb_draw_cubic_to_func_t) cubic_to, nullptr, nullptr);
  hb_draw_funcs_set_close_path_func (funcs, (hb_draw_close_path_func_t) close_path, nullptr, nullptr);

  unsigned رقم_faces = hb_face_شمار (blob);
  برای (unsigned face_index = 0; face_index < رقم_faces; ++face_index)
  {
    hb_face_t *face = hb_face_create (blob, face_index);
    hb_font_t *font = hb_font_create (face);

    اگر (hb_ot_color_has_png (face))
      printf ("Dumping png (CBDT/sbix)...\n");
    png_dump (face, face_index);

#ifndef HB_NO_SVG
    اگر (hb_ot_color_has_svg (face))
      printf ("Dumping svg (SVG )...\n");
    svg_dump (face, face_index);
#endif

    اگر (hb_ot_color_has_layers (face) && hb_ot_color_has_palettes (face))
      printf ("Dumping layered color glyphs (COLR/CPAL)...\n");
    layered_glyph_dump (font, funcs, face_index);

    dump_glyphs (font, funcs, face_index);

    hb_font_destroy (font);
    hb_face_destroy (face);
  }

  hb_draw_funcs_destroy (funcs);
}
#endif

#ifndef MAIN_CC_NO_PRIVATE_API
/* Only this part of this mini app uses private API */
#شامل "hb-static.cc"
#شامل "hb-open-file.hh"
#شامل "hb-ot-layout-gdef-table.hh"
#شامل "hb-ot-layout-gsubgpos.hh"

using فضاینام OT;

ایستا تهی
print_layout_info_using_private_api (hb_blob_t *blob)
{
  ثابت نویسه *font_data = hb_blob_get_data (blob, nullptr);
  hb_blob_t *font_blob = hb_sanitize_context_t ().sanitize_blob<OpenTypeFontFile> (blob);
  ثابت OpenTypeFontFile* sanitized = font_blob->as<OpenTypeFontFile> ();
  اگر (!font_blob->data)
  {
    printf ("Sanitization of the file wasn't successful. Exit");
    exit (1);
  }
  ثابت OpenTypeFontFile& ot = *sanitized;

  انتخاب (ot.get_tag ())
  {
  حالت OpenTypeFontFile::TrueTypeTag:
    printf ("OpenType font with TrueType outlines\n");
    بشکن;
  حالت OpenTypeFontFile::CFFTag:
    printf ("OpenType font with CFF (Type1) outlines\n");
    بشکن;
  حالت OpenTypeFontFile::TTCTag:
    printf ("TrueType Collection of OpenType fonts\n");
    بشکن;
  حالت OpenTypeFontFile::TrueTag:
    printf ("Obsolete Apple TrueType font\n");
    بشکن;
  حالت OpenTypeFontFile::Typ1Tag:
    printf ("Obsolete Apple Type1 font in SFNT container\n");
    بشکن;
  حالت OpenTypeFontFile::DFontTag:
    printf ("DFont Mac Resource Fork\n");
    بشکن;
  پیشفرض:
    printf ("Unknown font format\n");
    بشکن;
  }

  unsigned رقم_faces = hb_face_شمار (blob);
  printf ("%u font(s) found in file\n", رقم_faces);
  برای (unsigned n_font = 0; n_font < رقم_faces; ++n_font)
  {
    ثابت OpenTypeFontFace &font = ot.get_face (n_font);
    printf ("Font %u of %u:\n", n_font, رقم_faces);

    unsigned رقم_tables = font.get_table_شمار ();
    printf ("  %u table(s) found in font\n", رقم_tables);
    برای (unsigned n_table = 0; n_table < رقم_tables; ++n_table)
    {
      ثابت OpenTypeTable &table = font.get_table (n_table);
      printf ("  Table %2u of %2u: %.4s (0x%08x+0x%08x)\n", n_table, رقم_tables,
	      (ثابت نویسه *) table.tag,
	      (unsigned) table.offset,
	      (unsigned) table.length);

      انتخاب (table.tag)
      {

      حالت HB_OT_TAG_GSUB:
      حالت HB_OT_TAG_GPOS:
	{

	ثابت GSUBGPOS &g = *reinterpret_cast<ثابت GSUBGPOS *> (font_data + table.offset);

	unsigned رقم_scripts = g.get_script_شمار ();
	printf ("    %u script(s) found in table\n", رقم_scripts);
	برای (unsigned n_script = 0; n_script < رقم_scripts; ++n_script)
	{
	  ثابت Script &script = g.get_script (n_script);
	  printf ("    Script %2u of %2u: %.4s\n", n_script, رقم_scripts,
		  (ثابت نویسه *) g.get_script_tag (n_script));

	  اگر (!script.has_default_lang_sys ())
	    printf ("      No default language system\n");
	  صحیح رقم_langsys = script.get_lang_sys_شمار ();
	  printf ("      %d language system(s) found in script\n", رقم_langsys);
	  برای (صحیح n_langsys = script.has_default_lang_sys () ? -1 : 0; n_langsys < رقم_langsys; ++n_langsys)
	  {
	    ثابت LangSys &langsys = n_langsys == -1
				   ? script.get_default_lang_sys ()
				   : script.get_lang_sys (n_langsys);
	    اگر (n_langsys == -1)
	      printf ("      Default Language System\n");
	    وگرنه
	      printf ("      Language System %2d of %2d: %.4s\n", n_langsys, رقم_langsys,
		      (ثابت نویسه *) script.get_lang_sys_tag (n_langsys));
	    اگر (!langsys.has_required_feature ())
	      printf ("        No required feature\n");
	    وگرنه
	      printf ("        Required feature index: %u\n",
		      langsys.get_required_feature_index ());

	    unsigned رقم_features = langsys.get_feature_شمار ();
	    printf ("        %u feature(s) found in language system\n", رقم_features);
	    برای (unsigned n_feature = 0; n_feature < رقم_features; ++n_feature)
	    {
	      printf ("        Feature index %2u of %2u: %u\n", n_feature, رقم_features,
		      langsys.get_feature_index (n_feature));
	    }
	  }
	}

	unsigned رقم_features = g.get_feature_شمار ();
	printf ("    %u feature(s) found in table\n", رقم_features);
	برای (unsigned n_feature = 0; n_feature < رقم_features; ++n_feature)
	{
	  ثابت Feature &feature = g.get_feature (n_feature);
	  unsigned رقم_lookups = feature.get_lookup_شمار ();
	  printf ("    Feature %2u of %2u: %c%c%c%c\n", n_feature, رقم_features,
		  HB_UNTAG (g.get_feature_tag (n_feature)));

	  printf ("        %u lookup(s) found in feature\n", رقم_lookups);
	  برای (unsigned n_lookup = 0; n_lookup < رقم_lookups; ++n_lookup) {
	    printf ("        Lookup index %2u of %2u: %u\n", n_lookup, رقم_lookups,
		    feature.get_lookup_index (n_lookup));
	  }
	}

	unsigned رقم_lookups = g.get_lookup_شمار ();
	printf ("    %u lookup(s) found in table\n", رقم_lookups);
	برای (unsigned n_lookup = 0; n_lookup < رقم_lookups; ++n_lookup)
	{
	  ثابت Lookup &lookup = g.get_lookup (n_lookup);
	  printf ("    Lookup %2u of %2u: type %u, props 0x%04X\n", n_lookup, رقم_lookups,
		  lookup.get_type (), lookup.get_props ());
	}

	}
	بشکن;

      حالت GDEF::tableTag:
	{

	ثابت GDEF &gdef = *reinterpret_cast<ثابت GDEF *> (font_data + table.offset);

	printf ("    Has %sglyph classes\n",
		  gdef.has_glyph_classes () ? "" : "no ");
	printf ("    Has %smark attachment types\n",
		  gdef.has_mark_attachment_types () ? "" : "no ");
	printf ("    Has %sattach list\n",
		  gdef.has_attach_list () ? "" : "no ");
	printf ("    Has %slig carets\n",
		  gdef.has_lig_carets () ? "" : "no ");
	printf ("    Has %smark glyph sets\n",
		  gdef.has_mark_glyph_sets () ? "" : "no ");
	بشکن;
	}
      }
    }
  }
}
/* end of private API use */
#endif

صحیح
اصلی (صحیح argc, نویسه **argv)
{
  اگر (argc != 2)
  {
    fprintf (stderr, "usage: %s font-file.ttf\n\n"
		     "This tools is unsupported and crashes on bad data.\nDon't use it.\n", argv[0]);
    exit (1);
  }

  hb_blob_t *blob = hb_blob_create_from_file_or_fail (argv[1]);
  أكد (blob);
  printf ("Opened font file %s: %u bytes long\n", argv[1], hb_blob_get_length (blob));
#ifndef MAIN_CC_NO_PRIVATE_API
  print_layout_info_using_private_api (blob);
#endif
#اگر !defined(HB_NO_COLOR) && !defined(HB_NO_DRAW)
  dump_glyphs (blob, argv[1]);
#endif
  hb_blob_destroy (blob);

  بازگشت 0;
}
