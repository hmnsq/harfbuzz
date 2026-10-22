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

#அந்தர்பாவ "hb.hh"

#ifndef HB_NO_OT_FONT

#அந்தர்பாவ "hb-ot.h"

#அந்தர்பாவ "hb-cache.hh"
#அந்தர்பாவ "hb-font.hh"
#அந்தர்பாவ "hb-machinery.hh"
#அந்தர்பாவ "hb-ot-face.hh"

#அந்தர்பாவ "hb-ot-cmap-table.hh"
#அந்தர்பாவ "hb-ot-glyf-table.hh"
#அந்தர்பாவ "hb-ot-var-gvar-table.hh"
#அந்தர்பாவ "hb-ot-cff2-table.hh"
#அந்தர்பாவ "hb-ot-cff1-table.hh"
#அந்தர்பாவ "hb-ot-hmtx-table.hh"
#அந்தர்பாவ "hb-ot-post-table.hh"
#அந்தர்பாவ "hb-ot-stat-table.hh"
#அந்தர்பாவ "hb-ot-var-varc-table.hh"
#அந்தர்பாவ "hb-ot-vorg-table.hh"
#அந்தர்பாவ "OT/Color/CBDT/CBDT.hh"
#அந்தர்பாவ "OT/Color/COLR/COLR.hh"
#அந்தர்பாவ "OT/Color/sbix/sbix.hh"
#அந்தர்பாவ "OT/Color/svg/svg.hh"


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
static_assert (அளவு (hb_ot_font_advance_cache_t) == 1024, "");

using hb_ot_font_origin_cache_t = hb_cache_t<20, 20>;
static_assert (அளவு (hb_ot_font_origin_cache_t) == 1024, "");

கட்டமைப்பு hb_ot_font_t
{
  மாறிலி hb_ot_face_t *ot_face;

  mutable hb_atomic_t<முழு> cached_serial;
  mutable hb_atomic_t<முழு> cached_coords_serial;

  கட்டமைப்பு direction_cache_t
  {
    mutable hb_atomic_t<hb_ot_font_advance_cache_t *> advance_cache;
    mutable hb_atomic_t<OT::hb_scalar_cache_t *> varStore_cache;

    ~direction_cache_t ()
    {
      clear ();
    }

    hb_ot_font_advance_cache_t *acquire_advance_cache () மாறிலி
    {
    retry:
      auto *cache = advance_cache.get_acquire ();
      எனில் (!cache)
      {
        cache = (hb_ot_font_advance_cache_t *) hb_malloc (அளவு (hb_ot_font_advance_cache_t));
	எனில் (!cache)
	  திரும்பு nullptr;
	புதிய (cache) hb_ot_font_advance_cache_t;
	திரும்பு cache;
      }
      எனில் (advance_cache.cmpexch (cache, nullptr))
        திரும்பு cache;
      இல்லையெனில்
        செல் retry;
    }
    வெறுமை release_advance_cache (hb_ot_font_advance_cache_t *cache) மாறிலி
    {
      எனில் (!cache)
        திரும்பு;
      எனில் (!advance_cache.cmpexch (nullptr, cache))
        hb_free (cache);
    }
    வெறுமை clear_advance_cache () மாறிலி
    {
    retry:
      auto *cache = advance_cache.get_acquire ();
      எனில் (!cache)
	திரும்பு;
      எனில் (advance_cache.cmpexch (cache, nullptr))
	hb_free (cache);
      இல்லையெனில்
        செல் retry;
    }

    OT::hb_scalar_cache_t *acquire_varStore_cache (மாறிலி OT::ItemVariationStore &varStore) மாறிலி
    {
    retry:
      auto *cache = varStore_cache.get_acquire ();
      எனில் (!cache)
	திரும்பு varStore.create_cache ();
      எனில் (varStore_cache.cmpexch (cache, nullptr))
	திரும்பு cache;
      இல்லையெனில்
	செல் retry;
    }
    வெறுமை release_varStore_cache (OT::hb_scalar_cache_t *cache) மாறிலி
    {
      எனில் (!cache)
	திரும்பு;
      எனில் (!varStore_cache.cmpexch (nullptr, cache))
	OT::ItemVariationStore::destroy_cache (cache);
    }
    வெறுமை clear_varStore_cache () மாறிலி
    {
    retry:
      auto *cache = varStore_cache.get_acquire ();
      எனில் (!cache)
	திரும்பு;
      எனில் (varStore_cache.cmpexch (cache, nullptr))
	OT::ItemVariationStore::destroy_cache (cache);
      இல்லையெனில்
	செல் retry;
    }

    வெறுமை clear () மாறிலி
    {
      clear_advance_cache ();
      clear_varStore_cache ();
    }

  } h, v;

  கட்டமைப்பு origin_cache_t
  {
    mutable hb_atomic_t<hb_ot_font_origin_cache_t *> origin_cache;
    mutable hb_atomic_t<OT::hb_scalar_cache_t *> varStore_cache;

    ~origin_cache_t ()
    {
      clear ();
    }

    hb_ot_font_origin_cache_t *acquire_origin_cache () மாறிலி
    {
    retry:
      auto *cache = origin_cache.get_acquire ();
      எனில் (!cache)
      {
        cache = (hb_ot_font_origin_cache_t *) hb_malloc (அளவு (hb_ot_font_origin_cache_t));
	எனில் (!cache)
	  திரும்பு nullptr;
	புதிய (cache) hb_ot_font_origin_cache_t;
	திரும்பு cache;
      }
      எனில் (origin_cache.cmpexch (cache, nullptr))
        திரும்பு cache;
      இல்லையெனில்
        செல் retry;
    }
    வெறுமை release_origin_cache (hb_ot_font_origin_cache_t *cache) மாறிலி
    {
      எனில் (!cache)
        திரும்பு;
      எனில் (!origin_cache.cmpexch (nullptr, cache))
        hb_free (cache);
    }
    வெறுமை clear_origin_cache () மாறிலி
    {
    retry:
      auto *cache = origin_cache.get_acquire ();
      எனில் (!cache)
	திரும்பு;
      எனில் (origin_cache.cmpexch (cache, nullptr))
	hb_free (cache);
      இல்லையெனில்
        செல் retry;
    }

    OT::hb_scalar_cache_t *acquire_varStore_cache (மாறிலி OT::ItemVariationStore &varStore) மாறிலி
    {
    retry:
      auto *cache = varStore_cache.get_acquire ();
      எனில் (!cache)
	திரும்பு varStore.create_cache ();
      எனில் (varStore_cache.cmpexch (cache, nullptr))
	திரும்பு cache;
      இல்லையெனில்
	செல் retry;
    }
    வெறுமை release_varStore_cache (OT::hb_scalar_cache_t *cache) மாறிலி
    {
      எனில் (!cache)
	திரும்பு;
      எனில் (!varStore_cache.cmpexch (nullptr, cache))
	OT::ItemVariationStore::destroy_cache (cache);
    }
    வெறுமை clear_varStore_cache () மாறிலி
    {
    retry:
      auto *cache = varStore_cache.get_acquire ();
      எனில் (!cache)
	திரும்பு;
      எனில் (varStore_cache.cmpexch (cache, nullptr))
	OT::ItemVariationStore::destroy_cache (cache);
      இல்லையெனில்
	செல் retry;
    }

    வெறுமை clear () மாறிலி
    {
      clear_origin_cache ();
      clear_varStore_cache ();
    }
  } v_origin;

  கட்டமைப்பு draw_cache_t
  {
    mutable hb_atomic_t<OT::hb_scalar_cache_t *> gvar_cache;

    ~draw_cache_t ()
    {
      clear ();
    }

    OT::hb_scalar_cache_t *acquire_gvar_cache (மாறிலி OT::gvar_accelerator_t &gvar) மாறிலி
    {
    retry:
      auto *cache = gvar_cache.get_acquire ();
      எனில் (!cache)
	திரும்பு gvar.create_cache ();
      எனில் (gvar_cache.cmpexch (cache, nullptr))
	திரும்பு cache;
      இல்லையெனில்
	செல் retry;
    }
    வெறுமை release_gvar_cache (OT::hb_scalar_cache_t *cache) மாறிலி
    {
      எனில் (!cache)
	திரும்பு;
      எனில் (!gvar_cache.cmpexch (nullptr, cache))
	OT::gvar_accelerator_t::destroy_cache (cache);
    }
    வெறுமை clear_gvar_cache () மாறிலி
    {
    retry:
      auto *cache = gvar_cache.get_acquire ();
      எனில் (!cache)
	திரும்பு;
      எனில் (gvar_cache.cmpexch (cache, nullptr))
	OT::gvar_accelerator_t::destroy_cache (cache);
      இல்லையெனில்
	செல் retry;
    }

    வெறுமை clear () மாறிலி
    {
      clear_gvar_cache ();
    }
  } draw;

  வெறுமை check_serial (hb_font_t *font) மாறிலி
  {
    முழு font_serial = font->serial.get_acquire ();
    எனில் (cached_serial.get_acquire () != font_serial)
    {
      /* These caches are dependent on scale and synthetic settings.
       * Any change to the font invalidates them. */
      v_origin.clear ();

      cached_serial.set_release (font_serial);
    }

    முழு font_serial_coords = font->serial_coords.get_acquire ();
    எனில் (cached_coords_serial.get_acquire () != font_serial_coords)
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

நிலையான hb_ot_font_t *
_hb_ot_font_create (hb_font_t *font)
{
  hb_ot_font_t *ot_font = (hb_ot_font_t *) hb_calloc (1, அளவு (hb_ot_font_t));
  எனில் (unlikely (!ot_font))
    திரும்பு nullptr;

  ot_font->ot_face = &font->face->table;

  திரும்பு ot_font;
}

நிலையான வெறுமை
_hb_ot_font_destroy (வெறுமை *font_data)
{
  hb_ot_font_t *ot_font = (hb_ot_font_t *) font_data;

  ot_font->~hb_ot_font_t ();

  hb_free (ot_font);
}

நிலையான hb_bool_t
hb_ot_get_nominal_glyph (hb_font_t *font HB_UNUSED,
			 வெறுமை *font_data,
			 hb_codepoint_t unicode,
			 hb_codepoint_t *glyph,
			 வெறுமை *user_data HB_UNUSED)
{
  மாறிலி hb_ot_font_t *ot_font = (மாறிலி hb_ot_font_t *) font_data;
  மாறிலி hb_ot_face_t *ot_face = ot_font->ot_face;
  திரும்பு ot_face->cmap->get_nominal_glyph (unicode, glyph);
}

நிலையான unsigned முழு
hb_ot_get_nominal_glyphs (hb_font_t *font HB_UNUSED,
			  வெறுமை *font_data,
			  unsigned முழு மொத்தம்,
			  மாறிலி hb_codepoint_t *first_unicode,
			  unsigned முழு unicode_stride,
			  hb_codepoint_t *first_glyph,
			  unsigned முழு glyph_stride,
			  வெறுமை *user_data HB_UNUSED)
{
  மாறிலி hb_ot_font_t *ot_font = (மாறிலி hb_ot_font_t *) font_data;
  மாறிலி hb_ot_face_t *ot_face = ot_font->ot_face;
  திரும்பு ot_face->cmap->get_nominal_glyphs (மொத்தம்,
					    first_unicode, unicode_stride,
					    first_glyph, glyph_stride);
}

நிலையான hb_bool_t
hb_ot_get_variation_glyph (hb_font_t *font HB_UNUSED,
			   வெறுமை *font_data,
			   hb_codepoint_t unicode,
			   hb_codepoint_t variation_selector,
			   hb_codepoint_t *glyph,
			   வெறுமை *user_data HB_UNUSED)
{
  மாறிலி hb_ot_font_t *ot_font = (மாறிலி hb_ot_font_t *) font_data;
  மாறிலி hb_ot_face_t *ot_face = ot_font->ot_face;
  திரும்பு ot_face->cmap->get_variation_glyph (unicode,
                                             variation_selector, glyph);
}

நிலையான வெறுமை
hb_ot_get_glyph_h_advances (hb_font_t* font, வெறுமை* font_data,
			    unsigned மொத்தம்,
			    மாறிலி hb_codepoint_t *first_glyph,
			    unsigned glyph_stride,
			    hb_position_t *first_advance,
			    unsigned advance_stride,
			    வெறுமை *user_data HB_UNUSED)
{
  // Duplicated in v_advances. Ugly. Keep in sync'ish.

  மாறிலி hb_ot_font_t *ot_font = (மாறிலி hb_ot_font_t *) font_data;
  மாறிலி hb_ot_face_t *ot_face = ot_font->ot_face;
  மாறிலி OT::hmtx_accelerator_t &hmtx = *ot_face->hmtx;

  எனில் (unlikely (!hmtx.has_data ()))
  {
    hb_position_t advance = font->face->get_upem () / 2;
    advance = font->em_scale_x (advance);
    க்கு (unsigned முழு i = 0; i < மொத்தம்; i++)
    {
      *first_advance = advance;
      first_advance = &StructAtOffsetUnaligned<hb_position_t> (first_advance, advance_stride);
    }
    திரும்பு;
  }

#ifndef HB_NO_VAR
  எனில் (!font->has_nonzero_coords)
  {
  fallback:
#இல்லையெனில்
  {
#endif
    // Just plain htmx data. No need to cache.
    க்கு (unsigned முழு i = 0; i < மொத்தம்; i++)
    {
      *first_advance = font->em_scale_x (hmtx.get_advance_without_var_unscaled (*first_glyph));
      first_glyph = &StructAtOffsetUnaligned<hb_codepoint_t> (first_glyph, glyph_stride);
      first_advance = &StructAtOffsetUnaligned<hb_position_t> (first_advance, advance_stride);
    }
    திரும்பு;
  }

#ifndef HB_NO_VAR
  /* has_nonzero_coords. */

  ot_font->check_serial (font);
  hb_ot_font_advance_cache_t *advance_cache = ot_font->h.acquire_advance_cache ();
  எனில் (!advance_cache)
  {
    // malloc failure. Just use the fallback non-variable path.
    செல் fallback;
  }

  /* If HVAR is present, use it.*/
  மாறிலி OT::HVAR &HVAR = *hmtx.var_table;
  எனில் (HVAR.has_data ())
  {
    மாறிலி OT::ItemVariationStore &varStore = &HVAR + HVAR.varStore;
    OT::hb_scalar_cache_t *varStore_cache = ot_font->h.acquire_varStore_cache (varStore);

    க்கு (unsigned முழு i = 0; i < மொத்தம்; i++)
    {
      hb_position_t v;
      unsigned cv;
      எனில் (advance_cache->get (*first_glyph, &cv))
	v = cv;
      இல்லையெனில்
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
    திரும்பு;
  }

  மாறிலி auto &gvar = *ot_face->gvar;
  எனில் (gvar.has_data ())
  {
    மாறிலி auto &glyf = *ot_face->glyf;
    auto *scratch = glyf.acquire_scratch ();
    எனில் (unlikely (!scratch))
    {
      ot_font->h.release_advance_cache (advance_cache);
      செல் fallback;
    }
    OT::hb_scalar_cache_t *gvar_cache = ot_font->draw.acquire_gvar_cache (gvar);

    க்கு (unsigned முழு i = 0; i < மொத்தம்; i++)
    {
      hb_position_t v;
      unsigned cv;
      எனில் (advance_cache->get (*first_glyph, &cv))
	v = cv;
      இல்லையெனில்
      {
        v = glyf.get_advance_with_var_unscaled (*first_glyph, font, பொய், *scratch, gvar_cache);
	advance_cache->set (*first_glyph, v);
      }
      *first_advance = font->em_scale_x (v);
      first_glyph = &StructAtOffsetUnaligned<hb_codepoint_t> (first_glyph, glyph_stride);
      first_advance = &StructAtOffsetUnaligned<hb_position_t> (first_advance, advance_stride);
    }

    ot_font->draw.release_gvar_cache (gvar_cache);
    glyf.release_scratch (scratch);
    ot_font->h.release_advance_cache (advance_cache);
    திரும்பு;
  }

  ot_font->h.release_advance_cache (advance_cache);
  // No HVAR or GVAR.  Just use the fallback non-variable path.
  செல் fallback;
#endif
}

#ifndef HB_NO_VERTICAL
நிலையான வெறுமை
hb_ot_get_glyph_v_advances (hb_font_t* font, வெறுமை* font_data,
			    unsigned மொத்தம்,
			    மாறிலி hb_codepoint_t *first_glyph,
			    unsigned glyph_stride,
			    hb_position_t *first_advance,
			    unsigned advance_stride,
			    வெறுமை *user_data HB_UNUSED)
{
  // Duplicated from h_advances. Ugly. Keep in sync'ish.

  மாறிலி hb_ot_font_t *ot_font = (மாறிலி hb_ot_font_t *) font_data;
  மாறிலி hb_ot_face_t *ot_face = ot_font->ot_face;
  மாறிலி OT::vmtx_accelerator_t &vmtx = *ot_face->vmtx;

  எனில் (unlikely (!vmtx.has_data ()))
  {
    hb_font_extents_t font_extents;
    font->get_h_extents_with_fallback (&font_extents);
    hb_position_t advance = font_extents.descender - font_extents.ascender;
    க்கு (unsigned முழு i = 0; i < மொத்தம்; i++)
    {
      *first_advance = advance;
      first_advance = &StructAtOffsetUnaligned<hb_position_t> (first_advance, advance_stride);
    }
    திரும்பு;
  }

#ifndef HB_NO_VAR
  எனில் (!font->has_nonzero_coords)
  {
  fallback:
#இல்லையெனில்
  {
#endif
    // Just plain vtmx data. No need to cache.
    க்கு (unsigned முழு i = 0; i < மொத்தம்; i++)
    {
      *first_advance = font->em_scale_y (- (முழு) vmtx.get_advance_without_var_unscaled (*first_glyph));
      first_glyph = &StructAtOffsetUnaligned<hb_codepoint_t> (first_glyph, glyph_stride);
      first_advance = &StructAtOffsetUnaligned<hb_position_t> (first_advance, advance_stride);
    }
    திரும்பு;
  }

#ifndef HB_NO_VAR
  /* has_nonzero_coords. */

  ot_font->check_serial (font);
  hb_ot_font_advance_cache_t *advance_cache = ot_font->v.acquire_advance_cache ();
  எனில் (!advance_cache)
  {
    // malloc failure. Just use the fallback non-variable path.
    செல் fallback;
  }

  /* If VVAR is present, use it.*/
  மாறிலி OT::VVAR &VVAR = *vmtx.var_table;
  எனில் (VVAR.has_data ())
  {
    மாறிலி OT::ItemVariationStore &varStore = &VVAR + VVAR.varStore;
    OT::hb_scalar_cache_t *varStore_cache = ot_font->v.acquire_varStore_cache (varStore);

    க்கு (unsigned முழு i = 0; i < மொத்தம்; i++)
    {
      hb_position_t v;
      unsigned cv;
      எனில் (advance_cache->get (*first_glyph, &cv))
	v = cv;
      இல்லையெனில்
      {
        v = vmtx.get_advance_with_var_unscaled (*first_glyph, font, varStore_cache);
	advance_cache->set (*first_glyph, v);
      }
      *first_advance = font->em_scale_y (- (முழு) v);
      first_glyph = &StructAtOffsetUnaligned<hb_codepoint_t> (first_glyph, glyph_stride);
      first_advance = &StructAtOffsetUnaligned<hb_position_t> (first_advance, advance_stride);
    }

    ot_font->v.release_varStore_cache (varStore_cache);
    ot_font->v.release_advance_cache (advance_cache);
    திரும்பு;
  }

  மாறிலி auto &gvar = *ot_face->gvar;
  எனில் (gvar.has_data ())
  {
    மாறிலி auto &glyf = *ot_face->glyf;
    auto *scratch = glyf.acquire_scratch ();
    எனில் (unlikely (!scratch))
    {
      ot_font->v.release_advance_cache (advance_cache);
      செல் fallback;
    }
    OT::hb_scalar_cache_t *gvar_cache = ot_font->draw.acquire_gvar_cache (gvar);

    க்கு (unsigned முழு i = 0; i < மொத்தம்; i++)
    {
      hb_position_t v;
      unsigned cv;
      எனில் (advance_cache->get (*first_glyph, &cv))
	v = cv;
      இல்லையெனில்
      {
        v = glyf.get_advance_with_var_unscaled (*first_glyph, font, உண்மை, *scratch, gvar_cache);
	advance_cache->set (*first_glyph, v);
      }
      *first_advance = font->em_scale_y (- (முழு) v);
      first_glyph = &StructAtOffsetUnaligned<hb_codepoint_t> (first_glyph, glyph_stride);
      first_advance = &StructAtOffsetUnaligned<hb_position_t> (first_advance, advance_stride);
    }

    ot_font->draw.release_gvar_cache (gvar_cache);
    glyf.release_scratch (scratch);
    ot_font->v.release_advance_cache (advance_cache);
    திரும்பு;
  }

  ot_font->v.release_advance_cache (advance_cache);
  // No VVAR or GVAR.  Just use the fallback non-variable path.
  செல் fallback;
#endif
}
#endif

#ifndef HB_NO_VERTICAL
HB_HOT
நிலையான hb_bool_t
hb_ot_get_glyph_v_origins (hb_font_t *font,
			   வெறுமை *font_data,
			   unsigned முழு மொத்தம்,
			   மாறிலி hb_codepoint_t *first_glyph,
			   unsigned glyph_stride,
			   hb_position_t *first_x,
			   unsigned x_stride,
			   hb_position_t *first_y,
			   unsigned y_stride,
			   வெறுமை *user_data HB_UNUSED)
{
  மாறிலி hb_ot_font_t *ot_font = (மாறிலி hb_ot_font_t *) font_data;
  மாறிலி hb_ot_face_t *ot_face = ot_font->ot_face;

  ot_font->check_serial (font);

  /* First, set all the x values to half the advance width. */
  font->get_glyph_h_advances (மொத்தம்,
			      first_glyph, glyph_stride,
			      first_x, x_stride);
  க்கு (unsigned i = 0; i < மொத்தம்; i++)
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
  மாறிலி OT::VORG &VORG = *ot_face->VORG;
  எனில் (origin_cache && VORG.has_data ())
  {
#ifndef HB_NO_VAR
    எனில் (!font->has_nonzero_coords)
#endif
    {
      க்கு (unsigned i = 0; i < மொத்தம்; i++)
      {
	hb_position_t origin;
	unsigned cv;
	எனில் (origin_cache->get (*first_glyph, &cv))
	  origin = font->y_scale < 0 ? -static_cast<hb_position_t>(cv) : static_cast<hb_position_t>(cv);
	இல்லையெனில்
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
    இல்லையெனில்
    {
      மாறிலி OT::VVAR &VVAR = *ot_face->vmtx->var_table;
      மாறிலி auto &varStore = &VVAR + VVAR.varStore;
      auto *varStore_cache = ot_font->v_origin.acquire_varStore_cache (varStore);
      க்கு (unsigned i = 0; i < மொத்தம்; i++)
      {
	hb_position_t origin;
	unsigned cv;
	எனில் (origin_cache->get (*first_glyph, &cv))
	  origin = font->y_scale < 0 ? -static_cast<hb_position_t>(cv) : static_cast<hb_position_t>(cv);
	இல்லையெனில்
	{
	  origin = font->em_scalef_y (VORG.get_y_origin (*first_glyph) +
				      VVAR.get_vorg_delta_unscaled (*first_glyph,
								    font->coords, font->இலக்கம்_coords,
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
    திரும்பு உண்மை;
  }

  /* If and only if `vmtx` is present and it's a `glyf` font,
   * we use the top phantom point, deduced from vmtx,glyf[,gvar]. */
  மாறிலி auto &vmtx = *ot_face->vmtx;
  மாறிலி auto &glyf = *ot_face->glyf;
  எனில் (origin_cache && vmtx.has_data() && glyf.has_data ())
  {
    auto *scratch = glyf.acquire_scratch ();
    எனில் (unlikely (!scratch))
    {
      ot_font->v_origin.release_origin_cache (origin_cache);
      திரும்பு பொய்;
    }
    OT::hb_scalar_cache_t *gvar_cache = nullptr;
#ifndef HB_NO_VAR
    gvar_cache = font->has_nonzero_coords ?
					ot_font->draw.acquire_gvar_cache (*ot_face->gvar) :
					nullptr;
#endif

    க்கு (unsigned i = 0; i < மொத்தம்; i++)
    {
      hb_position_t origin;
      unsigned cv;
      எனில் (origin_cache->get (*first_glyph, &cv))
	origin = font->y_scale < 0 ? -static_cast<hb_position_t>(cv) : static_cast<hb_position_t>(cv);
      இல்லையெனில்
      {
	origin = font->em_scalef_y (glyf.get_v_origin_with_var_unscaled (*first_glyph, font, *scratch, gvar_cache));
	origin_cache->set (*first_glyph, font->y_scale < 0 ? -origin : origin);
      }

      *first_y = origin;

      first_glyph = &StructAtOffsetUnaligned<hb_codepoint_t> (first_glyph, glyph_stride);
      first_y = &StructAtOffsetUnaligned<hb_position_t> (first_y, y_stride);
    }

#ifndef HB_NO_VAR
    எனில் (gvar_cache)
      ot_font->draw.release_gvar_cache (gvar_cache);
#endif
    glyf.release_scratch (scratch);
    ot_font->v_origin.release_origin_cache (origin_cache);
    திரும்பு உண்மை;
  }

  /* Otherwise, use glyph extents to center the glyph vertically.
   * If getting glyph extents failed, just use the font ascender. */
  எனில் (origin_cache && font->has_glyph_extents_func ())
  {
    hb_font_extents_t font_extents;
    font->get_h_extents_with_fallback (&font_extents);
    hb_position_t font_advance = font_extents.ascender - font_extents.descender;

    க்கு (unsigned i = 0; i < மொத்தம்; i++)
    {
      hb_position_t origin;
      unsigned cv;

      எனில் (origin_cache->get (*first_glyph, &cv))
	origin = font->y_scale < 0 ? -static_cast<hb_position_t>(cv) : static_cast<hb_position_t>(cv);
      இல்லையெனில்
      {
	hb_glyph_extents_t extents = {0};
	எனில் (likely (font->get_glyph_extents (*first_glyph, &extents)))
	  origin = hb_clamp_to<hb_position_t> ((int64_t) extents.y_bearing +
					 (((int64_t) font_advance + extents.height) >> 1));
	இல்லையெனில்
	  origin = font_extents.ascender;

	origin_cache->set (*first_glyph, font->y_scale < 0 ? -origin : origin);
      }

      *first_y = origin;

      first_glyph = &StructAtOffsetUnaligned<hb_codepoint_t> (first_glyph, glyph_stride);
      first_y = &StructAtOffsetUnaligned<hb_position_t> (first_y, y_stride);
    }
  }

  ot_font->v_origin.release_origin_cache (origin_cache);
  திரும்பு உண்மை;
}
#endif

நிலையான hb_bool_t
hb_ot_get_glyph_extents (hb_font_t *font,
			 வெறுமை *font_data,
			 hb_codepoint_t glyph,
			 hb_glyph_extents_t *extents,
			 வெறுமை *user_data HB_UNUSED)
{
  மாறிலி hb_ot_font_t *ot_font = (மாறிலி hb_ot_font_t *) font_data;
  மாறிலி hb_ot_face_t *ot_face = ot_font->ot_face;

#எனில் !defined(HB_NO_OT_FONT_BITMAP) && !defined(HB_NO_COLOR)
  எனில் (ot_face->sbix->get_extents (font, glyph, extents)) திரும்பு உண்மை;
  எனில் (ot_face->CBDT->get_extents (font, glyph, extents)) திரும்பு உண்மை;
#endif
#எனில் !defined(HB_NO_COLOR) && !defined(HB_NO_PAINT)
  எனில் (ot_face->COLR->get_extents (font, glyph, extents)) திரும்பு உண்மை;
#endif
#ifndef HB_NO_VAR_COMPOSITES
  எனில் (ot_face->VARC->get_extents (font, glyph, extents)) திரும்பு உண்மை;
#endif
  எனில் (ot_face->glyf->get_extents (font, glyph, extents)) திரும்பு உண்மை;
#ifndef HB_NO_OT_FONT_CFF
  எனில் (ot_face->cff2->get_extents (font, glyph, extents)) திரும்பு உண்மை;
  எனில் (ot_face->cff1->get_extents (font, glyph, extents)) திரும்பு உண்மை;
#endif

  திரும்பு பொய்;
}

#ifndef HB_NO_OT_FONT_GLYPH_NAMES
நிலையான hb_bool_t
hb_ot_get_glyph_name (hb_font_t *font HB_UNUSED,
		      வெறுமை *font_data,
		      hb_codepoint_t glyph,
		      எழுத்து *name, unsigned முழு size,
		      வெறுமை *user_data HB_UNUSED)
{
  மாறிலி hb_ot_font_t *ot_font = (மாறிலி hb_ot_font_t *) font_data;
  மாறிலி hb_ot_face_t *ot_face = ot_font->ot_face;

  எனில் (ot_face->post->get_glyph_name (glyph, name, size)) திரும்பு உண்மை;
#ifndef HB_NO_OT_FONT_CFF
  எனில் (ot_face->cff1->get_glyph_name (glyph, name, size)) திரும்பு உண்மை;
#endif
  திரும்பு பொய்;
}
நிலையான hb_bool_t
hb_ot_get_glyph_from_name (hb_font_t *font HB_UNUSED,
			   வெறுமை *font_data,
			   மாறிலி எழுத்து *name, முழு len,
			   hb_codepoint_t *glyph,
			   வெறுமை *user_data HB_UNUSED)
{
  மாறிலி hb_ot_font_t *ot_font = (மாறிலி hb_ot_font_t *) font_data;
  மாறிலி hb_ot_face_t *ot_face = ot_font->ot_face;

  எனில் (ot_face->post->get_glyph_from_name (name, len, glyph)) திரும்பு உண்மை;
#ifndef HB_NO_OT_FONT_CFF
    எனில் (ot_face->cff1->get_glyph_from_name (name, len, glyph)) திரும்பு உண்மை;
#endif
  திரும்பு பொய்;
}
#endif

நிலையான hb_bool_t
hb_ot_get_font_h_extents (hb_font_t *font,
			  வெறுமை *font_data HB_UNUSED,
			  hb_font_extents_t *metrics,
			  வெறுமை *user_data HB_UNUSED)
{
  திரும்பு _hb_ot_metrics_get_position_common (font, HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER, &metrics->ascender) &&
	 _hb_ot_metrics_get_position_common (font, HB_OT_METRICS_TAG_HORIZONTAL_DESCENDER, &metrics->descender) &&
	 _hb_ot_metrics_get_position_common (font, HB_OT_METRICS_TAG_HORIZONTAL_வரி_GAP, &metrics->வரி_gap);
}

#ifndef HB_NO_VERTICAL
நிலையான hb_bool_t
hb_ot_get_font_v_extents (hb_font_t *font,
			  வெறுமை *font_data HB_UNUSED,
			  hb_font_extents_t *metrics,
			  வெறுமை *user_data HB_UNUSED)
{
  திரும்பு _hb_ot_metrics_get_position_common (font, HB_OT_METRICS_TAG_VERTICAL_ASCENDER, &metrics->ascender) &&
	 _hb_ot_metrics_get_position_common (font, HB_OT_METRICS_TAG_VERTICAL_DESCENDER, &metrics->descender) &&
	 _hb_ot_metrics_get_position_common (font, HB_OT_METRICS_TAG_VERTICAL_வரி_GAP, &metrics->வரி_gap);
}
#endif

#ifndef HB_NO_DRAW
நிலையான hb_bool_t
hb_ot_draw_glyph_or_fail (hb_font_t *font,
			  வெறுமை *font_data HB_UNUSED,
			  hb_codepoint_t glyph,
			  hb_draw_funcs_t *draw_funcs, வெறுமை *draw_data,
			  வெறுமை *user_data)
{
  மாறிலி hb_ot_font_t *ot_font = (மாறிலி hb_ot_font_t *) font_data;
  hb_draw_session_t draw_session {draw_funcs, draw_data};

  OT::hb_scalar_cache_t *gvar_cache = nullptr;
#ifndef HB_NO_VAR
  எனில் (font->இலக்கம்_coords)
  {
    ot_font->check_serial (font);
    gvar_cache = ot_font->draw.acquire_gvar_cache (*ot_font->ot_face->gvar);
  }
#endif
  HB_SCOPE_GUARD (ot_font->draw.release_gvar_cache (gvar_cache));

#ifndef HB_NO_VAR_COMPOSITES
  எனில் (font->face->table.VARC->get_path (font, glyph, draw_session)) திரும்பு உண்மை;
#endif
  // Keep the following in synch with VARC::get_path_at()
  எனில் (font->face->table.glyf->get_path (font, glyph, draw_session, gvar_cache,
				       &draw_session.get_budget ())) திரும்பு உண்மை;

#ifndef HB_NO_CFF
  எனில் (font->face->table.cff2->get_path (font, glyph, draw_session,
				       &draw_session.get_budget ())) திரும்பு உண்மை;
  எனில் (font->face->table.cff1->get_path (font, glyph, draw_session,
				       &draw_session.get_budget ())) திரும்பு உண்மை;
#endif

  திரும்பு பொய்;
}
#endif

#ifndef HB_NO_PAINT
நிலையான hb_bool_t
hb_ot_paint_glyph_or_fail (hb_font_t *font,
			   வெறுமை *font_data,
			   hb_codepoint_t glyph,
			   hb_paint_funcs_t *paint_funcs, வெறுமை *paint_data,
			   unsigned முழு palette,
			   hb_color_t foreground,
			   வெறுமை *user_data)
{
#ifndef HB_NO_COLOR
  எனில் (font->face->table.COLR->paint_glyph (font, glyph, paint_funcs, paint_data, palette, foreground)) திரும்பு உண்மை;
#ifndef HB_NO_SVG
  எனில் (font->face->table.SVG->paint_glyph (font, glyph, paint_funcs, paint_data)) திரும்பு உண்மை;
#endif
#ifndef HB_NO_OT_FONT_BITMAP
  எனில் (font->face->table.CBDT->paint_glyph (font, glyph, paint_funcs, paint_data)) திரும்பு உண்மை;
  எனில் (font->face->table.sbix->paint_glyph (font, glyph, paint_funcs, paint_data)) திரும்பு உண்மை;
#endif
#endif
  திரும்பு பொய்;
}
#endif

நிலையான உள்வரி வெறுமை free_static_ot_funcs ();

நிலையான கட்டமைப்பு hb_ot_font_funcs_lazy_loader_t : hb_font_funcs_lazy_loader_t<hb_ot_font_funcs_lazy_loader_t>
{
  நிலையான hb_font_funcs_t *create ()
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

    திரும்பு funcs;
  }
} static_ot_funcs;

நிலையான உள்வரி
வெறுமை free_static_ot_funcs ()
{
  static_ot_funcs.free_instance ();
}

நிலையான hb_font_funcs_t *
_hb_ot_get_font_funcs ()
{
  திரும்பு static_ot_funcs.get_unconst ();
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
வெறுமை
hb_ot_font_set_funcs (hb_font_t *font)
{
  hb_ot_font_t *ot_font = _hb_ot_font_create (font);
  எனில் (unlikely (!ot_font))
    திரும்பு;

  hb_font_set_funcs (font,
		     _hb_ot_get_font_funcs (),
		     ot_font,
		     _hb_ot_font_destroy);
}

#endif
