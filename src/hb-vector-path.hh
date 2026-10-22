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

#నిర్వచితం_కాకపోతే HB_సదిశ_మార్గం_HH
#నిర్వచించు HB_సదిశ_మార్గం_HH

#అన్తర్భావ "hb.hh"
#అన్తర్భావ "hb-vector.hh"
#అన్తర్భావ "hb-vector.h"
#అన్తర్భావ "hb-draw.h"
#అన్తర్భావ "hb-vector-buf.hh"

/* Lightweight path sink: serializes hb_draw_* calls into an
 * external char buffer as either SVG path-data or PDF path
 * operators, depending on format.  Used by the vector paint
 * backends to accumulate arbitrary clip paths (and by the
 * SVG backend to emit glyph outlines into defs). */
నిర్మాణం hb_సదిశ_మార్గం_sink_t
{
  hb_సదిశ_buf_t *మార్గం;
  గుర్తులేని precision;
  దశాంశం x_scale;
  దశాంశం y_scale;
  int64_t *budget;

  తార్కిక begin_command ()
  {
    తిరిగి likely (!మార్గం->లోపల_error () && !(budget && *budget < 0));
  }

  శూన్య చివర_command ()
  {
    /* Serialized bytes are bounded by the output buffer's document-size
     * cap (in_error), not the outline work budget; stop outline traversal
     * once that cap or an allocation error trips. */
    ఒకవేళ (!budget) తిరిగి;
    ఒకవేళ (unlikely (మార్గం->లోపల_error ()))
      *budget = 0;
  }
};

HB_అంతర్గత hb_draw_funcs_t *
hb_సదిశ_svg_మార్గం_draw_funcs_get ();

HB_అంతర్గత hb_draw_funcs_t *
hb_సదిశ_pdf_మార్గం_draw_funcs_get ();

HB_అంతర్గత hb_draw_funcs_t *
hb_సదిశ_మార్గం_draw_funcs_get (hb_సదిశ_format_t format);

#అయితే_ముగింపు /* HB_VECTOR_PATH_HH */
