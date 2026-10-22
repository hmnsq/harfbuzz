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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_ਸਦਿਸ਼_DRAW_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_ਸਦਿਸ਼_DRAW_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-vector.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-geometry.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-machinery.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-vector-buf.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-vector-internal.hh"

ਬਣਤਰ hb_ਸਦਿਸ਼_draw_t
{
  hb_object_ਸਿਰਲੇਖ_t ਸਿਰਲੇਖ;

  hb_ਸਦਿਸ਼_format_t format = HB_ਸਦਿਸ਼_FORMAT_INVALID;
  hb_transform_t<> transform = {1, 0, 0, 1, 0, 0};
  ਦਸ਼ਮਲਵ x_scale_ਗੁਣਕ = 1.f;
  ਦਸ਼ਮਲਵ y_scale_ਗੁਣਕ = 1.f;
  hb_ਸਦਿਸ਼_extents_t extents = {0, 0, 0, 0};
  ਤਾਰਕਿਕ has_extents = ਝੂਠ;
  hb_color_t foreground = HB_COLOR (0, 0, 0, 255);
  hb_color_t background = HB_COLOR (0, 0, 0, 0);

  hb_ਸਦਿਸ਼_buf_t defs;
  hb_ਸਦਿਸ਼_buf_t body;
  hb_ਸਦਿਸ਼_buf_t ਰਸਤਾ;
  hb_ਸਦਿਸ਼_buf_t pdf_extgstate_dict;
  ਬਿਨਾਨਿਸ਼ਾਨੀ pdf_extgstate_ਗਿਣਤੀ = 0;

  /* Cumulative work budget for the current draw session; reset by
   * hb_vector_draw_clear().  Outline traversal and complete path commands
   * charge the same live counter. */
  int64_t budget = HB_BUDGET_DEFAULT;
  int64_t budget_ਬਚਿਆ = HB_BUDGET_GLYPH;

  ਖਾਲੀ recharge_budget ()
  {
    budget_ਬਚਿਆ = budget == HB_BUDGET_DEFAULT ?
		       HB_BUDGET_GLYPH : budget;
  }

  ਤਾਰਕਿਕ begin_ਰਸਤਾ_command ()
  {
    ਵਾਪਸ likely (budget_ਬਚਿਆ >= 0 && !ਰਸਤਾ.ਵਿੱਚ_error ());
  }

  ਖਾਲੀ ਸਿਰਾ_ਰਸਤਾ_command ()
  {
    /* The serialized bytes are bounded by the output buffer's document-size
     * cap (in_error), not the outline work budget; stop outline traversal
     * once that cap or an allocation error trips. */
    ਜੇ (unlikely (ਰਸਤਾ.ਵਿੱਚ_error ()))
      budget_ਬਚਿਆ = 0;
  }

  ਖਾਲੀ ਸੈੱਟ_precision (ਬਿਨਾਨਿਸ਼ਾਨੀ p)
  {
    p = hb_min (p, 12u);
    defs.precision = p;
    body.precision = p;
    ਰਸਤਾ.precision = p;
  }

  ਬਿਨਾਨਿਸ਼ਾਨੀ get_precision () ਅਚਲ { ਵਾਪਸ ਰਸਤਾ.precision; }
  hb_blob_t *recycled_blob = ਖਾਲੀ_ਸੂਚਕ;

  ਖਾਲੀ new_ਰਸਤਾ ()
  {
    flush_ਰਸਤਾ ();
  }

  ਖਾਲੀ flush_ਰਸਤਾ ()
  {
    ਜੇ (!ਰਸਤਾ.ਲੰਮਾਈ) ਵਾਪਸ;
    ਚੋਣ (format)
    {
      ਹਾਲਤ HB_ਸਦਿਸ਼_FORMAT_PDF: flush_ਰਸਤਾ_pdf (); ਰੋਕੋ;
      ਹਾਲਤ HB_ਸਦਿਸ਼_FORMAT_SVG: flush_ਰਸਤਾ_svg (); ਰੋਕੋ;
      ਹਾਲਤ HB_ਸਦਿਸ਼_FORMAT_INVALID: ਮੂਲ: ਰੋਕੋ;
    }
    ਰਸਤਾ.shrink (0);
  }

  ਖਾਲੀ flush_ਰਸਤਾ_pdf ()
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ a = hb_color_get_alpha (foreground);
    ਜੇ (a < 255)
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ gs_idx = pdf_extgstate_ਗਿਣਤੀ++;
      body.append_str ("/GS");
      body.append_unsigned (gs_idx);
      body.append_str (" gs\n");

      pdf_extgstate_dict.append_str ("/GS");
      pdf_extgstate_dict.append_unsigned (gs_idx);
      pdf_extgstate_dict.append_str (" << /Type /ExtGState /ca ");
      pdf_extgstate_dict.append_ਅੰਕ (a / 255.f, 4);
      pdf_extgstate_dict.append_str (" >> ");
    }
    body.append_ਅੰਕ (hb_color_get_red (foreground) / 255.f, 4);
    body.append_c (' ');
    body.append_ਅੰਕ (hb_color_get_green (foreground) / 255.f, 4);
    body.append_c (' ');
    body.append_ਅੰਕ (hb_color_get_blue (foreground) / 255.f, 4);
    body.append_str (" rg\n");
    body.append_len (ਰਸਤਾ.arrayZ, ਰਸਤਾ.ਲੰਮਾਈ);
    body.append_str ("f\n");
  }

  ਖਾਲੀ flush_ਰਸਤਾ_svg ()
  {
    body.append_str ("<path d=\"");
    body.append_len (ਰਸਤਾ.arrayZ, ਰਸਤਾ.ਲੰਮਾਈ);
    body.append_str ("\" fill=\"");
    body.append_svg_color (foreground, ਸੱਚ);
    body.append_str ("\"/>\n");
  }

  ਖਾਲੀ transform_xy (ਦਸ਼ਮਲਵ x, ਦਸ਼ਮਲਵ y, ਦਸ਼ਮਲਵ *tx, ਦਸ਼ਮਲਵ *ty)
  {
    hb_ਸਦਿਸ਼_transform_ਬਿੰਦੂ (transform, x_scale_ਗੁਣਕ, y_scale_ਗੁਣਕ, x, y, tx, ty);
  }

  ਖਾਲੀ append_xy (ਦਸ਼ਮਲਵ x, ਦਸ਼ਮਲਵ y, ਅੱਖਰ sep)
  {
    ਦਸ਼ਮਲਵ tx, ty;
    transform_xy (x, y, &tx, &ty);
    ਰਸਤਾ.append_ਅੰਕ (tx, ਰਸਤਾ.precision);
    ਰਸਤਾ.append_c (sep);
    ਰਸਤਾ.append_ਅੰਕ (ty, ਰਸਤਾ.precision);
  }

  ਖਾਲੀ append_xy_svg (ਦਸ਼ਮਲਵ x, ਦਸ਼ਮਲਵ y) { append_xy (x, y, ','); }
  ਖਾਲੀ append_xy_pdf (ਦਸ਼ਮਲਵ x, ਦਸ਼ਮਲਵ y) { append_xy (x, y, ' '); }
};

#ਜੇ_ਅੰਤ /* HB_VECTOR_DRAW_HH */
