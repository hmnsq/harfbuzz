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

#شامل "hb.hh"

#شامل "hb-vector-draw.hh"
#شامل "hb-vector-path.hh"

#شامل <math.h>
#شامل <string.h>

HB_غیر_مستعمل ساکن اندرونی منطقی
hb_سمتیہ_svg_بفر_شامل_ہے (ثابت hb_سمتیہ_buf_t &buf, ثابت حرف *needle)
{
  غیرعلامتی nlen = (غیرعلامتی) لڑی_لمبائی (needle);
  اگر (!nlen || buf.لمبائی < nlen)
    واپس جھوٹ;

  برائے (غیرعلامتی i = 0; i + nlen <= buf.لمبائی; i++)
    اگر (buf.arrayZ[i] == needle[0] &&
        !memcmp (buf.arrayZ + i, needle, nlen))
      واپس سچ;
  واپس جھوٹ;
}

/* ---- SVG draw callbacks ---- */

ساکن خالی hb_سمتیہ_draw_svg_move_مقصد (hb_draw_funcs_t *, خالی *dd, hb_draw_state_t *, اعشاری x, اعشاری y, خالی *)
{
  خودکار *d = (hb_سمتیہ_draw_t *) dd;
  اگر (unlikely (!d->begin_راستہ_command ())) واپس;
  d->راستہ.append_c ('M'); d->append_xy_svg (x, y);
  d->انتہا_راستہ_command ();
}

ساکن خالی hb_سمتیہ_draw_svg_سطر_مقصد (hb_draw_funcs_t *, خالی *dd, hb_draw_state_t *, اعشاری x, اعشاری y, خالی *)
{
  خودکار *d = (hb_سمتیہ_draw_t *) dd;
  اگر (unlikely (!d->begin_راستہ_command ())) واپس;
  d->راستہ.append_c ('L'); d->append_xy_svg (x, y);
  d->انتہا_راستہ_command ();
}

ساکن خالی hb_سمتیہ_draw_svg_quadratic_مقصد (hb_draw_funcs_t *, خالی *dd, hb_draw_state_t *, اعشاری cx, اعشاری cy, اعشاری x, اعشاری y, خالی *)
{
  خودکار *d = (hb_سمتیہ_draw_t *) dd;
  اگر (unlikely (!d->begin_راستہ_command ())) واپس;
  d->راستہ.append_c ('Q'); d->append_xy_svg (cx, cy); d->راستہ.append_c (' '); d->append_xy_svg (x, y);
  d->انتہا_راستہ_command ();
}

ساکن خالی hb_سمتیہ_draw_svg_cubic_مقصد (hb_draw_funcs_t *, خالی *dd, hb_draw_state_t *, اعشاری c1x, اعشاری c1y, اعشاری c2x, اعشاری c2y, اعشاری x, اعشاری y, خالی *)
{
  خودکار *d = (hb_سمتیہ_draw_t *) dd;
  اگر (unlikely (!d->begin_راستہ_command ())) واپس;
  d->راستہ.append_c ('C'); d->append_xy_svg (c1x, c1y); d->راستہ.append_c (' '); d->append_xy_svg (c2x, c2y); d->راستہ.append_c (' '); d->append_xy_svg (x, y);
  d->انتہا_راستہ_command ();
}

ساکن خالی hb_سمتیہ_draw_svg_close_راستہ (hb_draw_funcs_t *, خالی *dd, hb_draw_state_t *, خالی *)
{
  خودکار *d = (hb_سمتیہ_draw_t *) dd;
  اگر (unlikely (!d->begin_راستہ_command ())) واپس;
  d->راستہ.append_c ('Z');
  d->انتہا_راستہ_command ();
}


/* ---- PDF draw callbacks ---- */

ساکن خالی hb_سمتیہ_draw_pdf_move_مقصد (hb_draw_funcs_t *, خالی *dd, hb_draw_state_t *, اعشاری x, اعشاری y, خالی *)
{
  خودکار *d = (hb_سمتیہ_draw_t *) dd;
  اگر (unlikely (!d->begin_راستہ_command ())) واپس;
  d->append_xy_pdf (x, y); d->راستہ.append_str (" m\n");
  d->انتہا_راستہ_command ();
}

ساکن خالی hb_سمتیہ_draw_pdf_سطر_مقصد (hb_draw_funcs_t *, خالی *dd, hb_draw_state_t *, اعشاری x, اعشاری y, خالی *)
{
  خودکار *d = (hb_سمتیہ_draw_t *) dd;
  اگر (unlikely (!d->begin_راستہ_command ())) واپس;
  d->append_xy_pdf (x, y); d->راستہ.append_str (" l\n");
  d->انتہا_راستہ_command ();
}

/* No quadratic_to — null fallback auto-promotes to cubic. */

ساکن خالی hb_سمتیہ_draw_pdf_cubic_مقصد (hb_draw_funcs_t *, خالی *dd, hb_draw_state_t *, اعشاری c1x, اعشاری c1y, اعشاری c2x, اعشاری c2y, اعشاری x, اعشاری y, خالی *)
{
  خودکار *d = (hb_سمتیہ_draw_t *) dd;
  اگر (unlikely (!d->begin_راستہ_command ())) واپس;
  d->append_xy_pdf (c1x, c1y); d->راستہ.append_c (' '); d->append_xy_pdf (c2x, c2y); d->راستہ.append_c (' '); d->append_xy_pdf (x, y); d->راستہ.append_str (" c\n");
  d->انتہا_راستہ_command ();
}

ساکن خالی hb_سمتیہ_draw_pdf_close_راستہ (hb_draw_funcs_t *, خالی *dd, hb_draw_state_t *, خالی *)
{
  خودکار *d = (hb_سمتیہ_draw_t *) dd;
  اگر (unlikely (!d->begin_راستہ_command ())) واپس;
  d->راستہ.append_str ("h\n");
  d->انتہا_راستہ_command ();
}

ساکن hb_bool_t
hb_سمتیہ_draw_سیٹ_budget (hb_draw_funcs_t *, خالی *draw_ڈیٹا,
			   int64_t budget, خالی *)
{
  خودکار *draw = (hb_سمتیہ_draw_t *) draw_ڈیٹا;
  draw->budget = budget;
  draw->recharge_budget ();
  واپس سچ;
}

ساکن int64_t
hb_سمتیہ_draw_get_budget (hb_draw_funcs_t *, خالی *draw_ڈیٹا, خالی *)
{
  واپس ((hb_سمتیہ_draw_t *) draw_ڈیٹا)->budget;
}

ساکن int64_t *
hb_سمتیہ_draw_get_budget_بقایا (hb_draw_funcs_t *, خالی *draw_ڈیٹا, خالی *)
{
  واپس &((hb_سمتیہ_draw_t *) draw_ڈیٹا)->budget_بقایا;
}


/* ---- Lazy loaders ---- */

ساکن اندرونی خالی free_static_سمتیہ_draw_svg_funcs ();
ساکن ساخت hb_سمتیہ_draw_svg_funcs_lazy_loader_t
  : hb_draw_funcs_lazy_loader_t<hb_سمتیہ_draw_svg_funcs_lazy_loader_t>
{
  ساکن hb_draw_funcs_t *create ()
  {
    hb_draw_funcs_t *funcs = hb_draw_funcs_create ();
    hb_draw_funcs_سیٹ_move_مقصد_func (funcs, (hb_draw_move_مقصد_func_t) hb_سمتیہ_draw_svg_move_مقصد, خالی_اشاریہ, خالی_اشاریہ);
    hb_draw_funcs_سیٹ_سطر_مقصد_func (funcs, (hb_draw_سطر_مقصد_func_t) hb_سمتیہ_draw_svg_سطر_مقصد, خالی_اشاریہ, خالی_اشاریہ);
    hb_draw_funcs_سیٹ_quadratic_مقصد_func (funcs, (hb_draw_quadratic_مقصد_func_t) hb_سمتیہ_draw_svg_quadratic_مقصد, خالی_اشاریہ, خالی_اشاریہ);
    hb_draw_funcs_سیٹ_cubic_مقصد_func (funcs, (hb_draw_cubic_مقصد_func_t) hb_سمتیہ_draw_svg_cubic_مقصد, خالی_اشاریہ, خالی_اشاریہ);
    hb_draw_funcs_سیٹ_close_راستہ_func (funcs, (hb_draw_close_راستہ_func_t) hb_سمتیہ_draw_svg_close_راستہ, خالی_اشاریہ, خالی_اشاریہ);
    hb_draw_funcs_سیٹ_سیٹ_budget_func (funcs, hb_سمتیہ_draw_سیٹ_budget, خالی_اشاریہ, خالی_اشاریہ);
    hb_draw_funcs_سیٹ_get_budget_func (funcs, hb_سمتیہ_draw_get_budget, خالی_اشاریہ, خالی_اشاریہ);
    hb_draw_funcs_سیٹ_get_budget_بقایا_func (funcs, hb_سمتیہ_draw_get_budget_بقایا, خالی_اشاریہ, خالی_اشاریہ);
    hb_draw_funcs_make_immutable (funcs);
    hb_atexit (free_static_سمتیہ_draw_svg_funcs);
    واپس funcs;
  }
} static_سمتیہ_draw_svg_funcs;
ساکن اندرونی خالی free_static_سمتیہ_draw_svg_funcs () { static_سمتیہ_draw_svg_funcs.free_instance (); }

ساکن اندرونی خالی free_static_سمتیہ_draw_pdf_funcs ();
ساکن ساخت hb_سمتیہ_draw_pdf_funcs_lazy_loader_t
  : hb_draw_funcs_lazy_loader_t<hb_سمتیہ_draw_pdf_funcs_lazy_loader_t>
{
  ساکن hb_draw_funcs_t *create ()
  {
    hb_draw_funcs_t *funcs = hb_draw_funcs_create ();
    hb_draw_funcs_سیٹ_move_مقصد_func (funcs, (hb_draw_move_مقصد_func_t) hb_سمتیہ_draw_pdf_move_مقصد, خالی_اشاریہ, خالی_اشاریہ);
    hb_draw_funcs_سیٹ_سطر_مقصد_func (funcs, (hb_draw_سطر_مقصد_func_t) hb_سمتیہ_draw_pdf_سطر_مقصد, خالی_اشاریہ, خالی_اشاریہ);
    /* No quadratic_to: null fallback auto-promotes to cubic. */
    hb_draw_funcs_سیٹ_cubic_مقصد_func (funcs, (hb_draw_cubic_مقصد_func_t) hb_سمتیہ_draw_pdf_cubic_مقصد, خالی_اشاریہ, خالی_اشاریہ);
    hb_draw_funcs_سیٹ_close_راستہ_func (funcs, (hb_draw_close_راستہ_func_t) hb_سمتیہ_draw_pdf_close_راستہ, خالی_اشاریہ, خالی_اشاریہ);
    hb_draw_funcs_سیٹ_سیٹ_budget_func (funcs, hb_سمتیہ_draw_سیٹ_budget, خالی_اشاریہ, خالی_اشاریہ);
    hb_draw_funcs_سیٹ_get_budget_func (funcs, hb_سمتیہ_draw_get_budget, خالی_اشاریہ, خالی_اشاریہ);
    hb_draw_funcs_سیٹ_get_budget_بقایا_func (funcs, hb_سمتیہ_draw_get_budget_بقایا, خالی_اشاریہ, خالی_اشاریہ);
    hb_draw_funcs_make_immutable (funcs);
    hb_atexit (free_static_سمتیہ_draw_pdf_funcs);
    واپس funcs;
  }
} static_سمتیہ_draw_pdf_funcs;
ساکن اندرونی خالی free_static_سمتیہ_draw_pdf_funcs () { static_سمتیہ_draw_pdf_funcs.free_instance (); }



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
hb_سمتیہ_draw_t *
hb_سمتیہ_draw_create_or_fail (hb_سمتیہ_format_t format)
{
  انتخاب (format)
  {
    حالت HB_سمتیہ_FORMAT_SVG:
    حالت HB_سمتیہ_FORMAT_PDF:
      توڑو;
    حالت HB_سمتیہ_FORMAT_INVALID: طےشدہ:
      واپس خالی_اشاریہ;
  }

  hb_سمتیہ_draw_t *draw = hb_object_create<hb_سمتیہ_draw_t> ();
  اگر (unlikely (!draw))
    واپس خالی_اشاریہ;
  draw->format = format;
  draw->defs.alloc (2048);
  draw->body.alloc (8192);
  draw->راستہ.alloc (2048);
  واپس draw;
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
hb_سمتیہ_draw_t *
hb_سمتیہ_draw_reference (hb_سمتیہ_draw_t *draw)
{
  واپس hb_object_reference (draw);
}

/**
 * hb_vector_draw_destroy:
 * @draw: a draw context.
 *
 * Decreases the reference count of @draw and destroys it when it reaches zero.
 *
 * Since: 13.0.0
 */
خالی
hb_سمتیہ_draw_destroy (hb_سمتیہ_draw_t *draw)
{
  اگر (!hb_object_should_destroy (draw))
    واپس;

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
hb_سمتیہ_draw_سیٹ_user_ڈیٹا (hb_سمتیہ_draw_t   *draw,
                              hb_user_ڈیٹا_کلید_t *key,
                              خالی               *ڈیٹا,
                              hb_destroy_func_t   destroy,
                              hb_bool_t           replace)
{
  واپس hb_object_سیٹ_user_ڈیٹا (draw, key, ڈیٹا, destroy, replace);
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
خالی *
hb_سمتیہ_draw_get_user_ڈیٹا (ثابت hb_سمتیہ_draw_t   *draw,
                              hb_user_ڈیٹا_کلید_t *key)
{
  واپس hb_object_get_user_ڈیٹا (draw, key);
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
خالی
hb_سمتیہ_draw_سیٹ_transform (hb_سمتیہ_draw_t *draw,
                              اعشاری xx, اعشاری yx,
                              اعشاری xy, اعشاری yy,
                              اعشاری dx, اعشاری dy)
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
خالی
hb_سمتیہ_draw_get_transform (ثابت hb_سمتیہ_draw_t *draw,
                              اعشاری *xx, اعشاری *yx,
                              اعشاری *xy, اعشاری *yy,
                              اعشاری *dx, اعشاری *dy)
{
  اگر (xx) *xx = draw->transform.xx;
  اگر (yx) *yx = draw->transform.yx;
  اگر (xy) *xy = draw->transform.xy;
  اگر (yy) *yy = draw->transform.yy;
  اگر (dx) *dx = draw->transform.x0;
  اگر (dy) *dy = draw->transform.y0;
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
خالی
hb_سمتیہ_draw_سیٹ_scale_ضربی_عدد (hb_سمتیہ_draw_t *draw,
                                 اعشاری x_scale_ضربی_عدد,
                                 اعشاری y_scale_ضربی_عدد)
{
  draw->x_scale_ضربی_عدد = x_scale_ضربی_عدد > 0.f ? x_scale_ضربی_عدد : 1.f;
  draw->y_scale_ضربی_عدد = y_scale_ضربی_عدد > 0.f ? y_scale_ضربی_عدد : 1.f;
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
خالی
hb_سمتیہ_draw_get_scale_ضربی_عدد (ثابت hb_سمتیہ_draw_t *draw,
                                 اعشاری *x_scale_ضربی_عدد,
                                 اعشاری *y_scale_ضربی_عدد)
{
  اگر (x_scale_ضربی_عدد) *x_scale_ضربی_عدد = draw->x_scale_ضربی_عدد;
  اگر (y_scale_ضربی_عدد) *y_scale_ضربی_عدد = draw->y_scale_ضربی_عدد;
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
خالی
hb_سمتیہ_draw_سیٹ_extents (hb_سمتیہ_draw_t *draw,
                            ثابت hb_سمتیہ_extents_t *extents)
{
  اگر (!extents)
  {
    draw->extents = {0, 0, 0, 0};
    draw->has_extents = جھوٹ;
    واپس;
  }

  اگر (extents->چوڑائی == 0.f || extents->اونچائی == 0.f)
    واپس;

  /* Caller-supplied extents are in input-space; divide by
   * scale_factor so they end up in output-space, matching
   * the per-glyph extents accumulated via
   * hb_vector_set_glyph_extents_common (which applies the
   * same divide).  Normalize so origin is the min corner and
   * width/height are positive — callers may pass a
   * glyph-extents-style box with negative height. */
  اعشاری x0 = extents->x / draw->x_scale_ضربی_عدد;
  اعشاری y0 = extents->y / draw->y_scale_ضربی_عدد;
  اعشاری x1 = x0 + extents->چوڑائی  / draw->x_scale_ضربی_عدد;
  اعشاری y1 = y0 + extents->اونچائی / draw->y_scale_ضربی_عدد;
  hb_سمتیہ_extents_t e = {
    hb_min (x0, x1), hb_min (y0, y1),
    fabsf (x1 - x0), fabsf (y1 - y0),
  };

  اگر (draw->has_extents)
  {
    اعشاری x0 = hb_min (draw->extents.x, e.x);
    اعشاری y0 = hb_min (draw->extents.y, e.y);
    اعشاری x1 = hb_max (draw->extents.x + draw->extents.چوڑائی,
                       e.x + e.چوڑائی);
    اعشاری y1 = hb_max (draw->extents.y + draw->extents.اونچائی,
                       e.y + e.اونچائی);
    draw->extents = {x0, y0, x1 - x0, y1 - y0};
  }
  ورنہ
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
hb_سمتیہ_draw_get_extents (ثابت hb_سمتیہ_draw_t *draw,
                            hb_سمتیہ_extents_t *extents)
{
  اگر (!draw->has_extents)
    واپس جھوٹ;

  اگر (extents)
    *extents = draw->extents;
  واپس سچ;
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
hb_سمتیہ_draw_سیٹ_glyph_extents (hb_سمتیہ_draw_t *draw,
                                  ثابت hb_glyph_extents_t *glyph_extents)
{
  hb_bool_t has_extents = draw->has_extents;
  hb_bool_t ret = hb_سمتیہ_سیٹ_glyph_extents_common (draw->transform,
						   draw->x_scale_ضربی_عدد,
						   draw->y_scale_ضربی_عدد,
						   glyph_extents,
						   &draw->extents,
						   &has_extents);
  draw->has_extents = has_extents;
  واپس ret;
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
hb_سمتیہ_format_t
hb_سمتیہ_draw_get_format (ثابت hb_سمتیہ_draw_t *draw)
{
  واپس draw->format;
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
hb_سمتیہ_draw_get_funcs (ثابت hb_سمتیہ_draw_t *draw)
{
  انتخاب (draw ? draw->format : HB_سمتیہ_FORMAT_INVALID)
  {
    حالت HB_سمتیہ_FORMAT_SVG: واپس static_سمتیہ_draw_svg_funcs.get_unconst ();
    حالت HB_سمتیہ_FORMAT_PDF: واپس static_سمتیہ_draw_pdf_funcs.get_unconst ();
    حالت HB_سمتیہ_FORMAT_INVALID: طےشدہ: واپس خالی_اشاریہ;
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
خالی
hb_سمتیہ_draw_new_راستہ (hb_سمتیہ_draw_t *draw)
{
  draw->new_راستہ ();
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
hb_سمتیہ_draw_glyph_or_fail (hb_سمتیہ_draw_t *draw,
                      hb_font_t *font,
                      hb_codepoint_t glyph,
                      hb_سمتیہ_extents_منوال_t extents_منوال)
{

  اگر (extents_منوال == HB_سمتیہ_EXTENTS_منوال_EXPAND)
  {
    hb_glyph_extents_t ge;
    اگر (hb_font_get_glyph_extents (font, glyph, &ge))
    {
      hb_bool_t has_extents = draw->has_extents;
      hb_سمتیہ_سیٹ_glyph_extents_common (draw->transform,
					  draw->x_scale_ضربی_عدد,
					  draw->y_scale_ضربی_عدد,
					  &ge,
					  &draw->extents,
					  &has_extents);
      draw->has_extents = has_extents;
    }
  }

  draw->new_راستہ ();
  واپس hb_font_draw_glyph_or_fail (font, glyph, hb_سمتیہ_draw_get_funcs (draw), draw);
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
خالی
hb_سمتیہ_draw_glyph (hb_سمتیہ_draw_t *draw,
                      hb_font_t *font,
                      hb_codepoint_t glyph,
                      hb_سمتیہ_extents_منوال_t extents_منوال)
{
  hb_سمتیہ_draw_glyph_or_fail (draw, font, glyph, extents_منوال);
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
خالی
hb_سمتیہ_draw_سیٹ_precision (hb_سمتیہ_draw_t *draw,
                             غیرعلامتی precision)
{
  draw->سیٹ_precision (precision);
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
غیرعلامتی
hb_سمتیہ_draw_get_precision (ثابت hb_سمتیہ_draw_t *draw)
{
  واپس draw->get_precision ();
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
خالی
hb_سمتیہ_draw_سیٹ_foreground (hb_سمتیہ_draw_t *draw,
                               hb_color_t foreground)
{
  draw->flush_راستہ ();
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
hb_سمتیہ_draw_get_foreground (ثابت hb_سمتیہ_draw_t *draw)
{
  واپس draw->foreground;
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
خالی
hb_سمتیہ_draw_سیٹ_background (hb_سمتیہ_draw_t *draw,
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
hb_سمتیہ_draw_get_background (ثابت hb_سمتیہ_draw_t *draw)
{
  واپس draw->background;
}

ساکن hb_blob_t *
hb_سمتیہ_draw_render_pdf (hb_سمتیہ_draw_t *draw)
{
  draw->flush_راستہ ();
  اگر (!draw->has_extents)
    واپس خالی_اشاریہ;

  /* Collect the content stream.  The path coordinates are in
   * SVG space (Y-down).  Prepend a CTM that flips Y so the
   * PDF page (Y-up) renders correctly. */
  اعشاری ex = draw->extents.x;
  اعشاری ey = draw->extents.y;
  اعشاری ew = draw->extents.چوڑائی;
  اعشاری eh = draw->extents.اونچائی;

  hb_سمتیہ_buf_t دھارا;
  دھارا.alloc (draw->body.لمبائی + draw->راستہ.لمبائی + 256);

  /* Background rect. */
  اگر (hb_color_get_alpha (draw->background))
  {
    اعشاری a = hb_color_get_alpha (draw->background) / 255.f;
    اگر (a < 1.f - 1.f / 512.f)
    {
      دھارا.append_عدد (a, 4);
      دھارا.append_str (" ca gs\n");
    }
    دھارا.append_عدد (hb_color_get_red (draw->background) / 255.f, 4);
    دھارا.append_c (' ');
    دھارا.append_عدد (hb_color_get_green (draw->background) / 255.f, 4);
    دھارا.append_c (' ');
    دھارا.append_عدد (hb_color_get_blue (draw->background) / 255.f, 4);
    دھارا.append_str (" rg\n");
    دھارا.append_عدد (ex);
    دھارا.append_c (' ');
    دھارا.append_عدد (-(ey + eh));
    دھارا.append_c (' ');
    دھارا.append_عدد (ew);
    دھارا.append_c (' ');
    دھارا.append_عدد (eh);
    دھارا.append_str (" re f\n");
  }

  draw->flush_راستہ ();
  اگر (draw->body.لمبائی)
    دھارا.append_len (draw->body.arrayZ, draw->body.لمبائی);

  /* Build PDF objects, tracking byte offsets for xref. */
  hb_سمتیہ_buf_t برآمد;
  hb_buf_recover_recycled (draw->recycled_blob, &برآمد);
  برآمد.alloc (دھارا.لمبائی + 512);

  غیرعلامتی offsets[5]; /* objects 1-4, plus end */

  برآمد.append_str ("%PDF-1.4\n%\xC0\xC1\xC2\xC3\n");

  /* Object 1: Catalog */
  offsets[0] = برآمد.لمبائی;
  برآمد.append_str ("1 0 obj\n<< /Type /Catalog /Pages 2 0 R >>\nendobj\n");

  /* Object 2: Pages */
  offsets[1] = برآمد.لمبائی;
  برآمد.append_str ("2 0 obj\n<< /Type /Pages /Kids [3 0 R] /Count 1 >>\nendobj\n");

  /* Object 3: Page.  Extents are in SVG space (y = -font_y).
   * Convert back: font Y range = [-(ey+eh) .. -ey]. */
  offsets[2] = برآمد.لمبائی;
  برآمد.append_str ("3 0 obj\n<< /Type /Page /Parent 2 0 R /MediaBox [");
  برآمد.append_عدد (ex);
  برآمد.append_c (' ');
  برآمد.append_عدد (-(ey + eh));
  برآمد.append_c (' ');
  برآمد.append_عدد (ex + ew);
  برآمد.append_c (' ');
  برآمد.append_عدد (-ey);
  برآمد.append_str ("] /Contents 4 0 R");
  اگر (draw->pdf_extgstate_dict.لمبائی)
  {
    برآمد.append_str (" /Resources << /ExtGState << ");
    برآمد.append_len (draw->pdf_extgstate_dict.arrayZ, draw->pdf_extgstate_dict.لمبائی);
    برآمد.append_str (">> >>");
  }
  برآمد.append_str (" >>\nendobj\n");

  /* Object 4: Content stream */
  offsets[3] = برآمد.لمبائی;
  برآمد.append_str ("4 0 obj\n<< /Length ");
  برآمد.append_unsigned (دھارا.لمبائی);
  برآمد.append_str (" >>\nstream\n");
  برآمد.append_len (دھارا.arrayZ, دھارا.لمبائی);
  برآمد.append_str ("endstream\nendobj\n");

  /* Cross-reference table */
  غیرعلامتی xref_انحراف = برآمد.لمبائی;
  برآمد.append_str ("xref\n0 5\n");
  برآمد.append_str ("0000000000 65535 f \n");
  برائے (غیرعلامتی i = 0; i < 4; i++)
  {
    حرف tmp[21];
    snprintf (tmp, حجم (tmp), "%010u 00000 n \n", offsets[i]);
    برآمد.append_len (tmp, 20);
  }

  /* Trailer */
  برآمد.append_str ("trailer\n<< /Size 5 /Root 1 0 R >>\nstartxref\n");
  برآمد.append_unsigned (xref_انحراف);
  برآمد.append_str ("\n%%EOF\n");

  hb_blob_t *blob = hb_buf_blob_from (&draw->recycled_blob, &برآمد);

  واپس blob;
}

ساکن hb_blob_t *
hb_سمتیہ_draw_render_svg (hb_سمتیہ_draw_t *draw)
{
  draw->flush_راستہ ();
  اگر (!draw->has_extents)
    واپس خالی_اشاریہ;

  hb_سمتیہ_buf_t برآمد;
  hb_buf_recover_recycled (draw->recycled_blob, &برآمد);
  غیرعلامتی estimated = draw->defs.لمبائی +
		       (draw->body.لمبائی ? draw->body.لمبائی : draw->راستہ.لمبائی) +
		       256;
  برآمد.alloc (estimated);
  /* Extents are in Y-up (font) space.  SVG is Y-down.
   * The global scale(1,-1) wrapper flips content; the
   * viewBox maps the flipped range. */
  اعشاری vb_x = draw->extents.x;
  اعشاری vb_y = -(draw->extents.y + draw->extents.اونچائی);
  اعشاری vb_w = draw->extents.چوڑائی;
  اعشاری vb_h = draw->extents.اونچائی;
  برآمد.append_str ("<svg xmlns=\"http://www.w3.org/2000/svg\" viewBox=\"");
  برآمد.append_عدد (vb_x);
  برآمد.append_c (' ');
  برآمد.append_عدد (vb_y);
  برآمد.append_c (' ');
  برآمد.append_عدد (vb_w);
  برآمد.append_c (' ');
  برآمد.append_عدد (vb_h);
  برآمد.append_str ("\" width=\"");
  برآمد.append_عدد (vb_w);
  برآمد.append_str ("\" height=\"");
  برآمد.append_عدد (vb_h);
  برآمد.append_str ("\">\n");

  اگر (draw->defs.لمبائی)
  {
    برآمد.append_str ("<defs>\n");
    برآمد.append_len (draw->defs.arrayZ, draw->defs.لمبائی);
    برآمد.append_str ("</defs>\n");
  }

  /* Background rect. */
  اگر (hb_color_get_alpha (draw->background))
  {
    برآمد.append_str ("<rect x=\"");
    برآمد.append_عدد (vb_x);
    برآمد.append_str ("\" y=\"");
    برآمد.append_عدد (vb_y);
    برآمد.append_str ("\" width=\"");
    برآمد.append_عدد (vb_w);
    برآمد.append_str ("\" height=\"");
    برآمد.append_عدد (vb_h);
    برآمد.append_str ("\" fill=\"rgb(");
    برآمد.append_unsigned (hb_color_get_red (draw->background));
    برآمد.append_c (',');
    برآمد.append_unsigned (hb_color_get_green (draw->background));
    برآمد.append_c (',');
    برآمد.append_unsigned (hb_color_get_blue (draw->background));
    برآمد.append_str (")\"");
    اگر (hb_color_get_alpha (draw->background) < 255)
    {
      برآمد.append_str (" fill-opacity=\"");
      برآمد.append_عدد (hb_color_get_alpha (draw->background) / 255.f, 4);
      برآمد.append_c ('"');
    }
    برآمد.append_str ("/>\n");
  }

  draw->flush_راستہ ();
  اگر (draw->body.لمبائی)
  {
    برآمد.append_str ("<g transform=\"scale(1,-1)\">\n");
    برآمد.append_len (draw->body.arrayZ, draw->body.لمبائی);
    برآمد.append_str ("</g>\n");
  }

  برآمد.append_str ("</svg>\n");

  hb_blob_t *blob = hb_buf_blob_from (&draw->recycled_blob, &برآمد);

  واپس blob;
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
hb_سمتیہ_draw_render (hb_سمتیہ_draw_t *draw)
{
  HB_SCOPE_GUARD (hb_سمتیہ_draw_clear (draw));
  انتخاب (draw->format)
  {
    حالت HB_سمتیہ_FORMAT_SVG:
      واپس hb_سمتیہ_draw_render_svg (draw);

    حالت HB_سمتیہ_FORMAT_PDF:
      واپس hb_سمتیہ_draw_render_pdf (draw);

    حالت HB_سمتیہ_FORMAT_INVALID: طےشدہ:
      واپس خالی_اشاریہ;
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
خالی
hb_سمتیہ_draw_clear (hb_سمتیہ_draw_t *draw)
{
  draw->extents = {0, 0, 0, 0};
  draw->has_extents = جھوٹ;
  draw->defs.clear ();
  draw->body.clear ();
  draw->راستہ.clear ();
  draw->pdf_extgstate_dict.clear ();
  draw->pdf_extgstate_گنتی = 0;
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
خالی
hb_سمتیہ_draw_reset (hb_سمتیہ_draw_t *draw)
{
  draw->transform = {1, 0, 0, 1, 0, 0};
  draw->x_scale_ضربی_عدد = 1.f;
  draw->y_scale_ضربی_عدد = 1.f;
  draw->foreground = HB_COLOR (0, 0, 0, 255);
  draw->background = HB_COLOR (0, 0, 0, 0);
  draw->سیٹ_precision (2);
  draw->budget = HB_BUDGET_DEFAULT;
  hb_سمتیہ_draw_clear (draw);
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
خالی
hb_سمتیہ_draw_recycle_blob (hb_سمتیہ_draw_t *draw,
                             hb_blob_t *blob)
{
  hb_blob_destroy (draw->recycled_blob);
  draw->recycled_blob = خالی_اشاریہ;
  اگر (!blob || blob == hb_blob_get_خالی_خانے ())
    واپس;
  draw->recycled_blob = blob;
}
