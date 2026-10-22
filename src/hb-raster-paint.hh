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

#که_نه_تعریف_شوی HB_RASTER_PAINT_HH
#تعریف_کړه HB_RASTER_PAINT_HH

#ضمن "hb.hh"

#ضمن "hb-raster-image.hh"
#ضمن "hb-geometry.hh"

/* hb_raster_clip_t — alpha mask for clipping */
بنية hb_raster_clip_t
{
  hb_ویکټور_t<uint8_t> alpha;	/* A8 mask, same extents as root surface */
  بینښه پلنوالی  = 0;
  بینښه لوړوالی = 0;
  بینښه stride = 0;

  /* Fast path: simple rectangle (no alpha buffer needed) */
  منطقي is_rect = ریښتیا;
  صحيح rect_x0 = 0, rect_y0 = 0;
  صحيح rect_x1 = 0, rect_y1 = 0;

  /* Bounding box of non-zero alpha region (valid for both rect and mask) */
  بینښه min_x = 0, min_y = 0;
  بینښه max_x = 0, max_y = 0;

  فراغ init_ډک (بینښه w, بینښه h)
  {
    پلنوالی = w;
    لوړوالی = h;
    stride = (w + 3u) & ~3u;
    is_rect = ریښتیا;
    rect_x0 = 0;
    rect_y0 = 0;
    rect_x1 = (صحيح) w;
    rect_y1 = (صحيح) h;
    min_x = 0;
    min_y = 0;
    max_x = w;
    max_y = h;
  }

  فراغ update_bounds_from_rect ()
  {
    min_x = (بینښه) hb_max (rect_x0, 0);
    min_y = (بینښه) hb_max (rect_y0, 0);
    max_x = (بینښه) hb_max (hb_min (rect_x1, (صحيح) پلنوالی), 0);
    max_y = (بینښه) hb_max (hb_min (rect_y1, (صحيح) لوړوالی), 0);
  }

  uint8_t get_alpha (بینښه x, بینښه y) ثابت
  {
    إذا (is_rect)
      أرجع ((صحيح) x >= rect_x0 && (صحيح) x < rect_x1 &&
	      (صحيح) y >= rect_y0 && (صحيح) y < rect_y1) ? 255 : 0;
    إذا (x >= پلنوالی || y >= لوړوالی) أرجع 0;
    إذا (stride < پلنوالی) أرجع 0;
    إذا (لوړوالی && stride > (size_t) -1 / لوړوالی) أرجع 0;
    size_t idx = (size_t) y * stride + x;
    إذا (!alpha.arrayZ || idx >= alpha.اوږدتیا) أرجع 0;
    أرجع alpha[idx];
  }

  منطقي has_valid_alpha_ماسک () ثابت
  {
    إذا (is_rect)
      أرجع ریښتیا;
    إذا (!alpha.arrayZ || stride < پلنوالی)
      أرجع خطأ;
    إذا (لوړوالی && stride > (size_t) -1 / لوړوالی)
      أرجع خطأ;
    أرجع (size_t) stride * لوړوالی <= alpha.اوږدتیا;
  }
};


/* hb_raster_paint_t — color glyph paint context */
بنية hb_raster_paint_t
{
  hb_object_سرلیک_t سرلیک;

  /* Configuration */
  hb_transform_t<>    بنسټ_transform     = {1, 0, 0, 1, 0, 0};
  عائم               x_scale_ضریب     = 1.f;
  عائم               y_scale_ضریب     = 1.f;
  hb_raster_extents_t fixed_extents      = {};
  منطقي                has_extents  = خطأ;
  hb_color_t          foreground         = HB_COLOR (0, 0, 0, 255);
  hb_color_t          background         = HB_COLOR (0, 0, 0, 0);
  بینښه            palette            = 0;
  hb_map_t           *custom_palette     = تش_اشاره;

  /* Stacks */
  hb_ویکټور_t<hb_transform_t<>>     transform_ډېری;
  hb_ویکټور_t<hb_raster_clip_t>     clip_ډېری;
  hb_ویکټور_t<hb_raster_clip_t>     clip_cache;
  hb_ویکټور_t<hb_raster_image_t *>  surface_ډېری;

  /* Cached surface pool (freelist for reuse across push/pop group) */
  hb_ویکټور_t<hb_raster_image_t *>  surface_cache;
  hb_ویکټور_t<hb_color_stop_t>      scratch_color_stops;

  /* Internal rasterizer for clip-to-glyph */
  hb_raster_draw_t *clip_rdr = تش_اشاره;

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
  int64_t budget_پاتې_شوی = HB_BUDGET_GLYPH;
  int64_t pixel_پاتې_شوی = HB_BUDGET_RASTER_PIXELS;

  /* Helpers */

  int64_t get_default_pixel_budget () ثابت
  {
    أرجع hb_max ((int64_t) HB_BUDGET_RASTER_PIXELS,
		   (int64_t) HB_BUDGET_RASTER_PAINT_PASSES *
		   fixed_extents.پلنوالی * fixed_extents.لوړوالی);
  }

  فراغ recharge_budget ()
  {
    budget_پاتې_شوی = budget == HB_BUDGET_DEFAULT ?
		       (int64_t) HB_BUDGET_GLYPH : budget;
    pixel_پاتې_شوی = budget == HB_BUDGET_UNLIMITED ? budget :
		      (surface_ډېری.اوږدتیا ? get_default_pixel_budget () :
					      (int64_t) HB_BUDGET_RASTER_PIXELS);
  }

  /* Returns whether the pixel operation should proceed.  Allows a single
   * overshoot past zero; callers skip work once the budget is spent. */
  منطقي charge_work (int64_t work)
  {
    إذا (unlikely (pixel_پاتې_شوی < 0)) أرجع خطأ;
    pixel_پاتې_شوی -= work;
    أرجع ریښتیا;
  }

  /* Reject work that would exceed the remaining budget before it starts. */
  منطقي precharge_work (int64_t work)
  {
    إذا (unlikely (pixel_پاتې_شوی < work))
    {
      pixel_پاتې_شوی = -1;
      أرجع خطأ;
    }
    pixel_پاتې_شوی -= work;
    أرجع ریښتیا;
  }

  hb_raster_image_t *acquire_surface ()
  {
    hb_raster_image_t *img;
    إذا (surface_cache.اوږدتیا)
      img = surface_cache.راوباسه ();
    وإلا
    {
      img = hb_raster_image_create_or_fail ();
      إذا (unlikely (!img)) أرجع تش_اشاره;
    }

    إذا (unlikely (!img->configure (HB_RASTER_FORMAT_BGRA32, fixed_extents)))
    {
      hb_raster_image_destroy (img);
      أرجع تش_اشاره;
    }
    img->clear ();
    أرجع img;
  }

  فراغ release_surface (hb_raster_image_t *img)
  {
    إذا (!surface_cache.ټېل_or_fail (img))
      hb_raster_image_destroy (img);
  }

  hb_raster_clip_t acquire_clip (بینښه w, بینښه h)
  {
    hb_raster_clip_t clip;
    إذا (clip_cache.اوږدتیا)
      clip = clip_cache.راوباسه ();
    clip.پلنوالی = w;
    clip.لوړوالی = h;
    clip.stride = (w + 3u) & ~3u;
    clip.is_rect = خطأ;
    clip.rect_x0 = clip.rect_y0 = 0;
    clip.rect_x1 = clip.rect_y1 = 0;
    clip.min_x = clip.min_y = clip.max_x = clip.max_y = 0;
    أرجع clip;
  }

  فراغ release_clip (hb_raster_clip_t &&clip)
  {
    إذا (clip.alpha.arrayZ)
      clip_cache.push (std::move (clip));
  }

  فراغ release_all_clips ()
  {
    طالما (clip_ډېری.اوږدتیا)
      release_clip (clip_ډېری.راوباسه ());
  }

  hb_raster_image_t *current_surface ()
  {
    أرجع surface_ډېری.اوږدتیا ? surface_ډېری.لکۍ () : تش_اشاره;
  }

  hb_raster_clip_t &current_clip ()
  {
    أرجع clip_ډېری.لکۍ ();
  }

  hb_transform_t<> current_transform ()
  {
    إذا (unlikely (!transform_ډېری.اوږدتیا))
      أرجع {1, 0, 0, 1, 0, 0};
    أرجع transform_ډېری.لکۍ ();
  }

  فراغ apply_scale_ضریب (hb_transform_t<> &t) ثابت
  {
    t.xx /= x_scale_ضریب;
    t.xy /= x_scale_ضریب;
    t.x0 /= x_scale_ضریب;
    t.yx /= y_scale_ضریب;
    t.yy /= y_scale_ضریب;
    t.y0 /= y_scale_ضریب;
  }

  hb_transform_t<> current_effective_transform ()
  {
    hb_transform_t<> t = current_transform ();
    apply_scale_ضریب (t);
    أرجع t;
  }

  منطقي fetch_color_stops (hb_color_کرښه_t *color_کرښه)
  {
    بینښه شمېر = hb_color_کرښه_get_color_stops (color_کرښه, 0, تش_اشاره, تش_اشاره);
    إذا (unlikely (!شمېر || !scratch_color_stops.resize (شمېر)))
    {
      scratch_color_stops.resize (0);
      أرجع خطأ;
    }
    hb_color_کرښه_get_color_stops (color_کرښه, 0, &شمېر, scratch_color_stops.arrayZ);
    أرجع ریښتیا;
  }
};


#د_که_پای /* HB_RASTER_PAINT_HH */
