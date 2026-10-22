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

#अन्तर्भाव "hb.hh"

#अन्तर्भाव "hb-raster-paint.hh"
#अन्तर्भाव "hb-machinery.hh"
#अन्तर्भाव "hb-paint.hh"

#अन्तर्भाव <math.h>


/*
 * Pixel helpers (paint-specific)
 */

/* Convert unpremultiplied hb_color_t (BGRA order) to premultiplied BGRA32 pixel. */
स्थिर अन्तःस्थ uint32_t
color_to_premul_pixel (hb_color_t color)
{
  uint8_t a = hb_color_get_alpha (color);
  uint8_t r = hb_raster_div255 (hb_color_get_red (color) * a);
  uint8_t g = hb_raster_div255 (hb_color_get_green (color) * a);
  uint8_t b = hb_raster_div255 (hb_color_get_blue (color) * a);
  फर्काऊ (uint32_t) b | ((uint32_t) g << 8) | ((uint32_t) r << 16) | ((uint32_t) a << 24);
}

/* Bilinear sample from a premultiplied BGRA32 image. */
स्थिर अन्तःस्थ uint32_t
hb_raster_sample_bilinear_premul (अचर hb_packed_t<uint32_t> *src,
				  अचिन्हित width,
				  अचिन्हित height,
				  भिन्न x,
				  भिन्न y)
{
  पूर्णाङ्क x0 = (पूर्णाङ्क) floorf (x);
  पूर्णाङ्क y0 = (पूर्णाङ्क) floorf (y);
  पूर्णाङ्क x1 = x0 + 1;
  पूर्णाङ्क y1 = y0 + 1;
  यदि (x1 >= (पूर्णाङ्क) width) x1 = (पूर्णाङ्क) width - 1;
  यदि (y1 >= (पूर्णाङ्क) height) y1 = (पूर्णाङ्क) height - 1;

  भिन्न tx = x - x0;
  भिन्न ty = y - y0;
  भिन्न omtx = 1.f - tx;
  भिन्न omty = 1.f - ty;

  uint32_t p00 = (uint32_t) src[(माप_प्रकार) y0 * width + (माप_प्रकार) x0];
  uint32_t p10 = (uint32_t) src[(माप_प्रकार) y0 * width + (माप_प्रकार) x1];
  uint32_t p01 = (uint32_t) src[(माप_प्रकार) y1 * width + (माप_प्रकार) x0];
  uint32_t p11 = (uint32_t) src[(माप_प्रकार) y1 * width + (माप_प्रकार) x1];

  भिन्न w00 = omtx * omty;
  भिन्न w10 = tx * omty;
  भिन्न w01 = omtx * ty;
  भिन्न w11 = tx * ty;

  भिन्न b = ((p00 >>  0) & 0xff) * w00 + ((p10 >>  0) & 0xff) * w10 +
	    ((p01 >>  0) & 0xff) * w01 + ((p11 >>  0) & 0xff) * w11;
  भिन्न g = ((p00 >>  8) & 0xff) * w00 + ((p10 >>  8) & 0xff) * w10 +
	    ((p01 >>  8) & 0xff) * w01 + ((p11 >>  8) & 0xff) * w11;
  भिन्न r = ((p00 >> 16) & 0xff) * w00 + ((p10 >> 16) & 0xff) * w10 +
	    ((p01 >> 16) & 0xff) * w01 + ((p11 >> 16) & 0xff) * w11;
  भिन्न a = ((p00 >> 24) & 0xff) * w00 + ((p10 >> 24) & 0xff) * w10 +
	    ((p01 >> 24) & 0xff) * w01 + ((p11 >> 24) & 0xff) * w11;

  फर्काऊ (uint32_t) (b + 0.5f)
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
स्थिर शून्य
ensure_initialized (hb_raster_paint_t *c)
{
  यदि (c->surface_थाक.length) फर्काऊ;

  /* A failed stack push (OOM) leaves the vector in a sticky error
   * state, so initialization can never succeed; bail out cheaply
   * instead of re-paying the surface acquire-and-clear below on
   * every subsequent paint callback. */
  यदि (unlikely (c->surface_थाक.in_error () || c->clip_थाक.in_error ()))
    फर्काऊ;

  /* acquire_surface() clears a full surface; charge its area, so
   * repeated failed initialization attempts cannot re-pay it
   * indefinitely.  Successful initialization re-arms the session
   * budget below, making the charge a no-op for normal sessions. */
  यदि (unlikely (!c->charge_work ((int64_t) c->fixed_extents.width *
				 c->fixed_extents.height)))
    फर्काऊ;

  /* Root surface */
  hb_raster_image_t *जरा = c->acquire_surface ();
  यदि (unlikely (!जरा)) फर्काऊ;

  यदि (hb_color_get_alpha (c->background))
  {
    uint32_t bg = color_to_premul_pixel (c->background);
    hb_raster_extents_t ext;
    hb_raster_image_get_extents (जरा, &ext);
    अचर uint8_t *buf = hb_raster_image_get_buffer (जरा);
    लागि (अचिन्हित y = 0; y < ext.height; y++)
    {
      hb_packed_t<uint32_t> *row =
	(hb_packed_t<uint32_t> *) (buf + (माप_प्रकार) y * ext.stride);
      लागि (अचिन्हित x = 0; x < ext.width; x++)
	row[x] = hb_packed_t<uint32_t> (bg);
    }
  }

  यदि (unlikely (!c->surface_थाक.थप_or_fail (जरा)))
  {
    c->release_surface (जरा);
    फर्काऊ;
  }

  /* Resolve the surface-sized pixel budget now that the surface size is
   * known.  An unlimited policy disables the pixel budget, so leave it.
   * The outline budget is re-resolved to the same value it already holds
   * (nothing has been drawn yet). */
  यदि (c->budget != HB_BUDGET_UNLIMITED)
    c->recharge_budget ();

  /* Initial clip: full coverage rectangle */
  hb_raster_clip_t clip;
  clip.init_full (c->fixed_extents.width, c->fixed_extents.height);
  यदि (unlikely (!c->clip_थाक.थप_or_fail (std::move (clip))))
  {
    c->release_surface (c->surface_थाक.झिक ());
    फर्काऊ;
  }
}

स्थिर शून्य
hb_raster_paint_थप_transform (hb_paint_funcs_t *pfuncs HB_UNUSED,
				शून्य *paint_data,
				भिन्न xx, भिन्न yx,
				भिन्न xy, भिन्न yy,
				भिन्न dx, भिन्न dy,
				शून्य *user_data HB_UNUSED)
{
  hb_raster_paint_t *c = (hb_raster_paint_t *) paint_data;

  ensure_initialized (c);

  hb_transform_t<> t = c->current_transform ();
  t.multiply ({xx, yx, xy, yy, dx, dy});
  (शून्य) c->transform_थाक.push (t);
}

स्थिर शून्य
hb_raster_paint_झिक_transform (hb_paint_funcs_t *pfuncs HB_UNUSED,
			       शून्य *paint_data,
			       शून्य *user_data HB_UNUSED)
{
  hb_raster_paint_t *c = (hb_raster_paint_t *) paint_data;
  यदि (c->transform_थाक.length)
    c->transform_थाक.झिक ();
}

स्थिर hb_bool_t
hb_raster_paint_color_glyph (hb_paint_funcs_t *pfuncs HB_UNUSED,
			     शून्य *paint_data HB_UNUSED,
			     hb_codepoint_t glyph HB_UNUSED,
			     hb_font_t *font HB_UNUSED,
			     शून्य *user_data HB_UNUSED)
{
  फर्काऊ असत्य;
}

प्रकारनिर्देश शून्य (*hb_raster_paint_clip_mask_emit_t) (hb_raster_draw_t *rdr, शून्य *user_data);

/* Attach the surface box to @rdr and seed its outline budget before
 * outlines are drawn into it: curves outside the surface collapse to
 * their chord, and outline traversal and curve flattening charge the
 * seeded budget.  hb_raster_paint_readback_budget() pulls the remainder
 * back into the session afterwards. */
स्थिर शून्य
hb_raster_paint_attach_clip_rdr (hb_raster_paint_t *c,
				 hb_raster_draw_t *rdr,
				 अचर hb_raster_image_t *surf)
{
  hb_raster_draw_set_clip_box (rdr,
			       (भिन्न) surf->extents.x_origin,
			       (भिन्न) surf->extents.y_origin,
			       (भिन्न) surf->extents.x_origin + (भिन्न) surf->extents.width,
			       (भिन्न) surf->extents.y_origin + (भिन्न) surf->extents.height);
  hb_draw_set_budget (hb_raster_draw_get_funcs (rdr), rdr, c->budget_remaining);
}

/* Pull the outline budget the rasterizer has consumed back into the
 * session, so it aggregates across every glyph in the paint walk. */
स्थिर शून्य
hb_raster_paint_readback_budget (hb_raster_paint_t *c,
				 hb_raster_draw_t *rdr)
{
  c->budget_remaining = hb_draw_get_budget_remaining (hb_raster_draw_get_funcs (rdr), rdr);
}

स्थिर शून्य
hb_raster_paint_थप_empty_clip (hb_raster_paint_t *c, अचिन्हित w, अचिन्हित h)
{
  hb_raster_clip_t new_clip = c->acquire_clip (w, h);
  new_clip.init_full (w, h);
  new_clip.is_rect = सत्य;
  new_clip.rect_x0 = new_clip.rect_y0 = 0;
  new_clip.rect_x1 = new_clip.rect_y1 = 0;
  new_clip.min_x = new_clip.min_y = new_clip.max_x = new_clip.max_y = 0;
  (शून्य) c->clip_थाक.push (std::move (new_clip));
}

स्थिर तार्किक
hb_raster_paint_intersect_mask (अचर hb_raster_image_t *surf,
				अचर hb_raster_extents_t &mask_ext,
				अचर hb_raster_clip_t &clip,
				अचिन्हित w, अचिन्हित h,
				int64_t *mask_x0, int64_t *mask_y0,
				अचिन्हित *ix0, अचिन्हित *iy0,
				अचिन्हित *ix1, अचिन्हित *iy1)
{
  /* Convert mask extents from surface coordinates to clip-buffer coordinates. */
  int64_t mx0 = (int64_t) mask_ext.x_origin - (int64_t) surf->extents.x_origin;
  int64_t my0 = (int64_t) mask_ext.y_origin - (int64_t) surf->extents.y_origin;

  int64_t x0 = hb_max ((int64_t) clip.min_x, hb_max (mx0, (int64_t) 0));
  int64_t y0 = hb_max ((int64_t) clip.min_y, hb_max (my0, (int64_t) 0));
  int64_t x1 = hb_min ((int64_t) clip.max_x, hb_min (mx0 + mask_ext.width, (int64_t) w));
  int64_t y1 = hb_min ((int64_t) clip.max_y, hb_min (my0 + mask_ext.height, (int64_t) h));

  यदि (x0 >= x1 || y0 >= y1) फर्काऊ असत्य;

  *mask_x0 = mx0;
  *mask_y0 = my0;
  *ix0 = (अचिन्हित) x0;
  *iy0 = (अचिन्हित) y0;
  *ix1 = (अचिन्हित) x1;
  *iy1 = (अचिन्हित) y1;
  फर्काऊ सत्य;
}

/* Render whatever edges have been accumulated into @rdr and
 * push the result as a new clip on the stack, intersecting
 * with the existing clip.  Used by the emitter-based clip
 * helpers (push_clip_glyph) and by push_clip_path_end once
 * the caller has drawn the path into @rdr. */
स्थिर शून्य
hb_raster_paint_finalize_path_clip (hb_raster_paint_t *c,
				    hb_raster_draw_t *rdr,
				    hb_raster_image_t *surf,
				    अचिन्हित w, अचिन्हित h)
{
  यदि (unlikely (!c->precharge_work (hb_raster_draw_get_pixel_work (rdr, h, w))))
  {
    hb_raster_draw_clear (rdr);
    hb_raster_paint_थप_empty_clip (c, w, h);
    फर्काऊ;
  }

  hb_raster_image_t *mask_img = hb_raster_draw_render (rdr);

  यदि (unlikely (!mask_img))
  {
    hb_raster_paint_थप_empty_clip (c, w, h);
    फर्काऊ;
  }

  /* Charge the mask render (its own area; may exceed the surface) plus
   * the clip-buffer pass below. */
  hb_raster_extents_t mask_render_ext;
  hb_raster_image_get_extents (mask_img, &mask_render_ext);
  यदि (unlikely (!c->charge_work ((int64_t) mask_render_ext.width * mask_render_ext.height +
				 (int64_t) w * h)))
  {
    hb_raster_draw_recycle_image (rdr, mask_img);
    hb_raster_paint_थप_empty_clip (c, w, h);
    फर्काऊ;
  }

  hb_raster_clip_t new_clip = c->acquire_clip (w, h);

  /* Allocate alpha buffer and intersect with previous clip */
  माप_प्रकार clip_size = (माप_प्रकार) new_clip.stride * h;
  यदि (unlikely (clip_size > HB_RASTER_MAX_BUFFER_SIZE ||
                !new_clip.alpha.resize ((अचिन्हित) clip_size)))
  {
    hb_raster_draw_recycle_image (rdr, mask_img);
    hb_raster_paint_थप_empty_clip (c, w, h);
    फर्काऊ;
  }
  hb_memset (new_clip.alpha.arrayZ, 0, (अचिन्हित) clip_size);

  अचर uint8_t *mask_buf = hb_raster_image_get_buffer (mask_img);
  hb_raster_extents_t mask_ext;
  hb_raster_image_get_extents (mask_img, &mask_ext);
  अचर hb_raster_clip_t &old_clip = c->current_clip ();
  यदि (unlikely (!old_clip.has_valid_alpha_mask ()))
  {
    hb_raster_draw_recycle_image (rdr, mask_img);
    hb_raster_paint_थप_empty_clip (c, w, h);
    फर्काऊ;
  }

  int64_t mask_x0, mask_y0;
  अचिन्हित ix0, iy0, ix1, iy1;
  यदि (!hb_raster_paint_intersect_mask (surf, mask_ext, old_clip, w, h,
				       &mask_x0, &mask_y0,
				       &ix0, &iy0, &ix1, &iy1))
  {
    hb_raster_draw_recycle_image (rdr, mask_img);
    hb_raster_paint_थप_empty_clip (c, w, h);
    फर्काऊ;
  }

  new_clip.min_x = w; new_clip.min_y = h;
  new_clip.max_x = 0; new_clip.max_y = 0;

  यदि (old_clip.is_rect)
  {
    लागि (अचिन्हित y = iy0; y < iy1; y++)
    {
      अचर uint8_t *mask_row = mask_buf + (अचिन्हित) ((int64_t) y - mask_y0) * mask_ext.stride;
      uint8_t *out_row = new_clip.alpha.arrayZ + y * new_clip.stride;
      अचिन्हित row_min = ix1;
      अचिन्हित row_max = ix0;
      अचिन्हित mx = (अचिन्हित) ((int64_t) ix0 - mask_x0);
      लागि (अचिन्हित x = ix0; x < ix1; x++)
      {
	uint8_t a = mask_row[mx++];
	out_row[x] = a;
	यदि (a && row_min == ix1)
	{
	  row_min = x;
	  row_max = x + 1;
	}
	अन्यथा यदि (a)
	  row_max = x + 1;
      }
      यदि (row_min < row_max)
      {
	new_clip.min_x = hb_min (new_clip.min_x, row_min);
	new_clip.min_y = hb_min (new_clip.min_y, y);
	new_clip.max_x = hb_max (new_clip.max_x, row_max);
	new_clip.max_y = hb_max (new_clip.max_y, y + 1);
      }
    }
  }
  अन्यथा
  {
    लागि (अचिन्हित y = iy0; y < iy1; y++)
    {
      अचर uint8_t *old_row = old_clip.alpha.arrayZ + y * old_clip.stride;
      अचर uint8_t *mask_row = mask_buf + (अचिन्हित) ((int64_t) y - mask_y0) * mask_ext.stride;
      uint8_t *out_row = new_clip.alpha.arrayZ + y * new_clip.stride;
      अचिन्हित row_min = ix1;
      अचिन्हित row_max = ix0;
      लागि (अचिन्हित x = ix0; x < ix1; x++)
      {
	अचिन्हित mx = (अचिन्हित) ((int64_t) x - mask_x0);
	uint8_t a = hb_raster_div255 (mask_row[mx] * old_row[x]);
	out_row[x] = a;
	यदि (a)
	{
	  row_min = hb_min (row_min, x);
	  row_max = x + 1;
	}
      }
      यदि (row_min < row_max)
      {
	new_clip.min_x = hb_min (new_clip.min_x, row_min);
	new_clip.min_y = hb_min (new_clip.min_y, y);
	new_clip.max_x = hb_max (new_clip.max_x, row_max);
	new_clip.max_y = hb_max (new_clip.max_y, y + 1);
      }
    }
  }

  hb_raster_draw_recycle_image (rdr, mask_img);
  यदि (unlikely (!c->clip_थाक.थप_or_fail (std::move (new_clip))))
    hb_raster_paint_थप_empty_clip (c, w, h);
}

स्थिर शून्य
hb_raster_paint_थप_clip_from_emitter (hb_raster_paint_t *c,
					hb_raster_paint_clip_mask_emit_t emit,
					शून्य *emit_data)
{
  ensure_initialized (c);

  hb_raster_image_t *surf = c->current_surface ();
  यदि (unlikely (!surf)) फर्काऊ;

  अचिन्हित w = surf->extents.width;
  अचिन्हित h = surf->extents.height;

  /* Out of budget: skip the glyph-outline extraction entirely, so
   * per-glyph outline limits cannot multiply with the caller's
   * paint-graph traversal limits. */
  यदि (unlikely (c->budget_remaining < 0))
  {
    hb_raster_paint_थप_empty_clip (c, w, h);
    फर्काऊ;
  }

  hb_raster_draw_t *rdr = c->clip_rdr;
  hb_transform_t<> t = c->current_effective_transform ();
  hb_raster_draw_set_transform (rdr, t.xx, t.yx, t.xy, t.yy, t.x0, t.y0);
  hb_raster_paint_attach_clip_rdr (c, rdr, surf);
  emit (rdr, emit_data);
  hb_raster_paint_readback_budget (c, rdr);

  hb_raster_paint_finalize_path_clip (c, rdr, surf, w, h);
}

संरचना hb_raster_paint_glyph_clip_data_t
{
  hb_codepoint_t glyph;
  hb_font_t *font;
};

स्थिर शून्य
hb_raster_paint_emit_clip_glyph_mask (hb_raster_draw_t *rdr, शून्य *user_data)
{
  hb_raster_paint_glyph_clip_data_t *data = (hb_raster_paint_glyph_clip_data_t *) user_data;
  /* Let draw-render choose tight glyph extents; we map by mask origin below. */
  hb_font_draw_glyph (data->font, data->glyph, hb_raster_draw_get_funcs (rdr), rdr);
}

स्थिर शून्य
hb_raster_paint_थप_clip_glyph (hb_paint_funcs_t *pfuncs HB_UNUSED,
				 शून्य *paint_data,
				 hb_codepoint_t glyph,
				 hb_font_t *font,
				 शून्य *user_data HB_UNUSED)
{
  hb_raster_paint_t *c = (hb_raster_paint_t *) paint_data;
  hb_raster_paint_glyph_clip_data_t data = {glyph, font};
  hb_raster_paint_थप_clip_from_emitter (c, hb_raster_paint_emit_clip_glyph_mask, &data);
}

स्थिर शून्य
hb_raster_paint_थप_clip_rectangle (hb_paint_funcs_t *pfuncs HB_UNUSED,
				     शून्य *paint_data,
				     भिन्न xmin, भिन्न ymin,
				     भिन्न xmax, भिन्न ymax,
				     शून्य *user_data HB_UNUSED)
{
  hb_raster_paint_t *c = (hb_raster_paint_t *) paint_data;

  ensure_initialized (c);

  यदि (!c->surface_थाक.length) फर्काऊ;

  hb_transform_t<> t = c->current_effective_transform ();

  hb_raster_image_t *surf = c->current_surface ();
  यदि (unlikely (!surf)) फर्काऊ;

  अचिन्हित w = surf->extents.width;
  अचिन्हित h = surf->extents.height;
  तार्किक is_axis_aligned = (t.xy == 0.f && t.yx == 0.f);

  /* Transform the four corners to pixel space */
  भिन्न cx[4], cy[4];
  cx[0] = xmin; cy[0] = ymin;
  cx[1] = xmax; cy[1] = ymin;
  cx[2] = xmax; cy[2] = ymax;
  cx[3] = xmin; cy[3] = ymax;
  लागि (अचिन्हित i = 0; i < 4; i++)
    t.transform_point (cx[i], cy[i]);

  /* Compute bounding box in pixel coords */
  भिन्न fmin_x = cx[0], fmin_y = cy[0], fmax_x = cx[0], fmax_y = cy[0];
  लागि (अचिन्हित i = 1; i < 4; i++)
  {
    fmin_x = hb_min (fmin_x, cx[i]); fmin_y = hb_min (fmin_y, cy[i]);
    fmax_x = hb_max (fmax_x, cx[i]); fmax_y = hb_max (fmax_y, cy[i]);
  }

  /* A spec-legal but very large transform can push these bounds past the int
     range or to non-finite values; a bare float->int conversion would then be
     undefined.  Saturate through hb_clamp_to<> (which also maps NaN to
     INT32_MIN); the subtraction is done in double to avoid overflow. */
  पूर्णाङ्क px0 = hb_clamp_to<int32_t> ((द्विगुण) floorf (fmin_x) - surf->extents.x_origin);
  पूर्णाङ्क py0 = hb_clamp_to<int32_t> ((द्विगुण) floorf (fmin_y) - surf->extents.y_origin);
  पूर्णाङ्क px1 = hb_clamp_to<int32_t> ((द्विगुण) ceilf  (fmax_x) - surf->extents.x_origin);
  पूर्णाङ्क py1 = hb_clamp_to<int32_t> ((द्विगुण) ceilf  (fmax_y) - surf->extents.y_origin);

  /* Clamp to surface bounds */
  px0 = hb_max (px0, 0);
  py0 = hb_max (py0, 0);
  px1 = hb_min (px1, (पूर्णाङ्क) w);
  py1 = hb_min (py1, (पूर्णाङ्क) h);

  अचर hb_raster_clip_t &old_clip = c->current_clip ();
  यदि (unlikely (!old_clip.has_valid_alpha_mask ()))
  {
    hb_raster_paint_थप_empty_clip (c, w, h);
    फर्काऊ;
  }

  /* All paths below except rect-on-rect run a full clip-buffer pass. */
  यदि (!(is_axis_aligned && old_clip.is_rect) &&
      unlikely (!c->charge_work ((int64_t) w * h)))
  {
    hb_raster_paint_थप_empty_clip (c, w, h);
    फर्काऊ;
  }

  hb_raster_clip_t new_clip = c->acquire_clip (w, h);

  यदि (is_axis_aligned && old_clip.is_rect)
  {
    /* Fast path: axis-aligned rect-on-rect intersection */
    new_clip.is_rect = सत्य;
    new_clip.rect_x0 = hb_max (px0, old_clip.rect_x0);
    new_clip.rect_y0 = hb_max (py0, old_clip.rect_y0);
    new_clip.rect_x1 = hb_min (px1, old_clip.rect_x1);
    new_clip.rect_y1 = hb_min (py1, old_clip.rect_y1);
    new_clip.update_bounds_from_rect ();
  }
  अन्यथा यदि (is_axis_aligned)
  {
    /* Fast path: intersect an axis-aligned rect with an existing alpha clip. */
    new_clip.is_rect = असत्य;
    माप_प्रकार clip_size = (माप_प्रकार) new_clip.stride * h;
    यदि (unlikely (clip_size > HB_RASTER_MAX_BUFFER_SIZE ||
                  !new_clip.alpha.resize ((अचिन्हित) clip_size)))
    {
      hb_raster_paint_थप_empty_clip (c, w, h);
      फर्काऊ;
    }
    hb_memset (new_clip.alpha.arrayZ, 0, (अचिन्हित) clip_size);

    पूर्णाङ्क iy0_i = hb_max (py0, (पूर्णाङ्क) old_clip.min_y);
    पूर्णाङ्क iy1_i = hb_min (py1, (पूर्णाङ्क) old_clip.max_y);
    पूर्णाङ्क ix0_i = hb_max (px0, (पूर्णाङ्क) old_clip.min_x);
    पूर्णाङ्क ix1_i = hb_min (px1, (पूर्णाङ्क) old_clip.max_x);

    यदि (ix0_i < ix1_i && iy0_i < iy1_i)
    {
      अचिन्हित iy0 = (अचिन्हित) iy0_i;
      अचिन्हित iy1 = (अचिन्हित) iy1_i;
      अचिन्हित ix0 = (अचिन्हित) ix0_i;
      अचिन्हित ix1 = (अचिन्हित) ix1_i;

      लागि (अचिन्हित y = iy0; y < iy1; y++)
      {
        अचर uint8_t *old_row = old_clip.alpha.arrayZ + y * old_clip.stride;
        uint8_t *new_row = new_clip.alpha.arrayZ + y * new_clip.stride;
        hb_memcpy (new_row + ix0, old_row + ix0, ix1 - ix0);
      }
      new_clip.min_x = ix0;
      new_clip.min_y = iy0;
      new_clip.max_x = ix1;
      new_clip.max_y = iy1;
    }
  }
  अन्यथा
  {
    /* General case: rasterize transformed quad as alpha mask */
    new_clip.is_rect = असत्य;
    माप_प्रकार clip_size = (माप_प्रकार) new_clip.stride * h;
    यदि (unlikely (clip_size > HB_RASTER_MAX_BUFFER_SIZE ||
                  !new_clip.alpha.resize ((अचिन्हित) clip_size)))
    {
      hb_raster_paint_थप_empty_clip (c, w, h);
      फर्काऊ;
    }
    hb_memset (new_clip.alpha.arrayZ, 0, (अचिन्हित) clip_size);

    /* Convert quad corners to pixel-relative coords */
    भिन्न qx[4], qy[4];
    पूर्णाङ्क ox = surf->extents.x_origin;
    पूर्णाङ्क oy = surf->extents.y_origin;
    लागि (अचिन्हित i = 0; i < 4; i++)
    {
      qx[i] = cx[i] - ox;
      qy[i] = cy[i] - oy;
    }

    /* For each pixel in the bounding box, test if inside the quad
     * using cross-product edge tests (winding order). */
    पूर्णाङ्क iy0_i = hb_max (py0, (पूर्णाङ्क) old_clip.min_y);
    पूर्णाङ्क iy1_i = hb_min (py1, (पूर्णाङ्क) old_clip.max_y);
    पूर्णाङ्क ix0_i = hb_max (px0, (पूर्णाङ्क) old_clip.min_x);
    पूर्णाङ्क ix1_i = hb_min (px1, (पूर्णाङ्क) old_clip.max_x);
    new_clip.min_x = w; new_clip.min_y = h;
    new_clip.max_x = 0; new_clip.max_y = 0;

    यदि (ix0_i < ix1_i && iy0_i < iy1_i)
    {
      अचिन्हित iy0 = (अचिन्हित) iy0_i;
      अचिन्हित iy1 = (अचिन्हित) iy1_i;
      अचिन्हित ix0 = (अचिन्हित) ix0_i;
      अचिन्हित ix1 = (अचिन्हित) ix1_i;

      /* Precompute edge normals for point-in-quad test.
       * Edge i goes from corner i to corner (i+1)%4.
       * Normal = (dy, -dx); inside test: dot(normal, p-corner) >= 0 */
	      भिन्न enx[4], eny[4], ed[4];
	      लागि (अचिन्हित i = 0; i < 4; i++)
	      {
		अचिन्हित j = (i + 1) & 3;
		भिन्न edx = qx[j] - qx[i], edy = qy[j] - qy[i];
		enx[i] = edy;       /* normal x */
		eny[i] = -edx;      /* normal y */
		ed[i] = enx[i] * qx[i] + eny[i] * qy[i]; /* distance threshold */
	      }
	      भिन्न area2 = 0.f;
	      लागि (अचिन्हित i = 0; i < 4; i++)
	      {
		अचिन्हित j = (i + 1) & 3;
		area2 += qx[i] * qy[j] - qx[j] * qy[i];
	      }
	      तार्किक ccw = area2 >= 0.f;

      यदि (old_clip.is_rect)
      {
	लागि (अचिन्हित y = iy0; y < iy1; y++)
	  लागि (अचिन्हित x = ix0; x < ix1; x++)
	  {
	    भिन्न px_f = x + 0.5f, py_f = y + 0.5f;
	    /* Test if pixel center is inside the quad */
		    तार्किक inside = सत्य;
		    लागि (अचिन्हित i = 0; i < 4; i++)
		    {
		      भिन्न d = enx[i] * px_f + eny[i] * py_f;
		      यदि (ccw ? d < ed[i] : d > ed[i])
		      {
			inside = असत्य;
			रोक;
		      }
		    }
	    uint8_t a = inside ? 255 : 0;
	    new_clip.alpha[y * new_clip.stride + x] = a;
	    यदि (a)
	    {
	      new_clip.min_x = hb_min (new_clip.min_x, x);
	      new_clip.min_y = hb_min (new_clip.min_y, y);
	      new_clip.max_x = hb_max (new_clip.max_x, x + 1);
	      new_clip.max_y = hb_max (new_clip.max_y, y + 1);
	    }
	  }
      }
      अन्यथा
      {
	लागि (अचिन्हित y = iy0; y < iy1; y++)
	{
	  अचर uint8_t *old_row = old_clip.alpha.arrayZ + y * old_clip.stride;
	  लागि (अचिन्हित x = ix0; x < ix1; x++)
	  {
	    भिन्न px_f = x + 0.5f, py_f = y + 0.5f;
	    /* Test if pixel center is inside the quad */
		    तार्किक inside = सत्य;
		    लागि (अचिन्हित i = 0; i < 4; i++)
		    {
		      भिन्न d = enx[i] * px_f + eny[i] * py_f;
		      यदि (ccw ? d < ed[i] : d > ed[i])
		      {
			inside = असत्य;
			रोक;
		      }
		    }
	    uint8_t a = inside ? old_row[x] : 0;
	    new_clip.alpha[y * new_clip.stride + x] = a;
	    यदि (a)
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

  यदि (unlikely (!c->clip_थाक.थप_or_fail (std::move (new_clip))))
    hb_raster_paint_थप_empty_clip (c, surf->extents.width, surf->extents.height);
}

स्थिर hb_draw_funcs_t *
hb_raster_paint_थप_clip_path_start (hb_paint_funcs_t *pfuncs HB_UNUSED,
				      शून्य *paint_data,
				      शून्य **draw_data,
				      शून्य *user_data HB_UNUSED)
{
  hb_raster_paint_t *c = (hb_raster_paint_t *) paint_data;

  ensure_initialized (c);

  /* Prime clip_rdr with the current effective transform; the
   * caller then drives hb_draw_*() into it, and _end renders
   * the accumulated edges into a mask and intersects with the
   * current clip. */
  यदि (unlikely (!c->surface_थाक.length || !c->clip_rdr))
  {
    *draw_data = nullptr;
    फर्काऊ nullptr;
  }

  hb_raster_draw_t *rdr = c->clip_rdr;
  hb_transform_t<> t = c->current_effective_transform ();
  hb_raster_draw_set_transform (rdr, t.xx, t.yx, t.xy, t.yy, t.x0, t.y0);
  hb_raster_image_t *surf = c->current_surface ();
  यदि (likely (surf))
    hb_raster_paint_attach_clip_rdr (c, rdr, surf);

  *draw_data = rdr;
  फर्काऊ hb_raster_draw_get_funcs (rdr);
}

स्थिर शून्य
hb_raster_paint_थप_clip_path_end (hb_paint_funcs_t *pfuncs HB_UNUSED,
				    शून्य *paint_data,
				    शून्य *user_data HB_UNUSED)
{
  hb_raster_paint_t *c = (hb_raster_paint_t *) paint_data;

  hb_raster_image_t *surf = c->current_surface ();
  यदि (unlikely (!surf || !c->clip_rdr)) फर्काऊ;

  अचिन्हित w = surf->extents.width;
  अचिन्हित h = surf->extents.height;

  hb_raster_paint_readback_budget (c, c->clip_rdr);
  यदि (unlikely (c->budget_remaining < 0))
  {
    hb_raster_draw_clear (c->clip_rdr);
    hb_raster_paint_थप_empty_clip (c, w, h);
    फर्काऊ;
  }

  hb_raster_paint_finalize_path_clip (c, c->clip_rdr, surf, w, h);
}

स्थिर शून्य
hb_raster_paint_झिक_clip (hb_paint_funcs_t *pfuncs HB_UNUSED,
			  शून्य *paint_data,
			  शून्य *user_data HB_UNUSED)
{
  hb_raster_paint_t *c = (hb_raster_paint_t *) paint_data;
  यदि (!c->clip_थाक.length) फर्काऊ;
  c->release_clip (c->clip_थाक.झिक ());
}

स्थिर शून्य
hb_raster_paint_थप_group (hb_paint_funcs_t *pfuncs HB_UNUSED,
			    शून्य *paint_data,
			    शून्य *user_data HB_UNUSED)
{
  hb_raster_paint_t *c = (hb_raster_paint_t *) paint_data;

  ensure_initialized (c);

  /* acquire_surface() clears a full surface; charge its area. */
  यदि (unlikely (!c->charge_work ((int64_t) c->fixed_extents.width * c->fixed_extents.height)))
    फर्काऊ;

  hb_raster_image_t *new_surf = c->acquire_surface ();
  यदि (unlikely (!new_surf)) फर्काऊ;
  यदि (unlikely (!c->surface_थाक.थप_or_fail (new_surf)))
    c->release_surface (new_surf);
}

स्थिर शून्य
hb_raster_paint_झिक_group (hb_paint_funcs_t *pfuncs HB_UNUSED,
			   शून्य *paint_data,
			   hb_paint_composite_mode_t mode,
			   शून्य *user_data HB_UNUSED)
{
  hb_raster_paint_t *c = (hb_raster_paint_t *) paint_data;

  यदि (c->surface_थाक.length < 2) फर्काऊ;

  hb_raster_image_t *src = c->surface_थाक.झिक ();
  hb_raster_image_t *dst = c->current_surface ();

  यदि (dst && src &&
      likely (c->charge_work ((int64_t) dst->extents.width * dst->extents.height)))
    hb_raster_image_composite (dst, src, mode);

  c->release_surface (src);
}

स्थिर शून्य
hb_raster_paint_solid (hb_raster_paint_t *c,
		       hb_raster_image_t *surf,
		       hb_color_t color,
		       hb_raster_image_t *mask_img)
{
  uint32_t premul = color_to_premul_pixel (color);
  uint8_t premul_a = (uint8_t) (premul >> 24);
  अचर hb_raster_clip_t &clip = c->current_clip ();

  अचिन्हित stride = surf->extents.stride;
  यदि (clip.min_x >= clip.max_x || clip.min_y >= clip.max_y) फर्काऊ;
  यदि (premul_a == 0) फर्काऊ;

  अचिन्हित ix0 = clip.min_x, iy0 = clip.min_y;
  अचिन्हित ix1 = clip.max_x, iy1 = clip.max_y;
  अचर uint8_t *mask_buf = nullptr;
  hb_raster_extents_t mask_ext;
  int64_t mask_x0 = 0, mask_y0 = 0;
  यदि (mask_img)
  {
    mask_buf = hb_raster_image_get_buffer (mask_img);
    hb_raster_image_get_extents (mask_img, &mask_ext);
    यदि (!hb_raster_paint_intersect_mask (surf, mask_ext, clip,
					 surf->extents.width, surf->extents.height,
					 &mask_x0, &mask_y0,
					 &ix0, &iy0, &ix1, &iy1))
      फर्काऊ;
  }

  यदि (unlikely (!c->charge_work ((int64_t) (ix1 - ix0) * (iy1 - iy0))))
    फर्काऊ;

  यदि (likely (!clip.is_rect))
  {
    लागि (अचिन्हित y = iy0; y < iy1; y++)
    {
      hb_packed_t<uint32_t> *__restrict row = (hb_packed_t<uint32_t> *) (surf->buffer.arrayZ + y * stride);
      अचर uint8_t *__restrict clip_row = clip.alpha.arrayZ + y * clip.stride;
      अचर uint8_t *__restrict mask_row = mask_buf ? mask_buf + (अचिन्हित) ((int64_t) y - mask_y0) * mask_ext.stride
						    : nullptr;
      अचिन्हित mx = (अचिन्हित) ((int64_t) ix0 - mask_x0);
      लागि (अचिन्हित x = ix0; x < ix1; x++)
      {
	uint8_t clip_alpha = clip_row[x];
	यदि (mask_row)
	  clip_alpha = hb_raster_div255 (mask_row[mx++] * clip_alpha);
	यदि (clip_alpha == 0) जारी;
	यदि (clip_alpha == 255)
	{
	  यदि (premul_a == 255)
	    row[x] = hb_packed_t<uint32_t> (premul);
	  अन्यथा
	    row[x] = hb_packed_t<uint32_t> (hb_raster_src_over (premul, (uint32_t) row[x]));
	}
	अन्यथा
	{
	  uint32_t src = hb_raster_alpha_mul (premul, clip_alpha);
	  row[x] = hb_packed_t<uint32_t> (hb_raster_src_over (src, (uint32_t) row[x]));
	}
      }
    }
  }
  अन्यथा यदि (mask_buf)
  {
    लागि (अचिन्हित y = iy0; y < iy1; y++)
    {
      hb_packed_t<uint32_t> *__restrict row = (hb_packed_t<uint32_t> *) (surf->buffer.arrayZ + y * stride);
      अचर uint8_t *__restrict mask_row = mask_buf + (अचिन्हित) ((int64_t) y - mask_y0) * mask_ext.stride;
      अचिन्हित mx = (अचिन्हित) ((int64_t) ix0 - mask_x0);
      लागि (अचिन्हित x = ix0; x < ix1; x++)
      {
	uint8_t mask_alpha = mask_row[mx++];
	यदि (mask_alpha == 0) जारी;
	uint32_t src = hb_raster_alpha_mul (premul, mask_alpha);
	row[x] = hb_packed_t<uint32_t> (hb_raster_src_over (src, (uint32_t) row[x]));
      }
    }
  }
  अन्यथा
  {
    लागि (अचिन्हित y = clip.min_y; y < clip.max_y; y++)
    {
      hb_packed_t<uint32_t> *row = (hb_packed_t<uint32_t> *) (surf->buffer.arrayZ + y * stride);
      लागि (अचिन्हित x = clip.min_x; x < clip.max_x; x++)
	row[x] = hb_packed_t<uint32_t> (hb_raster_src_over (premul, (uint32_t) row[x]));
    }
  }
}

स्थिर शून्य
hb_raster_paint_color (hb_paint_funcs_t *pfuncs HB_UNUSED,
		       शून्य *paint_data,
		       hb_bool_t is_foreground HB_UNUSED,
		       hb_color_t color,
		       शून्य *user_data HB_UNUSED)
{
  hb_raster_paint_t *c = (hb_raster_paint_t *) paint_data;

  ensure_initialized (c);

  hb_raster_image_t *surf = c->current_surface ();
  यदि (unlikely (!surf)) फर्काऊ;

  hb_raster_paint_solid (c, surf, color, nullptr);
}

स्थिर शून्य
hb_raster_paint_fill_glyph (hb_paint_funcs_t *pfuncs HB_UNUSED,
			    शून्य *paint_data,
			    hb_codepoint_t glyph,
			    hb_font_t *font,
			    hb_bool_t is_foreground HB_UNUSED,
			    hb_color_t color,
			    शून्य *user_data HB_UNUSED)
{
  hb_raster_paint_t *c = (hb_raster_paint_t *) paint_data;

  ensure_initialized (c);

  hb_raster_image_t *surf = c->current_surface ();
  यदि (unlikely (!surf)) फर्काऊ;

  /* Out of budget: skip the glyph-outline extraction entirely. */
  यदि (unlikely (c->budget_remaining < 0)) फर्काऊ;

  hb_raster_draw_t *rdr = c->clip_rdr;
  hb_transform_t<> t = c->current_effective_transform ();
  hb_raster_draw_set_transform (rdr, t.xx, t.yx, t.xy, t.yy, t.x0, t.y0);
  hb_raster_paint_attach_clip_rdr (c, rdr, surf);

  hb_raster_paint_glyph_clip_data_t data = {glyph, font};
  hb_raster_paint_emit_clip_glyph_mask (rdr, &data);
  hb_raster_paint_readback_budget (c, rdr);

  यदि (unlikely (!c->precharge_work (hb_raster_draw_get_pixel_work (rdr,
							  surf->extents.height,
							  surf->extents.width))))
  {
    hb_raster_draw_clear (rdr);
    फर्काऊ;
  }

  hb_raster_image_t *mask_img = hb_raster_draw_render (rdr);
  यदि (unlikely (!mask_img)) फर्काऊ;

  hb_raster_extents_t mask_ext;
  hb_raster_image_get_extents (mask_img, &mask_ext);
  यदि (likely (c->charge_work ((int64_t) mask_ext.width * mask_ext.height)))
    hb_raster_paint_solid (c, surf, color, mask_img);

  hb_raster_draw_recycle_image (rdr, mask_img);
}

स्थिर hb_bool_t
hb_raster_paint_image (hb_paint_funcs_t *pfuncs HB_UNUSED,
		       शून्य *paint_data,
		       hb_blob_t *blob,
		       अचिन्हित width,
		       अचिन्हित height,
		       hb_tag_t format,
		       भिन्न slant HB_UNUSED,
		       hb_glyph_extents_t *extents,
		       शून्य *user_data HB_UNUSED)
{
  hb_raster_paint_t *c = (hb_raster_paint_t *) paint_data;

  ensure_initialized (c);

  /* Out of pixel budget: skip, including the image decode below.  Image
   * compositing is pixel work and does no outline traversal. */
  यदि (unlikely (c->pixel_remaining < 0)) फर्काऊ असत्य;

  अचिन्हित src_width = width;
  अचिन्हित src_height = height;
  अचर hb_packed_t<uint32_t> *src_data = nullptr;
  hb_raster_image_t decoded_png;

  यदि (format == HB_PAINT_IMAGE_FORMAT_BGRA)
  {
    यदि (src_width == 0 || src_height == 0)
      फर्काऊ असत्य;
    यदि (src_width > (अचिन्हित) पूर्णांक_उच्च || src_height > (अचिन्हित) पूर्णांक_उच्च)
      फर्काऊ असत्य;

    अचिन्हित data_len;
    अचर uint8_t *data = (अचर uint8_t *) hb_blob_get_data (blob, &data_len);
    माप_प्रकार pixel_गन्ती = (माप_प्रकार) src_width * (माप_प्रकार) src_height;
    यदि (src_width && pixel_गन्ती / src_width != src_height)
      फर्काऊ असत्य;
    यदि (pixel_गन्ती > (माप_प्रकार) -1 / 4u)
      फर्काऊ असत्य;
    माप_प्रकार required_size = pixel_गन्ती * 4u;
    यदि (!data || (माप_प्रकार) data_len < required_size)
      फर्काऊ असत्य;

    src_data = (अचर hb_packed_t<uint32_t> *) data;
  }
  अन्यथा यदि (format == HB_PAINT_IMAGE_FORMAT_PNG)
  {
#ifdef HAVE_PNG
    यदि (!decoded_png.deserialize_from_png (blob))
      फर्काऊ असत्य;
    src_width = decoded_png.extents.width;
    src_height = decoded_png.extents.height;
    src_data = (अचर hb_packed_t<uint32_t> *) decoded_png.buffer.arrayZ;
#अन्यथा
    फर्काऊ असत्य;
#endif
  }
  अन्यथा
    फर्काऊ असत्य;

  hb_raster_image_t *surf = c->current_surface ();
  यदि (unlikely (!surf)) फर्काऊ असत्य;
  यदि (!extents) फर्काऊ असत्य;

  अचर hb_raster_clip_t &clip = c->current_clip ();
  hb_transform_t<> t = c->current_effective_transform ();

  /* Compute inverse transform for sampling */
  भिन्न det = t.xx * t.yy - t.xy * t.yx;
  यदि (fabsf (det) < 1e-10f) फर्काऊ असत्य;
  भिन्न inv_det = 1.f / det;
  भिन्न inv_xx =  t.yy * inv_det;
  भिन्न inv_xy = -t.xy * inv_det;
  भिन्न inv_yx = -t.yx * inv_det;
  भिन्न inv_yy =  t.xx * inv_det;
  भिन्न inv_x0 = (t.xy * t.y0 - t.yy * t.x0) * inv_det;
  भिन्न inv_y0 = (t.yx * t.x0 - t.xx * t.y0) * inv_det;

  अचिन्हित surf_stride = surf->extents.stride;
  पूर्णाङ्क ox = surf->extents.x_origin;
  पूर्णाङ्क oy = surf->extents.y_origin;

  /* Image source rectangle in glyph space */
  भिन्न img_x = extents->x_bearing;
  भिन्न img_y = extents->y_bearing;
  भिन्न img_sx = (भिन्न) extents->width / src_width;
  भिन्न img_sy = (भिन्न) extents->height / src_height;
  यदि (fabsf (img_sx) < 1e-10f || fabsf (img_sy) < 1e-10f)
    फर्काऊ असत्य;

  {
    अचिन्हित clip_w = clip.max_x > clip.min_x ? clip.max_x - clip.min_x : 0;
    अचिन्हित clip_h = clip.max_y > clip.min_y ? clip.max_y - clip.min_y : 0;
    यदि (unlikely (!c->charge_work ((int64_t) clip_w * clip_h +
				   (int64_t) src_width * src_height)))
      फर्काऊ असत्य;
  }

  यदि (clip.is_rect)
  {
    लागि (अचिन्हित py = clip.min_y; py < clip.max_y; py++)
    {
      hb_packed_t<uint32_t> *row = (hb_packed_t<uint32_t> *) (surf->buffer.arrayZ + py * surf_stride);
      भिन्न gx = inv_xx * (भिन्न) ((पूर्णाङ्क) clip.min_x + ox) + inv_xy * (भिन्न) ((पूर्णाङ्क) py + oy) + inv_x0;
      भिन्न gy = inv_yx * (भिन्न) ((पूर्णाङ्क) clip.min_x + ox) + inv_yy * (भिन्न) ((पूर्णाङ्क) py + oy) + inv_y0;
      लागि (अचिन्हित px = clip.min_x; px < clip.max_x; px++)
      {
	/* Map glyph space to image texel; bilinear reconstruction. */
	भिन्न ix = (gx - img_x) / img_sx;
	भिन्न iy = (भिन्न) (src_height - 1) - (gy - img_y) / img_sy;

	यदि (unlikely (!std::है_सीमित (ix) || !std::है_सीमित (iy)) ||
	    ix < 0.f || iy < 0.f ||
	    ix > (भिन्न) (src_width - 1) || iy > (भिन्न) (src_height - 1))
	{
	  gx += inv_xx;
	  gy += inv_yx;
	  जारी;
	}

	uint32_t src_px = hb_raster_sample_bilinear_premul (src_data, src_width, src_height,
							     ix, iy);
	row[px] = hb_packed_t<uint32_t> (hb_raster_src_over (src_px, (uint32_t) row[px]));
	gx += inv_xx;
	gy += inv_yx;
      }
    }
  }
  अन्यथा
  {
    लागि (अचिन्हित py = clip.min_y; py < clip.max_y; py++)
    {
      hb_packed_t<uint32_t> *row = (hb_packed_t<uint32_t> *) (surf->buffer.arrayZ + py * surf_stride);
      अचर uint8_t *clip_row = clip.alpha.arrayZ + py * clip.stride;
      भिन्न gx = inv_xx * (भिन्न) ((पूर्णाङ्क) clip.min_x + ox) + inv_xy * (भिन्न) ((पूर्णाङ्क) py + oy) + inv_x0;
      भिन्न gy = inv_yx * (भिन्न) ((पूर्णाङ्क) clip.min_x + ox) + inv_yy * (भिन्न) ((पूर्णाङ्क) py + oy) + inv_y0;
      लागि (अचिन्हित px = clip.min_x; px < clip.max_x; px++)
      {
	uint8_t clip_alpha = clip_row[px];
	यदि (clip_alpha == 0)
	{
	  gx += inv_xx;
	  gy += inv_yx;
	  जारी;
	}

	/* Map glyph space to image texel; bilinear reconstruction. */
	भिन्न ix = (gx - img_x) / img_sx;
	भिन्न iy = (भिन्न) (src_height - 1) - (gy - img_y) / img_sy;

	यदि (unlikely (!std::है_सीमित (ix) || !std::है_सीमित (iy)) ||
	    ix < 0.f || iy < 0.f ||
	    ix > (भिन्न) (src_width - 1) || iy > (भिन्न) (src_height - 1))
	{
	  gx += inv_xx;
	  gy += inv_yx;
	  जारी;
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

  फर्काऊ सत्य;
}


/*
 * Gradient helpers
 */

#define GRADIENT_LUT_SIZE 256
#define GRADIENT_LUT_MIN_PIXELS (64u * 64u)

स्थिर HB_ALWAYS_INLINE भिन्न
reflect_gradient_t (भिन्न t)
{
  t = fmodf (fabsf (t), 2.f);
  फर्काऊ t > 1.f ? 2.f - t : t;
}

/* Evaluate color at normalized position t, interpolating in premultiplied space. */
स्थिर uint32_t
evaluate_color_पङ्क्ति (अचर hb_color_stop_t *stops, अचिन्हित len, भिन्न t,
		     hb_paint_extend_t extend)
{
  यदि (unlikely (!len))
    फर्काऊ 0;
  यदि (unlikely (len == 1))
    फर्काऊ color_to_premul_pixel (stops[0].color);
  यदि (unlikely (!std::है_सीमित (t)))
    t = 0.f;

  /* Apply extend mode */
  यदि (extend == HB_PAINT_EXTEND_PAD)
  {
    t = hb_clamp (t, 0.f, 1.f);
  }
  अन्यथा यदि (extend == HB_PAINT_EXTEND_REPEAT)
  {
    t = t - floorf (t);
  }
  अन्यथा /* REFLECT */
  {
    t = reflect_gradient_t (t);
  }

  /* Find bounding stops */
  यदि (t <= stops[0].offset)
    फर्काऊ color_to_premul_pixel (stops[0].color);
  यदि (t >= stops[len - 1].offset)
    फर्काऊ color_to_premul_pixel (stops[len - 1].color);

  अचिन्हित i;
  लागि (i = 0; i < len - 1; i++)
    यदि (t < stops[i + 1].offset)
      रोक;

  भिन्न range = stops[i + 1].offset - stops[i].offset;
  भिन्न k = range > 0.f ? (t - stops[i].offset) / range : 0.f;

  /* Interpolate in premultiplied [0,255] space */
  hb_color_t c0 = stops[i].color;
  hb_color_t c1 = stops[i + 1].color;

  भिन्न a0 = hb_color_get_alpha (c0) / 255.f;
  भिन्न r0 = hb_color_get_red   (c0) / 255.f * a0;
  भिन्न g0 = hb_color_get_green (c0) / 255.f * a0;
  भिन्न b0 = hb_color_get_blue  (c0) / 255.f * a0;

  भिन्न a1 = hb_color_get_alpha (c1) / 255.f;
  भिन्न r1 = hb_color_get_red   (c1) / 255.f * a1;
  भिन्न g1 = hb_color_get_green (c1) / 255.f * a1;
  भिन्न b1 = hb_color_get_blue  (c1) / 255.f * a1;

  भिन्न a = a0 + k * (a1 - a0);
  भिन्न r = r0 + k * (r1 - r0);
  भिन्न g = g0 + k * (g1 - g0);
  भिन्न b = b0 + k * (b1 - b0);

  uint8_t pa = (uint8_t) (a * 255.f + 0.5f);
  uint8_t pr = (uint8_t) (r * 255.f + 0.5f);
  uint8_t pg = (uint8_t) (g * 255.f + 0.5f);
  uint8_t pb = (uint8_t) (b * 255.f + 0.5f);

  फर्काऊ (uint32_t) pb | ((uint32_t) pg << 8) | ((uint32_t) pr << 16) | ((uint32_t) pa << 24);
}

स्थिर HB_ALWAYS_INLINE भिन्न
normalize_gradient_t (भिन्न t, hb_paint_extend_t extend)
{
  यदि (unlikely (!std::है_सीमित (t)))
    फर्काऊ 0.f;

  यदि (extend == HB_PAINT_EXTEND_PAD)
    फर्काऊ hb_clamp (t, 0.f, 1.f);
  यदि (extend == HB_PAINT_EXTEND_REPEAT)
  {
    t = t - floorf (t);
    फर्काऊ t < 0.f ? t + 1.f : t;
  }

  /* REFLECT */
  फर्काऊ reflect_gradient_t (t);
}

स्थिर शून्य
build_gradient_lut (अचर hb_color_stop_t *stops,
		    अचिन्हित len,
		    uint32_t *lut)
{
  लागि (अचिन्हित i = 0; i < GRADIENT_LUT_SIZE; i++)
  {
    भिन्न t = (भिन्न) i / (GRADIENT_LUT_SIZE - 1);
    lut[i] = evaluate_color_पङ्क्ति (stops, len, t, HB_PAINT_EXTEND_PAD);
  }
}

स्थिर HB_ALWAYS_INLINE uint32_t
lookup_gradient_lut (अचर uint32_t *lut,
		     भिन्न t,
		     hb_paint_extend_t extend)
{
  भिन्न u = normalize_gradient_t (t, extend);
  यदि (unlikely (!std::है_सीमित (u)))
    u = 0.f;
  अन्यथा
    u = hb_clamp (u, 0.f, 1.f);
  अचिन्हित idx = (अचिन्हित) (u * (GRADIENT_LUT_SIZE - 1) + 0.5f);
  फर्काऊ lut[idx];
}

/*
 * Gradient paint callbacks
 */

स्थिर शून्य
hb_raster_paint_linear_gradient (hb_paint_funcs_t *pfuncs HB_UNUSED,
				 शून्य *paint_data,
				 hb_color_पङ्क्ति_t *color_पङ्क्ति,
				 भिन्न x0, भिन्न y0,
				 भिन्न x1, भिन्न y1,
				 भिन्न x2, भिन्न y2,
				 शून्य *user_data HB_UNUSED)
{
  hb_raster_paint_t *c = (hb_raster_paint_t *) paint_data;

  ensure_initialized (c);

  hb_raster_image_t *surf = c->current_surface ();
  यदि (unlikely (!surf)) फर्काऊ;

  यदि (unlikely (!c->fetch_color_stops (color_पङ्क्ति)))
    फर्काऊ;
  hb_color_stop_t *stops = c->scratch_color_stops.arrayZ;
  अचिन्हित len = c->scratch_color_stops.length;
  भिन्न mn, mx;
  hb_paint_normalize_color_पङ्क्ति (stops, len, &mn, &mx);

  hb_paint_extend_t extend = hb_color_पङ्क्ति_get_extend (color_पङ्क्ति);
  अचर hb_raster_clip_t &clip = c->current_clip ();
  अचिन्हित clip_w = clip.max_x > clip.min_x ? clip.max_x - clip.min_x : 0;
  अचिन्हित clip_h = clip.max_y > clip.min_y ? clip.max_y - clip.min_y : 0;
  यदि (unlikely (!c->charge_work ((int64_t) clip_w * clip_h)))
    फर्काऊ;
  तार्किक use_lut = (uint64_t) clip_w * clip_h >= GRADIENT_LUT_MIN_PIXELS;
  uint32_t lut[GRADIENT_LUT_SIZE];
  यदि (use_lut)
    build_gradient_lut (stops, len, lut);

  /* Reduce 3-point anchor to 2-point gradient axis */
  भिन्न lx0, ly0, lx1, ly1;
  hb_paint_reduce_linear_anchors (x0, y0, x1, y1, x2, y2,
				  &lx0, &ly0, &lx1, &ly1);

  /* Apply normalization to endpoints */
  भिन्न gx0 = lx0 + mn * (lx1 - lx0);
  भिन्न gy0 = ly0 + mn * (ly1 - ly0);
  भिन्न gx1 = lx0 + mx * (lx1 - lx0);
  भिन्न gy1 = ly0 + mx * (ly1 - ly0);

  /* Inverse transform: pixel → glyph space */
  hb_transform_t<> t = c->current_effective_transform ();
  भिन्न det = t.xx * t.yy - t.xy * t.yx;
  यदि (fabsf (det) < 1e-10f) फर्काऊ;

  {
    भिन्न inv_det = 1.f / det;
    भिन्न inv_xx =  t.yy * inv_det;
    भिन्न inv_xy = -t.xy * inv_det;
    भिन्न inv_yx = -t.yx * inv_det;
    भिन्न inv_yy =  t.xx * inv_det;
    भिन्न inv_x0 = (t.xy * t.y0 - t.yy * t.x0) * inv_det;
    भिन्न inv_y0 = (t.yx * t.x0 - t.xx * t.y0) * inv_det;

    /* Gradient direction vector and denominator for projection */
    भिन्न dx = gx1 - gx0, dy = gy1 - gy0;
    भिन्न denom = dx * dx + dy * dy;
    यदि (denom < 1e-10f) फर्काऊ;
    भिन्न inv_denom = 1.f / denom;

    अचिन्हित stride = surf->extents.stride;
    पूर्णाङ्क ox = surf->extents.x_origin;
    पूर्णाङ्क oy = surf->extents.y_origin;

    यदि (clip.is_rect)
    {
      लागि (अचिन्हित py = clip.min_y; py < clip.max_y; py++)
      {
	hb_packed_t<uint32_t> *row = (hb_packed_t<uint32_t> *) (surf->buffer.arrayZ + py * stride);
	भिन्न gx = inv_xx * ((भिन्न) ((पूर्णाङ्क) clip.min_x + ox) + 0.5f) + inv_xy * ((भिन्न) ((पूर्णाङ्क) py + oy) + 0.5f) + inv_x0;
	भिन्न gy = inv_yx * ((भिन्न) ((पूर्णाङ्क) clip.min_x + ox) + 0.5f) + inv_yy * ((भिन्न) ((पूर्णाङ्क) py + oy) + 0.5f) + inv_y0;
	यदि (use_lut)
	{
	  लागि (अचिन्हित px = clip.min_x; px < clip.max_x; px++)
	  {
	    भिन्न proj_t = ((gx - gx0) * dx + (gy - gy0) * dy) * inv_denom;
	    uint32_t src = lookup_gradient_lut (lut, proj_t, extend);
	    row[px] = hb_packed_t<uint32_t> (hb_raster_src_over (src, (uint32_t) row[px]));
	    gx += inv_xx;
	    gy += inv_yx;
	  }
	}
	अन्यथा
	{
	  लागि (अचिन्हित px = clip.min_x; px < clip.max_x; px++)
	  {
	    भिन्न proj_t = ((gx - gx0) * dx + (gy - gy0) * dy) * inv_denom;
	    uint32_t src = evaluate_color_पङ्क्ति (stops, len, proj_t, extend);
	    row[px] = hb_packed_t<uint32_t> (hb_raster_src_over (src, (uint32_t) row[px]));
	    gx += inv_xx;
	    gy += inv_yx;
	  }
	}
      }
    }
    अन्यथा
    {
      लागि (अचिन्हित py = clip.min_y; py < clip.max_y; py++)
      {
	hb_packed_t<uint32_t> *row = (hb_packed_t<uint32_t> *) (surf->buffer.arrayZ + py * stride);
	अचर uint8_t *clip_row = clip.alpha.arrayZ + py * clip.stride;
	भिन्न gx = inv_xx * ((भिन्न) ((पूर्णाङ्क) clip.min_x + ox) + 0.5f) + inv_xy * ((भिन्न) ((पूर्णाङ्क) py + oy) + 0.5f) + inv_x0;
	भिन्न gy = inv_yx * ((भिन्न) ((पूर्णाङ्क) clip.min_x + ox) + 0.5f) + inv_yy * ((भिन्न) ((पूर्णाङ्क) py + oy) + 0.5f) + inv_y0;
	यदि (use_lut)
	{
	  लागि (अचिन्हित px = clip.min_x; px < clip.max_x; px++)
	  {
	    uint8_t clip_alpha = clip_row[px];
	    यदि (clip_alpha == 0)
	    {
	      gx += inv_xx;
	      gy += inv_yx;
	      जारी;
	    }
	    भिन्न proj_t = ((gx - gx0) * dx + (gy - gy0) * dy) * inv_denom;
	    uint32_t src = lookup_gradient_lut (lut, proj_t, extend);
	    src = hb_raster_alpha_mul (src, clip_alpha);
	    row[px] = hb_packed_t<uint32_t> (hb_raster_src_over (src, (uint32_t) row[px]));
	    gx += inv_xx;
	    gy += inv_yx;
	  }
	}
	अन्यथा
	{
	  लागि (अचिन्हित px = clip.min_x; px < clip.max_x; px++)
	  {
	    uint8_t clip_alpha = clip_row[px];
	    यदि (clip_alpha == 0)
	    {
	      gx += inv_xx;
	      gy += inv_yx;
	      जारी;
	    }
	    भिन्न proj_t = ((gx - gx0) * dx + (gy - gy0) * dy) * inv_denom;
	    uint32_t src = evaluate_color_पङ्क्ति (stops, len, proj_t, extend);
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

स्थिर शून्य
hb_raster_paint_radial_gradient (hb_paint_funcs_t *pfuncs HB_UNUSED,
				 शून्य *paint_data,
				 hb_color_पङ्क्ति_t *color_पङ्क्ति,
				 भिन्न x0, भिन्न y0, भिन्न r0,
				 भिन्न x1, भिन्न y1, भिन्न r1,
				 शून्य *user_data HB_UNUSED)
{
  hb_raster_paint_t *c = (hb_raster_paint_t *) paint_data;

  ensure_initialized (c);

  hb_raster_image_t *surf = c->current_surface ();
  यदि (unlikely (!surf)) फर्काऊ;

  यदि (unlikely (!c->fetch_color_stops (color_पङ्क्ति)))
    फर्काऊ;
  hb_color_stop_t *stops = c->scratch_color_stops.arrayZ;
  अचिन्हित len = c->scratch_color_stops.length;
  भिन्न mn, mx;
  hb_paint_normalize_color_पङ्क्ति (stops, len, &mn, &mx);

  hb_paint_extend_t extend = hb_color_पङ्क्ति_get_extend (color_पङ्क्ति);
  अचर hb_raster_clip_t &clip = c->current_clip ();
  अचिन्हित clip_w = clip.max_x > clip.min_x ? clip.max_x - clip.min_x : 0;
  अचिन्हित clip_h = clip.max_y > clip.min_y ? clip.max_y - clip.min_y : 0;
  यदि (unlikely (!c->charge_work ((int64_t) clip_w * clip_h)))
    फर्काऊ;
  तार्किक use_lut = (uint64_t) clip_w * clip_h >= GRADIENT_LUT_MIN_PIXELS;
  uint32_t lut[GRADIENT_LUT_SIZE];
  यदि (use_lut)
    build_gradient_lut (stops, len, lut);

  /* Apply normalization to circle parameters */
  भिन्न cx0 = x0 + mn * (x1 - x0);
  भिन्न cy0 = y0 + mn * (y1 - y0);
  भिन्न cr0 = r0 + mn * (r1 - r0);
  भिन्न cx1 = x0 + mx * (x1 - x0);
  भिन्न cy1 = y0 + mx * (y1 - y0);
  भिन्न cr1 = r0 + mx * (r1 - r0);

  /* Inverse transform */
  hb_transform_t<> t = c->current_effective_transform ();
  भिन्न det = t.xx * t.yy - t.xy * t.yx;
  यदि (fabsf (det) < 1e-10f) फर्काऊ;

  {
    भिन्न inv_det = 1.f / det;
    भिन्न inv_xx =  t.yy * inv_det;
    भिन्न inv_xy = -t.xy * inv_det;
    भिन्न inv_yx = -t.yx * inv_det;
    भिन्न inv_yy =  t.xx * inv_det;
    भिन्न inv_x0 = (t.xy * t.y0 - t.yy * t.x0) * inv_det;
    भिन्न inv_y0 = (t.yx * t.x0 - t.xx * t.y0) * inv_det;

    /* Precompute quadratic coefficients for radial gradient:
     * |p - c0 - t*(c1-c0)|^2 = (r0 + t*(r1-r0))^2
     *
     * Expanding gives At^2 + Bt + C = 0 where:
     *   cdx = c1.x - c0.x, cdy = c1.y - c0.y, dr = r1 - r0
     *   A = cdx^2 + cdy^2 - dr^2
     *   B = -2*(px-c0.x)*cdx - 2*(py-c0.y)*cdy - 2*r0*dr
     *   C = (px-c0.x)^2 + (py-c0.y)^2 - r0^2
     */
    भिन्न cdx = cx1 - cx0, cdy = cy1 - cy0;
    भिन्न dr = cr1 - cr0;
    भिन्न A = cdx * cdx + cdy * cdy - dr * dr;

    अचिन्हित stride = surf->extents.stride;
    पूर्णाङ्क ox = surf->extents.x_origin;
    पूर्णाङ्क oy = surf->extents.y_origin;

    यदि (clip.is_rect)
    {
      लागि (अचिन्हित py = clip.min_y; py < clip.max_y; py++)
      {
	hb_packed_t<uint32_t> *row = (hb_packed_t<uint32_t> *) (surf->buffer.arrayZ + py * stride);
	भिन्न gx = inv_xx * ((भिन्न) ((पूर्णाङ्क) clip.min_x + ox) + 0.5f) + inv_xy * ((भिन्न) ((पूर्णाङ्क) py + oy) + 0.5f) + inv_x0;
	भिन्न gy = inv_yx * ((भिन्न) ((पूर्णाङ्क) clip.min_x + ox) + 0.5f) + inv_yy * ((भिन्न) ((पूर्णाङ्क) py + oy) + 0.5f) + inv_y0;
	यदि (use_lut)
	{
	  लागि (अचिन्हित px = clip.min_x; px < clip.max_x; px++)
	  {
	    भिन्न dpx = gx - cx0, dpy = gy - cy0;
	    भिन्न B = -2.f * (dpx * cdx + dpy * cdy + cr0 * dr);
	    भिन्न C = dpx * dpx + dpy * dpy - cr0 * cr0;

	    भिन्न grad_t;
	    यदि (fabsf (A) > 1e-10f)
	    {
	      भिन्न disc = B * B - 4.f * A * C;
	      यदि (disc < 0.f)
	      {
		gx += inv_xx;
		gy += inv_yx;
		जारी;
	      }
	      भिन्न sq = sqrtf (disc);
	      /* Pick the larger root (t closer to 1 = outer circle) */
	      भिन्न t1 = (-B + sq) / (2.f * A);
	      भिन्न t2 = (-B - sq) / (2.f * A);
	      /* Choose the root that gives a positive radius */
	      यदि (cr0 + t1 * dr >= 0.f)
		grad_t = t1;
	      अन्यथा
		grad_t = t2;
	    }
	    अन्यथा
	    {
	      /* Linear case: Bt + C = 0 */
	      यदि (fabsf (B) < 1e-10f)
	      {
		gx += inv_xx;
		gy += inv_yx;
		जारी;
	      }
	      grad_t = -C / B;
	    }

	    uint32_t src = lookup_gradient_lut (lut, grad_t, extend);
	    row[px] = hb_packed_t<uint32_t> (hb_raster_src_over (src, (uint32_t) row[px]));
	    gx += inv_xx;
	    gy += inv_yx;
	  }
	}
	अन्यथा
	{
	  लागि (अचिन्हित px = clip.min_x; px < clip.max_x; px++)
	  {
	    भिन्न dpx = gx - cx0, dpy = gy - cy0;
	    भिन्न B = -2.f * (dpx * cdx + dpy * cdy + cr0 * dr);
	    भिन्न C = dpx * dpx + dpy * dpy - cr0 * cr0;

	    भिन्न grad_t;
	    यदि (fabsf (A) > 1e-10f)
	    {
	      भिन्न disc = B * B - 4.f * A * C;
	      यदि (disc < 0.f)
	      {
		gx += inv_xx;
		gy += inv_yx;
		जारी;
	      }
	      भिन्न sq = sqrtf (disc);
	      भिन्न t1 = (-B + sq) / (2.f * A);
	      भिन्न t2 = (-B - sq) / (2.f * A);
	      grad_t = (cr0 + t1 * dr >= 0.f) ? t1 : t2;
	    }
	    अन्यथा
	    {
	      यदि (fabsf (B) < 1e-10f)
	      {
		gx += inv_xx;
		gy += inv_yx;
		जारी;
	      }
	      grad_t = -C / B;
	    }

	    uint32_t src = evaluate_color_पङ्क्ति (stops, len, grad_t, extend);
	    row[px] = hb_packed_t<uint32_t> (hb_raster_src_over (src, (uint32_t) row[px]));
	    gx += inv_xx;
	    gy += inv_yx;
	  }
	}
      }
    }
    अन्यथा
    {
      लागि (अचिन्हित py = clip.min_y; py < clip.max_y; py++)
      {
	hb_packed_t<uint32_t> *row = (hb_packed_t<uint32_t> *) (surf->buffer.arrayZ + py * stride);
	अचर uint8_t *clip_row = clip.alpha.arrayZ + py * clip.stride;
	भिन्न gx = inv_xx * ((भिन्न) ((पूर्णाङ्क) clip.min_x + ox) + 0.5f) + inv_xy * ((भिन्न) ((पूर्णाङ्क) py + oy) + 0.5f) + inv_x0;
	भिन्न gy = inv_yx * ((भिन्न) ((पूर्णाङ्क) clip.min_x + ox) + 0.5f) + inv_yy * ((भिन्न) ((पूर्णाङ्क) py + oy) + 0.5f) + inv_y0;
	यदि (use_lut)
	{
	  लागि (अचिन्हित px = clip.min_x; px < clip.max_x; px++)
	  {
	    uint8_t clip_alpha = clip_row[px];
	    यदि (clip_alpha == 0)
	    {
	      gx += inv_xx;
	      gy += inv_yx;
	      जारी;
	    }
	    भिन्न dpx = gx - cx0, dpy = gy - cy0;
	    भिन्न B = -2.f * (dpx * cdx + dpy * cdy + cr0 * dr);
	    भिन्न C = dpx * dpx + dpy * dpy - cr0 * cr0;

	    भिन्न grad_t;
	    यदि (fabsf (A) > 1e-10f)
	    {
	      भिन्न disc = B * B - 4.f * A * C;
	      यदि (disc < 0.f)
	      {
		gx += inv_xx;
		gy += inv_yx;
		जारी;
	      }
	      भिन्न sq = sqrtf (disc);
	      भिन्न t1 = (-B + sq) / (2.f * A);
	      भिन्न t2 = (-B - sq) / (2.f * A);
	      grad_t = (cr0 + t1 * dr >= 0.f) ? t1 : t2;
	    }
	    अन्यथा
	    {
	      यदि (fabsf (B) < 1e-10f)
	      {
		gx += inv_xx;
		gy += inv_yx;
		जारी;
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
	अन्यथा
	{
	  लागि (अचिन्हित px = clip.min_x; px < clip.max_x; px++)
	  {
	    uint8_t clip_alpha = clip_row[px];
	    यदि (clip_alpha == 0)
	    {
	      gx += inv_xx;
	      gy += inv_yx;
	      जारी;
	    }
	    भिन्न dpx = gx - cx0, dpy = gy - cy0;
	    भिन्न B = -2.f * (dpx * cdx + dpy * cdy + cr0 * dr);
	    भिन्न C = dpx * dpx + dpy * dpy - cr0 * cr0;

	    भिन्न grad_t;
	    यदि (fabsf (A) > 1e-10f)
	    {
	      भिन्न disc = B * B - 4.f * A * C;
	      यदि (disc < 0.f)
	      {
		gx += inv_xx;
		gy += inv_yx;
		जारी;
	      }
	      भिन्न sq = sqrtf (disc);
	      भिन्न t1 = (-B + sq) / (2.f * A);
	      भिन्न t2 = (-B - sq) / (2.f * A);
	      grad_t = (cr0 + t1 * dr >= 0.f) ? t1 : t2;
	    }
	    अन्यथा
	    {
	      यदि (fabsf (B) < 1e-10f)
	      {
		gx += inv_xx;
		gy += inv_yx;
		जारी;
	      }
	      grad_t = -C / B;
	    }

	    uint32_t src = evaluate_color_पङ्क्ति (stops, len, grad_t, extend);
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

स्थिर शून्य
hb_raster_paint_sweep_gradient (hb_paint_funcs_t *pfuncs HB_UNUSED,
				शून्य *paint_data,
				hb_color_पङ्क्ति_t *color_पङ्क्ति,
				भिन्न cx, भिन्न cy,
				भिन्न start_angle,
				भिन्न end_angle,
				शून्य *user_data HB_UNUSED)
{
  hb_raster_paint_t *c = (hb_raster_paint_t *) paint_data;

  ensure_initialized (c);

  hb_raster_image_t *surf = c->current_surface ();
  यदि (unlikely (!surf)) फर्काऊ;

  यदि (unlikely (!c->fetch_color_stops (color_पङ्क्ति)))
    फर्काऊ;
  hb_color_stop_t *stops = c->scratch_color_stops.arrayZ;
  अचिन्हित len = c->scratch_color_stops.length;
  भिन्न mn, mx;
  hb_paint_normalize_color_पङ्क्ति (stops, len, &mn, &mx);

  hb_paint_extend_t extend = hb_color_पङ्क्ति_get_extend (color_पङ्क्ति);
  अचर hb_raster_clip_t &clip = c->current_clip ();
  अचिन्हित clip_w = clip.max_x > clip.min_x ? clip.max_x - clip.min_x : 0;
  अचिन्हित clip_h = clip.max_y > clip.min_y ? clip.max_y - clip.min_y : 0;
  यदि (unlikely (!c->charge_work ((int64_t) clip_w * clip_h)))
    फर्काऊ;
  तार्किक use_lut = (uint64_t) clip_w * clip_h >= GRADIENT_LUT_MIN_PIXELS;
  uint32_t lut[GRADIENT_LUT_SIZE];
  यदि (use_lut)
    build_gradient_lut (stops, len, lut);

  /* Apply normalization to angle range */
  भिन्न a0 = start_angle + mn * (end_angle - start_angle);
  भिन्न a1 = start_angle + mx * (end_angle - start_angle);
  भिन्न angle_range = a1 - a0;

  /* Inverse transform */
  hb_transform_t<> t = c->current_effective_transform ();
  भिन्न det = t.xx * t.yy - t.xy * t.yx;
  यदि (fabsf (det) < 1e-10f || fabsf (angle_range) < 1e-10f) फर्काऊ;

  {
    भिन्न inv_det = 1.f / det;
    भिन्न inv_xx =  t.yy * inv_det;
    भिन्न inv_xy = -t.xy * inv_det;
    भिन्न inv_yx = -t.yx * inv_det;
    भिन्न inv_yy =  t.xx * inv_det;
    भिन्न inv_x0 = (t.xy * t.y0 - t.yy * t.x0) * inv_det;
    भिन्न inv_y0 = (t.yx * t.x0 - t.xx * t.y0) * inv_det;

    भिन्न inv_angle_range = 1.f / angle_range;

    अचिन्हित stride = surf->extents.stride;
    पूर्णाङ्क ox = surf->extents.x_origin;
    पूर्णाङ्क oy = surf->extents.y_origin;

    यदि (clip.is_rect)
    {
      लागि (अचिन्हित py = clip.min_y; py < clip.max_y; py++)
      {
	hb_packed_t<uint32_t> *row = (hb_packed_t<uint32_t> *) (surf->buffer.arrayZ + py * stride);
	भिन्न gx = inv_xx * ((भिन्न) ((पूर्णाङ्क) clip.min_x + ox) + 0.5f) + inv_xy * ((भिन्न) ((पूर्णाङ्क) py + oy) + 0.5f) + inv_x0;
	भिन्न gy = inv_yx * ((भिन्न) ((पूर्णाङ्क) clip.min_x + ox) + 0.5f) + inv_yy * ((भिन्न) ((पूर्णाङ्क) py + oy) + 0.5f) + inv_y0;
	यदि (use_lut)
	{
	  लागि (अचिन्हित px = clip.min_x; px < clip.max_x; px++)
	  {
	    भिन्न angle = atan2f (gy - cy, gx - cx);
	    यदि (angle < 0) angle += (भिन्न) HB_2_PI;
	    भिन्न grad_t = (angle - a0) * inv_angle_range;
	    uint32_t src = lookup_gradient_lut (lut, grad_t, extend);
	    row[px] = hb_packed_t<uint32_t> (hb_raster_src_over (src, (uint32_t) row[px]));
	    gx += inv_xx;
	    gy += inv_yx;
	  }
	}
	अन्यथा
	{
	  लागि (अचिन्हित px = clip.min_x; px < clip.max_x; px++)
	  {
	    भिन्न angle = atan2f (gy - cy, gx - cx);
	    यदि (angle < 0) angle += (भिन्न) HB_2_PI;
	    भिन्न grad_t = (angle - a0) * inv_angle_range;
	    uint32_t src = evaluate_color_पङ्क्ति (stops, len, grad_t, extend);
	    row[px] = hb_packed_t<uint32_t> (hb_raster_src_over (src, (uint32_t) row[px]));
	    gx += inv_xx;
	    gy += inv_yx;
	  }
	}
      }
    }
    अन्यथा
    {
      लागि (अचिन्हित py = clip.min_y; py < clip.max_y; py++)
      {
	hb_packed_t<uint32_t> *row = (hb_packed_t<uint32_t> *) (surf->buffer.arrayZ + py * stride);
	अचर uint8_t *clip_row = clip.alpha.arrayZ + py * clip.stride;
	भिन्न gx = inv_xx * ((भिन्न) ((पूर्णाङ्क) clip.min_x + ox) + 0.5f) + inv_xy * ((भिन्न) ((पूर्णाङ्क) py + oy) + 0.5f) + inv_x0;
	भिन्न gy = inv_yx * ((भिन्न) ((पूर्णाङ्क) clip.min_x + ox) + 0.5f) + inv_yy * ((भिन्न) ((पूर्णाङ्क) py + oy) + 0.5f) + inv_y0;
	यदि (use_lut)
	{
	  लागि (अचिन्हित px = clip.min_x; px < clip.max_x; px++)
	  {
	    uint8_t clip_alpha = clip_row[px];
	    यदि (clip_alpha == 0)
	    {
	      gx += inv_xx;
	      gy += inv_yx;
	      जारी;
	    }
	    भिन्न angle = atan2f (gy - cy, gx - cx);
	    यदि (angle < 0) angle += (भिन्न) HB_2_PI;
	    भिन्न grad_t = (angle - a0) * inv_angle_range;
	    uint32_t src = lookup_gradient_lut (lut, grad_t, extend);
	    src = hb_raster_alpha_mul (src, clip_alpha);
	    row[px] = hb_packed_t<uint32_t> (hb_raster_src_over (src, (uint32_t) row[px]));
	    gx += inv_xx;
	    gy += inv_yx;
	  }
	}
	अन्यथा
	{
	  लागि (अचिन्हित px = clip.min_x; px < clip.max_x; px++)
	  {
	    uint8_t clip_alpha = clip_row[px];
	    यदि (clip_alpha == 0)
	    {
	      gx += inv_xx;
	      gy += inv_yx;
	      जारी;
	    }
	    भिन्न angle = atan2f (gy - cy, gx - cx);
	    यदि (angle < 0) angle += (भिन्न) HB_2_PI;
	    भिन्न grad_t = (angle - a0) * inv_angle_range;
	    uint32_t src = evaluate_color_पङ्क्ति (stops, len, grad_t, extend);
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

स्थिर hb_bool_t
hb_raster_paint_custom_palette_color (hb_paint_funcs_t *funcs HB_UNUSED,
				      शून्य *paint_data,
				      अचिन्हित पूर्णाङ्क color_index,
				      hb_color_t *color,
				      शून्य *user_data HB_UNUSED)
{
  hb_raster_paint_t *c = (hb_raster_paint_t *) paint_data;
  यदि (likely (c->custom_palette && hb_map_has (c->custom_palette, color_index)))
  {
    *color = hb_map_get (c->custom_palette, color_index);
    फर्काऊ सत्य;
  }
  फर्काऊ असत्य;
}

स्थिर hb_bool_t
hb_raster_paint_set_budget (hb_paint_funcs_t *, शून्य *paint_data,
			    int64_t budget, शून्य *)
{
  स्वतः *paint = (hb_raster_paint_t *) paint_data;
  paint->budget = budget;
  paint->recharge_budget ();
  फर्काऊ सत्य;
}

स्थिर int64_t
hb_raster_paint_get_budget (hb_paint_funcs_t *, शून्य *paint_data, शून्य *)
{
  फर्काऊ ((hb_raster_paint_t *) paint_data)->budget;
}

स्थिर int64_t *
hb_raster_paint_get_budget_remaining (hb_paint_funcs_t *, शून्य *paint_data, शून्य *)
{
  फर्काऊ &((hb_raster_paint_t *) paint_data)->budget_remaining;
}


/*
 * Lazy-loader singleton for paint funcs
 */

स्थिर अन्तःस्थ शून्य free_static_raster_paint_funcs ();

स्थिर संरचना hb_raster_paint_funcs_lazy_loader_t : hb_paint_funcs_lazy_loader_t<hb_raster_paint_funcs_lazy_loader_t>
{
  स्थिर hb_paint_funcs_t *create ()
  {
    hb_paint_funcs_t *funcs = hb_paint_funcs_create ();

    hb_paint_funcs_set_थप_transform_func (funcs, hb_raster_paint_थप_transform, nullptr, nullptr);
    hb_paint_funcs_set_झिक_transform_func (funcs, hb_raster_paint_झिक_transform, nullptr, nullptr);
    hb_paint_funcs_set_fill_glyph_func (funcs, hb_raster_paint_fill_glyph, nullptr, nullptr);
    hb_paint_funcs_set_color_glyph_func (funcs, hb_raster_paint_color_glyph, nullptr, nullptr);
    hb_paint_funcs_set_थप_clip_glyph_func (funcs, hb_raster_paint_थप_clip_glyph, nullptr, nullptr);
    hb_paint_funcs_set_थप_clip_rectangle_func (funcs, hb_raster_paint_थप_clip_rectangle, nullptr, nullptr);
    hb_paint_funcs_set_थप_clip_path_start_func (funcs, hb_raster_paint_थप_clip_path_start, nullptr, nullptr);
    hb_paint_funcs_set_थप_clip_path_end_func (funcs, hb_raster_paint_थप_clip_path_end, nullptr, nullptr);
    hb_paint_funcs_set_झिक_clip_func (funcs, hb_raster_paint_झिक_clip, nullptr, nullptr);
    hb_paint_funcs_set_थप_group_func (funcs, hb_raster_paint_थप_group, nullptr, nullptr);
    hb_paint_funcs_set_झिक_group_func (funcs, hb_raster_paint_झिक_group, nullptr, nullptr);
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

    फर्काऊ funcs;
  }
} static_raster_paint_funcs;

स्थिर अन्तःस्थ शून्य
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
hb_raster_paint_create_or_fail (शून्य)
{
  hb_raster_paint_t *paint = hb_object_create<hb_raster_paint_t> ();
  यदि (unlikely (!paint))
    फर्काऊ nullptr;

  paint->clip_rdr = hb_raster_draw_create_or_fail ();
  यदि (unlikely (!paint->clip_rdr))
  {
    hb_free (paint);
    फर्काऊ nullptr;
  }

  फर्काऊ paint;
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
  फर्काऊ hb_object_reference (paint);
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
शून्य
hb_raster_paint_destroy (hb_raster_paint_t *paint)
{
  यदि (!hb_object_should_destroy (paint))
    फर्काऊ;

  hb_map_destroy (paint->custom_palette);
  hb_raster_draw_destroy (paint->clip_rdr);
  लागि (स्वतः *s : paint->surface_थाक)
    hb_raster_image_destroy (s);
  लागि (स्वतः *s : paint->surface_cache)
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
			       hb_user_data_साँचो_t *key,
			       शून्य               *data,
			       hb_destroy_func_t   destroy,
			       hb_bool_t           replace)
{
  फर्काऊ hb_object_set_user_data (paint, key, data, destroy, replace);
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
शून्य *
hb_raster_paint_get_user_data (अचर hb_raster_paint_t  *paint,
			       hb_user_data_साँचो_t *key)
{
  फर्काऊ hb_object_get_user_data (paint, key);
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
शून्य
hb_raster_paint_set_transform (hb_raster_paint_t *paint,
			       भिन्न xx, भिन्न yx,
			       भिन्न xy, भिन्न yy,
			       भिन्न dx, भिन्न dy)
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
शून्य
hb_raster_paint_get_transform (अचर hb_raster_paint_t *paint,
			       भिन्न *xx, भिन्न *yx,
			       भिन्न *xy, भिन्न *yy,
			       भिन्न *dx, भिन्न *dy)
{
  यदि (xx) *xx = paint->base_transform.xx;
  यदि (yx) *yx = paint->base_transform.yx;
  यदि (xy) *xy = paint->base_transform.xy;
  यदि (yy) *yy = paint->base_transform.yy;
  यदि (dx) *dx = paint->base_transform.x0;
  यदि (dy) *dy = paint->base_transform.y0;
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
शून्य
hb_raster_paint_set_scale_factor (hb_raster_paint_t *paint,
				  भिन्न x_scale_factor,
				  भिन्न y_scale_factor)
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
शून्य
hb_raster_paint_get_scale_factor (अचर hb_raster_paint_t *paint,
				  भिन्न *x_scale_factor,
				  भिन्न *y_scale_factor)
{
  यदि (x_scale_factor) *x_scale_factor = paint->x_scale_factor;
  यदि (y_scale_factor) *y_scale_factor = paint->y_scale_factor;
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
शून्य
hb_raster_paint_set_extents (hb_raster_paint_t         *paint,
			     अचर hb_raster_extents_t *extents)
{
  paint->fixed_extents = *extents;
  paint->has_extents = सत्य;
  यदि (paint->fixed_extents.stride == 0)
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
hb_raster_paint_get_extents (अचर hb_raster_paint_t   *paint,
			     hb_raster_extents_t *extents)
{
  यदि (!paint->has_extents)
    फर्काऊ असत्य;

  यदि (extents)
    *extents = paint->fixed_extents;
  फर्काऊ सत्य;
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
				   अचर hb_glyph_extents_t *glyph_extents)
{
  भिन्न x0 = (भिन्न) glyph_extents->x_bearing;
  भिन्न y0 = (भिन्न) glyph_extents->y_bearing;
  भिन्न x1 = (भिन्न) glyph_extents->x_bearing + glyph_extents->width;
  भिन्न y1 = (भिन्न) glyph_extents->y_bearing + glyph_extents->height;

  भिन्न xmin = hb_min (x0, x1);
  भिन्न xmax = hb_max (x0, x1);
  भिन्न ymin = hb_min (y0, y1);
  भिन्न ymax = hb_max (y0, y1);

  भिन्न px[4] = {xmin, xmin, xmax, xmax};
  भिन्न py[4] = {ymin, ymax, ymin, ymax};

  hb_transform_t<> t = paint->base_transform;
  paint->apply_scale_factor (t);

  भिन्न tx, ty;
  t.transform_point (px[0], py[0]);
  tx = px[0]; ty = py[0];
  भिन्न tx_min = tx, tx_max = tx;
  भिन्न ty_min = ty, ty_max = ty;

  लागि (अचिन्हित i = 1; i < 4; i++)
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

  यदि (ex1 <= ex0 || ey1 <= ey0)
  {
    paint->fixed_extents = {};
    paint->has_extents = असत्य;
    फर्काऊ असत्य;
  }

  paint->fixed_extents = {
    ex0, ey0,
    (अचिन्हित) hb_min ((int64_t) ex1 - ex0, (int64_t) HB_RASTER_MAX_AUTO_DIMENSION),
    (अचिन्हित) hb_min ((int64_t) ey1 - ey0, (int64_t) HB_RASTER_MAX_AUTO_DIMENSION),
    0
  };
  paint->has_extents = सत्य;
  यदि (paint->fixed_extents.stride == 0)
    paint->fixed_extents.stride = paint->fixed_extents.width * 4;
  फर्काऊ सत्य;
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
शून्य
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
hb_raster_paint_get_foreground (अचर hb_raster_paint_t *paint)
{
  फर्काऊ paint->foreground;
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
शून्य
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
hb_raster_paint_get_background (अचर hb_raster_paint_t *paint)
{
  फर्काऊ paint->background;
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
शून्य
hb_raster_paint_set_palette (hb_raster_paint_t *paint,
			     अचिन्हित           palette)
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
अचिन्हित
hb_raster_paint_get_palette (अचर hb_raster_paint_t *paint)
{
  फर्काऊ paint->palette;
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
शून्य
hb_raster_paint_clear_custom_palette_colors (hb_raster_paint_t *paint)
{
  यदि (paint->custom_palette)
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
					  अचिन्हित पूर्णाङ्क       color_index,
					  hb_color_t         color)
{
  यदि (unlikely (!paint->custom_palette))
  {
    paint->custom_palette = hb_map_create ();
    यदि (unlikely (!paint->custom_palette))
      फर्काऊ असत्य;
  }
  hb_map_set (paint->custom_palette, color_index, color);
  फर्काऊ hb_map_allocation_successful (paint->custom_palette);
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
hb_raster_paint_get_funcs (अचर hb_raster_paint_t *paint HB_UNUSED)
{
  फर्काऊ static_raster_paint_funcs.get_unconst ();
}

स्थिर hb_bool_t
hb_raster_paint_glyph_impl (hb_raster_paint_t *paint,
			    hb_font_t        *font,
			    hb_codepoint_t    glyph,
			    hb_bool_t         fallible)
{
  यदि (!paint->has_extents)
  {
    hb_glyph_extents_t ge;
    यदि (hb_font_get_glyph_extents (font, glyph, &ge))
      hb_raster_paint_set_glyph_extents (paint, &ge);
  }

  hb_paint_funcs_t *funcs = hb_raster_paint_get_funcs (paint);
  hb_paint_थप_transform (funcs, paint,
			   paint->base_transform.xx, paint->base_transform.yx,
			   paint->base_transform.xy, paint->base_transform.yy,
			   paint->base_transform.x0, paint->base_transform.y0);

  hb_bool_t ret = सत्य;
  यदि (fallible)
    ret = hb_font_paint_glyph_or_fail (font, glyph,
				       funcs, paint,
				       paint->palette, paint->foreground);
  अन्यथा
    hb_font_paint_glyph (font, glyph,
			 funcs, paint,
			 paint->palette, paint->foreground);

  hb_paint_झिक_transform (funcs, paint);
  फर्काऊ ret;
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
  फर्काऊ hb_raster_paint_glyph_impl (paint, font, glyph, सत्य);
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
शून्य
hb_raster_paint_glyph (hb_raster_paint_t *paint,
		       hb_font_t        *font,
		       hb_codepoint_t    glyph)
{
  hb_raster_paint_glyph_impl (paint, font, glyph, असत्य);
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

  यदि (unlikely (!paint->has_extents))
    फर्काऊ nullptr;

  hb_raster_image_t *result;
  यदि (paint->surface_थाक.length)
  {
    result = paint->surface_थाक[0];
    /* Release any remaining group surfaces (shouldn't happen with
     * well-formed paint calls, but be safe). */
    लागि (अचिन्हित i = 1; i < paint->surface_थाक.length; i++)
      paint->release_surface (paint->surface_थाक[i]);
    paint->surface_थाक.clear ();
  }
  अन्यथा
  {
    result = paint->acquire_surface ();
    यदि (unlikely (!result))
      फर्काऊ nullptr;
  }

  फर्काऊ result;
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
शून्य
hb_raster_paint_clear (hb_raster_paint_t *paint)
{
  paint->fixed_extents = {};
  paint->has_extents = असत्य;
  paint->transform_थाक.clear ();
  paint->release_all_clips ();
  लागि (स्वतः *s : paint->surface_थाक)
    paint->release_surface (s);
  paint->surface_थाक.clear ();
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
शून्य
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
शून्य
hb_raster_paint_recycle_image (hb_raster_paint_t  *paint,
			       hb_raster_image_t  *image)
{
  paint->release_surface (image);
}
