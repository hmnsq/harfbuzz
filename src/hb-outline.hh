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

#অন্তর্ভাব "hb.hh"

#অন্তর্ভাব "hb-draw.hh"


কাঠামো hb_outline_point_t
{
  গণনা শ্রেণী type_t
  {
    MOVE_TO,
    সারি_TO,
    QUADRATIC_TO,
    CUBIC_TO,
  };

  hb_outline_point_t (দশমিক x, দশমিক y, type_t type) :
    x (x), y (y), type (type) {}

  দশমিক x, y;
  type_t type;
};

কাঠামো hb_outline_vector_t
{
  দশমিক normalize_len ()
  {
    দশমিক len = hypotf (x, y);
    যদি (len)
    {
      x /= len;
      y /= len;
    }
    ফেরত len;
  }

  দশমিক x, y;
};

কাঠামো hb_outline_t
{
  শূন্য reset () { points.clear (); contours.clear (); }

  HB_INTERNAL শূন্য replay (hb_draw_funcs_t *pen, শূন্য *pen_তথ্য) ধ্রুবক;
  HB_INTERNAL দশমিক control_area () ধ্রুবক;
  HB_INTERNAL শূন্য translate (দশমিক dx, দশমিক dy);
  HB_INTERNAL শূন্য slant (দশমিক slant_xy);
  HB_INTERNAL শূন্য embolden (দশমিক x_strength, দশমিক y_strength,
			     দশমিক x_shift, দশমিক y_shift);

  hb_vector_t<hb_outline_point_t> points;
  hb_vector_t<unsigned> contours;
  int64_t *budget_remaining = nullptr;
};

HB_INTERNAL hb_draw_funcs_t *
hb_outline_recording_pen_get_funcs ();


#endif /* HB_OUTLINE_HH */
