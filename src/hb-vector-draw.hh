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

#నిర్వచితం_కాకపోతే HB_సదిశ_DRAW_HH
#నిర్వచించు HB_సదిశ_DRAW_HH

#అన్తర్భావ "hb.hh"

#అన్తర్భావ "hb-vector.h"
#అన్తర్భావ "hb-geometry.hh"
#అన్తర్భావ "hb-machinery.hh"
#అన్తర్భావ "hb-vector-buf.hh"
#అన్తర్భావ "hb-vector-internal.hh"

నిర్మాణం hb_సదిశ_draw_t
{
  hb_object_శీర్షిక_t శీర్షిక;

  hb_సదిశ_format_t format = HB_సదిశ_FORMAT_INVALID;
  hb_transform_t<> transform = {1, 0, 0, 1, 0, 0};
  దశాంశం x_scale_గుణకం = 1.f;
  దశాంశం y_scale_గుణకం = 1.f;
  hb_సదిశ_extents_t extents = {0, 0, 0, 0};
  తార్కిక has_extents = అబద్ధం;
  hb_color_t foreground = HB_COLOR (0, 0, 0, 255);
  hb_color_t background = HB_COLOR (0, 0, 0, 0);

  hb_సదిశ_buf_t defs;
  hb_సదిశ_buf_t body;
  hb_సదిశ_buf_t మార్గం;
  hb_సదిశ_buf_t pdf_extgstate_dict;
  గుర్తులేని pdf_extgstate_లెక్క = 0;

  /* Cumulative work budget for the current draw session; reset by
   * hb_vector_draw_clear().  Outline traversal and complete path commands
   * charge the same live counter. */
  int64_t budget = HB_BUDGET_DEFAULT;
  int64_t budget_మిగిలిన = HB_BUDGET_GLYPH;

  శూన్య recharge_budget ()
  {
    budget_మిగిలిన = budget == HB_BUDGET_DEFAULT ?
		       HB_BUDGET_GLYPH : budget;
  }

  తార్కిక begin_మార్గం_command ()
  {
    తిరిగి likely (budget_మిగిలిన >= 0 && !మార్గం.లోపల_error ());
  }

  శూన్య చివర_మార్గం_command ()
  {
    /* The serialized bytes are bounded by the output buffer's document-size
     * cap (in_error), not the outline work budget; stop outline traversal
     * once that cap or an allocation error trips. */
    ఒకవేళ (unlikely (మార్గం.లోపల_error ()))
      budget_మిగిలిన = 0;
  }

  శూన్య సమితి_precision (గుర్తులేని p)
  {
    p = hb_min (p, 12u);
    defs.precision = p;
    body.precision = p;
    మార్గం.precision = p;
  }

  గుర్తులేని get_precision () స్థిరరాశి { తిరిగి మార్గం.precision; }
  hb_blob_t *recycled_blob = శూన్యసూచిక;

  శూన్య new_మార్గం ()
  {
    flush_మార్గం ();
  }

  శూన్య flush_మార్గం ()
  {
    ఒకవేళ (!మార్గం.నిడివి) తిరిగి;
    ఎంపిక (format)
    {
      స్థితి HB_సదిశ_FORMAT_PDF: flush_మార్గం_pdf (); ఆపు;
      స్థితి HB_సదిశ_FORMAT_SVG: flush_మార్గం_svg (); ఆపు;
      స్థితి HB_సదిశ_FORMAT_INVALID: అప్రమేయ: ఆపు;
    }
    మార్గం.shrink (0);
  }

  శూన్య flush_మార్గం_pdf ()
  {
    గుర్తులేని a = hb_color_get_alpha (foreground);
    ఒకవేళ (a < 255)
    {
      గుర్తులేని gs_idx = pdf_extgstate_లెక్క++;
      body.append_str ("/GS");
      body.append_unsigned (gs_idx);
      body.append_str (" gs\n");

      pdf_extgstate_dict.append_str ("/GS");
      pdf_extgstate_dict.append_unsigned (gs_idx);
      pdf_extgstate_dict.append_str (" << /Type /ExtGState /ca ");
      pdf_extgstate_dict.append_అంకె (a / 255.f, 4);
      pdf_extgstate_dict.append_str (" >> ");
    }
    body.append_అంకె (hb_color_get_red (foreground) / 255.f, 4);
    body.append_c (' ');
    body.append_అంకె (hb_color_get_green (foreground) / 255.f, 4);
    body.append_c (' ');
    body.append_అంకె (hb_color_get_blue (foreground) / 255.f, 4);
    body.append_str (" rg\n");
    body.append_len (మార్గం.arrayZ, మార్గం.నిడివి);
    body.append_str ("f\n");
  }

  శూన్య flush_మార్గం_svg ()
  {
    body.append_str ("<path d=\"");
    body.append_len (మార్గం.arrayZ, మార్గం.నిడివి);
    body.append_str ("\" fill=\"");
    body.append_svg_color (foreground, నిజం);
    body.append_str ("\"/>\n");
  }

  శూన్య transform_xy (దశాంశం x, దశాంశం y, దశాంశం *tx, దశాంశం *ty)
  {
    hb_సదిశ_transform_బిందువు (transform, x_scale_గుణకం, y_scale_గుణకం, x, y, tx, ty);
  }

  శూన్య append_xy (దశాంశం x, దశాంశం y, అక్షరం sep)
  {
    దశాంశం tx, ty;
    transform_xy (x, y, &tx, &ty);
    మార్గం.append_అంకె (tx, మార్గం.precision);
    మార్గం.append_c (sep);
    మార్గం.append_అంకె (ty, మార్గం.precision);
  }

  శూన్య append_xy_svg (దశాంశం x, దశాంశం y) { append_xy (x, y, ','); }
  శూన్య append_xy_pdf (దశాంశం x, దశాంశం y) { append_xy (x, y, ' '); }
};

#అయితే_ముగింపు /* HB_VECTOR_DRAW_HH */
