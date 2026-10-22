/*
 * Copyright © 2009  Red Hat, Inc.
 * Copyright © 2009  Keith Stribley
 * Copyright © 2015  Google, Inc.
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

#నిర్వచితమైతే HAVE_FREETYPE

#అన్తర్భావ "hb-ft.h"

#అన్తర్భావ "hb-cache.hh"
#అన్తర్భావ "hb-draw.hh"
#అన్తర్భావ "hb-font.hh"
#అన్తర్భావ "hb-machinery.hh"
#అన్తర్భావ "hb-ot-os2-table.hh"
#అన్తర్భావ "hb-ot-shaper-arabic-pua.hh"
#అన్తర్భావ "hb-paint.hh"

#అన్తర్భావ FT_MODULE_H
#అన్తర్భావ FT_ADVANCES_H
#అన్తర్భావ FT_గుణిజం_MASTERS_H
#అన్తర్భావ FT_OUTLINE_H
#అన్తర్భావ FT_TRUETYPE_TABLES_H
#అన్తర్భావ FT_SYNTHESIS_H
#ఒకవేళ (FREETYPE_MAJOR*10000 + FREETYPE_MINOR*100 + FREETYPE_PATCH) >= 21300
#అన్తర్భావ FT_COLOR_H
#అయితే_ముగింపు


/**
 * SECTION:hb-ft
 * @title: hb-ft
 * @short_description: FreeType integration
 * @include: hb-ft.h
 *
 * Functions for using HarfBuzz with the FreeType library.
 *
 * HarfBuzz supports using FreeType to provide face and
 * font data.
 *
 * <note>Note that FreeType is not thread-safe, therefore these
 * functions are not thread-safe either.</note>
 **/


/* TODO:
 *
 * In general, this file does a fine job of what it's supposed to do.
 * There are, however, things that need more work:
 *
 *   - FreeType works in 26.6 mode.  Clients can decide to use that mode, and everything
 *     would work fine.  However, we also abuse this API for performing in font-space,
 *     but don't pass the correct flags to FreeType.  We just abuse the no-hinting mode
 *     for that, such that no rounding etc happens.  As such, we don't set ppem, and
 *     pass NO_HINTING as load_flags.  Would be much better to use NO_SCALE, and scale
 *     ourselves.
 *
 *   - We don't handle / allow for emboldening / obliqueing.
 *
 *   - In the future, we should add constructors to create fonts in font space?
 */


ఉపయోగించి hb_ft_advance_cache_t = hb_cache_t<16, 24, 8, అబద్ధం>;

స్థిర శూన్య _hb_ft_face_destroy_static (శూన్య *దత్తాంశం);

నిర్మాణం hb_ft_font_t
{
  పూర్ణాంకం ఎక్కించు_flags;
  తార్కిక చిహ్నం; /* Whether selected cmap is symbol cmap. */
  తార్కిక unref; /* Whether to destroy ft_face when done. */
  తార్కిక transform; /* Whether to apply FT_Face's transform. */
  తార్కిక static_library; /* Whether ft_face uses static_ft_library. */

  మార్చదగిన hb_mutex_t lock; /* Protects members below. */
  FT_Face ft_face;
  మార్చదగిన hb_atomic_t<గుర్తులేని> cached_serial;
  మార్చదగిన hb_ft_advance_cache_t advance_cache;
};

స్థిర hb_ft_font_t *
_hb_ft_font_create (FT_Face ft_face, తార్కిక చిహ్నం, తార్కిక unref, తార్కిక static_library)
{
  hb_ft_font_t *ft_font = (hb_ft_font_t *) hb_calloc (1, పరిమాణం (hb_ft_font_t));
  ఒకవేళ (unlikely (!ft_font)) తిరిగి శూన్యసూచిక;

  ft_font->lock.init ();
  ft_font->ft_face = ft_face;
  ft_font->చిహ్నం = చిహ్నం;
  ft_font->unref = unref;
  ft_font->static_library = static_library;

  ft_font->ఎక్కించు_flags = FT_ఎక్కించు_DEFAULT | FT_ఎక్కించు_NO_HINTING;

  ft_font->cached_serial = UINT_MAX;
  కొత్త (&ft_font->advance_cache) hb_ft_advance_cache_t;

  తిరిగి ft_font;
}

స్థిర శూన్య
_hb_ft_face_destroy (శూన్య *దత్తాంశం)
{
  FT_పూర్తి_Face ((FT_Face) దత్తాంశం);
}

స్థిర శూన్య
_hb_ft_font_destroy (శూన్య *దత్తాంశం)
{
  hb_ft_font_t *ft_font = (hb_ft_font_t *) దత్తాంశం;

  ఒకవేళ (ft_font->unref)
  {
    ఒకవేళ (ft_font->static_library)
    {
      /* ft_font->ft_face->generic.data is the blob that hb_ft_font_set_funcs()
       * hooked up as this internal FT_Face's finalizer target (_release_blob).
       * FT_Done_Face() below runs that finalizer while static_ft_library_mutex
       * is held; if dropping its reference were to free the blob right there,
       * the blob's attached destroy_ft_library() user-data callback would try
       * to re-lock the same non-recursive mutex and self-deadlock. Hold an
       * extra reference across the locked call so the blob (and thus that
       * callback) can only be freed after the lock is released. */
      hb_blob_t *blob = hb_blob_reference ((hb_blob_t *) ft_font->ft_face->generic.దత్తాంశం);
      _hb_ft_face_destroy_static (ft_font->ft_face);
      hb_blob_destroy (blob);
    }
    లేకపోతే
      _hb_ft_face_destroy (ft_font->ft_face);
  }

  ft_font->lock.fini ();

  hb_free (ft_font);
}


/* hb_font changed, update FT_Face. */
స్థిర శూన్య hb_ft_hb_font_మారింది (hb_font_t *font, FT_Face ft_face)
{
  ఒకవేళ (unlikely (font->destroy != (hb_destroy_func_t) _hb_ft_font_destroy))
    తిరిగి;

  hb_ft_font_t *ft_font = (hb_ft_font_t *) font->user_దత్తాంశం;

  దశాంశం x_mult = 1.f, y_mult = 1.f;

  ఒకవేళ (font->x_scale < 0) x_mult = -x_mult;
  ఒకవేళ (font->y_scale < 0) y_mult = -y_mult;

  ఒకవేళ (FT_సమితి_Char_కొలత (ft_face,
			నిరపేక్ష (font->x_scale), నిరపేక్ష (font->y_scale),
			0, 0
#ఒకవేళ 0
		    font->x_ppem * 72 * 64 / font->x_scale,
		    font->y_ppem * 72 * 64 / font->y_scale
#అయితే_ముగింపు
     ) && ft_face->అంకె_fixed_sizes)
  {
#నిర్వచితమైతే HAVE_FT_GET_TRANSFORM
    /* Bitmap font, eg. bitmap color emoji. */
    /* Pick largest size? */
    పూర్ణాంకం x_scale  = ft_face->అందుబాటు_sizes[ft_face->అంకె_fixed_sizes - 1].x_ppem;
    పూర్ణాంకం y_scale = ft_face->అందుబాటు_sizes[ft_face->అంకె_fixed_sizes - 1].y_ppem;
    FT_సమితి_Char_కొలత (ft_face,
		      x_scale, y_scale,
		      0, 0);

    /* This contains the sign that was previously in x_mult/y_mult. */
    x_mult = (దశాంశం) font->x_scale / x_scale;
    y_mult = (దశాంశం) font->y_scale / y_scale;
#అయితే_ముగింపు
  }
  లేకపోతే
  { /* Shrug */ }


  ఒకవేళ (x_mult != 1.f || y_mult != 1.f)
  {
    FT_మాతృక మాతృక = { (పూర్ణాంకం) roundf (x_mult * (1<<16)), 0,
			  0, (పూర్ణాంకం) roundf (y_mult * (1<<16))};
    FT_సమితి_Transform (ft_face, &మాతృక, శూన్యసూచిక);
    ft_font->transform = నిజం;
  }
  లేకపోతే
    FT_సమితి_Transform (ft_face, శూన్యసూచిక, శూన్యసూచిక);

#ఒకవేళ defined(HAVE_FT_GET_VAR_BLEND_COORDINATES) && !defined(HB_NO_VAR)
  ఒకవేళ (font->has_nonzero_coords)
  {
    గుర్తులేని పూర్ణాంకం అంకె_coords;
    స్థిరరాశి దశాంశం *coords = hb_font_get_var_coords_design (font, &అంకె_coords);
    FT_Fixed *ft_coords = (FT_Fixed *) hb_calloc (అంకె_coords, పరిమాణం (FT_Fixed));
    ఒకవేళ (ft_coords)
    {
      కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < అంకె_coords; i++)
	  ft_coords[i] = coords[i] * 65536.f;
      FT_సమితి_Var_Design_Coordinates (ft_face, అంకె_coords, ft_coords);
      hb_free (ft_coords);
    }
  }
  లేకపోతే ఒకవేళ (font->అంకె_coords)
  {
    // Some old versions of FreeType crash if we
    // call this function on non-variable fonts.
    FT_సమితి_Var_Design_Coordinates (ft_face, 0, శూన్యసూచిక);
  }
#అయితే_ముగింపు
}

/* Check if hb_font changed, update FT_Face. */
స్థిర అంతఃవరుస తార్కిక
hb_ft_hb_font_check_మారింది (hb_font_t *font,
			      స్థిరరాశి hb_ft_font_t *ft_font)
{
  ఒకవేళ (font->serial != ft_font->cached_serial)
  {
    hb_తాళం_t lock (ft_font->lock);
    hb_ft_hb_font_మారింది (font, ft_font->ft_face);
    ft_font->advance_cache.clear ();
    ft_font->cached_serial.సమితి_release (font->serial.get_acquire ());
    తిరిగి నిజం;
  }
  తిరిగి అబద్ధం;
}


/**
 * hb_ft_font_set_load_flags:
 * @font: #hb_font_t to work upon
 * @load_flags: The FreeType load flags to set
 *
 * Sets the FT_Load_Glyph load flags for the specified #hb_font_t.
 *
 * For more information, see
 * <https://freetype.org/freetype2/docs/reference/ft2-glyph_retrieval.html#ft_load_xxx>
 *
 * This function works with #hb_font_t objects created by
 * hb_ft_font_create() or hb_ft_font_create_referenced().
 *
 * Since: 1.0.5
 **/
శూన్య
hb_ft_font_సమితి_ఎక్కించు_flags (hb_font_t *font, పూర్ణాంకం ఎక్కించు_flags)
{
  ఒకవేళ (hb_object_is_immutable (font))
    తిరిగి;

  ఒకవేళ (unlikely (font->destroy != (hb_destroy_func_t) _hb_ft_font_destroy))
    తిరిగి;

  hb_ft_font_t *ft_font = (hb_ft_font_t *) font->user_దత్తాంశం;

  ft_font->ఎక్కించు_flags = ఎక్కించు_flags;
}

/**
 * hb_ft_font_get_load_flags:
 * @font: #hb_font_t to work upon
 *
 * Fetches the FT_Load_Glyph load flags of the specified #hb_font_t.
 *
 * For more information, see
 * <https://freetype.org/freetype2/docs/reference/ft2-glyph_retrieval.html#ft_load_xxx>
 *
 * This function works with #hb_font_t objects created by
 * hb_ft_font_create() or hb_ft_font_create_referenced().
 *
 * Return value: FT_Load_Glyph flags found, or 0
 *
 * Since: 1.0.5
 **/
పూర్ణాంకం
hb_ft_font_get_ఎక్కించు_flags (hb_font_t *font)
{
  ఒకవేళ (unlikely (font->destroy != (hb_destroy_func_t) _hb_ft_font_destroy))
    తిరిగి 0;

  స్థిరరాశి hb_ft_font_t *ft_font = (స్థిరరాశి hb_ft_font_t *) font->user_దత్తాంశం;

  తిరిగి ft_font->ఎక్కించు_flags;
}

/**
 * hb_ft_font_get_ft_face: (skip)
 * @font: #hb_font_t to work upon
 *
 * Fetches the FT_Face associated with the specified #hb_font_t
 * font object.
 *
 * This function works with #hb_font_t objects created by
 * hb_ft_font_create() or hb_ft_font_create_referenced().
 *
 * Return value: (nullable): the FT_Face found or `NULL`
 *
 * Since: 10.4.0
 **/
FT_Face
hb_ft_font_get_ft_face (hb_font_t *font)
{
  ఒకవేళ (unlikely (font->destroy != (hb_destroy_func_t) _hb_ft_font_destroy))
    తిరిగి శూన్యసూచిక;

  స్థిరరాశి hb_ft_font_t *ft_font = (స్థిరరాశి hb_ft_font_t *) font->user_దత్తాంశం;

  తిరిగి ft_font->ft_face;
}

#నిర్వచితం_కాకపోతే HB_DISABLE_DEPRECATED

/**
 * hb_ft_font_get_face: (skip)
 * @font: #hb_font_t to work upon
 *
 * Fetches the FT_Face associated with the specified #hb_font_t
 * font object.
 *
 * This function works with #hb_font_t objects created by
 * hb_ft_font_create() or hb_ft_font_create_referenced().
 *
 * Return value: (nullable): the FT_Face found or `NULL`
 *
 * Since: 0.9.2
 * Deprecated: 10.4.0: Use hb_ft_font_get_ft_face() instead.
 **/
FT_Face
hb_ft_font_get_face (hb_font_t *font)
{
  తిరిగి hb_ft_font_get_ft_face (font);
}

#అయితే_ముగింపు

/**
 * hb_ft_font_lock_face: (skip)
 * @font: #hb_font_t to work upon
 *
 * Gets the FT_Face associated with @font.
 *
 * This face will be kept around and access to the FT_Face object
 * from other HarfBuzz API wil be blocked until you call hb_ft_font_unlock_face().
 *
 * This function works with #hb_font_t objects created by
 * hb_ft_font_create() or hb_ft_font_create_referenced().
 *
 * Return value: (nullable) (transfer none): the FT_Face associated with @font or `NULL`
 * Since: 2.6.5
 **/
FT_Face
hb_ft_font_తాళం_face (hb_font_t *font)
{
  ఒకవేళ (unlikely (font->destroy != (hb_destroy_func_t) _hb_ft_font_destroy))
    తిరిగి శూన్యసూచిక;

  స్థిరరాశి hb_ft_font_t *ft_font = (స్థిరరాశి hb_ft_font_t *) font->user_దత్తాంశం;

  ft_font->lock.lock ();

  తిరిగి ft_font->ft_face;
}

/**
 * hb_ft_font_unlock_face: (skip)
 * @font: #hb_font_t to work upon
 *
 * Releases an FT_Face previously obtained with hb_ft_font_lock_face().
 *
 * Since: 2.6.5
 **/
శూన్య
hb_ft_font_unlock_face (hb_font_t *font)
{
  ఒకవేళ (unlikely (font->destroy != (hb_destroy_func_t) _hb_ft_font_destroy))
    తిరిగి;

  స్థిరరాశి hb_ft_font_t *ft_font = (స్థిరరాశి hb_ft_font_t *) font->user_దత్తాంశం;

  ft_font->lock.unlock ();
}


స్థిర hb_bool_t
hb_ft_get_nominal_glyph (hb_font_t *font,
			 శూన్య *font_దత్తాంశం,
			 hb_codepoint_t unicode,
			 hb_codepoint_t *glyph,
			 శూన్య *user_దత్తాంశం HB_వాడని)
{
  స్థిరరాశి hb_ft_font_t *ft_font = (స్థిరరాశి hb_ft_font_t *) font_దత్తాంశం;
  hb_తాళం_t lock (ft_font->lock);
  గుర్తులేని పూర్ణాంకం g = FT_Get_Char_సూచీ (ft_font->ft_face, unicode);

  ఒకవేళ (unlikely (!g))
  {
    ఒకవేళ (unlikely (ft_font->చిహ్నం))
    {
      ఎంపిక ((గుర్తులేని) font->face->పట్టిక.OS2->get_font_పుట ()) {
      స్థితి OT::OS2::font_పుట_t::FONT_పుట_NONE:
	ఒకవేళ (unicode <= 0x00FFu)
	  /* For symbol-encoded OpenType fonts, we duplicate the
	   * U+F000..F0FF range at U+0000..U+00FF.  That's what
	   * Windows seems to do, and that's hinted about at:
	   * https://docs.microsoft.com/en-us/typography/opentype/spec/recom
	   * under "Non-Standard (Symbol) Fonts". */
	  g = FT_Get_Char_సూచీ (ft_font->ft_face, 0xF000u + unicode);
	ఆపు;
#నిర్వచితం_కాకపోతే HB_NO_OT_SHAPER_ARABIC_FALLBACK
      స్థితి OT::OS2::font_పుట_t::FONT_పుట_SIMP_ARABIC:
	g = FT_Get_Char_సూచీ (ft_font->ft_face, _hb_arabic_pua_simp_map (unicode));
	ఆపు;
      స్థితి OT::OS2::font_పుట_t::FONT_పుట_TRAD_ARABIC:
	g = FT_Get_Char_సూచీ (ft_font->ft_face, _hb_arabic_pua_trad_map (unicode));
	ఆపు;
#అయితే_ముగింపు
      అప్రమేయ:
	ఆపు;
      }
      ఒకవేళ (!g)
	తిరిగి అబద్ధం;
    }
    లేకపోతే
      తిరిగి అబద్ధం;
  }

  *glyph = g;
  తిరిగి నిజం;
}

స్థిర గుర్తులేని పూర్ణాంకం
hb_ft_get_nominal_glyphs (hb_font_t *font HB_వాడని,
			  శూన్య *font_దత్తాంశం,
			  గుర్తులేని పూర్ణాంకం లెక్క,
			  స్థిరరాశి hb_codepoint_t *మొదటి_unicode,
			  గుర్తులేని పూర్ణాంకం unicode_stride,
			  hb_codepoint_t *మొదటి_glyph,
			  గుర్తులేని పూర్ణాంకం glyph_stride,
			  శూన్య *user_దత్తాంశం HB_వాడని)
{
  స్థిరరాశి hb_ft_font_t *ft_font = (స్థిరరాశి hb_ft_font_t *) font_దత్తాంశం;
  hb_తాళం_t lock (ft_font->lock);
  గుర్తులేని పూర్ణాంకం పూర్తి;
  కొరకు (పూర్తి = 0;
       పూర్తి < లెక్క && (*మొదటి_glyph = FT_Get_Char_సూచీ (ft_font->ft_face, *మొదటి_unicode));
       పూర్తి++)
  {
    మొదటి_unicode = &Struct_At_స్థానభ్రంశం_Unaligned<hb_codepoint_t> (మొదటి_unicode, unicode_stride);
    మొదటి_glyph = &Struct_At_స్థానభ్రంశం_Unaligned<hb_codepoint_t> (మొదటి_glyph, glyph_stride);
  }
  /* We don't need to do ft_font->symbol dance here, since HB calls the singular
   * nominal_glyph() for what we don't handle here. */
  తిరిగి పూర్తి;
}


స్థిర hb_bool_t
hb_ft_get_variation_glyph (hb_font_t *font HB_వాడని,
			   శూన్య *font_దత్తాంశం,
			   hb_codepoint_t unicode,
			   hb_codepoint_t variation_selector,
			   hb_codepoint_t *glyph,
			   శూన్య *user_దత్తాంశం HB_వాడని)
{
  స్థిరరాశి hb_ft_font_t *ft_font = (స్థిరరాశి hb_ft_font_t *) font_దత్తాంశం;
  hb_తాళం_t lock (ft_font->lock);
  గుర్తులేని పూర్ణాంకం g = FT_Face_Get_Char_Variant_సూచీ (ft_font->ft_face, unicode, variation_selector);

  ఒకవేళ (unlikely (!g))
    తిరిగి అబద్ధం;

  *glyph = g;
  తిరిగి నిజం;
}

స్థిర శూన్య
hb_ft_get_glyph_h_advances (hb_font_t* font, శూన్య* font_దత్తాంశం,
			    గుర్తులేని లెక్క,
			    స్థిరరాశి hb_codepoint_t *మొదటి_glyph,
			    గుర్తులేని glyph_stride,
			    hb_position_t *మొదటి_advance,
			    గుర్తులేని advance_stride,
			    శూన్య *user_దత్తాంశం HB_వాడని)
{
  స్థిరరాశి hb_ft_font_t *ft_font = (స్థిరరాశి hb_ft_font_t *) font_దత్తాంశం;
  hb_ft_hb_font_check_మారింది (font, ft_font);

  hb_తాళం_t lock (ft_font->lock);
  FT_Face ft_face = ft_font->ft_face;
  పూర్ణాంకం ఎక్కించు_flags = ft_font->ఎక్కించు_flags;
  దశాంశం x_mult;
#నిర్వచితమైతే HAVE_FT_GET_TRANSFORM
  ఒకవేళ (ft_font->transform)
  {
    FT_మాతృక మాతృక;
    FT_Get_Transform (ft_face, &మాతృక, శూన్యసూచిక);
    x_mult = sqrtf ((దశాంశం)మాతృక.xx * మాతృక.xx + (దశాంశం)మాతృక.xy * మాతృక.xy) / 65536.f;
    x_mult *= font->x_scale < 0 ? -1 : +1;
  }
  లేకపోతే
#అయితే_ముగింపు
  {
    x_mult = font->x_scale < 0 ? -1 : +1;
  }

  కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++)
  {
    hb_position_t advance;
    hb_codepoint_t glyph = *మొదటి_glyph;

    గుర్తులేని పూర్ణాంకం cv;
    ఒకవేళ (ft_font->advance_cache.get (glyph, &cv))
      advance = (hb_position_t) cv;
    లేకపోతే
    {
      FT_Fixed v = 0;
      FT_Get_Advance (ft_face, glyph, ఎక్కించు_flags, &v);
      /* Work around bug that FreeType seems to return negative advance
       * for variable-set fonts if x_scale is negative! */
      ద్విగుణ scaled = trunc (fabs ((ద్విగుణ) v) * (ద్విగుణ) x_mult + (1<<9)) / (1<<10);
      advance = hb_clamp_గమ్యం<hb_position_t> (floor (scaled));
      ft_font->advance_cache.సమితి (glyph, advance);
    }

    *మొదటి_advance = advance;
    మొదటి_glyph = &Struct_At_స్థానభ్రంశం_Unaligned<hb_codepoint_t> (మొదటి_glyph, glyph_stride);
    మొదటి_advance = &Struct_At_స్థానభ్రంశం_Unaligned<hb_position_t> (మొదటి_advance, advance_stride);
  }
}

#నిర్వచితం_కాకపోతే HB_NO_VERTICAL
స్థిర hb_position_t
hb_ft_get_glyph_v_advance (hb_font_t *font,
			   శూన్య *font_దత్తాంశం,
			   hb_codepoint_t glyph,
			   శూన్య *user_దత్తాంశం HB_వాడని)
{
  స్థిరరాశి hb_ft_font_t *ft_font = (స్థిరరాశి hb_ft_font_t *) font_దత్తాంశం;
  hb_ft_hb_font_check_మారింది (font, ft_font);

  hb_తాళం_t lock (ft_font->lock);
  FT_Fixed v;
  దశాంశం y_mult;
#నిర్వచితమైతే HAVE_FT_GET_TRANSFORM
  ఒకవేళ (ft_font->transform)
  {
    FT_మాతృక మాతృక;
    FT_Get_Transform (ft_font->ft_face, &మాతృక, శూన్యసూచిక);
    y_mult = sqrtf ((దశాంశం)మాతృక.yx * మాతృక.yx + (దశాంశం)మాతృక.yy * మాతృక.yy) / 65536.f;
    y_mult *= font->y_scale < 0 ? -1 : +1;
  }
  లేకపోతే
#అయితే_ముగింపు
  {
    y_mult = font->y_scale < 0 ? -1 : +1;
  }

  ఒకవేళ (unlikely (FT_Get_Advance (ft_font->ft_face, glyph, ft_font->ఎక్కించు_flags | FT_ఎక్కించు_VERTICAL_LAYOUT, &v)))
    తిరిగి 0;

  /* Note: FreeType's vertical metrics grows downward while other FreeType coordinates
   * have a Y growing upward.  Hence the extra negation. */
  ద్విగుణ advance = floor ((-(ద్విగుణ) v + (1<<9)) / (1<<10));

  తిరిగి hb_clamp_గమ్యం<hb_position_t> ((ద్విగుణ) y_mult * advance);
}
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే HB_NO_VERTICAL
స్థిర hb_bool_t
hb_ft_get_glyph_v_origin (hb_font_t *font,
			  శూన్య *font_దత్తాంశం,
			  hb_codepoint_t glyph,
			  hb_position_t *x,
			  hb_position_t *y,
			  శూన్య *user_దత్తాంశం HB_వాడని)
{
  స్థిరరాశి hb_ft_font_t *ft_font = (స్థిరరాశి hb_ft_font_t *) font_దత్తాంశం;
  hb_ft_hb_font_check_మారింది (font, ft_font);

  hb_తాళం_t lock (ft_font->lock);
  FT_Face ft_face = ft_font->ft_face;
  దశాంశం x_mult, y_mult;
#నిర్వచితమైతే HAVE_FT_GET_TRANSFORM
  ఒకవేళ (ft_font->transform)
  {
    FT_మాతృక మాతృక;
    FT_Get_Transform (ft_face, &మాతృక, శూన్యసూచిక);
    x_mult = sqrtf ((దశాంశం)మాతృక.xx * మాతృక.xx + (దశాంశం)మాతృక.xy * మాతృక.xy) / 65536.f;
    x_mult *= font->x_scale < 0 ? -1 : +1;
    y_mult = sqrtf ((దశాంశం)మాతృక.yx * మాతృక.yx + (దశాంశం)మాతృక.yy * మాతృక.yy) / 65536.f;
    y_mult *= font->y_scale < 0 ? -1 : +1;
  }
  లేకపోతే
#అయితే_ముగింపు
  {
    x_mult = font->x_scale < 0 ? -1 : +1;
    y_mult = font->y_scale < 0 ? -1 : +1;
  }

  ఒకవేళ (unlikely (FT_ఎక్కించు_Glyph (ft_face, glyph, ft_font->ఎక్కించు_flags)))
    తిరిగి అబద్ధం;

  /* Note: FreeType's vertical metrics grows downward while other FreeType coordinates
   * have a Y growing upward.  Hence the extra negation. */
  *x = hb_clamp_గమ్యం<hb_position_t> ((ద్విగుణ) x_mult *
				   ((ద్విగుణ) ft_face->glyph->metrics.horiBearingX -
				    ft_face->glyph->metrics.vertBearingX));
  *y = hb_clamp_గమ్యం<hb_position_t> ((ద్విగుణ) y_mult *
				   ((ద్విగుణ) ft_face->glyph->metrics.horiBearingY +
				    ft_face->glyph->metrics.vertBearingY));

  తిరిగి నిజం;
}
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే HB_NO_OT_SHAPE_FALLBACK
స్థిర hb_position_t
hb_ft_get_glyph_h_kerning (hb_font_t *font,
			   శూన్య *font_దత్తాంశం,
			   hb_codepoint_t ఎడమ_glyph,
			   hb_codepoint_t కుడి_glyph,
			   శూన్య *user_దత్తాంశం HB_వాడని)
{
  స్థిరరాశి hb_ft_font_t *ft_font = (స్థిరరాశి hb_ft_font_t *) font_దత్తాంశం;
  hb_ft_hb_font_check_మారింది (font, ft_font);

  hb_తాళం_t lock (ft_font->lock);
  FT_సదిశ kerningv;

  FT_Kerning_బహుళకం బహుళకం = font->x_ppem ? FT_KERNING_DEFAULT : FT_KERNING_UNFITTED;
  ఒకవేళ (FT_Get_Kerning (ft_font->ft_face, ఎడమ_glyph, కుడి_glyph, బహుళకం, &kerningv))
    తిరిగి 0;

  తిరిగి hb_clamp_గమ్యం<hb_position_t> ((int64_t) kerningv.x);
}
#అయితే_ముగింపు

స్థిర తార్కిక
hb_ft_is_colr_glyph (hb_font_t *font,
		     శూన్య *font_దత్తాంశం,
		     hb_codepoint_t gid)
{
#నిర్వచితం_కాకపోతే HB_NO_PAINT
#ఒకవేళ (FREETYPE_MAJOR*10000 + FREETYPE_MINOR*100 + FREETYPE_PATCH) >= 21300
  స్థిరరాశి hb_ft_font_t *ft_font = (స్థిరరాశి hb_ft_font_t *) font_దత్తాంశం;
  FT_Face ft_face = ft_font->ft_face;


  /* COLRv1 */
  FT_OpaquePaint paint = {0};
  ఒకవేళ (FT_Get_Color_Glyph_Paint (ft_face, gid,
			        FT_COLOR_NO_మూలం_TRANSFORM,
			        &paint))
    తిరిగి నిజం;

  /* COLRv0 */
  FT_LayerIterator  iterator;
  FT_UInt  layer_glyph_సూచీ;
  FT_UInt  layer_color_సూచీ;
  iterator.p  = ఖాళీ;
  ఒకవేళ (FT_Get_Color_Glyph_Layer (ft_face,
				gid,
				&layer_glyph_సూచీ,
				&layer_color_సూచీ,
				&iterator))
    తిరిగి నిజం;
#అయితే_ముగింపు
#అయితే_ముగింపు

  తిరిగి అబద్ధం;
}

స్థిర hb_bool_t
hb_ft_get_glyph_extents (hb_font_t *font,
			 శూన్య *font_దత్తాంశం,
			 hb_codepoint_t glyph,
			 hb_glyph_extents_t *extents,
			 శూన్య *user_దత్తాంశం HB_వాడని)
{
  // FreeType doesn't return COLR glyph extents.
  ఒకవేళ (hb_ft_is_colr_glyph (font, font_దత్తాంశం, glyph))
    తిరిగి అబద్ధం;

  స్థిరరాశి hb_ft_font_t *ft_font = (స్థిరరాశి hb_ft_font_t *) font_దత్తాంశం;
  hb_ft_hb_font_check_మారింది (font, ft_font);

  hb_తాళం_t lock (ft_font->lock);
  FT_Face ft_face = ft_font->ft_face;
  దశాంశం x_mult, y_mult;

#నిర్వచితమైతే HAVE_FT_GET_TRANSFORM
  ఒకవేళ (ft_font->transform)
  {
    FT_మాతృక మాతృక;
    FT_Get_Transform (ft_face, &మాతృక, శూన్యసూచిక);
    x_mult = sqrtf ((దశాంశం)మాతృక.xx * మాతృక.xx + (దశాంశం)మాతృక.xy * మాతృక.xy) / 65536.f;
    x_mult *= font->x_scale < 0 ? -1 : +1;
    y_mult = sqrtf ((దశాంశం)మాతృక.yx * మాతృక.yx + (దశాంశం)మాతృక.yy * మాతృక.yy) / 65536.f;
    y_mult *= font->y_scale < 0 ? -1 : +1;
  }
  లేకపోతే
#అయితే_ముగింపు
  {
    x_mult = font->x_scale < 0 ? -1 : +1;
    y_mult = font->y_scale < 0 ? -1 : +1;
  }

  ఒకవేళ (unlikely (FT_ఎక్కించు_Glyph (ft_face, glyph, ft_font->ఎక్కించు_flags)))
    తిరిగి అబద్ధం;

  /* Copied from hb_font_t::scale_glyph_extents. */

  దశాంశం x1 = x_mult * ft_face->glyph->metrics.horiBearingX;
  దశాంశం y1 = y_mult * ft_face->glyph->metrics.horiBearingY;
  దశాంశం x2 = x1 + x_mult *  ft_face->glyph->metrics.వెడల్పు;
  దశాంశం y2 = y1 + y_mult * -ft_face->glyph->metrics.ఎత్తు;

  ద్విగుణ rx1 = (ద్విగుణ) roundf (x1);
  ద్విగుణ ry1 = (ద్విగుణ) roundf (y1);
  ద్విగుణ rx2 = (ద్విగుణ) roundf (x2);
  ద్విగుణ ry2 = (ద్విగుణ) roundf (y2);

  extents->x_bearing = hb_clamp_గమ్యం<hb_position_t> (rx1);
  extents->y_bearing = hb_clamp_గమ్యం<hb_position_t> (ry1);
  extents->వెడల్పు = hb_clamp_గమ్యం<hb_position_t> (rx2 - rx1);
  extents->ఎత్తు = hb_clamp_గమ్యం<hb_position_t> (ry2 - ry1);

  తిరిగి నిజం;
}

స్థిర hb_bool_t
hb_ft_get_glyph_contour_బిందువు (hb_font_t *font HB_వాడని,
			       శూన్య *font_దత్తాంశం,
			       hb_codepoint_t glyph,
			       గుర్తులేని పూర్ణాంకం బిందువు_సూచీ,
			       hb_position_t *x,
			       hb_position_t *y,
			       శూన్య *user_దత్తాంశం HB_వాడని)
{
  స్థిరరాశి hb_ft_font_t *ft_font = (స్థిరరాశి hb_ft_font_t *) font_దత్తాంశం;
  hb_ft_hb_font_check_మారింది (font, ft_font);

  hb_తాళం_t lock (ft_font->lock);
  FT_Face ft_face = ft_font->ft_face;

  ఒకవేళ (unlikely (FT_ఎక్కించు_Glyph (ft_face, glyph, ft_font->ఎక్కించు_flags)))
      తిరిగి అబద్ధం;

  ఒకవేళ (unlikely (ft_face->glyph->format != FT_GLYPH_FORMAT_OUTLINE))
      తిరిగి అబద్ధం;

  ఒకవేళ (unlikely (బిందువు_సూచీ >= (గుర్తులేని పూర్ణాంకం) ft_face->glyph->outline.n_points))
      తిరిగి అబద్ధం;

  *x = ft_face->glyph->outline.points[బిందువు_సూచీ].x;
  *y = ft_face->glyph->outline.points[బిందువు_సూచీ].y;

  తిరిగి నిజం;
}

స్థిర hb_bool_t
hb_ft_get_glyph_name (hb_font_t *font HB_వాడని,
		      శూన్య *font_దత్తాంశం,
		      hb_codepoint_t glyph,
		      అక్షరం *name, గుర్తులేని పూర్ణాంకం కొలత,
		      శూన్య *user_దత్తాంశం HB_వాడని)
{
  స్థిరరాశి hb_ft_font_t *ft_font = (స్థిరరాశి hb_ft_font_t *) font_దత్తాంశం;
  hb_తాళం_t lock (ft_font->lock);
  FT_Face ft_face = ft_font->ft_face;

  ఒకవేళ (!కొలత)
  {
    అక్షరం buf[128];
    తిరిగి !FT_Get_Glyph_Name (ft_face, glyph, buf, పరిమాణం (buf)) && *buf;
  }

  hb_bool_t ret = !FT_Get_Glyph_Name (ft_face, glyph, name, కొలత);
  ఒకవేళ (ret && (కొలత && !*name))
    ret = అబద్ధం;

  తిరిగి ret;
}

స్థిర hb_bool_t
hb_ft_get_glyph_from_name (hb_font_t *font HB_వాడని,
			   శూన్య *font_దత్తాంశం,
			   స్థిరరాశి అక్షరం *name, పూర్ణాంకం len, /* -1 means nul-terminated */
			   hb_codepoint_t *glyph,
			   శూన్య *user_దత్తాంశం HB_వాడని)
{
  స్థిరరాశి hb_ft_font_t *ft_font = (స్థిరరాశి hb_ft_font_t *) font_దత్తాంశం;
  hb_తాళం_t lock (ft_font->lock);
  FT_Face ft_face = ft_font->ft_face;

  ఒకవేళ (len < 0)
    *glyph = FT_Get_Name_సూచీ (ft_face, (FT_String *) name);
  లేకపోతే {
    /* Make a nul-terminated version. */
    అక్షరం buf[128];
    len = hb_min (len, (పూర్ణాంకం) పరిమాణం (buf) - 1);
    strncpy (buf, name, len);
    buf[len] = '\0';
    *glyph = FT_Get_Name_సూచీ (ft_face, buf);
  }

  ఒకవేళ (*glyph == 0)
  {
    /* Check whether the given name was actually the name of glyph 0. */
    అక్షరం buf[128];
    ఒకవేళ (!FT_Get_Glyph_Name(ft_face, 0, buf, పరిమాణం (buf)) &&
	len < 0 ? !strcmp (buf, name) : !అక్షరశ్రేణి_పోలిక (buf, name, len))
      తిరిగి నిజం;
  }

  తిరిగి *glyph != 0;
}

స్థిర hb_bool_t
hb_ft_get_font_h_extents (hb_font_t *font HB_వాడని,
			  శూన్య *font_దత్తాంశం,
			  hb_font_extents_t *metrics,
			  శూన్య *user_దత్తాంశం HB_వాడని)
{
  స్థిరరాశి hb_ft_font_t *ft_font = (స్థిరరాశి hb_ft_font_t *) font_దత్తాంశం;
  hb_ft_hb_font_check_మారింది (font, ft_font);

  hb_తాళం_t lock (ft_font->lock);
  FT_Face ft_face = ft_font->ft_face;
  దశాంశం y_mult;
#నిర్వచితమైతే HAVE_FT_GET_TRANSFORM
  ఒకవేళ (ft_font->transform)
  {
    FT_మాతృక మాతృక;
    FT_Get_Transform (ft_face, &మాతృక, శూన్యసూచిక);
    y_mult = sqrtf ((దశాంశం)మాతృక.yx * మాతృక.yx + (దశాంశం)మాతృక.yy * మాతృక.yy) / 65536.f;
    y_mult *= font->y_scale < 0 ? -1 : +1;
  }
  లేకపోతే
#అయితే_ముగింపు
  {
    y_mult = font->y_scale < 0 ? -1 : +1;
  }

  ఒకవేళ (ft_face->units_చొప్పున_EM != 0)
  {
    ద్విగుణ ascender = FT_MulFix(ft_face->ascender, ft_face->కొలత->metrics.y_scale);
    ద్విగుణ descender = FT_MulFix(ft_face->descender, ft_face->కొలత->metrics.y_scale);
    ద్విగుణ ఎత్తు = FT_MulFix( ft_face->ఎత్తు, ft_face->కొలత->metrics.y_scale );
    metrics->ascender = hb_clamp_గమ్యం<hb_position_t> ((ద్విగుణ) y_mult * ascender);
    metrics->descender = hb_clamp_గమ్యం<hb_position_t> ((ద్విగుణ) y_mult * descender);
    metrics->పంక్తి_gap = hb_clamp_గమ్యం<hb_position_t> ((ద్విగుణ) y_mult * (ఎత్తు - (ascender - descender)));
  }
  లేకపోతే
  {
    /* Bitmap-only font, eg. color bitmap font. */
    ద్విగుణ ascender = ft_face->కొలత->metrics.ascender;
    ద్విగుణ descender = ft_face->కొలత->metrics.descender;
    ద్విగుణ ఎత్తు = ft_face->కొలత->metrics.ఎత్తు;
    metrics->ascender = hb_clamp_గమ్యం<hb_position_t> ((ద్విగుణ) y_mult * ascender);
    metrics->descender = hb_clamp_గమ్యం<hb_position_t> ((ద్విగుణ) y_mult * descender);
    metrics->పంక్తి_gap = hb_clamp_గమ్యం<hb_position_t> ((ద్విగుణ) y_mult * (ఎత్తు - (ascender - descender)));
  }

  తిరిగి నిజం;
}

#నిర్వచితం_కాకపోతే HB_NO_DRAW

స్థిర పూర్ణాంకం
hb_ft_move_గమ్యం (స్థిరరాశి FT_సదిశ *to,
		శూన్య *arg)
{
  hb_draw_session_t *drawing = (hb_draw_session_t *) arg;
  drawing->move_గమ్యం (to->x, to->y);
  తిరిగి FT_Err_Ok;
}

స్థిర పూర్ణాంకం
hb_ft_పంక్తి_గమ్యం (స్థిరరాశి FT_సదిశ *to,
		శూన్య *arg)
{
  hb_draw_session_t *drawing = (hb_draw_session_t *) arg;
  drawing->పంక్తి_గమ్యం (to->x, to->y);
  తిరిగి FT_Err_Ok;
}

స్థిర పూర్ణాంకం
hb_ft_conic_గమ్యం (స్థిరరాశి FT_సదిశ *control,
		 స్థిరరాశి FT_సదిశ *to,
		 శూన్య *arg)
{
  hb_draw_session_t *drawing = (hb_draw_session_t *) arg;
  drawing->quadratic_గమ్యం (control->x, control->y,
			 to->x, to->y);
  తిరిగి FT_Err_Ok;
}

స్థిర పూర్ణాంకం
hb_ft_cubic_గమ్యం (స్థిరరాశి FT_సదిశ *control1,
		 స్థిరరాశి FT_సదిశ *control2,
		 స్థిరరాశి FT_సదిశ *to,
		 శూన్య *arg)
{
  hb_draw_session_t *drawing = (hb_draw_session_t *) arg;
  drawing->cubic_గమ్యం (control1->x, control1->y,
		     control2->x, control2->y,
		     to->x, to->y);
  తిరిగి FT_Err_Ok;
}

స్థిర hb_bool_t
hb_ft_draw_glyph_or_fail (hb_font_t *font,
			  శూన్య *font_దత్తాంశం,
			  hb_codepoint_t glyph,
			  hb_draw_funcs_t *draw_funcs, శూన్య *draw_దత్తాంశం,
			  శూన్య *user_దత్తాంశం HB_వాడని)
{
  స్థిరరాశి hb_ft_font_t *ft_font = (స్థిరరాశి hb_ft_font_t *) font_దత్తాంశం;
  hb_ft_hb_font_check_మారింది (font, ft_font);

  hb_తాళం_t lock (ft_font->lock);
  FT_Face ft_face = ft_font->ft_face;

  ఒకవేళ (unlikely (FT_ఎక్కించు_Glyph (ft_face, glyph,
			       FT_ఎక్కించు_NO_BITMAP | ft_font->ఎక్కించు_flags)))
    తిరిగి అబద్ధం;

  ఒకవేళ (ft_face->glyph->format != FT_GLYPH_FORMAT_OUTLINE)
    తిరిగి అబద్ధం;

  స్థిరరాశి FT_Outline_Funcs outline_funcs = {
    hb_ft_move_గమ్యం,
    hb_ft_పంక్తి_గమ్యం,
    hb_ft_conic_గమ్యం,
    hb_ft_cubic_గమ్యం,
    0, /*జరుపు*/
    0, /* delta */
  };

  hb_draw_session_t draw_session {draw_funcs, draw_దత్తాంశం};

  FT_Outline_Decompose (&ft_face->glyph->outline,
			&outline_funcs,
			&draw_session);

  తిరిగి నిజం;
}
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే HB_NO_PAINT
#ఒకవేళ (FREETYPE_MAJOR*10000 + FREETYPE_MINOR*100 + FREETYPE_PATCH) >= 21300

#అన్తర్భావ "hb-ft-colr.hh"

స్థిర hb_bool_t
hb_ft_paint_glyph_or_fail (hb_font_t *font,
			   శూన్య *font_దత్తాంశం,
			   hb_codepoint_t gid,
			   hb_paint_funcs_t *paint_funcs, శూన్య *paint_దత్తాంశం,
			   గుర్తులేని పూర్ణాంకం palette_సూచీ,
			   hb_color_t foreground,
			   శూన్య *user_దత్తాంశం)
{
  స్థిరరాశి hb_ft_font_t *ft_font = (స్థిరరాశి hb_ft_font_t *) font_దత్తాంశం;
  hb_ft_hb_font_check_మారింది (font, ft_font);

  hb_తాళం_t lock (ft_font->lock);
  FT_Face ft_face = ft_font->ft_face;

  FT_Long ఎక్కించు_flags = ft_font->ఎక్కించు_flags | FT_ఎక్కించు_COLOR;
#ఒకవేళ (FREETYPE_MAJOR*10000 + FREETYPE_MINOR*100 + FREETYPE_PATCH) >= 21301
  ఎక్కించు_flags |= FT_ఎక్కించు_NO_SVG;
#అయితే_ముగింపు

  /* We release the lock before calling into glyph callbacks, such that
   * eg. draw API can call back into the face.*/

  ఒకవేళ (unlikely (FT_ఎక్కించు_Glyph (ft_face, gid, ఎక్కించు_flags)))
    తిరిగి అబద్ధం;

  ఒకవేళ (ft_face->glyph->format == FT_GLYPH_FORMAT_OUTLINE)
  {
    ఒకవేళ (hb_ft_paint_glyph_colr (font, font_దత్తాంశం, gid,
				paint_funcs, paint_దత్తాంశం,
				palette_సూచీ, foreground,
				user_దత్తాంశం))
      తిరిగి నిజం;

    // Outline glyph
    తిరిగి అబద్ధం;
  }

  స్వయంచాలక *glyph = ft_face->glyph;
  ఒకవేళ (glyph->format == FT_GLYPH_FORMAT_BITMAP)
  {
    తార్కిక ret = అబద్ధం;
    స్వయంచాలక &bitmap = glyph->bitmap;
    ఒకవేళ (bitmap.pixel_బహుళకం == FT_PIXEL_బహుళకం_BGRA)
    {
      ఒకవేళ (bitmap.pitch != (గుర్తుగల) bitmap.వెడల్పు * 4)
        తిరిగి ret;

      ft_font->lock.unlock ();

      hb_blob_t *blob = hb_blob_create ((స్థిరరాశి అక్షరం *) bitmap.బఫరు,
					bitmap.pitch * bitmap.అడ్డువరుసలు,
					HB_జ్ఞాపకం_బహుళకం_DUPLICATE,
					శూన్యసూచిక, శూన్యసూచిక);

      hb_glyph_extents_t extents;
      ఒకవేళ (!font->get_glyph_extents (gid, &extents, అబద్ధం))
	వెళ్ళు బహిర్గతం;

      ఒకవేళ (paint_funcs->image (paint_దత్తాంశం,
			      blob,
			      bitmap.వెడల్పు,
			      bitmap.అడ్డువరుసలు,
			      HB_PAINT_IMAGE_FORMAT_BGRA,
			      0.f,
			      &extents))
        ret = నిజం;

    బహిర్గతం:
      hb_blob_destroy (blob);
      ft_font->lock.lock ();
    }

    తిరిగి ret;
  }
  తిరిగి అబద్ధం;
}
#అయితే_ముగింపు
#అయితే_ముగింపు


స్థిర అంతఃవరుస శూన్య free_static_ft_funcs ();

స్థిర నిర్మాణం hb_ft_font_funcs_lazy_loader_t : hb_font_funcs_lazy_loader_t<hb_ft_font_funcs_lazy_loader_t>
{
  స్థిర hb_font_funcs_t *create ()
  {
    hb_font_funcs_t *funcs = hb_font_funcs_create ();

    hb_font_funcs_సమితి_nominal_glyph_func (funcs, hb_ft_get_nominal_glyph, శూన్యసూచిక, శూన్యసూచిక);
    hb_font_funcs_సమితి_nominal_glyphs_func (funcs, hb_ft_get_nominal_glyphs, శూన్యసూచిక, శూన్యసూచిక);
    hb_font_funcs_సమితి_variation_glyph_func (funcs, hb_ft_get_variation_glyph, శూన్యసూచిక, శూన్యసూచిక);

    hb_font_funcs_సమితి_font_h_extents_func (funcs, hb_ft_get_font_h_extents, శూన్యసూచిక, శూన్యసూచిక);
    hb_font_funcs_సమితి_glyph_h_advances_func (funcs, hb_ft_get_glyph_h_advances, శూన్యసూచిక, శూన్యసూచిక);

#నిర్వచితం_కాకపోతే HB_NO_VERTICAL
    hb_font_funcs_సమితి_glyph_v_advance_func (funcs, hb_ft_get_glyph_v_advance, శూన్యసూచిక, శూన్యసూచిక);
    hb_font_funcs_సమితి_glyph_v_origin_func (funcs, hb_ft_get_glyph_v_origin, శూన్యసూచిక, శూన్యసూచిక);
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే HB_NO_OT_SHAPE_FALLBACK
    hb_font_funcs_సమితి_glyph_h_kerning_func (funcs, hb_ft_get_glyph_h_kerning, శూన్యసూచిక, శూన్యసూచిక);
#అయితే_ముగింపు
    hb_font_funcs_సమితి_glyph_extents_func (funcs, hb_ft_get_glyph_extents, శూన్యసూచిక, శూన్యసూచిక);
    hb_font_funcs_సమితి_glyph_contour_బిందువు_func (funcs, hb_ft_get_glyph_contour_బిందువు, శూన్యసూచిక, శూన్యసూచిక);
    hb_font_funcs_సమితి_glyph_name_func (funcs, hb_ft_get_glyph_name, శూన్యసూచిక, శూన్యసూచిక);
    hb_font_funcs_సమితి_glyph_from_name_func (funcs, hb_ft_get_glyph_from_name, శూన్యసూచిక, శూన్యసూచిక);

#నిర్వచితం_కాకపోతే HB_NO_DRAW
    hb_font_funcs_సమితి_draw_glyph_or_fail_func (funcs, hb_ft_draw_glyph_or_fail, శూన్యసూచిక, శూన్యసూచిక);
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే HB_NO_PAINT
#ఒకవేళ (FREETYPE_MAJOR*10000 + FREETYPE_MINOR*100 + FREETYPE_PATCH) >= 21300
    hb_font_funcs_సమితి_paint_glyph_or_fail_func (funcs, hb_ft_paint_glyph_or_fail, శూన్యసూచిక, శూన్యసూచిక);
#అయితే_ముగింపు
#అయితే_ముగింపు

    hb_font_funcs_make_immutable (funcs);

    hb_atexit (free_static_ft_funcs);

    తిరిగి funcs;
  }
} static_ft_funcs;

స్థిర అంతఃవరుస
శూన్య free_static_ft_funcs ()
{
  static_ft_funcs.free_instance ();
}

స్థిర hb_font_funcs_t *
_hb_ft_get_font_funcs ()
{
  తిరిగి static_ft_funcs.get_unconst ();
}

స్థిర శూన్య
hb_ft_font_సమితి_funcs_2 (hb_font_t *font, FT_Face ft_face, తార్కిక unref,
		       తార్కిక static_library = అబద్ధం)
{
  తార్కిక చిహ్నం = ft_face->charmap && ft_face->charmap->encoding == FT_ENCODING_MS_చిహ్నం;

  hb_ft_font_t *ft_font = _hb_ft_font_create (ft_face, చిహ్నం, unref, static_library);
  ఒకవేళ (unlikely (!ft_font)) తిరిగి;

  hb_font_సమితి_funcs (font,
		     _hb_ft_get_font_funcs (),
		     ft_font,
		     _hb_ft_font_destroy);
}


స్థిర hb_blob_t *
hb_ft_reference_పట్టిక (hb_face_t *face HB_వాడని, hb_ట్యాగ్_t ట్యాగ్_2, శూన్య *user_దత్తాంశం)
{
  FT_Face ft_face = (FT_Face) user_దత్తాంశం;
  FT_Byte *బఫరు;
  FT_ULong  నిడివి = 0;
  FT_Error error;

  /* In new FreeType, a tag value of 1 loads the SFNT table directory. Reject it. */
  ఒకవేళ (ట్యాగ్_2 == 1)
    తిరిగి శూన్యసూచిక;

  /* Note: FreeType like HarfBuzz uses the NONE tag for fetching the entire blob */

  error = FT_ఎక్కించు_Sfnt_పట్టిక (ft_face, ట్యాగ్_2, 0, శూన్యసూచిక, &నిడివి);
  ఒకవేళ (error)
    తిరిగి శూన్యసూచిక;

  బఫరు = (FT_Byte *) hb_malloc (నిడివి);
  ఒకవేళ (!బఫరు)
    తిరిగి శూన్యసూచిక;
  స్వయంచాలక బఫరు_guard = hb_make_scope_guard ([&]() { hb_free (బఫరు); });

  error = FT_ఎక్కించు_Sfnt_పట్టిక (ft_face, ట్యాగ్_2, 0, బఫరు, &నిడివి);
  ఒకవేళ (error)
    తిరిగి శూన్యసూచిక;

  బఫరు_guard.release ();
  తిరిగి hb_blob_create ((స్థిరరాశి అక్షరం *) బఫరు, నిడివి,
			 HB_జ్ఞాపకం_బహుళకం_WRITABLE,
			 బఫరు, hb_free);
}

స్థిర గుర్తులేని
hb_ft_get_పట్టిక_ట్యాగులు (స్థిరరాశి hb_face_t *face HB_వాడని,
		       గుర్తులేని పూర్ణాంకం ప్రారంభం_స్థానభ్రంశం,
		       గుర్తులేని పూర్ణాంకం *పట్టిక_లెక్క_2,
		       hb_ట్యాగ్_t *పట్టిక_ట్యాగులు,
		       శూన్య *user_దత్తాంశం)
{
  FT_Face ft_face = (FT_Face) user_దత్తాంశం;

  FT_ULong population = 0;
  FT_Sfnt_పట్టిక_Info (ft_face,
		      0, // table_index; ignored
		      శూన్యసూచిక,
                      &population);

  ఒకవేళ (!పట్టిక_లెక్క_2)
    తిరిగి population;

  ఒకవేళ (unlikely (ప్రారంభం_స్థానభ్రంశం >= population))
  {
    *పట్టిక_లెక్క_2 = 0;
    తిరిగి population;
  }

  గుర్తులేని చివర_స్థానభ్రంశం = ప్రారంభం_స్థానభ్రంశం + *పట్టిక_లెక్క_2;
  ఒకవేళ (unlikely (చివర_స్థానభ్రంశం < ప్రారంభం_స్థానభ్రంశం)) /* Overflow. */
  {
    *పట్టిక_లెక్క_2 = 0;
    తిరిగి population;
  }
  చివర_స్థానభ్రంశం = hb_min (చివర_స్థానభ్రంశం, (గుర్తులేని) population);

  *పట్టిక_లెక్క_2 = చివర_స్థానభ్రంశం - ప్రారంభం_స్థానభ్రంశం;
  ఒకవేళ (పట్టిక_ట్యాగులు)
    కొరకు (గుర్తులేని i = ప్రారంభం_స్థానభ్రంశం; i < చివర_స్థానభ్రంశం; i++)
    {
      FT_ULong ట్యాగ్_2 = 0, నిడివి;
      FT_Sfnt_పట్టిక_Info (ft_face, i, &ట్యాగ్_2, &నిడివి);
      పట్టిక_ట్యాగులు[i - ప్రారంభం_స్థానభ్రంశం] = ట్యాగ్_2;
    }

  తిరిగి population;
}


/**
 * hb_ft_face_create:
 * @ft_face: FT_Face to work upon
 * @destroy: (nullable) (scope async): A callback to call when the face object is not needed anymore
 *
 * Creates an #hb_face_t face object from the specified FT_Face.
 *
 * Note that this is using the FT_Face object just to get at the underlying
 * font data, and fonts created from the returned #hb_face_t will use the native
 * HarfBuzz font implementation, unless you call hb_ft_font_set_funcs() on them.
 *
 * This variant of the function does not provide any life-cycle management.
 *
 * Most client programs should use hb_ft_face_create_referenced()
 * (or, perhaps, hb_ft_face_create_cached()) instead.
 *
 * If you know you have valid reasons not to use hb_ft_face_create_referenced(),
 * then it is the client program's responsibility to destroy @ft_face
 * after the #hb_face_t face object has been destroyed.
 *
 * Return value: (transfer full): the new #hb_face_t face object
 *
 * Since: 0.9.2
 **/
hb_face_t *
hb_ft_face_create (FT_Face           ft_face,
		   hb_destroy_func_t destroy)
{
  hb_face_t *face;

  ఒకవేళ (!ft_face->ప్రవాహం->పఠ) {
    hb_blob_t *blob;

    blob = hb_blob_create ((స్థిరరాశి అక్షరం *) ft_face->ప్రవాహం->పునాది,
			   (గుర్తులేని పూర్ణాంకం) ft_face->ప్రవాహం->కొలత,
			   HB_జ్ఞాపకం_బహుళకం_READONLY,
			   ft_face, destroy);
    face = hb_face_create (blob, ft_face->face_సూచీ);
    hb_blob_destroy (blob);
  } లేకపోతే {
    face = hb_face_create_for_tables (hb_ft_reference_పట్టిక, ft_face, destroy);
    hb_face_సమితి_get_పట్టిక_ట్యాగులు_func (face, hb_ft_get_పట్టిక_ట్యాగులు, ft_face, శూన్యసూచిక);
  }

  hb_face_సమితి_సూచీ (face, ft_face->face_సూచీ);
  hb_face_సమితి_upem (face, ft_face->units_చొప్పున_EM);

  తిరిగి face;
}

/**
 * hb_ft_face_create_referenced:
 * @ft_face: FT_Face to work upon
 *
 * Creates an #hb_face_t face object from the specified FT_Face.
 *
 * Note that this is using the FT_Face object just to get at the underlying
 * font data, and fonts created from the returned #hb_face_t will use the native
 * HarfBuzz font implementation, unless you call hb_ft_font_set_funcs() on them.
 *
 * This is the preferred variant of the hb_ft_face_create*
 * function family, because it calls FT_Reference_Face() on @ft_face,
 * ensuring that @ft_face remains alive as long as the resulting
 * #hb_face_t face object remains alive. Also calls FT_Done_Face()
 * when the #hb_face_t face object is destroyed.
 *
 * Use this version unless you know you have good reasons not to.
 *
 * Return value: (transfer full): the new #hb_face_t face object
 *
 * Since: 0.9.38
 **/
hb_face_t *
hb_ft_face_create_referenced (FT_Face ft_face)
{
  FT_Reference_Face (ft_face);
  తిరిగి hb_ft_face_create (ft_face, _hb_ft_face_destroy);
}

స్థిర శూన్య
hb_ft_face_finalize (శూన్య *arg)
{
  FT_Face ft_face = (FT_Face) arg;
  hb_face_destroy ((hb_face_t *) ft_face->generic.దత్తాంశం);
}

/**
 * hb_ft_face_create_cached:
 * @ft_face: FT_Face to work upon
 *
 * Creates an #hb_face_t face object from the specified FT_Face.
 *
 * Note that this is using the FT_Face object just to get at the underlying
 * font data, and fonts created from the returned #hb_face_t will use the native
 * HarfBuzz font implementation, unless you call hb_ft_font_set_funcs() on them.
 *
 * This variant of the function caches the newly created #hb_face_t
 * face object, using the @generic pointer of @ft_face. Subsequent function
 * calls that are passed the same @ft_face parameter will have the same
 * #hb_face_t returned to them, and that #hb_face_t will be correctly
 * reference counted.
 *
 * However, client programs are still responsible for destroying
 * @ft_face after the last #hb_face_t face object has been destroyed.
 *
 * Return value: (transfer full): the new #hb_face_t face object
 *
 * Since: 0.9.2
 **/
hb_face_t *
hb_ft_face_create_cached (FT_Face ft_face)
{
  ఒకవేళ (unlikely (!ft_face->generic.దత్తాంశం || ft_face->generic.finalizer != hb_ft_face_finalize))
  {
    ఒకవేళ (ft_face->generic.finalizer)
      ft_face->generic.finalizer (ft_face);

    ft_face->generic.దత్తాంశం = hb_ft_face_create (ft_face, శూన్యసూచిక);
    ft_face->generic.finalizer = hb_ft_face_finalize;
  }

  తిరిగి hb_face_reference ((hb_face_t *) ft_face->generic.దత్తాంశం);
}

/**
 * hb_ft_font_create:
 * @ft_face: FT_Face to work upon
 * @destroy: (nullable) (scope async): A callback to call when the font object is not needed anymore
 *
 * Creates an #hb_font_t font object from the specified FT_Face.
 *
 * <note>Note: You must set the face size on @ft_face before calling
 * hb_ft_font_create() on it. HarfBuzz assumes size is always set and will
 * access `size` member of FT_Face unconditionally.</note>
 *
 * This variant of the function does not provide any life-cycle management.
 *
 * Most client programs should use hb_ft_font_create_referenced()
 * instead.
 *
 * If you know you have valid reasons not to use hb_ft_font_create_referenced(),
 * then it is the client program's responsibility to destroy @ft_face
 * only after the #hb_font_t font object has been destroyed.
 *
 * HarfBuzz will use the @destroy callback on the #hb_font_t font object
 * if it is supplied when you use this function. However, even if @destroy
 * is provided, it is the client program's responsibility to destroy @ft_face,
 * and it is the client program's responsibility to ensure that @ft_face is
 * destroyed only after the #hb_font_t font object has been destroyed.
 *
 * Return value: (transfer full): the new #hb_font_t font object
 *
 * Since: 0.9.2
 **/
hb_font_t *
hb_ft_font_create (FT_Face           ft_face,
		   hb_destroy_func_t destroy)
{
  hb_font_t *font;
  hb_face_t *face;

  face = hb_ft_face_create (ft_face, destroy);
  font = hb_font_create (face);
  hb_face_destroy (face);
  hb_ft_font_సమితి_funcs_2 (font, ft_face, అబద్ధం);
  hb_ft_font_మారింది (font);
  తిరిగి font;
}

/**
 * hb_ft_font_changed:
 * @font: #hb_font_t to work upon
 *
 * Refreshes the state of @font when the underlying FT_Face has changed.
 * This function should be called after changing the size or
 * variation-axis settings on the FT_Face.
 *
 * Since: 1.0.5
 **/
శూన్య
hb_ft_font_మారింది (hb_font_t *font)
{
  ఒకవేళ (font->destroy != (hb_destroy_func_t) _hb_ft_font_destroy)
    తిరిగి;

  hb_ft_font_t *ft_font = (hb_ft_font_t *) font->user_దత్తాంశం;

  FT_Face ft_face = ft_font->ft_face;

  hb_font_సమితి_scale (font,
		     (పూర్ణాంకం) (((uint64_t) ft_face->కొలత->metrics.x_scale * (uint64_t) ft_face->units_చొప్పున_EM + (1u<<15)) >> 16),
		     (పూర్ణాంకం) (((uint64_t) ft_face->కొలత->metrics.y_scale * (uint64_t) ft_face->units_చొప్పున_EM + (1u<<15)) >> 16));
#ఒకవేళ 0 /* hb-ft works in no-hinting model */
  hb_font_సమితి_ppem (font,
		    ft_face->కొలత->metrics.x_ppem,
		    ft_face->కొలత->metrics.y_ppem);
#అయితే_ముగింపు

#ఒకవేళ defined(HAVE_FT_GET_VAR_BLEND_COORDINATES) && !defined(HB_NO_VAR)
  FT_MM_Var *mm_var = శూన్యసూచిక;
  ఒకవేళ (!FT_Get_MM_Var (ft_face, &mm_var))
  {
    FT_Fixed *ft_coords = (FT_Fixed *) hb_calloc (mm_var->అంకె_axis, పరిమాణం (FT_Fixed));
    పూర్ణాంకం *coords = (పూర్ణాంకం *) hb_calloc (mm_var->అంకె_axis, పరిమాణం (పూర్ణాంకం));
    ఒకవేళ (coords && ft_coords)
    {
      ఒకవేళ (!FT_Get_Var_Blend_Coordinates (ft_face, mm_var->అంకె_axis, ft_coords))
      {
	తార్కిక nonzero = అబద్ధం;

	కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < mm_var->అంకె_axis; ++i)
	 {
	  coords[i] = (ft_coords[i] + 2) >> 2;
	  nonzero = nonzero || coords[i];
	 }

	ఒకవేళ (nonzero)
	  hb_font_సమితి_var_coords_normalized (font, coords, mm_var->అంకె_axis);
	లేకపోతే
	  hb_font_సమితి_var_coords_normalized (font, శూన్యసూచిక, 0);
      }
    }
    hb_free (coords);
    hb_free (ft_coords);
#నిర్వచితమైతే HAVE_FT_పూర్తి_MM_VAR
    FT_పూర్తి_MM_Var (ft_face->glyph->library, mm_var);
#లేకపోతే
    hb_free (mm_var);
#అయితే_ముగింపు
  }
#అయితే_ముగింపు

  ft_font->advance_cache.clear ();
  ft_font->cached_serial = font->serial;
}

/**
 * hb_ft_hb_font_changed:
 * @font: #hb_font_t to work upon
 *
 * Refreshes the state of the underlying FT_Face of @font when the hb_font_t
 * @font has changed.
 * This function should be called after changing the size or
 * variation-axis settings on the @font.
 * This call is fast if nothing has changed on @font.
 *
 * Note that as of version 11.0.0, calling this function is not necessary,
 * as HarfBuzz will automatically detect changes to the font and update
 * the underlying FT_Face as needed.
 *
 * Return value: true if changed, false otherwise
 *
 * Since: 4.4.0
 **/
hb_bool_t
hb_ft_hb_font_మారింది_2 (hb_font_t *font)
{
  ఒకవేళ (font->destroy != (hb_destroy_func_t) _hb_ft_font_destroy)
    తిరిగి అబద్ధం;

  hb_ft_font_t *ft_font = (hb_ft_font_t *) font->user_దత్తాంశం;

  తిరిగి hb_ft_hb_font_check_మారింది (font, ft_font);
}

/**
 * hb_ft_font_create_referenced:
 * @ft_face: FT_Face to work upon
 *
 * Creates an #hb_font_t font object from the specified FT_Face.
 *
 * <note>Note: You must set the face size on @ft_face before calling
 * hb_ft_font_create_referenced() on it. HarfBuzz assumes size is always set
 * and will access `size` member of FT_Face unconditionally.</note>
 *
 * This is the preferred variant of the hb_ft_font_create*
 * function family, because it calls FT_Reference_Face() on @ft_face,
 * ensuring that @ft_face remains alive as long as the resulting
 * #hb_font_t font object remains alive.
 *
 * Use this version unless you know you have good reasons not to.
 *
 * Return value: (transfer full): the new #hb_font_t font object
 *
 * Since: 0.9.38
 **/
hb_font_t *
hb_ft_font_create_referenced (FT_Face ft_face)
{
  FT_Reference_Face (ft_face);
  తిరిగి hb_ft_font_create (ft_face, _hb_ft_face_destroy);
}


స్థిర శూన్య * _hb_ft_alloc (FT_జ్ఞాపకం జ్ఞాపకం, దీర్ఘ కొలత)
{ తిరిగి hb_malloc (కొలత); }

స్థిర శూన్య _hb_ft_free (FT_జ్ఞాపకం జ్ఞాపకం, శూన్య *ఖండం)
{ hb_free (ఖండం); }

స్థిర శూన్య * _hb_ft_realloc (FT_జ్ఞాపకం జ్ఞాపకం, దీర్ఘ cur_కొలత, దీర్ఘ new_కొలత, శూన్య *ఖండం)
{ తిరిగి hb_realloc (ఖండం, new_కొలత); }

స్థిర FT_జ్ఞాపకం_Rec m =
{
  శూన్యసూచిక,
  _hb_ft_alloc,
  _hb_ft_free,
  _hb_ft_realloc
};

స్థిర అంతఃవరుస శూన్య free_static_ft_library ();
స్థిర అంతఃవరుస శూన్య free_static_ft_library_mutex ();

స్థిర నిర్మాణం hb_ft_library_mutex_lazy_loader_t : hb_lazy_loader_t<hb_mutex_t,
								   hb_ft_library_mutex_lazy_loader_t>
{
  స్థిర hb_mutex_t *create ()
  {
    hb_mutex_t *lock = (hb_mutex_t *) hb_calloc (1, పరిమాణం (hb_mutex_t));
    ఒకవేళ (unlikely (!lock))
      తిరిగి శూన్యసూచిక;

    lock = కొత్త (lock) hb_mutex_t;
    hb_atexit (free_static_ft_library_mutex);
    తిరిగి lock;
  }
  స్థిర శూన్య destroy (hb_mutex_t *lock)
  {
    lock->~hb_mutex_t ();
    hb_free (lock);
  }
  స్థిర hb_mutex_t *get_null ()
  {
    తిరిగి శూన్యసూచిక;
  }
} static_ft_library_mutex;

స్థిర నిర్మాణం hb_ft_library_lazy_loader_t : hb_lazy_loader_t<hb_remove_సూచిక<FT_Library>,
							     hb_ft_library_lazy_loader_t>
{
  స్థిర FT_Library create ()
  {
    FT_Library l;
    ఒకవేళ (FT_New_Library (&m, &l))
      తిరిగి శూన్యసూచిక;

    FT_కూడు_Default_Modules (l);
    FT_సమితి_Default_Properties (l);

    hb_atexit (free_static_ft_library);

    తిరిగి l;
  }
  స్థిర శూన్య destroy (FT_Library l)
  {
    FT_పూర్తి_Library (l);
  }
  స్థిర FT_Library get_null ()
  {
    తిరిగి శూన్యసూచిక;
  }
} static_ft_library;

స్థిర అంతఃవరుస
శూన్య free_static_ft_library_mutex ()
{
  static_ft_library_mutex.free_instance ();
}

స్థిర అంతఃవరుస
శూన్య free_static_ft_library ()
{
  hb_తాళం_t lock (static_ft_library_mutex.get_unconst ());
  static_ft_library.free_instance ();
}

స్థిర FT_Library
reference_ft_library ()
{
  hb_mutex_t *mutex = static_ft_library_mutex.get_unconst ();
  ఒకవేళ (unlikely (!mutex))
    తిరిగి శూన్యసూచిక;
  hb_తాళం_t lock (mutex);
  FT_Library l = static_ft_library.get_unconst ();
  ఒకవేళ (unlikely (FT_Reference_Library (l)))
  {
    DEBUG_సందేశం (FT, l, "FT_Reference_Library() failed");
    తిరిగి శూన్యసూచిక;
  }
  తిరిగి l;
}

స్థిర hb_user_దత్తాంశం_కీలకం_t ft_library_కీలకం = {0};

స్థిర శూన్య
_hb_ft_face_destroy_static (శూన్య *దత్తాంశం)
{
  hb_తాళం_t lock (static_ft_library_mutex.get_unconst ());
  FT_పూర్తి_Face ((FT_Face) దత్తాంశం);
}

స్థిర FT_Error
new_ft_face (FT_Library  ft_library,
	     స్థిరరాశి అక్షరం *file_name,
	     గుర్తులేని పూర్ణాంకం సూచీ_2,
	     FT_Face     *ft_face)
{
  hb_తాళం_t lock (static_ft_library_mutex.get_unconst ());
  తిరిగి FT_New_Face (ft_library, file_name, సూచీ_2, ft_face);
}

స్థిర FT_Error
new_ft_జ్ఞాపకం_face (FT_Library    ft_library,
		    స్థిరరాశి FT_Byte *blob_దత్తాంశం,
		    గుర్తులేని పూర్ణాంకం   blob_కొలత,
		    గుర్తులేని పూర్ణాంకం   సూచీ_2,
		    FT_Face       *ft_face)
{
  hb_తాళం_t lock (static_ft_library_mutex.get_unconst ());
  తిరిగి FT_New_జ్ఞాపకం_Face (ft_library, blob_దత్తాంశం, blob_కొలత, సూచీ_2, ft_face);
}

స్థిర శూన్య
finalize_ft_library (శూన్య *arg)
{
  FT_Face ft_face = (FT_Face) arg;
  FT_పూర్తి_Library ((FT_Library) ft_face->generic.దత్తాంశం);
}

స్థిర శూన్య
destroy_ft_library (శూన్య *arg)
{
  hb_తాళం_t lock (static_ft_library_mutex.get_unconst ());
  FT_పూర్తి_Library ((FT_Library) arg);
}

/**
 * hb_ft_face_create_from_file_or_fail:
 * @file_name: A font filename
 * @index: The index of the face within the file
 *
 * Creates an #hb_face_t face object from the specified
 * font file and face index.
 *
 * This is similar in functionality to hb_face_create_from_file_or_fail(),
 * but uses the FreeType library for loading the font file. This can
 * be useful, for example, to load WOFF and WOFF2 font data.
 *
 * Return value: (transfer full): The new face object, or `NULL` if
 * no face is found at the specified index or the file cannot be read.
 *
 * Since: 10.1.0
 */
hb_face_t *
hb_ft_face_create_from_file_or_fail (స్థిరరాశి అక్షరం   *file_name,
				     గుర్తులేని పూర్ణాంకం  సూచీ_2)
{
  FT_Library ft_library = reference_ft_library ();
  ఒకవేళ (unlikely (!ft_library))
  {
    DEBUG_సందేశం (FT, ft_library, "reference_ft_library failed");
    తిరిగి శూన్యసూచిక;
  }

  FT_Face ft_face;
  ఒకవేళ (unlikely (new_ft_face (ft_library,
			     file_name,
			     సూచీ_2,
			     &ft_face)))
  {
    destroy_ft_library (ft_library);
    తిరిగి శూన్యసూచిక;
  }

  FT_Reference_Face (ft_face);
  hb_face_t *face = hb_ft_face_create (ft_face, _hb_ft_face_destroy_static);
  _hb_ft_face_destroy_static (ft_face);

  ft_face->generic.దత్తాంశం = ft_library;
  ft_face->generic.finalizer = finalize_ft_library;

  ఒకవేళ (hb_face_is_immutable (face))
    తిరిగి శూన్యసూచిక;

  తిరిగి face;
}

స్థిర hb_user_దత్తాంశం_కీలకం_t ft_blob_కీలకం = {0};

స్థిర శూన్య
_destroy_blob (శూన్య *p)
{
  hb_blob_destroy ((hb_blob_t *) p);
}

/**
 * hb_ft_face_create_from_blob_or_fail:
 * @blob: A blob
 * @index: The index of the face within the blob
 *
 * Creates an #hb_face_t face object from the specified
 * font blob and face index.
 *
 * This is similar in functionality to hb_face_create_or_fail(),
 * but uses the FreeType library for loading the font blob. This can
 * be useful, for example, to load WOFF and WOFF2 font data.
 *
 * Return value: (transfer full): The new face object, or `NULL` if
 * loading fails (eg. blob does not contain valid font data).
 *
 * Since: 11.0.0
 */
hb_face_t *
hb_ft_face_create_from_blob_or_fail (hb_blob_t    *blob,
				     గుర్తులేని పూర్ణాంకం  సూచీ_2)
{
  FT_Library ft_library = reference_ft_library ();
  ఒకవేళ (unlikely (!ft_library))
  {
    DEBUG_సందేశం (FT, ft_library, "reference_ft_library failed");
    తిరిగి శూన్యసూచిక;
  }

  hb_blob_make_immutable (blob);
  గుర్తులేని blob_కొలత;
  స్థిరరాశి అక్షరం *blob_దత్తాంశం = hb_blob_get_దత్తాంశం (blob, &blob_కొలత);

  FT_Face ft_face;
  ఒకవేళ (unlikely (new_ft_జ్ఞాపకం_face (ft_library,
				    (స్థిరరాశి FT_Byte *) blob_దత్తాంశం,
				    blob_కొలత,
				    సూచీ_2,
				    &ft_face)))
  {
    destroy_ft_library (ft_library);
    తిరిగి శూన్యసూచిక;
  }

  FT_Reference_Face (ft_face);
  hb_face_t *face = hb_ft_face_create (ft_face, _hb_ft_face_destroy_static);
  _hb_ft_face_destroy_static (ft_face);

  ft_face->generic.దత్తాంశం = ft_library;
  ft_face->generic.finalizer = finalize_ft_library;

  ఒకవేళ (hb_face_is_immutable (face))
    తిరిగి శూన్యసూచిక;

  // Hook the blob to the hb_face_t, since FT_Face still needs it.
  hb_blob_reference (blob);
  ఒకవేళ (!hb_face_సమితి_user_దత్తాంశం (face, &ft_blob_కీలకం, blob, _destroy_blob, నిజం))
  {
    hb_blob_destroy (blob);
    hb_face_destroy (face);
    తిరిగి శూన్యసూచిక;
  }

  తిరిగి face;
}

స్థిర శూన్య
_release_blob (శూన్య *arg)
{
  FT_Face ft_face = (FT_Face) arg;
  hb_blob_destroy ((hb_blob_t *) ft_face->generic.దత్తాంశం);
}

/**
 * hb_ft_font_set_funcs:
 * @font: #hb_font_t to work upon
 *
 * Configures the font-functions structure of the specified
 * #hb_font_t font object to use FreeType font functions.
 *
 * In particular, you can use this function to configure an
 * existing #hb_face_t face object for use with FreeType font
 * functions even if that #hb_face_t face object was initially
 * created with hb_face_create(), and therefore was not
 * initially configured to use FreeType font functions.
 *
 * An #hb_font_t object created with hb_ft_font_create()
 * is preconfigured for FreeType font functions and does not
 * require this function to be used.
 *
 * Note that if you modify the underlying #hb_font_t after
 * calling this function, you need to call hb_ft_hb_font_changed()
 * to update the underlying FT_Face.
 *
 * <note>Note: Internally, this function creates an FT_Face.
* </note>
 *
 * Since: 1.0.5
 **/
శూన్య
hb_ft_font_సమితి_funcs (hb_font_t *font)
{
  పూర్ణాంకం ఎక్కించు_flags = FT_ఎక్కించు_DEFAULT | FT_ఎక్కించు_NO_HINTING;
  ఒకవేళ (font->destroy == (hb_destroy_func_t) _hb_ft_font_destroy && font->user_దత్తాంశం)
  {
    స్థిరరాశి hb_ft_font_t *existing_ft_font = (స్థిరరాశి hb_ft_font_t *) font->user_దత్తాంశం;
    ఎక్కించు_flags = existing_ft_font->ఎక్కించు_flags;
  }

  // In case of failure...
  hb_font_సమితి_funcs (font,
		     hb_font_funcs_get_ఖాళీలు (),
		     శూన్యసూచిక, శూన్యసూచిక);

  hb_blob_t *blob = hb_face_reference_blob (font->face);
  గుర్తులేని పూర్ణాంకం blob_నిడివి;
  స్థిరరాశి అక్షరం *blob_దత్తాంశం = hb_blob_get_దత్తాంశం (blob, &blob_నిడివి);
  ఒకవేళ (unlikely (!blob_నిడివి))
    DEBUG_సందేశం (FT, font, "Font face has empty blob");

  FT_Library ft_library = reference_ft_library ();
  ఒకవేళ (unlikely (!ft_library))
  {
    hb_blob_destroy (blob);
    DEBUG_సందేశం (FT, font, "reference_ft_library failed");
    తిరిగి;
  }

  FT_Face ft_face = శూన్యసూచిక;
  ఒకవేళ (unlikely (new_ft_జ్ఞాపకం_face (ft_library,
				    (స్థిరరాశి FT_Byte *) blob_దత్తాంశం,
				    blob_నిడివి,
				    hb_face_get_సూచీ (font->face),
				    &ft_face)))
  {
    hb_blob_destroy (blob);
    destroy_ft_library (ft_library);
    DEBUG_సందేశం (FT, font, "FT_New_Memory_Face() failed");
    తిరిగి;
  }

  ఒకవేళ (FT_Select_Charmap (ft_face, FT_ENCODING_MS_చిహ్నం))
    FT_Select_Charmap (ft_face, FT_ENCODING_UNICODE);

  // Hook the blob to the FT_Face
  ft_face->generic.దత్తాంశం = blob;
  ft_face->generic.finalizer = _release_blob;

  // And the FT_Library to the blob
  ఒకవేళ (unlikely (!hb_blob_సమితి_user_దత్తాంశం (blob, &ft_library_కీలకం, ft_library, destroy_ft_library, నిజం)))
  {
    DEBUG_సందేశం (FT, font, "hb_blob_set_user_data() failed");
    _hb_ft_face_destroy_static (ft_face);
    destroy_ft_library (ft_library);
    తిరిగి;
  }

  hb_ft_font_సమితి_funcs_2 (font, ft_face, నిజం, నిజం);
  hb_ft_font_సమితి_ఎక్కించు_flags (font, ఎక్కించు_flags);

  hb_ft_hb_font_మారింది (font, ft_face);
}

#అయితే_ముగింపు
