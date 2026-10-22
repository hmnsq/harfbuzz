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

#ଅନ୍ତର୍ଭାଵ "hb.hh"

#ଅନ୍ତର୍ଭାଵ "hb-draw.hh"


ସଂରଚନା hb_outline_point_t
{
  ଗଣନା ଶ୍ରେଣୀ type_t
  {
    MOVE_TO,
    ଧାଡ଼ି_TO,
    QUADRATIC_TO,
    CUBIC_TO,
  };

  hb_outline_point_t (ଦଶମିକ x, ଦଶମିକ y, type_t type) :
    x (x), y (y), type (type) {}

  ଦଶମିକ x, y;
  type_t type;
};

ସଂରଚନା hb_outline_vector_t
{
  ଦଶମିକ normalize_len ()
  {
    ଦଶମିକ len = hypotf (x, y);
    ଯଦି (len)
    {
      x /= len;
      y /= len;
    }
    ଫେରନ୍ତୁ len;
  }

  ଦଶମିକ x, y;
};

ସଂରଚନା hb_outline_t
{
  ଶୂନ୍ୟ reset () { points.clear (); contours.clear (); }

  HB_INTERNAL ଶୂନ୍ୟ replay (hb_draw_funcs_t *pen, ଶୂନ୍ୟ *pen_data) ଅଚଳ;
  HB_INTERNAL ଦଶମିକ control_area () ଅଚଳ;
  HB_INTERNAL ଶୂନ୍ୟ translate (ଦଶମିକ dx, ଦଶମିକ dy);
  HB_INTERNAL ଶୂନ୍ୟ slant (ଦଶମିକ slant_xy);
  HB_INTERNAL ଶୂନ୍ୟ embolden (ଦଶମିକ x_strength, ଦଶମିକ y_strength,
			     ଦଶମିକ x_shift, ଦଶମିକ y_shift);

  hb_vector_t<hb_outline_point_t> points;
  hb_vector_t<unsigned> contours;
  int64_t *budget_remaining = nullptr;
};

HB_INTERNAL hb_draw_funcs_t *
hb_outline_recording_pen_get_funcs ();


#endif /* HB_OUTLINE_HH */
