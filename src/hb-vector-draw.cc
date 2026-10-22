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

HB_استفادهنشده ساكن مضمن منطقي
hb_وکتور_svg_میانگیر_شامل_است (ثابت hb_وکتور_buf_t &buf, ثابت حرف *needle)
{
  بیعلامت nlen = (بیعلامت) طول_رشته (needle);
  إذا (!nlen || buf.درازی < nlen)
    أرجع خطأ;

  لكل (بیعلامت i = 0; i + nlen <= buf.درازی; i++)
    إذا (buf.arrayZ[i] == needle[0] &&
        !memcmp (buf.arrayZ + i, needle, nlen))
      أرجع درست;
  أرجع خطأ;
}

/* ---- SVG draw callbacks ---- */

ساكن فراغ hb_وکتور_draw_svg_move_مقصد (hb_draw_funcs_t *, فراغ *dd, hb_draw_state_t *, عائم x, عائم y, فراغ *)
{
  خودکار *d = (hb_وکتور_draw_t *) dd;
  إذا (unlikely (!d->begin_مسیر_command ())) أرجع;
  d->مسیر.append_c ('M'); d->append_xy_svg (x, y);
  d->انتها_مسیر_command ();
}

ساكن فراغ hb_وکتور_draw_svg_سطر_مقصد (hb_draw_funcs_t *, فراغ *dd, hb_draw_state_t *, عائم x, عائم y, فراغ *)
{
  خودکار *d = (hb_وکتور_draw_t *) dd;
  إذا (unlikely (!d->begin_مسیر_command ())) أرجع;
  d->مسیر.append_c ('L'); d->append_xy_svg (x, y);
  d->انتها_مسیر_command ();
}

ساكن فراغ hb_وکتور_draw_svg_quadratic_مقصد (hb_draw_funcs_t *, فراغ *dd, hb_draw_state_t *, عائم cx, عائم cy, عائم x, عائم y, فراغ *)
{
  خودکار *d = (hb_وکتور_draw_t *) dd;
  إذا (unlikely (!d->begin_مسیر_command ())) أرجع;
  d->مسیر.append_c ('Q'); d->append_xy_svg (cx, cy); d->مسیر.append_c (' '); d->append_xy_svg (x, y);
  d->انتها_مسیر_command ();
}

ساكن فراغ hb_وکتور_draw_svg_cubic_مقصد (hb_draw_funcs_t *, فراغ *dd, hb_draw_state_t *, عائم c1x, عائم c1y, عائم c2x, عائم c2y, عائم x, عائم y, فراغ *)
{
  خودکار *d = (hb_وکتور_draw_t *) dd;
  إذا (unlikely (!d->begin_مسیر_command ())) أرجع;
  d->مسیر.append_c ('C'); d->append_xy_svg (c1x, c1y); d->مسیر.append_c (' '); d->append_xy_svg (c2x, c2y); d->مسیر.append_c (' '); d->append_xy_svg (x, y);
  d->انتها_مسیر_command ();
}

ساكن فراغ hb_وکتور_draw_svg_close_مسیر (hb_draw_funcs_t *, فراغ *dd, hb_draw_state_t *, فراغ *)
{
  خودکار *d = (hb_وکتور_draw_t *) dd;
  إذا (unlikely (!d->begin_مسیر_command ())) أرجع;
  d->مسیر.append_c ('Z');
  d->انتها_مسیر_command ();
}


/* ---- PDF draw callbacks ---- */

ساكن فراغ hb_وکتور_draw_pdf_move_مقصد (hb_draw_funcs_t *, فراغ *dd, hb_draw_state_t *, عائم x, عائم y, فراغ *)
{
  خودکار *d = (hb_وکتور_draw_t *) dd;
  إذا (unlikely (!d->begin_مسیر_command ())) أرجع;
  d->append_xy_pdf (x, y); d->مسیر.append_str (" m\n");
  d->انتها_مسیر_command ();
}

ساكن فراغ hb_وکتور_draw_pdf_سطر_مقصد (hb_draw_funcs_t *, فراغ *dd, hb_draw_state_t *, عائم x, عائم y, فراغ *)
{
  خودکار *d = (hb_وکتور_draw_t *) dd;
  إذا (unlikely (!d->begin_مسیر_command ())) أرجع;
  d->append_xy_pdf (x, y); d->مسیر.append_str (" l\n");
  d->انتها_مسیر_command ();
}

/* No quadratic_to — null fallback auto-promotes to cubic. */

ساكن فراغ hb_وکتور_draw_pdf_cubic_مقصد (hb_draw_funcs_t *, فراغ *dd, hb_draw_state_t *, عائم c1x, عائم c1y, عائم c2x, عائم c2y, عائم x, عائم y, فراغ *)
{
  خودکار *d = (hb_وکتور_draw_t *) dd;
  إذا (unlikely (!d->begin_مسیر_command ())) أرجع;
  d->append_xy_pdf (c1x, c1y); d->مسیر.append_c (' '); d->append_xy_pdf (c2x, c2y); d->مسیر.append_c (' '); d->append_xy_pdf (x, y); d->مسیر.append_str (" c\n");
  d->انتها_مسیر_command ();
}

ساكن فراغ hb_وکتور_draw_pdf_close_مسیر (hb_draw_funcs_t *, فراغ *dd, hb_draw_state_t *, فراغ *)
{
  خودکار *d = (hb_وکتور_draw_t *) dd;
  إذا (unlikely (!d->begin_مسیر_command ())) أرجع;
  d->مسیر.append_str ("h\n");
  d->انتها_مسیر_command ();
}

ساكن hb_bool_t
hb_وکتور_draw_مجموعه_budget (hb_draw_funcs_t *, فراغ *draw_داده,
			   int64_t budget, فراغ *)
{
  خودکار *draw = (hb_وکتور_draw_t *) draw_داده;
  draw->budget = budget;
  draw->recharge_budget ();
  أرجع درست;
}

ساكن int64_t
hb_وکتور_draw_get_budget (hb_draw_funcs_t *, فراغ *draw_داده, فراغ *)
{
  أرجع ((hb_وکتور_draw_t *) draw_داده)->budget;
}

ساكن int64_t *
hb_وکتور_draw_get_budget_مانده (hb_draw_funcs_t *, فراغ *draw_داده, فراغ *)
{
  أرجع &((hb_وکتور_draw_t *) draw_داده)->budget_مانده;
}


/* ---- Lazy loaders ---- */

ساكن مضمن فراغ free_static_وکتور_draw_svg_funcs ();
ساكن بنية hb_وکتور_draw_svg_funcs_lazy_loader_t
  : hb_draw_funcs_lazy_loader_t<hb_وکتور_draw_svg_funcs_lazy_loader_t>
{
  ساكن hb_draw_funcs_t *create ()
  {
    hb_draw_funcs_t *funcs = hb_draw_funcs_create ();
    hb_draw_funcs_مجموعه_move_مقصد_func (funcs, (hb_draw_move_مقصد_func_t) hb_وکتور_draw_svg_move_مقصد, اشاره_تهی, اشاره_تهی);
    hb_draw_funcs_مجموعه_سطر_مقصد_func (funcs, (hb_draw_سطر_مقصد_func_t) hb_وکتور_draw_svg_سطر_مقصد, اشاره_تهی, اشاره_تهی);
    hb_draw_funcs_مجموعه_quadratic_مقصد_func (funcs, (hb_draw_quadratic_مقصد_func_t) hb_وکتور_draw_svg_quadratic_مقصد, اشاره_تهی, اشاره_تهی);
    hb_draw_funcs_مجموعه_cubic_مقصد_func (funcs, (hb_draw_cubic_مقصد_func_t) hb_وکتور_draw_svg_cubic_مقصد, اشاره_تهی, اشاره_تهی);
    hb_draw_funcs_مجموعه_close_مسیر_func (funcs, (hb_draw_close_مسیر_func_t) hb_وکتور_draw_svg_close_مسیر, اشاره_تهی, اشاره_تهی);
    hb_draw_funcs_مجموعه_مجموعه_budget_func (funcs, hb_وکتور_draw_مجموعه_budget, اشاره_تهی, اشاره_تهی);
    hb_draw_funcs_مجموعه_get_budget_func (funcs, hb_وکتور_draw_get_budget, اشاره_تهی, اشاره_تهی);
    hb_draw_funcs_مجموعه_get_budget_مانده_func (funcs, hb_وکتور_draw_get_budget_مانده, اشاره_تهی, اشاره_تهی);
    hb_draw_funcs_make_immutable (funcs);
    hb_atexit (free_static_وکتور_draw_svg_funcs);
    أرجع funcs;
  }
} static_وکتور_draw_svg_funcs;
ساكن مضمن فراغ free_static_وکتور_draw_svg_funcs () { static_وکتور_draw_svg_funcs.free_instance (); }

ساكن مضمن فراغ free_static_وکتور_draw_pdf_funcs ();
ساكن بنية hb_وکتور_draw_pdf_funcs_lazy_loader_t
  : hb_draw_funcs_lazy_loader_t<hb_وکتور_draw_pdf_funcs_lazy_loader_t>
{
  ساكن hb_draw_funcs_t *create ()
  {
    hb_draw_funcs_t *funcs = hb_draw_funcs_create ();
    hb_draw_funcs_مجموعه_move_مقصد_func (funcs, (hb_draw_move_مقصد_func_t) hb_وکتور_draw_pdf_move_مقصد, اشاره_تهی, اشاره_تهی);
    hb_draw_funcs_مجموعه_سطر_مقصد_func (funcs, (hb_draw_سطر_مقصد_func_t) hb_وکتور_draw_pdf_سطر_مقصد, اشاره_تهی, اشاره_تهی);
    /* No quadratic_to: null fallback auto-promotes to cubic. */
    hb_draw_funcs_مجموعه_cubic_مقصد_func (funcs, (hb_draw_cubic_مقصد_func_t) hb_وکتور_draw_pdf_cubic_مقصد, اشاره_تهی, اشاره_تهی);
    hb_draw_funcs_مجموعه_close_مسیر_func (funcs, (hb_draw_close_مسیر_func_t) hb_وکتور_draw_pdf_close_مسیر, اشاره_تهی, اشاره_تهی);
    hb_draw_funcs_مجموعه_مجموعه_budget_func (funcs, hb_وکتور_draw_مجموعه_budget, اشاره_تهی, اشاره_تهی);
    hb_draw_funcs_مجموعه_get_budget_func (funcs, hb_وکتور_draw_get_budget, اشاره_تهی, اشاره_تهی);
    hb_draw_funcs_مجموعه_get_budget_مانده_func (funcs, hb_وکتور_draw_get_budget_مانده, اشاره_تهی, اشاره_تهی);
    hb_draw_funcs_make_immutable (funcs);
    hb_atexit (free_static_وکتور_draw_pdf_funcs);
    أرجع funcs;
  }
} static_وکتور_draw_pdf_funcs;
ساكن مضمن فراغ free_static_وکتور_draw_pdf_funcs () { static_وکتور_draw_pdf_funcs.free_instance (); }



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
hb_وکتور_draw_t *
hb_وکتور_draw_create_or_fail (hb_وکتور_format_t format)
{
  بدل (format)
  {
    حالة HB_وکتور_FORMAT_SVG:
    حالة HB_وکتور_FORMAT_PDF:
      اكسر;
    حالة HB_وکتور_FORMAT_INVALID: افتراضي:
      أرجع اشاره_تهی;
  }

  hb_وکتور_draw_t *draw = hb_object_create<hb_وکتور_draw_t> ();
  إذا (unlikely (!draw))
    أرجع اشاره_تهی;
  draw->format = format;
  draw->defs.alloc (2048);
  draw->body.alloc (8192);
  draw->مسیر.alloc (2048);
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
hb_وکتور_draw_t *
hb_وکتور_draw_reference (hb_وکتور_draw_t *draw)
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
hb_وکتور_draw_destroy (hb_وکتور_draw_t *draw)
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
hb_وکتور_draw_مجموعه_user_داده (hb_وکتور_draw_t   *draw,
                              hb_user_داده_کلید_t *key,
                              فراغ               *داده,
                              hb_destroy_func_t   destroy,
                              hb_bool_t           replace)
{
  أرجع hb_object_مجموعه_user_داده (draw, key, داده, destroy, replace);
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
hb_وکتور_draw_get_user_داده (ثابت hb_وکتور_draw_t   *draw,
                              hb_user_داده_کلید_t *key)
{
  أرجع hb_object_get_user_داده (draw, key);
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
hb_وکتور_draw_مجموعه_transform (hb_وکتور_draw_t *draw,
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
hb_وکتور_draw_get_transform (ثابت hb_وکتور_draw_t *draw,
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
hb_وکتور_draw_مجموعه_scale_ضریب (hb_وکتور_draw_t *draw,
                                 عائم x_scale_ضریب,
                                 عائم y_scale_ضریب)
{
  draw->x_scale_ضریب = x_scale_ضریب > 0.f ? x_scale_ضریب : 1.f;
  draw->y_scale_ضریب = y_scale_ضریب > 0.f ? y_scale_ضریب : 1.f;
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
hb_وکتور_draw_get_scale_ضریب (ثابت hb_وکتور_draw_t *draw,
                                 عائم *x_scale_ضریب,
                                 عائم *y_scale_ضریب)
{
  إذا (x_scale_ضریب) *x_scale_ضریب = draw->x_scale_ضریب;
  إذا (y_scale_ضریب) *y_scale_ضریب = draw->y_scale_ضریب;
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
hb_وکتور_draw_مجموعه_extents (hb_وکتور_draw_t *draw,
                            ثابت hb_وکتور_extents_t *extents)
{
  إذا (!extents)
  {
    draw->extents = {0, 0, 0, 0};
    draw->has_extents = خطأ;
    أرجع;
  }

  إذا (extents->عرض == 0.f || extents->ارتفاع == 0.f)
    أرجع;

  /* Caller-supplied extents are in input-space; divide by
   * scale_factor so they end up in output-space, matching
   * the per-glyph extents accumulated via
   * hb_vector_set_glyph_extents_common (which applies the
   * same divide).  Normalize so origin is the min corner and
   * width/height are positive — callers may pass a
   * glyph-extents-style box with negative height. */
  عائم x0 = extents->x / draw->x_scale_ضریب;
  عائم y0 = extents->y / draw->y_scale_ضریب;
  عائم x1 = x0 + extents->عرض  / draw->x_scale_ضریب;
  عائم y1 = y0 + extents->ارتفاع / draw->y_scale_ضریب;
  hb_وکتور_extents_t e = {
    hb_min (x0, x1), hb_min (y0, y1),
    fabsf (x1 - x0), fabsf (y1 - y0),
  };

  إذا (draw->has_extents)
  {
    عائم x0 = hb_min (draw->extents.x, e.x);
    عائم y0 = hb_min (draw->extents.y, e.y);
    عائم x1 = hb_max (draw->extents.x + draw->extents.عرض,
                       e.x + e.عرض);
    عائم y1 = hb_max (draw->extents.y + draw->extents.ارتفاع,
                       e.y + e.ارتفاع);
    draw->extents = {x0, y0, x1 - x0, y1 - y0};
  }
  وإلا
  {
    draw->extents = e;
    draw->has_extents = درست;
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
hb_وکتور_draw_get_extents (ثابت hb_وکتور_draw_t *draw,
                            hb_وکتور_extents_t *extents)
{
  إذا (!draw->has_extents)
    أرجع خطأ;

  إذا (extents)
    *extents = draw->extents;
  أرجع درست;
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
hb_وکتور_draw_مجموعه_glyph_extents (hb_وکتور_draw_t *draw,
                                  ثابت hb_glyph_extents_t *glyph_extents)
{
  hb_bool_t has_extents = draw->has_extents;
  hb_bool_t ret = hb_وکتور_مجموعه_glyph_extents_common (draw->transform,
						   draw->x_scale_ضریب,
						   draw->y_scale_ضریب,
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
hb_وکتور_format_t
hb_وکتور_draw_get_format (ثابت hb_وکتور_draw_t *draw)
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
hb_وکتور_draw_get_funcs (ثابت hb_وکتور_draw_t *draw)
{
  بدل (draw ? draw->format : HB_وکتور_FORMAT_INVALID)
  {
    حالة HB_وکتور_FORMAT_SVG: أرجع static_وکتور_draw_svg_funcs.get_unconst ();
    حالة HB_وکتور_FORMAT_PDF: أرجع static_وکتور_draw_pdf_funcs.get_unconst ();
    حالة HB_وکتور_FORMAT_INVALID: افتراضي: أرجع اشاره_تهی;
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
hb_وکتور_draw_new_مسیر (hb_وکتور_draw_t *draw)
{
  draw->new_مسیر ();
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
hb_وکتور_draw_glyph_or_fail (hb_وکتور_draw_t *draw,
                      hb_font_t *font,
                      hb_codepoint_t glyph,
                      hb_وکتور_extents_نما_t extents_نما)
{

  إذا (extents_نما == HB_وکتور_EXTENTS_نما_EXPAND)
  {
    hb_glyph_extents_t ge;
    إذا (hb_font_get_glyph_extents (font, glyph, &ge))
    {
      hb_bool_t has_extents = draw->has_extents;
      hb_وکتور_مجموعه_glyph_extents_common (draw->transform,
					  draw->x_scale_ضریب,
					  draw->y_scale_ضریب,
					  &ge,
					  &draw->extents,
					  &has_extents);
      draw->has_extents = has_extents;
    }
  }

  draw->new_مسیر ();
  أرجع hb_font_draw_glyph_or_fail (font, glyph, hb_وکتور_draw_get_funcs (draw), draw);
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
hb_وکتور_draw_glyph (hb_وکتور_draw_t *draw,
                      hb_font_t *font,
                      hb_codepoint_t glyph,
                      hb_وکتور_extents_نما_t extents_نما)
{
  hb_وکتور_draw_glyph_or_fail (draw, font, glyph, extents_نما);
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
hb_وکتور_draw_مجموعه_precision (hb_وکتور_draw_t *draw,
                             بیعلامت precision)
{
  draw->مجموعه_precision (precision);
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
بیعلامت
hb_وکتور_draw_get_precision (ثابت hb_وکتور_draw_t *draw)
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
hb_وکتور_draw_مجموعه_foreground (hb_وکتور_draw_t *draw,
                               hb_color_t foreground)
{
  draw->flush_مسیر ();
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
hb_وکتور_draw_get_foreground (ثابت hb_وکتور_draw_t *draw)
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
hb_وکتور_draw_مجموعه_background (hb_وکتور_draw_t *draw,
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
hb_وکتور_draw_get_background (ثابت hb_وکتور_draw_t *draw)
{
  أرجع draw->background;
}

ساكن hb_blob_t *
hb_وکتور_draw_render_pdf (hb_وکتور_draw_t *draw)
{
  draw->flush_مسیر ();
  إذا (!draw->has_extents)
    أرجع اشاره_تهی;

  /* Collect the content stream.  The path coordinates are in
   * SVG space (Y-down).  Prepend a CTM that flips Y so the
   * PDF page (Y-up) renders correctly. */
  عائم ex = draw->extents.x;
  عائم ey = draw->extents.y;
  عائم ew = draw->extents.عرض;
  عائم eh = draw->extents.ارتفاع;

  hb_وکتور_buf_t جریان;
  جریان.alloc (draw->body.درازی + draw->مسیر.درازی + 256);

  /* Background rect. */
  إذا (hb_color_get_alpha (draw->background))
  {
    عائم a = hb_color_get_alpha (draw->background) / 255.f;
    إذا (a < 1.f - 1.f / 512.f)
    {
      جریان.append_رقم (a, 4);
      جریان.append_str (" ca gs\n");
    }
    جریان.append_رقم (hb_color_get_red (draw->background) / 255.f, 4);
    جریان.append_c (' ');
    جریان.append_رقم (hb_color_get_green (draw->background) / 255.f, 4);
    جریان.append_c (' ');
    جریان.append_رقم (hb_color_get_blue (draw->background) / 255.f, 4);
    جریان.append_str (" rg\n");
    جریان.append_رقم (ex);
    جریان.append_c (' ');
    جریان.append_رقم (-(ey + eh));
    جریان.append_c (' ');
    جریان.append_رقم (ew);
    جریان.append_c (' ');
    جریان.append_رقم (eh);
    جریان.append_str (" re f\n");
  }

  draw->flush_مسیر ();
  إذا (draw->body.درازی)
    جریان.append_len (draw->body.arrayZ, draw->body.درازی);

  /* Build PDF objects, tracking byte offsets for xref. */
  hb_وکتور_buf_t بیرون;
  hb_buf_recover_recycled (draw->recycled_blob, &بیرون);
  بیرون.alloc (جریان.درازی + 512);

  بیعلامت offsets[5]; /* objects 1-4, plus end */

  بیرون.append_str ("%PDF-1.4\n%\xC0\xC1\xC2\xC3\n");

  /* Object 1: Catalog */
  offsets[0] = بیرون.درازی;
  بیرون.append_str ("1 0 obj\n<< /Type /Catalog /Pages 2 0 R >>\nendobj\n");

  /* Object 2: Pages */
  offsets[1] = بیرون.درازی;
  بیرون.append_str ("2 0 obj\n<< /Type /Pages /Kids [3 0 R] /Count 1 >>\nendobj\n");

  /* Object 3: Page.  Extents are in SVG space (y = -font_y).
   * Convert back: font Y range = [-(ey+eh) .. -ey]. */
  offsets[2] = بیرون.درازی;
  بیرون.append_str ("3 0 obj\n<< /Type /Page /Parent 2 0 R /MediaBox [");
  بیرون.append_رقم (ex);
  بیرون.append_c (' ');
  بیرون.append_رقم (-(ey + eh));
  بیرون.append_c (' ');
  بیرون.append_رقم (ex + ew);
  بیرون.append_c (' ');
  بیرون.append_رقم (-ey);
  بیرون.append_str ("] /Contents 4 0 R");
  إذا (draw->pdf_extgstate_dict.درازی)
  {
    بیرون.append_str (" /Resources << /ExtGState << ");
    بیرون.append_len (draw->pdf_extgstate_dict.arrayZ, draw->pdf_extgstate_dict.درازی);
    بیرون.append_str (">> >>");
  }
  بیرون.append_str (" >>\nendobj\n");

  /* Object 4: Content stream */
  offsets[3] = بیرون.درازی;
  بیرون.append_str ("4 0 obj\n<< /Length ");
  بیرون.append_unsigned (جریان.درازی);
  بیرون.append_str (" >>\nstream\n");
  بیرون.append_len (جریان.arrayZ, جریان.درازی);
  بیرون.append_str ("endstream\nendobj\n");

  /* Cross-reference table */
  بیعلامت xref_جابه_جایی = بیرون.درازی;
  بیرون.append_str ("xref\n0 5\n");
  بیرون.append_str ("0000000000 65535 f \n");
  لكل (بیعلامت i = 0; i < 4; i++)
  {
    حرف tmp[21];
    snprintf (tmp, حجم (tmp), "%010u 00000 n \n", offsets[i]);
    بیرون.append_len (tmp, 20);
  }

  /* Trailer */
  بیرون.append_str ("trailer\n<< /Size 5 /Root 1 0 R >>\nstartxref\n");
  بیرون.append_unsigned (xref_جابه_جایی);
  بیرون.append_str ("\n%%EOF\n");

  hb_blob_t *blob = hb_buf_blob_from (&draw->recycled_blob, &بیرون);

  أرجع blob;
}

ساكن hb_blob_t *
hb_وکتور_draw_render_svg (hb_وکتور_draw_t *draw)
{
  draw->flush_مسیر ();
  إذا (!draw->has_extents)
    أرجع اشاره_تهی;

  hb_وکتور_buf_t بیرون;
  hb_buf_recover_recycled (draw->recycled_blob, &بیرون);
  بیعلامت estimated = draw->defs.درازی +
		       (draw->body.درازی ? draw->body.درازی : draw->مسیر.درازی) +
		       256;
  بیرون.alloc (estimated);
  /* Extents are in Y-up (font) space.  SVG is Y-down.
   * The global scale(1,-1) wrapper flips content; the
   * viewBox maps the flipped range. */
  عائم vb_x = draw->extents.x;
  عائم vb_y = -(draw->extents.y + draw->extents.ارتفاع);
  عائم vb_w = draw->extents.عرض;
  عائم vb_h = draw->extents.ارتفاع;
  بیرون.append_str ("<svg xmlns=\"http://www.w3.org/2000/svg\" viewBox=\"");
  بیرون.append_رقم (vb_x);
  بیرون.append_c (' ');
  بیرون.append_رقم (vb_y);
  بیرون.append_c (' ');
  بیرون.append_رقم (vb_w);
  بیرون.append_c (' ');
  بیرون.append_رقم (vb_h);
  بیرون.append_str ("\" width=\"");
  بیرون.append_رقم (vb_w);
  بیرون.append_str ("\" height=\"");
  بیرون.append_رقم (vb_h);
  بیرون.append_str ("\">\n");

  إذا (draw->defs.درازی)
  {
    بیرون.append_str ("<defs>\n");
    بیرون.append_len (draw->defs.arrayZ, draw->defs.درازی);
    بیرون.append_str ("</defs>\n");
  }

  /* Background rect. */
  إذا (hb_color_get_alpha (draw->background))
  {
    بیرون.append_str ("<rect x=\"");
    بیرون.append_رقم (vb_x);
    بیرون.append_str ("\" y=\"");
    بیرون.append_رقم (vb_y);
    بیرون.append_str ("\" width=\"");
    بیرون.append_رقم (vb_w);
    بیرون.append_str ("\" height=\"");
    بیرون.append_رقم (vb_h);
    بیرون.append_str ("\" fill=\"rgb(");
    بیرون.append_unsigned (hb_color_get_red (draw->background));
    بیرون.append_c (',');
    بیرون.append_unsigned (hb_color_get_green (draw->background));
    بیرون.append_c (',');
    بیرون.append_unsigned (hb_color_get_blue (draw->background));
    بیرون.append_str (")\"");
    إذا (hb_color_get_alpha (draw->background) < 255)
    {
      بیرون.append_str (" fill-opacity=\"");
      بیرون.append_رقم (hb_color_get_alpha (draw->background) / 255.f, 4);
      بیرون.append_c ('"');
    }
    بیرون.append_str ("/>\n");
  }

  draw->flush_مسیر ();
  إذا (draw->body.درازی)
  {
    بیرون.append_str ("<g transform=\"scale(1,-1)\">\n");
    بیرون.append_len (draw->body.arrayZ, draw->body.درازی);
    بیرون.append_str ("</g>\n");
  }

  بیرون.append_str ("</svg>\n");

  hb_blob_t *blob = hb_buf_blob_from (&draw->recycled_blob, &بیرون);

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
hb_وکتور_draw_render (hb_وکتور_draw_t *draw)
{
  HB_SCOPE_GUARD (hb_وکتور_draw_clear (draw));
  بدل (draw->format)
  {
    حالة HB_وکتور_FORMAT_SVG:
      أرجع hb_وکتور_draw_render_svg (draw);

    حالة HB_وکتور_FORMAT_PDF:
      أرجع hb_وکتور_draw_render_pdf (draw);

    حالة HB_وکتور_FORMAT_INVALID: افتراضي:
      أرجع اشاره_تهی;
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
hb_وکتور_draw_clear (hb_وکتور_draw_t *draw)
{
  draw->extents = {0, 0, 0, 0};
  draw->has_extents = خطأ;
  draw->defs.clear ();
  draw->body.clear ();
  draw->مسیر.clear ();
  draw->pdf_extgstate_dict.clear ();
  draw->pdf_extgstate_شمار = 0;
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
hb_وکتور_draw_reset (hb_وکتور_draw_t *draw)
{
  draw->transform = {1, 0, 0, 1, 0, 0};
  draw->x_scale_ضریب = 1.f;
  draw->y_scale_ضریب = 1.f;
  draw->foreground = HB_COLOR (0, 0, 0, 255);
  draw->background = HB_COLOR (0, 0, 0, 0);
  draw->مجموعه_precision (2);
  draw->budget = HB_BUDGET_DEFAULT;
  hb_وکتور_draw_clear (draw);
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
hb_وکتور_draw_recycle_blob (hb_وکتور_draw_t *draw,
                             hb_blob_t *blob)
{
  hb_blob_destroy (draw->recycled_blob);
  draw->recycled_blob = اشاره_تهی;
  إذا (!blob || blob == hb_blob_get_خالی ())
    أرجع;
  draw->recycled_blob = blob;
}
