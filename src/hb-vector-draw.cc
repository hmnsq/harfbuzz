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

#ضمن "hb-vector-draw.hh"
#ضمن "hb-vector-path.hh"

#ضمن <math.h>
#ضمن <string.h>

HB_اڻ_استعمال_ٿيل ساكن مضمن منطقي
hb_ويڪٽر_svg_بفر_شامل_آهي (ثابت hb_ويڪٽر_buf_t &buf, ثابت حرف *needle)
{
  بنانشان nlen = (بنانشان) لڙي_ڊيگهه (needle);
  إذا (!nlen || buf.ڊيگهائي < nlen)
    أرجع خطأ;

  لكل (بنانشان i = 0; i + nlen <= buf.ڊيگهائي; i++)
    إذا (buf.arrayZ[i] == needle[0] &&
        !memcmp (buf.arrayZ + i, needle, nlen))
      أرجع سچ;
  أرجع خطأ;
}

/* ---- SVG draw callbacks ---- */

ساكن فراغ hb_ويڪٽر_draw_svg_move_منزل (hb_draw_funcs_t *, فراغ *dd, hb_draw_state_t *, عائم x, عائم y, فراغ *)
{
  خودڪار *d = (hb_ويڪٽر_draw_t *) dd;
  إذا (unlikely (!d->begin_رستو_command ())) أرجع;
  d->رستو.append_c ('M'); d->append_xy_svg (x, y);
  d->پڇاڙي_رستو_command ();
}

ساكن فراغ hb_ويڪٽر_draw_svg_سٽ_منزل (hb_draw_funcs_t *, فراغ *dd, hb_draw_state_t *, عائم x, عائم y, فراغ *)
{
  خودڪار *d = (hb_ويڪٽر_draw_t *) dd;
  إذا (unlikely (!d->begin_رستو_command ())) أرجع;
  d->رستو.append_c ('L'); d->append_xy_svg (x, y);
  d->پڇاڙي_رستو_command ();
}

ساكن فراغ hb_ويڪٽر_draw_svg_quadratic_منزل (hb_draw_funcs_t *, فراغ *dd, hb_draw_state_t *, عائم cx, عائم cy, عائم x, عائم y, فراغ *)
{
  خودڪار *d = (hb_ويڪٽر_draw_t *) dd;
  إذا (unlikely (!d->begin_رستو_command ())) أرجع;
  d->رستو.append_c ('Q'); d->append_xy_svg (cx, cy); d->رستو.append_c (' '); d->append_xy_svg (x, y);
  d->پڇاڙي_رستو_command ();
}

ساكن فراغ hb_ويڪٽر_draw_svg_cubic_منزل (hb_draw_funcs_t *, فراغ *dd, hb_draw_state_t *, عائم c1x, عائم c1y, عائم c2x, عائم c2y, عائم x, عائم y, فراغ *)
{
  خودڪار *d = (hb_ويڪٽر_draw_t *) dd;
  إذا (unlikely (!d->begin_رستو_command ())) أرجع;
  d->رستو.append_c ('C'); d->append_xy_svg (c1x, c1y); d->رستو.append_c (' '); d->append_xy_svg (c2x, c2y); d->رستو.append_c (' '); d->append_xy_svg (x, y);
  d->پڇاڙي_رستو_command ();
}

ساكن فراغ hb_ويڪٽر_draw_svg_close_رستو (hb_draw_funcs_t *, فراغ *dd, hb_draw_state_t *, فراغ *)
{
  خودڪار *d = (hb_ويڪٽر_draw_t *) dd;
  إذا (unlikely (!d->begin_رستو_command ())) أرجع;
  d->رستو.append_c ('Z');
  d->پڇاڙي_رستو_command ();
}


/* ---- PDF draw callbacks ---- */

ساكن فراغ hb_ويڪٽر_draw_pdf_move_منزل (hb_draw_funcs_t *, فراغ *dd, hb_draw_state_t *, عائم x, عائم y, فراغ *)
{
  خودڪار *d = (hb_ويڪٽر_draw_t *) dd;
  إذا (unlikely (!d->begin_رستو_command ())) أرجع;
  d->append_xy_pdf (x, y); d->رستو.append_str (" m\n");
  d->پڇاڙي_رستو_command ();
}

ساكن فراغ hb_ويڪٽر_draw_pdf_سٽ_منزل (hb_draw_funcs_t *, فراغ *dd, hb_draw_state_t *, عائم x, عائم y, فراغ *)
{
  خودڪار *d = (hb_ويڪٽر_draw_t *) dd;
  إذا (unlikely (!d->begin_رستو_command ())) أرجع;
  d->append_xy_pdf (x, y); d->رستو.append_str (" l\n");
  d->پڇاڙي_رستو_command ();
}

/* No quadratic_to — null fallback auto-promotes to cubic. */

ساكن فراغ hb_ويڪٽر_draw_pdf_cubic_منزل (hb_draw_funcs_t *, فراغ *dd, hb_draw_state_t *, عائم c1x, عائم c1y, عائم c2x, عائم c2y, عائم x, عائم y, فراغ *)
{
  خودڪار *d = (hb_ويڪٽر_draw_t *) dd;
  إذا (unlikely (!d->begin_رستو_command ())) أرجع;
  d->append_xy_pdf (c1x, c1y); d->رستو.append_c (' '); d->append_xy_pdf (c2x, c2y); d->رستو.append_c (' '); d->append_xy_pdf (x, y); d->رستو.append_str (" c\n");
  d->پڇاڙي_رستو_command ();
}

ساكن فراغ hb_ويڪٽر_draw_pdf_close_رستو (hb_draw_funcs_t *, فراغ *dd, hb_draw_state_t *, فراغ *)
{
  خودڪار *d = (hb_ويڪٽر_draw_t *) dd;
  إذا (unlikely (!d->begin_رستو_command ())) أرجع;
  d->رستو.append_str ("h\n");
  d->پڇاڙي_رستو_command ();
}

ساكن hb_bool_t
hb_ويڪٽر_draw_سيٽ_budget (hb_draw_funcs_t *, فراغ *draw_معلومات,
			   int64_t budget, فراغ *)
{
  خودڪار *draw = (hb_ويڪٽر_draw_t *) draw_معلومات;
  draw->budget = budget;
  draw->recharge_budget ();
  أرجع سچ;
}

ساكن int64_t
hb_ويڪٽر_draw_get_budget (hb_draw_funcs_t *, فراغ *draw_معلومات, فراغ *)
{
  أرجع ((hb_ويڪٽر_draw_t *) draw_معلومات)->budget;
}

ساكن int64_t *
hb_ويڪٽر_draw_get_budget_بچيل (hb_draw_funcs_t *, فراغ *draw_معلومات, فراغ *)
{
  أرجع &((hb_ويڪٽر_draw_t *) draw_معلومات)->budget_بچيل;
}


/* ---- Lazy loaders ---- */

ساكن مضمن فراغ free_static_ويڪٽر_draw_svg_funcs ();
ساكن بنية hb_ويڪٽر_draw_svg_funcs_lazy_loader_t
  : hb_draw_funcs_lazy_loader_t<hb_ويڪٽر_draw_svg_funcs_lazy_loader_t>
{
  ساكن hb_draw_funcs_t *create ()
  {
    hb_draw_funcs_t *funcs = hb_draw_funcs_create ();
    hb_draw_funcs_سيٽ_move_منزل_func (funcs, (hb_draw_move_منزل_func_t) hb_ويڪٽر_draw_svg_move_منزل, خالي_اشارو, خالي_اشارو);
    hb_draw_funcs_سيٽ_سٽ_منزل_func (funcs, (hb_draw_سٽ_منزل_func_t) hb_ويڪٽر_draw_svg_سٽ_منزل, خالي_اشارو, خالي_اشارو);
    hb_draw_funcs_سيٽ_quadratic_منزل_func (funcs, (hb_draw_quadratic_منزل_func_t) hb_ويڪٽر_draw_svg_quadratic_منزل, خالي_اشارو, خالي_اشارو);
    hb_draw_funcs_سيٽ_cubic_منزل_func (funcs, (hb_draw_cubic_منزل_func_t) hb_ويڪٽر_draw_svg_cubic_منزل, خالي_اشارو, خالي_اشارو);
    hb_draw_funcs_سيٽ_close_رستو_func (funcs, (hb_draw_close_رستو_func_t) hb_ويڪٽر_draw_svg_close_رستو, خالي_اشارو, خالي_اشارو);
    hb_draw_funcs_سيٽ_سيٽ_budget_func (funcs, hb_ويڪٽر_draw_سيٽ_budget, خالي_اشارو, خالي_اشارو);
    hb_draw_funcs_سيٽ_get_budget_func (funcs, hb_ويڪٽر_draw_get_budget, خالي_اشارو, خالي_اشارو);
    hb_draw_funcs_سيٽ_get_budget_بچيل_func (funcs, hb_ويڪٽر_draw_get_budget_بچيل, خالي_اشارو, خالي_اشارو);
    hb_draw_funcs_make_immutable (funcs);
    hb_atexit (free_static_ويڪٽر_draw_svg_funcs);
    أرجع funcs;
  }
} static_ويڪٽر_draw_svg_funcs;
ساكن مضمن فراغ free_static_ويڪٽر_draw_svg_funcs () { static_ويڪٽر_draw_svg_funcs.free_instance (); }

ساكن مضمن فراغ free_static_ويڪٽر_draw_pdf_funcs ();
ساكن بنية hb_ويڪٽر_draw_pdf_funcs_lazy_loader_t
  : hb_draw_funcs_lazy_loader_t<hb_ويڪٽر_draw_pdf_funcs_lazy_loader_t>
{
  ساكن hb_draw_funcs_t *create ()
  {
    hb_draw_funcs_t *funcs = hb_draw_funcs_create ();
    hb_draw_funcs_سيٽ_move_منزل_func (funcs, (hb_draw_move_منزل_func_t) hb_ويڪٽر_draw_pdf_move_منزل, خالي_اشارو, خالي_اشارو);
    hb_draw_funcs_سيٽ_سٽ_منزل_func (funcs, (hb_draw_سٽ_منزل_func_t) hb_ويڪٽر_draw_pdf_سٽ_منزل, خالي_اشارو, خالي_اشارو);
    /* No quadratic_to: null fallback auto-promotes to cubic. */
    hb_draw_funcs_سيٽ_cubic_منزل_func (funcs, (hb_draw_cubic_منزل_func_t) hb_ويڪٽر_draw_pdf_cubic_منزل, خالي_اشارو, خالي_اشارو);
    hb_draw_funcs_سيٽ_close_رستو_func (funcs, (hb_draw_close_رستو_func_t) hb_ويڪٽر_draw_pdf_close_رستو, خالي_اشارو, خالي_اشارو);
    hb_draw_funcs_سيٽ_سيٽ_budget_func (funcs, hb_ويڪٽر_draw_سيٽ_budget, خالي_اشارو, خالي_اشارو);
    hb_draw_funcs_سيٽ_get_budget_func (funcs, hb_ويڪٽر_draw_get_budget, خالي_اشارو, خالي_اشارو);
    hb_draw_funcs_سيٽ_get_budget_بچيل_func (funcs, hb_ويڪٽر_draw_get_budget_بچيل, خالي_اشارو, خالي_اشارو);
    hb_draw_funcs_make_immutable (funcs);
    hb_atexit (free_static_ويڪٽر_draw_pdf_funcs);
    أرجع funcs;
  }
} static_ويڪٽر_draw_pdf_funcs;
ساكن مضمن فراغ free_static_ويڪٽر_draw_pdf_funcs () { static_ويڪٽر_draw_pdf_funcs.free_instance (); }



/**
 * hb_vector_draw_create_or_fail:
 * @format: output format.
 *
 * Creates a new draw context for vector output.
 *
 * Return value: (nullable): a newly allocated #hb_vector_draw_t, or `NULL` on failure.
 *
 * Since: 13.0.0
 */
hb_ويڪٽر_draw_t *
hb_ويڪٽر_draw_create_or_fail (hb_ويڪٽر_format_t format)
{
  بدل (format)
  {
    حالة HB_ويڪٽر_FORMAT_SVG:
    حالة HB_ويڪٽر_FORMAT_PDF:
      اكسر;
    حالة HB_ويڪٽر_FORMAT_INVALID: افتراضي:
      أرجع خالي_اشارو;
  }

  hb_ويڪٽر_draw_t *draw = hb_object_create<hb_ويڪٽر_draw_t> ();
  إذا (unlikely (!draw))
    أرجع خالي_اشارو;
  draw->format = format;
  draw->defs.alloc (2048);
  draw->body.alloc (8192);
  draw->رستو.alloc (2048);
  أرجع draw;
}

/**
 * hb_vector_draw_reference:
 * @draw: a draw context.
 *
 * Increases the reference count of @draw.
 *
 * Return value: (transfer full): referenced @draw.
 *
 * Since: 13.0.0
 */
hb_ويڪٽر_draw_t *
hb_ويڪٽر_draw_reference (hb_ويڪٽر_draw_t *draw)
{
  أرجع hb_object_reference (draw);
}

/**
 * hb_vector_draw_destroy:
 * @draw: a draw context.
 *
 * Decreases the reference count of @draw and destroys it when it reaches zero.
 *
 * Since: 13.0.0
 */
فراغ
hb_ويڪٽر_draw_destroy (hb_ويڪٽر_draw_t *draw)
{
  إذا (!hb_object_should_destroy (draw))
    أرجع;

  hb_blob_destroy (draw->recycled_blob);
  hb_object_actually_destroy (draw);
  hb_free (draw);
}

/**
 * hb_vector_draw_set_user_data:
 * @draw: a draw context.
 * @key: user-data key.
 * @data: user-data value.
 * @destroy: (nullable): destroy callback for @data.
 * @replace: whether to replace an existing value for @key.
 *
 * Attaches user data to @draw.
 *
 * Return value: `true` on success, `false` otherwise.
 *
 * Since: 13.0.0
 */
hb_bool_t
hb_ويڪٽر_draw_سيٽ_user_معلومات (hb_ويڪٽر_draw_t   *draw,
                              hb_user_معلومات_ڪنجي_t *key,
                              فراغ               *معلومات,
                              hb_destroy_func_t   destroy,
                              hb_bool_t           replace)
{
  أرجع hb_object_سيٽ_user_معلومات (draw, key, معلومات, destroy, replace);
}

/**
 * hb_vector_draw_get_user_data:
 * @draw: a draw context.
 * @key: user-data key.
 *
 * Gets previously attached user data from @draw.
 *
 * Return value: (nullable): user-data value associated with @key.
 *
 * Since: 13.0.0
 */
فراغ *
hb_ويڪٽر_draw_get_user_معلومات (ثابت hb_ويڪٽر_draw_t   *draw,
                              hb_user_معلومات_ڪنجي_t *key)
{
  أرجع hb_object_get_user_معلومات (draw, key);
}

/**
 * hb_vector_draw_set_transform:
 * @draw: a draw context.
 * @xx: transform xx component.
 * @yx: transform yx component.
 * @xy: transform xy component.
 * @yy: transform yy component.
 * @dx: transform x translation.
 * @dy: transform y translation.
 *
 * Sets the affine transform used when drawing glyphs.
 *
 * Since: 13.0.0
 */
فراغ
hb_ويڪٽر_draw_سيٽ_transform (hb_ويڪٽر_draw_t *draw,
                              عائم xx, عائم yx,
                              عائم xy, عائم yy,
                              عائم dx, عائم dy)
{
  draw->transform = {xx, yx, xy, yy, dx, dy};
}

/**
 * hb_vector_draw_get_transform:
 * @draw: a draw context.
 * @xx: (out) (nullable): transform xx component.
 * @yx: (out) (nullable): transform yx component.
 * @xy: (out) (nullable): transform xy component.
 * @yy: (out) (nullable): transform yy component.
 * @dx: (out) (nullable): transform x translation.
 * @dy: (out) (nullable): transform y translation.
 *
 * Gets the affine transform used when drawing glyphs.
 *
 * Since: 13.0.0
 */
فراغ
hb_ويڪٽر_draw_get_transform (ثابت hb_ويڪٽر_draw_t *draw,
                              عائم *xx, عائم *yx,
                              عائم *xy, عائم *yy,
                              عائم *dx, عائم *dy)
{
  إذا (xx) *xx = draw->transform.xx;
  إذا (yx) *yx = draw->transform.yx;
  إذا (xy) *xy = draw->transform.xy;
  إذا (yy) *yy = draw->transform.yy;
  إذا (dx) *dx = draw->transform.x0;
  إذا (dy) *dy = draw->transform.y0;
}

/**
 * hb_vector_draw_set_scale_factor:
 * @draw: a draw context.
 * @x_scale_factor: x scale factor.
 * @y_scale_factor: y scale factor.
 *
 * Sets additional output scaling factors.
 *
 * Since: 13.0.0
 */
فراغ
hb_ويڪٽر_draw_سيٽ_scale_ضرب_ڪندڙ (hb_ويڪٽر_draw_t *draw,
                                 عائم x_scale_ضرب_ڪندڙ,
                                 عائم y_scale_ضرب_ڪندڙ)
{
  draw->x_scale_ضرب_ڪندڙ = x_scale_ضرب_ڪندڙ > 0.f ? x_scale_ضرب_ڪندڙ : 1.f;
  draw->y_scale_ضرب_ڪندڙ = y_scale_ضرب_ڪندڙ > 0.f ? y_scale_ضرب_ڪندڙ : 1.f;
}

/**
 * hb_vector_draw_get_scale_factor:
 * @draw: a draw context.
 * @x_scale_factor: (out) (nullable): x scale factor.
 * @y_scale_factor: (out) (nullable): y scale factor.
 *
 * Gets additional output scaling factors.
 *
 * Since: 13.0.0
 */
فراغ
hb_ويڪٽر_draw_get_scale_ضرب_ڪندڙ (ثابت hb_ويڪٽر_draw_t *draw,
                                 عائم *x_scale_ضرب_ڪندڙ,
                                 عائم *y_scale_ضرب_ڪندڙ)
{
  إذا (x_scale_ضرب_ڪندڙ) *x_scale_ضرب_ڪندڙ = draw->x_scale_ضرب_ڪندڙ;
  إذا (y_scale_ضرب_ڪندڙ) *y_scale_ضرب_ڪندڙ = draw->y_scale_ضرب_ڪندڙ;
}

/**
 * hb_vector_draw_set_extents:
 * @draw: a draw context.
 * @extents: (nullable): output extents to set or expand.
 *
 * Sets or expands output extents on @draw. Passing `NULL` clears extents.
 *
 * Since: 13.0.0
 */
فراغ
hb_ويڪٽر_draw_سيٽ_extents (hb_ويڪٽر_draw_t *draw,
                            ثابت hb_ويڪٽر_extents_t *extents)
{
  إذا (!extents)
  {
    draw->extents = {0, 0, 0, 0};
    draw->has_extents = خطأ;
    أرجع;
  }

  إذا (extents->ويڪر == 0.f || extents->اوچائي == 0.f)
    أرجع;

  /* Caller-supplied extents are in input-space; divide by
   * scale_factor so they end up in output-space, matching
   * the per-glyph extents accumulated via
   * hb_vector_set_glyph_extents_common (which applies the
   * same divide).  Normalize so origin is the min corner and
   * width/height are positive — callers may pass a
   * glyph-extents-style box with negative height. */
  عائم x0 = extents->x / draw->x_scale_ضرب_ڪندڙ;
  عائم y0 = extents->y / draw->y_scale_ضرب_ڪندڙ;
  عائم x1 = x0 + extents->ويڪر  / draw->x_scale_ضرب_ڪندڙ;
  عائم y1 = y0 + extents->اوچائي / draw->y_scale_ضرب_ڪندڙ;
  hb_ويڪٽر_extents_t e = {
    hb_min (x0, x1), hb_min (y0, y1),
    fabsf (x1 - x0), fabsf (y1 - y0),
  };

  إذا (draw->has_extents)
  {
    عائم x0 = hb_min (draw->extents.x, e.x);
    عائم y0 = hb_min (draw->extents.y, e.y);
    عائم x1 = hb_max (draw->extents.x + draw->extents.ويڪر,
                       e.x + e.ويڪر);
    عائم y1 = hb_max (draw->extents.y + draw->extents.اوچائي,
                       e.y + e.اوچائي);
    draw->extents = {x0, y0, x1 - x0, y1 - y0};
  }
  وإلا
  {
    draw->extents = e;
    draw->has_extents = سچ;
  }
}

/**
 * hb_vector_draw_get_extents:
 * @draw: a draw context.
 * @extents: (out) (nullable): where to store current output extents.
 *
 * Gets current output extents from @draw.
 *
 * Return value: `true` if extents are set, `false` otherwise.
 *
 * Since: 13.0.0
 */
hb_bool_t
hb_ويڪٽر_draw_get_extents (ثابت hb_ويڪٽر_draw_t *draw,
                            hb_ويڪٽر_extents_t *extents)
{
  إذا (!draw->has_extents)
    أرجع خطأ;

  إذا (extents)
    *extents = draw->extents;
  أرجع سچ;
}

/**
 * hb_vector_draw_set_glyph_extents:
 * @draw: a draw context.
 * @glyph_extents: glyph extents in font units.
 *
 * Expands @draw extents using @glyph_extents under the current transform.
 *
 * Return value: `true` on success, `false` otherwise.
 *
 * Since: 13.0.0
 */
hb_bool_t
hb_ويڪٽر_draw_سيٽ_glyph_extents (hb_ويڪٽر_draw_t *draw,
                                  ثابت hb_glyph_extents_t *glyph_extents)
{
  hb_bool_t has_extents = draw->has_extents;
  hb_bool_t ret = hb_ويڪٽر_سيٽ_glyph_extents_common (draw->transform,
						   draw->x_scale_ضرب_ڪندڙ,
						   draw->y_scale_ضرب_ڪندڙ,
						   glyph_extents,
						   &draw->extents,
						   &has_extents);
  draw->has_extents = has_extents;
  أرجع ret;
}

/**
 * hb_vector_draw_get_format:
 * @draw: a vector draw context.
 *
 * Gets the output format @draw was created with.
 *
 * Return value: the output format.
 *
 * Since: 14.2.0
 */
hb_ويڪٽر_format_t
hb_ويڪٽر_draw_get_format (ثابت hb_ويڪٽر_draw_t *draw)
{
  أرجع draw->format;
}

/**
 * hb_vector_draw_get_funcs:
 * @draw: a vector draw context.
 *
 * Gets draw callbacks for feeding outline data into @draw.
 * Pass @draw as the @draw_data argument when calling them.
 *
 * Return value: (transfer none): immutable #hb_draw_funcs_t.
 *
 * Since: 14.2.0
 */
hb_draw_funcs_t *
hb_ويڪٽر_draw_get_funcs (ثابت hb_ويڪٽر_draw_t *draw)
{
  بدل (draw ? draw->format : HB_ويڪٽر_FORMAT_INVALID)
  {
    حالة HB_ويڪٽر_FORMAT_SVG: أرجع static_ويڪٽر_draw_svg_funcs.get_unconst ();
    حالة HB_ويڪٽر_FORMAT_PDF: أرجع static_ويڪٽر_draw_pdf_funcs.get_unconst ();
    حالة HB_ويڪٽر_FORMAT_INVALID: افتراضي: أرجع خالي_اشارو;
  }
}

/**
 * hb_vector_draw_new_path:
 * @draw: a draw context.
 *
 * Flushes any pending path and starts a new one.  Call this
 * between glyphs to separate their outlines so fill rules
 * don't interact across glyphs.
 *
 * Since: 14.2.0
 */
فراغ
hb_ويڪٽر_draw_new_رستو (hb_ويڪٽر_draw_t *draw)
{
  draw->new_رستو ();
}

/**
 * hb_vector_draw_glyph_or_fail:
 * @draw: a draw context.
 * @font: font object.
 * @glyph: glyph ID.
 * @extents_mode: extents update mode.
 *
 * Convenience to draw one glyph into @draw.  Equivalent to:
 *
 * |[<!-- language="plain" -->
 * // extend extents if requested
 * hb_vector_draw_new_path (draw);
 * hb_font_draw_glyph_or_fail (font, glyph,
 *   hb_vector_draw_get_funcs (draw), draw);
 * ]|
 *
 * Return value: `true` if glyph data was emitted, `false` otherwise.
 *
 * Since: 14.2.0
 */
hb_bool_t
hb_ويڪٽر_draw_glyph_or_fail (hb_ويڪٽر_draw_t *draw,
                      hb_font_t *font,
                      hb_codepoint_t glyph,
                      hb_ويڪٽر_extents_گهڻائي_t extents_گهڻائي)
{

  إذا (extents_گهڻائي == HB_ويڪٽر_EXTENTS_گهڻائي_EXPAND)
  {
    hb_glyph_extents_t ge;
    إذا (hb_font_get_glyph_extents (font, glyph, &ge))
    {
      hb_bool_t has_extents = draw->has_extents;
      hb_ويڪٽر_سيٽ_glyph_extents_common (draw->transform,
					  draw->x_scale_ضرب_ڪندڙ,
					  draw->y_scale_ضرب_ڪندڙ,
					  &ge,
					  &draw->extents,
					  &has_extents);
      draw->has_extents = has_extents;
    }
  }

  draw->new_رستو ();
  أرجع hb_font_draw_glyph_or_fail (font, glyph, hb_ويڪٽر_draw_get_funcs (draw), draw);
}

/**
 * hb_vector_draw_glyph:
 * @draw: a draw context.
 * @font: font object.
 * @glyph: glyph ID.
 * @extents_mode: extents update mode.
 *
 * Draws one glyph into @draw.  Equivalent to
 * hb_vector_draw_glyph_or_fail() with the return value ignored.
 *
 * Since: 14.2.0
 */
فراغ
hb_ويڪٽر_draw_glyph (hb_ويڪٽر_draw_t *draw,
                      hb_font_t *font,
                      hb_codepoint_t glyph,
                      hb_ويڪٽر_extents_گهڻائي_t extents_گهڻائي)
{
  hb_ويڪٽر_draw_glyph_or_fail (draw, font, glyph, extents_گهڻائي);
}


/**
 * hb_vector_draw_set_precision:
 * @draw: a draw context.
 * @precision: decimal precision.
 *
 * Sets numeric output precision for draw output.
 *
 * Since: 14.2.0
 */
فراغ
hb_ويڪٽر_draw_سيٽ_precision (hb_ويڪٽر_draw_t *draw,
                             بنانشان precision)
{
  draw->سيٽ_precision (precision);
}

/**
 * hb_vector_draw_get_precision:
 * @draw: a draw context.
 *
 * Returns the numeric output precision previously set on @draw,
 * or the default if none was set.
 *
 * Return value: the precision.
 *
 * Since: 14.2.0
 */
بنانشان
hb_ويڪٽر_draw_get_precision (ثابت hb_ويڪٽر_draw_t *draw)
{
  أرجع draw->get_precision ();
}

/**
 * hb_vector_draw_set_foreground:
 * @draw: a draw context.
 * @foreground: foreground fill color.
 *
 * Sets the fill color for drawn glyph outlines.
 * Default is opaque black.
 *
 * Since: 14.2.0
 */
فراغ
hb_ويڪٽر_draw_سيٽ_foreground (hb_ويڪٽر_draw_t *draw,
                               hb_color_t foreground)
{
  draw->flush_رستو ();
  draw->foreground = foreground;
}

/**
 * hb_vector_draw_get_foreground:
 * @draw: a draw context.
 *
 * Returns the foreground fill color.
 *
 * Return value: the foreground color.
 *
 * Since: 14.2.0
 */
hb_color_t
hb_ويڪٽر_draw_get_foreground (ثابت hb_ويڪٽر_draw_t *draw)
{
  أرجع draw->foreground;
}

/**
 * hb_vector_draw_set_background:
 * @draw: a draw context.
 * @background: background color.
 *
 * Sets the background color.  If non-transparent, a filled
 * rectangle covering the extents is emitted behind all content.
 * Default is transparent (no background).
 *
 * Since: 14.2.0
 */
فراغ
hb_ويڪٽر_draw_سيٽ_background (hb_ويڪٽر_draw_t *draw,
                               hb_color_t background)
{
  draw->background = background;
}

/**
 * hb_vector_draw_get_background:
 * @draw: a draw context.
 *
 * Returns the background color.
 *
 * Return value: the background color.
 *
 * Since: 14.2.0
 */
hb_color_t
hb_ويڪٽر_draw_get_background (ثابت hb_ويڪٽر_draw_t *draw)
{
  أرجع draw->background;
}

ساكن hb_blob_t *
hb_ويڪٽر_draw_render_pdf (hb_ويڪٽر_draw_t *draw)
{
  draw->flush_رستو ();
  إذا (!draw->has_extents)
    أرجع خالي_اشارو;

  /* Collect the content stream.  The path coordinates are in
   * SVG space (Y-down).  Prepend a CTM that flips Y so the
   * PDF page (Y-up) renders correctly. */
  عائم ex = draw->extents.x;
  عائم ey = draw->extents.y;
  عائم ew = draw->extents.ويڪر;
  عائم eh = draw->extents.اوچائي;

  hb_ويڪٽر_buf_t وهڪرو;
  وهڪرو.alloc (draw->body.ڊيگهائي + draw->رستو.ڊيگهائي + 256);

  /* Background rect. */
  إذا (hb_color_get_alpha (draw->background))
  {
    عائم a = hb_color_get_alpha (draw->background) / 255.f;
    إذا (a < 1.f - 1.f / 512.f)
    {
      وهڪرو.append_انگ (a, 4);
      وهڪرو.append_str (" ca gs\n");
    }
    وهڪرو.append_انگ (hb_color_get_red (draw->background) / 255.f, 4);
    وهڪرو.append_c (' ');
    وهڪرو.append_انگ (hb_color_get_green (draw->background) / 255.f, 4);
    وهڪرو.append_c (' ');
    وهڪرو.append_انگ (hb_color_get_blue (draw->background) / 255.f, 4);
    وهڪرو.append_str (" rg\n");
    وهڪرو.append_انگ (ex);
    وهڪرو.append_c (' ');
    وهڪرو.append_انگ (-(ey + eh));
    وهڪرو.append_c (' ');
    وهڪرو.append_انگ (ew);
    وهڪرو.append_c (' ');
    وهڪرو.append_انگ (eh);
    وهڪرو.append_str (" re f\n");
  }

  draw->flush_رستو ();
  إذا (draw->body.ڊيگهائي)
    وهڪرو.append_len (draw->body.arrayZ, draw->body.ڊيگهائي);

  /* Build PDF objects, tracking byte offsets for xref. */
  hb_ويڪٽر_buf_t ٻاهريون;
  hb_buf_recover_recycled (draw->recycled_blob, &ٻاهريون);
  ٻاهريون.alloc (وهڪرو.ڊيگهائي + 512);

  بنانشان offsets[5]; /* objects 1-4, plus end */

  ٻاهريون.append_str ("%PDF-1.4\n%\xC0\xC1\xC2\xC3\n");

  /* Object 1: Catalog */
  offsets[0] = ٻاهريون.ڊيگهائي;
  ٻاهريون.append_str ("1 0 obj\n<< /Type /Catalog /Pages 2 0 R >>\nendobj\n");

  /* Object 2: Pages */
  offsets[1] = ٻاهريون.ڊيگهائي;
  ٻاهريون.append_str ("2 0 obj\n<< /Type /Pages /Kids [3 0 R] /Count 1 >>\nendobj\n");

  /* Object 3: Page.  Extents are in SVG space (y = -font_y).
   * Convert back: font Y range = [-(ey+eh) .. -ey]. */
  offsets[2] = ٻاهريون.ڊيگهائي;
  ٻاهريون.append_str ("3 0 obj\n<< /Type /Page /Parent 2 0 R /MediaBox [");
  ٻاهريون.append_انگ (ex);
  ٻاهريون.append_c (' ');
  ٻاهريون.append_انگ (-(ey + eh));
  ٻاهريون.append_c (' ');
  ٻاهريون.append_انگ (ex + ew);
  ٻاهريون.append_c (' ');
  ٻاهريون.append_انگ (-ey);
  ٻاهريون.append_str ("] /Contents 4 0 R");
  إذا (draw->pdf_extgstate_dict.ڊيگهائي)
  {
    ٻاهريون.append_str (" /Resources << /ExtGState << ");
    ٻاهريون.append_len (draw->pdf_extgstate_dict.arrayZ, draw->pdf_extgstate_dict.ڊيگهائي);
    ٻاهريون.append_str (">> >>");
  }
  ٻاهريون.append_str (" >>\nendobj\n");

  /* Object 4: Content stream */
  offsets[3] = ٻاهريون.ڊيگهائي;
  ٻاهريون.append_str ("4 0 obj\n<< /Length ");
  ٻاهريون.append_unsigned (وهڪرو.ڊيگهائي);
  ٻاهريون.append_str (" >>\nstream\n");
  ٻاهريون.append_len (وهڪرو.arrayZ, وهڪرو.ڊيگهائي);
  ٻاهريون.append_str ("endstream\nendobj\n");

  /* Cross-reference table */
  بنانشان xref_وٿي = ٻاهريون.ڊيگهائي;
  ٻاهريون.append_str ("xref\n0 5\n");
  ٻاهريون.append_str ("0000000000 65535 f \n");
  لكل (بنانشان i = 0; i < 4; i++)
  {
    حرف tmp[21];
    snprintf (tmp, حجم (tmp), "%010u 00000 n \n", offsets[i]);
    ٻاهريون.append_len (tmp, 20);
  }

  /* Trailer */
  ٻاهريون.append_str ("trailer\n<< /Size 5 /Root 1 0 R >>\nstartxref\n");
  ٻاهريون.append_unsigned (xref_وٿي);
  ٻاهريون.append_str ("\n%%EOF\n");

  hb_blob_t *blob = hb_buf_blob_from (&draw->recycled_blob, &ٻاهريون);

  أرجع blob;
}

ساكن hb_blob_t *
hb_ويڪٽر_draw_render_svg (hb_ويڪٽر_draw_t *draw)
{
  draw->flush_رستو ();
  إذا (!draw->has_extents)
    أرجع خالي_اشارو;

  hb_ويڪٽر_buf_t ٻاهريون;
  hb_buf_recover_recycled (draw->recycled_blob, &ٻاهريون);
  بنانشان estimated = draw->defs.ڊيگهائي +
		       (draw->body.ڊيگهائي ? draw->body.ڊيگهائي : draw->رستو.ڊيگهائي) +
		       256;
  ٻاهريون.alloc (estimated);
  /* Extents are in Y-up (font) space.  SVG is Y-down.
   * The global scale(1,-1) wrapper flips content; the
   * viewBox maps the flipped range. */
  عائم vb_x = draw->extents.x;
  عائم vb_y = -(draw->extents.y + draw->extents.اوچائي);
  عائم vb_w = draw->extents.ويڪر;
  عائم vb_h = draw->extents.اوچائي;
  ٻاهريون.append_str ("<svg xmlns=\"http://www.w3.org/2000/svg\" viewBox=\"");
  ٻاهريون.append_انگ (vb_x);
  ٻاهريون.append_c (' ');
  ٻاهريون.append_انگ (vb_y);
  ٻاهريون.append_c (' ');
  ٻاهريون.append_انگ (vb_w);
  ٻاهريون.append_c (' ');
  ٻاهريون.append_انگ (vb_h);
  ٻاهريون.append_str ("\" width=\"");
  ٻاهريون.append_انگ (vb_w);
  ٻاهريون.append_str ("\" height=\"");
  ٻاهريون.append_انگ (vb_h);
  ٻاهريون.append_str ("\">\n");

  إذا (draw->defs.ڊيگهائي)
  {
    ٻاهريون.append_str ("<defs>\n");
    ٻاهريون.append_len (draw->defs.arrayZ, draw->defs.ڊيگهائي);
    ٻاهريون.append_str ("</defs>\n");
  }

  /* Background rect. */
  إذا (hb_color_get_alpha (draw->background))
  {
    ٻاهريون.append_str ("<rect x=\"");
    ٻاهريون.append_انگ (vb_x);
    ٻاهريون.append_str ("\" y=\"");
    ٻاهريون.append_انگ (vb_y);
    ٻاهريون.append_str ("\" width=\"");
    ٻاهريون.append_انگ (vb_w);
    ٻاهريون.append_str ("\" height=\"");
    ٻاهريون.append_انگ (vb_h);
    ٻاهريون.append_str ("\" fill=\"rgb(");
    ٻاهريون.append_unsigned (hb_color_get_red (draw->background));
    ٻاهريون.append_c (',');
    ٻاهريون.append_unsigned (hb_color_get_green (draw->background));
    ٻاهريون.append_c (',');
    ٻاهريون.append_unsigned (hb_color_get_blue (draw->background));
    ٻاهريون.append_str (")\"");
    إذا (hb_color_get_alpha (draw->background) < 255)
    {
      ٻاهريون.append_str (" fill-opacity=\"");
      ٻاهريون.append_انگ (hb_color_get_alpha (draw->background) / 255.f, 4);
      ٻاهريون.append_c ('"');
    }
    ٻاهريون.append_str ("/>\n");
  }

  draw->flush_رستو ();
  إذا (draw->body.ڊيگهائي)
  {
    ٻاهريون.append_str ("<g transform=\"scale(1,-1)\">\n");
    ٻاهريون.append_len (draw->body.arrayZ, draw->body.ڊيگهائي);
    ٻاهريون.append_str ("</g>\n");
  }

  ٻاهريون.append_str ("</svg>\n");

  hb_blob_t *blob = hb_buf_blob_from (&draw->recycled_blob, &ٻاهريون);

  أرجع blob;
}

/**
 * hb_vector_draw_render:
 * @draw: a draw context.
 *
 * Renders accumulated draw content to an output blob.
 * Accumulated content and extents are cleared on return, including
 * when rendering fails. User configuration is preserved.
 *
 * Return value: (transfer full) (nullable): output blob, or `NULL` if rendering cannot proceed.
 *
 * Since: 13.0.0
 */
hb_blob_t *
hb_ويڪٽر_draw_render (hb_ويڪٽر_draw_t *draw)
{
  HB_SCOPE_GUARD (hb_ويڪٽر_draw_clear (draw));
  بدل (draw->format)
  {
    حالة HB_ويڪٽر_FORMAT_SVG:
      أرجع hb_ويڪٽر_draw_render_svg (draw);

    حالة HB_ويڪٽر_FORMAT_PDF:
      أرجع hb_ويڪٽر_draw_render_pdf (draw);

    حالة HB_ويڪٽر_FORMAT_INVALID: افتراضي:
      أرجع خالي_اشارو;
  }
}

/**
 * hb_vector_draw_clear:
 * @draw: a draw context.
 *
 * Discards accumulated draw output so @draw can be reused for
 * another render.  User configuration (transform, scale factors,
 * precision, foreground, background, and budget) is preserved.  Call
 * hb_vector_draw_reset() to also reset user configuration to defaults.
 *
 * Since: 14.2.0
 */
فراغ
hb_ويڪٽر_draw_clear (hb_ويڪٽر_draw_t *draw)
{
  draw->extents = {0, 0, 0, 0};
  draw->has_extents = خطأ;
  draw->defs.clear ();
  draw->body.clear ();
  draw->رستو.clear ();
  draw->pdf_extgstate_dict.clear ();
  draw->pdf_extgstate_ڳڻپ = 0;
  draw->recharge_budget ();
}

/**
 * hb_vector_draw_reset:
 * @draw: a draw context.
 *
 * Restores user configuration to defaults and clears accumulated content.
 * The output format and attached user data are preserved.
 *
 * Since: 13.0.0
 */
فراغ
hb_ويڪٽر_draw_reset (hb_ويڪٽر_draw_t *draw)
{
  draw->transform = {1, 0, 0, 1, 0, 0};
  draw->x_scale_ضرب_ڪندڙ = 1.f;
  draw->y_scale_ضرب_ڪندڙ = 1.f;
  draw->foreground = HB_COLOR (0, 0, 0, 255);
  draw->background = HB_COLOR (0, 0, 0, 0);
  draw->سيٽ_precision (2);
  draw->budget = HB_BUDGET_DEFAULT;
  hb_ويڪٽر_draw_clear (draw);
}

/**
 * hb_vector_draw_recycle_blob:
 * @draw: a draw context.
 * @blob: (transfer full) (nullable): previously rendered blob to recycle.
 *
 * Provides a blob for internal buffer reuse by later render calls.
 * The caller transfers ownership of one reference to @draw and must
 * not destroy that reference after this call. Finish accessing @blob
 * and its data before recycling: later renders may reuse the blob
 * and overwrite its data.
 *
 * Since: 13.0.0
 */
فراغ
hb_ويڪٽر_draw_recycle_blob (hb_ويڪٽر_draw_t *draw,
                             hb_blob_t *blob)
{
  hb_blob_destroy (draw->recycled_blob);
  draw->recycled_blob = خالي_اشارو;
  إذا (!blob || blob == hb_blob_get_خالي_جايون ())
    أرجع;
  draw->recycled_blob = blob;
}
