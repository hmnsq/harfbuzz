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

#جيڪڏهن_وصف_نه_ٿيل HB_OUTLINE_HH
#وصف_ڪريو HB_OUTLINE_HH

#ضمن "hb.hh"

#ضمن "hb-draw.hh"


بنية hb_outline_نقطو_t
{
  تعداد صنف type_t
  {
    MOVE_منزل,
    سٽ_منزل_2,
    QUADRATIC_منزل,
    CUBIC_منزل,
  };

  hb_outline_نقطو_t (عائم x, عائم y, type_t type) :
    x (x), y (y), type (type) {}

  عائم x, y;
  type_t type;
};

بنية hb_outline_ويڪٽر_t
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

  HB_اندروني فراغ replay (hb_draw_funcs_t *pen, فراغ *pen_معلومات) ثابت;
  HB_اندروني عائم control_area () ثابت;
  HB_اندروني فراغ translate (عائم dx, عائم dy);
  HB_اندروني فراغ slant (عائم slant_xy);
  HB_اندروني فراغ embolden (عائم x_strength, عائم y_strength,
			     عائم x_سرڪايو, عائم y_سرڪايو);

  hb_ويڪٽر_t<hb_outline_نقطو_t> points;
  hb_ويڪٽر_t<بنانشان> contours;
  int64_t *budget_بچيل = خالي_اشارو;
};

HB_اندروني hb_draw_funcs_t *
hb_outline_recording_pen_get_funcs ();


#جيڪڏهن_پورو /* HB_OUTLINE_HH */
