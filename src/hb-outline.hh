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

#അന്തര്ഭാവ "hb.hh"

#അന്തര്ഭാവ "hb-draw.hh"


ഘടന hb_outline_point_t
{
  എണ്ണൽ വർഗ്ഗം type_t
  {
    MOVE_TO,
    വരി_TO,
    QUADRATIC_TO,
    CUBIC_TO,
  };

  hb_outline_point_t (ദശാംശം x, ദശാംശം y, type_t type) :
    x (x), y (y), type (type) {}

  ദശാംശം x, y;
  type_t type;
};

ഘടന hb_outline_vector_t
{
  ദശാംശം normalize_len ()
  {
    ദശാംശം len = hypotf (x, y);
    എങ്കിൽ (len)
    {
      x /= len;
      y /= len;
    }
    തിരികെ len;
  }

  ദശാംശം x, y;
};

ഘടന hb_outline_t
{
  ശൂന്യം reset () { points.clear (); contours.clear (); }

  HB_INTERNAL ശൂന്യം replay (hb_draw_funcs_t *pen, ശൂന്യം *pen_data) സ്ഥിരം;
  HB_INTERNAL ദശാംശം control_area () സ്ഥിരം;
  HB_INTERNAL ശൂന്യം translate (ദശാംശം dx, ദശാംശം dy);
  HB_INTERNAL ശൂന്യം slant (ദശാംശം slant_xy);
  HB_INTERNAL ശൂന്യം embolden (ദശാംശം x_strength, ദശാംശം y_strength,
			     ദശാംശം x_shift, ദശാംശം y_shift);

  hb_vector_t<hb_outline_point_t> points;
  hb_vector_t<unsigned> contours;
  int64_t *budget_remaining = nullptr;
};

HB_INTERNAL hb_draw_funcs_t *
hb_outline_recording_pen_get_funcs ();


#endif /* HB_OUTLINE_HH */
