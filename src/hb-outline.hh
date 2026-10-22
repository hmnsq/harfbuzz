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

#جے_بنا_تعریف HB_OUTLINE_HH
#تعریف_کرو HB_OUTLINE_HH

#ضمن "hb.hh"

#ضمن "hb-draw.hh"


بنية hb_outline_نقطہ_t
{
  تعداد صنف type_t
  {
    MOVE_منزل,
    سطر_منزل_2,
    QUADRATIC_منزل,
    CUBIC_منزل,
  };

  hb_outline_نقطہ_t (عائم x, عائم y, type_t type) :
    x (x), y (y), type (type) {}

  عائم x, y;
  type_t type;
};

بنية hb_outline_سمتیہ_t
{
  عائم normalize_len ()
  {
    عائم len = hypotf (x, y);
    إذا (len)
    {
      x /= len;
      y /= len;
    }
    أرجع len;
  }

  عائم x, y;
};

بنية hb_outline_t
{
  فراغ reset () { points.clear (); contours.clear (); }

  HB_اندرونی فراغ replay (hb_draw_funcs_t *pen, فراغ *pen_انکڑے) ثابت;
  HB_اندرونی عائم control_area () ثابت;
  HB_اندرونی فراغ translate (عائم dx, عائم dy);
  HB_اندرونی فراغ slant (عائم slant_xy);
  HB_اندرونی فراغ embolden (عائم x_strength, عائم y_strength,
			     عائم x_کھسکاؤ, عائم y_کھسکاؤ);

  hb_سمتیہ_t<hb_outline_نقطہ_t> points;
  hb_سمتیہ_t<بنانشانی> contours;
  int64_t *budget_بچیا = خالی_سوچک;
};

HB_اندرونی hb_draw_funcs_t *
hb_outline_recording_pen_get_funcs ();


#جے_انت /* HB_OUTLINE_HH */
