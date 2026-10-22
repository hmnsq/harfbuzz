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

#ifndef HB_OUTLINE_HH
#define HB_OUTLINE_HH

#અન્તર્ભાવ "hb.hh"

#અન્તર્ભાવ "hb-draw.hh"


સંરચના hb_outline_point_t
{
  ગણના વર્ગ type_t
  {
    MOVE_TO,
    લીટી_TO,
    QUADRATIC_TO,
    CUBIC_TO,
  };

  hb_outline_point_t (દશાંશ x, દશાંશ y, type_t type) :
    x (x), y (y), type (type) {}

  દશાંશ x, y;
  type_t type;
};

સંરચના hb_outline_vector_t
{
  દશાંશ normalize_len ()
  {
    દશાંશ len = hypotf (x, y);
    જો (len)
    {
      x /= len;
      y /= len;
    }
    પાછા len;
  }

  દશાંશ x, y;
};

સંરચના hb_outline_t
{
  શૂન્ય reset () { points.clear (); contours.clear (); }

  HB_INTERNAL શૂન્ય replay (hb_draw_funcs_t *pen, શૂન્ય *pen_data) અચલ;
  HB_INTERNAL દશાંશ control_area () અચલ;
  HB_INTERNAL શૂન્ય translate (દશાંશ dx, દશાંશ dy);
  HB_INTERNAL શૂન્ય slant (દશાંશ slant_xy);
  HB_INTERNAL શૂન્ય embolden (દશાંશ x_strength, દશાંશ y_strength,
			     દશાંશ x_shift, દશાંશ y_shift);

  hb_vector_t<hb_outline_point_t> points;
  hb_vector_t<unsigned> contours;
  int64_t *budget_remaining = nullptr;
};

HB_INTERNAL hb_draw_funcs_t *
hb_outline_recording_pen_get_funcs ();


#endif /* HB_OUTLINE_HH */
