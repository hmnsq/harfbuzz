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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_OUTLINE_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OUTLINE_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-draw.hh"


ਬਣਤਰ hb_outline_ਬਿੰਦੂ_t
{
  ਗਿਣਤੀ ਵਰਗ type_t
  {
    MOVE_ਮੰਜ਼ਿਲ,
    ਸਤਰ_ਮੰਜ਼ਿਲ_2,
    QUADRATIC_ਮੰਜ਼ਿਲ,
    CUBIC_ਮੰਜ਼ਿਲ,
  };

  hb_outline_ਬਿੰਦੂ_t (ਦਸ਼ਮਲਵ x, ਦਸ਼ਮਲਵ y, type_t type) :
    x (x), y (y), type (type) {}

  ਦਸ਼ਮਲਵ x, y;
  type_t type;
};

ਬਣਤਰ hb_outline_ਸਦਿਸ਼_t
{
  ਦਸ਼ਮਲਵ normalize_len ()
  {
    ਦਸ਼ਮਲਵ len = hypotf (x, y);
    ਜੇ (len)
    {
      x /= len;
      y /= len;
    }
    ਵਾਪਸ len;
  }

  ਦਸ਼ਮਲਵ x, y;
};

ਬਣਤਰ hb_outline_t
{
  ਖਾਲੀ reset () { points.clear (); contours.clear (); }

  HB_ਅੰਦਰੂਨੀ ਖਾਲੀ replay (hb_draw_funcs_t *pen, ਖਾਲੀ *pen_ਅੰਕੜੇ) ਅਚਲ;
  HB_ਅੰਦਰੂਨੀ ਦਸ਼ਮਲਵ control_area () ਅਚਲ;
  HB_ਅੰਦਰੂਨੀ ਖਾਲੀ translate (ਦਸ਼ਮਲਵ dx, ਦਸ਼ਮਲਵ dy);
  HB_ਅੰਦਰੂਨੀ ਖਾਲੀ slant (ਦਸ਼ਮਲਵ slant_xy);
  HB_ਅੰਦਰੂਨੀ ਖਾਲੀ embolden (ਦਸ਼ਮਲਵ x_strength, ਦਸ਼ਮਲਵ y_strength,
			     ਦਸ਼ਮਲਵ x_ਖਿਸਕਾਓ, ਦਸ਼ਮਲਵ y_ਖਿਸਕਾਓ);

  hb_ਸਦਿਸ਼_t<hb_outline_ਬਿੰਦੂ_t> points;
  hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> contours;
  int64_t *budget_ਬਚਿਆ = ਖਾਲੀ_ਸੂਚਕ;
};

HB_ਅੰਦਰੂਨੀ hb_draw_funcs_t *
hb_outline_recording_pen_get_funcs ();


#ਜੇ_ਅੰਤ /* HB_OUTLINE_HH */
