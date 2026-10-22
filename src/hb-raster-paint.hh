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

#അന്തര്ഭാവ "hb.hh"

#അന്തര്ഭാവ "hb-raster-image.hh"
#അന്തര്ഭാവ "hb-geometry.hh"

/* hb_raster_clip_t — alpha mask for clipping */
ഘടന hb_raster_clip_t
{
  hb_vector_t<uint8_t> alpha;	/* A8 mask, same extents as root surface */
  unsigned width  = 0;
  unsigned height = 0;
  unsigned stride = 0;

  /* Fast path: simple rectangle (no alpha buffer needed) */
  തർക്കം is_rect = സത്യം;
  പൂർണ്ണസംഖ്യ rect_x0 = 0, rect_y0 = 0;
  പൂർണ്ണസംഖ്യ rect_x1 = 0, rect_y1 = 0;

  /* Bounding box of non-zero alpha region (valid for both rect and mask) */
  unsigned min_x = 0, min_y = 0;
  unsigned max_x = 0, max_y = 0;

  ശൂന്യം init_full (unsigned w, unsigned h)
  {
    width = w;
    height = h;
    stride = (w + 3u) & ~3u;
    is_rect = സത്യം;
    rect_x0 = 0;
    rect_y0 = 0;
    rect_x1 = (പൂർണ്ണസംഖ്യ) w;
    rect_y1 = (പൂർണ്ണസംഖ്യ) h;
    min_x = 0;
    min_y = 0;
    max_x = w;
    max_y = h;
  }

  ശൂന്യം update_bounds_from_rect ()
  {
    min_x = (unsigned) hb_max (rect_x0, 0);
    min_y = (unsigned) hb_max (rect_y0, 0);
    max_x = (unsigned) hb_max (hb_min (rect_x1, (പൂർണ്ണസംഖ്യ) width), 0);
    max_y = (unsigned) hb_max (hb_min (rect_y1, (പൂർണ്ണസംഖ്യ) height), 0);
  }

  uint8_t get_alpha (unsigned x, unsigned y) സ്ഥിരം
  {
    എങ്കിൽ (is_rect)
      തിരികെ ((പൂർണ്ണസംഖ്യ) x >= rect_x0 && (പൂർണ്ണസംഖ്യ) x < rect_x1 &&
	      (പൂർണ്ണസംഖ്യ) y >= rect_y0 && (പൂർണ്ണസംഖ്യ) y < rect_y1) ? 255 : 0;
    എങ്കിൽ (x >= width || y >= height) തിരികെ 0;
    എങ്കിൽ (stride < width) തിരികെ 0;
    എങ്കിൽ (height && stride > (size_t) -1 / height) തിരികെ 0;
    size_t idx = (size_t) y * stride + x;
    എങ്കിൽ (!alpha.arrayZ || idx >= alpha.length) തിരികെ 0;
    തിരികെ alpha[idx];
  }

  തർക്കം has_valid_alpha_mask () സ്ഥിരം
  {
    എങ്കിൽ (is_rect)
      തിരികെ സത്യം;
    എങ്കിൽ (!alpha.arrayZ || stride < width)
      തിരികെ അസത്യം;
    എങ്കിൽ (height && stride > (size_t) -1 / height)
      തിരികെ അസത്യം;
    തിരികെ (size_t) stride * height <= alpha.length;
  }
};


/* hb_raster_paint_t — color glyph paint context */
ഘടന hb_raster_paint_t
{
  hb_object_header_t header;

  /* Configuration */
  hb_transform_t<>    base_transform     = {1, 0, 0, 1, 0, 0};
  ദശാംശം               x_scale_factor     = 1.f;
  ദശാംശം               y_scale_factor     = 1.f;
  hb_raster_extents_t fixed_extents      = {};
  തർക്കം                has_extents  = അസത്യം;
  hb_color_t          foreground         = HB_COLOR (0, 0, 0, 255);
  hb_color_t          background         = HB_COLOR (0, 0, 0, 0);
  unsigned            palette            = 0;
  hb_map_t           *custom_palette     = nullptr;

  /* Stacks */
  hb_vector_t<hb_transform_t<>>     transform_അട്ടി;
  hb_vector_t<hb_raster_clip_t>     clip_അട്ടി;
  hb_vector_t<hb_raster_clip_t>     clip_cache;
  hb_vector_t<hb_raster_image_t *>  surface_അട്ടി;

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

  int64_t get_default_pixel_budget () സ്ഥിരം
  {
    തിരികെ hb_max ((int64_t) HB_BUDGET_RASTER_PIXELS,
		   (int64_t) HB_BUDGET_RASTER_PAINT_PASSES *
		   fixed_extents.width * fixed_extents.height);
  }

  ശൂന്യം recharge_budget ()
  {
    budget_remaining = budget == HB_BUDGET_DEFAULT ?
		       (int64_t) HB_BUDGET_GLYPH : budget;
    pixel_remaining = budget == HB_BUDGET_UNLIMITED ? budget :
		      (surface_അട്ടി.length ? get_default_pixel_budget () :
					      (int64_t) HB_BUDGET_RASTER_PIXELS);
  }

  /* Returns whether the pixel operation should proceed.  Allows a single
   * overshoot past zero; callers skip work once the budget is spent. */
  തർക്കം charge_work (int64_t work)
  {
    എങ്കിൽ (unlikely (pixel_remaining < 0)) തിരികെ അസത്യം;
    pixel_remaining -= work;
    തിരികെ സത്യം;
  }

  /* Reject work that would exceed the remaining budget before it starts. */
  തർക്കം precharge_work (int64_t work)
  {
    എങ്കിൽ (unlikely (pixel_remaining < work))
    {
      pixel_remaining = -1;
      തിരികെ അസത്യം;
    }
    pixel_remaining -= work;
    തിരികെ സത്യം;
  }

  hb_raster_image_t *acquire_surface ()
  {
    hb_raster_image_t *img;
    എങ്കിൽ (surface_cache.length)
      img = surface_cache.എടുക്കുക ();
    അല്ലെങ്കിൽ
    {
      img = hb_raster_image_create_or_fail ();
      എങ്കിൽ (unlikely (!img)) തിരികെ nullptr;
    }

    എങ്കിൽ (unlikely (!img->configure (HB_RASTER_FORMAT_BGRA32, fixed_extents)))
    {
      hb_raster_image_destroy (img);
      തിരികെ nullptr;
    }
    img->clear ();
    തിരികെ img;
  }

  ശൂന്യം release_surface (hb_raster_image_t *img)
  {
    എങ്കിൽ (!surface_cache.തള്ളുക_or_fail (img))
      hb_raster_image_destroy (img);
  }

  hb_raster_clip_t acquire_clip (unsigned w, unsigned h)
  {
    hb_raster_clip_t clip;
    എങ്കിൽ (clip_cache.length)
      clip = clip_cache.എടുക്കുക ();
    clip.width = w;
    clip.height = h;
    clip.stride = (w + 3u) & ~3u;
    clip.is_rect = അസത്യം;
    clip.rect_x0 = clip.rect_y0 = 0;
    clip.rect_x1 = clip.rect_y1 = 0;
    clip.min_x = clip.min_y = clip.max_x = clip.max_y = 0;
    തിരികെ clip;
  }

  ശൂന്യം release_clip (hb_raster_clip_t &&clip)
  {
    എങ്കിൽ (clip.alpha.arrayZ)
      clip_cache.push (std::move (clip));
  }

  ശൂന്യം release_all_clips ()
  {
    ആകുമ്പോൾ (clip_അട്ടി.length)
      release_clip (clip_അട്ടി.എടുക്കുക ());
  }

  hb_raster_image_t *current_surface ()
  {
    തിരികെ surface_അട്ടി.length ? surface_അട്ടി.tail () : nullptr;
  }

  hb_raster_clip_t &current_clip ()
  {
    തിരികെ clip_അട്ടി.tail ();
  }

  hb_transform_t<> current_transform ()
  {
    എങ്കിൽ (unlikely (!transform_അട്ടി.length))
      തിരികെ {1, 0, 0, 1, 0, 0};
    തിരികെ transform_അട്ടി.tail ();
  }

  ശൂന്യം apply_scale_factor (hb_transform_t<> &t) സ്ഥിരം
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
    തിരികെ t;
  }

  തർക്കം fetch_color_stops (hb_color_വരി_t *color_വരി)
  {
    unsigned എണ്ണം = hb_color_വരി_get_color_stops (color_വരി, 0, nullptr, nullptr);
    എങ്കിൽ (unlikely (!എണ്ണം || !scratch_color_stops.resize (എണ്ണം)))
    {
      scratch_color_stops.resize (0);
      തിരികെ അസത്യം;
    }
    hb_color_വരി_get_color_stops (color_വരി, 0, &എണ്ണം, scratch_color_stops.arrayZ);
    തിരികെ സത്യം;
  }
};


#endif /* HB_RASTER_PAINT_HH */
