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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_ਸਦਿਸ਼_ਰਸਤਾ_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_ਸਦਿਸ਼_ਰਸਤਾ_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-vector.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-vector.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-draw.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-vector-buf.hh"

/* Lightweight path sink: serializes hb_draw_* calls into an
 * external char buffer as either SVG path-data or PDF path
 * operators, depending on format.  Used by the vector paint
 * backends to accumulate arbitrary clip paths (and by the
 * SVG backend to emit glyph outlines into defs). */
ਬਣਤਰ hb_ਸਦਿਸ਼_ਰਸਤਾ_sink_t
{
  hb_ਸਦਿਸ਼_buf_t *ਰਸਤਾ;
  ਬਿਨਾਨਿਸ਼ਾਨੀ precision;
  ਦਸ਼ਮਲਵ x_scale;
  ਦਸ਼ਮਲਵ y_scale;
  int64_t *budget;

  ਤਾਰਕਿਕ begin_command ()
  {
    ਵਾਪਸ likely (!ਰਸਤਾ->ਵਿੱਚ_error () && !(budget && *budget < 0));
  }

  ਖਾਲੀ ਸਿਰਾ_command ()
  {
    /* Serialized bytes are bounded by the output buffer's document-size
     * cap (in_error), not the outline work budget; stop outline traversal
     * once that cap or an allocation error trips. */
    ਜੇ (!budget) ਵਾਪਸ;
    ਜੇ (unlikely (ਰਸਤਾ->ਵਿੱਚ_error ()))
      *budget = 0;
  }
};

HB_ਅੰਦਰੂਨੀ hb_draw_funcs_t *
hb_ਸਦਿਸ਼_svg_ਰਸਤਾ_draw_funcs_get ();

HB_ਅੰਦਰੂਨੀ hb_draw_funcs_t *
hb_ਸਦਿਸ਼_pdf_ਰਸਤਾ_draw_funcs_get ();

HB_ਅੰਦਰੂਨੀ hb_draw_funcs_t *
hb_ਸਦਿਸ਼_ਰਸਤਾ_draw_funcs_get (hb_ਸਦਿਸ਼_format_t format);

#ਜੇ_ਅੰਤ /* HB_VECTOR_PATH_HH */
