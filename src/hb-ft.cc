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

#ضمن "hb.hh"

#جے_تعریف_شدہ HAVE_FREETYPE

#ضمن "hb-ft.h"

#ضمن "hb-cache.hh"
#ضمن "hb-draw.hh"
#ضمن "hb-font.hh"
#ضمن "hb-machinery.hh"
#ضمن "hb-ot-os2-table.hh"
#ضمن "hb-ot-shaper-arabic-pua.hh"
#ضمن "hb-paint.hh"

#ضمن FT_MODULE_H
#ضمن FT_ADVANCES_H
#ضمن FT_مضرب_MASTERS_H
#ضمن FT_OUTLINE_H
#ضمن FT_TRUETYPE_TABLES_H
#ضمن FT_SYNTHESIS_H
#إذا (FREETYPE_MAJOR*10000 + FREETYPE_MINOR*100 + FREETYPE_PATCH) >= 21300
#ضمن FT_COLOR_H
#جے_انت


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


ورتوں hb_ft_advance_cache_t = hb_cache_t<16, 24, 8, خطأ>;

ساكن فراغ _hb_ft_face_destroy_static (فراغ *انکڑے);

بنية hb_ft_font_t
{
  صحيح لدو_flags;
  منطقي علامت; /* Whether selected cmap is symbol cmap. */
  منطقي unref; /* Whether to destroy ft_face when done. */
  منطقي transform; /* Whether to apply FT_Face's transform. */
  منطقي static_library; /* Whether ft_face uses static_ft_library. */

  بدلنجوگ hb_mutex_t lock; /* Protects members below. */
  FT_Face ft_face;
  بدلنجوگ hb_atomic_t<بنانشانی> cached_serial;
  بدلنجوگ hb_ft_advance_cache_t advance_cache;
};

ساكن hb_ft_font_t *
_hb_ft_font_create (FT_Face ft_face, منطقي علامت, منطقي unref, منطقي static_library)
{
  hb_ft_font_t *ft_font = (hb_ft_font_t *) hb_calloc (1, حجم (hb_ft_font_t));
  إذا (unlikely (!ft_font)) أرجع خالی_سوچک;

  ft_font->lock.init ();
  ft_font->ft_face = ft_face;
  ft_font->علامت = علامت;
  ft_font->unref = unref;
  ft_font->static_library = static_library;

  ft_font->لدو_flags = FT_لدو_DEFAULT | FT_لدو_NO_HINTING;

  ft_font->cached_serial = UINT_MAX;
  جديد (&ft_font->advance_cache) hb_ft_advance_cache_t;

  أرجع ft_font;
}

ساكن فراغ
_hb_ft_face_destroy (فراغ *انکڑے)
{
  FT_پورا_Face ((FT_Face) انکڑے);
}

ساكن فراغ
_hb_ft_font_destroy (فراغ *انکڑے)
{
  hb_ft_font_t *ft_font = (hb_ft_font_t *) انکڑے;

  إذا (ft_font->unref)
  {
    إذا (ft_font->static_library)
    {
      /* ft_font->ft_face->generic.data is the blob that hb_ft_font_set_funcs()
       * hooked up as this internal FT_Face's finalizer target (_release_blob).
       * FT_Done_Face() below runs that finalizer while static_ft_library_mutex
       * is held; if dropping its reference were to free the blob right there,
       * the blob's attached destroy_ft_library() user-data callback would try
       * to re-lock the same non-recursive mutex and self-deadlock. Hold an
       * extra reference across the locked call so the blob (and thus that
       * callback) can only be freed after the lock is released. */
      hb_blob_t *blob = hb_blob_reference ((hb_blob_t *) ft_font->ft_face->generic.انکڑے);
      _hb_ft_face_destroy_static (ft_font->ft_face);
      hb_blob_destroy (blob);
    }
    وإلا
      _hb_ft_face_destroy (ft_font->ft_face);
  }

  ft_font->lock.fini ();

  hb_free (ft_font);
}


/* hb_font changed, update FT_Face. */
ساكن فراغ hb_ft_hb_font_بدلیا (hb_font_t *font, FT_Face ft_face)
{
  إذا (unlikely (font->destroy != (hb_destroy_func_t) _hb_ft_font_destroy))
    أرجع;

  hb_ft_font_t *ft_font = (hb_ft_font_t *) font->user_انکڑے;

  عائم x_mult = 1.f, y_mult = 1.f;

  إذا (font->x_scale < 0) x_mult = -x_mult;
  إذا (font->y_scale < 0) y_mult = -y_mult;

  إذا (FT_سیٹ_Char_ناپ (ft_face,
			مطلق_قدر (font->x_scale), مطلق_قدر (font->y_scale),
			0, 0
#إذا 0
		    font->x_ppem * 72 * 64 / font->x_scale,
		    font->y_ppem * 72 * 64 / font->y_scale
#جے_انت
     ) && ft_face->عدد_fixed_sizes)
  {
#جے_تعریف_شدہ HAVE_FT_GET_TRANSFORM
    /* Bitmap font, eg. bitmap color emoji. */
    /* Pick largest size? */
    صحيح x_scale  = ft_face->دستیاب_sizes[ft_face->عدد_fixed_sizes - 1].x_ppem;
    صحيح y_scale = ft_face->دستیاب_sizes[ft_face->عدد_fixed_sizes - 1].y_ppem;
    FT_سیٹ_Char_ناپ (ft_face,
		      x_scale, y_scale,
		      0, 0);

    /* This contains the sign that was previously in x_mult/y_mult. */
    x_mult = (عائم) font->x_scale / x_scale;
    y_mult = (عائم) font->y_scale / y_scale;
#جے_انت
  }
  وإلا
  { /* Shrug */ }


  إذا (x_mult != 1.f || y_mult != 1.f)
  {
    FT_میٹرکس میٹرکس = { (صحيح) roundf (x_mult * (1<<16)), 0,
			  0, (صحيح) roundf (y_mult * (1<<16))};
    FT_سیٹ_Transform (ft_face, &میٹرکس, خالی_سوچک);
    ft_font->transform = سچ;
  }
  وإلا
    FT_سیٹ_Transform (ft_face, خالی_سوچک, خالی_سوچک);

#إذا defined(HAVE_FT_GET_VAR_BLEND_COORDINATES) && !defined(HB_NO_VAR)
  إذا (font->has_nonzero_coords)
  {
    بنانشانی صحيح عدد_coords;
    ثابت عائم *coords = hb_font_get_var_coords_design (font, &عدد_coords);
    FT_Fixed *ft_coords = (FT_Fixed *) hb_calloc (عدد_coords, حجم (FT_Fixed));
    إذا (ft_coords)
    {
      لكل (بنانشانی صحيح i = 0; i < عدد_coords; i++)
	  ft_coords[i] = coords[i] * 65536.f;
      FT_سیٹ_Var_Design_Coordinates (ft_face, عدد_coords, ft_coords);
      hb_free (ft_coords);
    }
  }
  وإلا إذا (font->عدد_coords)
  {
    // Some old versions of FreeType crash if we
    // call this function on non-variable fonts.
    FT_سیٹ_Var_Design_Coordinates (ft_face, 0, خالی_سوچک);
  }
#جے_انت
}

/* Check if hb_font changed, update FT_Face. */
ساكن مضمن منطقي
hb_ft_hb_font_check_بدلیا (hb_font_t *font,
			      ثابت hb_ft_font_t *ft_font)
{
  إذا (font->serial != ft_font->cached_serial)
  {
    hb_جندرا_t lock (ft_font->lock);
    hb_ft_hb_font_بدلیا (font, ft_font->ft_face);
    ft_font->advance_cache.clear ();
    ft_font->cached_serial.سیٹ_release (font->serial.get_acquire ());
    أرجع سچ;
  }
  أرجع خطأ;
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
فراغ
hb_ft_font_سیٹ_لدو_flags (hb_font_t *font, صحيح لدو_flags)
{
  إذا (hb_object_is_immutable (font))
    أرجع;

  إذا (unlikely (font->destroy != (hb_destroy_func_t) _hb_ft_font_destroy))
    أرجع;

  hb_ft_font_t *ft_font = (hb_ft_font_t *) font->user_انکڑے;

  ft_font->لدو_flags = لدو_flags;
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
صحيح
hb_ft_font_get_لدو_flags (hb_font_t *font)
{
  إذا (unlikely (font->destroy != (hb_destroy_func_t) _hb_ft_font_destroy))
    أرجع 0;

  ثابت hb_ft_font_t *ft_font = (ثابت hb_ft_font_t *) font->user_انکڑے;

  أرجع ft_font->لدو_flags;
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
  إذا (unlikely (font->destroy != (hb_destroy_func_t) _hb_ft_font_destroy))
    أرجع خالی_سوچک;

  ثابت hb_ft_font_t *ft_font = (ثابت hb_ft_font_t *) font->user_انکڑے;

  أرجع ft_font->ft_face;
}

#جے_بنا_تعریف HB_DISABLE_DEPRECATED

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
  أرجع hb_ft_font_get_ft_face (font);
}

#جے_انت

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
hb_ft_font_جندرا_face (hb_font_t *font)
{
  إذا (unlikely (font->destroy != (hb_destroy_func_t) _hb_ft_font_destroy))
    أرجع خالی_سوچک;

  ثابت hb_ft_font_t *ft_font = (ثابت hb_ft_font_t *) font->user_انکڑے;

  ft_font->lock.lock ();

  أرجع ft_font->ft_face;
}

/**
 * hb_ft_font_unlock_face: (skip)
 * @font: #hb_font_t to work upon
 *
 * Releases an FT_Face previously obtained with hb_ft_font_lock_face().
 *
 * Since: 2.6.5
 **/
فراغ
hb_ft_font_unlock_face (hb_font_t *font)
{
  إذا (unlikely (font->destroy != (hb_destroy_func_t) _hb_ft_font_destroy))
    أرجع;

  ثابت hb_ft_font_t *ft_font = (ثابت hb_ft_font_t *) font->user_انکڑے;

  ft_font->lock.unlock ();
}


ساكن hb_bool_t
hb_ft_get_nominal_glyph (hb_font_t *font,
			 فراغ *font_انکڑے,
			 hb_codepoint_t unicode,
			 hb_codepoint_t *glyph,
			 فراغ *user_انکڑے HB_بن_ورتیا)
{
  ثابت hb_ft_font_t *ft_font = (ثابت hb_ft_font_t *) font_انکڑے;
  hb_جندرا_t lock (ft_font->lock);
  بنانشانی صحيح g = FT_Get_Char_اشاریہ (ft_font->ft_face, unicode);

  إذا (unlikely (!g))
  {
    إذا (unlikely (ft_font->علامت))
    {
      بدل ((بنانشانی) font->face->جدول.OS2->get_font_ورقہ ()) {
      حالة OT::OS2::font_ورقہ_t::FONT_ورقہ_NONE:
	إذا (unicode <= 0x00FFu)
	  /* For symbol-encoded OpenType fonts, we duplicate the
	   * U+F000..F0FF range at U+0000..U+00FF.  That's what
	   * Windows seems to do, and that's hinted about at:
	   * https://docs.microsoft.com/en-us/typography/opentype/spec/recom
	   * under "Non-Standard (Symbol) Fonts". */
	  g = FT_Get_Char_اشاریہ (ft_font->ft_face, 0xF000u + unicode);
	اكسر;
#جے_بنا_تعریف HB_NO_OT_SHAPER_ARABIC_FALLBACK
      حالة OT::OS2::font_ورقہ_t::FONT_ورقہ_SIMP_ARABIC:
	g = FT_Get_Char_اشاریہ (ft_font->ft_face, _hb_arabic_pua_simp_map (unicode));
	اكسر;
      حالة OT::OS2::font_ورقہ_t::FONT_ورقہ_TRAD_ARABIC:
	g = FT_Get_Char_اشاریہ (ft_font->ft_face, _hb_arabic_pua_trad_map (unicode));
	اكسر;
#جے_انت
      افتراضي:
	اكسر;
      }
      إذا (!g)
	أرجع خطأ;
    }
    وإلا
      أرجع خطأ;
  }

  *glyph = g;
  أرجع سچ;
}

ساكن بنانشانی صحيح
hb_ft_get_nominal_glyphs (hb_font_t *font HB_بن_ورتیا,
			  فراغ *font_انکڑے,
			  بنانشانی صحيح گنتی,
			  ثابت hb_codepoint_t *پہلا_unicode,
			  بنانشانی صحيح unicode_stride,
			  hb_codepoint_t *پہلا_glyph,
			  بنانشانی صحيح glyph_stride,
			  فراغ *user_انکڑے HB_بن_ورتیا)
{
  ثابت hb_ft_font_t *ft_font = (ثابت hb_ft_font_t *) font_انکڑے;
  hb_جندرا_t lock (ft_font->lock);
  بنانشانی صحيح پورا;
  لكل (پورا = 0;
       پورا < گنتی && (*پہلا_glyph = FT_Get_Char_اشاریہ (ft_font->ft_face, *پہلا_unicode));
       پورا++)
  {
    پہلا_unicode = &Struct_At_وتھ_Unaligned<hb_codepoint_t> (پہلا_unicode, unicode_stride);
    پہلا_glyph = &Struct_At_وتھ_Unaligned<hb_codepoint_t> (پہلا_glyph, glyph_stride);
  }
  /* We don't need to do ft_font->symbol dance here, since HB calls the singular
   * nominal_glyph() for what we don't handle here. */
  أرجع پورا;
}


ساكن hb_bool_t
hb_ft_get_variation_glyph (hb_font_t *font HB_بن_ورتیا,
			   فراغ *font_انکڑے,
			   hb_codepoint_t unicode,
			   hb_codepoint_t variation_selector,
			   hb_codepoint_t *glyph,
			   فراغ *user_انکڑے HB_بن_ورتیا)
{
  ثابت hb_ft_font_t *ft_font = (ثابت hb_ft_font_t *) font_انکڑے;
  hb_جندرا_t lock (ft_font->lock);
  بنانشانی صحيح g = FT_Face_Get_Char_Variant_اشاریہ (ft_font->ft_face, unicode, variation_selector);

  إذا (unlikely (!g))
    أرجع خطأ;

  *glyph = g;
  أرجع سچ;
}

ساكن فراغ
hb_ft_get_glyph_h_advances (hb_font_t* font, فراغ* font_انکڑے,
			    بنانشانی گنتی,
			    ثابت hb_codepoint_t *پہلا_glyph,
			    بنانشانی glyph_stride,
			    hb_position_t *پہلا_advance,
			    بنانشانی advance_stride,
			    فراغ *user_انکڑے HB_بن_ورتیا)
{
  ثابت hb_ft_font_t *ft_font = (ثابت hb_ft_font_t *) font_انکڑے;
  hb_ft_hb_font_check_بدلیا (font, ft_font);

  hb_جندرا_t lock (ft_font->lock);
  FT_Face ft_face = ft_font->ft_face;
  صحيح لدو_flags = ft_font->لدو_flags;
  عائم x_mult;
#جے_تعریف_شدہ HAVE_FT_GET_TRANSFORM
  إذا (ft_font->transform)
  {
    FT_میٹرکس میٹرکس;
    FT_Get_Transform (ft_face, &میٹرکس, خالی_سوچک);
    x_mult = sqrtf ((عائم)میٹرکس.xx * میٹرکس.xx + (عائم)میٹرکس.xy * میٹرکس.xy) / 65536.f;
    x_mult *= font->x_scale < 0 ? -1 : +1;
  }
  وإلا
#جے_انت
  {
    x_mult = font->x_scale < 0 ? -1 : +1;
  }

  لكل (بنانشانی صحيح i = 0; i < گنتی; i++)
  {
    hb_position_t advance;
    hb_codepoint_t glyph = *پہلا_glyph;

    بنانشانی صحيح cv;
    إذا (ft_font->advance_cache.get (glyph, &cv))
      advance = (hb_position_t) cv;
    وإلا
    {
      FT_Fixed v = 0;
      FT_Get_Advance (ft_face, glyph, لدو_flags, &v);
      /* Work around bug that FreeType seems to return negative advance
       * for variable-set fonts if x_scale is negative! */
      مزدوج scaled = trunc (fabs ((مزدوج) v) * (مزدوج) x_mult + (1<<9)) / (1<<10);
      advance = hb_clamp_منزل<hb_position_t> (floor (scaled));
      ft_font->advance_cache.سیٹ (glyph, advance);
    }

    *پہلا_advance = advance;
    پہلا_glyph = &Struct_At_وتھ_Unaligned<hb_codepoint_t> (پہلا_glyph, glyph_stride);
    پہلا_advance = &Struct_At_وتھ_Unaligned<hb_position_t> (پہلا_advance, advance_stride);
  }
}

#جے_بنا_تعریف HB_NO_VERTICAL
ساكن hb_position_t
hb_ft_get_glyph_v_advance (hb_font_t *font,
			   فراغ *font_انکڑے,
			   hb_codepoint_t glyph,
			   فراغ *user_انکڑے HB_بن_ورتیا)
{
  ثابت hb_ft_font_t *ft_font = (ثابت hb_ft_font_t *) font_انکڑے;
  hb_ft_hb_font_check_بدلیا (font, ft_font);

  hb_جندرا_t lock (ft_font->lock);
  FT_Fixed v;
  عائم y_mult;
#جے_تعریف_شدہ HAVE_FT_GET_TRANSFORM
  إذا (ft_font->transform)
  {
    FT_میٹرکس میٹرکس;
    FT_Get_Transform (ft_font->ft_face, &میٹرکس, خالی_سوچک);
    y_mult = sqrtf ((عائم)میٹرکس.yx * میٹرکس.yx + (عائم)میٹرکس.yy * میٹرکس.yy) / 65536.f;
    y_mult *= font->y_scale < 0 ? -1 : +1;
  }
  وإلا
#جے_انت
  {
    y_mult = font->y_scale < 0 ? -1 : +1;
  }

  إذا (unlikely (FT_Get_Advance (ft_font->ft_face, glyph, ft_font->لدو_flags | FT_لدو_VERTICAL_LAYOUT, &v)))
    أرجع 0;

  /* Note: FreeType's vertical metrics grows downward while other FreeType coordinates
   * have a Y growing upward.  Hence the extra negation. */
  مزدوج advance = floor ((-(مزدوج) v + (1<<9)) / (1<<10));

  أرجع hb_clamp_منزل<hb_position_t> ((مزدوج) y_mult * advance);
}
#جے_انت

#جے_بنا_تعریف HB_NO_VERTICAL
ساكن hb_bool_t
hb_ft_get_glyph_v_origin (hb_font_t *font,
			  فراغ *font_انکڑے,
			  hb_codepoint_t glyph,
			  hb_position_t *x,
			  hb_position_t *y,
			  فراغ *user_انکڑے HB_بن_ورتیا)
{
  ثابت hb_ft_font_t *ft_font = (ثابت hb_ft_font_t *) font_انکڑے;
  hb_ft_hb_font_check_بدلیا (font, ft_font);

  hb_جندرا_t lock (ft_font->lock);
  FT_Face ft_face = ft_font->ft_face;
  عائم x_mult, y_mult;
#جے_تعریف_شدہ HAVE_FT_GET_TRANSFORM
  إذا (ft_font->transform)
  {
    FT_میٹرکس میٹرکس;
    FT_Get_Transform (ft_face, &میٹرکس, خالی_سوچک);
    x_mult = sqrtf ((عائم)میٹرکس.xx * میٹرکس.xx + (عائم)میٹرکس.xy * میٹرکس.xy) / 65536.f;
    x_mult *= font->x_scale < 0 ? -1 : +1;
    y_mult = sqrtf ((عائم)میٹرکس.yx * میٹرکس.yx + (عائم)میٹرکس.yy * میٹرکس.yy) / 65536.f;
    y_mult *= font->y_scale < 0 ? -1 : +1;
  }
  وإلا
#جے_انت
  {
    x_mult = font->x_scale < 0 ? -1 : +1;
    y_mult = font->y_scale < 0 ? -1 : +1;
  }

  إذا (unlikely (FT_لدو_Glyph (ft_face, glyph, ft_font->لدو_flags)))
    أرجع خطأ;

  /* Note: FreeType's vertical metrics grows downward while other FreeType coordinates
   * have a Y growing upward.  Hence the extra negation. */
  *x = hb_clamp_منزل<hb_position_t> ((مزدوج) x_mult *
				   ((مزدوج) ft_face->glyph->metrics.horiBearingX -
				    ft_face->glyph->metrics.vertBearingX));
  *y = hb_clamp_منزل<hb_position_t> ((مزدوج) y_mult *
				   ((مزدوج) ft_face->glyph->metrics.horiBearingY +
				    ft_face->glyph->metrics.vertBearingY));

  أرجع سچ;
}
#جے_انت

#جے_بنا_تعریف HB_NO_OT_SHAPE_FALLBACK
ساكن hb_position_t
hb_ft_get_glyph_h_kerning (hb_font_t *font,
			   فراغ *font_انکڑے,
			   hb_codepoint_t بایاں_glyph,
			   hb_codepoint_t دایاں_glyph,
			   فراغ *user_انکڑے HB_بن_ورتیا)
{
  ثابت hb_ft_font_t *ft_font = (ثابت hb_ft_font_t *) font_انکڑے;
  hb_ft_hb_font_check_بدلیا (font, ft_font);

  hb_جندرا_t lock (ft_font->lock);
  FT_سمتیہ kerningv;

  FT_Kerning_منوال منوال = font->x_ppem ? FT_KERNING_DEFAULT : FT_KERNING_UNFITTED;
  إذا (FT_Get_Kerning (ft_font->ft_face, بایاں_glyph, دایاں_glyph, منوال, &kerningv))
    أرجع 0;

  أرجع hb_clamp_منزل<hb_position_t> ((int64_t) kerningv.x);
}
#جے_انت

ساكن منطقي
hb_ft_is_colr_glyph (hb_font_t *font,
		     فراغ *font_انکڑے,
		     hb_codepoint_t gid)
{
#جے_بنا_تعریف HB_NO_PAINT
#إذا (FREETYPE_MAJOR*10000 + FREETYPE_MINOR*100 + FREETYPE_PATCH) >= 21300
  ثابت hb_ft_font_t *ft_font = (ثابت hb_ft_font_t *) font_انکڑے;
  FT_Face ft_face = ft_font->ft_face;


  /* COLRv1 */
  FT_OpaquePaint paint = {0};
  إذا (FT_Get_Color_Glyph_Paint (ft_face, gid,
			        FT_COLOR_NO_جڑ_TRANSFORM,
			        &paint))
    أرجع سچ;

  /* COLRv0 */
  FT_LayerIterator  iterator;
  FT_UInt  layer_glyph_اشاریہ;
  FT_UInt  layer_color_اشاریہ;
  iterator.p  = عدم;
  إذا (FT_Get_Color_Glyph_Layer (ft_face,
				gid,
				&layer_glyph_اشاریہ,
				&layer_color_اشاریہ,
				&iterator))
    أرجع سچ;
#جے_انت
#جے_انت

  أرجع خطأ;
}

ساكن hb_bool_t
hb_ft_get_glyph_extents (hb_font_t *font,
			 فراغ *font_انکڑے,
			 hb_codepoint_t glyph,
			 hb_glyph_extents_t *extents,
			 فراغ *user_انکڑے HB_بن_ورتیا)
{
  // FreeType doesn't return COLR glyph extents.
  إذا (hb_ft_is_colr_glyph (font, font_انکڑے, glyph))
    أرجع خطأ;

  ثابت hb_ft_font_t *ft_font = (ثابت hb_ft_font_t *) font_انکڑے;
  hb_ft_hb_font_check_بدلیا (font, ft_font);

  hb_جندرا_t lock (ft_font->lock);
  FT_Face ft_face = ft_font->ft_face;
  عائم x_mult, y_mult;

#جے_تعریف_شدہ HAVE_FT_GET_TRANSFORM
  إذا (ft_font->transform)
  {
    FT_میٹرکس میٹرکس;
    FT_Get_Transform (ft_face, &میٹرکس, خالی_سوچک);
    x_mult = sqrtf ((عائم)میٹرکس.xx * میٹرکس.xx + (عائم)میٹرکس.xy * میٹرکس.xy) / 65536.f;
    x_mult *= font->x_scale < 0 ? -1 : +1;
    y_mult = sqrtf ((عائم)میٹرکس.yx * میٹرکس.yx + (عائم)میٹرکس.yy * میٹرکس.yy) / 65536.f;
    y_mult *= font->y_scale < 0 ? -1 : +1;
  }
  وإلا
#جے_انت
  {
    x_mult = font->x_scale < 0 ? -1 : +1;
    y_mult = font->y_scale < 0 ? -1 : +1;
  }

  إذا (unlikely (FT_لدو_Glyph (ft_face, glyph, ft_font->لدو_flags)))
    أرجع خطأ;

  /* Copied from hb_font_t::scale_glyph_extents. */

  عائم x1 = x_mult * ft_face->glyph->metrics.horiBearingX;
  عائم y1 = y_mult * ft_face->glyph->metrics.horiBearingY;
  عائم x2 = x1 + x_mult *  ft_face->glyph->metrics.چوڑائی;
  عائم y2 = y1 + y_mult * -ft_face->glyph->metrics.اچائی;

  مزدوج rx1 = (مزدوج) roundf (x1);
  مزدوج ry1 = (مزدوج) roundf (y1);
  مزدوج rx2 = (مزدوج) roundf (x2);
  مزدوج ry2 = (مزدوج) roundf (y2);

  extents->x_bearing = hb_clamp_منزل<hb_position_t> (rx1);
  extents->y_bearing = hb_clamp_منزل<hb_position_t> (ry1);
  extents->چوڑائی = hb_clamp_منزل<hb_position_t> (rx2 - rx1);
  extents->اچائی = hb_clamp_منزل<hb_position_t> (ry2 - ry1);

  أرجع سچ;
}

ساكن hb_bool_t
hb_ft_get_glyph_contour_نقطہ (hb_font_t *font HB_بن_ورتیا,
			       فراغ *font_انکڑے,
			       hb_codepoint_t glyph,
			       بنانشانی صحيح نقطہ_اشاریہ,
			       hb_position_t *x,
			       hb_position_t *y,
			       فراغ *user_انکڑے HB_بن_ورتیا)
{
  ثابت hb_ft_font_t *ft_font = (ثابت hb_ft_font_t *) font_انکڑے;
  hb_ft_hb_font_check_بدلیا (font, ft_font);

  hb_جندرا_t lock (ft_font->lock);
  FT_Face ft_face = ft_font->ft_face;

  إذا (unlikely (FT_لدو_Glyph (ft_face, glyph, ft_font->لدو_flags)))
      أرجع خطأ;

  إذا (unlikely (ft_face->glyph->format != FT_GLYPH_FORMAT_OUTLINE))
      أرجع خطأ;

  إذا (unlikely (نقطہ_اشاریہ >= (بنانشانی صحيح) ft_face->glyph->outline.n_points))
      أرجع خطأ;

  *x = ft_face->glyph->outline.points[نقطہ_اشاریہ].x;
  *y = ft_face->glyph->outline.points[نقطہ_اشاریہ].y;

  أرجع سچ;
}

ساكن hb_bool_t
hb_ft_get_glyph_name (hb_font_t *font HB_بن_ورتیا,
		      فراغ *font_انکڑے,
		      hb_codepoint_t glyph,
		      حرف *name, بنانشانی صحيح ناپ,
		      فراغ *user_انکڑے HB_بن_ورتیا)
{
  ثابت hb_ft_font_t *ft_font = (ثابت hb_ft_font_t *) font_انکڑے;
  hb_جندرا_t lock (ft_font->lock);
  FT_Face ft_face = ft_font->ft_face;

  إذا (!ناپ)
  {
    حرف buf[128];
    أرجع !FT_Get_Glyph_Name (ft_face, glyph, buf, حجم (buf)) && *buf;
  }

  hb_bool_t ret = !FT_Get_Glyph_Name (ft_face, glyph, name, ناپ);
  إذا (ret && (ناپ && !*name))
    ret = خطأ;

  أرجع ret;
}

ساكن hb_bool_t
hb_ft_get_glyph_from_name (hb_font_t *font HB_بن_ورتیا,
			   فراغ *font_انکڑے,
			   ثابت حرف *name, صحيح len, /* -1 means nul-terminated */
			   hb_codepoint_t *glyph,
			   فراغ *user_انکڑے HB_بن_ورتیا)
{
  ثابت hb_ft_font_t *ft_font = (ثابت hb_ft_font_t *) font_انکڑے;
  hb_جندرا_t lock (ft_font->lock);
  FT_Face ft_face = ft_font->ft_face;

  إذا (len < 0)
    *glyph = FT_Get_Name_اشاریہ (ft_face, (FT_String *) name);
  وإلا {
    /* Make a nul-terminated version. */
    حرف buf[128];
    len = hb_min (len, (صحيح) حجم (buf) - 1);
    strncpy (buf, name, len);
    buf[len] = '\0';
    *glyph = FT_Get_Name_اشاریہ (ft_face, buf);
  }

  إذا (*glyph == 0)
  {
    /* Check whether the given name was actually the name of glyph 0. */
    حرف buf[128];
    إذا (!FT_Get_Glyph_Name(ft_face, 0, buf, حجم (buf)) &&
	len < 0 ? !strcmp (buf, name) : !لڑی_مقابلہ (buf, name, len))
      أرجع سچ;
  }

  أرجع *glyph != 0;
}

ساكن hb_bool_t
hb_ft_get_font_h_extents (hb_font_t *font HB_بن_ورتیا,
			  فراغ *font_انکڑے,
			  hb_font_extents_t *metrics,
			  فراغ *user_انکڑے HB_بن_ورتیا)
{
  ثابت hb_ft_font_t *ft_font = (ثابت hb_ft_font_t *) font_انکڑے;
  hb_ft_hb_font_check_بدلیا (font, ft_font);

  hb_جندرا_t lock (ft_font->lock);
  FT_Face ft_face = ft_font->ft_face;
  عائم y_mult;
#جے_تعریف_شدہ HAVE_FT_GET_TRANSFORM
  إذا (ft_font->transform)
  {
    FT_میٹرکس میٹرکس;
    FT_Get_Transform (ft_face, &میٹرکس, خالی_سوچک);
    y_mult = sqrtf ((عائم)میٹرکس.yx * میٹرکس.yx + (عائم)میٹرکس.yy * میٹرکس.yy) / 65536.f;
    y_mult *= font->y_scale < 0 ? -1 : +1;
  }
  وإلا
#جے_انت
  {
    y_mult = font->y_scale < 0 ? -1 : +1;
  }

  إذا (ft_face->units_فی_EM != 0)
  {
    مزدوج ascender = FT_MulFix(ft_face->ascender, ft_face->ناپ->metrics.y_scale);
    مزدوج descender = FT_MulFix(ft_face->descender, ft_face->ناپ->metrics.y_scale);
    مزدوج اچائی = FT_MulFix( ft_face->اچائی, ft_face->ناپ->metrics.y_scale );
    metrics->ascender = hb_clamp_منزل<hb_position_t> ((مزدوج) y_mult * ascender);
    metrics->descender = hb_clamp_منزل<hb_position_t> ((مزدوج) y_mult * descender);
    metrics->سطر_gap = hb_clamp_منزل<hb_position_t> ((مزدوج) y_mult * (اچائی - (ascender - descender)));
  }
  وإلا
  {
    /* Bitmap-only font, eg. color bitmap font. */
    مزدوج ascender = ft_face->ناپ->metrics.ascender;
    مزدوج descender = ft_face->ناپ->metrics.descender;
    مزدوج اچائی = ft_face->ناپ->metrics.اچائی;
    metrics->ascender = hb_clamp_منزل<hb_position_t> ((مزدوج) y_mult * ascender);
    metrics->descender = hb_clamp_منزل<hb_position_t> ((مزدوج) y_mult * descender);
    metrics->سطر_gap = hb_clamp_منزل<hb_position_t> ((مزدوج) y_mult * (اچائی - (ascender - descender)));
  }

  أرجع سچ;
}

#جے_بنا_تعریف HB_NO_DRAW

ساكن صحيح
hb_ft_move_منزل (ثابت FT_سمتیہ *to,
		فراغ *arg)
{
  hb_draw_session_t *drawing = (hb_draw_session_t *) arg;
  drawing->move_منزل (to->x, to->y);
  أرجع FT_Err_Ok;
}

ساكن صحيح
hb_ft_سطر_منزل (ثابت FT_سمتیہ *to,
		فراغ *arg)
{
  hb_draw_session_t *drawing = (hb_draw_session_t *) arg;
  drawing->سطر_منزل (to->x, to->y);
  أرجع FT_Err_Ok;
}

ساكن صحيح
hb_ft_conic_منزل (ثابت FT_سمتیہ *control,
		 ثابت FT_سمتیہ *to,
		 فراغ *arg)
{
  hb_draw_session_t *drawing = (hb_draw_session_t *) arg;
  drawing->quadratic_منزل (control->x, control->y,
			 to->x, to->y);
  أرجع FT_Err_Ok;
}

ساكن صحيح
hb_ft_cubic_منزل (ثابت FT_سمتیہ *control1,
		 ثابت FT_سمتیہ *control2,
		 ثابت FT_سمتیہ *to,
		 فراغ *arg)
{
  hb_draw_session_t *drawing = (hb_draw_session_t *) arg;
  drawing->cubic_منزل (control1->x, control1->y,
		     control2->x, control2->y,
		     to->x, to->y);
  أرجع FT_Err_Ok;
}

ساكن hb_bool_t
hb_ft_draw_glyph_or_fail (hb_font_t *font,
			  فراغ *font_انکڑے,
			  hb_codepoint_t glyph,
			  hb_draw_funcs_t *draw_funcs, فراغ *draw_انکڑے,
			  فراغ *user_انکڑے HB_بن_ورتیا)
{
  ثابت hb_ft_font_t *ft_font = (ثابت hb_ft_font_t *) font_انکڑے;
  hb_ft_hb_font_check_بدلیا (font, ft_font);

  hb_جندرا_t lock (ft_font->lock);
  FT_Face ft_face = ft_font->ft_face;

  إذا (unlikely (FT_لدو_Glyph (ft_face, glyph,
			       FT_لدو_NO_BITMAP | ft_font->لدو_flags)))
    أرجع خطأ;

  إذا (ft_face->glyph->format != FT_GLYPH_FORMAT_OUTLINE)
    أرجع خطأ;

  ثابت FT_Outline_Funcs outline_funcs = {
    hb_ft_move_منزل,
    hb_ft_سطر_منزل,
    hb_ft_conic_منزل,
    hb_ft_cubic_منزل,
    0, /*کھسکاؤ*/
    0, /* delta */
  };

  hb_draw_session_t draw_session {draw_funcs, draw_انکڑے};

  FT_Outline_Decompose (&ft_face->glyph->outline,
			&outline_funcs,
			&draw_session);

  أرجع سچ;
}
#جے_انت

#جے_بنا_تعریف HB_NO_PAINT
#إذا (FREETYPE_MAJOR*10000 + FREETYPE_MINOR*100 + FREETYPE_PATCH) >= 21300

#ضمن "hb-ft-colr.hh"

ساكن hb_bool_t
hb_ft_paint_glyph_or_fail (hb_font_t *font,
			   فراغ *font_انکڑے,
			   hb_codepoint_t gid,
			   hb_paint_funcs_t *paint_funcs, فراغ *paint_انکڑے,
			   بنانشانی صحيح palette_اشاریہ,
			   hb_color_t foreground,
			   فراغ *user_انکڑے)
{
  ثابت hb_ft_font_t *ft_font = (ثابت hb_ft_font_t *) font_انکڑے;
  hb_ft_hb_font_check_بدلیا (font, ft_font);

  hb_جندرا_t lock (ft_font->lock);
  FT_Face ft_face = ft_font->ft_face;

  FT_Long لدو_flags = ft_font->لدو_flags | FT_لدو_COLOR;
#إذا (FREETYPE_MAJOR*10000 + FREETYPE_MINOR*100 + FREETYPE_PATCH) >= 21301
  لدو_flags |= FT_لدو_NO_SVG;
#جے_انت

  /* We release the lock before calling into glyph callbacks, such that
   * eg. draw API can call back into the face.*/

  إذا (unlikely (FT_لدو_Glyph (ft_face, gid, لدو_flags)))
    أرجع خطأ;

  إذا (ft_face->glyph->format == FT_GLYPH_FORMAT_OUTLINE)
  {
    إذا (hb_ft_paint_glyph_colr (font, font_انکڑے, gid,
				paint_funcs, paint_انکڑے,
				palette_اشاریہ, foreground,
				user_انکڑے))
      أرجع سچ;

    // Outline glyph
    أرجع خطأ;
  }

  خودکار *glyph = ft_face->glyph;
  إذا (glyph->format == FT_GLYPH_FORMAT_BITMAP)
  {
    منطقي ret = خطأ;
    خودکار &bitmap = glyph->bitmap;
    إذا (bitmap.pixel_منوال == FT_PIXEL_منوال_BGRA)
    {
      إذا (bitmap.pitch != (نشانیوالا) bitmap.چوڑائی * 4)
        أرجع ret;

      ft_font->lock.unlock ();

      hb_blob_t *blob = hb_blob_create ((ثابت حرف *) bitmap.بفر,
					bitmap.pitch * bitmap.صفاں,
					HB_حافظہ_منوال_DUPLICATE,
					خالی_سوچک, خالی_سوچک);

      hb_glyph_extents_t extents;
      إذا (!font->get_glyph_extents (gid, &extents, خطأ))
	اذهب باہرلا;

      إذا (paint_funcs->image (paint_انکڑے,
			      blob,
			      bitmap.چوڑائی,
			      bitmap.صفاں,
			      HB_PAINT_IMAGE_FORMAT_BGRA,
			      0.f,
			      &extents))
        ret = سچ;

    باہرلا:
      hb_blob_destroy (blob);
      ft_font->lock.lock ();
    }

    أرجع ret;
  }
  أرجع خطأ;
}
#جے_انت
#جے_انت


ساكن مضمن فراغ free_static_ft_funcs ();

ساكن بنية hb_ft_font_funcs_lazy_loader_t : hb_font_funcs_lazy_loader_t<hb_ft_font_funcs_lazy_loader_t>
{
  ساكن hb_font_funcs_t *create ()
  {
    hb_font_funcs_t *funcs = hb_font_funcs_create ();

    hb_font_funcs_سیٹ_nominal_glyph_func (funcs, hb_ft_get_nominal_glyph, خالی_سوچک, خالی_سوچک);
    hb_font_funcs_سیٹ_nominal_glyphs_func (funcs, hb_ft_get_nominal_glyphs, خالی_سوچک, خالی_سوچک);
    hb_font_funcs_سیٹ_variation_glyph_func (funcs, hb_ft_get_variation_glyph, خالی_سوچک, خالی_سوچک);

    hb_font_funcs_سیٹ_font_h_extents_func (funcs, hb_ft_get_font_h_extents, خالی_سوچک, خالی_سوچک);
    hb_font_funcs_سیٹ_glyph_h_advances_func (funcs, hb_ft_get_glyph_h_advances, خالی_سوچک, خالی_سوچک);

#جے_بنا_تعریف HB_NO_VERTICAL
    hb_font_funcs_سیٹ_glyph_v_advance_func (funcs, hb_ft_get_glyph_v_advance, خالی_سوچک, خالی_سوچک);
    hb_font_funcs_سیٹ_glyph_v_origin_func (funcs, hb_ft_get_glyph_v_origin, خالی_سوچک, خالی_سوچک);
#جے_انت

#جے_بنا_تعریف HB_NO_OT_SHAPE_FALLBACK
    hb_font_funcs_سیٹ_glyph_h_kerning_func (funcs, hb_ft_get_glyph_h_kerning, خالی_سوچک, خالی_سوچک);
#جے_انت
    hb_font_funcs_سیٹ_glyph_extents_func (funcs, hb_ft_get_glyph_extents, خالی_سوچک, خالی_سوچک);
    hb_font_funcs_سیٹ_glyph_contour_نقطہ_func (funcs, hb_ft_get_glyph_contour_نقطہ, خالی_سوچک, خالی_سوچک);
    hb_font_funcs_سیٹ_glyph_name_func (funcs, hb_ft_get_glyph_name, خالی_سوچک, خالی_سوچک);
    hb_font_funcs_سیٹ_glyph_from_name_func (funcs, hb_ft_get_glyph_from_name, خالی_سوچک, خالی_سوچک);

#جے_بنا_تعریف HB_NO_DRAW
    hb_font_funcs_سیٹ_draw_glyph_or_fail_func (funcs, hb_ft_draw_glyph_or_fail, خالی_سوچک, خالی_سوچک);
#جے_انت

#جے_بنا_تعریف HB_NO_PAINT
#إذا (FREETYPE_MAJOR*10000 + FREETYPE_MINOR*100 + FREETYPE_PATCH) >= 21300
    hb_font_funcs_سیٹ_paint_glyph_or_fail_func (funcs, hb_ft_paint_glyph_or_fail, خالی_سوچک, خالی_سوچک);
#جے_انت
#جے_انت

    hb_font_funcs_make_immutable (funcs);

    hb_atexit (free_static_ft_funcs);

    أرجع funcs;
  }
} static_ft_funcs;

ساكن مضمن
فراغ free_static_ft_funcs ()
{
  static_ft_funcs.free_instance ();
}

ساكن hb_font_funcs_t *
_hb_ft_get_font_funcs ()
{
  أرجع static_ft_funcs.get_unconst ();
}

ساكن فراغ
hb_ft_font_سیٹ_funcs_2 (hb_font_t *font, FT_Face ft_face, منطقي unref,
		       منطقي static_library = خطأ)
{
  منطقي علامت = ft_face->charmap && ft_face->charmap->encoding == FT_ENCODING_MS_علامت;

  hb_ft_font_t *ft_font = _hb_ft_font_create (ft_face, علامت, unref, static_library);
  إذا (unlikely (!ft_font)) أرجع;

  hb_font_سیٹ_funcs (font,
		     _hb_ft_get_font_funcs (),
		     ft_font,
		     _hb_ft_font_destroy);
}


ساكن hb_blob_t *
hb_ft_reference_جدول (hb_face_t *face HB_بن_ورتیا, hb_ٹیگ_t ٹیگ_2, فراغ *user_انکڑے)
{
  FT_Face ft_face = (FT_Face) user_انکڑے;
  FT_Byte *بفر;
  FT_ULong  لمائی = 0;
  FT_Error error;

  /* In new FreeType, a tag value of 1 loads the SFNT table directory. Reject it. */
  إذا (ٹیگ_2 == 1)
    أرجع خالی_سوچک;

  /* Note: FreeType like HarfBuzz uses the NONE tag for fetching the entire blob */

  error = FT_لدو_Sfnt_جدول (ft_face, ٹیگ_2, 0, خالی_سوچک, &لمائی);
  إذا (error)
    أرجع خالی_سوچک;

  بفر = (FT_Byte *) hb_malloc (لمائی);
  إذا (!بفر)
    أرجع خالی_سوچک;
  خودکار بفر_guard = hb_make_scope_guard ([&]() { hb_free (بفر); });

  error = FT_لدو_Sfnt_جدول (ft_face, ٹیگ_2, 0, بفر, &لمائی);
  إذا (error)
    أرجع خالی_سوچک;

  بفر_guard.release ();
  أرجع hb_blob_create ((ثابت حرف *) بفر, لمائی,
			 HB_حافظہ_منوال_WRITABLE,
			 بفر, hb_free);
}

ساكن بنانشانی
hb_ft_get_جدول_ٹیگ_سوچی (ثابت hb_face_t *face HB_بن_ورتیا,
		       بنانشانی صحيح شروع_وتھ,
		       بنانشانی صحيح *جدول_گنتی_2,
		       hb_ٹیگ_t *جدول_ٹیگ_سوچی,
		       فراغ *user_انکڑے)
{
  FT_Face ft_face = (FT_Face) user_انکڑے;

  FT_ULong population = 0;
  FT_Sfnt_جدول_Info (ft_face,
		      0, // table_index; ignored
		      خالی_سوچک,
                      &population);

  إذا (!جدول_گنتی_2)
    أرجع population;

  إذا (unlikely (شروع_وتھ >= population))
  {
    *جدول_گنتی_2 = 0;
    أرجع population;
  }

  بنانشانی سرا_وتھ = شروع_وتھ + *جدول_گنتی_2;
  إذا (unlikely (سرا_وتھ < شروع_وتھ)) /* Overflow. */
  {
    *جدول_گنتی_2 = 0;
    أرجع population;
  }
  سرا_وتھ = hb_min (سرا_وتھ, (بنانشانی) population);

  *جدول_گنتی_2 = سرا_وتھ - شروع_وتھ;
  إذا (جدول_ٹیگ_سوچی)
    لكل (بنانشانی i = شروع_وتھ; i < سرا_وتھ; i++)
    {
      FT_ULong ٹیگ_2 = 0, لمائی;
      FT_Sfnt_جدول_Info (ft_face, i, &ٹیگ_2, &لمائی);
      جدول_ٹیگ_سوچی[i - شروع_وتھ] = ٹیگ_2;
    }

  أرجع population;
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

  إذا (!ft_face->دھارا->اقرأ) {
    hb_blob_t *blob;

    blob = hb_blob_create ((ثابت حرف *) ft_face->دھارا->ادھار,
			   (بنانشانی صحيح) ft_face->دھارا->ناپ,
			   HB_حافظہ_منوال_READONLY,
			   ft_face, destroy);
    face = hb_face_create (blob, ft_face->face_اشاریہ);
    hb_blob_destroy (blob);
  } وإلا {
    face = hb_face_create_for_tables (hb_ft_reference_جدول, ft_face, destroy);
    hb_face_سیٹ_get_جدول_ٹیگ_سوچی_func (face, hb_ft_get_جدول_ٹیگ_سوچی, ft_face, خالی_سوچک);
  }

  hb_face_سیٹ_اشاریہ (face, ft_face->face_اشاریہ);
  hb_face_سیٹ_upem (face, ft_face->units_فی_EM);

  أرجع face;
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
  أرجع hb_ft_face_create (ft_face, _hb_ft_face_destroy);
}

ساكن فراغ
hb_ft_face_finalize (فراغ *arg)
{
  FT_Face ft_face = (FT_Face) arg;
  hb_face_destroy ((hb_face_t *) ft_face->generic.انکڑے);
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
  إذا (unlikely (!ft_face->generic.انکڑے || ft_face->generic.finalizer != hb_ft_face_finalize))
  {
    إذا (ft_face->generic.finalizer)
      ft_face->generic.finalizer (ft_face);

    ft_face->generic.انکڑے = hb_ft_face_create (ft_face, خالی_سوچک);
    ft_face->generic.finalizer = hb_ft_face_finalize;
  }

  أرجع hb_face_reference ((hb_face_t *) ft_face->generic.انکڑے);
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
  hb_ft_font_سیٹ_funcs_2 (font, ft_face, خطأ);
  hb_ft_font_بدلیا (font);
  أرجع font;
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
فراغ
hb_ft_font_بدلیا (hb_font_t *font)
{
  إذا (font->destroy != (hb_destroy_func_t) _hb_ft_font_destroy)
    أرجع;

  hb_ft_font_t *ft_font = (hb_ft_font_t *) font->user_انکڑے;

  FT_Face ft_face = ft_font->ft_face;

  hb_font_سیٹ_scale (font,
		     (صحيح) (((uint64_t) ft_face->ناپ->metrics.x_scale * (uint64_t) ft_face->units_فی_EM + (1u<<15)) >> 16),
		     (صحيح) (((uint64_t) ft_face->ناپ->metrics.y_scale * (uint64_t) ft_face->units_فی_EM + (1u<<15)) >> 16));
#إذا 0 /* hb-ft works in no-hinting model */
  hb_font_سیٹ_ppem (font,
		    ft_face->ناپ->metrics.x_ppem,
		    ft_face->ناپ->metrics.y_ppem);
#جے_انت

#إذا defined(HAVE_FT_GET_VAR_BLEND_COORDINATES) && !defined(HB_NO_VAR)
  FT_MM_Var *mm_var = خالی_سوچک;
  إذا (!FT_Get_MM_Var (ft_face, &mm_var))
  {
    FT_Fixed *ft_coords = (FT_Fixed *) hb_calloc (mm_var->عدد_axis, حجم (FT_Fixed));
    صحيح *coords = (صحيح *) hb_calloc (mm_var->عدد_axis, حجم (صحيح));
    إذا (coords && ft_coords)
    {
      إذا (!FT_Get_Var_Blend_Coordinates (ft_face, mm_var->عدد_axis, ft_coords))
      {
	منطقي nonzero = خطأ;

	لكل (بنانشانی صحيح i = 0; i < mm_var->عدد_axis; ++i)
	 {
	  coords[i] = (ft_coords[i] + 2) >> 2;
	  nonzero = nonzero || coords[i];
	 }

	إذا (nonzero)
	  hb_font_سیٹ_var_coords_normalized (font, coords, mm_var->عدد_axis);
	وإلا
	  hb_font_سیٹ_var_coords_normalized (font, خالی_سوچک, 0);
      }
    }
    hb_free (coords);
    hb_free (ft_coords);
#جے_تعریف_شدہ HAVE_FT_پورا_MM_VAR
    FT_پورا_MM_Var (ft_face->glyph->library, mm_var);
#وإلا
    hb_free (mm_var);
#جے_انت
  }
#جے_انت

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
hb_ft_hb_font_بدلیا_2 (hb_font_t *font)
{
  إذا (font->destroy != (hb_destroy_func_t) _hb_ft_font_destroy)
    أرجع خطأ;

  hb_ft_font_t *ft_font = (hb_ft_font_t *) font->user_انکڑے;

  أرجع hb_ft_hb_font_check_بدلیا (font, ft_font);
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
  أرجع hb_ft_font_create (ft_face, _hb_ft_face_destroy);
}


ساكن فراغ * _hb_ft_alloc (FT_حافظہ حافظہ, لما ناپ)
{ أرجع hb_malloc (ناپ); }

ساكن فراغ _hb_ft_free (FT_حافظہ حافظہ, فراغ *بلاک)
{ hb_free (بلاک); }

ساكن فراغ * _hb_ft_realloc (FT_حافظہ حافظہ, لما cur_ناپ, لما new_ناپ, فراغ *بلاک)
{ أرجع hb_realloc (بلاک, new_ناپ); }

ساكن FT_حافظہ_Rec m =
{
  خالی_سوچک,
  _hb_ft_alloc,
  _hb_ft_free,
  _hb_ft_realloc
};

ساكن مضمن فراغ free_static_ft_library ();
ساكن مضمن فراغ free_static_ft_library_mutex ();

ساكن بنية hb_ft_library_mutex_lazy_loader_t : hb_lazy_loader_t<hb_mutex_t,
								   hb_ft_library_mutex_lazy_loader_t>
{
  ساكن hb_mutex_t *create ()
  {
    hb_mutex_t *lock = (hb_mutex_t *) hb_calloc (1, حجم (hb_mutex_t));
    إذا (unlikely (!lock))
      أرجع خالی_سوچک;

    lock = جديد (lock) hb_mutex_t;
    hb_atexit (free_static_ft_library_mutex);
    أرجع lock;
  }
  ساكن فراغ destroy (hb_mutex_t *lock)
  {
    lock->~hb_mutex_t ();
    hb_free (lock);
  }
  ساكن hb_mutex_t *get_null ()
  {
    أرجع خالی_سوچک;
  }
} static_ft_library_mutex;

ساكن بنية hb_ft_library_lazy_loader_t : hb_lazy_loader_t<hb_remove_اشارہ_گر<FT_Library>,
							     hb_ft_library_lazy_loader_t>
{
  ساكن FT_Library create ()
  {
    FT_Library l;
    إذا (FT_New_Library (&m, &l))
      أرجع خالی_سوچک;

    FT_جوڑو_Default_Modules (l);
    FT_سیٹ_Default_Properties (l);

    hb_atexit (free_static_ft_library);

    أرجع l;
  }
  ساكن فراغ destroy (FT_Library l)
  {
    FT_پورا_Library (l);
  }
  ساكن FT_Library get_null ()
  {
    أرجع خالی_سوچک;
  }
} static_ft_library;

ساكن مضمن
فراغ free_static_ft_library_mutex ()
{
  static_ft_library_mutex.free_instance ();
}

ساكن مضمن
فراغ free_static_ft_library ()
{
  hb_جندرا_t lock (static_ft_library_mutex.get_unconst ());
  static_ft_library.free_instance ();
}

ساكن FT_Library
reference_ft_library ()
{
  hb_mutex_t *mutex = static_ft_library_mutex.get_unconst ();
  إذا (unlikely (!mutex))
    أرجع خالی_سوچک;
  hb_جندرا_t lock (mutex);
  FT_Library l = static_ft_library.get_unconst ();
  إذا (unlikely (FT_Reference_Library (l)))
  {
    DEBUG_پیغام (FT, l, "FT_Reference_Library() failed");
    أرجع خالی_سوچک;
  }
  أرجع l;
}

ساكن hb_user_انکڑے_کلید_t ft_library_کلید = {0};

ساكن فراغ
_hb_ft_face_destroy_static (فراغ *انکڑے)
{
  hb_جندرا_t lock (static_ft_library_mutex.get_unconst ());
  FT_پورا_Face ((FT_Face) انکڑے);
}

ساكن FT_Error
new_ft_face (FT_Library  ft_library,
	     ثابت حرف *file_name,
	     بنانشانی صحيح اشاریہ_2,
	     FT_Face     *ft_face)
{
  hb_جندرا_t lock (static_ft_library_mutex.get_unconst ());
  أرجع FT_New_Face (ft_library, file_name, اشاریہ_2, ft_face);
}

ساكن FT_Error
new_ft_حافظہ_face (FT_Library    ft_library,
		    ثابت FT_Byte *blob_انکڑے,
		    بنانشانی صحيح   blob_ناپ,
		    بنانشانی صحيح   اشاریہ_2,
		    FT_Face       *ft_face)
{
  hb_جندرا_t lock (static_ft_library_mutex.get_unconst ());
  أرجع FT_New_حافظہ_Face (ft_library, blob_انکڑے, blob_ناپ, اشاریہ_2, ft_face);
}

ساكن فراغ
finalize_ft_library (فراغ *arg)
{
  FT_Face ft_face = (FT_Face) arg;
  FT_پورا_Library ((FT_Library) ft_face->generic.انکڑے);
}

ساكن فراغ
destroy_ft_library (فراغ *arg)
{
  hb_جندرا_t lock (static_ft_library_mutex.get_unconst ());
  FT_پورا_Library ((FT_Library) arg);
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
				     بنانشانی صحيح  اشاریہ_2)
{
  FT_Library ft_library = reference_ft_library ();
  إذا (unlikely (!ft_library))
  {
    DEBUG_پیغام (FT, ft_library, "reference_ft_library failed");
    أرجع خالی_سوچک;
  }

  FT_Face ft_face;
  إذا (unlikely (new_ft_face (ft_library,
			     file_name,
			     اشاریہ_2,
			     &ft_face)))
  {
    destroy_ft_library (ft_library);
    أرجع خالی_سوچک;
  }

  FT_Reference_Face (ft_face);
  hb_face_t *face = hb_ft_face_create (ft_face, _hb_ft_face_destroy_static);
  _hb_ft_face_destroy_static (ft_face);

  ft_face->generic.انکڑے = ft_library;
  ft_face->generic.finalizer = finalize_ft_library;

  إذا (hb_face_is_immutable (face))
    أرجع خالی_سوچک;

  أرجع face;
}

ساكن hb_user_انکڑے_کلید_t ft_blob_کلید = {0};

ساكن فراغ
_destroy_blob (فراغ *p)
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
				     بنانشانی صحيح  اشاریہ_2)
{
  FT_Library ft_library = reference_ft_library ();
  إذا (unlikely (!ft_library))
  {
    DEBUG_پیغام (FT, ft_library, "reference_ft_library failed");
    أرجع خالی_سوچک;
  }

  hb_blob_make_immutable (blob);
  بنانشانی blob_ناپ;
  ثابت حرف *blob_انکڑے = hb_blob_get_انکڑے (blob, &blob_ناپ);

  FT_Face ft_face;
  إذا (unlikely (new_ft_حافظہ_face (ft_library,
				    (ثابت FT_Byte *) blob_انکڑے,
				    blob_ناپ,
				    اشاریہ_2,
				    &ft_face)))
  {
    destroy_ft_library (ft_library);
    أرجع خالی_سوچک;
  }

  FT_Reference_Face (ft_face);
  hb_face_t *face = hb_ft_face_create (ft_face, _hb_ft_face_destroy_static);
  _hb_ft_face_destroy_static (ft_face);

  ft_face->generic.انکڑے = ft_library;
  ft_face->generic.finalizer = finalize_ft_library;

  إذا (hb_face_is_immutable (face))
    أرجع خالی_سوچک;

  // Hook the blob to the hb_face_t, since FT_Face still needs it.
  hb_blob_reference (blob);
  إذا (!hb_face_سیٹ_user_انکڑے (face, &ft_blob_کلید, blob, _destroy_blob, سچ))
  {
    hb_blob_destroy (blob);
    hb_face_destroy (face);
    أرجع خالی_سوچک;
  }

  أرجع face;
}

ساكن فراغ
_release_blob (فراغ *arg)
{
  FT_Face ft_face = (FT_Face) arg;
  hb_blob_destroy ((hb_blob_t *) ft_face->generic.انکڑے);
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
فراغ
hb_ft_font_سیٹ_funcs (hb_font_t *font)
{
  صحيح لدو_flags = FT_لدو_DEFAULT | FT_لدو_NO_HINTING;
  إذا (font->destroy == (hb_destroy_func_t) _hb_ft_font_destroy && font->user_انکڑے)
  {
    ثابت hb_ft_font_t *existing_ft_font = (ثابت hb_ft_font_t *) font->user_انکڑے;
    لدو_flags = existing_ft_font->لدو_flags;
  }

  // In case of failure...
  hb_font_سیٹ_funcs (font,
		     hb_font_funcs_get_خالی_تھاواں (),
		     خالی_سوچک, خالی_سوچک);

  hb_blob_t *blob = hb_face_reference_blob (font->face);
  بنانشانی صحيح blob_لمائی;
  ثابت حرف *blob_انکڑے = hb_blob_get_انکڑے (blob, &blob_لمائی);
  إذا (unlikely (!blob_لمائی))
    DEBUG_پیغام (FT, font, "Font face has empty blob");

  FT_Library ft_library = reference_ft_library ();
  إذا (unlikely (!ft_library))
  {
    hb_blob_destroy (blob);
    DEBUG_پیغام (FT, font, "reference_ft_library failed");
    أرجع;
  }

  FT_Face ft_face = خالی_سوچک;
  إذا (unlikely (new_ft_حافظہ_face (ft_library,
				    (ثابت FT_Byte *) blob_انکڑے,
				    blob_لمائی,
				    hb_face_get_اشاریہ (font->face),
				    &ft_face)))
  {
    hb_blob_destroy (blob);
    destroy_ft_library (ft_library);
    DEBUG_پیغام (FT, font, "FT_New_Memory_Face() failed");
    أرجع;
  }

  إذا (FT_Select_Charmap (ft_face, FT_ENCODING_MS_علامت))
    FT_Select_Charmap (ft_face, FT_ENCODING_UNICODE);

  // Hook the blob to the FT_Face
  ft_face->generic.انکڑے = blob;
  ft_face->generic.finalizer = _release_blob;

  // And the FT_Library to the blob
  إذا (unlikely (!hb_blob_سیٹ_user_انکڑے (blob, &ft_library_کلید, ft_library, destroy_ft_library, سچ)))
  {
    DEBUG_پیغام (FT, font, "hb_blob_set_user_data() failed");
    _hb_ft_face_destroy_static (ft_face);
    destroy_ft_library (ft_library);
    أرجع;
  }

  hb_ft_font_سیٹ_funcs_2 (font, ft_face, سچ, سچ);
  hb_ft_font_سیٹ_لدو_flags (font, لدو_flags);

  hb_ft_hb_font_بدلیا (font, ft_face);
}

#جے_انت
