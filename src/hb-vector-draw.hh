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

#که_نه_تعریف_شوی HB_ویکټور_DRAW_HH
#تعریف_کړه HB_ویکټور_DRAW_HH

#ضمن "hb.hh"

#ضمن "hb-vector.h"
#ضمن "hb-geometry.hh"
#ضمن "hb-machinery.hh"
#ضمن "hb-vector-buf.hh"
#ضمن "hb-vector-internal.hh"

بنية hb_ویکټور_draw_t
{
  hb_object_سرلیک_t سرلیک;

  hb_ویکټور_format_t format = HB_ویکټور_FORMAT_INVALID;
  hb_transform_t<> transform = {1, 0, 0, 1, 0, 0};
  عائم x_scale_ضریب = 1.f;
  عائم y_scale_ضریب = 1.f;
  hb_ویکټور_extents_t extents = {0, 0, 0, 0};
  منطقي has_extents = خطأ;
  hb_color_t foreground = HB_COLOR (0, 0, 0, 255);
  hb_color_t background = HB_COLOR (0, 0, 0, 0);

  hb_ویکټور_buf_t defs;
  hb_ویکټور_buf_t body;
  hb_ویکټور_buf_t لار;
  hb_ویکټور_buf_t pdf_extgstate_dict;
  بینښه pdf_extgstate_شمېر = 0;

  /* Cumulative work budget for the current draw session; reset by
   * hb_vector_draw_clear().  Outline traversal and complete path commands
   * charge the same live counter. */
  int64_t budget = HB_BUDGET_DEFAULT;
  int64_t budget_پاتې_شوی = HB_BUDGET_GLYPH;

  فراغ recharge_budget ()
  {
    budget_پاتې_شوی = budget == HB_BUDGET_DEFAULT ?
		       HB_BUDGET_GLYPH : budget;
  }

  منطقي begin_لار_command ()
  {
    أرجع likely (budget_پاتې_شوی >= 0 && !لار.دننه_error ());
  }

  فراغ پای_ټکی_لار_command ()
  {
    /* The serialized bytes are bounded by the output buffer's document-size
     * cap (in_error), not the outline work budget; stop outline traversal
     * once that cap or an allocation error trips. */
    إذا (unlikely (لار.دننه_error ()))
      budget_پاتې_شوی = 0;
  }

  فراغ سېټ_precision (بینښه p)
  {
    p = hb_min (p, 12u);
    defs.precision = p;
    body.precision = p;
    لار.precision = p;
  }

  بینښه get_precision () ثابت { أرجع لار.precision; }
  hb_blob_t *recycled_blob = تش_اشاره;

  فراغ new_لار ()
  {
    flush_لار ();
  }

  فراغ flush_لار ()
  {
    إذا (!لار.اوږدتیا) أرجع;
    بدل (format)
    {
      حالة HB_ویکټور_FORMAT_PDF: flush_لار_pdf (); اكسر;
      حالة HB_ویکټور_FORMAT_SVG: flush_لار_svg (); اكسر;
      حالة HB_ویکټور_FORMAT_INVALID: افتراضي: اكسر;
    }
    لار.shrink (0);
  }

  فراغ flush_لار_pdf ()
  {
    بینښه a = hb_color_get_alpha (foreground);
    إذا (a < 255)
    {
      بینښه gs_idx = pdf_extgstate_شمېر++;
      body.append_str ("/GS");
      body.append_unsigned (gs_idx);
      body.append_str (" gs\n");

      pdf_extgstate_dict.append_str ("/GS");
      pdf_extgstate_dict.append_unsigned (gs_idx);
      pdf_extgstate_dict.append_str (" << /Type /ExtGState /ca ");
      pdf_extgstate_dict.append_عدد (a / 255.f, 4);
      pdf_extgstate_dict.append_str (" >> ");
    }
    body.append_عدد (hb_color_get_red (foreground) / 255.f, 4);
    body.append_c (' ');
    body.append_عدد (hb_color_get_green (foreground) / 255.f, 4);
    body.append_c (' ');
    body.append_عدد (hb_color_get_blue (foreground) / 255.f, 4);
    body.append_str (" rg\n");
    body.append_len (لار.arrayZ, لار.اوږدتیا);
    body.append_str ("f\n");
  }

  فراغ flush_لار_svg ()
  {
    body.append_str ("<path d=\"");
    body.append_len (لار.arrayZ, لار.اوږدتیا);
    body.append_str ("\" fill=\"");
    body.append_svg_color (foreground, ریښتیا);
    body.append_str ("\"/>\n");
  }

  فراغ transform_xy (عائم x, عائم y, عائم *tx, عائم *ty)
  {
    hb_ویکټور_transform_ټکی (transform, x_scale_ضریب, y_scale_ضریب, x, y, tx, ty);
  }

  فراغ append_xy (عائم x, عائم y, حرف sep)
  {
    عائم tx, ty;
    transform_xy (x, y, &tx, &ty);
    لار.append_عدد (tx, لار.precision);
    لار.append_c (sep);
    لار.append_عدد (ty, لار.precision);
  }

  فراغ append_xy_svg (عائم x, عائم y) { append_xy (x, y, ','); }
  فراغ append_xy_pdf (عائم x, عائم y) { append_xy (x, y, ' '); }
};

#د_که_پای /* HB_VECTOR_DRAW_HH */
