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
hb_سمتیہ_svg_رستہ_append_xy (hb_سمتیہ_رستہ_sink_t *s, عائم x, عائم y)
{
  s->رستہ->append_عدد (x / s->x_scale, s->precision);
  s->رستہ->append_c (',');
  s->رستہ->append_عدد (y / s->y_scale, s->precision);
}

ساكن فراغ
hb_سمتیہ_svg_رستہ_move_منزل (hb_draw_funcs_t *, فراغ *draw_انکڑے, hb_draw_state_t *,
			     عائم منزل_x, عائم منزل_y, فراغ *)
{
  خودکار *s = (hb_سمتیہ_رستہ_sink_t *) draw_انکڑے;
  إذا (unlikely (!s->begin_command ())) أرجع;
  s->رستہ->append_c ('M');
  hb_سمتیہ_svg_رستہ_append_xy (s, منزل_x, منزل_y);
  s->سرا_command ();
}

ساكن فراغ
hb_سمتیہ_svg_رستہ_سطر_منزل (hb_draw_funcs_t *, فراغ *draw_انکڑے, hb_draw_state_t *,
			     عائم منزل_x, عائم منزل_y, فراغ *)
{
  خودکار *s = (hb_سمتیہ_رستہ_sink_t *) draw_انکڑے;
  إذا (unlikely (!s->begin_command ())) أرجع;
  s->رستہ->append_c ('L');
  hb_سمتیہ_svg_رستہ_append_xy (s, منزل_x, منزل_y);
  s->سرا_command ();
}

ساكن فراغ
hb_سمتیہ_svg_رستہ_quadratic_منزل (hb_draw_funcs_t *, فراغ *draw_انکڑے, hb_draw_state_t *,
				  عائم cx, عائم cy, عائم منزل_x, عائم منزل_y, فراغ *)
{
  خودکار *s = (hb_سمتیہ_رستہ_sink_t *) draw_انکڑے;
  إذا (unlikely (!s->begin_command ())) أرجع;
  s->رستہ->append_c ('Q');
  hb_سمتیہ_svg_رستہ_append_xy (s, cx, cy);
  s->رستہ->append_c (' ');
  hb_سمتیہ_svg_رستہ_append_xy (s, منزل_x, منزل_y);
  s->سرا_command ();
}

ساكن فراغ
hb_سمتیہ_svg_رستہ_cubic_منزل (hb_draw_funcs_t *, فراغ *draw_انکڑے, hb_draw_state_t *,
			      عائم c1x, عائم c1y, عائم c2x, عائم c2y,
			      عائم منزل_x, عائم منزل_y, فراغ *)
{
  خودکار *s = (hb_سمتیہ_رستہ_sink_t *) draw_انکڑے;
  إذا (unlikely (!s->begin_command ())) أرجع;
  s->رستہ->append_c ('C');
  hb_سمتیہ_svg_رستہ_append_xy (s, c1x, c1y);
  s->رستہ->append_c (' ');
  hb_سمتیہ_svg_رستہ_append_xy (s, c2x, c2y);
  s->رستہ->append_c (' ');
  hb_سمتیہ_svg_رستہ_append_xy (s, منزل_x, منزل_y);
  s->سرا_command ();
}

ساكن فراغ
hb_سمتیہ_svg_رستہ_close_رستہ (hb_draw_funcs_t *, فراغ *draw_انکڑے, hb_draw_state_t *, فراغ *)
{
  خودکار *s = (hb_سمتیہ_رستہ_sink_t *) draw_انکڑے;
  إذا (unlikely (!s->begin_command ())) أرجع;
  s->رستہ->append_c ('Z');
  s->سرا_command ();
}


/* ---- PDF path callbacks ---- */

ساكن مضمن فراغ
hb_سمتیہ_pdf_رستہ_append_xy (hb_سمتیہ_رستہ_sink_t *s, عائم x, عائم y)
{
  s->رستہ->append_عدد (x / s->x_scale, s->precision);
  s->رستہ->append_c (' ');
  s->رستہ->append_عدد (y / s->y_scale, s->precision);
}

ساكن فراغ
hb_سمتیہ_pdf_رستہ_move_منزل (hb_draw_funcs_t *, فراغ *draw_انکڑے, hb_draw_state_t *,
			     عائم منزل_x, عائم منزل_y, فراغ *)
{
  خودکار *s = (hb_سمتیہ_رستہ_sink_t *) draw_انکڑے;
  إذا (unlikely (!s->begin_command ())) أرجع;
  hb_سمتیہ_pdf_رستہ_append_xy (s, منزل_x, منزل_y);
  s->رستہ->append_str (" m\n");
  s->سرا_command ();
}

ساكن فراغ
hb_سمتیہ_pdf_رستہ_سطر_منزل (hb_draw_funcs_t *, فراغ *draw_انکڑے, hb_draw_state_t *,
			     عائم منزل_x, عائم منزل_y, فراغ *)
{
  خودکار *s = (hb_سمتیہ_رستہ_sink_t *) draw_انکڑے;
  إذا (unlikely (!s->begin_command ())) أرجع;
  hb_سمتیہ_pdf_رستہ_append_xy (s, منزل_x, منزل_y);
  s->رستہ->append_str (" l\n");
  s->سرا_command ();
}

/* No quadratic_to — the null fallback auto-promotes to cubic. */

ساكن فراغ
hb_سمتیہ_pdf_رستہ_cubic_منزل (hb_draw_funcs_t *, فراغ *draw_انکڑے, hb_draw_state_t *,
			      عائم c1x, عائم c1y, عائم c2x, عائم c2y,
			      عائم منزل_x, عائم منزل_y, فراغ *)
{
  خودکار *s = (hb_سمتیہ_رستہ_sink_t *) draw_انکڑے;
  إذا (unlikely (!s->begin_command ())) أرجع;
  hb_سمتیہ_pdf_رستہ_append_xy (s, c1x, c1y);
  s->رستہ->append_c (' ');
  hb_سمتیہ_pdf_رستہ_append_xy (s, c2x, c2y);
  s->رستہ->append_c (' ');
  hb_سمتیہ_pdf_رستہ_append_xy (s, منزل_x, منزل_y);
  s->رستہ->append_str (" c\n");
  s->سرا_command ();
}

ساكن فراغ
hb_سمتیہ_pdf_رستہ_close_رستہ (hb_draw_funcs_t *, فراغ *draw_انکڑے, hb_draw_state_t *, فراغ *)
{
  خودکار *s = (hb_سمتیہ_رستہ_sink_t *) draw_انکڑے;
  إذا (unlikely (!s->begin_command ())) أرجع;
  s->رستہ->append_str ("h\n");
  s->سرا_command ();
}

ساكن int64_t *
hb_سمتیہ_رستہ_get_budget_بچیا (hb_draw_funcs_t *, فراغ *draw_انکڑے, فراغ *)
{
  أرجع ((hb_سمتیہ_رستہ_sink_t *) draw_انکڑے)->budget;
}


/* ---- Lazy loaders ---- */

ساكن مضمن فراغ free_static_سمتیہ_svg_رستہ_draw_funcs ();
ساكن بنية hb_سمتیہ_svg_رستہ_draw_funcs_lazy_loader_t
  : hb_draw_funcs_lazy_loader_t<hb_سمتیہ_svg_رستہ_draw_funcs_lazy_loader_t>
{
  ساكن hb_draw_funcs_t *create ()
  {
    hb_draw_funcs_t *funcs = hb_draw_funcs_create ();
    hb_draw_funcs_سیٹ_move_منزل_func (funcs, (hb_draw_move_منزل_func_t) hb_سمتیہ_svg_رستہ_move_منزل, خالی_سوچک, خالی_سوچک);
    hb_draw_funcs_سیٹ_سطر_منزل_func (funcs, (hb_draw_سطر_منزل_func_t) hb_سمتیہ_svg_رستہ_سطر_منزل, خالی_سوچک, خالی_سوچک);
    hb_draw_funcs_سیٹ_quadratic_منزل_func (funcs, (hb_draw_quadratic_منزل_func_t) hb_سمتیہ_svg_رستہ_quadratic_منزل, خالی_سوچک, خالی_سوچک);
    hb_draw_funcs_سیٹ_cubic_منزل_func (funcs, (hb_draw_cubic_منزل_func_t) hb_سمتیہ_svg_رستہ_cubic_منزل, خالی_سوچک, خالی_سوچک);
    hb_draw_funcs_سیٹ_close_رستہ_func (funcs, (hb_draw_close_رستہ_func_t) hb_سمتیہ_svg_رستہ_close_رستہ, خالی_سوچک, خالی_سوچک);
    hb_draw_funcs_سیٹ_get_budget_بچیا_func (funcs, hb_سمتیہ_رستہ_get_budget_بچیا, خالی_سوچک, خالی_سوچک);
    hb_draw_funcs_make_immutable (funcs);
    hb_atexit (free_static_سمتیہ_svg_رستہ_draw_funcs);
    أرجع funcs;
  }
} static_سمتیہ_svg_رستہ_draw_funcs;

ساكن مضمن فراغ
free_static_سمتیہ_svg_رستہ_draw_funcs ()
{ static_سمتیہ_svg_رستہ_draw_funcs.free_instance (); }

hb_draw_funcs_t *
hb_سمتیہ_svg_رستہ_draw_funcs_get ()
{ أرجع static_سمتیہ_svg_رستہ_draw_funcs.get_unconst (); }


ساكن مضمن فراغ free_static_سمتیہ_pdf_رستہ_draw_funcs ();
ساكن بنية hb_سمتیہ_pdf_رستہ_draw_funcs_lazy_loader_t
  : hb_draw_funcs_lazy_loader_t<hb_سمتیہ_pdf_رستہ_draw_funcs_lazy_loader_t>
{
  ساكن hb_draw_funcs_t *create ()
  {
    hb_draw_funcs_t *funcs = hb_draw_funcs_create ();
    hb_draw_funcs_سیٹ_move_منزل_func (funcs, (hb_draw_move_منزل_func_t) hb_سمتیہ_pdf_رستہ_move_منزل, خالی_سوچک, خالی_سوچک);
    hb_draw_funcs_سیٹ_سطر_منزل_func (funcs, (hb_draw_سطر_منزل_func_t) hb_سمتیہ_pdf_رستہ_سطر_منزل, خالی_سوچک, خالی_سوچک);
    /* No quadratic_to: the null fallback auto-promotes to cubic. */
    hb_draw_funcs_سیٹ_cubic_منزل_func (funcs, (hb_draw_cubic_منزل_func_t) hb_سمتیہ_pdf_رستہ_cubic_منزل, خالی_سوچک, خالی_سوچک);
    hb_draw_funcs_سیٹ_close_رستہ_func (funcs, (hb_draw_close_رستہ_func_t) hb_سمتیہ_pdf_رستہ_close_رستہ, خالی_سوچک, خالی_سوچک);
    hb_draw_funcs_سیٹ_get_budget_بچیا_func (funcs, hb_سمتیہ_رستہ_get_budget_بچیا, خالی_سوچک, خالی_سوچک);
    hb_draw_funcs_make_immutable (funcs);
    hb_atexit (free_static_سمتیہ_pdf_رستہ_draw_funcs);
    أرجع funcs;
  }
} static_سمتیہ_pdf_رستہ_draw_funcs;

ساكن مضمن فراغ
free_static_سمتیہ_pdf_رستہ_draw_funcs ()
{ static_سمتیہ_pdf_رستہ_draw_funcs.free_instance (); }

hb_draw_funcs_t *
hb_سمتیہ_pdf_رستہ_draw_funcs_get ()
{ أرجع static_سمتیہ_pdf_رستہ_draw_funcs.get_unconst (); }


hb_draw_funcs_t *
hb_سمتیہ_رستہ_draw_funcs_get (hb_سمتیہ_format_t format)
{
  بدل (format)
  {
    حالة HB_سمتیہ_FORMAT_SVG: أرجع hb_سمتیہ_svg_رستہ_draw_funcs_get ();
    حالة HB_سمتیہ_FORMAT_PDF: أرجع hb_سمتیہ_pdf_رستہ_draw_funcs_get ();
    حالة HB_سمتیہ_FORMAT_INVALID: افتراضي: أرجع خالی_سوچک;
  }
}
