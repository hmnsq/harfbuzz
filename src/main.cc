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
#അന്തര്ഭാവ "config.h"
#endif

#അന്തര്ഭാവ "hb.h"
#അന്തര്ഭാവ "hb-ot.h"

#അന്തര്ഭാവ <cassert>
#അന്തര്ഭാവ <cstdlib>
#അന്തര്ഭാവ <cstdio>
#അന്തര്ഭാവ <cstring>

#ifdef HB_NO_OPEN
#define hb_blob_create_from_file_or_fail(x)  hb_blob_get_empty ()
#endif

#എങ്കിൽ !defined(HB_NO_COLOR) && !defined(HB_NO_DRAW)
#ifndef HB_NO_SVG
സ്ഥിതം ശൂന്യം
svg_dump (hb_face_t *face, unsigned face_index)
{
  unsigned glyph_എണ്ണം = hb_face_get_glyph_എണ്ണം (face);

  വേണ്ടി (unsigned glyph_id = 0; glyph_id < glyph_എണ്ണം; ++glyph_id)
  {
    hb_blob_t *blob = hb_ot_color_glyph_reference_svg (face, glyph_id);

    എങ്കിൽ (hb_blob_get_length (blob) == 0) തുടരുക;

    unsigned length;
    സ്ഥിരം അക്ഷരം *data = hb_blob_get_data (blob, &length);

    അക്ഷരം output_path[255];
    snprintf (output_path, വലുപ്പം output_path,
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
സ്ഥിതം ശൂന്യം
png_dump (hb_face_t *face, unsigned face_index)
{
  unsigned glyph_എണ്ണം = hb_face_get_glyph_എണ്ണം (face);
  hb_font_t *font = hb_font_create (face);

  /* scans the font for strikes */
  unsigned sample_glyph_id;
  /* we don't care about different strikes for different glyphs at this point */
  വേണ്ടി (sample_glyph_id = 0; sample_glyph_id < glyph_എണ്ണം; ++sample_glyph_id)
  {
    hb_blob_t *blob = hb_ot_color_glyph_reference_png (font, sample_glyph_id);
    unsigned blob_length = hb_blob_get_length (blob);
    hb_blob_destroy (blob);
    എങ്കിൽ (blob_length != 0)
      നിർത്തുക;
  }

  unsigned upem = hb_face_get_upem (face);
  unsigned blob_length = 0;
  unsigned strike = 0;
  വേണ്ടി (unsigned ppem = 1; ppem < upem; ++ppem)
  {
    hb_font_set_ppem (font, ppem, ppem);
    hb_blob_t *blob = hb_ot_color_glyph_reference_png (font, sample_glyph_id);
    unsigned new_blob_length = hb_blob_get_length (blob);
    hb_blob_destroy (blob);
    എങ്കിൽ (new_blob_length != blob_length)
    {
      വേണ്ടി (unsigned glyph_id = 0; glyph_id < glyph_എണ്ണം; ++glyph_id)
      {
	hb_blob_t *blob = hb_ot_color_glyph_reference_png (font, glyph_id);

	എങ്കിൽ (hb_blob_get_length (blob) == 0) തുടരുക;

	unsigned length;
	സ്ഥിരം അക്ഷരം *data = hb_blob_get_data (blob, &length);

	അക്ഷരം output_path[255];
	snprintf (output_path, വലുപ്പം output_path, "out/png-%u-%u-%u.png", glyph_id, strike, face_index);

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

ഘടന draw_data_t
{
  FILE *f;
  hb_position_t ascender;
};

സ്ഥിതം ശൂന്യം
move_to (hb_draw_funcs_t *, draw_data_t *draw_data,
	 hb_draw_state_t *,
	 ദശാംശം to_x, ദശാംശം to_y,
	 ശൂന്യം *)
{
  fprintf (draw_data->f, "M%g,%g", to_x, draw_data->ascender - to_y);
}

സ്ഥിതം ശൂന്യം
വരി_to (hb_draw_funcs_t *, draw_data_t *draw_data,
	 hb_draw_state_t *,
	 ദശാംശം to_x, ദശാംശം to_y,
	 ശൂന്യം *)
{
  fprintf (draw_data->f, "L%g,%g", to_x, draw_data->ascender - to_y);
}

സ്ഥിതം ശൂന്യം
quadratic_to (hb_draw_funcs_t *, draw_data_t *draw_data,
	      hb_draw_state_t *,
	      ദശാംശം control_x, ദശാംശം control_y,
	      ദശാംശം to_x, ദശാംശം to_y,
	      ശൂന്യം *)
{
  fprintf (draw_data->f, "Q%g,%g %g,%g", control_x, draw_data->ascender - control_y,
					to_x, draw_data->ascender - to_y);
}

സ്ഥിതം ശൂന്യം
cubic_to (hb_draw_funcs_t *, draw_data_t *draw_data,
	  hb_draw_state_t *,
	  ദശാംശം control1_x, ദശാംശം control1_y,
	  ദശാംശം control2_x, ദശാംശം control2_y,
	  ദശാംശം to_x, ദശാംശം to_y,
	  ശൂന്യം *)
{
  fprintf (draw_data->f, "C%g,%g %g,%g %g,%g", control1_x, draw_data->ascender - control1_y,
					       control2_x, draw_data->ascender - control2_y,
					       to_x, draw_data->ascender - to_y);
}

സ്ഥിതം ശൂന്യം
close_path (hb_draw_funcs_t *, draw_data_t *draw_data,
	    hb_draw_state_t *,
	    ശൂന്യം *)
{
  fprintf (draw_data->f, "Z");
}

സ്ഥിതം ശൂന്യം
layered_glyph_dump (hb_font_t *font, hb_draw_funcs_t *funcs, unsigned face_index)
{
  hb_face_t *face = hb_font_get_face (font);
  unsigned palette_എണ്ണം = hb_ot_color_palette_get_എണ്ണം (face);
  വേണ്ടി (unsigned palette = 0; palette < palette_എണ്ണം; ++palette)
  {
    unsigned അക്കം_colors = hb_ot_color_palette_get_colors (face, palette, 0, nullptr, nullptr);
    എങ്കിൽ (!അക്കം_colors) തുടരുക;

    hb_color_t *colors = (hb_color_t*) calloc (അക്കം_colors, വലുപ്പം (hb_color_t));
    hb_ot_color_palette_get_colors (face, palette, 0, &അക്കം_colors, colors);
    എങ്കിൽ (!അക്കം_colors)
    {
      സ്വതന്ത്രം (colors);
      തുടരുക;
    }

    unsigned അക്കം_glyphs = hb_face_get_glyph_എണ്ണം (face);
    വേണ്ടി (hb_codepoint_t gid = 0; gid < അക്കം_glyphs; ++gid)
    {
      unsigned അക്കം_layers = hb_ot_color_glyph_get_layers (face, gid, 0, nullptr, nullptr);
      എങ്കിൽ (!അക്കം_layers) തുടരുക;

      hb_ot_color_layer_t *layers = (hb_ot_color_layer_t*) malloc (അക്കം_layers * വലുപ്പം (hb_ot_color_layer_t));

      hb_ot_color_glyph_get_layers (face, gid, 0, &അക്കം_layers, layers);
      എങ്കിൽ (അക്കം_layers)
      {
	hb_font_extents_t font_extents;
	hb_font_get_extents_for_direction (font, HB_DIRECTION_LTR, &font_extents);
	hb_glyph_extents_t extents = {0, 0, 0, 0};
	എങ്കിൽ (!hb_font_get_glyph_extents (font, gid, &extents))
	{
	  printf ("Skip gid: %u\n", gid);
	  തുടരുക;
	}

	അക്ഷരം output_path[255];
	snprintf (output_path, വലുപ്പം output_path, "out/colr-%u-%u-%u.svg", gid, palette, face_index);
	FILE *f = fopen (output_path, "wb");
	fprintf (f, "<svg xmlns=\"http://www.w3.org/2000/svg\""
		    " viewBox=\"%d %d %lld %lld\">\n",
		    extents.x_bearing, 0,
		    (long long) extents.x_bearing + extents.width,
		    -(long long) extents.height);
	draw_data_t draw_data;
	draw_data.ascender = extents.y_bearing;
	draw_data.f = f;

	വേണ്ടി (unsigned layer = 0; layer < അക്കം_layers; ++layer)
	{
	  hb_color_t color = 0x000000FF;
	  എങ്കിൽ (layers[layer].color_index != 0xFFFF)
	    color = colors[layers[layer].color_index];
	  fprintf (f, "<path fill=\"#%02X%02X%02X\" ",
		   hb_color_get_red (color), hb_color_get_green (color), hb_color_get_green (color));
	  എങ്കിൽ (hb_color_get_alpha (color) != 255)
	    fprintf (f, "fill-opacity=\"%.3f\"", (ഇരട്ട) hb_color_get_alpha (color) / 255.);
	  fprintf (f, "d=\"");
	  hb_font_draw_glyph (font, layers[layer].glyph, funcs, &draw_data);
	  fprintf (f, "\"/>\n");
	}

	fprintf (f, "</svg>");
	fclose (f);
      }
      സ്വതന്ത്രം (layers);
    }

    സ്വതന്ത്രം (colors);
  }
}

സ്ഥിതം ശൂന്യം
dump_glyphs (hb_font_t *font, hb_draw_funcs_t *funcs, unsigned face_index)
{
  unsigned അക്കം_glyphs = hb_face_get_glyph_എണ്ണം (hb_font_get_face (font));
  വേണ്ടി (unsigned gid = 0; gid < അക്കം_glyphs; ++gid)
  {
    hb_font_extents_t font_extents;
    hb_font_get_extents_for_direction (font, HB_DIRECTION_LTR, &font_extents);
    hb_glyph_extents_t extents = {0, 0, 0, 0};
    എങ്കിൽ (!hb_font_get_glyph_extents (font, gid, &extents))
    {
      printf ("Skip gid: %u\n", gid);
      തുടരുക;
    }

    അക്ഷരം output_path[255];
    snprintf (output_path, വലുപ്പം output_path, "out/%u-%u.svg", face_index, gid);
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

സ്ഥിതം ശൂന്യം
dump_glyphs (hb_blob_t *blob, സ്ഥിരം അക്ഷരം *font_name)
{
  FILE *font_name_file = fopen ("out/.dumped_font_name", "r");
  എങ്കിൽ (font_name_file)
  {
    fprintf (stderr, "Purge or rename ./out folder if you like to run a glyph dump,\n"
		     "run it like `rm -rf out && mkdir out && src/main font-file.ttf`\n");
    തിരികെ;
  }

  font_name_file = fopen ("out/.dumped_font_name", "w");
  എങ്കിൽ (!font_name_file)
  {
    fprintf (stderr, "./out is not accessible as a folder, create it please\n");
    തിരികെ;
  }
  fwrite (font_name, 1, strlen (font_name), font_name_file);
  fclose (font_name_file);

  hb_draw_funcs_t *funcs = hb_draw_funcs_create ();
  hb_draw_funcs_set_move_to_func (funcs, (hb_draw_move_to_func_t) move_to, nullptr, nullptr);
  hb_draw_funcs_set_വരി_to_func (funcs, (hb_draw_വരി_to_func_t) വരി_to, nullptr, nullptr);
  hb_draw_funcs_set_quadratic_to_func (funcs, (hb_draw_quadratic_to_func_t) quadratic_to, nullptr, nullptr);
  hb_draw_funcs_set_cubic_to_func (funcs, (hb_draw_cubic_to_func_t) cubic_to, nullptr, nullptr);
  hb_draw_funcs_set_close_path_func (funcs, (hb_draw_close_path_func_t) close_path, nullptr, nullptr);

  unsigned അക്കം_faces = hb_face_എണ്ണം (blob);
  വേണ്ടി (unsigned face_index = 0; face_index < അക്കം_faces; ++face_index)
  {
    hb_face_t *face = hb_face_create (blob, face_index);
    hb_font_t *font = hb_font_create (face);

    എങ്കിൽ (hb_ot_color_has_png (face))
      printf ("Dumping png (CBDT/sbix)...\n");
    png_dump (face, face_index);

#ifndef HB_NO_SVG
    എങ്കിൽ (hb_ot_color_has_svg (face))
      printf ("Dumping svg (SVG )...\n");
    svg_dump (face, face_index);
#endif

    എങ്കിൽ (hb_ot_color_has_layers (face) && hb_ot_color_has_palettes (face))
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
#അന്തര്ഭാവ "hb-static.cc"
#അന്തര്ഭാവ "hb-open-file.hh"
#അന്തര്ഭാവ "hb-ot-layout-gdef-table.hh"
#അന്തര്ഭാവ "hb-ot-layout-gsubgpos.hh"

using നാമാകാശ OT;

സ്ഥിതം ശൂന്യം
print_layout_info_using_private_api (hb_blob_t *blob)
{
  സ്ഥിരം അക്ഷരം *font_data = hb_blob_get_data (blob, nullptr);
  hb_blob_t *font_blob = hb_sanitize_context_t ().sanitize_blob<OpenTypeFontFile> (blob);
  സ്ഥിരം OpenTypeFontFile* sanitized = font_blob->as<OpenTypeFontFile> ();
  എങ്കിൽ (!font_blob->data)
  {
    printf ("Sanitization of the file wasn't successful. Exit");
    exit (1);
  }
  സ്ഥിരം OpenTypeFontFile& ot = *sanitized;

  തിരഞ്ഞെടുപ്പ് (ot.get_tag ())
  {
  അവസ്ഥ OpenTypeFontFile::TrueTypeTag:
    printf ("OpenType font with TrueType outlines\n");
    നിർത്തുക;
  അവസ്ഥ OpenTypeFontFile::CFFTag:
    printf ("OpenType font with CFF (Type1) outlines\n");
    നിർത്തുക;
  അവസ്ഥ OpenTypeFontFile::TTCTag:
    printf ("TrueType Collection of OpenType fonts\n");
    നിർത്തുക;
  അവസ്ഥ OpenTypeFontFile::TrueTag:
    printf ("Obsolete Apple TrueType font\n");
    നിർത്തുക;
  അവസ്ഥ OpenTypeFontFile::Typ1Tag:
    printf ("Obsolete Apple Type1 font in SFNT container\n");
    നിർത്തുക;
  അവസ്ഥ OpenTypeFontFile::DFontTag:
    printf ("DFont Mac Resource Fork\n");
    നിർത്തുക;
  സ്ഥിരസ്ഥിതി:
    printf ("Unknown font format\n");
    നിർത്തുക;
  }

  unsigned അക്കം_faces = hb_face_എണ്ണം (blob);
  printf ("%u font(s) found in file\n", അക്കം_faces);
  വേണ്ടി (unsigned n_font = 0; n_font < അക്കം_faces; ++n_font)
  {
    സ്ഥിരം OpenTypeFontFace &font = ot.get_face (n_font);
    printf ("Font %u of %u:\n", n_font, അക്കം_faces);

    unsigned അക്കം_tables = font.get_table_എണ്ണം ();
    printf ("  %u table(s) found in font\n", അക്കം_tables);
    വേണ്ടി (unsigned n_table = 0; n_table < അക്കം_tables; ++n_table)
    {
      സ്ഥിരം OpenTypeTable &table = font.get_table (n_table);
      printf ("  Table %2u of %2u: %.4s (0x%08x+0x%08x)\n", n_table, അക്കം_tables,
	      (സ്ഥിരം അക്ഷരം *) table.tag,
	      (unsigned) table.offset,
	      (unsigned) table.length);

      തിരഞ്ഞെടുപ്പ് (table.tag)
      {

      അവസ്ഥ HB_OT_TAG_GSUB:
      അവസ്ഥ HB_OT_TAG_GPOS:
	{

	സ്ഥിരം GSUBGPOS &g = *reinterpret_cast<സ്ഥിരം GSUBGPOS *> (font_data + table.offset);

	unsigned അക്കം_scripts = g.get_script_എണ്ണം ();
	printf ("    %u script(s) found in table\n", അക്കം_scripts);
	വേണ്ടി (unsigned n_script = 0; n_script < അക്കം_scripts; ++n_script)
	{
	  സ്ഥിരം Script &script = g.get_script (n_script);
	  printf ("    Script %2u of %2u: %.4s\n", n_script, അക്കം_scripts,
		  (സ്ഥിരം അക്ഷരം *) g.get_script_tag (n_script));

	  എങ്കിൽ (!script.has_default_lang_sys ())
	    printf ("      No default language system\n");
	  പൂർണ്ണസംഖ്യ അക്കം_langsys = script.get_lang_sys_എണ്ണം ();
	  printf ("      %d language system(s) found in script\n", അക്കം_langsys);
	  വേണ്ടി (പൂർണ്ണസംഖ്യ n_langsys = script.has_default_lang_sys () ? -1 : 0; n_langsys < അക്കം_langsys; ++n_langsys)
	  {
	    സ്ഥിരം LangSys &langsys = n_langsys == -1
				   ? script.get_default_lang_sys ()
				   : script.get_lang_sys (n_langsys);
	    എങ്കിൽ (n_langsys == -1)
	      printf ("      Default Language System\n");
	    അല്ലെങ്കിൽ
	      printf ("      Language System %2d of %2d: %.4s\n", n_langsys, അക്കം_langsys,
		      (സ്ഥിരം അക്ഷരം *) script.get_lang_sys_tag (n_langsys));
	    എങ്കിൽ (!langsys.has_required_feature ())
	      printf ("        No required feature\n");
	    അല്ലെങ്കിൽ
	      printf ("        Required feature index: %u\n",
		      langsys.get_required_feature_index ());

	    unsigned അക്കം_features = langsys.get_feature_എണ്ണം ();
	    printf ("        %u feature(s) found in language system\n", അക്കം_features);
	    വേണ്ടി (unsigned n_feature = 0; n_feature < അക്കം_features; ++n_feature)
	    {
	      printf ("        Feature index %2u of %2u: %u\n", n_feature, അക്കം_features,
		      langsys.get_feature_index (n_feature));
	    }
	  }
	}

	unsigned അക്കം_features = g.get_feature_എണ്ണം ();
	printf ("    %u feature(s) found in table\n", അക്കം_features);
	വേണ്ടി (unsigned n_feature = 0; n_feature < അക്കം_features; ++n_feature)
	{
	  സ്ഥിരം Feature &feature = g.get_feature (n_feature);
	  unsigned അക്കം_lookups = feature.get_lookup_എണ്ണം ();
	  printf ("    Feature %2u of %2u: %c%c%c%c\n", n_feature, അക്കം_features,
		  HB_UNTAG (g.get_feature_tag (n_feature)));

	  printf ("        %u lookup(s) found in feature\n", അക്കം_lookups);
	  വേണ്ടി (unsigned n_lookup = 0; n_lookup < അക്കം_lookups; ++n_lookup) {
	    printf ("        Lookup index %2u of %2u: %u\n", n_lookup, അക്കം_lookups,
		    feature.get_lookup_index (n_lookup));
	  }
	}

	unsigned അക്കം_lookups = g.get_lookup_എണ്ണം ();
	printf ("    %u lookup(s) found in table\n", അക്കം_lookups);
	വേണ്ടി (unsigned n_lookup = 0; n_lookup < അക്കം_lookups; ++n_lookup)
	{
	  സ്ഥിരം Lookup &lookup = g.get_lookup (n_lookup);
	  printf ("    Lookup %2u of %2u: type %u, props 0x%04X\n", n_lookup, അക്കം_lookups,
		  lookup.get_type (), lookup.get_props ());
	}

	}
	നിർത്തുക;

      അവസ്ഥ GDEF::tableTag:
	{

	സ്ഥിരം GDEF &gdef = *reinterpret_cast<സ്ഥിരം GDEF *> (font_data + table.offset);

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
	നിർത്തുക;
	}
      }
    }
  }
}
/* end of private API use */
#endif

പൂർണ്ണസംഖ്യ
പ്രധാന (പൂർണ്ണസംഖ്യ argc, അക്ഷരം **argv)
{
  എങ്കിൽ (argc != 2)
  {
    fprintf (stderr, "usage: %s font-file.ttf\n\n"
		     "This tools is unsupported and crashes on bad data.\nDon't use it.\n", argv[0]);
    exit (1);
  }

  hb_blob_t *blob = hb_blob_create_from_file_or_fail (argv[1]);
  പ്രതിജ്ഞാ (blob);
  printf ("Opened font file %s: %u bytes long\n", argv[1], hb_blob_get_length (blob));
#ifndef MAIN_CC_NO_PRIVATE_API
  print_layout_info_using_private_api (blob);
#endif
#എങ്കിൽ !defined(HB_NO_COLOR) && !defined(HB_NO_DRAW)
  dump_glyphs (blob, argv[1]);
#endif
  hb_blob_destroy (blob);

  തിരികെ 0;
}
