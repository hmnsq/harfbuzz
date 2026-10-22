/*
 * Copyright © 2022 Behdad Esfahbod
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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_PAINT_BOUNDED_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_PAINT_BOUNDED_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-paint.h"

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-geometry.hh"


ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ  hb_paint_bounded_context_t hb_paint_bounded_context_t;

ਬਣਤਰ hb_paint_bounded_context_t
{
  ਖਾਲੀ clear ()
  {
    clips = 0;
    bounded = ਸੱਚ;
    groups.clear ();
  }

  hb_paint_bounded_context_t ()
  {
    clear ();
  }

  ਤਾਰਕਿਕ is_bounded ()
  {
    ਵਾਪਸ bounded;
  }

  ਖਾਲੀ ਧੱਕੋ_clip ()
  {
    clips++;
  }

  ਖਾਲੀ ਕੱਢੋ_clip ()
  {
    ਜੇ (clips == 0) ਵਾਪਸ;
    clips--;
  }

  ਖਾਲੀ ਧੱਕੋ_ਸਮੂਹ ()
  {
    groups.push (bounded);
    bounded = ਸੱਚ;
  }

  ਖਾਲੀ ਕੱਢੋ_ਸਮੂਹ (hb_paint_ਸੰਯੁਕਤ_ਬਹੁਲਕ_t ਬਹੁਲਕ)
  {
    ਅਚਲ ਤਾਰਕਿਕ src_bounded = bounded;
    bounded = groups.ਕੱਢੋ ();
    ਤਾਰਕਿਕ &backdrop_bounded = bounded;

    // https://learn.microsoft.com/en-us/typography/opentype/spec/colr#format-32-paintcomposite
    ਚੋਣ ((ਪੂਰਨ) ਬਹੁਲਕ)
    {
      ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_CLEAR:
	backdrop_bounded = ਸੱਚ;
	ਰੋਕੋ;
      ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_SRC:
      ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_SRC_ਬਾਹਰਲਾ:
	backdrop_bounded = src_bounded;
	ਰੋਕੋ;
      ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_DEST:
      ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_DEST_ਬਾਹਰਲਾ:
	ਰੋਕੋ;
      ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_SRC_ਵਿੱਚ:
      ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_DEST_ਵਿੱਚ:
	backdrop_bounded = backdrop_bounded && src_bounded;
	ਰੋਕੋ;
      ਮੂਲ:
	backdrop_bounded = backdrop_bounded || src_bounded;
	ਰੋਕੋ;
     }
  }

  ਖਾਲੀ paint ()
  {
    ਜੇ (!clips)
      bounded = ਝੂਠ;
  }

  ਸੁਰੱਖਿਅਤ:
  ਤਾਰਕਿਕ bounded; // true if current drawing bounded
  ਬਿਨਾਨਿਸ਼ਾਨੀ clips; // number of active clips
  hb_ਸਦਿਸ਼_t<ਤਾਰਕਿਕ> groups; // true if group bounded
};

HB_ਅੰਦਰੂਨੀ hb_paint_funcs_t *
hb_paint_bounded_get_funcs ();


#ਜੇ_ਅੰਤ /* HB_PAINT_BOUNDED_HH */
