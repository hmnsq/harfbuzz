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

#شامل "hb.hh"
#شامل "hb-paint.h"

#شامل "hb-geometry.hh"


تعریفنوع ساختار  hb_paint_bounded_context_t hb_paint_bounded_context_t;

ساختار hb_paint_bounded_context_t
{
  تهی clear ()
  {
    clips = 0;
    bounded = درست;
    groups.clear ();
  }

  hb_paint_bounded_context_t ()
  {
    clear ();
  }

  منطقی is_bounded ()
  {
    بازگشت bounded;
  }

  تهی افزودن_clip ()
  {
    clips++;
  }

  تهی برداشت_clip ()
  {
    اگر (clips == 0) بازگشت;
    clips--;
  }

  تهی افزودن_group ()
  {
    groups.push (bounded);
    bounded = درست;
  }

  تهی برداشت_group (hb_paint_composite_mode_t mode)
  {
    ثابت منطقی src_bounded = bounded;
    bounded = groups.برداشت ();
    منطقی &backdrop_bounded = bounded;

    // https://learn.microsoft.com/en-us/typography/opentype/spec/colr#format-32-paintcomposite
    انتخاب ((صحیح) mode)
    {
      حالت HB_PAINT_COMPOSITE_MODE_CLEAR:
	backdrop_bounded = درست;
	بشکن;
      حالت HB_PAINT_COMPOSITE_MODE_SRC:
      حالت HB_PAINT_COMPOSITE_MODE_SRC_OUT:
	backdrop_bounded = src_bounded;
	بشکن;
      حالت HB_PAINT_COMPOSITE_MODE_DEST:
      حالت HB_PAINT_COMPOSITE_MODE_DEST_OUT:
	بشکن;
      حالت HB_PAINT_COMPOSITE_MODE_SRC_IN:
      حالت HB_PAINT_COMPOSITE_MODE_DEST_IN:
	backdrop_bounded = backdrop_bounded && src_bounded;
	بشکن;
      پیشفرض:
	backdrop_bounded = backdrop_bounded || src_bounded;
	بشکن;
     }
  }

  تهی paint ()
  {
    اگر (!clips)
      bounded = نادرست;
  }

  محافظتشده:
  منطقی bounded; // true if current drawing bounded
  unsigned clips; // number of active clips
  hb_vector_t<منطقی> groups; // true if group bounded
};

HB_INTERNAL hb_paint_funcs_t *
hb_paint_bounded_get_funcs ();


#endif /* HB_PAINT_BOUNDED_HH */
