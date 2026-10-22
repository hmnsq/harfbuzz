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

#அந்தர்பாவ "hb.hh"

#அந்தர்பாவ "hb-raster-image.hh"
#அந்தர்பாவ "hb-geometry.hh"

/* hb_raster_clip_t — alpha mask for clipping */
கட்டமைப்பு hb_raster_clip_t
{
  hb_vector_t<uint8_t> alpha;	/* A8 mask, same extents as root surface */
  unsigned width  = 0;
  unsigned height = 0;
  unsigned stride = 0;

  /* Fast path: simple rectangle (no alpha buffer needed) */
  தருக்க is_rect = உண்மை;
  முழு rect_x0 = 0, rect_y0 = 0;
  முழு rect_x1 = 0, rect_y1 = 0;

  /* Bounding box of non-zero alpha region (valid for both rect and mask) */
  unsigned min_x = 0, min_y = 0;
  unsigned max_x = 0, max_y = 0;

  வெறுமை init_full (unsigned w, unsigned h)
  {
    width = w;
    height = h;
    stride = (w + 3u) & ~3u;
    is_rect = உண்மை;
    rect_x0 = 0;
    rect_y0 = 0;
    rect_x1 = (முழு) w;
    rect_y1 = (முழு) h;
    min_x = 0;
    min_y = 0;
    max_x = w;
    max_y = h;
  }

  வெறுமை update_bounds_from_rect ()
  {
    min_x = (unsigned) hb_max (rect_x0, 0);
    min_y = (unsigned) hb_max (rect_y0, 0);
    max_x = (unsigned) hb_max (hb_min (rect_x1, (முழு) width), 0);
    max_y = (unsigned) hb_max (hb_min (rect_y1, (முழு) height), 0);
  }

  uint8_t get_alpha (unsigned x, unsigned y) மாறிலி
  {
    எனில் (is_rect)
      திரும்பு ((முழு) x >= rect_x0 && (முழு) x < rect_x1 &&
	      (முழு) y >= rect_y0 && (முழு) y < rect_y1) ? 255 : 0;
    எனில் (x >= width || y >= height) திரும்பு 0;
    எனில் (stride < width) திரும்பு 0;
    எனில் (height && stride > (size_t) -1 / height) திரும்பு 0;
    size_t idx = (size_t) y * stride + x;
    எனில் (!alpha.arrayZ || idx >= alpha.length) திரும்பு 0;
    திரும்பு alpha[idx];
  }

  தருக்க has_valid_alpha_mask () மாறிலி
  {
    எனில் (is_rect)
      திரும்பு உண்மை;
    எனில் (!alpha.arrayZ || stride < width)
      திரும்பு பொய்;
    எனில் (height && stride > (size_t) -1 / height)
      திரும்பு பொய்;
    திரும்பு (size_t) stride * height <= alpha.length;
  }
};


/* hb_raster_paint_t — color glyph paint context */
கட்டமைப்பு hb_raster_paint_t
{
  hb_object_header_t header;

  /* Configuration */
  hb_transform_t<>    base_transform     = {1, 0, 0, 1, 0, 0};
  தசம               x_scale_factor     = 1.f;
  தசம               y_scale_factor     = 1.f;
  hb_raster_extents_t fixed_extents      = {};
  தருக்க                has_extents  = பொய்;
  hb_color_t          foreground         = HB_COLOR (0, 0, 0, 255);
  hb_color_t          background         = HB_COLOR (0, 0, 0, 0);
  unsigned            palette            = 0;
  hb_map_t           *custom_palette     = nullptr;

  /* Stacks */
  hb_vector_t<hb_transform_t<>>     transform_அடுக்கு;
  hb_vector_t<hb_raster_clip_t>     clip_அடுக்கு;
  hb_vector_t<hb_raster_clip_t>     clip_cache;
  hb_vector_t<hb_raster_image_t *>  surface_அடுக்கு;

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

  int64_t get_default_pixel_budget () மாறிலி
  {
    திரும்பு hb_max ((int64_t) HB_BUDGET_RASTER_PIXELS,
		   (int64_t) HB_BUDGET_RASTER_PAINT_PASSES *
		   fixed_extents.width * fixed_extents.height);
  }

  வெறுமை recharge_budget ()
  {
    budget_remaining = budget == HB_BUDGET_DEFAULT ?
		       (int64_t) HB_BUDGET_GLYPH : budget;
    pixel_remaining = budget == HB_BUDGET_UNLIMITED ? budget :
		      (surface_அடுக்கு.length ? get_default_pixel_budget () :
					      (int64_t) HB_BUDGET_RASTER_PIXELS);
  }

  /* Returns whether the pixel operation should proceed.  Allows a single
   * overshoot past zero; callers skip work once the budget is spent. */
  தருக்க charge_work (int64_t work)
  {
    எனில் (unlikely (pixel_remaining < 0)) திரும்பு பொய்;
    pixel_remaining -= work;
    திரும்பு உண்மை;
  }

  /* Reject work that would exceed the remaining budget before it starts. */
  தருக்க precharge_work (int64_t work)
  {
    எனில் (unlikely (pixel_remaining < work))
    {
      pixel_remaining = -1;
      திரும்பு பொய்;
    }
    pixel_remaining -= work;
    திரும்பு உண்மை;
  }

  hb_raster_image_t *acquire_surface ()
  {
    hb_raster_image_t *img;
    எனில் (surface_cache.length)
      img = surface_cache.எடு ();
    இல்லையெனில்
    {
      img = hb_raster_image_create_or_fail ();
      எனில் (unlikely (!img)) திரும்பு nullptr;
    }

    எனில் (unlikely (!img->configure (HB_RASTER_FORMAT_BGRA32, fixed_extents)))
    {
      hb_raster_image_destroy (img);
      திரும்பு nullptr;
    }
    img->clear ();
    திரும்பு img;
  }

  வெறுமை release_surface (hb_raster_image_t *img)
  {
    எனில் (!surface_cache.தள்ளு_or_fail (img))
      hb_raster_image_destroy (img);
  }

  hb_raster_clip_t acquire_clip (unsigned w, unsigned h)
  {
    hb_raster_clip_t clip;
    எனில் (clip_cache.length)
      clip = clip_cache.எடு ();
    clip.width = w;
    clip.height = h;
    clip.stride = (w + 3u) & ~3u;
    clip.is_rect = பொய்;
    clip.rect_x0 = clip.rect_y0 = 0;
    clip.rect_x1 = clip.rect_y1 = 0;
    clip.min_x = clip.min_y = clip.max_x = clip.max_y = 0;
    திரும்பு clip;
  }

  வெறுமை release_clip (hb_raster_clip_t &&clip)
  {
    எனில் (clip.alpha.arrayZ)
      clip_cache.push (std::move (clip));
  }

  வெறுமை release_all_clips ()
  {
    போது (clip_அடுக்கு.length)
      release_clip (clip_அடுக்கு.எடு ());
  }

  hb_raster_image_t *current_surface ()
  {
    திரும்பு surface_அடுக்கு.length ? surface_அடுக்கு.tail () : nullptr;
  }

  hb_raster_clip_t &current_clip ()
  {
    திரும்பு clip_அடுக்கு.tail ();
  }

  hb_transform_t<> current_transform ()
  {
    எனில் (unlikely (!transform_அடுக்கு.length))
      திரும்பு {1, 0, 0, 1, 0, 0};
    திரும்பு transform_அடுக்கு.tail ();
  }

  வெறுமை apply_scale_factor (hb_transform_t<> &t) மாறிலி
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
    திரும்பு t;
  }

  தருக்க fetch_color_stops (hb_color_வரி_t *color_வரி)
  {
    unsigned மொத்தம் = hb_color_வரி_get_color_stops (color_வரி, 0, nullptr, nullptr);
    எனில் (unlikely (!மொத்தம் || !scratch_color_stops.resize (மொத்தம்)))
    {
      scratch_color_stops.resize (0);
      திரும்பு பொய்;
    }
    hb_color_வரி_get_color_stops (color_வரி, 0, &மொத்தம், scratch_color_stops.arrayZ);
    திரும்பு உண்மை;
  }
};


#endif /* HB_RASTER_PAINT_HH */
