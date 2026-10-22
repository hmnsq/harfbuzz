/*
 * Copyright © 2023  Behdad Esfahbod
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
 */

#నిర్వచితం_కాకపోతే HB_OUTLINE_HH
#నిర్వచించు HB_OUTLINE_HH

#అన్తర్భావ "hb.hh"

#అన్తర్భావ "hb-draw.hh"


నిర్మాణం hb_outline_బిందువు_t
{
  గణన తరగతి type_t
  {
    MOVE_గమ్యం,
    పంక్తి_గమ్యం_2,
    QUADRATIC_గమ్యం,
    CUBIC_గమ్యం,
  };

  hb_outline_బిందువు_t (దశాంశం x, దశాంశం y, type_t type) :
    x (x), y (y), type (type) {}

  దశాంశం x, y;
  type_t type;
};

నిర్మాణం hb_outline_సదిశ_t
{
  దశాంశం normalize_len ()
  {
    దశాంశం len = hypotf (x, y);
    ఒకవేళ (len)
    {
      x /= len;
      y /= len;
    }
    తిరిగి len;
  }

  దశాంశం x, y;
};

నిర్మాణం hb_outline_t
{
  శూన్య reset () { points.clear (); contours.clear (); }

  HB_అంతర్గత శూన్య replay (hb_draw_funcs_t *pen, శూన్య *pen_దత్తాంశం) స్థిరరాశి;
  HB_అంతర్గత దశాంశం control_area () స్థిరరాశి;
  HB_అంతర్గత శూన్య translate (దశాంశం dx, దశాంశం dy);
  HB_అంతర్గత శూన్య slant (దశాంశం slant_xy);
  HB_అంతర్గత శూన్య embolden (దశాంశం x_strength, దశాంశం y_strength,
			     దశాంశం x_జరుపు, దశాంశం y_జరుపు);

  hb_సదిశ_t<hb_outline_బిందువు_t> points;
  hb_సదిశ_t<గుర్తులేని> contours;
  int64_t *budget_మిగిలిన = శూన్యసూచిక;
};

HB_అంతర్గత hb_draw_funcs_t *
hb_outline_recording_pen_get_funcs ();


#అయితే_ముగింపు /* HB_OUTLINE_HH */
