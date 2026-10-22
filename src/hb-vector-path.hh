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

#اگر_تعریف_نشده HB_وکتور_مسیر_HH
#تعریف_کن HB_وکتور_مسیر_HH

#ضمن "hb.hh"
#ضمن "hb-vector.hh"
#ضمن "hb-vector.h"
#ضمن "hb-draw.h"
#ضمن "hb-vector-buf.hh"

/* Lightweight path sink: serializes hb_draw_* calls into an
 * external char buffer as either SVG path-data or PDF path
 * operators, depending on format.  Used by the vector paint
 * backends to accumulate arbitrary clip paths (and by the
 * SVG backend to emit glyph outlines into defs). */
بنية hb_وکتور_مسیر_sink_t
{
  hb_وکتور_buf_t *مسیر;
  بیعلامت precision;
  عائم x_scale;
  عائم y_scale;
  int64_t *budget;

  منطقي begin_command ()
  {
    أرجع likely (!مسیر->درون_error () && !(budget && *budget < 0));
  }

  فراغ انتها_command ()
  {
    /* Serialized bytes are bounded by the output buffer's document-size
     * cap (in_error), not the outline work budget; stop outline traversal
     * once that cap or an allocation error trips. */
    إذا (!budget) أرجع;
    إذا (unlikely (مسیر->درون_error ()))
      *budget = 0;
  }
};

HB_داخلی hb_draw_funcs_t *
hb_وکتور_svg_مسیر_draw_funcs_get ();

HB_داخلی hb_draw_funcs_t *
hb_وکتور_pdf_مسیر_draw_funcs_get ();

HB_داخلی hb_draw_funcs_t *
hb_وکتور_مسیر_draw_funcs_get (hb_وکتور_format_t format);

#پایان_اگر /* HB_VECTOR_PATH_HH */
