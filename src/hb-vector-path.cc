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

#ضمن "hb-geometry.hh"
#ضمن "hb-machinery.hh"
#ضمن "hb-vector-path.hh"
#ضمن "hb-vector-buf.hh"
#ضمن "hb-vector-internal.hh"


/* ---- SVG path callbacks ---- */

ساكن مضمن فراغ
hb_وکتور_svg_مسیر_append_xy (hb_وکتور_مسیر_sink_t *s, عائم x, عائم y)
{
  s->مسیر->append_رقم (x / s->x_scale, s->precision);
  s->مسیر->append_c (',');
  s->مسیر->append_رقم (y / s->y_scale, s->precision);
}

ساكن فراغ
hb_وکتور_svg_مسیر_move_مقصد (hb_draw_funcs_t *, فراغ *draw_داده, hb_draw_state_t *,
			     عائم مقصد_x, عائم مقصد_y, فراغ *)
{
  خودکار *s = (hb_وکتور_مسیر_sink_t *) draw_داده;
  إذا (unlikely (!s->begin_command ())) أرجع;
  s->مسیر->append_c ('M');
  hb_وکتور_svg_مسیر_append_xy (s, مقصد_x, مقصد_y);
  s->انتها_command ();
}

ساكن فراغ
hb_وکتور_svg_مسیر_سطر_مقصد (hb_draw_funcs_t *, فراغ *draw_داده, hb_draw_state_t *,
			     عائم مقصد_x, عائم مقصد_y, فراغ *)
{
  خودکار *s = (hb_وکتور_مسیر_sink_t *) draw_داده;
  إذا (unlikely (!s->begin_command ())) أرجع;
  s->مسیر->append_c ('L');
  hb_وکتور_svg_مسیر_append_xy (s, مقصد_x, مقصد_y);
  s->انتها_command ();
}

ساكن فراغ
hb_وکتور_svg_مسیر_quadratic_مقصد (hb_draw_funcs_t *, فراغ *draw_داده, hb_draw_state_t *,
				  عائم cx, عائم cy, عائم مقصد_x, عائم مقصد_y, فراغ *)
{
  خودکار *s = (hb_وکتور_مسیر_sink_t *) draw_داده;
  إذا (unlikely (!s->begin_command ())) أرجع;
  s->مسیر->append_c ('Q');
  hb_وکتور_svg_مسیر_append_xy (s, cx, cy);
  s->مسیر->append_c (' ');
  hb_وکتور_svg_مسیر_append_xy (s, مقصد_x, مقصد_y);
  s->انتها_command ();
}

ساكن فراغ
hb_وکتور_svg_مسیر_cubic_مقصد (hb_draw_funcs_t *, فراغ *draw_داده, hb_draw_state_t *,
			      عائم c1x, عائم c1y, عائم c2x, عائم c2y,
			      عائم مقصد_x, عائم مقصد_y, فراغ *)
{
  خودکار *s = (hb_وکتور_مسیر_sink_t *) draw_داده;
  إذا (unlikely (!s->begin_command ())) أرجع;
  s->مسیر->append_c ('C');
  hb_وکتور_svg_مسیر_append_xy (s, c1x, c1y);
  s->مسیر->append_c (' ');
  hb_وکتور_svg_مسیر_append_xy (s, c2x, c2y);
  s->مسیر->append_c (' ');
  hb_وکتور_svg_مسیر_append_xy (s, مقصد_x, مقصد_y);
  s->انتها_command ();
}

ساكن فراغ
hb_وکتور_svg_مسیر_close_مسیر (hb_draw_funcs_t *, فراغ *draw_داده, hb_draw_state_t *, فراغ *)
{
  خودکار *s = (hb_وکتور_مسیر_sink_t *) draw_داده;
  إذا (unlikely (!s->begin_command ())) أرجع;
  s->مسیر->append_c ('Z');
  s->انتها_command ();
}


/* ---- PDF path callbacks ---- */

ساكن مضمن فراغ
hb_وکتور_pdf_مسیر_append_xy (hb_وکتور_مسیر_sink_t *s, عائم x, عائم y)
{
  s->مسیر->append_رقم (x / s->x_scale, s->precision);
  s->مسیر->append_c (' ');
  s->مسیر->append_رقم (y / s->y_scale, s->precision);
}

ساكن فراغ
hb_وکتور_pdf_مسیر_move_مقصد (hb_draw_funcs_t *, فراغ *draw_داده, hb_draw_state_t *,
			     عائم مقصد_x, عائم مقصد_y, فراغ *)
{
  خودکار *s = (hb_وکتور_مسیر_sink_t *) draw_داده;
  إذا (unlikely (!s->begin_command ())) أرجع;
  hb_وکتور_pdf_مسیر_append_xy (s, مقصد_x, مقصد_y);
  s->مسیر->append_str (" m\n");
  s->انتها_command ();
}

ساكن فراغ
hb_وکتور_pdf_مسیر_سطر_مقصد (hb_draw_funcs_t *, فراغ *draw_داده, hb_draw_state_t *,
			     عائم مقصد_x, عائم مقصد_y, فراغ *)
{
  خودکار *s = (hb_وکتور_مسیر_sink_t *) draw_داده;
  إذا (unlikely (!s->begin_command ())) أرجع;
  hb_وکتور_pdf_مسیر_append_xy (s, مقصد_x, مقصد_y);
  s->مسیر->append_str (" l\n");
  s->انتها_command ();
}

/* No quadratic_to — the null fallback auto-promotes to cubic. */

ساكن فراغ
hb_وکتور_pdf_مسیر_cubic_مقصد (hb_draw_funcs_t *, فراغ *draw_داده, hb_draw_state_t *,
			      عائم c1x, عائم c1y, عائم c2x, عائم c2y,
			      عائم مقصد_x, عائم مقصد_y, فراغ *)
{
  خودکار *s = (hb_وکتور_مسیر_sink_t *) draw_داده;
  إذا (unlikely (!s->begin_command ())) أرجع;
  hb_وکتور_pdf_مسیر_append_xy (s, c1x, c1y);
  s->مسیر->append_c (' ');
  hb_وکتور_pdf_مسیر_append_xy (s, c2x, c2y);
  s->مسیر->append_c (' ');
  hb_وکتور_pdf_مسیر_append_xy (s, مقصد_x, مقصد_y);
  s->مسیر->append_str (" c\n");
  s->انتها_command ();
}

ساكن فراغ
hb_وکتور_pdf_مسیر_close_مسیر (hb_draw_funcs_t *, فراغ *draw_داده, hb_draw_state_t *, فراغ *)
{
  خودکار *s = (hb_وکتور_مسیر_sink_t *) draw_داده;
  إذا (unlikely (!s->begin_command ())) أرجع;
  s->مسیر->append_str ("h\n");
  s->انتها_command ();
}

ساكن int64_t *
hb_وکتور_مسیر_get_budget_مانده (hb_draw_funcs_t *, فراغ *draw_داده, فراغ *)
{
  أرجع ((hb_وکتور_مسیر_sink_t *) draw_داده)->budget;
}


/* ---- Lazy loaders ---- */

ساكن مضمن فراغ free_static_وکتور_svg_مسیر_draw_funcs ();
ساكن بنية hb_وکتور_svg_مسیر_draw_funcs_lazy_loader_t
  : hb_draw_funcs_lazy_loader_t<hb_وکتور_svg_مسیر_draw_funcs_lazy_loader_t>
{
  ساكن hb_draw_funcs_t *create ()
  {
    hb_draw_funcs_t *funcs = hb_draw_funcs_create ();
    hb_draw_funcs_مجموعه_move_مقصد_func (funcs, (hb_draw_move_مقصد_func_t) hb_وکتور_svg_مسیر_move_مقصد, اشاره_تهی, اشاره_تهی);
    hb_draw_funcs_مجموعه_سطر_مقصد_func (funcs, (hb_draw_سطر_مقصد_func_t) hb_وکتور_svg_مسیر_سطر_مقصد, اشاره_تهی, اشاره_تهی);
    hb_draw_funcs_مجموعه_quadratic_مقصد_func (funcs, (hb_draw_quadratic_مقصد_func_t) hb_وکتور_svg_مسیر_quadratic_مقصد, اشاره_تهی, اشاره_تهی);
    hb_draw_funcs_مجموعه_cubic_مقصد_func (funcs, (hb_draw_cubic_مقصد_func_t) hb_وکتور_svg_مسیر_cubic_مقصد, اشاره_تهی, اشاره_تهی);
    hb_draw_funcs_مجموعه_close_مسیر_func (funcs, (hb_draw_close_مسیر_func_t) hb_وکتور_svg_مسیر_close_مسیر, اشاره_تهی, اشاره_تهی);
    hb_draw_funcs_مجموعه_get_budget_مانده_func (funcs, hb_وکتور_مسیر_get_budget_مانده, اشاره_تهی, اشاره_تهی);
    hb_draw_funcs_make_immutable (funcs);
    hb_atexit (free_static_وکتور_svg_مسیر_draw_funcs);
    أرجع funcs;
  }
} static_وکتور_svg_مسیر_draw_funcs;

ساكن مضمن فراغ
free_static_وکتور_svg_مسیر_draw_funcs ()
{ static_وکتور_svg_مسیر_draw_funcs.free_instance (); }

hb_draw_funcs_t *
hb_وکتور_svg_مسیر_draw_funcs_get ()
{ أرجع static_وکتور_svg_مسیر_draw_funcs.get_unconst (); }


ساكن مضمن فراغ free_static_وکتور_pdf_مسیر_draw_funcs ();
ساكن بنية hb_وکتور_pdf_مسیر_draw_funcs_lazy_loader_t
  : hb_draw_funcs_lazy_loader_t<hb_وکتور_pdf_مسیر_draw_funcs_lazy_loader_t>
{
  ساكن hb_draw_funcs_t *create ()
  {
    hb_draw_funcs_t *funcs = hb_draw_funcs_create ();
    hb_draw_funcs_مجموعه_move_مقصد_func (funcs, (hb_draw_move_مقصد_func_t) hb_وکتور_pdf_مسیر_move_مقصد, اشاره_تهی, اشاره_تهی);
    hb_draw_funcs_مجموعه_سطر_مقصد_func (funcs, (hb_draw_سطر_مقصد_func_t) hb_وکتور_pdf_مسیر_سطر_مقصد, اشاره_تهی, اشاره_تهی);
    /* No quadratic_to: the null fallback auto-promotes to cubic. */
    hb_draw_funcs_مجموعه_cubic_مقصد_func (funcs, (hb_draw_cubic_مقصد_func_t) hb_وکتور_pdf_مسیر_cubic_مقصد, اشاره_تهی, اشاره_تهی);
    hb_draw_funcs_مجموعه_close_مسیر_func (funcs, (hb_draw_close_مسیر_func_t) hb_وکتور_pdf_مسیر_close_مسیر, اشاره_تهی, اشاره_تهی);
    hb_draw_funcs_مجموعه_get_budget_مانده_func (funcs, hb_وکتور_مسیر_get_budget_مانده, اشاره_تهی, اشاره_تهی);
    hb_draw_funcs_make_immutable (funcs);
    hb_atexit (free_static_وکتور_pdf_مسیر_draw_funcs);
    أرجع funcs;
  }
} static_وکتور_pdf_مسیر_draw_funcs;

ساكن مضمن فراغ
free_static_وکتور_pdf_مسیر_draw_funcs ()
{ static_وکتور_pdf_مسیر_draw_funcs.free_instance (); }

hb_draw_funcs_t *
hb_وکتور_pdf_مسیر_draw_funcs_get ()
{ أرجع static_وکتور_pdf_مسیر_draw_funcs.get_unconst (); }


hb_draw_funcs_t *
hb_وکتور_مسیر_draw_funcs_get (hb_وکتور_format_t format)
{
  بدل (format)
  {
    حالة HB_وکتور_FORMAT_SVG: أرجع hb_وکتور_svg_مسیر_draw_funcs_get ();
    حالة HB_وکتور_FORMAT_PDF: أرجع hb_وکتور_pdf_مسیر_draw_funcs_get ();
    حالة HB_وکتور_FORMAT_INVALID: افتراضي: أرجع اشاره_تهی;
  }
}
