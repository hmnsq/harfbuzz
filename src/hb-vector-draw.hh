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

#ଅନ୍ତର୍ଭାଵ "hb.hh"

#ଅନ୍ତର୍ଭାଵ "hb-vector.h"
#ଅନ୍ତର୍ଭାଵ "hb-geometry.hh"
#ଅନ୍ତର୍ଭାଵ "hb-machinery.hh"
#ଅନ୍ତର୍ଭାଵ "hb-vector-buf.hh"
#ଅନ୍ତର୍ଭାଵ "hb-vector-internal.hh"

ସଂରଚନା hb_vector_draw_t
{
  hb_object_header_t header;

  hb_vector_format_t format = HB_VECTOR_FORMAT_INVALID;
  hb_transform_t<> transform = {1, 0, 0, 1, 0, 0};
  ଦଶମିକ x_scale_factor = 1.f;
  ଦଶମିକ y_scale_factor = 1.f;
  hb_vector_extents_t extents = {0, 0, 0, 0};
  ତାର୍କିକ has_extents = ମିଥ୍ୟା;
  hb_color_t foreground = HB_COLOR (0, 0, 0, 255);
  hb_color_t background = HB_COLOR (0, 0, 0, 0);

  hb_vector_buf_t defs;
  hb_vector_buf_t body;
  hb_vector_buf_t path;
  hb_vector_buf_t pdf_extgstate_dict;
  unsigned pdf_extgstate_ଗଣନା = 0;

  /* Cumulative work budget for the current draw session; reset by
   * hb_vector_draw_clear().  Outline traversal and complete path commands
   * charge the same live counter. */
  int64_t budget = HB_BUDGET_DEFAULT;
  int64_t budget_remaining = HB_BUDGET_GLYPH;

  ଶୂନ୍ୟ recharge_budget ()
  {
    budget_remaining = budget == HB_BUDGET_DEFAULT ?
		       HB_BUDGET_GLYPH : budget;
  }

  ତାର୍କିକ begin_path_command ()
  {
    ଫେରନ୍ତୁ likely (budget_remaining >= 0 && !path.in_error ());
  }

  ଶୂନ୍ୟ end_path_command ()
  {
    /* The serialized bytes are bounded by the output buffer's document-size
     * cap (in_error), not the outline work budget; stop outline traversal
     * once that cap or an allocation error trips. */
    ଯଦି (unlikely (path.in_error ()))
      budget_remaining = 0;
  }

  ଶୂନ୍ୟ set_precision (unsigned p)
  {
    p = hb_min (p, 12u);
    defs.precision = p;
    body.precision = p;
    path.precision = p;
  }

  unsigned get_precision () ଅଚଳ { ଫେରନ୍ତୁ path.precision; }
  hb_blob_t *recycled_blob = nullptr;

  ଶୂନ୍ୟ new_path ()
  {
    flush_path ();
  }

  ଶୂନ୍ୟ flush_path ()
  {
    ଯଦି (!path.length) ଫେରନ୍ତୁ;
    ବାଛ (format)
    {
      ସ୍ଥିତି HB_VECTOR_FORMAT_PDF: flush_path_pdf (); ଭାଙ୍ଗ;
      ସ୍ଥିତି HB_VECTOR_FORMAT_SVG: flush_path_svg (); ଭାଙ୍ଗ;
      ସ୍ଥିତି HB_VECTOR_FORMAT_INVALID: ପୂର୍ବନିର୍ଦ୍ଧାରିତ: ଭାଙ୍ଗ;
    }
    path.shrink (0);
  }

  ଶୂନ୍ୟ flush_path_pdf ()
  {
    unsigned a = hb_color_get_alpha (foreground);
    ଯଦି (a < 255)
    {
      unsigned gs_idx = pdf_extgstate_ଗଣନା++;
      body.append_str ("/GS");
      body.append_unsigned (gs_idx);
      body.append_str (" gs\n");

      pdf_extgstate_dict.append_str ("/GS");
      pdf_extgstate_dict.append_unsigned (gs_idx);
      pdf_extgstate_dict.append_str (" << /Type /ExtGState /ca ");
      pdf_extgstate_dict.append_ଅଙ୍କ (a / 255.f, 4);
      pdf_extgstate_dict.append_str (" >> ");
    }
    body.append_ଅଙ୍କ (hb_color_get_red (foreground) / 255.f, 4);
    body.append_c (' ');
    body.append_ଅଙ୍କ (hb_color_get_green (foreground) / 255.f, 4);
    body.append_c (' ');
    body.append_ଅଙ୍କ (hb_color_get_blue (foreground) / 255.f, 4);
    body.append_str (" rg\n");
    body.append_len (path.arrayZ, path.length);
    body.append_str ("f\n");
  }

  ଶୂନ୍ୟ flush_path_svg ()
  {
    body.append_str ("<path d=\"");
    body.append_len (path.arrayZ, path.length);
    body.append_str ("\" fill=\"");
    body.append_svg_color (foreground, ସତ୍ୟ);
    body.append_str ("\"/>\n");
  }

  ଶୂନ୍ୟ transform_xy (ଦଶମିକ x, ଦଶମିକ y, ଦଶମିକ *tx, ଦଶମିକ *ty)
  {
    hb_vector_transform_point (transform, x_scale_factor, y_scale_factor, x, y, tx, ty);
  }

  ଶୂନ୍ୟ append_xy (ଦଶମିକ x, ଦଶମିକ y, ଅକ୍ଷର sep)
  {
    ଦଶମିକ tx, ty;
    transform_xy (x, y, &tx, &ty);
    path.append_ଅଙ୍କ (tx, path.precision);
    path.append_c (sep);
    path.append_ଅଙ୍କ (ty, path.precision);
  }

  ଶୂନ୍ୟ append_xy_svg (ଦଶମିକ x, ଦଶମିକ y) { append_xy (x, y, ','); }
  ଶୂନ୍ୟ append_xy_pdf (ଦଶମିକ x, ଦଶମିକ y) { append_xy (x, y, ' '); }
};

#endif /* HB_VECTOR_DRAW_HH */
