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

#अगर_अघोषित HB_VECTOR_DRAW_HH
#घोषणा HB_VECTOR_DRAW_HH

#समावेश "hb.hh"

#समावेश "hb-vector.h"
#समावेश "hb-geometry.hh"
#समावेश "hb-machinery.hh"
#समावेश "hb-vector-buf.hh"
#समावेश "hb-vector-internal.hh"

काष्ठा hb_vector_draw_t
{
  hb_object_header_t header;

  hb_vector_format_t format = HB_VECTOR_FORMAT_INVALID;
  hb_transform_t<> transform = {1, 0, 0, 1, 0, 0};
  भग्न x_scale_factor = 1.f;
  भग्न y_scale_factor = 1.f;
  hb_vector_extents_t extents = {0, 0, 0, 0};
  bool has_extents = false;
  hb_color_t foreground = HB_COLOR (0, 0, 0, 255);
  hb_color_t background = HB_COLOR (0, 0, 0, 0);

  hb_vector_buf_t defs;
  hb_vector_buf_t body;
  hb_vector_buf_t path;
  hb_vector_buf_t pdf_extgstate_dict;
  अचिन्हित pdf_extgstate_गिनती = 0;

  /* Cumulative work budget for the current draw session; reset by
   * hb_vector_draw_clear().  Outline traversal and complete path commands
   * charge the same live counter. */
  int64_t budget = HB_BUDGET_DEFAULT;
  int64_t budget_remaining = HB_BUDGET_GLYPH;

  व्योम recharge_budget ()
  {
    budget_remaining = budget == HB_BUDGET_DEFAULT ?
		       HB_BUDGET_GLYPH : budget;
  }

  bool begin_path_command ()
  {
    वापस likely (budget_remaining >= 0 && !path.in_error ());
  }

  व्योम end_path_command ()
  {
    /* The serialized bytes are bounded by the output buffer's document-size
     * cap (in_error), not the outline work budget; stop outline traversal
     * once that cap or an allocation error trips. */
    अगर (unlikely (path.in_error ()))
      budget_remaining = 0;
  }

  व्योम set_precision (अचिन्हित p)
  {
    p = hb_min (p, 12u);
    defs.precision = p;
    body.precision = p;
    path.precision = p;
  }

  अचिन्हित get_precision () स्थिर { वापस path.precision; }
  hb_blob_t *recycled_blob = nullptr;

  व्योम new_path ()
  {
    flush_path ();
  }

  व्योम flush_path ()
  {
    अगर (!path.लंबाई) वापस;
    चयन (format)
    {
      हाल HB_VECTOR_FORMAT_PDF: flush_path_pdf (); अवरोध;
      हाल HB_VECTOR_FORMAT_SVG: flush_path_svg (); अवरोध;
      हाल HB_VECTOR_FORMAT_INVALID: शेष: अवरोध;
    }
    path.shrink (0);
  }

  व्योम flush_path_pdf ()
  {
    अचिन्हित a = hb_color_get_alpha (foreground);
    अगर (a < 255)
    {
      अचिन्हित gs_idx = pdf_extgstate_गिनती++;
      body.append_str ("/GS");
      body.append_unsigned (gs_idx);
      body.append_str (" gs\n");

      pdf_extgstate_dict.append_str ("/GS");
      pdf_extgstate_dict.append_unsigned (gs_idx);
      pdf_extgstate_dict.append_str (" << /Type /ExtGState /ca ");
      pdf_extgstate_dict.append_अंक (a / 255.f, 4);
      pdf_extgstate_dict.append_str (" >> ");
    }
    body.append_अंक (hb_color_get_red (foreground) / 255.f, 4);
    body.append_c (' ');
    body.append_अंक (hb_color_get_green (foreground) / 255.f, 4);
    body.append_c (' ');
    body.append_अंक (hb_color_get_blue (foreground) / 255.f, 4);
    body.append_str (" rg\n");
    body.append_len (path.arrayZ, path.लंबाई);
    body.append_str ("f\n");
  }

  व्योम flush_path_svg ()
  {
    body.append_str ("<path d=\"");
    body.append_len (path.arrayZ, path.लंबाई);
    body.append_str ("\" fill=\"");
    body.append_svg_color (foreground, true);
    body.append_str ("\"/>\n");
  }

  व्योम transform_xy (भग्न x, भग्न y, भग्न *tx, भग्न *ty)
  {
    hb_vector_transform_point (transform, x_scale_factor, y_scale_factor, x, y, tx, ty);
  }

  व्योम append_xy (भग्न x, भग्न y, अक्षर sep)
  {
    भग्न tx, ty;
    transform_xy (x, y, &tx, &ty);
    path.append_अंक (tx, path.precision);
    path.append_c (sep);
    path.append_अंक (ty, path.precision);
  }

  व्योम append_xy_svg (भग्न x, भग्न y) { append_xy (x, y, ','); }
  व्योम append_xy_pdf (भग्न x, भग्न y) { append_xy (x, y, ' '); }
};

#पूर्ण_अगर /* HB_VECTOR_DRAW_HH */
