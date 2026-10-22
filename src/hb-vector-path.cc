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

#అన్తర్భావ "hb-geometry.hh"
#అన్తర్భావ "hb-machinery.hh"
#అన్తర్భావ "hb-vector-path.hh"
#అన్తర్భావ "hb-vector-buf.hh"
#అన్తర్భావ "hb-vector-internal.hh"


/* ---- SVG path callbacks ---- */

స్థిర అంతఃవరుస శూన్య
hb_సదిశ_svg_మార్గం_append_xy (hb_సదిశ_మార్గం_sink_t *s, దశాంశం x, దశాంశం y)
{
  s->మార్గం->append_అంకె (x / s->x_scale, s->precision);
  s->మార్గం->append_c (',');
  s->మార్గం->append_అంకె (y / s->y_scale, s->precision);
}

స్థిర శూన్య
hb_సదిశ_svg_మార్గం_move_గమ్యం (hb_draw_funcs_t *, శూన్య *draw_దత్తాంశం, hb_draw_state_t *,
			     దశాంశం గమ్యం_x, దశాంశం గమ్యం_y, శూన్య *)
{
  స్వయంచాలక *s = (hb_సదిశ_మార్గం_sink_t *) draw_దత్తాంశం;
  ఒకవేళ (unlikely (!s->begin_command ())) తిరిగి;
  s->మార్గం->append_c ('M');
  hb_సదిశ_svg_మార్గం_append_xy (s, గమ్యం_x, గమ్యం_y);
  s->చివర_command ();
}

స్థిర శూన్య
hb_సదిశ_svg_మార్గం_పంక్తి_గమ్యం (hb_draw_funcs_t *, శూన్య *draw_దత్తాంశం, hb_draw_state_t *,
			     దశాంశం గమ్యం_x, దశాంశం గమ్యం_y, శూన్య *)
{
  స్వయంచాలక *s = (hb_సదిశ_మార్గం_sink_t *) draw_దత్తాంశం;
  ఒకవేళ (unlikely (!s->begin_command ())) తిరిగి;
  s->మార్గం->append_c ('L');
  hb_సదిశ_svg_మార్గం_append_xy (s, గమ్యం_x, గమ్యం_y);
  s->చివర_command ();
}

స్థిర శూన్య
hb_సదిశ_svg_మార్గం_quadratic_గమ్యం (hb_draw_funcs_t *, శూన్య *draw_దత్తాంశం, hb_draw_state_t *,
				  దశాంశం cx, దశాంశం cy, దశాంశం గమ్యం_x, దశాంశం గమ్యం_y, శూన్య *)
{
  స్వయంచాలక *s = (hb_సదిశ_మార్గం_sink_t *) draw_దత్తాంశం;
  ఒకవేళ (unlikely (!s->begin_command ())) తిరిగి;
  s->మార్గం->append_c ('Q');
  hb_సదిశ_svg_మార్గం_append_xy (s, cx, cy);
  s->మార్గం->append_c (' ');
  hb_సదిశ_svg_మార్గం_append_xy (s, గమ్యం_x, గమ్యం_y);
  s->చివర_command ();
}

స్థిర శూన్య
hb_సదిశ_svg_మార్గం_cubic_గమ్యం (hb_draw_funcs_t *, శూన్య *draw_దత్తాంశం, hb_draw_state_t *,
			      దశాంశం c1x, దశాంశం c1y, దశాంశం c2x, దశాంశం c2y,
			      దశాంశం గమ్యం_x, దశాంశం గమ్యం_y, శూన్య *)
{
  స్వయంచాలక *s = (hb_సదిశ_మార్గం_sink_t *) draw_దత్తాంశం;
  ఒకవేళ (unlikely (!s->begin_command ())) తిరిగి;
  s->మార్గం->append_c ('C');
  hb_సదిశ_svg_మార్గం_append_xy (s, c1x, c1y);
  s->మార్గం->append_c (' ');
  hb_సదిశ_svg_మార్గం_append_xy (s, c2x, c2y);
  s->మార్గం->append_c (' ');
  hb_సదిశ_svg_మార్గం_append_xy (s, గమ్యం_x, గమ్యం_y);
  s->చివర_command ();
}

స్థిర శూన్య
hb_సదిశ_svg_మార్గం_close_మార్గం (hb_draw_funcs_t *, శూన్య *draw_దత్తాంశం, hb_draw_state_t *, శూన్య *)
{
  స్వయంచాలక *s = (hb_సదిశ_మార్గం_sink_t *) draw_దత్తాంశం;
  ఒకవేళ (unlikely (!s->begin_command ())) తిరిగి;
  s->మార్గం->append_c ('Z');
  s->చివర_command ();
}


/* ---- PDF path callbacks ---- */

స్థిర అంతఃవరుస శూన్య
hb_సదిశ_pdf_మార్గం_append_xy (hb_సదిశ_మార్గం_sink_t *s, దశాంశం x, దశాంశం y)
{
  s->మార్గం->append_అంకె (x / s->x_scale, s->precision);
  s->మార్గం->append_c (' ');
  s->మార్గం->append_అంకె (y / s->y_scale, s->precision);
}

స్థిర శూన్య
hb_సదిశ_pdf_మార్గం_move_గమ్యం (hb_draw_funcs_t *, శూన్య *draw_దత్తాంశం, hb_draw_state_t *,
			     దశాంశం గమ్యం_x, దశాంశం గమ్యం_y, శూన్య *)
{
  స్వయంచాలక *s = (hb_సదిశ_మార్గం_sink_t *) draw_దత్తాంశం;
  ఒకవేళ (unlikely (!s->begin_command ())) తిరిగి;
  hb_సదిశ_pdf_మార్గం_append_xy (s, గమ్యం_x, గమ్యం_y);
  s->మార్గం->append_str (" m\n");
  s->చివర_command ();
}

స్థిర శూన్య
hb_సదిశ_pdf_మార్గం_పంక్తి_గమ్యం (hb_draw_funcs_t *, శూన్య *draw_దత్తాంశం, hb_draw_state_t *,
			     దశాంశం గమ్యం_x, దశాంశం గమ్యం_y, శూన్య *)
{
  స్వయంచాలక *s = (hb_సదిశ_మార్గం_sink_t *) draw_దత్తాంశం;
  ఒకవేళ (unlikely (!s->begin_command ())) తిరిగి;
  hb_సదిశ_pdf_మార్గం_append_xy (s, గమ్యం_x, గమ్యం_y);
  s->మార్గం->append_str (" l\n");
  s->చివర_command ();
}

/* No quadratic_to — the null fallback auto-promotes to cubic. */

స్థిర శూన్య
hb_సదిశ_pdf_మార్గం_cubic_గమ్యం (hb_draw_funcs_t *, శూన్య *draw_దత్తాంశం, hb_draw_state_t *,
			      దశాంశం c1x, దశాంశం c1y, దశాంశం c2x, దశాంశం c2y,
			      దశాంశం గమ్యం_x, దశాంశం గమ్యం_y, శూన్య *)
{
  స్వయంచాలక *s = (hb_సదిశ_మార్గం_sink_t *) draw_దత్తాంశం;
  ఒకవేళ (unlikely (!s->begin_command ())) తిరిగి;
  hb_సదిశ_pdf_మార్గం_append_xy (s, c1x, c1y);
  s->మార్గం->append_c (' ');
  hb_సదిశ_pdf_మార్గం_append_xy (s, c2x, c2y);
  s->మార్గం->append_c (' ');
  hb_సదిశ_pdf_మార్గం_append_xy (s, గమ్యం_x, గమ్యం_y);
  s->మార్గం->append_str (" c\n");
  s->చివర_command ();
}

స్థిర శూన్య
hb_సదిశ_pdf_మార్గం_close_మార్గం (hb_draw_funcs_t *, శూన్య *draw_దత్తాంశం, hb_draw_state_t *, శూన్య *)
{
  స్వయంచాలక *s = (hb_సదిశ_మార్గం_sink_t *) draw_దత్తాంశం;
  ఒకవేళ (unlikely (!s->begin_command ())) తిరిగి;
  s->మార్గం->append_str ("h\n");
  s->చివర_command ();
}

స్థిర int64_t *
hb_సదిశ_మార్గం_get_budget_మిగిలిన (hb_draw_funcs_t *, శూన్య *draw_దత్తాంశం, శూన్య *)
{
  తిరిగి ((hb_సదిశ_మార్గం_sink_t *) draw_దత్తాంశం)->budget;
}


/* ---- Lazy loaders ---- */

స్థిర అంతఃవరుస శూన్య free_static_సదిశ_svg_మార్గం_draw_funcs ();
స్థిర నిర్మాణం hb_సదిశ_svg_మార్గం_draw_funcs_lazy_loader_t
  : hb_draw_funcs_lazy_loader_t<hb_సదిశ_svg_మార్గం_draw_funcs_lazy_loader_t>
{
  స్థిర hb_draw_funcs_t *create ()
  {
    hb_draw_funcs_t *funcs = hb_draw_funcs_create ();
    hb_draw_funcs_సమితి_move_గమ్యం_func (funcs, (hb_draw_move_గమ్యం_func_t) hb_సదిశ_svg_మార్గం_move_గమ్యం, శూన్యసూచిక, శూన్యసూచిక);
    hb_draw_funcs_సమితి_పంక్తి_గమ్యం_func (funcs, (hb_draw_పంక్తి_గమ్యం_func_t) hb_సదిశ_svg_మార్గం_పంక్తి_గమ్యం, శూన్యసూచిక, శూన్యసూచిక);
    hb_draw_funcs_సమితి_quadratic_గమ్యం_func (funcs, (hb_draw_quadratic_గమ్యం_func_t) hb_సదిశ_svg_మార్గం_quadratic_గమ్యం, శూన్యసూచిక, శూన్యసూచిక);
    hb_draw_funcs_సమితి_cubic_గమ్యం_func (funcs, (hb_draw_cubic_గమ్యం_func_t) hb_సదిశ_svg_మార్గం_cubic_గమ్యం, శూన్యసూచిక, శూన్యసూచిక);
    hb_draw_funcs_సమితి_close_మార్గం_func (funcs, (hb_draw_close_మార్గం_func_t) hb_సదిశ_svg_మార్గం_close_మార్గం, శూన్యసూచిక, శూన్యసూచిక);
    hb_draw_funcs_సమితి_get_budget_మిగిలిన_func (funcs, hb_సదిశ_మార్గం_get_budget_మిగిలిన, శూన్యసూచిక, శూన్యసూచిక);
    hb_draw_funcs_make_immutable (funcs);
    hb_atexit (free_static_సదిశ_svg_మార్గం_draw_funcs);
    తిరిగి funcs;
  }
} static_సదిశ_svg_మార్గం_draw_funcs;

స్థిర అంతఃవరుస శూన్య
free_static_సదిశ_svg_మార్గం_draw_funcs ()
{ static_సదిశ_svg_మార్గం_draw_funcs.free_instance (); }

hb_draw_funcs_t *
hb_సదిశ_svg_మార్గం_draw_funcs_get ()
{ తిరిగి static_సదిశ_svg_మార్గం_draw_funcs.get_unconst (); }


స్థిర అంతఃవరుస శూన్య free_static_సదిశ_pdf_మార్గం_draw_funcs ();
స్థిర నిర్మాణం hb_సదిశ_pdf_మార్గం_draw_funcs_lazy_loader_t
  : hb_draw_funcs_lazy_loader_t<hb_సదిశ_pdf_మార్గం_draw_funcs_lazy_loader_t>
{
  స్థిర hb_draw_funcs_t *create ()
  {
    hb_draw_funcs_t *funcs = hb_draw_funcs_create ();
    hb_draw_funcs_సమితి_move_గమ్యం_func (funcs, (hb_draw_move_గమ్యం_func_t) hb_సదిశ_pdf_మార్గం_move_గమ్యం, శూన్యసూచిక, శూన్యసూచిక);
    hb_draw_funcs_సమితి_పంక్తి_గమ్యం_func (funcs, (hb_draw_పంక్తి_గమ్యం_func_t) hb_సదిశ_pdf_మార్గం_పంక్తి_గమ్యం, శూన్యసూచిక, శూన్యసూచిక);
    /* No quadratic_to: the null fallback auto-promotes to cubic. */
    hb_draw_funcs_సమితి_cubic_గమ్యం_func (funcs, (hb_draw_cubic_గమ్యం_func_t) hb_సదిశ_pdf_మార్గం_cubic_గమ్యం, శూన్యసూచిక, శూన్యసూచిక);
    hb_draw_funcs_సమితి_close_మార్గం_func (funcs, (hb_draw_close_మార్గం_func_t) hb_సదిశ_pdf_మార్గం_close_మార్గం, శూన్యసూచిక, శూన్యసూచిక);
    hb_draw_funcs_సమితి_get_budget_మిగిలిన_func (funcs, hb_సదిశ_మార్గం_get_budget_మిగిలిన, శూన్యసూచిక, శూన్యసూచిక);
    hb_draw_funcs_make_immutable (funcs);
    hb_atexit (free_static_సదిశ_pdf_మార్గం_draw_funcs);
    తిరిగి funcs;
  }
} static_సదిశ_pdf_మార్గం_draw_funcs;

స్థిర అంతఃవరుస శూన్య
free_static_సదిశ_pdf_మార్గం_draw_funcs ()
{ static_సదిశ_pdf_మార్గం_draw_funcs.free_instance (); }

hb_draw_funcs_t *
hb_సదిశ_pdf_మార్గం_draw_funcs_get ()
{ తిరిగి static_సదిశ_pdf_మార్గం_draw_funcs.get_unconst (); }


hb_draw_funcs_t *
hb_సదిశ_మార్గం_draw_funcs_get (hb_సదిశ_format_t format)
{
  ఎంపిక (format)
  {
    స్థితి HB_సదిశ_FORMAT_SVG: తిరిగి hb_సదిశ_svg_మార్గం_draw_funcs_get ();
    స్థితి HB_సదిశ_FORMAT_PDF: తిరిగి hb_సదిశ_pdf_మార్గం_draw_funcs_get ();
    స్థితి HB_సదిశ_FORMAT_INVALID: అప్రమేయ: తిరిగి శూన్యసూచిక;
  }
}
