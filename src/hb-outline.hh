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

#اگر_تعریف_نشده HB_OUTLINE_HH
#تعریف_کن HB_OUTLINE_HH

#ضمن "hb.hh"

#ضمن "hb-draw.hh"


بنية hb_outline_نقطه_t
{
  تعداد صنف type_t
  {
    MOVE_مقصد,
    سطر_مقصد_2,
    QUADRATIC_مقصد,
    CUBIC_مقصد,
  };

  hb_outline_نقطه_t (عائم x, عائم y, type_t type) :
    x (x), y (y), type (type) {}

  عائم x, y;
  type_t type;
};

بنية hb_outline_وکتور_t
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

  HB_داخلی فراغ replay (hb_draw_funcs_t *pen, فراغ *pen_داده) ثابت;
  HB_داخلی عائم control_area () ثابت;
  HB_داخلی فراغ translate (عائم dx, عائم dy);
  HB_داخلی فراغ slant (عائم slant_xy);
  HB_داخلی فراغ embolden (عائم x_strength, عائم y_strength,
			     عائم x_لغزش, عائم y_لغزش);

  hb_وکتور_t<hb_outline_نقطه_t> points;
  hb_وکتور_t<بیعلامت> contours;
  int64_t *budget_مانده = اشاره_تهی;
};

HB_داخلی hb_draw_funcs_t *
hb_outline_recording_pen_get_funcs ();


#پایان_اگر /* HB_OUTLINE_HH */
