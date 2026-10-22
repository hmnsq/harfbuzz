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


प्रकारव्याख्या संरचना  hb_paint_bounded_context_t hb_paint_bounded_context_t;

संरचना hb_paint_bounded_context_t
{
  शून्य clear ()
  {
    clips = 0;
    bounded = सत्य;
    groups.clear ();
  }

  hb_paint_bounded_context_t ()
  {
    clear ();
  }

  तार्किक is_bounded ()
  {
    परत bounded;
  }

  शून्य ढकला_clip ()
  {
    clips++;
  }

  शून्य काढा_clip ()
  {
    जर (clips == 0) परत;
    clips--;
  }

  शून्य ढकला_group ()
  {
    groups.push (bounded);
    bounded = सत्य;
  }

  शून्य काढा_group (hb_paint_composite_mode_t mode)
  {
    स्थिरांक तार्किक src_bounded = bounded;
    bounded = groups.काढा ();
    तार्किक &backdrop_bounded = bounded;

    // https://learn.microsoft.com/en-us/typography/opentype/spec/colr#format-32-paintcomposite
    निवड ((पूर्णांक) mode)
    {
      स्थिती HB_PAINT_COMPOSITE_MODE_CLEAR:
	backdrop_bounded = सत्य;
	थांबा;
      स्थिती HB_PAINT_COMPOSITE_MODE_SRC:
      स्थिती HB_PAINT_COMPOSITE_MODE_SRC_OUT:
	backdrop_bounded = src_bounded;
	थांबा;
      स्थिती HB_PAINT_COMPOSITE_MODE_DEST:
      स्थिती HB_PAINT_COMPOSITE_MODE_DEST_OUT:
	थांबा;
      स्थिती HB_PAINT_COMPOSITE_MODE_SRC_IN:
      स्थिती HB_PAINT_COMPOSITE_MODE_DEST_IN:
	backdrop_bounded = backdrop_bounded && src_bounded;
	थांबा;
      पूर्वनिर्धारित:
	backdrop_bounded = backdrop_bounded || src_bounded;
	थांबा;
     }
  }

  शून्य paint ()
  {
    जर (!clips)
      bounded = असत्य;
  }

  संरक्षित:
  तार्किक bounded; // true if current drawing bounded
  अचिन्हित clips; // number of active clips
  hb_vector_t<तार्किक> groups; // true if group bounded
};

HB_INTERNAL hb_paint_funcs_t *
hb_paint_bounded_get_funcs ();


#endif /* HB_PAINT_BOUNDED_HH */
