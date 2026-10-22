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

#که_نه_تعریف_شوی HB_PAINT_BOUNDED_HH
#تعریف_کړه HB_PAINT_BOUNDED_HH

#ضمن "hb.hh"
#ضمن "hb-paint.h"

#ضمن "hb-geometry.hh"


تعريفنوع بنية  hb_paint_bounded_context_t hb_paint_bounded_context_t;

بنية hb_paint_bounded_context_t
{
  فراغ clear ()
  {
    clips = 0;
    bounded = ریښتیا;
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

  فراغ ټېل_clip ()
  {
    clips++;
  }

  فراغ راوباسه_clip ()
  {
    إذا (clips == 0) أرجع;
    clips--;
  }

  فراغ ټېل_ډله ()
  {
    groups.push (bounded);
    bounded = ریښتیا;
  }

  فراغ راوباسه_ډله (hb_paint_مرکب_ډېرپېښ_t ډېرپېښ)
  {
    ثابت منطقي src_bounded = bounded;
    bounded = groups.راوباسه ();
    منطقي &backdrop_bounded = bounded;

    // https://learn.microsoft.com/en-us/typography/opentype/spec/colr#format-32-paintcomposite
    بدل ((صحيح) ډېرپېښ)
    {
      حالة HB_PAINT_مرکب_ډېرپېښ_CLEAR:
	backdrop_bounded = ریښتیا;
	اكسر;
      حالة HB_PAINT_مرکب_ډېرپېښ_SRC:
      حالة HB_PAINT_مرکب_ډېرپېښ_SRC_بهر:
	backdrop_bounded = src_bounded;
	اكسر;
      حالة HB_PAINT_مرکب_ډېرپېښ_DEST:
      حالة HB_PAINT_مرکب_ډېرپېښ_DEST_بهر:
	اكسر;
      حالة HB_PAINT_مرکب_ډېرپېښ_SRC_دننه:
      حالة HB_PAINT_مرکب_ډېرپېښ_DEST_دننه:
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
  بینښه clips; // number of active clips
  hb_ویکټور_t<منطقي> groups; // true if group bounded
};

HB_دننی hb_paint_funcs_t *
hb_paint_bounded_get_funcs ();


#د_که_پای /* HB_PAINT_BOUNDED_HH */
