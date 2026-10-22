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

#अन्तर्भाव "hb.hh"

#अन्तर्भाव "hb-draw.hh"


संरचना hb_outline_point_t
{
  गणना वर्ग type_t
  {
    MOVE_TO,
    पंक्ति_TO,
    QUADRATIC_TO,
    CUBIC_TO,
  };

  hb_outline_point_t (भिन्न x, भिन्न y, type_t type) :
    x (x), y (y), type (type) {}

  भिन्न x, y;
  type_t type;
};

संरचना hb_outline_vector_t
{
  भिन्न normalize_len ()
  {
    भिन्न len = hypotf (x, y);
    यदि (len)
    {
      x /= len;
      y /= len;
    }
    प्रत्यागम len;
  }

  भिन्न x, y;
};

संरचना hb_outline_t
{
  शून्य reset () { points.clear (); contours.clear (); }

  HB_INTERNAL शून्य replay (hb_draw_funcs_t *pen, शून्य *pen_data) ध्रुव;
  HB_INTERNAL भिन्न control_area () ध्रुव;
  HB_INTERNAL शून्य translate (भिन्न dx, भिन्न dy);
  HB_INTERNAL शून्य slant (भिन्न slant_xy);
  HB_INTERNAL शून्य embolden (भिन्न x_strength, भिन्न y_strength,
			     भिन्न x_shift, भिन्न y_shift);

  hb_vector_t<hb_outline_point_t> points;
  hb_vector_t<अचिन्हित> contours;
  int64_t *budget_remaining = nullptr;
};

HB_INTERNAL hb_draw_funcs_t *
hb_outline_recording_pen_get_funcs ();


#endif /* HB_OUTLINE_HH */
