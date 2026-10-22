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

#অন্তর্ভাৱ "hb.hh"

#ifndef HB_NO_OT_FONT

#অন্তর্ভাৱ "hb-ot.h"

#অন্তর্ভাৱ "hb-cache.hh"
#অন্তর্ভাৱ "hb-font.hh"
#অন্তর্ভাৱ "hb-machinery.hh"
#অন্তর্ভাৱ "hb-ot-face.hh"

#অন্তর্ভাৱ "hb-ot-cmap-table.hh"
#অন্তর্ভাৱ "hb-ot-glyf-table.hh"
#অন্তর্ভাৱ "hb-ot-var-gvar-table.hh"
#অন্তর্ভাৱ "hb-ot-cff2-table.hh"
#অন্তর্ভাৱ "hb-ot-cff1-table.hh"
#অন্তর্ভাৱ "hb-ot-hmtx-table.hh"
#অন্তর্ভাৱ "hb-ot-post-table.hh"
#অন্তর্ভাৱ "hb-ot-stat-table.hh"
#অন্তর্ভাৱ "hb-ot-var-varc-table.hh"
#অন্তর্ভাৱ "hb-ot-vorg-table.hh"
#অন্তর্ভাৱ "OT/Color/CBDT/CBDT.hh"
#অন্তর্ভাৱ "OT/Color/COLR/COLR.hh"
#অন্তর্ভাৱ "OT/Color/sbix/sbix.hh"
#অন্তর্ভাৱ "OT/Color/svg/svg.hh"


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
static_assert (পরিমাণ (hb_ot_font_advance_cache_t) == 1024, "");

using hb_ot_font_origin_cache_t = hb_cache_t<20, 20>;
static_assert (পরিমাণ (hb_ot_font_origin_cache_t) == 1024, "");

সংরচনা hb_ot_font_t
{
  ধ্রুৱ hb_ot_face_t *ot_face;

  mutable hb_atomic_t<পূর্ণাঙ্ক> cached_serial;
  mutable hb_atomic_t<পূর্ণাঙ্ক> cached_coords_serial;

  সংরচনা direction_cache_t
  {
    mutable hb_atomic_t<hb_ot_font_advance_cache_t *> advance_cache;
    mutable hb_atomic_t<OT::hb_scalar_cache_t *> varStore_cache;

    ~direction_cache_t ()
    {
      clear ();
    }

    hb_ot_font_advance_cache_t *acquire_advance_cache () ধ্রুৱ
    {
    retry:
      auto *cache = advance_cache.get_acquire ();
      যদি (!cache)
      {
        cache = (hb_ot_font_advance_cache_t *) hb_malloc (পরিমাণ (hb_ot_font_advance_cache_t));
	যদি (!cache)
	  ঘূৰাই nullptr;
	নূতন (cache) hb_ot_font_advance_cache_t;
	ঘূৰাই cache;
      }
      যদি (advance_cache.cmpexch (cache, nullptr))
        ঘূৰাই cache;
      অন্যথা
        গচ্ছ retry;
    }
    শূন্য release_advance_cache (hb_ot_font_advance_cache_t *cache) ধ্রুৱ
    {
      যদি (!cache)
        ঘূৰাই;
      যদি (!advance_cache.cmpexch (nullptr, cache))
        hb_free (cache);
    }
    শূন্য clear_advance_cache () ধ্রুৱ
    {
    retry:
      auto *cache = advance_cache.get_acquire ();
      যদি (!cache)
	ঘূৰাই;
      যদি (advance_cache.cmpexch (cache, nullptr))
	hb_free (cache);
      অন্যথা
        গচ্ছ retry;
    }

    OT::hb_scalar_cache_t *acquire_varStore_cache (ধ্রুৱ OT::ItemVariationStore &varStore) ধ্রুৱ
    {
    retry:
      auto *cache = varStore_cache.get_acquire ();
      যদি (!cache)
	ঘূৰাই varStore.create_cache ();
      যদি (varStore_cache.cmpexch (cache, nullptr))
	ঘূৰাই cache;
      অন্যথা
	গচ্ছ retry;
    }
    শূন্য release_varStore_cache (OT::hb_scalar_cache_t *cache) ধ্রুৱ
    {
      যদি (!cache)
	ঘূৰাই;
      যদি (!varStore_cache.cmpexch (nullptr, cache))
	OT::ItemVariationStore::destroy_cache (cache);
    }
    শূন্য clear_varStore_cache () ধ্রুৱ
    {
    retry:
      auto *cache = varStore_cache.get_acquire ();
      যদি (!cache)
	ঘূৰাই;
      যদি (varStore_cache.cmpexch (cache, nullptr))
	OT::ItemVariationStore::destroy_cache (cache);
      অন্যথা
	গচ্ছ retry;
    }

    শূন্য clear () ধ্রুৱ
    {
      clear_advance_cache ();
      clear_varStore_cache ();
    }

  } h, v;

  সংরচনা origin_cache_t
  {
    mutable hb_atomic_t<hb_ot_font_origin_cache_t *> origin_cache;
    mutable hb_atomic_t<OT::hb_scalar_cache_t *> varStore_cache;

    ~origin_cache_t ()
    {
      clear ();
    }

    hb_ot_font_origin_cache_t *acquire_origin_cache () ধ্রুৱ
    {
    retry:
      auto *cache = origin_cache.get_acquire ();
      যদি (!cache)
      {
        cache = (hb_ot_font_origin_cache_t *) hb_malloc (পরিমাণ (hb_ot_font_origin_cache_t));
	যদি (!cache)
	  ঘূৰাই nullptr;
	নূতন (cache) hb_ot_font_origin_cache_t;
	ঘূৰাই cache;
      }
      যদি (origin_cache.cmpexch (cache, nullptr))
        ঘূৰাই cache;
      অন্যথা
        গচ্ছ retry;
    }
    শূন্য release_origin_cache (hb_ot_font_origin_cache_t *cache) ধ্রুৱ
    {
      যদি (!cache)
        ঘূৰাই;
      যদি (!origin_cache.cmpexch (nullptr, cache))
        hb_free (cache);
    }
    শূন্য clear_origin_cache () ধ্রুৱ
    {
    retry:
      auto *cache = origin_cache.get_acquire ();
      যদি (!cache)
	ঘূৰাই;
      যদি (origin_cache.cmpexch (cache, nullptr))
	hb_free (cache);
      অন্যথা
        গচ্ছ retry;
    }

    OT::hb_scalar_cache_t *acquire_varStore_cache (ধ্রুৱ OT::ItemVariationStore &varStore) ধ্রুৱ
    {
    retry:
      auto *cache = varStore_cache.get_acquire ();
      যদি (!cache)
	ঘূৰাই varStore.create_cache ();
      যদি (varStore_cache.cmpexch (cache, nullptr))
	ঘূৰাই cache;
      অন্যথা
	গচ্ছ retry;
    }
    শূন্য release_varStore_cache (OT::hb_scalar_cache_t *cache) ধ্রুৱ
    {
      যদি (!cache)
	ঘূৰাই;
      যদি (!varStore_cache.cmpexch (nullptr, cache))
	OT::ItemVariationStore::destroy_cache (cache);
    }
    শূন্য clear_varStore_cache () ধ্রুৱ
    {
    retry:
      auto *cache = varStore_cache.get_acquire ();
      যদি (!cache)
	ঘূৰাই;
      যদি (varStore_cache.cmpexch (cache, nullptr))
	OT::ItemVariationStore::destroy_cache (cache);
      অন্যথা
	গচ্ছ retry;
    }

    শূন্য clear () ধ্রুৱ
    {
      clear_origin_cache ();
      clear_varStore_cache ();
    }
  } v_origin;

  সংরচনা draw_cache_t
  {
    mutable hb_atomic_t<OT::hb_scalar_cache_t *> gvar_cache;

    ~draw_cache_t ()
    {
      clear ();
    }

    OT::hb_scalar_cache_t *acquire_gvar_cache (ধ্রুৱ OT::gvar_accelerator_t &gvar) ধ্রুৱ
    {
    retry:
      auto *cache = gvar_cache.get_acquire ();
      যদি (!cache)
	ঘূৰাই gvar.create_cache ();
      যদি (gvar_cache.cmpexch (cache, nullptr))
	ঘূৰাই cache;
      অন্যথা
	গচ্ছ retry;
    }
    শূন্য release_gvar_cache (OT::hb_scalar_cache_t *cache) ধ্রুৱ
    {
      যদি (!cache)
	ঘূৰাই;
      যদি (!gvar_cache.cmpexch (nullptr, cache))
	OT::gvar_accelerator_t::destroy_cache (cache);
    }
    শূন্য clear_gvar_cache () ধ্রুৱ
    {
    retry:
      auto *cache = gvar_cache.get_acquire ();
      যদি (!cache)
	ঘূৰাই;
      যদি (gvar_cache.cmpexch (cache, nullptr))
	OT::gvar_accelerator_t::destroy_cache (cache);
      অন্যথা
	গচ্ছ retry;
    }

    শূন্য clear () ধ্রুৱ
    {
      clear_gvar_cache ();
    }
  } draw;

  শূন্য check_serial (hb_font_t *font) ধ্রুৱ
  {
    পূর্ণাঙ্ক font_serial = font->serial.get_acquire ();
    যদি (cached_serial.get_acquire () != font_serial)
    {
      /* These caches are dependent on scale and synthetic settings.
       * Any change to the font invalidates them. */
      v_origin.clear ();

      cached_serial.set_release (font_serial);
    }

    পূর্ণাঙ্ক font_serial_coords = font->serial_coords.get_acquire ();
    যদি (cached_coords_serial.get_acquire () != font_serial_coords)
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

স্থির hb_ot_font_t *
_hb_ot_font_create (hb_font_t *font)
{
  hb_ot_font_t *ot_font = (hb_ot_font_t *) hb_calloc (1, পরিমাণ (hb_ot_font_t));
  যদি (unlikely (!ot_font))
    ঘূৰাই nullptr;

  ot_font->ot_face = &font->face->table;

  ঘূৰাই ot_font;
}

স্থির শূন্য
_hb_ot_font_destroy (শূন্য *font_data)
{
  hb_ot_font_t *ot_font = (hb_ot_font_t *) font_data;

  ot_font->~hb_ot_font_t ();

  hb_free (ot_font);
}

স্থির hb_bool_t
hb_ot_get_nominal_glyph (hb_font_t *font HB_UNUSED,
			 শূন্য *font_data,
			 hb_codepoint_t unicode,
			 hb_codepoint_t *glyph,
			 শূন্য *user_data HB_UNUSED)
{
  ধ্রুৱ hb_ot_font_t *ot_font = (ধ্রুৱ hb_ot_font_t *) font_data;
  ধ্রুৱ hb_ot_face_t *ot_face = ot_font->ot_face;
  ঘূৰাই ot_face->cmap->get_nominal_glyph (unicode, glyph);
}

স্থির unsigned পূর্ণাঙ্ক
hb_ot_get_nominal_glyphs (hb_font_t *font HB_UNUSED,
			  শূন্য *font_data,
			  unsigned পূর্ণাঙ্ক গুনতি,
			  ধ্রুৱ hb_codepoint_t *first_unicode,
			  unsigned পূর্ণাঙ্ক unicode_stride,
			  hb_codepoint_t *first_glyph,
			  unsigned পূর্ণাঙ্ক glyph_stride,
			  শূন্য *user_data HB_UNUSED)
{
  ধ্রুৱ hb_ot_font_t *ot_font = (ধ্রুৱ hb_ot_font_t *) font_data;
  ধ্রুৱ hb_ot_face_t *ot_face = ot_font->ot_face;
  ঘূৰাই ot_face->cmap->get_nominal_glyphs (গুনতি,
					    first_unicode, unicode_stride,
					    first_glyph, glyph_stride);
}

স্থির hb_bool_t
hb_ot_get_variation_glyph (hb_font_t *font HB_UNUSED,
			   শূন্য *font_data,
			   hb_codepoint_t unicode,
			   hb_codepoint_t variation_selector,
			   hb_codepoint_t *glyph,
			   শূন্য *user_data HB_UNUSED)
{
  ধ্রুৱ hb_ot_font_t *ot_font = (ধ্রুৱ hb_ot_font_t *) font_data;
  ধ্রুৱ hb_ot_face_t *ot_face = ot_font->ot_face;
  ঘূৰাই ot_face->cmap->get_variation_glyph (unicode,
                                             variation_selector, glyph);
}

স্থির শূন্য
hb_ot_get_glyph_h_advances (hb_font_t* font, শূন্য* font_data,
			    unsigned গুনতি,
			    ধ্রুৱ hb_codepoint_t *first_glyph,
			    unsigned glyph_stride,
			    hb_position_t *first_advance,
			    unsigned advance_stride,
			    শূন্য *user_data HB_UNUSED)
{
  // Duplicated in v_advances. Ugly. Keep in sync'ish.

  ধ্রুৱ hb_ot_font_t *ot_font = (ধ্রুৱ hb_ot_font_t *) font_data;
  ধ্রুৱ hb_ot_face_t *ot_face = ot_font->ot_face;
  ধ্রুৱ OT::hmtx_accelerator_t &hmtx = *ot_face->hmtx;

  যদি (unlikely (!hmtx.has_data ()))
  {
    hb_position_t advance = font->face->get_upem () / 2;
    advance = font->em_scale_x (advance);
    বাবে (unsigned পূর্ণাঙ্ক i = 0; i < গুনতি; i++)
    {
      *first_advance = advance;
      first_advance = &StructAtOffsetUnaligned<hb_position_t> (first_advance, advance_stride);
    }
    ঘূৰাই;
  }

#ifndef HB_NO_VAR
  যদি (!font->has_nonzero_coords)
  {
  fallback:
#অন্যথা
  {
#endif
    // Just plain htmx data. No need to cache.
    বাবে (unsigned পূর্ণাঙ্ক i = 0; i < গুনতি; i++)
    {
      *first_advance = font->em_scale_x (hmtx.get_advance_without_var_unscaled (*first_glyph));
      first_glyph = &StructAtOffsetUnaligned<hb_codepoint_t> (first_glyph, glyph_stride);
      first_advance = &StructAtOffsetUnaligned<hb_position_t> (first_advance, advance_stride);
    }
    ঘূৰাই;
  }

#ifndef HB_NO_VAR
  /* has_nonzero_coords. */

  ot_font->check_serial (font);
  hb_ot_font_advance_cache_t *advance_cache = ot_font->h.acquire_advance_cache ();
  যদি (!advance_cache)
  {
    // malloc failure. Just use the fallback non-variable path.
    গচ্ছ fallback;
  }

  /* If HVAR is present, use it.*/
  ধ্রুৱ OT::HVAR &HVAR = *hmtx.var_table;
  যদি (HVAR.has_data ())
  {
    ধ্রুৱ OT::ItemVariationStore &varStore = &HVAR + HVAR.varStore;
    OT::hb_scalar_cache_t *varStore_cache = ot_font->h.acquire_varStore_cache (varStore);

    বাবে (unsigned পূর্ণাঙ্ক i = 0; i < গুনতি; i++)
    {
      hb_position_t v;
      unsigned cv;
      যদি (advance_cache->get (*first_glyph, &cv))
	v = cv;
      অন্যথা
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
    ঘূৰাই;
  }

  ধ্রুৱ auto &gvar = *ot_face->gvar;
  যদি (gvar.has_data ())
  {
    ধ্রুৱ auto &glyf = *ot_face->glyf;
    auto *scratch = glyf.acquire_scratch ();
    যদি (unlikely (!scratch))
    {
      ot_font->h.release_advance_cache (advance_cache);
      গচ্ছ fallback;
    }
    OT::hb_scalar_cache_t *gvar_cache = ot_font->draw.acquire_gvar_cache (gvar);

    বাবে (unsigned পূর্ণাঙ্ক i = 0; i < গুনতি; i++)
    {
      hb_position_t v;
      unsigned cv;
      যদি (advance_cache->get (*first_glyph, &cv))
	v = cv;
      অন্যথা
      {
        v = glyf.get_advance_with_var_unscaled (*first_glyph, font, মিছা, *scratch, gvar_cache);
	advance_cache->set (*first_glyph, v);
      }
      *first_advance = font->em_scale_x (v);
      first_glyph = &StructAtOffsetUnaligned<hb_codepoint_t> (first_glyph, glyph_stride);
      first_advance = &StructAtOffsetUnaligned<hb_position_t> (first_advance, advance_stride);
    }

    ot_font->draw.release_gvar_cache (gvar_cache);
    glyf.release_scratch (scratch);
    ot_font->h.release_advance_cache (advance_cache);
    ঘূৰাই;
  }

  ot_font->h.release_advance_cache (advance_cache);
  // No HVAR or GVAR.  Just use the fallback non-variable path.
  গচ্ছ fallback;
#endif
}

#ifndef HB_NO_VERTICAL
স্থির শূন্য
hb_ot_get_glyph_v_advances (hb_font_t* font, শূন্য* font_data,
			    unsigned গুনতি,
			    ধ্রুৱ hb_codepoint_t *first_glyph,
			    unsigned glyph_stride,
			    hb_position_t *first_advance,
			    unsigned advance_stride,
			    শূন্য *user_data HB_UNUSED)
{
  // Duplicated from h_advances. Ugly. Keep in sync'ish.

  ধ্রুৱ hb_ot_font_t *ot_font = (ধ্রুৱ hb_ot_font_t *) font_data;
  ধ্রুৱ hb_ot_face_t *ot_face = ot_font->ot_face;
  ধ্রুৱ OT::vmtx_accelerator_t &vmtx = *ot_face->vmtx;

  যদি (unlikely (!vmtx.has_data ()))
  {
    hb_font_extents_t font_extents;
    font->get_h_extents_with_fallback (&font_extents);
    hb_position_t advance = font_extents.descender - font_extents.ascender;
    বাবে (unsigned পূর্ণাঙ্ক i = 0; i < গুনতি; i++)
    {
      *first_advance = advance;
      first_advance = &StructAtOffsetUnaligned<hb_position_t> (first_advance, advance_stride);
    }
    ঘূৰাই;
  }

#ifndef HB_NO_VAR
  যদি (!font->has_nonzero_coords)
  {
  fallback:
#অন্যথা
  {
#endif
    // Just plain vtmx data. No need to cache.
    বাবে (unsigned পূর্ণাঙ্ক i = 0; i < গুনতি; i++)
    {
      *first_advance = font->em_scale_y (- (পূর্ণাঙ্ক) vmtx.get_advance_without_var_unscaled (*first_glyph));
      first_glyph = &StructAtOffsetUnaligned<hb_codepoint_t> (first_glyph, glyph_stride);
      first_advance = &StructAtOffsetUnaligned<hb_position_t> (first_advance, advance_stride);
    }
    ঘূৰাই;
  }

#ifndef HB_NO_VAR
  /* has_nonzero_coords. */

  ot_font->check_serial (font);
  hb_ot_font_advance_cache_t *advance_cache = ot_font->v.acquire_advance_cache ();
  যদি (!advance_cache)
  {
    // malloc failure. Just use the fallback non-variable path.
    গচ্ছ fallback;
  }

  /* If VVAR is present, use it.*/
  ধ্রুৱ OT::VVAR &VVAR = *vmtx.var_table;
  যদি (VVAR.has_data ())
  {
    ধ্রুৱ OT::ItemVariationStore &varStore = &VVAR + VVAR.varStore;
    OT::hb_scalar_cache_t *varStore_cache = ot_font->v.acquire_varStore_cache (varStore);

    বাবে (unsigned পূর্ণাঙ্ক i = 0; i < গুনতি; i++)
    {
      hb_position_t v;
      unsigned cv;
      যদি (advance_cache->get (*first_glyph, &cv))
	v = cv;
      অন্যথা
      {
        v = vmtx.get_advance_with_var_unscaled (*first_glyph, font, varStore_cache);
	advance_cache->set (*first_glyph, v);
      }
      *first_advance = font->em_scale_y (- (পূর্ণাঙ্ক) v);
      first_glyph = &StructAtOffsetUnaligned<hb_codepoint_t> (first_glyph, glyph_stride);
      first_advance = &StructAtOffsetUnaligned<hb_position_t> (first_advance, advance_stride);
    }

    ot_font->v.release_varStore_cache (varStore_cache);
    ot_font->v.release_advance_cache (advance_cache);
    ঘূৰাই;
  }

  ধ্রুৱ auto &gvar = *ot_face->gvar;
  যদি (gvar.has_data ())
  {
    ধ্রুৱ auto &glyf = *ot_face->glyf;
    auto *scratch = glyf.acquire_scratch ();
    যদি (unlikely (!scratch))
    {
      ot_font->v.release_advance_cache (advance_cache);
      গচ্ছ fallback;
    }
    OT::hb_scalar_cache_t *gvar_cache = ot_font->draw.acquire_gvar_cache (gvar);

    বাবে (unsigned পূর্ণাঙ্ক i = 0; i < গুনতি; i++)
    {
      hb_position_t v;
      unsigned cv;
      যদি (advance_cache->get (*first_glyph, &cv))
	v = cv;
      অন্যথা
      {
        v = glyf.get_advance_with_var_unscaled (*first_glyph, font, সঁচা, *scratch, gvar_cache);
	advance_cache->set (*first_glyph, v);
      }
      *first_advance = font->em_scale_y (- (পূর্ণাঙ্ক) v);
      first_glyph = &StructAtOffsetUnaligned<hb_codepoint_t> (first_glyph, glyph_stride);
      first_advance = &StructAtOffsetUnaligned<hb_position_t> (first_advance, advance_stride);
    }

    ot_font->draw.release_gvar_cache (gvar_cache);
    glyf.release_scratch (scratch);
    ot_font->v.release_advance_cache (advance_cache);
    ঘূৰাই;
  }

  ot_font->v.release_advance_cache (advance_cache);
  // No VVAR or GVAR.  Just use the fallback non-variable path.
  গচ্ছ fallback;
#endif
}
#endif

#ifndef HB_NO_VERTICAL
HB_HOT
স্থির hb_bool_t
hb_ot_get_glyph_v_origins (hb_font_t *font,
			   শূন্য *font_data,
			   unsigned পূর্ণাঙ্ক গুনতি,
			   ধ্রুৱ hb_codepoint_t *first_glyph,
			   unsigned glyph_stride,
			   hb_position_t *first_x,
			   unsigned x_stride,
			   hb_position_t *first_y,
			   unsigned y_stride,
			   শূন্য *user_data HB_UNUSED)
{
  ধ্রুৱ hb_ot_font_t *ot_font = (ধ্রুৱ hb_ot_font_t *) font_data;
  ধ্রুৱ hb_ot_face_t *ot_face = ot_font->ot_face;

  ot_font->check_serial (font);

  /* First, set all the x values to half the advance width. */
  font->get_glyph_h_advances (গুনতি,
			      first_glyph, glyph_stride,
			      first_x, x_stride);
  বাবে (unsigned i = 0; i < গুনতি; i++)
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
  ধ্রুৱ OT::VORG &VORG = *ot_face->VORG;
  যদি (origin_cache && VORG.has_data ())
  {
#ifndef HB_NO_VAR
    যদি (!font->has_nonzero_coords)
#endif
    {
      বাবে (unsigned i = 0; i < গুনতি; i++)
      {
	hb_position_t origin;
	unsigned cv;
	যদি (origin_cache->get (*first_glyph, &cv))
	  origin = font->y_scale < 0 ? -static_cast<hb_position_t>(cv) : static_cast<hb_position_t>(cv);
	অন্যথা
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
    অন্যথা
    {
      ধ্রুৱ OT::VVAR &VVAR = *ot_face->vmtx->var_table;
      ধ্রুৱ auto &varStore = &VVAR + VVAR.varStore;
      auto *varStore_cache = ot_font->v_origin.acquire_varStore_cache (varStore);
      বাবে (unsigned i = 0; i < গুনতি; i++)
      {
	hb_position_t origin;
	unsigned cv;
	যদি (origin_cache->get (*first_glyph, &cv))
	  origin = font->y_scale < 0 ? -static_cast<hb_position_t>(cv) : static_cast<hb_position_t>(cv);
	অন্যথা
	{
	  origin = font->em_scalef_y (VORG.get_y_origin (*first_glyph) +
				      VVAR.get_vorg_delta_unscaled (*first_glyph,
								    font->coords, font->অঙ্ক_coords,
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
    ঘূৰাই সঁচা;
  }

  /* If and only if `vmtx` is present and it's a `glyf` font,
   * we use the top phantom point, deduced from vmtx,glyf[,gvar]. */
  ধ্রুৱ auto &vmtx = *ot_face->vmtx;
  ধ্রুৱ auto &glyf = *ot_face->glyf;
  যদি (origin_cache && vmtx.has_data() && glyf.has_data ())
  {
    auto *scratch = glyf.acquire_scratch ();
    যদি (unlikely (!scratch))
    {
      ot_font->v_origin.release_origin_cache (origin_cache);
      ঘূৰাই মিছা;
    }
    OT::hb_scalar_cache_t *gvar_cache = nullptr;
#ifndef HB_NO_VAR
    gvar_cache = font->has_nonzero_coords ?
					ot_font->draw.acquire_gvar_cache (*ot_face->gvar) :
					nullptr;
#endif

    বাবে (unsigned i = 0; i < গুনতি; i++)
    {
      hb_position_t origin;
      unsigned cv;
      যদি (origin_cache->get (*first_glyph, &cv))
	origin = font->y_scale < 0 ? -static_cast<hb_position_t>(cv) : static_cast<hb_position_t>(cv);
      অন্যথা
      {
	origin = font->em_scalef_y (glyf.get_v_origin_with_var_unscaled (*first_glyph, font, *scratch, gvar_cache));
	origin_cache->set (*first_glyph, font->y_scale < 0 ? -origin : origin);
      }

      *first_y = origin;

      first_glyph = &StructAtOffsetUnaligned<hb_codepoint_t> (first_glyph, glyph_stride);
      first_y = &StructAtOffsetUnaligned<hb_position_t> (first_y, y_stride);
    }

#ifndef HB_NO_VAR
    যদি (gvar_cache)
      ot_font->draw.release_gvar_cache (gvar_cache);
#endif
    glyf.release_scratch (scratch);
    ot_font->v_origin.release_origin_cache (origin_cache);
    ঘূৰাই সঁচা;
  }

  /* Otherwise, use glyph extents to center the glyph vertically.
   * If getting glyph extents failed, just use the font ascender. */
  যদি (origin_cache && font->has_glyph_extents_func ())
  {
    hb_font_extents_t font_extents;
    font->get_h_extents_with_fallback (&font_extents);
    hb_position_t font_advance = font_extents.ascender - font_extents.descender;

    বাবে (unsigned i = 0; i < গুনতি; i++)
    {
      hb_position_t origin;
      unsigned cv;

      যদি (origin_cache->get (*first_glyph, &cv))
	origin = font->y_scale < 0 ? -static_cast<hb_position_t>(cv) : static_cast<hb_position_t>(cv);
      অন্যথা
      {
	hb_glyph_extents_t extents = {0};
	যদি (likely (font->get_glyph_extents (*first_glyph, &extents)))
	  origin = hb_clamp_to<hb_position_t> ((int64_t) extents.y_bearing +
					 (((int64_t) font_advance + extents.height) >> 1));
	অন্যথা
	  origin = font_extents.ascender;

	origin_cache->set (*first_glyph, font->y_scale < 0 ? -origin : origin);
      }

      *first_y = origin;

      first_glyph = &StructAtOffsetUnaligned<hb_codepoint_t> (first_glyph, glyph_stride);
      first_y = &StructAtOffsetUnaligned<hb_position_t> (first_y, y_stride);
    }
  }

  ot_font->v_origin.release_origin_cache (origin_cache);
  ঘূৰাই সঁচা;
}
#endif

স্থির hb_bool_t
hb_ot_get_glyph_extents (hb_font_t *font,
			 শূন্য *font_data,
			 hb_codepoint_t glyph,
			 hb_glyph_extents_t *extents,
			 শূন্য *user_data HB_UNUSED)
{
  ধ্রুৱ hb_ot_font_t *ot_font = (ধ্রুৱ hb_ot_font_t *) font_data;
  ধ্রুৱ hb_ot_face_t *ot_face = ot_font->ot_face;

#যদি !defined(HB_NO_OT_FONT_BITMAP) && !defined(HB_NO_COLOR)
  যদি (ot_face->sbix->get_extents (font, glyph, extents)) ঘূৰাই সঁচা;
  যদি (ot_face->CBDT->get_extents (font, glyph, extents)) ঘূৰাই সঁচা;
#endif
#যদি !defined(HB_NO_COLOR) && !defined(HB_NO_PAINT)
  যদি (ot_face->COLR->get_extents (font, glyph, extents)) ঘূৰাই সঁচা;
#endif
#ifndef HB_NO_VAR_COMPOSITES
  যদি (ot_face->VARC->get_extents (font, glyph, extents)) ঘূৰাই সঁচা;
#endif
  যদি (ot_face->glyf->get_extents (font, glyph, extents)) ঘূৰাই সঁচা;
#ifndef HB_NO_OT_FONT_CFF
  যদি (ot_face->cff2->get_extents (font, glyph, extents)) ঘূৰাই সঁচা;
  যদি (ot_face->cff1->get_extents (font, glyph, extents)) ঘূৰাই সঁচা;
#endif

  ঘূৰাই মিছা;
}

#ifndef HB_NO_OT_FONT_GLYPH_NAMES
স্থির hb_bool_t
hb_ot_get_glyph_name (hb_font_t *font HB_UNUSED,
		      শূন্য *font_data,
		      hb_codepoint_t glyph,
		      ৱর্ণ *name, unsigned পূর্ণাঙ্ক size,
		      শূন্য *user_data HB_UNUSED)
{
  ধ্রুৱ hb_ot_font_t *ot_font = (ধ্রুৱ hb_ot_font_t *) font_data;
  ধ্রুৱ hb_ot_face_t *ot_face = ot_font->ot_face;

  যদি (ot_face->post->get_glyph_name (glyph, name, size)) ঘূৰাই সঁচা;
#ifndef HB_NO_OT_FONT_CFF
  যদি (ot_face->cff1->get_glyph_name (glyph, name, size)) ঘূৰাই সঁচা;
#endif
  ঘূৰাই মিছা;
}
স্থির hb_bool_t
hb_ot_get_glyph_from_name (hb_font_t *font HB_UNUSED,
			   শূন্য *font_data,
			   ধ্রুৱ ৱর্ণ *name, পূর্ণাঙ্ক len,
			   hb_codepoint_t *glyph,
			   শূন্য *user_data HB_UNUSED)
{
  ধ্রুৱ hb_ot_font_t *ot_font = (ধ্রুৱ hb_ot_font_t *) font_data;
  ধ্রুৱ hb_ot_face_t *ot_face = ot_font->ot_face;

  যদি (ot_face->post->get_glyph_from_name (name, len, glyph)) ঘূৰাই সঁচা;
#ifndef HB_NO_OT_FONT_CFF
    যদি (ot_face->cff1->get_glyph_from_name (name, len, glyph)) ঘূৰাই সঁচা;
#endif
  ঘূৰাই মিছা;
}
#endif

স্থির hb_bool_t
hb_ot_get_font_h_extents (hb_font_t *font,
			  শূন্য *font_data HB_UNUSED,
			  hb_font_extents_t *metrics,
			  শূন্য *user_data HB_UNUSED)
{
  ঘূৰাই _hb_ot_metrics_get_position_common (font, HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER, &metrics->ascender) &&
	 _hb_ot_metrics_get_position_common (font, HB_OT_METRICS_TAG_HORIZONTAL_DESCENDER, &metrics->descender) &&
	 _hb_ot_metrics_get_position_common (font, HB_OT_METRICS_TAG_HORIZONTAL_সাৰি_GAP, &metrics->সাৰি_gap);
}

#ifndef HB_NO_VERTICAL
স্থির hb_bool_t
hb_ot_get_font_v_extents (hb_font_t *font,
			  শূন্য *font_data HB_UNUSED,
			  hb_font_extents_t *metrics,
			  শূন্য *user_data HB_UNUSED)
{
  ঘূৰাই _hb_ot_metrics_get_position_common (font, HB_OT_METRICS_TAG_VERTICAL_ASCENDER, &metrics->ascender) &&
	 _hb_ot_metrics_get_position_common (font, HB_OT_METRICS_TAG_VERTICAL_DESCENDER, &metrics->descender) &&
	 _hb_ot_metrics_get_position_common (font, HB_OT_METRICS_TAG_VERTICAL_সাৰি_GAP, &metrics->সাৰি_gap);
}
#endif

#ifndef HB_NO_DRAW
স্থির hb_bool_t
hb_ot_draw_glyph_or_fail (hb_font_t *font,
			  শূন্য *font_data HB_UNUSED,
			  hb_codepoint_t glyph,
			  hb_draw_funcs_t *draw_funcs, শূন্য *draw_data,
			  শূন্য *user_data)
{
  ধ্রুৱ hb_ot_font_t *ot_font = (ধ্রুৱ hb_ot_font_t *) font_data;
  hb_draw_session_t draw_session {draw_funcs, draw_data};

  OT::hb_scalar_cache_t *gvar_cache = nullptr;
#ifndef HB_NO_VAR
  যদি (font->অঙ্ক_coords)
  {
    ot_font->check_serial (font);
    gvar_cache = ot_font->draw.acquire_gvar_cache (*ot_font->ot_face->gvar);
  }
#endif
  HB_SCOPE_GUARD (ot_font->draw.release_gvar_cache (gvar_cache));

#ifndef HB_NO_VAR_COMPOSITES
  যদি (font->face->table.VARC->get_path (font, glyph, draw_session)) ঘূৰাই সঁচা;
#endif
  // Keep the following in synch with VARC::get_path_at()
  যদি (font->face->table.glyf->get_path (font, glyph, draw_session, gvar_cache,
				       &draw_session.get_budget ())) ঘূৰাই সঁচা;

#ifndef HB_NO_CFF
  যদি (font->face->table.cff2->get_path (font, glyph, draw_session,
				       &draw_session.get_budget ())) ঘূৰাই সঁচা;
  যদি (font->face->table.cff1->get_path (font, glyph, draw_session,
				       &draw_session.get_budget ())) ঘূৰাই সঁচা;
#endif

  ঘূৰাই মিছা;
}
#endif

#ifndef HB_NO_PAINT
স্থির hb_bool_t
hb_ot_paint_glyph_or_fail (hb_font_t *font,
			   শূন্য *font_data,
			   hb_codepoint_t glyph,
			   hb_paint_funcs_t *paint_funcs, শূন্য *paint_data,
			   unsigned পূর্ণাঙ্ক palette,
			   hb_color_t foreground,
			   শূন্য *user_data)
{
#ifndef HB_NO_COLOR
  যদি (font->face->table.COLR->paint_glyph (font, glyph, paint_funcs, paint_data, palette, foreground)) ঘূৰাই সঁচা;
#ifndef HB_NO_SVG
  যদি (font->face->table.SVG->paint_glyph (font, glyph, paint_funcs, paint_data)) ঘূৰাই সঁচা;
#endif
#ifndef HB_NO_OT_FONT_BITMAP
  যদি (font->face->table.CBDT->paint_glyph (font, glyph, paint_funcs, paint_data)) ঘূৰাই সঁচা;
  যদি (font->face->table.sbix->paint_glyph (font, glyph, paint_funcs, paint_data)) ঘূৰাই সঁচা;
#endif
#endif
  ঘূৰাই মিছা;
}
#endif

স্থির অন্তঃস্থ শূন্য free_static_ot_funcs ();

স্থির সংরচনা hb_ot_font_funcs_lazy_loader_t : hb_font_funcs_lazy_loader_t<hb_ot_font_funcs_lazy_loader_t>
{
  স্থির hb_font_funcs_t *create ()
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

    ঘূৰাই funcs;
  }
} static_ot_funcs;

স্থির অন্তঃস্থ
শূন্য free_static_ot_funcs ()
{
  static_ot_funcs.free_instance ();
}

স্থির hb_font_funcs_t *
_hb_ot_get_font_funcs ()
{
  ঘূৰাই static_ot_funcs.get_unconst ();
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
শূন্য
hb_ot_font_set_funcs (hb_font_t *font)
{
  hb_ot_font_t *ot_font = _hb_ot_font_create (font);
  যদি (unlikely (!ot_font))
    ঘূৰাই;

  hb_font_set_funcs (font,
		     _hb_ot_get_font_funcs (),
		     ot_font,
		     _hb_ot_font_destroy);
}

#endif
