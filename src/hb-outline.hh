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

#اگر_غیر_تعریف_شدہ HB_OUTLINE_HH
#تعریف_کرو HB_OUTLINE_HH

#شامل "hb.hh"

#شامل "hb-draw.hh"


ساخت hb_outline_نقطہ_t
{
  شمار جماعت type_t
  {
    MOVE_مقصد,
    سطر_مقصد_2,
    QUADRATIC_مقصد,
    CUBIC_مقصد,
  };

  hb_outline_نقطہ_t (اعشاری x, اعشاری y, type_t type) :
    x (x), y (y), type (type) {}

  اعشاری x, y;
  type_t type;
};

ساخت hb_outline_سمتیہ_t
{
  اعشاری normalize_len ()
  {
    اعشاری len = hypotf (x, y);
    اگر (len)
    {
      x /= len;
      y /= len;
    }
    واپس len;
  }

  اعشاری x, y;
};

ساخت hb_outline_t
{
  خالی reset () { points.clear (); contours.clear (); }

  HB_اندرونی خالی replay (hb_draw_funcs_t *pen, خالی *pen_ڈیٹا) ثابت;
  HB_اندرونی اعشاری control_area () ثابت;
  HB_اندرونی خالی translate (اعشاری dx, اعشاری dy);
  HB_اندرونی خالی slant (اعشاری slant_xy);
  HB_اندرونی خالی embolden (اعشاری x_strength, اعشاری y_strength,
			     اعشاری x_سرکاؤ, اعشاری y_سرکاؤ);

  hb_سمتیہ_t<hb_outline_نقطہ_t> points;
  hb_سمتیہ_t<غیرعلامتی> contours;
  int64_t *budget_بقایا = خالی_اشاریہ;
};

HB_اندرونی hb_draw_funcs_t *
hb_outline_recording_pen_get_funcs ();


#اگر_ختم /* HB_OUTLINE_HH */
