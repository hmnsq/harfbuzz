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

#அந்தர்பாவ "hb.hh"

#அந்தர்பாவ "hb-draw.hh"


கட்டமைப்பு hb_outline_point_t
{
  எண்ணிக்கை வகுப்பு type_t
  {
    MOVE_TO,
    வரி_TO,
    QUADRATIC_TO,
    CUBIC_TO,
  };

  hb_outline_point_t (தசம x, தசம y, type_t type) :
    x (x), y (y), type (type) {}

  தசம x, y;
  type_t type;
};

கட்டமைப்பு hb_outline_vector_t
{
  தசம normalize_len ()
  {
    தசம len = hypotf (x, y);
    எனில் (len)
    {
      x /= len;
      y /= len;
    }
    திரும்பு len;
  }

  தசம x, y;
};

கட்டமைப்பு hb_outline_t
{
  வெறுமை reset () { points.clear (); contours.clear (); }

  HB_INTERNAL வெறுமை replay (hb_draw_funcs_t *pen, வெறுமை *pen_data) மாறிலி;
  HB_INTERNAL தசம control_area () மாறிலி;
  HB_INTERNAL வெறுமை translate (தசம dx, தசம dy);
  HB_INTERNAL வெறுமை slant (தசம slant_xy);
  HB_INTERNAL வெறுமை embolden (தசம x_strength, தசம y_strength,
			     தசம x_shift, தசம y_shift);

  hb_vector_t<hb_outline_point_t> points;
  hb_vector_t<unsigned> contours;
  int64_t *budget_remaining = nullptr;
};

HB_INTERNAL hb_draw_funcs_t *
hb_outline_recording_pen_get_funcs ();


#endif /* HB_OUTLINE_HH */
