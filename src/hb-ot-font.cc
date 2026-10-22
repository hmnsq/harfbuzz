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

#ଅନ୍ତର୍ଭାଵ "hb.hh"

#ifndef HB_NO_OT_FONT

#ଅନ୍ତର୍ଭାଵ "hb-ot.h"

#ଅନ୍ତର୍ଭାଵ "hb-cache.hh"
#ଅନ୍ତର୍ଭାଵ "hb-font.hh"
#ଅନ୍ତର୍ଭାଵ "hb-machinery.hh"
#ଅନ୍ତର୍ଭାଵ "hb-ot-face.hh"

#ଅନ୍ତର୍ଭାଵ "hb-ot-cmap-table.hh"
#ଅନ୍ତର୍ଭାଵ "hb-ot-glyf-table.hh"
#ଅନ୍ତର୍ଭାଵ "hb-ot-var-gvar-table.hh"
#ଅନ୍ତର୍ଭାଵ "hb-ot-cff2-table.hh"
#ଅନ୍ତର୍ଭାଵ "hb-ot-cff1-table.hh"
#ଅନ୍ତର୍ଭାଵ "hb-ot-hmtx-table.hh"
#ଅନ୍ତର୍ଭାଵ "hb-ot-post-table.hh"
#ଅନ୍ତର୍ଭାଵ "hb-ot-stat-table.hh"
#ଅନ୍ତର୍ଭାଵ "hb-ot-var-varc-table.hh"
#ଅନ୍ତର୍ଭାଵ "hb-ot-vorg-table.hh"
#ଅନ୍ତର୍ଭାଵ "OT/Color/CBDT/CBDT.hh"
#ଅନ୍ତର୍ଭାଵ "OT/Color/COLR/COLR.hh"
#ଅନ୍ତର୍ଭାଵ "OT/Color/sbix/sbix.hh"
#ଅନ୍ତର୍ଭାଵ "OT/Color/svg/svg.hh"


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
static_assert (ଆକାର (hb_ot_font_advance_cache_t) == 1024, "");

using hb_ot_font_origin_cache_t = hb_cache_t<20, 20>;
static_assert (ଆକାର (hb_ot_font_origin_cache_t) == 1024, "");

ସଂରଚନା hb_ot_font_t
{
  ଅଚଳ hb_ot_face_t *ot_face;

  mutable hb_atomic_t<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> cached_serial;
  mutable hb_atomic_t<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> cached_coords_serial;

  ସଂରଚନା direction_cache_t
  {
    mutable hb_atomic_t<hb_ot_font_advance_cache_t *> advance_cache;
    mutable hb_atomic_t<OT::hb_scalar_cache_t *> varStore_cache;

    ~direction_cache_t ()
    {
      clear ();
    }

    hb_ot_font_advance_cache_t *acquire_advance_cache () ଅଚଳ
    {
    retry:
      auto *cache = advance_cache.get_acquire ();
      ଯଦି (!cache)
      {
        cache = (hb_ot_font_advance_cache_t *) hb_malloc (ଆକାର (hb_ot_font_advance_cache_t));
	ଯଦି (!cache)
	  ଫେରନ୍ତୁ nullptr;
	ନୂତନ (cache) hb_ot_font_advance_cache_t;
	ଫେରନ୍ତୁ cache;
      }
      ଯଦି (advance_cache.cmpexch (cache, nullptr))
        ଫେରନ୍ତୁ cache;
      ଅନ୍ୟଥା
        ଯାଅ retry;
    }
    ଶୂନ୍ୟ release_advance_cache (hb_ot_font_advance_cache_t *cache) ଅଚଳ
    {
      ଯଦି (!cache)
        ଫେରନ୍ତୁ;
      ଯଦି (!advance_cache.cmpexch (nullptr, cache))
        hb_free (cache);
    }
    ଶୂନ୍ୟ clear_advance_cache () ଅଚଳ
    {
    retry:
      auto *cache = advance_cache.get_acquire ();
      ଯଦି (!cache)
	ଫେରନ୍ତୁ;
      ଯଦି (advance_cache.cmpexch (cache, nullptr))
	hb_free (cache);
      ଅନ୍ୟଥା
        ଯାଅ retry;
    }

    OT::hb_scalar_cache_t *acquire_varStore_cache (ଅଚଳ OT::ItemVariationStore &varStore) ଅଚଳ
    {
    retry:
      auto *cache = varStore_cache.get_acquire ();
      ଯଦି (!cache)
	ଫେରନ୍ତୁ varStore.create_cache ();
      ଯଦି (varStore_cache.cmpexch (cache, nullptr))
	ଫେରନ୍ତୁ cache;
      ଅନ୍ୟଥା
	ଯାଅ retry;
    }
    ଶୂନ୍ୟ release_varStore_cache (OT::hb_scalar_cache_t *cache) ଅଚଳ
    {
      ଯଦି (!cache)
	ଫେରନ୍ତୁ;
      ଯଦି (!varStore_cache.cmpexch (nullptr, cache))
	OT::ItemVariationStore::destroy_cache (cache);
    }
    ଶୂନ୍ୟ clear_varStore_cache () ଅଚଳ
    {
    retry:
      auto *cache = varStore_cache.get_acquire ();
      ଯଦି (!cache)
	ଫେରନ୍ତୁ;
      ଯଦି (varStore_cache.cmpexch (cache, nullptr))
	OT::ItemVariationStore::destroy_cache (cache);
      ଅନ୍ୟଥା
	ଯାଅ retry;
    }

    ଶୂନ୍ୟ clear () ଅଚଳ
    {
      clear_advance_cache ();
      clear_varStore_cache ();
    }

  } h, v;

  ସଂରଚନା origin_cache_t
  {
    mutable hb_atomic_t<hb_ot_font_origin_cache_t *> origin_cache;
    mutable hb_atomic_t<OT::hb_scalar_cache_t *> varStore_cache;

    ~origin_cache_t ()
    {
      clear ();
    }

    hb_ot_font_origin_cache_t *acquire_origin_cache () ଅଚଳ
    {
    retry:
      auto *cache = origin_cache.get_acquire ();
      ଯଦି (!cache)
      {
        cache = (hb_ot_font_origin_cache_t *) hb_malloc (ଆକାର (hb_ot_font_origin_cache_t));
	ଯଦି (!cache)
	  ଫେରନ୍ତୁ nullptr;
	ନୂତନ (cache) hb_ot_font_origin_cache_t;
	ଫେରନ୍ତୁ cache;
      }
      ଯଦି (origin_cache.cmpexch (cache, nullptr))
        ଫେରନ୍ତୁ cache;
      ଅନ୍ୟଥା
        ଯାଅ retry;
    }
    ଶୂନ୍ୟ release_origin_cache (hb_ot_font_origin_cache_t *cache) ଅଚଳ
    {
      ଯଦି (!cache)
        ଫେରନ୍ତୁ;
      ଯଦି (!origin_cache.cmpexch (nullptr, cache))
        hb_free (cache);
    }
    ଶୂନ୍ୟ clear_origin_cache () ଅଚଳ
    {
    retry:
      auto *cache = origin_cache.get_acquire ();
      ଯଦି (!cache)
	ଫେରନ୍ତୁ;
      ଯଦି (origin_cache.cmpexch (cache, nullptr))
	hb_free (cache);
      ଅନ୍ୟଥା
        ଯାଅ retry;
    }

    OT::hb_scalar_cache_t *acquire_varStore_cache (ଅଚଳ OT::ItemVariationStore &varStore) ଅଚଳ
    {
    retry:
      auto *cache = varStore_cache.get_acquire ();
      ଯଦି (!cache)
	ଫେରନ୍ତୁ varStore.create_cache ();
      ଯଦି (varStore_cache.cmpexch (cache, nullptr))
	ଫେରନ୍ତୁ cache;
      ଅନ୍ୟଥା
	ଯାଅ retry;
    }
    ଶୂନ୍ୟ release_varStore_cache (OT::hb_scalar_cache_t *cache) ଅଚଳ
    {
      ଯଦି (!cache)
	ଫେରନ୍ତୁ;
      ଯଦି (!varStore_cache.cmpexch (nullptr, cache))
	OT::ItemVariationStore::destroy_cache (cache);
    }
    ଶୂନ୍ୟ clear_varStore_cache () ଅଚଳ
    {
    retry:
      auto *cache = varStore_cache.get_acquire ();
      ଯଦି (!cache)
	ଫେରନ୍ତୁ;
      ଯଦି (varStore_cache.cmpexch (cache, nullptr))
	OT::ItemVariationStore::destroy_cache (cache);
      ଅନ୍ୟଥା
	ଯାଅ retry;
    }

    ଶୂନ୍ୟ clear () ଅଚଳ
    {
      clear_origin_cache ();
      clear_varStore_cache ();
    }
  } v_origin;

  ସଂରଚନା draw_cache_t
  {
    mutable hb_atomic_t<OT::hb_scalar_cache_t *> gvar_cache;

    ~draw_cache_t ()
    {
      clear ();
    }

    OT::hb_scalar_cache_t *acquire_gvar_cache (ଅଚଳ OT::gvar_accelerator_t &gvar) ଅଚଳ
    {
    retry:
      auto *cache = gvar_cache.get_acquire ();
      ଯଦି (!cache)
	ଫେରନ୍ତୁ gvar.create_cache ();
      ଯଦି (gvar_cache.cmpexch (cache, nullptr))
	ଫେରନ୍ତୁ cache;
      ଅନ୍ୟଥା
	ଯାଅ retry;
    }
    ଶୂନ୍ୟ release_gvar_cache (OT::hb_scalar_cache_t *cache) ଅଚଳ
    {
      ଯଦି (!cache)
	ଫେରନ୍ତୁ;
      ଯଦି (!gvar_cache.cmpexch (nullptr, cache))
	OT::gvar_accelerator_t::destroy_cache (cache);
    }
    ଶୂନ୍ୟ clear_gvar_cache () ଅଚଳ
    {
    retry:
      auto *cache = gvar_cache.get_acquire ();
      ଯଦି (!cache)
	ଫେରନ୍ତୁ;
      ଯଦି (gvar_cache.cmpexch (cache, nullptr))
	OT::gvar_accelerator_t::destroy_cache (cache);
      ଅନ୍ୟଥା
	ଯାଅ retry;
    }

    ଶୂନ୍ୟ clear () ଅଚଳ
    {
      clear_gvar_cache ();
    }
  } draw;

  ଶୂନ୍ୟ check_serial (hb_font_t *font) ଅଚଳ
  {
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା font_serial = font->serial.get_acquire ();
    ଯଦି (cached_serial.get_acquire () != font_serial)
    {
      /* These caches are dependent on scale and synthetic settings.
       * Any change to the font invalidates them. */
      v_origin.clear ();

      cached_serial.set_release (font_serial);
    }

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା font_serial_coords = font->serial_coords.get_acquire ();
    ଯଦି (cached_coords_serial.get_acquire () != font_serial_coords)
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

ସ୍ଥିର hb_ot_font_t *
_hb_ot_font_create (hb_font_t *font)
{
  hb_ot_font_t *ot_font = (hb_ot_font_t *) hb_calloc (1, ଆକାର (hb_ot_font_t));
  ଯଦି (unlikely (!ot_font))
    ଫେରନ୍ତୁ nullptr;

  ot_font->ot_face = &font->face->table;

  ଫେରନ୍ତୁ ot_font;
}

ସ୍ଥିର ଶୂନ୍ୟ
_hb_ot_font_destroy (ଶୂନ୍ୟ *font_data)
{
  hb_ot_font_t *ot_font = (hb_ot_font_t *) font_data;

  ot_font->~hb_ot_font_t ();

  hb_free (ot_font);
}

ସ୍ଥିର hb_bool_t
hb_ot_get_nominal_glyph (hb_font_t *font HB_UNUSED,
			 ଶୂନ୍ୟ *font_data,
			 hb_codepoint_t unicode,
			 hb_codepoint_t *glyph,
			 ଶୂନ୍ୟ *user_data HB_UNUSED)
{
  ଅଚଳ hb_ot_font_t *ot_font = (ଅଚଳ hb_ot_font_t *) font_data;
  ଅଚଳ hb_ot_face_t *ot_face = ot_font->ot_face;
  ଫେରନ୍ତୁ ot_face->cmap->get_nominal_glyph (unicode, glyph);
}

ସ୍ଥିର unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା
hb_ot_get_nominal_glyphs (hb_font_t *font HB_UNUSED,
			  ଶୂନ୍ୟ *font_data,
			  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2,
			  ଅଚଳ hb_codepoint_t *first_unicode,
			  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା unicode_stride,
			  hb_codepoint_t *first_glyph,
			  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା glyph_stride,
			  ଶୂନ୍ୟ *user_data HB_UNUSED)
{
  ଅଚଳ hb_ot_font_t *ot_font = (ଅଚଳ hb_ot_font_t *) font_data;
  ଅଚଳ hb_ot_face_t *ot_face = ot_font->ot_face;
  ଫେରନ୍ତୁ ot_face->cmap->get_nominal_glyphs (ଗଣନା_2,
					    first_unicode, unicode_stride,
					    first_glyph, glyph_stride);
}

ସ୍ଥିର hb_bool_t
hb_ot_get_variation_glyph (hb_font_t *font HB_UNUSED,
			   ଶୂନ୍ୟ *font_data,
			   hb_codepoint_t unicode,
			   hb_codepoint_t variation_selector,
			   hb_codepoint_t *glyph,
			   ଶୂନ୍ୟ *user_data HB_UNUSED)
{
  ଅଚଳ hb_ot_font_t *ot_font = (ଅଚଳ hb_ot_font_t *) font_data;
  ଅଚଳ hb_ot_face_t *ot_face = ot_font->ot_face;
  ଫେରନ୍ତୁ ot_face->cmap->get_variation_glyph (unicode,
                                             variation_selector, glyph);
}

ସ୍ଥିର ଶୂନ୍ୟ
hb_ot_get_glyph_h_advances (hb_font_t* font, ଶୂନ୍ୟ* font_data,
			    unsigned ଗଣନା_2,
			    ଅଚଳ hb_codepoint_t *first_glyph,
			    unsigned glyph_stride,
			    hb_position_t *first_advance,
			    unsigned advance_stride,
			    ଶୂନ୍ୟ *user_data HB_UNUSED)
{
  // Duplicated in v_advances. Ugly. Keep in sync'ish.

  ଅଚଳ hb_ot_font_t *ot_font = (ଅଚଳ hb_ot_font_t *) font_data;
  ଅଚଳ hb_ot_face_t *ot_face = ot_font->ot_face;
  ଅଚଳ OT::hmtx_accelerator_t &hmtx = *ot_face->hmtx;

  ଯଦି (unlikely (!hmtx.has_data ()))
  {
    hb_position_t advance = font->face->get_upem () / 2;
    advance = font->em_scale_x (advance);
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଗଣନା_2; i++)
    {
      *first_advance = advance;
      first_advance = &StructAtOffsetUnaligned<hb_position_t> (first_advance, advance_stride);
    }
    ଫେରନ୍ତୁ;
  }

#ifndef HB_NO_VAR
  ଯଦି (!font->has_nonzero_coords)
  {
  fallback:
#ଅନ୍ୟଥା
  {
#endif
    // Just plain htmx data. No need to cache.
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଗଣନା_2; i++)
    {
      *first_advance = font->em_scale_x (hmtx.get_advance_without_var_unscaled (*first_glyph));
      first_glyph = &StructAtOffsetUnaligned<hb_codepoint_t> (first_glyph, glyph_stride);
      first_advance = &StructAtOffsetUnaligned<hb_position_t> (first_advance, advance_stride);
    }
    ଫେରନ୍ତୁ;
  }

#ifndef HB_NO_VAR
  /* has_nonzero_coords. */

  ot_font->check_serial (font);
  hb_ot_font_advance_cache_t *advance_cache = ot_font->h.acquire_advance_cache ();
  ଯଦି (!advance_cache)
  {
    // malloc failure. Just use the fallback non-variable path.
    ଯାଅ fallback;
  }

  /* If HVAR is present, use it.*/
  ଅଚଳ OT::HVAR &HVAR = *hmtx.var_table;
  ଯଦି (HVAR.has_data ())
  {
    ଅଚଳ OT::ItemVariationStore &varStore = &HVAR + HVAR.varStore;
    OT::hb_scalar_cache_t *varStore_cache = ot_font->h.acquire_varStore_cache (varStore);

    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଗଣନା_2; i++)
    {
      hb_position_t v;
      unsigned cv;
      ଯଦି (advance_cache->get (*first_glyph, &cv))
	v = cv;
      ଅନ୍ୟଥା
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
    ଫେରନ୍ତୁ;
  }

  ଅଚଳ auto &gvar = *ot_face->gvar;
  ଯଦି (gvar.has_data ())
  {
    ଅଚଳ auto &glyf = *ot_face->glyf;
    auto *scratch = glyf.acquire_scratch ();
    ଯଦି (unlikely (!scratch))
    {
      ot_font->h.release_advance_cache (advance_cache);
      ଯାଅ fallback;
    }
    OT::hb_scalar_cache_t *gvar_cache = ot_font->draw.acquire_gvar_cache (gvar);

    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଗଣନା_2; i++)
    {
      hb_position_t v;
      unsigned cv;
      ଯଦି (advance_cache->get (*first_glyph, &cv))
	v = cv;
      ଅନ୍ୟଥା
      {
        v = glyf.get_advance_with_var_unscaled (*first_glyph, font, ମିଥ୍ୟା, *scratch, gvar_cache);
	advance_cache->set (*first_glyph, v);
      }
      *first_advance = font->em_scale_x (v);
      first_glyph = &StructAtOffsetUnaligned<hb_codepoint_t> (first_glyph, glyph_stride);
      first_advance = &StructAtOffsetUnaligned<hb_position_t> (first_advance, advance_stride);
    }

    ot_font->draw.release_gvar_cache (gvar_cache);
    glyf.release_scratch (scratch);
    ot_font->h.release_advance_cache (advance_cache);
    ଫେରନ୍ତୁ;
  }

  ot_font->h.release_advance_cache (advance_cache);
  // No HVAR or GVAR.  Just use the fallback non-variable path.
  ଯାଅ fallback;
#endif
}

#ifndef HB_NO_VERTICAL
ସ୍ଥିର ଶୂନ୍ୟ
hb_ot_get_glyph_v_advances (hb_font_t* font, ଶୂନ୍ୟ* font_data,
			    unsigned ଗଣନା_2,
			    ଅଚଳ hb_codepoint_t *first_glyph,
			    unsigned glyph_stride,
			    hb_position_t *first_advance,
			    unsigned advance_stride,
			    ଶୂନ୍ୟ *user_data HB_UNUSED)
{
  // Duplicated from h_advances. Ugly. Keep in sync'ish.

  ଅଚଳ hb_ot_font_t *ot_font = (ଅଚଳ hb_ot_font_t *) font_data;
  ଅଚଳ hb_ot_face_t *ot_face = ot_font->ot_face;
  ଅଚଳ OT::vmtx_accelerator_t &vmtx = *ot_face->vmtx;

  ଯଦି (unlikely (!vmtx.has_data ()))
  {
    hb_font_extents_t font_extents;
    font->get_h_extents_with_fallback (&font_extents);
    hb_position_t advance = font_extents.descender - font_extents.ascender;
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଗଣନା_2; i++)
    {
      *first_advance = advance;
      first_advance = &StructAtOffsetUnaligned<hb_position_t> (first_advance, advance_stride);
    }
    ଫେରନ୍ତୁ;
  }

#ifndef HB_NO_VAR
  ଯଦି (!font->has_nonzero_coords)
  {
  fallback:
#ଅନ୍ୟଥା
  {
#endif
    // Just plain vtmx data. No need to cache.
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଗଣନା_2; i++)
    {
      *first_advance = font->em_scale_y (- (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) vmtx.get_advance_without_var_unscaled (*first_glyph));
      first_glyph = &StructAtOffsetUnaligned<hb_codepoint_t> (first_glyph, glyph_stride);
      first_advance = &StructAtOffsetUnaligned<hb_position_t> (first_advance, advance_stride);
    }
    ଫେରନ୍ତୁ;
  }

#ifndef HB_NO_VAR
  /* has_nonzero_coords. */

  ot_font->check_serial (font);
  hb_ot_font_advance_cache_t *advance_cache = ot_font->v.acquire_advance_cache ();
  ଯଦି (!advance_cache)
  {
    // malloc failure. Just use the fallback non-variable path.
    ଯାଅ fallback;
  }

  /* If VVAR is present, use it.*/
  ଅଚଳ OT::VVAR &VVAR = *vmtx.var_table;
  ଯଦି (VVAR.has_data ())
  {
    ଅଚଳ OT::ItemVariationStore &varStore = &VVAR + VVAR.varStore;
    OT::hb_scalar_cache_t *varStore_cache = ot_font->v.acquire_varStore_cache (varStore);

    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଗଣନା_2; i++)
    {
      hb_position_t v;
      unsigned cv;
      ଯଦି (advance_cache->get (*first_glyph, &cv))
	v = cv;
      ଅନ୍ୟଥା
      {
        v = vmtx.get_advance_with_var_unscaled (*first_glyph, font, varStore_cache);
	advance_cache->set (*first_glyph, v);
      }
      *first_advance = font->em_scale_y (- (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) v);
      first_glyph = &StructAtOffsetUnaligned<hb_codepoint_t> (first_glyph, glyph_stride);
      first_advance = &StructAtOffsetUnaligned<hb_position_t> (first_advance, advance_stride);
    }

    ot_font->v.release_varStore_cache (varStore_cache);
    ot_font->v.release_advance_cache (advance_cache);
    ଫେରନ୍ତୁ;
  }

  ଅଚଳ auto &gvar = *ot_face->gvar;
  ଯଦି (gvar.has_data ())
  {
    ଅଚଳ auto &glyf = *ot_face->glyf;
    auto *scratch = glyf.acquire_scratch ();
    ଯଦି (unlikely (!scratch))
    {
      ot_font->v.release_advance_cache (advance_cache);
      ଯାଅ fallback;
    }
    OT::hb_scalar_cache_t *gvar_cache = ot_font->draw.acquire_gvar_cache (gvar);

    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଗଣନା_2; i++)
    {
      hb_position_t v;
      unsigned cv;
      ଯଦି (advance_cache->get (*first_glyph, &cv))
	v = cv;
      ଅନ୍ୟଥା
      {
        v = glyf.get_advance_with_var_unscaled (*first_glyph, font, ସତ୍ୟ, *scratch, gvar_cache);
	advance_cache->set (*first_glyph, v);
      }
      *first_advance = font->em_scale_y (- (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) v);
      first_glyph = &StructAtOffsetUnaligned<hb_codepoint_t> (first_glyph, glyph_stride);
      first_advance = &StructAtOffsetUnaligned<hb_position_t> (first_advance, advance_stride);
    }

    ot_font->draw.release_gvar_cache (gvar_cache);
    glyf.release_scratch (scratch);
    ot_font->v.release_advance_cache (advance_cache);
    ଫେରନ୍ତୁ;
  }

  ot_font->v.release_advance_cache (advance_cache);
  // No VVAR or GVAR.  Just use the fallback non-variable path.
  ଯାଅ fallback;
#endif
}
#endif

#ifndef HB_NO_VERTICAL
HB_HOT
ସ୍ଥିର hb_bool_t
hb_ot_get_glyph_v_origins (hb_font_t *font,
			   ଶୂନ୍ୟ *font_data,
			   unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2,
			   ଅଚଳ hb_codepoint_t *first_glyph,
			   unsigned glyph_stride,
			   hb_position_t *first_x,
			   unsigned x_stride,
			   hb_position_t *first_y,
			   unsigned y_stride,
			   ଶୂନ୍ୟ *user_data HB_UNUSED)
{
  ଅଚଳ hb_ot_font_t *ot_font = (ଅଚଳ hb_ot_font_t *) font_data;
  ଅଚଳ hb_ot_face_t *ot_face = ot_font->ot_face;

  ot_font->check_serial (font);

  /* First, set all the x values to half the advance width. */
  font->get_glyph_h_advances (ଗଣନା_2,
			      first_glyph, glyph_stride,
			      first_x, x_stride);
  ପାଇଁ (unsigned i = 0; i < ଗଣନା_2; i++)
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
  ଅଚଳ OT::VORG &VORG = *ot_face->VORG;
  ଯଦି (origin_cache && VORG.has_data ())
  {
#ifndef HB_NO_VAR
    ଯଦି (!font->has_nonzero_coords)
#endif
    {
      ପାଇଁ (unsigned i = 0; i < ଗଣନା_2; i++)
      {
	hb_position_t origin;
	unsigned cv;
	ଯଦି (origin_cache->get (*first_glyph, &cv))
	  origin = font->y_scale < 0 ? -static_cast<hb_position_t>(cv) : static_cast<hb_position_t>(cv);
	ଅନ୍ୟଥା
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
    ଅନ୍ୟଥା
    {
      ଅଚଳ OT::VVAR &VVAR = *ot_face->vmtx->var_table;
      ଅଚଳ auto &varStore = &VVAR + VVAR.varStore;
      auto *varStore_cache = ot_font->v_origin.acquire_varStore_cache (varStore);
      ପାଇଁ (unsigned i = 0; i < ଗଣନା_2; i++)
      {
	hb_position_t origin;
	unsigned cv;
	ଯଦି (origin_cache->get (*first_glyph, &cv))
	  origin = font->y_scale < 0 ? -static_cast<hb_position_t>(cv) : static_cast<hb_position_t>(cv);
	ଅନ୍ୟଥା
	{
	  origin = font->em_scalef_y (VORG.get_y_origin (*first_glyph) +
				      VVAR.get_vorg_delta_unscaled (*first_glyph,
								    font->coords, font->ଅଙ୍କ_coords,
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
    ଫେରନ୍ତୁ ସତ୍ୟ;
  }

  /* If and only if `vmtx` is present and it's a `glyf` font,
   * we use the top phantom point, deduced from vmtx,glyf[,gvar]. */
  ଅଚଳ auto &vmtx = *ot_face->vmtx;
  ଅଚଳ auto &glyf = *ot_face->glyf;
  ଯଦି (origin_cache && vmtx.has_data() && glyf.has_data ())
  {
    auto *scratch = glyf.acquire_scratch ();
    ଯଦି (unlikely (!scratch))
    {
      ot_font->v_origin.release_origin_cache (origin_cache);
      ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }
    OT::hb_scalar_cache_t *gvar_cache = nullptr;
#ifndef HB_NO_VAR
    gvar_cache = font->has_nonzero_coords ?
					ot_font->draw.acquire_gvar_cache (*ot_face->gvar) :
					nullptr;
#endif

    ପାଇଁ (unsigned i = 0; i < ଗଣନା_2; i++)
    {
      hb_position_t origin;
      unsigned cv;
      ଯଦି (origin_cache->get (*first_glyph, &cv))
	origin = font->y_scale < 0 ? -static_cast<hb_position_t>(cv) : static_cast<hb_position_t>(cv);
      ଅନ୍ୟଥା
      {
	origin = font->em_scalef_y (glyf.get_v_origin_with_var_unscaled (*first_glyph, font, *scratch, gvar_cache));
	origin_cache->set (*first_glyph, font->y_scale < 0 ? -origin : origin);
      }

      *first_y = origin;

      first_glyph = &StructAtOffsetUnaligned<hb_codepoint_t> (first_glyph, glyph_stride);
      first_y = &StructAtOffsetUnaligned<hb_position_t> (first_y, y_stride);
    }

#ifndef HB_NO_VAR
    ଯଦି (gvar_cache)
      ot_font->draw.release_gvar_cache (gvar_cache);
#endif
    glyf.release_scratch (scratch);
    ot_font->v_origin.release_origin_cache (origin_cache);
    ଫେରନ୍ତୁ ସତ୍ୟ;
  }

  /* Otherwise, use glyph extents to center the glyph vertically.
   * If getting glyph extents failed, just use the font ascender. */
  ଯଦି (origin_cache && font->has_glyph_extents_func ())
  {
    hb_font_extents_t font_extents;
    font->get_h_extents_with_fallback (&font_extents);
    hb_position_t font_advance = font_extents.ascender - font_extents.descender;

    ପାଇଁ (unsigned i = 0; i < ଗଣନା_2; i++)
    {
      hb_position_t origin;
      unsigned cv;

      ଯଦି (origin_cache->get (*first_glyph, &cv))
	origin = font->y_scale < 0 ? -static_cast<hb_position_t>(cv) : static_cast<hb_position_t>(cv);
      ଅନ୍ୟଥା
      {
	hb_glyph_extents_t extents = {0};
	ଯଦି (likely (font->get_glyph_extents (*first_glyph, &extents)))
	  origin = hb_clamp_to<hb_position_t> ((int64_t) extents.y_bearing +
					 (((int64_t) font_advance + extents.height) >> 1));
	ଅନ୍ୟଥା
	  origin = font_extents.ascender;

	origin_cache->set (*first_glyph, font->y_scale < 0 ? -origin : origin);
      }

      *first_y = origin;

      first_glyph = &StructAtOffsetUnaligned<hb_codepoint_t> (first_glyph, glyph_stride);
      first_y = &StructAtOffsetUnaligned<hb_position_t> (first_y, y_stride);
    }
  }

  ot_font->v_origin.release_origin_cache (origin_cache);
  ଫେରନ୍ତୁ ସତ୍ୟ;
}
#endif

ସ୍ଥିର hb_bool_t
hb_ot_get_glyph_extents (hb_font_t *font,
			 ଶୂନ୍ୟ *font_data,
			 hb_codepoint_t glyph,
			 hb_glyph_extents_t *extents,
			 ଶୂନ୍ୟ *user_data HB_UNUSED)
{
  ଅଚଳ hb_ot_font_t *ot_font = (ଅଚଳ hb_ot_font_t *) font_data;
  ଅଚଳ hb_ot_face_t *ot_face = ot_font->ot_face;

#ଯଦି !defined(HB_NO_OT_FONT_BITMAP) && !defined(HB_NO_COLOR)
  ଯଦି (ot_face->sbix->get_extents (font, glyph, extents)) ଫେରନ୍ତୁ ସତ୍ୟ;
  ଯଦି (ot_face->CBDT->get_extents (font, glyph, extents)) ଫେରନ୍ତୁ ସତ୍ୟ;
#endif
#ଯଦି !defined(HB_NO_COLOR) && !defined(HB_NO_PAINT)
  ଯଦି (ot_face->COLR->get_extents (font, glyph, extents)) ଫେରନ୍ତୁ ସତ୍ୟ;
#endif
#ifndef HB_NO_VAR_COMPOSITES
  ଯଦି (ot_face->VARC->get_extents (font, glyph, extents)) ଫେରନ୍ତୁ ସତ୍ୟ;
#endif
  ଯଦି (ot_face->glyf->get_extents (font, glyph, extents)) ଫେରନ୍ତୁ ସତ୍ୟ;
#ifndef HB_NO_OT_FONT_CFF
  ଯଦି (ot_face->cff2->get_extents (font, glyph, extents)) ଫେରନ୍ତୁ ସତ୍ୟ;
  ଯଦି (ot_face->cff1->get_extents (font, glyph, extents)) ଫେରନ୍ତୁ ସତ୍ୟ;
#endif

  ଫେରନ୍ତୁ ମିଥ୍ୟା;
}

#ifndef HB_NO_OT_FONT_GLYPH_NAMES
ସ୍ଥିର hb_bool_t
hb_ot_get_glyph_name (hb_font_t *font HB_UNUSED,
		      ଶୂନ୍ୟ *font_data,
		      hb_codepoint_t glyph,
		      ଅକ୍ଷର *name, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା size,
		      ଶୂନ୍ୟ *user_data HB_UNUSED)
{
  ଅଚଳ hb_ot_font_t *ot_font = (ଅଚଳ hb_ot_font_t *) font_data;
  ଅଚଳ hb_ot_face_t *ot_face = ot_font->ot_face;

  ଯଦି (ot_face->post->get_glyph_name (glyph, name, size)) ଫେରନ୍ତୁ ସତ୍ୟ;
#ifndef HB_NO_OT_FONT_CFF
  ଯଦି (ot_face->cff1->get_glyph_name (glyph, name, size)) ଫେରନ୍ତୁ ସତ୍ୟ;
#endif
  ଫେରନ୍ତୁ ମିଥ୍ୟା;
}
ସ୍ଥିର hb_bool_t
hb_ot_get_glyph_from_name (hb_font_t *font HB_UNUSED,
			   ଶୂନ୍ୟ *font_data,
			   ଅଚଳ ଅକ୍ଷର *name, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len,
			   hb_codepoint_t *glyph,
			   ଶୂନ୍ୟ *user_data HB_UNUSED)
{
  ଅଚଳ hb_ot_font_t *ot_font = (ଅଚଳ hb_ot_font_t *) font_data;
  ଅଚଳ hb_ot_face_t *ot_face = ot_font->ot_face;

  ଯଦି (ot_face->post->get_glyph_from_name (name, len, glyph)) ଫେରନ୍ତୁ ସତ୍ୟ;
#ifndef HB_NO_OT_FONT_CFF
    ଯଦି (ot_face->cff1->get_glyph_from_name (name, len, glyph)) ଫେରନ୍ତୁ ସତ୍ୟ;
#endif
  ଫେରନ୍ତୁ ମିଥ୍ୟା;
}
#endif

ସ୍ଥିର hb_bool_t
hb_ot_get_font_h_extents (hb_font_t *font,
			  ଶୂନ୍ୟ *font_data HB_UNUSED,
			  hb_font_extents_t *metrics,
			  ଶୂନ୍ୟ *user_data HB_UNUSED)
{
  ଫେରନ୍ତୁ _hb_ot_metrics_get_position_common (font, HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER, &metrics->ascender) &&
	 _hb_ot_metrics_get_position_common (font, HB_OT_METRICS_TAG_HORIZONTAL_DESCENDER, &metrics->descender) &&
	 _hb_ot_metrics_get_position_common (font, HB_OT_METRICS_TAG_HORIZONTAL_ଧାଡ଼ି_GAP, &metrics->ଧାଡ଼ି_gap);
}

#ifndef HB_NO_VERTICAL
ସ୍ଥିର hb_bool_t
hb_ot_get_font_v_extents (hb_font_t *font,
			  ଶୂନ୍ୟ *font_data HB_UNUSED,
			  hb_font_extents_t *metrics,
			  ଶୂନ୍ୟ *user_data HB_UNUSED)
{
  ଫେରନ୍ତୁ _hb_ot_metrics_get_position_common (font, HB_OT_METRICS_TAG_VERTICAL_ASCENDER, &metrics->ascender) &&
	 _hb_ot_metrics_get_position_common (font, HB_OT_METRICS_TAG_VERTICAL_DESCENDER, &metrics->descender) &&
	 _hb_ot_metrics_get_position_common (font, HB_OT_METRICS_TAG_VERTICAL_ଧାଡ଼ି_GAP, &metrics->ଧାଡ଼ି_gap);
}
#endif

#ifndef HB_NO_DRAW
ସ୍ଥିର hb_bool_t
hb_ot_draw_glyph_or_fail (hb_font_t *font,
			  ଶୂନ୍ୟ *font_data HB_UNUSED,
			  hb_codepoint_t glyph,
			  hb_draw_funcs_t *draw_funcs, ଶୂନ୍ୟ *draw_data,
			  ଶୂନ୍ୟ *user_data)
{
  ଅଚଳ hb_ot_font_t *ot_font = (ଅଚଳ hb_ot_font_t *) font_data;
  hb_draw_session_t draw_session {draw_funcs, draw_data};

  OT::hb_scalar_cache_t *gvar_cache = nullptr;
#ifndef HB_NO_VAR
  ଯଦି (font->ଅଙ୍କ_coords)
  {
    ot_font->check_serial (font);
    gvar_cache = ot_font->draw.acquire_gvar_cache (*ot_font->ot_face->gvar);
  }
#endif
  HB_SCOPE_GUARD (ot_font->draw.release_gvar_cache (gvar_cache));

#ifndef HB_NO_VAR_COMPOSITES
  ଯଦି (font->face->table.VARC->get_path (font, glyph, draw_session)) ଫେରନ୍ତୁ ସତ୍ୟ;
#endif
  // Keep the following in synch with VARC::get_path_at()
  ଯଦି (font->face->table.glyf->get_path (font, glyph, draw_session, gvar_cache,
				       &draw_session.get_budget ())) ଫେରନ୍ତୁ ସତ୍ୟ;

#ifndef HB_NO_CFF
  ଯଦି (font->face->table.cff2->get_path (font, glyph, draw_session,
				       &draw_session.get_budget ())) ଫେରନ୍ତୁ ସତ୍ୟ;
  ଯଦି (font->face->table.cff1->get_path (font, glyph, draw_session,
				       &draw_session.get_budget ())) ଫେରନ୍ତୁ ସତ୍ୟ;
#endif

  ଫେରନ୍ତୁ ମିଥ୍ୟା;
}
#endif

#ifndef HB_NO_PAINT
ସ୍ଥିର hb_bool_t
hb_ot_paint_glyph_or_fail (hb_font_t *font,
			   ଶୂନ୍ୟ *font_data,
			   hb_codepoint_t glyph,
			   hb_paint_funcs_t *paint_funcs, ଶୂନ୍ୟ *paint_data,
			   unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା palette,
			   hb_color_t foreground,
			   ଶୂନ୍ୟ *user_data)
{
#ifndef HB_NO_COLOR
  ଯଦି (font->face->table.COLR->paint_glyph (font, glyph, paint_funcs, paint_data, palette, foreground)) ଫେରନ୍ତୁ ସତ୍ୟ;
#ifndef HB_NO_SVG
  ଯଦି (font->face->table.SVG->paint_glyph (font, glyph, paint_funcs, paint_data)) ଫେରନ୍ତୁ ସତ୍ୟ;
#endif
#ifndef HB_NO_OT_FONT_BITMAP
  ଯଦି (font->face->table.CBDT->paint_glyph (font, glyph, paint_funcs, paint_data)) ଫେରନ୍ତୁ ସତ୍ୟ;
  ଯଦି (font->face->table.sbix->paint_glyph (font, glyph, paint_funcs, paint_data)) ଫେରନ୍ତୁ ସତ୍ୟ;
#endif
#endif
  ଫେରନ୍ତୁ ମିଥ୍ୟା;
}
#endif

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ free_static_ot_funcs ();

ସ୍ଥିର ସଂରଚନା hb_ot_font_funcs_lazy_loader_t : hb_font_funcs_lazy_loader_t<hb_ot_font_funcs_lazy_loader_t>
{
  ସ୍ଥିର hb_font_funcs_t *create ()
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

    ଫେରନ୍ତୁ funcs;
  }
} static_ot_funcs;

ସ୍ଥିର ଧାଡ଼ିଭିତରେ
ଶୂନ୍ୟ free_static_ot_funcs ()
{
  static_ot_funcs.free_instance ();
}

ସ୍ଥିର hb_font_funcs_t *
_hb_ot_get_font_funcs ()
{
  ଫେରନ୍ତୁ static_ot_funcs.get_unconst ();
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
ଶୂନ୍ୟ
hb_ot_font_set_funcs (hb_font_t *font)
{
  hb_ot_font_t *ot_font = _hb_ot_font_create (font);
  ଯଦି (unlikely (!ot_font))
    ଫେରନ୍ତୁ;

  hb_font_set_funcs (font,
		     _hb_ot_get_font_funcs (),
		     ot_font,
		     _hb_ot_font_destroy);
}

#endif
