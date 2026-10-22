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

#شامل "hb.hh"

#شامل "hb-raster-image.hh"
#شامل "hb-geometry.hh"

/* hb_raster_clip_t — alpha mask for clipping */
ساختار hb_raster_clip_t
{
  hb_vector_t<uint8_t> alpha;	/* A8 mask, same extents as root surface */
  unsigned width  = 0;
  unsigned height = 0;
  unsigned stride = 0;

  /* Fast path: simple rectangle (no alpha buffer needed) */
  منطقی is_rect = درست;
  صحیح rect_x0 = 0, rect_y0 = 0;
  صحیح rect_x1 = 0, rect_y1 = 0;

  /* Bounding box of non-zero alpha region (valid for both rect and mask) */
  unsigned min_x = 0, min_y = 0;
  unsigned max_x = 0, max_y = 0;

  تهی init_full (unsigned w, unsigned h)
  {
    width = w;
    height = h;
    stride = (w + 3u) & ~3u;
    is_rect = درست;
    rect_x0 = 0;
    rect_y0 = 0;
    rect_x1 = (صحیح) w;
    rect_y1 = (صحیح) h;
    min_x = 0;
    min_y = 0;
    max_x = w;
    max_y = h;
  }

  تهی update_bounds_from_rect ()
  {
    min_x = (unsigned) hb_max (rect_x0, 0);
    min_y = (unsigned) hb_max (rect_y0, 0);
    max_x = (unsigned) hb_max (hb_min (rect_x1, (صحیح) width), 0);
    max_y = (unsigned) hb_max (hb_min (rect_y1, (صحیح) height), 0);
  }

  uint8_t get_alpha (unsigned x, unsigned y) ثابت
  {
    اگر (is_rect)
      بازگشت ((صحیح) x >= rect_x0 && (صحیح) x < rect_x1 &&
	      (صحیح) y >= rect_y0 && (صحیح) y < rect_y1) ? 255 : 0;
    اگر (x >= width || y >= height) بازگشت 0;
    اگر (stride < width) بازگشت 0;
    اگر (height && stride > (size_t) -1 / height) بازگشت 0;
    size_t idx = (size_t) y * stride + x;
    اگر (!alpha.arrayZ || idx >= alpha.length) بازگشت 0;
    بازگشت alpha[idx];
  }

  منطقی has_valid_alpha_mask () ثابت
  {
    اگر (is_rect)
      بازگشت درست;
    اگر (!alpha.arrayZ || stride < width)
      بازگشت نادرست;
    اگر (height && stride > (size_t) -1 / height)
      بازگشت نادرست;
    بازگشت (size_t) stride * height <= alpha.length;
  }
};


/* hb_raster_paint_t — color glyph paint context */
ساختار hb_raster_paint_t
{
  hb_object_header_t header;

  /* Configuration */
  hb_transform_t<>    base_transform     = {1, 0, 0, 1, 0, 0};
  اعشاری               x_scale_factor     = 1.f;
  اعشاری               y_scale_factor     = 1.f;
  hb_raster_extents_t fixed_extents      = {};
  منطقی                has_extents  = نادرست;
  hb_color_t          foreground         = HB_COLOR (0, 0, 0, 255);
  hb_color_t          background         = HB_COLOR (0, 0, 0, 0);
  unsigned            palette            = 0;
  hb_map_t           *custom_palette     = nullptr;

  /* Stacks */
  hb_vector_t<hb_transform_t<>>     transform_پشته;
  hb_vector_t<hb_raster_clip_t>     clip_پشته;
  hb_vector_t<hb_raster_clip_t>     clip_cache;
  hb_vector_t<hb_raster_image_t *>  surface_پشته;

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

  int64_t get_default_pixel_budget () ثابت
  {
    بازگشت hb_max ((int64_t) HB_BUDGET_RASTER_PIXELS,
		   (int64_t) HB_BUDGET_RASTER_PAINT_PASSES *
		   fixed_extents.width * fixed_extents.height);
  }

  تهی recharge_budget ()
  {
    budget_remaining = budget == HB_BUDGET_DEFAULT ?
		       (int64_t) HB_BUDGET_GLYPH : budget;
    pixel_remaining = budget == HB_BUDGET_UNLIMITED ? budget :
		      (surface_پشته.length ? get_default_pixel_budget () :
					      (int64_t) HB_BUDGET_RASTER_PIXELS);
  }

  /* Returns whether the pixel operation should proceed.  Allows a single
   * overshoot past zero; callers skip work once the budget is spent. */
  منطقی charge_work (int64_t work)
  {
    اگر (unlikely (pixel_remaining < 0)) بازگشت نادرست;
    pixel_remaining -= work;
    بازگشت درست;
  }

  /* Reject work that would exceed the remaining budget before it starts. */
  منطقی precharge_work (int64_t work)
  {
    اگر (unlikely (pixel_remaining < work))
    {
      pixel_remaining = -1;
      بازگشت نادرست;
    }
    pixel_remaining -= work;
    بازگشت درست;
  }

  hb_raster_image_t *acquire_surface ()
  {
    hb_raster_image_t *img;
    اگر (surface_cache.length)
      img = surface_cache.برداشت ();
    وگرنه
    {
      img = hb_raster_image_create_or_fail ();
      اگر (unlikely (!img)) بازگشت nullptr;
    }

    اگر (unlikely (!img->configure (HB_RASTER_FORMAT_BGRA32, fixed_extents)))
    {
      hb_raster_image_destroy (img);
      بازگشت nullptr;
    }
    img->clear ();
    بازگشت img;
  }

  تهی release_surface (hb_raster_image_t *img)
  {
    اگر (!surface_cache.افزودن_or_fail (img))
      hb_raster_image_destroy (img);
  }

  hb_raster_clip_t acquire_clip (unsigned w, unsigned h)
  {
    hb_raster_clip_t clip;
    اگر (clip_cache.length)
      clip = clip_cache.برداشت ();
    clip.width = w;
    clip.height = h;
    clip.stride = (w + 3u) & ~3u;
    clip.is_rect = نادرست;
    clip.rect_x0 = clip.rect_y0 = 0;
    clip.rect_x1 = clip.rect_y1 = 0;
    clip.min_x = clip.min_y = clip.max_x = clip.max_y = 0;
    بازگشت clip;
  }

  تهی release_clip (hb_raster_clip_t &&clip)
  {
    اگر (clip.alpha.arrayZ)
      clip_cache.push (std::move (clip));
  }

  تهی release_all_clips ()
  {
    وقتی (clip_پشته.length)
      release_clip (clip_پشته.برداشت ());
  }

  hb_raster_image_t *current_surface ()
  {
    بازگشت surface_پشته.length ? surface_پشته.tail () : nullptr;
  }

  hb_raster_clip_t &current_clip ()
  {
    بازگشت clip_پشته.tail ();
  }

  hb_transform_t<> current_transform ()
  {
    اگر (unlikely (!transform_پشته.length))
      بازگشت {1, 0, 0, 1, 0, 0};
    بازگشت transform_پشته.tail ();
  }

  تهی apply_scale_factor (hb_transform_t<> &t) ثابت
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
    بازگشت t;
  }

  منطقی fetch_color_stops (hb_color_سطر_t *color_سطر)
  {
    unsigned شمار = hb_color_سطر_get_color_stops (color_سطر, 0, nullptr, nullptr);
    اگر (unlikely (!شمار || !scratch_color_stops.resize (شمار)))
    {
      scratch_color_stops.resize (0);
      بازگشت نادرست;
    }
    hb_color_سطر_get_color_stops (color_سطر, 0, &شمار, scratch_color_stops.arrayZ);
    بازگشت درست;
  }
};


#endif /* HB_RASTER_PAINT_HH */
