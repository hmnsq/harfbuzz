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

#ଅନ୍ତର୍ଭାଵ "hb.hh"

#ଅନ୍ତର୍ଭାଵ "hb-raster-paint.hh"
#ଅନ୍ତର୍ଭାଵ "hb-machinery.hh"
#ଅନ୍ତର୍ଭାଵ "hb-paint.hh"

#ଅନ୍ତର୍ଭାଵ <math.h>


/*
 * Pixel helpers (paint-specific)
 */

/* Convert unpremultiplied hb_color_t (BGRA order) to premultiplied BGRA32 pixel. */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ uint32_t
color_to_premul_pixel (hb_color_t color)
{
  uint8_t a = hb_color_get_alpha (color);
  uint8_t r = hb_raster_div255 (hb_color_get_red (color) * a);
  uint8_t g = hb_raster_div255 (hb_color_get_green (color) * a);
  uint8_t b = hb_raster_div255 (hb_color_get_blue (color) * a);
  ଫେରନ୍ତୁ (uint32_t) b | ((uint32_t) g << 8) | ((uint32_t) r << 16) | ((uint32_t) a << 24);
}

/* Bilinear sample from a premultiplied BGRA32 image. */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ uint32_t
hb_raster_sample_bilinear_premul (ଅଚଳ hb_packed_t<uint32_t> *src,
				  unsigned width,
				  unsigned height,
				  ଦଶମିକ x,
				  ଦଶମିକ y)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା x0 = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) floorf (x);
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା y0 = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) floorf (y);
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା x1 = x0 + 1;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା y1 = y0 + 1;
  ଯଦି (x1 >= (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) width) x1 = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) width - 1;
  ଯଦି (y1 >= (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) height) y1 = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) height - 1;

  ଦଶମିକ tx = x - x0;
  ଦଶମିକ ty = y - y0;
  ଦଶମିକ omtx = 1.f - tx;
  ଦଶମିକ omty = 1.f - ty;

  uint32_t p00 = (uint32_t) src[(size_t) y0 * width + (size_t) x0];
  uint32_t p10 = (uint32_t) src[(size_t) y0 * width + (size_t) x1];
  uint32_t p01 = (uint32_t) src[(size_t) y1 * width + (size_t) x0];
  uint32_t p11 = (uint32_t) src[(size_t) y1 * width + (size_t) x1];

  ଦଶମିକ w00 = omtx * omty;
  ଦଶମିକ w10 = tx * omty;
  ଦଶମିକ w01 = omtx * ty;
  ଦଶମିକ w11 = tx * ty;

  ଦଶମିକ b = ((p00 >>  0) & 0xff) * w00 + ((p10 >>  0) & 0xff) * w10 +
	    ((p01 >>  0) & 0xff) * w01 + ((p11 >>  0) & 0xff) * w11;
  ଦଶମିକ g = ((p00 >>  8) & 0xff) * w00 + ((p10 >>  8) & 0xff) * w10 +
	    ((p01 >>  8) & 0xff) * w01 + ((p11 >>  8) & 0xff) * w11;
  ଦଶମିକ r = ((p00 >> 16) & 0xff) * w00 + ((p10 >> 16) & 0xff) * w10 +
	    ((p01 >> 16) & 0xff) * w01 + ((p11 >> 16) & 0xff) * w11;
  ଦଶମିକ a = ((p00 >> 24) & 0xff) * w00 + ((p10 >> 24) & 0xff) * w10 +
	    ((p01 >> 24) & 0xff) * w01 + ((p11 >> 24) & 0xff) * w11;

  ଫେରନ୍ତୁ (uint32_t) (b + 0.5f)
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
ସ୍ଥିର ଶୂନ୍ୟ
ensure_initialized (hb_raster_paint_t *c)
{
  ଯଦି (c->surface_ଗଦା.length) ଫେରନ୍ତୁ;

  /* A failed stack push (OOM) leaves the vector in a sticky error
   * state, so initialization can never succeed; bail out cheaply
   * instead of re-paying the surface acquire-and-clear below on
   * every subsequent paint callback. */
  ଯଦି (unlikely (c->surface_ଗଦା.in_error () || c->clip_ଗଦା.in_error ()))
    ଫେରନ୍ତୁ;

  /* acquire_surface() clears a full surface; charge its area, so
   * repeated failed initialization attempts cannot re-pay it
   * indefinitely.  Successful initialization re-arms the session
   * budget below, making the charge a no-op for normal sessions. */
  ଯଦି (unlikely (!c->charge_work ((int64_t) c->fixed_extents.width *
				 c->fixed_extents.height)))
    ଫେରନ୍ତୁ;

  /* Root surface */
  hb_raster_image_t *ମୂଳ = c->acquire_surface ();
  ଯଦି (unlikely (!ମୂଳ)) ଫେରନ୍ତୁ;

  ଯଦି (hb_color_get_alpha (c->background))
  {
    uint32_t bg = color_to_premul_pixel (c->background);
    hb_raster_extents_t ext;
    hb_raster_image_get_extents (ମୂଳ, &ext);
    ଅଚଳ uint8_t *buf = hb_raster_image_get_buffer (ମୂଳ);
    ପାଇଁ (unsigned y = 0; y < ext.height; y++)
    {
      hb_packed_t<uint32_t> *row =
	(hb_packed_t<uint32_t> *) (buf + (size_t) y * ext.stride);
      ପାଇଁ (unsigned x = 0; x < ext.width; x++)
	row[x] = hb_packed_t<uint32_t> (bg);
    }
  }

  ଯଦି (unlikely (!c->surface_ଗଦା.ଠେଲ_or_fail (ମୂଳ)))
  {
    c->release_surface (ମୂଳ);
    ଫେରନ୍ତୁ;
  }

  /* Resolve the surface-sized pixel budget now that the surface size is
   * known.  An unlimited policy disables the pixel budget, so leave it.
   * The outline budget is re-resolved to the same value it already holds
   * (nothing has been drawn yet). */
  ଯଦି (c->budget != HB_BUDGET_UNLIMITED)
    c->recharge_budget ();

  /* Initial clip: full coverage rectangle */
  hb_raster_clip_t clip;
  clip.init_full (c->fixed_extents.width, c->fixed_extents.height);
  ଯଦି (unlikely (!c->clip_ଗଦା.ଠେଲ_or_fail (std::move (clip))))
  {
    c->release_surface (c->surface_ଗଦା.କାଢ଼ ());
    ଫେରନ୍ତୁ;
  }
}

ସ୍ଥିର ଶୂନ୍ୟ
hb_raster_paint_ଠେଲ_transform (hb_paint_funcs_t *pfuncs HB_UNUSED,
				ଶୂନ୍ୟ *paint_data,
				ଦଶମିକ xx, ଦଶମିକ yx,
				ଦଶମିକ xy, ଦଶମିକ yy,
				ଦଶମିକ dx, ଦଶମିକ dy,
				ଶୂନ୍ୟ *user_data HB_UNUSED)
{
  hb_raster_paint_t *c = (hb_raster_paint_t *) paint_data;

  ensure_initialized (c);

  hb_transform_t<> t = c->current_transform ();
  t.multiply ({xx, yx, xy, yy, dx, dy});
  (ଶୂନ୍ୟ) c->transform_ଗଦା.push (t);
}

ସ୍ଥିର ଶୂନ୍ୟ
hb_raster_paint_କାଢ଼_transform (hb_paint_funcs_t *pfuncs HB_UNUSED,
			       ଶୂନ୍ୟ *paint_data,
			       ଶୂନ୍ୟ *user_data HB_UNUSED)
{
  hb_raster_paint_t *c = (hb_raster_paint_t *) paint_data;
  ଯଦି (c->transform_ଗଦା.length)
    c->transform_ଗଦା.କାଢ଼ ();
}

ସ୍ଥିର hb_bool_t
hb_raster_paint_color_glyph (hb_paint_funcs_t *pfuncs HB_UNUSED,
			     ଶୂନ୍ୟ *paint_data HB_UNUSED,
			     hb_codepoint_t glyph HB_UNUSED,
			     hb_font_t *font HB_UNUSED,
			     ଶୂନ୍ୟ *user_data HB_UNUSED)
{
  ଫେରନ୍ତୁ ମିଥ୍ୟା;
}

ପ୍ରକାରବ୍ୟାଖ୍ୟା ଶୂନ୍ୟ (*hb_raster_paint_clip_mask_emit_t) (hb_raster_draw_t *rdr, ଶୂନ୍ୟ *user_data);

/* Attach the surface box to @rdr and seed its outline budget before
 * outlines are drawn into it: curves outside the surface collapse to
 * their chord, and outline traversal and curve flattening charge the
 * seeded budget.  hb_raster_paint_readback_budget() pulls the remainder
 * back into the session afterwards. */
ସ୍ଥିର ଶୂନ୍ୟ
hb_raster_paint_attach_clip_rdr (hb_raster_paint_t *c,
				 hb_raster_draw_t *rdr,
				 ଅଚଳ hb_raster_image_t *surf)
{
  hb_raster_draw_set_clip_box (rdr,
			       (ଦଶମିକ) surf->extents.x_origin,
			       (ଦଶମିକ) surf->extents.y_origin,
			       (ଦଶମିକ) surf->extents.x_origin + (ଦଶମିକ) surf->extents.width,
			       (ଦଶମିକ) surf->extents.y_origin + (ଦଶମିକ) surf->extents.height);
  hb_draw_set_budget (hb_raster_draw_get_funcs (rdr), rdr, c->budget_remaining);
}

/* Pull the outline budget the rasterizer has consumed back into the
 * session, so it aggregates across every glyph in the paint walk. */
ସ୍ଥିର ଶୂନ୍ୟ
hb_raster_paint_readback_budget (hb_raster_paint_t *c,
				 hb_raster_draw_t *rdr)
{
  c->budget_remaining = hb_draw_get_budget_remaining (hb_raster_draw_get_funcs (rdr), rdr);
}

ସ୍ଥିର ଶୂନ୍ୟ
hb_raster_paint_ଠେଲ_empty_clip (hb_raster_paint_t *c, unsigned w, unsigned h)
{
  hb_raster_clip_t new_clip = c->acquire_clip (w, h);
  new_clip.init_full (w, h);
  new_clip.is_rect = ସତ୍ୟ;
  new_clip.rect_x0 = new_clip.rect_y0 = 0;
  new_clip.rect_x1 = new_clip.rect_y1 = 0;
  new_clip.min_x = new_clip.min_y = new_clip.max_x = new_clip.max_y = 0;
  (ଶୂନ୍ୟ) c->clip_ଗଦା.push (std::move (new_clip));
}

ସ୍ଥିର ତାର୍କିକ
hb_raster_paint_intersect_mask (ଅଚଳ hb_raster_image_t *surf,
				ଅଚଳ hb_raster_extents_t &mask_ext,
				ଅଚଳ hb_raster_clip_t &clip,
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

  ଯଦି (x0 >= x1 || y0 >= y1) ଫେରନ୍ତୁ ମିଥ୍ୟା;

  *mask_x0 = mx0;
  *mask_y0 = my0;
  *ix0 = (unsigned) x0;
  *iy0 = (unsigned) y0;
  *ix1 = (unsigned) x1;
  *iy1 = (unsigned) y1;
  ଫେରନ୍ତୁ ସତ୍ୟ;
}

/* Render whatever edges have been accumulated into @rdr and
 * push the result as a new clip on the stack, intersecting
 * with the existing clip.  Used by the emitter-based clip
 * helpers (push_clip_glyph) and by push_clip_path_end once
 * the caller has drawn the path into @rdr. */
ସ୍ଥିର ଶୂନ୍ୟ
hb_raster_paint_finalize_path_clip (hb_raster_paint_t *c,
				    hb_raster_draw_t *rdr,
				    hb_raster_image_t *surf,
				    unsigned w, unsigned h)
{
  ଯଦି (unlikely (!c->precharge_work (hb_raster_draw_get_pixel_work (rdr, h, w))))
  {
    hb_raster_draw_clear (rdr);
    hb_raster_paint_ଠେଲ_empty_clip (c, w, h);
    ଫେରନ୍ତୁ;
  }

  hb_raster_image_t *mask_img = hb_raster_draw_render (rdr);

  ଯଦି (unlikely (!mask_img))
  {
    hb_raster_paint_ଠେଲ_empty_clip (c, w, h);
    ଫେରନ୍ତୁ;
  }

  /* Charge the mask render (its own area; may exceed the surface) plus
   * the clip-buffer pass below. */
  hb_raster_extents_t mask_render_ext;
  hb_raster_image_get_extents (mask_img, &mask_render_ext);
  ଯଦି (unlikely (!c->charge_work ((int64_t) mask_render_ext.width * mask_render_ext.height +
				 (int64_t) w * h)))
  {
    hb_raster_draw_recycle_image (rdr, mask_img);
    hb_raster_paint_ଠେଲ_empty_clip (c, w, h);
    ଫେରନ୍ତୁ;
  }

  hb_raster_clip_t new_clip = c->acquire_clip (w, h);

  /* Allocate alpha buffer and intersect with previous clip */
  size_t clip_size = (size_t) new_clip.stride * h;
  ଯଦି (unlikely (clip_size > HB_RASTER_MAX_BUFFER_SIZE ||
                !new_clip.alpha.resize ((unsigned) clip_size)))
  {
    hb_raster_draw_recycle_image (rdr, mask_img);
    hb_raster_paint_ଠେଲ_empty_clip (c, w, h);
    ଫେରନ୍ତୁ;
  }
  hb_memset (new_clip.alpha.arrayZ, 0, (unsigned) clip_size);

  ଅଚଳ uint8_t *mask_buf = hb_raster_image_get_buffer (mask_img);
  hb_raster_extents_t mask_ext;
  hb_raster_image_get_extents (mask_img, &mask_ext);
  ଅଚଳ hb_raster_clip_t &old_clip = c->current_clip ();
  ଯଦି (unlikely (!old_clip.has_valid_alpha_mask ()))
  {
    hb_raster_draw_recycle_image (rdr, mask_img);
    hb_raster_paint_ଠେଲ_empty_clip (c, w, h);
    ଫେରନ୍ତୁ;
  }

  int64_t mask_x0, mask_y0;
  unsigned ix0, iy0, ix1, iy1;
  ଯଦି (!hb_raster_paint_intersect_mask (surf, mask_ext, old_clip, w, h,
				       &mask_x0, &mask_y0,
				       &ix0, &iy0, &ix1, &iy1))
  {
    hb_raster_draw_recycle_image (rdr, mask_img);
    hb_raster_paint_ଠେଲ_empty_clip (c, w, h);
    ଫେରନ୍ତୁ;
  }

  new_clip.min_x = w; new_clip.min_y = h;
  new_clip.max_x = 0; new_clip.max_y = 0;

  ଯଦି (old_clip.is_rect)
  {
    ପାଇଁ (unsigned y = iy0; y < iy1; y++)
    {
      ଅଚଳ uint8_t *mask_row = mask_buf + (unsigned) ((int64_t) y - mask_y0) * mask_ext.stride;
      uint8_t *out_row = new_clip.alpha.arrayZ + y * new_clip.stride;
      unsigned row_min = ix1;
      unsigned row_max = ix0;
      unsigned mx = (unsigned) ((int64_t) ix0 - mask_x0);
      ପାଇଁ (unsigned x = ix0; x < ix1; x++)
      {
	uint8_t a = mask_row[mx++];
	out_row[x] = a;
	ଯଦି (a && row_min == ix1)
	{
	  row_min = x;
	  row_max = x + 1;
	}
	ଅନ୍ୟଥା ଯଦି (a)
	  row_max = x + 1;
      }
      ଯଦି (row_min < row_max)
      {
	new_clip.min_x = hb_min (new_clip.min_x, row_min);
	new_clip.min_y = hb_min (new_clip.min_y, y);
	new_clip.max_x = hb_max (new_clip.max_x, row_max);
	new_clip.max_y = hb_max (new_clip.max_y, y + 1);
      }
    }
  }
  ଅନ୍ୟଥା
  {
    ପାଇଁ (unsigned y = iy0; y < iy1; y++)
    {
      ଅଚଳ uint8_t *old_row = old_clip.alpha.arrayZ + y * old_clip.stride;
      ଅଚଳ uint8_t *mask_row = mask_buf + (unsigned) ((int64_t) y - mask_y0) * mask_ext.stride;
      uint8_t *out_row = new_clip.alpha.arrayZ + y * new_clip.stride;
      unsigned row_min = ix1;
      unsigned row_max = ix0;
      ପାଇଁ (unsigned x = ix0; x < ix1; x++)
      {
	unsigned mx = (unsigned) ((int64_t) x - mask_x0);
	uint8_t a = hb_raster_div255 (mask_row[mx] * old_row[x]);
	out_row[x] = a;
	ଯଦି (a)
	{
	  row_min = hb_min (row_min, x);
	  row_max = x + 1;
	}
      }
      ଯଦି (row_min < row_max)
      {
	new_clip.min_x = hb_min (new_clip.min_x, row_min);
	new_clip.min_y = hb_min (new_clip.min_y, y);
	new_clip.max_x = hb_max (new_clip.max_x, row_max);
	new_clip.max_y = hb_max (new_clip.max_y, y + 1);
      }
    }
  }

  hb_raster_draw_recycle_image (rdr, mask_img);
  ଯଦି (unlikely (!c->clip_ଗଦା.ଠେଲ_or_fail (std::move (new_clip))))
    hb_raster_paint_ଠେଲ_empty_clip (c, w, h);
}

ସ୍ଥିର ଶୂନ୍ୟ
hb_raster_paint_ଠେଲ_clip_from_emitter (hb_raster_paint_t *c,
					hb_raster_paint_clip_mask_emit_t emit,
					ଶୂନ୍ୟ *emit_data)
{
  ensure_initialized (c);

  hb_raster_image_t *surf = c->current_surface ();
  ଯଦି (unlikely (!surf)) ଫେରନ୍ତୁ;

  unsigned w = surf->extents.width;
  unsigned h = surf->extents.height;

  /* Out of budget: skip the glyph-outline extraction entirely, so
   * per-glyph outline limits cannot multiply with the caller's
   * paint-graph traversal limits. */
  ଯଦି (unlikely (c->budget_remaining < 0))
  {
    hb_raster_paint_ଠେଲ_empty_clip (c, w, h);
    ଫେରନ୍ତୁ;
  }

  hb_raster_draw_t *rdr = c->clip_rdr;
  hb_transform_t<> t = c->current_effective_transform ();
  hb_raster_draw_set_transform (rdr, t.xx, t.yx, t.xy, t.yy, t.x0, t.y0);
  hb_raster_paint_attach_clip_rdr (c, rdr, surf);
  emit (rdr, emit_data);
  hb_raster_paint_readback_budget (c, rdr);

  hb_raster_paint_finalize_path_clip (c, rdr, surf, w, h);
}

ସଂରଚନା hb_raster_paint_glyph_clip_data_t
{
  hb_codepoint_t glyph;
  hb_font_t *font;
};

ସ୍ଥିର ଶୂନ୍ୟ
hb_raster_paint_emit_clip_glyph_mask (hb_raster_draw_t *rdr, ଶୂନ୍ୟ *user_data)
{
  hb_raster_paint_glyph_clip_data_t *data = (hb_raster_paint_glyph_clip_data_t *) user_data;
  /* Let draw-render choose tight glyph extents; we map by mask origin below. */
  hb_font_draw_glyph (data->font, data->glyph, hb_raster_draw_get_funcs (rdr), rdr);
}

ସ୍ଥିର ଶୂନ୍ୟ
hb_raster_paint_ଠେଲ_clip_glyph (hb_paint_funcs_t *pfuncs HB_UNUSED,
				 ଶୂନ୍ୟ *paint_data,
				 hb_codepoint_t glyph,
				 hb_font_t *font,
				 ଶୂନ୍ୟ *user_data HB_UNUSED)
{
  hb_raster_paint_t *c = (hb_raster_paint_t *) paint_data;
  hb_raster_paint_glyph_clip_data_t data = {glyph, font};
  hb_raster_paint_ଠେଲ_clip_from_emitter (c, hb_raster_paint_emit_clip_glyph_mask, &data);
}

ସ୍ଥିର ଶୂନ୍ୟ
hb_raster_paint_ଠେଲ_clip_rectangle (hb_paint_funcs_t *pfuncs HB_UNUSED,
				     ଶୂନ୍ୟ *paint_data,
				     ଦଶମିକ xmin, ଦଶମିକ ymin,
				     ଦଶମିକ xmax, ଦଶମିକ ymax,
				     ଶୂନ୍ୟ *user_data HB_UNUSED)
{
  hb_raster_paint_t *c = (hb_raster_paint_t *) paint_data;

  ensure_initialized (c);

  ଯଦି (!c->surface_ଗଦା.length) ଫେରନ୍ତୁ;

  hb_transform_t<> t = c->current_effective_transform ();

  hb_raster_image_t *surf = c->current_surface ();
  ଯଦି (unlikely (!surf)) ଫେରନ୍ତୁ;

  unsigned w = surf->extents.width;
  unsigned h = surf->extents.height;
  ତାର୍କିକ is_axis_aligned = (t.xy == 0.f && t.yx == 0.f);

  /* Transform the four corners to pixel space */
  ଦଶମିକ cx[4], cy[4];
  cx[0] = xmin; cy[0] = ymin;
  cx[1] = xmax; cy[1] = ymin;
  cx[2] = xmax; cy[2] = ymax;
  cx[3] = xmin; cy[3] = ymax;
  ପାଇଁ (unsigned i = 0; i < 4; i++)
    t.transform_point (cx[i], cy[i]);

  /* Compute bounding box in pixel coords */
  ଦଶମିକ fmin_x = cx[0], fmin_y = cy[0], fmax_x = cx[0], fmax_y = cy[0];
  ପାଇଁ (unsigned i = 1; i < 4; i++)
  {
    fmin_x = hb_min (fmin_x, cx[i]); fmin_y = hb_min (fmin_y, cy[i]);
    fmax_x = hb_max (fmax_x, cx[i]); fmax_y = hb_max (fmax_y, cy[i]);
  }

  /* A spec-legal but very large transform can push these bounds past the int
     range or to non-finite values; a bare float->int conversion would then be
     undefined.  Saturate through hb_clamp_to<> (which also maps NaN to
     INT32_MIN); the subtraction is done in double to avoid overflow. */
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା px0 = hb_clamp_to<int32_t> ((ଦ୍ୱିଗୁଣ) floorf (fmin_x) - surf->extents.x_origin);
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା py0 = hb_clamp_to<int32_t> ((ଦ୍ୱିଗୁଣ) floorf (fmin_y) - surf->extents.y_origin);
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା px1 = hb_clamp_to<int32_t> ((ଦ୍ୱିଗୁଣ) ceilf  (fmax_x) - surf->extents.x_origin);
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା py1 = hb_clamp_to<int32_t> ((ଦ୍ୱିଗୁଣ) ceilf  (fmax_y) - surf->extents.y_origin);

  /* Clamp to surface bounds */
  px0 = hb_max (px0, 0);
  py0 = hb_max (py0, 0);
  px1 = hb_min (px1, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) w);
  py1 = hb_min (py1, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) h);

  ଅଚଳ hb_raster_clip_t &old_clip = c->current_clip ();
  ଯଦି (unlikely (!old_clip.has_valid_alpha_mask ()))
  {
    hb_raster_paint_ଠେଲ_empty_clip (c, w, h);
    ଫେରନ୍ତୁ;
  }

  /* All paths below except rect-on-rect run a full clip-buffer pass. */
  ଯଦି (!(is_axis_aligned && old_clip.is_rect) &&
      unlikely (!c->charge_work ((int64_t) w * h)))
  {
    hb_raster_paint_ଠେଲ_empty_clip (c, w, h);
    ଫେରନ୍ତୁ;
  }

  hb_raster_clip_t new_clip = c->acquire_clip (w, h);

  ଯଦି (is_axis_aligned && old_clip.is_rect)
  {
    /* Fast path: axis-aligned rect-on-rect intersection */
    new_clip.is_rect = ସତ୍ୟ;
    new_clip.rect_x0 = hb_max (px0, old_clip.rect_x0);
    new_clip.rect_y0 = hb_max (py0, old_clip.rect_y0);
    new_clip.rect_x1 = hb_min (px1, old_clip.rect_x1);
    new_clip.rect_y1 = hb_min (py1, old_clip.rect_y1);
    new_clip.update_bounds_from_rect ();
  }
  ଅନ୍ୟଥା ଯଦି (is_axis_aligned)
  {
    /* Fast path: intersect an axis-aligned rect with an existing alpha clip. */
    new_clip.is_rect = ମିଥ୍ୟା;
    size_t clip_size = (size_t) new_clip.stride * h;
    ଯଦି (unlikely (clip_size > HB_RASTER_MAX_BUFFER_SIZE ||
                  !new_clip.alpha.resize ((unsigned) clip_size)))
    {
      hb_raster_paint_ଠେଲ_empty_clip (c, w, h);
      ଫେରନ୍ତୁ;
    }
    hb_memset (new_clip.alpha.arrayZ, 0, (unsigned) clip_size);

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା iy0_i = hb_max (py0, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) old_clip.min_y);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା iy1_i = hb_min (py1, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) old_clip.max_y);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ix0_i = hb_max (px0, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) old_clip.min_x);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ix1_i = hb_min (px1, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) old_clip.max_x);

    ଯଦି (ix0_i < ix1_i && iy0_i < iy1_i)
    {
      unsigned iy0 = (unsigned) iy0_i;
      unsigned iy1 = (unsigned) iy1_i;
      unsigned ix0 = (unsigned) ix0_i;
      unsigned ix1 = (unsigned) ix1_i;

      ପାଇଁ (unsigned y = iy0; y < iy1; y++)
      {
        ଅଚଳ uint8_t *old_row = old_clip.alpha.arrayZ + y * old_clip.stride;
        uint8_t *new_row = new_clip.alpha.arrayZ + y * new_clip.stride;
        hb_memcpy (new_row + ix0, old_row + ix0, ix1 - ix0);
      }
      new_clip.min_x = ix0;
      new_clip.min_y = iy0;
      new_clip.max_x = ix1;
      new_clip.max_y = iy1;
    }
  }
  ଅନ୍ୟଥା
  {
    /* General case: rasterize transformed quad as alpha mask */
    new_clip.is_rect = ମିଥ୍ୟା;
    size_t clip_size = (size_t) new_clip.stride * h;
    ଯଦି (unlikely (clip_size > HB_RASTER_MAX_BUFFER_SIZE ||
                  !new_clip.alpha.resize ((unsigned) clip_size)))
    {
      hb_raster_paint_ଠେଲ_empty_clip (c, w, h);
      ଫେରନ୍ତୁ;
    }
    hb_memset (new_clip.alpha.arrayZ, 0, (unsigned) clip_size);

    /* Convert quad corners to pixel-relative coords */
    ଦଶମିକ qx[4], qy[4];
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ox = surf->extents.x_origin;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା oy = surf->extents.y_origin;
    ପାଇଁ (unsigned i = 0; i < 4; i++)
    {
      qx[i] = cx[i] - ox;
      qy[i] = cy[i] - oy;
    }

    /* For each pixel in the bounding box, test if inside the quad
     * using cross-product edge tests (winding order). */
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା iy0_i = hb_max (py0, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) old_clip.min_y);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା iy1_i = hb_min (py1, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) old_clip.max_y);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ix0_i = hb_max (px0, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) old_clip.min_x);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ix1_i = hb_min (px1, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) old_clip.max_x);
    new_clip.min_x = w; new_clip.min_y = h;
    new_clip.max_x = 0; new_clip.max_y = 0;

    ଯଦି (ix0_i < ix1_i && iy0_i < iy1_i)
    {
      unsigned iy0 = (unsigned) iy0_i;
      unsigned iy1 = (unsigned) iy1_i;
      unsigned ix0 = (unsigned) ix0_i;
      unsigned ix1 = (unsigned) ix1_i;

      /* Precompute edge normals for point-in-quad test.
       * Edge i goes from corner i to corner (i+1)%4.
       * Normal = (dy, -dx); inside test: dot(normal, p-corner) >= 0 */
	      ଦଶମିକ enx[4], eny[4], ed[4];
	      ପାଇଁ (unsigned i = 0; i < 4; i++)
	      {
		unsigned j = (i + 1) & 3;
		ଦଶମିକ edx = qx[j] - qx[i], edy = qy[j] - qy[i];
		enx[i] = edy;       /* normal x */
		eny[i] = -edx;      /* normal y */
		ed[i] = enx[i] * qx[i] + eny[i] * qy[i]; /* distance threshold */
	      }
	      ଦଶମିକ area2 = 0.f;
	      ପାଇଁ (unsigned i = 0; i < 4; i++)
	      {
		unsigned j = (i + 1) & 3;
		area2 += qx[i] * qy[j] - qx[j] * qy[i];
	      }
	      ତାର୍କିକ ccw = area2 >= 0.f;

      ଯଦି (old_clip.is_rect)
      {
	ପାଇଁ (unsigned y = iy0; y < iy1; y++)
	  ପାଇଁ (unsigned x = ix0; x < ix1; x++)
	  {
	    ଦଶମିକ px_f = x + 0.5f, py_f = y + 0.5f;
	    /* Test if pixel center is inside the quad */
		    ତାର୍କିକ inside = ସତ୍ୟ;
		    ପାଇଁ (unsigned i = 0; i < 4; i++)
		    {
		      ଦଶମିକ d = enx[i] * px_f + eny[i] * py_f;
		      ଯଦି (ccw ? d < ed[i] : d > ed[i])
		      {
			inside = ମିଥ୍ୟା;
			ଭାଙ୍ଗ;
		      }
		    }
	    uint8_t a = inside ? 255 : 0;
	    new_clip.alpha[y * new_clip.stride + x] = a;
	    ଯଦି (a)
	    {
	      new_clip.min_x = hb_min (new_clip.min_x, x);
	      new_clip.min_y = hb_min (new_clip.min_y, y);
	      new_clip.max_x = hb_max (new_clip.max_x, x + 1);
	      new_clip.max_y = hb_max (new_clip.max_y, y + 1);
	    }
	  }
      }
      ଅନ୍ୟଥା
      {
	ପାଇଁ (unsigned y = iy0; y < iy1; y++)
	{
	  ଅଚଳ uint8_t *old_row = old_clip.alpha.arrayZ + y * old_clip.stride;
	  ପାଇଁ (unsigned x = ix0; x < ix1; x++)
	  {
	    ଦଶମିକ px_f = x + 0.5f, py_f = y + 0.5f;
	    /* Test if pixel center is inside the quad */
		    ତାର୍କିକ inside = ସତ୍ୟ;
		    ପାଇଁ (unsigned i = 0; i < 4; i++)
		    {
		      ଦଶମିକ d = enx[i] * px_f + eny[i] * py_f;
		      ଯଦି (ccw ? d < ed[i] : d > ed[i])
		      {
			inside = ମିଥ୍ୟା;
			ଭାଙ୍ଗ;
		      }
		    }
	    uint8_t a = inside ? old_row[x] : 0;
	    new_clip.alpha[y * new_clip.stride + x] = a;
	    ଯଦି (a)
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

  ଯଦି (unlikely (!c->clip_ଗଦା.ଠେଲ_or_fail (std::move (new_clip))))
    hb_raster_paint_ଠେଲ_empty_clip (c, surf->extents.width, surf->extents.height);
}

ସ୍ଥିର hb_draw_funcs_t *
hb_raster_paint_ଠେଲ_clip_path_start (hb_paint_funcs_t *pfuncs HB_UNUSED,
				      ଶୂନ୍ୟ *paint_data,
				      ଶୂନ୍ୟ **draw_data,
				      ଶୂନ୍ୟ *user_data HB_UNUSED)
{
  hb_raster_paint_t *c = (hb_raster_paint_t *) paint_data;

  ensure_initialized (c);

  /* Prime clip_rdr with the current effective transform; the
   * caller then drives hb_draw_*() into it, and _end renders
   * the accumulated edges into a mask and intersects with the
   * current clip. */
  ଯଦି (unlikely (!c->surface_ଗଦା.length || !c->clip_rdr))
  {
    *draw_data = nullptr;
    ଫେରନ୍ତୁ nullptr;
  }

  hb_raster_draw_t *rdr = c->clip_rdr;
  hb_transform_t<> t = c->current_effective_transform ();
  hb_raster_draw_set_transform (rdr, t.xx, t.yx, t.xy, t.yy, t.x0, t.y0);
  hb_raster_image_t *surf = c->current_surface ();
  ଯଦି (likely (surf))
    hb_raster_paint_attach_clip_rdr (c, rdr, surf);

  *draw_data = rdr;
  ଫେରନ୍ତୁ hb_raster_draw_get_funcs (rdr);
}

ସ୍ଥିର ଶୂନ୍ୟ
hb_raster_paint_ଠେଲ_clip_path_end (hb_paint_funcs_t *pfuncs HB_UNUSED,
				    ଶୂନ୍ୟ *paint_data,
				    ଶୂନ୍ୟ *user_data HB_UNUSED)
{
  hb_raster_paint_t *c = (hb_raster_paint_t *) paint_data;

  hb_raster_image_t *surf = c->current_surface ();
  ଯଦି (unlikely (!surf || !c->clip_rdr)) ଫେରନ୍ତୁ;

  unsigned w = surf->extents.width;
  unsigned h = surf->extents.height;

  hb_raster_paint_readback_budget (c, c->clip_rdr);
  ଯଦି (unlikely (c->budget_remaining < 0))
  {
    hb_raster_draw_clear (c->clip_rdr);
    hb_raster_paint_ଠେଲ_empty_clip (c, w, h);
    ଫେରନ୍ତୁ;
  }

  hb_raster_paint_finalize_path_clip (c, c->clip_rdr, surf, w, h);
}

ସ୍ଥିର ଶୂନ୍ୟ
hb_raster_paint_କାଢ଼_clip (hb_paint_funcs_t *pfuncs HB_UNUSED,
			  ଶୂନ୍ୟ *paint_data,
			  ଶୂନ୍ୟ *user_data HB_UNUSED)
{
  hb_raster_paint_t *c = (hb_raster_paint_t *) paint_data;
  ଯଦି (!c->clip_ଗଦା.length) ଫେରନ୍ତୁ;
  c->release_clip (c->clip_ଗଦା.କାଢ଼ ());
}

ସ୍ଥିର ଶୂନ୍ୟ
hb_raster_paint_ଠେଲ_group (hb_paint_funcs_t *pfuncs HB_UNUSED,
			    ଶୂନ୍ୟ *paint_data,
			    ଶୂନ୍ୟ *user_data HB_UNUSED)
{
  hb_raster_paint_t *c = (hb_raster_paint_t *) paint_data;

  ensure_initialized (c);

  /* acquire_surface() clears a full surface; charge its area. */
  ଯଦି (unlikely (!c->charge_work ((int64_t) c->fixed_extents.width * c->fixed_extents.height)))
    ଫେରନ୍ତୁ;

  hb_raster_image_t *new_surf = c->acquire_surface ();
  ଯଦି (unlikely (!new_surf)) ଫେରନ୍ତୁ;
  ଯଦି (unlikely (!c->surface_ଗଦା.ଠେଲ_or_fail (new_surf)))
    c->release_surface (new_surf);
}

ସ୍ଥିର ଶୂନ୍ୟ
hb_raster_paint_କାଢ଼_group (hb_paint_funcs_t *pfuncs HB_UNUSED,
			   ଶୂନ୍ୟ *paint_data,
			   hb_paint_composite_mode_t mode,
			   ଶୂନ୍ୟ *user_data HB_UNUSED)
{
  hb_raster_paint_t *c = (hb_raster_paint_t *) paint_data;

  ଯଦି (c->surface_ଗଦା.length < 2) ଫେରନ୍ତୁ;

  hb_raster_image_t *src = c->surface_ଗଦା.କାଢ଼ ();
  hb_raster_image_t *dst = c->current_surface ();

  ଯଦି (dst && src &&
      likely (c->charge_work ((int64_t) dst->extents.width * dst->extents.height)))
    hb_raster_image_composite (dst, src, mode);

  c->release_surface (src);
}

ସ୍ଥିର ଶୂନ୍ୟ
hb_raster_paint_solid (hb_raster_paint_t *c,
		       hb_raster_image_t *surf,
		       hb_color_t color,
		       hb_raster_image_t *mask_img)
{
  uint32_t premul = color_to_premul_pixel (color);
  uint8_t premul_a = (uint8_t) (premul >> 24);
  ଅଚଳ hb_raster_clip_t &clip = c->current_clip ();

  unsigned stride = surf->extents.stride;
  ଯଦି (clip.min_x >= clip.max_x || clip.min_y >= clip.max_y) ଫେରନ୍ତୁ;
  ଯଦି (premul_a == 0) ଫେରନ୍ତୁ;

  unsigned ix0 = clip.min_x, iy0 = clip.min_y;
  unsigned ix1 = clip.max_x, iy1 = clip.max_y;
  ଅଚଳ uint8_t *mask_buf = nullptr;
  hb_raster_extents_t mask_ext;
  int64_t mask_x0 = 0, mask_y0 = 0;
  ଯଦି (mask_img)
  {
    mask_buf = hb_raster_image_get_buffer (mask_img);
    hb_raster_image_get_extents (mask_img, &mask_ext);
    ଯଦି (!hb_raster_paint_intersect_mask (surf, mask_ext, clip,
					 surf->extents.width, surf->extents.height,
					 &mask_x0, &mask_y0,
					 &ix0, &iy0, &ix1, &iy1))
      ଫେରନ୍ତୁ;
  }

  ଯଦି (unlikely (!c->charge_work ((int64_t) (ix1 - ix0) * (iy1 - iy0))))
    ଫେରନ୍ତୁ;

  ଯଦି (likely (!clip.is_rect))
  {
    ପାଇଁ (unsigned y = iy0; y < iy1; y++)
    {
      hb_packed_t<uint32_t> *__restrict row = (hb_packed_t<uint32_t> *) (surf->buffer.arrayZ + y * stride);
      ଅଚଳ uint8_t *__restrict clip_row = clip.alpha.arrayZ + y * clip.stride;
      ଅଚଳ uint8_t *__restrict mask_row = mask_buf ? mask_buf + (unsigned) ((int64_t) y - mask_y0) * mask_ext.stride
						    : nullptr;
      unsigned mx = (unsigned) ((int64_t) ix0 - mask_x0);
      ପାଇଁ (unsigned x = ix0; x < ix1; x++)
      {
	uint8_t clip_alpha = clip_row[x];
	ଯଦି (mask_row)
	  clip_alpha = hb_raster_div255 (mask_row[mx++] * clip_alpha);
	ଯଦି (clip_alpha == 0) ଜାରି;
	ଯଦି (clip_alpha == 255)
	{
	  ଯଦି (premul_a == 255)
	    row[x] = hb_packed_t<uint32_t> (premul);
	  ଅନ୍ୟଥା
	    row[x] = hb_packed_t<uint32_t> (hb_raster_src_over (premul, (uint32_t) row[x]));
	}
	ଅନ୍ୟଥା
	{
	  uint32_t src = hb_raster_alpha_mul (premul, clip_alpha);
	  row[x] = hb_packed_t<uint32_t> (hb_raster_src_over (src, (uint32_t) row[x]));
	}
      }
    }
  }
  ଅନ୍ୟଥା ଯଦି (mask_buf)
  {
    ପାଇଁ (unsigned y = iy0; y < iy1; y++)
    {
      hb_packed_t<uint32_t> *__restrict row = (hb_packed_t<uint32_t> *) (surf->buffer.arrayZ + y * stride);
      ଅଚଳ uint8_t *__restrict mask_row = mask_buf + (unsigned) ((int64_t) y - mask_y0) * mask_ext.stride;
      unsigned mx = (unsigned) ((int64_t) ix0 - mask_x0);
      ପାଇଁ (unsigned x = ix0; x < ix1; x++)
      {
	uint8_t mask_alpha = mask_row[mx++];
	ଯଦି (mask_alpha == 0) ଜାରି;
	uint32_t src = hb_raster_alpha_mul (premul, mask_alpha);
	row[x] = hb_packed_t<uint32_t> (hb_raster_src_over (src, (uint32_t) row[x]));
      }
    }
  }
  ଅନ୍ୟଥା
  {
    ପାଇଁ (unsigned y = clip.min_y; y < clip.max_y; y++)
    {
      hb_packed_t<uint32_t> *row = (hb_packed_t<uint32_t> *) (surf->buffer.arrayZ + y * stride);
      ପାଇଁ (unsigned x = clip.min_x; x < clip.max_x; x++)
	row[x] = hb_packed_t<uint32_t> (hb_raster_src_over (premul, (uint32_t) row[x]));
    }
  }
}

ସ୍ଥିର ଶୂନ୍ୟ
hb_raster_paint_color (hb_paint_funcs_t *pfuncs HB_UNUSED,
		       ଶୂନ୍ୟ *paint_data,
		       hb_bool_t is_foreground HB_UNUSED,
		       hb_color_t color,
		       ଶୂନ୍ୟ *user_data HB_UNUSED)
{
  hb_raster_paint_t *c = (hb_raster_paint_t *) paint_data;

  ensure_initialized (c);

  hb_raster_image_t *surf = c->current_surface ();
  ଯଦି (unlikely (!surf)) ଫେରନ୍ତୁ;

  hb_raster_paint_solid (c, surf, color, nullptr);
}

ସ୍ଥିର ଶୂନ୍ୟ
hb_raster_paint_fill_glyph (hb_paint_funcs_t *pfuncs HB_UNUSED,
			    ଶୂନ୍ୟ *paint_data,
			    hb_codepoint_t glyph,
			    hb_font_t *font,
			    hb_bool_t is_foreground HB_UNUSED,
			    hb_color_t color,
			    ଶୂନ୍ୟ *user_data HB_UNUSED)
{
  hb_raster_paint_t *c = (hb_raster_paint_t *) paint_data;

  ensure_initialized (c);

  hb_raster_image_t *surf = c->current_surface ();
  ଯଦି (unlikely (!surf)) ଫେରନ୍ତୁ;

  /* Out of budget: skip the glyph-outline extraction entirely. */
  ଯଦି (unlikely (c->budget_remaining < 0)) ଫେରନ୍ତୁ;

  hb_raster_draw_t *rdr = c->clip_rdr;
  hb_transform_t<> t = c->current_effective_transform ();
  hb_raster_draw_set_transform (rdr, t.xx, t.yx, t.xy, t.yy, t.x0, t.y0);
  hb_raster_paint_attach_clip_rdr (c, rdr, surf);

  hb_raster_paint_glyph_clip_data_t data = {glyph, font};
  hb_raster_paint_emit_clip_glyph_mask (rdr, &data);
  hb_raster_paint_readback_budget (c, rdr);

  ଯଦି (unlikely (!c->precharge_work (hb_raster_draw_get_pixel_work (rdr,
							  surf->extents.height,
							  surf->extents.width))))
  {
    hb_raster_draw_clear (rdr);
    ଫେରନ୍ତୁ;
  }

  hb_raster_image_t *mask_img = hb_raster_draw_render (rdr);
  ଯଦି (unlikely (!mask_img)) ଫେରନ୍ତୁ;

  hb_raster_extents_t mask_ext;
  hb_raster_image_get_extents (mask_img, &mask_ext);
  ଯଦି (likely (c->charge_work ((int64_t) mask_ext.width * mask_ext.height)))
    hb_raster_paint_solid (c, surf, color, mask_img);

  hb_raster_draw_recycle_image (rdr, mask_img);
}

ସ୍ଥିର hb_bool_t
hb_raster_paint_image (hb_paint_funcs_t *pfuncs HB_UNUSED,
		       ଶୂନ୍ୟ *paint_data,
		       hb_blob_t *blob,
		       unsigned width,
		       unsigned height,
		       hb_tag_t format,
		       ଦଶମିକ slant HB_UNUSED,
		       hb_glyph_extents_t *extents,
		       ଶୂନ୍ୟ *user_data HB_UNUSED)
{
  hb_raster_paint_t *c = (hb_raster_paint_t *) paint_data;

  ensure_initialized (c);

  /* Out of pixel budget: skip, including the image decode below.  Image
   * compositing is pixel work and does no outline traversal. */
  ଯଦି (unlikely (c->pixel_remaining < 0)) ଫେରନ୍ତୁ ମିଥ୍ୟା;

  unsigned src_width = width;
  unsigned src_height = height;
  ଅଚଳ hb_packed_t<uint32_t> *src_data = nullptr;
  hb_raster_image_t decoded_png;

  ଯଦି (format == HB_PAINT_IMAGE_FORMAT_BGRA)
  {
    ଯଦି (src_width == 0 || src_height == 0)
      ଫେରନ୍ତୁ ମିଥ୍ୟା;
    ଯଦି (src_width > (unsigned) INT_MAX || src_height > (unsigned) INT_MAX)
      ଫେରନ୍ତୁ ମିଥ୍ୟା;

    unsigned data_len;
    ଅଚଳ uint8_t *data = (ଅଚଳ uint8_t *) hb_blob_get_data (blob, &data_len);
    size_t pixel_ଗଣନା = (size_t) src_width * (size_t) src_height;
    ଯଦି (src_width && pixel_ଗଣନା / src_width != src_height)
      ଫେରନ୍ତୁ ମିଥ୍ୟା;
    ଯଦି (pixel_ଗଣନା > (size_t) -1 / 4u)
      ଫେରନ୍ତୁ ମିଥ୍ୟା;
    size_t required_size = pixel_ଗଣନା * 4u;
    ଯଦି (!data || (size_t) data_len < required_size)
      ଫେରନ୍ତୁ ମିଥ୍ୟା;

    src_data = (ଅଚଳ hb_packed_t<uint32_t> *) data;
  }
  ଅନ୍ୟଥା ଯଦି (format == HB_PAINT_IMAGE_FORMAT_PNG)
  {
#ifdef HAVE_PNG
    ଯଦି (!decoded_png.deserialize_from_png (blob))
      ଫେରନ୍ତୁ ମିଥ୍ୟା;
    src_width = decoded_png.extents.width;
    src_height = decoded_png.extents.height;
    src_data = (ଅଚଳ hb_packed_t<uint32_t> *) decoded_png.buffer.arrayZ;
#ଅନ୍ୟଥା
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
#endif
  }
  ଅନ୍ୟଥା
    ଫେରନ୍ତୁ ମିଥ୍ୟା;

  hb_raster_image_t *surf = c->current_surface ();
  ଯଦି (unlikely (!surf)) ଫେରନ୍ତୁ ମିଥ୍ୟା;
  ଯଦି (!extents) ଫେରନ୍ତୁ ମିଥ୍ୟା;

  ଅଚଳ hb_raster_clip_t &clip = c->current_clip ();
  hb_transform_t<> t = c->current_effective_transform ();

  /* Compute inverse transform for sampling */
  ଦଶମିକ det = t.xx * t.yy - t.xy * t.yx;
  ଯଦି (fabsf (det) < 1e-10f) ଫେରନ୍ତୁ ମିଥ୍ୟା;
  ଦଶମିକ inv_det = 1.f / det;
  ଦଶମିକ inv_xx =  t.yy * inv_det;
  ଦଶମିକ inv_xy = -t.xy * inv_det;
  ଦଶମିକ inv_yx = -t.yx * inv_det;
  ଦଶମିକ inv_yy =  t.xx * inv_det;
  ଦଶମିକ inv_x0 = (t.xy * t.y0 - t.yy * t.x0) * inv_det;
  ଦଶମିକ inv_y0 = (t.yx * t.x0 - t.xx * t.y0) * inv_det;

  unsigned surf_stride = surf->extents.stride;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ox = surf->extents.x_origin;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା oy = surf->extents.y_origin;

  /* Image source rectangle in glyph space */
  ଦଶମିକ img_x = extents->x_bearing;
  ଦଶମିକ img_y = extents->y_bearing;
  ଦଶମିକ img_sx = (ଦଶମିକ) extents->width / src_width;
  ଦଶମିକ img_sy = (ଦଶମିକ) extents->height / src_height;
  ଯଦି (fabsf (img_sx) < 1e-10f || fabsf (img_sy) < 1e-10f)
    ଫେରନ୍ତୁ ମିଥ୍ୟା;

  {
    unsigned clip_w = clip.max_x > clip.min_x ? clip.max_x - clip.min_x : 0;
    unsigned clip_h = clip.max_y > clip.min_y ? clip.max_y - clip.min_y : 0;
    ଯଦି (unlikely (!c->charge_work ((int64_t) clip_w * clip_h +
				   (int64_t) src_width * src_height)))
      ଫେରନ୍ତୁ ମିଥ୍ୟା;
  }

  ଯଦି (clip.is_rect)
  {
    ପାଇଁ (unsigned py = clip.min_y; py < clip.max_y; py++)
    {
      hb_packed_t<uint32_t> *row = (hb_packed_t<uint32_t> *) (surf->buffer.arrayZ + py * surf_stride);
      ଦଶମିକ gx = inv_xx * (ଦଶମିକ) ((ପୂର୍ଣ୍ଣସଂଖ୍ୟା) clip.min_x + ox) + inv_xy * (ଦଶମିକ) ((ପୂର୍ଣ୍ଣସଂଖ୍ୟା) py + oy) + inv_x0;
      ଦଶମିକ gy = inv_yx * (ଦଶମିକ) ((ପୂର୍ଣ୍ଣସଂଖ୍ୟା) clip.min_x + ox) + inv_yy * (ଦଶମିକ) ((ପୂର୍ଣ୍ଣସଂଖ୍ୟା) py + oy) + inv_y0;
      ପାଇଁ (unsigned px = clip.min_x; px < clip.max_x; px++)
      {
	/* Map glyph space to image texel; bilinear reconstruction. */
	ଦଶମିକ ix = (gx - img_x) / img_sx;
	ଦଶମିକ iy = (ଦଶମିକ) (src_height - 1) - (gy - img_y) / img_sy;

	ଯଦି (unlikely (!std::isfinite (ix) || !std::isfinite (iy)) ||
	    ix < 0.f || iy < 0.f ||
	    ix > (ଦଶମିକ) (src_width - 1) || iy > (ଦଶମିକ) (src_height - 1))
	{
	  gx += inv_xx;
	  gy += inv_yx;
	  ଜାରି;
	}

	uint32_t src_px = hb_raster_sample_bilinear_premul (src_data, src_width, src_height,
							     ix, iy);
	row[px] = hb_packed_t<uint32_t> (hb_raster_src_over (src_px, (uint32_t) row[px]));
	gx += inv_xx;
	gy += inv_yx;
      }
    }
  }
  ଅନ୍ୟଥା
  {
    ପାଇଁ (unsigned py = clip.min_y; py < clip.max_y; py++)
    {
      hb_packed_t<uint32_t> *row = (hb_packed_t<uint32_t> *) (surf->buffer.arrayZ + py * surf_stride);
      ଅଚଳ uint8_t *clip_row = clip.alpha.arrayZ + py * clip.stride;
      ଦଶମିକ gx = inv_xx * (ଦଶମିକ) ((ପୂର୍ଣ୍ଣସଂଖ୍ୟା) clip.min_x + ox) + inv_xy * (ଦଶମିକ) ((ପୂର୍ଣ୍ଣସଂଖ୍ୟା) py + oy) + inv_x0;
      ଦଶମିକ gy = inv_yx * (ଦଶମିକ) ((ପୂର୍ଣ୍ଣସଂଖ୍ୟା) clip.min_x + ox) + inv_yy * (ଦଶମିକ) ((ପୂର୍ଣ୍ଣସଂଖ୍ୟା) py + oy) + inv_y0;
      ପାଇଁ (unsigned px = clip.min_x; px < clip.max_x; px++)
      {
	uint8_t clip_alpha = clip_row[px];
	ଯଦି (clip_alpha == 0)
	{
	  gx += inv_xx;
	  gy += inv_yx;
	  ଜାରି;
	}

	/* Map glyph space to image texel; bilinear reconstruction. */
	ଦଶମିକ ix = (gx - img_x) / img_sx;
	ଦଶମିକ iy = (ଦଶମିକ) (src_height - 1) - (gy - img_y) / img_sy;

	ଯଦି (unlikely (!std::isfinite (ix) || !std::isfinite (iy)) ||
	    ix < 0.f || iy < 0.f ||
	    ix > (ଦଶମିକ) (src_width - 1) || iy > (ଦଶମିକ) (src_height - 1))
	{
	  gx += inv_xx;
	  gy += inv_yx;
	  ଜାରି;
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

  ଫେରନ୍ତୁ ସତ୍ୟ;
}


/*
 * Gradient helpers
 */

#define GRADIENT_LUT_SIZE 256
#define GRADIENT_LUT_MIN_PIXELS (64u * 64u)

ସ୍ଥିର HB_ALWAYS_INLINE ଦଶମିକ
reflect_gradient_t (ଦଶମିକ t)
{
  t = fmodf (fabsf (t), 2.f);
  ଫେରନ୍ତୁ t > 1.f ? 2.f - t : t;
}

/* Evaluate color at normalized position t, interpolating in premultiplied space. */
ସ୍ଥିର uint32_t
evaluate_color_ଧାଡ଼ି (ଅଚଳ hb_color_stop_t *stops, unsigned len, ଦଶମିକ t,
		     hb_paint_extend_t extend)
{
  ଯଦି (unlikely (!len))
    ଫେରନ୍ତୁ 0;
  ଯଦି (unlikely (len == 1))
    ଫେରନ୍ତୁ color_to_premul_pixel (stops[0].color);
  ଯଦି (unlikely (!std::isfinite (t)))
    t = 0.f;

  /* Apply extend mode */
  ଯଦି (extend == HB_PAINT_EXTEND_PAD)
  {
    t = hb_clamp (t, 0.f, 1.f);
  }
  ଅନ୍ୟଥା ଯଦି (extend == HB_PAINT_EXTEND_REPEAT)
  {
    t = t - floorf (t);
  }
  ଅନ୍ୟଥା /* REFLECT */
  {
    t = reflect_gradient_t (t);
  }

  /* Find bounding stops */
  ଯଦି (t <= stops[0].offset)
    ଫେରନ୍ତୁ color_to_premul_pixel (stops[0].color);
  ଯଦି (t >= stops[len - 1].offset)
    ଫେରନ୍ତୁ color_to_premul_pixel (stops[len - 1].color);

  unsigned i;
  ପାଇଁ (i = 0; i < len - 1; i++)
    ଯଦି (t < stops[i + 1].offset)
      ଭାଙ୍ଗ;

  ଦଶମିକ range = stops[i + 1].offset - stops[i].offset;
  ଦଶମିକ k = range > 0.f ? (t - stops[i].offset) / range : 0.f;

  /* Interpolate in premultiplied [0,255] space */
  hb_color_t c0 = stops[i].color;
  hb_color_t c1 = stops[i + 1].color;

  ଦଶମିକ a0 = hb_color_get_alpha (c0) / 255.f;
  ଦଶମିକ r0 = hb_color_get_red   (c0) / 255.f * a0;
  ଦଶମିକ g0 = hb_color_get_green (c0) / 255.f * a0;
  ଦଶମିକ b0 = hb_color_get_blue  (c0) / 255.f * a0;

  ଦଶମିକ a1 = hb_color_get_alpha (c1) / 255.f;
  ଦଶମିକ r1 = hb_color_get_red   (c1) / 255.f * a1;
  ଦଶମିକ g1 = hb_color_get_green (c1) / 255.f * a1;
  ଦଶମିକ b1 = hb_color_get_blue  (c1) / 255.f * a1;

  ଦଶମିକ a = a0 + k * (a1 - a0);
  ଦଶମିକ r = r0 + k * (r1 - r0);
  ଦଶମିକ g = g0 + k * (g1 - g0);
  ଦଶମିକ b = b0 + k * (b1 - b0);

  uint8_t pa = (uint8_t) (a * 255.f + 0.5f);
  uint8_t pr = (uint8_t) (r * 255.f + 0.5f);
  uint8_t pg = (uint8_t) (g * 255.f + 0.5f);
  uint8_t pb = (uint8_t) (b * 255.f + 0.5f);

  ଫେରନ୍ତୁ (uint32_t) pb | ((uint32_t) pg << 8) | ((uint32_t) pr << 16) | ((uint32_t) pa << 24);
}

ସ୍ଥିର HB_ALWAYS_INLINE ଦଶମିକ
normalize_gradient_t (ଦଶମିକ t, hb_paint_extend_t extend)
{
  ଯଦି (unlikely (!std::isfinite (t)))
    ଫେରନ୍ତୁ 0.f;

  ଯଦି (extend == HB_PAINT_EXTEND_PAD)
    ଫେରନ୍ତୁ hb_clamp (t, 0.f, 1.f);
  ଯଦି (extend == HB_PAINT_EXTEND_REPEAT)
  {
    t = t - floorf (t);
    ଫେରନ୍ତୁ t < 0.f ? t + 1.f : t;
  }

  /* REFLECT */
  ଫେରନ୍ତୁ reflect_gradient_t (t);
}

ସ୍ଥିର ଶୂନ୍ୟ
build_gradient_lut (ଅଚଳ hb_color_stop_t *stops,
		    unsigned len,
		    uint32_t *lut)
{
  ପାଇଁ (unsigned i = 0; i < GRADIENT_LUT_SIZE; i++)
  {
    ଦଶମିକ t = (ଦଶମିକ) i / (GRADIENT_LUT_SIZE - 1);
    lut[i] = evaluate_color_ଧାଡ଼ି (stops, len, t, HB_PAINT_EXTEND_PAD);
  }
}

ସ୍ଥିର HB_ALWAYS_INLINE uint32_t
lookup_gradient_lut (ଅଚଳ uint32_t *lut,
		     ଦଶମିକ t,
		     hb_paint_extend_t extend)
{
  ଦଶମିକ u = normalize_gradient_t (t, extend);
  ଯଦି (unlikely (!std::isfinite (u)))
    u = 0.f;
  ଅନ୍ୟଥା
    u = hb_clamp (u, 0.f, 1.f);
  unsigned idx = (unsigned) (u * (GRADIENT_LUT_SIZE - 1) + 0.5f);
  ଫେରନ୍ତୁ lut[idx];
}

/*
 * Gradient paint callbacks
 */

ସ୍ଥିର ଶୂନ୍ୟ
hb_raster_paint_linear_gradient (hb_paint_funcs_t *pfuncs HB_UNUSED,
				 ଶୂନ୍ୟ *paint_data,
				 hb_color_ଧାଡ଼ି_t *color_ଧାଡ଼ି,
				 ଦଶମିକ x0, ଦଶମିକ y0,
				 ଦଶମିକ x1, ଦଶମିକ y1,
				 ଦଶମିକ x2, ଦଶମିକ y2,
				 ଶୂନ୍ୟ *user_data HB_UNUSED)
{
  hb_raster_paint_t *c = (hb_raster_paint_t *) paint_data;

  ensure_initialized (c);

  hb_raster_image_t *surf = c->current_surface ();
  ଯଦି (unlikely (!surf)) ଫେରନ୍ତୁ;

  ଯଦି (unlikely (!c->fetch_color_stops (color_ଧାଡ଼ି)))
    ଫେରନ୍ତୁ;
  hb_color_stop_t *stops = c->scratch_color_stops.arrayZ;
  unsigned len = c->scratch_color_stops.length;
  ଦଶମିକ mn, mx;
  hb_paint_normalize_color_ଧାଡ଼ି (stops, len, &mn, &mx);

  hb_paint_extend_t extend = hb_color_ଧାଡ଼ି_get_extend (color_ଧାଡ଼ି);
  ଅଚଳ hb_raster_clip_t &clip = c->current_clip ();
  unsigned clip_w = clip.max_x > clip.min_x ? clip.max_x - clip.min_x : 0;
  unsigned clip_h = clip.max_y > clip.min_y ? clip.max_y - clip.min_y : 0;
  ଯଦି (unlikely (!c->charge_work ((int64_t) clip_w * clip_h)))
    ଫେରନ୍ତୁ;
  ତାର୍କିକ use_lut = (uint64_t) clip_w * clip_h >= GRADIENT_LUT_MIN_PIXELS;
  uint32_t lut[GRADIENT_LUT_SIZE];
  ଯଦି (use_lut)
    build_gradient_lut (stops, len, lut);

  /* Reduce 3-point anchor to 2-point gradient axis */
  ଦଶମିକ lx0, ly0, lx1, ly1;
  hb_paint_reduce_linear_anchors (x0, y0, x1, y1, x2, y2,
				  &lx0, &ly0, &lx1, &ly1);

  /* Apply normalization to endpoints */
  ଦଶମିକ gx0 = lx0 + mn * (lx1 - lx0);
  ଦଶମିକ gy0 = ly0 + mn * (ly1 - ly0);
  ଦଶମିକ gx1 = lx0 + mx * (lx1 - lx0);
  ଦଶମିକ gy1 = ly0 + mx * (ly1 - ly0);

  /* Inverse transform: pixel → glyph space */
  hb_transform_t<> t = c->current_effective_transform ();
  ଦଶମିକ det = t.xx * t.yy - t.xy * t.yx;
  ଯଦି (fabsf (det) < 1e-10f) ଫେରନ୍ତୁ;

  {
    ଦଶମିକ inv_det = 1.f / det;
    ଦଶମିକ inv_xx =  t.yy * inv_det;
    ଦଶମିକ inv_xy = -t.xy * inv_det;
    ଦଶମିକ inv_yx = -t.yx * inv_det;
    ଦଶମିକ inv_yy =  t.xx * inv_det;
    ଦଶମିକ inv_x0 = (t.xy * t.y0 - t.yy * t.x0) * inv_det;
    ଦଶମିକ inv_y0 = (t.yx * t.x0 - t.xx * t.y0) * inv_det;

    /* Gradient direction vector and denominator for projection */
    ଦଶମିକ dx = gx1 - gx0, dy = gy1 - gy0;
    ଦଶମିକ denom = dx * dx + dy * dy;
    ଯଦି (denom < 1e-10f) ଫେରନ୍ତୁ;
    ଦଶମିକ inv_denom = 1.f / denom;

    unsigned stride = surf->extents.stride;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ox = surf->extents.x_origin;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା oy = surf->extents.y_origin;

    ଯଦି (clip.is_rect)
    {
      ପାଇଁ (unsigned py = clip.min_y; py < clip.max_y; py++)
      {
	hb_packed_t<uint32_t> *row = (hb_packed_t<uint32_t> *) (surf->buffer.arrayZ + py * stride);
	ଦଶମିକ gx = inv_xx * ((ଦଶମିକ) ((ପୂର୍ଣ୍ଣସଂଖ୍ୟା) clip.min_x + ox) + 0.5f) + inv_xy * ((ଦଶମିକ) ((ପୂର୍ଣ୍ଣସଂଖ୍ୟା) py + oy) + 0.5f) + inv_x0;
	ଦଶମିକ gy = inv_yx * ((ଦଶମିକ) ((ପୂର୍ଣ୍ଣସଂଖ୍ୟା) clip.min_x + ox) + 0.5f) + inv_yy * ((ଦଶମିକ) ((ପୂର୍ଣ୍ଣସଂଖ୍ୟା) py + oy) + 0.5f) + inv_y0;
	ଯଦି (use_lut)
	{
	  ପାଇଁ (unsigned px = clip.min_x; px < clip.max_x; px++)
	  {
	    ଦଶମିକ proj_t = ((gx - gx0) * dx + (gy - gy0) * dy) * inv_denom;
	    uint32_t src = lookup_gradient_lut (lut, proj_t, extend);
	    row[px] = hb_packed_t<uint32_t> (hb_raster_src_over (src, (uint32_t) row[px]));
	    gx += inv_xx;
	    gy += inv_yx;
	  }
	}
	ଅନ୍ୟଥା
	{
	  ପାଇଁ (unsigned px = clip.min_x; px < clip.max_x; px++)
	  {
	    ଦଶମିକ proj_t = ((gx - gx0) * dx + (gy - gy0) * dy) * inv_denom;
	    uint32_t src = evaluate_color_ଧାଡ଼ି (stops, len, proj_t, extend);
	    row[px] = hb_packed_t<uint32_t> (hb_raster_src_over (src, (uint32_t) row[px]));
	    gx += inv_xx;
	    gy += inv_yx;
	  }
	}
      }
    }
    ଅନ୍ୟଥା
    {
      ପାଇଁ (unsigned py = clip.min_y; py < clip.max_y; py++)
      {
	hb_packed_t<uint32_t> *row = (hb_packed_t<uint32_t> *) (surf->buffer.arrayZ + py * stride);
	ଅଚଳ uint8_t *clip_row = clip.alpha.arrayZ + py * clip.stride;
	ଦଶମିକ gx = inv_xx * ((ଦଶମିକ) ((ପୂର୍ଣ୍ଣସଂଖ୍ୟା) clip.min_x + ox) + 0.5f) + inv_xy * ((ଦଶମିକ) ((ପୂର୍ଣ୍ଣସଂଖ୍ୟା) py + oy) + 0.5f) + inv_x0;
	ଦଶମିକ gy = inv_yx * ((ଦଶମିକ) ((ପୂର୍ଣ୍ଣସଂଖ୍ୟା) clip.min_x + ox) + 0.5f) + inv_yy * ((ଦଶମିକ) ((ପୂର୍ଣ୍ଣସଂଖ୍ୟା) py + oy) + 0.5f) + inv_y0;
	ଯଦି (use_lut)
	{
	  ପାଇଁ (unsigned px = clip.min_x; px < clip.max_x; px++)
	  {
	    uint8_t clip_alpha = clip_row[px];
	    ଯଦି (clip_alpha == 0)
	    {
	      gx += inv_xx;
	      gy += inv_yx;
	      ଜାରି;
	    }
	    ଦଶମିକ proj_t = ((gx - gx0) * dx + (gy - gy0) * dy) * inv_denom;
	    uint32_t src = lookup_gradient_lut (lut, proj_t, extend);
	    src = hb_raster_alpha_mul (src, clip_alpha);
	    row[px] = hb_packed_t<uint32_t> (hb_raster_src_over (src, (uint32_t) row[px]));
	    gx += inv_xx;
	    gy += inv_yx;
	  }
	}
	ଅନ୍ୟଥା
	{
	  ପାଇଁ (unsigned px = clip.min_x; px < clip.max_x; px++)
	  {
	    uint8_t clip_alpha = clip_row[px];
	    ଯଦି (clip_alpha == 0)
	    {
	      gx += inv_xx;
	      gy += inv_yx;
	      ଜାରି;
	    }
	    ଦଶମିକ proj_t = ((gx - gx0) * dx + (gy - gy0) * dy) * inv_denom;
	    uint32_t src = evaluate_color_ଧାଡ଼ି (stops, len, proj_t, extend);
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

ସ୍ଥିର ଶୂନ୍ୟ
hb_raster_paint_radial_gradient (hb_paint_funcs_t *pfuncs HB_UNUSED,
				 ଶୂନ୍ୟ *paint_data,
				 hb_color_ଧାଡ଼ି_t *color_ଧାଡ଼ି,
				 ଦଶମିକ x0, ଦଶମିକ y0, ଦଶମିକ r0,
				 ଦଶମିକ x1, ଦଶମିକ y1, ଦଶମିକ r1,
				 ଶୂନ୍ୟ *user_data HB_UNUSED)
{
  hb_raster_paint_t *c = (hb_raster_paint_t *) paint_data;

  ensure_initialized (c);

  hb_raster_image_t *surf = c->current_surface ();
  ଯଦି (unlikely (!surf)) ଫେରନ୍ତୁ;

  ଯଦି (unlikely (!c->fetch_color_stops (color_ଧାଡ଼ି)))
    ଫେରନ୍ତୁ;
  hb_color_stop_t *stops = c->scratch_color_stops.arrayZ;
  unsigned len = c->scratch_color_stops.length;
  ଦଶମିକ mn, mx;
  hb_paint_normalize_color_ଧାଡ଼ି (stops, len, &mn, &mx);

  hb_paint_extend_t extend = hb_color_ଧାଡ଼ି_get_extend (color_ଧାଡ଼ି);
  ଅଚଳ hb_raster_clip_t &clip = c->current_clip ();
  unsigned clip_w = clip.max_x > clip.min_x ? clip.max_x - clip.min_x : 0;
  unsigned clip_h = clip.max_y > clip.min_y ? clip.max_y - clip.min_y : 0;
  ଯଦି (unlikely (!c->charge_work ((int64_t) clip_w * clip_h)))
    ଫେରନ୍ତୁ;
  ତାର୍କିକ use_lut = (uint64_t) clip_w * clip_h >= GRADIENT_LUT_MIN_PIXELS;
  uint32_t lut[GRADIENT_LUT_SIZE];
  ଯଦି (use_lut)
    build_gradient_lut (stops, len, lut);

  /* Apply normalization to circle parameters */
  ଦଶମିକ cx0 = x0 + mn * (x1 - x0);
  ଦଶମିକ cy0 = y0 + mn * (y1 - y0);
  ଦଶମିକ cr0 = r0 + mn * (r1 - r0);
  ଦଶମିକ cx1 = x0 + mx * (x1 - x0);
  ଦଶମିକ cy1 = y0 + mx * (y1 - y0);
  ଦଶମିକ cr1 = r0 + mx * (r1 - r0);

  /* Inverse transform */
  hb_transform_t<> t = c->current_effective_transform ();
  ଦଶମିକ det = t.xx * t.yy - t.xy * t.yx;
  ଯଦି (fabsf (det) < 1e-10f) ଫେରନ୍ତୁ;

  {
    ଦଶମିକ inv_det = 1.f / det;
    ଦଶମିକ inv_xx =  t.yy * inv_det;
    ଦଶମିକ inv_xy = -t.xy * inv_det;
    ଦଶମିକ inv_yx = -t.yx * inv_det;
    ଦଶମିକ inv_yy =  t.xx * inv_det;
    ଦଶମିକ inv_x0 = (t.xy * t.y0 - t.yy * t.x0) * inv_det;
    ଦଶମିକ inv_y0 = (t.yx * t.x0 - t.xx * t.y0) * inv_det;

    /* Precompute quadratic coefficients for radial gradient:
     * |p - c0 - t*(c1-c0)|^2 = (r0 + t*(r1-r0))^2
     *
     * Expanding gives At^2 + Bt + C = 0 where:
     *   cdx = c1.x - c0.x, cdy = c1.y - c0.y, dr = r1 - r0
     *   A = cdx^2 + cdy^2 - dr^2
     *   B = -2*(px-c0.x)*cdx - 2*(py-c0.y)*cdy - 2*r0*dr
     *   C = (px-c0.x)^2 + (py-c0.y)^2 - r0^2
     */
    ଦଶମିକ cdx = cx1 - cx0, cdy = cy1 - cy0;
    ଦଶମିକ dr = cr1 - cr0;
    ଦଶମିକ A = cdx * cdx + cdy * cdy - dr * dr;

    unsigned stride = surf->extents.stride;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ox = surf->extents.x_origin;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା oy = surf->extents.y_origin;

    ଯଦି (clip.is_rect)
    {
      ପାଇଁ (unsigned py = clip.min_y; py < clip.max_y; py++)
      {
	hb_packed_t<uint32_t> *row = (hb_packed_t<uint32_t> *) (surf->buffer.arrayZ + py * stride);
	ଦଶମିକ gx = inv_xx * ((ଦଶମିକ) ((ପୂର୍ଣ୍ଣସଂଖ୍ୟା) clip.min_x + ox) + 0.5f) + inv_xy * ((ଦଶମିକ) ((ପୂର୍ଣ୍ଣସଂଖ୍ୟା) py + oy) + 0.5f) + inv_x0;
	ଦଶମିକ gy = inv_yx * ((ଦଶମିକ) ((ପୂର୍ଣ୍ଣସଂଖ୍ୟା) clip.min_x + ox) + 0.5f) + inv_yy * ((ଦଶମିକ) ((ପୂର୍ଣ୍ଣସଂଖ୍ୟା) py + oy) + 0.5f) + inv_y0;
	ଯଦି (use_lut)
	{
	  ପାଇଁ (unsigned px = clip.min_x; px < clip.max_x; px++)
	  {
	    ଦଶମିକ dpx = gx - cx0, dpy = gy - cy0;
	    ଦଶମିକ B = -2.f * (dpx * cdx + dpy * cdy + cr0 * dr);
	    ଦଶମିକ C = dpx * dpx + dpy * dpy - cr0 * cr0;

	    ଦଶମିକ grad_t;
	    ଯଦି (fabsf (A) > 1e-10f)
	    {
	      ଦଶମିକ disc = B * B - 4.f * A * C;
	      ଯଦି (disc < 0.f)
	      {
		gx += inv_xx;
		gy += inv_yx;
		ଜାରି;
	      }
	      ଦଶମିକ sq = sqrtf (disc);
	      /* Pick the larger root (t closer to 1 = outer circle) */
	      ଦଶମିକ t1 = (-B + sq) / (2.f * A);
	      ଦଶମିକ t2 = (-B - sq) / (2.f * A);
	      /* Choose the root that gives a positive radius */
	      ଯଦି (cr0 + t1 * dr >= 0.f)
		grad_t = t1;
	      ଅନ୍ୟଥା
		grad_t = t2;
	    }
	    ଅନ୍ୟଥା
	    {
	      /* Linear case: Bt + C = 0 */
	      ଯଦି (fabsf (B) < 1e-10f)
	      {
		gx += inv_xx;
		gy += inv_yx;
		ଜାରି;
	      }
	      grad_t = -C / B;
	    }

	    uint32_t src = lookup_gradient_lut (lut, grad_t, extend);
	    row[px] = hb_packed_t<uint32_t> (hb_raster_src_over (src, (uint32_t) row[px]));
	    gx += inv_xx;
	    gy += inv_yx;
	  }
	}
	ଅନ୍ୟଥା
	{
	  ପାଇଁ (unsigned px = clip.min_x; px < clip.max_x; px++)
	  {
	    ଦଶମିକ dpx = gx - cx0, dpy = gy - cy0;
	    ଦଶମିକ B = -2.f * (dpx * cdx + dpy * cdy + cr0 * dr);
	    ଦଶମିକ C = dpx * dpx + dpy * dpy - cr0 * cr0;

	    ଦଶମିକ grad_t;
	    ଯଦି (fabsf (A) > 1e-10f)
	    {
	      ଦଶମିକ disc = B * B - 4.f * A * C;
	      ଯଦି (disc < 0.f)
	      {
		gx += inv_xx;
		gy += inv_yx;
		ଜାରି;
	      }
	      ଦଶମିକ sq = sqrtf (disc);
	      ଦଶମିକ t1 = (-B + sq) / (2.f * A);
	      ଦଶମିକ t2 = (-B - sq) / (2.f * A);
	      grad_t = (cr0 + t1 * dr >= 0.f) ? t1 : t2;
	    }
	    ଅନ୍ୟଥା
	    {
	      ଯଦି (fabsf (B) < 1e-10f)
	      {
		gx += inv_xx;
		gy += inv_yx;
		ଜାରି;
	      }
	      grad_t = -C / B;
	    }

	    uint32_t src = evaluate_color_ଧାଡ଼ି (stops, len, grad_t, extend);
	    row[px] = hb_packed_t<uint32_t> (hb_raster_src_over (src, (uint32_t) row[px]));
	    gx += inv_xx;
	    gy += inv_yx;
	  }
	}
      }
    }
    ଅନ୍ୟଥା
    {
      ପାଇଁ (unsigned py = clip.min_y; py < clip.max_y; py++)
      {
	hb_packed_t<uint32_t> *row = (hb_packed_t<uint32_t> *) (surf->buffer.arrayZ + py * stride);
	ଅଚଳ uint8_t *clip_row = clip.alpha.arrayZ + py * clip.stride;
	ଦଶମିକ gx = inv_xx * ((ଦଶମିକ) ((ପୂର୍ଣ୍ଣସଂଖ୍ୟା) clip.min_x + ox) + 0.5f) + inv_xy * ((ଦଶମିକ) ((ପୂର୍ଣ୍ଣସଂଖ୍ୟା) py + oy) + 0.5f) + inv_x0;
	ଦଶମିକ gy = inv_yx * ((ଦଶମିକ) ((ପୂର୍ଣ୍ଣସଂଖ୍ୟା) clip.min_x + ox) + 0.5f) + inv_yy * ((ଦଶମିକ) ((ପୂର୍ଣ୍ଣସଂଖ୍ୟା) py + oy) + 0.5f) + inv_y0;
	ଯଦି (use_lut)
	{
	  ପାଇଁ (unsigned px = clip.min_x; px < clip.max_x; px++)
	  {
	    uint8_t clip_alpha = clip_row[px];
	    ଯଦି (clip_alpha == 0)
	    {
	      gx += inv_xx;
	      gy += inv_yx;
	      ଜାରି;
	    }
	    ଦଶମିକ dpx = gx - cx0, dpy = gy - cy0;
	    ଦଶମିକ B = -2.f * (dpx * cdx + dpy * cdy + cr0 * dr);
	    ଦଶମିକ C = dpx * dpx + dpy * dpy - cr0 * cr0;

	    ଦଶମିକ grad_t;
	    ଯଦି (fabsf (A) > 1e-10f)
	    {
	      ଦଶମିକ disc = B * B - 4.f * A * C;
	      ଯଦି (disc < 0.f)
	      {
		gx += inv_xx;
		gy += inv_yx;
		ଜାରି;
	      }
	      ଦଶମିକ sq = sqrtf (disc);
	      ଦଶମିକ t1 = (-B + sq) / (2.f * A);
	      ଦଶମିକ t2 = (-B - sq) / (2.f * A);
	      grad_t = (cr0 + t1 * dr >= 0.f) ? t1 : t2;
	    }
	    ଅନ୍ୟଥା
	    {
	      ଯଦି (fabsf (B) < 1e-10f)
	      {
		gx += inv_xx;
		gy += inv_yx;
		ଜାରି;
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
	ଅନ୍ୟଥା
	{
	  ପାଇଁ (unsigned px = clip.min_x; px < clip.max_x; px++)
	  {
	    uint8_t clip_alpha = clip_row[px];
	    ଯଦି (clip_alpha == 0)
	    {
	      gx += inv_xx;
	      gy += inv_yx;
	      ଜାରି;
	    }
	    ଦଶମିକ dpx = gx - cx0, dpy = gy - cy0;
	    ଦଶମିକ B = -2.f * (dpx * cdx + dpy * cdy + cr0 * dr);
	    ଦଶମିକ C = dpx * dpx + dpy * dpy - cr0 * cr0;

	    ଦଶମିକ grad_t;
	    ଯଦି (fabsf (A) > 1e-10f)
	    {
	      ଦଶମିକ disc = B * B - 4.f * A * C;
	      ଯଦି (disc < 0.f)
	      {
		gx += inv_xx;
		gy += inv_yx;
		ଜାରି;
	      }
	      ଦଶମିକ sq = sqrtf (disc);
	      ଦଶମିକ t1 = (-B + sq) / (2.f * A);
	      ଦଶମିକ t2 = (-B - sq) / (2.f * A);
	      grad_t = (cr0 + t1 * dr >= 0.f) ? t1 : t2;
	    }
	    ଅନ୍ୟଥା
	    {
	      ଯଦି (fabsf (B) < 1e-10f)
	      {
		gx += inv_xx;
		gy += inv_yx;
		ଜାରି;
	      }
	      grad_t = -C / B;
	    }

	    uint32_t src = evaluate_color_ଧାଡ଼ି (stops, len, grad_t, extend);
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

ସ୍ଥିର ଶୂନ୍ୟ
hb_raster_paint_sweep_gradient (hb_paint_funcs_t *pfuncs HB_UNUSED,
				ଶୂନ୍ୟ *paint_data,
				hb_color_ଧାଡ଼ି_t *color_ଧାଡ଼ି,
				ଦଶମିକ cx, ଦଶମିକ cy,
				ଦଶମିକ start_angle,
				ଦଶମିକ end_angle,
				ଶୂନ୍ୟ *user_data HB_UNUSED)
{
  hb_raster_paint_t *c = (hb_raster_paint_t *) paint_data;

  ensure_initialized (c);

  hb_raster_image_t *surf = c->current_surface ();
  ଯଦି (unlikely (!surf)) ଫେରନ୍ତୁ;

  ଯଦି (unlikely (!c->fetch_color_stops (color_ଧାଡ଼ି)))
    ଫେରନ୍ତୁ;
  hb_color_stop_t *stops = c->scratch_color_stops.arrayZ;
  unsigned len = c->scratch_color_stops.length;
  ଦଶମିକ mn, mx;
  hb_paint_normalize_color_ଧାଡ଼ି (stops, len, &mn, &mx);

  hb_paint_extend_t extend = hb_color_ଧାଡ଼ି_get_extend (color_ଧାଡ଼ି);
  ଅଚଳ hb_raster_clip_t &clip = c->current_clip ();
  unsigned clip_w = clip.max_x > clip.min_x ? clip.max_x - clip.min_x : 0;
  unsigned clip_h = clip.max_y > clip.min_y ? clip.max_y - clip.min_y : 0;
  ଯଦି (unlikely (!c->charge_work ((int64_t) clip_w * clip_h)))
    ଫେରନ୍ତୁ;
  ତାର୍କିକ use_lut = (uint64_t) clip_w * clip_h >= GRADIENT_LUT_MIN_PIXELS;
  uint32_t lut[GRADIENT_LUT_SIZE];
  ଯଦି (use_lut)
    build_gradient_lut (stops, len, lut);

  /* Apply normalization to angle range */
  ଦଶମିକ a0 = start_angle + mn * (end_angle - start_angle);
  ଦଶମିକ a1 = start_angle + mx * (end_angle - start_angle);
  ଦଶମିକ angle_range = a1 - a0;

  /* Inverse transform */
  hb_transform_t<> t = c->current_effective_transform ();
  ଦଶମିକ det = t.xx * t.yy - t.xy * t.yx;
  ଯଦି (fabsf (det) < 1e-10f || fabsf (angle_range) < 1e-10f) ଫେରନ୍ତୁ;

  {
    ଦଶମିକ inv_det = 1.f / det;
    ଦଶମିକ inv_xx =  t.yy * inv_det;
    ଦଶମିକ inv_xy = -t.xy * inv_det;
    ଦଶମିକ inv_yx = -t.yx * inv_det;
    ଦଶମିକ inv_yy =  t.xx * inv_det;
    ଦଶମିକ inv_x0 = (t.xy * t.y0 - t.yy * t.x0) * inv_det;
    ଦଶମିକ inv_y0 = (t.yx * t.x0 - t.xx * t.y0) * inv_det;

    ଦଶମିକ inv_angle_range = 1.f / angle_range;

    unsigned stride = surf->extents.stride;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ox = surf->extents.x_origin;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା oy = surf->extents.y_origin;

    ଯଦି (clip.is_rect)
    {
      ପାଇଁ (unsigned py = clip.min_y; py < clip.max_y; py++)
      {
	hb_packed_t<uint32_t> *row = (hb_packed_t<uint32_t> *) (surf->buffer.arrayZ + py * stride);
	ଦଶମିକ gx = inv_xx * ((ଦଶମିକ) ((ପୂର୍ଣ୍ଣସଂଖ୍ୟା) clip.min_x + ox) + 0.5f) + inv_xy * ((ଦଶମିକ) ((ପୂର୍ଣ୍ଣସଂଖ୍ୟା) py + oy) + 0.5f) + inv_x0;
	ଦଶମିକ gy = inv_yx * ((ଦଶମିକ) ((ପୂର୍ଣ୍ଣସଂଖ୍ୟା) clip.min_x + ox) + 0.5f) + inv_yy * ((ଦଶମିକ) ((ପୂର୍ଣ୍ଣସଂଖ୍ୟା) py + oy) + 0.5f) + inv_y0;
	ଯଦି (use_lut)
	{
	  ପାଇଁ (unsigned px = clip.min_x; px < clip.max_x; px++)
	  {
	    ଦଶମିକ angle = atan2f (gy - cy, gx - cx);
	    ଯଦି (angle < 0) angle += (ଦଶମିକ) HB_2_PI;
	    ଦଶମିକ grad_t = (angle - a0) * inv_angle_range;
	    uint32_t src = lookup_gradient_lut (lut, grad_t, extend);
	    row[px] = hb_packed_t<uint32_t> (hb_raster_src_over (src, (uint32_t) row[px]));
	    gx += inv_xx;
	    gy += inv_yx;
	  }
	}
	ଅନ୍ୟଥା
	{
	  ପାଇଁ (unsigned px = clip.min_x; px < clip.max_x; px++)
	  {
	    ଦଶମିକ angle = atan2f (gy - cy, gx - cx);
	    ଯଦି (angle < 0) angle += (ଦଶମିକ) HB_2_PI;
	    ଦଶମିକ grad_t = (angle - a0) * inv_angle_range;
	    uint32_t src = evaluate_color_ଧାଡ଼ି (stops, len, grad_t, extend);
	    row[px] = hb_packed_t<uint32_t> (hb_raster_src_over (src, (uint32_t) row[px]));
	    gx += inv_xx;
	    gy += inv_yx;
	  }
	}
      }
    }
    ଅନ୍ୟଥା
    {
      ପାଇଁ (unsigned py = clip.min_y; py < clip.max_y; py++)
      {
	hb_packed_t<uint32_t> *row = (hb_packed_t<uint32_t> *) (surf->buffer.arrayZ + py * stride);
	ଅଚଳ uint8_t *clip_row = clip.alpha.arrayZ + py * clip.stride;
	ଦଶମିକ gx = inv_xx * ((ଦଶମିକ) ((ପୂର୍ଣ୍ଣସଂଖ୍ୟା) clip.min_x + ox) + 0.5f) + inv_xy * ((ଦଶମିକ) ((ପୂର୍ଣ୍ଣସଂଖ୍ୟା) py + oy) + 0.5f) + inv_x0;
	ଦଶମିକ gy = inv_yx * ((ଦଶମିକ) ((ପୂର୍ଣ୍ଣସଂଖ୍ୟା) clip.min_x + ox) + 0.5f) + inv_yy * ((ଦଶମିକ) ((ପୂର୍ଣ୍ଣସଂଖ୍ୟା) py + oy) + 0.5f) + inv_y0;
	ଯଦି (use_lut)
	{
	  ପାଇଁ (unsigned px = clip.min_x; px < clip.max_x; px++)
	  {
	    uint8_t clip_alpha = clip_row[px];
	    ଯଦି (clip_alpha == 0)
	    {
	      gx += inv_xx;
	      gy += inv_yx;
	      ଜାରି;
	    }
	    ଦଶମିକ angle = atan2f (gy - cy, gx - cx);
	    ଯଦି (angle < 0) angle += (ଦଶମିକ) HB_2_PI;
	    ଦଶମିକ grad_t = (angle - a0) * inv_angle_range;
	    uint32_t src = lookup_gradient_lut (lut, grad_t, extend);
	    src = hb_raster_alpha_mul (src, clip_alpha);
	    row[px] = hb_packed_t<uint32_t> (hb_raster_src_over (src, (uint32_t) row[px]));
	    gx += inv_xx;
	    gy += inv_yx;
	  }
	}
	ଅନ୍ୟଥା
	{
	  ପାଇଁ (unsigned px = clip.min_x; px < clip.max_x; px++)
	  {
	    uint8_t clip_alpha = clip_row[px];
	    ଯଦି (clip_alpha == 0)
	    {
	      gx += inv_xx;
	      gy += inv_yx;
	      ଜାରି;
	    }
	    ଦଶମିକ angle = atan2f (gy - cy, gx - cx);
	    ଯଦି (angle < 0) angle += (ଦଶମିକ) HB_2_PI;
	    ଦଶମିକ grad_t = (angle - a0) * inv_angle_range;
	    uint32_t src = evaluate_color_ଧାଡ଼ି (stops, len, grad_t, extend);
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

ସ୍ଥିର hb_bool_t
hb_raster_paint_custom_palette_color (hb_paint_funcs_t *funcs HB_UNUSED,
				      ଶୂନ୍ୟ *paint_data,
				      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା color_index,
				      hb_color_t *color,
				      ଶୂନ୍ୟ *user_data HB_UNUSED)
{
  hb_raster_paint_t *c = (hb_raster_paint_t *) paint_data;
  ଯଦି (likely (c->custom_palette && hb_map_has (c->custom_palette, color_index)))
  {
    *color = hb_map_get (c->custom_palette, color_index);
    ଫେରନ୍ତୁ ସତ୍ୟ;
  }
  ଫେରନ୍ତୁ ମିଥ୍ୟା;
}

ସ୍ଥିର hb_bool_t
hb_raster_paint_set_budget (hb_paint_funcs_t *, ଶୂନ୍ୟ *paint_data,
			    int64_t budget, ଶୂନ୍ୟ *)
{
  auto *paint = (hb_raster_paint_t *) paint_data;
  paint->budget = budget;
  paint->recharge_budget ();
  ଫେରନ୍ତୁ ସତ୍ୟ;
}

ସ୍ଥିର int64_t
hb_raster_paint_get_budget (hb_paint_funcs_t *, ଶୂନ୍ୟ *paint_data, ଶୂନ୍ୟ *)
{
  ଫେରନ୍ତୁ ((hb_raster_paint_t *) paint_data)->budget;
}

ସ୍ଥିର int64_t *
hb_raster_paint_get_budget_remaining (hb_paint_funcs_t *, ଶୂନ୍ୟ *paint_data, ଶୂନ୍ୟ *)
{
  ଫେରନ୍ତୁ &((hb_raster_paint_t *) paint_data)->budget_remaining;
}


/*
 * Lazy-loader singleton for paint funcs
 */

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ free_static_raster_paint_funcs ();

ସ୍ଥିର ସଂରଚନା hb_raster_paint_funcs_lazy_loader_t : hb_paint_funcs_lazy_loader_t<hb_raster_paint_funcs_lazy_loader_t>
{
  ସ୍ଥିର hb_paint_funcs_t *create ()
  {
    hb_paint_funcs_t *funcs = hb_paint_funcs_create ();

    hb_paint_funcs_set_ଠେଲ_transform_func (funcs, hb_raster_paint_ଠେଲ_transform, nullptr, nullptr);
    hb_paint_funcs_set_କାଢ଼_transform_func (funcs, hb_raster_paint_କାଢ଼_transform, nullptr, nullptr);
    hb_paint_funcs_set_fill_glyph_func (funcs, hb_raster_paint_fill_glyph, nullptr, nullptr);
    hb_paint_funcs_set_color_glyph_func (funcs, hb_raster_paint_color_glyph, nullptr, nullptr);
    hb_paint_funcs_set_ଠେଲ_clip_glyph_func (funcs, hb_raster_paint_ଠେଲ_clip_glyph, nullptr, nullptr);
    hb_paint_funcs_set_ଠେଲ_clip_rectangle_func (funcs, hb_raster_paint_ଠେଲ_clip_rectangle, nullptr, nullptr);
    hb_paint_funcs_set_ଠେଲ_clip_path_start_func (funcs, hb_raster_paint_ଠେଲ_clip_path_start, nullptr, nullptr);
    hb_paint_funcs_set_ଠେଲ_clip_path_end_func (funcs, hb_raster_paint_ଠେଲ_clip_path_end, nullptr, nullptr);
    hb_paint_funcs_set_କାଢ଼_clip_func (funcs, hb_raster_paint_କାଢ଼_clip, nullptr, nullptr);
    hb_paint_funcs_set_ଠେଲ_group_func (funcs, hb_raster_paint_ଠେଲ_group, nullptr, nullptr);
    hb_paint_funcs_set_କାଢ଼_group_func (funcs, hb_raster_paint_କାଢ଼_group, nullptr, nullptr);
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

    ଫେରନ୍ତୁ funcs;
  }
} static_raster_paint_funcs;

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
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
hb_raster_paint_create_or_fail (ଶୂନ୍ୟ)
{
  hb_raster_paint_t *paint = hb_object_create<hb_raster_paint_t> ();
  ଯଦି (unlikely (!paint))
    ଫେରନ୍ତୁ nullptr;

  paint->clip_rdr = hb_raster_draw_create_or_fail ();
  ଯଦି (unlikely (!paint->clip_rdr))
  {
    hb_free (paint);
    ଫେରନ୍ତୁ nullptr;
  }

  ଫେରନ୍ତୁ paint;
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
  ଫେରନ୍ତୁ hb_object_reference (paint);
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
ଶୂନ୍ୟ
hb_raster_paint_destroy (hb_raster_paint_t *paint)
{
  ଯଦି (!hb_object_should_destroy (paint))
    ଫେରନ୍ତୁ;

  hb_map_destroy (paint->custom_palette);
  hb_raster_draw_destroy (paint->clip_rdr);
  ପାଇଁ (auto *s : paint->surface_ଗଦା)
    hb_raster_image_destroy (s);
  ପାଇଁ (auto *s : paint->surface_cache)
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
			       hb_user_data_ଚାବି_t *key,
			       ଶୂନ୍ୟ               *data,
			       hb_destroy_func_t   destroy,
			       hb_bool_t           replace)
{
  ଫେରନ୍ତୁ hb_object_set_user_data (paint, key, data, destroy, replace);
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
ଶୂନ୍ୟ *
hb_raster_paint_get_user_data (ଅଚଳ hb_raster_paint_t  *paint,
			       hb_user_data_ଚାବି_t *key)
{
  ଫେରନ୍ତୁ hb_object_get_user_data (paint, key);
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
ଶୂନ୍ୟ
hb_raster_paint_set_transform (hb_raster_paint_t *paint,
			       ଦଶମିକ xx, ଦଶମିକ yx,
			       ଦଶମିକ xy, ଦଶମିକ yy,
			       ଦଶମିକ dx, ଦଶମିକ dy)
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
ଶୂନ୍ୟ
hb_raster_paint_get_transform (ଅଚଳ hb_raster_paint_t *paint,
			       ଦଶମିକ *xx, ଦଶମିକ *yx,
			       ଦଶମିକ *xy, ଦଶମିକ *yy,
			       ଦଶମିକ *dx, ଦଶମିକ *dy)
{
  ଯଦି (xx) *xx = paint->base_transform.xx;
  ଯଦି (yx) *yx = paint->base_transform.yx;
  ଯଦି (xy) *xy = paint->base_transform.xy;
  ଯଦି (yy) *yy = paint->base_transform.yy;
  ଯଦି (dx) *dx = paint->base_transform.x0;
  ଯଦି (dy) *dy = paint->base_transform.y0;
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
ଶୂନ୍ୟ
hb_raster_paint_set_scale_factor (hb_raster_paint_t *paint,
				  ଦଶମିକ x_scale_factor,
				  ଦଶମିକ y_scale_factor)
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
ଶୂନ୍ୟ
hb_raster_paint_get_scale_factor (ଅଚଳ hb_raster_paint_t *paint,
				  ଦଶମିକ *x_scale_factor,
				  ଦଶମିକ *y_scale_factor)
{
  ଯଦି (x_scale_factor) *x_scale_factor = paint->x_scale_factor;
  ଯଦି (y_scale_factor) *y_scale_factor = paint->y_scale_factor;
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
ଶୂନ୍ୟ
hb_raster_paint_set_extents (hb_raster_paint_t         *paint,
			     ଅଚଳ hb_raster_extents_t *extents)
{
  paint->fixed_extents = *extents;
  paint->has_extents = ସତ୍ୟ;
  ଯଦି (paint->fixed_extents.stride == 0)
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
hb_raster_paint_get_extents (ଅଚଳ hb_raster_paint_t   *paint,
			     hb_raster_extents_t *extents)
{
  ଯଦି (!paint->has_extents)
    ଫେରନ୍ତୁ ମିଥ୍ୟା;

  ଯଦି (extents)
    *extents = paint->fixed_extents;
  ଫେରନ୍ତୁ ସତ୍ୟ;
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
				   ଅଚଳ hb_glyph_extents_t *glyph_extents)
{
  ଦଶମିକ x0 = (ଦଶମିକ) glyph_extents->x_bearing;
  ଦଶମିକ y0 = (ଦଶମିକ) glyph_extents->y_bearing;
  ଦଶମିକ x1 = (ଦଶମିକ) glyph_extents->x_bearing + glyph_extents->width;
  ଦଶମିକ y1 = (ଦଶମିକ) glyph_extents->y_bearing + glyph_extents->height;

  ଦଶମିକ xmin = hb_min (x0, x1);
  ଦଶମିକ xmax = hb_max (x0, x1);
  ଦଶମିକ ymin = hb_min (y0, y1);
  ଦଶମିକ ymax = hb_max (y0, y1);

  ଦଶମିକ px[4] = {xmin, xmin, xmax, xmax};
  ଦଶମିକ py[4] = {ymin, ymax, ymin, ymax};

  hb_transform_t<> t = paint->base_transform;
  paint->apply_scale_factor (t);

  ଦଶମିକ tx, ty;
  t.transform_point (px[0], py[0]);
  tx = px[0]; ty = py[0];
  ଦଶମିକ tx_min = tx, tx_max = tx;
  ଦଶମିକ ty_min = ty, ty_max = ty;

  ପାଇଁ (unsigned i = 1; i < 4; i++)
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

  ଯଦି (ex1 <= ex0 || ey1 <= ey0)
  {
    paint->fixed_extents = {};
    paint->has_extents = ମିଥ୍ୟା;
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
  }

  paint->fixed_extents = {
    ex0, ey0,
    (unsigned) hb_min ((int64_t) ex1 - ex0, (int64_t) HB_RASTER_MAX_AUTO_DIMENSION),
    (unsigned) hb_min ((int64_t) ey1 - ey0, (int64_t) HB_RASTER_MAX_AUTO_DIMENSION),
    0
  };
  paint->has_extents = ସତ୍ୟ;
  ଯଦି (paint->fixed_extents.stride == 0)
    paint->fixed_extents.stride = paint->fixed_extents.width * 4;
  ଫେରନ୍ତୁ ସତ୍ୟ;
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
ଶୂନ୍ୟ
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
hb_raster_paint_get_foreground (ଅଚଳ hb_raster_paint_t *paint)
{
  ଫେରନ୍ତୁ paint->foreground;
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
ଶୂନ୍ୟ
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
hb_raster_paint_get_background (ଅଚଳ hb_raster_paint_t *paint)
{
  ଫେରନ୍ତୁ paint->background;
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
ଶୂନ୍ୟ
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
hb_raster_paint_get_palette (ଅଚଳ hb_raster_paint_t *paint)
{
  ଫେରନ୍ତୁ paint->palette;
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
ଶୂନ୍ୟ
hb_raster_paint_clear_custom_palette_colors (hb_raster_paint_t *paint)
{
  ଯଦି (paint->custom_palette)
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
					  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା       color_index,
					  hb_color_t         color)
{
  ଯଦି (unlikely (!paint->custom_palette))
  {
    paint->custom_palette = hb_map_create ();
    ଯଦି (unlikely (!paint->custom_palette))
      ଫେରନ୍ତୁ ମିଥ୍ୟା;
  }
  hb_map_set (paint->custom_palette, color_index, color);
  ଫେରନ୍ତୁ hb_map_allocation_successful (paint->custom_palette);
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
hb_raster_paint_get_funcs (ଅଚଳ hb_raster_paint_t *paint HB_UNUSED)
{
  ଫେରନ୍ତୁ static_raster_paint_funcs.get_unconst ();
}

ସ୍ଥିର hb_bool_t
hb_raster_paint_glyph_impl (hb_raster_paint_t *paint,
			    hb_font_t        *font,
			    hb_codepoint_t    glyph,
			    hb_bool_t         fallible)
{
  ଯଦି (!paint->has_extents)
  {
    hb_glyph_extents_t ge;
    ଯଦି (hb_font_get_glyph_extents (font, glyph, &ge))
      hb_raster_paint_set_glyph_extents (paint, &ge);
  }

  hb_paint_funcs_t *funcs = hb_raster_paint_get_funcs (paint);
  hb_paint_ଠେଲ_transform (funcs, paint,
			   paint->base_transform.xx, paint->base_transform.yx,
			   paint->base_transform.xy, paint->base_transform.yy,
			   paint->base_transform.x0, paint->base_transform.y0);

  hb_bool_t ret = ସତ୍ୟ;
  ଯଦି (fallible)
    ret = hb_font_paint_glyph_or_fail (font, glyph,
				       funcs, paint,
				       paint->palette, paint->foreground);
  ଅନ୍ୟଥା
    hb_font_paint_glyph (font, glyph,
			 funcs, paint,
			 paint->palette, paint->foreground);

  hb_paint_କାଢ଼_transform (funcs, paint);
  ଫେରନ୍ତୁ ret;
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
  ଫେରନ୍ତୁ hb_raster_paint_glyph_impl (paint, font, glyph, ସତ୍ୟ);
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
ଶୂନ୍ୟ
hb_raster_paint_glyph (hb_raster_paint_t *paint,
		       hb_font_t        *font,
		       hb_codepoint_t    glyph)
{
  hb_raster_paint_glyph_impl (paint, font, glyph, ମିଥ୍ୟା);
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

  ଯଦି (unlikely (!paint->has_extents))
    ଫେରନ୍ତୁ nullptr;

  hb_raster_image_t *result;
  ଯଦି (paint->surface_ଗଦା.length)
  {
    result = paint->surface_ଗଦା[0];
    /* Release any remaining group surfaces (shouldn't happen with
     * well-formed paint calls, but be safe). */
    ପାଇଁ (unsigned i = 1; i < paint->surface_ଗଦା.length; i++)
      paint->release_surface (paint->surface_ଗଦା[i]);
    paint->surface_ଗଦା.clear ();
  }
  ଅନ୍ୟଥା
  {
    result = paint->acquire_surface ();
    ଯଦି (unlikely (!result))
      ଫେରନ୍ତୁ nullptr;
  }

  ଫେରନ୍ତୁ result;
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
ଶୂନ୍ୟ
hb_raster_paint_clear (hb_raster_paint_t *paint)
{
  paint->fixed_extents = {};
  paint->has_extents = ମିଥ୍ୟା;
  paint->transform_ଗଦା.clear ();
  paint->release_all_clips ();
  ପାଇଁ (auto *s : paint->surface_ଗଦା)
    paint->release_surface (s);
  paint->surface_ଗଦା.clear ();
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
ଶୂନ୍ୟ
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
ଶୂନ୍ୟ
hb_raster_paint_recycle_image (hb_raster_paint_t  *paint,
			       hb_raster_image_t  *image)
{
  paint->release_surface (image);
}
