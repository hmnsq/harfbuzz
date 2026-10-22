/*
 * Copyright © 2026  Behdad Esfahbod
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
 * Author(s): Behdad Esfahbod
 */

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_RASTER_PAINT_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_RASTER_PAINT_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-raster-image.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-geometry.hh"

/* hb_raster_clip_t — alpha mask for clipping */
ਬਣਤਰ hb_raster_clip_t
{
  hb_ਸਦਿਸ਼_t<uint8_t> alpha;	/* A8 mask, same extents as root surface */
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਚੌੜਾਈ  = 0;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਉਚਾਈ = 0;
  ਬਿਨਾਨਿਸ਼ਾਨੀ stride = 0;

  /* Fast path: simple rectangle (no alpha buffer needed) */
  ਤਾਰਕਿਕ is_rect = ਸੱਚ;
  ਪੂਰਨ rect_x0 = 0, rect_y0 = 0;
  ਪੂਰਨ rect_x1 = 0, rect_y1 = 0;

  /* Bounding box of non-zero alpha region (valid for both rect and mask) */
  ਬਿਨਾਨਿਸ਼ਾਨੀ min_x = 0, min_y = 0;
  ਬਿਨਾਨਿਸ਼ਾਨੀ max_x = 0, max_y = 0;

  ਖਾਲੀ init_ਭਰਿਆ (ਬਿਨਾਨਿਸ਼ਾਨੀ w, ਬਿਨਾਨਿਸ਼ਾਨੀ h)
  {
    ਚੌੜਾਈ = w;
    ਉਚਾਈ = h;
    stride = (w + 3u) & ~3u;
    is_rect = ਸੱਚ;
    rect_x0 = 0;
    rect_y0 = 0;
    rect_x1 = (ਪੂਰਨ) w;
    rect_y1 = (ਪੂਰਨ) h;
    min_x = 0;
    min_y = 0;
    max_x = w;
    max_y = h;
  }

  ਖਾਲੀ update_bounds_from_rect ()
  {
    min_x = (ਬਿਨਾਨਿਸ਼ਾਨੀ) hb_max (rect_x0, 0);
    min_y = (ਬਿਨਾਨਿਸ਼ਾਨੀ) hb_max (rect_y0, 0);
    max_x = (ਬਿਨਾਨਿਸ਼ਾਨੀ) hb_max (hb_min (rect_x1, (ਪੂਰਨ) ਚੌੜਾਈ), 0);
    max_y = (ਬਿਨਾਨਿਸ਼ਾਨੀ) hb_max (hb_min (rect_y1, (ਪੂਰਨ) ਉਚਾਈ), 0);
  }

  uint8_t get_alpha (ਬਿਨਾਨਿਸ਼ਾਨੀ x, ਬਿਨਾਨਿਸ਼ਾਨੀ y) ਅਚਲ
  {
    ਜੇ (is_rect)
      ਵਾਪਸ ((ਪੂਰਨ) x >= rect_x0 && (ਪੂਰਨ) x < rect_x1 &&
	      (ਪੂਰਨ) y >= rect_y0 && (ਪੂਰਨ) y < rect_y1) ? 255 : 0;
    ਜੇ (x >= ਚੌੜਾਈ || y >= ਉਚਾਈ) ਵਾਪਸ 0;
    ਜੇ (stride < ਚੌੜਾਈ) ਵਾਪਸ 0;
    ਜੇ (ਉਚਾਈ && stride > (size_t) -1 / ਉਚਾਈ) ਵਾਪਸ 0;
    size_t idx = (size_t) y * stride + x;
    ਜੇ (!alpha.arrayZ || idx >= alpha.ਲੰਮਾਈ) ਵਾਪਸ 0;
    ਵਾਪਸ alpha[idx];
  }

  ਤਾਰਕਿਕ has_valid_alpha_ਨਕਾਬ () ਅਚਲ
  {
    ਜੇ (is_rect)
      ਵਾਪਸ ਸੱਚ;
    ਜੇ (!alpha.arrayZ || stride < ਚੌੜਾਈ)
      ਵਾਪਸ ਝੂਠ;
    ਜੇ (ਉਚਾਈ && stride > (size_t) -1 / ਉਚਾਈ)
      ਵਾਪਸ ਝੂਠ;
    ਵਾਪਸ (size_t) stride * ਉਚਾਈ <= alpha.ਲੰਮਾਈ;
  }
};


/* hb_raster_paint_t — color glyph paint context */
ਬਣਤਰ hb_raster_paint_t
{
  hb_object_ਸਿਰਲੇਖ_t ਸਿਰਲੇਖ;

  /* Configuration */
  hb_transform_t<>    ਅਧਾਰ_transform     = {1, 0, 0, 1, 0, 0};
  ਦਸ਼ਮਲਵ               x_scale_ਗੁਣਕ     = 1.f;
  ਦਸ਼ਮਲਵ               y_scale_ਗੁਣਕ     = 1.f;
  hb_raster_extents_t fixed_extents      = {};
  ਤਾਰਕਿਕ                has_extents  = ਝੂਠ;
  hb_color_t          foreground         = HB_COLOR (0, 0, 0, 255);
  hb_color_t          background         = HB_COLOR (0, 0, 0, 0);
  ਬਿਨਾਨਿਸ਼ਾਨੀ            palette            = 0;
  hb_map_t           *custom_palette     = ਖਾਲੀ_ਸੂਚਕ;

  /* Stacks */
  hb_ਸਦਿਸ਼_t<hb_transform_t<>>     transform_ਢੇਰ;
  hb_ਸਦਿਸ਼_t<hb_raster_clip_t>     clip_ਢੇਰ;
  hb_ਸਦਿਸ਼_t<hb_raster_clip_t>     clip_cache;
  hb_ਸਦਿਸ਼_t<hb_raster_image_t *>  surface_ਢੇਰ;

  /* Cached surface pool (freelist for reuse across push/pop group) */
  hb_ਸਦਿਸ਼_t<hb_raster_image_t *>  surface_cache;
  hb_ਸਦਿਸ਼_t<hb_color_stop_t>      scratch_color_stops;

  /* Internal rasterizer for clip-to-glyph */
  hb_raster_draw_t *clip_rdr = ਖਾਲੀ_ਸੂਚਕ;

  /* Two work budgets for the current paint session, both reset by
   * hb_raster_paint_clear(), so per-node costs cannot multiply with the
   * paint-graph traversal limits of the font tables driving us (e.g.
   * COLR).  They bound different resources and must not share a counter:
   *
   *  - The outline budget (policy + live) bounds glyph outline traversal
   *    and curve flattening.  It is the public draw/paint budget, seeded
   *    into the clip rasterizer through the public draw-budget API and
   *    read back after each glyph.  Default HB_BUDGET_GLYPH.
   *
   *  - The pixel budget bounds scanline/fill work, which scales with
   *    surface area rather than outline complexity.  It is sized from the
   *    surface once known and is internal (not exposed through the API);
   *    an unlimited outline policy disables it too. */
  int64_t budget = HB_BUDGET_DEFAULT;
  int64_t budget_ਬਚਿਆ = HB_BUDGET_GLYPH;
  int64_t pixel_ਬਚਿਆ = HB_BUDGET_RASTER_PIXELS;

  /* Helpers */

  int64_t get_default_pixel_budget () ਅਚਲ
  {
    ਵਾਪਸ hb_max ((int64_t) HB_BUDGET_RASTER_PIXELS,
		   (int64_t) HB_BUDGET_RASTER_PAINT_PASSES *
		   fixed_extents.ਚੌੜਾਈ * fixed_extents.ਉਚਾਈ);
  }

  ਖਾਲੀ recharge_budget ()
  {
    budget_ਬਚਿਆ = budget == HB_BUDGET_DEFAULT ?
		       (int64_t) HB_BUDGET_GLYPH : budget;
    pixel_ਬਚਿਆ = budget == HB_BUDGET_UNLIMITED ? budget :
		      (surface_ਢੇਰ.ਲੰਮਾਈ ? get_default_pixel_budget () :
					      (int64_t) HB_BUDGET_RASTER_PIXELS);
  }

  /* Returns whether the pixel operation should proceed.  Allows a single
   * overshoot past zero; callers skip work once the budget is spent. */
  ਤਾਰਕਿਕ charge_work (int64_t work)
  {
    ਜੇ (unlikely (pixel_ਬਚਿਆ < 0)) ਵਾਪਸ ਝੂਠ;
    pixel_ਬਚਿਆ -= work;
    ਵਾਪਸ ਸੱਚ;
  }

  /* Reject work that would exceed the remaining budget before it starts. */
  ਤਾਰਕਿਕ precharge_work (int64_t work)
  {
    ਜੇ (unlikely (pixel_ਬਚਿਆ < work))
    {
      pixel_ਬਚਿਆ = -1;
      ਵਾਪਸ ਝੂਠ;
    }
    pixel_ਬਚਿਆ -= work;
    ਵਾਪਸ ਸੱਚ;
  }

  hb_raster_image_t *acquire_surface ()
  {
    hb_raster_image_t *img;
    ਜੇ (surface_cache.ਲੰਮਾਈ)
      img = surface_cache.ਕੱਢੋ ();
    ਨਹੀਂਤਾਂ
    {
      img = hb_raster_image_create_or_fail ();
      ਜੇ (unlikely (!img)) ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
    }

    ਜੇ (unlikely (!img->configure (HB_RASTER_FORMAT_BGRA32, fixed_extents)))
    {
      hb_raster_image_destroy (img);
      ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
    }
    img->clear ();
    ਵਾਪਸ img;
  }

  ਖਾਲੀ release_surface (hb_raster_image_t *img)
  {
    ਜੇ (!surface_cache.ਧੱਕੋ_or_fail (img))
      hb_raster_image_destroy (img);
  }

  hb_raster_clip_t acquire_clip (ਬਿਨਾਨਿਸ਼ਾਨੀ w, ਬਿਨਾਨਿਸ਼ਾਨੀ h)
  {
    hb_raster_clip_t clip;
    ਜੇ (clip_cache.ਲੰਮਾਈ)
      clip = clip_cache.ਕੱਢੋ ();
    clip.ਚੌੜਾਈ = w;
    clip.ਉਚਾਈ = h;
    clip.stride = (w + 3u) & ~3u;
    clip.is_rect = ਝੂਠ;
    clip.rect_x0 = clip.rect_y0 = 0;
    clip.rect_x1 = clip.rect_y1 = 0;
    clip.min_x = clip.min_y = clip.max_x = clip.max_y = 0;
    ਵਾਪਸ clip;
  }

  ਖਾਲੀ release_clip (hb_raster_clip_t &&clip)
  {
    ਜੇ (clip.alpha.arrayZ)
      clip_cache.push (std::move (clip));
  }

  ਖਾਲੀ release_all_clips ()
  {
    ਜਦੋਂ (clip_ਢੇਰ.ਲੰਮਾਈ)
      release_clip (clip_ਢੇਰ.ਕੱਢੋ ());
  }

  hb_raster_image_t *current_surface ()
  {
    ਵਾਪਸ surface_ਢੇਰ.ਲੰਮਾਈ ? surface_ਢੇਰ.ਪੂਛ () : ਖਾਲੀ_ਸੂਚਕ;
  }

  hb_raster_clip_t &current_clip ()
  {
    ਵਾਪਸ clip_ਢੇਰ.ਪੂਛ ();
  }

  hb_transform_t<> current_transform ()
  {
    ਜੇ (unlikely (!transform_ਢੇਰ.ਲੰਮਾਈ))
      ਵਾਪਸ {1, 0, 0, 1, 0, 0};
    ਵਾਪਸ transform_ਢੇਰ.ਪੂਛ ();
  }

  ਖਾਲੀ apply_scale_ਗੁਣਕ (hb_transform_t<> &t) ਅਚਲ
  {
    t.xx /= x_scale_ਗੁਣਕ;
    t.xy /= x_scale_ਗੁਣਕ;
    t.x0 /= x_scale_ਗੁਣਕ;
    t.yx /= y_scale_ਗੁਣਕ;
    t.yy /= y_scale_ਗੁਣਕ;
    t.y0 /= y_scale_ਗੁਣਕ;
  }

  hb_transform_t<> current_effective_transform ()
  {
    hb_transform_t<> t = current_transform ();
    apply_scale_ਗੁਣਕ (t);
    ਵਾਪਸ t;
  }

  ਤਾਰਕਿਕ fetch_color_stops (hb_color_ਸਤਰ_t *color_ਸਤਰ)
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2 = hb_color_ਸਤਰ_get_color_stops (color_ਸਤਰ, 0, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    ਜੇ (unlikely (!ਗਿਣਤੀ_2 || !scratch_color_stops.resize (ਗਿਣਤੀ_2)))
    {
      scratch_color_stops.resize (0);
      ਵਾਪਸ ਝੂਠ;
    }
    hb_color_ਸਤਰ_get_color_stops (color_ਸਤਰ, 0, &ਗਿਣਤੀ_2, scratch_color_stops.arrayZ);
    ਵਾਪਸ ਸੱਚ;
  }
};


#ਜੇ_ਅੰਤ /* HB_RASTER_PAINT_HH */
