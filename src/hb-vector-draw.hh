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

#جے_بنا_تعریف HB_سمتیہ_DRAW_HH
#تعریف_کرو HB_سمتیہ_DRAW_HH

#ضمن "hb.hh"

#ضمن "hb-vector.h"
#ضمن "hb-geometry.hh"
#ضمن "hb-machinery.hh"
#ضمن "hb-vector-buf.hh"
#ضمن "hb-vector-internal.hh"

بنية hb_سمتیہ_draw_t
{
  hb_object_سرخی_t سرخی;

  hb_سمتیہ_format_t format = HB_سمتیہ_FORMAT_INVALID;
  hb_transform_t<> transform = {1, 0, 0, 1, 0, 0};
  عائم x_scale_ضارب = 1.f;
  عائم y_scale_ضارب = 1.f;
  hb_سمتیہ_extents_t extents = {0, 0, 0, 0};
  منطقي has_extents = خطأ;
  hb_color_t foreground = HB_COLOR (0, 0, 0, 255);
  hb_color_t background = HB_COLOR (0, 0, 0, 0);

  hb_سمتیہ_buf_t defs;
  hb_سمتیہ_buf_t body;
  hb_سمتیہ_buf_t رستہ;
  hb_سمتیہ_buf_t pdf_extgstate_dict;
  بنانشانی pdf_extgstate_گنتی = 0;

  /* Cumulative work budget for the current draw session; reset by
   * hb_vector_draw_clear().  Outline traversal and complete path commands
   * charge the same live counter. */
  int64_t budget = HB_BUDGET_DEFAULT;
  int64_t budget_بچیا = HB_BUDGET_GLYPH;

  فراغ recharge_budget ()
  {
    budget_بچیا = budget == HB_BUDGET_DEFAULT ?
		       HB_BUDGET_GLYPH : budget;
  }

  منطقي begin_رستہ_command ()
  {
    أرجع likely (budget_بچیا >= 0 && !رستہ.وچ_error ());
  }

  فراغ سرا_رستہ_command ()
  {
    /* The serialized bytes are bounded by the output buffer's document-size
     * cap (in_error), not the outline work budget; stop outline traversal
     * once that cap or an allocation error trips. */
    إذا (unlikely (رستہ.وچ_error ()))
      budget_بچیا = 0;
  }

  فراغ سیٹ_precision (بنانشانی p)
  {
    p = hb_min (p, 12u);
    defs.precision = p;
    body.precision = p;
    رستہ.precision = p;
  }

  بنانشانی get_precision () ثابت { أرجع رستہ.precision; }
  hb_blob_t *recycled_blob = خالی_سوچک;

  فراغ new_رستہ ()
  {
    flush_رستہ ();
  }

  فراغ flush_رستہ ()
  {
    إذا (!رستہ.لمائی) أرجع;
    بدل (format)
    {
      حالة HB_سمتیہ_FORMAT_PDF: flush_رستہ_pdf (); اكسر;
      حالة HB_سمتیہ_FORMAT_SVG: flush_رستہ_svg (); اكسر;
      حالة HB_سمتیہ_FORMAT_INVALID: افتراضي: اكسر;
    }
    رستہ.shrink (0);
  }

  فراغ flush_رستہ_pdf ()
  {
    بنانشانی a = hb_color_get_alpha (foreground);
    إذا (a < 255)
    {
      بنانشانی gs_idx = pdf_extgstate_گنتی++;
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
    body.append_len (رستہ.arrayZ, رستہ.لمائی);
    body.append_str ("f\n");
  }

  فراغ flush_رستہ_svg ()
  {
    body.append_str ("<path d=\"");
    body.append_len (رستہ.arrayZ, رستہ.لمائی);
    body.append_str ("\" fill=\"");
    body.append_svg_color (foreground, سچ);
    body.append_str ("\"/>\n");
  }

  فراغ transform_xy (عائم x, عائم y, عائم *tx, عائم *ty)
  {
    hb_سمتیہ_transform_نقطہ (transform, x_scale_ضارب, y_scale_ضارب, x, y, tx, ty);
  }

  فراغ append_xy (عائم x, عائم y, حرف sep)
  {
    عائم tx, ty;
    transform_xy (x, y, &tx, &ty);
    رستہ.append_عدد (tx, رستہ.precision);
    رستہ.append_c (sep);
    رستہ.append_عدد (ty, رستہ.precision);
  }

  فراغ append_xy_svg (عائم x, عائم y) { append_xy (x, y, ','); }
  فراغ append_xy_pdf (عائم x, عائم y) { append_xy (x, y, ' '); }
};

#جے_انت /* HB_VECTOR_DRAW_HH */
