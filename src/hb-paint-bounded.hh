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

#অন্তর্ভাৱ "hb.hh"
#অন্তর্ভাৱ "hb-paint.h"

#অন্তর্ভাৱ "hb-geometry.hh"


প্রকারনির্দেশ সংরচনা  hb_paint_bounded_context_t hb_paint_bounded_context_t;

সংরচনা hb_paint_bounded_context_t
{
  শূন্য clear ()
  {
    clips = 0;
    bounded = সঁচা;
    groups.clear ();
  }

  hb_paint_bounded_context_t ()
  {
    clear ();
  }

  তার্কিক is_bounded ()
  {
    ঘূৰাই bounded;
  }

  শূন্য ঠেলো_clip ()
  {
    clips++;
  }

  শূন্য বেৰ_কৰো_clip ()
  {
    যদি (clips == 0) ঘূৰাই;
    clips--;
  }

  শূন্য ঠেলো_group ()
  {
    groups.push (bounded);
    bounded = সঁচা;
  }

  শূন্য বেৰ_কৰো_group (hb_paint_composite_mode_t mode)
  {
    ধ্রুৱ তার্কিক src_bounded = bounded;
    bounded = groups.বেৰ_কৰো ();
    তার্কিক &backdrop_bounded = bounded;

    // https://learn.microsoft.com/en-us/typography/opentype/spec/colr#format-32-paintcomposite
    ৱিকল্প ((পূর্ণাঙ্ক) mode)
    {
      পক্ষ HB_PAINT_COMPOSITE_MODE_CLEAR:
	backdrop_bounded = সঁচা;
	ভঙ্গ;
      পক্ষ HB_PAINT_COMPOSITE_MODE_SRC:
      পক্ষ HB_PAINT_COMPOSITE_MODE_SRC_OUT:
	backdrop_bounded = src_bounded;
	ভঙ্গ;
      পক্ষ HB_PAINT_COMPOSITE_MODE_DEST:
      পক্ষ HB_PAINT_COMPOSITE_MODE_DEST_OUT:
	ভঙ্গ;
      পক্ষ HB_PAINT_COMPOSITE_MODE_SRC_IN:
      পক্ষ HB_PAINT_COMPOSITE_MODE_DEST_IN:
	backdrop_bounded = backdrop_bounded && src_bounded;
	ভঙ্গ;
      স্ৱভাৱ:
	backdrop_bounded = backdrop_bounded || src_bounded;
	ভঙ্গ;
     }
  }

  শূন্য paint ()
  {
    যদি (!clips)
      bounded = মিছা;
  }

  রক্ষিত:
  তার্কিক bounded; // true if current drawing bounded
  unsigned clips; // number of active clips
  hb_vector_t<তার্কিক> groups; // true if group bounded
};

HB_INTERNAL hb_paint_funcs_t *
hb_paint_bounded_get_funcs ();


#endif /* HB_PAINT_BOUNDED_HH */
