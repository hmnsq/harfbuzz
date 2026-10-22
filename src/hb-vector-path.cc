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
hb_ویکټور_svg_لار_append_xy (hb_ویکټور_لار_sink_t *s, عائم x, عائم y)
{
  s->لار->append_عدد (x / s->x_scale, s->precision);
  s->لار->append_c (',');
  s->لار->append_عدد (y / s->y_scale, s->precision);
}

ساكن فراغ
hb_ویکټور_svg_لار_move_منزل (hb_draw_funcs_t *, فراغ *draw_معلومات, hb_draw_state_t *,
			     عائم منزل_x, عائم منزل_y, فراغ *)
{
  خپلکاری *s = (hb_ویکټور_لار_sink_t *) draw_معلومات;
  إذا (unlikely (!s->begin_command ())) أرجع;
  s->لار->append_c ('M');
  hb_ویکټور_svg_لار_append_xy (s, منزل_x, منزل_y);
  s->پای_ټکی_command ();
}

ساكن فراغ
hb_ویکټور_svg_لار_کرښه_منزل (hb_draw_funcs_t *, فراغ *draw_معلومات, hb_draw_state_t *,
			     عائم منزل_x, عائم منزل_y, فراغ *)
{
  خپلکاری *s = (hb_ویکټور_لار_sink_t *) draw_معلومات;
  إذا (unlikely (!s->begin_command ())) أرجع;
  s->لار->append_c ('L');
  hb_ویکټور_svg_لار_append_xy (s, منزل_x, منزل_y);
  s->پای_ټکی_command ();
}

ساكن فراغ
hb_ویکټور_svg_لار_quadratic_منزل (hb_draw_funcs_t *, فراغ *draw_معلومات, hb_draw_state_t *,
				  عائم cx, عائم cy, عائم منزل_x, عائم منزل_y, فراغ *)
{
  خپلکاری *s = (hb_ویکټور_لار_sink_t *) draw_معلومات;
  إذا (unlikely (!s->begin_command ())) أرجع;
  s->لار->append_c ('Q');
  hb_ویکټور_svg_لار_append_xy (s, cx, cy);
  s->لار->append_c (' ');
  hb_ویکټور_svg_لار_append_xy (s, منزل_x, منزل_y);
  s->پای_ټکی_command ();
}

ساكن فراغ
hb_ویکټور_svg_لار_cubic_منزل (hb_draw_funcs_t *, فراغ *draw_معلومات, hb_draw_state_t *,
			      عائم c1x, عائم c1y, عائم c2x, عائم c2y,
			      عائم منزل_x, عائم منزل_y, فراغ *)
{
  خپلکاری *s = (hb_ویکټور_لار_sink_t *) draw_معلومات;
  إذا (unlikely (!s->begin_command ())) أرجع;
  s->لار->append_c ('C');
  hb_ویکټور_svg_لار_append_xy (s, c1x, c1y);
  s->لار->append_c (' ');
  hb_ویکټور_svg_لار_append_xy (s, c2x, c2y);
  s->لار->append_c (' ');
  hb_ویکټور_svg_لار_append_xy (s, منزل_x, منزل_y);
  s->پای_ټکی_command ();
}

ساكن فراغ
hb_ویکټور_svg_لار_close_لار (hb_draw_funcs_t *, فراغ *draw_معلومات, hb_draw_state_t *, فراغ *)
{
  خپلکاری *s = (hb_ویکټور_لار_sink_t *) draw_معلومات;
  إذا (unlikely (!s->begin_command ())) أرجع;
  s->لار->append_c ('Z');
  s->پای_ټکی_command ();
}


/* ---- PDF path callbacks ---- */

ساكن مضمن فراغ
hb_ویکټور_pdf_لار_append_xy (hb_ویکټور_لار_sink_t *s, عائم x, عائم y)
{
  s->لار->append_عدد (x / s->x_scale, s->precision);
  s->لار->append_c (' ');
  s->لار->append_عدد (y / s->y_scale, s->precision);
}

ساكن فراغ
hb_ویکټور_pdf_لار_move_منزل (hb_draw_funcs_t *, فراغ *draw_معلومات, hb_draw_state_t *,
			     عائم منزل_x, عائم منزل_y, فراغ *)
{
  خپلکاری *s = (hb_ویکټور_لار_sink_t *) draw_معلومات;
  إذا (unlikely (!s->begin_command ())) أرجع;
  hb_ویکټور_pdf_لار_append_xy (s, منزل_x, منزل_y);
  s->لار->append_str (" m\n");
  s->پای_ټکی_command ();
}

ساكن فراغ
hb_ویکټور_pdf_لار_کرښه_منزل (hb_draw_funcs_t *, فراغ *draw_معلومات, hb_draw_state_t *,
			     عائم منزل_x, عائم منزل_y, فراغ *)
{
  خپلکاری *s = (hb_ویکټور_لار_sink_t *) draw_معلومات;
  إذا (unlikely (!s->begin_command ())) أرجع;
  hb_ویکټور_pdf_لار_append_xy (s, منزل_x, منزل_y);
  s->لار->append_str (" l\n");
  s->پای_ټکی_command ();
}

/* No quadratic_to — the null fallback auto-promotes to cubic. */

ساكن فراغ
hb_ویکټور_pdf_لار_cubic_منزل (hb_draw_funcs_t *, فراغ *draw_معلومات, hb_draw_state_t *,
			      عائم c1x, عائم c1y, عائم c2x, عائم c2y,
			      عائم منزل_x, عائم منزل_y, فراغ *)
{
  خپلکاری *s = (hb_ویکټور_لار_sink_t *) draw_معلومات;
  إذا (unlikely (!s->begin_command ())) أرجع;
  hb_ویکټور_pdf_لار_append_xy (s, c1x, c1y);
  s->لار->append_c (' ');
  hb_ویکټور_pdf_لار_append_xy (s, c2x, c2y);
  s->لار->append_c (' ');
  hb_ویکټور_pdf_لار_append_xy (s, منزل_x, منزل_y);
  s->لار->append_str (" c\n");
  s->پای_ټکی_command ();
}

ساكن فراغ
hb_ویکټور_pdf_لار_close_لار (hb_draw_funcs_t *, فراغ *draw_معلومات, hb_draw_state_t *, فراغ *)
{
  خپلکاری *s = (hb_ویکټور_لار_sink_t *) draw_معلومات;
  إذا (unlikely (!s->begin_command ())) أرجع;
  s->لار->append_str ("h\n");
  s->پای_ټکی_command ();
}

ساكن int64_t *
hb_ویکټور_لار_get_budget_پاتې_شوی (hb_draw_funcs_t *, فراغ *draw_معلومات, فراغ *)
{
  أرجع ((hb_ویکټور_لار_sink_t *) draw_معلومات)->budget;
}


/* ---- Lazy loaders ---- */

ساكن مضمن فراغ free_static_ویکټور_svg_لار_draw_funcs ();
ساكن بنية hb_ویکټور_svg_لار_draw_funcs_lazy_loader_t
  : hb_draw_funcs_lazy_loader_t<hb_ویکټور_svg_لار_draw_funcs_lazy_loader_t>
{
  ساكن hb_draw_funcs_t *create ()
  {
    hb_draw_funcs_t *funcs = hb_draw_funcs_create ();
    hb_draw_funcs_سېټ_move_منزل_func (funcs, (hb_draw_move_منزل_func_t) hb_ویکټور_svg_لار_move_منزل, تش_اشاره, تش_اشاره);
    hb_draw_funcs_سېټ_کرښه_منزل_func (funcs, (hb_draw_کرښه_منزل_func_t) hb_ویکټور_svg_لار_کرښه_منزل, تش_اشاره, تش_اشاره);
    hb_draw_funcs_سېټ_quadratic_منزل_func (funcs, (hb_draw_quadratic_منزل_func_t) hb_ویکټور_svg_لار_quadratic_منزل, تش_اشاره, تش_اشاره);
    hb_draw_funcs_سېټ_cubic_منزل_func (funcs, (hb_draw_cubic_منزل_func_t) hb_ویکټور_svg_لار_cubic_منزل, تش_اشاره, تش_اشاره);
    hb_draw_funcs_سېټ_close_لار_func (funcs, (hb_draw_close_لار_func_t) hb_ویکټور_svg_لار_close_لار, تش_اشاره, تش_اشاره);
    hb_draw_funcs_سېټ_get_budget_پاتې_شوی_func (funcs, hb_ویکټور_لار_get_budget_پاتې_شوی, تش_اشاره, تش_اشاره);
    hb_draw_funcs_make_immutable (funcs);
    hb_atexit (free_static_ویکټور_svg_لار_draw_funcs);
    أرجع funcs;
  }
} static_ویکټور_svg_لار_draw_funcs;

ساكن مضمن فراغ
free_static_ویکټور_svg_لار_draw_funcs ()
{ static_ویکټور_svg_لار_draw_funcs.free_instance (); }

hb_draw_funcs_t *
hb_ویکټور_svg_لار_draw_funcs_get ()
{ أرجع static_ویکټور_svg_لار_draw_funcs.get_unconst (); }


ساكن مضمن فراغ free_static_ویکټور_pdf_لار_draw_funcs ();
ساكن بنية hb_ویکټور_pdf_لار_draw_funcs_lazy_loader_t
  : hb_draw_funcs_lazy_loader_t<hb_ویکټور_pdf_لار_draw_funcs_lazy_loader_t>
{
  ساكن hb_draw_funcs_t *create ()
  {
    hb_draw_funcs_t *funcs = hb_draw_funcs_create ();
    hb_draw_funcs_سېټ_move_منزل_func (funcs, (hb_draw_move_منزل_func_t) hb_ویکټور_pdf_لار_move_منزل, تش_اشاره, تش_اشاره);
    hb_draw_funcs_سېټ_کرښه_منزل_func (funcs, (hb_draw_کرښه_منزل_func_t) hb_ویکټور_pdf_لار_کرښه_منزل, تش_اشاره, تش_اشاره);
    /* No quadratic_to: the null fallback auto-promotes to cubic. */
    hb_draw_funcs_سېټ_cubic_منزل_func (funcs, (hb_draw_cubic_منزل_func_t) hb_ویکټور_pdf_لار_cubic_منزل, تش_اشاره, تش_اشاره);
    hb_draw_funcs_سېټ_close_لار_func (funcs, (hb_draw_close_لار_func_t) hb_ویکټور_pdf_لار_close_لار, تش_اشاره, تش_اشاره);
    hb_draw_funcs_سېټ_get_budget_پاتې_شوی_func (funcs, hb_ویکټور_لار_get_budget_پاتې_شوی, تش_اشاره, تش_اشاره);
    hb_draw_funcs_make_immutable (funcs);
    hb_atexit (free_static_ویکټور_pdf_لار_draw_funcs);
    أرجع funcs;
  }
} static_ویکټور_pdf_لار_draw_funcs;

ساكن مضمن فراغ
free_static_ویکټور_pdf_لار_draw_funcs ()
{ static_ویکټور_pdf_لار_draw_funcs.free_instance (); }

hb_draw_funcs_t *
hb_ویکټور_pdf_لار_draw_funcs_get ()
{ أرجع static_ویکټور_pdf_لار_draw_funcs.get_unconst (); }


hb_draw_funcs_t *
hb_ویکټور_لار_draw_funcs_get (hb_ویکټور_format_t format)
{
  بدل (format)
  {
    حالة HB_ویکټور_FORMAT_SVG: أرجع hb_ویکټور_svg_لار_draw_funcs_get ();
    حالة HB_ویکټور_FORMAT_PDF: أرجع hb_ویکټور_pdf_لار_draw_funcs_get ();
    حالة HB_ویکټور_FORMAT_INVALID: افتراضي: أرجع تش_اشاره;
  }
}
