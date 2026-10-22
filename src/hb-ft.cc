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

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_FREETYPE

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ft.h"

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-cache.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-draw.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-font.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-machinery.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-os2-table.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-shaper-arabic-pua.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-paint.hh"

#ਅਨ੍ਤਰ੍ਭਾਵ FT_MODULE_H
#ਅਨ੍ਤਰ੍ਭਾਵ FT_ADVANCES_H
#ਅਨ੍ਤਰ੍ਭਾਵ FT_ਗੁਣਜ_MASTERS_H
#ਅਨ੍ਤਰ੍ਭਾਵ FT_OUTLINE_H
#ਅਨ੍ਤਰ੍ਭਾਵ FT_TRUETYPE_TABLES_H
#ਅਨ੍ਤਰ੍ਭਾਵ FT_SYNTHESIS_H
#ਜੇ (FREETYPE_MAJOR*10000 + FREETYPE_MINOR*100 + FREETYPE_PATCH) >= 21300
#ਅਨ੍ਤਰ੍ਭਾਵ FT_COLOR_H
#ਜੇ_ਅੰਤ


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


ਵਰਤੋਂ hb_ft_advance_cache_t = hb_cache_t<16, 24, 8, ਝੂਠ>;

ਸਥਿਰ ਖਾਲੀ _hb_ft_face_destroy_static (ਖਾਲੀ *ਅੰਕੜੇ);

ਬਣਤਰ hb_ft_font_t
{
  ਪੂਰਨ ਲੱਦੋ_flags;
  ਤਾਰਕਿਕ ਚਿੰਨ੍ਹ; /* Whether selected cmap is symbol cmap. */
  ਤਾਰਕਿਕ unref; /* Whether to destroy ft_face when done. */
  ਤਾਰਕਿਕ transform; /* Whether to apply FT_Face's transform. */
  ਤਾਰਕਿਕ static_library; /* Whether ft_face uses static_ft_library. */

  ਬਦਲਣਯੋਗ hb_mutex_t lock; /* Protects members below. */
  FT_Face ft_face;
  ਬਦਲਣਯੋਗ hb_atomic_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> cached_serial;
  ਬਦਲਣਯੋਗ hb_ft_advance_cache_t advance_cache;
};

ਸਥਿਰ hb_ft_font_t *
_hb_ft_font_create (FT_Face ft_face, ਤਾਰਕਿਕ ਚਿੰਨ੍ਹ, ਤਾਰਕਿਕ unref, ਤਾਰਕਿਕ static_library)
{
  hb_ft_font_t *ft_font = (hb_ft_font_t *) hb_calloc (1, ਆਕਾਰ (hb_ft_font_t));
  ਜੇ (unlikely (!ft_font)) ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

  ft_font->lock.init ();
  ft_font->ft_face = ft_face;
  ft_font->ਚਿੰਨ੍ਹ = ਚਿੰਨ੍ਹ;
  ft_font->unref = unref;
  ft_font->static_library = static_library;

  ft_font->ਲੱਦੋ_flags = FT_ਲੱਦੋ_DEFAULT | FT_ਲੱਦੋ_NO_HINTING;

  ft_font->cached_serial = UINT_MAX;
  ਨਵਾਂ (&ft_font->advance_cache) hb_ft_advance_cache_t;

  ਵਾਪਸ ft_font;
}

ਸਥਿਰ ਖਾਲੀ
_hb_ft_face_destroy (ਖਾਲੀ *ਅੰਕੜੇ)
{
  FT_ਪੂਰਾ_Face ((FT_Face) ਅੰਕੜੇ);
}

ਸਥਿਰ ਖਾਲੀ
_hb_ft_font_destroy (ਖਾਲੀ *ਅੰਕੜੇ)
{
  hb_ft_font_t *ft_font = (hb_ft_font_t *) ਅੰਕੜੇ;

  ਜੇ (ft_font->unref)
  {
    ਜੇ (ft_font->static_library)
    {
      /* ft_font->ft_face->generic.data is the blob that hb_ft_font_set_funcs()
       * hooked up as this internal FT_Face's finalizer target (_release_blob).
       * FT_Done_Face() below runs that finalizer while static_ft_library_mutex
       * is held; if dropping its reference were to free the blob right there,
       * the blob's attached destroy_ft_library() user-data callback would try
       * to re-lock the same non-recursive mutex and self-deadlock. Hold an
       * extra reference across the locked call so the blob (and thus that
       * callback) can only be freed after the lock is released. */
      hb_blob_t *blob = hb_blob_reference ((hb_blob_t *) ft_font->ft_face->generic.ਅੰਕੜੇ);
      _hb_ft_face_destroy_static (ft_font->ft_face);
      hb_blob_destroy (blob);
    }
    ਨਹੀਂਤਾਂ
      _hb_ft_face_destroy (ft_font->ft_face);
  }

  ft_font->lock.fini ();

  hb_free (ft_font);
}


/* hb_font changed, update FT_Face. */
ਸਥਿਰ ਖਾਲੀ hb_ft_hb_font_ਬਦਲਿਆ (hb_font_t *font, FT_Face ft_face)
{
  ਜੇ (unlikely (font->destroy != (hb_destroy_func_t) _hb_ft_font_destroy))
    ਵਾਪਸ;

  hb_ft_font_t *ft_font = (hb_ft_font_t *) font->user_ਅੰਕੜੇ;

  ਦਸ਼ਮਲਵ x_mult = 1.f, y_mult = 1.f;

  ਜੇ (font->x_scale < 0) x_mult = -x_mult;
  ਜੇ (font->y_scale < 0) y_mult = -y_mult;

  ਜੇ (FT_ਸੈੱਟ_Char_ਮਾਪ (ft_face,
			ਨਿਰਪੇਖ (font->x_scale), ਨਿਰਪੇਖ (font->y_scale),
			0, 0
#ਜੇ 0
		    font->x_ppem * 72 * 64 / font->x_scale,
		    font->y_ppem * 72 * 64 / font->y_scale
#ਜੇ_ਅੰਤ
     ) && ft_face->ਅੰਕ_fixed_sizes)
  {
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_FT_GET_TRANSFORM
    /* Bitmap font, eg. bitmap color emoji. */
    /* Pick largest size? */
    ਪੂਰਨ x_scale  = ft_face->ਉਪਲਬਧ_sizes[ft_face->ਅੰਕ_fixed_sizes - 1].x_ppem;
    ਪੂਰਨ y_scale = ft_face->ਉਪਲਬਧ_sizes[ft_face->ਅੰਕ_fixed_sizes - 1].y_ppem;
    FT_ਸੈੱਟ_Char_ਮਾਪ (ft_face,
		      x_scale, y_scale,
		      0, 0);

    /* This contains the sign that was previously in x_mult/y_mult. */
    x_mult = (ਦਸ਼ਮਲਵ) font->x_scale / x_scale;
    y_mult = (ਦਸ਼ਮਲਵ) font->y_scale / y_scale;
#ਜੇ_ਅੰਤ
  }
  ਨਹੀਂਤਾਂ
  { /* Shrug */ }


  ਜੇ (x_mult != 1.f || y_mult != 1.f)
  {
    FT_ਮੈਟ੍ਰਿਕਸ ਮੈਟ੍ਰਿਕਸ = { (ਪੂਰਨ) roundf (x_mult * (1<<16)), 0,
			  0, (ਪੂਰਨ) roundf (y_mult * (1<<16))};
    FT_ਸੈੱਟ_Transform (ft_face, &ਮੈਟ੍ਰਿਕਸ, ਖਾਲੀ_ਸੂਚਕ);
    ft_font->transform = ਸੱਚ;
  }
  ਨਹੀਂਤਾਂ
    FT_ਸੈੱਟ_Transform (ft_face, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);

#ਜੇ defined(HAVE_FT_GET_VAR_BLEND_COORDINATES) && !defined(HB_NO_VAR)
  ਜੇ (font->has_nonzero_coords)
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ_coords;
    ਅਚਲ ਦਸ਼ਮਲਵ *coords = hb_font_get_var_coords_design (font, &ਅੰਕ_coords);
    FT_Fixed *ft_coords = (FT_Fixed *) hb_calloc (ਅੰਕ_coords, ਆਕਾਰ (FT_Fixed));
    ਜੇ (ft_coords)
    {
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਅੰਕ_coords; i++)
	  ft_coords[i] = coords[i] * 65536.f;
      FT_ਸੈੱਟ_Var_Design_Coordinates (ft_face, ਅੰਕ_coords, ft_coords);
      hb_free (ft_coords);
    }
  }
  ਨਹੀਂਤਾਂ ਜੇ (font->ਅੰਕ_coords)
  {
    // Some old versions of FreeType crash if we
    // call this function on non-variable fonts.
    FT_ਸੈੱਟ_Var_Design_Coordinates (ft_face, 0, ਖਾਲੀ_ਸੂਚਕ);
  }
#ਜੇ_ਅੰਤ
}

/* Check if hb_font changed, update FT_Face. */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ
hb_ft_hb_font_check_ਬਦਲਿਆ (hb_font_t *font,
			      ਅਚਲ hb_ft_font_t *ft_font)
{
  ਜੇ (font->serial != ft_font->cached_serial)
  {
    hb_ਤਾਲਾ_t lock (ft_font->lock);
    hb_ft_hb_font_ਬਦਲਿਆ (font, ft_font->ft_face);
    ft_font->advance_cache.clear ();
    ft_font->cached_serial.ਸੈੱਟ_release (font->serial.get_acquire ());
    ਵਾਪਸ ਸੱਚ;
  }
  ਵਾਪਸ ਝੂਠ;
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
ਖਾਲੀ
hb_ft_font_ਸੈੱਟ_ਲੱਦੋ_flags (hb_font_t *font, ਪੂਰਨ ਲੱਦੋ_flags)
{
  ਜੇ (hb_object_is_immutable (font))
    ਵਾਪਸ;

  ਜੇ (unlikely (font->destroy != (hb_destroy_func_t) _hb_ft_font_destroy))
    ਵਾਪਸ;

  hb_ft_font_t *ft_font = (hb_ft_font_t *) font->user_ਅੰਕੜੇ;

  ft_font->ਲੱਦੋ_flags = ਲੱਦੋ_flags;
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
ਪੂਰਨ
hb_ft_font_get_ਲੱਦੋ_flags (hb_font_t *font)
{
  ਜੇ (unlikely (font->destroy != (hb_destroy_func_t) _hb_ft_font_destroy))
    ਵਾਪਸ 0;

  ਅਚਲ hb_ft_font_t *ft_font = (ਅਚਲ hb_ft_font_t *) font->user_ਅੰਕੜੇ;

  ਵਾਪਸ ft_font->ਲੱਦੋ_flags;
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
  ਜੇ (unlikely (font->destroy != (hb_destroy_func_t) _hb_ft_font_destroy))
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

  ਅਚਲ hb_ft_font_t *ft_font = (ਅਚਲ hb_ft_font_t *) font->user_ਅੰਕੜੇ;

  ਵਾਪਸ ft_font->ft_face;
}

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_DISABLE_DEPRECATED

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
  ਵਾਪਸ hb_ft_font_get_ft_face (font);
}

#ਜੇ_ਅੰਤ

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
hb_ft_font_ਤਾਲਾ_face (hb_font_t *font)
{
  ਜੇ (unlikely (font->destroy != (hb_destroy_func_t) _hb_ft_font_destroy))
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

  ਅਚਲ hb_ft_font_t *ft_font = (ਅਚਲ hb_ft_font_t *) font->user_ਅੰਕੜੇ;

  ft_font->lock.lock ();

  ਵਾਪਸ ft_font->ft_face;
}

/**
 * hb_ft_font_unlock_face: (skip)
 * @font: #hb_font_t to work upon
 *
 * Releases an FT_Face previously obtained with hb_ft_font_lock_face().
 *
 * Since: 2.6.5
 **/
ਖਾਲੀ
hb_ft_font_unlock_face (hb_font_t *font)
{
  ਜੇ (unlikely (font->destroy != (hb_destroy_func_t) _hb_ft_font_destroy))
    ਵਾਪਸ;

  ਅਚਲ hb_ft_font_t *ft_font = (ਅਚਲ hb_ft_font_t *) font->user_ਅੰਕੜੇ;

  ft_font->lock.unlock ();
}


ਸਥਿਰ hb_bool_t
hb_ft_get_nominal_glyph (hb_font_t *font,
			 ਖਾਲੀ *font_ਅੰਕੜੇ,
			 hb_codepoint_t unicode,
			 hb_codepoint_t *glyph,
			 ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  ਅਚਲ hb_ft_font_t *ft_font = (ਅਚਲ hb_ft_font_t *) font_ਅੰਕੜੇ;
  hb_ਤਾਲਾ_t lock (ft_font->lock);
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ g = FT_Get_Char_ਸੂਚਕਾਂਕ (ft_font->ft_face, unicode);

  ਜੇ (unlikely (!g))
  {
    ਜੇ (unlikely (ft_font->ਚਿੰਨ੍ਹ))
    {
      ਚੋਣ ((ਬਿਨਾਨਿਸ਼ਾਨੀ) font->face->ਸਾਰਣੀ.OS2->get_font_ਪੰਨਾ ()) {
      ਹਾਲਤ OT::OS2::font_ਪੰਨਾ_t::FONT_ਪੰਨਾ_NONE:
	ਜੇ (unicode <= 0x00FFu)
	  /* For symbol-encoded OpenType fonts, we duplicate the
	   * U+F000..F0FF range at U+0000..U+00FF.  That's what
	   * Windows seems to do, and that's hinted about at:
	   * https://docs.microsoft.com/en-us/typography/opentype/spec/recom
	   * under "Non-Standard (Symbol) Fonts". */
	  g = FT_Get_Char_ਸੂਚਕਾਂਕ (ft_font->ft_face, 0xF000u + unicode);
	ਰੋਕੋ;
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_OT_SHAPER_ARABIC_FALLBACK
      ਹਾਲਤ OT::OS2::font_ਪੰਨਾ_t::FONT_ਪੰਨਾ_SIMP_ARABIC:
	g = FT_Get_Char_ਸੂਚਕਾਂਕ (ft_font->ft_face, _hb_arabic_pua_simp_map (unicode));
	ਰੋਕੋ;
      ਹਾਲਤ OT::OS2::font_ਪੰਨਾ_t::FONT_ਪੰਨਾ_TRAD_ARABIC:
	g = FT_Get_Char_ਸੂਚਕਾਂਕ (ft_font->ft_face, _hb_arabic_pua_trad_map (unicode));
	ਰੋਕੋ;
#ਜੇ_ਅੰਤ
      ਮੂਲ:
	ਰੋਕੋ;
      }
      ਜੇ (!g)
	ਵਾਪਸ ਝੂਠ;
    }
    ਨਹੀਂਤਾਂ
      ਵਾਪਸ ਝੂਠ;
  }

  *glyph = g;
  ਵਾਪਸ ਸੱਚ;
}

ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ
hb_ft_get_nominal_glyphs (hb_font_t *font HB_ਅਣਵਰਤਿਆ,
			  ਖਾਲੀ *font_ਅੰਕੜੇ,
			  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2,
			  ਅਚਲ hb_codepoint_t *ਪਹਿਲਾ_unicode,
			  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ unicode_stride,
			  hb_codepoint_t *ਪਹਿਲਾ_glyph,
			  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ glyph_stride,
			  ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  ਅਚਲ hb_ft_font_t *ft_font = (ਅਚਲ hb_ft_font_t *) font_ਅੰਕੜੇ;
  hb_ਤਾਲਾ_t lock (ft_font->lock);
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਪੂਰਾ;
  ਲਈ (ਪੂਰਾ = 0;
       ਪੂਰਾ < ਗਿਣਤੀ_2 && (*ਪਹਿਲਾ_glyph = FT_Get_Char_ਸੂਚਕਾਂਕ (ft_font->ft_face, *ਪਹਿਲਾ_unicode));
       ਪੂਰਾ++)
  {
    ਪਹਿਲਾ_unicode = &Struct_At_ਵਿੱਥ_Unaligned<hb_codepoint_t> (ਪਹਿਲਾ_unicode, unicode_stride);
    ਪਹਿਲਾ_glyph = &Struct_At_ਵਿੱਥ_Unaligned<hb_codepoint_t> (ਪਹਿਲਾ_glyph, glyph_stride);
  }
  /* We don't need to do ft_font->symbol dance here, since HB calls the singular
   * nominal_glyph() for what we don't handle here. */
  ਵਾਪਸ ਪੂਰਾ;
}


ਸਥਿਰ hb_bool_t
hb_ft_get_variation_glyph (hb_font_t *font HB_ਅਣਵਰਤਿਆ,
			   ਖਾਲੀ *font_ਅੰਕੜੇ,
			   hb_codepoint_t unicode,
			   hb_codepoint_t variation_selector,
			   hb_codepoint_t *glyph,
			   ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  ਅਚਲ hb_ft_font_t *ft_font = (ਅਚਲ hb_ft_font_t *) font_ਅੰਕੜੇ;
  hb_ਤਾਲਾ_t lock (ft_font->lock);
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ g = FT_Face_Get_Char_Variant_ਸੂਚਕਾਂਕ (ft_font->ft_face, unicode, variation_selector);

  ਜੇ (unlikely (!g))
    ਵਾਪਸ ਝੂਠ;

  *glyph = g;
  ਵਾਪਸ ਸੱਚ;
}

ਸਥਿਰ ਖਾਲੀ
hb_ft_get_glyph_h_advances (hb_font_t* font, ਖਾਲੀ* font_ਅੰਕੜੇ,
			    ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2,
			    ਅਚਲ hb_codepoint_t *ਪਹਿਲਾ_glyph,
			    ਬਿਨਾਨਿਸ਼ਾਨੀ glyph_stride,
			    hb_position_t *ਪਹਿਲਾ_advance,
			    ਬਿਨਾਨਿਸ਼ਾਨੀ advance_stride,
			    ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  ਅਚਲ hb_ft_font_t *ft_font = (ਅਚਲ hb_ft_font_t *) font_ਅੰਕੜੇ;
  hb_ft_hb_font_check_ਬਦਲਿਆ (font, ft_font);

  hb_ਤਾਲਾ_t lock (ft_font->lock);
  FT_Face ft_face = ft_font->ft_face;
  ਪੂਰਨ ਲੱਦੋ_flags = ft_font->ਲੱਦੋ_flags;
  ਦਸ਼ਮਲਵ x_mult;
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_FT_GET_TRANSFORM
  ਜੇ (ft_font->transform)
  {
    FT_ਮੈਟ੍ਰਿਕਸ ਮੈਟ੍ਰਿਕਸ;
    FT_Get_Transform (ft_face, &ਮੈਟ੍ਰਿਕਸ, ਖਾਲੀ_ਸੂਚਕ);
    x_mult = sqrtf ((ਦਸ਼ਮਲਵ)ਮੈਟ੍ਰਿਕਸ.xx * ਮੈਟ੍ਰਿਕਸ.xx + (ਦਸ਼ਮਲਵ)ਮੈਟ੍ਰਿਕਸ.xy * ਮੈਟ੍ਰਿਕਸ.xy) / 65536.f;
    x_mult *= font->x_scale < 0 ? -1 : +1;
  }
  ਨਹੀਂਤਾਂ
#ਜੇ_ਅੰਤ
  {
    x_mult = font->x_scale < 0 ? -1 : +1;
  }

  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++)
  {
    hb_position_t advance;
    hb_codepoint_t glyph = *ਪਹਿਲਾ_glyph;

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ cv;
    ਜੇ (ft_font->advance_cache.get (glyph, &cv))
      advance = (hb_position_t) cv;
    ਨਹੀਂਤਾਂ
    {
      FT_Fixed v = 0;
      FT_Get_Advance (ft_face, glyph, ਲੱਦੋ_flags, &v);
      /* Work around bug that FreeType seems to return negative advance
       * for variable-set fonts if x_scale is negative! */
      ਦੁੱਗਣਾ scaled = trunc (fabs ((ਦੁੱਗਣਾ) v) * (ਦੁੱਗਣਾ) x_mult + (1<<9)) / (1<<10);
      advance = hb_clamp_ਮੰਜ਼ਿਲ<hb_position_t> (floor (scaled));
      ft_font->advance_cache.ਸੈੱਟ (glyph, advance);
    }

    *ਪਹਿਲਾ_advance = advance;
    ਪਹਿਲਾ_glyph = &Struct_At_ਵਿੱਥ_Unaligned<hb_codepoint_t> (ਪਹਿਲਾ_glyph, glyph_stride);
    ਪਹਿਲਾ_advance = &Struct_At_ਵਿੱਥ_Unaligned<hb_position_t> (ਪਹਿਲਾ_advance, advance_stride);
  }
}

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_VERTICAL
ਸਥਿਰ hb_position_t
hb_ft_get_glyph_v_advance (hb_font_t *font,
			   ਖਾਲੀ *font_ਅੰਕੜੇ,
			   hb_codepoint_t glyph,
			   ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  ਅਚਲ hb_ft_font_t *ft_font = (ਅਚਲ hb_ft_font_t *) font_ਅੰਕੜੇ;
  hb_ft_hb_font_check_ਬਦਲਿਆ (font, ft_font);

  hb_ਤਾਲਾ_t lock (ft_font->lock);
  FT_Fixed v;
  ਦਸ਼ਮਲਵ y_mult;
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_FT_GET_TRANSFORM
  ਜੇ (ft_font->transform)
  {
    FT_ਮੈਟ੍ਰਿਕਸ ਮੈਟ੍ਰਿਕਸ;
    FT_Get_Transform (ft_font->ft_face, &ਮੈਟ੍ਰਿਕਸ, ਖਾਲੀ_ਸੂਚਕ);
    y_mult = sqrtf ((ਦਸ਼ਮਲਵ)ਮੈਟ੍ਰਿਕਸ.yx * ਮੈਟ੍ਰਿਕਸ.yx + (ਦਸ਼ਮਲਵ)ਮੈਟ੍ਰਿਕਸ.yy * ਮੈਟ੍ਰਿਕਸ.yy) / 65536.f;
    y_mult *= font->y_scale < 0 ? -1 : +1;
  }
  ਨਹੀਂਤਾਂ
#ਜੇ_ਅੰਤ
  {
    y_mult = font->y_scale < 0 ? -1 : +1;
  }

  ਜੇ (unlikely (FT_Get_Advance (ft_font->ft_face, glyph, ft_font->ਲੱਦੋ_flags | FT_ਲੱਦੋ_VERTICAL_LAYOUT, &v)))
    ਵਾਪਸ 0;

  /* Note: FreeType's vertical metrics grows downward while other FreeType coordinates
   * have a Y growing upward.  Hence the extra negation. */
  ਦੁੱਗਣਾ advance = floor ((-(ਦੁੱਗਣਾ) v + (1<<9)) / (1<<10));

  ਵਾਪਸ hb_clamp_ਮੰਜ਼ਿਲ<hb_position_t> ((ਦੁੱਗਣਾ) y_mult * advance);
}
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_VERTICAL
ਸਥਿਰ hb_bool_t
hb_ft_get_glyph_v_origin (hb_font_t *font,
			  ਖਾਲੀ *font_ਅੰਕੜੇ,
			  hb_codepoint_t glyph,
			  hb_position_t *x,
			  hb_position_t *y,
			  ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  ਅਚਲ hb_ft_font_t *ft_font = (ਅਚਲ hb_ft_font_t *) font_ਅੰਕੜੇ;
  hb_ft_hb_font_check_ਬਦਲਿਆ (font, ft_font);

  hb_ਤਾਲਾ_t lock (ft_font->lock);
  FT_Face ft_face = ft_font->ft_face;
  ਦਸ਼ਮਲਵ x_mult, y_mult;
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_FT_GET_TRANSFORM
  ਜੇ (ft_font->transform)
  {
    FT_ਮੈਟ੍ਰਿਕਸ ਮੈਟ੍ਰਿਕਸ;
    FT_Get_Transform (ft_face, &ਮੈਟ੍ਰਿਕਸ, ਖਾਲੀ_ਸੂਚਕ);
    x_mult = sqrtf ((ਦਸ਼ਮਲਵ)ਮੈਟ੍ਰਿਕਸ.xx * ਮੈਟ੍ਰਿਕਸ.xx + (ਦਸ਼ਮਲਵ)ਮੈਟ੍ਰਿਕਸ.xy * ਮੈਟ੍ਰਿਕਸ.xy) / 65536.f;
    x_mult *= font->x_scale < 0 ? -1 : +1;
    y_mult = sqrtf ((ਦਸ਼ਮਲਵ)ਮੈਟ੍ਰਿਕਸ.yx * ਮੈਟ੍ਰਿਕਸ.yx + (ਦਸ਼ਮਲਵ)ਮੈਟ੍ਰਿਕਸ.yy * ਮੈਟ੍ਰਿਕਸ.yy) / 65536.f;
    y_mult *= font->y_scale < 0 ? -1 : +1;
  }
  ਨਹੀਂਤਾਂ
#ਜੇ_ਅੰਤ
  {
    x_mult = font->x_scale < 0 ? -1 : +1;
    y_mult = font->y_scale < 0 ? -1 : +1;
  }

  ਜੇ (unlikely (FT_ਲੱਦੋ_Glyph (ft_face, glyph, ft_font->ਲੱਦੋ_flags)))
    ਵਾਪਸ ਝੂਠ;

  /* Note: FreeType's vertical metrics grows downward while other FreeType coordinates
   * have a Y growing upward.  Hence the extra negation. */
  *x = hb_clamp_ਮੰਜ਼ਿਲ<hb_position_t> ((ਦੁੱਗਣਾ) x_mult *
				   ((ਦੁੱਗਣਾ) ft_face->glyph->metrics.horiBearingX -
				    ft_face->glyph->metrics.vertBearingX));
  *y = hb_clamp_ਮੰਜ਼ਿਲ<hb_position_t> ((ਦੁੱਗਣਾ) y_mult *
				   ((ਦੁੱਗਣਾ) ft_face->glyph->metrics.horiBearingY +
				    ft_face->glyph->metrics.vertBearingY));

  ਵਾਪਸ ਸੱਚ;
}
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_OT_SHAPE_FALLBACK
ਸਥਿਰ hb_position_t
hb_ft_get_glyph_h_kerning (hb_font_t *font,
			   ਖਾਲੀ *font_ਅੰਕੜੇ,
			   hb_codepoint_t ਖੱਬਾ_glyph,
			   hb_codepoint_t ਸੱਜਾ_glyph,
			   ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  ਅਚਲ hb_ft_font_t *ft_font = (ਅਚਲ hb_ft_font_t *) font_ਅੰਕੜੇ;
  hb_ft_hb_font_check_ਬਦਲਿਆ (font, ft_font);

  hb_ਤਾਲਾ_t lock (ft_font->lock);
  FT_ਸਦਿਸ਼ kerningv;

  FT_Kerning_ਬਹੁਲਕ ਬਹੁਲਕ = font->x_ppem ? FT_KERNING_DEFAULT : FT_KERNING_UNFITTED;
  ਜੇ (FT_Get_Kerning (ft_font->ft_face, ਖੱਬਾ_glyph, ਸੱਜਾ_glyph, ਬਹੁਲਕ, &kerningv))
    ਵਾਪਸ 0;

  ਵਾਪਸ hb_clamp_ਮੰਜ਼ਿਲ<hb_position_t> ((int64_t) kerningv.x);
}
#ਜੇ_ਅੰਤ

ਸਥਿਰ ਤਾਰਕਿਕ
hb_ft_is_colr_glyph (hb_font_t *font,
		     ਖਾਲੀ *font_ਅੰਕੜੇ,
		     hb_codepoint_t gid)
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_PAINT
#ਜੇ (FREETYPE_MAJOR*10000 + FREETYPE_MINOR*100 + FREETYPE_PATCH) >= 21300
  ਅਚਲ hb_ft_font_t *ft_font = (ਅਚਲ hb_ft_font_t *) font_ਅੰਕੜੇ;
  FT_Face ft_face = ft_font->ft_face;


  /* COLRv1 */
  FT_OpaquePaint paint = {0};
  ਜੇ (FT_Get_Color_Glyph_Paint (ft_face, gid,
			        FT_COLOR_NO_ਜੜ੍ਹ_TRANSFORM,
			        &paint))
    ਵਾਪਸ ਸੱਚ;

  /* COLRv0 */
  FT_LayerIterator  iterator;
  FT_UInt  layer_glyph_ਸੂਚਕਾਂਕ;
  FT_UInt  layer_color_ਸੂਚਕਾਂਕ;
  iterator.p  = ਨਿਰਾ;
  ਜੇ (FT_Get_Color_Glyph_Layer (ft_face,
				gid,
				&layer_glyph_ਸੂਚਕਾਂਕ,
				&layer_color_ਸੂਚਕਾਂਕ,
				&iterator))
    ਵਾਪਸ ਸੱਚ;
#ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ

  ਵਾਪਸ ਝੂਠ;
}

ਸਥਿਰ hb_bool_t
hb_ft_get_glyph_extents (hb_font_t *font,
			 ਖਾਲੀ *font_ਅੰਕੜੇ,
			 hb_codepoint_t glyph,
			 hb_glyph_extents_t *extents,
			 ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  // FreeType doesn't return COLR glyph extents.
  ਜੇ (hb_ft_is_colr_glyph (font, font_ਅੰਕੜੇ, glyph))
    ਵਾਪਸ ਝੂਠ;

  ਅਚਲ hb_ft_font_t *ft_font = (ਅਚਲ hb_ft_font_t *) font_ਅੰਕੜੇ;
  hb_ft_hb_font_check_ਬਦਲਿਆ (font, ft_font);

  hb_ਤਾਲਾ_t lock (ft_font->lock);
  FT_Face ft_face = ft_font->ft_face;
  ਦਸ਼ਮਲਵ x_mult, y_mult;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_FT_GET_TRANSFORM
  ਜੇ (ft_font->transform)
  {
    FT_ਮੈਟ੍ਰਿਕਸ ਮੈਟ੍ਰਿਕਸ;
    FT_Get_Transform (ft_face, &ਮੈਟ੍ਰਿਕਸ, ਖਾਲੀ_ਸੂਚਕ);
    x_mult = sqrtf ((ਦਸ਼ਮਲਵ)ਮੈਟ੍ਰਿਕਸ.xx * ਮੈਟ੍ਰਿਕਸ.xx + (ਦਸ਼ਮਲਵ)ਮੈਟ੍ਰਿਕਸ.xy * ਮੈਟ੍ਰਿਕਸ.xy) / 65536.f;
    x_mult *= font->x_scale < 0 ? -1 : +1;
    y_mult = sqrtf ((ਦਸ਼ਮਲਵ)ਮੈਟ੍ਰਿਕਸ.yx * ਮੈਟ੍ਰਿਕਸ.yx + (ਦਸ਼ਮਲਵ)ਮੈਟ੍ਰਿਕਸ.yy * ਮੈਟ੍ਰਿਕਸ.yy) / 65536.f;
    y_mult *= font->y_scale < 0 ? -1 : +1;
  }
  ਨਹੀਂਤਾਂ
#ਜੇ_ਅੰਤ
  {
    x_mult = font->x_scale < 0 ? -1 : +1;
    y_mult = font->y_scale < 0 ? -1 : +1;
  }

  ਜੇ (unlikely (FT_ਲੱਦੋ_Glyph (ft_face, glyph, ft_font->ਲੱਦੋ_flags)))
    ਵਾਪਸ ਝੂਠ;

  /* Copied from hb_font_t::scale_glyph_extents. */

  ਦਸ਼ਮਲਵ x1 = x_mult * ft_face->glyph->metrics.horiBearingX;
  ਦਸ਼ਮਲਵ y1 = y_mult * ft_face->glyph->metrics.horiBearingY;
  ਦਸ਼ਮਲਵ x2 = x1 + x_mult *  ft_face->glyph->metrics.ਚੌੜਾਈ;
  ਦਸ਼ਮਲਵ y2 = y1 + y_mult * -ft_face->glyph->metrics.ਉਚਾਈ;

  ਦੁੱਗਣਾ rx1 = (ਦੁੱਗਣਾ) roundf (x1);
  ਦੁੱਗਣਾ ry1 = (ਦੁੱਗਣਾ) roundf (y1);
  ਦੁੱਗਣਾ rx2 = (ਦੁੱਗਣਾ) roundf (x2);
  ਦੁੱਗਣਾ ry2 = (ਦੁੱਗਣਾ) roundf (y2);

  extents->x_bearing = hb_clamp_ਮੰਜ਼ਿਲ<hb_position_t> (rx1);
  extents->y_bearing = hb_clamp_ਮੰਜ਼ਿਲ<hb_position_t> (ry1);
  extents->ਚੌੜਾਈ = hb_clamp_ਮੰਜ਼ਿਲ<hb_position_t> (rx2 - rx1);
  extents->ਉਚਾਈ = hb_clamp_ਮੰਜ਼ਿਲ<hb_position_t> (ry2 - ry1);

  ਵਾਪਸ ਸੱਚ;
}

ਸਥਿਰ hb_bool_t
hb_ft_get_glyph_contour_ਬਿੰਦੂ (hb_font_t *font HB_ਅਣਵਰਤਿਆ,
			       ਖਾਲੀ *font_ਅੰਕੜੇ,
			       hb_codepoint_t glyph,
			       ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਬਿੰਦੂ_ਸੂਚਕਾਂਕ,
			       hb_position_t *x,
			       hb_position_t *y,
			       ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  ਅਚਲ hb_ft_font_t *ft_font = (ਅਚਲ hb_ft_font_t *) font_ਅੰਕੜੇ;
  hb_ft_hb_font_check_ਬਦਲਿਆ (font, ft_font);

  hb_ਤਾਲਾ_t lock (ft_font->lock);
  FT_Face ft_face = ft_font->ft_face;

  ਜੇ (unlikely (FT_ਲੱਦੋ_Glyph (ft_face, glyph, ft_font->ਲੱਦੋ_flags)))
      ਵਾਪਸ ਝੂਠ;

  ਜੇ (unlikely (ft_face->glyph->format != FT_GLYPH_FORMAT_OUTLINE))
      ਵਾਪਸ ਝੂਠ;

  ਜੇ (unlikely (ਬਿੰਦੂ_ਸੂਚਕਾਂਕ >= (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) ft_face->glyph->outline.n_points))
      ਵਾਪਸ ਝੂਠ;

  *x = ft_face->glyph->outline.points[ਬਿੰਦੂ_ਸੂਚਕਾਂਕ].x;
  *y = ft_face->glyph->outline.points[ਬਿੰਦੂ_ਸੂਚਕਾਂਕ].y;

  ਵਾਪਸ ਸੱਚ;
}

ਸਥਿਰ hb_bool_t
hb_ft_get_glyph_name (hb_font_t *font HB_ਅਣਵਰਤਿਆ,
		      ਖਾਲੀ *font_ਅੰਕੜੇ,
		      hb_codepoint_t glyph,
		      ਅੱਖਰ *name, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਮਾਪ,
		      ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  ਅਚਲ hb_ft_font_t *ft_font = (ਅਚਲ hb_ft_font_t *) font_ਅੰਕੜੇ;
  hb_ਤਾਲਾ_t lock (ft_font->lock);
  FT_Face ft_face = ft_font->ft_face;

  ਜੇ (!ਮਾਪ)
  {
    ਅੱਖਰ buf[128];
    ਵਾਪਸ !FT_Get_Glyph_Name (ft_face, glyph, buf, ਆਕਾਰ (buf)) && *buf;
  }

  hb_bool_t ret = !FT_Get_Glyph_Name (ft_face, glyph, name, ਮਾਪ);
  ਜੇ (ret && (ਮਾਪ && !*name))
    ret = ਝੂਠ;

  ਵਾਪਸ ret;
}

ਸਥਿਰ hb_bool_t
hb_ft_get_glyph_from_name (hb_font_t *font HB_ਅਣਵਰਤਿਆ,
			   ਖਾਲੀ *font_ਅੰਕੜੇ,
			   ਅਚਲ ਅੱਖਰ *name, ਪੂਰਨ len, /* -1 means nul-terminated */
			   hb_codepoint_t *glyph,
			   ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  ਅਚਲ hb_ft_font_t *ft_font = (ਅਚਲ hb_ft_font_t *) font_ਅੰਕੜੇ;
  hb_ਤਾਲਾ_t lock (ft_font->lock);
  FT_Face ft_face = ft_font->ft_face;

  ਜੇ (len < 0)
    *glyph = FT_Get_Name_ਸੂਚਕਾਂਕ (ft_face, (FT_String *) name);
  ਨਹੀਂਤਾਂ {
    /* Make a nul-terminated version. */
    ਅੱਖਰ buf[128];
    len = hb_min (len, (ਪੂਰਨ) ਆਕਾਰ (buf) - 1);
    strncpy (buf, name, len);
    buf[len] = '\0';
    *glyph = FT_Get_Name_ਸੂਚਕਾਂਕ (ft_face, buf);
  }

  ਜੇ (*glyph == 0)
  {
    /* Check whether the given name was actually the name of glyph 0. */
    ਅੱਖਰ buf[128];
    ਜੇ (!FT_Get_Glyph_Name(ft_face, 0, buf, ਆਕਾਰ (buf)) &&
	len < 0 ? !strcmp (buf, name) : !ਲੜੀ_ਤੁਲਨਾ (buf, name, len))
      ਵਾਪਸ ਸੱਚ;
  }

  ਵਾਪਸ *glyph != 0;
}

ਸਥਿਰ hb_bool_t
hb_ft_get_font_h_extents (hb_font_t *font HB_ਅਣਵਰਤਿਆ,
			  ਖਾਲੀ *font_ਅੰਕੜੇ,
			  hb_font_extents_t *metrics,
			  ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  ਅਚਲ hb_ft_font_t *ft_font = (ਅਚਲ hb_ft_font_t *) font_ਅੰਕੜੇ;
  hb_ft_hb_font_check_ਬਦਲਿਆ (font, ft_font);

  hb_ਤਾਲਾ_t lock (ft_font->lock);
  FT_Face ft_face = ft_font->ft_face;
  ਦਸ਼ਮਲਵ y_mult;
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_FT_GET_TRANSFORM
  ਜੇ (ft_font->transform)
  {
    FT_ਮੈਟ੍ਰਿਕਸ ਮੈਟ੍ਰਿਕਸ;
    FT_Get_Transform (ft_face, &ਮੈਟ੍ਰਿਕਸ, ਖਾਲੀ_ਸੂਚਕ);
    y_mult = sqrtf ((ਦਸ਼ਮਲਵ)ਮੈਟ੍ਰਿਕਸ.yx * ਮੈਟ੍ਰਿਕਸ.yx + (ਦਸ਼ਮਲਵ)ਮੈਟ੍ਰਿਕਸ.yy * ਮੈਟ੍ਰਿਕਸ.yy) / 65536.f;
    y_mult *= font->y_scale < 0 ? -1 : +1;
  }
  ਨਹੀਂਤਾਂ
#ਜੇ_ਅੰਤ
  {
    y_mult = font->y_scale < 0 ? -1 : +1;
  }

  ਜੇ (ft_face->units_ਪ੍ਰਤੀ_EM != 0)
  {
    ਦੁੱਗਣਾ ascender = FT_MulFix(ft_face->ascender, ft_face->ਮਾਪ->metrics.y_scale);
    ਦੁੱਗਣਾ descender = FT_MulFix(ft_face->descender, ft_face->ਮਾਪ->metrics.y_scale);
    ਦੁੱਗਣਾ ਉਚਾਈ = FT_MulFix( ft_face->ਉਚਾਈ, ft_face->ਮਾਪ->metrics.y_scale );
    metrics->ascender = hb_clamp_ਮੰਜ਼ਿਲ<hb_position_t> ((ਦੁੱਗਣਾ) y_mult * ascender);
    metrics->descender = hb_clamp_ਮੰਜ਼ਿਲ<hb_position_t> ((ਦੁੱਗਣਾ) y_mult * descender);
    metrics->ਸਤਰ_gap = hb_clamp_ਮੰਜ਼ਿਲ<hb_position_t> ((ਦੁੱਗਣਾ) y_mult * (ਉਚਾਈ - (ascender - descender)));
  }
  ਨਹੀਂਤਾਂ
  {
    /* Bitmap-only font, eg. color bitmap font. */
    ਦੁੱਗਣਾ ascender = ft_face->ਮਾਪ->metrics.ascender;
    ਦੁੱਗਣਾ descender = ft_face->ਮਾਪ->metrics.descender;
    ਦੁੱਗਣਾ ਉਚਾਈ = ft_face->ਮਾਪ->metrics.ਉਚਾਈ;
    metrics->ascender = hb_clamp_ਮੰਜ਼ਿਲ<hb_position_t> ((ਦੁੱਗਣਾ) y_mult * ascender);
    metrics->descender = hb_clamp_ਮੰਜ਼ਿਲ<hb_position_t> ((ਦੁੱਗਣਾ) y_mult * descender);
    metrics->ਸਤਰ_gap = hb_clamp_ਮੰਜ਼ਿਲ<hb_position_t> ((ਦੁੱਗਣਾ) y_mult * (ਉਚਾਈ - (ascender - descender)));
  }

  ਵਾਪਸ ਸੱਚ;
}

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_DRAW

ਸਥਿਰ ਪੂਰਨ
hb_ft_move_ਮੰਜ਼ਿਲ (ਅਚਲ FT_ਸਦਿਸ਼ *to,
		ਖਾਲੀ *arg)
{
  hb_draw_session_t *drawing = (hb_draw_session_t *) arg;
  drawing->move_ਮੰਜ਼ਿਲ (to->x, to->y);
  ਵਾਪਸ FT_Err_Ok;
}

ਸਥਿਰ ਪੂਰਨ
hb_ft_ਸਤਰ_ਮੰਜ਼ਿਲ (ਅਚਲ FT_ਸਦਿਸ਼ *to,
		ਖਾਲੀ *arg)
{
  hb_draw_session_t *drawing = (hb_draw_session_t *) arg;
  drawing->ਸਤਰ_ਮੰਜ਼ਿਲ (to->x, to->y);
  ਵਾਪਸ FT_Err_Ok;
}

ਸਥਿਰ ਪੂਰਨ
hb_ft_conic_ਮੰਜ਼ਿਲ (ਅਚਲ FT_ਸਦਿਸ਼ *control,
		 ਅਚਲ FT_ਸਦਿਸ਼ *to,
		 ਖਾਲੀ *arg)
{
  hb_draw_session_t *drawing = (hb_draw_session_t *) arg;
  drawing->quadratic_ਮੰਜ਼ਿਲ (control->x, control->y,
			 to->x, to->y);
  ਵਾਪਸ FT_Err_Ok;
}

ਸਥਿਰ ਪੂਰਨ
hb_ft_cubic_ਮੰਜ਼ਿਲ (ਅਚਲ FT_ਸਦਿਸ਼ *control1,
		 ਅਚਲ FT_ਸਦਿਸ਼ *control2,
		 ਅਚਲ FT_ਸਦਿਸ਼ *to,
		 ਖਾਲੀ *arg)
{
  hb_draw_session_t *drawing = (hb_draw_session_t *) arg;
  drawing->cubic_ਮੰਜ਼ਿਲ (control1->x, control1->y,
		     control2->x, control2->y,
		     to->x, to->y);
  ਵਾਪਸ FT_Err_Ok;
}

ਸਥਿਰ hb_bool_t
hb_ft_draw_glyph_or_fail (hb_font_t *font,
			  ਖਾਲੀ *font_ਅੰਕੜੇ,
			  hb_codepoint_t glyph,
			  hb_draw_funcs_t *draw_funcs, ਖਾਲੀ *draw_ਅੰਕੜੇ,
			  ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  ਅਚਲ hb_ft_font_t *ft_font = (ਅਚਲ hb_ft_font_t *) font_ਅੰਕੜੇ;
  hb_ft_hb_font_check_ਬਦਲਿਆ (font, ft_font);

  hb_ਤਾਲਾ_t lock (ft_font->lock);
  FT_Face ft_face = ft_font->ft_face;

  ਜੇ (unlikely (FT_ਲੱਦੋ_Glyph (ft_face, glyph,
			       FT_ਲੱਦੋ_NO_BITMAP | ft_font->ਲੱਦੋ_flags)))
    ਵਾਪਸ ਝੂਠ;

  ਜੇ (ft_face->glyph->format != FT_GLYPH_FORMAT_OUTLINE)
    ਵਾਪਸ ਝੂਠ;

  ਅਚਲ FT_Outline_Funcs outline_funcs = {
    hb_ft_move_ਮੰਜ਼ਿਲ,
    hb_ft_ਸਤਰ_ਮੰਜ਼ਿਲ,
    hb_ft_conic_ਮੰਜ਼ਿਲ,
    hb_ft_cubic_ਮੰਜ਼ਿਲ,
    0, /*ਖਿਸਕਾਓ*/
    0, /* delta */
  };

  hb_draw_session_t draw_session {draw_funcs, draw_ਅੰਕੜੇ};

  FT_Outline_Decompose (&ft_face->glyph->outline,
			&outline_funcs,
			&draw_session);

  ਵਾਪਸ ਸੱਚ;
}
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_PAINT
#ਜੇ (FREETYPE_MAJOR*10000 + FREETYPE_MINOR*100 + FREETYPE_PATCH) >= 21300

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ft-colr.hh"

ਸਥਿਰ hb_bool_t
hb_ft_paint_glyph_or_fail (hb_font_t *font,
			   ਖਾਲੀ *font_ਅੰਕੜੇ,
			   hb_codepoint_t gid,
			   hb_paint_funcs_t *paint_funcs, ਖਾਲੀ *paint_ਅੰਕੜੇ,
			   ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ palette_ਸੂਚਕਾਂਕ,
			   hb_color_t foreground,
			   ਖਾਲੀ *user_ਅੰਕੜੇ)
{
  ਅਚਲ hb_ft_font_t *ft_font = (ਅਚਲ hb_ft_font_t *) font_ਅੰਕੜੇ;
  hb_ft_hb_font_check_ਬਦਲਿਆ (font, ft_font);

  hb_ਤਾਲਾ_t lock (ft_font->lock);
  FT_Face ft_face = ft_font->ft_face;

  FT_Long ਲੱਦੋ_flags = ft_font->ਲੱਦੋ_flags | FT_ਲੱਦੋ_COLOR;
#ਜੇ (FREETYPE_MAJOR*10000 + FREETYPE_MINOR*100 + FREETYPE_PATCH) >= 21301
  ਲੱਦੋ_flags |= FT_ਲੱਦੋ_NO_SVG;
#ਜੇ_ਅੰਤ

  /* We release the lock before calling into glyph callbacks, such that
   * eg. draw API can call back into the face.*/

  ਜੇ (unlikely (FT_ਲੱਦੋ_Glyph (ft_face, gid, ਲੱਦੋ_flags)))
    ਵਾਪਸ ਝੂਠ;

  ਜੇ (ft_face->glyph->format == FT_GLYPH_FORMAT_OUTLINE)
  {
    ਜੇ (hb_ft_paint_glyph_colr (font, font_ਅੰਕੜੇ, gid,
				paint_funcs, paint_ਅੰਕੜੇ,
				palette_ਸੂਚਕਾਂਕ, foreground,
				user_ਅੰਕੜੇ))
      ਵਾਪਸ ਸੱਚ;

    // Outline glyph
    ਵਾਪਸ ਝੂਠ;
  }

  ਸਵੈਚਾਲਿਤ *glyph = ft_face->glyph;
  ਜੇ (glyph->format == FT_GLYPH_FORMAT_BITMAP)
  {
    ਤਾਰਕਿਕ ret = ਝੂਠ;
    ਸਵੈਚਾਲਿਤ &bitmap = glyph->bitmap;
    ਜੇ (bitmap.pixel_ਬਹੁਲਕ == FT_PIXEL_ਬਹੁਲਕ_BGRA)
    {
      ਜੇ (bitmap.pitch != (ਨਿਸ਼ਾਨੀਵਾਲਾ) bitmap.ਚੌੜਾਈ * 4)
        ਵਾਪਸ ret;

      ft_font->lock.unlock ();

      hb_blob_t *blob = hb_blob_create ((ਅਚਲ ਅੱਖਰ *) bitmap.ਬਫਰ,
					bitmap.pitch * bitmap.ਪੰਕਤੀਆਂ,
					HB_ਸਮ੍ਰਿਤੀ_ਬਹੁਲਕ_DUPLICATE,
					ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);

      hb_glyph_extents_t extents;
      ਜੇ (!font->get_glyph_extents (gid, &extents, ਝੂਠ))
	ਜਾਓ ਬਾਹਰਲਾ;

      ਜੇ (paint_funcs->image (paint_ਅੰਕੜੇ,
			      blob,
			      bitmap.ਚੌੜਾਈ,
			      bitmap.ਪੰਕਤੀਆਂ,
			      HB_PAINT_IMAGE_FORMAT_BGRA,
			      0.f,
			      &extents))
        ret = ਸੱਚ;

    ਬਾਹਰਲਾ:
      hb_blob_destroy (blob);
      ft_font->lock.lock ();
    }

    ਵਾਪਸ ret;
  }
  ਵਾਪਸ ਝੂਠ;
}
#ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ


ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ free_static_ft_funcs ();

ਸਥਿਰ ਬਣਤਰ hb_ft_font_funcs_lazy_loader_t : hb_font_funcs_lazy_loader_t<hb_ft_font_funcs_lazy_loader_t>
{
  ਸਥਿਰ hb_font_funcs_t *create ()
  {
    hb_font_funcs_t *funcs = hb_font_funcs_create ();

    hb_font_funcs_ਸੈੱਟ_nominal_glyph_func (funcs, hb_ft_get_nominal_glyph, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_font_funcs_ਸੈੱਟ_nominal_glyphs_func (funcs, hb_ft_get_nominal_glyphs, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_font_funcs_ਸੈੱਟ_variation_glyph_func (funcs, hb_ft_get_variation_glyph, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);

    hb_font_funcs_ਸੈੱਟ_font_h_extents_func (funcs, hb_ft_get_font_h_extents, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_font_funcs_ਸੈੱਟ_glyph_h_advances_func (funcs, hb_ft_get_glyph_h_advances, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_VERTICAL
    hb_font_funcs_ਸੈੱਟ_glyph_v_advance_func (funcs, hb_ft_get_glyph_v_advance, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_font_funcs_ਸੈੱਟ_glyph_v_origin_func (funcs, hb_ft_get_glyph_v_origin, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_OT_SHAPE_FALLBACK
    hb_font_funcs_ਸੈੱਟ_glyph_h_kerning_func (funcs, hb_ft_get_glyph_h_kerning, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
#ਜੇ_ਅੰਤ
    hb_font_funcs_ਸੈੱਟ_glyph_extents_func (funcs, hb_ft_get_glyph_extents, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_font_funcs_ਸੈੱਟ_glyph_contour_ਬਿੰਦੂ_func (funcs, hb_ft_get_glyph_contour_ਬਿੰਦੂ, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_font_funcs_ਸੈੱਟ_glyph_name_func (funcs, hb_ft_get_glyph_name, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_font_funcs_ਸੈੱਟ_glyph_from_name_func (funcs, hb_ft_get_glyph_from_name, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_DRAW
    hb_font_funcs_ਸੈੱਟ_draw_glyph_or_fail_func (funcs, hb_ft_draw_glyph_or_fail, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_PAINT
#ਜੇ (FREETYPE_MAJOR*10000 + FREETYPE_MINOR*100 + FREETYPE_PATCH) >= 21300
    hb_font_funcs_ਸੈੱਟ_paint_glyph_or_fail_func (funcs, hb_ft_paint_glyph_or_fail, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
#ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ

    hb_font_funcs_make_immutable (funcs);

    hb_atexit (free_static_ft_funcs);

    ਵਾਪਸ funcs;
  }
} static_ft_funcs;

ਸਥਿਰ ਸਤਰਅੰਦਰ
ਖਾਲੀ free_static_ft_funcs ()
{
  static_ft_funcs.free_instance ();
}

ਸਥਿਰ hb_font_funcs_t *
_hb_ft_get_font_funcs ()
{
  ਵਾਪਸ static_ft_funcs.get_unconst ();
}

ਸਥਿਰ ਖਾਲੀ
hb_ft_font_ਸੈੱਟ_funcs_2 (hb_font_t *font, FT_Face ft_face, ਤਾਰਕਿਕ unref,
		       ਤਾਰਕਿਕ static_library = ਝੂਠ)
{
  ਤਾਰਕਿਕ ਚਿੰਨ੍ਹ = ft_face->charmap && ft_face->charmap->encoding == FT_ENCODING_MS_ਚਿੰਨ੍ਹ;

  hb_ft_font_t *ft_font = _hb_ft_font_create (ft_face, ਚਿੰਨ੍ਹ, unref, static_library);
  ਜੇ (unlikely (!ft_font)) ਵਾਪਸ;

  hb_font_ਸੈੱਟ_funcs (font,
		     _hb_ft_get_font_funcs (),
		     ft_font,
		     _hb_ft_font_destroy);
}


ਸਥਿਰ hb_blob_t *
hb_ft_reference_ਸਾਰਣੀ (hb_face_t *face HB_ਅਣਵਰਤਿਆ, hb_ਟੈਗ_t ਟੈਗ_2, ਖਾਲੀ *user_ਅੰਕੜੇ)
{
  FT_Face ft_face = (FT_Face) user_ਅੰਕੜੇ;
  FT_Byte *ਬਫਰ;
  FT_ULong  ਲੰਮਾਈ = 0;
  FT_Error error;

  /* In new FreeType, a tag value of 1 loads the SFNT table directory. Reject it. */
  ਜੇ (ਟੈਗ_2 == 1)
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

  /* Note: FreeType like HarfBuzz uses the NONE tag for fetching the entire blob */

  error = FT_ਲੱਦੋ_Sfnt_ਸਾਰਣੀ (ft_face, ਟੈਗ_2, 0, ਖਾਲੀ_ਸੂਚਕ, &ਲੰਮਾਈ);
  ਜੇ (error)
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

  ਬਫਰ = (FT_Byte *) hb_malloc (ਲੰਮਾਈ);
  ਜੇ (!ਬਫਰ)
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
  ਸਵੈਚਾਲਿਤ ਬਫਰ_guard = hb_make_scope_guard ([&]() { hb_free (ਬਫਰ); });

  error = FT_ਲੱਦੋ_Sfnt_ਸਾਰਣੀ (ft_face, ਟੈਗ_2, 0, ਬਫਰ, &ਲੰਮਾਈ);
  ਜੇ (error)
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

  ਬਫਰ_guard.release ();
  ਵਾਪਸ hb_blob_create ((ਅਚਲ ਅੱਖਰ *) ਬਫਰ, ਲੰਮਾਈ,
			 HB_ਸਮ੍ਰਿਤੀ_ਬਹੁਲਕ_WRITABLE,
			 ਬਫਰ, hb_free);
}

ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ
hb_ft_get_ਸਾਰਣੀ_ਟੈਗ_ਸੂਚੀ (ਅਚਲ hb_face_t *face HB_ਅਣਵਰਤਿਆ,
		       ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸ਼ੁਰੂ_ਵਿੱਥ,
		       ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *ਸਾਰਣੀ_ਗਿਣਤੀ_2,
		       hb_ਟੈਗ_t *ਸਾਰਣੀ_ਟੈਗ_ਸੂਚੀ,
		       ਖਾਲੀ *user_ਅੰਕੜੇ)
{
  FT_Face ft_face = (FT_Face) user_ਅੰਕੜੇ;

  FT_ULong population = 0;
  FT_Sfnt_ਸਾਰਣੀ_Info (ft_face,
		      0, // table_index; ignored
		      ਖਾਲੀ_ਸੂਚਕ,
                      &population);

  ਜੇ (!ਸਾਰਣੀ_ਗਿਣਤੀ_2)
    ਵਾਪਸ population;

  ਜੇ (unlikely (ਸ਼ੁਰੂ_ਵਿੱਥ >= population))
  {
    *ਸਾਰਣੀ_ਗਿਣਤੀ_2 = 0;
    ਵਾਪਸ population;
  }

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਸਿਰਾ_ਵਿੱਥ = ਸ਼ੁਰੂ_ਵਿੱਥ + *ਸਾਰਣੀ_ਗਿਣਤੀ_2;
  ਜੇ (unlikely (ਸਿਰਾ_ਵਿੱਥ < ਸ਼ੁਰੂ_ਵਿੱਥ)) /* Overflow. */
  {
    *ਸਾਰਣੀ_ਗਿਣਤੀ_2 = 0;
    ਵਾਪਸ population;
  }
  ਸਿਰਾ_ਵਿੱਥ = hb_min (ਸਿਰਾ_ਵਿੱਥ, (ਬਿਨਾਨਿਸ਼ਾਨੀ) population);

  *ਸਾਰਣੀ_ਗਿਣਤੀ_2 = ਸਿਰਾ_ਵਿੱਥ - ਸ਼ੁਰੂ_ਵਿੱਥ;
  ਜੇ (ਸਾਰਣੀ_ਟੈਗ_ਸੂਚੀ)
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = ਸ਼ੁਰੂ_ਵਿੱਥ; i < ਸਿਰਾ_ਵਿੱਥ; i++)
    {
      FT_ULong ਟੈਗ_2 = 0, ਲੰਮਾਈ;
      FT_Sfnt_ਸਾਰਣੀ_Info (ft_face, i, &ਟੈਗ_2, &ਲੰਮਾਈ);
      ਸਾਰਣੀ_ਟੈਗ_ਸੂਚੀ[i - ਸ਼ੁਰੂ_ਵਿੱਥ] = ਟੈਗ_2;
    }

  ਵਾਪਸ population;
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

  ਜੇ (!ft_face->ਧਾਰਾ->ਪਠ) {
    hb_blob_t *blob;

    blob = hb_blob_create ((ਅਚਲ ਅੱਖਰ *) ft_face->ਧਾਰਾ->ਅਧਾਰ,
			   (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) ft_face->ਧਾਰਾ->ਮਾਪ,
			   HB_ਸਮ੍ਰਿਤੀ_ਬਹੁਲਕ_READONLY,
			   ft_face, destroy);
    face = hb_face_create (blob, ft_face->face_ਸੂਚਕਾਂਕ);
    hb_blob_destroy (blob);
  } ਨਹੀਂਤਾਂ {
    face = hb_face_create_for_tables (hb_ft_reference_ਸਾਰਣੀ, ft_face, destroy);
    hb_face_ਸੈੱਟ_get_ਸਾਰਣੀ_ਟੈਗ_ਸੂਚੀ_func (face, hb_ft_get_ਸਾਰਣੀ_ਟੈਗ_ਸੂਚੀ, ft_face, ਖਾਲੀ_ਸੂਚਕ);
  }

  hb_face_ਸੈੱਟ_ਸੂਚਕਾਂਕ (face, ft_face->face_ਸੂਚਕਾਂਕ);
  hb_face_ਸੈੱਟ_upem (face, ft_face->units_ਪ੍ਰਤੀ_EM);

  ਵਾਪਸ face;
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
  ਵਾਪਸ hb_ft_face_create (ft_face, _hb_ft_face_destroy);
}

ਸਥਿਰ ਖਾਲੀ
hb_ft_face_finalize (ਖਾਲੀ *arg)
{
  FT_Face ft_face = (FT_Face) arg;
  hb_face_destroy ((hb_face_t *) ft_face->generic.ਅੰਕੜੇ);
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
  ਜੇ (unlikely (!ft_face->generic.ਅੰਕੜੇ || ft_face->generic.finalizer != hb_ft_face_finalize))
  {
    ਜੇ (ft_face->generic.finalizer)
      ft_face->generic.finalizer (ft_face);

    ft_face->generic.ਅੰਕੜੇ = hb_ft_face_create (ft_face, ਖਾਲੀ_ਸੂਚਕ);
    ft_face->generic.finalizer = hb_ft_face_finalize;
  }

  ਵਾਪਸ hb_face_reference ((hb_face_t *) ft_face->generic.ਅੰਕੜੇ);
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
  hb_ft_font_ਸੈੱਟ_funcs_2 (font, ft_face, ਝੂਠ);
  hb_ft_font_ਬਦਲਿਆ (font);
  ਵਾਪਸ font;
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
ਖਾਲੀ
hb_ft_font_ਬਦਲਿਆ (hb_font_t *font)
{
  ਜੇ (font->destroy != (hb_destroy_func_t) _hb_ft_font_destroy)
    ਵਾਪਸ;

  hb_ft_font_t *ft_font = (hb_ft_font_t *) font->user_ਅੰਕੜੇ;

  FT_Face ft_face = ft_font->ft_face;

  hb_font_ਸੈੱਟ_scale (font,
		     (ਪੂਰਨ) (((uint64_t) ft_face->ਮਾਪ->metrics.x_scale * (uint64_t) ft_face->units_ਪ੍ਰਤੀ_EM + (1u<<15)) >> 16),
		     (ਪੂਰਨ) (((uint64_t) ft_face->ਮਾਪ->metrics.y_scale * (uint64_t) ft_face->units_ਪ੍ਰਤੀ_EM + (1u<<15)) >> 16));
#ਜੇ 0 /* hb-ft works in no-hinting model */
  hb_font_ਸੈੱਟ_ppem (font,
		    ft_face->ਮਾਪ->metrics.x_ppem,
		    ft_face->ਮਾਪ->metrics.y_ppem);
#ਜੇ_ਅੰਤ

#ਜੇ defined(HAVE_FT_GET_VAR_BLEND_COORDINATES) && !defined(HB_NO_VAR)
  FT_MM_Var *mm_var = ਖਾਲੀ_ਸੂਚਕ;
  ਜੇ (!FT_Get_MM_Var (ft_face, &mm_var))
  {
    FT_Fixed *ft_coords = (FT_Fixed *) hb_calloc (mm_var->ਅੰਕ_axis, ਆਕਾਰ (FT_Fixed));
    ਪੂਰਨ *coords = (ਪੂਰਨ *) hb_calloc (mm_var->ਅੰਕ_axis, ਆਕਾਰ (ਪੂਰਨ));
    ਜੇ (coords && ft_coords)
    {
      ਜੇ (!FT_Get_Var_Blend_Coordinates (ft_face, mm_var->ਅੰਕ_axis, ft_coords))
      {
	ਤਾਰਕਿਕ nonzero = ਝੂਠ;

	ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < mm_var->ਅੰਕ_axis; ++i)
	 {
	  coords[i] = (ft_coords[i] + 2) >> 2;
	  nonzero = nonzero || coords[i];
	 }

	ਜੇ (nonzero)
	  hb_font_ਸੈੱਟ_var_coords_normalized (font, coords, mm_var->ਅੰਕ_axis);
	ਨਹੀਂਤਾਂ
	  hb_font_ਸੈੱਟ_var_coords_normalized (font, ਖਾਲੀ_ਸੂਚਕ, 0);
      }
    }
    hb_free (coords);
    hb_free (ft_coords);
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_FT_ਪੂਰਾ_MM_VAR
    FT_ਪੂਰਾ_MM_Var (ft_face->glyph->library, mm_var);
#ਨਹੀਂਤਾਂ
    hb_free (mm_var);
#ਜੇ_ਅੰਤ
  }
#ਜੇ_ਅੰਤ

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
hb_ft_hb_font_ਬਦਲਿਆ_2 (hb_font_t *font)
{
  ਜੇ (font->destroy != (hb_destroy_func_t) _hb_ft_font_destroy)
    ਵਾਪਸ ਝੂਠ;

  hb_ft_font_t *ft_font = (hb_ft_font_t *) font->user_ਅੰਕੜੇ;

  ਵਾਪਸ hb_ft_hb_font_check_ਬਦਲਿਆ (font, ft_font);
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
  ਵਾਪਸ hb_ft_font_create (ft_face, _hb_ft_face_destroy);
}


ਸਥਿਰ ਖਾਲੀ * _hb_ft_alloc (FT_ਸਮ੍ਰਿਤੀ ਸਮ੍ਰਿਤੀ, ਲੰਮਾ ਮਾਪ)
{ ਵਾਪਸ hb_malloc (ਮਾਪ); }

ਸਥਿਰ ਖਾਲੀ _hb_ft_free (FT_ਸਮ੍ਰਿਤੀ ਸਮ੍ਰਿਤੀ, ਖਾਲੀ *ਖੰਡ)
{ hb_free (ਖੰਡ); }

ਸਥਿਰ ਖਾਲੀ * _hb_ft_realloc (FT_ਸਮ੍ਰਿਤੀ ਸਮ੍ਰਿਤੀ, ਲੰਮਾ cur_ਮਾਪ, ਲੰਮਾ new_ਮਾਪ, ਖਾਲੀ *ਖੰਡ)
{ ਵਾਪਸ hb_realloc (ਖੰਡ, new_ਮਾਪ); }

ਸਥਿਰ FT_ਸਮ੍ਰਿਤੀ_Rec m =
{
  ਖਾਲੀ_ਸੂਚਕ,
  _hb_ft_alloc,
  _hb_ft_free,
  _hb_ft_realloc
};

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ free_static_ft_library ();
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ free_static_ft_library_mutex ();

ਸਥਿਰ ਬਣਤਰ hb_ft_library_mutex_lazy_loader_t : hb_lazy_loader_t<hb_mutex_t,
								   hb_ft_library_mutex_lazy_loader_t>
{
  ਸਥਿਰ hb_mutex_t *create ()
  {
    hb_mutex_t *lock = (hb_mutex_t *) hb_calloc (1, ਆਕਾਰ (hb_mutex_t));
    ਜੇ (unlikely (!lock))
      ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

    lock = ਨਵਾਂ (lock) hb_mutex_t;
    hb_atexit (free_static_ft_library_mutex);
    ਵਾਪਸ lock;
  }
  ਸਥਿਰ ਖਾਲੀ destroy (hb_mutex_t *lock)
  {
    lock->~hb_mutex_t ();
    hb_free (lock);
  }
  ਸਥਿਰ hb_mutex_t *get_null ()
  {
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
  }
} static_ft_library_mutex;

ਸਥਿਰ ਬਣਤਰ hb_ft_library_lazy_loader_t : hb_lazy_loader_t<hb_remove_ਸੂਚਕ<FT_Library>,
							     hb_ft_library_lazy_loader_t>
{
  ਸਥਿਰ FT_Library create ()
  {
    FT_Library l;
    ਜੇ (FT_New_Library (&m, &l))
      ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

    FT_ਜੋੜੋ_Default_Modules (l);
    FT_ਸੈੱਟ_Default_Properties (l);

    hb_atexit (free_static_ft_library);

    ਵਾਪਸ l;
  }
  ਸਥਿਰ ਖਾਲੀ destroy (FT_Library l)
  {
    FT_ਪੂਰਾ_Library (l);
  }
  ਸਥਿਰ FT_Library get_null ()
  {
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
  }
} static_ft_library;

ਸਥਿਰ ਸਤਰਅੰਦਰ
ਖਾਲੀ free_static_ft_library_mutex ()
{
  static_ft_library_mutex.free_instance ();
}

ਸਥਿਰ ਸਤਰਅੰਦਰ
ਖਾਲੀ free_static_ft_library ()
{
  hb_ਤਾਲਾ_t lock (static_ft_library_mutex.get_unconst ());
  static_ft_library.free_instance ();
}

ਸਥਿਰ FT_Library
reference_ft_library ()
{
  hb_mutex_t *mutex = static_ft_library_mutex.get_unconst ();
  ਜੇ (unlikely (!mutex))
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
  hb_ਤਾਲਾ_t lock (mutex);
  FT_Library l = static_ft_library.get_unconst ();
  ਜੇ (unlikely (FT_Reference_Library (l)))
  {
    DEBUG_ਸੁਨੇਹਾ (FT, l, "FT_Reference_Library() failed");
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
  }
  ਵਾਪਸ l;
}

ਸਥਿਰ hb_user_ਅੰਕੜੇ_ਕੁੰਜੀ_t ft_library_ਕੁੰਜੀ = {0};

ਸਥਿਰ ਖਾਲੀ
_hb_ft_face_destroy_static (ਖਾਲੀ *ਅੰਕੜੇ)
{
  hb_ਤਾਲਾ_t lock (static_ft_library_mutex.get_unconst ());
  FT_ਪੂਰਾ_Face ((FT_Face) ਅੰਕੜੇ);
}

ਸਥਿਰ FT_Error
new_ft_face (FT_Library  ft_library,
	     ਅਚਲ ਅੱਖਰ *file_name,
	     ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸੂਚਕਾਂਕ_2,
	     FT_Face     *ft_face)
{
  hb_ਤਾਲਾ_t lock (static_ft_library_mutex.get_unconst ());
  ਵਾਪਸ FT_New_Face (ft_library, file_name, ਸੂਚਕਾਂਕ_2, ft_face);
}

ਸਥਿਰ FT_Error
new_ft_ਸਮ੍ਰਿਤੀ_face (FT_Library    ft_library,
		    ਅਚਲ FT_Byte *blob_ਅੰਕੜੇ,
		    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ   blob_ਮਾਪ,
		    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ   ਸੂਚਕਾਂਕ_2,
		    FT_Face       *ft_face)
{
  hb_ਤਾਲਾ_t lock (static_ft_library_mutex.get_unconst ());
  ਵਾਪਸ FT_New_ਸਮ੍ਰਿਤੀ_Face (ft_library, blob_ਅੰਕੜੇ, blob_ਮਾਪ, ਸੂਚਕਾਂਕ_2, ft_face);
}

ਸਥਿਰ ਖਾਲੀ
finalize_ft_library (ਖਾਲੀ *arg)
{
  FT_Face ft_face = (FT_Face) arg;
  FT_ਪੂਰਾ_Library ((FT_Library) ft_face->generic.ਅੰਕੜੇ);
}

ਸਥਿਰ ਖਾਲੀ
destroy_ft_library (ਖਾਲੀ *arg)
{
  hb_ਤਾਲਾ_t lock (static_ft_library_mutex.get_unconst ());
  FT_ਪੂਰਾ_Library ((FT_Library) arg);
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
hb_ft_face_create_from_file_or_fail (ਅਚਲ ਅੱਖਰ   *file_name,
				     ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ  ਸੂਚਕਾਂਕ_2)
{
  FT_Library ft_library = reference_ft_library ();
  ਜੇ (unlikely (!ft_library))
  {
    DEBUG_ਸੁਨੇਹਾ (FT, ft_library, "reference_ft_library failed");
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
  }

  FT_Face ft_face;
  ਜੇ (unlikely (new_ft_face (ft_library,
			     file_name,
			     ਸੂਚਕਾਂਕ_2,
			     &ft_face)))
  {
    destroy_ft_library (ft_library);
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
  }

  FT_Reference_Face (ft_face);
  hb_face_t *face = hb_ft_face_create (ft_face, _hb_ft_face_destroy_static);
  _hb_ft_face_destroy_static (ft_face);

  ft_face->generic.ਅੰਕੜੇ = ft_library;
  ft_face->generic.finalizer = finalize_ft_library;

  ਜੇ (hb_face_is_immutable (face))
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

  ਵਾਪਸ face;
}

ਸਥਿਰ hb_user_ਅੰਕੜੇ_ਕੁੰਜੀ_t ft_blob_ਕੁੰਜੀ = {0};

ਸਥਿਰ ਖਾਲੀ
_destroy_blob (ਖਾਲੀ *p)
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
				     ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ  ਸੂਚਕਾਂਕ_2)
{
  FT_Library ft_library = reference_ft_library ();
  ਜੇ (unlikely (!ft_library))
  {
    DEBUG_ਸੁਨੇਹਾ (FT, ft_library, "reference_ft_library failed");
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
  }

  hb_blob_make_immutable (blob);
  ਬਿਨਾਨਿਸ਼ਾਨੀ blob_ਮਾਪ;
  ਅਚਲ ਅੱਖਰ *blob_ਅੰਕੜੇ = hb_blob_get_ਅੰਕੜੇ (blob, &blob_ਮਾਪ);

  FT_Face ft_face;
  ਜੇ (unlikely (new_ft_ਸਮ੍ਰਿਤੀ_face (ft_library,
				    (ਅਚਲ FT_Byte *) blob_ਅੰਕੜੇ,
				    blob_ਮਾਪ,
				    ਸੂਚਕਾਂਕ_2,
				    &ft_face)))
  {
    destroy_ft_library (ft_library);
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
  }

  FT_Reference_Face (ft_face);
  hb_face_t *face = hb_ft_face_create (ft_face, _hb_ft_face_destroy_static);
  _hb_ft_face_destroy_static (ft_face);

  ft_face->generic.ਅੰਕੜੇ = ft_library;
  ft_face->generic.finalizer = finalize_ft_library;

  ਜੇ (hb_face_is_immutable (face))
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

  // Hook the blob to the hb_face_t, since FT_Face still needs it.
  hb_blob_reference (blob);
  ਜੇ (!hb_face_ਸੈੱਟ_user_ਅੰਕੜੇ (face, &ft_blob_ਕੁੰਜੀ, blob, _destroy_blob, ਸੱਚ))
  {
    hb_blob_destroy (blob);
    hb_face_destroy (face);
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
  }

  ਵਾਪਸ face;
}

ਸਥਿਰ ਖਾਲੀ
_release_blob (ਖਾਲੀ *arg)
{
  FT_Face ft_face = (FT_Face) arg;
  hb_blob_destroy ((hb_blob_t *) ft_face->generic.ਅੰਕੜੇ);
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
ਖਾਲੀ
hb_ft_font_ਸੈੱਟ_funcs (hb_font_t *font)
{
  ਪੂਰਨ ਲੱਦੋ_flags = FT_ਲੱਦੋ_DEFAULT | FT_ਲੱਦੋ_NO_HINTING;
  ਜੇ (font->destroy == (hb_destroy_func_t) _hb_ft_font_destroy && font->user_ਅੰਕੜੇ)
  {
    ਅਚਲ hb_ft_font_t *existing_ft_font = (ਅਚਲ hb_ft_font_t *) font->user_ਅੰਕੜੇ;
    ਲੱਦੋ_flags = existing_ft_font->ਲੱਦੋ_flags;
  }

  // In case of failure...
  hb_font_ਸੈੱਟ_funcs (font,
		     hb_font_funcs_get_ਖਾਲੀ_ਥਾਵਾਂ (),
		     ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);

  hb_blob_t *blob = hb_face_reference_blob (font->face);
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ blob_ਲੰਮਾਈ;
  ਅਚਲ ਅੱਖਰ *blob_ਅੰਕੜੇ = hb_blob_get_ਅੰਕੜੇ (blob, &blob_ਲੰਮਾਈ);
  ਜੇ (unlikely (!blob_ਲੰਮਾਈ))
    DEBUG_ਸੁਨੇਹਾ (FT, font, "Font face has empty blob");

  FT_Library ft_library = reference_ft_library ();
  ਜੇ (unlikely (!ft_library))
  {
    hb_blob_destroy (blob);
    DEBUG_ਸੁਨੇਹਾ (FT, font, "reference_ft_library failed");
    ਵਾਪਸ;
  }

  FT_Face ft_face = ਖਾਲੀ_ਸੂਚਕ;
  ਜੇ (unlikely (new_ft_ਸਮ੍ਰਿਤੀ_face (ft_library,
				    (ਅਚਲ FT_Byte *) blob_ਅੰਕੜੇ,
				    blob_ਲੰਮਾਈ,
				    hb_face_get_ਸੂਚਕਾਂਕ (font->face),
				    &ft_face)))
  {
    hb_blob_destroy (blob);
    destroy_ft_library (ft_library);
    DEBUG_ਸੁਨੇਹਾ (FT, font, "FT_New_Memory_Face() failed");
    ਵਾਪਸ;
  }

  ਜੇ (FT_Select_Charmap (ft_face, FT_ENCODING_MS_ਚਿੰਨ੍ਹ))
    FT_Select_Charmap (ft_face, FT_ENCODING_UNICODE);

  // Hook the blob to the FT_Face
  ft_face->generic.ਅੰਕੜੇ = blob;
  ft_face->generic.finalizer = _release_blob;

  // And the FT_Library to the blob
  ਜੇ (unlikely (!hb_blob_ਸੈੱਟ_user_ਅੰਕੜੇ (blob, &ft_library_ਕੁੰਜੀ, ft_library, destroy_ft_library, ਸੱਚ)))
  {
    DEBUG_ਸੁਨੇਹਾ (FT, font, "hb_blob_set_user_data() failed");
    _hb_ft_face_destroy_static (ft_face);
    destroy_ft_library (ft_library);
    ਵਾਪਸ;
  }

  hb_ft_font_ਸੈੱਟ_funcs_2 (font, ft_face, ਸੱਚ, ਸੱਚ);
  hb_ft_font_ਸੈੱਟ_ਲੱਦੋ_flags (font, ਲੱਦੋ_flags);

  hb_ft_hb_font_ਬਦਲਿਆ (font, ft_face);
}

#ਜੇ_ਅੰਤ
