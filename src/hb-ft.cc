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

#அந்தர்பாவ "hb.hh"

#ifdef HAVE_FREETYPE

#அந்தர்பாவ "hb-ft.h"

#அந்தர்பாவ "hb-cache.hh"
#அந்தர்பாவ "hb-draw.hh"
#அந்தர்பாவ "hb-font.hh"
#அந்தர்பாவ "hb-machinery.hh"
#அந்தர்பாவ "hb-ot-os2-table.hh"
#அந்தர்பாவ "hb-ot-shaper-arabic-pua.hh"
#அந்தர்பாவ "hb-paint.hh"

#அந்தர்பாவ FT_MODULE_H
#அந்தர்பாவ FT_ADVANCES_H
#அந்தர்பாவ FT_MULTIPLE_MASTERS_H
#அந்தர்பாவ FT_OUTLINE_H
#அந்தர்பாவ FT_TRUETYPE_TABLES_H
#அந்தர்பாவ FT_SYNTHESIS_H
#எனில் (FREETYPE_MAJOR*10000 + FREETYPE_MINOR*100 + FREETYPE_PATCH) >= 21300
#அந்தர்பாவ FT_COLOR_H
#endif


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


using hb_ft_advance_cache_t = hb_cache_t<16, 24, 8, பொய்>;

நிலையான வெறுமை _hb_ft_face_destroy_static (வெறுமை *data);

கட்டமைப்பு hb_ft_font_t
{
  முழு load_flags;
  தருக்க குறியீடு; /* Whether selected cmap is symbol cmap. */
  தருக்க unref; /* Whether to destroy ft_face when done. */
  தருக்க transform; /* Whether to apply FT_Face's transform. */
  தருக்க static_library; /* Whether ft_face uses static_ft_library. */

  mutable hb_mutex_t lock; /* Protects members below. */
  FT_Face ft_face;
  mutable hb_atomic_t<unsigned> cached_serial;
  mutable hb_ft_advance_cache_t advance_cache;
};

நிலையான hb_ft_font_t *
_hb_ft_font_create (FT_Face ft_face, தருக்க குறியீடு, தருக்க unref, தருக்க static_library)
{
  hb_ft_font_t *ft_font = (hb_ft_font_t *) hb_calloc (1, அளவு (hb_ft_font_t));
  எனில் (unlikely (!ft_font)) திரும்பு nullptr;

  ft_font->lock.init ();
  ft_font->ft_face = ft_face;
  ft_font->குறியீடு = குறியீடு;
  ft_font->unref = unref;
  ft_font->static_library = static_library;

  ft_font->load_flags = FT_LOAD_DEFAULT | FT_LOAD_NO_HINTING;

  ft_font->cached_serial = UINT_MAX;
  புதிய (&ft_font->advance_cache) hb_ft_advance_cache_t;

  திரும்பு ft_font;
}

நிலையான வெறுமை
_hb_ft_face_destroy (வெறுமை *data)
{
  FT_Done_Face ((FT_Face) data);
}

நிலையான வெறுமை
_hb_ft_font_destroy (வெறுமை *data)
{
  hb_ft_font_t *ft_font = (hb_ft_font_t *) data;

  எனில் (ft_font->unref)
  {
    எனில் (ft_font->static_library)
    {
      /* ft_font->ft_face->generic.data is the blob that hb_ft_font_set_funcs()
       * hooked up as this internal FT_Face's finalizer target (_release_blob).
       * FT_Done_Face() below runs that finalizer while static_ft_library_mutex
       * is held; if dropping its reference were to free the blob right there,
       * the blob's attached destroy_ft_library() user-data callback would try
       * to re-lock the same non-recursive mutex and self-deadlock. Hold an
       * extra reference across the locked call so the blob (and thus that
       * callback) can only be freed after the lock is released. */
      hb_blob_t *blob = hb_blob_reference ((hb_blob_t *) ft_font->ft_face->generic.data);
      _hb_ft_face_destroy_static (ft_font->ft_face);
      hb_blob_destroy (blob);
    }
    இல்லையெனில்
      _hb_ft_face_destroy (ft_font->ft_face);
  }

  ft_font->lock.fini ();

  hb_free (ft_font);
}


/* hb_font changed, update FT_Face. */
நிலையான வெறுமை _hb_ft_hb_font_changed (hb_font_t *font, FT_Face ft_face)
{
  எனில் (unlikely (font->destroy != (hb_destroy_func_t) _hb_ft_font_destroy))
    திரும்பு;

  hb_ft_font_t *ft_font = (hb_ft_font_t *) font->user_data;

  தசம x_mult = 1.f, y_mult = 1.f;

  எனில் (font->x_scale < 0) x_mult = -x_mult;
  எனில் (font->y_scale < 0) y_mult = -y_mult;

  எனில் (FT_Set_Char_Size (ft_face,
			abs (font->x_scale), abs (font->y_scale),
			0, 0
#எனில் 0
		    font->x_ppem * 72 * 64 / font->x_scale,
		    font->y_ppem * 72 * 64 / font->y_scale
#endif
     ) && ft_face->இலக்கம்_fixed_sizes)
  {
#ifdef HAVE_FT_GET_TRANSFORM
    /* Bitmap font, eg. bitmap color emoji. */
    /* Pick largest size? */
    முழு x_scale  = ft_face->available_sizes[ft_face->இலக்கம்_fixed_sizes - 1].x_ppem;
    முழு y_scale = ft_face->available_sizes[ft_face->இலக்கம்_fixed_sizes - 1].y_ppem;
    FT_Set_Char_Size (ft_face,
		      x_scale, y_scale,
		      0, 0);

    /* This contains the sign that was previously in x_mult/y_mult. */
    x_mult = (தசம) font->x_scale / x_scale;
    y_mult = (தசம) font->y_scale / y_scale;
#endif
  }
  இல்லையெனில்
  { /* Shrug */ }


  எனில் (x_mult != 1.f || y_mult != 1.f)
  {
    FT_Matrix matrix = { (முழு) roundf (x_mult * (1<<16)), 0,
			  0, (முழு) roundf (y_mult * (1<<16))};
    FT_Set_Transform (ft_face, &matrix, nullptr);
    ft_font->transform = உண்மை;
  }
  இல்லையெனில்
    FT_Set_Transform (ft_face, nullptr, nullptr);

#எனில் defined(HAVE_FT_GET_VAR_BLEND_COORDINATES) && !defined(HB_NO_VAR)
  எனில் (font->has_nonzero_coords)
  {
    unsigned முழு இலக்கம்_coords;
    மாறிலி தசம *coords = hb_font_get_var_coords_design (font, &இலக்கம்_coords);
    FT_Fixed *ft_coords = (FT_Fixed *) hb_calloc (இலக்கம்_coords, அளவு (FT_Fixed));
    எனில் (ft_coords)
    {
      க்கு (unsigned முழு i = 0; i < இலக்கம்_coords; i++)
	  ft_coords[i] = coords[i] * 65536.f;
      FT_Set_Var_Design_Coordinates (ft_face, இலக்கம்_coords, ft_coords);
      hb_free (ft_coords);
    }
  }
  இல்லையெனில் எனில் (font->இலக்கம்_coords)
  {
    // Some old versions of FreeType crash if we
    // call this function on non-variable fonts.
    FT_Set_Var_Design_Coordinates (ft_face, 0, nullptr);
  }
#endif
}

/* Check if hb_font changed, update FT_Face. */
நிலையான உள்வரி தருக்க
_hb_ft_hb_font_check_changed (hb_font_t *font,
			      மாறிலி hb_ft_font_t *ft_font)
{
  எனில் (font->serial != ft_font->cached_serial)
  {
    hb_lock_t lock (ft_font->lock);
    _hb_ft_hb_font_changed (font, ft_font->ft_face);
    ft_font->advance_cache.clear ();
    ft_font->cached_serial.set_release (font->serial.get_acquire ());
    திரும்பு உண்மை;
  }
  திரும்பு பொய்;
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
வெறுமை
hb_ft_font_set_load_flags (hb_font_t *font, முழு load_flags)
{
  எனில் (hb_object_is_immutable (font))
    திரும்பு;

  எனில் (unlikely (font->destroy != (hb_destroy_func_t) _hb_ft_font_destroy))
    திரும்பு;

  hb_ft_font_t *ft_font = (hb_ft_font_t *) font->user_data;

  ft_font->load_flags = load_flags;
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
முழு
hb_ft_font_get_load_flags (hb_font_t *font)
{
  எனில் (unlikely (font->destroy != (hb_destroy_func_t) _hb_ft_font_destroy))
    திரும்பு 0;

  மாறிலி hb_ft_font_t *ft_font = (மாறிலி hb_ft_font_t *) font->user_data;

  திரும்பு ft_font->load_flags;
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
  எனில் (unlikely (font->destroy != (hb_destroy_func_t) _hb_ft_font_destroy))
    திரும்பு nullptr;

  மாறிலி hb_ft_font_t *ft_font = (மாறிலி hb_ft_font_t *) font->user_data;

  திரும்பு ft_font->ft_face;
}

#ifndef HB_DISABLE_DEPRECATED

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
  திரும்பு hb_ft_font_get_ft_face (font);
}

#endif

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
hb_ft_font_lock_face (hb_font_t *font)
{
  எனில் (unlikely (font->destroy != (hb_destroy_func_t) _hb_ft_font_destroy))
    திரும்பு nullptr;

  மாறிலி hb_ft_font_t *ft_font = (மாறிலி hb_ft_font_t *) font->user_data;

  ft_font->lock.lock ();

  திரும்பு ft_font->ft_face;
}

/**
 * hb_ft_font_unlock_face: (skip)
 * @font: #hb_font_t to work upon
 *
 * Releases an FT_Face previously obtained with hb_ft_font_lock_face().
 *
 * Since: 2.6.5
 **/
வெறுமை
hb_ft_font_unlock_face (hb_font_t *font)
{
  எனில் (unlikely (font->destroy != (hb_destroy_func_t) _hb_ft_font_destroy))
    திரும்பு;

  மாறிலி hb_ft_font_t *ft_font = (மாறிலி hb_ft_font_t *) font->user_data;

  ft_font->lock.unlock ();
}


நிலையான hb_bool_t
hb_ft_get_nominal_glyph (hb_font_t *font,
			 வெறுமை *font_data,
			 hb_codepoint_t unicode,
			 hb_codepoint_t *glyph,
			 வெறுமை *user_data HB_UNUSED)
{
  மாறிலி hb_ft_font_t *ft_font = (மாறிலி hb_ft_font_t *) font_data;
  hb_lock_t lock (ft_font->lock);
  unsigned முழு g = FT_Get_Char_Index (ft_font->ft_face, unicode);

  எனில் (unlikely (!g))
  {
    எனில் (unlikely (ft_font->குறியீடு))
    {
      தேர்வு ((unsigned) font->face->table.OS2->get_font_page ()) {
      நிலை OT::OS2::font_page_t::FONT_PAGE_NONE:
	எனில் (unicode <= 0x00FFu)
	  /* For symbol-encoded OpenType fonts, we duplicate the
	   * U+F000..F0FF range at U+0000..U+00FF.  That's what
	   * Windows seems to do, and that's hinted about at:
	   * https://docs.microsoft.com/en-us/typography/opentype/spec/recom
	   * under "Non-Standard (Symbol) Fonts". */
	  g = FT_Get_Char_Index (ft_font->ft_face, 0xF000u + unicode);
	நிறுத்து;
#ifndef HB_NO_OT_SHAPER_ARABIC_FALLBACK
      நிலை OT::OS2::font_page_t::FONT_PAGE_SIMP_ARABIC:
	g = FT_Get_Char_Index (ft_font->ft_face, _hb_arabic_pua_simp_map (unicode));
	நிறுத்து;
      நிலை OT::OS2::font_page_t::FONT_PAGE_TRAD_ARABIC:
	g = FT_Get_Char_Index (ft_font->ft_face, _hb_arabic_pua_trad_map (unicode));
	நிறுத்து;
#endif
      இயல்பு:
	நிறுத்து;
      }
      எனில் (!g)
	திரும்பு பொய்;
    }
    இல்லையெனில்
      திரும்பு பொய்;
  }

  *glyph = g;
  திரும்பு உண்மை;
}

நிலையான unsigned முழு
hb_ft_get_nominal_glyphs (hb_font_t *font HB_UNUSED,
			  வெறுமை *font_data,
			  unsigned முழு மொத்தம்,
			  மாறிலி hb_codepoint_t *first_unicode,
			  unsigned முழு unicode_stride,
			  hb_codepoint_t *first_glyph,
			  unsigned முழு glyph_stride,
			  வெறுமை *user_data HB_UNUSED)
{
  மாறிலி hb_ft_font_t *ft_font = (மாறிலி hb_ft_font_t *) font_data;
  hb_lock_t lock (ft_font->lock);
  unsigned முழு done;
  க்கு (done = 0;
       done < மொத்தம் && (*first_glyph = FT_Get_Char_Index (ft_font->ft_face, *first_unicode));
       done++)
  {
    first_unicode = &StructAtOffsetUnaligned<hb_codepoint_t> (first_unicode, unicode_stride);
    first_glyph = &StructAtOffsetUnaligned<hb_codepoint_t> (first_glyph, glyph_stride);
  }
  /* We don't need to do ft_font->symbol dance here, since HB calls the singular
   * nominal_glyph() for what we don't handle here. */
  திரும்பு done;
}


நிலையான hb_bool_t
hb_ft_get_variation_glyph (hb_font_t *font HB_UNUSED,
			   வெறுமை *font_data,
			   hb_codepoint_t unicode,
			   hb_codepoint_t variation_selector,
			   hb_codepoint_t *glyph,
			   வெறுமை *user_data HB_UNUSED)
{
  மாறிலி hb_ft_font_t *ft_font = (மாறிலி hb_ft_font_t *) font_data;
  hb_lock_t lock (ft_font->lock);
  unsigned முழு g = FT_Face_GetCharVariantIndex (ft_font->ft_face, unicode, variation_selector);

  எனில் (unlikely (!g))
    திரும்பு பொய்;

  *glyph = g;
  திரும்பு உண்மை;
}

நிலையான வெறுமை
hb_ft_get_glyph_h_advances (hb_font_t* font, வெறுமை* font_data,
			    unsigned மொத்தம்,
			    மாறிலி hb_codepoint_t *first_glyph,
			    unsigned glyph_stride,
			    hb_position_t *first_advance,
			    unsigned advance_stride,
			    வெறுமை *user_data HB_UNUSED)
{
  மாறிலி hb_ft_font_t *ft_font = (மாறிலி hb_ft_font_t *) font_data;
  _hb_ft_hb_font_check_changed (font, ft_font);

  hb_lock_t lock (ft_font->lock);
  FT_Face ft_face = ft_font->ft_face;
  முழு load_flags = ft_font->load_flags;
  தசம x_mult;
#ifdef HAVE_FT_GET_TRANSFORM
  எனில் (ft_font->transform)
  {
    FT_Matrix matrix;
    FT_Get_Transform (ft_face, &matrix, nullptr);
    x_mult = sqrtf ((தசம)matrix.xx * matrix.xx + (தசம)matrix.xy * matrix.xy) / 65536.f;
    x_mult *= font->x_scale < 0 ? -1 : +1;
  }
  இல்லையெனில்
#endif
  {
    x_mult = font->x_scale < 0 ? -1 : +1;
  }

  க்கு (unsigned முழு i = 0; i < மொத்தம்; i++)
  {
    hb_position_t advance;
    hb_codepoint_t glyph = *first_glyph;

    unsigned முழு cv;
    எனில் (ft_font->advance_cache.get (glyph, &cv))
      advance = (hb_position_t) cv;
    இல்லையெனில்
    {
      FT_Fixed v = 0;
      FT_Get_Advance (ft_face, glyph, load_flags, &v);
      /* Work around bug that FreeType seems to return negative advance
       * for variable-set fonts if x_scale is negative! */
      இரட்டை scaled = trunc (fabs ((இரட்டை) v) * (இரட்டை) x_mult + (1<<9)) / (1<<10);
      advance = hb_clamp_to<hb_position_t> (floor (scaled));
      ft_font->advance_cache.set (glyph, advance);
    }

    *first_advance = advance;
    first_glyph = &StructAtOffsetUnaligned<hb_codepoint_t> (first_glyph, glyph_stride);
    first_advance = &StructAtOffsetUnaligned<hb_position_t> (first_advance, advance_stride);
  }
}

#ifndef HB_NO_VERTICAL
நிலையான hb_position_t
hb_ft_get_glyph_v_advance (hb_font_t *font,
			   வெறுமை *font_data,
			   hb_codepoint_t glyph,
			   வெறுமை *user_data HB_UNUSED)
{
  மாறிலி hb_ft_font_t *ft_font = (மாறிலி hb_ft_font_t *) font_data;
  _hb_ft_hb_font_check_changed (font, ft_font);

  hb_lock_t lock (ft_font->lock);
  FT_Fixed v;
  தசம y_mult;
#ifdef HAVE_FT_GET_TRANSFORM
  எனில் (ft_font->transform)
  {
    FT_Matrix matrix;
    FT_Get_Transform (ft_font->ft_face, &matrix, nullptr);
    y_mult = sqrtf ((தசம)matrix.yx * matrix.yx + (தசம)matrix.yy * matrix.yy) / 65536.f;
    y_mult *= font->y_scale < 0 ? -1 : +1;
  }
  இல்லையெனில்
#endif
  {
    y_mult = font->y_scale < 0 ? -1 : +1;
  }

  எனில் (unlikely (FT_Get_Advance (ft_font->ft_face, glyph, ft_font->load_flags | FT_LOAD_VERTICAL_LAYOUT, &v)))
    திரும்பு 0;

  /* Note: FreeType's vertical metrics grows downward while other FreeType coordinates
   * have a Y growing upward.  Hence the extra negation. */
  இரட்டை advance = floor ((-(இரட்டை) v + (1<<9)) / (1<<10));

  திரும்பு hb_clamp_to<hb_position_t> ((இரட்டை) y_mult * advance);
}
#endif

#ifndef HB_NO_VERTICAL
நிலையான hb_bool_t
hb_ft_get_glyph_v_origin (hb_font_t *font,
			  வெறுமை *font_data,
			  hb_codepoint_t glyph,
			  hb_position_t *x,
			  hb_position_t *y,
			  வெறுமை *user_data HB_UNUSED)
{
  மாறிலி hb_ft_font_t *ft_font = (மாறிலி hb_ft_font_t *) font_data;
  _hb_ft_hb_font_check_changed (font, ft_font);

  hb_lock_t lock (ft_font->lock);
  FT_Face ft_face = ft_font->ft_face;
  தசம x_mult, y_mult;
#ifdef HAVE_FT_GET_TRANSFORM
  எனில் (ft_font->transform)
  {
    FT_Matrix matrix;
    FT_Get_Transform (ft_face, &matrix, nullptr);
    x_mult = sqrtf ((தசம)matrix.xx * matrix.xx + (தசம)matrix.xy * matrix.xy) / 65536.f;
    x_mult *= font->x_scale < 0 ? -1 : +1;
    y_mult = sqrtf ((தசம)matrix.yx * matrix.yx + (தசம)matrix.yy * matrix.yy) / 65536.f;
    y_mult *= font->y_scale < 0 ? -1 : +1;
  }
  இல்லையெனில்
#endif
  {
    x_mult = font->x_scale < 0 ? -1 : +1;
    y_mult = font->y_scale < 0 ? -1 : +1;
  }

  எனில் (unlikely (FT_Load_Glyph (ft_face, glyph, ft_font->load_flags)))
    திரும்பு பொய்;

  /* Note: FreeType's vertical metrics grows downward while other FreeType coordinates
   * have a Y growing upward.  Hence the extra negation. */
  *x = hb_clamp_to<hb_position_t> ((இரட்டை) x_mult *
				   ((இரட்டை) ft_face->glyph->metrics.horiBearingX -
				    ft_face->glyph->metrics.vertBearingX));
  *y = hb_clamp_to<hb_position_t> ((இரட்டை) y_mult *
				   ((இரட்டை) ft_face->glyph->metrics.horiBearingY +
				    ft_face->glyph->metrics.vertBearingY));

  திரும்பு உண்மை;
}
#endif

#ifndef HB_NO_OT_SHAPE_FALLBACK
நிலையான hb_position_t
hb_ft_get_glyph_h_kerning (hb_font_t *font,
			   வெறுமை *font_data,
			   hb_codepoint_t இடது_glyph,
			   hb_codepoint_t வலது_glyph,
			   வெறுமை *user_data HB_UNUSED)
{
  மாறிலி hb_ft_font_t *ft_font = (மாறிலி hb_ft_font_t *) font_data;
  _hb_ft_hb_font_check_changed (font, ft_font);

  hb_lock_t lock (ft_font->lock);
  FT_Vector kerningv;

  FT_Kerning_Mode mode = font->x_ppem ? FT_KERNING_DEFAULT : FT_KERNING_UNFITTED;
  எனில் (FT_Get_Kerning (ft_font->ft_face, இடது_glyph, வலது_glyph, mode, &kerningv))
    திரும்பு 0;

  திரும்பு hb_clamp_to<hb_position_t> ((int64_t) kerningv.x);
}
#endif

நிலையான தருக்க
hb_ft_is_colr_glyph (hb_font_t *font,
		     வெறுமை *font_data,
		     hb_codepoint_t gid)
{
#ifndef HB_NO_PAINT
#எனில் (FREETYPE_MAJOR*10000 + FREETYPE_MINOR*100 + FREETYPE_PATCH) >= 21300
  மாறிலி hb_ft_font_t *ft_font = (மாறிலி hb_ft_font_t *) font_data;
  FT_Face ft_face = ft_font->ft_face;


  /* COLRv1 */
  FT_OpaquePaint paint = {0};
  எனில் (FT_Get_Color_Glyph_Paint (ft_face, gid,
			        FT_COLOR_NO_வேர்_TRANSFORM,
			        &paint))
    திரும்பு உண்மை;

  /* COLRv0 */
  FT_LayerIterator  iterator;
  FT_UInt  layer_glyph_index;
  FT_UInt  layer_color_index;
  iterator.p  = வெற்று;
  எனில் (FT_Get_Color_Glyph_Layer (ft_face,
				gid,
				&layer_glyph_index,
				&layer_color_index,
				&iterator))
    திரும்பு உண்மை;
#endif
#endif

  திரும்பு பொய்;
}

நிலையான hb_bool_t
hb_ft_get_glyph_extents (hb_font_t *font,
			 வெறுமை *font_data,
			 hb_codepoint_t glyph,
			 hb_glyph_extents_t *extents,
			 வெறுமை *user_data HB_UNUSED)
{
  // FreeType doesn't return COLR glyph extents.
  எனில் (hb_ft_is_colr_glyph (font, font_data, glyph))
    திரும்பு பொய்;

  மாறிலி hb_ft_font_t *ft_font = (மாறிலி hb_ft_font_t *) font_data;
  _hb_ft_hb_font_check_changed (font, ft_font);

  hb_lock_t lock (ft_font->lock);
  FT_Face ft_face = ft_font->ft_face;
  தசம x_mult, y_mult;

#ifdef HAVE_FT_GET_TRANSFORM
  எனில் (ft_font->transform)
  {
    FT_Matrix matrix;
    FT_Get_Transform (ft_face, &matrix, nullptr);
    x_mult = sqrtf ((தசம)matrix.xx * matrix.xx + (தசம)matrix.xy * matrix.xy) / 65536.f;
    x_mult *= font->x_scale < 0 ? -1 : +1;
    y_mult = sqrtf ((தசம)matrix.yx * matrix.yx + (தசம)matrix.yy * matrix.yy) / 65536.f;
    y_mult *= font->y_scale < 0 ? -1 : +1;
  }
  இல்லையெனில்
#endif
  {
    x_mult = font->x_scale < 0 ? -1 : +1;
    y_mult = font->y_scale < 0 ? -1 : +1;
  }

  எனில் (unlikely (FT_Load_Glyph (ft_face, glyph, ft_font->load_flags)))
    திரும்பு பொய்;

  /* Copied from hb_font_t::scale_glyph_extents. */

  தசம x1 = x_mult * ft_face->glyph->metrics.horiBearingX;
  தசம y1 = y_mult * ft_face->glyph->metrics.horiBearingY;
  தசம x2 = x1 + x_mult *  ft_face->glyph->metrics.width;
  தசம y2 = y1 + y_mult * -ft_face->glyph->metrics.height;

  இரட்டை rx1 = (இரட்டை) roundf (x1);
  இரட்டை ry1 = (இரட்டை) roundf (y1);
  இரட்டை rx2 = (இரட்டை) roundf (x2);
  இரட்டை ry2 = (இரட்டை) roundf (y2);

  extents->x_bearing = hb_clamp_to<hb_position_t> (rx1);
  extents->y_bearing = hb_clamp_to<hb_position_t> (ry1);
  extents->width = hb_clamp_to<hb_position_t> (rx2 - rx1);
  extents->height = hb_clamp_to<hb_position_t> (ry2 - ry1);

  திரும்பு உண்மை;
}

நிலையான hb_bool_t
hb_ft_get_glyph_contour_point (hb_font_t *font HB_UNUSED,
			       வெறுமை *font_data,
			       hb_codepoint_t glyph,
			       unsigned முழு point_index,
			       hb_position_t *x,
			       hb_position_t *y,
			       வெறுமை *user_data HB_UNUSED)
{
  மாறிலி hb_ft_font_t *ft_font = (மாறிலி hb_ft_font_t *) font_data;
  _hb_ft_hb_font_check_changed (font, ft_font);

  hb_lock_t lock (ft_font->lock);
  FT_Face ft_face = ft_font->ft_face;

  எனில் (unlikely (FT_Load_Glyph (ft_face, glyph, ft_font->load_flags)))
      திரும்பு பொய்;

  எனில் (unlikely (ft_face->glyph->format != FT_GLYPH_FORMAT_OUTLINE))
      திரும்பு பொய்;

  எனில் (unlikely (point_index >= (unsigned முழு) ft_face->glyph->outline.n_points))
      திரும்பு பொய்;

  *x = ft_face->glyph->outline.points[point_index].x;
  *y = ft_face->glyph->outline.points[point_index].y;

  திரும்பு உண்மை;
}

நிலையான hb_bool_t
hb_ft_get_glyph_name (hb_font_t *font HB_UNUSED,
		      வெறுமை *font_data,
		      hb_codepoint_t glyph,
		      எழுத்து *name, unsigned முழு size,
		      வெறுமை *user_data HB_UNUSED)
{
  மாறிலி hb_ft_font_t *ft_font = (மாறிலி hb_ft_font_t *) font_data;
  hb_lock_t lock (ft_font->lock);
  FT_Face ft_face = ft_font->ft_face;

  எனில் (!size)
  {
    எழுத்து buf[128];
    திரும்பு !FT_Get_Glyph_Name (ft_face, glyph, buf, அளவு (buf)) && *buf;
  }

  hb_bool_t ret = !FT_Get_Glyph_Name (ft_face, glyph, name, size);
  எனில் (ret && (size && !*name))
    ret = பொய்;

  திரும்பு ret;
}

நிலையான hb_bool_t
hb_ft_get_glyph_from_name (hb_font_t *font HB_UNUSED,
			   வெறுமை *font_data,
			   மாறிலி எழுத்து *name, முழு len, /* -1 means nul-terminated */
			   hb_codepoint_t *glyph,
			   வெறுமை *user_data HB_UNUSED)
{
  மாறிலி hb_ft_font_t *ft_font = (மாறிலி hb_ft_font_t *) font_data;
  hb_lock_t lock (ft_font->lock);
  FT_Face ft_face = ft_font->ft_face;

  எனில் (len < 0)
    *glyph = FT_Get_Name_Index (ft_face, (FT_String *) name);
  இல்லையெனில் {
    /* Make a nul-terminated version. */
    எழுத்து buf[128];
    len = hb_min (len, (முழு) அளவு (buf) - 1);
    strncpy (buf, name, len);
    buf[len] = '\0';
    *glyph = FT_Get_Name_Index (ft_face, buf);
  }

  எனில் (*glyph == 0)
  {
    /* Check whether the given name was actually the name of glyph 0. */
    எழுத்து buf[128];
    எனில் (!FT_Get_Glyph_Name(ft_face, 0, buf, அளவு (buf)) &&
	len < 0 ? !strcmp (buf, name) : !strncmp (buf, name, len))
      திரும்பு உண்மை;
  }

  திரும்பு *glyph != 0;
}

நிலையான hb_bool_t
hb_ft_get_font_h_extents (hb_font_t *font HB_UNUSED,
			  வெறுமை *font_data,
			  hb_font_extents_t *metrics,
			  வெறுமை *user_data HB_UNUSED)
{
  மாறிலி hb_ft_font_t *ft_font = (மாறிலி hb_ft_font_t *) font_data;
  _hb_ft_hb_font_check_changed (font, ft_font);

  hb_lock_t lock (ft_font->lock);
  FT_Face ft_face = ft_font->ft_face;
  தசம y_mult;
#ifdef HAVE_FT_GET_TRANSFORM
  எனில் (ft_font->transform)
  {
    FT_Matrix matrix;
    FT_Get_Transform (ft_face, &matrix, nullptr);
    y_mult = sqrtf ((தசம)matrix.yx * matrix.yx + (தசம)matrix.yy * matrix.yy) / 65536.f;
    y_mult *= font->y_scale < 0 ? -1 : +1;
  }
  இல்லையெனில்
#endif
  {
    y_mult = font->y_scale < 0 ? -1 : +1;
  }

  எனில் (ft_face->units_per_EM != 0)
  {
    இரட்டை ascender = FT_MulFix(ft_face->ascender, ft_face->size->metrics.y_scale);
    இரட்டை descender = FT_MulFix(ft_face->descender, ft_face->size->metrics.y_scale);
    இரட்டை height = FT_MulFix( ft_face->height, ft_face->size->metrics.y_scale );
    metrics->ascender = hb_clamp_to<hb_position_t> ((இரட்டை) y_mult * ascender);
    metrics->descender = hb_clamp_to<hb_position_t> ((இரட்டை) y_mult * descender);
    metrics->வரி_gap = hb_clamp_to<hb_position_t> ((இரட்டை) y_mult * (height - (ascender - descender)));
  }
  இல்லையெனில்
  {
    /* Bitmap-only font, eg. color bitmap font. */
    இரட்டை ascender = ft_face->size->metrics.ascender;
    இரட்டை descender = ft_face->size->metrics.descender;
    இரட்டை height = ft_face->size->metrics.height;
    metrics->ascender = hb_clamp_to<hb_position_t> ((இரட்டை) y_mult * ascender);
    metrics->descender = hb_clamp_to<hb_position_t> ((இரட்டை) y_mult * descender);
    metrics->வரி_gap = hb_clamp_to<hb_position_t> ((இரட்டை) y_mult * (height - (ascender - descender)));
  }

  திரும்பு உண்மை;
}

#ifndef HB_NO_DRAW

நிலையான முழு
_hb_ft_move_to (மாறிலி FT_Vector *to,
		வெறுமை *arg)
{
  hb_draw_session_t *drawing = (hb_draw_session_t *) arg;
  drawing->move_to (to->x, to->y);
  திரும்பு FT_Err_Ok;
}

நிலையான முழு
hb_ft_வரி_to (மாறிலி FT_Vector *to,
		வெறுமை *arg)
{
  hb_draw_session_t *drawing = (hb_draw_session_t *) arg;
  drawing->வரி_to (to->x, to->y);
  திரும்பு FT_Err_Ok;
}

நிலையான முழு
_hb_ft_conic_to (மாறிலி FT_Vector *control,
		 மாறிலி FT_Vector *to,
		 வெறுமை *arg)
{
  hb_draw_session_t *drawing = (hb_draw_session_t *) arg;
  drawing->quadratic_to (control->x, control->y,
			 to->x, to->y);
  திரும்பு FT_Err_Ok;
}

நிலையான முழு
_hb_ft_cubic_to (மாறிலி FT_Vector *control1,
		 மாறிலி FT_Vector *control2,
		 மாறிலி FT_Vector *to,
		 வெறுமை *arg)
{
  hb_draw_session_t *drawing = (hb_draw_session_t *) arg;
  drawing->cubic_to (control1->x, control1->y,
		     control2->x, control2->y,
		     to->x, to->y);
  திரும்பு FT_Err_Ok;
}

நிலையான hb_bool_t
hb_ft_draw_glyph_or_fail (hb_font_t *font,
			  வெறுமை *font_data,
			  hb_codepoint_t glyph,
			  hb_draw_funcs_t *draw_funcs, வெறுமை *draw_data,
			  வெறுமை *user_data HB_UNUSED)
{
  மாறிலி hb_ft_font_t *ft_font = (மாறிலி hb_ft_font_t *) font_data;
  _hb_ft_hb_font_check_changed (font, ft_font);

  hb_lock_t lock (ft_font->lock);
  FT_Face ft_face = ft_font->ft_face;

  எனில் (unlikely (FT_Load_Glyph (ft_face, glyph,
			       FT_LOAD_NO_BITMAP | ft_font->load_flags)))
    திரும்பு பொய்;

  எனில் (ft_face->glyph->format != FT_GLYPH_FORMAT_OUTLINE)
    திரும்பு பொய்;

  மாறிலி FT_Outline_Funcs outline_funcs = {
    _hb_ft_move_to,
    hb_ft_வரி_to,
    _hb_ft_conic_to,
    _hb_ft_cubic_to,
    0, /* shift */
    0, /* delta */
  };

  hb_draw_session_t draw_session {draw_funcs, draw_data};

  FT_Outline_Decompose (&ft_face->glyph->outline,
			&outline_funcs,
			&draw_session);

  திரும்பு உண்மை;
}
#endif

#ifndef HB_NO_PAINT
#எனில் (FREETYPE_MAJOR*10000 + FREETYPE_MINOR*100 + FREETYPE_PATCH) >= 21300

#அந்தர்பாவ "hb-ft-colr.hh"

நிலையான hb_bool_t
hb_ft_paint_glyph_or_fail (hb_font_t *font,
			   வெறுமை *font_data,
			   hb_codepoint_t gid,
			   hb_paint_funcs_t *paint_funcs, வெறுமை *paint_data,
			   unsigned முழு palette_index,
			   hb_color_t foreground,
			   வெறுமை *user_data)
{
  மாறிலி hb_ft_font_t *ft_font = (மாறிலி hb_ft_font_t *) font_data;
  _hb_ft_hb_font_check_changed (font, ft_font);

  hb_lock_t lock (ft_font->lock);
  FT_Face ft_face = ft_font->ft_face;

  FT_Long load_flags = ft_font->load_flags | FT_LOAD_COLOR;
#எனில் (FREETYPE_MAJOR*10000 + FREETYPE_MINOR*100 + FREETYPE_PATCH) >= 21301
  load_flags |= FT_LOAD_NO_SVG;
#endif

  /* We release the lock before calling into glyph callbacks, such that
   * eg. draw API can call back into the face.*/

  எனில் (unlikely (FT_Load_Glyph (ft_face, gid, load_flags)))
    திரும்பு பொய்;

  எனில் (ft_face->glyph->format == FT_GLYPH_FORMAT_OUTLINE)
  {
    எனில் (hb_ft_paint_glyph_colr (font, font_data, gid,
				paint_funcs, paint_data,
				palette_index, foreground,
				user_data))
      திரும்பு உண்மை;

    // Outline glyph
    திரும்பு பொய்;
  }

  auto *glyph = ft_face->glyph;
  எனில் (glyph->format == FT_GLYPH_FORMAT_BITMAP)
  {
    தருக்க ret = பொய்;
    auto &bitmap = glyph->bitmap;
    எனில் (bitmap.pixel_mode == FT_PIXEL_MODE_BGRA)
    {
      எனில் (bitmap.pitch != (signed) bitmap.width * 4)
        திரும்பு ret;

      ft_font->lock.unlock ();

      hb_blob_t *blob = hb_blob_create ((மாறிலி எழுத்து *) bitmap.buffer,
					bitmap.pitch * bitmap.rows,
					HB_MEMORY_MODE_DUPLICATE,
					nullptr, nullptr);

      hb_glyph_extents_t extents;
      எனில் (!font->get_glyph_extents (gid, &extents, பொய்))
	செல் out;

      எனில் (paint_funcs->image (paint_data,
			      blob,
			      bitmap.width,
			      bitmap.rows,
			      HB_PAINT_IMAGE_FORMAT_BGRA,
			      0.f,
			      &extents))
        ret = உண்மை;

    out:
      hb_blob_destroy (blob);
      ft_font->lock.lock ();
    }

    திரும்பு ret;
  }
  திரும்பு பொய்;
}
#endif
#endif


நிலையான உள்வரி வெறுமை free_static_ft_funcs ();

நிலையான கட்டமைப்பு hb_ft_font_funcs_lazy_loader_t : hb_font_funcs_lazy_loader_t<hb_ft_font_funcs_lazy_loader_t>
{
  நிலையான hb_font_funcs_t *create ()
  {
    hb_font_funcs_t *funcs = hb_font_funcs_create ();

    hb_font_funcs_set_nominal_glyph_func (funcs, hb_ft_get_nominal_glyph, nullptr, nullptr);
    hb_font_funcs_set_nominal_glyphs_func (funcs, hb_ft_get_nominal_glyphs, nullptr, nullptr);
    hb_font_funcs_set_variation_glyph_func (funcs, hb_ft_get_variation_glyph, nullptr, nullptr);

    hb_font_funcs_set_font_h_extents_func (funcs, hb_ft_get_font_h_extents, nullptr, nullptr);
    hb_font_funcs_set_glyph_h_advances_func (funcs, hb_ft_get_glyph_h_advances, nullptr, nullptr);

#ifndef HB_NO_VERTICAL
    hb_font_funcs_set_glyph_v_advance_func (funcs, hb_ft_get_glyph_v_advance, nullptr, nullptr);
    hb_font_funcs_set_glyph_v_origin_func (funcs, hb_ft_get_glyph_v_origin, nullptr, nullptr);
#endif

#ifndef HB_NO_OT_SHAPE_FALLBACK
    hb_font_funcs_set_glyph_h_kerning_func (funcs, hb_ft_get_glyph_h_kerning, nullptr, nullptr);
#endif
    hb_font_funcs_set_glyph_extents_func (funcs, hb_ft_get_glyph_extents, nullptr, nullptr);
    hb_font_funcs_set_glyph_contour_point_func (funcs, hb_ft_get_glyph_contour_point, nullptr, nullptr);
    hb_font_funcs_set_glyph_name_func (funcs, hb_ft_get_glyph_name, nullptr, nullptr);
    hb_font_funcs_set_glyph_from_name_func (funcs, hb_ft_get_glyph_from_name, nullptr, nullptr);

#ifndef HB_NO_DRAW
    hb_font_funcs_set_draw_glyph_or_fail_func (funcs, hb_ft_draw_glyph_or_fail, nullptr, nullptr);
#endif

#ifndef HB_NO_PAINT
#எனில் (FREETYPE_MAJOR*10000 + FREETYPE_MINOR*100 + FREETYPE_PATCH) >= 21300
    hb_font_funcs_set_paint_glyph_or_fail_func (funcs, hb_ft_paint_glyph_or_fail, nullptr, nullptr);
#endif
#endif

    hb_font_funcs_make_immutable (funcs);

    hb_atexit (free_static_ft_funcs);

    திரும்பு funcs;
  }
} static_ft_funcs;

நிலையான உள்வரி
வெறுமை free_static_ft_funcs ()
{
  static_ft_funcs.free_instance ();
}

நிலையான hb_font_funcs_t *
_hb_ft_get_font_funcs ()
{
  திரும்பு static_ft_funcs.get_unconst ();
}

நிலையான வெறுமை
_hb_ft_font_set_funcs (hb_font_t *font, FT_Face ft_face, தருக்க unref,
		       தருக்க static_library = பொய்)
{
  தருக்க குறியீடு = ft_face->charmap && ft_face->charmap->encoding == FT_ENCODING_MS_குறியீடு;

  hb_ft_font_t *ft_font = _hb_ft_font_create (ft_face, குறியீடு, unref, static_library);
  எனில் (unlikely (!ft_font)) திரும்பு;

  hb_font_set_funcs (font,
		     _hb_ft_get_font_funcs (),
		     ft_font,
		     _hb_ft_font_destroy);
}


நிலையான hb_blob_t *
_hb_ft_reference_table (hb_face_t *face HB_UNUSED, hb_tag_t tag, வெறுமை *user_data)
{
  FT_Face ft_face = (FT_Face) user_data;
  FT_Byte *buffer;
  FT_ULong  length = 0;
  FT_Error error;

  /* In new FreeType, a tag value of 1 loads the SFNT table directory. Reject it. */
  எனில் (tag == 1)
    திரும்பு nullptr;

  /* Note: FreeType like HarfBuzz uses the NONE tag for fetching the entire blob */

  error = FT_Load_Sfnt_Table (ft_face, tag, 0, nullptr, &length);
  எனில் (error)
    திரும்பு nullptr;

  buffer = (FT_Byte *) hb_malloc (length);
  எனில் (!buffer)
    திரும்பு nullptr;
  auto buffer_guard = hb_make_scope_guard ([&]() { hb_free (buffer); });

  error = FT_Load_Sfnt_Table (ft_face, tag, 0, buffer, &length);
  எனில் (error)
    திரும்பு nullptr;

  buffer_guard.release ();
  திரும்பு hb_blob_create ((மாறிலி எழுத்து *) buffer, length,
			 HB_MEMORY_MODE_WRITABLE,
			 buffer, hb_free);
}

நிலையான unsigned
_hb_ft_get_table_tags (மாறிலி hb_face_t *face HB_UNUSED,
		       unsigned முழு start_offset,
		       unsigned முழு *table_மொத்தம்_2,
		       hb_tag_t *table_tags,
		       வெறுமை *user_data)
{
  FT_Face ft_face = (FT_Face) user_data;

  FT_ULong population = 0;
  FT_Sfnt_Table_Info (ft_face,
		      0, // table_index; ignored
		      nullptr,
                      &population);

  எனில் (!table_மொத்தம்_2)
    திரும்பு population;

  எனில் (unlikely (start_offset >= population))
  {
    *table_மொத்தம்_2 = 0;
    திரும்பு population;
  }

  unsigned end_offset = start_offset + *table_மொத்தம்_2;
  எனில் (unlikely (end_offset < start_offset)) /* Overflow. */
  {
    *table_மொத்தம்_2 = 0;
    திரும்பு population;
  }
  end_offset = hb_min (end_offset, (unsigned) population);

  *table_மொத்தம்_2 = end_offset - start_offset;
  எனில் (table_tags)
    க்கு (unsigned i = start_offset; i < end_offset; i++)
    {
      FT_ULong tag = 0, length;
      FT_Sfnt_Table_Info (ft_face, i, &tag, &length);
      table_tags[i - start_offset] = tag;
    }

  திரும்பு population;
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

  எனில் (!ft_face->stream->பட) {
    hb_blob_t *blob;

    blob = hb_blob_create ((மாறிலி எழுத்து *) ft_face->stream->base,
			   (unsigned முழு) ft_face->stream->size,
			   HB_MEMORY_MODE_READONLY,
			   ft_face, destroy);
    face = hb_face_create (blob, ft_face->face_index);
    hb_blob_destroy (blob);
  } இல்லையெனில் {
    face = hb_face_create_for_tables (_hb_ft_reference_table, ft_face, destroy);
    hb_face_set_get_table_tags_func (face, _hb_ft_get_table_tags, ft_face, nullptr);
  }

  hb_face_set_index (face, ft_face->face_index);
  hb_face_set_upem (face, ft_face->units_per_EM);

  திரும்பு face;
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
  திரும்பு hb_ft_face_create (ft_face, _hb_ft_face_destroy);
}

நிலையான வெறுமை
hb_ft_face_finalize (வெறுமை *arg)
{
  FT_Face ft_face = (FT_Face) arg;
  hb_face_destroy ((hb_face_t *) ft_face->generic.data);
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
  எனில் (unlikely (!ft_face->generic.data || ft_face->generic.finalizer != hb_ft_face_finalize))
  {
    எனில் (ft_face->generic.finalizer)
      ft_face->generic.finalizer (ft_face);

    ft_face->generic.data = hb_ft_face_create (ft_face, nullptr);
    ft_face->generic.finalizer = hb_ft_face_finalize;
  }

  திரும்பு hb_face_reference ((hb_face_t *) ft_face->generic.data);
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
  _hb_ft_font_set_funcs (font, ft_face, பொய்);
  hb_ft_font_changed (font);
  திரும்பு font;
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
வெறுமை
hb_ft_font_changed (hb_font_t *font)
{
  எனில் (font->destroy != (hb_destroy_func_t) _hb_ft_font_destroy)
    திரும்பு;

  hb_ft_font_t *ft_font = (hb_ft_font_t *) font->user_data;

  FT_Face ft_face = ft_font->ft_face;

  hb_font_set_scale (font,
		     (முழு) (((uint64_t) ft_face->size->metrics.x_scale * (uint64_t) ft_face->units_per_EM + (1u<<15)) >> 16),
		     (முழு) (((uint64_t) ft_face->size->metrics.y_scale * (uint64_t) ft_face->units_per_EM + (1u<<15)) >> 16));
#எனில் 0 /* hb-ft works in no-hinting model */
  hb_font_set_ppem (font,
		    ft_face->size->metrics.x_ppem,
		    ft_face->size->metrics.y_ppem);
#endif

#எனில் defined(HAVE_FT_GET_VAR_BLEND_COORDINATES) && !defined(HB_NO_VAR)
  FT_MM_Var *mm_var = nullptr;
  எனில் (!FT_Get_MM_Var (ft_face, &mm_var))
  {
    FT_Fixed *ft_coords = (FT_Fixed *) hb_calloc (mm_var->இலக்கம்_axis, அளவு (FT_Fixed));
    முழு *coords = (முழு *) hb_calloc (mm_var->இலக்கம்_axis, அளவு (முழு));
    எனில் (coords && ft_coords)
    {
      எனில் (!FT_Get_Var_Blend_Coordinates (ft_face, mm_var->இலக்கம்_axis, ft_coords))
      {
	தருக்க nonzero = பொய்;

	க்கு (unsigned முழு i = 0; i < mm_var->இலக்கம்_axis; ++i)
	 {
	  coords[i] = (ft_coords[i] + 2) >> 2;
	  nonzero = nonzero || coords[i];
	 }

	எனில் (nonzero)
	  hb_font_set_var_coords_normalized (font, coords, mm_var->இலக்கம்_axis);
	இல்லையெனில்
	  hb_font_set_var_coords_normalized (font, nullptr, 0);
      }
    }
    hb_free (coords);
    hb_free (ft_coords);
#ifdef HAVE_FT_DONE_MM_VAR
    FT_Done_MM_Var (ft_face->glyph->library, mm_var);
#இல்லையெனில்
    hb_free (mm_var);
#endif
  }
#endif

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
hb_ft_hb_font_changed (hb_font_t *font)
{
  எனில் (font->destroy != (hb_destroy_func_t) _hb_ft_font_destroy)
    திரும்பு பொய்;

  hb_ft_font_t *ft_font = (hb_ft_font_t *) font->user_data;

  திரும்பு _hb_ft_hb_font_check_changed (font, ft_font);
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
  திரும்பு hb_ft_font_create (ft_face, _hb_ft_face_destroy);
}


நிலையான வெறுமை * _hb_ft_alloc (FT_Memory memory, long size)
{ திரும்பு hb_malloc (size); }

நிலையான வெறுமை _hb_ft_free (FT_Memory memory, வெறுமை *block)
{ hb_free (block); }

நிலையான வெறுமை * _hb_ft_realloc (FT_Memory memory, long cur_size, long new_size, வெறுமை *block)
{ திரும்பு hb_realloc (block, new_size); }

நிலையான FT_MemoryRec_ m =
{
  nullptr,
  _hb_ft_alloc,
  _hb_ft_free,
  _hb_ft_realloc
};

நிலையான உள்வரி வெறுமை free_static_ft_library ();
நிலையான உள்வரி வெறுமை free_static_ft_library_mutex ();

நிலையான கட்டமைப்பு hb_ft_library_mutex_lazy_loader_t : hb_lazy_loader_t<hb_mutex_t,
								   hb_ft_library_mutex_lazy_loader_t>
{
  நிலையான hb_mutex_t *create ()
  {
    hb_mutex_t *lock = (hb_mutex_t *) hb_calloc (1, அளவு (hb_mutex_t));
    எனில் (unlikely (!lock))
      திரும்பு nullptr;

    lock = புதிய (lock) hb_mutex_t;
    hb_atexit (free_static_ft_library_mutex);
    திரும்பு lock;
  }
  நிலையான வெறுமை destroy (hb_mutex_t *lock)
  {
    lock->~hb_mutex_t ();
    hb_free (lock);
  }
  நிலையான hb_mutex_t *get_null ()
  {
    திரும்பு nullptr;
  }
} static_ft_library_mutex;

நிலையான கட்டமைப்பு hb_ft_library_lazy_loader_t : hb_lazy_loader_t<hb_remove_pointer<FT_Library>,
							     hb_ft_library_lazy_loader_t>
{
  நிலையான FT_Library create ()
  {
    FT_Library l;
    எனில் (FT_New_Library (&m, &l))
      திரும்பு nullptr;

    FT_Add_Default_Modules (l);
    FT_Set_Default_Properties (l);

    hb_atexit (free_static_ft_library);

    திரும்பு l;
  }
  நிலையான வெறுமை destroy (FT_Library l)
  {
    FT_Done_Library (l);
  }
  நிலையான FT_Library get_null ()
  {
    திரும்பு nullptr;
  }
} static_ft_library;

நிலையான உள்வரி
வெறுமை free_static_ft_library_mutex ()
{
  static_ft_library_mutex.free_instance ();
}

நிலையான உள்வரி
வெறுமை free_static_ft_library ()
{
  hb_lock_t lock (static_ft_library_mutex.get_unconst ());
  static_ft_library.free_instance ();
}

நிலையான FT_Library
reference_ft_library ()
{
  hb_mutex_t *mutex = static_ft_library_mutex.get_unconst ();
  எனில் (unlikely (!mutex))
    திரும்பு nullptr;
  hb_lock_t lock (mutex);
  FT_Library l = static_ft_library.get_unconst ();
  எனில் (unlikely (FT_Reference_Library (l)))
  {
    DEBUG_செய்தி (FT, l, "FT_Reference_Library() failed");
    திரும்பு nullptr;
  }
  திரும்பு l;
}

நிலையான hb_user_data_சாவி_t ft_library_சாவி = {0};

நிலையான வெறுமை
_hb_ft_face_destroy_static (வெறுமை *data)
{
  hb_lock_t lock (static_ft_library_mutex.get_unconst ());
  FT_Done_Face ((FT_Face) data);
}

நிலையான FT_Error
new_ft_face (FT_Library  ft_library,
	     மாறிலி எழுத்து *file_name,
	     unsigned முழு index,
	     FT_Face     *ft_face)
{
  hb_lock_t lock (static_ft_library_mutex.get_unconst ());
  திரும்பு FT_New_Face (ft_library, file_name, index, ft_face);
}

நிலையான FT_Error
new_ft_memory_face (FT_Library    ft_library,
		    மாறிலி FT_Byte *blob_data,
		    unsigned முழு   blob_size,
		    unsigned முழு   index,
		    FT_Face       *ft_face)
{
  hb_lock_t lock (static_ft_library_mutex.get_unconst ());
  திரும்பு FT_New_Memory_Face (ft_library, blob_data, blob_size, index, ft_face);
}

நிலையான வெறுமை
finalize_ft_library (வெறுமை *arg)
{
  FT_Face ft_face = (FT_Face) arg;
  FT_Done_Library ((FT_Library) ft_face->generic.data);
}

நிலையான வெறுமை
destroy_ft_library (வெறுமை *arg)
{
  hb_lock_t lock (static_ft_library_mutex.get_unconst ());
  FT_Done_Library ((FT_Library) arg);
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
hb_ft_face_create_from_file_or_fail (மாறிலி எழுத்து   *file_name,
				     unsigned முழு  index)
{
  FT_Library ft_library = reference_ft_library ();
  எனில் (unlikely (!ft_library))
  {
    DEBUG_செய்தி (FT, ft_library, "reference_ft_library failed");
    திரும்பு nullptr;
  }

  FT_Face ft_face;
  எனில் (unlikely (new_ft_face (ft_library,
			     file_name,
			     index,
			     &ft_face)))
  {
    destroy_ft_library (ft_library);
    திரும்பு nullptr;
  }

  FT_Reference_Face (ft_face);
  hb_face_t *face = hb_ft_face_create (ft_face, _hb_ft_face_destroy_static);
  _hb_ft_face_destroy_static (ft_face);

  ft_face->generic.data = ft_library;
  ft_face->generic.finalizer = finalize_ft_library;

  எனில் (hb_face_is_immutable (face))
    திரும்பு nullptr;

  திரும்பு face;
}

நிலையான hb_user_data_சாவி_t ft_blob_சாவி = {0};

நிலையான வெறுமை
_destroy_blob (வெறுமை *p)
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
				     unsigned முழு  index)
{
  FT_Library ft_library = reference_ft_library ();
  எனில் (unlikely (!ft_library))
  {
    DEBUG_செய்தி (FT, ft_library, "reference_ft_library failed");
    திரும்பு nullptr;
  }

  hb_blob_make_immutable (blob);
  unsigned blob_size;
  மாறிலி எழுத்து *blob_data = hb_blob_get_data (blob, &blob_size);

  FT_Face ft_face;
  எனில் (unlikely (new_ft_memory_face (ft_library,
				    (மாறிலி FT_Byte *) blob_data,
				    blob_size,
				    index,
				    &ft_face)))
  {
    destroy_ft_library (ft_library);
    திரும்பு nullptr;
  }

  FT_Reference_Face (ft_face);
  hb_face_t *face = hb_ft_face_create (ft_face, _hb_ft_face_destroy_static);
  _hb_ft_face_destroy_static (ft_face);

  ft_face->generic.data = ft_library;
  ft_face->generic.finalizer = finalize_ft_library;

  எனில் (hb_face_is_immutable (face))
    திரும்பு nullptr;

  // Hook the blob to the hb_face_t, since FT_Face still needs it.
  hb_blob_reference (blob);
  எனில் (!hb_face_set_user_data (face, &ft_blob_சாவி, blob, _destroy_blob, உண்மை))
  {
    hb_blob_destroy (blob);
    hb_face_destroy (face);
    திரும்பு nullptr;
  }

  திரும்பு face;
}

நிலையான வெறுமை
_release_blob (வெறுமை *arg)
{
  FT_Face ft_face = (FT_Face) arg;
  hb_blob_destroy ((hb_blob_t *) ft_face->generic.data);
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
வெறுமை
hb_ft_font_set_funcs (hb_font_t *font)
{
  முழு load_flags = FT_LOAD_DEFAULT | FT_LOAD_NO_HINTING;
  எனில் (font->destroy == (hb_destroy_func_t) _hb_ft_font_destroy && font->user_data)
  {
    மாறிலி hb_ft_font_t *existing_ft_font = (மாறிலி hb_ft_font_t *) font->user_data;
    load_flags = existing_ft_font->load_flags;
  }

  // In case of failure...
  hb_font_set_funcs (font,
		     hb_font_funcs_get_empty (),
		     nullptr, nullptr);

  hb_blob_t *blob = hb_face_reference_blob (font->face);
  unsigned முழு blob_length;
  மாறிலி எழுத்து *blob_data = hb_blob_get_data (blob, &blob_length);
  எனில் (unlikely (!blob_length))
    DEBUG_செய்தி (FT, font, "Font face has empty blob");

  FT_Library ft_library = reference_ft_library ();
  எனில் (unlikely (!ft_library))
  {
    hb_blob_destroy (blob);
    DEBUG_செய்தி (FT, font, "reference_ft_library failed");
    திரும்பு;
  }

  FT_Face ft_face = nullptr;
  எனில் (unlikely (new_ft_memory_face (ft_library,
				    (மாறிலி FT_Byte *) blob_data,
				    blob_length,
				    hb_face_get_index (font->face),
				    &ft_face)))
  {
    hb_blob_destroy (blob);
    destroy_ft_library (ft_library);
    DEBUG_செய்தி (FT, font, "FT_New_Memory_Face() failed");
    திரும்பு;
  }

  எனில் (FT_Select_Charmap (ft_face, FT_ENCODING_MS_குறியீடு))
    FT_Select_Charmap (ft_face, FT_ENCODING_UNICODE);

  // Hook the blob to the FT_Face
  ft_face->generic.data = blob;
  ft_face->generic.finalizer = _release_blob;

  // And the FT_Library to the blob
  எனில் (unlikely (!hb_blob_set_user_data (blob, &ft_library_சாவி, ft_library, destroy_ft_library, உண்மை)))
  {
    DEBUG_செய்தி (FT, font, "hb_blob_set_user_data() failed");
    _hb_ft_face_destroy_static (ft_face);
    destroy_ft_library (ft_library);
    திரும்பு;
  }

  _hb_ft_font_set_funcs (font, ft_face, உண்மை, உண்மை);
  hb_ft_font_set_load_flags (font, load_flags);

  _hb_ft_hb_font_changed (font, ft_face);
}

#endif
