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

#अगर_अघोषित HB_VECTOR_PATH_HH
#घोषणा HB_VECTOR_PATH_HH

#समावेश "hb.hh"
#समावेश "hb-vector.hh"
#समावेश "hb-vector.h"
#समावेश "hb-draw.h"
#समावेश "hb-vector-buf.hh"

/* Lightweight path sink: serializes hb_draw_* calls into an
 * external char buffer as either SVG path-data or PDF path
 * operators, depending on format.  Used by the vector paint
 * backends to accumulate arbitrary clip paths (and by the
 * SVG backend to emit glyph outlines into defs). */
काष्ठा hb_vector_path_sink_t
{
  hb_vector_buf_t *path;
  अचिन्हित precision;
  भग्न x_scale;
  भग्न y_scale;
  int64_t *budget;

  bool begin_command ()
  {
    वापस likely (!path->in_error () && !(budget && *budget < 0));
  }

  व्योम end_command ()
  {
    /* Serialized bytes are bounded by the output buffer's document-size
     * cap (in_error), not the outline work budget; stop outline traversal
     * once that cap or an allocation error trips. */
    अगर (!budget) वापस;
    अगर (unlikely (path->in_error ()))
      *budget = 0;
  }
};

HB_INTERNAL hb_draw_funcs_t *
hb_vector_svg_path_draw_funcs_get ();

HB_INTERNAL hb_draw_funcs_t *
hb_vector_pdf_path_draw_funcs_get ();

HB_INTERNAL hb_draw_funcs_t *
hb_vector_path_draw_funcs_get (hb_vector_format_t format);

#पूर्ण_अगर /* HB_VECTOR_PATH_HH */
