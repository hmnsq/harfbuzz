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

#অন্তর্ভাব "hb.hh"

#অন্তর্ভাব "hb-raster-image.hh"
#অন্তর্ভাব "hb-geometry.hh"

/* hb_raster_clip_t — alpha mask for clipping */
কাঠামো hb_raster_clip_t
{
  hb_vector_t<uint8_t> alpha;	/* A8 mask, same extents as root surface */
  unsigned width  = 0;
  unsigned height = 0;
  unsigned stride = 0;

  /* Fast path: simple rectangle (no alpha buffer needed) */
  যৌক্তিক is_rect = সত্য;
  পূর্ণসংখ্যা rect_x0 = 0, rect_y0 = 0;
  পূর্ণসংখ্যা rect_x1 = 0, rect_y1 = 0;

  /* Bounding box of non-zero alpha region (valid for both rect and mask) */
  unsigned min_x = 0, min_y = 0;
  unsigned max_x = 0, max_y = 0;

  শূন্য init_full (unsigned w, unsigned h)
  {
    width = w;
    height = h;
    stride = (w + 3u) & ~3u;
    is_rect = সত্য;
    rect_x0 = 0;
    rect_y0 = 0;
    rect_x1 = (পূর্ণসংখ্যা) w;
    rect_y1 = (পূর্ণসংখ্যা) h;
    min_x = 0;
    min_y = 0;
    max_x = w;
    max_y = h;
  }

  শূন্য update_bounds_from_rect ()
  {
    min_x = (unsigned) hb_max (rect_x0, 0);
    min_y = (unsigned) hb_max (rect_y0, 0);
    max_x = (unsigned) hb_max (hb_min (rect_x1, (পূর্ণসংখ্যা) width), 0);
    max_y = (unsigned) hb_max (hb_min (rect_y1, (পূর্ণসংখ্যা) height), 0);
  }

  uint8_t get_alpha (unsigned x, unsigned y) ধ্রুবক
  {
    যদি (is_rect)
      ফেরত ((পূর্ণসংখ্যা) x >= rect_x0 && (পূর্ণসংখ্যা) x < rect_x1 &&
	      (পূর্ণসংখ্যা) y >= rect_y0 && (পূর্ণসংখ্যা) y < rect_y1) ? 255 : 0;
    যদি (x >= width || y >= height) ফেরত 0;
    যদি (stride < width) ফেরত 0;
    যদি (height && stride > (size_t) -1 / height) ফেরত 0;
    size_t idx = (size_t) y * stride + x;
    যদি (!alpha.arrayZ || idx >= alpha.দৈর্ঘ্য) ফেরত 0;
    ফেরত alpha[idx];
  }

  যৌক্তিক has_valid_alpha_mask () ধ্রুবক
  {
    যদি (is_rect)
      ফেরত সত্য;
    যদি (!alpha.arrayZ || stride < width)
      ফেরত মিথ্যা;
    যদি (height && stride > (size_t) -1 / height)
      ফেরত মিথ্যা;
    ফেরত (size_t) stride * height <= alpha.দৈর্ঘ্য;
  }
};


/* hb_raster_paint_t — color glyph paint context */
কাঠামো hb_raster_paint_t
{
  hb_object_header_t header;

  /* Configuration */
  hb_transform_t<>    base_transform     = {1, 0, 0, 1, 0, 0};
  দশমিক               x_scale_factor     = 1.f;
  দশমিক               y_scale_factor     = 1.f;
  hb_raster_extents_t fixed_extents      = {};
  যৌক্তিক                has_extents  = মিথ্যা;
  hb_color_t          foreground         = HB_COLOR (0, 0, 0, 255);
  hb_color_t          background         = HB_COLOR (0, 0, 0, 0);
  unsigned            palette            = 0;
  hb_map_t           *custom_palette     = nullptr;

  /* Stacks */
  hb_vector_t<hb_transform_t<>>     transform_স্তূপ;
  hb_vector_t<hb_raster_clip_t>     clip_স্তূপ;
  hb_vector_t<hb_raster_clip_t>     clip_cache;
  hb_vector_t<hb_raster_image_t *>  surface_স্তূপ;

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

  int64_t get_default_pixel_budget () ধ্রুবক
  {
    ফেরত hb_max ((int64_t) HB_BUDGET_RASTER_PIXELS,
		   (int64_t) HB_BUDGET_RASTER_PAINT_PASSES *
		   fixed_extents.width * fixed_extents.height);
  }

  শূন্য recharge_budget ()
  {
    budget_remaining = budget == HB_BUDGET_DEFAULT ?
		       (int64_t) HB_BUDGET_GLYPH : budget;
    pixel_remaining = budget == HB_BUDGET_UNLIMITED ? budget :
		      (surface_স্তূপ.দৈর্ঘ্য ? get_default_pixel_budget () :
					      (int64_t) HB_BUDGET_RASTER_PIXELS);
  }

  /* Returns whether the pixel operation should proceed.  Allows a single
   * overshoot past zero; callers skip work once the budget is spent. */
  যৌক্তিক charge_work (int64_t work)
  {
    যদি (unlikely (pixel_remaining < 0)) ফেরত মিথ্যা;
    pixel_remaining -= work;
    ফেরত সত্য;
  }

  /* Reject work that would exceed the remaining budget before it starts. */
  যৌক্তিক precharge_work (int64_t work)
  {
    যদি (unlikely (pixel_remaining < work))
    {
      pixel_remaining = -1;
      ফেরত মিথ্যা;
    }
    pixel_remaining -= work;
    ফেরত সত্য;
  }

  hb_raster_image_t *acquire_surface ()
  {
    hb_raster_image_t *img;
    যদি (surface_cache.দৈর্ঘ্য)
      img = surface_cache.বের_করো ();
    অন্যথা
    {
      img = hb_raster_image_create_or_fail ();
      যদি (unlikely (!img)) ফেরত nullptr;
    }

    যদি (unlikely (!img->configure (HB_RASTER_FORMAT_BGRA32, fixed_extents)))
    {
      hb_raster_image_destroy (img);
      ফেরত nullptr;
    }
    img->clear ();
    ফেরত img;
  }

  শূন্য release_surface (hb_raster_image_t *img)
  {
    যদি (!surface_cache.ঠেলো_or_fail (img))
      hb_raster_image_destroy (img);
  }

  hb_raster_clip_t acquire_clip (unsigned w, unsigned h)
  {
    hb_raster_clip_t clip;
    যদি (clip_cache.দৈর্ঘ্য)
      clip = clip_cache.বের_করো ();
    clip.width = w;
    clip.height = h;
    clip.stride = (w + 3u) & ~3u;
    clip.is_rect = মিথ্যা;
    clip.rect_x0 = clip.rect_y0 = 0;
    clip.rect_x1 = clip.rect_y1 = 0;
    clip.min_x = clip.min_y = clip.max_x = clip.max_y = 0;
    ফেরত clip;
  }

  শূন্য release_clip (hb_raster_clip_t &&clip)
  {
    যদি (clip.alpha.arrayZ)
      clip_cache.push (std::move (clip));
  }

  শূন্য release_all_clips ()
  {
    যখন (clip_স্তূপ.দৈর্ঘ্য)
      release_clip (clip_স্তূপ.বের_করো ());
  }

  hb_raster_image_t *current_surface ()
  {
    ফেরত surface_স্তূপ.দৈর্ঘ্য ? surface_স্তূপ.লেজ () : nullptr;
  }

  hb_raster_clip_t &current_clip ()
  {
    ফেরত clip_স্তূপ.লেজ ();
  }

  hb_transform_t<> current_transform ()
  {
    যদি (unlikely (!transform_স্তূপ.দৈর্ঘ্য))
      ফেরত {1, 0, 0, 1, 0, 0};
    ফেরত transform_স্তূপ.লেজ ();
  }

  শূন্য apply_scale_factor (hb_transform_t<> &t) ধ্রুবক
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
    ফেরত t;
  }

  যৌক্তিক fetch_color_stops (hb_color_সারি_t *color_সারি)
  {
    unsigned গুনতি = hb_color_সারি_get_color_stops (color_সারি, 0, nullptr, nullptr);
    যদি (unlikely (!গুনতি || !scratch_color_stops.resize (গুনতি)))
    {
      scratch_color_stops.resize (0);
      ফেরত মিথ্যা;
    }
    hb_color_সারি_get_color_stops (color_সারি, 0, &গুনতি, scratch_color_stops.arrayZ);
    ফেরত সত্য;
  }
};


#endif /* HB_RASTER_PAINT_HH */
