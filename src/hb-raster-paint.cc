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

#అన్తర్భావ "hb.hh"

#అన్తర్భావ "hb-raster-paint.hh"
#అన్తర్భావ "hb-machinery.hh"
#అన్తర్భావ "hb-paint.hh"

#అన్తర్భావ <math.h>


/*
 * Pixel helpers (paint-specific)
 */

/* Convert unpremultiplied hb_color_t (BGRA order) to premultiplied BGRA32 pixel. */
స్థిర అంతఃవరుస uint32_t
color_గమ్యం_premul_pixel (hb_color_t color)
{
  uint8_t a = hb_color_get_alpha (color);
  uint8_t r = hb_raster_div255 (hb_color_get_red (color) * a);
  uint8_t g = hb_raster_div255 (hb_color_get_green (color) * a);
  uint8_t b = hb_raster_div255 (hb_color_get_blue (color) * a);
  తిరిగి (uint32_t) b | ((uint32_t) g << 8) | ((uint32_t) r << 16) | ((uint32_t) a << 24);
}

/* Bilinear sample from a premultiplied BGRA32 image. */
స్థిర అంతఃవరుస uint32_t
hb_raster_sample_bilinear_premul (స్థిరరాశి hb_packed_t<uint32_t> *src,
				  గుర్తులేని వెడల్పు,
				  గుర్తులేని ఎత్తు,
				  దశాంశం x,
				  దశాంశం y)
{
  పూర్ణాంకం x0 = (పూర్ణాంకం) floorf (x);
  పూర్ణాంకం y0 = (పూర్ణాంకం) floorf (y);
  పూర్ణాంకం x1 = x0 + 1;
  పూర్ణాంకం y1 = y0 + 1;
  ఒకవేళ (x1 >= (పూర్ణాంకం) వెడల్పు) x1 = (పూర్ణాంకం) వెడల్పు - 1;
  ఒకవేళ (y1 >= (పూర్ణాంకం) ఎత్తు) y1 = (పూర్ణాంకం) ఎత్తు - 1;

  దశాంశం tx = x - x0;
  దశాంశం ty = y - y0;
  దశాంశం omtx = 1.f - tx;
  దశాంశం omty = 1.f - ty;

  uint32_t p00 = (uint32_t) src[(size_t) y0 * వెడల్పు + (size_t) x0];
  uint32_t p10 = (uint32_t) src[(size_t) y0 * వెడల్పు + (size_t) x1];
  uint32_t p01 = (uint32_t) src[(size_t) y1 * వెడల్పు + (size_t) x0];
  uint32_t p11 = (uint32_t) src[(size_t) y1 * వెడల్పు + (size_t) x1];

  దశాంశం w00 = omtx * omty;
  దశాంశం w10 = tx * omty;
  దశాంశం w01 = omtx * ty;
  దశాంశం w11 = tx * ty;

  దశాంశం b = ((p00 >>  0) & 0xff) * w00 + ((p10 >>  0) & 0xff) * w10 +
	    ((p01 >>  0) & 0xff) * w01 + ((p11 >>  0) & 0xff) * w11;
  దశాంశం g = ((p00 >>  8) & 0xff) * w00 + ((p10 >>  8) & 0xff) * w10 +
	    ((p01 >>  8) & 0xff) * w01 + ((p11 >>  8) & 0xff) * w11;
  దశాంశం r = ((p00 >> 16) & 0xff) * w00 + ((p10 >> 16) & 0xff) * w10 +
	    ((p01 >> 16) & 0xff) * w01 + ((p11 >> 16) & 0xff) * w11;
  దశాంశం a = ((p00 >> 24) & 0xff) * w00 + ((p10 >> 24) & 0xff) * w10 +
	    ((p01 >> 24) & 0xff) * w01 + ((p11 >> 24) & 0xff) * w11;

  తిరిగి (uint32_t) (b + 0.5f)
       | ((uint32_t) (g + 0.5f) << 8)
       | ((uint32_t) (r + 0.5f) << 16)
       | ((uint32_t) (a + 0.5f) << 24);
}


/*
 * Paint callbacks
 */

/* Lazy initialization: set up root surface, initial clip and transform.
 * Called from every paint callback that needs state.
 * hb_font_paint_glyph() does NOT wrap with push/pop_transform,
 * so the first callback could be push_clip_glyph or paint_color. */
స్థిర శూన్య
ensure_initialized (hb_raster_paint_t *c)
{
  ఒకవేళ (c->surface_దొంతర.నిడివి) తిరిగి;

  /* A failed stack push (OOM) leaves the vector in a sticky error
   * state, so initialization can never succeed; bail out cheaply
   * instead of re-paying the surface acquire-and-clear below on
   * every subsequent paint callback. */
  ఒకవేళ (unlikely (c->surface_దొంతర.లోపల_error () || c->clip_దొంతర.లోపల_error ()))
    తిరిగి;

  /* acquire_surface() clears a full surface; charge its area, so
   * repeated failed initialization attempts cannot re-pay it
   * indefinitely.  Successful initialization re-arms the session
   * budget below, making the charge a no-op for normal sessions. */
  ఒకవేళ (unlikely (!c->charge_work ((int64_t) c->fixed_extents.వెడల్పు *
				 c->fixed_extents.ఎత్తు)))
    తిరిగి;

  /* Root surface */
  hb_raster_image_t *మూలం = c->acquire_surface ();
  ఒకవేళ (unlikely (!మూలం)) తిరిగి;

  ఒకవేళ (hb_color_get_alpha (c->background))
  {
    uint32_t bg = color_గమ్యం_premul_pixel (c->background);
    hb_raster_extents_t ext;
    hb_raster_image_get_extents (మూలం, &ext);
    స్థిరరాశి uint8_t *buf = hb_raster_image_get_బఫరు (మూలం);
    కొరకు (గుర్తులేని y = 0; y < ext.ఎత్తు; y++)
    {
      hb_packed_t<uint32_t> *అడ్డువరుస =
	(hb_packed_t<uint32_t> *) (buf + (size_t) y * ext.stride);
      కొరకు (గుర్తులేని x = 0; x < ext.వెడల్పు; x++)
	అడ్డువరుస[x] = hb_packed_t<uint32_t> (bg);
    }
  }

  ఒకవేళ (unlikely (!c->surface_దొంతర.నెట్టు_or_fail (మూలం)))
  {
    c->release_surface (మూలం);
    తిరిగి;
  }

  /* Resolve the surface-sized pixel budget now that the surface size is
   * known.  An unlimited policy disables the pixel budget, so leave it.
   * The outline budget is re-resolved to the same value it already holds
   * (nothing has been drawn yet). */
  ఒకవేళ (c->budget != HB_BUDGET_UNLIMITED)
    c->recharge_budget ();

  /* Initial clip: full coverage rectangle */
  hb_raster_clip_t clip;
  clip.init_నిండిన (c->fixed_extents.వెడల్పు, c->fixed_extents.ఎత్తు);
  ఒకవేళ (unlikely (!c->clip_దొంతర.నెట్టు_or_fail (std::move (clip))))
  {
    c->release_surface (c->surface_దొంతర.తీయి ());
    తిరిగి;
  }
}

స్థిర శూన్య
hb_raster_paint_నెట్టు_transform (hb_paint_funcs_t *pfuncs HB_వాడని,
				శూన్య *paint_దత్తాంశం,
				దశాంశం xx, దశాంశం yx,
				దశాంశం xy, దశాంశం yy,
				దశాంశం dx, దశాంశం dy,
				శూన్య *user_దత్తాంశం HB_వాడని)
{
  hb_raster_paint_t *c = (hb_raster_paint_t *) paint_దత్తాంశం;

  ensure_initialized (c);

  hb_transform_t<> t = c->current_transform ();
  t.multiply ({xx, yx, xy, yy, dx, dy});
  (శూన్య) c->transform_దొంతర.push (t);
}

స్థిర శూన్య
hb_raster_paint_తీయి_transform (hb_paint_funcs_t *pfuncs HB_వాడని,
			       శూన్య *paint_దత్తాంశం,
			       శూన్య *user_దత్తాంశం HB_వాడని)
{
  hb_raster_paint_t *c = (hb_raster_paint_t *) paint_దత్తాంశం;
  ఒకవేళ (c->transform_దొంతర.నిడివి)
    c->transform_దొంతర.తీయి ();
}

స్థిర hb_bool_t
hb_raster_paint_color_glyph (hb_paint_funcs_t *pfuncs HB_వాడని,
			     శూన్య *paint_దత్తాంశం HB_వాడని,
			     hb_codepoint_t glyph HB_వాడని,
			     hb_font_t *font HB_వాడని,
			     శూన్య *user_దత్తాంశం HB_వాడని)
{
  తిరిగి అబద్ధం;
}

రకనిర్వచనం శూన్య (*hb_raster_paint_clip_ముసుగు_emit_t) (hb_raster_draw_t *rdr, శూన్య *user_దత్తాంశం);

/* Attach the surface box to @rdr and seed its outline budget before
 * outlines are drawn into it: curves outside the surface collapse to
 * their chord, and outline traversal and curve flattening charge the
 * seeded budget.  hb_raster_paint_readback_budget() pulls the remainder
 * back into the session afterwards. */
స్థిర శూన్య
hb_raster_paint_attach_clip_rdr (hb_raster_paint_t *c,
				 hb_raster_draw_t *rdr,
				 స్థిరరాశి hb_raster_image_t *surf)
{
  hb_raster_draw_సమితి_clip_box (rdr,
			       (దశాంశం) surf->extents.x_origin,
			       (దశాంశం) surf->extents.y_origin,
			       (దశాంశం) surf->extents.x_origin + (దశాంశం) surf->extents.వెడల్పు,
			       (దశాంశం) surf->extents.y_origin + (దశాంశం) surf->extents.ఎత్తు);
  hb_draw_సమితి_budget (hb_raster_draw_get_funcs (rdr), rdr, c->budget_మిగిలిన);
}

/* Pull the outline budget the rasterizer has consumed back into the
 * session, so it aggregates across every glyph in the paint walk. */
స్థిర శూన్య
hb_raster_paint_readback_budget (hb_raster_paint_t *c,
				 hb_raster_draw_t *rdr)
{
  c->budget_మిగిలిన = hb_draw_get_budget_మిగిలిన (hb_raster_draw_get_funcs (rdr), rdr);
}

స్థిర శూన్య
hb_raster_paint_నెట్టు_ఖాళీలు_clip (hb_raster_paint_t *c, గుర్తులేని w, గుర్తులేని h)
{
  hb_raster_clip_t new_clip = c->acquire_clip (w, h);
  new_clip.init_నిండిన (w, h);
  new_clip.is_rect = నిజం;
  new_clip.rect_x0 = new_clip.rect_y0 = 0;
  new_clip.rect_x1 = new_clip.rect_y1 = 0;
  new_clip.min_x = new_clip.min_y = new_clip.max_x = new_clip.max_y = 0;
  (శూన్య) c->clip_దొంతర.push (std::move (new_clip));
}

స్థిర తార్కిక
hb_raster_paint_intersect_ముసుగు (స్థిరరాశి hb_raster_image_t *surf,
				స్థిరరాశి hb_raster_extents_t &ముసుగు_ext,
				స్థిరరాశి hb_raster_clip_t &clip,
				గుర్తులేని w, గుర్తులేని h,
				int64_t *ముసుగు_x0, int64_t *ముసుగు_y0,
				గుర్తులేని *ix0, గుర్తులేని *iy0,
				గుర్తులేని *ix1, గుర్తులేని *iy1)
{
  /* Convert mask extents from surface coordinates to clip-buffer coordinates. */
  int64_t mx0 = (int64_t) ముసుగు_ext.x_origin - (int64_t) surf->extents.x_origin;
  int64_t my0 = (int64_t) ముసుగు_ext.y_origin - (int64_t) surf->extents.y_origin;

  int64_t x0 = hb_max ((int64_t) clip.min_x, hb_max (mx0, (int64_t) 0));
  int64_t y0 = hb_max ((int64_t) clip.min_y, hb_max (my0, (int64_t) 0));
  int64_t x1 = hb_min ((int64_t) clip.max_x, hb_min (mx0 + ముసుగు_ext.వెడల్పు, (int64_t) w));
  int64_t y1 = hb_min ((int64_t) clip.max_y, hb_min (my0 + ముసుగు_ext.ఎత్తు, (int64_t) h));

  ఒకవేళ (x0 >= x1 || y0 >= y1) తిరిగి అబద్ధం;

  *ముసుగు_x0 = mx0;
  *ముసుగు_y0 = my0;
  *ix0 = (గుర్తులేని) x0;
  *iy0 = (గుర్తులేని) y0;
  *ix1 = (గుర్తులేని) x1;
  *iy1 = (గుర్తులేని) y1;
  తిరిగి నిజం;
}

/* Render whatever edges have been accumulated into @rdr and
 * push the result as a new clip on the stack, intersecting
 * with the existing clip.  Used by the emitter-based clip
 * helpers (push_clip_glyph) and by push_clip_path_end once
 * the caller has drawn the path into @rdr. */
స్థిర శూన్య
hb_raster_paint_finalize_మార్గం_clip (hb_raster_paint_t *c,
				    hb_raster_draw_t *rdr,
				    hb_raster_image_t *surf,
				    గుర్తులేని w, గుర్తులేని h)
{
  ఒకవేళ (unlikely (!c->precharge_work (hb_raster_draw_get_pixel_work (rdr, h, w))))
  {
    hb_raster_draw_clear (rdr);
    hb_raster_paint_నెట్టు_ఖాళీలు_clip (c, w, h);
    తిరిగి;
  }

  hb_raster_image_t *ముసుగు_img = hb_raster_draw_render (rdr);

  ఒకవేళ (unlikely (!ముసుగు_img))
  {
    hb_raster_paint_నెట్టు_ఖాళీలు_clip (c, w, h);
    తిరిగి;
  }

  /* Charge the mask render (its own area; may exceed the surface) plus
   * the clip-buffer pass below. */
  hb_raster_extents_t ముసుగు_render_ext;
  hb_raster_image_get_extents (ముసుగు_img, &ముసుగు_render_ext);
  ఒకవేళ (unlikely (!c->charge_work ((int64_t) ముసుగు_render_ext.వెడల్పు * ముసుగు_render_ext.ఎత్తు +
				 (int64_t) w * h)))
  {
    hb_raster_draw_recycle_image (rdr, ముసుగు_img);
    hb_raster_paint_నెట్టు_ఖాళీలు_clip (c, w, h);
    తిరిగి;
  }

  hb_raster_clip_t new_clip = c->acquire_clip (w, h);

  /* Allocate alpha buffer and intersect with previous clip */
  size_t clip_కొలత = (size_t) new_clip.stride * h;
  ఒకవేళ (unlikely (clip_కొలత > HB_RASTER_MAX_బఫరు_కొలత ||
                !new_clip.alpha.resize ((గుర్తులేని) clip_కొలత)))
  {
    hb_raster_draw_recycle_image (rdr, ముసుగు_img);
    hb_raster_paint_నెట్టు_ఖాళీలు_clip (c, w, h);
    తిరిగి;
  }
  hb_memset (new_clip.alpha.arrayZ, 0, (గుర్తులేని) clip_కొలత);

  స్థిరరాశి uint8_t *ముసుగు_buf = hb_raster_image_get_బఫరు (ముసుగు_img);
  hb_raster_extents_t ముసుగు_ext;
  hb_raster_image_get_extents (ముసుగు_img, &ముసుగు_ext);
  స్థిరరాశి hb_raster_clip_t &old_clip = c->current_clip ();
  ఒకవేళ (unlikely (!old_clip.has_valid_alpha_ముసుగు ()))
  {
    hb_raster_draw_recycle_image (rdr, ముసుగు_img);
    hb_raster_paint_నెట్టు_ఖాళీలు_clip (c, w, h);
    తిరిగి;
  }

  int64_t ముసుగు_x0, ముసుగు_y0;
  గుర్తులేని ix0, iy0, ix1, iy1;
  ఒకవేళ (!hb_raster_paint_intersect_ముసుగు (surf, ముసుగు_ext, old_clip, w, h,
				       &ముసుగు_x0, &ముసుగు_y0,
				       &ix0, &iy0, &ix1, &iy1))
  {
    hb_raster_draw_recycle_image (rdr, ముసుగు_img);
    hb_raster_paint_నెట్టు_ఖాళీలు_clip (c, w, h);
    తిరిగి;
  }

  new_clip.min_x = w; new_clip.min_y = h;
  new_clip.max_x = 0; new_clip.max_y = 0;

  ఒకవేళ (old_clip.is_rect)
  {
    కొరకు (గుర్తులేని y = iy0; y < iy1; y++)
    {
      స్థిరరాశి uint8_t *ముసుగు_అడ్డువరుస = ముసుగు_buf + (గుర్తులేని) ((int64_t) y - ముసుగు_y0) * ముసుగు_ext.stride;
      uint8_t *బహిర్గతం_అడ్డువరుస = new_clip.alpha.arrayZ + y * new_clip.stride;
      గుర్తులేని అడ్డువరుస_min = ix1;
      గుర్తులేని అడ్డువరుస_max = ix0;
      గుర్తులేని mx = (గుర్తులేని) ((int64_t) ix0 - ముసుగు_x0);
      కొరకు (గుర్తులేని x = ix0; x < ix1; x++)
      {
	uint8_t a = ముసుగు_అడ్డువరుస[mx++];
	బహిర్గతం_అడ్డువరుస[x] = a;
	ఒకవేళ (a && అడ్డువరుస_min == ix1)
	{
	  అడ్డువరుస_min = x;
	  అడ్డువరుస_max = x + 1;
	}
	లేకపోతే ఒకవేళ (a)
	  అడ్డువరుస_max = x + 1;
      }
      ఒకవేళ (అడ్డువరుస_min < అడ్డువరుస_max)
      {
	new_clip.min_x = hb_min (new_clip.min_x, అడ్డువరుస_min);
	new_clip.min_y = hb_min (new_clip.min_y, y);
	new_clip.max_x = hb_max (new_clip.max_x, అడ్డువరుస_max);
	new_clip.max_y = hb_max (new_clip.max_y, y + 1);
      }
    }
  }
  లేకపోతే
  {
    కొరకు (గుర్తులేని y = iy0; y < iy1; y++)
    {
      స్థిరరాశి uint8_t *old_అడ్డువరుస = old_clip.alpha.arrayZ + y * old_clip.stride;
      స్థిరరాశి uint8_t *ముసుగు_అడ్డువరుస = ముసుగు_buf + (గుర్తులేని) ((int64_t) y - ముసుగు_y0) * ముసుగు_ext.stride;
      uint8_t *బహిర్గతం_అడ్డువరుస = new_clip.alpha.arrayZ + y * new_clip.stride;
      గుర్తులేని అడ్డువరుస_min = ix1;
      గుర్తులేని అడ్డువరుస_max = ix0;
      కొరకు (గుర్తులేని x = ix0; x < ix1; x++)
      {
	గుర్తులేని mx = (గుర్తులేని) ((int64_t) x - ముసుగు_x0);
	uint8_t a = hb_raster_div255 (ముసుగు_అడ్డువరుస[mx] * old_అడ్డువరుస[x]);
	బహిర్గతం_అడ్డువరుస[x] = a;
	ఒకవేళ (a)
	{
	  అడ్డువరుస_min = hb_min (అడ్డువరుస_min, x);
	  అడ్డువరుస_max = x + 1;
	}
      }
      ఒకవేళ (అడ్డువరుస_min < అడ్డువరుస_max)
      {
	new_clip.min_x = hb_min (new_clip.min_x, అడ్డువరుస_min);
	new_clip.min_y = hb_min (new_clip.min_y, y);
	new_clip.max_x = hb_max (new_clip.max_x, అడ్డువరుస_max);
	new_clip.max_y = hb_max (new_clip.max_y, y + 1);
      }
    }
  }

  hb_raster_draw_recycle_image (rdr, ముసుగు_img);
  ఒకవేళ (unlikely (!c->clip_దొంతర.నెట్టు_or_fail (std::move (new_clip))))
    hb_raster_paint_నెట్టు_ఖాళీలు_clip (c, w, h);
}

స్థిర శూన్య
hb_raster_paint_నెట్టు_clip_from_emitter (hb_raster_paint_t *c,
					hb_raster_paint_clip_ముసుగు_emit_t emit,
					శూన్య *emit_దత్తాంశం)
{
  ensure_initialized (c);

  hb_raster_image_t *surf = c->current_surface ();
  ఒకవేళ (unlikely (!surf)) తిరిగి;

  గుర్తులేని w = surf->extents.వెడల్పు;
  గుర్తులేని h = surf->extents.ఎత్తు;

  /* Out of budget: skip the glyph-outline extraction entirely, so
   * per-glyph outline limits cannot multiply with the caller's
   * paint-graph traversal limits. */
  ఒకవేళ (unlikely (c->budget_మిగిలిన < 0))
  {
    hb_raster_paint_నెట్టు_ఖాళీలు_clip (c, w, h);
    తిరిగి;
  }

  hb_raster_draw_t *rdr = c->clip_rdr;
  hb_transform_t<> t = c->current_effective_transform ();
  hb_raster_draw_సమితి_transform (rdr, t.xx, t.yx, t.xy, t.yy, t.x0, t.y0);
  hb_raster_paint_attach_clip_rdr (c, rdr, surf);
  emit (rdr, emit_దత్తాంశం);
  hb_raster_paint_readback_budget (c, rdr);

  hb_raster_paint_finalize_మార్గం_clip (c, rdr, surf, w, h);
}

నిర్మాణం hb_raster_paint_glyph_clip_దత్తాంశం_t
{
  hb_codepoint_t glyph;
  hb_font_t *font;
};

స్థిర శూన్య
hb_raster_paint_emit_clip_glyph_ముసుగు (hb_raster_draw_t *rdr, శూన్య *user_దత్తాంశం)
{
  hb_raster_paint_glyph_clip_దత్తాంశం_t *దత్తాంశం = (hb_raster_paint_glyph_clip_దత్తాంశం_t *) user_దత్తాంశం;
  /* Let draw-render choose tight glyph extents; we map by mask origin below. */
  hb_font_draw_glyph (దత్తాంశం->font, దత్తాంశం->glyph, hb_raster_draw_get_funcs (rdr), rdr);
}

స్థిర శూన్య
hb_raster_paint_నెట్టు_clip_glyph (hb_paint_funcs_t *pfuncs HB_వాడని,
				 శూన్య *paint_దత్తాంశం,
				 hb_codepoint_t glyph,
				 hb_font_t *font,
				 శూన్య *user_దత్తాంశం HB_వాడని)
{
  hb_raster_paint_t *c = (hb_raster_paint_t *) paint_దత్తాంశం;
  hb_raster_paint_glyph_clip_దత్తాంశం_t దత్తాంశం = {glyph, font};
  hb_raster_paint_నెట్టు_clip_from_emitter (c, hb_raster_paint_emit_clip_glyph_ముసుగు, &దత్తాంశం);
}

స్థిర శూన్య
hb_raster_paint_నెట్టు_clip_rectangle (hb_paint_funcs_t *pfuncs HB_వాడని,
				     శూన్య *paint_దత్తాంశం,
				     దశాంశం xmin, దశాంశం ymin,
				     దశాంశం xmax, దశాంశం ymax,
				     శూన్య *user_దత్తాంశం HB_వాడని)
{
  hb_raster_paint_t *c = (hb_raster_paint_t *) paint_దత్తాంశం;

  ensure_initialized (c);

  ఒకవేళ (!c->surface_దొంతర.నిడివి) తిరిగి;

  hb_transform_t<> t = c->current_effective_transform ();

  hb_raster_image_t *surf = c->current_surface ();
  ఒకవేళ (unlikely (!surf)) తిరిగి;

  గుర్తులేని w = surf->extents.వెడల్పు;
  గుర్తులేని h = surf->extents.ఎత్తు;
  తార్కిక is_axis_aligned = (t.xy == 0.f && t.yx == 0.f);

  /* Transform the four corners to pixel space */
  దశాంశం cx[4], cy[4];
  cx[0] = xmin; cy[0] = ymin;
  cx[1] = xmax; cy[1] = ymin;
  cx[2] = xmax; cy[2] = ymax;
  cx[3] = xmin; cy[3] = ymax;
  కొరకు (గుర్తులేని i = 0; i < 4; i++)
    t.transform_బిందువు (cx[i], cy[i]);

  /* Compute bounding box in pixel coords */
  దశాంశం fmin_x = cx[0], fmin_y = cy[0], fmax_x = cx[0], fmax_y = cy[0];
  కొరకు (గుర్తులేని i = 1; i < 4; i++)
  {
    fmin_x = hb_min (fmin_x, cx[i]); fmin_y = hb_min (fmin_y, cy[i]);
    fmax_x = hb_max (fmax_x, cx[i]); fmax_y = hb_max (fmax_y, cy[i]);
  }

  /* A spec-legal but very large transform can push these bounds past the int
     range or to non-finite values; a bare float->int conversion would then be
     undefined.  Saturate through hb_clamp_to<> (which also maps NaN to
     INT32_MIN); the subtraction is done in double to avoid overflow. */
  పూర్ణాంకం px0 = hb_clamp_గమ్యం<int32_t> ((ద్విగుణ) floorf (fmin_x) - surf->extents.x_origin);
  పూర్ణాంకం py0 = hb_clamp_గమ్యం<int32_t> ((ద్విగుణ) floorf (fmin_y) - surf->extents.y_origin);
  పూర్ణాంకం px1 = hb_clamp_గమ్యం<int32_t> ((ద్విగుణ) ceilf  (fmax_x) - surf->extents.x_origin);
  పూర్ణాంకం py1 = hb_clamp_గమ్యం<int32_t> ((ద్విగుణ) ceilf  (fmax_y) - surf->extents.y_origin);

  /* Clamp to surface bounds */
  px0 = hb_max (px0, 0);
  py0 = hb_max (py0, 0);
  px1 = hb_min (px1, (పూర్ణాంకం) w);
  py1 = hb_min (py1, (పూర్ణాంకం) h);

  స్థిరరాశి hb_raster_clip_t &old_clip = c->current_clip ();
  ఒకవేళ (unlikely (!old_clip.has_valid_alpha_ముసుగు ()))
  {
    hb_raster_paint_నెట్టు_ఖాళీలు_clip (c, w, h);
    తిరిగి;
  }

  /* All paths below except rect-on-rect run a full clip-buffer pass. */
  ఒకవేళ (!(is_axis_aligned && old_clip.is_rect) &&
      unlikely (!c->charge_work ((int64_t) w * h)))
  {
    hb_raster_paint_నెట్టు_ఖాళీలు_clip (c, w, h);
    తిరిగి;
  }

  hb_raster_clip_t new_clip = c->acquire_clip (w, h);

  ఒకవేళ (is_axis_aligned && old_clip.is_rect)
  {
    /* Fast path: axis-aligned rect-on-rect intersection */
    new_clip.is_rect = నిజం;
    new_clip.rect_x0 = hb_max (px0, old_clip.rect_x0);
    new_clip.rect_y0 = hb_max (py0, old_clip.rect_y0);
    new_clip.rect_x1 = hb_min (px1, old_clip.rect_x1);
    new_clip.rect_y1 = hb_min (py1, old_clip.rect_y1);
    new_clip.update_bounds_from_rect ();
  }
  లేకపోతే ఒకవేళ (is_axis_aligned)
  {
    /* Fast path: intersect an axis-aligned rect with an existing alpha clip. */
    new_clip.is_rect = అబద్ధం;
    size_t clip_కొలత = (size_t) new_clip.stride * h;
    ఒకవేళ (unlikely (clip_కొలత > HB_RASTER_MAX_బఫరు_కొలత ||
                  !new_clip.alpha.resize ((గుర్తులేని) clip_కొలత)))
    {
      hb_raster_paint_నెట్టు_ఖాళీలు_clip (c, w, h);
      తిరిగి;
    }
    hb_memset (new_clip.alpha.arrayZ, 0, (గుర్తులేని) clip_కొలత);

    పూర్ణాంకం iy0_i = hb_max (py0, (పూర్ణాంకం) old_clip.min_y);
    పూర్ణాంకం iy1_i = hb_min (py1, (పూర్ణాంకం) old_clip.max_y);
    పూర్ణాంకం ix0_i = hb_max (px0, (పూర్ణాంకం) old_clip.min_x);
    పూర్ణాంకం ix1_i = hb_min (px1, (పూర్ణాంకం) old_clip.max_x);

    ఒకవేళ (ix0_i < ix1_i && iy0_i < iy1_i)
    {
      గుర్తులేని iy0 = (గుర్తులేని) iy0_i;
      గుర్తులేని iy1 = (గుర్తులేని) iy1_i;
      గుర్తులేని ix0 = (గుర్తులేని) ix0_i;
      గుర్తులేని ix1 = (గుర్తులేని) ix1_i;

      కొరకు (గుర్తులేని y = iy0; y < iy1; y++)
      {
        స్థిరరాశి uint8_t *old_అడ్డువరుస = old_clip.alpha.arrayZ + y * old_clip.stride;
        uint8_t *new_అడ్డువరుస = new_clip.alpha.arrayZ + y * new_clip.stride;
        hb_memcpy (new_అడ్డువరుస + ix0, old_అడ్డువరుస + ix0, ix1 - ix0);
      }
      new_clip.min_x = ix0;
      new_clip.min_y = iy0;
      new_clip.max_x = ix1;
      new_clip.max_y = iy1;
    }
  }
  లేకపోతే
  {
    /* General case: rasterize transformed quad as alpha mask */
    new_clip.is_rect = అబద్ధం;
    size_t clip_కొలత = (size_t) new_clip.stride * h;
    ఒకవేళ (unlikely (clip_కొలత > HB_RASTER_MAX_బఫరు_కొలత ||
                  !new_clip.alpha.resize ((గుర్తులేని) clip_కొలత)))
    {
      hb_raster_paint_నెట్టు_ఖాళీలు_clip (c, w, h);
      తిరిగి;
    }
    hb_memset (new_clip.alpha.arrayZ, 0, (గుర్తులేని) clip_కొలత);

    /* Convert quad corners to pixel-relative coords */
    దశాంశం qx[4], qy[4];
    పూర్ణాంకం ox = surf->extents.x_origin;
    పూర్ణాంకం oy = surf->extents.y_origin;
    కొరకు (గుర్తులేని i = 0; i < 4; i++)
    {
      qx[i] = cx[i] - ox;
      qy[i] = cy[i] - oy;
    }

    /* For each pixel in the bounding box, test if inside the quad
     * using cross-product edge tests (winding order). */
    పూర్ణాంకం iy0_i = hb_max (py0, (పూర్ణాంకం) old_clip.min_y);
    పూర్ణాంకం iy1_i = hb_min (py1, (పూర్ణాంకం) old_clip.max_y);
    పూర్ణాంకం ix0_i = hb_max (px0, (పూర్ణాంకం) old_clip.min_x);
    పూర్ణాంకం ix1_i = hb_min (px1, (పూర్ణాంకం) old_clip.max_x);
    new_clip.min_x = w; new_clip.min_y = h;
    new_clip.max_x = 0; new_clip.max_y = 0;

    ఒకవేళ (ix0_i < ix1_i && iy0_i < iy1_i)
    {
      గుర్తులేని iy0 = (గుర్తులేని) iy0_i;
      గుర్తులేని iy1 = (గుర్తులేని) iy1_i;
      గుర్తులేని ix0 = (గుర్తులేని) ix0_i;
      గుర్తులేని ix1 = (గుర్తులేని) ix1_i;

      /* Precompute edge normals for point-in-quad test.
       * Edge i goes from corner i to corner (i+1)%4.
       * Normal = (dy, -dx); inside test: dot(normal, p-corner) >= 0 */
	      దశాంశం enx[4], eny[4], ed[4];
	      కొరకు (గుర్తులేని i = 0; i < 4; i++)
	      {
		గుర్తులేని j = (i + 1) & 3;
		దశాంశం edx = qx[j] - qx[i], edy = qy[j] - qy[i];
		enx[i] = edy;       /* normal x */
		eny[i] = -edx;      /* normal y */
		ed[i] = enx[i] * qx[i] + eny[i] * qy[i]; /* distance threshold */
	      }
	      దశాంశం area2 = 0.f;
	      కొరకు (గుర్తులేని i = 0; i < 4; i++)
	      {
		గుర్తులేని j = (i + 1) & 3;
		area2 += qx[i] * qy[j] - qx[j] * qy[i];
	      }
	      తార్కిక ccw = area2 >= 0.f;

      ఒకవేళ (old_clip.is_rect)
      {
	కొరకు (గుర్తులేని y = iy0; y < iy1; y++)
	  కొరకు (గుర్తులేని x = ix0; x < ix1; x++)
	  {
	    దశాంశం px_f = x + 0.5f, py_f = y + 0.5f;
	    /* Test if pixel center is inside the quad */
		    తార్కిక inside = నిజం;
		    కొరకు (గుర్తులేని i = 0; i < 4; i++)
		    {
		      దశాంశం d = enx[i] * px_f + eny[i] * py_f;
		      ఒకవేళ (ccw ? d < ed[i] : d > ed[i])
		      {
			inside = అబద్ధం;
			ఆపు;
		      }
		    }
	    uint8_t a = inside ? 255 : 0;
	    new_clip.alpha[y * new_clip.stride + x] = a;
	    ఒకవేళ (a)
	    {
	      new_clip.min_x = hb_min (new_clip.min_x, x);
	      new_clip.min_y = hb_min (new_clip.min_y, y);
	      new_clip.max_x = hb_max (new_clip.max_x, x + 1);
	      new_clip.max_y = hb_max (new_clip.max_y, y + 1);
	    }
	  }
      }
      లేకపోతే
      {
	కొరకు (గుర్తులేని y = iy0; y < iy1; y++)
	{
	  స్థిరరాశి uint8_t *old_అడ్డువరుస = old_clip.alpha.arrayZ + y * old_clip.stride;
	  కొరకు (గుర్తులేని x = ix0; x < ix1; x++)
	  {
	    దశాంశం px_f = x + 0.5f, py_f = y + 0.5f;
	    /* Test if pixel center is inside the quad */
		    తార్కిక inside = నిజం;
		    కొరకు (గుర్తులేని i = 0; i < 4; i++)
		    {
		      దశాంశం d = enx[i] * px_f + eny[i] * py_f;
		      ఒకవేళ (ccw ? d < ed[i] : d > ed[i])
		      {
			inside = అబద్ధం;
			ఆపు;
		      }
		    }
	    uint8_t a = inside ? old_అడ్డువరుస[x] : 0;
	    new_clip.alpha[y * new_clip.stride + x] = a;
	    ఒకవేళ (a)
	    {
	      new_clip.min_x = hb_min (new_clip.min_x, x);
	      new_clip.min_y = hb_min (new_clip.min_y, y);
	      new_clip.max_x = hb_max (new_clip.max_x, x + 1);
	      new_clip.max_y = hb_max (new_clip.max_y, y + 1);
	    }
	  }
	}
      }
    }
  }

  ఒకవేళ (unlikely (!c->clip_దొంతర.నెట్టు_or_fail (std::move (new_clip))))
    hb_raster_paint_నెట్టు_ఖాళీలు_clip (c, surf->extents.వెడల్పు, surf->extents.ఎత్తు);
}

స్థిర hb_draw_funcs_t *
hb_raster_paint_నెట్టు_clip_మార్గం_ప్రారంభం (hb_paint_funcs_t *pfuncs HB_వాడని,
				      శూన్య *paint_దత్తాంశం,
				      శూన్య **draw_దత్తాంశం,
				      శూన్య *user_దత్తాంశం HB_వాడని)
{
  hb_raster_paint_t *c = (hb_raster_paint_t *) paint_దత్తాంశం;

  ensure_initialized (c);

  /* Prime clip_rdr with the current effective transform; the
   * caller then drives hb_draw_*() into it, and _end renders
   * the accumulated edges into a mask and intersects with the
   * current clip. */
  ఒకవేళ (unlikely (!c->surface_దొంతర.నిడివి || !c->clip_rdr))
  {
    *draw_దత్తాంశం = శూన్యసూచిక;
    తిరిగి శూన్యసూచిక;
  }

  hb_raster_draw_t *rdr = c->clip_rdr;
  hb_transform_t<> t = c->current_effective_transform ();
  hb_raster_draw_సమితి_transform (rdr, t.xx, t.yx, t.xy, t.yy, t.x0, t.y0);
  hb_raster_image_t *surf = c->current_surface ();
  ఒకవేళ (likely (surf))
    hb_raster_paint_attach_clip_rdr (c, rdr, surf);

  *draw_దత్తాంశం = rdr;
  తిరిగి hb_raster_draw_get_funcs (rdr);
}

స్థిర శూన్య
hb_raster_paint_నెట్టు_clip_మార్గం_చివర (hb_paint_funcs_t *pfuncs HB_వాడని,
				    శూన్య *paint_దత్తాంశం,
				    శూన్య *user_దత్తాంశం HB_వాడని)
{
  hb_raster_paint_t *c = (hb_raster_paint_t *) paint_దత్తాంశం;

  hb_raster_image_t *surf = c->current_surface ();
  ఒకవేళ (unlikely (!surf || !c->clip_rdr)) తిరిగి;

  గుర్తులేని w = surf->extents.వెడల్పు;
  గుర్తులేని h = surf->extents.ఎత్తు;

  hb_raster_paint_readback_budget (c, c->clip_rdr);
  ఒకవేళ (unlikely (c->budget_మిగిలిన < 0))
  {
    hb_raster_draw_clear (c->clip_rdr);
    hb_raster_paint_నెట్టు_ఖాళీలు_clip (c, w, h);
    తిరిగి;
  }

  hb_raster_paint_finalize_మార్గం_clip (c, c->clip_rdr, surf, w, h);
}

స్థిర శూన్య
hb_raster_paint_తీయి_clip (hb_paint_funcs_t *pfuncs HB_వాడని,
			  శూన్య *paint_దత్తాంశం,
			  శూన్య *user_దత్తాంశం HB_వాడని)
{
  hb_raster_paint_t *c = (hb_raster_paint_t *) paint_దత్తాంశం;
  ఒకవేళ (!c->clip_దొంతర.నిడివి) తిరిగి;
  c->release_clip (c->clip_దొంతర.తీయి ());
}

స్థిర శూన్య
hb_raster_paint_నెట్టు_సమూహం (hb_paint_funcs_t *pfuncs HB_వాడని,
			    శూన్య *paint_దత్తాంశం,
			    శూన్య *user_దత్తాంశం HB_వాడని)
{
  hb_raster_paint_t *c = (hb_raster_paint_t *) paint_దత్తాంశం;

  ensure_initialized (c);

  /* acquire_surface() clears a full surface; charge its area. */
  ఒకవేళ (unlikely (!c->charge_work ((int64_t) c->fixed_extents.వెడల్పు * c->fixed_extents.ఎత్తు)))
    తిరిగి;

  hb_raster_image_t *new_surf = c->acquire_surface ();
  ఒకవేళ (unlikely (!new_surf)) తిరిగి;
  ఒకవేళ (unlikely (!c->surface_దొంతర.నెట్టు_or_fail (new_surf)))
    c->release_surface (new_surf);
}

స్థిర శూన్య
hb_raster_paint_తీయి_సమూహం (hb_paint_funcs_t *pfuncs HB_వాడని,
			   శూన్య *paint_దత్తాంశం,
			   hb_paint_సంయుక్త_బహుళకం_t బహుళకం,
			   శూన్య *user_దత్తాంశం HB_వాడని)
{
  hb_raster_paint_t *c = (hb_raster_paint_t *) paint_దత్తాంశం;

  ఒకవేళ (c->surface_దొంతర.నిడివి < 2) తిరిగి;

  hb_raster_image_t *src = c->surface_దొంతర.తీయి ();
  hb_raster_image_t *dst = c->current_surface ();

  ఒకవేళ (dst && src &&
      likely (c->charge_work ((int64_t) dst->extents.వెడల్పు * dst->extents.ఎత్తు)))
    hb_raster_image_సంయుక్త (dst, src, బహుళకం);

  c->release_surface (src);
}

స్థిర శూన్య
hb_raster_paint_solid (hb_raster_paint_t *c,
		       hb_raster_image_t *surf,
		       hb_color_t color,
		       hb_raster_image_t *ముసుగు_img)
{
  uint32_t premul = color_గమ్యం_premul_pixel (color);
  uint8_t premul_a = (uint8_t) (premul >> 24);
  స్థిరరాశి hb_raster_clip_t &clip = c->current_clip ();

  గుర్తులేని stride = surf->extents.stride;
  ఒకవేళ (clip.min_x >= clip.max_x || clip.min_y >= clip.max_y) తిరిగి;
  ఒకవేళ (premul_a == 0) తిరిగి;

  గుర్తులేని ix0 = clip.min_x, iy0 = clip.min_y;
  గుర్తులేని ix1 = clip.max_x, iy1 = clip.max_y;
  స్థిరరాశి uint8_t *ముసుగు_buf = శూన్యసూచిక;
  hb_raster_extents_t ముసుగు_ext;
  int64_t ముసుగు_x0 = 0, ముసుగు_y0 = 0;
  ఒకవేళ (ముసుగు_img)
  {
    ముసుగు_buf = hb_raster_image_get_బఫరు (ముసుగు_img);
    hb_raster_image_get_extents (ముసుగు_img, &ముసుగు_ext);
    ఒకవేళ (!hb_raster_paint_intersect_ముసుగు (surf, ముసుగు_ext, clip,
					 surf->extents.వెడల్పు, surf->extents.ఎత్తు,
					 &ముసుగు_x0, &ముసుగు_y0,
					 &ix0, &iy0, &ix1, &iy1))
      తిరిగి;
  }

  ఒకవేళ (unlikely (!c->charge_work ((int64_t) (ix1 - ix0) * (iy1 - iy0))))
    తిరిగి;

  ఒకవేళ (likely (!clip.is_rect))
  {
    కొరకు (గుర్తులేని y = iy0; y < iy1; y++)
    {
      hb_packed_t<uint32_t> *__restrict అడ్డువరుస = (hb_packed_t<uint32_t> *) (surf->బఫరు.arrayZ + y * stride);
      స్థిరరాశి uint8_t *__restrict clip_అడ్డువరుస = clip.alpha.arrayZ + y * clip.stride;
      స్థిరరాశి uint8_t *__restrict ముసుగు_అడ్డువరుస = ముసుగు_buf ? ముసుగు_buf + (గుర్తులేని) ((int64_t) y - ముసుగు_y0) * ముసుగు_ext.stride
						    : శూన్యసూచిక;
      గుర్తులేని mx = (గుర్తులేని) ((int64_t) ix0 - ముసుగు_x0);
      కొరకు (గుర్తులేని x = ix0; x < ix1; x++)
      {
	uint8_t clip_alpha = clip_అడ్డువరుస[x];
	ఒకవేళ (ముసుగు_అడ్డువరుస)
	  clip_alpha = hb_raster_div255 (ముసుగు_అడ్డువరుస[mx++] * clip_alpha);
	ఒకవేళ (clip_alpha == 0) కొనసాగు;
	ఒకవేళ (clip_alpha == 255)
	{
	  ఒకవేళ (premul_a == 255)
	    అడ్డువరుస[x] = hb_packed_t<uint32_t> (premul);
	  లేకపోతే
	    అడ్డువరుస[x] = hb_packed_t<uint32_t> (hb_raster_src_over (premul, (uint32_t) అడ్డువరుస[x]));
	}
	లేకపోతే
	{
	  uint32_t src = hb_raster_alpha_mul (premul, clip_alpha);
	  అడ్డువరుస[x] = hb_packed_t<uint32_t> (hb_raster_src_over (src, (uint32_t) అడ్డువరుస[x]));
	}
      }
    }
  }
  లేకపోతే ఒకవేళ (ముసుగు_buf)
  {
    కొరకు (గుర్తులేని y = iy0; y < iy1; y++)
    {
      hb_packed_t<uint32_t> *__restrict అడ్డువరుస = (hb_packed_t<uint32_t> *) (surf->బఫరు.arrayZ + y * stride);
      స్థిరరాశి uint8_t *__restrict ముసుగు_అడ్డువరుస = ముసుగు_buf + (గుర్తులేని) ((int64_t) y - ముసుగు_y0) * ముసుగు_ext.stride;
      గుర్తులేని mx = (గుర్తులేని) ((int64_t) ix0 - ముసుగు_x0);
      కొరకు (గుర్తులేని x = ix0; x < ix1; x++)
      {
	uint8_t ముసుగు_alpha = ముసుగు_అడ్డువరుస[mx++];
	ఒకవేళ (ముసుగు_alpha == 0) కొనసాగు;
	uint32_t src = hb_raster_alpha_mul (premul, ముసుగు_alpha);
	అడ్డువరుస[x] = hb_packed_t<uint32_t> (hb_raster_src_over (src, (uint32_t) అడ్డువరుస[x]));
      }
    }
  }
  లేకపోతే
  {
    కొరకు (గుర్తులేని y = clip.min_y; y < clip.max_y; y++)
    {
      hb_packed_t<uint32_t> *అడ్డువరుస = (hb_packed_t<uint32_t> *) (surf->బఫరు.arrayZ + y * stride);
      కొరకు (గుర్తులేని x = clip.min_x; x < clip.max_x; x++)
	అడ్డువరుస[x] = hb_packed_t<uint32_t> (hb_raster_src_over (premul, (uint32_t) అడ్డువరుస[x]));
    }
  }
}

స్థిర శూన్య
hb_raster_paint_color (hb_paint_funcs_t *pfuncs HB_వాడని,
		       శూన్య *paint_దత్తాంశం,
		       hb_bool_t is_foreground HB_వాడని,
		       hb_color_t color,
		       శూన్య *user_దత్తాంశం HB_వాడని)
{
  hb_raster_paint_t *c = (hb_raster_paint_t *) paint_దత్తాంశం;

  ensure_initialized (c);

  hb_raster_image_t *surf = c->current_surface ();
  ఒకవేళ (unlikely (!surf)) తిరిగి;

  hb_raster_paint_solid (c, surf, color, శూన్యసూచిక);
}

స్థిర శూన్య
hb_raster_paint_fill_glyph (hb_paint_funcs_t *pfuncs HB_వాడని,
			    శూన్య *paint_దత్తాంశం,
			    hb_codepoint_t glyph,
			    hb_font_t *font,
			    hb_bool_t is_foreground HB_వాడని,
			    hb_color_t color,
			    శూన్య *user_దత్తాంశం HB_వాడని)
{
  hb_raster_paint_t *c = (hb_raster_paint_t *) paint_దత్తాంశం;

  ensure_initialized (c);

  hb_raster_image_t *surf = c->current_surface ();
  ఒకవేళ (unlikely (!surf)) తిరిగి;

  /* Out of budget: skip the glyph-outline extraction entirely. */
  ఒకవేళ (unlikely (c->budget_మిగిలిన < 0)) తిరిగి;

  hb_raster_draw_t *rdr = c->clip_rdr;
  hb_transform_t<> t = c->current_effective_transform ();
  hb_raster_draw_సమితి_transform (rdr, t.xx, t.yx, t.xy, t.yy, t.x0, t.y0);
  hb_raster_paint_attach_clip_rdr (c, rdr, surf);

  hb_raster_paint_glyph_clip_దత్తాంశం_t దత్తాంశం = {glyph, font};
  hb_raster_paint_emit_clip_glyph_ముసుగు (rdr, &దత్తాంశం);
  hb_raster_paint_readback_budget (c, rdr);

  ఒకవేళ (unlikely (!c->precharge_work (hb_raster_draw_get_pixel_work (rdr,
							  surf->extents.ఎత్తు,
							  surf->extents.వెడల్పు))))
  {
    hb_raster_draw_clear (rdr);
    తిరిగి;
  }

  hb_raster_image_t *ముసుగు_img = hb_raster_draw_render (rdr);
  ఒకవేళ (unlikely (!ముసుగు_img)) తిరిగి;

  hb_raster_extents_t ముసుగు_ext;
  hb_raster_image_get_extents (ముసుగు_img, &ముసుగు_ext);
  ఒకవేళ (likely (c->charge_work ((int64_t) ముసుగు_ext.వెడల్పు * ముసుగు_ext.ఎత్తు)))
    hb_raster_paint_solid (c, surf, color, ముసుగు_img);

  hb_raster_draw_recycle_image (rdr, ముసుగు_img);
}

స్థిర hb_bool_t
hb_raster_paint_image (hb_paint_funcs_t *pfuncs HB_వాడని,
		       శూన్య *paint_దత్తాంశం,
		       hb_blob_t *blob,
		       గుర్తులేని వెడల్పు,
		       గుర్తులేని ఎత్తు,
		       hb_ట్యాగ్_t format,
		       దశాంశం slant HB_వాడని,
		       hb_glyph_extents_t *extents,
		       శూన్య *user_దత్తాంశం HB_వాడని)
{
  hb_raster_paint_t *c = (hb_raster_paint_t *) paint_దత్తాంశం;

  ensure_initialized (c);

  /* Out of pixel budget: skip, including the image decode below.  Image
   * compositing is pixel work and does no outline traversal. */
  ఒకవేళ (unlikely (c->pixel_మిగిలిన < 0)) తిరిగి అబద్ధం;

  గుర్తులేని src_వెడల్పు = వెడల్పు;
  గుర్తులేని src_ఎత్తు = ఎత్తు;
  స్థిరరాశి hb_packed_t<uint32_t> *src_దత్తాంశం = శూన్యసూచిక;
  hb_raster_image_t decoded_png;

  ఒకవేళ (format == HB_PAINT_IMAGE_FORMAT_BGRA)
  {
    ఒకవేళ (src_వెడల్పు == 0 || src_ఎత్తు == 0)
      తిరిగి అబద్ధం;
    ఒకవేళ (src_వెడల్పు > (గుర్తులేని) గరిష్ఠ_పూర్ణాంకం || src_ఎత్తు > (గుర్తులేని) గరిష్ఠ_పూర్ణాంకం)
      తిరిగి అబద్ధం;

    గుర్తులేని దత్తాంశం_len;
    స్థిరరాశి uint8_t *దత్తాంశం = (స్థిరరాశి uint8_t *) hb_blob_get_దత్తాంశం (blob, &దత్తాంశం_len);
    size_t pixel_లెక్క = (size_t) src_వెడల్పు * (size_t) src_ఎత్తు;
    ఒకవేళ (src_వెడల్పు && pixel_లెక్క / src_వెడల్పు != src_ఎత్తు)
      తిరిగి అబద్ధం;
    ఒకవేళ (pixel_లెక్క > (size_t) -1 / 4u)
      తిరిగి అబద్ధం;
    size_t required_కొలత = pixel_లెక్క * 4u;
    ఒకవేళ (!దత్తాంశం || (size_t) దత్తాంశం_len < required_కొలత)
      తిరిగి అబద్ధం;

    src_దత్తాంశం = (స్థిరరాశి hb_packed_t<uint32_t> *) దత్తాంశం;
  }
  లేకపోతే ఒకవేళ (format == HB_PAINT_IMAGE_FORMAT_PNG)
  {
#నిర్వచితమైతే HAVE_PNG
    ఒకవేళ (!decoded_png.deserialize_from_png (blob))
      తిరిగి అబద్ధం;
    src_వెడల్పు = decoded_png.extents.వెడల్పు;
    src_ఎత్తు = decoded_png.extents.ఎత్తు;
    src_దత్తాంశం = (స్థిరరాశి hb_packed_t<uint32_t> *) decoded_png.బఫరు.arrayZ;
#లేకపోతే
    తిరిగి అబద్ధం;
#అయితే_ముగింపు
  }
  లేకపోతే
    తిరిగి అబద్ధం;

  hb_raster_image_t *surf = c->current_surface ();
  ఒకవేళ (unlikely (!surf)) తిరిగి అబద్ధం;
  ఒకవేళ (!extents) తిరిగి అబద్ధం;

  స్థిరరాశి hb_raster_clip_t &clip = c->current_clip ();
  hb_transform_t<> t = c->current_effective_transform ();

  /* Compute inverse transform for sampling */
  దశాంశం det = t.xx * t.yy - t.xy * t.yx;
  ఒకవేళ (fabsf (det) < 1e-10f) తిరిగి అబద్ధం;
  దశాంశం inv_det = 1.f / det;
  దశాంశం inv_xx =  t.yy * inv_det;
  దశాంశం inv_xy = -t.xy * inv_det;
  దశాంశం inv_yx = -t.yx * inv_det;
  దశాంశం inv_yy =  t.xx * inv_det;
  దశాంశం inv_x0 = (t.xy * t.y0 - t.yy * t.x0) * inv_det;
  దశాంశం inv_y0 = (t.yx * t.x0 - t.xx * t.y0) * inv_det;

  గుర్తులేని surf_stride = surf->extents.stride;
  పూర్ణాంకం ox = surf->extents.x_origin;
  పూర్ణాంకం oy = surf->extents.y_origin;

  /* Image source rectangle in glyph space */
  దశాంశం img_x = extents->x_bearing;
  దశాంశం img_y = extents->y_bearing;
  దశాంశం img_sx = (దశాంశం) extents->వెడల్పు / src_వెడల్పు;
  దశాంశం img_sy = (దశాంశం) extents->ఎత్తు / src_ఎత్తు;
  ఒకవేళ (fabsf (img_sx) < 1e-10f || fabsf (img_sy) < 1e-10f)
    తిరిగి అబద్ధం;

  {
    గుర్తులేని clip_w = clip.max_x > clip.min_x ? clip.max_x - clip.min_x : 0;
    గుర్తులేని clip_h = clip.max_y > clip.min_y ? clip.max_y - clip.min_y : 0;
    ఒకవేళ (unlikely (!c->charge_work ((int64_t) clip_w * clip_h +
				   (int64_t) src_వెడల్పు * src_ఎత్తు)))
      తిరిగి అబద్ధం;
  }

  ఒకవేళ (clip.is_rect)
  {
    కొరకు (గుర్తులేని py = clip.min_y; py < clip.max_y; py++)
    {
      hb_packed_t<uint32_t> *అడ్డువరుస = (hb_packed_t<uint32_t> *) (surf->బఫరు.arrayZ + py * surf_stride);
      దశాంశం gx = inv_xx * (దశాంశం) ((పూర్ణాంకం) clip.min_x + ox) + inv_xy * (దశాంశం) ((పూర్ణాంకం) py + oy) + inv_x0;
      దశాంశం gy = inv_yx * (దశాంశం) ((పూర్ణాంకం) clip.min_x + ox) + inv_yy * (దశాంశం) ((పూర్ణాంకం) py + oy) + inv_y0;
      కొరకు (గుర్తులేని px = clip.min_x; px < clip.max_x; px++)
      {
	/* Map glyph space to image texel; bilinear reconstruction. */
	దశాంశం ix = (gx - img_x) / img_sx;
	దశాంశం iy = (దశాంశం) (src_ఎత్తు - 1) - (gy - img_y) / img_sy;

	ఒకవేళ (unlikely (!std::isfinite (ix) || !std::isfinite (iy)) ||
	    ix < 0.f || iy < 0.f ||
	    ix > (దశాంశం) (src_వెడల్పు - 1) || iy > (దశాంశం) (src_ఎత్తు - 1))
	{
	  gx += inv_xx;
	  gy += inv_yx;
	  కొనసాగు;
	}

	uint32_t src_px = hb_raster_sample_bilinear_premul (src_దత్తాంశం, src_వెడల్పు, src_ఎత్తు,
							     ix, iy);
	అడ్డువరుస[px] = hb_packed_t<uint32_t> (hb_raster_src_over (src_px, (uint32_t) అడ్డువరుస[px]));
	gx += inv_xx;
	gy += inv_yx;
      }
    }
  }
  లేకపోతే
  {
    కొరకు (గుర్తులేని py = clip.min_y; py < clip.max_y; py++)
    {
      hb_packed_t<uint32_t> *అడ్డువరుస = (hb_packed_t<uint32_t> *) (surf->బఫరు.arrayZ + py * surf_stride);
      స్థిరరాశి uint8_t *clip_అడ్డువరుస = clip.alpha.arrayZ + py * clip.stride;
      దశాంశం gx = inv_xx * (దశాంశం) ((పూర్ణాంకం) clip.min_x + ox) + inv_xy * (దశాంశం) ((పూర్ణాంకం) py + oy) + inv_x0;
      దశాంశం gy = inv_yx * (దశాంశం) ((పూర్ణాంకం) clip.min_x + ox) + inv_yy * (దశాంశం) ((పూర్ణాంకం) py + oy) + inv_y0;
      కొరకు (గుర్తులేని px = clip.min_x; px < clip.max_x; px++)
      {
	uint8_t clip_alpha = clip_అడ్డువరుస[px];
	ఒకవేళ (clip_alpha == 0)
	{
	  gx += inv_xx;
	  gy += inv_yx;
	  కొనసాగు;
	}

	/* Map glyph space to image texel; bilinear reconstruction. */
	దశాంశం ix = (gx - img_x) / img_sx;
	దశాంశం iy = (దశాంశం) (src_ఎత్తు - 1) - (gy - img_y) / img_sy;

	ఒకవేళ (unlikely (!std::isfinite (ix) || !std::isfinite (iy)) ||
	    ix < 0.f || iy < 0.f ||
	    ix > (దశాంశం) (src_వెడల్పు - 1) || iy > (దశాంశం) (src_ఎత్తు - 1))
	{
	  gx += inv_xx;
	  gy += inv_yx;
	  కొనసాగు;
	}

	uint32_t src_px = hb_raster_sample_bilinear_premul (src_దత్తాంశం, src_వెడల్పు, src_ఎత్తు,
							     ix, iy);
	src_px = hb_raster_alpha_mul (src_px, clip_alpha);
	అడ్డువరుస[px] = hb_packed_t<uint32_t> (hb_raster_src_over (src_px, (uint32_t) అడ్డువరుస[px]));
	gx += inv_xx;
	gy += inv_yx;
      }
    }
  }

  తిరిగి నిజం;
}


/*
 * Gradient helpers
 */

#నిర్వచించు GRADIENT_LUT_కొలత 256
#నిర్వచించు GRADIENT_LUT_MIN_PIXELS (64u * 64u)

స్థిర HB_ALWAYS_INLINE దశాంశం
reflect_gradient_t (దశాంశం t)
{
  t = fmodf (fabsf (t), 2.f);
  తిరిగి t > 1.f ? 2.f - t : t;
}

/* Evaluate color at normalized position t, interpolating in premultiplied space. */
స్థిర uint32_t
evaluate_color_పంక్తి (స్థిరరాశి hb_color_stop_t *stops, గుర్తులేని len, దశాంశం t,
		     hb_paint_extend_t extend)
{
  ఒకవేళ (unlikely (!len))
    తిరిగి 0;
  ఒకవేళ (unlikely (len == 1))
    తిరిగి color_గమ్యం_premul_pixel (stops[0].color);
  ఒకవేళ (unlikely (!std::isfinite (t)))
    t = 0.f;

  /* Apply extend mode */
  ఒకవేళ (extend == HB_PAINT_EXTEND_PAD)
  {
    t = hb_clamp (t, 0.f, 1.f);
  }
  లేకపోతే ఒకవేళ (extend == HB_PAINT_EXTEND_REPEAT)
  {
    t = t - floorf (t);
  }
  లేకపోతే /* REFLECT */
  {
    t = reflect_gradient_t (t);
  }

  /* Find bounding stops */
  ఒకవేళ (t <= stops[0].స్థానభ్రంశం)
    తిరిగి color_గమ్యం_premul_pixel (stops[0].color);
  ఒకవేళ (t >= stops[len - 1].స్థానభ్రంశం)
    తిరిగి color_గమ్యం_premul_pixel (stops[len - 1].color);

  గుర్తులేని i;
  కొరకు (i = 0; i < len - 1; i++)
    ఒకవేళ (t < stops[i + 1].స్థానభ్రంశం)
      ఆపు;

  దశాంశం range = stops[i + 1].స్థానభ్రంశం - stops[i].స్థానభ్రంశం;
  దశాంశం k = range > 0.f ? (t - stops[i].స్థానభ్రంశం) / range : 0.f;

  /* Interpolate in premultiplied [0,255] space */
  hb_color_t c0 = stops[i].color;
  hb_color_t c1 = stops[i + 1].color;

  దశాంశం a0 = hb_color_get_alpha (c0) / 255.f;
  దశాంశం r0 = hb_color_get_red   (c0) / 255.f * a0;
  దశాంశం g0 = hb_color_get_green (c0) / 255.f * a0;
  దశాంశం b0 = hb_color_get_blue  (c0) / 255.f * a0;

  దశాంశం a1 = hb_color_get_alpha (c1) / 255.f;
  దశాంశం r1 = hb_color_get_red   (c1) / 255.f * a1;
  దశాంశం g1 = hb_color_get_green (c1) / 255.f * a1;
  దశాంశం b1 = hb_color_get_blue  (c1) / 255.f * a1;

  దశాంశం a = a0 + k * (a1 - a0);
  దశాంశం r = r0 + k * (r1 - r0);
  దశాంశం g = g0 + k * (g1 - g0);
  దశాంశం b = b0 + k * (b1 - b0);

  uint8_t pa = (uint8_t) (a * 255.f + 0.5f);
  uint8_t pr = (uint8_t) (r * 255.f + 0.5f);
  uint8_t pg = (uint8_t) (g * 255.f + 0.5f);
  uint8_t pb = (uint8_t) (b * 255.f + 0.5f);

  తిరిగి (uint32_t) pb | ((uint32_t) pg << 8) | ((uint32_t) pr << 16) | ((uint32_t) pa << 24);
}

స్థిర HB_ALWAYS_INLINE దశాంశం
normalize_gradient_t (దశాంశం t, hb_paint_extend_t extend)
{
  ఒకవేళ (unlikely (!std::isfinite (t)))
    తిరిగి 0.f;

  ఒకవేళ (extend == HB_PAINT_EXTEND_PAD)
    తిరిగి hb_clamp (t, 0.f, 1.f);
  ఒకవేళ (extend == HB_PAINT_EXTEND_REPEAT)
  {
    t = t - floorf (t);
    తిరిగి t < 0.f ? t + 1.f : t;
  }

  /* REFLECT */
  తిరిగి reflect_gradient_t (t);
}

స్థిర శూన్య
build_gradient_lut (స్థిరరాశి hb_color_stop_t *stops,
		    గుర్తులేని len,
		    uint32_t *lut)
{
  కొరకు (గుర్తులేని i = 0; i < GRADIENT_LUT_కొలత; i++)
  {
    దశాంశం t = (దశాంశం) i / (GRADIENT_LUT_కొలత - 1);
    lut[i] = evaluate_color_పంక్తి (stops, len, t, HB_PAINT_EXTEND_PAD);
  }
}

స్థిర HB_ALWAYS_INLINE uint32_t
lookup_gradient_lut (స్థిరరాశి uint32_t *lut,
		     దశాంశం t,
		     hb_paint_extend_t extend)
{
  దశాంశం u = normalize_gradient_t (t, extend);
  ఒకవేళ (unlikely (!std::isfinite (u)))
    u = 0.f;
  లేకపోతే
    u = hb_clamp (u, 0.f, 1.f);
  గుర్తులేని idx = (గుర్తులేని) (u * (GRADIENT_LUT_కొలత - 1) + 0.5f);
  తిరిగి lut[idx];
}

/*
 * Gradient paint callbacks
 */

స్థిర శూన్య
hb_raster_paint_రేఖీయ_gradient (hb_paint_funcs_t *pfuncs HB_వాడని,
				 శూన్య *paint_దత్తాంశం,
				 hb_color_పంక్తి_t *color_పంక్తి,
				 దశాంశం x0, దశాంశం y0,
				 దశాంశం x1, దశాంశం y1,
				 దశాంశం x2, దశాంశం y2,
				 శూన్య *user_దత్తాంశం HB_వాడని)
{
  hb_raster_paint_t *c = (hb_raster_paint_t *) paint_దత్తాంశం;

  ensure_initialized (c);

  hb_raster_image_t *surf = c->current_surface ();
  ఒకవేళ (unlikely (!surf)) తిరిగి;

  ఒకవేళ (unlikely (!c->fetch_color_stops (color_పంక్తి)))
    తిరిగి;
  hb_color_stop_t *stops = c->scratch_color_stops.arrayZ;
  గుర్తులేని len = c->scratch_color_stops.నిడివి;
  దశాంశం mn, mx;
  hb_paint_normalize_color_పంక్తి (stops, len, &mn, &mx);

  hb_paint_extend_t extend = hb_color_పంక్తి_get_extend (color_పంక్తి);
  స్థిరరాశి hb_raster_clip_t &clip = c->current_clip ();
  గుర్తులేని clip_w = clip.max_x > clip.min_x ? clip.max_x - clip.min_x : 0;
  గుర్తులేని clip_h = clip.max_y > clip.min_y ? clip.max_y - clip.min_y : 0;
  ఒకవేళ (unlikely (!c->charge_work ((int64_t) clip_w * clip_h)))
    తిరిగి;
  తార్కిక use_lut = (uint64_t) clip_w * clip_h >= GRADIENT_LUT_MIN_PIXELS;
  uint32_t lut[GRADIENT_LUT_కొలత];
  ఒకవేళ (use_lut)
    build_gradient_lut (stops, len, lut);

  /* Reduce 3-point anchor to 2-point gradient axis */
  దశాంశం lx0, ly0, lx1, ly1;
  hb_paint_reduce_రేఖీయ_anchors (x0, y0, x1, y1, x2, y2,
				  &lx0, &ly0, &lx1, &ly1);

  /* Apply normalization to endpoints */
  దశాంశం gx0 = lx0 + mn * (lx1 - lx0);
  దశాంశం gy0 = ly0 + mn * (ly1 - ly0);
  దశాంశం gx1 = lx0 + mx * (lx1 - lx0);
  దశాంశం gy1 = ly0 + mx * (ly1 - ly0);

  /* Inverse transform: pixel → glyph space */
  hb_transform_t<> t = c->current_effective_transform ();
  దశాంశం det = t.xx * t.yy - t.xy * t.yx;
  ఒకవేళ (fabsf (det) < 1e-10f) తిరిగి;

  {
    దశాంశం inv_det = 1.f / det;
    దశాంశం inv_xx =  t.yy * inv_det;
    దశాంశం inv_xy = -t.xy * inv_det;
    దశాంశం inv_yx = -t.yx * inv_det;
    దశాంశం inv_yy =  t.xx * inv_det;
    దశాంశం inv_x0 = (t.xy * t.y0 - t.yy * t.x0) * inv_det;
    దశాంశం inv_y0 = (t.yx * t.x0 - t.xx * t.y0) * inv_det;

    /* Gradient direction vector and denominator for projection */
    దశాంశం dx = gx1 - gx0, dy = gy1 - gy0;
    దశాంశం denom = dx * dx + dy * dy;
    ఒకవేళ (denom < 1e-10f) తిరిగి;
    దశాంశం inv_denom = 1.f / denom;

    గుర్తులేని stride = surf->extents.stride;
    పూర్ణాంకం ox = surf->extents.x_origin;
    పూర్ణాంకం oy = surf->extents.y_origin;

    ఒకవేళ (clip.is_rect)
    {
      కొరకు (గుర్తులేని py = clip.min_y; py < clip.max_y; py++)
      {
	hb_packed_t<uint32_t> *అడ్డువరుస = (hb_packed_t<uint32_t> *) (surf->బఫరు.arrayZ + py * stride);
	దశాంశం gx = inv_xx * ((దశాంశం) ((పూర్ణాంకం) clip.min_x + ox) + 0.5f) + inv_xy * ((దశాంశం) ((పూర్ణాంకం) py + oy) + 0.5f) + inv_x0;
	దశాంశం gy = inv_yx * ((దశాంశం) ((పూర్ణాంకం) clip.min_x + ox) + 0.5f) + inv_yy * ((దశాంశం) ((పూర్ణాంకం) py + oy) + 0.5f) + inv_y0;
	ఒకవేళ (use_lut)
	{
	  కొరకు (గుర్తులేని px = clip.min_x; px < clip.max_x; px++)
	  {
	    దశాంశం proj_t = ((gx - gx0) * dx + (gy - gy0) * dy) * inv_denom;
	    uint32_t src = lookup_gradient_lut (lut, proj_t, extend);
	    అడ్డువరుస[px] = hb_packed_t<uint32_t> (hb_raster_src_over (src, (uint32_t) అడ్డువరుస[px]));
	    gx += inv_xx;
	    gy += inv_yx;
	  }
	}
	లేకపోతే
	{
	  కొరకు (గుర్తులేని px = clip.min_x; px < clip.max_x; px++)
	  {
	    దశాంశం proj_t = ((gx - gx0) * dx + (gy - gy0) * dy) * inv_denom;
	    uint32_t src = evaluate_color_పంక్తి (stops, len, proj_t, extend);
	    అడ్డువరుస[px] = hb_packed_t<uint32_t> (hb_raster_src_over (src, (uint32_t) అడ్డువరుస[px]));
	    gx += inv_xx;
	    gy += inv_yx;
	  }
	}
      }
    }
    లేకపోతే
    {
      కొరకు (గుర్తులేని py = clip.min_y; py < clip.max_y; py++)
      {
	hb_packed_t<uint32_t> *అడ్డువరుస = (hb_packed_t<uint32_t> *) (surf->బఫరు.arrayZ + py * stride);
	స్థిరరాశి uint8_t *clip_అడ్డువరుస = clip.alpha.arrayZ + py * clip.stride;
	దశాంశం gx = inv_xx * ((దశాంశం) ((పూర్ణాంకం) clip.min_x + ox) + 0.5f) + inv_xy * ((దశాంశం) ((పూర్ణాంకం) py + oy) + 0.5f) + inv_x0;
	దశాంశం gy = inv_yx * ((దశాంశం) ((పూర్ణాంకం) clip.min_x + ox) + 0.5f) + inv_yy * ((దశాంశం) ((పూర్ణాంకం) py + oy) + 0.5f) + inv_y0;
	ఒకవేళ (use_lut)
	{
	  కొరకు (గుర్తులేని px = clip.min_x; px < clip.max_x; px++)
	  {
	    uint8_t clip_alpha = clip_అడ్డువరుస[px];
	    ఒకవేళ (clip_alpha == 0)
	    {
	      gx += inv_xx;
	      gy += inv_yx;
	      కొనసాగు;
	    }
	    దశాంశం proj_t = ((gx - gx0) * dx + (gy - gy0) * dy) * inv_denom;
	    uint32_t src = lookup_gradient_lut (lut, proj_t, extend);
	    src = hb_raster_alpha_mul (src, clip_alpha);
	    అడ్డువరుస[px] = hb_packed_t<uint32_t> (hb_raster_src_over (src, (uint32_t) అడ్డువరుస[px]));
	    gx += inv_xx;
	    gy += inv_yx;
	  }
	}
	లేకపోతే
	{
	  కొరకు (గుర్తులేని px = clip.min_x; px < clip.max_x; px++)
	  {
	    uint8_t clip_alpha = clip_అడ్డువరుస[px];
	    ఒకవేళ (clip_alpha == 0)
	    {
	      gx += inv_xx;
	      gy += inv_yx;
	      కొనసాగు;
	    }
	    దశాంశం proj_t = ((gx - gx0) * dx + (gy - gy0) * dy) * inv_denom;
	    uint32_t src = evaluate_color_పంక్తి (stops, len, proj_t, extend);
	    src = hb_raster_alpha_mul (src, clip_alpha);
	    అడ్డువరుస[px] = hb_packed_t<uint32_t> (hb_raster_src_over (src, (uint32_t) అడ్డువరుస[px]));
	    gx += inv_xx;
	    gy += inv_yx;
	  }
	}
      }
    }
  }

}

స్థిర శూన్య
hb_raster_paint_radial_gradient (hb_paint_funcs_t *pfuncs HB_వాడని,
				 శూన్య *paint_దత్తాంశం,
				 hb_color_పంక్తి_t *color_పంక్తి,
				 దశాంశం x0, దశాంశం y0, దశాంశం r0,
				 దశాంశం x1, దశాంశం y1, దశాంశం r1,
				 శూన్య *user_దత్తాంశం HB_వాడని)
{
  hb_raster_paint_t *c = (hb_raster_paint_t *) paint_దత్తాంశం;

  ensure_initialized (c);

  hb_raster_image_t *surf = c->current_surface ();
  ఒకవేళ (unlikely (!surf)) తిరిగి;

  ఒకవేళ (unlikely (!c->fetch_color_stops (color_పంక్తి)))
    తిరిగి;
  hb_color_stop_t *stops = c->scratch_color_stops.arrayZ;
  గుర్తులేని len = c->scratch_color_stops.నిడివి;
  దశాంశం mn, mx;
  hb_paint_normalize_color_పంక్తి (stops, len, &mn, &mx);

  hb_paint_extend_t extend = hb_color_పంక్తి_get_extend (color_పంక్తి);
  స్థిరరాశి hb_raster_clip_t &clip = c->current_clip ();
  గుర్తులేని clip_w = clip.max_x > clip.min_x ? clip.max_x - clip.min_x : 0;
  గుర్తులేని clip_h = clip.max_y > clip.min_y ? clip.max_y - clip.min_y : 0;
  ఒకవేళ (unlikely (!c->charge_work ((int64_t) clip_w * clip_h)))
    తిరిగి;
  తార్కిక use_lut = (uint64_t) clip_w * clip_h >= GRADIENT_LUT_MIN_PIXELS;
  uint32_t lut[GRADIENT_LUT_కొలత];
  ఒకవేళ (use_lut)
    build_gradient_lut (stops, len, lut);

  /* Apply normalization to circle parameters */
  దశాంశం cx0 = x0 + mn * (x1 - x0);
  దశాంశం cy0 = y0 + mn * (y1 - y0);
  దశాంశం cr0 = r0 + mn * (r1 - r0);
  దశాంశం cx1 = x0 + mx * (x1 - x0);
  దశాంశం cy1 = y0 + mx * (y1 - y0);
  దశాంశం cr1 = r0 + mx * (r1 - r0);

  /* Inverse transform */
  hb_transform_t<> t = c->current_effective_transform ();
  దశాంశం det = t.xx * t.yy - t.xy * t.yx;
  ఒకవేళ (fabsf (det) < 1e-10f) తిరిగి;

  {
    దశాంశం inv_det = 1.f / det;
    దశాంశం inv_xx =  t.yy * inv_det;
    దశాంశం inv_xy = -t.xy * inv_det;
    దశాంశం inv_yx = -t.yx * inv_det;
    దశాంశం inv_yy =  t.xx * inv_det;
    దశాంశం inv_x0 = (t.xy * t.y0 - t.yy * t.x0) * inv_det;
    దశాంశం inv_y0 = (t.yx * t.x0 - t.xx * t.y0) * inv_det;

    /* Precompute quadratic coefficients for radial gradient:
     * |p - c0 - t*(c1-c0)|^2 = (r0 + t*(r1-r0))^2
     *
     * Expanding gives At^2 + Bt + C = 0 where:
     *   cdx = c1.x - c0.x, cdy = c1.y - c0.y, dr = r1 - r0
     *   A = cdx^2 + cdy^2 - dr^2
     *   B = -2*(px-c0.x)*cdx - 2*(py-c0.y)*cdy - 2*r0*dr
     *   C = (px-c0.x)^2 + (py-c0.y)^2 - r0^2
     */
    దశాంశం cdx = cx1 - cx0, cdy = cy1 - cy0;
    దశాంశం dr = cr1 - cr0;
    దశాంశం A = cdx * cdx + cdy * cdy - dr * dr;

    గుర్తులేని stride = surf->extents.stride;
    పూర్ణాంకం ox = surf->extents.x_origin;
    పూర్ణాంకం oy = surf->extents.y_origin;

    ఒకవేళ (clip.is_rect)
    {
      కొరకు (గుర్తులేని py = clip.min_y; py < clip.max_y; py++)
      {
	hb_packed_t<uint32_t> *అడ్డువరుస = (hb_packed_t<uint32_t> *) (surf->బఫరు.arrayZ + py * stride);
	దశాంశం gx = inv_xx * ((దశాంశం) ((పూర్ణాంకం) clip.min_x + ox) + 0.5f) + inv_xy * ((దశాంశం) ((పూర్ణాంకం) py + oy) + 0.5f) + inv_x0;
	దశాంశం gy = inv_yx * ((దశాంశం) ((పూర్ణాంకం) clip.min_x + ox) + 0.5f) + inv_yy * ((దశాంశం) ((పూర్ణాంకం) py + oy) + 0.5f) + inv_y0;
	ఒకవేళ (use_lut)
	{
	  కొరకు (గుర్తులేని px = clip.min_x; px < clip.max_x; px++)
	  {
	    దశాంశం dpx = gx - cx0, dpy = gy - cy0;
	    దశాంశం B = -2.f * (dpx * cdx + dpy * cdy + cr0 * dr);
	    దశాంశం C = dpx * dpx + dpy * dpy - cr0 * cr0;

	    దశాంశం grad_t;
	    ఒకవేళ (fabsf (A) > 1e-10f)
	    {
	      దశాంశం disc = B * B - 4.f * A * C;
	      ఒకవేళ (disc < 0.f)
	      {
		gx += inv_xx;
		gy += inv_yx;
		కొనసాగు;
	      }
	      దశాంశం sq = sqrtf (disc);
	      /* Pick the larger root (t closer to 1 = outer circle) */
	      దశాంశం t1 = (-B + sq) / (2.f * A);
	      దశాంశం t2 = (-B - sq) / (2.f * A);
	      /* Choose the root that gives a positive radius */
	      ఒకవేళ (cr0 + t1 * dr >= 0.f)
		grad_t = t1;
	      లేకపోతే
		grad_t = t2;
	    }
	    లేకపోతే
	    {
	      /* Linear case: Bt + C = 0 */
	      ఒకవేళ (fabsf (B) < 1e-10f)
	      {
		gx += inv_xx;
		gy += inv_yx;
		కొనసాగు;
	      }
	      grad_t = -C / B;
	    }

	    uint32_t src = lookup_gradient_lut (lut, grad_t, extend);
	    అడ్డువరుస[px] = hb_packed_t<uint32_t> (hb_raster_src_over (src, (uint32_t) అడ్డువరుస[px]));
	    gx += inv_xx;
	    gy += inv_yx;
	  }
	}
	లేకపోతే
	{
	  కొరకు (గుర్తులేని px = clip.min_x; px < clip.max_x; px++)
	  {
	    దశాంశం dpx = gx - cx0, dpy = gy - cy0;
	    దశాంశం B = -2.f * (dpx * cdx + dpy * cdy + cr0 * dr);
	    దశాంశం C = dpx * dpx + dpy * dpy - cr0 * cr0;

	    దశాంశం grad_t;
	    ఒకవేళ (fabsf (A) > 1e-10f)
	    {
	      దశాంశం disc = B * B - 4.f * A * C;
	      ఒకవేళ (disc < 0.f)
	      {
		gx += inv_xx;
		gy += inv_yx;
		కొనసాగు;
	      }
	      దశాంశం sq = sqrtf (disc);
	      దశాంశం t1 = (-B + sq) / (2.f * A);
	      దశాంశం t2 = (-B - sq) / (2.f * A);
	      grad_t = (cr0 + t1 * dr >= 0.f) ? t1 : t2;
	    }
	    లేకపోతే
	    {
	      ఒకవేళ (fabsf (B) < 1e-10f)
	      {
		gx += inv_xx;
		gy += inv_yx;
		కొనసాగు;
	      }
	      grad_t = -C / B;
	    }

	    uint32_t src = evaluate_color_పంక్తి (stops, len, grad_t, extend);
	    అడ్డువరుస[px] = hb_packed_t<uint32_t> (hb_raster_src_over (src, (uint32_t) అడ్డువరుస[px]));
	    gx += inv_xx;
	    gy += inv_yx;
	  }
	}
      }
    }
    లేకపోతే
    {
      కొరకు (గుర్తులేని py = clip.min_y; py < clip.max_y; py++)
      {
	hb_packed_t<uint32_t> *అడ్డువరుస = (hb_packed_t<uint32_t> *) (surf->బఫరు.arrayZ + py * stride);
	స్థిరరాశి uint8_t *clip_అడ్డువరుస = clip.alpha.arrayZ + py * clip.stride;
	దశాంశం gx = inv_xx * ((దశాంశం) ((పూర్ణాంకం) clip.min_x + ox) + 0.5f) + inv_xy * ((దశాంశం) ((పూర్ణాంకం) py + oy) + 0.5f) + inv_x0;
	దశాంశం gy = inv_yx * ((దశాంశం) ((పూర్ణాంకం) clip.min_x + ox) + 0.5f) + inv_yy * ((దశాంశం) ((పూర్ణాంకం) py + oy) + 0.5f) + inv_y0;
	ఒకవేళ (use_lut)
	{
	  కొరకు (గుర్తులేని px = clip.min_x; px < clip.max_x; px++)
	  {
	    uint8_t clip_alpha = clip_అడ్డువరుస[px];
	    ఒకవేళ (clip_alpha == 0)
	    {
	      gx += inv_xx;
	      gy += inv_yx;
	      కొనసాగు;
	    }
	    దశాంశం dpx = gx - cx0, dpy = gy - cy0;
	    దశాంశం B = -2.f * (dpx * cdx + dpy * cdy + cr0 * dr);
	    దశాంశం C = dpx * dpx + dpy * dpy - cr0 * cr0;

	    దశాంశం grad_t;
	    ఒకవేళ (fabsf (A) > 1e-10f)
	    {
	      దశాంశం disc = B * B - 4.f * A * C;
	      ఒకవేళ (disc < 0.f)
	      {
		gx += inv_xx;
		gy += inv_yx;
		కొనసాగు;
	      }
	      దశాంశం sq = sqrtf (disc);
	      దశాంశం t1 = (-B + sq) / (2.f * A);
	      దశాంశం t2 = (-B - sq) / (2.f * A);
	      grad_t = (cr0 + t1 * dr >= 0.f) ? t1 : t2;
	    }
	    లేకపోతే
	    {
	      ఒకవేళ (fabsf (B) < 1e-10f)
	      {
		gx += inv_xx;
		gy += inv_yx;
		కొనసాగు;
	      }
	      grad_t = -C / B;
	    }

	    uint32_t src = lookup_gradient_lut (lut, grad_t, extend);
	    src = hb_raster_alpha_mul (src, clip_alpha);
	    అడ్డువరుస[px] = hb_packed_t<uint32_t> (hb_raster_src_over (src, (uint32_t) అడ్డువరుస[px]));
	    gx += inv_xx;
	    gy += inv_yx;
	  }
	}
	లేకపోతే
	{
	  కొరకు (గుర్తులేని px = clip.min_x; px < clip.max_x; px++)
	  {
	    uint8_t clip_alpha = clip_అడ్డువరుస[px];
	    ఒకవేళ (clip_alpha == 0)
	    {
	      gx += inv_xx;
	      gy += inv_yx;
	      కొనసాగు;
	    }
	    దశాంశం dpx = gx - cx0, dpy = gy - cy0;
	    దశాంశం B = -2.f * (dpx * cdx + dpy * cdy + cr0 * dr);
	    దశాంశం C = dpx * dpx + dpy * dpy - cr0 * cr0;

	    దశాంశం grad_t;
	    ఒకవేళ (fabsf (A) > 1e-10f)
	    {
	      దశాంశం disc = B * B - 4.f * A * C;
	      ఒకవేళ (disc < 0.f)
	      {
		gx += inv_xx;
		gy += inv_yx;
		కొనసాగు;
	      }
	      దశాంశం sq = sqrtf (disc);
	      దశాంశం t1 = (-B + sq) / (2.f * A);
	      దశాంశం t2 = (-B - sq) / (2.f * A);
	      grad_t = (cr0 + t1 * dr >= 0.f) ? t1 : t2;
	    }
	    లేకపోతే
	    {
	      ఒకవేళ (fabsf (B) < 1e-10f)
	      {
		gx += inv_xx;
		gy += inv_yx;
		కొనసాగు;
	      }
	      grad_t = -C / B;
	    }

	    uint32_t src = evaluate_color_పంక్తి (stops, len, grad_t, extend);
	    src = hb_raster_alpha_mul (src, clip_alpha);
	    అడ్డువరుస[px] = hb_packed_t<uint32_t> (hb_raster_src_over (src, (uint32_t) అడ్డువరుస[px]));
	    gx += inv_xx;
	    gy += inv_yx;
	  }
	}
      }
    }
  }

}

స్థిర శూన్య
hb_raster_paint_sweep_gradient (hb_paint_funcs_t *pfuncs HB_వాడని,
				శూన్య *paint_దత్తాంశం,
				hb_color_పంక్తి_t *color_పంక్తి,
				దశాంశం cx, దశాంశం cy,
				దశాంశం ప్రారంభం_angle,
				దశాంశం చివర_angle,
				శూన్య *user_దత్తాంశం HB_వాడని)
{
  hb_raster_paint_t *c = (hb_raster_paint_t *) paint_దత్తాంశం;

  ensure_initialized (c);

  hb_raster_image_t *surf = c->current_surface ();
  ఒకవేళ (unlikely (!surf)) తిరిగి;

  ఒకవేళ (unlikely (!c->fetch_color_stops (color_పంక్తి)))
    తిరిగి;
  hb_color_stop_t *stops = c->scratch_color_stops.arrayZ;
  గుర్తులేని len = c->scratch_color_stops.నిడివి;
  దశాంశం mn, mx;
  hb_paint_normalize_color_పంక్తి (stops, len, &mn, &mx);

  hb_paint_extend_t extend = hb_color_పంక్తి_get_extend (color_పంక్తి);
  స్థిరరాశి hb_raster_clip_t &clip = c->current_clip ();
  గుర్తులేని clip_w = clip.max_x > clip.min_x ? clip.max_x - clip.min_x : 0;
  గుర్తులేని clip_h = clip.max_y > clip.min_y ? clip.max_y - clip.min_y : 0;
  ఒకవేళ (unlikely (!c->charge_work ((int64_t) clip_w * clip_h)))
    తిరిగి;
  తార్కిక use_lut = (uint64_t) clip_w * clip_h >= GRADIENT_LUT_MIN_PIXELS;
  uint32_t lut[GRADIENT_LUT_కొలత];
  ఒకవేళ (use_lut)
    build_gradient_lut (stops, len, lut);

  /* Apply normalization to angle range */
  దశాంశం a0 = ప్రారంభం_angle + mn * (చివర_angle - ప్రారంభం_angle);
  దశాంశం a1 = ప్రారంభం_angle + mx * (చివర_angle - ప్రారంభం_angle);
  దశాంశం angle_range = a1 - a0;

  /* Inverse transform */
  hb_transform_t<> t = c->current_effective_transform ();
  దశాంశం det = t.xx * t.yy - t.xy * t.yx;
  ఒకవేళ (fabsf (det) < 1e-10f || fabsf (angle_range) < 1e-10f) తిరిగి;

  {
    దశాంశం inv_det = 1.f / det;
    దశాంశం inv_xx =  t.yy * inv_det;
    దశాంశం inv_xy = -t.xy * inv_det;
    దశాంశం inv_yx = -t.yx * inv_det;
    దశాంశం inv_yy =  t.xx * inv_det;
    దశాంశం inv_x0 = (t.xy * t.y0 - t.yy * t.x0) * inv_det;
    దశాంశం inv_y0 = (t.yx * t.x0 - t.xx * t.y0) * inv_det;

    దశాంశం inv_angle_range = 1.f / angle_range;

    గుర్తులేని stride = surf->extents.stride;
    పూర్ణాంకం ox = surf->extents.x_origin;
    పూర్ణాంకం oy = surf->extents.y_origin;

    ఒకవేళ (clip.is_rect)
    {
      కొరకు (గుర్తులేని py = clip.min_y; py < clip.max_y; py++)
      {
	hb_packed_t<uint32_t> *అడ్డువరుస = (hb_packed_t<uint32_t> *) (surf->బఫరు.arrayZ + py * stride);
	దశాంశం gx = inv_xx * ((దశాంశం) ((పూర్ణాంకం) clip.min_x + ox) + 0.5f) + inv_xy * ((దశాంశం) ((పూర్ణాంకం) py + oy) + 0.5f) + inv_x0;
	దశాంశం gy = inv_yx * ((దశాంశం) ((పూర్ణాంకం) clip.min_x + ox) + 0.5f) + inv_yy * ((దశాంశం) ((పూర్ణాంకం) py + oy) + 0.5f) + inv_y0;
	ఒకవేళ (use_lut)
	{
	  కొరకు (గుర్తులేని px = clip.min_x; px < clip.max_x; px++)
	  {
	    దశాంశం angle = atan2f (gy - cy, gx - cx);
	    ఒకవేళ (angle < 0) angle += (దశాంశం) HB_2_PI;
	    దశాంశం grad_t = (angle - a0) * inv_angle_range;
	    uint32_t src = lookup_gradient_lut (lut, grad_t, extend);
	    అడ్డువరుస[px] = hb_packed_t<uint32_t> (hb_raster_src_over (src, (uint32_t) అడ్డువరుస[px]));
	    gx += inv_xx;
	    gy += inv_yx;
	  }
	}
	లేకపోతే
	{
	  కొరకు (గుర్తులేని px = clip.min_x; px < clip.max_x; px++)
	  {
	    దశాంశం angle = atan2f (gy - cy, gx - cx);
	    ఒకవేళ (angle < 0) angle += (దశాంశం) HB_2_PI;
	    దశాంశం grad_t = (angle - a0) * inv_angle_range;
	    uint32_t src = evaluate_color_పంక్తి (stops, len, grad_t, extend);
	    అడ్డువరుస[px] = hb_packed_t<uint32_t> (hb_raster_src_over (src, (uint32_t) అడ్డువరుస[px]));
	    gx += inv_xx;
	    gy += inv_yx;
	  }
	}
      }
    }
    లేకపోతే
    {
      కొరకు (గుర్తులేని py = clip.min_y; py < clip.max_y; py++)
      {
	hb_packed_t<uint32_t> *అడ్డువరుస = (hb_packed_t<uint32_t> *) (surf->బఫరు.arrayZ + py * stride);
	స్థిరరాశి uint8_t *clip_అడ్డువరుస = clip.alpha.arrayZ + py * clip.stride;
	దశాంశం gx = inv_xx * ((దశాంశం) ((పూర్ణాంకం) clip.min_x + ox) + 0.5f) + inv_xy * ((దశాంశం) ((పూర్ణాంకం) py + oy) + 0.5f) + inv_x0;
	దశాంశం gy = inv_yx * ((దశాంశం) ((పూర్ణాంకం) clip.min_x + ox) + 0.5f) + inv_yy * ((దశాంశం) ((పూర్ణాంకం) py + oy) + 0.5f) + inv_y0;
	ఒకవేళ (use_lut)
	{
	  కొరకు (గుర్తులేని px = clip.min_x; px < clip.max_x; px++)
	  {
	    uint8_t clip_alpha = clip_అడ్డువరుస[px];
	    ఒకవేళ (clip_alpha == 0)
	    {
	      gx += inv_xx;
	      gy += inv_yx;
	      కొనసాగు;
	    }
	    దశాంశం angle = atan2f (gy - cy, gx - cx);
	    ఒకవేళ (angle < 0) angle += (దశాంశం) HB_2_PI;
	    దశాంశం grad_t = (angle - a0) * inv_angle_range;
	    uint32_t src = lookup_gradient_lut (lut, grad_t, extend);
	    src = hb_raster_alpha_mul (src, clip_alpha);
	    అడ్డువరుస[px] = hb_packed_t<uint32_t> (hb_raster_src_over (src, (uint32_t) అడ్డువరుస[px]));
	    gx += inv_xx;
	    gy += inv_yx;
	  }
	}
	లేకపోతే
	{
	  కొరకు (గుర్తులేని px = clip.min_x; px < clip.max_x; px++)
	  {
	    uint8_t clip_alpha = clip_అడ్డువరుస[px];
	    ఒకవేళ (clip_alpha == 0)
	    {
	      gx += inv_xx;
	      gy += inv_yx;
	      కొనసాగు;
	    }
	    దశాంశం angle = atan2f (gy - cy, gx - cx);
	    ఒకవేళ (angle < 0) angle += (దశాంశం) HB_2_PI;
	    దశాంశం grad_t = (angle - a0) * inv_angle_range;
	    uint32_t src = evaluate_color_పంక్తి (stops, len, grad_t, extend);
	    src = hb_raster_alpha_mul (src, clip_alpha);
	    అడ్డువరుస[px] = hb_packed_t<uint32_t> (hb_raster_src_over (src, (uint32_t) అడ్డువరుస[px]));
	    gx += inv_xx;
	    gy += inv_yx;
	  }
	}
      }
    }
  }

}

స్థిర hb_bool_t
hb_raster_paint_custom_palette_color (hb_paint_funcs_t *funcs HB_వాడని,
				      శూన్య *paint_దత్తాంశం,
				      గుర్తులేని పూర్ణాంకం color_సూచీ,
				      hb_color_t *color,
				      శూన్య *user_దత్తాంశం HB_వాడని)
{
  hb_raster_paint_t *c = (hb_raster_paint_t *) paint_దత్తాంశం;
  ఒకవేళ (likely (c->custom_palette && hb_map_has (c->custom_palette, color_సూచీ)))
  {
    *color = hb_map_get (c->custom_palette, color_సూచీ);
    తిరిగి నిజం;
  }
  తిరిగి అబద్ధం;
}

స్థిర hb_bool_t
hb_raster_paint_సమితి_budget (hb_paint_funcs_t *, శూన్య *paint_దత్తాంశం,
			    int64_t budget, శూన్య *)
{
  స్వయంచాలక *paint = (hb_raster_paint_t *) paint_దత్తాంశం;
  paint->budget = budget;
  paint->recharge_budget ();
  తిరిగి నిజం;
}

స్థిర int64_t
hb_raster_paint_get_budget (hb_paint_funcs_t *, శూన్య *paint_దత్తాంశం, శూన్య *)
{
  తిరిగి ((hb_raster_paint_t *) paint_దత్తాంశం)->budget;
}

స్థిర int64_t *
hb_raster_paint_get_budget_మిగిలిన (hb_paint_funcs_t *, శూన్య *paint_దత్తాంశం, శూన్య *)
{
  తిరిగి &((hb_raster_paint_t *) paint_దత్తాంశం)->budget_మిగిలిన;
}


/*
 * Lazy-loader singleton for paint funcs
 */

స్థిర అంతఃవరుస శూన్య free_static_raster_paint_funcs ();

స్థిర నిర్మాణం hb_raster_paint_funcs_lazy_loader_t : hb_paint_funcs_lazy_loader_t<hb_raster_paint_funcs_lazy_loader_t>
{
  స్థిర hb_paint_funcs_t *create ()
  {
    hb_paint_funcs_t *funcs = hb_paint_funcs_create ();

    hb_paint_funcs_సమితి_నెట్టు_transform_func (funcs, hb_raster_paint_నెట్టు_transform, శూన్యసూచిక, శూన్యసూచిక);
    hb_paint_funcs_సమితి_తీయి_transform_func (funcs, hb_raster_paint_తీయి_transform, శూన్యసూచిక, శూన్యసూచిక);
    hb_paint_funcs_సమితి_fill_glyph_func (funcs, hb_raster_paint_fill_glyph, శూన్యసూచిక, శూన్యసూచిక);
    hb_paint_funcs_సమితి_color_glyph_func (funcs, hb_raster_paint_color_glyph, శూన్యసూచిక, శూన్యసూచిక);
    hb_paint_funcs_సమితి_నెట్టు_clip_glyph_func (funcs, hb_raster_paint_నెట్టు_clip_glyph, శూన్యసూచిక, శూన్యసూచిక);
    hb_paint_funcs_సమితి_నెట్టు_clip_rectangle_func (funcs, hb_raster_paint_నెట్టు_clip_rectangle, శూన్యసూచిక, శూన్యసూచిక);
    hb_paint_funcs_సమితి_నెట్టు_clip_మార్గం_ప్రారంభం_func (funcs, hb_raster_paint_నెట్టు_clip_మార్గం_ప్రారంభం, శూన్యసూచిక, శూన్యసూచిక);
    hb_paint_funcs_సమితి_నెట్టు_clip_మార్గం_చివర_func (funcs, hb_raster_paint_నెట్టు_clip_మార్గం_చివర, శూన్యసూచిక, శూన్యసూచిక);
    hb_paint_funcs_సమితి_తీయి_clip_func (funcs, hb_raster_paint_తీయి_clip, శూన్యసూచిక, శూన్యసూచిక);
    hb_paint_funcs_సమితి_నెట్టు_సమూహం_func (funcs, hb_raster_paint_నెట్టు_సమూహం, శూన్యసూచిక, శూన్యసూచిక);
    hb_paint_funcs_సమితి_తీయి_సమూహం_func (funcs, hb_raster_paint_తీయి_సమూహం, శూన్యసూచిక, శూన్యసూచిక);
    hb_paint_funcs_సమితి_color_func (funcs, hb_raster_paint_color, శూన్యసూచిక, శూన్యసూచిక);
    hb_paint_funcs_సమితి_image_func (funcs, hb_raster_paint_image, శూన్యసూచిక, శూన్యసూచిక);
    hb_paint_funcs_సమితి_రేఖీయ_gradient_func (funcs, hb_raster_paint_రేఖీయ_gradient, శూన్యసూచిక, శూన్యసూచిక);
    hb_paint_funcs_సమితి_radial_gradient_func (funcs, hb_raster_paint_radial_gradient, శూన్యసూచిక, శూన్యసూచిక);
    hb_paint_funcs_సమితి_sweep_gradient_func (funcs, hb_raster_paint_sweep_gradient, శూన్యసూచిక, శూన్యసూచిక);
    hb_paint_funcs_సమితి_custom_palette_color_func (funcs, hb_raster_paint_custom_palette_color, శూన్యసూచిక, శూన్యసూచిక);
    hb_paint_funcs_సమితి_సమితి_budget_func (funcs, hb_raster_paint_సమితి_budget, శూన్యసూచిక, శూన్యసూచిక);
    hb_paint_funcs_సమితి_get_budget_func (funcs, hb_raster_paint_get_budget, శూన్యసూచిక, శూన్యసూచిక);
    hb_paint_funcs_సమితి_get_budget_మిగిలిన_func (funcs, hb_raster_paint_get_budget_మిగిలిన, శూన్యసూచిక, శూన్యసూచిక);

    hb_paint_funcs_make_immutable (funcs);

    hb_atexit (free_static_raster_paint_funcs);

    తిరిగి funcs;
  }
} static_raster_paint_funcs;

స్థిర అంతఃవరుస శూన్య
free_static_raster_paint_funcs ()
{
  static_raster_paint_funcs.free_instance ();
}


/*
 * Public API
 */

/**
 * hb_raster_paint_create_or_fail:
 *
 * Creates a new color-glyph paint context.
 *
 * Return value: (transfer full):
 * A newly allocated #hb_raster_paint_t, or `NULL` on allocation failure.
 *
 * Since: 13.0.0
 **/
hb_raster_paint_t *
hb_raster_paint_create_or_fail (శూన్య)
{
  hb_raster_paint_t *paint = hb_object_create<hb_raster_paint_t> ();
  ఒకవేళ (unlikely (!paint))
    తిరిగి శూన్యసూచిక;

  paint->clip_rdr = hb_raster_draw_create_or_fail ();
  ఒకవేళ (unlikely (!paint->clip_rdr))
  {
    hb_free (paint);
    తిరిగి శూన్యసూచిక;
  }

  తిరిగి paint;
}

/**
 * hb_raster_paint_reference: (skip)
 * @paint: a paint context
 *
 * Increases the reference count on @paint by one.
 *
 * Return value: (transfer full):
 * The referenced #hb_raster_paint_t.
 *
 * Since: 13.0.0
 **/
hb_raster_paint_t *
hb_raster_paint_reference (hb_raster_paint_t *paint)
{
  తిరిగి hb_object_reference (paint);
}

/**
 * hb_raster_paint_destroy: (skip)
 * @paint: a paint context
 *
 * Decreases the reference count on @paint by one. When the
 * reference count reaches zero, the paint context is freed.
 *
 * Since: 13.0.0
 **/
శూన్య
hb_raster_paint_destroy (hb_raster_paint_t *paint)
{
  ఒకవేళ (!hb_object_should_destroy (paint))
    తిరిగి;

  hb_map_destroy (paint->custom_palette);
  hb_raster_draw_destroy (paint->clip_rdr);
  కొరకు (స్వయంచాలక *s : paint->surface_దొంతర)
    hb_raster_image_destroy (s);
  కొరకు (స్వయంచాలక *s : paint->surface_cache)
    hb_raster_image_destroy (s);
  hb_object_actually_destroy (paint);
  hb_free (paint);
}

/**
 * hb_raster_paint_set_user_data: (skip)
 * @paint: a paint context
 * @key: the user-data key
 * @data: a pointer to the user data
 * @destroy: (nullable): a callback to call when @data is not needed anymore
 * @replace: whether to replace an existing data with the same key
 *
 * Attaches a user-data key/data pair to the specified paint context.
 *
 * Return value: `true` if success, `false` otherwise
 *
 * Since: 13.0.0
 **/
hb_bool_t
hb_raster_paint_సమితి_user_దత్తాంశం (hb_raster_paint_t  *paint,
			       hb_user_దత్తాంశం_కీలకం_t *key,
			       శూన్య               *దత్తాంశం,
			       hb_destroy_func_t   destroy,
			       hb_bool_t           replace)
{
  తిరిగి hb_object_సమితి_user_దత్తాంశం (paint, key, దత్తాంశం, destroy, replace);
}

/**
 * hb_raster_paint_get_user_data: (skip)
 * @paint: a paint context
 * @key: the user-data key
 *
 * Fetches the user-data associated with the specified key,
 * attached to the specified paint context.
 *
 * Return value: (transfer none):
 * A pointer to the user data
 *
 * Since: 13.0.0
 **/
శూన్య *
hb_raster_paint_get_user_దత్తాంశం (స్థిరరాశి hb_raster_paint_t  *paint,
			       hb_user_దత్తాంశం_కీలకం_t *key)
{
  తిరిగి hb_object_get_user_దత్తాంశం (paint, key);
}

/**
 * hb_raster_paint_set_transform:
 * @paint: a paint context
 * @xx: xx component of the transform matrix
 * @yx: yx component of the transform matrix
 * @xy: xy component of the transform matrix
 * @yy: yy component of the transform matrix
 * @dx: x translation
 * @dy: y translation
 *
 * Sets the base 2×3 affine transform that maps from glyph-space
 * coordinates to pixel-space coordinates.
 *
 * Since: 13.0.0
 **/
శూన్య
hb_raster_paint_సమితి_transform (hb_raster_paint_t *paint,
			       దశాంశం xx, దశాంశం yx,
			       దశాంశం xy, దశాంశం yy,
			       దశాంశం dx, దశాంశం dy)
{
  paint->పునాది_transform = {xx, yx, xy, yy, dx, dy};
}

/**
 * hb_raster_paint_get_transform:
 * @paint: a paint context
 * @xx: (out) (nullable): xx component of the transform matrix
 * @yx: (out) (nullable): yx component of the transform matrix
 * @xy: (out) (nullable): xy component of the transform matrix
 * @yy: (out) (nullable): yy component of the transform matrix
 * @dx: (out) (nullable): x translation
 * @dy: (out) (nullable): y translation
 *
 * Gets the current base 2x3 affine transform.
 *
 * Since: 13.0.0
 **/
శూన్య
hb_raster_paint_get_transform (స్థిరరాశి hb_raster_paint_t *paint,
			       దశాంశం *xx, దశాంశం *yx,
			       దశాంశం *xy, దశాంశం *yy,
			       దశాంశం *dx, దశాంశం *dy)
{
  ఒకవేళ (xx) *xx = paint->పునాది_transform.xx;
  ఒకవేళ (yx) *yx = paint->పునాది_transform.yx;
  ఒకవేళ (xy) *xy = paint->పునాది_transform.xy;
  ఒకవేళ (yy) *yy = paint->పునాది_transform.yy;
  ఒకవేళ (dx) *dx = paint->పునాది_transform.x0;
  ఒకవేళ (dy) *dy = paint->పునాది_transform.y0;
}

/**
 * hb_raster_paint_set_scale_factor:
 * @paint: a paint context
 * @x_scale_factor: x-axis minification factor
 * @y_scale_factor: y-axis minification factor
 *
 * Sets post-transform minification factors applied during painting.
 * Factors larger than 1 shrink the output in pixels. The default is 1.
 *
 * Since: 13.0.0
 **/
శూన్య
hb_raster_paint_సమితి_scale_గుణకం (hb_raster_paint_t *paint,
				  దశాంశం x_scale_గుణకం,
				  దశాంశం y_scale_గుణకం)
{
  paint->x_scale_గుణకం = x_scale_గుణకం > 0.f ? x_scale_గుణకం : 1.f;
  paint->y_scale_గుణకం = y_scale_గుణకం > 0.f ? y_scale_గుణకం : 1.f;
}

/**
 * hb_raster_paint_get_scale_factor:
 * @paint: a paint context
 * @x_scale_factor: (out) (nullable): x-axis minification factor
 * @y_scale_factor: (out) (nullable): y-axis minification factor
 *
 * Fetches the current post-transform minification factors.
 *
 * Since: 13.0.0
 **/
శూన్య
hb_raster_paint_get_scale_గుణకం (స్థిరరాశి hb_raster_paint_t *paint,
				  దశాంశం *x_scale_గుణకం,
				  దశాంశం *y_scale_గుణకం)
{
  ఒకవేళ (x_scale_గుణకం) *x_scale_గుణకం = paint->x_scale_గుణకం;
  ఒకవేళ (y_scale_గుణకం) *y_scale_గుణకం = paint->y_scale_గుణకం;
}

/**
 * hb_raster_paint_set_extents:
 * @paint: a paint context
 * @extents: the desired output extents
 *
 * Sets the output image extents (pixel rectangle).
 *
 * Call this before hb_font_paint_glyph() for each render.
 * A common pattern is:
 * |[<!-- language="plain" -->
 * hb_glyph_extents_t gext;
 * if (hb_font_get_glyph_extents (font, gid, &gext))
 *   hb_raster_paint_set_glyph_extents (paint, &gext);
 * ]|
 *
 * Since: 13.0.0
 **/
శూన్య
hb_raster_paint_సమితి_extents (hb_raster_paint_t         *paint,
			     స్థిరరాశి hb_raster_extents_t *extents)
{
  paint->fixed_extents = *extents;
  paint->has_extents = నిజం;
  ఒకవేళ (paint->fixed_extents.stride == 0)
    paint->fixed_extents.stride = paint->fixed_extents.వెడల్పు * 4;
}

/**
 * hb_raster_paint_get_extents:
 * @paint: a paint context
 * @extents: (out) (nullable): where to write current extents
 *
 * Gets currently configured output extents.
 *
 * Return value: `true` if extents are set, `false` otherwise.
 *
 * Since: 13.0.0
 **/
hb_bool_t
hb_raster_paint_get_extents (స్థిరరాశి hb_raster_paint_t   *paint,
			     hb_raster_extents_t *extents)
{
  ఒకవేళ (!paint->has_extents)
    తిరిగి అబద్ధం;

  ఒకవేళ (extents)
    *extents = paint->fixed_extents;
  తిరిగి నిజం;
}

/**
 * hb_raster_paint_set_glyph_extents:
 * @paint: a paint context
 * @glyph_extents: glyph extents from hb_font_get_glyph_extents()
 *
 * Transforms @glyph_extents with the paint context's base transform and
 * sets the resulting output image extents.
 *
 * This is equivalent to computing a transformed bounding box in pixel
 * space and calling hb_raster_paint_set_extents().
 *
 * The resulting dimensions are capped at 4096 pixels per side.
 *
 * Return value: `true` if transformed extents are non-empty and set;
 * `false` otherwise.
 *
 * Since: 13.0.0
 **/
hb_bool_t
hb_raster_paint_సమితి_glyph_extents (hb_raster_paint_t        *paint,
				   స్థిరరాశి hb_glyph_extents_t *glyph_extents)
{
  దశాంశం x0 = (దశాంశం) glyph_extents->x_bearing;
  దశాంశం y0 = (దశాంశం) glyph_extents->y_bearing;
  దశాంశం x1 = (దశాంశం) glyph_extents->x_bearing + glyph_extents->వెడల్పు;
  దశాంశం y1 = (దశాంశం) glyph_extents->y_bearing + glyph_extents->ఎత్తు;

  దశాంశం xmin = hb_min (x0, x1);
  దశాంశం xmax = hb_max (x0, x1);
  దశాంశం ymin = hb_min (y0, y1);
  దశాంశం ymax = hb_max (y0, y1);

  దశాంశం px[4] = {xmin, xmin, xmax, xmax};
  దశాంశం py[4] = {ymin, ymax, ymin, ymax};

  hb_transform_t<> t = paint->పునాది_transform;
  paint->apply_scale_గుణకం (t);

  దశాంశం tx, ty;
  t.transform_బిందువు (px[0], py[0]);
  tx = px[0]; ty = py[0];
  దశాంశం tx_min = tx, tx_max = tx;
  దశాంశం ty_min = ty, ty_max = ty;

  కొరకు (గుర్తులేని i = 1; i < 4; i++)
  {
    t.transform_బిందువు (px[i], py[i]);
    tx_min = hb_min (tx_min, px[i]);
    tx_max = hb_max (tx_max, px[i]);
    ty_min = hb_min (ty_min, py[i]);
    ty_max = hb_max (ty_max, py[i]);
  }

  int32_t ex0 = hb_clamp_గమ్యం<int32_t> (floorf (tx_min));
  int32_t ey0 = hb_clamp_గమ్యం<int32_t> (floorf (ty_min));
  int32_t ex1 = hb_clamp_గమ్యం<int32_t> (ceilf  (tx_max));
  int32_t ey1 = hb_clamp_గమ్యం<int32_t> (ceilf  (ty_max));

  ఒకవేళ (ex1 <= ex0 || ey1 <= ey0)
  {
    paint->fixed_extents = {};
    paint->has_extents = అబద్ధం;
    తిరిగి అబద్ధం;
  }

  paint->fixed_extents = {
    ex0, ey0,
    (గుర్తులేని) hb_min ((int64_t) ex1 - ex0, (int64_t) HB_RASTER_MAX_AUTO_DIMENSION),
    (గుర్తులేని) hb_min ((int64_t) ey1 - ey0, (int64_t) HB_RASTER_MAX_AUTO_DIMENSION),
    0
  };
  paint->has_extents = నిజం;
  ఒకవేళ (paint->fixed_extents.stride == 0)
    paint->fixed_extents.stride = paint->fixed_extents.వెడల్పు * 4;
  తిరిగి నిజం;
}

/**
 * hb_raster_paint_set_foreground:
 * @paint: a paint context
 * @foreground: the foreground color
 *
 * Sets the foreground color used when paint callbacks request it
 * (e.g. `is_foreground` in color stops or solid fills).
 *
 * Since: 13.0.0
 **/
శూన్య
hb_raster_paint_సమితి_foreground (hb_raster_paint_t *paint,
				hb_color_t         foreground)
{
  paint->foreground = foreground;
}

/**
 * hb_raster_paint_get_foreground:
 * @paint: a paint context
 *
 * Returns the foreground color previously set on @paint, or the
 * default opaque black if none was set.
 *
 * Return value: the foreground color.
 *
 * Since: 14.2.0
 **/
hb_color_t
hb_raster_paint_get_foreground (స్థిరరాశి hb_raster_paint_t *paint)
{
  తిరిగి paint->foreground;
}

/**
 * hb_raster_paint_set_background:
 * @paint: a paint context
 * @background: the background color
 *
 * Sets the background color for @paint.  If set to a non-transparent
 * value, the rendered image is pre-filled with this color before
 * glyph content is composited on top.  Default is transparent.
 *
 * Since: 14.2.0
 **/
శూన్య
hb_raster_paint_సమితి_background (hb_raster_paint_t *paint,
				hb_color_t         background)
{
  paint->background = background;
}

/**
 * hb_raster_paint_get_background:
 * @paint: a paint context
 *
 * Returns the background color previously set on @paint, or
 * transparent if none was set.
 *
 * Return value: the background color.
 *
 * Since: 14.2.0
 **/
hb_color_t
hb_raster_paint_get_background (స్థిరరాశి hb_raster_paint_t *paint)
{
  తిరిగి paint->background;
}

/**
 * hb_raster_paint_set_palette:
 * @paint: a paint context
 * @palette: palette index
 *
 * Selects which font palette is used when paint callbacks look up
 * indexed colors.  Default is palette 0.
 *
 * Since: 14.2.0
 **/
శూన్య
hb_raster_paint_సమితి_palette (hb_raster_paint_t *paint,
			     గుర్తులేని           palette)
{
  paint->palette = palette;
}

/**
 * hb_raster_paint_get_palette:
 * @paint: a paint context
 *
 * Returns the palette index previously set on @paint, or 0 if none
 * was set.
 *
 * Return value: the palette index.
 *
 * Since: 14.2.0
 **/
గుర్తులేని
hb_raster_paint_get_palette (స్థిరరాశి hb_raster_paint_t *paint)
{
  తిరిగి paint->palette;
}

/**
 * hb_raster_paint_clear_custom_palette_colors:
 * @paint: a paint context.
 *
 * Clears all custom palette color overrides previously set on @paint.
 *
 * After this call, palette lookups use the selected font palette without
 * custom override entries.
 *
 * Since: 13.0.0
 **/
శూన్య
hb_raster_paint_clear_custom_palette_colors (hb_raster_paint_t *paint)
{
  ఒకవేళ (paint->custom_palette)
    hb_map_clear (paint->custom_palette);
}

/**
 * hb_raster_paint_set_custom_palette_color:
 * @paint: a paint context.
 * @color_index: color index to override.
 * @color: replacement color.
 *
 * Overrides one font palette color entry for subsequent paint operations.
 * Overrides are keyed by @color_index and persist on @paint until cleared
 * (or replaced for the same index).
 *
 * These overrides are consulted by paint operations that resolve CPAL
 * entries.
 *
 * Return value: `true` if the override was set; `false` on allocation failure.
 *
 * Since: 13.0.0
 **/
hb_bool_t
hb_raster_paint_సమితి_custom_palette_color (hb_raster_paint_t *paint,
					  గుర్తులేని పూర్ణాంకం       color_సూచీ,
					  hb_color_t         color)
{
  ఒకవేళ (unlikely (!paint->custom_palette))
  {
    paint->custom_palette = hb_map_create ();
    ఒకవేళ (unlikely (!paint->custom_palette))
      తిరిగి అబద్ధం;
  }
  hb_map_సమితి (paint->custom_palette, color_సూచీ, color);
  తిరిగి hb_map_కేటాయింపు_successful (paint->custom_palette);
}

/**
 * hb_raster_paint_get_funcs:
 * @paint: a rasterizer paint context.
 *
 * Fetches the #hb_paint_funcs_t that renders color glyphs into
 * @paint.  Pass @paint as the @paint_data argument when calling
 * hb_font_paint_glyph().
 *
 * Return value: (transfer none):
 * The rasterizer paint functions
 *
 * Since: 14.2.0
 **/
hb_paint_funcs_t *
hb_raster_paint_get_funcs (స్థిరరాశి hb_raster_paint_t *paint HB_వాడని)
{
  తిరిగి static_raster_paint_funcs.get_unconst ();
}

స్థిర hb_bool_t
hb_raster_paint_glyph_impl (hb_raster_paint_t *paint,
			    hb_font_t        *font,
			    hb_codepoint_t    glyph,
			    hb_bool_t         fallible)
{
  ఒకవేళ (!paint->has_extents)
  {
    hb_glyph_extents_t ge;
    ఒకవేళ (hb_font_get_glyph_extents (font, glyph, &ge))
      hb_raster_paint_సమితి_glyph_extents (paint, &ge);
  }

  hb_paint_funcs_t *funcs = hb_raster_paint_get_funcs (paint);
  hb_paint_నెట్టు_transform (funcs, paint,
			   paint->పునాది_transform.xx, paint->పునాది_transform.yx,
			   paint->పునాది_transform.xy, paint->పునాది_transform.yy,
			   paint->పునాది_transform.x0, paint->పునాది_transform.y0);

  hb_bool_t ret = నిజం;
  ఒకవేళ (fallible)
    ret = hb_font_paint_glyph_or_fail (font, glyph,
				       funcs, paint,
				       paint->palette, paint->foreground);
  లేకపోతే
    hb_font_paint_glyph (font, glyph,
			 funcs, paint,
			 paint->palette, paint->foreground);

  hb_paint_తీయి_transform (funcs, paint);
  తిరిగి ret;
}

/**
 * hb_raster_paint_glyph_or_fail:
 * @paint: a paint context
 * @font: font to paint from
 * @glyph: glyph ID to paint
 *
 * Convenience to paint one color glyph.  Equivalent to:
 *
 * |[<!-- language="plain" -->
 * hb_font_paint_glyph_or_fail (font, glyph,
 *   hb_raster_paint_get_funcs (paint), paint,
 *   palette, foreground);
 * ]|
 *
 * Return value: `true` if painting succeeded, `false` otherwise.
 *
 * Since: 14.2.0
 **/
hb_bool_t
hb_raster_paint_glyph_or_fail (hb_raster_paint_t *paint,
			       hb_font_t        *font,
			       hb_codepoint_t    glyph)
{
  తిరిగి hb_raster_paint_glyph_impl (paint, font, glyph, నిజం);
}

/**
 * hb_raster_paint_glyph:
 * @paint: a paint context
 * @font: font to paint from
 * @glyph: glyph ID to paint
 *
 * Paints one glyph into @paint.  Unlike
 * hb_raster_paint_glyph_or_fail(), glyphs with no color paint data
 * fall back to a synthesized foreground-colored outline, so any
 * glyph with an outline or bitmap image produces output.
 *
 * Since: 14.2.0
 **/
శూన్య
hb_raster_paint_glyph (hb_raster_paint_t *paint,
		       hb_font_t        *font,
		       hb_codepoint_t    glyph)
{
  hb_raster_paint_glyph_impl (paint, font, glyph, అబద్ధం);
}

/**
 * hb_raster_paint_render:
 * @paint: a paint context
 *
 * Extracts the rendered image after hb_font_paint_glyph() has
 * completed.  The paint context's surface stack is consumed and
 * the result returned as a new #hb_raster_image_t. Output format is
 * always @HB_RASTER_FORMAT_BGRA32.
 *
 * Call hb_font_paint_glyph() before calling this function.
 * hb_raster_paint_set_extents() or hb_raster_paint_set_glyph_extents()
 * must be called before painting; otherwise this function returns `NULL`.
 * Internal drawing state is cleared here so the same object can
 * be reused without client-side clearing.
 *
 * Return value: (transfer full):
 * A rendered #hb_raster_image_t. Returns `NULL` if extents were not set
 * or if allocation/configuration fails. If extents were set but nothing
 * was painted, returns an empty image.
 *
 * Since: 13.0.0
 **/
hb_raster_image_t *
hb_raster_paint_render (hb_raster_paint_t *paint)
{
  /* Common per-exit state reset.  Runs on every return path,
   * including the early-return failure cases below. */
  HB_SCOPE_GUARD (hb_raster_paint_clear (paint));

  ఒకవేళ (unlikely (!paint->has_extents))
    తిరిగి శూన్యసూచిక;

  hb_raster_image_t *ఫలితం;
  ఒకవేళ (paint->surface_దొంతర.నిడివి)
  {
    ఫలితం = paint->surface_దొంతర[0];
    /* Release any remaining group surfaces (shouldn't happen with
     * well-formed paint calls, but be safe). */
    కొరకు (గుర్తులేని i = 1; i < paint->surface_దొంతర.నిడివి; i++)
      paint->release_surface (paint->surface_దొంతర[i]);
    paint->surface_దొంతర.clear ();
  }
  లేకపోతే
  {
    ఫలితం = paint->acquire_surface ();
    ఒకవేళ (unlikely (!ఫలితం))
      తిరిగి శూన్యసూచిక;
  }

  తిరిగి ఫలితం;
}

/**
 * hb_raster_paint_clear:
 * @paint: a paint context
 *
 * Discards accumulated paint output so @paint can be reused for
 * another render.  User configuration (base transform, scale
 * factors, foreground, background, palette, custom palette colors)
 * is preserved.  Call hb_raster_paint_reset() to also reset user
 * configuration to defaults.
 *
 * Since: 14.2.0
 **/
శూన్య
hb_raster_paint_clear (hb_raster_paint_t *paint)
{
  paint->fixed_extents = {};
  paint->has_extents = అబద్ధం;
  paint->transform_దొంతర.clear ();
  paint->release_all_clips ();
  కొరకు (స్వయంచాలక *s : paint->surface_దొంతర)
    paint->release_surface (s);
  paint->surface_దొంతర.clear ();
  paint->recharge_budget ();
  hb_raster_draw_reset (paint->clip_rdr);
}

/**
 * hb_raster_paint_reset:
 * @paint: a paint context
 *
 * Resets the paint context to its initial state, clearing all
 * configuration while preserving internal image caches.
 *
 * Since: 13.0.0
 **/
శూన్య
hb_raster_paint_reset (hb_raster_paint_t *paint)
{
  paint->పునాది_transform = {1, 0, 0, 1, 0, 0};
  paint->x_scale_గుణకం = 1.f;
  paint->y_scale_గుణకం = 1.f;
  paint->foreground = HB_COLOR (0, 0, 0, 255);
  paint->background = HB_COLOR (0, 0, 0, 0);
  paint->palette = 0;
  paint->budget = HB_BUDGET_DEFAULT;
  hb_raster_paint_clear_custom_palette_colors (paint);
  hb_raster_paint_clear (paint);
}

/**
 * hb_raster_paint_recycle_image:
 * @paint: a paint context
 * @image: a raster image to recycle
 *
 * Recycles @image for reuse by subsequent render calls.
 * The caller transfers ownership of @image to @paint.
 *
 * Since: 13.0.0
 **/
శూన్య
hb_raster_paint_recycle_image (hb_raster_paint_t  *paint,
			       hb_raster_image_t  *image)
{
  paint->release_surface (image);
}
