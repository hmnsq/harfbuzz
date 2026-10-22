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

#ଅନ୍ତର୍ଭାଵ "hb.hh"

#ଅନ୍ତର୍ଭାଵ "hb-raster-image.hh"
#ଅନ୍ତର୍ଭାଵ "hb-geometry.hh"

/* hb_raster_clip_t — alpha mask for clipping */
ସଂରଚନା hb_raster_clip_t
{
  hb_vector_t<uint8_t> alpha;	/* A8 mask, same extents as root surface */
  unsigned width  = 0;
  unsigned height = 0;
  unsigned stride = 0;

  /* Fast path: simple rectangle (no alpha buffer needed) */
  ତାର୍କିକ is_rect = ସତ୍ୟ;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା rect_x0 = 0, rect_y0 = 0;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା rect_x1 = 0, rect_y1 = 0;

  /* Bounding box of non-zero alpha region (valid for both rect and mask) */
  unsigned min_x = 0, min_y = 0;
  unsigned max_x = 0, max_y = 0;

  ଶୂନ୍ୟ init_full (unsigned w, unsigned h)
  {
    width = w;
    height = h;
    stride = (w + 3u) & ~3u;
    is_rect = ସତ୍ୟ;
    rect_x0 = 0;
    rect_y0 = 0;
    rect_x1 = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) w;
    rect_y1 = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) h;
    min_x = 0;
    min_y = 0;
    max_x = w;
    max_y = h;
  }

  ଶୂନ୍ୟ update_bounds_from_rect ()
  {
    min_x = (unsigned) hb_max (rect_x0, 0);
    min_y = (unsigned) hb_max (rect_y0, 0);
    max_x = (unsigned) hb_max (hb_min (rect_x1, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) width), 0);
    max_y = (unsigned) hb_max (hb_min (rect_y1, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) height), 0);
  }

  uint8_t get_alpha (unsigned x, unsigned y) ଅଚଳ
  {
    ଯଦି (is_rect)
      ଫେରନ୍ତୁ ((ପୂର୍ଣ୍ଣସଂଖ୍ୟା) x >= rect_x0 && (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) x < rect_x1 &&
	      (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) y >= rect_y0 && (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) y < rect_y1) ? 255 : 0;
    ଯଦି (x >= width || y >= height) ଫେରନ୍ତୁ 0;
    ଯଦି (stride < width) ଫେରନ୍ତୁ 0;
    ଯଦି (height && stride > (size_t) -1 / height) ଫେରନ୍ତୁ 0;
    size_t idx = (size_t) y * stride + x;
    ଯଦି (!alpha.arrayZ || idx >= alpha.length) ଫେରନ୍ତୁ 0;
    ଫେରନ୍ତୁ alpha[idx];
  }

  ତାର୍କିକ has_valid_alpha_mask () ଅଚଳ
  {
    ଯଦି (is_rect)
      ଫେରନ୍ତୁ ସତ୍ୟ;
    ଯଦି (!alpha.arrayZ || stride < width)
      ଫେରନ୍ତୁ ମିଥ୍ୟା;
    ଯଦି (height && stride > (size_t) -1 / height)
      ଫେରନ୍ତୁ ମିଥ୍ୟା;
    ଫେରନ୍ତୁ (size_t) stride * height <= alpha.length;
  }
};


/* hb_raster_paint_t — color glyph paint context */
ସଂରଚନା hb_raster_paint_t
{
  hb_object_header_t header;

  /* Configuration */
  hb_transform_t<>    base_transform     = {1, 0, 0, 1, 0, 0};
  ଦଶମିକ               x_scale_factor     = 1.f;
  ଦଶମିକ               y_scale_factor     = 1.f;
  hb_raster_extents_t fixed_extents      = {};
  ତାର୍କିକ                has_extents  = ମିଥ୍ୟା;
  hb_color_t          foreground         = HB_COLOR (0, 0, 0, 255);
  hb_color_t          background         = HB_COLOR (0, 0, 0, 0);
  unsigned            palette            = 0;
  hb_map_t           *custom_palette     = nullptr;

  /* Stacks */
  hb_vector_t<hb_transform_t<>>     transform_ଗଦା;
  hb_vector_t<hb_raster_clip_t>     clip_ଗଦା;
  hb_vector_t<hb_raster_clip_t>     clip_cache;
  hb_vector_t<hb_raster_image_t *>  surface_ଗଦା;

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

  int64_t get_default_pixel_budget () ଅଚଳ
  {
    ଫେରନ୍ତୁ hb_max ((int64_t) HB_BUDGET_RASTER_PIXELS,
		   (int64_t) HB_BUDGET_RASTER_PAINT_PASSES *
		   fixed_extents.width * fixed_extents.height);
  }

  ଶୂନ୍ୟ recharge_budget ()
  {
    budget_remaining = budget == HB_BUDGET_DEFAULT ?
		       (int64_t) HB_BUDGET_GLYPH : budget;
    pixel_remaining = budget == HB_BUDGET_UNLIMITED ? budget :
		      (surface_ଗଦା.length ? get_default_pixel_budget () :
					      (int64_t) HB_BUDGET_RASTER_PIXELS);
  }

  /* Returns whether the pixel operation should proceed.  Allows a single
   * overshoot past zero; callers skip work once the budget is spent. */
  ତାର୍କିକ charge_work (int64_t work)
  {
    ଯଦି (unlikely (pixel_remaining < 0)) ଫେରନ୍ତୁ ମିଥ୍ୟା;
    pixel_remaining -= work;
    ଫେରନ୍ତୁ ସତ୍ୟ;
  }

  /* Reject work that would exceed the remaining budget before it starts. */
  ତାର୍କିକ precharge_work (int64_t work)
  {
    ଯଦି (unlikely (pixel_remaining < work))
    {
      pixel_remaining = -1;
      ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }
    pixel_remaining -= work;
    ଫେରନ୍ତୁ ସତ୍ୟ;
  }

  hb_raster_image_t *acquire_surface ()
  {
    hb_raster_image_t *img;
    ଯଦି (surface_cache.length)
      img = surface_cache.କାଢ଼ ();
    ଅନ୍ୟଥା
    {
      img = hb_raster_image_create_or_fail ();
      ଯଦି (unlikely (!img)) ଫେରନ୍ତୁ nullptr;
    }

    ଯଦି (unlikely (!img->configure (HB_RASTER_FORMAT_BGRA32, fixed_extents)))
    {
      hb_raster_image_destroy (img);
      ଫେରନ୍ତୁ nullptr;
    }
    img->clear ();
    ଫେରନ୍ତୁ img;
  }

  ଶୂନ୍ୟ release_surface (hb_raster_image_t *img)
  {
    ଯଦି (!surface_cache.ଠେଲ_or_fail (img))
      hb_raster_image_destroy (img);
  }

  hb_raster_clip_t acquire_clip (unsigned w, unsigned h)
  {
    hb_raster_clip_t clip;
    ଯଦି (clip_cache.length)
      clip = clip_cache.କାଢ଼ ();
    clip.width = w;
    clip.height = h;
    clip.stride = (w + 3u) & ~3u;
    clip.is_rect = ମିଥ୍ୟା;
    clip.rect_x0 = clip.rect_y0 = 0;
    clip.rect_x1 = clip.rect_y1 = 0;
    clip.min_x = clip.min_y = clip.max_x = clip.max_y = 0;
    ଫେରନ୍ତୁ clip;
  }

  ଶୂନ୍ୟ release_clip (hb_raster_clip_t &&clip)
  {
    ଯଦି (clip.alpha.arrayZ)
      clip_cache.push (std::move (clip));
  }

  ଶୂନ୍ୟ release_all_clips ()
  {
    ଯେତେବେଳେ (clip_ଗଦା.length)
      release_clip (clip_ଗଦା.କାଢ଼ ());
  }

  hb_raster_image_t *current_surface ()
  {
    ଫେରନ୍ତୁ surface_ଗଦା.length ? surface_ଗଦା.tail () : nullptr;
  }

  hb_raster_clip_t &current_clip ()
  {
    ଫେରନ୍ତୁ clip_ଗଦା.tail ();
  }

  hb_transform_t<> current_transform ()
  {
    ଯଦି (unlikely (!transform_ଗଦା.length))
      ଫେରନ୍ତୁ {1, 0, 0, 1, 0, 0};
    ଫେରନ୍ତୁ transform_ଗଦା.tail ();
  }

  ଶୂନ୍ୟ apply_scale_factor (hb_transform_t<> &t) ଅଚଳ
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
    ଫେରନ୍ତୁ t;
  }

  ତାର୍କିକ fetch_color_stops (hb_color_ଧାଡ଼ି_t *color_ଧାଡ଼ି)
  {
    unsigned ଗଣନା_2 = hb_color_ଧାଡ଼ି_get_color_stops (color_ଧାଡ଼ି, 0, nullptr, nullptr);
    ଯଦି (unlikely (!ଗଣନା_2 || !scratch_color_stops.resize (ଗଣନା_2)))
    {
      scratch_color_stops.resize (0);
      ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }
    hb_color_ଧାଡ଼ି_get_color_stops (color_ଧାଡ଼ି, 0, &ଗଣନା_2, scratch_color_stops.arrayZ);
    ଫେରନ୍ତୁ ସତ୍ୟ;
  }
};


#endif /* HB_RASTER_PAINT_HH */
