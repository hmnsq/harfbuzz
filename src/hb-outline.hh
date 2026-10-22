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

#अगर_अघोषित HB_OUTLINE_HH
#घोषणा HB_OUTLINE_HH

#समावेश "hb.hh"

#समावेश "hb-draw.hh"


काष्ठा hb_outline_point_t
{
  क्रमागत श्रेणी type_t
  {
    MOVE_TO,
    पंक्ति_TO,
    QUADRATIC_TO,
    CUBIC_TO,
  };

  hb_outline_point_t (भग्न x, भग्न y, type_t type) :
    x (x), y (y), type (type) {}

  भग्न x, y;
  type_t type;
};

काष्ठा hb_outline_vector_t
{
  भग्न normalize_len ()
  {
    भग्न len = hypotf (x, y);
    अगर (len)
    {
      x /= len;
      y /= len;
    }
    वापस len;
  }

  भग्न x, y;
};

काष्ठा hb_outline_t
{
  व्योम reset () { points.सफा (); contours.सफा (); }

  HB_INTERNAL व्योम replay (hb_draw_funcs_t *pen, व्योम *pen_तथ्य) स्थिर;
  HB_INTERNAL भग्न control_area () स्थिर;
  HB_INTERNAL व्योम translate (भग्न dx, भग्न dy);
  HB_INTERNAL व्योम slant (भग्न slant_xy);
  HB_INTERNAL व्योम embolden (भग्न x_strength, भग्न y_strength,
			     भग्न x_shift, भग्न y_shift);

  hb_vector_t<hb_outline_point_t> points;
  hb_vector_t<अचिन्हित> contours;
  int64_t *budget_remaining = nullptr;
};

HB_INTERNAL hb_draw_funcs_t *
hb_outline_recording_pen_get_funcs ();


#पूर्ण_अगर /* HB_OUTLINE_HH */
