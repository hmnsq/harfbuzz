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

#അന്തര്ഭാവ "hb.hh"

#ifndef HB_NO_OT_FONT

#അന്തര്ഭാവ "hb-ot.h"

#അന്തര്ഭാവ "hb-cache.hh"
#അന്തര്ഭാവ "hb-font.hh"
#അന്തര്ഭാവ "hb-machinery.hh"
#അന്തര്ഭാവ "hb-ot-face.hh"

#അന്തര്ഭാവ "hb-ot-cmap-table.hh"
#അന്തര്ഭാവ "hb-ot-glyf-table.hh"
#അന്തര്ഭാവ "hb-ot-var-gvar-table.hh"
#അന്തര്ഭാവ "hb-ot-cff2-table.hh"
#അന്തര്ഭാവ "hb-ot-cff1-table.hh"
#അന്തര്ഭാവ "hb-ot-hmtx-table.hh"
#അന്തര്ഭാവ "hb-ot-post-table.hh"
#അന്തര്ഭാവ "hb-ot-stat-table.hh"
#അന്തര്ഭാവ "hb-ot-var-varc-table.hh"
#അന്തര്ഭാവ "hb-ot-vorg-table.hh"
#അന്തര്ഭാവ "OT/Color/CBDT/CBDT.hh"
#അന്തര്ഭാവ "OT/Color/COLR/COLR.hh"
#അന്തര്ഭാവ "OT/Color/sbix/sbix.hh"
#അന്തര്ഭാവ "OT/Color/svg/svg.hh"


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
static_assert (വലുപ്പം (hb_ot_font_advance_cache_t) == 1024, "");

using hb_ot_font_origin_cache_t = hb_cache_t<20, 20>;
static_assert (വലുപ്പം (hb_ot_font_origin_cache_t) == 1024, "");

ഘടന hb_ot_font_t
{
  സ്ഥിരം hb_ot_face_t *ot_face;

  mutable hb_atomic_t<പൂർണ്ണസംഖ്യ> cached_serial;
  mutable hb_atomic_t<പൂർണ്ണസംഖ്യ> cached_coords_serial;

  ഘടന direction_cache_t
  {
    mutable hb_atomic_t<hb_ot_font_advance_cache_t *> advance_cache;
    mutable hb_atomic_t<OT::hb_scalar_cache_t *> varStore_cache;

    ~direction_cache_t ()
    {
      clear ();
    }

    hb_ot_font_advance_cache_t *acquire_advance_cache () സ്ഥിരം
    {
    retry:
      auto *cache = advance_cache.get_acquire ();
      എങ്കിൽ (!cache)
      {
        cache = (hb_ot_font_advance_cache_t *) hb_malloc (വലുപ്പം (hb_ot_font_advance_cache_t));
	എങ്കിൽ (!cache)
	  തിരികെ nullptr;
	പുതിയ (cache) hb_ot_font_advance_cache_t;
	തിരികെ cache;
      }
      എങ്കിൽ (advance_cache.cmpexch (cache, nullptr))
        തിരികെ cache;
      അല്ലെങ്കിൽ
        പോകുക retry;
    }
    ശൂന്യം release_advance_cache (hb_ot_font_advance_cache_t *cache) സ്ഥിരം
    {
      എങ്കിൽ (!cache)
        തിരികെ;
      എങ്കിൽ (!advance_cache.cmpexch (nullptr, cache))
        hb_free (cache);
    }
    ശൂന്യം clear_advance_cache () സ്ഥിരം
    {
    retry:
      auto *cache = advance_cache.get_acquire ();
      എങ്കിൽ (!cache)
	തിരികെ;
      എങ്കിൽ (advance_cache.cmpexch (cache, nullptr))
	hb_free (cache);
      അല്ലെങ്കിൽ
        പോകുക retry;
    }

    OT::hb_scalar_cache_t *acquire_varStore_cache (സ്ഥിരം OT::ItemVariationStore &varStore) സ്ഥിരം
    {
    retry:
      auto *cache = varStore_cache.get_acquire ();
      എങ്കിൽ (!cache)
	തിരികെ varStore.create_cache ();
      എങ്കിൽ (varStore_cache.cmpexch (cache, nullptr))
	തിരികെ cache;
      അല്ലെങ്കിൽ
	പോകുക retry;
    }
    ശൂന്യം release_varStore_cache (OT::hb_scalar_cache_t *cache) സ്ഥിരം
    {
      എങ്കിൽ (!cache)
	തിരികെ;
      എങ്കിൽ (!varStore_cache.cmpexch (nullptr, cache))
	OT::ItemVariationStore::destroy_cache (cache);
    }
    ശൂന്യം clear_varStore_cache () സ്ഥിരം
    {
    retry:
      auto *cache = varStore_cache.get_acquire ();
      എങ്കിൽ (!cache)
	തിരികെ;
      എങ്കിൽ (varStore_cache.cmpexch (cache, nullptr))
	OT::ItemVariationStore::destroy_cache (cache);
      അല്ലെങ്കിൽ
	പോകുക retry;
    }

    ശൂന്യം clear () സ്ഥിരം
    {
      clear_advance_cache ();
      clear_varStore_cache ();
    }

  } h, v;

  ഘടന origin_cache_t
  {
    mutable hb_atomic_t<hb_ot_font_origin_cache_t *> origin_cache;
    mutable hb_atomic_t<OT::hb_scalar_cache_t *> varStore_cache;

    ~origin_cache_t ()
    {
      clear ();
    }

    hb_ot_font_origin_cache_t *acquire_origin_cache () സ്ഥിരം
    {
    retry:
      auto *cache = origin_cache.get_acquire ();
      എങ്കിൽ (!cache)
      {
        cache = (hb_ot_font_origin_cache_t *) hb_malloc (വലുപ്പം (hb_ot_font_origin_cache_t));
	എങ്കിൽ (!cache)
	  തിരികെ nullptr;
	പുതിയ (cache) hb_ot_font_origin_cache_t;
	തിരികെ cache;
      }
      എങ്കിൽ (origin_cache.cmpexch (cache, nullptr))
        തിരികെ cache;
      അല്ലെങ്കിൽ
        പോകുക retry;
    }
    ശൂന്യം release_origin_cache (hb_ot_font_origin_cache_t *cache) സ്ഥിരം
    {
      എങ്കിൽ (!cache)
        തിരികെ;
      എങ്കിൽ (!origin_cache.cmpexch (nullptr, cache))
        hb_free (cache);
    }
    ശൂന്യം clear_origin_cache () സ്ഥിരം
    {
    retry:
      auto *cache = origin_cache.get_acquire ();
      എങ്കിൽ (!cache)
	തിരികെ;
      എങ്കിൽ (origin_cache.cmpexch (cache, nullptr))
	hb_free (cache);
      അല്ലെങ്കിൽ
        പോകുക retry;
    }

    OT::hb_scalar_cache_t *acquire_varStore_cache (സ്ഥിരം OT::ItemVariationStore &varStore) സ്ഥിരം
    {
    retry:
      auto *cache = varStore_cache.get_acquire ();
      എങ്കിൽ (!cache)
	തിരികെ varStore.create_cache ();
      എങ്കിൽ (varStore_cache.cmpexch (cache, nullptr))
	തിരികെ cache;
      അല്ലെങ്കിൽ
	പോകുക retry;
    }
    ശൂന്യം release_varStore_cache (OT::hb_scalar_cache_t *cache) സ്ഥിരം
    {
      എങ്കിൽ (!cache)
	തിരികെ;
      എങ്കിൽ (!varStore_cache.cmpexch (nullptr, cache))
	OT::ItemVariationStore::destroy_cache (cache);
    }
    ശൂന്യം clear_varStore_cache () സ്ഥിരം
    {
    retry:
      auto *cache = varStore_cache.get_acquire ();
      എങ്കിൽ (!cache)
	തിരികെ;
      എങ്കിൽ (varStore_cache.cmpexch (cache, nullptr))
	OT::ItemVariationStore::destroy_cache (cache);
      അല്ലെങ്കിൽ
	പോകുക retry;
    }

    ശൂന്യം clear () സ്ഥിരം
    {
      clear_origin_cache ();
      clear_varStore_cache ();
    }
  } v_origin;

  ഘടന draw_cache_t
  {
    mutable hb_atomic_t<OT::hb_scalar_cache_t *> gvar_cache;

    ~draw_cache_t ()
    {
      clear ();
    }

    OT::hb_scalar_cache_t *acquire_gvar_cache (സ്ഥിരം OT::gvar_accelerator_t &gvar) സ്ഥിരം
    {
    retry:
      auto *cache = gvar_cache.get_acquire ();
      എങ്കിൽ (!cache)
	തിരികെ gvar.create_cache ();
      എങ്കിൽ (gvar_cache.cmpexch (cache, nullptr))
	തിരികെ cache;
      അല്ലെങ്കിൽ
	പോകുക retry;
    }
    ശൂന്യം release_gvar_cache (OT::hb_scalar_cache_t *cache) സ്ഥിരം
    {
      എങ്കിൽ (!cache)
	തിരികെ;
      എങ്കിൽ (!gvar_cache.cmpexch (nullptr, cache))
	OT::gvar_accelerator_t::destroy_cache (cache);
    }
    ശൂന്യം clear_gvar_cache () സ്ഥിരം
    {
    retry:
      auto *cache = gvar_cache.get_acquire ();
      എങ്കിൽ (!cache)
	തിരികെ;
      എങ്കിൽ (gvar_cache.cmpexch (cache, nullptr))
	OT::gvar_accelerator_t::destroy_cache (cache);
      അല്ലെങ്കിൽ
	പോകുക retry;
    }

    ശൂന്യം clear () സ്ഥിരം
    {
      clear_gvar_cache ();
    }
  } draw;

  ശൂന്യം check_serial (hb_font_t *font) സ്ഥിരം
  {
    പൂർണ്ണസംഖ്യ font_serial = font->serial.get_acquire ();
    എങ്കിൽ (cached_serial.get_acquire () != font_serial)
    {
      /* These caches are dependent on scale and synthetic settings.
       * Any change to the font invalidates them. */
      v_origin.clear ();

      cached_serial.set_release (font_serial);
    }

    പൂർണ്ണസംഖ്യ font_serial_coords = font->serial_coords.get_acquire ();
    എങ്കിൽ (cached_coords_serial.get_acquire () != font_serial_coords)
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

സ്ഥിതം hb_ot_font_t *
_hb_ot_font_create (hb_font_t *font)
{
  hb_ot_font_t *ot_font = (hb_ot_font_t *) hb_calloc (1, വലുപ്പം (hb_ot_font_t));
  എങ്കിൽ (unlikely (!ot_font))
    തിരികെ nullptr;

  ot_font->ot_face = &font->face->table;

  തിരികെ ot_font;
}

സ്ഥിതം ശൂന്യം
_hb_ot_font_destroy (ശൂന്യം *font_data)
{
  hb_ot_font_t *ot_font = (hb_ot_font_t *) font_data;

  ot_font->~hb_ot_font_t ();

  hb_free (ot_font);
}

സ്ഥിതം hb_bool_t
hb_ot_get_nominal_glyph (hb_font_t *font HB_UNUSED,
			 ശൂന്യം *font_data,
			 hb_codepoint_t unicode,
			 hb_codepoint_t *glyph,
			 ശൂന്യം *user_data HB_UNUSED)
{
  സ്ഥിരം hb_ot_font_t *ot_font = (സ്ഥിരം hb_ot_font_t *) font_data;
  സ്ഥിരം hb_ot_face_t *ot_face = ot_font->ot_face;
  തിരികെ ot_face->cmap->get_nominal_glyph (unicode, glyph);
}

സ്ഥിതം unsigned പൂർണ്ണസംഖ്യ
hb_ot_get_nominal_glyphs (hb_font_t *font HB_UNUSED,
			  ശൂന്യം *font_data,
			  unsigned പൂർണ്ണസംഖ്യ എണ്ണം,
			  സ്ഥിരം hb_codepoint_t *first_unicode,
			  unsigned പൂർണ്ണസംഖ്യ unicode_stride,
			  hb_codepoint_t *first_glyph,
			  unsigned പൂർണ്ണസംഖ്യ glyph_stride,
			  ശൂന്യം *user_data HB_UNUSED)
{
  സ്ഥിരം hb_ot_font_t *ot_font = (സ്ഥിരം hb_ot_font_t *) font_data;
  സ്ഥിരം hb_ot_face_t *ot_face = ot_font->ot_face;
  തിരികെ ot_face->cmap->get_nominal_glyphs (എണ്ണം,
					    first_unicode, unicode_stride,
					    first_glyph, glyph_stride);
}

സ്ഥിതം hb_bool_t
hb_ot_get_variation_glyph (hb_font_t *font HB_UNUSED,
			   ശൂന്യം *font_data,
			   hb_codepoint_t unicode,
			   hb_codepoint_t variation_selector,
			   hb_codepoint_t *glyph,
			   ശൂന്യം *user_data HB_UNUSED)
{
  സ്ഥിരം hb_ot_font_t *ot_font = (സ്ഥിരം hb_ot_font_t *) font_data;
  സ്ഥിരം hb_ot_face_t *ot_face = ot_font->ot_face;
  തിരികെ ot_face->cmap->get_variation_glyph (unicode,
                                             variation_selector, glyph);
}

സ്ഥിതം ശൂന്യം
hb_ot_get_glyph_h_advances (hb_font_t* font, ശൂന്യം* font_data,
			    unsigned എണ്ണം,
			    സ്ഥിരം hb_codepoint_t *first_glyph,
			    unsigned glyph_stride,
			    hb_position_t *first_advance,
			    unsigned advance_stride,
			    ശൂന്യം *user_data HB_UNUSED)
{
  // Duplicated in v_advances. Ugly. Keep in sync'ish.

  സ്ഥിരം hb_ot_font_t *ot_font = (സ്ഥിരം hb_ot_font_t *) font_data;
  സ്ഥിരം hb_ot_face_t *ot_face = ot_font->ot_face;
  സ്ഥിരം OT::hmtx_accelerator_t &hmtx = *ot_face->hmtx;

  എങ്കിൽ (unlikely (!hmtx.has_data ()))
  {
    hb_position_t advance = font->face->get_upem () / 2;
    advance = font->em_scale_x (advance);
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < എണ്ണം; i++)
    {
      *first_advance = advance;
      first_advance = &StructAtOffsetUnaligned<hb_position_t> (first_advance, advance_stride);
    }
    തിരികെ;
  }

#ifndef HB_NO_VAR
  എങ്കിൽ (!font->has_nonzero_coords)
  {
  fallback:
#അല്ലെങ്കിൽ
  {
#endif
    // Just plain htmx data. No need to cache.
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < എണ്ണം; i++)
    {
      *first_advance = font->em_scale_x (hmtx.get_advance_without_var_unscaled (*first_glyph));
      first_glyph = &StructAtOffsetUnaligned<hb_codepoint_t> (first_glyph, glyph_stride);
      first_advance = &StructAtOffsetUnaligned<hb_position_t> (first_advance, advance_stride);
    }
    തിരികെ;
  }

#ifndef HB_NO_VAR
  /* has_nonzero_coords. */

  ot_font->check_serial (font);
  hb_ot_font_advance_cache_t *advance_cache = ot_font->h.acquire_advance_cache ();
  എങ്കിൽ (!advance_cache)
  {
    // malloc failure. Just use the fallback non-variable path.
    പോകുക fallback;
  }

  /* If HVAR is present, use it.*/
  സ്ഥിരം OT::HVAR &HVAR = *hmtx.var_table;
  എങ്കിൽ (HVAR.has_data ())
  {
    സ്ഥിരം OT::ItemVariationStore &varStore = &HVAR + HVAR.varStore;
    OT::hb_scalar_cache_t *varStore_cache = ot_font->h.acquire_varStore_cache (varStore);

    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < എണ്ണം; i++)
    {
      hb_position_t v;
      unsigned cv;
      എങ്കിൽ (advance_cache->get (*first_glyph, &cv))
	v = cv;
      അല്ലെങ്കിൽ
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
    തിരികെ;
  }

  സ്ഥിരം auto &gvar = *ot_face->gvar;
  എങ്കിൽ (gvar.has_data ())
  {
    സ്ഥിരം auto &glyf = *ot_face->glyf;
    auto *scratch = glyf.acquire_scratch ();
    എങ്കിൽ (unlikely (!scratch))
    {
      ot_font->h.release_advance_cache (advance_cache);
      പോകുക fallback;
    }
    OT::hb_scalar_cache_t *gvar_cache = ot_font->draw.acquire_gvar_cache (gvar);

    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < എണ്ണം; i++)
    {
      hb_position_t v;
      unsigned cv;
      എങ്കിൽ (advance_cache->get (*first_glyph, &cv))
	v = cv;
      അല്ലെങ്കിൽ
      {
        v = glyf.get_advance_with_var_unscaled (*first_glyph, font, അസത്യം, *scratch, gvar_cache);
	advance_cache->set (*first_glyph, v);
      }
      *first_advance = font->em_scale_x (v);
      first_glyph = &StructAtOffsetUnaligned<hb_codepoint_t> (first_glyph, glyph_stride);
      first_advance = &StructAtOffsetUnaligned<hb_position_t> (first_advance, advance_stride);
    }

    ot_font->draw.release_gvar_cache (gvar_cache);
    glyf.release_scratch (scratch);
    ot_font->h.release_advance_cache (advance_cache);
    തിരികെ;
  }

  ot_font->h.release_advance_cache (advance_cache);
  // No HVAR or GVAR.  Just use the fallback non-variable path.
  പോകുക fallback;
#endif
}

#ifndef HB_NO_VERTICAL
സ്ഥിതം ശൂന്യം
hb_ot_get_glyph_v_advances (hb_font_t* font, ശൂന്യം* font_data,
			    unsigned എണ്ണം,
			    സ്ഥിരം hb_codepoint_t *first_glyph,
			    unsigned glyph_stride,
			    hb_position_t *first_advance,
			    unsigned advance_stride,
			    ശൂന്യം *user_data HB_UNUSED)
{
  // Duplicated from h_advances. Ugly. Keep in sync'ish.

  സ്ഥിരം hb_ot_font_t *ot_font = (സ്ഥിരം hb_ot_font_t *) font_data;
  സ്ഥിരം hb_ot_face_t *ot_face = ot_font->ot_face;
  സ്ഥിരം OT::vmtx_accelerator_t &vmtx = *ot_face->vmtx;

  എങ്കിൽ (unlikely (!vmtx.has_data ()))
  {
    hb_font_extents_t font_extents;
    font->get_h_extents_with_fallback (&font_extents);
    hb_position_t advance = font_extents.descender - font_extents.ascender;
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < എണ്ണം; i++)
    {
      *first_advance = advance;
      first_advance = &StructAtOffsetUnaligned<hb_position_t> (first_advance, advance_stride);
    }
    തിരികെ;
  }

#ifndef HB_NO_VAR
  എങ്കിൽ (!font->has_nonzero_coords)
  {
  fallback:
#അല്ലെങ്കിൽ
  {
#endif
    // Just plain vtmx data. No need to cache.
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < എണ്ണം; i++)
    {
      *first_advance = font->em_scale_y (- (പൂർണ്ണസംഖ്യ) vmtx.get_advance_without_var_unscaled (*first_glyph));
      first_glyph = &StructAtOffsetUnaligned<hb_codepoint_t> (first_glyph, glyph_stride);
      first_advance = &StructAtOffsetUnaligned<hb_position_t> (first_advance, advance_stride);
    }
    തിരികെ;
  }

#ifndef HB_NO_VAR
  /* has_nonzero_coords. */

  ot_font->check_serial (font);
  hb_ot_font_advance_cache_t *advance_cache = ot_font->v.acquire_advance_cache ();
  എങ്കിൽ (!advance_cache)
  {
    // malloc failure. Just use the fallback non-variable path.
    പോകുക fallback;
  }

  /* If VVAR is present, use it.*/
  സ്ഥിരം OT::VVAR &VVAR = *vmtx.var_table;
  എങ്കിൽ (VVAR.has_data ())
  {
    സ്ഥിരം OT::ItemVariationStore &varStore = &VVAR + VVAR.varStore;
    OT::hb_scalar_cache_t *varStore_cache = ot_font->v.acquire_varStore_cache (varStore);

    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < എണ്ണം; i++)
    {
      hb_position_t v;
      unsigned cv;
      എങ്കിൽ (advance_cache->get (*first_glyph, &cv))
	v = cv;
      അല്ലെങ്കിൽ
      {
        v = vmtx.get_advance_with_var_unscaled (*first_glyph, font, varStore_cache);
	advance_cache->set (*first_glyph, v);
      }
      *first_advance = font->em_scale_y (- (പൂർണ്ണസംഖ്യ) v);
      first_glyph = &StructAtOffsetUnaligned<hb_codepoint_t> (first_glyph, glyph_stride);
      first_advance = &StructAtOffsetUnaligned<hb_position_t> (first_advance, advance_stride);
    }

    ot_font->v.release_varStore_cache (varStore_cache);
    ot_font->v.release_advance_cache (advance_cache);
    തിരികെ;
  }

  സ്ഥിരം auto &gvar = *ot_face->gvar;
  എങ്കിൽ (gvar.has_data ())
  {
    സ്ഥിരം auto &glyf = *ot_face->glyf;
    auto *scratch = glyf.acquire_scratch ();
    എങ്കിൽ (unlikely (!scratch))
    {
      ot_font->v.release_advance_cache (advance_cache);
      പോകുക fallback;
    }
    OT::hb_scalar_cache_t *gvar_cache = ot_font->draw.acquire_gvar_cache (gvar);

    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < എണ്ണം; i++)
    {
      hb_position_t v;
      unsigned cv;
      എങ്കിൽ (advance_cache->get (*first_glyph, &cv))
	v = cv;
      അല്ലെങ്കിൽ
      {
        v = glyf.get_advance_with_var_unscaled (*first_glyph, font, സത്യം, *scratch, gvar_cache);
	advance_cache->set (*first_glyph, v);
      }
      *first_advance = font->em_scale_y (- (പൂർണ്ണസംഖ്യ) v);
      first_glyph = &StructAtOffsetUnaligned<hb_codepoint_t> (first_glyph, glyph_stride);
      first_advance = &StructAtOffsetUnaligned<hb_position_t> (first_advance, advance_stride);
    }

    ot_font->draw.release_gvar_cache (gvar_cache);
    glyf.release_scratch (scratch);
    ot_font->v.release_advance_cache (advance_cache);
    തിരികെ;
  }

  ot_font->v.release_advance_cache (advance_cache);
  // No VVAR or GVAR.  Just use the fallback non-variable path.
  പോകുക fallback;
#endif
}
#endif

#ifndef HB_NO_VERTICAL
HB_HOT
സ്ഥിതം hb_bool_t
hb_ot_get_glyph_v_origins (hb_font_t *font,
			   ശൂന്യം *font_data,
			   unsigned പൂർണ്ണസംഖ്യ എണ്ണം,
			   സ്ഥിരം hb_codepoint_t *first_glyph,
			   unsigned glyph_stride,
			   hb_position_t *first_x,
			   unsigned x_stride,
			   hb_position_t *first_y,
			   unsigned y_stride,
			   ശൂന്യം *user_data HB_UNUSED)
{
  സ്ഥിരം hb_ot_font_t *ot_font = (സ്ഥിരം hb_ot_font_t *) font_data;
  സ്ഥിരം hb_ot_face_t *ot_face = ot_font->ot_face;

  ot_font->check_serial (font);

  /* First, set all the x values to half the advance width. */
  font->get_glyph_h_advances (എണ്ണം,
			      first_glyph, glyph_stride,
			      first_x, x_stride);
  വേണ്ടി (unsigned i = 0; i < എണ്ണം; i++)
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
  സ്ഥിരം OT::VORG &VORG = *ot_face->VORG;
  എങ്കിൽ (origin_cache && VORG.has_data ())
  {
#ifndef HB_NO_VAR
    എങ്കിൽ (!font->has_nonzero_coords)
#endif
    {
      വേണ്ടി (unsigned i = 0; i < എണ്ണം; i++)
      {
	hb_position_t origin;
	unsigned cv;
	എങ്കിൽ (origin_cache->get (*first_glyph, &cv))
	  origin = font->y_scale < 0 ? -static_cast<hb_position_t>(cv) : static_cast<hb_position_t>(cv);
	അല്ലെങ്കിൽ
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
    അല്ലെങ്കിൽ
    {
      സ്ഥിരം OT::VVAR &VVAR = *ot_face->vmtx->var_table;
      സ്ഥിരം auto &varStore = &VVAR + VVAR.varStore;
      auto *varStore_cache = ot_font->v_origin.acquire_varStore_cache (varStore);
      വേണ്ടി (unsigned i = 0; i < എണ്ണം; i++)
      {
	hb_position_t origin;
	unsigned cv;
	എങ്കിൽ (origin_cache->get (*first_glyph, &cv))
	  origin = font->y_scale < 0 ? -static_cast<hb_position_t>(cv) : static_cast<hb_position_t>(cv);
	അല്ലെങ്കിൽ
	{
	  origin = font->em_scalef_y (VORG.get_y_origin (*first_glyph) +
				      VVAR.get_vorg_delta_unscaled (*first_glyph,
								    font->coords, font->അക്കം_coords,
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
    തിരികെ സത്യം;
  }

  /* If and only if `vmtx` is present and it's a `glyf` font,
   * we use the top phantom point, deduced from vmtx,glyf[,gvar]. */
  സ്ഥിരം auto &vmtx = *ot_face->vmtx;
  സ്ഥിരം auto &glyf = *ot_face->glyf;
  എങ്കിൽ (origin_cache && vmtx.has_data() && glyf.has_data ())
  {
    auto *scratch = glyf.acquire_scratch ();
    എങ്കിൽ (unlikely (!scratch))
    {
      ot_font->v_origin.release_origin_cache (origin_cache);
      തിരികെ അസത്യം;
    }
    OT::hb_scalar_cache_t *gvar_cache = nullptr;
#ifndef HB_NO_VAR
    gvar_cache = font->has_nonzero_coords ?
					ot_font->draw.acquire_gvar_cache (*ot_face->gvar) :
					nullptr;
#endif

    വേണ്ടി (unsigned i = 0; i < എണ്ണം; i++)
    {
      hb_position_t origin;
      unsigned cv;
      എങ്കിൽ (origin_cache->get (*first_glyph, &cv))
	origin = font->y_scale < 0 ? -static_cast<hb_position_t>(cv) : static_cast<hb_position_t>(cv);
      അല്ലെങ്കിൽ
      {
	origin = font->em_scalef_y (glyf.get_v_origin_with_var_unscaled (*first_glyph, font, *scratch, gvar_cache));
	origin_cache->set (*first_glyph, font->y_scale < 0 ? -origin : origin);
      }

      *first_y = origin;

      first_glyph = &StructAtOffsetUnaligned<hb_codepoint_t> (first_glyph, glyph_stride);
      first_y = &StructAtOffsetUnaligned<hb_position_t> (first_y, y_stride);
    }

#ifndef HB_NO_VAR
    എങ്കിൽ (gvar_cache)
      ot_font->draw.release_gvar_cache (gvar_cache);
#endif
    glyf.release_scratch (scratch);
    ot_font->v_origin.release_origin_cache (origin_cache);
    തിരികെ സത്യം;
  }

  /* Otherwise, use glyph extents to center the glyph vertically.
   * If getting glyph extents failed, just use the font ascender. */
  എങ്കിൽ (origin_cache && font->has_glyph_extents_func ())
  {
    hb_font_extents_t font_extents;
    font->get_h_extents_with_fallback (&font_extents);
    hb_position_t font_advance = font_extents.ascender - font_extents.descender;

    വേണ്ടി (unsigned i = 0; i < എണ്ണം; i++)
    {
      hb_position_t origin;
      unsigned cv;

      എങ്കിൽ (origin_cache->get (*first_glyph, &cv))
	origin = font->y_scale < 0 ? -static_cast<hb_position_t>(cv) : static_cast<hb_position_t>(cv);
      അല്ലെങ്കിൽ
      {
	hb_glyph_extents_t extents = {0};
	എങ്കിൽ (likely (font->get_glyph_extents (*first_glyph, &extents)))
	  origin = hb_clamp_to<hb_position_t> ((int64_t) extents.y_bearing +
					 (((int64_t) font_advance + extents.height) >> 1));
	അല്ലെങ്കിൽ
	  origin = font_extents.ascender;

	origin_cache->set (*first_glyph, font->y_scale < 0 ? -origin : origin);
      }

      *first_y = origin;

      first_glyph = &StructAtOffsetUnaligned<hb_codepoint_t> (first_glyph, glyph_stride);
      first_y = &StructAtOffsetUnaligned<hb_position_t> (first_y, y_stride);
    }
  }

  ot_font->v_origin.release_origin_cache (origin_cache);
  തിരികെ സത്യം;
}
#endif

സ്ഥിതം hb_bool_t
hb_ot_get_glyph_extents (hb_font_t *font,
			 ശൂന്യം *font_data,
			 hb_codepoint_t glyph,
			 hb_glyph_extents_t *extents,
			 ശൂന്യം *user_data HB_UNUSED)
{
  സ്ഥിരം hb_ot_font_t *ot_font = (സ്ഥിരം hb_ot_font_t *) font_data;
  സ്ഥിരം hb_ot_face_t *ot_face = ot_font->ot_face;

#എങ്കിൽ !defined(HB_NO_OT_FONT_BITMAP) && !defined(HB_NO_COLOR)
  എങ്കിൽ (ot_face->sbix->get_extents (font, glyph, extents)) തിരികെ സത്യം;
  എങ്കിൽ (ot_face->CBDT->get_extents (font, glyph, extents)) തിരികെ സത്യം;
#endif
#എങ്കിൽ !defined(HB_NO_COLOR) && !defined(HB_NO_PAINT)
  എങ്കിൽ (ot_face->COLR->get_extents (font, glyph, extents)) തിരികെ സത്യം;
#endif
#ifndef HB_NO_VAR_COMPOSITES
  എങ്കിൽ (ot_face->VARC->get_extents (font, glyph, extents)) തിരികെ സത്യം;
#endif
  എങ്കിൽ (ot_face->glyf->get_extents (font, glyph, extents)) തിരികെ സത്യം;
#ifndef HB_NO_OT_FONT_CFF
  എങ്കിൽ (ot_face->cff2->get_extents (font, glyph, extents)) തിരികെ സത്യം;
  എങ്കിൽ (ot_face->cff1->get_extents (font, glyph, extents)) തിരികെ സത്യം;
#endif

  തിരികെ അസത്യം;
}

#ifndef HB_NO_OT_FONT_GLYPH_NAMES
സ്ഥിതം hb_bool_t
hb_ot_get_glyph_name (hb_font_t *font HB_UNUSED,
		      ശൂന്യം *font_data,
		      hb_codepoint_t glyph,
		      അക്ഷരം *name, unsigned പൂർണ്ണസംഖ്യ size,
		      ശൂന്യം *user_data HB_UNUSED)
{
  സ്ഥിരം hb_ot_font_t *ot_font = (സ്ഥിരം hb_ot_font_t *) font_data;
  സ്ഥിരം hb_ot_face_t *ot_face = ot_font->ot_face;

  എങ്കിൽ (ot_face->post->get_glyph_name (glyph, name, size)) തിരികെ സത്യം;
#ifndef HB_NO_OT_FONT_CFF
  എങ്കിൽ (ot_face->cff1->get_glyph_name (glyph, name, size)) തിരികെ സത്യം;
#endif
  തിരികെ അസത്യം;
}
സ്ഥിതം hb_bool_t
hb_ot_get_glyph_from_name (hb_font_t *font HB_UNUSED,
			   ശൂന്യം *font_data,
			   സ്ഥിരം അക്ഷരം *name, പൂർണ്ണസംഖ്യ len,
			   hb_codepoint_t *glyph,
			   ശൂന്യം *user_data HB_UNUSED)
{
  സ്ഥിരം hb_ot_font_t *ot_font = (സ്ഥിരം hb_ot_font_t *) font_data;
  സ്ഥിരം hb_ot_face_t *ot_face = ot_font->ot_face;

  എങ്കിൽ (ot_face->post->get_glyph_from_name (name, len, glyph)) തിരികെ സത്യം;
#ifndef HB_NO_OT_FONT_CFF
    എങ്കിൽ (ot_face->cff1->get_glyph_from_name (name, len, glyph)) തിരികെ സത്യം;
#endif
  തിരികെ അസത്യം;
}
#endif

സ്ഥിതം hb_bool_t
hb_ot_get_font_h_extents (hb_font_t *font,
			  ശൂന്യം *font_data HB_UNUSED,
			  hb_font_extents_t *metrics,
			  ശൂന്യം *user_data HB_UNUSED)
{
  തിരികെ _hb_ot_metrics_get_position_common (font, HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER, &metrics->ascender) &&
	 _hb_ot_metrics_get_position_common (font, HB_OT_METRICS_TAG_HORIZONTAL_DESCENDER, &metrics->descender) &&
	 _hb_ot_metrics_get_position_common (font, HB_OT_METRICS_TAG_HORIZONTAL_വരി_GAP, &metrics->വരി_gap);
}

#ifndef HB_NO_VERTICAL
സ്ഥിതം hb_bool_t
hb_ot_get_font_v_extents (hb_font_t *font,
			  ശൂന്യം *font_data HB_UNUSED,
			  hb_font_extents_t *metrics,
			  ശൂന്യം *user_data HB_UNUSED)
{
  തിരികെ _hb_ot_metrics_get_position_common (font, HB_OT_METRICS_TAG_VERTICAL_ASCENDER, &metrics->ascender) &&
	 _hb_ot_metrics_get_position_common (font, HB_OT_METRICS_TAG_VERTICAL_DESCENDER, &metrics->descender) &&
	 _hb_ot_metrics_get_position_common (font, HB_OT_METRICS_TAG_VERTICAL_വരി_GAP, &metrics->വരി_gap);
}
#endif

#ifndef HB_NO_DRAW
സ്ഥിതം hb_bool_t
hb_ot_draw_glyph_or_fail (hb_font_t *font,
			  ശൂന്യം *font_data HB_UNUSED,
			  hb_codepoint_t glyph,
			  hb_draw_funcs_t *draw_funcs, ശൂന്യം *draw_data,
			  ശൂന്യം *user_data)
{
  സ്ഥിരം hb_ot_font_t *ot_font = (സ്ഥിരം hb_ot_font_t *) font_data;
  hb_draw_session_t draw_session {draw_funcs, draw_data};

  OT::hb_scalar_cache_t *gvar_cache = nullptr;
#ifndef HB_NO_VAR
  എങ്കിൽ (font->അക്കം_coords)
  {
    ot_font->check_serial (font);
    gvar_cache = ot_font->draw.acquire_gvar_cache (*ot_font->ot_face->gvar);
  }
#endif
  HB_SCOPE_GUARD (ot_font->draw.release_gvar_cache (gvar_cache));

#ifndef HB_NO_VAR_COMPOSITES
  എങ്കിൽ (font->face->table.VARC->get_path (font, glyph, draw_session)) തിരികെ സത്യം;
#endif
  // Keep the following in synch with VARC::get_path_at()
  എങ്കിൽ (font->face->table.glyf->get_path (font, glyph, draw_session, gvar_cache,
				       &draw_session.get_budget ())) തിരികെ സത്യം;

#ifndef HB_NO_CFF
  എങ്കിൽ (font->face->table.cff2->get_path (font, glyph, draw_session,
				       &draw_session.get_budget ())) തിരികെ സത്യം;
  എങ്കിൽ (font->face->table.cff1->get_path (font, glyph, draw_session,
				       &draw_session.get_budget ())) തിരികെ സത്യം;
#endif

  തിരികെ അസത്യം;
}
#endif

#ifndef HB_NO_PAINT
സ്ഥിതം hb_bool_t
hb_ot_paint_glyph_or_fail (hb_font_t *font,
			   ശൂന്യം *font_data,
			   hb_codepoint_t glyph,
			   hb_paint_funcs_t *paint_funcs, ശൂന്യം *paint_data,
			   unsigned പൂർണ്ണസംഖ്യ palette,
			   hb_color_t foreground,
			   ശൂന്യം *user_data)
{
#ifndef HB_NO_COLOR
  എങ്കിൽ (font->face->table.COLR->paint_glyph (font, glyph, paint_funcs, paint_data, palette, foreground)) തിരികെ സത്യം;
#ifndef HB_NO_SVG
  എങ്കിൽ (font->face->table.SVG->paint_glyph (font, glyph, paint_funcs, paint_data)) തിരികെ സത്യം;
#endif
#ifndef HB_NO_OT_FONT_BITMAP
  എങ്കിൽ (font->face->table.CBDT->paint_glyph (font, glyph, paint_funcs, paint_data)) തിരികെ സത്യം;
  എങ്കിൽ (font->face->table.sbix->paint_glyph (font, glyph, paint_funcs, paint_data)) തിരികെ സത്യം;
#endif
#endif
  തിരികെ അസത്യം;
}
#endif

സ്ഥിതം വരിയിൽ ശൂന്യം free_static_ot_funcs ();

സ്ഥിതം ഘടന hb_ot_font_funcs_lazy_loader_t : hb_font_funcs_lazy_loader_t<hb_ot_font_funcs_lazy_loader_t>
{
  സ്ഥിതം hb_font_funcs_t *create ()
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

    തിരികെ funcs;
  }
} static_ot_funcs;

സ്ഥിതം വരിയിൽ
ശൂന്യം free_static_ot_funcs ()
{
  static_ot_funcs.free_instance ();
}

സ്ഥിതം hb_font_funcs_t *
_hb_ot_get_font_funcs ()
{
  തിരികെ static_ot_funcs.get_unconst ();
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
ശൂന്യം
hb_ot_font_set_funcs (hb_font_t *font)
{
  hb_ot_font_t *ot_font = _hb_ot_font_create (font);
  എങ്കിൽ (unlikely (!ot_font))
    തിരികെ;

  hb_font_set_funcs (font,
		     _hb_ot_get_font_funcs (),
		     ot_font,
		     _hb_ot_font_destroy);
}

#endif
