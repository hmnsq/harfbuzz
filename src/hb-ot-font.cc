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

#شامل "hb.hh"

#اگر_غیر_تعریف_شدہ HB_NO_OT_FONT

#شامل "hb-ot.h"

#شامل "hb-cache.hh"
#شامل "hb-font.hh"
#شامل "hb-machinery.hh"
#شامل "hb-ot-face.hh"

#شامل "hb-ot-cmap-table.hh"
#شامل "hb-ot-glyf-table.hh"
#شامل "hb-ot-var-gvar-table.hh"
#شامل "hb-ot-cff2-table.hh"
#شامل "hb-ot-cff1-table.hh"
#شامل "hb-ot-hmtx-table.hh"
#شامل "hb-ot-post-table.hh"
#شامل "hb-ot-stat-table.hh"
#شامل "hb-ot-var-varc-table.hh"
#شامل "hb-ot-vorg-table.hh"
#شامل "OT/Color/CBDT/CBDT.hh"
#شامل "OT/Color/COLR/COLR.hh"
#شامل "OT/Color/sbix/sbix.hh"
#شامل "OT/Color/svg/svg.hh"


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

استعمال hb_ot_font_advance_cache_t = hb_cache_t<24, 16>;
static_assert (حجم (hb_ot_font_advance_cache_t) == 1024, "");

استعمال hb_ot_font_origin_cache_t = hb_cache_t<20, 20>;
static_assert (حجم (hb_ot_font_origin_cache_t) == 1024, "");

ساخت hb_ot_font_t
{
  ثابت hb_ot_face_t *ot_face;

  تغیرپذیر hb_atomic_t<صحیح> cached_serial;
  تغیرپذیر hb_atomic_t<صحیح> cached_coords_serial;

  ساخت direction_cache_t
  {
    تغیرپذیر hb_atomic_t<hb_ot_font_advance_cache_t *> advance_cache;
    تغیرپذیر hb_atomic_t<OT::hb_scalar_cache_t *> varStore_cache;

    ~direction_cache_t ()
    {
      clear ();
    }

    hb_ot_font_advance_cache_t *acquire_advance_cache () ثابت
    {
    retry:
      خودکار *cache = advance_cache.get_acquire ();
      اگر (!cache)
      {
        cache = (hb_ot_font_advance_cache_t *) hb_malloc (حجم (hb_ot_font_advance_cache_t));
	اگر (!cache)
	  واپس خالی_اشاریہ;
	نیا (cache) hb_ot_font_advance_cache_t;
	واپس cache;
      }
      اگر (advance_cache.cmpexch (cache, خالی_اشاریہ))
        واپس cache;
      ورنہ
        جاؤ retry;
    }
    خالی release_advance_cache (hb_ot_font_advance_cache_t *cache) ثابت
    {
      اگر (!cache)
        واپس;
      اگر (!advance_cache.cmpexch (خالی_اشاریہ, cache))
        hb_free (cache);
    }
    خالی clear_advance_cache () ثابت
    {
    retry:
      خودکار *cache = advance_cache.get_acquire ();
      اگر (!cache)
	واپس;
      اگر (advance_cache.cmpexch (cache, خالی_اشاریہ))
	hb_free (cache);
      ورنہ
        جاؤ retry;
    }

    OT::hb_scalar_cache_t *acquire_varStore_cache (ثابت OT::شے_Variation_Store &varStore) ثابت
    {
    retry:
      خودکار *cache = varStore_cache.get_acquire ();
      اگر (!cache)
	واپس varStore.create_cache ();
      اگر (varStore_cache.cmpexch (cache, خالی_اشاریہ))
	واپس cache;
      ورنہ
	جاؤ retry;
    }
    خالی release_varStore_cache (OT::hb_scalar_cache_t *cache) ثابت
    {
      اگر (!cache)
	واپس;
      اگر (!varStore_cache.cmpexch (خالی_اشاریہ, cache))
	OT::شے_Variation_Store::destroy_cache (cache);
    }
    خالی clear_varStore_cache () ثابت
    {
    retry:
      خودکار *cache = varStore_cache.get_acquire ();
      اگر (!cache)
	واپس;
      اگر (varStore_cache.cmpexch (cache, خالی_اشاریہ))
	OT::شے_Variation_Store::destroy_cache (cache);
      ورنہ
	جاؤ retry;
    }

    خالی clear () ثابت
    {
      clear_advance_cache ();
      clear_varStore_cache ();
    }

  } h, v;

  ساخت origin_cache_t
  {
    تغیرپذیر hb_atomic_t<hb_ot_font_origin_cache_t *> origin_cache;
    تغیرپذیر hb_atomic_t<OT::hb_scalar_cache_t *> varStore_cache;

    ~origin_cache_t ()
    {
      clear ();
    }

    hb_ot_font_origin_cache_t *acquire_origin_cache () ثابت
    {
    retry:
      خودکار *cache = origin_cache.get_acquire ();
      اگر (!cache)
      {
        cache = (hb_ot_font_origin_cache_t *) hb_malloc (حجم (hb_ot_font_origin_cache_t));
	اگر (!cache)
	  واپس خالی_اشاریہ;
	نیا (cache) hb_ot_font_origin_cache_t;
	واپس cache;
      }
      اگر (origin_cache.cmpexch (cache, خالی_اشاریہ))
        واپس cache;
      ورنہ
        جاؤ retry;
    }
    خالی release_origin_cache (hb_ot_font_origin_cache_t *cache) ثابت
    {
      اگر (!cache)
        واپس;
      اگر (!origin_cache.cmpexch (خالی_اشاریہ, cache))
        hb_free (cache);
    }
    خالی clear_origin_cache () ثابت
    {
    retry:
      خودکار *cache = origin_cache.get_acquire ();
      اگر (!cache)
	واپس;
      اگر (origin_cache.cmpexch (cache, خالی_اشاریہ))
	hb_free (cache);
      ورنہ
        جاؤ retry;
    }

    OT::hb_scalar_cache_t *acquire_varStore_cache (ثابت OT::شے_Variation_Store &varStore) ثابت
    {
    retry:
      خودکار *cache = varStore_cache.get_acquire ();
      اگر (!cache)
	واپس varStore.create_cache ();
      اگر (varStore_cache.cmpexch (cache, خالی_اشاریہ))
	واپس cache;
      ورنہ
	جاؤ retry;
    }
    خالی release_varStore_cache (OT::hb_scalar_cache_t *cache) ثابت
    {
      اگر (!cache)
	واپس;
      اگر (!varStore_cache.cmpexch (خالی_اشاریہ, cache))
	OT::شے_Variation_Store::destroy_cache (cache);
    }
    خالی clear_varStore_cache () ثابت
    {
    retry:
      خودکار *cache = varStore_cache.get_acquire ();
      اگر (!cache)
	واپس;
      اگر (varStore_cache.cmpexch (cache, خالی_اشاریہ))
	OT::شے_Variation_Store::destroy_cache (cache);
      ورنہ
	جاؤ retry;
    }

    خالی clear () ثابت
    {
      clear_origin_cache ();
      clear_varStore_cache ();
    }
  } v_origin;

  ساخت draw_cache_t
  {
    تغیرپذیر hb_atomic_t<OT::hb_scalar_cache_t *> gvar_cache;

    ~draw_cache_t ()
    {
      clear ();
    }

    OT::hb_scalar_cache_t *acquire_gvar_cache (ثابت OT::gvar_accelerator_t &gvar) ثابت
    {
    retry:
      خودکار *cache = gvar_cache.get_acquire ();
      اگر (!cache)
	واپس gvar.create_cache ();
      اگر (gvar_cache.cmpexch (cache, خالی_اشاریہ))
	واپس cache;
      ورنہ
	جاؤ retry;
    }
    خالی release_gvar_cache (OT::hb_scalar_cache_t *cache) ثابت
    {
      اگر (!cache)
	واپس;
      اگر (!gvar_cache.cmpexch (خالی_اشاریہ, cache))
	OT::gvar_accelerator_t::destroy_cache (cache);
    }
    خالی clear_gvar_cache () ثابت
    {
    retry:
      خودکار *cache = gvar_cache.get_acquire ();
      اگر (!cache)
	واپس;
      اگر (gvar_cache.cmpexch (cache, خالی_اشاریہ))
	OT::gvar_accelerator_t::destroy_cache (cache);
      ورنہ
	جاؤ retry;
    }

    خالی clear () ثابت
    {
      clear_gvar_cache ();
    }
  } draw;

  خالی check_serial (hb_font_t *font) ثابت
  {
    صحیح font_serial = font->serial.get_acquire ();
    اگر (cached_serial.get_acquire () != font_serial)
    {
      /* These caches are dependent on scale and synthetic settings.
       * Any change to the font invalidates them. */
      v_origin.clear ();

      cached_serial.سیٹ_release (font_serial);
    }

    صحیح font_serial_coords = font->serial_coords.get_acquire ();
    اگر (cached_coords_serial.get_acquire () != font_serial_coords)
    {
      /* These caches are independent of scale or synthetic settings.
       * Just variation changes will invalidate them. */
      h.clear ();
      v.clear ();
      draw.clear ();

      cached_coords_serial.سیٹ_release (font_serial_coords);
    }
  }
};

ساکن hb_ot_font_t *
_hb_ot_font_create (hb_font_t *font)
{
  hb_ot_font_t *ot_font = (hb_ot_font_t *) hb_calloc (1, حجم (hb_ot_font_t));
  اگر (unlikely (!ot_font))
    واپس خالی_اشاریہ;

  ot_font->ot_face = &font->face->جدول;

  واپس ot_font;
}

ساکن خالی
_hb_ot_font_destroy (خالی *font_ڈیٹا)
{
  hb_ot_font_t *ot_font = (hb_ot_font_t *) font_ڈیٹا;

  ot_font->~hb_ot_font_t ();

  hb_free (ot_font);
}

ساکن hb_bool_t
hb_ot_get_nominal_glyph (hb_font_t *font HB_غیر_مستعمل,
			 خالی *font_ڈیٹا,
			 hb_codepoint_t unicode,
			 hb_codepoint_t *glyph,
			 خالی *user_ڈیٹا HB_غیر_مستعمل)
{
  ثابت hb_ot_font_t *ot_font = (ثابت hb_ot_font_t *) font_ڈیٹا;
  ثابت hb_ot_face_t *ot_face = ot_font->ot_face;
  واپس ot_face->cmap->get_nominal_glyph (unicode, glyph);
}

ساکن غیرعلامتی صحیح
hb_ot_get_nominal_glyphs (hb_font_t *font HB_غیر_مستعمل,
			  خالی *font_ڈیٹا,
			  غیرعلامتی صحیح گنتی,
			  ثابت hb_codepoint_t *پہلا_unicode,
			  غیرعلامتی صحیح unicode_stride,
			  hb_codepoint_t *پہلا_glyph,
			  غیرعلامتی صحیح glyph_stride,
			  خالی *user_ڈیٹا HB_غیر_مستعمل)
{
  ثابت hb_ot_font_t *ot_font = (ثابت hb_ot_font_t *) font_ڈیٹا;
  ثابت hb_ot_face_t *ot_face = ot_font->ot_face;
  واپس ot_face->cmap->get_nominal_glyphs (گنتی,
					    پہلا_unicode, unicode_stride,
					    پہلا_glyph, glyph_stride);
}

ساکن hb_bool_t
hb_ot_get_variation_glyph (hb_font_t *font HB_غیر_مستعمل,
			   خالی *font_ڈیٹا,
			   hb_codepoint_t unicode,
			   hb_codepoint_t variation_selector,
			   hb_codepoint_t *glyph,
			   خالی *user_ڈیٹا HB_غیر_مستعمل)
{
  ثابت hb_ot_font_t *ot_font = (ثابت hb_ot_font_t *) font_ڈیٹا;
  ثابت hb_ot_face_t *ot_face = ot_font->ot_face;
  واپس ot_face->cmap->get_variation_glyph (unicode,
                                             variation_selector, glyph);
}

ساکن خالی
hb_ot_get_glyph_h_advances (hb_font_t* font, خالی* font_ڈیٹا,
			    غیرعلامتی گنتی,
			    ثابت hb_codepoint_t *پہلا_glyph,
			    غیرعلامتی glyph_stride,
			    hb_position_t *پہلا_advance,
			    غیرعلامتی advance_stride,
			    خالی *user_ڈیٹا HB_غیر_مستعمل)
{
  // Duplicated in v_advances. Ugly. Keep in sync'ish.

  ثابت hb_ot_font_t *ot_font = (ثابت hb_ot_font_t *) font_ڈیٹا;
  ثابت hb_ot_face_t *ot_face = ot_font->ot_face;
  ثابت OT::hmtx_accelerator_t &hmtx = *ot_face->hmtx;

  اگر (unlikely (!hmtx.has_ڈیٹا ()))
  {
    hb_position_t advance = font->face->get_upem () / 2;
    advance = font->em_scale_x (advance);
    برائے (غیرعلامتی صحیح i = 0; i < گنتی; i++)
    {
      *پہلا_advance = advance;
      پہلا_advance = &Struct_At_انحراف_Unaligned<hb_position_t> (پہلا_advance, advance_stride);
    }
    واپس;
  }

#اگر_غیر_تعریف_شدہ HB_NO_VAR
  اگر (!font->has_nonzero_coords)
  {
  fallback:
#ورنہ
  {
#اگر_ختم
    // Just plain htmx data. No need to cache.
    برائے (غیرعلامتی صحیح i = 0; i < گنتی; i++)
    {
      *پہلا_advance = font->em_scale_x (hmtx.get_advance_without_var_unscaled (*پہلا_glyph));
      پہلا_glyph = &Struct_At_انحراف_Unaligned<hb_codepoint_t> (پہلا_glyph, glyph_stride);
      پہلا_advance = &Struct_At_انحراف_Unaligned<hb_position_t> (پہلا_advance, advance_stride);
    }
    واپس;
  }

#اگر_غیر_تعریف_شدہ HB_NO_VAR
  /* has_nonzero_coords. */

  ot_font->check_serial (font);
  hb_ot_font_advance_cache_t *advance_cache = ot_font->h.acquire_advance_cache ();
  اگر (!advance_cache)
  {
    // malloc failure. Just use the fallback non-variable path.
    جاؤ fallback;
  }

  /* If HVAR is present, use it.*/
  ثابت OT::HVAR &HVAR = *hmtx.var_جدول;
  اگر (HVAR.has_ڈیٹا ())
  {
    ثابت OT::شے_Variation_Store &varStore = &HVAR + HVAR.varStore;
    OT::hb_scalar_cache_t *varStore_cache = ot_font->h.acquire_varStore_cache (varStore);

    برائے (غیرعلامتی صحیح i = 0; i < گنتی; i++)
    {
      hb_position_t v;
      غیرعلامتی cv;
      اگر (advance_cache->get (*پہلا_glyph, &cv))
	v = cv;
      ورنہ
      {
        v = hmtx.get_advance_with_var_unscaled (*پہلا_glyph, font, varStore_cache);
	advance_cache->سیٹ (*پہلا_glyph, v);
      }
      *پہلا_advance = font->em_scale_x (v);
      پہلا_glyph = &Struct_At_انحراف_Unaligned<hb_codepoint_t> (پہلا_glyph, glyph_stride);
      پہلا_advance = &Struct_At_انحراف_Unaligned<hb_position_t> (پہلا_advance, advance_stride);
    }

    ot_font->h.release_varStore_cache (varStore_cache);
    ot_font->h.release_advance_cache (advance_cache);
    واپس;
  }

  ثابت خودکار &gvar = *ot_face->gvar;
  اگر (gvar.has_ڈیٹا ())
  {
    ثابت خودکار &glyf = *ot_face->glyf;
    خودکار *scratch = glyf.acquire_scratch ();
    اگر (unlikely (!scratch))
    {
      ot_font->h.release_advance_cache (advance_cache);
      جاؤ fallback;
    }
    OT::hb_scalar_cache_t *gvar_cache = ot_font->draw.acquire_gvar_cache (gvar);

    برائے (غیرعلامتی صحیح i = 0; i < گنتی; i++)
    {
      hb_position_t v;
      غیرعلامتی cv;
      اگر (advance_cache->get (*پہلا_glyph, &cv))
	v = cv;
      ورنہ
      {
        v = glyf.get_advance_with_var_unscaled (*پہلا_glyph, font, جھوٹ, *scratch, gvar_cache);
	advance_cache->سیٹ (*پہلا_glyph, v);
      }
      *پہلا_advance = font->em_scale_x (v);
      پہلا_glyph = &Struct_At_انحراف_Unaligned<hb_codepoint_t> (پہلا_glyph, glyph_stride);
      پہلا_advance = &Struct_At_انحراف_Unaligned<hb_position_t> (پہلا_advance, advance_stride);
    }

    ot_font->draw.release_gvar_cache (gvar_cache);
    glyf.release_scratch (scratch);
    ot_font->h.release_advance_cache (advance_cache);
    واپس;
  }

  ot_font->h.release_advance_cache (advance_cache);
  // No HVAR or GVAR.  Just use the fallback non-variable path.
  جاؤ fallback;
#اگر_ختم
}

#اگر_غیر_تعریف_شدہ HB_NO_VERTICAL
ساکن خالی
hb_ot_get_glyph_v_advances (hb_font_t* font, خالی* font_ڈیٹا,
			    غیرعلامتی گنتی,
			    ثابت hb_codepoint_t *پہلا_glyph,
			    غیرعلامتی glyph_stride,
			    hb_position_t *پہلا_advance,
			    غیرعلامتی advance_stride,
			    خالی *user_ڈیٹا HB_غیر_مستعمل)
{
  // Duplicated from h_advances. Ugly. Keep in sync'ish.

  ثابت hb_ot_font_t *ot_font = (ثابت hb_ot_font_t *) font_ڈیٹا;
  ثابت hb_ot_face_t *ot_face = ot_font->ot_face;
  ثابت OT::vmtx_accelerator_t &vmtx = *ot_face->vmtx;

  اگر (unlikely (!vmtx.has_ڈیٹا ()))
  {
    hb_font_extents_t font_extents;
    font->get_h_extents_with_fallback (&font_extents);
    hb_position_t advance = font_extents.descender - font_extents.ascender;
    برائے (غیرعلامتی صحیح i = 0; i < گنتی; i++)
    {
      *پہلا_advance = advance;
      پہلا_advance = &Struct_At_انحراف_Unaligned<hb_position_t> (پہلا_advance, advance_stride);
    }
    واپس;
  }

#اگر_غیر_تعریف_شدہ HB_NO_VAR
  اگر (!font->has_nonzero_coords)
  {
  fallback:
#ورنہ
  {
#اگر_ختم
    // Just plain vtmx data. No need to cache.
    برائے (غیرعلامتی صحیح i = 0; i < گنتی; i++)
    {
      *پہلا_advance = font->em_scale_y (- (صحیح) vmtx.get_advance_without_var_unscaled (*پہلا_glyph));
      پہلا_glyph = &Struct_At_انحراف_Unaligned<hb_codepoint_t> (پہلا_glyph, glyph_stride);
      پہلا_advance = &Struct_At_انحراف_Unaligned<hb_position_t> (پہلا_advance, advance_stride);
    }
    واپس;
  }

#اگر_غیر_تعریف_شدہ HB_NO_VAR
  /* has_nonzero_coords. */

  ot_font->check_serial (font);
  hb_ot_font_advance_cache_t *advance_cache = ot_font->v.acquire_advance_cache ();
  اگر (!advance_cache)
  {
    // malloc failure. Just use the fallback non-variable path.
    جاؤ fallback;
  }

  /* If VVAR is present, use it.*/
  ثابت OT::VVAR &VVAR = *vmtx.var_جدول;
  اگر (VVAR.has_ڈیٹا ())
  {
    ثابت OT::شے_Variation_Store &varStore = &VVAR + VVAR.varStore;
    OT::hb_scalar_cache_t *varStore_cache = ot_font->v.acquire_varStore_cache (varStore);

    برائے (غیرعلامتی صحیح i = 0; i < گنتی; i++)
    {
      hb_position_t v;
      غیرعلامتی cv;
      اگر (advance_cache->get (*پہلا_glyph, &cv))
	v = cv;
      ورنہ
      {
        v = vmtx.get_advance_with_var_unscaled (*پہلا_glyph, font, varStore_cache);
	advance_cache->سیٹ (*پہلا_glyph, v);
      }
      *پہلا_advance = font->em_scale_y (- (صحیح) v);
      پہلا_glyph = &Struct_At_انحراف_Unaligned<hb_codepoint_t> (پہلا_glyph, glyph_stride);
      پہلا_advance = &Struct_At_انحراف_Unaligned<hb_position_t> (پہلا_advance, advance_stride);
    }

    ot_font->v.release_varStore_cache (varStore_cache);
    ot_font->v.release_advance_cache (advance_cache);
    واپس;
  }

  ثابت خودکار &gvar = *ot_face->gvar;
  اگر (gvar.has_ڈیٹا ())
  {
    ثابت خودکار &glyf = *ot_face->glyf;
    خودکار *scratch = glyf.acquire_scratch ();
    اگر (unlikely (!scratch))
    {
      ot_font->v.release_advance_cache (advance_cache);
      جاؤ fallback;
    }
    OT::hb_scalar_cache_t *gvar_cache = ot_font->draw.acquire_gvar_cache (gvar);

    برائے (غیرعلامتی صحیح i = 0; i < گنتی; i++)
    {
      hb_position_t v;
      غیرعلامتی cv;
      اگر (advance_cache->get (*پہلا_glyph, &cv))
	v = cv;
      ورنہ
      {
        v = glyf.get_advance_with_var_unscaled (*پہلا_glyph, font, سچ, *scratch, gvar_cache);
	advance_cache->سیٹ (*پہلا_glyph, v);
      }
      *پہلا_advance = font->em_scale_y (- (صحیح) v);
      پہلا_glyph = &Struct_At_انحراف_Unaligned<hb_codepoint_t> (پہلا_glyph, glyph_stride);
      پہلا_advance = &Struct_At_انحراف_Unaligned<hb_position_t> (پہلا_advance, advance_stride);
    }

    ot_font->draw.release_gvar_cache (gvar_cache);
    glyf.release_scratch (scratch);
    ot_font->v.release_advance_cache (advance_cache);
    واپس;
  }

  ot_font->v.release_advance_cache (advance_cache);
  // No VVAR or GVAR.  Just use the fallback non-variable path.
  جاؤ fallback;
#اگر_ختم
}
#اگر_ختم

#اگر_غیر_تعریف_شدہ HB_NO_VERTICAL
HB_HOT
ساکن hb_bool_t
hb_ot_get_glyph_v_origins (hb_font_t *font,
			   خالی *font_ڈیٹا,
			   غیرعلامتی صحیح گنتی,
			   ثابت hb_codepoint_t *پہلا_glyph,
			   غیرعلامتی glyph_stride,
			   hb_position_t *پہلا_x,
			   غیرعلامتی x_stride,
			   hb_position_t *پہلا_y,
			   غیرعلامتی y_stride,
			   خالی *user_ڈیٹا HB_غیر_مستعمل)
{
  ثابت hb_ot_font_t *ot_font = (ثابت hb_ot_font_t *) font_ڈیٹا;
  ثابت hb_ot_face_t *ot_face = ot_font->ot_face;

  ot_font->check_serial (font);

  /* First, set all the x values to half the advance width. */
  font->get_glyph_h_advances (گنتی,
			      پہلا_glyph, glyph_stride,
			      پہلا_x, x_stride);
  برائے (غیرعلامتی i = 0; i < گنتی; i++)
  {
    *پہلا_x /= 2;
    پہلا_x = &Struct_At_انحراف_Unaligned<hb_position_t> (پہلا_x, x_stride);
  }

  /* The vertical origin business is messy...
   *
   * We allocate the cache, then have various code paths that use the cache.
   * Each one is responsible to free it before returning.
   */
  hb_ot_font_origin_cache_t *origin_cache = ot_font->v_origin.acquire_origin_cache ();

  /* If there is VORG, always use it. It uses VVAR for variations if necessary. */
  ثابت OT::VORG &VORG = *ot_face->VORG;
  اگر (origin_cache && VORG.has_ڈیٹا ())
  {
#اگر_غیر_تعریف_شدہ HB_NO_VAR
    اگر (!font->has_nonzero_coords)
#اگر_ختم
    {
      برائے (غیرعلامتی i = 0; i < گنتی; i++)
      {
	hb_position_t origin;
	غیرعلامتی cv;
	اگر (origin_cache->get (*پہلا_glyph, &cv))
	  origin = font->y_scale < 0 ? -static_cast<hb_position_t>(cv) : static_cast<hb_position_t>(cv);
	ورنہ
	{
	  origin = font->em_scalef_y (VORG.get_y_origin (*پہلا_glyph));
	  origin_cache->سیٹ (*پہلا_glyph, font->y_scale < 0 ? -origin : origin);
	}

	*پہلا_y = origin;

	پہلا_glyph = &Struct_At_انحراف_Unaligned<hb_codepoint_t> (پہلا_glyph, glyph_stride);
	پہلا_y = &Struct_At_انحراف_Unaligned<hb_position_t> (پہلا_y, y_stride);
      }
    }
#اگر_غیر_تعریف_شدہ HB_NO_VAR
    ورنہ
    {
      ثابت OT::VVAR &VVAR = *ot_face->vmtx->var_جدول;
      ثابت خودکار &varStore = &VVAR + VVAR.varStore;
      خودکار *varStore_cache = ot_font->v_origin.acquire_varStore_cache (varStore);
      برائے (غیرعلامتی i = 0; i < گنتی; i++)
      {
	hb_position_t origin;
	غیرعلامتی cv;
	اگر (origin_cache->get (*پہلا_glyph, &cv))
	  origin = font->y_scale < 0 ? -static_cast<hb_position_t>(cv) : static_cast<hb_position_t>(cv);
	ورنہ
	{
	  origin = font->em_scalef_y (VORG.get_y_origin (*پہلا_glyph) +
				      VVAR.get_vorg_delta_unscaled (*پہلا_glyph,
								    font->coords, font->عدد_coords,
								    varStore_cache));
	  origin_cache->سیٹ (*پہلا_glyph, font->y_scale < 0 ? -origin : origin);
	}

	*پہلا_y = origin;

	پہلا_glyph = &Struct_At_انحراف_Unaligned<hb_codepoint_t> (پہلا_glyph, glyph_stride);
	پہلا_y = &Struct_At_انحراف_Unaligned<hb_position_t> (پہلا_y, y_stride);
      }
      ot_font->v_origin.release_varStore_cache (varStore_cache);
    }
#اگر_ختم
    ot_font->v_origin.release_origin_cache (origin_cache);
    واپس سچ;
  }

  /* If and only if `vmtx` is present and it's a `glyf` font,
   * we use the top phantom point, deduced from vmtx,glyf[,gvar]. */
  ثابت خودکار &vmtx = *ot_face->vmtx;
  ثابت خودکار &glyf = *ot_face->glyf;
  اگر (origin_cache && vmtx.has_ڈیٹا() && glyf.has_ڈیٹا ())
  {
    خودکار *scratch = glyf.acquire_scratch ();
    اگر (unlikely (!scratch))
    {
      ot_font->v_origin.release_origin_cache (origin_cache);
      واپس جھوٹ;
    }
    OT::hb_scalar_cache_t *gvar_cache = خالی_اشاریہ;
#اگر_غیر_تعریف_شدہ HB_NO_VAR
    gvar_cache = font->has_nonzero_coords ?
					ot_font->draw.acquire_gvar_cache (*ot_face->gvar) :
					خالی_اشاریہ;
#اگر_ختم

    برائے (غیرعلامتی i = 0; i < گنتی; i++)
    {
      hb_position_t origin;
      غیرعلامتی cv;
      اگر (origin_cache->get (*پہلا_glyph, &cv))
	origin = font->y_scale < 0 ? -static_cast<hb_position_t>(cv) : static_cast<hb_position_t>(cv);
      ورنہ
      {
	origin = font->em_scalef_y (glyf.get_v_origin_with_var_unscaled (*پہلا_glyph, font, *scratch, gvar_cache));
	origin_cache->سیٹ (*پہلا_glyph, font->y_scale < 0 ? -origin : origin);
      }

      *پہلا_y = origin;

      پہلا_glyph = &Struct_At_انحراف_Unaligned<hb_codepoint_t> (پہلا_glyph, glyph_stride);
      پہلا_y = &Struct_At_انحراف_Unaligned<hb_position_t> (پہلا_y, y_stride);
    }

#اگر_غیر_تعریف_شدہ HB_NO_VAR
    اگر (gvar_cache)
      ot_font->draw.release_gvar_cache (gvar_cache);
#اگر_ختم
    glyf.release_scratch (scratch);
    ot_font->v_origin.release_origin_cache (origin_cache);
    واپس سچ;
  }

  /* Otherwise, use glyph extents to center the glyph vertically.
   * If getting glyph extents failed, just use the font ascender. */
  اگر (origin_cache && font->has_glyph_extents_func ())
  {
    hb_font_extents_t font_extents;
    font->get_h_extents_with_fallback (&font_extents);
    hb_position_t font_advance = font_extents.ascender - font_extents.descender;

    برائے (غیرعلامتی i = 0; i < گنتی; i++)
    {
      hb_position_t origin;
      غیرعلامتی cv;

      اگر (origin_cache->get (*پہلا_glyph, &cv))
	origin = font->y_scale < 0 ? -static_cast<hb_position_t>(cv) : static_cast<hb_position_t>(cv);
      ورنہ
      {
	hb_glyph_extents_t extents = {0};
	اگر (likely (font->get_glyph_extents (*پہلا_glyph, &extents)))
	  origin = hb_clamp_مقصد<hb_position_t> ((int64_t) extents.y_bearing +
					 (((int64_t) font_advance + extents.اونچائی) >> 1));
	ورنہ
	  origin = font_extents.ascender;

	origin_cache->سیٹ (*پہلا_glyph, font->y_scale < 0 ? -origin : origin);
      }

      *پہلا_y = origin;

      پہلا_glyph = &Struct_At_انحراف_Unaligned<hb_codepoint_t> (پہلا_glyph, glyph_stride);
      پہلا_y = &Struct_At_انحراف_Unaligned<hb_position_t> (پہلا_y, y_stride);
    }
  }

  ot_font->v_origin.release_origin_cache (origin_cache);
  واپس سچ;
}
#اگر_ختم

ساکن hb_bool_t
hb_ot_get_glyph_extents (hb_font_t *font,
			 خالی *font_ڈیٹا,
			 hb_codepoint_t glyph,
			 hb_glyph_extents_t *extents,
			 خالی *user_ڈیٹا HB_غیر_مستعمل)
{
  ثابت hb_ot_font_t *ot_font = (ثابت hb_ot_font_t *) font_ڈیٹا;
  ثابت hb_ot_face_t *ot_face = ot_font->ot_face;

#اگر !defined(HB_NO_OT_FONT_BITMAP) && !defined(HB_NO_COLOR)
  اگر (ot_face->sbix->get_extents (font, glyph, extents)) واپس سچ;
  اگر (ot_face->CBDT->get_extents (font, glyph, extents)) واپس سچ;
#اگر_ختم
#اگر !defined(HB_NO_COLOR) && !defined(HB_NO_PAINT)
  اگر (ot_face->COLR->get_extents (font, glyph, extents)) واپس سچ;
#اگر_ختم
#اگر_غیر_تعریف_شدہ HB_NO_VAR_COMPOSITES
  اگر (ot_face->VARC->get_extents (font, glyph, extents)) واپس سچ;
#اگر_ختم
  اگر (ot_face->glyf->get_extents (font, glyph, extents)) واپس سچ;
#اگر_غیر_تعریف_شدہ HB_NO_OT_FONT_CFF
  اگر (ot_face->cff2->get_extents (font, glyph, extents)) واپس سچ;
  اگر (ot_face->cff1->get_extents (font, glyph, extents)) واپس سچ;
#اگر_ختم

  واپس جھوٹ;
}

#اگر_غیر_تعریف_شدہ HB_NO_OT_FONT_GLYPH_نام
ساکن hb_bool_t
hb_ot_get_glyph_name (hb_font_t *font HB_غیر_مستعمل,
		      خالی *font_ڈیٹا,
		      hb_codepoint_t glyph,
		      حرف *name, غیرعلامتی صحیح سائز,
		      خالی *user_ڈیٹا HB_غیر_مستعمل)
{
  ثابت hb_ot_font_t *ot_font = (ثابت hb_ot_font_t *) font_ڈیٹا;
  ثابت hb_ot_face_t *ot_face = ot_font->ot_face;

  اگر (ot_face->post->get_glyph_name (glyph, name, سائز)) واپس سچ;
#اگر_غیر_تعریف_شدہ HB_NO_OT_FONT_CFF
  اگر (ot_face->cff1->get_glyph_name (glyph, name, سائز)) واپس سچ;
#اگر_ختم
  واپس جھوٹ;
}
ساکن hb_bool_t
hb_ot_get_glyph_from_name (hb_font_t *font HB_غیر_مستعمل,
			   خالی *font_ڈیٹا,
			   ثابت حرف *name, صحیح len,
			   hb_codepoint_t *glyph,
			   خالی *user_ڈیٹا HB_غیر_مستعمل)
{
  ثابت hb_ot_font_t *ot_font = (ثابت hb_ot_font_t *) font_ڈیٹا;
  ثابت hb_ot_face_t *ot_face = ot_font->ot_face;

  اگر (ot_face->post->get_glyph_from_name (name, len, glyph)) واپس سچ;
#اگر_غیر_تعریف_شدہ HB_NO_OT_FONT_CFF
    اگر (ot_face->cff1->get_glyph_from_name (name, len, glyph)) واپس سچ;
#اگر_ختم
  واپس جھوٹ;
}
#اگر_ختم

ساکن hb_bool_t
hb_ot_get_font_h_extents (hb_font_t *font,
			  خالی *font_ڈیٹا HB_غیر_مستعمل,
			  hb_font_extents_t *metrics,
			  خالی *user_ڈیٹا HB_غیر_مستعمل)
{
  واپس _hb_ot_metrics_get_position_common (font, HB_OT_METRICS_لیبل_HORIZONTAL_ASCENDER, &metrics->ascender) &&
	 _hb_ot_metrics_get_position_common (font, HB_OT_METRICS_لیبل_HORIZONTAL_DESCENDER, &metrics->descender) &&
	 _hb_ot_metrics_get_position_common (font, HB_OT_METRICS_لیبل_HORIZONTAL_سطر_GAP, &metrics->سطر_gap);
}

#اگر_غیر_تعریف_شدہ HB_NO_VERTICAL
ساکن hb_bool_t
hb_ot_get_font_v_extents (hb_font_t *font,
			  خالی *font_ڈیٹا HB_غیر_مستعمل,
			  hb_font_extents_t *metrics,
			  خالی *user_ڈیٹا HB_غیر_مستعمل)
{
  واپس _hb_ot_metrics_get_position_common (font, HB_OT_METRICS_لیبل_VERTICAL_ASCENDER, &metrics->ascender) &&
	 _hb_ot_metrics_get_position_common (font, HB_OT_METRICS_لیبل_VERTICAL_DESCENDER, &metrics->descender) &&
	 _hb_ot_metrics_get_position_common (font, HB_OT_METRICS_لیبل_VERTICAL_سطر_GAP, &metrics->سطر_gap);
}
#اگر_ختم

#اگر_غیر_تعریف_شدہ HB_NO_DRAW
ساکن hb_bool_t
hb_ot_draw_glyph_or_fail (hb_font_t *font,
			  خالی *font_ڈیٹا HB_غیر_مستعمل,
			  hb_codepoint_t glyph,
			  hb_draw_funcs_t *draw_funcs, خالی *draw_ڈیٹا,
			  خالی *user_ڈیٹا)
{
  ثابت hb_ot_font_t *ot_font = (ثابت hb_ot_font_t *) font_ڈیٹا;
  hb_draw_session_t draw_session {draw_funcs, draw_ڈیٹا};

  OT::hb_scalar_cache_t *gvar_cache = خالی_اشاریہ;
#اگر_غیر_تعریف_شدہ HB_NO_VAR
  اگر (font->عدد_coords)
  {
    ot_font->check_serial (font);
    gvar_cache = ot_font->draw.acquire_gvar_cache (*ot_font->ot_face->gvar);
  }
#اگر_ختم
  HB_SCOPE_GUARD (ot_font->draw.release_gvar_cache (gvar_cache));

#اگر_غیر_تعریف_شدہ HB_NO_VAR_COMPOSITES
  اگر (font->face->جدول.VARC->get_راستہ (font, glyph, draw_session)) واپس سچ;
#اگر_ختم
  // Keep the following in synch with VARC::get_path_at()
  اگر (font->face->جدول.glyf->get_راستہ (font, glyph, draw_session, gvar_cache,
				       &draw_session.get_budget ())) واپس سچ;

#اگر_غیر_تعریف_شدہ HB_NO_CFF
  اگر (font->face->جدول.cff2->get_راستہ (font, glyph, draw_session,
				       &draw_session.get_budget ())) واپس سچ;
  اگر (font->face->جدول.cff1->get_راستہ (font, glyph, draw_session,
				       &draw_session.get_budget ())) واپس سچ;
#اگر_ختم

  واپس جھوٹ;
}
#اگر_ختم

#اگر_غیر_تعریف_شدہ HB_NO_PAINT
ساکن hb_bool_t
hb_ot_paint_glyph_or_fail (hb_font_t *font,
			   خالی *font_ڈیٹا,
			   hb_codepoint_t glyph,
			   hb_paint_funcs_t *paint_funcs, خالی *paint_ڈیٹا,
			   غیرعلامتی صحیح palette,
			   hb_color_t foreground,
			   خالی *user_ڈیٹا)
{
#اگر_غیر_تعریف_شدہ HB_NO_COLOR
  اگر (font->face->جدول.COLR->paint_glyph (font, glyph, paint_funcs, paint_ڈیٹا, palette, foreground)) واپس سچ;
#اگر_غیر_تعریف_شدہ HB_NO_SVG
  اگر (font->face->جدول.SVG->paint_glyph (font, glyph, paint_funcs, paint_ڈیٹا)) واپس سچ;
#اگر_ختم
#اگر_غیر_تعریف_شدہ HB_NO_OT_FONT_BITMAP
  اگر (font->face->جدول.CBDT->paint_glyph (font, glyph, paint_funcs, paint_ڈیٹا)) واپس سچ;
  اگر (font->face->جدول.sbix->paint_glyph (font, glyph, paint_funcs, paint_ڈیٹا)) واپس سچ;
#اگر_ختم
#اگر_ختم
  واپس جھوٹ;
}
#اگر_ختم

ساکن اندرونی خالی free_static_ot_funcs ();

ساکن ساخت hb_ot_font_funcs_lazy_loader_t : hb_font_funcs_lazy_loader_t<hb_ot_font_funcs_lazy_loader_t>
{
  ساکن hb_font_funcs_t *create ()
  {
    hb_font_funcs_t *funcs = hb_font_funcs_create ();

    hb_font_funcs_سیٹ_nominal_glyph_func (funcs, hb_ot_get_nominal_glyph, خالی_اشاریہ, خالی_اشاریہ);
    hb_font_funcs_سیٹ_nominal_glyphs_func (funcs, hb_ot_get_nominal_glyphs, خالی_اشاریہ, خالی_اشاریہ);
    hb_font_funcs_سیٹ_variation_glyph_func (funcs, hb_ot_get_variation_glyph, خالی_اشاریہ, خالی_اشاریہ);

    hb_font_funcs_سیٹ_font_h_extents_func (funcs, hb_ot_get_font_h_extents, خالی_اشاریہ, خالی_اشاریہ);
    hb_font_funcs_سیٹ_glyph_h_advances_func (funcs, hb_ot_get_glyph_h_advances, خالی_اشاریہ, خالی_اشاریہ);

#اگر_غیر_تعریف_شدہ HB_NO_VERTICAL
    hb_font_funcs_سیٹ_font_v_extents_func (funcs, hb_ot_get_font_v_extents, خالی_اشاریہ, خالی_اشاریہ);
    hb_font_funcs_سیٹ_glyph_v_advances_func (funcs, hb_ot_get_glyph_v_advances, خالی_اشاریہ, خالی_اشاریہ);
    hb_font_funcs_سیٹ_glyph_v_origins_func (funcs, hb_ot_get_glyph_v_origins, خالی_اشاریہ, خالی_اشاریہ);
#اگر_ختم

#اگر_غیر_تعریف_شدہ HB_NO_DRAW
    hb_font_funcs_سیٹ_draw_glyph_or_fail_func (funcs, hb_ot_draw_glyph_or_fail, خالی_اشاریہ, خالی_اشاریہ);
#اگر_ختم

#اگر_غیر_تعریف_شدہ HB_NO_PAINT
    hb_font_funcs_سیٹ_paint_glyph_or_fail_func (funcs, hb_ot_paint_glyph_or_fail, خالی_اشاریہ, خالی_اشاریہ);
#اگر_ختم

    hb_font_funcs_سیٹ_glyph_extents_func (funcs, hb_ot_get_glyph_extents, خالی_اشاریہ, خالی_اشاریہ);
    //hb_font_funcs_set_glyph_contour_point_func (funcs, hb_ot_get_glyph_contour_point, nullptr, nullptr);

#اگر_غیر_تعریف_شدہ HB_NO_OT_FONT_GLYPH_نام
    hb_font_funcs_سیٹ_glyph_name_func (funcs, hb_ot_get_glyph_name, خالی_اشاریہ, خالی_اشاریہ);
    hb_font_funcs_سیٹ_glyph_from_name_func (funcs, hb_ot_get_glyph_from_name, خالی_اشاریہ, خالی_اشاریہ);
#اگر_ختم

    hb_font_funcs_make_immutable (funcs);

    hb_atexit (free_static_ot_funcs);

    واپس funcs;
  }
} static_ot_funcs;

ساکن اندرونی
خالی free_static_ot_funcs ()
{
  static_ot_funcs.free_instance ();
}

ساکن hb_font_funcs_t *
_hb_ot_get_font_funcs ()
{
  واپس static_ot_funcs.get_unconst ();
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
خالی
hb_ot_font_سیٹ_funcs (hb_font_t *font)
{
  hb_ot_font_t *ot_font = _hb_ot_font_create (font);
  اگر (unlikely (!ot_font))
    واپس;

  hb_font_سیٹ_funcs (font,
		     _hb_ot_get_font_funcs (),
		     ot_font,
		     _hb_ot_font_destroy);
}

#اگر_ختم
