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

#অন্তর্ভাৱ "hb.hh"

#অন্তর্ভাৱ "hb-vector.h"
#অন্তর্ভাৱ "hb-geometry.hh"
#অন্তর্ভাৱ "hb-machinery.hh"
#অন্তর্ভাৱ "hb-vector-buf.hh"
#অন্তর্ভাৱ "hb-vector-internal.hh"

সংরচনা hb_vector_draw_t
{
  hb_object_header_t header;

  hb_vector_format_t format = HB_VECTOR_FORMAT_INVALID;
  hb_transform_t<> transform = {1, 0, 0, 1, 0, 0};
  ভিন্ন x_scale_factor = 1.f;
  ভিন্ন y_scale_factor = 1.f;
  hb_vector_extents_t extents = {0, 0, 0, 0};
  তার্কিক has_extents = মিছা;
  hb_color_t foreground = HB_COLOR (0, 0, 0, 255);
  hb_color_t background = HB_COLOR (0, 0, 0, 0);

  hb_vector_buf_t defs;
  hb_vector_buf_t body;
  hb_vector_buf_t path;
  hb_vector_buf_t pdf_extgstate_dict;
  unsigned pdf_extgstate_গুনতি = 0;

  /* Cumulative work budget for the current draw session; reset by
   * hb_vector_draw_clear().  Outline traversal and complete path commands
   * charge the same live counter. */
  int64_t budget = HB_BUDGET_DEFAULT;
  int64_t budget_remaining = HB_BUDGET_GLYPH;

  শূন্য recharge_budget ()
  {
    budget_remaining = budget == HB_BUDGET_DEFAULT ?
		       HB_BUDGET_GLYPH : budget;
  }

  তার্কিক begin_path_command ()
  {
    ঘূৰাই likely (budget_remaining >= 0 && !path.in_error ());
  }

  শূন্য end_path_command ()
  {
    /* The serialized bytes are bounded by the output buffer's document-size
     * cap (in_error), not the outline work budget; stop outline traversal
     * once that cap or an allocation error trips. */
    যদি (unlikely (path.in_error ()))
      budget_remaining = 0;
  }

  শূন্য set_precision (unsigned p)
  {
    p = hb_min (p, 12u);
    defs.precision = p;
    body.precision = p;
    path.precision = p;
  }

  unsigned get_precision () ধ্রুৱ { ঘূৰাই path.precision; }
  hb_blob_t *recycled_blob = nullptr;

  শূন্য new_path ()
  {
    flush_path ();
  }

  শূন্য flush_path ()
  {
    যদি (!path.length) ঘূৰাই;
    ৱিকল্প (format)
    {
      পক্ষ HB_VECTOR_FORMAT_PDF: flush_path_pdf (); ভঙ্গ;
      পক্ষ HB_VECTOR_FORMAT_SVG: flush_path_svg (); ভঙ্গ;
      পক্ষ HB_VECTOR_FORMAT_INVALID: স্ৱভাৱ: ভঙ্গ;
    }
    path.shrink (0);
  }

  শূন্য flush_path_pdf ()
  {
    unsigned a = hb_color_get_alpha (foreground);
    যদি (a < 255)
    {
      unsigned gs_idx = pdf_extgstate_গুনতি++;
      body.append_str ("/GS");
      body.append_unsigned (gs_idx);
      body.append_str (" gs\n");

      pdf_extgstate_dict.append_str ("/GS");
      pdf_extgstate_dict.append_unsigned (gs_idx);
      pdf_extgstate_dict.append_str (" << /Type /ExtGState /ca ");
      pdf_extgstate_dict.append_অঙ্ক (a / 255.f, 4);
      pdf_extgstate_dict.append_str (" >> ");
    }
    body.append_অঙ্ক (hb_color_get_red (foreground) / 255.f, 4);
    body.append_c (' ');
    body.append_অঙ্ক (hb_color_get_green (foreground) / 255.f, 4);
    body.append_c (' ');
    body.append_অঙ্ক (hb_color_get_blue (foreground) / 255.f, 4);
    body.append_str (" rg\n");
    body.append_len (path.arrayZ, path.length);
    body.append_str ("f\n");
  }

  শূন্য flush_path_svg ()
  {
    body.append_str ("<path d=\"");
    body.append_len (path.arrayZ, path.length);
    body.append_str ("\" fill=\"");
    body.append_svg_color (foreground, সঁচা);
    body.append_str ("\"/>\n");
  }

  শূন্য transform_xy (ভিন্ন x, ভিন্ন y, ভিন্ন *tx, ভিন্ন *ty)
  {
    hb_vector_transform_point (transform, x_scale_factor, y_scale_factor, x, y, tx, ty);
  }

  শূন্য append_xy (ভিন্ন x, ভিন্ন y, ৱর্ণ sep)
  {
    ভিন্ন tx, ty;
    transform_xy (x, y, &tx, &ty);
    path.append_অঙ্ক (tx, path.precision);
    path.append_c (sep);
    path.append_অঙ্ক (ty, path.precision);
  }

  শূন্য append_xy_svg (ভিন্ন x, ভিন্ন y) { append_xy (x, y, ','); }
  শূন্য append_xy_pdf (ভিন্ন x, ভিন্ন y) { append_xy (x, y, ' '); }
};

#endif /* HB_VECTOR_DRAW_HH */
