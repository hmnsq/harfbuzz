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

#अन्तर्भाव "hb.hh"
#अन्तर्भाव "hb-paint.h"

#अन्तर्भाव "hb-geometry.hh"


प्रकारनिर्देश संरचना  hb_paint_bounded_context_t hb_paint_bounded_context_t;

संरचना hb_paint_bounded_context_t
{
  शून्य clear ()
  {
    clips = 0;
    bounded = सच्च;
    groups.clear ();
  }

  hb_paint_bounded_context_t ()
  {
    clear ();
  }

  तार्किक is_bounded ()
  {
    पटिनिवत्त bounded;
  }

  शून्य प्रक्षिप_clip ()
  {
    clips++;
  }

  शून्य निष्कासय_clip ()
  {
    सचे (clips == 0) पटिनिवत्त;
    clips--;
  }

  शून्य प्रक्षिप_group ()
  {
    groups.push (bounded);
    bounded = सच्च;
  }

  शून्य निष्कासय_group (hb_paint_composite_mode_t mode)
  {
    ध्रुव तार्किक src_bounded = bounded;
    bounded = groups.निष्कासय ();
    तार्किक &backdrop_bounded = bounded;

    // https://learn.microsoft.com/en-us/typography/opentype/spec/colr#format-32-paintcomposite
    विकल्प ((पूर्णाङ्क) mode)
    {
      पक्ष HB_PAINT_COMPOSITE_MODE_CLEAR:
	backdrop_bounded = सच्च;
	भङ्ग;
      पक्ष HB_PAINT_COMPOSITE_MODE_SRC:
      पक्ष HB_PAINT_COMPOSITE_MODE_SRC_OUT:
	backdrop_bounded = src_bounded;
	भङ्ग;
      पक्ष HB_PAINT_COMPOSITE_MODE_DEST:
      पक्ष HB_PAINT_COMPOSITE_MODE_DEST_OUT:
	भङ्ग;
      पक्ष HB_PAINT_COMPOSITE_MODE_SRC_IN:
      पक्ष HB_PAINT_COMPOSITE_MODE_DEST_IN:
	backdrop_bounded = backdrop_bounded && src_bounded;
	भङ्ग;
      स्वभाव:
	backdrop_bounded = backdrop_bounded || src_bounded;
	भङ्ग;
     }
  }

  शून्य paint ()
  {
    सचे (!clips)
      bounded = मिच्छा;
  }

  रक्षित:
  तार्किक bounded; // true if current drawing bounded
  unsigned clips; // number of active clips
  hb_vector_t<तार्किक> groups; // true if group bounded
};

HB_INTERNAL hb_paint_funcs_t *
hb_paint_bounded_get_funcs ();


#endif /* HB_PAINT_BOUNDED_HH */
