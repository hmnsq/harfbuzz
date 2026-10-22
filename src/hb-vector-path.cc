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
hb_ويڪٽر_svg_رستو_append_xy (hb_ويڪٽر_رستو_sink_t *s, عائم x, عائم y)
{
  s->رستو->append_انگ (x / s->x_scale, s->precision);
  s->رستو->append_c (',');
  s->رستو->append_انگ (y / s->y_scale, s->precision);
}

ساكن فراغ
hb_ويڪٽر_svg_رستو_move_منزل (hb_draw_funcs_t *, فراغ *draw_معلومات, hb_draw_state_t *,
			     عائم منزل_x, عائم منزل_y, فراغ *)
{
  خودڪار *s = (hb_ويڪٽر_رستو_sink_t *) draw_معلومات;
  إذا (unlikely (!s->begin_command ())) أرجع;
  s->رستو->append_c ('M');
  hb_ويڪٽر_svg_رستو_append_xy (s, منزل_x, منزل_y);
  s->پڇاڙي_command ();
}

ساكن فراغ
hb_ويڪٽر_svg_رستو_سٽ_منزل (hb_draw_funcs_t *, فراغ *draw_معلومات, hb_draw_state_t *,
			     عائم منزل_x, عائم منزل_y, فراغ *)
{
  خودڪار *s = (hb_ويڪٽر_رستو_sink_t *) draw_معلومات;
  إذا (unlikely (!s->begin_command ())) أرجع;
  s->رستو->append_c ('L');
  hb_ويڪٽر_svg_رستو_append_xy (s, منزل_x, منزل_y);
  s->پڇاڙي_command ();
}

ساكن فراغ
hb_ويڪٽر_svg_رستو_quadratic_منزل (hb_draw_funcs_t *, فراغ *draw_معلومات, hb_draw_state_t *,
				  عائم cx, عائم cy, عائم منزل_x, عائم منزل_y, فراغ *)
{
  خودڪار *s = (hb_ويڪٽر_رستو_sink_t *) draw_معلومات;
  إذا (unlikely (!s->begin_command ())) أرجع;
  s->رستو->append_c ('Q');
  hb_ويڪٽر_svg_رستو_append_xy (s, cx, cy);
  s->رستو->append_c (' ');
  hb_ويڪٽر_svg_رستو_append_xy (s, منزل_x, منزل_y);
  s->پڇاڙي_command ();
}

ساكن فراغ
hb_ويڪٽر_svg_رستو_cubic_منزل (hb_draw_funcs_t *, فراغ *draw_معلومات, hb_draw_state_t *,
			      عائم c1x, عائم c1y, عائم c2x, عائم c2y,
			      عائم منزل_x, عائم منزل_y, فراغ *)
{
  خودڪار *s = (hb_ويڪٽر_رستو_sink_t *) draw_معلومات;
  إذا (unlikely (!s->begin_command ())) أرجع;
  s->رستو->append_c ('C');
  hb_ويڪٽر_svg_رستو_append_xy (s, c1x, c1y);
  s->رستو->append_c (' ');
  hb_ويڪٽر_svg_رستو_append_xy (s, c2x, c2y);
  s->رستو->append_c (' ');
  hb_ويڪٽر_svg_رستو_append_xy (s, منزل_x, منزل_y);
  s->پڇاڙي_command ();
}

ساكن فراغ
hb_ويڪٽر_svg_رستو_close_رستو (hb_draw_funcs_t *, فراغ *draw_معلومات, hb_draw_state_t *, فراغ *)
{
  خودڪار *s = (hb_ويڪٽر_رستو_sink_t *) draw_معلومات;
  إذا (unlikely (!s->begin_command ())) أرجع;
  s->رستو->append_c ('Z');
  s->پڇاڙي_command ();
}


/* ---- PDF path callbacks ---- */

ساكن مضمن فراغ
hb_ويڪٽر_pdf_رستو_append_xy (hb_ويڪٽر_رستو_sink_t *s, عائم x, عائم y)
{
  s->رستو->append_انگ (x / s->x_scale, s->precision);
  s->رستو->append_c (' ');
  s->رستو->append_انگ (y / s->y_scale, s->precision);
}

ساكن فراغ
hb_ويڪٽر_pdf_رستو_move_منزل (hb_draw_funcs_t *, فراغ *draw_معلومات, hb_draw_state_t *,
			     عائم منزل_x, عائم منزل_y, فراغ *)
{
  خودڪار *s = (hb_ويڪٽر_رستو_sink_t *) draw_معلومات;
  إذا (unlikely (!s->begin_command ())) أرجع;
  hb_ويڪٽر_pdf_رستو_append_xy (s, منزل_x, منزل_y);
  s->رستو->append_str (" m\n");
  s->پڇاڙي_command ();
}

ساكن فراغ
hb_ويڪٽر_pdf_رستو_سٽ_منزل (hb_draw_funcs_t *, فراغ *draw_معلومات, hb_draw_state_t *,
			     عائم منزل_x, عائم منزل_y, فراغ *)
{
  خودڪار *s = (hb_ويڪٽر_رستو_sink_t *) draw_معلومات;
  إذا (unlikely (!s->begin_command ())) أرجع;
  hb_ويڪٽر_pdf_رستو_append_xy (s, منزل_x, منزل_y);
  s->رستو->append_str (" l\n");
  s->پڇاڙي_command ();
}

/* No quadratic_to — the null fallback auto-promotes to cubic. */

ساكن فراغ
hb_ويڪٽر_pdf_رستو_cubic_منزل (hb_draw_funcs_t *, فراغ *draw_معلومات, hb_draw_state_t *,
			      عائم c1x, عائم c1y, عائم c2x, عائم c2y,
			      عائم منزل_x, عائم منزل_y, فراغ *)
{
  خودڪار *s = (hb_ويڪٽر_رستو_sink_t *) draw_معلومات;
  إذا (unlikely (!s->begin_command ())) أرجع;
  hb_ويڪٽر_pdf_رستو_append_xy (s, c1x, c1y);
  s->رستو->append_c (' ');
  hb_ويڪٽر_pdf_رستو_append_xy (s, c2x, c2y);
  s->رستو->append_c (' ');
  hb_ويڪٽر_pdf_رستو_append_xy (s, منزل_x, منزل_y);
  s->رستو->append_str (" c\n");
  s->پڇاڙي_command ();
}

ساكن فراغ
hb_ويڪٽر_pdf_رستو_close_رستو (hb_draw_funcs_t *, فراغ *draw_معلومات, hb_draw_state_t *, فراغ *)
{
  خودڪار *s = (hb_ويڪٽر_رستو_sink_t *) draw_معلومات;
  إذا (unlikely (!s->begin_command ())) أرجع;
  s->رستو->append_str ("h\n");
  s->پڇاڙي_command ();
}

ساكن int64_t *
hb_ويڪٽر_رستو_get_budget_بچيل (hb_draw_funcs_t *, فراغ *draw_معلومات, فراغ *)
{
  أرجع ((hb_ويڪٽر_رستو_sink_t *) draw_معلومات)->budget;
}


/* ---- Lazy loaders ---- */

ساكن مضمن فراغ free_static_ويڪٽر_svg_رستو_draw_funcs ();
ساكن بنية hb_ويڪٽر_svg_رستو_draw_funcs_lazy_loader_t
  : hb_draw_funcs_lazy_loader_t<hb_ويڪٽر_svg_رستو_draw_funcs_lazy_loader_t>
{
  ساكن hb_draw_funcs_t *create ()
  {
    hb_draw_funcs_t *funcs = hb_draw_funcs_create ();
    hb_draw_funcs_سيٽ_move_منزل_func (funcs, (hb_draw_move_منزل_func_t) hb_ويڪٽر_svg_رستو_move_منزل, خالي_اشارو, خالي_اشارو);
    hb_draw_funcs_سيٽ_سٽ_منزل_func (funcs, (hb_draw_سٽ_منزل_func_t) hb_ويڪٽر_svg_رستو_سٽ_منزل, خالي_اشارو, خالي_اشارو);
    hb_draw_funcs_سيٽ_quadratic_منزل_func (funcs, (hb_draw_quadratic_منزل_func_t) hb_ويڪٽر_svg_رستو_quadratic_منزل, خالي_اشارو, خالي_اشارو);
    hb_draw_funcs_سيٽ_cubic_منزل_func (funcs, (hb_draw_cubic_منزل_func_t) hb_ويڪٽر_svg_رستو_cubic_منزل, خالي_اشارو, خالي_اشارو);
    hb_draw_funcs_سيٽ_close_رستو_func (funcs, (hb_draw_close_رستو_func_t) hb_ويڪٽر_svg_رستو_close_رستو, خالي_اشارو, خالي_اشارو);
    hb_draw_funcs_سيٽ_get_budget_بچيل_func (funcs, hb_ويڪٽر_رستو_get_budget_بچيل, خالي_اشارو, خالي_اشارو);
    hb_draw_funcs_make_immutable (funcs);
    hb_atexit (free_static_ويڪٽر_svg_رستو_draw_funcs);
    أرجع funcs;
  }
} static_ويڪٽر_svg_رستو_draw_funcs;

ساكن مضمن فراغ
free_static_ويڪٽر_svg_رستو_draw_funcs ()
{ static_ويڪٽر_svg_رستو_draw_funcs.free_instance (); }

hb_draw_funcs_t *
hb_ويڪٽر_svg_رستو_draw_funcs_get ()
{ أرجع static_ويڪٽر_svg_رستو_draw_funcs.get_unconst (); }


ساكن مضمن فراغ free_static_ويڪٽر_pdf_رستو_draw_funcs ();
ساكن بنية hb_ويڪٽر_pdf_رستو_draw_funcs_lazy_loader_t
  : hb_draw_funcs_lazy_loader_t<hb_ويڪٽر_pdf_رستو_draw_funcs_lazy_loader_t>
{
  ساكن hb_draw_funcs_t *create ()
  {
    hb_draw_funcs_t *funcs = hb_draw_funcs_create ();
    hb_draw_funcs_سيٽ_move_منزل_func (funcs, (hb_draw_move_منزل_func_t) hb_ويڪٽر_pdf_رستو_move_منزل, خالي_اشارو, خالي_اشارو);
    hb_draw_funcs_سيٽ_سٽ_منزل_func (funcs, (hb_draw_سٽ_منزل_func_t) hb_ويڪٽر_pdf_رستو_سٽ_منزل, خالي_اشارو, خالي_اشارو);
    /* No quadratic_to: the null fallback auto-promotes to cubic. */
    hb_draw_funcs_سيٽ_cubic_منزل_func (funcs, (hb_draw_cubic_منزل_func_t) hb_ويڪٽر_pdf_رستو_cubic_منزل, خالي_اشارو, خالي_اشارو);
    hb_draw_funcs_سيٽ_close_رستو_func (funcs, (hb_draw_close_رستو_func_t) hb_ويڪٽر_pdf_رستو_close_رستو, خالي_اشارو, خالي_اشارو);
    hb_draw_funcs_سيٽ_get_budget_بچيل_func (funcs, hb_ويڪٽر_رستو_get_budget_بچيل, خالي_اشارو, خالي_اشارو);
    hb_draw_funcs_make_immutable (funcs);
    hb_atexit (free_static_ويڪٽر_pdf_رستو_draw_funcs);
    أرجع funcs;
  }
} static_ويڪٽر_pdf_رستو_draw_funcs;

ساكن مضمن فراغ
free_static_ويڪٽر_pdf_رستو_draw_funcs ()
{ static_ويڪٽر_pdf_رستو_draw_funcs.free_instance (); }

hb_draw_funcs_t *
hb_ويڪٽر_pdf_رستو_draw_funcs_get ()
{ أرجع static_ويڪٽر_pdf_رستو_draw_funcs.get_unconst (); }


hb_draw_funcs_t *
hb_ويڪٽر_رستو_draw_funcs_get (hb_ويڪٽر_format_t format)
{
  بدل (format)
  {
    حالة HB_ويڪٽر_FORMAT_SVG: أرجع hb_ويڪٽر_svg_رستو_draw_funcs_get ();
    حالة HB_ويڪٽر_FORMAT_PDF: أرجع hb_ويڪٽر_pdf_رستو_draw_funcs_get ();
    حالة HB_ويڪٽر_FORMAT_INVALID: افتراضي: أرجع خالي_اشارو;
  }
}
