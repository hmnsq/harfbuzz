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

#अगर_अघोषित HB_RASTER_PAINT_HH
#घोषणा HB_RASTER_PAINT_HH

#समावेश "hb.hh"

#समावेश "hb-raster-image.hh"
#समावेश "hb-geometry.hh"

/* hb_raster_clip_t — alpha mask for clipping */
काष्ठा hb_raster_clip_t
{
  hb_vector_t<uint8_t> alpha;	/* A8 mask, same extents as root surface */
  अचिन्हित width  = 0;
  अचिन्हित height = 0;
  अचिन्हित stride = 0;

  /* Fast path: simple rectangle (no alpha buffer needed) */
  bool is_rect = true;
  पूर्णांक rect_x0 = 0, rect_y0 = 0;
  पूर्णांक rect_x1 = 0, rect_y1 = 0;

  /* Bounding box of non-zero alpha region (valid for both rect and mask) */
  अचिन्हित min_x = 0, min_y = 0;
  अचिन्हित max_x = 0, max_y = 0;

  व्योम init_full (अचिन्हित w, अचिन्हित h)
  {
    width = w;
    height = h;
    stride = (w + 3u) & ~3u;
    is_rect = true;
    rect_x0 = 0;
    rect_y0 = 0;
    rect_x1 = (पूर्णांक) w;
    rect_y1 = (पूर्णांक) h;
    min_x = 0;
    min_y = 0;
    max_x = w;
    max_y = h;
  }

  व्योम update_bounds_from_rect ()
  {
    min_x = (अचिन्हित) hb_max (rect_x0, 0);
    min_y = (अचिन्हित) hb_max (rect_y0, 0);
    max_x = (अचिन्हित) hb_max (hb_min (rect_x1, (पूर्णांक) width), 0);
    max_y = (अचिन्हित) hb_max (hb_min (rect_y1, (पूर्णांक) height), 0);
  }

  uint8_t get_alpha (अचिन्हित x, अचिन्हित y) स्थिर
  {
    अगर (is_rect)
      वापस ((पूर्णांक) x >= rect_x0 && (पूर्णांक) x < rect_x1 &&
	      (पूर्णांक) y >= rect_y0 && (पूर्णांक) y < rect_y1) ? 255 : 0;
    अगर (x >= width || y >= height) वापस 0;
    अगर (stride < width) वापस 0;
    अगर (height && stride > (माप_प्रकार) -1 / height) वापस 0;
    माप_प्रकार idx = (माप_प्रकार) y * stride + x;
    अगर (!alpha.arrayZ || idx >= alpha.लंबाई) वापस 0;
    वापस alpha[idx];
  }

  bool has_valid_alpha_mask () स्थिर
  {
    अगर (is_rect)
      वापस true;
    अगर (!alpha.arrayZ || stride < width)
      वापस false;
    अगर (height && stride > (माप_प्रकार) -1 / height)
      वापस false;
    वापस (माप_प्रकार) stride * height <= alpha.लंबाई;
  }
};


/* hb_raster_paint_t — color glyph paint context */
काष्ठा hb_raster_paint_t
{
  hb_object_header_t header;

  /* Configuration */
  hb_transform_t<>    base_transform     = {1, 0, 0, 1, 0, 0};
  भग्न               x_scale_factor     = 1.f;
  भग्न               y_scale_factor     = 1.f;
  hb_raster_extents_t fixed_extents      = {};
  bool                has_extents  = false;
  hb_color_t          foreground         = HB_COLOR (0, 0, 0, 255);
  hb_color_t          background         = HB_COLOR (0, 0, 0, 0);
  अचिन्हित            palette            = 0;
  hb_map_t           *custom_palette     = nullptr;

  /* Stacks */
  hb_vector_t<hb_transform_t<>>     transform_ढेर;
  hb_vector_t<hb_raster_clip_t>     clip_ढेर;
  hb_vector_t<hb_raster_clip_t>     clip_cache;
  hb_vector_t<hb_raster_image_t *>  surface_ढेर;

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

  int64_t get_default_pixel_budget () स्थिर
  {
    वापस hb_max ((int64_t) HB_BUDGET_RASTER_PIXELS,
		   (int64_t) HB_BUDGET_RASTER_PAINT_PASSES *
		   fixed_extents.width * fixed_extents.height);
  }

  व्योम recharge_budget ()
  {
    budget_remaining = budget == HB_BUDGET_DEFAULT ?
		       (int64_t) HB_BUDGET_GLYPH : budget;
    pixel_remaining = budget == HB_BUDGET_UNLIMITED ? budget :
		      (surface_ढेर.लंबाई ? get_default_pixel_budget () :
					      (int64_t) HB_BUDGET_RASTER_PIXELS);
  }

  /* Returns whether the pixel operation should proceed.  Allows a single
   * overshoot past zero; callers skip work once the budget is spent. */
  bool charge_work (int64_t work)
  {
    अगर (unlikely (pixel_remaining < 0)) वापस false;
    pixel_remaining -= work;
    वापस true;
  }

  /* Reject work that would exceed the remaining budget before it starts. */
  bool precharge_work (int64_t work)
  {
    अगर (unlikely (pixel_remaining < work))
    {
      pixel_remaining = -1;
      वापस false;
    }
    pixel_remaining -= work;
    वापस true;
  }

  hb_raster_image_t *acquire_surface ()
  {
    hb_raster_image_t *img;
    अगर (surface_cache.लंबाई)
      img = surface_cache.निकालो ();
    अन्यथा
    {
      img = hb_raster_image_create_or_fail ();
      अगर (unlikely (!img)) वापस nullptr;
    }

    अगर (unlikely (!img->configure (HB_RASTER_FORMAT_BGRA32, fixed_extents)))
    {
      hb_raster_image_destroy (img);
      वापस nullptr;
    }
    img->सफा ();
    वापस img;
  }

  व्योम release_surface (hb_raster_image_t *img)
  {
    अगर (!surface_cache.धकेलो_or_fail (img))
      hb_raster_image_destroy (img);
  }

  hb_raster_clip_t acquire_clip (अचिन्हित w, अचिन्हित h)
  {
    hb_raster_clip_t clip;
    अगर (clip_cache.लंबाई)
      clip = clip_cache.निकालो ();
    clip.width = w;
    clip.height = h;
    clip.stride = (w + 3u) & ~3u;
    clip.is_rect = false;
    clip.rect_x0 = clip.rect_y0 = 0;
    clip.rect_x1 = clip.rect_y1 = 0;
    clip.min_x = clip.min_y = clip.max_x = clip.max_y = 0;
    वापस clip;
  }

  व्योम release_clip (hb_raster_clip_t &&clip)
  {
    अगर (clip.alpha.arrayZ)
      clip_cache.push (std::move (clip));
  }

  व्योम release_all_clips ()
  {
    जबतक (clip_ढेर.लंबाई)
      release_clip (clip_ढेर.निकालो ());
  }

  hb_raster_image_t *current_surface ()
  {
    वापस surface_ढेर.लंबाई ? surface_ढेर.पुच्छ () : nullptr;
  }

  hb_raster_clip_t &current_clip ()
  {
    वापस clip_ढेर.पुच्छ ();
  }

  hb_transform_t<> current_transform ()
  {
    अगर (unlikely (!transform_ढेर.लंबाई))
      वापस {1, 0, 0, 1, 0, 0};
    वापस transform_ढेर.पुच्छ ();
  }

  व्योम apply_scale_factor (hb_transform_t<> &t) स्थिर
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
    वापस t;
  }

  bool fetch_color_stops (hb_color_पंक्ति_t *color_पंक्ति)
  {
    अचिन्हित गिनती = hb_color_पंक्ति_get_color_stops (color_पंक्ति, 0, nullptr, nullptr);
    अगर (unlikely (!गिनती || !scratch_color_stops.resize (गिनती)))
    {
      scratch_color_stops.resize (0);
      वापस false;
    }
    hb_color_पंक्ति_get_color_stops (color_पंक्ति, 0, &गिनती, scratch_color_stops.arrayZ);
    वापस true;
  }
};


#पूर्ण_अगर /* HB_RASTER_PAINT_HH */
