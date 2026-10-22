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

#અન્તર્ભાવ "hb.hh"

#ifndef HB_NO_OT_FONT

#અન્તર્ભાવ "hb-ot.h"

#અન્તર્ભાવ "hb-cache.hh"
#અન્તર્ભાવ "hb-font.hh"
#અન્તર્ભાવ "hb-machinery.hh"
#અન્તર્ભાવ "hb-ot-face.hh"

#અન્તર્ભાવ "hb-ot-cmap-table.hh"
#અન્તર્ભાવ "hb-ot-glyf-table.hh"
#અન્તર્ભાવ "hb-ot-var-gvar-table.hh"
#અન્તર્ભાવ "hb-ot-cff2-table.hh"
#અન્તર્ભાવ "hb-ot-cff1-table.hh"
#અન્તર્ભાવ "hb-ot-hmtx-table.hh"
#અન્તર્ભાવ "hb-ot-post-table.hh"
#અન્તર્ભાવ "hb-ot-stat-table.hh"
#અન્તર્ભાવ "hb-ot-var-varc-table.hh"
#અન્તર્ભાવ "hb-ot-vorg-table.hh"
#અન્તર્ભાવ "OT/Color/CBDT/CBDT.hh"
#અન્તર્ભાવ "OT/Color/COLR/COLR.hh"
#અન્તર્ભાવ "OT/Color/sbix/sbix.hh"
#અન્તર્ભાવ "OT/Color/svg/svg.hh"


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
static_assert (કદ (hb_ot_font_advance_cache_t) == 1024, "");

using hb_ot_font_origin_cache_t = hb_cache_t<20, 20>;
static_assert (કદ (hb_ot_font_origin_cache_t) == 1024, "");

સંરચના hb_ot_font_t
{
  અચલ hb_ot_face_t *ot_face;

  mutable hb_atomic_t<પૂર્ણાંક> cached_serial;
  mutable hb_atomic_t<પૂર્ણાંક> cached_coords_serial;

  સંરચના direction_cache_t
  {
    mutable hb_atomic_t<hb_ot_font_advance_cache_t *> advance_cache;
    mutable hb_atomic_t<OT::hb_scalar_cache_t *> varStore_cache;

    ~direction_cache_t ()
    {
      clear ();
    }

    hb_ot_font_advance_cache_t *acquire_advance_cache () અચલ
    {
    retry:
      auto *cache = advance_cache.get_acquire ();
      જો (!cache)
      {
        cache = (hb_ot_font_advance_cache_t *) hb_malloc (કદ (hb_ot_font_advance_cache_t));
	જો (!cache)
	  પાછા nullptr;
	નવું (cache) hb_ot_font_advance_cache_t;
	પાછા cache;
      }
      જો (advance_cache.cmpexch (cache, nullptr))
        પાછા cache;
      નહીંતર
        જાઓ retry;
    }
    શૂન્ય release_advance_cache (hb_ot_font_advance_cache_t *cache) અચલ
    {
      જો (!cache)
        પાછા;
      જો (!advance_cache.cmpexch (nullptr, cache))
        hb_free (cache);
    }
    શૂન્ય clear_advance_cache () અચલ
    {
    retry:
      auto *cache = advance_cache.get_acquire ();
      જો (!cache)
	પાછા;
      જો (advance_cache.cmpexch (cache, nullptr))
	hb_free (cache);
      નહીંતર
        જાઓ retry;
    }

    OT::hb_scalar_cache_t *acquire_varStore_cache (અચલ OT::ItemVariationStore &varStore) અચલ
    {
    retry:
      auto *cache = varStore_cache.get_acquire ();
      જો (!cache)
	પાછા varStore.create_cache ();
      જો (varStore_cache.cmpexch (cache, nullptr))
	પાછા cache;
      નહીંતર
	જાઓ retry;
    }
    શૂન્ય release_varStore_cache (OT::hb_scalar_cache_t *cache) અચલ
    {
      જો (!cache)
	પાછા;
      જો (!varStore_cache.cmpexch (nullptr, cache))
	OT::ItemVariationStore::destroy_cache (cache);
    }
    શૂન્ય clear_varStore_cache () અચલ
    {
    retry:
      auto *cache = varStore_cache.get_acquire ();
      જો (!cache)
	પાછા;
      જો (varStore_cache.cmpexch (cache, nullptr))
	OT::ItemVariationStore::destroy_cache (cache);
      નહીંતર
	જાઓ retry;
    }

    શૂન્ય clear () અચલ
    {
      clear_advance_cache ();
      clear_varStore_cache ();
    }

  } h, v;

  સંરચના origin_cache_t
  {
    mutable hb_atomic_t<hb_ot_font_origin_cache_t *> origin_cache;
    mutable hb_atomic_t<OT::hb_scalar_cache_t *> varStore_cache;

    ~origin_cache_t ()
    {
      clear ();
    }

    hb_ot_font_origin_cache_t *acquire_origin_cache () અચલ
    {
    retry:
      auto *cache = origin_cache.get_acquire ();
      જો (!cache)
      {
        cache = (hb_ot_font_origin_cache_t *) hb_malloc (કદ (hb_ot_font_origin_cache_t));
	જો (!cache)
	  પાછા nullptr;
	નવું (cache) hb_ot_font_origin_cache_t;
	પાછા cache;
      }
      જો (origin_cache.cmpexch (cache, nullptr))
        પાછા cache;
      નહીંતર
        જાઓ retry;
    }
    શૂન્ય release_origin_cache (hb_ot_font_origin_cache_t *cache) અચલ
    {
      જો (!cache)
        પાછા;
      જો (!origin_cache.cmpexch (nullptr, cache))
        hb_free (cache);
    }
    શૂન્ય clear_origin_cache () અચલ
    {
    retry:
      auto *cache = origin_cache.get_acquire ();
      જો (!cache)
	પાછા;
      જો (origin_cache.cmpexch (cache, nullptr))
	hb_free (cache);
      નહીંતર
        જાઓ retry;
    }

    OT::hb_scalar_cache_t *acquire_varStore_cache (અચલ OT::ItemVariationStore &varStore) અચલ
    {
    retry:
      auto *cache = varStore_cache.get_acquire ();
      જો (!cache)
	પાછા varStore.create_cache ();
      જો (varStore_cache.cmpexch (cache, nullptr))
	પાછા cache;
      નહીંતર
	જાઓ retry;
    }
    શૂન્ય release_varStore_cache (OT::hb_scalar_cache_t *cache) અચલ
    {
      જો (!cache)
	પાછા;
      જો (!varStore_cache.cmpexch (nullptr, cache))
	OT::ItemVariationStore::destroy_cache (cache);
    }
    શૂન્ય clear_varStore_cache () અચલ
    {
    retry:
      auto *cache = varStore_cache.get_acquire ();
      જો (!cache)
	પાછા;
      જો (varStore_cache.cmpexch (cache, nullptr))
	OT::ItemVariationStore::destroy_cache (cache);
      નહીંતર
	જાઓ retry;
    }

    શૂન્ય clear () અચલ
    {
      clear_origin_cache ();
      clear_varStore_cache ();
    }
  } v_origin;

  સંરચના draw_cache_t
  {
    mutable hb_atomic_t<OT::hb_scalar_cache_t *> gvar_cache;

    ~draw_cache_t ()
    {
      clear ();
    }

    OT::hb_scalar_cache_t *acquire_gvar_cache (અચલ OT::gvar_accelerator_t &gvar) અચલ
    {
    retry:
      auto *cache = gvar_cache.get_acquire ();
      જો (!cache)
	પાછા gvar.create_cache ();
      જો (gvar_cache.cmpexch (cache, nullptr))
	પાછા cache;
      નહીંતર
	જાઓ retry;
    }
    શૂન્ય release_gvar_cache (OT::hb_scalar_cache_t *cache) અચલ
    {
      જો (!cache)
	પાછા;
      જો (!gvar_cache.cmpexch (nullptr, cache))
	OT::gvar_accelerator_t::destroy_cache (cache);
    }
    શૂન્ય clear_gvar_cache () અચલ
    {
    retry:
      auto *cache = gvar_cache.get_acquire ();
      જો (!cache)
	પાછા;
      જો (gvar_cache.cmpexch (cache, nullptr))
	OT::gvar_accelerator_t::destroy_cache (cache);
      નહીંતર
	જાઓ retry;
    }

    શૂન્ય clear () અચલ
    {
      clear_gvar_cache ();
    }
  } draw;

  શૂન્ય check_serial (hb_font_t *font) અચલ
  {
    પૂર્ણાંક font_serial = font->serial.get_acquire ();
    જો (cached_serial.get_acquire () != font_serial)
    {
      /* These caches are dependent on scale and synthetic settings.
       * Any change to the font invalidates them. */
      v_origin.clear ();

      cached_serial.set_release (font_serial);
    }

    પૂર્ણાંક font_serial_coords = font->serial_coords.get_acquire ();
    જો (cached_coords_serial.get_acquire () != font_serial_coords)
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

સ્થિર hb_ot_font_t *
_hb_ot_font_create (hb_font_t *font)
{
  hb_ot_font_t *ot_font = (hb_ot_font_t *) hb_calloc (1, કદ (hb_ot_font_t));
  જો (unlikely (!ot_font))
    પાછા nullptr;

  ot_font->ot_face = &font->face->table;

  પાછા ot_font;
}

સ્થિર શૂન્ય
_hb_ot_font_destroy (શૂન્ય *font_data)
{
  hb_ot_font_t *ot_font = (hb_ot_font_t *) font_data;

  ot_font->~hb_ot_font_t ();

  hb_free (ot_font);
}

સ્થિર hb_bool_t
hb_ot_get_nominal_glyph (hb_font_t *font HB_UNUSED,
			 શૂન્ય *font_data,
			 hb_codepoint_t unicode,
			 hb_codepoint_t *glyph,
			 શૂન્ય *user_data HB_UNUSED)
{
  અચલ hb_ot_font_t *ot_font = (અચલ hb_ot_font_t *) font_data;
  અચલ hb_ot_face_t *ot_face = ot_font->ot_face;
  પાછા ot_face->cmap->get_nominal_glyph (unicode, glyph);
}

સ્થિર unsigned પૂર્ણાંક
hb_ot_get_nominal_glyphs (hb_font_t *font HB_UNUSED,
			  શૂન્ય *font_data,
			  unsigned પૂર્ણાંક ગણતરી,
			  અચલ hb_codepoint_t *first_unicode,
			  unsigned પૂર્ણાંક unicode_stride,
			  hb_codepoint_t *first_glyph,
			  unsigned પૂર્ણાંક glyph_stride,
			  શૂન્ય *user_data HB_UNUSED)
{
  અચલ hb_ot_font_t *ot_font = (અચલ hb_ot_font_t *) font_data;
  અચલ hb_ot_face_t *ot_face = ot_font->ot_face;
  પાછા ot_face->cmap->get_nominal_glyphs (ગણતરી,
					    first_unicode, unicode_stride,
					    first_glyph, glyph_stride);
}

સ્થિર hb_bool_t
hb_ot_get_variation_glyph (hb_font_t *font HB_UNUSED,
			   શૂન્ય *font_data,
			   hb_codepoint_t unicode,
			   hb_codepoint_t variation_selector,
			   hb_codepoint_t *glyph,
			   શૂન્ય *user_data HB_UNUSED)
{
  અચલ hb_ot_font_t *ot_font = (અચલ hb_ot_font_t *) font_data;
  અચલ hb_ot_face_t *ot_face = ot_font->ot_face;
  પાછા ot_face->cmap->get_variation_glyph (unicode,
                                             variation_selector, glyph);
}

સ્થિર શૂન્ય
hb_ot_get_glyph_h_advances (hb_font_t* font, શૂન્ય* font_data,
			    unsigned ગણતરી,
			    અચલ hb_codepoint_t *first_glyph,
			    unsigned glyph_stride,
			    hb_position_t *first_advance,
			    unsigned advance_stride,
			    શૂન્ય *user_data HB_UNUSED)
{
  // Duplicated in v_advances. Ugly. Keep in sync'ish.

  અચલ hb_ot_font_t *ot_font = (અચલ hb_ot_font_t *) font_data;
  અચલ hb_ot_face_t *ot_face = ot_font->ot_face;
  અચલ OT::hmtx_accelerator_t &hmtx = *ot_face->hmtx;

  જો (unlikely (!hmtx.has_data ()))
  {
    hb_position_t advance = font->face->get_upem () / 2;
    advance = font->em_scale_x (advance);
    માટે (unsigned પૂર્ણાંક i = 0; i < ગણતરી; i++)
    {
      *first_advance = advance;
      first_advance = &StructAtOffsetUnaligned<hb_position_t> (first_advance, advance_stride);
    }
    પાછા;
  }

#ifndef HB_NO_VAR
  જો (!font->has_nonzero_coords)
  {
  fallback:
#નહીંતર
  {
#endif
    // Just plain htmx data. No need to cache.
    માટે (unsigned પૂર્ણાંક i = 0; i < ગણતરી; i++)
    {
      *first_advance = font->em_scale_x (hmtx.get_advance_without_var_unscaled (*first_glyph));
      first_glyph = &StructAtOffsetUnaligned<hb_codepoint_t> (first_glyph, glyph_stride);
      first_advance = &StructAtOffsetUnaligned<hb_position_t> (first_advance, advance_stride);
    }
    પાછા;
  }

#ifndef HB_NO_VAR
  /* has_nonzero_coords. */

  ot_font->check_serial (font);
  hb_ot_font_advance_cache_t *advance_cache = ot_font->h.acquire_advance_cache ();
  જો (!advance_cache)
  {
    // malloc failure. Just use the fallback non-variable path.
    જાઓ fallback;
  }

  /* If HVAR is present, use it.*/
  અચલ OT::HVAR &HVAR = *hmtx.var_table;
  જો (HVAR.has_data ())
  {
    અચલ OT::ItemVariationStore &varStore = &HVAR + HVAR.varStore;
    OT::hb_scalar_cache_t *varStore_cache = ot_font->h.acquire_varStore_cache (varStore);

    માટે (unsigned પૂર્ણાંક i = 0; i < ગણતરી; i++)
    {
      hb_position_t v;
      unsigned cv;
      જો (advance_cache->get (*first_glyph, &cv))
	v = cv;
      નહીંતર
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
    પાછા;
  }

  અચલ auto &gvar = *ot_face->gvar;
  જો (gvar.has_data ())
  {
    અચલ auto &glyf = *ot_face->glyf;
    auto *scratch = glyf.acquire_scratch ();
    જો (unlikely (!scratch))
    {
      ot_font->h.release_advance_cache (advance_cache);
      જાઓ fallback;
    }
    OT::hb_scalar_cache_t *gvar_cache = ot_font->draw.acquire_gvar_cache (gvar);

    માટે (unsigned પૂર્ણાંક i = 0; i < ગણતરી; i++)
    {
      hb_position_t v;
      unsigned cv;
      જો (advance_cache->get (*first_glyph, &cv))
	v = cv;
      નહીંતર
      {
        v = glyf.get_advance_with_var_unscaled (*first_glyph, font, ખોટું, *scratch, gvar_cache);
	advance_cache->set (*first_glyph, v);
      }
      *first_advance = font->em_scale_x (v);
      first_glyph = &StructAtOffsetUnaligned<hb_codepoint_t> (first_glyph, glyph_stride);
      first_advance = &StructAtOffsetUnaligned<hb_position_t> (first_advance, advance_stride);
    }

    ot_font->draw.release_gvar_cache (gvar_cache);
    glyf.release_scratch (scratch);
    ot_font->h.release_advance_cache (advance_cache);
    પાછા;
  }

  ot_font->h.release_advance_cache (advance_cache);
  // No HVAR or GVAR.  Just use the fallback non-variable path.
  જાઓ fallback;
#endif
}

#ifndef HB_NO_VERTICAL
સ્થિર શૂન્ય
hb_ot_get_glyph_v_advances (hb_font_t* font, શૂન્ય* font_data,
			    unsigned ગણતરી,
			    અચલ hb_codepoint_t *first_glyph,
			    unsigned glyph_stride,
			    hb_position_t *first_advance,
			    unsigned advance_stride,
			    શૂન્ય *user_data HB_UNUSED)
{
  // Duplicated from h_advances. Ugly. Keep in sync'ish.

  અચલ hb_ot_font_t *ot_font = (અચલ hb_ot_font_t *) font_data;
  અચલ hb_ot_face_t *ot_face = ot_font->ot_face;
  અચલ OT::vmtx_accelerator_t &vmtx = *ot_face->vmtx;

  જો (unlikely (!vmtx.has_data ()))
  {
    hb_font_extents_t font_extents;
    font->get_h_extents_with_fallback (&font_extents);
    hb_position_t advance = font_extents.descender - font_extents.ascender;
    માટે (unsigned પૂર્ણાંક i = 0; i < ગણતરી; i++)
    {
      *first_advance = advance;
      first_advance = &StructAtOffsetUnaligned<hb_position_t> (first_advance, advance_stride);
    }
    પાછા;
  }

#ifndef HB_NO_VAR
  જો (!font->has_nonzero_coords)
  {
  fallback:
#નહીંતર
  {
#endif
    // Just plain vtmx data. No need to cache.
    માટે (unsigned પૂર્ણાંક i = 0; i < ગણતરી; i++)
    {
      *first_advance = font->em_scale_y (- (પૂર્ણાંક) vmtx.get_advance_without_var_unscaled (*first_glyph));
      first_glyph = &StructAtOffsetUnaligned<hb_codepoint_t> (first_glyph, glyph_stride);
      first_advance = &StructAtOffsetUnaligned<hb_position_t> (first_advance, advance_stride);
    }
    પાછા;
  }

#ifndef HB_NO_VAR
  /* has_nonzero_coords. */

  ot_font->check_serial (font);
  hb_ot_font_advance_cache_t *advance_cache = ot_font->v.acquire_advance_cache ();
  જો (!advance_cache)
  {
    // malloc failure. Just use the fallback non-variable path.
    જાઓ fallback;
  }

  /* If VVAR is present, use it.*/
  અચલ OT::VVAR &VVAR = *vmtx.var_table;
  જો (VVAR.has_data ())
  {
    અચલ OT::ItemVariationStore &varStore = &VVAR + VVAR.varStore;
    OT::hb_scalar_cache_t *varStore_cache = ot_font->v.acquire_varStore_cache (varStore);

    માટે (unsigned પૂર્ણાંક i = 0; i < ગણતરી; i++)
    {
      hb_position_t v;
      unsigned cv;
      જો (advance_cache->get (*first_glyph, &cv))
	v = cv;
      નહીંતર
      {
        v = vmtx.get_advance_with_var_unscaled (*first_glyph, font, varStore_cache);
	advance_cache->set (*first_glyph, v);
      }
      *first_advance = font->em_scale_y (- (પૂર્ણાંક) v);
      first_glyph = &StructAtOffsetUnaligned<hb_codepoint_t> (first_glyph, glyph_stride);
      first_advance = &StructAtOffsetUnaligned<hb_position_t> (first_advance, advance_stride);
    }

    ot_font->v.release_varStore_cache (varStore_cache);
    ot_font->v.release_advance_cache (advance_cache);
    પાછા;
  }

  અચલ auto &gvar = *ot_face->gvar;
  જો (gvar.has_data ())
  {
    અચલ auto &glyf = *ot_face->glyf;
    auto *scratch = glyf.acquire_scratch ();
    જો (unlikely (!scratch))
    {
      ot_font->v.release_advance_cache (advance_cache);
      જાઓ fallback;
    }
    OT::hb_scalar_cache_t *gvar_cache = ot_font->draw.acquire_gvar_cache (gvar);

    માટે (unsigned પૂર્ણાંક i = 0; i < ગણતરી; i++)
    {
      hb_position_t v;
      unsigned cv;
      જો (advance_cache->get (*first_glyph, &cv))
	v = cv;
      નહીંતર
      {
        v = glyf.get_advance_with_var_unscaled (*first_glyph, font, સાચું, *scratch, gvar_cache);
	advance_cache->set (*first_glyph, v);
      }
      *first_advance = font->em_scale_y (- (પૂર્ણાંક) v);
      first_glyph = &StructAtOffsetUnaligned<hb_codepoint_t> (first_glyph, glyph_stride);
      first_advance = &StructAtOffsetUnaligned<hb_position_t> (first_advance, advance_stride);
    }

    ot_font->draw.release_gvar_cache (gvar_cache);
    glyf.release_scratch (scratch);
    ot_font->v.release_advance_cache (advance_cache);
    પાછા;
  }

  ot_font->v.release_advance_cache (advance_cache);
  // No VVAR or GVAR.  Just use the fallback non-variable path.
  જાઓ fallback;
#endif
}
#endif

#ifndef HB_NO_VERTICAL
HB_HOT
સ્થિર hb_bool_t
hb_ot_get_glyph_v_origins (hb_font_t *font,
			   શૂન્ય *font_data,
			   unsigned પૂર્ણાંક ગણતરી,
			   અચલ hb_codepoint_t *first_glyph,
			   unsigned glyph_stride,
			   hb_position_t *first_x,
			   unsigned x_stride,
			   hb_position_t *first_y,
			   unsigned y_stride,
			   શૂન્ય *user_data HB_UNUSED)
{
  અચલ hb_ot_font_t *ot_font = (અચલ hb_ot_font_t *) font_data;
  અચલ hb_ot_face_t *ot_face = ot_font->ot_face;

  ot_font->check_serial (font);

  /* First, set all the x values to half the advance width. */
  font->get_glyph_h_advances (ગણતરી,
			      first_glyph, glyph_stride,
			      first_x, x_stride);
  માટે (unsigned i = 0; i < ગણતરી; i++)
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
  અચલ OT::VORG &VORG = *ot_face->VORG;
  જો (origin_cache && VORG.has_data ())
  {
#ifndef HB_NO_VAR
    જો (!font->has_nonzero_coords)
#endif
    {
      માટે (unsigned i = 0; i < ગણતરી; i++)
      {
	hb_position_t origin;
	unsigned cv;
	જો (origin_cache->get (*first_glyph, &cv))
	  origin = font->y_scale < 0 ? -static_cast<hb_position_t>(cv) : static_cast<hb_position_t>(cv);
	નહીંતર
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
    નહીંતર
    {
      અચલ OT::VVAR &VVAR = *ot_face->vmtx->var_table;
      અચલ auto &varStore = &VVAR + VVAR.varStore;
      auto *varStore_cache = ot_font->v_origin.acquire_varStore_cache (varStore);
      માટે (unsigned i = 0; i < ગણતરી; i++)
      {
	hb_position_t origin;
	unsigned cv;
	જો (origin_cache->get (*first_glyph, &cv))
	  origin = font->y_scale < 0 ? -static_cast<hb_position_t>(cv) : static_cast<hb_position_t>(cv);
	નહીંતર
	{
	  origin = font->em_scalef_y (VORG.get_y_origin (*first_glyph) +
				      VVAR.get_vorg_delta_unscaled (*first_glyph,
								    font->coords, font->અંક_coords,
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
    પાછા સાચું;
  }

  /* If and only if `vmtx` is present and it's a `glyf` font,
   * we use the top phantom point, deduced from vmtx,glyf[,gvar]. */
  અચલ auto &vmtx = *ot_face->vmtx;
  અચલ auto &glyf = *ot_face->glyf;
  જો (origin_cache && vmtx.has_data() && glyf.has_data ())
  {
    auto *scratch = glyf.acquire_scratch ();
    જો (unlikely (!scratch))
    {
      ot_font->v_origin.release_origin_cache (origin_cache);
      પાછા ખોટું;
    }
    OT::hb_scalar_cache_t *gvar_cache = nullptr;
#ifndef HB_NO_VAR
    gvar_cache = font->has_nonzero_coords ?
					ot_font->draw.acquire_gvar_cache (*ot_face->gvar) :
					nullptr;
#endif

    માટે (unsigned i = 0; i < ગણતરી; i++)
    {
      hb_position_t origin;
      unsigned cv;
      જો (origin_cache->get (*first_glyph, &cv))
	origin = font->y_scale < 0 ? -static_cast<hb_position_t>(cv) : static_cast<hb_position_t>(cv);
      નહીંતર
      {
	origin = font->em_scalef_y (glyf.get_v_origin_with_var_unscaled (*first_glyph, font, *scratch, gvar_cache));
	origin_cache->set (*first_glyph, font->y_scale < 0 ? -origin : origin);
      }

      *first_y = origin;

      first_glyph = &StructAtOffsetUnaligned<hb_codepoint_t> (first_glyph, glyph_stride);
      first_y = &StructAtOffsetUnaligned<hb_position_t> (first_y, y_stride);
    }

#ifndef HB_NO_VAR
    જો (gvar_cache)
      ot_font->draw.release_gvar_cache (gvar_cache);
#endif
    glyf.release_scratch (scratch);
    ot_font->v_origin.release_origin_cache (origin_cache);
    પાછા સાચું;
  }

  /* Otherwise, use glyph extents to center the glyph vertically.
   * If getting glyph extents failed, just use the font ascender. */
  જો (origin_cache && font->has_glyph_extents_func ())
  {
    hb_font_extents_t font_extents;
    font->get_h_extents_with_fallback (&font_extents);
    hb_position_t font_advance = font_extents.ascender - font_extents.descender;

    માટે (unsigned i = 0; i < ગણતરી; i++)
    {
      hb_position_t origin;
      unsigned cv;

      જો (origin_cache->get (*first_glyph, &cv))
	origin = font->y_scale < 0 ? -static_cast<hb_position_t>(cv) : static_cast<hb_position_t>(cv);
      નહીંતર
      {
	hb_glyph_extents_t extents = {0};
	જો (likely (font->get_glyph_extents (*first_glyph, &extents)))
	  origin = hb_clamp_to<hb_position_t> ((int64_t) extents.y_bearing +
					 (((int64_t) font_advance + extents.height) >> 1));
	નહીંતર
	  origin = font_extents.ascender;

	origin_cache->set (*first_glyph, font->y_scale < 0 ? -origin : origin);
      }

      *first_y = origin;

      first_glyph = &StructAtOffsetUnaligned<hb_codepoint_t> (first_glyph, glyph_stride);
      first_y = &StructAtOffsetUnaligned<hb_position_t> (first_y, y_stride);
    }
  }

  ot_font->v_origin.release_origin_cache (origin_cache);
  પાછા સાચું;
}
#endif

સ્થિર hb_bool_t
hb_ot_get_glyph_extents (hb_font_t *font,
			 શૂન્ય *font_data,
			 hb_codepoint_t glyph,
			 hb_glyph_extents_t *extents,
			 શૂન્ય *user_data HB_UNUSED)
{
  અચલ hb_ot_font_t *ot_font = (અચલ hb_ot_font_t *) font_data;
  અચલ hb_ot_face_t *ot_face = ot_font->ot_face;

#જો !defined(HB_NO_OT_FONT_BITMAP) && !defined(HB_NO_COLOR)
  જો (ot_face->sbix->get_extents (font, glyph, extents)) પાછા સાચું;
  જો (ot_face->CBDT->get_extents (font, glyph, extents)) પાછા સાચું;
#endif
#જો !defined(HB_NO_COLOR) && !defined(HB_NO_PAINT)
  જો (ot_face->COLR->get_extents (font, glyph, extents)) પાછા સાચું;
#endif
#ifndef HB_NO_VAR_COMPOSITES
  જો (ot_face->VARC->get_extents (font, glyph, extents)) પાછા સાચું;
#endif
  જો (ot_face->glyf->get_extents (font, glyph, extents)) પાછા સાચું;
#ifndef HB_NO_OT_FONT_CFF
  જો (ot_face->cff2->get_extents (font, glyph, extents)) પાછા સાચું;
  જો (ot_face->cff1->get_extents (font, glyph, extents)) પાછા સાચું;
#endif

  પાછા ખોટું;
}

#ifndef HB_NO_OT_FONT_GLYPH_NAMES
સ્થિર hb_bool_t
hb_ot_get_glyph_name (hb_font_t *font HB_UNUSED,
		      શૂન્ય *font_data,
		      hb_codepoint_t glyph,
		      અક્ષર *name, unsigned પૂર્ણાંક size,
		      શૂન્ય *user_data HB_UNUSED)
{
  અચલ hb_ot_font_t *ot_font = (અચલ hb_ot_font_t *) font_data;
  અચલ hb_ot_face_t *ot_face = ot_font->ot_face;

  જો (ot_face->post->get_glyph_name (glyph, name, size)) પાછા સાચું;
#ifndef HB_NO_OT_FONT_CFF
  જો (ot_face->cff1->get_glyph_name (glyph, name, size)) પાછા સાચું;
#endif
  પાછા ખોટું;
}
સ્થિર hb_bool_t
hb_ot_get_glyph_from_name (hb_font_t *font HB_UNUSED,
			   શૂન્ય *font_data,
			   અચલ અક્ષર *name, પૂર્ણાંક len,
			   hb_codepoint_t *glyph,
			   શૂન્ય *user_data HB_UNUSED)
{
  અચલ hb_ot_font_t *ot_font = (અચલ hb_ot_font_t *) font_data;
  અચલ hb_ot_face_t *ot_face = ot_font->ot_face;

  જો (ot_face->post->get_glyph_from_name (name, len, glyph)) પાછા સાચું;
#ifndef HB_NO_OT_FONT_CFF
    જો (ot_face->cff1->get_glyph_from_name (name, len, glyph)) પાછા સાચું;
#endif
  પાછા ખોટું;
}
#endif

સ્થિર hb_bool_t
hb_ot_get_font_h_extents (hb_font_t *font,
			  શૂન્ય *font_data HB_UNUSED,
			  hb_font_extents_t *metrics,
			  શૂન્ય *user_data HB_UNUSED)
{
  પાછા _hb_ot_metrics_get_position_common (font, HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER, &metrics->ascender) &&
	 _hb_ot_metrics_get_position_common (font, HB_OT_METRICS_TAG_HORIZONTAL_DESCENDER, &metrics->descender) &&
	 _hb_ot_metrics_get_position_common (font, HB_OT_METRICS_TAG_HORIZONTAL_લીટી_GAP, &metrics->લીટી_gap);
}

#ifndef HB_NO_VERTICAL
સ્થિર hb_bool_t
hb_ot_get_font_v_extents (hb_font_t *font,
			  શૂન્ય *font_data HB_UNUSED,
			  hb_font_extents_t *metrics,
			  શૂન્ય *user_data HB_UNUSED)
{
  પાછા _hb_ot_metrics_get_position_common (font, HB_OT_METRICS_TAG_VERTICAL_ASCENDER, &metrics->ascender) &&
	 _hb_ot_metrics_get_position_common (font, HB_OT_METRICS_TAG_VERTICAL_DESCENDER, &metrics->descender) &&
	 _hb_ot_metrics_get_position_common (font, HB_OT_METRICS_TAG_VERTICAL_લીટી_GAP, &metrics->લીટી_gap);
}
#endif

#ifndef HB_NO_DRAW
સ્થિર hb_bool_t
hb_ot_draw_glyph_or_fail (hb_font_t *font,
			  શૂન્ય *font_data HB_UNUSED,
			  hb_codepoint_t glyph,
			  hb_draw_funcs_t *draw_funcs, શૂન્ય *draw_data,
			  શૂન્ય *user_data)
{
  અચલ hb_ot_font_t *ot_font = (અચલ hb_ot_font_t *) font_data;
  hb_draw_session_t draw_session {draw_funcs, draw_data};

  OT::hb_scalar_cache_t *gvar_cache = nullptr;
#ifndef HB_NO_VAR
  જો (font->અંક_coords)
  {
    ot_font->check_serial (font);
    gvar_cache = ot_font->draw.acquire_gvar_cache (*ot_font->ot_face->gvar);
  }
#endif
  HB_SCOPE_GUARD (ot_font->draw.release_gvar_cache (gvar_cache));

#ifndef HB_NO_VAR_COMPOSITES
  જો (font->face->table.VARC->get_path (font, glyph, draw_session)) પાછા સાચું;
#endif
  // Keep the following in synch with VARC::get_path_at()
  જો (font->face->table.glyf->get_path (font, glyph, draw_session, gvar_cache,
				       &draw_session.get_budget ())) પાછા સાચું;

#ifndef HB_NO_CFF
  જો (font->face->table.cff2->get_path (font, glyph, draw_session,
				       &draw_session.get_budget ())) પાછા સાચું;
  જો (font->face->table.cff1->get_path (font, glyph, draw_session,
				       &draw_session.get_budget ())) પાછા સાચું;
#endif

  પાછા ખોટું;
}
#endif

#ifndef HB_NO_PAINT
સ્થિર hb_bool_t
hb_ot_paint_glyph_or_fail (hb_font_t *font,
			   શૂન્ય *font_data,
			   hb_codepoint_t glyph,
			   hb_paint_funcs_t *paint_funcs, શૂન્ય *paint_data,
			   unsigned પૂર્ણાંક palette,
			   hb_color_t foreground,
			   શૂન્ય *user_data)
{
#ifndef HB_NO_COLOR
  જો (font->face->table.COLR->paint_glyph (font, glyph, paint_funcs, paint_data, palette, foreground)) પાછા સાચું;
#ifndef HB_NO_SVG
  જો (font->face->table.SVG->paint_glyph (font, glyph, paint_funcs, paint_data)) પાછા સાચું;
#endif
#ifndef HB_NO_OT_FONT_BITMAP
  જો (font->face->table.CBDT->paint_glyph (font, glyph, paint_funcs, paint_data)) પાછા સાચું;
  જો (font->face->table.sbix->paint_glyph (font, glyph, paint_funcs, paint_data)) પાછા સાચું;
#endif
#endif
  પાછા ખોટું;
}
#endif

સ્થિર અંતઃરેખ શૂન્ય free_static_ot_funcs ();

સ્થિર સંરચના hb_ot_font_funcs_lazy_loader_t : hb_font_funcs_lazy_loader_t<hb_ot_font_funcs_lazy_loader_t>
{
  સ્થિર hb_font_funcs_t *create ()
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

    પાછા funcs;
  }
} static_ot_funcs;

સ્થિર અંતઃરેખ
શૂન્ય free_static_ot_funcs ()
{
  static_ot_funcs.free_instance ();
}

સ્થિર hb_font_funcs_t *
_hb_ot_get_font_funcs ()
{
  પાછા static_ot_funcs.get_unconst ();
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
શૂન્ય
hb_ot_font_set_funcs (hb_font_t *font)
{
  hb_ot_font_t *ot_font = _hb_ot_font_create (font);
  જો (unlikely (!ot_font))
    પાછા;

  hb_font_set_funcs (font,
		     _hb_ot_get_font_funcs (),
		     ot_font,
		     _hb_ot_font_destroy);
}

#endif
