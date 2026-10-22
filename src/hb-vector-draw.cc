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

#అన్తర్భావ "hb-vector-draw.hh"
#అన్తర్భావ "hb-vector-path.hh"

#అన్తర్భావ <math.h>
#అన్తర్భావ <string.h>

HB_వాడని స్థిర అంతఃవరుస తార్కిక
hb_సదిశ_svg_బఫరు_కలిగియుంది (స్థిరరాశి hb_సదిశ_buf_t &buf, స్థిరరాశి అక్షరం *needle)
{
  గుర్తులేని nlen = (గుర్తులేని) అక్షరశ్రేణి_పొడవు (needle);
  ఒకవేళ (!nlen || buf.నిడివి < nlen)
    తిరిగి అబద్ధం;

  కొరకు (గుర్తులేని i = 0; i + nlen <= buf.నిడివి; i++)
    ఒకవేళ (buf.arrayZ[i] == needle[0] &&
        !memcmp (buf.arrayZ + i, needle, nlen))
      తిరిగి నిజం;
  తిరిగి అబద్ధం;
}

/* ---- SVG draw callbacks ---- */

స్థిర శూన్య hb_సదిశ_draw_svg_move_గమ్యం (hb_draw_funcs_t *, శూన్య *dd, hb_draw_state_t *, దశాంశం x, దశాంశం y, శూన్య *)
{
  స్వయంచాలక *d = (hb_సదిశ_draw_t *) dd;
  ఒకవేళ (unlikely (!d->begin_మార్గం_command ())) తిరిగి;
  d->మార్గం.append_c ('M'); d->append_xy_svg (x, y);
  d->చివర_మార్గం_command ();
}

స్థిర శూన్య hb_సదిశ_draw_svg_పంక్తి_గమ్యం (hb_draw_funcs_t *, శూన్య *dd, hb_draw_state_t *, దశాంశం x, దశాంశం y, శూన్య *)
{
  స్వయంచాలక *d = (hb_సదిశ_draw_t *) dd;
  ఒకవేళ (unlikely (!d->begin_మార్గం_command ())) తిరిగి;
  d->మార్గం.append_c ('L'); d->append_xy_svg (x, y);
  d->చివర_మార్గం_command ();
}

స్థిర శూన్య hb_సదిశ_draw_svg_quadratic_గమ్యం (hb_draw_funcs_t *, శూన్య *dd, hb_draw_state_t *, దశాంశం cx, దశాంశం cy, దశాంశం x, దశాంశం y, శూన్య *)
{
  స్వయంచాలక *d = (hb_సదిశ_draw_t *) dd;
  ఒకవేళ (unlikely (!d->begin_మార్గం_command ())) తిరిగి;
  d->మార్గం.append_c ('Q'); d->append_xy_svg (cx, cy); d->మార్గం.append_c (' '); d->append_xy_svg (x, y);
  d->చివర_మార్గం_command ();
}

స్థిర శూన్య hb_సదిశ_draw_svg_cubic_గమ్యం (hb_draw_funcs_t *, శూన్య *dd, hb_draw_state_t *, దశాంశం c1x, దశాంశం c1y, దశాంశం c2x, దశాంశం c2y, దశాంశం x, దశాంశం y, శూన్య *)
{
  స్వయంచాలక *d = (hb_సదిశ_draw_t *) dd;
  ఒకవేళ (unlikely (!d->begin_మార్గం_command ())) తిరిగి;
  d->మార్గం.append_c ('C'); d->append_xy_svg (c1x, c1y); d->మార్గం.append_c (' '); d->append_xy_svg (c2x, c2y); d->మార్గం.append_c (' '); d->append_xy_svg (x, y);
  d->చివర_మార్గం_command ();
}

స్థిర శూన్య hb_సదిశ_draw_svg_close_మార్గం (hb_draw_funcs_t *, శూన్య *dd, hb_draw_state_t *, శూన్య *)
{
  స్వయంచాలక *d = (hb_సదిశ_draw_t *) dd;
  ఒకవేళ (unlikely (!d->begin_మార్గం_command ())) తిరిగి;
  d->మార్గం.append_c ('Z');
  d->చివర_మార్గం_command ();
}


/* ---- PDF draw callbacks ---- */

స్థిర శూన్య hb_సదిశ_draw_pdf_move_గమ్యం (hb_draw_funcs_t *, శూన్య *dd, hb_draw_state_t *, దశాంశం x, దశాంశం y, శూన్య *)
{
  స్వయంచాలక *d = (hb_సదిశ_draw_t *) dd;
  ఒకవేళ (unlikely (!d->begin_మార్గం_command ())) తిరిగి;
  d->append_xy_pdf (x, y); d->మార్గం.append_str (" m\n");
  d->చివర_మార్గం_command ();
}

స్థిర శూన్య hb_సదిశ_draw_pdf_పంక్తి_గమ్యం (hb_draw_funcs_t *, శూన్య *dd, hb_draw_state_t *, దశాంశం x, దశాంశం y, శూన్య *)
{
  స్వయంచాలక *d = (hb_సదిశ_draw_t *) dd;
  ఒకవేళ (unlikely (!d->begin_మార్గం_command ())) తిరిగి;
  d->append_xy_pdf (x, y); d->మార్గం.append_str (" l\n");
  d->చివర_మార్గం_command ();
}

/* No quadratic_to — null fallback auto-promotes to cubic. */

స్థిర శూన్య hb_సదిశ_draw_pdf_cubic_గమ్యం (hb_draw_funcs_t *, శూన్య *dd, hb_draw_state_t *, దశాంశం c1x, దశాంశం c1y, దశాంశం c2x, దశాంశం c2y, దశాంశం x, దశాంశం y, శూన్య *)
{
  స్వయంచాలక *d = (hb_సదిశ_draw_t *) dd;
  ఒకవేళ (unlikely (!d->begin_మార్గం_command ())) తిరిగి;
  d->append_xy_pdf (c1x, c1y); d->మార్గం.append_c (' '); d->append_xy_pdf (c2x, c2y); d->మార్గం.append_c (' '); d->append_xy_pdf (x, y); d->మార్గం.append_str (" c\n");
  d->చివర_మార్గం_command ();
}

స్థిర శూన్య hb_సదిశ_draw_pdf_close_మార్గం (hb_draw_funcs_t *, శూన్య *dd, hb_draw_state_t *, శూన్య *)
{
  స్వయంచాలక *d = (hb_సదిశ_draw_t *) dd;
  ఒకవేళ (unlikely (!d->begin_మార్గం_command ())) తిరిగి;
  d->మార్గం.append_str ("h\n");
  d->చివర_మార్గం_command ();
}

స్థిర hb_bool_t
hb_సదిశ_draw_సమితి_budget (hb_draw_funcs_t *, శూన్య *draw_దత్తాంశం,
			   int64_t budget, శూన్య *)
{
  స్వయంచాలక *draw = (hb_సదిశ_draw_t *) draw_దత్తాంశం;
  draw->budget = budget;
  draw->recharge_budget ();
  తిరిగి నిజం;
}

స్థిర int64_t
hb_సదిశ_draw_get_budget (hb_draw_funcs_t *, శూన్య *draw_దత్తాంశం, శూన్య *)
{
  తిరిగి ((hb_సదిశ_draw_t *) draw_దత్తాంశం)->budget;
}

స్థిర int64_t *
hb_సదిశ_draw_get_budget_మిగిలిన (hb_draw_funcs_t *, శూన్య *draw_దత్తాంశం, శూన్య *)
{
  తిరిగి &((hb_సదిశ_draw_t *) draw_దత్తాంశం)->budget_మిగిలిన;
}


/* ---- Lazy loaders ---- */

స్థిర అంతఃవరుస శూన్య free_static_సదిశ_draw_svg_funcs ();
స్థిర నిర్మాణం hb_సదిశ_draw_svg_funcs_lazy_loader_t
  : hb_draw_funcs_lazy_loader_t<hb_సదిశ_draw_svg_funcs_lazy_loader_t>
{
  స్థిర hb_draw_funcs_t *create ()
  {
    hb_draw_funcs_t *funcs = hb_draw_funcs_create ();
    hb_draw_funcs_సమితి_move_గమ్యం_func (funcs, (hb_draw_move_గమ్యం_func_t) hb_సదిశ_draw_svg_move_గమ్యం, శూన్యసూచిక, శూన్యసూచిక);
    hb_draw_funcs_సమితి_పంక్తి_గమ్యం_func (funcs, (hb_draw_పంక్తి_గమ్యం_func_t) hb_సదిశ_draw_svg_పంక్తి_గమ్యం, శూన్యసూచిక, శూన్యసూచిక);
    hb_draw_funcs_సమితి_quadratic_గమ్యం_func (funcs, (hb_draw_quadratic_గమ్యం_func_t) hb_సదిశ_draw_svg_quadratic_గమ్యం, శూన్యసూచిక, శూన్యసూచిక);
    hb_draw_funcs_సమితి_cubic_గమ్యం_func (funcs, (hb_draw_cubic_గమ్యం_func_t) hb_సదిశ_draw_svg_cubic_గమ్యం, శూన్యసూచిక, శూన్యసూచిక);
    hb_draw_funcs_సమితి_close_మార్గం_func (funcs, (hb_draw_close_మార్గం_func_t) hb_సదిశ_draw_svg_close_మార్గం, శూన్యసూచిక, శూన్యసూచిక);
    hb_draw_funcs_సమితి_సమితి_budget_func (funcs, hb_సదిశ_draw_సమితి_budget, శూన్యసూచిక, శూన్యసూచిక);
    hb_draw_funcs_సమితి_get_budget_func (funcs, hb_సదిశ_draw_get_budget, శూన్యసూచిక, శూన్యసూచిక);
    hb_draw_funcs_సమితి_get_budget_మిగిలిన_func (funcs, hb_సదిశ_draw_get_budget_మిగిలిన, శూన్యసూచిక, శూన్యసూచిక);
    hb_draw_funcs_make_immutable (funcs);
    hb_atexit (free_static_సదిశ_draw_svg_funcs);
    తిరిగి funcs;
  }
} static_సదిశ_draw_svg_funcs;
స్థిర అంతఃవరుస శూన్య free_static_సదిశ_draw_svg_funcs () { static_సదిశ_draw_svg_funcs.free_instance (); }

స్థిర అంతఃవరుస శూన్య free_static_సదిశ_draw_pdf_funcs ();
స్థిర నిర్మాణం hb_సదిశ_draw_pdf_funcs_lazy_loader_t
  : hb_draw_funcs_lazy_loader_t<hb_సదిశ_draw_pdf_funcs_lazy_loader_t>
{
  స్థిర hb_draw_funcs_t *create ()
  {
    hb_draw_funcs_t *funcs = hb_draw_funcs_create ();
    hb_draw_funcs_సమితి_move_గమ్యం_func (funcs, (hb_draw_move_గమ్యం_func_t) hb_సదిశ_draw_pdf_move_గమ్యం, శూన్యసూచిక, శూన్యసూచిక);
    hb_draw_funcs_సమితి_పంక్తి_గమ్యం_func (funcs, (hb_draw_పంక్తి_గమ్యం_func_t) hb_సదిశ_draw_pdf_పంక్తి_గమ్యం, శూన్యసూచిక, శూన్యసూచిక);
    /* No quadratic_to: null fallback auto-promotes to cubic. */
    hb_draw_funcs_సమితి_cubic_గమ్యం_func (funcs, (hb_draw_cubic_గమ్యం_func_t) hb_సదిశ_draw_pdf_cubic_గమ్యం, శూన్యసూచిక, శూన్యసూచిక);
    hb_draw_funcs_సమితి_close_మార్గం_func (funcs, (hb_draw_close_మార్గం_func_t) hb_సదిశ_draw_pdf_close_మార్గం, శూన్యసూచిక, శూన్యసూచిక);
    hb_draw_funcs_సమితి_సమితి_budget_func (funcs, hb_సదిశ_draw_సమితి_budget, శూన్యసూచిక, శూన్యసూచిక);
    hb_draw_funcs_సమితి_get_budget_func (funcs, hb_సదిశ_draw_get_budget, శూన్యసూచిక, శూన్యసూచిక);
    hb_draw_funcs_సమితి_get_budget_మిగిలిన_func (funcs, hb_సదిశ_draw_get_budget_మిగిలిన, శూన్యసూచిక, శూన్యసూచిక);
    hb_draw_funcs_make_immutable (funcs);
    hb_atexit (free_static_సదిశ_draw_pdf_funcs);
    తిరిగి funcs;
  }
} static_సదిశ_draw_pdf_funcs;
స్థిర అంతఃవరుస శూన్య free_static_సదిశ_draw_pdf_funcs () { static_సదిశ_draw_pdf_funcs.free_instance (); }



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
hb_సదిశ_draw_t *
hb_సదిశ_draw_create_or_fail (hb_సదిశ_format_t format)
{
  ఎంపిక (format)
  {
    స్థితి HB_సదిశ_FORMAT_SVG:
    స్థితి HB_సదిశ_FORMAT_PDF:
      ఆపు;
    స్థితి HB_సదిశ_FORMAT_INVALID: అప్రమేయ:
      తిరిగి శూన్యసూచిక;
  }

  hb_సదిశ_draw_t *draw = hb_object_create<hb_సదిశ_draw_t> ();
  ఒకవేళ (unlikely (!draw))
    తిరిగి శూన్యసూచిక;
  draw->format = format;
  draw->defs.alloc (2048);
  draw->body.alloc (8192);
  draw->మార్గం.alloc (2048);
  తిరిగి draw;
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
hb_సదిశ_draw_t *
hb_సదిశ_draw_reference (hb_సదిశ_draw_t *draw)
{
  తిరిగి hb_object_reference (draw);
}

/**
 * hb_vector_draw_destroy:
 * @draw: a draw context.
 *
 * Decreases the reference count of @draw and destroys it when it reaches zero.
 *
 * Since: 13.0.0
 */
శూన్య
hb_సదిశ_draw_destroy (hb_సదిశ_draw_t *draw)
{
  ఒకవేళ (!hb_object_should_destroy (draw))
    తిరిగి;

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
hb_సదిశ_draw_సమితి_user_దత్తాంశం (hb_సదిశ_draw_t   *draw,
                              hb_user_దత్తాంశం_కీలకం_t *key,
                              శూన్య               *దత్తాంశం,
                              hb_destroy_func_t   destroy,
                              hb_bool_t           replace)
{
  తిరిగి hb_object_సమితి_user_దత్తాంశం (draw, key, దత్తాంశం, destroy, replace);
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
శూన్య *
hb_సదిశ_draw_get_user_దత్తాంశం (స్థిరరాశి hb_సదిశ_draw_t   *draw,
                              hb_user_దత్తాంశం_కీలకం_t *key)
{
  తిరిగి hb_object_get_user_దత్తాంశం (draw, key);
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
శూన్య
hb_సదిశ_draw_సమితి_transform (hb_సదిశ_draw_t *draw,
                              దశాంశం xx, దశాంశం yx,
                              దశాంశం xy, దశాంశం yy,
                              దశాంశం dx, దశాంశం dy)
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
శూన్య
hb_సదిశ_draw_get_transform (స్థిరరాశి hb_సదిశ_draw_t *draw,
                              దశాంశం *xx, దశాంశం *yx,
                              దశాంశం *xy, దశాంశం *yy,
                              దశాంశం *dx, దశాంశం *dy)
{
  ఒకవేళ (xx) *xx = draw->transform.xx;
  ఒకవేళ (yx) *yx = draw->transform.yx;
  ఒకవేళ (xy) *xy = draw->transform.xy;
  ఒకవేళ (yy) *yy = draw->transform.yy;
  ఒకవేళ (dx) *dx = draw->transform.x0;
  ఒకవేళ (dy) *dy = draw->transform.y0;
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
శూన్య
hb_సదిశ_draw_సమితి_scale_గుణకం (hb_సదిశ_draw_t *draw,
                                 దశాంశం x_scale_గుణకం,
                                 దశాంశం y_scale_గుణకం)
{
  draw->x_scale_గుణకం = x_scale_గుణకం > 0.f ? x_scale_గుణకం : 1.f;
  draw->y_scale_గుణకం = y_scale_గుణకం > 0.f ? y_scale_గుణకం : 1.f;
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
శూన్య
hb_సదిశ_draw_get_scale_గుణకం (స్థిరరాశి hb_సదిశ_draw_t *draw,
                                 దశాంశం *x_scale_గుణకం,
                                 దశాంశం *y_scale_గుణకం)
{
  ఒకవేళ (x_scale_గుణకం) *x_scale_గుణకం = draw->x_scale_గుణకం;
  ఒకవేళ (y_scale_గుణకం) *y_scale_గుణకం = draw->y_scale_గుణకం;
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
శూన్య
hb_సదిశ_draw_సమితి_extents (hb_సదిశ_draw_t *draw,
                            స్థిరరాశి hb_సదిశ_extents_t *extents)
{
  ఒకవేళ (!extents)
  {
    draw->extents = {0, 0, 0, 0};
    draw->has_extents = అబద్ధం;
    తిరిగి;
  }

  ఒకవేళ (extents->వెడల్పు == 0.f || extents->ఎత్తు == 0.f)
    తిరిగి;

  /* Caller-supplied extents are in input-space; divide by
   * scale_factor so they end up in output-space, matching
   * the per-glyph extents accumulated via
   * hb_vector_set_glyph_extents_common (which applies the
   * same divide).  Normalize so origin is the min corner and
   * width/height are positive — callers may pass a
   * glyph-extents-style box with negative height. */
  దశాంశం x0 = extents->x / draw->x_scale_గుణకం;
  దశాంశం y0 = extents->y / draw->y_scale_గుణకం;
  దశాంశం x1 = x0 + extents->వెడల్పు  / draw->x_scale_గుణకం;
  దశాంశం y1 = y0 + extents->ఎత్తు / draw->y_scale_గుణకం;
  hb_సదిశ_extents_t e = {
    hb_min (x0, x1), hb_min (y0, y1),
    fabsf (x1 - x0), fabsf (y1 - y0),
  };

  ఒకవేళ (draw->has_extents)
  {
    దశాంశం x0 = hb_min (draw->extents.x, e.x);
    దశాంశం y0 = hb_min (draw->extents.y, e.y);
    దశాంశం x1 = hb_max (draw->extents.x + draw->extents.వెడల్పు,
                       e.x + e.వెడల్పు);
    దశాంశం y1 = hb_max (draw->extents.y + draw->extents.ఎత్తు,
                       e.y + e.ఎత్తు);
    draw->extents = {x0, y0, x1 - x0, y1 - y0};
  }
  లేకపోతే
  {
    draw->extents = e;
    draw->has_extents = నిజం;
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
hb_సదిశ_draw_get_extents (స్థిరరాశి hb_సదిశ_draw_t *draw,
                            hb_సదిశ_extents_t *extents)
{
  ఒకవేళ (!draw->has_extents)
    తిరిగి అబద్ధం;

  ఒకవేళ (extents)
    *extents = draw->extents;
  తిరిగి నిజం;
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
hb_సదిశ_draw_సమితి_glyph_extents (hb_సదిశ_draw_t *draw,
                                  స్థిరరాశి hb_glyph_extents_t *glyph_extents)
{
  hb_bool_t has_extents = draw->has_extents;
  hb_bool_t ret = hb_సదిశ_సమితి_glyph_extents_common (draw->transform,
						   draw->x_scale_గుణకం,
						   draw->y_scale_గుణకం,
						   glyph_extents,
						   &draw->extents,
						   &has_extents);
  draw->has_extents = has_extents;
  తిరిగి ret;
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
hb_సదిశ_format_t
hb_సదిశ_draw_get_format (స్థిరరాశి hb_సదిశ_draw_t *draw)
{
  తిరిగి draw->format;
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
hb_సదిశ_draw_get_funcs (స్థిరరాశి hb_సదిశ_draw_t *draw)
{
  ఎంపిక (draw ? draw->format : HB_సదిశ_FORMAT_INVALID)
  {
    స్థితి HB_సదిశ_FORMAT_SVG: తిరిగి static_సదిశ_draw_svg_funcs.get_unconst ();
    స్థితి HB_సదిశ_FORMAT_PDF: తిరిగి static_సదిశ_draw_pdf_funcs.get_unconst ();
    స్థితి HB_సదిశ_FORMAT_INVALID: అప్రమేయ: తిరిగి శూన్యసూచిక;
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
శూన్య
hb_సదిశ_draw_new_మార్గం (hb_సదిశ_draw_t *draw)
{
  draw->new_మార్గం ();
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
hb_సదిశ_draw_glyph_or_fail (hb_సదిశ_draw_t *draw,
                      hb_font_t *font,
                      hb_codepoint_t glyph,
                      hb_సదిశ_extents_బహుళకం_t extents_బహుళకం)
{

  ఒకవేళ (extents_బహుళకం == HB_సదిశ_EXTENTS_బహుళకం_EXPAND)
  {
    hb_glyph_extents_t ge;
    ఒకవేళ (hb_font_get_glyph_extents (font, glyph, &ge))
    {
      hb_bool_t has_extents = draw->has_extents;
      hb_సదిశ_సమితి_glyph_extents_common (draw->transform,
					  draw->x_scale_గుణకం,
					  draw->y_scale_గుణకం,
					  &ge,
					  &draw->extents,
					  &has_extents);
      draw->has_extents = has_extents;
    }
  }

  draw->new_మార్గం ();
  తిరిగి hb_font_draw_glyph_or_fail (font, glyph, hb_సదిశ_draw_get_funcs (draw), draw);
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
శూన్య
hb_సదిశ_draw_glyph (hb_సదిశ_draw_t *draw,
                      hb_font_t *font,
                      hb_codepoint_t glyph,
                      hb_సదిశ_extents_బహుళకం_t extents_బహుళకం)
{
  hb_సదిశ_draw_glyph_or_fail (draw, font, glyph, extents_బహుళకం);
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
శూన్య
hb_సదిశ_draw_సమితి_precision (hb_సదిశ_draw_t *draw,
                             గుర్తులేని precision)
{
  draw->సమితి_precision (precision);
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
గుర్తులేని
hb_సదిశ_draw_get_precision (స్థిరరాశి hb_సదిశ_draw_t *draw)
{
  తిరిగి draw->get_precision ();
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
శూన్య
hb_సదిశ_draw_సమితి_foreground (hb_సదిశ_draw_t *draw,
                               hb_color_t foreground)
{
  draw->flush_మార్గం ();
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
hb_సదిశ_draw_get_foreground (స్థిరరాశి hb_సదిశ_draw_t *draw)
{
  తిరిగి draw->foreground;
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
శూన్య
hb_సదిశ_draw_సమితి_background (hb_సదిశ_draw_t *draw,
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
hb_సదిశ_draw_get_background (స్థిరరాశి hb_సదిశ_draw_t *draw)
{
  తిరిగి draw->background;
}

స్థిర hb_blob_t *
hb_సదిశ_draw_render_pdf (hb_సదిశ_draw_t *draw)
{
  draw->flush_మార్గం ();
  ఒకవేళ (!draw->has_extents)
    తిరిగి శూన్యసూచిక;

  /* Collect the content stream.  The path coordinates are in
   * SVG space (Y-down).  Prepend a CTM that flips Y so the
   * PDF page (Y-up) renders correctly. */
  దశాంశం ex = draw->extents.x;
  దశాంశం ey = draw->extents.y;
  దశాంశం ew = draw->extents.వెడల్పు;
  దశాంశం eh = draw->extents.ఎత్తు;

  hb_సదిశ_buf_t ప్రవాహం;
  ప్రవాహం.alloc (draw->body.నిడివి + draw->మార్గం.నిడివి + 256);

  /* Background rect. */
  ఒకవేళ (hb_color_get_alpha (draw->background))
  {
    దశాంశం a = hb_color_get_alpha (draw->background) / 255.f;
    ఒకవేళ (a < 1.f - 1.f / 512.f)
    {
      ప్రవాహం.append_అంకె (a, 4);
      ప్రవాహం.append_str (" ca gs\n");
    }
    ప్రవాహం.append_అంకె (hb_color_get_red (draw->background) / 255.f, 4);
    ప్రవాహం.append_c (' ');
    ప్రవాహం.append_అంకె (hb_color_get_green (draw->background) / 255.f, 4);
    ప్రవాహం.append_c (' ');
    ప్రవాహం.append_అంకె (hb_color_get_blue (draw->background) / 255.f, 4);
    ప్రవాహం.append_str (" rg\n");
    ప్రవాహం.append_అంకె (ex);
    ప్రవాహం.append_c (' ');
    ప్రవాహం.append_అంకె (-(ey + eh));
    ప్రవాహం.append_c (' ');
    ప్రవాహం.append_అంకె (ew);
    ప్రవాహం.append_c (' ');
    ప్రవాహం.append_అంకె (eh);
    ప్రవాహం.append_str (" re f\n");
  }

  draw->flush_మార్గం ();
  ఒకవేళ (draw->body.నిడివి)
    ప్రవాహం.append_len (draw->body.arrayZ, draw->body.నిడివి);

  /* Build PDF objects, tracking byte offsets for xref. */
  hb_సదిశ_buf_t బహిర్గతం;
  hb_buf_recover_recycled (draw->recycled_blob, &బహిర్గతం);
  బహిర్గతం.alloc (ప్రవాహం.నిడివి + 512);

  గుర్తులేని offsets[5]; /* objects 1-4, plus end */

  బహిర్గతం.append_str ("%PDF-1.4\n%\xC0\xC1\xC2\xC3\n");

  /* Object 1: Catalog */
  offsets[0] = బహిర్గతం.నిడివి;
  బహిర్గతం.append_str ("1 0 obj\n<< /Type /Catalog /Pages 2 0 R >>\nendobj\n");

  /* Object 2: Pages */
  offsets[1] = బహిర్గతం.నిడివి;
  బహిర్గతం.append_str ("2 0 obj\n<< /Type /Pages /Kids [3 0 R] /Count 1 >>\nendobj\n");

  /* Object 3: Page.  Extents are in SVG space (y = -font_y).
   * Convert back: font Y range = [-(ey+eh) .. -ey]. */
  offsets[2] = బహిర్గతం.నిడివి;
  బహిర్గతం.append_str ("3 0 obj\n<< /Type /Page /Parent 2 0 R /MediaBox [");
  బహిర్గతం.append_అంకె (ex);
  బహిర్గతం.append_c (' ');
  బహిర్గతం.append_అంకె (-(ey + eh));
  బహిర్గతం.append_c (' ');
  బహిర్గతం.append_అంకె (ex + ew);
  బహిర్గతం.append_c (' ');
  బహిర్గతం.append_అంకె (-ey);
  బహిర్గతం.append_str ("] /Contents 4 0 R");
  ఒకవేళ (draw->pdf_extgstate_dict.నిడివి)
  {
    బహిర్గతం.append_str (" /Resources << /ExtGState << ");
    బహిర్గతం.append_len (draw->pdf_extgstate_dict.arrayZ, draw->pdf_extgstate_dict.నిడివి);
    బహిర్గతం.append_str (">> >>");
  }
  బహిర్గతం.append_str (" >>\nendobj\n");

  /* Object 4: Content stream */
  offsets[3] = బహిర్గతం.నిడివి;
  బహిర్గతం.append_str ("4 0 obj\n<< /Length ");
  బహిర్గతం.append_unsigned (ప్రవాహం.నిడివి);
  బహిర్గతం.append_str (" >>\nstream\n");
  బహిర్గతం.append_len (ప్రవాహం.arrayZ, ప్రవాహం.నిడివి);
  బహిర్గతం.append_str ("endstream\nendobj\n");

  /* Cross-reference table */
  గుర్తులేని xref_స్థానభ్రంశం = బహిర్గతం.నిడివి;
  బహిర్గతం.append_str ("xref\n0 5\n");
  బహిర్గతం.append_str ("0000000000 65535 f \n");
  కొరకు (గుర్తులేని i = 0; i < 4; i++)
  {
    అక్షరం tmp[21];
    snprintf (tmp, పరిమాణం (tmp), "%010u 00000 n \n", offsets[i]);
    బహిర్గతం.append_len (tmp, 20);
  }

  /* Trailer */
  బహిర్గతం.append_str ("trailer\n<< /Size 5 /Root 1 0 R >>\nstartxref\n");
  బహిర్గతం.append_unsigned (xref_స్థానభ్రంశం);
  బహిర్గతం.append_str ("\n%%EOF\n");

  hb_blob_t *blob = hb_buf_blob_from (&draw->recycled_blob, &బహిర్గతం);

  తిరిగి blob;
}

స్థిర hb_blob_t *
hb_సదిశ_draw_render_svg (hb_సదిశ_draw_t *draw)
{
  draw->flush_మార్గం ();
  ఒకవేళ (!draw->has_extents)
    తిరిగి శూన్యసూచిక;

  hb_సదిశ_buf_t బహిర్గతం;
  hb_buf_recover_recycled (draw->recycled_blob, &బహిర్గతం);
  గుర్తులేని estimated = draw->defs.నిడివి +
		       (draw->body.నిడివి ? draw->body.నిడివి : draw->మార్గం.నిడివి) +
		       256;
  బహిర్గతం.alloc (estimated);
  /* Extents are in Y-up (font) space.  SVG is Y-down.
   * The global scale(1,-1) wrapper flips content; the
   * viewBox maps the flipped range. */
  దశాంశం vb_x = draw->extents.x;
  దశాంశం vb_y = -(draw->extents.y + draw->extents.ఎత్తు);
  దశాంశం vb_w = draw->extents.వెడల్పు;
  దశాంశం vb_h = draw->extents.ఎత్తు;
  బహిర్గతం.append_str ("<svg xmlns=\"http://www.w3.org/2000/svg\" viewBox=\"");
  బహిర్గతం.append_అంకె (vb_x);
  బహిర్గతం.append_c (' ');
  బహిర్గతం.append_అంకె (vb_y);
  బహిర్గతం.append_c (' ');
  బహిర్గతం.append_అంకె (vb_w);
  బహిర్గతం.append_c (' ');
  బహిర్గతం.append_అంకె (vb_h);
  బహిర్గతం.append_str ("\" width=\"");
  బహిర్గతం.append_అంకె (vb_w);
  బహిర్గతం.append_str ("\" height=\"");
  బహిర్గతం.append_అంకె (vb_h);
  బహిర్గతం.append_str ("\">\n");

  ఒకవేళ (draw->defs.నిడివి)
  {
    బహిర్గతం.append_str ("<defs>\n");
    బహిర్గతం.append_len (draw->defs.arrayZ, draw->defs.నిడివి);
    బహిర్గతం.append_str ("</defs>\n");
  }

  /* Background rect. */
  ఒకవేళ (hb_color_get_alpha (draw->background))
  {
    బహిర్గతం.append_str ("<rect x=\"");
    బహిర్గతం.append_అంకె (vb_x);
    బహిర్గతం.append_str ("\" y=\"");
    బహిర్గతం.append_అంకె (vb_y);
    బహిర్గతం.append_str ("\" width=\"");
    బహిర్గతం.append_అంకె (vb_w);
    బహిర్గతం.append_str ("\" height=\"");
    బహిర్గతం.append_అంకె (vb_h);
    బహిర్గతం.append_str ("\" fill=\"rgb(");
    బహిర్గతం.append_unsigned (hb_color_get_red (draw->background));
    బహిర్గతం.append_c (',');
    బహిర్గతం.append_unsigned (hb_color_get_green (draw->background));
    బహిర్గతం.append_c (',');
    బహిర్గతం.append_unsigned (hb_color_get_blue (draw->background));
    బహిర్గతం.append_str (")\"");
    ఒకవేళ (hb_color_get_alpha (draw->background) < 255)
    {
      బహిర్గతం.append_str (" fill-opacity=\"");
      బహిర్గతం.append_అంకె (hb_color_get_alpha (draw->background) / 255.f, 4);
      బహిర్గతం.append_c ('"');
    }
    బహిర్గతం.append_str ("/>\n");
  }

  draw->flush_మార్గం ();
  ఒకవేళ (draw->body.నిడివి)
  {
    బహిర్గతం.append_str ("<g transform=\"scale(1,-1)\">\n");
    బహిర్గతం.append_len (draw->body.arrayZ, draw->body.నిడివి);
    బహిర్గతం.append_str ("</g>\n");
  }

  బహిర్గతం.append_str ("</svg>\n");

  hb_blob_t *blob = hb_buf_blob_from (&draw->recycled_blob, &బహిర్గతం);

  తిరిగి blob;
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
hb_సదిశ_draw_render (hb_సదిశ_draw_t *draw)
{
  HB_SCOPE_GUARD (hb_సదిశ_draw_clear (draw));
  ఎంపిక (draw->format)
  {
    స్థితి HB_సదిశ_FORMAT_SVG:
      తిరిగి hb_సదిశ_draw_render_svg (draw);

    స్థితి HB_సదిశ_FORMAT_PDF:
      తిరిగి hb_సదిశ_draw_render_pdf (draw);

    స్థితి HB_సదిశ_FORMAT_INVALID: అప్రమేయ:
      తిరిగి శూన్యసూచిక;
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
శూన్య
hb_సదిశ_draw_clear (hb_సదిశ_draw_t *draw)
{
  draw->extents = {0, 0, 0, 0};
  draw->has_extents = అబద్ధం;
  draw->defs.clear ();
  draw->body.clear ();
  draw->మార్గం.clear ();
  draw->pdf_extgstate_dict.clear ();
  draw->pdf_extgstate_లెక్క = 0;
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
శూన్య
hb_సదిశ_draw_reset (hb_సదిశ_draw_t *draw)
{
  draw->transform = {1, 0, 0, 1, 0, 0};
  draw->x_scale_గుణకం = 1.f;
  draw->y_scale_గుణకం = 1.f;
  draw->foreground = HB_COLOR (0, 0, 0, 255);
  draw->background = HB_COLOR (0, 0, 0, 0);
  draw->సమితి_precision (2);
  draw->budget = HB_BUDGET_DEFAULT;
  hb_సదిశ_draw_clear (draw);
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
శూన్య
hb_సదిశ_draw_recycle_blob (hb_సదిశ_draw_t *draw,
                             hb_blob_t *blob)
{
  hb_blob_destroy (draw->recycled_blob);
  draw->recycled_blob = శూన్యసూచిక;
  ఒకవేళ (!blob || blob == hb_blob_get_ఖాళీలు ())
    తిరిగి;
  draw->recycled_blob = blob;
}
