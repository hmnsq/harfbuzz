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

#شامل "hb.hh"

#شامل "hb-draw.hh"


ساختار hb_outline_point_t
{
  شمارش رده type_t
  {
    MOVE_TO,
    سطر_TO,
    QUADRATIC_TO,
    CUBIC_TO,
  };

  hb_outline_point_t (اعشاری x, اعشاری y, type_t type) :
    x (x), y (y), type (type) {}

  اعشاری x, y;
  type_t type;
};

ساختار hb_outline_vector_t
{
  اعشاری normalize_len ()
  {
    اعشاری len = hypotf (x, y);
    اگر (len)
    {
      x /= len;
      y /= len;
    }
    بازگشت len;
  }

  اعشاری x, y;
};

ساختار hb_outline_t
{
  تهی reset () { points.clear (); contours.clear (); }

  HB_INTERNAL تهی replay (hb_draw_funcs_t *pen, تهی *pen_data) ثابت;
  HB_INTERNAL اعشاری control_area () ثابت;
  HB_INTERNAL تهی translate (اعشاری dx, اعشاری dy);
  HB_INTERNAL تهی slant (اعشاری slant_xy);
  HB_INTERNAL تهی embolden (اعشاری x_strength, اعشاری y_strength,
			     اعشاری x_shift, اعشاری y_shift);

  hb_vector_t<hb_outline_point_t> points;
  hb_vector_t<unsigned> contours;
  int64_t *budget_remaining = nullptr;
};

HB_INTERNAL hb_draw_funcs_t *
hb_outline_recording_pen_get_funcs ();


#endif /* HB_OUTLINE_HH */
