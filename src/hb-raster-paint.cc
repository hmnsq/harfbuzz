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

#ضمن "hb.hh"

#ضمن "hb-raster-paint.hh"
#ضمن "hb-machinery.hh"
#ضمن "hb-paint.hh"

#ضمن <math.h>


/*
 * Pixel helpers (paint-specific)
 */

/* Convert unpremultiplied hb_color_t (BGRA order) to premultiplied BGRA32 pixel. */
ساكن مضمن uint32_t
color_منزل_premul_pixel (hb_color_t color)
{
  uint8_t a = hb_color_get_alpha (color);
  uint8_t r = hb_raster_div255 (hb_color_get_red (color) * a);
  uint8_t g = hb_raster_div255 (hb_color_get_green (color) * a);
  uint8_t b = hb_raster_div255 (hb_color_get_blue (color) * a);
  أرجع (uint32_t) b | ((uint32_t) g << 8) | ((uint32_t) r << 16) | ((uint32_t) a << 24);
}

/* Bilinear sample from a premultiplied BGRA32 image. */
ساكن مضمن uint32_t
hb_raster_sample_bilinear_premul (ثابت hb_packed_t<uint32_t> *src,
				  بنانشانی چوڑائی,
				  بنانشانی اچائی,
				  عائم x,
				  عائم y)
{
  صحيح x0 = (صحيح) floorf (x);
  صحيح y0 = (صحيح) floorf (y);
  صحيح x1 = x0 + 1;
  صحيح y1 = y0 + 1;
  إذا (x1 >= (صحيح) چوڑائی) x1 = (صحيح) چوڑائی - 1;
  إذا (y1 >= (صحيح) اچائی) y1 = (صحيح) اچائی - 1;

  عائم tx = x - x0;
  عائم ty = y - y0;
  عائم omtx = 1.f - tx;
  عائم omty = 1.f - ty;

  uint32_t p00 = (uint32_t) src[(size_t) y0 * چوڑائی + (size_t) x0];
  uint32_t p10 = (uint32_t) src[(size_t) y0 * چوڑائی + (size_t) x1];
  uint32_t p01 = (uint32_t) src[(size_t) y1 * چوڑائی + (size_t) x0];
  uint32_t p11 = (uint32_t) src[(size_t) y1 * چوڑائی + (size_t) x1];

  عائم w00 = omtx * omty;
  عائم w10 = tx * omty;
  عائم w01 = omtx * ty;
  عائم w11 = tx * ty;

  عائم b = ((p00 >>  0) & 0xff) * w00 + ((p10 >>  0) & 0xff) * w10 +
	    ((p01 >>  0) & 0xff) * w01 + ((p11 >>  0) & 0xff) * w11;
  عائم g = ((p00 >>  8) & 0xff) * w00 + ((p10 >>  8) & 0xff) * w10 +
	    ((p01 >>  8) & 0xff) * w01 + ((p11 >>  8) & 0xff) * w11;
  عائم r = ((p00 >> 16) & 0xff) * w00 + ((p10 >> 16) & 0xff) * w10 +
	    ((p01 >> 16) & 0xff) * w01 + ((p11 >> 16) & 0xff) * w11;
  عائم a = ((p00 >> 24) & 0xff) * w00 + ((p10 >> 24) & 0xff) * w10 +
	    ((p01 >> 24) & 0xff) * w01 + ((p11 >> 24) & 0xff) * w11;

  أرجع (uint32_t) (b + 0.5f)
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
ساكن فراغ
ensure_initialized (hb_raster_paint_t *c)
{
  إذا (c->surface_ڈھیر.لمائی) أرجع;

  /* A failed stack push (OOM) leaves the vector in a sticky error
   * state, so initialization can never succeed; bail out cheaply
   * instead of re-paying the surface acquire-and-clear below on
   * every subsequent paint callback. */
  إذا (unlikely (c->surface_ڈھیر.وچ_error () || c->clip_ڈھیر.وچ_error ()))
    أرجع;

  /* acquire_surface() clears a full surface; charge its area, so
   * repeated failed initialization attempts cannot re-pay it
   * indefinitely.  Successful initialization re-arms the session
   * budget below, making the charge a no-op for normal sessions. */
  إذا (unlikely (!c->charge_work ((int64_t) c->fixed_extents.چوڑائی *
				 c->fixed_extents.اچائی)))
    أرجع;

  /* Root surface */
  hb_raster_image_t *جڑ = c->acquire_surface ();
  إذا (unlikely (!جڑ)) أرجع;

  إذا (hb_color_get_alpha (c->background))
  {
    uint32_t bg = color_منزل_premul_pixel (c->background);
    hb_raster_extents_t ext;
    hb_raster_image_get_extents (جڑ, &ext);
    ثابت uint8_t *buf = hb_raster_image_get_بفر (جڑ);
    لكل (بنانشانی y = 0; y < ext.اچائی; y++)
    {
      hb_packed_t<uint32_t> *صف =
	(hb_packed_t<uint32_t> *) (buf + (size_t) y * ext.stride);
      لكل (بنانشانی x = 0; x < ext.چوڑائی; x++)
	صف[x] = hb_packed_t<uint32_t> (bg);
    }
  }

  إذا (unlikely (!c->surface_ڈھیر.ڈالو_or_fail (جڑ)))
  {
    c->release_surface (جڑ);
    أرجع;
  }

  /* Resolve the surface-sized pixel budget now that the surface size is
   * known.  An unlimited policy disables the pixel budget, so leave it.
   * The outline budget is re-resolved to the same value it already holds
   * (nothing has been drawn yet). */
  إذا (c->budget != HB_BUDGET_UNLIMITED)
    c->recharge_budget ();

  /* Initial clip: full coverage rectangle */
  hb_raster_clip_t clip;
  clip.init_بھریا (c->fixed_extents.چوڑائی, c->fixed_extents.اچائی);
  إذا (unlikely (!c->clip_ڈھیر.ڈالو_or_fail (std::move (clip))))
  {
    c->release_surface (c->surface_ڈھیر.نکالو ());
    أرجع;
  }
}

ساكن فراغ
hb_raster_paint_ڈالو_transform (hb_paint_funcs_t *pfuncs HB_بن_ورتیا,
				فراغ *paint_انکڑے,
				عائم xx, عائم yx,
				عائم xy, عائم yy,
				عائم dx, عائم dy,
				فراغ *user_انکڑے HB_بن_ورتیا)
{
  hb_raster_paint_t *c = (hb_raster_paint_t *) paint_انکڑے;

  ensure_initialized (c);

  hb_transform_t<> t = c->current_transform ();
  t.multiply ({xx, yx, xy, yy, dx, dy});
  (فراغ) c->transform_ڈھیر.push (t);
}

ساكن فراغ
hb_raster_paint_نکالو_transform (hb_paint_funcs_t *pfuncs HB_بن_ورتیا,
			       فراغ *paint_انکڑے,
			       فراغ *user_انکڑے HB_بن_ورتیا)
{
  hb_raster_paint_t *c = (hb_raster_paint_t *) paint_انکڑے;
  إذا (c->transform_ڈھیر.لمائی)
    c->transform_ڈھیر.نکالو ();
}

ساكن hb_bool_t
hb_raster_paint_color_glyph (hb_paint_funcs_t *pfuncs HB_بن_ورتیا,
			     فراغ *paint_انکڑے HB_بن_ورتیا,
			     hb_codepoint_t glyph HB_بن_ورتیا,
			     hb_font_t *font HB_بن_ورتیا,
			     فراغ *user_انکڑے HB_بن_ورتیا)
{
  أرجع خطأ;
}

تعريفنوع فراغ (*hb_raster_paint_clip_نقاب_emit_t) (hb_raster_draw_t *rdr, فراغ *user_انکڑے);

/* Attach the surface box to @rdr and seed its outline budget before
 * outlines are drawn into it: curves outside the surface collapse to
 * their chord, and outline traversal and curve flattening charge the
 * seeded budget.  hb_raster_paint_readback_budget() pulls the remainder
 * back into the session afterwards. */
ساكن فراغ
hb_raster_paint_attach_clip_rdr (hb_raster_paint_t *c,
				 hb_raster_draw_t *rdr,
				 ثابت hb_raster_image_t *surf)
{
  hb_raster_draw_سیٹ_clip_box (rdr,
			       (عائم) surf->extents.x_origin,
			       (عائم) surf->extents.y_origin,
			       (عائم) surf->extents.x_origin + (عائم) surf->extents.چوڑائی,
			       (عائم) surf->extents.y_origin + (عائم) surf->extents.اچائی);
  hb_draw_سیٹ_budget (hb_raster_draw_get_funcs (rdr), rdr, c->budget_بچیا);
}

/* Pull the outline budget the rasterizer has consumed back into the
 * session, so it aggregates across every glyph in the paint walk. */
ساكن فراغ
hb_raster_paint_readback_budget (hb_raster_paint_t *c,
				 hb_raster_draw_t *rdr)
{
  c->budget_بچیا = hb_draw_get_budget_بچیا (hb_raster_draw_get_funcs (rdr), rdr);
}

ساكن فراغ
hb_raster_paint_ڈالو_خالی_تھاواں_clip (hb_raster_paint_t *c, بنانشانی w, بنانشانی h)
{
  hb_raster_clip_t new_clip = c->acquire_clip (w, h);
  new_clip.init_بھریا (w, h);
  new_clip.is_rect = سچ;
  new_clip.rect_x0 = new_clip.rect_y0 = 0;
  new_clip.rect_x1 = new_clip.rect_y1 = 0;
  new_clip.min_x = new_clip.min_y = new_clip.max_x = new_clip.max_y = 0;
  (فراغ) c->clip_ڈھیر.push (std::move (new_clip));
}

ساكن منطقي
hb_raster_paint_intersect_نقاب (ثابت hb_raster_image_t *surf,
				ثابت hb_raster_extents_t &نقاب_ext,
				ثابت hb_raster_clip_t &clip,
				بنانشانی w, بنانشانی h,
				int64_t *نقاب_x0, int64_t *نقاب_y0,
				بنانشانی *ix0, بنانشانی *iy0,
				بنانشانی *ix1, بنانشانی *iy1)
{
  /* Convert mask extents from surface coordinates to clip-buffer coordinates. */
  int64_t mx0 = (int64_t) نقاب_ext.x_origin - (int64_t) surf->extents.x_origin;
  int64_t my0 = (int64_t) نقاب_ext.y_origin - (int64_t) surf->extents.y_origin;

  int64_t x0 = hb_max ((int64_t) clip.min_x, hb_max (mx0, (int64_t) 0));
  int64_t y0 = hb_max ((int64_t) clip.min_y, hb_max (my0, (int64_t) 0));
  int64_t x1 = hb_min ((int64_t) clip.max_x, hb_min (mx0 + نقاب_ext.چوڑائی, (int64_t) w));
  int64_t y1 = hb_min ((int64_t) clip.max_y, hb_min (my0 + نقاب_ext.اچائی, (int64_t) h));

  إذا (x0 >= x1 || y0 >= y1) أرجع خطأ;

  *نقاب_x0 = mx0;
  *نقاب_y0 = my0;
  *ix0 = (بنانشانی) x0;
  *iy0 = (بنانشانی) y0;
  *ix1 = (بنانشانی) x1;
  *iy1 = (بنانشانی) y1;
  أرجع سچ;
}

/* Render whatever edges have been accumulated into @rdr and
 * push the result as a new clip on the stack, intersecting
 * with the existing clip.  Used by the emitter-based clip
 * helpers (push_clip_glyph) and by push_clip_path_end once
 * the caller has drawn the path into @rdr. */
ساكن فراغ
hb_raster_paint_finalize_رستہ_clip (hb_raster_paint_t *c,
				    hb_raster_draw_t *rdr,
				    hb_raster_image_t *surf,
				    بنانشانی w, بنانشانی h)
{
  إذا (unlikely (!c->precharge_work (hb_raster_draw_get_pixel_work (rdr, h, w))))
  {
    hb_raster_draw_clear (rdr);
    hb_raster_paint_ڈالو_خالی_تھاواں_clip (c, w, h);
    أرجع;
  }

  hb_raster_image_t *نقاب_img = hb_raster_draw_render (rdr);

  إذا (unlikely (!نقاب_img))
  {
    hb_raster_paint_ڈالو_خالی_تھاواں_clip (c, w, h);
    أرجع;
  }

  /* Charge the mask render (its own area; may exceed the surface) plus
   * the clip-buffer pass below. */
  hb_raster_extents_t نقاب_render_ext;
  hb_raster_image_get_extents (نقاب_img, &نقاب_render_ext);
  إذا (unlikely (!c->charge_work ((int64_t) نقاب_render_ext.چوڑائی * نقاب_render_ext.اچائی +
				 (int64_t) w * h)))
  {
    hb_raster_draw_recycle_image (rdr, نقاب_img);
    hb_raster_paint_ڈالو_خالی_تھاواں_clip (c, w, h);
    أرجع;
  }

  hb_raster_clip_t new_clip = c->acquire_clip (w, h);

  /* Allocate alpha buffer and intersect with previous clip */
  size_t clip_ناپ = (size_t) new_clip.stride * h;
  إذا (unlikely (clip_ناپ > HB_RASTER_MAX_بفر_ناپ ||
                !new_clip.alpha.resize ((بنانشانی) clip_ناپ)))
  {
    hb_raster_draw_recycle_image (rdr, نقاب_img);
    hb_raster_paint_ڈالو_خالی_تھاواں_clip (c, w, h);
    أرجع;
  }
  hb_memset (new_clip.alpha.arrayZ, 0, (بنانشانی) clip_ناپ);

  ثابت uint8_t *نقاب_buf = hb_raster_image_get_بفر (نقاب_img);
  hb_raster_extents_t نقاب_ext;
  hb_raster_image_get_extents (نقاب_img, &نقاب_ext);
  ثابت hb_raster_clip_t &old_clip = c->current_clip ();
  إذا (unlikely (!old_clip.has_valid_alpha_نقاب ()))
  {
    hb_raster_draw_recycle_image (rdr, نقاب_img);
    hb_raster_paint_ڈالو_خالی_تھاواں_clip (c, w, h);
    أرجع;
  }

  int64_t نقاب_x0, نقاب_y0;
  بنانشانی ix0, iy0, ix1, iy1;
  إذا (!hb_raster_paint_intersect_نقاب (surf, نقاب_ext, old_clip, w, h,
				       &نقاب_x0, &نقاب_y0,
				       &ix0, &iy0, &ix1, &iy1))
  {
    hb_raster_draw_recycle_image (rdr, نقاب_img);
    hb_raster_paint_ڈالو_خالی_تھاواں_clip (c, w, h);
    أرجع;
  }

  new_clip.min_x = w; new_clip.min_y = h;
  new_clip.max_x = 0; new_clip.max_y = 0;

  إذا (old_clip.is_rect)
  {
    لكل (بنانشانی y = iy0; y < iy1; y++)
    {
      ثابت uint8_t *نقاب_صف = نقاب_buf + (بنانشانی) ((int64_t) y - نقاب_y0) * نقاب_ext.stride;
      uint8_t *باہرلا_صف = new_clip.alpha.arrayZ + y * new_clip.stride;
      بنانشانی صف_min = ix1;
      بنانشانی صف_max = ix0;
      بنانشانی mx = (بنانشانی) ((int64_t) ix0 - نقاب_x0);
      لكل (بنانشانی x = ix0; x < ix1; x++)
      {
	uint8_t a = نقاب_صف[mx++];
	باہرلا_صف[x] = a;
	إذا (a && صف_min == ix1)
	{
	  صف_min = x;
	  صف_max = x + 1;
	}
	وإلا إذا (a)
	  صف_max = x + 1;
      }
      إذا (صف_min < صف_max)
      {
	new_clip.min_x = hb_min (new_clip.min_x, صف_min);
	new_clip.min_y = hb_min (new_clip.min_y, y);
	new_clip.max_x = hb_max (new_clip.max_x, صف_max);
	new_clip.max_y = hb_max (new_clip.max_y, y + 1);
      }
    }
  }
  وإلا
  {
    لكل (بنانشانی y = iy0; y < iy1; y++)
    {
      ثابت uint8_t *old_صف = old_clip.alpha.arrayZ + y * old_clip.stride;
      ثابت uint8_t *نقاب_صف = نقاب_buf + (بنانشانی) ((int64_t) y - نقاب_y0) * نقاب_ext.stride;
      uint8_t *باہرلا_صف = new_clip.alpha.arrayZ + y * new_clip.stride;
      بنانشانی صف_min = ix1;
      بنانشانی صف_max = ix0;
      لكل (بنانشانی x = ix0; x < ix1; x++)
      {
	بنانشانی mx = (بنانشانی) ((int64_t) x - نقاب_x0);
	uint8_t a = hb_raster_div255 (نقاب_صف[mx] * old_صف[x]);
	باہرلا_صف[x] = a;
	إذا (a)
	{
	  صف_min = hb_min (صف_min, x);
	  صف_max = x + 1;
	}
      }
      إذا (صف_min < صف_max)
      {
	new_clip.min_x = hb_min (new_clip.min_x, صف_min);
	new_clip.min_y = hb_min (new_clip.min_y, y);
	new_clip.max_x = hb_max (new_clip.max_x, صف_max);
	new_clip.max_y = hb_max (new_clip.max_y, y + 1);
      }
    }
  }

  hb_raster_draw_recycle_image (rdr, نقاب_img);
  إذا (unlikely (!c->clip_ڈھیر.ڈالو_or_fail (std::move (new_clip))))
    hb_raster_paint_ڈالو_خالی_تھاواں_clip (c, w, h);
}

ساكن فراغ
hb_raster_paint_ڈالو_clip_from_emitter (hb_raster_paint_t *c,
					hb_raster_paint_clip_نقاب_emit_t emit,
					فراغ *emit_انکڑے)
{
  ensure_initialized (c);

  hb_raster_image_t *surf = c->current_surface ();
  إذا (unlikely (!surf)) أرجع;

  بنانشانی w = surf->extents.چوڑائی;
  بنانشانی h = surf->extents.اچائی;

  /* Out of budget: skip the glyph-outline extraction entirely, so
   * per-glyph outline limits cannot multiply with the caller's
   * paint-graph traversal limits. */
  إذا (unlikely (c->budget_بچیا < 0))
  {
    hb_raster_paint_ڈالو_خالی_تھاواں_clip (c, w, h);
    أرجع;
  }

  hb_raster_draw_t *rdr = c->clip_rdr;
  hb_transform_t<> t = c->current_effective_transform ();
  hb_raster_draw_سیٹ_transform (rdr, t.xx, t.yx, t.xy, t.yy, t.x0, t.y0);
  hb_raster_paint_attach_clip_rdr (c, rdr, surf);
  emit (rdr, emit_انکڑے);
  hb_raster_paint_readback_budget (c, rdr);

  hb_raster_paint_finalize_رستہ_clip (c, rdr, surf, w, h);
}

بنية hb_raster_paint_glyph_clip_انکڑے_t
{
  hb_codepoint_t glyph;
  hb_font_t *font;
};

ساكن فراغ
hb_raster_paint_emit_clip_glyph_نقاب (hb_raster_draw_t *rdr, فراغ *user_انکڑے)
{
  hb_raster_paint_glyph_clip_انکڑے_t *انکڑے = (hb_raster_paint_glyph_clip_انکڑے_t *) user_انکڑے;
  /* Let draw-render choose tight glyph extents; we map by mask origin below. */
  hb_font_draw_glyph (انکڑے->font, انکڑے->glyph, hb_raster_draw_get_funcs (rdr), rdr);
}

ساكن فراغ
hb_raster_paint_ڈالو_clip_glyph (hb_paint_funcs_t *pfuncs HB_بن_ورتیا,
				 فراغ *paint_انکڑے,
				 hb_codepoint_t glyph,
				 hb_font_t *font,
				 فراغ *user_انکڑے HB_بن_ورتیا)
{
  hb_raster_paint_t *c = (hb_raster_paint_t *) paint_انکڑے;
  hb_raster_paint_glyph_clip_انکڑے_t انکڑے = {glyph, font};
  hb_raster_paint_ڈالو_clip_from_emitter (c, hb_raster_paint_emit_clip_glyph_نقاب, &انکڑے);
}

ساكن فراغ
hb_raster_paint_ڈالو_clip_rectangle (hb_paint_funcs_t *pfuncs HB_بن_ورتیا,
				     فراغ *paint_انکڑے,
				     عائم xmin, عائم ymin,
				     عائم xmax, عائم ymax,
				     فراغ *user_انکڑے HB_بن_ورتیا)
{
  hb_raster_paint_t *c = (hb_raster_paint_t *) paint_انکڑے;

  ensure_initialized (c);

  إذا (!c->surface_ڈھیر.لمائی) أرجع;

  hb_transform_t<> t = c->current_effective_transform ();

  hb_raster_image_t *surf = c->current_surface ();
  إذا (unlikely (!surf)) أرجع;

  بنانشانی w = surf->extents.چوڑائی;
  بنانشانی h = surf->extents.اچائی;
  منطقي is_axis_aligned = (t.xy == 0.f && t.yx == 0.f);

  /* Transform the four corners to pixel space */
  عائم cx[4], cy[4];
  cx[0] = xmin; cy[0] = ymin;
  cx[1] = xmax; cy[1] = ymin;
  cx[2] = xmax; cy[2] = ymax;
  cx[3] = xmin; cy[3] = ymax;
  لكل (بنانشانی i = 0; i < 4; i++)
    t.transform_نقطہ (cx[i], cy[i]);

  /* Compute bounding box in pixel coords */
  عائم fmin_x = cx[0], fmin_y = cy[0], fmax_x = cx[0], fmax_y = cy[0];
  لكل (بنانشانی i = 1; i < 4; i++)
  {
    fmin_x = hb_min (fmin_x, cx[i]); fmin_y = hb_min (fmin_y, cy[i]);
    fmax_x = hb_max (fmax_x, cx[i]); fmax_y = hb_max (fmax_y, cy[i]);
  }

  /* A spec-legal but very large transform can push these bounds past the int
     range or to non-finite values; a bare float->int conversion would then be
     undefined.  Saturate through hb_clamp_to<> (which also maps NaN to
     INT32_MIN); the subtraction is done in double to avoid overflow. */
  صحيح px0 = hb_clamp_منزل<int32_t> ((مزدوج) floorf (fmin_x) - surf->extents.x_origin);
  صحيح py0 = hb_clamp_منزل<int32_t> ((مزدوج) floorf (fmin_y) - surf->extents.y_origin);
  صحيح px1 = hb_clamp_منزل<int32_t> ((مزدوج) ceilf  (fmax_x) - surf->extents.x_origin);
  صحيح py1 = hb_clamp_منزل<int32_t> ((مزدوج) ceilf  (fmax_y) - surf->extents.y_origin);

  /* Clamp to surface bounds */
  px0 = hb_max (px0, 0);
  py0 = hb_max (py0, 0);
  px1 = hb_min (px1, (صحيح) w);
  py1 = hb_min (py1, (صحيح) h);

  ثابت hb_raster_clip_t &old_clip = c->current_clip ();
  إذا (unlikely (!old_clip.has_valid_alpha_نقاب ()))
  {
    hb_raster_paint_ڈالو_خالی_تھاواں_clip (c, w, h);
    أرجع;
  }

  /* All paths below except rect-on-rect run a full clip-buffer pass. */
  إذا (!(is_axis_aligned && old_clip.is_rect) &&
      unlikely (!c->charge_work ((int64_t) w * h)))
  {
    hb_raster_paint_ڈالو_خالی_تھاواں_clip (c, w, h);
    أرجع;
  }

  hb_raster_clip_t new_clip = c->acquire_clip (w, h);

  إذا (is_axis_aligned && old_clip.is_rect)
  {
    /* Fast path: axis-aligned rect-on-rect intersection */
    new_clip.is_rect = سچ;
    new_clip.rect_x0 = hb_max (px0, old_clip.rect_x0);
    new_clip.rect_y0 = hb_max (py0, old_clip.rect_y0);
    new_clip.rect_x1 = hb_min (px1, old_clip.rect_x1);
    new_clip.rect_y1 = hb_min (py1, old_clip.rect_y1);
    new_clip.update_bounds_from_rect ();
  }
  وإلا إذا (is_axis_aligned)
  {
    /* Fast path: intersect an axis-aligned rect with an existing alpha clip. */
    new_clip.is_rect = خطأ;
    size_t clip_ناپ = (size_t) new_clip.stride * h;
    إذا (unlikely (clip_ناپ > HB_RASTER_MAX_بفر_ناپ ||
                  !new_clip.alpha.resize ((بنانشانی) clip_ناپ)))
    {
      hb_raster_paint_ڈالو_خالی_تھاواں_clip (c, w, h);
      أرجع;
    }
    hb_memset (new_clip.alpha.arrayZ, 0, (بنانشانی) clip_ناپ);

    صحيح iy0_i = hb_max (py0, (صحيح) old_clip.min_y);
    صحيح iy1_i = hb_min (py1, (صحيح) old_clip.max_y);
    صحيح ix0_i = hb_max (px0, (صحيح) old_clip.min_x);
    صحيح ix1_i = hb_min (px1, (صحيح) old_clip.max_x);

    إذا (ix0_i < ix1_i && iy0_i < iy1_i)
    {
      بنانشانی iy0 = (بنانشانی) iy0_i;
      بنانشانی iy1 = (بنانشانی) iy1_i;
      بنانشانی ix0 = (بنانشانی) ix0_i;
      بنانشانی ix1 = (بنانشانی) ix1_i;

      لكل (بنانشانی y = iy0; y < iy1; y++)
      {
        ثابت uint8_t *old_صف = old_clip.alpha.arrayZ + y * old_clip.stride;
        uint8_t *new_صف = new_clip.alpha.arrayZ + y * new_clip.stride;
        hb_memcpy (new_صف + ix0, old_صف + ix0, ix1 - ix0);
      }
      new_clip.min_x = ix0;
      new_clip.min_y = iy0;
      new_clip.max_x = ix1;
      new_clip.max_y = iy1;
    }
  }
  وإلا
  {
    /* General case: rasterize transformed quad as alpha mask */
    new_clip.is_rect = خطأ;
    size_t clip_ناپ = (size_t) new_clip.stride * h;
    إذا (unlikely (clip_ناپ > HB_RASTER_MAX_بفر_ناپ ||
                  !new_clip.alpha.resize ((بنانشانی) clip_ناپ)))
    {
      hb_raster_paint_ڈالو_خالی_تھاواں_clip (c, w, h);
      أرجع;
    }
    hb_memset (new_clip.alpha.arrayZ, 0, (بنانشانی) clip_ناپ);

    /* Convert quad corners to pixel-relative coords */
    عائم qx[4], qy[4];
    صحيح ox = surf->extents.x_origin;
    صحيح oy = surf->extents.y_origin;
    لكل (بنانشانی i = 0; i < 4; i++)
    {
      qx[i] = cx[i] - ox;
      qy[i] = cy[i] - oy;
    }

    /* For each pixel in the bounding box, test if inside the quad
     * using cross-product edge tests (winding order). */
    صحيح iy0_i = hb_max (py0, (صحيح) old_clip.min_y);
    صحيح iy1_i = hb_min (py1, (صحيح) old_clip.max_y);
    صحيح ix0_i = hb_max (px0, (صحيح) old_clip.min_x);
    صحيح ix1_i = hb_min (px1, (صحيح) old_clip.max_x);
    new_clip.min_x = w; new_clip.min_y = h;
    new_clip.max_x = 0; new_clip.max_y = 0;

    إذا (ix0_i < ix1_i && iy0_i < iy1_i)
    {
      بنانشانی iy0 = (بنانشانی) iy0_i;
      بنانشانی iy1 = (بنانشانی) iy1_i;
      بنانشانی ix0 = (بنانشانی) ix0_i;
      بنانشانی ix1 = (بنانشانی) ix1_i;

      /* Precompute edge normals for point-in-quad test.
       * Edge i goes from corner i to corner (i+1)%4.
       * Normal = (dy, -dx); inside test: dot(normal, p-corner) >= 0 */
	      عائم enx[4], eny[4], ed[4];
	      لكل (بنانشانی i = 0; i < 4; i++)
	      {
		بنانشانی j = (i + 1) & 3;
		عائم edx = qx[j] - qx[i], edy = qy[j] - qy[i];
		enx[i] = edy;       /* normal x */
		eny[i] = -edx;      /* normal y */
		ed[i] = enx[i] * qx[i] + eny[i] * qy[i]; /* distance threshold */
	      }
	      عائم area2 = 0.f;
	      لكل (بنانشانی i = 0; i < 4; i++)
	      {
		بنانشانی j = (i + 1) & 3;
		area2 += qx[i] * qy[j] - qx[j] * qy[i];
	      }
	      منطقي ccw = area2 >= 0.f;

      إذا (old_clip.is_rect)
      {
	لكل (بنانشانی y = iy0; y < iy1; y++)
	  لكل (بنانشانی x = ix0; x < ix1; x++)
	  {
	    عائم px_f = x + 0.5f, py_f = y + 0.5f;
	    /* Test if pixel center is inside the quad */
		    منطقي inside = سچ;
		    لكل (بنانشانی i = 0; i < 4; i++)
		    {
		      عائم d = enx[i] * px_f + eny[i] * py_f;
		      إذا (ccw ? d < ed[i] : d > ed[i])
		      {
			inside = خطأ;
			اكسر;
		      }
		    }
	    uint8_t a = inside ? 255 : 0;
	    new_clip.alpha[y * new_clip.stride + x] = a;
	    إذا (a)
	    {
	      new_clip.min_x = hb_min (new_clip.min_x, x);
	      new_clip.min_y = hb_min (new_clip.min_y, y);
	      new_clip.max_x = hb_max (new_clip.max_x, x + 1);
	      new_clip.max_y = hb_max (new_clip.max_y, y + 1);
	    }
	  }
      }
      وإلا
      {
	لكل (بنانشانی y = iy0; y < iy1; y++)
	{
	  ثابت uint8_t *old_صف = old_clip.alpha.arrayZ + y * old_clip.stride;
	  لكل (بنانشانی x = ix0; x < ix1; x++)
	  {
	    عائم px_f = x + 0.5f, py_f = y + 0.5f;
	    /* Test if pixel center is inside the quad */
		    منطقي inside = سچ;
		    لكل (بنانشانی i = 0; i < 4; i++)
		    {
		      عائم d = enx[i] * px_f + eny[i] * py_f;
		      إذا (ccw ? d < ed[i] : d > ed[i])
		      {
			inside = خطأ;
			اكسر;
		      }
		    }
	    uint8_t a = inside ? old_صف[x] : 0;
	    new_clip.alpha[y * new_clip.stride + x] = a;
	    إذا (a)
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

  إذا (unlikely (!c->clip_ڈھیر.ڈالو_or_fail (std::move (new_clip))))
    hb_raster_paint_ڈالو_خالی_تھاواں_clip (c, surf->extents.چوڑائی, surf->extents.اچائی);
}

ساكن hb_draw_funcs_t *
hb_raster_paint_ڈالو_clip_رستہ_شروع (hb_paint_funcs_t *pfuncs HB_بن_ورتیا,
				      فراغ *paint_انکڑے,
				      فراغ **draw_انکڑے,
				      فراغ *user_انکڑے HB_بن_ورتیا)
{
  hb_raster_paint_t *c = (hb_raster_paint_t *) paint_انکڑے;

  ensure_initialized (c);

  /* Prime clip_rdr with the current effective transform; the
   * caller then drives hb_draw_*() into it, and _end renders
   * the accumulated edges into a mask and intersects with the
   * current clip. */
  إذا (unlikely (!c->surface_ڈھیر.لمائی || !c->clip_rdr))
  {
    *draw_انکڑے = خالی_سوچک;
    أرجع خالی_سوچک;
  }

  hb_raster_draw_t *rdr = c->clip_rdr;
  hb_transform_t<> t = c->current_effective_transform ();
  hb_raster_draw_سیٹ_transform (rdr, t.xx, t.yx, t.xy, t.yy, t.x0, t.y0);
  hb_raster_image_t *surf = c->current_surface ();
  إذا (likely (surf))
    hb_raster_paint_attach_clip_rdr (c, rdr, surf);

  *draw_انکڑے = rdr;
  أرجع hb_raster_draw_get_funcs (rdr);
}

ساكن فراغ
hb_raster_paint_ڈالو_clip_رستہ_سرا (hb_paint_funcs_t *pfuncs HB_بن_ورتیا,
				    فراغ *paint_انکڑے,
				    فراغ *user_انکڑے HB_بن_ورتیا)
{
  hb_raster_paint_t *c = (hb_raster_paint_t *) paint_انکڑے;

  hb_raster_image_t *surf = c->current_surface ();
  إذا (unlikely (!surf || !c->clip_rdr)) أرجع;

  بنانشانی w = surf->extents.چوڑائی;
  بنانشانی h = surf->extents.اچائی;

  hb_raster_paint_readback_budget (c, c->clip_rdr);
  إذا (unlikely (c->budget_بچیا < 0))
  {
    hb_raster_draw_clear (c->clip_rdr);
    hb_raster_paint_ڈالو_خالی_تھاواں_clip (c, w, h);
    أرجع;
  }

  hb_raster_paint_finalize_رستہ_clip (c, c->clip_rdr, surf, w, h);
}

ساكن فراغ
hb_raster_paint_نکالو_clip (hb_paint_funcs_t *pfuncs HB_بن_ورتیا,
			  فراغ *paint_انکڑے,
			  فراغ *user_انکڑے HB_بن_ورتیا)
{
  hb_raster_paint_t *c = (hb_raster_paint_t *) paint_انکڑے;
  إذا (!c->clip_ڈھیر.لمائی) أرجع;
  c->release_clip (c->clip_ڈھیر.نکالو ());
}

ساكن فراغ
hb_raster_paint_ڈالو_گروہ (hb_paint_funcs_t *pfuncs HB_بن_ورتیا,
			    فراغ *paint_انکڑے,
			    فراغ *user_انکڑے HB_بن_ورتیا)
{
  hb_raster_paint_t *c = (hb_raster_paint_t *) paint_انکڑے;

  ensure_initialized (c);

  /* acquire_surface() clears a full surface; charge its area. */
  إذا (unlikely (!c->charge_work ((int64_t) c->fixed_extents.چوڑائی * c->fixed_extents.اچائی)))
    أرجع;

  hb_raster_image_t *new_surf = c->acquire_surface ();
  إذا (unlikely (!new_surf)) أرجع;
  إذا (unlikely (!c->surface_ڈھیر.ڈالو_or_fail (new_surf)))
    c->release_surface (new_surf);
}

ساكن فراغ
hb_raster_paint_نکالو_گروہ (hb_paint_funcs_t *pfuncs HB_بن_ورتیا,
			   فراغ *paint_انکڑے,
			   hb_paint_مرکب_منوال_t منوال,
			   فراغ *user_انکڑے HB_بن_ورتیا)
{
  hb_raster_paint_t *c = (hb_raster_paint_t *) paint_انکڑے;

  إذا (c->surface_ڈھیر.لمائی < 2) أرجع;

  hb_raster_image_t *src = c->surface_ڈھیر.نکالو ();
  hb_raster_image_t *dst = c->current_surface ();

  إذا (dst && src &&
      likely (c->charge_work ((int64_t) dst->extents.چوڑائی * dst->extents.اچائی)))
    hb_raster_image_مرکب (dst, src, منوال);

  c->release_surface (src);
}

ساكن فراغ
hb_raster_paint_solid (hb_raster_paint_t *c,
		       hb_raster_image_t *surf,
		       hb_color_t color,
		       hb_raster_image_t *نقاب_img)
{
  uint32_t premul = color_منزل_premul_pixel (color);
  uint8_t premul_a = (uint8_t) (premul >> 24);
  ثابت hb_raster_clip_t &clip = c->current_clip ();

  بنانشانی stride = surf->extents.stride;
  إذا (clip.min_x >= clip.max_x || clip.min_y >= clip.max_y) أرجع;
  إذا (premul_a == 0) أرجع;

  بنانشانی ix0 = clip.min_x, iy0 = clip.min_y;
  بنانشانی ix1 = clip.max_x, iy1 = clip.max_y;
  ثابت uint8_t *نقاب_buf = خالی_سوچک;
  hb_raster_extents_t نقاب_ext;
  int64_t نقاب_x0 = 0, نقاب_y0 = 0;
  إذا (نقاب_img)
  {
    نقاب_buf = hb_raster_image_get_بفر (نقاب_img);
    hb_raster_image_get_extents (نقاب_img, &نقاب_ext);
    إذا (!hb_raster_paint_intersect_نقاب (surf, نقاب_ext, clip,
					 surf->extents.چوڑائی, surf->extents.اچائی,
					 &نقاب_x0, &نقاب_y0,
					 &ix0, &iy0, &ix1, &iy1))
      أرجع;
  }

  إذا (unlikely (!c->charge_work ((int64_t) (ix1 - ix0) * (iy1 - iy0))))
    أرجع;

  إذا (likely (!clip.is_rect))
  {
    لكل (بنانشانی y = iy0; y < iy1; y++)
    {
      hb_packed_t<uint32_t> *__restrict صف = (hb_packed_t<uint32_t> *) (surf->بفر.arrayZ + y * stride);
      ثابت uint8_t *__restrict clip_صف = clip.alpha.arrayZ + y * clip.stride;
      ثابت uint8_t *__restrict نقاب_صف = نقاب_buf ? نقاب_buf + (بنانشانی) ((int64_t) y - نقاب_y0) * نقاب_ext.stride
						    : خالی_سوچک;
      بنانشانی mx = (بنانشانی) ((int64_t) ix0 - نقاب_x0);
      لكل (بنانشانی x = ix0; x < ix1; x++)
      {
	uint8_t clip_alpha = clip_صف[x];
	إذا (نقاب_صف)
	  clip_alpha = hb_raster_div255 (نقاب_صف[mx++] * clip_alpha);
	إذا (clip_alpha == 0) استمر;
	إذا (clip_alpha == 255)
	{
	  إذا (premul_a == 255)
	    صف[x] = hb_packed_t<uint32_t> (premul);
	  وإلا
	    صف[x] = hb_packed_t<uint32_t> (hb_raster_src_over (premul, (uint32_t) صف[x]));
	}
	وإلا
	{
	  uint32_t src = hb_raster_alpha_mul (premul, clip_alpha);
	  صف[x] = hb_packed_t<uint32_t> (hb_raster_src_over (src, (uint32_t) صف[x]));
	}
      }
    }
  }
  وإلا إذا (نقاب_buf)
  {
    لكل (بنانشانی y = iy0; y < iy1; y++)
    {
      hb_packed_t<uint32_t> *__restrict صف = (hb_packed_t<uint32_t> *) (surf->بفر.arrayZ + y * stride);
      ثابت uint8_t *__restrict نقاب_صف = نقاب_buf + (بنانشانی) ((int64_t) y - نقاب_y0) * نقاب_ext.stride;
      بنانشانی mx = (بنانشانی) ((int64_t) ix0 - نقاب_x0);
      لكل (بنانشانی x = ix0; x < ix1; x++)
      {
	uint8_t نقاب_alpha = نقاب_صف[mx++];
	إذا (نقاب_alpha == 0) استمر;
	uint32_t src = hb_raster_alpha_mul (premul, نقاب_alpha);
	صف[x] = hb_packed_t<uint32_t> (hb_raster_src_over (src, (uint32_t) صف[x]));
      }
    }
  }
  وإلا
  {
    لكل (بنانشانی y = clip.min_y; y < clip.max_y; y++)
    {
      hb_packed_t<uint32_t> *صف = (hb_packed_t<uint32_t> *) (surf->بفر.arrayZ + y * stride);
      لكل (بنانشانی x = clip.min_x; x < clip.max_x; x++)
	صف[x] = hb_packed_t<uint32_t> (hb_raster_src_over (premul, (uint32_t) صف[x]));
    }
  }
}

ساكن فراغ
hb_raster_paint_color (hb_paint_funcs_t *pfuncs HB_بن_ورتیا,
		       فراغ *paint_انکڑے,
		       hb_bool_t is_foreground HB_بن_ورتیا,
		       hb_color_t color,
		       فراغ *user_انکڑے HB_بن_ورتیا)
{
  hb_raster_paint_t *c = (hb_raster_paint_t *) paint_انکڑے;

  ensure_initialized (c);

  hb_raster_image_t *surf = c->current_surface ();
  إذا (unlikely (!surf)) أرجع;

  hb_raster_paint_solid (c, surf, color, خالی_سوچک);
}

ساكن فراغ
hb_raster_paint_fill_glyph (hb_paint_funcs_t *pfuncs HB_بن_ورتیا,
			    فراغ *paint_انکڑے,
			    hb_codepoint_t glyph,
			    hb_font_t *font,
			    hb_bool_t is_foreground HB_بن_ورتیا,
			    hb_color_t color,
			    فراغ *user_انکڑے HB_بن_ورتیا)
{
  hb_raster_paint_t *c = (hb_raster_paint_t *) paint_انکڑے;

  ensure_initialized (c);

  hb_raster_image_t *surf = c->current_surface ();
  إذا (unlikely (!surf)) أرجع;

  /* Out of budget: skip the glyph-outline extraction entirely. */
  إذا (unlikely (c->budget_بچیا < 0)) أرجع;

  hb_raster_draw_t *rdr = c->clip_rdr;
  hb_transform_t<> t = c->current_effective_transform ();
  hb_raster_draw_سیٹ_transform (rdr, t.xx, t.yx, t.xy, t.yy, t.x0, t.y0);
  hb_raster_paint_attach_clip_rdr (c, rdr, surf);

  hb_raster_paint_glyph_clip_انکڑے_t انکڑے = {glyph, font};
  hb_raster_paint_emit_clip_glyph_نقاب (rdr, &انکڑے);
  hb_raster_paint_readback_budget (c, rdr);

  إذا (unlikely (!c->precharge_work (hb_raster_draw_get_pixel_work (rdr,
							  surf->extents.اچائی,
							  surf->extents.چوڑائی))))
  {
    hb_raster_draw_clear (rdr);
    أرجع;
  }

  hb_raster_image_t *نقاب_img = hb_raster_draw_render (rdr);
  إذا (unlikely (!نقاب_img)) أرجع;

  hb_raster_extents_t نقاب_ext;
  hb_raster_image_get_extents (نقاب_img, &نقاب_ext);
  إذا (likely (c->charge_work ((int64_t) نقاب_ext.چوڑائی * نقاب_ext.اچائی)))
    hb_raster_paint_solid (c, surf, color, نقاب_img);

  hb_raster_draw_recycle_image (rdr, نقاب_img);
}

ساكن hb_bool_t
hb_raster_paint_image (hb_paint_funcs_t *pfuncs HB_بن_ورتیا,
		       فراغ *paint_انکڑے,
		       hb_blob_t *blob,
		       بنانشانی چوڑائی,
		       بنانشانی اچائی,
		       hb_ٹیگ_t format,
		       عائم slant HB_بن_ورتیا,
		       hb_glyph_extents_t *extents,
		       فراغ *user_انکڑے HB_بن_ورتیا)
{
  hb_raster_paint_t *c = (hb_raster_paint_t *) paint_انکڑے;

  ensure_initialized (c);

  /* Out of pixel budget: skip, including the image decode below.  Image
   * compositing is pixel work and does no outline traversal. */
  إذا (unlikely (c->pixel_بچیا < 0)) أرجع خطأ;

  بنانشانی src_چوڑائی = چوڑائی;
  بنانشانی src_اچائی = اچائی;
  ثابت hb_packed_t<uint32_t> *src_انکڑے = خالی_سوچک;
  hb_raster_image_t decoded_png;

  إذا (format == HB_PAINT_IMAGE_FORMAT_BGRA)
  {
    إذا (src_چوڑائی == 0 || src_اچائی == 0)
      أرجع خطأ;
    إذا (src_چوڑائی > (بنانشانی) ودھ_توں_ودھ_عدد || src_اچائی > (بنانشانی) ودھ_توں_ودھ_عدد)
      أرجع خطأ;

    بنانشانی انکڑے_len;
    ثابت uint8_t *انکڑے = (ثابت uint8_t *) hb_blob_get_انکڑے (blob, &انکڑے_len);
    size_t pixel_گنتی = (size_t) src_چوڑائی * (size_t) src_اچائی;
    إذا (src_چوڑائی && pixel_گنتی / src_چوڑائی != src_اچائی)
      أرجع خطأ;
    إذا (pixel_گنتی > (size_t) -1 / 4u)
      أرجع خطأ;
    size_t required_ناپ = pixel_گنتی * 4u;
    إذا (!انکڑے || (size_t) انکڑے_len < required_ناپ)
      أرجع خطأ;

    src_انکڑے = (ثابت hb_packed_t<uint32_t> *) انکڑے;
  }
  وإلا إذا (format == HB_PAINT_IMAGE_FORMAT_PNG)
  {
#جے_تعریف_شدہ HAVE_PNG
    إذا (!decoded_png.deserialize_from_png (blob))
      أرجع خطأ;
    src_چوڑائی = decoded_png.extents.چوڑائی;
    src_اچائی = decoded_png.extents.اچائی;
    src_انکڑے = (ثابت hb_packed_t<uint32_t> *) decoded_png.بفر.arrayZ;
#وإلا
    أرجع خطأ;
#جے_انت
  }
  وإلا
    أرجع خطأ;

  hb_raster_image_t *surf = c->current_surface ();
  إذا (unlikely (!surf)) أرجع خطأ;
  إذا (!extents) أرجع خطأ;

  ثابت hb_raster_clip_t &clip = c->current_clip ();
  hb_transform_t<> t = c->current_effective_transform ();

  /* Compute inverse transform for sampling */
  عائم det = t.xx * t.yy - t.xy * t.yx;
  إذا (fabsf (det) < 1e-10f) أرجع خطأ;
  عائم inv_det = 1.f / det;
  عائم inv_xx =  t.yy * inv_det;
  عائم inv_xy = -t.xy * inv_det;
  عائم inv_yx = -t.yx * inv_det;
  عائم inv_yy =  t.xx * inv_det;
  عائم inv_x0 = (t.xy * t.y0 - t.yy * t.x0) * inv_det;
  عائم inv_y0 = (t.yx * t.x0 - t.xx * t.y0) * inv_det;

  بنانشانی surf_stride = surf->extents.stride;
  صحيح ox = surf->extents.x_origin;
  صحيح oy = surf->extents.y_origin;

  /* Image source rectangle in glyph space */
  عائم img_x = extents->x_bearing;
  عائم img_y = extents->y_bearing;
  عائم img_sx = (عائم) extents->چوڑائی / src_چوڑائی;
  عائم img_sy = (عائم) extents->اچائی / src_اچائی;
  إذا (fabsf (img_sx) < 1e-10f || fabsf (img_sy) < 1e-10f)
    أرجع خطأ;

  {
    بنانشانی clip_w = clip.max_x > clip.min_x ? clip.max_x - clip.min_x : 0;
    بنانشانی clip_h = clip.max_y > clip.min_y ? clip.max_y - clip.min_y : 0;
    إذا (unlikely (!c->charge_work ((int64_t) clip_w * clip_h +
				   (int64_t) src_چوڑائی * src_اچائی)))
      أرجع خطأ;
  }

  إذا (clip.is_rect)
  {
    لكل (بنانشانی py = clip.min_y; py < clip.max_y; py++)
    {
      hb_packed_t<uint32_t> *صف = (hb_packed_t<uint32_t> *) (surf->بفر.arrayZ + py * surf_stride);
      عائم gx = inv_xx * (عائم) ((صحيح) clip.min_x + ox) + inv_xy * (عائم) ((صحيح) py + oy) + inv_x0;
      عائم gy = inv_yx * (عائم) ((صحيح) clip.min_x + ox) + inv_yy * (عائم) ((صحيح) py + oy) + inv_y0;
      لكل (بنانشانی px = clip.min_x; px < clip.max_x; px++)
      {
	/* Map glyph space to image texel; bilinear reconstruction. */
	عائم ix = (gx - img_x) / img_sx;
	عائم iy = (عائم) (src_اچائی - 1) - (gy - img_y) / img_sy;

	إذا (unlikely (!std::isfinite (ix) || !std::isfinite (iy)) ||
	    ix < 0.f || iy < 0.f ||
	    ix > (عائم) (src_چوڑائی - 1) || iy > (عائم) (src_اچائی - 1))
	{
	  gx += inv_xx;
	  gy += inv_yx;
	  استمر;
	}

	uint32_t src_px = hb_raster_sample_bilinear_premul (src_انکڑے, src_چوڑائی, src_اچائی,
							     ix, iy);
	صف[px] = hb_packed_t<uint32_t> (hb_raster_src_over (src_px, (uint32_t) صف[px]));
	gx += inv_xx;
	gy += inv_yx;
      }
    }
  }
  وإلا
  {
    لكل (بنانشانی py = clip.min_y; py < clip.max_y; py++)
    {
      hb_packed_t<uint32_t> *صف = (hb_packed_t<uint32_t> *) (surf->بفر.arrayZ + py * surf_stride);
      ثابت uint8_t *clip_صف = clip.alpha.arrayZ + py * clip.stride;
      عائم gx = inv_xx * (عائم) ((صحيح) clip.min_x + ox) + inv_xy * (عائم) ((صحيح) py + oy) + inv_x0;
      عائم gy = inv_yx * (عائم) ((صحيح) clip.min_x + ox) + inv_yy * (عائم) ((صحيح) py + oy) + inv_y0;
      لكل (بنانشانی px = clip.min_x; px < clip.max_x; px++)
      {
	uint8_t clip_alpha = clip_صف[px];
	إذا (clip_alpha == 0)
	{
	  gx += inv_xx;
	  gy += inv_yx;
	  استمر;
	}

	/* Map glyph space to image texel; bilinear reconstruction. */
	عائم ix = (gx - img_x) / img_sx;
	عائم iy = (عائم) (src_اچائی - 1) - (gy - img_y) / img_sy;

	إذا (unlikely (!std::isfinite (ix) || !std::isfinite (iy)) ||
	    ix < 0.f || iy < 0.f ||
	    ix > (عائم) (src_چوڑائی - 1) || iy > (عائم) (src_اچائی - 1))
	{
	  gx += inv_xx;
	  gy += inv_yx;
	  استمر;
	}

	uint32_t src_px = hb_raster_sample_bilinear_premul (src_انکڑے, src_چوڑائی, src_اچائی,
							     ix, iy);
	src_px = hb_raster_alpha_mul (src_px, clip_alpha);
	صف[px] = hb_packed_t<uint32_t> (hb_raster_src_over (src_px, (uint32_t) صف[px]));
	gx += inv_xx;
	gy += inv_yx;
      }
    }
  }

  أرجع سچ;
}


/*
 * Gradient helpers
 */

#تعریف_کرو GRADIENT_LUT_ناپ 256
#تعریف_کرو GRADIENT_LUT_MIN_PIXELS (64u * 64u)

ساكن HB_ALWAYS_INLINE عائم
reflect_gradient_t (عائم t)
{
  t = fmodf (fabsf (t), 2.f);
  أرجع t > 1.f ? 2.f - t : t;
}

/* Evaluate color at normalized position t, interpolating in premultiplied space. */
ساكن uint32_t
evaluate_color_سطر (ثابت hb_color_stop_t *stops, بنانشانی len, عائم t,
		     hb_paint_extend_t extend)
{
  إذا (unlikely (!len))
    أرجع 0;
  إذا (unlikely (len == 1))
    أرجع color_منزل_premul_pixel (stops[0].color);
  إذا (unlikely (!std::isfinite (t)))
    t = 0.f;

  /* Apply extend mode */
  إذا (extend == HB_PAINT_EXTEND_PAD)
  {
    t = hb_clamp (t, 0.f, 1.f);
  }
  وإلا إذا (extend == HB_PAINT_EXTEND_REPEAT)
  {
    t = t - floorf (t);
  }
  وإلا /* REFLECT */
  {
    t = reflect_gradient_t (t);
  }

  /* Find bounding stops */
  إذا (t <= stops[0].وتھ)
    أرجع color_منزل_premul_pixel (stops[0].color);
  إذا (t >= stops[len - 1].وتھ)
    أرجع color_منزل_premul_pixel (stops[len - 1].color);

  بنانشانی i;
  لكل (i = 0; i < len - 1; i++)
    إذا (t < stops[i + 1].وتھ)
      اكسر;

  عائم range = stops[i + 1].وتھ - stops[i].وتھ;
  عائم k = range > 0.f ? (t - stops[i].وتھ) / range : 0.f;

  /* Interpolate in premultiplied [0,255] space */
  hb_color_t c0 = stops[i].color;
  hb_color_t c1 = stops[i + 1].color;

  عائم a0 = hb_color_get_alpha (c0) / 255.f;
  عائم r0 = hb_color_get_red   (c0) / 255.f * a0;
  عائم g0 = hb_color_get_green (c0) / 255.f * a0;
  عائم b0 = hb_color_get_blue  (c0) / 255.f * a0;

  عائم a1 = hb_color_get_alpha (c1) / 255.f;
  عائم r1 = hb_color_get_red   (c1) / 255.f * a1;
  عائم g1 = hb_color_get_green (c1) / 255.f * a1;
  عائم b1 = hb_color_get_blue  (c1) / 255.f * a1;

  عائم a = a0 + k * (a1 - a0);
  عائم r = r0 + k * (r1 - r0);
  عائم g = g0 + k * (g1 - g0);
  عائم b = b0 + k * (b1 - b0);

  uint8_t pa = (uint8_t) (a * 255.f + 0.5f);
  uint8_t pr = (uint8_t) (r * 255.f + 0.5f);
  uint8_t pg = (uint8_t) (g * 255.f + 0.5f);
  uint8_t pb = (uint8_t) (b * 255.f + 0.5f);

  أرجع (uint32_t) pb | ((uint32_t) pg << 8) | ((uint32_t) pr << 16) | ((uint32_t) pa << 24);
}

ساكن HB_ALWAYS_INLINE عائم
normalize_gradient_t (عائم t, hb_paint_extend_t extend)
{
  إذا (unlikely (!std::isfinite (t)))
    أرجع 0.f;

  إذا (extend == HB_PAINT_EXTEND_PAD)
    أرجع hb_clamp (t, 0.f, 1.f);
  إذا (extend == HB_PAINT_EXTEND_REPEAT)
  {
    t = t - floorf (t);
    أرجع t < 0.f ? t + 1.f : t;
  }

  /* REFLECT */
  أرجع reflect_gradient_t (t);
}

ساكن فراغ
build_gradient_lut (ثابت hb_color_stop_t *stops,
		    بنانشانی len,
		    uint32_t *lut)
{
  لكل (بنانشانی i = 0; i < GRADIENT_LUT_ناپ; i++)
  {
    عائم t = (عائم) i / (GRADIENT_LUT_ناپ - 1);
    lut[i] = evaluate_color_سطر (stops, len, t, HB_PAINT_EXTEND_PAD);
  }
}

ساكن HB_ALWAYS_INLINE uint32_t
lookup_gradient_lut (ثابت uint32_t *lut,
		     عائم t,
		     hb_paint_extend_t extend)
{
  عائم u = normalize_gradient_t (t, extend);
  إذا (unlikely (!std::isfinite (u)))
    u = 0.f;
  وإلا
    u = hb_clamp (u, 0.f, 1.f);
  بنانشانی idx = (بنانشانی) (u * (GRADIENT_LUT_ناپ - 1) + 0.5f);
  أرجع lut[idx];
}

/*
 * Gradient paint callbacks
 */

ساكن فراغ
hb_raster_paint_خطی_gradient (hb_paint_funcs_t *pfuncs HB_بن_ورتیا,
				 فراغ *paint_انکڑے,
				 hb_color_سطر_t *color_سطر,
				 عائم x0, عائم y0,
				 عائم x1, عائم y1,
				 عائم x2, عائم y2,
				 فراغ *user_انکڑے HB_بن_ورتیا)
{
  hb_raster_paint_t *c = (hb_raster_paint_t *) paint_انکڑے;

  ensure_initialized (c);

  hb_raster_image_t *surf = c->current_surface ();
  إذا (unlikely (!surf)) أرجع;

  إذا (unlikely (!c->fetch_color_stops (color_سطر)))
    أرجع;
  hb_color_stop_t *stops = c->scratch_color_stops.arrayZ;
  بنانشانی len = c->scratch_color_stops.لمائی;
  عائم mn, mx;
  hb_paint_normalize_color_سطر (stops, len, &mn, &mx);

  hb_paint_extend_t extend = hb_color_سطر_get_extend (color_سطر);
  ثابت hb_raster_clip_t &clip = c->current_clip ();
  بنانشانی clip_w = clip.max_x > clip.min_x ? clip.max_x - clip.min_x : 0;
  بنانشانی clip_h = clip.max_y > clip.min_y ? clip.max_y - clip.min_y : 0;
  إذا (unlikely (!c->charge_work ((int64_t) clip_w * clip_h)))
    أرجع;
  منطقي use_lut = (uint64_t) clip_w * clip_h >= GRADIENT_LUT_MIN_PIXELS;
  uint32_t lut[GRADIENT_LUT_ناپ];
  إذا (use_lut)
    build_gradient_lut (stops, len, lut);

  /* Reduce 3-point anchor to 2-point gradient axis */
  عائم lx0, ly0, lx1, ly1;
  hb_paint_reduce_خطی_anchors (x0, y0, x1, y1, x2, y2,
				  &lx0, &ly0, &lx1, &ly1);

  /* Apply normalization to endpoints */
  عائم gx0 = lx0 + mn * (lx1 - lx0);
  عائم gy0 = ly0 + mn * (ly1 - ly0);
  عائم gx1 = lx0 + mx * (lx1 - lx0);
  عائم gy1 = ly0 + mx * (ly1 - ly0);

  /* Inverse transform: pixel → glyph space */
  hb_transform_t<> t = c->current_effective_transform ();
  عائم det = t.xx * t.yy - t.xy * t.yx;
  إذا (fabsf (det) < 1e-10f) أرجع;

  {
    عائم inv_det = 1.f / det;
    عائم inv_xx =  t.yy * inv_det;
    عائم inv_xy = -t.xy * inv_det;
    عائم inv_yx = -t.yx * inv_det;
    عائم inv_yy =  t.xx * inv_det;
    عائم inv_x0 = (t.xy * t.y0 - t.yy * t.x0) * inv_det;
    عائم inv_y0 = (t.yx * t.x0 - t.xx * t.y0) * inv_det;

    /* Gradient direction vector and denominator for projection */
    عائم dx = gx1 - gx0, dy = gy1 - gy0;
    عائم denom = dx * dx + dy * dy;
    إذا (denom < 1e-10f) أرجع;
    عائم inv_denom = 1.f / denom;

    بنانشانی stride = surf->extents.stride;
    صحيح ox = surf->extents.x_origin;
    صحيح oy = surf->extents.y_origin;

    إذا (clip.is_rect)
    {
      لكل (بنانشانی py = clip.min_y; py < clip.max_y; py++)
      {
	hb_packed_t<uint32_t> *صف = (hb_packed_t<uint32_t> *) (surf->بفر.arrayZ + py * stride);
	عائم gx = inv_xx * ((عائم) ((صحيح) clip.min_x + ox) + 0.5f) + inv_xy * ((عائم) ((صحيح) py + oy) + 0.5f) + inv_x0;
	عائم gy = inv_yx * ((عائم) ((صحيح) clip.min_x + ox) + 0.5f) + inv_yy * ((عائم) ((صحيح) py + oy) + 0.5f) + inv_y0;
	إذا (use_lut)
	{
	  لكل (بنانشانی px = clip.min_x; px < clip.max_x; px++)
	  {
	    عائم proj_t = ((gx - gx0) * dx + (gy - gy0) * dy) * inv_denom;
	    uint32_t src = lookup_gradient_lut (lut, proj_t, extend);
	    صف[px] = hb_packed_t<uint32_t> (hb_raster_src_over (src, (uint32_t) صف[px]));
	    gx += inv_xx;
	    gy += inv_yx;
	  }
	}
	وإلا
	{
	  لكل (بنانشانی px = clip.min_x; px < clip.max_x; px++)
	  {
	    عائم proj_t = ((gx - gx0) * dx + (gy - gy0) * dy) * inv_denom;
	    uint32_t src = evaluate_color_سطر (stops, len, proj_t, extend);
	    صف[px] = hb_packed_t<uint32_t> (hb_raster_src_over (src, (uint32_t) صف[px]));
	    gx += inv_xx;
	    gy += inv_yx;
	  }
	}
      }
    }
    وإلا
    {
      لكل (بنانشانی py = clip.min_y; py < clip.max_y; py++)
      {
	hb_packed_t<uint32_t> *صف = (hb_packed_t<uint32_t> *) (surf->بفر.arrayZ + py * stride);
	ثابت uint8_t *clip_صف = clip.alpha.arrayZ + py * clip.stride;
	عائم gx = inv_xx * ((عائم) ((صحيح) clip.min_x + ox) + 0.5f) + inv_xy * ((عائم) ((صحيح) py + oy) + 0.5f) + inv_x0;
	عائم gy = inv_yx * ((عائم) ((صحيح) clip.min_x + ox) + 0.5f) + inv_yy * ((عائم) ((صحيح) py + oy) + 0.5f) + inv_y0;
	إذا (use_lut)
	{
	  لكل (بنانشانی px = clip.min_x; px < clip.max_x; px++)
	  {
	    uint8_t clip_alpha = clip_صف[px];
	    إذا (clip_alpha == 0)
	    {
	      gx += inv_xx;
	      gy += inv_yx;
	      استمر;
	    }
	    عائم proj_t = ((gx - gx0) * dx + (gy - gy0) * dy) * inv_denom;
	    uint32_t src = lookup_gradient_lut (lut, proj_t, extend);
	    src = hb_raster_alpha_mul (src, clip_alpha);
	    صف[px] = hb_packed_t<uint32_t> (hb_raster_src_over (src, (uint32_t) صف[px]));
	    gx += inv_xx;
	    gy += inv_yx;
	  }
	}
	وإلا
	{
	  لكل (بنانشانی px = clip.min_x; px < clip.max_x; px++)
	  {
	    uint8_t clip_alpha = clip_صف[px];
	    إذا (clip_alpha == 0)
	    {
	      gx += inv_xx;
	      gy += inv_yx;
	      استمر;
	    }
	    عائم proj_t = ((gx - gx0) * dx + (gy - gy0) * dy) * inv_denom;
	    uint32_t src = evaluate_color_سطر (stops, len, proj_t, extend);
	    src = hb_raster_alpha_mul (src, clip_alpha);
	    صف[px] = hb_packed_t<uint32_t> (hb_raster_src_over (src, (uint32_t) صف[px]));
	    gx += inv_xx;
	    gy += inv_yx;
	  }
	}
      }
    }
  }

}

ساكن فراغ
hb_raster_paint_radial_gradient (hb_paint_funcs_t *pfuncs HB_بن_ورتیا,
				 فراغ *paint_انکڑے,
				 hb_color_سطر_t *color_سطر,
				 عائم x0, عائم y0, عائم r0,
				 عائم x1, عائم y1, عائم r1,
				 فراغ *user_انکڑے HB_بن_ورتیا)
{
  hb_raster_paint_t *c = (hb_raster_paint_t *) paint_انکڑے;

  ensure_initialized (c);

  hb_raster_image_t *surf = c->current_surface ();
  إذا (unlikely (!surf)) أرجع;

  إذا (unlikely (!c->fetch_color_stops (color_سطر)))
    أرجع;
  hb_color_stop_t *stops = c->scratch_color_stops.arrayZ;
  بنانشانی len = c->scratch_color_stops.لمائی;
  عائم mn, mx;
  hb_paint_normalize_color_سطر (stops, len, &mn, &mx);

  hb_paint_extend_t extend = hb_color_سطر_get_extend (color_سطر);
  ثابت hb_raster_clip_t &clip = c->current_clip ();
  بنانشانی clip_w = clip.max_x > clip.min_x ? clip.max_x - clip.min_x : 0;
  بنانشانی clip_h = clip.max_y > clip.min_y ? clip.max_y - clip.min_y : 0;
  إذا (unlikely (!c->charge_work ((int64_t) clip_w * clip_h)))
    أرجع;
  منطقي use_lut = (uint64_t) clip_w * clip_h >= GRADIENT_LUT_MIN_PIXELS;
  uint32_t lut[GRADIENT_LUT_ناپ];
  إذا (use_lut)
    build_gradient_lut (stops, len, lut);

  /* Apply normalization to circle parameters */
  عائم cx0 = x0 + mn * (x1 - x0);
  عائم cy0 = y0 + mn * (y1 - y0);
  عائم cr0 = r0 + mn * (r1 - r0);
  عائم cx1 = x0 + mx * (x1 - x0);
  عائم cy1 = y0 + mx * (y1 - y0);
  عائم cr1 = r0 + mx * (r1 - r0);

  /* Inverse transform */
  hb_transform_t<> t = c->current_effective_transform ();
  عائم det = t.xx * t.yy - t.xy * t.yx;
  إذا (fabsf (det) < 1e-10f) أرجع;

  {
    عائم inv_det = 1.f / det;
    عائم inv_xx =  t.yy * inv_det;
    عائم inv_xy = -t.xy * inv_det;
    عائم inv_yx = -t.yx * inv_det;
    عائم inv_yy =  t.xx * inv_det;
    عائم inv_x0 = (t.xy * t.y0 - t.yy * t.x0) * inv_det;
    عائم inv_y0 = (t.yx * t.x0 - t.xx * t.y0) * inv_det;

    /* Precompute quadratic coefficients for radial gradient:
     * |p - c0 - t*(c1-c0)|^2 = (r0 + t*(r1-r0))^2
     *
     * Expanding gives At^2 + Bt + C = 0 where:
     *   cdx = c1.x - c0.x, cdy = c1.y - c0.y, dr = r1 - r0
     *   A = cdx^2 + cdy^2 - dr^2
     *   B = -2*(px-c0.x)*cdx - 2*(py-c0.y)*cdy - 2*r0*dr
     *   C = (px-c0.x)^2 + (py-c0.y)^2 - r0^2
     */
    عائم cdx = cx1 - cx0, cdy = cy1 - cy0;
    عائم dr = cr1 - cr0;
    عائم A = cdx * cdx + cdy * cdy - dr * dr;

    بنانشانی stride = surf->extents.stride;
    صحيح ox = surf->extents.x_origin;
    صحيح oy = surf->extents.y_origin;

    إذا (clip.is_rect)
    {
      لكل (بنانشانی py = clip.min_y; py < clip.max_y; py++)
      {
	hb_packed_t<uint32_t> *صف = (hb_packed_t<uint32_t> *) (surf->بفر.arrayZ + py * stride);
	عائم gx = inv_xx * ((عائم) ((صحيح) clip.min_x + ox) + 0.5f) + inv_xy * ((عائم) ((صحيح) py + oy) + 0.5f) + inv_x0;
	عائم gy = inv_yx * ((عائم) ((صحيح) clip.min_x + ox) + 0.5f) + inv_yy * ((عائم) ((صحيح) py + oy) + 0.5f) + inv_y0;
	إذا (use_lut)
	{
	  لكل (بنانشانی px = clip.min_x; px < clip.max_x; px++)
	  {
	    عائم dpx = gx - cx0, dpy = gy - cy0;
	    عائم B = -2.f * (dpx * cdx + dpy * cdy + cr0 * dr);
	    عائم C = dpx * dpx + dpy * dpy - cr0 * cr0;

	    عائم grad_t;
	    إذا (fabsf (A) > 1e-10f)
	    {
	      عائم disc = B * B - 4.f * A * C;
	      إذا (disc < 0.f)
	      {
		gx += inv_xx;
		gy += inv_yx;
		استمر;
	      }
	      عائم sq = sqrtf (disc);
	      /* Pick the larger root (t closer to 1 = outer circle) */
	      عائم t1 = (-B + sq) / (2.f * A);
	      عائم t2 = (-B - sq) / (2.f * A);
	      /* Choose the root that gives a positive radius */
	      إذا (cr0 + t1 * dr >= 0.f)
		grad_t = t1;
	      وإلا
		grad_t = t2;
	    }
	    وإلا
	    {
	      /* Linear case: Bt + C = 0 */
	      إذا (fabsf (B) < 1e-10f)
	      {
		gx += inv_xx;
		gy += inv_yx;
		استمر;
	      }
	      grad_t = -C / B;
	    }

	    uint32_t src = lookup_gradient_lut (lut, grad_t, extend);
	    صف[px] = hb_packed_t<uint32_t> (hb_raster_src_over (src, (uint32_t) صف[px]));
	    gx += inv_xx;
	    gy += inv_yx;
	  }
	}
	وإلا
	{
	  لكل (بنانشانی px = clip.min_x; px < clip.max_x; px++)
	  {
	    عائم dpx = gx - cx0, dpy = gy - cy0;
	    عائم B = -2.f * (dpx * cdx + dpy * cdy + cr0 * dr);
	    عائم C = dpx * dpx + dpy * dpy - cr0 * cr0;

	    عائم grad_t;
	    إذا (fabsf (A) > 1e-10f)
	    {
	      عائم disc = B * B - 4.f * A * C;
	      إذا (disc < 0.f)
	      {
		gx += inv_xx;
		gy += inv_yx;
		استمر;
	      }
	      عائم sq = sqrtf (disc);
	      عائم t1 = (-B + sq) / (2.f * A);
	      عائم t2 = (-B - sq) / (2.f * A);
	      grad_t = (cr0 + t1 * dr >= 0.f) ? t1 : t2;
	    }
	    وإلا
	    {
	      إذا (fabsf (B) < 1e-10f)
	      {
		gx += inv_xx;
		gy += inv_yx;
		استمر;
	      }
	      grad_t = -C / B;
	    }

	    uint32_t src = evaluate_color_سطر (stops, len, grad_t, extend);
	    صف[px] = hb_packed_t<uint32_t> (hb_raster_src_over (src, (uint32_t) صف[px]));
	    gx += inv_xx;
	    gy += inv_yx;
	  }
	}
      }
    }
    وإلا
    {
      لكل (بنانشانی py = clip.min_y; py < clip.max_y; py++)
      {
	hb_packed_t<uint32_t> *صف = (hb_packed_t<uint32_t> *) (surf->بفر.arrayZ + py * stride);
	ثابت uint8_t *clip_صف = clip.alpha.arrayZ + py * clip.stride;
	عائم gx = inv_xx * ((عائم) ((صحيح) clip.min_x + ox) + 0.5f) + inv_xy * ((عائم) ((صحيح) py + oy) + 0.5f) + inv_x0;
	عائم gy = inv_yx * ((عائم) ((صحيح) clip.min_x + ox) + 0.5f) + inv_yy * ((عائم) ((صحيح) py + oy) + 0.5f) + inv_y0;
	إذا (use_lut)
	{
	  لكل (بنانشانی px = clip.min_x; px < clip.max_x; px++)
	  {
	    uint8_t clip_alpha = clip_صف[px];
	    إذا (clip_alpha == 0)
	    {
	      gx += inv_xx;
	      gy += inv_yx;
	      استمر;
	    }
	    عائم dpx = gx - cx0, dpy = gy - cy0;
	    عائم B = -2.f * (dpx * cdx + dpy * cdy + cr0 * dr);
	    عائم C = dpx * dpx + dpy * dpy - cr0 * cr0;

	    عائم grad_t;
	    إذا (fabsf (A) > 1e-10f)
	    {
	      عائم disc = B * B - 4.f * A * C;
	      إذا (disc < 0.f)
	      {
		gx += inv_xx;
		gy += inv_yx;
		استمر;
	      }
	      عائم sq = sqrtf (disc);
	      عائم t1 = (-B + sq) / (2.f * A);
	      عائم t2 = (-B - sq) / (2.f * A);
	      grad_t = (cr0 + t1 * dr >= 0.f) ? t1 : t2;
	    }
	    وإلا
	    {
	      إذا (fabsf (B) < 1e-10f)
	      {
		gx += inv_xx;
		gy += inv_yx;
		استمر;
	      }
	      grad_t = -C / B;
	    }

	    uint32_t src = lookup_gradient_lut (lut, grad_t, extend);
	    src = hb_raster_alpha_mul (src, clip_alpha);
	    صف[px] = hb_packed_t<uint32_t> (hb_raster_src_over (src, (uint32_t) صف[px]));
	    gx += inv_xx;
	    gy += inv_yx;
	  }
	}
	وإلا
	{
	  لكل (بنانشانی px = clip.min_x; px < clip.max_x; px++)
	  {
	    uint8_t clip_alpha = clip_صف[px];
	    إذا (clip_alpha == 0)
	    {
	      gx += inv_xx;
	      gy += inv_yx;
	      استمر;
	    }
	    عائم dpx = gx - cx0, dpy = gy - cy0;
	    عائم B = -2.f * (dpx * cdx + dpy * cdy + cr0 * dr);
	    عائم C = dpx * dpx + dpy * dpy - cr0 * cr0;

	    عائم grad_t;
	    إذا (fabsf (A) > 1e-10f)
	    {
	      عائم disc = B * B - 4.f * A * C;
	      إذا (disc < 0.f)
	      {
		gx += inv_xx;
		gy += inv_yx;
		استمر;
	      }
	      عائم sq = sqrtf (disc);
	      عائم t1 = (-B + sq) / (2.f * A);
	      عائم t2 = (-B - sq) / (2.f * A);
	      grad_t = (cr0 + t1 * dr >= 0.f) ? t1 : t2;
	    }
	    وإلا
	    {
	      إذا (fabsf (B) < 1e-10f)
	      {
		gx += inv_xx;
		gy += inv_yx;
		استمر;
	      }
	      grad_t = -C / B;
	    }

	    uint32_t src = evaluate_color_سطر (stops, len, grad_t, extend);
	    src = hb_raster_alpha_mul (src, clip_alpha);
	    صف[px] = hb_packed_t<uint32_t> (hb_raster_src_over (src, (uint32_t) صف[px]));
	    gx += inv_xx;
	    gy += inv_yx;
	  }
	}
      }
    }
  }

}

ساكن فراغ
hb_raster_paint_sweep_gradient (hb_paint_funcs_t *pfuncs HB_بن_ورتیا,
				فراغ *paint_انکڑے,
				hb_color_سطر_t *color_سطر,
				عائم cx, عائم cy,
				عائم شروع_angle,
				عائم سرا_angle,
				فراغ *user_انکڑے HB_بن_ورتیا)
{
  hb_raster_paint_t *c = (hb_raster_paint_t *) paint_انکڑے;

  ensure_initialized (c);

  hb_raster_image_t *surf = c->current_surface ();
  إذا (unlikely (!surf)) أرجع;

  إذا (unlikely (!c->fetch_color_stops (color_سطر)))
    أرجع;
  hb_color_stop_t *stops = c->scratch_color_stops.arrayZ;
  بنانشانی len = c->scratch_color_stops.لمائی;
  عائم mn, mx;
  hb_paint_normalize_color_سطر (stops, len, &mn, &mx);

  hb_paint_extend_t extend = hb_color_سطر_get_extend (color_سطر);
  ثابت hb_raster_clip_t &clip = c->current_clip ();
  بنانشانی clip_w = clip.max_x > clip.min_x ? clip.max_x - clip.min_x : 0;
  بنانشانی clip_h = clip.max_y > clip.min_y ? clip.max_y - clip.min_y : 0;
  إذا (unlikely (!c->charge_work ((int64_t) clip_w * clip_h)))
    أرجع;
  منطقي use_lut = (uint64_t) clip_w * clip_h >= GRADIENT_LUT_MIN_PIXELS;
  uint32_t lut[GRADIENT_LUT_ناپ];
  إذا (use_lut)
    build_gradient_lut (stops, len, lut);

  /* Apply normalization to angle range */
  عائم a0 = شروع_angle + mn * (سرا_angle - شروع_angle);
  عائم a1 = شروع_angle + mx * (سرا_angle - شروع_angle);
  عائم angle_range = a1 - a0;

  /* Inverse transform */
  hb_transform_t<> t = c->current_effective_transform ();
  عائم det = t.xx * t.yy - t.xy * t.yx;
  إذا (fabsf (det) < 1e-10f || fabsf (angle_range) < 1e-10f) أرجع;

  {
    عائم inv_det = 1.f / det;
    عائم inv_xx =  t.yy * inv_det;
    عائم inv_xy = -t.xy * inv_det;
    عائم inv_yx = -t.yx * inv_det;
    عائم inv_yy =  t.xx * inv_det;
    عائم inv_x0 = (t.xy * t.y0 - t.yy * t.x0) * inv_det;
    عائم inv_y0 = (t.yx * t.x0 - t.xx * t.y0) * inv_det;

    عائم inv_angle_range = 1.f / angle_range;

    بنانشانی stride = surf->extents.stride;
    صحيح ox = surf->extents.x_origin;
    صحيح oy = surf->extents.y_origin;

    إذا (clip.is_rect)
    {
      لكل (بنانشانی py = clip.min_y; py < clip.max_y; py++)
      {
	hb_packed_t<uint32_t> *صف = (hb_packed_t<uint32_t> *) (surf->بفر.arrayZ + py * stride);
	عائم gx = inv_xx * ((عائم) ((صحيح) clip.min_x + ox) + 0.5f) + inv_xy * ((عائم) ((صحيح) py + oy) + 0.5f) + inv_x0;
	عائم gy = inv_yx * ((عائم) ((صحيح) clip.min_x + ox) + 0.5f) + inv_yy * ((عائم) ((صحيح) py + oy) + 0.5f) + inv_y0;
	إذا (use_lut)
	{
	  لكل (بنانشانی px = clip.min_x; px < clip.max_x; px++)
	  {
	    عائم angle = atan2f (gy - cy, gx - cx);
	    إذا (angle < 0) angle += (عائم) HB_2_PI;
	    عائم grad_t = (angle - a0) * inv_angle_range;
	    uint32_t src = lookup_gradient_lut (lut, grad_t, extend);
	    صف[px] = hb_packed_t<uint32_t> (hb_raster_src_over (src, (uint32_t) صف[px]));
	    gx += inv_xx;
	    gy += inv_yx;
	  }
	}
	وإلا
	{
	  لكل (بنانشانی px = clip.min_x; px < clip.max_x; px++)
	  {
	    عائم angle = atan2f (gy - cy, gx - cx);
	    إذا (angle < 0) angle += (عائم) HB_2_PI;
	    عائم grad_t = (angle - a0) * inv_angle_range;
	    uint32_t src = evaluate_color_سطر (stops, len, grad_t, extend);
	    صف[px] = hb_packed_t<uint32_t> (hb_raster_src_over (src, (uint32_t) صف[px]));
	    gx += inv_xx;
	    gy += inv_yx;
	  }
	}
      }
    }
    وإلا
    {
      لكل (بنانشانی py = clip.min_y; py < clip.max_y; py++)
      {
	hb_packed_t<uint32_t> *صف = (hb_packed_t<uint32_t> *) (surf->بفر.arrayZ + py * stride);
	ثابت uint8_t *clip_صف = clip.alpha.arrayZ + py * clip.stride;
	عائم gx = inv_xx * ((عائم) ((صحيح) clip.min_x + ox) + 0.5f) + inv_xy * ((عائم) ((صحيح) py + oy) + 0.5f) + inv_x0;
	عائم gy = inv_yx * ((عائم) ((صحيح) clip.min_x + ox) + 0.5f) + inv_yy * ((عائم) ((صحيح) py + oy) + 0.5f) + inv_y0;
	إذا (use_lut)
	{
	  لكل (بنانشانی px = clip.min_x; px < clip.max_x; px++)
	  {
	    uint8_t clip_alpha = clip_صف[px];
	    إذا (clip_alpha == 0)
	    {
	      gx += inv_xx;
	      gy += inv_yx;
	      استمر;
	    }
	    عائم angle = atan2f (gy - cy, gx - cx);
	    إذا (angle < 0) angle += (عائم) HB_2_PI;
	    عائم grad_t = (angle - a0) * inv_angle_range;
	    uint32_t src = lookup_gradient_lut (lut, grad_t, extend);
	    src = hb_raster_alpha_mul (src, clip_alpha);
	    صف[px] = hb_packed_t<uint32_t> (hb_raster_src_over (src, (uint32_t) صف[px]));
	    gx += inv_xx;
	    gy += inv_yx;
	  }
	}
	وإلا
	{
	  لكل (بنانشانی px = clip.min_x; px < clip.max_x; px++)
	  {
	    uint8_t clip_alpha = clip_صف[px];
	    إذا (clip_alpha == 0)
	    {
	      gx += inv_xx;
	      gy += inv_yx;
	      استمر;
	    }
	    عائم angle = atan2f (gy - cy, gx - cx);
	    إذا (angle < 0) angle += (عائم) HB_2_PI;
	    عائم grad_t = (angle - a0) * inv_angle_range;
	    uint32_t src = evaluate_color_سطر (stops, len, grad_t, extend);
	    src = hb_raster_alpha_mul (src, clip_alpha);
	    صف[px] = hb_packed_t<uint32_t> (hb_raster_src_over (src, (uint32_t) صف[px]));
	    gx += inv_xx;
	    gy += inv_yx;
	  }
	}
      }
    }
  }

}

ساكن hb_bool_t
hb_raster_paint_custom_palette_color (hb_paint_funcs_t *funcs HB_بن_ورتیا,
				      فراغ *paint_انکڑے,
				      بنانشانی صحيح color_اشاریہ,
				      hb_color_t *color,
				      فراغ *user_انکڑے HB_بن_ورتیا)
{
  hb_raster_paint_t *c = (hb_raster_paint_t *) paint_انکڑے;
  إذا (likely (c->custom_palette && hb_map_has (c->custom_palette, color_اشاریہ)))
  {
    *color = hb_map_get (c->custom_palette, color_اشاریہ);
    أرجع سچ;
  }
  أرجع خطأ;
}

ساكن hb_bool_t
hb_raster_paint_سیٹ_budget (hb_paint_funcs_t *, فراغ *paint_انکڑے,
			    int64_t budget, فراغ *)
{
  خودکار *paint = (hb_raster_paint_t *) paint_انکڑے;
  paint->budget = budget;
  paint->recharge_budget ();
  أرجع سچ;
}

ساكن int64_t
hb_raster_paint_get_budget (hb_paint_funcs_t *, فراغ *paint_انکڑے, فراغ *)
{
  أرجع ((hb_raster_paint_t *) paint_انکڑے)->budget;
}

ساكن int64_t *
hb_raster_paint_get_budget_بچیا (hb_paint_funcs_t *, فراغ *paint_انکڑے, فراغ *)
{
  أرجع &((hb_raster_paint_t *) paint_انکڑے)->budget_بچیا;
}


/*
 * Lazy-loader singleton for paint funcs
 */

ساكن مضمن فراغ free_static_raster_paint_funcs ();

ساكن بنية hb_raster_paint_funcs_lazy_loader_t : hb_paint_funcs_lazy_loader_t<hb_raster_paint_funcs_lazy_loader_t>
{
  ساكن hb_paint_funcs_t *create ()
  {
    hb_paint_funcs_t *funcs = hb_paint_funcs_create ();

    hb_paint_funcs_سیٹ_ڈالو_transform_func (funcs, hb_raster_paint_ڈالو_transform, خالی_سوچک, خالی_سوچک);
    hb_paint_funcs_سیٹ_نکالو_transform_func (funcs, hb_raster_paint_نکالو_transform, خالی_سوچک, خالی_سوچک);
    hb_paint_funcs_سیٹ_fill_glyph_func (funcs, hb_raster_paint_fill_glyph, خالی_سوچک, خالی_سوچک);
    hb_paint_funcs_سیٹ_color_glyph_func (funcs, hb_raster_paint_color_glyph, خالی_سوچک, خالی_سوچک);
    hb_paint_funcs_سیٹ_ڈالو_clip_glyph_func (funcs, hb_raster_paint_ڈالو_clip_glyph, خالی_سوچک, خالی_سوچک);
    hb_paint_funcs_سیٹ_ڈالو_clip_rectangle_func (funcs, hb_raster_paint_ڈالو_clip_rectangle, خالی_سوچک, خالی_سوچک);
    hb_paint_funcs_سیٹ_ڈالو_clip_رستہ_شروع_func (funcs, hb_raster_paint_ڈالو_clip_رستہ_شروع, خالی_سوچک, خالی_سوچک);
    hb_paint_funcs_سیٹ_ڈالو_clip_رستہ_سرا_func (funcs, hb_raster_paint_ڈالو_clip_رستہ_سرا, خالی_سوچک, خالی_سوچک);
    hb_paint_funcs_سیٹ_نکالو_clip_func (funcs, hb_raster_paint_نکالو_clip, خالی_سوچک, خالی_سوچک);
    hb_paint_funcs_سیٹ_ڈالو_گروہ_func (funcs, hb_raster_paint_ڈالو_گروہ, خالی_سوچک, خالی_سوچک);
    hb_paint_funcs_سیٹ_نکالو_گروہ_func (funcs, hb_raster_paint_نکالو_گروہ, خالی_سوچک, خالی_سوچک);
    hb_paint_funcs_سیٹ_color_func (funcs, hb_raster_paint_color, خالی_سوچک, خالی_سوچک);
    hb_paint_funcs_سیٹ_image_func (funcs, hb_raster_paint_image, خالی_سوچک, خالی_سوچک);
    hb_paint_funcs_سیٹ_خطی_gradient_func (funcs, hb_raster_paint_خطی_gradient, خالی_سوچک, خالی_سوچک);
    hb_paint_funcs_سیٹ_radial_gradient_func (funcs, hb_raster_paint_radial_gradient, خالی_سوچک, خالی_سوچک);
    hb_paint_funcs_سیٹ_sweep_gradient_func (funcs, hb_raster_paint_sweep_gradient, خالی_سوچک, خالی_سوچک);
    hb_paint_funcs_سیٹ_custom_palette_color_func (funcs, hb_raster_paint_custom_palette_color, خالی_سوچک, خالی_سوچک);
    hb_paint_funcs_سیٹ_سیٹ_budget_func (funcs, hb_raster_paint_سیٹ_budget, خالی_سوچک, خالی_سوچک);
    hb_paint_funcs_سیٹ_get_budget_func (funcs, hb_raster_paint_get_budget, خالی_سوچک, خالی_سوچک);
    hb_paint_funcs_سیٹ_get_budget_بچیا_func (funcs, hb_raster_paint_get_budget_بچیا, خالی_سوچک, خالی_سوچک);

    hb_paint_funcs_make_immutable (funcs);

    hb_atexit (free_static_raster_paint_funcs);

    أرجع funcs;
  }
} static_raster_paint_funcs;

ساكن مضمن فراغ
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
hb_raster_paint_create_or_fail (فراغ)
{
  hb_raster_paint_t *paint = hb_object_create<hb_raster_paint_t> ();
  إذا (unlikely (!paint))
    أرجع خالی_سوچک;

  paint->clip_rdr = hb_raster_draw_create_or_fail ();
  إذا (unlikely (!paint->clip_rdr))
  {
    hb_free (paint);
    أرجع خالی_سوچک;
  }

  أرجع paint;
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
  أرجع hb_object_reference (paint);
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
فراغ
hb_raster_paint_destroy (hb_raster_paint_t *paint)
{
  إذا (!hb_object_should_destroy (paint))
    أرجع;

  hb_map_destroy (paint->custom_palette);
  hb_raster_draw_destroy (paint->clip_rdr);
  لكل (خودکار *s : paint->surface_ڈھیر)
    hb_raster_image_destroy (s);
  لكل (خودکار *s : paint->surface_cache)
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
hb_raster_paint_سیٹ_user_انکڑے (hb_raster_paint_t  *paint,
			       hb_user_انکڑے_کلید_t *key,
			       فراغ               *انکڑے,
			       hb_destroy_func_t   destroy,
			       hb_bool_t           replace)
{
  أرجع hb_object_سیٹ_user_انکڑے (paint, key, انکڑے, destroy, replace);
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
فراغ *
hb_raster_paint_get_user_انکڑے (ثابت hb_raster_paint_t  *paint,
			       hb_user_انکڑے_کلید_t *key)
{
  أرجع hb_object_get_user_انکڑے (paint, key);
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
فراغ
hb_raster_paint_سیٹ_transform (hb_raster_paint_t *paint,
			       عائم xx, عائم yx,
			       عائم xy, عائم yy,
			       عائم dx, عائم dy)
{
  paint->ادھار_transform = {xx, yx, xy, yy, dx, dy};
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
فراغ
hb_raster_paint_get_transform (ثابت hb_raster_paint_t *paint,
			       عائم *xx, عائم *yx,
			       عائم *xy, عائم *yy,
			       عائم *dx, عائم *dy)
{
  إذا (xx) *xx = paint->ادھار_transform.xx;
  إذا (yx) *yx = paint->ادھار_transform.yx;
  إذا (xy) *xy = paint->ادھار_transform.xy;
  إذا (yy) *yy = paint->ادھار_transform.yy;
  إذا (dx) *dx = paint->ادھار_transform.x0;
  إذا (dy) *dy = paint->ادھار_transform.y0;
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
فراغ
hb_raster_paint_سیٹ_scale_ضارب (hb_raster_paint_t *paint,
				  عائم x_scale_ضارب,
				  عائم y_scale_ضارب)
{
  paint->x_scale_ضارب = x_scale_ضارب > 0.f ? x_scale_ضارب : 1.f;
  paint->y_scale_ضارب = y_scale_ضارب > 0.f ? y_scale_ضارب : 1.f;
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
فراغ
hb_raster_paint_get_scale_ضارب (ثابت hb_raster_paint_t *paint,
				  عائم *x_scale_ضارب,
				  عائم *y_scale_ضارب)
{
  إذا (x_scale_ضارب) *x_scale_ضارب = paint->x_scale_ضارب;
  إذا (y_scale_ضارب) *y_scale_ضارب = paint->y_scale_ضارب;
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
فراغ
hb_raster_paint_سیٹ_extents (hb_raster_paint_t         *paint,
			     ثابت hb_raster_extents_t *extents)
{
  paint->fixed_extents = *extents;
  paint->has_extents = سچ;
  إذا (paint->fixed_extents.stride == 0)
    paint->fixed_extents.stride = paint->fixed_extents.چوڑائی * 4;
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
hb_raster_paint_get_extents (ثابت hb_raster_paint_t   *paint,
			     hb_raster_extents_t *extents)
{
  إذا (!paint->has_extents)
    أرجع خطأ;

  إذا (extents)
    *extents = paint->fixed_extents;
  أرجع سچ;
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
hb_raster_paint_سیٹ_glyph_extents (hb_raster_paint_t        *paint,
				   ثابت hb_glyph_extents_t *glyph_extents)
{
  عائم x0 = (عائم) glyph_extents->x_bearing;
  عائم y0 = (عائم) glyph_extents->y_bearing;
  عائم x1 = (عائم) glyph_extents->x_bearing + glyph_extents->چوڑائی;
  عائم y1 = (عائم) glyph_extents->y_bearing + glyph_extents->اچائی;

  عائم xmin = hb_min (x0, x1);
  عائم xmax = hb_max (x0, x1);
  عائم ymin = hb_min (y0, y1);
  عائم ymax = hb_max (y0, y1);

  عائم px[4] = {xmin, xmin, xmax, xmax};
  عائم py[4] = {ymin, ymax, ymin, ymax};

  hb_transform_t<> t = paint->ادھار_transform;
  paint->apply_scale_ضارب (t);

  عائم tx, ty;
  t.transform_نقطہ (px[0], py[0]);
  tx = px[0]; ty = py[0];
  عائم tx_min = tx, tx_max = tx;
  عائم ty_min = ty, ty_max = ty;

  لكل (بنانشانی i = 1; i < 4; i++)
  {
    t.transform_نقطہ (px[i], py[i]);
    tx_min = hb_min (tx_min, px[i]);
    tx_max = hb_max (tx_max, px[i]);
    ty_min = hb_min (ty_min, py[i]);
    ty_max = hb_max (ty_max, py[i]);
  }

  int32_t ex0 = hb_clamp_منزل<int32_t> (floorf (tx_min));
  int32_t ey0 = hb_clamp_منزل<int32_t> (floorf (ty_min));
  int32_t ex1 = hb_clamp_منزل<int32_t> (ceilf  (tx_max));
  int32_t ey1 = hb_clamp_منزل<int32_t> (ceilf  (ty_max));

  إذا (ex1 <= ex0 || ey1 <= ey0)
  {
    paint->fixed_extents = {};
    paint->has_extents = خطأ;
    أرجع خطأ;
  }

  paint->fixed_extents = {
    ex0, ey0,
    (بنانشانی) hb_min ((int64_t) ex1 - ex0, (int64_t) HB_RASTER_MAX_AUTO_DIMENSION),
    (بنانشانی) hb_min ((int64_t) ey1 - ey0, (int64_t) HB_RASTER_MAX_AUTO_DIMENSION),
    0
  };
  paint->has_extents = سچ;
  إذا (paint->fixed_extents.stride == 0)
    paint->fixed_extents.stride = paint->fixed_extents.چوڑائی * 4;
  أرجع سچ;
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
فراغ
hb_raster_paint_سیٹ_foreground (hb_raster_paint_t *paint,
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
hb_raster_paint_get_foreground (ثابت hb_raster_paint_t *paint)
{
  أرجع paint->foreground;
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
فراغ
hb_raster_paint_سیٹ_background (hb_raster_paint_t *paint,
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
hb_raster_paint_get_background (ثابت hb_raster_paint_t *paint)
{
  أرجع paint->background;
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
فراغ
hb_raster_paint_سیٹ_palette (hb_raster_paint_t *paint,
			     بنانشانی           palette)
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
بنانشانی
hb_raster_paint_get_palette (ثابت hb_raster_paint_t *paint)
{
  أرجع paint->palette;
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
فراغ
hb_raster_paint_clear_custom_palette_colors (hb_raster_paint_t *paint)
{
  إذا (paint->custom_palette)
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
hb_raster_paint_سیٹ_custom_palette_color (hb_raster_paint_t *paint,
					  بنانشانی صحيح       color_اشاریہ,
					  hb_color_t         color)
{
  إذا (unlikely (!paint->custom_palette))
  {
    paint->custom_palette = hb_map_create ();
    إذا (unlikely (!paint->custom_palette))
      أرجع خطأ;
  }
  hb_map_سیٹ (paint->custom_palette, color_اشاریہ, color);
  أرجع hb_map_ونڈ_successful (paint->custom_palette);
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
hb_raster_paint_get_funcs (ثابت hb_raster_paint_t *paint HB_بن_ورتیا)
{
  أرجع static_raster_paint_funcs.get_unconst ();
}

ساكن hb_bool_t
hb_raster_paint_glyph_impl (hb_raster_paint_t *paint,
			    hb_font_t        *font,
			    hb_codepoint_t    glyph,
			    hb_bool_t         fallible)
{
  إذا (!paint->has_extents)
  {
    hb_glyph_extents_t ge;
    إذا (hb_font_get_glyph_extents (font, glyph, &ge))
      hb_raster_paint_سیٹ_glyph_extents (paint, &ge);
  }

  hb_paint_funcs_t *funcs = hb_raster_paint_get_funcs (paint);
  hb_paint_ڈالو_transform (funcs, paint,
			   paint->ادھار_transform.xx, paint->ادھار_transform.yx,
			   paint->ادھار_transform.xy, paint->ادھار_transform.yy,
			   paint->ادھار_transform.x0, paint->ادھار_transform.y0);

  hb_bool_t ret = سچ;
  إذا (fallible)
    ret = hb_font_paint_glyph_or_fail (font, glyph,
				       funcs, paint,
				       paint->palette, paint->foreground);
  وإلا
    hb_font_paint_glyph (font, glyph,
			 funcs, paint,
			 paint->palette, paint->foreground);

  hb_paint_نکالو_transform (funcs, paint);
  أرجع ret;
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
  أرجع hb_raster_paint_glyph_impl (paint, font, glyph, سچ);
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
فراغ
hb_raster_paint_glyph (hb_raster_paint_t *paint,
		       hb_font_t        *font,
		       hb_codepoint_t    glyph)
{
  hb_raster_paint_glyph_impl (paint, font, glyph, خطأ);
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

  إذا (unlikely (!paint->has_extents))
    أرجع خالی_سوچک;

  hb_raster_image_t *نتیجہ;
  إذا (paint->surface_ڈھیر.لمائی)
  {
    نتیجہ = paint->surface_ڈھیر[0];
    /* Release any remaining group surfaces (shouldn't happen with
     * well-formed paint calls, but be safe). */
    لكل (بنانشانی i = 1; i < paint->surface_ڈھیر.لمائی; i++)
      paint->release_surface (paint->surface_ڈھیر[i]);
    paint->surface_ڈھیر.clear ();
  }
  وإلا
  {
    نتیجہ = paint->acquire_surface ();
    إذا (unlikely (!نتیجہ))
      أرجع خالی_سوچک;
  }

  أرجع نتیجہ;
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
فراغ
hb_raster_paint_clear (hb_raster_paint_t *paint)
{
  paint->fixed_extents = {};
  paint->has_extents = خطأ;
  paint->transform_ڈھیر.clear ();
  paint->release_all_clips ();
  لكل (خودکار *s : paint->surface_ڈھیر)
    paint->release_surface (s);
  paint->surface_ڈھیر.clear ();
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
فراغ
hb_raster_paint_reset (hb_raster_paint_t *paint)
{
  paint->ادھار_transform = {1, 0, 0, 1, 0, 0};
  paint->x_scale_ضارب = 1.f;
  paint->y_scale_ضارب = 1.f;
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
فراغ
hb_raster_paint_recycle_image (hb_raster_paint_t  *paint,
			       hb_raster_image_t  *image)
{
  paint->release_surface (image);
}
