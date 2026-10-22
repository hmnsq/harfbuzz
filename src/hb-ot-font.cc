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

#समावेश "hb.hh"

#अगर_अघोषित HB_NO_OT_FONT

#समावेश "hb-ot.h"

#समावेश "hb-cache.hh"
#समावेश "hb-font.hh"
#समावेश "hb-machinery.hh"
#समावेश "hb-ot-face.hh"

#समावेश "hb-ot-cmap-table.hh"
#समावेश "hb-ot-glyf-table.hh"
#समावेश "hb-ot-var-gvar-table.hh"
#समावेश "hb-ot-cff2-table.hh"
#समावेश "hb-ot-cff1-table.hh"
#समावेश "hb-ot-hmtx-table.hh"
#समावेश "hb-ot-post-table.hh"
#समावेश "hb-ot-stat-table.hh"
#समावेश "hb-ot-var-varc-table.hh"
#समावेश "hb-ot-vorg-table.hh"
#समावेश "OT/Color/CBDT/CBDT.hh"
#समावेश "OT/Color/COLR/COLR.hh"
#समावेश "OT/Color/sbix/sbix.hh"
#समावेश "OT/Color/svg/svg.hh"


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
static_assert (माप (hb_ot_font_advance_cache_t) == 1024, "");

using hb_ot_font_origin_cache_t = hb_cache_t<20, 20>;
static_assert (माप (hb_ot_font_origin_cache_t) == 1024, "");

काष्ठा hb_ot_font_t
{
  स्थिर hb_ot_face_t *ot_face;

  mutable hb_atomic_t<पूर्णांक> cached_serial;
  mutable hb_atomic_t<पूर्णांक> cached_coords_serial;

  काष्ठा direction_cache_t
  {
    mutable hb_atomic_t<hb_ot_font_advance_cache_t *> advance_cache;
    mutable hb_atomic_t<OT::hb_scalar_cache_t *> varStore_cache;

    ~direction_cache_t ()
    {
      सफा ();
    }

    hb_ot_font_advance_cache_t *acquire_advance_cache () स्थिर
    {
    retry:
      स्वतः *cache = advance_cache.get_acquire ();
      अगर (!cache)
      {
        cache = (hb_ot_font_advance_cache_t *) hb_malloc (माप (hb_ot_font_advance_cache_t));
	अगर (!cache)
	  वापस nullptr;
	नया (cache) hb_ot_font_advance_cache_t;
	वापस cache;
      }
      अगर (advance_cache.cmpexch (cache, nullptr))
        वापस cache;
      अन्यथा
        जाओ retry;
    }
    व्योम release_advance_cache (hb_ot_font_advance_cache_t *cache) स्थिर
    {
      अगर (!cache)
        वापस;
      अगर (!advance_cache.cmpexch (nullptr, cache))
        hb_free (cache);
    }
    व्योम clear_advance_cache () स्थिर
    {
    retry:
      स्वतः *cache = advance_cache.get_acquire ();
      अगर (!cache)
	वापस;
      अगर (advance_cache.cmpexch (cache, nullptr))
	hb_free (cache);
      अन्यथा
        जाओ retry;
    }

    OT::hb_scalar_cache_t *acquire_varStore_cache (स्थिर OT::ItemVariationStore &varStore) स्थिर
    {
    retry:
      स्वतः *cache = varStore_cache.get_acquire ();
      अगर (!cache)
	वापस varStore.create_cache ();
      अगर (varStore_cache.cmpexch (cache, nullptr))
	वापस cache;
      अन्यथा
	जाओ retry;
    }
    व्योम release_varStore_cache (OT::hb_scalar_cache_t *cache) स्थिर
    {
      अगर (!cache)
	वापस;
      अगर (!varStore_cache.cmpexch (nullptr, cache))
	OT::ItemVariationStore::destroy_cache (cache);
    }
    व्योम clear_varStore_cache () स्थिर
    {
    retry:
      स्वतः *cache = varStore_cache.get_acquire ();
      अगर (!cache)
	वापस;
      अगर (varStore_cache.cmpexch (cache, nullptr))
	OT::ItemVariationStore::destroy_cache (cache);
      अन्यथा
	जाओ retry;
    }

    व्योम सफा () स्थिर
    {
      clear_advance_cache ();
      clear_varStore_cache ();
    }

  } h, v;

  काष्ठा origin_cache_t
  {
    mutable hb_atomic_t<hb_ot_font_origin_cache_t *> origin_cache;
    mutable hb_atomic_t<OT::hb_scalar_cache_t *> varStore_cache;

    ~origin_cache_t ()
    {
      सफा ();
    }

    hb_ot_font_origin_cache_t *acquire_origin_cache () स्थिर
    {
    retry:
      स्वतः *cache = origin_cache.get_acquire ();
      अगर (!cache)
      {
        cache = (hb_ot_font_origin_cache_t *) hb_malloc (माप (hb_ot_font_origin_cache_t));
	अगर (!cache)
	  वापस nullptr;
	नया (cache) hb_ot_font_origin_cache_t;
	वापस cache;
      }
      अगर (origin_cache.cmpexch (cache, nullptr))
        वापस cache;
      अन्यथा
        जाओ retry;
    }
    व्योम release_origin_cache (hb_ot_font_origin_cache_t *cache) स्थिर
    {
      अगर (!cache)
        वापस;
      अगर (!origin_cache.cmpexch (nullptr, cache))
        hb_free (cache);
    }
    व्योम clear_origin_cache () स्थिर
    {
    retry:
      स्वतः *cache = origin_cache.get_acquire ();
      अगर (!cache)
	वापस;
      अगर (origin_cache.cmpexch (cache, nullptr))
	hb_free (cache);
      अन्यथा
        जाओ retry;
    }

    OT::hb_scalar_cache_t *acquire_varStore_cache (स्थिर OT::ItemVariationStore &varStore) स्थिर
    {
    retry:
      स्वतः *cache = varStore_cache.get_acquire ();
      अगर (!cache)
	वापस varStore.create_cache ();
      अगर (varStore_cache.cmpexch (cache, nullptr))
	वापस cache;
      अन्यथा
	जाओ retry;
    }
    व्योम release_varStore_cache (OT::hb_scalar_cache_t *cache) स्थिर
    {
      अगर (!cache)
	वापस;
      अगर (!varStore_cache.cmpexch (nullptr, cache))
	OT::ItemVariationStore::destroy_cache (cache);
    }
    व्योम clear_varStore_cache () स्थिर
    {
    retry:
      स्वतः *cache = varStore_cache.get_acquire ();
      अगर (!cache)
	वापस;
      अगर (varStore_cache.cmpexch (cache, nullptr))
	OT::ItemVariationStore::destroy_cache (cache);
      अन्यथा
	जाओ retry;
    }

    व्योम सफा () स्थिर
    {
      clear_origin_cache ();
      clear_varStore_cache ();
    }
  } v_origin;

  काष्ठा draw_cache_t
  {
    mutable hb_atomic_t<OT::hb_scalar_cache_t *> gvar_cache;

    ~draw_cache_t ()
    {
      सफा ();
    }

    OT::hb_scalar_cache_t *acquire_gvar_cache (स्थिर OT::gvar_accelerator_t &gvar) स्थिर
    {
    retry:
      स्वतः *cache = gvar_cache.get_acquire ();
      अगर (!cache)
	वापस gvar.create_cache ();
      अगर (gvar_cache.cmpexch (cache, nullptr))
	वापस cache;
      अन्यथा
	जाओ retry;
    }
    व्योम release_gvar_cache (OT::hb_scalar_cache_t *cache) स्थिर
    {
      अगर (!cache)
	वापस;
      अगर (!gvar_cache.cmpexch (nullptr, cache))
	OT::gvar_accelerator_t::destroy_cache (cache);
    }
    व्योम clear_gvar_cache () स्थिर
    {
    retry:
      स्वतः *cache = gvar_cache.get_acquire ();
      अगर (!cache)
	वापस;
      अगर (gvar_cache.cmpexch (cache, nullptr))
	OT::gvar_accelerator_t::destroy_cache (cache);
      अन्यथा
	जाओ retry;
    }

    व्योम सफा () स्थिर
    {
      clear_gvar_cache ();
    }
  } draw;

  व्योम check_serial (hb_font_t *font) स्थिर
  {
    पूर्णांक font_serial = font->serial.get_acquire ();
    अगर (cached_serial.get_acquire () != font_serial)
    {
      /* These caches are dependent on scale and synthetic settings.
       * Any change to the font invalidates them. */
      v_origin.सफा ();

      cached_serial.set_release (font_serial);
    }

    पूर्णांक font_serial_coords = font->serial_coords.get_acquire ();
    अगर (cached_coords_serial.get_acquire () != font_serial_coords)
    {
      /* These caches are independent of scale or synthetic settings.
       * Just variation changes will invalidate them. */
      h.सफा ();
      v.सफा ();
      draw.सफा ();

      cached_coords_serial.set_release (font_serial_coords);
    }
  }
};

अटल hb_ot_font_t *
_hb_ot_font_create (hb_font_t *font)
{
  hb_ot_font_t *ot_font = (hb_ot_font_t *) hb_calloc (1, माप (hb_ot_font_t));
  अगर (unlikely (!ot_font))
    वापस nullptr;

  ot_font->ot_face = &font->face->table;

  वापस ot_font;
}

अटल व्योम
_hb_ot_font_destroy (व्योम *font_तथ्य)
{
  hb_ot_font_t *ot_font = (hb_ot_font_t *) font_तथ्य;

  ot_font->~hb_ot_font_t ();

  hb_free (ot_font);
}

अटल hb_bool_t
hb_ot_get_nominal_glyph (hb_font_t *font HB_UNUSED,
			 व्योम *font_तथ्य,
			 hb_codepoint_t unicode,
			 hb_codepoint_t *glyph,
			 व्योम *user_तथ्य HB_UNUSED)
{
  स्थिर hb_ot_font_t *ot_font = (स्थिर hb_ot_font_t *) font_तथ्य;
  स्थिर hb_ot_face_t *ot_face = ot_font->ot_face;
  वापस ot_face->cmap->get_nominal_glyph (unicode, glyph);
}

अटल अचिन्हित पूर्णांक
hb_ot_get_nominal_glyphs (hb_font_t *font HB_UNUSED,
			  व्योम *font_तथ्य,
			  अचिन्हित पूर्णांक गिनती,
			  स्थिर hb_codepoint_t *first_unicode,
			  अचिन्हित पूर्णांक unicode_stride,
			  hb_codepoint_t *first_glyph,
			  अचिन्हित पूर्णांक glyph_stride,
			  व्योम *user_तथ्य HB_UNUSED)
{
  स्थिर hb_ot_font_t *ot_font = (स्थिर hb_ot_font_t *) font_तथ्य;
  स्थिर hb_ot_face_t *ot_face = ot_font->ot_face;
  वापस ot_face->cmap->get_nominal_glyphs (गिनती,
					    first_unicode, unicode_stride,
					    first_glyph, glyph_stride);
}

अटल hb_bool_t
hb_ot_get_variation_glyph (hb_font_t *font HB_UNUSED,
			   व्योम *font_तथ्य,
			   hb_codepoint_t unicode,
			   hb_codepoint_t variation_selector,
			   hb_codepoint_t *glyph,
			   व्योम *user_तथ्य HB_UNUSED)
{
  स्थिर hb_ot_font_t *ot_font = (स्थिर hb_ot_font_t *) font_तथ्य;
  स्थिर hb_ot_face_t *ot_face = ot_font->ot_face;
  वापस ot_face->cmap->get_variation_glyph (unicode,
                                             variation_selector, glyph);
}

अटल व्योम
hb_ot_get_glyph_h_advances (hb_font_t* font, व्योम* font_तथ्य,
			    अचिन्हित गिनती,
			    स्थिर hb_codepoint_t *first_glyph,
			    अचिन्हित glyph_stride,
			    hb_position_t *first_advance,
			    अचिन्हित advance_stride,
			    व्योम *user_तथ्य HB_UNUSED)
{
  // Duplicated in v_advances. Ugly. Keep in sync'ish.

  स्थिर hb_ot_font_t *ot_font = (स्थिर hb_ot_font_t *) font_तथ्य;
  स्थिर hb_ot_face_t *ot_face = ot_font->ot_face;
  स्थिर OT::hmtx_accelerator_t &hmtx = *ot_face->hmtx;

  अगर (unlikely (!hmtx.has_तथ्य ()))
  {
    hb_position_t advance = font->face->get_upem () / 2;
    advance = font->em_scale_x (advance);
    क्रम (अचिन्हित पूर्णांक i = 0; i < गिनती; i++)
    {
      *first_advance = advance;
      first_advance = &StructAtOffsetUnaligned<hb_position_t> (first_advance, advance_stride);
    }
    वापस;
  }

#अगर_अघोषित HB_NO_VAR
  अगर (!font->has_nonzero_coords)
  {
  fallback:
#अन्यथा
  {
#पूर्ण_अगर
    // Just plain htmx data. No need to cache.
    क्रम (अचिन्हित पूर्णांक i = 0; i < गिनती; i++)
    {
      *first_advance = font->em_scale_x (hmtx.get_advance_without_var_unscaled (*first_glyph));
      first_glyph = &StructAtOffsetUnaligned<hb_codepoint_t> (first_glyph, glyph_stride);
      first_advance = &StructAtOffsetUnaligned<hb_position_t> (first_advance, advance_stride);
    }
    वापस;
  }

#अगर_अघोषित HB_NO_VAR
  /* has_nonzero_coords. */

  ot_font->check_serial (font);
  hb_ot_font_advance_cache_t *advance_cache = ot_font->h.acquire_advance_cache ();
  अगर (!advance_cache)
  {
    // malloc failure. Just use the fallback non-variable path.
    जाओ fallback;
  }

  /* If HVAR is present, use it.*/
  स्थिर OT::HVAR &HVAR = *hmtx.var_table;
  अगर (HVAR.has_तथ्य ())
  {
    स्थिर OT::ItemVariationStore &varStore = &HVAR + HVAR.varStore;
    OT::hb_scalar_cache_t *varStore_cache = ot_font->h.acquire_varStore_cache (varStore);

    क्रम (अचिन्हित पूर्णांक i = 0; i < गिनती; i++)
    {
      hb_position_t v;
      अचिन्हित cv;
      अगर (advance_cache->लो (*first_glyph, &cv))
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
    वापस;
  }

  स्थिर स्वतः &gvar = *ot_face->gvar;
  अगर (gvar.has_तथ्य ())
  {
    स्थिर स्वतः &glyf = *ot_face->glyf;
    स्वतः *scratch = glyf.acquire_scratch ();
    अगर (unlikely (!scratch))
    {
      ot_font->h.release_advance_cache (advance_cache);
      जाओ fallback;
    }
    OT::hb_scalar_cache_t *gvar_cache = ot_font->draw.acquire_gvar_cache (gvar);

    क्रम (अचिन्हित पूर्णांक i = 0; i < गिनती; i++)
    {
      hb_position_t v;
      अचिन्हित cv;
      अगर (advance_cache->लो (*first_glyph, &cv))
	v = cv;
      अन्यथा
      {
        v = glyf.get_advance_with_var_unscaled (*first_glyph, font, false, *scratch, gvar_cache);
	advance_cache->set (*first_glyph, v);
      }
      *first_advance = font->em_scale_x (v);
      first_glyph = &StructAtOffsetUnaligned<hb_codepoint_t> (first_glyph, glyph_stride);
      first_advance = &StructAtOffsetUnaligned<hb_position_t> (first_advance, advance_stride);
    }

    ot_font->draw.release_gvar_cache (gvar_cache);
    glyf.release_scratch (scratch);
    ot_font->h.release_advance_cache (advance_cache);
    वापस;
  }

  ot_font->h.release_advance_cache (advance_cache);
  // No HVAR or GVAR.  Just use the fallback non-variable path.
  जाओ fallback;
#पूर्ण_अगर
}

#अगर_अघोषित HB_NO_VERTICAL
अटल व्योम
hb_ot_get_glyph_v_advances (hb_font_t* font, व्योम* font_तथ्य,
			    अचिन्हित गिनती,
			    स्थिर hb_codepoint_t *first_glyph,
			    अचिन्हित glyph_stride,
			    hb_position_t *first_advance,
			    अचिन्हित advance_stride,
			    व्योम *user_तथ्य HB_UNUSED)
{
  // Duplicated from h_advances. Ugly. Keep in sync'ish.

  स्थिर hb_ot_font_t *ot_font = (स्थिर hb_ot_font_t *) font_तथ्य;
  स्थिर hb_ot_face_t *ot_face = ot_font->ot_face;
  स्थिर OT::vmtx_accelerator_t &vmtx = *ot_face->vmtx;

  अगर (unlikely (!vmtx.has_तथ्य ()))
  {
    hb_font_extents_t font_extents;
    font->get_h_extents_with_fallback (&font_extents);
    hb_position_t advance = font_extents.descender - font_extents.ascender;
    क्रम (अचिन्हित पूर्णांक i = 0; i < गिनती; i++)
    {
      *first_advance = advance;
      first_advance = &StructAtOffsetUnaligned<hb_position_t> (first_advance, advance_stride);
    }
    वापस;
  }

#अगर_अघोषित HB_NO_VAR
  अगर (!font->has_nonzero_coords)
  {
  fallback:
#अन्यथा
  {
#पूर्ण_अगर
    // Just plain vtmx data. No need to cache.
    क्रम (अचिन्हित पूर्णांक i = 0; i < गिनती; i++)
    {
      *first_advance = font->em_scale_y (- (पूर्णांक) vmtx.get_advance_without_var_unscaled (*first_glyph));
      first_glyph = &StructAtOffsetUnaligned<hb_codepoint_t> (first_glyph, glyph_stride);
      first_advance = &StructAtOffsetUnaligned<hb_position_t> (first_advance, advance_stride);
    }
    वापस;
  }

#अगर_अघोषित HB_NO_VAR
  /* has_nonzero_coords. */

  ot_font->check_serial (font);
  hb_ot_font_advance_cache_t *advance_cache = ot_font->v.acquire_advance_cache ();
  अगर (!advance_cache)
  {
    // malloc failure. Just use the fallback non-variable path.
    जाओ fallback;
  }

  /* If VVAR is present, use it.*/
  स्थिर OT::VVAR &VVAR = *vmtx.var_table;
  अगर (VVAR.has_तथ्य ())
  {
    स्थिर OT::ItemVariationStore &varStore = &VVAR + VVAR.varStore;
    OT::hb_scalar_cache_t *varStore_cache = ot_font->v.acquire_varStore_cache (varStore);

    क्रम (अचिन्हित पूर्णांक i = 0; i < गिनती; i++)
    {
      hb_position_t v;
      अचिन्हित cv;
      अगर (advance_cache->लो (*first_glyph, &cv))
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
    वापस;
  }

  स्थिर स्वतः &gvar = *ot_face->gvar;
  अगर (gvar.has_तथ्य ())
  {
    स्थिर स्वतः &glyf = *ot_face->glyf;
    स्वतः *scratch = glyf.acquire_scratch ();
    अगर (unlikely (!scratch))
    {
      ot_font->v.release_advance_cache (advance_cache);
      जाओ fallback;
    }
    OT::hb_scalar_cache_t *gvar_cache = ot_font->draw.acquire_gvar_cache (gvar);

    क्रम (अचिन्हित पूर्णांक i = 0; i < गिनती; i++)
    {
      hb_position_t v;
      अचिन्हित cv;
      अगर (advance_cache->लो (*first_glyph, &cv))
	v = cv;
      अन्यथा
      {
        v = glyf.get_advance_with_var_unscaled (*first_glyph, font, true, *scratch, gvar_cache);
	advance_cache->set (*first_glyph, v);
      }
      *first_advance = font->em_scale_y (- (पूर्णांक) v);
      first_glyph = &StructAtOffsetUnaligned<hb_codepoint_t> (first_glyph, glyph_stride);
      first_advance = &StructAtOffsetUnaligned<hb_position_t> (first_advance, advance_stride);
    }

    ot_font->draw.release_gvar_cache (gvar_cache);
    glyf.release_scratch (scratch);
    ot_font->v.release_advance_cache (advance_cache);
    वापस;
  }

  ot_font->v.release_advance_cache (advance_cache);
  // No VVAR or GVAR.  Just use the fallback non-variable path.
  जाओ fallback;
#पूर्ण_अगर
}
#पूर्ण_अगर

#अगर_अघोषित HB_NO_VERTICAL
HB_HOT
अटल hb_bool_t
hb_ot_get_glyph_v_origins (hb_font_t *font,
			   व्योम *font_तथ्य,
			   अचिन्हित पूर्णांक गिनती,
			   स्थिर hb_codepoint_t *first_glyph,
			   अचिन्हित glyph_stride,
			   hb_position_t *first_x,
			   अचिन्हित x_stride,
			   hb_position_t *first_y,
			   अचिन्हित y_stride,
			   व्योम *user_तथ्य HB_UNUSED)
{
  स्थिर hb_ot_font_t *ot_font = (स्थिर hb_ot_font_t *) font_तथ्य;
  स्थिर hb_ot_face_t *ot_face = ot_font->ot_face;

  ot_font->check_serial (font);

  /* First, set all the x values to half the advance width. */
  font->get_glyph_h_advances (गिनती,
			      first_glyph, glyph_stride,
			      first_x, x_stride);
  क्रम (अचिन्हित i = 0; i < गिनती; i++)
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
  स्थिर OT::VORG &VORG = *ot_face->VORG;
  अगर (origin_cache && VORG.has_तथ्य ())
  {
#अगर_अघोषित HB_NO_VAR
    अगर (!font->has_nonzero_coords)
#पूर्ण_अगर
    {
      क्रम (अचिन्हित i = 0; i < गिनती; i++)
      {
	hb_position_t origin;
	अचिन्हित cv;
	अगर (origin_cache->लो (*first_glyph, &cv))
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
#अगर_अघोषित HB_NO_VAR
    अन्यथा
    {
      स्थिर OT::VVAR &VVAR = *ot_face->vmtx->var_table;
      स्थिर स्वतः &varStore = &VVAR + VVAR.varStore;
      स्वतः *varStore_cache = ot_font->v_origin.acquire_varStore_cache (varStore);
      क्रम (अचिन्हित i = 0; i < गिनती; i++)
      {
	hb_position_t origin;
	अचिन्हित cv;
	अगर (origin_cache->लो (*first_glyph, &cv))
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
#पूर्ण_अगर
    ot_font->v_origin.release_origin_cache (origin_cache);
    वापस true;
  }

  /* If and only if `vmtx` is present and it's a `glyf` font,
   * we use the top phantom point, deduced from vmtx,glyf[,gvar]. */
  स्थिर स्वतः &vmtx = *ot_face->vmtx;
  स्थिर स्वतः &glyf = *ot_face->glyf;
  अगर (origin_cache && vmtx.has_तथ्य() && glyf.has_तथ्य ())
  {
    स्वतः *scratch = glyf.acquire_scratch ();
    अगर (unlikely (!scratch))
    {
      ot_font->v_origin.release_origin_cache (origin_cache);
      वापस false;
    }
    OT::hb_scalar_cache_t *gvar_cache = nullptr;
#अगर_अघोषित HB_NO_VAR
    gvar_cache = font->has_nonzero_coords ?
					ot_font->draw.acquire_gvar_cache (*ot_face->gvar) :
					nullptr;
#पूर्ण_अगर

    क्रम (अचिन्हित i = 0; i < गिनती; i++)
    {
      hb_position_t origin;
      अचिन्हित cv;
      अगर (origin_cache->लो (*first_glyph, &cv))
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

#अगर_अघोषित HB_NO_VAR
    अगर (gvar_cache)
      ot_font->draw.release_gvar_cache (gvar_cache);
#पूर्ण_अगर
    glyf.release_scratch (scratch);
    ot_font->v_origin.release_origin_cache (origin_cache);
    वापस true;
  }

  /* Otherwise, use glyph extents to center the glyph vertically.
   * If getting glyph extents failed, just use the font ascender. */
  अगर (origin_cache && font->has_glyph_extents_func ())
  {
    hb_font_extents_t font_extents;
    font->get_h_extents_with_fallback (&font_extents);
    hb_position_t font_advance = font_extents.ascender - font_extents.descender;

    क्रम (अचिन्हित i = 0; i < गिनती; i++)
    {
      hb_position_t origin;
      अचिन्हित cv;

      अगर (origin_cache->लो (*first_glyph, &cv))
	origin = font->y_scale < 0 ? -static_cast<hb_position_t>(cv) : static_cast<hb_position_t>(cv);
      अन्यथा
      {
	hb_glyph_extents_t extents = {0};
	अगर (likely (font->get_glyph_extents (*first_glyph, &extents)))
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
  वापस true;
}
#पूर्ण_अगर

अटल hb_bool_t
hb_ot_get_glyph_extents (hb_font_t *font,
			 व्योम *font_तथ्य,
			 hb_codepoint_t glyph,
			 hb_glyph_extents_t *extents,
			 व्योम *user_तथ्य HB_UNUSED)
{
  स्थिर hb_ot_font_t *ot_font = (स्थिर hb_ot_font_t *) font_तथ्य;
  स्थिर hb_ot_face_t *ot_face = ot_font->ot_face;

#अगर !defined(HB_NO_OT_FONT_BITMAP) && !defined(HB_NO_COLOR)
  अगर (ot_face->sbix->get_extents (font, glyph, extents)) वापस true;
  अगर (ot_face->CBDT->get_extents (font, glyph, extents)) वापस true;
#पूर्ण_अगर
#अगर !defined(HB_NO_COLOR) && !defined(HB_NO_PAINT)
  अगर (ot_face->COLR->get_extents (font, glyph, extents)) वापस true;
#पूर्ण_अगर
#अगर_अघोषित HB_NO_VAR_COMPOSITES
  अगर (ot_face->VARC->get_extents (font, glyph, extents)) वापस true;
#पूर्ण_अगर
  अगर (ot_face->glyf->get_extents (font, glyph, extents)) वापस true;
#अगर_अघोषित HB_NO_OT_FONT_CFF
  अगर (ot_face->cff2->get_extents (font, glyph, extents)) वापस true;
  अगर (ot_face->cff1->get_extents (font, glyph, extents)) वापस true;
#पूर्ण_अगर

  वापस false;
}

#अगर_अघोषित HB_NO_OT_FONT_GLYPH_NAMES
अटल hb_bool_t
hb_ot_get_glyph_name (hb_font_t *font HB_UNUSED,
		      व्योम *font_तथ्य,
		      hb_codepoint_t glyph,
		      अक्षर *name, अचिन्हित पूर्णांक आकार,
		      व्योम *user_तथ्य HB_UNUSED)
{
  स्थिर hb_ot_font_t *ot_font = (स्थिर hb_ot_font_t *) font_तथ्य;
  स्थिर hb_ot_face_t *ot_face = ot_font->ot_face;

  अगर (ot_face->post->get_glyph_name (glyph, name, आकार)) वापस true;
#अगर_अघोषित HB_NO_OT_FONT_CFF
  अगर (ot_face->cff1->get_glyph_name (glyph, name, आकार)) वापस true;
#पूर्ण_अगर
  वापस false;
}
अटल hb_bool_t
hb_ot_get_glyph_from_name (hb_font_t *font HB_UNUSED,
			   व्योम *font_तथ्य,
			   स्थिर अक्षर *name, पूर्णांक len,
			   hb_codepoint_t *glyph,
			   व्योम *user_तथ्य HB_UNUSED)
{
  स्थिर hb_ot_font_t *ot_font = (स्थिर hb_ot_font_t *) font_तथ्य;
  स्थिर hb_ot_face_t *ot_face = ot_font->ot_face;

  अगर (ot_face->post->get_glyph_from_name (name, len, glyph)) वापस true;
#अगर_अघोषित HB_NO_OT_FONT_CFF
    अगर (ot_face->cff1->get_glyph_from_name (name, len, glyph)) वापस true;
#पूर्ण_अगर
  वापस false;
}
#पूर्ण_अगर

अटल hb_bool_t
hb_ot_get_font_h_extents (hb_font_t *font,
			  व्योम *font_तथ्य HB_UNUSED,
			  hb_font_extents_t *metrics,
			  व्योम *user_तथ्य HB_UNUSED)
{
  वापस _hb_ot_metrics_get_position_common (font, HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER, &metrics->ascender) &&
	 _hb_ot_metrics_get_position_common (font, HB_OT_METRICS_TAG_HORIZONTAL_DESCENDER, &metrics->descender) &&
	 _hb_ot_metrics_get_position_common (font, HB_OT_METRICS_TAG_HORIZONTAL_पंक्ति_GAP, &metrics->पंक्ति_gap);
}

#अगर_अघोषित HB_NO_VERTICAL
अटल hb_bool_t
hb_ot_get_font_v_extents (hb_font_t *font,
			  व्योम *font_तथ्य HB_UNUSED,
			  hb_font_extents_t *metrics,
			  व्योम *user_तथ्य HB_UNUSED)
{
  वापस _hb_ot_metrics_get_position_common (font, HB_OT_METRICS_TAG_VERTICAL_ASCENDER, &metrics->ascender) &&
	 _hb_ot_metrics_get_position_common (font, HB_OT_METRICS_TAG_VERTICAL_DESCENDER, &metrics->descender) &&
	 _hb_ot_metrics_get_position_common (font, HB_OT_METRICS_TAG_VERTICAL_पंक्ति_GAP, &metrics->पंक्ति_gap);
}
#पूर्ण_अगर

#अगर_अघोषित HB_NO_DRAW
अटल hb_bool_t
hb_ot_draw_glyph_or_fail (hb_font_t *font,
			  व्योम *font_तथ्य HB_UNUSED,
			  hb_codepoint_t glyph,
			  hb_draw_funcs_t *draw_funcs, व्योम *draw_तथ्य,
			  व्योम *user_तथ्य)
{
  स्थिर hb_ot_font_t *ot_font = (स्थिर hb_ot_font_t *) font_तथ्य;
  hb_draw_session_t draw_session {draw_funcs, draw_तथ्य};

  OT::hb_scalar_cache_t *gvar_cache = nullptr;
#अगर_अघोषित HB_NO_VAR
  अगर (font->अंक_coords)
  {
    ot_font->check_serial (font);
    gvar_cache = ot_font->draw.acquire_gvar_cache (*ot_font->ot_face->gvar);
  }
#पूर्ण_अगर
  HB_SCOPE_GUARD (ot_font->draw.release_gvar_cache (gvar_cache));

#अगर_अघोषित HB_NO_VAR_COMPOSITES
  अगर (font->face->table.VARC->get_path (font, glyph, draw_session)) वापस true;
#पूर्ण_अगर
  // Keep the following in synch with VARC::get_path_at()
  अगर (font->face->table.glyf->get_path (font, glyph, draw_session, gvar_cache,
				       &draw_session.get_budget ())) वापस true;

#अगर_अघोषित HB_NO_CFF
  अगर (font->face->table.cff2->get_path (font, glyph, draw_session,
				       &draw_session.get_budget ())) वापस true;
  अगर (font->face->table.cff1->get_path (font, glyph, draw_session,
				       &draw_session.get_budget ())) वापस true;
#पूर्ण_अगर

  वापस false;
}
#पूर्ण_अगर

#अगर_अघोषित HB_NO_PAINT
अटल hb_bool_t
hb_ot_paint_glyph_or_fail (hb_font_t *font,
			   व्योम *font_तथ्य,
			   hb_codepoint_t glyph,
			   hb_paint_funcs_t *paint_funcs, व्योम *paint_तथ्य,
			   अचिन्हित पूर्णांक palette,
			   hb_color_t foreground,
			   व्योम *user_तथ्य)
{
#अगर_अघोषित HB_NO_COLOR
  अगर (font->face->table.COLR->paint_glyph (font, glyph, paint_funcs, paint_तथ्य, palette, foreground)) वापस true;
#अगर_अघोषित HB_NO_SVG
  अगर (font->face->table.SVG->paint_glyph (font, glyph, paint_funcs, paint_तथ्य)) वापस true;
#पूर्ण_अगर
#अगर_अघोषित HB_NO_OT_FONT_BITMAP
  अगर (font->face->table.CBDT->paint_glyph (font, glyph, paint_funcs, paint_तथ्य)) वापस true;
  अगर (font->face->table.sbix->paint_glyph (font, glyph, paint_funcs, paint_तथ्य)) वापस true;
#पूर्ण_अगर
#पूर्ण_अगर
  वापस false;
}
#पूर्ण_अगर

अटल अंतरभूत व्योम free_static_ot_funcs ();

अटल काष्ठा hb_ot_font_funcs_lazy_loader_t : hb_font_funcs_lazy_loader_t<hb_ot_font_funcs_lazy_loader_t>
{
  अटल hb_font_funcs_t *create ()
  {
    hb_font_funcs_t *funcs = hb_font_funcs_create ();

    hb_font_funcs_set_nominal_glyph_func (funcs, hb_ot_get_nominal_glyph, nullptr, nullptr);
    hb_font_funcs_set_nominal_glyphs_func (funcs, hb_ot_get_nominal_glyphs, nullptr, nullptr);
    hb_font_funcs_set_variation_glyph_func (funcs, hb_ot_get_variation_glyph, nullptr, nullptr);

    hb_font_funcs_set_font_h_extents_func (funcs, hb_ot_get_font_h_extents, nullptr, nullptr);
    hb_font_funcs_set_glyph_h_advances_func (funcs, hb_ot_get_glyph_h_advances, nullptr, nullptr);

#अगर_अघोषित HB_NO_VERTICAL
    hb_font_funcs_set_font_v_extents_func (funcs, hb_ot_get_font_v_extents, nullptr, nullptr);
    hb_font_funcs_set_glyph_v_advances_func (funcs, hb_ot_get_glyph_v_advances, nullptr, nullptr);
    hb_font_funcs_set_glyph_v_origins_func (funcs, hb_ot_get_glyph_v_origins, nullptr, nullptr);
#पूर्ण_अगर

#अगर_अघोषित HB_NO_DRAW
    hb_font_funcs_set_draw_glyph_or_fail_func (funcs, hb_ot_draw_glyph_or_fail, nullptr, nullptr);
#पूर्ण_अगर

#अगर_अघोषित HB_NO_PAINT
    hb_font_funcs_set_paint_glyph_or_fail_func (funcs, hb_ot_paint_glyph_or_fail, nullptr, nullptr);
#पूर्ण_अगर

    hb_font_funcs_set_glyph_extents_func (funcs, hb_ot_get_glyph_extents, nullptr, nullptr);
    //hb_font_funcs_set_glyph_contour_point_func (funcs, hb_ot_get_glyph_contour_point, nullptr, nullptr);

#अगर_अघोषित HB_NO_OT_FONT_GLYPH_NAMES
    hb_font_funcs_set_glyph_name_func (funcs, hb_ot_get_glyph_name, nullptr, nullptr);
    hb_font_funcs_set_glyph_from_name_func (funcs, hb_ot_get_glyph_from_name, nullptr, nullptr);
#पूर्ण_अगर

    hb_font_funcs_make_immutable (funcs);

    hb_atexit (free_static_ot_funcs);

    वापस funcs;
  }
} static_ot_funcs;

अटल अंतरभूत
व्योम free_static_ot_funcs ()
{
  static_ot_funcs.free_instance ();
}

अटल hb_font_funcs_t *
_hb_ot_get_font_funcs ()
{
  वापस static_ot_funcs.get_unconst ();
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
व्योम
hb_ot_font_set_funcs (hb_font_t *font)
{
  hb_ot_font_t *ot_font = _hb_ot_font_create (font);
  अगर (unlikely (!ot_font))
    वापस;

  hb_font_set_funcs (font,
		     _hb_ot_get_font_funcs (),
		     ot_font,
		     _hb_ot_font_destroy);
}

#पूर्ण_अगर
