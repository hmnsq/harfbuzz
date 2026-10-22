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

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_OT_FONT

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot.h"

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-cache.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-font.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-machinery.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-face.hh"

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-cmap-table.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-glyf-table.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-var-gvar-table.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-cff2-table.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-cff1-table.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-hmtx-table.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-post-table.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-stat-table.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-var-varc-table.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-vorg-table.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "OT/Color/CBDT/CBDT.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "OT/Color/COLR/COLR.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "OT/Color/sbix/sbix.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "OT/Color/svg/svg.hh"


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

ਵਰਤੋਂ hb_ot_font_advance_cache_t = hb_cache_t<24, 16>;
static_assert (ਆਕਾਰ (hb_ot_font_advance_cache_t) == 1024, "");

ਵਰਤੋਂ hb_ot_font_origin_cache_t = hb_cache_t<20, 20>;
static_assert (ਆਕਾਰ (hb_ot_font_origin_cache_t) == 1024, "");

ਬਣਤਰ hb_ot_font_t
{
  ਅਚਲ hb_ot_face_t *ot_face;

  ਬਦਲਣਯੋਗ hb_atomic_t<ਪੂਰਨ> cached_serial;
  ਬਦਲਣਯੋਗ hb_atomic_t<ਪੂਰਨ> cached_coords_serial;

  ਬਣਤਰ direction_cache_t
  {
    ਬਦਲਣਯੋਗ hb_atomic_t<hb_ot_font_advance_cache_t *> advance_cache;
    ਬਦਲਣਯੋਗ hb_atomic_t<OT::hb_scalar_cache_t *> varStore_cache;

    ~direction_cache_t ()
    {
      clear ();
    }

    hb_ot_font_advance_cache_t *acquire_advance_cache () ਅਚਲ
    {
    retry:
      ਸਵੈਚਾਲਿਤ *cache = advance_cache.get_acquire ();
      ਜੇ (!cache)
      {
        cache = (hb_ot_font_advance_cache_t *) hb_malloc (ਆਕਾਰ (hb_ot_font_advance_cache_t));
	ਜੇ (!cache)
	  ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
	ਨਵਾਂ (cache) hb_ot_font_advance_cache_t;
	ਵਾਪਸ cache;
      }
      ਜੇ (advance_cache.cmpexch (cache, ਖਾਲੀ_ਸੂਚਕ))
        ਵਾਪਸ cache;
      ਨਹੀਂਤਾਂ
        ਜਾਓ retry;
    }
    ਖਾਲੀ release_advance_cache (hb_ot_font_advance_cache_t *cache) ਅਚਲ
    {
      ਜੇ (!cache)
        ਵਾਪਸ;
      ਜੇ (!advance_cache.cmpexch (ਖਾਲੀ_ਸੂਚਕ, cache))
        hb_free (cache);
    }
    ਖਾਲੀ clear_advance_cache () ਅਚਲ
    {
    retry:
      ਸਵੈਚਾਲਿਤ *cache = advance_cache.get_acquire ();
      ਜੇ (!cache)
	ਵਾਪਸ;
      ਜੇ (advance_cache.cmpexch (cache, ਖਾਲੀ_ਸੂਚਕ))
	hb_free (cache);
      ਨਹੀਂਤਾਂ
        ਜਾਓ retry;
    }

    OT::hb_scalar_cache_t *acquire_varStore_cache (ਅਚਲ OT::ਵਸਤੂ_Variation_Store &varStore) ਅਚਲ
    {
    retry:
      ਸਵੈਚਾਲਿਤ *cache = varStore_cache.get_acquire ();
      ਜੇ (!cache)
	ਵਾਪਸ varStore.create_cache ();
      ਜੇ (varStore_cache.cmpexch (cache, ਖਾਲੀ_ਸੂਚਕ))
	ਵਾਪਸ cache;
      ਨਹੀਂਤਾਂ
	ਜਾਓ retry;
    }
    ਖਾਲੀ release_varStore_cache (OT::hb_scalar_cache_t *cache) ਅਚਲ
    {
      ਜੇ (!cache)
	ਵਾਪਸ;
      ਜੇ (!varStore_cache.cmpexch (ਖਾਲੀ_ਸੂਚਕ, cache))
	OT::ਵਸਤੂ_Variation_Store::destroy_cache (cache);
    }
    ਖਾਲੀ clear_varStore_cache () ਅਚਲ
    {
    retry:
      ਸਵੈਚਾਲਿਤ *cache = varStore_cache.get_acquire ();
      ਜੇ (!cache)
	ਵਾਪਸ;
      ਜੇ (varStore_cache.cmpexch (cache, ਖਾਲੀ_ਸੂਚਕ))
	OT::ਵਸਤੂ_Variation_Store::destroy_cache (cache);
      ਨਹੀਂਤਾਂ
	ਜਾਓ retry;
    }

    ਖਾਲੀ clear () ਅਚਲ
    {
      clear_advance_cache ();
      clear_varStore_cache ();
    }

  } h, v;

  ਬਣਤਰ origin_cache_t
  {
    ਬਦਲਣਯੋਗ hb_atomic_t<hb_ot_font_origin_cache_t *> origin_cache;
    ਬਦਲਣਯੋਗ hb_atomic_t<OT::hb_scalar_cache_t *> varStore_cache;

    ~origin_cache_t ()
    {
      clear ();
    }

    hb_ot_font_origin_cache_t *acquire_origin_cache () ਅਚਲ
    {
    retry:
      ਸਵੈਚਾਲਿਤ *cache = origin_cache.get_acquire ();
      ਜੇ (!cache)
      {
        cache = (hb_ot_font_origin_cache_t *) hb_malloc (ਆਕਾਰ (hb_ot_font_origin_cache_t));
	ਜੇ (!cache)
	  ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
	ਨਵਾਂ (cache) hb_ot_font_origin_cache_t;
	ਵਾਪਸ cache;
      }
      ਜੇ (origin_cache.cmpexch (cache, ਖਾਲੀ_ਸੂਚਕ))
        ਵਾਪਸ cache;
      ਨਹੀਂਤਾਂ
        ਜਾਓ retry;
    }
    ਖਾਲੀ release_origin_cache (hb_ot_font_origin_cache_t *cache) ਅਚਲ
    {
      ਜੇ (!cache)
        ਵਾਪਸ;
      ਜੇ (!origin_cache.cmpexch (ਖਾਲੀ_ਸੂਚਕ, cache))
        hb_free (cache);
    }
    ਖਾਲੀ clear_origin_cache () ਅਚਲ
    {
    retry:
      ਸਵੈਚਾਲਿਤ *cache = origin_cache.get_acquire ();
      ਜੇ (!cache)
	ਵਾਪਸ;
      ਜੇ (origin_cache.cmpexch (cache, ਖਾਲੀ_ਸੂਚਕ))
	hb_free (cache);
      ਨਹੀਂਤਾਂ
        ਜਾਓ retry;
    }

    OT::hb_scalar_cache_t *acquire_varStore_cache (ਅਚਲ OT::ਵਸਤੂ_Variation_Store &varStore) ਅਚਲ
    {
    retry:
      ਸਵੈਚਾਲਿਤ *cache = varStore_cache.get_acquire ();
      ਜੇ (!cache)
	ਵਾਪਸ varStore.create_cache ();
      ਜੇ (varStore_cache.cmpexch (cache, ਖਾਲੀ_ਸੂਚਕ))
	ਵਾਪਸ cache;
      ਨਹੀਂਤਾਂ
	ਜਾਓ retry;
    }
    ਖਾਲੀ release_varStore_cache (OT::hb_scalar_cache_t *cache) ਅਚਲ
    {
      ਜੇ (!cache)
	ਵਾਪਸ;
      ਜੇ (!varStore_cache.cmpexch (ਖਾਲੀ_ਸੂਚਕ, cache))
	OT::ਵਸਤੂ_Variation_Store::destroy_cache (cache);
    }
    ਖਾਲੀ clear_varStore_cache () ਅਚਲ
    {
    retry:
      ਸਵੈਚਾਲਿਤ *cache = varStore_cache.get_acquire ();
      ਜੇ (!cache)
	ਵਾਪਸ;
      ਜੇ (varStore_cache.cmpexch (cache, ਖਾਲੀ_ਸੂਚਕ))
	OT::ਵਸਤੂ_Variation_Store::destroy_cache (cache);
      ਨਹੀਂਤਾਂ
	ਜਾਓ retry;
    }

    ਖਾਲੀ clear () ਅਚਲ
    {
      clear_origin_cache ();
      clear_varStore_cache ();
    }
  } v_origin;

  ਬਣਤਰ draw_cache_t
  {
    ਬਦਲਣਯੋਗ hb_atomic_t<OT::hb_scalar_cache_t *> gvar_cache;

    ~draw_cache_t ()
    {
      clear ();
    }

    OT::hb_scalar_cache_t *acquire_gvar_cache (ਅਚਲ OT::gvar_accelerator_t &gvar) ਅਚਲ
    {
    retry:
      ਸਵੈਚਾਲਿਤ *cache = gvar_cache.get_acquire ();
      ਜੇ (!cache)
	ਵਾਪਸ gvar.create_cache ();
      ਜੇ (gvar_cache.cmpexch (cache, ਖਾਲੀ_ਸੂਚਕ))
	ਵਾਪਸ cache;
      ਨਹੀਂਤਾਂ
	ਜਾਓ retry;
    }
    ਖਾਲੀ release_gvar_cache (OT::hb_scalar_cache_t *cache) ਅਚਲ
    {
      ਜੇ (!cache)
	ਵਾਪਸ;
      ਜੇ (!gvar_cache.cmpexch (ਖਾਲੀ_ਸੂਚਕ, cache))
	OT::gvar_accelerator_t::destroy_cache (cache);
    }
    ਖਾਲੀ clear_gvar_cache () ਅਚਲ
    {
    retry:
      ਸਵੈਚਾਲਿਤ *cache = gvar_cache.get_acquire ();
      ਜੇ (!cache)
	ਵਾਪਸ;
      ਜੇ (gvar_cache.cmpexch (cache, ਖਾਲੀ_ਸੂਚਕ))
	OT::gvar_accelerator_t::destroy_cache (cache);
      ਨਹੀਂਤਾਂ
	ਜਾਓ retry;
    }

    ਖਾਲੀ clear () ਅਚਲ
    {
      clear_gvar_cache ();
    }
  } draw;

  ਖਾਲੀ check_serial (hb_font_t *font) ਅਚਲ
  {
    ਪੂਰਨ font_serial = font->serial.get_acquire ();
    ਜੇ (cached_serial.get_acquire () != font_serial)
    {
      /* These caches are dependent on scale and synthetic settings.
       * Any change to the font invalidates them. */
      v_origin.clear ();

      cached_serial.ਸੈੱਟ_release (font_serial);
    }

    ਪੂਰਨ font_serial_coords = font->serial_coords.get_acquire ();
    ਜੇ (cached_coords_serial.get_acquire () != font_serial_coords)
    {
      /* These caches are independent of scale or synthetic settings.
       * Just variation changes will invalidate them. */
      h.clear ();
      v.clear ();
      draw.clear ();

      cached_coords_serial.ਸੈੱਟ_release (font_serial_coords);
    }
  }
};

ਸਥਿਰ hb_ot_font_t *
_hb_ot_font_create (hb_font_t *font)
{
  hb_ot_font_t *ot_font = (hb_ot_font_t *) hb_calloc (1, ਆਕਾਰ (hb_ot_font_t));
  ਜੇ (unlikely (!ot_font))
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

  ot_font->ot_face = &font->face->ਸਾਰਣੀ;

  ਵਾਪਸ ot_font;
}

ਸਥਿਰ ਖਾਲੀ
_hb_ot_font_destroy (ਖਾਲੀ *font_ਅੰਕੜੇ)
{
  hb_ot_font_t *ot_font = (hb_ot_font_t *) font_ਅੰਕੜੇ;

  ot_font->~hb_ot_font_t ();

  hb_free (ot_font);
}

ਸਥਿਰ hb_bool_t
hb_ot_get_nominal_glyph (hb_font_t *font HB_ਅਣਵਰਤਿਆ,
			 ਖਾਲੀ *font_ਅੰਕੜੇ,
			 hb_codepoint_t unicode,
			 hb_codepoint_t *glyph,
			 ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  ਅਚਲ hb_ot_font_t *ot_font = (ਅਚਲ hb_ot_font_t *) font_ਅੰਕੜੇ;
  ਅਚਲ hb_ot_face_t *ot_face = ot_font->ot_face;
  ਵਾਪਸ ot_face->cmap->get_nominal_glyph (unicode, glyph);
}

ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ
hb_ot_get_nominal_glyphs (hb_font_t *font HB_ਅਣਵਰਤਿਆ,
			  ਖਾਲੀ *font_ਅੰਕੜੇ,
			  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2,
			  ਅਚਲ hb_codepoint_t *ਪਹਿਲਾ_unicode,
			  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ unicode_stride,
			  hb_codepoint_t *ਪਹਿਲਾ_glyph,
			  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ glyph_stride,
			  ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  ਅਚਲ hb_ot_font_t *ot_font = (ਅਚਲ hb_ot_font_t *) font_ਅੰਕੜੇ;
  ਅਚਲ hb_ot_face_t *ot_face = ot_font->ot_face;
  ਵਾਪਸ ot_face->cmap->get_nominal_glyphs (ਗਿਣਤੀ_2,
					    ਪਹਿਲਾ_unicode, unicode_stride,
					    ਪਹਿਲਾ_glyph, glyph_stride);
}

ਸਥਿਰ hb_bool_t
hb_ot_get_variation_glyph (hb_font_t *font HB_ਅਣਵਰਤਿਆ,
			   ਖਾਲੀ *font_ਅੰਕੜੇ,
			   hb_codepoint_t unicode,
			   hb_codepoint_t variation_selector,
			   hb_codepoint_t *glyph,
			   ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  ਅਚਲ hb_ot_font_t *ot_font = (ਅਚਲ hb_ot_font_t *) font_ਅੰਕੜੇ;
  ਅਚਲ hb_ot_face_t *ot_face = ot_font->ot_face;
  ਵਾਪਸ ot_face->cmap->get_variation_glyph (unicode,
                                             variation_selector, glyph);
}

ਸਥਿਰ ਖਾਲੀ
hb_ot_get_glyph_h_advances (hb_font_t* font, ਖਾਲੀ* font_ਅੰਕੜੇ,
			    ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2,
			    ਅਚਲ hb_codepoint_t *ਪਹਿਲਾ_glyph,
			    ਬਿਨਾਨਿਸ਼ਾਨੀ glyph_stride,
			    hb_position_t *ਪਹਿਲਾ_advance,
			    ਬਿਨਾਨਿਸ਼ਾਨੀ advance_stride,
			    ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  // Duplicated in v_advances. Ugly. Keep in sync'ish.

  ਅਚਲ hb_ot_font_t *ot_font = (ਅਚਲ hb_ot_font_t *) font_ਅੰਕੜੇ;
  ਅਚਲ hb_ot_face_t *ot_face = ot_font->ot_face;
  ਅਚਲ OT::hmtx_accelerator_t &hmtx = *ot_face->hmtx;

  ਜੇ (unlikely (!hmtx.has_ਅੰਕੜੇ ()))
  {
    hb_position_t advance = font->face->get_upem () / 2;
    advance = font->em_scale_x (advance);
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++)
    {
      *ਪਹਿਲਾ_advance = advance;
      ਪਹਿਲਾ_advance = &Struct_At_ਵਿੱਥ_Unaligned<hb_position_t> (ਪਹਿਲਾ_advance, advance_stride);
    }
    ਵਾਪਸ;
  }

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_VAR
  ਜੇ (!font->has_nonzero_coords)
  {
  fallback:
#ਨਹੀਂਤਾਂ
  {
#ਜੇ_ਅੰਤ
    // Just plain htmx data. No need to cache.
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++)
    {
      *ਪਹਿਲਾ_advance = font->em_scale_x (hmtx.get_advance_without_var_unscaled (*ਪਹਿਲਾ_glyph));
      ਪਹਿਲਾ_glyph = &Struct_At_ਵਿੱਥ_Unaligned<hb_codepoint_t> (ਪਹਿਲਾ_glyph, glyph_stride);
      ਪਹਿਲਾ_advance = &Struct_At_ਵਿੱਥ_Unaligned<hb_position_t> (ਪਹਿਲਾ_advance, advance_stride);
    }
    ਵਾਪਸ;
  }

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_VAR
  /* has_nonzero_coords. */

  ot_font->check_serial (font);
  hb_ot_font_advance_cache_t *advance_cache = ot_font->h.acquire_advance_cache ();
  ਜੇ (!advance_cache)
  {
    // malloc failure. Just use the fallback non-variable path.
    ਜਾਓ fallback;
  }

  /* If HVAR is present, use it.*/
  ਅਚਲ OT::HVAR &HVAR = *hmtx.var_ਸਾਰਣੀ;
  ਜੇ (HVAR.has_ਅੰਕੜੇ ())
  {
    ਅਚਲ OT::ਵਸਤੂ_Variation_Store &varStore = &HVAR + HVAR.varStore;
    OT::hb_scalar_cache_t *varStore_cache = ot_font->h.acquire_varStore_cache (varStore);

    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++)
    {
      hb_position_t v;
      ਬਿਨਾਨਿਸ਼ਾਨੀ cv;
      ਜੇ (advance_cache->get (*ਪਹਿਲਾ_glyph, &cv))
	v = cv;
      ਨਹੀਂਤਾਂ
      {
        v = hmtx.get_advance_with_var_unscaled (*ਪਹਿਲਾ_glyph, font, varStore_cache);
	advance_cache->ਸੈੱਟ (*ਪਹਿਲਾ_glyph, v);
      }
      *ਪਹਿਲਾ_advance = font->em_scale_x (v);
      ਪਹਿਲਾ_glyph = &Struct_At_ਵਿੱਥ_Unaligned<hb_codepoint_t> (ਪਹਿਲਾ_glyph, glyph_stride);
      ਪਹਿਲਾ_advance = &Struct_At_ਵਿੱਥ_Unaligned<hb_position_t> (ਪਹਿਲਾ_advance, advance_stride);
    }

    ot_font->h.release_varStore_cache (varStore_cache);
    ot_font->h.release_advance_cache (advance_cache);
    ਵਾਪਸ;
  }

  ਅਚਲ ਸਵੈਚਾਲਿਤ &gvar = *ot_face->gvar;
  ਜੇ (gvar.has_ਅੰਕੜੇ ())
  {
    ਅਚਲ ਸਵੈਚਾਲਿਤ &glyf = *ot_face->glyf;
    ਸਵੈਚਾਲਿਤ *scratch = glyf.acquire_scratch ();
    ਜੇ (unlikely (!scratch))
    {
      ot_font->h.release_advance_cache (advance_cache);
      ਜਾਓ fallback;
    }
    OT::hb_scalar_cache_t *gvar_cache = ot_font->draw.acquire_gvar_cache (gvar);

    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++)
    {
      hb_position_t v;
      ਬਿਨਾਨਿਸ਼ਾਨੀ cv;
      ਜੇ (advance_cache->get (*ਪਹਿਲਾ_glyph, &cv))
	v = cv;
      ਨਹੀਂਤਾਂ
      {
        v = glyf.get_advance_with_var_unscaled (*ਪਹਿਲਾ_glyph, font, ਝੂਠ, *scratch, gvar_cache);
	advance_cache->ਸੈੱਟ (*ਪਹਿਲਾ_glyph, v);
      }
      *ਪਹਿਲਾ_advance = font->em_scale_x (v);
      ਪਹਿਲਾ_glyph = &Struct_At_ਵਿੱਥ_Unaligned<hb_codepoint_t> (ਪਹਿਲਾ_glyph, glyph_stride);
      ਪਹਿਲਾ_advance = &Struct_At_ਵਿੱਥ_Unaligned<hb_position_t> (ਪਹਿਲਾ_advance, advance_stride);
    }

    ot_font->draw.release_gvar_cache (gvar_cache);
    glyf.release_scratch (scratch);
    ot_font->h.release_advance_cache (advance_cache);
    ਵਾਪਸ;
  }

  ot_font->h.release_advance_cache (advance_cache);
  // No HVAR or GVAR.  Just use the fallback non-variable path.
  ਜਾਓ fallback;
#ਜੇ_ਅੰਤ
}

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_VERTICAL
ਸਥਿਰ ਖਾਲੀ
hb_ot_get_glyph_v_advances (hb_font_t* font, ਖਾਲੀ* font_ਅੰਕੜੇ,
			    ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2,
			    ਅਚਲ hb_codepoint_t *ਪਹਿਲਾ_glyph,
			    ਬਿਨਾਨਿਸ਼ਾਨੀ glyph_stride,
			    hb_position_t *ਪਹਿਲਾ_advance,
			    ਬਿਨਾਨਿਸ਼ਾਨੀ advance_stride,
			    ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  // Duplicated from h_advances. Ugly. Keep in sync'ish.

  ਅਚਲ hb_ot_font_t *ot_font = (ਅਚਲ hb_ot_font_t *) font_ਅੰਕੜੇ;
  ਅਚਲ hb_ot_face_t *ot_face = ot_font->ot_face;
  ਅਚਲ OT::vmtx_accelerator_t &vmtx = *ot_face->vmtx;

  ਜੇ (unlikely (!vmtx.has_ਅੰਕੜੇ ()))
  {
    hb_font_extents_t font_extents;
    font->get_h_extents_with_fallback (&font_extents);
    hb_position_t advance = font_extents.descender - font_extents.ascender;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++)
    {
      *ਪਹਿਲਾ_advance = advance;
      ਪਹਿਲਾ_advance = &Struct_At_ਵਿੱਥ_Unaligned<hb_position_t> (ਪਹਿਲਾ_advance, advance_stride);
    }
    ਵਾਪਸ;
  }

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_VAR
  ਜੇ (!font->has_nonzero_coords)
  {
  fallback:
#ਨਹੀਂਤਾਂ
  {
#ਜੇ_ਅੰਤ
    // Just plain vtmx data. No need to cache.
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++)
    {
      *ਪਹਿਲਾ_advance = font->em_scale_y (- (ਪੂਰਨ) vmtx.get_advance_without_var_unscaled (*ਪਹਿਲਾ_glyph));
      ਪਹਿਲਾ_glyph = &Struct_At_ਵਿੱਥ_Unaligned<hb_codepoint_t> (ਪਹਿਲਾ_glyph, glyph_stride);
      ਪਹਿਲਾ_advance = &Struct_At_ਵਿੱਥ_Unaligned<hb_position_t> (ਪਹਿਲਾ_advance, advance_stride);
    }
    ਵਾਪਸ;
  }

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_VAR
  /* has_nonzero_coords. */

  ot_font->check_serial (font);
  hb_ot_font_advance_cache_t *advance_cache = ot_font->v.acquire_advance_cache ();
  ਜੇ (!advance_cache)
  {
    // malloc failure. Just use the fallback non-variable path.
    ਜਾਓ fallback;
  }

  /* If VVAR is present, use it.*/
  ਅਚਲ OT::VVAR &VVAR = *vmtx.var_ਸਾਰਣੀ;
  ਜੇ (VVAR.has_ਅੰਕੜੇ ())
  {
    ਅਚਲ OT::ਵਸਤੂ_Variation_Store &varStore = &VVAR + VVAR.varStore;
    OT::hb_scalar_cache_t *varStore_cache = ot_font->v.acquire_varStore_cache (varStore);

    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++)
    {
      hb_position_t v;
      ਬਿਨਾਨਿਸ਼ਾਨੀ cv;
      ਜੇ (advance_cache->get (*ਪਹਿਲਾ_glyph, &cv))
	v = cv;
      ਨਹੀਂਤਾਂ
      {
        v = vmtx.get_advance_with_var_unscaled (*ਪਹਿਲਾ_glyph, font, varStore_cache);
	advance_cache->ਸੈੱਟ (*ਪਹਿਲਾ_glyph, v);
      }
      *ਪਹਿਲਾ_advance = font->em_scale_y (- (ਪੂਰਨ) v);
      ਪਹਿਲਾ_glyph = &Struct_At_ਵਿੱਥ_Unaligned<hb_codepoint_t> (ਪਹਿਲਾ_glyph, glyph_stride);
      ਪਹਿਲਾ_advance = &Struct_At_ਵਿੱਥ_Unaligned<hb_position_t> (ਪਹਿਲਾ_advance, advance_stride);
    }

    ot_font->v.release_varStore_cache (varStore_cache);
    ot_font->v.release_advance_cache (advance_cache);
    ਵਾਪਸ;
  }

  ਅਚਲ ਸਵੈਚਾਲਿਤ &gvar = *ot_face->gvar;
  ਜੇ (gvar.has_ਅੰਕੜੇ ())
  {
    ਅਚਲ ਸਵੈਚਾਲਿਤ &glyf = *ot_face->glyf;
    ਸਵੈਚਾਲਿਤ *scratch = glyf.acquire_scratch ();
    ਜੇ (unlikely (!scratch))
    {
      ot_font->v.release_advance_cache (advance_cache);
      ਜਾਓ fallback;
    }
    OT::hb_scalar_cache_t *gvar_cache = ot_font->draw.acquire_gvar_cache (gvar);

    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++)
    {
      hb_position_t v;
      ਬਿਨਾਨਿਸ਼ਾਨੀ cv;
      ਜੇ (advance_cache->get (*ਪਹਿਲਾ_glyph, &cv))
	v = cv;
      ਨਹੀਂਤਾਂ
      {
        v = glyf.get_advance_with_var_unscaled (*ਪਹਿਲਾ_glyph, font, ਸੱਚ, *scratch, gvar_cache);
	advance_cache->ਸੈੱਟ (*ਪਹਿਲਾ_glyph, v);
      }
      *ਪਹਿਲਾ_advance = font->em_scale_y (- (ਪੂਰਨ) v);
      ਪਹਿਲਾ_glyph = &Struct_At_ਵਿੱਥ_Unaligned<hb_codepoint_t> (ਪਹਿਲਾ_glyph, glyph_stride);
      ਪਹਿਲਾ_advance = &Struct_At_ਵਿੱਥ_Unaligned<hb_position_t> (ਪਹਿਲਾ_advance, advance_stride);
    }

    ot_font->draw.release_gvar_cache (gvar_cache);
    glyf.release_scratch (scratch);
    ot_font->v.release_advance_cache (advance_cache);
    ਵਾਪਸ;
  }

  ot_font->v.release_advance_cache (advance_cache);
  // No VVAR or GVAR.  Just use the fallback non-variable path.
  ਜਾਓ fallback;
#ਜੇ_ਅੰਤ
}
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_VERTICAL
HB_HOT
ਸਥਿਰ hb_bool_t
hb_ot_get_glyph_v_origins (hb_font_t *font,
			   ਖਾਲੀ *font_ਅੰਕੜੇ,
			   ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2,
			   ਅਚਲ hb_codepoint_t *ਪਹਿਲਾ_glyph,
			   ਬਿਨਾਨਿਸ਼ਾਨੀ glyph_stride,
			   hb_position_t *ਪਹਿਲਾ_x,
			   ਬਿਨਾਨਿਸ਼ਾਨੀ x_stride,
			   hb_position_t *ਪਹਿਲਾ_y,
			   ਬਿਨਾਨਿਸ਼ਾਨੀ y_stride,
			   ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  ਅਚਲ hb_ot_font_t *ot_font = (ਅਚਲ hb_ot_font_t *) font_ਅੰਕੜੇ;
  ਅਚਲ hb_ot_face_t *ot_face = ot_font->ot_face;

  ot_font->check_serial (font);

  /* First, set all the x values to half the advance width. */
  font->get_glyph_h_advances (ਗਿਣਤੀ_2,
			      ਪਹਿਲਾ_glyph, glyph_stride,
			      ਪਹਿਲਾ_x, x_stride);
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਗਿਣਤੀ_2; i++)
  {
    *ਪਹਿਲਾ_x /= 2;
    ਪਹਿਲਾ_x = &Struct_At_ਵਿੱਥ_Unaligned<hb_position_t> (ਪਹਿਲਾ_x, x_stride);
  }

  /* The vertical origin business is messy...
   *
   * We allocate the cache, then have various code paths that use the cache.
   * Each one is responsible to free it before returning.
   */
  hb_ot_font_origin_cache_t *origin_cache = ot_font->v_origin.acquire_origin_cache ();

  /* If there is VORG, always use it. It uses VVAR for variations if necessary. */
  ਅਚਲ OT::VORG &VORG = *ot_face->VORG;
  ਜੇ (origin_cache && VORG.has_ਅੰਕੜੇ ())
  {
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_VAR
    ਜੇ (!font->has_nonzero_coords)
#ਜੇ_ਅੰਤ
    {
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਗਿਣਤੀ_2; i++)
      {
	hb_position_t origin;
	ਬਿਨਾਨਿਸ਼ਾਨੀ cv;
	ਜੇ (origin_cache->get (*ਪਹਿਲਾ_glyph, &cv))
	  origin = font->y_scale < 0 ? -static_cast<hb_position_t>(cv) : static_cast<hb_position_t>(cv);
	ਨਹੀਂਤਾਂ
	{
	  origin = font->em_scalef_y (VORG.get_y_origin (*ਪਹਿਲਾ_glyph));
	  origin_cache->ਸੈੱਟ (*ਪਹਿਲਾ_glyph, font->y_scale < 0 ? -origin : origin);
	}

	*ਪਹਿਲਾ_y = origin;

	ਪਹਿਲਾ_glyph = &Struct_At_ਵਿੱਥ_Unaligned<hb_codepoint_t> (ਪਹਿਲਾ_glyph, glyph_stride);
	ਪਹਿਲਾ_y = &Struct_At_ਵਿੱਥ_Unaligned<hb_position_t> (ਪਹਿਲਾ_y, y_stride);
      }
    }
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_VAR
    ਨਹੀਂਤਾਂ
    {
      ਅਚਲ OT::VVAR &VVAR = *ot_face->vmtx->var_ਸਾਰਣੀ;
      ਅਚਲ ਸਵੈਚਾਲਿਤ &varStore = &VVAR + VVAR.varStore;
      ਸਵੈਚਾਲਿਤ *varStore_cache = ot_font->v_origin.acquire_varStore_cache (varStore);
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਗਿਣਤੀ_2; i++)
      {
	hb_position_t origin;
	ਬਿਨਾਨਿਸ਼ਾਨੀ cv;
	ਜੇ (origin_cache->get (*ਪਹਿਲਾ_glyph, &cv))
	  origin = font->y_scale < 0 ? -static_cast<hb_position_t>(cv) : static_cast<hb_position_t>(cv);
	ਨਹੀਂਤਾਂ
	{
	  origin = font->em_scalef_y (VORG.get_y_origin (*ਪਹਿਲਾ_glyph) +
				      VVAR.get_vorg_delta_unscaled (*ਪਹਿਲਾ_glyph,
								    font->coords, font->ਅੰਕ_coords,
								    varStore_cache));
	  origin_cache->ਸੈੱਟ (*ਪਹਿਲਾ_glyph, font->y_scale < 0 ? -origin : origin);
	}

	*ਪਹਿਲਾ_y = origin;

	ਪਹਿਲਾ_glyph = &Struct_At_ਵਿੱਥ_Unaligned<hb_codepoint_t> (ਪਹਿਲਾ_glyph, glyph_stride);
	ਪਹਿਲਾ_y = &Struct_At_ਵਿੱਥ_Unaligned<hb_position_t> (ਪਹਿਲਾ_y, y_stride);
      }
      ot_font->v_origin.release_varStore_cache (varStore_cache);
    }
#ਜੇ_ਅੰਤ
    ot_font->v_origin.release_origin_cache (origin_cache);
    ਵਾਪਸ ਸੱਚ;
  }

  /* If and only if `vmtx` is present and it's a `glyf` font,
   * we use the top phantom point, deduced from vmtx,glyf[,gvar]. */
  ਅਚਲ ਸਵੈਚਾਲਿਤ &vmtx = *ot_face->vmtx;
  ਅਚਲ ਸਵੈਚਾਲਿਤ &glyf = *ot_face->glyf;
  ਜੇ (origin_cache && vmtx.has_ਅੰਕੜੇ() && glyf.has_ਅੰਕੜੇ ())
  {
    ਸਵੈਚਾਲਿਤ *scratch = glyf.acquire_scratch ();
    ਜੇ (unlikely (!scratch))
    {
      ot_font->v_origin.release_origin_cache (origin_cache);
      ਵਾਪਸ ਝੂਠ;
    }
    OT::hb_scalar_cache_t *gvar_cache = ਖਾਲੀ_ਸੂਚਕ;
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_VAR
    gvar_cache = font->has_nonzero_coords ?
					ot_font->draw.acquire_gvar_cache (*ot_face->gvar) :
					ਖਾਲੀ_ਸੂਚਕ;
#ਜੇ_ਅੰਤ

    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਗਿਣਤੀ_2; i++)
    {
      hb_position_t origin;
      ਬਿਨਾਨਿਸ਼ਾਨੀ cv;
      ਜੇ (origin_cache->get (*ਪਹਿਲਾ_glyph, &cv))
	origin = font->y_scale < 0 ? -static_cast<hb_position_t>(cv) : static_cast<hb_position_t>(cv);
      ਨਹੀਂਤਾਂ
      {
	origin = font->em_scalef_y (glyf.get_v_origin_with_var_unscaled (*ਪਹਿਲਾ_glyph, font, *scratch, gvar_cache));
	origin_cache->ਸੈੱਟ (*ਪਹਿਲਾ_glyph, font->y_scale < 0 ? -origin : origin);
      }

      *ਪਹਿਲਾ_y = origin;

      ਪਹਿਲਾ_glyph = &Struct_At_ਵਿੱਥ_Unaligned<hb_codepoint_t> (ਪਹਿਲਾ_glyph, glyph_stride);
      ਪਹਿਲਾ_y = &Struct_At_ਵਿੱਥ_Unaligned<hb_position_t> (ਪਹਿਲਾ_y, y_stride);
    }

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_VAR
    ਜੇ (gvar_cache)
      ot_font->draw.release_gvar_cache (gvar_cache);
#ਜੇ_ਅੰਤ
    glyf.release_scratch (scratch);
    ot_font->v_origin.release_origin_cache (origin_cache);
    ਵਾਪਸ ਸੱਚ;
  }

  /* Otherwise, use glyph extents to center the glyph vertically.
   * If getting glyph extents failed, just use the font ascender. */
  ਜੇ (origin_cache && font->has_glyph_extents_func ())
  {
    hb_font_extents_t font_extents;
    font->get_h_extents_with_fallback (&font_extents);
    hb_position_t font_advance = font_extents.ascender - font_extents.descender;

    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਗਿਣਤੀ_2; i++)
    {
      hb_position_t origin;
      ਬਿਨਾਨਿਸ਼ਾਨੀ cv;

      ਜੇ (origin_cache->get (*ਪਹਿਲਾ_glyph, &cv))
	origin = font->y_scale < 0 ? -static_cast<hb_position_t>(cv) : static_cast<hb_position_t>(cv);
      ਨਹੀਂਤਾਂ
      {
	hb_glyph_extents_t extents = {0};
	ਜੇ (likely (font->get_glyph_extents (*ਪਹਿਲਾ_glyph, &extents)))
	  origin = hb_clamp_ਮੰਜ਼ਿਲ<hb_position_t> ((int64_t) extents.y_bearing +
					 (((int64_t) font_advance + extents.ਉਚਾਈ) >> 1));
	ਨਹੀਂਤਾਂ
	  origin = font_extents.ascender;

	origin_cache->ਸੈੱਟ (*ਪਹਿਲਾ_glyph, font->y_scale < 0 ? -origin : origin);
      }

      *ਪਹਿਲਾ_y = origin;

      ਪਹਿਲਾ_glyph = &Struct_At_ਵਿੱਥ_Unaligned<hb_codepoint_t> (ਪਹਿਲਾ_glyph, glyph_stride);
      ਪਹਿਲਾ_y = &Struct_At_ਵਿੱਥ_Unaligned<hb_position_t> (ਪਹਿਲਾ_y, y_stride);
    }
  }

  ot_font->v_origin.release_origin_cache (origin_cache);
  ਵਾਪਸ ਸੱਚ;
}
#ਜੇ_ਅੰਤ

ਸਥਿਰ hb_bool_t
hb_ot_get_glyph_extents (hb_font_t *font,
			 ਖਾਲੀ *font_ਅੰਕੜੇ,
			 hb_codepoint_t glyph,
			 hb_glyph_extents_t *extents,
			 ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  ਅਚਲ hb_ot_font_t *ot_font = (ਅਚਲ hb_ot_font_t *) font_ਅੰਕੜੇ;
  ਅਚਲ hb_ot_face_t *ot_face = ot_font->ot_face;

#ਜੇ !defined(HB_NO_OT_FONT_BITMAP) && !defined(HB_NO_COLOR)
  ਜੇ (ot_face->sbix->get_extents (font, glyph, extents)) ਵਾਪਸ ਸੱਚ;
  ਜੇ (ot_face->CBDT->get_extents (font, glyph, extents)) ਵਾਪਸ ਸੱਚ;
#ਜੇ_ਅੰਤ
#ਜੇ !defined(HB_NO_COLOR) && !defined(HB_NO_PAINT)
  ਜੇ (ot_face->COLR->get_extents (font, glyph, extents)) ਵਾਪਸ ਸੱਚ;
#ਜੇ_ਅੰਤ
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_VAR_COMPOSITES
  ਜੇ (ot_face->VARC->get_extents (font, glyph, extents)) ਵਾਪਸ ਸੱਚ;
#ਜੇ_ਅੰਤ
  ਜੇ (ot_face->glyf->get_extents (font, glyph, extents)) ਵਾਪਸ ਸੱਚ;
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_OT_FONT_CFF
  ਜੇ (ot_face->cff2->get_extents (font, glyph, extents)) ਵਾਪਸ ਸੱਚ;
  ਜੇ (ot_face->cff1->get_extents (font, glyph, extents)) ਵਾਪਸ ਸੱਚ;
#ਜੇ_ਅੰਤ

  ਵਾਪਸ ਝੂਠ;
}

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_OT_FONT_GLYPH_ਨਾਂ
ਸਥਿਰ hb_bool_t
hb_ot_get_glyph_name (hb_font_t *font HB_ਅਣਵਰਤਿਆ,
		      ਖਾਲੀ *font_ਅੰਕੜੇ,
		      hb_codepoint_t glyph,
		      ਅੱਖਰ *name, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਮਾਪ,
		      ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  ਅਚਲ hb_ot_font_t *ot_font = (ਅਚਲ hb_ot_font_t *) font_ਅੰਕੜੇ;
  ਅਚਲ hb_ot_face_t *ot_face = ot_font->ot_face;

  ਜੇ (ot_face->post->get_glyph_name (glyph, name, ਮਾਪ)) ਵਾਪਸ ਸੱਚ;
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_OT_FONT_CFF
  ਜੇ (ot_face->cff1->get_glyph_name (glyph, name, ਮਾਪ)) ਵਾਪਸ ਸੱਚ;
#ਜੇ_ਅੰਤ
  ਵਾਪਸ ਝੂਠ;
}
ਸਥਿਰ hb_bool_t
hb_ot_get_glyph_from_name (hb_font_t *font HB_ਅਣਵਰਤਿਆ,
			   ਖਾਲੀ *font_ਅੰਕੜੇ,
			   ਅਚਲ ਅੱਖਰ *name, ਪੂਰਨ len,
			   hb_codepoint_t *glyph,
			   ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  ਅਚਲ hb_ot_font_t *ot_font = (ਅਚਲ hb_ot_font_t *) font_ਅੰਕੜੇ;
  ਅਚਲ hb_ot_face_t *ot_face = ot_font->ot_face;

  ਜੇ (ot_face->post->get_glyph_from_name (name, len, glyph)) ਵਾਪਸ ਸੱਚ;
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_OT_FONT_CFF
    ਜੇ (ot_face->cff1->get_glyph_from_name (name, len, glyph)) ਵਾਪਸ ਸੱਚ;
#ਜੇ_ਅੰਤ
  ਵਾਪਸ ਝੂਠ;
}
#ਜੇ_ਅੰਤ

ਸਥਿਰ hb_bool_t
hb_ot_get_font_h_extents (hb_font_t *font,
			  ਖਾਲੀ *font_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ,
			  hb_font_extents_t *metrics,
			  ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  ਵਾਪਸ _hb_ot_metrics_get_position_common (font, HB_OT_METRICS_ਟੈਗ_HORIZONTAL_ASCENDER, &metrics->ascender) &&
	 _hb_ot_metrics_get_position_common (font, HB_OT_METRICS_ਟੈਗ_HORIZONTAL_DESCENDER, &metrics->descender) &&
	 _hb_ot_metrics_get_position_common (font, HB_OT_METRICS_ਟੈਗ_HORIZONTAL_ਸਤਰ_GAP, &metrics->ਸਤਰ_gap);
}

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_VERTICAL
ਸਥਿਰ hb_bool_t
hb_ot_get_font_v_extents (hb_font_t *font,
			  ਖਾਲੀ *font_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ,
			  hb_font_extents_t *metrics,
			  ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  ਵਾਪਸ _hb_ot_metrics_get_position_common (font, HB_OT_METRICS_ਟੈਗ_VERTICAL_ASCENDER, &metrics->ascender) &&
	 _hb_ot_metrics_get_position_common (font, HB_OT_METRICS_ਟੈਗ_VERTICAL_DESCENDER, &metrics->descender) &&
	 _hb_ot_metrics_get_position_common (font, HB_OT_METRICS_ਟੈਗ_VERTICAL_ਸਤਰ_GAP, &metrics->ਸਤਰ_gap);
}
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_DRAW
ਸਥਿਰ hb_bool_t
hb_ot_draw_glyph_or_fail (hb_font_t *font,
			  ਖਾਲੀ *font_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ,
			  hb_codepoint_t glyph,
			  hb_draw_funcs_t *draw_funcs, ਖਾਲੀ *draw_ਅੰਕੜੇ,
			  ਖਾਲੀ *user_ਅੰਕੜੇ)
{
  ਅਚਲ hb_ot_font_t *ot_font = (ਅਚਲ hb_ot_font_t *) font_ਅੰਕੜੇ;
  hb_draw_session_t draw_session {draw_funcs, draw_ਅੰਕੜੇ};

  OT::hb_scalar_cache_t *gvar_cache = ਖਾਲੀ_ਸੂਚਕ;
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_VAR
  ਜੇ (font->ਅੰਕ_coords)
  {
    ot_font->check_serial (font);
    gvar_cache = ot_font->draw.acquire_gvar_cache (*ot_font->ot_face->gvar);
  }
#ਜੇ_ਅੰਤ
  HB_SCOPE_GUARD (ot_font->draw.release_gvar_cache (gvar_cache));

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_VAR_COMPOSITES
  ਜੇ (font->face->ਸਾਰਣੀ.VARC->get_ਰਸਤਾ (font, glyph, draw_session)) ਵਾਪਸ ਸੱਚ;
#ਜੇ_ਅੰਤ
  // Keep the following in synch with VARC::get_path_at()
  ਜੇ (font->face->ਸਾਰਣੀ.glyf->get_ਰਸਤਾ (font, glyph, draw_session, gvar_cache,
				       &draw_session.get_budget ())) ਵਾਪਸ ਸੱਚ;

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_CFF
  ਜੇ (font->face->ਸਾਰਣੀ.cff2->get_ਰਸਤਾ (font, glyph, draw_session,
				       &draw_session.get_budget ())) ਵਾਪਸ ਸੱਚ;
  ਜੇ (font->face->ਸਾਰਣੀ.cff1->get_ਰਸਤਾ (font, glyph, draw_session,
				       &draw_session.get_budget ())) ਵਾਪਸ ਸੱਚ;
#ਜੇ_ਅੰਤ

  ਵਾਪਸ ਝੂਠ;
}
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_PAINT
ਸਥਿਰ hb_bool_t
hb_ot_paint_glyph_or_fail (hb_font_t *font,
			   ਖਾਲੀ *font_ਅੰਕੜੇ,
			   hb_codepoint_t glyph,
			   hb_paint_funcs_t *paint_funcs, ਖਾਲੀ *paint_ਅੰਕੜੇ,
			   ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ palette,
			   hb_color_t foreground,
			   ਖਾਲੀ *user_ਅੰਕੜੇ)
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_COLOR
  ਜੇ (font->face->ਸਾਰਣੀ.COLR->paint_glyph (font, glyph, paint_funcs, paint_ਅੰਕੜੇ, palette, foreground)) ਵਾਪਸ ਸੱਚ;
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_SVG
  ਜੇ (font->face->ਸਾਰਣੀ.SVG->paint_glyph (font, glyph, paint_funcs, paint_ਅੰਕੜੇ)) ਵਾਪਸ ਸੱਚ;
#ਜੇ_ਅੰਤ
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_OT_FONT_BITMAP
  ਜੇ (font->face->ਸਾਰਣੀ.CBDT->paint_glyph (font, glyph, paint_funcs, paint_ਅੰਕੜੇ)) ਵਾਪਸ ਸੱਚ;
  ਜੇ (font->face->ਸਾਰਣੀ.sbix->paint_glyph (font, glyph, paint_funcs, paint_ਅੰਕੜੇ)) ਵਾਪਸ ਸੱਚ;
#ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ
  ਵਾਪਸ ਝੂਠ;
}
#ਜੇ_ਅੰਤ

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ free_static_ot_funcs ();

ਸਥਿਰ ਬਣਤਰ hb_ot_font_funcs_lazy_loader_t : hb_font_funcs_lazy_loader_t<hb_ot_font_funcs_lazy_loader_t>
{
  ਸਥਿਰ hb_font_funcs_t *create ()
  {
    hb_font_funcs_t *funcs = hb_font_funcs_create ();

    hb_font_funcs_ਸੈੱਟ_nominal_glyph_func (funcs, hb_ot_get_nominal_glyph, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_font_funcs_ਸੈੱਟ_nominal_glyphs_func (funcs, hb_ot_get_nominal_glyphs, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_font_funcs_ਸੈੱਟ_variation_glyph_func (funcs, hb_ot_get_variation_glyph, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);

    hb_font_funcs_ਸੈੱਟ_font_h_extents_func (funcs, hb_ot_get_font_h_extents, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_font_funcs_ਸੈੱਟ_glyph_h_advances_func (funcs, hb_ot_get_glyph_h_advances, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_VERTICAL
    hb_font_funcs_ਸੈੱਟ_font_v_extents_func (funcs, hb_ot_get_font_v_extents, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_font_funcs_ਸੈੱਟ_glyph_v_advances_func (funcs, hb_ot_get_glyph_v_advances, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_font_funcs_ਸੈੱਟ_glyph_v_origins_func (funcs, hb_ot_get_glyph_v_origins, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_DRAW
    hb_font_funcs_ਸੈੱਟ_draw_glyph_or_fail_func (funcs, hb_ot_draw_glyph_or_fail, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_PAINT
    hb_font_funcs_ਸੈੱਟ_paint_glyph_or_fail_func (funcs, hb_ot_paint_glyph_or_fail, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
#ਜੇ_ਅੰਤ

    hb_font_funcs_ਸੈੱਟ_glyph_extents_func (funcs, hb_ot_get_glyph_extents, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    //hb_font_funcs_set_glyph_contour_point_func (funcs, hb_ot_get_glyph_contour_point, nullptr, nullptr);

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_OT_FONT_GLYPH_ਨਾਂ
    hb_font_funcs_ਸੈੱਟ_glyph_name_func (funcs, hb_ot_get_glyph_name, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_font_funcs_ਸੈੱਟ_glyph_from_name_func (funcs, hb_ot_get_glyph_from_name, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
#ਜੇ_ਅੰਤ

    hb_font_funcs_make_immutable (funcs);

    hb_atexit (free_static_ot_funcs);

    ਵਾਪਸ funcs;
  }
} static_ot_funcs;

ਸਥਿਰ ਸਤਰਅੰਦਰ
ਖਾਲੀ free_static_ot_funcs ()
{
  static_ot_funcs.free_instance ();
}

ਸਥਿਰ hb_font_funcs_t *
_hb_ot_get_font_funcs ()
{
  ਵਾਪਸ static_ot_funcs.get_unconst ();
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
ਖਾਲੀ
hb_ot_font_ਸੈੱਟ_funcs (hb_font_t *font)
{
  hb_ot_font_t *ot_font = _hb_ot_font_create (font);
  ਜੇ (unlikely (!ot_font))
    ਵਾਪਸ;

  hb_font_ਸੈੱਟ_funcs (font,
		     _hb_ot_get_font_funcs (),
		     ot_font,
		     _hb_ot_font_destroy);
}

#ਜੇ_ਅੰਤ
