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

#اگر_غیر_تعریف_شدہ HB_RASTER_PAINT_HH
#تعریف_کرو HB_RASTER_PAINT_HH

#شامل "hb.hh"

#شامل "hb-raster-image.hh"
#شامل "hb-geometry.hh"

/* hb_raster_clip_t — alpha mask for clipping */
ساخت hb_raster_clip_t
{
  hb_سمتیہ_t<uint8_t> alpha;	/* A8 mask, same extents as root surface */
  غیرعلامتی چوڑائی  = 0;
  غیرعلامتی اونچائی = 0;
  غیرعلامتی stride = 0;

  /* Fast path: simple rectangle (no alpha buffer needed) */
  منطقی is_rect = سچ;
  صحیح rect_x0 = 0, rect_y0 = 0;
  صحیح rect_x1 = 0, rect_y1 = 0;

  /* Bounding box of non-zero alpha region (valid for both rect and mask) */
  غیرعلامتی min_x = 0, min_y = 0;
  غیرعلامتی max_x = 0, max_y = 0;

  خالی init_بھرا (غیرعلامتی w, غیرعلامتی h)
  {
    چوڑائی = w;
    اونچائی = h;
    stride = (w + 3u) & ~3u;
    is_rect = سچ;
    rect_x0 = 0;
    rect_y0 = 0;
    rect_x1 = (صحیح) w;
    rect_y1 = (صحیح) h;
    min_x = 0;
    min_y = 0;
    max_x = w;
    max_y = h;
  }

  خالی update_bounds_from_rect ()
  {
    min_x = (غیرعلامتی) hb_max (rect_x0, 0);
    min_y = (غیرعلامتی) hb_max (rect_y0, 0);
    max_x = (غیرعلامتی) hb_max (hb_min (rect_x1, (صحیح) چوڑائی), 0);
    max_y = (غیرعلامتی) hb_max (hb_min (rect_y1, (صحیح) اونچائی), 0);
  }

  uint8_t get_alpha (غیرعلامتی x, غیرعلامتی y) ثابت
  {
    اگر (is_rect)
      واپس ((صحیح) x >= rect_x0 && (صحیح) x < rect_x1 &&
	      (صحیح) y >= rect_y0 && (صحیح) y < rect_y1) ? 255 : 0;
    اگر (x >= چوڑائی || y >= اونچائی) واپس 0;
    اگر (stride < چوڑائی) واپس 0;
    اگر (اونچائی && stride > (size_t) -1 / اونچائی) واپس 0;
    size_t idx = (size_t) y * stride + x;
    اگر (!alpha.arrayZ || idx >= alpha.لمبائی) واپس 0;
    واپس alpha[idx];
  }

  منطقی has_valid_alpha_نقاب () ثابت
  {
    اگر (is_rect)
      واپس سچ;
    اگر (!alpha.arrayZ || stride < چوڑائی)
      واپس جھوٹ;
    اگر (اونچائی && stride > (size_t) -1 / اونچائی)
      واپس جھوٹ;
    واپس (size_t) stride * اونچائی <= alpha.لمبائی;
  }
};


/* hb_raster_paint_t — color glyph paint context */
ساخت hb_raster_paint_t
{
  hb_object_سرخی_t سرخی;

  /* Configuration */
  hb_transform_t<>    بنیاد_transform     = {1, 0, 0, 1, 0, 0};
  اعشاری               x_scale_ضربی_عدد     = 1.f;
  اعشاری               y_scale_ضربی_عدد     = 1.f;
  hb_raster_extents_t fixed_extents      = {};
  منطقی                has_extents  = جھوٹ;
  hb_color_t          foreground         = HB_COLOR (0, 0, 0, 255);
  hb_color_t          background         = HB_COLOR (0, 0, 0, 0);
  غیرعلامتی            palette            = 0;
  hb_map_t           *custom_palette     = خالی_اشاریہ;

  /* Stacks */
  hb_سمتیہ_t<hb_transform_t<>>     transform_ڈھیر;
  hb_سمتیہ_t<hb_raster_clip_t>     clip_ڈھیر;
  hb_سمتیہ_t<hb_raster_clip_t>     clip_cache;
  hb_سمتیہ_t<hb_raster_image_t *>  surface_ڈھیر;

  /* Cached surface pool (freelist for reuse across push/pop group) */
  hb_سمتیہ_t<hb_raster_image_t *>  surface_cache;
  hb_سمتیہ_t<hb_color_stop_t>      scratch_color_stops;

  /* Internal rasterizer for clip-to-glyph */
  hb_raster_draw_t *clip_rdr = خالی_اشاریہ;

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
  int64_t budget_بقایا = HB_BUDGET_GLYPH;
  int64_t pixel_بقایا = HB_BUDGET_RASTER_PIXELS;

  /* Helpers */

  int64_t get_default_pixel_budget () ثابت
  {
    واپس hb_max ((int64_t) HB_BUDGET_RASTER_PIXELS,
		   (int64_t) HB_BUDGET_RASTER_PAINT_PASSES *
		   fixed_extents.چوڑائی * fixed_extents.اونچائی);
  }

  خالی recharge_budget ()
  {
    budget_بقایا = budget == HB_BUDGET_DEFAULT ?
		       (int64_t) HB_BUDGET_GLYPH : budget;
    pixel_بقایا = budget == HB_BUDGET_UNLIMITED ? budget :
		      (surface_ڈھیر.لمبائی ? get_default_pixel_budget () :
					      (int64_t) HB_BUDGET_RASTER_PIXELS);
  }

  /* Returns whether the pixel operation should proceed.  Allows a single
   * overshoot past zero; callers skip work once the budget is spent. */
  منطقی charge_work (int64_t work)
  {
    اگر (unlikely (pixel_بقایا < 0)) واپس جھوٹ;
    pixel_بقایا -= work;
    واپس سچ;
  }

  /* Reject work that would exceed the remaining budget before it starts. */
  منطقی precharge_work (int64_t work)
  {
    اگر (unlikely (pixel_بقایا < work))
    {
      pixel_بقایا = -1;
      واپس جھوٹ;
    }
    pixel_بقایا -= work;
    واپس سچ;
  }

  hb_raster_image_t *acquire_surface ()
  {
    hb_raster_image_t *img;
    اگر (surface_cache.لمبائی)
      img = surface_cache.نکالو ();
    ورنہ
    {
      img = hb_raster_image_create_or_fail ();
      اگر (unlikely (!img)) واپس خالی_اشاریہ;
    }

    اگر (unlikely (!img->configure (HB_RASTER_FORMAT_BGRA32, fixed_extents)))
    {
      hb_raster_image_destroy (img);
      واپس خالی_اشاریہ;
    }
    img->clear ();
    واپس img;
  }

  خالی release_surface (hb_raster_image_t *img)
  {
    اگر (!surface_cache.ڈالو_or_fail (img))
      hb_raster_image_destroy (img);
  }

  hb_raster_clip_t acquire_clip (غیرعلامتی w, غیرعلامتی h)
  {
    hb_raster_clip_t clip;
    اگر (clip_cache.لمبائی)
      clip = clip_cache.نکالو ();
    clip.چوڑائی = w;
    clip.اونچائی = h;
    clip.stride = (w + 3u) & ~3u;
    clip.is_rect = جھوٹ;
    clip.rect_x0 = clip.rect_y0 = 0;
    clip.rect_x1 = clip.rect_y1 = 0;
    clip.min_x = clip.min_y = clip.max_x = clip.max_y = 0;
    واپس clip;
  }

  خالی release_clip (hb_raster_clip_t &&clip)
  {
    اگر (clip.alpha.arrayZ)
      clip_cache.push (std::move (clip));
  }

  خالی release_all_clips ()
  {
    جب (clip_ڈھیر.لمبائی)
      release_clip (clip_ڈھیر.نکالو ());
  }

  hb_raster_image_t *current_surface ()
  {
    واپس surface_ڈھیر.لمبائی ? surface_ڈھیر.دم () : خالی_اشاریہ;
  }

  hb_raster_clip_t &current_clip ()
  {
    واپس clip_ڈھیر.دم ();
  }

  hb_transform_t<> current_transform ()
  {
    اگر (unlikely (!transform_ڈھیر.لمبائی))
      واپس {1, 0, 0, 1, 0, 0};
    واپس transform_ڈھیر.دم ();
  }

  خالی apply_scale_ضربی_عدد (hb_transform_t<> &t) ثابت
  {
    t.xx /= x_scale_ضربی_عدد;
    t.xy /= x_scale_ضربی_عدد;
    t.x0 /= x_scale_ضربی_عدد;
    t.yx /= y_scale_ضربی_عدد;
    t.yy /= y_scale_ضربی_عدد;
    t.y0 /= y_scale_ضربی_عدد;
  }

  hb_transform_t<> current_effective_transform ()
  {
    hb_transform_t<> t = current_transform ();
    apply_scale_ضربی_عدد (t);
    واپس t;
  }

  منطقی fetch_color_stops (hb_color_سطر_t *color_سطر)
  {
    غیرعلامتی گنتی = hb_color_سطر_get_color_stops (color_سطر, 0, خالی_اشاریہ, خالی_اشاریہ);
    اگر (unlikely (!گنتی || !scratch_color_stops.resize (گنتی)))
    {
      scratch_color_stops.resize (0);
      واپس جھوٹ;
    }
    hb_color_سطر_get_color_stops (color_سطر, 0, &گنتی, scratch_color_stops.arrayZ);
    واپس سچ;
  }
};


#اگر_ختم /* HB_RASTER_PAINT_HH */
