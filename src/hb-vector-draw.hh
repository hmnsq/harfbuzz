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

#اگر_غیر_تعریف_شدہ HB_سمتیہ_DRAW_HH
#تعریف_کرو HB_سمتیہ_DRAW_HH

#شامل "hb.hh"

#شامل "hb-vector.h"
#شامل "hb-geometry.hh"
#شامل "hb-machinery.hh"
#شامل "hb-vector-buf.hh"
#شامل "hb-vector-internal.hh"

ساخت hb_سمتیہ_draw_t
{
  hb_object_سرخی_t سرخی;

  hb_سمتیہ_format_t format = HB_سمتیہ_FORMAT_INVALID;
  hb_transform_t<> transform = {1, 0, 0, 1, 0, 0};
  اعشاری x_scale_ضربی_عدد = 1.f;
  اعشاری y_scale_ضربی_عدد = 1.f;
  hb_سمتیہ_extents_t extents = {0, 0, 0, 0};
  منطقی has_extents = جھوٹ;
  hb_color_t foreground = HB_COLOR (0, 0, 0, 255);
  hb_color_t background = HB_COLOR (0, 0, 0, 0);

  hb_سمتیہ_buf_t defs;
  hb_سمتیہ_buf_t body;
  hb_سمتیہ_buf_t راستہ;
  hb_سمتیہ_buf_t pdf_extgstate_dict;
  غیرعلامتی pdf_extgstate_گنتی = 0;

  /* Cumulative work budget for the current draw session; reset by
   * hb_vector_draw_clear().  Outline traversal and complete path commands
   * charge the same live counter. */
  int64_t budget = HB_BUDGET_DEFAULT;
  int64_t budget_بقایا = HB_BUDGET_GLYPH;

  خالی recharge_budget ()
  {
    budget_بقایا = budget == HB_BUDGET_DEFAULT ?
		       HB_BUDGET_GLYPH : budget;
  }

  منطقی begin_راستہ_command ()
  {
    واپس likely (budget_بقایا >= 0 && !راستہ.اندر_error ());
  }

  خالی انتہا_راستہ_command ()
  {
    /* The serialized bytes are bounded by the output buffer's document-size
     * cap (in_error), not the outline work budget; stop outline traversal
     * once that cap or an allocation error trips. */
    اگر (unlikely (راستہ.اندر_error ()))
      budget_بقایا = 0;
  }

  خالی سیٹ_precision (غیرعلامتی p)
  {
    p = hb_min (p, 12u);
    defs.precision = p;
    body.precision = p;
    راستہ.precision = p;
  }

  غیرعلامتی get_precision () ثابت { واپس راستہ.precision; }
  hb_blob_t *recycled_blob = خالی_اشاریہ;

  خالی new_راستہ ()
  {
    flush_راستہ ();
  }

  خالی flush_راستہ ()
  {
    اگر (!راستہ.لمبائی) واپس;
    انتخاب (format)
    {
      حالت HB_سمتیہ_FORMAT_PDF: flush_راستہ_pdf (); توڑو;
      حالت HB_سمتیہ_FORMAT_SVG: flush_راستہ_svg (); توڑو;
      حالت HB_سمتیہ_FORMAT_INVALID: طےشدہ: توڑو;
    }
    راستہ.shrink (0);
  }

  خالی flush_راستہ_pdf ()
  {
    غیرعلامتی a = hb_color_get_alpha (foreground);
    اگر (a < 255)
    {
      غیرعلامتی gs_idx = pdf_extgstate_گنتی++;
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
    body.append_len (راستہ.arrayZ, راستہ.لمبائی);
    body.append_str ("f\n");
  }

  خالی flush_راستہ_svg ()
  {
    body.append_str ("<path d=\"");
    body.append_len (راستہ.arrayZ, راستہ.لمبائی);
    body.append_str ("\" fill=\"");
    body.append_svg_color (foreground, سچ);
    body.append_str ("\"/>\n");
  }

  خالی transform_xy (اعشاری x, اعشاری y, اعشاری *tx, اعشاری *ty)
  {
    hb_سمتیہ_transform_نقطہ (transform, x_scale_ضربی_عدد, y_scale_ضربی_عدد, x, y, tx, ty);
  }

  خالی append_xy (اعشاری x, اعشاری y, حرف sep)
  {
    اعشاری tx, ty;
    transform_xy (x, y, &tx, &ty);
    راستہ.append_عدد (tx, راستہ.precision);
    راستہ.append_c (sep);
    راستہ.append_عدد (ty, راستہ.precision);
  }

  خالی append_xy_svg (اعشاری x, اعشاری y) { append_xy (x, y, ','); }
  خالی append_xy_pdf (اعشاری x, اعشاری y) { append_xy (x, y, ' '); }
};

#اگر_ختم /* HB_VECTOR_DRAW_HH */
