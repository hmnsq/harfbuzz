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

#अन्तर्भाव "hb.hh"

#ifndef HB_NO_OT_FONT

#अन्तर्भाव "hb-ot.h"

#अन्तर्भाव "hb-cache.hh"
#अन्तर्भाव "hb-font.hh"
#अन्तर्भाव "hb-machinery.hh"
#अन्तर्भाव "hb-ot-face.hh"

#अन्तर्भाव "hb-ot-cmap-table.hh"
#अन्तर्भाव "hb-ot-glyf-table.hh"
#अन्तर्भाव "hb-ot-var-gvar-table.hh"
#अन्तर्भाव "hb-ot-cff2-table.hh"
#अन्तर्भाव "hb-ot-cff1-table.hh"
#अन्तर्भाव "hb-ot-hmtx-table.hh"
#अन्तर्भाव "hb-ot-post-table.hh"
#अन्तर्भाव "hb-ot-stat-table.hh"
#अन्तर्भाव "hb-ot-var-varc-table.hh"
#अन्तर्भाव "hb-ot-vorg-table.hh"
#अन्तर्भाव "OT/Color/CBDT/CBDT.hh"
#अन्तर्भाव "OT/Color/COLR/COLR.hh"
#अन्तर्भाव "OT/Color/sbix/sbix.hh"
#अन्तर्भाव "OT/Color/svg/svg.hh"


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
static_assert (आकार (hb_ot_font_advance_cache_t) == 1024, "");

using hb_ot_font_origin_cache_t = hb_cache_t<20, 20>;
static_assert (आकार (hb_ot_font_origin_cache_t) == 1024, "");

संरचना hb_ot_font_t
{
  स्थिरांक hb_ot_face_t *ot_face;

  mutable hb_atomic_t<पूर्णांक> cached_serial;
  mutable hb_atomic_t<पूर्णांक> cached_coords_serial;

  संरचना direction_cache_t
  {
    mutable hb_atomic_t<hb_ot_font_advance_cache_t *> advance_cache;
    mutable hb_atomic_t<OT::hb_scalar_cache_t *> varStore_cache;

    ~direction_cache_t ()
    {
      clear ();
    }

    hb_ot_font_advance_cache_t *acquire_advance_cache () स्थिरांक
    {
    retry:
      स्वतः *cache = advance_cache.get_acquire ();
      जर (!cache)
      {
        cache = (hb_ot_font_advance_cache_t *) hb_malloc (आकार (hb_ot_font_advance_cache_t));
	जर (!cache)
	  परत nullptr;
	नवीन (cache) hb_ot_font_advance_cache_t;
	परत cache;
      }
      जर (advance_cache.cmpexch (cache, nullptr))
        परत cache;
      अन्यथा
        जा retry;
    }
    शून्य release_advance_cache (hb_ot_font_advance_cache_t *cache) स्थिरांक
    {
      जर (!cache)
        परत;
      जर (!advance_cache.cmpexch (nullptr, cache))
        hb_free (cache);
    }
    शून्य clear_advance_cache () स्थिरांक
    {
    retry:
      स्वतः *cache = advance_cache.get_acquire ();
      जर (!cache)
	परत;
      जर (advance_cache.cmpexch (cache, nullptr))
	hb_free (cache);
      अन्यथा
        जा retry;
    }

    OT::hb_scalar_cache_t *acquire_varStore_cache (स्थिरांक OT::ItemVariationStore &varStore) स्थिरांक
    {
    retry:
      स्वतः *cache = varStore_cache.get_acquire ();
      जर (!cache)
	परत varStore.create_cache ();
      जर (varStore_cache.cmpexch (cache, nullptr))
	परत cache;
      अन्यथा
	जा retry;
    }
    शून्य release_varStore_cache (OT::hb_scalar_cache_t *cache) स्थिरांक
    {
      जर (!cache)
	परत;
      जर (!varStore_cache.cmpexch (nullptr, cache))
	OT::ItemVariationStore::destroy_cache (cache);
    }
    शून्य clear_varStore_cache () स्थिरांक
    {
    retry:
      स्वतः *cache = varStore_cache.get_acquire ();
      जर (!cache)
	परत;
      जर (varStore_cache.cmpexch (cache, nullptr))
	OT::ItemVariationStore::destroy_cache (cache);
      अन्यथा
	जा retry;
    }

    शून्य clear () स्थिरांक
    {
      clear_advance_cache ();
      clear_varStore_cache ();
    }

  } h, v;

  संरचना origin_cache_t
  {
    mutable hb_atomic_t<hb_ot_font_origin_cache_t *> origin_cache;
    mutable hb_atomic_t<OT::hb_scalar_cache_t *> varStore_cache;

    ~origin_cache_t ()
    {
      clear ();
    }

    hb_ot_font_origin_cache_t *acquire_origin_cache () स्थिरांक
    {
    retry:
      स्वतः *cache = origin_cache.get_acquire ();
      जर (!cache)
      {
        cache = (hb_ot_font_origin_cache_t *) hb_malloc (आकार (hb_ot_font_origin_cache_t));
	जर (!cache)
	  परत nullptr;
	नवीन (cache) hb_ot_font_origin_cache_t;
	परत cache;
      }
      जर (origin_cache.cmpexch (cache, nullptr))
        परत cache;
      अन्यथा
        जा retry;
    }
    शून्य release_origin_cache (hb_ot_font_origin_cache_t *cache) स्थिरांक
    {
      जर (!cache)
        परत;
      जर (!origin_cache.cmpexch (nullptr, cache))
        hb_free (cache);
    }
    शून्य clear_origin_cache () स्थिरांक
    {
    retry:
      स्वतः *cache = origin_cache.get_acquire ();
      जर (!cache)
	परत;
      जर (origin_cache.cmpexch (cache, nullptr))
	hb_free (cache);
      अन्यथा
        जा retry;
    }

    OT::hb_scalar_cache_t *acquire_varStore_cache (स्थिरांक OT::ItemVariationStore &varStore) स्थिरांक
    {
    retry:
      स्वतः *cache = varStore_cache.get_acquire ();
      जर (!cache)
	परत varStore.create_cache ();
      जर (varStore_cache.cmpexch (cache, nullptr))
	परत cache;
      अन्यथा
	जा retry;
    }
    शून्य release_varStore_cache (OT::hb_scalar_cache_t *cache) स्थिरांक
    {
      जर (!cache)
	परत;
      जर (!varStore_cache.cmpexch (nullptr, cache))
	OT::ItemVariationStore::destroy_cache (cache);
    }
    शून्य clear_varStore_cache () स्थिरांक
    {
    retry:
      स्वतः *cache = varStore_cache.get_acquire ();
      जर (!cache)
	परत;
      जर (varStore_cache.cmpexch (cache, nullptr))
	OT::ItemVariationStore::destroy_cache (cache);
      अन्यथा
	जा retry;
    }

    शून्य clear () स्थिरांक
    {
      clear_origin_cache ();
      clear_varStore_cache ();
    }
  } v_origin;

  संरचना draw_cache_t
  {
    mutable hb_atomic_t<OT::hb_scalar_cache_t *> gvar_cache;

    ~draw_cache_t ()
    {
      clear ();
    }

    OT::hb_scalar_cache_t *acquire_gvar_cache (स्थिरांक OT::gvar_accelerator_t &gvar) स्थिरांक
    {
    retry:
      स्वतः *cache = gvar_cache.get_acquire ();
      जर (!cache)
	परत gvar.create_cache ();
      जर (gvar_cache.cmpexch (cache, nullptr))
	परत cache;
      अन्यथा
	जा retry;
    }
    शून्य release_gvar_cache (OT::hb_scalar_cache_t *cache) स्थिरांक
    {
      जर (!cache)
	परत;
      जर (!gvar_cache.cmpexch (nullptr, cache))
	OT::gvar_accelerator_t::destroy_cache (cache);
    }
    शून्य clear_gvar_cache () स्थिरांक
    {
    retry:
      स्वतः *cache = gvar_cache.get_acquire ();
      जर (!cache)
	परत;
      जर (gvar_cache.cmpexch (cache, nullptr))
	OT::gvar_accelerator_t::destroy_cache (cache);
      अन्यथा
	जा retry;
    }

    शून्य clear () स्थिरांक
    {
      clear_gvar_cache ();
    }
  } draw;

  शून्य check_serial (hb_font_t *font) स्थिरांक
  {
    पूर्णांक font_serial = font->serial.get_acquire ();
    जर (cached_serial.get_acquire () != font_serial)
    {
      /* These caches are dependent on scale and synthetic settings.
       * Any change to the font invalidates them. */
      v_origin.clear ();

      cached_serial.set_release (font_serial);
    }

    पूर्णांक font_serial_coords = font->serial_coords.get_acquire ();
    जर (cached_coords_serial.get_acquire () != font_serial_coords)
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

स्थिर hb_ot_font_t *
_hb_ot_font_create (hb_font_t *font)
{
  hb_ot_font_t *ot_font = (hb_ot_font_t *) hb_calloc (1, आकार (hb_ot_font_t));
  जर (unlikely (!ot_font))
    परत nullptr;

  ot_font->ot_face = &font->face->table;

  परत ot_font;
}

स्थिर शून्य
_hb_ot_font_destroy (शून्य *font_data)
{
  hb_ot_font_t *ot_font = (hb_ot_font_t *) font_data;

  ot_font->~hb_ot_font_t ();

  hb_free (ot_font);
}

स्थिर hb_bool_t
hb_ot_get_nominal_glyph (hb_font_t *font HB_UNUSED,
			 शून्य *font_data,
			 hb_codepoint_t unicode,
			 hb_codepoint_t *glyph,
			 शून्य *user_data HB_UNUSED)
{
  स्थिरांक hb_ot_font_t *ot_font = (स्थिरांक hb_ot_font_t *) font_data;
  स्थिरांक hb_ot_face_t *ot_face = ot_font->ot_face;
  परत ot_face->cmap->get_nominal_glyph (unicode, glyph);
}

स्थिर अचिन्हित पूर्णांक
hb_ot_get_nominal_glyphs (hb_font_t *font HB_UNUSED,
			  शून्य *font_data,
			  अचिन्हित पूर्णांक मोजणी,
			  स्थिरांक hb_codepoint_t *first_unicode,
			  अचिन्हित पूर्णांक unicode_stride,
			  hb_codepoint_t *first_glyph,
			  अचिन्हित पूर्णांक glyph_stride,
			  शून्य *user_data HB_UNUSED)
{
  स्थिरांक hb_ot_font_t *ot_font = (स्थिरांक hb_ot_font_t *) font_data;
  स्थिरांक hb_ot_face_t *ot_face = ot_font->ot_face;
  परत ot_face->cmap->get_nominal_glyphs (मोजणी,
					    first_unicode, unicode_stride,
					    first_glyph, glyph_stride);
}

स्थिर hb_bool_t
hb_ot_get_variation_glyph (hb_font_t *font HB_UNUSED,
			   शून्य *font_data,
			   hb_codepoint_t unicode,
			   hb_codepoint_t variation_selector,
			   hb_codepoint_t *glyph,
			   शून्य *user_data HB_UNUSED)
{
  स्थिरांक hb_ot_font_t *ot_font = (स्थिरांक hb_ot_font_t *) font_data;
  स्थिरांक hb_ot_face_t *ot_face = ot_font->ot_face;
  परत ot_face->cmap->get_variation_glyph (unicode,
                                             variation_selector, glyph);
}

स्थिर शून्य
hb_ot_get_glyph_h_advances (hb_font_t* font, शून्य* font_data,
			    अचिन्हित मोजणी,
			    स्थिरांक hb_codepoint_t *first_glyph,
			    अचिन्हित glyph_stride,
			    hb_position_t *first_advance,
			    अचिन्हित advance_stride,
			    शून्य *user_data HB_UNUSED)
{
  // Duplicated in v_advances. Ugly. Keep in sync'ish.

  स्थिरांक hb_ot_font_t *ot_font = (स्थिरांक hb_ot_font_t *) font_data;
  स्थिरांक hb_ot_face_t *ot_face = ot_font->ot_face;
  स्थिरांक OT::hmtx_accelerator_t &hmtx = *ot_face->hmtx;

  जर (unlikely (!hmtx.has_data ()))
  {
    hb_position_t advance = font->face->get_upem () / 2;
    advance = font->em_scale_x (advance);
    साठी (अचिन्हित पूर्णांक i = 0; i < मोजणी; i++)
    {
      *first_advance = advance;
      first_advance = &StructAtOffsetUnaligned<hb_position_t> (first_advance, advance_stride);
    }
    परत;
  }

#ifndef HB_NO_VAR
  जर (!font->has_nonzero_coords)
  {
  fallback:
#अन्यथा
  {
#endif
    // Just plain htmx data. No need to cache.
    साठी (अचिन्हित पूर्णांक i = 0; i < मोजणी; i++)
    {
      *first_advance = font->em_scale_x (hmtx.get_advance_without_var_unscaled (*first_glyph));
      first_glyph = &StructAtOffsetUnaligned<hb_codepoint_t> (first_glyph, glyph_stride);
      first_advance = &StructAtOffsetUnaligned<hb_position_t> (first_advance, advance_stride);
    }
    परत;
  }

#ifndef HB_NO_VAR
  /* has_nonzero_coords. */

  ot_font->check_serial (font);
  hb_ot_font_advance_cache_t *advance_cache = ot_font->h.acquire_advance_cache ();
  जर (!advance_cache)
  {
    // malloc failure. Just use the fallback non-variable path.
    जा fallback;
  }

  /* If HVAR is present, use it.*/
  स्थिरांक OT::HVAR &HVAR = *hmtx.var_table;
  जर (HVAR.has_data ())
  {
    स्थिरांक OT::ItemVariationStore &varStore = &HVAR + HVAR.varStore;
    OT::hb_scalar_cache_t *varStore_cache = ot_font->h.acquire_varStore_cache (varStore);

    साठी (अचिन्हित पूर्णांक i = 0; i < मोजणी; i++)
    {
      hb_position_t v;
      अचिन्हित cv;
      जर (advance_cache->get (*first_glyph, &cv))
	v = cv;
      अन्यथा
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
    परत;
  }

  स्थिरांक स्वतः &gvar = *ot_face->gvar;
  जर (gvar.has_data ())
  {
    स्थिरांक स्वतः &glyf = *ot_face->glyf;
    स्वतः *scratch = glyf.acquire_scratch ();
    जर (unlikely (!scratch))
    {
      ot_font->h.release_advance_cache (advance_cache);
      जा fallback;
    }
    OT::hb_scalar_cache_t *gvar_cache = ot_font->draw.acquire_gvar_cache (gvar);

    साठी (अचिन्हित पूर्णांक i = 0; i < मोजणी; i++)
    {
      hb_position_t v;
      अचिन्हित cv;
      जर (advance_cache->get (*first_glyph, &cv))
	v = cv;
      अन्यथा
      {
        v = glyf.get_advance_with_var_unscaled (*first_glyph, font, असत्य, *scratch, gvar_cache);
	advance_cache->set (*first_glyph, v);
      }
      *first_advance = font->em_scale_x (v);
      first_glyph = &StructAtOffsetUnaligned<hb_codepoint_t> (first_glyph, glyph_stride);
      first_advance = &StructAtOffsetUnaligned<hb_position_t> (first_advance, advance_stride);
    }

    ot_font->draw.release_gvar_cache (gvar_cache);
    glyf.release_scratch (scratch);
    ot_font->h.release_advance_cache (advance_cache);
    परत;
  }

  ot_font->h.release_advance_cache (advance_cache);
  // No HVAR or GVAR.  Just use the fallback non-variable path.
  जा fallback;
#endif
}

#ifndef HB_NO_VERTICAL
स्थिर शून्य
hb_ot_get_glyph_v_advances (hb_font_t* font, शून्य* font_data,
			    अचिन्हित मोजणी,
			    स्थिरांक hb_codepoint_t *first_glyph,
			    अचिन्हित glyph_stride,
			    hb_position_t *first_advance,
			    अचिन्हित advance_stride,
			    शून्य *user_data HB_UNUSED)
{
  // Duplicated from h_advances. Ugly. Keep in sync'ish.

  स्थिरांक hb_ot_font_t *ot_font = (स्थिरांक hb_ot_font_t *) font_data;
  स्थिरांक hb_ot_face_t *ot_face = ot_font->ot_face;
  स्थिरांक OT::vmtx_accelerator_t &vmtx = *ot_face->vmtx;

  जर (unlikely (!vmtx.has_data ()))
  {
    hb_font_extents_t font_extents;
    font->get_h_extents_with_fallback (&font_extents);
    hb_position_t advance = font_extents.descender - font_extents.ascender;
    साठी (अचिन्हित पूर्णांक i = 0; i < मोजणी; i++)
    {
      *first_advance = advance;
      first_advance = &StructAtOffsetUnaligned<hb_position_t> (first_advance, advance_stride);
    }
    परत;
  }

#ifndef HB_NO_VAR
  जर (!font->has_nonzero_coords)
  {
  fallback:
#अन्यथा
  {
#endif
    // Just plain vtmx data. No need to cache.
    साठी (अचिन्हित पूर्णांक i = 0; i < मोजणी; i++)
    {
      *first_advance = font->em_scale_y (- (पूर्णांक) vmtx.get_advance_without_var_unscaled (*first_glyph));
      first_glyph = &StructAtOffsetUnaligned<hb_codepoint_t> (first_glyph, glyph_stride);
      first_advance = &StructAtOffsetUnaligned<hb_position_t> (first_advance, advance_stride);
    }
    परत;
  }

#ifndef HB_NO_VAR
  /* has_nonzero_coords. */

  ot_font->check_serial (font);
  hb_ot_font_advance_cache_t *advance_cache = ot_font->v.acquire_advance_cache ();
  जर (!advance_cache)
  {
    // malloc failure. Just use the fallback non-variable path.
    जा fallback;
  }

  /* If VVAR is present, use it.*/
  स्थिरांक OT::VVAR &VVAR = *vmtx.var_table;
  जर (VVAR.has_data ())
  {
    स्थिरांक OT::ItemVariationStore &varStore = &VVAR + VVAR.varStore;
    OT::hb_scalar_cache_t *varStore_cache = ot_font->v.acquire_varStore_cache (varStore);

    साठी (अचिन्हित पूर्णांक i = 0; i < मोजणी; i++)
    {
      hb_position_t v;
      अचिन्हित cv;
      जर (advance_cache->get (*first_glyph, &cv))
	v = cv;
      अन्यथा
      {
        v = vmtx.get_advance_with_var_unscaled (*first_glyph, font, varStore_cache);
	advance_cache->set (*first_glyph, v);
      }
      *first_advance = font->em_scale_y (- (पूर्णांक) v);
      first_glyph = &StructAtOffsetUnaligned<hb_codepoint_t> (first_glyph, glyph_stride);
      first_advance = &StructAtOffsetUnaligned<hb_position_t> (first_advance, advance_stride);
    }

    ot_font->v.release_varStore_cache (varStore_cache);
    ot_font->v.release_advance_cache (advance_cache);
    परत;
  }

  स्थिरांक स्वतः &gvar = *ot_face->gvar;
  जर (gvar.has_data ())
  {
    स्थिरांक स्वतः &glyf = *ot_face->glyf;
    स्वतः *scratch = glyf.acquire_scratch ();
    जर (unlikely (!scratch))
    {
      ot_font->v.release_advance_cache (advance_cache);
      जा fallback;
    }
    OT::hb_scalar_cache_t *gvar_cache = ot_font->draw.acquire_gvar_cache (gvar);

    साठी (अचिन्हित पूर्णांक i = 0; i < मोजणी; i++)
    {
      hb_position_t v;
      अचिन्हित cv;
      जर (advance_cache->get (*first_glyph, &cv))
	v = cv;
      अन्यथा
      {
        v = glyf.get_advance_with_var_unscaled (*first_glyph, font, सत्य, *scratch, gvar_cache);
	advance_cache->set (*first_glyph, v);
      }
      *first_advance = font->em_scale_y (- (पूर्णांक) v);
      first_glyph = &StructAtOffsetUnaligned<hb_codepoint_t> (first_glyph, glyph_stride);
      first_advance = &StructAtOffsetUnaligned<hb_position_t> (first_advance, advance_stride);
    }

    ot_font->draw.release_gvar_cache (gvar_cache);
    glyf.release_scratch (scratch);
    ot_font->v.release_advance_cache (advance_cache);
    परत;
  }

  ot_font->v.release_advance_cache (advance_cache);
  // No VVAR or GVAR.  Just use the fallback non-variable path.
  जा fallback;
#endif
}
#endif

#ifndef HB_NO_VERTICAL
HB_HOT
स्थिर hb_bool_t
hb_ot_get_glyph_v_origins (hb_font_t *font,
			   शून्य *font_data,
			   अचिन्हित पूर्णांक मोजणी,
			   स्थिरांक hb_codepoint_t *first_glyph,
			   अचिन्हित glyph_stride,
			   hb_position_t *first_x,
			   अचिन्हित x_stride,
			   hb_position_t *first_y,
			   अचिन्हित y_stride,
			   शून्य *user_data HB_UNUSED)
{
  स्थिरांक hb_ot_font_t *ot_font = (स्थिरांक hb_ot_font_t *) font_data;
  स्थिरांक hb_ot_face_t *ot_face = ot_font->ot_face;

  ot_font->check_serial (font);

  /* First, set all the x values to half the advance width. */
  font->get_glyph_h_advances (मोजणी,
			      first_glyph, glyph_stride,
			      first_x, x_stride);
  साठी (अचिन्हित i = 0; i < मोजणी; i++)
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
  स्थिरांक OT::VORG &VORG = *ot_face->VORG;
  जर (origin_cache && VORG.has_data ())
  {
#ifndef HB_NO_VAR
    जर (!font->has_nonzero_coords)
#endif
    {
      साठी (अचिन्हित i = 0; i < मोजणी; i++)
      {
	hb_position_t origin;
	अचिन्हित cv;
	जर (origin_cache->get (*first_glyph, &cv))
	  origin = font->y_scale < 0 ? -static_cast<hb_position_t>(cv) : static_cast<hb_position_t>(cv);
	अन्यथा
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
    अन्यथा
    {
      स्थिरांक OT::VVAR &VVAR = *ot_face->vmtx->var_table;
      स्थिरांक स्वतः &varStore = &VVAR + VVAR.varStore;
      स्वतः *varStore_cache = ot_font->v_origin.acquire_varStore_cache (varStore);
      साठी (अचिन्हित i = 0; i < मोजणी; i++)
      {
	hb_position_t origin;
	अचिन्हित cv;
	जर (origin_cache->get (*first_glyph, &cv))
	  origin = font->y_scale < 0 ? -static_cast<hb_position_t>(cv) : static_cast<hb_position_t>(cv);
	अन्यथा
	{
	  origin = font->em_scalef_y (VORG.get_y_origin (*first_glyph) +
				      VVAR.get_vorg_delta_unscaled (*first_glyph,
								    font->coords, font->अंक_coords,
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
    परत सत्य;
  }

  /* If and only if `vmtx` is present and it's a `glyf` font,
   * we use the top phantom point, deduced from vmtx,glyf[,gvar]. */
  स्थिरांक स्वतः &vmtx = *ot_face->vmtx;
  स्थिरांक स्वतः &glyf = *ot_face->glyf;
  जर (origin_cache && vmtx.has_data() && glyf.has_data ())
  {
    स्वतः *scratch = glyf.acquire_scratch ();
    जर (unlikely (!scratch))
    {
      ot_font->v_origin.release_origin_cache (origin_cache);
      परत असत्य;
    }
    OT::hb_scalar_cache_t *gvar_cache = nullptr;
#ifndef HB_NO_VAR
    gvar_cache = font->has_nonzero_coords ?
					ot_font->draw.acquire_gvar_cache (*ot_face->gvar) :
					nullptr;
#endif

    साठी (अचिन्हित i = 0; i < मोजणी; i++)
    {
      hb_position_t origin;
      अचिन्हित cv;
      जर (origin_cache->get (*first_glyph, &cv))
	origin = font->y_scale < 0 ? -static_cast<hb_position_t>(cv) : static_cast<hb_position_t>(cv);
      अन्यथा
      {
	origin = font->em_scalef_y (glyf.get_v_origin_with_var_unscaled (*first_glyph, font, *scratch, gvar_cache));
	origin_cache->set (*first_glyph, font->y_scale < 0 ? -origin : origin);
      }

      *first_y = origin;

      first_glyph = &StructAtOffsetUnaligned<hb_codepoint_t> (first_glyph, glyph_stride);
      first_y = &StructAtOffsetUnaligned<hb_position_t> (first_y, y_stride);
    }

#ifndef HB_NO_VAR
    जर (gvar_cache)
      ot_font->draw.release_gvar_cache (gvar_cache);
#endif
    glyf.release_scratch (scratch);
    ot_font->v_origin.release_origin_cache (origin_cache);
    परत सत्य;
  }

  /* Otherwise, use glyph extents to center the glyph vertically.
   * If getting glyph extents failed, just use the font ascender. */
  जर (origin_cache && font->has_glyph_extents_func ())
  {
    hb_font_extents_t font_extents;
    font->get_h_extents_with_fallback (&font_extents);
    hb_position_t font_advance = font_extents.ascender - font_extents.descender;

    साठी (अचिन्हित i = 0; i < मोजणी; i++)
    {
      hb_position_t origin;
      अचिन्हित cv;

      जर (origin_cache->get (*first_glyph, &cv))
	origin = font->y_scale < 0 ? -static_cast<hb_position_t>(cv) : static_cast<hb_position_t>(cv);
      अन्यथा
      {
	hb_glyph_extents_t extents = {0};
	जर (likely (font->get_glyph_extents (*first_glyph, &extents)))
	  origin = hb_clamp_to<hb_position_t> ((int64_t) extents.y_bearing +
					 (((int64_t) font_advance + extents.height) >> 1));
	अन्यथा
	  origin = font_extents.ascender;

	origin_cache->set (*first_glyph, font->y_scale < 0 ? -origin : origin);
      }

      *first_y = origin;

      first_glyph = &StructAtOffsetUnaligned<hb_codepoint_t> (first_glyph, glyph_stride);
      first_y = &StructAtOffsetUnaligned<hb_position_t> (first_y, y_stride);
    }
  }

  ot_font->v_origin.release_origin_cache (origin_cache);
  परत सत्य;
}
#endif

स्थिर hb_bool_t
hb_ot_get_glyph_extents (hb_font_t *font,
			 शून्य *font_data,
			 hb_codepoint_t glyph,
			 hb_glyph_extents_t *extents,
			 शून्य *user_data HB_UNUSED)
{
  स्थिरांक hb_ot_font_t *ot_font = (स्थिरांक hb_ot_font_t *) font_data;
  स्थिरांक hb_ot_face_t *ot_face = ot_font->ot_face;

#जर !defined(HB_NO_OT_FONT_BITMAP) && !defined(HB_NO_COLOR)
  जर (ot_face->sbix->get_extents (font, glyph, extents)) परत सत्य;
  जर (ot_face->CBDT->get_extents (font, glyph, extents)) परत सत्य;
#endif
#जर !defined(HB_NO_COLOR) && !defined(HB_NO_PAINT)
  जर (ot_face->COLR->get_extents (font, glyph, extents)) परत सत्य;
#endif
#ifndef HB_NO_VAR_COMPOSITES
  जर (ot_face->VARC->get_extents (font, glyph, extents)) परत सत्य;
#endif
  जर (ot_face->glyf->get_extents (font, glyph, extents)) परत सत्य;
#ifndef HB_NO_OT_FONT_CFF
  जर (ot_face->cff2->get_extents (font, glyph, extents)) परत सत्य;
  जर (ot_face->cff1->get_extents (font, glyph, extents)) परत सत्य;
#endif

  परत असत्य;
}

#ifndef HB_NO_OT_FONT_GLYPH_NAMES
स्थिर hb_bool_t
hb_ot_get_glyph_name (hb_font_t *font HB_UNUSED,
		      शून्य *font_data,
		      hb_codepoint_t glyph,
		      अक्षर *name, अचिन्हित पूर्णांक size,
		      शून्य *user_data HB_UNUSED)
{
  स्थिरांक hb_ot_font_t *ot_font = (स्थिरांक hb_ot_font_t *) font_data;
  स्थिरांक hb_ot_face_t *ot_face = ot_font->ot_face;

  जर (ot_face->post->get_glyph_name (glyph, name, size)) परत सत्य;
#ifndef HB_NO_OT_FONT_CFF
  जर (ot_face->cff1->get_glyph_name (glyph, name, size)) परत सत्य;
#endif
  परत असत्य;
}
स्थिर hb_bool_t
hb_ot_get_glyph_from_name (hb_font_t *font HB_UNUSED,
			   शून्य *font_data,
			   स्थिरांक अक्षर *name, पूर्णांक len,
			   hb_codepoint_t *glyph,
			   शून्य *user_data HB_UNUSED)
{
  स्थिरांक hb_ot_font_t *ot_font = (स्थिरांक hb_ot_font_t *) font_data;
  स्थिरांक hb_ot_face_t *ot_face = ot_font->ot_face;

  जर (ot_face->post->get_glyph_from_name (name, len, glyph)) परत सत्य;
#ifndef HB_NO_OT_FONT_CFF
    जर (ot_face->cff1->get_glyph_from_name (name, len, glyph)) परत सत्य;
#endif
  परत असत्य;
}
#endif

स्थिर hb_bool_t
hb_ot_get_font_h_extents (hb_font_t *font,
			  शून्य *font_data HB_UNUSED,
			  hb_font_extents_t *metrics,
			  शून्य *user_data HB_UNUSED)
{
  परत _hb_ot_metrics_get_position_common (font, HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER, &metrics->ascender) &&
	 _hb_ot_metrics_get_position_common (font, HB_OT_METRICS_TAG_HORIZONTAL_DESCENDER, &metrics->descender) &&
	 _hb_ot_metrics_get_position_common (font, HB_OT_METRICS_TAG_HORIZONTAL_ओळ_GAP, &metrics->ओळ_gap);
}

#ifndef HB_NO_VERTICAL
स्थिर hb_bool_t
hb_ot_get_font_v_extents (hb_font_t *font,
			  शून्य *font_data HB_UNUSED,
			  hb_font_extents_t *metrics,
			  शून्य *user_data HB_UNUSED)
{
  परत _hb_ot_metrics_get_position_common (font, HB_OT_METRICS_TAG_VERTICAL_ASCENDER, &metrics->ascender) &&
	 _hb_ot_metrics_get_position_common (font, HB_OT_METRICS_TAG_VERTICAL_DESCENDER, &metrics->descender) &&
	 _hb_ot_metrics_get_position_common (font, HB_OT_METRICS_TAG_VERTICAL_ओळ_GAP, &metrics->ओळ_gap);
}
#endif

#ifndef HB_NO_DRAW
स्थिर hb_bool_t
hb_ot_draw_glyph_or_fail (hb_font_t *font,
			  शून्य *font_data HB_UNUSED,
			  hb_codepoint_t glyph,
			  hb_draw_funcs_t *draw_funcs, शून्य *draw_data,
			  शून्य *user_data)
{
  स्थिरांक hb_ot_font_t *ot_font = (स्थिरांक hb_ot_font_t *) font_data;
  hb_draw_session_t draw_session {draw_funcs, draw_data};

  OT::hb_scalar_cache_t *gvar_cache = nullptr;
#ifndef HB_NO_VAR
  जर (font->अंक_coords)
  {
    ot_font->check_serial (font);
    gvar_cache = ot_font->draw.acquire_gvar_cache (*ot_font->ot_face->gvar);
  }
#endif
  HB_SCOPE_GUARD (ot_font->draw.release_gvar_cache (gvar_cache));

#ifndef HB_NO_VAR_COMPOSITES
  जर (font->face->table.VARC->get_path (font, glyph, draw_session)) परत सत्य;
#endif
  // Keep the following in synch with VARC::get_path_at()
  जर (font->face->table.glyf->get_path (font, glyph, draw_session, gvar_cache,
				       &draw_session.get_budget ())) परत सत्य;

#ifndef HB_NO_CFF
  जर (font->face->table.cff2->get_path (font, glyph, draw_session,
				       &draw_session.get_budget ())) परत सत्य;
  जर (font->face->table.cff1->get_path (font, glyph, draw_session,
				       &draw_session.get_budget ())) परत सत्य;
#endif

  परत असत्य;
}
#endif

#ifndef HB_NO_PAINT
स्थिर hb_bool_t
hb_ot_paint_glyph_or_fail (hb_font_t *font,
			   शून्य *font_data,
			   hb_codepoint_t glyph,
			   hb_paint_funcs_t *paint_funcs, शून्य *paint_data,
			   अचिन्हित पूर्णांक palette,
			   hb_color_t foreground,
			   शून्य *user_data)
{
#ifndef HB_NO_COLOR
  जर (font->face->table.COLR->paint_glyph (font, glyph, paint_funcs, paint_data, palette, foreground)) परत सत्य;
#ifndef HB_NO_SVG
  जर (font->face->table.SVG->paint_glyph (font, glyph, paint_funcs, paint_data)) परत सत्य;
#endif
#ifndef HB_NO_OT_FONT_BITMAP
  जर (font->face->table.CBDT->paint_glyph (font, glyph, paint_funcs, paint_data)) परत सत्य;
  जर (font->face->table.sbix->paint_glyph (font, glyph, paint_funcs, paint_data)) परत सत्य;
#endif
#endif
  परत असत्य;
}
#endif

स्थिर ओळीत शून्य free_static_ot_funcs ();

स्थिर संरचना hb_ot_font_funcs_lazy_loader_t : hb_font_funcs_lazy_loader_t<hb_ot_font_funcs_lazy_loader_t>
{
  स्थिर hb_font_funcs_t *create ()
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

    परत funcs;
  }
} static_ot_funcs;

स्थिर ओळीत
शून्य free_static_ot_funcs ()
{
  static_ot_funcs.free_instance ();
}

स्थिर hb_font_funcs_t *
_hb_ot_get_font_funcs ()
{
  परत static_ot_funcs.get_unconst ();
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
शून्य
hb_ot_font_set_funcs (hb_font_t *font)
{
  hb_ot_font_t *ot_font = _hb_ot_font_create (font);
  जर (unlikely (!ot_font))
    परत;

  hb_font_set_funcs (font,
		     _hb_ot_get_font_funcs (),
		     ot_font,
		     _hb_ot_font_destroy);
}

#endif
