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

#నిర్వచితం_కాకపోతే HB_RASTER_PAINT_HH
#నిర్వచించు HB_RASTER_PAINT_HH

#అన్తర్భావ "hb.hh"

#అన్తర్భావ "hb-raster-image.hh"
#అన్తర్భావ "hb-geometry.hh"

/* hb_raster_clip_t — alpha mask for clipping */
నిర్మాణం hb_raster_clip_t
{
  hb_సదిశ_t<uint8_t> alpha;	/* A8 mask, same extents as root surface */
  గుర్తులేని వెడల్పు  = 0;
  గుర్తులేని ఎత్తు = 0;
  గుర్తులేని stride = 0;

  /* Fast path: simple rectangle (no alpha buffer needed) */
  తార్కిక is_rect = నిజం;
  పూర్ణాంకం rect_x0 = 0, rect_y0 = 0;
  పూర్ణాంకం rect_x1 = 0, rect_y1 = 0;

  /* Bounding box of non-zero alpha region (valid for both rect and mask) */
  గుర్తులేని min_x = 0, min_y = 0;
  గుర్తులేని max_x = 0, max_y = 0;

  శూన్య init_నిండిన (గుర్తులేని w, గుర్తులేని h)
  {
    వెడల్పు = w;
    ఎత్తు = h;
    stride = (w + 3u) & ~3u;
    is_rect = నిజం;
    rect_x0 = 0;
    rect_y0 = 0;
    rect_x1 = (పూర్ణాంకం) w;
    rect_y1 = (పూర్ణాంకం) h;
    min_x = 0;
    min_y = 0;
    max_x = w;
    max_y = h;
  }

  శూన్య update_bounds_from_rect ()
  {
    min_x = (గుర్తులేని) hb_max (rect_x0, 0);
    min_y = (గుర్తులేని) hb_max (rect_y0, 0);
    max_x = (గుర్తులేని) hb_max (hb_min (rect_x1, (పూర్ణాంకం) వెడల్పు), 0);
    max_y = (గుర్తులేని) hb_max (hb_min (rect_y1, (పూర్ణాంకం) ఎత్తు), 0);
  }

  uint8_t get_alpha (గుర్తులేని x, గుర్తులేని y) స్థిరరాశి
  {
    ఒకవేళ (is_rect)
      తిరిగి ((పూర్ణాంకం) x >= rect_x0 && (పూర్ణాంకం) x < rect_x1 &&
	      (పూర్ణాంకం) y >= rect_y0 && (పూర్ణాంకం) y < rect_y1) ? 255 : 0;
    ఒకవేళ (x >= వెడల్పు || y >= ఎత్తు) తిరిగి 0;
    ఒకవేళ (stride < వెడల్పు) తిరిగి 0;
    ఒకవేళ (ఎత్తు && stride > (size_t) -1 / ఎత్తు) తిరిగి 0;
    size_t idx = (size_t) y * stride + x;
    ఒకవేళ (!alpha.arrayZ || idx >= alpha.నిడివి) తిరిగి 0;
    తిరిగి alpha[idx];
  }

  తార్కిక has_valid_alpha_ముసుగు () స్థిరరాశి
  {
    ఒకవేళ (is_rect)
      తిరిగి నిజం;
    ఒకవేళ (!alpha.arrayZ || stride < వెడల్పు)
      తిరిగి అబద్ధం;
    ఒకవేళ (ఎత్తు && stride > (size_t) -1 / ఎత్తు)
      తిరిగి అబద్ధం;
    తిరిగి (size_t) stride * ఎత్తు <= alpha.నిడివి;
  }
};


/* hb_raster_paint_t — color glyph paint context */
నిర్మాణం hb_raster_paint_t
{
  hb_object_శీర్షిక_t శీర్షిక;

  /* Configuration */
  hb_transform_t<>    పునాది_transform     = {1, 0, 0, 1, 0, 0};
  దశాంశం               x_scale_గుణకం     = 1.f;
  దశాంశం               y_scale_గుణకం     = 1.f;
  hb_raster_extents_t fixed_extents      = {};
  తార్కిక                has_extents  = అబద్ధం;
  hb_color_t          foreground         = HB_COLOR (0, 0, 0, 255);
  hb_color_t          background         = HB_COLOR (0, 0, 0, 0);
  గుర్తులేని            palette            = 0;
  hb_map_t           *custom_palette     = శూన్యసూచిక;

  /* Stacks */
  hb_సదిశ_t<hb_transform_t<>>     transform_దొంతర;
  hb_సదిశ_t<hb_raster_clip_t>     clip_దొంతర;
  hb_సదిశ_t<hb_raster_clip_t>     clip_cache;
  hb_సదిశ_t<hb_raster_image_t *>  surface_దొంతర;

  /* Cached surface pool (freelist for reuse across push/pop group) */
  hb_సదిశ_t<hb_raster_image_t *>  surface_cache;
  hb_సదిశ_t<hb_color_stop_t>      scratch_color_stops;

  /* Internal rasterizer for clip-to-glyph */
  hb_raster_draw_t *clip_rdr = శూన్యసూచిక;

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
  int64_t budget_మిగిలిన = HB_BUDGET_GLYPH;
  int64_t pixel_మిగిలిన = HB_BUDGET_RASTER_PIXELS;

  /* Helpers */

  int64_t get_default_pixel_budget () స్థిరరాశి
  {
    తిరిగి hb_max ((int64_t) HB_BUDGET_RASTER_PIXELS,
		   (int64_t) HB_BUDGET_RASTER_PAINT_PASSES *
		   fixed_extents.వెడల్పు * fixed_extents.ఎత్తు);
  }

  శూన్య recharge_budget ()
  {
    budget_మిగిలిన = budget == HB_BUDGET_DEFAULT ?
		       (int64_t) HB_BUDGET_GLYPH : budget;
    pixel_మిగిలిన = budget == HB_BUDGET_UNLIMITED ? budget :
		      (surface_దొంతర.నిడివి ? get_default_pixel_budget () :
					      (int64_t) HB_BUDGET_RASTER_PIXELS);
  }

  /* Returns whether the pixel operation should proceed.  Allows a single
   * overshoot past zero; callers skip work once the budget is spent. */
  తార్కిక charge_work (int64_t work)
  {
    ఒకవేళ (unlikely (pixel_మిగిలిన < 0)) తిరిగి అబద్ధం;
    pixel_మిగిలిన -= work;
    తిరిగి నిజం;
  }

  /* Reject work that would exceed the remaining budget before it starts. */
  తార్కిక precharge_work (int64_t work)
  {
    ఒకవేళ (unlikely (pixel_మిగిలిన < work))
    {
      pixel_మిగిలిన = -1;
      తిరిగి అబద్ధం;
    }
    pixel_మిగిలిన -= work;
    తిరిగి నిజం;
  }

  hb_raster_image_t *acquire_surface ()
  {
    hb_raster_image_t *img;
    ఒకవేళ (surface_cache.నిడివి)
      img = surface_cache.తీయి ();
    లేకపోతే
    {
      img = hb_raster_image_create_or_fail ();
      ఒకవేళ (unlikely (!img)) తిరిగి శూన్యసూచిక;
    }

    ఒకవేళ (unlikely (!img->configure (HB_RASTER_FORMAT_BGRA32, fixed_extents)))
    {
      hb_raster_image_destroy (img);
      తిరిగి శూన్యసూచిక;
    }
    img->clear ();
    తిరిగి img;
  }

  శూన్య release_surface (hb_raster_image_t *img)
  {
    ఒకవేళ (!surface_cache.నెట్టు_or_fail (img))
      hb_raster_image_destroy (img);
  }

  hb_raster_clip_t acquire_clip (గుర్తులేని w, గుర్తులేని h)
  {
    hb_raster_clip_t clip;
    ఒకవేళ (clip_cache.నిడివి)
      clip = clip_cache.తీయి ();
    clip.వెడల్పు = w;
    clip.ఎత్తు = h;
    clip.stride = (w + 3u) & ~3u;
    clip.is_rect = అబద్ధం;
    clip.rect_x0 = clip.rect_y0 = 0;
    clip.rect_x1 = clip.rect_y1 = 0;
    clip.min_x = clip.min_y = clip.max_x = clip.max_y = 0;
    తిరిగి clip;
  }

  శూన్య release_clip (hb_raster_clip_t &&clip)
  {
    ఒకవేళ (clip.alpha.arrayZ)
      clip_cache.push (std::move (clip));
  }

  శూన్య release_all_clips ()
  {
    అయితే (clip_దొంతర.నిడివి)
      release_clip (clip_దొంతర.తీయి ());
  }

  hb_raster_image_t *current_surface ()
  {
    తిరిగి surface_దొంతర.నిడివి ? surface_దొంతర.తోక () : శూన్యసూచిక;
  }

  hb_raster_clip_t &current_clip ()
  {
    తిరిగి clip_దొంతర.తోక ();
  }

  hb_transform_t<> current_transform ()
  {
    ఒకవేళ (unlikely (!transform_దొంతర.నిడివి))
      తిరిగి {1, 0, 0, 1, 0, 0};
    తిరిగి transform_దొంతర.తోక ();
  }

  శూన్య apply_scale_గుణకం (hb_transform_t<> &t) స్థిరరాశి
  {
    t.xx /= x_scale_గుణకం;
    t.xy /= x_scale_గుణకం;
    t.x0 /= x_scale_గుణకం;
    t.yx /= y_scale_గుణకం;
    t.yy /= y_scale_గుణకం;
    t.y0 /= y_scale_గుణకం;
  }

  hb_transform_t<> current_effective_transform ()
  {
    hb_transform_t<> t = current_transform ();
    apply_scale_గుణకం (t);
    తిరిగి t;
  }

  తార్కిక fetch_color_stops (hb_color_పంక్తి_t *color_పంక్తి)
  {
    గుర్తులేని లెక్క = hb_color_పంక్తి_get_color_stops (color_పంక్తి, 0, శూన్యసూచిక, శూన్యసూచిక);
    ఒకవేళ (unlikely (!లెక్క || !scratch_color_stops.resize (లెక్క)))
    {
      scratch_color_stops.resize (0);
      తిరిగి అబద్ధం;
    }
    hb_color_పంక్తి_get_color_stops (color_పంక్తి, 0, &లెక్క, scratch_color_stops.arrayZ);
    తిరిగి నిజం;
  }
};


#అయితే_ముగింపు /* HB_RASTER_PAINT_HH */
