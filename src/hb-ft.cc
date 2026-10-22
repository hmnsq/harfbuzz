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

#अन्तर्भाव "hb.hh"

#ifdef HAVE_FREETYPE

#अन्तर्भाव "hb-ft.h"

#अन्तर्भाव "hb-cache.hh"
#अन्तर्भाव "hb-draw.hh"
#अन्तर्भाव "hb-font.hh"
#अन्तर्भाव "hb-machinery.hh"
#अन्तर्भाव "hb-ot-os2-table.hh"
#अन्तर्भाव "hb-ot-shaper-arabic-pua.hh"
#अन्तर्भाव "hb-paint.hh"

#अन्तर्भाव FT_MODULE_H
#अन्तर्भाव FT_ADVANCES_H
#अन्तर्भाव FT_MULTIPLE_MASTERS_H
#अन्तर्भाव FT_OUTLINE_H
#अन्तर्भाव FT_TRUETYPE_TABLES_H
#अन्तर्भाव FT_SYNTHESIS_H
#यदि (FREETYPE_MAJOR*10000 + FREETYPE_MINOR*100 + FREETYPE_PATCH) >= 21300
#अन्तर्भाव FT_COLOR_H
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


using hb_ft_advance_cache_t = hb_cache_t<16, 24, 8, असत्य>;

स्थिर शून्य _hb_ft_face_destroy_static (शून्य *data);

संरचना hb_ft_font_t
{
  पूर्णाङ्क load_flags;
  तार्किक चिन्ह; /* Whether selected cmap is symbol cmap. */
  तार्किक unref; /* Whether to destroy ft_face when done. */
  तार्किक transform; /* Whether to apply FT_Face's transform. */
  तार्किक static_library; /* Whether ft_face uses static_ft_library. */

  mutable hb_mutex_t lock; /* Protects members below. */
  FT_Face ft_face;
  mutable hb_atomic_t<unsigned> cached_serial;
  mutable hb_ft_advance_cache_t advance_cache;
};

स्थिर hb_ft_font_t *
_hb_ft_font_create (FT_Face ft_face, तार्किक चिन्ह, तार्किक unref, तार्किक static_library)
{
  hb_ft_font_t *ft_font = (hb_ft_font_t *) hb_calloc (1, परिमाण (hb_ft_font_t));
  यदि (unlikely (!ft_font)) प्रत्यागम nullptr;

  ft_font->lock.init ();
  ft_font->ft_face = ft_face;
  ft_font->चिन्ह = चिन्ह;
  ft_font->unref = unref;
  ft_font->static_library = static_library;

  ft_font->load_flags = FT_LOAD_DEFAULT | FT_LOAD_NO_HINTING;

  ft_font->cached_serial = UINT_MAX;
  नूतन (&ft_font->advance_cache) hb_ft_advance_cache_t;

  प्रत्यागम ft_font;
}

स्थिर शून्य
_hb_ft_face_destroy (शून्य *data)
{
  FT_Done_Face ((FT_Face) data);
}

स्थिर शून्य
_hb_ft_font_destroy (शून्य *data)
{
  hb_ft_font_t *ft_font = (hb_ft_font_t *) data;

  यदि (ft_font->unref)
  {
    यदि (ft_font->static_library)
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
    अन्यथा
      _hb_ft_face_destroy (ft_font->ft_face);
  }

  ft_font->lock.fini ();

  hb_free (ft_font);
}


/* hb_font changed, update FT_Face. */
स्थिर शून्य _hb_ft_hb_font_changed (hb_font_t *font, FT_Face ft_face)
{
  यदि (unlikely (font->destroy != (hb_destroy_func_t) _hb_ft_font_destroy))
    प्रत्यागम;

  hb_ft_font_t *ft_font = (hb_ft_font_t *) font->user_data;

  भिन्न x_mult = 1.f, y_mult = 1.f;

  यदि (font->x_scale < 0) x_mult = -x_mult;
  यदि (font->y_scale < 0) y_mult = -y_mult;

  यदि (FT_Set_Char_Size (ft_face,
			abs (font->x_scale), abs (font->y_scale),
			0, 0
#यदि 0
		    font->x_ppem * 72 * 64 / font->x_scale,
		    font->y_ppem * 72 * 64 / font->y_scale
#endif
     ) && ft_face->अंक_fixed_sizes)
  {
#ifdef HAVE_FT_GET_TRANSFORM
    /* Bitmap font, eg. bitmap color emoji. */
    /* Pick largest size? */
    पूर्णाङ्क x_scale  = ft_face->available_sizes[ft_face->अंक_fixed_sizes - 1].x_ppem;
    पूर्णाङ्क y_scale = ft_face->available_sizes[ft_face->अंक_fixed_sizes - 1].y_ppem;
    FT_Set_Char_Size (ft_face,
		      x_scale, y_scale,
		      0, 0);

    /* This contains the sign that was previously in x_mult/y_mult. */
    x_mult = (भिन्न) font->x_scale / x_scale;
    y_mult = (भिन्न) font->y_scale / y_scale;
#endif
  }
  अन्यथा
  { /* Shrug */ }


  यदि (x_mult != 1.f || y_mult != 1.f)
  {
    FT_Matrix matrix = { (पूर्णाङ्क) roundf (x_mult * (1<<16)), 0,
			  0, (पूर्णाङ्क) roundf (y_mult * (1<<16))};
    FT_Set_Transform (ft_face, &matrix, nullptr);
    ft_font->transform = सत्य;
  }
  अन्यथा
    FT_Set_Transform (ft_face, nullptr, nullptr);

#यदि defined(HAVE_FT_GET_VAR_BLEND_COORDINATES) && !defined(HB_NO_VAR)
  यदि (font->has_nonzero_coords)
  {
    unsigned पूर्णाङ्क अंक_coords;
    ध्रुव भिन्न *coords = hb_font_get_var_coords_design (font, &अंक_coords);
    FT_Fixed *ft_coords = (FT_Fixed *) hb_calloc (अंक_coords, परिमाण (FT_Fixed));
    यदि (ft_coords)
    {
      प्रति (unsigned पूर्णाङ्क i = 0; i < अंक_coords; i++)
	  ft_coords[i] = coords[i] * 65536.f;
      FT_Set_Var_Design_Coordinates (ft_face, अंक_coords, ft_coords);
      hb_free (ft_coords);
    }
  }
  अन्यथा यदि (font->अंक_coords)
  {
    // Some old versions of FreeType crash if we
    // call this function on non-variable fonts.
    FT_Set_Var_Design_Coordinates (ft_face, 0, nullptr);
  }
#endif
}

/* Check if hb_font changed, update FT_Face. */
स्थिर अन्तःस्थ तार्किक
_hb_ft_hb_font_check_changed (hb_font_t *font,
			      ध्रुव hb_ft_font_t *ft_font)
{
  यदि (font->serial != ft_font->cached_serial)
  {
    hb_lock_t lock (ft_font->lock);
    _hb_ft_hb_font_changed (font, ft_font->ft_face);
    ft_font->advance_cache.clear ();
    ft_font->cached_serial.set_release (font->serial.get_acquire ());
    प्रत्यागम सत्य;
  }
  प्रत्यागम असत्य;
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
शून्य
hb_ft_font_set_load_flags (hb_font_t *font, पूर्णाङ्क load_flags)
{
  यदि (hb_object_is_immutable (font))
    प्रत्यागम;

  यदि (unlikely (font->destroy != (hb_destroy_func_t) _hb_ft_font_destroy))
    प्रत्यागम;

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
पूर्णाङ्क
hb_ft_font_get_load_flags (hb_font_t *font)
{
  यदि (unlikely (font->destroy != (hb_destroy_func_t) _hb_ft_font_destroy))
    प्रत्यागम 0;

  ध्रुव hb_ft_font_t *ft_font = (ध्रुव hb_ft_font_t *) font->user_data;

  प्रत्यागम ft_font->load_flags;
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
  यदि (unlikely (font->destroy != (hb_destroy_func_t) _hb_ft_font_destroy))
    प्रत्यागम nullptr;

  ध्रुव hb_ft_font_t *ft_font = (ध्रुव hb_ft_font_t *) font->user_data;

  प्रत्यागम ft_font->ft_face;
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
  प्रत्यागम hb_ft_font_get_ft_face (font);
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
  यदि (unlikely (font->destroy != (hb_destroy_func_t) _hb_ft_font_destroy))
    प्रत्यागम nullptr;

  ध्रुव hb_ft_font_t *ft_font = (ध्रुव hb_ft_font_t *) font->user_data;

  ft_font->lock.lock ();

  प्रत्यागम ft_font->ft_face;
}

/**
 * hb_ft_font_unlock_face: (skip)
 * @font: #hb_font_t to work upon
 *
 * Releases an FT_Face previously obtained with hb_ft_font_lock_face().
 *
 * Since: 2.6.5
 **/
शून्य
hb_ft_font_unlock_face (hb_font_t *font)
{
  यदि (unlikely (font->destroy != (hb_destroy_func_t) _hb_ft_font_destroy))
    प्रत्यागम;

  ध्रुव hb_ft_font_t *ft_font = (ध्रुव hb_ft_font_t *) font->user_data;

  ft_font->lock.unlock ();
}


स्थिर hb_bool_t
hb_ft_get_nominal_glyph (hb_font_t *font,
			 शून्य *font_data,
			 hb_codepoint_t unicode,
			 hb_codepoint_t *glyph,
			 शून्य *user_data HB_UNUSED)
{
  ध्रुव hb_ft_font_t *ft_font = (ध्रुव hb_ft_font_t *) font_data;
  hb_lock_t lock (ft_font->lock);
  unsigned पूर्णाङ्क g = FT_Get_Char_Index (ft_font->ft_face, unicode);

  यदि (unlikely (!g))
  {
    यदि (unlikely (ft_font->चिन्ह))
    {
      विकल्प ((unsigned) font->face->table.OS2->get_font_page ()) {
      पक्ष OT::OS2::font_page_t::FONT_PAGE_NONE:
	यदि (unicode <= 0x00FFu)
	  /* For symbol-encoded OpenType fonts, we duplicate the
	   * U+F000..F0FF range at U+0000..U+00FF.  That's what
	   * Windows seems to do, and that's hinted about at:
	   * https://docs.microsoft.com/en-us/typography/opentype/spec/recom
	   * under "Non-Standard (Symbol) Fonts". */
	  g = FT_Get_Char_Index (ft_font->ft_face, 0xF000u + unicode);
	भङ्ग;
#ifndef HB_NO_OT_SHAPER_ARABIC_FALLBACK
      पक्ष OT::OS2::font_page_t::FONT_PAGE_SIMP_ARABIC:
	g = FT_Get_Char_Index (ft_font->ft_face, _hb_arabic_pua_simp_map (unicode));
	भङ्ग;
      पक्ष OT::OS2::font_page_t::FONT_PAGE_TRAD_ARABIC:
	g = FT_Get_Char_Index (ft_font->ft_face, _hb_arabic_pua_trad_map (unicode));
	भङ्ग;
#endif
      स्वभाव:
	भङ्ग;
      }
      यदि (!g)
	प्रत्यागम असत्य;
    }
    अन्यथा
      प्रत्यागम असत्य;
  }

  *glyph = g;
  प्रत्यागम सत्य;
}

स्थिर unsigned पूर्णाङ्क
hb_ft_get_nominal_glyphs (hb_font_t *font HB_UNUSED,
			  शून्य *font_data,
			  unsigned पूर्णाङ्क मोजणी,
			  ध्रुव hb_codepoint_t *first_unicode,
			  unsigned पूर्णाङ्क unicode_stride,
			  hb_codepoint_t *first_glyph,
			  unsigned पूर्णाङ्क glyph_stride,
			  शून्य *user_data HB_UNUSED)
{
  ध्रुव hb_ft_font_t *ft_font = (ध्रुव hb_ft_font_t *) font_data;
  hb_lock_t lock (ft_font->lock);
  unsigned पूर्णाङ्क done;
  प्रति (done = 0;
       done < मोजणी && (*first_glyph = FT_Get_Char_Index (ft_font->ft_face, *first_unicode));
       done++)
  {
    first_unicode = &StructAtOffsetUnaligned<hb_codepoint_t> (first_unicode, unicode_stride);
    first_glyph = &StructAtOffsetUnaligned<hb_codepoint_t> (first_glyph, glyph_stride);
  }
  /* We don't need to do ft_font->symbol dance here, since HB calls the singular
   * nominal_glyph() for what we don't handle here. */
  प्रत्यागम done;
}


स्थिर hb_bool_t
hb_ft_get_variation_glyph (hb_font_t *font HB_UNUSED,
			   शून्य *font_data,
			   hb_codepoint_t unicode,
			   hb_codepoint_t variation_selector,
			   hb_codepoint_t *glyph,
			   शून्य *user_data HB_UNUSED)
{
  ध्रुव hb_ft_font_t *ft_font = (ध्रुव hb_ft_font_t *) font_data;
  hb_lock_t lock (ft_font->lock);
  unsigned पूर्णाङ्क g = FT_Face_GetCharVariantIndex (ft_font->ft_face, unicode, variation_selector);

  यदि (unlikely (!g))
    प्रत्यागम असत्य;

  *glyph = g;
  प्रत्यागम सत्य;
}

स्थिर शून्य
hb_ft_get_glyph_h_advances (hb_font_t* font, शून्य* font_data,
			    unsigned मोजणी,
			    ध्रुव hb_codepoint_t *first_glyph,
			    unsigned glyph_stride,
			    hb_position_t *first_advance,
			    unsigned advance_stride,
			    शून्य *user_data HB_UNUSED)
{
  ध्रुव hb_ft_font_t *ft_font = (ध्रुव hb_ft_font_t *) font_data;
  _hb_ft_hb_font_check_changed (font, ft_font);

  hb_lock_t lock (ft_font->lock);
  FT_Face ft_face = ft_font->ft_face;
  पूर्णाङ्क load_flags = ft_font->load_flags;
  भिन्न x_mult;
#ifdef HAVE_FT_GET_TRANSFORM
  यदि (ft_font->transform)
  {
    FT_Matrix matrix;
    FT_Get_Transform (ft_face, &matrix, nullptr);
    x_mult = sqrtf ((भिन्न)matrix.xx * matrix.xx + (भिन्न)matrix.xy * matrix.xy) / 65536.f;
    x_mult *= font->x_scale < 0 ? -1 : +1;
  }
  अन्यथा
#endif
  {
    x_mult = font->x_scale < 0 ? -1 : +1;
  }

  प्रति (unsigned पूर्णाङ्क i = 0; i < मोजणी; i++)
  {
    hb_position_t advance;
    hb_codepoint_t glyph = *first_glyph;

    unsigned पूर्णाङ्क cv;
    यदि (ft_font->advance_cache.get (glyph, &cv))
      advance = (hb_position_t) cv;
    अन्यथा
    {
      FT_Fixed v = 0;
      FT_Get_Advance (ft_face, glyph, load_flags, &v);
      /* Work around bug that FreeType seems to return negative advance
       * for variable-set fonts if x_scale is negative! */
      द्विगुण scaled = trunc (fabs ((द्विगुण) v) * (द्विगुण) x_mult + (1<<9)) / (1<<10);
      advance = hb_clamp_to<hb_position_t> (floor (scaled));
      ft_font->advance_cache.set (glyph, advance);
    }

    *first_advance = advance;
    first_glyph = &StructAtOffsetUnaligned<hb_codepoint_t> (first_glyph, glyph_stride);
    first_advance = &StructAtOffsetUnaligned<hb_position_t> (first_advance, advance_stride);
  }
}

#ifndef HB_NO_VERTICAL
स्थिर hb_position_t
hb_ft_get_glyph_v_advance (hb_font_t *font,
			   शून्य *font_data,
			   hb_codepoint_t glyph,
			   शून्य *user_data HB_UNUSED)
{
  ध्रुव hb_ft_font_t *ft_font = (ध्रुव hb_ft_font_t *) font_data;
  _hb_ft_hb_font_check_changed (font, ft_font);

  hb_lock_t lock (ft_font->lock);
  FT_Fixed v;
  भिन्न y_mult;
#ifdef HAVE_FT_GET_TRANSFORM
  यदि (ft_font->transform)
  {
    FT_Matrix matrix;
    FT_Get_Transform (ft_font->ft_face, &matrix, nullptr);
    y_mult = sqrtf ((भिन्न)matrix.yx * matrix.yx + (भिन्न)matrix.yy * matrix.yy) / 65536.f;
    y_mult *= font->y_scale < 0 ? -1 : +1;
  }
  अन्यथा
#endif
  {
    y_mult = font->y_scale < 0 ? -1 : +1;
  }

  यदि (unlikely (FT_Get_Advance (ft_font->ft_face, glyph, ft_font->load_flags | FT_LOAD_VERTICAL_LAYOUT, &v)))
    प्रत्यागम 0;

  /* Note: FreeType's vertical metrics grows downward while other FreeType coordinates
   * have a Y growing upward.  Hence the extra negation. */
  द्विगुण advance = floor ((-(द्विगुण) v + (1<<9)) / (1<<10));

  प्रत्यागम hb_clamp_to<hb_position_t> ((द्विगुण) y_mult * advance);
}
#endif

#ifndef HB_NO_VERTICAL
स्थिर hb_bool_t
hb_ft_get_glyph_v_origin (hb_font_t *font,
			  शून्य *font_data,
			  hb_codepoint_t glyph,
			  hb_position_t *x,
			  hb_position_t *y,
			  शून्य *user_data HB_UNUSED)
{
  ध्रुव hb_ft_font_t *ft_font = (ध्रुव hb_ft_font_t *) font_data;
  _hb_ft_hb_font_check_changed (font, ft_font);

  hb_lock_t lock (ft_font->lock);
  FT_Face ft_face = ft_font->ft_face;
  भिन्न x_mult, y_mult;
#ifdef HAVE_FT_GET_TRANSFORM
  यदि (ft_font->transform)
  {
    FT_Matrix matrix;
    FT_Get_Transform (ft_face, &matrix, nullptr);
    x_mult = sqrtf ((भिन्न)matrix.xx * matrix.xx + (भिन्न)matrix.xy * matrix.xy) / 65536.f;
    x_mult *= font->x_scale < 0 ? -1 : +1;
    y_mult = sqrtf ((भिन्न)matrix.yx * matrix.yx + (भिन्न)matrix.yy * matrix.yy) / 65536.f;
    y_mult *= font->y_scale < 0 ? -1 : +1;
  }
  अन्यथा
#endif
  {
    x_mult = font->x_scale < 0 ? -1 : +1;
    y_mult = font->y_scale < 0 ? -1 : +1;
  }

  यदि (unlikely (FT_Load_Glyph (ft_face, glyph, ft_font->load_flags)))
    प्रत्यागम असत्य;

  /* Note: FreeType's vertical metrics grows downward while other FreeType coordinates
   * have a Y growing upward.  Hence the extra negation. */
  *x = hb_clamp_to<hb_position_t> ((द्विगुण) x_mult *
				   ((द्विगुण) ft_face->glyph->metrics.horiBearingX -
				    ft_face->glyph->metrics.vertBearingX));
  *y = hb_clamp_to<hb_position_t> ((द्विगुण) y_mult *
				   ((द्विगुण) ft_face->glyph->metrics.horiBearingY +
				    ft_face->glyph->metrics.vertBearingY));

  प्रत्यागम सत्य;
}
#endif

#ifndef HB_NO_OT_SHAPE_FALLBACK
स्थिर hb_position_t
hb_ft_get_glyph_h_kerning (hb_font_t *font,
			   शून्य *font_data,
			   hb_codepoint_t डावा_glyph,
			   hb_codepoint_t उजवा_glyph,
			   शून्य *user_data HB_UNUSED)
{
  ध्रुव hb_ft_font_t *ft_font = (ध्रुव hb_ft_font_t *) font_data;
  _hb_ft_hb_font_check_changed (font, ft_font);

  hb_lock_t lock (ft_font->lock);
  FT_Vector kerningv;

  FT_Kerning_Mode mode = font->x_ppem ? FT_KERNING_DEFAULT : FT_KERNING_UNFITTED;
  यदि (FT_Get_Kerning (ft_font->ft_face, डावा_glyph, उजवा_glyph, mode, &kerningv))
    प्रत्यागम 0;

  प्रत्यागम hb_clamp_to<hb_position_t> ((int64_t) kerningv.x);
}
#endif

स्थिर तार्किक
hb_ft_is_colr_glyph (hb_font_t *font,
		     शून्य *font_data,
		     hb_codepoint_t gid)
{
#ifndef HB_NO_PAINT
#यदि (FREETYPE_MAJOR*10000 + FREETYPE_MINOR*100 + FREETYPE_PATCH) >= 21300
  ध्रुव hb_ft_font_t *ft_font = (ध्रुव hb_ft_font_t *) font_data;
  FT_Face ft_face = ft_font->ft_face;


  /* COLRv1 */
  FT_OpaquePaint paint = {0};
  यदि (FT_Get_Color_Glyph_Paint (ft_face, gid,
			        FT_COLOR_NO_मूळ_TRANSFORM,
			        &paint))
    प्रत्यागम सत्य;

  /* COLRv0 */
  FT_LayerIterator  iterator;
  FT_UInt  layer_glyph_index;
  FT_UInt  layer_color_index;
  iterator.p  = रिक्त;
  यदि (FT_Get_Color_Glyph_Layer (ft_face,
				gid,
				&layer_glyph_index,
				&layer_color_index,
				&iterator))
    प्रत्यागम सत्य;
#endif
#endif

  प्रत्यागम असत्य;
}

स्थिर hb_bool_t
hb_ft_get_glyph_extents (hb_font_t *font,
			 शून्य *font_data,
			 hb_codepoint_t glyph,
			 hb_glyph_extents_t *extents,
			 शून्य *user_data HB_UNUSED)
{
  // FreeType doesn't return COLR glyph extents.
  यदि (hb_ft_is_colr_glyph (font, font_data, glyph))
    प्रत्यागम असत्य;

  ध्रुव hb_ft_font_t *ft_font = (ध्रुव hb_ft_font_t *) font_data;
  _hb_ft_hb_font_check_changed (font, ft_font);

  hb_lock_t lock (ft_font->lock);
  FT_Face ft_face = ft_font->ft_face;
  भिन्न x_mult, y_mult;

#ifdef HAVE_FT_GET_TRANSFORM
  यदि (ft_font->transform)
  {
    FT_Matrix matrix;
    FT_Get_Transform (ft_face, &matrix, nullptr);
    x_mult = sqrtf ((भिन्न)matrix.xx * matrix.xx + (भिन्न)matrix.xy * matrix.xy) / 65536.f;
    x_mult *= font->x_scale < 0 ? -1 : +1;
    y_mult = sqrtf ((भिन्न)matrix.yx * matrix.yx + (भिन्न)matrix.yy * matrix.yy) / 65536.f;
    y_mult *= font->y_scale < 0 ? -1 : +1;
  }
  अन्यथा
#endif
  {
    x_mult = font->x_scale < 0 ? -1 : +1;
    y_mult = font->y_scale < 0 ? -1 : +1;
  }

  यदि (unlikely (FT_Load_Glyph (ft_face, glyph, ft_font->load_flags)))
    प्रत्यागम असत्य;

  /* Copied from hb_font_t::scale_glyph_extents. */

  भिन्न x1 = x_mult * ft_face->glyph->metrics.horiBearingX;
  भिन्न y1 = y_mult * ft_face->glyph->metrics.horiBearingY;
  भिन्न x2 = x1 + x_mult *  ft_face->glyph->metrics.width;
  भिन्न y2 = y1 + y_mult * -ft_face->glyph->metrics.height;

  द्विगुण rx1 = (द्विगुण) roundf (x1);
  द्विगुण ry1 = (द्विगुण) roundf (y1);
  द्विगुण rx2 = (द्विगुण) roundf (x2);
  द्विगुण ry2 = (द्विगुण) roundf (y2);

  extents->x_bearing = hb_clamp_to<hb_position_t> (rx1);
  extents->y_bearing = hb_clamp_to<hb_position_t> (ry1);
  extents->width = hb_clamp_to<hb_position_t> (rx2 - rx1);
  extents->height = hb_clamp_to<hb_position_t> (ry2 - ry1);

  प्रत्यागम सत्य;
}

स्थिर hb_bool_t
hb_ft_get_glyph_contour_point (hb_font_t *font HB_UNUSED,
			       शून्य *font_data,
			       hb_codepoint_t glyph,
			       unsigned पूर्णाङ्क point_index,
			       hb_position_t *x,
			       hb_position_t *y,
			       शून्य *user_data HB_UNUSED)
{
  ध्रुव hb_ft_font_t *ft_font = (ध्रुव hb_ft_font_t *) font_data;
  _hb_ft_hb_font_check_changed (font, ft_font);

  hb_lock_t lock (ft_font->lock);
  FT_Face ft_face = ft_font->ft_face;

  यदि (unlikely (FT_Load_Glyph (ft_face, glyph, ft_font->load_flags)))
      प्रत्यागम असत्य;

  यदि (unlikely (ft_face->glyph->format != FT_GLYPH_FORMAT_OUTLINE))
      प्रत्यागम असत्य;

  यदि (unlikely (point_index >= (unsigned पूर्णाङ्क) ft_face->glyph->outline.n_points))
      प्रत्यागम असत्य;

  *x = ft_face->glyph->outline.points[point_index].x;
  *y = ft_face->glyph->outline.points[point_index].y;

  प्रत्यागम सत्य;
}

स्थिर hb_bool_t
hb_ft_get_glyph_name (hb_font_t *font HB_UNUSED,
		      शून्य *font_data,
		      hb_codepoint_t glyph,
		      वर्ण *name, unsigned पूर्णाङ्क size,
		      शून्य *user_data HB_UNUSED)
{
  ध्रुव hb_ft_font_t *ft_font = (ध्रुव hb_ft_font_t *) font_data;
  hb_lock_t lock (ft_font->lock);
  FT_Face ft_face = ft_font->ft_face;

  यदि (!size)
  {
    वर्ण buf[128];
    प्रत्यागम !FT_Get_Glyph_Name (ft_face, glyph, buf, परिमाण (buf)) && *buf;
  }

  hb_bool_t ret = !FT_Get_Glyph_Name (ft_face, glyph, name, size);
  यदि (ret && (size && !*name))
    ret = असत्य;

  प्रत्यागम ret;
}

स्थिर hb_bool_t
hb_ft_get_glyph_from_name (hb_font_t *font HB_UNUSED,
			   शून्य *font_data,
			   ध्रुव वर्ण *name, पूर्णाङ्क len, /* -1 means nul-terminated */
			   hb_codepoint_t *glyph,
			   शून्य *user_data HB_UNUSED)
{
  ध्रुव hb_ft_font_t *ft_font = (ध्रुव hb_ft_font_t *) font_data;
  hb_lock_t lock (ft_font->lock);
  FT_Face ft_face = ft_font->ft_face;

  यदि (len < 0)
    *glyph = FT_Get_Name_Index (ft_face, (FT_String *) name);
  अन्यथा {
    /* Make a nul-terminated version. */
    वर्ण buf[128];
    len = hb_min (len, (पूर्णाङ्क) परिमाण (buf) - 1);
    strncpy (buf, name, len);
    buf[len] = '\0';
    *glyph = FT_Get_Name_Index (ft_face, buf);
  }

  यदि (*glyph == 0)
  {
    /* Check whether the given name was actually the name of glyph 0. */
    वर्ण buf[128];
    यदि (!FT_Get_Glyph_Name(ft_face, 0, buf, परिमाण (buf)) &&
	len < 0 ? !strcmp (buf, name) : !strncmp (buf, name, len))
      प्रत्यागम सत्य;
  }

  प्रत्यागम *glyph != 0;
}

स्थिर hb_bool_t
hb_ft_get_font_h_extents (hb_font_t *font HB_UNUSED,
			  शून्य *font_data,
			  hb_font_extents_t *metrics,
			  शून्य *user_data HB_UNUSED)
{
  ध्रुव hb_ft_font_t *ft_font = (ध्रुव hb_ft_font_t *) font_data;
  _hb_ft_hb_font_check_changed (font, ft_font);

  hb_lock_t lock (ft_font->lock);
  FT_Face ft_face = ft_font->ft_face;
  भिन्न y_mult;
#ifdef HAVE_FT_GET_TRANSFORM
  यदि (ft_font->transform)
  {
    FT_Matrix matrix;
    FT_Get_Transform (ft_face, &matrix, nullptr);
    y_mult = sqrtf ((भिन्न)matrix.yx * matrix.yx + (भिन्न)matrix.yy * matrix.yy) / 65536.f;
    y_mult *= font->y_scale < 0 ? -1 : +1;
  }
  अन्यथा
#endif
  {
    y_mult = font->y_scale < 0 ? -1 : +1;
  }

  यदि (ft_face->units_per_EM != 0)
  {
    द्विगुण ascender = FT_MulFix(ft_face->ascender, ft_face->size->metrics.y_scale);
    द्विगुण descender = FT_MulFix(ft_face->descender, ft_face->size->metrics.y_scale);
    द्विगुण height = FT_MulFix( ft_face->height, ft_face->size->metrics.y_scale );
    metrics->ascender = hb_clamp_to<hb_position_t> ((द्विगुण) y_mult * ascender);
    metrics->descender = hb_clamp_to<hb_position_t> ((द्विगुण) y_mult * descender);
    metrics->ओळ_gap = hb_clamp_to<hb_position_t> ((द्विगुण) y_mult * (height - (ascender - descender)));
  }
  अन्यथा
  {
    /* Bitmap-only font, eg. color bitmap font. */
    द्विगुण ascender = ft_face->size->metrics.ascender;
    द्विगुण descender = ft_face->size->metrics.descender;
    द्विगुण height = ft_face->size->metrics.height;
    metrics->ascender = hb_clamp_to<hb_position_t> ((द्विगुण) y_mult * ascender);
    metrics->descender = hb_clamp_to<hb_position_t> ((द्विगुण) y_mult * descender);
    metrics->ओळ_gap = hb_clamp_to<hb_position_t> ((द्विगुण) y_mult * (height - (ascender - descender)));
  }

  प्रत्यागम सत्य;
}

#ifndef HB_NO_DRAW

स्थिर पूर्णाङ्क
_hb_ft_move_to (ध्रुव FT_Vector *to,
		शून्य *arg)
{
  hb_draw_session_t *drawing = (hb_draw_session_t *) arg;
  drawing->move_to (to->x, to->y);
  प्रत्यागम FT_Err_Ok;
}

स्थिर पूर्णाङ्क
hb_ft_ओळ_to (ध्रुव FT_Vector *to,
		शून्य *arg)
{
  hb_draw_session_t *drawing = (hb_draw_session_t *) arg;
  drawing->ओळ_to (to->x, to->y);
  प्रत्यागम FT_Err_Ok;
}

स्थिर पूर्णाङ्क
_hb_ft_conic_to (ध्रुव FT_Vector *control,
		 ध्रुव FT_Vector *to,
		 शून्य *arg)
{
  hb_draw_session_t *drawing = (hb_draw_session_t *) arg;
  drawing->quadratic_to (control->x, control->y,
			 to->x, to->y);
  प्रत्यागम FT_Err_Ok;
}

स्थिर पूर्णाङ्क
_hb_ft_cubic_to (ध्रुव FT_Vector *control1,
		 ध्रुव FT_Vector *control2,
		 ध्रुव FT_Vector *to,
		 शून्य *arg)
{
  hb_draw_session_t *drawing = (hb_draw_session_t *) arg;
  drawing->cubic_to (control1->x, control1->y,
		     control2->x, control2->y,
		     to->x, to->y);
  प्रत्यागम FT_Err_Ok;
}

स्थिर hb_bool_t
hb_ft_draw_glyph_or_fail (hb_font_t *font,
			  शून्य *font_data,
			  hb_codepoint_t glyph,
			  hb_draw_funcs_t *draw_funcs, शून्य *draw_data,
			  शून्य *user_data HB_UNUSED)
{
  ध्रुव hb_ft_font_t *ft_font = (ध्रुव hb_ft_font_t *) font_data;
  _hb_ft_hb_font_check_changed (font, ft_font);

  hb_lock_t lock (ft_font->lock);
  FT_Face ft_face = ft_font->ft_face;

  यदि (unlikely (FT_Load_Glyph (ft_face, glyph,
			       FT_LOAD_NO_BITMAP | ft_font->load_flags)))
    प्रत्यागम असत्य;

  यदि (ft_face->glyph->format != FT_GLYPH_FORMAT_OUTLINE)
    प्रत्यागम असत्य;

  ध्रुव FT_Outline_Funcs outline_funcs = {
    _hb_ft_move_to,
    hb_ft_ओळ_to,
    _hb_ft_conic_to,
    _hb_ft_cubic_to,
    0, /* shift */
    0, /* delta */
  };

  hb_draw_session_t draw_session {draw_funcs, draw_data};

  FT_Outline_Decompose (&ft_face->glyph->outline,
			&outline_funcs,
			&draw_session);

  प्रत्यागम सत्य;
}
#endif

#ifndef HB_NO_PAINT
#यदि (FREETYPE_MAJOR*10000 + FREETYPE_MINOR*100 + FREETYPE_PATCH) >= 21300

#अन्तर्भाव "hb-ft-colr.hh"

स्थिर hb_bool_t
hb_ft_paint_glyph_or_fail (hb_font_t *font,
			   शून्य *font_data,
			   hb_codepoint_t gid,
			   hb_paint_funcs_t *paint_funcs, शून्य *paint_data,
			   unsigned पूर्णाङ्क palette_index,
			   hb_color_t foreground,
			   शून्य *user_data)
{
  ध्रुव hb_ft_font_t *ft_font = (ध्रुव hb_ft_font_t *) font_data;
  _hb_ft_hb_font_check_changed (font, ft_font);

  hb_lock_t lock (ft_font->lock);
  FT_Face ft_face = ft_font->ft_face;

  FT_Long load_flags = ft_font->load_flags | FT_LOAD_COLOR;
#यदि (FREETYPE_MAJOR*10000 + FREETYPE_MINOR*100 + FREETYPE_PATCH) >= 21301
  load_flags |= FT_LOAD_NO_SVG;
#endif

  /* We release the lock before calling into glyph callbacks, such that
   * eg. draw API can call back into the face.*/

  यदि (unlikely (FT_Load_Glyph (ft_face, gid, load_flags)))
    प्रत्यागम असत्य;

  यदि (ft_face->glyph->format == FT_GLYPH_FORMAT_OUTLINE)
  {
    यदि (hb_ft_paint_glyph_colr (font, font_data, gid,
				paint_funcs, paint_data,
				palette_index, foreground,
				user_data))
      प्रत्यागम सत्य;

    // Outline glyph
    प्रत्यागम असत्य;
  }

  auto *glyph = ft_face->glyph;
  यदि (glyph->format == FT_GLYPH_FORMAT_BITMAP)
  {
    तार्किक ret = असत्य;
    auto &bitmap = glyph->bitmap;
    यदि (bitmap.pixel_mode == FT_PIXEL_MODE_BGRA)
    {
      यदि (bitmap.pitch != (signed) bitmap.width * 4)
        प्रत्यागम ret;

      ft_font->lock.unlock ();

      hb_blob_t *blob = hb_blob_create ((ध्रुव वर्ण *) bitmap.buffer,
					bitmap.pitch * bitmap.rows,
					HB_MEMORY_MODE_DUPLICATE,
					nullptr, nullptr);

      hb_glyph_extents_t extents;
      यदि (!font->get_glyph_extents (gid, &extents, असत्य))
	गच्छ out;

      यदि (paint_funcs->image (paint_data,
			      blob,
			      bitmap.width,
			      bitmap.rows,
			      HB_PAINT_IMAGE_FORMAT_BGRA,
			      0.f,
			      &extents))
        ret = सत्य;

    out:
      hb_blob_destroy (blob);
      ft_font->lock.lock ();
    }

    प्रत्यागम ret;
  }
  प्रत्यागम असत्य;
}
#endif
#endif


स्थिर अन्तःस्थ शून्य free_static_ft_funcs ();

स्थिर संरचना hb_ft_font_funcs_lazy_loader_t : hb_font_funcs_lazy_loader_t<hb_ft_font_funcs_lazy_loader_t>
{
  स्थिर hb_font_funcs_t *create ()
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
#यदि (FREETYPE_MAJOR*10000 + FREETYPE_MINOR*100 + FREETYPE_PATCH) >= 21300
    hb_font_funcs_set_paint_glyph_or_fail_func (funcs, hb_ft_paint_glyph_or_fail, nullptr, nullptr);
#endif
#endif

    hb_font_funcs_make_immutable (funcs);

    hb_atexit (free_static_ft_funcs);

    प्रत्यागम funcs;
  }
} static_ft_funcs;

स्थिर अन्तःस्थ
शून्य free_static_ft_funcs ()
{
  static_ft_funcs.free_instance ();
}

स्थिर hb_font_funcs_t *
_hb_ft_get_font_funcs ()
{
  प्रत्यागम static_ft_funcs.get_unconst ();
}

स्थिर शून्य
_hb_ft_font_set_funcs (hb_font_t *font, FT_Face ft_face, तार्किक unref,
		       तार्किक static_library = असत्य)
{
  तार्किक चिन्ह = ft_face->charmap && ft_face->charmap->encoding == FT_ENCODING_MS_चिन्ह;

  hb_ft_font_t *ft_font = _hb_ft_font_create (ft_face, चिन्ह, unref, static_library);
  यदि (unlikely (!ft_font)) प्रत्यागम;

  hb_font_set_funcs (font,
		     _hb_ft_get_font_funcs (),
		     ft_font,
		     _hb_ft_font_destroy);
}


स्थिर hb_blob_t *
_hb_ft_reference_table (hb_face_t *face HB_UNUSED, hb_tag_t tag, शून्य *user_data)
{
  FT_Face ft_face = (FT_Face) user_data;
  FT_Byte *buffer;
  FT_ULong  length = 0;
  FT_Error error;

  /* In new FreeType, a tag value of 1 loads the SFNT table directory. Reject it. */
  यदि (tag == 1)
    प्रत्यागम nullptr;

  /* Note: FreeType like HarfBuzz uses the NONE tag for fetching the entire blob */

  error = FT_Load_Sfnt_Table (ft_face, tag, 0, nullptr, &length);
  यदि (error)
    प्रत्यागम nullptr;

  buffer = (FT_Byte *) hb_malloc (length);
  यदि (!buffer)
    प्रत्यागम nullptr;
  auto buffer_guard = hb_make_scope_guard ([&]() { hb_free (buffer); });

  error = FT_Load_Sfnt_Table (ft_face, tag, 0, buffer, &length);
  यदि (error)
    प्रत्यागम nullptr;

  buffer_guard.release ();
  प्रत्यागम hb_blob_create ((ध्रुव वर्ण *) buffer, length,
			 HB_MEMORY_MODE_WRITABLE,
			 buffer, hb_free);
}

स्थिर unsigned
_hb_ft_get_table_tags (ध्रुव hb_face_t *face HB_UNUSED,
		       unsigned पूर्णाङ्क start_offset,
		       unsigned पूर्णाङ्क *table_मोजणी_2,
		       hb_tag_t *table_tags,
		       शून्य *user_data)
{
  FT_Face ft_face = (FT_Face) user_data;

  FT_ULong population = 0;
  FT_Sfnt_Table_Info (ft_face,
		      0, // table_index; ignored
		      nullptr,
                      &population);

  यदि (!table_मोजणी_2)
    प्रत्यागम population;

  यदि (unlikely (start_offset >= population))
  {
    *table_मोजणी_2 = 0;
    प्रत्यागम population;
  }

  unsigned end_offset = start_offset + *table_मोजणी_2;
  यदि (unlikely (end_offset < start_offset)) /* Overflow. */
  {
    *table_मोजणी_2 = 0;
    प्रत्यागम population;
  }
  end_offset = hb_min (end_offset, (unsigned) population);

  *table_मोजणी_2 = end_offset - start_offset;
  यदि (table_tags)
    प्रति (unsigned i = start_offset; i < end_offset; i++)
    {
      FT_ULong tag = 0, length;
      FT_Sfnt_Table_Info (ft_face, i, &tag, &length);
      table_tags[i - start_offset] = tag;
    }

  प्रत्यागम population;
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

  यदि (!ft_face->stream->पठ) {
    hb_blob_t *blob;

    blob = hb_blob_create ((ध्रुव वर्ण *) ft_face->stream->base,
			   (unsigned पूर्णाङ्क) ft_face->stream->size,
			   HB_MEMORY_MODE_READONLY,
			   ft_face, destroy);
    face = hb_face_create (blob, ft_face->face_index);
    hb_blob_destroy (blob);
  } अन्यथा {
    face = hb_face_create_for_tables (_hb_ft_reference_table, ft_face, destroy);
    hb_face_set_get_table_tags_func (face, _hb_ft_get_table_tags, ft_face, nullptr);
  }

  hb_face_set_index (face, ft_face->face_index);
  hb_face_set_upem (face, ft_face->units_per_EM);

  प्रत्यागम face;
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
  प्रत्यागम hb_ft_face_create (ft_face, _hb_ft_face_destroy);
}

स्थिर शून्य
hb_ft_face_finalize (शून्य *arg)
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
  यदि (unlikely (!ft_face->generic.data || ft_face->generic.finalizer != hb_ft_face_finalize))
  {
    यदि (ft_face->generic.finalizer)
      ft_face->generic.finalizer (ft_face);

    ft_face->generic.data = hb_ft_face_create (ft_face, nullptr);
    ft_face->generic.finalizer = hb_ft_face_finalize;
  }

  प्रत्यागम hb_face_reference ((hb_face_t *) ft_face->generic.data);
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
  _hb_ft_font_set_funcs (font, ft_face, असत्य);
  hb_ft_font_changed (font);
  प्रत्यागम font;
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
शून्य
hb_ft_font_changed (hb_font_t *font)
{
  यदि (font->destroy != (hb_destroy_func_t) _hb_ft_font_destroy)
    प्रत्यागम;

  hb_ft_font_t *ft_font = (hb_ft_font_t *) font->user_data;

  FT_Face ft_face = ft_font->ft_face;

  hb_font_set_scale (font,
		     (पूर्णाङ्क) (((uint64_t) ft_face->size->metrics.x_scale * (uint64_t) ft_face->units_per_EM + (1u<<15)) >> 16),
		     (पूर्णाङ्क) (((uint64_t) ft_face->size->metrics.y_scale * (uint64_t) ft_face->units_per_EM + (1u<<15)) >> 16));
#यदि 0 /* hb-ft works in no-hinting model */
  hb_font_set_ppem (font,
		    ft_face->size->metrics.x_ppem,
		    ft_face->size->metrics.y_ppem);
#endif

#यदि defined(HAVE_FT_GET_VAR_BLEND_COORDINATES) && !defined(HB_NO_VAR)
  FT_MM_Var *mm_var = nullptr;
  यदि (!FT_Get_MM_Var (ft_face, &mm_var))
  {
    FT_Fixed *ft_coords = (FT_Fixed *) hb_calloc (mm_var->अंक_axis, परिमाण (FT_Fixed));
    पूर्णाङ्क *coords = (पूर्णाङ्क *) hb_calloc (mm_var->अंक_axis, परिमाण (पूर्णाङ्क));
    यदि (coords && ft_coords)
    {
      यदि (!FT_Get_Var_Blend_Coordinates (ft_face, mm_var->अंक_axis, ft_coords))
      {
	तार्किक nonzero = असत्य;

	प्रति (unsigned पूर्णाङ्क i = 0; i < mm_var->अंक_axis; ++i)
	 {
	  coords[i] = (ft_coords[i] + 2) >> 2;
	  nonzero = nonzero || coords[i];
	 }

	यदि (nonzero)
	  hb_font_set_var_coords_normalized (font, coords, mm_var->अंक_axis);
	अन्यथा
	  hb_font_set_var_coords_normalized (font, nullptr, 0);
      }
    }
    hb_free (coords);
    hb_free (ft_coords);
#ifdef HAVE_FT_DONE_MM_VAR
    FT_Done_MM_Var (ft_face->glyph->library, mm_var);
#अन्यथा
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
  यदि (font->destroy != (hb_destroy_func_t) _hb_ft_font_destroy)
    प्रत्यागम असत्य;

  hb_ft_font_t *ft_font = (hb_ft_font_t *) font->user_data;

  प्रत्यागम _hb_ft_hb_font_check_changed (font, ft_font);
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
  प्रत्यागम hb_ft_font_create (ft_face, _hb_ft_face_destroy);
}


स्थिर शून्य * _hb_ft_alloc (FT_Memory memory, long size)
{ प्रत्यागम hb_malloc (size); }

स्थिर शून्य _hb_ft_free (FT_Memory memory, शून्य *block)
{ hb_free (block); }

स्थिर शून्य * _hb_ft_realloc (FT_Memory memory, long cur_size, long new_size, शून्य *block)
{ प्रत्यागम hb_realloc (block, new_size); }

स्थिर FT_MemoryRec_ m =
{
  nullptr,
  _hb_ft_alloc,
  _hb_ft_free,
  _hb_ft_realloc
};

स्थिर अन्तःस्थ शून्य free_static_ft_library ();
स्थिर अन्तःस्थ शून्य free_static_ft_library_mutex ();

स्थिर संरचना hb_ft_library_mutex_lazy_loader_t : hb_lazy_loader_t<hb_mutex_t,
								   hb_ft_library_mutex_lazy_loader_t>
{
  स्थिर hb_mutex_t *create ()
  {
    hb_mutex_t *lock = (hb_mutex_t *) hb_calloc (1, परिमाण (hb_mutex_t));
    यदि (unlikely (!lock))
      प्रत्यागम nullptr;

    lock = नूतन (lock) hb_mutex_t;
    hb_atexit (free_static_ft_library_mutex);
    प्रत्यागम lock;
  }
  स्थिर शून्य destroy (hb_mutex_t *lock)
  {
    lock->~hb_mutex_t ();
    hb_free (lock);
  }
  स्थिर hb_mutex_t *get_null ()
  {
    प्रत्यागम nullptr;
  }
} static_ft_library_mutex;

स्थिर संरचना hb_ft_library_lazy_loader_t : hb_lazy_loader_t<hb_remove_pointer<FT_Library>,
							     hb_ft_library_lazy_loader_t>
{
  स्थिर FT_Library create ()
  {
    FT_Library l;
    यदि (FT_New_Library (&m, &l))
      प्रत्यागम nullptr;

    FT_Add_Default_Modules (l);
    FT_Set_Default_Properties (l);

    hb_atexit (free_static_ft_library);

    प्रत्यागम l;
  }
  स्थिर शून्य destroy (FT_Library l)
  {
    FT_Done_Library (l);
  }
  स्थिर FT_Library get_null ()
  {
    प्रत्यागम nullptr;
  }
} static_ft_library;

स्थिर अन्तःस्थ
शून्य free_static_ft_library_mutex ()
{
  static_ft_library_mutex.free_instance ();
}

स्थिर अन्तःस्थ
शून्य free_static_ft_library ()
{
  hb_lock_t lock (static_ft_library_mutex.get_unconst ());
  static_ft_library.free_instance ();
}

स्थिर FT_Library
reference_ft_library ()
{
  hb_mutex_t *mutex = static_ft_library_mutex.get_unconst ();
  यदि (unlikely (!mutex))
    प्रत्यागम nullptr;
  hb_lock_t lock (mutex);
  FT_Library l = static_ft_library.get_unconst ();
  यदि (unlikely (FT_Reference_Library (l)))
  {
    DEBUG_संदेश (FT, l, "FT_Reference_Library() failed");
    प्रत्यागम nullptr;
  }
  प्रत्यागम l;
}

स्थिर hb_user_data_किल्ली_t ft_library_किल्ली = {0};

स्थिर शून्य
_hb_ft_face_destroy_static (शून्य *data)
{
  hb_lock_t lock (static_ft_library_mutex.get_unconst ());
  FT_Done_Face ((FT_Face) data);
}

स्थिर FT_Error
new_ft_face (FT_Library  ft_library,
	     ध्रुव वर्ण *file_name,
	     unsigned पूर्णाङ्क index,
	     FT_Face     *ft_face)
{
  hb_lock_t lock (static_ft_library_mutex.get_unconst ());
  प्रत्यागम FT_New_Face (ft_library, file_name, index, ft_face);
}

स्थिर FT_Error
new_ft_memory_face (FT_Library    ft_library,
		    ध्रुव FT_Byte *blob_data,
		    unsigned पूर्णाङ्क   blob_size,
		    unsigned पूर्णाङ्क   index,
		    FT_Face       *ft_face)
{
  hb_lock_t lock (static_ft_library_mutex.get_unconst ());
  प्रत्यागम FT_New_Memory_Face (ft_library, blob_data, blob_size, index, ft_face);
}

स्थिर शून्य
finalize_ft_library (शून्य *arg)
{
  FT_Face ft_face = (FT_Face) arg;
  FT_Done_Library ((FT_Library) ft_face->generic.data);
}

स्थिर शून्य
destroy_ft_library (शून्य *arg)
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
hb_ft_face_create_from_file_or_fail (ध्रुव वर्ण   *file_name,
				     unsigned पूर्णाङ्क  index)
{
  FT_Library ft_library = reference_ft_library ();
  यदि (unlikely (!ft_library))
  {
    DEBUG_संदेश (FT, ft_library, "reference_ft_library failed");
    प्रत्यागम nullptr;
  }

  FT_Face ft_face;
  यदि (unlikely (new_ft_face (ft_library,
			     file_name,
			     index,
			     &ft_face)))
  {
    destroy_ft_library (ft_library);
    प्रत्यागम nullptr;
  }

  FT_Reference_Face (ft_face);
  hb_face_t *face = hb_ft_face_create (ft_face, _hb_ft_face_destroy_static);
  _hb_ft_face_destroy_static (ft_face);

  ft_face->generic.data = ft_library;
  ft_face->generic.finalizer = finalize_ft_library;

  यदि (hb_face_is_immutable (face))
    प्रत्यागम nullptr;

  प्रत्यागम face;
}

स्थिर hb_user_data_किल्ली_t ft_blob_किल्ली = {0};

स्थिर शून्य
_destroy_blob (शून्य *p)
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
				     unsigned पूर्णाङ्क  index)
{
  FT_Library ft_library = reference_ft_library ();
  यदि (unlikely (!ft_library))
  {
    DEBUG_संदेश (FT, ft_library, "reference_ft_library failed");
    प्रत्यागम nullptr;
  }

  hb_blob_make_immutable (blob);
  unsigned blob_size;
  ध्रुव वर्ण *blob_data = hb_blob_get_data (blob, &blob_size);

  FT_Face ft_face;
  यदि (unlikely (new_ft_memory_face (ft_library,
				    (ध्रुव FT_Byte *) blob_data,
				    blob_size,
				    index,
				    &ft_face)))
  {
    destroy_ft_library (ft_library);
    प्रत्यागम nullptr;
  }

  FT_Reference_Face (ft_face);
  hb_face_t *face = hb_ft_face_create (ft_face, _hb_ft_face_destroy_static);
  _hb_ft_face_destroy_static (ft_face);

  ft_face->generic.data = ft_library;
  ft_face->generic.finalizer = finalize_ft_library;

  यदि (hb_face_is_immutable (face))
    प्रत्यागम nullptr;

  // Hook the blob to the hb_face_t, since FT_Face still needs it.
  hb_blob_reference (blob);
  यदि (!hb_face_set_user_data (face, &ft_blob_किल्ली, blob, _destroy_blob, सत्य))
  {
    hb_blob_destroy (blob);
    hb_face_destroy (face);
    प्रत्यागम nullptr;
  }

  प्रत्यागम face;
}

स्थिर शून्य
_release_blob (शून्य *arg)
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
शून्य
hb_ft_font_set_funcs (hb_font_t *font)
{
  पूर्णाङ्क load_flags = FT_LOAD_DEFAULT | FT_LOAD_NO_HINTING;
  यदि (font->destroy == (hb_destroy_func_t) _hb_ft_font_destroy && font->user_data)
  {
    ध्रुव hb_ft_font_t *existing_ft_font = (ध्रुव hb_ft_font_t *) font->user_data;
    load_flags = existing_ft_font->load_flags;
  }

  // In case of failure...
  hb_font_set_funcs (font,
		     hb_font_funcs_get_empty (),
		     nullptr, nullptr);

  hb_blob_t *blob = hb_face_reference_blob (font->face);
  unsigned पूर्णाङ्क blob_length;
  ध्रुव वर्ण *blob_data = hb_blob_get_data (blob, &blob_length);
  यदि (unlikely (!blob_length))
    DEBUG_संदेश (FT, font, "Font face has empty blob");

  FT_Library ft_library = reference_ft_library ();
  यदि (unlikely (!ft_library))
  {
    hb_blob_destroy (blob);
    DEBUG_संदेश (FT, font, "reference_ft_library failed");
    प्रत्यागम;
  }

  FT_Face ft_face = nullptr;
  यदि (unlikely (new_ft_memory_face (ft_library,
				    (ध्रुव FT_Byte *) blob_data,
				    blob_length,
				    hb_face_get_index (font->face),
				    &ft_face)))
  {
    hb_blob_destroy (blob);
    destroy_ft_library (ft_library);
    DEBUG_संदेश (FT, font, "FT_New_Memory_Face() failed");
    प्रत्यागम;
  }

  यदि (FT_Select_Charmap (ft_face, FT_ENCODING_MS_चिन्ह))
    FT_Select_Charmap (ft_face, FT_ENCODING_UNICODE);

  // Hook the blob to the FT_Face
  ft_face->generic.data = blob;
  ft_face->generic.finalizer = _release_blob;

  // And the FT_Library to the blob
  यदि (unlikely (!hb_blob_set_user_data (blob, &ft_library_किल्ली, ft_library, destroy_ft_library, सत्य)))
  {
    DEBUG_संदेश (FT, font, "hb_blob_set_user_data() failed");
    _hb_ft_face_destroy_static (ft_face);
    destroy_ft_library (ft_library);
    प्रत्यागम;
  }

  _hb_ft_font_set_funcs (font, ft_face, सत्य, सत्य);
  hb_ft_font_set_load_flags (font, load_flags);

  _hb_ft_hb_font_changed (font, ft_face);
}

#endif
