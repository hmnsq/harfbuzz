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
#அந்தர்பாவ "config.h"
#endif

#அந்தர்பாவ "hb.h"
#அந்தர்பாவ "hb-ot.h"

#அந்தர்பாவ <cassert>
#அந்தர்பாவ <cstdlib>
#அந்தர்பாவ <cstdio>
#அந்தர்பாவ <cstring>

#ifdef HB_NO_OPEN
#define hb_blob_create_from_file_or_fail(x)  hb_blob_get_empty ()
#endif

#எனில் !defined(HB_NO_COLOR) && !defined(HB_NO_DRAW)
#ifndef HB_NO_SVG
நிலையான வெறுமை
svg_dump (hb_face_t *face, unsigned face_index)
{
  unsigned glyph_மொத்தம் = hb_face_get_glyph_மொத்தம் (face);

  க்கு (unsigned glyph_id = 0; glyph_id < glyph_மொத்தம்; ++glyph_id)
  {
    hb_blob_t *blob = hb_ot_color_glyph_reference_svg (face, glyph_id);

    எனில் (hb_blob_get_length (blob) == 0) தொடர்;

    unsigned length;
    மாறிலி எழுத்து *data = hb_blob_get_data (blob, &length);

    எழுத்து output_path[255];
    snprintf (output_path, அளவு output_path,
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
நிலையான வெறுமை
png_dump (hb_face_t *face, unsigned face_index)
{
  unsigned glyph_மொத்தம் = hb_face_get_glyph_மொத்தம் (face);
  hb_font_t *font = hb_font_create (face);

  /* scans the font for strikes */
  unsigned sample_glyph_id;
  /* we don't care about different strikes for different glyphs at this point */
  க்கு (sample_glyph_id = 0; sample_glyph_id < glyph_மொத்தம்; ++sample_glyph_id)
  {
    hb_blob_t *blob = hb_ot_color_glyph_reference_png (font, sample_glyph_id);
    unsigned blob_length = hb_blob_get_length (blob);
    hb_blob_destroy (blob);
    எனில் (blob_length != 0)
      நிறுத்து;
  }

  unsigned upem = hb_face_get_upem (face);
  unsigned blob_length = 0;
  unsigned strike = 0;
  க்கு (unsigned ppem = 1; ppem < upem; ++ppem)
  {
    hb_font_set_ppem (font, ppem, ppem);
    hb_blob_t *blob = hb_ot_color_glyph_reference_png (font, sample_glyph_id);
    unsigned new_blob_length = hb_blob_get_length (blob);
    hb_blob_destroy (blob);
    எனில் (new_blob_length != blob_length)
    {
      க்கு (unsigned glyph_id = 0; glyph_id < glyph_மொத்தம்; ++glyph_id)
      {
	hb_blob_t *blob = hb_ot_color_glyph_reference_png (font, glyph_id);

	எனில் (hb_blob_get_length (blob) == 0) தொடர்;

	unsigned length;
	மாறிலி எழுத்து *data = hb_blob_get_data (blob, &length);

	எழுத்து output_path[255];
	snprintf (output_path, அளவு output_path, "out/png-%u-%u-%u.png", glyph_id, strike, face_index);

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

கட்டமைப்பு draw_data_t
{
  FILE *f;
  hb_position_t ascender;
};

நிலையான வெறுமை
move_to (hb_draw_funcs_t *, draw_data_t *draw_data,
	 hb_draw_state_t *,
	 தசம to_x, தசம to_y,
	 வெறுமை *)
{
  fprintf (draw_data->f, "M%g,%g", to_x, draw_data->ascender - to_y);
}

நிலையான வெறுமை
வரி_to (hb_draw_funcs_t *, draw_data_t *draw_data,
	 hb_draw_state_t *,
	 தசம to_x, தசம to_y,
	 வெறுமை *)
{
  fprintf (draw_data->f, "L%g,%g", to_x, draw_data->ascender - to_y);
}

நிலையான வெறுமை
quadratic_to (hb_draw_funcs_t *, draw_data_t *draw_data,
	      hb_draw_state_t *,
	      தசம control_x, தசம control_y,
	      தசம to_x, தசம to_y,
	      வெறுமை *)
{
  fprintf (draw_data->f, "Q%g,%g %g,%g", control_x, draw_data->ascender - control_y,
					to_x, draw_data->ascender - to_y);
}

நிலையான வெறுமை
cubic_to (hb_draw_funcs_t *, draw_data_t *draw_data,
	  hb_draw_state_t *,
	  தசம control1_x, தசம control1_y,
	  தசம control2_x, தசம control2_y,
	  தசம to_x, தசம to_y,
	  வெறுமை *)
{
  fprintf (draw_data->f, "C%g,%g %g,%g %g,%g", control1_x, draw_data->ascender - control1_y,
					       control2_x, draw_data->ascender - control2_y,
					       to_x, draw_data->ascender - to_y);
}

நிலையான வெறுமை
close_path (hb_draw_funcs_t *, draw_data_t *draw_data,
	    hb_draw_state_t *,
	    வெறுமை *)
{
  fprintf (draw_data->f, "Z");
}

நிலையான வெறுமை
layered_glyph_dump (hb_font_t *font, hb_draw_funcs_t *funcs, unsigned face_index)
{
  hb_face_t *face = hb_font_get_face (font);
  unsigned palette_மொத்தம் = hb_ot_color_palette_get_மொத்தம் (face);
  க்கு (unsigned palette = 0; palette < palette_மொத்தம்; ++palette)
  {
    unsigned இலக்கம்_colors = hb_ot_color_palette_get_colors (face, palette, 0, nullptr, nullptr);
    எனில் (!இலக்கம்_colors) தொடர்;

    hb_color_t *colors = (hb_color_t*) calloc (இலக்கம்_colors, அளவு (hb_color_t));
    hb_ot_color_palette_get_colors (face, palette, 0, &இலக்கம்_colors, colors);
    எனில் (!இலக்கம்_colors)
    {
      விடுவி (colors);
      தொடர்;
    }

    unsigned இலக்கம்_glyphs = hb_face_get_glyph_மொத்தம் (face);
    க்கு (hb_codepoint_t gid = 0; gid < இலக்கம்_glyphs; ++gid)
    {
      unsigned இலக்கம்_layers = hb_ot_color_glyph_get_layers (face, gid, 0, nullptr, nullptr);
      எனில் (!இலக்கம்_layers) தொடர்;

      hb_ot_color_layer_t *layers = (hb_ot_color_layer_t*) malloc (இலக்கம்_layers * அளவு (hb_ot_color_layer_t));

      hb_ot_color_glyph_get_layers (face, gid, 0, &இலக்கம்_layers, layers);
      எனில் (இலக்கம்_layers)
      {
	hb_font_extents_t font_extents;
	hb_font_get_extents_for_direction (font, HB_DIRECTION_LTR, &font_extents);
	hb_glyph_extents_t extents = {0, 0, 0, 0};
	எனில் (!hb_font_get_glyph_extents (font, gid, &extents))
	{
	  printf ("Skip gid: %u\n", gid);
	  தொடர்;
	}

	எழுத்து output_path[255];
	snprintf (output_path, அளவு output_path, "out/colr-%u-%u-%u.svg", gid, palette, face_index);
	FILE *f = fopen (output_path, "wb");
	fprintf (f, "<svg xmlns=\"http://www.w3.org/2000/svg\""
		    " viewBox=\"%d %d %lld %lld\">\n",
		    extents.x_bearing, 0,
		    (long long) extents.x_bearing + extents.width,
		    -(long long) extents.height);
	draw_data_t draw_data;
	draw_data.ascender = extents.y_bearing;
	draw_data.f = f;

	க்கு (unsigned layer = 0; layer < இலக்கம்_layers; ++layer)
	{
	  hb_color_t color = 0x000000FF;
	  எனில் (layers[layer].color_index != 0xFFFF)
	    color = colors[layers[layer].color_index];
	  fprintf (f, "<path fill=\"#%02X%02X%02X\" ",
		   hb_color_get_red (color), hb_color_get_green (color), hb_color_get_green (color));
	  எனில் (hb_color_get_alpha (color) != 255)
	    fprintf (f, "fill-opacity=\"%.3f\"", (இரட்டை) hb_color_get_alpha (color) / 255.);
	  fprintf (f, "d=\"");
	  hb_font_draw_glyph (font, layers[layer].glyph, funcs, &draw_data);
	  fprintf (f, "\"/>\n");
	}

	fprintf (f, "</svg>");
	fclose (f);
      }
      விடுவி (layers);
    }

    விடுவி (colors);
  }
}

நிலையான வெறுமை
dump_glyphs (hb_font_t *font, hb_draw_funcs_t *funcs, unsigned face_index)
{
  unsigned இலக்கம்_glyphs = hb_face_get_glyph_மொத்தம் (hb_font_get_face (font));
  க்கு (unsigned gid = 0; gid < இலக்கம்_glyphs; ++gid)
  {
    hb_font_extents_t font_extents;
    hb_font_get_extents_for_direction (font, HB_DIRECTION_LTR, &font_extents);
    hb_glyph_extents_t extents = {0, 0, 0, 0};
    எனில் (!hb_font_get_glyph_extents (font, gid, &extents))
    {
      printf ("Skip gid: %u\n", gid);
      தொடர்;
    }

    எழுத்து output_path[255];
    snprintf (output_path, அளவு output_path, "out/%u-%u.svg", face_index, gid);
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

நிலையான வெறுமை
dump_glyphs (hb_blob_t *blob, மாறிலி எழுத்து *font_name)
{
  FILE *font_name_file = fopen ("out/.dumped_font_name", "r");
  எனில் (font_name_file)
  {
    fprintf (stderr, "Purge or rename ./out folder if you like to run a glyph dump,\n"
		     "run it like `rm -rf out && mkdir out && src/main font-file.ttf`\n");
    திரும்பு;
  }

  font_name_file = fopen ("out/.dumped_font_name", "w");
  எனில் (!font_name_file)
  {
    fprintf (stderr, "./out is not accessible as a folder, create it please\n");
    திரும்பு;
  }
  fwrite (font_name, 1, strlen (font_name), font_name_file);
  fclose (font_name_file);

  hb_draw_funcs_t *funcs = hb_draw_funcs_create ();
  hb_draw_funcs_set_move_to_func (funcs, (hb_draw_move_to_func_t) move_to, nullptr, nullptr);
  hb_draw_funcs_set_வரி_to_func (funcs, (hb_draw_வரி_to_func_t) வரி_to, nullptr, nullptr);
  hb_draw_funcs_set_quadratic_to_func (funcs, (hb_draw_quadratic_to_func_t) quadratic_to, nullptr, nullptr);
  hb_draw_funcs_set_cubic_to_func (funcs, (hb_draw_cubic_to_func_t) cubic_to, nullptr, nullptr);
  hb_draw_funcs_set_close_path_func (funcs, (hb_draw_close_path_func_t) close_path, nullptr, nullptr);

  unsigned இலக்கம்_faces = hb_face_மொத்தம் (blob);
  க்கு (unsigned face_index = 0; face_index < இலக்கம்_faces; ++face_index)
  {
    hb_face_t *face = hb_face_create (blob, face_index);
    hb_font_t *font = hb_font_create (face);

    எனில் (hb_ot_color_has_png (face))
      printf ("Dumping png (CBDT/sbix)...\n");
    png_dump (face, face_index);

#ifndef HB_NO_SVG
    எனில் (hb_ot_color_has_svg (face))
      printf ("Dumping svg (SVG )...\n");
    svg_dump (face, face_index);
#endif

    எனில் (hb_ot_color_has_layers (face) && hb_ot_color_has_palettes (face))
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
#அந்தர்பாவ "hb-static.cc"
#அந்தர்பாவ "hb-open-file.hh"
#அந்தர்பாவ "hb-ot-layout-gdef-table.hh"
#அந்தர்பாவ "hb-ot-layout-gsubgpos.hh"

using நாமாகாஶ OT;

நிலையான வெறுமை
print_layout_info_using_private_api (hb_blob_t *blob)
{
  மாறிலி எழுத்து *font_data = hb_blob_get_data (blob, nullptr);
  hb_blob_t *font_blob = hb_sanitize_context_t ().sanitize_blob<OpenTypeFontFile> (blob);
  மாறிலி OpenTypeFontFile* sanitized = font_blob->as<OpenTypeFontFile> ();
  எனில் (!font_blob->data)
  {
    printf ("Sanitization of the file wasn't successful. Exit");
    exit (1);
  }
  மாறிலி OpenTypeFontFile& ot = *sanitized;

  தேர்வு (ot.get_tag ())
  {
  நிலை OpenTypeFontFile::TrueTypeTag:
    printf ("OpenType font with TrueType outlines\n");
    நிறுத்து;
  நிலை OpenTypeFontFile::CFFTag:
    printf ("OpenType font with CFF (Type1) outlines\n");
    நிறுத்து;
  நிலை OpenTypeFontFile::TTCTag:
    printf ("TrueType Collection of OpenType fonts\n");
    நிறுத்து;
  நிலை OpenTypeFontFile::TrueTag:
    printf ("Obsolete Apple TrueType font\n");
    நிறுத்து;
  நிலை OpenTypeFontFile::Typ1Tag:
    printf ("Obsolete Apple Type1 font in SFNT container\n");
    நிறுத்து;
  நிலை OpenTypeFontFile::DFontTag:
    printf ("DFont Mac Resource Fork\n");
    நிறுத்து;
  இயல்பு:
    printf ("Unknown font format\n");
    நிறுத்து;
  }

  unsigned இலக்கம்_faces = hb_face_மொத்தம் (blob);
  printf ("%u font(s) found in file\n", இலக்கம்_faces);
  க்கு (unsigned n_font = 0; n_font < இலக்கம்_faces; ++n_font)
  {
    மாறிலி OpenTypeFontFace &font = ot.get_face (n_font);
    printf ("Font %u of %u:\n", n_font, இலக்கம்_faces);

    unsigned இலக்கம்_tables = font.get_table_மொத்தம் ();
    printf ("  %u table(s) found in font\n", இலக்கம்_tables);
    க்கு (unsigned n_table = 0; n_table < இலக்கம்_tables; ++n_table)
    {
      மாறிலி OpenTypeTable &table = font.get_table (n_table);
      printf ("  Table %2u of %2u: %.4s (0x%08x+0x%08x)\n", n_table, இலக்கம்_tables,
	      (மாறிலி எழுத்து *) table.tag,
	      (unsigned) table.offset,
	      (unsigned) table.length);

      தேர்வு (table.tag)
      {

      நிலை HB_OT_TAG_GSUB:
      நிலை HB_OT_TAG_GPOS:
	{

	மாறிலி GSUBGPOS &g = *reinterpret_cast<மாறிலி GSUBGPOS *> (font_data + table.offset);

	unsigned இலக்கம்_scripts = g.get_script_மொத்தம் ();
	printf ("    %u script(s) found in table\n", இலக்கம்_scripts);
	க்கு (unsigned n_script = 0; n_script < இலக்கம்_scripts; ++n_script)
	{
	  மாறிலி Script &script = g.get_script (n_script);
	  printf ("    Script %2u of %2u: %.4s\n", n_script, இலக்கம்_scripts,
		  (மாறிலி எழுத்து *) g.get_script_tag (n_script));

	  எனில் (!script.has_default_lang_sys ())
	    printf ("      No default language system\n");
	  முழு இலக்கம்_langsys = script.get_lang_sys_மொத்தம் ();
	  printf ("      %d language system(s) found in script\n", இலக்கம்_langsys);
	  க்கு (முழு n_langsys = script.has_default_lang_sys () ? -1 : 0; n_langsys < இலக்கம்_langsys; ++n_langsys)
	  {
	    மாறிலி LangSys &langsys = n_langsys == -1
				   ? script.get_default_lang_sys ()
				   : script.get_lang_sys (n_langsys);
	    எனில் (n_langsys == -1)
	      printf ("      Default Language System\n");
	    இல்லையெனில்
	      printf ("      Language System %2d of %2d: %.4s\n", n_langsys, இலக்கம்_langsys,
		      (மாறிலி எழுத்து *) script.get_lang_sys_tag (n_langsys));
	    எனில் (!langsys.has_required_feature ())
	      printf ("        No required feature\n");
	    இல்லையெனில்
	      printf ("        Required feature index: %u\n",
		      langsys.get_required_feature_index ());

	    unsigned இலக்கம்_features = langsys.get_feature_மொத்தம் ();
	    printf ("        %u feature(s) found in language system\n", இலக்கம்_features);
	    க்கு (unsigned n_feature = 0; n_feature < இலக்கம்_features; ++n_feature)
	    {
	      printf ("        Feature index %2u of %2u: %u\n", n_feature, இலக்கம்_features,
		      langsys.get_feature_index (n_feature));
	    }
	  }
	}

	unsigned இலக்கம்_features = g.get_feature_மொத்தம் ();
	printf ("    %u feature(s) found in table\n", இலக்கம்_features);
	க்கு (unsigned n_feature = 0; n_feature < இலக்கம்_features; ++n_feature)
	{
	  மாறிலி Feature &feature = g.get_feature (n_feature);
	  unsigned இலக்கம்_lookups = feature.get_lookup_மொத்தம் ();
	  printf ("    Feature %2u of %2u: %c%c%c%c\n", n_feature, இலக்கம்_features,
		  HB_UNTAG (g.get_feature_tag (n_feature)));

	  printf ("        %u lookup(s) found in feature\n", இலக்கம்_lookups);
	  க்கு (unsigned n_lookup = 0; n_lookup < இலக்கம்_lookups; ++n_lookup) {
	    printf ("        Lookup index %2u of %2u: %u\n", n_lookup, இலக்கம்_lookups,
		    feature.get_lookup_index (n_lookup));
	  }
	}

	unsigned இலக்கம்_lookups = g.get_lookup_மொத்தம் ();
	printf ("    %u lookup(s) found in table\n", இலக்கம்_lookups);
	க்கு (unsigned n_lookup = 0; n_lookup < இலக்கம்_lookups; ++n_lookup)
	{
	  மாறிலி Lookup &lookup = g.get_lookup (n_lookup);
	  printf ("    Lookup %2u of %2u: type %u, props 0x%04X\n", n_lookup, இலக்கம்_lookups,
		  lookup.get_type (), lookup.get_props ());
	}

	}
	நிறுத்து;

      நிலை GDEF::tableTag:
	{

	மாறிலி GDEF &gdef = *reinterpret_cast<மாறிலி GDEF *> (font_data + table.offset);

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
	நிறுத்து;
	}
      }
    }
  }
}
/* end of private API use */
#endif

முழு
முதன்மை (முழு argc, எழுத்து **argv)
{
  எனில் (argc != 2)
  {
    fprintf (stderr, "usage: %s font-file.ttf\n\n"
		     "This tools is unsupported and crashes on bad data.\nDon't use it.\n", argv[0]);
    exit (1);
  }

  hb_blob_t *blob = hb_blob_create_from_file_or_fail (argv[1]);
  ப்ரதிஜ்ஞா (blob);
  printf ("Opened font file %s: %u bytes long\n", argv[1], hb_blob_get_length (blob));
#ifndef MAIN_CC_NO_PRIVATE_API
  print_layout_info_using_private_api (blob);
#endif
#எனில் !defined(HB_NO_COLOR) && !defined(HB_NO_DRAW)
  dump_glyphs (blob, argv[1]);
#endif
  hb_blob_destroy (blob);

  திரும்பு 0;
}
