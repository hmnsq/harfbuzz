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

#شامل "hb-geometry.hh"
#شامل "hb-machinery.hh"
#شامل "hb-vector-path.hh"
#شامل "hb-vector-buf.hh"
#شامل "hb-vector-internal.hh"


/* ---- SVG path callbacks ---- */

ساکن اندرونی خالی
hb_سمتیہ_svg_راستہ_append_xy (hb_سمتیہ_راستہ_sink_t *s, اعشاری x, اعشاری y)
{
  s->راستہ->append_عدد (x / s->x_scale, s->precision);
  s->راستہ->append_c (',');
  s->راستہ->append_عدد (y / s->y_scale, s->precision);
}

ساکن خالی
hb_سمتیہ_svg_راستہ_move_مقصد (hb_draw_funcs_t *, خالی *draw_ڈیٹا, hb_draw_state_t *,
			     اعشاری مقصد_x, اعشاری مقصد_y, خالی *)
{
  خودکار *s = (hb_سمتیہ_راستہ_sink_t *) draw_ڈیٹا;
  اگر (unlikely (!s->begin_command ())) واپس;
  s->راستہ->append_c ('M');
  hb_سمتیہ_svg_راستہ_append_xy (s, مقصد_x, مقصد_y);
  s->انتہا_command ();
}

ساکن خالی
hb_سمتیہ_svg_راستہ_سطر_مقصد (hb_draw_funcs_t *, خالی *draw_ڈیٹا, hb_draw_state_t *,
			     اعشاری مقصد_x, اعشاری مقصد_y, خالی *)
{
  خودکار *s = (hb_سمتیہ_راستہ_sink_t *) draw_ڈیٹا;
  اگر (unlikely (!s->begin_command ())) واپس;
  s->راستہ->append_c ('L');
  hb_سمتیہ_svg_راستہ_append_xy (s, مقصد_x, مقصد_y);
  s->انتہا_command ();
}

ساکن خالی
hb_سمتیہ_svg_راستہ_quadratic_مقصد (hb_draw_funcs_t *, خالی *draw_ڈیٹا, hb_draw_state_t *,
				  اعشاری cx, اعشاری cy, اعشاری مقصد_x, اعشاری مقصد_y, خالی *)
{
  خودکار *s = (hb_سمتیہ_راستہ_sink_t *) draw_ڈیٹا;
  اگر (unlikely (!s->begin_command ())) واپس;
  s->راستہ->append_c ('Q');
  hb_سمتیہ_svg_راستہ_append_xy (s, cx, cy);
  s->راستہ->append_c (' ');
  hb_سمتیہ_svg_راستہ_append_xy (s, مقصد_x, مقصد_y);
  s->انتہا_command ();
}

ساکن خالی
hb_سمتیہ_svg_راستہ_cubic_مقصد (hb_draw_funcs_t *, خالی *draw_ڈیٹا, hb_draw_state_t *,
			      اعشاری c1x, اعشاری c1y, اعشاری c2x, اعشاری c2y,
			      اعشاری مقصد_x, اعشاری مقصد_y, خالی *)
{
  خودکار *s = (hb_سمتیہ_راستہ_sink_t *) draw_ڈیٹا;
  اگر (unlikely (!s->begin_command ())) واپس;
  s->راستہ->append_c ('C');
  hb_سمتیہ_svg_راستہ_append_xy (s, c1x, c1y);
  s->راستہ->append_c (' ');
  hb_سمتیہ_svg_راستہ_append_xy (s, c2x, c2y);
  s->راستہ->append_c (' ');
  hb_سمتیہ_svg_راستہ_append_xy (s, مقصد_x, مقصد_y);
  s->انتہا_command ();
}

ساکن خالی
hb_سمتیہ_svg_راستہ_close_راستہ (hb_draw_funcs_t *, خالی *draw_ڈیٹا, hb_draw_state_t *, خالی *)
{
  خودکار *s = (hb_سمتیہ_راستہ_sink_t *) draw_ڈیٹا;
  اگر (unlikely (!s->begin_command ())) واپس;
  s->راستہ->append_c ('Z');
  s->انتہا_command ();
}


/* ---- PDF path callbacks ---- */

ساکن اندرونی خالی
hb_سمتیہ_pdf_راستہ_append_xy (hb_سمتیہ_راستہ_sink_t *s, اعشاری x, اعشاری y)
{
  s->راستہ->append_عدد (x / s->x_scale, s->precision);
  s->راستہ->append_c (' ');
  s->راستہ->append_عدد (y / s->y_scale, s->precision);
}

ساکن خالی
hb_سمتیہ_pdf_راستہ_move_مقصد (hb_draw_funcs_t *, خالی *draw_ڈیٹا, hb_draw_state_t *,
			     اعشاری مقصد_x, اعشاری مقصد_y, خالی *)
{
  خودکار *s = (hb_سمتیہ_راستہ_sink_t *) draw_ڈیٹا;
  اگر (unlikely (!s->begin_command ())) واپس;
  hb_سمتیہ_pdf_راستہ_append_xy (s, مقصد_x, مقصد_y);
  s->راستہ->append_str (" m\n");
  s->انتہا_command ();
}

ساکن خالی
hb_سمتیہ_pdf_راستہ_سطر_مقصد (hb_draw_funcs_t *, خالی *draw_ڈیٹا, hb_draw_state_t *,
			     اعشاری مقصد_x, اعشاری مقصد_y, خالی *)
{
  خودکار *s = (hb_سمتیہ_راستہ_sink_t *) draw_ڈیٹا;
  اگر (unlikely (!s->begin_command ())) واپس;
  hb_سمتیہ_pdf_راستہ_append_xy (s, مقصد_x, مقصد_y);
  s->راستہ->append_str (" l\n");
  s->انتہا_command ();
}

/* No quadratic_to — the null fallback auto-promotes to cubic. */

ساکن خالی
hb_سمتیہ_pdf_راستہ_cubic_مقصد (hb_draw_funcs_t *, خالی *draw_ڈیٹا, hb_draw_state_t *,
			      اعشاری c1x, اعشاری c1y, اعشاری c2x, اعشاری c2y,
			      اعشاری مقصد_x, اعشاری مقصد_y, خالی *)
{
  خودکار *s = (hb_سمتیہ_راستہ_sink_t *) draw_ڈیٹا;
  اگر (unlikely (!s->begin_command ())) واپس;
  hb_سمتیہ_pdf_راستہ_append_xy (s, c1x, c1y);
  s->راستہ->append_c (' ');
  hb_سمتیہ_pdf_راستہ_append_xy (s, c2x, c2y);
  s->راستہ->append_c (' ');
  hb_سمتیہ_pdf_راستہ_append_xy (s, مقصد_x, مقصد_y);
  s->راستہ->append_str (" c\n");
  s->انتہا_command ();
}

ساکن خالی
hb_سمتیہ_pdf_راستہ_close_راستہ (hb_draw_funcs_t *, خالی *draw_ڈیٹا, hb_draw_state_t *, خالی *)
{
  خودکار *s = (hb_سمتیہ_راستہ_sink_t *) draw_ڈیٹا;
  اگر (unlikely (!s->begin_command ())) واپس;
  s->راستہ->append_str ("h\n");
  s->انتہا_command ();
}

ساکن int64_t *
hb_سمتیہ_راستہ_get_budget_بقایا (hb_draw_funcs_t *, خالی *draw_ڈیٹا, خالی *)
{
  واپس ((hb_سمتیہ_راستہ_sink_t *) draw_ڈیٹا)->budget;
}


/* ---- Lazy loaders ---- */

ساکن اندرونی خالی free_static_سمتیہ_svg_راستہ_draw_funcs ();
ساکن ساخت hb_سمتیہ_svg_راستہ_draw_funcs_lazy_loader_t
  : hb_draw_funcs_lazy_loader_t<hb_سمتیہ_svg_راستہ_draw_funcs_lazy_loader_t>
{
  ساکن hb_draw_funcs_t *create ()
  {
    hb_draw_funcs_t *funcs = hb_draw_funcs_create ();
    hb_draw_funcs_سیٹ_move_مقصد_func (funcs, (hb_draw_move_مقصد_func_t) hb_سمتیہ_svg_راستہ_move_مقصد, خالی_اشاریہ, خالی_اشاریہ);
    hb_draw_funcs_سیٹ_سطر_مقصد_func (funcs, (hb_draw_سطر_مقصد_func_t) hb_سمتیہ_svg_راستہ_سطر_مقصد, خالی_اشاریہ, خالی_اشاریہ);
    hb_draw_funcs_سیٹ_quadratic_مقصد_func (funcs, (hb_draw_quadratic_مقصد_func_t) hb_سمتیہ_svg_راستہ_quadratic_مقصد, خالی_اشاریہ, خالی_اشاریہ);
    hb_draw_funcs_سیٹ_cubic_مقصد_func (funcs, (hb_draw_cubic_مقصد_func_t) hb_سمتیہ_svg_راستہ_cubic_مقصد, خالی_اشاریہ, خالی_اشاریہ);
    hb_draw_funcs_سیٹ_close_راستہ_func (funcs, (hb_draw_close_راستہ_func_t) hb_سمتیہ_svg_راستہ_close_راستہ, خالی_اشاریہ, خالی_اشاریہ);
    hb_draw_funcs_سیٹ_get_budget_بقایا_func (funcs, hb_سمتیہ_راستہ_get_budget_بقایا, خالی_اشاریہ, خالی_اشاریہ);
    hb_draw_funcs_make_immutable (funcs);
    hb_atexit (free_static_سمتیہ_svg_راستہ_draw_funcs);
    واپس funcs;
  }
} static_سمتیہ_svg_راستہ_draw_funcs;

ساکن اندرونی خالی
free_static_سمتیہ_svg_راستہ_draw_funcs ()
{ static_سمتیہ_svg_راستہ_draw_funcs.free_instance (); }

hb_draw_funcs_t *
hb_سمتیہ_svg_راستہ_draw_funcs_get ()
{ واپس static_سمتیہ_svg_راستہ_draw_funcs.get_unconst (); }


ساکن اندرونی خالی free_static_سمتیہ_pdf_راستہ_draw_funcs ();
ساکن ساخت hb_سمتیہ_pdf_راستہ_draw_funcs_lazy_loader_t
  : hb_draw_funcs_lazy_loader_t<hb_سمتیہ_pdf_راستہ_draw_funcs_lazy_loader_t>
{
  ساکن hb_draw_funcs_t *create ()
  {
    hb_draw_funcs_t *funcs = hb_draw_funcs_create ();
    hb_draw_funcs_سیٹ_move_مقصد_func (funcs, (hb_draw_move_مقصد_func_t) hb_سمتیہ_pdf_راستہ_move_مقصد, خالی_اشاریہ, خالی_اشاریہ);
    hb_draw_funcs_سیٹ_سطر_مقصد_func (funcs, (hb_draw_سطر_مقصد_func_t) hb_سمتیہ_pdf_راستہ_سطر_مقصد, خالی_اشاریہ, خالی_اشاریہ);
    /* No quadratic_to: the null fallback auto-promotes to cubic. */
    hb_draw_funcs_سیٹ_cubic_مقصد_func (funcs, (hb_draw_cubic_مقصد_func_t) hb_سمتیہ_pdf_راستہ_cubic_مقصد, خالی_اشاریہ, خالی_اشاریہ);
    hb_draw_funcs_سیٹ_close_راستہ_func (funcs, (hb_draw_close_راستہ_func_t) hb_سمتیہ_pdf_راستہ_close_راستہ, خالی_اشاریہ, خالی_اشاریہ);
    hb_draw_funcs_سیٹ_get_budget_بقایا_func (funcs, hb_سمتیہ_راستہ_get_budget_بقایا, خالی_اشاریہ, خالی_اشاریہ);
    hb_draw_funcs_make_immutable (funcs);
    hb_atexit (free_static_سمتیہ_pdf_راستہ_draw_funcs);
    واپس funcs;
  }
} static_سمتیہ_pdf_راستہ_draw_funcs;

ساکن اندرونی خالی
free_static_سمتیہ_pdf_راستہ_draw_funcs ()
{ static_سمتیہ_pdf_راستہ_draw_funcs.free_instance (); }

hb_draw_funcs_t *
hb_سمتیہ_pdf_راستہ_draw_funcs_get ()
{ واپس static_سمتیہ_pdf_راستہ_draw_funcs.get_unconst (); }


hb_draw_funcs_t *
hb_سمتیہ_راستہ_draw_funcs_get (hb_سمتیہ_format_t format)
{
  انتخاب (format)
  {
    حالت HB_سمتیہ_FORMAT_SVG: واپس hb_سمتیہ_svg_راستہ_draw_funcs_get ();
    حالت HB_سمتیہ_FORMAT_PDF: واپس hb_سمتیہ_pdf_راستہ_draw_funcs_get ();
    حالت HB_سمتیہ_FORMAT_INVALID: طےشدہ: واپس خالی_اشاریہ;
  }
}
