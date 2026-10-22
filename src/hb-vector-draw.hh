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

#جيڪڏهن_وصف_نه_ٿيل HB_ويڪٽر_DRAW_HH
#وصف_ڪريو HB_ويڪٽر_DRAW_HH

#ضمن "hb.hh"

#ضمن "hb-vector.h"
#ضمن "hb-geometry.hh"
#ضمن "hb-machinery.hh"
#ضمن "hb-vector-buf.hh"
#ضمن "hb-vector-internal.hh"

بنية hb_ويڪٽر_draw_t
{
  hb_object_سرو_t سرو;

  hb_ويڪٽر_format_t format = HB_ويڪٽر_FORMAT_INVALID;
  hb_transform_t<> transform = {1, 0, 0, 1, 0, 0};
  عائم x_scale_ضرب_ڪندڙ = 1.f;
  عائم y_scale_ضرب_ڪندڙ = 1.f;
  hb_ويڪٽر_extents_t extents = {0, 0, 0, 0};
  منطقي has_extents = خطأ;
  hb_color_t foreground = HB_COLOR (0, 0, 0, 255);
  hb_color_t background = HB_COLOR (0, 0, 0, 0);

  hb_ويڪٽر_buf_t defs;
  hb_ويڪٽر_buf_t body;
  hb_ويڪٽر_buf_t رستو;
  hb_ويڪٽر_buf_t pdf_extgstate_dict;
  بنانشان pdf_extgstate_ڳڻپ = 0;

  /* Cumulative work budget for the current draw session; reset by
   * hb_vector_draw_clear().  Outline traversal and complete path commands
   * charge the same live counter. */
  int64_t budget = HB_BUDGET_DEFAULT;
  int64_t budget_بچيل = HB_BUDGET_GLYPH;

  فراغ recharge_budget ()
  {
    budget_بچيل = budget == HB_BUDGET_DEFAULT ?
		       HB_BUDGET_GLYPH : budget;
  }

  منطقي begin_رستو_command ()
  {
    أرجع likely (budget_بچيل >= 0 && !رستو.اندريون_error ());
  }

  فراغ پڇاڙي_رستو_command ()
  {
    /* The serialized bytes are bounded by the output buffer's document-size
     * cap (in_error), not the outline work budget; stop outline traversal
     * once that cap or an allocation error trips. */
    إذا (unlikely (رستو.اندريون_error ()))
      budget_بچيل = 0;
  }

  فراغ سيٽ_precision (بنانشان p)
  {
    p = hb_min (p, 12u);
    defs.precision = p;
    body.precision = p;
    رستو.precision = p;
  }

  بنانشان get_precision () ثابت { أرجع رستو.precision; }
  hb_blob_t *recycled_blob = خالي_اشارو;

  فراغ new_رستو ()
  {
    flush_رستو ();
  }

  فراغ flush_رستو ()
  {
    إذا (!رستو.ڊيگهائي) أرجع;
    بدل (format)
    {
      حالة HB_ويڪٽر_FORMAT_PDF: flush_رستو_pdf (); اكسر;
      حالة HB_ويڪٽر_FORMAT_SVG: flush_رستو_svg (); اكسر;
      حالة HB_ويڪٽر_FORMAT_INVALID: افتراضي: اكسر;
    }
    رستو.shrink (0);
  }

  فراغ flush_رستو_pdf ()
  {
    بنانشان a = hb_color_get_alpha (foreground);
    إذا (a < 255)
    {
      بنانشان gs_idx = pdf_extgstate_ڳڻپ++;
      body.append_str ("/GS");
      body.append_unsigned (gs_idx);
      body.append_str (" gs\n");

      pdf_extgstate_dict.append_str ("/GS");
      pdf_extgstate_dict.append_unsigned (gs_idx);
      pdf_extgstate_dict.append_str (" << /Type /ExtGState /ca ");
      pdf_extgstate_dict.append_انگ (a / 255.f, 4);
      pdf_extgstate_dict.append_str (" >> ");
    }
    body.append_انگ (hb_color_get_red (foreground) / 255.f, 4);
    body.append_c (' ');
    body.append_انگ (hb_color_get_green (foreground) / 255.f, 4);
    body.append_c (' ');
    body.append_انگ (hb_color_get_blue (foreground) / 255.f, 4);
    body.append_str (" rg\n");
    body.append_len (رستو.arrayZ, رستو.ڊيگهائي);
    body.append_str ("f\n");
  }

  فراغ flush_رستو_svg ()
  {
    body.append_str ("<path d=\"");
    body.append_len (رستو.arrayZ, رستو.ڊيگهائي);
    body.append_str ("\" fill=\"");
    body.append_svg_color (foreground, سچ);
    body.append_str ("\"/>\n");
  }

  فراغ transform_xy (عائم x, عائم y, عائم *tx, عائم *ty)
  {
    hb_ويڪٽر_transform_نقطو (transform, x_scale_ضرب_ڪندڙ, y_scale_ضرب_ڪندڙ, x, y, tx, ty);
  }

  فراغ append_xy (عائم x, عائم y, حرف sep)
  {
    عائم tx, ty;
    transform_xy (x, y, &tx, &ty);
    رستو.append_انگ (tx, رستو.precision);
    رستو.append_c (sep);
    رستو.append_انگ (ty, رستو.precision);
  }

  فراغ append_xy_svg (عائم x, عائم y) { append_xy (x, y, ','); }
  فراغ append_xy_pdf (عائم x, عائم y) { append_xy (x, y, ' '); }
};

#جيڪڏهن_پورو /* HB_VECTOR_DRAW_HH */
