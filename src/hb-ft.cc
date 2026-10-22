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

#که_تعریف_شوی HAVE_FREETYPE

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
#د_که_پای


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


کارول hb_ft_advance_cache_t = hb_cache_t<16, 24, 8, خطأ>;

ساكن فراغ _hb_ft_face_destroy_static (فراغ *معلومات);

بنية hb_ft_font_t
{
  صحيح بارول_flags;
  منطقي نښه; /* Whether selected cmap is symbol cmap. */
  منطقي unref; /* Whether to destroy ft_face when done. */
  منطقي transform; /* Whether to apply FT_Face's transform. */
  منطقي static_library; /* Whether ft_face uses static_ft_library. */

  بدلیدونکی hb_mutex_t lock; /* Protects members below. */
  FT_Face ft_face;
  بدلیدونکی hb_atomic_t<بینښه> cached_serial;
  بدلیدونکی hb_ft_advance_cache_t advance_cache;
};

ساكن hb_ft_font_t *
_hb_ft_font_create (FT_Face ft_face, منطقي نښه, منطقي unref, منطقي static_library)
{
  hb_ft_font_t *ft_font = (hb_ft_font_t *) hb_calloc (1, حجم (hb_ft_font_t));
  إذا (unlikely (!ft_font)) أرجع تش_اشاره;

  ft_font->lock.init ();
  ft_font->ft_face = ft_face;
  ft_font->نښه = نښه;
  ft_font->unref = unref;
  ft_font->static_library = static_library;

  ft_font->بارول_flags = FT_بارول_DEFAULT | FT_بارول_NO_HINTING;

  ft_font->cached_serial = UINT_MAX;
  جديد (&ft_font->advance_cache) hb_ft_advance_cache_t;

  أرجع ft_font;
}

ساكن فراغ
_hb_ft_face_destroy (فراغ *معلومات)
{
  FT_بشپړ_Face ((FT_Face) معلومات);
}

ساكن فراغ
_hb_ft_font_destroy (فراغ *معلومات)
{
  hb_ft_font_t *ft_font = (hb_ft_font_t *) معلومات;

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
      hb_blob_t *blob = hb_blob_reference ((hb_blob_t *) ft_font->ft_face->generic.معلومات);
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
ساكن فراغ hb_ft_hb_font_بدل_شوی (hb_font_t *font, FT_Face ft_face)
{
  إذا (unlikely (font->destroy != (hb_destroy_func_t) _hb_ft_font_destroy))
    أرجع;

  hb_ft_font_t *ft_font = (hb_ft_font_t *) font->user_معلومات;

  عائم x_mult = 1.f, y_mult = 1.f;

  إذا (font->x_scale < 0) x_mult = -x_mult;
  إذا (font->y_scale < 0) y_mult = -y_mult;

  إذا (FT_سېټ_Char_اندازه (ft_face,
			مطلق_قیمت (font->x_scale), مطلق_قیمت (font->y_scale),
			0, 0
#إذا 0
		    font->x_ppem * 72 * 64 / font->x_scale,
		    font->y_ppem * 72 * 64 / font->y_scale
#د_که_پای
     ) && ft_face->عدد_fixed_sizes)
  {
#که_تعریف_شوی HAVE_FT_GET_TRANSFORM
    /* Bitmap font, eg. bitmap color emoji. */
    /* Pick largest size? */
    صحيح x_scale  = ft_face->شته_sizes[ft_face->عدد_fixed_sizes - 1].x_ppem;
    صحيح y_scale = ft_face->شته_sizes[ft_face->عدد_fixed_sizes - 1].y_ppem;
    FT_سېټ_Char_اندازه (ft_face,
		      x_scale, y_scale,
		      0, 0);

    /* This contains the sign that was previously in x_mult/y_mult. */
    x_mult = (عائم) font->x_scale / x_scale;
    y_mult = (عائم) font->y_scale / y_scale;
#د_که_پای
  }
  وإلا
  { /* Shrug */ }


  إذا (x_mult != 1.f || y_mult != 1.f)
  {
    FT_ماتریکس ماتریکس = { (صحيح) roundf (x_mult * (1<<16)), 0,
			  0, (صحيح) roundf (y_mult * (1<<16))};
    FT_سېټ_Transform (ft_face, &ماتریکس, تش_اشاره);
    ft_font->transform = ریښتیا;
  }
  وإلا
    FT_سېټ_Transform (ft_face, تش_اشاره, تش_اشاره);

#إذا defined(HAVE_FT_GET_VAR_BLEND_COORDINATES) && !defined(HB_NO_VAR)
  إذا (font->has_nonzero_coords)
  {
    بینښه صحيح عدد_coords;
    ثابت عائم *coords = hb_font_get_var_coords_design (font, &عدد_coords);
    FT_Fixed *ft_coords = (FT_Fixed *) hb_calloc (عدد_coords, حجم (FT_Fixed));
    إذا (ft_coords)
    {
      لكل (بینښه صحيح i = 0; i < عدد_coords; i++)
	  ft_coords[i] = coords[i] * 65536.f;
      FT_سېټ_Var_Design_Coordinates (ft_face, عدد_coords, ft_coords);
      hb_free (ft_coords);
    }
  }
  وإلا إذا (font->عدد_coords)
  {
    // Some old versions of FreeType crash if we
    // call this function on non-variable fonts.
    FT_سېټ_Var_Design_Coordinates (ft_face, 0, تش_اشاره);
  }
#د_که_پای
}

/* Check if hb_font changed, update FT_Face. */
ساكن مضمن منطقي
hb_ft_hb_font_check_بدل_شوی (hb_font_t *font,
			      ثابت hb_ft_font_t *ft_font)
{
  إذا (font->serial != ft_font->cached_serial)
  {
    hb_قلف_t lock (ft_font->lock);
    hb_ft_hb_font_بدل_شوی (font, ft_font->ft_face);
    ft_font->advance_cache.clear ();
    ft_font->cached_serial.سېټ_release (font->serial.get_acquire ());
    أرجع ریښتیا;
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
hb_ft_font_سېټ_بارول_flags (hb_font_t *font, صحيح بارول_flags)
{
  إذا (hb_object_is_immutable (font))
    أرجع;

  إذا (unlikely (font->destroy != (hb_destroy_func_t) _hb_ft_font_destroy))
    أرجع;

  hb_ft_font_t *ft_font = (hb_ft_font_t *) font->user_معلومات;

  ft_font->بارول_flags = بارول_flags;
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
hb_ft_font_get_بارول_flags (hb_font_t *font)
{
  إذا (unlikely (font->destroy != (hb_destroy_func_t) _hb_ft_font_destroy))
    أرجع 0;

  ثابت hb_ft_font_t *ft_font = (ثابت hb_ft_font_t *) font->user_معلومات;

  أرجع ft_font->بارول_flags;
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
    أرجع تش_اشاره;

  ثابت hb_ft_font_t *ft_font = (ثابت hb_ft_font_t *) font->user_معلومات;

  أرجع ft_font->ft_face;
}

#که_نه_تعریف_شوی HB_DISABLE_DEPRECATED

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

#د_که_پای

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
hb_ft_font_قلف_face (hb_font_t *font)
{
  إذا (unlikely (font->destroy != (hb_destroy_func_t) _hb_ft_font_destroy))
    أرجع تش_اشاره;

  ثابت hb_ft_font_t *ft_font = (ثابت hb_ft_font_t *) font->user_معلومات;

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

  ثابت hb_ft_font_t *ft_font = (ثابت hb_ft_font_t *) font->user_معلومات;

  ft_font->lock.unlock ();
}


ساكن hb_bool_t
hb_ft_get_nominal_glyph (hb_font_t *font,
			 فراغ *font_معلومات,
			 hb_codepoint_t unicode,
			 hb_codepoint_t *glyph,
			 فراغ *user_معلومات HB_ناکارول_شوی)
{
  ثابت hb_ft_font_t *ft_font = (ثابت hb_ft_font_t *) font_معلومات;
  hb_قلف_t lock (ft_font->lock);
  بینښه صحيح g = FT_Get_Char_شاخص (ft_font->ft_face, unicode);

  إذا (unlikely (!g))
  {
    إذا (unlikely (ft_font->نښه))
    {
      بدل ((بینښه) font->face->جدول.OS2->get_font_پاڼه ()) {
      حالة OT::OS2::font_پاڼه_t::FONT_پاڼه_NONE:
	إذا (unicode <= 0x00FFu)
	  /* For symbol-encoded OpenType fonts, we duplicate the
	   * U+F000..F0FF range at U+0000..U+00FF.  That's what
	   * Windows seems to do, and that's hinted about at:
	   * https://docs.microsoft.com/en-us/typography/opentype/spec/recom
	   * under "Non-Standard (Symbol) Fonts". */
	  g = FT_Get_Char_شاخص (ft_font->ft_face, 0xF000u + unicode);
	اكسر;
#که_نه_تعریف_شوی HB_NO_OT_SHAPER_ARABIC_FALLBACK
      حالة OT::OS2::font_پاڼه_t::FONT_پاڼه_SIMP_ARABIC:
	g = FT_Get_Char_شاخص (ft_font->ft_face, _hb_arabic_pua_simp_map (unicode));
	اكسر;
      حالة OT::OS2::font_پاڼه_t::FONT_پاڼه_TRAD_ARABIC:
	g = FT_Get_Char_شاخص (ft_font->ft_face, _hb_arabic_pua_trad_map (unicode));
	اكسر;
#د_که_پای
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
  أرجع ریښتیا;
}

ساكن بینښه صحيح
hb_ft_get_nominal_glyphs (hb_font_t *font HB_ناکارول_شوی,
			  فراغ *font_معلومات,
			  بینښه صحيح شمېر,
			  ثابت hb_codepoint_t *لومړی_unicode,
			  بینښه صحيح unicode_stride,
			  hb_codepoint_t *لومړی_glyph,
			  بینښه صحيح glyph_stride,
			  فراغ *user_معلومات HB_ناکارول_شوی)
{
  ثابت hb_ft_font_t *ft_font = (ثابت hb_ft_font_t *) font_معلومات;
  hb_قلف_t lock (ft_font->lock);
  بینښه صحيح بشپړ;
  لكل (بشپړ = 0;
       بشپړ < شمېر && (*لومړی_glyph = FT_Get_Char_شاخص (ft_font->ft_face, *لومړی_unicode));
       بشپړ++)
  {
    لومړی_unicode = &Struct_At_دننه_واټن_Unaligned<hb_codepoint_t> (لومړی_unicode, unicode_stride);
    لومړی_glyph = &Struct_At_دننه_واټن_Unaligned<hb_codepoint_t> (لومړی_glyph, glyph_stride);
  }
  /* We don't need to do ft_font->symbol dance here, since HB calls the singular
   * nominal_glyph() for what we don't handle here. */
  أرجع بشپړ;
}


ساكن hb_bool_t
hb_ft_get_variation_glyph (hb_font_t *font HB_ناکارول_شوی,
			   فراغ *font_معلومات,
			   hb_codepoint_t unicode,
			   hb_codepoint_t variation_selector,
			   hb_codepoint_t *glyph,
			   فراغ *user_معلومات HB_ناکارول_شوی)
{
  ثابت hb_ft_font_t *ft_font = (ثابت hb_ft_font_t *) font_معلومات;
  hb_قلف_t lock (ft_font->lock);
  بینښه صحيح g = FT_Face_Get_Char_Variant_شاخص (ft_font->ft_face, unicode, variation_selector);

  إذا (unlikely (!g))
    أرجع خطأ;

  *glyph = g;
  أرجع ریښتیا;
}

ساكن فراغ
hb_ft_get_glyph_h_advances (hb_font_t* font, فراغ* font_معلومات,
			    بینښه شمېر,
			    ثابت hb_codepoint_t *لومړی_glyph,
			    بینښه glyph_stride,
			    hb_position_t *لومړی_advance,
			    بینښه advance_stride,
			    فراغ *user_معلومات HB_ناکارول_شوی)
{
  ثابت hb_ft_font_t *ft_font = (ثابت hb_ft_font_t *) font_معلومات;
  hb_ft_hb_font_check_بدل_شوی (font, ft_font);

  hb_قلف_t lock (ft_font->lock);
  FT_Face ft_face = ft_font->ft_face;
  صحيح بارول_flags = ft_font->بارول_flags;
  عائم x_mult;
#که_تعریف_شوی HAVE_FT_GET_TRANSFORM
  إذا (ft_font->transform)
  {
    FT_ماتریکس ماتریکس;
    FT_Get_Transform (ft_face, &ماتریکس, تش_اشاره);
    x_mult = sqrtf ((عائم)ماتریکس.xx * ماتریکس.xx + (عائم)ماتریکس.xy * ماتریکس.xy) / 65536.f;
    x_mult *= font->x_scale < 0 ? -1 : +1;
  }
  وإلا
#د_که_پای
  {
    x_mult = font->x_scale < 0 ? -1 : +1;
  }

  لكل (بینښه صحيح i = 0; i < شمېر; i++)
  {
    hb_position_t advance;
    hb_codepoint_t glyph = *لومړی_glyph;

    بینښه صحيح cv;
    إذا (ft_font->advance_cache.get (glyph, &cv))
      advance = (hb_position_t) cv;
    وإلا
    {
      FT_Fixed v = 0;
      FT_Get_Advance (ft_face, glyph, بارول_flags, &v);
      /* Work around bug that FreeType seems to return negative advance
       * for variable-set fonts if x_scale is negative! */
      مزدوج scaled = trunc (fabs ((مزدوج) v) * (مزدوج) x_mult + (1<<9)) / (1<<10);
      advance = hb_clamp_منزل<hb_position_t> (floor (scaled));
      ft_font->advance_cache.سېټ (glyph, advance);
    }

    *لومړی_advance = advance;
    لومړی_glyph = &Struct_At_دننه_واټن_Unaligned<hb_codepoint_t> (لومړی_glyph, glyph_stride);
    لومړی_advance = &Struct_At_دننه_واټن_Unaligned<hb_position_t> (لومړی_advance, advance_stride);
  }
}

#که_نه_تعریف_شوی HB_NO_VERTICAL
ساكن hb_position_t
hb_ft_get_glyph_v_advance (hb_font_t *font,
			   فراغ *font_معلومات,
			   hb_codepoint_t glyph,
			   فراغ *user_معلومات HB_ناکارول_شوی)
{
  ثابت hb_ft_font_t *ft_font = (ثابت hb_ft_font_t *) font_معلومات;
  hb_ft_hb_font_check_بدل_شوی (font, ft_font);

  hb_قلف_t lock (ft_font->lock);
  FT_Fixed v;
  عائم y_mult;
#که_تعریف_شوی HAVE_FT_GET_TRANSFORM
  إذا (ft_font->transform)
  {
    FT_ماتریکس ماتریکس;
    FT_Get_Transform (ft_font->ft_face, &ماتریکس, تش_اشاره);
    y_mult = sqrtf ((عائم)ماتریکس.yx * ماتریکس.yx + (عائم)ماتریکس.yy * ماتریکس.yy) / 65536.f;
    y_mult *= font->y_scale < 0 ? -1 : +1;
  }
  وإلا
#د_که_پای
  {
    y_mult = font->y_scale < 0 ? -1 : +1;
  }

  إذا (unlikely (FT_Get_Advance (ft_font->ft_face, glyph, ft_font->بارول_flags | FT_بارول_VERTICAL_LAYOUT, &v)))
    أرجع 0;

  /* Note: FreeType's vertical metrics grows downward while other FreeType coordinates
   * have a Y growing upward.  Hence the extra negation. */
  مزدوج advance = floor ((-(مزدوج) v + (1<<9)) / (1<<10));

  أرجع hb_clamp_منزل<hb_position_t> ((مزدوج) y_mult * advance);
}
#د_که_پای

#که_نه_تعریف_شوی HB_NO_VERTICAL
ساكن hb_bool_t
hb_ft_get_glyph_v_origin (hb_font_t *font,
			  فراغ *font_معلومات,
			  hb_codepoint_t glyph,
			  hb_position_t *x,
			  hb_position_t *y,
			  فراغ *user_معلومات HB_ناکارول_شوی)
{
  ثابت hb_ft_font_t *ft_font = (ثابت hb_ft_font_t *) font_معلومات;
  hb_ft_hb_font_check_بدل_شوی (font, ft_font);

  hb_قلف_t lock (ft_font->lock);
  FT_Face ft_face = ft_font->ft_face;
  عائم x_mult, y_mult;
#که_تعریف_شوی HAVE_FT_GET_TRANSFORM
  إذا (ft_font->transform)
  {
    FT_ماتریکس ماتریکس;
    FT_Get_Transform (ft_face, &ماتریکس, تش_اشاره);
    x_mult = sqrtf ((عائم)ماتریکس.xx * ماتریکس.xx + (عائم)ماتریکس.xy * ماتریکس.xy) / 65536.f;
    x_mult *= font->x_scale < 0 ? -1 : +1;
    y_mult = sqrtf ((عائم)ماتریکس.yx * ماتریکس.yx + (عائم)ماتریکس.yy * ماتریکس.yy) / 65536.f;
    y_mult *= font->y_scale < 0 ? -1 : +1;
  }
  وإلا
#د_که_پای
  {
    x_mult = font->x_scale < 0 ? -1 : +1;
    y_mult = font->y_scale < 0 ? -1 : +1;
  }

  إذا (unlikely (FT_بارول_Glyph (ft_face, glyph, ft_font->بارول_flags)))
    أرجع خطأ;

  /* Note: FreeType's vertical metrics grows downward while other FreeType coordinates
   * have a Y growing upward.  Hence the extra negation. */
  *x = hb_clamp_منزل<hb_position_t> ((مزدوج) x_mult *
				   ((مزدوج) ft_face->glyph->metrics.horiBearingX -
				    ft_face->glyph->metrics.vertBearingX));
  *y = hb_clamp_منزل<hb_position_t> ((مزدوج) y_mult *
				   ((مزدوج) ft_face->glyph->metrics.horiBearingY +
				    ft_face->glyph->metrics.vertBearingY));

  أرجع ریښتیا;
}
#د_که_پای

#که_نه_تعریف_شوی HB_NO_OT_SHAPE_FALLBACK
ساكن hb_position_t
hb_ft_get_glyph_h_kerning (hb_font_t *font,
			   فراغ *font_معلومات,
			   hb_codepoint_t کیڼ_glyph,
			   hb_codepoint_t ښی_glyph,
			   فراغ *user_معلومات HB_ناکارول_شوی)
{
  ثابت hb_ft_font_t *ft_font = (ثابت hb_ft_font_t *) font_معلومات;
  hb_ft_hb_font_check_بدل_شوی (font, ft_font);

  hb_قلف_t lock (ft_font->lock);
  FT_ویکټور kerningv;

  FT_Kerning_ډېرپېښ ډېرپېښ = font->x_ppem ? FT_KERNING_DEFAULT : FT_KERNING_UNFITTED;
  إذا (FT_Get_Kerning (ft_font->ft_face, کیڼ_glyph, ښی_glyph, ډېرپېښ, &kerningv))
    أرجع 0;

  أرجع hb_clamp_منزل<hb_position_t> ((int64_t) kerningv.x);
}
#د_که_پای

ساكن منطقي
hb_ft_is_colr_glyph (hb_font_t *font,
		     فراغ *font_معلومات,
		     hb_codepoint_t gid)
{
#که_نه_تعریف_شوی HB_NO_PAINT
#إذا (FREETYPE_MAJOR*10000 + FREETYPE_MINOR*100 + FREETYPE_PATCH) >= 21300
  ثابت hb_ft_font_t *ft_font = (ثابت hb_ft_font_t *) font_معلومات;
  FT_Face ft_face = ft_font->ft_face;


  /* COLRv1 */
  FT_OpaquePaint paint = {0};
  إذا (FT_Get_Color_Glyph_Paint (ft_face, gid,
			        FT_COLOR_NO_ریښه_TRANSFORM,
			        &paint))
    أرجع ریښتیا;

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
    أرجع ریښتیا;
#د_که_پای
#د_که_پای

  أرجع خطأ;
}

ساكن hb_bool_t
hb_ft_get_glyph_extents (hb_font_t *font,
			 فراغ *font_معلومات,
			 hb_codepoint_t glyph,
			 hb_glyph_extents_t *extents,
			 فراغ *user_معلومات HB_ناکارول_شوی)
{
  // FreeType doesn't return COLR glyph extents.
  إذا (hb_ft_is_colr_glyph (font, font_معلومات, glyph))
    أرجع خطأ;

  ثابت hb_ft_font_t *ft_font = (ثابت hb_ft_font_t *) font_معلومات;
  hb_ft_hb_font_check_بدل_شوی (font, ft_font);

  hb_قلف_t lock (ft_font->lock);
  FT_Face ft_face = ft_font->ft_face;
  عائم x_mult, y_mult;

#که_تعریف_شوی HAVE_FT_GET_TRANSFORM
  إذا (ft_font->transform)
  {
    FT_ماتریکس ماتریکس;
    FT_Get_Transform (ft_face, &ماتریکس, تش_اشاره);
    x_mult = sqrtf ((عائم)ماتریکس.xx * ماتریکس.xx + (عائم)ماتریکس.xy * ماتریکس.xy) / 65536.f;
    x_mult *= font->x_scale < 0 ? -1 : +1;
    y_mult = sqrtf ((عائم)ماتریکس.yx * ماتریکس.yx + (عائم)ماتریکس.yy * ماتریکس.yy) / 65536.f;
    y_mult *= font->y_scale < 0 ? -1 : +1;
  }
  وإلا
#د_که_پای
  {
    x_mult = font->x_scale < 0 ? -1 : +1;
    y_mult = font->y_scale < 0 ? -1 : +1;
  }

  إذا (unlikely (FT_بارول_Glyph (ft_face, glyph, ft_font->بارول_flags)))
    أرجع خطأ;

  /* Copied from hb_font_t::scale_glyph_extents. */

  عائم x1 = x_mult * ft_face->glyph->metrics.horiBearingX;
  عائم y1 = y_mult * ft_face->glyph->metrics.horiBearingY;
  عائم x2 = x1 + x_mult *  ft_face->glyph->metrics.پلنوالی;
  عائم y2 = y1 + y_mult * -ft_face->glyph->metrics.لوړوالی;

  مزدوج rx1 = (مزدوج) roundf (x1);
  مزدوج ry1 = (مزدوج) roundf (y1);
  مزدوج rx2 = (مزدوج) roundf (x2);
  مزدوج ry2 = (مزدوج) roundf (y2);

  extents->x_bearing = hb_clamp_منزل<hb_position_t> (rx1);
  extents->y_bearing = hb_clamp_منزل<hb_position_t> (ry1);
  extents->پلنوالی = hb_clamp_منزل<hb_position_t> (rx2 - rx1);
  extents->لوړوالی = hb_clamp_منزل<hb_position_t> (ry2 - ry1);

  أرجع ریښتیا;
}

ساكن hb_bool_t
hb_ft_get_glyph_contour_ټکی (hb_font_t *font HB_ناکارول_شوی,
			       فراغ *font_معلومات,
			       hb_codepoint_t glyph,
			       بینښه صحيح ټکی_شاخص,
			       hb_position_t *x,
			       hb_position_t *y,
			       فراغ *user_معلومات HB_ناکارول_شوی)
{
  ثابت hb_ft_font_t *ft_font = (ثابت hb_ft_font_t *) font_معلومات;
  hb_ft_hb_font_check_بدل_شوی (font, ft_font);

  hb_قلف_t lock (ft_font->lock);
  FT_Face ft_face = ft_font->ft_face;

  إذا (unlikely (FT_بارول_Glyph (ft_face, glyph, ft_font->بارول_flags)))
      أرجع خطأ;

  إذا (unlikely (ft_face->glyph->format != FT_GLYPH_FORMAT_OUTLINE))
      أرجع خطأ;

  إذا (unlikely (ټکی_شاخص >= (بینښه صحيح) ft_face->glyph->outline.n_points))
      أرجع خطأ;

  *x = ft_face->glyph->outline.points[ټکی_شاخص].x;
  *y = ft_face->glyph->outline.points[ټکی_شاخص].y;

  أرجع ریښتیا;
}

ساكن hb_bool_t
hb_ft_get_glyph_name (hb_font_t *font HB_ناکارول_شوی,
		      فراغ *font_معلومات,
		      hb_codepoint_t glyph,
		      حرف *name, بینښه صحيح اندازه,
		      فراغ *user_معلومات HB_ناکارول_شوی)
{
  ثابت hb_ft_font_t *ft_font = (ثابت hb_ft_font_t *) font_معلومات;
  hb_قلف_t lock (ft_font->lock);
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
hb_ft_get_glyph_from_name (hb_font_t *font HB_ناکارول_شوی,
			   فراغ *font_معلومات,
			   ثابت حرف *name, صحيح len, /* -1 means nul-terminated */
			   hb_codepoint_t *glyph,
			   فراغ *user_معلومات HB_ناکارول_شوی)
{
  ثابت hb_ft_font_t *ft_font = (ثابت hb_ft_font_t *) font_معلومات;
  hb_قلف_t lock (ft_font->lock);
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
	len < 0 ? !strcmp (buf, name) : !مزي_پرتله (buf, name, len))
      أرجع ریښتیا;
  }

  أرجع *glyph != 0;
}

ساكن hb_bool_t
hb_ft_get_font_h_extents (hb_font_t *font HB_ناکارول_شوی,
			  فراغ *font_معلومات,
			  hb_font_extents_t *metrics,
			  فراغ *user_معلومات HB_ناکارول_شوی)
{
  ثابت hb_ft_font_t *ft_font = (ثابت hb_ft_font_t *) font_معلومات;
  hb_ft_hb_font_check_بدل_شوی (font, ft_font);

  hb_قلف_t lock (ft_font->lock);
  FT_Face ft_face = ft_font->ft_face;
  عائم y_mult;
#که_تعریف_شوی HAVE_FT_GET_TRANSFORM
  إذا (ft_font->transform)
  {
    FT_ماتریکس ماتریکس;
    FT_Get_Transform (ft_face, &ماتریکس, تش_اشاره);
    y_mult = sqrtf ((عائم)ماتریکس.yx * ماتریکس.yx + (عائم)ماتریکس.yy * ماتریکس.yy) / 65536.f;
    y_mult *= font->y_scale < 0 ? -1 : +1;
  }
  وإلا
#د_که_پای
  {
    y_mult = font->y_scale < 0 ? -1 : +1;
  }

  إذا (ft_face->units_فی_EM != 0)
  {
    مزدوج ascender = FT_MulFix(ft_face->ascender, ft_face->اندازه->metrics.y_scale);
    مزدوج descender = FT_MulFix(ft_face->descender, ft_face->اندازه->metrics.y_scale);
    مزدوج لوړوالی = FT_MulFix( ft_face->لوړوالی, ft_face->اندازه->metrics.y_scale );
    metrics->ascender = hb_clamp_منزل<hb_position_t> ((مزدوج) y_mult * ascender);
    metrics->descender = hb_clamp_منزل<hb_position_t> ((مزدوج) y_mult * descender);
    metrics->کرښه_gap = hb_clamp_منزل<hb_position_t> ((مزدوج) y_mult * (لوړوالی - (ascender - descender)));
  }
  وإلا
  {
    /* Bitmap-only font, eg. color bitmap font. */
    مزدوج ascender = ft_face->اندازه->metrics.ascender;
    مزدوج descender = ft_face->اندازه->metrics.descender;
    مزدوج لوړوالی = ft_face->اندازه->metrics.لوړوالی;
    metrics->ascender = hb_clamp_منزل<hb_position_t> ((مزدوج) y_mult * ascender);
    metrics->descender = hb_clamp_منزل<hb_position_t> ((مزدوج) y_mult * descender);
    metrics->کرښه_gap = hb_clamp_منزل<hb_position_t> ((مزدوج) y_mult * (لوړوالی - (ascender - descender)));
  }

  أرجع ریښتیا;
}

#که_نه_تعریف_شوی HB_NO_DRAW

ساكن صحيح
hb_ft_move_منزل (ثابت FT_ویکټور *to,
		فراغ *arg)
{
  hb_draw_session_t *drawing = (hb_draw_session_t *) arg;
  drawing->move_منزل (to->x, to->y);
  أرجع FT_Err_Ok;
}

ساكن صحيح
hb_ft_کرښه_منزل (ثابت FT_ویکټور *to,
		فراغ *arg)
{
  hb_draw_session_t *drawing = (hb_draw_session_t *) arg;
  drawing->کرښه_منزل (to->x, to->y);
  أرجع FT_Err_Ok;
}

ساكن صحيح
hb_ft_conic_منزل (ثابت FT_ویکټور *control,
		 ثابت FT_ویکټور *to,
		 فراغ *arg)
{
  hb_draw_session_t *drawing = (hb_draw_session_t *) arg;
  drawing->quadratic_منزل (control->x, control->y,
			 to->x, to->y);
  أرجع FT_Err_Ok;
}

ساكن صحيح
hb_ft_cubic_منزل (ثابت FT_ویکټور *control1,
		 ثابت FT_ویکټور *control2,
		 ثابت FT_ویکټور *to,
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
			  فراغ *font_معلومات,
			  hb_codepoint_t glyph,
			  hb_draw_funcs_t *draw_funcs, فراغ *draw_معلومات,
			  فراغ *user_معلومات HB_ناکارول_شوی)
{
  ثابت hb_ft_font_t *ft_font = (ثابت hb_ft_font_t *) font_معلومات;
  hb_ft_hb_font_check_بدل_شوی (font, ft_font);

  hb_قلف_t lock (ft_font->lock);
  FT_Face ft_face = ft_font->ft_face;

  إذا (unlikely (FT_بارول_Glyph (ft_face, glyph,
			       FT_بارول_NO_BITMAP | ft_font->بارول_flags)))
    أرجع خطأ;

  إذا (ft_face->glyph->format != FT_GLYPH_FORMAT_OUTLINE)
    أرجع خطأ;

  ثابت FT_Outline_Funcs outline_funcs = {
    hb_ft_move_منزل,
    hb_ft_کرښه_منزل,
    hb_ft_conic_منزل,
    hb_ft_cubic_منزل,
    0, /*ښويدنه*/
    0, /* delta */
  };

  hb_draw_session_t draw_session {draw_funcs, draw_معلومات};

  FT_Outline_Decompose (&ft_face->glyph->outline,
			&outline_funcs,
			&draw_session);

  أرجع ریښتیا;
}
#د_که_پای

#که_نه_تعریف_شوی HB_NO_PAINT
#إذا (FREETYPE_MAJOR*10000 + FREETYPE_MINOR*100 + FREETYPE_PATCH) >= 21300

#ضمن "hb-ft-colr.hh"

ساكن hb_bool_t
hb_ft_paint_glyph_or_fail (hb_font_t *font,
			   فراغ *font_معلومات,
			   hb_codepoint_t gid,
			   hb_paint_funcs_t *paint_funcs, فراغ *paint_معلومات,
			   بینښه صحيح palette_شاخص,
			   hb_color_t foreground,
			   فراغ *user_معلومات)
{
  ثابت hb_ft_font_t *ft_font = (ثابت hb_ft_font_t *) font_معلومات;
  hb_ft_hb_font_check_بدل_شوی (font, ft_font);

  hb_قلف_t lock (ft_font->lock);
  FT_Face ft_face = ft_font->ft_face;

  FT_Long بارول_flags = ft_font->بارول_flags | FT_بارول_COLOR;
#إذا (FREETYPE_MAJOR*10000 + FREETYPE_MINOR*100 + FREETYPE_PATCH) >= 21301
  بارول_flags |= FT_بارول_NO_SVG;
#د_که_پای

  /* We release the lock before calling into glyph callbacks, such that
   * eg. draw API can call back into the face.*/

  إذا (unlikely (FT_بارول_Glyph (ft_face, gid, بارول_flags)))
    أرجع خطأ;

  إذا (ft_face->glyph->format == FT_GLYPH_FORMAT_OUTLINE)
  {
    إذا (hb_ft_paint_glyph_colr (font, font_معلومات, gid,
				paint_funcs, paint_معلومات,
				palette_شاخص, foreground,
				user_معلومات))
      أرجع ریښتیا;

    // Outline glyph
    أرجع خطأ;
  }

  خپلکاری *glyph = ft_face->glyph;
  إذا (glyph->format == FT_GLYPH_FORMAT_BITMAP)
  {
    منطقي ret = خطأ;
    خپلکاری &bitmap = glyph->bitmap;
    إذا (bitmap.pixel_ډېرپېښ == FT_PIXEL_ډېرپېښ_BGRA)
    {
      إذا (bitmap.pitch != (نښهدار) bitmap.پلنوالی * 4)
        أرجع ret;

      ft_font->lock.unlock ();

      hb_blob_t *blob = hb_blob_create ((ثابت حرف *) bitmap.بفر,
					bitmap.pitch * bitmap.لیکې,
					HB_حافظه_ډېرپېښ_DUPLICATE,
					تش_اشاره, تش_اشاره);

      hb_glyph_extents_t extents;
      إذا (!font->get_glyph_extents (gid, &extents, خطأ))
	اذهب بهر;

      إذا (paint_funcs->image (paint_معلومات,
			      blob,
			      bitmap.پلنوالی,
			      bitmap.لیکې,
			      HB_PAINT_IMAGE_FORMAT_BGRA,
			      0.f,
			      &extents))
        ret = ریښتیا;

    بهر:
      hb_blob_destroy (blob);
      ft_font->lock.lock ();
    }

    أرجع ret;
  }
  أرجع خطأ;
}
#د_که_پای
#د_که_پای


ساكن مضمن فراغ free_static_ft_funcs ();

ساكن بنية hb_ft_font_funcs_lazy_loader_t : hb_font_funcs_lazy_loader_t<hb_ft_font_funcs_lazy_loader_t>
{
  ساكن hb_font_funcs_t *create ()
  {
    hb_font_funcs_t *funcs = hb_font_funcs_create ();

    hb_font_funcs_سېټ_nominal_glyph_func (funcs, hb_ft_get_nominal_glyph, تش_اشاره, تش_اشاره);
    hb_font_funcs_سېټ_nominal_glyphs_func (funcs, hb_ft_get_nominal_glyphs, تش_اشاره, تش_اشاره);
    hb_font_funcs_سېټ_variation_glyph_func (funcs, hb_ft_get_variation_glyph, تش_اشاره, تش_اشاره);

    hb_font_funcs_سېټ_font_h_extents_func (funcs, hb_ft_get_font_h_extents, تش_اشاره, تش_اشاره);
    hb_font_funcs_سېټ_glyph_h_advances_func (funcs, hb_ft_get_glyph_h_advances, تش_اشاره, تش_اشاره);

#که_نه_تعریف_شوی HB_NO_VERTICAL
    hb_font_funcs_سېټ_glyph_v_advance_func (funcs, hb_ft_get_glyph_v_advance, تش_اشاره, تش_اشاره);
    hb_font_funcs_سېټ_glyph_v_origin_func (funcs, hb_ft_get_glyph_v_origin, تش_اشاره, تش_اشاره);
#د_که_پای

#که_نه_تعریف_شوی HB_NO_OT_SHAPE_FALLBACK
    hb_font_funcs_سېټ_glyph_h_kerning_func (funcs, hb_ft_get_glyph_h_kerning, تش_اشاره, تش_اشاره);
#د_که_پای
    hb_font_funcs_سېټ_glyph_extents_func (funcs, hb_ft_get_glyph_extents, تش_اشاره, تش_اشاره);
    hb_font_funcs_سېټ_glyph_contour_ټکی_func (funcs, hb_ft_get_glyph_contour_ټکی, تش_اشاره, تش_اشاره);
    hb_font_funcs_سېټ_glyph_name_func (funcs, hb_ft_get_glyph_name, تش_اشاره, تش_اشاره);
    hb_font_funcs_سېټ_glyph_from_name_func (funcs, hb_ft_get_glyph_from_name, تش_اشاره, تش_اشاره);

#که_نه_تعریف_شوی HB_NO_DRAW
    hb_font_funcs_سېټ_draw_glyph_or_fail_func (funcs, hb_ft_draw_glyph_or_fail, تش_اشاره, تش_اشاره);
#د_که_پای

#که_نه_تعریف_شوی HB_NO_PAINT
#إذا (FREETYPE_MAJOR*10000 + FREETYPE_MINOR*100 + FREETYPE_PATCH) >= 21300
    hb_font_funcs_سېټ_paint_glyph_or_fail_func (funcs, hb_ft_paint_glyph_or_fail, تش_اشاره, تش_اشاره);
#د_که_پای
#د_که_پای

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
hb_ft_font_سېټ_funcs_2 (hb_font_t *font, FT_Face ft_face, منطقي unref,
		       منطقي static_library = خطأ)
{
  منطقي نښه = ft_face->charmap && ft_face->charmap->encoding == FT_ENCODING_MS_نښه;

  hb_ft_font_t *ft_font = _hb_ft_font_create (ft_face, نښه, unref, static_library);
  إذا (unlikely (!ft_font)) أرجع;

  hb_font_سېټ_funcs (font,
		     _hb_ft_get_font_funcs (),
		     ft_font,
		     _hb_ft_font_destroy);
}


ساكن hb_blob_t *
hb_ft_reference_جدول (hb_face_t *face HB_ناکارول_شوی, hb_نښان_t نښان_2, فراغ *user_معلومات)
{
  FT_Face ft_face = (FT_Face) user_معلومات;
  FT_Byte *بفر;
  FT_ULong  اوږدتیا = 0;
  FT_Error error;

  /* In new FreeType, a tag value of 1 loads the SFNT table directory. Reject it. */
  إذا (نښان_2 == 1)
    أرجع تش_اشاره;

  /* Note: FreeType like HarfBuzz uses the NONE tag for fetching the entire blob */

  error = FT_بارول_Sfnt_جدول (ft_face, نښان_2, 0, تش_اشاره, &اوږدتیا);
  إذا (error)
    أرجع تش_اشاره;

  بفر = (FT_Byte *) hb_malloc (اوږدتیا);
  إذا (!بفر)
    أرجع تش_اشاره;
  خپلکاری بفر_guard = hb_make_scope_guard ([&]() { hb_free (بفر); });

  error = FT_بارول_Sfnt_جدول (ft_face, نښان_2, 0, بفر, &اوږدتیا);
  إذا (error)
    أرجع تش_اشاره;

  بفر_guard.release ();
  أرجع hb_blob_create ((ثابت حرف *) بفر, اوږدتیا,
			 HB_حافظه_ډېرپېښ_WRITABLE,
			 بفر, hb_free);
}

ساكن بینښه
hb_ft_get_جدول_نښانونه (ثابت hb_face_t *face HB_ناکارول_شوی,
		       بینښه صحيح پیل_دننه_واټن,
		       بینښه صحيح *جدول_شمېر_2,
		       hb_نښان_t *جدول_نښانونه,
		       فراغ *user_معلومات)
{
  FT_Face ft_face = (FT_Face) user_معلومات;

  FT_ULong population = 0;
  FT_Sfnt_جدول_Info (ft_face,
		      0, // table_index; ignored
		      تش_اشاره,
                      &population);

  إذا (!جدول_شمېر_2)
    أرجع population;

  إذا (unlikely (پیل_دننه_واټن >= population))
  {
    *جدول_شمېر_2 = 0;
    أرجع population;
  }

  بینښه پای_ټکی_دننه_واټن = پیل_دننه_واټن + *جدول_شمېر_2;
  إذا (unlikely (پای_ټکی_دننه_واټن < پیل_دننه_واټن)) /* Overflow. */
  {
    *جدول_شمېر_2 = 0;
    أرجع population;
  }
  پای_ټکی_دننه_واټن = hb_min (پای_ټکی_دننه_واټن, (بینښه) population);

  *جدول_شمېر_2 = پای_ټکی_دننه_واټن - پیل_دننه_واټن;
  إذا (جدول_نښانونه)
    لكل (بینښه i = پیل_دننه_واټن; i < پای_ټکی_دننه_واټن; i++)
    {
      FT_ULong نښان_2 = 0, اوږدتیا;
      FT_Sfnt_جدول_Info (ft_face, i, &نښان_2, &اوږدتیا);
      جدول_نښانونه[i - پیل_دننه_واټن] = نښان_2;
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

  إذا (!ft_face->بهیر->اقرأ) {
    hb_blob_t *blob;

    blob = hb_blob_create ((ثابت حرف *) ft_face->بهیر->بنسټ,
			   (بینښه صحيح) ft_face->بهیر->اندازه,
			   HB_حافظه_ډېرپېښ_READONLY,
			   ft_face, destroy);
    face = hb_face_create (blob, ft_face->face_شاخص);
    hb_blob_destroy (blob);
  } وإلا {
    face = hb_face_create_for_tables (hb_ft_reference_جدول, ft_face, destroy);
    hb_face_سېټ_get_جدول_نښانونه_func (face, hb_ft_get_جدول_نښانونه, ft_face, تش_اشاره);
  }

  hb_face_سېټ_شاخص (face, ft_face->face_شاخص);
  hb_face_سېټ_upem (face, ft_face->units_فی_EM);

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
  hb_face_destroy ((hb_face_t *) ft_face->generic.معلومات);
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
  إذا (unlikely (!ft_face->generic.معلومات || ft_face->generic.finalizer != hb_ft_face_finalize))
  {
    إذا (ft_face->generic.finalizer)
      ft_face->generic.finalizer (ft_face);

    ft_face->generic.معلومات = hb_ft_face_create (ft_face, تش_اشاره);
    ft_face->generic.finalizer = hb_ft_face_finalize;
  }

  أرجع hb_face_reference ((hb_face_t *) ft_face->generic.معلومات);
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
  hb_ft_font_سېټ_funcs_2 (font, ft_face, خطأ);
  hb_ft_font_بدل_شوی (font);
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
hb_ft_font_بدل_شوی (hb_font_t *font)
{
  إذا (font->destroy != (hb_destroy_func_t) _hb_ft_font_destroy)
    أرجع;

  hb_ft_font_t *ft_font = (hb_ft_font_t *) font->user_معلومات;

  FT_Face ft_face = ft_font->ft_face;

  hb_font_سېټ_scale (font,
		     (صحيح) (((uint64_t) ft_face->اندازه->metrics.x_scale * (uint64_t) ft_face->units_فی_EM + (1u<<15)) >> 16),
		     (صحيح) (((uint64_t) ft_face->اندازه->metrics.y_scale * (uint64_t) ft_face->units_فی_EM + (1u<<15)) >> 16));
#إذا 0 /* hb-ft works in no-hinting model */
  hb_font_سېټ_ppem (font,
		    ft_face->اندازه->metrics.x_ppem,
		    ft_face->اندازه->metrics.y_ppem);
#د_که_پای

#إذا defined(HAVE_FT_GET_VAR_BLEND_COORDINATES) && !defined(HB_NO_VAR)
  FT_MM_Var *mm_var = تش_اشاره;
  إذا (!FT_Get_MM_Var (ft_face, &mm_var))
  {
    FT_Fixed *ft_coords = (FT_Fixed *) hb_calloc (mm_var->عدد_axis, حجم (FT_Fixed));
    صحيح *coords = (صحيح *) hb_calloc (mm_var->عدد_axis, حجم (صحيح));
    إذا (coords && ft_coords)
    {
      إذا (!FT_Get_Var_Blend_Coordinates (ft_face, mm_var->عدد_axis, ft_coords))
      {
	منطقي nonzero = خطأ;

	لكل (بینښه صحيح i = 0; i < mm_var->عدد_axis; ++i)
	 {
	  coords[i] = (ft_coords[i] + 2) >> 2;
	  nonzero = nonzero || coords[i];
	 }

	إذا (nonzero)
	  hb_font_سېټ_var_coords_normalized (font, coords, mm_var->عدد_axis);
	وإلا
	  hb_font_سېټ_var_coords_normalized (font, تش_اشاره, 0);
      }
    }
    hb_free (coords);
    hb_free (ft_coords);
#که_تعریف_شوی HAVE_FT_بشپړ_MM_VAR
    FT_بشپړ_MM_Var (ft_face->glyph->library, mm_var);
#وإلا
    hb_free (mm_var);
#د_که_پای
  }
#د_که_پای

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
hb_ft_hb_font_بدل_شوی_2 (hb_font_t *font)
{
  إذا (font->destroy != (hb_destroy_func_t) _hb_ft_font_destroy)
    أرجع خطأ;

  hb_ft_font_t *ft_font = (hb_ft_font_t *) font->user_معلومات;

  أرجع hb_ft_hb_font_check_بدل_شوی (font, ft_font);
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


ساكن فراغ * _hb_ft_alloc (FT_حافظه حافظه, اوږد اندازه)
{ أرجع hb_malloc (اندازه); }

ساكن فراغ _hb_ft_free (FT_حافظه حافظه, فراغ *بلاک)
{ hb_free (بلاک); }

ساكن فراغ * _hb_ft_realloc (FT_حافظه حافظه, اوږد cur_اندازه, اوږد new_اندازه, فراغ *بلاک)
{ أرجع hb_realloc (بلاک, new_اندازه); }

ساكن FT_حافظه_Rec m =
{
  تش_اشاره,
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
      أرجع تش_اشاره;

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
    أرجع تش_اشاره;
  }
} static_ft_library_mutex;

ساكن بنية hb_ft_library_lazy_loader_t : hb_lazy_loader_t<hb_remove_اشاره_ګر<FT_Library>,
							     hb_ft_library_lazy_loader_t>
{
  ساكن FT_Library create ()
  {
    FT_Library l;
    إذا (FT_New_Library (&m, &l))
      أرجع تش_اشاره;

    FT_جمع_کړه_Default_Modules (l);
    FT_سېټ_Default_Properties (l);

    hb_atexit (free_static_ft_library);

    أرجع l;
  }
  ساكن فراغ destroy (FT_Library l)
  {
    FT_بشپړ_Library (l);
  }
  ساكن FT_Library get_null ()
  {
    أرجع تش_اشاره;
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
  hb_قلف_t lock (static_ft_library_mutex.get_unconst ());
  static_ft_library.free_instance ();
}

ساكن FT_Library
reference_ft_library ()
{
  hb_mutex_t *mutex = static_ft_library_mutex.get_unconst ();
  إذا (unlikely (!mutex))
    أرجع تش_اشاره;
  hb_قلف_t lock (mutex);
  FT_Library l = static_ft_library.get_unconst ();
  إذا (unlikely (FT_Reference_Library (l)))
  {
    DEBUG_پیغام (FT, l, "FT_Reference_Library() failed");
    أرجع تش_اشاره;
  }
  أرجع l;
}

ساكن hb_user_معلومات_کلی_t ft_library_کلی = {0};

ساكن فراغ
_hb_ft_face_destroy_static (فراغ *معلومات)
{
  hb_قلف_t lock (static_ft_library_mutex.get_unconst ());
  FT_بشپړ_Face ((FT_Face) معلومات);
}

ساكن FT_Error
new_ft_face (FT_Library  ft_library,
	     ثابت حرف *file_name,
	     بینښه صحيح شاخص_2,
	     FT_Face     *ft_face)
{
  hb_قلف_t lock (static_ft_library_mutex.get_unconst ());
  أرجع FT_New_Face (ft_library, file_name, شاخص_2, ft_face);
}

ساكن FT_Error
new_ft_حافظه_face (FT_Library    ft_library,
		    ثابت FT_Byte *blob_معلومات,
		    بینښه صحيح   blob_اندازه,
		    بینښه صحيح   شاخص_2,
		    FT_Face       *ft_face)
{
  hb_قلف_t lock (static_ft_library_mutex.get_unconst ());
  أرجع FT_New_حافظه_Face (ft_library, blob_معلومات, blob_اندازه, شاخص_2, ft_face);
}

ساكن فراغ
finalize_ft_library (فراغ *arg)
{
  FT_Face ft_face = (FT_Face) arg;
  FT_بشپړ_Library ((FT_Library) ft_face->generic.معلومات);
}

ساكن فراغ
destroy_ft_library (فراغ *arg)
{
  hb_قلف_t lock (static_ft_library_mutex.get_unconst ());
  FT_بشپړ_Library ((FT_Library) arg);
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
				     بینښه صحيح  شاخص_2)
{
  FT_Library ft_library = reference_ft_library ();
  إذا (unlikely (!ft_library))
  {
    DEBUG_پیغام (FT, ft_library, "reference_ft_library failed");
    أرجع تش_اشاره;
  }

  FT_Face ft_face;
  إذا (unlikely (new_ft_face (ft_library,
			     file_name,
			     شاخص_2,
			     &ft_face)))
  {
    destroy_ft_library (ft_library);
    أرجع تش_اشاره;
  }

  FT_Reference_Face (ft_face);
  hb_face_t *face = hb_ft_face_create (ft_face, _hb_ft_face_destroy_static);
  _hb_ft_face_destroy_static (ft_face);

  ft_face->generic.معلومات = ft_library;
  ft_face->generic.finalizer = finalize_ft_library;

  إذا (hb_face_is_immutable (face))
    أرجع تش_اشاره;

  أرجع face;
}

ساكن hb_user_معلومات_کلی_t ft_blob_کلی = {0};

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
				     بینښه صحيح  شاخص_2)
{
  FT_Library ft_library = reference_ft_library ();
  إذا (unlikely (!ft_library))
  {
    DEBUG_پیغام (FT, ft_library, "reference_ft_library failed");
    أرجع تش_اشاره;
  }

  hb_blob_make_immutable (blob);
  بینښه blob_اندازه;
  ثابت حرف *blob_معلومات = hb_blob_get_معلومات (blob, &blob_اندازه);

  FT_Face ft_face;
  إذا (unlikely (new_ft_حافظه_face (ft_library,
				    (ثابت FT_Byte *) blob_معلومات,
				    blob_اندازه,
				    شاخص_2,
				    &ft_face)))
  {
    destroy_ft_library (ft_library);
    أرجع تش_اشاره;
  }

  FT_Reference_Face (ft_face);
  hb_face_t *face = hb_ft_face_create (ft_face, _hb_ft_face_destroy_static);
  _hb_ft_face_destroy_static (ft_face);

  ft_face->generic.معلومات = ft_library;
  ft_face->generic.finalizer = finalize_ft_library;

  إذا (hb_face_is_immutable (face))
    أرجع تش_اشاره;

  // Hook the blob to the hb_face_t, since FT_Face still needs it.
  hb_blob_reference (blob);
  إذا (!hb_face_سېټ_user_معلومات (face, &ft_blob_کلی, blob, _destroy_blob, ریښتیا))
  {
    hb_blob_destroy (blob);
    hb_face_destroy (face);
    أرجع تش_اشاره;
  }

  أرجع face;
}

ساكن فراغ
_release_blob (فراغ *arg)
{
  FT_Face ft_face = (FT_Face) arg;
  hb_blob_destroy ((hb_blob_t *) ft_face->generic.معلومات);
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
hb_ft_font_سېټ_funcs (hb_font_t *font)
{
  صحيح بارول_flags = FT_بارول_DEFAULT | FT_بارول_NO_HINTING;
  إذا (font->destroy == (hb_destroy_func_t) _hb_ft_font_destroy && font->user_معلومات)
  {
    ثابت hb_ft_font_t *existing_ft_font = (ثابت hb_ft_font_t *) font->user_معلومات;
    بارول_flags = existing_ft_font->بارول_flags;
  }

  // In case of failure...
  hb_font_سېټ_funcs (font,
		     hb_font_funcs_get_تشه (),
		     تش_اشاره, تش_اشاره);

  hb_blob_t *blob = hb_face_reference_blob (font->face);
  بینښه صحيح blob_اوږدتیا;
  ثابت حرف *blob_معلومات = hb_blob_get_معلومات (blob, &blob_اوږدتیا);
  إذا (unlikely (!blob_اوږدتیا))
    DEBUG_پیغام (FT, font, "Font face has empty blob");

  FT_Library ft_library = reference_ft_library ();
  إذا (unlikely (!ft_library))
  {
    hb_blob_destroy (blob);
    DEBUG_پیغام (FT, font, "reference_ft_library failed");
    أرجع;
  }

  FT_Face ft_face = تش_اشاره;
  إذا (unlikely (new_ft_حافظه_face (ft_library,
				    (ثابت FT_Byte *) blob_معلومات,
				    blob_اوږدتیا,
				    hb_face_get_شاخص (font->face),
				    &ft_face)))
  {
    hb_blob_destroy (blob);
    destroy_ft_library (ft_library);
    DEBUG_پیغام (FT, font, "FT_New_Memory_Face() failed");
    أرجع;
  }

  إذا (FT_Select_Charmap (ft_face, FT_ENCODING_MS_نښه))
    FT_Select_Charmap (ft_face, FT_ENCODING_UNICODE);

  // Hook the blob to the FT_Face
  ft_face->generic.معلومات = blob;
  ft_face->generic.finalizer = _release_blob;

  // And the FT_Library to the blob
  إذا (unlikely (!hb_blob_سېټ_user_معلومات (blob, &ft_library_کلی, ft_library, destroy_ft_library, ریښتیا)))
  {
    DEBUG_پیغام (FT, font, "hb_blob_set_user_data() failed");
    _hb_ft_face_destroy_static (ft_face);
    destroy_ft_library (ft_library);
    أرجع;
  }

  hb_ft_font_سېټ_funcs_2 (font, ft_face, ریښتیا, ریښتیا);
  hb_ft_font_سېټ_بارول_flags (font, بارول_flags);

  hb_ft_hb_font_بدل_شوی (font, ft_face);
}

#د_که_پای
