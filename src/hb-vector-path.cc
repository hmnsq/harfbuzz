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

ایستا درونخطی تهی
hb_vector_svg_path_append_xy (hb_vector_path_sink_t *s, اعشاری x, اعشاری y)
{
  s->path->append_رقم (x / s->x_scale, s->precision);
  s->path->append_c (',');
  s->path->append_رقم (y / s->y_scale, s->precision);
}

ایستا تهی
hb_vector_svg_path_move_to (hb_draw_funcs_t *, تهی *draw_data, hb_draw_state_t *,
			     اعشاری to_x, اعشاری to_y, تهی *)
{
  auto *s = (hb_vector_path_sink_t *) draw_data;
  اگر (unlikely (!s->begin_command ())) بازگشت;
  s->path->append_c ('M');
  hb_vector_svg_path_append_xy (s, to_x, to_y);
  s->end_command ();
}

ایستا تهی
hb_vector_svg_path_سطر_to (hb_draw_funcs_t *, تهی *draw_data, hb_draw_state_t *,
			     اعشاری to_x, اعشاری to_y, تهی *)
{
  auto *s = (hb_vector_path_sink_t *) draw_data;
  اگر (unlikely (!s->begin_command ())) بازگشت;
  s->path->append_c ('L');
  hb_vector_svg_path_append_xy (s, to_x, to_y);
  s->end_command ();
}

ایستا تهی
hb_vector_svg_path_quadratic_to (hb_draw_funcs_t *, تهی *draw_data, hb_draw_state_t *,
				  اعشاری cx, اعشاری cy, اعشاری to_x, اعشاری to_y, تهی *)
{
  auto *s = (hb_vector_path_sink_t *) draw_data;
  اگر (unlikely (!s->begin_command ())) بازگشت;
  s->path->append_c ('Q');
  hb_vector_svg_path_append_xy (s, cx, cy);
  s->path->append_c (' ');
  hb_vector_svg_path_append_xy (s, to_x, to_y);
  s->end_command ();
}

ایستا تهی
hb_vector_svg_path_cubic_to (hb_draw_funcs_t *, تهی *draw_data, hb_draw_state_t *,
			      اعشاری c1x, اعشاری c1y, اعشاری c2x, اعشاری c2y,
			      اعشاری to_x, اعشاری to_y, تهی *)
{
  auto *s = (hb_vector_path_sink_t *) draw_data;
  اگر (unlikely (!s->begin_command ())) بازگشت;
  s->path->append_c ('C');
  hb_vector_svg_path_append_xy (s, c1x, c1y);
  s->path->append_c (' ');
  hb_vector_svg_path_append_xy (s, c2x, c2y);
  s->path->append_c (' ');
  hb_vector_svg_path_append_xy (s, to_x, to_y);
  s->end_command ();
}

ایستا تهی
hb_vector_svg_path_close_path (hb_draw_funcs_t *, تهی *draw_data, hb_draw_state_t *, تهی *)
{
  auto *s = (hb_vector_path_sink_t *) draw_data;
  اگر (unlikely (!s->begin_command ())) بازگشت;
  s->path->append_c ('Z');
  s->end_command ();
}


/* ---- PDF path callbacks ---- */

ایستا درونخطی تهی
hb_vector_pdf_path_append_xy (hb_vector_path_sink_t *s, اعشاری x, اعشاری y)
{
  s->path->append_رقم (x / s->x_scale, s->precision);
  s->path->append_c (' ');
  s->path->append_رقم (y / s->y_scale, s->precision);
}

ایستا تهی
hb_vector_pdf_path_move_to (hb_draw_funcs_t *, تهی *draw_data, hb_draw_state_t *,
			     اعشاری to_x, اعشاری to_y, تهی *)
{
  auto *s = (hb_vector_path_sink_t *) draw_data;
  اگر (unlikely (!s->begin_command ())) بازگشت;
  hb_vector_pdf_path_append_xy (s, to_x, to_y);
  s->path->append_str (" m\n");
  s->end_command ();
}

ایستا تهی
hb_vector_pdf_path_سطر_to (hb_draw_funcs_t *, تهی *draw_data, hb_draw_state_t *,
			     اعشاری to_x, اعشاری to_y, تهی *)
{
  auto *s = (hb_vector_path_sink_t *) draw_data;
  اگر (unlikely (!s->begin_command ())) بازگشت;
  hb_vector_pdf_path_append_xy (s, to_x, to_y);
  s->path->append_str (" l\n");
  s->end_command ();
}

/* No quadratic_to — the null fallback auto-promotes to cubic. */

ایستا تهی
hb_vector_pdf_path_cubic_to (hb_draw_funcs_t *, تهی *draw_data, hb_draw_state_t *,
			      اعشاری c1x, اعشاری c1y, اعشاری c2x, اعشاری c2y,
			      اعشاری to_x, اعشاری to_y, تهی *)
{
  auto *s = (hb_vector_path_sink_t *) draw_data;
  اگر (unlikely (!s->begin_command ())) بازگشت;
  hb_vector_pdf_path_append_xy (s, c1x, c1y);
  s->path->append_c (' ');
  hb_vector_pdf_path_append_xy (s, c2x, c2y);
  s->path->append_c (' ');
  hb_vector_pdf_path_append_xy (s, to_x, to_y);
  s->path->append_str (" c\n");
  s->end_command ();
}

ایستا تهی
hb_vector_pdf_path_close_path (hb_draw_funcs_t *, تهی *draw_data, hb_draw_state_t *, تهی *)
{
  auto *s = (hb_vector_path_sink_t *) draw_data;
  اگر (unlikely (!s->begin_command ())) بازگشت;
  s->path->append_str ("h\n");
  s->end_command ();
}

ایستا int64_t *
hb_vector_path_get_budget_remaining (hb_draw_funcs_t *, تهی *draw_data, تهی *)
{
  بازگشت ((hb_vector_path_sink_t *) draw_data)->budget;
}


/* ---- Lazy loaders ---- */

ایستا درونخطی تهی free_static_vector_svg_path_draw_funcs ();
ایستا ساختار hb_vector_svg_path_draw_funcs_lazy_loader_t
  : hb_draw_funcs_lazy_loader_t<hb_vector_svg_path_draw_funcs_lazy_loader_t>
{
  ایستا hb_draw_funcs_t *create ()
  {
    hb_draw_funcs_t *funcs = hb_draw_funcs_create ();
    hb_draw_funcs_set_move_to_func (funcs, (hb_draw_move_to_func_t) hb_vector_svg_path_move_to, nullptr, nullptr);
    hb_draw_funcs_set_سطر_to_func (funcs, (hb_draw_سطر_to_func_t) hb_vector_svg_path_سطر_to, nullptr, nullptr);
    hb_draw_funcs_set_quadratic_to_func (funcs, (hb_draw_quadratic_to_func_t) hb_vector_svg_path_quadratic_to, nullptr, nullptr);
    hb_draw_funcs_set_cubic_to_func (funcs, (hb_draw_cubic_to_func_t) hb_vector_svg_path_cubic_to, nullptr, nullptr);
    hb_draw_funcs_set_close_path_func (funcs, (hb_draw_close_path_func_t) hb_vector_svg_path_close_path, nullptr, nullptr);
    hb_draw_funcs_set_get_budget_remaining_func (funcs, hb_vector_path_get_budget_remaining, nullptr, nullptr);
    hb_draw_funcs_make_immutable (funcs);
    hb_atexit (free_static_vector_svg_path_draw_funcs);
    بازگشت funcs;
  }
} static_vector_svg_path_draw_funcs;

ایستا درونخطی تهی
free_static_vector_svg_path_draw_funcs ()
{ static_vector_svg_path_draw_funcs.free_instance (); }

hb_draw_funcs_t *
hb_vector_svg_path_draw_funcs_get ()
{ بازگشت static_vector_svg_path_draw_funcs.get_unconst (); }


ایستا درونخطی تهی free_static_vector_pdf_path_draw_funcs ();
ایستا ساختار hb_vector_pdf_path_draw_funcs_lazy_loader_t
  : hb_draw_funcs_lazy_loader_t<hb_vector_pdf_path_draw_funcs_lazy_loader_t>
{
  ایستا hb_draw_funcs_t *create ()
  {
    hb_draw_funcs_t *funcs = hb_draw_funcs_create ();
    hb_draw_funcs_set_move_to_func (funcs, (hb_draw_move_to_func_t) hb_vector_pdf_path_move_to, nullptr, nullptr);
    hb_draw_funcs_set_سطر_to_func (funcs, (hb_draw_سطر_to_func_t) hb_vector_pdf_path_سطر_to, nullptr, nullptr);
    /* No quadratic_to: the null fallback auto-promotes to cubic. */
    hb_draw_funcs_set_cubic_to_func (funcs, (hb_draw_cubic_to_func_t) hb_vector_pdf_path_cubic_to, nullptr, nullptr);
    hb_draw_funcs_set_close_path_func (funcs, (hb_draw_close_path_func_t) hb_vector_pdf_path_close_path, nullptr, nullptr);
    hb_draw_funcs_set_get_budget_remaining_func (funcs, hb_vector_path_get_budget_remaining, nullptr, nullptr);
    hb_draw_funcs_make_immutable (funcs);
    hb_atexit (free_static_vector_pdf_path_draw_funcs);
    بازگشت funcs;
  }
} static_vector_pdf_path_draw_funcs;

ایستا درونخطی تهی
free_static_vector_pdf_path_draw_funcs ()
{ static_vector_pdf_path_draw_funcs.free_instance (); }

hb_draw_funcs_t *
hb_vector_pdf_path_draw_funcs_get ()
{ بازگشت static_vector_pdf_path_draw_funcs.get_unconst (); }


hb_draw_funcs_t *
hb_vector_path_draw_funcs_get (hb_vector_format_t format)
{
  انتخاب (format)
  {
    حالت HB_VECTOR_FORMAT_SVG: بازگشت hb_vector_svg_path_draw_funcs_get ();
    حالت HB_VECTOR_FORMAT_PDF: بازگشت hb_vector_pdf_path_draw_funcs_get ();
    حالت HB_VECTOR_FORMAT_INVALID: پیشفرض: بازگشت nullptr;
  }
}
