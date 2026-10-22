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

#ಅನ್ತರ್ಭಾವ "hb.hh"

#ಅನ್ತರ್ಭಾವ "hb-raster-paint.hh"
#ಅನ್ತರ್ಭಾವ "hb-machinery.hh"
#ಅನ್ತರ್ಭಾವ "hb-paint.hh"

#ಅನ್ತರ್ಭಾವ <math.h>


/*
 * Pixel helpers (paint-specific)
 */

/* Convert unpremultiplied hb_color_t (BGRA order) to premultiplied BGRA32 pixel. */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ uint32_t
color_to_premul_pixel (hb_color_t color)
{
  uint8_t a = hb_color_get_alpha (color);
  uint8_t r = hb_raster_div255 (hb_color_get_red (color) * a);
  uint8_t g = hb_raster_div255 (hb_color_get_green (color) * a);
  uint8_t b = hb_raster_div255 (hb_color_get_blue (color) * a);
  ಪ್ರತ್ಯಾಗಮ (uint32_t) b | ((uint32_t) g << 8) | ((uint32_t) r << 16) | ((uint32_t) a << 24);
}

/* Bilinear sample from a premultiplied BGRA32 image. */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ uint32_t
hb_raster_sample_bilinear_premul (ಧ್ರುವ hb_packed_t<uint32_t> *src,
				  unsigned width,
				  unsigned height,
				  ಭಿನ್ನ x,
				  ಭಿನ್ನ y)
{
  ಪೂರ್ಣಾಙ್ಕ x0 = (ಪೂರ್ಣಾಙ್ಕ) floorf (x);
  ಪೂರ್ಣಾಙ್ಕ y0 = (ಪೂರ್ಣಾಙ್ಕ) floorf (y);
  ಪೂರ್ಣಾಙ್ಕ x1 = x0 + 1;
  ಪೂರ್ಣಾಙ್ಕ y1 = y0 + 1;
  ಯದಿ (x1 >= (ಪೂರ್ಣಾಙ್ಕ) width) x1 = (ಪೂರ್ಣಾಙ್ಕ) width - 1;
  ಯದಿ (y1 >= (ಪೂರ್ಣಾಙ್ಕ) height) y1 = (ಪೂರ್ಣಾಙ್ಕ) height - 1;

  ಭಿನ್ನ tx = x - x0;
  ಭಿನ್ನ ty = y - y0;
  ಭಿನ್ನ omtx = 1.f - tx;
  ಭಿನ್ನ omty = 1.f - ty;

  uint32_t p00 = (uint32_t) src[(size_t) y0 * width + (size_t) x0];
  uint32_t p10 = (uint32_t) src[(size_t) y0 * width + (size_t) x1];
  uint32_t p01 = (uint32_t) src[(size_t) y1 * width + (size_t) x0];
  uint32_t p11 = (uint32_t) src[(size_t) y1 * width + (size_t) x1];

  ಭಿನ್ನ w00 = omtx * omty;
  ಭಿನ್ನ w10 = tx * omty;
  ಭಿನ್ನ w01 = omtx * ty;
  ಭಿನ್ನ w11 = tx * ty;

  ಭಿನ್ನ b = ((p00 >>  0) & 0xff) * w00 + ((p10 >>  0) & 0xff) * w10 +
	    ((p01 >>  0) & 0xff) * w01 + ((p11 >>  0) & 0xff) * w11;
  ಭಿನ್ನ g = ((p00 >>  8) & 0xff) * w00 + ((p10 >>  8) & 0xff) * w10 +
	    ((p01 >>  8) & 0xff) * w01 + ((p11 >>  8) & 0xff) * w11;
  ಭಿನ್ನ r = ((p00 >> 16) & 0xff) * w00 + ((p10 >> 16) & 0xff) * w10 +
	    ((p01 >> 16) & 0xff) * w01 + ((p11 >> 16) & 0xff) * w11;
  ಭಿನ್ನ a = ((p00 >> 24) & 0xff) * w00 + ((p10 >> 24) & 0xff) * w10 +
	    ((p01 >> 24) & 0xff) * w01 + ((p11 >> 24) & 0xff) * w11;

  ಪ್ರತ್ಯಾಗಮ (uint32_t) (b + 0.5f)
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
ಸ್ಥಿರ ಶೂನ್ಯ
ensure_initialized (hb_raster_paint_t *c)
{
  ಯದಿ (c->surface_ರಾಶಿ.length) ಪ್ರತ್ಯಾಗಮ;

  /* A failed stack push (OOM) leaves the vector in a sticky error
   * state, so initialization can never succeed; bail out cheaply
   * instead of re-paying the surface acquire-and-clear below on
   * every subsequent paint callback. */
  ಯದಿ (unlikely (c->surface_ರಾಶಿ.in_error () || c->clip_ರಾಶಿ.in_error ()))
    ಪ್ರತ್ಯಾಗಮ;

  /* acquire_surface() clears a full surface; charge its area, so
   * repeated failed initialization attempts cannot re-pay it
   * indefinitely.  Successful initialization re-arms the session
   * budget below, making the charge a no-op for normal sessions. */
  ಯದಿ (unlikely (!c->charge_work ((int64_t) c->fixed_extents.width *
				 c->fixed_extents.height)))
    ಪ್ರತ್ಯಾಗಮ;

  /* Root surface */
  hb_raster_image_t *ಬೇರು = c->acquire_surface ();
  ಯದಿ (unlikely (!ಬೇರು)) ಪ್ರತ್ಯಾಗಮ;

  ಯದಿ (hb_color_get_alpha (c->background))
  {
    uint32_t bg = color_to_premul_pixel (c->background);
    hb_raster_extents_t ext;
    hb_raster_image_get_extents (ಬೇರು, &ext);
    ಧ್ರುವ uint8_t *buf = hb_raster_image_get_buffer (ಬೇರು);
    ಪ್ರತಿ (unsigned y = 0; y < ext.height; y++)
    {
      hb_packed_t<uint32_t> *row =
	(hb_packed_t<uint32_t> *) (buf + (size_t) y * ext.stride);
      ಪ್ರತಿ (unsigned x = 0; x < ext.width; x++)
	row[x] = hb_packed_t<uint32_t> (bg);
    }
  }

  ಯದಿ (unlikely (!c->surface_ರಾಶಿ.ತಳ್ಳು_or_fail (ಬೇರು)))
  {
    c->release_surface (ಬೇರು);
    ಪ್ರತ್ಯಾಗಮ;
  }

  /* Resolve the surface-sized pixel budget now that the surface size is
   * known.  An unlimited policy disables the pixel budget, so leave it.
   * The outline budget is re-resolved to the same value it already holds
   * (nothing has been drawn yet). */
  ಯದಿ (c->budget != HB_BUDGET_UNLIMITED)
    c->recharge_budget ();

  /* Initial clip: full coverage rectangle */
  hb_raster_clip_t clip;
  clip.init_full (c->fixed_extents.width, c->fixed_extents.height);
  ಯದಿ (unlikely (!c->clip_ರಾಶಿ.ತಳ್ಳು_or_fail (std::move (clip))))
  {
    c->release_surface (c->surface_ರಾಶಿ.ತೆಗೆ ());
    ಪ್ರತ್ಯಾಗಮ;
  }
}

ಸ್ಥಿರ ಶೂನ್ಯ
hb_raster_paint_ತಳ್ಳು_transform (hb_paint_funcs_t *pfuncs HB_UNUSED,
				ಶೂನ್ಯ *paint_data,
				ಭಿನ್ನ xx, ಭಿನ್ನ yx,
				ಭಿನ್ನ xy, ಭಿನ್ನ yy,
				ಭಿನ್ನ dx, ಭಿನ್ನ dy,
				ಶೂನ್ಯ *user_data HB_UNUSED)
{
  hb_raster_paint_t *c = (hb_raster_paint_t *) paint_data;

  ensure_initialized (c);

  hb_transform_t<> t = c->current_transform ();
  t.multiply ({xx, yx, xy, yy, dx, dy});
  (ಶೂನ್ಯ) c->transform_ರಾಶಿ.push (t);
}

ಸ್ಥಿರ ಶೂನ್ಯ
hb_raster_paint_ತೆಗೆ_transform (hb_paint_funcs_t *pfuncs HB_UNUSED,
			       ಶೂನ್ಯ *paint_data,
			       ಶೂನ್ಯ *user_data HB_UNUSED)
{
  hb_raster_paint_t *c = (hb_raster_paint_t *) paint_data;
  ಯದಿ (c->transform_ರಾಶಿ.length)
    c->transform_ರಾಶಿ.ತೆಗೆ ();
}

ಸ್ಥಿರ hb_bool_t
hb_raster_paint_color_glyph (hb_paint_funcs_t *pfuncs HB_UNUSED,
			     ಶೂನ್ಯ *paint_data HB_UNUSED,
			     hb_codepoint_t glyph HB_UNUSED,
			     hb_font_t *font HB_UNUSED,
			     ಶೂನ್ಯ *user_data HB_UNUSED)
{
  ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
}

ಪ್ರಕಾರನಿರ್ದೇಶ ಶೂನ್ಯ (*hb_raster_paint_clip_mask_emit_t) (hb_raster_draw_t *rdr, ಶೂನ್ಯ *user_data);

/* Attach the surface box to @rdr and seed its outline budget before
 * outlines are drawn into it: curves outside the surface collapse to
 * their chord, and outline traversal and curve flattening charge the
 * seeded budget.  hb_raster_paint_readback_budget() pulls the remainder
 * back into the session afterwards. */
ಸ್ಥಿರ ಶೂನ್ಯ
hb_raster_paint_attach_clip_rdr (hb_raster_paint_t *c,
				 hb_raster_draw_t *rdr,
				 ಧ್ರುವ hb_raster_image_t *surf)
{
  hb_raster_draw_set_clip_box (rdr,
			       (ಭಿನ್ನ) surf->extents.x_origin,
			       (ಭಿನ್ನ) surf->extents.y_origin,
			       (ಭಿನ್ನ) surf->extents.x_origin + (ಭಿನ್ನ) surf->extents.width,
			       (ಭಿನ್ನ) surf->extents.y_origin + (ಭಿನ್ನ) surf->extents.height);
  hb_draw_set_budget (hb_raster_draw_get_funcs (rdr), rdr, c->budget_remaining);
}

/* Pull the outline budget the rasterizer has consumed back into the
 * session, so it aggregates across every glyph in the paint walk. */
ಸ್ಥಿರ ಶೂನ್ಯ
hb_raster_paint_readback_budget (hb_raster_paint_t *c,
				 hb_raster_draw_t *rdr)
{
  c->budget_remaining = hb_draw_get_budget_remaining (hb_raster_draw_get_funcs (rdr), rdr);
}

ಸ್ಥಿರ ಶೂನ್ಯ
hb_raster_paint_ತಳ್ಳು_empty_clip (hb_raster_paint_t *c, unsigned w, unsigned h)
{
  hb_raster_clip_t new_clip = c->acquire_clip (w, h);
  new_clip.init_full (w, h);
  new_clip.is_rect = ಸತ್ಯ;
  new_clip.rect_x0 = new_clip.rect_y0 = 0;
  new_clip.rect_x1 = new_clip.rect_y1 = 0;
  new_clip.min_x = new_clip.min_y = new_clip.max_x = new_clip.max_y = 0;
  (ಶೂನ್ಯ) c->clip_ರಾಶಿ.push (std::move (new_clip));
}

ಸ್ಥಿರ ತಾರ್ಕಿಕ
hb_raster_paint_intersect_mask (ಧ್ರುವ hb_raster_image_t *surf,
				ಧ್ರುವ hb_raster_extents_t &mask_ext,
				ಧ್ರುವ hb_raster_clip_t &clip,
				unsigned w, unsigned h,
				int64_t *mask_x0, int64_t *mask_y0,
				unsigned *ix0, unsigned *iy0,
				unsigned *ix1, unsigned *iy1)
{
  /* Convert mask extents from surface coordinates to clip-buffer coordinates. */
  int64_t mx0 = (int64_t) mask_ext.x_origin - (int64_t) surf->extents.x_origin;
  int64_t my0 = (int64_t) mask_ext.y_origin - (int64_t) surf->extents.y_origin;

  int64_t x0 = hb_max ((int64_t) clip.min_x, hb_max (mx0, (int64_t) 0));
  int64_t y0 = hb_max ((int64_t) clip.min_y, hb_max (my0, (int64_t) 0));
  int64_t x1 = hb_min ((int64_t) clip.max_x, hb_min (mx0 + mask_ext.width, (int64_t) w));
  int64_t y1 = hb_min ((int64_t) clip.max_y, hb_min (my0 + mask_ext.height, (int64_t) h));

  ಯದಿ (x0 >= x1 || y0 >= y1) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

  *mask_x0 = mx0;
  *mask_y0 = my0;
  *ix0 = (unsigned) x0;
  *iy0 = (unsigned) y0;
  *ix1 = (unsigned) x1;
  *iy1 = (unsigned) y1;
  ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
}

/* Render whatever edges have been accumulated into @rdr and
 * push the result as a new clip on the stack, intersecting
 * with the existing clip.  Used by the emitter-based clip
 * helpers (push_clip_glyph) and by push_clip_path_end once
 * the caller has drawn the path into @rdr. */
ಸ್ಥಿರ ಶೂನ್ಯ
hb_raster_paint_finalize_path_clip (hb_raster_paint_t *c,
				    hb_raster_draw_t *rdr,
				    hb_raster_image_t *surf,
				    unsigned w, unsigned h)
{
  ಯದಿ (unlikely (!c->precharge_work (hb_raster_draw_get_pixel_work (rdr, h, w))))
  {
    hb_raster_draw_clear (rdr);
    hb_raster_paint_ತಳ್ಳು_empty_clip (c, w, h);
    ಪ್ರತ್ಯಾಗಮ;
  }

  hb_raster_image_t *mask_img = hb_raster_draw_render (rdr);

  ಯದಿ (unlikely (!mask_img))
  {
    hb_raster_paint_ತಳ್ಳು_empty_clip (c, w, h);
    ಪ್ರತ್ಯಾಗಮ;
  }

  /* Charge the mask render (its own area; may exceed the surface) plus
   * the clip-buffer pass below. */
  hb_raster_extents_t mask_render_ext;
  hb_raster_image_get_extents (mask_img, &mask_render_ext);
  ಯದಿ (unlikely (!c->charge_work ((int64_t) mask_render_ext.width * mask_render_ext.height +
				 (int64_t) w * h)))
  {
    hb_raster_draw_recycle_image (rdr, mask_img);
    hb_raster_paint_ತಳ್ಳು_empty_clip (c, w, h);
    ಪ್ರತ್ಯಾಗಮ;
  }

  hb_raster_clip_t new_clip = c->acquire_clip (w, h);

  /* Allocate alpha buffer and intersect with previous clip */
  size_t clip_size = (size_t) new_clip.stride * h;
  ಯದಿ (unlikely (clip_size > HB_RASTER_MAX_BUFFER_SIZE ||
                !new_clip.alpha.resize ((unsigned) clip_size)))
  {
    hb_raster_draw_recycle_image (rdr, mask_img);
    hb_raster_paint_ತಳ್ಳು_empty_clip (c, w, h);
    ಪ್ರತ್ಯಾಗಮ;
  }
  hb_memset (new_clip.alpha.arrayZ, 0, (unsigned) clip_size);

  ಧ್ರುವ uint8_t *mask_buf = hb_raster_image_get_buffer (mask_img);
  hb_raster_extents_t mask_ext;
  hb_raster_image_get_extents (mask_img, &mask_ext);
  ಧ್ರುವ hb_raster_clip_t &old_clip = c->current_clip ();
  ಯದಿ (unlikely (!old_clip.has_valid_alpha_mask ()))
  {
    hb_raster_draw_recycle_image (rdr, mask_img);
    hb_raster_paint_ತಳ್ಳು_empty_clip (c, w, h);
    ಪ್ರತ್ಯಾಗಮ;
  }

  int64_t mask_x0, mask_y0;
  unsigned ix0, iy0, ix1, iy1;
  ಯದಿ (!hb_raster_paint_intersect_mask (surf, mask_ext, old_clip, w, h,
				       &mask_x0, &mask_y0,
				       &ix0, &iy0, &ix1, &iy1))
  {
    hb_raster_draw_recycle_image (rdr, mask_img);
    hb_raster_paint_ತಳ್ಳು_empty_clip (c, w, h);
    ಪ್ರತ್ಯಾಗಮ;
  }

  new_clip.min_x = w; new_clip.min_y = h;
  new_clip.max_x = 0; new_clip.max_y = 0;

  ಯದಿ (old_clip.is_rect)
  {
    ಪ್ರತಿ (unsigned y = iy0; y < iy1; y++)
    {
      ಧ್ರುವ uint8_t *mask_row = mask_buf + (unsigned) ((int64_t) y - mask_y0) * mask_ext.stride;
      uint8_t *out_row = new_clip.alpha.arrayZ + y * new_clip.stride;
      unsigned row_min = ix1;
      unsigned row_max = ix0;
      unsigned mx = (unsigned) ((int64_t) ix0 - mask_x0);
      ಪ್ರತಿ (unsigned x = ix0; x < ix1; x++)
      {
	uint8_t a = mask_row[mx++];
	out_row[x] = a;
	ಯದಿ (a && row_min == ix1)
	{
	  row_min = x;
	  row_max = x + 1;
	}
	ಅನ್ಯಥಾ ಯದಿ (a)
	  row_max = x + 1;
      }
      ಯದಿ (row_min < row_max)
      {
	new_clip.min_x = hb_min (new_clip.min_x, row_min);
	new_clip.min_y = hb_min (new_clip.min_y, y);
	new_clip.max_x = hb_max (new_clip.max_x, row_max);
	new_clip.max_y = hb_max (new_clip.max_y, y + 1);
      }
    }
  }
  ಅನ್ಯಥಾ
  {
    ಪ್ರತಿ (unsigned y = iy0; y < iy1; y++)
    {
      ಧ್ರುವ uint8_t *old_row = old_clip.alpha.arrayZ + y * old_clip.stride;
      ಧ್ರುವ uint8_t *mask_row = mask_buf + (unsigned) ((int64_t) y - mask_y0) * mask_ext.stride;
      uint8_t *out_row = new_clip.alpha.arrayZ + y * new_clip.stride;
      unsigned row_min = ix1;
      unsigned row_max = ix0;
      ಪ್ರತಿ (unsigned x = ix0; x < ix1; x++)
      {
	unsigned mx = (unsigned) ((int64_t) x - mask_x0);
	uint8_t a = hb_raster_div255 (mask_row[mx] * old_row[x]);
	out_row[x] = a;
	ಯದಿ (a)
	{
	  row_min = hb_min (row_min, x);
	  row_max = x + 1;
	}
      }
      ಯದಿ (row_min < row_max)
      {
	new_clip.min_x = hb_min (new_clip.min_x, row_min);
	new_clip.min_y = hb_min (new_clip.min_y, y);
	new_clip.max_x = hb_max (new_clip.max_x, row_max);
	new_clip.max_y = hb_max (new_clip.max_y, y + 1);
      }
    }
  }

  hb_raster_draw_recycle_image (rdr, mask_img);
  ಯದಿ (unlikely (!c->clip_ರಾಶಿ.ತಳ್ಳು_or_fail (std::move (new_clip))))
    hb_raster_paint_ತಳ್ಳು_empty_clip (c, w, h);
}

ಸ್ಥಿರ ಶೂನ್ಯ
hb_raster_paint_ತಳ್ಳು_clip_from_emitter (hb_raster_paint_t *c,
					hb_raster_paint_clip_mask_emit_t emit,
					ಶೂನ್ಯ *emit_data)
{
  ensure_initialized (c);

  hb_raster_image_t *surf = c->current_surface ();
  ಯದಿ (unlikely (!surf)) ಪ್ರತ್ಯಾಗಮ;

  unsigned w = surf->extents.width;
  unsigned h = surf->extents.height;

  /* Out of budget: skip the glyph-outline extraction entirely, so
   * per-glyph outline limits cannot multiply with the caller's
   * paint-graph traversal limits. */
  ಯದಿ (unlikely (c->budget_remaining < 0))
  {
    hb_raster_paint_ತಳ್ಳು_empty_clip (c, w, h);
    ಪ್ರತ್ಯಾಗಮ;
  }

  hb_raster_draw_t *rdr = c->clip_rdr;
  hb_transform_t<> t = c->current_effective_transform ();
  hb_raster_draw_set_transform (rdr, t.xx, t.yx, t.xy, t.yy, t.x0, t.y0);
  hb_raster_paint_attach_clip_rdr (c, rdr, surf);
  emit (rdr, emit_data);
  hb_raster_paint_readback_budget (c, rdr);

  hb_raster_paint_finalize_path_clip (c, rdr, surf, w, h);
}

ಸಂರಚನಾ hb_raster_paint_glyph_clip_data_t
{
  hb_codepoint_t glyph;
  hb_font_t *font;
};

ಸ್ಥಿರ ಶೂನ್ಯ
hb_raster_paint_emit_clip_glyph_mask (hb_raster_draw_t *rdr, ಶೂನ್ಯ *user_data)
{
  hb_raster_paint_glyph_clip_data_t *data = (hb_raster_paint_glyph_clip_data_t *) user_data;
  /* Let draw-render choose tight glyph extents; we map by mask origin below. */
  hb_font_draw_glyph (data->font, data->glyph, hb_raster_draw_get_funcs (rdr), rdr);
}

ಸ್ಥಿರ ಶೂನ್ಯ
hb_raster_paint_ತಳ್ಳು_clip_glyph (hb_paint_funcs_t *pfuncs HB_UNUSED,
				 ಶೂನ್ಯ *paint_data,
				 hb_codepoint_t glyph,
				 hb_font_t *font,
				 ಶೂನ್ಯ *user_data HB_UNUSED)
{
  hb_raster_paint_t *c = (hb_raster_paint_t *) paint_data;
  hb_raster_paint_glyph_clip_data_t data = {glyph, font};
  hb_raster_paint_ತಳ್ಳು_clip_from_emitter (c, hb_raster_paint_emit_clip_glyph_mask, &data);
}

ಸ್ಥಿರ ಶೂನ್ಯ
hb_raster_paint_ತಳ್ಳು_clip_rectangle (hb_paint_funcs_t *pfuncs HB_UNUSED,
				     ಶೂನ್ಯ *paint_data,
				     ಭಿನ್ನ xmin, ಭಿನ್ನ ymin,
				     ಭಿನ್ನ xmax, ಭಿನ್ನ ymax,
				     ಶೂನ್ಯ *user_data HB_UNUSED)
{
  hb_raster_paint_t *c = (hb_raster_paint_t *) paint_data;

  ensure_initialized (c);

  ಯದಿ (!c->surface_ರಾಶಿ.length) ಪ್ರತ್ಯಾಗಮ;

  hb_transform_t<> t = c->current_effective_transform ();

  hb_raster_image_t *surf = c->current_surface ();
  ಯದಿ (unlikely (!surf)) ಪ್ರತ್ಯಾಗಮ;

  unsigned w = surf->extents.width;
  unsigned h = surf->extents.height;
  ತಾರ್ಕಿಕ is_axis_aligned = (t.xy == 0.f && t.yx == 0.f);

  /* Transform the four corners to pixel space */
  ಭಿನ್ನ cx[4], cy[4];
  cx[0] = xmin; cy[0] = ymin;
  cx[1] = xmax; cy[1] = ymin;
  cx[2] = xmax; cy[2] = ymax;
  cx[3] = xmin; cy[3] = ymax;
  ಪ್ರತಿ (unsigned i = 0; i < 4; i++)
    t.transform_point (cx[i], cy[i]);

  /* Compute bounding box in pixel coords */
  ಭಿನ್ನ fmin_x = cx[0], fmin_y = cy[0], fmax_x = cx[0], fmax_y = cy[0];
  ಪ್ರತಿ (unsigned i = 1; i < 4; i++)
  {
    fmin_x = hb_min (fmin_x, cx[i]); fmin_y = hb_min (fmin_y, cy[i]);
    fmax_x = hb_max (fmax_x, cx[i]); fmax_y = hb_max (fmax_y, cy[i]);
  }

  /* A spec-legal but very large transform can push these bounds past the int
     range or to non-finite values; a bare float->int conversion would then be
     undefined.  Saturate through hb_clamp_to<> (which also maps NaN to
     INT32_MIN); the subtraction is done in double to avoid overflow. */
  ಪೂರ್ಣಾಙ್ಕ px0 = hb_clamp_to<int32_t> ((ದ್ವಿಗುಣ) floorf (fmin_x) - surf->extents.x_origin);
  ಪೂರ್ಣಾಙ್ಕ py0 = hb_clamp_to<int32_t> ((ದ್ವಿಗುಣ) floorf (fmin_y) - surf->extents.y_origin);
  ಪೂರ್ಣಾಙ್ಕ px1 = hb_clamp_to<int32_t> ((ದ್ವಿಗುಣ) ceilf  (fmax_x) - surf->extents.x_origin);
  ಪೂರ್ಣಾಙ್ಕ py1 = hb_clamp_to<int32_t> ((ದ್ವಿಗುಣ) ceilf  (fmax_y) - surf->extents.y_origin);

  /* Clamp to surface bounds */
  px0 = hb_max (px0, 0);
  py0 = hb_max (py0, 0);
  px1 = hb_min (px1, (ಪೂರ್ಣಾಙ್ಕ) w);
  py1 = hb_min (py1, (ಪೂರ್ಣಾಙ್ಕ) h);

  ಧ್ರುವ hb_raster_clip_t &old_clip = c->current_clip ();
  ಯದಿ (unlikely (!old_clip.has_valid_alpha_mask ()))
  {
    hb_raster_paint_ತಳ್ಳು_empty_clip (c, w, h);
    ಪ್ರತ್ಯಾಗಮ;
  }

  /* All paths below except rect-on-rect run a full clip-buffer pass. */
  ಯದಿ (!(is_axis_aligned && old_clip.is_rect) &&
      unlikely (!c->charge_work ((int64_t) w * h)))
  {
    hb_raster_paint_ತಳ್ಳು_empty_clip (c, w, h);
    ಪ್ರತ್ಯಾಗಮ;
  }

  hb_raster_clip_t new_clip = c->acquire_clip (w, h);

  ಯದಿ (is_axis_aligned && old_clip.is_rect)
  {
    /* Fast path: axis-aligned rect-on-rect intersection */
    new_clip.is_rect = ಸತ್ಯ;
    new_clip.rect_x0 = hb_max (px0, old_clip.rect_x0);
    new_clip.rect_y0 = hb_max (py0, old_clip.rect_y0);
    new_clip.rect_x1 = hb_min (px1, old_clip.rect_x1);
    new_clip.rect_y1 = hb_min (py1, old_clip.rect_y1);
    new_clip.update_bounds_from_rect ();
  }
  ಅನ್ಯಥಾ ಯದಿ (is_axis_aligned)
  {
    /* Fast path: intersect an axis-aligned rect with an existing alpha clip. */
    new_clip.is_rect = ಅಸತ್ಯ;
    size_t clip_size = (size_t) new_clip.stride * h;
    ಯದಿ (unlikely (clip_size > HB_RASTER_MAX_BUFFER_SIZE ||
                  !new_clip.alpha.resize ((unsigned) clip_size)))
    {
      hb_raster_paint_ತಳ್ಳು_empty_clip (c, w, h);
      ಪ್ರತ್ಯಾಗಮ;
    }
    hb_memset (new_clip.alpha.arrayZ, 0, (unsigned) clip_size);

    ಪೂರ್ಣಾಙ್ಕ iy0_i = hb_max (py0, (ಪೂರ್ಣಾಙ್ಕ) old_clip.min_y);
    ಪೂರ್ಣಾಙ್ಕ iy1_i = hb_min (py1, (ಪೂರ್ಣಾಙ್ಕ) old_clip.max_y);
    ಪೂರ್ಣಾಙ್ಕ ix0_i = hb_max (px0, (ಪೂರ್ಣಾಙ್ಕ) old_clip.min_x);
    ಪೂರ್ಣಾಙ್ಕ ix1_i = hb_min (px1, (ಪೂರ್ಣಾಙ್ಕ) old_clip.max_x);

    ಯದಿ (ix0_i < ix1_i && iy0_i < iy1_i)
    {
      unsigned iy0 = (unsigned) iy0_i;
      unsigned iy1 = (unsigned) iy1_i;
      unsigned ix0 = (unsigned) ix0_i;
      unsigned ix1 = (unsigned) ix1_i;

      ಪ್ರತಿ (unsigned y = iy0; y < iy1; y++)
      {
        ಧ್ರುವ uint8_t *old_row = old_clip.alpha.arrayZ + y * old_clip.stride;
        uint8_t *new_row = new_clip.alpha.arrayZ + y * new_clip.stride;
        hb_memcpy (new_row + ix0, old_row + ix0, ix1 - ix0);
      }
      new_clip.min_x = ix0;
      new_clip.min_y = iy0;
      new_clip.max_x = ix1;
      new_clip.max_y = iy1;
    }
  }
  ಅನ್ಯಥಾ
  {
    /* General case: rasterize transformed quad as alpha mask */
    new_clip.is_rect = ಅಸತ್ಯ;
    size_t clip_size = (size_t) new_clip.stride * h;
    ಯದಿ (unlikely (clip_size > HB_RASTER_MAX_BUFFER_SIZE ||
                  !new_clip.alpha.resize ((unsigned) clip_size)))
    {
      hb_raster_paint_ತಳ್ಳು_empty_clip (c, w, h);
      ಪ್ರತ್ಯಾಗಮ;
    }
    hb_memset (new_clip.alpha.arrayZ, 0, (unsigned) clip_size);

    /* Convert quad corners to pixel-relative coords */
    ಭಿನ್ನ qx[4], qy[4];
    ಪೂರ್ಣಾಙ್ಕ ox = surf->extents.x_origin;
    ಪೂರ್ಣಾಙ್ಕ oy = surf->extents.y_origin;
    ಪ್ರತಿ (unsigned i = 0; i < 4; i++)
    {
      qx[i] = cx[i] - ox;
      qy[i] = cy[i] - oy;
    }

    /* For each pixel in the bounding box, test if inside the quad
     * using cross-product edge tests (winding order). */
    ಪೂರ್ಣಾಙ್ಕ iy0_i = hb_max (py0, (ಪೂರ್ಣಾಙ್ಕ) old_clip.min_y);
    ಪೂರ್ಣಾಙ್ಕ iy1_i = hb_min (py1, (ಪೂರ್ಣಾಙ್ಕ) old_clip.max_y);
    ಪೂರ್ಣಾಙ್ಕ ix0_i = hb_max (px0, (ಪೂರ್ಣಾಙ್ಕ) old_clip.min_x);
    ಪೂರ್ಣಾಙ್ಕ ix1_i = hb_min (px1, (ಪೂರ್ಣಾಙ್ಕ) old_clip.max_x);
    new_clip.min_x = w; new_clip.min_y = h;
    new_clip.max_x = 0; new_clip.max_y = 0;

    ಯದಿ (ix0_i < ix1_i && iy0_i < iy1_i)
    {
      unsigned iy0 = (unsigned) iy0_i;
      unsigned iy1 = (unsigned) iy1_i;
      unsigned ix0 = (unsigned) ix0_i;
      unsigned ix1 = (unsigned) ix1_i;

      /* Precompute edge normals for point-in-quad test.
       * Edge i goes from corner i to corner (i+1)%4.
       * Normal = (dy, -dx); inside test: dot(normal, p-corner) >= 0 */
	      ಭಿನ್ನ enx[4], eny[4], ed[4];
	      ಪ್ರತಿ (unsigned i = 0; i < 4; i++)
	      {
		unsigned j = (i + 1) & 3;
		ಭಿನ್ನ edx = qx[j] - qx[i], edy = qy[j] - qy[i];
		enx[i] = edy;       /* normal x */
		eny[i] = -edx;      /* normal y */
		ed[i] = enx[i] * qx[i] + eny[i] * qy[i]; /* distance threshold */
	      }
	      ಭಿನ್ನ area2 = 0.f;
	      ಪ್ರತಿ (unsigned i = 0; i < 4; i++)
	      {
		unsigned j = (i + 1) & 3;
		area2 += qx[i] * qy[j] - qx[j] * qy[i];
	      }
	      ತಾರ್ಕಿಕ ccw = area2 >= 0.f;

      ಯದಿ (old_clip.is_rect)
      {
	ಪ್ರತಿ (unsigned y = iy0; y < iy1; y++)
	  ಪ್ರತಿ (unsigned x = ix0; x < ix1; x++)
	  {
	    ಭಿನ್ನ px_f = x + 0.5f, py_f = y + 0.5f;
	    /* Test if pixel center is inside the quad */
		    ತಾರ್ಕಿಕ inside = ಸತ್ಯ;
		    ಪ್ರತಿ (unsigned i = 0; i < 4; i++)
		    {
		      ಭಿನ್ನ d = enx[i] * px_f + eny[i] * py_f;
		      ಯದಿ (ccw ? d < ed[i] : d > ed[i])
		      {
			inside = ಅಸತ್ಯ;
			ಭಙ್ಗ;
		      }
		    }
	    uint8_t a = inside ? 255 : 0;
	    new_clip.alpha[y * new_clip.stride + x] = a;
	    ಯದಿ (a)
	    {
	      new_clip.min_x = hb_min (new_clip.min_x, x);
	      new_clip.min_y = hb_min (new_clip.min_y, y);
	      new_clip.max_x = hb_max (new_clip.max_x, x + 1);
	      new_clip.max_y = hb_max (new_clip.max_y, y + 1);
	    }
	  }
      }
      ಅನ್ಯಥಾ
      {
	ಪ್ರತಿ (unsigned y = iy0; y < iy1; y++)
	{
	  ಧ್ರುವ uint8_t *old_row = old_clip.alpha.arrayZ + y * old_clip.stride;
	  ಪ್ರತಿ (unsigned x = ix0; x < ix1; x++)
	  {
	    ಭಿನ್ನ px_f = x + 0.5f, py_f = y + 0.5f;
	    /* Test if pixel center is inside the quad */
		    ತಾರ್ಕಿಕ inside = ಸತ್ಯ;
		    ಪ್ರತಿ (unsigned i = 0; i < 4; i++)
		    {
		      ಭಿನ್ನ d = enx[i] * px_f + eny[i] * py_f;
		      ಯದಿ (ccw ? d < ed[i] : d > ed[i])
		      {
			inside = ಅಸತ್ಯ;
			ಭಙ್ಗ;
		      }
		    }
	    uint8_t a = inside ? old_row[x] : 0;
	    new_clip.alpha[y * new_clip.stride + x] = a;
	    ಯದಿ (a)
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

  ಯದಿ (unlikely (!c->clip_ರಾಶಿ.ತಳ್ಳು_or_fail (std::move (new_clip))))
    hb_raster_paint_ತಳ್ಳು_empty_clip (c, surf->extents.width, surf->extents.height);
}

ಸ್ಥಿರ hb_draw_funcs_t *
hb_raster_paint_ತಳ್ಳು_clip_path_start (hb_paint_funcs_t *pfuncs HB_UNUSED,
				      ಶೂನ್ಯ *paint_data,
				      ಶೂನ್ಯ **draw_data,
				      ಶೂನ್ಯ *user_data HB_UNUSED)
{
  hb_raster_paint_t *c = (hb_raster_paint_t *) paint_data;

  ensure_initialized (c);

  /* Prime clip_rdr with the current effective transform; the
   * caller then drives hb_draw_*() into it, and _end renders
   * the accumulated edges into a mask and intersects with the
   * current clip. */
  ಯದಿ (unlikely (!c->surface_ರಾಶಿ.length || !c->clip_rdr))
  {
    *draw_data = nullptr;
    ಪ್ರತ್ಯಾಗಮ nullptr;
  }

  hb_raster_draw_t *rdr = c->clip_rdr;
  hb_transform_t<> t = c->current_effective_transform ();
  hb_raster_draw_set_transform (rdr, t.xx, t.yx, t.xy, t.yy, t.x0, t.y0);
  hb_raster_image_t *surf = c->current_surface ();
  ಯದಿ (likely (surf))
    hb_raster_paint_attach_clip_rdr (c, rdr, surf);

  *draw_data = rdr;
  ಪ್ರತ್ಯಾಗಮ hb_raster_draw_get_funcs (rdr);
}

ಸ್ಥಿರ ಶೂನ್ಯ
hb_raster_paint_ತಳ್ಳು_clip_path_end (hb_paint_funcs_t *pfuncs HB_UNUSED,
				    ಶೂನ್ಯ *paint_data,
				    ಶೂನ್ಯ *user_data HB_UNUSED)
{
  hb_raster_paint_t *c = (hb_raster_paint_t *) paint_data;

  hb_raster_image_t *surf = c->current_surface ();
  ಯದಿ (unlikely (!surf || !c->clip_rdr)) ಪ್ರತ್ಯಾಗಮ;

  unsigned w = surf->extents.width;
  unsigned h = surf->extents.height;

  hb_raster_paint_readback_budget (c, c->clip_rdr);
  ಯದಿ (unlikely (c->budget_remaining < 0))
  {
    hb_raster_draw_clear (c->clip_rdr);
    hb_raster_paint_ತಳ್ಳು_empty_clip (c, w, h);
    ಪ್ರತ್ಯಾಗಮ;
  }

  hb_raster_paint_finalize_path_clip (c, c->clip_rdr, surf, w, h);
}

ಸ್ಥಿರ ಶೂನ್ಯ
hb_raster_paint_ತೆಗೆ_clip (hb_paint_funcs_t *pfuncs HB_UNUSED,
			  ಶೂನ್ಯ *paint_data,
			  ಶೂನ್ಯ *user_data HB_UNUSED)
{
  hb_raster_paint_t *c = (hb_raster_paint_t *) paint_data;
  ಯದಿ (!c->clip_ರಾಶಿ.length) ಪ್ರತ್ಯಾಗಮ;
  c->release_clip (c->clip_ರಾಶಿ.ತೆಗೆ ());
}

ಸ್ಥಿರ ಶೂನ್ಯ
hb_raster_paint_ತಳ್ಳು_group (hb_paint_funcs_t *pfuncs HB_UNUSED,
			    ಶೂನ್ಯ *paint_data,
			    ಶೂನ್ಯ *user_data HB_UNUSED)
{
  hb_raster_paint_t *c = (hb_raster_paint_t *) paint_data;

  ensure_initialized (c);

  /* acquire_surface() clears a full surface; charge its area. */
  ಯದಿ (unlikely (!c->charge_work ((int64_t) c->fixed_extents.width * c->fixed_extents.height)))
    ಪ್ರತ್ಯಾಗಮ;

  hb_raster_image_t *new_surf = c->acquire_surface ();
  ಯದಿ (unlikely (!new_surf)) ಪ್ರತ್ಯಾಗಮ;
  ಯದಿ (unlikely (!c->surface_ರಾಶಿ.ತಳ್ಳು_or_fail (new_surf)))
    c->release_surface (new_surf);
}

ಸ್ಥಿರ ಶೂನ್ಯ
hb_raster_paint_ತೆಗೆ_group (hb_paint_funcs_t *pfuncs HB_UNUSED,
			   ಶೂನ್ಯ *paint_data,
			   hb_paint_composite_mode_t mode,
			   ಶೂನ್ಯ *user_data HB_UNUSED)
{
  hb_raster_paint_t *c = (hb_raster_paint_t *) paint_data;

  ಯದಿ (c->surface_ರಾಶಿ.length < 2) ಪ್ರತ್ಯಾಗಮ;

  hb_raster_image_t *src = c->surface_ರಾಶಿ.ತೆಗೆ ();
  hb_raster_image_t *dst = c->current_surface ();

  ಯದಿ (dst && src &&
      likely (c->charge_work ((int64_t) dst->extents.width * dst->extents.height)))
    hb_raster_image_composite (dst, src, mode);

  c->release_surface (src);
}

ಸ್ಥಿರ ಶೂನ್ಯ
hb_raster_paint_solid (hb_raster_paint_t *c,
		       hb_raster_image_t *surf,
		       hb_color_t color,
		       hb_raster_image_t *mask_img)
{
  uint32_t premul = color_to_premul_pixel (color);
  uint8_t premul_a = (uint8_t) (premul >> 24);
  ಧ್ರುವ hb_raster_clip_t &clip = c->current_clip ();

  unsigned stride = surf->extents.stride;
  ಯದಿ (clip.min_x >= clip.max_x || clip.min_y >= clip.max_y) ಪ್ರತ್ಯಾಗಮ;
  ಯದಿ (premul_a == 0) ಪ್ರತ್ಯಾಗಮ;

  unsigned ix0 = clip.min_x, iy0 = clip.min_y;
  unsigned ix1 = clip.max_x, iy1 = clip.max_y;
  ಧ್ರುವ uint8_t *mask_buf = nullptr;
  hb_raster_extents_t mask_ext;
  int64_t mask_x0 = 0, mask_y0 = 0;
  ಯದಿ (mask_img)
  {
    mask_buf = hb_raster_image_get_buffer (mask_img);
    hb_raster_image_get_extents (mask_img, &mask_ext);
    ಯದಿ (!hb_raster_paint_intersect_mask (surf, mask_ext, clip,
					 surf->extents.width, surf->extents.height,
					 &mask_x0, &mask_y0,
					 &ix0, &iy0, &ix1, &iy1))
      ಪ್ರತ್ಯಾಗಮ;
  }

  ಯದಿ (unlikely (!c->charge_work ((int64_t) (ix1 - ix0) * (iy1 - iy0))))
    ಪ್ರತ್ಯಾಗಮ;

  ಯದಿ (likely (!clip.is_rect))
  {
    ಪ್ರತಿ (unsigned y = iy0; y < iy1; y++)
    {
      hb_packed_t<uint32_t> *__restrict row = (hb_packed_t<uint32_t> *) (surf->buffer.arrayZ + y * stride);
      ಧ್ರುವ uint8_t *__restrict clip_row = clip.alpha.arrayZ + y * clip.stride;
      ಧ್ರುವ uint8_t *__restrict mask_row = mask_buf ? mask_buf + (unsigned) ((int64_t) y - mask_y0) * mask_ext.stride
						    : nullptr;
      unsigned mx = (unsigned) ((int64_t) ix0 - mask_x0);
      ಪ್ರತಿ (unsigned x = ix0; x < ix1; x++)
      {
	uint8_t clip_alpha = clip_row[x];
	ಯದಿ (mask_row)
	  clip_alpha = hb_raster_div255 (mask_row[mx++] * clip_alpha);
	ಯದಿ (clip_alpha == 0) ಅನುವರ್ತ;
	ಯದಿ (clip_alpha == 255)
	{
	  ಯದಿ (premul_a == 255)
	    row[x] = hb_packed_t<uint32_t> (premul);
	  ಅನ್ಯಥಾ
	    row[x] = hb_packed_t<uint32_t> (hb_raster_src_over (premul, (uint32_t) row[x]));
	}
	ಅನ್ಯಥಾ
	{
	  uint32_t src = hb_raster_alpha_mul (premul, clip_alpha);
	  row[x] = hb_packed_t<uint32_t> (hb_raster_src_over (src, (uint32_t) row[x]));
	}
      }
    }
  }
  ಅನ್ಯಥಾ ಯದಿ (mask_buf)
  {
    ಪ್ರತಿ (unsigned y = iy0; y < iy1; y++)
    {
      hb_packed_t<uint32_t> *__restrict row = (hb_packed_t<uint32_t> *) (surf->buffer.arrayZ + y * stride);
      ಧ್ರುವ uint8_t *__restrict mask_row = mask_buf + (unsigned) ((int64_t) y - mask_y0) * mask_ext.stride;
      unsigned mx = (unsigned) ((int64_t) ix0 - mask_x0);
      ಪ್ರತಿ (unsigned x = ix0; x < ix1; x++)
      {
	uint8_t mask_alpha = mask_row[mx++];
	ಯದಿ (mask_alpha == 0) ಅನುವರ್ತ;
	uint32_t src = hb_raster_alpha_mul (premul, mask_alpha);
	row[x] = hb_packed_t<uint32_t> (hb_raster_src_over (src, (uint32_t) row[x]));
      }
    }
  }
  ಅನ್ಯಥಾ
  {
    ಪ್ರತಿ (unsigned y = clip.min_y; y < clip.max_y; y++)
    {
      hb_packed_t<uint32_t> *row = (hb_packed_t<uint32_t> *) (surf->buffer.arrayZ + y * stride);
      ಪ್ರತಿ (unsigned x = clip.min_x; x < clip.max_x; x++)
	row[x] = hb_packed_t<uint32_t> (hb_raster_src_over (premul, (uint32_t) row[x]));
    }
  }
}

ಸ್ಥಿರ ಶೂನ್ಯ
hb_raster_paint_color (hb_paint_funcs_t *pfuncs HB_UNUSED,
		       ಶೂನ್ಯ *paint_data,
		       hb_bool_t is_foreground HB_UNUSED,
		       hb_color_t color,
		       ಶೂನ್ಯ *user_data HB_UNUSED)
{
  hb_raster_paint_t *c = (hb_raster_paint_t *) paint_data;

  ensure_initialized (c);

  hb_raster_image_t *surf = c->current_surface ();
  ಯದಿ (unlikely (!surf)) ಪ್ರತ್ಯಾಗಮ;

  hb_raster_paint_solid (c, surf, color, nullptr);
}

ಸ್ಥಿರ ಶೂನ್ಯ
hb_raster_paint_fill_glyph (hb_paint_funcs_t *pfuncs HB_UNUSED,
			    ಶೂನ್ಯ *paint_data,
			    hb_codepoint_t glyph,
			    hb_font_t *font,
			    hb_bool_t is_foreground HB_UNUSED,
			    hb_color_t color,
			    ಶೂನ್ಯ *user_data HB_UNUSED)
{
  hb_raster_paint_t *c = (hb_raster_paint_t *) paint_data;

  ensure_initialized (c);

  hb_raster_image_t *surf = c->current_surface ();
  ಯದಿ (unlikely (!surf)) ಪ್ರತ್ಯಾಗಮ;

  /* Out of budget: skip the glyph-outline extraction entirely. */
  ಯದಿ (unlikely (c->budget_remaining < 0)) ಪ್ರತ್ಯಾಗಮ;

  hb_raster_draw_t *rdr = c->clip_rdr;
  hb_transform_t<> t = c->current_effective_transform ();
  hb_raster_draw_set_transform (rdr, t.xx, t.yx, t.xy, t.yy, t.x0, t.y0);
  hb_raster_paint_attach_clip_rdr (c, rdr, surf);

  hb_raster_paint_glyph_clip_data_t data = {glyph, font};
  hb_raster_paint_emit_clip_glyph_mask (rdr, &data);
  hb_raster_paint_readback_budget (c, rdr);

  ಯದಿ (unlikely (!c->precharge_work (hb_raster_draw_get_pixel_work (rdr,
							  surf->extents.height,
							  surf->extents.width))))
  {
    hb_raster_draw_clear (rdr);
    ಪ್ರತ್ಯಾಗಮ;
  }

  hb_raster_image_t *mask_img = hb_raster_draw_render (rdr);
  ಯದಿ (unlikely (!mask_img)) ಪ್ರತ್ಯಾಗಮ;

  hb_raster_extents_t mask_ext;
  hb_raster_image_get_extents (mask_img, &mask_ext);
  ಯದಿ (likely (c->charge_work ((int64_t) mask_ext.width * mask_ext.height)))
    hb_raster_paint_solid (c, surf, color, mask_img);

  hb_raster_draw_recycle_image (rdr, mask_img);
}

ಸ್ಥಿರ hb_bool_t
hb_raster_paint_image (hb_paint_funcs_t *pfuncs HB_UNUSED,
		       ಶೂನ್ಯ *paint_data,
		       hb_blob_t *blob,
		       unsigned width,
		       unsigned height,
		       hb_tag_t format,
		       ಭಿನ್ನ slant HB_UNUSED,
		       hb_glyph_extents_t *extents,
		       ಶೂನ್ಯ *user_data HB_UNUSED)
{
  hb_raster_paint_t *c = (hb_raster_paint_t *) paint_data;

  ensure_initialized (c);

  /* Out of pixel budget: skip, including the image decode below.  Image
   * compositing is pixel work and does no outline traversal. */
  ಯದಿ (unlikely (c->pixel_remaining < 0)) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

  unsigned src_width = width;
  unsigned src_height = height;
  ಧ್ರುವ hb_packed_t<uint32_t> *src_data = nullptr;
  hb_raster_image_t decoded_png;

  ಯದಿ (format == HB_PAINT_IMAGE_FORMAT_BGRA)
  {
    ಯದಿ (src_width == 0 || src_height == 0)
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    ಯದಿ (src_width > (unsigned) INT_MAX || src_height > (unsigned) INT_MAX)
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    unsigned data_len;
    ಧ್ರುವ uint8_t *data = (ಧ್ರುವ uint8_t *) hb_blob_get_data (blob, &data_len);
    size_t pixel_ಎಣಿಕೆ = (size_t) src_width * (size_t) src_height;
    ಯದಿ (src_width && pixel_ಎಣಿಕೆ / src_width != src_height)
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    ಯದಿ (pixel_ಎಣಿಕೆ > (size_t) -1 / 4u)
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    size_t required_size = pixel_ಎಣಿಕೆ * 4u;
    ಯದಿ (!data || (size_t) data_len < required_size)
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    src_data = (ಧ್ರುವ hb_packed_t<uint32_t> *) data;
  }
  ಅನ್ಯಥಾ ಯದಿ (format == HB_PAINT_IMAGE_FORMAT_PNG)
  {
#ifdef HAVE_PNG
    ಯದಿ (!decoded_png.deserialize_from_png (blob))
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    src_width = decoded_png.extents.width;
    src_height = decoded_png.extents.height;
    src_data = (ಧ್ರುವ hb_packed_t<uint32_t> *) decoded_png.buffer.arrayZ;
#ಅನ್ಯಥಾ
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
#endif
  }
  ಅನ್ಯಥಾ
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

  hb_raster_image_t *surf = c->current_surface ();
  ಯದಿ (unlikely (!surf)) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
  ಯದಿ (!extents) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

  ಧ್ರುವ hb_raster_clip_t &clip = c->current_clip ();
  hb_transform_t<> t = c->current_effective_transform ();

  /* Compute inverse transform for sampling */
  ಭಿನ್ನ det = t.xx * t.yy - t.xy * t.yx;
  ಯದಿ (fabsf (det) < 1e-10f) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
  ಭಿನ್ನ inv_det = 1.f / det;
  ಭಿನ್ನ inv_xx =  t.yy * inv_det;
  ಭಿನ್ನ inv_xy = -t.xy * inv_det;
  ಭಿನ್ನ inv_yx = -t.yx * inv_det;
  ಭಿನ್ನ inv_yy =  t.xx * inv_det;
  ಭಿನ್ನ inv_x0 = (t.xy * t.y0 - t.yy * t.x0) * inv_det;
  ಭಿನ್ನ inv_y0 = (t.yx * t.x0 - t.xx * t.y0) * inv_det;

  unsigned surf_stride = surf->extents.stride;
  ಪೂರ್ಣಾಙ್ಕ ox = surf->extents.x_origin;
  ಪೂರ್ಣಾಙ್ಕ oy = surf->extents.y_origin;

  /* Image source rectangle in glyph space */
  ಭಿನ್ನ img_x = extents->x_bearing;
  ಭಿನ್ನ img_y = extents->y_bearing;
  ಭಿನ್ನ img_sx = (ಭಿನ್ನ) extents->width / src_width;
  ಭಿನ್ನ img_sy = (ಭಿನ್ನ) extents->height / src_height;
  ಯದಿ (fabsf (img_sx) < 1e-10f || fabsf (img_sy) < 1e-10f)
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

  {
    unsigned clip_w = clip.max_x > clip.min_x ? clip.max_x - clip.min_x : 0;
    unsigned clip_h = clip.max_y > clip.min_y ? clip.max_y - clip.min_y : 0;
    ಯದಿ (unlikely (!c->charge_work ((int64_t) clip_w * clip_h +
				   (int64_t) src_width * src_height)))
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
  }

  ಯದಿ (clip.is_rect)
  {
    ಪ್ರತಿ (unsigned py = clip.min_y; py < clip.max_y; py++)
    {
      hb_packed_t<uint32_t> *row = (hb_packed_t<uint32_t> *) (surf->buffer.arrayZ + py * surf_stride);
      ಭಿನ್ನ gx = inv_xx * (ಭಿನ್ನ) ((ಪೂರ್ಣಾಙ್ಕ) clip.min_x + ox) + inv_xy * (ಭಿನ್ನ) ((ಪೂರ್ಣಾಙ್ಕ) py + oy) + inv_x0;
      ಭಿನ್ನ gy = inv_yx * (ಭಿನ್ನ) ((ಪೂರ್ಣಾಙ್ಕ) clip.min_x + ox) + inv_yy * (ಭಿನ್ನ) ((ಪೂರ್ಣಾಙ್ಕ) py + oy) + inv_y0;
      ಪ್ರತಿ (unsigned px = clip.min_x; px < clip.max_x; px++)
      {
	/* Map glyph space to image texel; bilinear reconstruction. */
	ಭಿನ್ನ ix = (gx - img_x) / img_sx;
	ಭಿನ್ನ iy = (ಭಿನ್ನ) (src_height - 1) - (gy - img_y) / img_sy;

	ಯದಿ (unlikely (!std::isfinite (ix) || !std::isfinite (iy)) ||
	    ix < 0.f || iy < 0.f ||
	    ix > (ಭಿನ್ನ) (src_width - 1) || iy > (ಭಿನ್ನ) (src_height - 1))
	{
	  gx += inv_xx;
	  gy += inv_yx;
	  ಅನುವರ್ತ;
	}

	uint32_t src_px = hb_raster_sample_bilinear_premul (src_data, src_width, src_height,
							     ix, iy);
	row[px] = hb_packed_t<uint32_t> (hb_raster_src_over (src_px, (uint32_t) row[px]));
	gx += inv_xx;
	gy += inv_yx;
      }
    }
  }
  ಅನ್ಯಥಾ
  {
    ಪ್ರತಿ (unsigned py = clip.min_y; py < clip.max_y; py++)
    {
      hb_packed_t<uint32_t> *row = (hb_packed_t<uint32_t> *) (surf->buffer.arrayZ + py * surf_stride);
      ಧ್ರುವ uint8_t *clip_row = clip.alpha.arrayZ + py * clip.stride;
      ಭಿನ್ನ gx = inv_xx * (ಭಿನ್ನ) ((ಪೂರ್ಣಾಙ್ಕ) clip.min_x + ox) + inv_xy * (ಭಿನ್ನ) ((ಪೂರ್ಣಾಙ್ಕ) py + oy) + inv_x0;
      ಭಿನ್ನ gy = inv_yx * (ಭಿನ್ನ) ((ಪೂರ್ಣಾಙ್ಕ) clip.min_x + ox) + inv_yy * (ಭಿನ್ನ) ((ಪೂರ್ಣಾಙ್ಕ) py + oy) + inv_y0;
      ಪ್ರತಿ (unsigned px = clip.min_x; px < clip.max_x; px++)
      {
	uint8_t clip_alpha = clip_row[px];
	ಯದಿ (clip_alpha == 0)
	{
	  gx += inv_xx;
	  gy += inv_yx;
	  ಅನುವರ್ತ;
	}

	/* Map glyph space to image texel; bilinear reconstruction. */
	ಭಿನ್ನ ix = (gx - img_x) / img_sx;
	ಭಿನ್ನ iy = (ಭಿನ್ನ) (src_height - 1) - (gy - img_y) / img_sy;

	ಯದಿ (unlikely (!std::isfinite (ix) || !std::isfinite (iy)) ||
	    ix < 0.f || iy < 0.f ||
	    ix > (ಭಿನ್ನ) (src_width - 1) || iy > (ಭಿನ್ನ) (src_height - 1))
	{
	  gx += inv_xx;
	  gy += inv_yx;
	  ಅನುವರ್ತ;
	}

	uint32_t src_px = hb_raster_sample_bilinear_premul (src_data, src_width, src_height,
							     ix, iy);
	src_px = hb_raster_alpha_mul (src_px, clip_alpha);
	row[px] = hb_packed_t<uint32_t> (hb_raster_src_over (src_px, (uint32_t) row[px]));
	gx += inv_xx;
	gy += inv_yx;
      }
    }
  }

  ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
}


/*
 * Gradient helpers
 */

#define GRADIENT_LUT_SIZE 256
#define GRADIENT_LUT_MIN_PIXELS (64u * 64u)

ಸ್ಥಿರ HB_ALWAYS_INLINE ಭಿನ್ನ
reflect_gradient_t (ಭಿನ್ನ t)
{
  t = fmodf (fabsf (t), 2.f);
  ಪ್ರತ್ಯಾಗಮ t > 1.f ? 2.f - t : t;
}

/* Evaluate color at normalized position t, interpolating in premultiplied space. */
ಸ್ಥಿರ uint32_t
evaluate_color_ಸಾಲು (ಧ್ರುವ hb_color_stop_t *stops, unsigned len, ಭಿನ್ನ t,
		     hb_paint_extend_t extend)
{
  ಯದಿ (unlikely (!len))
    ಪ್ರತ್ಯಾಗಮ 0;
  ಯದಿ (unlikely (len == 1))
    ಪ್ರತ್ಯಾಗಮ color_to_premul_pixel (stops[0].color);
  ಯದಿ (unlikely (!std::isfinite (t)))
    t = 0.f;

  /* Apply extend mode */
  ಯದಿ (extend == HB_PAINT_EXTEND_PAD)
  {
    t = hb_clamp (t, 0.f, 1.f);
  }
  ಅನ್ಯಥಾ ಯದಿ (extend == HB_PAINT_EXTEND_REPEAT)
  {
    t = t - floorf (t);
  }
  ಅನ್ಯಥಾ /* REFLECT */
  {
    t = reflect_gradient_t (t);
  }

  /* Find bounding stops */
  ಯದಿ (t <= stops[0].offset)
    ಪ್ರತ್ಯಾಗಮ color_to_premul_pixel (stops[0].color);
  ಯದಿ (t >= stops[len - 1].offset)
    ಪ್ರತ್ಯಾಗಮ color_to_premul_pixel (stops[len - 1].color);

  unsigned i;
  ಪ್ರತಿ (i = 0; i < len - 1; i++)
    ಯದಿ (t < stops[i + 1].offset)
      ಭಙ್ಗ;

  ಭಿನ್ನ range = stops[i + 1].offset - stops[i].offset;
  ಭಿನ್ನ k = range > 0.f ? (t - stops[i].offset) / range : 0.f;

  /* Interpolate in premultiplied [0,255] space */
  hb_color_t c0 = stops[i].color;
  hb_color_t c1 = stops[i + 1].color;

  ಭಿನ್ನ a0 = hb_color_get_alpha (c0) / 255.f;
  ಭಿನ್ನ r0 = hb_color_get_red   (c0) / 255.f * a0;
  ಭಿನ್ನ g0 = hb_color_get_green (c0) / 255.f * a0;
  ಭಿನ್ನ b0 = hb_color_get_blue  (c0) / 255.f * a0;

  ಭಿನ್ನ a1 = hb_color_get_alpha (c1) / 255.f;
  ಭಿನ್ನ r1 = hb_color_get_red   (c1) / 255.f * a1;
  ಭಿನ್ನ g1 = hb_color_get_green (c1) / 255.f * a1;
  ಭಿನ್ನ b1 = hb_color_get_blue  (c1) / 255.f * a1;

  ಭಿನ್ನ a = a0 + k * (a1 - a0);
  ಭಿನ್ನ r = r0 + k * (r1 - r0);
  ಭಿನ್ನ g = g0 + k * (g1 - g0);
  ಭಿನ್ನ b = b0 + k * (b1 - b0);

  uint8_t pa = (uint8_t) (a * 255.f + 0.5f);
  uint8_t pr = (uint8_t) (r * 255.f + 0.5f);
  uint8_t pg = (uint8_t) (g * 255.f + 0.5f);
  uint8_t pb = (uint8_t) (b * 255.f + 0.5f);

  ಪ್ರತ್ಯಾಗಮ (uint32_t) pb | ((uint32_t) pg << 8) | ((uint32_t) pr << 16) | ((uint32_t) pa << 24);
}

ಸ್ಥಿರ HB_ALWAYS_INLINE ಭಿನ್ನ
normalize_gradient_t (ಭಿನ್ನ t, hb_paint_extend_t extend)
{
  ಯದಿ (unlikely (!std::isfinite (t)))
    ಪ್ರತ್ಯಾಗಮ 0.f;

  ಯದಿ (extend == HB_PAINT_EXTEND_PAD)
    ಪ್ರತ್ಯಾಗಮ hb_clamp (t, 0.f, 1.f);
  ಯದಿ (extend == HB_PAINT_EXTEND_REPEAT)
  {
    t = t - floorf (t);
    ಪ್ರತ್ಯಾಗಮ t < 0.f ? t + 1.f : t;
  }

  /* REFLECT */
  ಪ್ರತ್ಯಾಗಮ reflect_gradient_t (t);
}

ಸ್ಥಿರ ಶೂನ್ಯ
build_gradient_lut (ಧ್ರುವ hb_color_stop_t *stops,
		    unsigned len,
		    uint32_t *lut)
{
  ಪ್ರತಿ (unsigned i = 0; i < GRADIENT_LUT_SIZE; i++)
  {
    ಭಿನ್ನ t = (ಭಿನ್ನ) i / (GRADIENT_LUT_SIZE - 1);
    lut[i] = evaluate_color_ಸಾಲು (stops, len, t, HB_PAINT_EXTEND_PAD);
  }
}

ಸ್ಥಿರ HB_ALWAYS_INLINE uint32_t
lookup_gradient_lut (ಧ್ರುವ uint32_t *lut,
		     ಭಿನ್ನ t,
		     hb_paint_extend_t extend)
{
  ಭಿನ್ನ u = normalize_gradient_t (t, extend);
  ಯದಿ (unlikely (!std::isfinite (u)))
    u = 0.f;
  ಅನ್ಯಥಾ
    u = hb_clamp (u, 0.f, 1.f);
  unsigned idx = (unsigned) (u * (GRADIENT_LUT_SIZE - 1) + 0.5f);
  ಪ್ರತ್ಯಾಗಮ lut[idx];
}

/*
 * Gradient paint callbacks
 */

ಸ್ಥಿರ ಶೂನ್ಯ
hb_raster_paint_linear_gradient (hb_paint_funcs_t *pfuncs HB_UNUSED,
				 ಶೂನ್ಯ *paint_data,
				 hb_color_ಸಾಲು_t *color_ಸಾಲು,
				 ಭಿನ್ನ x0, ಭಿನ್ನ y0,
				 ಭಿನ್ನ x1, ಭಿನ್ನ y1,
				 ಭಿನ್ನ x2, ಭಿನ್ನ y2,
				 ಶೂನ್ಯ *user_data HB_UNUSED)
{
  hb_raster_paint_t *c = (hb_raster_paint_t *) paint_data;

  ensure_initialized (c);

  hb_raster_image_t *surf = c->current_surface ();
  ಯದಿ (unlikely (!surf)) ಪ್ರತ್ಯಾಗಮ;

  ಯದಿ (unlikely (!c->fetch_color_stops (color_ಸಾಲು)))
    ಪ್ರತ್ಯಾಗಮ;
  hb_color_stop_t *stops = c->scratch_color_stops.arrayZ;
  unsigned len = c->scratch_color_stops.length;
  ಭಿನ್ನ mn, mx;
  hb_paint_normalize_color_ಸಾಲು (stops, len, &mn, &mx);

  hb_paint_extend_t extend = hb_color_ಸಾಲು_get_extend (color_ಸಾಲು);
  ಧ್ರುವ hb_raster_clip_t &clip = c->current_clip ();
  unsigned clip_w = clip.max_x > clip.min_x ? clip.max_x - clip.min_x : 0;
  unsigned clip_h = clip.max_y > clip.min_y ? clip.max_y - clip.min_y : 0;
  ಯದಿ (unlikely (!c->charge_work ((int64_t) clip_w * clip_h)))
    ಪ್ರತ್ಯಾಗಮ;
  ತಾರ್ಕಿಕ use_lut = (uint64_t) clip_w * clip_h >= GRADIENT_LUT_MIN_PIXELS;
  uint32_t lut[GRADIENT_LUT_SIZE];
  ಯದಿ (use_lut)
    build_gradient_lut (stops, len, lut);

  /* Reduce 3-point anchor to 2-point gradient axis */
  ಭಿನ್ನ lx0, ly0, lx1, ly1;
  hb_paint_reduce_linear_anchors (x0, y0, x1, y1, x2, y2,
				  &lx0, &ly0, &lx1, &ly1);

  /* Apply normalization to endpoints */
  ಭಿನ್ನ gx0 = lx0 + mn * (lx1 - lx0);
  ಭಿನ್ನ gy0 = ly0 + mn * (ly1 - ly0);
  ಭಿನ್ನ gx1 = lx0 + mx * (lx1 - lx0);
  ಭಿನ್ನ gy1 = ly0 + mx * (ly1 - ly0);

  /* Inverse transform: pixel → glyph space */
  hb_transform_t<> t = c->current_effective_transform ();
  ಭಿನ್ನ det = t.xx * t.yy - t.xy * t.yx;
  ಯದಿ (fabsf (det) < 1e-10f) ಪ್ರತ್ಯಾಗಮ;

  {
    ಭಿನ್ನ inv_det = 1.f / det;
    ಭಿನ್ನ inv_xx =  t.yy * inv_det;
    ಭಿನ್ನ inv_xy = -t.xy * inv_det;
    ಭಿನ್ನ inv_yx = -t.yx * inv_det;
    ಭಿನ್ನ inv_yy =  t.xx * inv_det;
    ಭಿನ್ನ inv_x0 = (t.xy * t.y0 - t.yy * t.x0) * inv_det;
    ಭಿನ್ನ inv_y0 = (t.yx * t.x0 - t.xx * t.y0) * inv_det;

    /* Gradient direction vector and denominator for projection */
    ಭಿನ್ನ dx = gx1 - gx0, dy = gy1 - gy0;
    ಭಿನ್ನ denom = dx * dx + dy * dy;
    ಯದಿ (denom < 1e-10f) ಪ್ರತ್ಯಾಗಮ;
    ಭಿನ್ನ inv_denom = 1.f / denom;

    unsigned stride = surf->extents.stride;
    ಪೂರ್ಣಾಙ್ಕ ox = surf->extents.x_origin;
    ಪೂರ್ಣಾಙ್ಕ oy = surf->extents.y_origin;

    ಯದಿ (clip.is_rect)
    {
      ಪ್ರತಿ (unsigned py = clip.min_y; py < clip.max_y; py++)
      {
	hb_packed_t<uint32_t> *row = (hb_packed_t<uint32_t> *) (surf->buffer.arrayZ + py * stride);
	ಭಿನ್ನ gx = inv_xx * ((ಭಿನ್ನ) ((ಪೂರ್ಣಾಙ್ಕ) clip.min_x + ox) + 0.5f) + inv_xy * ((ಭಿನ್ನ) ((ಪೂರ್ಣಾಙ್ಕ) py + oy) + 0.5f) + inv_x0;
	ಭಿನ್ನ gy = inv_yx * ((ಭಿನ್ನ) ((ಪೂರ್ಣಾಙ್ಕ) clip.min_x + ox) + 0.5f) + inv_yy * ((ಭಿನ್ನ) ((ಪೂರ್ಣಾಙ್ಕ) py + oy) + 0.5f) + inv_y0;
	ಯದಿ (use_lut)
	{
	  ಪ್ರತಿ (unsigned px = clip.min_x; px < clip.max_x; px++)
	  {
	    ಭಿನ್ನ proj_t = ((gx - gx0) * dx + (gy - gy0) * dy) * inv_denom;
	    uint32_t src = lookup_gradient_lut (lut, proj_t, extend);
	    row[px] = hb_packed_t<uint32_t> (hb_raster_src_over (src, (uint32_t) row[px]));
	    gx += inv_xx;
	    gy += inv_yx;
	  }
	}
	ಅನ್ಯಥಾ
	{
	  ಪ್ರತಿ (unsigned px = clip.min_x; px < clip.max_x; px++)
	  {
	    ಭಿನ್ನ proj_t = ((gx - gx0) * dx + (gy - gy0) * dy) * inv_denom;
	    uint32_t src = evaluate_color_ಸಾಲು (stops, len, proj_t, extend);
	    row[px] = hb_packed_t<uint32_t> (hb_raster_src_over (src, (uint32_t) row[px]));
	    gx += inv_xx;
	    gy += inv_yx;
	  }
	}
      }
    }
    ಅನ್ಯಥಾ
    {
      ಪ್ರತಿ (unsigned py = clip.min_y; py < clip.max_y; py++)
      {
	hb_packed_t<uint32_t> *row = (hb_packed_t<uint32_t> *) (surf->buffer.arrayZ + py * stride);
	ಧ್ರುವ uint8_t *clip_row = clip.alpha.arrayZ + py * clip.stride;
	ಭಿನ್ನ gx = inv_xx * ((ಭಿನ್ನ) ((ಪೂರ್ಣಾಙ್ಕ) clip.min_x + ox) + 0.5f) + inv_xy * ((ಭಿನ್ನ) ((ಪೂರ್ಣಾಙ್ಕ) py + oy) + 0.5f) + inv_x0;
	ಭಿನ್ನ gy = inv_yx * ((ಭಿನ್ನ) ((ಪೂರ್ಣಾಙ್ಕ) clip.min_x + ox) + 0.5f) + inv_yy * ((ಭಿನ್ನ) ((ಪೂರ್ಣಾಙ್ಕ) py + oy) + 0.5f) + inv_y0;
	ಯದಿ (use_lut)
	{
	  ಪ್ರತಿ (unsigned px = clip.min_x; px < clip.max_x; px++)
	  {
	    uint8_t clip_alpha = clip_row[px];
	    ಯದಿ (clip_alpha == 0)
	    {
	      gx += inv_xx;
	      gy += inv_yx;
	      ಅನುವರ್ತ;
	    }
	    ಭಿನ್ನ proj_t = ((gx - gx0) * dx + (gy - gy0) * dy) * inv_denom;
	    uint32_t src = lookup_gradient_lut (lut, proj_t, extend);
	    src = hb_raster_alpha_mul (src, clip_alpha);
	    row[px] = hb_packed_t<uint32_t> (hb_raster_src_over (src, (uint32_t) row[px]));
	    gx += inv_xx;
	    gy += inv_yx;
	  }
	}
	ಅನ್ಯಥಾ
	{
	  ಪ್ರತಿ (unsigned px = clip.min_x; px < clip.max_x; px++)
	  {
	    uint8_t clip_alpha = clip_row[px];
	    ಯದಿ (clip_alpha == 0)
	    {
	      gx += inv_xx;
	      gy += inv_yx;
	      ಅನುವರ್ತ;
	    }
	    ಭಿನ್ನ proj_t = ((gx - gx0) * dx + (gy - gy0) * dy) * inv_denom;
	    uint32_t src = evaluate_color_ಸಾಲು (stops, len, proj_t, extend);
	    src = hb_raster_alpha_mul (src, clip_alpha);
	    row[px] = hb_packed_t<uint32_t> (hb_raster_src_over (src, (uint32_t) row[px]));
	    gx += inv_xx;
	    gy += inv_yx;
	  }
	}
      }
    }
  }

}

ಸ್ಥಿರ ಶೂನ್ಯ
hb_raster_paint_radial_gradient (hb_paint_funcs_t *pfuncs HB_UNUSED,
				 ಶೂನ್ಯ *paint_data,
				 hb_color_ಸಾಲು_t *color_ಸಾಲು,
				 ಭಿನ್ನ x0, ಭಿನ್ನ y0, ಭಿನ್ನ r0,
				 ಭಿನ್ನ x1, ಭಿನ್ನ y1, ಭಿನ್ನ r1,
				 ಶೂನ್ಯ *user_data HB_UNUSED)
{
  hb_raster_paint_t *c = (hb_raster_paint_t *) paint_data;

  ensure_initialized (c);

  hb_raster_image_t *surf = c->current_surface ();
  ಯದಿ (unlikely (!surf)) ಪ್ರತ್ಯಾಗಮ;

  ಯದಿ (unlikely (!c->fetch_color_stops (color_ಸಾಲು)))
    ಪ್ರತ್ಯಾಗಮ;
  hb_color_stop_t *stops = c->scratch_color_stops.arrayZ;
  unsigned len = c->scratch_color_stops.length;
  ಭಿನ್ನ mn, mx;
  hb_paint_normalize_color_ಸಾಲು (stops, len, &mn, &mx);

  hb_paint_extend_t extend = hb_color_ಸಾಲು_get_extend (color_ಸಾಲು);
  ಧ್ರುವ hb_raster_clip_t &clip = c->current_clip ();
  unsigned clip_w = clip.max_x > clip.min_x ? clip.max_x - clip.min_x : 0;
  unsigned clip_h = clip.max_y > clip.min_y ? clip.max_y - clip.min_y : 0;
  ಯದಿ (unlikely (!c->charge_work ((int64_t) clip_w * clip_h)))
    ಪ್ರತ್ಯಾಗಮ;
  ತಾರ್ಕಿಕ use_lut = (uint64_t) clip_w * clip_h >= GRADIENT_LUT_MIN_PIXELS;
  uint32_t lut[GRADIENT_LUT_SIZE];
  ಯದಿ (use_lut)
    build_gradient_lut (stops, len, lut);

  /* Apply normalization to circle parameters */
  ಭಿನ್ನ cx0 = x0 + mn * (x1 - x0);
  ಭಿನ್ನ cy0 = y0 + mn * (y1 - y0);
  ಭಿನ್ನ cr0 = r0 + mn * (r1 - r0);
  ಭಿನ್ನ cx1 = x0 + mx * (x1 - x0);
  ಭಿನ್ನ cy1 = y0 + mx * (y1 - y0);
  ಭಿನ್ನ cr1 = r0 + mx * (r1 - r0);

  /* Inverse transform */
  hb_transform_t<> t = c->current_effective_transform ();
  ಭಿನ್ನ det = t.xx * t.yy - t.xy * t.yx;
  ಯದಿ (fabsf (det) < 1e-10f) ಪ್ರತ್ಯಾಗಮ;

  {
    ಭಿನ್ನ inv_det = 1.f / det;
    ಭಿನ್ನ inv_xx =  t.yy * inv_det;
    ಭಿನ್ನ inv_xy = -t.xy * inv_det;
    ಭಿನ್ನ inv_yx = -t.yx * inv_det;
    ಭಿನ್ನ inv_yy =  t.xx * inv_det;
    ಭಿನ್ನ inv_x0 = (t.xy * t.y0 - t.yy * t.x0) * inv_det;
    ಭಿನ್ನ inv_y0 = (t.yx * t.x0 - t.xx * t.y0) * inv_det;

    /* Precompute quadratic coefficients for radial gradient:
     * |p - c0 - t*(c1-c0)|^2 = (r0 + t*(r1-r0))^2
     *
     * Expanding gives At^2 + Bt + C = 0 where:
     *   cdx = c1.x - c0.x, cdy = c1.y - c0.y, dr = r1 - r0
     *   A = cdx^2 + cdy^2 - dr^2
     *   B = -2*(px-c0.x)*cdx - 2*(py-c0.y)*cdy - 2*r0*dr
     *   C = (px-c0.x)^2 + (py-c0.y)^2 - r0^2
     */
    ಭಿನ್ನ cdx = cx1 - cx0, cdy = cy1 - cy0;
    ಭಿನ್ನ dr = cr1 - cr0;
    ಭಿನ್ನ A = cdx * cdx + cdy * cdy - dr * dr;

    unsigned stride = surf->extents.stride;
    ಪೂರ್ಣಾಙ್ಕ ox = surf->extents.x_origin;
    ಪೂರ್ಣಾಙ್ಕ oy = surf->extents.y_origin;

    ಯದಿ (clip.is_rect)
    {
      ಪ್ರತಿ (unsigned py = clip.min_y; py < clip.max_y; py++)
      {
	hb_packed_t<uint32_t> *row = (hb_packed_t<uint32_t> *) (surf->buffer.arrayZ + py * stride);
	ಭಿನ್ನ gx = inv_xx * ((ಭಿನ್ನ) ((ಪೂರ್ಣಾಙ್ಕ) clip.min_x + ox) + 0.5f) + inv_xy * ((ಭಿನ್ನ) ((ಪೂರ್ಣಾಙ್ಕ) py + oy) + 0.5f) + inv_x0;
	ಭಿನ್ನ gy = inv_yx * ((ಭಿನ್ನ) ((ಪೂರ್ಣಾಙ್ಕ) clip.min_x + ox) + 0.5f) + inv_yy * ((ಭಿನ್ನ) ((ಪೂರ್ಣಾಙ್ಕ) py + oy) + 0.5f) + inv_y0;
	ಯದಿ (use_lut)
	{
	  ಪ್ರತಿ (unsigned px = clip.min_x; px < clip.max_x; px++)
	  {
	    ಭಿನ್ನ dpx = gx - cx0, dpy = gy - cy0;
	    ಭಿನ್ನ B = -2.f * (dpx * cdx + dpy * cdy + cr0 * dr);
	    ಭಿನ್ನ C = dpx * dpx + dpy * dpy - cr0 * cr0;

	    ಭಿನ್ನ grad_t;
	    ಯದಿ (fabsf (A) > 1e-10f)
	    {
	      ಭಿನ್ನ disc = B * B - 4.f * A * C;
	      ಯದಿ (disc < 0.f)
	      {
		gx += inv_xx;
		gy += inv_yx;
		ಅನುವರ್ತ;
	      }
	      ಭಿನ್ನ sq = sqrtf (disc);
	      /* Pick the larger root (t closer to 1 = outer circle) */
	      ಭಿನ್ನ t1 = (-B + sq) / (2.f * A);
	      ಭಿನ್ನ t2 = (-B - sq) / (2.f * A);
	      /* Choose the root that gives a positive radius */
	      ಯದಿ (cr0 + t1 * dr >= 0.f)
		grad_t = t1;
	      ಅನ್ಯಥಾ
		grad_t = t2;
	    }
	    ಅನ್ಯಥಾ
	    {
	      /* Linear case: Bt + C = 0 */
	      ಯದಿ (fabsf (B) < 1e-10f)
	      {
		gx += inv_xx;
		gy += inv_yx;
		ಅನುವರ್ತ;
	      }
	      grad_t = -C / B;
	    }

	    uint32_t src = lookup_gradient_lut (lut, grad_t, extend);
	    row[px] = hb_packed_t<uint32_t> (hb_raster_src_over (src, (uint32_t) row[px]));
	    gx += inv_xx;
	    gy += inv_yx;
	  }
	}
	ಅನ್ಯಥಾ
	{
	  ಪ್ರತಿ (unsigned px = clip.min_x; px < clip.max_x; px++)
	  {
	    ಭಿನ್ನ dpx = gx - cx0, dpy = gy - cy0;
	    ಭಿನ್ನ B = -2.f * (dpx * cdx + dpy * cdy + cr0 * dr);
	    ಭಿನ್ನ C = dpx * dpx + dpy * dpy - cr0 * cr0;

	    ಭಿನ್ನ grad_t;
	    ಯದಿ (fabsf (A) > 1e-10f)
	    {
	      ಭಿನ್ನ disc = B * B - 4.f * A * C;
	      ಯದಿ (disc < 0.f)
	      {
		gx += inv_xx;
		gy += inv_yx;
		ಅನುವರ್ತ;
	      }
	      ಭಿನ್ನ sq = sqrtf (disc);
	      ಭಿನ್ನ t1 = (-B + sq) / (2.f * A);
	      ಭಿನ್ನ t2 = (-B - sq) / (2.f * A);
	      grad_t = (cr0 + t1 * dr >= 0.f) ? t1 : t2;
	    }
	    ಅನ್ಯಥಾ
	    {
	      ಯದಿ (fabsf (B) < 1e-10f)
	      {
		gx += inv_xx;
		gy += inv_yx;
		ಅನುವರ್ತ;
	      }
	      grad_t = -C / B;
	    }

	    uint32_t src = evaluate_color_ಸಾಲು (stops, len, grad_t, extend);
	    row[px] = hb_packed_t<uint32_t> (hb_raster_src_over (src, (uint32_t) row[px]));
	    gx += inv_xx;
	    gy += inv_yx;
	  }
	}
      }
    }
    ಅನ್ಯಥಾ
    {
      ಪ್ರತಿ (unsigned py = clip.min_y; py < clip.max_y; py++)
      {
	hb_packed_t<uint32_t> *row = (hb_packed_t<uint32_t> *) (surf->buffer.arrayZ + py * stride);
	ಧ್ರುವ uint8_t *clip_row = clip.alpha.arrayZ + py * clip.stride;
	ಭಿನ್ನ gx = inv_xx * ((ಭಿನ್ನ) ((ಪೂರ್ಣಾಙ್ಕ) clip.min_x + ox) + 0.5f) + inv_xy * ((ಭಿನ್ನ) ((ಪೂರ್ಣಾಙ್ಕ) py + oy) + 0.5f) + inv_x0;
	ಭಿನ್ನ gy = inv_yx * ((ಭಿನ್ನ) ((ಪೂರ್ಣಾಙ್ಕ) clip.min_x + ox) + 0.5f) + inv_yy * ((ಭಿನ್ನ) ((ಪೂರ್ಣಾಙ್ಕ) py + oy) + 0.5f) + inv_y0;
	ಯದಿ (use_lut)
	{
	  ಪ್ರತಿ (unsigned px = clip.min_x; px < clip.max_x; px++)
	  {
	    uint8_t clip_alpha = clip_row[px];
	    ಯದಿ (clip_alpha == 0)
	    {
	      gx += inv_xx;
	      gy += inv_yx;
	      ಅನುವರ್ತ;
	    }
	    ಭಿನ್ನ dpx = gx - cx0, dpy = gy - cy0;
	    ಭಿನ್ನ B = -2.f * (dpx * cdx + dpy * cdy + cr0 * dr);
	    ಭಿನ್ನ C = dpx * dpx + dpy * dpy - cr0 * cr0;

	    ಭಿನ್ನ grad_t;
	    ಯದಿ (fabsf (A) > 1e-10f)
	    {
	      ಭಿನ್ನ disc = B * B - 4.f * A * C;
	      ಯದಿ (disc < 0.f)
	      {
		gx += inv_xx;
		gy += inv_yx;
		ಅನುವರ್ತ;
	      }
	      ಭಿನ್ನ sq = sqrtf (disc);
	      ಭಿನ್ನ t1 = (-B + sq) / (2.f * A);
	      ಭಿನ್ನ t2 = (-B - sq) / (2.f * A);
	      grad_t = (cr0 + t1 * dr >= 0.f) ? t1 : t2;
	    }
	    ಅನ್ಯಥಾ
	    {
	      ಯದಿ (fabsf (B) < 1e-10f)
	      {
		gx += inv_xx;
		gy += inv_yx;
		ಅನುವರ್ತ;
	      }
	      grad_t = -C / B;
	    }

	    uint32_t src = lookup_gradient_lut (lut, grad_t, extend);
	    src = hb_raster_alpha_mul (src, clip_alpha);
	    row[px] = hb_packed_t<uint32_t> (hb_raster_src_over (src, (uint32_t) row[px]));
	    gx += inv_xx;
	    gy += inv_yx;
	  }
	}
	ಅನ್ಯಥಾ
	{
	  ಪ್ರತಿ (unsigned px = clip.min_x; px < clip.max_x; px++)
	  {
	    uint8_t clip_alpha = clip_row[px];
	    ಯದಿ (clip_alpha == 0)
	    {
	      gx += inv_xx;
	      gy += inv_yx;
	      ಅನುವರ್ತ;
	    }
	    ಭಿನ್ನ dpx = gx - cx0, dpy = gy - cy0;
	    ಭಿನ್ನ B = -2.f * (dpx * cdx + dpy * cdy + cr0 * dr);
	    ಭಿನ್ನ C = dpx * dpx + dpy * dpy - cr0 * cr0;

	    ಭಿನ್ನ grad_t;
	    ಯದಿ (fabsf (A) > 1e-10f)
	    {
	      ಭಿನ್ನ disc = B * B - 4.f * A * C;
	      ಯದಿ (disc < 0.f)
	      {
		gx += inv_xx;
		gy += inv_yx;
		ಅನುವರ್ತ;
	      }
	      ಭಿನ್ನ sq = sqrtf (disc);
	      ಭಿನ್ನ t1 = (-B + sq) / (2.f * A);
	      ಭಿನ್ನ t2 = (-B - sq) / (2.f * A);
	      grad_t = (cr0 + t1 * dr >= 0.f) ? t1 : t2;
	    }
	    ಅನ್ಯಥಾ
	    {
	      ಯದಿ (fabsf (B) < 1e-10f)
	      {
		gx += inv_xx;
		gy += inv_yx;
		ಅನುವರ್ತ;
	      }
	      grad_t = -C / B;
	    }

	    uint32_t src = evaluate_color_ಸಾಲು (stops, len, grad_t, extend);
	    src = hb_raster_alpha_mul (src, clip_alpha);
	    row[px] = hb_packed_t<uint32_t> (hb_raster_src_over (src, (uint32_t) row[px]));
	    gx += inv_xx;
	    gy += inv_yx;
	  }
	}
      }
    }
  }

}

ಸ್ಥಿರ ಶೂನ್ಯ
hb_raster_paint_sweep_gradient (hb_paint_funcs_t *pfuncs HB_UNUSED,
				ಶೂನ್ಯ *paint_data,
				hb_color_ಸಾಲು_t *color_ಸಾಲು,
				ಭಿನ್ನ cx, ಭಿನ್ನ cy,
				ಭಿನ್ನ start_angle,
				ಭಿನ್ನ end_angle,
				ಶೂನ್ಯ *user_data HB_UNUSED)
{
  hb_raster_paint_t *c = (hb_raster_paint_t *) paint_data;

  ensure_initialized (c);

  hb_raster_image_t *surf = c->current_surface ();
  ಯದಿ (unlikely (!surf)) ಪ್ರತ್ಯಾಗಮ;

  ಯದಿ (unlikely (!c->fetch_color_stops (color_ಸಾಲು)))
    ಪ್ರತ್ಯಾಗಮ;
  hb_color_stop_t *stops = c->scratch_color_stops.arrayZ;
  unsigned len = c->scratch_color_stops.length;
  ಭಿನ್ನ mn, mx;
  hb_paint_normalize_color_ಸಾಲು (stops, len, &mn, &mx);

  hb_paint_extend_t extend = hb_color_ಸಾಲು_get_extend (color_ಸಾಲು);
  ಧ್ರುವ hb_raster_clip_t &clip = c->current_clip ();
  unsigned clip_w = clip.max_x > clip.min_x ? clip.max_x - clip.min_x : 0;
  unsigned clip_h = clip.max_y > clip.min_y ? clip.max_y - clip.min_y : 0;
  ಯದಿ (unlikely (!c->charge_work ((int64_t) clip_w * clip_h)))
    ಪ್ರತ್ಯಾಗಮ;
  ತಾರ್ಕಿಕ use_lut = (uint64_t) clip_w * clip_h >= GRADIENT_LUT_MIN_PIXELS;
  uint32_t lut[GRADIENT_LUT_SIZE];
  ಯದಿ (use_lut)
    build_gradient_lut (stops, len, lut);

  /* Apply normalization to angle range */
  ಭಿನ್ನ a0 = start_angle + mn * (end_angle - start_angle);
  ಭಿನ್ನ a1 = start_angle + mx * (end_angle - start_angle);
  ಭಿನ್ನ angle_range = a1 - a0;

  /* Inverse transform */
  hb_transform_t<> t = c->current_effective_transform ();
  ಭಿನ್ನ det = t.xx * t.yy - t.xy * t.yx;
  ಯದಿ (fabsf (det) < 1e-10f || fabsf (angle_range) < 1e-10f) ಪ್ರತ್ಯಾಗಮ;

  {
    ಭಿನ್ನ inv_det = 1.f / det;
    ಭಿನ್ನ inv_xx =  t.yy * inv_det;
    ಭಿನ್ನ inv_xy = -t.xy * inv_det;
    ಭಿನ್ನ inv_yx = -t.yx * inv_det;
    ಭಿನ್ನ inv_yy =  t.xx * inv_det;
    ಭಿನ್ನ inv_x0 = (t.xy * t.y0 - t.yy * t.x0) * inv_det;
    ಭಿನ್ನ inv_y0 = (t.yx * t.x0 - t.xx * t.y0) * inv_det;

    ಭಿನ್ನ inv_angle_range = 1.f / angle_range;

    unsigned stride = surf->extents.stride;
    ಪೂರ್ಣಾಙ್ಕ ox = surf->extents.x_origin;
    ಪೂರ್ಣಾಙ್ಕ oy = surf->extents.y_origin;

    ಯದಿ (clip.is_rect)
    {
      ಪ್ರತಿ (unsigned py = clip.min_y; py < clip.max_y; py++)
      {
	hb_packed_t<uint32_t> *row = (hb_packed_t<uint32_t> *) (surf->buffer.arrayZ + py * stride);
	ಭಿನ್ನ gx = inv_xx * ((ಭಿನ್ನ) ((ಪೂರ್ಣಾಙ್ಕ) clip.min_x + ox) + 0.5f) + inv_xy * ((ಭಿನ್ನ) ((ಪೂರ್ಣಾಙ್ಕ) py + oy) + 0.5f) + inv_x0;
	ಭಿನ್ನ gy = inv_yx * ((ಭಿನ್ನ) ((ಪೂರ್ಣಾಙ್ಕ) clip.min_x + ox) + 0.5f) + inv_yy * ((ಭಿನ್ನ) ((ಪೂರ್ಣಾಙ್ಕ) py + oy) + 0.5f) + inv_y0;
	ಯದಿ (use_lut)
	{
	  ಪ್ರತಿ (unsigned px = clip.min_x; px < clip.max_x; px++)
	  {
	    ಭಿನ್ನ angle = atan2f (gy - cy, gx - cx);
	    ಯದಿ (angle < 0) angle += (ಭಿನ್ನ) HB_2_PI;
	    ಭಿನ್ನ grad_t = (angle - a0) * inv_angle_range;
	    uint32_t src = lookup_gradient_lut (lut, grad_t, extend);
	    row[px] = hb_packed_t<uint32_t> (hb_raster_src_over (src, (uint32_t) row[px]));
	    gx += inv_xx;
	    gy += inv_yx;
	  }
	}
	ಅನ್ಯಥಾ
	{
	  ಪ್ರತಿ (unsigned px = clip.min_x; px < clip.max_x; px++)
	  {
	    ಭಿನ್ನ angle = atan2f (gy - cy, gx - cx);
	    ಯದಿ (angle < 0) angle += (ಭಿನ್ನ) HB_2_PI;
	    ಭಿನ್ನ grad_t = (angle - a0) * inv_angle_range;
	    uint32_t src = evaluate_color_ಸಾಲು (stops, len, grad_t, extend);
	    row[px] = hb_packed_t<uint32_t> (hb_raster_src_over (src, (uint32_t) row[px]));
	    gx += inv_xx;
	    gy += inv_yx;
	  }
	}
      }
    }
    ಅನ್ಯಥಾ
    {
      ಪ್ರತಿ (unsigned py = clip.min_y; py < clip.max_y; py++)
      {
	hb_packed_t<uint32_t> *row = (hb_packed_t<uint32_t> *) (surf->buffer.arrayZ + py * stride);
	ಧ್ರುವ uint8_t *clip_row = clip.alpha.arrayZ + py * clip.stride;
	ಭಿನ್ನ gx = inv_xx * ((ಭಿನ್ನ) ((ಪೂರ್ಣಾಙ್ಕ) clip.min_x + ox) + 0.5f) + inv_xy * ((ಭಿನ್ನ) ((ಪೂರ್ಣಾಙ್ಕ) py + oy) + 0.5f) + inv_x0;
	ಭಿನ್ನ gy = inv_yx * ((ಭಿನ್ನ) ((ಪೂರ್ಣಾಙ್ಕ) clip.min_x + ox) + 0.5f) + inv_yy * ((ಭಿನ್ನ) ((ಪೂರ್ಣಾಙ್ಕ) py + oy) + 0.5f) + inv_y0;
	ಯದಿ (use_lut)
	{
	  ಪ್ರತಿ (unsigned px = clip.min_x; px < clip.max_x; px++)
	  {
	    uint8_t clip_alpha = clip_row[px];
	    ಯದಿ (clip_alpha == 0)
	    {
	      gx += inv_xx;
	      gy += inv_yx;
	      ಅನುವರ್ತ;
	    }
	    ಭಿನ್ನ angle = atan2f (gy - cy, gx - cx);
	    ಯದಿ (angle < 0) angle += (ಭಿನ್ನ) HB_2_PI;
	    ಭಿನ್ನ grad_t = (angle - a0) * inv_angle_range;
	    uint32_t src = lookup_gradient_lut (lut, grad_t, extend);
	    src = hb_raster_alpha_mul (src, clip_alpha);
	    row[px] = hb_packed_t<uint32_t> (hb_raster_src_over (src, (uint32_t) row[px]));
	    gx += inv_xx;
	    gy += inv_yx;
	  }
	}
	ಅನ್ಯಥಾ
	{
	  ಪ್ರತಿ (unsigned px = clip.min_x; px < clip.max_x; px++)
	  {
	    uint8_t clip_alpha = clip_row[px];
	    ಯದಿ (clip_alpha == 0)
	    {
	      gx += inv_xx;
	      gy += inv_yx;
	      ಅನುವರ್ತ;
	    }
	    ಭಿನ್ನ angle = atan2f (gy - cy, gx - cx);
	    ಯದಿ (angle < 0) angle += (ಭಿನ್ನ) HB_2_PI;
	    ಭಿನ್ನ grad_t = (angle - a0) * inv_angle_range;
	    uint32_t src = evaluate_color_ಸಾಲು (stops, len, grad_t, extend);
	    src = hb_raster_alpha_mul (src, clip_alpha);
	    row[px] = hb_packed_t<uint32_t> (hb_raster_src_over (src, (uint32_t) row[px]));
	    gx += inv_xx;
	    gy += inv_yx;
	  }
	}
      }
    }
  }

}

ಸ್ಥಿರ hb_bool_t
hb_raster_paint_custom_palette_color (hb_paint_funcs_t *funcs HB_UNUSED,
				      ಶೂನ್ಯ *paint_data,
				      unsigned ಪೂರ್ಣಾಙ್ಕ color_index,
				      hb_color_t *color,
				      ಶೂನ್ಯ *user_data HB_UNUSED)
{
  hb_raster_paint_t *c = (hb_raster_paint_t *) paint_data;
  ಯದಿ (likely (c->custom_palette && hb_map_has (c->custom_palette, color_index)))
  {
    *color = hb_map_get (c->custom_palette, color_index);
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }
  ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
}

ಸ್ಥಿರ hb_bool_t
hb_raster_paint_set_budget (hb_paint_funcs_t *, ಶೂನ್ಯ *paint_data,
			    int64_t budget, ಶೂನ್ಯ *)
{
  auto *paint = (hb_raster_paint_t *) paint_data;
  paint->budget = budget;
  paint->recharge_budget ();
  ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
}

ಸ್ಥಿರ int64_t
hb_raster_paint_get_budget (hb_paint_funcs_t *, ಶೂನ್ಯ *paint_data, ಶೂನ್ಯ *)
{
  ಪ್ರತ್ಯಾಗಮ ((hb_raster_paint_t *) paint_data)->budget;
}

ಸ್ಥಿರ int64_t *
hb_raster_paint_get_budget_remaining (hb_paint_funcs_t *, ಶೂನ್ಯ *paint_data, ಶೂನ್ಯ *)
{
  ಪ್ರತ್ಯಾಗಮ &((hb_raster_paint_t *) paint_data)->budget_remaining;
}


/*
 * Lazy-loader singleton for paint funcs
 */

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ free_static_raster_paint_funcs ();

ಸ್ಥಿರ ಸಂರಚನಾ hb_raster_paint_funcs_lazy_loader_t : hb_paint_funcs_lazy_loader_t<hb_raster_paint_funcs_lazy_loader_t>
{
  ಸ್ಥಿರ hb_paint_funcs_t *create ()
  {
    hb_paint_funcs_t *funcs = hb_paint_funcs_create ();

    hb_paint_funcs_set_ತಳ್ಳು_transform_func (funcs, hb_raster_paint_ತಳ್ಳು_transform, nullptr, nullptr);
    hb_paint_funcs_set_ತೆಗೆ_transform_func (funcs, hb_raster_paint_ತೆಗೆ_transform, nullptr, nullptr);
    hb_paint_funcs_set_fill_glyph_func (funcs, hb_raster_paint_fill_glyph, nullptr, nullptr);
    hb_paint_funcs_set_color_glyph_func (funcs, hb_raster_paint_color_glyph, nullptr, nullptr);
    hb_paint_funcs_set_ತಳ್ಳು_clip_glyph_func (funcs, hb_raster_paint_ತಳ್ಳು_clip_glyph, nullptr, nullptr);
    hb_paint_funcs_set_ತಳ್ಳು_clip_rectangle_func (funcs, hb_raster_paint_ತಳ್ಳು_clip_rectangle, nullptr, nullptr);
    hb_paint_funcs_set_ತಳ್ಳು_clip_path_start_func (funcs, hb_raster_paint_ತಳ್ಳು_clip_path_start, nullptr, nullptr);
    hb_paint_funcs_set_ತಳ್ಳು_clip_path_end_func (funcs, hb_raster_paint_ತಳ್ಳು_clip_path_end, nullptr, nullptr);
    hb_paint_funcs_set_ತೆಗೆ_clip_func (funcs, hb_raster_paint_ತೆಗೆ_clip, nullptr, nullptr);
    hb_paint_funcs_set_ತಳ್ಳು_group_func (funcs, hb_raster_paint_ತಳ್ಳು_group, nullptr, nullptr);
    hb_paint_funcs_set_ತೆಗೆ_group_func (funcs, hb_raster_paint_ತೆಗೆ_group, nullptr, nullptr);
    hb_paint_funcs_set_color_func (funcs, hb_raster_paint_color, nullptr, nullptr);
    hb_paint_funcs_set_image_func (funcs, hb_raster_paint_image, nullptr, nullptr);
    hb_paint_funcs_set_linear_gradient_func (funcs, hb_raster_paint_linear_gradient, nullptr, nullptr);
    hb_paint_funcs_set_radial_gradient_func (funcs, hb_raster_paint_radial_gradient, nullptr, nullptr);
    hb_paint_funcs_set_sweep_gradient_func (funcs, hb_raster_paint_sweep_gradient, nullptr, nullptr);
    hb_paint_funcs_set_custom_palette_color_func (funcs, hb_raster_paint_custom_palette_color, nullptr, nullptr);
    hb_paint_funcs_set_set_budget_func (funcs, hb_raster_paint_set_budget, nullptr, nullptr);
    hb_paint_funcs_set_get_budget_func (funcs, hb_raster_paint_get_budget, nullptr, nullptr);
    hb_paint_funcs_set_get_budget_remaining_func (funcs, hb_raster_paint_get_budget_remaining, nullptr, nullptr);

    hb_paint_funcs_make_immutable (funcs);

    hb_atexit (free_static_raster_paint_funcs);

    ಪ್ರತ್ಯಾಗಮ funcs;
  }
} static_raster_paint_funcs;

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ
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
hb_raster_paint_create_or_fail (ಶೂನ್ಯ)
{
  hb_raster_paint_t *paint = hb_object_create<hb_raster_paint_t> ();
  ಯದಿ (unlikely (!paint))
    ಪ್ರತ್ಯಾಗಮ nullptr;

  paint->clip_rdr = hb_raster_draw_create_or_fail ();
  ಯದಿ (unlikely (!paint->clip_rdr))
  {
    hb_free (paint);
    ಪ್ರತ್ಯಾಗಮ nullptr;
  }

  ಪ್ರತ್ಯಾಗಮ paint;
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
  ಪ್ರತ್ಯಾಗಮ hb_object_reference (paint);
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
ಶೂನ್ಯ
hb_raster_paint_destroy (hb_raster_paint_t *paint)
{
  ಯದಿ (!hb_object_should_destroy (paint))
    ಪ್ರತ್ಯಾಗಮ;

  hb_map_destroy (paint->custom_palette);
  hb_raster_draw_destroy (paint->clip_rdr);
  ಪ್ರತಿ (auto *s : paint->surface_ರಾಶಿ)
    hb_raster_image_destroy (s);
  ಪ್ರತಿ (auto *s : paint->surface_cache)
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
hb_raster_paint_set_user_data (hb_raster_paint_t  *paint,
			       hb_user_data_ಕೀಲಿ_t *key,
			       ಶೂನ್ಯ               *data,
			       hb_destroy_func_t   destroy,
			       hb_bool_t           replace)
{
  ಪ್ರತ್ಯಾಗಮ hb_object_set_user_data (paint, key, data, destroy, replace);
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
ಶೂನ್ಯ *
hb_raster_paint_get_user_data (ಧ್ರುವ hb_raster_paint_t  *paint,
			       hb_user_data_ಕೀಲಿ_t *key)
{
  ಪ್ರತ್ಯಾಗಮ hb_object_get_user_data (paint, key);
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
ಶೂನ್ಯ
hb_raster_paint_set_transform (hb_raster_paint_t *paint,
			       ಭಿನ್ನ xx, ಭಿನ್ನ yx,
			       ಭಿನ್ನ xy, ಭಿನ್ನ yy,
			       ಭಿನ್ನ dx, ಭಿನ್ನ dy)
{
  paint->base_transform = {xx, yx, xy, yy, dx, dy};
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
ಶೂನ್ಯ
hb_raster_paint_get_transform (ಧ್ರುವ hb_raster_paint_t *paint,
			       ಭಿನ್ನ *xx, ಭಿನ್ನ *yx,
			       ಭಿನ್ನ *xy, ಭಿನ್ನ *yy,
			       ಭಿನ್ನ *dx, ಭಿನ್ನ *dy)
{
  ಯದಿ (xx) *xx = paint->base_transform.xx;
  ಯದಿ (yx) *yx = paint->base_transform.yx;
  ಯದಿ (xy) *xy = paint->base_transform.xy;
  ಯದಿ (yy) *yy = paint->base_transform.yy;
  ಯದಿ (dx) *dx = paint->base_transform.x0;
  ಯದಿ (dy) *dy = paint->base_transform.y0;
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
ಶೂನ್ಯ
hb_raster_paint_set_scale_factor (hb_raster_paint_t *paint,
				  ಭಿನ್ನ x_scale_factor,
				  ಭಿನ್ನ y_scale_factor)
{
  paint->x_scale_factor = x_scale_factor > 0.f ? x_scale_factor : 1.f;
  paint->y_scale_factor = y_scale_factor > 0.f ? y_scale_factor : 1.f;
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
ಶೂನ್ಯ
hb_raster_paint_get_scale_factor (ಧ್ರುವ hb_raster_paint_t *paint,
				  ಭಿನ್ನ *x_scale_factor,
				  ಭಿನ್ನ *y_scale_factor)
{
  ಯದಿ (x_scale_factor) *x_scale_factor = paint->x_scale_factor;
  ಯದಿ (y_scale_factor) *y_scale_factor = paint->y_scale_factor;
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
ಶೂನ್ಯ
hb_raster_paint_set_extents (hb_raster_paint_t         *paint,
			     ಧ್ರುವ hb_raster_extents_t *extents)
{
  paint->fixed_extents = *extents;
  paint->has_extents = ಸತ್ಯ;
  ಯದಿ (paint->fixed_extents.stride == 0)
    paint->fixed_extents.stride = paint->fixed_extents.width * 4;
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
hb_raster_paint_get_extents (ಧ್ರುವ hb_raster_paint_t   *paint,
			     hb_raster_extents_t *extents)
{
  ಯದಿ (!paint->has_extents)
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

  ಯದಿ (extents)
    *extents = paint->fixed_extents;
  ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
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
hb_raster_paint_set_glyph_extents (hb_raster_paint_t        *paint,
				   ಧ್ರುವ hb_glyph_extents_t *glyph_extents)
{
  ಭಿನ್ನ x0 = (ಭಿನ್ನ) glyph_extents->x_bearing;
  ಭಿನ್ನ y0 = (ಭಿನ್ನ) glyph_extents->y_bearing;
  ಭಿನ್ನ x1 = (ಭಿನ್ನ) glyph_extents->x_bearing + glyph_extents->width;
  ಭಿನ್ನ y1 = (ಭಿನ್ನ) glyph_extents->y_bearing + glyph_extents->height;

  ಭಿನ್ನ xmin = hb_min (x0, x1);
  ಭಿನ್ನ xmax = hb_max (x0, x1);
  ಭಿನ್ನ ymin = hb_min (y0, y1);
  ಭಿನ್ನ ymax = hb_max (y0, y1);

  ಭಿನ್ನ px[4] = {xmin, xmin, xmax, xmax};
  ಭಿನ್ನ py[4] = {ymin, ymax, ymin, ymax};

  hb_transform_t<> t = paint->base_transform;
  paint->apply_scale_factor (t);

  ಭಿನ್ನ tx, ty;
  t.transform_point (px[0], py[0]);
  tx = px[0]; ty = py[0];
  ಭಿನ್ನ tx_min = tx, tx_max = tx;
  ಭಿನ್ನ ty_min = ty, ty_max = ty;

  ಪ್ರತಿ (unsigned i = 1; i < 4; i++)
  {
    t.transform_point (px[i], py[i]);
    tx_min = hb_min (tx_min, px[i]);
    tx_max = hb_max (tx_max, px[i]);
    ty_min = hb_min (ty_min, py[i]);
    ty_max = hb_max (ty_max, py[i]);
  }

  int32_t ex0 = hb_clamp_to<int32_t> (floorf (tx_min));
  int32_t ey0 = hb_clamp_to<int32_t> (floorf (ty_min));
  int32_t ex1 = hb_clamp_to<int32_t> (ceilf  (tx_max));
  int32_t ey1 = hb_clamp_to<int32_t> (ceilf  (ty_max));

  ಯದಿ (ex1 <= ex0 || ey1 <= ey0)
  {
    paint->fixed_extents = {};
    paint->has_extents = ಅಸತ್ಯ;
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
  }

  paint->fixed_extents = {
    ex0, ey0,
    (unsigned) hb_min ((int64_t) ex1 - ex0, (int64_t) HB_RASTER_MAX_AUTO_DIMENSION),
    (unsigned) hb_min ((int64_t) ey1 - ey0, (int64_t) HB_RASTER_MAX_AUTO_DIMENSION),
    0
  };
  paint->has_extents = ಸತ್ಯ;
  ಯದಿ (paint->fixed_extents.stride == 0)
    paint->fixed_extents.stride = paint->fixed_extents.width * 4;
  ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
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
ಶೂನ್ಯ
hb_raster_paint_set_foreground (hb_raster_paint_t *paint,
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
hb_raster_paint_get_foreground (ಧ್ರುವ hb_raster_paint_t *paint)
{
  ಪ್ರತ್ಯಾಗಮ paint->foreground;
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
ಶೂನ್ಯ
hb_raster_paint_set_background (hb_raster_paint_t *paint,
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
hb_raster_paint_get_background (ಧ್ರುವ hb_raster_paint_t *paint)
{
  ಪ್ರತ್ಯಾಗಮ paint->background;
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
ಶೂನ್ಯ
hb_raster_paint_set_palette (hb_raster_paint_t *paint,
			     unsigned           palette)
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
unsigned
hb_raster_paint_get_palette (ಧ್ರುವ hb_raster_paint_t *paint)
{
  ಪ್ರತ್ಯಾಗಮ paint->palette;
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
ಶೂನ್ಯ
hb_raster_paint_clear_custom_palette_colors (hb_raster_paint_t *paint)
{
  ಯದಿ (paint->custom_palette)
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
hb_raster_paint_set_custom_palette_color (hb_raster_paint_t *paint,
					  unsigned ಪೂರ್ಣಾಙ್ಕ       color_index,
					  hb_color_t         color)
{
  ಯದಿ (unlikely (!paint->custom_palette))
  {
    paint->custom_palette = hb_map_create ();
    ಯದಿ (unlikely (!paint->custom_palette))
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
  }
  hb_map_set (paint->custom_palette, color_index, color);
  ಪ್ರತ್ಯಾಗಮ hb_map_allocation_successful (paint->custom_palette);
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
hb_raster_paint_get_funcs (ಧ್ರುವ hb_raster_paint_t *paint HB_UNUSED)
{
  ಪ್ರತ್ಯಾಗಮ static_raster_paint_funcs.get_unconst ();
}

ಸ್ಥಿರ hb_bool_t
hb_raster_paint_glyph_impl (hb_raster_paint_t *paint,
			    hb_font_t        *font,
			    hb_codepoint_t    glyph,
			    hb_bool_t         fallible)
{
  ಯದಿ (!paint->has_extents)
  {
    hb_glyph_extents_t ge;
    ಯದಿ (hb_font_get_glyph_extents (font, glyph, &ge))
      hb_raster_paint_set_glyph_extents (paint, &ge);
  }

  hb_paint_funcs_t *funcs = hb_raster_paint_get_funcs (paint);
  hb_paint_ತಳ್ಳು_transform (funcs, paint,
			   paint->base_transform.xx, paint->base_transform.yx,
			   paint->base_transform.xy, paint->base_transform.yy,
			   paint->base_transform.x0, paint->base_transform.y0);

  hb_bool_t ret = ಸತ್ಯ;
  ಯದಿ (fallible)
    ret = hb_font_paint_glyph_or_fail (font, glyph,
				       funcs, paint,
				       paint->palette, paint->foreground);
  ಅನ್ಯಥಾ
    hb_font_paint_glyph (font, glyph,
			 funcs, paint,
			 paint->palette, paint->foreground);

  hb_paint_ತೆಗೆ_transform (funcs, paint);
  ಪ್ರತ್ಯಾಗಮ ret;
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
  ಪ್ರತ್ಯಾಗಮ hb_raster_paint_glyph_impl (paint, font, glyph, ಸತ್ಯ);
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
ಶೂನ್ಯ
hb_raster_paint_glyph (hb_raster_paint_t *paint,
		       hb_font_t        *font,
		       hb_codepoint_t    glyph)
{
  hb_raster_paint_glyph_impl (paint, font, glyph, ಅಸತ್ಯ);
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

  ಯದಿ (unlikely (!paint->has_extents))
    ಪ್ರತ್ಯಾಗಮ nullptr;

  hb_raster_image_t *result;
  ಯದಿ (paint->surface_ರಾಶಿ.length)
  {
    result = paint->surface_ರಾಶಿ[0];
    /* Release any remaining group surfaces (shouldn't happen with
     * well-formed paint calls, but be safe). */
    ಪ್ರತಿ (unsigned i = 1; i < paint->surface_ರಾಶಿ.length; i++)
      paint->release_surface (paint->surface_ರಾಶಿ[i]);
    paint->surface_ರಾಶಿ.clear ();
  }
  ಅನ್ಯಥಾ
  {
    result = paint->acquire_surface ();
    ಯದಿ (unlikely (!result))
      ಪ್ರತ್ಯಾಗಮ nullptr;
  }

  ಪ್ರತ್ಯಾಗಮ result;
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
ಶೂನ್ಯ
hb_raster_paint_clear (hb_raster_paint_t *paint)
{
  paint->fixed_extents = {};
  paint->has_extents = ಅಸತ್ಯ;
  paint->transform_ರಾಶಿ.clear ();
  paint->release_all_clips ();
  ಪ್ರತಿ (auto *s : paint->surface_ರಾಶಿ)
    paint->release_surface (s);
  paint->surface_ರಾಶಿ.clear ();
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
ಶೂನ್ಯ
hb_raster_paint_reset (hb_raster_paint_t *paint)
{
  paint->base_transform = {1, 0, 0, 1, 0, 0};
  paint->x_scale_factor = 1.f;
  paint->y_scale_factor = 1.f;
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
ಶೂನ್ಯ
hb_raster_paint_recycle_image (hb_raster_paint_t  *paint,
			       hb_raster_image_t  *image)
{
  paint->release_surface (image);
}
