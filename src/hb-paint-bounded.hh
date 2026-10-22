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

#ifndef HB_PAINT_BOUNDED_HH
#define HB_PAINT_BOUNDED_HH

#અન્તર્ભાવ "hb.hh"
#અન્તર્ભાવ "hb-paint.h"

#અન્તર્ભાવ "hb-geometry.hh"


પ્રકારવ્યાખ્યા સંરચના  hb_paint_bounded_context_t hb_paint_bounded_context_t;

સંરચના hb_paint_bounded_context_t
{
  શૂન્ય clear ()
  {
    clips = 0;
    bounded = સાચું;
    groups.clear ();
  }

  hb_paint_bounded_context_t ()
  {
    clear ();
  }

  તાર્કિક is_bounded ()
  {
    પાછા bounded;
  }

  શૂન્ય ધકેલો_clip ()
  {
    clips++;
  }

  શૂન્ય કાઢો_clip ()
  {
    જો (clips == 0) પાછા;
    clips--;
  }

  શૂન્ય ધકેલો_group ()
  {
    groups.push (bounded);
    bounded = સાચું;
  }

  શૂન્ય કાઢો_group (hb_paint_composite_mode_t mode)
  {
    અચલ તાર્કિક src_bounded = bounded;
    bounded = groups.કાઢો ();
    તાર્કિક &backdrop_bounded = bounded;

    // https://learn.microsoft.com/en-us/typography/opentype/spec/colr#format-32-paintcomposite
    પસંદગી ((પૂર્ણાંક) mode)
    {
      સ્થિતિ HB_PAINT_COMPOSITE_MODE_CLEAR:
	backdrop_bounded = સાચું;
	તોડો;
      સ્થિતિ HB_PAINT_COMPOSITE_MODE_SRC:
      સ્થિતિ HB_PAINT_COMPOSITE_MODE_SRC_OUT:
	backdrop_bounded = src_bounded;
	તોડો;
      સ્થિતિ HB_PAINT_COMPOSITE_MODE_DEST:
      સ્થિતિ HB_PAINT_COMPOSITE_MODE_DEST_OUT:
	તોડો;
      સ્થિતિ HB_PAINT_COMPOSITE_MODE_SRC_IN:
      સ્થિતિ HB_PAINT_COMPOSITE_MODE_DEST_IN:
	backdrop_bounded = backdrop_bounded && src_bounded;
	તોડો;
      મૂળભૂત:
	backdrop_bounded = backdrop_bounded || src_bounded;
	તોડો;
     }
  }

  શૂન્ય paint ()
  {
    જો (!clips)
      bounded = ખોટું;
  }

  સુરક્ષિત:
  તાર્કિક bounded; // true if current drawing bounded
  unsigned clips; // number of active clips
  hb_vector_t<તાર્કિક> groups; // true if group bounded
};

HB_INTERNAL hb_paint_funcs_t *
hb_paint_bounded_get_funcs ();


#endif /* HB_PAINT_BOUNDED_HH */
