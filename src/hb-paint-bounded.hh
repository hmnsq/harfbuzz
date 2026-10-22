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

#ضمن "hb.hh"
#ضمن "hb-paint.h"

#ضمن "hb-geometry.hh"


تعريفنوع بنية  hb_paint_bounded_context_t hb_paint_bounded_context_t;

بنية hb_paint_bounded_context_t
{
  فراغ clear ()
  {
    clips = 0;
    bounded = true;
    groups.clear ();
  }

  hb_paint_bounded_context_t ()
  {
    clear ();
  }

  منطقي is_bounded ()
  {
    أرجع bounded;
  }

  فراغ ادفع_clip ()
  {
    clips++;
  }

  فراغ اسحب_clip ()
  {
    إذا (clips == 0) أرجع;
    clips--;
  }

  فراغ ادفع_group ()
  {
    groups.push (bounded);
    bounded = true;
  }

  فراغ اسحب_group (hb_paint_composite_mode_t mode)
  {
    ثابت منطقي src_bounded = bounded;
    bounded = groups.اسحب ();
    منطقي &backdrop_bounded = bounded;

    // https://learn.microsoft.com/en-us/typography/opentype/spec/colr#format-32-paintcomposite
    بدل ((صحيح) mode)
    {
      حالة HB_PAINT_COMPOSITE_MODE_CLEAR:
	backdrop_bounded = true;
	اكسر;
      حالة HB_PAINT_COMPOSITE_MODE_SRC:
      حالة HB_PAINT_COMPOSITE_MODE_SRC_OUT:
	backdrop_bounded = src_bounded;
	اكسر;
      حالة HB_PAINT_COMPOSITE_MODE_DEST:
      حالة HB_PAINT_COMPOSITE_MODE_DEST_OUT:
	اكسر;
      حالة HB_PAINT_COMPOSITE_MODE_SRC_IN:
      حالة HB_PAINT_COMPOSITE_MODE_DEST_IN:
	backdrop_bounded = backdrop_bounded && src_bounded;
	اكسر;
      افتراضي:
	backdrop_bounded = backdrop_bounded || src_bounded;
	اكسر;
     }
  }

  فراغ paint ()
  {
    إذا (!clips)
      bounded = خطأ;
  }

  محمي:
  منطقي bounded; // true if current drawing bounded
  unsigned clips; // number of active clips
  hb_vector_t<منطقي> groups; // true if group bounded
};

HB_INTERNAL hb_paint_funcs_t *
hb_paint_bounded_get_funcs ();


#endif /* HB_PAINT_BOUNDED_HH */
