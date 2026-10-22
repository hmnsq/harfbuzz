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

#अन्तर्भाव "hb.hh"

#अन्तर्भाव "hb-vector.h"
#अन्तर्भाव "hb-geometry.hh"
#अन्तर्भाव "hb-machinery.hh"
#अन्तर्भाव "hb-vector-buf.hh"
#अन्तर्भाव "hb-vector-internal.hh"

संरचना hb_vector_draw_t
{
  hb_object_header_t header;

  hb_vector_format_t format = HB_VECTOR_FORMAT_INVALID;
  hb_transform_t<> transform = {1, 0, 0, 1, 0, 0};
  भिन्न x_scale_factor = 1.f;
  भिन्न y_scale_factor = 1.f;
  hb_vector_extents_t extents = {0, 0, 0, 0};
  तार्किक has_extents = मिच्छा;
  hb_color_t foreground = HB_COLOR (0, 0, 0, 255);
  hb_color_t background = HB_COLOR (0, 0, 0, 0);

  hb_vector_buf_t defs;
  hb_vector_buf_t body;
  hb_vector_buf_t path;
  hb_vector_buf_t pdf_extgstate_dict;
  unsigned pdf_extgstate_गणनम् = 0;

  /* Cumulative work budget for the current draw session; reset by
   * hb_vector_draw_clear().  Outline traversal and complete path commands
   * charge the same live counter. */
  int64_t budget = HB_BUDGET_DEFAULT;
  int64_t budget_remaining = HB_BUDGET_GLYPH;

  शून्य recharge_budget ()
  {
    budget_remaining = budget == HB_BUDGET_DEFAULT ?
		       HB_BUDGET_GLYPH : budget;
  }

  तार्किक begin_path_command ()
  {
    पटिनिवत्त likely (budget_remaining >= 0 && !path.in_error ());
  }

  शून्य end_path_command ()
  {
    /* The serialized bytes are bounded by the output buffer's document-size
     * cap (in_error), not the outline work budget; stop outline traversal
     * once that cap or an allocation error trips. */
    सचे (unlikely (path.in_error ()))
      budget_remaining = 0;
  }

  शून्य set_precision (unsigned p)
  {
    p = hb_min (p, 12u);
    defs.precision = p;
    body.precision = p;
    path.precision = p;
  }

  unsigned get_precision () ध्रुव { पटिनिवत्त path.precision; }
  hb_blob_t *recycled_blob = nullptr;

  शून्य new_path ()
  {
    flush_path ();
  }

  शून्य flush_path ()
  {
    सचे (!path.length) पटिनिवत्त;
    विकल्प (format)
    {
      पक्ष HB_VECTOR_FORMAT_PDF: flush_path_pdf (); भङ्ग;
      पक्ष HB_VECTOR_FORMAT_SVG: flush_path_svg (); भङ्ग;
      पक्ष HB_VECTOR_FORMAT_INVALID: स्वभाव: भङ्ग;
    }
    path.shrink (0);
  }

  शून्य flush_path_pdf ()
  {
    unsigned a = hb_color_get_alpha (foreground);
    सचे (a < 255)
    {
      unsigned gs_idx = pdf_extgstate_गणनम्++;
      body.append_str ("/GS");
      body.append_unsigned (gs_idx);
      body.append_str (" gs\n");

      pdf_extgstate_dict.append_str ("/GS");
      pdf_extgstate_dict.append_unsigned (gs_idx);
      pdf_extgstate_dict.append_str (" << /Type /ExtGState /ca ");
      pdf_extgstate_dict.append_अङ्कः (a / 255.f, 4);
      pdf_extgstate_dict.append_str (" >> ");
    }
    body.append_अङ्कः (hb_color_get_red (foreground) / 255.f, 4);
    body.append_c (' ');
    body.append_अङ्कः (hb_color_get_green (foreground) / 255.f, 4);
    body.append_c (' ');
    body.append_अङ्कः (hb_color_get_blue (foreground) / 255.f, 4);
    body.append_str (" rg\n");
    body.append_len (path.arrayZ, path.length);
    body.append_str ("f\n");
  }

  शून्य flush_path_svg ()
  {
    body.append_str ("<path d=\"");
    body.append_len (path.arrayZ, path.length);
    body.append_str ("\" fill=\"");
    body.append_svg_color (foreground, सच्च);
    body.append_str ("\"/>\n");
  }

  शून्य transform_xy (भिन्न x, भिन्न y, भिन्न *tx, भिन्न *ty)
  {
    hb_vector_transform_point (transform, x_scale_factor, y_scale_factor, x, y, tx, ty);
  }

  शून्य append_xy (भिन्न x, भिन्न y, वर्ण sep)
  {
    भिन्न tx, ty;
    transform_xy (x, y, &tx, &ty);
    path.append_अङ्कः (tx, path.precision);
    path.append_c (sep);
    path.append_अङ्कः (ty, path.precision);
  }

  शून्य append_xy_svg (भिन्न x, भिन्न y) { append_xy (x, y, ','); }
  शून्य append_xy_pdf (भिन्न x, भिन्न y) { append_xy (x, y, ' '); }
};

#endif /* HB_VECTOR_DRAW_HH */
