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

#اگر_تعریف_شده HAVE_FREETYPE

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
#پایان_اگر


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


استفاده hb_ft_advance_cache_t = hb_cache_t<16, 24, 8, خطأ>;

ساكن فراغ _hb_ft_face_destroy_static (فراغ *داده);

بنية hb_ft_font_t
{
  صحيح بارگیری_flags;
  منطقي نماد; /* Whether selected cmap is symbol cmap. */
  منطقي unref; /* Whether to destroy ft_face when done. */
  منطقي transform; /* Whether to apply FT_Face's transform. */
  منطقي static_library; /* Whether ft_face uses static_ft_library. */

  تغییرپذیر hb_mutex_t lock; /* Protects members below. */
  FT_Face ft_face;
  تغییرپذیر hb_atomic_t<بیعلامت> cached_serial;
  تغییرپذیر hb_ft_advance_cache_t advance_cache;
};

ساكن hb_ft_font_t *
_hb_ft_font_create (FT_Face ft_face, منطقي نماد, منطقي unref, منطقي static_library)
{
  hb_ft_font_t *ft_font = (hb_ft_font_t *) hb_calloc (1, حجم (hb_ft_font_t));
  إذا (unlikely (!ft_font)) أرجع اشاره_تهی;

  ft_font->lock.init ();
  ft_font->ft_face = ft_face;
  ft_font->نماد = نماد;
  ft_font->unref = unref;
  ft_font->static_library = static_library;

  ft_font->بارگیری_flags = FT_بارگیری_DEFAULT | FT_بارگیری_NO_HINTING;

  ft_font->cached_serial = UINT_MAX;
  جديد (&ft_font->advance_cache) hb_ft_advance_cache_t;

  أرجع ft_font;
}

ساكن فراغ
_hb_ft_face_destroy (فراغ *داده)
{
  FT_انجامشده_Face ((FT_Face) داده);
}

ساكن فراغ
_hb_ft_font_destroy (فراغ *داده)
{
  hb_ft_font_t *ft_font = (hb_ft_font_t *) داده;

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
      hb_blob_t *blob = hb_blob_reference ((hb_blob_t *) ft_font->ft_face->generic.داده);
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
ساكن فراغ hb_ft_hb_font_تغییریافته (hb_font_t *font, FT_Face ft_face)
{
  إذا (unlikely (font->destroy != (hb_destroy_func_t) _hb_ft_font_destroy))
    أرجع;

  hb_ft_font_t *ft_font = (hb_ft_font_t *) font->user_داده;

  عائم x_mult = 1.f, y_mult = 1.f;

  إذا (font->x_scale < 0) x_mult = -x_mult;
  إذا (font->y_scale < 0) y_mult = -y_mult;

  إذا (FT_مجموعه_Char_اندازه (ft_face,
			قدر_مطلق (font->x_scale), قدر_مطلق (font->y_scale),
			0, 0
#إذا 0
		    font->x_ppem * 72 * 64 / font->x_scale,
		    font->y_ppem * 72 * 64 / font->y_scale
#پایان_اگر
     ) && ft_face->رقم_fixed_sizes)
  {
#اگر_تعریف_شده HAVE_FT_GET_TRANSFORM
    /* Bitmap font, eg. bitmap color emoji. */
    /* Pick largest size? */
    صحيح x_scale  = ft_face->موجود_sizes[ft_face->رقم_fixed_sizes - 1].x_ppem;
    صحيح y_scale = ft_face->موجود_sizes[ft_face->رقم_fixed_sizes - 1].y_ppem;
    FT_مجموعه_Char_اندازه (ft_face,
		      x_scale, y_scale,
		      0, 0);

    /* This contains the sign that was previously in x_mult/y_mult. */
    x_mult = (عائم) font->x_scale / x_scale;
    y_mult = (عائم) font->y_scale / y_scale;
#پایان_اگر
  }
  وإلا
  { /* Shrug */ }


  إذا (x_mult != 1.f || y_mult != 1.f)
  {
    FT_ماتریس ماتریس = { (صحيح) roundf (x_mult * (1<<16)), 0,
			  0, (صحيح) roundf (y_mult * (1<<16))};
    FT_مجموعه_Transform (ft_face, &ماتریس, اشاره_تهی);
    ft_font->transform = درست;
  }
  وإلا
    FT_مجموعه_Transform (ft_face, اشاره_تهی, اشاره_تهی);

#إذا defined(HAVE_FT_GET_VAR_BLEND_COORDINATES) && !defined(HB_NO_VAR)
  إذا (font->has_nonzero_coords)
  {
    بیعلامت صحيح رقم_coords;
    ثابت عائم *coords = hb_font_get_var_coords_design (font, &رقم_coords);
    FT_Fixed *ft_coords = (FT_Fixed *) hb_calloc (رقم_coords, حجم (FT_Fixed));
    إذا (ft_coords)
    {
      لكل (بیعلامت صحيح i = 0; i < رقم_coords; i++)
	  ft_coords[i] = coords[i] * 65536.f;
      FT_مجموعه_Var_Design_Coordinates (ft_face, رقم_coords, ft_coords);
      hb_free (ft_coords);
    }
  }
  وإلا إذا (font->رقم_coords)
  {
    // Some old versions of FreeType crash if we
    // call this function on non-variable fonts.
    FT_مجموعه_Var_Design_Coordinates (ft_face, 0, اشاره_تهی);
  }
#پایان_اگر
}

/* Check if hb_font changed, update FT_Face. */
ساكن مضمن منطقي
hb_ft_hb_font_check_تغییریافته (hb_font_t *font,
			      ثابت hb_ft_font_t *ft_font)
{
  إذا (font->serial != ft_font->cached_serial)
  {
    hb_قفل_t lock (ft_font->lock);
    hb_ft_hb_font_تغییریافته (font, ft_font->ft_face);
    ft_font->advance_cache.clear ();
    ft_font->cached_serial.مجموعه_release (font->serial.get_acquire ());
    أرجع درست;
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
hb_ft_font_مجموعه_بارگیری_flags (hb_font_t *font, صحيح بارگیری_flags)
{
  إذا (hb_object_is_immutable (font))
    أرجع;

  إذا (unlikely (font->destroy != (hb_destroy_func_t) _hb_ft_font_destroy))
    أرجع;

  hb_ft_font_t *ft_font = (hb_ft_font_t *) font->user_داده;

  ft_font->بارگیری_flags = بارگیری_flags;
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
hb_ft_font_get_بارگیری_flags (hb_font_t *font)
{
  إذا (unlikely (font->destroy != (hb_destroy_func_t) _hb_ft_font_destroy))
    أرجع 0;

  ثابت hb_ft_font_t *ft_font = (ثابت hb_ft_font_t *) font->user_داده;

  أرجع ft_font->بارگیری_flags;
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
    أرجع اشاره_تهی;

  ثابت hb_ft_font_t *ft_font = (ثابت hb_ft_font_t *) font->user_داده;

  أرجع ft_font->ft_face;
}

#اگر_تعریف_نشده HB_DISABLE_DEPRECATED

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

#پایان_اگر

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
hb_ft_font_قفل_face (hb_font_t *font)
{
  إذا (unlikely (font->destroy != (hb_destroy_func_t) _hb_ft_font_destroy))
    أرجع اشاره_تهی;

  ثابت hb_ft_font_t *ft_font = (ثابت hb_ft_font_t *) font->user_داده;

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

  ثابت hb_ft_font_t *ft_font = (ثابت hb_ft_font_t *) font->user_داده;

  ft_font->lock.unlock ();
}


ساكن hb_bool_t
hb_ft_get_nominal_glyph (hb_font_t *font,
			 فراغ *font_داده,
			 hb_codepoint_t unicode,
			 hb_codepoint_t *glyph,
			 فراغ *user_داده HB_استفادهنشده)
{
  ثابت hb_ft_font_t *ft_font = (ثابت hb_ft_font_t *) font_داده;
  hb_قفل_t lock (ft_font->lock);
  بیعلامت صحيح g = FT_Get_Char_شاخص (ft_font->ft_face, unicode);

  إذا (unlikely (!g))
  {
    إذا (unlikely (ft_font->نماد))
    {
      بدل ((بیعلامت) font->face->جدول.OS2->get_font_صفحه ()) {
      حالة OT::OS2::font_صفحه_t::FONT_صفحه_NONE:
	إذا (unicode <= 0x00FFu)
	  /* For symbol-encoded OpenType fonts, we duplicate the
	   * U+F000..F0FF range at U+0000..U+00FF.  That's what
	   * Windows seems to do, and that's hinted about at:
	   * https://docs.microsoft.com/en-us/typography/opentype/spec/recom
	   * under "Non-Standard (Symbol) Fonts". */
	  g = FT_Get_Char_شاخص (ft_font->ft_face, 0xF000u + unicode);
	اكسر;
#اگر_تعریف_نشده HB_NO_OT_SHAPER_ARABIC_FALLBACK
      حالة OT::OS2::font_صفحه_t::FONT_صفحه_SIMP_ARABIC:
	g = FT_Get_Char_شاخص (ft_font->ft_face, _hb_arabic_pua_simp_map (unicode));
	اكسر;
      حالة OT::OS2::font_صفحه_t::FONT_صفحه_TRAD_ARABIC:
	g = FT_Get_Char_شاخص (ft_font->ft_face, _hb_arabic_pua_trad_map (unicode));
	اكسر;
#پایان_اگر
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
  أرجع درست;
}

ساكن بیعلامت صحيح
hb_ft_get_nominal_glyphs (hb_font_t *font HB_استفادهنشده,
			  فراغ *font_داده,
			  بیعلامت صحيح شمار,
			  ثابت hb_codepoint_t *اولی_unicode,
			  بیعلامت صحيح unicode_stride,
			  hb_codepoint_t *اولی_glyph,
			  بیعلامت صحيح glyph_stride,
			  فراغ *user_داده HB_استفادهنشده)
{
  ثابت hb_ft_font_t *ft_font = (ثابت hb_ft_font_t *) font_داده;
  hb_قفل_t lock (ft_font->lock);
  بیعلامت صحيح انجامشده;
  لكل (انجامشده = 0;
       انجامشده < شمار && (*اولی_glyph = FT_Get_Char_شاخص (ft_font->ft_face, *اولی_unicode));
       انجامشده++)
  {
    اولی_unicode = &Struct_At_جابه_جایی_Unaligned<hb_codepoint_t> (اولی_unicode, unicode_stride);
    اولی_glyph = &Struct_At_جابه_جایی_Unaligned<hb_codepoint_t> (اولی_glyph, glyph_stride);
  }
  /* We don't need to do ft_font->symbol dance here, since HB calls the singular
   * nominal_glyph() for what we don't handle here. */
  أرجع انجامشده;
}


ساكن hb_bool_t
hb_ft_get_variation_glyph (hb_font_t *font HB_استفادهنشده,
			   فراغ *font_داده,
			   hb_codepoint_t unicode,
			   hb_codepoint_t variation_selector,
			   hb_codepoint_t *glyph,
			   فراغ *user_داده HB_استفادهنشده)
{
  ثابت hb_ft_font_t *ft_font = (ثابت hb_ft_font_t *) font_داده;
  hb_قفل_t lock (ft_font->lock);
  بیعلامت صحيح g = FT_Face_Get_Char_Variant_شاخص (ft_font->ft_face, unicode, variation_selector);

  إذا (unlikely (!g))
    أرجع خطأ;

  *glyph = g;
  أرجع درست;
}

ساكن فراغ
hb_ft_get_glyph_h_advances (hb_font_t* font, فراغ* font_داده,
			    بیعلامت شمار,
			    ثابت hb_codepoint_t *اولی_glyph,
			    بیعلامت glyph_stride,
			    hb_position_t *اولی_advance,
			    بیعلامت advance_stride,
			    فراغ *user_داده HB_استفادهنشده)
{
  ثابت hb_ft_font_t *ft_font = (ثابت hb_ft_font_t *) font_داده;
  hb_ft_hb_font_check_تغییریافته (font, ft_font);

  hb_قفل_t lock (ft_font->lock);
  FT_Face ft_face = ft_font->ft_face;
  صحيح بارگیری_flags = ft_font->بارگیری_flags;
  عائم x_mult;
#اگر_تعریف_شده HAVE_FT_GET_TRANSFORM
  إذا (ft_font->transform)
  {
    FT_ماتریس ماتریس;
    FT_Get_Transform (ft_face, &ماتریس, اشاره_تهی);
    x_mult = sqrtf ((عائم)ماتریس.xx * ماتریس.xx + (عائم)ماتریس.xy * ماتریس.xy) / 65536.f;
    x_mult *= font->x_scale < 0 ? -1 : +1;
  }
  وإلا
#پایان_اگر
  {
    x_mult = font->x_scale < 0 ? -1 : +1;
  }

  لكل (بیعلامت صحيح i = 0; i < شمار; i++)
  {
    hb_position_t advance;
    hb_codepoint_t glyph = *اولی_glyph;

    بیعلامت صحيح cv;
    إذا (ft_font->advance_cache.get (glyph, &cv))
      advance = (hb_position_t) cv;
    وإلا
    {
      FT_Fixed v = 0;
      FT_Get_Advance (ft_face, glyph, بارگیری_flags, &v);
      /* Work around bug that FreeType seems to return negative advance
       * for variable-set fonts if x_scale is negative! */
      مزدوج scaled = trunc (fabs ((مزدوج) v) * (مزدوج) x_mult + (1<<9)) / (1<<10);
      advance = hb_clamp_مقصد<hb_position_t> (floor (scaled));
      ft_font->advance_cache.مجموعه (glyph, advance);
    }

    *اولی_advance = advance;
    اولی_glyph = &Struct_At_جابه_جایی_Unaligned<hb_codepoint_t> (اولی_glyph, glyph_stride);
    اولی_advance = &Struct_At_جابه_جایی_Unaligned<hb_position_t> (اولی_advance, advance_stride);
  }
}

#اگر_تعریف_نشده HB_NO_VERTICAL
ساكن hb_position_t
hb_ft_get_glyph_v_advance (hb_font_t *font,
			   فراغ *font_داده,
			   hb_codepoint_t glyph,
			   فراغ *user_داده HB_استفادهنشده)
{
  ثابت hb_ft_font_t *ft_font = (ثابت hb_ft_font_t *) font_داده;
  hb_ft_hb_font_check_تغییریافته (font, ft_font);

  hb_قفل_t lock (ft_font->lock);
  FT_Fixed v;
  عائم y_mult;
#اگر_تعریف_شده HAVE_FT_GET_TRANSFORM
  إذا (ft_font->transform)
  {
    FT_ماتریس ماتریس;
    FT_Get_Transform (ft_font->ft_face, &ماتریس, اشاره_تهی);
    y_mult = sqrtf ((عائم)ماتریس.yx * ماتریس.yx + (عائم)ماتریس.yy * ماتریس.yy) / 65536.f;
    y_mult *= font->y_scale < 0 ? -1 : +1;
  }
  وإلا
#پایان_اگر
  {
    y_mult = font->y_scale < 0 ? -1 : +1;
  }

  إذا (unlikely (FT_Get_Advance (ft_font->ft_face, glyph, ft_font->بارگیری_flags | FT_بارگیری_VERTICAL_LAYOUT, &v)))
    أرجع 0;

  /* Note: FreeType's vertical metrics grows downward while other FreeType coordinates
   * have a Y growing upward.  Hence the extra negation. */
  مزدوج advance = floor ((-(مزدوج) v + (1<<9)) / (1<<10));

  أرجع hb_clamp_مقصد<hb_position_t> ((مزدوج) y_mult * advance);
}
#پایان_اگر

#اگر_تعریف_نشده HB_NO_VERTICAL
ساكن hb_bool_t
hb_ft_get_glyph_v_origin (hb_font_t *font,
			  فراغ *font_داده,
			  hb_codepoint_t glyph,
			  hb_position_t *x,
			  hb_position_t *y,
			  فراغ *user_داده HB_استفادهنشده)
{
  ثابت hb_ft_font_t *ft_font = (ثابت hb_ft_font_t *) font_داده;
  hb_ft_hb_font_check_تغییریافته (font, ft_font);

  hb_قفل_t lock (ft_font->lock);
  FT_Face ft_face = ft_font->ft_face;
  عائم x_mult, y_mult;
#اگر_تعریف_شده HAVE_FT_GET_TRANSFORM
  إذا (ft_font->transform)
  {
    FT_ماتریس ماتریس;
    FT_Get_Transform (ft_face, &ماتریس, اشاره_تهی);
    x_mult = sqrtf ((عائم)ماتریس.xx * ماتریس.xx + (عائم)ماتریس.xy * ماتریس.xy) / 65536.f;
    x_mult *= font->x_scale < 0 ? -1 : +1;
    y_mult = sqrtf ((عائم)ماتریس.yx * ماتریس.yx + (عائم)ماتریس.yy * ماتریس.yy) / 65536.f;
    y_mult *= font->y_scale < 0 ? -1 : +1;
  }
  وإلا
#پایان_اگر
  {
    x_mult = font->x_scale < 0 ? -1 : +1;
    y_mult = font->y_scale < 0 ? -1 : +1;
  }

  إذا (unlikely (FT_بارگیری_Glyph (ft_face, glyph, ft_font->بارگیری_flags)))
    أرجع خطأ;

  /* Note: FreeType's vertical metrics grows downward while other FreeType coordinates
   * have a Y growing upward.  Hence the extra negation. */
  *x = hb_clamp_مقصد<hb_position_t> ((مزدوج) x_mult *
				   ((مزدوج) ft_face->glyph->metrics.horiBearingX -
				    ft_face->glyph->metrics.vertBearingX));
  *y = hb_clamp_مقصد<hb_position_t> ((مزدوج) y_mult *
				   ((مزدوج) ft_face->glyph->metrics.horiBearingY +
				    ft_face->glyph->metrics.vertBearingY));

  أرجع درست;
}
#پایان_اگر

#اگر_تعریف_نشده HB_NO_OT_SHAPE_FALLBACK
ساكن hb_position_t
hb_ft_get_glyph_h_kerning (hb_font_t *font,
			   فراغ *font_داده,
			   hb_codepoint_t چپ_glyph,
			   hb_codepoint_t راست_glyph,
			   فراغ *user_داده HB_استفادهنشده)
{
  ثابت hb_ft_font_t *ft_font = (ثابت hb_ft_font_t *) font_داده;
  hb_ft_hb_font_check_تغییریافته (font, ft_font);

  hb_قفل_t lock (ft_font->lock);
  FT_وکتور kerningv;

  FT_Kerning_نما نما = font->x_ppem ? FT_KERNING_DEFAULT : FT_KERNING_UNFITTED;
  إذا (FT_Get_Kerning (ft_font->ft_face, چپ_glyph, راست_glyph, نما, &kerningv))
    أرجع 0;

  أرجع hb_clamp_مقصد<hb_position_t> ((int64_t) kerningv.x);
}
#پایان_اگر

ساكن منطقي
hb_ft_is_colr_glyph (hb_font_t *font,
		     فراغ *font_داده,
		     hb_codepoint_t gid)
{
#اگر_تعریف_نشده HB_NO_PAINT
#إذا (FREETYPE_MAJOR*10000 + FREETYPE_MINOR*100 + FREETYPE_PATCH) >= 21300
  ثابت hb_ft_font_t *ft_font = (ثابت hb_ft_font_t *) font_داده;
  FT_Face ft_face = ft_font->ft_face;


  /* COLRv1 */
  FT_OpaquePaint paint = {0};
  إذا (FT_Get_Color_Glyph_Paint (ft_face, gid,
			        FT_COLOR_NO_ریشه_TRANSFORM,
			        &paint))
    أرجع درست;

  /* COLRv0 */
  FT_LayerIterator  iterator;
  FT_UInt  layer_glyph_شاخص;
  FT_UInt  layer_color_شاخص;
  iterator.p  = عدم;
  إذا (FT_Get_Color_Glyph_Layer (ft_face,
				gid,
				&layer_glyph_شاخص,
				&layer_color_شاخص,
				&iterator))
    أرجع درست;
#پایان_اگر
#پایان_اگر

  أرجع خطأ;
}

ساكن hb_bool_t
hb_ft_get_glyph_extents (hb_font_t *font,
			 فراغ *font_داده,
			 hb_codepoint_t glyph,
			 hb_glyph_extents_t *extents,
			 فراغ *user_داده HB_استفادهنشده)
{
  // FreeType doesn't return COLR glyph extents.
  إذا (hb_ft_is_colr_glyph (font, font_داده, glyph))
    أرجع خطأ;

  ثابت hb_ft_font_t *ft_font = (ثابت hb_ft_font_t *) font_داده;
  hb_ft_hb_font_check_تغییریافته (font, ft_font);

  hb_قفل_t lock (ft_font->lock);
  FT_Face ft_face = ft_font->ft_face;
  عائم x_mult, y_mult;

#اگر_تعریف_شده HAVE_FT_GET_TRANSFORM
  إذا (ft_font->transform)
  {
    FT_ماتریس ماتریس;
    FT_Get_Transform (ft_face, &ماتریس, اشاره_تهی);
    x_mult = sqrtf ((عائم)ماتریس.xx * ماتریس.xx + (عائم)ماتریس.xy * ماتریس.xy) / 65536.f;
    x_mult *= font->x_scale < 0 ? -1 : +1;
    y_mult = sqrtf ((عائم)ماتریس.yx * ماتریس.yx + (عائم)ماتریس.yy * ماتریس.yy) / 65536.f;
    y_mult *= font->y_scale < 0 ? -1 : +1;
  }
  وإلا
#پایان_اگر
  {
    x_mult = font->x_scale < 0 ? -1 : +1;
    y_mult = font->y_scale < 0 ? -1 : +1;
  }

  إذا (unlikely (FT_بارگیری_Glyph (ft_face, glyph, ft_font->بارگیری_flags)))
    أرجع خطأ;

  /* Copied from hb_font_t::scale_glyph_extents. */

  عائم x1 = x_mult * ft_face->glyph->metrics.horiBearingX;
  عائم y1 = y_mult * ft_face->glyph->metrics.horiBearingY;
  عائم x2 = x1 + x_mult *  ft_face->glyph->metrics.عرض;
  عائم y2 = y1 + y_mult * -ft_face->glyph->metrics.ارتفاع;

  مزدوج rx1 = (مزدوج) roundf (x1);
  مزدوج ry1 = (مزدوج) roundf (y1);
  مزدوج rx2 = (مزدوج) roundf (x2);
  مزدوج ry2 = (مزدوج) roundf (y2);

  extents->x_bearing = hb_clamp_مقصد<hb_position_t> (rx1);
  extents->y_bearing = hb_clamp_مقصد<hb_position_t> (ry1);
  extents->عرض = hb_clamp_مقصد<hb_position_t> (rx2 - rx1);
  extents->ارتفاع = hb_clamp_مقصد<hb_position_t> (ry2 - ry1);

  أرجع درست;
}

ساكن hb_bool_t
hb_ft_get_glyph_contour_نقطه (hb_font_t *font HB_استفادهنشده,
			       فراغ *font_داده,
			       hb_codepoint_t glyph,
			       بیعلامت صحيح نقطه_شاخص,
			       hb_position_t *x,
			       hb_position_t *y,
			       فراغ *user_داده HB_استفادهنشده)
{
  ثابت hb_ft_font_t *ft_font = (ثابت hb_ft_font_t *) font_داده;
  hb_ft_hb_font_check_تغییریافته (font, ft_font);

  hb_قفل_t lock (ft_font->lock);
  FT_Face ft_face = ft_font->ft_face;

  إذا (unlikely (FT_بارگیری_Glyph (ft_face, glyph, ft_font->بارگیری_flags)))
      أرجع خطأ;

  إذا (unlikely (ft_face->glyph->format != FT_GLYPH_FORMAT_OUTLINE))
      أرجع خطأ;

  إذا (unlikely (نقطه_شاخص >= (بیعلامت صحيح) ft_face->glyph->outline.n_points))
      أرجع خطأ;

  *x = ft_face->glyph->outline.points[نقطه_شاخص].x;
  *y = ft_face->glyph->outline.points[نقطه_شاخص].y;

  أرجع درست;
}

ساكن hb_bool_t
hb_ft_get_glyph_name (hb_font_t *font HB_استفادهنشده,
		      فراغ *font_داده,
		      hb_codepoint_t glyph,
		      حرف *name, بیعلامت صحيح اندازه,
		      فراغ *user_داده HB_استفادهنشده)
{
  ثابت hb_ft_font_t *ft_font = (ثابت hb_ft_font_t *) font_داده;
  hb_قفل_t lock (ft_font->lock);
  FT_Face ft_face = ft_font->ft_face;

  إذا (!اندازه)
  {
    حرف buf[128];
    أرجع !FT_Get_Glyph_Name (ft_face, glyph, buf, حجم (buf)) && *buf;
  }

  hb_bool_t ret = !FT_Get_Glyph_Name (ft_face, glyph, name, اندازه);
  إذا (ret && (اندازه && !*name))
    ret = خطأ;

  أرجع ret;
}

ساكن hb_bool_t
hb_ft_get_glyph_from_name (hb_font_t *font HB_استفادهنشده,
			   فراغ *font_داده,
			   ثابت حرف *name, صحيح len, /* -1 means nul-terminated */
			   hb_codepoint_t *glyph,
			   فراغ *user_داده HB_استفادهنشده)
{
  ثابت hb_ft_font_t *ft_font = (ثابت hb_ft_font_t *) font_داده;
  hb_قفل_t lock (ft_font->lock);
  FT_Face ft_face = ft_font->ft_face;

  إذا (len < 0)
    *glyph = FT_Get_Name_شاخص (ft_face, (FT_String *) name);
  وإلا {
    /* Make a nul-terminated version. */
    حرف buf[128];
    len = hb_min (len, (صحيح) حجم (buf) - 1);
    strncpy (buf, name, len);
    buf[len] = '\0';
    *glyph = FT_Get_Name_شاخص (ft_face, buf);
  }

  إذا (*glyph == 0)
  {
    /* Check whether the given name was actually the name of glyph 0. */
    حرف buf[128];
    إذا (!FT_Get_Glyph_Name(ft_face, 0, buf, حجم (buf)) &&
	len < 0 ? !strcmp (buf, name) : !مقایسه_رشته (buf, name, len))
      أرجع درست;
  }

  أرجع *glyph != 0;
}

ساكن hb_bool_t
hb_ft_get_font_h_extents (hb_font_t *font HB_استفادهنشده,
			  فراغ *font_داده,
			  hb_font_extents_t *metrics,
			  فراغ *user_داده HB_استفادهنشده)
{
  ثابت hb_ft_font_t *ft_font = (ثابت hb_ft_font_t *) font_داده;
  hb_ft_hb_font_check_تغییریافته (font, ft_font);

  hb_قفل_t lock (ft_font->lock);
  FT_Face ft_face = ft_font->ft_face;
  عائم y_mult;
#اگر_تعریف_شده HAVE_FT_GET_TRANSFORM
  إذا (ft_font->transform)
  {
    FT_ماتریس ماتریس;
    FT_Get_Transform (ft_face, &ماتریس, اشاره_تهی);
    y_mult = sqrtf ((عائم)ماتریس.yx * ماتریس.yx + (عائم)ماتریس.yy * ماتریس.yy) / 65536.f;
    y_mult *= font->y_scale < 0 ? -1 : +1;
  }
  وإلا
#پایان_اگر
  {
    y_mult = font->y_scale < 0 ? -1 : +1;
  }

  إذا (ft_face->units_در_هر_EM != 0)
  {
    مزدوج ascender = FT_MulFix(ft_face->ascender, ft_face->اندازه->metrics.y_scale);
    مزدوج descender = FT_MulFix(ft_face->descender, ft_face->اندازه->metrics.y_scale);
    مزدوج ارتفاع = FT_MulFix( ft_face->ارتفاع, ft_face->اندازه->metrics.y_scale );
    metrics->ascender = hb_clamp_مقصد<hb_position_t> ((مزدوج) y_mult * ascender);
    metrics->descender = hb_clamp_مقصد<hb_position_t> ((مزدوج) y_mult * descender);
    metrics->سطر_gap = hb_clamp_مقصد<hb_position_t> ((مزدوج) y_mult * (ارتفاع - (ascender - descender)));
  }
  وإلا
  {
    /* Bitmap-only font, eg. color bitmap font. */
    مزدوج ascender = ft_face->اندازه->metrics.ascender;
    مزدوج descender = ft_face->اندازه->metrics.descender;
    مزدوج ارتفاع = ft_face->اندازه->metrics.ارتفاع;
    metrics->ascender = hb_clamp_مقصد<hb_position_t> ((مزدوج) y_mult * ascender);
    metrics->descender = hb_clamp_مقصد<hb_position_t> ((مزدوج) y_mult * descender);
    metrics->سطر_gap = hb_clamp_مقصد<hb_position_t> ((مزدوج) y_mult * (ارتفاع - (ascender - descender)));
  }

  أرجع درست;
}

#اگر_تعریف_نشده HB_NO_DRAW

ساكن صحيح
hb_ft_move_مقصد (ثابت FT_وکتور *to,
		فراغ *arg)
{
  hb_draw_session_t *drawing = (hb_draw_session_t *) arg;
  drawing->move_مقصد (to->x, to->y);
  أرجع FT_Err_Ok;
}

ساكن صحيح
hb_ft_سطر_مقصد (ثابت FT_وکتور *to,
		فراغ *arg)
{
  hb_draw_session_t *drawing = (hb_draw_session_t *) arg;
  drawing->سطر_مقصد (to->x, to->y);
  أرجع FT_Err_Ok;
}

ساكن صحيح
hb_ft_conic_مقصد (ثابت FT_وکتور *control,
		 ثابت FT_وکتور *to,
		 فراغ *arg)
{
  hb_draw_session_t *drawing = (hb_draw_session_t *) arg;
  drawing->quadratic_مقصد (control->x, control->y,
			 to->x, to->y);
  أرجع FT_Err_Ok;
}

ساكن صحيح
hb_ft_cubic_مقصد (ثابت FT_وکتور *control1,
		 ثابت FT_وکتور *control2,
		 ثابت FT_وکتور *to,
		 فراغ *arg)
{
  hb_draw_session_t *drawing = (hb_draw_session_t *) arg;
  drawing->cubic_مقصد (control1->x, control1->y,
		     control2->x, control2->y,
		     to->x, to->y);
  أرجع FT_Err_Ok;
}

ساكن hb_bool_t
hb_ft_draw_glyph_or_fail (hb_font_t *font,
			  فراغ *font_داده,
			  hb_codepoint_t glyph,
			  hb_draw_funcs_t *draw_funcs, فراغ *draw_داده,
			  فراغ *user_داده HB_استفادهنشده)
{
  ثابت hb_ft_font_t *ft_font = (ثابت hb_ft_font_t *) font_داده;
  hb_ft_hb_font_check_تغییریافته (font, ft_font);

  hb_قفل_t lock (ft_font->lock);
  FT_Face ft_face = ft_font->ft_face;

  إذا (unlikely (FT_بارگیری_Glyph (ft_face, glyph,
			       FT_بارگیری_NO_BITMAP | ft_font->بارگیری_flags)))
    أرجع خطأ;

  إذا (ft_face->glyph->format != FT_GLYPH_FORMAT_OUTLINE)
    أرجع خطأ;

  ثابت FT_Outline_Funcs outline_funcs = {
    hb_ft_move_مقصد,
    hb_ft_سطر_مقصد,
    hb_ft_conic_مقصد,
    hb_ft_cubic_مقصد,
    0, /*لغزش*/
    0, /* delta */
  };

  hb_draw_session_t draw_session {draw_funcs, draw_داده};

  FT_Outline_Decompose (&ft_face->glyph->outline,
			&outline_funcs,
			&draw_session);

  أرجع درست;
}
#پایان_اگر

#اگر_تعریف_نشده HB_NO_PAINT
#إذا (FREETYPE_MAJOR*10000 + FREETYPE_MINOR*100 + FREETYPE_PATCH) >= 21300

#ضمن "hb-ft-colr.hh"

ساكن hb_bool_t
hb_ft_paint_glyph_or_fail (hb_font_t *font,
			   فراغ *font_داده,
			   hb_codepoint_t gid,
			   hb_paint_funcs_t *paint_funcs, فراغ *paint_داده,
			   بیعلامت صحيح palette_شاخص,
			   hb_color_t foreground,
			   فراغ *user_داده)
{
  ثابت hb_ft_font_t *ft_font = (ثابت hb_ft_font_t *) font_داده;
  hb_ft_hb_font_check_تغییریافته (font, ft_font);

  hb_قفل_t lock (ft_font->lock);
  FT_Face ft_face = ft_font->ft_face;

  FT_Long بارگیری_flags = ft_font->بارگیری_flags | FT_بارگیری_COLOR;
#إذا (FREETYPE_MAJOR*10000 + FREETYPE_MINOR*100 + FREETYPE_PATCH) >= 21301
  بارگیری_flags |= FT_بارگیری_NO_SVG;
#پایان_اگر

  /* We release the lock before calling into glyph callbacks, such that
   * eg. draw API can call back into the face.*/

  إذا (unlikely (FT_بارگیری_Glyph (ft_face, gid, بارگیری_flags)))
    أرجع خطأ;

  إذا (ft_face->glyph->format == FT_GLYPH_FORMAT_OUTLINE)
  {
    إذا (hb_ft_paint_glyph_colr (font, font_داده, gid,
				paint_funcs, paint_داده,
				palette_شاخص, foreground,
				user_داده))
      أرجع درست;

    // Outline glyph
    أرجع خطأ;
  }

  خودکار *glyph = ft_face->glyph;
  إذا (glyph->format == FT_GLYPH_FORMAT_BITMAP)
  {
    منطقي ret = خطأ;
    خودکار &bitmap = glyph->bitmap;
    إذا (bitmap.pixel_نما == FT_PIXEL_نما_BGRA)
    {
      إذا (bitmap.pitch != (علامتدار) bitmap.عرض * 4)
        أرجع ret;

      ft_font->lock.unlock ();

      hb_blob_t *blob = hb_blob_create ((ثابت حرف *) bitmap.میانگیر,
					bitmap.pitch * bitmap.ردیفها,
					HB_حافظه_نما_DUPLICATE,
					اشاره_تهی, اشاره_تهی);

      hb_glyph_extents_t extents;
      إذا (!font->get_glyph_extents (gid, &extents, خطأ))
	اذهب بیرون;

      إذا (paint_funcs->image (paint_داده,
			      blob,
			      bitmap.عرض,
			      bitmap.ردیفها,
			      HB_PAINT_IMAGE_FORMAT_BGRA,
			      0.f,
			      &extents))
        ret = درست;

    بیرون:
      hb_blob_destroy (blob);
      ft_font->lock.lock ();
    }

    أرجع ret;
  }
  أرجع خطأ;
}
#پایان_اگر
#پایان_اگر


ساكن مضمن فراغ free_static_ft_funcs ();

ساكن بنية hb_ft_font_funcs_lazy_loader_t : hb_font_funcs_lazy_loader_t<hb_ft_font_funcs_lazy_loader_t>
{
  ساكن hb_font_funcs_t *create ()
  {
    hb_font_funcs_t *funcs = hb_font_funcs_create ();

    hb_font_funcs_مجموعه_nominal_glyph_func (funcs, hb_ft_get_nominal_glyph, اشاره_تهی, اشاره_تهی);
    hb_font_funcs_مجموعه_nominal_glyphs_func (funcs, hb_ft_get_nominal_glyphs, اشاره_تهی, اشاره_تهی);
    hb_font_funcs_مجموعه_variation_glyph_func (funcs, hb_ft_get_variation_glyph, اشاره_تهی, اشاره_تهی);

    hb_font_funcs_مجموعه_font_h_extents_func (funcs, hb_ft_get_font_h_extents, اشاره_تهی, اشاره_تهی);
    hb_font_funcs_مجموعه_glyph_h_advances_func (funcs, hb_ft_get_glyph_h_advances, اشاره_تهی, اشاره_تهی);

#اگر_تعریف_نشده HB_NO_VERTICAL
    hb_font_funcs_مجموعه_glyph_v_advance_func (funcs, hb_ft_get_glyph_v_advance, اشاره_تهی, اشاره_تهی);
    hb_font_funcs_مجموعه_glyph_v_origin_func (funcs, hb_ft_get_glyph_v_origin, اشاره_تهی, اشاره_تهی);
#پایان_اگر

#اگر_تعریف_نشده HB_NO_OT_SHAPE_FALLBACK
    hb_font_funcs_مجموعه_glyph_h_kerning_func (funcs, hb_ft_get_glyph_h_kerning, اشاره_تهی, اشاره_تهی);
#پایان_اگر
    hb_font_funcs_مجموعه_glyph_extents_func (funcs, hb_ft_get_glyph_extents, اشاره_تهی, اشاره_تهی);
    hb_font_funcs_مجموعه_glyph_contour_نقطه_func (funcs, hb_ft_get_glyph_contour_نقطه, اشاره_تهی, اشاره_تهی);
    hb_font_funcs_مجموعه_glyph_name_func (funcs, hb_ft_get_glyph_name, اشاره_تهی, اشاره_تهی);
    hb_font_funcs_مجموعه_glyph_from_name_func (funcs, hb_ft_get_glyph_from_name, اشاره_تهی, اشاره_تهی);

#اگر_تعریف_نشده HB_NO_DRAW
    hb_font_funcs_مجموعه_draw_glyph_or_fail_func (funcs, hb_ft_draw_glyph_or_fail, اشاره_تهی, اشاره_تهی);
#پایان_اگر

#اگر_تعریف_نشده HB_NO_PAINT
#إذا (FREETYPE_MAJOR*10000 + FREETYPE_MINOR*100 + FREETYPE_PATCH) >= 21300
    hb_font_funcs_مجموعه_paint_glyph_or_fail_func (funcs, hb_ft_paint_glyph_or_fail, اشاره_تهی, اشاره_تهی);
#پایان_اگر
#پایان_اگر

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
hb_ft_font_مجموعه_funcs_2 (hb_font_t *font, FT_Face ft_face, منطقي unref,
		       منطقي static_library = خطأ)
{
  منطقي نماد = ft_face->charmap && ft_face->charmap->encoding == FT_ENCODING_MS_نماد;

  hb_ft_font_t *ft_font = _hb_ft_font_create (ft_face, نماد, unref, static_library);
  إذا (unlikely (!ft_font)) أرجع;

  hb_font_مجموعه_funcs (font,
		     _hb_ft_get_font_funcs (),
		     ft_font,
		     _hb_ft_font_destroy);
}


ساكن hb_blob_t *
hb_ft_reference_جدول (hb_face_t *face HB_استفادهنشده, hb_برچسب_t برچسب_2, فراغ *user_داده)
{
  FT_Face ft_face = (FT_Face) user_داده;
  FT_Byte *میانگیر;
  FT_ULong  درازی = 0;
  FT_Error error;

  /* In new FreeType, a tag value of 1 loads the SFNT table directory. Reject it. */
  إذا (برچسب_2 == 1)
    أرجع اشاره_تهی;

  /* Note: FreeType like HarfBuzz uses the NONE tag for fetching the entire blob */

  error = FT_بارگیری_Sfnt_جدول (ft_face, برچسب_2, 0, اشاره_تهی, &درازی);
  إذا (error)
    أرجع اشاره_تهی;

  میانگیر = (FT_Byte *) hb_malloc (درازی);
  إذا (!میانگیر)
    أرجع اشاره_تهی;
  خودکار میانگیر_guard = hb_make_scope_guard ([&]() { hb_free (میانگیر); });

  error = FT_بارگیری_Sfnt_جدول (ft_face, برچسب_2, 0, میانگیر, &درازی);
  إذا (error)
    أرجع اشاره_تهی;

  میانگیر_guard.release ();
  أرجع hb_blob_create ((ثابت حرف *) میانگیر, درازی,
			 HB_حافظه_نما_WRITABLE,
			 میانگیر, hb_free);
}

ساكن بیعلامت
hb_ft_get_جدول_برچسبها (ثابت hb_face_t *face HB_استفادهنشده,
		       بیعلامت صحيح شروع_جابه_جایی,
		       بیعلامت صحيح *جدول_شمار_2,
		       hb_برچسب_t *جدول_برچسبها,
		       فراغ *user_داده)
{
  FT_Face ft_face = (FT_Face) user_داده;

  FT_ULong population = 0;
  FT_Sfnt_جدول_Info (ft_face,
		      0, // table_index; ignored
		      اشاره_تهی,
                      &population);

  إذا (!جدول_شمار_2)
    أرجع population;

  إذا (unlikely (شروع_جابه_جایی >= population))
  {
    *جدول_شمار_2 = 0;
    أرجع population;
  }

  بیعلامت انتها_جابه_جایی = شروع_جابه_جایی + *جدول_شمار_2;
  إذا (unlikely (انتها_جابه_جایی < شروع_جابه_جایی)) /* Overflow. */
  {
    *جدول_شمار_2 = 0;
    أرجع population;
  }
  انتها_جابه_جایی = hb_min (انتها_جابه_جایی, (بیعلامت) population);

  *جدول_شمار_2 = انتها_جابه_جایی - شروع_جابه_جایی;
  إذا (جدول_برچسبها)
    لكل (بیعلامت i = شروع_جابه_جایی; i < انتها_جابه_جایی; i++)
    {
      FT_ULong برچسب_2 = 0, درازی;
      FT_Sfnt_جدول_Info (ft_face, i, &برچسب_2, &درازی);
      جدول_برچسبها[i - شروع_جابه_جایی] = برچسب_2;
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

  إذا (!ft_face->جریان->اقرأ) {
    hb_blob_t *blob;

    blob = hb_blob_create ((ثابت حرف *) ft_face->جریان->پایه,
			   (بیعلامت صحيح) ft_face->جریان->اندازه,
			   HB_حافظه_نما_READONLY,
			   ft_face, destroy);
    face = hb_face_create (blob, ft_face->face_شاخص);
    hb_blob_destroy (blob);
  } وإلا {
    face = hb_face_create_for_tables (hb_ft_reference_جدول, ft_face, destroy);
    hb_face_مجموعه_get_جدول_برچسبها_func (face, hb_ft_get_جدول_برچسبها, ft_face, اشاره_تهی);
  }

  hb_face_مجموعه_شاخص (face, ft_face->face_شاخص);
  hb_face_مجموعه_upem (face, ft_face->units_در_هر_EM);

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
  hb_face_destroy ((hb_face_t *) ft_face->generic.داده);
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
  إذا (unlikely (!ft_face->generic.داده || ft_face->generic.finalizer != hb_ft_face_finalize))
  {
    إذا (ft_face->generic.finalizer)
      ft_face->generic.finalizer (ft_face);

    ft_face->generic.داده = hb_ft_face_create (ft_face, اشاره_تهی);
    ft_face->generic.finalizer = hb_ft_face_finalize;
  }

  أرجع hb_face_reference ((hb_face_t *) ft_face->generic.داده);
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
  hb_ft_font_مجموعه_funcs_2 (font, ft_face, خطأ);
  hb_ft_font_تغییریافته (font);
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
hb_ft_font_تغییریافته (hb_font_t *font)
{
  إذا (font->destroy != (hb_destroy_func_t) _hb_ft_font_destroy)
    أرجع;

  hb_ft_font_t *ft_font = (hb_ft_font_t *) font->user_داده;

  FT_Face ft_face = ft_font->ft_face;

  hb_font_مجموعه_scale (font,
		     (صحيح) (((uint64_t) ft_face->اندازه->metrics.x_scale * (uint64_t) ft_face->units_در_هر_EM + (1u<<15)) >> 16),
		     (صحيح) (((uint64_t) ft_face->اندازه->metrics.y_scale * (uint64_t) ft_face->units_در_هر_EM + (1u<<15)) >> 16));
#إذا 0 /* hb-ft works in no-hinting model */
  hb_font_مجموعه_ppem (font,
		    ft_face->اندازه->metrics.x_ppem,
		    ft_face->اندازه->metrics.y_ppem);
#پایان_اگر

#إذا defined(HAVE_FT_GET_VAR_BLEND_COORDINATES) && !defined(HB_NO_VAR)
  FT_MM_Var *mm_var = اشاره_تهی;
  إذا (!FT_Get_MM_Var (ft_face, &mm_var))
  {
    FT_Fixed *ft_coords = (FT_Fixed *) hb_calloc (mm_var->رقم_axis, حجم (FT_Fixed));
    صحيح *coords = (صحيح *) hb_calloc (mm_var->رقم_axis, حجم (صحيح));
    إذا (coords && ft_coords)
    {
      إذا (!FT_Get_Var_Blend_Coordinates (ft_face, mm_var->رقم_axis, ft_coords))
      {
	منطقي nonzero = خطأ;

	لكل (بیعلامت صحيح i = 0; i < mm_var->رقم_axis; ++i)
	 {
	  coords[i] = (ft_coords[i] + 2) >> 2;
	  nonzero = nonzero || coords[i];
	 }

	إذا (nonzero)
	  hb_font_مجموعه_var_coords_normalized (font, coords, mm_var->رقم_axis);
	وإلا
	  hb_font_مجموعه_var_coords_normalized (font, اشاره_تهی, 0);
      }
    }
    hb_free (coords);
    hb_free (ft_coords);
#اگر_تعریف_شده HAVE_FT_انجامشده_MM_VAR
    FT_انجامشده_MM_Var (ft_face->glyph->library, mm_var);
#وإلا
    hb_free (mm_var);
#پایان_اگر
  }
#پایان_اگر

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
hb_ft_hb_font_تغییریافته_2 (hb_font_t *font)
{
  إذا (font->destroy != (hb_destroy_func_t) _hb_ft_font_destroy)
    أرجع خطأ;

  hb_ft_font_t *ft_font = (hb_ft_font_t *) font->user_داده;

  أرجع hb_ft_hb_font_check_تغییریافته (font, ft_font);
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


ساكن فراغ * _hb_ft_alloc (FT_حافظه حافظه, دراز اندازه)
{ أرجع hb_malloc (اندازه); }

ساكن فراغ _hb_ft_free (FT_حافظه حافظه, فراغ *بلوک)
{ hb_free (بلوک); }

ساكن فراغ * _hb_ft_realloc (FT_حافظه حافظه, دراز cur_اندازه, دراز new_اندازه, فراغ *بلوک)
{ أرجع hb_realloc (بلوک, new_اندازه); }

ساكن FT_حافظه_Rec m =
{
  اشاره_تهی,
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
      أرجع اشاره_تهی;

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
    أرجع اشاره_تهی;
  }
} static_ft_library_mutex;

ساكن بنية hb_ft_library_lazy_loader_t : hb_lazy_loader_t<hb_remove_اشاره_گر<FT_Library>,
							     hb_ft_library_lazy_loader_t>
{
  ساكن FT_Library create ()
  {
    FT_Library l;
    إذا (FT_New_Library (&m, &l))
      أرجع اشاره_تهی;

    FT_جمع_کن_Default_Modules (l);
    FT_مجموعه_Default_Properties (l);

    hb_atexit (free_static_ft_library);

    أرجع l;
  }
  ساكن فراغ destroy (FT_Library l)
  {
    FT_انجامشده_Library (l);
  }
  ساكن FT_Library get_null ()
  {
    أرجع اشاره_تهی;
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
  hb_قفل_t lock (static_ft_library_mutex.get_unconst ());
  static_ft_library.free_instance ();
}

ساكن FT_Library
reference_ft_library ()
{
  hb_mutex_t *mutex = static_ft_library_mutex.get_unconst ();
  إذا (unlikely (!mutex))
    أرجع اشاره_تهی;
  hb_قفل_t lock (mutex);
  FT_Library l = static_ft_library.get_unconst ();
  إذا (unlikely (FT_Reference_Library (l)))
  {
    DEBUG_پیام (FT, l, "FT_Reference_Library() failed");
    أرجع اشاره_تهی;
  }
  أرجع l;
}

ساكن hb_user_داده_کلید_t ft_library_کلید = {0};

ساكن فراغ
_hb_ft_face_destroy_static (فراغ *داده)
{
  hb_قفل_t lock (static_ft_library_mutex.get_unconst ());
  FT_انجامشده_Face ((FT_Face) داده);
}

ساكن FT_Error
new_ft_face (FT_Library  ft_library,
	     ثابت حرف *file_name,
	     بیعلامت صحيح شاخص_2,
	     FT_Face     *ft_face)
{
  hb_قفل_t lock (static_ft_library_mutex.get_unconst ());
  أرجع FT_New_Face (ft_library, file_name, شاخص_2, ft_face);
}

ساكن FT_Error
new_ft_حافظه_face (FT_Library    ft_library,
		    ثابت FT_Byte *blob_داده,
		    بیعلامت صحيح   blob_اندازه,
		    بیعلامت صحيح   شاخص_2,
		    FT_Face       *ft_face)
{
  hb_قفل_t lock (static_ft_library_mutex.get_unconst ());
  أرجع FT_New_حافظه_Face (ft_library, blob_داده, blob_اندازه, شاخص_2, ft_face);
}

ساكن فراغ
finalize_ft_library (فراغ *arg)
{
  FT_Face ft_face = (FT_Face) arg;
  FT_انجامشده_Library ((FT_Library) ft_face->generic.داده);
}

ساكن فراغ
destroy_ft_library (فراغ *arg)
{
  hb_قفل_t lock (static_ft_library_mutex.get_unconst ());
  FT_انجامشده_Library ((FT_Library) arg);
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
				     بیعلامت صحيح  شاخص_2)
{
  FT_Library ft_library = reference_ft_library ();
  إذا (unlikely (!ft_library))
  {
    DEBUG_پیام (FT, ft_library, "reference_ft_library failed");
    أرجع اشاره_تهی;
  }

  FT_Face ft_face;
  إذا (unlikely (new_ft_face (ft_library,
			     file_name,
			     شاخص_2,
			     &ft_face)))
  {
    destroy_ft_library (ft_library);
    أرجع اشاره_تهی;
  }

  FT_Reference_Face (ft_face);
  hb_face_t *face = hb_ft_face_create (ft_face, _hb_ft_face_destroy_static);
  _hb_ft_face_destroy_static (ft_face);

  ft_face->generic.داده = ft_library;
  ft_face->generic.finalizer = finalize_ft_library;

  إذا (hb_face_is_immutable (face))
    أرجع اشاره_تهی;

  أرجع face;
}

ساكن hb_user_داده_کلید_t ft_blob_کلید = {0};

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
				     بیعلامت صحيح  شاخص_2)
{
  FT_Library ft_library = reference_ft_library ();
  إذا (unlikely (!ft_library))
  {
    DEBUG_پیام (FT, ft_library, "reference_ft_library failed");
    أرجع اشاره_تهی;
  }

  hb_blob_make_immutable (blob);
  بیعلامت blob_اندازه;
  ثابت حرف *blob_داده = hb_blob_get_داده (blob, &blob_اندازه);

  FT_Face ft_face;
  إذا (unlikely (new_ft_حافظه_face (ft_library,
				    (ثابت FT_Byte *) blob_داده,
				    blob_اندازه,
				    شاخص_2,
				    &ft_face)))
  {
    destroy_ft_library (ft_library);
    أرجع اشاره_تهی;
  }

  FT_Reference_Face (ft_face);
  hb_face_t *face = hb_ft_face_create (ft_face, _hb_ft_face_destroy_static);
  _hb_ft_face_destroy_static (ft_face);

  ft_face->generic.داده = ft_library;
  ft_face->generic.finalizer = finalize_ft_library;

  إذا (hb_face_is_immutable (face))
    أرجع اشاره_تهی;

  // Hook the blob to the hb_face_t, since FT_Face still needs it.
  hb_blob_reference (blob);
  إذا (!hb_face_مجموعه_user_داده (face, &ft_blob_کلید, blob, _destroy_blob, درست))
  {
    hb_blob_destroy (blob);
    hb_face_destroy (face);
    أرجع اشاره_تهی;
  }

  أرجع face;
}

ساكن فراغ
_release_blob (فراغ *arg)
{
  FT_Face ft_face = (FT_Face) arg;
  hb_blob_destroy ((hb_blob_t *) ft_face->generic.داده);
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
hb_ft_font_مجموعه_funcs (hb_font_t *font)
{
  صحيح بارگیری_flags = FT_بارگیری_DEFAULT | FT_بارگیری_NO_HINTING;
  إذا (font->destroy == (hb_destroy_func_t) _hb_ft_font_destroy && font->user_داده)
  {
    ثابت hb_ft_font_t *existing_ft_font = (ثابت hb_ft_font_t *) font->user_داده;
    بارگیری_flags = existing_ft_font->بارگیری_flags;
  }

  // In case of failure...
  hb_font_مجموعه_funcs (font,
		     hb_font_funcs_get_خالی (),
		     اشاره_تهی, اشاره_تهی);

  hb_blob_t *blob = hb_face_reference_blob (font->face);
  بیعلامت صحيح blob_درازی;
  ثابت حرف *blob_داده = hb_blob_get_داده (blob, &blob_درازی);
  إذا (unlikely (!blob_درازی))
    DEBUG_پیام (FT, font, "Font face has empty blob");

  FT_Library ft_library = reference_ft_library ();
  إذا (unlikely (!ft_library))
  {
    hb_blob_destroy (blob);
    DEBUG_پیام (FT, font, "reference_ft_library failed");
    أرجع;
  }

  FT_Face ft_face = اشاره_تهی;
  إذا (unlikely (new_ft_حافظه_face (ft_library,
				    (ثابت FT_Byte *) blob_داده,
				    blob_درازی,
				    hb_face_get_شاخص (font->face),
				    &ft_face)))
  {
    hb_blob_destroy (blob);
    destroy_ft_library (ft_library);
    DEBUG_پیام (FT, font, "FT_New_Memory_Face() failed");
    أرجع;
  }

  إذا (FT_Select_Charmap (ft_face, FT_ENCODING_MS_نماد))
    FT_Select_Charmap (ft_face, FT_ENCODING_UNICODE);

  // Hook the blob to the FT_Face
  ft_face->generic.داده = blob;
  ft_face->generic.finalizer = _release_blob;

  // And the FT_Library to the blob
  إذا (unlikely (!hb_blob_مجموعه_user_داده (blob, &ft_library_کلید, ft_library, destroy_ft_library, درست)))
  {
    DEBUG_پیام (FT, font, "hb_blob_set_user_data() failed");
    _hb_ft_face_destroy_static (ft_face);
    destroy_ft_library (ft_library);
    أرجع;
  }

  hb_ft_font_مجموعه_funcs_2 (font, ft_face, درست, درست);
  hb_ft_font_مجموعه_بارگیری_flags (font, بارگیری_flags);

  hb_ft_hb_font_تغییریافته (font, ft_face);
}

#پایان_اگر
