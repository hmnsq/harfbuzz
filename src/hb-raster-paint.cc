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

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-raster-paint.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-machinery.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-paint.hh"

#ਅਨ੍ਤਰ੍ਭਾਵ <math.h>


/*
 * Pixel helpers (paint-specific)
 */

/* Convert unpremultiplied hb_color_t (BGRA order) to premultiplied BGRA32 pixel. */
ਸਥਿਰ ਸਤਰਅੰਦਰ uint32_t
color_ਮੰਜ਼ਿਲ_premul_pixel (hb_color_t color)
{
  uint8_t a = hb_color_get_alpha (color);
  uint8_t r = hb_raster_div255 (hb_color_get_red (color) * a);
  uint8_t g = hb_raster_div255 (hb_color_get_green (color) * a);
  uint8_t b = hb_raster_div255 (hb_color_get_blue (color) * a);
  ਵਾਪਸ (uint32_t) b | ((uint32_t) g << 8) | ((uint32_t) r << 16) | ((uint32_t) a << 24);
}

/* Bilinear sample from a premultiplied BGRA32 image. */
ਸਥਿਰ ਸਤਰਅੰਦਰ uint32_t
hb_raster_sample_bilinear_premul (ਅਚਲ hb_packed_t<uint32_t> *src,
				  ਬਿਨਾਨਿਸ਼ਾਨੀ ਚੌੜਾਈ,
				  ਬਿਨਾਨਿਸ਼ਾਨੀ ਉਚਾਈ,
				  ਦਸ਼ਮਲਵ x,
				  ਦਸ਼ਮਲਵ y)
{
  ਪੂਰਨ x0 = (ਪੂਰਨ) floorf (x);
  ਪੂਰਨ y0 = (ਪੂਰਨ) floorf (y);
  ਪੂਰਨ x1 = x0 + 1;
  ਪੂਰਨ y1 = y0 + 1;
  ਜੇ (x1 >= (ਪੂਰਨ) ਚੌੜਾਈ) x1 = (ਪੂਰਨ) ਚੌੜਾਈ - 1;
  ਜੇ (y1 >= (ਪੂਰਨ) ਉਚਾਈ) y1 = (ਪੂਰਨ) ਉਚਾਈ - 1;

  ਦਸ਼ਮਲਵ tx = x - x0;
  ਦਸ਼ਮਲਵ ty = y - y0;
  ਦਸ਼ਮਲਵ omtx = 1.f - tx;
  ਦਸ਼ਮਲਵ omty = 1.f - ty;

  uint32_t p00 = (uint32_t) src[(size_t) y0 * ਚੌੜਾਈ + (size_t) x0];
  uint32_t p10 = (uint32_t) src[(size_t) y0 * ਚੌੜਾਈ + (size_t) x1];
  uint32_t p01 = (uint32_t) src[(size_t) y1 * ਚੌੜਾਈ + (size_t) x0];
  uint32_t p11 = (uint32_t) src[(size_t) y1 * ਚੌੜਾਈ + (size_t) x1];

  ਦਸ਼ਮਲਵ w00 = omtx * omty;
  ਦਸ਼ਮਲਵ w10 = tx * omty;
  ਦਸ਼ਮਲਵ w01 = omtx * ty;
  ਦਸ਼ਮਲਵ w11 = tx * ty;

  ਦਸ਼ਮਲਵ b = ((p00 >>  0) & 0xff) * w00 + ((p10 >>  0) & 0xff) * w10 +
	    ((p01 >>  0) & 0xff) * w01 + ((p11 >>  0) & 0xff) * w11;
  ਦਸ਼ਮਲਵ g = ((p00 >>  8) & 0xff) * w00 + ((p10 >>  8) & 0xff) * w10 +
	    ((p01 >>  8) & 0xff) * w01 + ((p11 >>  8) & 0xff) * w11;
  ਦਸ਼ਮਲਵ r = ((p00 >> 16) & 0xff) * w00 + ((p10 >> 16) & 0xff) * w10 +
	    ((p01 >> 16) & 0xff) * w01 + ((p11 >> 16) & 0xff) * w11;
  ਦਸ਼ਮਲਵ a = ((p00 >> 24) & 0xff) * w00 + ((p10 >> 24) & 0xff) * w10 +
	    ((p01 >> 24) & 0xff) * w01 + ((p11 >> 24) & 0xff) * w11;

  ਵਾਪਸ (uint32_t) (b + 0.5f)
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
ਸਥਿਰ ਖਾਲੀ
ensure_initialized (hb_raster_paint_t *c)
{
  ਜੇ (c->surface_ਢੇਰ.ਲੰਮਾਈ) ਵਾਪਸ;

  /* A failed stack push (OOM) leaves the vector in a sticky error
   * state, so initialization can never succeed; bail out cheaply
   * instead of re-paying the surface acquire-and-clear below on
   * every subsequent paint callback. */
  ਜੇ (unlikely (c->surface_ਢੇਰ.ਵਿੱਚ_error () || c->clip_ਢੇਰ.ਵਿੱਚ_error ()))
    ਵਾਪਸ;

  /* acquire_surface() clears a full surface; charge its area, so
   * repeated failed initialization attempts cannot re-pay it
   * indefinitely.  Successful initialization re-arms the session
   * budget below, making the charge a no-op for normal sessions. */
  ਜੇ (unlikely (!c->charge_work ((int64_t) c->fixed_extents.ਚੌੜਾਈ *
				 c->fixed_extents.ਉਚਾਈ)))
    ਵਾਪਸ;

  /* Root surface */
  hb_raster_image_t *ਜੜ੍ਹ = c->acquire_surface ();
  ਜੇ (unlikely (!ਜੜ੍ਹ)) ਵਾਪਸ;

  ਜੇ (hb_color_get_alpha (c->background))
  {
    uint32_t bg = color_ਮੰਜ਼ਿਲ_premul_pixel (c->background);
    hb_raster_extents_t ext;
    hb_raster_image_get_extents (ਜੜ੍ਹ, &ext);
    ਅਚਲ uint8_t *buf = hb_raster_image_get_ਬਫਰ (ਜੜ੍ਹ);
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ y = 0; y < ext.ਉਚਾਈ; y++)
    {
      hb_packed_t<uint32_t> *ਪੰਕਤੀ =
	(hb_packed_t<uint32_t> *) (buf + (size_t) y * ext.stride);
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ x = 0; x < ext.ਚੌੜਾਈ; x++)
	ਪੰਕਤੀ[x] = hb_packed_t<uint32_t> (bg);
    }
  }

  ਜੇ (unlikely (!c->surface_ਢੇਰ.ਧੱਕੋ_or_fail (ਜੜ੍ਹ)))
  {
    c->release_surface (ਜੜ੍ਹ);
    ਵਾਪਸ;
  }

  /* Resolve the surface-sized pixel budget now that the surface size is
   * known.  An unlimited policy disables the pixel budget, so leave it.
   * The outline budget is re-resolved to the same value it already holds
   * (nothing has been drawn yet). */
  ਜੇ (c->budget != HB_BUDGET_UNLIMITED)
    c->recharge_budget ();

  /* Initial clip: full coverage rectangle */
  hb_raster_clip_t clip;
  clip.init_ਭਰਿਆ (c->fixed_extents.ਚੌੜਾਈ, c->fixed_extents.ਉਚਾਈ);
  ਜੇ (unlikely (!c->clip_ਢੇਰ.ਧੱਕੋ_or_fail (std::move (clip))))
  {
    c->release_surface (c->surface_ਢੇਰ.ਕੱਢੋ ());
    ਵਾਪਸ;
  }
}

ਸਥਿਰ ਖਾਲੀ
hb_raster_paint_ਧੱਕੋ_transform (hb_paint_funcs_t *pfuncs HB_ਅਣਵਰਤਿਆ,
				ਖਾਲੀ *paint_ਅੰਕੜੇ,
				ਦਸ਼ਮਲਵ xx, ਦਸ਼ਮਲਵ yx,
				ਦਸ਼ਮਲਵ xy, ਦਸ਼ਮਲਵ yy,
				ਦਸ਼ਮਲਵ dx, ਦਸ਼ਮਲਵ dy,
				ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  hb_raster_paint_t *c = (hb_raster_paint_t *) paint_ਅੰਕੜੇ;

  ensure_initialized (c);

  hb_transform_t<> t = c->current_transform ();
  t.multiply ({xx, yx, xy, yy, dx, dy});
  (ਖਾਲੀ) c->transform_ਢੇਰ.push (t);
}

ਸਥਿਰ ਖਾਲੀ
hb_raster_paint_ਕੱਢੋ_transform (hb_paint_funcs_t *pfuncs HB_ਅਣਵਰਤਿਆ,
			       ਖਾਲੀ *paint_ਅੰਕੜੇ,
			       ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  hb_raster_paint_t *c = (hb_raster_paint_t *) paint_ਅੰਕੜੇ;
  ਜੇ (c->transform_ਢੇਰ.ਲੰਮਾਈ)
    c->transform_ਢੇਰ.ਕੱਢੋ ();
}

ਸਥਿਰ hb_bool_t
hb_raster_paint_color_glyph (hb_paint_funcs_t *pfuncs HB_ਅਣਵਰਤਿਆ,
			     ਖਾਲੀ *paint_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ,
			     hb_codepoint_t glyph HB_ਅਣਵਰਤਿਆ,
			     hb_font_t *font HB_ਅਣਵਰਤਿਆ,
			     ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  ਵਾਪਸ ਝੂਠ;
}

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (*hb_raster_paint_clip_ਨਕਾਬ_emit_t) (hb_raster_draw_t *rdr, ਖਾਲੀ *user_ਅੰਕੜੇ);

/* Attach the surface box to @rdr and seed its outline budget before
 * outlines are drawn into it: curves outside the surface collapse to
 * their chord, and outline traversal and curve flattening charge the
 * seeded budget.  hb_raster_paint_readback_budget() pulls the remainder
 * back into the session afterwards. */
ਸਥਿਰ ਖਾਲੀ
hb_raster_paint_attach_clip_rdr (hb_raster_paint_t *c,
				 hb_raster_draw_t *rdr,
				 ਅਚਲ hb_raster_image_t *surf)
{
  hb_raster_draw_ਸੈੱਟ_clip_box (rdr,
			       (ਦਸ਼ਮਲਵ) surf->extents.x_origin,
			       (ਦਸ਼ਮਲਵ) surf->extents.y_origin,
			       (ਦਸ਼ਮਲਵ) surf->extents.x_origin + (ਦਸ਼ਮਲਵ) surf->extents.ਚੌੜਾਈ,
			       (ਦਸ਼ਮਲਵ) surf->extents.y_origin + (ਦਸ਼ਮਲਵ) surf->extents.ਉਚਾਈ);
  hb_draw_ਸੈੱਟ_budget (hb_raster_draw_get_funcs (rdr), rdr, c->budget_ਬਚਿਆ);
}

/* Pull the outline budget the rasterizer has consumed back into the
 * session, so it aggregates across every glyph in the paint walk. */
ਸਥਿਰ ਖਾਲੀ
hb_raster_paint_readback_budget (hb_raster_paint_t *c,
				 hb_raster_draw_t *rdr)
{
  c->budget_ਬਚਿਆ = hb_draw_get_budget_ਬਚਿਆ (hb_raster_draw_get_funcs (rdr), rdr);
}

ਸਥਿਰ ਖਾਲੀ
hb_raster_paint_ਧੱਕੋ_ਖਾਲੀ_ਥਾਵਾਂ_clip (hb_raster_paint_t *c, ਬਿਨਾਨਿਸ਼ਾਨੀ w, ਬਿਨਾਨਿਸ਼ਾਨੀ h)
{
  hb_raster_clip_t new_clip = c->acquire_clip (w, h);
  new_clip.init_ਭਰਿਆ (w, h);
  new_clip.is_rect = ਸੱਚ;
  new_clip.rect_x0 = new_clip.rect_y0 = 0;
  new_clip.rect_x1 = new_clip.rect_y1 = 0;
  new_clip.min_x = new_clip.min_y = new_clip.max_x = new_clip.max_y = 0;
  (ਖਾਲੀ) c->clip_ਢੇਰ.push (std::move (new_clip));
}

ਸਥਿਰ ਤਾਰਕਿਕ
hb_raster_paint_intersect_ਨਕਾਬ (ਅਚਲ hb_raster_image_t *surf,
				ਅਚਲ hb_raster_extents_t &ਨਕਾਬ_ext,
				ਅਚਲ hb_raster_clip_t &clip,
				ਬਿਨਾਨਿਸ਼ਾਨੀ w, ਬਿਨਾਨਿਸ਼ਾਨੀ h,
				int64_t *ਨਕਾਬ_x0, int64_t *ਨਕਾਬ_y0,
				ਬਿਨਾਨਿਸ਼ਾਨੀ *ix0, ਬਿਨਾਨਿਸ਼ਾਨੀ *iy0,
				ਬਿਨਾਨਿਸ਼ਾਨੀ *ix1, ਬਿਨਾਨਿਸ਼ਾਨੀ *iy1)
{
  /* Convert mask extents from surface coordinates to clip-buffer coordinates. */
  int64_t mx0 = (int64_t) ਨਕਾਬ_ext.x_origin - (int64_t) surf->extents.x_origin;
  int64_t my0 = (int64_t) ਨਕਾਬ_ext.y_origin - (int64_t) surf->extents.y_origin;

  int64_t x0 = hb_max ((int64_t) clip.min_x, hb_max (mx0, (int64_t) 0));
  int64_t y0 = hb_max ((int64_t) clip.min_y, hb_max (my0, (int64_t) 0));
  int64_t x1 = hb_min ((int64_t) clip.max_x, hb_min (mx0 + ਨਕਾਬ_ext.ਚੌੜਾਈ, (int64_t) w));
  int64_t y1 = hb_min ((int64_t) clip.max_y, hb_min (my0 + ਨਕਾਬ_ext.ਉਚਾਈ, (int64_t) h));

  ਜੇ (x0 >= x1 || y0 >= y1) ਵਾਪਸ ਝੂਠ;

  *ਨਕਾਬ_x0 = mx0;
  *ਨਕਾਬ_y0 = my0;
  *ix0 = (ਬਿਨਾਨਿਸ਼ਾਨੀ) x0;
  *iy0 = (ਬਿਨਾਨਿਸ਼ਾਨੀ) y0;
  *ix1 = (ਬਿਨਾਨਿਸ਼ਾਨੀ) x1;
  *iy1 = (ਬਿਨਾਨਿਸ਼ਾਨੀ) y1;
  ਵਾਪਸ ਸੱਚ;
}

/* Render whatever edges have been accumulated into @rdr and
 * push the result as a new clip on the stack, intersecting
 * with the existing clip.  Used by the emitter-based clip
 * helpers (push_clip_glyph) and by push_clip_path_end once
 * the caller has drawn the path into @rdr. */
ਸਥਿਰ ਖਾਲੀ
hb_raster_paint_finalize_ਰਸਤਾ_clip (hb_raster_paint_t *c,
				    hb_raster_draw_t *rdr,
				    hb_raster_image_t *surf,
				    ਬਿਨਾਨਿਸ਼ਾਨੀ w, ਬਿਨਾਨਿਸ਼ਾਨੀ h)
{
  ਜੇ (unlikely (!c->precharge_work (hb_raster_draw_get_pixel_work (rdr, h, w))))
  {
    hb_raster_draw_clear (rdr);
    hb_raster_paint_ਧੱਕੋ_ਖਾਲੀ_ਥਾਵਾਂ_clip (c, w, h);
    ਵਾਪਸ;
  }

  hb_raster_image_t *ਨਕਾਬ_img = hb_raster_draw_render (rdr);

  ਜੇ (unlikely (!ਨਕਾਬ_img))
  {
    hb_raster_paint_ਧੱਕੋ_ਖਾਲੀ_ਥਾਵਾਂ_clip (c, w, h);
    ਵਾਪਸ;
  }

  /* Charge the mask render (its own area; may exceed the surface) plus
   * the clip-buffer pass below. */
  hb_raster_extents_t ਨਕਾਬ_render_ext;
  hb_raster_image_get_extents (ਨਕਾਬ_img, &ਨਕਾਬ_render_ext);
  ਜੇ (unlikely (!c->charge_work ((int64_t) ਨਕਾਬ_render_ext.ਚੌੜਾਈ * ਨਕਾਬ_render_ext.ਉਚਾਈ +
				 (int64_t) w * h)))
  {
    hb_raster_draw_recycle_image (rdr, ਨਕਾਬ_img);
    hb_raster_paint_ਧੱਕੋ_ਖਾਲੀ_ਥਾਵਾਂ_clip (c, w, h);
    ਵਾਪਸ;
  }

  hb_raster_clip_t new_clip = c->acquire_clip (w, h);

  /* Allocate alpha buffer and intersect with previous clip */
  size_t clip_ਮਾਪ = (size_t) new_clip.stride * h;
  ਜੇ (unlikely (clip_ਮਾਪ > HB_RASTER_MAX_ਬਫਰ_ਮਾਪ ||
                !new_clip.alpha.resize ((ਬਿਨਾਨਿਸ਼ਾਨੀ) clip_ਮਾਪ)))
  {
    hb_raster_draw_recycle_image (rdr, ਨਕਾਬ_img);
    hb_raster_paint_ਧੱਕੋ_ਖਾਲੀ_ਥਾਵਾਂ_clip (c, w, h);
    ਵਾਪਸ;
  }
  hb_memset (new_clip.alpha.arrayZ, 0, (ਬਿਨਾਨਿਸ਼ਾਨੀ) clip_ਮਾਪ);

  ਅਚਲ uint8_t *ਨਕਾਬ_buf = hb_raster_image_get_ਬਫਰ (ਨਕਾਬ_img);
  hb_raster_extents_t ਨਕਾਬ_ext;
  hb_raster_image_get_extents (ਨਕਾਬ_img, &ਨਕਾਬ_ext);
  ਅਚਲ hb_raster_clip_t &old_clip = c->current_clip ();
  ਜੇ (unlikely (!old_clip.has_valid_alpha_ਨਕਾਬ ()))
  {
    hb_raster_draw_recycle_image (rdr, ਨਕਾਬ_img);
    hb_raster_paint_ਧੱਕੋ_ਖਾਲੀ_ਥਾਵਾਂ_clip (c, w, h);
    ਵਾਪਸ;
  }

  int64_t ਨਕਾਬ_x0, ਨਕਾਬ_y0;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ix0, iy0, ix1, iy1;
  ਜੇ (!hb_raster_paint_intersect_ਨਕਾਬ (surf, ਨਕਾਬ_ext, old_clip, w, h,
				       &ਨਕਾਬ_x0, &ਨਕਾਬ_y0,
				       &ix0, &iy0, &ix1, &iy1))
  {
    hb_raster_draw_recycle_image (rdr, ਨਕਾਬ_img);
    hb_raster_paint_ਧੱਕੋ_ਖਾਲੀ_ਥਾਵਾਂ_clip (c, w, h);
    ਵਾਪਸ;
  }

  new_clip.min_x = w; new_clip.min_y = h;
  new_clip.max_x = 0; new_clip.max_y = 0;

  ਜੇ (old_clip.is_rect)
  {
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ y = iy0; y < iy1; y++)
    {
      ਅਚਲ uint8_t *ਨਕਾਬ_ਪੰਕਤੀ = ਨਕਾਬ_buf + (ਬਿਨਾਨਿਸ਼ਾਨੀ) ((int64_t) y - ਨਕਾਬ_y0) * ਨਕਾਬ_ext.stride;
      uint8_t *ਬਾਹਰਲਾ_ਪੰਕਤੀ = new_clip.alpha.arrayZ + y * new_clip.stride;
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੰਕਤੀ_min = ix1;
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੰਕਤੀ_max = ix0;
      ਬਿਨਾਨਿਸ਼ਾਨੀ mx = (ਬਿਨਾਨਿਸ਼ਾਨੀ) ((int64_t) ix0 - ਨਕਾਬ_x0);
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ x = ix0; x < ix1; x++)
      {
	uint8_t a = ਨਕਾਬ_ਪੰਕਤੀ[mx++];
	ਬਾਹਰਲਾ_ਪੰਕਤੀ[x] = a;
	ਜੇ (a && ਪੰਕਤੀ_min == ix1)
	{
	  ਪੰਕਤੀ_min = x;
	  ਪੰਕਤੀ_max = x + 1;
	}
	ਨਹੀਂਤਾਂ ਜੇ (a)
	  ਪੰਕਤੀ_max = x + 1;
      }
      ਜੇ (ਪੰਕਤੀ_min < ਪੰਕਤੀ_max)
      {
	new_clip.min_x = hb_min (new_clip.min_x, ਪੰਕਤੀ_min);
	new_clip.min_y = hb_min (new_clip.min_y, y);
	new_clip.max_x = hb_max (new_clip.max_x, ਪੰਕਤੀ_max);
	new_clip.max_y = hb_max (new_clip.max_y, y + 1);
      }
    }
  }
  ਨਹੀਂਤਾਂ
  {
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ y = iy0; y < iy1; y++)
    {
      ਅਚਲ uint8_t *old_ਪੰਕਤੀ = old_clip.alpha.arrayZ + y * old_clip.stride;
      ਅਚਲ uint8_t *ਨਕਾਬ_ਪੰਕਤੀ = ਨਕਾਬ_buf + (ਬਿਨਾਨਿਸ਼ਾਨੀ) ((int64_t) y - ਨਕਾਬ_y0) * ਨਕਾਬ_ext.stride;
      uint8_t *ਬਾਹਰਲਾ_ਪੰਕਤੀ = new_clip.alpha.arrayZ + y * new_clip.stride;
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੰਕਤੀ_min = ix1;
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੰਕਤੀ_max = ix0;
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ x = ix0; x < ix1; x++)
      {
	ਬਿਨਾਨਿਸ਼ਾਨੀ mx = (ਬਿਨਾਨਿਸ਼ਾਨੀ) ((int64_t) x - ਨਕਾਬ_x0);
	uint8_t a = hb_raster_div255 (ਨਕਾਬ_ਪੰਕਤੀ[mx] * old_ਪੰਕਤੀ[x]);
	ਬਾਹਰਲਾ_ਪੰਕਤੀ[x] = a;
	ਜੇ (a)
	{
	  ਪੰਕਤੀ_min = hb_min (ਪੰਕਤੀ_min, x);
	  ਪੰਕਤੀ_max = x + 1;
	}
      }
      ਜੇ (ਪੰਕਤੀ_min < ਪੰਕਤੀ_max)
      {
	new_clip.min_x = hb_min (new_clip.min_x, ਪੰਕਤੀ_min);
	new_clip.min_y = hb_min (new_clip.min_y, y);
	new_clip.max_x = hb_max (new_clip.max_x, ਪੰਕਤੀ_max);
	new_clip.max_y = hb_max (new_clip.max_y, y + 1);
      }
    }
  }

  hb_raster_draw_recycle_image (rdr, ਨਕਾਬ_img);
  ਜੇ (unlikely (!c->clip_ਢੇਰ.ਧੱਕੋ_or_fail (std::move (new_clip))))
    hb_raster_paint_ਧੱਕੋ_ਖਾਲੀ_ਥਾਵਾਂ_clip (c, w, h);
}

ਸਥਿਰ ਖਾਲੀ
hb_raster_paint_ਧੱਕੋ_clip_from_emitter (hb_raster_paint_t *c,
					hb_raster_paint_clip_ਨਕਾਬ_emit_t emit,
					ਖਾਲੀ *emit_ਅੰਕੜੇ)
{
  ensure_initialized (c);

  hb_raster_image_t *surf = c->current_surface ();
  ਜੇ (unlikely (!surf)) ਵਾਪਸ;

  ਬਿਨਾਨਿਸ਼ਾਨੀ w = surf->extents.ਚੌੜਾਈ;
  ਬਿਨਾਨਿਸ਼ਾਨੀ h = surf->extents.ਉਚਾਈ;

  /* Out of budget: skip the glyph-outline extraction entirely, so
   * per-glyph outline limits cannot multiply with the caller's
   * paint-graph traversal limits. */
  ਜੇ (unlikely (c->budget_ਬਚਿਆ < 0))
  {
    hb_raster_paint_ਧੱਕੋ_ਖਾਲੀ_ਥਾਵਾਂ_clip (c, w, h);
    ਵਾਪਸ;
  }

  hb_raster_draw_t *rdr = c->clip_rdr;
  hb_transform_t<> t = c->current_effective_transform ();
  hb_raster_draw_ਸੈੱਟ_transform (rdr, t.xx, t.yx, t.xy, t.yy, t.x0, t.y0);
  hb_raster_paint_attach_clip_rdr (c, rdr, surf);
  emit (rdr, emit_ਅੰਕੜੇ);
  hb_raster_paint_readback_budget (c, rdr);

  hb_raster_paint_finalize_ਰਸਤਾ_clip (c, rdr, surf, w, h);
}

ਬਣਤਰ hb_raster_paint_glyph_clip_ਅੰਕੜੇ_t
{
  hb_codepoint_t glyph;
  hb_font_t *font;
};

ਸਥਿਰ ਖਾਲੀ
hb_raster_paint_emit_clip_glyph_ਨਕਾਬ (hb_raster_draw_t *rdr, ਖਾਲੀ *user_ਅੰਕੜੇ)
{
  hb_raster_paint_glyph_clip_ਅੰਕੜੇ_t *ਅੰਕੜੇ = (hb_raster_paint_glyph_clip_ਅੰਕੜੇ_t *) user_ਅੰਕੜੇ;
  /* Let draw-render choose tight glyph extents; we map by mask origin below. */
  hb_font_draw_glyph (ਅੰਕੜੇ->font, ਅੰਕੜੇ->glyph, hb_raster_draw_get_funcs (rdr), rdr);
}

ਸਥਿਰ ਖਾਲੀ
hb_raster_paint_ਧੱਕੋ_clip_glyph (hb_paint_funcs_t *pfuncs HB_ਅਣਵਰਤਿਆ,
				 ਖਾਲੀ *paint_ਅੰਕੜੇ,
				 hb_codepoint_t glyph,
				 hb_font_t *font,
				 ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  hb_raster_paint_t *c = (hb_raster_paint_t *) paint_ਅੰਕੜੇ;
  hb_raster_paint_glyph_clip_ਅੰਕੜੇ_t ਅੰਕੜੇ = {glyph, font};
  hb_raster_paint_ਧੱਕੋ_clip_from_emitter (c, hb_raster_paint_emit_clip_glyph_ਨਕਾਬ, &ਅੰਕੜੇ);
}

ਸਥਿਰ ਖਾਲੀ
hb_raster_paint_ਧੱਕੋ_clip_rectangle (hb_paint_funcs_t *pfuncs HB_ਅਣਵਰਤਿਆ,
				     ਖਾਲੀ *paint_ਅੰਕੜੇ,
				     ਦਸ਼ਮਲਵ xmin, ਦਸ਼ਮਲਵ ymin,
				     ਦਸ਼ਮਲਵ xmax, ਦਸ਼ਮਲਵ ymax,
				     ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  hb_raster_paint_t *c = (hb_raster_paint_t *) paint_ਅੰਕੜੇ;

  ensure_initialized (c);

  ਜੇ (!c->surface_ਢੇਰ.ਲੰਮਾਈ) ਵਾਪਸ;

  hb_transform_t<> t = c->current_effective_transform ();

  hb_raster_image_t *surf = c->current_surface ();
  ਜੇ (unlikely (!surf)) ਵਾਪਸ;

  ਬਿਨਾਨਿਸ਼ਾਨੀ w = surf->extents.ਚੌੜਾਈ;
  ਬਿਨਾਨਿਸ਼ਾਨੀ h = surf->extents.ਉਚਾਈ;
  ਤਾਰਕਿਕ is_axis_aligned = (t.xy == 0.f && t.yx == 0.f);

  /* Transform the four corners to pixel space */
  ਦਸ਼ਮਲਵ cx[4], cy[4];
  cx[0] = xmin; cy[0] = ymin;
  cx[1] = xmax; cy[1] = ymin;
  cx[2] = xmax; cy[2] = ymax;
  cx[3] = xmin; cy[3] = ymax;
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < 4; i++)
    t.transform_ਬਿੰਦੂ (cx[i], cy[i]);

  /* Compute bounding box in pixel coords */
  ਦਸ਼ਮਲਵ fmin_x = cx[0], fmin_y = cy[0], fmax_x = cx[0], fmax_y = cy[0];
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 1; i < 4; i++)
  {
    fmin_x = hb_min (fmin_x, cx[i]); fmin_y = hb_min (fmin_y, cy[i]);
    fmax_x = hb_max (fmax_x, cx[i]); fmax_y = hb_max (fmax_y, cy[i]);
  }

  /* A spec-legal but very large transform can push these bounds past the int
     range or to non-finite values; a bare float->int conversion would then be
     undefined.  Saturate through hb_clamp_to<> (which also maps NaN to
     INT32_MIN); the subtraction is done in double to avoid overflow. */
  ਪੂਰਨ px0 = hb_clamp_ਮੰਜ਼ਿਲ<int32_t> ((ਦੁੱਗਣਾ) floorf (fmin_x) - surf->extents.x_origin);
  ਪੂਰਨ py0 = hb_clamp_ਮੰਜ਼ਿਲ<int32_t> ((ਦੁੱਗਣਾ) floorf (fmin_y) - surf->extents.y_origin);
  ਪੂਰਨ px1 = hb_clamp_ਮੰਜ਼ਿਲ<int32_t> ((ਦੁੱਗਣਾ) ceilf  (fmax_x) - surf->extents.x_origin);
  ਪੂਰਨ py1 = hb_clamp_ਮੰਜ਼ਿਲ<int32_t> ((ਦੁੱਗਣਾ) ceilf  (fmax_y) - surf->extents.y_origin);

  /* Clamp to surface bounds */
  px0 = hb_max (px0, 0);
  py0 = hb_max (py0, 0);
  px1 = hb_min (px1, (ਪੂਰਨ) w);
  py1 = hb_min (py1, (ਪੂਰਨ) h);

  ਅਚਲ hb_raster_clip_t &old_clip = c->current_clip ();
  ਜੇ (unlikely (!old_clip.has_valid_alpha_ਨਕਾਬ ()))
  {
    hb_raster_paint_ਧੱਕੋ_ਖਾਲੀ_ਥਾਵਾਂ_clip (c, w, h);
    ਵਾਪਸ;
  }

  /* All paths below except rect-on-rect run a full clip-buffer pass. */
  ਜੇ (!(is_axis_aligned && old_clip.is_rect) &&
      unlikely (!c->charge_work ((int64_t) w * h)))
  {
    hb_raster_paint_ਧੱਕੋ_ਖਾਲੀ_ਥਾਵਾਂ_clip (c, w, h);
    ਵਾਪਸ;
  }

  hb_raster_clip_t new_clip = c->acquire_clip (w, h);

  ਜੇ (is_axis_aligned && old_clip.is_rect)
  {
    /* Fast path: axis-aligned rect-on-rect intersection */
    new_clip.is_rect = ਸੱਚ;
    new_clip.rect_x0 = hb_max (px0, old_clip.rect_x0);
    new_clip.rect_y0 = hb_max (py0, old_clip.rect_y0);
    new_clip.rect_x1 = hb_min (px1, old_clip.rect_x1);
    new_clip.rect_y1 = hb_min (py1, old_clip.rect_y1);
    new_clip.update_bounds_from_rect ();
  }
  ਨਹੀਂਤਾਂ ਜੇ (is_axis_aligned)
  {
    /* Fast path: intersect an axis-aligned rect with an existing alpha clip. */
    new_clip.is_rect = ਝੂਠ;
    size_t clip_ਮਾਪ = (size_t) new_clip.stride * h;
    ਜੇ (unlikely (clip_ਮਾਪ > HB_RASTER_MAX_ਬਫਰ_ਮਾਪ ||
                  !new_clip.alpha.resize ((ਬਿਨਾਨਿਸ਼ਾਨੀ) clip_ਮਾਪ)))
    {
      hb_raster_paint_ਧੱਕੋ_ਖਾਲੀ_ਥਾਵਾਂ_clip (c, w, h);
      ਵਾਪਸ;
    }
    hb_memset (new_clip.alpha.arrayZ, 0, (ਬਿਨਾਨਿਸ਼ਾਨੀ) clip_ਮਾਪ);

    ਪੂਰਨ iy0_i = hb_max (py0, (ਪੂਰਨ) old_clip.min_y);
    ਪੂਰਨ iy1_i = hb_min (py1, (ਪੂਰਨ) old_clip.max_y);
    ਪੂਰਨ ix0_i = hb_max (px0, (ਪੂਰਨ) old_clip.min_x);
    ਪੂਰਨ ix1_i = hb_min (px1, (ਪੂਰਨ) old_clip.max_x);

    ਜੇ (ix0_i < ix1_i && iy0_i < iy1_i)
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ iy0 = (ਬਿਨਾਨਿਸ਼ਾਨੀ) iy0_i;
      ਬਿਨਾਨਿਸ਼ਾਨੀ iy1 = (ਬਿਨਾਨਿਸ਼ਾਨੀ) iy1_i;
      ਬਿਨਾਨਿਸ਼ਾਨੀ ix0 = (ਬਿਨਾਨਿਸ਼ਾਨੀ) ix0_i;
      ਬਿਨਾਨਿਸ਼ਾਨੀ ix1 = (ਬਿਨਾਨਿਸ਼ਾਨੀ) ix1_i;

      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ y = iy0; y < iy1; y++)
      {
        ਅਚਲ uint8_t *old_ਪੰਕਤੀ = old_clip.alpha.arrayZ + y * old_clip.stride;
        uint8_t *new_ਪੰਕਤੀ = new_clip.alpha.arrayZ + y * new_clip.stride;
        hb_memcpy (new_ਪੰਕਤੀ + ix0, old_ਪੰਕਤੀ + ix0, ix1 - ix0);
      }
      new_clip.min_x = ix0;
      new_clip.min_y = iy0;
      new_clip.max_x = ix1;
      new_clip.max_y = iy1;
    }
  }
  ਨਹੀਂਤਾਂ
  {
    /* General case: rasterize transformed quad as alpha mask */
    new_clip.is_rect = ਝੂਠ;
    size_t clip_ਮਾਪ = (size_t) new_clip.stride * h;
    ਜੇ (unlikely (clip_ਮਾਪ > HB_RASTER_MAX_ਬਫਰ_ਮਾਪ ||
                  !new_clip.alpha.resize ((ਬਿਨਾਨਿਸ਼ਾਨੀ) clip_ਮਾਪ)))
    {
      hb_raster_paint_ਧੱਕੋ_ਖਾਲੀ_ਥਾਵਾਂ_clip (c, w, h);
      ਵਾਪਸ;
    }
    hb_memset (new_clip.alpha.arrayZ, 0, (ਬਿਨਾਨਿਸ਼ਾਨੀ) clip_ਮਾਪ);

    /* Convert quad corners to pixel-relative coords */
    ਦਸ਼ਮਲਵ qx[4], qy[4];
    ਪੂਰਨ ox = surf->extents.x_origin;
    ਪੂਰਨ oy = surf->extents.y_origin;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < 4; i++)
    {
      qx[i] = cx[i] - ox;
      qy[i] = cy[i] - oy;
    }

    /* For each pixel in the bounding box, test if inside the quad
     * using cross-product edge tests (winding order). */
    ਪੂਰਨ iy0_i = hb_max (py0, (ਪੂਰਨ) old_clip.min_y);
    ਪੂਰਨ iy1_i = hb_min (py1, (ਪੂਰਨ) old_clip.max_y);
    ਪੂਰਨ ix0_i = hb_max (px0, (ਪੂਰਨ) old_clip.min_x);
    ਪੂਰਨ ix1_i = hb_min (px1, (ਪੂਰਨ) old_clip.max_x);
    new_clip.min_x = w; new_clip.min_y = h;
    new_clip.max_x = 0; new_clip.max_y = 0;

    ਜੇ (ix0_i < ix1_i && iy0_i < iy1_i)
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ iy0 = (ਬਿਨਾਨਿਸ਼ਾਨੀ) iy0_i;
      ਬਿਨਾਨਿਸ਼ਾਨੀ iy1 = (ਬਿਨਾਨਿਸ਼ਾਨੀ) iy1_i;
      ਬਿਨਾਨਿਸ਼ਾਨੀ ix0 = (ਬਿਨਾਨਿਸ਼ਾਨੀ) ix0_i;
      ਬਿਨਾਨਿਸ਼ਾਨੀ ix1 = (ਬਿਨਾਨਿਸ਼ਾਨੀ) ix1_i;

      /* Precompute edge normals for point-in-quad test.
       * Edge i goes from corner i to corner (i+1)%4.
       * Normal = (dy, -dx); inside test: dot(normal, p-corner) >= 0 */
	      ਦਸ਼ਮਲਵ enx[4], eny[4], ed[4];
	      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < 4; i++)
	      {
		ਬਿਨਾਨਿਸ਼ਾਨੀ j = (i + 1) & 3;
		ਦਸ਼ਮਲਵ edx = qx[j] - qx[i], edy = qy[j] - qy[i];
		enx[i] = edy;       /* normal x */
		eny[i] = -edx;      /* normal y */
		ed[i] = enx[i] * qx[i] + eny[i] * qy[i]; /* distance threshold */
	      }
	      ਦਸ਼ਮਲਵ area2 = 0.f;
	      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < 4; i++)
	      {
		ਬਿਨਾਨਿਸ਼ਾਨੀ j = (i + 1) & 3;
		area2 += qx[i] * qy[j] - qx[j] * qy[i];
	      }
	      ਤਾਰਕਿਕ ccw = area2 >= 0.f;

      ਜੇ (old_clip.is_rect)
      {
	ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ y = iy0; y < iy1; y++)
	  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ x = ix0; x < ix1; x++)
	  {
	    ਦਸ਼ਮਲਵ px_f = x + 0.5f, py_f = y + 0.5f;
	    /* Test if pixel center is inside the quad */
		    ਤਾਰਕਿਕ inside = ਸੱਚ;
		    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < 4; i++)
		    {
		      ਦਸ਼ਮਲਵ d = enx[i] * px_f + eny[i] * py_f;
		      ਜੇ (ccw ? d < ed[i] : d > ed[i])
		      {
			inside = ਝੂਠ;
			ਰੋਕੋ;
		      }
		    }
	    uint8_t a = inside ? 255 : 0;
	    new_clip.alpha[y * new_clip.stride + x] = a;
	    ਜੇ (a)
	    {
	      new_clip.min_x = hb_min (new_clip.min_x, x);
	      new_clip.min_y = hb_min (new_clip.min_y, y);
	      new_clip.max_x = hb_max (new_clip.max_x, x + 1);
	      new_clip.max_y = hb_max (new_clip.max_y, y + 1);
	    }
	  }
      }
      ਨਹੀਂਤਾਂ
      {
	ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ y = iy0; y < iy1; y++)
	{
	  ਅਚਲ uint8_t *old_ਪੰਕਤੀ = old_clip.alpha.arrayZ + y * old_clip.stride;
	  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ x = ix0; x < ix1; x++)
	  {
	    ਦਸ਼ਮਲਵ px_f = x + 0.5f, py_f = y + 0.5f;
	    /* Test if pixel center is inside the quad */
		    ਤਾਰਕਿਕ inside = ਸੱਚ;
		    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < 4; i++)
		    {
		      ਦਸ਼ਮਲਵ d = enx[i] * px_f + eny[i] * py_f;
		      ਜੇ (ccw ? d < ed[i] : d > ed[i])
		      {
			inside = ਝੂਠ;
			ਰੋਕੋ;
		      }
		    }
	    uint8_t a = inside ? old_ਪੰਕਤੀ[x] : 0;
	    new_clip.alpha[y * new_clip.stride + x] = a;
	    ਜੇ (a)
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

  ਜੇ (unlikely (!c->clip_ਢੇਰ.ਧੱਕੋ_or_fail (std::move (new_clip))))
    hb_raster_paint_ਧੱਕੋ_ਖਾਲੀ_ਥਾਵਾਂ_clip (c, surf->extents.ਚੌੜਾਈ, surf->extents.ਉਚਾਈ);
}

ਸਥਿਰ hb_draw_funcs_t *
hb_raster_paint_ਧੱਕੋ_clip_ਰਸਤਾ_ਸ਼ੁਰੂ (hb_paint_funcs_t *pfuncs HB_ਅਣਵਰਤਿਆ,
				      ਖਾਲੀ *paint_ਅੰਕੜੇ,
				      ਖਾਲੀ **draw_ਅੰਕੜੇ,
				      ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  hb_raster_paint_t *c = (hb_raster_paint_t *) paint_ਅੰਕੜੇ;

  ensure_initialized (c);

  /* Prime clip_rdr with the current effective transform; the
   * caller then drives hb_draw_*() into it, and _end renders
   * the accumulated edges into a mask and intersects with the
   * current clip. */
  ਜੇ (unlikely (!c->surface_ਢੇਰ.ਲੰਮਾਈ || !c->clip_rdr))
  {
    *draw_ਅੰਕੜੇ = ਖਾਲੀ_ਸੂਚਕ;
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
  }

  hb_raster_draw_t *rdr = c->clip_rdr;
  hb_transform_t<> t = c->current_effective_transform ();
  hb_raster_draw_ਸੈੱਟ_transform (rdr, t.xx, t.yx, t.xy, t.yy, t.x0, t.y0);
  hb_raster_image_t *surf = c->current_surface ();
  ਜੇ (likely (surf))
    hb_raster_paint_attach_clip_rdr (c, rdr, surf);

  *draw_ਅੰਕੜੇ = rdr;
  ਵਾਪਸ hb_raster_draw_get_funcs (rdr);
}

ਸਥਿਰ ਖਾਲੀ
hb_raster_paint_ਧੱਕੋ_clip_ਰਸਤਾ_ਸਿਰਾ (hb_paint_funcs_t *pfuncs HB_ਅਣਵਰਤਿਆ,
				    ਖਾਲੀ *paint_ਅੰਕੜੇ,
				    ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  hb_raster_paint_t *c = (hb_raster_paint_t *) paint_ਅੰਕੜੇ;

  hb_raster_image_t *surf = c->current_surface ();
  ਜੇ (unlikely (!surf || !c->clip_rdr)) ਵਾਪਸ;

  ਬਿਨਾਨਿਸ਼ਾਨੀ w = surf->extents.ਚੌੜਾਈ;
  ਬਿਨਾਨਿਸ਼ਾਨੀ h = surf->extents.ਉਚਾਈ;

  hb_raster_paint_readback_budget (c, c->clip_rdr);
  ਜੇ (unlikely (c->budget_ਬਚਿਆ < 0))
  {
    hb_raster_draw_clear (c->clip_rdr);
    hb_raster_paint_ਧੱਕੋ_ਖਾਲੀ_ਥਾਵਾਂ_clip (c, w, h);
    ਵਾਪਸ;
  }

  hb_raster_paint_finalize_ਰਸਤਾ_clip (c, c->clip_rdr, surf, w, h);
}

ਸਥਿਰ ਖਾਲੀ
hb_raster_paint_ਕੱਢੋ_clip (hb_paint_funcs_t *pfuncs HB_ਅਣਵਰਤਿਆ,
			  ਖਾਲੀ *paint_ਅੰਕੜੇ,
			  ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  hb_raster_paint_t *c = (hb_raster_paint_t *) paint_ਅੰਕੜੇ;
  ਜੇ (!c->clip_ਢੇਰ.ਲੰਮਾਈ) ਵਾਪਸ;
  c->release_clip (c->clip_ਢੇਰ.ਕੱਢੋ ());
}

ਸਥਿਰ ਖਾਲੀ
hb_raster_paint_ਧੱਕੋ_ਸਮੂਹ (hb_paint_funcs_t *pfuncs HB_ਅਣਵਰਤਿਆ,
			    ਖਾਲੀ *paint_ਅੰਕੜੇ,
			    ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  hb_raster_paint_t *c = (hb_raster_paint_t *) paint_ਅੰਕੜੇ;

  ensure_initialized (c);

  /* acquire_surface() clears a full surface; charge its area. */
  ਜੇ (unlikely (!c->charge_work ((int64_t) c->fixed_extents.ਚੌੜਾਈ * c->fixed_extents.ਉਚਾਈ)))
    ਵਾਪਸ;

  hb_raster_image_t *new_surf = c->acquire_surface ();
  ਜੇ (unlikely (!new_surf)) ਵਾਪਸ;
  ਜੇ (unlikely (!c->surface_ਢੇਰ.ਧੱਕੋ_or_fail (new_surf)))
    c->release_surface (new_surf);
}

ਸਥਿਰ ਖਾਲੀ
hb_raster_paint_ਕੱਢੋ_ਸਮੂਹ (hb_paint_funcs_t *pfuncs HB_ਅਣਵਰਤਿਆ,
			   ਖਾਲੀ *paint_ਅੰਕੜੇ,
			   hb_paint_ਸੰਯੁਕਤ_ਬਹੁਲਕ_t ਬਹੁਲਕ,
			   ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  hb_raster_paint_t *c = (hb_raster_paint_t *) paint_ਅੰਕੜੇ;

  ਜੇ (c->surface_ਢੇਰ.ਲੰਮਾਈ < 2) ਵਾਪਸ;

  hb_raster_image_t *src = c->surface_ਢੇਰ.ਕੱਢੋ ();
  hb_raster_image_t *dst = c->current_surface ();

  ਜੇ (dst && src &&
      likely (c->charge_work ((int64_t) dst->extents.ਚੌੜਾਈ * dst->extents.ਉਚਾਈ)))
    hb_raster_image_ਸੰਯੁਕਤ (dst, src, ਬਹੁਲਕ);

  c->release_surface (src);
}

ਸਥਿਰ ਖਾਲੀ
hb_raster_paint_solid (hb_raster_paint_t *c,
		       hb_raster_image_t *surf,
		       hb_color_t color,
		       hb_raster_image_t *ਨਕਾਬ_img)
{
  uint32_t premul = color_ਮੰਜ਼ਿਲ_premul_pixel (color);
  uint8_t premul_a = (uint8_t) (premul >> 24);
  ਅਚਲ hb_raster_clip_t &clip = c->current_clip ();

  ਬਿਨਾਨਿਸ਼ਾਨੀ stride = surf->extents.stride;
  ਜੇ (clip.min_x >= clip.max_x || clip.min_y >= clip.max_y) ਵਾਪਸ;
  ਜੇ (premul_a == 0) ਵਾਪਸ;

  ਬਿਨਾਨਿਸ਼ਾਨੀ ix0 = clip.min_x, iy0 = clip.min_y;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ix1 = clip.max_x, iy1 = clip.max_y;
  ਅਚਲ uint8_t *ਨਕਾਬ_buf = ਖਾਲੀ_ਸੂਚਕ;
  hb_raster_extents_t ਨਕਾਬ_ext;
  int64_t ਨਕਾਬ_x0 = 0, ਨਕਾਬ_y0 = 0;
  ਜੇ (ਨਕਾਬ_img)
  {
    ਨਕਾਬ_buf = hb_raster_image_get_ਬਫਰ (ਨਕਾਬ_img);
    hb_raster_image_get_extents (ਨਕਾਬ_img, &ਨਕਾਬ_ext);
    ਜੇ (!hb_raster_paint_intersect_ਨਕਾਬ (surf, ਨਕਾਬ_ext, clip,
					 surf->extents.ਚੌੜਾਈ, surf->extents.ਉਚਾਈ,
					 &ਨਕਾਬ_x0, &ਨਕਾਬ_y0,
					 &ix0, &iy0, &ix1, &iy1))
      ਵਾਪਸ;
  }

  ਜੇ (unlikely (!c->charge_work ((int64_t) (ix1 - ix0) * (iy1 - iy0))))
    ਵਾਪਸ;

  ਜੇ (likely (!clip.is_rect))
  {
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ y = iy0; y < iy1; y++)
    {
      hb_packed_t<uint32_t> *__restrict ਪੰਕਤੀ = (hb_packed_t<uint32_t> *) (surf->ਬਫਰ.arrayZ + y * stride);
      ਅਚਲ uint8_t *__restrict clip_ਪੰਕਤੀ = clip.alpha.arrayZ + y * clip.stride;
      ਅਚਲ uint8_t *__restrict ਨਕਾਬ_ਪੰਕਤੀ = ਨਕਾਬ_buf ? ਨਕਾਬ_buf + (ਬਿਨਾਨਿਸ਼ਾਨੀ) ((int64_t) y - ਨਕਾਬ_y0) * ਨਕਾਬ_ext.stride
						    : ਖਾਲੀ_ਸੂਚਕ;
      ਬਿਨਾਨਿਸ਼ਾਨੀ mx = (ਬਿਨਾਨਿਸ਼ਾਨੀ) ((int64_t) ix0 - ਨਕਾਬ_x0);
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ x = ix0; x < ix1; x++)
      {
	uint8_t clip_alpha = clip_ਪੰਕਤੀ[x];
	ਜੇ (ਨਕਾਬ_ਪੰਕਤੀ)
	  clip_alpha = hb_raster_div255 (ਨਕਾਬ_ਪੰਕਤੀ[mx++] * clip_alpha);
	ਜੇ (clip_alpha == 0) ਜਾਰੀ;
	ਜੇ (clip_alpha == 255)
	{
	  ਜੇ (premul_a == 255)
	    ਪੰਕਤੀ[x] = hb_packed_t<uint32_t> (premul);
	  ਨਹੀਂਤਾਂ
	    ਪੰਕਤੀ[x] = hb_packed_t<uint32_t> (hb_raster_src_over (premul, (uint32_t) ਪੰਕਤੀ[x]));
	}
	ਨਹੀਂਤਾਂ
	{
	  uint32_t src = hb_raster_alpha_mul (premul, clip_alpha);
	  ਪੰਕਤੀ[x] = hb_packed_t<uint32_t> (hb_raster_src_over (src, (uint32_t) ਪੰਕਤੀ[x]));
	}
      }
    }
  }
  ਨਹੀਂਤਾਂ ਜੇ (ਨਕਾਬ_buf)
  {
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ y = iy0; y < iy1; y++)
    {
      hb_packed_t<uint32_t> *__restrict ਪੰਕਤੀ = (hb_packed_t<uint32_t> *) (surf->ਬਫਰ.arrayZ + y * stride);
      ਅਚਲ uint8_t *__restrict ਨਕਾਬ_ਪੰਕਤੀ = ਨਕਾਬ_buf + (ਬਿਨਾਨਿਸ਼ਾਨੀ) ((int64_t) y - ਨਕਾਬ_y0) * ਨਕਾਬ_ext.stride;
      ਬਿਨਾਨਿਸ਼ਾਨੀ mx = (ਬਿਨਾਨਿਸ਼ਾਨੀ) ((int64_t) ix0 - ਨਕਾਬ_x0);
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ x = ix0; x < ix1; x++)
      {
	uint8_t ਨਕਾਬ_alpha = ਨਕਾਬ_ਪੰਕਤੀ[mx++];
	ਜੇ (ਨਕਾਬ_alpha == 0) ਜਾਰੀ;
	uint32_t src = hb_raster_alpha_mul (premul, ਨਕਾਬ_alpha);
	ਪੰਕਤੀ[x] = hb_packed_t<uint32_t> (hb_raster_src_over (src, (uint32_t) ਪੰਕਤੀ[x]));
      }
    }
  }
  ਨਹੀਂਤਾਂ
  {
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ y = clip.min_y; y < clip.max_y; y++)
    {
      hb_packed_t<uint32_t> *ਪੰਕਤੀ = (hb_packed_t<uint32_t> *) (surf->ਬਫਰ.arrayZ + y * stride);
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ x = clip.min_x; x < clip.max_x; x++)
	ਪੰਕਤੀ[x] = hb_packed_t<uint32_t> (hb_raster_src_over (premul, (uint32_t) ਪੰਕਤੀ[x]));
    }
  }
}

ਸਥਿਰ ਖਾਲੀ
hb_raster_paint_color (hb_paint_funcs_t *pfuncs HB_ਅਣਵਰਤਿਆ,
		       ਖਾਲੀ *paint_ਅੰਕੜੇ,
		       hb_bool_t is_foreground HB_ਅਣਵਰਤਿਆ,
		       hb_color_t color,
		       ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  hb_raster_paint_t *c = (hb_raster_paint_t *) paint_ਅੰਕੜੇ;

  ensure_initialized (c);

  hb_raster_image_t *surf = c->current_surface ();
  ਜੇ (unlikely (!surf)) ਵਾਪਸ;

  hb_raster_paint_solid (c, surf, color, ਖਾਲੀ_ਸੂਚਕ);
}

ਸਥਿਰ ਖਾਲੀ
hb_raster_paint_fill_glyph (hb_paint_funcs_t *pfuncs HB_ਅਣਵਰਤਿਆ,
			    ਖਾਲੀ *paint_ਅੰਕੜੇ,
			    hb_codepoint_t glyph,
			    hb_font_t *font,
			    hb_bool_t is_foreground HB_ਅਣਵਰਤਿਆ,
			    hb_color_t color,
			    ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  hb_raster_paint_t *c = (hb_raster_paint_t *) paint_ਅੰਕੜੇ;

  ensure_initialized (c);

  hb_raster_image_t *surf = c->current_surface ();
  ਜੇ (unlikely (!surf)) ਵਾਪਸ;

  /* Out of budget: skip the glyph-outline extraction entirely. */
  ਜੇ (unlikely (c->budget_ਬਚਿਆ < 0)) ਵਾਪਸ;

  hb_raster_draw_t *rdr = c->clip_rdr;
  hb_transform_t<> t = c->current_effective_transform ();
  hb_raster_draw_ਸੈੱਟ_transform (rdr, t.xx, t.yx, t.xy, t.yy, t.x0, t.y0);
  hb_raster_paint_attach_clip_rdr (c, rdr, surf);

  hb_raster_paint_glyph_clip_ਅੰਕੜੇ_t ਅੰਕੜੇ = {glyph, font};
  hb_raster_paint_emit_clip_glyph_ਨਕਾਬ (rdr, &ਅੰਕੜੇ);
  hb_raster_paint_readback_budget (c, rdr);

  ਜੇ (unlikely (!c->precharge_work (hb_raster_draw_get_pixel_work (rdr,
							  surf->extents.ਉਚਾਈ,
							  surf->extents.ਚੌੜਾਈ))))
  {
    hb_raster_draw_clear (rdr);
    ਵਾਪਸ;
  }

  hb_raster_image_t *ਨਕਾਬ_img = hb_raster_draw_render (rdr);
  ਜੇ (unlikely (!ਨਕਾਬ_img)) ਵਾਪਸ;

  hb_raster_extents_t ਨਕਾਬ_ext;
  hb_raster_image_get_extents (ਨਕਾਬ_img, &ਨਕਾਬ_ext);
  ਜੇ (likely (c->charge_work ((int64_t) ਨਕਾਬ_ext.ਚੌੜਾਈ * ਨਕਾਬ_ext.ਉਚਾਈ)))
    hb_raster_paint_solid (c, surf, color, ਨਕਾਬ_img);

  hb_raster_draw_recycle_image (rdr, ਨਕਾਬ_img);
}

ਸਥਿਰ hb_bool_t
hb_raster_paint_image (hb_paint_funcs_t *pfuncs HB_ਅਣਵਰਤਿਆ,
		       ਖਾਲੀ *paint_ਅੰਕੜੇ,
		       hb_blob_t *blob,
		       ਬਿਨਾਨਿਸ਼ਾਨੀ ਚੌੜਾਈ,
		       ਬਿਨਾਨਿਸ਼ਾਨੀ ਉਚਾਈ,
		       hb_ਟੈਗ_t format,
		       ਦਸ਼ਮਲਵ slant HB_ਅਣਵਰਤਿਆ,
		       hb_glyph_extents_t *extents,
		       ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  hb_raster_paint_t *c = (hb_raster_paint_t *) paint_ਅੰਕੜੇ;

  ensure_initialized (c);

  /* Out of pixel budget: skip, including the image decode below.  Image
   * compositing is pixel work and does no outline traversal. */
  ਜੇ (unlikely (c->pixel_ਬਚਿਆ < 0)) ਵਾਪਸ ਝੂਠ;

  ਬਿਨਾਨਿਸ਼ਾਨੀ src_ਚੌੜਾਈ = ਚੌੜਾਈ;
  ਬਿਨਾਨਿਸ਼ਾਨੀ src_ਉਚਾਈ = ਉਚਾਈ;
  ਅਚਲ hb_packed_t<uint32_t> *src_ਅੰਕੜੇ = ਖਾਲੀ_ਸੂਚਕ;
  hb_raster_image_t decoded_png;

  ਜੇ (format == HB_PAINT_IMAGE_FORMAT_BGRA)
  {
    ਜੇ (src_ਚੌੜਾਈ == 0 || src_ਉਚਾਈ == 0)
      ਵਾਪਸ ਝੂਠ;
    ਜੇ (src_ਚੌੜਾਈ > (ਬਿਨਾਨਿਸ਼ਾਨੀ) ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ || src_ਉਚਾਈ > (ਬਿਨਾਨਿਸ਼ਾਨੀ) ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ)
      ਵਾਪਸ ਝੂਠ;

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕੜੇ_len;
    ਅਚਲ uint8_t *ਅੰਕੜੇ = (ਅਚਲ uint8_t *) hb_blob_get_ਅੰਕੜੇ (blob, &ਅੰਕੜੇ_len);
    size_t pixel_ਗਿਣਤੀ = (size_t) src_ਚੌੜਾਈ * (size_t) src_ਉਚਾਈ;
    ਜੇ (src_ਚੌੜਾਈ && pixel_ਗਿਣਤੀ / src_ਚੌੜਾਈ != src_ਉਚਾਈ)
      ਵਾਪਸ ਝੂਠ;
    ਜੇ (pixel_ਗਿਣਤੀ > (size_t) -1 / 4u)
      ਵਾਪਸ ਝੂਠ;
    size_t required_ਮਾਪ = pixel_ਗਿਣਤੀ * 4u;
    ਜੇ (!ਅੰਕੜੇ || (size_t) ਅੰਕੜੇ_len < required_ਮਾਪ)
      ਵਾਪਸ ਝੂਠ;

    src_ਅੰਕੜੇ = (ਅਚਲ hb_packed_t<uint32_t> *) ਅੰਕੜੇ;
  }
  ਨਹੀਂਤਾਂ ਜੇ (format == HB_PAINT_IMAGE_FORMAT_PNG)
  {
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_PNG
    ਜੇ (!decoded_png.deserialize_from_png (blob))
      ਵਾਪਸ ਝੂਠ;
    src_ਚੌੜਾਈ = decoded_png.extents.ਚੌੜਾਈ;
    src_ਉਚਾਈ = decoded_png.extents.ਉਚਾਈ;
    src_ਅੰਕੜੇ = (ਅਚਲ hb_packed_t<uint32_t> *) decoded_png.ਬਫਰ.arrayZ;
#ਨਹੀਂਤਾਂ
    ਵਾਪਸ ਝੂਠ;
#ਜੇ_ਅੰਤ
  }
  ਨਹੀਂਤਾਂ
    ਵਾਪਸ ਝੂਠ;

  hb_raster_image_t *surf = c->current_surface ();
  ਜੇ (unlikely (!surf)) ਵਾਪਸ ਝੂਠ;
  ਜੇ (!extents) ਵਾਪਸ ਝੂਠ;

  ਅਚਲ hb_raster_clip_t &clip = c->current_clip ();
  hb_transform_t<> t = c->current_effective_transform ();

  /* Compute inverse transform for sampling */
  ਦਸ਼ਮਲਵ det = t.xx * t.yy - t.xy * t.yx;
  ਜੇ (fabsf (det) < 1e-10f) ਵਾਪਸ ਝੂਠ;
  ਦਸ਼ਮਲਵ inv_det = 1.f / det;
  ਦਸ਼ਮਲਵ inv_xx =  t.yy * inv_det;
  ਦਸ਼ਮਲਵ inv_xy = -t.xy * inv_det;
  ਦਸ਼ਮਲਵ inv_yx = -t.yx * inv_det;
  ਦਸ਼ਮਲਵ inv_yy =  t.xx * inv_det;
  ਦਸ਼ਮਲਵ inv_x0 = (t.xy * t.y0 - t.yy * t.x0) * inv_det;
  ਦਸ਼ਮਲਵ inv_y0 = (t.yx * t.x0 - t.xx * t.y0) * inv_det;

  ਬਿਨਾਨਿਸ਼ਾਨੀ surf_stride = surf->extents.stride;
  ਪੂਰਨ ox = surf->extents.x_origin;
  ਪੂਰਨ oy = surf->extents.y_origin;

  /* Image source rectangle in glyph space */
  ਦਸ਼ਮਲਵ img_x = extents->x_bearing;
  ਦਸ਼ਮਲਵ img_y = extents->y_bearing;
  ਦਸ਼ਮਲਵ img_sx = (ਦਸ਼ਮਲਵ) extents->ਚੌੜਾਈ / src_ਚੌੜਾਈ;
  ਦਸ਼ਮਲਵ img_sy = (ਦਸ਼ਮਲਵ) extents->ਉਚਾਈ / src_ਉਚਾਈ;
  ਜੇ (fabsf (img_sx) < 1e-10f || fabsf (img_sy) < 1e-10f)
    ਵਾਪਸ ਝੂਠ;

  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ clip_w = clip.max_x > clip.min_x ? clip.max_x - clip.min_x : 0;
    ਬਿਨਾਨਿਸ਼ਾਨੀ clip_h = clip.max_y > clip.min_y ? clip.max_y - clip.min_y : 0;
    ਜੇ (unlikely (!c->charge_work ((int64_t) clip_w * clip_h +
				   (int64_t) src_ਚੌੜਾਈ * src_ਉਚਾਈ)))
      ਵਾਪਸ ਝੂਠ;
  }

  ਜੇ (clip.is_rect)
  {
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ py = clip.min_y; py < clip.max_y; py++)
    {
      hb_packed_t<uint32_t> *ਪੰਕਤੀ = (hb_packed_t<uint32_t> *) (surf->ਬਫਰ.arrayZ + py * surf_stride);
      ਦਸ਼ਮਲਵ gx = inv_xx * (ਦਸ਼ਮਲਵ) ((ਪੂਰਨ) clip.min_x + ox) + inv_xy * (ਦਸ਼ਮਲਵ) ((ਪੂਰਨ) py + oy) + inv_x0;
      ਦਸ਼ਮਲਵ gy = inv_yx * (ਦਸ਼ਮਲਵ) ((ਪੂਰਨ) clip.min_x + ox) + inv_yy * (ਦਸ਼ਮਲਵ) ((ਪੂਰਨ) py + oy) + inv_y0;
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ px = clip.min_x; px < clip.max_x; px++)
      {
	/* Map glyph space to image texel; bilinear reconstruction. */
	ਦਸ਼ਮਲਵ ix = (gx - img_x) / img_sx;
	ਦਸ਼ਮਲਵ iy = (ਦਸ਼ਮਲਵ) (src_ਉਚਾਈ - 1) - (gy - img_y) / img_sy;

	ਜੇ (unlikely (!std::isfinite (ix) || !std::isfinite (iy)) ||
	    ix < 0.f || iy < 0.f ||
	    ix > (ਦਸ਼ਮਲਵ) (src_ਚੌੜਾਈ - 1) || iy > (ਦਸ਼ਮਲਵ) (src_ਉਚਾਈ - 1))
	{
	  gx += inv_xx;
	  gy += inv_yx;
	  ਜਾਰੀ;
	}

	uint32_t src_px = hb_raster_sample_bilinear_premul (src_ਅੰਕੜੇ, src_ਚੌੜਾਈ, src_ਉਚਾਈ,
							     ix, iy);
	ਪੰਕਤੀ[px] = hb_packed_t<uint32_t> (hb_raster_src_over (src_px, (uint32_t) ਪੰਕਤੀ[px]));
	gx += inv_xx;
	gy += inv_yx;
      }
    }
  }
  ਨਹੀਂਤਾਂ
  {
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ py = clip.min_y; py < clip.max_y; py++)
    {
      hb_packed_t<uint32_t> *ਪੰਕਤੀ = (hb_packed_t<uint32_t> *) (surf->ਬਫਰ.arrayZ + py * surf_stride);
      ਅਚਲ uint8_t *clip_ਪੰਕਤੀ = clip.alpha.arrayZ + py * clip.stride;
      ਦਸ਼ਮਲਵ gx = inv_xx * (ਦਸ਼ਮਲਵ) ((ਪੂਰਨ) clip.min_x + ox) + inv_xy * (ਦਸ਼ਮਲਵ) ((ਪੂਰਨ) py + oy) + inv_x0;
      ਦਸ਼ਮਲਵ gy = inv_yx * (ਦਸ਼ਮਲਵ) ((ਪੂਰਨ) clip.min_x + ox) + inv_yy * (ਦਸ਼ਮਲਵ) ((ਪੂਰਨ) py + oy) + inv_y0;
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ px = clip.min_x; px < clip.max_x; px++)
      {
	uint8_t clip_alpha = clip_ਪੰਕਤੀ[px];
	ਜੇ (clip_alpha == 0)
	{
	  gx += inv_xx;
	  gy += inv_yx;
	  ਜਾਰੀ;
	}

	/* Map glyph space to image texel; bilinear reconstruction. */
	ਦਸ਼ਮਲਵ ix = (gx - img_x) / img_sx;
	ਦਸ਼ਮਲਵ iy = (ਦਸ਼ਮਲਵ) (src_ਉਚਾਈ - 1) - (gy - img_y) / img_sy;

	ਜੇ (unlikely (!std::isfinite (ix) || !std::isfinite (iy)) ||
	    ix < 0.f || iy < 0.f ||
	    ix > (ਦਸ਼ਮਲਵ) (src_ਚੌੜਾਈ - 1) || iy > (ਦਸ਼ਮਲਵ) (src_ਉਚਾਈ - 1))
	{
	  gx += inv_xx;
	  gy += inv_yx;
	  ਜਾਰੀ;
	}

	uint32_t src_px = hb_raster_sample_bilinear_premul (src_ਅੰਕੜੇ, src_ਚੌੜਾਈ, src_ਉਚਾਈ,
							     ix, iy);
	src_px = hb_raster_alpha_mul (src_px, clip_alpha);
	ਪੰਕਤੀ[px] = hb_packed_t<uint32_t> (hb_raster_src_over (src_px, (uint32_t) ਪੰਕਤੀ[px]));
	gx += inv_xx;
	gy += inv_yx;
      }
    }
  }

  ਵਾਪਸ ਸੱਚ;
}


/*
 * Gradient helpers
 */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ GRADIENT_LUT_ਮਾਪ 256
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ GRADIENT_LUT_MIN_PIXELS (64u * 64u)

ਸਥਿਰ HB_ALWAYS_INLINE ਦਸ਼ਮਲਵ
reflect_gradient_t (ਦਸ਼ਮਲਵ t)
{
  t = fmodf (fabsf (t), 2.f);
  ਵਾਪਸ t > 1.f ? 2.f - t : t;
}

/* Evaluate color at normalized position t, interpolating in premultiplied space. */
ਸਥਿਰ uint32_t
evaluate_color_ਸਤਰ (ਅਚਲ hb_color_stop_t *stops, ਬਿਨਾਨਿਸ਼ਾਨੀ len, ਦਸ਼ਮਲਵ t,
		     hb_paint_extend_t extend)
{
  ਜੇ (unlikely (!len))
    ਵਾਪਸ 0;
  ਜੇ (unlikely (len == 1))
    ਵਾਪਸ color_ਮੰਜ਼ਿਲ_premul_pixel (stops[0].color);
  ਜੇ (unlikely (!std::isfinite (t)))
    t = 0.f;

  /* Apply extend mode */
  ਜੇ (extend == HB_PAINT_EXTEND_PAD)
  {
    t = hb_clamp (t, 0.f, 1.f);
  }
  ਨਹੀਂਤਾਂ ਜੇ (extend == HB_PAINT_EXTEND_REPEAT)
  {
    t = t - floorf (t);
  }
  ਨਹੀਂਤਾਂ /* REFLECT */
  {
    t = reflect_gradient_t (t);
  }

  /* Find bounding stops */
  ਜੇ (t <= stops[0].ਵਿੱਥ)
    ਵਾਪਸ color_ਮੰਜ਼ਿਲ_premul_pixel (stops[0].color);
  ਜੇ (t >= stops[len - 1].ਵਿੱਥ)
    ਵਾਪਸ color_ਮੰਜ਼ਿਲ_premul_pixel (stops[len - 1].color);

  ਬਿਨਾਨਿਸ਼ਾਨੀ i;
  ਲਈ (i = 0; i < len - 1; i++)
    ਜੇ (t < stops[i + 1].ਵਿੱਥ)
      ਰੋਕੋ;

  ਦਸ਼ਮਲਵ range = stops[i + 1].ਵਿੱਥ - stops[i].ਵਿੱਥ;
  ਦਸ਼ਮਲਵ k = range > 0.f ? (t - stops[i].ਵਿੱਥ) / range : 0.f;

  /* Interpolate in premultiplied [0,255] space */
  hb_color_t c0 = stops[i].color;
  hb_color_t c1 = stops[i + 1].color;

  ਦਸ਼ਮਲਵ a0 = hb_color_get_alpha (c0) / 255.f;
  ਦਸ਼ਮਲਵ r0 = hb_color_get_red   (c0) / 255.f * a0;
  ਦਸ਼ਮਲਵ g0 = hb_color_get_green (c0) / 255.f * a0;
  ਦਸ਼ਮਲਵ b0 = hb_color_get_blue  (c0) / 255.f * a0;

  ਦਸ਼ਮਲਵ a1 = hb_color_get_alpha (c1) / 255.f;
  ਦਸ਼ਮਲਵ r1 = hb_color_get_red   (c1) / 255.f * a1;
  ਦਸ਼ਮਲਵ g1 = hb_color_get_green (c1) / 255.f * a1;
  ਦਸ਼ਮਲਵ b1 = hb_color_get_blue  (c1) / 255.f * a1;

  ਦਸ਼ਮਲਵ a = a0 + k * (a1 - a0);
  ਦਸ਼ਮਲਵ r = r0 + k * (r1 - r0);
  ਦਸ਼ਮਲਵ g = g0 + k * (g1 - g0);
  ਦਸ਼ਮਲਵ b = b0 + k * (b1 - b0);

  uint8_t pa = (uint8_t) (a * 255.f + 0.5f);
  uint8_t pr = (uint8_t) (r * 255.f + 0.5f);
  uint8_t pg = (uint8_t) (g * 255.f + 0.5f);
  uint8_t pb = (uint8_t) (b * 255.f + 0.5f);

  ਵਾਪਸ (uint32_t) pb | ((uint32_t) pg << 8) | ((uint32_t) pr << 16) | ((uint32_t) pa << 24);
}

ਸਥਿਰ HB_ALWAYS_INLINE ਦਸ਼ਮਲਵ
normalize_gradient_t (ਦਸ਼ਮਲਵ t, hb_paint_extend_t extend)
{
  ਜੇ (unlikely (!std::isfinite (t)))
    ਵਾਪਸ 0.f;

  ਜੇ (extend == HB_PAINT_EXTEND_PAD)
    ਵਾਪਸ hb_clamp (t, 0.f, 1.f);
  ਜੇ (extend == HB_PAINT_EXTEND_REPEAT)
  {
    t = t - floorf (t);
    ਵਾਪਸ t < 0.f ? t + 1.f : t;
  }

  /* REFLECT */
  ਵਾਪਸ reflect_gradient_t (t);
}

ਸਥਿਰ ਖਾਲੀ
build_gradient_lut (ਅਚਲ hb_color_stop_t *stops,
		    ਬਿਨਾਨਿਸ਼ਾਨੀ len,
		    uint32_t *lut)
{
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < GRADIENT_LUT_ਮਾਪ; i++)
  {
    ਦਸ਼ਮਲਵ t = (ਦਸ਼ਮਲਵ) i / (GRADIENT_LUT_ਮਾਪ - 1);
    lut[i] = evaluate_color_ਸਤਰ (stops, len, t, HB_PAINT_EXTEND_PAD);
  }
}

ਸਥਿਰ HB_ALWAYS_INLINE uint32_t
lookup_gradient_lut (ਅਚਲ uint32_t *lut,
		     ਦਸ਼ਮਲਵ t,
		     hb_paint_extend_t extend)
{
  ਦਸ਼ਮਲਵ u = normalize_gradient_t (t, extend);
  ਜੇ (unlikely (!std::isfinite (u)))
    u = 0.f;
  ਨਹੀਂਤਾਂ
    u = hb_clamp (u, 0.f, 1.f);
  ਬਿਨਾਨਿਸ਼ਾਨੀ idx = (ਬਿਨਾਨਿਸ਼ਾਨੀ) (u * (GRADIENT_LUT_ਮਾਪ - 1) + 0.5f);
  ਵਾਪਸ lut[idx];
}

/*
 * Gradient paint callbacks
 */

ਸਥਿਰ ਖਾਲੀ
hb_raster_paint_ਰੇਖਿਕ_gradient (hb_paint_funcs_t *pfuncs HB_ਅਣਵਰਤਿਆ,
				 ਖਾਲੀ *paint_ਅੰਕੜੇ,
				 hb_color_ਸਤਰ_t *color_ਸਤਰ,
				 ਦਸ਼ਮਲਵ x0, ਦਸ਼ਮਲਵ y0,
				 ਦਸ਼ਮਲਵ x1, ਦਸ਼ਮਲਵ y1,
				 ਦਸ਼ਮਲਵ x2, ਦਸ਼ਮਲਵ y2,
				 ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  hb_raster_paint_t *c = (hb_raster_paint_t *) paint_ਅੰਕੜੇ;

  ensure_initialized (c);

  hb_raster_image_t *surf = c->current_surface ();
  ਜੇ (unlikely (!surf)) ਵਾਪਸ;

  ਜੇ (unlikely (!c->fetch_color_stops (color_ਸਤਰ)))
    ਵਾਪਸ;
  hb_color_stop_t *stops = c->scratch_color_stops.arrayZ;
  ਬਿਨਾਨਿਸ਼ਾਨੀ len = c->scratch_color_stops.ਲੰਮਾਈ;
  ਦਸ਼ਮਲਵ mn, mx;
  hb_paint_normalize_color_ਸਤਰ (stops, len, &mn, &mx);

  hb_paint_extend_t extend = hb_color_ਸਤਰ_get_extend (color_ਸਤਰ);
  ਅਚਲ hb_raster_clip_t &clip = c->current_clip ();
  ਬਿਨਾਨਿਸ਼ਾਨੀ clip_w = clip.max_x > clip.min_x ? clip.max_x - clip.min_x : 0;
  ਬਿਨਾਨਿਸ਼ਾਨੀ clip_h = clip.max_y > clip.min_y ? clip.max_y - clip.min_y : 0;
  ਜੇ (unlikely (!c->charge_work ((int64_t) clip_w * clip_h)))
    ਵਾਪਸ;
  ਤਾਰਕਿਕ use_lut = (uint64_t) clip_w * clip_h >= GRADIENT_LUT_MIN_PIXELS;
  uint32_t lut[GRADIENT_LUT_ਮਾਪ];
  ਜੇ (use_lut)
    build_gradient_lut (stops, len, lut);

  /* Reduce 3-point anchor to 2-point gradient axis */
  ਦਸ਼ਮਲਵ lx0, ly0, lx1, ly1;
  hb_paint_reduce_ਰੇਖਿਕ_anchors (x0, y0, x1, y1, x2, y2,
				  &lx0, &ly0, &lx1, &ly1);

  /* Apply normalization to endpoints */
  ਦਸ਼ਮਲਵ gx0 = lx0 + mn * (lx1 - lx0);
  ਦਸ਼ਮਲਵ gy0 = ly0 + mn * (ly1 - ly0);
  ਦਸ਼ਮਲਵ gx1 = lx0 + mx * (lx1 - lx0);
  ਦਸ਼ਮਲਵ gy1 = ly0 + mx * (ly1 - ly0);

  /* Inverse transform: pixel → glyph space */
  hb_transform_t<> t = c->current_effective_transform ();
  ਦਸ਼ਮਲਵ det = t.xx * t.yy - t.xy * t.yx;
  ਜੇ (fabsf (det) < 1e-10f) ਵਾਪਸ;

  {
    ਦਸ਼ਮਲਵ inv_det = 1.f / det;
    ਦਸ਼ਮਲਵ inv_xx =  t.yy * inv_det;
    ਦਸ਼ਮਲਵ inv_xy = -t.xy * inv_det;
    ਦਸ਼ਮਲਵ inv_yx = -t.yx * inv_det;
    ਦਸ਼ਮਲਵ inv_yy =  t.xx * inv_det;
    ਦਸ਼ਮਲਵ inv_x0 = (t.xy * t.y0 - t.yy * t.x0) * inv_det;
    ਦਸ਼ਮਲਵ inv_y0 = (t.yx * t.x0 - t.xx * t.y0) * inv_det;

    /* Gradient direction vector and denominator for projection */
    ਦਸ਼ਮਲਵ dx = gx1 - gx0, dy = gy1 - gy0;
    ਦਸ਼ਮਲਵ denom = dx * dx + dy * dy;
    ਜੇ (denom < 1e-10f) ਵਾਪਸ;
    ਦਸ਼ਮਲਵ inv_denom = 1.f / denom;

    ਬਿਨਾਨਿਸ਼ਾਨੀ stride = surf->extents.stride;
    ਪੂਰਨ ox = surf->extents.x_origin;
    ਪੂਰਨ oy = surf->extents.y_origin;

    ਜੇ (clip.is_rect)
    {
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ py = clip.min_y; py < clip.max_y; py++)
      {
	hb_packed_t<uint32_t> *ਪੰਕਤੀ = (hb_packed_t<uint32_t> *) (surf->ਬਫਰ.arrayZ + py * stride);
	ਦਸ਼ਮਲਵ gx = inv_xx * ((ਦਸ਼ਮਲਵ) ((ਪੂਰਨ) clip.min_x + ox) + 0.5f) + inv_xy * ((ਦਸ਼ਮਲਵ) ((ਪੂਰਨ) py + oy) + 0.5f) + inv_x0;
	ਦਸ਼ਮਲਵ gy = inv_yx * ((ਦਸ਼ਮਲਵ) ((ਪੂਰਨ) clip.min_x + ox) + 0.5f) + inv_yy * ((ਦਸ਼ਮਲਵ) ((ਪੂਰਨ) py + oy) + 0.5f) + inv_y0;
	ਜੇ (use_lut)
	{
	  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ px = clip.min_x; px < clip.max_x; px++)
	  {
	    ਦਸ਼ਮਲਵ proj_t = ((gx - gx0) * dx + (gy - gy0) * dy) * inv_denom;
	    uint32_t src = lookup_gradient_lut (lut, proj_t, extend);
	    ਪੰਕਤੀ[px] = hb_packed_t<uint32_t> (hb_raster_src_over (src, (uint32_t) ਪੰਕਤੀ[px]));
	    gx += inv_xx;
	    gy += inv_yx;
	  }
	}
	ਨਹੀਂਤਾਂ
	{
	  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ px = clip.min_x; px < clip.max_x; px++)
	  {
	    ਦਸ਼ਮਲਵ proj_t = ((gx - gx0) * dx + (gy - gy0) * dy) * inv_denom;
	    uint32_t src = evaluate_color_ਸਤਰ (stops, len, proj_t, extend);
	    ਪੰਕਤੀ[px] = hb_packed_t<uint32_t> (hb_raster_src_over (src, (uint32_t) ਪੰਕਤੀ[px]));
	    gx += inv_xx;
	    gy += inv_yx;
	  }
	}
      }
    }
    ਨਹੀਂਤਾਂ
    {
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ py = clip.min_y; py < clip.max_y; py++)
      {
	hb_packed_t<uint32_t> *ਪੰਕਤੀ = (hb_packed_t<uint32_t> *) (surf->ਬਫਰ.arrayZ + py * stride);
	ਅਚਲ uint8_t *clip_ਪੰਕਤੀ = clip.alpha.arrayZ + py * clip.stride;
	ਦਸ਼ਮਲਵ gx = inv_xx * ((ਦਸ਼ਮਲਵ) ((ਪੂਰਨ) clip.min_x + ox) + 0.5f) + inv_xy * ((ਦਸ਼ਮਲਵ) ((ਪੂਰਨ) py + oy) + 0.5f) + inv_x0;
	ਦਸ਼ਮਲਵ gy = inv_yx * ((ਦਸ਼ਮਲਵ) ((ਪੂਰਨ) clip.min_x + ox) + 0.5f) + inv_yy * ((ਦਸ਼ਮਲਵ) ((ਪੂਰਨ) py + oy) + 0.5f) + inv_y0;
	ਜੇ (use_lut)
	{
	  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ px = clip.min_x; px < clip.max_x; px++)
	  {
	    uint8_t clip_alpha = clip_ਪੰਕਤੀ[px];
	    ਜੇ (clip_alpha == 0)
	    {
	      gx += inv_xx;
	      gy += inv_yx;
	      ਜਾਰੀ;
	    }
	    ਦਸ਼ਮਲਵ proj_t = ((gx - gx0) * dx + (gy - gy0) * dy) * inv_denom;
	    uint32_t src = lookup_gradient_lut (lut, proj_t, extend);
	    src = hb_raster_alpha_mul (src, clip_alpha);
	    ਪੰਕਤੀ[px] = hb_packed_t<uint32_t> (hb_raster_src_over (src, (uint32_t) ਪੰਕਤੀ[px]));
	    gx += inv_xx;
	    gy += inv_yx;
	  }
	}
	ਨਹੀਂਤਾਂ
	{
	  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ px = clip.min_x; px < clip.max_x; px++)
	  {
	    uint8_t clip_alpha = clip_ਪੰਕਤੀ[px];
	    ਜੇ (clip_alpha == 0)
	    {
	      gx += inv_xx;
	      gy += inv_yx;
	      ਜਾਰੀ;
	    }
	    ਦਸ਼ਮਲਵ proj_t = ((gx - gx0) * dx + (gy - gy0) * dy) * inv_denom;
	    uint32_t src = evaluate_color_ਸਤਰ (stops, len, proj_t, extend);
	    src = hb_raster_alpha_mul (src, clip_alpha);
	    ਪੰਕਤੀ[px] = hb_packed_t<uint32_t> (hb_raster_src_over (src, (uint32_t) ਪੰਕਤੀ[px]));
	    gx += inv_xx;
	    gy += inv_yx;
	  }
	}
      }
    }
  }

}

ਸਥਿਰ ਖਾਲੀ
hb_raster_paint_radial_gradient (hb_paint_funcs_t *pfuncs HB_ਅਣਵਰਤਿਆ,
				 ਖਾਲੀ *paint_ਅੰਕੜੇ,
				 hb_color_ਸਤਰ_t *color_ਸਤਰ,
				 ਦਸ਼ਮਲਵ x0, ਦਸ਼ਮਲਵ y0, ਦਸ਼ਮਲਵ r0,
				 ਦਸ਼ਮਲਵ x1, ਦਸ਼ਮਲਵ y1, ਦਸ਼ਮਲਵ r1,
				 ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  hb_raster_paint_t *c = (hb_raster_paint_t *) paint_ਅੰਕੜੇ;

  ensure_initialized (c);

  hb_raster_image_t *surf = c->current_surface ();
  ਜੇ (unlikely (!surf)) ਵਾਪਸ;

  ਜੇ (unlikely (!c->fetch_color_stops (color_ਸਤਰ)))
    ਵਾਪਸ;
  hb_color_stop_t *stops = c->scratch_color_stops.arrayZ;
  ਬਿਨਾਨਿਸ਼ਾਨੀ len = c->scratch_color_stops.ਲੰਮਾਈ;
  ਦਸ਼ਮਲਵ mn, mx;
  hb_paint_normalize_color_ਸਤਰ (stops, len, &mn, &mx);

  hb_paint_extend_t extend = hb_color_ਸਤਰ_get_extend (color_ਸਤਰ);
  ਅਚਲ hb_raster_clip_t &clip = c->current_clip ();
  ਬਿਨਾਨਿਸ਼ਾਨੀ clip_w = clip.max_x > clip.min_x ? clip.max_x - clip.min_x : 0;
  ਬਿਨਾਨਿਸ਼ਾਨੀ clip_h = clip.max_y > clip.min_y ? clip.max_y - clip.min_y : 0;
  ਜੇ (unlikely (!c->charge_work ((int64_t) clip_w * clip_h)))
    ਵਾਪਸ;
  ਤਾਰਕਿਕ use_lut = (uint64_t) clip_w * clip_h >= GRADIENT_LUT_MIN_PIXELS;
  uint32_t lut[GRADIENT_LUT_ਮਾਪ];
  ਜੇ (use_lut)
    build_gradient_lut (stops, len, lut);

  /* Apply normalization to circle parameters */
  ਦਸ਼ਮਲਵ cx0 = x0 + mn * (x1 - x0);
  ਦਸ਼ਮਲਵ cy0 = y0 + mn * (y1 - y0);
  ਦਸ਼ਮਲਵ cr0 = r0 + mn * (r1 - r0);
  ਦਸ਼ਮਲਵ cx1 = x0 + mx * (x1 - x0);
  ਦਸ਼ਮਲਵ cy1 = y0 + mx * (y1 - y0);
  ਦਸ਼ਮਲਵ cr1 = r0 + mx * (r1 - r0);

  /* Inverse transform */
  hb_transform_t<> t = c->current_effective_transform ();
  ਦਸ਼ਮਲਵ det = t.xx * t.yy - t.xy * t.yx;
  ਜੇ (fabsf (det) < 1e-10f) ਵਾਪਸ;

  {
    ਦਸ਼ਮਲਵ inv_det = 1.f / det;
    ਦਸ਼ਮਲਵ inv_xx =  t.yy * inv_det;
    ਦਸ਼ਮਲਵ inv_xy = -t.xy * inv_det;
    ਦਸ਼ਮਲਵ inv_yx = -t.yx * inv_det;
    ਦਸ਼ਮਲਵ inv_yy =  t.xx * inv_det;
    ਦਸ਼ਮਲਵ inv_x0 = (t.xy * t.y0 - t.yy * t.x0) * inv_det;
    ਦਸ਼ਮਲਵ inv_y0 = (t.yx * t.x0 - t.xx * t.y0) * inv_det;

    /* Precompute quadratic coefficients for radial gradient:
     * |p - c0 - t*(c1-c0)|^2 = (r0 + t*(r1-r0))^2
     *
     * Expanding gives At^2 + Bt + C = 0 where:
     *   cdx = c1.x - c0.x, cdy = c1.y - c0.y, dr = r1 - r0
     *   A = cdx^2 + cdy^2 - dr^2
     *   B = -2*(px-c0.x)*cdx - 2*(py-c0.y)*cdy - 2*r0*dr
     *   C = (px-c0.x)^2 + (py-c0.y)^2 - r0^2
     */
    ਦਸ਼ਮਲਵ cdx = cx1 - cx0, cdy = cy1 - cy0;
    ਦਸ਼ਮਲਵ dr = cr1 - cr0;
    ਦਸ਼ਮਲਵ A = cdx * cdx + cdy * cdy - dr * dr;

    ਬਿਨਾਨਿਸ਼ਾਨੀ stride = surf->extents.stride;
    ਪੂਰਨ ox = surf->extents.x_origin;
    ਪੂਰਨ oy = surf->extents.y_origin;

    ਜੇ (clip.is_rect)
    {
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ py = clip.min_y; py < clip.max_y; py++)
      {
	hb_packed_t<uint32_t> *ਪੰਕਤੀ = (hb_packed_t<uint32_t> *) (surf->ਬਫਰ.arrayZ + py * stride);
	ਦਸ਼ਮਲਵ gx = inv_xx * ((ਦਸ਼ਮਲਵ) ((ਪੂਰਨ) clip.min_x + ox) + 0.5f) + inv_xy * ((ਦਸ਼ਮਲਵ) ((ਪੂਰਨ) py + oy) + 0.5f) + inv_x0;
	ਦਸ਼ਮਲਵ gy = inv_yx * ((ਦਸ਼ਮਲਵ) ((ਪੂਰਨ) clip.min_x + ox) + 0.5f) + inv_yy * ((ਦਸ਼ਮਲਵ) ((ਪੂਰਨ) py + oy) + 0.5f) + inv_y0;
	ਜੇ (use_lut)
	{
	  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ px = clip.min_x; px < clip.max_x; px++)
	  {
	    ਦਸ਼ਮਲਵ dpx = gx - cx0, dpy = gy - cy0;
	    ਦਸ਼ਮਲਵ B = -2.f * (dpx * cdx + dpy * cdy + cr0 * dr);
	    ਦਸ਼ਮਲਵ C = dpx * dpx + dpy * dpy - cr0 * cr0;

	    ਦਸ਼ਮਲਵ grad_t;
	    ਜੇ (fabsf (A) > 1e-10f)
	    {
	      ਦਸ਼ਮਲਵ disc = B * B - 4.f * A * C;
	      ਜੇ (disc < 0.f)
	      {
		gx += inv_xx;
		gy += inv_yx;
		ਜਾਰੀ;
	      }
	      ਦਸ਼ਮਲਵ sq = sqrtf (disc);
	      /* Pick the larger root (t closer to 1 = outer circle) */
	      ਦਸ਼ਮਲਵ t1 = (-B + sq) / (2.f * A);
	      ਦਸ਼ਮਲਵ t2 = (-B - sq) / (2.f * A);
	      /* Choose the root that gives a positive radius */
	      ਜੇ (cr0 + t1 * dr >= 0.f)
		grad_t = t1;
	      ਨਹੀਂਤਾਂ
		grad_t = t2;
	    }
	    ਨਹੀਂਤਾਂ
	    {
	      /* Linear case: Bt + C = 0 */
	      ਜੇ (fabsf (B) < 1e-10f)
	      {
		gx += inv_xx;
		gy += inv_yx;
		ਜਾਰੀ;
	      }
	      grad_t = -C / B;
	    }

	    uint32_t src = lookup_gradient_lut (lut, grad_t, extend);
	    ਪੰਕਤੀ[px] = hb_packed_t<uint32_t> (hb_raster_src_over (src, (uint32_t) ਪੰਕਤੀ[px]));
	    gx += inv_xx;
	    gy += inv_yx;
	  }
	}
	ਨਹੀਂਤਾਂ
	{
	  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ px = clip.min_x; px < clip.max_x; px++)
	  {
	    ਦਸ਼ਮਲਵ dpx = gx - cx0, dpy = gy - cy0;
	    ਦਸ਼ਮਲਵ B = -2.f * (dpx * cdx + dpy * cdy + cr0 * dr);
	    ਦਸ਼ਮਲਵ C = dpx * dpx + dpy * dpy - cr0 * cr0;

	    ਦਸ਼ਮਲਵ grad_t;
	    ਜੇ (fabsf (A) > 1e-10f)
	    {
	      ਦਸ਼ਮਲਵ disc = B * B - 4.f * A * C;
	      ਜੇ (disc < 0.f)
	      {
		gx += inv_xx;
		gy += inv_yx;
		ਜਾਰੀ;
	      }
	      ਦਸ਼ਮਲਵ sq = sqrtf (disc);
	      ਦਸ਼ਮਲਵ t1 = (-B + sq) / (2.f * A);
	      ਦਸ਼ਮਲਵ t2 = (-B - sq) / (2.f * A);
	      grad_t = (cr0 + t1 * dr >= 0.f) ? t1 : t2;
	    }
	    ਨਹੀਂਤਾਂ
	    {
	      ਜੇ (fabsf (B) < 1e-10f)
	      {
		gx += inv_xx;
		gy += inv_yx;
		ਜਾਰੀ;
	      }
	      grad_t = -C / B;
	    }

	    uint32_t src = evaluate_color_ਸਤਰ (stops, len, grad_t, extend);
	    ਪੰਕਤੀ[px] = hb_packed_t<uint32_t> (hb_raster_src_over (src, (uint32_t) ਪੰਕਤੀ[px]));
	    gx += inv_xx;
	    gy += inv_yx;
	  }
	}
      }
    }
    ਨਹੀਂਤਾਂ
    {
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ py = clip.min_y; py < clip.max_y; py++)
      {
	hb_packed_t<uint32_t> *ਪੰਕਤੀ = (hb_packed_t<uint32_t> *) (surf->ਬਫਰ.arrayZ + py * stride);
	ਅਚਲ uint8_t *clip_ਪੰਕਤੀ = clip.alpha.arrayZ + py * clip.stride;
	ਦਸ਼ਮਲਵ gx = inv_xx * ((ਦਸ਼ਮਲਵ) ((ਪੂਰਨ) clip.min_x + ox) + 0.5f) + inv_xy * ((ਦਸ਼ਮਲਵ) ((ਪੂਰਨ) py + oy) + 0.5f) + inv_x0;
	ਦਸ਼ਮਲਵ gy = inv_yx * ((ਦਸ਼ਮਲਵ) ((ਪੂਰਨ) clip.min_x + ox) + 0.5f) + inv_yy * ((ਦਸ਼ਮਲਵ) ((ਪੂਰਨ) py + oy) + 0.5f) + inv_y0;
	ਜੇ (use_lut)
	{
	  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ px = clip.min_x; px < clip.max_x; px++)
	  {
	    uint8_t clip_alpha = clip_ਪੰਕਤੀ[px];
	    ਜੇ (clip_alpha == 0)
	    {
	      gx += inv_xx;
	      gy += inv_yx;
	      ਜਾਰੀ;
	    }
	    ਦਸ਼ਮਲਵ dpx = gx - cx0, dpy = gy - cy0;
	    ਦਸ਼ਮਲਵ B = -2.f * (dpx * cdx + dpy * cdy + cr0 * dr);
	    ਦਸ਼ਮਲਵ C = dpx * dpx + dpy * dpy - cr0 * cr0;

	    ਦਸ਼ਮਲਵ grad_t;
	    ਜੇ (fabsf (A) > 1e-10f)
	    {
	      ਦਸ਼ਮਲਵ disc = B * B - 4.f * A * C;
	      ਜੇ (disc < 0.f)
	      {
		gx += inv_xx;
		gy += inv_yx;
		ਜਾਰੀ;
	      }
	      ਦਸ਼ਮਲਵ sq = sqrtf (disc);
	      ਦਸ਼ਮਲਵ t1 = (-B + sq) / (2.f * A);
	      ਦਸ਼ਮਲਵ t2 = (-B - sq) / (2.f * A);
	      grad_t = (cr0 + t1 * dr >= 0.f) ? t1 : t2;
	    }
	    ਨਹੀਂਤਾਂ
	    {
	      ਜੇ (fabsf (B) < 1e-10f)
	      {
		gx += inv_xx;
		gy += inv_yx;
		ਜਾਰੀ;
	      }
	      grad_t = -C / B;
	    }

	    uint32_t src = lookup_gradient_lut (lut, grad_t, extend);
	    src = hb_raster_alpha_mul (src, clip_alpha);
	    ਪੰਕਤੀ[px] = hb_packed_t<uint32_t> (hb_raster_src_over (src, (uint32_t) ਪੰਕਤੀ[px]));
	    gx += inv_xx;
	    gy += inv_yx;
	  }
	}
	ਨਹੀਂਤਾਂ
	{
	  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ px = clip.min_x; px < clip.max_x; px++)
	  {
	    uint8_t clip_alpha = clip_ਪੰਕਤੀ[px];
	    ਜੇ (clip_alpha == 0)
	    {
	      gx += inv_xx;
	      gy += inv_yx;
	      ਜਾਰੀ;
	    }
	    ਦਸ਼ਮਲਵ dpx = gx - cx0, dpy = gy - cy0;
	    ਦਸ਼ਮਲਵ B = -2.f * (dpx * cdx + dpy * cdy + cr0 * dr);
	    ਦਸ਼ਮਲਵ C = dpx * dpx + dpy * dpy - cr0 * cr0;

	    ਦਸ਼ਮਲਵ grad_t;
	    ਜੇ (fabsf (A) > 1e-10f)
	    {
	      ਦਸ਼ਮਲਵ disc = B * B - 4.f * A * C;
	      ਜੇ (disc < 0.f)
	      {
		gx += inv_xx;
		gy += inv_yx;
		ਜਾਰੀ;
	      }
	      ਦਸ਼ਮਲਵ sq = sqrtf (disc);
	      ਦਸ਼ਮਲਵ t1 = (-B + sq) / (2.f * A);
	      ਦਸ਼ਮਲਵ t2 = (-B - sq) / (2.f * A);
	      grad_t = (cr0 + t1 * dr >= 0.f) ? t1 : t2;
	    }
	    ਨਹੀਂਤਾਂ
	    {
	      ਜੇ (fabsf (B) < 1e-10f)
	      {
		gx += inv_xx;
		gy += inv_yx;
		ਜਾਰੀ;
	      }
	      grad_t = -C / B;
	    }

	    uint32_t src = evaluate_color_ਸਤਰ (stops, len, grad_t, extend);
	    src = hb_raster_alpha_mul (src, clip_alpha);
	    ਪੰਕਤੀ[px] = hb_packed_t<uint32_t> (hb_raster_src_over (src, (uint32_t) ਪੰਕਤੀ[px]));
	    gx += inv_xx;
	    gy += inv_yx;
	  }
	}
      }
    }
  }

}

ਸਥਿਰ ਖਾਲੀ
hb_raster_paint_sweep_gradient (hb_paint_funcs_t *pfuncs HB_ਅਣਵਰਤਿਆ,
				ਖਾਲੀ *paint_ਅੰਕੜੇ,
				hb_color_ਸਤਰ_t *color_ਸਤਰ,
				ਦਸ਼ਮਲਵ cx, ਦਸ਼ਮਲਵ cy,
				ਦਸ਼ਮਲਵ ਸ਼ੁਰੂ_angle,
				ਦਸ਼ਮਲਵ ਸਿਰਾ_angle,
				ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  hb_raster_paint_t *c = (hb_raster_paint_t *) paint_ਅੰਕੜੇ;

  ensure_initialized (c);

  hb_raster_image_t *surf = c->current_surface ();
  ਜੇ (unlikely (!surf)) ਵਾਪਸ;

  ਜੇ (unlikely (!c->fetch_color_stops (color_ਸਤਰ)))
    ਵਾਪਸ;
  hb_color_stop_t *stops = c->scratch_color_stops.arrayZ;
  ਬਿਨਾਨਿਸ਼ਾਨੀ len = c->scratch_color_stops.ਲੰਮਾਈ;
  ਦਸ਼ਮਲਵ mn, mx;
  hb_paint_normalize_color_ਸਤਰ (stops, len, &mn, &mx);

  hb_paint_extend_t extend = hb_color_ਸਤਰ_get_extend (color_ਸਤਰ);
  ਅਚਲ hb_raster_clip_t &clip = c->current_clip ();
  ਬਿਨਾਨਿਸ਼ਾਨੀ clip_w = clip.max_x > clip.min_x ? clip.max_x - clip.min_x : 0;
  ਬਿਨਾਨਿਸ਼ਾਨੀ clip_h = clip.max_y > clip.min_y ? clip.max_y - clip.min_y : 0;
  ਜੇ (unlikely (!c->charge_work ((int64_t) clip_w * clip_h)))
    ਵਾਪਸ;
  ਤਾਰਕਿਕ use_lut = (uint64_t) clip_w * clip_h >= GRADIENT_LUT_MIN_PIXELS;
  uint32_t lut[GRADIENT_LUT_ਮਾਪ];
  ਜੇ (use_lut)
    build_gradient_lut (stops, len, lut);

  /* Apply normalization to angle range */
  ਦਸ਼ਮਲਵ a0 = ਸ਼ੁਰੂ_angle + mn * (ਸਿਰਾ_angle - ਸ਼ੁਰੂ_angle);
  ਦਸ਼ਮਲਵ a1 = ਸ਼ੁਰੂ_angle + mx * (ਸਿਰਾ_angle - ਸ਼ੁਰੂ_angle);
  ਦਸ਼ਮਲਵ angle_range = a1 - a0;

  /* Inverse transform */
  hb_transform_t<> t = c->current_effective_transform ();
  ਦਸ਼ਮਲਵ det = t.xx * t.yy - t.xy * t.yx;
  ਜੇ (fabsf (det) < 1e-10f || fabsf (angle_range) < 1e-10f) ਵਾਪਸ;

  {
    ਦਸ਼ਮਲਵ inv_det = 1.f / det;
    ਦਸ਼ਮਲਵ inv_xx =  t.yy * inv_det;
    ਦਸ਼ਮਲਵ inv_xy = -t.xy * inv_det;
    ਦਸ਼ਮਲਵ inv_yx = -t.yx * inv_det;
    ਦਸ਼ਮਲਵ inv_yy =  t.xx * inv_det;
    ਦਸ਼ਮਲਵ inv_x0 = (t.xy * t.y0 - t.yy * t.x0) * inv_det;
    ਦਸ਼ਮਲਵ inv_y0 = (t.yx * t.x0 - t.xx * t.y0) * inv_det;

    ਦਸ਼ਮਲਵ inv_angle_range = 1.f / angle_range;

    ਬਿਨਾਨਿਸ਼ਾਨੀ stride = surf->extents.stride;
    ਪੂਰਨ ox = surf->extents.x_origin;
    ਪੂਰਨ oy = surf->extents.y_origin;

    ਜੇ (clip.is_rect)
    {
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ py = clip.min_y; py < clip.max_y; py++)
      {
	hb_packed_t<uint32_t> *ਪੰਕਤੀ = (hb_packed_t<uint32_t> *) (surf->ਬਫਰ.arrayZ + py * stride);
	ਦਸ਼ਮਲਵ gx = inv_xx * ((ਦਸ਼ਮਲਵ) ((ਪੂਰਨ) clip.min_x + ox) + 0.5f) + inv_xy * ((ਦਸ਼ਮਲਵ) ((ਪੂਰਨ) py + oy) + 0.5f) + inv_x0;
	ਦਸ਼ਮਲਵ gy = inv_yx * ((ਦਸ਼ਮਲਵ) ((ਪੂਰਨ) clip.min_x + ox) + 0.5f) + inv_yy * ((ਦਸ਼ਮਲਵ) ((ਪੂਰਨ) py + oy) + 0.5f) + inv_y0;
	ਜੇ (use_lut)
	{
	  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ px = clip.min_x; px < clip.max_x; px++)
	  {
	    ਦਸ਼ਮਲਵ angle = atan2f (gy - cy, gx - cx);
	    ਜੇ (angle < 0) angle += (ਦਸ਼ਮਲਵ) HB_2_PI;
	    ਦਸ਼ਮਲਵ grad_t = (angle - a0) * inv_angle_range;
	    uint32_t src = lookup_gradient_lut (lut, grad_t, extend);
	    ਪੰਕਤੀ[px] = hb_packed_t<uint32_t> (hb_raster_src_over (src, (uint32_t) ਪੰਕਤੀ[px]));
	    gx += inv_xx;
	    gy += inv_yx;
	  }
	}
	ਨਹੀਂਤਾਂ
	{
	  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ px = clip.min_x; px < clip.max_x; px++)
	  {
	    ਦਸ਼ਮਲਵ angle = atan2f (gy - cy, gx - cx);
	    ਜੇ (angle < 0) angle += (ਦਸ਼ਮਲਵ) HB_2_PI;
	    ਦਸ਼ਮਲਵ grad_t = (angle - a0) * inv_angle_range;
	    uint32_t src = evaluate_color_ਸਤਰ (stops, len, grad_t, extend);
	    ਪੰਕਤੀ[px] = hb_packed_t<uint32_t> (hb_raster_src_over (src, (uint32_t) ਪੰਕਤੀ[px]));
	    gx += inv_xx;
	    gy += inv_yx;
	  }
	}
      }
    }
    ਨਹੀਂਤਾਂ
    {
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ py = clip.min_y; py < clip.max_y; py++)
      {
	hb_packed_t<uint32_t> *ਪੰਕਤੀ = (hb_packed_t<uint32_t> *) (surf->ਬਫਰ.arrayZ + py * stride);
	ਅਚਲ uint8_t *clip_ਪੰਕਤੀ = clip.alpha.arrayZ + py * clip.stride;
	ਦਸ਼ਮਲਵ gx = inv_xx * ((ਦਸ਼ਮਲਵ) ((ਪੂਰਨ) clip.min_x + ox) + 0.5f) + inv_xy * ((ਦਸ਼ਮਲਵ) ((ਪੂਰਨ) py + oy) + 0.5f) + inv_x0;
	ਦਸ਼ਮਲਵ gy = inv_yx * ((ਦਸ਼ਮਲਵ) ((ਪੂਰਨ) clip.min_x + ox) + 0.5f) + inv_yy * ((ਦਸ਼ਮਲਵ) ((ਪੂਰਨ) py + oy) + 0.5f) + inv_y0;
	ਜੇ (use_lut)
	{
	  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ px = clip.min_x; px < clip.max_x; px++)
	  {
	    uint8_t clip_alpha = clip_ਪੰਕਤੀ[px];
	    ਜੇ (clip_alpha == 0)
	    {
	      gx += inv_xx;
	      gy += inv_yx;
	      ਜਾਰੀ;
	    }
	    ਦਸ਼ਮਲਵ angle = atan2f (gy - cy, gx - cx);
	    ਜੇ (angle < 0) angle += (ਦਸ਼ਮਲਵ) HB_2_PI;
	    ਦਸ਼ਮਲਵ grad_t = (angle - a0) * inv_angle_range;
	    uint32_t src = lookup_gradient_lut (lut, grad_t, extend);
	    src = hb_raster_alpha_mul (src, clip_alpha);
	    ਪੰਕਤੀ[px] = hb_packed_t<uint32_t> (hb_raster_src_over (src, (uint32_t) ਪੰਕਤੀ[px]));
	    gx += inv_xx;
	    gy += inv_yx;
	  }
	}
	ਨਹੀਂਤਾਂ
	{
	  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ px = clip.min_x; px < clip.max_x; px++)
	  {
	    uint8_t clip_alpha = clip_ਪੰਕਤੀ[px];
	    ਜੇ (clip_alpha == 0)
	    {
	      gx += inv_xx;
	      gy += inv_yx;
	      ਜਾਰੀ;
	    }
	    ਦਸ਼ਮਲਵ angle = atan2f (gy - cy, gx - cx);
	    ਜੇ (angle < 0) angle += (ਦਸ਼ਮਲਵ) HB_2_PI;
	    ਦਸ਼ਮਲਵ grad_t = (angle - a0) * inv_angle_range;
	    uint32_t src = evaluate_color_ਸਤਰ (stops, len, grad_t, extend);
	    src = hb_raster_alpha_mul (src, clip_alpha);
	    ਪੰਕਤੀ[px] = hb_packed_t<uint32_t> (hb_raster_src_over (src, (uint32_t) ਪੰਕਤੀ[px]));
	    gx += inv_xx;
	    gy += inv_yx;
	  }
	}
      }
    }
  }

}

ਸਥਿਰ hb_bool_t
hb_raster_paint_custom_palette_color (hb_paint_funcs_t *funcs HB_ਅਣਵਰਤਿਆ,
				      ਖਾਲੀ *paint_ਅੰਕੜੇ,
				      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ color_ਸੂਚਕਾਂਕ,
				      hb_color_t *color,
				      ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  hb_raster_paint_t *c = (hb_raster_paint_t *) paint_ਅੰਕੜੇ;
  ਜੇ (likely (c->custom_palette && hb_map_has (c->custom_palette, color_ਸੂਚਕਾਂਕ)))
  {
    *color = hb_map_get (c->custom_palette, color_ਸੂਚਕਾਂਕ);
    ਵਾਪਸ ਸੱਚ;
  }
  ਵਾਪਸ ਝੂਠ;
}

ਸਥਿਰ hb_bool_t
hb_raster_paint_ਸੈੱਟ_budget (hb_paint_funcs_t *, ਖਾਲੀ *paint_ਅੰਕੜੇ,
			    int64_t budget, ਖਾਲੀ *)
{
  ਸਵੈਚਾਲਿਤ *paint = (hb_raster_paint_t *) paint_ਅੰਕੜੇ;
  paint->budget = budget;
  paint->recharge_budget ();
  ਵਾਪਸ ਸੱਚ;
}

ਸਥਿਰ int64_t
hb_raster_paint_get_budget (hb_paint_funcs_t *, ਖਾਲੀ *paint_ਅੰਕੜੇ, ਖਾਲੀ *)
{
  ਵਾਪਸ ((hb_raster_paint_t *) paint_ਅੰਕੜੇ)->budget;
}

ਸਥਿਰ int64_t *
hb_raster_paint_get_budget_ਬਚਿਆ (hb_paint_funcs_t *, ਖਾਲੀ *paint_ਅੰਕੜੇ, ਖਾਲੀ *)
{
  ਵਾਪਸ &((hb_raster_paint_t *) paint_ਅੰਕੜੇ)->budget_ਬਚਿਆ;
}


/*
 * Lazy-loader singleton for paint funcs
 */

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ free_static_raster_paint_funcs ();

ਸਥਿਰ ਬਣਤਰ hb_raster_paint_funcs_lazy_loader_t : hb_paint_funcs_lazy_loader_t<hb_raster_paint_funcs_lazy_loader_t>
{
  ਸਥਿਰ hb_paint_funcs_t *create ()
  {
    hb_paint_funcs_t *funcs = hb_paint_funcs_create ();

    hb_paint_funcs_ਸੈੱਟ_ਧੱਕੋ_transform_func (funcs, hb_raster_paint_ਧੱਕੋ_transform, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_paint_funcs_ਸੈੱਟ_ਕੱਢੋ_transform_func (funcs, hb_raster_paint_ਕੱਢੋ_transform, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_paint_funcs_ਸੈੱਟ_fill_glyph_func (funcs, hb_raster_paint_fill_glyph, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_paint_funcs_ਸੈੱਟ_color_glyph_func (funcs, hb_raster_paint_color_glyph, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_paint_funcs_ਸੈੱਟ_ਧੱਕੋ_clip_glyph_func (funcs, hb_raster_paint_ਧੱਕੋ_clip_glyph, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_paint_funcs_ਸੈੱਟ_ਧੱਕੋ_clip_rectangle_func (funcs, hb_raster_paint_ਧੱਕੋ_clip_rectangle, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_paint_funcs_ਸੈੱਟ_ਧੱਕੋ_clip_ਰਸਤਾ_ਸ਼ੁਰੂ_func (funcs, hb_raster_paint_ਧੱਕੋ_clip_ਰਸਤਾ_ਸ਼ੁਰੂ, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_paint_funcs_ਸੈੱਟ_ਧੱਕੋ_clip_ਰਸਤਾ_ਸਿਰਾ_func (funcs, hb_raster_paint_ਧੱਕੋ_clip_ਰਸਤਾ_ਸਿਰਾ, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_paint_funcs_ਸੈੱਟ_ਕੱਢੋ_clip_func (funcs, hb_raster_paint_ਕੱਢੋ_clip, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_paint_funcs_ਸੈੱਟ_ਧੱਕੋ_ਸਮੂਹ_func (funcs, hb_raster_paint_ਧੱਕੋ_ਸਮੂਹ, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_paint_funcs_ਸੈੱਟ_ਕੱਢੋ_ਸਮੂਹ_func (funcs, hb_raster_paint_ਕੱਢੋ_ਸਮੂਹ, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_paint_funcs_ਸੈੱਟ_color_func (funcs, hb_raster_paint_color, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_paint_funcs_ਸੈੱਟ_image_func (funcs, hb_raster_paint_image, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_paint_funcs_ਸੈੱਟ_ਰੇਖਿਕ_gradient_func (funcs, hb_raster_paint_ਰੇਖਿਕ_gradient, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_paint_funcs_ਸੈੱਟ_radial_gradient_func (funcs, hb_raster_paint_radial_gradient, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_paint_funcs_ਸੈੱਟ_sweep_gradient_func (funcs, hb_raster_paint_sweep_gradient, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_paint_funcs_ਸੈੱਟ_custom_palette_color_func (funcs, hb_raster_paint_custom_palette_color, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_paint_funcs_ਸੈੱਟ_ਸੈੱਟ_budget_func (funcs, hb_raster_paint_ਸੈੱਟ_budget, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_paint_funcs_ਸੈੱਟ_get_budget_func (funcs, hb_raster_paint_get_budget, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_paint_funcs_ਸੈੱਟ_get_budget_ਬਚਿਆ_func (funcs, hb_raster_paint_get_budget_ਬਚਿਆ, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);

    hb_paint_funcs_make_immutable (funcs);

    hb_atexit (free_static_raster_paint_funcs);

    ਵਾਪਸ funcs;
  }
} static_raster_paint_funcs;

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
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
hb_raster_paint_create_or_fail (ਖਾਲੀ)
{
  hb_raster_paint_t *paint = hb_object_create<hb_raster_paint_t> ();
  ਜੇ (unlikely (!paint))
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

  paint->clip_rdr = hb_raster_draw_create_or_fail ();
  ਜੇ (unlikely (!paint->clip_rdr))
  {
    hb_free (paint);
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
  }

  ਵਾਪਸ paint;
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
  ਵਾਪਸ hb_object_reference (paint);
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
ਖਾਲੀ
hb_raster_paint_destroy (hb_raster_paint_t *paint)
{
  ਜੇ (!hb_object_should_destroy (paint))
    ਵਾਪਸ;

  hb_map_destroy (paint->custom_palette);
  hb_raster_draw_destroy (paint->clip_rdr);
  ਲਈ (ਸਵੈਚਾਲਿਤ *s : paint->surface_ਢੇਰ)
    hb_raster_image_destroy (s);
  ਲਈ (ਸਵੈਚਾਲਿਤ *s : paint->surface_cache)
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
hb_raster_paint_ਸੈੱਟ_user_ਅੰਕੜੇ (hb_raster_paint_t  *paint,
			       hb_user_ਅੰਕੜੇ_ਕੁੰਜੀ_t *key,
			       ਖਾਲੀ               *ਅੰਕੜੇ,
			       hb_destroy_func_t   destroy,
			       hb_bool_t           replace)
{
  ਵਾਪਸ hb_object_ਸੈੱਟ_user_ਅੰਕੜੇ (paint, key, ਅੰਕੜੇ, destroy, replace);
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
ਖਾਲੀ *
hb_raster_paint_get_user_ਅੰਕੜੇ (ਅਚਲ hb_raster_paint_t  *paint,
			       hb_user_ਅੰਕੜੇ_ਕੁੰਜੀ_t *key)
{
  ਵਾਪਸ hb_object_get_user_ਅੰਕੜੇ (paint, key);
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
ਖਾਲੀ
hb_raster_paint_ਸੈੱਟ_transform (hb_raster_paint_t *paint,
			       ਦਸ਼ਮਲਵ xx, ਦਸ਼ਮਲਵ yx,
			       ਦਸ਼ਮਲਵ xy, ਦਸ਼ਮਲਵ yy,
			       ਦਸ਼ਮਲਵ dx, ਦਸ਼ਮਲਵ dy)
{
  paint->ਅਧਾਰ_transform = {xx, yx, xy, yy, dx, dy};
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
ਖਾਲੀ
hb_raster_paint_get_transform (ਅਚਲ hb_raster_paint_t *paint,
			       ਦਸ਼ਮਲਵ *xx, ਦਸ਼ਮਲਵ *yx,
			       ਦਸ਼ਮਲਵ *xy, ਦਸ਼ਮਲਵ *yy,
			       ਦਸ਼ਮਲਵ *dx, ਦਸ਼ਮਲਵ *dy)
{
  ਜੇ (xx) *xx = paint->ਅਧਾਰ_transform.xx;
  ਜੇ (yx) *yx = paint->ਅਧਾਰ_transform.yx;
  ਜੇ (xy) *xy = paint->ਅਧਾਰ_transform.xy;
  ਜੇ (yy) *yy = paint->ਅਧਾਰ_transform.yy;
  ਜੇ (dx) *dx = paint->ਅਧਾਰ_transform.x0;
  ਜੇ (dy) *dy = paint->ਅਧਾਰ_transform.y0;
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
ਖਾਲੀ
hb_raster_paint_ਸੈੱਟ_scale_ਗੁਣਕ (hb_raster_paint_t *paint,
				  ਦਸ਼ਮਲਵ x_scale_ਗੁਣਕ,
				  ਦਸ਼ਮਲਵ y_scale_ਗੁਣਕ)
{
  paint->x_scale_ਗੁਣਕ = x_scale_ਗੁਣਕ > 0.f ? x_scale_ਗੁਣਕ : 1.f;
  paint->y_scale_ਗੁਣਕ = y_scale_ਗੁਣਕ > 0.f ? y_scale_ਗੁਣਕ : 1.f;
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
ਖਾਲੀ
hb_raster_paint_get_scale_ਗੁਣਕ (ਅਚਲ hb_raster_paint_t *paint,
				  ਦਸ਼ਮਲਵ *x_scale_ਗੁਣਕ,
				  ਦਸ਼ਮਲਵ *y_scale_ਗੁਣਕ)
{
  ਜੇ (x_scale_ਗੁਣਕ) *x_scale_ਗੁਣਕ = paint->x_scale_ਗੁਣਕ;
  ਜੇ (y_scale_ਗੁਣਕ) *y_scale_ਗੁਣਕ = paint->y_scale_ਗੁਣਕ;
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
ਖਾਲੀ
hb_raster_paint_ਸੈੱਟ_extents (hb_raster_paint_t         *paint,
			     ਅਚਲ hb_raster_extents_t *extents)
{
  paint->fixed_extents = *extents;
  paint->has_extents = ਸੱਚ;
  ਜੇ (paint->fixed_extents.stride == 0)
    paint->fixed_extents.stride = paint->fixed_extents.ਚੌੜਾਈ * 4;
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
hb_raster_paint_get_extents (ਅਚਲ hb_raster_paint_t   *paint,
			     hb_raster_extents_t *extents)
{
  ਜੇ (!paint->has_extents)
    ਵਾਪਸ ਝੂਠ;

  ਜੇ (extents)
    *extents = paint->fixed_extents;
  ਵਾਪਸ ਸੱਚ;
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
hb_raster_paint_ਸੈੱਟ_glyph_extents (hb_raster_paint_t        *paint,
				   ਅਚਲ hb_glyph_extents_t *glyph_extents)
{
  ਦਸ਼ਮਲਵ x0 = (ਦਸ਼ਮਲਵ) glyph_extents->x_bearing;
  ਦਸ਼ਮਲਵ y0 = (ਦਸ਼ਮਲਵ) glyph_extents->y_bearing;
  ਦਸ਼ਮਲਵ x1 = (ਦਸ਼ਮਲਵ) glyph_extents->x_bearing + glyph_extents->ਚੌੜਾਈ;
  ਦਸ਼ਮਲਵ y1 = (ਦਸ਼ਮਲਵ) glyph_extents->y_bearing + glyph_extents->ਉਚਾਈ;

  ਦਸ਼ਮਲਵ xmin = hb_min (x0, x1);
  ਦਸ਼ਮਲਵ xmax = hb_max (x0, x1);
  ਦਸ਼ਮਲਵ ymin = hb_min (y0, y1);
  ਦਸ਼ਮਲਵ ymax = hb_max (y0, y1);

  ਦਸ਼ਮਲਵ px[4] = {xmin, xmin, xmax, xmax};
  ਦਸ਼ਮਲਵ py[4] = {ymin, ymax, ymin, ymax};

  hb_transform_t<> t = paint->ਅਧਾਰ_transform;
  paint->apply_scale_ਗੁਣਕ (t);

  ਦਸ਼ਮਲਵ tx, ty;
  t.transform_ਬਿੰਦੂ (px[0], py[0]);
  tx = px[0]; ty = py[0];
  ਦਸ਼ਮਲਵ tx_min = tx, tx_max = tx;
  ਦਸ਼ਮਲਵ ty_min = ty, ty_max = ty;

  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 1; i < 4; i++)
  {
    t.transform_ਬਿੰਦੂ (px[i], py[i]);
    tx_min = hb_min (tx_min, px[i]);
    tx_max = hb_max (tx_max, px[i]);
    ty_min = hb_min (ty_min, py[i]);
    ty_max = hb_max (ty_max, py[i]);
  }

  int32_t ex0 = hb_clamp_ਮੰਜ਼ਿਲ<int32_t> (floorf (tx_min));
  int32_t ey0 = hb_clamp_ਮੰਜ਼ਿਲ<int32_t> (floorf (ty_min));
  int32_t ex1 = hb_clamp_ਮੰਜ਼ਿਲ<int32_t> (ceilf  (tx_max));
  int32_t ey1 = hb_clamp_ਮੰਜ਼ਿਲ<int32_t> (ceilf  (ty_max));

  ਜੇ (ex1 <= ex0 || ey1 <= ey0)
  {
    paint->fixed_extents = {};
    paint->has_extents = ਝੂਠ;
    ਵਾਪਸ ਝੂਠ;
  }

  paint->fixed_extents = {
    ex0, ey0,
    (ਬਿਨਾਨਿਸ਼ਾਨੀ) hb_min ((int64_t) ex1 - ex0, (int64_t) HB_RASTER_MAX_AUTO_DIMENSION),
    (ਬਿਨਾਨਿਸ਼ਾਨੀ) hb_min ((int64_t) ey1 - ey0, (int64_t) HB_RASTER_MAX_AUTO_DIMENSION),
    0
  };
  paint->has_extents = ਸੱਚ;
  ਜੇ (paint->fixed_extents.stride == 0)
    paint->fixed_extents.stride = paint->fixed_extents.ਚੌੜਾਈ * 4;
  ਵਾਪਸ ਸੱਚ;
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
ਖਾਲੀ
hb_raster_paint_ਸੈੱਟ_foreground (hb_raster_paint_t *paint,
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
hb_raster_paint_get_foreground (ਅਚਲ hb_raster_paint_t *paint)
{
  ਵਾਪਸ paint->foreground;
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
ਖਾਲੀ
hb_raster_paint_ਸੈੱਟ_background (hb_raster_paint_t *paint,
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
hb_raster_paint_get_background (ਅਚਲ hb_raster_paint_t *paint)
{
  ਵਾਪਸ paint->background;
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
ਖਾਲੀ
hb_raster_paint_ਸੈੱਟ_palette (hb_raster_paint_t *paint,
			     ਬਿਨਾਨਿਸ਼ਾਨੀ           palette)
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
ਬਿਨਾਨਿਸ਼ਾਨੀ
hb_raster_paint_get_palette (ਅਚਲ hb_raster_paint_t *paint)
{
  ਵਾਪਸ paint->palette;
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
ਖਾਲੀ
hb_raster_paint_clear_custom_palette_colors (hb_raster_paint_t *paint)
{
  ਜੇ (paint->custom_palette)
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
hb_raster_paint_ਸੈੱਟ_custom_palette_color (hb_raster_paint_t *paint,
					  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ       color_ਸੂਚਕਾਂਕ,
					  hb_color_t         color)
{
  ਜੇ (unlikely (!paint->custom_palette))
  {
    paint->custom_palette = hb_map_create ();
    ਜੇ (unlikely (!paint->custom_palette))
      ਵਾਪਸ ਝੂਠ;
  }
  hb_map_ਸੈੱਟ (paint->custom_palette, color_ਸੂਚਕਾਂਕ, color);
  ਵਾਪਸ hb_map_ਵੰਡ_successful (paint->custom_palette);
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
hb_raster_paint_get_funcs (ਅਚਲ hb_raster_paint_t *paint HB_ਅਣਵਰਤਿਆ)
{
  ਵਾਪਸ static_raster_paint_funcs.get_unconst ();
}

ਸਥਿਰ hb_bool_t
hb_raster_paint_glyph_impl (hb_raster_paint_t *paint,
			    hb_font_t        *font,
			    hb_codepoint_t    glyph,
			    hb_bool_t         fallible)
{
  ਜੇ (!paint->has_extents)
  {
    hb_glyph_extents_t ge;
    ਜੇ (hb_font_get_glyph_extents (font, glyph, &ge))
      hb_raster_paint_ਸੈੱਟ_glyph_extents (paint, &ge);
  }

  hb_paint_funcs_t *funcs = hb_raster_paint_get_funcs (paint);
  hb_paint_ਧੱਕੋ_transform (funcs, paint,
			   paint->ਅਧਾਰ_transform.xx, paint->ਅਧਾਰ_transform.yx,
			   paint->ਅਧਾਰ_transform.xy, paint->ਅਧਾਰ_transform.yy,
			   paint->ਅਧਾਰ_transform.x0, paint->ਅਧਾਰ_transform.y0);

  hb_bool_t ret = ਸੱਚ;
  ਜੇ (fallible)
    ret = hb_font_paint_glyph_or_fail (font, glyph,
				       funcs, paint,
				       paint->palette, paint->foreground);
  ਨਹੀਂਤਾਂ
    hb_font_paint_glyph (font, glyph,
			 funcs, paint,
			 paint->palette, paint->foreground);

  hb_paint_ਕੱਢੋ_transform (funcs, paint);
  ਵਾਪਸ ret;
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
  ਵਾਪਸ hb_raster_paint_glyph_impl (paint, font, glyph, ਸੱਚ);
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
ਖਾਲੀ
hb_raster_paint_glyph (hb_raster_paint_t *paint,
		       hb_font_t        *font,
		       hb_codepoint_t    glyph)
{
  hb_raster_paint_glyph_impl (paint, font, glyph, ਝੂਠ);
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

  ਜੇ (unlikely (!paint->has_extents))
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

  hb_raster_image_t *ਨਤੀਜਾ;
  ਜੇ (paint->surface_ਢੇਰ.ਲੰਮਾਈ)
  {
    ਨਤੀਜਾ = paint->surface_ਢੇਰ[0];
    /* Release any remaining group surfaces (shouldn't happen with
     * well-formed paint calls, but be safe). */
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 1; i < paint->surface_ਢੇਰ.ਲੰਮਾਈ; i++)
      paint->release_surface (paint->surface_ਢੇਰ[i]);
    paint->surface_ਢੇਰ.clear ();
  }
  ਨਹੀਂਤਾਂ
  {
    ਨਤੀਜਾ = paint->acquire_surface ();
    ਜੇ (unlikely (!ਨਤੀਜਾ))
      ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
  }

  ਵਾਪਸ ਨਤੀਜਾ;
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
ਖਾਲੀ
hb_raster_paint_clear (hb_raster_paint_t *paint)
{
  paint->fixed_extents = {};
  paint->has_extents = ਝੂਠ;
  paint->transform_ਢੇਰ.clear ();
  paint->release_all_clips ();
  ਲਈ (ਸਵੈਚਾਲਿਤ *s : paint->surface_ਢੇਰ)
    paint->release_surface (s);
  paint->surface_ਢੇਰ.clear ();
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
ਖਾਲੀ
hb_raster_paint_reset (hb_raster_paint_t *paint)
{
  paint->ਅਧਾਰ_transform = {1, 0, 0, 1, 0, 0};
  paint->x_scale_ਗੁਣਕ = 1.f;
  paint->y_scale_ਗੁਣਕ = 1.f;
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
ਖਾਲੀ
hb_raster_paint_recycle_image (hb_raster_paint_t  *paint,
			       hb_raster_image_t  *image)
{
  paint->release_surface (image);
}
