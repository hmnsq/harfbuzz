/*
 * Copyright © 2011,2014  Google, Inc.
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
 * Google Author(s): Behdad Esfahbod, Roozbeh Pournader
 */

#ضمن "hb.hh"

#اگر_تعریف_نشده HB_NO_OT_FONT

#ضمن "hb-ot.h"

#ضمن "hb-cache.hh"
#ضمن "hb-font.hh"
#ضمن "hb-machinery.hh"
#ضمن "hb-ot-face.hh"

#ضمن "hb-ot-cmap-table.hh"
#ضمن "hb-ot-glyf-table.hh"
#ضمن "hb-ot-var-gvar-table.hh"
#ضمن "hb-ot-cff2-table.hh"
#ضمن "hb-ot-cff1-table.hh"
#ضمن "hb-ot-hmtx-table.hh"
#ضمن "hb-ot-post-table.hh"
#ضمن "hb-ot-stat-table.hh"
#ضمن "hb-ot-var-varc-table.hh"
#ضمن "hb-ot-vorg-table.hh"
#ضمن "OT/Color/CBDT/CBDT.hh"
#ضمن "OT/Color/COLR/COLR.hh"
#ضمن "OT/Color/sbix/sbix.hh"
#ضمن "OT/Color/svg/svg.hh"


/**
 * SECTION:hb-ot-font
 * @title: hb-ot-font
 * @short_description: OpenType font implementation
 * @include: hb-ot.h
 *
 * Functions for using OpenType fonts with hb_shape().  Note that fonts returned
 * by hb_font_create() default to using these functions, so most clients would
 * never need to call these functions directly.
 **/

استفاده hb_ot_font_advance_cache_t = hb_cache_t<24, 16>;
static_assert (حجم (hb_ot_font_advance_cache_t) == 1024, "");

استفاده hb_ot_font_origin_cache_t = hb_cache_t<20, 20>;
static_assert (حجم (hb_ot_font_origin_cache_t) == 1024, "");

بنية hb_ot_font_t
{
  ثابت hb_ot_face_t *ot_face;

  تغییرپذیر hb_atomic_t<صحيح> cached_serial;
  تغییرپذیر hb_atomic_t<صحيح> cached_coords_serial;

  بنية direction_cache_t
  {
    تغییرپذیر hb_atomic_t<hb_ot_font_advance_cache_t *> advance_cache;
    تغییرپذیر hb_atomic_t<OT::hb_scalar_cache_t *> varStore_cache;

    ~direction_cache_t ()
    {
      clear ();
    }

    hb_ot_font_advance_cache_t *acquire_advance_cache () ثابت
    {
    retry:
      خودکار *cache = advance_cache.get_acquire ();
      إذا (!cache)
      {
        cache = (hb_ot_font_advance_cache_t *) hb_malloc (حجم (hb_ot_font_advance_cache_t));
	إذا (!cache)
	  أرجع اشاره_تهی;
	جديد (cache) hb_ot_font_advance_cache_t;
	أرجع cache;
      }
      إذا (advance_cache.cmpexch (cache, اشاره_تهی))
        أرجع cache;
      وإلا
        اذهب retry;
    }
    فراغ release_advance_cache (hb_ot_font_advance_cache_t *cache) ثابت
    {
      إذا (!cache)
        أرجع;
      إذا (!advance_cache.cmpexch (اشاره_تهی, cache))
        hb_free (cache);
    }
    فراغ clear_advance_cache () ثابت
    {
    retry:
      خودکار *cache = advance_cache.get_acquire ();
      إذا (!cache)
	أرجع;
      إذا (advance_cache.cmpexch (cache, اشاره_تهی))
	hb_free (cache);
      وإلا
        اذهب retry;
    }

    OT::hb_scalar_cache_t *acquire_varStore_cache (ثابت OT::قلم_Variation_Store &varStore) ثابت
    {
    retry:
      خودکار *cache = varStore_cache.get_acquire ();
      إذا (!cache)
	أرجع varStore.create_cache ();
      إذا (varStore_cache.cmpexch (cache, اشاره_تهی))
	أرجع cache;
      وإلا
	اذهب retry;
    }
    فراغ release_varStore_cache (OT::hb_scalar_cache_t *cache) ثابت
    {
      إذا (!cache)
	أرجع;
      إذا (!varStore_cache.cmpexch (اشاره_تهی, cache))
	OT::قلم_Variation_Store::destroy_cache (cache);
    }
    فراغ clear_varStore_cache () ثابت
    {
    retry:
      خودکار *cache = varStore_cache.get_acquire ();
      إذا (!cache)
	أرجع;
      إذا (varStore_cache.cmpexch (cache, اشاره_تهی))
	OT::قلم_Variation_Store::destroy_cache (cache);
      وإلا
	اذهب retry;
    }

    فراغ clear () ثابت
    {
      clear_advance_cache ();
      clear_varStore_cache ();
    }

  } h, v;

  بنية origin_cache_t
  {
    تغییرپذیر hb_atomic_t<hb_ot_font_origin_cache_t *> origin_cache;
    تغییرپذیر hb_atomic_t<OT::hb_scalar_cache_t *> varStore_cache;

    ~origin_cache_t ()
    {
      clear ();
    }

    hb_ot_font_origin_cache_t *acquire_origin_cache () ثابت
    {
    retry:
      خودکار *cache = origin_cache.get_acquire ();
      إذا (!cache)
      {
        cache = (hb_ot_font_origin_cache_t *) hb_malloc (حجم (hb_ot_font_origin_cache_t));
	إذا (!cache)
	  أرجع اشاره_تهی;
	جديد (cache) hb_ot_font_origin_cache_t;
	أرجع cache;
      }
      إذا (origin_cache.cmpexch (cache, اشاره_تهی))
        أرجع cache;
      وإلا
        اذهب retry;
    }
    فراغ release_origin_cache (hb_ot_font_origin_cache_t *cache) ثابت
    {
      إذا (!cache)
        أرجع;
      إذا (!origin_cache.cmpexch (اشاره_تهی, cache))
        hb_free (cache);
    }
    فراغ clear_origin_cache () ثابت
    {
    retry:
      خودکار *cache = origin_cache.get_acquire ();
      إذا (!cache)
	أرجع;
      إذا (origin_cache.cmpexch (cache, اشاره_تهی))
	hb_free (cache);
      وإلا
        اذهب retry;
    }

    OT::hb_scalar_cache_t *acquire_varStore_cache (ثابت OT::قلم_Variation_Store &varStore) ثابت
    {
    retry:
      خودکار *cache = varStore_cache.get_acquire ();
      إذا (!cache)
	أرجع varStore.create_cache ();
      إذا (varStore_cache.cmpexch (cache, اشاره_تهی))
	أرجع cache;
      وإلا
	اذهب retry;
    }
    فراغ release_varStore_cache (OT::hb_scalar_cache_t *cache) ثابت
    {
      إذا (!cache)
	أرجع;
      إذا (!varStore_cache.cmpexch (اشاره_تهی, cache))
	OT::قلم_Variation_Store::destroy_cache (cache);
    }
    فراغ clear_varStore_cache () ثابت
    {
    retry:
      خودکار *cache = varStore_cache.get_acquire ();
      إذا (!cache)
	أرجع;
      إذا (varStore_cache.cmpexch (cache, اشاره_تهی))
	OT::قلم_Variation_Store::destroy_cache (cache);
      وإلا
	اذهب retry;
    }

    فراغ clear () ثابت
    {
      clear_origin_cache ();
      clear_varStore_cache ();
    }
  } v_origin;

  بنية draw_cache_t
  {
    تغییرپذیر hb_atomic_t<OT::hb_scalar_cache_t *> gvar_cache;

    ~draw_cache_t ()
    {
      clear ();
    }

    OT::hb_scalar_cache_t *acquire_gvar_cache (ثابت OT::gvar_accelerator_t &gvar) ثابت
    {
    retry:
      خودکار *cache = gvar_cache.get_acquire ();
      إذا (!cache)
	أرجع gvar.create_cache ();
      إذا (gvar_cache.cmpexch (cache, اشاره_تهی))
	أرجع cache;
      وإلا
	اذهب retry;
    }
    فراغ release_gvar_cache (OT::hb_scalar_cache_t *cache) ثابت
    {
      إذا (!cache)
	أرجع;
      إذا (!gvar_cache.cmpexch (اشاره_تهی, cache))
	OT::gvar_accelerator_t::destroy_cache (cache);
    }
    فراغ clear_gvar_cache () ثابت
    {
    retry:
      خودکار *cache = gvar_cache.get_acquire ();
      إذا (!cache)
	أرجع;
      إذا (gvar_cache.cmpexch (cache, اشاره_تهی))
	OT::gvar_accelerator_t::destroy_cache (cache);
      وإلا
	اذهب retry;
    }

    فراغ clear () ثابت
    {
      clear_gvar_cache ();
    }
  } draw;

  فراغ check_serial (hb_font_t *font) ثابت
  {
    صحيح font_serial = font->serial.get_acquire ();
    إذا (cached_serial.get_acquire () != font_serial)
    {
      /* These caches are dependent on scale and synthetic settings.
       * Any change to the font invalidates them. */
      v_origin.clear ();

      cached_serial.مجموعه_release (font_serial);
    }

    صحيح font_serial_coords = font->serial_coords.get_acquire ();
    إذا (cached_coords_serial.get_acquire () != font_serial_coords)
    {
      /* These caches are independent of scale or synthetic settings.
       * Just variation changes will invalidate them. */
      h.clear ();
      v.clear ();
      draw.clear ();

      cached_coords_serial.مجموعه_release (font_serial_coords);
    }
  }
};

ساكن hb_ot_font_t *
_hb_ot_font_create (hb_font_t *font)
{
  hb_ot_font_t *ot_font = (hb_ot_font_t *) hb_calloc (1, حجم (hb_ot_font_t));
  إذا (unlikely (!ot_font))
    أرجع اشاره_تهی;

  ot_font->ot_face = &font->face->جدول;

  أرجع ot_font;
}

ساكن فراغ
_hb_ot_font_destroy (فراغ *font_داده)
{
  hb_ot_font_t *ot_font = (hb_ot_font_t *) font_داده;

  ot_font->~hb_ot_font_t ();

  hb_free (ot_font);
}

ساكن hb_bool_t
hb_ot_get_nominal_glyph (hb_font_t *font HB_استفادهنشده,
			 فراغ *font_داده,
			 hb_codepoint_t unicode,
			 hb_codepoint_t *glyph,
			 فراغ *user_داده HB_استفادهنشده)
{
  ثابت hb_ot_font_t *ot_font = (ثابت hb_ot_font_t *) font_داده;
  ثابت hb_ot_face_t *ot_face = ot_font->ot_face;
  أرجع ot_face->cmap->get_nominal_glyph (unicode, glyph);
}

ساكن بیعلامت صحيح
hb_ot_get_nominal_glyphs (hb_font_t *font HB_استفادهنشده,
			  فراغ *font_داده,
			  بیعلامت صحيح شمار,
			  ثابت hb_codepoint_t *اولی_unicode,
			  بیعلامت صحيح unicode_stride,
			  hb_codepoint_t *اولی_glyph,
			  بیعلامت صحيح glyph_stride,
			  فراغ *user_داده HB_استفادهنشده)
{
  ثابت hb_ot_font_t *ot_font = (ثابت hb_ot_font_t *) font_داده;
  ثابت hb_ot_face_t *ot_face = ot_font->ot_face;
  أرجع ot_face->cmap->get_nominal_glyphs (شمار,
					    اولی_unicode, unicode_stride,
					    اولی_glyph, glyph_stride);
}

ساكن hb_bool_t
hb_ot_get_variation_glyph (hb_font_t *font HB_استفادهنشده,
			   فراغ *font_داده,
			   hb_codepoint_t unicode,
			   hb_codepoint_t variation_selector,
			   hb_codepoint_t *glyph,
			   فراغ *user_داده HB_استفادهنشده)
{
  ثابت hb_ot_font_t *ot_font = (ثابت hb_ot_font_t *) font_داده;
  ثابت hb_ot_face_t *ot_face = ot_font->ot_face;
  أرجع ot_face->cmap->get_variation_glyph (unicode,
                                             variation_selector, glyph);
}

ساكن فراغ
hb_ot_get_glyph_h_advances (hb_font_t* font, فراغ* font_داده,
			    بیعلامت شمار,
			    ثابت hb_codepoint_t *اولی_glyph,
			    بیعلامت glyph_stride,
			    hb_position_t *اولی_advance,
			    بیعلامت advance_stride,
			    فراغ *user_داده HB_استفادهنشده)
{
  // Duplicated in v_advances. Ugly. Keep in sync'ish.

  ثابت hb_ot_font_t *ot_font = (ثابت hb_ot_font_t *) font_داده;
  ثابت hb_ot_face_t *ot_face = ot_font->ot_face;
  ثابت OT::hmtx_accelerator_t &hmtx = *ot_face->hmtx;

  إذا (unlikely (!hmtx.has_داده ()))
  {
    hb_position_t advance = font->face->get_upem () / 2;
    advance = font->em_scale_x (advance);
    لكل (بیعلامت صحيح i = 0; i < شمار; i++)
    {
      *اولی_advance = advance;
      اولی_advance = &Struct_At_جابه_جایی_Unaligned<hb_position_t> (اولی_advance, advance_stride);
    }
    أرجع;
  }

#اگر_تعریف_نشده HB_NO_VAR
  إذا (!font->has_nonzero_coords)
  {
  fallback:
#وإلا
  {
#پایان_اگر
    // Just plain htmx data. No need to cache.
    لكل (بیعلامت صحيح i = 0; i < شمار; i++)
    {
      *اولی_advance = font->em_scale_x (hmtx.get_advance_without_var_unscaled (*اولی_glyph));
      اولی_glyph = &Struct_At_جابه_جایی_Unaligned<hb_codepoint_t> (اولی_glyph, glyph_stride);
      اولی_advance = &Struct_At_جابه_جایی_Unaligned<hb_position_t> (اولی_advance, advance_stride);
    }
    أرجع;
  }

#اگر_تعریف_نشده HB_NO_VAR
  /* has_nonzero_coords. */

  ot_font->check_serial (font);
  hb_ot_font_advance_cache_t *advance_cache = ot_font->h.acquire_advance_cache ();
  إذا (!advance_cache)
  {
    // malloc failure. Just use the fallback non-variable path.
    اذهب fallback;
  }

  /* If HVAR is present, use it.*/
  ثابت OT::HVAR &HVAR = *hmtx.var_جدول;
  إذا (HVAR.has_داده ())
  {
    ثابت OT::قلم_Variation_Store &varStore = &HVAR + HVAR.varStore;
    OT::hb_scalar_cache_t *varStore_cache = ot_font->h.acquire_varStore_cache (varStore);

    لكل (بیعلامت صحيح i = 0; i < شمار; i++)
    {
      hb_position_t v;
      بیعلامت cv;
      إذا (advance_cache->get (*اولی_glyph, &cv))
	v = cv;
      وإلا
      {
        v = hmtx.get_advance_with_var_unscaled (*اولی_glyph, font, varStore_cache);
	advance_cache->مجموعه (*اولی_glyph, v);
      }
      *اولی_advance = font->em_scale_x (v);
      اولی_glyph = &Struct_At_جابه_جایی_Unaligned<hb_codepoint_t> (اولی_glyph, glyph_stride);
      اولی_advance = &Struct_At_جابه_جایی_Unaligned<hb_position_t> (اولی_advance, advance_stride);
    }

    ot_font->h.release_varStore_cache (varStore_cache);
    ot_font->h.release_advance_cache (advance_cache);
    أرجع;
  }

  ثابت خودکار &gvar = *ot_face->gvar;
  إذا (gvar.has_داده ())
  {
    ثابت خودکار &glyf = *ot_face->glyf;
    خودکار *scratch = glyf.acquire_scratch ();
    إذا (unlikely (!scratch))
    {
      ot_font->h.release_advance_cache (advance_cache);
      اذهب fallback;
    }
    OT::hb_scalar_cache_t *gvar_cache = ot_font->draw.acquire_gvar_cache (gvar);

    لكل (بیعلامت صحيح i = 0; i < شمار; i++)
    {
      hb_position_t v;
      بیعلامت cv;
      إذا (advance_cache->get (*اولی_glyph, &cv))
	v = cv;
      وإلا
      {
        v = glyf.get_advance_with_var_unscaled (*اولی_glyph, font, خطأ, *scratch, gvar_cache);
	advance_cache->مجموعه (*اولی_glyph, v);
      }
      *اولی_advance = font->em_scale_x (v);
      اولی_glyph = &Struct_At_جابه_جایی_Unaligned<hb_codepoint_t> (اولی_glyph, glyph_stride);
      اولی_advance = &Struct_At_جابه_جایی_Unaligned<hb_position_t> (اولی_advance, advance_stride);
    }

    ot_font->draw.release_gvar_cache (gvar_cache);
    glyf.release_scratch (scratch);
    ot_font->h.release_advance_cache (advance_cache);
    أرجع;
  }

  ot_font->h.release_advance_cache (advance_cache);
  // No HVAR or GVAR.  Just use the fallback non-variable path.
  اذهب fallback;
#پایان_اگر
}

#اگر_تعریف_نشده HB_NO_VERTICAL
ساكن فراغ
hb_ot_get_glyph_v_advances (hb_font_t* font, فراغ* font_داده,
			    بیعلامت شمار,
			    ثابت hb_codepoint_t *اولی_glyph,
			    بیعلامت glyph_stride,
			    hb_position_t *اولی_advance,
			    بیعلامت advance_stride,
			    فراغ *user_داده HB_استفادهنشده)
{
  // Duplicated from h_advances. Ugly. Keep in sync'ish.

  ثابت hb_ot_font_t *ot_font = (ثابت hb_ot_font_t *) font_داده;
  ثابت hb_ot_face_t *ot_face = ot_font->ot_face;
  ثابت OT::vmtx_accelerator_t &vmtx = *ot_face->vmtx;

  إذا (unlikely (!vmtx.has_داده ()))
  {
    hb_font_extents_t font_extents;
    font->get_h_extents_with_fallback (&font_extents);
    hb_position_t advance = font_extents.descender - font_extents.ascender;
    لكل (بیعلامت صحيح i = 0; i < شمار; i++)
    {
      *اولی_advance = advance;
      اولی_advance = &Struct_At_جابه_جایی_Unaligned<hb_position_t> (اولی_advance, advance_stride);
    }
    أرجع;
  }

#اگر_تعریف_نشده HB_NO_VAR
  إذا (!font->has_nonzero_coords)
  {
  fallback:
#وإلا
  {
#پایان_اگر
    // Just plain vtmx data. No need to cache.
    لكل (بیعلامت صحيح i = 0; i < شمار; i++)
    {
      *اولی_advance = font->em_scale_y (- (صحيح) vmtx.get_advance_without_var_unscaled (*اولی_glyph));
      اولی_glyph = &Struct_At_جابه_جایی_Unaligned<hb_codepoint_t> (اولی_glyph, glyph_stride);
      اولی_advance = &Struct_At_جابه_جایی_Unaligned<hb_position_t> (اولی_advance, advance_stride);
    }
    أرجع;
  }

#اگر_تعریف_نشده HB_NO_VAR
  /* has_nonzero_coords. */

  ot_font->check_serial (font);
  hb_ot_font_advance_cache_t *advance_cache = ot_font->v.acquire_advance_cache ();
  إذا (!advance_cache)
  {
    // malloc failure. Just use the fallback non-variable path.
    اذهب fallback;
  }

  /* If VVAR is present, use it.*/
  ثابت OT::VVAR &VVAR = *vmtx.var_جدول;
  إذا (VVAR.has_داده ())
  {
    ثابت OT::قلم_Variation_Store &varStore = &VVAR + VVAR.varStore;
    OT::hb_scalar_cache_t *varStore_cache = ot_font->v.acquire_varStore_cache (varStore);

    لكل (بیعلامت صحيح i = 0; i < شمار; i++)
    {
      hb_position_t v;
      بیعلامت cv;
      إذا (advance_cache->get (*اولی_glyph, &cv))
	v = cv;
      وإلا
      {
        v = vmtx.get_advance_with_var_unscaled (*اولی_glyph, font, varStore_cache);
	advance_cache->مجموعه (*اولی_glyph, v);
      }
      *اولی_advance = font->em_scale_y (- (صحيح) v);
      اولی_glyph = &Struct_At_جابه_جایی_Unaligned<hb_codepoint_t> (اولی_glyph, glyph_stride);
      اولی_advance = &Struct_At_جابه_جایی_Unaligned<hb_position_t> (اولی_advance, advance_stride);
    }

    ot_font->v.release_varStore_cache (varStore_cache);
    ot_font->v.release_advance_cache (advance_cache);
    أرجع;
  }

  ثابت خودکار &gvar = *ot_face->gvar;
  إذا (gvar.has_داده ())
  {
    ثابت خودکار &glyf = *ot_face->glyf;
    خودکار *scratch = glyf.acquire_scratch ();
    إذا (unlikely (!scratch))
    {
      ot_font->v.release_advance_cache (advance_cache);
      اذهب fallback;
    }
    OT::hb_scalar_cache_t *gvar_cache = ot_font->draw.acquire_gvar_cache (gvar);

    لكل (بیعلامت صحيح i = 0; i < شمار; i++)
    {
      hb_position_t v;
      بیعلامت cv;
      إذا (advance_cache->get (*اولی_glyph, &cv))
	v = cv;
      وإلا
      {
        v = glyf.get_advance_with_var_unscaled (*اولی_glyph, font, درست, *scratch, gvar_cache);
	advance_cache->مجموعه (*اولی_glyph, v);
      }
      *اولی_advance = font->em_scale_y (- (صحيح) v);
      اولی_glyph = &Struct_At_جابه_جایی_Unaligned<hb_codepoint_t> (اولی_glyph, glyph_stride);
      اولی_advance = &Struct_At_جابه_جایی_Unaligned<hb_position_t> (اولی_advance, advance_stride);
    }

    ot_font->draw.release_gvar_cache (gvar_cache);
    glyf.release_scratch (scratch);
    ot_font->v.release_advance_cache (advance_cache);
    أرجع;
  }

  ot_font->v.release_advance_cache (advance_cache);
  // No VVAR or GVAR.  Just use the fallback non-variable path.
  اذهب fallback;
#پایان_اگر
}
#پایان_اگر

#اگر_تعریف_نشده HB_NO_VERTICAL
HB_HOT
ساكن hb_bool_t
hb_ot_get_glyph_v_origins (hb_font_t *font,
			   فراغ *font_داده,
			   بیعلامت صحيح شمار,
			   ثابت hb_codepoint_t *اولی_glyph,
			   بیعلامت glyph_stride,
			   hb_position_t *اولی_x,
			   بیعلامت x_stride,
			   hb_position_t *اولی_y,
			   بیعلامت y_stride,
			   فراغ *user_داده HB_استفادهنشده)
{
  ثابت hb_ot_font_t *ot_font = (ثابت hb_ot_font_t *) font_داده;
  ثابت hb_ot_face_t *ot_face = ot_font->ot_face;

  ot_font->check_serial (font);

  /* First, set all the x values to half the advance width. */
  font->get_glyph_h_advances (شمار,
			      اولی_glyph, glyph_stride,
			      اولی_x, x_stride);
  لكل (بیعلامت i = 0; i < شمار; i++)
  {
    *اولی_x /= 2;
    اولی_x = &Struct_At_جابه_جایی_Unaligned<hb_position_t> (اولی_x, x_stride);
  }

  /* The vertical origin business is messy...
   *
   * We allocate the cache, then have various code paths that use the cache.
   * Each one is responsible to free it before returning.
   */
  hb_ot_font_origin_cache_t *origin_cache = ot_font->v_origin.acquire_origin_cache ();

  /* If there is VORG, always use it. It uses VVAR for variations if necessary. */
  ثابت OT::VORG &VORG = *ot_face->VORG;
  إذا (origin_cache && VORG.has_داده ())
  {
#اگر_تعریف_نشده HB_NO_VAR
    إذا (!font->has_nonzero_coords)
#پایان_اگر
    {
      لكل (بیعلامت i = 0; i < شمار; i++)
      {
	hb_position_t origin;
	بیعلامت cv;
	إذا (origin_cache->get (*اولی_glyph, &cv))
	  origin = font->y_scale < 0 ? -static_cast<hb_position_t>(cv) : static_cast<hb_position_t>(cv);
	وإلا
	{
	  origin = font->em_scalef_y (VORG.get_y_origin (*اولی_glyph));
	  origin_cache->مجموعه (*اولی_glyph, font->y_scale < 0 ? -origin : origin);
	}

	*اولی_y = origin;

	اولی_glyph = &Struct_At_جابه_جایی_Unaligned<hb_codepoint_t> (اولی_glyph, glyph_stride);
	اولی_y = &Struct_At_جابه_جایی_Unaligned<hb_position_t> (اولی_y, y_stride);
      }
    }
#اگر_تعریف_نشده HB_NO_VAR
    وإلا
    {
      ثابت OT::VVAR &VVAR = *ot_face->vmtx->var_جدول;
      ثابت خودکار &varStore = &VVAR + VVAR.varStore;
      خودکار *varStore_cache = ot_font->v_origin.acquire_varStore_cache (varStore);
      لكل (بیعلامت i = 0; i < شمار; i++)
      {
	hb_position_t origin;
	بیعلامت cv;
	إذا (origin_cache->get (*اولی_glyph, &cv))
	  origin = font->y_scale < 0 ? -static_cast<hb_position_t>(cv) : static_cast<hb_position_t>(cv);
	وإلا
	{
	  origin = font->em_scalef_y (VORG.get_y_origin (*اولی_glyph) +
				      VVAR.get_vorg_delta_unscaled (*اولی_glyph,
								    font->coords, font->رقم_coords,
								    varStore_cache));
	  origin_cache->مجموعه (*اولی_glyph, font->y_scale < 0 ? -origin : origin);
	}

	*اولی_y = origin;

	اولی_glyph = &Struct_At_جابه_جایی_Unaligned<hb_codepoint_t> (اولی_glyph, glyph_stride);
	اولی_y = &Struct_At_جابه_جایی_Unaligned<hb_position_t> (اولی_y, y_stride);
      }
      ot_font->v_origin.release_varStore_cache (varStore_cache);
    }
#پایان_اگر
    ot_font->v_origin.release_origin_cache (origin_cache);
    أرجع درست;
  }

  /* If and only if `vmtx` is present and it's a `glyf` font,
   * we use the top phantom point, deduced from vmtx,glyf[,gvar]. */
  ثابت خودکار &vmtx = *ot_face->vmtx;
  ثابت خودکار &glyf = *ot_face->glyf;
  إذا (origin_cache && vmtx.has_داده() && glyf.has_داده ())
  {
    خودکار *scratch = glyf.acquire_scratch ();
    إذا (unlikely (!scratch))
    {
      ot_font->v_origin.release_origin_cache (origin_cache);
      أرجع خطأ;
    }
    OT::hb_scalar_cache_t *gvar_cache = اشاره_تهی;
#اگر_تعریف_نشده HB_NO_VAR
    gvar_cache = font->has_nonzero_coords ?
					ot_font->draw.acquire_gvar_cache (*ot_face->gvar) :
					اشاره_تهی;
#پایان_اگر

    لكل (بیعلامت i = 0; i < شمار; i++)
    {
      hb_position_t origin;
      بیعلامت cv;
      إذا (origin_cache->get (*اولی_glyph, &cv))
	origin = font->y_scale < 0 ? -static_cast<hb_position_t>(cv) : static_cast<hb_position_t>(cv);
      وإلا
      {
	origin = font->em_scalef_y (glyf.get_v_origin_with_var_unscaled (*اولی_glyph, font, *scratch, gvar_cache));
	origin_cache->مجموعه (*اولی_glyph, font->y_scale < 0 ? -origin : origin);
      }

      *اولی_y = origin;

      اولی_glyph = &Struct_At_جابه_جایی_Unaligned<hb_codepoint_t> (اولی_glyph, glyph_stride);
      اولی_y = &Struct_At_جابه_جایی_Unaligned<hb_position_t> (اولی_y, y_stride);
    }

#اگر_تعریف_نشده HB_NO_VAR
    إذا (gvar_cache)
      ot_font->draw.release_gvar_cache (gvar_cache);
#پایان_اگر
    glyf.release_scratch (scratch);
    ot_font->v_origin.release_origin_cache (origin_cache);
    أرجع درست;
  }

  /* Otherwise, use glyph extents to center the glyph vertically.
   * If getting glyph extents failed, just use the font ascender. */
  إذا (origin_cache && font->has_glyph_extents_func ())
  {
    hb_font_extents_t font_extents;
    font->get_h_extents_with_fallback (&font_extents);
    hb_position_t font_advance = font_extents.ascender - font_extents.descender;

    لكل (بیعلامت i = 0; i < شمار; i++)
    {
      hb_position_t origin;
      بیعلامت cv;

      إذا (origin_cache->get (*اولی_glyph, &cv))
	origin = font->y_scale < 0 ? -static_cast<hb_position_t>(cv) : static_cast<hb_position_t>(cv);
      وإلا
      {
	hb_glyph_extents_t extents = {0};
	إذا (likely (font->get_glyph_extents (*اولی_glyph, &extents)))
	  origin = hb_clamp_مقصد<hb_position_t> ((int64_t) extents.y_bearing +
					 (((int64_t) font_advance + extents.ارتفاع) >> 1));
	وإلا
	  origin = font_extents.ascender;

	origin_cache->مجموعه (*اولی_glyph, font->y_scale < 0 ? -origin : origin);
      }

      *اولی_y = origin;

      اولی_glyph = &Struct_At_جابه_جایی_Unaligned<hb_codepoint_t> (اولی_glyph, glyph_stride);
      اولی_y = &Struct_At_جابه_جایی_Unaligned<hb_position_t> (اولی_y, y_stride);
    }
  }

  ot_font->v_origin.release_origin_cache (origin_cache);
  أرجع درست;
}
#پایان_اگر

ساكن hb_bool_t
hb_ot_get_glyph_extents (hb_font_t *font,
			 فراغ *font_داده,
			 hb_codepoint_t glyph,
			 hb_glyph_extents_t *extents,
			 فراغ *user_داده HB_استفادهنشده)
{
  ثابت hb_ot_font_t *ot_font = (ثابت hb_ot_font_t *) font_داده;
  ثابت hb_ot_face_t *ot_face = ot_font->ot_face;

#إذا !defined(HB_NO_OT_FONT_BITMAP) && !defined(HB_NO_COLOR)
  إذا (ot_face->sbix->get_extents (font, glyph, extents)) أرجع درست;
  إذا (ot_face->CBDT->get_extents (font, glyph, extents)) أرجع درست;
#پایان_اگر
#إذا !defined(HB_NO_COLOR) && !defined(HB_NO_PAINT)
  إذا (ot_face->COLR->get_extents (font, glyph, extents)) أرجع درست;
#پایان_اگر
#اگر_تعریف_نشده HB_NO_VAR_COMPOSITES
  إذا (ot_face->VARC->get_extents (font, glyph, extents)) أرجع درست;
#پایان_اگر
  إذا (ot_face->glyf->get_extents (font, glyph, extents)) أرجع درست;
#اگر_تعریف_نشده HB_NO_OT_FONT_CFF
  إذا (ot_face->cff2->get_extents (font, glyph, extents)) أرجع درست;
  إذا (ot_face->cff1->get_extents (font, glyph, extents)) أرجع درست;
#پایان_اگر

  أرجع خطأ;
}

#اگر_تعریف_نشده HB_NO_OT_FONT_GLYPH_نامها
ساكن hb_bool_t
hb_ot_get_glyph_name (hb_font_t *font HB_استفادهنشده,
		      فراغ *font_داده,
		      hb_codepoint_t glyph,
		      حرف *name, بیعلامت صحيح اندازه,
		      فراغ *user_داده HB_استفادهنشده)
{
  ثابت hb_ot_font_t *ot_font = (ثابت hb_ot_font_t *) font_داده;
  ثابت hb_ot_face_t *ot_face = ot_font->ot_face;

  إذا (ot_face->post->get_glyph_name (glyph, name, اندازه)) أرجع درست;
#اگر_تعریف_نشده HB_NO_OT_FONT_CFF
  إذا (ot_face->cff1->get_glyph_name (glyph, name, اندازه)) أرجع درست;
#پایان_اگر
  أرجع خطأ;
}
ساكن hb_bool_t
hb_ot_get_glyph_from_name (hb_font_t *font HB_استفادهنشده,
			   فراغ *font_داده,
			   ثابت حرف *name, صحيح len,
			   hb_codepoint_t *glyph,
			   فراغ *user_داده HB_استفادهنشده)
{
  ثابت hb_ot_font_t *ot_font = (ثابت hb_ot_font_t *) font_داده;
  ثابت hb_ot_face_t *ot_face = ot_font->ot_face;

  إذا (ot_face->post->get_glyph_from_name (name, len, glyph)) أرجع درست;
#اگر_تعریف_نشده HB_NO_OT_FONT_CFF
    إذا (ot_face->cff1->get_glyph_from_name (name, len, glyph)) أرجع درست;
#پایان_اگر
  أرجع خطأ;
}
#پایان_اگر

ساكن hb_bool_t
hb_ot_get_font_h_extents (hb_font_t *font,
			  فراغ *font_داده HB_استفادهنشده,
			  hb_font_extents_t *metrics,
			  فراغ *user_داده HB_استفادهنشده)
{
  أرجع _hb_ot_metrics_get_position_common (font, HB_OT_METRICS_برچسب_HORIZONTAL_ASCENDER, &metrics->ascender) &&
	 _hb_ot_metrics_get_position_common (font, HB_OT_METRICS_برچسب_HORIZONTAL_DESCENDER, &metrics->descender) &&
	 _hb_ot_metrics_get_position_common (font, HB_OT_METRICS_برچسب_HORIZONTAL_سطر_GAP, &metrics->سطر_gap);
}

#اگر_تعریف_نشده HB_NO_VERTICAL
ساكن hb_bool_t
hb_ot_get_font_v_extents (hb_font_t *font,
			  فراغ *font_داده HB_استفادهنشده,
			  hb_font_extents_t *metrics,
			  فراغ *user_داده HB_استفادهنشده)
{
  أرجع _hb_ot_metrics_get_position_common (font, HB_OT_METRICS_برچسب_VERTICAL_ASCENDER, &metrics->ascender) &&
	 _hb_ot_metrics_get_position_common (font, HB_OT_METRICS_برچسب_VERTICAL_DESCENDER, &metrics->descender) &&
	 _hb_ot_metrics_get_position_common (font, HB_OT_METRICS_برچسب_VERTICAL_سطر_GAP, &metrics->سطر_gap);
}
#پایان_اگر

#اگر_تعریف_نشده HB_NO_DRAW
ساكن hb_bool_t
hb_ot_draw_glyph_or_fail (hb_font_t *font,
			  فراغ *font_داده HB_استفادهنشده,
			  hb_codepoint_t glyph,
			  hb_draw_funcs_t *draw_funcs, فراغ *draw_داده,
			  فراغ *user_داده)
{
  ثابت hb_ot_font_t *ot_font = (ثابت hb_ot_font_t *) font_داده;
  hb_draw_session_t draw_session {draw_funcs, draw_داده};

  OT::hb_scalar_cache_t *gvar_cache = اشاره_تهی;
#اگر_تعریف_نشده HB_NO_VAR
  إذا (font->رقم_coords)
  {
    ot_font->check_serial (font);
    gvar_cache = ot_font->draw.acquire_gvar_cache (*ot_font->ot_face->gvar);
  }
#پایان_اگر
  HB_SCOPE_GUARD (ot_font->draw.release_gvar_cache (gvar_cache));

#اگر_تعریف_نشده HB_NO_VAR_COMPOSITES
  إذا (font->face->جدول.VARC->get_مسیر (font, glyph, draw_session)) أرجع درست;
#پایان_اگر
  // Keep the following in synch with VARC::get_path_at()
  إذا (font->face->جدول.glyf->get_مسیر (font, glyph, draw_session, gvar_cache,
				       &draw_session.get_budget ())) أرجع درست;

#اگر_تعریف_نشده HB_NO_CFF
  إذا (font->face->جدول.cff2->get_مسیر (font, glyph, draw_session,
				       &draw_session.get_budget ())) أرجع درست;
  إذا (font->face->جدول.cff1->get_مسیر (font, glyph, draw_session,
				       &draw_session.get_budget ())) أرجع درست;
#پایان_اگر

  أرجع خطأ;
}
#پایان_اگر

#اگر_تعریف_نشده HB_NO_PAINT
ساكن hb_bool_t
hb_ot_paint_glyph_or_fail (hb_font_t *font,
			   فراغ *font_داده,
			   hb_codepoint_t glyph,
			   hb_paint_funcs_t *paint_funcs, فراغ *paint_داده,
			   بیعلامت صحيح palette,
			   hb_color_t foreground,
			   فراغ *user_داده)
{
#اگر_تعریف_نشده HB_NO_COLOR
  إذا (font->face->جدول.COLR->paint_glyph (font, glyph, paint_funcs, paint_داده, palette, foreground)) أرجع درست;
#اگر_تعریف_نشده HB_NO_SVG
  إذا (font->face->جدول.SVG->paint_glyph (font, glyph, paint_funcs, paint_داده)) أرجع درست;
#پایان_اگر
#اگر_تعریف_نشده HB_NO_OT_FONT_BITMAP
  إذا (font->face->جدول.CBDT->paint_glyph (font, glyph, paint_funcs, paint_داده)) أرجع درست;
  إذا (font->face->جدول.sbix->paint_glyph (font, glyph, paint_funcs, paint_داده)) أرجع درست;
#پایان_اگر
#پایان_اگر
  أرجع خطأ;
}
#پایان_اگر

ساكن مضمن فراغ free_static_ot_funcs ();

ساكن بنية hb_ot_font_funcs_lazy_loader_t : hb_font_funcs_lazy_loader_t<hb_ot_font_funcs_lazy_loader_t>
{
  ساكن hb_font_funcs_t *create ()
  {
    hb_font_funcs_t *funcs = hb_font_funcs_create ();

    hb_font_funcs_مجموعه_nominal_glyph_func (funcs, hb_ot_get_nominal_glyph, اشاره_تهی, اشاره_تهی);
    hb_font_funcs_مجموعه_nominal_glyphs_func (funcs, hb_ot_get_nominal_glyphs, اشاره_تهی, اشاره_تهی);
    hb_font_funcs_مجموعه_variation_glyph_func (funcs, hb_ot_get_variation_glyph, اشاره_تهی, اشاره_تهی);

    hb_font_funcs_مجموعه_font_h_extents_func (funcs, hb_ot_get_font_h_extents, اشاره_تهی, اشاره_تهی);
    hb_font_funcs_مجموعه_glyph_h_advances_func (funcs, hb_ot_get_glyph_h_advances, اشاره_تهی, اشاره_تهی);

#اگر_تعریف_نشده HB_NO_VERTICAL
    hb_font_funcs_مجموعه_font_v_extents_func (funcs, hb_ot_get_font_v_extents, اشاره_تهی, اشاره_تهی);
    hb_font_funcs_مجموعه_glyph_v_advances_func (funcs, hb_ot_get_glyph_v_advances, اشاره_تهی, اشاره_تهی);
    hb_font_funcs_مجموعه_glyph_v_origins_func (funcs, hb_ot_get_glyph_v_origins, اشاره_تهی, اشاره_تهی);
#پایان_اگر

#اگر_تعریف_نشده HB_NO_DRAW
    hb_font_funcs_مجموعه_draw_glyph_or_fail_func (funcs, hb_ot_draw_glyph_or_fail, اشاره_تهی, اشاره_تهی);
#پایان_اگر

#اگر_تعریف_نشده HB_NO_PAINT
    hb_font_funcs_مجموعه_paint_glyph_or_fail_func (funcs, hb_ot_paint_glyph_or_fail, اشاره_تهی, اشاره_تهی);
#پایان_اگر

    hb_font_funcs_مجموعه_glyph_extents_func (funcs, hb_ot_get_glyph_extents, اشاره_تهی, اشاره_تهی);
    //hb_font_funcs_set_glyph_contour_point_func (funcs, hb_ot_get_glyph_contour_point, nullptr, nullptr);

#اگر_تعریف_نشده HB_NO_OT_FONT_GLYPH_نامها
    hb_font_funcs_مجموعه_glyph_name_func (funcs, hb_ot_get_glyph_name, اشاره_تهی, اشاره_تهی);
    hb_font_funcs_مجموعه_glyph_from_name_func (funcs, hb_ot_get_glyph_from_name, اشاره_تهی, اشاره_تهی);
#پایان_اگر

    hb_font_funcs_make_immutable (funcs);

    hb_atexit (free_static_ot_funcs);

    أرجع funcs;
  }
} static_ot_funcs;

ساكن مضمن
فراغ free_static_ot_funcs ()
{
  static_ot_funcs.free_instance ();
}

ساكن hb_font_funcs_t *
_hb_ot_get_font_funcs ()
{
  أرجع static_ot_funcs.get_unconst ();
}


/**
 * hb_ot_font_set_funcs:
 * @font: #hb_font_t to work upon
 *
 * Sets the font functions to use when working with @font to
 * the HarfBuzz's native implementation. This is the default
 * for fonts newly created.
 *
 * Since: 0.9.28
 **/
فراغ
hb_ot_font_مجموعه_funcs (hb_font_t *font)
{
  hb_ot_font_t *ot_font = _hb_ot_font_create (font);
  إذا (unlikely (!ot_font))
    أرجع;

  hb_font_مجموعه_funcs (font,
		     _hb_ot_get_font_funcs (),
		     ot_font,
		     _hb_ot_font_destroy);
}

#پایان_اگر
