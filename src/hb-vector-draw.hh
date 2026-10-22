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

#شامل "hb.hh"

#شامل "hb-vector.h"
#شامل "hb-geometry.hh"
#شامل "hb-machinery.hh"
#شامل "hb-vector-buf.hh"
#شامل "hb-vector-internal.hh"

ساختار hb_vector_draw_t
{
  hb_object_header_t header;

  hb_vector_format_t format = HB_VECTOR_FORMAT_INVALID;
  hb_transform_t<> transform = {1, 0, 0, 1, 0, 0};
  اعشاری x_scale_factor = 1.f;
  اعشاری y_scale_factor = 1.f;
  hb_vector_extents_t extents = {0, 0, 0, 0};
  منطقی has_extents = نادرست;
  hb_color_t foreground = HB_COLOR (0, 0, 0, 255);
  hb_color_t background = HB_COLOR (0, 0, 0, 0);

  hb_vector_buf_t defs;
  hb_vector_buf_t body;
  hb_vector_buf_t path;
  hb_vector_buf_t pdf_extgstate_dict;
  unsigned pdf_extgstate_شمار = 0;

  /* Cumulative work budget for the current draw session; reset by
   * hb_vector_draw_clear().  Outline traversal and complete path commands
   * charge the same live counter. */
  int64_t budget = HB_BUDGET_DEFAULT;
  int64_t budget_remaining = HB_BUDGET_GLYPH;

  تهی recharge_budget ()
  {
    budget_remaining = budget == HB_BUDGET_DEFAULT ?
		       HB_BUDGET_GLYPH : budget;
  }

  منطقی begin_path_command ()
  {
    بازگشت likely (budget_remaining >= 0 && !path.in_error ());
  }

  تهی end_path_command ()
  {
    /* The serialized bytes are bounded by the output buffer's document-size
     * cap (in_error), not the outline work budget; stop outline traversal
     * once that cap or an allocation error trips. */
    اگر (unlikely (path.in_error ()))
      budget_remaining = 0;
  }

  تهی set_precision (unsigned p)
  {
    p = hb_min (p, 12u);
    defs.precision = p;
    body.precision = p;
    path.precision = p;
  }

  unsigned get_precision () ثابت { بازگشت path.precision; }
  hb_blob_t *recycled_blob = nullptr;

  تهی new_path ()
  {
    flush_path ();
  }

  تهی flush_path ()
  {
    اگر (!path.length) بازگشت;
    انتخاب (format)
    {
      حالت HB_VECTOR_FORMAT_PDF: flush_path_pdf (); بشکن;
      حالت HB_VECTOR_FORMAT_SVG: flush_path_svg (); بشکن;
      حالت HB_VECTOR_FORMAT_INVALID: پیشفرض: بشکن;
    }
    path.shrink (0);
  }

  تهی flush_path_pdf ()
  {
    unsigned a = hb_color_get_alpha (foreground);
    اگر (a < 255)
    {
      unsigned gs_idx = pdf_extgstate_شمار++;
      body.append_str ("/GS");
      body.append_unsigned (gs_idx);
      body.append_str (" gs\n");

      pdf_extgstate_dict.append_str ("/GS");
      pdf_extgstate_dict.append_unsigned (gs_idx);
      pdf_extgstate_dict.append_str (" << /Type /ExtGState /ca ");
      pdf_extgstate_dict.append_رقم (a / 255.f, 4);
      pdf_extgstate_dict.append_str (" >> ");
    }
    body.append_رقم (hb_color_get_red (foreground) / 255.f, 4);
    body.append_c (' ');
    body.append_رقم (hb_color_get_green (foreground) / 255.f, 4);
    body.append_c (' ');
    body.append_رقم (hb_color_get_blue (foreground) / 255.f, 4);
    body.append_str (" rg\n");
    body.append_len (path.arrayZ, path.length);
    body.append_str ("f\n");
  }

  تهی flush_path_svg ()
  {
    body.append_str ("<path d=\"");
    body.append_len (path.arrayZ, path.length);
    body.append_str ("\" fill=\"");
    body.append_svg_color (foreground, درست);
    body.append_str ("\"/>\n");
  }

  تهی transform_xy (اعشاری x, اعشاری y, اعشاری *tx, اعشاری *ty)
  {
    hb_vector_transform_point (transform, x_scale_factor, y_scale_factor, x, y, tx, ty);
  }

  تهی append_xy (اعشاری x, اعشاری y, نویسه sep)
  {
    اعشاری tx, ty;
    transform_xy (x, y, &tx, &ty);
    path.append_رقم (tx, path.precision);
    path.append_c (sep);
    path.append_رقم (ty, path.precision);
  }

  تهی append_xy_svg (اعشاری x, اعشاری y) { append_xy (x, y, ','); }
  تهی append_xy_pdf (اعشاری x, اعشاری y) { append_xy (x, y, ' '); }
};

#endif /* HB_VECTOR_DRAW_HH */
