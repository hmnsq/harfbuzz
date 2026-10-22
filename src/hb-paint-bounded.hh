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

#ಅನ್ತರ್ಭಾವ "hb.hh"
#ಅನ್ತರ್ಭಾವ "hb-paint.h"

#ಅನ್ತರ್ಭಾವ "hb-geometry.hh"


ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ  hb_paint_bounded_context_t hb_paint_bounded_context_t;

ಸಂರಚನಾ hb_paint_bounded_context_t
{
  ಶೂನ್ಯ clear ()
  {
    clips = 0;
    bounded = ಸತ್ಯ;
    groups.clear ();
  }

  hb_paint_bounded_context_t ()
  {
    clear ();
  }

  ತಾರ್ಕಿಕ is_bounded ()
  {
    ಪ್ರತ್ಯಾಗಮ bounded;
  }

  ಶೂನ್ಯ ತಳ್ಳು_clip ()
  {
    clips++;
  }

  ಶೂನ್ಯ ತೆಗೆ_clip ()
  {
    ಯದಿ (clips == 0) ಪ್ರತ್ಯಾಗಮ;
    clips--;
  }

  ಶೂನ್ಯ ತಳ್ಳು_group ()
  {
    groups.push (bounded);
    bounded = ಸತ್ಯ;
  }

  ಶೂನ್ಯ ತೆಗೆ_group (hb_paint_composite_mode_t mode)
  {
    ಧ್ರುವ ತಾರ್ಕಿಕ src_bounded = bounded;
    bounded = groups.ತೆಗೆ ();
    ತಾರ್ಕಿಕ &backdrop_bounded = bounded;

    // https://learn.microsoft.com/en-us/typography/opentype/spec/colr#format-32-paintcomposite
    ವಿಕಲ್ಪ ((ಪೂರ್ಣಾಙ್ಕ) mode)
    {
      ಪಕ್ಷ HB_PAINT_COMPOSITE_MODE_CLEAR:
	backdrop_bounded = ಸತ್ಯ;
	ಭಙ್ಗ;
      ಪಕ್ಷ HB_PAINT_COMPOSITE_MODE_SRC:
      ಪಕ್ಷ HB_PAINT_COMPOSITE_MODE_SRC_OUT:
	backdrop_bounded = src_bounded;
	ಭಙ್ಗ;
      ಪಕ್ಷ HB_PAINT_COMPOSITE_MODE_DEST:
      ಪಕ್ಷ HB_PAINT_COMPOSITE_MODE_DEST_OUT:
	ಭಙ್ಗ;
      ಪಕ್ಷ HB_PAINT_COMPOSITE_MODE_SRC_IN:
      ಪಕ್ಷ HB_PAINT_COMPOSITE_MODE_DEST_IN:
	backdrop_bounded = backdrop_bounded && src_bounded;
	ಭಙ್ಗ;
      ಸ್ವಭಾವ:
	backdrop_bounded = backdrop_bounded || src_bounded;
	ಭಙ್ಗ;
     }
  }

  ಶೂನ್ಯ paint ()
  {
    ಯದಿ (!clips)
      bounded = ಅಸತ್ಯ;
  }

  ರಕ್ಷಿತ:
  ತಾರ್ಕಿಕ bounded; // true if current drawing bounded
  unsigned clips; // number of active clips
  hb_vector_t<ತಾರ್ಕಿಕ> groups; // true if group bounded
};

HB_INTERNAL hb_paint_funcs_t *
hb_paint_bounded_get_funcs ();


#endif /* HB_PAINT_BOUNDED_HH */
