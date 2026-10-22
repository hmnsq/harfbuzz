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

#అన్తర్భావ "hb.hh"

#నిర్వచితం_కాకపోతే HB_NO_OT_FONT

#అన్తర్భావ "hb-ot.h"

#అన్తర్భావ "hb-cache.hh"
#అన్తర్భావ "hb-font.hh"
#అన్తర్భావ "hb-machinery.hh"
#అన్తర్భావ "hb-ot-face.hh"

#అన్తర్భావ "hb-ot-cmap-table.hh"
#అన్తర్భావ "hb-ot-glyf-table.hh"
#అన్తర్భావ "hb-ot-var-gvar-table.hh"
#అన్తర్భావ "hb-ot-cff2-table.hh"
#అన్తర్భావ "hb-ot-cff1-table.hh"
#అన్తర్భావ "hb-ot-hmtx-table.hh"
#అన్తర్భావ "hb-ot-post-table.hh"
#అన్తర్భావ "hb-ot-stat-table.hh"
#అన్తర్భావ "hb-ot-var-varc-table.hh"
#అన్తర్భావ "hb-ot-vorg-table.hh"
#అన్తర్భావ "OT/Color/CBDT/CBDT.hh"
#అన్తర్భావ "OT/Color/COLR/COLR.hh"
#అన్తర్భావ "OT/Color/sbix/sbix.hh"
#అన్తర్భావ "OT/Color/svg/svg.hh"


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

ఉపయోగించి hb_ot_font_advance_cache_t = hb_cache_t<24, 16>;
static_assert (పరిమాణం (hb_ot_font_advance_cache_t) == 1024, "");

ఉపయోగించి hb_ot_font_origin_cache_t = hb_cache_t<20, 20>;
static_assert (పరిమాణం (hb_ot_font_origin_cache_t) == 1024, "");

నిర్మాణం hb_ot_font_t
{
  స్థిరరాశి hb_ot_face_t *ot_face;

  మార్చదగిన hb_atomic_t<పూర్ణాంకం> cached_serial;
  మార్చదగిన hb_atomic_t<పూర్ణాంకం> cached_coords_serial;

  నిర్మాణం direction_cache_t
  {
    మార్చదగిన hb_atomic_t<hb_ot_font_advance_cache_t *> advance_cache;
    మార్చదగిన hb_atomic_t<OT::hb_scalar_cache_t *> varStore_cache;

    ~direction_cache_t ()
    {
      clear ();
    }

    hb_ot_font_advance_cache_t *acquire_advance_cache () స్థిరరాశి
    {
    retry:
      స్వయంచాలక *cache = advance_cache.get_acquire ();
      ఒకవేళ (!cache)
      {
        cache = (hb_ot_font_advance_cache_t *) hb_malloc (పరిమాణం (hb_ot_font_advance_cache_t));
	ఒకవేళ (!cache)
	  తిరిగి శూన్యసూచిక;
	కొత్త (cache) hb_ot_font_advance_cache_t;
	తిరిగి cache;
      }
      ఒకవేళ (advance_cache.cmpexch (cache, శూన్యసూచిక))
        తిరిగి cache;
      లేకపోతే
        వెళ్ళు retry;
    }
    శూన్య release_advance_cache (hb_ot_font_advance_cache_t *cache) స్థిరరాశి
    {
      ఒకవేళ (!cache)
        తిరిగి;
      ఒకవేళ (!advance_cache.cmpexch (శూన్యసూచిక, cache))
        hb_free (cache);
    }
    శూన్య clear_advance_cache () స్థిరరాశి
    {
    retry:
      స్వయంచాలక *cache = advance_cache.get_acquire ();
      ఒకవేళ (!cache)
	తిరిగి;
      ఒకవేళ (advance_cache.cmpexch (cache, శూన్యసూచిక))
	hb_free (cache);
      లేకపోతే
        వెళ్ళు retry;
    }

    OT::hb_scalar_cache_t *acquire_varStore_cache (స్థిరరాశి OT::అంశం_Variation_Store &varStore) స్థిరరాశి
    {
    retry:
      స్వయంచాలక *cache = varStore_cache.get_acquire ();
      ఒకవేళ (!cache)
	తిరిగి varStore.create_cache ();
      ఒకవేళ (varStore_cache.cmpexch (cache, శూన్యసూచిక))
	తిరిగి cache;
      లేకపోతే
	వెళ్ళు retry;
    }
    శూన్య release_varStore_cache (OT::hb_scalar_cache_t *cache) స్థిరరాశి
    {
      ఒకవేళ (!cache)
	తిరిగి;
      ఒకవేళ (!varStore_cache.cmpexch (శూన్యసూచిక, cache))
	OT::అంశం_Variation_Store::destroy_cache (cache);
    }
    శూన్య clear_varStore_cache () స్థిరరాశి
    {
    retry:
      స్వయంచాలక *cache = varStore_cache.get_acquire ();
      ఒకవేళ (!cache)
	తిరిగి;
      ఒకవేళ (varStore_cache.cmpexch (cache, శూన్యసూచిక))
	OT::అంశం_Variation_Store::destroy_cache (cache);
      లేకపోతే
	వెళ్ళు retry;
    }

    శూన్య clear () స్థిరరాశి
    {
      clear_advance_cache ();
      clear_varStore_cache ();
    }

  } h, v;

  నిర్మాణం origin_cache_t
  {
    మార్చదగిన hb_atomic_t<hb_ot_font_origin_cache_t *> origin_cache;
    మార్చదగిన hb_atomic_t<OT::hb_scalar_cache_t *> varStore_cache;

    ~origin_cache_t ()
    {
      clear ();
    }

    hb_ot_font_origin_cache_t *acquire_origin_cache () స్థిరరాశి
    {
    retry:
      స్వయంచాలక *cache = origin_cache.get_acquire ();
      ఒకవేళ (!cache)
      {
        cache = (hb_ot_font_origin_cache_t *) hb_malloc (పరిమాణం (hb_ot_font_origin_cache_t));
	ఒకవేళ (!cache)
	  తిరిగి శూన్యసూచిక;
	కొత్త (cache) hb_ot_font_origin_cache_t;
	తిరిగి cache;
      }
      ఒకవేళ (origin_cache.cmpexch (cache, శూన్యసూచిక))
        తిరిగి cache;
      లేకపోతే
        వెళ్ళు retry;
    }
    శూన్య release_origin_cache (hb_ot_font_origin_cache_t *cache) స్థిరరాశి
    {
      ఒకవేళ (!cache)
        తిరిగి;
      ఒకవేళ (!origin_cache.cmpexch (శూన్యసూచిక, cache))
        hb_free (cache);
    }
    శూన్య clear_origin_cache () స్థిరరాశి
    {
    retry:
      స్వయంచాలక *cache = origin_cache.get_acquire ();
      ఒకవేళ (!cache)
	తిరిగి;
      ఒకవేళ (origin_cache.cmpexch (cache, శూన్యసూచిక))
	hb_free (cache);
      లేకపోతే
        వెళ్ళు retry;
    }

    OT::hb_scalar_cache_t *acquire_varStore_cache (స్థిరరాశి OT::అంశం_Variation_Store &varStore) స్థిరరాశి
    {
    retry:
      స్వయంచాలక *cache = varStore_cache.get_acquire ();
      ఒకవేళ (!cache)
	తిరిగి varStore.create_cache ();
      ఒకవేళ (varStore_cache.cmpexch (cache, శూన్యసూచిక))
	తిరిగి cache;
      లేకపోతే
	వెళ్ళు retry;
    }
    శూన్య release_varStore_cache (OT::hb_scalar_cache_t *cache) స్థిరరాశి
    {
      ఒకవేళ (!cache)
	తిరిగి;
      ఒకవేళ (!varStore_cache.cmpexch (శూన్యసూచిక, cache))
	OT::అంశం_Variation_Store::destroy_cache (cache);
    }
    శూన్య clear_varStore_cache () స్థిరరాశి
    {
    retry:
      స్వయంచాలక *cache = varStore_cache.get_acquire ();
      ఒకవేళ (!cache)
	తిరిగి;
      ఒకవేళ (varStore_cache.cmpexch (cache, శూన్యసూచిక))
	OT::అంశం_Variation_Store::destroy_cache (cache);
      లేకపోతే
	వెళ్ళు retry;
    }

    శూన్య clear () స్థిరరాశి
    {
      clear_origin_cache ();
      clear_varStore_cache ();
    }
  } v_origin;

  నిర్మాణం draw_cache_t
  {
    మార్చదగిన hb_atomic_t<OT::hb_scalar_cache_t *> gvar_cache;

    ~draw_cache_t ()
    {
      clear ();
    }

    OT::hb_scalar_cache_t *acquire_gvar_cache (స్థిరరాశి OT::gvar_accelerator_t &gvar) స్థిరరాశి
    {
    retry:
      స్వయంచాలక *cache = gvar_cache.get_acquire ();
      ఒకవేళ (!cache)
	తిరిగి gvar.create_cache ();
      ఒకవేళ (gvar_cache.cmpexch (cache, శూన్యసూచిక))
	తిరిగి cache;
      లేకపోతే
	వెళ్ళు retry;
    }
    శూన్య release_gvar_cache (OT::hb_scalar_cache_t *cache) స్థిరరాశి
    {
      ఒకవేళ (!cache)
	తిరిగి;
      ఒకవేళ (!gvar_cache.cmpexch (శూన్యసూచిక, cache))
	OT::gvar_accelerator_t::destroy_cache (cache);
    }
    శూన్య clear_gvar_cache () స్థిరరాశి
    {
    retry:
      స్వయంచాలక *cache = gvar_cache.get_acquire ();
      ఒకవేళ (!cache)
	తిరిగి;
      ఒకవేళ (gvar_cache.cmpexch (cache, శూన్యసూచిక))
	OT::gvar_accelerator_t::destroy_cache (cache);
      లేకపోతే
	వెళ్ళు retry;
    }

    శూన్య clear () స్థిరరాశి
    {
      clear_gvar_cache ();
    }
  } draw;

  శూన్య check_serial (hb_font_t *font) స్థిరరాశి
  {
    పూర్ణాంకం font_serial = font->serial.get_acquire ();
    ఒకవేళ (cached_serial.get_acquire () != font_serial)
    {
      /* These caches are dependent on scale and synthetic settings.
       * Any change to the font invalidates them. */
      v_origin.clear ();

      cached_serial.సమితి_release (font_serial);
    }

    పూర్ణాంకం font_serial_coords = font->serial_coords.get_acquire ();
    ఒకవేళ (cached_coords_serial.get_acquire () != font_serial_coords)
    {
      /* These caches are independent of scale or synthetic settings.
       * Just variation changes will invalidate them. */
      h.clear ();
      v.clear ();
      draw.clear ();

      cached_coords_serial.సమితి_release (font_serial_coords);
    }
  }
};

స్థిర hb_ot_font_t *
_hb_ot_font_create (hb_font_t *font)
{
  hb_ot_font_t *ot_font = (hb_ot_font_t *) hb_calloc (1, పరిమాణం (hb_ot_font_t));
  ఒకవేళ (unlikely (!ot_font))
    తిరిగి శూన్యసూచిక;

  ot_font->ot_face = &font->face->పట్టిక;

  తిరిగి ot_font;
}

స్థిర శూన్య
_hb_ot_font_destroy (శూన్య *font_దత్తాంశం)
{
  hb_ot_font_t *ot_font = (hb_ot_font_t *) font_దత్తాంశం;

  ot_font->~hb_ot_font_t ();

  hb_free (ot_font);
}

స్థిర hb_bool_t
hb_ot_get_nominal_glyph (hb_font_t *font HB_వాడని,
			 శూన్య *font_దత్తాంశం,
			 hb_codepoint_t unicode,
			 hb_codepoint_t *glyph,
			 శూన్య *user_దత్తాంశం HB_వాడని)
{
  స్థిరరాశి hb_ot_font_t *ot_font = (స్థిరరాశి hb_ot_font_t *) font_దత్తాంశం;
  స్థిరరాశి hb_ot_face_t *ot_face = ot_font->ot_face;
  తిరిగి ot_face->cmap->get_nominal_glyph (unicode, glyph);
}

స్థిర గుర్తులేని పూర్ణాంకం
hb_ot_get_nominal_glyphs (hb_font_t *font HB_వాడని,
			  శూన్య *font_దత్తాంశం,
			  గుర్తులేని పూర్ణాంకం లెక్క,
			  స్థిరరాశి hb_codepoint_t *మొదటి_unicode,
			  గుర్తులేని పూర్ణాంకం unicode_stride,
			  hb_codepoint_t *మొదటి_glyph,
			  గుర్తులేని పూర్ణాంకం glyph_stride,
			  శూన్య *user_దత్తాంశం HB_వాడని)
{
  స్థిరరాశి hb_ot_font_t *ot_font = (స్థిరరాశి hb_ot_font_t *) font_దత్తాంశం;
  స్థిరరాశి hb_ot_face_t *ot_face = ot_font->ot_face;
  తిరిగి ot_face->cmap->get_nominal_glyphs (లెక్క,
					    మొదటి_unicode, unicode_stride,
					    మొదటి_glyph, glyph_stride);
}

స్థిర hb_bool_t
hb_ot_get_variation_glyph (hb_font_t *font HB_వాడని,
			   శూన్య *font_దత్తాంశం,
			   hb_codepoint_t unicode,
			   hb_codepoint_t variation_selector,
			   hb_codepoint_t *glyph,
			   శూన్య *user_దత్తాంశం HB_వాడని)
{
  స్థిరరాశి hb_ot_font_t *ot_font = (స్థిరరాశి hb_ot_font_t *) font_దత్తాంశం;
  స్థిరరాశి hb_ot_face_t *ot_face = ot_font->ot_face;
  తిరిగి ot_face->cmap->get_variation_glyph (unicode,
                                             variation_selector, glyph);
}

స్థిర శూన్య
hb_ot_get_glyph_h_advances (hb_font_t* font, శూన్య* font_దత్తాంశం,
			    గుర్తులేని లెక్క,
			    స్థిరరాశి hb_codepoint_t *మొదటి_glyph,
			    గుర్తులేని glyph_stride,
			    hb_position_t *మొదటి_advance,
			    గుర్తులేని advance_stride,
			    శూన్య *user_దత్తాంశం HB_వాడని)
{
  // Duplicated in v_advances. Ugly. Keep in sync'ish.

  స్థిరరాశి hb_ot_font_t *ot_font = (స్థిరరాశి hb_ot_font_t *) font_దత్తాంశం;
  స్థిరరాశి hb_ot_face_t *ot_face = ot_font->ot_face;
  స్థిరరాశి OT::hmtx_accelerator_t &hmtx = *ot_face->hmtx;

  ఒకవేళ (unlikely (!hmtx.has_దత్తాంశం ()))
  {
    hb_position_t advance = font->face->get_upem () / 2;
    advance = font->em_scale_x (advance);
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++)
    {
      *మొదటి_advance = advance;
      మొదటి_advance = &Struct_At_స్థానభ్రంశం_Unaligned<hb_position_t> (మొదటి_advance, advance_stride);
    }
    తిరిగి;
  }

#నిర్వచితం_కాకపోతే HB_NO_VAR
  ఒకవేళ (!font->has_nonzero_coords)
  {
  fallback:
#లేకపోతే
  {
#అయితే_ముగింపు
    // Just plain htmx data. No need to cache.
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++)
    {
      *మొదటి_advance = font->em_scale_x (hmtx.get_advance_without_var_unscaled (*మొదటి_glyph));
      మొదటి_glyph = &Struct_At_స్థానభ్రంశం_Unaligned<hb_codepoint_t> (మొదటి_glyph, glyph_stride);
      మొదటి_advance = &Struct_At_స్థానభ్రంశం_Unaligned<hb_position_t> (మొదటి_advance, advance_stride);
    }
    తిరిగి;
  }

#నిర్వచితం_కాకపోతే HB_NO_VAR
  /* has_nonzero_coords. */

  ot_font->check_serial (font);
  hb_ot_font_advance_cache_t *advance_cache = ot_font->h.acquire_advance_cache ();
  ఒకవేళ (!advance_cache)
  {
    // malloc failure. Just use the fallback non-variable path.
    వెళ్ళు fallback;
  }

  /* If HVAR is present, use it.*/
  స్థిరరాశి OT::HVAR &HVAR = *hmtx.var_పట్టిక;
  ఒకవేళ (HVAR.has_దత్తాంశం ())
  {
    స్థిరరాశి OT::అంశం_Variation_Store &varStore = &HVAR + HVAR.varStore;
    OT::hb_scalar_cache_t *varStore_cache = ot_font->h.acquire_varStore_cache (varStore);

    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++)
    {
      hb_position_t v;
      గుర్తులేని cv;
      ఒకవేళ (advance_cache->get (*మొదటి_glyph, &cv))
	v = cv;
      లేకపోతే
      {
        v = hmtx.get_advance_with_var_unscaled (*మొదటి_glyph, font, varStore_cache);
	advance_cache->సమితి (*మొదటి_glyph, v);
      }
      *మొదటి_advance = font->em_scale_x (v);
      మొదటి_glyph = &Struct_At_స్థానభ్రంశం_Unaligned<hb_codepoint_t> (మొదటి_glyph, glyph_stride);
      మొదటి_advance = &Struct_At_స్థానభ్రంశం_Unaligned<hb_position_t> (మొదటి_advance, advance_stride);
    }

    ot_font->h.release_varStore_cache (varStore_cache);
    ot_font->h.release_advance_cache (advance_cache);
    తిరిగి;
  }

  స్థిరరాశి స్వయంచాలక &gvar = *ot_face->gvar;
  ఒకవేళ (gvar.has_దత్తాంశం ())
  {
    స్థిరరాశి స్వయంచాలక &glyf = *ot_face->glyf;
    స్వయంచాలక *scratch = glyf.acquire_scratch ();
    ఒకవేళ (unlikely (!scratch))
    {
      ot_font->h.release_advance_cache (advance_cache);
      వెళ్ళు fallback;
    }
    OT::hb_scalar_cache_t *gvar_cache = ot_font->draw.acquire_gvar_cache (gvar);

    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++)
    {
      hb_position_t v;
      గుర్తులేని cv;
      ఒకవేళ (advance_cache->get (*మొదటి_glyph, &cv))
	v = cv;
      లేకపోతే
      {
        v = glyf.get_advance_with_var_unscaled (*మొదటి_glyph, font, అబద్ధం, *scratch, gvar_cache);
	advance_cache->సమితి (*మొదటి_glyph, v);
      }
      *మొదటి_advance = font->em_scale_x (v);
      మొదటి_glyph = &Struct_At_స్థానభ్రంశం_Unaligned<hb_codepoint_t> (మొదటి_glyph, glyph_stride);
      మొదటి_advance = &Struct_At_స్థానభ్రంశం_Unaligned<hb_position_t> (మొదటి_advance, advance_stride);
    }

    ot_font->draw.release_gvar_cache (gvar_cache);
    glyf.release_scratch (scratch);
    ot_font->h.release_advance_cache (advance_cache);
    తిరిగి;
  }

  ot_font->h.release_advance_cache (advance_cache);
  // No HVAR or GVAR.  Just use the fallback non-variable path.
  వెళ్ళు fallback;
#అయితే_ముగింపు
}

#నిర్వచితం_కాకపోతే HB_NO_VERTICAL
స్థిర శూన్య
hb_ot_get_glyph_v_advances (hb_font_t* font, శూన్య* font_దత్తాంశం,
			    గుర్తులేని లెక్క,
			    స్థిరరాశి hb_codepoint_t *మొదటి_glyph,
			    గుర్తులేని glyph_stride,
			    hb_position_t *మొదటి_advance,
			    గుర్తులేని advance_stride,
			    శూన్య *user_దత్తాంశం HB_వాడని)
{
  // Duplicated from h_advances. Ugly. Keep in sync'ish.

  స్థిరరాశి hb_ot_font_t *ot_font = (స్థిరరాశి hb_ot_font_t *) font_దత్తాంశం;
  స్థిరరాశి hb_ot_face_t *ot_face = ot_font->ot_face;
  స్థిరరాశి OT::vmtx_accelerator_t &vmtx = *ot_face->vmtx;

  ఒకవేళ (unlikely (!vmtx.has_దత్తాంశం ()))
  {
    hb_font_extents_t font_extents;
    font->get_h_extents_with_fallback (&font_extents);
    hb_position_t advance = font_extents.descender - font_extents.ascender;
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++)
    {
      *మొదటి_advance = advance;
      మొదటి_advance = &Struct_At_స్థానభ్రంశం_Unaligned<hb_position_t> (మొదటి_advance, advance_stride);
    }
    తిరిగి;
  }

#నిర్వచితం_కాకపోతే HB_NO_VAR
  ఒకవేళ (!font->has_nonzero_coords)
  {
  fallback:
#లేకపోతే
  {
#అయితే_ముగింపు
    // Just plain vtmx data. No need to cache.
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++)
    {
      *మొదటి_advance = font->em_scale_y (- (పూర్ణాంకం) vmtx.get_advance_without_var_unscaled (*మొదటి_glyph));
      మొదటి_glyph = &Struct_At_స్థానభ్రంశం_Unaligned<hb_codepoint_t> (మొదటి_glyph, glyph_stride);
      మొదటి_advance = &Struct_At_స్థానభ్రంశం_Unaligned<hb_position_t> (మొదటి_advance, advance_stride);
    }
    తిరిగి;
  }

#నిర్వచితం_కాకపోతే HB_NO_VAR
  /* has_nonzero_coords. */

  ot_font->check_serial (font);
  hb_ot_font_advance_cache_t *advance_cache = ot_font->v.acquire_advance_cache ();
  ఒకవేళ (!advance_cache)
  {
    // malloc failure. Just use the fallback non-variable path.
    వెళ్ళు fallback;
  }

  /* If VVAR is present, use it.*/
  స్థిరరాశి OT::VVAR &VVAR = *vmtx.var_పట్టిక;
  ఒకవేళ (VVAR.has_దత్తాంశం ())
  {
    స్థిరరాశి OT::అంశం_Variation_Store &varStore = &VVAR + VVAR.varStore;
    OT::hb_scalar_cache_t *varStore_cache = ot_font->v.acquire_varStore_cache (varStore);

    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++)
    {
      hb_position_t v;
      గుర్తులేని cv;
      ఒకవేళ (advance_cache->get (*మొదటి_glyph, &cv))
	v = cv;
      లేకపోతే
      {
        v = vmtx.get_advance_with_var_unscaled (*మొదటి_glyph, font, varStore_cache);
	advance_cache->సమితి (*మొదటి_glyph, v);
      }
      *మొదటి_advance = font->em_scale_y (- (పూర్ణాంకం) v);
      మొదటి_glyph = &Struct_At_స్థానభ్రంశం_Unaligned<hb_codepoint_t> (మొదటి_glyph, glyph_stride);
      మొదటి_advance = &Struct_At_స్థానభ్రంశం_Unaligned<hb_position_t> (మొదటి_advance, advance_stride);
    }

    ot_font->v.release_varStore_cache (varStore_cache);
    ot_font->v.release_advance_cache (advance_cache);
    తిరిగి;
  }

  స్థిరరాశి స్వయంచాలక &gvar = *ot_face->gvar;
  ఒకవేళ (gvar.has_దత్తాంశం ())
  {
    స్థిరరాశి స్వయంచాలక &glyf = *ot_face->glyf;
    స్వయంచాలక *scratch = glyf.acquire_scratch ();
    ఒకవేళ (unlikely (!scratch))
    {
      ot_font->v.release_advance_cache (advance_cache);
      వెళ్ళు fallback;
    }
    OT::hb_scalar_cache_t *gvar_cache = ot_font->draw.acquire_gvar_cache (gvar);

    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++)
    {
      hb_position_t v;
      గుర్తులేని cv;
      ఒకవేళ (advance_cache->get (*మొదటి_glyph, &cv))
	v = cv;
      లేకపోతే
      {
        v = glyf.get_advance_with_var_unscaled (*మొదటి_glyph, font, నిజం, *scratch, gvar_cache);
	advance_cache->సమితి (*మొదటి_glyph, v);
      }
      *మొదటి_advance = font->em_scale_y (- (పూర్ణాంకం) v);
      మొదటి_glyph = &Struct_At_స్థానభ్రంశం_Unaligned<hb_codepoint_t> (మొదటి_glyph, glyph_stride);
      మొదటి_advance = &Struct_At_స్థానభ్రంశం_Unaligned<hb_position_t> (మొదటి_advance, advance_stride);
    }

    ot_font->draw.release_gvar_cache (gvar_cache);
    glyf.release_scratch (scratch);
    ot_font->v.release_advance_cache (advance_cache);
    తిరిగి;
  }

  ot_font->v.release_advance_cache (advance_cache);
  // No VVAR or GVAR.  Just use the fallback non-variable path.
  వెళ్ళు fallback;
#అయితే_ముగింపు
}
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే HB_NO_VERTICAL
HB_HOT
స్థిర hb_bool_t
hb_ot_get_glyph_v_origins (hb_font_t *font,
			   శూన్య *font_దత్తాంశం,
			   గుర్తులేని పూర్ణాంకం లెక్క,
			   స్థిరరాశి hb_codepoint_t *మొదటి_glyph,
			   గుర్తులేని glyph_stride,
			   hb_position_t *మొదటి_x,
			   గుర్తులేని x_stride,
			   hb_position_t *మొదటి_y,
			   గుర్తులేని y_stride,
			   శూన్య *user_దత్తాంశం HB_వాడని)
{
  స్థిరరాశి hb_ot_font_t *ot_font = (స్థిరరాశి hb_ot_font_t *) font_దత్తాంశం;
  స్థిరరాశి hb_ot_face_t *ot_face = ot_font->ot_face;

  ot_font->check_serial (font);

  /* First, set all the x values to half the advance width. */
  font->get_glyph_h_advances (లెక్క,
			      మొదటి_glyph, glyph_stride,
			      మొదటి_x, x_stride);
  కొరకు (గుర్తులేని i = 0; i < లెక్క; i++)
  {
    *మొదటి_x /= 2;
    మొదటి_x = &Struct_At_స్థానభ్రంశం_Unaligned<hb_position_t> (మొదటి_x, x_stride);
  }

  /* The vertical origin business is messy...
   *
   * We allocate the cache, then have various code paths that use the cache.
   * Each one is responsible to free it before returning.
   */
  hb_ot_font_origin_cache_t *origin_cache = ot_font->v_origin.acquire_origin_cache ();

  /* If there is VORG, always use it. It uses VVAR for variations if necessary. */
  స్థిరరాశి OT::VORG &VORG = *ot_face->VORG;
  ఒకవేళ (origin_cache && VORG.has_దత్తాంశం ())
  {
#నిర్వచితం_కాకపోతే HB_NO_VAR
    ఒకవేళ (!font->has_nonzero_coords)
#అయితే_ముగింపు
    {
      కొరకు (గుర్తులేని i = 0; i < లెక్క; i++)
      {
	hb_position_t origin;
	గుర్తులేని cv;
	ఒకవేళ (origin_cache->get (*మొదటి_glyph, &cv))
	  origin = font->y_scale < 0 ? -static_cast<hb_position_t>(cv) : static_cast<hb_position_t>(cv);
	లేకపోతే
	{
	  origin = font->em_scalef_y (VORG.get_y_origin (*మొదటి_glyph));
	  origin_cache->సమితి (*మొదటి_glyph, font->y_scale < 0 ? -origin : origin);
	}

	*మొదటి_y = origin;

	మొదటి_glyph = &Struct_At_స్థానభ్రంశం_Unaligned<hb_codepoint_t> (మొదటి_glyph, glyph_stride);
	మొదటి_y = &Struct_At_స్థానభ్రంశం_Unaligned<hb_position_t> (మొదటి_y, y_stride);
      }
    }
#నిర్వచితం_కాకపోతే HB_NO_VAR
    లేకపోతే
    {
      స్థిరరాశి OT::VVAR &VVAR = *ot_face->vmtx->var_పట్టిక;
      స్థిరరాశి స్వయంచాలక &varStore = &VVAR + VVAR.varStore;
      స్వయంచాలక *varStore_cache = ot_font->v_origin.acquire_varStore_cache (varStore);
      కొరకు (గుర్తులేని i = 0; i < లెక్క; i++)
      {
	hb_position_t origin;
	గుర్తులేని cv;
	ఒకవేళ (origin_cache->get (*మొదటి_glyph, &cv))
	  origin = font->y_scale < 0 ? -static_cast<hb_position_t>(cv) : static_cast<hb_position_t>(cv);
	లేకపోతే
	{
	  origin = font->em_scalef_y (VORG.get_y_origin (*మొదటి_glyph) +
				      VVAR.get_vorg_delta_unscaled (*మొదటి_glyph,
								    font->coords, font->అంకె_coords,
								    varStore_cache));
	  origin_cache->సమితి (*మొదటి_glyph, font->y_scale < 0 ? -origin : origin);
	}

	*మొదటి_y = origin;

	మొదటి_glyph = &Struct_At_స్థానభ్రంశం_Unaligned<hb_codepoint_t> (మొదటి_glyph, glyph_stride);
	మొదటి_y = &Struct_At_స్థానభ్రంశం_Unaligned<hb_position_t> (మొదటి_y, y_stride);
      }
      ot_font->v_origin.release_varStore_cache (varStore_cache);
    }
#అయితే_ముగింపు
    ot_font->v_origin.release_origin_cache (origin_cache);
    తిరిగి నిజం;
  }

  /* If and only if `vmtx` is present and it's a `glyf` font,
   * we use the top phantom point, deduced from vmtx,glyf[,gvar]. */
  స్థిరరాశి స్వయంచాలక &vmtx = *ot_face->vmtx;
  స్థిరరాశి స్వయంచాలక &glyf = *ot_face->glyf;
  ఒకవేళ (origin_cache && vmtx.has_దత్తాంశం() && glyf.has_దత్తాంశం ())
  {
    స్వయంచాలక *scratch = glyf.acquire_scratch ();
    ఒకవేళ (unlikely (!scratch))
    {
      ot_font->v_origin.release_origin_cache (origin_cache);
      తిరిగి అబద్ధం;
    }
    OT::hb_scalar_cache_t *gvar_cache = శూన్యసూచిక;
#నిర్వచితం_కాకపోతే HB_NO_VAR
    gvar_cache = font->has_nonzero_coords ?
					ot_font->draw.acquire_gvar_cache (*ot_face->gvar) :
					శూన్యసూచిక;
#అయితే_ముగింపు

    కొరకు (గుర్తులేని i = 0; i < లెక్క; i++)
    {
      hb_position_t origin;
      గుర్తులేని cv;
      ఒకవేళ (origin_cache->get (*మొదటి_glyph, &cv))
	origin = font->y_scale < 0 ? -static_cast<hb_position_t>(cv) : static_cast<hb_position_t>(cv);
      లేకపోతే
      {
	origin = font->em_scalef_y (glyf.get_v_origin_with_var_unscaled (*మొదటి_glyph, font, *scratch, gvar_cache));
	origin_cache->సమితి (*మొదటి_glyph, font->y_scale < 0 ? -origin : origin);
      }

      *మొదటి_y = origin;

      మొదటి_glyph = &Struct_At_స్థానభ్రంశం_Unaligned<hb_codepoint_t> (మొదటి_glyph, glyph_stride);
      మొదటి_y = &Struct_At_స్థానభ్రంశం_Unaligned<hb_position_t> (మొదటి_y, y_stride);
    }

#నిర్వచితం_కాకపోతే HB_NO_VAR
    ఒకవేళ (gvar_cache)
      ot_font->draw.release_gvar_cache (gvar_cache);
#అయితే_ముగింపు
    glyf.release_scratch (scratch);
    ot_font->v_origin.release_origin_cache (origin_cache);
    తిరిగి నిజం;
  }

  /* Otherwise, use glyph extents to center the glyph vertically.
   * If getting glyph extents failed, just use the font ascender. */
  ఒకవేళ (origin_cache && font->has_glyph_extents_func ())
  {
    hb_font_extents_t font_extents;
    font->get_h_extents_with_fallback (&font_extents);
    hb_position_t font_advance = font_extents.ascender - font_extents.descender;

    కొరకు (గుర్తులేని i = 0; i < లెక్క; i++)
    {
      hb_position_t origin;
      గుర్తులేని cv;

      ఒకవేళ (origin_cache->get (*మొదటి_glyph, &cv))
	origin = font->y_scale < 0 ? -static_cast<hb_position_t>(cv) : static_cast<hb_position_t>(cv);
      లేకపోతే
      {
	hb_glyph_extents_t extents = {0};
	ఒకవేళ (likely (font->get_glyph_extents (*మొదటి_glyph, &extents)))
	  origin = hb_clamp_గమ్యం<hb_position_t> ((int64_t) extents.y_bearing +
					 (((int64_t) font_advance + extents.ఎత్తు) >> 1));
	లేకపోతే
	  origin = font_extents.ascender;

	origin_cache->సమితి (*మొదటి_glyph, font->y_scale < 0 ? -origin : origin);
      }

      *మొదటి_y = origin;

      మొదటి_glyph = &Struct_At_స్థానభ్రంశం_Unaligned<hb_codepoint_t> (మొదటి_glyph, glyph_stride);
      మొదటి_y = &Struct_At_స్థానభ్రంశం_Unaligned<hb_position_t> (మొదటి_y, y_stride);
    }
  }

  ot_font->v_origin.release_origin_cache (origin_cache);
  తిరిగి నిజం;
}
#అయితే_ముగింపు

స్థిర hb_bool_t
hb_ot_get_glyph_extents (hb_font_t *font,
			 శూన్య *font_దత్తాంశం,
			 hb_codepoint_t glyph,
			 hb_glyph_extents_t *extents,
			 శూన్య *user_దత్తాంశం HB_వాడని)
{
  స్థిరరాశి hb_ot_font_t *ot_font = (స్థిరరాశి hb_ot_font_t *) font_దత్తాంశం;
  స్థిరరాశి hb_ot_face_t *ot_face = ot_font->ot_face;

#ఒకవేళ !defined(HB_NO_OT_FONT_BITMAP) && !defined(HB_NO_COLOR)
  ఒకవేళ (ot_face->sbix->get_extents (font, glyph, extents)) తిరిగి నిజం;
  ఒకవేళ (ot_face->CBDT->get_extents (font, glyph, extents)) తిరిగి నిజం;
#అయితే_ముగింపు
#ఒకవేళ !defined(HB_NO_COLOR) && !defined(HB_NO_PAINT)
  ఒకవేళ (ot_face->COLR->get_extents (font, glyph, extents)) తిరిగి నిజం;
#అయితే_ముగింపు
#నిర్వచితం_కాకపోతే HB_NO_VAR_COMPOSITES
  ఒకవేళ (ot_face->VARC->get_extents (font, glyph, extents)) తిరిగి నిజం;
#అయితే_ముగింపు
  ఒకవేళ (ot_face->glyf->get_extents (font, glyph, extents)) తిరిగి నిజం;
#నిర్వచితం_కాకపోతే HB_NO_OT_FONT_CFF
  ఒకవేళ (ot_face->cff2->get_extents (font, glyph, extents)) తిరిగి నిజం;
  ఒకవేళ (ot_face->cff1->get_extents (font, glyph, extents)) తిరిగి నిజం;
#అయితే_ముగింపు

  తిరిగి అబద్ధం;
}

#నిర్వచితం_కాకపోతే HB_NO_OT_FONT_GLYPH_పేర్లు
స్థిర hb_bool_t
hb_ot_get_glyph_name (hb_font_t *font HB_వాడని,
		      శూన్య *font_దత్తాంశం,
		      hb_codepoint_t glyph,
		      అక్షరం *name, గుర్తులేని పూర్ణాంకం కొలత,
		      శూన్య *user_దత్తాంశం HB_వాడని)
{
  స్థిరరాశి hb_ot_font_t *ot_font = (స్థిరరాశి hb_ot_font_t *) font_దత్తాంశం;
  స్థిరరాశి hb_ot_face_t *ot_face = ot_font->ot_face;

  ఒకవేళ (ot_face->post->get_glyph_name (glyph, name, కొలత)) తిరిగి నిజం;
#నిర్వచితం_కాకపోతే HB_NO_OT_FONT_CFF
  ఒకవేళ (ot_face->cff1->get_glyph_name (glyph, name, కొలత)) తిరిగి నిజం;
#అయితే_ముగింపు
  తిరిగి అబద్ధం;
}
స్థిర hb_bool_t
hb_ot_get_glyph_from_name (hb_font_t *font HB_వాడని,
			   శూన్య *font_దత్తాంశం,
			   స్థిరరాశి అక్షరం *name, పూర్ణాంకం len,
			   hb_codepoint_t *glyph,
			   శూన్య *user_దత్తాంశం HB_వాడని)
{
  స్థిరరాశి hb_ot_font_t *ot_font = (స్థిరరాశి hb_ot_font_t *) font_దత్తాంశం;
  స్థిరరాశి hb_ot_face_t *ot_face = ot_font->ot_face;

  ఒకవేళ (ot_face->post->get_glyph_from_name (name, len, glyph)) తిరిగి నిజం;
#నిర్వచితం_కాకపోతే HB_NO_OT_FONT_CFF
    ఒకవేళ (ot_face->cff1->get_glyph_from_name (name, len, glyph)) తిరిగి నిజం;
#అయితే_ముగింపు
  తిరిగి అబద్ధం;
}
#అయితే_ముగింపు

స్థిర hb_bool_t
hb_ot_get_font_h_extents (hb_font_t *font,
			  శూన్య *font_దత్తాంశం HB_వాడని,
			  hb_font_extents_t *metrics,
			  శూన్య *user_దత్తాంశం HB_వాడని)
{
  తిరిగి _hb_ot_metrics_get_position_common (font, HB_OT_METRICS_ట్యాగ్_HORIZONTAL_ASCENDER, &metrics->ascender) &&
	 _hb_ot_metrics_get_position_common (font, HB_OT_METRICS_ట్యాగ్_HORIZONTAL_DESCENDER, &metrics->descender) &&
	 _hb_ot_metrics_get_position_common (font, HB_OT_METRICS_ట్యాగ్_HORIZONTAL_పంక్తి_GAP, &metrics->పంక్తి_gap);
}

#నిర్వచితం_కాకపోతే HB_NO_VERTICAL
స్థిర hb_bool_t
hb_ot_get_font_v_extents (hb_font_t *font,
			  శూన్య *font_దత్తాంశం HB_వాడని,
			  hb_font_extents_t *metrics,
			  శూన్య *user_దత్తాంశం HB_వాడని)
{
  తిరిగి _hb_ot_metrics_get_position_common (font, HB_OT_METRICS_ట్యాగ్_VERTICAL_ASCENDER, &metrics->ascender) &&
	 _hb_ot_metrics_get_position_common (font, HB_OT_METRICS_ట్యాగ్_VERTICAL_DESCENDER, &metrics->descender) &&
	 _hb_ot_metrics_get_position_common (font, HB_OT_METRICS_ట్యాగ్_VERTICAL_పంక్తి_GAP, &metrics->పంక్తి_gap);
}
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే HB_NO_DRAW
స్థిర hb_bool_t
hb_ot_draw_glyph_or_fail (hb_font_t *font,
			  శూన్య *font_దత్తాంశం HB_వాడని,
			  hb_codepoint_t glyph,
			  hb_draw_funcs_t *draw_funcs, శూన్య *draw_దత్తాంశం,
			  శూన్య *user_దత్తాంశం)
{
  స్థిరరాశి hb_ot_font_t *ot_font = (స్థిరరాశి hb_ot_font_t *) font_దత్తాంశం;
  hb_draw_session_t draw_session {draw_funcs, draw_దత్తాంశం};

  OT::hb_scalar_cache_t *gvar_cache = శూన్యసూచిక;
#నిర్వచితం_కాకపోతే HB_NO_VAR
  ఒకవేళ (font->అంకె_coords)
  {
    ot_font->check_serial (font);
    gvar_cache = ot_font->draw.acquire_gvar_cache (*ot_font->ot_face->gvar);
  }
#అయితే_ముగింపు
  HB_SCOPE_GUARD (ot_font->draw.release_gvar_cache (gvar_cache));

#నిర్వచితం_కాకపోతే HB_NO_VAR_COMPOSITES
  ఒకవేళ (font->face->పట్టిక.VARC->get_మార్గం (font, glyph, draw_session)) తిరిగి నిజం;
#అయితే_ముగింపు
  // Keep the following in synch with VARC::get_path_at()
  ఒకవేళ (font->face->పట్టిక.glyf->get_మార్గం (font, glyph, draw_session, gvar_cache,
				       &draw_session.get_budget ())) తిరిగి నిజం;

#నిర్వచితం_కాకపోతే HB_NO_CFF
  ఒకవేళ (font->face->పట్టిక.cff2->get_మార్గం (font, glyph, draw_session,
				       &draw_session.get_budget ())) తిరిగి నిజం;
  ఒకవేళ (font->face->పట్టిక.cff1->get_మార్గం (font, glyph, draw_session,
				       &draw_session.get_budget ())) తిరిగి నిజం;
#అయితే_ముగింపు

  తిరిగి అబద్ధం;
}
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే HB_NO_PAINT
స్థిర hb_bool_t
hb_ot_paint_glyph_or_fail (hb_font_t *font,
			   శూన్య *font_దత్తాంశం,
			   hb_codepoint_t glyph,
			   hb_paint_funcs_t *paint_funcs, శూన్య *paint_దత్తాంశం,
			   గుర్తులేని పూర్ణాంకం palette,
			   hb_color_t foreground,
			   శూన్య *user_దత్తాంశం)
{
#నిర్వచితం_కాకపోతే HB_NO_COLOR
  ఒకవేళ (font->face->పట్టిక.COLR->paint_glyph (font, glyph, paint_funcs, paint_దత్తాంశం, palette, foreground)) తిరిగి నిజం;
#నిర్వచితం_కాకపోతే HB_NO_SVG
  ఒకవేళ (font->face->పట్టిక.SVG->paint_glyph (font, glyph, paint_funcs, paint_దత్తాంశం)) తిరిగి నిజం;
#అయితే_ముగింపు
#నిర్వచితం_కాకపోతే HB_NO_OT_FONT_BITMAP
  ఒకవేళ (font->face->పట్టిక.CBDT->paint_glyph (font, glyph, paint_funcs, paint_దత్తాంశం)) తిరిగి నిజం;
  ఒకవేళ (font->face->పట్టిక.sbix->paint_glyph (font, glyph, paint_funcs, paint_దత్తాంశం)) తిరిగి నిజం;
#అయితే_ముగింపు
#అయితే_ముగింపు
  తిరిగి అబద్ధం;
}
#అయితే_ముగింపు

స్థిర అంతఃవరుస శూన్య free_static_ot_funcs ();

స్థిర నిర్మాణం hb_ot_font_funcs_lazy_loader_t : hb_font_funcs_lazy_loader_t<hb_ot_font_funcs_lazy_loader_t>
{
  స్థిర hb_font_funcs_t *create ()
  {
    hb_font_funcs_t *funcs = hb_font_funcs_create ();

    hb_font_funcs_సమితి_nominal_glyph_func (funcs, hb_ot_get_nominal_glyph, శూన్యసూచిక, శూన్యసూచిక);
    hb_font_funcs_సమితి_nominal_glyphs_func (funcs, hb_ot_get_nominal_glyphs, శూన్యసూచిక, శూన్యసూచిక);
    hb_font_funcs_సమితి_variation_glyph_func (funcs, hb_ot_get_variation_glyph, శూన్యసూచిక, శూన్యసూచిక);

    hb_font_funcs_సమితి_font_h_extents_func (funcs, hb_ot_get_font_h_extents, శూన్యసూచిక, శూన్యసూచిక);
    hb_font_funcs_సమితి_glyph_h_advances_func (funcs, hb_ot_get_glyph_h_advances, శూన్యసూచిక, శూన్యసూచిక);

#నిర్వచితం_కాకపోతే HB_NO_VERTICAL
    hb_font_funcs_సమితి_font_v_extents_func (funcs, hb_ot_get_font_v_extents, శూన్యసూచిక, శూన్యసూచిక);
    hb_font_funcs_సమితి_glyph_v_advances_func (funcs, hb_ot_get_glyph_v_advances, శూన్యసూచిక, శూన్యసూచిక);
    hb_font_funcs_సమితి_glyph_v_origins_func (funcs, hb_ot_get_glyph_v_origins, శూన్యసూచిక, శూన్యసూచిక);
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే HB_NO_DRAW
    hb_font_funcs_సమితి_draw_glyph_or_fail_func (funcs, hb_ot_draw_glyph_or_fail, శూన్యసూచిక, శూన్యసూచిక);
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే HB_NO_PAINT
    hb_font_funcs_సమితి_paint_glyph_or_fail_func (funcs, hb_ot_paint_glyph_or_fail, శూన్యసూచిక, శూన్యసూచిక);
#అయితే_ముగింపు

    hb_font_funcs_సమితి_glyph_extents_func (funcs, hb_ot_get_glyph_extents, శూన్యసూచిక, శూన్యసూచిక);
    //hb_font_funcs_set_glyph_contour_point_func (funcs, hb_ot_get_glyph_contour_point, nullptr, nullptr);

#నిర్వచితం_కాకపోతే HB_NO_OT_FONT_GLYPH_పేర్లు
    hb_font_funcs_సమితి_glyph_name_func (funcs, hb_ot_get_glyph_name, శూన్యసూచిక, శూన్యసూచిక);
    hb_font_funcs_సమితి_glyph_from_name_func (funcs, hb_ot_get_glyph_from_name, శూన్యసూచిక, శూన్యసూచిక);
#అయితే_ముగింపు

    hb_font_funcs_make_immutable (funcs);

    hb_atexit (free_static_ot_funcs);

    తిరిగి funcs;
  }
} static_ot_funcs;

స్థిర అంతఃవరుస
శూన్య free_static_ot_funcs ()
{
  static_ot_funcs.free_instance ();
}

స్థిర hb_font_funcs_t *
_hb_ot_get_font_funcs ()
{
  తిరిగి static_ot_funcs.get_unconst ();
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
శూన్య
hb_ot_font_సమితి_funcs (hb_font_t *font)
{
  hb_ot_font_t *ot_font = _hb_ot_font_create (font);
  ఒకవేళ (unlikely (!ot_font))
    తిరిగి;

  hb_font_సమితి_funcs (font,
		     _hb_ot_get_font_funcs (),
		     ot_font,
		     _hb_ot_font_destroy);
}

#అయితే_ముగింపు
