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

#اگر_تعریف_نشده HB_وکتور_DRAW_HH
#تعریف_کن HB_وکتور_DRAW_HH

#ضمن "hb.hh"

#ضمن "hb-vector.h"
#ضمن "hb-geometry.hh"
#ضمن "hb-machinery.hh"
#ضمن "hb-vector-buf.hh"
#ضمن "hb-vector-internal.hh"

بنية hb_وکتور_draw_t
{
  hb_object_سرآیند_t سرآیند;

  hb_وکتور_format_t format = HB_وکتور_FORMAT_INVALID;
  hb_transform_t<> transform = {1, 0, 0, 1, 0, 0};
  عائم x_scale_ضریب = 1.f;
  عائم y_scale_ضریب = 1.f;
  hb_وکتور_extents_t extents = {0, 0, 0, 0};
  منطقي has_extents = خطأ;
  hb_color_t foreground = HB_COLOR (0, 0, 0, 255);
  hb_color_t background = HB_COLOR (0, 0, 0, 0);

  hb_وکتور_buf_t defs;
  hb_وکتور_buf_t body;
  hb_وکتور_buf_t مسیر;
  hb_وکتور_buf_t pdf_extgstate_dict;
  بیعلامت pdf_extgstate_شمار = 0;

  /* Cumulative work budget for the current draw session; reset by
   * hb_vector_draw_clear().  Outline traversal and complete path commands
   * charge the same live counter. */
  int64_t budget = HB_BUDGET_DEFAULT;
  int64_t budget_مانده = HB_BUDGET_GLYPH;

  فراغ recharge_budget ()
  {
    budget_مانده = budget == HB_BUDGET_DEFAULT ?
		       HB_BUDGET_GLYPH : budget;
  }

  منطقي begin_مسیر_command ()
  {
    أرجع likely (budget_مانده >= 0 && !مسیر.درون_error ());
  }

  فراغ انتها_مسیر_command ()
  {
    /* The serialized bytes are bounded by the output buffer's document-size
     * cap (in_error), not the outline work budget; stop outline traversal
     * once that cap or an allocation error trips. */
    إذا (unlikely (مسیر.درون_error ()))
      budget_مانده = 0;
  }

  فراغ مجموعه_precision (بیعلامت p)
  {
    p = hb_min (p, 12u);
    defs.precision = p;
    body.precision = p;
    مسیر.precision = p;
  }

  بیعلامت get_precision () ثابت { أرجع مسیر.precision; }
  hb_blob_t *recycled_blob = اشاره_تهی;

  فراغ new_مسیر ()
  {
    flush_مسیر ();
  }

  فراغ flush_مسیر ()
  {
    إذا (!مسیر.درازی) أرجع;
    بدل (format)
    {
      حالة HB_وکتور_FORMAT_PDF: flush_مسیر_pdf (); اكسر;
      حالة HB_وکتور_FORMAT_SVG: flush_مسیر_svg (); اكسر;
      حالة HB_وکتور_FORMAT_INVALID: افتراضي: اكسر;
    }
    مسیر.shrink (0);
  }

  فراغ flush_مسیر_pdf ()
  {
    بیعلامت a = hb_color_get_alpha (foreground);
    إذا (a < 255)
    {
      بیعلامت gs_idx = pdf_extgstate_شمار++;
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
    body.append_len (مسیر.arrayZ, مسیر.درازی);
    body.append_str ("f\n");
  }

  فراغ flush_مسیر_svg ()
  {
    body.append_str ("<path d=\"");
    body.append_len (مسیر.arrayZ, مسیر.درازی);
    body.append_str ("\" fill=\"");
    body.append_svg_color (foreground, درست);
    body.append_str ("\"/>\n");
  }

  فراغ transform_xy (عائم x, عائم y, عائم *tx, عائم *ty)
  {
    hb_وکتور_transform_نقطه (transform, x_scale_ضریب, y_scale_ضریب, x, y, tx, ty);
  }

  فراغ append_xy (عائم x, عائم y, حرف sep)
  {
    عائم tx, ty;
    transform_xy (x, y, &tx, &ty);
    مسیر.append_رقم (tx, مسیر.precision);
    مسیر.append_c (sep);
    مسیر.append_رقم (ty, مسیر.precision);
  }

  فراغ append_xy_svg (عائم x, عائم y) { append_xy (x, y, ','); }
  فراغ append_xy_pdf (عائم x, عائم y) { append_xy (x, y, ' '); }
};

#پایان_اگر /* HB_VECTOR_DRAW_HH */
