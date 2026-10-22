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

#ಅನ್ತರ್ಭಾವ "hb.hh"

#ಅನ್ತರ್ಭಾವ "hb-vector.h"
#ಅನ್ತರ್ಭಾವ "hb-geometry.hh"
#ಅನ್ತರ್ಭಾವ "hb-machinery.hh"
#ಅನ್ತರ್ಭಾವ "hb-vector-buf.hh"
#ಅನ್ತರ್ಭಾವ "hb-vector-internal.hh"

ಸಂರಚನಾ hb_vector_draw_t
{
  hb_object_header_t header;

  hb_vector_format_t format = HB_VECTOR_FORMAT_INVALID;
  hb_transform_t<> transform = {1, 0, 0, 1, 0, 0};
  ಭಿನ್ನ x_scale_factor = 1.f;
  ಭಿನ್ನ y_scale_factor = 1.f;
  hb_vector_extents_t extents = {0, 0, 0, 0};
  ತಾರ್ಕಿಕ has_extents = ಅಸತ್ಯ;
  hb_color_t foreground = HB_COLOR (0, 0, 0, 255);
  hb_color_t background = HB_COLOR (0, 0, 0, 0);

  hb_vector_buf_t defs;
  hb_vector_buf_t body;
  hb_vector_buf_t path;
  hb_vector_buf_t pdf_extgstate_dict;
  unsigned pdf_extgstate_ಎಣಿಕೆ = 0;

  /* Cumulative work budget for the current draw session; reset by
   * hb_vector_draw_clear().  Outline traversal and complete path commands
   * charge the same live counter. */
  int64_t budget = HB_BUDGET_DEFAULT;
  int64_t budget_remaining = HB_BUDGET_GLYPH;

  ಶೂನ್ಯ recharge_budget ()
  {
    budget_remaining = budget == HB_BUDGET_DEFAULT ?
		       HB_BUDGET_GLYPH : budget;
  }

  ತಾರ್ಕಿಕ begin_path_command ()
  {
    ಪ್ರತ್ಯಾಗಮ likely (budget_remaining >= 0 && !path.in_error ());
  }

  ಶೂನ್ಯ end_path_command ()
  {
    /* The serialized bytes are bounded by the output buffer's document-size
     * cap (in_error), not the outline work budget; stop outline traversal
     * once that cap or an allocation error trips. */
    ಯದಿ (unlikely (path.in_error ()))
      budget_remaining = 0;
  }

  ಶೂನ್ಯ set_precision (unsigned p)
  {
    p = hb_min (p, 12u);
    defs.precision = p;
    body.precision = p;
    path.precision = p;
  }

  unsigned get_precision () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ path.precision; }
  hb_blob_t *recycled_blob = nullptr;

  ಶೂನ್ಯ new_path ()
  {
    flush_path ();
  }

  ಶೂನ್ಯ flush_path ()
  {
    ಯದಿ (!path.length) ಪ್ರತ್ಯಾಗಮ;
    ವಿಕಲ್ಪ (format)
    {
      ಪಕ್ಷ HB_VECTOR_FORMAT_PDF: flush_path_pdf (); ಭಙ್ಗ;
      ಪಕ್ಷ HB_VECTOR_FORMAT_SVG: flush_path_svg (); ಭಙ್ಗ;
      ಪಕ್ಷ HB_VECTOR_FORMAT_INVALID: ಸ್ವಭಾವ: ಭಙ್ಗ;
    }
    path.shrink (0);
  }

  ಶೂನ್ಯ flush_path_pdf ()
  {
    unsigned a = hb_color_get_alpha (foreground);
    ಯದಿ (a < 255)
    {
      unsigned gs_idx = pdf_extgstate_ಎಣಿಕೆ++;
      body.append_str ("/GS");
      body.append_unsigned (gs_idx);
      body.append_str (" gs\n");

      pdf_extgstate_dict.append_str ("/GS");
      pdf_extgstate_dict.append_unsigned (gs_idx);
      pdf_extgstate_dict.append_str (" << /Type /ExtGState /ca ");
      pdf_extgstate_dict.append_ಅಂಕೆ (a / 255.f, 4);
      pdf_extgstate_dict.append_str (" >> ");
    }
    body.append_ಅಂಕೆ (hb_color_get_red (foreground) / 255.f, 4);
    body.append_c (' ');
    body.append_ಅಂಕೆ (hb_color_get_green (foreground) / 255.f, 4);
    body.append_c (' ');
    body.append_ಅಂಕೆ (hb_color_get_blue (foreground) / 255.f, 4);
    body.append_str (" rg\n");
    body.append_len (path.arrayZ, path.length);
    body.append_str ("f\n");
  }

  ಶೂನ್ಯ flush_path_svg ()
  {
    body.append_str ("<path d=\"");
    body.append_len (path.arrayZ, path.length);
    body.append_str ("\" fill=\"");
    body.append_svg_color (foreground, ಸತ್ಯ);
    body.append_str ("\"/>\n");
  }

  ಶೂನ್ಯ transform_xy (ಭಿನ್ನ x, ಭಿನ್ನ y, ಭಿನ್ನ *tx, ಭಿನ್ನ *ty)
  {
    hb_vector_transform_point (transform, x_scale_factor, y_scale_factor, x, y, tx, ty);
  }

  ಶೂನ್ಯ append_xy (ಭಿನ್ನ x, ಭಿನ್ನ y, ವರ್ಣ sep)
  {
    ಭಿನ್ನ tx, ty;
    transform_xy (x, y, &tx, &ty);
    path.append_ಅಂಕೆ (tx, path.precision);
    path.append_c (sep);
    path.append_ಅಂಕೆ (ty, path.precision);
  }

  ಶೂನ್ಯ append_xy_svg (ಭಿನ್ನ x, ಭಿನ್ನ y) { append_xy (x, y, ','); }
  ಶೂನ್ಯ append_xy_pdf (ಭಿನ್ನ x, ಭಿನ್ನ y) { append_xy (x, y, ' '); }
};

#endif /* HB_VECTOR_DRAW_HH */
