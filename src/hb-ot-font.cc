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

#جے_بنا_تعریف HB_NO_OT_FONT

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

ورتوں hb_ot_font_advance_cache_t = hb_cache_t<24, 16>;
static_assert (حجم (hb_ot_font_advance_cache_t) == 1024, "");

ورتوں hb_ot_font_origin_cache_t = hb_cache_t<20, 20>;
static_assert (حجم (hb_ot_font_origin_cache_t) == 1024, "");

بنية hb_ot_font_t
{
  ثابت hb_ot_face_t *ot_face;

  بدلنجوگ hb_atomic_t<صحيح> cached_serial;
  بدلنجوگ hb_atomic_t<صحيح> cached_coords_serial;

  بنية direction_cache_t
  {
    بدلنجوگ hb_atomic_t<hb_ot_font_advance_cache_t *> advance_cache;
    بدلنجوگ hb_atomic_t<OT::hb_scalar_cache_t *> varStore_cache;

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
	  أرجع خالی_سوچک;
	جديد (cache) hb_ot_font_advance_cache_t;
	أرجع cache;
      }
      إذا (advance_cache.cmpexch (cache, خالی_سوچک))
        أرجع cache;
      وإلا
        اذهب retry;
    }
    فراغ release_advance_cache (hb_ot_font_advance_cache_t *cache) ثابت
    {
      إذا (!cache)
        أرجع;
      إذا (!advance_cache.cmpexch (خالی_سوچک, cache))
        hb_free (cache);
    }
    فراغ clear_advance_cache () ثابت
    {
    retry:
      خودکار *cache = advance_cache.get_acquire ();
      إذا (!cache)
	أرجع;
      إذا (advance_cache.cmpexch (cache, خالی_سوچک))
	hb_free (cache);
      وإلا
        اذهب retry;
    }

    OT::hb_scalar_cache_t *acquire_varStore_cache (ثابت OT::شے_Variation_Store &varStore) ثابت
    {
    retry:
      خودکار *cache = varStore_cache.get_acquire ();
      إذا (!cache)
	أرجع varStore.create_cache ();
      إذا (varStore_cache.cmpexch (cache, خالی_سوچک))
	أرجع cache;
      وإلا
	اذهب retry;
    }
    فراغ release_varStore_cache (OT::hb_scalar_cache_t *cache) ثابت
    {
      إذا (!cache)
	أرجع;
      إذا (!varStore_cache.cmpexch (خالی_سوچک, cache))
	OT::شے_Variation_Store::destroy_cache (cache);
    }
    فراغ clear_varStore_cache () ثابت
    {
    retry:
      خودکار *cache = varStore_cache.get_acquire ();
      إذا (!cache)
	أرجع;
      إذا (varStore_cache.cmpexch (cache, خالی_سوچک))
	OT::شے_Variation_Store::destroy_cache (cache);
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
    بدلنجوگ hb_atomic_t<hb_ot_font_origin_cache_t *> origin_cache;
    بدلنجوگ hb_atomic_t<OT::hb_scalar_cache_t *> varStore_cache;

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
	  أرجع خالی_سوچک;
	جديد (cache) hb_ot_font_origin_cache_t;
	أرجع cache;
      }
      إذا (origin_cache.cmpexch (cache, خالی_سوچک))
        أرجع cache;
      وإلا
        اذهب retry;
    }
    فراغ release_origin_cache (hb_ot_font_origin_cache_t *cache) ثابت
    {
      إذا (!cache)
        أرجع;
      إذا (!origin_cache.cmpexch (خالی_سوچک, cache))
        hb_free (cache);
    }
    فراغ clear_origin_cache () ثابت
    {
    retry:
      خودکار *cache = origin_cache.get_acquire ();
      إذا (!cache)
	أرجع;
      إذا (origin_cache.cmpexch (cache, خالی_سوچک))
	hb_free (cache);
      وإلا
        اذهب retry;
    }

    OT::hb_scalar_cache_t *acquire_varStore_cache (ثابت OT::شے_Variation_Store &varStore) ثابت
    {
    retry:
      خودکار *cache = varStore_cache.get_acquire ();
      إذا (!cache)
	أرجع varStore.create_cache ();
      إذا (varStore_cache.cmpexch (cache, خالی_سوچک))
	أرجع cache;
      وإلا
	اذهب retry;
    }
    فراغ release_varStore_cache (OT::hb_scalar_cache_t *cache) ثابت
    {
      إذا (!cache)
	أرجع;
      إذا (!varStore_cache.cmpexch (خالی_سوچک, cache))
	OT::شے_Variation_Store::destroy_cache (cache);
    }
    فراغ clear_varStore_cache () ثابت
    {
    retry:
      خودکار *cache = varStore_cache.get_acquire ();
      إذا (!cache)
	أرجع;
      إذا (varStore_cache.cmpexch (cache, خالی_سوچک))
	OT::شے_Variation_Store::destroy_cache (cache);
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
    بدلنجوگ hb_atomic_t<OT::hb_scalar_cache_t *> gvar_cache;

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
      إذا (gvar_cache.cmpexch (cache, خالی_سوچک))
	أرجع cache;
      وإلا
	اذهب retry;
    }
    فراغ release_gvar_cache (OT::hb_scalar_cache_t *cache) ثابت
    {
      إذا (!cache)
	أرجع;
      إذا (!gvar_cache.cmpexch (خالی_سوچک, cache))
	OT::gvar_accelerator_t::destroy_cache (cache);
    }
    فراغ clear_gvar_cache () ثابت
    {
    retry:
      خودکار *cache = gvar_cache.get_acquire ();
      إذا (!cache)
	أرجع;
      إذا (gvar_cache.cmpexch (cache, خالی_سوچک))
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

      cached_serial.سیٹ_release (font_serial);
    }

    صحيح font_serial_coords = font->serial_coords.get_acquire ();
    إذا (cached_coords_serial.get_acquire () != font_serial_coords)
    {
      /* These caches are independent of scale or synthetic settings.
       * Just variation changes will invalidate them. */
      h.clear ();
      v.clear ();
      draw.clear ();

      cached_coords_serial.سیٹ_release (font_serial_coords);
    }
  }
};

ساكن hb_ot_font_t *
_hb_ot_font_create (hb_font_t *font)
{
  hb_ot_font_t *ot_font = (hb_ot_font_t *) hb_calloc (1, حجم (hb_ot_font_t));
  إذا (unlikely (!ot_font))
    أرجع خالی_سوچک;

  ot_font->ot_face = &font->face->جدول;

  أرجع ot_font;
}

ساكن فراغ
_hb_ot_font_destroy (فراغ *font_انکڑے)
{
  hb_ot_font_t *ot_font = (hb_ot_font_t *) font_انکڑے;

  ot_font->~hb_ot_font_t ();

  hb_free (ot_font);
}

ساكن hb_bool_t
hb_ot_get_nominal_glyph (hb_font_t *font HB_بن_ورتیا,
			 فراغ *font_انکڑے,
			 hb_codepoint_t unicode,
			 hb_codepoint_t *glyph,
			 فراغ *user_انکڑے HB_بن_ورتیا)
{
  ثابت hb_ot_font_t *ot_font = (ثابت hb_ot_font_t *) font_انکڑے;
  ثابت hb_ot_face_t *ot_face = ot_font->ot_face;
  أرجع ot_face->cmap->get_nominal_glyph (unicode, glyph);
}

ساكن بنانشانی صحيح
hb_ot_get_nominal_glyphs (hb_font_t *font HB_بن_ورتیا,
			  فراغ *font_انکڑے,
			  بنانشانی صحيح گنتی,
			  ثابت hb_codepoint_t *پہلا_unicode,
			  بنانشانی صحيح unicode_stride,
			  hb_codepoint_t *پہلا_glyph,
			  بنانشانی صحيح glyph_stride,
			  فراغ *user_انکڑے HB_بن_ورتیا)
{
  ثابت hb_ot_font_t *ot_font = (ثابت hb_ot_font_t *) font_انکڑے;
  ثابت hb_ot_face_t *ot_face = ot_font->ot_face;
  أرجع ot_face->cmap->get_nominal_glyphs (گنتی,
					    پہلا_unicode, unicode_stride,
					    پہلا_glyph, glyph_stride);
}

ساكن hb_bool_t
hb_ot_get_variation_glyph (hb_font_t *font HB_بن_ورتیا,
			   فراغ *font_انکڑے,
			   hb_codepoint_t unicode,
			   hb_codepoint_t variation_selector,
			   hb_codepoint_t *glyph,
			   فراغ *user_انکڑے HB_بن_ورتیا)
{
  ثابت hb_ot_font_t *ot_font = (ثابت hb_ot_font_t *) font_انکڑے;
  ثابت hb_ot_face_t *ot_face = ot_font->ot_face;
  أرجع ot_face->cmap->get_variation_glyph (unicode,
                                             variation_selector, glyph);
}

ساكن فراغ
hb_ot_get_glyph_h_advances (hb_font_t* font, فراغ* font_انکڑے,
			    بنانشانی گنتی,
			    ثابت hb_codepoint_t *پہلا_glyph,
			    بنانشانی glyph_stride,
			    hb_position_t *پہلا_advance,
			    بنانشانی advance_stride,
			    فراغ *user_انکڑے HB_بن_ورتیا)
{
  // Duplicated in v_advances. Ugly. Keep in sync'ish.

  ثابت hb_ot_font_t *ot_font = (ثابت hb_ot_font_t *) font_انکڑے;
  ثابت hb_ot_face_t *ot_face = ot_font->ot_face;
  ثابت OT::hmtx_accelerator_t &hmtx = *ot_face->hmtx;

  إذا (unlikely (!hmtx.has_انکڑے ()))
  {
    hb_position_t advance = font->face->get_upem () / 2;
    advance = font->em_scale_x (advance);
    لكل (بنانشانی صحيح i = 0; i < گنتی; i++)
    {
      *پہلا_advance = advance;
      پہلا_advance = &Struct_At_وتھ_Unaligned<hb_position_t> (پہلا_advance, advance_stride);
    }
    أرجع;
  }

#جے_بنا_تعریف HB_NO_VAR
  إذا (!font->has_nonzero_coords)
  {
  fallback:
#وإلا
  {
#جے_انت
    // Just plain htmx data. No need to cache.
    لكل (بنانشانی صحيح i = 0; i < گنتی; i++)
    {
      *پہلا_advance = font->em_scale_x (hmtx.get_advance_without_var_unscaled (*پہلا_glyph));
      پہلا_glyph = &Struct_At_وتھ_Unaligned<hb_codepoint_t> (پہلا_glyph, glyph_stride);
      پہلا_advance = &Struct_At_وتھ_Unaligned<hb_position_t> (پہلا_advance, advance_stride);
    }
    أرجع;
  }

#جے_بنا_تعریف HB_NO_VAR
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
  إذا (HVAR.has_انکڑے ())
  {
    ثابت OT::شے_Variation_Store &varStore = &HVAR + HVAR.varStore;
    OT::hb_scalar_cache_t *varStore_cache = ot_font->h.acquire_varStore_cache (varStore);

    لكل (بنانشانی صحيح i = 0; i < گنتی; i++)
    {
      hb_position_t v;
      بنانشانی cv;
      إذا (advance_cache->get (*پہلا_glyph, &cv))
	v = cv;
      وإلا
      {
        v = hmtx.get_advance_with_var_unscaled (*پہلا_glyph, font, varStore_cache);
	advance_cache->سیٹ (*پہلا_glyph, v);
      }
      *پہلا_advance = font->em_scale_x (v);
      پہلا_glyph = &Struct_At_وتھ_Unaligned<hb_codepoint_t> (پہلا_glyph, glyph_stride);
      پہلا_advance = &Struct_At_وتھ_Unaligned<hb_position_t> (پہلا_advance, advance_stride);
    }

    ot_font->h.release_varStore_cache (varStore_cache);
    ot_font->h.release_advance_cache (advance_cache);
    أرجع;
  }

  ثابت خودکار &gvar = *ot_face->gvar;
  إذا (gvar.has_انکڑے ())
  {
    ثابت خودکار &glyf = *ot_face->glyf;
    خودکار *scratch = glyf.acquire_scratch ();
    إذا (unlikely (!scratch))
    {
      ot_font->h.release_advance_cache (advance_cache);
      اذهب fallback;
    }
    OT::hb_scalar_cache_t *gvar_cache = ot_font->draw.acquire_gvar_cache (gvar);

    لكل (بنانشانی صحيح i = 0; i < گنتی; i++)
    {
      hb_position_t v;
      بنانشانی cv;
      إذا (advance_cache->get (*پہلا_glyph, &cv))
	v = cv;
      وإلا
      {
        v = glyf.get_advance_with_var_unscaled (*پہلا_glyph, font, خطأ, *scratch, gvar_cache);
	advance_cache->سیٹ (*پہلا_glyph, v);
      }
      *پہلا_advance = font->em_scale_x (v);
      پہلا_glyph = &Struct_At_وتھ_Unaligned<hb_codepoint_t> (پہلا_glyph, glyph_stride);
      پہلا_advance = &Struct_At_وتھ_Unaligned<hb_position_t> (پہلا_advance, advance_stride);
    }

    ot_font->draw.release_gvar_cache (gvar_cache);
    glyf.release_scratch (scratch);
    ot_font->h.release_advance_cache (advance_cache);
    أرجع;
  }

  ot_font->h.release_advance_cache (advance_cache);
  // No HVAR or GVAR.  Just use the fallback non-variable path.
  اذهب fallback;
#جے_انت
}

#جے_بنا_تعریف HB_NO_VERTICAL
ساكن فراغ
hb_ot_get_glyph_v_advances (hb_font_t* font, فراغ* font_انکڑے,
			    بنانشانی گنتی,
			    ثابت hb_codepoint_t *پہلا_glyph,
			    بنانشانی glyph_stride,
			    hb_position_t *پہلا_advance,
			    بنانشانی advance_stride,
			    فراغ *user_انکڑے HB_بن_ورتیا)
{
  // Duplicated from h_advances. Ugly. Keep in sync'ish.

  ثابت hb_ot_font_t *ot_font = (ثابت hb_ot_font_t *) font_انکڑے;
  ثابت hb_ot_face_t *ot_face = ot_font->ot_face;
  ثابت OT::vmtx_accelerator_t &vmtx = *ot_face->vmtx;

  إذا (unlikely (!vmtx.has_انکڑے ()))
  {
    hb_font_extents_t font_extents;
    font->get_h_extents_with_fallback (&font_extents);
    hb_position_t advance = font_extents.descender - font_extents.ascender;
    لكل (بنانشانی صحيح i = 0; i < گنتی; i++)
    {
      *پہلا_advance = advance;
      پہلا_advance = &Struct_At_وتھ_Unaligned<hb_position_t> (پہلا_advance, advance_stride);
    }
    أرجع;
  }

#جے_بنا_تعریف HB_NO_VAR
  إذا (!font->has_nonzero_coords)
  {
  fallback:
#وإلا
  {
#جے_انت
    // Just plain vtmx data. No need to cache.
    لكل (بنانشانی صحيح i = 0; i < گنتی; i++)
    {
      *پہلا_advance = font->em_scale_y (- (صحيح) vmtx.get_advance_without_var_unscaled (*پہلا_glyph));
      پہلا_glyph = &Struct_At_وتھ_Unaligned<hb_codepoint_t> (پہلا_glyph, glyph_stride);
      پہلا_advance = &Struct_At_وتھ_Unaligned<hb_position_t> (پہلا_advance, advance_stride);
    }
    أرجع;
  }

#جے_بنا_تعریف HB_NO_VAR
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
  إذا (VVAR.has_انکڑے ())
  {
    ثابت OT::شے_Variation_Store &varStore = &VVAR + VVAR.varStore;
    OT::hb_scalar_cache_t *varStore_cache = ot_font->v.acquire_varStore_cache (varStore);

    لكل (بنانشانی صحيح i = 0; i < گنتی; i++)
    {
      hb_position_t v;
      بنانشانی cv;
      إذا (advance_cache->get (*پہلا_glyph, &cv))
	v = cv;
      وإلا
      {
        v = vmtx.get_advance_with_var_unscaled (*پہلا_glyph, font, varStore_cache);
	advance_cache->سیٹ (*پہلا_glyph, v);
      }
      *پہلا_advance = font->em_scale_y (- (صحيح) v);
      پہلا_glyph = &Struct_At_وتھ_Unaligned<hb_codepoint_t> (پہلا_glyph, glyph_stride);
      پہلا_advance = &Struct_At_وتھ_Unaligned<hb_position_t> (پہلا_advance, advance_stride);
    }

    ot_font->v.release_varStore_cache (varStore_cache);
    ot_font->v.release_advance_cache (advance_cache);
    أرجع;
  }

  ثابت خودکار &gvar = *ot_face->gvar;
  إذا (gvar.has_انکڑے ())
  {
    ثابت خودکار &glyf = *ot_face->glyf;
    خودکار *scratch = glyf.acquire_scratch ();
    إذا (unlikely (!scratch))
    {
      ot_font->v.release_advance_cache (advance_cache);
      اذهب fallback;
    }
    OT::hb_scalar_cache_t *gvar_cache = ot_font->draw.acquire_gvar_cache (gvar);

    لكل (بنانشانی صحيح i = 0; i < گنتی; i++)
    {
      hb_position_t v;
      بنانشانی cv;
      إذا (advance_cache->get (*پہلا_glyph, &cv))
	v = cv;
      وإلا
      {
        v = glyf.get_advance_with_var_unscaled (*پہلا_glyph, font, سچ, *scratch, gvar_cache);
	advance_cache->سیٹ (*پہلا_glyph, v);
      }
      *پہلا_advance = font->em_scale_y (- (صحيح) v);
      پہلا_glyph = &Struct_At_وتھ_Unaligned<hb_codepoint_t> (پہلا_glyph, glyph_stride);
      پہلا_advance = &Struct_At_وتھ_Unaligned<hb_position_t> (پہلا_advance, advance_stride);
    }

    ot_font->draw.release_gvar_cache (gvar_cache);
    glyf.release_scratch (scratch);
    ot_font->v.release_advance_cache (advance_cache);
    أرجع;
  }

  ot_font->v.release_advance_cache (advance_cache);
  // No VVAR or GVAR.  Just use the fallback non-variable path.
  اذهب fallback;
#جے_انت
}
#جے_انت

#جے_بنا_تعریف HB_NO_VERTICAL
HB_HOT
ساكن hb_bool_t
hb_ot_get_glyph_v_origins (hb_font_t *font,
			   فراغ *font_انکڑے,
			   بنانشانی صحيح گنتی,
			   ثابت hb_codepoint_t *پہلا_glyph,
			   بنانشانی glyph_stride,
			   hb_position_t *پہلا_x,
			   بنانشانی x_stride,
			   hb_position_t *پہلا_y,
			   بنانشانی y_stride,
			   فراغ *user_انکڑے HB_بن_ورتیا)
{
  ثابت hb_ot_font_t *ot_font = (ثابت hb_ot_font_t *) font_انکڑے;
  ثابت hb_ot_face_t *ot_face = ot_font->ot_face;

  ot_font->check_serial (font);

  /* First, set all the x values to half the advance width. */
  font->get_glyph_h_advances (گنتی,
			      پہلا_glyph, glyph_stride,
			      پہلا_x, x_stride);
  لكل (بنانشانی i = 0; i < گنتی; i++)
  {
    *پہلا_x /= 2;
    پہلا_x = &Struct_At_وتھ_Unaligned<hb_position_t> (پہلا_x, x_stride);
  }

  /* The vertical origin business is messy...
   *
   * We allocate the cache, then have various code paths that use the cache.
   * Each one is responsible to free it before returning.
   */
  hb_ot_font_origin_cache_t *origin_cache = ot_font->v_origin.acquire_origin_cache ();

  /* If there is VORG, always use it. It uses VVAR for variations if necessary. */
  ثابت OT::VORG &VORG = *ot_face->VORG;
  إذا (origin_cache && VORG.has_انکڑے ())
  {
#جے_بنا_تعریف HB_NO_VAR
    إذا (!font->has_nonzero_coords)
#جے_انت
    {
      لكل (بنانشانی i = 0; i < گنتی; i++)
      {
	hb_position_t origin;
	بنانشانی cv;
	إذا (origin_cache->get (*پہلا_glyph, &cv))
	  origin = font->y_scale < 0 ? -static_cast<hb_position_t>(cv) : static_cast<hb_position_t>(cv);
	وإلا
	{
	  origin = font->em_scalef_y (VORG.get_y_origin (*پہلا_glyph));
	  origin_cache->سیٹ (*پہلا_glyph, font->y_scale < 0 ? -origin : origin);
	}

	*پہلا_y = origin;

	پہلا_glyph = &Struct_At_وتھ_Unaligned<hb_codepoint_t> (پہلا_glyph, glyph_stride);
	پہلا_y = &Struct_At_وتھ_Unaligned<hb_position_t> (پہلا_y, y_stride);
      }
    }
#جے_بنا_تعریف HB_NO_VAR
    وإلا
    {
      ثابت OT::VVAR &VVAR = *ot_face->vmtx->var_جدول;
      ثابت خودکار &varStore = &VVAR + VVAR.varStore;
      خودکار *varStore_cache = ot_font->v_origin.acquire_varStore_cache (varStore);
      لكل (بنانشانی i = 0; i < گنتی; i++)
      {
	hb_position_t origin;
	بنانشانی cv;
	إذا (origin_cache->get (*پہلا_glyph, &cv))
	  origin = font->y_scale < 0 ? -static_cast<hb_position_t>(cv) : static_cast<hb_position_t>(cv);
	وإلا
	{
	  origin = font->em_scalef_y (VORG.get_y_origin (*پہلا_glyph) +
				      VVAR.get_vorg_delta_unscaled (*پہلا_glyph,
								    font->coords, font->عدد_coords,
								    varStore_cache));
	  origin_cache->سیٹ (*پہلا_glyph, font->y_scale < 0 ? -origin : origin);
	}

	*پہلا_y = origin;

	پہلا_glyph = &Struct_At_وتھ_Unaligned<hb_codepoint_t> (پہلا_glyph, glyph_stride);
	پہلا_y = &Struct_At_وتھ_Unaligned<hb_position_t> (پہلا_y, y_stride);
      }
      ot_font->v_origin.release_varStore_cache (varStore_cache);
    }
#جے_انت
    ot_font->v_origin.release_origin_cache (origin_cache);
    أرجع سچ;
  }

  /* If and only if `vmtx` is present and it's a `glyf` font,
   * we use the top phantom point, deduced from vmtx,glyf[,gvar]. */
  ثابت خودکار &vmtx = *ot_face->vmtx;
  ثابت خودکار &glyf = *ot_face->glyf;
  إذا (origin_cache && vmtx.has_انکڑے() && glyf.has_انکڑے ())
  {
    خودکار *scratch = glyf.acquire_scratch ();
    إذا (unlikely (!scratch))
    {
      ot_font->v_origin.release_origin_cache (origin_cache);
      أرجع خطأ;
    }
    OT::hb_scalar_cache_t *gvar_cache = خالی_سوچک;
#جے_بنا_تعریف HB_NO_VAR
    gvar_cache = font->has_nonzero_coords ?
					ot_font->draw.acquire_gvar_cache (*ot_face->gvar) :
					خالی_سوچک;
#جے_انت

    لكل (بنانشانی i = 0; i < گنتی; i++)
    {
      hb_position_t origin;
      بنانشانی cv;
      إذا (origin_cache->get (*پہلا_glyph, &cv))
	origin = font->y_scale < 0 ? -static_cast<hb_position_t>(cv) : static_cast<hb_position_t>(cv);
      وإلا
      {
	origin = font->em_scalef_y (glyf.get_v_origin_with_var_unscaled (*پہلا_glyph, font, *scratch, gvar_cache));
	origin_cache->سیٹ (*پہلا_glyph, font->y_scale < 0 ? -origin : origin);
      }

      *پہلا_y = origin;

      پہلا_glyph = &Struct_At_وتھ_Unaligned<hb_codepoint_t> (پہلا_glyph, glyph_stride);
      پہلا_y = &Struct_At_وتھ_Unaligned<hb_position_t> (پہلا_y, y_stride);
    }

#جے_بنا_تعریف HB_NO_VAR
    إذا (gvar_cache)
      ot_font->draw.release_gvar_cache (gvar_cache);
#جے_انت
    glyf.release_scratch (scratch);
    ot_font->v_origin.release_origin_cache (origin_cache);
    أرجع سچ;
  }

  /* Otherwise, use glyph extents to center the glyph vertically.
   * If getting glyph extents failed, just use the font ascender. */
  إذا (origin_cache && font->has_glyph_extents_func ())
  {
    hb_font_extents_t font_extents;
    font->get_h_extents_with_fallback (&font_extents);
    hb_position_t font_advance = font_extents.ascender - font_extents.descender;

    لكل (بنانشانی i = 0; i < گنتی; i++)
    {
      hb_position_t origin;
      بنانشانی cv;

      إذا (origin_cache->get (*پہلا_glyph, &cv))
	origin = font->y_scale < 0 ? -static_cast<hb_position_t>(cv) : static_cast<hb_position_t>(cv);
      وإلا
      {
	hb_glyph_extents_t extents = {0};
	إذا (likely (font->get_glyph_extents (*پہلا_glyph, &extents)))
	  origin = hb_clamp_منزل<hb_position_t> ((int64_t) extents.y_bearing +
					 (((int64_t) font_advance + extents.اچائی) >> 1));
	وإلا
	  origin = font_extents.ascender;

	origin_cache->سیٹ (*پہلا_glyph, font->y_scale < 0 ? -origin : origin);
      }

      *پہلا_y = origin;

      پہلا_glyph = &Struct_At_وتھ_Unaligned<hb_codepoint_t> (پہلا_glyph, glyph_stride);
      پہلا_y = &Struct_At_وتھ_Unaligned<hb_position_t> (پہلا_y, y_stride);
    }
  }

  ot_font->v_origin.release_origin_cache (origin_cache);
  أرجع سچ;
}
#جے_انت

ساكن hb_bool_t
hb_ot_get_glyph_extents (hb_font_t *font,
			 فراغ *font_انکڑے,
			 hb_codepoint_t glyph,
			 hb_glyph_extents_t *extents,
			 فراغ *user_انکڑے HB_بن_ورتیا)
{
  ثابت hb_ot_font_t *ot_font = (ثابت hb_ot_font_t *) font_انکڑے;
  ثابت hb_ot_face_t *ot_face = ot_font->ot_face;

#إذا !defined(HB_NO_OT_FONT_BITMAP) && !defined(HB_NO_COLOR)
  إذا (ot_face->sbix->get_extents (font, glyph, extents)) أرجع سچ;
  إذا (ot_face->CBDT->get_extents (font, glyph, extents)) أرجع سچ;
#جے_انت
#إذا !defined(HB_NO_COLOR) && !defined(HB_NO_PAINT)
  إذا (ot_face->COLR->get_extents (font, glyph, extents)) أرجع سچ;
#جے_انت
#جے_بنا_تعریف HB_NO_VAR_COMPOSITES
  إذا (ot_face->VARC->get_extents (font, glyph, extents)) أرجع سچ;
#جے_انت
  إذا (ot_face->glyf->get_extents (font, glyph, extents)) أرجع سچ;
#جے_بنا_تعریف HB_NO_OT_FONT_CFF
  إذا (ot_face->cff2->get_extents (font, glyph, extents)) أرجع سچ;
  إذا (ot_face->cff1->get_extents (font, glyph, extents)) أرجع سچ;
#جے_انت

  أرجع خطأ;
}

#جے_بنا_تعریف HB_NO_OT_FONT_GLYPH_ناں
ساكن hb_bool_t
hb_ot_get_glyph_name (hb_font_t *font HB_بن_ورتیا,
		      فراغ *font_انکڑے,
		      hb_codepoint_t glyph,
		      حرف *name, بنانشانی صحيح ناپ,
		      فراغ *user_انکڑے HB_بن_ورتیا)
{
  ثابت hb_ot_font_t *ot_font = (ثابت hb_ot_font_t *) font_انکڑے;
  ثابت hb_ot_face_t *ot_face = ot_font->ot_face;

  إذا (ot_face->post->get_glyph_name (glyph, name, ناپ)) أرجع سچ;
#جے_بنا_تعریف HB_NO_OT_FONT_CFF
  إذا (ot_face->cff1->get_glyph_name (glyph, name, ناپ)) أرجع سچ;
#جے_انت
  أرجع خطأ;
}
ساكن hb_bool_t
hb_ot_get_glyph_from_name (hb_font_t *font HB_بن_ورتیا,
			   فراغ *font_انکڑے,
			   ثابت حرف *name, صحيح len,
			   hb_codepoint_t *glyph,
			   فراغ *user_انکڑے HB_بن_ورتیا)
{
  ثابت hb_ot_font_t *ot_font = (ثابت hb_ot_font_t *) font_انکڑے;
  ثابت hb_ot_face_t *ot_face = ot_font->ot_face;

  إذا (ot_face->post->get_glyph_from_name (name, len, glyph)) أرجع سچ;
#جے_بنا_تعریف HB_NO_OT_FONT_CFF
    إذا (ot_face->cff1->get_glyph_from_name (name, len, glyph)) أرجع سچ;
#جے_انت
  أرجع خطأ;
}
#جے_انت

ساكن hb_bool_t
hb_ot_get_font_h_extents (hb_font_t *font,
			  فراغ *font_انکڑے HB_بن_ورتیا,
			  hb_font_extents_t *metrics,
			  فراغ *user_انکڑے HB_بن_ورتیا)
{
  أرجع _hb_ot_metrics_get_position_common (font, HB_OT_METRICS_ٹیگ_HORIZONTAL_ASCENDER, &metrics->ascender) &&
	 _hb_ot_metrics_get_position_common (font, HB_OT_METRICS_ٹیگ_HORIZONTAL_DESCENDER, &metrics->descender) &&
	 _hb_ot_metrics_get_position_common (font, HB_OT_METRICS_ٹیگ_HORIZONTAL_سطر_GAP, &metrics->سطر_gap);
}

#جے_بنا_تعریف HB_NO_VERTICAL
ساكن hb_bool_t
hb_ot_get_font_v_extents (hb_font_t *font,
			  فراغ *font_انکڑے HB_بن_ورتیا,
			  hb_font_extents_t *metrics,
			  فراغ *user_انکڑے HB_بن_ورتیا)
{
  أرجع _hb_ot_metrics_get_position_common (font, HB_OT_METRICS_ٹیگ_VERTICAL_ASCENDER, &metrics->ascender) &&
	 _hb_ot_metrics_get_position_common (font, HB_OT_METRICS_ٹیگ_VERTICAL_DESCENDER, &metrics->descender) &&
	 _hb_ot_metrics_get_position_common (font, HB_OT_METRICS_ٹیگ_VERTICAL_سطر_GAP, &metrics->سطر_gap);
}
#جے_انت

#جے_بنا_تعریف HB_NO_DRAW
ساكن hb_bool_t
hb_ot_draw_glyph_or_fail (hb_font_t *font,
			  فراغ *font_انکڑے HB_بن_ورتیا,
			  hb_codepoint_t glyph,
			  hb_draw_funcs_t *draw_funcs, فراغ *draw_انکڑے,
			  فراغ *user_انکڑے)
{
  ثابت hb_ot_font_t *ot_font = (ثابت hb_ot_font_t *) font_انکڑے;
  hb_draw_session_t draw_session {draw_funcs, draw_انکڑے};

  OT::hb_scalar_cache_t *gvar_cache = خالی_سوچک;
#جے_بنا_تعریف HB_NO_VAR
  إذا (font->عدد_coords)
  {
    ot_font->check_serial (font);
    gvar_cache = ot_font->draw.acquire_gvar_cache (*ot_font->ot_face->gvar);
  }
#جے_انت
  HB_SCOPE_GUARD (ot_font->draw.release_gvar_cache (gvar_cache));

#جے_بنا_تعریف HB_NO_VAR_COMPOSITES
  إذا (font->face->جدول.VARC->get_رستہ (font, glyph, draw_session)) أرجع سچ;
#جے_انت
  // Keep the following in synch with VARC::get_path_at()
  إذا (font->face->جدول.glyf->get_رستہ (font, glyph, draw_session, gvar_cache,
				       &draw_session.get_budget ())) أرجع سچ;

#جے_بنا_تعریف HB_NO_CFF
  إذا (font->face->جدول.cff2->get_رستہ (font, glyph, draw_session,
				       &draw_session.get_budget ())) أرجع سچ;
  إذا (font->face->جدول.cff1->get_رستہ (font, glyph, draw_session,
				       &draw_session.get_budget ())) أرجع سچ;
#جے_انت

  أرجع خطأ;
}
#جے_انت

#جے_بنا_تعریف HB_NO_PAINT
ساكن hb_bool_t
hb_ot_paint_glyph_or_fail (hb_font_t *font,
			   فراغ *font_انکڑے,
			   hb_codepoint_t glyph,
			   hb_paint_funcs_t *paint_funcs, فراغ *paint_انکڑے,
			   بنانشانی صحيح palette,
			   hb_color_t foreground,
			   فراغ *user_انکڑے)
{
#جے_بنا_تعریف HB_NO_COLOR
  إذا (font->face->جدول.COLR->paint_glyph (font, glyph, paint_funcs, paint_انکڑے, palette, foreground)) أرجع سچ;
#جے_بنا_تعریف HB_NO_SVG
  إذا (font->face->جدول.SVG->paint_glyph (font, glyph, paint_funcs, paint_انکڑے)) أرجع سچ;
#جے_انت
#جے_بنا_تعریف HB_NO_OT_FONT_BITMAP
  إذا (font->face->جدول.CBDT->paint_glyph (font, glyph, paint_funcs, paint_انکڑے)) أرجع سچ;
  إذا (font->face->جدول.sbix->paint_glyph (font, glyph, paint_funcs, paint_انکڑے)) أرجع سچ;
#جے_انت
#جے_انت
  أرجع خطأ;
}
#جے_انت

ساكن مضمن فراغ free_static_ot_funcs ();

ساكن بنية hb_ot_font_funcs_lazy_loader_t : hb_font_funcs_lazy_loader_t<hb_ot_font_funcs_lazy_loader_t>
{
  ساكن hb_font_funcs_t *create ()
  {
    hb_font_funcs_t *funcs = hb_font_funcs_create ();

    hb_font_funcs_سیٹ_nominal_glyph_func (funcs, hb_ot_get_nominal_glyph, خالی_سوچک, خالی_سوچک);
    hb_font_funcs_سیٹ_nominal_glyphs_func (funcs, hb_ot_get_nominal_glyphs, خالی_سوچک, خالی_سوچک);
    hb_font_funcs_سیٹ_variation_glyph_func (funcs, hb_ot_get_variation_glyph, خالی_سوچک, خالی_سوچک);

    hb_font_funcs_سیٹ_font_h_extents_func (funcs, hb_ot_get_font_h_extents, خالی_سوچک, خالی_سوچک);
    hb_font_funcs_سیٹ_glyph_h_advances_func (funcs, hb_ot_get_glyph_h_advances, خالی_سوچک, خالی_سوچک);

#جے_بنا_تعریف HB_NO_VERTICAL
    hb_font_funcs_سیٹ_font_v_extents_func (funcs, hb_ot_get_font_v_extents, خالی_سوچک, خالی_سوچک);
    hb_font_funcs_سیٹ_glyph_v_advances_func (funcs, hb_ot_get_glyph_v_advances, خالی_سوچک, خالی_سوچک);
    hb_font_funcs_سیٹ_glyph_v_origins_func (funcs, hb_ot_get_glyph_v_origins, خالی_سوچک, خالی_سوچک);
#جے_انت

#جے_بنا_تعریف HB_NO_DRAW
    hb_font_funcs_سیٹ_draw_glyph_or_fail_func (funcs, hb_ot_draw_glyph_or_fail, خالی_سوچک, خالی_سوچک);
#جے_انت

#جے_بنا_تعریف HB_NO_PAINT
    hb_font_funcs_سیٹ_paint_glyph_or_fail_func (funcs, hb_ot_paint_glyph_or_fail, خالی_سوچک, خالی_سوچک);
#جے_انت

    hb_font_funcs_سیٹ_glyph_extents_func (funcs, hb_ot_get_glyph_extents, خالی_سوچک, خالی_سوچک);
    //hb_font_funcs_set_glyph_contour_point_func (funcs, hb_ot_get_glyph_contour_point, nullptr, nullptr);

#جے_بنا_تعریف HB_NO_OT_FONT_GLYPH_ناں
    hb_font_funcs_سیٹ_glyph_name_func (funcs, hb_ot_get_glyph_name, خالی_سوچک, خالی_سوچک);
    hb_font_funcs_سیٹ_glyph_from_name_func (funcs, hb_ot_get_glyph_from_name, خالی_سوچک, خالی_سوچک);
#جے_انت

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
hb_ot_font_سیٹ_funcs (hb_font_t *font)
{
  hb_ot_font_t *ot_font = _hb_ot_font_create (font);
  إذا (unlikely (!ot_font))
    أرجع;

  hb_font_سیٹ_funcs (font,
		     _hb_ot_get_font_funcs (),
		     ot_font,
		     _hb_ot_font_destroy);
}

#جے_انت
