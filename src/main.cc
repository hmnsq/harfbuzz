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
#अन्तर्भाव "config.h"
#endif

#अन्तर्भाव "hb.h"
#अन्तर्भाव "hb-ot.h"

#अन्तर्भाव <cassert>
#अन्तर्भाव <cstdlib>
#अन्तर्भाव <cstdio>
#अन्तर्भाव <cstring>

#ifdef HB_NO_OPEN
#define hb_blob_create_from_file_or_fail(x)  hb_blob_get_empty ()
#endif

#यदि !defined(HB_NO_COLOR) && !defined(HB_NO_DRAW)
#ifndef HB_NO_SVG
स्थिर शून्य
svg_dump (hb_face_t *face, अचिन्हित face_index)
{
  अचिन्हित glyph_गिनती = hb_face_get_glyph_गिनती (face);

  प्रति (अचिन्हित glyph_id = 0; glyph_id < glyph_गिनती; ++glyph_id)
  {
    hb_blob_t *blob = hb_ot_color_glyph_reference_svg (face, glyph_id);

    यदि (hb_blob_get_length (blob) == 0) अनुवर्त;

    अचिन्हित length;
    ध्रुव वर्ण *data = hb_blob_get_data (blob, &length);

    वर्ण output_path[255];
    snprintf (output_path, परिमाण output_path,
	     "out/svg-%u-%u.svg%s",
	     glyph_id,
	     face_index,
	     // append "z" if the content is gzipped, https://stackoverflow.com/a/6059405
	     (length > 2 && (data[0] == '\x1F') && (data[1] == '\x8B')) ? "z" : "");

    खाता *f = ख_खोलो (output_path, "wb");
    ख_डालो (data, 1, length, f);
    ख_बंद (f);

    hb_blob_destroy (blob);
  }
}
#endif

/* _png API is so easy to use unlike the below code, don't get confused */
स्थिर शून्य
png_dump (hb_face_t *face, अचिन्हित face_index)
{
  अचिन्हित glyph_गिनती = hb_face_get_glyph_गिनती (face);
  hb_font_t *font = hb_font_create (face);

  /* scans the font for strikes */
  अचिन्हित sample_glyph_id;
  /* we don't care about different strikes for different glyphs at this point */
  प्रति (sample_glyph_id = 0; sample_glyph_id < glyph_गिनती; ++sample_glyph_id)
  {
    hb_blob_t *blob = hb_ot_color_glyph_reference_png (font, sample_glyph_id);
    अचिन्हित blob_length = hb_blob_get_length (blob);
    hb_blob_destroy (blob);
    यदि (blob_length != 0)
      भङ्ग;
  }

  अचिन्हित upem = hb_face_get_upem (face);
  अचिन्हित blob_length = 0;
  अचिन्हित strike = 0;
  प्रति (अचिन्हित ppem = 1; ppem < upem; ++ppem)
  {
    hb_font_set_ppem (font, ppem, ppem);
    hb_blob_t *blob = hb_ot_color_glyph_reference_png (font, sample_glyph_id);
    अचिन्हित new_blob_length = hb_blob_get_length (blob);
    hb_blob_destroy (blob);
    यदि (new_blob_length != blob_length)
    {
      प्रति (अचिन्हित glyph_id = 0; glyph_id < glyph_गिनती; ++glyph_id)
      {
	hb_blob_t *blob = hb_ot_color_glyph_reference_png (font, glyph_id);

	यदि (hb_blob_get_length (blob) == 0) अनुवर्त;

	अचिन्हित length;
	ध्रुव वर्ण *data = hb_blob_get_data (blob, &length);

	वर्ण output_path[255];
	snprintf (output_path, परिमाण output_path, "out/png-%u-%u-%u.png", glyph_id, strike, face_index);

	खाता *f = ख_खोलो (output_path, "wb");
	ख_डालो (data, 1, length, f);
	ख_बंद (f);

	hb_blob_destroy (blob);
      }

      strike++;
      blob_length = new_blob_length;
    }
  }

  hb_font_destroy (font);
}

संरचना draw_data_t
{
  खाता *f;
  hb_position_t ascender;
};

स्थिर शून्य
move_to (hb_draw_funcs_t *, draw_data_t *draw_data,
	 hb_draw_state_t *,
	 भिन्न to_x, भिन्न to_y,
	 शून्य *)
{
  ख_लिखो (draw_data->f, "M%g,%g", to_x, draw_data->ascender - to_y);
}

स्थिर शून्य
पंक्ति_to (hb_draw_funcs_t *, draw_data_t *draw_data,
	 hb_draw_state_t *,
	 भिन्न to_x, भिन्न to_y,
	 शून्य *)
{
  ख_लिखो (draw_data->f, "L%g,%g", to_x, draw_data->ascender - to_y);
}

स्थिर शून्य
quadratic_to (hb_draw_funcs_t *, draw_data_t *draw_data,
	      hb_draw_state_t *,
	      भिन्न control_x, भिन्न control_y,
	      भिन्न to_x, भिन्न to_y,
	      शून्य *)
{
  ख_लिखो (draw_data->f, "Q%g,%g %g,%g", control_x, draw_data->ascender - control_y,
					to_x, draw_data->ascender - to_y);
}

स्थिर शून्य
cubic_to (hb_draw_funcs_t *, draw_data_t *draw_data,
	  hb_draw_state_t *,
	  भिन्न control1_x, भिन्न control1_y,
	  भिन्न control2_x, भिन्न control2_y,
	  भिन्न to_x, भिन्न to_y,
	  शून्य *)
{
  ख_लिखो (draw_data->f, "C%g,%g %g,%g %g,%g", control1_x, draw_data->ascender - control1_y,
					       control2_x, draw_data->ascender - control2_y,
					       to_x, draw_data->ascender - to_y);
}

स्थिर शून्य
close_path (hb_draw_funcs_t *, draw_data_t *draw_data,
	    hb_draw_state_t *,
	    शून्य *)
{
  ख_लिखो (draw_data->f, "Z");
}

स्थिर शून्य
layered_glyph_dump (hb_font_t *font, hb_draw_funcs_t *funcs, अचिन्हित face_index)
{
  hb_face_t *face = hb_font_get_face (font);
  अचिन्हित palette_गिनती = hb_ot_color_palette_get_गिनती (face);
  प्रति (अचिन्हित palette = 0; palette < palette_गिनती; ++palette)
  {
    अचिन्हित अंक_colors = hb_ot_color_palette_get_colors (face, palette, 0, nullptr, nullptr);
    यदि (!अंक_colors) अनुवर्त;

    hb_color_t *colors = (hb_color_t*) सुस्मृति (अंक_colors, परिमाण (hb_color_t));
    hb_ot_color_palette_get_colors (face, palette, 0, &अंक_colors, colors);
    यदि (!अंक_colors)
    {
      मुक्त (colors);
      अनुवर्त;
    }

    अचिन्हित अंक_glyphs = hb_face_get_glyph_गिनती (face);
    प्रति (hb_codepoint_t gid = 0; gid < अंक_glyphs; ++gid)
    {
      अचिन्हित अंक_layers = hb_ot_color_glyph_get_layers (face, gid, 0, nullptr, nullptr);
      यदि (!अंक_layers) अनुवर्त;

      hb_ot_color_layer_t *layers = (hb_ot_color_layer_t*) दो_स्मृति (अंक_layers * परिमाण (hb_ot_color_layer_t));

      hb_ot_color_glyph_get_layers (face, gid, 0, &अंक_layers, layers);
      यदि (अंक_layers)
      {
	hb_font_extents_t font_extents;
	hb_font_get_extents_for_direction (font, HB_DIRECTION_LTR, &font_extents);
	hb_glyph_extents_t extents = {0, 0, 0, 0};
	यदि (!hb_font_get_glyph_extents (font, gid, &extents))
	{
	  म_लिखो ("Skip gid: %u\n", gid);
	  अनुवर्त;
	}

	वर्ण output_path[255];
	snprintf (output_path, परिमाण output_path, "out/colr-%u-%u-%u.svg", gid, palette, face_index);
	खाता *f = ख_खोलो (output_path, "wb");
	ख_लिखो (f, "<svg xmlns=\"http://www.w3.org/2000/svg\""
		    " viewBox=\"%d %d %lld %lld\">\n",
		    extents.x_bearing, 0,
		    (दीर्घ दीर्घ) extents.x_bearing + extents.width,
		    -(दीर्घ दीर्घ) extents.height);
	draw_data_t draw_data;
	draw_data.ascender = extents.y_bearing;
	draw_data.f = f;

	प्रति (अचिन्हित layer = 0; layer < अंक_layers; ++layer)
	{
	  hb_color_t color = 0x000000FF;
	  यदि (layers[layer].color_index != 0xFFFF)
	    color = colors[layers[layer].color_index];
	  ख_लिखो (f, "<path fill=\"#%02X%02X%02X\" ",
		   hb_color_get_red (color), hb_color_get_green (color), hb_color_get_green (color));
	  यदि (hb_color_get_alpha (color) != 255)
	    ख_लिखो (f, "fill-opacity=\"%.3f\"", (द्विगुण) hb_color_get_alpha (color) / 255.);
	  ख_लिखो (f, "d=\"");
	  hb_font_draw_glyph (font, layers[layer].glyph, funcs, &draw_data);
	  ख_लिखो (f, "\"/>\n");
	}

	ख_लिखो (f, "</svg>");
	ख_बंद (f);
      }
      मुक्त (layers);
    }

    मुक्त (colors);
  }
}

स्थिर शून्य
dump_glyphs (hb_font_t *font, hb_draw_funcs_t *funcs, अचिन्हित face_index)
{
  अचिन्हित अंक_glyphs = hb_face_get_glyph_गिनती (hb_font_get_face (font));
  प्रति (अचिन्हित gid = 0; gid < अंक_glyphs; ++gid)
  {
    hb_font_extents_t font_extents;
    hb_font_get_extents_for_direction (font, HB_DIRECTION_LTR, &font_extents);
    hb_glyph_extents_t extents = {0, 0, 0, 0};
    यदि (!hb_font_get_glyph_extents (font, gid, &extents))
    {
      म_लिखो ("Skip gid: %u\n", gid);
      अनुवर्त;
    }

    वर्ण output_path[255];
    snprintf (output_path, परिमाण output_path, "out/%u-%u.svg", face_index, gid);
    खाता *f = ख_खोलो (output_path, "wb");
    ख_लिखो (f, "<svg xmlns=\"http://www.w3.org/2000/svg\""
		" viewBox=\"%d %d %lld %lld\"><path d=\"",
		extents.x_bearing, 0,
		(दीर्घ दीर्घ) extents.x_bearing + extents.width,
		(दीर्घ दीर्घ) font_extents.ascender - font_extents.descender);
    draw_data_t draw_data;
    draw_data.ascender = font_extents.ascender;
    draw_data.f = f;
    hb_font_draw_glyph (font, gid, funcs, &draw_data);
    ख_लिखो (f, "\"/></svg>");
    ख_बंद (f);
  }
}

स्थिर शून्य
dump_glyphs (hb_blob_t *blob, ध्रुव वर्ण *font_name)
{
  खाता *font_name_file = ख_खोलो ("out/.dumped_font_name", "r");
  यदि (font_name_file)
  {
    ख_लिखो (मानक_त्रुटि, "Purge or rename ./out folder if you like to run a glyph dump,\n"
		     "run it like `rm -rf out && mkdir out && src/main font-file.ttf`\n");
    प्रत्यागम;
  }

  font_name_file = ख_खोलो ("out/.dumped_font_name", "w");
  यदि (!font_name_file)
  {
    ख_लिखो (मानक_त्रुटि, "./out is not accessible as a folder, create it please\n");
    प्रत्यागम;
  }
  ख_डालो (font_name, 1, म_माप (font_name), font_name_file);
  ख_बंद (font_name_file);

  hb_draw_funcs_t *funcs = hb_draw_funcs_create ();
  hb_draw_funcs_set_move_to_func (funcs, (hb_draw_move_to_func_t) move_to, nullptr, nullptr);
  hb_draw_funcs_set_पंक्ति_to_func (funcs, (hb_draw_पंक्ति_to_func_t) पंक्ति_to, nullptr, nullptr);
  hb_draw_funcs_set_quadratic_to_func (funcs, (hb_draw_quadratic_to_func_t) quadratic_to, nullptr, nullptr);
  hb_draw_funcs_set_cubic_to_func (funcs, (hb_draw_cubic_to_func_t) cubic_to, nullptr, nullptr);
  hb_draw_funcs_set_close_path_func (funcs, (hb_draw_close_path_func_t) close_path, nullptr, nullptr);

  अचिन्हित अंक_faces = hb_face_गिनती (blob);
  प्रति (अचिन्हित face_index = 0; face_index < अंक_faces; ++face_index)
  {
    hb_face_t *face = hb_face_create (blob, face_index);
    hb_font_t *font = hb_font_create (face);

    यदि (hb_ot_color_has_png (face))
      म_लिखो ("Dumping png (CBDT/sbix)...\n");
    png_dump (face, face_index);

#ifndef HB_NO_SVG
    यदि (hb_ot_color_has_svg (face))
      म_लिखो ("Dumping svg (SVG )...\n");
    svg_dump (face, face_index);
#endif

    यदि (hb_ot_color_has_layers (face) && hb_ot_color_has_palettes (face))
      म_लिखो ("Dumping layered color glyphs (COLR/CPAL)...\n");
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
#अन्तर्भाव "hb-static.cc"
#अन्तर्भाव "hb-open-file.hh"
#अन्तर्भाव "hb-ot-layout-gdef-table.hh"
#अन्तर्भाव "hb-ot-layout-gsubgpos.hh"

using नामाकाश OT;

स्थिर शून्य
print_layout_info_using_private_api (hb_blob_t *blob)
{
  ध्रुव वर्ण *font_data = hb_blob_get_data (blob, nullptr);
  hb_blob_t *font_blob = hb_sanitize_context_t ().sanitize_blob<OpenTypeFontFile> (blob);
  ध्रुव OpenTypeFontFile* sanitized = font_blob->as<OpenTypeFontFile> ();
  यदि (!font_blob->data)
  {
    म_लिखो ("Sanitization of the file wasn't successful. Exit");
    निकास (1);
  }
  ध्रुव OpenTypeFontFile& ot = *sanitized;

  विकल्प (ot.get_tag ())
  {
  पक्ष OpenTypeFontFile::TrueTypeTag:
    म_लिखो ("OpenType font with TrueType outlines\n");
    भङ्ग;
  पक्ष OpenTypeFontFile::CFFTag:
    म_लिखो ("OpenType font with CFF (Type1) outlines\n");
    भङ्ग;
  पक्ष OpenTypeFontFile::TTCTag:
    म_लिखो ("TrueType Collection of OpenType fonts\n");
    भङ्ग;
  पक्ष OpenTypeFontFile::TrueTag:
    म_लिखो ("Obsolete Apple TrueType font\n");
    भङ्ग;
  पक्ष OpenTypeFontFile::Typ1Tag:
    म_लिखो ("Obsolete Apple Type1 font in SFNT container\n");
    भङ्ग;
  पक्ष OpenTypeFontFile::DFontTag:
    म_लिखो ("DFont Mac Resource Fork\n");
    भङ्ग;
  स्वभाव:
    म_लिखो ("Unknown font format\n");
    भङ्ग;
  }

  अचिन्हित अंक_faces = hb_face_गिनती (blob);
  म_लिखो ("%u font(s) found in file\n", अंक_faces);
  प्रति (अचिन्हित n_font = 0; n_font < अंक_faces; ++n_font)
  {
    ध्रुव OpenTypeFontFace &font = ot.get_face (n_font);
    म_लिखो ("Font %u of %u:\n", n_font, अंक_faces);

    अचिन्हित अंक_tables = font.get_table_गिनती ();
    म_लिखो ("  %u table(s) found in font\n", अंक_tables);
    प्रति (अचिन्हित n_table = 0; n_table < अंक_tables; ++n_table)
    {
      ध्रुव OpenTypeTable &table = font.get_table (n_table);
      म_लिखो ("  Table %2u of %2u: %.4s (0x%08x+0x%08x)\n", n_table, अंक_tables,
	      (ध्रुव वर्ण *) table.tag,
	      (अचिन्हित) table.offset,
	      (अचिन्हित) table.length);

      विकल्प (table.tag)
      {

      पक्ष HB_OT_TAG_GSUB:
      पक्ष HB_OT_TAG_GPOS:
	{

	ध्रुव GSUBGPOS &g = *reinterpret_cast<ध्रुव GSUBGPOS *> (font_data + table.offset);

	अचिन्हित अंक_scripts = g.get_script_गिनती ();
	म_लिखो ("    %u script(s) found in table\n", अंक_scripts);
	प्रति (अचिन्हित n_script = 0; n_script < अंक_scripts; ++n_script)
	{
	  ध्रुव Script &script = g.get_script (n_script);
	  म_लिखो ("    Script %2u of %2u: %.4s\n", n_script, अंक_scripts,
		  (ध्रुव वर्ण *) g.get_script_tag (n_script));

	  यदि (!script.has_default_lang_sys ())
	    म_लिखो ("      No default language system\n");
	  पूर्णाङ्क अंक_langsys = script.get_lang_sys_गिनती ();
	  म_लिखो ("      %d language system(s) found in script\n", अंक_langsys);
	  प्रति (पूर्णाङ्क n_langsys = script.has_default_lang_sys () ? -1 : 0; n_langsys < अंक_langsys; ++n_langsys)
	  {
	    ध्रुव LangSys &langsys = n_langsys == -1
				   ? script.get_default_lang_sys ()
				   : script.get_lang_sys (n_langsys);
	    यदि (n_langsys == -1)
	      म_लिखो ("      Default Language System\n");
	    अन्यथा
	      म_लिखो ("      Language System %2d of %2d: %.4s\n", n_langsys, अंक_langsys,
		      (ध्रुव वर्ण *) script.get_lang_sys_tag (n_langsys));
	    यदि (!langsys.has_required_feature ())
	      म_लिखो ("        No required feature\n");
	    अन्यथा
	      म_लिखो ("        Required feature index: %u\n",
		      langsys.get_required_feature_index ());

	    अचिन्हित अंक_features = langsys.get_feature_गिनती ();
	    म_लिखो ("        %u feature(s) found in language system\n", अंक_features);
	    प्रति (अचिन्हित n_feature = 0; n_feature < अंक_features; ++n_feature)
	    {
	      म_लिखो ("        Feature index %2u of %2u: %u\n", n_feature, अंक_features,
		      langsys.get_feature_index (n_feature));
	    }
	  }
	}

	अचिन्हित अंक_features = g.get_feature_गिनती ();
	म_लिखो ("    %u feature(s) found in table\n", अंक_features);
	प्रति (अचिन्हित n_feature = 0; n_feature < अंक_features; ++n_feature)
	{
	  ध्रुव Feature &feature = g.get_feature (n_feature);
	  अचिन्हित अंक_lookups = feature.get_lookup_गिनती ();
	  म_लिखो ("    Feature %2u of %2u: %c%c%c%c\n", n_feature, अंक_features,
		  HB_UNTAG (g.get_feature_tag (n_feature)));

	  म_लिखो ("        %u lookup(s) found in feature\n", अंक_lookups);
	  प्रति (अचिन्हित n_lookup = 0; n_lookup < अंक_lookups; ++n_lookup) {
	    म_लिखो ("        Lookup index %2u of %2u: %u\n", n_lookup, अंक_lookups,
		    feature.get_lookup_index (n_lookup));
	  }
	}

	अचिन्हित अंक_lookups = g.get_lookup_गिनती ();
	म_लिखो ("    %u lookup(s) found in table\n", अंक_lookups);
	प्रति (अचिन्हित n_lookup = 0; n_lookup < अंक_lookups; ++n_lookup)
	{
	  ध्रुव Lookup &lookup = g.get_lookup (n_lookup);
	  म_लिखो ("    Lookup %2u of %2u: type %u, props 0x%04X\n", n_lookup, अंक_lookups,
		  lookup.get_type (), lookup.get_props ());
	}

	}
	भङ्ग;

      पक्ष GDEF::tableTag:
	{

	ध्रुव GDEF &gdef = *reinterpret_cast<ध्रुव GDEF *> (font_data + table.offset);

	म_लिखो ("    Has %sglyph classes\n",
		  gdef.has_glyph_classes () ? "" : "no ");
	म_लिखो ("    Has %smark attachment types\n",
		  gdef.has_mark_attachment_types () ? "" : "no ");
	म_लिखो ("    Has %sattach list\n",
		  gdef.has_attach_list () ? "" : "no ");
	म_लिखो ("    Has %slig carets\n",
		  gdef.has_lig_carets () ? "" : "no ");
	म_लिखो ("    Has %smark glyph sets\n",
		  gdef.has_mark_glyph_sets () ? "" : "no ");
	भङ्ग;
	}
      }
    }
  }
}
/* end of private API use */
#endif

पूर्णाङ्क
main (पूर्णाङ्क argc, वर्ण **argv)
{
  यदि (argc != 2)
  {
    ख_लिखो (मानक_त्रुटि, "usage: %s font-file.ttf\n\n"
		     "This tools is unsupported and crashes on bad data.\nDon't use it.\n", argv[0]);
    निकास (1);
  }

  hb_blob_t *blob = hb_blob_create_from_file_or_fail (argv[1]);
  प्रतिज्ञा (blob);
  म_लिखो ("Opened font file %s: %u bytes long\n", argv[1], hb_blob_get_length (blob));
#ifndef MAIN_CC_NO_PRIVATE_API
  print_layout_info_using_private_api (blob);
#endif
#यदि !defined(HB_NO_COLOR) && !defined(HB_NO_DRAW)
  dump_glyphs (blob, argv[1]);
#endif
  hb_blob_destroy (blob);

  प्रत्यागम 0;
}
