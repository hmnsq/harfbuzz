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

#شامل "hb.hh"

#اگر_تعریف_شدہ HAVE_FREETYPE

#شامل "hb-ft.h"

#شامل "hb-cache.hh"
#شامل "hb-draw.hh"
#شامل "hb-font.hh"
#شامل "hb-machinery.hh"
#شامل "hb-ot-os2-table.hh"
#شامل "hb-ot-shaper-arabic-pua.hh"
#شامل "hb-paint.hh"

#شامل FT_MODULE_H
#شامل FT_ADVANCES_H
#شامل FT_مضرب_MASTERS_H
#شامل FT_OUTLINE_H
#شامل FT_TRUETYPE_TABLES_H
#شامل FT_SYNTHESIS_H
#اگر (FREETYPE_MAJOR*10000 + FREETYPE_MINOR*100 + FREETYPE_PATCH) >= 21300
#شامل FT_COLOR_H
#اگر_ختم


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


استعمال hb_ft_advance_cache_t = hb_cache_t<16, 24, 8, جھوٹ>;

ساکن خالی _hb_ft_face_destroy_static (خالی *ڈیٹا);

ساخت hb_ft_font_t
{
  صحیح بھرو_flags;
  منطقی علامت; /* Whether selected cmap is symbol cmap. */
  منطقی unref; /* Whether to destroy ft_face when done. */
  منطقی transform; /* Whether to apply FT_Face's transform. */
  منطقی static_library; /* Whether ft_face uses static_ft_library. */

  تغیرپذیر hb_mutex_t lock; /* Protects members below. */
  FT_Face ft_face;
  تغیرپذیر hb_atomic_t<غیرعلامتی> cached_serial;
  تغیرپذیر hb_ft_advance_cache_t advance_cache;
};

ساکن hb_ft_font_t *
_hb_ft_font_create (FT_Face ft_face, منطقی علامت, منطقی unref, منطقی static_library)
{
  hb_ft_font_t *ft_font = (hb_ft_font_t *) hb_calloc (1, حجم (hb_ft_font_t));
  اگر (unlikely (!ft_font)) واپس خالی_اشاریہ;

  ft_font->lock.init ();
  ft_font->ft_face = ft_face;
  ft_font->علامت = علامت;
  ft_font->unref = unref;
  ft_font->static_library = static_library;

  ft_font->بھرو_flags = FT_بھرو_DEFAULT | FT_بھرو_NO_HINTING;

  ft_font->cached_serial = UINT_MAX;
  نیا (&ft_font->advance_cache) hb_ft_advance_cache_t;

  واپس ft_font;
}

ساکن خالی
_hb_ft_face_destroy (خالی *ڈیٹا)
{
  FT_مکمل_Face ((FT_Face) ڈیٹا);
}

ساکن خالی
_hb_ft_font_destroy (خالی *ڈیٹا)
{
  hb_ft_font_t *ft_font = (hb_ft_font_t *) ڈیٹا;

  اگر (ft_font->unref)
  {
    اگر (ft_font->static_library)
    {
      /* ft_font->ft_face->generic.data is the blob that hb_ft_font_set_funcs()
       * hooked up as this internal FT_Face's finalizer target (_release_blob).
       * FT_Done_Face() below runs that finalizer while static_ft_library_mutex
       * is held; if dropping its reference were to free the blob right there,
       * the blob's attached destroy_ft_library() user-data callback would try
       * to re-lock the same non-recursive mutex and self-deadlock. Hold an
       * extra reference across the locked call so the blob (and thus that
       * callback) can only be freed after the lock is released. */
      hb_blob_t *blob = hb_blob_reference ((hb_blob_t *) ft_font->ft_face->generic.ڈیٹا);
      _hb_ft_face_destroy_static (ft_font->ft_face);
      hb_blob_destroy (blob);
    }
    ورنہ
      _hb_ft_face_destroy (ft_font->ft_face);
  }

  ft_font->lock.fini ();

  hb_free (ft_font);
}


/* hb_font changed, update FT_Face. */
ساکن خالی hb_ft_hb_font_بدلا (hb_font_t *font, FT_Face ft_face)
{
  اگر (unlikely (font->destroy != (hb_destroy_func_t) _hb_ft_font_destroy))
    واپس;

  hb_ft_font_t *ft_font = (hb_ft_font_t *) font->user_ڈیٹا;

  اعشاری x_mult = 1.f, y_mult = 1.f;

  اگر (font->x_scale < 0) x_mult = -x_mult;
  اگر (font->y_scale < 0) y_mult = -y_mult;

  اگر (FT_سیٹ_Char_سائز (ft_face,
			مطلق_قدر (font->x_scale), مطلق_قدر (font->y_scale),
			0, 0
#اگر 0
		    font->x_ppem * 72 * 64 / font->x_scale,
		    font->y_ppem * 72 * 64 / font->y_scale
#اگر_ختم
     ) && ft_face->عدد_fixed_sizes)
  {
#اگر_تعریف_شدہ HAVE_FT_GET_TRANSFORM
    /* Bitmap font, eg. bitmap color emoji. */
    /* Pick largest size? */
    صحیح x_scale  = ft_face->دستیاب_sizes[ft_face->عدد_fixed_sizes - 1].x_ppem;
    صحیح y_scale = ft_face->دستیاب_sizes[ft_face->عدد_fixed_sizes - 1].y_ppem;
    FT_سیٹ_Char_سائز (ft_face,
		      x_scale, y_scale,
		      0, 0);

    /* This contains the sign that was previously in x_mult/y_mult. */
    x_mult = (اعشاری) font->x_scale / x_scale;
    y_mult = (اعشاری) font->y_scale / y_scale;
#اگر_ختم
  }
  ورنہ
  { /* Shrug */ }


  اگر (x_mult != 1.f || y_mult != 1.f)
  {
    FT_میٹرکس میٹرکس = { (صحیح) roundf (x_mult * (1<<16)), 0,
			  0, (صحیح) roundf (y_mult * (1<<16))};
    FT_سیٹ_Transform (ft_face, &میٹرکس, خالی_اشاریہ);
    ft_font->transform = سچ;
  }
  ورنہ
    FT_سیٹ_Transform (ft_face, خالی_اشاریہ, خالی_اشاریہ);

#اگر defined(HAVE_FT_GET_VAR_BLEND_COORDINATES) && !defined(HB_NO_VAR)
  اگر (font->has_nonzero_coords)
  {
    غیرعلامتی صحیح عدد_coords;
    ثابت اعشاری *coords = hb_font_get_var_coords_design (font, &عدد_coords);
    FT_Fixed *ft_coords = (FT_Fixed *) hb_calloc (عدد_coords, حجم (FT_Fixed));
    اگر (ft_coords)
    {
      برائے (غیرعلامتی صحیح i = 0; i < عدد_coords; i++)
	  ft_coords[i] = coords[i] * 65536.f;
      FT_سیٹ_Var_Design_Coordinates (ft_face, عدد_coords, ft_coords);
      hb_free (ft_coords);
    }
  }
  ورنہ اگر (font->عدد_coords)
  {
    // Some old versions of FreeType crash if we
    // call this function on non-variable fonts.
    FT_سیٹ_Var_Design_Coordinates (ft_face, 0, خالی_اشاریہ);
  }
#اگر_ختم
}

/* Check if hb_font changed, update FT_Face. */
ساکن اندرونی منطقی
hb_ft_hb_font_check_بدلا (hb_font_t *font,
			      ثابت hb_ft_font_t *ft_font)
{
  اگر (font->serial != ft_font->cached_serial)
  {
    hb_تالا_t lock (ft_font->lock);
    hb_ft_hb_font_بدلا (font, ft_font->ft_face);
    ft_font->advance_cache.clear ();
    ft_font->cached_serial.سیٹ_release (font->serial.get_acquire ());
    واپس سچ;
  }
  واپس جھوٹ;
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
خالی
hb_ft_font_سیٹ_بھرو_flags (hb_font_t *font, صحیح بھرو_flags)
{
  اگر (hb_object_is_immutable (font))
    واپس;

  اگر (unlikely (font->destroy != (hb_destroy_func_t) _hb_ft_font_destroy))
    واپس;

  hb_ft_font_t *ft_font = (hb_ft_font_t *) font->user_ڈیٹا;

  ft_font->بھرو_flags = بھرو_flags;
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
صحیح
hb_ft_font_get_بھرو_flags (hb_font_t *font)
{
  اگر (unlikely (font->destroy != (hb_destroy_func_t) _hb_ft_font_destroy))
    واپس 0;

  ثابت hb_ft_font_t *ft_font = (ثابت hb_ft_font_t *) font->user_ڈیٹا;

  واپس ft_font->بھرو_flags;
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
  اگر (unlikely (font->destroy != (hb_destroy_func_t) _hb_ft_font_destroy))
    واپس خالی_اشاریہ;

  ثابت hb_ft_font_t *ft_font = (ثابت hb_ft_font_t *) font->user_ڈیٹا;

  واپس ft_font->ft_face;
}

#اگر_غیر_تعریف_شدہ HB_DISABLE_DEPRECATED

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
  واپس hb_ft_font_get_ft_face (font);
}

#اگر_ختم

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
hb_ft_font_تالا_face (hb_font_t *font)
{
  اگر (unlikely (font->destroy != (hb_destroy_func_t) _hb_ft_font_destroy))
    واپس خالی_اشاریہ;

  ثابت hb_ft_font_t *ft_font = (ثابت hb_ft_font_t *) font->user_ڈیٹا;

  ft_font->lock.lock ();

  واپس ft_font->ft_face;
}

/**
 * hb_ft_font_unlock_face: (skip)
 * @font: #hb_font_t to work upon
 *
 * Releases an FT_Face previously obtained with hb_ft_font_lock_face().
 *
 * Since: 2.6.5
 **/
خالی
hb_ft_font_unlock_face (hb_font_t *font)
{
  اگر (unlikely (font->destroy != (hb_destroy_func_t) _hb_ft_font_destroy))
    واپس;

  ثابت hb_ft_font_t *ft_font = (ثابت hb_ft_font_t *) font->user_ڈیٹا;

  ft_font->lock.unlock ();
}


ساکن hb_bool_t
hb_ft_get_nominal_glyph (hb_font_t *font,
			 خالی *font_ڈیٹا,
			 hb_codepoint_t unicode,
			 hb_codepoint_t *glyph,
			 خالی *user_ڈیٹا HB_غیر_مستعمل)
{
  ثابت hb_ft_font_t *ft_font = (ثابت hb_ft_font_t *) font_ڈیٹا;
  hb_تالا_t lock (ft_font->lock);
  غیرعلامتی صحیح g = FT_Get_Char_اشاریہ (ft_font->ft_face, unicode);

  اگر (unlikely (!g))
  {
    اگر (unlikely (ft_font->علامت))
    {
      انتخاب ((غیرعلامتی) font->face->جدول.OS2->get_font_صفحہ ()) {
      حالت OT::OS2::font_صفحہ_t::FONT_صفحہ_NONE:
	اگر (unicode <= 0x00FFu)
	  /* For symbol-encoded OpenType fonts, we duplicate the
	   * U+F000..F0FF range at U+0000..U+00FF.  That's what
	   * Windows seems to do, and that's hinted about at:
	   * https://docs.microsoft.com/en-us/typography/opentype/spec/recom
	   * under "Non-Standard (Symbol) Fonts". */
	  g = FT_Get_Char_اشاریہ (ft_font->ft_face, 0xF000u + unicode);
	توڑو;
#اگر_غیر_تعریف_شدہ HB_NO_OT_SHAPER_ARABIC_FALLBACK
      حالت OT::OS2::font_صفحہ_t::FONT_صفحہ_SIMP_ARABIC:
	g = FT_Get_Char_اشاریہ (ft_font->ft_face, _hb_arabic_pua_simp_map (unicode));
	توڑو;
      حالت OT::OS2::font_صفحہ_t::FONT_صفحہ_TRAD_ARABIC:
	g = FT_Get_Char_اشاریہ (ft_font->ft_face, _hb_arabic_pua_trad_map (unicode));
	توڑو;
#اگر_ختم
      طےشدہ:
	توڑو;
      }
      اگر (!g)
	واپس جھوٹ;
    }
    ورنہ
      واپس جھوٹ;
  }

  *glyph = g;
  واپس سچ;
}

ساکن غیرعلامتی صحیح
hb_ft_get_nominal_glyphs (hb_font_t *font HB_غیر_مستعمل,
			  خالی *font_ڈیٹا,
			  غیرعلامتی صحیح گنتی,
			  ثابت hb_codepoint_t *پہلا_unicode,
			  غیرعلامتی صحیح unicode_stride,
			  hb_codepoint_t *پہلا_glyph,
			  غیرعلامتی صحیح glyph_stride,
			  خالی *user_ڈیٹا HB_غیر_مستعمل)
{
  ثابت hb_ft_font_t *ft_font = (ثابت hb_ft_font_t *) font_ڈیٹا;
  hb_تالا_t lock (ft_font->lock);
  غیرعلامتی صحیح مکمل;
  برائے (مکمل = 0;
       مکمل < گنتی && (*پہلا_glyph = FT_Get_Char_اشاریہ (ft_font->ft_face, *پہلا_unicode));
       مکمل++)
  {
    پہلا_unicode = &Struct_At_انحراف_Unaligned<hb_codepoint_t> (پہلا_unicode, unicode_stride);
    پہلا_glyph = &Struct_At_انحراف_Unaligned<hb_codepoint_t> (پہلا_glyph, glyph_stride);
  }
  /* We don't need to do ft_font->symbol dance here, since HB calls the singular
   * nominal_glyph() for what we don't handle here. */
  واپس مکمل;
}


ساکن hb_bool_t
hb_ft_get_variation_glyph (hb_font_t *font HB_غیر_مستعمل,
			   خالی *font_ڈیٹا,
			   hb_codepoint_t unicode,
			   hb_codepoint_t variation_selector,
			   hb_codepoint_t *glyph,
			   خالی *user_ڈیٹا HB_غیر_مستعمل)
{
  ثابت hb_ft_font_t *ft_font = (ثابت hb_ft_font_t *) font_ڈیٹا;
  hb_تالا_t lock (ft_font->lock);
  غیرعلامتی صحیح g = FT_Face_Get_Char_Variant_اشاریہ (ft_font->ft_face, unicode, variation_selector);

  اگر (unlikely (!g))
    واپس جھوٹ;

  *glyph = g;
  واپس سچ;
}

ساکن خالی
hb_ft_get_glyph_h_advances (hb_font_t* font, خالی* font_ڈیٹا,
			    غیرعلامتی گنتی,
			    ثابت hb_codepoint_t *پہلا_glyph,
			    غیرعلامتی glyph_stride,
			    hb_position_t *پہلا_advance,
			    غیرعلامتی advance_stride,
			    خالی *user_ڈیٹا HB_غیر_مستعمل)
{
  ثابت hb_ft_font_t *ft_font = (ثابت hb_ft_font_t *) font_ڈیٹا;
  hb_ft_hb_font_check_بدلا (font, ft_font);

  hb_تالا_t lock (ft_font->lock);
  FT_Face ft_face = ft_font->ft_face;
  صحیح بھرو_flags = ft_font->بھرو_flags;
  اعشاری x_mult;
#اگر_تعریف_شدہ HAVE_FT_GET_TRANSFORM
  اگر (ft_font->transform)
  {
    FT_میٹرکس میٹرکس;
    FT_Get_Transform (ft_face, &میٹرکس, خالی_اشاریہ);
    x_mult = sqrtf ((اعشاری)میٹرکس.xx * میٹرکس.xx + (اعشاری)میٹرکس.xy * میٹرکس.xy) / 65536.f;
    x_mult *= font->x_scale < 0 ? -1 : +1;
  }
  ورنہ
#اگر_ختم
  {
    x_mult = font->x_scale < 0 ? -1 : +1;
  }

  برائے (غیرعلامتی صحیح i = 0; i < گنتی; i++)
  {
    hb_position_t advance;
    hb_codepoint_t glyph = *پہلا_glyph;

    غیرعلامتی صحیح cv;
    اگر (ft_font->advance_cache.get (glyph, &cv))
      advance = (hb_position_t) cv;
    ورنہ
    {
      FT_Fixed v = 0;
      FT_Get_Advance (ft_face, glyph, بھرو_flags, &v);
      /* Work around bug that FreeType seems to return negative advance
       * for variable-set fonts if x_scale is negative! */
      دوہرا scaled = trunc (fabs ((دوہرا) v) * (دوہرا) x_mult + (1<<9)) / (1<<10);
      advance = hb_clamp_مقصد<hb_position_t> (floor (scaled));
      ft_font->advance_cache.سیٹ (glyph, advance);
    }

    *پہلا_advance = advance;
    پہلا_glyph = &Struct_At_انحراف_Unaligned<hb_codepoint_t> (پہلا_glyph, glyph_stride);
    پہلا_advance = &Struct_At_انحراف_Unaligned<hb_position_t> (پہلا_advance, advance_stride);
  }
}

#اگر_غیر_تعریف_شدہ HB_NO_VERTICAL
ساکن hb_position_t
hb_ft_get_glyph_v_advance (hb_font_t *font,
			   خالی *font_ڈیٹا,
			   hb_codepoint_t glyph,
			   خالی *user_ڈیٹا HB_غیر_مستعمل)
{
  ثابت hb_ft_font_t *ft_font = (ثابت hb_ft_font_t *) font_ڈیٹا;
  hb_ft_hb_font_check_بدلا (font, ft_font);

  hb_تالا_t lock (ft_font->lock);
  FT_Fixed v;
  اعشاری y_mult;
#اگر_تعریف_شدہ HAVE_FT_GET_TRANSFORM
  اگر (ft_font->transform)
  {
    FT_میٹرکس میٹرکس;
    FT_Get_Transform (ft_font->ft_face, &میٹرکس, خالی_اشاریہ);
    y_mult = sqrtf ((اعشاری)میٹرکس.yx * میٹرکس.yx + (اعشاری)میٹرکس.yy * میٹرکس.yy) / 65536.f;
    y_mult *= font->y_scale < 0 ? -1 : +1;
  }
  ورنہ
#اگر_ختم
  {
    y_mult = font->y_scale < 0 ? -1 : +1;
  }

  اگر (unlikely (FT_Get_Advance (ft_font->ft_face, glyph, ft_font->بھرو_flags | FT_بھرو_VERTICAL_LAYOUT, &v)))
    واپس 0;

  /* Note: FreeType's vertical metrics grows downward while other FreeType coordinates
   * have a Y growing upward.  Hence the extra negation. */
  دوہرا advance = floor ((-(دوہرا) v + (1<<9)) / (1<<10));

  واپس hb_clamp_مقصد<hb_position_t> ((دوہرا) y_mult * advance);
}
#اگر_ختم

#اگر_غیر_تعریف_شدہ HB_NO_VERTICAL
ساکن hb_bool_t
hb_ft_get_glyph_v_origin (hb_font_t *font,
			  خالی *font_ڈیٹا,
			  hb_codepoint_t glyph,
			  hb_position_t *x,
			  hb_position_t *y,
			  خالی *user_ڈیٹا HB_غیر_مستعمل)
{
  ثابت hb_ft_font_t *ft_font = (ثابت hb_ft_font_t *) font_ڈیٹا;
  hb_ft_hb_font_check_بدلا (font, ft_font);

  hb_تالا_t lock (ft_font->lock);
  FT_Face ft_face = ft_font->ft_face;
  اعشاری x_mult, y_mult;
#اگر_تعریف_شدہ HAVE_FT_GET_TRANSFORM
  اگر (ft_font->transform)
  {
    FT_میٹرکس میٹرکس;
    FT_Get_Transform (ft_face, &میٹرکس, خالی_اشاریہ);
    x_mult = sqrtf ((اعشاری)میٹرکس.xx * میٹرکس.xx + (اعشاری)میٹرکس.xy * میٹرکس.xy) / 65536.f;
    x_mult *= font->x_scale < 0 ? -1 : +1;
    y_mult = sqrtf ((اعشاری)میٹرکس.yx * میٹرکس.yx + (اعشاری)میٹرکس.yy * میٹرکس.yy) / 65536.f;
    y_mult *= font->y_scale < 0 ? -1 : +1;
  }
  ورنہ
#اگر_ختم
  {
    x_mult = font->x_scale < 0 ? -1 : +1;
    y_mult = font->y_scale < 0 ? -1 : +1;
  }

  اگر (unlikely (FT_بھرو_Glyph (ft_face, glyph, ft_font->بھرو_flags)))
    واپس جھوٹ;

  /* Note: FreeType's vertical metrics grows downward while other FreeType coordinates
   * have a Y growing upward.  Hence the extra negation. */
  *x = hb_clamp_مقصد<hb_position_t> ((دوہرا) x_mult *
				   ((دوہرا) ft_face->glyph->metrics.horiBearingX -
				    ft_face->glyph->metrics.vertBearingX));
  *y = hb_clamp_مقصد<hb_position_t> ((دوہرا) y_mult *
				   ((دوہرا) ft_face->glyph->metrics.horiBearingY +
				    ft_face->glyph->metrics.vertBearingY));

  واپس سچ;
}
#اگر_ختم

#اگر_غیر_تعریف_شدہ HB_NO_OT_SHAPE_FALLBACK
ساکن hb_position_t
hb_ft_get_glyph_h_kerning (hb_font_t *font,
			   خالی *font_ڈیٹا,
			   hb_codepoint_t بایاں_glyph,
			   hb_codepoint_t دایاں_glyph,
			   خالی *user_ڈیٹا HB_غیر_مستعمل)
{
  ثابت hb_ft_font_t *ft_font = (ثابت hb_ft_font_t *) font_ڈیٹا;
  hb_ft_hb_font_check_بدلا (font, ft_font);

  hb_تالا_t lock (ft_font->lock);
  FT_سمتیہ kerningv;

  FT_Kerning_منوال منوال = font->x_ppem ? FT_KERNING_DEFAULT : FT_KERNING_UNFITTED;
  اگر (FT_Get_Kerning (ft_font->ft_face, بایاں_glyph, دایاں_glyph, منوال, &kerningv))
    واپس 0;

  واپس hb_clamp_مقصد<hb_position_t> ((int64_t) kerningv.x);
}
#اگر_ختم

ساکن منطقی
hb_ft_is_colr_glyph (hb_font_t *font,
		     خالی *font_ڈیٹا,
		     hb_codepoint_t gid)
{
#اگر_غیر_تعریف_شدہ HB_NO_PAINT
#اگر (FREETYPE_MAJOR*10000 + FREETYPE_MINOR*100 + FREETYPE_PATCH) >= 21300
  ثابت hb_ft_font_t *ft_font = (ثابت hb_ft_font_t *) font_ڈیٹا;
  FT_Face ft_face = ft_font->ft_face;


  /* COLRv1 */
  FT_OpaquePaint paint = {0};
  اگر (FT_Get_Color_Glyph_Paint (ft_face, gid,
			        FT_COLOR_NO_جڑ_TRANSFORM,
			        &paint))
    واپس سچ;

  /* COLRv0 */
  FT_LayerIterator  iterator;
  FT_UInt  layer_glyph_اشاریہ;
  FT_UInt  layer_color_اشاریہ;
  iterator.p  = تہی;
  اگر (FT_Get_Color_Glyph_Layer (ft_face,
				gid,
				&layer_glyph_اشاریہ,
				&layer_color_اشاریہ,
				&iterator))
    واپس سچ;
#اگر_ختم
#اگر_ختم

  واپس جھوٹ;
}

ساکن hb_bool_t
hb_ft_get_glyph_extents (hb_font_t *font,
			 خالی *font_ڈیٹا,
			 hb_codepoint_t glyph,
			 hb_glyph_extents_t *extents,
			 خالی *user_ڈیٹا HB_غیر_مستعمل)
{
  // FreeType doesn't return COLR glyph extents.
  اگر (hb_ft_is_colr_glyph (font, font_ڈیٹا, glyph))
    واپس جھوٹ;

  ثابت hb_ft_font_t *ft_font = (ثابت hb_ft_font_t *) font_ڈیٹا;
  hb_ft_hb_font_check_بدلا (font, ft_font);

  hb_تالا_t lock (ft_font->lock);
  FT_Face ft_face = ft_font->ft_face;
  اعشاری x_mult, y_mult;

#اگر_تعریف_شدہ HAVE_FT_GET_TRANSFORM
  اگر (ft_font->transform)
  {
    FT_میٹرکس میٹرکس;
    FT_Get_Transform (ft_face, &میٹرکس, خالی_اشاریہ);
    x_mult = sqrtf ((اعشاری)میٹرکس.xx * میٹرکس.xx + (اعشاری)میٹرکس.xy * میٹرکس.xy) / 65536.f;
    x_mult *= font->x_scale < 0 ? -1 : +1;
    y_mult = sqrtf ((اعشاری)میٹرکس.yx * میٹرکس.yx + (اعشاری)میٹرکس.yy * میٹرکس.yy) / 65536.f;
    y_mult *= font->y_scale < 0 ? -1 : +1;
  }
  ورنہ
#اگر_ختم
  {
    x_mult = font->x_scale < 0 ? -1 : +1;
    y_mult = font->y_scale < 0 ? -1 : +1;
  }

  اگر (unlikely (FT_بھرو_Glyph (ft_face, glyph, ft_font->بھرو_flags)))
    واپس جھوٹ;

  /* Copied from hb_font_t::scale_glyph_extents. */

  اعشاری x1 = x_mult * ft_face->glyph->metrics.horiBearingX;
  اعشاری y1 = y_mult * ft_face->glyph->metrics.horiBearingY;
  اعشاری x2 = x1 + x_mult *  ft_face->glyph->metrics.چوڑائی;
  اعشاری y2 = y1 + y_mult * -ft_face->glyph->metrics.اونچائی;

  دوہرا rx1 = (دوہرا) roundf (x1);
  دوہرا ry1 = (دوہرا) roundf (y1);
  دوہرا rx2 = (دوہرا) roundf (x2);
  دوہرا ry2 = (دوہرا) roundf (y2);

  extents->x_bearing = hb_clamp_مقصد<hb_position_t> (rx1);
  extents->y_bearing = hb_clamp_مقصد<hb_position_t> (ry1);
  extents->چوڑائی = hb_clamp_مقصد<hb_position_t> (rx2 - rx1);
  extents->اونچائی = hb_clamp_مقصد<hb_position_t> (ry2 - ry1);

  واپس سچ;
}

ساکن hb_bool_t
hb_ft_get_glyph_contour_نقطہ (hb_font_t *font HB_غیر_مستعمل,
			       خالی *font_ڈیٹا,
			       hb_codepoint_t glyph,
			       غیرعلامتی صحیح نقطہ_اشاریہ,
			       hb_position_t *x,
			       hb_position_t *y,
			       خالی *user_ڈیٹا HB_غیر_مستعمل)
{
  ثابت hb_ft_font_t *ft_font = (ثابت hb_ft_font_t *) font_ڈیٹا;
  hb_ft_hb_font_check_بدلا (font, ft_font);

  hb_تالا_t lock (ft_font->lock);
  FT_Face ft_face = ft_font->ft_face;

  اگر (unlikely (FT_بھرو_Glyph (ft_face, glyph, ft_font->بھرو_flags)))
      واپس جھوٹ;

  اگر (unlikely (ft_face->glyph->format != FT_GLYPH_FORMAT_OUTLINE))
      واپس جھوٹ;

  اگر (unlikely (نقطہ_اشاریہ >= (غیرعلامتی صحیح) ft_face->glyph->outline.n_points))
      واپس جھوٹ;

  *x = ft_face->glyph->outline.points[نقطہ_اشاریہ].x;
  *y = ft_face->glyph->outline.points[نقطہ_اشاریہ].y;

  واپس سچ;
}

ساکن hb_bool_t
hb_ft_get_glyph_name (hb_font_t *font HB_غیر_مستعمل,
		      خالی *font_ڈیٹا,
		      hb_codepoint_t glyph,
		      حرف *name, غیرعلامتی صحیح سائز,
		      خالی *user_ڈیٹا HB_غیر_مستعمل)
{
  ثابت hb_ft_font_t *ft_font = (ثابت hb_ft_font_t *) font_ڈیٹا;
  hb_تالا_t lock (ft_font->lock);
  FT_Face ft_face = ft_font->ft_face;

  اگر (!سائز)
  {
    حرف buf[128];
    واپس !FT_Get_Glyph_Name (ft_face, glyph, buf, حجم (buf)) && *buf;
  }

  hb_bool_t ret = !FT_Get_Glyph_Name (ft_face, glyph, name, سائز);
  اگر (ret && (سائز && !*name))
    ret = جھوٹ;

  واپس ret;
}

ساکن hb_bool_t
hb_ft_get_glyph_from_name (hb_font_t *font HB_غیر_مستعمل,
			   خالی *font_ڈیٹا,
			   ثابت حرف *name, صحیح len, /* -1 means nul-terminated */
			   hb_codepoint_t *glyph,
			   خالی *user_ڈیٹا HB_غیر_مستعمل)
{
  ثابت hb_ft_font_t *ft_font = (ثابت hb_ft_font_t *) font_ڈیٹا;
  hb_تالا_t lock (ft_font->lock);
  FT_Face ft_face = ft_font->ft_face;

  اگر (len < 0)
    *glyph = FT_Get_Name_اشاریہ (ft_face, (FT_String *) name);
  ورنہ {
    /* Make a nul-terminated version. */
    حرف buf[128];
    len = hb_min (len, (صحیح) حجم (buf) - 1);
    strncpy (buf, name, len);
    buf[len] = '\0';
    *glyph = FT_Get_Name_اشاریہ (ft_face, buf);
  }

  اگر (*glyph == 0)
  {
    /* Check whether the given name was actually the name of glyph 0. */
    حرف buf[128];
    اگر (!FT_Get_Glyph_Name(ft_face, 0, buf, حجم (buf)) &&
	len < 0 ? !strcmp (buf, name) : !لڑی_موازنہ (buf, name, len))
      واپس سچ;
  }

  واپس *glyph != 0;
}

ساکن hb_bool_t
hb_ft_get_font_h_extents (hb_font_t *font HB_غیر_مستعمل,
			  خالی *font_ڈیٹا,
			  hb_font_extents_t *metrics,
			  خالی *user_ڈیٹا HB_غیر_مستعمل)
{
  ثابت hb_ft_font_t *ft_font = (ثابت hb_ft_font_t *) font_ڈیٹا;
  hb_ft_hb_font_check_بدلا (font, ft_font);

  hb_تالا_t lock (ft_font->lock);
  FT_Face ft_face = ft_font->ft_face;
  اعشاری y_mult;
#اگر_تعریف_شدہ HAVE_FT_GET_TRANSFORM
  اگر (ft_font->transform)
  {
    FT_میٹرکس میٹرکس;
    FT_Get_Transform (ft_face, &میٹرکس, خالی_اشاریہ);
    y_mult = sqrtf ((اعشاری)میٹرکس.yx * میٹرکس.yx + (اعشاری)میٹرکس.yy * میٹرکس.yy) / 65536.f;
    y_mult *= font->y_scale < 0 ? -1 : +1;
  }
  ورنہ
#اگر_ختم
  {
    y_mult = font->y_scale < 0 ? -1 : +1;
  }

  اگر (ft_face->units_فی_EM != 0)
  {
    دوہرا ascender = FT_MulFix(ft_face->ascender, ft_face->سائز->metrics.y_scale);
    دوہرا descender = FT_MulFix(ft_face->descender, ft_face->سائز->metrics.y_scale);
    دوہرا اونچائی = FT_MulFix( ft_face->اونچائی, ft_face->سائز->metrics.y_scale );
    metrics->ascender = hb_clamp_مقصد<hb_position_t> ((دوہرا) y_mult * ascender);
    metrics->descender = hb_clamp_مقصد<hb_position_t> ((دوہرا) y_mult * descender);
    metrics->سطر_gap = hb_clamp_مقصد<hb_position_t> ((دوہرا) y_mult * (اونچائی - (ascender - descender)));
  }
  ورنہ
  {
    /* Bitmap-only font, eg. color bitmap font. */
    دوہرا ascender = ft_face->سائز->metrics.ascender;
    دوہرا descender = ft_face->سائز->metrics.descender;
    دوہرا اونچائی = ft_face->سائز->metrics.اونچائی;
    metrics->ascender = hb_clamp_مقصد<hb_position_t> ((دوہرا) y_mult * ascender);
    metrics->descender = hb_clamp_مقصد<hb_position_t> ((دوہرا) y_mult * descender);
    metrics->سطر_gap = hb_clamp_مقصد<hb_position_t> ((دوہرا) y_mult * (اونچائی - (ascender - descender)));
  }

  واپس سچ;
}

#اگر_غیر_تعریف_شدہ HB_NO_DRAW

ساکن صحیح
hb_ft_move_مقصد (ثابت FT_سمتیہ *to,
		خالی *arg)
{
  hb_draw_session_t *drawing = (hb_draw_session_t *) arg;
  drawing->move_مقصد (to->x, to->y);
  واپس FT_Err_Ok;
}

ساکن صحیح
hb_ft_سطر_مقصد (ثابت FT_سمتیہ *to,
		خالی *arg)
{
  hb_draw_session_t *drawing = (hb_draw_session_t *) arg;
  drawing->سطر_مقصد (to->x, to->y);
  واپس FT_Err_Ok;
}

ساکن صحیح
hb_ft_conic_مقصد (ثابت FT_سمتیہ *control,
		 ثابت FT_سمتیہ *to,
		 خالی *arg)
{
  hb_draw_session_t *drawing = (hb_draw_session_t *) arg;
  drawing->quadratic_مقصد (control->x, control->y,
			 to->x, to->y);
  واپس FT_Err_Ok;
}

ساکن صحیح
hb_ft_cubic_مقصد (ثابت FT_سمتیہ *control1,
		 ثابت FT_سمتیہ *control2,
		 ثابت FT_سمتیہ *to,
		 خالی *arg)
{
  hb_draw_session_t *drawing = (hb_draw_session_t *) arg;
  drawing->cubic_مقصد (control1->x, control1->y,
		     control2->x, control2->y,
		     to->x, to->y);
  واپس FT_Err_Ok;
}

ساکن hb_bool_t
hb_ft_draw_glyph_or_fail (hb_font_t *font,
			  خالی *font_ڈیٹا,
			  hb_codepoint_t glyph,
			  hb_draw_funcs_t *draw_funcs, خالی *draw_ڈیٹا,
			  خالی *user_ڈیٹا HB_غیر_مستعمل)
{
  ثابت hb_ft_font_t *ft_font = (ثابت hb_ft_font_t *) font_ڈیٹا;
  hb_ft_hb_font_check_بدلا (font, ft_font);

  hb_تالا_t lock (ft_font->lock);
  FT_Face ft_face = ft_font->ft_face;

  اگر (unlikely (FT_بھرو_Glyph (ft_face, glyph,
			       FT_بھرو_NO_BITMAP | ft_font->بھرو_flags)))
    واپس جھوٹ;

  اگر (ft_face->glyph->format != FT_GLYPH_FORMAT_OUTLINE)
    واپس جھوٹ;

  ثابت FT_Outline_Funcs outline_funcs = {
    hb_ft_move_مقصد,
    hb_ft_سطر_مقصد,
    hb_ft_conic_مقصد,
    hb_ft_cubic_مقصد,
    0, /*سرکاؤ*/
    0, /* delta */
  };

  hb_draw_session_t draw_session {draw_funcs, draw_ڈیٹا};

  FT_Outline_Decompose (&ft_face->glyph->outline,
			&outline_funcs,
			&draw_session);

  واپس سچ;
}
#اگر_ختم

#اگر_غیر_تعریف_شدہ HB_NO_PAINT
#اگر (FREETYPE_MAJOR*10000 + FREETYPE_MINOR*100 + FREETYPE_PATCH) >= 21300

#شامل "hb-ft-colr.hh"

ساکن hb_bool_t
hb_ft_paint_glyph_or_fail (hb_font_t *font,
			   خالی *font_ڈیٹا,
			   hb_codepoint_t gid,
			   hb_paint_funcs_t *paint_funcs, خالی *paint_ڈیٹا,
			   غیرعلامتی صحیح palette_اشاریہ,
			   hb_color_t foreground,
			   خالی *user_ڈیٹا)
{
  ثابت hb_ft_font_t *ft_font = (ثابت hb_ft_font_t *) font_ڈیٹا;
  hb_ft_hb_font_check_بدلا (font, ft_font);

  hb_تالا_t lock (ft_font->lock);
  FT_Face ft_face = ft_font->ft_face;

  FT_Long بھرو_flags = ft_font->بھرو_flags | FT_بھرو_COLOR;
#اگر (FREETYPE_MAJOR*10000 + FREETYPE_MINOR*100 + FREETYPE_PATCH) >= 21301
  بھرو_flags |= FT_بھرو_NO_SVG;
#اگر_ختم

  /* We release the lock before calling into glyph callbacks, such that
   * eg. draw API can call back into the face.*/

  اگر (unlikely (FT_بھرو_Glyph (ft_face, gid, بھرو_flags)))
    واپس جھوٹ;

  اگر (ft_face->glyph->format == FT_GLYPH_FORMAT_OUTLINE)
  {
    اگر (hb_ft_paint_glyph_colr (font, font_ڈیٹا, gid,
				paint_funcs, paint_ڈیٹا,
				palette_اشاریہ, foreground,
				user_ڈیٹا))
      واپس سچ;

    // Outline glyph
    واپس جھوٹ;
  }

  خودکار *glyph = ft_face->glyph;
  اگر (glyph->format == FT_GLYPH_FORMAT_BITMAP)
  {
    منطقی ret = جھوٹ;
    خودکار &bitmap = glyph->bitmap;
    اگر (bitmap.pixel_منوال == FT_PIXEL_منوال_BGRA)
    {
      اگر (bitmap.pitch != (علامتی) bitmap.چوڑائی * 4)
        واپس ret;

      ft_font->lock.unlock ();

      hb_blob_t *blob = hb_blob_create ((ثابت حرف *) bitmap.بفر,
					bitmap.pitch * bitmap.صفیں,
					HB_حافظہ_منوال_DUPLICATE,
					خالی_اشاریہ, خالی_اشاریہ);

      hb_glyph_extents_t extents;
      اگر (!font->get_glyph_extents (gid, &extents, جھوٹ))
	جاؤ برآمد;

      اگر (paint_funcs->image (paint_ڈیٹا,
			      blob,
			      bitmap.چوڑائی,
			      bitmap.صفیں,
			      HB_PAINT_IMAGE_FORMAT_BGRA,
			      0.f,
			      &extents))
        ret = سچ;

    برآمد:
      hb_blob_destroy (blob);
      ft_font->lock.lock ();
    }

    واپس ret;
  }
  واپس جھوٹ;
}
#اگر_ختم
#اگر_ختم


ساکن اندرونی خالی free_static_ft_funcs ();

ساکن ساخت hb_ft_font_funcs_lazy_loader_t : hb_font_funcs_lazy_loader_t<hb_ft_font_funcs_lazy_loader_t>
{
  ساکن hb_font_funcs_t *create ()
  {
    hb_font_funcs_t *funcs = hb_font_funcs_create ();

    hb_font_funcs_سیٹ_nominal_glyph_func (funcs, hb_ft_get_nominal_glyph, خالی_اشاریہ, خالی_اشاریہ);
    hb_font_funcs_سیٹ_nominal_glyphs_func (funcs, hb_ft_get_nominal_glyphs, خالی_اشاریہ, خالی_اشاریہ);
    hb_font_funcs_سیٹ_variation_glyph_func (funcs, hb_ft_get_variation_glyph, خالی_اشاریہ, خالی_اشاریہ);

    hb_font_funcs_سیٹ_font_h_extents_func (funcs, hb_ft_get_font_h_extents, خالی_اشاریہ, خالی_اشاریہ);
    hb_font_funcs_سیٹ_glyph_h_advances_func (funcs, hb_ft_get_glyph_h_advances, خالی_اشاریہ, خالی_اشاریہ);

#اگر_غیر_تعریف_شدہ HB_NO_VERTICAL
    hb_font_funcs_سیٹ_glyph_v_advance_func (funcs, hb_ft_get_glyph_v_advance, خالی_اشاریہ, خالی_اشاریہ);
    hb_font_funcs_سیٹ_glyph_v_origin_func (funcs, hb_ft_get_glyph_v_origin, خالی_اشاریہ, خالی_اشاریہ);
#اگر_ختم

#اگر_غیر_تعریف_شدہ HB_NO_OT_SHAPE_FALLBACK
    hb_font_funcs_سیٹ_glyph_h_kerning_func (funcs, hb_ft_get_glyph_h_kerning, خالی_اشاریہ, خالی_اشاریہ);
#اگر_ختم
    hb_font_funcs_سیٹ_glyph_extents_func (funcs, hb_ft_get_glyph_extents, خالی_اشاریہ, خالی_اشاریہ);
    hb_font_funcs_سیٹ_glyph_contour_نقطہ_func (funcs, hb_ft_get_glyph_contour_نقطہ, خالی_اشاریہ, خالی_اشاریہ);
    hb_font_funcs_سیٹ_glyph_name_func (funcs, hb_ft_get_glyph_name, خالی_اشاریہ, خالی_اشاریہ);
    hb_font_funcs_سیٹ_glyph_from_name_func (funcs, hb_ft_get_glyph_from_name, خالی_اشاریہ, خالی_اشاریہ);

#اگر_غیر_تعریف_شدہ HB_NO_DRAW
    hb_font_funcs_سیٹ_draw_glyph_or_fail_func (funcs, hb_ft_draw_glyph_or_fail, خالی_اشاریہ, خالی_اشاریہ);
#اگر_ختم

#اگر_غیر_تعریف_شدہ HB_NO_PAINT
#اگر (FREETYPE_MAJOR*10000 + FREETYPE_MINOR*100 + FREETYPE_PATCH) >= 21300
    hb_font_funcs_سیٹ_paint_glyph_or_fail_func (funcs, hb_ft_paint_glyph_or_fail, خالی_اشاریہ, خالی_اشاریہ);
#اگر_ختم
#اگر_ختم

    hb_font_funcs_make_immutable (funcs);

    hb_atexit (free_static_ft_funcs);

    واپس funcs;
  }
} static_ft_funcs;

ساکن اندرونی
خالی free_static_ft_funcs ()
{
  static_ft_funcs.free_instance ();
}

ساکن hb_font_funcs_t *
_hb_ft_get_font_funcs ()
{
  واپس static_ft_funcs.get_unconst ();
}

ساکن خالی
hb_ft_font_سیٹ_funcs_2 (hb_font_t *font, FT_Face ft_face, منطقی unref,
		       منطقی static_library = جھوٹ)
{
  منطقی علامت = ft_face->charmap && ft_face->charmap->encoding == FT_ENCODING_MS_علامت;

  hb_ft_font_t *ft_font = _hb_ft_font_create (ft_face, علامت, unref, static_library);
  اگر (unlikely (!ft_font)) واپس;

  hb_font_سیٹ_funcs (font,
		     _hb_ft_get_font_funcs (),
		     ft_font,
		     _hb_ft_font_destroy);
}


ساکن hb_blob_t *
hb_ft_reference_جدول (hb_face_t *face HB_غیر_مستعمل, hb_لیبل_t لیبل_2, خالی *user_ڈیٹا)
{
  FT_Face ft_face = (FT_Face) user_ڈیٹا;
  FT_Byte *بفر;
  FT_ULong  لمبائی = 0;
  FT_Error error;

  /* In new FreeType, a tag value of 1 loads the SFNT table directory. Reject it. */
  اگر (لیبل_2 == 1)
    واپس خالی_اشاریہ;

  /* Note: FreeType like HarfBuzz uses the NONE tag for fetching the entire blob */

  error = FT_بھرو_Sfnt_جدول (ft_face, لیبل_2, 0, خالی_اشاریہ, &لمبائی);
  اگر (error)
    واپس خالی_اشاریہ;

  بفر = (FT_Byte *) hb_malloc (لمبائی);
  اگر (!بفر)
    واپس خالی_اشاریہ;
  خودکار بفر_guard = hb_make_scope_guard ([&]() { hb_free (بفر); });

  error = FT_بھرو_Sfnt_جدول (ft_face, لیبل_2, 0, بفر, &لمبائی);
  اگر (error)
    واپس خالی_اشاریہ;

  بفر_guard.release ();
  واپس hb_blob_create ((ثابت حرف *) بفر, لمبائی,
			 HB_حافظہ_منوال_WRITABLE,
			 بفر, hb_free);
}

ساکن غیرعلامتی
hb_ft_get_جدول_لیبل_فہرست (ثابت hb_face_t *face HB_غیر_مستعمل,
		       غیرعلامتی صحیح آغاز_انحراف,
		       غیرعلامتی صحیح *جدول_گنتی_2,
		       hb_لیبل_t *جدول_لیبل_فہرست,
		       خالی *user_ڈیٹا)
{
  FT_Face ft_face = (FT_Face) user_ڈیٹا;

  FT_ULong population = 0;
  FT_Sfnt_جدول_Info (ft_face,
		      0, // table_index; ignored
		      خالی_اشاریہ,
                      &population);

  اگر (!جدول_گنتی_2)
    واپس population;

  اگر (unlikely (آغاز_انحراف >= population))
  {
    *جدول_گنتی_2 = 0;
    واپس population;
  }

  غیرعلامتی انتہا_انحراف = آغاز_انحراف + *جدول_گنتی_2;
  اگر (unlikely (انتہا_انحراف < آغاز_انحراف)) /* Overflow. */
  {
    *جدول_گنتی_2 = 0;
    واپس population;
  }
  انتہا_انحراف = hb_min (انتہا_انحراف, (غیرعلامتی) population);

  *جدول_گنتی_2 = انتہا_انحراف - آغاز_انحراف;
  اگر (جدول_لیبل_فہرست)
    برائے (غیرعلامتی i = آغاز_انحراف; i < انتہا_انحراف; i++)
    {
      FT_ULong لیبل_2 = 0, لمبائی;
      FT_Sfnt_جدول_Info (ft_face, i, &لیبل_2, &لمبائی);
      جدول_لیبل_فہرست[i - آغاز_انحراف] = لیبل_2;
    }

  واپس population;
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

  اگر (!ft_face->دھارا->اقرأ) {
    hb_blob_t *blob;

    blob = hb_blob_create ((ثابت حرف *) ft_face->دھارا->بنیاد,
			   (غیرعلامتی صحیح) ft_face->دھارا->سائز,
			   HB_حافظہ_منوال_READONLY,
			   ft_face, destroy);
    face = hb_face_create (blob, ft_face->face_اشاریہ);
    hb_blob_destroy (blob);
  } ورنہ {
    face = hb_face_create_for_tables (hb_ft_reference_جدول, ft_face, destroy);
    hb_face_سیٹ_get_جدول_لیبل_فہرست_func (face, hb_ft_get_جدول_لیبل_فہرست, ft_face, خالی_اشاریہ);
  }

  hb_face_سیٹ_اشاریہ (face, ft_face->face_اشاریہ);
  hb_face_سیٹ_upem (face, ft_face->units_فی_EM);

  واپس face;
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
  واپس hb_ft_face_create (ft_face, _hb_ft_face_destroy);
}

ساکن خالی
hb_ft_face_finalize (خالی *arg)
{
  FT_Face ft_face = (FT_Face) arg;
  hb_face_destroy ((hb_face_t *) ft_face->generic.ڈیٹا);
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
  اگر (unlikely (!ft_face->generic.ڈیٹا || ft_face->generic.finalizer != hb_ft_face_finalize))
  {
    اگر (ft_face->generic.finalizer)
      ft_face->generic.finalizer (ft_face);

    ft_face->generic.ڈیٹا = hb_ft_face_create (ft_face, خالی_اشاریہ);
    ft_face->generic.finalizer = hb_ft_face_finalize;
  }

  واپس hb_face_reference ((hb_face_t *) ft_face->generic.ڈیٹا);
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
  hb_ft_font_سیٹ_funcs_2 (font, ft_face, جھوٹ);
  hb_ft_font_بدلا (font);
  واپس font;
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
خالی
hb_ft_font_بدلا (hb_font_t *font)
{
  اگر (font->destroy != (hb_destroy_func_t) _hb_ft_font_destroy)
    واپس;

  hb_ft_font_t *ft_font = (hb_ft_font_t *) font->user_ڈیٹا;

  FT_Face ft_face = ft_font->ft_face;

  hb_font_سیٹ_scale (font,
		     (صحیح) (((uint64_t) ft_face->سائز->metrics.x_scale * (uint64_t) ft_face->units_فی_EM + (1u<<15)) >> 16),
		     (صحیح) (((uint64_t) ft_face->سائز->metrics.y_scale * (uint64_t) ft_face->units_فی_EM + (1u<<15)) >> 16));
#اگر 0 /* hb-ft works in no-hinting model */
  hb_font_سیٹ_ppem (font,
		    ft_face->سائز->metrics.x_ppem,
		    ft_face->سائز->metrics.y_ppem);
#اگر_ختم

#اگر defined(HAVE_FT_GET_VAR_BLEND_COORDINATES) && !defined(HB_NO_VAR)
  FT_MM_Var *mm_var = خالی_اشاریہ;
  اگر (!FT_Get_MM_Var (ft_face, &mm_var))
  {
    FT_Fixed *ft_coords = (FT_Fixed *) hb_calloc (mm_var->عدد_axis, حجم (FT_Fixed));
    صحیح *coords = (صحیح *) hb_calloc (mm_var->عدد_axis, حجم (صحیح));
    اگر (coords && ft_coords)
    {
      اگر (!FT_Get_Var_Blend_Coordinates (ft_face, mm_var->عدد_axis, ft_coords))
      {
	منطقی nonzero = جھوٹ;

	برائے (غیرعلامتی صحیح i = 0; i < mm_var->عدد_axis; ++i)
	 {
	  coords[i] = (ft_coords[i] + 2) >> 2;
	  nonzero = nonzero || coords[i];
	 }

	اگر (nonzero)
	  hb_font_سیٹ_var_coords_normalized (font, coords, mm_var->عدد_axis);
	ورنہ
	  hb_font_سیٹ_var_coords_normalized (font, خالی_اشاریہ, 0);
      }
    }
    hb_free (coords);
    hb_free (ft_coords);
#اگر_تعریف_شدہ HAVE_FT_مکمل_MM_VAR
    FT_مکمل_MM_Var (ft_face->glyph->library, mm_var);
#ورنہ
    hb_free (mm_var);
#اگر_ختم
  }
#اگر_ختم

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
hb_ft_hb_font_بدلا_2 (hb_font_t *font)
{
  اگر (font->destroy != (hb_destroy_func_t) _hb_ft_font_destroy)
    واپس جھوٹ;

  hb_ft_font_t *ft_font = (hb_ft_font_t *) font->user_ڈیٹا;

  واپس hb_ft_hb_font_check_بدلا (font, ft_font);
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
  واپس hb_ft_font_create (ft_face, _hb_ft_face_destroy);
}


ساکن خالی * _hb_ft_alloc (FT_حافظہ حافظہ, طویل سائز)
{ واپس hb_malloc (سائز); }

ساکن خالی _hb_ft_free (FT_حافظہ حافظہ, خالی *بلاک)
{ hb_free (بلاک); }

ساکن خالی * _hb_ft_realloc (FT_حافظہ حافظہ, طویل cur_سائز, طویل new_سائز, خالی *بلاک)
{ واپس hb_realloc (بلاک, new_سائز); }

ساکن FT_حافظہ_Rec m =
{
  خالی_اشاریہ,
  _hb_ft_alloc,
  _hb_ft_free,
  _hb_ft_realloc
};

ساکن اندرونی خالی free_static_ft_library ();
ساکن اندرونی خالی free_static_ft_library_mutex ();

ساکن ساخت hb_ft_library_mutex_lazy_loader_t : hb_lazy_loader_t<hb_mutex_t,
								   hb_ft_library_mutex_lazy_loader_t>
{
  ساکن hb_mutex_t *create ()
  {
    hb_mutex_t *lock = (hb_mutex_t *) hb_calloc (1, حجم (hb_mutex_t));
    اگر (unlikely (!lock))
      واپس خالی_اشاریہ;

    lock = نیا (lock) hb_mutex_t;
    hb_atexit (free_static_ft_library_mutex);
    واپس lock;
  }
  ساکن خالی destroy (hb_mutex_t *lock)
  {
    lock->~hb_mutex_t ();
    hb_free (lock);
  }
  ساکن hb_mutex_t *get_null ()
  {
    واپس خالی_اشاریہ;
  }
} static_ft_library_mutex;

ساکن ساخت hb_ft_library_lazy_loader_t : hb_lazy_loader_t<hb_remove_اشارہ_گر<FT_Library>,
							     hb_ft_library_lazy_loader_t>
{
  ساکن FT_Library create ()
  {
    FT_Library l;
    اگر (FT_New_Library (&m, &l))
      واپس خالی_اشاریہ;

    FT_جوڑو_Default_Modules (l);
    FT_سیٹ_Default_Properties (l);

    hb_atexit (free_static_ft_library);

    واپس l;
  }
  ساکن خالی destroy (FT_Library l)
  {
    FT_مکمل_Library (l);
  }
  ساکن FT_Library get_null ()
  {
    واپس خالی_اشاریہ;
  }
} static_ft_library;

ساکن اندرونی
خالی free_static_ft_library_mutex ()
{
  static_ft_library_mutex.free_instance ();
}

ساکن اندرونی
خالی free_static_ft_library ()
{
  hb_تالا_t lock (static_ft_library_mutex.get_unconst ());
  static_ft_library.free_instance ();
}

ساکن FT_Library
reference_ft_library ()
{
  hb_mutex_t *mutex = static_ft_library_mutex.get_unconst ();
  اگر (unlikely (!mutex))
    واپس خالی_اشاریہ;
  hb_تالا_t lock (mutex);
  FT_Library l = static_ft_library.get_unconst ();
  اگر (unlikely (FT_Reference_Library (l)))
  {
    DEBUG_پیغام (FT, l, "FT_Reference_Library() failed");
    واپس خالی_اشاریہ;
  }
  واپس l;
}

ساکن hb_user_ڈیٹا_کلید_t ft_library_کلید = {0};

ساکن خالی
_hb_ft_face_destroy_static (خالی *ڈیٹا)
{
  hb_تالا_t lock (static_ft_library_mutex.get_unconst ());
  FT_مکمل_Face ((FT_Face) ڈیٹا);
}

ساکن FT_Error
new_ft_face (FT_Library  ft_library,
	     ثابت حرف *file_name,
	     غیرعلامتی صحیح اشاریہ_2,
	     FT_Face     *ft_face)
{
  hb_تالا_t lock (static_ft_library_mutex.get_unconst ());
  واپس FT_New_Face (ft_library, file_name, اشاریہ_2, ft_face);
}

ساکن FT_Error
new_ft_حافظہ_face (FT_Library    ft_library,
		    ثابت FT_Byte *blob_ڈیٹا,
		    غیرعلامتی صحیح   blob_سائز,
		    غیرعلامتی صحیح   اشاریہ_2,
		    FT_Face       *ft_face)
{
  hb_تالا_t lock (static_ft_library_mutex.get_unconst ());
  واپس FT_New_حافظہ_Face (ft_library, blob_ڈیٹا, blob_سائز, اشاریہ_2, ft_face);
}

ساکن خالی
finalize_ft_library (خالی *arg)
{
  FT_Face ft_face = (FT_Face) arg;
  FT_مکمل_Library ((FT_Library) ft_face->generic.ڈیٹا);
}

ساکن خالی
destroy_ft_library (خالی *arg)
{
  hb_تالا_t lock (static_ft_library_mutex.get_unconst ());
  FT_مکمل_Library ((FT_Library) arg);
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
hb_ft_face_create_from_file_or_fail (ثابت حرف   *file_name,
				     غیرعلامتی صحیح  اشاریہ_2)
{
  FT_Library ft_library = reference_ft_library ();
  اگر (unlikely (!ft_library))
  {
    DEBUG_پیغام (FT, ft_library, "reference_ft_library failed");
    واپس خالی_اشاریہ;
  }

  FT_Face ft_face;
  اگر (unlikely (new_ft_face (ft_library,
			     file_name,
			     اشاریہ_2,
			     &ft_face)))
  {
    destroy_ft_library (ft_library);
    واپس خالی_اشاریہ;
  }

  FT_Reference_Face (ft_face);
  hb_face_t *face = hb_ft_face_create (ft_face, _hb_ft_face_destroy_static);
  _hb_ft_face_destroy_static (ft_face);

  ft_face->generic.ڈیٹا = ft_library;
  ft_face->generic.finalizer = finalize_ft_library;

  اگر (hb_face_is_immutable (face))
    واپس خالی_اشاریہ;

  واپس face;
}

ساکن hb_user_ڈیٹا_کلید_t ft_blob_کلید = {0};

ساکن خالی
_destroy_blob (خالی *p)
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
				     غیرعلامتی صحیح  اشاریہ_2)
{
  FT_Library ft_library = reference_ft_library ();
  اگر (unlikely (!ft_library))
  {
    DEBUG_پیغام (FT, ft_library, "reference_ft_library failed");
    واپس خالی_اشاریہ;
  }

  hb_blob_make_immutable (blob);
  غیرعلامتی blob_سائز;
  ثابت حرف *blob_ڈیٹا = hb_blob_get_ڈیٹا (blob, &blob_سائز);

  FT_Face ft_face;
  اگر (unlikely (new_ft_حافظہ_face (ft_library,
				    (ثابت FT_Byte *) blob_ڈیٹا,
				    blob_سائز,
				    اشاریہ_2,
				    &ft_face)))
  {
    destroy_ft_library (ft_library);
    واپس خالی_اشاریہ;
  }

  FT_Reference_Face (ft_face);
  hb_face_t *face = hb_ft_face_create (ft_face, _hb_ft_face_destroy_static);
  _hb_ft_face_destroy_static (ft_face);

  ft_face->generic.ڈیٹا = ft_library;
  ft_face->generic.finalizer = finalize_ft_library;

  اگر (hb_face_is_immutable (face))
    واپس خالی_اشاریہ;

  // Hook the blob to the hb_face_t, since FT_Face still needs it.
  hb_blob_reference (blob);
  اگر (!hb_face_سیٹ_user_ڈیٹا (face, &ft_blob_کلید, blob, _destroy_blob, سچ))
  {
    hb_blob_destroy (blob);
    hb_face_destroy (face);
    واپس خالی_اشاریہ;
  }

  واپس face;
}

ساکن خالی
_release_blob (خالی *arg)
{
  FT_Face ft_face = (FT_Face) arg;
  hb_blob_destroy ((hb_blob_t *) ft_face->generic.ڈیٹا);
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
خالی
hb_ft_font_سیٹ_funcs (hb_font_t *font)
{
  صحیح بھرو_flags = FT_بھرو_DEFAULT | FT_بھرو_NO_HINTING;
  اگر (font->destroy == (hb_destroy_func_t) _hb_ft_font_destroy && font->user_ڈیٹا)
  {
    ثابت hb_ft_font_t *existing_ft_font = (ثابت hb_ft_font_t *) font->user_ڈیٹا;
    بھرو_flags = existing_ft_font->بھرو_flags;
  }

  // In case of failure...
  hb_font_سیٹ_funcs (font,
		     hb_font_funcs_get_خالی_خانے (),
		     خالی_اشاریہ, خالی_اشاریہ);

  hb_blob_t *blob = hb_face_reference_blob (font->face);
  غیرعلامتی صحیح blob_لمبائی;
  ثابت حرف *blob_ڈیٹا = hb_blob_get_ڈیٹا (blob, &blob_لمبائی);
  اگر (unlikely (!blob_لمبائی))
    DEBUG_پیغام (FT, font, "Font face has empty blob");

  FT_Library ft_library = reference_ft_library ();
  اگر (unlikely (!ft_library))
  {
    hb_blob_destroy (blob);
    DEBUG_پیغام (FT, font, "reference_ft_library failed");
    واپس;
  }

  FT_Face ft_face = خالی_اشاریہ;
  اگر (unlikely (new_ft_حافظہ_face (ft_library,
				    (ثابت FT_Byte *) blob_ڈیٹا,
				    blob_لمبائی,
				    hb_face_get_اشاریہ (font->face),
				    &ft_face)))
  {
    hb_blob_destroy (blob);
    destroy_ft_library (ft_library);
    DEBUG_پیغام (FT, font, "FT_New_Memory_Face() failed");
    واپس;
  }

  اگر (FT_Select_Charmap (ft_face, FT_ENCODING_MS_علامت))
    FT_Select_Charmap (ft_face, FT_ENCODING_UNICODE);

  // Hook the blob to the FT_Face
  ft_face->generic.ڈیٹا = blob;
  ft_face->generic.finalizer = _release_blob;

  // And the FT_Library to the blob
  اگر (unlikely (!hb_blob_سیٹ_user_ڈیٹا (blob, &ft_library_کلید, ft_library, destroy_ft_library, سچ)))
  {
    DEBUG_پیغام (FT, font, "hb_blob_set_user_data() failed");
    _hb_ft_face_destroy_static (ft_face);
    destroy_ft_library (ft_library);
    واپس;
  }

  hb_ft_font_سیٹ_funcs_2 (font, ft_face, سچ, سچ);
  hb_ft_font_سیٹ_بھرو_flags (font, بھرو_flags);

  hb_ft_hb_font_بدلا (font, ft_face);
}

#اگر_ختم
