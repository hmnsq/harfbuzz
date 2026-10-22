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

#ifndef HB_RASTER_PAINT_HH
#define HB_RASTER_PAINT_HH

#ಅನ್ತರ್ಭಾವ "hb.hh"

#ಅನ್ತರ್ಭಾವ "hb-raster-image.hh"
#ಅನ್ತರ್ಭಾವ "hb-geometry.hh"

/* hb_raster_clip_t — alpha mask for clipping */
ಸಂರಚನಾ hb_raster_clip_t
{
  hb_vector_t<uint8_t> alpha;	/* A8 mask, same extents as root surface */
  unsigned width  = 0;
  unsigned height = 0;
  unsigned stride = 0;

  /* Fast path: simple rectangle (no alpha buffer needed) */
  ತಾರ್ಕಿಕ is_rect = ಸತ್ಯ;
  ಪೂರ್ಣಾಙ್ಕ rect_x0 = 0, rect_y0 = 0;
  ಪೂರ್ಣಾಙ್ಕ rect_x1 = 0, rect_y1 = 0;

  /* Bounding box of non-zero alpha region (valid for both rect and mask) */
  unsigned min_x = 0, min_y = 0;
  unsigned max_x = 0, max_y = 0;

  ಶೂನ್ಯ init_full (unsigned w, unsigned h)
  {
    width = w;
    height = h;
    stride = (w + 3u) & ~3u;
    is_rect = ಸತ್ಯ;
    rect_x0 = 0;
    rect_y0 = 0;
    rect_x1 = (ಪೂರ್ಣಾಙ್ಕ) w;
    rect_y1 = (ಪೂರ್ಣಾಙ್ಕ) h;
    min_x = 0;
    min_y = 0;
    max_x = w;
    max_y = h;
  }

  ಶೂನ್ಯ update_bounds_from_rect ()
  {
    min_x = (unsigned) hb_max (rect_x0, 0);
    min_y = (unsigned) hb_max (rect_y0, 0);
    max_x = (unsigned) hb_max (hb_min (rect_x1, (ಪೂರ್ಣಾಙ್ಕ) width), 0);
    max_y = (unsigned) hb_max (hb_min (rect_y1, (ಪೂರ್ಣಾಙ್ಕ) height), 0);
  }

  uint8_t get_alpha (unsigned x, unsigned y) ಧ್ರುವ
  {
    ಯದಿ (is_rect)
      ಪ್ರತ್ಯಾಗಮ ((ಪೂರ್ಣಾಙ್ಕ) x >= rect_x0 && (ಪೂರ್ಣಾಙ್ಕ) x < rect_x1 &&
	      (ಪೂರ್ಣಾಙ್ಕ) y >= rect_y0 && (ಪೂರ್ಣಾಙ್ಕ) y < rect_y1) ? 255 : 0;
    ಯದಿ (x >= width || y >= height) ಪ್ರತ್ಯಾಗಮ 0;
    ಯದಿ (stride < width) ಪ್ರತ್ಯಾಗಮ 0;
    ಯದಿ (height && stride > (size_t) -1 / height) ಪ್ರತ್ಯಾಗಮ 0;
    size_t idx = (size_t) y * stride + x;
    ಯದಿ (!alpha.arrayZ || idx >= alpha.length) ಪ್ರತ್ಯಾಗಮ 0;
    ಪ್ರತ್ಯಾಗಮ alpha[idx];
  }

  ತಾರ್ಕಿಕ has_valid_alpha_mask () ಧ್ರುವ
  {
    ಯದಿ (is_rect)
      ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    ಯದಿ (!alpha.arrayZ || stride < width)
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    ಯದಿ (height && stride > (size_t) -1 / height)
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    ಪ್ರತ್ಯಾಗಮ (size_t) stride * height <= alpha.length;
  }
};


/* hb_raster_paint_t — color glyph paint context */
ಸಂರಚನಾ hb_raster_paint_t
{
  hb_object_header_t header;

  /* Configuration */
  hb_transform_t<>    base_transform     = {1, 0, 0, 1, 0, 0};
  ಭಿನ್ನ               x_scale_factor     = 1.f;
  ಭಿನ್ನ               y_scale_factor     = 1.f;
  hb_raster_extents_t fixed_extents      = {};
  ತಾರ್ಕಿಕ                has_extents  = ಅಸತ್ಯ;
  hb_color_t          foreground         = HB_COLOR (0, 0, 0, 255);
  hb_color_t          background         = HB_COLOR (0, 0, 0, 0);
  unsigned            palette            = 0;
  hb_map_t           *custom_palette     = nullptr;

  /* Stacks */
  hb_vector_t<hb_transform_t<>>     transform_ರಾಶಿ;
  hb_vector_t<hb_raster_clip_t>     clip_ರಾಶಿ;
  hb_vector_t<hb_raster_clip_t>     clip_cache;
  hb_vector_t<hb_raster_image_t *>  surface_ರಾಶಿ;

  /* Cached surface pool (freelist for reuse across push/pop group) */
  hb_vector_t<hb_raster_image_t *>  surface_cache;
  hb_vector_t<hb_color_stop_t>      scratch_color_stops;

  /* Internal rasterizer for clip-to-glyph */
  hb_raster_draw_t *clip_rdr = nullptr;

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
  int64_t budget_remaining = HB_BUDGET_GLYPH;
  int64_t pixel_remaining = HB_BUDGET_RASTER_PIXELS;

  /* Helpers */

  int64_t get_default_pixel_budget () ಧ್ರುವ
  {
    ಪ್ರತ್ಯಾಗಮ hb_max ((int64_t) HB_BUDGET_RASTER_PIXELS,
		   (int64_t) HB_BUDGET_RASTER_PAINT_PASSES *
		   fixed_extents.width * fixed_extents.height);
  }

  ಶೂನ್ಯ recharge_budget ()
  {
    budget_remaining = budget == HB_BUDGET_DEFAULT ?
		       (int64_t) HB_BUDGET_GLYPH : budget;
    pixel_remaining = budget == HB_BUDGET_UNLIMITED ? budget :
		      (surface_ರಾಶಿ.length ? get_default_pixel_budget () :
					      (int64_t) HB_BUDGET_RASTER_PIXELS);
  }

  /* Returns whether the pixel operation should proceed.  Allows a single
   * overshoot past zero; callers skip work once the budget is spent. */
  ತಾರ್ಕಿಕ charge_work (int64_t work)
  {
    ಯದಿ (unlikely (pixel_remaining < 0)) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    pixel_remaining -= work;
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }

  /* Reject work that would exceed the remaining budget before it starts. */
  ತಾರ್ಕಿಕ precharge_work (int64_t work)
  {
    ಯದಿ (unlikely (pixel_remaining < work))
    {
      pixel_remaining = -1;
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }
    pixel_remaining -= work;
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }

  hb_raster_image_t *acquire_surface ()
  {
    hb_raster_image_t *img;
    ಯದಿ (surface_cache.length)
      img = surface_cache.ತೆಗೆ ();
    ಅನ್ಯಥಾ
    {
      img = hb_raster_image_create_or_fail ();
      ಯದಿ (unlikely (!img)) ಪ್ರತ್ಯಾಗಮ nullptr;
    }

    ಯದಿ (unlikely (!img->configure (HB_RASTER_FORMAT_BGRA32, fixed_extents)))
    {
      hb_raster_image_destroy (img);
      ಪ್ರತ್ಯಾಗಮ nullptr;
    }
    img->clear ();
    ಪ್ರತ್ಯಾಗಮ img;
  }

  ಶೂನ್ಯ release_surface (hb_raster_image_t *img)
  {
    ಯದಿ (!surface_cache.ತಳ್ಳು_or_fail (img))
      hb_raster_image_destroy (img);
  }

  hb_raster_clip_t acquire_clip (unsigned w, unsigned h)
  {
    hb_raster_clip_t clip;
    ಯದಿ (clip_cache.length)
      clip = clip_cache.ತೆಗೆ ();
    clip.width = w;
    clip.height = h;
    clip.stride = (w + 3u) & ~3u;
    clip.is_rect = ಅಸತ್ಯ;
    clip.rect_x0 = clip.rect_y0 = 0;
    clip.rect_x1 = clip.rect_y1 = 0;
    clip.min_x = clip.min_y = clip.max_x = clip.max_y = 0;
    ಪ್ರತ್ಯಾಗಮ clip;
  }

  ಶೂನ್ಯ release_clip (hb_raster_clip_t &&clip)
  {
    ಯದಿ (clip.alpha.arrayZ)
      clip_cache.push (std::move (clip));
  }

  ಶೂನ್ಯ release_all_clips ()
  {
    ಯಾವತ್ (clip_ರಾಶಿ.length)
      release_clip (clip_ರಾಶಿ.ತೆಗೆ ());
  }

  hb_raster_image_t *current_surface ()
  {
    ಪ್ರತ್ಯಾಗಮ surface_ರಾಶಿ.length ? surface_ರಾಶಿ.tail () : nullptr;
  }

  hb_raster_clip_t &current_clip ()
  {
    ಪ್ರತ್ಯಾಗಮ clip_ರಾಶಿ.tail ();
  }

  hb_transform_t<> current_transform ()
  {
    ಯದಿ (unlikely (!transform_ರಾಶಿ.length))
      ಪ್ರತ್ಯಾಗಮ {1, 0, 0, 1, 0, 0};
    ಪ್ರತ್ಯಾಗಮ transform_ರಾಶಿ.tail ();
  }

  ಶೂನ್ಯ apply_scale_factor (hb_transform_t<> &t) ಧ್ರುವ
  {
    t.xx /= x_scale_factor;
    t.xy /= x_scale_factor;
    t.x0 /= x_scale_factor;
    t.yx /= y_scale_factor;
    t.yy /= y_scale_factor;
    t.y0 /= y_scale_factor;
  }

  hb_transform_t<> current_effective_transform ()
  {
    hb_transform_t<> t = current_transform ();
    apply_scale_factor (t);
    ಪ್ರತ್ಯಾಗಮ t;
  }

  ತಾರ್ಕಿಕ fetch_color_stops (hb_color_ಸಾಲು_t *color_ಸಾಲು)
  {
    unsigned ಎಣಿಕೆ = hb_color_ಸಾಲು_get_color_stops (color_ಸಾಲು, 0, nullptr, nullptr);
    ಯದಿ (unlikely (!ಎಣಿಕೆ || !scratch_color_stops.resize (ಎಣಿಕೆ)))
    {
      scratch_color_stops.resize (0);
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }
    hb_color_ಸಾಲು_get_color_stops (color_ಸಾಲು, 0, &ಎಣಿಕೆ, scratch_color_stops.arrayZ);
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }
};


#endif /* HB_RASTER_PAINT_HH */
