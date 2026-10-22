/*
 * Copyright © 2009  Red Hat, Inc.
 * Copyright © 2012  Google, Inc.
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
 * Google Author(s): Behdad Esfahbod
 */

#అన్తర్భావ "hb.hh"

#అన్తర్భావ "hb-font.hh"
#అన్తర్భావ "hb-draw.hh"
#అన్తర్భావ "hb-paint.hh"
#అన్తర్భావ "hb-machinery.hh"

#అన్తర్భావ "hb-ot.h"

#అన్తర్భావ "hb-ot-var-avar-table.hh"
#అన్తర్భావ "hb-ot-var-fvar-table.hh"

#నిర్వచితం_కాకపోతే HB_NO_OT_FONT
#అన్తర్భావ "hb-ot.h"
#అయితే_ముగింపు
#నిర్వచితమైతే HAVE_FREETYPE
#అన్తర్భావ "hb-ft.h"
#అయితే_ముగింపు
#నిర్వచితమైతే HAVE_FONTATIONS
#అన్తర్భావ "hb-fontations.h"
#అయితే_ముగింపు
#నిర్వచితమైతే HAVE_CORETEXT
#అన్తర్భావ "hb-coretext.h"
#అయితే_ముగింపు
#నిర్వచితమైతే HAVE_DIRECTWRITE
#అన్తర్భావ "hb-directwrite.h"
#అయితే_ముగింపు


/**
 * SECTION:hb-font
 * @title: hb-font
 * @short_description: Font objects
 * @include: hb.h
 *
 * Functions for working with font objects.
 *
 * A font object represents a font face at a specific size and with
 * certain other parameters (pixels-per-em, points-per-em, variation
 * settings) specified. Font objects are created from font face
 * objects, and are used as input to hb_shape(), among other things.
 *
 * Client programs can optionally pass in their own functions that
 * implement the basic, lower-level queries of font objects. This set
 * of font functions is defined by the virtual methods in
 * #hb_font_funcs_t.
 *
 * HarfBuzz provides a built-in set of lightweight default
 * functions for each method in #hb_font_funcs_t.
 *
 * The default font functions are implemented in terms of the
 * #hb_font_funcs_t methods of the parent font object.  This allows
 * client programs to override only the methods they need to, and
 * otherwise inherit the parent font's implementation, if any.
 **/


/*
 * hb_font_funcs_t
 */

స్థిర hb_bool_t
hb_font_get_font_h_extents_nil (hb_font_t         *font HB_వాడని,
				శూన్య              *font_దత్తాంశం HB_వాడని,
				hb_font_extents_t *extents,
				శూన్య              *user_దత్తాంశం HB_వాడని)
{
  hb_memset (extents, 0, పరిమాణం (*extents));
  తిరిగి అబద్ధం;
}

స్థిర hb_bool_t
hb_font_get_font_h_extents_default (hb_font_t         *font,
				    శూన్య              *font_దత్తాంశం HB_వాడని,
				    hb_font_extents_t *extents,
				    శూన్య              *user_దత్తాంశం HB_వాడని)
{
  hb_bool_t ret = font->మాతృ->get_font_h_extents (extents, అబద్ధం);
  ఒకవేళ (ret) {
    extents->ascender = font->మాతృ_scale_y_దూరం (extents->ascender);
    extents->descender = font->మాతృ_scale_y_దూరం (extents->descender);
    extents->పంక్తి_gap = font->మాతృ_scale_y_దూరం (extents->పంక్తి_gap);
  }
  తిరిగి ret;
}

స్థిర hb_bool_t
hb_font_get_font_v_extents_nil (hb_font_t         *font HB_వాడని,
				శూన్య              *font_దత్తాంశం HB_వాడని,
				hb_font_extents_t *extents,
				శూన్య              *user_దత్తాంశం HB_వాడని)
{
  hb_memset (extents, 0, పరిమాణం (*extents));
  తిరిగి అబద్ధం;
}

స్థిర hb_bool_t
hb_font_get_font_v_extents_default (hb_font_t         *font,
				    శూన్య              *font_దత్తాంశం HB_వాడని,
				    hb_font_extents_t *extents,
				    శూన్య              *user_దత్తాంశం HB_వాడని)
{
  hb_bool_t ret = font->మాతృ->get_font_v_extents (extents, అబద్ధం);
  ఒకవేళ (ret) {
    extents->ascender = font->మాతృ_scale_x_దూరం (extents->ascender);
    extents->descender = font->మాతృ_scale_x_దూరం (extents->descender);
    extents->పంక్తి_gap = font->మాతృ_scale_x_దూరం (extents->పంక్తి_gap);
  }
  తిరిగి ret;
}

స్థిర hb_bool_t
hb_font_get_nominal_glyph_nil (hb_font_t      *font HB_వాడని,
			       శూన్య           *font_దత్తాంశం HB_వాడని,
			       hb_codepoint_t  unicode HB_వాడని,
			       hb_codepoint_t *glyph,
			       శూన్య           *user_దత్తాంశం HB_వాడని)
{
  *glyph = 0;
  తిరిగి అబద్ధం;
}

స్థిర hb_bool_t
hb_font_get_nominal_glyph_default (hb_font_t      *font,
				   శూన్య           *font_దత్తాంశం HB_వాడని,
				   hb_codepoint_t  unicode,
				   hb_codepoint_t *glyph,
				   శూన్య           *user_దత్తాంశం HB_వాడని)
{
  ఒకవేళ (font->has_nominal_glyphs_func_సమితి ())
  {
    తిరిగి font->get_nominal_glyphs (1, &unicode, 0, glyph, 0);
  }
  తిరిగి font->మాతృ->get_nominal_glyph (unicode, glyph);
}

#నిర్వచించు hb_font_get_nominal_glyphs_nil hb_font_get_nominal_glyphs_default

స్థిర గుర్తులేని పూర్ణాంకం
hb_font_get_nominal_glyphs_default (hb_font_t            *font,
				    శూన్య                 *font_దత్తాంశం HB_వాడని,
				    గుర్తులేని పూర్ణాంకం          లెక్క,
				    స్థిరరాశి hb_codepoint_t *మొదటి_unicode,
				    గుర్తులేని పూర్ణాంకం          unicode_stride,
				    hb_codepoint_t       *మొదటి_glyph,
				    గుర్తులేని పూర్ణాంకం          glyph_stride,
				    శూన్య                 *user_దత్తాంశం HB_వాడని)
{
  ఒకవేళ (font->has_nominal_glyph_func_సమితి ())
  {
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++)
    {
      ఒకవేళ (!font->get_nominal_glyph (*మొదటి_unicode, మొదటి_glyph))
	తిరిగి i;

      మొదటి_unicode = &Struct_At_స్థానభ్రంశం_Unaligned<hb_codepoint_t> (మొదటి_unicode, unicode_stride);
      మొదటి_glyph = &Struct_At_స్థానభ్రంశం_Unaligned<hb_codepoint_t> (మొదటి_glyph, glyph_stride);
    }
    తిరిగి లెక్క;
  }

  తిరిగి font->మాతృ->get_nominal_glyphs (లెక్క,
					   మొదటి_unicode, unicode_stride,
					   మొదటి_glyph, glyph_stride);
}

స్థిర hb_bool_t
hb_font_get_variation_glyph_nil (hb_font_t      *font HB_వాడని,
				 శూన్య           *font_దత్తాంశం HB_వాడని,
				 hb_codepoint_t  unicode HB_వాడని,
				 hb_codepoint_t  variation_selector HB_వాడని,
				 hb_codepoint_t *glyph,
				 శూన్య           *user_దత్తాంశం HB_వాడని)
{
  *glyph = 0;
  తిరిగి అబద్ధం;
}

స్థిర hb_bool_t
hb_font_get_variation_glyph_default (hb_font_t      *font,
				     శూన్య           *font_దత్తాంశం HB_వాడని,
				     hb_codepoint_t  unicode,
				     hb_codepoint_t  variation_selector,
				     hb_codepoint_t *glyph,
				     శూన్య           *user_దత్తాంశం HB_వాడని)
{
  తిరిగి font->మాతృ->get_variation_glyph (unicode, variation_selector, glyph);
}


స్థిర hb_position_t
hb_font_get_glyph_h_advance_nil (hb_font_t      *font,
				 శూన్య           *font_దత్తాంశం HB_వాడని,
				 hb_codepoint_t  glyph HB_వాడని,
				 శూన్య           *user_దత్తాంశం HB_వాడని)
{
  తిరిగి font->x_scale;
}

స్థిర hb_position_t
hb_font_get_glyph_h_advance_default (hb_font_t      *font,
				     శూన్య           *font_దత్తాంశం HB_వాడని,
				     hb_codepoint_t  glyph,
				     శూన్య           *user_దత్తాంశం HB_వాడని)
{
  ఒకవేళ (font->has_glyph_h_advances_func_సమితి ())
  {
    hb_position_t ret;
    font->get_glyph_h_advances (1, &glyph, 0, &ret, 0, అబద్ధం);
    తిరిగి ret;
  }
  తిరిగి font->మాతృ_scale_x_దూరం (font->మాతృ->get_glyph_h_advance (glyph, అబద్ధం));
}

స్థిర hb_position_t
hb_font_get_glyph_v_advance_nil (hb_font_t      *font,
				 శూన్య           *font_దత్తాంశం HB_వాడని,
				 hb_codepoint_t  glyph HB_వాడని,
				 శూన్య           *user_దత్తాంశం HB_వాడని)
{
  తిరిగి -font->y_scale;
}

స్థిర hb_position_t
hb_font_get_glyph_v_advance_default (hb_font_t      *font,
				     శూన్య           *font_దత్తాంశం HB_వాడని,
				     hb_codepoint_t  glyph,
				     శూన్య           *user_దత్తాంశం HB_వాడని)
{
  ఒకవేళ (font->has_glyph_v_advances_func_సమితి ())
  {
    hb_position_t ret;
    font->get_glyph_v_advances (1, &glyph, 0, &ret, 0, అబద్ధం);
    తిరిగి ret;
  }
  తిరిగి font->మాతృ_scale_y_దూరం (font->మాతృ->get_glyph_v_advance (glyph, అబద్ధం));
}

#నిర్వచించు hb_font_get_glyph_h_advances_nil hb_font_get_glyph_h_advances_default

స్థిర శూన్య
hb_font_get_glyph_h_advances_default (hb_font_t*            font,
				      శూన్య*                 font_దత్తాంశం HB_వాడని,
				      గుర్తులేని పూర్ణాంకం          లెక్క,
				      స్థిరరాశి hb_codepoint_t *మొదటి_glyph,
				      గుర్తులేని పూర్ణాంకం          glyph_stride,
				      hb_position_t        *మొదటి_advance,
				      గుర్తులేని పూర్ణాంకం          advance_stride,
				      శూన్య                 *user_దత్తాంశం HB_వాడని)
{
  ఒకవేళ (font->has_glyph_h_advance_func_సమితి ())
  {
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++)
    {
      *మొదటి_advance = font->get_glyph_h_advance (*మొదటి_glyph, అబద్ధం);
      మొదటి_glyph = &Struct_At_స్థానభ్రంశం_Unaligned<hb_codepoint_t> (మొదటి_glyph, glyph_stride);
      మొదటి_advance = &Struct_At_స్థానభ్రంశం_Unaligned<hb_position_t> (మొదటి_advance, advance_stride);
    }
    తిరిగి;
  }

  font->మాతృ->get_glyph_h_advances (లెక్క,
				      మొదటి_glyph, glyph_stride,
				      మొదటి_advance, advance_stride,
				      అబద్ధం);
  కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++)
  {
    *మొదటి_advance = font->మాతృ_scale_x_దూరం (*మొదటి_advance);
    మొదటి_advance = &Struct_At_స్థానభ్రంశం_Unaligned<hb_position_t> (మొదటి_advance, advance_stride);
  }
}

#నిర్వచించు hb_font_get_glyph_v_advances_nil hb_font_get_glyph_v_advances_default
స్థిర శూన్య
hb_font_get_glyph_v_advances_default (hb_font_t*            font,
				      శూన్య*                 font_దత్తాంశం HB_వాడని,
				      గుర్తులేని పూర్ణాంకం          లెక్క,
				      స్థిరరాశి hb_codepoint_t *మొదటి_glyph,
				      గుర్తులేని పూర్ణాంకం          glyph_stride,
				      hb_position_t        *మొదటి_advance,
				      గుర్తులేని పూర్ణాంకం          advance_stride,
				      శూన్య                 *user_దత్తాంశం HB_వాడని)
{
  ఒకవేళ (font->has_glyph_v_advance_func_సమితి ())
  {
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++)
    {
      *మొదటి_advance = font->get_glyph_v_advance (*మొదటి_glyph, అబద్ధం);
      మొదటి_glyph = &Struct_At_స్థానభ్రంశం_Unaligned<hb_codepoint_t> (మొదటి_glyph, glyph_stride);
      మొదటి_advance = &Struct_At_స్థానభ్రంశం_Unaligned<hb_position_t> (మొదటి_advance, advance_stride);
    }
    తిరిగి;
  }

  font->మాతృ->get_glyph_v_advances (లెక్క,
				      మొదటి_glyph, glyph_stride,
				      మొదటి_advance, advance_stride,
				      అబద్ధం);
  కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++)
  {
    *మొదటి_advance = font->మాతృ_scale_y_దూరం (*మొదటి_advance);
    మొదటి_advance = &Struct_At_స్థానభ్రంశం_Unaligned<hb_position_t> (మొదటి_advance, advance_stride);
  }
}

స్థిర hb_bool_t
hb_font_get_glyph_h_origin_nil (hb_font_t      *font HB_వాడని,
				శూన్య           *font_దత్తాంశం HB_వాడని,
				hb_codepoint_t  glyph HB_వాడని,
				hb_position_t  *x,
				hb_position_t  *y,
				శూన్య           *user_దత్తాంశం HB_వాడని)
{
  *x = *y = 0;
  తిరిగి నిజం;
}

స్థిర hb_bool_t
hb_font_get_glyph_h_origin_default (hb_font_t      *font,
				    శూన్య           *font_దత్తాంశం HB_వాడని,
				    hb_codepoint_t  glyph,
				    hb_position_t  *x,
				    hb_position_t  *y,
				    శూన్య           *user_దత్తాంశం HB_వాడని)
{
  ఒకవేళ (font->has_glyph_h_origins_func_సమితి ())
  {
    తిరిగి font->get_glyph_h_origins (1, &glyph, 0, x, 0, y, 0, అబద్ధం);
  }
  hb_bool_t ret = font->మాతృ->get_glyph_h_origin (glyph, x, y);
  ఒకవేళ (ret)
    font->మాతృ_scale_position (x, y);
  తిరిగి ret;
}

స్థిర hb_bool_t
hb_font_get_glyph_v_origin_nil (hb_font_t      *font HB_వాడని,
				శూన్య           *font_దత్తాంశం HB_వాడని,
				hb_codepoint_t  glyph HB_వాడని,
				hb_position_t  *x,
				hb_position_t  *y,
				శూన్య           *user_దత్తాంశం HB_వాడని)
{
  తిరిగి అబద్ధం;
}

స్థిర hb_bool_t
hb_font_get_glyph_v_origin_default (hb_font_t      *font,
				    శూన్య           *font_దత్తాంశం HB_వాడని,
				    hb_codepoint_t  glyph,
				    hb_position_t  *x,
				    hb_position_t  *y,
				    శూన్య           *user_దత్తాంశం HB_వాడని)
{
  ఒకవేళ (font->has_glyph_v_origins_func_సమితి ())
  {
    తిరిగి font->get_glyph_v_origins (1, &glyph, 0, x, 0, y, 0, అబద్ధం);
  }
  hb_bool_t ret = font->మాతృ->get_glyph_v_origin (glyph, x, y);
  ఒకవేళ (ret)
    font->మాతృ_scale_position (x, y);
  తిరిగి ret;
}

#నిర్వచించు hb_font_get_glyph_h_origins_nil hb_font_get_glyph_h_origins_default

స్థిర hb_bool_t
hb_font_get_glyph_h_origins_default (hb_font_t *font HB_వాడని,
				     శూన్య *font_దత్తాంశం HB_వాడని,
				     గుర్తులేని పూర్ణాంకం లెక్క,
				     స్థిరరాశి hb_codepoint_t *మొదటి_glyph HB_వాడని,
				     గుర్తులేని glyph_stride HB_వాడని,
				     hb_position_t *మొదటి_x,
				     గుర్తులేని x_stride,
				     hb_position_t *మొదటి_y,
				     గుర్తులేని y_stride,
				     శూన్య *user_దత్తాంశం HB_వాడని)
{
  ఒకవేళ (font->has_glyph_h_origin_func_సమితి ())
  {
    hb_bool_t ret = నిజం;
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++)
    {
      ret &= font->get_glyph_h_origin (*మొదటి_glyph, మొదటి_x, మొదటి_y, అబద్ధం);
      మొదటి_glyph = &Struct_At_స్థానభ్రంశం_Unaligned<hb_codepoint_t> (మొదటి_glyph, glyph_stride);
      మొదటి_x = &Struct_At_స్థానభ్రంశం_Unaligned<hb_position_t> (మొదటి_x, x_stride);
      మొదటి_y = &Struct_At_స్థానభ్రంశం_Unaligned<hb_position_t> (మొదటి_y, y_stride);
    }
    తిరిగి ret;
  }

  hb_bool_t ret = font->మాతృ->get_glyph_h_origins (లెక్క,
						     మొదటి_glyph, glyph_stride,
						     మొదటి_x, x_stride,
						     మొదటి_y, y_stride);
  ఒకవేళ (ret)
  {
    కొరకు (గుర్తులేని i = 0; i < లెక్క; i++)
    {
      font->మాతృ_scale_position (మొదటి_x, మొదటి_y);
      మొదటి_x = &Struct_At_స్థానభ్రంశం_Unaligned<hb_position_t> (మొదటి_x, x_stride);
      మొదటి_y = &Struct_At_స్థానభ్రంశం_Unaligned<hb_position_t> (మొదటి_y, y_stride);
    }
  }
  తిరిగి ret;
}

#నిర్వచించు hb_font_get_glyph_v_origins_nil hb_font_get_glyph_v_origins_default

స్థిర hb_bool_t
hb_font_get_glyph_v_origins_default (hb_font_t *font HB_వాడని,
				     శూన్య *font_దత్తాంశం HB_వాడని,
				     గుర్తులేని పూర్ణాంకం లెక్క,
				     స్థిరరాశి hb_codepoint_t *మొదటి_glyph HB_వాడని,
				     గుర్తులేని glyph_stride HB_వాడని,
				     hb_position_t *మొదటి_x,
				     గుర్తులేని x_stride,
				     hb_position_t *మొదటి_y,
				     గుర్తులేని y_stride,
				     శూన్య *user_దత్తాంశం HB_వాడని)
{
  ఒకవేళ (font->has_glyph_v_origin_func_సమితి ())
  {
    hb_bool_t ret = నిజం;
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++)
    {
      ret &= font->get_glyph_v_origin (*మొదటి_glyph, మొదటి_x, మొదటి_y, అబద్ధం);
      మొదటి_glyph = &Struct_At_స్థానభ్రంశం_Unaligned<hb_codepoint_t> (మొదటి_glyph, glyph_stride);
      మొదటి_x = &Struct_At_స్థానభ్రంశం_Unaligned<hb_position_t> (మొదటి_x, x_stride);
      మొదటి_y = &Struct_At_స్థానభ్రంశం_Unaligned<hb_position_t> (మొదటి_y, y_stride);
    }
    తిరిగి ret;
  }

  hb_bool_t ret = font->మాతృ->get_glyph_v_origins (లెక్క,
						     మొదటి_glyph, glyph_stride,
						     మొదటి_x, x_stride,
						     మొదటి_y, y_stride);
  ఒకవేళ (ret)
  {
    కొరకు (గుర్తులేని i = 0; i < లెక్క; i++)
    {
      font->మాతృ_scale_position (మొదటి_x, మొదటి_y);
      మొదటి_x = &Struct_At_స్థానభ్రంశం_Unaligned<hb_position_t> (మొదటి_x, x_stride);
      మొదటి_y = &Struct_At_స్థానభ్రంశం_Unaligned<hb_position_t> (మొదటి_y, y_stride);
    }
  }
  తిరిగి ret;
}

స్థిర hb_position_t
hb_font_get_glyph_h_kerning_nil (hb_font_t      *font HB_వాడని,
				 శూన్య           *font_దత్తాంశం HB_వాడని,
				 hb_codepoint_t  ఎడమ_glyph HB_వాడని,
				 hb_codepoint_t  కుడి_glyph HB_వాడని,
				 శూన్య           *user_దత్తాంశం HB_వాడని)
{
  తిరిగి 0;
}

స్థిర hb_position_t
hb_font_get_glyph_h_kerning_default (hb_font_t      *font,
				     శూన్య           *font_దత్తాంశం HB_వాడని,
				     hb_codepoint_t  ఎడమ_glyph,
				     hb_codepoint_t  కుడి_glyph,
				     శూన్య           *user_దత్తాంశం HB_వాడని)
{
  తిరిగి font->మాతృ_scale_x_దూరం (font->మాతృ->get_glyph_h_kerning (ఎడమ_glyph, కుడి_glyph));
}

#నిర్వచితం_కాకపోతే HB_DISABLE_DEPRECATED
స్థిర hb_position_t
hb_font_get_glyph_v_kerning_nil (hb_font_t      *font HB_వాడని,
				 శూన్య           *font_దత్తాంశం HB_వాడని,
				 hb_codepoint_t  శిఖరం_glyph HB_వాడని,
				 hb_codepoint_t  bottom_glyph HB_వాడని,
				 శూన్య           *user_దత్తాంశం HB_వాడని)
{
  తిరిగి 0;
}

స్థిర hb_position_t
hb_font_get_glyph_v_kerning_default (hb_font_t      *font,
				     శూన్య           *font_దత్తాంశం HB_వాడని,
				     hb_codepoint_t  శిఖరం_glyph,
				     hb_codepoint_t  bottom_glyph,
				     శూన్య           *user_దత్తాంశం HB_వాడని)
{
  తిరిగి font->మాతృ_scale_y_దూరం (font->మాతృ->get_glyph_v_kerning (శిఖరం_glyph, bottom_glyph));
}
#అయితే_ముగింపు

స్థిర hb_bool_t
hb_font_get_glyph_extents_nil (hb_font_t          *font HB_వాడని,
			       శూన్య               *font_దత్తాంశం HB_వాడని,
			       hb_codepoint_t      glyph HB_వాడని,
			       hb_glyph_extents_t *extents,
			       శూన్య               *user_దత్తాంశం HB_వాడని)
{
  hb_memset (extents, 0, పరిమాణం (*extents));
  తిరిగి అబద్ధం;
}

స్థిర hb_bool_t
hb_font_get_glyph_extents_default (hb_font_t          *font,
				   శూన్య               *font_దత్తాంశం HB_వాడని,
				   hb_codepoint_t      glyph,
				   hb_glyph_extents_t *extents,
				   శూన్య               *user_దత్తాంశం HB_వాడని)
{
  hb_bool_t ret = font->మాతృ->get_glyph_extents (glyph, extents, అబద్ధం);
  ఒకవేళ (ret) {
    font->మాతృ_scale_position (&extents->x_bearing, &extents->y_bearing);
    font->మాతృ_scale_దూరం (&extents->వెడల్పు, &extents->ఎత్తు);
  }
  తిరిగి ret;
}

స్థిర hb_bool_t
hb_font_get_glyph_contour_బిందువు_nil (hb_font_t      *font HB_వాడని,
				     శూన్య           *font_దత్తాంశం HB_వాడని,
				     hb_codepoint_t  glyph HB_వాడని,
				     గుర్తులేని పూర్ణాంకం    బిందువు_సూచీ HB_వాడని,
				     hb_position_t  *x,
				     hb_position_t  *y,
				     శూన్య           *user_దత్తాంశం HB_వాడని)
{
  *x = *y = 0;
  తిరిగి అబద్ధం;
}

స్థిర hb_bool_t
hb_font_get_glyph_contour_బిందువు_default (hb_font_t      *font,
					 శూన్య           *font_దత్తాంశం HB_వాడని,
					 hb_codepoint_t  glyph,
					 గుర్తులేని పూర్ణాంకం    బిందువు_సూచీ,
					 hb_position_t  *x,
					 hb_position_t  *y,
					 శూన్య           *user_దత్తాంశం HB_వాడని)
{
  hb_bool_t ret = font->మాతృ->get_glyph_contour_బిందువు (glyph, బిందువు_సూచీ, x, y, అబద్ధం);
  ఒకవేళ (ret)
    font->మాతృ_scale_position (x, y);
  తిరిగి ret;
}

స్థిర hb_bool_t
hb_font_get_glyph_name_nil (hb_font_t      *font HB_వాడని,
			    శూన్య           *font_దత్తాంశం HB_వాడని,
			    hb_codepoint_t  glyph HB_వాడని,
			    అక్షరం           *name,
			    గుర్తులేని పూర్ణాంకం    కొలత,
			    శూన్య           *user_దత్తాంశం HB_వాడని)
{
  ఒకవేళ (కొలత) *name = '\0';
  తిరిగి అబద్ధం;
}

స్థిర hb_bool_t
hb_font_get_glyph_name_default (hb_font_t      *font,
				శూన్య           *font_దత్తాంశం HB_వాడని,
				hb_codepoint_t  glyph,
				అక్షరం           *name,
				గుర్తులేని పూర్ణాంకం    కొలత,
				శూన్య           *user_దత్తాంశం HB_వాడని)
{
  తిరిగి font->మాతృ->get_glyph_name (glyph, name, కొలత);
}

స్థిర hb_bool_t
hb_font_get_glyph_from_name_nil (hb_font_t      *font HB_వాడని,
				 శూన్య           *font_దత్తాంశం HB_వాడని,
				 స్థిరరాశి అక్షరం     *name HB_వాడని,
				 పూర్ణాంకం             len HB_వాడని, /* -1 means nul-terminated */
				 hb_codepoint_t *glyph,
				 శూన్య           *user_దత్తాంశం HB_వాడని)
{
  *glyph = 0;
  తిరిగి అబద్ధం;
}

స్థిర hb_bool_t
hb_font_get_glyph_from_name_default (hb_font_t      *font,
				     శూన్య           *font_దత్తాంశం HB_వాడని,
				     స్థిరరాశి అక్షరం     *name,
				     పూర్ణాంకం             len, /* -1 means nul-terminated */
				     hb_codepoint_t *glyph,
				     శూన్య           *user_దత్తాంశం HB_వాడని)
{
  తిరిగి font->మాతృ->get_glyph_from_name (name, len, glyph);
}

స్థిర hb_bool_t
hb_font_draw_glyph_or_fail_nil (hb_font_t       *font HB_వాడని,
				శూన్య            *font_దత్తాంశం HB_వాడని,
				hb_codepoint_t   glyph,
				hb_draw_funcs_t *draw_funcs,
				శూన్య            *draw_దత్తాంశం,
				శూన్య            *user_దత్తాంశం HB_వాడని)
{
  తిరిగి అబద్ధం;
}

స్థిర hb_bool_t
hb_font_paint_glyph_or_fail_nil (hb_font_t *font HB_వాడని,
				 శూన్య *font_దత్తాంశం HB_వాడని,
				 hb_codepoint_t glyph HB_వాడని,
				 hb_paint_funcs_t *paint_funcs HB_వాడని,
				 శూన్య *paint_దత్తాంశం HB_వాడని,
				 గుర్తులేని పూర్ణాంకం palette HB_వాడని,
				 hb_color_t foreground HB_వాడని,
				 శూన్య *user_దత్తాంశం HB_వాడని)
{
  తిరిగి అబద్ధం;
}

రకనిర్వచనం నిర్మాణం hb_font_draw_glyph_default_adaptor_t {
  hb_draw_funcs_t *draw_funcs;
  శూన్య		  *draw_దత్తాంశం;
  దశాంశం		   x_scale;
  దశాంశం		   y_scale;
} hb_font_draw_glyph_default_adaptor_t;

స్థిర శూన్య
hb_draw_move_గమ్యం_default (hb_draw_funcs_t *dfuncs HB_వాడని,
			 శూన్య *draw_దత్తాంశం,
			 hb_draw_state_t *st,
			 దశాంశం గమ్యం_x, దశాంశం గమ్యం_y,
			 శూన్య *user_దత్తాంశం HB_వాడని)
{
  hb_font_draw_glyph_default_adaptor_t *adaptor = (hb_font_draw_glyph_default_adaptor_t *) draw_దత్తాంశం;
  దశాంశం x_scale = adaptor->x_scale;
  దశాంశం y_scale = adaptor->y_scale;

  adaptor->draw_funcs->emit_move_గమ్యం (adaptor->draw_దత్తాంశం, *st,
				     x_scale * గమ్యం_x, y_scale * గమ్యం_y);
}

స్థిర శూన్య
hb_draw_పంక్తి_గమ్యం_default (hb_draw_funcs_t *dfuncs HB_వాడని, శూన్య *draw_దత్తాంశం,
			 hb_draw_state_t *st,
			 దశాంశం గమ్యం_x, దశాంశం గమ్యం_y,
			 శూన్య *user_దత్తాంశం HB_వాడని)
{
  hb_font_draw_glyph_default_adaptor_t *adaptor = (hb_font_draw_glyph_default_adaptor_t *) draw_దత్తాంశం;
  దశాంశం x_scale = adaptor->x_scale;
  దశాంశం y_scale = adaptor->y_scale;

  st->current_x = st->current_x * x_scale;
  st->current_y = st->current_y * y_scale;

  adaptor->draw_funcs->emit_పంక్తి_గమ్యం (adaptor->draw_దత్తాంశం, *st,
				     x_scale * గమ్యం_x, y_scale * గమ్యం_y);
}

స్థిర శూన్య
hb_draw_quadratic_గమ్యం_default (hb_draw_funcs_t *dfuncs HB_వాడని, శూన్య *draw_దత్తాంశం,
			      hb_draw_state_t *st,
			      దశాంశం control_x, దశాంశం control_y,
			      దశాంశం గమ్యం_x, దశాంశం గమ్యం_y,
			      శూన్య *user_దత్తాంశం HB_వాడని)
{
  hb_font_draw_glyph_default_adaptor_t *adaptor = (hb_font_draw_glyph_default_adaptor_t *) draw_దత్తాంశం;
  దశాంశం x_scale = adaptor->x_scale;
  దశాంశం y_scale = adaptor->y_scale;

  st->current_x = st->current_x * x_scale;
  st->current_y = st->current_y * y_scale;

  adaptor->draw_funcs->emit_quadratic_గమ్యం (adaptor->draw_దత్తాంశం, *st,
					  x_scale * control_x, y_scale * control_y,
					  x_scale * గమ్యం_x, y_scale * గమ్యం_y);
}

స్థిర శూన్య
hb_draw_cubic_గమ్యం_default (hb_draw_funcs_t *dfuncs HB_వాడని, శూన్య *draw_దత్తాంశం,
			  hb_draw_state_t *st,
			  దశాంశం control1_x, దశాంశం control1_y,
			  దశాంశం control2_x, దశాంశం control2_y,
			  దశాంశం గమ్యం_x, దశాంశం గమ్యం_y,
			  శూన్య *user_దత్తాంశం HB_వాడని)
{
  hb_font_draw_glyph_default_adaptor_t *adaptor = (hb_font_draw_glyph_default_adaptor_t *) draw_దత్తాంశం;
  దశాంశం x_scale = adaptor->x_scale;
  దశాంశం y_scale = adaptor->y_scale;

  st->current_x = st->current_x * x_scale;
  st->current_y = st->current_y * y_scale;

  adaptor->draw_funcs->emit_cubic_గమ్యం (adaptor->draw_దత్తాంశం, *st,
				      x_scale * control1_x, y_scale * control1_y,
				      x_scale * control2_x, y_scale * control2_y,
				      x_scale * గమ్యం_x, y_scale * గమ్యం_y);
}

స్థిర శూన్య
hb_draw_close_మార్గం_default (hb_draw_funcs_t *dfuncs HB_వాడని, శూన్య *draw_దత్తాంశం,
			    hb_draw_state_t *st,
			    శూన్య *user_దత్తాంశం HB_వాడని)
{
  hb_font_draw_glyph_default_adaptor_t *adaptor = (hb_font_draw_glyph_default_adaptor_t *) draw_దత్తాంశం;

  adaptor->draw_funcs->emit_close_మార్గం (adaptor->draw_దత్తాంశం, *st);
}

స్థిర hb_bool_t
hb_draw_సమితి_budget_default (hb_draw_funcs_t *dfuncs HB_వాడని, శూన్య *draw_దత్తాంశం,
			    int64_t budget,
			    శూన్య *user_దత్తాంశం HB_వాడని)
{
  hb_font_draw_glyph_default_adaptor_t *adaptor = (hb_font_draw_glyph_default_adaptor_t *) draw_దత్తాంశం;
  తిరిగి adaptor->draw_funcs->సమితి_budget (adaptor->draw_దత్తాంశం, budget);
}

స్థిర int64_t
hb_draw_get_budget_default (hb_draw_funcs_t *dfuncs HB_వాడని, శూన్య *draw_దత్తాంశం,
			    శూన్య *user_దత్తాంశం HB_వాడని)
{
  hb_font_draw_glyph_default_adaptor_t *adaptor = (hb_font_draw_glyph_default_adaptor_t *) draw_దత్తాంశం;
  తిరిగి adaptor->draw_funcs->get_budget (adaptor->draw_దత్తాంశం);
}

స్థిర int64_t *
hb_draw_get_budget_మిగిలిన_default (hb_draw_funcs_t *dfuncs HB_వాడని, శూన్య *draw_దత్తాంశం,
				      శూన్య *user_దత్తాంశం HB_వాడని)
{
  hb_font_draw_glyph_default_adaptor_t *adaptor = (hb_font_draw_glyph_default_adaptor_t *) draw_దత్తాంశం;
  తిరిగి adaptor->draw_funcs->get_budget_మిగిలిన_ptr (adaptor->draw_దత్తాంశం);
}

స్థిర స్థిరరాశి hb_draw_funcs_t _hb_draw_funcs_default = {
  HB_OBJECT_శీర్షిక_STATIC,

  {
#నిర్వచించు HB_DRAW_FUNC_IMPLEMENT(name) hb_draw_##name##_default,
    HB_DRAW_FUNCS_IMPLEMENT_CALLBACKS
#నిర్వచనం_రద్దు HB_DRAW_FUNC_IMPLEMENT
  }
};

స్థిర hb_bool_t
hb_font_draw_glyph_or_fail_default (hb_font_t       *font,
				    శూన్య            *font_దత్తాంశం HB_వాడని,
				    hb_codepoint_t   glyph,
				    hb_draw_funcs_t *draw_funcs,
				    శూన్య            *draw_దత్తాంశం,
				    శూన్య            *user_దత్తాంశం HB_వాడని)
{
  hb_font_draw_glyph_default_adaptor_t adaptor = {
    draw_funcs,
    draw_దత్తాంశం,
    font->మాతృ->x_scale ? (దశాంశం) font->x_scale / (దశాంశం) font->మాతృ->x_scale : 0.f,
    font->మాతృ->y_scale ? (దశాంశం) font->y_scale / (దశాంశం) font->మాతృ->y_scale : 0.f
  };

  తిరిగి font->మాతృ->draw_glyph_or_fail (glyph,
					   const_cast<hb_draw_funcs_t *> (&_hb_draw_funcs_default),
					   &adaptor,
					   అబద్ధం);
}

స్థిర hb_bool_t
hb_font_paint_glyph_or_fail_default (hb_font_t *font,
				     శూన్య *font_దత్తాంశం,
				     hb_codepoint_t glyph,
				     hb_paint_funcs_t *paint_funcs,
				     శూన్య *paint_దత్తాంశం,
				     గుర్తులేని పూర్ణాంకం palette,
				     hb_color_t foreground,
				     శూన్య *user_దత్తాంశం)
{
  paint_funcs->నెట్టు_transform (paint_దత్తాంశం,
    font->మాతృ->x_scale ? (దశాంశం) font->x_scale / (దశాంశం) font->మాతృ->x_scale : 0, 0,
    0, font->మాతృ->y_scale ? (దశాంశం) font->y_scale / (దశాంశం) font->మాతృ->y_scale : 0,
    0, 0);

  తార్కిక ret = font->మాతృ->paint_glyph_or_fail (glyph, paint_funcs, paint_దత్తాంశం, palette, foreground);

  paint_funcs->తీయి_transform (paint_దత్తాంశం);

  తిరిగి ret;
}

DEFINE_NULL_INSTANCE (hb_font_funcs_t) =
{
  HB_OBJECT_శీర్షిక_STATIC,

  శూన్యసూచిక,
  శూన్యసూచిక,
  {
    {
#నిర్వచించు HB_FONT_FUNC_IMPLEMENT(get_,name) hb_font_##get_##name##_nil,
      HB_FONT_FUNCS_IMPLEMENT_CALLBACKS
#నిర్వచనం_రద్దు HB_FONT_FUNC_IMPLEMENT
    }
  }
};

స్థిర స్థిరరాశి hb_font_funcs_t _hb_font_funcs_default = {
  HB_OBJECT_శీర్షిక_STATIC,

  శూన్యసూచిక,
  శూన్యసూచిక,
  {
    {
#నిర్వచించు HB_FONT_FUNC_IMPLEMENT(get_,name) hb_font_##get_##name##_default,
      HB_FONT_FUNCS_IMPLEMENT_CALLBACKS
#నిర్వచనం_రద్దు HB_FONT_FUNC_IMPLEMENT
    }
  }
};


/**
 * hb_font_funcs_create:
 *
 * Creates a new #hb_font_funcs_t structure of font functions.
 *
 * Return value: (transfer full): The font-functions structure
 *
 * Since: 0.9.2
 **/
hb_font_funcs_t *
hb_font_funcs_create ()
{
  hb_font_funcs_t *ffuncs;

  ఒకవేళ (!(ffuncs = hb_object_create<hb_font_funcs_t> ()))
    తిరిగి hb_font_funcs_get_ఖాళీలు ();

  ffuncs->get = _hb_font_funcs_default.get;

  తిరిగి ffuncs;
}

/**
 * hb_font_funcs_get_empty:
 *
 * Fetches an empty font-functions structure.
 *
 * Return value: (transfer full): The font-functions structure
 *
 * Since: 0.9.2
 **/
hb_font_funcs_t *
hb_font_funcs_get_ఖాళీలు ()
{
  తిరిగి const_cast<hb_font_funcs_t *> (&_hb_font_funcs_default);
}

/**
 * hb_font_funcs_reference: (skip)
 * @ffuncs: The font-functions structure
 *
 * Increases the reference count on a font-functions structure.
 *
 * Return value: The font-functions structure
 *
 * Since: 0.9.2
 **/
hb_font_funcs_t *
hb_font_funcs_reference (hb_font_funcs_t *ffuncs)
{
  తిరిగి hb_object_reference (ffuncs);
}

/**
 * hb_font_funcs_destroy: (skip)
 * @ffuncs: The font-functions structure
 *
 * Decreases the reference count on a font-functions structure. When
 * the reference count reaches zero, the font-functions structure is
 * destroyed, freeing all memory.
 *
 * Since: 0.9.2
 **/
శూన్య
hb_font_funcs_destroy (hb_font_funcs_t *ffuncs)
{
  ఒకవేళ (!hb_object_destroy (ffuncs)) తిరిగి;

  ఒకవేళ (ffuncs->destroy)
  {
#నిర్వచించు HB_FONT_FUNC_IMPLEMENT(get_,name) ఒకవేళ (ffuncs->destroy->name) \
    ffuncs->destroy->name (!ffuncs->user_దత్తాంశం ? శూన్యసూచిక : ffuncs->user_దత్తాంశం->name);
    HB_FONT_FUNCS_IMPLEMENT_CALLBACKS
#నిర్వచనం_రద్దు HB_FONT_FUNC_IMPLEMENT
  }

  hb_free (ffuncs->destroy);
  hb_free (ffuncs->user_దత్తాంశం);

  hb_free (ffuncs);
}

/**
 * hb_font_funcs_set_user_data: (skip)
 * @ffuncs: The font-functions structure
 * @key: The user-data key to set
 * @data: A pointer to the user data set
 * @destroy: (nullable): A callback to call when @data is not needed anymore
 * @replace: Whether to replace an existing data with the same key
 *
 * Attaches a user-data key/data pair to the specified font-functions structure.
 *
 * Return value: `true` if success, `false` otherwise
 *
 * Since: 0.9.2
 **/
hb_bool_t
hb_font_funcs_సమితి_user_దత్తాంశం (hb_font_funcs_t    *ffuncs,
			     hb_user_దత్తాంశం_కీలకం_t *key,
			     శూన్య *              దత్తాంశం,
			     hb_destroy_func_t   destroy /* May be NULL. */,
			     hb_bool_t           replace)
{
  తిరిగి hb_object_సమితి_user_దత్తాంశం (ffuncs, key, దత్తాంశం, destroy, replace);
}

/**
 * hb_font_funcs_get_user_data: (skip)
 * @ffuncs: The font-functions structure
 * @key: The user-data key to query
 *
 * Fetches the user data associated with the specified key,
 * attached to the specified font-functions structure.
 *
 * Return value: (transfer none): A pointer to the user data
 *
 * Since: 0.9.2
 **/
శూన్య *
hb_font_funcs_get_user_దత్తాంశం (స్థిరరాశి hb_font_funcs_t *ffuncs,
			     hb_user_దత్తాంశం_కీలకం_t    *key)
{
  తిరిగి hb_object_get_user_దత్తాంశం (ffuncs, key);
}


/**
 * hb_font_funcs_make_immutable:
 * @ffuncs: The font-functions structure
 *
 * Makes a font-functions structure immutable.
 *
 * Since: 0.9.2
 **/
శూన్య
hb_font_funcs_make_immutable (hb_font_funcs_t *ffuncs)
{
  ఒకవేళ (hb_object_is_immutable (ffuncs))
    తిరిగి;

  hb_object_make_immutable (ffuncs);
}

/**
 * hb_font_funcs_is_immutable:
 * @ffuncs: The font-functions structure
 *
 * Tests whether a font-functions structure is immutable.
 *
 * Return value: `true` if @ffuncs is immutable, `false` otherwise
 *
 * Since: 0.9.2
 **/
hb_bool_t
hb_font_funcs_is_immutable (hb_font_funcs_t *ffuncs)
{
  తిరిగి hb_object_is_immutable (ffuncs);
}


స్థిర తార్కిక
hb_font_funcs_సమితి_preamble (hb_font_funcs_t    *ffuncs,
			     తార్కిక                func_is_null,
			     శూన్య              **user_దత్తాంశం,
			     hb_destroy_func_t  *destroy)
{
  ఒకవేళ (hb_object_is_immutable (ffuncs))
  {
    ఒకవేళ (*destroy)
      (*destroy) (*user_దత్తాంశం);
    తిరిగి అబద్ధం;
  }

  ఒకవేళ (func_is_null)
  {
    ఒకవేళ (*destroy)
      (*destroy) (*user_దత్తాంశం);
    *destroy = శూన్యసూచిక;
    *user_దత్తాంశం = శూన్యసూచిక;
  }

  తిరిగి నిజం;
}

స్థిర తార్కిక
hb_font_funcs_సమితి_మధ్య (hb_font_funcs_t   *ffuncs,
			   శూన్య              *user_దత్తాంశం,
			   hb_destroy_func_t  destroy)
{
  స్వయంచాలక destroy_guard = hb_make_scope_guard ([&]() {
    ఒకవేళ (destroy) destroy (user_దత్తాంశం);
  });

  ఒకవేళ (user_దత్తాంశం && !ffuncs->user_దత్తాంశం)
  {
    ffuncs->user_దత్తాంశం = (decltype (ffuncs->user_దత్తాంశం)) hb_calloc (1, పరిమాణం (*ffuncs->user_దత్తాంశం));
    ఒకవేళ (unlikely (!ffuncs->user_దత్తాంశం))
      తిరిగి అబద్ధం;
  }
  ఒకవేళ (destroy && !ffuncs->destroy)
  {
    ffuncs->destroy = (decltype (ffuncs->destroy)) hb_calloc (1, పరిమాణం (*ffuncs->destroy));
    ఒకవేళ (unlikely (!ffuncs->destroy))
      తిరిగి అబద్ధం;
  }

  destroy_guard.release ();
  తిరిగి నిజం;
}

#నిర్వచించు HB_FONT_FUNC_IMPLEMENT(get_,name) \
									 \
శూన్య                                                                     \
hb_font_funcs_సమితి##name##_func (hb_font_funcs_t             *ffuncs,    \
				 hb_font_##get_##name##_func_t func,     \
				 శూన్య                        *user_దత్తాంశం, \
				 hb_destroy_func_t            destroy)   \
{                                                                        \
  ఒకవేళ (!hb_font_funcs_సమితి_preamble (ffuncs, !func, &user_దత్తాంశం, &destroy))\
      తిరిగి;                                                            \
									 \
  ఒకవేళ (ffuncs->destroy && ffuncs->destroy->name)                          \
    ffuncs->destroy->name (!ffuncs->user_దత్తాంశం ? శూన్యసూచిక : ffuncs->user_దత్తాంశం->name); \
                                                                         \
  ఒకవేళ (!hb_font_funcs_సమితి_మధ్య (ffuncs, user_దత్తాంశం, destroy))           \
      తిరిగి;                                                            \
									 \
  ఒకవేళ (func)                                                              \
    ffuncs->get.f.name = func;                                           \
  లేకపోతే                                                                   \
    ffuncs->get.f.name = hb_font_##get_##name##_default;                   \
									 \
  ఒకవేళ (ffuncs->user_దత్తాంశం)                                                 \
    ffuncs->user_దత్తాంశం->name = user_దత్తాంశం;                                 \
  ఒకవేళ (ffuncs->destroy)                                                   \
    ffuncs->destroy->name = destroy;                                     \
}

HB_FONT_FUNCS_IMPLEMENT_CALLBACKS
#నిర్వచనం_రద్దు HB_FONT_FUNC_IMPLEMENT

తార్కిక
hb_font_t::has_func_సమితి (గుర్తులేని పూర్ణాంకం i)
{
  తిరిగి ఇది->klass->get.array[i] != _hb_font_funcs_default.get.array[i];
}

తార్కిక
hb_font_t::has_func (గుర్తులేని పూర్ణాంకం i)
{
  తిరిగి has_func_సమితి (i) ||
	 (మాతృ && మాతృ != &_hb_Null_hb_font_t && మాతృ->has_func (i));
}

/* Public getters */

/**
 * hb_font_get_h_extents:
 * @font: #hb_font_t to work upon
 * @extents: (out): The font extents retrieved
 *
 * Fetches the extents for a specified font, for horizontal
 * text segments.
 *
 * Return value: `true` if data found, `false` otherwise
 *
 * Since: 1.1.3
 **/
hb_bool_t
hb_font_get_h_extents (hb_font_t         *font,
		       hb_font_extents_t *extents)
{
  తిరిగి font->get_font_h_extents (extents);
}

/**
 * hb_font_get_v_extents:
 * @font: #hb_font_t to work upon
 * @extents: (out): The font extents retrieved
 *
 * Fetches the extents for a specified font, for vertical
 * text segments.
 *
 * Return value: `true` if data found, `false` otherwise
 *
 * Since: 1.1.3
 **/
hb_bool_t
hb_font_get_v_extents (hb_font_t         *font,
		       hb_font_extents_t *extents)
{
  తిరిగి font->get_font_v_extents (extents);
}

/**
 * hb_font_get_glyph:
 * @font: #hb_font_t to work upon
 * @unicode: The Unicode code point to query
 * @variation_selector: A variation-selector code point
 * @glyph: (out): The glyph ID retrieved
 *
 * Fetches the glyph ID for a Unicode code point in the specified
 * font, with an optional variation selector.
 *
 * If @variation_selector is 0, calls hb_font_get_nominal_glyph();
 * otherwise calls hb_font_get_variation_glyph().
 *
 * Return value: `true` if data found, `false` otherwise
 *
 * Since: 0.9.2
 **/
hb_bool_t
hb_font_get_glyph (hb_font_t      *font,
		   hb_codepoint_t  unicode,
		   hb_codepoint_t  variation_selector,
		   hb_codepoint_t *glyph)
{
  ఒకవేళ (unlikely (variation_selector))
    తిరిగి font->get_variation_glyph (unicode, variation_selector, glyph);
  తిరిగి font->get_nominal_glyph (unicode, glyph);
}

/**
 * hb_font_get_nominal_glyph:
 * @font: #hb_font_t to work upon
 * @unicode: The Unicode code point to query
 * @glyph: (out): The glyph ID retrieved
 *
 * Fetches the nominal glyph ID for a Unicode code point in the
 * specified font.
 *
 * This version of the function should not be used to fetch glyph IDs
 * for code points modified by variation selectors. For variation-selector
 * support, user hb_font_get_variation_glyph() or use hb_font_get_glyph().
 *
 * Return value: `true` if data found, `false` otherwise
 *
 * Since: 1.2.3
 **/
hb_bool_t
hb_font_get_nominal_glyph (hb_font_t      *font,
			   hb_codepoint_t  unicode,
			   hb_codepoint_t *glyph)
{
  తిరిగి font->get_nominal_glyph (unicode, glyph);
}

/**
 * hb_font_get_nominal_glyphs:
 * @font: #hb_font_t to work upon
 * @count: number of code points to query
 * @first_unicode: The first Unicode code point to query
 * @unicode_stride: The stride between successive code points
 * @first_glyph: (out): The first glyph ID retrieved
 * @glyph_stride: The stride between successive glyph IDs
 *
 * Fetches the nominal glyph IDs for a sequence of Unicode code points. Glyph
 * IDs must be returned in a #hb_codepoint_t output parameter. Stops at the
 * first unsupported glyph ID.
 *
 * Return value: the number of code points processed
 *
 * Since: 2.6.3
 **/
గుర్తులేని పూర్ణాంకం
hb_font_get_nominal_glyphs (hb_font_t *font,
			    గుర్తులేని పూర్ణాంకం లెక్క,
			    స్థిరరాశి hb_codepoint_t *మొదటి_unicode,
			    గుర్తులేని పూర్ణాంకం unicode_stride,
			    hb_codepoint_t *మొదటి_glyph,
			    గుర్తులేని పూర్ణాంకం glyph_stride)
{
  తిరిగి font->get_nominal_glyphs (లెక్క,
				   మొదటి_unicode, unicode_stride,
				   మొదటి_glyph, glyph_stride);
}

/**
 * hb_font_get_variation_glyph:
 * @font: #hb_font_t to work upon
 * @unicode: The Unicode code point to query
 * @variation_selector: The  variation-selector code point to query
 * @glyph: (out): The glyph ID retrieved
 *
 * Fetches the glyph ID for a Unicode code point when followed by
 * by the specified variation-selector code point, in the specified
 * font.
 *
 * Return value: `true` if data found, `false` otherwise
 *
 * Since: 1.2.3
 **/
hb_bool_t
hb_font_get_variation_glyph (hb_font_t      *font,
			     hb_codepoint_t  unicode,
			     hb_codepoint_t  variation_selector,
			     hb_codepoint_t *glyph)
{
  తిరిగి font->get_variation_glyph (unicode, variation_selector, glyph);
}

/**
 * hb_font_get_glyph_h_advance:
 * @font: #hb_font_t to work upon
 * @glyph: The glyph ID to query
 *
 * Fetches the advance for a glyph ID in the specified font,
 * for horizontal text segments.
 *
 * Return value: The advance of @glyph within @font
 *
 * Since: 0.9.2
 **/
hb_position_t
hb_font_get_glyph_h_advance (hb_font_t      *font,
			     hb_codepoint_t  glyph)
{
  తిరిగి font->get_glyph_h_advance (glyph);
}

/**
 * hb_font_get_glyph_v_advance:
 * @font: #hb_font_t to work upon
 * @glyph: The glyph ID to query
 *
 * Fetches the advance for a glyph ID in the specified font,
 * for vertical text segments.
 *
 * Return value: The advance of @glyph within @font
 *
 * Since: 0.9.2
 **/
hb_position_t
hb_font_get_glyph_v_advance (hb_font_t      *font,
			     hb_codepoint_t  glyph)
{
  తిరిగి font->get_glyph_v_advance (glyph);
}

/**
 * hb_font_get_glyph_h_advances:
 * @font: #hb_font_t to work upon
 * @count: The number of glyph IDs in the sequence queried
 * @first_glyph: The first glyph ID to query
 * @glyph_stride: The stride between successive glyph IDs
 * @first_advance: (out): The first advance retrieved
 * @advance_stride: The stride between successive advances
 *
 * Fetches the advances for a sequence of glyph IDs in the specified
 * font, for horizontal text segments.
 *
 * Since: 1.8.6
 **/
శూన్య
hb_font_get_glyph_h_advances (hb_font_t*            font,
			      గుర్తులేని పూర్ణాంకం          లెక్క,
			      స్థిరరాశి hb_codepoint_t *మొదటి_glyph,
			      గుర్తులేని              glyph_stride,
			      hb_position_t        *మొదటి_advance,
			      గుర్తులేని              advance_stride)
{
  font->get_glyph_h_advances (లెక్క, మొదటి_glyph, glyph_stride, మొదటి_advance, advance_stride);
}
/**
 * hb_font_get_glyph_v_advances:
 * @font: #hb_font_t to work upon
 * @count: The number of glyph IDs in the sequence queried
 * @first_glyph: The first glyph ID to query
 * @glyph_stride: The stride between successive glyph IDs
 * @first_advance: (out): The first advance retrieved
 * @advance_stride: (out): The stride between successive advances
 *
 * Fetches the advances for a sequence of glyph IDs in the specified
 * font, for vertical text segments.
 *
 * Since: 1.8.6
 **/
శూన్య
hb_font_get_glyph_v_advances (hb_font_t*            font,
			      గుర్తులేని పూర్ణాంకం          లెక్క,
			      స్థిరరాశి hb_codepoint_t *మొదటి_glyph,
			      గుర్తులేని              glyph_stride,
			      hb_position_t        *మొదటి_advance,
			      గుర్తులేని              advance_stride)
{
  font->get_glyph_v_advances (లెక్క, మొదటి_glyph, glyph_stride, మొదటి_advance, advance_stride);
}

/**
 * hb_font_get_glyph_h_origin:
 * @font: #hb_font_t to work upon
 * @glyph: The glyph ID to query
 * @x: (out): The X coordinate of the origin
 * @y: (out): The Y coordinate of the origin
 *
 * Fetches the (X,Y) coordinates of the origin for a glyph ID
 * in the specified font, for horizontal text segments.
 *
 * Return value: `true` if data found, `false` otherwise
 *
 * Since: 0.9.2
 **/
hb_bool_t
hb_font_get_glyph_h_origin (hb_font_t      *font,
			    hb_codepoint_t  glyph,
			    hb_position_t  *x,
			    hb_position_t  *y)
{
  తిరిగి font->get_glyph_h_origin (glyph, x, y);
}

/**
 * hb_font_get_glyph_v_origin:
 * @font: #hb_font_t to work upon
 * @glyph: The glyph ID to query
 * @x: (out): The X coordinate of the origin
 * @y: (out): The Y coordinate of the origin
 *
 * Fetches the (X,Y) coordinates of the origin for a glyph ID
 * in the specified font, for vertical text segments.
 *
 * Return value: `true` if data found, `false` otherwise
 *
 * Since: 0.9.2
 **/
hb_bool_t
hb_font_get_glyph_v_origin (hb_font_t      *font,
			    hb_codepoint_t  glyph,
			    hb_position_t  *x,
			    hb_position_t  *y)
{
  తిరిగి font->get_glyph_v_origin (glyph, x, y);
}

/**
 * hb_font_get_glyph_h_origins:
 * @font: #hb_font_t to work upon
 * @count: The number of glyph IDs in the sequence queried
 * @first_glyph: The first glyph ID to query
 * @glyph_stride: The stride between successive glyph IDs
 * @first_x: (out): The first X coordinate of the origin retrieved
 * @x_stride: The stride between successive X coordinates
 * @first_y: (out): The first Y coordinate of the origin retrieved
 * @y_stride: The stride between successive Y coordinates
 *
 * Fetches the (X,Y) coordinates of the origin for requested glyph IDs
 * in the specified font, for horizontal text segments.
 *
 * Return value: `true` if data found, `false` otherwise
 *
 * Since: 11.3.0
 **/
hb_bool_t
hb_font_get_glyph_h_origins (hb_font_t      *font,
			     గుర్తులేని పూర్ణాంకం    లెక్క,
			     స్థిరరాశి hb_codepoint_t *మొదటి_glyph,
			     గుర్తులేని పూర్ణాంకం    glyph_stride,
			     hb_position_t  *మొదటి_x,
			     గుర్తులేని పూర్ణాంకం    x_stride,
			     hb_position_t  *మొదటి_y,
			     గుర్తులేని పూర్ణాంకం    y_stride)

{
  తిరిగి font->get_glyph_h_origins (లెక్క,
				    మొదటి_glyph, glyph_stride,
				    మొదటి_x, x_stride,
				    మొదటి_y, y_stride);
}

/**
 * hb_font_get_glyph_v_origins:
 * @font: #hb_font_t to work upon
 * @count: The number of glyph IDs in the sequence queried
 * @first_glyph: The first glyph ID to query
 * @glyph_stride: The stride between successive glyph IDs
 * @first_x: (out): The first X coordinate of the origin retrieved
 * @x_stride: The stride between successive X coordinates
 * @first_y: (out): The first Y coordinate of the origin retrieved
 * @y_stride: The stride between successive Y coordinates
 *
 * Fetches the (X,Y) coordinates of the origin for requested glyph IDs
 * in the specified font, for vertical text segments.
 *
 * Return value: `true` if data found, `false` otherwise
 *
 * Since: 11.3.0
 **/
hb_bool_t
hb_font_get_glyph_v_origins (hb_font_t      *font,
			     గుర్తులేని పూర్ణాంకం    లెక్క,
			     స్థిరరాశి hb_codepoint_t *మొదటి_glyph,
			     గుర్తులేని పూర్ణాంకం    glyph_stride,
			     hb_position_t  *మొదటి_x,
			     గుర్తులేని పూర్ణాంకం    x_stride,
			     hb_position_t  *మొదటి_y,
			     గుర్తులేని పూర్ణాంకం    y_stride)

{
  తిరిగి font->get_glyph_v_origins (లెక్క,
				    మొదటి_glyph, glyph_stride,
				    మొదటి_x, x_stride,
				    మొదటి_y, y_stride);
}


/**
 * hb_font_get_glyph_h_kerning:
 * @font: #hb_font_t to work upon
 * @left_glyph: The glyph ID of the left glyph in the glyph pair
 * @right_glyph: The glyph ID of the right glyph in the glyph pair
 *
 * Fetches the kerning-adjustment value for a glyph-pair in
 * the specified font, for horizontal text segments.
 *
 * <note>It handles legacy kerning only (as returned by the corresponding
 * #hb_font_funcs_t function).</note>
 *
 * Return value: The kerning adjustment value
 *
 * Since: 0.9.2
 **/
hb_position_t
hb_font_get_glyph_h_kerning (hb_font_t      *font,
			     hb_codepoint_t  ఎడమ_glyph,
			     hb_codepoint_t  కుడి_glyph)
{
  తిరిగి font->get_glyph_h_kerning (ఎడమ_glyph, కుడి_glyph);
}

#నిర్వచితం_కాకపోతే HB_DISABLE_DEPRECATED
/**
 * hb_font_get_glyph_v_kerning:
 * @font: #hb_font_t to work upon
 * @top_glyph: The glyph ID of the top glyph in the glyph pair
 * @bottom_glyph: The glyph ID of the bottom glyph in the glyph pair
 *
 * Fetches the kerning-adjustment value for a glyph-pair in
 * the specified font, for vertical text segments.
 *
 * <note>It handles legacy kerning only (as returned by the corresponding
 * #hb_font_funcs_t function).</note>
 *
 * Return value: The kerning adjustment value
 *
 * Since: 0.9.2
 * Deprecated: 2.0.0
 **/
hb_position_t
hb_font_get_glyph_v_kerning (hb_font_t      *font,
			     hb_codepoint_t  శిఖరం_glyph,
			     hb_codepoint_t  bottom_glyph)
{
  తిరిగి font->get_glyph_v_kerning (శిఖరం_glyph, bottom_glyph);
}
#అయితే_ముగింపు

/**
 * hb_font_get_glyph_extents:
 * @font: #hb_font_t to work upon
 * @glyph: The glyph ID to query
 * @extents: (out): The #hb_glyph_extents_t retrieved
 *
 * Fetches the #hb_glyph_extents_t data for a glyph ID
 * in the specified font.
 *
 * Return value: `true` if data found, `false` otherwise
 *
 * Since: 0.9.2
 **/
hb_bool_t
hb_font_get_glyph_extents (hb_font_t          *font,
			   hb_codepoint_t      glyph,
			   hb_glyph_extents_t *extents)
{
  తిరిగి font->get_glyph_extents (glyph, extents);
}

/**
 * hb_font_get_glyph_contour_point:
 * @font: #hb_font_t to work upon
 * @glyph: The glyph ID to query
 * @point_index: The contour-point index to query
 * @x: (out): The X value retrieved for the contour point
 * @y: (out): The Y value retrieved for the contour point
 *
 * Fetches the (x,y) coordinates of a specified contour-point index
 * in the specified glyph, within the specified font.
 *
 * Return value: `true` if data found, `false` otherwise
 *
 * Since: 0.9.2
 **/
hb_bool_t
hb_font_get_glyph_contour_బిందువు (hb_font_t      *font,
				 hb_codepoint_t  glyph,
				 గుర్తులేని పూర్ణాంకం    బిందువు_సూచీ,
				 hb_position_t  *x,
				 hb_position_t  *y)
{
  తిరిగి font->get_glyph_contour_బిందువు (glyph, బిందువు_సూచీ, x, y);
}

/**
 * hb_font_get_glyph_name:
 * @font: #hb_font_t to work upon
 * @glyph: The glyph ID to query
 * @name: (out) (array length=size): Name string retrieved for the glyph ID
 * @size: Length of the glyph-name string retrieved
 *
 * Fetches the glyph-name string for a glyph ID in the specified @font.
 *
 * According to the OpenType specification, glyph names are limited to 63
 * characters and can only contain (a subset of) ASCII.
 *
 * Return value: `true` if data found, `false` otherwise
 *
 * Since: 0.9.2
 **/
hb_bool_t
hb_font_get_glyph_name (hb_font_t      *font,
			hb_codepoint_t  glyph,
			అక్షరం           *name,
			గుర్తులేని పూర్ణాంకం    కొలత)
{
  తిరిగి font->get_glyph_name (glyph, name, కొలత);
}

/**
 * hb_font_get_glyph_from_name:
 * @font: #hb_font_t to work upon
 * @name: (array length=len): The name string to query
 * @len: The length of the name queried
 * @glyph: (out): The glyph ID retrieved
 *
 * Fetches the glyph ID that corresponds to a name string in the specified @font.
 *
 * <note>Note: @len == -1 means the name string is null-terminated.</note>
 *
 * Return value: `true` if data found, `false` otherwise
 *
 * Since: 0.9.2
 **/
hb_bool_t
hb_font_get_glyph_from_name (hb_font_t      *font,
			     స్థిరరాశి అక్షరం     *name,
			     పూర్ణాంకం             len, /* -1 means nul-terminated */
			     hb_codepoint_t *glyph)
{
  తిరిగి font->get_glyph_from_name (name, len, glyph);
}

#నిర్వచితం_కాకపోతే HB_DISABLE_DEPRECATED
/**
 * hb_font_get_glyph_shape:
 * @font: #hb_font_t to work upon
 * @glyph: The glyph ID
 * @dfuncs: #hb_draw_funcs_t to draw to
 * @draw_data: User data to pass to draw callbacks
 *
 * Fetches the glyph shape that corresponds to a glyph in the specified @font.
 * The shape is returned by way of calls to the callbacks of the @dfuncs
 * objects, with @draw_data passed to them.
 *
 * Since: 4.0.0
 * Deprecated: 7.0.0: Use hb_font_draw_glyph() instead
 */
శూన్య
hb_font_get_glyph_shape (hb_font_t *font,
		         hb_codepoint_t glyph,
		         hb_draw_funcs_t *dfuncs, శూన్య *draw_దత్తాంశం)
{
  hb_font_draw_glyph (font, glyph, dfuncs, draw_దత్తాంశం);
}
#అయితే_ముగింపు

/**
 * hb_font_draw_glyph_or_fail:
 * @font: #hb_font_t to work upon
 * @glyph: The glyph ID
 * @dfuncs: #hb_draw_funcs_t to draw to
 * @draw_data: User data to pass to draw callbacks
 *
 * Draws the outline that corresponds to a glyph in the specified @font.
 *
 * This is a newer name for hb_font_draw_glyph(), that returns `false`
 * if the font has no outlines for the glyph.
 *
 * The outline is returned by way of calls to the callbacks of the @dfuncs
 * objects, with @draw_data passed to them.
 *
 * Return value: `true` if glyph was drawn, `false` otherwise
 *
 * Since: 11.2.0
 **/
hb_bool_t
hb_font_draw_glyph_or_fail (hb_font_t *font,
			    hb_codepoint_t glyph,
			    hb_draw_funcs_t *dfuncs, శూన్య *draw_దత్తాంశం)
{
  తిరిగి font->draw_glyph_or_fail (glyph, dfuncs, draw_దత్తాంశం);
}

/**
 * hb_font_paint_glyph_or_fail:
 * @font: #hb_font_t to work upon
 * @glyph: The glyph ID
 * @pfuncs: #hb_paint_funcs_t to paint with
 * @paint_data: User data to pass to paint callbacks
 * @palette_index: The index of the font's color palette to use
 * @foreground: The foreground color, unpremultipled
 *
 * Paints a color glyph.
 *
 * Succeeds if @glyph has color paint layers (COLRv0),
 * a color paint graph (COLRv1), or a bitmap image that the
 * font's callbacks render successfully.  Returns `false` if
 * the font has no color data for @glyph; the client can then
 * fall back to hb_font_draw_glyph_or_fail() for the monochrome
 * outline.
 *
 * The painting instructions are returned by way of calls to
 * the callbacks of the @funcs object, with @paint_data passed
 * to them.
 *
 * If the font has color palettes (see hb_ot_color_has_palettes()),
 * then @palette_index selects the palette to use. If the font only
 * has one palette, this will be 0.
 *
 * Return value: `true` if glyph was painted, `false` otherwise
 *
 * Since: 11.2.0
 */
hb_bool_t
hb_font_paint_glyph_or_fail (hb_font_t *font,
			     hb_codepoint_t glyph,
			     hb_paint_funcs_t *pfuncs, శూన్య *paint_దత్తాంశం,
			     గుర్తులేని పూర్ణాంకం palette_సూచీ,
			     hb_color_t foreground)
{
  తిరిగి font->paint_glyph_or_fail (glyph, pfuncs, paint_దత్తాంశం, palette_సూచీ, foreground);
}

/* A bit higher-level, and with fallback */

శూన్య
hb_font_t::paint_glyph (hb_codepoint_t glyph,
			hb_paint_funcs_t *paint_funcs, శూన్య *paint_దత్తాంశం,
			గుర్తులేని పూర్ణాంకం palette,
			hb_color_t foreground)
{
  ఒకవేళ (paint_glyph_or_fail (glyph,
			   paint_funcs, paint_దత్తాంశం,
			   palette, foreground))
    తిరిగి;

  /* Fallback for outline glyph. */
  paint_funcs->fill_glyph (paint_దత్తాంశం, glyph, ఇది, నిజం, foreground);
}


/**
 * hb_font_draw_glyph:
 * @font: #hb_font_t to work upon
 * @glyph: The glyph ID
 * @dfuncs: #hb_draw_funcs_t to draw to
 * @draw_data: User data to pass to draw callbacks
 *
 * Draws the outline that corresponds to a glyph in the specified @font.
 *
 * This is an older name for hb_font_draw_glyph_or_fail(), with no
 * return value.
 *
 * The outline is returned by way of calls to the callbacks of the @dfuncs
 * objects, with @draw_data passed to them.
 *
 * Since: 7.0.0
 **/
శూన్య
hb_font_draw_glyph (hb_font_t *font,
		    hb_codepoint_t glyph,
		    hb_draw_funcs_t *dfuncs, శూన్య *draw_దత్తాంశం)
{
  (శూన్య) hb_font_draw_glyph_or_fail (font, glyph, dfuncs, draw_దత్తాంశం);
}

/**
 * hb_font_paint_glyph:
 * @font: #hb_font_t to work upon
 * @glyph: The glyph ID
 * @pfuncs: #hb_paint_funcs_t to paint with
 * @paint_data: User data to pass to paint callbacks
 * @palette_index: The index of the font's color palette to use
 * @foreground: The foreground color, unpremultipled
 *
 * Paints the glyph. This function is similar to
 * hb_font_paint_glyph_or_fail(), but if painting a color glyph
 * failed, it will fall back to painting an outline monochrome
 * glyph.
 *
 * The painting instructions are returned by way of calls to
 * the callbacks of the @funcs object, with @paint_data passed
 * to them.
 *
 * If the font has color palettes (see hb_ot_color_has_palettes()),
 * then @palette_index selects the palette to use. If the font only
 * has one palette, this will be 0.
 *
 * Since: 7.0.0
 */
శూన్య
hb_font_paint_glyph (hb_font_t *font,
                     hb_codepoint_t glyph,
                     hb_paint_funcs_t *pfuncs, శూన్య *paint_దత్తాంశం,
                     గుర్తులేని పూర్ణాంకం palette_సూచీ,
                     hb_color_t foreground)
{
  font->paint_glyph (glyph, pfuncs, paint_దత్తాంశం, palette_సూచీ, foreground);
}

/**
 * hb_font_get_extents_for_direction:
 * @font: #hb_font_t to work upon
 * @direction: The direction of the text segment
 * @extents: (out): The #hb_font_extents_t retrieved
 *
 * Fetches the extents for a font in a text segment of the
 * specified direction.
 *
 * Calls the appropriate direction-specific variant (horizontal
 * or vertical) depending on the value of @direction.
 *
 * Since: 1.1.3
 **/
శూన్య
hb_font_get_extents_for_direction (hb_font_t         *font,
				   hb_direction_t     direction,
				   hb_font_extents_t *extents)
{
  font->get_extents_for_direction (direction, extents);
}
/**
 * hb_font_get_glyph_advance_for_direction:
 * @font: #hb_font_t to work upon
 * @glyph: The glyph ID to query
 * @direction: The direction of the text segment
 * @x: (out): The horizontal advance retrieved
 * @y: (out):  The vertical advance retrieved
 *
 * Fetches the advance for a glyph ID from the specified font,
 * in a text segment of the specified direction.
 *
 * Calls the appropriate direction-specific variant (horizontal
 * or vertical) depending on the value of @direction.
 *
 * Since: 0.9.2
 **/
శూన్య
hb_font_get_glyph_advance_for_direction (hb_font_t      *font,
					 hb_codepoint_t  glyph,
					 hb_direction_t  direction,
					 hb_position_t  *x,
					 hb_position_t  *y)
{
  font->get_glyph_advance_for_direction (glyph, direction, x, y);
}
/**
 * hb_font_get_glyph_advances_for_direction:
 * @font: #hb_font_t to work upon
 * @direction: The direction of the text segment
 * @count: The number of glyph IDs in the sequence queried
 * @first_glyph: The first glyph ID to query
 * @glyph_stride: The stride between successive glyph IDs
 * @first_advance: (out): The first advance retrieved
 * @advance_stride: (out): The stride between successive advances
 *
 * Fetches the advances for a sequence of glyph IDs in the specified
 * font, in a text segment of the specified direction.
 *
 * Calls the appropriate direction-specific variant (horizontal
 * or vertical) depending on the value of @direction.
 *
 * Since: 1.8.6
 **/
HB_EXTERN శూన్య
hb_font_get_glyph_advances_for_direction (hb_font_t*            font,
					  hb_direction_t        direction,
					  గుర్తులేని పూర్ణాంకం          లెక్క,
					  స్థిరరాశి hb_codepoint_t *మొదటి_glyph,
					  గుర్తులేని              glyph_stride,
					  hb_position_t        *మొదటి_advance,
					  గుర్తులేని              advance_stride)
{
  font->get_glyph_advances_for_direction (direction, లెక్క, మొదటి_glyph, glyph_stride, మొదటి_advance, advance_stride);
}

/**
 * hb_font_get_glyph_origin_for_direction:
 * @font: #hb_font_t to work upon
 * @glyph: The glyph ID to query
 * @direction: The direction of the text segment
 * @x: (out): The X coordinate retrieved for the origin
 * @y: (out): The Y coordinate retrieved for the origin
 *
 * Fetches the (X,Y) coordinates of the origin for a glyph in
 * the specified font.
 *
 * Calls the appropriate direction-specific variant (horizontal
 * or vertical) depending on the value of @direction.
 *
 * Since: 0.9.2
 **/
శూన్య
hb_font_get_glyph_origin_for_direction (hb_font_t      *font,
					hb_codepoint_t  glyph,
					hb_direction_t  direction,
					hb_position_t  *x,
					hb_position_t  *y)
{
  తిరిగి font->get_glyph_origin_for_direction (glyph, direction, x, y);
}

/**
 * hb_font_add_glyph_origin_for_direction:
 * @font: #hb_font_t to work upon
 * @glyph: The glyph ID to query
 * @direction: The direction of the text segment
 * @x: (inout): Input = The original X coordinate
 *     Output = The X coordinate plus the X-coordinate of the origin
 * @y: (inout): Input = The original Y coordinate
 *     Output = The Y coordinate plus the Y-coordinate of the origin
 *
 * Adds the origin coordinates to an (X,Y) point coordinate, in
 * the specified glyph ID in the specified font.
 *
 * Calls the appropriate direction-specific variant (horizontal
 * or vertical) depending on the value of @direction.
 *
 * Since: 0.9.2
 **/
శూన్య
hb_font_కూడు_glyph_origin_for_direction (hb_font_t      *font,
					hb_codepoint_t  glyph,
					hb_direction_t  direction,
					hb_position_t  *x,
					hb_position_t  *y)
{
  తిరిగి font->కూడు_glyph_origin_for_direction (glyph, direction, x, y);
}

/**
 * hb_font_subtract_glyph_origin_for_direction:
 * @font: #hb_font_t to work upon
 * @glyph: The glyph ID to query
 * @direction: The direction of the text segment
 * @x: (inout): Input = The original X coordinate
 *     Output = The X coordinate minus the X-coordinate of the origin
 * @y: (inout): Input = The original Y coordinate
 *     Output = The Y coordinate minus the Y-coordinate of the origin
 *
 * Subtracts the origin coordinates from an (X,Y) point coordinate,
 * in the specified glyph ID in the specified font.
 *
 * Calls the appropriate direction-specific variant (horizontal
 * or vertical) depending on the value of @direction.
 *
 * Since: 0.9.2
 **/
శూన్య
hb_font_వ్యవకలనం_glyph_origin_for_direction (hb_font_t      *font,
					     hb_codepoint_t  glyph,
					     hb_direction_t  direction,
					     hb_position_t  *x,
					     hb_position_t  *y)
{
  తిరిగి font->వ్యవకలనం_glyph_origin_for_direction (glyph, direction, x, y);
}

/**
 * hb_font_get_glyph_kerning_for_direction:
 * @font: #hb_font_t to work upon
 * @first_glyph: The glyph ID of the first glyph in the glyph pair to query
 * @second_glyph: The glyph ID of the second glyph in the glyph pair to query
 * @direction: The direction of the text segment
 * @x: (out): The horizontal kerning-adjustment value retrieved
 * @y: (out): The vertical kerning-adjustment value retrieved
 *
 * Fetches the kerning-adjustment value for a glyph-pair in the specified font.
 *
 * Calls the appropriate direction-specific variant (horizontal
 * or vertical) depending on the value of @direction.
 *
 * Since: 0.9.2
 **/
శూన్య
hb_font_get_glyph_kerning_for_direction (hb_font_t      *font,
					 hb_codepoint_t  మొదటి_glyph,
					 hb_codepoint_t  రెండవ_glyph,
					 hb_direction_t  direction,
					 hb_position_t  *x,
					 hb_position_t  *y)
{
  తిరిగి font->get_glyph_kerning_for_direction (మొదటి_glyph, రెండవ_glyph, direction, x, y);
}

/**
 * hb_font_get_glyph_extents_for_origin:
 * @font: #hb_font_t to work upon
 * @glyph: The glyph ID to query
 * @direction: The direction of the text segment
 * @extents: (out): The #hb_glyph_extents_t retrieved
 *
 * Fetches the #hb_glyph_extents_t data for a glyph ID
 * in the specified font, with respect to the origin in
 * a text segment in the specified direction.
 *
 * Calls the appropriate direction-specific variant (horizontal
 * or vertical) depending on the value of @direction.
 *
 * Return value: `true` if data found, `false` otherwise
 *
 * Since: 0.9.2
 **/
hb_bool_t
hb_font_get_glyph_extents_for_origin (hb_font_t          *font,
				      hb_codepoint_t      glyph,
				      hb_direction_t      direction,
				      hb_glyph_extents_t *extents)
{
  తిరిగి font->get_glyph_extents_for_origin (glyph, direction, extents);
}

/**
 * hb_font_get_glyph_contour_point_for_origin:
 * @font: #hb_font_t to work upon
 * @glyph: The glyph ID to query
 * @point_index: The contour-point index to query
 * @direction: The direction of the text segment
 * @x: (out): The X value retrieved for the contour point
 * @y: (out): The Y value retrieved for the contour point
 *
 * Fetches the (X,Y) coordinates of a specified contour-point index
 * in the specified glyph ID in the specified font, with respect
 * to the origin in a text segment in the specified direction.
 *
 * Calls the appropriate direction-specific variant (horizontal
 * or vertical) depending on the value of @direction.
 *
 * Return value: `true` if data found, `false` otherwise
 *
 * Since: 0.9.2
 **/
hb_bool_t
hb_font_get_glyph_contour_బిందువు_for_origin (hb_font_t      *font,
					    hb_codepoint_t  glyph,
					    గుర్తులేని పూర్ణాంకం    బిందువు_సూచీ,
					    hb_direction_t  direction,
					    hb_position_t  *x,
					    hb_position_t  *y)
{
  తిరిగి font->get_glyph_contour_బిందువు_for_origin (glyph, బిందువు_సూచీ, direction, x, y);
}

/**
 * hb_font_glyph_to_string:
 * @font: #hb_font_t to work upon
 * @glyph: The glyph ID to query
 * @s: (out) (array length=size): The string containing the glyph name
 * @size: Length of string @s
 *
 * Fetches the name of the specified glyph ID in @font and returns
 * it in string @s.
 *
 * If the glyph ID has no name in @font, a string of the form `gidDDD` is
 * generated, with `DDD` being the glyph ID.
 *
 * According to the OpenType specification, glyph names are limited to 63
 * characters and can only contain (a subset of) ASCII.
 *
 * Since: 0.9.2
 **/
శూన్య
hb_font_glyph_గమ్యం_string (hb_font_t      *font,
			 hb_codepoint_t  glyph,
			 అక్షరం           *s,
			 గుర్తులేని పూర్ణాంకం    కొలత)
{
  font->glyph_గమ్యం_string (glyph, s, కొలత);
}

/**
 * hb_font_glyph_from_string:
 * @font: #hb_font_t to work upon
 * @s: (array length=len) (element-type uint8_t): string to query
 * @len: The length of the string @s
 * @glyph: (out): The glyph ID corresponding to the string requested
 *
 * Fetches the glyph ID from @font that matches the specified string.
 * Strings of the format `gidDDD` or `uniUUUU` are parsed automatically.
 *
 * <note>Note: @len == -1 means the string is null-terminated.</note>
 *
 * Return value: `true` if data found, `false` otherwise
 *
 * Since: 0.9.2
 **/
hb_bool_t
hb_font_glyph_from_string (hb_font_t      *font,
			   స్థిరరాశి అక్షరం     *s,
			   పూర్ణాంకం             len,
			   hb_codepoint_t *glyph)
{
  తిరిగి font->glyph_from_string (s, len, glyph);
}


/*
 * hb_font_t
 */

DEFINE_NULL_INSTANCE (hb_font_t) =
{
  HB_OBJECT_శీర్షిక_STATIC,

  0, /* serial */
  0, /* serial_coords */

  శూన్యసూచిక, /*మాతృ*/
  const_cast<hb_face_t *> (&_hb_Null_hb_face_t),

  1000, /* x_scale */
  1000, /* y_scale */
  అబద్ధం, /* is_synthetic */
  0.f, /* x_embolden */
  0.f, /* y_embolden */
  నిజం, /* embolden_in_place */
  0, /* x_strength */
  0, /* y_strength */
  0.f, /* slant */
  0.f, /* slant_xy; */
  1.f, /* x_multf */
  1.f, /* y_multf */
  1<<16, /* x_mult */
  1<<16, /* y_mult */

  0, /* x_ppem */
  0, /* y_ppem */
  0, /* ptem */

  HB_FONT_NO_VAR_NAMED_INSTANCE, /* instance_index */
  అబద్ధం, /* has_nonzero_coords */
  0, /* num_coords */
  శూన్యసూచిక, /* coords */
  శూన్యసూచిక, /* design_coords */

  const_cast<hb_font_funcs_t *> (&_hb_Null_hb_font_funcs_t),

  /* Zero for the rest is fine. */
};


స్థిర hb_font_t *
_hb_font_create (hb_face_t *face)
{
  hb_font_t *font;

  ఒకవేళ (unlikely (!face))
    face = hb_face_get_ఖాళీలు ();

  ఒకవేళ (!(font = hb_object_create<hb_font_t> ()))
    తిరిగి hb_font_get_ఖాళీలు ();

  hb_face_make_immutable (face);
  font->మాతృ = hb_font_get_ఖాళీలు ();
  font->face = hb_face_reference (face);
  font->klass = hb_font_funcs_get_ఖాళీలు ();
  font->దత్తాంశం.init0 (font);
  font->x_scale = font->y_scale = face->get_upem ();
  font->embolden_లోపల_ఉంచు = నిజం;
  font->x_multf = font->y_multf = 1.f;
  font->x_mult = font->y_mult = 1 << 16;
  font->instance_సూచీ = HB_FONT_NO_VAR_NAMED_INSTANCE;

  తిరిగి font;
}

/**
 * hb_font_create:
 * @face: a face.
 *
 * Constructs a new font object from the specified face.
 *
 * <note>Note: If @face's index value (as passed to hb_face_create()
 * has non-zero top 16-bits, those bits minus one are passed to
 * hb_font_set_var_named_instance(), effectively loading a named-instance
 * of a variable font, instead of the default-instance.  This allows
 * specifying which named-instance to load by default when creating the
 * face.</note>
 *
 * Return value: (transfer full): The new font object
 *
 * Since: 0.9.2
 **/
hb_font_t *
hb_font_create (hb_face_t *face)
{
  hb_font_t *font = _hb_font_create (face);

  hb_font_సమితి_funcs_using (font, శూన్యసూచిక);

#నిర్వచితం_కాకపోతే HB_NO_VAR
  // Initialize variations.
  ఒకవేళ (likely (face))
  {
    ఒకవేళ (face->సూచీ_2 >> 16)
      hb_font_సమితి_var_named_instance (font, (face->సూచీ_2 >> 16) - 1);
    లేకపోతే
      hb_font_సమితి_variations (font, శూన్యసూచిక, 0);
  }
#అయితే_ముగింపు

  తిరిగి font;
}

స్థిర శూన్య
_hb_font_adopt_var_coords (hb_font_t *font,
			   పూర్ణాంకం *coords, /* 2.14 normalized */
			   దశాంశం *design_coords,
			   గుర్తులేని పూర్ణాంకం coords_నిడివి)
{
  hb_free (font->coords);
  hb_free (font->design_coords);

  font->coords = coords;
  font->design_coords = design_coords;
  font->అంకె_coords = coords_నిడివి;
  font->has_nonzero_coords = hb_any (hb_array (coords, coords_నిడివి));

  font->మారింది ();
  font->serial_coords = font->serial;
}

/**
 * hb_font_create_sub_font:
 * @parent: The parent font object
 *
 * Constructs a sub-font font object from the specified @parent font,
 * replicating the parent's properties.
 *
 * Return value: (transfer full): The new sub-font font object
 *
 * Since: 0.9.2
 **/
hb_font_t *
hb_font_create_sub_font (hb_font_t *మాతృ)
{
  ఒకవేళ (unlikely (!మాతృ))
    మాతృ = hb_font_get_ఖాళీలు ();

  hb_font_t *font = _hb_font_create (మాతృ->face);

  ఒకవేళ (unlikely (hb_object_is_immutable (font)))
    తిరిగి font;

  font->మాతృ = hb_font_reference (మాతృ);

  font->x_scale = మాతృ->x_scale;
  font->y_scale = మాతృ->y_scale;
  font->x_embolden = మాతృ->x_embolden;
  font->y_embolden = మాతృ->y_embolden;
  font->embolden_లోపల_ఉంచు = మాతృ->embolden_లోపల_ఉంచు;
  font->slant = మాతృ->slant;
  font->x_ppem = మాతృ->x_ppem;
  font->y_ppem = మాతృ->y_ppem;
  font->ptem = మాతృ->ptem;

  గుర్తులేని పూర్ణాంకం అంకె_coords = మాతృ->అంకె_coords;
  ఒకవేళ (అంకె_coords)
  {
    పూర్ణాంకం *coords = (పూర్ణాంకం *) hb_calloc (అంకె_coords, పరిమాణం (మాతృ->coords[0]));
    దశాంశం *design_coords = (దశాంశం *) hb_calloc (అంకె_coords, పరిమాణం (మాతృ->design_coords[0]));
    ఒకవేళ (likely (coords && design_coords))
    {
      hb_memcpy (coords, మాతృ->coords, అంకె_coords * పరిమాణం (మాతృ->coords[0]));
      hb_memcpy (design_coords, మాతృ->design_coords, అంకె_coords * పరిమాణం (మాతృ->design_coords[0]));
      _hb_font_adopt_var_coords (font, coords, design_coords, అంకె_coords);
    }
    లేకపోతే
    {
      hb_free (coords);
      hb_free (design_coords);
    }
  }

  font->మారింది ();
  font->serial_coords = font->serial;

  తిరిగి font;
}

/**
 * hb_font_get_empty:
 *
 * Fetches the empty font object.
 *
 * Return value: (transfer full): The empty font object
 *
 * Since: 0.9.2
 **/
hb_font_t *
hb_font_get_ఖాళీలు ()
{
  తిరిగి const_cast<hb_font_t *> (&Null (hb_font_t));
}

/**
 * hb_font_reference: (skip)
 * @font: #hb_font_t to work upon
 *
 * Increases the reference count on the given font object.
 *
 * Return value: (transfer full): The @font object
 *
 * Since: 0.9.2
 **/
hb_font_t *
hb_font_reference (hb_font_t *font)
{
  తిరిగి hb_object_reference (font);
}

/**
 * hb_font_destroy: (skip)
 * @font: #hb_font_t to work upon
 *
 * Decreases the reference count on the given font object. When the
 * reference count reaches zero, the font is destroyed,
 * freeing all memory.
 *
 * Since: 0.9.2
 **/
శూన్య
hb_font_destroy (hb_font_t *font)
{
  ఒకవేళ (!hb_object_destroy (font)) తిరిగి;

  font->దత్తాంశం.fini ();

  ఒకవేళ (font->destroy)
    font->destroy (font->user_దత్తాంశం);

  hb_font_destroy (font->మాతృ);
  hb_face_destroy (font->face);
  hb_font_funcs_destroy (font->klass);

  hb_free (font->coords);
  hb_free (font->design_coords);

  hb_free (font);
}

/**
 * hb_font_set_user_data: (skip)
 * @font: #hb_font_t to work upon
 * @key: The user-data key
 * @data: A pointer to the user data
 * @destroy: (nullable): A callback to call when @data is not needed anymore
 * @replace: Whether to replace an existing data with the same key
 *
 * Attaches a user-data key/data pair to the specified font object.
 *
 * Return value: `true` if success, `false` otherwise
 *
 * Since: 0.9.2
 **/
hb_bool_t
hb_font_సమితి_user_దత్తాంశం (hb_font_t          *font,
		       hb_user_దత్తాంశం_కీలకం_t *key,
		       శూన్య *              దత్తాంశం,
		       hb_destroy_func_t   destroy /* May be NULL. */,
		       hb_bool_t           replace)
{
  ఒకవేళ (!hb_object_is_immutable (font))
    font->మారింది ();

  తిరిగి hb_object_సమితి_user_దత్తాంశం (font, key, దత్తాంశం, destroy, replace);
}

/**
 * hb_font_get_user_data: (skip)
 * @font: #hb_font_t to work upon
 * @key: The user-data key to query
 *
 * Fetches the user-data object associated with the specified key,
 * attached to the specified font object.
 *
 * Return value: (transfer none): Pointer to the user data
 *
 * Since: 0.9.2
 **/
శూన్య *
hb_font_get_user_దత్తాంశం (స్థిరరాశి hb_font_t    *font,
		       hb_user_దత్తాంశం_కీలకం_t *key)
{
  తిరిగి hb_object_get_user_దత్తాంశం (font, key);
}

/**
 * hb_font_make_immutable:
 * @font: #hb_font_t to work upon
 *
 * Makes @font immutable.
 *
 * Since: 0.9.2
 **/
శూన్య
hb_font_make_immutable (hb_font_t *font)
{
  ఒకవేళ (hb_object_is_immutable (font))
    తిరిగి;

  ఒకవేళ (font->మాతృ)
    hb_font_make_immutable (font->మాతృ);

  hb_object_make_immutable (font);
}

/**
 * hb_font_is_immutable:
 * @font: #hb_font_t to work upon
 *
 * Tests whether a font object is immutable.
 *
 * Return value: `true` if @font is immutable, `false` otherwise
 *
 * Since: 0.9.2
 **/
hb_bool_t
hb_font_is_immutable (hb_font_t *font)
{
  తిరిగి hb_object_is_immutable (font);
}

/**
 * hb_font_get_serial:
 * @font: #hb_font_t to work upon
 *
 * Returns the internal serial number of the font. The serial
 * number is increased every time a setting on the font is
 * changed, using a setter function.
 *
 * Return value: serial number
 *
 * Since: 4.4.0
 **/
గుర్తులేని పూర్ణాంకం
hb_font_get_serial (hb_font_t *font)
{
  తిరిగి font->serial.get_acquire ();
}

/**
 * hb_font_changed:
 * @font: #hb_font_t to work upon
 *
 * Notifies the @font that underlying font data has changed.
 * This has the effect of increasing the serial as returned
 * by hb_font_get_serial(), which invalidates internal caches.
 *
 * Since: 4.4.0
 **/
శూన్య
hb_font_మారింది (hb_font_t *font)
{
  ఒకవేళ (hb_object_is_immutable (font))
    తిరిగి;

  font->మారింది ();
}

/**
 * hb_font_set_parent:
 * @font: #hb_font_t to work upon
 * @parent: The parent font object to assign
 *
 * Sets the parent font of @font.
 *
 * Since: 1.0.5
 **/
శూన్య
hb_font_సమితి_మాతృ (hb_font_t *font,
		    hb_font_t *మాతృ)
{
  ఒకవేళ (hb_object_is_immutable (font))
    తిరిగి;

  ఒకవేళ (మాతృ == font->మాతృ)
    తిరిగి;

  ఒకవేళ (!మాతృ)
    మాతృ = hb_font_get_ఖాళీలు ();

  కొరకు (hb_font_t *p = మాతృ; p && p != hb_font_get_ఖాళీలు(); p = p->మాతృ)
    ఒకవేళ (p == font)
      తిరిగి; /* Would create a cycle - reject */

  hb_font_t *old = font->మాతృ;

  font->మాతృ = hb_font_reference (మాతృ);

  hb_font_destroy (old);

  font->మారింది ();
}

/**
 * hb_font_get_parent:
 * @font: #hb_font_t to work upon
 *
 * Fetches the parent font of @font.
 *
 * Return value: (transfer none): The parent font object
 *
 * Since: 0.9.2
 **/
hb_font_t *
hb_font_get_మాతృ (hb_font_t *font)
{
  తిరిగి font->మాతృ;
}

#నిర్వచితం_కాకపోతే HB_DISABLE_DEPRECATED
/**
 * hb_font_set_face:
 * @font: #hb_font_t to work upon
 * @face: The #hb_face_t to assign
 *
 * Sets @face as the font-face value of @font.
 *
 * Font functions and their data may hold pointers into the old face,
 * so the font functions of @font are released and replaced with the
 * default ones for @face, as hb_font_create() would set up.  Other
 * font properties, such as scale and variations, are kept.
 *
 * Since: 1.4.3
 * XDeprecated: REPLACEME: Use hb_font_create() instead.
 **/
శూన్య
hb_font_సమితి_face (hb_font_t *font,
		  hb_face_t *face)
{
  ఒకవేళ (hb_object_is_immutable (font))
    తిరిగి;

  ఒకవేళ (face == font->face)
    తిరిగి;

  ఒకవేళ (unlikely (!face))
    face = hb_face_get_ఖాళీలు ();

  hb_face_t *old = font->face;

  hb_face_make_immutable (face);
  font->face = hb_face_reference (face);
  font->మారింది ();

  /* Release the old font funcs while the old face is still alive. */
  hb_font_సమితి_funcs (font, శూన్యసూచిక, శూన్యసూచిక, శూన్యసూచిక);
  hb_font_సమితి_funcs_using (font, శూన్యసూచిక);

  hb_face_destroy (old);

  font->మారింది ();
  font->serial_coords = font->serial;
}
#అయితే_ముగింపు

/**
 * hb_font_get_face:
 * @font: #hb_font_t to work upon
 *
 * Fetches the face associated with the specified font object.
 *
 * Return value: (transfer none): The #hb_face_t value
 *
 * Since: 0.9.2
 **/
hb_face_t *
hb_font_get_face (hb_font_t *font)
{
  తిరిగి font->face;
}


/**
 * hb_font_set_funcs:
 * @font: #hb_font_t to work upon
 * @klass: The font-functions structure.
 * @font_data: Data to attach to @font
 * @destroy: (nullable) (scope async) (closure font_data): The function to call when @font_data is not needed anymore
 *
 * Replaces the font-functions structure attached to a font, updating
 * the font's user-data with @font-data and the @destroy callback.
 *
 * Since: 0.9.2
 **/
శూన్య
hb_font_సమితి_funcs (hb_font_t         *font,
		   hb_font_funcs_t   *klass,
		   శూన్య              *font_దత్తాంశం,
		   hb_destroy_func_t  destroy /* May be NULL. */)
{
  ఒకవేళ (hb_object_is_immutable (font))
  {
    ఒకవేళ (destroy)
      destroy (font_దత్తాంశం);
    తిరిగి;
  }

  ఒకవేళ (font->destroy)
    font->destroy (font->user_దత్తాంశం);

  ఒకవేళ (!klass)
    klass = hb_font_funcs_get_ఖాళీలు ();

  hb_font_funcs_reference (klass);
  hb_font_funcs_destroy (font->klass);
  font->klass = klass;
  font->user_దత్తాంశం = font_దత్తాంశం;
  font->destroy = destroy;

  font->మారింది ();
}

/**
 * hb_font_set_funcs_data:
 * @font: #hb_font_t to work upon
 * @font_data: Data to attach to @font
 * @destroy: (nullable) (scope async) (closure font_data): The function to call when @font_data is not needed anymore
 *
 * Replaces the user data attached to a font, updating the font's
 * @destroy callback.
 *
 * Since: 0.9.2
 **/
శూన్య
hb_font_సమితి_funcs_దత్తాంశం (hb_font_t         *font,
		        శూన్య              *font_దత్తాంశం,
		        hb_destroy_func_t  destroy /* May be NULL. */)
{
  /* Destroy user_data? */
  ఒకవేళ (hb_object_is_immutable (font))
  {
    ఒకవేళ (destroy)
      destroy (font_దత్తాంశం);
    తిరిగి;
  }

  ఒకవేళ (font->destroy)
    font->destroy (font->user_దత్తాంశం);

  font->user_దత్తాంశం = font_దత్తాంశం;
  font->destroy = destroy;

  font->మారింది ();
}

స్థిర స్థిరరాశి నిర్మాణం supported_font_funcs_t {
	అక్షరం name[16];
	శూన్య (*func) (hb_font_t *);
} supported_font_funcs[] =
{
#నిర్వచితం_కాకపోతే HB_NO_OT_FONT
  {"ot",	hb_ot_font_సమితి_funcs},
#అయితే_ముగింపు
#నిర్వచితమైతే HAVE_FREETYPE
  {"ft",	hb_ft_font_సమితి_funcs},
#అయితే_ముగింపు
#నిర్వచితమైతే HAVE_FONTATIONS
  {"fontations",hb_fontations_font_సమితి_funcs},
#అయితే_ముగింపు
#నిర్వచితమైతే HAVE_CORETEXT
  {"coretext",	hb_coretext_font_సమితి_funcs},
#అయితే_ముగింపు
#నిర్వచితమైతే HAVE_DIRECTWRITE
  {"directwrite",hb_directwrite_font_సమితి_funcs},
#అయితే_ముగింపు
};

స్థిర స్థిరరాశి అక్షరం *get_default_funcs_name ()
{
  స్థిర hb_atomic_t<స్థిరరాశి అక్షరం *> static_funcs_name;
  స్థిరరాశి అక్షరం *name = static_funcs_name.get_acquire ();
  ఒకవేళ (!name)
  {
    name = getenv ("HB_FONT_FUNCS");
    ఒకవేళ (!name)
      name = "";
    ఒకవేళ (!static_funcs_name.cmpexch (శూన్యసూచిక, name))
      name = static_funcs_name.get_acquire ();
  }
  తిరిగి name;
}

/**
 * hb_font_set_funcs_using:
 * @font: #hb_font_t to work upon
 * @name: The name of the font-functions structure to use, or `NULL`
 *
 * Sets the font-functions structure to use for a font, based on the
 * specified name.
 *
 * If @name is `NULL` or the empty string, the default (first) functioning font-functions
 * are used.  This default can be changed by setting the `HB_FONT_FUNCS` environment
 * variable to the name of the desired font-functions.
 *
 * Return value: `true` if the font-functions was found and set, `false` otherwise
 *
 * Since: 11.0.0
 **/
hb_bool_t
hb_font_సమితి_funcs_using (hb_font_t  *font,
			 స్థిరరాశి అక్షరం *name)
{
  ఒకవేళ (unlikely (hb_object_is_immutable (font)))
    తిరిగి అబద్ధం;

  తార్కిక retry = అబద్ధం;

  ఒకవేళ (!name || !*name)
  {
    name = get_default_funcs_name ();
    retry = నిజం;
  }
  ఒకవేళ (name && !*name) name = శూన్యసూచిక;

retry:
  కొరకు (గుర్తులేని i = 0; i < ARRAY_నిడివి (supported_font_funcs); i++)
    ఒకవేళ (!name || strcmp (supported_font_funcs[i].name, name) == 0)
    {
      supported_font_funcs[i].func (font);
      ఒకవేళ (name || font->klass != hb_font_funcs_get_ఖాళీలు ())
	తిరిగి నిజం;
    }

  ఒకవేళ (retry)
  {
    retry = అబద్ధం;
    name = శూన్యసూచిక;
    వెళ్ళు retry;
  }

  తిరిగి అబద్ధం;
}

స్థిర అంతఃవరుస శూన్య free_static_font_funcs_list ();

స్థిర స్థిరరాశి అక్షరం * స్థిరరాశి nil_font_funcs_list[] = {శూన్యసూచిక};

స్థిర నిర్మాణం hb_font_funcs_list_lazy_loader_t : hb_lazy_loader_t<స్థిరరాశి అక్షరం *,
								  hb_font_funcs_list_lazy_loader_t>
{
  స్థిర స్థిరరాశి అక్షరం ** create ()
  {
    స్థిరరాశి అక్షరం **font_funcs_list = (స్థిరరాశి అక్షరం **) hb_calloc (1 + ARRAY_నిడివి (supported_font_funcs), పరిమాణం (స్థిరరాశి అక్షరం *));
    ఒకవేళ (unlikely (!font_funcs_list))
      తిరిగి శూన్యసూచిక;

    గుర్తులేని i;
    కొరకు (i = 0; i < ARRAY_నిడివి (supported_font_funcs); i++)
      font_funcs_list[i] = supported_font_funcs[i].name;
    font_funcs_list[i] = శూన్యసూచిక;

    hb_atexit (free_static_font_funcs_list);

    తిరిగి font_funcs_list;
  }
  స్థిర శూన్య destroy (స్థిరరాశి అక్షరం **l)
  { hb_free (l); }
  స్థిర స్థిరరాశి అక్షరం * స్థిరరాశి * get_null ()
  { తిరిగి nil_font_funcs_list; }
} static_font_funcs_list;

స్థిర అంతఃవరుస
శూన్య free_static_font_funcs_list ()
{
  static_font_funcs_list.free_instance ();
}

/**
 * hb_font_list_funcs:
 *
 * Retrieves the list of font functions supported by HarfBuzz.
 *
 * Return value: (transfer none) (array zero-terminated=1): a
 *    `NULL`-terminated array of supported font functions
 *    constant strings. The returned array is owned by HarfBuzz
 *    and should not be modified or freed.
 *
 * Since: 11.0.0
 **/
స్థిరరాశి అక్షరం **
hb_font_list_funcs ()
{
  తిరిగి static_font_funcs_list.get_unconst ();
}

/**
 * hb_font_set_scale:
 * @font: #hb_font_t to work upon
 * @x_scale: Horizontal scale value to assign
 * @y_scale: Vertical scale value to assign
 *
 * Sets the horizontal and vertical scale of a font.
 *
 * The font scale is a number related to, but not the same as,
 * font size. Typically the client establishes a scale factor
 * to be used between the two. For example, 64, or 256, which
 * would be the fractional-precision part of the font scale.
 * This is necessary because #hb_position_t values are integer
 * types and you need to leave room for fractional values
 * in there.
 *
 * For example, to set the font size to 20, with 64
 * levels of fractional precision you would call
 * `hb_font_set_scale(font, 20 * 64, 20 * 64)`.
 *
 * In the example above, even what font size 20 means is up to
 * you. It might be 20 pixels, or 20 points, or 20 millimeters.
 * HarfBuzz does not care about that.  You can set the point
 * size of the font using hb_font_set_ptem(), and the pixel
 * size using hb_font_set_ppem().
 *
 * The choice of scale is yours but needs to be consistent between
 * what you set here, and what you expect out of #hb_position_t
 * as well has draw / paint API output values.
 *
 * Fonts default to a scale equal to the UPEM value of their face.
 * A font with this setting is sometimes called an "unscaled" font.
 *
 * Since: 0.9.2
 **/
శూన్య
hb_font_సమితి_scale (hb_font_t *font,
		   పూర్ణాంకం        x_scale,
		   పూర్ణాంకం        y_scale)
{
  ఒకవేళ (hb_object_is_immutable (font))
    తిరిగి;

  ఒకవేళ (font->x_scale == x_scale && font->y_scale == y_scale)
    తిరిగి;

  font->x_scale = x_scale;
  font->y_scale = y_scale;

  font->మారింది ();
}

/**
 * hb_font_get_scale:
 * @font: #hb_font_t to work upon
 * @x_scale: (out): Horizontal scale value
 * @y_scale: (out): Vertical scale value
 *
 * Fetches the horizontal and vertical scale of a font.
 *
 * Since: 0.9.2
 **/
శూన్య
hb_font_get_scale (hb_font_t *font,
		   పూర్ణాంకం       *x_scale,
		   పూర్ణాంకం       *y_scale)
{
  ఒకవేళ (x_scale) *x_scale = font->x_scale;
  ఒకవేళ (y_scale) *y_scale = font->y_scale;
}

/**
 * hb_font_set_ppem:
 * @font: #hb_font_t to work upon
 * @x_ppem: Horizontal ppem value to assign
 * @y_ppem: Vertical ppem value to assign
 *
 * Sets the horizontal and vertical pixels-per-em (PPEM) of a font.
 *
 * These values are used for pixel-size-specific adjustment to
 * shaping and draw results, though for the most part they are
 * unused and can be left unset.
 *
 * Since: 0.9.2
 **/
శూన్య
hb_font_సమితి_ppem (hb_font_t    *font,
		  గుర్తులేని పూర్ణాంకం  x_ppem,
		  గుర్తులేని పూర్ణాంకం  y_ppem)
{
  ఒకవేళ (hb_object_is_immutable (font))
    తిరిగి;

  ఒకవేళ (font->x_ppem == x_ppem && font->y_ppem == y_ppem)
    తిరిగి;

  font->x_ppem = x_ppem;
  font->y_ppem = y_ppem;

  font->మారింది ();
}

/**
 * hb_font_get_ppem:
 * @font: #hb_font_t to work upon
 * @x_ppem: (out): Horizontal ppem value
 * @y_ppem: (out): Vertical ppem value
 *
 * Fetches the horizontal and vertical points-per-em (ppem) of a font.
 *
 * Since: 0.9.2
 **/
శూన్య
hb_font_get_ppem (hb_font_t    *font,
		  గుర్తులేని పూర్ణాంకం *x_ppem,
		  గుర్తులేని పూర్ణాంకం *y_ppem)
{
  ఒకవేళ (x_ppem) *x_ppem = font->x_ppem;
  ఒకవేళ (y_ppem) *y_ppem = font->y_ppem;
}

/**
 * hb_font_set_ptem:
 * @font: #hb_font_t to work upon
 * @ptem: font size in points.
 *
 * Sets the "point size" of a font. Set to zero to unset.
 * Used in CoreText to implement optical sizing.
 *
 * <note>Note: There are 72 points in an inch.</note>
 *
 * Since: 1.6.0
 **/
శూన్య
hb_font_సమితి_ptem (hb_font_t *font,
		  దశాంశం      ptem)
{
  ఒకవేళ (hb_object_is_immutable (font))
    తిరిగి;

  ఒకవేళ (font->ptem == ptem)
    తిరిగి;

  font->ptem = ptem;

  font->మారింది ();
}

/**
 * hb_font_get_ptem:
 * @font: #hb_font_t to work upon
 *
 * Fetches the "point size" of a font. Used in CoreText to
 * implement optical sizing.
 *
 * Return value: Point size.  A value of zero means "not set."
 *
 * Since: 1.6.0
 **/
దశాంశం
hb_font_get_ptem (hb_font_t *font)
{
  తిరిగి font->ptem;
}

/**
 * hb_font_is_synthetic:
 * @font: #hb_font_t to work upon
 *
 * Tests whether a font is synthetic. A synthetic font is one
 * that has either synthetic slant or synthetic bold set on it.
 *
 * Return value: `true` if the font is synthetic, `false` otherwise.
 *
 * Since: 11.2.0
 */
hb_bool_t
hb_font_is_synthetic (hb_font_t *font)
{
  తిరిగి font->is_synthetic;
}

/**
 * hb_font_set_synthetic_bold:
 * @font: #hb_font_t to work upon
 * @x_embolden: the amount to embolden horizontally
 * @y_embolden: the amount to embolden vertically
 * @in_place: whether to embolden glyphs in-place
 *
 * Sets the "synthetic boldness" of a font.
 *
 * Positive values for @x_embolden / @y_embolden make a font
 * bolder, negative values thinner. Typical values are in the
 * 0.01 to 0.05 range. The default value is zero.
 *
 * Synthetic boldness is applied by offsetting the contour
 * points of the glyph shape.
 *
 * Synthetic boldness is applied when rendering a glyph via
 * hb_font_draw_glyph_or_fail().
 *
 * If @in_place is `false`, then glyph advance-widths are also
 * adjusted, otherwise they are not.  The in-place mode is
 * useful for simulating [font grading](https://fonts.google.com/knowledge/glossary/grade).
 *
 *
 * Since: 7.0.0
 **/
శూన్య
hb_font_సమితి_synthetic_bold (hb_font_t *font,
			    దశాంశం x_embolden,
			    దశాంశం y_embolden,
			    hb_bool_t in_place)
{
  ఒకవేళ (hb_object_is_immutable (font))
    తిరిగి;

  ఒకవేళ (font->x_embolden == x_embolden &&
      font->y_embolden == y_embolden &&
      font->embolden_లోపల_ఉంచు == (తార్కిక) in_place)
    తిరిగి;

  font->x_embolden = x_embolden;
  font->y_embolden = y_embolden;
  font->embolden_లోపల_ఉంచు = in_place;

  font->మారింది ();
}

/**
 * hb_font_get_synthetic_bold:
 * @font: #hb_font_t to work upon
 * @x_embolden: (out): return location for horizontal value
 * @y_embolden: (out): return location for vertical value
 * @in_place: (out): return location for in-place value
 *
 * Fetches the "synthetic boldness" parameters of a font.
 *
 * Since: 7.0.0
 **/
శూన్య
hb_font_get_synthetic_bold (hb_font_t *font,
			    దశాంశం *x_embolden,
			    దశాంశం *y_embolden,
			    hb_bool_t *in_place)
{
  ఒకవేళ (x_embolden) *x_embolden = font->x_embolden;
  ఒకవేళ (y_embolden) *y_embolden = font->y_embolden;
  ఒకవేళ (in_place) *in_place = font->embolden_లోపల_ఉంచు;
}

/**
 * hb_font_set_synthetic_slant:
 * @font: #hb_font_t to work upon
 * @slant: synthetic slant value.
 *
 * Sets the "synthetic slant" of a font.  By default is zero.
 * Synthetic slant is the graphical skew applied to the font
 * at rendering time.
 *
 * HarfBuzz needs to know this value to adjust shaping results,
 * metrics, and style values to match the slanted rendering.
 *
 * <note>Note: The glyph shape fetched via the hb_font_draw_glyph_or_fail()
 * function is slanted to reflect this value as well.</note>
 *
 * <note>Note: The slant value is a ratio.  For example, a
 * 20% slant would be represented as a 0.2 value.</note>
 *
 * Since: 3.3.0
 **/
HB_EXTERN శూన్య
hb_font_సమితి_synthetic_slant (hb_font_t *font, దశాంశం slant)
{
  ఒకవేళ (hb_object_is_immutable (font))
    తిరిగి;

  ఒకవేళ (font->slant == slant)
    తిరిగి;

  font->slant = slant;

  font->మారింది ();
}

/**
 * hb_font_get_synthetic_slant:
 * @font: #hb_font_t to work upon
 *
 * Fetches the "synthetic slant" of a font.
 *
 * Return value: Synthetic slant.  By default is zero.
 *
 * Since: 3.3.0
 **/
HB_EXTERN దశాంశం
hb_font_get_synthetic_slant (hb_font_t *font)
{
  తిరిగి font->slant;
}

#నిర్వచితం_కాకపోతే HB_NO_VAR
/*
 * Variations
 */

/**
 * hb_font_set_variations:
 * @font: #hb_font_t to work upon
 * @variations: (array length=variations_length): Array of variation settings to apply
 * @variations_length: Number of variations to apply
 *
 * Applies a list of font-variation settings to a font.
 *
 * Note that this overrides all existing variations set on @font.
 * Axes not included in @variations will be effectively set to their
 * default values.
 *
 * Since: 1.4.2
 */
శూన్య
hb_font_సమితి_variations (hb_font_t            *font,
			స్థిరరాశి hb_variation_t *variations,
			గుర్తులేని పూర్ణాంకం          variations_నిడివి)
{
  ఒకవేళ (hb_object_is_immutable (font))
    తిరిగి;

  స్థిరరాశి OT::fvar &fvar = *font->face->పట్టిక.fvar;
  స్వయంచాలక axes = fvar.get_axes ();
  స్థిరరాశి గుర్తులేని coords_నిడివి = axes.నిడివి;

  పూర్ణాంకం *normalized = coords_నిడివి ? (పూర్ణాంకం *) hb_calloc (coords_నిడివి, పరిమాణం (పూర్ణాంకం)) : శూన్యసూచిక;
  దశాంశం *design_coords = coords_నిడివి ? (దశాంశం *) hb_calloc (coords_నిడివి, పరిమాణం (దశాంశం)) : శూన్యసూచిక;

  ఒకవేళ (unlikely (coords_నిడివి && !(normalized && design_coords)))
  {
    hb_free (normalized);
    hb_free (design_coords);
    తిరిగి;
  }

  /* Initialize design coords. */
  కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < coords_నిడివి; i++)
    design_coords[i] = axes[i].get_default ();
  ఒకవేళ (font->instance_సూచీ != HB_FONT_NO_VAR_NAMED_INSTANCE)
  {
    గుర్తులేని లెక్క = coords_నిడివి;
    /* This may fail if index is out-of-range;
     * That's why we initialize design_coords from fvar above
     * unconditionally. */
    hb_ot_var_named_instance_get_design_coords (font->face, font->instance_సూచీ,
						&లెక్క, design_coords);
  }

  కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < variations_నిడివి; i++)
  {
    స్థిరరాశి స్వయంచాలక ట్యాగ్_2 = variations[i].ట్యాగ్_2;
    స్థిరరాశి స్వయంచాలక v = variations[i].value;
    కొరకు (గుర్తులేని axis_సూచీ = 0; axis_సూచీ < coords_నిడివి; axis_సూచీ++)
      ఒకవేళ (axes[axis_సూచీ].axis_ట్యాగ్ == ట్యాగ్_2)
	design_coords[axis_సూచీ] = v;
  }

  hb_ot_var_normalize_coords (font->face, coords_నిడివి, design_coords, normalized);
  _hb_font_adopt_var_coords (font, normalized, design_coords, coords_నిడివి);
}

/**
 * hb_font_set_variation:
 * @font: #hb_font_t to work upon
 * @tag: The #hb_tag_t tag of the variation-axis name
 * @value: The value of the variation axis
 *
 * Change the value of one variation axis on the font.
 *
 * Note: This function is expensive to be called repeatedly.
 *   If you want to set multiple variation axes at the same time,
 *   use hb_font_set_variations() instead.
 *
 * Since: 7.1.0
 */
శూన్య
hb_font_సమితి_variation (hb_font_t *font,
		       hb_ట్యాగ్_t ట్యాగ్_2,
		       దశాంశం    value)
{
  ఒకవేళ (hb_object_is_immutable (font))
    తిరిగి;

  // TODO Share some of this code with set_variations()

  స్థిరరాశి OT::fvar &fvar = *font->face->పట్టిక.fvar;
  స్వయంచాలక axes = fvar.get_axes ();
  స్థిరరాశి గుర్తులేని coords_నిడివి = axes.నిడివి;

  పూర్ణాంకం *normalized = coords_నిడివి ? (పూర్ణాంకం *) hb_calloc (coords_నిడివి, పరిమాణం (పూర్ణాంకం)) : శూన్యసూచిక;
  దశాంశం *design_coords = coords_నిడివి ? (దశాంశం *) hb_calloc (coords_నిడివి, పరిమాణం (దశాంశం)) : శూన్యసూచిక;

  ఒకవేళ (unlikely (coords_నిడివి && !(normalized && design_coords)))
  {
    hb_free (normalized);
    hb_free (design_coords);
    తిరిగి;
  }

  /* Initialize design coords.
   * The font's coords may belong to another face after hb_font_set_face(). */
  ఒకవేళ (font->design_coords && font->అంకె_coords == coords_నిడివి)
  {
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < coords_నిడివి; i++)
      design_coords[i] = font->design_coords[i];
  }
  లేకపోతే
  {
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < coords_నిడివి; i++)
      design_coords[i] = axes[i].get_default ();
    ఒకవేళ (font->instance_సూచీ != HB_FONT_NO_VAR_NAMED_INSTANCE)
    {
      గుర్తులేని లెక్క = coords_నిడివి;
      /* This may fail if index is out-of-range;
       * That's why we initialize design_coords from fvar above
       * unconditionally. */
      hb_ot_var_named_instance_get_design_coords (font->face, font->instance_సూచీ,
						  &లెక్క, design_coords);
    }
  }

  కొరకు (గుర్తులేని axis_సూచీ = 0; axis_సూచీ < coords_నిడివి; axis_సూచీ++)
    ఒకవేళ (axes[axis_సూచీ].axis_ట్యాగ్ == ట్యాగ్_2)
      design_coords[axis_సూచీ] = value;

  hb_ot_var_normalize_coords (font->face, coords_నిడివి, design_coords, normalized);
  _hb_font_adopt_var_coords (font, normalized, design_coords, coords_నిడివి);
}

/**
 * hb_font_set_var_coords_design:
 * @font: #hb_font_t to work upon
 * @coords: (array length=coords_length): Array of variation coordinates to apply
 * @coords_length: Number of coordinates to apply
 *
 * Applies a list of variation coordinates (in design-space units)
 * to a font.
 *
 * Note that this overrides all existing variations set on @font.
 * Axes not included in @coords will be effectively set to their
 * default values.
 *
 * Since: 1.4.2
 */
శూన్య
hb_font_సమితి_var_coords_design (hb_font_t    *font,
			       స్థిరరాశి దశాంశం  *coords,
			       గుర్తులేని పూర్ణాంకం  ఆదానం_coords_నిడివి)
{
  ఒకవేళ (hb_object_is_immutable (font))
    తిరిగి;

  స్థిరరాశి OT::fvar &fvar = *font->face->పట్టిక.fvar;
  స్వయంచాలక axes = fvar.get_axes ();
  స్థిరరాశి గుర్తులేని coords_నిడివి = axes.నిడివి;

  ఆదానం_coords_నిడివి = hb_min (ఆదానం_coords_నిడివి, coords_నిడివి);
  పూర్ణాంకం *normalized = coords_నిడివి ? (పూర్ణాంకం *) hb_calloc (coords_నిడివి, పరిమాణం (పూర్ణాంకం)) : శూన్యసూచిక;
  దశాంశం *design_coords = coords_నిడివి ? (దశాంశం *) hb_calloc (coords_నిడివి, పరిమాణం (దశాంశం)) : శూన్యసూచిక;

  ఒకవేళ (unlikely (coords_నిడివి && !(normalized && design_coords)))
  {
    hb_free (normalized);
    hb_free (design_coords);
    తిరిగి;
  }

  ఒకవేళ (ఆదానం_coords_నిడివి)
    hb_memcpy (design_coords, coords, ఆదానం_coords_నిడివి * పరిమాణం (font->design_coords[0]));
  // Fill in the rest with default values
  కొరకు (గుర్తులేని పూర్ణాంకం i = ఆదానం_coords_నిడివి; i < coords_నిడివి; i++)
    design_coords[i] = axes[i].get_default ();

  hb_ot_var_normalize_coords (font->face, coords_నిడివి, design_coords, normalized);
  _hb_font_adopt_var_coords (font, normalized, design_coords, coords_నిడివి);
}

/**
 * hb_font_set_var_named_instance:
 * @font: a font.
 * @instance_index: named instance index.
 *
 * Sets design coords of a font from a named-instance index.
 *
 * Since: 2.6.0
 */
శూన్య
hb_font_సమితి_var_named_instance (hb_font_t *font,
				గుర్తులేని పూర్ణాంకం instance_సూచీ)
{
  ఒకవేళ (hb_object_is_immutable (font))
    తిరిగి;

  ఒకవేళ (font->instance_సూచీ == instance_సూచీ)
    తిరిగి;

  font->instance_సూచీ = instance_సూచీ;
  hb_font_సమితి_variations (font, శూన్యసూచిక, 0);
}

/**
 * hb_font_get_var_named_instance:
 * @font: a font.
 *
 * Returns the currently-set named-instance index of the font.
 *
 * Return value: Named-instance index or %HB_FONT_NO_VAR_NAMED_INSTANCE.
 *
 * Since: 7.0.0
 **/
గుర్తులేని పూర్ణాంకం
hb_font_get_var_named_instance (hb_font_t *font)
{
  తిరిగి font->instance_సూచీ;
}

/**
 * hb_font_set_var_coords_normalized:
 * @font: #hb_font_t to work upon
 * @coords: (array length=coords_length): Array of variation coordinates to apply
 * @coords_length: Number of coordinates to apply
 *
 * Applies a list of variation coordinates (in normalized units)
 * to a font.
 *
 * Note that this overrides all existing variations set on @font.
 * Axes not included in @coords will be effectively set to their
 * default values.
 *
 * <note>Note: Coordinates should be normalized to 2.14.</note>
 *
 * Since: 1.4.2
 */
శూన్య
hb_font_సమితి_var_coords_normalized (hb_font_t    *font,
				   స్థిరరాశి పూర్ణాంకం    *coords, /* 2.14 normalized */
				   గుర్తులేని పూర్ణాంకం  ఆదానం_coords_నిడివి)
{
  ఒకవేళ (hb_object_is_immutable (font))
    తిరిగి;

  స్థిరరాశి OT::fvar &fvar = *font->face->పట్టిక.fvar;
  స్వయంచాలక axes = fvar.get_axes ();
  గుర్తులేని coords_నిడివి = axes.నిడివి;

  ఆదానం_coords_నిడివి = hb_min (ఆదానం_coords_నిడివి, coords_నిడివి);
  పూర్ణాంకం *copy = coords_నిడివి ? (పూర్ణాంకం *) hb_calloc (coords_నిడివి, పరిమాణం (coords[0])) : శూన్యసూచిక;
  దశాంశం *design_coords = coords_నిడివి ? (దశాంశం *) hb_calloc (coords_నిడివి, పరిమాణం (design_coords[0])) : శూన్యసూచిక;

  ఒకవేళ (unlikely (coords_నిడివి && !(copy && design_coords)))
  {
    hb_free (copy);
    hb_free (design_coords);
    తిరిగి;
  }

  ఒకవేళ (ఆదానం_coords_నిడివి)
    hb_memcpy (copy, coords, ఆదానం_coords_నిడివి * పరిమాణం (coords[0]));

  కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < coords_నిడివి; ++i)
    design_coords[i] = NAN;

  _hb_font_adopt_var_coords (font, copy, design_coords, coords_నిడివి);
}

/**
 * hb_font_get_var_coords_normalized:
 * @font: #hb_font_t to work upon
 * @length: (out): Number of coordinates retrieved
 *
 * Fetches the list of normalized variation coordinates currently
 * set on a font.
 *
 * <note>Note that if no variation coordinates are set, this function may
 * return %NULL.</note>
 *
 * Return value is valid as long as variation coordinates of the font
 * are not modified.
 *
 * Return value: coordinates array
 *
 * Since: 1.4.2
 */
స్థిరరాశి పూర్ణాంకం *
hb_font_get_var_coords_normalized (hb_font_t    *font,
				   గుర్తులేని పూర్ణాంకం *నిడివి)
{
  ఒకవేళ (నిడివి)
    *నిడివి = font->అంకె_coords;

  తిరిగి font->coords;
}

/**
 * hb_font_get_var_coords_design:
 * @font: #hb_font_t to work upon
 * @length: (out): Number of coordinates retrieved
 *
 * Fetches the list of variation coordinates (in design-space units) currently
 * set on a font.
 *
 * <note>Note that if no variation coordinates are set, this function may
 * return %NULL.</note>
 *
 * <note>If variations have been set on the font using normalized coordinates
 * (i.e. via hb_font_set_var_coords_normalized()), the design coordinates will
 * have NaN (Not a Number) values.</note>
 *
 * Return value is valid as long as variation coordinates of the font
 * are not modified.
 *
 * Return value: coordinates array
 *
 * Since: 3.3.0
 */
స్థిరరాశి దశాంశం *
hb_font_get_var_coords_design (hb_font_t *font,
			       గుర్తులేని పూర్ణాంకం *నిడివి)
{
  ఒకవేళ (నిడివి)
    *నిడివి = font->అంకె_coords;

  తిరిగి font->design_coords;
}
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే HB_DISABLE_DEPRECATED
/*
 * Deprecated get_glyph_func():
 */

నిర్మాణం hb_trampoline_సంవృతం_t
{
  శూన్య *user_దత్తాంశం;
  hb_destroy_func_t destroy;
  గుర్తులేని పూర్ణాంకం ref_లెక్క_2;
};

మూస <రకం_పేరు FuncType>
నిర్మాణం hb_trampoline_t
{
  hb_trampoline_సంవృతం_t సంవృతం; /* Must be first. */
  FuncType func;
};

మూస <రకం_పేరు FuncType>
స్థిర hb_trampoline_t<FuncType> *
trampoline_create (FuncType           func,
		   శూన్య              *user_దత్తాంశం,
		   hb_destroy_func_t  destroy)
{
  రకనిర్వచనం hb_trampoline_t<FuncType> trampoline_t;

  trampoline_t *trampoline = (trampoline_t *) hb_calloc (1, పరిమాణం (trampoline_t));

  ఒకవేళ (unlikely (!trampoline))
    తిరిగి శూన్యసూచిక;

  trampoline->సంవృతం.user_దత్తాంశం = user_దత్తాంశం;
  trampoline->సంవృతం.destroy = destroy;
  trampoline->సంవృతం.ref_లెక్క_2 = 1;
  trampoline->func = func;

  తిరిగి trampoline;
}

స్థిర శూన్య
trampoline_reference (hb_trampoline_సంవృతం_t *సంవృతం)
{
  సంవృతం->ref_లెక్క_2++;
}

స్థిర శూన్య
trampoline_destroy (శూన్య *user_దత్తాంశం)
{
  hb_trampoline_సంవృతం_t *సంవృతం = (hb_trampoline_సంవృతం_t *) user_దత్తాంశం;

  ఒకవేళ (--సంవృతం->ref_లెక్క_2)
    తిరిగి;

  ఒకవేళ (సంవృతం->destroy)
    సంవృతం->destroy (సంవృతం->user_దత్తాంశం);
  hb_free (సంవృతం);
}

రకనిర్వచనం hb_trampoline_t<hb_font_get_glyph_func_t> hb_font_get_glyph_trampoline_t;

స్థిర hb_bool_t
hb_font_get_nominal_glyph_trampoline (hb_font_t      *font,
				      శూన్య           *font_దత్తాంశం,
				      hb_codepoint_t  unicode,
				      hb_codepoint_t *glyph,
				      శూన్య           *user_దత్తాంశం)
{
  hb_font_get_glyph_trampoline_t *trampoline = (hb_font_get_glyph_trampoline_t *) user_దత్తాంశం;
  తిరిగి trampoline->func (font, font_దత్తాంశం, unicode, 0, glyph, trampoline->సంవృతం.user_దత్తాంశం);
}

స్థిర hb_bool_t
hb_font_get_variation_glyph_trampoline (hb_font_t      *font,
					శూన్య           *font_దత్తాంశం,
					hb_codepoint_t  unicode,
					hb_codepoint_t  variation_selector,
					hb_codepoint_t *glyph,
					శూన్య           *user_దత్తాంశం)
{
  hb_font_get_glyph_trampoline_t *trampoline = (hb_font_get_glyph_trampoline_t *) user_దత్తాంశం;
  తిరిగి trampoline->func (font, font_దత్తాంశం, unicode, variation_selector, glyph, trampoline->సంవృతం.user_దత్తాంశం);
}

/**
 * hb_font_funcs_set_glyph_func:
 * @ffuncs: The font-functions structure
 * @func: (closure user_data) (destroy destroy) (scope notified): callback function
 * @user_data: data to pass to @func
 * @destroy: (nullable): function to call when @user_data is not needed anymore
 *
 * Deprecated.  Use hb_font_funcs_set_nominal_glyph_func() and
 * hb_font_funcs_set_variation_glyph_func() instead.
 *
 * Since: 0.9.2
 * Deprecated: 1.2.3
 **/
శూన్య
hb_font_funcs_సమితి_glyph_func (hb_font_funcs_t          *ffuncs,
			      hb_font_get_glyph_func_t  func,
			      శూన్య                     *user_దత్తాంశం,
			      hb_destroy_func_t         destroy /* May be NULL. */)
{
  ఒకవేళ (hb_object_is_immutable (ffuncs))
  {
    ఒకవేళ (destroy)
      destroy (user_దత్తాంశం);
    తిరిగి;
  }

  hb_font_get_glyph_trampoline_t *trampoline;

  trampoline = trampoline_create (func, user_దత్తాంశం, destroy);
  ఒకవేళ (unlikely (!trampoline))
  {
    ఒకవేళ (destroy)
      destroy (user_దత్తాంశం);
    తిరిగి;
  }

  /* Since we pass it to two destroying functions. */
  trampoline_reference (&trampoline->సంవృతం);

  hb_font_funcs_సమితి_nominal_glyph_func (ffuncs,
					hb_font_get_nominal_glyph_trampoline,
					trampoline,
					trampoline_destroy);

  hb_font_funcs_సమితి_variation_glyph_func (ffuncs,
					  hb_font_get_variation_glyph_trampoline,
					  trampoline,
					  trampoline_destroy);
}
#అయితే_ముగింపు


#నిర్వచితం_కాకపోతే HB_DISABLE_DEPRECATED

నిర్మాణం hb_draw_glyph_సంవృతం_t
{
  hb_font_draw_glyph_func_t func;
  శూన్య *user_దత్తాంశం;
  hb_destroy_func_t destroy;
};
స్థిర hb_bool_t
hb_font_draw_glyph_trampoline (hb_font_t       *font,
			       శూన్య            *font_దత్తాంశం,
			       hb_codepoint_t   glyph,
			       hb_draw_funcs_t *draw_funcs,
			       శూన్య            *draw_దత్తాంశం,
			       శూన్య            *user_దత్తాంశం)
{
  hb_draw_glyph_సంవృతం_t *సంవృతం = (hb_draw_glyph_సంవృతం_t *) user_దత్తాంశం;
  సంవృతం->func (font, font_దత్తాంశం, glyph, draw_funcs, draw_దత్తాంశం, సంవృతం->user_దత్తాంశం);
  తిరిగి నిజం;
}
స్థిర శూన్య
hb_font_draw_glyph_సంవృతం_destroy (శూన్య *user_దత్తాంశం)
{
  hb_draw_glyph_సంవృతం_t *సంవృతం = (hb_draw_glyph_సంవృతం_t *) user_దత్తాంశం;

  ఒకవేళ (సంవృతం->destroy)
    సంవృతం->destroy (సంవృతం->user_దత్తాంశం);
  hb_free (సంవృతం);
}
స్థిర శూన్య
hb_font_funcs_సమితి_draw_glyph_func_2 (hb_font_funcs_t           *ffuncs,
				    hb_font_draw_glyph_func_t  func,
				    శూన్య                      *user_దత్తాంశం,
				    hb_destroy_func_t          destroy /* May be NULL. */)
{
  ఒకవేళ (hb_object_is_immutable (ffuncs))
  {
    ఒకవేళ (destroy)
      destroy (user_దత్తాంశం);
    తిరిగి;
  }
  hb_draw_glyph_సంవృతం_t *సంవృతం = (hb_draw_glyph_సంవృతం_t *) hb_calloc (1, పరిమాణం (hb_draw_glyph_సంవృతం_t));
  ఒకవేళ (unlikely (!సంవృతం))
  {
    ఒకవేళ (destroy)
      destroy (user_దత్తాంశం);
    తిరిగి;
  }
  సంవృతం->func = func;
  సంవృతం->user_దత్తాంశం = user_దత్తాంశం;
  సంవృతం->destroy = destroy;

  hb_font_funcs_సమితి_draw_glyph_or_fail_func (ffuncs,
					     hb_font_draw_glyph_trampoline,
					     సంవృతం,
					     hb_font_draw_glyph_సంవృతం_destroy);
}
శూన్య
hb_font_funcs_సమితి_draw_glyph_func (hb_font_funcs_t           *ffuncs,
                                   hb_font_draw_glyph_func_t  func,
                                   శూన్య                      *user_దత్తాంశం,
                                   hb_destroy_func_t          destroy /* May be NULL. */)
{
  hb_font_funcs_సమితి_draw_glyph_func_2 (ffuncs, func, user_దత్తాంశం, destroy);
}
శూన్య
hb_font_funcs_సమితి_glyph_shape_func (hb_font_funcs_t               *ffuncs,
                                   hb_font_get_glyph_shape_func_t  func,
                                   శూన్య                           *user_దత్తాంశం,
                                   hb_destroy_func_t               destroy /* May be NULL. */)
{
  hb_font_funcs_సమితి_draw_glyph_func_2 (ffuncs, func, user_దత్తాంశం, destroy);
}

నిర్మాణం hb_paint_glyph_సంవృతం_t
{
  hb_font_paint_glyph_func_t func;
  శూన్య *user_దత్తాంశం;
  hb_destroy_func_t destroy;
};
స్థిర hb_bool_t
hb_font_paint_glyph_trampoline (hb_font_t        *font,
				శూన్య *font_దత్తాంశం,
				hb_codepoint_t glyph,
				hb_paint_funcs_t *paint_funcs,
				శూన్య *paint_దత్తాంశం,
				గుర్తులేని పూర్ణాంకం palette,
				hb_color_t foreground,
				శూన్య *user_దత్తాంశం)
{
  hb_paint_glyph_సంవృతం_t *సంవృతం = (hb_paint_glyph_సంవృతం_t *) user_దత్తాంశం;
  సంవృతం->func (font, font_దత్తాంశం, glyph, paint_funcs, paint_దత్తాంశం, palette, foreground, సంవృతం->user_దత్తాంశం);
  తిరిగి నిజం;
}
స్థిర శూన్య
hb_font_paint_glyph_సంవృతం_destroy (శూన్య *user_దత్తాంశం)
{
  hb_paint_glyph_సంవృతం_t *సంవృతం = (hb_paint_glyph_సంవృతం_t *) user_దత్తాంశం;

  ఒకవేళ (సంవృతం->destroy)
    సంవృతం->destroy (సంవృతం->user_దత్తాంశం);
  hb_free (సంవృతం);
}
శూన్య
hb_font_funcs_సమితి_paint_glyph_func (hb_font_funcs_t           *ffuncs,
				    hb_font_paint_glyph_func_t  func,
				    శూన్య                      *user_దత్తాంశం,
				    hb_destroy_func_t          destroy /* May be NULL. */)
{
  ఒకవేళ (hb_object_is_immutable (ffuncs))
  {
    ఒకవేళ (destroy)
      destroy (user_దత్తాంశం);
    తిరిగి;
  }
  hb_paint_glyph_సంవృతం_t *సంవృతం = (hb_paint_glyph_సంవృతం_t *) hb_calloc (1, పరిమాణం (hb_paint_glyph_సంవృతం_t));
  ఒకవేళ (unlikely (!సంవృతం))
  {
    ఒకవేళ (destroy)
      destroy (user_దత్తాంశం);
    తిరిగి;
  }
  సంవృతం->func = func;
  సంవృతం->user_దత్తాంశం = user_దత్తాంశం;
  సంవృతం->destroy = destroy;

  hb_font_funcs_సమితి_paint_glyph_or_fail_func (ffuncs,
					      hb_font_paint_glyph_trampoline,
					      సంవృతం,
					      hb_font_paint_glyph_సంవృతం_destroy);
}
#అయితే_ముగింపు
