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

#अन्तर्भाव "hb.hh"

#अन्तर्भाव "hb-geometry.hh"
#अन्तर्भाव "hb-machinery.hh"
#अन्तर्भाव "hb-vector-path.hh"
#अन्तर्भाव "hb-vector-buf.hh"
#अन्तर्भाव "hb-vector-internal.hh"


/* ---- SVG path callbacks ---- */

स्थिर ओळीत शून्य
hb_vector_svg_path_append_xy (hb_vector_path_sink_t *s, दशांश x, दशांश y)
{
  s->path->append_अंक (x / s->x_scale, s->precision);
  s->path->append_c (',');
  s->path->append_अंक (y / s->y_scale, s->precision);
}

स्थिर शून्य
hb_vector_svg_path_move_to (hb_draw_funcs_t *, शून्य *draw_data, hb_draw_state_t *,
			     दशांश to_x, दशांश to_y, शून्य *)
{
  स्वतः *s = (hb_vector_path_sink_t *) draw_data;
  जर (unlikely (!s->begin_command ())) परत;
  s->path->append_c ('M');
  hb_vector_svg_path_append_xy (s, to_x, to_y);
  s->end_command ();
}

स्थिर शून्य
hb_vector_svg_path_ओळ_to (hb_draw_funcs_t *, शून्य *draw_data, hb_draw_state_t *,
			     दशांश to_x, दशांश to_y, शून्य *)
{
  स्वतः *s = (hb_vector_path_sink_t *) draw_data;
  जर (unlikely (!s->begin_command ())) परत;
  s->path->append_c ('L');
  hb_vector_svg_path_append_xy (s, to_x, to_y);
  s->end_command ();
}

स्थिर शून्य
hb_vector_svg_path_quadratic_to (hb_draw_funcs_t *, शून्य *draw_data, hb_draw_state_t *,
				  दशांश cx, दशांश cy, दशांश to_x, दशांश to_y, शून्य *)
{
  स्वतः *s = (hb_vector_path_sink_t *) draw_data;
  जर (unlikely (!s->begin_command ())) परत;
  s->path->append_c ('Q');
  hb_vector_svg_path_append_xy (s, cx, cy);
  s->path->append_c (' ');
  hb_vector_svg_path_append_xy (s, to_x, to_y);
  s->end_command ();
}

स्थिर शून्य
hb_vector_svg_path_cubic_to (hb_draw_funcs_t *, शून्य *draw_data, hb_draw_state_t *,
			      दशांश c1x, दशांश c1y, दशांश c2x, दशांश c2y,
			      दशांश to_x, दशांश to_y, शून्य *)
{
  स्वतः *s = (hb_vector_path_sink_t *) draw_data;
  जर (unlikely (!s->begin_command ())) परत;
  s->path->append_c ('C');
  hb_vector_svg_path_append_xy (s, c1x, c1y);
  s->path->append_c (' ');
  hb_vector_svg_path_append_xy (s, c2x, c2y);
  s->path->append_c (' ');
  hb_vector_svg_path_append_xy (s, to_x, to_y);
  s->end_command ();
}

स्थिर शून्य
hb_vector_svg_path_close_path (hb_draw_funcs_t *, शून्य *draw_data, hb_draw_state_t *, शून्य *)
{
  स्वतः *s = (hb_vector_path_sink_t *) draw_data;
  जर (unlikely (!s->begin_command ())) परत;
  s->path->append_c ('Z');
  s->end_command ();
}


/* ---- PDF path callbacks ---- */

स्थिर ओळीत शून्य
hb_vector_pdf_path_append_xy (hb_vector_path_sink_t *s, दशांश x, दशांश y)
{
  s->path->append_अंक (x / s->x_scale, s->precision);
  s->path->append_c (' ');
  s->path->append_अंक (y / s->y_scale, s->precision);
}

स्थिर शून्य
hb_vector_pdf_path_move_to (hb_draw_funcs_t *, शून्य *draw_data, hb_draw_state_t *,
			     दशांश to_x, दशांश to_y, शून्य *)
{
  स्वतः *s = (hb_vector_path_sink_t *) draw_data;
  जर (unlikely (!s->begin_command ())) परत;
  hb_vector_pdf_path_append_xy (s, to_x, to_y);
  s->path->append_str (" m\n");
  s->end_command ();
}

स्थिर शून्य
hb_vector_pdf_path_ओळ_to (hb_draw_funcs_t *, शून्य *draw_data, hb_draw_state_t *,
			     दशांश to_x, दशांश to_y, शून्य *)
{
  स्वतः *s = (hb_vector_path_sink_t *) draw_data;
  जर (unlikely (!s->begin_command ())) परत;
  hb_vector_pdf_path_append_xy (s, to_x, to_y);
  s->path->append_str (" l\n");
  s->end_command ();
}

/* No quadratic_to — the null fallback auto-promotes to cubic. */

स्थिर शून्य
hb_vector_pdf_path_cubic_to (hb_draw_funcs_t *, शून्य *draw_data, hb_draw_state_t *,
			      दशांश c1x, दशांश c1y, दशांश c2x, दशांश c2y,
			      दशांश to_x, दशांश to_y, शून्य *)
{
  स्वतः *s = (hb_vector_path_sink_t *) draw_data;
  जर (unlikely (!s->begin_command ())) परत;
  hb_vector_pdf_path_append_xy (s, c1x, c1y);
  s->path->append_c (' ');
  hb_vector_pdf_path_append_xy (s, c2x, c2y);
  s->path->append_c (' ');
  hb_vector_pdf_path_append_xy (s, to_x, to_y);
  s->path->append_str (" c\n");
  s->end_command ();
}

स्थिर शून्य
hb_vector_pdf_path_close_path (hb_draw_funcs_t *, शून्य *draw_data, hb_draw_state_t *, शून्य *)
{
  स्वतः *s = (hb_vector_path_sink_t *) draw_data;
  जर (unlikely (!s->begin_command ())) परत;
  s->path->append_str ("h\n");
  s->end_command ();
}

स्थिर int64_t *
hb_vector_path_get_budget_remaining (hb_draw_funcs_t *, शून्य *draw_data, शून्य *)
{
  परत ((hb_vector_path_sink_t *) draw_data)->budget;
}


/* ---- Lazy loaders ---- */

स्थिर ओळीत शून्य free_static_vector_svg_path_draw_funcs ();
स्थिर संरचना hb_vector_svg_path_draw_funcs_lazy_loader_t
  : hb_draw_funcs_lazy_loader_t<hb_vector_svg_path_draw_funcs_lazy_loader_t>
{
  स्थिर hb_draw_funcs_t *create ()
  {
    hb_draw_funcs_t *funcs = hb_draw_funcs_create ();
    hb_draw_funcs_set_move_to_func (funcs, (hb_draw_move_to_func_t) hb_vector_svg_path_move_to, nullptr, nullptr);
    hb_draw_funcs_set_ओळ_to_func (funcs, (hb_draw_ओळ_to_func_t) hb_vector_svg_path_ओळ_to, nullptr, nullptr);
    hb_draw_funcs_set_quadratic_to_func (funcs, (hb_draw_quadratic_to_func_t) hb_vector_svg_path_quadratic_to, nullptr, nullptr);
    hb_draw_funcs_set_cubic_to_func (funcs, (hb_draw_cubic_to_func_t) hb_vector_svg_path_cubic_to, nullptr, nullptr);
    hb_draw_funcs_set_close_path_func (funcs, (hb_draw_close_path_func_t) hb_vector_svg_path_close_path, nullptr, nullptr);
    hb_draw_funcs_set_get_budget_remaining_func (funcs, hb_vector_path_get_budget_remaining, nullptr, nullptr);
    hb_draw_funcs_make_immutable (funcs);
    hb_atexit (free_static_vector_svg_path_draw_funcs);
    परत funcs;
  }
} static_vector_svg_path_draw_funcs;

स्थिर ओळीत शून्य
free_static_vector_svg_path_draw_funcs ()
{ static_vector_svg_path_draw_funcs.free_instance (); }

hb_draw_funcs_t *
hb_vector_svg_path_draw_funcs_get ()
{ परत static_vector_svg_path_draw_funcs.get_unconst (); }


स्थिर ओळीत शून्य free_static_vector_pdf_path_draw_funcs ();
स्थिर संरचना hb_vector_pdf_path_draw_funcs_lazy_loader_t
  : hb_draw_funcs_lazy_loader_t<hb_vector_pdf_path_draw_funcs_lazy_loader_t>
{
  स्थिर hb_draw_funcs_t *create ()
  {
    hb_draw_funcs_t *funcs = hb_draw_funcs_create ();
    hb_draw_funcs_set_move_to_func (funcs, (hb_draw_move_to_func_t) hb_vector_pdf_path_move_to, nullptr, nullptr);
    hb_draw_funcs_set_ओळ_to_func (funcs, (hb_draw_ओळ_to_func_t) hb_vector_pdf_path_ओळ_to, nullptr, nullptr);
    /* No quadratic_to: the null fallback auto-promotes to cubic. */
    hb_draw_funcs_set_cubic_to_func (funcs, (hb_draw_cubic_to_func_t) hb_vector_pdf_path_cubic_to, nullptr, nullptr);
    hb_draw_funcs_set_close_path_func (funcs, (hb_draw_close_path_func_t) hb_vector_pdf_path_close_path, nullptr, nullptr);
    hb_draw_funcs_set_get_budget_remaining_func (funcs, hb_vector_path_get_budget_remaining, nullptr, nullptr);
    hb_draw_funcs_make_immutable (funcs);
    hb_atexit (free_static_vector_pdf_path_draw_funcs);
    परत funcs;
  }
} static_vector_pdf_path_draw_funcs;

स्थिर ओळीत शून्य
free_static_vector_pdf_path_draw_funcs ()
{ static_vector_pdf_path_draw_funcs.free_instance (); }

hb_draw_funcs_t *
hb_vector_pdf_path_draw_funcs_get ()
{ परत static_vector_pdf_path_draw_funcs.get_unconst (); }


hb_draw_funcs_t *
hb_vector_path_draw_funcs_get (hb_vector_format_t format)
{
  निवड (format)
  {
    स्थिती HB_VECTOR_FORMAT_SVG: परत hb_vector_svg_path_draw_funcs_get ();
    स्थिती HB_VECTOR_FORMAT_PDF: परत hb_vector_pdf_path_draw_funcs_get ();
    स्थिती HB_VECTOR_FORMAT_INVALID: पूर्वनिर्धारित: परत nullptr;
  }
}
