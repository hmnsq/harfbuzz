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

#جيڪڏهن_وصف_نه_ٿيل HB_PAINT_BOUNDED_HH
#وصف_ڪريو HB_PAINT_BOUNDED_HH

#ضمن "hb.hh"
#ضمن "hb-paint.h"

#ضمن "hb-geometry.hh"


تعريفنوع بنية  hb_paint_bounded_context_t hb_paint_bounded_context_t;

بنية hb_paint_bounded_context_t
{
  فراغ clear ()
  {
    clips = 0;
    bounded = سچ;
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

  فراغ ڌڪو_clip ()
  {
    clips++;
  }

  فراغ ڪڍو_clip ()
  {
    إذا (clips == 0) أرجع;
    clips--;
  }

  فراغ ڌڪو_گروهه ()
  {
    groups.push (bounded);
    bounded = سچ;
  }

  فراغ ڪڍو_گروهه (hb_paint_مرکب_گهڻائي_t گهڻائي)
  {
    ثابت منطقي src_bounded = bounded;
    bounded = groups.ڪڍو ();
    منطقي &backdrop_bounded = bounded;

    // https://learn.microsoft.com/en-us/typography/opentype/spec/colr#format-32-paintcomposite
    بدل ((صحيح) گهڻائي)
    {
      حالة HB_PAINT_مرکب_گهڻائي_CLEAR:
	backdrop_bounded = سچ;
	اكسر;
      حالة HB_PAINT_مرکب_گهڻائي_SRC:
      حالة HB_PAINT_مرکب_گهڻائي_SRC_ٻاهريون:
	backdrop_bounded = src_bounded;
	اكسر;
      حالة HB_PAINT_مرکب_گهڻائي_DEST:
      حالة HB_PAINT_مرکب_گهڻائي_DEST_ٻاهريون:
	اكسر;
      حالة HB_PAINT_مرکب_گهڻائي_SRC_اندريون:
      حالة HB_PAINT_مرکب_گهڻائي_DEST_اندريون:
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
  بنانشان clips; // number of active clips
  hb_ويڪٽر_t<منطقي> groups; // true if group bounded
};

HB_اندروني hb_paint_funcs_t *
hb_paint_bounded_get_funcs ();


#جيڪڏهن_پورو /* HB_PAINT_BOUNDED_HH */
