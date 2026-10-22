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
#અન્તર્ભાવ "config.h"
#endif

#અન્તર્ભાવ "hb.h"
#અન્તર્ભાવ "hb-ot.h"

#અન્તર્ભાવ <cassert>
#અન્તર્ભાવ <cstdlib>
#અન્તર્ભાવ <cstdio>
#અન્તર્ભાવ <cstring>

#ifdef HB_NO_OPEN
#define hb_blob_create_from_file_or_fail(x)  hb_blob_get_empty ()
#endif

#જો !defined(HB_NO_COLOR) && !defined(HB_NO_DRAW)
#ifndef HB_NO_SVG
સ્થિર શૂન્ય
svg_dump (hb_face_t *face, unsigned face_index)
{
  unsigned glyph_ગણતરી = hb_face_get_glyph_ગણતરી (face);

  માટે (unsigned glyph_id = 0; glyph_id < glyph_ગણતરી; ++glyph_id)
  {
    hb_blob_t *blob = hb_ot_color_glyph_reference_svg (face, glyph_id);

    જો (hb_blob_get_length (blob) == 0) ચાલુ;

    unsigned length;
    અચલ અક્ષર *data = hb_blob_get_data (blob, &length);

    અક્ષર output_path[255];
    snprintf (output_path, કદ output_path,
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
સ્થિર શૂન્ય
png_dump (hb_face_t *face, unsigned face_index)
{
  unsigned glyph_ગણતરી = hb_face_get_glyph_ગણતરી (face);
  hb_font_t *font = hb_font_create (face);

  /* scans the font for strikes */
  unsigned sample_glyph_id;
  /* we don't care about different strikes for different glyphs at this point */
  માટે (sample_glyph_id = 0; sample_glyph_id < glyph_ગણતરી; ++sample_glyph_id)
  {
    hb_blob_t *blob = hb_ot_color_glyph_reference_png (font, sample_glyph_id);
    unsigned blob_length = hb_blob_get_length (blob);
    hb_blob_destroy (blob);
    જો (blob_length != 0)
      તોડો;
  }

  unsigned upem = hb_face_get_upem (face);
  unsigned blob_length = 0;
  unsigned strike = 0;
  માટે (unsigned ppem = 1; ppem < upem; ++ppem)
  {
    hb_font_set_ppem (font, ppem, ppem);
    hb_blob_t *blob = hb_ot_color_glyph_reference_png (font, sample_glyph_id);
    unsigned new_blob_length = hb_blob_get_length (blob);
    hb_blob_destroy (blob);
    જો (new_blob_length != blob_length)
    {
      માટે (unsigned glyph_id = 0; glyph_id < glyph_ગણતરી; ++glyph_id)
      {
	hb_blob_t *blob = hb_ot_color_glyph_reference_png (font, glyph_id);

	જો (hb_blob_get_length (blob) == 0) ચાલુ;

	unsigned length;
	અચલ અક્ષર *data = hb_blob_get_data (blob, &length);

	અક્ષર output_path[255];
	snprintf (output_path, કદ output_path, "out/png-%u-%u-%u.png", glyph_id, strike, face_index);

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

સંરચના draw_data_t
{
  FILE *f;
  hb_position_t ascender;
};

સ્થિર શૂન્ય
move_to (hb_draw_funcs_t *, draw_data_t *draw_data,
	 hb_draw_state_t *,
	 દશાંશ to_x, દશાંશ to_y,
	 શૂન્ય *)
{
  fprintf (draw_data->f, "M%g,%g", to_x, draw_data->ascender - to_y);
}

સ્થિર શૂન્ય
લીટી_to (hb_draw_funcs_t *, draw_data_t *draw_data,
	 hb_draw_state_t *,
	 દશાંશ to_x, દશાંશ to_y,
	 શૂન્ય *)
{
  fprintf (draw_data->f, "L%g,%g", to_x, draw_data->ascender - to_y);
}

સ્થિર શૂન્ય
quadratic_to (hb_draw_funcs_t *, draw_data_t *draw_data,
	      hb_draw_state_t *,
	      દશાંશ control_x, દશાંશ control_y,
	      દશાંશ to_x, દશાંશ to_y,
	      શૂન્ય *)
{
  fprintf (draw_data->f, "Q%g,%g %g,%g", control_x, draw_data->ascender - control_y,
					to_x, draw_data->ascender - to_y);
}

સ્થિર શૂન્ય
cubic_to (hb_draw_funcs_t *, draw_data_t *draw_data,
	  hb_draw_state_t *,
	  દશાંશ control1_x, દશાંશ control1_y,
	  દશાંશ control2_x, દશાંશ control2_y,
	  દશાંશ to_x, દશાંશ to_y,
	  શૂન્ય *)
{
  fprintf (draw_data->f, "C%g,%g %g,%g %g,%g", control1_x, draw_data->ascender - control1_y,
					       control2_x, draw_data->ascender - control2_y,
					       to_x, draw_data->ascender - to_y);
}

સ્થિર શૂન્ય
close_path (hb_draw_funcs_t *, draw_data_t *draw_data,
	    hb_draw_state_t *,
	    શૂન્ય *)
{
  fprintf (draw_data->f, "Z");
}

સ્થિર શૂન્ય
layered_glyph_dump (hb_font_t *font, hb_draw_funcs_t *funcs, unsigned face_index)
{
  hb_face_t *face = hb_font_get_face (font);
  unsigned palette_ગણતરી = hb_ot_color_palette_get_ગણતરી (face);
  માટે (unsigned palette = 0; palette < palette_ગણતરી; ++palette)
  {
    unsigned અંક_colors = hb_ot_color_palette_get_colors (face, palette, 0, nullptr, nullptr);
    જો (!અંક_colors) ચાલુ;

    hb_color_t *colors = (hb_color_t*) calloc (અંક_colors, કદ (hb_color_t));
    hb_ot_color_palette_get_colors (face, palette, 0, &અંક_colors, colors);
    જો (!અંક_colors)
    {
      મુક્ત (colors);
      ચાલુ;
    }

    unsigned અંક_glyphs = hb_face_get_glyph_ગણતરી (face);
    માટે (hb_codepoint_t gid = 0; gid < અંક_glyphs; ++gid)
    {
      unsigned અંક_layers = hb_ot_color_glyph_get_layers (face, gid, 0, nullptr, nullptr);
      જો (!અંક_layers) ચાલુ;

      hb_ot_color_layer_t *layers = (hb_ot_color_layer_t*) malloc (અંક_layers * કદ (hb_ot_color_layer_t));

      hb_ot_color_glyph_get_layers (face, gid, 0, &અંક_layers, layers);
      જો (અંક_layers)
      {
	hb_font_extents_t font_extents;
	hb_font_get_extents_for_direction (font, HB_DIRECTION_LTR, &font_extents);
	hb_glyph_extents_t extents = {0, 0, 0, 0};
	જો (!hb_font_get_glyph_extents (font, gid, &extents))
	{
	  printf ("Skip gid: %u\n", gid);
	  ચાલુ;
	}

	અક્ષર output_path[255];
	snprintf (output_path, કદ output_path, "out/colr-%u-%u-%u.svg", gid, palette, face_index);
	FILE *f = fopen (output_path, "wb");
	fprintf (f, "<svg xmlns=\"http://www.w3.org/2000/svg\""
		    " viewBox=\"%d %d %lld %lld\">\n",
		    extents.x_bearing, 0,
		    (long long) extents.x_bearing + extents.width,
		    -(long long) extents.height);
	draw_data_t draw_data;
	draw_data.ascender = extents.y_bearing;
	draw_data.f = f;

	માટે (unsigned layer = 0; layer < અંક_layers; ++layer)
	{
	  hb_color_t color = 0x000000FF;
	  જો (layers[layer].color_index != 0xFFFF)
	    color = colors[layers[layer].color_index];
	  fprintf (f, "<path fill=\"#%02X%02X%02X\" ",
		   hb_color_get_red (color), hb_color_get_green (color), hb_color_get_green (color));
	  જો (hb_color_get_alpha (color) != 255)
	    fprintf (f, "fill-opacity=\"%.3f\"", (દ્વિગુણ) hb_color_get_alpha (color) / 255.);
	  fprintf (f, "d=\"");
	  hb_font_draw_glyph (font, layers[layer].glyph, funcs, &draw_data);
	  fprintf (f, "\"/>\n");
	}

	fprintf (f, "</svg>");
	fclose (f);
      }
      મુક્ત (layers);
    }

    મુક્ત (colors);
  }
}

સ્થિર શૂન્ય
dump_glyphs (hb_font_t *font, hb_draw_funcs_t *funcs, unsigned face_index)
{
  unsigned અંક_glyphs = hb_face_get_glyph_ગણતરી (hb_font_get_face (font));
  માટે (unsigned gid = 0; gid < અંક_glyphs; ++gid)
  {
    hb_font_extents_t font_extents;
    hb_font_get_extents_for_direction (font, HB_DIRECTION_LTR, &font_extents);
    hb_glyph_extents_t extents = {0, 0, 0, 0};
    જો (!hb_font_get_glyph_extents (font, gid, &extents))
    {
      printf ("Skip gid: %u\n", gid);
      ચાલુ;
    }

    અક્ષર output_path[255];
    snprintf (output_path, કદ output_path, "out/%u-%u.svg", face_index, gid);
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

સ્થિર શૂન્ય
dump_glyphs (hb_blob_t *blob, અચલ અક્ષર *font_name)
{
  FILE *font_name_file = fopen ("out/.dumped_font_name", "r");
  જો (font_name_file)
  {
    fprintf (stderr, "Purge or rename ./out folder if you like to run a glyph dump,\n"
		     "run it like `rm -rf out && mkdir out && src/main font-file.ttf`\n");
    પાછા;
  }

  font_name_file = fopen ("out/.dumped_font_name", "w");
  જો (!font_name_file)
  {
    fprintf (stderr, "./out is not accessible as a folder, create it please\n");
    પાછા;
  }
  fwrite (font_name, 1, strlen (font_name), font_name_file);
  fclose (font_name_file);

  hb_draw_funcs_t *funcs = hb_draw_funcs_create ();
  hb_draw_funcs_set_move_to_func (funcs, (hb_draw_move_to_func_t) move_to, nullptr, nullptr);
  hb_draw_funcs_set_લીટી_to_func (funcs, (hb_draw_લીટી_to_func_t) લીટી_to, nullptr, nullptr);
  hb_draw_funcs_set_quadratic_to_func (funcs, (hb_draw_quadratic_to_func_t) quadratic_to, nullptr, nullptr);
  hb_draw_funcs_set_cubic_to_func (funcs, (hb_draw_cubic_to_func_t) cubic_to, nullptr, nullptr);
  hb_draw_funcs_set_close_path_func (funcs, (hb_draw_close_path_func_t) close_path, nullptr, nullptr);

  unsigned અંક_faces = hb_face_ગણતરી (blob);
  માટે (unsigned face_index = 0; face_index < અંક_faces; ++face_index)
  {
    hb_face_t *face = hb_face_create (blob, face_index);
    hb_font_t *font = hb_font_create (face);

    જો (hb_ot_color_has_png (face))
      printf ("Dumping png (CBDT/sbix)...\n");
    png_dump (face, face_index);

#ifndef HB_NO_SVG
    જો (hb_ot_color_has_svg (face))
      printf ("Dumping svg (SVG )...\n");
    svg_dump (face, face_index);
#endif

    જો (hb_ot_color_has_layers (face) && hb_ot_color_has_palettes (face))
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
#અન્તર્ભાવ "hb-static.cc"
#અન્તર્ભાવ "hb-open-file.hh"
#અન્તર્ભાવ "hb-ot-layout-gdef-table.hh"
#અન્તર્ભાવ "hb-ot-layout-gsubgpos.hh"

using નામાકાશ OT;

સ્થિર શૂન્ય
print_layout_info_using_private_api (hb_blob_t *blob)
{
  અચલ અક્ષર *font_data = hb_blob_get_data (blob, nullptr);
  hb_blob_t *font_blob = hb_sanitize_context_t ().sanitize_blob<OpenTypeFontFile> (blob);
  અચલ OpenTypeFontFile* sanitized = font_blob->as<OpenTypeFontFile> ();
  જો (!font_blob->data)
  {
    printf ("Sanitization of the file wasn't successful. Exit");
    exit (1);
  }
  અચલ OpenTypeFontFile& ot = *sanitized;

  પસંદગી (ot.get_tag ())
  {
  સ્થિતિ OpenTypeFontFile::TrueTypeTag:
    printf ("OpenType font with TrueType outlines\n");
    તોડો;
  સ્થિતિ OpenTypeFontFile::CFFTag:
    printf ("OpenType font with CFF (Type1) outlines\n");
    તોડો;
  સ્થિતિ OpenTypeFontFile::TTCTag:
    printf ("TrueType Collection of OpenType fonts\n");
    તોડો;
  સ્થિતિ OpenTypeFontFile::TrueTag:
    printf ("Obsolete Apple TrueType font\n");
    તોડો;
  સ્થિતિ OpenTypeFontFile::Typ1Tag:
    printf ("Obsolete Apple Type1 font in SFNT container\n");
    તોડો;
  સ્થિતિ OpenTypeFontFile::DFontTag:
    printf ("DFont Mac Resource Fork\n");
    તોડો;
  મૂળભૂત:
    printf ("Unknown font format\n");
    તોડો;
  }

  unsigned અંક_faces = hb_face_ગણતરી (blob);
  printf ("%u font(s) found in file\n", અંક_faces);
  માટે (unsigned n_font = 0; n_font < અંક_faces; ++n_font)
  {
    અચલ OpenTypeFontFace &font = ot.get_face (n_font);
    printf ("Font %u of %u:\n", n_font, અંક_faces);

    unsigned અંક_tables = font.get_table_ગણતરી ();
    printf ("  %u table(s) found in font\n", અંક_tables);
    માટે (unsigned n_table = 0; n_table < અંક_tables; ++n_table)
    {
      અચલ OpenTypeTable &table = font.get_table (n_table);
      printf ("  Table %2u of %2u: %.4s (0x%08x+0x%08x)\n", n_table, અંક_tables,
	      (અચલ અક્ષર *) table.tag,
	      (unsigned) table.offset,
	      (unsigned) table.length);

      પસંદગી (table.tag)
      {

      સ્થિતિ HB_OT_TAG_GSUB:
      સ્થિતિ HB_OT_TAG_GPOS:
	{

	અચલ GSUBGPOS &g = *reinterpret_cast<અચલ GSUBGPOS *> (font_data + table.offset);

	unsigned અંક_scripts = g.get_script_ગણતરી ();
	printf ("    %u script(s) found in table\n", અંક_scripts);
	માટે (unsigned n_script = 0; n_script < અંક_scripts; ++n_script)
	{
	  અચલ Script &script = g.get_script (n_script);
	  printf ("    Script %2u of %2u: %.4s\n", n_script, અંક_scripts,
		  (અચલ અક્ષર *) g.get_script_tag (n_script));

	  જો (!script.has_default_lang_sys ())
	    printf ("      No default language system\n");
	  પૂર્ણાંક અંક_langsys = script.get_lang_sys_ગણતરી ();
	  printf ("      %d language system(s) found in script\n", અંક_langsys);
	  માટે (પૂર્ણાંક n_langsys = script.has_default_lang_sys () ? -1 : 0; n_langsys < અંક_langsys; ++n_langsys)
	  {
	    અચલ LangSys &langsys = n_langsys == -1
				   ? script.get_default_lang_sys ()
				   : script.get_lang_sys (n_langsys);
	    જો (n_langsys == -1)
	      printf ("      Default Language System\n");
	    નહીંતર
	      printf ("      Language System %2d of %2d: %.4s\n", n_langsys, અંક_langsys,
		      (અચલ અક્ષર *) script.get_lang_sys_tag (n_langsys));
	    જો (!langsys.has_required_feature ())
	      printf ("        No required feature\n");
	    નહીંતર
	      printf ("        Required feature index: %u\n",
		      langsys.get_required_feature_index ());

	    unsigned અંક_features = langsys.get_feature_ગણતરી ();
	    printf ("        %u feature(s) found in language system\n", અંક_features);
	    માટે (unsigned n_feature = 0; n_feature < અંક_features; ++n_feature)
	    {
	      printf ("        Feature index %2u of %2u: %u\n", n_feature, અંક_features,
		      langsys.get_feature_index (n_feature));
	    }
	  }
	}

	unsigned અંક_features = g.get_feature_ગણતરી ();
	printf ("    %u feature(s) found in table\n", અંક_features);
	માટે (unsigned n_feature = 0; n_feature < અંક_features; ++n_feature)
	{
	  અચલ Feature &feature = g.get_feature (n_feature);
	  unsigned અંક_lookups = feature.get_lookup_ગણતરી ();
	  printf ("    Feature %2u of %2u: %c%c%c%c\n", n_feature, અંક_features,
		  HB_UNTAG (g.get_feature_tag (n_feature)));

	  printf ("        %u lookup(s) found in feature\n", અંક_lookups);
	  માટે (unsigned n_lookup = 0; n_lookup < અંક_lookups; ++n_lookup) {
	    printf ("        Lookup index %2u of %2u: %u\n", n_lookup, અંક_lookups,
		    feature.get_lookup_index (n_lookup));
	  }
	}

	unsigned અંક_lookups = g.get_lookup_ગણતરી ();
	printf ("    %u lookup(s) found in table\n", અંક_lookups);
	માટે (unsigned n_lookup = 0; n_lookup < અંક_lookups; ++n_lookup)
	{
	  અચલ Lookup &lookup = g.get_lookup (n_lookup);
	  printf ("    Lookup %2u of %2u: type %u, props 0x%04X\n", n_lookup, અંક_lookups,
		  lookup.get_type (), lookup.get_props ());
	}

	}
	તોડો;

      સ્થિતિ GDEF::tableTag:
	{

	અચલ GDEF &gdef = *reinterpret_cast<અચલ GDEF *> (font_data + table.offset);

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
	તોડો;
	}
      }
    }
  }
}
/* end of private API use */
#endif

પૂર્ણાંક
મુખ્ય (પૂર્ણાંક argc, અક્ષર **argv)
{
  જો (argc != 2)
  {
    fprintf (stderr, "usage: %s font-file.ttf\n\n"
		     "This tools is unsupported and crashes on bad data.\nDon't use it.\n", argv[0]);
    exit (1);
  }

  hb_blob_t *blob = hb_blob_create_from_file_or_fail (argv[1]);
  પ્રતિજ્ઞા (blob);
  printf ("Opened font file %s: %u bytes long\n", argv[1], hb_blob_get_length (blob));
#ifndef MAIN_CC_NO_PRIVATE_API
  print_layout_info_using_private_api (blob);
#endif
#જો !defined(HB_NO_COLOR) && !defined(HB_NO_DRAW)
  dump_glyphs (blob, argv[1]);
#endif
  hb_blob_destroy (blob);

  પાછા 0;
}
