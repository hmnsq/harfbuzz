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
#ضمن "config.h"
#endif

#ضمن "hb.h"
#ضمن "hb-ot.h"

#ضمن <cassert>
#ضمن <cstdlib>
#ضمن <cstdio>
#ضمن <cstring>

#ifdef HB_NO_OPEN
#define hb_blob_create_from_file_or_fail(x)  hb_blob_get_empty ()
#endif

#إذا !defined(HB_NO_COLOR) && !defined(HB_NO_DRAW)
#ifndef HB_NO_SVG
ساكن فراغ
svg_dump (hb_face_t *face, unsigned face_index)
{
  unsigned glyph_کيراءن = hb_face_get_glyph_کيراءن (face);

  لكل (unsigned glyph_id = 0; glyph_id < glyph_کيراءن; ++glyph_id)
  {
    hb_blob_t *blob = hb_ot_color_glyph_reference_svg (face, glyph_id);

    إذا (hb_blob_get_length (blob) == 0) استمر;

    unsigned length;
    ثابت حرف *data = hb_blob_get_data (blob, &length);

    حرف output_path[255];
    snprintf (output_path, حجم output_path,
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
ساكن فراغ
png_dump (hb_face_t *face, unsigned face_index)
{
  unsigned glyph_کيراءن = hb_face_get_glyph_کيراءن (face);
  hb_font_t *font = hb_font_create (face);

  /* scans the font for strikes */
  unsigned sample_glyph_id;
  /* we don't care about different strikes for different glyphs at this point */
  لكل (sample_glyph_id = 0; sample_glyph_id < glyph_کيراءن; ++sample_glyph_id)
  {
    hb_blob_t *blob = hb_ot_color_glyph_reference_png (font, sample_glyph_id);
    unsigned blob_length = hb_blob_get_length (blob);
    hb_blob_destroy (blob);
    إذا (blob_length != 0)
      اكسر;
  }

  unsigned upem = hb_face_get_upem (face);
  unsigned blob_length = 0;
  unsigned strike = 0;
  لكل (unsigned ppem = 1; ppem < upem; ++ppem)
  {
    hb_font_set_ppem (font, ppem, ppem);
    hb_blob_t *blob = hb_ot_color_glyph_reference_png (font, sample_glyph_id);
    unsigned new_blob_length = hb_blob_get_length (blob);
    hb_blob_destroy (blob);
    إذا (new_blob_length != blob_length)
    {
      لكل (unsigned glyph_id = 0; glyph_id < glyph_کيراءن; ++glyph_id)
      {
	hb_blob_t *blob = hb_ot_color_glyph_reference_png (font, glyph_id);

	إذا (hb_blob_get_length (blob) == 0) استمر;

	unsigned length;
	ثابت حرف *data = hb_blob_get_data (blob, &length);

	حرف output_path[255];
	snprintf (output_path, حجم output_path, "out/png-%u-%u-%u.png", glyph_id, strike, face_index);

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

بنية draw_data_t
{
  FILE *f;
  hb_position_t ascender;
};

ساكن فراغ
move_to (hb_draw_funcs_t *, draw_data_t *draw_data,
	 hb_draw_state_t *,
	 عائم to_x, عائم to_y,
	 فراغ *)
{
  fprintf (draw_data->f, "M%g,%g", to_x, draw_data->ascender - to_y);
}

ساكن فراغ
باريس_to (hb_draw_funcs_t *, draw_data_t *draw_data,
	 hb_draw_state_t *,
	 عائم to_x, عائم to_y,
	 فراغ *)
{
  fprintf (draw_data->f, "L%g,%g", to_x, draw_data->ascender - to_y);
}

ساكن فراغ
quadratic_to (hb_draw_funcs_t *, draw_data_t *draw_data,
	      hb_draw_state_t *,
	      عائم control_x, عائم control_y,
	      عائم to_x, عائم to_y,
	      فراغ *)
{
  fprintf (draw_data->f, "Q%g,%g %g,%g", control_x, draw_data->ascender - control_y,
					to_x, draw_data->ascender - to_y);
}

ساكن فراغ
cubic_to (hb_draw_funcs_t *, draw_data_t *draw_data,
	  hb_draw_state_t *,
	  عائم control1_x, عائم control1_y,
	  عائم control2_x, عائم control2_y,
	  عائم to_x, عائم to_y,
	  فراغ *)
{
  fprintf (draw_data->f, "C%g,%g %g,%g %g,%g", control1_x, draw_data->ascender - control1_y,
					       control2_x, draw_data->ascender - control2_y,
					       to_x, draw_data->ascender - to_y);
}

ساكن فراغ
close_path (hb_draw_funcs_t *, draw_data_t *draw_data,
	    hb_draw_state_t *,
	    فراغ *)
{
  fprintf (draw_data->f, "Z");
}

ساكن فراغ
layered_glyph_dump (hb_font_t *font, hb_draw_funcs_t *funcs, unsigned face_index)
{
  hb_face_t *face = hb_font_get_face (font);
  unsigned palette_کيراءن = hb_ot_color_palette_get_کيراءن (face);
  لكل (unsigned palette = 0; palette < palette_کيراءن; ++palette)
  {
    unsigned اڠک_colors = hb_ot_color_palette_get_colors (face, palette, 0, nullptr, nullptr);
    إذا (!اڠک_colors) استمر;

    hb_color_t *colors = (hb_color_t*) calloc (اڠک_colors, حجم (hb_color_t));
    hb_ot_color_palette_get_colors (face, palette, 0, &اڠک_colors, colors);
    إذا (!اڠک_colors)
    {
      حرر (colors);
      استمر;
    }

    unsigned اڠک_glyphs = hb_face_get_glyph_کيراءن (face);
    لكل (hb_codepoint_t gid = 0; gid < اڠک_glyphs; ++gid)
    {
      unsigned اڠک_layers = hb_ot_color_glyph_get_layers (face, gid, 0, nullptr, nullptr);
      إذا (!اڠک_layers) استمر;

      hb_ot_color_layer_t *layers = (hb_ot_color_layer_t*) malloc (اڠک_layers * حجم (hb_ot_color_layer_t));

      hb_ot_color_glyph_get_layers (face, gid, 0, &اڠک_layers, layers);
      إذا (اڠک_layers)
      {
	hb_font_extents_t font_extents;
	hb_font_get_extents_for_direction (font, HB_DIRECTION_LTR, &font_extents);
	hb_glyph_extents_t extents = {0, 0, 0, 0};
	إذا (!hb_font_get_glyph_extents (font, gid, &extents))
	{
	  printf ("Skip gid: %u\n", gid);
	  استمر;
	}

	حرف output_path[255];
	snprintf (output_path, حجم output_path, "out/colr-%u-%u-%u.svg", gid, palette, face_index);
	FILE *f = fopen (output_path, "wb");
	fprintf (f, "<svg xmlns=\"http://www.w3.org/2000/svg\""
		    " viewBox=\"%d %d %lld %lld\">\n",
		    extents.x_bearing, 0,
		    (long long) extents.x_bearing + extents.width,
		    -(long long) extents.height);
	draw_data_t draw_data;
	draw_data.ascender = extents.y_bearing;
	draw_data.f = f;

	لكل (unsigned layer = 0; layer < اڠک_layers; ++layer)
	{
	  hb_color_t color = 0x000000FF;
	  إذا (layers[layer].color_index != 0xFFFF)
	    color = colors[layers[layer].color_index];
	  fprintf (f, "<path fill=\"#%02X%02X%02X\" ",
		   hb_color_get_red (color), hb_color_get_green (color), hb_color_get_green (color));
	  إذا (hb_color_get_alpha (color) != 255)
	    fprintf (f, "fill-opacity=\"%.3f\"", (مزدوج) hb_color_get_alpha (color) / 255.);
	  fprintf (f, "d=\"");
	  hb_font_draw_glyph (font, layers[layer].glyph, funcs, &draw_data);
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
dump_glyphs (hb_font_t *font, hb_draw_funcs_t *funcs, unsigned face_index)
{
  unsigned اڠک_glyphs = hb_face_get_glyph_کيراءن (hb_font_get_face (font));
  لكل (unsigned gid = 0; gid < اڠک_glyphs; ++gid)
  {
    hb_font_extents_t font_extents;
    hb_font_get_extents_for_direction (font, HB_DIRECTION_LTR, &font_extents);
    hb_glyph_extents_t extents = {0, 0, 0, 0};
    إذا (!hb_font_get_glyph_extents (font, gid, &extents))
    {
      printf ("Skip gid: %u\n", gid);
      استمر;
    }

    حرف output_path[255];
    snprintf (output_path, حجم output_path, "out/%u-%u.svg", face_index, gid);
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
  fwrite (font_name, 1, strlen (font_name), font_name_file);
  fclose (font_name_file);

  hb_draw_funcs_t *funcs = hb_draw_funcs_create ();
  hb_draw_funcs_set_move_to_func (funcs, (hb_draw_move_to_func_t) move_to, nullptr, nullptr);
  hb_draw_funcs_set_باريس_to_func (funcs, (hb_draw_باريس_to_func_t) باريس_to, nullptr, nullptr);
  hb_draw_funcs_set_quadratic_to_func (funcs, (hb_draw_quadratic_to_func_t) quadratic_to, nullptr, nullptr);
  hb_draw_funcs_set_cubic_to_func (funcs, (hb_draw_cubic_to_func_t) cubic_to, nullptr, nullptr);
  hb_draw_funcs_set_close_path_func (funcs, (hb_draw_close_path_func_t) close_path, nullptr, nullptr);

  unsigned اڠک_faces = hb_face_کيراءن (blob);
  لكل (unsigned face_index = 0; face_index < اڠک_faces; ++face_index)
  {
    hb_face_t *face = hb_face_create (blob, face_index);
    hb_font_t *font = hb_font_create (face);

    إذا (hb_ot_color_has_png (face))
      printf ("Dumping png (CBDT/sbix)...\n");
    png_dump (face, face_index);

#ifndef HB_NO_SVG
    إذا (hb_ot_color_has_svg (face))
      printf ("Dumping svg (SVG )...\n");
    svg_dump (face, face_index);
#endif

    إذا (hb_ot_color_has_layers (face) && hb_ot_color_has_palettes (face))
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
#ضمن "hb-static.cc"
#ضمن "hb-open-file.hh"
#ضمن "hb-ot-layout-gdef-table.hh"
#ضمن "hb-ot-layout-gsubgpos.hh"

using نطاق OT;

ساكن فراغ
print_layout_info_using_private_api (hb_blob_t *blob)
{
  ثابت حرف *font_data = hb_blob_get_data (blob, nullptr);
  hb_blob_t *font_blob = hb_sanitize_context_t ().sanitize_blob<OpenTypeFontFile> (blob);
  ثابت OpenTypeFontFile* sanitized = font_blob->as<OpenTypeFontFile> ();
  إذا (!font_blob->data)
  {
    printf ("Sanitization of the file wasn't successful. Exit");
    exit (1);
  }
  ثابت OpenTypeFontFile& ot = *sanitized;

  بدل (ot.get_tag ())
  {
  حالة OpenTypeFontFile::TrueTypeTag:
    printf ("OpenType font with TrueType outlines\n");
    اكسر;
  حالة OpenTypeFontFile::CFFTag:
    printf ("OpenType font with CFF (Type1) outlines\n");
    اكسر;
  حالة OpenTypeFontFile::TTCTag:
    printf ("TrueType Collection of OpenType fonts\n");
    اكسر;
  حالة OpenTypeFontFile::TrueTag:
    printf ("Obsolete Apple TrueType font\n");
    اكسر;
  حالة OpenTypeFontFile::Typ1Tag:
    printf ("Obsolete Apple Type1 font in SFNT container\n");
    اكسر;
  حالة OpenTypeFontFile::DFontTag:
    printf ("DFont Mac Resource Fork\n");
    اكسر;
  افتراضي:
    printf ("Unknown font format\n");
    اكسر;
  }

  unsigned اڠک_faces = hb_face_کيراءن (blob);
  printf ("%u font(s) found in file\n", اڠک_faces);
  لكل (unsigned n_font = 0; n_font < اڠک_faces; ++n_font)
  {
    ثابت OpenTypeFontFace &font = ot.get_face (n_font);
    printf ("Font %u of %u:\n", n_font, اڠک_faces);

    unsigned اڠک_tables = font.get_table_کيراءن ();
    printf ("  %u table(s) found in font\n", اڠک_tables);
    لكل (unsigned n_table = 0; n_table < اڠک_tables; ++n_table)
    {
      ثابت OpenTypeTable &table = font.get_table (n_table);
      printf ("  Table %2u of %2u: %.4s (0x%08x+0x%08x)\n", n_table, اڠک_tables,
	      (ثابت حرف *) table.tag,
	      (unsigned) table.offset,
	      (unsigned) table.length);

      بدل (table.tag)
      {

      حالة HB_OT_TAG_GSUB:
      حالة HB_OT_TAG_GPOS:
	{

	ثابت GSUBGPOS &g = *reinterpret_cast<ثابت GSUBGPOS *> (font_data + table.offset);

	unsigned اڠک_scripts = g.get_script_کيراءن ();
	printf ("    %u script(s) found in table\n", اڠک_scripts);
	لكل (unsigned n_script = 0; n_script < اڠک_scripts; ++n_script)
	{
	  ثابت Script &script = g.get_script (n_script);
	  printf ("    Script %2u of %2u: %.4s\n", n_script, اڠک_scripts,
		  (ثابت حرف *) g.get_script_tag (n_script));

	  إذا (!script.has_default_lang_sys ())
	    printf ("      No default language system\n");
	  صحيح اڠک_langsys = script.get_lang_sys_کيراءن ();
	  printf ("      %d language system(s) found in script\n", اڠک_langsys);
	  لكل (صحيح n_langsys = script.has_default_lang_sys () ? -1 : 0; n_langsys < اڠک_langsys; ++n_langsys)
	  {
	    ثابت LangSys &langsys = n_langsys == -1
				   ? script.get_default_lang_sys ()
				   : script.get_lang_sys (n_langsys);
	    إذا (n_langsys == -1)
	      printf ("      Default Language System\n");
	    وإلا
	      printf ("      Language System %2d of %2d: %.4s\n", n_langsys, اڠک_langsys,
		      (ثابت حرف *) script.get_lang_sys_tag (n_langsys));
	    إذا (!langsys.has_required_feature ())
	      printf ("        No required feature\n");
	    وإلا
	      printf ("        Required feature index: %u\n",
		      langsys.get_required_feature_index ());

	    unsigned اڠک_features = langsys.get_feature_کيراءن ();
	    printf ("        %u feature(s) found in language system\n", اڠک_features);
	    لكل (unsigned n_feature = 0; n_feature < اڠک_features; ++n_feature)
	    {
	      printf ("        Feature index %2u of %2u: %u\n", n_feature, اڠک_features,
		      langsys.get_feature_index (n_feature));
	    }
	  }
	}

	unsigned اڠک_features = g.get_feature_کيراءن ();
	printf ("    %u feature(s) found in table\n", اڠک_features);
	لكل (unsigned n_feature = 0; n_feature < اڠک_features; ++n_feature)
	{
	  ثابت Feature &feature = g.get_feature (n_feature);
	  unsigned اڠک_lookups = feature.get_lookup_کيراءن ();
	  printf ("    Feature %2u of %2u: %c%c%c%c\n", n_feature, اڠک_features,
		  HB_UNTAG (g.get_feature_tag (n_feature)));

	  printf ("        %u lookup(s) found in feature\n", اڠک_lookups);
	  لكل (unsigned n_lookup = 0; n_lookup < اڠک_lookups; ++n_lookup) {
	    printf ("        Lookup index %2u of %2u: %u\n", n_lookup, اڠک_lookups,
		    feature.get_lookup_index (n_lookup));
	  }
	}

	unsigned اڠک_lookups = g.get_lookup_کيراءن ();
	printf ("    %u lookup(s) found in table\n", اڠک_lookups);
	لكل (unsigned n_lookup = 0; n_lookup < اڠک_lookups; ++n_lookup)
	{
	  ثابت Lookup &lookup = g.get_lookup (n_lookup);
	  printf ("    Lookup %2u of %2u: type %u, props 0x%04X\n", n_lookup, اڠک_lookups,
		  lookup.get_type (), lookup.get_props ());
	}

	}
	اكسر;

      حالة GDEF::tableTag:
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
	اكسر;
	}
      }
    }
  }
}
/* end of private API use */
#endif

صحيح
main (صحيح argc, حرف **argv)
{
  إذا (argc != 2)
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
#إذا !defined(HB_NO_COLOR) && !defined(HB_NO_DRAW)
  dump_glyphs (blob, argv[1]);
#endif
  hb_blob_destroy (blob);

  أرجع 0;
}
