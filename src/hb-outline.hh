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

#ಅನ್ತರ್ಭಾವ "hb.hh"

#ಅನ್ತರ್ಭಾವ "hb-draw.hh"


ಸಂರಚನಾ hb_outline_point_t
{
  ಗಣನಾ ವರ್ಗ type_t
  {
    MOVE_TO,
    ಸಾಲು_TO,
    QUADRATIC_TO,
    CUBIC_TO,
  };

  hb_outline_point_t (ಭಿನ್ನ x, ಭಿನ್ನ y, type_t type) :
    x (x), y (y), type (type) {}

  ಭಿನ್ನ x, y;
  type_t type;
};

ಸಂರಚನಾ hb_outline_vector_t
{
  ಭಿನ್ನ normalize_len ()
  {
    ಭಿನ್ನ len = hypotf (x, y);
    ಯದಿ (len)
    {
      x /= len;
      y /= len;
    }
    ಪ್ರತ್ಯಾಗಮ len;
  }

  ಭಿನ್ನ x, y;
};

ಸಂರಚನಾ hb_outline_t
{
  ಶೂನ್ಯ reset () { points.clear (); contours.clear (); }

  HB_INTERNAL ಶೂನ್ಯ replay (hb_draw_funcs_t *pen, ಶೂನ್ಯ *pen_data) ಧ್ರುವ;
  HB_INTERNAL ಭಿನ್ನ control_area () ಧ್ರುವ;
  HB_INTERNAL ಶೂನ್ಯ translate (ಭಿನ್ನ dx, ಭಿನ್ನ dy);
  HB_INTERNAL ಶೂನ್ಯ slant (ಭಿನ್ನ slant_xy);
  HB_INTERNAL ಶೂನ್ಯ embolden (ಭಿನ್ನ x_strength, ಭಿನ್ನ y_strength,
			     ಭಿನ್ನ x_shift, ಭಿನ್ನ y_shift);

  hb_vector_t<hb_outline_point_t> points;
  hb_vector_t<unsigned> contours;
  int64_t *budget_remaining = nullptr;
};

HB_INTERNAL hb_draw_funcs_t *
hb_outline_recording_pen_get_funcs ();


#endif /* HB_OUTLINE_HH */
