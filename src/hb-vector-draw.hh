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

#ifndef HB_VECTOR_DRAW_HH
#define HB_VECTOR_DRAW_HH

#અન્તર્ભાવ "hb.hh"

#અન્તર્ભાવ "hb-vector.h"
#અન્તર્ભાવ "hb-geometry.hh"
#અન્તર્ભાવ "hb-machinery.hh"
#અન્તર્ભાવ "hb-vector-buf.hh"
#અન્તર્ભાવ "hb-vector-internal.hh"

સંરચના hb_vector_draw_t
{
  hb_object_header_t header;

  hb_vector_format_t format = HB_VECTOR_FORMAT_INVALID;
  hb_transform_t<> transform = {1, 0, 0, 1, 0, 0};
  દશાંશ x_scale_factor = 1.f;
  દશાંશ y_scale_factor = 1.f;
  hb_vector_extents_t extents = {0, 0, 0, 0};
  તાર્કિક has_extents = ખોટું;
  hb_color_t foreground = HB_COLOR (0, 0, 0, 255);
  hb_color_t background = HB_COLOR (0, 0, 0, 0);

  hb_vector_buf_t defs;
  hb_vector_buf_t body;
  hb_vector_buf_t path;
  hb_vector_buf_t pdf_extgstate_dict;
  unsigned pdf_extgstate_ગણતરી = 0;

  /* Cumulative work budget for the current draw session; reset by
   * hb_vector_draw_clear().  Outline traversal and complete path commands
   * charge the same live counter. */
  int64_t budget = HB_BUDGET_DEFAULT;
  int64_t budget_remaining = HB_BUDGET_GLYPH;

  શૂન્ય recharge_budget ()
  {
    budget_remaining = budget == HB_BUDGET_DEFAULT ?
		       HB_BUDGET_GLYPH : budget;
  }

  તાર્કિક begin_path_command ()
  {
    પાછા likely (budget_remaining >= 0 && !path.in_error ());
  }

  શૂન્ય end_path_command ()
  {
    /* The serialized bytes are bounded by the output buffer's document-size
     * cap (in_error), not the outline work budget; stop outline traversal
     * once that cap or an allocation error trips. */
    જો (unlikely (path.in_error ()))
      budget_remaining = 0;
  }

  શૂન્ય set_precision (unsigned p)
  {
    p = hb_min (p, 12u);
    defs.precision = p;
    body.precision = p;
    path.precision = p;
  }

  unsigned get_precision () અચલ { પાછા path.precision; }
  hb_blob_t *recycled_blob = nullptr;

  શૂન્ય new_path ()
  {
    flush_path ();
  }

  શૂન્ય flush_path ()
  {
    જો (!path.length) પાછા;
    પસંદગી (format)
    {
      સ્થિતિ HB_VECTOR_FORMAT_PDF: flush_path_pdf (); તોડો;
      સ્થિતિ HB_VECTOR_FORMAT_SVG: flush_path_svg (); તોડો;
      સ્થિતિ HB_VECTOR_FORMAT_INVALID: મૂળભૂત: તોડો;
    }
    path.shrink (0);
  }

  શૂન્ય flush_path_pdf ()
  {
    unsigned a = hb_color_get_alpha (foreground);
    જો (a < 255)
    {
      unsigned gs_idx = pdf_extgstate_ગણતરી++;
      body.append_str ("/GS");
      body.append_unsigned (gs_idx);
      body.append_str (" gs\n");

      pdf_extgstate_dict.append_str ("/GS");
      pdf_extgstate_dict.append_unsigned (gs_idx);
      pdf_extgstate_dict.append_str (" << /Type /ExtGState /ca ");
      pdf_extgstate_dict.append_અંક (a / 255.f, 4);
      pdf_extgstate_dict.append_str (" >> ");
    }
    body.append_અંક (hb_color_get_red (foreground) / 255.f, 4);
    body.append_c (' ');
    body.append_અંક (hb_color_get_green (foreground) / 255.f, 4);
    body.append_c (' ');
    body.append_અંક (hb_color_get_blue (foreground) / 255.f, 4);
    body.append_str (" rg\n");
    body.append_len (path.arrayZ, path.length);
    body.append_str ("f\n");
  }

  શૂન્ય flush_path_svg ()
  {
    body.append_str ("<path d=\"");
    body.append_len (path.arrayZ, path.length);
    body.append_str ("\" fill=\"");
    body.append_svg_color (foreground, સાચું);
    body.append_str ("\"/>\n");
  }

  શૂન્ય transform_xy (દશાંશ x, દશાંશ y, દશાંશ *tx, દશાંશ *ty)
  {
    hb_vector_transform_point (transform, x_scale_factor, y_scale_factor, x, y, tx, ty);
  }

  શૂન્ય append_xy (દશાંશ x, દશાંશ y, અક્ષર sep)
  {
    દશાંશ tx, ty;
    transform_xy (x, y, &tx, &ty);
    path.append_અંક (tx, path.precision);
    path.append_c (sep);
    path.append_અંક (ty, path.precision);
  }

  શૂન્ય append_xy_svg (દશાંશ x, દશાંશ y) { append_xy (x, y, ','); }
  શૂન્ય append_xy_pdf (દશાંશ x, દશાંશ y) { append_xy (x, y, ' '); }
};

#endif /* HB_VECTOR_DRAW_HH */
