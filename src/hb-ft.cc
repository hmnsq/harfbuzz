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

#অন্তর্ভাব "hb.hh"

#ifdef HAVE_FREETYPE

#অন্তর্ভাব "hb-ft.h"

#অন্তর্ভাব "hb-cache.hh"
#অন্তর্ভাব "hb-draw.hh"
#অন্তর্ভাব "hb-font.hh"
#অন্তর্ভাব "hb-machinery.hh"
#অন্তর্ভাব "hb-ot-os2-table.hh"
#অন্তর্ভাব "hb-ot-shaper-arabic-pua.hh"
#অন্তর্ভাব "hb-paint.hh"

#অন্তর্ভাব FT_MODULE_H
#অন্তর্ভাব FT_ADVANCES_H
#অন্তর্ভাব FT_MULTIPLE_MASTERS_H
#অন্তর্ভাব FT_OUTLINE_H
#অন্তর্ভাব FT_TRUETYPE_TABLES_H
#অন্তর্ভাব FT_SYNTHESIS_H
#যদি (FREETYPE_MAJOR*10000 + FREETYPE_MINOR*100 + FREETYPE_PATCH) >= 21300
#অন্তর্ভাব FT_COLOR_H
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


using hb_ft_advance_cache_t = hb_cache_t<16, 24, 8, মিথ্যা>;

স্থির শূন্য _hb_ft_face_destroy_static (শূন্য *তথ্য);

কাঠামো hb_ft_font_t
{
  পূর্ণসংখ্যা load_flags;
  যৌক্তিক প্রতীক; /* Whether selected cmap is symbol cmap. */
  যৌক্তিক unref; /* Whether to destroy ft_face when done. */
  যৌক্তিক transform; /* Whether to apply FT_Face's transform. */
  যৌক্তিক static_library; /* Whether ft_face uses static_ft_library. */

  mutable hb_mutex_t lock; /* Protects members below. */
  FT_Face ft_face;
  mutable hb_atomic_t<unsigned> cached_serial;
  mutable hb_ft_advance_cache_t advance_cache;
};

স্থির hb_ft_font_t *
_hb_ft_font_create (FT_Face ft_face, যৌক্তিক প্রতীক, যৌক্তিক unref, যৌক্তিক static_library)
{
  hb_ft_font_t *ft_font = (hb_ft_font_t *) hb_calloc (1, আকার (hb_ft_font_t));
  যদি (unlikely (!ft_font)) ফেরত nullptr;

  ft_font->lock.init ();
  ft_font->ft_face = ft_face;
  ft_font->প্রতীক = প্রতীক;
  ft_font->unref = unref;
  ft_font->static_library = static_library;

  ft_font->load_flags = FT_LOAD_DEFAULT | FT_LOAD_NO_HINTING;

  ft_font->cached_serial = UINT_MAX;
  নতুন (&ft_font->advance_cache) hb_ft_advance_cache_t;

  ফেরত ft_font;
}

স্থির শূন্য
_hb_ft_face_destroy (শূন্য *তথ্য)
{
  FT_Done_Face ((FT_Face) তথ্য);
}

স্থির শূন্য
_hb_ft_font_destroy (শূন্য *তথ্য)
{
  hb_ft_font_t *ft_font = (hb_ft_font_t *) তথ্য;

  যদি (ft_font->unref)
  {
    যদি (ft_font->static_library)
    {
      /* ft_font->ft_face->generic.data is the blob that hb_ft_font_set_funcs()
       * hooked up as this internal FT_Face's finalizer target (_release_blob).
       * FT_Done_Face() below runs that finalizer while static_ft_library_mutex
       * is held; if dropping its reference were to free the blob right there,
       * the blob's attached destroy_ft_library() user-data callback would try
       * to re-lock the same non-recursive mutex and self-deadlock. Hold an
       * extra reference across the locked call so the blob (and thus that
       * callback) can only be freed after the lock is released. */
      hb_blob_t *blob = hb_blob_reference ((hb_blob_t *) ft_font->ft_face->generic.তথ্য);
      _hb_ft_face_destroy_static (ft_font->ft_face);
      hb_blob_destroy (blob);
    }
    অন্যথা
      _hb_ft_face_destroy (ft_font->ft_face);
  }

  ft_font->lock.fini ();

  hb_free (ft_font);
}


/* hb_font changed, update FT_Face. */
স্থির শূন্য _hb_ft_hb_font_changed (hb_font_t *font, FT_Face ft_face)
{
  যদি (unlikely (font->destroy != (hb_destroy_func_t) _hb_ft_font_destroy))
    ফেরত;

  hb_ft_font_t *ft_font = (hb_ft_font_t *) font->user_তথ্য;

  দশমিক x_mult = 1.f, y_mult = 1.f;

  যদি (font->x_scale < 0) x_mult = -x_mult;
  যদি (font->y_scale < 0) y_mult = -y_mult;

  যদি (FT_Set_Char_আকার (ft_face,
			abs (font->x_scale), abs (font->y_scale),
			0, 0
#যদি 0
		    font->x_ppem * 72 * 64 / font->x_scale,
		    font->y_ppem * 72 * 64 / font->y_scale
#endif
     ) && ft_face->অঙ্ক_fixed_sizes)
  {
#ifdef HAVE_FT_GET_TRANSFORM
    /* Bitmap font, eg. bitmap color emoji. */
    /* Pick largest size? */
    পূর্ণসংখ্যা x_scale  = ft_face->available_sizes[ft_face->অঙ্ক_fixed_sizes - 1].x_ppem;
    পূর্ণসংখ্যা y_scale = ft_face->available_sizes[ft_face->অঙ্ক_fixed_sizes - 1].y_ppem;
    FT_Set_Char_আকার (ft_face,
		      x_scale, y_scale,
		      0, 0);

    /* This contains the sign that was previously in x_mult/y_mult. */
    x_mult = (দশমিক) font->x_scale / x_scale;
    y_mult = (দশমিক) font->y_scale / y_scale;
#endif
  }
  অন্যথা
  { /* Shrug */ }


  যদি (x_mult != 1.f || y_mult != 1.f)
  {
    FT_Matrix matrix = { (পূর্ণসংখ্যা) roundf (x_mult * (1<<16)), 0,
			  0, (পূর্ণসংখ্যা) roundf (y_mult * (1<<16))};
    FT_Set_Transform (ft_face, &matrix, nullptr);
    ft_font->transform = সত্য;
  }
  অন্যথা
    FT_Set_Transform (ft_face, nullptr, nullptr);

#যদি defined(HAVE_FT_GET_VAR_BLEND_COORDINATES) && !defined(HB_NO_VAR)
  যদি (font->has_nonzero_coords)
  {
    unsigned পূর্ণসংখ্যা অঙ্ক_coords;
    ধ্রুবক দশমিক *coords = hb_font_get_var_coords_design (font, &অঙ্ক_coords);
    FT_Fixed *ft_coords = (FT_Fixed *) hb_calloc (অঙ্ক_coords, আকার (FT_Fixed));
    যদি (ft_coords)
    {
      জন্য (unsigned পূর্ণসংখ্যা i = 0; i < অঙ্ক_coords; i++)
	  ft_coords[i] = coords[i] * 65536.f;
      FT_Set_Var_Design_Coordinates (ft_face, অঙ্ক_coords, ft_coords);
      hb_free (ft_coords);
    }
  }
  অন্যথা যদি (font->অঙ্ক_coords)
  {
    // Some old versions of FreeType crash if we
    // call this function on non-variable fonts.
    FT_Set_Var_Design_Coordinates (ft_face, 0, nullptr);
  }
#endif
}

/* Check if hb_font changed, update FT_Face. */
স্থির অন্তঃরেখ যৌক্তিক
_hb_ft_hb_font_check_changed (hb_font_t *font,
			      ধ্রুবক hb_ft_font_t *ft_font)
{
  যদি (font->serial != ft_font->cached_serial)
  {
    hb_lock_t lock (ft_font->lock);
    _hb_ft_hb_font_changed (font, ft_font->ft_face);
    ft_font->advance_cache.clear ();
    ft_font->cached_serial.set_release (font->serial.get_acquire ());
    ফেরত সত্য;
  }
  ফেরত মিথ্যা;
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
শূন্য
hb_ft_font_set_load_flags (hb_font_t *font, পূর্ণসংখ্যা load_flags)
{
  যদি (hb_object_is_immutable (font))
    ফেরত;

  যদি (unlikely (font->destroy != (hb_destroy_func_t) _hb_ft_font_destroy))
    ফেরত;

  hb_ft_font_t *ft_font = (hb_ft_font_t *) font->user_তথ্য;

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
পূর্ণসংখ্যা
hb_ft_font_get_load_flags (hb_font_t *font)
{
  যদি (unlikely (font->destroy != (hb_destroy_func_t) _hb_ft_font_destroy))
    ফেরত 0;

  ধ্রুবক hb_ft_font_t *ft_font = (ধ্রুবক hb_ft_font_t *) font->user_তথ্য;

  ফেরত ft_font->load_flags;
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
  যদি (unlikely (font->destroy != (hb_destroy_func_t) _hb_ft_font_destroy))
    ফেরত nullptr;

  ধ্রুবক hb_ft_font_t *ft_font = (ধ্রুবক hb_ft_font_t *) font->user_তথ্য;

  ফেরত ft_font->ft_face;
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
  ফেরত hb_ft_font_get_ft_face (font);
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
  যদি (unlikely (font->destroy != (hb_destroy_func_t) _hb_ft_font_destroy))
    ফেরত nullptr;

  ধ্রুবক hb_ft_font_t *ft_font = (ধ্রুবক hb_ft_font_t *) font->user_তথ্য;

  ft_font->lock.lock ();

  ফেরত ft_font->ft_face;
}

/**
 * hb_ft_font_unlock_face: (skip)
 * @font: #hb_font_t to work upon
 *
 * Releases an FT_Face previously obtained with hb_ft_font_lock_face().
 *
 * Since: 2.6.5
 **/
শূন্য
hb_ft_font_unlock_face (hb_font_t *font)
{
  যদি (unlikely (font->destroy != (hb_destroy_func_t) _hb_ft_font_destroy))
    ফেরত;

  ধ্রুবক hb_ft_font_t *ft_font = (ধ্রুবক hb_ft_font_t *) font->user_তথ্য;

  ft_font->lock.unlock ();
}


স্থির hb_bool_t
hb_ft_get_nominal_glyph (hb_font_t *font,
			 শূন্য *font_তথ্য,
			 hb_codepoint_t unicode,
			 hb_codepoint_t *glyph,
			 শূন্য *user_তথ্য HB_UNUSED)
{
  ধ্রুবক hb_ft_font_t *ft_font = (ধ্রুবক hb_ft_font_t *) font_তথ্য;
  hb_lock_t lock (ft_font->lock);
  unsigned পূর্ণসংখ্যা g = FT_Get_Char_সূচক (ft_font->ft_face, unicode);

  যদি (unlikely (!g))
  {
    যদি (unlikely (ft_font->প্রতীক))
    {
      নির্বাচন ((unsigned) font->face->table.OS2->get_font_page ()) {
      ক্ষেত্র OT::OS2::font_page_t::FONT_PAGE_NONE:
	যদি (unicode <= 0x00FFu)
	  /* For symbol-encoded OpenType fonts, we duplicate the
	   * U+F000..F0FF range at U+0000..U+00FF.  That's what
	   * Windows seems to do, and that's hinted about at:
	   * https://docs.microsoft.com/en-us/typography/opentype/spec/recom
	   * under "Non-Standard (Symbol) Fonts". */
	  g = FT_Get_Char_সূচক (ft_font->ft_face, 0xF000u + unicode);
	বিরতি;
#ifndef HB_NO_OT_SHAPER_ARABIC_FALLBACK
      ক্ষেত্র OT::OS2::font_page_t::FONT_PAGE_SIMP_ARABIC:
	g = FT_Get_Char_সূচক (ft_font->ft_face, _hb_arabic_pua_simp_map (unicode));
	বিরতি;
      ক্ষেত্র OT::OS2::font_page_t::FONT_PAGE_TRAD_ARABIC:
	g = FT_Get_Char_সূচক (ft_font->ft_face, _hb_arabic_pua_trad_map (unicode));
	বিরতি;
#endif
      পূর্বনির্ধারিত:
	বিরতি;
      }
      যদি (!g)
	ফেরত মিথ্যা;
    }
    অন্যথা
      ফেরত মিথ্যা;
  }

  *glyph = g;
  ফেরত সত্য;
}

স্থির unsigned পূর্ণসংখ্যা
hb_ft_get_nominal_glyphs (hb_font_t *font HB_UNUSED,
			  শূন্য *font_তথ্য,
			  unsigned পূর্ণসংখ্যা গুনতি,
			  ধ্রুবক hb_codepoint_t *first_unicode,
			  unsigned পূর্ণসংখ্যা unicode_stride,
			  hb_codepoint_t *first_glyph,
			  unsigned পূর্ণসংখ্যা glyph_stride,
			  শূন্য *user_তথ্য HB_UNUSED)
{
  ধ্রুবক hb_ft_font_t *ft_font = (ধ্রুবক hb_ft_font_t *) font_তথ্য;
  hb_lock_t lock (ft_font->lock);
  unsigned পূর্ণসংখ্যা done;
  জন্য (done = 0;
       done < গুনতি && (*first_glyph = FT_Get_Char_সূচক (ft_font->ft_face, *first_unicode));
       done++)
  {
    first_unicode = &StructAtOffsetUnaligned<hb_codepoint_t> (first_unicode, unicode_stride);
    first_glyph = &StructAtOffsetUnaligned<hb_codepoint_t> (first_glyph, glyph_stride);
  }
  /* We don't need to do ft_font->symbol dance here, since HB calls the singular
   * nominal_glyph() for what we don't handle here. */
  ফেরত done;
}


স্থির hb_bool_t
hb_ft_get_variation_glyph (hb_font_t *font HB_UNUSED,
			   শূন্য *font_তথ্য,
			   hb_codepoint_t unicode,
			   hb_codepoint_t variation_selector,
			   hb_codepoint_t *glyph,
			   শূন্য *user_তথ্য HB_UNUSED)
{
  ধ্রুবক hb_ft_font_t *ft_font = (ধ্রুবক hb_ft_font_t *) font_তথ্য;
  hb_lock_t lock (ft_font->lock);
  unsigned পূর্ণসংখ্যা g = FT_Face_Get_Char_Variant_সূচক (ft_font->ft_face, unicode, variation_selector);

  যদি (unlikely (!g))
    ফেরত মিথ্যা;

  *glyph = g;
  ফেরত সত্য;
}

স্থির শূন্য
hb_ft_get_glyph_h_advances (hb_font_t* font, শূন্য* font_তথ্য,
			    unsigned গুনতি,
			    ধ্রুবক hb_codepoint_t *first_glyph,
			    unsigned glyph_stride,
			    hb_position_t *first_advance,
			    unsigned advance_stride,
			    শূন্য *user_তথ্য HB_UNUSED)
{
  ধ্রুবক hb_ft_font_t *ft_font = (ধ্রুবক hb_ft_font_t *) font_তথ্য;
  _hb_ft_hb_font_check_changed (font, ft_font);

  hb_lock_t lock (ft_font->lock);
  FT_Face ft_face = ft_font->ft_face;
  পূর্ণসংখ্যা load_flags = ft_font->load_flags;
  দশমিক x_mult;
#ifdef HAVE_FT_GET_TRANSFORM
  যদি (ft_font->transform)
  {
    FT_Matrix matrix;
    FT_Get_Transform (ft_face, &matrix, nullptr);
    x_mult = sqrtf ((দশমিক)matrix.xx * matrix.xx + (দশমিক)matrix.xy * matrix.xy) / 65536.f;
    x_mult *= font->x_scale < 0 ? -1 : +1;
  }
  অন্যথা
#endif
  {
    x_mult = font->x_scale < 0 ? -1 : +1;
  }

  জন্য (unsigned পূর্ণসংখ্যা i = 0; i < গুনতি; i++)
  {
    hb_position_t advance;
    hb_codepoint_t glyph = *first_glyph;

    unsigned পূর্ণসংখ্যা cv;
    যদি (ft_font->advance_cache.get (glyph, &cv))
      advance = (hb_position_t) cv;
    অন্যথা
    {
      FT_Fixed v = 0;
      FT_Get_Advance (ft_face, glyph, load_flags, &v);
      /* Work around bug that FreeType seems to return negative advance
       * for variable-set fonts if x_scale is negative! */
      দ্বিগুণ scaled = trunc (fabs ((দ্বিগুণ) v) * (দ্বিগুণ) x_mult + (1<<9)) / (1<<10);
      advance = hb_clamp_to<hb_position_t> (floor (scaled));
      ft_font->advance_cache.set (glyph, advance);
    }

    *first_advance = advance;
    first_glyph = &StructAtOffsetUnaligned<hb_codepoint_t> (first_glyph, glyph_stride);
    first_advance = &StructAtOffsetUnaligned<hb_position_t> (first_advance, advance_stride);
  }
}

#ifndef HB_NO_VERTICAL
স্থির hb_position_t
hb_ft_get_glyph_v_advance (hb_font_t *font,
			   শূন্য *font_তথ্য,
			   hb_codepoint_t glyph,
			   শূন্য *user_তথ্য HB_UNUSED)
{
  ধ্রুবক hb_ft_font_t *ft_font = (ধ্রুবক hb_ft_font_t *) font_তথ্য;
  _hb_ft_hb_font_check_changed (font, ft_font);

  hb_lock_t lock (ft_font->lock);
  FT_Fixed v;
  দশমিক y_mult;
#ifdef HAVE_FT_GET_TRANSFORM
  যদি (ft_font->transform)
  {
    FT_Matrix matrix;
    FT_Get_Transform (ft_font->ft_face, &matrix, nullptr);
    y_mult = sqrtf ((দশমিক)matrix.yx * matrix.yx + (দশমিক)matrix.yy * matrix.yy) / 65536.f;
    y_mult *= font->y_scale < 0 ? -1 : +1;
  }
  অন্যথা
#endif
  {
    y_mult = font->y_scale < 0 ? -1 : +1;
  }

  যদি (unlikely (FT_Get_Advance (ft_font->ft_face, glyph, ft_font->load_flags | FT_LOAD_VERTICAL_LAYOUT, &v)))
    ফেরত 0;

  /* Note: FreeType's vertical metrics grows downward while other FreeType coordinates
   * have a Y growing upward.  Hence the extra negation. */
  দ্বিগুণ advance = floor ((-(দ্বিগুণ) v + (1<<9)) / (1<<10));

  ফেরত hb_clamp_to<hb_position_t> ((দ্বিগুণ) y_mult * advance);
}
#endif

#ifndef HB_NO_VERTICAL
স্থির hb_bool_t
hb_ft_get_glyph_v_origin (hb_font_t *font,
			  শূন্য *font_তথ্য,
			  hb_codepoint_t glyph,
			  hb_position_t *x,
			  hb_position_t *y,
			  শূন্য *user_তথ্য HB_UNUSED)
{
  ধ্রুবক hb_ft_font_t *ft_font = (ধ্রুবক hb_ft_font_t *) font_তথ্য;
  _hb_ft_hb_font_check_changed (font, ft_font);

  hb_lock_t lock (ft_font->lock);
  FT_Face ft_face = ft_font->ft_face;
  দশমিক x_mult, y_mult;
#ifdef HAVE_FT_GET_TRANSFORM
  যদি (ft_font->transform)
  {
    FT_Matrix matrix;
    FT_Get_Transform (ft_face, &matrix, nullptr);
    x_mult = sqrtf ((দশমিক)matrix.xx * matrix.xx + (দশমিক)matrix.xy * matrix.xy) / 65536.f;
    x_mult *= font->x_scale < 0 ? -1 : +1;
    y_mult = sqrtf ((দশমিক)matrix.yx * matrix.yx + (দশমিক)matrix.yy * matrix.yy) / 65536.f;
    y_mult *= font->y_scale < 0 ? -1 : +1;
  }
  অন্যথা
#endif
  {
    x_mult = font->x_scale < 0 ? -1 : +1;
    y_mult = font->y_scale < 0 ? -1 : +1;
  }

  যদি (unlikely (FT_Load_Glyph (ft_face, glyph, ft_font->load_flags)))
    ফেরত মিথ্যা;

  /* Note: FreeType's vertical metrics grows downward while other FreeType coordinates
   * have a Y growing upward.  Hence the extra negation. */
  *x = hb_clamp_to<hb_position_t> ((দ্বিগুণ) x_mult *
				   ((দ্বিগুণ) ft_face->glyph->metrics.horiBearingX -
				    ft_face->glyph->metrics.vertBearingX));
  *y = hb_clamp_to<hb_position_t> ((দ্বিগুণ) y_mult *
				   ((দ্বিগুণ) ft_face->glyph->metrics.horiBearingY +
				    ft_face->glyph->metrics.vertBearingY));

  ফেরত সত্য;
}
#endif

#ifndef HB_NO_OT_SHAPE_FALLBACK
স্থির hb_position_t
hb_ft_get_glyph_h_kerning (hb_font_t *font,
			   শূন্য *font_তথ্য,
			   hb_codepoint_t বাম_glyph,
			   hb_codepoint_t ডান_glyph,
			   শূন্য *user_তথ্য HB_UNUSED)
{
  ধ্রুবক hb_ft_font_t *ft_font = (ধ্রুবক hb_ft_font_t *) font_তথ্য;
  _hb_ft_hb_font_check_changed (font, ft_font);

  hb_lock_t lock (ft_font->lock);
  FT_Vector kerningv;

  FT_Kerning_Mode mode = font->x_ppem ? FT_KERNING_DEFAULT : FT_KERNING_UNFITTED;
  যদি (FT_Get_Kerning (ft_font->ft_face, বাম_glyph, ডান_glyph, mode, &kerningv))
    ফেরত 0;

  ফেরত hb_clamp_to<hb_position_t> ((int64_t) kerningv.x);
}
#endif

স্থির যৌক্তিক
hb_ft_is_colr_glyph (hb_font_t *font,
		     শূন্য *font_তথ্য,
		     hb_codepoint_t gid)
{
#ifndef HB_NO_PAINT
#যদি (FREETYPE_MAJOR*10000 + FREETYPE_MINOR*100 + FREETYPE_PATCH) >= 21300
  ধ্রুবক hb_ft_font_t *ft_font = (ধ্রুবক hb_ft_font_t *) font_তথ্য;
  FT_Face ft_face = ft_font->ft_face;


  /* COLRv1 */
  FT_OpaquePaint paint = {0};
  যদি (FT_Get_Color_Glyph_Paint (ft_face, gid,
			        FT_COLOR_NO_মূল_TRANSFORM,
			        &paint))
    ফেরত সত্য;

  /* COLRv0 */
  FT_LayerIterator  iterator;
  FT_UInt  layer_glyph_সূচক;
  FT_UInt  layer_color_সূচক;
  iterator.p  = ফাঁকা;
  যদি (FT_Get_Color_Glyph_Layer (ft_face,
				gid,
				&layer_glyph_সূচক,
				&layer_color_সূচক,
				&iterator))
    ফেরত সত্য;
#endif
#endif

  ফেরত মিথ্যা;
}

স্থির hb_bool_t
hb_ft_get_glyph_extents (hb_font_t *font,
			 শূন্য *font_তথ্য,
			 hb_codepoint_t glyph,
			 hb_glyph_extents_t *extents,
			 শূন্য *user_তথ্য HB_UNUSED)
{
  // FreeType doesn't return COLR glyph extents.
  যদি (hb_ft_is_colr_glyph (font, font_তথ্য, glyph))
    ফেরত মিথ্যা;

  ধ্রুবক hb_ft_font_t *ft_font = (ধ্রুবক hb_ft_font_t *) font_তথ্য;
  _hb_ft_hb_font_check_changed (font, ft_font);

  hb_lock_t lock (ft_font->lock);
  FT_Face ft_face = ft_font->ft_face;
  দশমিক x_mult, y_mult;

#ifdef HAVE_FT_GET_TRANSFORM
  যদি (ft_font->transform)
  {
    FT_Matrix matrix;
    FT_Get_Transform (ft_face, &matrix, nullptr);
    x_mult = sqrtf ((দশমিক)matrix.xx * matrix.xx + (দশমিক)matrix.xy * matrix.xy) / 65536.f;
    x_mult *= font->x_scale < 0 ? -1 : +1;
    y_mult = sqrtf ((দশমিক)matrix.yx * matrix.yx + (দশমিক)matrix.yy * matrix.yy) / 65536.f;
    y_mult *= font->y_scale < 0 ? -1 : +1;
  }
  অন্যথা
#endif
  {
    x_mult = font->x_scale < 0 ? -1 : +1;
    y_mult = font->y_scale < 0 ? -1 : +1;
  }

  যদি (unlikely (FT_Load_Glyph (ft_face, glyph, ft_font->load_flags)))
    ফেরত মিথ্যা;

  /* Copied from hb_font_t::scale_glyph_extents. */

  দশমিক x1 = x_mult * ft_face->glyph->metrics.horiBearingX;
  দশমিক y1 = y_mult * ft_face->glyph->metrics.horiBearingY;
  দশমিক x2 = x1 + x_mult *  ft_face->glyph->metrics.width;
  দশমিক y2 = y1 + y_mult * -ft_face->glyph->metrics.height;

  দ্বিগুণ rx1 = (দ্বিগুণ) roundf (x1);
  দ্বিগুণ ry1 = (দ্বিগুণ) roundf (y1);
  দ্বিগুণ rx2 = (দ্বিগুণ) roundf (x2);
  দ্বিগুণ ry2 = (দ্বিগুণ) roundf (y2);

  extents->x_bearing = hb_clamp_to<hb_position_t> (rx1);
  extents->y_bearing = hb_clamp_to<hb_position_t> (ry1);
  extents->width = hb_clamp_to<hb_position_t> (rx2 - rx1);
  extents->height = hb_clamp_to<hb_position_t> (ry2 - ry1);

  ফেরত সত্য;
}

স্থির hb_bool_t
hb_ft_get_glyph_contour_point (hb_font_t *font HB_UNUSED,
			       শূন্য *font_তথ্য,
			       hb_codepoint_t glyph,
			       unsigned পূর্ণসংখ্যা point_সূচক,
			       hb_position_t *x,
			       hb_position_t *y,
			       শূন্য *user_তথ্য HB_UNUSED)
{
  ধ্রুবক hb_ft_font_t *ft_font = (ধ্রুবক hb_ft_font_t *) font_তথ্য;
  _hb_ft_hb_font_check_changed (font, ft_font);

  hb_lock_t lock (ft_font->lock);
  FT_Face ft_face = ft_font->ft_face;

  যদি (unlikely (FT_Load_Glyph (ft_face, glyph, ft_font->load_flags)))
      ফেরত মিথ্যা;

  যদি (unlikely (ft_face->glyph->format != FT_GLYPH_FORMAT_OUTLINE))
      ফেরত মিথ্যা;

  যদি (unlikely (point_সূচক >= (unsigned পূর্ণসংখ্যা) ft_face->glyph->outline.n_points))
      ফেরত মিথ্যা;

  *x = ft_face->glyph->outline.points[point_সূচক].x;
  *y = ft_face->glyph->outline.points[point_সূচক].y;

  ফেরত সত্য;
}

স্থির hb_bool_t
hb_ft_get_glyph_name (hb_font_t *font HB_UNUSED,
		      শূন্য *font_তথ্য,
		      hb_codepoint_t glyph,
		      অক্ষর *name, unsigned পূর্ণসংখ্যা আকার_2,
		      শূন্য *user_তথ্য HB_UNUSED)
{
  ধ্রুবক hb_ft_font_t *ft_font = (ধ্রুবক hb_ft_font_t *) font_তথ্য;
  hb_lock_t lock (ft_font->lock);
  FT_Face ft_face = ft_font->ft_face;

  যদি (!আকার_2)
  {
    অক্ষর buf[128];
    ফেরত !FT_Get_Glyph_Name (ft_face, glyph, buf, আকার (buf)) && *buf;
  }

  hb_bool_t ret = !FT_Get_Glyph_Name (ft_face, glyph, name, আকার_2);
  যদি (ret && (আকার_2 && !*name))
    ret = মিথ্যা;

  ফেরত ret;
}

স্থির hb_bool_t
hb_ft_get_glyph_from_name (hb_font_t *font HB_UNUSED,
			   শূন্য *font_তথ্য,
			   ধ্রুবক অক্ষর *name, পূর্ণসংখ্যা len, /* -1 means nul-terminated */
			   hb_codepoint_t *glyph,
			   শূন্য *user_তথ্য HB_UNUSED)
{
  ধ্রুবক hb_ft_font_t *ft_font = (ধ্রুবক hb_ft_font_t *) font_তথ্য;
  hb_lock_t lock (ft_font->lock);
  FT_Face ft_face = ft_font->ft_face;

  যদি (len < 0)
    *glyph = FT_Get_Name_সূচক (ft_face, (FT_String *) name);
  অন্যথা {
    /* Make a nul-terminated version. */
    অক্ষর buf[128];
    len = hb_min (len, (পূর্ণসংখ্যা) আকার (buf) - 1);
    strncpy (buf, name, len);
    buf[len] = '\0';
    *glyph = FT_Get_Name_সূচক (ft_face, buf);
  }

  যদি (*glyph == 0)
  {
    /* Check whether the given name was actually the name of glyph 0. */
    অক্ষর buf[128];
    যদি (!FT_Get_Glyph_Name(ft_face, 0, buf, আকার (buf)) &&
	len < 0 ? !strcmp (buf, name) : !strncmp (buf, name, len))
      ফেরত সত্য;
  }

  ফেরত *glyph != 0;
}

স্থির hb_bool_t
hb_ft_get_font_h_extents (hb_font_t *font HB_UNUSED,
			  শূন্য *font_তথ্য,
			  hb_font_extents_t *metrics,
			  শূন্য *user_তথ্য HB_UNUSED)
{
  ধ্রুবক hb_ft_font_t *ft_font = (ধ্রুবক hb_ft_font_t *) font_তথ্য;
  _hb_ft_hb_font_check_changed (font, ft_font);

  hb_lock_t lock (ft_font->lock);
  FT_Face ft_face = ft_font->ft_face;
  দশমিক y_mult;
#ifdef HAVE_FT_GET_TRANSFORM
  যদি (ft_font->transform)
  {
    FT_Matrix matrix;
    FT_Get_Transform (ft_face, &matrix, nullptr);
    y_mult = sqrtf ((দশমিক)matrix.yx * matrix.yx + (দশমিক)matrix.yy * matrix.yy) / 65536.f;
    y_mult *= font->y_scale < 0 ? -1 : +1;
  }
  অন্যথা
#endif
  {
    y_mult = font->y_scale < 0 ? -1 : +1;
  }

  যদি (ft_face->units_per_EM != 0)
  {
    দ্বিগুণ ascender = FT_MulFix(ft_face->ascender, ft_face->আকার_2->metrics.y_scale);
    দ্বিগুণ descender = FT_MulFix(ft_face->descender, ft_face->আকার_2->metrics.y_scale);
    দ্বিগুণ height = FT_MulFix( ft_face->height, ft_face->আকার_2->metrics.y_scale );
    metrics->ascender = hb_clamp_to<hb_position_t> ((দ্বিগুণ) y_mult * ascender);
    metrics->descender = hb_clamp_to<hb_position_t> ((দ্বিগুণ) y_mult * descender);
    metrics->সারি_gap = hb_clamp_to<hb_position_t> ((দ্বিগুণ) y_mult * (height - (ascender - descender)));
  }
  অন্যথা
  {
    /* Bitmap-only font, eg. color bitmap font. */
    দ্বিগুণ ascender = ft_face->আকার_2->metrics.ascender;
    দ্বিগুণ descender = ft_face->আকার_2->metrics.descender;
    দ্বিগুণ height = ft_face->আকার_2->metrics.height;
    metrics->ascender = hb_clamp_to<hb_position_t> ((দ্বিগুণ) y_mult * ascender);
    metrics->descender = hb_clamp_to<hb_position_t> ((দ্বিগুণ) y_mult * descender);
    metrics->সারি_gap = hb_clamp_to<hb_position_t> ((দ্বিগুণ) y_mult * (height - (ascender - descender)));
  }

  ফেরত সত্য;
}

#ifndef HB_NO_DRAW

স্থির পূর্ণসংখ্যা
_hb_ft_move_to (ধ্রুবক FT_Vector *to,
		শূন্য *arg)
{
  hb_draw_session_t *drawing = (hb_draw_session_t *) arg;
  drawing->move_to (to->x, to->y);
  ফেরত FT_Err_Ok;
}

স্থির পূর্ণসংখ্যা
hb_ft_সারি_to (ধ্রুবক FT_Vector *to,
		শূন্য *arg)
{
  hb_draw_session_t *drawing = (hb_draw_session_t *) arg;
  drawing->সারি_to (to->x, to->y);
  ফেরত FT_Err_Ok;
}

স্থির পূর্ণসংখ্যা
_hb_ft_conic_to (ধ্রুবক FT_Vector *control,
		 ধ্রুবক FT_Vector *to,
		 শূন্য *arg)
{
  hb_draw_session_t *drawing = (hb_draw_session_t *) arg;
  drawing->quadratic_to (control->x, control->y,
			 to->x, to->y);
  ফেরত FT_Err_Ok;
}

স্থির পূর্ণসংখ্যা
_hb_ft_cubic_to (ধ্রুবক FT_Vector *control1,
		 ধ্রুবক FT_Vector *control2,
		 ধ্রুবক FT_Vector *to,
		 শূন্য *arg)
{
  hb_draw_session_t *drawing = (hb_draw_session_t *) arg;
  drawing->cubic_to (control1->x, control1->y,
		     control2->x, control2->y,
		     to->x, to->y);
  ফেরত FT_Err_Ok;
}

স্থির hb_bool_t
hb_ft_draw_glyph_or_fail (hb_font_t *font,
			  শূন্য *font_তথ্য,
			  hb_codepoint_t glyph,
			  hb_draw_funcs_t *draw_funcs, শূন্য *draw_তথ্য,
			  শূন্য *user_তথ্য HB_UNUSED)
{
  ধ্রুবক hb_ft_font_t *ft_font = (ধ্রুবক hb_ft_font_t *) font_তথ্য;
  _hb_ft_hb_font_check_changed (font, ft_font);

  hb_lock_t lock (ft_font->lock);
  FT_Face ft_face = ft_font->ft_face;

  যদি (unlikely (FT_Load_Glyph (ft_face, glyph,
			       FT_LOAD_NO_BITMAP | ft_font->load_flags)))
    ফেরত মিথ্যা;

  যদি (ft_face->glyph->format != FT_GLYPH_FORMAT_OUTLINE)
    ফেরত মিথ্যা;

  ধ্রুবক FT_Outline_Funcs outline_funcs = {
    _hb_ft_move_to,
    hb_ft_সারি_to,
    _hb_ft_conic_to,
    _hb_ft_cubic_to,
    0, /* shift */
    0, /* delta */
  };

  hb_draw_session_t draw_session {draw_funcs, draw_তথ্য};

  FT_Outline_Decompose (&ft_face->glyph->outline,
			&outline_funcs,
			&draw_session);

  ফেরত সত্য;
}
#endif

#ifndef HB_NO_PAINT
#যদি (FREETYPE_MAJOR*10000 + FREETYPE_MINOR*100 + FREETYPE_PATCH) >= 21300

#অন্তর্ভাব "hb-ft-colr.hh"

স্থির hb_bool_t
hb_ft_paint_glyph_or_fail (hb_font_t *font,
			   শূন্য *font_তথ্য,
			   hb_codepoint_t gid,
			   hb_paint_funcs_t *paint_funcs, শূন্য *paint_তথ্য,
			   unsigned পূর্ণসংখ্যা palette_সূচক,
			   hb_color_t foreground,
			   শূন্য *user_তথ্য)
{
  ধ্রুবক hb_ft_font_t *ft_font = (ধ্রুবক hb_ft_font_t *) font_তথ্য;
  _hb_ft_hb_font_check_changed (font, ft_font);

  hb_lock_t lock (ft_font->lock);
  FT_Face ft_face = ft_font->ft_face;

  FT_Long load_flags = ft_font->load_flags | FT_LOAD_COLOR;
#যদি (FREETYPE_MAJOR*10000 + FREETYPE_MINOR*100 + FREETYPE_PATCH) >= 21301
  load_flags |= FT_LOAD_NO_SVG;
#endif

  /* We release the lock before calling into glyph callbacks, such that
   * eg. draw API can call back into the face.*/

  যদি (unlikely (FT_Load_Glyph (ft_face, gid, load_flags)))
    ফেরত মিথ্যা;

  যদি (ft_face->glyph->format == FT_GLYPH_FORMAT_OUTLINE)
  {
    যদি (hb_ft_paint_glyph_colr (font, font_তথ্য, gid,
				paint_funcs, paint_তথ্য,
				palette_সূচক, foreground,
				user_তথ্য))
      ফেরত সত্য;

    // Outline glyph
    ফেরত মিথ্যা;
  }

  auto *glyph = ft_face->glyph;
  যদি (glyph->format == FT_GLYPH_FORMAT_BITMAP)
  {
    যৌক্তিক ret = মিথ্যা;
    auto &bitmap = glyph->bitmap;
    যদি (bitmap.pixel_mode == FT_PIXEL_MODE_BGRA)
    {
      যদি (bitmap.pitch != (signed) bitmap.width * 4)
        ফেরত ret;

      ft_font->lock.unlock ();

      hb_blob_t *blob = hb_blob_create ((ধ্রুবক অক্ষর *) bitmap.বাফার,
					bitmap.pitch * bitmap.rows,
					HB_MEMORY_MODE_DUPLICATE,
					nullptr, nullptr);

      hb_glyph_extents_t extents;
      যদি (!font->get_glyph_extents (gid, &extents, মিথ্যা))
	যাও out;

      যদি (paint_funcs->image (paint_তথ্য,
			      blob,
			      bitmap.width,
			      bitmap.rows,
			      HB_PAINT_IMAGE_FORMAT_BGRA,
			      0.f,
			      &extents))
        ret = সত্য;

    out:
      hb_blob_destroy (blob);
      ft_font->lock.lock ();
    }

    ফেরত ret;
  }
  ফেরত মিথ্যা;
}
#endif
#endif


স্থির অন্তঃরেখ শূন্য free_static_ft_funcs ();

স্থির কাঠামো hb_ft_font_funcs_lazy_loader_t : hb_font_funcs_lazy_loader_t<hb_ft_font_funcs_lazy_loader_t>
{
  স্থির hb_font_funcs_t *create ()
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
#যদি (FREETYPE_MAJOR*10000 + FREETYPE_MINOR*100 + FREETYPE_PATCH) >= 21300
    hb_font_funcs_set_paint_glyph_or_fail_func (funcs, hb_ft_paint_glyph_or_fail, nullptr, nullptr);
#endif
#endif

    hb_font_funcs_make_immutable (funcs);

    hb_atexit (free_static_ft_funcs);

    ফেরত funcs;
  }
} static_ft_funcs;

স্থির অন্তঃরেখ
শূন্য free_static_ft_funcs ()
{
  static_ft_funcs.free_instance ();
}

স্থির hb_font_funcs_t *
_hb_ft_get_font_funcs ()
{
  ফেরত static_ft_funcs.get_unconst ();
}

স্থির শূন্য
_hb_ft_font_set_funcs (hb_font_t *font, FT_Face ft_face, যৌক্তিক unref,
		       যৌক্তিক static_library = মিথ্যা)
{
  যৌক্তিক প্রতীক = ft_face->charmap && ft_face->charmap->encoding == FT_ENCODING_MS_প্রতীক;

  hb_ft_font_t *ft_font = _hb_ft_font_create (ft_face, প্রতীক, unref, static_library);
  যদি (unlikely (!ft_font)) ফেরত;

  hb_font_set_funcs (font,
		     _hb_ft_get_font_funcs (),
		     ft_font,
		     _hb_ft_font_destroy);
}


স্থির hb_blob_t *
_hb_ft_reference_table (hb_face_t *face HB_UNUSED, hb_tag_t tag, শূন্য *user_তথ্য)
{
  FT_Face ft_face = (FT_Face) user_তথ্য;
  FT_Byte *বাফার;
  FT_ULong  দৈর্ঘ্য = 0;
  FT_Error error;

  /* In new FreeType, a tag value of 1 loads the SFNT table directory. Reject it. */
  যদি (tag == 1)
    ফেরত nullptr;

  /* Note: FreeType like HarfBuzz uses the NONE tag for fetching the entire blob */

  error = FT_Load_Sfnt_Table (ft_face, tag, 0, nullptr, &দৈর্ঘ্য);
  যদি (error)
    ফেরত nullptr;

  বাফার = (FT_Byte *) hb_malloc (দৈর্ঘ্য);
  যদি (!বাফার)
    ফেরত nullptr;
  auto বাফার_guard = hb_make_scope_guard ([&]() { hb_free (বাফার); });

  error = FT_Load_Sfnt_Table (ft_face, tag, 0, বাফার, &দৈর্ঘ্য);
  যদি (error)
    ফেরত nullptr;

  বাফার_guard.release ();
  ফেরত hb_blob_create ((ধ্রুবক অক্ষর *) বাফার, দৈর্ঘ্য,
			 HB_MEMORY_MODE_WRITABLE,
			 বাফার, hb_free);
}

স্থির unsigned
_hb_ft_get_table_tags (ধ্রুবক hb_face_t *face HB_UNUSED,
		       unsigned পূর্ণসংখ্যা start_offset,
		       unsigned পূর্ণসংখ্যা *table_গুনতি_2,
		       hb_tag_t *table_tags,
		       শূন্য *user_তথ্য)
{
  FT_Face ft_face = (FT_Face) user_তথ্য;

  FT_ULong population = 0;
  FT_Sfnt_Table_Info (ft_face,
		      0, // table_index; ignored
		      nullptr,
                      &population);

  যদি (!table_গুনতি_2)
    ফেরত population;

  যদি (unlikely (start_offset >= population))
  {
    *table_গুনতি_2 = 0;
    ফেরত population;
  }

  unsigned end_offset = start_offset + *table_গুনতি_2;
  যদি (unlikely (end_offset < start_offset)) /* Overflow. */
  {
    *table_গুনতি_2 = 0;
    ফেরত population;
  }
  end_offset = hb_min (end_offset, (unsigned) population);

  *table_গুনতি_2 = end_offset - start_offset;
  যদি (table_tags)
    জন্য (unsigned i = start_offset; i < end_offset; i++)
    {
      FT_ULong tag = 0, দৈর্ঘ্য;
      FT_Sfnt_Table_Info (ft_face, i, &tag, &দৈর্ঘ্য);
      table_tags[i - start_offset] = tag;
    }

  ফেরত population;
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

  যদি (!ft_face->stream->পঠ) {
    hb_blob_t *blob;

    blob = hb_blob_create ((ধ্রুবক অক্ষর *) ft_face->stream->base,
			   (unsigned পূর্ণসংখ্যা) ft_face->stream->আকার_2,
			   HB_MEMORY_MODE_READONLY,
			   ft_face, destroy);
    face = hb_face_create (blob, ft_face->face_সূচক);
    hb_blob_destroy (blob);
  } অন্যথা {
    face = hb_face_create_for_tables (_hb_ft_reference_table, ft_face, destroy);
    hb_face_set_get_table_tags_func (face, _hb_ft_get_table_tags, ft_face, nullptr);
  }

  hb_face_set_সূচক (face, ft_face->face_সূচক);
  hb_face_set_upem (face, ft_face->units_per_EM);

  ফেরত face;
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
  ফেরত hb_ft_face_create (ft_face, _hb_ft_face_destroy);
}

স্থির শূন্য
hb_ft_face_finalize (শূন্য *arg)
{
  FT_Face ft_face = (FT_Face) arg;
  hb_face_destroy ((hb_face_t *) ft_face->generic.তথ্য);
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
  যদি (unlikely (!ft_face->generic.তথ্য || ft_face->generic.finalizer != hb_ft_face_finalize))
  {
    যদি (ft_face->generic.finalizer)
      ft_face->generic.finalizer (ft_face);

    ft_face->generic.তথ্য = hb_ft_face_create (ft_face, nullptr);
    ft_face->generic.finalizer = hb_ft_face_finalize;
  }

  ফেরত hb_face_reference ((hb_face_t *) ft_face->generic.তথ্য);
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
  _hb_ft_font_set_funcs (font, ft_face, মিথ্যা);
  hb_ft_font_changed (font);
  ফেরত font;
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
শূন্য
hb_ft_font_changed (hb_font_t *font)
{
  যদি (font->destroy != (hb_destroy_func_t) _hb_ft_font_destroy)
    ফেরত;

  hb_ft_font_t *ft_font = (hb_ft_font_t *) font->user_তথ্য;

  FT_Face ft_face = ft_font->ft_face;

  hb_font_set_scale (font,
		     (পূর্ণসংখ্যা) (((uint64_t) ft_face->আকার_2->metrics.x_scale * (uint64_t) ft_face->units_per_EM + (1u<<15)) >> 16),
		     (পূর্ণসংখ্যা) (((uint64_t) ft_face->আকার_2->metrics.y_scale * (uint64_t) ft_face->units_per_EM + (1u<<15)) >> 16));
#যদি 0 /* hb-ft works in no-hinting model */
  hb_font_set_ppem (font,
		    ft_face->আকার_2->metrics.x_ppem,
		    ft_face->আকার_2->metrics.y_ppem);
#endif

#যদি defined(HAVE_FT_GET_VAR_BLEND_COORDINATES) && !defined(HB_NO_VAR)
  FT_MM_Var *mm_var = nullptr;
  যদি (!FT_Get_MM_Var (ft_face, &mm_var))
  {
    FT_Fixed *ft_coords = (FT_Fixed *) hb_calloc (mm_var->অঙ্ক_axis, আকার (FT_Fixed));
    পূর্ণসংখ্যা *coords = (পূর্ণসংখ্যা *) hb_calloc (mm_var->অঙ্ক_axis, আকার (পূর্ণসংখ্যা));
    যদি (coords && ft_coords)
    {
      যদি (!FT_Get_Var_Blend_Coordinates (ft_face, mm_var->অঙ্ক_axis, ft_coords))
      {
	যৌক্তিক nonzero = মিথ্যা;

	জন্য (unsigned পূর্ণসংখ্যা i = 0; i < mm_var->অঙ্ক_axis; ++i)
	 {
	  coords[i] = (ft_coords[i] + 2) >> 2;
	  nonzero = nonzero || coords[i];
	 }

	যদি (nonzero)
	  hb_font_set_var_coords_normalized (font, coords, mm_var->অঙ্ক_axis);
	অন্যথা
	  hb_font_set_var_coords_normalized (font, nullptr, 0);
      }
    }
    hb_free (coords);
    hb_free (ft_coords);
#ifdef HAVE_FT_DONE_MM_VAR
    FT_Done_MM_Var (ft_face->glyph->library, mm_var);
#অন্যথা
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
  যদি (font->destroy != (hb_destroy_func_t) _hb_ft_font_destroy)
    ফেরত মিথ্যা;

  hb_ft_font_t *ft_font = (hb_ft_font_t *) font->user_তথ্য;

  ফেরত _hb_ft_hb_font_check_changed (font, ft_font);
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
  ফেরত hb_ft_font_create (ft_face, _hb_ft_face_destroy);
}


স্থির শূন্য * _hb_ft_alloc (FT_Memory memory, long আকার_2)
{ ফেরত hb_malloc (আকার_2); }

স্থির শূন্য _hb_ft_free (FT_Memory memory, শূন্য *block)
{ hb_free (block); }

স্থির শূন্য * _hb_ft_realloc (FT_Memory memory, long cur_আকার, long new_আকার, শূন্য *block)
{ ফেরত hb_realloc (block, new_আকার); }

স্থির FT_MemoryRec_ m =
{
  nullptr,
  _hb_ft_alloc,
  _hb_ft_free,
  _hb_ft_realloc
};

স্থির অন্তঃরেখ শূন্য free_static_ft_library ();
স্থির অন্তঃরেখ শূন্য free_static_ft_library_mutex ();

স্থির কাঠামো hb_ft_library_mutex_lazy_loader_t : hb_lazy_loader_t<hb_mutex_t,
								   hb_ft_library_mutex_lazy_loader_t>
{
  স্থির hb_mutex_t *create ()
  {
    hb_mutex_t *lock = (hb_mutex_t *) hb_calloc (1, আকার (hb_mutex_t));
    যদি (unlikely (!lock))
      ফেরত nullptr;

    lock = নতুন (lock) hb_mutex_t;
    hb_atexit (free_static_ft_library_mutex);
    ফেরত lock;
  }
  স্থির শূন্য destroy (hb_mutex_t *lock)
  {
    lock->~hb_mutex_t ();
    hb_free (lock);
  }
  স্থির hb_mutex_t *get_null ()
  {
    ফেরত nullptr;
  }
} static_ft_library_mutex;

স্থির কাঠামো hb_ft_library_lazy_loader_t : hb_lazy_loader_t<hb_remove_pointer<FT_Library>,
							     hb_ft_library_lazy_loader_t>
{
  স্থির FT_Library create ()
  {
    FT_Library l;
    যদি (FT_New_Library (&m, &l))
      ফেরত nullptr;

    FT_Add_Default_Modules (l);
    FT_Set_Default_Properties (l);

    hb_atexit (free_static_ft_library);

    ফেরত l;
  }
  স্থির শূন্য destroy (FT_Library l)
  {
    FT_Done_Library (l);
  }
  স্থির FT_Library get_null ()
  {
    ফেরত nullptr;
  }
} static_ft_library;

স্থির অন্তঃরেখ
শূন্য free_static_ft_library_mutex ()
{
  static_ft_library_mutex.free_instance ();
}

স্থির অন্তঃরেখ
শূন্য free_static_ft_library ()
{
  hb_lock_t lock (static_ft_library_mutex.get_unconst ());
  static_ft_library.free_instance ();
}

স্থির FT_Library
reference_ft_library ()
{
  hb_mutex_t *mutex = static_ft_library_mutex.get_unconst ();
  যদি (unlikely (!mutex))
    ফেরত nullptr;
  hb_lock_t lock (mutex);
  FT_Library l = static_ft_library.get_unconst ();
  যদি (unlikely (FT_Reference_Library (l)))
  {
    DEBUG_বার্তা (FT, l, "FT_Reference_Library() failed");
    ফেরত nullptr;
  }
  ফেরত l;
}

স্থির hb_user_তথ্য_চাবি_t ft_library_চাবি = {0};

স্থির শূন্য
_hb_ft_face_destroy_static (শূন্য *তথ্য)
{
  hb_lock_t lock (static_ft_library_mutex.get_unconst ());
  FT_Done_Face ((FT_Face) তথ্য);
}

স্থির FT_Error
new_ft_face (FT_Library  ft_library,
	     ধ্রুবক অক্ষর *file_name,
	     unsigned পূর্ণসংখ্যা সূচক_2,
	     FT_Face     *ft_face)
{
  hb_lock_t lock (static_ft_library_mutex.get_unconst ());
  ফেরত FT_New_Face (ft_library, file_name, সূচক_2, ft_face);
}

স্থির FT_Error
new_ft_memory_face (FT_Library    ft_library,
		    ধ্রুবক FT_Byte *blob_তথ্য,
		    unsigned পূর্ণসংখ্যা   blob_আকার,
		    unsigned পূর্ণসংখ্যা   সূচক_2,
		    FT_Face       *ft_face)
{
  hb_lock_t lock (static_ft_library_mutex.get_unconst ());
  ফেরত FT_New_Memory_Face (ft_library, blob_তথ্য, blob_আকার, সূচক_2, ft_face);
}

স্থির শূন্য
finalize_ft_library (শূন্য *arg)
{
  FT_Face ft_face = (FT_Face) arg;
  FT_Done_Library ((FT_Library) ft_face->generic.তথ্য);
}

স্থির শূন্য
destroy_ft_library (শূন্য *arg)
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
hb_ft_face_create_from_file_or_fail (ধ্রুবক অক্ষর   *file_name,
				     unsigned পূর্ণসংখ্যা  সূচক_2)
{
  FT_Library ft_library = reference_ft_library ();
  যদি (unlikely (!ft_library))
  {
    DEBUG_বার্তা (FT, ft_library, "reference_ft_library failed");
    ফেরত nullptr;
  }

  FT_Face ft_face;
  যদি (unlikely (new_ft_face (ft_library,
			     file_name,
			     সূচক_2,
			     &ft_face)))
  {
    destroy_ft_library (ft_library);
    ফেরত nullptr;
  }

  FT_Reference_Face (ft_face);
  hb_face_t *face = hb_ft_face_create (ft_face, _hb_ft_face_destroy_static);
  _hb_ft_face_destroy_static (ft_face);

  ft_face->generic.তথ্য = ft_library;
  ft_face->generic.finalizer = finalize_ft_library;

  যদি (hb_face_is_immutable (face))
    ফেরত nullptr;

  ফেরত face;
}

স্থির hb_user_তথ্য_চাবি_t ft_blob_চাবি = {0};

স্থির শূন্য
_destroy_blob (শূন্য *p)
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
				     unsigned পূর্ণসংখ্যা  সূচক_2)
{
  FT_Library ft_library = reference_ft_library ();
  যদি (unlikely (!ft_library))
  {
    DEBUG_বার্তা (FT, ft_library, "reference_ft_library failed");
    ফেরত nullptr;
  }

  hb_blob_make_immutable (blob);
  unsigned blob_আকার;
  ধ্রুবক অক্ষর *blob_তথ্য = hb_blob_get_তথ্য (blob, &blob_আকার);

  FT_Face ft_face;
  যদি (unlikely (new_ft_memory_face (ft_library,
				    (ধ্রুবক FT_Byte *) blob_তথ্য,
				    blob_আকার,
				    সূচক_2,
				    &ft_face)))
  {
    destroy_ft_library (ft_library);
    ফেরত nullptr;
  }

  FT_Reference_Face (ft_face);
  hb_face_t *face = hb_ft_face_create (ft_face, _hb_ft_face_destroy_static);
  _hb_ft_face_destroy_static (ft_face);

  ft_face->generic.তথ্য = ft_library;
  ft_face->generic.finalizer = finalize_ft_library;

  যদি (hb_face_is_immutable (face))
    ফেরত nullptr;

  // Hook the blob to the hb_face_t, since FT_Face still needs it.
  hb_blob_reference (blob);
  যদি (!hb_face_set_user_তথ্য (face, &ft_blob_চাবি, blob, _destroy_blob, সত্য))
  {
    hb_blob_destroy (blob);
    hb_face_destroy (face);
    ফেরত nullptr;
  }

  ফেরত face;
}

স্থির শূন্য
_release_blob (শূন্য *arg)
{
  FT_Face ft_face = (FT_Face) arg;
  hb_blob_destroy ((hb_blob_t *) ft_face->generic.তথ্য);
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
শূন্য
hb_ft_font_set_funcs (hb_font_t *font)
{
  পূর্ণসংখ্যা load_flags = FT_LOAD_DEFAULT | FT_LOAD_NO_HINTING;
  যদি (font->destroy == (hb_destroy_func_t) _hb_ft_font_destroy && font->user_তথ্য)
  {
    ধ্রুবক hb_ft_font_t *existing_ft_font = (ধ্রুবক hb_ft_font_t *) font->user_তথ্য;
    load_flags = existing_ft_font->load_flags;
  }

  // In case of failure...
  hb_font_set_funcs (font,
		     hb_font_funcs_get_empty (),
		     nullptr, nullptr);

  hb_blob_t *blob = hb_face_reference_blob (font->face);
  unsigned পূর্ণসংখ্যা blob_দৈর্ঘ্য;
  ধ্রুবক অক্ষর *blob_তথ্য = hb_blob_get_তথ্য (blob, &blob_দৈর্ঘ্য);
  যদি (unlikely (!blob_দৈর্ঘ্য))
    DEBUG_বার্তা (FT, font, "Font face has empty blob");

  FT_Library ft_library = reference_ft_library ();
  যদি (unlikely (!ft_library))
  {
    hb_blob_destroy (blob);
    DEBUG_বার্তা (FT, font, "reference_ft_library failed");
    ফেরত;
  }

  FT_Face ft_face = nullptr;
  যদি (unlikely (new_ft_memory_face (ft_library,
				    (ধ্রুবক FT_Byte *) blob_তথ্য,
				    blob_দৈর্ঘ্য,
				    hb_face_get_সূচক (font->face),
				    &ft_face)))
  {
    hb_blob_destroy (blob);
    destroy_ft_library (ft_library);
    DEBUG_বার্তা (FT, font, "FT_New_Memory_Face() failed");
    ফেরত;
  }

  যদি (FT_Select_Charmap (ft_face, FT_ENCODING_MS_প্রতীক))
    FT_Select_Charmap (ft_face, FT_ENCODING_UNICODE);

  // Hook the blob to the FT_Face
  ft_face->generic.তথ্য = blob;
  ft_face->generic.finalizer = _release_blob;

  // And the FT_Library to the blob
  যদি (unlikely (!hb_blob_set_user_তথ্য (blob, &ft_library_চাবি, ft_library, destroy_ft_library, সত্য)))
  {
    DEBUG_বার্তা (FT, font, "hb_blob_set_user_data() failed");
    _hb_ft_face_destroy_static (ft_face);
    destroy_ft_library (ft_library);
    ফেরত;
  }

  _hb_ft_font_set_funcs (font, ft_face, সত্য, সত্য);
  hb_ft_font_set_load_flags (font, load_flags);

  _hb_ft_hb_font_changed (font, ft_face);
}

#endif
