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

#ifndef HB_NO_OT_FONT

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

using hb_ot_font_advance_cache_t = hb_cache_t<24, 16>;
static_assert (حجم (hb_ot_font_advance_cache_t) == 1024, "");

using hb_ot_font_origin_cache_t = hb_cache_t<20, 20>;
static_assert (حجم (hb_ot_font_origin_cache_t) == 1024, "");

بنية hb_ot_font_t
{
  ثابت hb_ot_face_t *ot_face;

  mutable hb_atomic_t<صحيح> cached_serial;
  mutable hb_atomic_t<صحيح> cached_coords_serial;

  بنية direction_cache_t
  {
    mutable hb_atomic_t<hb_ot_font_advance_cache_t *> advance_cache;
    mutable hb_atomic_t<OT::hb_scalar_cache_t *> varStore_cache;

    ~direction_cache_t ()
    {
      clear ();
    }

    hb_ot_font_advance_cache_t *acquire_advance_cache () ثابت
    {
    retry:
      auto *cache = advance_cache.get_acquire ();
      إذا (!cache)
      {
        cache = (hb_ot_font_advance_cache_t *) hb_malloc (حجم (hb_ot_font_advance_cache_t));
	إذا (!cache)
	  إرجاع nullptr;
	جديد (cache) hb_ot_font_advance_cache_t;
	إرجاع cache;
      }
      إذا (advance_cache.cmpexch (cache, nullptr))
        إرجاع cache;
      وإلا
        اذهب retry;
    }
    فراغ release_advance_cache (hb_ot_font_advance_cache_t *cache) ثابت
    {
      إذا (!cache)
        إرجاع;
      إذا (!advance_cache.cmpexch (nullptr, cache))
        hb_free (cache);
    }
    فراغ clear_advance_cache () ثابت
    {
    retry:
      auto *cache = advance_cache.get_acquire ();
      إذا (!cache)
	إرجاع;
      إذا (advance_cache.cmpexch (cache, nullptr))
	hb_free (cache);
      وإلا
        اذهب retry;
    }

    OT::hb_scalar_cache_t *acquire_varStore_cache (ثابت OT::ItemVariationStore &varStore) ثابت
    {
    retry:
      auto *cache = varStore_cache.get_acquire ();
      إذا (!cache)
	إرجاع varStore.create_cache ();
      إذا (varStore_cache.cmpexch (cache, nullptr))
	إرجاع cache;
      وإلا
	اذهب retry;
    }
    فراغ release_varStore_cache (OT::hb_scalar_cache_t *cache) ثابت
    {
      إذا (!cache)
	إرجاع;
      إذا (!varStore_cache.cmpexch (nullptr, cache))
	OT::ItemVariationStore::destroy_cache (cache);
    }
    فراغ clear_varStore_cache () ثابت
    {
    retry:
      auto *cache = varStore_cache.get_acquire ();
      إذا (!cache)
	إرجاع;
      إذا (varStore_cache.cmpexch (cache, nullptr))
	OT::ItemVariationStore::destroy_cache (cache);
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
    mutable hb_atomic_t<hb_ot_font_origin_cache_t *> origin_cache;
    mutable hb_atomic_t<OT::hb_scalar_cache_t *> varStore_cache;

    ~origin_cache_t ()
    {
      clear ();
    }

    hb_ot_font_origin_cache_t *acquire_origin_cache () ثابت
    {
    retry:
      auto *cache = origin_cache.get_acquire ();
      إذا (!cache)
      {
        cache = (hb_ot_font_origin_cache_t *) hb_malloc (حجم (hb_ot_font_origin_cache_t));
	إذا (!cache)
	  إرجاع nullptr;
	جديد (cache) hb_ot_font_origin_cache_t;
	إرجاع cache;
      }
      إذا (origin_cache.cmpexch (cache, nullptr))
        إرجاع cache;
      وإلا
        اذهب retry;
    }
    فراغ release_origin_cache (hb_ot_font_origin_cache_t *cache) ثابت
    {
      إذا (!cache)
        إرجاع;
      إذا (!origin_cache.cmpexch (nullptr, cache))
        hb_free (cache);
    }
    فراغ clear_origin_cache () ثابت
    {
    retry:
      auto *cache = origin_cache.get_acquire ();
      إذا (!cache)
	إرجاع;
      إذا (origin_cache.cmpexch (cache, nullptr))
	hb_free (cache);
      وإلا
        اذهب retry;
    }

    OT::hb_scalar_cache_t *acquire_varStore_cache (ثابت OT::ItemVariationStore &varStore) ثابت
    {
    retry:
      auto *cache = varStore_cache.get_acquire ();
      إذا (!cache)
	إرجاع varStore.create_cache ();
      إذا (varStore_cache.cmpexch (cache, nullptr))
	إرجاع cache;
      وإلا
	اذهب retry;
    }
    فراغ release_varStore_cache (OT::hb_scalar_cache_t *cache) ثابت
    {
      إذا (!cache)
	إرجاع;
      إذا (!varStore_cache.cmpexch (nullptr, cache))
	OT::ItemVariationStore::destroy_cache (cache);
    }
    فراغ clear_varStore_cache () ثابت
    {
    retry:
      auto *cache = varStore_cache.get_acquire ();
      إذا (!cache)
	إرجاع;
      إذا (varStore_cache.cmpexch (cache, nullptr))
	OT::ItemVariationStore::destroy_cache (cache);
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
    mutable hb_atomic_t<OT::hb_scalar_cache_t *> gvar_cache;

    ~draw_cache_t ()
    {
      clear ();
    }

    OT::hb_scalar_cache_t *acquire_gvar_cache (ثابت OT::gvar_accelerator_t &gvar) ثابت
    {
    retry:
      auto *cache = gvar_cache.get_acquire ();
      إذا (!cache)
	إرجاع gvar.create_cache ();
      إذا (gvar_cache.cmpexch (cache, nullptr))
	إرجاع cache;
      وإلا
	اذهب retry;
    }
    فراغ release_gvar_cache (OT::hb_scalar_cache_t *cache) ثابت
    {
      إذا (!cache)
	إرجاع;
      إذا (!gvar_cache.cmpexch (nullptr, cache))
	OT::gvar_accelerator_t::destroy_cache (cache);
    }
    فراغ clear_gvar_cache () ثابت
    {
    retry:
      auto *cache = gvar_cache.get_acquire ();
      إذا (!cache)
	إرجاع;
      إذا (gvar_cache.cmpexch (cache, nullptr))
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

      cached_serial.set_release (font_serial);
    }

    صحيح font_serial_coords = font->serial_coords.get_acquire ();
    إذا (cached_coords_serial.get_acquire () != font_serial_coords)
    {
      /* These caches are independent of scale or synthetic settings.
       * Just variation changes will invalidate them. */
      h.clear ();
      v.clear ();
      draw.clear ();

      cached_coords_serial.set_release (font_serial_coords);
    }
  }
};

ساكن hb_ot_font_t *
_hb_ot_font_create (hb_font_t *font)
{
  hb_ot_font_t *ot_font = (hb_ot_font_t *) hb_calloc (1, حجم (hb_ot_font_t));
  إذا (unlikely (!ot_font))
    إرجاع nullptr;

  ot_font->ot_face = &font->face->table;

  إرجاع ot_font;
}

ساكن فراغ
_hb_ot_font_destroy (فراغ *font_data)
{
  hb_ot_font_t *ot_font = (hb_ot_font_t *) font_data;

  ot_font->~hb_ot_font_t ();

  hb_free (ot_font);
}

ساكن hb_bool_t
hb_ot_get_nominal_glyph (hb_font_t *font HB_UNUSED,
			 فراغ *font_data,
			 hb_codepoint_t unicode,
			 hb_codepoint_t *glyph,
			 فراغ *user_data HB_UNUSED)
{
  ثابت hb_ot_font_t *ot_font = (ثابت hb_ot_font_t *) font_data;
  ثابت hb_ot_face_t *ot_face = ot_font->ot_face;
  إرجاع ot_face->cmap->get_nominal_glyph (unicode, glyph);
}

ساكن unsigned صحيح
hb_ot_get_nominal_glyphs (hb_font_t *font HB_UNUSED,
			  فراغ *font_data,
			  unsigned صحيح تعداد_2,
			  ثابت hb_codepoint_t *first_unicode,
			  unsigned صحيح unicode_stride,
			  hb_codepoint_t *first_glyph,
			  unsigned صحيح glyph_stride,
			  فراغ *user_data HB_UNUSED)
{
  ثابت hb_ot_font_t *ot_font = (ثابت hb_ot_font_t *) font_data;
  ثابت hb_ot_face_t *ot_face = ot_font->ot_face;
  إرجاع ot_face->cmap->get_nominal_glyphs (تعداد_2,
					    first_unicode, unicode_stride,
					    first_glyph, glyph_stride);
}

ساكن hb_bool_t
hb_ot_get_variation_glyph (hb_font_t *font HB_UNUSED,
			   فراغ *font_data,
			   hb_codepoint_t unicode,
			   hb_codepoint_t variation_selector,
			   hb_codepoint_t *glyph,
			   فراغ *user_data HB_UNUSED)
{
  ثابت hb_ot_font_t *ot_font = (ثابت hb_ot_font_t *) font_data;
  ثابت hb_ot_face_t *ot_face = ot_font->ot_face;
  إرجاع ot_face->cmap->get_variation_glyph (unicode,
                                             variation_selector, glyph);
}

ساكن فراغ
hb_ot_get_glyph_h_advances (hb_font_t* font, فراغ* font_data,
			    unsigned تعداد_2,
			    ثابت hb_codepoint_t *first_glyph,
			    unsigned glyph_stride,
			    hb_position_t *first_advance,
			    unsigned advance_stride,
			    فراغ *user_data HB_UNUSED)
{
  // Duplicated in v_advances. Ugly. Keep in sync'ish.

  ثابت hb_ot_font_t *ot_font = (ثابت hb_ot_font_t *) font_data;
  ثابت hb_ot_face_t *ot_face = ot_font->ot_face;
  ثابت OT::hmtx_accelerator_t &hmtx = *ot_face->hmtx;

  إذا (unlikely (!hmtx.has_data ()))
  {
    hb_position_t advance = font->face->get_upem () / 2;
    advance = font->em_scale_x (advance);
    لكل (unsigned صحيح i = 0; i < تعداد_2; i++)
    {
      *first_advance = advance;
      first_advance = &StructAtOffsetUnaligned<hb_position_t> (first_advance, advance_stride);
    }
    إرجاع;
  }

#ifndef HB_NO_VAR
  إذا (!font->has_nonzero_coords)
  {
  fallback:
#وإلا
  {
#endif
    // Just plain htmx data. No need to cache.
    لكل (unsigned صحيح i = 0; i < تعداد_2; i++)
    {
      *first_advance = font->em_scale_x (hmtx.get_advance_without_var_unscaled (*first_glyph));
      first_glyph = &StructAtOffsetUnaligned<hb_codepoint_t> (first_glyph, glyph_stride);
      first_advance = &StructAtOffsetUnaligned<hb_position_t> (first_advance, advance_stride);
    }
    إرجاع;
  }

#ifndef HB_NO_VAR
  /* has_nonzero_coords. */

  ot_font->check_serial (font);
  hb_ot_font_advance_cache_t *advance_cache = ot_font->h.acquire_advance_cache ();
  إذا (!advance_cache)
  {
    // malloc failure. Just use the fallback non-variable path.
    اذهب fallback;
  }

  /* If HVAR is present, use it.*/
  ثابت OT::HVAR &HVAR = *hmtx.var_table;
  إذا (HVAR.has_data ())
  {
    ثابت OT::ItemVariationStore &varStore = &HVAR + HVAR.varStore;
    OT::hb_scalar_cache_t *varStore_cache = ot_font->h.acquire_varStore_cache (varStore);

    لكل (unsigned صحيح i = 0; i < تعداد_2; i++)
    {
      hb_position_t v;
      unsigned cv;
      إذا (advance_cache->get (*first_glyph, &cv))
	v = cv;
      وإلا
      {
        v = hmtx.get_advance_with_var_unscaled (*first_glyph, font, varStore_cache);
	advance_cache->set (*first_glyph, v);
      }
      *first_advance = font->em_scale_x (v);
      first_glyph = &StructAtOffsetUnaligned<hb_codepoint_t> (first_glyph, glyph_stride);
      first_advance = &StructAtOffsetUnaligned<hb_position_t> (first_advance, advance_stride);
    }

    ot_font->h.release_varStore_cache (varStore_cache);
    ot_font->h.release_advance_cache (advance_cache);
    إرجاع;
  }

  ثابت auto &gvar = *ot_face->gvar;
  إذا (gvar.has_data ())
  {
    ثابت auto &glyf = *ot_face->glyf;
    auto *scratch = glyf.acquire_scratch ();
    إذا (unlikely (!scratch))
    {
      ot_font->h.release_advance_cache (advance_cache);
      اذهب fallback;
    }
    OT::hb_scalar_cache_t *gvar_cache = ot_font->draw.acquire_gvar_cache (gvar);

    لكل (unsigned صحيح i = 0; i < تعداد_2; i++)
    {
      hb_position_t v;
      unsigned cv;
      إذا (advance_cache->get (*first_glyph, &cv))
	v = cv;
      وإلا
      {
        v = glyf.get_advance_with_var_unscaled (*first_glyph, font, خطأ, *scratch, gvar_cache);
	advance_cache->set (*first_glyph, v);
      }
      *first_advance = font->em_scale_x (v);
      first_glyph = &StructAtOffsetUnaligned<hb_codepoint_t> (first_glyph, glyph_stride);
      first_advance = &StructAtOffsetUnaligned<hb_position_t> (first_advance, advance_stride);
    }

    ot_font->draw.release_gvar_cache (gvar_cache);
    glyf.release_scratch (scratch);
    ot_font->h.release_advance_cache (advance_cache);
    إرجاع;
  }

  ot_font->h.release_advance_cache (advance_cache);
  // No HVAR or GVAR.  Just use the fallback non-variable path.
  اذهب fallback;
#endif
}

#ifndef HB_NO_VERTICAL
ساكن فراغ
hb_ot_get_glyph_v_advances (hb_font_t* font, فراغ* font_data,
			    unsigned تعداد_2,
			    ثابت hb_codepoint_t *first_glyph,
			    unsigned glyph_stride,
			    hb_position_t *first_advance,
			    unsigned advance_stride,
			    فراغ *user_data HB_UNUSED)
{
  // Duplicated from h_advances. Ugly. Keep in sync'ish.

  ثابت hb_ot_font_t *ot_font = (ثابت hb_ot_font_t *) font_data;
  ثابت hb_ot_face_t *ot_face = ot_font->ot_face;
  ثابت OT::vmtx_accelerator_t &vmtx = *ot_face->vmtx;

  إذا (unlikely (!vmtx.has_data ()))
  {
    hb_font_extents_t font_extents;
    font->get_h_extents_with_fallback (&font_extents);
    hb_position_t advance = font_extents.descender - font_extents.ascender;
    لكل (unsigned صحيح i = 0; i < تعداد_2; i++)
    {
      *first_advance = advance;
      first_advance = &StructAtOffsetUnaligned<hb_position_t> (first_advance, advance_stride);
    }
    إرجاع;
  }

#ifndef HB_NO_VAR
  إذا (!font->has_nonzero_coords)
  {
  fallback:
#وإلا
  {
#endif
    // Just plain vtmx data. No need to cache.
    لكل (unsigned صحيح i = 0; i < تعداد_2; i++)
    {
      *first_advance = font->em_scale_y (- (صحيح) vmtx.get_advance_without_var_unscaled (*first_glyph));
      first_glyph = &StructAtOffsetUnaligned<hb_codepoint_t> (first_glyph, glyph_stride);
      first_advance = &StructAtOffsetUnaligned<hb_position_t> (first_advance, advance_stride);
    }
    إرجاع;
  }

#ifndef HB_NO_VAR
  /* has_nonzero_coords. */

  ot_font->check_serial (font);
  hb_ot_font_advance_cache_t *advance_cache = ot_font->v.acquire_advance_cache ();
  إذا (!advance_cache)
  {
    // malloc failure. Just use the fallback non-variable path.
    اذهب fallback;
  }

  /* If VVAR is present, use it.*/
  ثابت OT::VVAR &VVAR = *vmtx.var_table;
  إذا (VVAR.has_data ())
  {
    ثابت OT::ItemVariationStore &varStore = &VVAR + VVAR.varStore;
    OT::hb_scalar_cache_t *varStore_cache = ot_font->v.acquire_varStore_cache (varStore);

    لكل (unsigned صحيح i = 0; i < تعداد_2; i++)
    {
      hb_position_t v;
      unsigned cv;
      إذا (advance_cache->get (*first_glyph, &cv))
	v = cv;
      وإلا
      {
        v = vmtx.get_advance_with_var_unscaled (*first_glyph, font, varStore_cache);
	advance_cache->set (*first_glyph, v);
      }
      *first_advance = font->em_scale_y (- (صحيح) v);
      first_glyph = &StructAtOffsetUnaligned<hb_codepoint_t> (first_glyph, glyph_stride);
      first_advance = &StructAtOffsetUnaligned<hb_position_t> (first_advance, advance_stride);
    }

    ot_font->v.release_varStore_cache (varStore_cache);
    ot_font->v.release_advance_cache (advance_cache);
    إرجاع;
  }

  ثابت auto &gvar = *ot_face->gvar;
  إذا (gvar.has_data ())
  {
    ثابت auto &glyf = *ot_face->glyf;
    auto *scratch = glyf.acquire_scratch ();
    إذا (unlikely (!scratch))
    {
      ot_font->v.release_advance_cache (advance_cache);
      اذهب fallback;
    }
    OT::hb_scalar_cache_t *gvar_cache = ot_font->draw.acquire_gvar_cache (gvar);

    لكل (unsigned صحيح i = 0; i < تعداد_2; i++)
    {
      hb_position_t v;
      unsigned cv;
      إذا (advance_cache->get (*first_glyph, &cv))
	v = cv;
      وإلا
      {
        v = glyf.get_advance_with_var_unscaled (*first_glyph, font, true, *scratch, gvar_cache);
	advance_cache->set (*first_glyph, v);
      }
      *first_advance = font->em_scale_y (- (صحيح) v);
      first_glyph = &StructAtOffsetUnaligned<hb_codepoint_t> (first_glyph, glyph_stride);
      first_advance = &StructAtOffsetUnaligned<hb_position_t> (first_advance, advance_stride);
    }

    ot_font->draw.release_gvar_cache (gvar_cache);
    glyf.release_scratch (scratch);
    ot_font->v.release_advance_cache (advance_cache);
    إرجاع;
  }

  ot_font->v.release_advance_cache (advance_cache);
  // No VVAR or GVAR.  Just use the fallback non-variable path.
  اذهب fallback;
#endif
}
#endif

#ifndef HB_NO_VERTICAL
HB_HOT
ساكن hb_bool_t
hb_ot_get_glyph_v_origins (hb_font_t *font,
			   فراغ *font_data,
			   unsigned صحيح تعداد_2,
			   ثابت hb_codepoint_t *first_glyph,
			   unsigned glyph_stride,
			   hb_position_t *first_x,
			   unsigned x_stride,
			   hb_position_t *first_y,
			   unsigned y_stride,
			   فراغ *user_data HB_UNUSED)
{
  ثابت hb_ot_font_t *ot_font = (ثابت hb_ot_font_t *) font_data;
  ثابت hb_ot_face_t *ot_face = ot_font->ot_face;

  ot_font->check_serial (font);

  /* First, set all the x values to half the advance width. */
  font->get_glyph_h_advances (تعداد_2,
			      first_glyph, glyph_stride,
			      first_x, x_stride);
  لكل (unsigned i = 0; i < تعداد_2; i++)
  {
    *first_x /= 2;
    first_x = &StructAtOffsetUnaligned<hb_position_t> (first_x, x_stride);
  }

  /* The vertical origin business is messy...
   *
   * We allocate the cache, then have various code paths that use the cache.
   * Each one is responsible to free it before returning.
   */
  hb_ot_font_origin_cache_t *origin_cache = ot_font->v_origin.acquire_origin_cache ();

  /* If there is VORG, always use it. It uses VVAR for variations if necessary. */
  ثابت OT::VORG &VORG = *ot_face->VORG;
  إذا (origin_cache && VORG.has_data ())
  {
#ifndef HB_NO_VAR
    إذا (!font->has_nonzero_coords)
#endif
    {
      لكل (unsigned i = 0; i < تعداد_2; i++)
      {
	hb_position_t origin;
	unsigned cv;
	إذا (origin_cache->get (*first_glyph, &cv))
	  origin = font->y_scale < 0 ? -static_cast<hb_position_t>(cv) : static_cast<hb_position_t>(cv);
	وإلا
	{
	  origin = font->em_scalef_y (VORG.get_y_origin (*first_glyph));
	  origin_cache->set (*first_glyph, font->y_scale < 0 ? -origin : origin);
	}

	*first_y = origin;

	first_glyph = &StructAtOffsetUnaligned<hb_codepoint_t> (first_glyph, glyph_stride);
	first_y = &StructAtOffsetUnaligned<hb_position_t> (first_y, y_stride);
      }
    }
#ifndef HB_NO_VAR
    وإلا
    {
      ثابت OT::VVAR &VVAR = *ot_face->vmtx->var_table;
      ثابت auto &varStore = &VVAR + VVAR.varStore;
      auto *varStore_cache = ot_font->v_origin.acquire_varStore_cache (varStore);
      لكل (unsigned i = 0; i < تعداد_2; i++)
      {
	hb_position_t origin;
	unsigned cv;
	إذا (origin_cache->get (*first_glyph, &cv))
	  origin = font->y_scale < 0 ? -static_cast<hb_position_t>(cv) : static_cast<hb_position_t>(cv);
	وإلا
	{
	  origin = font->em_scalef_y (VORG.get_y_origin (*first_glyph) +
				      VVAR.get_vorg_delta_unscaled (*first_glyph,
								    font->coords, font->رقم_coords,
								    varStore_cache));
	  origin_cache->set (*first_glyph, font->y_scale < 0 ? -origin : origin);
	}

	*first_y = origin;

	first_glyph = &StructAtOffsetUnaligned<hb_codepoint_t> (first_glyph, glyph_stride);
	first_y = &StructAtOffsetUnaligned<hb_position_t> (first_y, y_stride);
      }
      ot_font->v_origin.release_varStore_cache (varStore_cache);
    }
#endif
    ot_font->v_origin.release_origin_cache (origin_cache);
    إرجاع true;
  }

  /* If and only if `vmtx` is present and it's a `glyf` font,
   * we use the top phantom point, deduced from vmtx,glyf[,gvar]. */
  ثابت auto &vmtx = *ot_face->vmtx;
  ثابت auto &glyf = *ot_face->glyf;
  إذا (origin_cache && vmtx.has_data() && glyf.has_data ())
  {
    auto *scratch = glyf.acquire_scratch ();
    إذا (unlikely (!scratch))
    {
      ot_font->v_origin.release_origin_cache (origin_cache);
      إرجاع خطأ;
    }
    OT::hb_scalar_cache_t *gvar_cache = nullptr;
#ifndef HB_NO_VAR
    gvar_cache = font->has_nonzero_coords ?
					ot_font->draw.acquire_gvar_cache (*ot_face->gvar) :
					nullptr;
#endif

    لكل (unsigned i = 0; i < تعداد_2; i++)
    {
      hb_position_t origin;
      unsigned cv;
      إذا (origin_cache->get (*first_glyph, &cv))
	origin = font->y_scale < 0 ? -static_cast<hb_position_t>(cv) : static_cast<hb_position_t>(cv);
      وإلا
      {
	origin = font->em_scalef_y (glyf.get_v_origin_with_var_unscaled (*first_glyph, font, *scratch, gvar_cache));
	origin_cache->set (*first_glyph, font->y_scale < 0 ? -origin : origin);
      }

      *first_y = origin;

      first_glyph = &StructAtOffsetUnaligned<hb_codepoint_t> (first_glyph, glyph_stride);
      first_y = &StructAtOffsetUnaligned<hb_position_t> (first_y, y_stride);
    }

#ifndef HB_NO_VAR
    إذا (gvar_cache)
      ot_font->draw.release_gvar_cache (gvar_cache);
#endif
    glyf.release_scratch (scratch);
    ot_font->v_origin.release_origin_cache (origin_cache);
    إرجاع true;
  }

  /* Otherwise, use glyph extents to center the glyph vertically.
   * If getting glyph extents failed, just use the font ascender. */
  إذا (origin_cache && font->has_glyph_extents_func ())
  {
    hb_font_extents_t font_extents;
    font->get_h_extents_with_fallback (&font_extents);
    hb_position_t font_advance = font_extents.ascender - font_extents.descender;

    لكل (unsigned i = 0; i < تعداد_2; i++)
    {
      hb_position_t origin;
      unsigned cv;

      إذا (origin_cache->get (*first_glyph, &cv))
	origin = font->y_scale < 0 ? -static_cast<hb_position_t>(cv) : static_cast<hb_position_t>(cv);
      وإلا
      {
	hb_glyph_extents_t extents = {0};
	إذا (likely (font->get_glyph_extents (*first_glyph, &extents)))
	  origin = hb_clamp_to<hb_position_t> ((int64_t) extents.y_bearing +
					 (((int64_t) font_advance + extents.height) >> 1));
	وإلا
	  origin = font_extents.ascender;

	origin_cache->set (*first_glyph, font->y_scale < 0 ? -origin : origin);
      }

      *first_y = origin;

      first_glyph = &StructAtOffsetUnaligned<hb_codepoint_t> (first_glyph, glyph_stride);
      first_y = &StructAtOffsetUnaligned<hb_position_t> (first_y, y_stride);
    }
  }

  ot_font->v_origin.release_origin_cache (origin_cache);
  إرجاع true;
}
#endif

ساكن hb_bool_t
hb_ot_get_glyph_extents (hb_font_t *font,
			 فراغ *font_data,
			 hb_codepoint_t glyph,
			 hb_glyph_extents_t *extents,
			 فراغ *user_data HB_UNUSED)
{
  ثابت hb_ot_font_t *ot_font = (ثابت hb_ot_font_t *) font_data;
  ثابت hb_ot_face_t *ot_face = ot_font->ot_face;

#إذا !defined(HB_NO_OT_FONT_BITMAP) && !defined(HB_NO_COLOR)
  إذا (ot_face->sbix->get_extents (font, glyph, extents)) إرجاع true;
  إذا (ot_face->CBDT->get_extents (font, glyph, extents)) إرجاع true;
#endif
#إذا !defined(HB_NO_COLOR) && !defined(HB_NO_PAINT)
  إذا (ot_face->COLR->get_extents (font, glyph, extents)) إرجاع true;
#endif
#ifndef HB_NO_VAR_COMPOSITES
  إذا (ot_face->VARC->get_extents (font, glyph, extents)) إرجاع true;
#endif
  إذا (ot_face->glyf->get_extents (font, glyph, extents)) إرجاع true;
#ifndef HB_NO_OT_FONT_CFF
  إذا (ot_face->cff2->get_extents (font, glyph, extents)) إرجاع true;
  إذا (ot_face->cff1->get_extents (font, glyph, extents)) إرجاع true;
#endif

  إرجاع خطأ;
}

#ifndef HB_NO_OT_FONT_GLYPH_NAMES
ساكن hb_bool_t
hb_ot_get_glyph_name (hb_font_t *font HB_UNUSED,
		      فراغ *font_data,
		      hb_codepoint_t glyph,
		      حرف *name, unsigned صحيح size,
		      فراغ *user_data HB_UNUSED)
{
  ثابت hb_ot_font_t *ot_font = (ثابت hb_ot_font_t *) font_data;
  ثابت hb_ot_face_t *ot_face = ot_font->ot_face;

  إذا (ot_face->post->get_glyph_name (glyph, name, size)) إرجاع true;
#ifndef HB_NO_OT_FONT_CFF
  إذا (ot_face->cff1->get_glyph_name (glyph, name, size)) إرجاع true;
#endif
  إرجاع خطأ;
}
ساكن hb_bool_t
hb_ot_get_glyph_from_name (hb_font_t *font HB_UNUSED,
			   فراغ *font_data,
			   ثابت حرف *name, صحيح len,
			   hb_codepoint_t *glyph,
			   فراغ *user_data HB_UNUSED)
{
  ثابت hb_ot_font_t *ot_font = (ثابت hb_ot_font_t *) font_data;
  ثابت hb_ot_face_t *ot_face = ot_font->ot_face;

  إذا (ot_face->post->get_glyph_from_name (name, len, glyph)) إرجاع true;
#ifndef HB_NO_OT_FONT_CFF
    إذا (ot_face->cff1->get_glyph_from_name (name, len, glyph)) إرجاع true;
#endif
  إرجاع خطأ;
}
#endif

ساكن hb_bool_t
hb_ot_get_font_h_extents (hb_font_t *font,
			  فراغ *font_data HB_UNUSED,
			  hb_font_extents_t *metrics,
			  فراغ *user_data HB_UNUSED)
{
  إرجاع _hb_ot_metrics_get_position_common (font, HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER, &metrics->ascender) &&
	 _hb_ot_metrics_get_position_common (font, HB_OT_METRICS_TAG_HORIZONTAL_DESCENDER, &metrics->descender) &&
	 _hb_ot_metrics_get_position_common (font, HB_OT_METRICS_TAG_HORIZONTAL_سطر_GAP, &metrics->سطر_gap);
}

#ifndef HB_NO_VERTICAL
ساكن hb_bool_t
hb_ot_get_font_v_extents (hb_font_t *font,
			  فراغ *font_data HB_UNUSED,
			  hb_font_extents_t *metrics,
			  فراغ *user_data HB_UNUSED)
{
  إرجاع _hb_ot_metrics_get_position_common (font, HB_OT_METRICS_TAG_VERTICAL_ASCENDER, &metrics->ascender) &&
	 _hb_ot_metrics_get_position_common (font, HB_OT_METRICS_TAG_VERTICAL_DESCENDER, &metrics->descender) &&
	 _hb_ot_metrics_get_position_common (font, HB_OT_METRICS_TAG_VERTICAL_سطر_GAP, &metrics->سطر_gap);
}
#endif

#ifndef HB_NO_DRAW
ساكن hb_bool_t
hb_ot_draw_glyph_or_fail (hb_font_t *font,
			  فراغ *font_data HB_UNUSED,
			  hb_codepoint_t glyph,
			  hb_draw_funcs_t *draw_funcs, فراغ *draw_data,
			  فراغ *user_data)
{
  ثابت hb_ot_font_t *ot_font = (ثابت hb_ot_font_t *) font_data;
  hb_draw_session_t draw_session {draw_funcs, draw_data};

  OT::hb_scalar_cache_t *gvar_cache = nullptr;
#ifndef HB_NO_VAR
  إذا (font->رقم_coords)
  {
    ot_font->check_serial (font);
    gvar_cache = ot_font->draw.acquire_gvar_cache (*ot_font->ot_face->gvar);
  }
#endif
  HB_SCOPE_GUARD (ot_font->draw.release_gvar_cache (gvar_cache));

#ifndef HB_NO_VAR_COMPOSITES
  إذا (font->face->table.VARC->get_path (font, glyph, draw_session)) إرجاع true;
#endif
  // Keep the following in synch with VARC::get_path_at()
  إذا (font->face->table.glyf->get_path (font, glyph, draw_session, gvar_cache,
				       &draw_session.get_budget ())) إرجاع true;

#ifndef HB_NO_CFF
  إذا (font->face->table.cff2->get_path (font, glyph, draw_session,
				       &draw_session.get_budget ())) إرجاع true;
  إذا (font->face->table.cff1->get_path (font, glyph, draw_session,
				       &draw_session.get_budget ())) إرجاع true;
#endif

  إرجاع خطأ;
}
#endif

#ifndef HB_NO_PAINT
ساكن hb_bool_t
hb_ot_paint_glyph_or_fail (hb_font_t *font,
			   فراغ *font_data,
			   hb_codepoint_t glyph,
			   hb_paint_funcs_t *paint_funcs, فراغ *paint_data,
			   unsigned صحيح palette,
			   hb_color_t foreground,
			   فراغ *user_data)
{
#ifndef HB_NO_COLOR
  إذا (font->face->table.COLR->paint_glyph (font, glyph, paint_funcs, paint_data, palette, foreground)) إرجاع true;
#ifndef HB_NO_SVG
  إذا (font->face->table.SVG->paint_glyph (font, glyph, paint_funcs, paint_data)) إرجاع true;
#endif
#ifndef HB_NO_OT_FONT_BITMAP
  إذا (font->face->table.CBDT->paint_glyph (font, glyph, paint_funcs, paint_data)) إرجاع true;
  إذا (font->face->table.sbix->paint_glyph (font, glyph, paint_funcs, paint_data)) إرجاع true;
#endif
#endif
  إرجاع خطأ;
}
#endif

ساكن مضمن فراغ free_static_ot_funcs ();

ساكن بنية hb_ot_font_funcs_lazy_loader_t : hb_font_funcs_lazy_loader_t<hb_ot_font_funcs_lazy_loader_t>
{
  ساكن hb_font_funcs_t *create ()
  {
    hb_font_funcs_t *funcs = hb_font_funcs_create ();

    hb_font_funcs_set_nominal_glyph_func (funcs, hb_ot_get_nominal_glyph, nullptr, nullptr);
    hb_font_funcs_set_nominal_glyphs_func (funcs, hb_ot_get_nominal_glyphs, nullptr, nullptr);
    hb_font_funcs_set_variation_glyph_func (funcs, hb_ot_get_variation_glyph, nullptr, nullptr);

    hb_font_funcs_set_font_h_extents_func (funcs, hb_ot_get_font_h_extents, nullptr, nullptr);
    hb_font_funcs_set_glyph_h_advances_func (funcs, hb_ot_get_glyph_h_advances, nullptr, nullptr);

#ifndef HB_NO_VERTICAL
    hb_font_funcs_set_font_v_extents_func (funcs, hb_ot_get_font_v_extents, nullptr, nullptr);
    hb_font_funcs_set_glyph_v_advances_func (funcs, hb_ot_get_glyph_v_advances, nullptr, nullptr);
    hb_font_funcs_set_glyph_v_origins_func (funcs, hb_ot_get_glyph_v_origins, nullptr, nullptr);
#endif

#ifndef HB_NO_DRAW
    hb_font_funcs_set_draw_glyph_or_fail_func (funcs, hb_ot_draw_glyph_or_fail, nullptr, nullptr);
#endif

#ifndef HB_NO_PAINT
    hb_font_funcs_set_paint_glyph_or_fail_func (funcs, hb_ot_paint_glyph_or_fail, nullptr, nullptr);
#endif

    hb_font_funcs_set_glyph_extents_func (funcs, hb_ot_get_glyph_extents, nullptr, nullptr);
    //hb_font_funcs_set_glyph_contour_point_func (funcs, hb_ot_get_glyph_contour_point, nullptr, nullptr);

#ifndef HB_NO_OT_FONT_GLYPH_NAMES
    hb_font_funcs_set_glyph_name_func (funcs, hb_ot_get_glyph_name, nullptr, nullptr);
    hb_font_funcs_set_glyph_from_name_func (funcs, hb_ot_get_glyph_from_name, nullptr, nullptr);
#endif

    hb_font_funcs_make_immutable (funcs);

    hb_atexit (free_static_ot_funcs);

    إرجاع funcs;
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
  إرجاع static_ot_funcs.get_unconst ();
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
hb_ot_font_set_funcs (hb_font_t *font)
{
  hb_ot_font_t *ot_font = _hb_ot_font_create (font);
  إذا (unlikely (!ot_font))
    إرجاع;

  hb_font_set_funcs (font,
		     _hb_ot_get_font_funcs (),
		     ot_font,
		     _hb_ot_font_destroy);
}

#endif
