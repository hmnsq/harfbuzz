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

#অন্তর্ভাব "hb.hh"
#অন্তর্ভাব "hb-paint.h"

#অন্তর্ভাব "hb-geometry.hh"


প্রকারলেখ কাঠামো  hb_paint_bounded_context_t hb_paint_bounded_context_t;

কাঠামো hb_paint_bounded_context_t
{
  শূন্য clear ()
  {
    clips = 0;
    bounded = সত্য;
    groups.clear ();
  }

  hb_paint_bounded_context_t ()
  {
    clear ();
  }

  যৌক্তিক is_bounded ()
  {
    ফেরত bounded;
  }

  শূন্য ঠেলো_clip ()
  {
    clips++;
  }

  শূন্য বের_করো_clip ()
  {
    যদি (clips == 0) ফেরত;
    clips--;
  }

  শূন্য ঠেলো_group ()
  {
    groups.push (bounded);
    bounded = সত্য;
  }

  শূন্য বের_করো_group (hb_paint_composite_mode_t mode)
  {
    ধ্রুবক যৌক্তিক src_bounded = bounded;
    bounded = groups.বের_করো ();
    যৌক্তিক &backdrop_bounded = bounded;

    // https://learn.microsoft.com/en-us/typography/opentype/spec/colr#format-32-paintcomposite
    নির্বাচন ((পূর্ণসংখ্যা) mode)
    {
      ক্ষেত্র HB_PAINT_COMPOSITE_MODE_CLEAR:
	backdrop_bounded = সত্য;
	বিরতি;
      ক্ষেত্র HB_PAINT_COMPOSITE_MODE_SRC:
      ক্ষেত্র HB_PAINT_COMPOSITE_MODE_SRC_OUT:
	backdrop_bounded = src_bounded;
	বিরতি;
      ক্ষেত্র HB_PAINT_COMPOSITE_MODE_DEST:
      ক্ষেত্র HB_PAINT_COMPOSITE_MODE_DEST_OUT:
	বিরতি;
      ক্ষেত্র HB_PAINT_COMPOSITE_MODE_SRC_IN:
      ক্ষেত্র HB_PAINT_COMPOSITE_MODE_DEST_IN:
	backdrop_bounded = backdrop_bounded && src_bounded;
	বিরতি;
      পূর্বনির্ধারিত:
	backdrop_bounded = backdrop_bounded || src_bounded;
	বিরতি;
     }
  }

  শূন্য paint ()
  {
    যদি (!clips)
      bounded = মিথ্যা;
  }

  সুরক্ষিত:
  যৌক্তিক bounded; // true if current drawing bounded
  unsigned clips; // number of active clips
  hb_vector_t<যৌক্তিক> groups; // true if group bounded
};

HB_INTERNAL hb_paint_funcs_t *
hb_paint_bounded_get_funcs ();


#endif /* HB_PAINT_BOUNDED_HH */
