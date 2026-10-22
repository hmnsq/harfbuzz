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

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-geometry.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-machinery.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-vector-path.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-vector-buf.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-vector-internal.hh"


/* ---- SVG path callbacks ---- */

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
hb_ਸਦਿਸ਼_svg_ਰਸਤਾ_append_xy (hb_ਸਦਿਸ਼_ਰਸਤਾ_sink_t *s, ਦਸ਼ਮਲਵ x, ਦਸ਼ਮਲਵ y)
{
  s->ਰਸਤਾ->append_ਅੰਕ (x / s->x_scale, s->precision);
  s->ਰਸਤਾ->append_c (',');
  s->ਰਸਤਾ->append_ਅੰਕ (y / s->y_scale, s->precision);
}

ਸਥਿਰ ਖਾਲੀ
hb_ਸਦਿਸ਼_svg_ਰਸਤਾ_move_ਮੰਜ਼ਿਲ (hb_draw_funcs_t *, ਖਾਲੀ *draw_ਅੰਕੜੇ, hb_draw_state_t *,
			     ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_x, ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_y, ਖਾਲੀ *)
{
  ਸਵੈਚਾਲਿਤ *s = (hb_ਸਦਿਸ਼_ਰਸਤਾ_sink_t *) draw_ਅੰਕੜੇ;
  ਜੇ (unlikely (!s->begin_command ())) ਵਾਪਸ;
  s->ਰਸਤਾ->append_c ('M');
  hb_ਸਦਿਸ਼_svg_ਰਸਤਾ_append_xy (s, ਮੰਜ਼ਿਲ_x, ਮੰਜ਼ਿਲ_y);
  s->ਸਿਰਾ_command ();
}

ਸਥਿਰ ਖਾਲੀ
hb_ਸਦਿਸ਼_svg_ਰਸਤਾ_ਸਤਰ_ਮੰਜ਼ਿਲ (hb_draw_funcs_t *, ਖਾਲੀ *draw_ਅੰਕੜੇ, hb_draw_state_t *,
			     ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_x, ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_y, ਖਾਲੀ *)
{
  ਸਵੈਚਾਲਿਤ *s = (hb_ਸਦਿਸ਼_ਰਸਤਾ_sink_t *) draw_ਅੰਕੜੇ;
  ਜੇ (unlikely (!s->begin_command ())) ਵਾਪਸ;
  s->ਰਸਤਾ->append_c ('L');
  hb_ਸਦਿਸ਼_svg_ਰਸਤਾ_append_xy (s, ਮੰਜ਼ਿਲ_x, ਮੰਜ਼ਿਲ_y);
  s->ਸਿਰਾ_command ();
}

ਸਥਿਰ ਖਾਲੀ
hb_ਸਦਿਸ਼_svg_ਰਸਤਾ_quadratic_ਮੰਜ਼ਿਲ (hb_draw_funcs_t *, ਖਾਲੀ *draw_ਅੰਕੜੇ, hb_draw_state_t *,
				  ਦਸ਼ਮਲਵ cx, ਦਸ਼ਮਲਵ cy, ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_x, ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_y, ਖਾਲੀ *)
{
  ਸਵੈਚਾਲਿਤ *s = (hb_ਸਦਿਸ਼_ਰਸਤਾ_sink_t *) draw_ਅੰਕੜੇ;
  ਜੇ (unlikely (!s->begin_command ())) ਵਾਪਸ;
  s->ਰਸਤਾ->append_c ('Q');
  hb_ਸਦਿਸ਼_svg_ਰਸਤਾ_append_xy (s, cx, cy);
  s->ਰਸਤਾ->append_c (' ');
  hb_ਸਦਿਸ਼_svg_ਰਸਤਾ_append_xy (s, ਮੰਜ਼ਿਲ_x, ਮੰਜ਼ਿਲ_y);
  s->ਸਿਰਾ_command ();
}

ਸਥਿਰ ਖਾਲੀ
hb_ਸਦਿਸ਼_svg_ਰਸਤਾ_cubic_ਮੰਜ਼ਿਲ (hb_draw_funcs_t *, ਖਾਲੀ *draw_ਅੰਕੜੇ, hb_draw_state_t *,
			      ਦਸ਼ਮਲਵ c1x, ਦਸ਼ਮਲਵ c1y, ਦਸ਼ਮਲਵ c2x, ਦਸ਼ਮਲਵ c2y,
			      ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_x, ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_y, ਖਾਲੀ *)
{
  ਸਵੈਚਾਲਿਤ *s = (hb_ਸਦਿਸ਼_ਰਸਤਾ_sink_t *) draw_ਅੰਕੜੇ;
  ਜੇ (unlikely (!s->begin_command ())) ਵਾਪਸ;
  s->ਰਸਤਾ->append_c ('C');
  hb_ਸਦਿਸ਼_svg_ਰਸਤਾ_append_xy (s, c1x, c1y);
  s->ਰਸਤਾ->append_c (' ');
  hb_ਸਦਿਸ਼_svg_ਰਸਤਾ_append_xy (s, c2x, c2y);
  s->ਰਸਤਾ->append_c (' ');
  hb_ਸਦਿਸ਼_svg_ਰਸਤਾ_append_xy (s, ਮੰਜ਼ਿਲ_x, ਮੰਜ਼ਿਲ_y);
  s->ਸਿਰਾ_command ();
}

ਸਥਿਰ ਖਾਲੀ
hb_ਸਦਿਸ਼_svg_ਰਸਤਾ_close_ਰਸਤਾ (hb_draw_funcs_t *, ਖਾਲੀ *draw_ਅੰਕੜੇ, hb_draw_state_t *, ਖਾਲੀ *)
{
  ਸਵੈਚਾਲਿਤ *s = (hb_ਸਦਿਸ਼_ਰਸਤਾ_sink_t *) draw_ਅੰਕੜੇ;
  ਜੇ (unlikely (!s->begin_command ())) ਵਾਪਸ;
  s->ਰਸਤਾ->append_c ('Z');
  s->ਸਿਰਾ_command ();
}


/* ---- PDF path callbacks ---- */

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
hb_ਸਦਿਸ਼_pdf_ਰਸਤਾ_append_xy (hb_ਸਦਿਸ਼_ਰਸਤਾ_sink_t *s, ਦਸ਼ਮਲਵ x, ਦਸ਼ਮਲਵ y)
{
  s->ਰਸਤਾ->append_ਅੰਕ (x / s->x_scale, s->precision);
  s->ਰਸਤਾ->append_c (' ');
  s->ਰਸਤਾ->append_ਅੰਕ (y / s->y_scale, s->precision);
}

ਸਥਿਰ ਖਾਲੀ
hb_ਸਦਿਸ਼_pdf_ਰਸਤਾ_move_ਮੰਜ਼ਿਲ (hb_draw_funcs_t *, ਖਾਲੀ *draw_ਅੰਕੜੇ, hb_draw_state_t *,
			     ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_x, ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_y, ਖਾਲੀ *)
{
  ਸਵੈਚਾਲਿਤ *s = (hb_ਸਦਿਸ਼_ਰਸਤਾ_sink_t *) draw_ਅੰਕੜੇ;
  ਜੇ (unlikely (!s->begin_command ())) ਵਾਪਸ;
  hb_ਸਦਿਸ਼_pdf_ਰਸਤਾ_append_xy (s, ਮੰਜ਼ਿਲ_x, ਮੰਜ਼ਿਲ_y);
  s->ਰਸਤਾ->append_str (" m\n");
  s->ਸਿਰਾ_command ();
}

ਸਥਿਰ ਖਾਲੀ
hb_ਸਦਿਸ਼_pdf_ਰਸਤਾ_ਸਤਰ_ਮੰਜ਼ਿਲ (hb_draw_funcs_t *, ਖਾਲੀ *draw_ਅੰਕੜੇ, hb_draw_state_t *,
			     ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_x, ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_y, ਖਾਲੀ *)
{
  ਸਵੈਚਾਲਿਤ *s = (hb_ਸਦਿਸ਼_ਰਸਤਾ_sink_t *) draw_ਅੰਕੜੇ;
  ਜੇ (unlikely (!s->begin_command ())) ਵਾਪਸ;
  hb_ਸਦਿਸ਼_pdf_ਰਸਤਾ_append_xy (s, ਮੰਜ਼ਿਲ_x, ਮੰਜ਼ਿਲ_y);
  s->ਰਸਤਾ->append_str (" l\n");
  s->ਸਿਰਾ_command ();
}

/* No quadratic_to — the null fallback auto-promotes to cubic. */

ਸਥਿਰ ਖਾਲੀ
hb_ਸਦਿਸ਼_pdf_ਰਸਤਾ_cubic_ਮੰਜ਼ਿਲ (hb_draw_funcs_t *, ਖਾਲੀ *draw_ਅੰਕੜੇ, hb_draw_state_t *,
			      ਦਸ਼ਮਲਵ c1x, ਦਸ਼ਮਲਵ c1y, ਦਸ਼ਮਲਵ c2x, ਦਸ਼ਮਲਵ c2y,
			      ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_x, ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_y, ਖਾਲੀ *)
{
  ਸਵੈਚਾਲਿਤ *s = (hb_ਸਦਿਸ਼_ਰਸਤਾ_sink_t *) draw_ਅੰਕੜੇ;
  ਜੇ (unlikely (!s->begin_command ())) ਵਾਪਸ;
  hb_ਸਦਿਸ਼_pdf_ਰਸਤਾ_append_xy (s, c1x, c1y);
  s->ਰਸਤਾ->append_c (' ');
  hb_ਸਦਿਸ਼_pdf_ਰਸਤਾ_append_xy (s, c2x, c2y);
  s->ਰਸਤਾ->append_c (' ');
  hb_ਸਦਿਸ਼_pdf_ਰਸਤਾ_append_xy (s, ਮੰਜ਼ਿਲ_x, ਮੰਜ਼ਿਲ_y);
  s->ਰਸਤਾ->append_str (" c\n");
  s->ਸਿਰਾ_command ();
}

ਸਥਿਰ ਖਾਲੀ
hb_ਸਦਿਸ਼_pdf_ਰਸਤਾ_close_ਰਸਤਾ (hb_draw_funcs_t *, ਖਾਲੀ *draw_ਅੰਕੜੇ, hb_draw_state_t *, ਖਾਲੀ *)
{
  ਸਵੈਚਾਲਿਤ *s = (hb_ਸਦਿਸ਼_ਰਸਤਾ_sink_t *) draw_ਅੰਕੜੇ;
  ਜੇ (unlikely (!s->begin_command ())) ਵਾਪਸ;
  s->ਰਸਤਾ->append_str ("h\n");
  s->ਸਿਰਾ_command ();
}

ਸਥਿਰ int64_t *
hb_ਸਦਿਸ਼_ਰਸਤਾ_get_budget_ਬਚਿਆ (hb_draw_funcs_t *, ਖਾਲੀ *draw_ਅੰਕੜੇ, ਖਾਲੀ *)
{
  ਵਾਪਸ ((hb_ਸਦਿਸ਼_ਰਸਤਾ_sink_t *) draw_ਅੰਕੜੇ)->budget;
}


/* ---- Lazy loaders ---- */

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ free_static_ਸਦਿਸ਼_svg_ਰਸਤਾ_draw_funcs ();
ਸਥਿਰ ਬਣਤਰ hb_ਸਦਿਸ਼_svg_ਰਸਤਾ_draw_funcs_lazy_loader_t
  : hb_draw_funcs_lazy_loader_t<hb_ਸਦਿਸ਼_svg_ਰਸਤਾ_draw_funcs_lazy_loader_t>
{
  ਸਥਿਰ hb_draw_funcs_t *create ()
  {
    hb_draw_funcs_t *funcs = hb_draw_funcs_create ();
    hb_draw_funcs_ਸੈੱਟ_move_ਮੰਜ਼ਿਲ_func (funcs, (hb_draw_move_ਮੰਜ਼ਿਲ_func_t) hb_ਸਦਿਸ਼_svg_ਰਸਤਾ_move_ਮੰਜ਼ਿਲ, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_draw_funcs_ਸੈੱਟ_ਸਤਰ_ਮੰਜ਼ਿਲ_func (funcs, (hb_draw_ਸਤਰ_ਮੰਜ਼ਿਲ_func_t) hb_ਸਦਿਸ਼_svg_ਰਸਤਾ_ਸਤਰ_ਮੰਜ਼ਿਲ, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_draw_funcs_ਸੈੱਟ_quadratic_ਮੰਜ਼ਿਲ_func (funcs, (hb_draw_quadratic_ਮੰਜ਼ਿਲ_func_t) hb_ਸਦਿਸ਼_svg_ਰਸਤਾ_quadratic_ਮੰਜ਼ਿਲ, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_draw_funcs_ਸੈੱਟ_cubic_ਮੰਜ਼ਿਲ_func (funcs, (hb_draw_cubic_ਮੰਜ਼ਿਲ_func_t) hb_ਸਦਿਸ਼_svg_ਰਸਤਾ_cubic_ਮੰਜ਼ਿਲ, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_draw_funcs_ਸੈੱਟ_close_ਰਸਤਾ_func (funcs, (hb_draw_close_ਰਸਤਾ_func_t) hb_ਸਦਿਸ਼_svg_ਰਸਤਾ_close_ਰਸਤਾ, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_draw_funcs_ਸੈੱਟ_get_budget_ਬਚਿਆ_func (funcs, hb_ਸਦਿਸ਼_ਰਸਤਾ_get_budget_ਬਚਿਆ, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_draw_funcs_make_immutable (funcs);
    hb_atexit (free_static_ਸਦਿਸ਼_svg_ਰਸਤਾ_draw_funcs);
    ਵਾਪਸ funcs;
  }
} static_ਸਦਿਸ਼_svg_ਰਸਤਾ_draw_funcs;

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
free_static_ਸਦਿਸ਼_svg_ਰਸਤਾ_draw_funcs ()
{ static_ਸਦਿਸ਼_svg_ਰਸਤਾ_draw_funcs.free_instance (); }

hb_draw_funcs_t *
hb_ਸਦਿਸ਼_svg_ਰਸਤਾ_draw_funcs_get ()
{ ਵਾਪਸ static_ਸਦਿਸ਼_svg_ਰਸਤਾ_draw_funcs.get_unconst (); }


ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ free_static_ਸਦਿਸ਼_pdf_ਰਸਤਾ_draw_funcs ();
ਸਥਿਰ ਬਣਤਰ hb_ਸਦਿਸ਼_pdf_ਰਸਤਾ_draw_funcs_lazy_loader_t
  : hb_draw_funcs_lazy_loader_t<hb_ਸਦਿਸ਼_pdf_ਰਸਤਾ_draw_funcs_lazy_loader_t>
{
  ਸਥਿਰ hb_draw_funcs_t *create ()
  {
    hb_draw_funcs_t *funcs = hb_draw_funcs_create ();
    hb_draw_funcs_ਸੈੱਟ_move_ਮੰਜ਼ਿਲ_func (funcs, (hb_draw_move_ਮੰਜ਼ਿਲ_func_t) hb_ਸਦਿਸ਼_pdf_ਰਸਤਾ_move_ਮੰਜ਼ਿਲ, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_draw_funcs_ਸੈੱਟ_ਸਤਰ_ਮੰਜ਼ਿਲ_func (funcs, (hb_draw_ਸਤਰ_ਮੰਜ਼ਿਲ_func_t) hb_ਸਦਿਸ਼_pdf_ਰਸਤਾ_ਸਤਰ_ਮੰਜ਼ਿਲ, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    /* No quadratic_to: the null fallback auto-promotes to cubic. */
    hb_draw_funcs_ਸੈੱਟ_cubic_ਮੰਜ਼ਿਲ_func (funcs, (hb_draw_cubic_ਮੰਜ਼ਿਲ_func_t) hb_ਸਦਿਸ਼_pdf_ਰਸਤਾ_cubic_ਮੰਜ਼ਿਲ, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_draw_funcs_ਸੈੱਟ_close_ਰਸਤਾ_func (funcs, (hb_draw_close_ਰਸਤਾ_func_t) hb_ਸਦਿਸ਼_pdf_ਰਸਤਾ_close_ਰਸਤਾ, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_draw_funcs_ਸੈੱਟ_get_budget_ਬਚਿਆ_func (funcs, hb_ਸਦਿਸ਼_ਰਸਤਾ_get_budget_ਬਚਿਆ, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_draw_funcs_make_immutable (funcs);
    hb_atexit (free_static_ਸਦਿਸ਼_pdf_ਰਸਤਾ_draw_funcs);
    ਵਾਪਸ funcs;
  }
} static_ਸਦਿਸ਼_pdf_ਰਸਤਾ_draw_funcs;

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
free_static_ਸਦਿਸ਼_pdf_ਰਸਤਾ_draw_funcs ()
{ static_ਸਦਿਸ਼_pdf_ਰਸਤਾ_draw_funcs.free_instance (); }

hb_draw_funcs_t *
hb_ਸਦਿਸ਼_pdf_ਰਸਤਾ_draw_funcs_get ()
{ ਵਾਪਸ static_ਸਦਿਸ਼_pdf_ਰਸਤਾ_draw_funcs.get_unconst (); }


hb_draw_funcs_t *
hb_ਸਦਿਸ਼_ਰਸਤਾ_draw_funcs_get (hb_ਸਦਿਸ਼_format_t format)
{
  ਚੋਣ (format)
  {
    ਹਾਲਤ HB_ਸਦਿਸ਼_FORMAT_SVG: ਵਾਪਸ hb_ਸਦਿਸ਼_svg_ਰਸਤਾ_draw_funcs_get ();
    ਹਾਲਤ HB_ਸਦਿਸ਼_FORMAT_PDF: ਵਾਪਸ hb_ਸਦਿਸ਼_pdf_ਰਸਤਾ_draw_funcs_get ();
    ਹਾਲਤ HB_ਸਦਿਸ਼_FORMAT_INVALID: ਮੂਲ: ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
  }
}
